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

//#W82-P12: THE ASYNC REQUEST LIFECYCLE AND TRANSPORT - the worker, the slot
//keys, pollCompletion and its retry/force-close legs, the transport stamps.
//Moved verbatim out of AIPlayerGPT.cpp; see AIPlayerGPTInternal.h.


//WAGIC_PADLOG (Android: flag file User/padlog.on): async-lifecycle slice of
//the input/UI trace - spawn/publish/consume/stale transitions, to pin the
//"opponent is thinking forever with no worker thread" stall (2026-08-17).
//Dev builds only - diagnostics are compiled out of release builds.
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
static FILE * gptPadlogFile()
{
    static FILE * out = NULL;
    static int state = 0;
    if (state == 0)
    {
        if (getenv("WAGIC_PADLOG"))
            { out = stderr; state = 1; }
#ifdef ANDROID
        else if (access("/sdcard/Wagic/User/padlog.on", F_OK) == 0)
            { out = fopen("/sdcard/Wagic/User/padlog.txt", "a"); state = out ? 1 : -1; }
#endif
        else state = -1;
    }
    return (state == 1) ? out : NULL;
}
#define GPTASYNCLOG(...) do { FILE * f_ = gptPadlogFile(); if (f_) { fprintf(f_, __VA_ARGS__); fflush(f_); } } while (0)
#else
#define GPTASYNCLOG(...) ((void)0)
#endif //_DEBUG || WAGIC_DEVLOGS


//#W61-V (R4, wave-61 codex review finding 4): did this round trip miss its
//DEADLINE, or did it finish? The wave-53 test was "empty body at/after 95% of
//the deadline", which is elapsed time alone - so an endpoint that answered
//HTTP 503 at 855 s of a 900 s deadline was recorded as a wall miss, opened a
//wall_miss account it never earned, and was bought again with a FULL fresh
//deadline instead of the bounded transport retry a completed error response
//gets. A result that ARRIVED decides first: a non-200 HTTP status is a server
//that answered, and a curl failure that is not CURLE_OPERATION_TIMEDOUT (28)
//is a round trip that died of something other than the clock. What is left -
//no status at all, or curl's own timeout - is the real wall miss, and it is
//unchanged. Pure over the five facts, so the whole table is pinned in
//PARSETEST.
static const long kCurlOperationTimedOut = 28; //CURLE_OPERATION_TIMEDOUT

bool gptDeadlineMissed(bool emptyBody, long elapsedMs, long timeoutMs,
                       long httpStatus, long curlCode)
{
    if (!emptyBody || timeoutMs <= 0)
        return false;
    //#W62-AA (R8, wave-62 codex review finding 8): ANY status that came back
    //means the round trip COMPLETED - the server answered. Wave 61 excused only
    //non-200 statuses, so a 200 that carried an empty body late in the window
    //still opened a wall_miss account and bought a fresh full deadline; it is
    //`empty_reply` (the model said nothing), and the elapsed fraction cannot
    //change what arrived. What remains a wall miss is unchanged: NO status.
    //#W63-AF (R9, wave-63 codex review finding 9): HEADERS ARE NOT A ROUND TRIP.
    //`httpStatus != 0` was read as "the server answered", but curl publishes the
    //status the moment the RESPONSE HEADERS land - a server that sends 200
    //headers, emits no body and stalls until curl gives up at our own deadline
    //reports status 200 AND CURLE_OPERATION_TIMEDOUT, and the status excuse then
    //fired first, so the clock's own verdict was thrown away and the classifier
    //below renamed a real wall miss `transport_error`. Curl's timeout is the one
    //fact that cannot mean anything else: WE set that timeout, so code 28 IS the
    //deadline expiring, whatever arrived before it. Asked before the status.
    //The wave-62 R8 case is untouched: an empty-bodied 200 that COMPLETED
    //carries curlCode 0 and is still `empty_reply`, not a wall miss.
    //#W65-AP (R6, wave-65 codex review finding 6 - HIGH): curl returns 28 for
    //BOTH bounded clocks we set - CURLOPT_TIMEOUT_MS (the decision deadline)
    //and CURLOPT_CONNECTTIMEOUT_MS (#W60-M B4's connect budget, tens of
    //seconds). Wave 63 was right that code 28 is OUR clock and not a foreign
    //fault, and wrong that it is always THIS clock: a connect timeout at 30 s
    //of a 900 s deadline was filed `timeout`, opened a wall_miss account it
    //never earned, and bought a fresh FULL deadline instead of the bounded
    //remainder retry a pre-deadline transport death gets. So the clock has to
    //have actually run: code 28 is a wall miss only inside the same >=95% band
    //every other wall verdict uses (and transportPhaseFor already uses to name
    //the phase on the record). Wave 63's case is untouched - a 200-headers
    //stall that runs to the deadline lands AT it and is still `timeout`.
    if (curlCode == kCurlOperationTimedOut)
        return elapsedMs * 100 >= timeoutMs * 95;
    if (httpStatus != 0)
        return false; //the server answered in full - not the clock
    if (curlCode > 0)
        return false; //transport died of something else (connect, DNS, TLS, reset)
    return elapsedMs * 100 >= timeoutMs * 95;
}


void AIPlayerGPT::WorkerMain(void * p)
{
    WorkerCtx * ctx = reinterpret_cast<WorkerCtx *>(p);
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
    //#W57-U: emulate a transport that NEVER RETURNS - the wedged socket / the
    //worker that dies without publishing. Returning here leaves status == 1
    //for ever, which is precisely the state that used to freeze the duel with
    //no bound (decisionPending re-extends the interrupt offer every tick and
    //ActionStack::Update exempts an in-flight seat from the stall floor). The
    //deadline path is NOT this path: curl honours CURLOPT_TIMEOUT, so a server
    //that accepts the connection and never answers comes back at the wall like
    //any other empty reply. Same role as WAGIC_GPT_NOTHREAD - a way to reach a
    //platform failure on the desktop, where it is easiest to test. Development
    //builds only (owner rule: diagnostics are compiled out of release builds).
    if (getenv("WAGIC_GPT_WEDGE"))
    {
        GPTASYNCLOG("gpt worker wedged (WAGIC_GPT_WEDGE) - publishing nothing\n");
        delete ctx;
        return;
    }
#endif
    string body;
    long httpCode = 0; //audit-L (A24)
    long curlCode = -1; //#W59-H (K1): non-curl transports say unavailable
    if (ctx->codex)
    {
        //The subscription backend answers in the Responses shape over SSE.
        //gptCodexComplete owns auth/refresh and hands back the assistant
        //text; wrapping it in the chat-completions envelope here lets
        //pollCompletion and every downstream reply parser stay unchanged.
        string err;
        string text = gptCodexComplete(ctx->url, ctx->requestBody, ctx->timeoutMs, err);
        if (!text.empty())
        {
            json wrapped = {{"choices", json::array({{{"message", {{"content", text}}}}})}};
            body = wrapped.dump();
        }
        else
        {
            //Empty body = the seams' existing transport-failure path (Baka
            //answers). Log the WHY once per distinct cause, not per decision.
            //audit-L (A49): the dedupe state lives in GptConfig, first-touched
            //on the game thread by gptCurlInit - not a function-local static
            //constructed on THIS worker, where the Vita's lockless __cxa_guard
            //let two concurrently failing seats double-construct the mutex.
            gptLogLineOnce("subscription request failed: " + err);
        }
    }
    else
    {
        GPTASYNCLOG("gpt worker start url=%s body=%zu\n", ctx->url.c_str(), ctx->requestBody.size());
        //audit-L (A24): the status code comes back with the body. A non-200
        //is logged once per distinct (url, code) - the WHY of a match that
        //plays on the heuristic - and rides the state to the record.
        string errBody;
        body = gptHttpPost(ctx->url, ctx->requestBody, ctx->timeoutMs, ctx->key,
                           &httpCode, &errBody, &curlCode);
        if (body.empty() && httpCode != 0 && httpCode != 200)
            gptNoteHttpFailure(ctx->url, httpCode, errBody);
    }
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
    //#W59-H (K1): every empty/non-200 round trip says what both transport
    //layers observed. This is deliberately not emitted by release builds.
    if (body.empty() || httpCode != 200)
    {
        fprintf(stderr, "AIPlayerGPT: transport outcome curl=%ld http=%ld empty=%d\n",
                curlCode, httpCode, body.empty() ? 1 : 0);
        fflush(stderr);
    }
#endif
    //#W82-A (L4, audit-2026-09): the guarded state is kept alive by a LOCAL
    //shared_ptr for the whole of the block below. The discard branch used to run
    //`delete ctx` while `lock_guard g(ctx->state->mtx)` was still in scope: if
    //this context held the last reference (the seat destroyed while an abandoned
    //worker was still in libcurl) that destroyed `AsyncState` - and its mutex -
    //WHILE THE MUTEX WAS LOCKED, and the guard then unlocked freed storage.
    //Destroying a locked std::mutex is undefined behaviour, and on Vita `GptMutex`
    //is a kernel mutex, where it is worse. The local reference outlives the guard,
    //so the unlock always runs on live storage whatever `delete ctx` frees.
    std::shared_ptr<AsyncState> stateAlive = ctx->state;
    bool discarded = false;
    {
        std::lock_guard<GptMutex> g(stateAlive->mtx);
        //#W57-U: was this round trip abandoned while it ran? The game thread
        //bumped the generation and has already answered that decision through
        //the heuristic, so publishing now would hand a stale answer to whatever
        //asks next. Drop it here rather than downstream: the existing stale-drop
        //gate keys on prompt equality, and the slot may since have been re-armed
        //with a prompt this body would falsely match.
        if (ctx->state->gen != ctx->gen)
        {
            GPTASYNCLOG("gpt worker publish discarded - abandoned generation\n");
            delete ctx;
            ctx = NULL;
            discarded = true;
        }
        else
        {
        ctx->state->httpStatus = httpCode; //audit-L (A24)
        ctx->state->curlResult = curlCode; //#W59-H (K1)
        //#W53-Q (D10): the deadline test, on the 95% mark so a wall hit is not
        //missed by scheduling jitter between the client's clock and curl's.
        //An empty body under that mark is a refusal/error, not a timeout.
        long elapsedMs = (long) std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - ctx->state->started).count();
        //#W61-V (R4): an HTTP status or a non-timeout curl result that ARRIVED
        //decides before the clock does.
        ctx->state->timedOut = gptDeadlineMissed(body.empty(), elapsedMs, ctx->timeoutMs,
                                                 httpCode, curlCode);
        ctx->state->response = body;
        ctx->state->status = 2;
        }
    }
    //#W82-A (L4): the guard is destroyed - and the mutex unlocked - before the
    //local reference goes, so whatever `delete ctx` freed above is irrelevant to
    //the unlock. Only now may the discard return.
    if (discarded)
        return;
    GPTASYNCLOG("gpt worker publish bytes=%zu\n", body.size());
    delete ctx;
}


//NATIVE REASONING CAPTURE (wave-34 #1b(A)). With the post-answer scratch
//block gone from the protocol, the reasoning the dev loop audits comes from
//the model's own thinking window - and the server decides where it lands.
//A vLLM running a reasoning parser for the served model puts it in
//message.reasoning_content and hands back clean content; one that does NOT
//leaves the raw "<think> ... </think>" inline at the head of content. Both
//happen on this stack, so the client implements BOTH paths unconditionally
//rather than betting on the server's configuration.
//
//This splits an inline block out of `content` into `reasoning`:
//  * a leading "<think>" closed by "</think>"  -> reasoning = the block,
//    content = what follows (the answer);
//  * no opening tag but a "</think>" present   -> the opener was consumed by
//    the chat template (a real vLLM shape): everything before it is reasoning;
//  * UNCLOSED "<think>"                        -> the reply is REASONING-ONLY
//    (truncation, or a model that opened and never closed). Returns FALSE with
//    content EMPTIED: think text must never be parsed as an answer - a
//    truncated deliberation ends mid-arithmetic and its stray integers are
//    exactly what the index scanners latch onto. The caller re-asks.
//Everything downstream - every length meter, every answer scanner, the
//translog `reply` field - then runs on the STRIPPED text, or the fields would
//be measuring thinking tokens instead of reply tokens.
//Internal slot key for the forced-close (phase-2) request. It only has to be
//a string no assembled prompt can equal - the async slot keys on it, and
//buildRequestBody recognises it, strips it, and rebuilds the SAME user
//message with the assistant prefill appended.
const char * kForceCloseTag = "\x01wagic-force-close\x01";

//#W53-Q (D10): the slot key for the ONE deadline retry. Like the force-close
//tag it is a KEY, not prompt text: the request builder strips it, so the retry
//re-sends a BYTE-IDENTICAL ask (same body, same max_tokens, same deadline) -
//the model never learns it is being re-asked and no reply shape changes. What
//the tag buys is a distinct async slot, which is what keeps the staleness gate
//(prompt equality) in front of the retry and stops a late first answer being
//consumed as the retry's.
const char * kTimeoutRetryTag = "\x01wagic-timeout-retry\x01";

//Consecutive stale-answer drops (no consume between) before an ask gives its
//decision to the heuristic fallback. Legitimate drops are isolated (~2%); a
//run of them is a prompt-stability bug looping at one round trip per cycle.
static const int kStaleLivelockLimit = 6;


//Wave-35 instrumentation: how repetitive is a reasoning trace? Degenerate
//decodes exist in this corpus and are INVISIBLE to every answer-side metric -
//the 415-repeat mojibake trace returned a well-formed reply, and the 13.8k-char
//"No. Okay." loop parsed fine - so nothing counts them today and no budget
//number can be read against them. Measured as the share of the trace's 40-char
//shingles (stride 8, so the scan is linear and cheap even on a 30k-char trace)
//that are copies of the single most common one: normal prose repeats a shingle
//a handful of times out of thousands; a collapse repeats one shingle for most
//of its length. Deliberately NOT a threshold or a behaviour change - it is a
//number to cut on later, once a corpus has produced a distribution.
static double reasoningRepetitionRatio(const string& s)
{
    const size_t kWindow = 40, kStride = 8, kFloor = 400;
    if (s.size() < kFloor)
        return 0.0; //too short for the shape to mean anything
    std::map<string, int> counts;
    int total = 0, best = 0;
    for (size_t i = 0; i + kWindow <= s.size(); i += kStride)
    {
        int c = ++counts[s.substr(i, kWindow)];
        if (c > best)
            best = c;
        total++;
    }
    if (!total)
        return 0.0;
    return (double) best / (double) total;
}


//#W75-CJ (P20, deck146 HIGH 2 - MEASURE ONLY). NEAR-DUPLICATE DEGENERACY.
//`reasoning_degenerate` above is a BYTE-EXACT 40-char window ratio, and it reads
//clean on every record of the wave-74 corpus (deck146 median 0.0034, max 0.0112,
//nothing above 0.5) while 319 of that seat's 322 traces re-quote the reply
//protocol and loop self-verification boilerplate with small edits. The loops are
//near-duplicates, so no exact window ever repeats and the meter is blind BY
//CONSTRUCTION. This is the shingled companion: whitespace-token 8-grams,
//lowercased and stripped of surrounding punctuation so an edited rerun of the
//same paragraph still collides, reported as the share of 8-grams that are not
//first occurrences (0 = every shingle new, ->1 = the trace is one loop).
//It REPLACES nothing and gates nothing: both numbers ride the record, and the
//wave that has two corpora with both can retire whichever is blind.
static double reasoningNgramRepeatRatio(const string& s)
{
    const size_t kN = 8, kMinTokens = 64;
    std::vector<string> toks;
    string cur;
    for (size_t i = 0; i <= s.size(); i++)
    {
        const char c = (i < s.size()) ? s[i] : ' ';
        if (isspace((unsigned char) c))
        {
            if (!cur.empty())
            {
                size_t a = 0, b = cur.size();
                while (a < b && !isalnum((unsigned char) cur[a])) a++;
                while (b > a && !isalnum((unsigned char) cur[b - 1])) b--;
                if (b > a)
                    toks.push_back(cur.substr(a, b - a));
                cur.clear();
            }
        }
        else
            cur += (char) tolower((unsigned char) c);
    }
    if (toks.size() < kMinTokens || toks.size() < kN)
        return 0.0; //too short for the shape to mean anything
    std::set<string> seen;
    size_t total = 0, repeats = 0;
    for (size_t i = 0; i + kN <= toks.size(); i++)
    {
        string g;
        for (size_t k = 0; k < kN; k++)
        {
            if (k)
                g += ' ';
            g += toks[i + k];
        }
        total++;
        if (!seen.insert(g).second)
            repeats++;
    }
    if (!total)
        return 0.0;
    return (double) repeats / (double) total;
}


//The answer reserve, in tokens: everything the reply itself needs once the
//thinking window is spent. Derived from this corpus, not guessed - p95 PLAN
//line 592 chars (~200 tokens) + p95 coded choice line 74 chars (~30 tokens),
//with margin. It is both the head-room added to the thinking budget in phase
//1 and the whole cap of the phase-2 forced close.
extern const long kAnswerReserveTokens = 400;


//#W70-BK (C4): the request ceiling when nothing is configured. It used to be
//a bare literal inside buildRequestBody; it is a named constant now because
//the cap arithmetic moved into a pure function PARSETEST can prove.
extern const long kDefaultReplyCeilingTokens = 4096;

//The answer-locked retry's own answer ceiling: that re-ask needs the coded
//line and nothing else.
extern const long kAnswerLockedRetryTokens = 512;


//Default thinking budget, in tokens, when thinking is ON and nothing is
//configured. The formula (owner, 2026-08-19) is b = 1.5 * (t - p - c) where t
//is the time bound expressed as tokens; at a 240s bound and ~30 tok/s that
//gives 10450. The owner set the shipped starting value at a flat 8000
//("oof. thats a lot. lets make it 8000"). Config/env override it; 0 or less
//means unbounded (the pre-budget behaviour).
//WAVE-35 (OWNER RULING 2026-08-20, "let's fix stuff, then maybe next round will
//propose a smaller budget"): 8000 -> 6000, measured off the wave-34 trace audit
//(~740 traces, 6 batches): last-novel-diagnostic depth pooled to p50 ~3k / p90
//~4.8k tokens, with the per-batch max at or under 6.2k in 5 of 6 batches, so
//6000 clears every batch's p90 with margin and satisfies batch-3's floor
//warning (below ~6k truncates ~10% mid-diagnostic). The audit's unanimous
//caveat is why this cut is paired with the protocol surgery above rather than
//taken alone: the churn is CAUSED and INTERLEAVED, not trailing, so a cap
//cannot recover it - only removing the undecidable instructions can. Wave-35's
//corpus re-measures depth post-surgery and any further cut is proposed from
//that data.
extern const long kDefaultReasoningBudget = 6000;


//Which endpoints accept an assistant prefill (vLLM's continue_final_message /
//llama.cpp's equivalent). The subscription (Codex) and OpenRouter paths keep
//reasoning server-side - there is nothing for the client to close - and
//api.openai.com rejects unknown top-level fields with a 400, which would turn
//every budget hit into a lost decision instead of a forced answer.
static bool gptForceCloseSupported(const string& endpoint)
{
    if (gptCodexEndpoint(endpoint))
        return false;
    return endpoint.find("api.openai.com") == string::npos
        && endpoint.find("openrouter.ai") == string::npos;
}


//#W75-CJ (P2). THE PHASE-2 PREFILL MUST SURVIVE THE CHAT TEMPLATE UNCHANGED.
//vLLM's `continue_final_message` renders the whole conversation and then
//truncates the rendered prompt at `rindex(final message content)`; if the
//template did not reproduce that content BYTE FOR BYTE the request is a 400
//("continue_final_message is set but the final message does not appear in the
//chat after applying the chat template"). The pilot's Qwen template renders an
//assistant turn as `<think>\n` + reasoning|trim + `\n</think>\n\n` + content,
//and it derives `reasoning` from our own content by
//`split('</think>')[0].rstrip('\n').split('<think>')[-1].lstrip('\n')` and then
//`|trim`. So the ONE thing that breaks the round trip is a prefill with LEADING
//OR TRAILING WHITESPACE (or an embedded think marker, which moves the split):
//the template trims it away, the rendered text no longer contains what we sent,
//and the decision is lost to the heuristic.
//Wave-74 corpus: 2 of 12 forced closes 400'd, and one of them was an ATTACK
//DECLARATION (`126v152` seq 48 - three lifelink Vampires and Sanguine Bond
//against 9 life). Both prefills ended in whitespace (`...\n   ` and `...\n`);
//the ten that succeeded did not. Live-probed against the pilot 2026-09-09, one
//request per shape: trailing-newline prefill -> 400 with that exact message,
//the same prefill trimmed -> 200 with a clean answer.
//#W75-CM (F6): Python's `str.strip()` whitespace set, UTF-8 encoded - every
//code point for which `str.isspace()` is true, which is what Jinja's `|trim`
//removes. Returns the byte length of the whitespace character at `i`, or 0.
static size_t gptPyWsLenAt(const string& s, size_t i)
{
    const unsigned char c = (unsigned char) s[i];
    if (c == ' ' || (c >= 0x09 && c <= 0x0D) || (c >= 0x1C && c <= 0x1F))
        return 1;
    if (c == 0xC2 && i + 1 < s.size())
    {
        const unsigned char d = (unsigned char) s[i + 1];
        if (d == 0x85 || d == 0xA0) //U+0085 NEL, U+00A0 NBSP
            return 2;
        return 0;
    }
    if (c == 0xE1 && i + 2 < s.size()
        && (unsigned char) s[i + 1] == 0x9A && (unsigned char) s[i + 2] == 0x80)
        return 3; //U+1680 OGHAM SPACE MARK
    if (c == 0xE2 && i + 2 < s.size())
    {
        const unsigned char d = (unsigned char) s[i + 1], e = (unsigned char) s[i + 2];
        if (d == 0x80 && ((e >= 0x80 && e <= 0x8A) //U+2000..U+200A
                          || e == 0xA8 || e == 0xA9 || e == 0xAF)) //U+2028/9, U+202F
            return 3;
        if (d == 0x81 && e == 0x9F) //U+205F MEDIUM MATHEMATICAL SPACE
            return 3;
        return 0;
    }
    if (c == 0xE3 && i + 2 < s.size()
        && (unsigned char) s[i + 1] == 0x80 && (unsigned char) s[i + 2] == 0x80)
        return 3; //U+3000 IDEOGRAPHIC SPACE
    return 0;
}


//First byte that is not whitespace, or npos when the whole string is.
static size_t gptPyStripFrom(const string& s)
{
    size_t i = 0;
    while (i < s.size())
    {
        const size_t w = gptPyWsLenAt(s, i);
        if (!w)
            return i;
        i += w;
    }
    return string::npos;
}


//One PAST the last byte that is not whitespace (0 when the whole string is).
static size_t gptPyStripTo(const string& s)
{
    size_t i = 0, end = 0;
    while (i < s.size())
    {
        const size_t w = gptPyWsLenAt(s, i);
        if (w)
        {
            i += w;
            continue;
        }
        const unsigned char c = (unsigned char) s[i];
        size_t adv = 1;
        if (c >= 0xF0)
            adv = 4;
        else if (c >= 0xE0)
            adv = 3;
        else if (c >= 0xC0)
            adv = 2;
        if (i + adv > s.size())
            adv = s.size() - i;
        i += adv;
        end = i;
    }
    return end;
}


//Pure, so PARSETEST pins the shape without a round trip.
string gptForceClosePrefillBody(const string& prefill)
{
    string inner = prefill;
    //An embedded marker would move the template's own split and desynchronise
    //the round trip; the trace is the model's thinking, not a nested block.
    for (;;)
    {
        size_t m = inner.find("</think>");
        if (m == string::npos)
            break;
        inner.erase(m, 8);
    }
    for (;;)
    {
        size_t m = inner.find("<think>");
        if (m == string::npos)
            break;
        inner.erase(m, 7);
    }
    //Jinja's `|trim` is str.strip(): the same whitespace set, both ends.
    //(history: comment-archaeology.md AIPlayerGPTTransport-gptPyStripFrom-1674)
    size_t a = gptPyStripFrom(inner);
    //A whitespace-only trace still has to produce a NON-EMPTY final message: an
    //empty one makes `rindex("")` the end of the render, which would continue
    //the prompt AFTER `<|im_end|>` and open a fresh turn. The empty think block
    //is what the template itself renders for an empty reasoning field, so it
    //round-trips exactly.
    if (a == string::npos)
        return "<think>\n\n</think>\n\n";
    const size_t b = gptPyStripTo(inner);
    inner = inner.substr(a, b - a);
    return "<think>\n" + inner + "\n</think>\n\n";
}

bool gptForceCloseEarned(bool reasoningOnly, bool contentEmpty,
                                bool finishLength, bool haveReasoning,
                                int codedAnswers)
{
    if (!haveReasoning)
        return false;                       //nothing to prefill back
    if (reasoningOnly && contentEmpty)
        return true;                        //the wave-34 arm, unchanged
    return finishLength && codedAnswers == 0; //#W75-CJ (P21): the cap ate the answer
}


static bool splitReasoningBlock(string& content, string& reasoning)
{
    size_t head = content.find_first_not_of(" \t\r\n");
    if (head == string::npos)
        return true; //empty/whitespace reply: the transport-failure path owns it
    bool opensWithThink = (content.compare(head, 7, "<think>") == 0);
    size_t close = content.find("</think>");
    if (!opensWithThink && close == string::npos)
        return true; //no thinking block in this reply at all
    if (close == string::npos)
    {
        //Opened and never closed: reasoning-only.
        string r = content.substr(head + 7);
        size_t rs = r.find_first_not_of(" \t\r\n");
        reasoning = (rs == string::npos) ? string() : r.substr(rs);
        content.clear();
        return false;
    }
    size_t rStart = opensWithThink ? head + 7 : 0;
    string r = content.substr(rStart, close - rStart);
    size_t rs = r.find_first_not_of(" \t\r\n");
    size_t re = r.find_last_not_of(" \t\r\n");
    if (rs != string::npos)
        reasoning = r.substr(rs, re - rs + 1);
    string rest = content.substr(close + 8);
    size_t as = rest.find_first_not_of(" \t\r\n");
    content = (as == string::npos) ? string() : rest.substr(as);
    return true;
}


//HIDDEN REASONING IS NORMAL, NOT A FAULT. Some providers reason and never
//return the trace: OpenAI and Anthropic withhold raw chain-of-thought as
//policy, and OpenRouter can hide it depending on the upstream - which this
//project has already been bitten by once, as the 40s mystery latency behind a
//140-token answer (71f4f615c). The reply is complete and correct; only the
//audit copy is missing.
//
//So this shape - reasoning REQUESTED, content non-empty, no reasoning field
//anywhere - parses exactly as the thinking-off path does, and is recorded as
//`reasoning_hidden` so review can tell "reasoned invisibly" from "did not
//reason". It is the INVERSE of reasoning_only (content empty, reasoning
//present) and must never be confused with it. Nothing in this client gates
//parsing, fallback or any assertion on reasoning being present: the "reasoning
//present at both seats before the corpus runs" rule is a Spark/dev-loop
//precondition for the A/B, not a client invariant - encoding it here would
//turn every OpenAI/Anthropic user's game into a heuristic-only game.
static bool reasoningHiddenShape(bool requested, const string& content, const string& reasoning)
{
    return requested && !content.empty() && reasoning.empty();
}


//THE ANSWER CAN ARRIVE IN THE REASONING FIELD (live-probed against Spark's
//vLLM 0.23.1rc1 + qwen35, 2026-08-19). The server's reasoning parser
//classifies by GENERATED tokens only: on the forced close, the "</think>" we
//PREFILL is never generated, so the parser never sees a thinking block end and
//routes the ENTIRE phase-2 generation into `message.reasoning` with `content`
//null - even though the injected close did its job on the MODEL, which
//answered immediately ("\n\nCHOICE: 1"). Throwing that away would lose exactly
//the decision the budget machinery exists to rescue.
//
//So: when content is empty and the reasoning field ENDS on a coded answer
//line, that line is the answer. Anchored to the END deliberately - phase-1
//mid-thinking text is full of answer-shaped candidates the model was still
//weighing ("maybe CHOICE: 3? no, 15 mana"), and those must NEVER be parsed.
//A reply cut off at the token cap (finish_reason == length) is mid-thought by
//definition and is refused here whatever it looks like; the forced close is
//the only path that recovers it.
//Returns the answer text (from the last coded answer line onward), or empty.
static string answerTailFromReasoning(const string& reasoning)
{
    size_t lineStart = 0, answerAt = string::npos;
    while (lineStart <= reasoning.size())
    {
        size_t lineEnd = reasoning.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? reasoning.size() : lineEnd;
        string line = reasoning.substr(lineStart, end - lineStart);
        if (hasCodedAnswerLine(line))
            answerAt = lineStart;
        else
        {
            //Anything substantive AFTER the last coded line means the model
            //kept thinking past it - that candidate was not its final word.
            size_t sig = line.find_first_not_of(" \t\r");
            if (sig != string::npos && answerAt != string::npos)
                answerAt = string::npos;
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    if (answerAt == string::npos)
        return string();
    return reasoning.substr(answerAt);
}


bool AIPlayerGPT::asyncBusy() const
{
    //#W57-A (D5): busy if EITHER arm has a round trip in flight. The second
    //slot separates what each arm REMEMBERS, never how many requests are out:
    //every caller that reads this (the decision gate, the stall watchdog, the
    //launch guard in pollCompletion) still sees at most one live request.
    {
        std::lock_guard<GptMutex> g(mAsyncState->mtx);
        if (mAsyncState->status == 1)
            return true;
    }
    std::lock_guard<GptMutex> g2(mAsyncLandState->mtx);
    return mAsyncLandState->status == 1;
}


//#W57-T (softlock escape): the seat's async/pending state, for the diagnostics
//dump. In the owner's vpk16 report the game loop was ALIVE and the selector
//frozen because the layers did not name his seat as the acting one; whether an
//AI seat had a round trip out at that moment is one of the facts that would
//separate the candidate causes, and nothing in the game recorded it. Hence:
//per arm, does it have a request out, for how long, and for which question.
//One line per arm; the mutexes are taken and released exactly the way
//asyncBusy takes them (no allocation while held).
std::string AIPlayerGPT::softlockDiagnostic() const
{
    std::ostringstream o;
    const char * armName[2] = { "main", "land" };
    const std::shared_ptr<AsyncState> arms[2] = { mAsyncState, mAsyncLandState };
    for (int i = 0; i < 2; ++i)
    {
        int status = -1; long http = 0; bool timedOut = false;
        std::string slot; size_t promptLen = 0, respLen = 0; long long inflightMs = -1;
        if (arms[i])
        {
            std::lock_guard<GptMutex> g(arms[i]->mtx);
            status = arms[i]->status;
            http = arms[i]->httpStatus;
            timedOut = arms[i]->timedOut;
            slot = arms[i]->slotKey;
            promptLen = arms[i]->prompt.size();
            respLen = arms[i]->response.size();
            if (status == 1)
                inflightMs = (long long) std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - arms[i]->started).count();
        }
        //The slot key is the seam: question + options tail + turn/phase/board.
        //Newlines in it would break the one-line-per-field shape of the dump.
        for (size_t k = 0; k < slot.size(); ++k)
            if (slot[k] == '\n' || slot[k] == '\r') slot[k] = '|';
        if (slot.size() > 300) slot = slot.substr(0, 300) + "...";
        o << "  arm=" << armName[i]
          << " status=" << status
          << (status == 0 ? "(idle)" : status == 1 ? "(in-flight)" : status == 2 ? "(done-unconsumed)" : "(none)")
          << " inflight_ms=" << inflightMs
          << " http=" << http
          << " timedout=" << (timedOut ? 1 : 0)
          << " prompt_bytes=" << (unsigned) promptLen
          << " response_bytes=" << (unsigned) respLen
          << " seam='" << slot << "'\n";
    }
    std::string notice = mNotice;
    for (size_t k = 0; k < notice.size(); ++k)
        if (notice[k] == '\n' || notice[k] == '\r') notice[k] = '|';
    o << "  endpoint_set=" << (mEndpoint.empty() ? 0 : 1)
      << " in_flight=" << (aiDecisionInFlight() ? 1 : 0)
      << " think_s=" << mThinkTime
      << " timeout_ms=" << mTimeoutMs
      << " patience_s=" << mPatienceLimit
      << " last_latency_ms=" << mLastLatencyMs
      << " fallbacks=" << mFallbackCount
      << " degraded_ticks=" << mDegradedTicks
      << " notice='" << notice << "'";
    return o.str();
    }

//#W57-U (the vpk16 in-flight softlock, 2026-09-03). THE BOUND ON AN IN-FLIGHT
//CALL. While a request is in flight this seat neither acts nor passes, and
//decisionPending calls extendInterruptOffer every tick, which the wave-54
//stall floor honours as progress (`aiDecisionInFlight` exempts the holder from
//both the 300-tick and the 1,200 s budgets). That exemption is CORRECT for a
//live call - wave-55 corpora hold legitimate 900 s decisions and cutting one
//short loses the game's whole opponent-turn surface - but it had no wall at
//all, so a round trip that never comes back (a wedged socket on the console's
//network stack, a server that accepts and never answers, a worker thread that
//dies without publishing) freezes the duel for ever with the turn indicator
//pinned on the AI: the owner's vpk16 report shape.
//
//The bound is DERIVED, never a fixed number: the request already carries its
//own deadline, so a call still out past that deadline plus a grace is one
//whose deadline machinery has itself failed. grace = half the deadline,
//clamped to [1 s, 30 s]: proportional for a short configured deadline, and
//capped so a 900 s wall does not buy another 450 s of frozen screen.
//At the shipped default (timeout=600 s) that is 630 s for ONE request; the
//deadline retry is a SEPARATE request with its own launch time, so a decision
//that misses the wall honestly and then wedges on the retry is bounded by
//600 + 630 = 1,230 s. ActionStack::Update carries the same arithmetic as its
//own backstop, for a seat whose in-flight flag is stuck for any other reason.
//#W57-U: THE DISABLE FLAG for the in-flight bound (both halves: the request
//abandonment here and ActionStack's stall-floor backstop, which keeps its own
//copy - not every platform build compiles both translation units against a
//shared one). Every output-affecting change here ships one, so "was it the new
//bound?" is one env var on a shipped binary instead of a build swap - and so
//the defect it fixes stays reproducible on the SAME binary as a positive
//control. WAGIC_INFLIGHT_BOUND=0 restores the pre-#W57-U behaviour: an
//in-flight call holds its window for ever.
static bool inFlightBoundEnabled()
{
    static int cached = -1;
    if (cached < 0)
    {
        const char * v = getenv("WAGIC_INFLIGHT_BOUND");
        cached = (v && (v[0] == '0') && !v[1]) ? 0 : 1;
    }
    return cached != 0;
}


long AIPlayerGPT::inFlightAbandonMsFor(long deadlineMs)
{
    if (deadlineMs <= 0)
        return 0; //no deadline to derive a bound from: nothing is abandoned
    long grace = deadlineMs / 2;
    if (grace < 1000) grace = 1000;
    if (grace > 30000) grace = 30000;
    return deadlineMs + grace;
}


long AIPlayerGPT::inFlightAbandonMs() const
{
    return inFlightAbandonMsFor(mTimeoutMs);
}


bool AIPlayerGPT::reapWedgedRequests(const std::shared_ptr<AsyncState>& polled, const char * polledArm)
{
    if (mEndpoint.empty() || !inFlightBoundEnabled())
        return false;
    const long bound = inFlightAbandonMs();
    if (bound <= 0)
        return false;
    bool polledAbandoned = false;
    //BOTH arms, not just the one being polled: the launch guard is
    //`asyncBusy()`, so a wedged LAND-drop request blocks every casting ask too
    //and would never be looked at by a poll of the casting arm.
    std::shared_ptr<AsyncState> arms[2] = { mAsyncState, mAsyncLandState };
    const char * names[2] = { "casting", "land-drop" };
    for (int i = 0; i < 2; i++)
    {
        long elapsedMs = 0;
        {
            std::lock_guard<GptMutex> g(arms[i]->mtx);
            if (arms[i]->status != 1)
                continue;
            elapsedMs = (long) std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - arms[i]->started).count();
            if (elapsedMs < bound)
                continue;
            //ABANDONED. The generation bump is what makes this safe: the worker
            //may still be alive and may still write, and on Vita it cannot be
            //joined or cancelled - so nothing is freed and nothing is cleared
            //out from under it. Its publish will find a generation that has
            //moved and drop the body. The slot itself goes back to IDLE, which
            //is the same shape the refused-worker path leaves behind.
            ++arms[i]->gen;
            arms[i]->status = 0;
            arms[i]->response.clear();
            arms[i]->prompt.clear();
            arms[i]->slotKey.clear();
            arms[i]->timedOut = false;
            arms[i]->httpStatus = 0;
        }
        const long secs = elapsedMs / 1000;
        //ONE line per abandonment, in the log the console actually keeps.
        //Not a once-per-process line: unlike a refused thread this is a
        //per-call fact, and a match that abandoned four calls read as one.
        {
            char buf[192];
            snprintf(buf, sizeof(buf),
                     "model call abandoned after %ld s (deadline %ld s):"
                     " answering this decision with the heuristic AI",
                     secs, mTimeoutMs / 1000);
            gptLogLine(string(buf));
        }
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS) || defined(WAGIC_TRANSCRIPT_ON)
        fprintf(stderr, "wagic: %s arm - model call abandoned after %ld s"
                        " (deadline %ld s, bound %ld s) - the heuristic AI answers this decision\n",
                names[i], secs, mTimeoutMs / 1000, bound / 1000);
        fflush(stderr);
#endif
        if (arms[i] == polled)
        {
            polledAbandoned = true;
            mAbandonedInFlightSecs = secs; //stamped onto this decision's record
            mAbandonedInFlightSeq = mWindowSeq; //#W82-A (L10): ...and onto THAT window
            setNotice("no reply from the model - playing this one with the built-in AI", 4.0f);
        }
    }
    (void) polledArm;
    return polledAbandoned;
}


//THE NO-ANSWER CLASSES ARE NOT ALL THE SAME FAILURE. "empty_reply" has always
//meant TRANSPORT: nothing came back, or the endpoint is down - the reply body
//was empty. With a server-side reasoning parser, a reply can arrive complete,
//well-formed and paid for, with content null because the whole generation was
//filed as thinking (a budget hit, or a model that spent its window without
//committing). Calling that "empty_reply" would put a MODEL behaviour into the
//bucket a seat review reads as an infrastructure fault - and the wave-34 A/B
//is scored on exactly this distinction. reasoning_only names it.
//Did THIS endpoint's request ask for reasoning at all? The thinking flag
//covers the vLLM/llama.cpp/OpenRouter families; the subscription (Codex)
//backend has no thinking flag and instead carries an effort tier, where unset
//means the built-in default (low) and only an explicit "none" is off. Used
//solely to tell a WITHHELD trace from a reply that never reasoned - never to
//gate parsing.
bool AIPlayerGPT::reasoningRequested() const
{
    if (gptCodexEndpoint(mEndpoint))
        return mReasoningEffort != "none";
    return mThinking;
}


//#W53-Q (D10). A 900 s non-answer used to arrive at seat review as
//"empty_reply" - the same word an unreachable endpoint writes - so three
//decisions lost at the wall in the wave-52 corpus (126v146 seq 1 and 123v130
//seq 3, both OPENING-HAND keeps decided by the heuristic; 130v162 seq 18) were
//indistinguishable from a dead server, and nothing in stderr said a deadline
//had passed. "timeout" is its own class: the request was accepted, the model
//was thinking or queued, and the CLOCK ran out. Pure over the three facts, so
//the whole table is pinned in PARSETEST.
//#W82-A (audit-2026-09): ONE table, not a four-deep chain of forwarders. The
//four overloads (3-, 4-, 5- and 6-argument) each added one fact and delegated to
//the one below it; the ORDER was the whole content and it is now written once,
//top to bottom, in the order the comments already argued for:
//  * a stale-livelock give-up consumed no round trip and outranks everything
//    (every level of the old chain guarded on `!staleLivelock`);
//  * a body that ARRIVED and could not be read is a protocol fault, not a
//    silent model - ranked above the http/model split for exactly that reason;
//  * a round trip that died BEFORE the deadline is transport (connect refused,
//    DNS, TLS, reset). The `!timedOut` guard is load-bearing: a request killed
//    at its own deadline ALSO reports a nonzero CURLcode (28,
//    CURLE_OPERATION_TIMEDOUT), so without it every wall miss would be renamed
//    `transport_error` and #W53-Q (D10)'s "the clock ran out" class - the one
//    the seat reviews read - would vanish;
//  * a server that ANSWERED 401/404/413/429/5xx is its own class EVEN AT THE
//    WALL (audit-L A24: the server answered), so it precedes the deadline;
//    0 and 200 carry nothing new and fall through;
//  * then the deadline, then the three-fact table: reasoning-only vs empty.
//Pure, so the whole table is pinned in PARSETEST.
const char * AIPlayerGPT::noAnswerClassFor(bool staleLivelock, bool timedOut,
                                           bool hasReasoning, long httpStatus,
                                           long curlCode, bool badReply)
{
    if (staleLivelock)
        return "stale_livelock";
    if (badReply && !timedOut && curlCode <= 0 && (httpStatus == 0 || httpStatus == 200))
        return "bad_reply";
    if (!timedOut && curlCode > 0)
        return "transport_error";
    if (httpStatus != 0 && httpStatus != 200)
        return "http_error";
    if (timedOut)
        return "timeout";
    return hasReasoning ? "reasoning_only" : "empty_reply";
}


//#W59-H (K1): ONE retry is offered only for transport-shaped failures. A 4xx
//is a completed client error and an empty HTTP 200 is an answered empty body;
//neither is made more legal by buying the same request again.
bool AIPlayerGPT::retryableTransportFailure(long curlCode, long httpStatus, bool emptyBody)
{
    return curlCode > 0 || httpStatus >= 500
           || (emptyBody && httpStatus == 0);
}


//#W59-H (K1): the second attempt receives only what the first left. This is
//the bound that makes the retry one decision deadline rather than two.
long AIPlayerGPT::remainingTransportRetryMs(long deadlineMs, long firstLatencyMs)
{
    if (deadlineMs <= 0 || firstLatencyMs < 0 || firstLatencyMs >= deadlineMs)
        return 0;
    return deadlineMs - firstLatencyMs;
}


//#W59-H (K1): stable scalar shape for the translog field. Keeping curl and
//HTTP beside one another prevents a status 0 from erasing CURLE_OPERATION_TIMEDOUT.
//#W82-A (audit-2026-09): the 3-argument forwarder is DELETED - both production
//calls already pass the 6-argument form, and only a PARSETEST pin reached it.

//#W61-U (C13): WHICH PHASE a curl failure died in. Both wave-60 transport
//records read `curl=28,http=0,empty=1` at latency 900,024 / 900,027 ms against
//a 900,000 ms deadline: CURLE_OPERATION_TIMEDOUT (28) is returned for BOTH the
//connect timeout and the whole-request timeout, and only the elapsed time tells
//them apart. At 100% of the deadline that is a WALL miss - correctly classed
//`timeout` / `wall_miss`, not `transport_error` - while a connect-phase death
//lands at the connect budget B4 bounded to <= 20 s and <= half the deadline,
//which is exactly what makes `transport_error` (and its retry inside what is
//left of the deadline) reachable. That adjudication was made by hand off two
//records; this writes it down on every record instead. "" whenever the facts do
//not settle it (no curl failure, no latency, neither bound known) - a phase is
//never guessed. Pure over the four numbers.
const char * AIPlayerGPT::transportPhaseFor(long curlCode, long latencyMs,
                                            long connectBudgetMs, long deadlineMs)
{
    if (curlCode <= 0 || latencyMs < 0)
        return "";
    if (deadlineMs > 0 && latencyMs * 100 >= deadlineMs * 95)
        return "wall";       //the whole deadline elapsed: the server never answered
    if (connectBudgetMs > 0 && latencyMs <= connectBudgetMs + connectBudgetMs / 4)
        return "connect";    //died at the connect bound: the round trip never started
    if (deadlineMs > 0 || connectBudgetMs > 0)
        return "midflight";  //neither bound: the connection was made and then broke
    return "";
}


//#W61-U (C13): the same stamp, with the two facts that make the phase provable
//to a corpus reader - the connect budget that was in force, and the verdict
//above. Both are appended ONLY on a curl failure, so every other stamp (and
//every stamp written before this wave) is byte-identical.
string AIPlayerGPT::transportOutcomeStamp(long curlCode, long httpStatus, bool emptyBody,
                                          long connectBudgetMs, long latencyMs, long deadlineMs)
{
    std::ostringstream o;
    o << "curl=" << curlCode << ",http=" << httpStatus
      << ",empty=" << (emptyBody ? 1 : 0);
    if (curlCode > 0 && connectBudgetMs > 0)
        o << ",connect_ms=" << connectBudgetMs;
    const char * phase = transportPhaseFor(curlCode, latencyMs, connectBudgetMs, deadlineMs);
    if (*phase)
        o << ",phase=" << phase;
    return o.str();
}


//#W69-BI (K7, engine MED-3): THE INSTRUMENT THAT COULD NOT PROVE ITSELF.
//Counted first on `matchups-20260906-134120` (42 seats, 2,414 records): the
//`transport` field appears on 0 records and `attempt_ms` on 0, because both
//were CONDITIONAL emitters - `transport` was pushed only for a non-200 or
//empty body, `attempt_ms` only when a retry set two legs - and this corpus had
//0 wall misses, 0 retries and 0 non-200 replies. Nothing was mis-wired; the
//fields were unfalsifiable from any clean corpus, which is exactly what the
//wave-68 engine seat could not distinguish ("that cannot be distinguished from
//not wired without a window"). Both now write on EVERY record that spent a
//round trip: a successful trip stamps `curl=0,http=200,empty=0` (the same
//builder, so a failed trip's stamp is byte-identical to what shipped), and a
//single-attempt record publishes its one leg. A record with no round trip
//(cache/reuse, latency -1) still writes neither - there is nothing to stamp.
//Pure over the recorded numbers, so PARSETEST pins the shape without a server.
string transportRecordField(const std::vector<std::string>& failures,
                                   const std::string& roundTripStamp, long latencyMs)
{
    if (!failures.empty())
    {
        string s;
        for (size_t i = 0; i < failures.size(); i++)
            s += (i ? ";" : "") + failures[i];
        return s; //every failed attempt, as #W59-H (K1) wrote them
    }
    if (latencyMs < 0)
        return ""; //no round trip was spent on this record
    return roundTripStamp;
}


//#W69-BI (K7): the legs of THIS record's round trip(s). Two entries when a
//retry ran (#W68-BC J2, unchanged), one entry on the ordinary path, none when
//no call was made. A reader can now divide `latency_ms` by attempt without
//having to know whether the absence of the field means "one call" or "not
//wired".
std::vector<long> attemptMsField(long firstMs, long secondMs, long latencyMs)
{
    std::vector<long> a;
    if (firstMs >= 0 || secondMs >= 0)
    {
        a.push_back(firstMs);
        a.push_back(secondMs);
        return a;
    }
    if (latencyMs >= 0)
        a.push_back(latencyMs);
    return a;
}


//#W55-E (D5b): stale-drop prompt-drift localiser. Prints the first byte at which
//the rebuilt prompt diverges from the one in flight, both neighbourhoods, and
//both lengths - which is enough to name the SECTION (narration / situation
//prefill / option list) without diffing two 40 KB strings by hand.
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
static void gptTracePromptDrift(const string& inflight, const string& rebuilt)
{
    if (!getenv("WAGIC_GPT_DRIFT"))
        return;
    size_t n = inflight.size() < rebuilt.size() ? inflight.size() : rebuilt.size();
    size_t i = 0;
    while (i < n && inflight[i] == rebuilt[i]) i++;
    size_t from = i > 60 ? i - 60 : 0;
    std::cerr << "[DRIFT] first diff at " << i << " of " << inflight.size()
              << " -> " << rebuilt.size() << "\n[DRIFT] inflight: |"
              << inflight.substr(from, 200) << "|\n[DRIFT] rebuilt : |"
              << rebuilt.substr(from, 200) << "|" << std::endl;
}
#define GPT_DRIFT_TRACE(a, b) gptTracePromptDrift((a), (b))
#else
#define GPT_DRIFT_TRACE(a, b) do { } while (0)
#endif


const char * AIPlayerGPT::noAnswerClass() const
{
    return noAnswerClassFor(mLastStaleLivelock, mLastTimeout, !mLastReasoning.empty(),
                            mLastHttpStatus, mLastCurlResult, mLastBadReply);
}


//#W54-B (D9). noAnswerClassFor above is the branch this item does NOT cover:
//it classifies a reply that never came. The wave-53 corpus had ZERO empty
//replies and six that reached 600 s and ANSWERED - 126v125 seq 13 at 868,729
//ms (96.5% of the 900 s wall, and one of D13's two mis-executions), 126v146
//seq 22 and 23 (the two windows that lost that game), 162v126 seq 10, 123v126
//seq 1, 152v123 seq 16 - and not one carried a mark of any kind. These two
//functions are the mark's arithmetic, pure so the table is pinned.
long AIPlayerGPT::deadlineTenthsPct(long latencyMs, long timeoutMs)
{
    if (latencyMs < 0 || timeoutMs <= 0)
        return -1; //no round trip on this record (cache/reuse), or no deadline
    return (latencyMs * 1000) / timeoutMs;
}


//#W68-BC (J2): the same fraction for a record that spent TWO round trips. The
//shipped field divided the SUMMED latency by ONE deadline, so 162v126 s13
//(900,035 ms miss + 333,376 ms retry against a 900,000 ms deadline) published
//`deadline_pct 137.0` - the corpus signature for "a call ran past its
//deadline" - on a record where NEITHER leg did. The honest figure is the
//worst ATTEMPT: what any single call cost of the deadline it was given. The
//sum is not lost; it is `latency_ms`, and the legs are `attempt_ms`.
long AIPlayerGPT::deadlineTenthsPctOfAttempts(long firstMs, long secondMs, long timeoutMs)
{
    const long a = deadlineTenthsPct(firstMs, timeoutMs);
    //#W68-BE (R8, codex review finding 8 - LOW): the SECOND attempt is never given
    //the original deadline. The retry arm hands it remainingTransportRetryMs - what
    //the first leg left - and buildRequestBody uses that as the call's timeout, so
    //dividing it by `timeoutMs` under-reports exactly the attempt at risk: a 120 s
    //decision that fails transport at 20 s and then spends its whole 100 s remainder
    //published 83.3% for a leg that consumed 100% of its budget. Each leg is now
    //measured against the budget it actually ran under.
    long secondBudget = (firstMs >= 0) ? remainingTransportRetryMs(timeoutMs, firstMs)
                                       : timeoutMs;
    if (secondBudget <= 0)
        secondBudget = timeoutMs; //no knowable remainder: fall back to the deadline
    const long b = deadlineTenthsPct(secondMs, secondBudget);
    return a > b ? a : b;
}


//#W68-BC (J2): whether a SECOND attempt fits inside what is left of this
//decision's deadline. The transport arm already asked this question
//(remainingTransportRetryMs); the wall arm did not - it set mRetryBudgetMs = 0,
//which the request builder reads as "no override", i.e. a fresh FULL deadline,
//so one decision could burn 2x WAGIC_GPT_TIMEOUT (1800 s on this corpus's
//dial). A wall miss has spent the whole deadline by definition, so the
//remainder is 0 and there is no second attempt to buy: the heuristic answers
//and the record says `wall_miss_no_retry`. Lane AX read the transport arm's
//arithmetic and concluded a wall miss already got no retry; the corpus says
//otherwise - 4 wall misses, all 4 armed a retry, 1 of them consumed a second
//333 s deadline (162v126 s13) and 3 were abandoned when the decision drifted.
bool AIPlayerGPT::retryFitsInDeadline(long deadlineMs, long firstLatencyMs)
{
    return remainingTransportRetryMs(deadlineMs, firstLatencyMs) > 0;
}


bool AIPlayerGPT::isLongReply(long latencyMs, long timeoutMs, bool answered)
{
    //the SAME >= 95% mark the worker's timeout test uses, on the answered side
    return answered && latencyMs >= 0 && timeoutMs > 0 && latencyMs * 100 >= timeoutMs * 95;
}


static string asyncSlotKeyOf(bool forceClose, int turn, int phase,
                             const string& seamTail, const string& board)
{
    std::ostringstream k;
    k << (forceClose ? "FC|" : "|") << turn << "|" << phase << "|\n"
      << seamTail << "\n=BOARD=\n" << board;
    return k.str();
}


//#W57-A (D5): WHICH ARM a seam tail belongs to. The land-drop ask is the only
//question this seat puts that a casting window can interleave with across a
//turn flip, and the wave-56 corpus classified all 65 stale drops by the ask
//that followed them: 44 `Land drop:`, 21 `Casting decision`, and every one of
//the 32 drops preceded by the Baka opponent's own land auto-tap
//("AIPlayerBaka: Mana cost is NULL.") is inside the 44. So the displacement is
//between the two ARMS, not inside either one. The test is the tail's first
//line, which is the question itself - the same string the slot key already
//carries, so no new state is introduced and the classification is a pure
//function of what was asked.
static const char * kLandDropQuestionPrefix = "Land drop: ";

bool asyncLandArm(const string& seamTail)
{
    return seamTail.compare(0, strlen(kLandDropQuestionPrefix), kLandDropQuestionPrefix) == 0;
}


//#W57-A (D5): a drop's own classification, so the residual never needs a second
//archaeology pass over two 40 KB strings. Both keys have the shape
//"<fc>|turn|phase|\n<seam>\n=BOARD=\n<board>", so the two halves are separable
//by their own separator and a drop can say WHICH half moved.
static const char * kAsyncSlotBoardSep = "\n=BOARD=\n";


static string asyncSlotDriftKind(const string& oldKey, const string& newKey)
{
    if (oldKey.empty() || newKey.empty())
        return "unknown";
    if (oldKey == newKey)
        return "same slot";
    const size_t a = oldKey.find(kAsyncSlotBoardSep);
    const size_t z = newKey.find(kAsyncSlotBoardSep);
    if (a == string::npos || z == string::npos)
        return "unknown";
    const bool seamMoved = (oldKey.compare(0, a, newKey, 0, z) != 0);
    const bool boardMoved = (oldKey.compare(a, string::npos, newKey, z, string::npos) != 0);
    if (seamMoved && boardMoved)
        return "question and board";
    if (seamMoved)
        return "question (or turn/phase)";
    return "board";
}


//#W58-C (D4): what a drop DID with the decision. Two exits and no third: the
//livelock breaker hands this decision to the bounded heuristic, or the answer
//is bought again. Pure.
static const char * asyncDropOutcome(bool gaveUpToHeuristic)
{
    return gaveUpToHeuristic ? "gave-up-to-heuristic" : "re-asked";
}


//#W58-C (D4): the drop's RECORD token - arm, which half of the slot key moved,
//and what the drop did. The wave-57 corpus took 108 drops (0.82 h of inference)
//and left one stderr line each, naming the arm and nothing else; the outcome
//had to be recovered by reading the next `AIPlayerGPT:` line, and a build
//without DebugTrace left no trace at all. This string is what the translog
//carries, so the price of the drops is a field rather than an archaeology pass.
//Pure.
static string asyncDropStamp(const char * arm, const string& driftKind, const char * outcome)
{
    return string(arm ? arm : "?") + "/" + (driftKind.empty() ? "unknown" : driftKind)
           + "/" + (outcome ? outcome : "?");
}


//#W58-C (D4): the stderr line for the same drop. The leading literal and the
//arm parenthetical are byte-identical to wave 57's - every existing census keys
//on them - and the outcome is appended in its own bracket. Pure.
static string asyncDropTraceLine(const char * arm, const string& driftKind, const char * outcome)
{
    return "AIPlayerGPT: dropping stale async answer (" + string(arm ? arm : "?")
           + " arm; the " + (driftKind.empty() ? "unknown" : driftKind)
           + " moved) [outcome: " + (outcome ? outcome : "?") + "]";
}


string AIPlayerGPT::asyncSlotKey(const string& userMsg)
{
    //#W82-A (L2): BOARD STATE + QUESTION, the same rule the ask and priority keys
    //now follow. The question is `mPromptTail` (the decision line and its
    //numbered option list, so a slot match means the reply's indices address
    //exactly the rows the model was shown); the board is the serialisation, whose
    //header line already carries turn and phase.
    return asyncSlotKeyOf(userMsg.compare(0, strlen(kForceCloseTag), kForceCloseTag) == 0,
                          observer->turn, observer->getCurrentGamePhase(),
                          mPromptTail, serializeGameState());
}



#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
//#W81-DI: the OFFLINE REPLY STUB, development builds only (the owner's rule: a
//diagnostic is compiled out of a release build, not merely runtime-gated). It
//exists so a SUITE fixture can drive the live ask/replay path with a known answer
//and no endpoint: `WAGIC_GPT_STUB` is a `|`-separated list of replies, one
//consumed per model call, the last repeating, with `\n` written as the two
//characters backslash-n. It is the only way to put a whole game's worth of the
//cached-replay path under the test harness's game timeout. Pure.
static string w81StubReplyNext(const string& spec, int index)
{
    std::vector<string> parts;
    size_t s = 0;
    while (true)
    {
        const size_t bar = spec.find('|', s);
        parts.push_back(spec.substr(s, bar == string::npos ? string::npos : bar - s));
        if (bar == string::npos)
            break;
        s = bar + 1;
    }
    if (parts.empty())
        return string();
    const size_t pick = (index < 0) ? 0
                      : ((size_t) index >= parts.size() ? parts.size() - 1 : (size_t) index);
    string out;
    const string& r = parts[pick];
    for (size_t i = 0; i < r.size(); i++)
    {
        if (r[i] == '\\' && i + 1 < r.size() && r[i + 1] == 'n')
        {
            out += '\n';
            i++;
        }
        else
            out += r[i];
    }
    return out;
}
#endif


int AIPlayerGPT::pollCompletion(const string& userMsg, string& content)
{
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
    //#W81-DI: the stub answers synchronously, in place of the whole async
    //transport, so a fixture game is deterministic and needs no network.
    if (const char * w81Stub = getenv("WAGIC_GPT_STUB"))
    {
        if (w81Stub[0])
        {
            content = w81StubReplyNext(w81Stub, mStubReplyIndex++);
            mTransportHandedOff = true; //#W81-DK (V9): the stub IS the transport here
            mLastLatencyMs = 0;
            mLastTimeout = false;
            mLastHttpStatus = 200;
            mLastCurlResult = 0;
            mLastReasoningOnly = false;
            mLastFinishLength = false;
            mLastBadReply = false;
            mStaleDropStreak = 0;
            mLastStaleLivelock = false;
            DebugTrace("AIPlayerGPT: STUB reply " << mStubReplyIndex << ": " << content);
            return 0;
        }
    }
#endif

    //#W57-A (D5): THE ARM'S OWN SLOT. `mPromptTail` is the question-and-options
    //tail assemblePrompt wrote for THIS ask, in this same tick, so the arm is
    //read off the question being asked - not off any latched state. A land-drop
    //ask no longer reaches into the casting arm's storage, which is where 44 of
    //the wave-56 corpus's 65 stale drops came from. Nothing about concurrency
    //changes: the launch below is still gated on asyncBusy(), which is true
    //while EITHER arm is in flight.
    const bool landArm = asyncLandArm(mPromptTail);
    std::shared_ptr<AsyncState> slot = landArm ? mAsyncLandState : mAsyncState;
    const char * armName = landArm ? "land-drop" : "casting";
    //#W57-U: before anything else, bound the in-flight exemption. A request
    //past its own deadline plus the grace with nothing published is abandoned
    //and THIS decision goes to the heuristic - the same shape, and the same
    //return, as a worker thread the platform refused.
    if (reapWedgedRequests(slot, armName))
    {
        content.clear();
        mLastTimeout = false;   //nothing came back at all: not a deadline miss
        mLastHttpStatus = 0;    //and no round trip was consumed here
        mLastCurlResult = -1;   //#W59-H (K1): no transport outcome either
        mLastLatencyMs = -1;
        return 0;               //empty reply -> the seam's heuristic answers
    }
    {
        std::lock_guard<GptMutex> g(slot->mtx);
        if (slot->status == 1)
            return kChoicePending; //one request at a time; whatever asked, wait
        if (slot->status == 2)
        {
            //#W56-A (D18): the same SLOT - same seam, turn, phase and board -
            //is the same question, however the narration header around it has
            //grown since the request went out. The answer is consumed, not
            //dropped and re-bought at a full round trip.
            bool sameSlot = false;
            if (slot->prompt != userMsg && !slot->slotKey.empty()
                && slot->slotKey == asyncSlotKey(userMsg))
            {
                sameSlot = true;
                DebugTrace("AIPlayerGPT: consuming an in-flight answer whose prompt text drifted"
                           " (same seam, turn, phase and board)");
            }
            if (slot->prompt == userMsg || sameSlot)
            {
                string body = slot->response;
                mLastLatencyMs = (long) std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - slot->started).count();
                slot->status = 0;
                slot->response.clear();
                //#W53-Q (D10): consumed with the body. A reply that CARRIED
                //text can never be a timeout, so the flag is cleared again
                //below once `content` is known.
                mLastTimeout = slot->timedOut;
                slot->timedOut = false;
                mLastHttpStatus = slot->httpStatus; //audit-L (A24)
                slot->httpStatus = 0;
                mLastCurlResult = slot->curlResult; //#W59-H (K1)
                slot->curlResult = -1;
                //#W59-H (K1): retain every failed attempt, including the
                //first attempt when the bounded retry later succeeds.
                if (body.empty() || mLastHttpStatus != 200)
                    mLastTransportOutcomes.push_back(
                        transportOutcomeStamp(mLastCurlResult, mLastHttpStatus, body.empty(),
                                              gptConnectTimeoutMs(mTimeoutMs), //#W61-U (C13)
                                              mLastLatencyMs, mTimeoutMs));
                //#W75-CJ (P2a, engine-seat HIGH-2): THE ERROR BODY IS THE
                //DIAGNOSIS. Wave 74 lost two decisions (one an attack
                //declaration) to a 400 whose message named the exact defect,
                //and the message went nowhere - not stderr, not the record.
                //Bounded, because a server may answer with a page.
                if (mLastHttpStatus != 0 && mLastHttpStatus != 200 && !body.empty())
                {
                    mLastHttpErrorBody = body.substr(0, 600);
                    const string line = "HTTP " + std::to_string(mLastHttpStatus)
                                      + " from the endpoint: " + mLastHttpErrorBody;
                    gptLogLine(line);
                    DebugTrace("AIPlayerGPT: " << line);
                }
                content.clear();
                mLastReasoningOnly = false;
                mLastFinishLength = false;
                mLastBadReply = false; //#W60-Q (R9): re-decided per consume
                //A consumed answer is forward progress: the livelock streak
                //resets here and ONLY here.
                mStaleDropStreak = 0;
                mLastStaleLivelock = false;
                //Is this the forced close (phase 2)? Its slot key carries the
                //tag, and the answer-recovery rule below is scoped by it.
                const bool forceClosePoll =
                    (userMsg.compare(0, strlen(kForceCloseTag), kForceCloseTag) == 0);
                //#W71-BO (L9, wave-70 MED-1). THE TWO RESCUE MARKERS ARE BOUND TO
                //THE DECISION THAT WAS RESCUED. They used to be set where the
                //forced close was ISSUED and cleared only where a record was
                //WRITTEN - so any decision whose phase-2 answer never became a
                //record (a stale/async drop, an abandoned retry) left them set and
                //the NEXT decision's record wore them. The corpus shows exactly
                //that: 19 records stamped `reasoning_budget_hit` +
                //`reasoning_forced_close`, 15 of them with 4.2k-15.6k reasoning
                //chars against a 6,000-TOKEN budget and a clean reply, while only 4
                //answer-locked retries (max_tokens 400) actually happened. Now they
                //are cleared on every consume and set ONLY on the phase-2 consume,
                //so `reasoning_forced_close` means a phase-2 close HAPPENED and
                //`reasoning_budget_hit` carries phase 1's own finish_reason.
                if (!forceClosePoll)
                {
                    mLastForcedClose = false;
                    mLastBudgetHit = false;
                }
                string fieldReasoning;
                if (!body.empty())
                {
                    try
                    {
                        json reply = json::parse(body);
                        json choice0 = reply["choices"][0];
                        content = choice0["message"]["content"].is_string()
                                  ? choice0["message"]["content"].get<string>() : string();
                        //PATH 1: the server ran a reasoning parser - the
                        //thinking arrives in its own field and `content` comes
                        //back ALREADY stripped (no inline <think> reaches the
                        //client at all). The field NAME is not settled across
                        //builds: the OpenAI-compatible spelling is
                        //reasoning_content, and Spark's vLLM 0.23.1rc1 answers
                        //with plain `reasoning` (live-probed 2026-08-19). Read
                        //both, in that order, rather than betting on one - the
                        //cost of guessing wrong is a corpus with an empty
                        //reasoning column and a blind seat review.
                        if (choice0["message"].contains("reasoning_content")
                            && choice0["message"]["reasoning_content"].is_string())
                            fieldReasoning = choice0["message"]["reasoning_content"].get<string>();
                        else if (choice0["message"].contains("reasoning")
                                 && choice0["message"]["reasoning"].is_string())
                            fieldReasoning = choice0["message"]["reasoning"].get<string>();
                        //Decode stopped at the cap: with a thinking budget in
                        //force this is the expected phase-1 exit, and the
                        //unclosed-think test below decides whether the answer
                        //still has to be forced out of the model.
                        //The server's OWN reasoning-token count when it
                        //reports one (vLLM/OpenAI shape:
                        //usage.completion_tokens_details.reasoning_tokens).
                        //Preferred over the client's char count because the
                        //budget is denominated in TOKENS - the first corpus
                        //under this budget is a CALIBRATION run and the next
                        //budget is read straight off this distribution.
                        if (reply.contains("usage")
                            && reply["usage"].contains("completion_tokens_details")
                            && reply["usage"]["completion_tokens_details"].contains("reasoning_tokens")
                            && reply["usage"]["completion_tokens_details"]["reasoning_tokens"].is_number())
                            mLastReasoningTokens =
                                reply["usage"]["completion_tokens_details"]["reasoning_tokens"].get<long>();
                        mLastFinishLength = (choice0.contains("finish_reason")
                                             && choice0["finish_reason"].is_string()
                                             && choice0["finish_reason"].get<string>() == "length");
                    }
                    catch (json::exception&)
                    {
                        //#W60-Q (R9): the endpoint sent SOMETHING and it is not
                        //an answer. Recorded as its own class rather than
                        //cleared into the empty-reply bucket.
                        content.clear();
                        mLastBadReply = true;
                    }
                    //A 200 that parses but carries no answer shape at all
                    //(`{"choices":[]}`, an error envelope) is the same fault.
                    if (content.empty() && fieldReasoning.empty())
                        mLastBadReply = true;
                }
                //PATH 2 (unconditional, whatever path 1 found): an inline
                //"<think> ... </think>" the server did not parse out. Strip it
                //into the same field so every scanner downstream measures REPLY
                //text. An unclosed block empties `content` - reasoning-only, and
                //the caller forces the answer rather than parsing think text.
                {
                    string inlineReasoning;
                    if (!splitReasoningBlock(content, inlineReasoning))
                        mLastReasoningOnly = true;
                    if (!inlineReasoning.empty())
                        fieldReasoning = inlineReasoning;
                }
                //ROUTING. With a parser active there is no inline block to
                //test, so "the reply is reasoning-only" has a second, and on
                //this stack the NORMAL, shape: content empty + reasoning
                //non-empty. That is what a phase-1 budget hit looks like
                //(finish_reason == "length"), and it is also what phase 2 looks
                //like - except phase 2's reasoning IS the answer.
                if (content.empty() && !fieldReasoning.empty())
                {
                    string tail = answerTailFromReasoning(fieldReasoning);
                    //#W71-BO (R7): the `reasoning_tail_answers` COUNTER is
                    //DELETED (0 of 40). #W82-A (audit-2026-09): the branch
                    //itself is NOT deleted - the tail is still taken below, and
                    //the comment read as though it were.
                    if (forceClosePoll)
                    {
                        //Phase 2: the generation is the answer, wherever the
                        //parser filed it. Keep phase 1's thinking as the
                        //audit record rather than overwriting it with the
                        //answer text.
                        content = tail.empty() ? fieldReasoning : tail;
                        fieldReasoning.clear();
                    }
                    else if (!mLastFinishLength && !tail.empty())
                    {
                        //Phase 1, ran to a natural stop, and its thinking ENDS
                        //on a coded answer line: the model answered inside the
                        //thinking window (no separate content). Take that line,
                        //keep the thinking. A cap-truncated reply is refused
                        //here - mid-thought candidates are not answers.
                        content = tail;
                    }
                    else
                    {
                        //Genuinely no answer yet: reasoning-only. The forced
                        //close is the recovery, not the heuristic.
                        mLastReasoningOnly = true;
                    }
                }
                //#W71-BO (L9): the phase-2 census, on the path that RUNS. It used
                //to sit inside the `content.empty() && reasoning non-empty` branch,
                //which a NORMAL phase-2 recovery (content present) never enters -
                //so `phase2_answer_missing`/`_recovered` read 0/0 across 40 games
                //while 4 answer-locked retries demonstrably happened. Every phase-2
                //consume is counted here, exactly once, whatever shape it came
                //back in; the marker pair is stamped on the same tick.
                if (forceClosePoll)
                {
                    //#W79-DD: the census must agree with the PARSER, or the
                    //instrument reports a recovered answer as missing. A phase-2
                    //consume returns `content`, and `content` goes to the seams'
                    //`consumePlan` like any other reply - which now also reads an
                    //unambiguous label-less action line. So the same two shapes
                    //count here. (`answerTailFromReasoning` above stays
                    //label-anchored on purpose: it selects a tail out of a
                    //REASONING trace, which is full of numbered parentheticals,
                    //and that is the genuine ambiguity the ruling still excludes.)
                    size_t w79bs = 0, w79be = 0;
                    const bool w79Bare =
                        w79LabellessAnswerLine(content, firstLineLeadingPlanPos(content),
                                               hasCodedAnswerLine(content), &w79bs, &w79be);
                    if (hasCodedAnswerLine(content) || w79Bare)
                        mPhase2AnswerRecovered++;
                    else
                        mPhase2AnswerMissing++;
                    mLastForcedClose = true;
                    mLastBudgetHit = mForceClosePhase1Length;
                    mForceClosePhase1Length = false;
                }
                if (!fieldReasoning.empty())
                {
                    mLastReasoning = fieldReasoning;
                    //Measured HERE, on the trace as it arrived, because the
                    //record path consumes (and clears) mLastReasoning.
                    mLastReasoningDegenerate = reasoningRepetitionRatio(fieldReasoning);
                    //#W75-CJ (P20): the shingled companion, measured on the same
                    //trace and reported beside it.
                    mLastReasoningNgramRepeat = reasoningNgramRepeatRatio(fieldReasoning);
                }
                //Reasoning was asked for and the provider kept it. Recorded,
                //never treated as a failure: the answer is right there in
                //content and is parsed exactly as the thinking-off path parses
                //it. Latency still carries the invisible decode cost (the
                //translog's own latency_ms), so a record with
                //reasoning_hidden + a long round trip is a WITHHELD trace, not
                //a defect class - that pair is precisely how this shape is told
                //apart from a fast non-thinking reply.
                mLastReasoningHidden = reasoningHiddenShape(reasoningRequested(), content, mLastReasoning);
                //Subscription preset: the backend prices every reply against a
                //rolling plan window and reports the gauge on each response.
                //Surface it when it moves - a player burning their ChatGPT plan
                //on a card game deserves to see the meter, and a static gauge
                //re-noticed every decision would be wallpaper.
                //#W53-Q (D10): anything the server actually sent - even an
                //unparseable body or a reasoning-only reply - is not a
                //deadline miss. Only a genuinely empty round trip at the wall
                //keeps the stamp.
                if (!body.empty())
                    mLastTimeout = false;
                GPTASYNCLOG("gpt consume body=%zu content=%zu latency=%ldms\n",
                            body.size(), content.size(), mLastLatencyMs);
                if (gptCodexEndpoint(mEndpoint))
                {
                    string pct = gptCodexUsedPercent();
                    if (!pct.empty() && pct != mCodexPctNotified)
                    {
                        mCodexPctNotified = pct;
                        setNotice("ChatGPT plan: " + pct + "% of the weekly limit used", 6.0f);
                    }
                }
                return 0;
            }
            //An answer for a prompt the game state has moved past (should
            //not happen while the AI neither acts nor passes; drop safely).
            //(history: comment-archaeology.md AIPlayerGPTTransport-asyncSlotDriftKind-1684)
            //#W58-C (D4): the drift kind is computed ONCE, here, and the line
            //is written after the drop's own decision is known - so one line
            //carries both halves and no census has to pair it with the next
            //`AIPlayerGPT:` line of the stderr. The leading literal and the
            //arm parenthetical are byte-identical to wave 57's: every existing
            //count keys on them.
            const string driftKind = asyncSlotDriftKind(slot->slotKey, asyncSlotKey(userMsg));
            //#W55-E (D5b): WHERE the prompt moved. A stale drop says only that
            //the rebuilt prompt differs from the one in flight; the wave-54
            //reveal livelock (146v123 s15) then needed an archaeology pass over
            //two 40 KB strings to find out which section was unstable. This
            //names the first divergence and its neighbourhood. Development
            //builds only (owner rule: diagnostics are compiled out of release);
            //WAGIC_GPT_DRIFT=1 arms it, so a dev build is silent by default.
            GPT_DRIFT_TRACE(slot->prompt, userMsg);
            GPTASYNCLOG("gpt stale drop (prompt moved) resp=%zu\n", slot->response.size());
            slot->status = 0;
            slot->response.clear();
            //LIVELOCK BREAKER (146v36, 2026-08-21): a RUN of drops with no
            //consume between them means the rebuilt prompt is not stable for
            //an unchanged state - left alone this loops at one model round
            //trip per cycle for hours (341 drops / 4.6h on the Kaya menu
            //before the deterministic-order fix). Give this ONE decision to
            //the bounded heuristic fallback instead; the game moves on and
            //the translog records the give-up as its own class.
            //(history: comment-archaeology.md AIPlayerGPTTransport-L1694-1696)
            {
                string board = serializeGameState();
                if (board != mStaleDropBoard)
                {
                    mStaleDropBoard = board;
                    mStaleDropStreak = 0;
                }
            }
            //#W58-C (D4): the drop's OUTCOME - what this drop did with the
            //decision, decided here and stated on the drop's own line. Two
            //exits exist and both are named: the breaker hands THIS decision
            //to the heuristic, or the answer is bought again from the model.
            //Wave 57 could tell them apart only by reading the following
            //stderr lines, and 108 drops of it were 0.82 h of inference with
            //no per-drop record of where any of it went.
            const char * dropOutcome =
                asyncDropOutcome(mStaleDropStreak + 1 >= kStaleLivelockLimit);
            DebugTrace(asyncDropTraceLine(armName, driftKind, dropOutcome));
            //The RECORD half, and it is not diagnostics: one token per drop on
            //the next translog record this seat writes, so the corpus can price
            //the drops without a stderr at all.
            mAsyncDropStamps.push_back(asyncDropStamp(armName, driftKind, dropOutcome));
            mAsyncDropStampsSeq = mWindowSeq; //#W82-A (L10): the window this drop is about
            mAsyncDropsGame++; //#W69-BI (K7): counted at the drop, not at the flush
            if (++mStaleDropStreak >= kStaleLivelockLimit)
            {
                DebugTrace("AIPlayerGPT: " << mStaleDropStreak
                           << " consecutive stale drops - giving this decision to the heuristic");
                mStaleDropStreak = 0;
                mLastStaleLivelock = true;
                mLastTimeout = false; //#W53-Q (D10): a give-up, not a deadline
                mLastHttpStatus = 0;  //audit-L (A24): no round trip was consumed here
                content.clear();
                return 0; //no answer: the caller's parse fails -> Baka fallback
            }
        }
    }

    //#W57-A (D5): ONE round trip at a time, across BOTH arms. The other arm may
    //be holding a done-but-unconsumed answer (that is the whole point of the
    //second slot) but it must never be holding a LIVE request while this arm
    //opens another: the second slot buys storage, never a second spend. The
    //caller unwinds and re-polls, exactly as it does for its own arm.
    if (asyncBusy())
        return kChoicePending;

    //Idle: build the request on the game thread (the prompt members are not
    //shared with the worker) and launch the round trip in the background.
    string requestBody = buildRequestBody(userMsg);
    const string slotAtLaunch = asyncSlotKey(userMsg); //#W56-A (D18)
    bool codex = gptCodexEndpoint(mEndpoint);
    string url = codex ? mEndpoint + "/responses" : mEndpoint + "/v1/chat/completions";
    string key = mApiKey;
    std::shared_ptr<AsyncState> state = slot; //#W57-A (D5): this arm's slot
    unsigned int genAtLaunch = 0; //#W57-U
    {
        std::lock_guard<GptMutex> g(state->mtx);
        state->status = 1;
        state->prompt = userMsg;
        state->slotKey = slotAtLaunch; //#W56-A (D18)
        state->response.clear();
        state->timedOut = false; //#W53-Q (D10): the worker decides this one
        state->httpStatus = 0;   //audit-L (A24): likewise
        state->curlResult = -1;  //#W59-H (K1): likewise
        state->started = std::chrono::steady_clock::now();
        genAtLaunch = state->gen; //#W57-U
    }
    GPTASYNCLOG("gpt spawn ask prompt=%zu endpoint=%s\n", userMsg.size(), mEndpoint.c_str());
    long timeoutMs = mTimeoutMs;
    //#W59-H (K1): a TRANSPORT retry spends the decision deadline's remainder
    //(mRetryBudgetMs, computed and bounded where the retry was armed), never a
    //fresh full deadline. A wall-miss retry leaves the budget 0 and keeps the
    //full deadline it has always had. Never 0 here: 0 is libcurl's "no
    //timeout at all", which is how a transport stall becomes a hung seat.
    if (userMsg.compare(0, strlen(kTimeoutRetryTag), kTimeoutRetryTag) == 0
        && mRetryBudgetMs > 0)
        timeoutMs = mRetryBudgetMs;
    //The worker runs through gptSpawnWorker - the platform threading seam.
    //On Vita that is a native sceKernelCreateThread (std::thread construction
    //throws there - no active gthreads layer); elsewhere it is a detached
    //std::thread. WAGIC_GPT_NOTHREAD (checked inside the seam) emulates a
    //refusing platform on desktop, where the refusal path is otherwise
    //unreachable and therefore untestable exactly where it is easiest to test.
    WorkerCtx * ctx = NEW WorkerCtx();
    ctx->state = state;
    ctx->url = url;
    ctx->requestBody = requestBody;
    ctx->key = key;
    ctx->timeoutMs = timeoutMs;
    ctx->codex = codex;
    ctx->gen = genAtLaunch; //#W57-U
    if (!gptSpawnWorker(WorkerMain, ctx))
    {
        delete ctx;
        //The platform refused a thread - resource limits, a thread cap, or an
        //inactive threading runtime. The guarantee every other seam here keeps
        //must hold: a transport failure degrades to the heuristic AI, it never
        //takes the game down. Publishing an empty reply is the same shape as an
        //unreachable endpoint, so the caller falls back to Baka on its existing
        //path. Resolve THIS tick rather than reporting a round trip that does
        //not exist. Returning kChoicePending here means "no action yet", and
        //the only thing that stops an empty clickstream being committed as a
        //pass is decisionPending(), which is true only while asyncBusy() - i.e.
        //while status == 1. Publishing status = 2 synchronously and then
        //claiming pending left the seam waiting and the gate reporting idle, so
        //the engine passed the turn. Every decision became attempt-once,
        //get-passed: on hardware that is an opponent that draws and passes
        //forever, and in the harness 107 turns with both players still at 20
        //life. Going straight back to idle with an empty reply is the same
        //shape as a synchronous transport failure, which the seams already
        //answer with the heuristic AI, in this tick.
        {
            std::lock_guard<GptMutex> g(state->mtx);
            state->status = 0;
            state->response.clear();
            state->prompt.clear();
            state->slotKey.clear(); //#W56-A (D18)
            state->timedOut = false;
        }
        content.clear();
        mLastTimeout = false; //#W53-Q (D10): a refused thread, not a deadline
        mLastHttpStatus = 0;  //audit-L (A24)
        mLastCurlResult = -1; //#W59-H (K1): no curl attempt existed
        DebugTrace("AIPlayerGPT: could not start the worker thread; falling back to the heuristic AI");
        //Log this ONCE. A platform that refuses one thread refuses all of them,
        //so the message is identical every time and repeats once per decision -
        //209 and 344 identical lines in two Vita sessions. Writing it each time
        //puts a file open/write/close on the game thread on the single platform
        //where storage I/O per game event is a known cause of lag, to say
        //something already known after the first line.
        static bool refusalLogged = false;
        if (!refusalLogged)
        {
            refusalLogged = true;
            gptLogLine(string("worker thread refused")
                       + " - falling back to the heuristic AI for every decision"
                       + " (logged once; this platform cannot start threads)");
        }
        return 0; //answer now, with an empty reply -> the seam's heuristic
    }
    //#W81-DK (V9): THE SEND BOUNDARY. The worker holds the prompt and will put it on
    //the wire - this line, and the stub above, are the only two places in this seat
    //where a prompt is handed over. Every verdict count and every record face is
    //gated on this flag from here on, instead of on a caller's literal `true`
    //written one statement before the call (`crackback_verdict_lines_rendered` 382
    //vs 329 prompts vs 291 records, wave-80 engine-seat HIGH-4).
    mTransportHandedOff = true;
    return kChoicePending;
}


//A line-leading coded answer label (CHOICE:/ATTACK:/BLOCKS:/PUT:) with SOME
//payload after it. Used only by the garbage detector: if the model emitted any
//such line, the normal parse + salvage can work it and we must NOT retry.
//ONE scanner for every line-leading coded answer (CHOICE:/ATTACK:/BLOCKS:/
//PUT:) in a reply, so the garbage detector, the retraction flag and the
//wave-34 protocol instruments all agree on what counts as an answer line.
//Returns how many there are; optionally hands back where the FIRST one's line
//ends (the post-answer meter's origin) and the normalised payload of the first
//and last (whether the reply ended on the answer it began with).
//#W68-BA (J3): `firstLabelStart` is the offset of the FIRST line-leading
//coded label, so the two-sided overrun meter measures the pre-answer side
//against the same offset every seam parses from (heading skip included).
int scanCodedAnswerLines(const string& content, size_t * firstLineEnd,
                                string * firstPayload, string * lastPayload,
                                size_t * firstLabelStart)
{
    static const char * kLabels[] = { "choice:", "attack:", "blocks:", "put:" };
    string low = content;
    for (size_t i = 0; i < low.size(); i++)
        low[i] = (char) tolower((unsigned char) low[i]);
    int count = 0;
    size_t lineStart = 0;
    while (lineStart <= low.size())
    {
        size_t lineEnd = low.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? low.size() : lineEnd;
        size_t s = lineStart;
        //#W70-BM (E2): whitespace only. The markdown-decoration skip
        //(`* # - > \``) was a tolerance for a reply that decorates its answer
        //line; the protocol permits two labelled lines and nothing else, so a
        //bulleted label is a violation to be counted, not a shape to accept.
        while (s < end && (low[s] == ' ' || low[s] == '\t'))
            s++;
        for (size_t k = 0; k < sizeof(kLabels) / sizeof(kLabels[0]); k++)
        {
            size_t len = strlen(kLabels[k]);
            if (end - s >= len && low.compare(s, len, kLabels[k]) == 0)
            {
                //require a non-space payload char after the label
                size_t p = s + len;
                while (p < end && (low[p] == ' ' || low[p] == '\t')) p++;
                if (p >= end)
                    break;
                count++;
                //Normalised payload: the label plus the rest of the line, one
                //space between tokens, so "CHOICE: 2 (Yotian Soldier)" and
                //"CHOICE:  2  (Yotian Soldier)" are the SAME answer and
                //"CHOICE: 1 (Akroma's Memorial)" is a different one.
                string pay;
                bool sp = false;
                for (size_t q = s; q < end; q++)
                {
                    char c = low[q];
                    if (c == ' ' || c == '\t' || c == '\r')
                        sp = !pay.empty();
                    else
                    {
                        if (sp) pay += ' ';
                        sp = false;
                        pay += c;
                    }
                }
                if (count == 1)
                {
                    if (firstLineEnd) *firstLineEnd = end;
                    if (firstPayload) *firstPayload = pay;
                    if (firstLabelStart) *firstLabelStart = s; //#W68-BA (J3)
                }
                if (lastPayload) *lastPayload = pay;
                break;
            }
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    return count;
}

bool hasCodedAnswerLine(const string& content)
{
    return scanCodedAnswerLines(content, NULL, NULL, NULL) > 0;
}



//#W50-Y D19 (wave-49 ledger LOW, R20 promoted): the SHORT garbage shapes.
//The wave-23 detector's 800-char floor was sized for the 6-10k-char decode
//spirals; both wave-49 garbage decodes were under it - deck146 vs125 seq 82
//(286 chars: punctuation soup, `::` / `«` / U+FFFD, two English words) and
//deck125 vs130 seq 101 (409 chars: a CJK/English token spray) - so neither
//was retried and both reached Baka as `unparsed_reply`. Below the floor the
//test is TIGHTER, on three independent signals over a reply with NO coded
//answer line: (a) non-ASCII bytes >= 20% of the text (a CJK / mojibake
//spray; an accented card name in English prose is ~2%); (b) letters under
//25% of the text (punctuation soup; English prose runs 70-80%); (c) a token
//repeated: the most frequent whitespace-delimited token of >= 2 chars holds
//>= 30% of the tokens across >= 6 occurrences (the literal decode loop).
//Negatives pinned in PARSETEST: short English prose with no coded line, a
//prose reply naming an accented card, a curly-quoted one-liner (under the
//40-char floor), a coded line amid junk (never garbage).
static bool shortDecodeGarbage(const string& content)
{
    const size_t kFloor = 40;
    size_t total = content.size();
    if (total < kFloor)
        return false;
    size_t letters = 0, nonAscii = 0;
    for (size_t i = 0; i < total; i++)
    {
        unsigned char c = (unsigned char) content[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            letters++;
        else if (c >= 0x80)
            nonAscii++;
    }
    bool nonAsciiHeavy = (nonAscii * 100 >= total * 20);
    bool lowProse = (letters * 100 < total * 25);
    bool repetition = false;
    {
        std::map<string, int> freq;
        int tokens = 0, top = 0;
        size_t i = 0;
        while (i < total)
        {
            while (i < total && isspace((unsigned char) content[i])) i++;
            size_t j = i;
            while (j < total && !isspace((unsigned char) content[j])) j++;
            if (j - i >= 2)
            {
                int c = ++freq[content.substr(i, j - i)];
                if (c > top) top = c;
                tokens++;
            }
            i = j;
        }
        repetition = (top >= 6 && top * 10 >= tokens * 3);
    }
    return nonAsciiHeavy || lowProse || repetition;
}


const char * AIPlayerGPT::unparsedReplyClass(const string& content)
{
    return isDecodeGarbage(content) ? "degenerate_decode" : "unparsed_reply";
}


bool AIPlayerGPT::isDecodeGarbage(const string& content)
{
    //A well-formed coded answer line means the parser/salvage can act on it -
    //not a collapse; never retry (this is what keeps ordinary unparsed replies
    //with a real answer line out of the retry path).
    if (hasCodedAnswerLine(content))
        return false;
    //Conservative floor: a decode collapse is always long (the 80-120s spirals
    //were 6.4-10.8k chars). #W50-Y D19: below it the SHORT shapes are judged
    //by their own, tighter test.
    const size_t kMinLen = 800;
    if (content.size() < kMinLen)
        return shortDecodeGarbage(content);

    size_t total = content.size();
    size_t letters = 0, junk = 0;
    for (size_t i = 0; i < total; i++)
    {
        unsigned char c = (unsigned char) content[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            letters++;
        else if (c >= 0x80 || c == '*' || c == '#' || c == '`')
            junk++; //non-ASCII (incl. U+FFFD collapse bytes) + markup symbols
    }
    //Signal 1: near-zero prose. A real reply - even a truncated one - is mostly
    //English; a token collapse is punctuation/markup/garbage. <5% letters over a
    //long reply cannot be a genuine unparsed answer (deck27 s12/s13/s14 ~0%).
    bool lowProse = (letters * 100 < total * 5);
    //Signal 2: markup/non-ASCII density (s13's U+FFFD **, s14's ``` ### fences).
    bool markupHeavy = (junk * 100 >= total * 30);
    //Signal 3: a short line repeated many times (the literal decode loop). Find
    //the most frequent non-empty trimmed line; a collapse repeats ONE tiny line.
    bool repetition = false;
    {
        std::map<string, int> freq;
        size_t lineStart = 0, nonEmpty = 0, top = 0;
        while (lineStart <= content.size())
        {
            size_t lineEnd = content.find('\n', lineStart);
            size_t end = (lineEnd == string::npos) ? content.size() : lineEnd;
            size_t a = lineStart, b = end;
            while (a < b && (unsigned char) content[a] <= ' ') a++;
            while (b > a && (unsigned char) content[b - 1] <= ' ') b--;
            if (b > a && (b - a) <= 12) //only short lines feed the loop signature
            {
                string ln = content.substr(a, b - a);
                int c = ++freq[ln];
                if ((size_t) c > top) top = c;
            }
            if (b > a) nonEmpty++;
            if (lineEnd == string::npos)
                break;
            lineStart = lineEnd + 1;
        }
        repetition = (top >= 20 && top * 10 >= nonEmpty * 3); //>=20x and >=30% of lines
    }
    return lowProse || markupHeavy || repetition;
}


//#W55-E (D23). A WALL MISS - the deadline reached with an empty reply - is an
//event a seat review counts, and wave 54 had one that produced no record at all:
//`146v123` missed the wall, the one retry was launched, and before it could be
//consumed the window auto-passed as display-toggle-only, so the decision was
//abandoned and NOTHING wrote it down. Two `no reply after 900s` events, one
//record. Pure: a wall-missed ask is abandoned once the seat is polling a
//DIFFERENT decision than the one that missed.
static bool wallMissAbandoned(bool pending, const string& missedBase, const string& nowBase)
{
    return pending && !missedBase.empty() && nowBase != missedBase;
}


//#W62-Z (D18, deck130 MED-1). `130v126` s12 carried
//`transport: curl=28,http=0,empty=1,connect_ms=20000,phase=wall`,
//`deadline_pct 100.0` and `latency_ms 900024` - a wall miss with its phase
//PROVEN by the #W61-U C13 stamp - and was still classed `wall_miss_unrecorded`,
//the class that means "an abandoned ask nothing else wrote down". One name for
//two different states, so a corpus cannot separate a phase-proven abandonment
//from an unexplained one without re-deriving the arithmetic by hand (which is
//what the wave-60 adjudication had to do, and what C13 shipped the stamp to
//stop). The class now carries the phase the stamp proved, as a SUFFIX: every
//existing `wall_miss_unrecorded` grep still matches, nothing is renamed away,
//and a record with no proven phase keeps the old string byte for byte.
//NOTE the correction the deck-130 seat's reading needs: `connect_ms=20000` is
//the connect BUDGET the attempt was given (#W60-M B4), not evidence the connect
//failed - only the elapsed time separates a connect death from a wall miss, and
//at 100% of the deadline this is a wall miss. `phase=` is that verdict, and the
//class now repeats it rather than contradicting it.
//Pure over the stamp text, so both halves are PARSETEST-provable.
string transportStampPhase(const string& transport)
{
    static const char * kKey = "phase=";
    size_t at = transport.rfind(kKey);
    if (at == string::npos)
        return "";
    at += strlen(kKey);
    size_t e = at;
    while (e < transport.size() && (isalnum((unsigned char) transport[e]) || transport[e] == '_'))
        e++;
    return transport.substr(at, e - at);
}

string wallMissClassFor(const string& phase)
{
    if (phase.empty())
        return "wall_miss_unrecorded";
    return "wall_miss_unrecorded_" + phase;
}


//The abandoned ask gets its own zero-choice record, carrying the prompt the
//model never answered. Additive: no window is removed, no retry is spent, and
//nothing in the engine reads it.
void AIPlayerGPT::flushWallMissRecord(const char * classOverride)
{
    if (!mWallMissPending)
        return;
    string base = mWallMissBase;
    mWallMissPending = false;
    mWallMissBase.clear();
    //#W68-BC (J2): a miss written down BECAUSE no retry was bought is not an
    //"unrecorded" (abandoned) miss - it has its own counter and its own class.
    //#W82-A R1 (LEDGER v2, Astra genuine): these three counters were deleted
    //under a "no consumer / per-record join" rule that Astra REFUTES -
    //`corpus-stats.py:465` iterates EVERY integer gameend key, so every counter
    //is consumed, and a zero is an observation rather than dead code. Restored
    //byte-for-byte. Deleting a MECHANISM and deleting a consumed COUNTER are
    //separate decisions; the wall-miss mechanism was never deleted.
    if (classOverride)
        mWallMissNoRetry++;
    else
        mWallMissUnrecorded++;
    //#W61-U (C13): the abandoned ask's OWN round trip, restored onto the record
    //that reports it. Without it the record read `latency_ms: -1` (a cache hit)
    //for a decision that had just spent the entire deadline, and carried no
    //`deadline_pct` at all - so nothing in the log distinguished the two
    //wave-60 wall misses from a connect failure. Consumed with the record, like
    //every other latency.
    if (mWallMissLatencyMs >= 0 && mLastLatencyMs < 0)
        mLastLatencyMs = mWallMissLatencyMs;
    mWallMissLatencyMs = -1;
    //#W62-Z (D18): the phase this attempt's own transport stamp proved, read
    //before writeTransLog consumes mLastTransportOutcomes.
    string phase;
    for (size_t i = 0; i < mLastTransportOutcomes.size() && phase.empty(); i++)
        phase = transportStampPhase(mLastTransportOutcomes[i]);
    const string wmClass = classOverride ? string(classOverride) : wallMissClassFor(phase); //#W68-BC (J2)
    writeTransLog("wall_miss", base, "", -1, 0, "", wmClass.c_str(), NULL);
}

void AIPlayerGPT::setAnswerFloorForSeam(const char * seam, long items, long bytesPerItem)
{
    mAnswerFloorSeam = (seam && *seam) ? seam : "";
    mAnswerFloorTokens = gptDeclarationAnswerFloorTokens(items, bytesPerItem);
}


//#W71-BS (F5): is this a seam whose answer is a DECLARATION - a list whose length
//is set by the board, where a cut silently drops legal choices rather than losing
//one coded line? Those are the seams that may buy the one raised-ceiling re-ask.
static bool gptSeamIsADeclaration(const string& seam)
{
    return seam == "blockers" || seam == "attackers" || seam == "discard"
           || seam == "reveal" || seam == "bottom";
}


//#W74-CF (F1, Astra review finding 1). PER-ARM RETRY STORAGE.
//`retryArmMatches` (#W74-CD O2) stopped the wrong arm CONSUMING or ABANDONING a
//leg, but the three arming branches below still wrote the single shared record
//whoever was running: land's primary reply arms a forced close; casting then
//launches and finishes its own primary and arms ANOTHER forced close, which
//overwrites land's base, prompt and prefill while land's second leg is still
//outstanding. Land's next poll then sends its ORDINARY prompt - a different slot
//key from a forced-close request - so the completed leg is discarded and the
//decision falls to the heuristic. Nothing about the ownership boolean, the
//delivery-ratio experiment or the two-second pass hold prevents that.
//The slot is now per arm. This function is the ONLY mutator: it leaves `live`
//holding this arm's leg (or empty) and the other arm's leg parked, untouched.
void gptRetrySelectArm(GptRetrySlot& live, GptRetrySlot& park, bool landArm)
{
    if (!live.activePrompt.empty() && live.armLand == landArm)
        return; //ours is already live
    if (!park.activePrompt.empty() && park.armLand == landArm)
    {
        std::swap(live, park); //ours is parked: bring it back, park theirs
        return;
    }
    if (!live.activePrompt.empty())
        std::swap(live, park); //theirs is live and ours is empty: park theirs
}


//#W82-A (audit-2026-09): `gptRetrySelectArmClose` is FOLDED AWAY. It existed
//only to thread one bool in and out of `gptRetrySelectArm`, and that bool is
//already a field of the slot the swap moves (`GptRetrySlot::forceCloseArmed`) -
//the caller does the two assignments itself now, in the one place they happen.
int gptForceCloseArm(bool& liveArmed)
{
    const int inc = liveArmed ? 1 : 0;
    liveArmed = true;
    return inc;
}

int gptForceCloseOutstanding(bool liveArmed, bool parkArmed)
{
    return (liveArmed ? 1 : 0) + (parkArmed ? 1 : 0);
}


//#W76-CN (Q8, wave-75 engine-seat MED-2). A CLOSE THAT CANNOT BE THE ONE THAT
//IS CONSUMED IS NOT WORTH A DECODE. Nine forced closes were armed, superseded
//and paid a second ~20 KB decode whose answer was discarded because the
//question had moved (`125v123` casting x2, `125v130` casting x2, `162v123`
//land x1, `152v123` casting x1, `162v130` casting x3 - about 12 minutes of wall
//clock). No decision was lost on any of them: every one is followed by
//`dropping stale async answer ... [outcome: re-asked]` or the seat's own
//re-serve, and the corpus carries 0 fallbacks. The bound is the one the seat can
//know at ARM time: the OTHER arm already has an outstanding close parked, so
//this seat owes two rescues at two different questions and at most one of them
//can still be the live one. The live arm's own outstanding close is NOT a
//refusal - it is this window superseding its predecessor, which is counted as
//unrecorded exactly as before. Pure over the two flags, so the interleaving is
//pinned.
bool w76ForceCloseArmAllowed(bool parkArmed)
{
    return !parkArmed;
}


//#W76-CQ (F2, Astra wave-76 finding 2). A REFUSED ARM IS NOT AN ANSWERED
//WINDOW. The Q8 bound above was correct about the DECODE and wrong about the
//DECISION: the refusal incremented its counter and fell through, and a 200 that
//carried native reasoning and no answer is not a transport failure, not a
//timeout, not a declaration payload and not decode garbage - so no other retry
//arm could fire, `pollCompletionRetry` returned success with empty content, and
//the caller's HEURISTIC played the window. The claimed "the window unwinds and
//is re-asked" never happened on this path.
//
//So the refusal now DEFERS: the slot is built exactly as the arming branch
//builds it (base, prefill, phase-1 finish reason) and the decision returns
//kChoicePending WITHOUT buying a decode. Each later tick re-asks this one
//question: has the other arm's close cleared? The moment it has, the close is
//armed and phase 2 is bought - which is the whole point of the Q8 bound, the
//decode deferred rather than lost.
//
//A PERMANENTLY PENDING DECISION IS WORSE THAN A LOST WINDOW, so the wait is
//BOUNDED: at kW76CloseDeferMaxTicks the close is armed anyway (the other arm
//may be parked behind a question this seat will not return to for a long time),
//and the bound hit is counted. Pure over the three facts the caller has at the
//decision point, so PARSETEST replays the caller's whole tick loop - the
//refusal, the wait, the clear, and the bound - without a game.
const int kW76CloseDeferMaxTicks = 8;


//#W78-CY (F11, Astra MED 11): ONE BUSY PREDICATE FOR BOTH SITES. The DEFERRAL site
//computes `!armAllowed(park) || thisArmArmed`; the CONTINUATION that decides whether
//the deferred close may arm now passed `mRetryPark.forceCloseArmed` ALONE - so with a
//deferred close, THIS arm still armed and an empty park, the continuation armed on the
//very next tick, superseded the outstanding close (`mForceCloseUnrecorded++`,
//`armed_after_defer_superseding`) and the same-arm bound the deferral had just taken
//never held for a single tick. The two sites now ask the same question of the same two
//flags. Pure, so PARSETEST walks the two-tick sequence.
static bool w78ForceCloseBusy(bool parkArmed, bool thisArmArmed)
{
    return !w76ForceCloseArmAllowed(parkArmed) || thisArmArmed;
}


static bool w78ForceCloseSameArmBusy(bool parkArmed, bool thisArmArmed)
{
    return thisArmArmed && w76ForceCloseArmAllowed(parkArmed);
}


int w76ForceCloseDecision(bool earned, bool parkArmed, int deferTicks)
{
    if (!earned)
        return kW76CloseFallThrough;
    if (!parkArmed)
        return kW76CloseArm;
    if (deferTicks >= kW76CloseDeferMaxTicks)
        return kW76CloseArm; //the deadlock guard: never a permanent pending
    return kW76CloseDefer;
}


void AIPlayerGPT::selectRetryArm(bool landArm)
{
    GptRetrySlot live;
    live.activePrompt = mRetryActivePrompt;
    live.base = mRetryBase;
    live.doneBase = mRetryDoneBase;
    live.ceilingDoneBase = mCeilingReaskDoneBase;
    live.forceClosePrefill = mForceClosePrefill;
    live.firstLatencyMs = mRetryFirstLatencyMs;
    live.budgetMs = mRetryBudgetMs;
    live.armLand = mRetryArmLand;
    live.phase1Length = mForceClosePhase1Length;
    live.forceCloseDeferred = mForceCloseDeferred;       //#W76-CQ (F2)
    live.forceCloseDeferTicks = mForceCloseDeferTicks;
    //#W75-CM (F5): the flag rides the slot, so nothing else has to remember it.
    live.forceCloseArmed = mForceCloseArmed; //#W82-A: was gptRetrySelectArmClose
    gptRetrySelectArm(live, mRetryPark, landArm);
    mForceCloseArmed = live.forceCloseArmed;
    mForceCloseDeferred = live.forceCloseDeferred;       //#W76-CQ (F2)
    mForceCloseDeferTicks = live.forceCloseDeferTicks;
    mRetryActivePrompt = live.activePrompt;
    mRetryBase = live.base;
    mRetryDoneBase = live.doneBase;
    mCeilingReaskDoneBase = live.ceilingDoneBase;
    mForceClosePrefill = live.forceClosePrefill;
    mRetryFirstLatencyMs = live.firstLatencyMs;
    mRetryBudgetMs = live.budgetMs;
    mForceClosePhase1Length = live.phase1Length;
    //ownership is stamped ONCE, here: after the swap the live slot is either
    //empty (this arm is about to arm it) or already this arm's, so every arming
    //branch below inherits the right owner and none can forget to set it. That
    //is also finding 2 - the decode-garbage branch that set mRetryActivePrompt
    //without ever setting mRetryArmLand, leaving a fresh seat's land-drop retry
    //owned by "casting" and unreachable from the land arm that armed it.
    mRetryArmLand = landArm;
}


int AIPlayerGPT::pollCompletionRetry(const string& userMsg, string& content,
                                    const char * seam)
{
    if (seam && *seam)
        mRequestSeam = seam; //#W68-BA (J3): the cap this seam decodes under
    //#W74-CF (F1): this arm's own retry slot, before anything reads or writes it.
    const bool w74ThisArmIsLand = asyncLandArm(mPromptTail);
    selectRetryArm(w74ThisArmIsLand);
    //Mid-retry: poll the retry prompt (buildRequestBody sees mRetryActivePrompt
    //and uses the tight retry max_tokens). If the decision drifted, abandon the
    //pending retry and fall through to a fresh poll of the new decision.
    //#W55-E (D23): the seat has moved to a different decision and the ask that
    //missed the wall will never be consumed - write it down before it is lost.
    if (wallMissAbandoned(mWallMissPending, mWallMissBase, asyncSlotKey(userMsg))) //#W82-A (L7)
        flushWallMissRecord();
    //#W74-CD (O2, wave-73 deck152 HIGH-1). THE ARMED SECOND LEG BELONGS TO ONE ARM.
    //The async slots are split per arm (#W57-A D5: land-drop and casting each
    //remember their own request), but this retry state is a single per-seat slot -
    //so the seam that runs on the NEXT tick, whichever arm it belongs to, matched
    //`userMsg == mRetryBase` against its own question, failed, and executed the
    //"decision changed under a pending retry: drop it" branch on the OTHER arm's
    //leg. Two arms alternating tick by tick therefore destroyed each other's
    //forced close for ever: measured on a stub endpoint that makes every reply
    //reasoning-only, 409 forced closes were armed and 38 phase-2 requests were
    //ever sent, and the live corpus shows the same ratio (33 armed, 2 consumed).
    //A leg is now only consumed or abandoned by the arm that armed it; the other
    //arm polls its own slot and leaves it alone, and the arm that owns it reaches
    //it on its next window.
    //#W74-CF (F1): selectRetryArm() above has already made this true - the live
    //slot is this arm's or empty.
    //#W82-A (audit-2026-09): the comment claimed this was "kept as an assertion
    //rather than as the gate it used to be". It is still &&-ed into the gate one
    //line below, so it IS the gate; what changed is that the invariant above
    //makes it always true on the paths that reach here.
    const bool retryArmMatches = mRetryActivePrompt.empty()
        || mRetryArmLand == w74ThisArmIsLand;
    if (!mRetryActivePrompt.empty() && retryArmMatches)
    {
        //#W82-A (L7, audit-2026-09): SLOT-KEYED, not byte-equal to the whole
        //prompt. `mRetryBase` held the FULL assembled prompt and this gate was
        //byte-equality against it, so every per-tick-moving byte the prompt
        //carries - the hold-check bracket's counts, decline notes, prompt-only
        //verdict lines, a narration append - dropped the armed leg on the very
        //next tick: 13 of 13 armed forced closes in the wave-80 corpus were
        //abandoned as `dropped_decision_moved`, each followed by a fresh
        //full-price phase-1 ask on the same window (35-186 s each, 23.4 minutes of
        //inference), and the phase-1 reasoning the model had already been paid for
        //was thrown away. The async slot abandoned full-prompt equality for
        //exactly this reason in #W56-A/#W62-fix; this leg never got the same fix.
        //The slot key is BOARD STATE + QUESTION (#W82-A L2), which is the identity
        //of the DECISION - so a leg survives a re-render and is still dropped the
        //moment the decision really moves.
        if (asyncSlotKey(userMsg) == mRetryBase)
        {
            //#W76-CQ (F2): a DEFERRED close waits here and buys nothing. This is
            //the only exit that can hold a decision without a round trip, and it
            //is bounded by w76ForceCloseDecision's own tick ceiling.
            if (mForceCloseDeferred)
            {
                //#W78-CY (F11): the SAME busy predicate the deferral took, so a
                //close deferred because THIS arm is still armed actually waits.
                const bool w78ContBusy = w78ForceCloseBusy(mRetryPark.forceCloseArmed,
                                                           mForceCloseArmed);
                const int deferWhat = w76ForceCloseDecision(true, w78ContBusy,
                                                            mForceCloseDeferTicks);
                if (deferWhat == kW76CloseDefer)
                {
                    mForceCloseDeferTicks++;
                    return kChoicePending; //still pending, still no heuristic
                }
                const bool w78AtBound = w78ContBusy; //#W78-CY (F11)
                if (w78AtBound)
                    mForceCloseDeferBoundHits++; //armed at the bound, not on a clear
                mForceCloseDeferred = false;
                mForceCloseDeferTicks = 0;
                //THE ARM, taken now rather than at the refused window.
                const int w78Superseded = gptForceCloseArm(mForceCloseArmed);
                mForceCloseUnrecorded += w78Superseded;
                //#W78-CV (S11): the event, named where it happens.
                writeForceCloseRecord(w78Superseded ? "armed_after_defer_superseding"
                                                    : (w78AtBound ? "armed_at_defer_bound"
                                                                  : "armed_after_defer_cleared"),
                                      w74ThisArmIsLand);
                DebugTrace("AIPlayerGPT[" << deckFileSmall << "]: the deferred forced close is"
                           " armed now (" << (w78ContBusy
                                              ? "tick bound reached" : "the arms cleared")
                           << ")");
            }
            int r = pollCompletion(mRetryActivePrompt, content);
            if (r == kChoicePending)
                return kChoicePending;
            //Retry finished: sum both attempts' latency into the record's field,
            //spend this decision's one retry, and hand back the retry reply
            //(possibly still unusable -> the caller's heuristic answers).
            //#W68-BC (J2): keep BOTH legs before the sum overwrites the field.
            //`latency_ms` stays the seat's total wait for the decision (what
            //the harness's cost accounting needs); `attempt_ms` is what each
            //call actually cost, and deadline_pct is computed from these.
            mLastAttemptFirstMs = mRetryFirstLatencyMs;
            mLastAttemptSecondMs = mLastLatencyMs;
            if (mLastLatencyMs >= 0 && mRetryFirstLatencyMs >= 0)
                mLastLatencyMs += mRetryFirstLatencyMs;
            mRetryDoneBase = userMsg;
            mRetryActivePrompt.clear();
            mRetryBase.clear();
            //#W76-CQ (F9): the trace this rescue handed back, kept for the
            //record that is about to classify the reply's off-protocol bytes.
            //Consumed by that record, like mLastReasoning.
            mLastForceClosePrefill = mForceClosePrefill;
            mForceClosePrefill.clear();
            mRetryFirstLatencyMs = -1;
            mRetryBudgetMs = 0; //#W59-H (K1): consumed with the retry
            mLastRetry = true;
            //#W53-Q (D10): the retry ALSO came back empty at the wall. This is
            //the handoff the seat review needs named - the retry below can no
            //longer fire for this base, so the line has to be printed here.
            if (content.empty() && mLastTimeout)
                DebugTrace("AIPlayerGPT: no reply after " << (mTimeoutMs / 1000)
                           << "s - heuristic");
            return 0;
        }
        //Decision changed under a pending retry: drop it, poll the new decision.
        //#W75-CJ (P2c): if that retry was a forced close, this is exactly the
        //arm that leaves no record - count it where it is dropped.
        if (mForceCloseArmed)
        {
            mForceCloseUnrecorded++; //#W75-CM (F5): THIS arm's close, dropped here
            mForceCloseArmed = false;
            //#W78-CV (S11): THIS is the path the wave-77 corpus's ten unrecorded
            //closes took - not a re-arm and not a bound, so no arm-bound counter
            //could ever have named them. The record says so per event.
            writeForceCloseRecord("dropped_decision_moved", w74ThisArmIsLand);
        }
        //#W76-CQ (F2): a DEFERRED close whose decision moved is dropped with the
        //slot. It never armed, so it never counted as outstanding and nothing is
        //unrecorded here - the deferral simply ends with the question it held.
        mForceCloseDeferred = false;
        mForceCloseDeferTicks = 0;
        mRetryActivePrompt.clear();
        mRetryBase.clear();
        mForceClosePrefill.clear();
        //#W82-A (L7, audit-2026-09): the ABANDONED phase-1 leg's latency is the
        //cost of the drop, and zeroing it here made the loss unpriceable from the
        //corpus - 13 drops of 35-186 s each read as free. It is carried onto the
        //record this window is about to write, where a reviewer can add it up.
        if (mRetryFirstLatencyMs >= 0)
        {
            mLastAttemptFirstMs = mRetryFirstLatencyMs;
            if (mLastLatencyMs >= 0)
                mLastLatencyMs += mRetryFirstLatencyMs;
            else
                mLastLatencyMs = mRetryFirstLatencyMs;
        }
        mRetryFirstLatencyMs = -1;
        mRetryBudgetMs = 0; //#W59-H (K1): abandoned with the retry
    }

    int r = pollCompletion(userMsg, content);
    if (r == kChoicePending)
        return kChoicePending;

    mLastRetry = false;
    //BUDGET-FORCED ANSWER (wave-34 #1a/#1b, owner ruling 2026-08-19). Native
    //thinking is BOUNDED, and a bound whose only mechanism is max_tokens loses
    //the decision - the reply comes back mid-thought with no answer at all,
    //which is exactly the truncation class that already costs this project
    //decisions. So the budget is two-phase: phase 1 decodes with
    //max_tokens = budget + the answer reserve; if it comes back with an
    //UNCLOSED <think> (the reply is reasoning-only), phase 2 hands the model
    //its own truncated thinking back with the "</think>" close INJECTED and a
    //tight answer-sized cap, so it must answer from what it already has. This
    //is Qwen's documented budget-forcing pattern, expressed through vLLM's
    //assistant-prefill (continue_final_message) rather than a raw /v1/
    //completions prompt render, because the client has no copy of the chat
    //template and a mis-rendered prompt would be a silent quality change.
    //Scoped to endpoints that accept the prefill (local vLLM/llama.cpp): the
    //subscription and OpenRouter paths hide their reasoning entirely and have
    //nothing to close, and api.openai.com 400s unknown fields.
    //`content.empty()` is stated explicitly rather than left implied by
    //mLastReasoningOnly: the forced close prefills the model's own trace back
    //to it, which is IMPOSSIBLE on a provider that withheld the trace, and a
    //content-bearing reply needs no rescue in the first place. Belt and
    //braces with gptForceCloseSupported(), which already excludes the
    //hidden-trace endpoints by name.
    //#W76-CQ (F2): ONE decision point for the rescue, so no path can reach the
    //heuristic while the rescue is earned. `earned` is the whole gate the two
    //branches below used to repeat.
    const bool w76CloseEarned =
        gptForceCloseEarned(mLastReasoningOnly, content.empty(), mLastFinishLength,
                            !mLastReasoning.empty(), codedAnswerCount(content))
        && userMsg != mRetryDoneBase && gptForceCloseSupported(mEndpoint);
    //#W77-CR (R11 a, wave-76 engine-seat MED-3): THE SAME ARM'S OUTSTANDING
    //CLOSE BOUNDS THE RE-ARM TOO. Wave 76's whole `forced_close_unrecorded` 5
    //were same-arm re-arms with an EMPTY park - the Q8 bound never fired once
    //(refused 0, deferred 0, bound-hits 0) because it only ever looked at the
    //OTHER arm. A re-arm while this arm's close is still outstanding supersedes
    //a phase-2 decode that was already bought: waste, not loss, and it is bounded
    //the same way rather than forbidden. The wait is the same 8-tick ceiling, so
    //a close that genuinely cannot resolve still arms and is still counted
    //unrecorded exactly as before; only the decode spent while the predecessor
    //was still in flight is saved.
    const bool w77CloseBusy = w78ForceCloseBusy(mRetryPark.forceCloseArmed,
                                                mForceCloseArmed); //#W78-CY (F11)
    const bool w77SameArmBusy = w78ForceCloseSameArmBusy(mRetryPark.forceCloseArmed,
                                                         mForceCloseArmed);
    const int w76CloseWhat = w76ForceCloseDecision(w76CloseEarned, w77CloseBusy,
                                                   mForceCloseDeferTicks); //#W76-CN (Q8), #W77-CR (R11 a)
    if (w76CloseWhat == kW76CloseDefer)
    {
        //DEFERRED: the other arm's close is still outstanding in the park. Do not
        //buy a second phase-2 decode for a question this seat cannot answer
        //twice - and do NOT let the window fall through to the heuristic either.
        //The slot is built exactly as the arm below builds it and the decision
        //stays PENDING until the park clears (or the tick bound arms it anyway).
        if (!mForceCloseDeferred)
        {
            //#W77-CR (R11 a): the two bounds keep their OWN meters - the park's
            //(Q8/F2) and this arm's - so a corpus can tell which one fired.
            if (w77SameArmBusy)
                mForceCloseSameArmDeferred++;
            else
                mForceCloseArmsRefused++; //once per deferral, never once per tick
            writeForceCloseRecord(w77SameArmBusy ? "deferred_same_arm_busy"
                                                 : "deferred_park_busy",
                                  w74ThisArmIsLand); //#W78-CV (S11)
        }
        mForceCloseDeferred = true;
        mForceCloseDeferTicks++;
        mRetryFirstLatencyMs = mLastLatencyMs;
        mRetryBase = asyncSlotKey(userMsg); //#W82-A (L7)
        mForceClosePrefill = mLastReasoning;
        mForceClosePhase1Length = mLastFinishLength;
        mRetryActivePrompt = string(kForceCloseTag) + userMsg;
        setNotice("thinking hit its budget - waiting for the answer", 3.0f);
        DebugTrace("AIPlayerGPT[" << deckFileSmall << "]: DEFERRING a forced close - the other"
                   " arm's close is still outstanding (" << mForceCloseArmsRefused
                   << " deferred this game); the decision stays pending, not the heuristic's");
        return kChoicePending;
    }
    else if (w76CloseWhat == kW76CloseArm)
    {
        mRetryFirstLatencyMs = mLastLatencyMs;
        mRetryBase = asyncSlotKey(userMsg); //#W82-A (L7)
        //#W75-CJ (P2c): an arm that never reached a record is the "third path".
        //A fresh arm while the previous one is still outstanding proves the
        //previous close was lost, so it is counted here rather than guessed
        //from a stderr/record diff after the fact.
        //#W75-CM (F5): only a second arm on THIS arm proves a lost close; the
        //other arm's outstanding close is parked, not lost.
        {
            const int w78Superseded = gptForceCloseArm(mForceCloseArmed);
            mForceCloseUnrecorded += w78Superseded;
            writeForceCloseRecord(w78Superseded ? "armed_superseding_outstanding"
                                                : "armed", w74ThisArmIsLand); //#W78-CV (S11)
        }
        mForceClosePrefill = mLastReasoning;
        mRetryActivePrompt = string(kForceCloseTag) + userMsg;
        //Only a decode that STOPPED AT THE CAP is a budget hit. The other way
        //into this branch is a reply that ended its thinking naturally and
        //simply never wrote an answer line - the rescue is identical, the
        //diagnosis is not, and conflating them made the budget-hit rate
        //unreadable (a 12,058-char trace marked as hitting an 8,000-token
        //budget, wave-34 b6). Both are still counted, under their own names.
        //#W71-BO (L9): phase 1's own finish_reason is REMEMBERED here and stamped
        //when the phase-2 answer is consumed - not stamped now, on a record this
        //decision may never write.
        mForceClosePhase1Length = mLastFinishLength;
        setNotice("thinking hit its budget - asking for the answer", 3.0f);
        DebugTrace("AIPlayerGPT: unclosed <think> (budget/truncation); forcing the answer");
        return kChoicePending; //next tick polls the forced-close request
    }
    //DEADLINE/TRANSPORT RETRY (#W53-Q, D10; #W59-H K1). The ask was never
    //answered, so there is no answer to double-consume and re-sending it
    //is not a re-ask on a drifted board - the staleness gate above still owns
    //that question (a decision that moved while this was in flight abandons the
    //retry at the top of this function, exactly as the other two retries do).
    //ONE, gated by mRetryDoneBase like every other retry here, and only when
    //the reply is EMPTY: a reply that came back and merely failed to parse has
    //its own classes and must not be spent on a second round trip.
    //#W59-H (K1) adds the SECOND way in, beside the wall miss: the round trip
    //never reached the model at all (F3 - a 2.5 s connect cap that fired 89
    //times in the wave-58 corpus, all filed as `empty_reply` and played by the
    //heuristic). That is not the model declining to answer, and the decision's
    //deadline is still mostly unspent, so the ask is bought again inside what
    //is LEFT of it - which is what keeps this one retry, not a loop, and keeps
    //the seat's total wait at one deadline. The wall-miss arm below is
    //unchanged: it still gets the full deadline and still opens the wall_miss
    //account.
    const bool transportFailure = retryableTransportFailure(
        mLastCurlResult, mLastHttpStatus, content.empty());
    const long retryBudgetMs = remainingTransportRetryMs(mTimeoutMs, mLastLatencyMs);
    //#W68-BC (J2): ONE remainder rule for BOTH arms. The wall arm used to pass
    //mRetryBudgetMs = 0, which buildRequestBody reads as "no override" - a
    //fresh, FULL deadline - so a decision that had already spent 900 s could
    //spend 900 s more. `retryFitsInDeadline` is that arithmetic, and on a wall
    //miss it is false: the decision goes to the heuristic and SAYS SO, in a
    //`wall_miss_no_retry` record written here rather than a second deadline.
    //Nothing is capped and no window is removed - the heuristic answers the
    //same window it would have answered at 1800 s.
    if (content.empty() && mLastTimeout && userMsg != mRetryDoneBase
        && !retryFitsInDeadline(mTimeoutMs, mLastLatencyMs))
    {
        mWallMissEvents++; //#W82-A R1: restored (LEDGER v2)
        mWallMissLatencyMs = mLastLatencyMs;
        mWallMissPending = true;
        mWallMissBase = asyncSlotKey(userMsg); //#W82-A (L7)
        setNotice("no reply from the model - the heuristic answers", 3.0f);
        DebugTrace("AIPlayerGPT: no reply after " << (mTimeoutMs / 1000)
                   << "s - the deadline is spent, no retry - heuristic");
        flushWallMissRecord("wall_miss_no_retry");
        return 0; //the caller's heuristic answers this decision
    }
    if (content.empty() && userMsg != mRetryDoneBase && retryBudgetMs > 0
        && (mLastTimeout
            || (transportFailure && !mLastStaleLivelock)))
    {
        mRetryFirstLatencyMs = mLastLatencyMs;
        mRetryBudgetMs = retryBudgetMs;
        mRetryBase = asyncSlotKey(userMsg); //#W82-A (L7)
        mRetryActivePrompt = string(kTimeoutRetryTag) + userMsg;
        //#W55-E (D23): arm the wall-miss account on a DEADLINE miss. Whichever comes
        //first closes it: the record that consumes this prompt stamps wall_miss,
        //or the decision is abandoned and flushWallMissRecord writes it down.
        //A transport miss never reached the wall and opens no such account.
        if (mLastTimeout)
        {
            mWallMissPending = true;
            mWallMissBase = asyncSlotKey(userMsg); //#W82-A (L7)
            mWallMissLatencyMs = mLastLatencyMs; //#W61-U (C13)
            mWallMissEvents++; //#W82-A R1: restored (LEDGER v2)
            setNotice("no reply from the model - asking once more", 3.0f);
            DebugTrace("AIPlayerGPT: no reply after " << (mTimeoutMs / 1000)
                       << "s - one retry");
        }
        else
        {
            setNotice("the connection failed - asking once more", 3.0f);
            DebugTrace("AIPlayerGPT: transport failed (curl=" << mLastCurlResult
                       << ", http=" << mLastHttpStatus << ") - one retry with "
                       << retryBudgetMs << " ms of the deadline left");
        }
        return kChoicePending; //next tick polls the retry
    }
    //The retry was already spent (or could not run) and the wall was reached
    //again: the decision goes to the heuristic, and SAYS SO. Before this line
    //stderr showed only "-> chose -1 of N" and a seat review could not tell a
    //15-minute deadline from a refusal.
    if (content.empty() && mLastTimeout)
        DebugTrace("AIPlayerGPT: no reply after " << (mTimeoutMs / 1000)
                   << "s - heuristic");
    //Fire ONE answer-locked retry iff the reply is decode-garbage AND this
    //decision's retry has not already been spent. isDecodeGarbage is
    //conservative - ordinary unparsed replies (real prose, no coded line) are
    //not garbage and fall straight through to the heuristic.
    //#W71-BS (F5, Astra review finding 5). The cap bit before the declaration was
    //finished. That is not an unparseable reply and not a decline - it is the
    //engine's own allowance landing mid-list, and a cut after a complete pair still
    //PARSES, so the surviving prefix executes and the rest of a legal declaration is
    //silently gone. It buys exactly ONE re-ask, and the re-ask is the same question
    //with a bigger allowance: no added text, no correction line, nothing that could
    //license prose (invariant 000). The floor is doubled for the retry, which is
    //what makes it a different request rather than the same one twice.
    if (!content.empty() && mLastFinishLength && gptSeamIsADeclaration(mRequestSeam)
        && userMsg != mRetryDoneBase && userMsg != mCeilingReaskDoneBase)
    {
        mCeilingReaskDoneBase = userMsg;
        const long raised = (mLastRequestAnswerTokens > 0 ? mLastRequestAnswerTokens : 256) * 2;
        if (raised > mAnswerFloorTokens)
        {
            mAnswerFloorSeam = mRequestSeam;
            mAnswerFloorTokens = raised;
        }
        mRetryFirstLatencyMs = mLastLatencyMs;
        mRetryBase = asyncSlotKey(userMsg); //#W82-A (L7)
        mRetryActivePrompt = string(kTimeoutRetryTag) + userMsg; //identical bytes, own slot
        setNotice("that declaration hit its length limit - asking again", 5.0f);
        DebugTrace("AIPlayerGPT: " << mRequestSeam << " reply truncated at the answer ceiling ("
                   << mLastRequestAnswerTokens << ") - one re-ask at " << mAnswerFloorTokens);
        return kChoicePending; //next tick polls the same question with more room
    }
    if (!content.empty() && userMsg != mRetryDoneBase && isDecodeGarbage(content))
    {
        mRetryFirstLatencyMs = mLastLatencyMs;
        mRetryBase = asyncSlotKey(userMsg); //#W82-A (L7)
        mRetryActivePrompt = string(kAnswerLockPrefix) + userMsg;
        setNotice("previous reply was corrupted - re-asking briefly", 3.0f);
        DebugTrace("AIPlayerGPT: decode-garbage reply; launching one answer-locked retry");
        return kChoicePending; //next tick polls the retry prompt
    }
    return 0;
}


AIPlayerGPT::AIPlayerGPT(GameObserver *observer, string deckFile, string deckfileSmall, string avatarFile, MTGDeck * deck)
    : AIPlayerBaka(observer, deckFile, deckfileSmall, avatarFile, deck), mAsyncState(std::make_shared<AsyncState>()), mAsyncLandState(std::make_shared<AsyncState>()), mThinkTime(0), mNoticeTicks(0), mFallbackCount(0), mDegradedTicks(0), mBlocksDoneTurn(-1), mBlockReaskTurn(-1), mBlockIllegalReaskTurn(-1), mLastRequestMaxTokens(0), mLastRequestAnswerTokens(0), mLastRequestReasoningTokens(0), mThinkingRegimeExplicit(false), mThinkingRegimeAnnounced(false), mAttackReaskTurn(-1), mBlockRevReaskTurn(-1), mAskReaskPriorChoice(-1), mPriorityReaskPriorChoice(-1), mAttacksDoneTurn(-1), mOrderDoneTurn(-1), mCastMenuAsked(false), mLandShapeForPrompt(0), mPriorityTurnSeen(-1), mLoopAbility(NULL), mLoopClick(NULL), mLoopCount(0), mRepeatAbility(NULL), mRepeatClick(NULL), mRepeatRemaining(0), mRepeatTotal(0), mRepeatDone(0), mRepeatNoProgress(0), mRepeatAbsent(0), mManaOnlyWindowsSkipped(0), mStopReachedWindowsSkipped(0), mOwnTurnWindowsSkipped(0), mRepeatAskTurn(-1), mRepeatAskChoice(0), mRepeatAskAnswersReserved(0), mCommittedCastTurn(-1), mAnswerReplacedFalse(false), mLandFacePreCard(NULL), mLandFacePreTurn(-1), mLandFacePreBack(false), mCastAskTurn(-1), mCastAskPhase(-1), mCrackBackFactsSeq(-1), //#W75-CI (P18)
       mHoldTurn(-1), mHoldOwnTurnAtTake(false), mHoldWindowTurn(-1), mHoldWindowPhase(-1), mSiblingWindowAsksSkipped(0), mHoldReleasedTurn(0), mChainWindowsCollapsed(0), mChainWindowsOnlySelfharm(0), mChainSelfharmRows(0), mChainActingRows(0), mChainWindowsOnlySelfharmCast(0), mChainSelfharmRowsCast(0), mChainActingRowsCast(0), //#W75-CI (P12)
       mMainPhaseWindowsSkipped(0), mHoldWindowsSkipped(0), mReserveDeclineSources(-1), mReserveDeclineTurn(-1), mReserveDeclinePhase(-1), mReserveDeclineWindows(0), mReserveDeclineSpanTurn(-1), mReserveDeclineNoted(0), mEngineRevealFloorPicks(0), mRecoveryExecRow(-1), mHoldWindowsSkippedPriority(0), mHoldWindowsSkippedCast(0), mAsyncDropsGame(0), mRepeatAnnotatedTakes(0), mBlockerForecastRows(0), mBlockerForecastMulti(0), mBlockerForecastGang(0), mBlockerForecastCollapsed(0), mProtocolReplies(0), mPlanStepsDone(0), mPlanLineMissing(0), mPlanNamesStrandedCard(0), mPhase2AnswerRecovered(0), mPhase2AnswerMissing(0), mPutGlossStripped(0), mForceClosePhase1Length(false), mRetryArmLand(false), mForceCloseUnrecorded(0), mForceCloseArmed(false), mForceCloseArmsRefused(0), mForceCloseDeferred(false), mForceCloseDeferTicks(0), mForceCloseDeferBoundHits(0), mForceCloseSameArmDeferred(0), mHoldCheckRefSeq(-2), mHoldCheckRefWindow(-2), mStopReachedRePutsCollapsed(0), mStackDrainWindowsAsked(0), mStackDrainCountedSeq(-1), mForceCloseEvents(0), mOwnLoopWindowsAsked(0), mOwnLoopCountedSeq(-1), mOwnLoopVerdictLinesRendered(0), mOwnLoopVerdictCountedSeq(-1), mHoldVerdictSaferIgnored(0), mMenuPassNoProgressSuppressed(0), mStubReplyIndex(0), mCrossPhaseBoardUnchanged(0), mPlanCastCompletionState(0), mPaidPendingSources(0), mActionBeforePlanRejected(false), mPlanCastOpenTurn(-1), mPlanCastStepsClosed(0), mNextSendDrain(false), mCrackBackLethalBlockedAway(0), mW81FoldedCrackBackTotals(0), mW81XCastRefusalMarkers(0), mW81XSweepRosterMarkers(0), mW81AttackCoverClauses(0), mW81SpareColourWithheld(0), mW81EventCountedSeq(-1), //#W76-CQ (F2), #W77-CR (R11 a, R2 d, R1, R8), #W79-DC (F1), #W80-DG (U1)
        mCrossPhaseRePuts(0), mCrossPhaseTurn(-1), mPlanNamesUncastableZoneCard(0), mProtocolDeviationReplies(0), mAnswerLabelAbsentRead(0), mCrackBackVerdictLinesRendered(0), mCrackBackVerdictCountedSeq(-1), mStackDeathVerdictLinesRendered(0), mStackDeathVerdictCountedSeq(-1), mAskReplaysCache(0), mAskReplaysRepeatLatch(0), //#W80-DE (U2/U8/U9), #W80-DF (U13) - restored after the merge dropped them (Astra w80 F1) //#W78-CX (S1), #W79-DD //#W74-CD (O2) //#W70-BK (C4/C5), #W70-BM (E2/E3), #W67-AX (I7), #W67-AZ (R7), #W68-BA (J3/J6), #W68-BE (R1)), #W68-BE (R1), #W69-BI (K7)
       mLoopAutoPassRun(0), mLastRepeatN(0), mListDeclineTurn(-1), mIncomingCombatTurn(-1), mIncomingCombatAttackers(0), mIncomingCombatDamage(0), mPlanSetSeq(-1), mPlanSetTurn(0), mTransSeq(0), mWindowSeq(0), mLastLatencyMs(-1), mAbandonedInFlightSecs(-1), mAsyncDropStampsSeq(-1), mAbandonedInFlightSeq(-1), mGameEndLogged(false), mGameStartLogged(false), mNarratedTurnOwner(NULL), mNarratedTurnNumber(-1), mLogWindowKind(kAskWindowUnknown), mLogWindowElided(0), mDealDone(false), mCounteredSpell(NULL), mLastChoice(-1), mRetryFirstLatencyMs(-1), mRetryBudgetMs(0), mLastRetry(false), mAskAnswerReserved(false),
      mPregameBottomAsked(false), mPregameBottomForMulls(-1), mPregameMullsSeen(0),
      mLastReasoningOnly(false), mLastFinishLength(false), mLastBudgetHit(false),
      mLastForcedClose(false), mLastReasoningDegenerate(-1.0), mLastReasoningNgramRepeat(-1.0), mReasoningBudget(0),
      mLastReasoningTokens(-1), mLastDroppedAssignments(-1), mLastReasoningHidden(false),
      mStaleDropStreak(0), mLastStaleLivelock(false),
      mRevealStallTicks(0), mRevealStallSecs(0), mRevealStallPhase(-1), mRevealStallParked(false), mRevealStallDriverTicks(0), mRevealStallDriverSecs(0),
      mWallMissPending(false), mWallMissLatencyMs(-1), //#W61-U (C13)
      mWallMissEvents(0), mWallMissUnrecorded(0), mWallMissNoRetry(0), //#W82-A R1
      mActionBeforePlanReplies(0), mActionBeforePlanRejects(0),        //#W82-A R1
      mHoldReopenedNewThreat(0), mHoldReopenedNewLethal(0), mHoldEvents(0), //#W82-A R1
      //#W68-BC (J2)
      mLastAttemptFirstMs(-1), mLastAttemptSecondMs(-1), //#W68-BC (J2)
      mLastTimeout(false), mLastBadReply(false), mRecoverySeq(-1),
      mReplyLabelAbsent(false), mRecoveryLabelAbsent(false), //#W79-DB (T1)
      mInPregameAsk(false),
      mInAnnounceXAsk(false),
            mMayBatchVerdict(kMayBatchNone), mMayBatchRemaining(0),
      mRunLastCount(0), //#W57-D (D29)
      mInStateBasedActionAsk(false), mSbaWindowsCacheBypassed(0),
      mHoldReopenAnswerInvalidated(0),
      mCastDecisionReopenedNewStack(0), mCastReopenCountedSeq(-1),
      mTransportHandedOff(false),
      mVerdictFaceWindow(-1), mVerdictFacesDroppedUnrecorded(0),
      mLastWindowRecordSeq(-1), //#W81-DK (V1/V3/V4/V9/V15)
      mOppCounteredSpells(0), mCounterIntelRendered(0), //#W81-DM (V13)
      mMultiAnswerFirstTaken(0) //#W81-DM (V12)

{
    mStatedStop = -1;      //#W67-AY (I6): nothing stated yet
    mStatedStopTurn = -1;  //#W72-BX (F2)
    mStatedStopOppLife = -1; //#W74-CE (O7c)
    mCastDecisionOpen = 0; //#W72-BX (F3)
    mLastPoison[0] = mLastPoison[1] = 0; //N-105a: poison deltas start from zero
    for (int i = 0; i < 3; i++) //#W57-E (D15)
    {
        mOppLifeByTurn[i] = 0;
        mOppLifeTurnNo[i] = 0;
        mOppLifeGainedAt[i] = 0; //#W78-CY (F7)
        mOppLifeLostAt[i] = 0;
    }
    mOppLifeSamples = 0;
    mOppLifeEventGained = 0; //#W78-CY (F7)
    mOppLifeEventLost = 0;
    mOppCounteredSpells = 0; //#W81-DM (V13)
    mCounterIntelRendered = 0; //#W81-DM (V13)
    mMultiAnswerFirstTaken = 0; //#W81-DM (V12)
    mOppLifeLastTurn = -1;
    mBulkMoveCount = 0;                  //W41-3(c): no bulk move pending
    mBulkMoveMine = false;
    //#W43-11/#W48 (D11): mEventRun starts empty (its own ctor); nothing held.
    mSearchRevealMine = false;           //#W42-D1: no search run pending
    mSearchIsFullDump = false;
    mSearchIsWholeZone = false;
    mSearchRevealOwner = NULL;
    mSearchMaskOwner = NULL;
    mDamageLifePlayer = NULL;            //W43-R2: no damage life change held
    mDamageLifeAmount = 0;
    mDamageLifeSettled = 0;
    mPregameBottomingNow = false;        //#W42-D9
    mPregameShufflingBack = false;
    mPregameBottomedCount = 0;
    //audit-L (A49): once per process, on the game thread, before any worker
    //(curl_global_init is documented not thread-safe; it used to run per seat,
    //possibly while the previous duel's detached worker was still in libcurl).
    gptCurlInit();
    mLastHttpStatus = 0; //audit-L (A24)
    mLastBadReply = false; //#W60-Q (R9)
    mLastCurlResult = -1; //#W59-H (K1)
    //File config first, environment variables override.
    GptSettings cfg = GptSettings::load();
    mConfigUrls = cfg.urls;
    mConfigModel = cfg.model;
    mMaxTokens = cfg.maxTokens;
    mRepetitionPenalty = cfg.repetitionPenalty;
    if (const char * rp = getenv("WAGIC_GPT_REPPENALTY"))
        mRepetitionPenalty = atof(rp);
    mProviderOnly = cfg.providerOnly;
    if (const char * po = getenv("WAGIC_GPT_PROVIDER"))
        mProviderOnly = po;
    mApiKey = cfg.key;
    mTimeoutMs = 1000L * cfg.timeoutSecs;
    if (const char * key = getenv("WAGIC_GPT_KEY"))
        mApiKey = key;
    if (const char * to = getenv("WAGIC_GPT_TIMEOUT"))
        mTimeoutMs = 1000L * atol(to);
    if (mTimeoutMs < 5000)
        mTimeoutMs = 5000;
    mPatienceLimit = (float) cfg.patienceSecs;
    if (const char * pt = getenv("WAGIC_GPT_PATIENCE"))
        mPatienceLimit = (float) atof(pt);
    if (mPatienceLimit < 0)
        mPatienceLimit = 0;
    //#W70-BK (C6, invariant 000(f)): the regime is a STATED thing. env wins
    //over the config file, exactly as before; what is new is that the client
    //remembers whether ANYONE stated it. Unset still resolves to OFF (the
    //product regime the owner named), but the first request says so out loud
    //with the ruling attached, so no corpus and no play session can inherit
    //thinking-off from a stale launch recipe again - which is exactly how
    //every corpus from wave 44 to wave 69 came to run with reasoning off.
    {
        const char * thinkEnv = getenv("WAGIC_GPT_THINKING");
        mThinkingRegimeExplicit = (thinkEnv && *thinkEnv) || cfg.thinking >= 0;
        mThinking = thinkEnv ? envFlag("WAGIC_GPT_THINKING") : (cfg.thinking == 1);
        mThinkingRegimeAnnounced = false;
    }
    //Thinking budget: configured value wins, else the shipped default, and
    //only in thinking mode (a non-thinking reply has no window to bound).
    mReasoningBudget = (cfg.reasoningBudget >= 0) ? cfg.reasoningBudget : kDefaultReasoningBudget;
    if (const char * rb = getenv("WAGIC_GPT_REASONING_BUDGET"))
        mReasoningBudget = atol(rb);
    if (!mThinking)
        mReasoningBudget = 0;
    //Timeout FLOOR for thinking mode, applied only to the DEFAULT. The HTTP
    //timeout is the ONLY watchdog that falls back to the heuristic (the
    //patience window raises a human prompt and never decides on its own), so
    //it has to sit ABOVE the worst case a decision can legitimately take -
    //otherwise the budget machinery gets cut off rescuing exactly the
    //decisions it exists for. Worst case, budgeted end to end at this stack's
    //~30 tok/s: phase 1 = prompt prefill (~10-20s at corpus prompt sizes) +
    //(6000 thinking + ~230 reply) tokens ~= 210s (wave-35 budget cut; it was
    //295s at 8000); a budget hit then adds phase 2 = re-prefill of prompt + the
    //6k of thinking (~15-25s) + ~400 tokens ~= 40s. Total ~= 250-275s, and each
    //phase is its own request. The floor is deliberately NOT retuned down: 420s
    //keeps a real margin over that rather than shaving it. An explicit timeout
    //(env or config) is the user's call and is never raised over their head;
    //the built-in default (600s) already clears this floor, so the guard is
    //for a future default, not for today.
    if (mThinking && mReasoningBudget > 0 && !getenv("WAGIC_GPT_TIMEOUT")
        && cfg.timeoutSecs == GptSettings().timeoutSecs && mTimeoutMs < 420000)
        mTimeoutMs = 420000;
    mReasoningEffort = cfg.reasoningEffort;
    if (const char * re = getenv("WAGIC_GPT_EFFORT"))
        mReasoningEffort = re;
    //Only the server's own tiers pass; anything else falls to the built-in
    //default rather than 400-ing every decision on a typo.
    if (!mReasoningEffort.empty() && !gptCodexEffortValid(mReasoningEffort))
    {
        gptLogLine("reasoning_effort '" + mReasoningEffort + "' is not one of none/low/medium/high/xhigh/max - using the default");
        mReasoningEffort.clear();
    }
    //Telemetry consent implies local decision logging: the log IS the data
    //a future contribution/upload mechanism would share.
    bool translog = getenv("WAGIC_GPT_TRANSLOG") ? envFlag("WAGIC_GPT_TRANSLOG")
                                                 : (cfg.translog == 1 || cfg.telemetry == 1);
    if (translog)
    {
        //gptUserRoot, NOT getenv("HOME"): this was the fifth raw-HOME site
        //(the Aug 1 sweep fixed four) and it silently disabled the translog
        //on Vita - the one platform where a decision log is hardest to
        //recover any other way.
        string dir = gptUserRoot();
        if (!dir.empty())
        {
            GPT_MKDIR(dir.c_str());
            dir += "/ai"; GPT_MKDIR(dir.c_str());
            dir += "/gpt"; GPT_MKDIR(dir.c_str());
            dir += "/logs"; GPT_MKDIR(dir.c_str());
            std::ostringstream p;
            p << dir << "/" << time(NULL) << "-" << deckfileSmall << "-" << (void *) this << ".jsonl";
            mTransLogPath = p.str();
        }
    }
    resolveEndpoint();
    if (mEndpoint.empty())
    {
        fprintf(stderr, "AIPlayerGPT: no LLM endpoint reachable, falling back to Baka heuristics for every decision\n");
        setNotice("no LLM endpoint reachable - the heuristic AI is playing", 12.0f);
    }
    else
        fprintf(stderr, "AIPlayerGPT: using %s (model %s)\n", mEndpoint.c_str(), mModel.c_str());
}

#if defined(WAGIC_GPT_PARSETEST_BUILD)
string AIPlayerGPTSelfTestAccess::answerTailFromReasoning(const string& reasoning) { return ::answerTailFromReasoning(reasoning); }
const char * AIPlayerGPTSelfTestAccess::asyncDropOutcome(bool gaveUpToHeuristic) { return ::asyncDropOutcome(gaveUpToHeuristic); }
string AIPlayerGPTSelfTestAccess::asyncDropStamp(const char * arm, const string& driftKind, const char * outcome) { return ::asyncDropStamp(arm, driftKind, outcome); }
string AIPlayerGPTSelfTestAccess::asyncDropTraceLine(const char * arm, const string& driftKind, const char * outcome) { return ::asyncDropTraceLine(arm, driftKind, outcome); }
bool AIPlayerGPTSelfTestAccess::asyncLandArm(const string& seamTail) { return ::asyncLandArm(seamTail); }
string AIPlayerGPTSelfTestAccess::asyncSlotDriftKind(const string& oldKey, const string& newKey) { return ::asyncSlotDriftKind(oldKey, newKey); }
string AIPlayerGPTSelfTestAccess::asyncSlotKeyOf(bool forceClose, int turn, int phase, const string& seamTail, const string& board) { return ::asyncSlotKeyOf(forceClose, turn, phase, seamTail, board); }
std::vector<long> AIPlayerGPTSelfTestAccess::attemptMsField(long firstMs, long secondMs, long latencyMs) { return ::attemptMsField(firstMs, secondMs, latencyMs); }
bool AIPlayerGPTSelfTestAccess::gptDeadlineMissed(bool emptyBody, long elapsedMs, long timeoutMs, long httpStatus, long curlCode) { return ::gptDeadlineMissed(emptyBody, elapsedMs, timeoutMs, httpStatus, curlCode); }
bool AIPlayerGPTSelfTestAccess::gptForceCloseEarned(bool reasoningOnly, bool contentEmpty, bool finishLength, bool haveReasoning, int codedAnswers) { return ::gptForceCloseEarned(reasoningOnly, contentEmpty, finishLength, haveReasoning, codedAnswers); }
string AIPlayerGPTSelfTestAccess::gptForceClosePrefillBody(const string& prefill) { return ::gptForceClosePrefillBody(prefill); }
bool AIPlayerGPTSelfTestAccess::hasCodedAnswerLine(const string& content) { return ::hasCodedAnswerLine(content); }
bool AIPlayerGPTSelfTestAccess::reasoningHiddenShape(bool requested, const string& content, const string& reasoning) { return ::reasoningHiddenShape(requested, content, reasoning); }
double AIPlayerGPTSelfTestAccess::reasoningNgramRepeatRatio(const string& s) { return ::reasoningNgramRepeatRatio(s); }
double AIPlayerGPTSelfTestAccess::reasoningRepetitionRatio(const string& s) { return ::reasoningRepetitionRatio(s); }
bool AIPlayerGPTSelfTestAccess::splitReasoningBlock(string& content, string& reasoning) { return ::splitReasoningBlock(content, reasoning); }
string AIPlayerGPTSelfTestAccess::transportRecordField(const std::vector<std::string>& failures, const std::string& roundTripStamp, long latencyMs) { return ::transportRecordField(failures, roundTripStamp, latencyMs); }
string AIPlayerGPTSelfTestAccess::transportStampPhase(const string& transport) { return ::transportStampPhase(transport); }
bool AIPlayerGPTSelfTestAccess::w76ForceCloseArmAllowed(bool parkArmed) { return ::w76ForceCloseArmAllowed(parkArmed); }
int AIPlayerGPTSelfTestAccess::w76ForceCloseDecision(bool earned, bool parkArmed, int deferTicks) { return ::w76ForceCloseDecision(earned, parkArmed, deferTicks); }
bool AIPlayerGPTSelfTestAccess::w78ForceCloseBusy(bool parkArmed, bool thisArmArmed) { return ::w78ForceCloseBusy(parkArmed, thisArmArmed); }
bool AIPlayerGPTSelfTestAccess::w78ForceCloseSameArmBusy(bool parkArmed, bool thisArmArmed) { return ::w78ForceCloseSameArmBusy(parkArmed, thisArmArmed); }
bool AIPlayerGPTSelfTestAccess::wallMissAbandoned(bool pending, const string& missedBase, const string& nowBase) { return ::wallMissAbandoned(pending, missedBase, nowBase); }
string AIPlayerGPTSelfTestAccess::wallMissClassFor(const string& phase) { return ::wallMissClassFor(phase); }
const long AIPlayerGPTSelfTestAccess::kAnswerReserveTokens = ::kAnswerReserveTokens;
const long AIPlayerGPTSelfTestAccess::kDefaultReplyCeilingTokens = ::kDefaultReplyCeilingTokens;
const long AIPlayerGPTSelfTestAccess::kAnswerLockedRetryTokens = ::kAnswerLockedRetryTokens;
const long AIPlayerGPTSelfTestAccess::kDefaultReasoningBudget = ::kDefaultReasoningBudget;
const int AIPlayerGPTSelfTestAccess::kW76CloseDeferMaxTicks = ::kW76CloseDeferMaxTicks;
#endif //WAGIC_GPT_PARSETEST_BUILD

#endif //WITH_GPT_AI
