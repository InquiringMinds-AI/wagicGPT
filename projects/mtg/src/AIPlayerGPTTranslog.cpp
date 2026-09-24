#include "PrecompiledHeader.h"

#ifdef WITH_GPT_AI

#include "AIPlayerGPT.h"
#include "AIPlayerGPTSelfTestAccess.h" //PARSETEST test-access seam + the shared internal types
#include "LegalActions.h"
#include "PlayRestrictions.h" //#W61-T (C7): the cast oracle's play-restriction gate
#include "GptPlanCaveat.h"
#include "DecisionContract.h"
#include <chrono>
#include "GptConfig.h"
#include "GameObserver.h"
#include "MTGDefinitions.h"
#include "WEvent.h"
#include "Token.h" //W35: a token's creator (Token::tokenSource) for the log
#include "Damage.h"
#include "PhaseRing.h"
#include "JFileSystem.h"
#include "MTGAbility.h"
#include "AbilityParser.h" //N-158m: AutoLineMacro::Process, for macro-defined magnitudes
#include "CardDescriptor.h"
#include "ManaCost.h"
#include "ManaCostHybrid.h"
#include "ManaEngine.h"
#include "ExtraCost.h"
#include "Counters.h"
#include "ActionLayer.h"
#include "SimpleMenuItem.h" //#W78-CX (S10): the deferred menu's own row text
#include "AllAbilities.h"
#include "ActionStack.h"
#include "WFont.h"
#include "WResourceManager.h"
#include "GuiCombat.h"
#include "DuelLayers.h"
#include "PreGamePhase.h" //PreGamePhase::bottomTarget, driven by the self-test

#ifndef WAGIC_NO_CURL
#include <curl/curl.h>
#endif
#include <nlohmann/json.hpp>

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
#include <set>
#include <algorithm>
#include <thread>
#include <mutex>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define GPT_MKDIR(p) _mkdir(p) //Windows mkdir takes no mode argument
#else
#define GPT_MKDIR(p) mkdir((p), 0755)
#endif
#include <ctime>

using json = nlohmann::json;
#include "AIPlayerGPTInternal.h" //#W82-P12

//#W82-P12: TELEMETRY - the translog records (window and side), gameend
//counters, notices. Moved verbatim out of AIPlayerGPT.cpp; see AIPlayerGPTInternal.h.


bool AIPlayerGPT::isEnabled()
{
    //The environment overrides in both directions (WAGIC_AI=gpt forces the
    //LLM opponent on, any other value forces it off); without it, the GPT
    //options tab's master switch (enabled= in the config file) decides.
    const char * mode = getenv("WAGIC_AI");
    if (mode && *mode)
        return string(mode) == "gpt";
    return GptSettings::load().enabled == 1;
}


void AIPlayerGPT::setNotice(const string& text, float seconds)
{
    mNotice = text;
    mNoticeTicks = (int) (seconds * 60); //decremented per rendered frame
}


//The model did not answer this decision; the heuristic did. Say so, and keep
//saying so quietly for a while after the notice fades - a run of fallbacks is
//a different thing from one, and the player deserves to see which they are in.
void AIPlayerGPT::noticeFallback(const string& text, float seconds)
{
    setNotice(text, seconds);
    mFallbackCount++;
    mDegradedTicks = 45 * 60; //~45s since the last one; lapses if it recovers
}



//#W72-BT (M22, wave-71 engine-seat MED-2): the same predicate, PER WINDOW.
//The single-slot version above is reset by ANY other key - including a window
//the model really answered in between - so the wave-71 sidecar shows an
//18-replay run of one byte-identical casting window whose counter restarted at
//1 halfway through (`...deck123-0x5629c69e3090`, turn 36, seqs 363-383). A
//livelock that alternates between two windows can never reach the cap that way,
//which is the one thing the cap exists for. One counter per state+question:
//interleaving cannot reset a run, and the map is dropped with the ask cache at
//the turn boundary (the keys embed the turn header, so they are dead anyway)
//and erased for a window the moment that window is really answered. Fires and
//re-arms exactly as the single-slot form does. Pure and static.
bool AIPlayerGPT::askReplayRefuseScoped(const std::string & key,
                                        std::map<std::string, int> & runs, int maxRun)
{
    if (key.empty())
        return false;
    int & run = runs[key];
    run++;
    if (run < maxRun)
        return false;
    run = 0;
    return true;
}


//#W71-BP (L2): the record a re-served answer writes. Compact ON PURPOSE - the
//point is that the log stops being SILENT, not that a replay costs as much as a
//decision - so it carries only what tells a reader "this window was answered
//without asking, from there, N times running". Bounded by the refusal above.
//#W71-BS (F4, Astra review finding 4, second half). A replay record is NOT
//progress: nothing was asked, nothing was decided, and the seat may be spinning.
//Written into the main translog it moved the file's mtime every iteration, which
//is exactly the signal BOTH harness silence arms read (tools/selfplay-harness.sh
//no_progress_sweep takes max(getmtime) over the seat logs), so making the hang
//visible would have made the watchdog blind to it. The records are kept - a hung
//seat log must never again be silent - but they live in a SIDECAR beside the
//translog, under `askreplay/` with the same basename. The sweep's glob is
//non-recursive over LOGDIR, so a subdirectory is invisible to it and visible to a
//reviewer, and the sidecar's own mtime is the spin's timeline.
string AIPlayerGPT::askReplaySidecarPath() const
{
    if (mTransLogPath.empty())
        return string();
    const size_t slash = mTransLogPath.find_last_of('/');
    const string dir = (slash == string::npos) ? string() : mTransLogPath.substr(0, slash + 1);
    const string base = (slash == string::npos) ? mTransLogPath : mTransLogPath.substr(slash + 1);
    return dir + "askreplay/" + base;
}


//#W73-CA (N8, wave-72 engine-seat MED-2): `replay_run` meant TWO different
//things. The `cache_replay` path passed this window's own run; the
//`repeat_ask_reserved` path passed `mRepeatAskAnswersReserved`, the GAME's
//running total - so the corpus's maximum `replay_run` read 90 against a cap of
//64 (a breach that never happened) while the true maximum per-window run was
//11. One field, one meaning: `replay_run` is ALWAYS this window's consecutive
//run against the same key, and the game total moves to its own field.
//#W73-CB (F7): `replays_this_game` is ALWAYS mAskReplaysReserved - this game's
//combined total of answers re-served without asking, over BOTH paths.
void AIPlayerGPT::logAskReplay(const char * why, const string & decision, int choice,
                               int optionCount, int fromSeq, int run)
{
    if (mTransLogPath.empty())
        return;
    ensureGameStartRecord();
    json rec = {
        {"seq", mTransSeq++},
        {"kind", "ask_replay"},
        {"why", why ? why : ""},
        {"replayed_from", fromSeq},
        {"replay_run", run},
        {"replays_this_game", mAskReplaysReserved}, //#W73-CB (F7)
        {"choice", choice},
        {"options", optionCount},
        {"turn", translogTurn(observer ? observer->turn : 0)},
        {"phase", observer ? observer->getCurrentGamePhase() : -1},
        {"question", decision},
    };
    //#W71-BS (F4): the sidecar, never the translog the watchdog times.
    const string side = askReplaySidecarPath();
    if (side.empty())
        return;
    const size_t slash = side.find_last_of('/');
    if (slash != string::npos)
        GPT_MKDIR(side.substr(0, slash).c_str());
    std::ofstream f(side.c_str(), std::ios::app);
    if (f)
        f << rec.dump() << "\n";
}


//#W78-CV (S11, wave-77 engine-seat M1 / deck126 MED-5). NAME EVERY FORCED CLOSE.
//`forced_close_unrecorded` went 5 -> 10 while every arm-bound counter stayed 0 for
//the fourth wave, and three waves of briefs have asked reviewers to "name each of
//the ten and what superseded it" from a bare integer on `gameend`. The identity
//(`recovered + missing + unrecorded == stderr closes`) held every time and named
//nothing. One record per EVENT - which arm, which window, what happened to it -
//makes the class decidable from the corpus instead of from stderr arithmetic.
//It is telemetry about a window still in flight, so it rides that window's number
//exactly as `wall_miss` does (kind is not a window record) and takes no seq of the
//window counter.
void AIPlayerGPT::writeForceCloseRecord(const char * outcome, bool landArm)
{
    mForceCloseEvents++;
    if (mTransLogPath.empty())
        return;
    ensureGameStartRecord();
    //#W82-P11: held until the window record it joins (same window ordinal) is
    //written, and folded onto it; a window that never writes a record leaves
    //this as a side record instead (flushForceCloseFold).
    ForceCloseEvent e;
    e.event = mForceCloseEvents;
    e.outcome = outcome ? outcome : "";
    e.arm = landArm ? "land" : "cast";
    e.windowSeq = mWindowSeq;
    e.parkArmed = mRetryPark.forceCloseArmed ? 1 : 0;
    e.deferTicks = mForceCloseDeferTicks;
    e.unrecordedSoFar = mForceCloseUnrecorded;
    e.turn = translogTurn(observer ? observer->turn : 0);
    e.phase = observer ? observer->getCurrentGamePhase() : -1;
    mForceCloseFold.push_back(e);
}


//#W80-DH (F11, Astra wave-80 review finding 11 - MED). ONE RECORD PER HOLD EVENT.
//The wave-80 shape incremented three counters immediately and wrote their story
//into ONE shared string each, consumed by the next `writeTransLog` of any kind:
//multiple clamps in one window retained at most one field, and the next unrelated
//target or discard record inherited whatever was left - the exact
//counter-without-an-adjudicable-trace failure the LESSON OF WAVE 79 names. This is
//the event, at the event, with its seam, its face, its reason and the window it
//happened in. It is telemetry about a window that may never be asked, so (like
//`forced_close` and `menu_single_outcome`) it takes no ask seq of the window
//counter. `hold_events` on the gameend record equals the number of these records,
//and the sum of the three class counters equals it too.
//(history: comment-archaeology.md AIPlayerGPTTranslog-AIPlayerGPT-w81InvalidateHeldAnswer-1708)
bool AIPlayerGPT::w81InvalidateHeldAnswer(const char * seam)
{
    const string s(seam ? seam : "");
    bool did = false;
    std::map<string, string>::iterator hk = mHoldAskKey.find(s);
    if (hk != mHoldAskKey.end())
    {
        if (!hk->second.empty())
        {
            if (mAskCache.erase(hk->second))
                did = true;
            mAskCacheSeq.erase(hk->second);
            mAskReplayRuns.erase(hk->second);
        }
        mHoldAskKey.erase(hk);
    }
    //#W71-BS (F4): BOTH re-serve paths, or neither - the repeat latch answers the
    //same window from the same retained choice.
    if (!mRepeatAskKey.empty())
    {
        mRepeatAskKey.clear();
        mRepeatAskTurn = -1;
        mRepeatAskSeq = -1;
        did = true;
    }
    //...and the priority seam's own retained answer, which is `mLastChoice` under an
    //unchanged key rather than a map entry.
    if (s == "priority" && !mLastAskKey.empty())
    {
        mLastAskKey.clear();
        did = true;
    }
    if (did)
    {
        mHoldReopenAnswerInvalidated++;
        DebugTrace("AIPlayerGPT: the hold re-opened at the " << s << " seam - the retained"
                   " answer for that window is invalidated, so the window goes to the model"
                   " rather than being re-served the hold row the re-open just retired ("
                   << mHoldReopenAnswerInvalidated << " this game)");
    }
    return did;
}


void AIPlayerGPT::writeHoldEventRecord(const char * event, const char * seam,
                                       const string& face, const string& reason,
                                       int answerInvalidated)
{
    //#W82-A R1 (LEDGER v2): restored. Deleting `mHoldEvents` also silently
    //removed the `event` SEQUENCE NUMBER from every hold_event record - the
    //field a reviewer joins the 180 records on. `hold_events` was NOT zero in
    //the wave-80 corpus (180), which is the second half of Astra's refutation.
    mHoldEvents++;
    if (mTransLogPath.empty())
        return;
    ensureGameStartRecord();
    json rec = {
        {"seq", mTransSeq++},
        {"kind", "hold_event"},
        {"event", mHoldEvents}, //#W82-A R1: restored
        {"what", event ? event : ""},
        {"seam", seam ? seam : ""},
        {"face", face},
        {"reason", reason},
        {"window_seq", mWindowSeq},
        //#W81-DK (V15, wave-80 known-bugs V15): the RECORD seq of the window this
        //event refers to - the numbering `seq`, `replayed_from` and `recovers_seq`
        //all use. `window_seq` is the WINDOW ordinal and the two diverge the moment
        //any sidecar record advances mTransSeq without advancing mWindowSeq, which
        //made a wave-80 hold at record 89 read `window_seq 42`. -1 = no window
        //record has been written at this seat yet.
        {"window_record_seq", mLastWindowRecordSeq},
        //#W81-DK (V3): did this event invalidate the retained answer for the
        //window's key (so the re-opened window is ASKED)? 1 = yes, 0 = nothing was
        //retained, -1 = not a re-open. The counter
        //`hold_reopen_answer_invalidated` is the join.
        {"answer_invalidated", answerInvalidated},
        {"turn", translogTurn(observer ? observer->turn : 0)},
        {"phase", observer ? observer->getCurrentGamePhase() : -1},
    };
    transLogWrite(rec.dump());
}



//One header record per seat log: decks, names, and a game_id BOTH seats
//share (the observer's address) - reviewers previously paired seat logs by
//filename epoch arithmetic, which broke on harvested copies (wave-7 7d).
void AIPlayerGPT::ensureGameStartRecord()
{
    if (mGameStartLogged || mTransLogPath.empty())
        return;
    mGameStartLogged = true;
    //Finalize the filename with the OPPONENT deck token now that the
    //opponent exists (it does not at construction time): reviewers mapped
    //game->file by cross-referencing results.tsv epochs, fragile on
    //harvested copies (wave-9 ledger 5b). This runs before the first
    //append, so the file has not been created yet under the old name.
    if (opponent() && !opponent()->deckFileSmall.empty())
    {
        size_t dot = mTransLogPath.rfind(".jsonl");
        if (dot != string::npos && mTransLogPath.find("-vs-") == string::npos)
            mTransLogPath = mTransLogPath.substr(0, dot) + "-vs-" + opponent()->deckFileSmall + ".jsonl";
    }
    //#W77-CU (F10, Astra MED 10): THE GAME NAMES ITS OWN SEAT LOGS. The harness
    //harvested by deck pair plus a time tolerance, which is not identity: with
    //two pools running it accepted a foreign seat five seconds away and rejected
    //a legitimate one sixteen seconds away, and two concurrent games of the SAME
    //pair were indistinguishable at any tolerance. The file name is known only
    //here - it carries this player's own pointer - so the process that owns it
    //announces it on its own stderr, which the harness already captures per
    //game. Printed once per seat, only when the translog (an opt-in telemetry
    //feature) is on, so no release build gains a line it did not already have.
    if (!mTransLogPath.empty())
    {
        const size_t slash = mTransLogPath.rfind('/');
        fprintf(stderr, "WAGIC_GPT_TRANSLOG_FILE %s\n",
                mTransLogPath.c_str() + (slash == string::npos ? 0 : slash + 1));
        fflush(stderr);
    }
    std::ostringstream gid;
    gid << (void *) observer;
    json rec = {
        {"seq", mTransSeq++},
        {"kind", "gamestart"},
        {"model", mModel},
        {"game_id", gid.str()},
        {"my_deck", deckFileSmall},
        {"my_deck_name", deckName},
        {"opp_deck", opponent() ? opponent()->deckFileSmall : ""},
        {"opp_deck_name", opponent() ? opponent()->deckName : ""},
    };
    transLogWrite(rec.dump());
}


//audit-L (L4): one lazily-opened stream per seat, flushed per record, in place
//of an open/append/close per record (the file's own #W-refusal note names a
//per-event open on the game thread as a measured cause of console lag). The
//stream opens on the FIRST write, which ensureGameStartRecord has already
//renamed to its final -vs- name; every record is flushed, so a process that
//exits without destroying the seat loses nothing. WAGIC_GPT_TRANSLOG_STREAM=0
//restores the per-record open.
void AIPlayerGPT::transLogWrite(const string& line)
{
    if (mTransLogPath.empty())
        return;
    static int streamOn = -1;
    if (streamOn < 0)
    {
        const char * e = getenv("WAGIC_GPT_TRANSLOG_STREAM");
        streamOn = (e && *e == '0') ? 0 : 1;
    }
    if (!streamOn)
    {
        std::ofstream f(mTransLogPath.c_str(), std::ios::app);
        if (f)
            f << line << "\n";
        return;
    }
    if (!mTransLog.is_open())
        mTransLog.open(mTransLogPath.c_str(), std::ios::app);
    if (mTransLog)
    {
        mTransLog << line << "\n";
        mTransLog.flush();
    }
}

//#W76-CQ (F9): the same walk may also HAND BACK the off-protocol lines. The
//byte count alone cannot answer "were these bytes the model's own prose or the
//forced close's own prefill handed back?", which is the question the deviation
//class claims to answer.
long offProtocolBytes(const string& replyIn, bool * actionBeforePlanOut,
                             std::vector<string> * offLinesOut)
{
    if (actionBeforePlanOut)
        *actionBeforePlanOut = false;
    string text = replyIn;
    const size_t thinkEnd = text.rfind("</think>");
    if (thinkEnd != string::npos)
        text = text.substr(thinkEnd + 8);
    const size_t planAt = firstLineLeadingPlanPos(text);
    size_t labelAt = string::npos;
    scanCodedAnswerLines(text, NULL, NULL, NULL, &labelAt);
    size_t planLine = string::npos, ansLine = string::npos;
    if (planAt != string::npos)
    {
        const size_t b = text.rfind('\n', planAt);
        planLine = (b == string::npos) ? 0 : b + 1;
    }
    if (labelAt != string::npos)
    {
        const size_t b = text.rfind('\n', labelAt);
        ansLine = (b == string::npos) ? 0 : b + 1;
    }
    if (actionBeforePlanOut && planLine != string::npos && ansLine != string::npos)
        *actionBeforePlanOut = (ansLine < planLine);
    long off = 0;
    size_t at = 0;
    while (at <= text.size())
    {
        const size_t nl = text.find('\n', at);
        const size_t end = (nl == string::npos) ? text.size() : nl;
        if (at != planLine && at != ansLine && end > at)
        {
            const size_t s = text.find_first_not_of(" \t\r", at);
            if (s != string::npos && s < end)
            {
                const size_t e = text.find_last_not_of(" \t\r", end - 1);
                if (e != string::npos && e >= s)
                {
                    off += (long) (e - s + 1);
                    if (offLinesOut) //#W76-CQ (F9)
                        offLinesOut->push_back(text.substr(s, e - s + 1));
                }
            }
        }
        if (nl == string::npos)
            break;
        at = nl + 1;
    }
    return off;
}


//#W70-BN (F4, Astra review finding 4): THE SPAN A STOP MAY BE READ FROM. The
//reply is the PLAN line and the action line; a stop number found anywhere else
//is off-protocol text, and wave 70 let it both supply the constraint and, on the
//retry-exhaustion arm, clamp a legal counted action to zero ("Correction: stop
//20; M 25" under a number-free plan turned a legal x3 into a pass). This returns
//the two permitted lines and nothing else - in text order, newline-joined, so
//every scan below reads exactly what the protocol allows the model to say.
//Pure over one string; PARSETEST pins it.
static string protocolLinesOnly(const string& replyIn)
{
    string text = replyIn;
    const size_t thinkEnd = text.rfind("</think>");
    if (thinkEnd != string::npos)
        text = text.substr(thinkEnd + 8);
    const size_t planAt = firstLineLeadingPlanPos(text);
    size_t labelAt = string::npos;
    scanCodedAnswerLines(text, NULL, NULL, NULL, &labelAt);
    size_t keep[2] = { string::npos, string::npos };
    if (planAt != string::npos)
    {
        const size_t b = text.rfind('\n', planAt);
        keep[0] = (b == string::npos) ? 0 : b + 1;
    }
    if (labelAt != string::npos)
    {
        const size_t b = text.rfind('\n', labelAt);
        keep[1] = (b == string::npos) ? 0 : b + 1;
    }
    string out;
    size_t at = 0;
    while (at <= text.size())
    {
        const size_t nl = text.find('\n', at);
        const size_t end = (nl == string::npos) ? text.size() : nl;
        if (at == keep[0] || at == keep[1])
        {
            if (!out.empty())
                out += '\n';
            out += text.substr(at, end - at);
        }
        if (nl == string::npos)
            break;
        at = nl + 1;
    }
    return out;
}


//#W73-BZ (N3): the stop scan's span is the PLAN LINE, and only that line. The
//stop-and-count store used to be fed `protocolLinesOnly`, which is the PLAN line
//AND the action line - so a count echoed in a CHOICE parenthetical could supply
//half of a "stated stop" the model never stated in its plan. The plan is where
//the row's own bracket says to write it; a labelled number on THAT line is the
//whole permitted read (invariant 000). Returns "" when the reply has no plan
//line. Pure over the reply text.
string planLineOnly(const string& replyIn)
{
    const string two = protocolLinesOnly(replyIn);
    size_t at = 0;
    while (at <= two.size())
    {
        const size_t nl = two.find('\n', at);
        const size_t end = (nl == string::npos) ? two.size() : nl;
        const string line = two.substr(at, end - at);
        string low;
        for (size_t i = 0; i < line.size(); i++)
            low += (char) tolower((unsigned char) line[i]);
        const size_t b = low.find_first_not_of(" \t");
        if (b != string::npos && low.compare(b, 5, "plan:") == 0)
            return line;
        if (nl == string::npos)
            break;
        at = nl + 1;
    }
    return string();
}


//#W70-BM (E3, audit B4.6): set by parseAttackerSet on the call that stripped a
//trailing gloss list off a PUT answer, consumed by the record writer. Instrument
//only - nothing branches on it, and it is set and read on the same thread.
extern bool gLastPutGlossStripped = false;


//#W70-BM (E2): the reply wrote its action line ABOVE its plan line. Parsed, but
//stamped for the census - never silently accepted as the intended shape.
static bool replyActionBeforePlan(const string& replyIn)
{
    bool before = false;
    offProtocolBytes(replyIn, &before);
    return before;
}


//#W71-BO (R1, wave-70 census): THE RECORD TRIM IS DELETED. `recordReplyTrimmed`
//(#W61-U C14) existed because replies used to run thousands of bytes past their
//own answer; under reasoning-on the whole corpus wrote 2 records with any
//trailing overrun at all (0.09% of 2,119) and none reached the 1,200-byte
//threshold. The record now stores the reply verbatim, and `reply_trimmed_bytes`
//goes with the trim - `off_protocol_bytes` below is the surviving meter.
//post_answer_overrun: the chars a reply wrote AFTER the end of its FIRST
//line-leading coded answer. THIS is the quantity the owner ruling is about,
//and post_plan_overrun is not: post_plan_overrun measures from the PLAN line,
//which the protocol put LAST, so it saw 5.3% of the real tail at deck116 and
//scored a flat 0 on a 13,326-char reply that simply omitted its PLAN. It is
//also format-blind in a way this is not - a reply with no PLAN line and a
//reply that ended at its answer are indistinguishable to it and identical
//(0) here only when the reply really did stop. Measured on the STRIPPED reply
//(post-</think>), like every other meter, or it would count thinking tokens.
static long postAnswerOverrun(const string& reply)
{
    size_t lineEnd = string::npos;
    if (scanCodedAnswerLines(reply, &lineEnd, NULL, NULL) < 1 || lineEnd == string::npos)
        return 0; //no coded answer at all: the empty/unparsed classes name that
    size_t last = reply.find_last_not_of(" \t\r\n");
    if (last == string::npos || last <= lineEnd)
        return 0;
    return (long) (last - lineEnd);
}


static bool answerReplaced(const string& reply)
{
    string first, last;
    int n = scanCodedAnswerLines(reply, NULL, &first, &last);
    return n > 1 && first != last;
}

int codedAnswerCount(const string& reply)
{
    const int n = scanCodedAnswerLines(reply, NULL, NULL, NULL);
    if (n > 0)
        return n;
    size_t a = 0, b = 0, c = 0;
    return gptInlineChoiceOnPlanLine(reply, a, b, c) ? 1 : 0;
}


//#W63-AD (E11, engine HIGH-6). WHAT `choice` MEANS IS NOT THE SAME AT EVERY
//SEAM. At the ask and priority seams it is the ROW the model picked. At the
//subset seams - reveal, attackers, blockers, discard, bottom - the value handed
//to writeTransLog is the SIZE of the selection (parseAttackerSet's return, or
//the blocker PAIR count): all 16 reveal records in the wave-62 corpus read
//`choice: 1` because a fixed <1> chooser takes exactly one card, whatever row it
//came from. Three row-index-derived stamps were nevertheless computed from it -
//`latched_coded_line`, `latched_row_mismatch` and `plan_contradicts_noop_row` -
//so 126v146 seq 66 (reply `CHOICE: 16 (Cast Sanguine Bond)`, the search took
//Sanguine Bond) carried a FALSE `latched_row_mismatch`, and the corpus's only
//instance of that stamp was a false positive. The stamps are gated on the kinds
//where the field is an index; the subset kinds say so on the record instead, so
//the number can never be read as a row again. Pure over the kind string.
static bool recordChoiceIsRowIndex(const char * kind)
{
    if (!kind || !*kind)
        return false;
    return strcmp(kind, "ask") == 0 || strcmp(kind, "priority") == 0
        || strcmp(kind, "defer") == 0;
}


//#W63-AD (E6a, deck146 HIGH-1). THE OVERRIDE NOBODY COULD AUDIT. When
//`answer_replaced` is true the record has said WHICH ordinal the engine latched
//(`latched_coded_line`, #W53-N D12b) since wave 53 - and `recordReplyTrimmed`
//(#W61-U C14) then cut the line itself out of the stored reply, because the
//latching token lives in exactly the overrun the trim exists to drop. 146v162
//seq 41 (`reply_trimmed_bytes: 2337`) and 146v126 seq 80 (`2551`) are 2 of 2
//answer_replaced records at that seat with the deciding line unrecoverable. So
//the line is lifted from the FULL reply, before the trim, and written verbatim
//on the record. Bounded (a coded line is one line; the cap only guards a reply
//with no newline at all) and pure over (reply, ordinal).
extern const size_t kLatchedLineKeep = 400;

//#W64-AJ (F13, engine seat HIGH-1/2): the line's OFFSET rides out with it.
//`codedAnswerLineInPlanBlock` used to locate the line again with
//`text.find(line)`, which is a FIRST-match search for a string the model's own
//deliberation prose frequently repeats verbatim - `130v126@1788653538` seq 29
//latched the line-leading `ATTACK: A4` on line 3 and the finder matched the
//"So ATTACK: A4 is the most efficient" sitting INSIDE the PLAN line above it,
//so the record read `latched_line_in_plan: true` for a line that is not in the
//plan block at all. The scan already knows where the line starts; nothing has
//to search for it.
string codedAnswerLineAt(const string& reply, int ordinal, size_t keep,
                                size_t * atOut)
{
    if (atOut)
        *atOut = string::npos;
    if (ordinal < 1)
        return string();
    static const char * kLabels[] = { "choice:", "attack:", "blocks:", "put:" };
    int count = 0;
    size_t lineStart = 0;
    while (lineStart <= reply.size())
    {
        size_t lineEnd = reply.find('\n', lineStart);
        const size_t end = (lineEnd == string::npos) ? reply.size() : lineEnd;
        size_t s = lineStart;
        while (s < end && (reply[s] == ' ' || reply[s] == '\t'
                           || reply[s] == '*' || reply[s] == '#' || reply[s] == '-'
                           || reply[s] == '>' || reply[s] == '`'))
            s++;
        for (size_t k = 0; k < sizeof(kLabels) / sizeof(kLabels[0]); k++)
        {
            const size_t len = strlen(kLabels[k]);
            if (end - s < len)
                continue;
            bool m = true;
            for (size_t q = 0; q < len && m; q++)
                m = (tolower((unsigned char) reply[s + q]) == kLabels[k][q]);
            if (!m)
                continue;
            size_t pp = s + len;
            while (pp < end && (reply[pp] == ' ' || reply[pp] == '\t')) pp++;
            if (pp >= end)
                break; //no payload: scanCodedAnswerLines does not count it either
            if (++count == ordinal)
            {
                if (atOut)
                    *atOut = s; //#W64-AJ (F13)
                string line = reply.substr(s, end - s);
                size_t e = line.find_last_not_of(" \t\r");
                line = (e == string::npos) ? string() : line.substr(0, e + 1);
                if (line.size() > keep)
                    line = line.substr(0, keep) + "...";
                return line;
            }
            break;
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    return string();
}

//#W64-AJ (F13, engine seat HIGH-1/2). The verdict AND the arithmetic behind it.
//Wave 63 wrote the boolean alone and the engine seat could not check it: the one
//record that carried `latched_line_in_plan: true` carried it FALSELY (see
//codedAnswerLineAt above), and nothing on the record let a reader see that. All
//three offsets are into the reply AS STORED (the full, untrimmed reply the
//record was written from), so a reader can slice it directly; when the model
//wrapped its answer in a think block the prefix length is added back rather
//than reported in a second coordinate space.
//`*atOut`/`*startOut`/`*endOut` are npos when the corresponding thing does not
//exist (no such coded line; no line-leading PLAN marker).
static bool codedAnswerLinePlanSpan(const string& reply, int ordinal,
                                    size_t * atOut, size_t * startOut, size_t * endOut)
{
    if (atOut) *atOut = string::npos;
    if (startOut) *startOut = string::npos;
    if (endOut) *endOut = string::npos;
    if (ordinal < 1)
        return false;
    size_t base = 0;
    string text = reply;
    const size_t thinkEnd = text.rfind("</think>");
    if (thinkEnd != string::npos)
    {
        base = thinkEnd + 8;
        text = text.substr(base);
    }
    size_t at = string::npos;
    const string line = codedAnswerLineAt(text, ordinal, (size_t) -1, &at);
    if (line.empty() || at == string::npos)
        return false;
    if (atOut)
        *atOut = base + at;
    const size_t planStart = firstLineLeadingPlanPos(text);
    if (planStart == string::npos)
        return false;
    const size_t planEnd = planBlockEndOffset(text, planStart);
    if (startOut) *startOut = base + planStart;
    if (endOut) *endOut = base + planEnd;
    return at >= planStart && at < planEnd;
}


static bool codedAnswerLineInPlanBlock(const string& reply, int ordinal)
{
    return codedAnswerLinePlanSpan(reply, ordinal, NULL, NULL, NULL);
}


static bool recordLatchedLineSpan(json& rec, const string& reply, int ordinal)
{
    size_t at = string::npos, ps = string::npos, pe = string::npos;
    const bool inPlan = codedAnswerLinePlanSpan(reply, ordinal, &at, &ps, &pe);
    if (at != string::npos)
        rec["latched_line_offset"] = (long) at;
    if (ps != string::npos && pe != string::npos)
    {
        rec["plan_block_start"] = (long) ps;
        rec["plan_block_end"] = (long) pe;
    }
    return inPlan;
}



//#W53-Q (D24). A record with `choice: -1`, `chosen_text` absent and a fallback
//class says the model did not answer - and then says NOTHING about what did.
//146v125 seq 282 (`reply: 'method4 * is_ k ind  *'`, a two-row cast menu at 14
//life) is the shape: the game continued, the heuristic decided, and the corpus
//holds no trace of the decision that was actually taken. The recovery record
//closes that: it names the record it recovers and its class, and carries the
//narration the game produced next - whose HEAD is the heuristic's own action,
//because the narration delta is append-only and this seat had priority. The
//lines are COPIED, never consumed: the following record's `events` field is
//byte-unchanged, so nothing downstream that reads deltas is disturbed.
void AIPlayerGPT::flushRecoveryRecord()
{
    if (mRecoverySeq < 0 || mTransLogPath.empty())
        return;
    int recovers = mRecoverySeq;
    string cls = mRecoveryClass, kind = mRecoveryKind;
    //#W67-AX (I7): the executed answer, captured with the same flush idiom so a
    //stamp can never leak onto a later recovery.
    const string execSeam = mRecoveryExecSeam;
    const string execText = mRecoveryExecText;
    const int execRow = mRecoveryExecRow;
    const string execBy = mRecoveryExecBy; //#W68-BC (MED)
    const bool labelAbsent = mRecoveryLabelAbsent; //#W79-DB (T1)
    mRecoveryLabelAbsent = false;
    mRecoverySeq = -1;              //cleared BEFORE the write, per the flush idiom
    mRecoveryClass.clear();
    mRecoveryKind.clear();
    mRecoveryExecSeam.clear();
    mRecoveryExecText.clear();
    mRecoveryExecRow = -1;
    mRecoveryExecBy.clear(); //#W68-BC (MED)
    json rec = {
        {"seq", mTransSeq++},
        {"kind", "recovery"},
        {"recovers_seq", recovers},
        {"recovers_kind", kind},
        {"recovers_fallback", cls},
        //#W79-DB (T1, MEASURE ONLY): the recovered record's protocol-deviation
        //class, as the one bit a seat needs to join this record to the S1
        //population. ALWAYS present, so `false` is a measured negative and not a
        //missing field. The parser is untouched.
        {"answer_label_absent", labelAbsent},
        {"turn", translogTurn(observer->turn)},
        {"phase", observer->getCurrentGamePhaseName()},
        {"my_life", life},
        {"opp_life", opponent() ? opponent()->life : 0},
    };
    //Absent, not empty, when the heuristic's answer produced no narration at
    //all (a pass, or a choice the log does not voice) - which is itself the
    //finding, and must not be dressed up as an action.
    if (!mNarrationPending.empty())
        rec["recovered_by"] = mNarrationPending;
    //#W67-AX (I7, engine MED-1 - load-bearing). WHAT THE HEURISTIC ACTUALLY DID.
    //`recovered_by` is the narration DELTA, which is silent for exactly the
    //answers a review most needs to see: a pass, a decline, a cast the log
    //voices only as zone events on a later window. 18 of the wave-66 corpus's
    //93 recovery records carried no `recovered_by` at all, and none of the 93
    //named a ROW. These three fields are the executed answer as the seam that
    //ran it saw it - the seam's own name, the 1-based printed row (0 = a pass
    //or a no-cast, which is an answer and is stated as one), and its text.
    //Written only where the seam stamped them, so a seam that does not yet
    //report stays silent rather than claiming an unknown row.
    if (!execSeam.empty())
    {
        rec["executed_seam"] = execSeam;
        rec["executed_choice"] = execRow;
        rec["executed_text"] = execText;
        rec["executed_by"] = execBy.empty() ? string("heuristic") : execBy; //#W68-BC (MED)
    }
    transLogWrite(rec.dump()); //audit-L (L4)
}


//#W67-AX (I7): the seam records what the heuristic answered on its behalf. Called
//AFTER the base class has decided, so it lands on the recovery record that
//already trails this decision's fallback record. A stamp with no pending
//recovery is dropped: only a decision the model failed earns one.
void AIPlayerGPT::noteHeuristicExecuted(const char * seam, int row, const string& text)
{
    if (mRecoverySeq < 0 || !seam || !*seam)
        return;
    mRecoveryExecSeam = seam;
    mRecoveryExecRow = row;
    mRecoveryExecText = text;
    mRecoveryExecBy = "heuristic"; //#W68-BC (MED)
}


//#W68-BC (MED, engine MED-1): the re-ask's own resolution, stamped where the
//second answer becomes final. Same guard as the heuristic stamp - a stamp with
//no pending recovery is dropped.
void AIPlayerGPT::noteReaskExecuted(const char * seam, int row, const string& text)
{
    if (mRecoverySeq < 0 || !seam || !*seam)
        return;
    mRecoveryExecSeam = seam;
    mRecoveryExecRow = row;
    mRecoveryExecText = text;
    mRecoveryExecBy = "reask";
}


//#W67-AZ (R7): the driver's own last-resort pick, recorded. Counted for the
//gameend report and traced, so a corpus can tell a card the seat chose from a
//card the engine pressed for it after every other path declined to answer.
void AIPlayerGPT::noteEngineRevealFloor(const string& card)
{
    mEngineRevealFloorPicks++;
    DebugTrace("AIPlayerGPT[" << deckFileSmall << "]: the reveal driver's floor picked "
               << card << " - no seam had selected a card for a chooser that cannot decline ("
               << mEngineRevealFloorPicks << " this game)");
}


//#W67-AZ (R7, codex review finding 7): the DRIVER's own floor - the last resort
//in MTGRevealingCards for any zero-click mandatory chooser, including seats that
//never asked a model - now says that it fired. It was silent, so a corpus could
//not tell a card the seat CHOSE from a card the engine pressed for it. Called
//from AllAbilities.cpp through this extern (the reveal driver deliberately does
//not include the GPT seat's header); a non-GPT seat records nothing.
void gptNoteEngineRevealFloor(Player * seat, const string& card)
{
    AIPlayerGPT * gpt = dynamic_cast<AIPlayerGPT *>(seat);
    if (!gpt)
        return;
    gpt->noteEngineRevealFloor(card);
}


//#W53-Q (D24): does this record hand its decision to the heuristic? TRUE only
//when nothing from the reply executed (choice < 0) AND a fallback class was
//stamped - so an executed answer, and a note-only record, never latch one.
//Pure, so the gate is pinned in PARSETEST.
//#W54-F (D7b): did the ENGINE answer this decision with no model call at all?
//True only when nothing from a reply executed (choice < 0), no class was already
//stamped, and BOTH the prompt and the reply are empty - the signature of a seam
//that self-declined (a predicate-gated reveal with nothing eligible) rather than
//of a model that failed. A record that carries a prompt was asked; a record that
//carries a reply was answered. Pure, so the gate is pinned in PARSETEST.
bool AIPlayerGPT::engineAnsweredNoModel(int choice, const char * fallback,
                                        bool emptyPrompt, bool emptyReply)
{
    return choice < 0 && (fallback == NULL || *fallback == '\0') && emptyPrompt && emptyReply;
}


bool AIPlayerGPT::handedToHeuristic(int choice, const char * fallback)
{
    return choice < 0 && fallback != NULL && *fallback != '\0';
}


//#W70-BN (F10, Astra review finding 10): DID THIS WINDOW EXECUTE A PLAN STEP?
//The action line IS step one of the plan, so a step is consumed when the action
//is EXECUTED - not when a reply is parsed. Wave 70 advanced the pointer inside
//consumePlan, which meant a re-ask, an invalid echo, an empty reply and even the
//plan-omission branch each walked the sequence forward although nothing had
//happened on the board, while a NEW plan that DID execute its first step
//re-served that step next window ("cast Bear" again after Bear resolved).
//The record writer is the one place every consumed decision passes through and
//it already carries the three facts the question needs: a model round trip
//happened (a non-empty prompt), the engine took a row (choice >= 0), and no
//fallback class intervened (a re-ask, a refusal, a stale drop and an engine
//answer all carry one). Pure, so PARSETEST pins it without a board.
bool AIPlayerGPT::planStepExecuted(bool hasPlan, bool modelCall, int choice,
                                   const char * fallback)
{
    return hasPlan && modelCall && choice >= 0
           && (fallback == NULL || *fallback == '\0');
}


//#W56-A (D4): `chosen_text` is never empty while `choice >= 0`. 9 of the
//wave-55 corpus's 3,171 records carried a live `choice` and no `chosen_text`,
//every one of them written on the RE-ASK path (146v152 s17, 123v152 s56,
//152v123 s77, 123v126 s25/s42, 123v146 s16, 130v123 s109/s122, 162v126 s33):
//the reply named a row, the engine did not execute it, and the field the
//harvest keys on went absent - so every class that passes through a re-ask is
//silently under-counted (this wave's own HOLD-take census had to fall back to
//the rendered row text). Nothing executed there, so the honest value is a
//REASON, not a row: "<refused: plan_choice_conflict>" says the row was named,
//not run, and names the class that stopped it. The row text is used only when
//no class is available, and the last resort still says which row.
static string refusedChosenText(int choice, const char * fallback,
                                const std::vector<string> * optionTexts)
{
    if (fallback && *fallback)
        return string("<refused: ") + fallback + ">";
    if (optionTexts && choice >= 1 && choice <= (int) optionTexts->size())
        return string("<refused: not executed: ") + (*optionTexts)[choice - 1] + ">";
    //#W57-A (D4): a record with no live choice and no class named still says
    //so in the field, rather than leaving it absent.
    if (choice < 0)
        return string("<refused: no answer executed>");
    std::ostringstream o;
    o << "<refused: not executed: row " << choice << ">";
    return o.str();
}


//#W77-CU (F5, Astra HIGH 5): A TIMED-OUT ASKED WINDOW LOST ITS HISTORY COMMIT.
//`mWindowSeq` is the window counter the hold memory stages against: the builder
//stages the menu under the value the counter holds, and the record that the
//model's answer earns consumes that same value, which is how
//`w76HoldWindowAsked` knows the record belongs to the staged window
//(`pendingSeq[s] != windowSeq` rejects anything else). Every writeTransLog call
//used to consume a value - including `wall_miss`, which is written from INSIDE
//the poll, while the very window that staged N is still in flight. The sequence
//the review names: window stages N, the deadline is spent,
//`pollCompletionRetry` calls `flushWallMissRecord("wall_miss_no_retry")` which
//consumes N, `askModel` then writes its own ask record at N+1, the guard
//rejects the commit, and the NEXT window at that seam says "this is the first
//window I have asked you" about a window the model was asked.
//A `wall_miss` is TELEMETRY ABOUT A WINDOW STILL IN FLIGHT, not a window of its
//own: it rides the pending window's number instead of taking one. Every other
//kind is a decision that closed a window and keeps the old behaviour, so a
//window that was built and then answered by the heuristic (`defer`) still
//orphans its stage - it was not asked, and must not become the bracket's
//referent.
static bool w77RecordIsWindowRecord(const char * kind)
{
    return !(kind && strcmp(kind, "wall_miss") == 0);
}


//The counter's consumption rule, as the one function both the live writer and
//the pin call. Returns the sequence number this record carries.
static int w77ConsumeWindowSeq(int& windowSeq, const char * kind)
{
    if (!w77RecordIsWindowRecord(kind))
        return windowSeq; //telemetry rides the in-flight window's own number
    return windowSeq++;
}


//#W82-P11: THE RECORD SHAPE. One record kind per decision the model was asked -
//`window` - with the seam as a FIELD (`seam`: ask, priority, attackers,
//blockers, discard, reveal, bottom, order) and the window's ordinal
//(`window_seq`), so a reader joins side events to it by number instead of by
//guessing from a kind word. Side records exist only for events with no window:
//`hold_event`, `recovery`, `gamestart`/`gameend`/`system`, `defer` (an answer
//the engine could not execute, no prompt), `ask_replay` (the sidecar), and a
//`forced_close` or `wall_miss` whose window never wrote a record. A forced close
//or wall miss whose window DOES write a record rides that record as a field
//(`forced_close`: the events; `wall_miss`: class + latency). Every counter is
//kept - this is a shape change, not a deletion. Pure: the (kind, seam) a
//writer call maps to.
static void translogRecordShape(const char * kind, string& outKind, string& outSeam)
{
    outKind = kind ? kind : "";
    outSeam.clear();
    if (!kind || !*kind)
        return;
    if (strcmp(kind, "defer") == 0 || strcmp(kind, "wall_miss") == 0)
        return; //side kinds: no window of their own
    outSeam = kind;
    outKind = "window";
}


//#W82-P11: the forced-close events that have not yet joined a window record.
//Joined = the same window ordinal as the record being written.
static json forceCloseEventJson(const AIPlayerGPT::ForceCloseEvent& e)
{
    return json{
        {"event", e.event}, {"outcome", e.outcome}, {"arm", e.arm},
        {"window_seq", e.windowSeq}, {"park_armed", e.parkArmed},
        {"defer_ticks", e.deferTicks}, {"unrecorded_so_far", e.unrecordedSoFar},
        {"turn", e.turn}, {"phase", e.phase},
    };
}

std::vector<AIPlayerGPT::ForceCloseEvent> AIPlayerGPT::flushForceCloseFold(int joinWindowSeq)
{
    std::vector<ForceCloseEvent> keep, joined;
    for (size_t i = 0; i < mForceCloseFold.size(); i++)
    {
        const ForceCloseEvent& e = mForceCloseFold[i];
        if (joinWindowSeq >= 0 && e.windowSeq == joinWindowSeq)
            joined.push_back(e);
        else if (joinWindowSeq < 0 || e.windowSeq < joinWindowSeq)
        {
            //its window is gone without a record: a side record, in file order
            json j = forceCloseEventJson(e);
            j["seq"] = mTransSeq++;
            j["kind"] = "forced_close";
            transLogWrite(j.dump());
        }
        else
            keep.push_back(e); //a later window's event: still pending
    }
    mForceCloseFold.swap(keep);
    return joined;
}


void AIPlayerGPT::writeTransLog(const char * kind, const string& userMsg, const string& reply, int choice, int optionCount,
                                const string& chosenText, const char * fallback, const vector<string> * optionTexts,
                                const char * choiceSource)
{
    //#W76-CN (Q1): THIS RECORD IS A WINDOW THE MODEL WAS ASKED. The count and
    //the commit run BEFORE the logging-off early return, because whether the
    //bracket's referent is true cannot depend on whether a translog is
    //configured (the shipped default is off). A window suppressed by the hold
    //latch or answered from a replay cache writes no record and therefore never
    //commits: that is the whole of the HIGH-1 fix.
    const int recordWindowSeq = w77ConsumeWindowSeq(mWindowSeq, kind); //#W77-CU (F5)
    //#W77-CR (R2 a, wave-76 engine-seat HIGH-2): THE LAND DROP IS ITS OWN
    //QUESTION AND NOW ITS OWN SEAM. Every `kind: ask` record mapped to the
    //"cast" seam, and the land-drop menu is a `kind: ask` record - so a land
    //window sat in the casting menu's hold-check history and vice versa. All 22
    //false brackets in the wave-76 corpus are a cast-seam pair with a land drop
    //in it; 0 of the remaining 1,011 pairs is false. The window's own class is
    //already known here (askModel set it before the call), so the commit is
    //routed by the builder that asked, not by the record's kind word.
    //(history: comment-archaeology.md AIPlayerGPTTranslog-L1093-1723)
    //#W77-CR R2 a policy: a window that was not asked leaves no trace) and the next
    //bracket at this seam still names the last window the model actually saw.
    const char * const holdSeamForRecord =
        (kind && strcmp(kind, "priority") == 0) ? "priority"
        : (kind && strcmp(kind, "ask") == 0)
              ? (mLogWindowKind == kAskWindowLandDrop ? "land" : "cast")
        : NULL;
    const bool holdBracketRendered = (userMsg.find("[hold check:") != string::npos);
    if (holdSeamForRecord)
    {
        if (holdBracketRendered || w78StagedNoteIsEmpty(mHoldMemory, holdSeamForRecord))
            w76HoldWindowAsked(mHoldMemory, holdSeamForRecord, recordWindowSeq);
        else
            w76HoldWindowNotAsked(mHoldMemory, holdSeamForRecord);
    }
    //#W77-CR (R2 d): consumed by THIS record whether or not a log is configured,
    //so a window that printed no bracket can never inherit an earlier one's
    //referent (the mLastParseNote discipline, and the same reason #W76-CN moved
    //the commit above this early return).
    //#W78-CV (S7, wave-77 engine-seat M3): THE FIELD'S OWN CONTRACT, ENFORCED
    //RATHER THAN ASSERTED. The documented meaning is "absent = this window printed
    //no bracket", and 18 records violated it (14 discard, 2 ask, 1 attackers, 1
    //blockers) by inheriting a stamp measured for a window that never reached this
    //prompt. The prompt that is about to be logged is the only authority on whether
    //a bracket reached the model, so it is what decides - the same test the commit
    //above is routed by.
    const int holdCheckRefSeq = holdBracketRendered ? mHoldCheckRefSeq : -2;
    const int holdCheckRefWindow = holdBracketRendered ? mHoldCheckRefWindow : -2;
    //#W77-CU (F5): ...and the stamp is consumed by the window's own record, not
    //by the wall-miss telemetry written while that window is still in flight.
    if (w77RecordIsWindowRecord(kind))
    {
        mHoldCheckRefSeq = -2;
        mHoldCheckRefWindow = -2; //#W78-CV (S7)
    }
    if (mTransLogPath.empty())
    {
        mLastParseNote.clear(); //consumed even when logging is off
        //audit-L (A18): the narration delta has no consumer with the log off
        //(the shipped default = the Vita) and grew for the whole game - 29 KB
        //median, 192 KB max per seat-game in the corpus, held on a 40 MB heap
        //for nobody. The feeders no longer write it when the path is empty;
        //this clear catches anything that reached it before the path was known.
        mNarrationPending.clear();
        mLastHttpStatus = 0; //audit-L (A24): consumed even when logging is off
        mLastCurlResult = -1; //#W59-H (K1): consumed with the attempt
        mLastTransportOutcomes.clear(); //#W59-H (K1): never leak to a later record
        mLastRepeatN = 0;    //#W57-A (D4): likewise, so it cannot leak forward
        return;
    }
    ensureGameStartRecord();
    //#W54-F (D7b): ANY engine-answered decision earns a fallback class. The
    //wave-53 corpus's ONE lost game left `152v125` seq 28 - kind reveal,
    //choice -1, prompt 0, reply "", latency_ms -1, and NO fallback field - so
    //lane Q's recovery contract (which latches on choice<0 AND a class) could
    //not see it, and the single record that marked a 13-hour park read exactly
    //like an ordinary cache hit. The shape is structural, not site-specific:
    //no prompt and no reply means no model call was made, and choice<0 means
    //nothing from a reply executed, so the ENGINE answered. Stamping it here
    //rather than at each call site covers every present and future such seam.
    if (engineAnsweredNoModel(choice, fallback, userMsg.empty(), reply.empty()))
        fallback = "engine_answered";
    //#W53-Q (D24): the PREVIOUS handoff's recovery lands before this record, so
    //the file reads unanswered -> recovery -> next decision in order.
    flushRecoveryRecord();
    //#W70-BN (F10): the plan pointer advances HERE, on the executed decision, and
    //nowhere else. Clamped one short of the last step so a fully-walked plan
    //still carries its final step (planStepsAfter's own rule).
    //#W80-DG (U1): the completing menu CONSUMES the cast step the cast row's own
    //record already advanced - it does not consume a second one. Everywhere else
    //this is the wave-70 rule unchanged.
    if (planStepExecuted(!mCurrentPlan.empty(), !userMsg.empty(), choice, fallback))
    {
        //#W80-DH (F5): a completing menu consumes NO step - the cast row's own
        //record already advanced it - and it does not close the cast either: the
        //latch drops back to 1 and waits for the next completing menu or the cast
        //event.
        if (mPlanCastCompletionState == 2)
            mPlanCastCompletionState = 1;
        else if ((size_t) (mPlanStepsDone + 1) < gptcaveat::planStepCount(mCurrentPlan))
            mPlanStepsDone++;
    }
    //#W71-BO (R1): the record keeps the reply verbatim - the trim is deleted.
    bool replyActionFirst = false;
    std::vector<string> replyOffLines; //#W76-CQ (F9)
    const long replyOffProtocol = offProtocolBytes(reply, &replyActionFirst, &replyOffLines);
    //#W67-AX (I5, engine HIGH-4): this record's OWN round trip, snapshotted
    //beside the field that publishes it. `mLastLatencyMs` is consumed (set to
    //-1) further down this same function, and the reveal residual below is
    //read AFTER that consume - so it saw -1 on every reveal that had a round
    //trip and reported the whole wait as unexplained (14 of the wave-66
    //corpus's 16 reveals; 1622 s reported against 1200 s real). Every later
    //reader of the latency takes this snapshot, so the order of the consume
    //cannot change what the record says again.
    const long recordLatencyMs = mLastLatencyMs;
    //#W78-CV (S7): the seq this record carries, handed back to the hold memory so
    //the NEXT bracket at this seam can name a record instead of a window ordinal.
    const int recordSeqNow = mTransSeq++;
    if (holdSeamForRecord)
        w78HoldWindowRecordSeq(mHoldMemory, holdSeamForRecord, recordWindowSeq, recordSeqNow);
    string recKind, recSeam;
    translogRecordShape(kind, recKind, recSeam); //#W82-P11
    json rec = {
        {"seq", recordSeqNow},
        {"kind", recKind},
        {"model", mModel},
        {"prompt", userMsg},
        {"reply", reply},
        {"choice", choice},
        {"options", optionCount},
        {"turn", translogTurn(observer->turn)},
        {"phase", observer->getCurrentGamePhaseName()},
        {"my_life", life},
        {"opp_life", opponent() ? opponent()->life : 0},
        {"latency_ms", mLastLatencyMs},
    };
    if (!recSeam.empty()) //#W82-P11: the seam is a field; the window its ordinal
    {
        rec["seam"] = recSeam;
        rec["window_seq"] = recordWindowSeq;
        const std::vector<ForceCloseEvent> fc = flushForceCloseFold(recordWindowSeq);
        if (!fc.empty())
        {
            json arr = json::array();
            for (size_t i = 0; i < fc.size(); i++)
                arr.push_back(forceCloseEventJson(fc[i]));
            rec["forced_close"] = arr;
        }
    }
    //#W64-AK (R2): the menu-pass floor's firings, on the first record after
    //they happened. Written only when nonzero, and cleared with the record, so
    //a livelock breaker that fires is visible in the corpus rather than silent.
    if (mMenuPassForced > 0)
    {
        rec["menu_pass_forced"] = mMenuPassForced;
        mMenuPassForced = 0;
    }
    //audit-L (A24): the status the transport saw, on the record that consumed
    //it (a 200 says nothing new and is not written; 0 = no status came back).
    if (mLastHttpStatus != 0 && mLastHttpStatus != 200)
        rec["http_status"] = mLastHttpStatus;
    //#W75-CJ (P2a): and what it said. Present only when a non-200 carried a body.
    if (!mLastHttpErrorBody.empty())
    {
        rec["http_error_body"] = mLastHttpErrorBody;
        mLastHttpErrorBody.clear();
    }
    //#W59-H (K1): every non-200/empty attempt, not merely the final attempt.
    //A scalar keeps the schema simple; `;` separates the bounded retry pair.
    //#W69-BI (K7): ...and the ordinary round trip's own stamp, so the field is
    //present on every record that made a call and its absence means exactly one
    //thing (no call was made).
    {
        const string transport = transportRecordField(
            mLastTransportOutcomes,
            transportOutcomeStamp(mLastCurlResult, mLastHttpStatus, false,
                                  gptConnectTimeoutMs(mTimeoutMs), mLastLatencyMs,
                                  mTimeoutMs),
            mLastLatencyMs);
        if (!transport.empty())
            rec["transport"] = transport;
    }
    //Answer-locked decode-garbage retry: mark the record and note the first
    //(garbage) attempt's latency separately; latency_ms above already carries the
    //SUMMED first+retry round trip (set in pollCompletionRetry).
    if (mLastRetry)
    {
        rec["retry"] = 1;
        mLastRetry = false;
    }
    //#W68-BC (J2): the LEGS of a retried decision, so a p90/max consumer can
    //tell one 1233 s round trip from a 900 s miss plus a 333 s success. Two
    //numbers, always in attempt order; absent on a single-attempt record.
    //#W69-BI (K7): one entry on the single-attempt path as well.
    {
        const std::vector<long> attempts = attemptMsField(mLastAttemptFirstMs,
                                                          mLastAttemptSecondMs,
                                                          mLastLatencyMs);
        if (!attempts.empty())
            rec["attempt_ms"] = attempts;
    }
    //#W54-B (D9): a reply that ANSWERED at or past 95% of the configured
    //deadline gets its own stamp and the elapsed fraction, so "the model
    //nearly missed the wall" stops reading as "the model answered". No
    //behaviour rides it and no dial moves - WAGIC_GPT_TIMEOUT is the owner's.
    if (isLongReply(mLastLatencyMs, mTimeoutMs, !reply.empty()))
    {
        rec["long_reply"] = 1;
        appendParseNote(&mLastParseNote, "long_reply");
    }
    //#W61-U (C13): ...and the FRACTION on every record that spent a round trip,
    //not only on the answered ones that crossed 95%. The wave-60 corpus carried
    //`deadline_pct` on ZERO records - no answered reply reached the long-reply
    //mark - so the one instrument that says how much of the deadline a decision
    //cost, and the only field that separates a wall miss from a connect failure
    //by arithmetic, was absent exactly where it was needed. -1 (cache/reuse, or
    //no deadline configured) still writes nothing.
    {
        //#W68-BC (J2): PER ATTEMPT. On a single-attempt record both legs are
        //-1 and this is exactly the shipped figure; on a retried record it is
        //the worst leg instead of the sum-over-one-deadline that published
        //`deadline_pct 137.0` for a decision in which no call ran past its
        //deadline (162v126 s13).
        const long tenths = (mLastAttemptFirstMs >= 0 || mLastAttemptSecondMs >= 0)
            ? deadlineTenthsPctOfAttempts(mLastAttemptFirstMs, mLastAttemptSecondMs, mTimeoutMs)
            : deadlineTenthsPct(mLastLatencyMs, mTimeoutMs);
        if (tenths >= 0)
            rec["deadline_pct"] = tenths / 10.0;
    }
    //#W54-B (D13): the latched coded line's index and its parenthetical BOTH
    //disagree with the row that ran. 2 of 3,253 parentheticals in wave 53,
    //silent both times.
    //#W63-AD (E11): only where `choice` IS a row index (see recordChoiceIsRowIndex).
    if (recordChoiceIsRowIndex(kind) && latchedRowMismatch(reply, choice, optionCount, optionTexts))
        appendParseNote(&mLastParseNote, "latched_row_mismatch");
    if (recordChoiceIsRowIndex(kind) && (!fallback || !*fallback) && choice >= 1 && optionTexts
        && choice <= (int) optionTexts->size())
    {
        const int repeatSeen = repeatAnnotationCount((*optionTexts)[choice - 1]);
        if (repeatSeen >= 0)
        {
            std::ostringstream rs;
            rs << "repeat_annotated_take(n=" << repeatSeen << ")";
            appendParseNote(&mLastParseNote, rs.str().c_str());
            mRepeatAnnotatedTakes++;
        }
    }
    mLastLatencyMs = -1; //consumed: the next record without a round trip is cache/reuse
    mLastAttemptFirstMs = -1; //#W68-BC (J2): consumed with it
    mLastAttemptSecondMs = -1;
    mLastHttpStatus = 0; //audit-L (A24): consumed with it
    mLastCurlResult = -1; //#W59-H (K1): consumed with it
    mLastTransportOutcomes.clear(); //#W59-H (K1): consumed with it
    //NATIVE REASONING (wave-34 #1b(A)): the model's own thinking for THIS
    //decision, from message.reasoning_content or from an inline <think> block -
    //captured on both paths because the server decides which one we get. This
    //is what seat review reads in place of the deleted post-answer scratch
    //text, so an absent field means the model genuinely returned none, not
    //that the client only implemented the other path. `reply` above is the
    //STRIPPED text, so every length/answer meter measures reply tokens.
    //#W70-BK (C3, invariant 000(a)/(f)): THE REGIME, stamped on every record.
    //A corpus used to be unable to say which regime produced it - every corpus
    //from wave 44 to wave 69 ran with reasoning OFF and nothing in the data
    //said so. The harness gate reads this field to prove the binary honoured
    //the regime the launch asked for.
    //#W82-EA (H6): the flag the REQUEST carried, not the seat's regime - the
    //legacy prefill close read `thinking: on` over `max_tokens_reasoning: 0`,
    //a true statement in the wrong scope. A record with no round trip keeps the
    //regime's value (mLastRequestThinking is reset to it where no request is built).
    rec["thinking"] = (userMsg.empty() ? mThinking : mLastRequestThinking) ? "on" : "off";
    //#W70-BK (C2): the LENGTH is written on every record that carried a round
    //trip, present or ZERO, so "the model returned no reasoning" and "the field
    //is not implemented" can never look alike to the harness gate. The text
    //itself stays present-only-when-true (it is large).
    if (!userMsg.empty())
        rec["reasoning_chars"] = (long) mLastReasoning.size();
    if (!mLastReasoning.empty())
    {
        rec["reasoning"] = mLastReasoning;
        //The LENGTH, separately, because the 6000-token budget is a
        //calibration value the owner expects to tune DOWN: the next budget is
        //read off this distribution (p95/p99 by decision kind) against the
        //reasoning_budget_hit rate, and doing that from the text field alone
        //means re-measuring every record. reasoning_tokens is the server's own
        //number when it reports one - the budget is denominated in tokens, so
        //prefer it and fall back to chars.
        mLastReasoning.clear();
    }
    if (mLastReasoningTokens >= 0)
    {
        rec["reasoning_tokens"] = mLastReasoningTokens;
        mLastReasoningTokens = -1;
    }
    //Reasoning requested, answer delivered, trace withheld by the provider.
    //Without this marker the A/B cannot separate "reasoned invisibly" from
    //"did not reason": both write no reasoning field, and only one of them is
    //paying for thinking tokens.
    if (mLastReasoningHidden)
    {
        rec["reasoning_hidden"] = true;
        mLastReasoningHidden = false;
    }
    //The thinking budget bound on this decision and the answer had to be
    //forced out of the model (phase-2 close). Counting these is how the A/B
    //tells "the budget is never reached" from "the budget is shaping every
    //decision".
    if (mLastBudgetHit)
    {
        rec["reasoning_budget_hit"] = true;
        mLastBudgetHit = false;
    }
    //The rescue itself, budget-driven or not. reasoning_forced_close without
    //reasoning_budget_hit is the shape that used to be misreported as a budget
    //hit: the model stopped thinking and never answered, with room to spare.
    if (mLastForcedClose)
    {
        rec["reasoning_forced_close"] = true;
        //#W82-EA (H6): which retry answered, and what phase 1 cost. `retry_thinking`
        //is the flag the retry request carried; `phase1_reasoning_chars` is the
        //trace the thinking-on retry did NOT prefill back (the reasoning field
        //on this record is the retry's own trace).
        rec["retry_thinking"] = mForceCloseIsPrefill ? "off" : "on";
        if (!mForceCloseIsPrefill)
            rec["phase1_reasoning_chars"] = (long) mLastForceClosePrefill.size();
        mLastForcedClose = false;
        mForceCloseArmed = false; //#W75-CJ (P2c): this close reached a record

    }
    //Both markers are PRESENT-ONLY-WHEN-TRUE, like retry/reasoning_hidden:
    //absence means false, not "unimplemented". An analysis that needs a
    //denominator counts records, not fields.
    if (mLastReasoningDegenerate >= 0.0)
    {
        rec["reasoning_degenerate"] = mLastReasoningDegenerate;
        mLastReasoningDegenerate = -1.0;
    }
    //#W75-CJ (P20): the near-duplicate meter, measure only.
    if (mLastReasoningNgramRepeat >= 0.0)
    {
        rec["reasoning_ngram_repeat"] = mLastReasoningNgramRepeat;
        mLastReasoningNgramRepeat = -1.0;
    }
    //Narration delta: the game events that landed since the previous
    //record. A consumed cast's outcome (resolved/countered/died) shows up
    //here on the NEXT record - machine-readable without re-parsing prompts.
    //W42-D8: consumed from the line-aligned accumulator, never sliced out of
    //mNarration by a byte offset the 24k trim invalidates.
    if (!mNarrationPending.empty())
    {
        rec["events"] = mNarrationPending;
        mNarrationPending.clear();
    }
    //#W57-H (D43): which log-window arm produced this prompt, which ask class
    //this window fell in, and how many turns the window elided. On EVERY
    //record (the arm is "full" by default), so the A/B harvest verifies the arm
    //from the data instead of trusting the launch environment - and so a record
    //whose class was never budgeted is distinguishable from one that was.
    rec["log_window"] = logWindowLabel();
    rec["log_window_kind"] = string(askWindowKindName(mLogWindowKind));
    //#W77-CR (R2 d): the hold-check bracket's REFERENT, as an integer on the
    //record that carries the bracket. -1 = "first window asked at this seam";
    //absent = this window printed no bracket. Consumed here so the next record
    //cannot inherit it (the same discipline as mLastParseNote).
    //#W79-CZ (T4): the own-loop verdict FACE this window's prompt printed, stamped
    //at the splice and consumed here, so the two `126` windows wave 78 could not
    //locate are a field on their own record rather than an unjoinable counter.
    //#W81-DK (V9): ...and only on the record that CLOSES the window the face was
    //stamped at. A face whose window never wrote a record (a stale async drop) is
    //dropped here and counted, so `<counter> == records carrying the face +
    //verdict_faces_dropped_unrecorded` holds exactly, both ways.
    //A `wall_miss` is telemetry about a window still IN FLIGHT and rides its number
    //(w77ConsumeWindowSeq) - it is not the record that closes it, so it neither
    //consumes a face nor drops one.
    const bool w81FaceIsThisWindow = w77RecordIsWindowRecord(kind)
                                     && mVerdictFaceWindow == recordWindowSeq;
    if (w77RecordIsWindowRecord(kind) && !w81FaceIsThisWindow
        && (!mOwnLoopVerdictFace.empty() || !mCrackBackVerdictFace.empty()
            || !mStackDeathVerdictFace.empty()))
    {
        mVerdictFacesDroppedUnrecorded++;
        mOwnLoopVerdictFace.clear();
        mCrackBackVerdictFace.clear();
        mStackDeathVerdictFace.clear();
    }
    if (w81FaceIsThisWindow && !mOwnLoopVerdictFace.empty())
    {
        rec["own_loop_verdict"] = mOwnLoopVerdictFace;
        mOwnLoopVerdictFace.clear();
    }
    //#W80-DE (U2, wave-79 engine-seat HIGH-2): the two verdicts the hold latch
    //arbitrates, on the record of the window whose prompt printed them, and the
    //clamp's own two events. Without these four fields `hold_verdict_safer_ignored`
    //59 and `hold_reopened_new_threat` 0 were unsamplable: neither the window, nor
    //the held face, nor the live face was recoverable from any of the 42 seat logs.
    //All four are consumed here so no later record can inherit them.
    if (w81FaceIsThisWindow && !mCrackBackVerdictFace.empty())
    {
        rec["crackback_verdict"] = mCrackBackVerdictFace;
        mCrackBackVerdictFace.clear();
    }
    if (w81FaceIsThisWindow && !mStackDeathVerdictFace.empty())
    {
        rec["stack_death_verdict"] = mStackDeathVerdictFace;
        mStackDeathVerdictFace.clear();
    }
    //#W81-DI: WHY this window was asked rather than served from the cache -
    //`cached_replay_bound` (an activating answer had already stood once for this
    //key and the board moved under it) or `cached_replay_past_stop` (the row it
    //would have taken is already at the model's own stated stop). Stamped at the
    //send and consumed here, so no later record can inherit it; the counter
    //`cached_replay_reasked` is the join.
    if (!mReaskReasonFace.empty())
    {
        rec["reask_reason"] = mReaskReasonFace;
        mReaskReasonFace.clear();
    }
    //#W81-DK (V4): WHY this casting window exists at all after the phase's casting
    //decision was already answered - `new_stack_object`. The counter
    //`cast_decision_reopened_new_stack` is the join.
    if (w81FaceIsThisWindow && !mCastReopenFace.empty())
    {
        rec["cast_reopen_reason"] = mCastReopenFace;
        mCastReopenFace.clear();
    }
    //#W81-DK (V1): ...and WHY this window went to the model rather than to either
    //re-serve cache - `state_based_action`. The counter
    //`sba_windows_cache_bypassed` is the join.
    if (!mCacheBypassFace.empty())
    {
        rec["cache_bypass_reason"] = mCacheBypassFace;
        mCacheBypassFace.clear();
    }
    //#W81-DK (V15): the six skip counters' per-record trace. Only non-zero entries
    //are written, so a record that closed a window with no skips behind it carries
    //no field at all; the sum over every record plus `skips_after_last_record` on
    //the gameend record equals each counter, by construction.
    {
        const std::map<std::string, int> w81Skips = mSkipTrace.drain();
        if (!w81Skips.empty())
        {
            json sk = json::object();
            for (std::map<std::string, int>::const_iterator si = w81Skips.begin();
                 si != w81Skips.end(); ++si)
                sk[si->first] = si->second;
            rec["skips_since_last"] = sk;
        }
    }
    //#W81-DL (V5/V8/V10): WHICH of this lane's render events this window carried,
    //stamped at the send and consumed here. Every counter this lane adds joins to
    //this field by name, so none of them can be adjudicated as UNTESTED.
    if (!mW81EventFace.empty())
    {
        rec["w81_render_events"] = mW81EventFace;
        mW81EventFace.clear();
    }
    if (holdCheckRefSeq > -2)
        rec["hold_check_ref_seq"] = holdCheckRefSeq;       //#W78-CV (S7): the RECORD seq
    if (holdCheckRefWindow > -2)
        rec["hold_check_ref_window"] = holdCheckRefWindow; //#W78-CV (S7): the window ordinal
    if (mLogWindowElided > 0)
        rec["log_window_turns_elided"] = mLogWindowElided;
    if (!chosenText.empty())
    {
        rec["chosen_text"] = chosenText;
        //#W64-AJ (E9 narration half, engine seat MED-2). 9 records carried
        //`{removes ` inside `chosen_text` and the E9 prediction called that a
        //leak. The GAME LOG is clean (0), so nothing the model reads was
        //harmed - and a BLANKET strip of this field is the wrong repair:
        //#W57-A D4 makes `chosen_text` the RENDERED row byte for byte on
        //purpose, and `stripNarrationDecoration` drops `{taking this row ...}`,
        //which is the HOLD row's benefit tail - the exact tail whose absence
        //made a wave-56 exact-match census read 208 HOLD takes where the truth
        //was 320. So the row is kept whole and the STRIPPED identity is written
        //beside it, present only where the two differ. A harvest that wants the
        //decision-time-tag-free name reads this field; nothing is deleted.
        const string core = stripNarrationDecoration(chosenText);
        if (core != chosenText && !core.empty())
            rec["chosen_text_core"] = core;
        //#W74-CE (O25, deck162 HIGH): the taken row's reserve verdict names a
        //card the row strands this turn, and the PLAN this same reply wrote
        //names it anyway. Instrument only - nothing is refused or re-asked.
        {
            const string stranded = reserveStrandedCardName(chosenText);
            if (!stranded.empty() && planNamesStrandedCard(mCurrentPlan, stranded))
            {
                rec["plan_names_stranded_card"] = stranded;
                mPlanNamesStrandedCard++;
            }
            //#W75-CL (P17): the widened population, on its own field so the two
            //are never summed by accident. Measure only.
            //#W76-CN (Q9): ...and INDEPENDENT of the reserve verdict. The two
            //were an if/else, so a window whose taken row DID carry a reserve
            //verdict was never examined for the wider class at all - a
            //structural under-count of the second measure that no corpus could
            //see, because the two fields are read separately by construction.
            {
                //#W77-CR (R9): and a reply that wrote no PLAN line is skipped -
                //mCurrentPlan is then an EARLIER window's sentence and this
                //window said nothing about any card.
                const bool thisReplyStatedAPlan =
                    (firstLineLeadingPlanPos(reply) != string::npos);
                const string dead = thisReplyStatedAPlan
                    ? w75PlanNamesUncastableZoneCard(mCurrentPlan, game) : string();
                if (!dead.empty())
                {
                    rec["plan_names_uncastable_zone_card"] = dead;
                    mPlanNamesUncastableZoneCard++;
                }
            }
        }
    }
    else //#W57-A (D4): never ABSENT either. Wave 56 filled the field only while
         //`choice >= 0`, so all 6 of the wave-56 corpus's `choice: -1` records
         //(3 named_row_reask, 1 stale_echo, 2 wall_miss_unrecorded) carried no
         //`chosen_text` at all and the promised `<refused: ...>` value rendered
         //0 times - a harvest could not tell a refusal from a missing field,
         //which is the same silent instrument D16 fixes one seam over.
        rec["chosen_text"] = refusedChosenText(choice, fallback, optionTexts);
    //#W57-A (D4): a repeat-N take keeps `chosen_text` BYTE-EQUAL to the row and
    //states its count in a field of its own. Wave 56 appended " xN" to the row
    //text, which is exactly the shape that makes an exact-match census fail.
    if (mLastRepeatN >= 2)
    {
        rec["repeat_n"] = mLastRepeatN;
        mLastRepeatN = 0;
    }
    if (fallback)
        rec["fallback"] = fallback;
    //#W57-U: the decision the heuristic answered because the model call was
    //ABANDONED - in flight past its own deadline plus the grace with nothing
    //published. This OVERWRITES the caller's class on purpose: the caller only
    //knows the reply was empty, and empty_reply already means five other
    //things. The elapsed seconds ride with it so a seat review can tell a
    //wedged transport from a slow one without reading stderr.
    //#W82-A (L10): only on the record of the window the abandonment happened in.
    if (mAbandonedInFlightSecs >= 0 && mAbandonedInFlightSeq != mWindowSeq)
    {
        mAbandonedInFlightSecs = -1;
        mAbandonedInFlightSeq = -1;
    }
    if (mAbandonedInFlightSecs >= 0)
    {
        rec["fallback"] = "abandoned_in_flight";
        rec["abandoned_after_s"] = mAbandonedInFlightSecs;
        mAbandonedInFlightSecs = -1;
        mAbandonedInFlightSeq = -1;
    }
    //#W50-Y D10 (iii): how many consecutive replies re-stated the carried plan
    //#W82-A (audit-2026-09): `plan_echo_count` is DELETED - a REPORT field with
    //no engine consumer and no tool reader, reconstructible from the replies the
    //same log already carries.
    //Parse-shape signature (W36 items 1-4): multi-answer overruns, echo/index
    //conflicts and blocker re-ask provenance - divergences between what the
    //reply said and what executed, none of which is a heuristic fallback.
    //Present only when something diverged; consumed here so a note can never
    //leak onto a later record.
    if (!mLastParseNote.empty())
    {
        rec["parse_note"] = mLastParseNote;
        //#W81-DM (V12): the game total for the one note that silently DISCARDS
        //part of the model's answer. Counted here, on the record that carries
        //the note, so `multi_answer_first_taken` in the gameend census and
        //`parse_note` on this seq are the same event seen twice.
        if (mLastParseNote.find("multi_answer_first_taken") != string::npos)
            mMultiAnswerFirstTaken++;
        mLastParseNote.clear();
    }
    //Commit-failure counters (see offProtocolBytes / commitRetracted above).
    //Written on EVERY record, present or zero, so a seat review can divide by
    //the record count without inferring absence.
    //#W70-BM (E2): bytes outside the two lines the protocol permits, on the FULL
    //reply. 0 on a compliant PLAN + action reply, in either line order.
    rec["off_protocol_bytes"] = replyOffProtocol;
    if (replyActionFirst && !userMsg.empty())
    {
        //#W70-BM (E2): parsed, but not the shape asked for - stamped, never
        //silently accepted. The gameend census carries the game's total.
        rec["action_before_plan"] = true;
        mActionBeforePlanReplies++; //#W82-A R1: restored (LEDGER v2)
    }
    //#W80-DH (F2): ...and whether the PARSER acted on it. `action_before_plan` is
    //the SHAPE meter (computed on the raw reply by offProtocolBytes);
    //`action_before_plan_rejected` is the EVENT - the window whose answer was
    //dropped for stating the action above the plan, stamped by consumePlan at the
    //refusal and consumed here so it can never migrate onto a later record. The
    //two counters reconcile with the corpus: `action_before_plan_rejected_replies`
    //is by construction the number of records carrying this field.
    if (mActionBeforePlanRejected)
    {
        rec["action_before_plan_rejected"] = true;
        mActionBeforePlanRejects++; //#W82-A R1: restored (LEDGER v2)
        mActionBeforePlanRejected = false;
    }
    //#W71-BO (L10, wave-70 MED-2). A MISSING PLAN LINE IS NOW VISIBLE. The
    //corpus carried 11 replies with NO `PLAN:` line at all, every one of them
    //reading `off_protocol_bytes: 0` - the meter charges a MISLABELLED plan (92
    //unlabelled + 7 `YOUR PLAN:`) and is blind to an ABSENT one, so "the plan
    //was omitted" and "the reply was perfect" produced identical records. The
    //silent-instrument shape lane BK fixed for `reasoning_chars`, fixed here:
    //written on EVERY record that carried a round trip, present or false.
    if (!userMsg.empty())
    {
        const bool planLineMissing = (firstLineLeadingPlanPos(reply) == string::npos);
        rec["plan_line_missing"] = planLineMissing;
        if (planLineMissing)
            mPlanLineMissing++;
        //#W75-CL (P23 d, deck162 HIGH). `off_protocol_bytes` and
        //`plan_line_missing` were reported as two findings and are ONE EVENT: the
        //reply wrote its plan as an unlabelled sentence, so the label is missing
        //(plan_line_missing) and the sentence is bytes outside the two permitted
        //lines (off_protocol_bytes) - 12 of deck162's 16 records carry both, and a
        //review that counts them separately double-counts one defect and cannot
        //tell it from the two OTHER shapes that live in the same fields (a reply
        //with a labelled plan plus real prose; a reply with an answer and no plan
        //at all, which reads 0 bytes off-protocol). One CLASS field says which of
        //the three this reply was, and one counter says how many replies deviated
        //at all. The two underlying fields are kept: they are what the class is
        //computed from and deleting a measure to add one is how instruments lose
        //their audit trail.
        //#W76-CP (Q15): the forced-close marker was written into `rec` a few
        //dozen lines above (and the latch cleared there), so the record itself
        //is what this reads - the one source that cannot drift from the field
        //the reviewer will filter on.
        //#W76-CQ (F9): the class is decided by a BYTE MATCH against the prefill
        //this rescue actually supplied, not by the fact that a rescue happened.
        const bool w76PrefillEcho = rec.count("reasoning_forced_close") > 0
                                    && w76OffProtocolIsPrefillEcho(replyOffLines,
                                                                   mLastForceClosePrefill);
        //#W78-CX (S1): the shape test needs to know whether the reply carried a
        //coded answer line AT ALL - the same count the record publishes as
        //`coded_answers` a few lines below, read from the same reply.
        const bool w78LabelAbsent = w78AnswerLabelAbsentShape(replyOffLines,
                                                              codedAnswerCount(reply) > 0);
        const char * devClass = w75ProtocolDeviationClass(planLineMissing, (int) replyOffProtocol,
                                                         w76PrefillEcho, w78LabelAbsent);
        mLastForceClosePrefill.clear(); //#W76-CQ (F9): consumed with this record
        rec["protocol_deviation"] = string(devClass);
        if (planLineMissing || replyOffProtocol > 0)
            mProtocolDeviationReplies++;
        //#W78-CX (S1): ...and the MEASURE - of the records that carried an answer
        //line with its label elided, how many the parser READ (#W79-DD: the ruling
        //flipped this meter from the heuristic's side of the split to the
        //parser's; same denominator, same class, opposite arm). Counted on the
        //same predicate the recovery handoff latches on, so the count and the
        //`recovery` records it will be joined against cannot disagree.
        //#W78-CY (F10, Astra MED 10): ...counted on the FINAL class, not on the raw
        //shape flag. A reply that is only `2 (Hold priority)` - no PLAN at all -
        //classifies `unlabelled_plan`, yet `w78LabelAbsent` is true for it, so the
        //counter counted records that carry a different class and could never be
        //reconciled against the `answer_label_absent` population it is named for.
        if (w79LabelAbsentRead(devClass, handedToHeuristic(choice, fallback)))
            mAnswerLabelAbsentRead++;
        //#W79-DB (T1): the SAME final class, latched for the recovery record the
        //handoff below may arm. Measure only.
        mReplyLabelAbsent = w79RecoveryLabelAbsent(devClass);
        mProtocolReplies++; //#W70-BM (E2): the census denominator
        //#W71-BO (L10 sibling, wave-70 P): `plan_steps_done` was absent from every
        //record, so lane BN's F10 step-advance claim was unmeasurable. The pointer
        //into the carried plan, and the plan's own step count, ride every record.
        rec["plan_steps_done"] = mPlanStepsDone;
        rec["plan_step_count"] = (long) gptcaveat::planStepCount(mCurrentPlan);
    }
    if (gLastPutGlossStripped) //#W70-BM (E3): B4.6's census
    {
        mPutGlossStripped++;
        gLastPutGlossStripped = false;
    }
    //#W63-AD (E11): on a subset seam `choice` is the SIZE of the selection, not a
    //row. Said on the record rather than left to be inferred; `chosen_text` above
    //is what names the cards that were actually taken.
    if (!recordChoiceIsRowIndex(kind))
        rec["choice_meaning"] = "selection size";
    //#W63-AD (E6c): which system message this window was answered under.
    if (!mSystemHash.empty() && !userMsg.empty())
        rec["system_hash"] = mSystemHash;
    //WAVE-34 #1b(B), instrument only - no behaviour rides these. They measure
    //the boundary the phenomenon actually has (post-ANSWER, not post-PLAN) and
    //the first-vs-last-label divergence the shipped fields were blind to.
    //Written on EVERY record, present or zero, so a seat review can divide by
    //the record count without inferring absence.
    rec["post_answer_overrun"] = postAnswerOverrun(reply);
    if (mLastRequestMaxTokens > 0 && !userMsg.empty())
    {
        rec["max_tokens"] = mLastRequestMaxTokens;
        //#W70-BK (C4): the two halves of that number. `max_tokens_reasoning` is
        //what the thinking window was allowed and `max_tokens_answer` is what
        //the PLAN + action line was allowed; a corpus proves invariant 000(d)
        //by checking that no record under thinking has a reasoning half of 0,
        //instead of trusting a code reading of the cap path.
        rec["max_tokens_answer"] = mLastRequestAnswerTokens;
        rec["max_tokens_reasoning"] = mLastRequestReasoningTokens;
    }
    if (mLastFinishLength && !userMsg.empty())
        rec["reply_truncated"] = 1;
    //#W49-S (D2): false whenever the EXECUTED answer is the reply's first coded
    //line (the seam that executed says so via mAnswerReplacedFalse); deck146
    //vs126 seq 48 read `answer_replaced: true` on a record whose first line was
    //what ran. Consumed here so the flag can never leak onto a later record.
    rec["answer_replaced"] = mAnswerReplacedFalse ? false : answerReplaced(reply);
    mAnswerReplacedFalse = false;
    rec["coded_answers"] = codedAnswerCount(reply);
    //#W53-N (D12b): on a multi-answer reply, WHICH coded CHOICE line the
    //engine latched. The five wave-52 plan_choice_conflict records held the
    //first answer in `reply` and the latched row in `choice` with nothing
    //tying them together; three of them carried answer_replaced + 2 coded
    //answers, so the field is exactly the missing link.
    if (recordChoiceIsRowIndex(kind) //#W63-AD (E11)
        && choice >= 0 && optionTexts && codedAnswerCount(reply) >= 2)
    {
        int ord = codedChoiceOrdinal(reply, choice, optionCount, optionTexts);
        if (ord >= 1)
        {
            rec["latched_coded_line"] = ord;
            //#W63-AD (E6a): ...and the line itself, from the FULL reply, so the
            //trim can never take the deciding line out of the record.
            const string latched = codedAnswerLineAt(reply, ord);
            if (!latched.empty())
                rec["latched_line"] = latched;
            //#W63-AD (E6b): and which side of the plan bound it fell on.
            //#W64-AJ (F13): with the offsets the verdict is computed from, so a
            //reader can check it against the reply instead of trusting it.
            rec["latched_line_in_plan"] = recordLatchedLineSpan(rec, reply, ord);
        }
    }
    //#W63-AD (E6a): the ordinal is only computable where the engine can map a
    //coded line back to a row (a CHOICE menu with its texts). Where it cannot -
    //a combat seam, a record with no optionTexts - and the reply still ended on
    //a DIFFERENT answer than it began with, the LAST coded line is what the
    //last-wins walk selected, and it is kept for the same reason.
    if (!rec.count("latched_line") && rec["answer_replaced"] == true)
    {
        const int n = codedAnswerCount(reply);
        const string last = codedAnswerLineAt(reply, n);
        if (!last.empty())
        {
            rec["latched_line"] = last;
            rec["latched_line_in_plan"] = recordLatchedLineSpan(rec, reply, n); //#W64-AJ (F13)
        }
    }
    //#W66-AR (H2c, engine HIGH-2 third half). THE LINE THE STAMP IS ABOUT.
    //`later_answer_ignored` says a later coded answer line was refused - and on
    //21 of the 33 wave-65 records carrying it the `reply` field had already been
    //trimmed past the PLAN (`[+N bytes ... trimmed from this record]`), so the
    //refused line was exactly the byte range the record did not keep, while
    //`latched_line` holds the line that RAN. Recorded verbatim from the FULL
    //reply, by the selector that refused it, so a seat review can read what the
    //rule cost without re-deriving it from a truncated string.
    if (!mLastIgnoredAnswerLine.empty())
        rec["ignored_line"] = mLastIgnoredAnswerLine;
    mLastIgnoredAnswerLine.clear(); //consumed: never leaks onto a later record
    //#W55-E (D5a): how long the reveal DRIVER had been parked with no structural
    //progress when this record was written. The wave-54 livelock (146v123 s15)
    //wrote a `reveal` record classed stale_livelock and nothing anywhere said the
    //driver had been sitting in phase 0 re-asking the whole time; this is that
    //missing half, and it makes a poll-churn park a NUMBER rather than an
    //archaeology pass over the stderr. Present only when the driver was parked.
    //(history: comment-archaeology.md AIPlayerGPTTranslog-if-1737)
    if (strcmp(kind, "reveal") == 0 && mRevealStallTicks > 0)
    {
        rec["reveal_wait_ticks"] = mRevealStallTicks;
        rec["reveal_wait_secs"] = mRevealStallSecs;
        //#W64-AI (F14, engine HIGH-3): the SAME wait, measured by the driver's
        //full progress signature, in which a model poll counts as progress. The
        //wave-63 finding read 152v146@1788653548 seq31's `reveal_wait_secs: 561`
        //(62% of all reveal wait time in 21 games) as a floor that would not
        //release; the record's own `latency_ms` on that line is 560586, so the
        //561 s WAS the round trip and the guard was correctly not firing. The
        //structural figure cannot say that on its own - it is built to treat a
        //call in flight as no progress - so a reviewer had no way to separate
        //"the engine held this reveal" from "the seat was waiting for its own
        //answer". These two fields are that separation: a wait whose driver half
        //is ~0 is inference time, and only a driver half that grows is a stall.
        rec["reveal_wait_driver_ticks"] = mRevealStallDriverTicks;
        rec["reveal_wait_driver_secs"] = mRevealStallDriverSecs;
        //#W66-AT (engine MED-3): and the subtraction a reviewer has to do by
        //hand to read the pair above, done here once. The wave-65 seat read
        //`reveal_wait_driver_secs: 0` on all 16 corpus reveals as "the wait is
        //engine-side, not model-side" and docketed 835 s of engine stall; the
        //same 16 records carry `reveal_wait_secs` equal to `latency_ms / 1000`
        //on every one of them (107/106146, 139/139015, 68/68246 ...), so the
        //whole 835 s was the seat's own round trips and there was no engine
        //wait to skip. The driver-half figure CANNOT say that on its own, by
        //construction: its progress signature treats a call in flight as
        //progress, so it resets on every poll and can never accumulate while
        //the model is answering. `reveal_wait_unexplained_secs` is the residual
        //the round trip does not account for - the only figure on this record
        //that a future stall would move, and 0 means there is nothing here to
        //fix. A cache hit (`latency_ms` -1) explains nothing, so the whole wait
        //is the residual.
        //#W67-AX (I5): the SNAPSHOT, not the consumed member (see recordLatencyMs).
        rec["reveal_wait_unexplained_secs"] =
            (int) revealWaitUnexplainedSecs(mRevealStallSecs, recordLatencyMs);
        if (mRevealStallParked)
        {
            rec["reveal_stall"] = true;
            rec["reveal_stall_phase"] = mRevealStallPhase;
        }
    }
    if (strcmp(kind, "reveal") == 0)
    {
        mRevealStallTicks = 0; //consumed: never leaks onto a later reveal
        mRevealStallSecs = 0;
        mRevealStallPhase = -1;
        mRevealStallParked = false;
        mRevealStallDriverTicks = 0; //#W64-AI (F14)
        mRevealStallDriverSecs = 0;
    }
    //#W58-C (D4): the stale drops this seat took since its last record. One
    //token per drop, "<arm>/<slot-key half that moved>/<outcome>", and the
    //list is CONSUMED here so a drop is stamped exactly once. Unconditional -
    //this is the record, not a diagnostic, and it is the only evidence a
    //release build (or a console) leaves of an answer bought twice.
    //#W82-A (L10): a drop stamp rides its OWN window's record or none.
    if (!mAsyncDropStamps.empty() && mAsyncDropStampsSeq != mWindowSeq)
    {
        mAsyncDropStamps.clear();
        mAsyncDropStampsSeq = -1;
    }
    if (!mAsyncDropStamps.empty())
    {
        rec["async_drops"] = (int) mAsyncDropStamps.size();
        rec["async_drop_events"] = mAsyncDropStamps;
        mAsyncDropStamps.clear();
        mAsyncDropStampsSeq = -1;
    }
    //#W55-E (D23): this record answers a prompt that had already missed the wall.
    //#W82-A (L7): same slot-keying as the forced-close leg above.
    if (mWallMissPending && !userMsg.empty() && asyncSlotKey(userMsg) == mWallMissBase)
    {
        //#W82-P11: the miss rides the window record it joins, with its facts
        string wmPhase;
        for (size_t i = 0; i < mLastTransportOutcomes.size() && wmPhase.empty(); i++)
            wmPhase = transportStampPhase(mLastTransportOutcomes[i]);
        rec["wall_miss"] = json{{"class", wallMissClassFor(wmPhase)},
                                {"latency_ms", mWallMissLatencyMs}};
        mWallMissPending = false;
        mWallMissBase.clear();
        mWallMissLatencyMs = -1; //#W61-U (C13): this record consumed it
    }
    //Assignments the combat validator pruned as illegal on this record.
    if (!mLastPrunedPairs.empty())
    {
        rec["pruned_pairs"] = mLastPrunedPairs;
        mLastPrunedPairs.clear();
    }
    //dropped_assignments (blocker records only): every B:A pair the reply
    //asked for that did NOT reach the battlefield - dropped by the PARSER as
    //out-of-range or as a second attacker for a blocker already assigned, or
    //pruned by the apply site as illegal. pruned_pairs saw only the second
    //half, which is why it is written on ~one record in a corpus; the silent
    //half is the one that costs games (deck139 vs105 s24: four "Illuna blocks
    //X" pairs, three dropped by first-wins, 7 poison connected, game over).
    //A record with 0 here is a reply whose whole intent was executed.
    if (strcmp(kind, "blockers") == 0 && mLastDroppedAssignments >= 0)
        rec["dropped_assignments"] = mLastDroppedAssignments;
    if (strcmp(kind, "blockers") == 0)
        mLastDroppedAssignments = -1;
    //Provenance for an answer recovered by prose-intent salvage (no coded
    //line existed) - so corpus review can audit every prose salvage.
    if (choiceSource)
        rec["choice_source"] = choiceSource;
    //the exact option strings, when the seam has them as a list (ask /
    //priority) - offered-vs-taken tallies without re-parsing the prompt
    if (optionTexts)
        rec["options_text"] = *optionTexts;
    transLogWrite(rec.dump()); //audit-L (L4)
    //#W81-DK (V15): the RECORD seq of the last window record, so a `hold_event`
    //written between two windows names the window it refers to on the numbering a
    //reviewer joins on (wave 80 gave it `window_seq`, a different numbering - the
    //wave-77 lesson in its own shape: hold at record 89 -> `window_seq 42`).
    if (w77RecordIsWindowRecord(kind) && rec.count("seq"))
        mLastWindowRecordSeq = rec["seq"].get<int>();
    //#W53-Q (D24): this decision was handed off - the next record (or the
    //game-end record) will say what answered.
    if (handedToHeuristic(choice, fallback))
    {
        mRecoverySeq = rec["seq"].get<int>();
        mRecoveryClass = fallback;
        mRecoveryKind = kind;
        mRecoveryLabelAbsent = mReplyLabelAbsent; //#W79-DB (T1)
    }
    mReplyLabelAbsent = false; //#W79-DB (T1): consumed with this record, always
}


//#W54-F (D7a): the engine force-closed a decision this seat was parked on.
//It gets a full record - kind, what the engine did instead, and its own class -
//so the recovery contract names it and a corpus can COUNT force-closes instead
//of inferring them from a game that simply stopped.
//#W55-E (D5a): the reveal driver parked on this seat, and says for how long.
//Report only - nothing in the engine reads these, and the driver's own force-close
//budget is the only thing that acts on a stall. Last writer wins: the driver
//calls this every tick it is parked, so the figures a record carries are the
//figures at the tick that wrote it.
void AIPlayerGPT::noteRevealStall(int ticks, long secs, int driverPhase, bool parked,
                                  int driverTicks, long driverSecs)
{
    mRevealStallTicks = ticks;
    mRevealStallSecs = secs;
    mRevealStallPhase = driverPhase;
    mRevealStallParked = parked;
    mRevealStallDriverTicks = driverTicks; //#W64-AI (F14)
    mRevealStallDriverSecs = driverSecs;
}


void AIPlayerGPT::logEngineResolution(const char * kind, const string& what,
                                      int optionCount, const char * fallbackClass)
{
    writeTransLog(kind, "", "", -1, optionCount, what,
                  (fallbackClass && *fallbackClass) ? fallbackClass : "engine_answered", NULL);
    narrateDecision(what);
}


void AIPlayerGPT::gameEnded()
{
    logGameEnd();
}


void AIPlayerGPT::logGameEnd()
{
    //Idempotent: the game-over transition frame can repeat before the state
    //machine moves on, and only ONE gameend record may close the file.
    if (mGameEndLogged || mTransLogPath.empty())
        return;
    //#W43-11: the last events of the game may still be held in the run buffer;
    //the closing record's delta is the only place they can still land.
    flushEventRun();
    ensureGameStartRecord();
    //#W53-Q (D24): a handoff on the LAST decision of the game still gets its
    //recovery record - that is exactly the decision a lost game is read back
    //from.
    //#W55-E (D23): a wall miss still open when the game ends is an abandoned ask
    //too - the last decision of a game is exactly the one a lost game is read
    //back from. Written BEFORE the recovery flush so its own handoff earns the
    //recovery record that names what answered instead.
    flushWallMissRecord();
    flushRecoveryRecord();
    flushMainPhaseSkip(); //#W73-CB (F6): a phase still pending at game end is
                          //still a phase that ended without a casting window
    mGameEndLogged = true;
    bool iWon = observer->didWin(this);
    bool oppWon = opponent() ? observer->didWin(opponent()) : false;
    flushForceCloseFold(-1); //#W82-P11: nothing left pending at the end
    json rec = {
        {"seq", mTransSeq++},
        {"kind", "gameend"},
        {"model", mModel},
        //#W70-BK (C3): the regime the whole game was played under. Verified
        //absent before this wave - a harvested corpus could not say whether it
        //had reasoning at all without opening every decision record.
        {"thinking", mThinking ? "on" : "off"},
        {"won", iWon},
        {"draw", !iWon && !oppWon},
        {"turn", translogTurn(observer->turn)},
        {"my_life", life},
        {"opp_life", opponent() ? opponent()->life : 0},
        //#W46-7: priority windows this seat auto-passed as mana-only (no
        //record of their own). Without it a corpus reading the translog cannot
        //tell a window that was never offered from one the gate skipped.
        {"mana_only_windows_skipped", mManaOnlyWindowsSkipped},
        //#W53-N (D2): windows the model's own HOLD row closed. Not a window
        //removed - a window the model answered once and did not want re-put.
        {"hold_windows_skipped", mHoldWindowsSkipped},
        //#W72-BT (M10 / M14): the two window classes that never reached the model.
        {"stop_reached_windows_skipped", mStopReachedWindowsSkipped},
        {"stop_reached_reputs_collapsed", mStopReachedRePutsCollapsed}, //#W78-CV (S3)
        //#W79-CZ (T3): re-opens the escalation test kept from happening.
        {"hold_verdict_safer_ignored", mHoldVerdictSaferIgnored},
        {"stack_drain_windows_asked", mStackDrainWindowsAsked},         //#W78-CV (S4)
        {"own_turn_windows_skipped", mOwnTurnWindowsSkipped},
        //#W69-BI (K7, engine MED-5): the same total by suppression class.
        //(history: comment-archaeology.md AIPlayerGPTTranslog-L2049-1752)
        {"ask_replays_reserved", mAskReplaysReserved},
        {"ask_replays_refused", mAskReplaysRefused},
        {"menu_pass_no_progress", mMenuPassNoProgress},
        {"hold_windows_skipped_priority", mHoldWindowsSkippedPriority},
        {"hold_windows_skipped_cast", mHoldWindowsSkippedCast},
        //#W72-BU (M4): of those, the ones closed because the OTHER seam of the
        //same window had already been held. This is the double-ask the wave-71
        //corpus measured (44 same-board cast/priority pairs, 19 hold-opened);
        //a rise here is model calls saved, not windows removed.
        {"sibling_window_asks_skipped", mSiblingWindowAsksSkipped},
        {"hold_released_turn", mHoldReleasedTurn},         //#W73-BY (N2 a)
        {"chain_windows_collapsed", mChainWindowsCollapsed}, //#W73-BY (N6)
        {"chain_windows_only_selfharm", mChainWindowsOnlySelfharm}, //#W74-CD (O13)
        {"chain_selfharm_rows", mChainSelfharmRows}, //#W74-CD (O13)
        {"chain_acting_rows", mChainActingRows}, //#W74-CD (O13)
        {"chain_windows_only_selfharm_cast", mChainWindowsOnlySelfharmCast}, //#W75-CI (P12)
        {"chain_selfharm_rows_cast", mChainSelfharmRowsCast}, //#W75-CI (P12)
        {"chain_acting_rows_cast", mChainActingRowsCast}, //#W75-CI (P12)
        {"main_phase_windows_skipped", mMainPhaseWindowsSkipped}, //#W73-BY (N16)
        //#W81-DM (V12, wave-80 engine-seat MED-3): records whose answer named
        //more rows than the window takes. The parser keeps row 1 and the model
        //believes it chose all of them; wave 80 had 2 (`152` seqs 217/237, both
        //Emrakul's Annihilator sacrifice-6) and no counter reported them.
        //Per-record trace: `parse_note` on the same seq.
        {"multi_answer_first_taken", mMultiAnswerFirstTaken},
        //#W81-DM (V13, wave-80 deck123 HIGH-3): the counterspell intel line -
        //the EVENTS it reports, and the windows it actually reached. The second
        //is incremented AT the render (wave-79 LESSON 1); a `counter_intel_
        //rendered` of 0 on a game whose opponent countered anything is a FAIL.
        {"opp_countered_spells", mOppCounteredSpells},
        {"counter_intel_rendered", mCounterIntelRendered},
        //#W69-BI (K7, engine MED-2): the game's stale-drop total. The
        //per-decision `async_drops` field is consumed with its record, so this
        //is the only place a reader can take the game's number from.
        {"async_drops", mAsyncDropsGame},
        //#W69-BI (K7, deck130 LOW): takes of a row carrying the
        //`[repeat: activated this turn N times already]` annotation.
        {"repeat_annotated_takes", mRepeatAnnotatedTakes},
        //#W69-BI (K7, deck146 MED): the attacker-line blocker forecast census -
        //rows rendered, rows with two or more candidates, rows that carried a
        //GANG BLOCK verdict, rows printed in the collapsed form.
        {"blocker_forecast_rows", mBlockerForecastRows},
        {"blocker_forecast_multi", mBlockerForecastMulti},
        {"blocker_forecast_gang", mBlockerForecastGang},
        {"blocker_forecast_collapsed", mBlockerForecastCollapsed},
        //#W67-AX (I7): casting windows held by the model's own reservation
        //decline (see reserveDeclineHonoured). A window the model answered once
        //and whose arithmetic nothing in the step can move - not a window removed.
        {"reserve_decline_windows_skipped", mReserveDeclineWindows},
        {"reserve_decline_windows_noted", mReserveDeclineNoted}, //#W72-BV (M9)
        //#W74-CE (O25): replies whose PLAN names a card the row they took says
        //it strands this turn. 0 = the reserve verdict is holding.
        {"plan_names_stranded_card", mPlanNamesStrandedCard},
        //#W67-AZ (R7): reveal picks made by the DRIVER's last-resort floor, after
        //every seam declined to select for a chooser that cannot be declined.
        //Zero on a healthy game; nonzero says the engine answered, not the seat.
        {"engine_reveal_floor_picks", mEngineRevealFloorPicks},
        //#W54-D (D8b): asks whose whole option list rendered as one
        //interchangeable row and were resolved without a model call.
        //#W59-J (K10): asks answered from the seat's own last answer to the
        //byte-identical row list, in the same turn, phase and decision, with
        //the plan unchanged. A window the model answered once and was then
        //asked again unchanged - not a window removed.
        {"identical_ask_answers_reserved", mRepeatAskAnswersReserved},
        //#W55-E (D23): deadline misses that spent this seat's one retry, and how
        //many of them were abandoned before any decision record could consume
        //them. Written always, present or zero, so a seat review divides rather
        //than infers - the wave-54 answer to "2 events, 1 record" was silence.
        //#W55-E (D23): deadline misses that spent this seat's one retry, and how
        //many of them were abandoned before any decision record could consume
        //them. Written always, present or zero, so a seat review divides rather
        //than infers - the wave-54 answer to "2 events, 1 record" was silence.
        //#W82-A R1 (LEDGER v2): restored - see flushWallMissRecord.
        {"wall_miss_events", mWallMissEvents},
        {"wall_miss_unrecorded", mWallMissUnrecorded},
        {"wall_miss_no_retry", mWallMissNoRetry}, //#W68-BC (J2)
        //#W70-BM (E2/E3): the reply-SHAPE census and the wave-70 VERIFY counters.
        //`protocol_replies` is the denominator every one of these divides by; a
        //zero beside a zero denominator is silence, not evidence, and that
        //distinction is exactly what the wave-70 audit could not make from code.
        {"protocol_replies", mProtocolReplies},
        //#W82-A R1 (LEDGER v2): restored. Astra: "these record the binding order
        //rule; zero violations do not make the counters unconsumed or the path
        //unreachable."
        {"action_before_plan_replies", mActionBeforePlanReplies},
        //#W80-DH (F2): of those, the ones whose answer was refused and re-asked.
        //Equal, by construction, to the number of records carrying
        //`action_before_plan_rejected`.
        {"action_before_plan_rejected_replies", mActionBeforePlanRejects},
        //#W80-DH (F2): of those, the ones whose answer was refused and re-asked.
        //Equal, by construction, to the number of records carrying
        //`action_before_plan_rejected`.
        {"phase2_answer_recovered", mPhase2AnswerRecovered},
        {"phase2_answer_missing", mPhase2AnswerMissing},
        //#W75-CJ (P2c): closes that armed and never reached any record.
        {"forced_close_unrecorded", mForceCloseUnrecorded
             + gptForceCloseOutstanding(mForceCloseArmed, mRetryPark.forceCloseArmed)}, //#W75-CM (F5)
        {"force_close_arms_refused", mForceCloseArmsRefused}, //#W76-CN (Q8)
        //#W82-A (audit-2026-09): the `force_close_arms_deferred` row is DELETED -
        //it serialised the SAME member as `force_close_arms_refused` above it, a
        //copy-paste artifact, so the two numbers could never differ.
        {"force_close_defer_bound_hits", mForceCloseDeferBoundHits}, //#W76-CQ (F2)
        //#W77-CR (R11 a): deferrals caused by THIS arm's own outstanding close.
        //Its own meter - the park's bound never fired in wave 76 and this one is
        //the shape that did.
        {"force_close_same_arm_deferred", mForceCloseSameArmDeferred},
        {"forced_close_events", mForceCloseEvents}, //#W78-CV (S11)
        //#W80-DF (U13): joins one-for-one with the `menu_single_outcome` records
        //in this same file - the census reconciles or the counter is wrong.
        {"crossphase_identical_reputs", mCrossPhaseRePuts},   //#W76-CN (Q13)
        //#W77-CR (R8): of those, the ones whose ask-cache board key MATCHED -
        //i.e. the ones that printed `nothing on the board has changed`. Never
        //summed with the line above; the difference is the moved-board class.
        {"crossphase_board_unchanged", mCrossPhaseBoardUnchanged},
        //#W77-CR (R1): windows put to the model while the SEAT'S OWN proven life
        //loop was mid-resolution - the 32-window population of engine-seat HIGH-1.
        {"own_loop_windows_asked", mOwnLoopWindowsAsked},
        //#W79-CZ (T4): the same population, counted where the line is spliced.
        {"own_loop_verdict_lines_rendered", mOwnLoopVerdictLinesRendered},
        //#W79-DC (F1): the two figures the continuation digest is accountable for.
        //#W79-DC (F2): holds re-opened by a NEW threat at the same danger rank.
        //#W80-DH (F3): ...and by a NEW LETHAL threat - a top-rank crack-back or
        //stack-death face over an object set the hold was not taken over. Astra's
        //second-Bolt board. Per-record trace: `kind: hold_event`, what =
        //`reopen_new_lethal`.
        //#W80-DH (F11): every clamp and every re-open, as its own record. This
        //equals the number of `kind: hold_event` records in the seat log, and the
        //three class counters above plus the rows-moved re-opens sum to it.
        //#W79-DC (F2): holds re-opened by a NEW threat at the same danger rank.
        //(history: comment-archaeology.md AIPlayerGPTTranslog-L2181-1763)
        {"hold_reopened_new_threat", mHoldReopenedNewThreat},
        //#W80-DH (F3): ...and by a NEW LETHAL threat - a top-rank crack-back or
        //stack-death face over an object set the hold was not taken over. Astra's
        //second-Bolt board. Per-record trace: `kind: hold_event`, what =
        //`reopen_new_lethal`.
        {"hold_reopened_new_lethal", mHoldReopenedNewLethal},
        //#W80-DH (F11): every clamp and every re-open, as its own record. This
        //equals the number of `kind: hold_event` records in the seat log, and the
        //three class counters above plus the rows-moved re-opens sum to it.
        {"hold_events", mHoldEvents},
        //#W80-DH (F5): cast plan steps that closed on the CAST EVENT (or the turn
        //belt) instead of on the first completing menu. Per-record trace: the
        //stderr close line names which of the two closed it.
        {"plan_cast_steps_closed", mPlanCastStepsClosed},
        //#W80-DE (U2): the two verdicts, now RENDERED - 0 in 2,203 wave-79 prompts.
        {"crackback_verdict_lines_rendered", mCrackBackVerdictLinesRendered},
        //#W80-DH (F4): of those, the ones a legal block saves - the false death
        //claims wave 80 would have printed. Per-record trace: `crackback_verdict`
        //= `[crack-back verdict: LETHAL unblocked, blocked away]`.
        {"crackback_lethal_blocked_away", mCrackBackLethalBlockedAway},
        {"stack_death_verdict_lines_rendered", mStackDeathVerdictLinesRendered},
        //#W80-DE (U8): declined lists re-served across a phase on an unchanged board.
        //#W80-DE (U9): the replay population SPLIT BY PATH, so the census sums and
        //is comparable across waves. `ask_replays_reserved` stays the total.
        {"ask_replays_cache", mAskReplaysCache},
        {"ask_replays_repeat_latch", mAskReplaysRepeatLatch},
        {"menu_pass_no_progress_suppressed", mMenuPassNoProgressSuppressed},
        //#W81-DI: windows the priority cache would have replayed an ACTIVATING or
        //CASTING answer into a second time and that went back to the model instead.
        //Per-record trace: `reask_reason`. > 0 whenever a loop engine is live.
        //#W81-DK (V1): SBA windows (the legend rule) that went to the model because
        //neither re-serve cache may answer one. Per-record trace:
        //`cache_bypass_reason: state_based_action`.
        {"sba_windows_cache_bypassed", mSbaWindowsCacheBypassed},
        //#W81-DK (V3): hold re-opens that INVALIDATED the retained answer for the
        //window's own ask key, so the re-opened window went to the model rather than
        //being re-served the hold row the re-open had just retired. Per-event trace:
        //the `hold_event` record's `answer_invalidated`.
        {"hold_reopen_answer_invalidated", mHoldReopenAnswerInvalidated},
        //#W81-DK (V4): casting windows re-opened inside an already-answered phase
        //because the STACK moved. Per-record trace: `cast_reopen_reason`.
        {"cast_decision_reopened_new_stack", mCastDecisionReopenedNewStack},
        //#W81-DK (V9): verdict faces stamped at a real send whose window never wrote
        //a record. `<lines_rendered> == records carrying the face + this`.
        {"verdict_faces_dropped_unrecorded", mVerdictFacesDroppedUnrecorded},
        //#W81-DL (V5): crack-back verdicts whose total was folded with the ADD
        //THOSE UP addenda and/or the compulsory draw the same screen publishes -
        //the windows where the verdict and its header used to name two figures.
        //Per-record trace: `w81_render_events` = `crackback_one_total_folded`.
        {"crackback_verdict_folded_total", mW81FoldedCrackBackTotals},
        //#W81-DL (V8): cast rows whose `[<- ...]` X marker carried the X-menu's
        //REFUSAL wording with no `best X for this cast:` recommendation prefix.
        //Trace: `w81_render_events` = `x_cast_row_refusal_marker`.
        {"x_cast_row_refusal_markers", mW81XCastRefusalMarkers},
        //#W81-DL (V8): board-sweep markers on the X-VARIABLE path that named the
        //seat's own losses instead of pricing them as a bare integer.
        //Trace: `w81_render_events` = `x_sweep_roster_marker`.
        {"x_sweep_roster_markers", mW81XSweepRosterMarkers},
        //#W81-DL (V10): attackers windows that printed a crack-back header AND the
        //cover paragraph beside it. Trace: `attackers_crackback_cover_clause`.
        {"attackers_crackback_cover_clauses", mW81AttackCoverClauses},
        //#W81-DL (V13): cleanup windows where a land row's `spare` VERDICT was
        //withheld because discarding it would leave a reachable card in hand short
        //of a colour. Trace: `w81_render_events` = `spare_colour_withheld`.
        {"spare_colour_verdict_withheld", mW81SpareColourWithheld},
        {"put_gloss_stripped", mPutGlossStripped},
        //#W71-BO (L10): replies that wrote no PLAN line at all, over the same
        //denominator - the class `off_protocol_bytes` cannot see.
        {"plan_line_missing", mPlanLineMissing},
        {"protocol_deviation_replies", mProtocolDeviationReplies}, //#W75-CL (P23 d)
        //#W78-CX (S1) / #W79-DD: the `answer_label_absent` class's READ count -
        //replies that gave a legible answer without its label and whose answer the
        //parser took. The class's population minus this is what the heuristic
        //still played.
        {"answer_label_absent_read", mAnswerLabelAbsentRead},
        {"plan_names_uncastable_zone_card", mPlanNamesUncastableZoneCard}, //#W75-CL (P17)
    };
    //#W81-DK (V15): the TAIL of the skip trace - the skips that fall after the last
    //prompt-bearing record of the game. With the per-record `skips_since_last`
    //deltas this closes the identity
    //   sum(record deltas) + skips_after_last_record == the counter above
    //for each of the six, so every skip is locatable to a window pair.
    {
        const std::map<std::string, int> w81Tail = mSkipTrace.drain();
        if (!w81Tail.empty())
        {
            json sk = json::object();
            for (std::map<std::string, int>::const_iterator si = w81Tail.begin();
                 si != w81Tail.end(); ++si)
                sk[si->first] = si->second;
            rec["skips_after_last_record"] = sk;
        }
    }
    transLogWrite(rec.dump()); //audit-L (L4)
    //#W57-A (D31): the closing totals on STDERR, per seat. A reviewer
    //cross-tabbing holds against savings reads the stderr; before this the
    //window-skip totals existed only inside the gameend JSON, so the two
    //surfaces could not be reconciled without opening both. Report only -
    //nothing reads it, nothing is removed, and the figures are the same
    //members the record above serialises.
    DebugTrace("AIPlayerGPT[" << deckFileSmall << "]: game end (turn "
               << observer->turn << ", " << (iWon ? "won" : (oppWon ? "lost" : "draw"))
               << ") - windows held by the model's own hold row: " << mHoldWindowsSkipped
               << "; mana-only windows auto-passed: " << mManaOnlyWindowsSkipped
               << "; deadline misses: " << mWallMissEvents //#W82-A R1: restored
               << "; repeated identical asks re-served from the seat's own answer: "
               << mRepeatAskAnswersReserved
               //#W79-CZ (T14, wave-78 known bugs T14): SAY WHOSE. This figure is the
               //DEADLINE-MISS population, and it printed `0 unrecorded` beside a wave
               //whose `forced_close_unrecorded` was 16 - two different "unrecorded"s,
               //one line, and the wave-78 seat had to open both surfaces to tell them
               //apart. Each is now named by its own mechanism, and the force-close
               //identity (armed = the stderr `unclosed <think>` lines; unrecorded =
               //`dropped_decision_moved`, the decision moving out from under a pending
               //retry) is printed here so it reconciles without a record join.
               << " (" << mWallMissUnrecorded << " wall-miss unrecorded, "
               << mWallMissNoRetry << " no-retry)" //#W68-BC (J2), #W82-A R1
               << "; forced closes: " << mForceCloseEvents << " events, "
               << mForceCloseUnrecorded << " superseded (dropped_decision_moved)"
               << "; phase-2 answer recovery: " << mPhase2AnswerRecovered << " recovered, "
               << mPhase2AnswerMissing << " missing");
    if (mTransLog.is_open())
        mTransLog.close(); //the game's last record
}

#if defined(WAGIC_GPT_PARSETEST_BUILD)
bool AIPlayerGPTSelfTestAccess::answerReplaced(const string& reply) { return ::answerReplaced(reply); }
int AIPlayerGPTSelfTestAccess::codedAnswerCount(const string& reply) { return ::codedAnswerCount(reply); }
string AIPlayerGPTSelfTestAccess::codedAnswerLineAt(const string& reply, int ordinal, size_t keep, size_t * atOut) { return ::codedAnswerLineAt(reply, ordinal, keep, atOut); }
bool AIPlayerGPTSelfTestAccess::codedAnswerLineInPlanBlock(const string& reply, int ordinal) { return ::codedAnswerLineInPlanBlock(reply, ordinal); }
bool AIPlayerGPTSelfTestAccess::codedAnswerLinePlanSpan(const string& reply, int ordinal, size_t * atOut, size_t * startOut, size_t * endOut) { return ::codedAnswerLinePlanSpan(reply, ordinal, atOut, startOut, endOut); }
void AIPlayerGPTSelfTestAccess::translogRecordShape(const char * kind, string& outKind, string& outSeam) { ::translogRecordShape(kind, outKind, outSeam); }
long AIPlayerGPTSelfTestAccess::offProtocolBytes(const string& replyIn, bool * actionBeforePlanOut, std::vector<string> * offLinesOut) { return ::offProtocolBytes(replyIn, actionBeforePlanOut, offLinesOut); }
string AIPlayerGPTSelfTestAccess::planLineOnly(const string& replyIn) { return ::planLineOnly(replyIn); }
long AIPlayerGPTSelfTestAccess::postAnswerOverrun(const string& reply) { return ::postAnswerOverrun(reply); }
string AIPlayerGPTSelfTestAccess::protocolLinesOnly(const string& replyIn) { return ::protocolLinesOnly(replyIn); }
bool AIPlayerGPTSelfTestAccess::recordChoiceIsRowIndex(const char * kind) { return ::recordChoiceIsRowIndex(kind); }
string AIPlayerGPTSelfTestAccess::refusedChosenText(int choice, const char * fallback, const std::vector<string> * optionTexts) { return ::refusedChosenText(choice, fallback, optionTexts); }
bool AIPlayerGPTSelfTestAccess::replyActionBeforePlan(const string& replyIn) { return ::replyActionBeforePlan(replyIn); }
int AIPlayerGPTSelfTestAccess::w77ConsumeWindowSeq(int& windowSeq, const char * kind) { return ::w77ConsumeWindowSeq(windowSeq, kind); }
const size_t AIPlayerGPTSelfTestAccess::kLatchedLineKeep = ::kLatchedLineKeep;
#endif //WAGIC_GPT_PARSETEST_BUILD

#endif //WITH_GPT_AI
