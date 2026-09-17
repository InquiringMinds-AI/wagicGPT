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

//#W82-P12: ANSWER SELECTION AND PARSING - the reply readers (PLAN split,
//coded lines, latches, parseChoice). Moved verbatim out of AIPlayerGPT.cpp.


bool AIPlayerGPT::choiceLineIsClean(const string& payload)
{
    size_t head = codedHeadEnd(payload);
    if (head == string::npos)
        return true; //not judged
    bool terminal = false;
    string tail = codedLineTail(payload, head, &terminal);
    if (tail.empty())
        return true;
    static const char * affirm[] = {
        "is correct", "is right", "is the best", "is best", "is better", "is optimal",
        "is the answer", "is my answer", "is my choice", "is my final", "is final",
        "final answer", "it is", "it's", "stands", "confirmed", "is the play",
        "is the correct", "is the right", "is the safest", "is the only", "is the strongest",
        "is what", "is clearly", "is definitely", "is still", "is the one", "is the pick",
        "is the choice", "is the decision", "is the line", "is my pick"
    };
    return tailOpensWith(tail, affirm, sizeof(affirm) / sizeof(affirm[0]));
}


bool AIPlayerGPT::choiceLineIsRejection(const string& payload)
{
    size_t head = codedHeadEnd(payload);
    if (head == string::npos)
        return false;
    bool terminal = false;
    string tail = codedLineTail(payload, head, &terminal);
    if (tail.empty())
        return false;
    static const char * reject[] = {
        "is wrong", "is incorrect", "is not", "isn't", "is a mistake", "was wrong",
        "was a mistake", "would", "wouldn't", "could", "couldn't", "instead", "rather",
        "not ", "no ", "no,", "no.", "don't", "do not", "doesn't", "does not", "cannot",
        "can't", "can not", "mistake", "wrong", "is bad", "is worse", "is illegal",
        "is impossible", "is unavailable", "is too", "should not", "shouldn't", "never",
        "is suboptimal", "is inferior", "is a trap", "is out", "loses", "only if", "if ",
        "unless", "when ", "this is wrong", "that is wrong", "that's wrong", "this is not",
        "that is not", "this would", "that would", "which is wrong", "which would", "but ",
        "however", "was the", "was my", "is the wrong", "is the worse", "seems wrong",
        "looks wrong", "might", "may not", "is risky", "is dangerous", "is off"
    };
    return tailOpensWith(tail, reject, sizeof(reject) / sizeof(reject[0]));
}


//Find the LAST line-leading answer-label line in `text`, returning its
//remainder (after the label token) as [segStart, segEnd) plus the line's start
//offset. Template-placeholder lines are skipped. When `expectedLabel` is
//non-NULL, ONLY lines with that exact label count - the other answer labels are
//treated as chain-of-thought and ignored.
//(history: comment-archaeology.md AIPlayerGPTParse-L190-1771)
//(history: comment-archaeology.md AIPlayerGPTParse-L190-1785)
//#W50-Y D7: CHOICE lines are judged by the clean-line grammar. A REJECTION
//line ("CHOICE: 1 (X) is wrong because ...") is never the answer and never
//an adjacency anchor; among the rest the last CLEAN line (or clean run head)
//wins, and an UNCLEAN trailer ("CHOICE: 2 (X) since ...") is taken only when
//no clean line exists. `rejectedLines` (out, optional) counts the lines the
//scan refused - rejections, and unclean lines that lost to a clean one - so
//the seam can sign `rejected_line_skipped`.
//(history: comment-archaeology.md AIPlayerGPTParse-L190-1802)
//(history: comment-archaeology.md AIPlayerGPTParse-L190-1825)
//#W63-AD's plan-block exclusion cannot reach any of them: the block ENDS at the
//first paragraph (735, 2467, 857) while the deliberation runs on for kilobytes,
//so every one of those lines is "outside the plan" by the letter and pure
//chain-of-thought in fact.
//THE RULE: the FIRST coded answer line is the answer. A LATER line supersedes it
//only when it EXPLICITLY CORRECTS it - a coded answer line (the #W64-AK R5 skip
//set, so an INDENTED top-level correction still counts) carrying a retraction
//marker on itself or on the nearest preceding non-blank line. Deliberation
//vocabulary ("wait", "actually", "hmm", "let me re-read") is deliberately NOT a
//marker: that is exactly the prose the corpus lost its rows to. kReplyProtocol
//now states this rule, so the surface and the engine agree about the same bytes.
//The clean/rejection grammar (#W50-Y D7) and the adjacent-run head rule (W36
//lane-B item 3) are unchanged - they now select among the heads the scan
//collected instead of tracking "the last one seen".
//`lastHeadLineStart` (out, optional) reports the LAST head the scan saw, so the
//consuming seam can sign `later_answer_ignored` whenever a later coded line was
//refused. No override and no refusal is silent.
struct GptAnswerHead
{
    size_t segStart, segEnd, lineStart;
    bool isChoice, clean;
};


//#W70-BM (E2). ONE ANSWER LINE, THE FIRST ONE. Everything the wave-65/66/67
//selector did beyond that was tolerance for in-band reasoning and is DELETED
//here: `gptAnswerCorrectionCue`'s 18-substring retraction set, the three-line
//`correctionHeaderCue` announcement window, the adjacent-run fold, the
//markdown-decoration and heading skips, and the supersession arm that let a
//LATER line replace the answer. Under invariant 000 the reply is a PLAN line
//and an action line; a second action line, a heading before the label, or a
//retraction written between two answers is a PROTOCOL VIOLATION. The engine
//still parses what it can - the first usable line executes, so no decision is
//thrown away over line order - but nothing here reads prose for a verdict and
//nothing tells the model that a correction paragraph will be honoured.
//  - the FIRST usable CLEAN candidate is the answer; with no clean candidate the
//    first usable one stands (#W50-Y D7's "an unclean trailer is taken only when
//    no clean line exists", read first-order);
//  - a rejection line ("CHOICE: 3 is wrong") is not an answer and is counted;
//  - `extraAnswerLines` (out) is how many usable answer lines the reply wrote
//    BEYOND the one that ran. Non-zero is a violation, and the seam re-asks.
//Returns the index of the answer, or -1 when nothing is usable.
int gptSelectAnswerIndex(const std::vector<bool>& usable,
                                const std::vector<bool>& clean)
{
    for (size_t i = 0; i < usable.size(); i++)
        if (usable[i] && clean[i])
            return (int) i;
    for (size_t i = 0; i < usable.size(); i++)
        if (usable[i])
            return (int) i;
    return -1;
}


//#W75-CJ (P9, deck126 MED-3, engine-seat MED-1). A PRESENT LABEL IS READ WHERE
//IT IS. `125v126` seq 154 answered
//`PLAN: Play a land to expand mana base and prepare to cast Chromatic Lantern. CHOICE: 1 (Play Sunpetal Grove)`
//- both labelled parts, in order, one CHOICE, a legal number and its short name
//- and the decision went to the heuristic because the label did not start a
//line. This is NOT prose tolerance (invariant 000): nothing here reads an
//unlabelled sentence, weighs a verdict out of the reply's words, or licenses a
//correction paragraph; it reads a label the model wrote, inside the two
//labelled parts the protocol asks for. The golden protocol text is unchanged -
//the reply shape asked for is still PLAN line, then the CHOICE on its own line.
//The gate is deliberately narrow, and every clause is a MUST-NOT-MATCH:
//  - only the CHOICE label (combat labels inside a CHOICE deliberation are CoT);
//  - the line must START with the PLAN marker, so this can only ever fire inside
//    the plan part, never in free prose;
//  - the label must occur EXACTLY ONCE in the whole reply - a second CHOICE
//    anywhere is the multi-answer violation and is left to the line scan;
//  - it must be whitespace-anchored, so "...MYCHOICE: 2" is not a label;
//  - the tail after the answer must be CLEAN (choiceLineIsClean) - prose after
//    the answer is refused exactly as it is on a line-leading label.
bool gptInlineChoiceOnPlanLine(const string& text, size_t& segStart,
                                      size_t& segEnd, size_t& lineStart)
{
    static const char kLabel[] = "CHOICE:";
    const size_t len = sizeof(kLabel) - 1;
    //Exactly one label in the whole reply.
    size_t only = string::npos, count = 0;
    for (size_t i = 0; i + len <= text.size(); i++)
    {
        bool m = true;
        for (size_t k = 0; k < len && m; k++)
            m = (toupper((unsigned char) text[i + k]) == kLabel[k]);
        if (!m)
            continue;
        count++;
        only = i;
        if (count > 1)
            return false;
    }
    if (count != 1)
        return false;
    //Whitespace-anchored (or at the very start of the reply).
    if (only > 0 && text[only - 1] != ' ' && text[only - 1] != '\t')
        return false;
    //Its physical line must begin with the line-leading PLAN marker, and the
    //label must come after it.
    const size_t planPos = firstLineLeadingPlanPos(text);
    if (planPos == string::npos || planPos >= only)
        return false;
    size_t ls = text.rfind('\n', only);
    ls = (ls == string::npos) ? 0 : ls + 1;
    if (planPos < ls)
        return false; //the PLAN marker is on an earlier line
    size_t lineEnd = text.find('\n', only);
    const size_t end = (lineEnd == string::npos) ? text.size() : lineEnd;
    const string payload = text.substr(only + len, end - (only + len));
    if (isTemplatePlaceholderLine(payload) || isExampleEchoLine(payload))
        return false;
    //#W75-CM (F3, Astra wave-75 review finding 3). A LABEL WITH NO CODED HEAD IS
    //NOT AN ANSWER. `choiceLineIsClean` and `choiceLineIsRejection` both DECLINE
    //TO JUDGE a payload with no digit head (`codedHeadEnd` -> npos returns
    //"clean"/"not a rejection"), so `PLAN: Do not take CHOICE: not 2 (Cast Rorix
    //Bladewing)` passed both gates and the downstream parser could bind the
    //parenthetical name or rescue the embedded 2 - executing the cast the reply
    //explicitly refused. On a LINE-LEADING label that risk is the legacy
    //name-form reply this project still accepts; INSIDE the plan line it is
    //prose, and prose is what this gate exists to refuse. So the inline form
    //requires the coded head the protocol asks for: whitespace, then a digit run.
    {
        const size_t h = payload.find_first_not_of(" \t");
        if (h == string::npos || !isdigit((unsigned char) payload[h]))
            return false;
    }
    if (AIPlayerGPT::choiceLineIsRejection(payload) || !AIPlayerGPT::choiceLineIsClean(payload))
        return false;
    segStart = only + len;
    segEnd = end;
    //The LABEL'S OWN OFFSET, not the line's start: the caller uses this both to
    //pick the plan marker that belongs to the answer's block and to bound the
    //carried plan. Handing back the line start would put the answer INSIDE the
    //plan text and carry it to the next window.
    lineStart = only;
    return true;
}

bool findAnswerLabelLine(const string& text, const char * expectedLabel,
                                size_t& segStart, size_t& segEnd, size_t& labelLineStart,
                                int * extraAnswerLines, int * rejectedLines,
                                size_t * lastHeadLineStart)
{
    static const char * kAnswerLabels[] = { "CHOICE:", "ATTACK:", "BLOCKS:", "PUT:" };
    const int kNumAnswerLabels = (int) (sizeof(kAnswerLabels) / sizeof(kAnswerLabels[0]));
    bool found = false;
    size_t lineStart = 0;
    int rejected = 0;
    std::vector<GptAnswerHead> heads;
    if (extraAnswerLines)
        *extraAnswerLines = 0;
    while (lineStart <= text.size())
    {
        size_t lineEnd = text.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? text.size() : lineEnd;
        size_t s = lineStart;
        //#W70-BM (E2): whitespace only - indentation is not prose, decoration is.
        while (s < end && (text[s] == ' ' || text[s] == '\t'))
            s++;
        for (int li = 0; li < kNumAnswerLabels; li++)
        {
            if (expectedLabel && strcmp(kAnswerLabels[li], expectedLabel) != 0)
                continue; //restricted to this decision's own answer label
            size_t len = strlen(kAnswerLabels[li]);
            if (end - s >= len)
            {
                bool match = true;
                for (size_t k = 0; k < len && match; k++)
                    match = (toupper((unsigned char) text[s + k]) == kAnswerLabels[li][k]);
                if (match && !isTemplatePlaceholderLine(text.substr(s, end - s))
                    && !isExampleEchoLine(text.substr(s, end - s)))
                {
                    bool isChoice = (strcmp(kAnswerLabels[li], "CHOICE:") == 0);
                    //#W50-Y D7: a rejection line is not an answer - skipped whole.
                    if (isChoice && AIPlayerGPT::choiceLineIsRejection(text.substr(s + len, end - (s + len))))
                    {
                        rejected++;
                        break;
                    }
                    GptAnswerHead h;
                    h.segStart = s + len;
                    h.segEnd = end;
                    h.lineStart = lineStart;
                    h.isChoice = isChoice;
                    h.clean = isChoice
                              && AIPlayerGPT::choiceLineIsClean(text.substr(s + len, end - (s + len)));
                    heads.push_back(h);
                    found = true;
                }
            }
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    if (rejectedLines)
        *rejectedLines = rejected;
    if (lastHeadLineStart)
        *lastHeadLineStart = heads.empty() ? string::npos : heads.back().lineStart;
    if (heads.empty())
    {
        //#W75-CJ (P9): no line-leading label, and no rejection line either -
        //the last place a PRESENT label can be is the PLAN's own line.
        if (rejected == 0 && expectedLabel && strcmp(expectedLabel, "CHOICE:") == 0
            && gptInlineChoiceOnPlanLine(text, segStart, segEnd, labelLineStart))
        {
            if (extraAnswerLines)
                *extraAnswerLines = 0;
            if (lastHeadLineStart)
                *lastHeadLineStart = labelLineStart;
            return true;
        }
        return found; //only rejection lines (or nothing at all)
    }
    std::vector<bool> vUsable(heads.size(), true), vClean;
    bool anyClean = false;
    for (size_t i = 0; i < heads.size(); i++)
    {
        vClean.push_back(heads[i].clean);
        if (heads[i].clean)
            anyClean = true;
    }
    const int sel = gptSelectAnswerIndex(vUsable, vClean);
    size_t ans = (sel < 0) ? 0 : (size_t) sel;
    //#W50-Y D7's counter, first-order: an unclean CHOICE head that lost to the
    //clean answer is a line the scan refused, and is signed as one.
    if (anyClean)
        for (size_t i = 0; i < heads.size(); i++)
            if (heads[i].isChoice && !heads[i].clean && i != ans)
                rejected++;
    if (rejectedLines)
        *rejectedLines = rejected;
    if (extraAnswerLines)
        *extraAnswerLines = (int) heads.size() - 1; //#W70-BM (E2): the violation
    segStart = heads[ans].segStart;
    segEnd = heads[ans].segEnd;
    labelLineStart = heads[ans].lineStart;
    return true;
}


//#W79-DD: THE ANSWER SELECTOR - the one entry every seam's reply parsing goes
//through. `consumePlan` calls it, and so does PARSETEST's `answerSegmentStatic`,
//so a case in the corpus is exercising the code the ask seam runs and not a
//mirror of it (the wave-64 lesson: two scanners that "agree" about the same
//bytes eventually do not).
//Step 1 is the labelled walk, unchanged. Step 2 fires ONLY when step 1 found no
//answer line AND refused no rejection line: the label-less action line, read
//under `w79LabellessAnswerLine`'s five unambiguity clauses (owner ruling
//2026-09-12 - make the parser robust; any plan must precede action). A reply
//that wrote only rejection lines still yields no answer, exactly as #W50-Y D7
//left it - a refusal is not an answer with its label missing.
static bool gptAnswerSegment(const string& text, const char * expectedLabel,
                             size_t& segStart, size_t& segEnd, size_t& labelLineStart,
                             int * extraAnswerLines, int * rejectedLines,
                             size_t * lastHeadLineStart, bool * labellessRead)
{
    if (labellessRead)
        *labellessRead = false;
    int rejected = 0;
    const bool got = findAnswerLabelLine(text, expectedLabel, segStart, segEnd, labelLineStart,
                                         extraAnswerLines, &rejected, lastHeadLineStart);
    if (rejectedLines)
        *rejectedLines = rejected;
    if (got || rejected > 0)
        return got;
    size_t bareStart = string::npos, bareEnd = 0;
    if (!w79LabellessAnswerLine(text, firstLineLeadingPlanPos(text),
                                codedAnswerCount(text) > 0, &bareStart, &bareEnd))
        return false;
    //The WHOLE line is the payload - there is no label to skip past. `parseChoice`
    //then validates the row number and the echoed short name, so an out-of-range
    //row and a name no option carries still fail here exactly as they do with a
    //label. `labelLineStart` is the line's own start, which bounds the plan carry
    //in consumePlan: the action line is never folded back into the plan.
    segStart = bareStart;
    segEnd = bareEnd;
    labelLineStart = bareStart;
    if (extraAnswerLines)
        *extraAnswerLines = 0;
    if (lastHeadLineStart)
        *lastHeadLineStart = bareStart;
    if (labellessRead)
        *labellessRead = true;
    return true;
}


//WHICH "PLAN:" IS THE CURRENT PLAN (wave-34 #3, N-36e). Case-insensitive, and
//the colon is required so prose like "I plan to attack" cannot truncate the
//reply. The CONSUMER anchors LAST: a reply that stated a plan, kept
//deliberating and then stated a REVISED plan carries two markers, and taking
//the first carried the ABANDONED plan forward while discarding the committed
//one - with the abandoned text running on through the retracted deliberation,
//which is how 53% of deck36's prompts came to carry a >400-char plan field
//(vs105 s35: markers at char 501 and char 5,359; the next prompt was fed
//1,424 chars beginning at 501). parseChoice already anchors LAST on the answer
//label and is right; every consumer of a reply now agrees. A marker that sits
//AFTER the trailing answer label is part of that line's tail, not a new plan,
//so it does not supersede. `firstOut` receives the FIRST marker, which is what
//the reply-SHAPE tests want (did the reply lead with its plan?) and what
//postPlanOverrun keeps using - as a TAIL METER the first plan is the right
//origin, and it deliberately does not move.
static size_t findPlanMarker(const string& text, size_t labelLineStart, size_t * firstOut)
{
    //#W65-AO (G8, deck130 HIGH-2). THE PLAN COMES FROM THE ANSWER'S OWN BLOCK.
    //130v146 seq 47 is a two-block reply: `CHOICE: 1 (Deal 2 damage ...)` + its
    //PLAN, then a paragraph of re-reading, then `CHOICE: 3 (Draw 1 ...)`. The
    //answer came from one block and the plan from the other, so the reasoning the
    //reply DISCARDED was quoted back as YOUR PLAN at the next windows (11 of 424).
    //With the answer now first-wins (#W65-AO above), "the same block" is exactly:
    //the last marker BEFORE the answer line if the reply led with its plan, and
    //otherwise the FIRST marker after it - never a marker belonging to a block
    //whose answer did not run. `first` (the reply-SHAPE probe postPlanOverrun
    //meters from) is unchanged.
    size_t pos = string::npos, first = string::npos, firstAfter = string::npos;
    for (size_t i = 0; i + 5 <= text.size(); i++)
    {
        if (tolower((unsigned char) text[i]) == 'p' && tolower((unsigned char) text[i + 1]) == 'l'
            && tolower((unsigned char) text[i + 2]) == 'a' && tolower((unsigned char) text[i + 3]) == 'n'
            && text[i + 4] == ':')
        {
            if (first == string::npos)
                first = i;
            if (labelLineStart == string::npos || i < labelLineStart)
                pos = i;
            else if (firstAfter == string::npos)
                firstAfter = i;
        }
    }
    if (pos == string::npos)
        pos = firstAfter;
    if (firstOut)
        *firstOut = first;
    return pos;
}


//#W54-A (D12a): the plan's bound is a SHAPE, not a byte count. The protocol
//asks for "a few sentences"; 1,600 bytes is far more than that, and a byte
//bound cuts wherever the arithmetic lands. The served block is the pilot's own
//prose - bounding it removes no legal window, no row and no option. Two cuts,
//in order: the first BLANK LINE, and the first line that STARTS A NEW SENTENCE
//(the line before it ended in . ! or ?) and does not open with a connective, so
//an ordinary wrapped sentence and a "Then attack." continuation both survive.
//The old 1,600-byte trim stays behind this as a backstop for a single runaway
//paragraph.
static bool planLineOpensWithConnective(const string& line)
{
    size_t s = line.find_first_not_of(" \t\r");
    if (s == string::npos)
        return false;
    string w;
    for (size_t i = s; i < line.size(); i++)
    {
        char c = (char) tolower((unsigned char) line[i]);
        if (!isalpha((unsigned char) c))
            break;
        w += c;
    }
    static const char * conn[] = {
        "and", "but", "or", "nor", "so", "then", "yet", "because", "since", "if",
        "unless", "until", "while", "after", "before", "once", "also", "plus",
        "however", "therefore", "thus", "next", "otherwise", "meanwhile", "with",
        "to", "for", "that", "which", "this", "these", "those", "it", "they"
    };
    for (size_t i = 0; i < sizeof(conn) / sizeof(conn[0]); i++)
        if (w == conn[i])
            return true;
    return false;
}



//#W64-AJ (F13, engine seat HIGH-1). A CODED ANSWER LINE ENDS THE PLAN
//PARAGRAPH. `planParagraphBound`'s two terminators are a blank line and a
//sentence boundary without a connective, and neither fires on the shape the
//protocol calls a second answer: a plan whose last line does not close a
//sentence, followed at column 0 by `ATTACK: A4`. That line is then INSIDE the
//block, so #W63-AD's exclusion demotes it - a top-level answer silently loses
//to an earlier one, with no note saying so (`plan_answer_line_ignored` was
//written 0 times in the wave-63 corpus, so nothing counted the demotions
//either). The bound is narrowed by exactly one rule: a line that IS a coded
//answer line is not plan prose. It never widens the block, so no reply that
//parsed stops parsing; it only moves lines OUT of it, which is the direction
//that turns an answer back into an answer. Same bullet prefixes and same
//payload requirement as `codedAnswerLineAt`, so the two agree on what a coded
//line is. Pure over one line.
static bool lineIsCodedAnswerLine(const string& line)
{
    static const char * kLabels[] = { "choice:", "attack:", "blocks:", "put:" };
    //#W64-AK (R5, wave-64 codex review finding 5): INDENTATION IS NOT PLAN
    //MEMBERSHIP. Wave 64 wrote this skip as `*` and `#` only and called it
    //"column 0, deliberately", but the seam that DECIDES an answer,
    //`findAnswerLabelLine`, skips ' ', '\t', '*' and '#' before it matches a
    //label - and so do `codedAnswerLineAt` and the menu parser. So a reply
    //ending `CHOICE: 1 / PLAN: reconsidering / <two spaces>CHOICE: 2` had its
    //correction RECOGNISED as a coded line by every scanner, kept INSIDE the
    //plan block by this predicate alone, and then demoted by #W63-AD's
    //exclusion: the pilot's own correction lost to the answer it was
    //correcting, and the wave-64 PARSETEST case pinned that as the expectation.
    //The predicate's stated invariant is that a line it calls an answer is a
    //line the answer seam can execute; the fix is to make the skip set the
    //same set, so the two can no longer disagree about the same bytes.
    size_t s = 0;
    while (s < line.size() && (line[s] == ' ' || line[s] == '\t'
                               || line[s] == '*' || line[s] == '#'))
        s++;
    for (size_t k = 0; k < sizeof(kLabels) / sizeof(kLabels[0]); k++)
    {
        const size_t len = strlen(kLabels[k]);
        if (line.size() - s < len)
            continue;
        bool m = true;
        for (size_t q = 0; q < len && m; q++)
            m = (tolower((unsigned char) line[s + q]) == kLabels[k][q]);
        if (!m)
            continue;
        size_t pp = s + len;
        while (pp < line.size() && (line[pp] == ' ' || line[pp] == '\t' || line[pp] == '\r'))
            pp++;
        return pp < line.size(); //a label with no payload is not an answer
    }
    return false;
}

size_t planBlockEndOffset(const string& text, size_t planPos)
{
    if (planPos == string::npos || planPos >= text.size())
        return text.size();
    size_t start = planPos;
    bool first = true;
    string prevLine;
    while (start <= text.size())
    {
        size_t end = text.find('\n', start);
        const size_t lineEnd = (end == string::npos) ? text.size() : end;
        const string line = text.substr(start, lineEnd - start);
        if (!first)
        {
            if (line.find_first_not_of(" \t\r") == string::npos)
                return start; //a blank line ends the paragraph
            size_t pe = prevLine.find_last_not_of(" \t\r");
            const char prevLast = (pe == string::npos) ? '\0' : prevLine[pe];
            const bool prevEnded = (prevLast == '.' || prevLast == '!' || prevLast == '?');
            if (prevEnded && !planLineOpensWithConnective(line))
                return start;
            if (lineIsCodedAnswerLine(line)) //#W64-AJ (F13)
                return start;
        }
        prevLine = line;
        first = false;
        if (end == string::npos)
            break;
        start = end + 1;
    }
    return text.size();
}


//#W63-AD (E6b): consumePlan's plan-answer signal. NONE = no coded line sat in
//the plan block. IGNORED = one did AND a line outside it answered the window, so
//the in-plan line was demoted (the 146v162 seq 41 shape). ONLY_IN_PLAN = every
//coded line was inside the plan block, so the unbounded walk answered after all
//and nothing was lost - the fail-safe, counted rather than assumed.
extern const int kPlanAnswerNoteNone = 0;

extern const int kPlanAnswerExtraLine = 1;


//#W66-AR (H2c): the whole line beginning at `off`, right-trimmed. The refused
//answer line, taken from the SAME string the selector walked - so the record's
//`ignored_line` is bytes the scan saw, not a re-derivation from a `reply` field
//that has already been trimmed past the PLAN.
static string gptLineAtOffset(const string& text, size_t off)
{
    if (off == string::npos || off >= text.size())
        return string();
    const size_t e = text.find('\n', off);
    string ln = text.substr(off, (e == string::npos) ? string::npos : e - off);
    const size_t last = ln.find_last_not_of(" \t\r");
    return (last == string::npos) ? string() : ln.substr(0, last + 1);
}


//#W80-DH (F2): THE ORDER RULE, as one predicate over the two offsets the live
//scanners produce - `gptAnswerSegment`'s answer line start and `findPlanMarker`'s
//marker, in that order, exactly as consumePlan reads them. consumePlan CALLS this;
//it is not a mirror of it. `npos` on either side means the reply is missing that
//part, and a reply missing one of them is not an ordering violation - it is
//handled by the paths that already exist for it (no answer -> re-ask; no plan ->
//the carry stands). Pure over two numbers, so PARSETEST walks every case.
static bool w80AnswerPrecedesPlan(size_t labelLineStart, size_t planMarkerPos)
{
    if (labelLineStart == string::npos || planMarkerPos == string::npos)
        return false;
    return labelLineStart < planMarkerPos;
}


//#W63-AD (E6b): the offset of the FIRST LINE-LEADING "PLAN:" marker - the same
//marker choiceRetractedNoReplacement scans for, factored out so the answer
//selector and the retraction gate agree on where the plan block begins. A
//mid-line "your plan:" quoted out of the prompt is NOT line-leading and is
//ignored, exactly as it is there.
size_t firstLineLeadingPlanPos(const string& text)
{
    size_t lineStart = 0;
    while (lineStart <= text.size())
    {
        size_t lineEnd = text.find('\n', lineStart);
        const size_t end = (lineEnd == string::npos) ? text.size() : lineEnd;
        size_t s = lineStart;
        while (s < end && (text[s] == ' ' || text[s] == '\t'
                           || text[s] == '*' || text[s] == '#' || text[s] == '-'))
            s++;
        if (end - s >= 5
            && tolower((unsigned char) text[s]) == 'p' && tolower((unsigned char) text[s + 1]) == 'l'
            && tolower((unsigned char) text[s + 2]) == 'a' && tolower((unsigned char) text[s + 3]) == 'n'
            && text[s + 4] == ':')
            return s;
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    return string::npos;
}


//#W71-BO (R8): `planParagraphBound` is DELETED - `plan_paragraph_bound_cuts`
//fired 0 times in 40 games (max PLAN line 422 B, p95 191 B, 2 records over 400).
//The plan is ONE line under invariant 000, the answer line is already excluded by
//`planEnd`, and the 400-character carry cut stands.

//#W54-A (D12b): the one-clause diff of the served plan against THIS window's
//option list. 162v152 s11 -> s12 was served, verbatim, a plan naming
//`Cast Master of the Feast` after the row it took in the same breath made
//Master unaffordable, and nothing on the block said so. The clause states a
//fact about the MENU, never a ruling about the card.
string planMenuDiffClause(const string& absentName)
{
    if (absentName.empty())
        return string();
    return string("; \"") + absentName + "\" is no longer on your menu";
}


string AIPlayerGPT::consumePlan(const string& content, const char * expectedLabel,
                                int * choiceRunLen, int * rejectedLines,
                                int * planAnswerNote, string * laterIgnoredLine)
{
    if (choiceRunLen)
        *choiceRunLen = 0;
    if (rejectedLines)
        *rejectedLines = 0;
    if (planAnswerNote)
        *planAnswerNote = kPlanAnswerNoteNone;
    if (laterIgnoredLine)
        laterIgnoredLine->clear();
    //Drop any inline think block first (same as parseChoice).
    string text = content;
    size_t thinkEnd = text.rfind("</think>");
    if (thinkEnd != string::npos)
        text = text.substr(thinkEnd + 8);

    //The reply contract puts the answer AFTER the plan, on a labeled final
    //line (CHOICE:/ATTACK:/BLOCKS:). Head-first answers committed the
    //choice token BEFORE the model reasoned in its PLAN; when the plan
    //concluded a DIFFERENT option for the same window, the stale head
    //stayed locked in (intent-collapse: 4 seats across waves 8-9, one
    //game-losing false Keep). The answer label anchors the parse so plan
    //prose full of numbers cannot hijack it - the reason head-first existed.
    //#W82-A (audit-2026-09): the comment said "find the LAST label"; the live
    //selector is `gptSelectAnswerIndex` and its rule is FIRST USABLE AND CLEAN,
    //else first usable - first-clean, not last. (The header comment above that
    //function states the rule correctly; this prose contradicted it.)
    //Line-leading answer-label lines (template placeholders skipped, as
    //in the deck62 N7-template fix). `expectedLabel` restricts the match to
    //this decision's own label so a CoT combat line ("Attack: ...") inside a
    //CHOICE deliberation is not mistaken for the answer (stale-echo family B).
    size_t answerStart = string::npos, answerEnd = 0, labelLineStart = string::npos;
    {
        size_t ss = 0, se = 0, ls = 0;
        //#W63-AD (E6b): the answer is not read out of the PLAN BLOCK - the span
        //mCurrentPlan carries and the prompt quotes back as YOUR PLAN. Bounded
        //walk first; if the plan block holds the reply's ONLY coded line the
        //unbounded walk still answers the window (the exclusion demotes, it
        //never deletes), and the seam signs which of the two happened.
        //#W70-BM (E2): the plan-block DEMOTION and its two-walk reconciliation
        //are DELETED. They existed because a reply could write a coded line
        //inside a rambling PLAN paragraph; a PLAN line that is a SEQUENCE OF
        //INTENDED ACTIONS cannot contain a line-leading answer label without
        //being a second answer line, which is a violation the scan now counts.
        //#W82-A (audit-2026-09): ...and so are the two locals that fed it. They
        //were computed (one of them running `planBlockEndOffset`) and then
        //explicitly discarded. `planBlockEndOffset` itself has other callers.
        size_t lastHead = string::npos;
        int extraLines = 0;
        //#W79-DD: the selector, labelled walk then the label-less action line.
        const bool got = gptAnswerSegment(text, expectedLabel, ss, se, ls,
                                          &extraLines, rejectedLines, &lastHead, NULL);
        if (got && extraLines > 0)
        {
            //#W70-BM (E2): a SECOND answer line. The first one runs (a decision
            //is never thrown away over line count) and the shape is stamped as
            //the violation it is; the seam's re-ask ladder asks once more.
            if (planAnswerNote)
                *planAnswerNote = kPlanAnswerExtraLine;
            if (laterIgnoredLine && lastHead != string::npos && lastHead != ls)
                *laterIgnoredLine = gptLineAtOffset(text, lastHead);
        }
        if (got)
        {
            answerStart = ss;
            answerEnd = se;
            labelLineStart = ls;
        }
        else if (rejectedLines && *rejectedLines > 0)
            return string(); //#W50-Y D7: every coded line was a rejection - no answer
    }

    //The current plan: LAST marker (see findPlanMarker). firstPos is the
    //reply-shape probe used further down, not the plan.
    size_t firstPos = string::npos;
    size_t pos = findPlanMarker(text, labelLineStart, &firstPos);
    //#W80-DH (F2, Astra wave-80 review finding 2 - HIGH). AN ANSWER THAT PRECEDES
    //THE PLAN IS NOT AN ANSWER. OWNER RULING (2026-09-12, verbatim): "any plan,
    //must precede action. Mechanically, for an llm, a plan that follows a choice
    //is post hoc justification instead of load bearing reasoning." The LABEL-LESS
    //reader has enforced this since #W79-DD (its fifth unambiguity clause); the
    //LABELLED walk did not, so `CHOICE: 2 (Cast Wall of Omens)\nPLAN: block and
    //draw.` was accepted and executed, and the order was recorded as telemetry
    //only. It is the ORDER that is refused, never the parser's tolerance: the
    //answer is dropped and the seam re-asks it on exactly the path a missing
    //answer takes (`replyLabelMissing` -> the one re-ask), so no legal option is
    //removed, capped or auto-answered - the same window comes back.
    //`findPlanMarker` already anchors the plan to the answer's own block: `pos`
    //is the last marker BEFORE the answer line when the reply led with its plan,
    //and only otherwise the first marker after it. So `pos > labelLineStart` is
    //exactly "this reply stated no plan before its action line".
    //The PLAN is still folded below - a plan the model wrote is a plan, and the
    //re-asked window is served it; only the action is refused.
    bool w80ActionBeforePlan = false;
    if (answerStart != string::npos && w80AnswerPrecedesPlan(labelLineStart, pos))
    {
        w80ActionBeforePlan = true;
        mActionBeforePlanRejected = true;
        answerStart = string::npos;
        answerEnd = 0;
        labelLineStart = string::npos;
        DebugTrace("AIPlayerGPT: the reply wrote its action line ABOVE its PLAN line -"
                   " a plan must precede the action, so the answer is refused and the"
                   " window is re-asked");
    }
    (void) w80ActionBeforePlan;
    if (pos == string::npos)
    {
        //#W70-BM (E1): the model omitted its PLAN, so the carried sequence stands.
        //#W70-BN (F10): parsing consumes NOTHING - the step is consumed when the
        //window's action executes (writeTransLog).
        //No plan stated: keep the previous one. The answer segment (when
        //labeled) is still the decision.
        if (answerStart != string::npos)
            return text.substr(answerStart, answerEnd - answerStart);
        return string();
    }

    //The plan ends where a trailing answer label begins - the answer line
    //must not be re-fed as plan text at the next decision.
    size_t planEnd = (labelLineStart != string::npos && labelLineStart > pos)
                     ? labelLineStart : string::npos;
    string plan = (planEnd == string::npos) ? text.substr(pos + 5)
                                            : text.substr(pos + 5, planEnd - (pos + 5));
    size_t s = plan.find_first_not_of(" \t\r\n");
    size_t e = plan.find_last_not_of(" \t\r\n");
    if (s != string::npos)
    {
        plan = plan.substr(s, e - s + 1);
        //#W71-BO (R8): the paragraph bound and its census are DELETED.
        //(history: comment-archaeology.md AIPlayerGPTParse-gptcaveat-planCarryComposeSteps-1839)
        //#W60-Q (R8): COMPOSE the two passes, do not run them blind. The bound
        //appends a marker that ends in ']', which the stump trim below then
        //reads as an unfinished sentence and cuts back through - deleting the
        //very marker the bound promised, on any plan over the bound with a
        //sentence end past its midpoint (the common case). The stump trim
        //exists for a max_tokens cut, and a plan the bound already ended at a
        //chosen boundary is not that: when the bound fired, its marker IS the
        //terminator and the trim is skipped.
        //A reply cut off by max_tokens leaves a mid-word stump ("...value
        //by sac", observed live); trim back to the last complete sentence -
        //but only when the bound above did not already choose the cut and mark
        //it. Both passes now live in planCarryCompose, so the composed result
        //is what PARSETEST checks.
        //#W70-BM (E1): the STEP-bounded compose. Same scratchpad cut, same
        //character ceiling, but the cut lands at the end of a step and the note
        //names how many later steps it dropped - so a three-step plan can never
        //be served as its first step and a half.
        plan = gptcaveat::planCarryComposeSteps(plan, kPlanCarryMaxSteps, kPlanCarryMaxChars);
        //#W49-U D7: count verbatim echoes of the plan already carried.
        //#W70-BM (E1): STEP ONE IS CONSUMED BY THE ACTION. The action line IS
        //step one of the plan, so a window whose reply RESTATES the same plan has
        //already executed the step the previous window was served: the carry
        //advances and the NEXT window is served the remainder. A CHANGED plan is
        //a new sequence and starts again at step one. The pointer is clamped in
        //planStepsAfter so a fully-walked plan still carries its last step rather
        //than vanishing (a blank carry reads as "no plan yet" and asks for one).
        //#W70-BN (F10): a CHANGED plan is a new sequence and starts at step one;
        //a carried plan keeps the pointer it has. Neither advances here - the
        //advance belongs to the executed action (writeTransLog).
        if (plan != mCurrentPlan)
        {
            mPlanStepsDone = 0;
            mPlanCastCompletionState = 0; mPlanCastOpenName.clear(); //#W80-DG (U1), #W80-DH (F5)
        }
        //#W53-N (D12a): the model WROTE a plan line here, whatever it says -
        //that, not its content, is what the age stamp reports.
        mPlanSetSeq = mTransSeq;
        mPlanSetTurn = observer ? observer->turn : 0;
        mCurrentPlan = plan;
        //#W67-AY (I6, deck123 HIGH-4): ONE SOURCE for the row's verdict and the
        //refusal. The stop and the count the reply states are persisted here,
        //where every reply's plan is folded, and outlive the CARRY - which is
        //cleared by a refusal and by every caveat gate above, and was therefore
        //absent from 19 of the 20 windows the verdict clause was built for.
        //Persisted, never cleared: a stop the pilot stated is the last thing it
        //said about its own stop until it says another.
        {
            int stStop = -1, stNow = -1;
            if (repeatPlanStopAndCurrent(plan, &stStop, &stNow) && repeatPlanStopIsOwn(plan))
            {
                mStatedStop = stStop;
                //#W74-CE (O7c): and the life total it was computed against.
                mStatedStopOppLife = (observer && opponent()) ? opponent()->life : -1;
                //#W72-BX (F2): the stop carries its OWN date. `mPlanSetTurn` is
                //stamped by EVERY plan line, so a plan that states no stop at all
                //re-dated a stop from an earlier turn and let it collapse this
                //turn's windows. Set here and nowhere else: only a stop parsed
                //out of THIS reply's PLAN line is a stop stated this turn.
                mStatedStopTurn = observer ? observer->turn : -1;
            }
        }
        //#W82-A (audit-2026-09): the duplicate `mPlanSetTurn`/`mCurrentPlan`
        //assignment that stood here (a verbatim repeat of the pair eleven lines
        //above, left by an edit - the intervening block changes neither value)
        //is DELETED.
    }
    //Labeled answer (the contract): the decision is the label line's
    //remainder, wherever the label sits relative to the plan. An answer
    //wrapped onto the next line ("CHOICE:\n2") leaves the label line
    //empty - extend to the rest of the reply rather than failing.
    if (answerStart != string::npos)
    {
        string ans = text.substr(answerStart, answerEnd - answerStart);
        bool hasContent = false;
        for (size_t k = 0; k < ans.size() && !hasContent; k++)
            hasContent = isalnum((unsigned char) ans[k]) != 0;
        if (!hasContent)
            ans = text.substr(answerStart);
        return ans;
    }
    //#W70-BM (E2): NO LABEL, whatever came before the plan. The reply wrote a
    //PLAN line and no action line - the length cap cut it, or it never wrote
    //one. Either way nothing here is a decision, and the LEGACY HEAD path that
    //read the bytes before the marker as one is DELETED with the 300-byte ramble
    //guard that bounded it. The seams re-ask.
    (void) firstPos;
    return string();
}


//#W80-DH (F5): the one place the open cast step closes, so the latch, the name and
//the turn stamp can never disagree. Counted, and traced, so a corpus can adjudicate
//whether a cast step closed on its own event or on the belt.
void AIPlayerGPT::w80CloseOpenCastStep(const char * why)
{
    if (mPlanCastCompletionState < 1)
        return;
    mPlanCastCompletionState = 0;
    mPlanCastOpenName.clear();
    mPlanCastOpenTurn = -1;
    mPlanCastStepsClosed++;
    mSkipTrace.note("plan_cast_steps_closed"); //#W81-DK (V15)
    DebugTrace("AIPlayerGPT: the open cast plan step closed - " << (why ? why : ""));
}


//#W54-B (D13). The audit the latch path never had. deck126 vs125 seq 13 and
//14 answered "CHOICE: 2 (Cast Idyllic Tutor)" on a three-row menu and row 1,
//"Cast Perimeter Captain", executed: coded_answers 2, answer_replaced true,
//latched_coded_line "2", parse_note None - the index and the parenthetical
//agreed with EACH OTHER and both disagreed with what ran, and nothing said
//so. 2 of the corpus's 3,253 parentheticals, AT the <= 1/3,000 carry ceiling
//rather than under it.
//Read on the LAST line-leading "CHOICE:" line, which is the line the engine's
//own last-wins rule honours. TRUE only when the head index differs from the
//executed row AND the parenthetical names no part of that row's own text.
//Deliberately silent on the two shapes that look similar and are correct: a
//name-over-index remap (the NAME won, so the parenthetical IS on the executed
//row) and a cosmetic prefix ("Cast Vampire" for "Cast Vampire Nighthawk",
//which the containment test accepts). Pure - no state, PARSETEST-provable.
bool AIPlayerGPT::latchedRowMismatch(const string& reply, int choice, int optionCount,
                                     const std::vector<string> * optionTexts)
{
    if (choice < 1 || !optionTexts || choice > (int) optionTexts->size())
        return false; //row 0 (pass) carries no option text of its own
    string line;
    size_t lineStart = 0;
    while (lineStart <= reply.size())
    {
        size_t lineEnd = reply.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? reply.size() : lineEnd;
        size_t s = lineStart;
        while (s < end && (reply[s] == ' ' || reply[s] == '\t'
                           || reply[s] == '*' || reply[s] == '#' || reply[s] == '-'))
            s++;
        if (end - s >= 7)
        {
            static const char * kLabel = "CHOICE:";
            bool m = true;
            for (int k = 0; k < 7 && m; k++)
                m = (toupper((unsigned char) reply[s + k]) == kLabel[k]);
            if (m)
                line = reply.substr(s + 7, end - (s + 7)); //last one wins
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    if (line.empty())
        return false;
    size_t d = line.find_first_not_of(" \t");
    if (d == string::npos || !isdigit((unsigned char) line[d]))
        return false; //no index on the latched line - nothing to disagree
    int n = atoi(line.c_str() + d);
    if (n < 0 || n > optionCount || n == choice)
        return false; //off the menu entirely, or the index IS what ran
    string name = headParenthetical(line);
    for (size_t i = 0; i < name.size(); i++)
        name[i] = (char) tolower((unsigned char) name[i]);
    size_t a = name.find_last_not_of(" \t.");
    name = (a == string::npos) ? string() : name.substr(0, a + 1);
    if (name.size() < 4 || name == "pass" || name == "none" || name == "hold"
        || name == "done" || name == "skip" || name == "decline" || name == "nobody")
        return false; //a bare index or decline filler - no card name to disagree
    if (stripRenderAnnotationsLc((*optionTexts)[choice - 1]).find(name) != string::npos)
        return false; //the parenthetical is on the row that ran
    string label = optionLabel((*optionTexts)[choice - 1]);
    for (size_t i = 0; i < label.size(); i++)
        label[i] = (char) tolower((unsigned char) label[i]);
    vector<string> nw;
    echoSignificantWords(name, nw);
    if (!nw.empty())
    {
        bool all = true;
        for (size_t k = 0; k < nw.size() && all; k++)
            all = label.find(nw[k]) != string::npos;
        if (all)
            return false; //every naming word of the parenthetical is on that row
    }
    return true;
}


//#W58-A (D3): read the computed numbers in the verdict grammar instead of
//maintaining a list of English verbs. The emitters currently produce drains,
//damage, life, draws, prevents, destroys, exiles and deals; this predicate also
//covers the next `<verb> 0` form without another whitelist edit. A second live
//magnitude, including the sweeper's bare `, N of yours`, keeps the row live.
//Only the computed core before ` - ` is scanned: later numbers explain the
//result (remaining life, toughness, and so on), rather than adding an effect.
static bool rightNowComputedMagnitudesAreZero(const string& row)
{
    string clause = rowVerdictClause(row);
    if (clause.empty())
        return false;
    for (size_t i = 0; i < clause.size(); i++)
        clause[i] = (char) tolower((unsigned char) clause[i]);
    size_t colon = clause.find(':');
    if (colon == string::npos)
        return false;
    size_t end = clause.find(" - ", colon + 1);
    if (end == string::npos)
        end = clause.size();
    bool found = false;
    size_t segment = colon + 1;
    while (segment < end)
    {
        while (segment < end && isspace((unsigned char) clause[segment]))
            segment++;
        size_t next = clause.find(',', segment);
        if (next == string::npos || next > end)
            next = end;
        size_t number = segment;
        if (number < next && !isdigit((unsigned char) clause[number])
            && clause[number] != '+' && clause[number] != '-')
        {
            size_t first = number;
            while (number < next && isalpha((unsigned char) clause[number]))
                number++;
            string firstWord = clause.substr(first, number - first);
            while (number < next && isspace((unsigned char) clause[number]))
                number++;
            //#W58-A (D3): only a subject-led fact (`they control 0`, `you
            //control 0`) has two words before its magnitude. An emitter such
            //as `makes a 0/0` describes a real object and must stay live.
            if ((firstWord == "they" || firstWord == "you") && number < next
                && !isdigit((unsigned char) clause[number])
                && clause[number] != '+' && clause[number] != '-')
            {
                while (number < next && isalpha((unsigned char) clause[number]))
                    number++;
                while (number < next && isspace((unsigned char) clause[number]))
                    number++;
            }
        }
        if (number < next && (clause[number] == '+' || clause[number] == '-'))
            number++;
        if (number < next && isdigit((unsigned char) clause[number]))
        {
            found = true;
            while (number < next && isdigit((unsigned char) clause[number]))
            {
                if (clause[number] != '0')
                    return false;
                number++;
            }
        }
        size_t arrow = clause.find("->", segment);
        if (arrow != string::npos && arrow < next)
        {
            arrow += 2;
            while (arrow < next && !isdigit((unsigned char) clause[arrow]))
                arrow++;
            while (arrow < next && isdigit((unsigned char) clause[arrow]))
            {
                found = true;
                if (clause[arrow] != '0')
                    return false;
                arrow++;
            }
        }
        segment = next + 1;
    }
    return found;
}


//#W58-A (D3), preserving #W54-B (D14): an explicit renderer conclusion and
//the Morbid status remain semantic no-op facts; computed verdicts use the
//number grammar above. A real magnitude must never match. Pure.
//(history: comment-archaeology.md AIPlayerGPTParse-rowNamesALiveKill-1850)
static bool rowNamesALiveKill(const string& row)
{
    //The victim-tag openers `removalVictimTag` and the magnitude emitter can
    //produce (`spellRemovalVerb` returns only "kills" or "removes"), each with
    //its split THEIRS/YOURS form. Every other `{word: ...}` tag is left alone.
    static const char * kVictimTag[] = {
        "{kills: ", "{removes: ",
        "{kills whichever you target: ", "{removes whichever you target: "
    };
    for (size_t t = 0; t < sizeof(kVictimTag) / sizeof(kVictimTag[0]); t++)
    {
        const string open(kVictimTag[t]);
        for (size_t p = row.find(open); p != string::npos; p = row.find(open, p + 1))
        {
            const size_t payload = p + open.size();
            const size_t close = row.find('}', payload);
            if (close == string::npos)
                continue;
            const size_t first = row.find_first_not_of(" \t", payload);
            if (first != string::npos && first < close)
                return true;
        }
    }
    return false;
}


//#W71-BQ (L3, second half - the deck126 evidence): a no-op phrase inside a
//CONDITIONAL is a statement about a board that does not exist yet.
//`edictOnlyVictimOnStackClause` appends "...; if it is gone when this resolves
//they control 0 creatures and this does nothing" to a verdict whose own
//operative scope names a 17/15 being sacrificed for 15 life - and the guard
//refused that row twice (deck126 vs125 seq 131 and 133) while the IDENTICAL row
//was accepted at seq 132 and paid the 15. The scope a phrase belongs to runs
//back to the nearest `;` or brace, exactly as `verdictReadsZero` splits scopes;
//a scope that opens a hypothetical states no verdict about now.
static bool phraseScopeIsConditional(const string& low, size_t hit)
{
    size_t start = 0;
    for (size_t i = hit; i > 0; i--)
    {
        const char c = low[i - 1];
        if (c == ';' || c == '{' || c == '}')
        {
            start = i;
            break;
        }
    }
    static const char * kIf[] = { "if ", "unless ", "would " };
    for (size_t k = 0; k < sizeof(kIf) / sizeof(kIf[0]); k++)
    {
        const string w(kIf[k]);
        for (size_t i = start; i + w.size() <= hit; i++)
        {
            if (low.compare(i, w.size(), w) != 0)
                continue;
            if (i == start || low[i - 1] == ' ' || low[i - 1] == '(' || low[i - 1] == '-')
                return true;
        }
    }
    return false;
}


//(b) A phrase is a VERDICT only where the renderer states one: at the top level
//of the row. "does not apply" is emitted in exactly one place - inside the
//Morbid qualifier of a real magnitude, `{right now: -1/-1 (no creature has died
//this turn, so Morbid does NOT apply)}` - which says WHICH of two magnitudes
//applies, never that the row does nothing; and a quoted `{card text: "..."}`
//blob is the card's printed words, not the engine's conclusion about this
//board. So the cue counts only at parenthesis depth 0 and outside double
//quotes. This is the same unit rule `verdictScopeOperative`/`verdictReadsZero`
//already apply one layer down; `rowSaysNoOp` is still the single zero-predicate.
static bool noOpPhraseIsAVerdict(const string& low, const char * phrase)
{
    const size_t n = strlen(phrase);
    int depth = 0;
    bool quoted = false;
    for (size_t i = 0; i < low.size(); i++)
    {
        const char c = low[i];
        if (c == '"')
        {
            quoted = !quoted;
            continue;
        }
        if (quoted)
            continue;
        if (c == '(')
        {
            depth++;
            continue;
        }
        if (c == ')')
        {
            if (depth > 0)
                depth--;
            continue;
        }
        if (!depth && i + n <= low.size() && low.compare(i, n, phrase) == 0
            && !phraseScopeIsConditional(low, i))
            return true;
    }
    return false;
}


//#W71-BS (F9, Astra review finding 9). A ROW IS A NO-OP ONLY IF EVERY BRANCH IS.
//The renderer writes one verdict per targeting branch, separated at top level by
//semicolons: `{right now: they control 1 creature - Rorix is sacrificed; YOU
//control 0 creatures - targeting yourself does nothing}`. One branch doing nothing
//is not the row doing nothing - that row is a live removal spell with a useless
//second target - and calling it a no-op earned the useful cast a no-op re-ask that
//argued the seat out of it. Conditional branches ("if they gain one before this
//resolves...") are about a board that does not exist and are not operative; they
//never rescue a row and never condemn one, which is the same unit rule
//phraseScopeIsConditional already applies inside a branch.
static void w71TopLevelBranches(const string& low, vector<string>& out)
{
    int depth = 0;
    bool quoted = false;
    string cur;
    for (size_t i = 0; i < low.size(); i++)
    {
        const char c = low[i];
        if (c == '"')
            quoted = !quoted;
        else if (!quoted && c == '(')
            depth++;
        else if (!quoted && c == ')' && depth > 0)
            depth--;
        else if (!quoted && !depth && c == ';')
        {
            out.push_back(cur);
            cur.clear();
            continue;
        }
        cur += c;
    }
    out.push_back(cur);
}


static bool w71BranchIsConditional(const string& branch)
{
    size_t a = branch.find_first_not_of(" \t-");
    if (a == string::npos)
        return true; //nothing operative in it
    return branch.compare(a, 3, "if ") == 0;
}


static bool w71EveryBranchIsANoOp(const string& low)
{
    vector<string> branches;
    w71TopLevelBranches(low, branches);
    if (branches.size() < 2)
        return true; //one verdict: the phrase already IS the row's conclusion
    bool anyOperative = false;
    for (size_t i = 0; i < branches.size(); i++)
    {
        if (w71BranchIsConditional(branches[i]))
            continue;
        anyOperative = true;
        if (!noOpPhraseIsAVerdict(branches[i], "does nothing")
            && !noOpPhraseIsAVerdict(branches[i], "does not apply"))
            return false; //this branch does something - the row is not a no-op
    }
    return anyOperative;
}


//#W72-BT (M2, wave-71 engine-seat HIGH-2 + deck125 A-1). A PERMANENT'S ZERO CAN
//BE PROSPECTIVE, AND A PROSPECTIVE ZERO IS NOT A NO-OP. 3 of the corpus's 4
//fallbacks were this shape: `Cast Lightmine Field {right now: they control 0
//creatures able to attack - deals 0 until they have an attacker ...}` (125v152
//seq 32, 125v123 seq 7) and `Cast Lightning Greaves {right now: you control 0
//creatures - this equips nothing}` (123v152 seq 10). Both rows put a PERMANENT
//on the battlefield, where it waits for the condition its own verdict says is
//not met yet; the model's PLAN was the set-up line and the re-ask argued it out
//of it. The magnitude scanner reads only the operative core before the first
//" - ", so what it actually read in both cases was the PRECONDITION ("they/you
//control 0 ..."), never an effect of the row.
//
//So the rescue asks for BOTH halves of that shape, and nothing else:
//  (a) every digit-bearing segment of the operative core is a SUBJECT-LED BOARD
//      COUNT ("they control 0 ...", "you control 0 ...") - a fact about the
//      board, not a magnitude this row produces. `Cast Supreme Verdict {right
//      now: destroys 0 of their creatures (0 without ...), 0 of yours - ...}`
//      fails here on both segments and stays the genuine no-op it is (125v152
//      seq 64, the one TRUE fire of the four); and
//  (b) a later segment carries a PROSPECTIVE cue - "until", "when", "whenever",
//      or the equipment renderer's "equips nothing" - i.e. the row's own text
//      says the zero is about a condition that has not happened YET.
//A row with an unqualified zero EFFECT ("deals 0", "destroys 0") is untouched by
//this, and so is every row that reaches the phrase path above. The miss
//direction is deliberately the seat's own answer standing: the harm measured is
//a FALSE refusal (a spent round trip that talked the pilot off its own plan),
//never a missed one - a row taken is a legal row the model chose.
//Pure over the rendered clause, so every face is pinned without a game.
static bool w72CoreZerosAreBoardCounts(const string& core)
{
    //Parentheticals qualify a magnitude, they do not state one (the
    //`verdictScopeOperative` rule); their digits are not this test's business.
    string flat;
    int depth = 0;
    for (size_t i = 0; i < core.size(); i++)
    {
        const char c = core[i];
        if (c == '(')
            depth++;
        else if (c == ')')
        {
            if (depth > 0)
                depth--;
        }
        else if (!depth)
            flat += c;
    }
    bool sawADigit = false;
    size_t seg = 0;
    while (seg <= flat.size())
    {
        size_t next = flat.find(',', seg);
        if (next == string::npos)
            next = flat.size();
        string piece = flat.substr(seg, next - seg);
        bool hasDigit = false;
        for (size_t i = 0; i < piece.size(); i++)
            if (isdigit((unsigned char) piece[i]))
                hasDigit = true;
        if (hasDigit)
        {
            sawADigit = true;
            size_t a = piece.find_first_not_of(" \t");
            if (a == string::npos)
                return false;
            const string head = piece.substr(a);
            if (head.compare(0, 5, "they ") != 0 && head.compare(0, 4, "you ") != 0)
                return false; //an effect magnitude, not a board count
        }
        seg = next + 1;
    }
    return sawADigit;
}


static bool w72ProspectiveZeroOnAPermanent(const string& row)
{
    const string clause = rowVerdictClause(row);
    if (clause.empty())
        return false;
    string low = clause;
    for (size_t i = 0; i < low.size(); i++)
        low[i] = (char) tolower((unsigned char) low[i]);
    const size_t colon = low.find(':');
    if (colon == string::npos)
        return false;
    string body = low.substr(colon + 1, low.size() - colon - 2); //drop the closing brace
    const size_t split = body.find(" - ");
    if (split == string::npos)
        return false; //no explanatory tail: nothing states a prospect
    const string core = body.substr(0, split);
    const string tail = body.substr(split + 3);
    if (!w72CoreZerosAreBoardCounts(core))
        return false;
    static const char * kProspect[] = { " until ", "when ", "whenever ", "equips nothing" };
    for (size_t k = 0; k < sizeof(kProspect) / sizeof(kProspect[0]); k++)
        if (tail.find(kProspect[k]) != string::npos)
            return true;
    return false;
}



bool AIPlayerGPT::rowSaysNoOp(const string& row)
{
    //#W71-BQ (L3): 3 of 3 seat fires were false at deck123 - every non-Morbid
    //Tragic Slip cast row matched on the Morbid clause's "does NOT apply" while
    //the same row printed `{kills: Elite Spellbinder, Intrepid Adversary}`, and
    //at 152 seq 13 -> 14 the re-ask's "says it does nothing" wording talked the
    //model out of two printed kills. A row the engine says kills something is
    //not a row that does nothing.
    if (rowNamesALiveKill(row))
        return false;
    string low = row;
    for (size_t i = 0; i < low.size(); i++)
        low[i] = (char) tolower((unsigned char) low[i]);
    if (noOpPhraseIsAVerdict(low, "does nothing")
        || noOpPhraseIsAVerdict(low, "does not apply"))
        return w71EveryBranchIsANoOp(low);
    //#W72-BT (M2): a computed zero that is a PRECONDITION plus a prospective
    //cue is a permanent waiting for its condition, not a row that does nothing.
    if (rightNowComputedMagnitudesAreZero(row))
        return !w72ProspectiveZeroOnAPermanent(row);
    return false;
}



//All-digit tokens of a string ("add 5 counters" -> {"5"}). The alpha filter
//above drops them; the number is often the ONLY discriminator between
//near-identical options ("add 5 counters" vs "add 10 counters" share every
//alpha word - the deck152 vs139 s19 echo/index conflict).
static void numericTokens(const string& seg, vector<string>& out)
{
    string w;
    bool digitsOnly = true;
    for (size_t i = 0; i <= seg.size(); i++)
    {
        char c = (i < seg.size()) ? seg[i] : ' ';
        if (isalnum((unsigned char) c))
        {
            if (!isdigit((unsigned char) c))
                digitsOnly = false;
            w += c;
        }
        else
        {
            if (!w.empty() && digitsOnly)
                out.push_back(w);
            w.clear();
            digitsOnly = true;
        }
    }
}


//Does this option text contain numTok as an EXACT numeric token? A substring
//find would let "5" match the "15" in another option.
static bool optionHasNumericToken(const string& optionText, const string& numTok)
{
    vector<string> toks;
    numericTokens(optionText, toks);
    for (size_t i = 0; i < toks.size(); i++)
        if (toks[i] == numTok)
            return true;
    return false;
}


//Split at top-level commas only (commas inside (...)/[...]/{...} belong to
//names and annotations, not to a pick list).
static void topLevelCommaSegments(const string& text, vector<string>& out)
{
    int depth = 0;
    string cur;
    for (size_t i = 0; i <= text.size(); i++)
    {
        char c = (i < text.size()) ? text[i] : ',';
        if (c == '(' || c == '[' || c == '{')
            depth++;
        else if (c == ')' || c == ']' || c == '}')
        {
            if (depth > 0)
                depth--;
        }
        if ((c == ',' && depth == 0) || i == text.size())
        {
            size_t s = cur.find_first_not_of(" \t\r\n");
            size_t e = cur.find_last_not_of(" \t\r\n");
            out.push_back(s == string::npos ? string() : cur.substr(s, e - s + 1));
            cur.clear();
        }
        else
            cur += c;
    }
    if (!out.empty() && out.back().empty())
        out.pop_back();
}


//Resolve one name phrase ("Pest #1") to the UNIQUE option it names, or -1.
//All significant words must appear in the option; among several same-worded
//matches a "#N" handle in the phrase picks the option whose own text carries
//that exact handle (the render prints "#N" on duplicate names, so the
//round-trip is faithful - this is NOT uniqueNameMatch's Nth-match rule,
//which a non-duplicate incidental match would derail: "Pest" also matches
//the Nuisance Engine line that mints Pest tokens).
static int resolveNamedOption(const string& phrase, const vector<string>& optionTexts)
{
    vector<string> words;
    echoSignificantWords(phrase, words);
    if (words.empty())
        return -1;
    vector<int> matches;
    for (size_t o = 0; o < optionTexts.size(); o++)
    {
        string low = optionTexts[o];
        for (size_t i = 0; i < low.size(); i++)
            low[i] = (char) tolower((unsigned char) low[i]);
        bool all = true;
        for (size_t k = 0; k < words.size() && all; k++)
            all = low.find(words[k]) != string::npos;
        if (all)
            matches.push_back((int) o);
    }
    if (matches.size() == 1)
        return matches[0];
    if (matches.size() > 1)
    {
        int ord = nameOrdinal(phrase);
        if (ord >= 1)
        {
            std::ostringstream h;
            h << "#" << ord;
            string handle = h.str();
            int hit = -1;
            for (size_t m = 0; m < matches.size(); m++)
            {
                const string& t = optionTexts[matches[m]];
                size_t p = t.find(handle);
                //exact handle: the char after "#N" must not extend the number
                if (p != string::npos
                    && (p + handle.size() >= t.size() || !isdigit((unsigned char) t[p + handle.size()])))
                {
                    if (hit >= 0)
                        return -1; //two options carry the same handle: ambiguous
                    hit = matches[m];
                }
            }
            return hit;
        }
    }
    return -1;
}


//Append a parse-shape signature, semicolon-joined, no duplicates.
void appendParseNote(std::string * noteOut, const char * sig)
{
    if (!noteOut || !sig)
        return;
    if (noteOut->find(sig) != string::npos)
        return;
    if (!noteOut->empty())
        *noteOut += ";";
    *noteOut += sig;
}


//#W68-BB (J4, deck125): the ANNOUNCE_X menu is the one menu whose row NAMES
//are numbers - `X = 4` at row 1, `X = 2` at row 4. `125v162` s90 answered
//`CHOICE: 2 (X = 2)`: the coded index says row 2 (X = 4) and the name says row
//4 (X = 2), which the generic rule scores as a number/name disagreement and
//re-asks (`index_name_conflict`, 1 of the corpus's 7 fallbacks; s91 recovered
//with `CHOICE: 4 (X = 2)`, one wasted round trip). But on THIS menu the name is
//not an echo of a row's prose - it is the value the model is announcing, and it
//is unique on the list, so it is an answer, not an ambiguity: the same reading
//`#W66-AS` gives the hold row's own unique label. Recognised from the rows
//themselves (two or more `X = <digits>` rows, every other row a decline/hold),
//so no seam has to declare what it is. Pure over the option list.
static bool menuIsBareXAnnounce(const std::vector<string> * options)
{
    if (!options)
        return false;
    size_t xRows = 0;
    for (size_t i = 0; i < options->size(); i++)
    {
        const string& r = (*options)[i];
        if (r.compare(0, 4, "X = ") == 0 && r.size() > 4 && isdigit((unsigned char) r[4]))
        {
            xRows++;
            continue;
        }
        if (r.compare(0, 7, "Decline") == 0
            || r.compare(0, 13, "Hold priority") == 0) //#W72-BW (M23b)
            continue;
        return false; //a row this menu class does not have: not an X menu
    }
    return xRows >= 2;
}


int AIPlayerGPT::parseChoice(const string& content, int optionCount,
                             const std::vector<string> * optionTexts,
                             bool * staleEcho,
                             const std::string * pendingSource,
                             std::string * noteOut,
                             bool passRowOffered)
{
    if (staleEcho) *staleEcho = false;
    //Drop any inline think block first.
    string text = content;
    size_t thinkEnd = text.rfind("</think>");
    if (thinkEnd != string::npos)
        text = text.substr(thinkEnd + 8);

    //Protocol-example parrot (belt-and-suspenders for the example-leak, deck133
    //vs131 s21 class). A coded token whose parenthetical verbatim-echoes the
    //reply protocol's worked example (kExampleFakeCardLc) is a copied format
    //string, not a decision, so exclude it from index resolution. This is the
    //universal choke point: findAnswerLabelLine segments, salvage lines and the
    //retraction walk all resolve through parseChoice, so guarding here closes the
    //real consumePlan flow (not just the salvage path). The de-fang makes the
    //example name NON-LIVE, so this is inert today (no real option is named it) -
    //it keeps the resolver safe should a FUTURE example text again collide with a
    //live option (the exact wave-24 shape). A drop routes to the safe heuristic,
    //and salvageLoopedChoice still recovers any real sibling line.
    if (isExampleEchoLine(text))
        return -1;

    //Name-echo reconciliation: the answer line carries the chosen option's
    //name in parentheses ("CHOICE: 2 (Cast Fatal Push)"). When the plan
    //fixates on a card that is not among the options, the trailing index
    //mis-maps in BOTH directions - casting over a hold and holding over an
    //offered cast (waves 10-11, 2 seats, ~1.3-1.9% of decisions, one
    //wasted kill shot). The echo makes the intent checkable: an index
    //whose option does not contain the echoed words remaps to the UNIQUE
    //option that does (this also repairs out-of-range indices), and an
    //echo matching nothing parses as FAIL - the heuristic answers, never
    //an arbitrary cast.
    int echoRemap = -1;
    bool echoConflict = false;
    bool echoNoMatch = false;
    bool passEchoNamed = false; //#W53-N (D9): the parenthetical names row 0
    bool holdEchoNamed = false; //#W54-A (D2a): the parenthetical names the HOLD row
    const int holdRowIdx = holdRowIndexOf(optionTexts); //#W54-A (D2a), 0-based
    bool exactNameRemap = false; //#W52-J D6: bound by the exact short name (hoisted for the conflict signatures)
    vector<string> words; //echo's significant words (hoisted for INDEX-WINS)
    string echoLc;        //lowercased echo string (hoisted for INDEX-WINS (a2))
    vector<int> echoMatches; //every option the echo's alpha words match
                             //(hoisted so a conflict note can test whether the
                             //index's own option is among them)
    if (optionTexts && !optionTexts->empty())
    {
        //the LAST "(...)" following a digit on the answer-ish tail
        size_t close = text.rfind(')');
        size_t open = (close == string::npos) ? string::npos : text.rfind('(', close);
        //#W50-Y D8 (wave-49 ledger MED; deck123 vs130 seq 31): when the line
        //is "<digits> (<name>)" the parenthetical that MATTERS is the one
        //opening right after the index, taken to its BALANCED close - so a
        //name carrying its own parentheses ("Goblin (1/1)", the row's own
        //rendered short name and the prompt's worked example) is the whole
        //echo, not the inner "(1/1" the walk-back below would settle on.
        {
            size_t hd = text.find_first_not_of(" \t");
            if (hd != string::npos && isdigit((unsigned char) text[hd]))
            {
                while (hd < text.size() && isdigit((unsigned char) text[hd]))
                    hd++;
                while (hd < text.size() && (text[hd] == ' ' || text[hd] == '\t'))
                    hd++;
                if (hd < text.size() && text[hd] == '(')
                {
                    int depth = 0;
                    for (size_t k = hd; k < text.size(); k++)
                    {
                        if (text[k] == '(')
                            depth++;
                        else if (text[k] == ')' && --depth == 0)
                        {
                            open = hd;
                            close = k;
                            break;
                        }
                    }
                }
            }
        }
        //NESTED PARENTHETICAL REPAIR. Walking back from the last ')' finds the
        //INNERMOST '(' when the echoed name carries its own parentheses -
        //"CHOICE: 1 (Nadaar, Selfless Paladin #1 (5/5) [vigilance])" yielded
        //the echo "5/5) [vigilance]", i.e. the render furniture with the card
        //NAME cut off, so the staleness test ran on the wrong string. If the
        //captured span contains an unmatched ')', the real opener is further
        //left; step back until the span balances.
        while (open != string::npos && open > 0)
        {
            int depth = 0;
            bool unmatched = false;
            for (size_t q = open + 1; q < close && !unmatched; q++)
            {
                if (text[q] == '(')
                    depth++;
                else if (text[q] == ')')
                {
                    if (depth == 0)
                        unmatched = true;
                    else
                        depth--;
                }
            }
            if (!unmatched)
                break;
            size_t prev = text.rfind('(', open - 1);
            if (prev == string::npos || prev == open)
                break;
            open = prev;
        }
        if (open != string::npos && close != string::npos && close > open + 1)
        {
            string echo = text.substr(open + 1, close - open - 1);
            //Target-menu spell-name prefix (stale-echo family A, wave-22).
            //Target sub-menus instruct "answer with the TARGET's name (not
            //'<spell>')", yet the model routinely echoes "<spell> targeting
            //<target>" (deck14 vs27 s29 "Unsummon targeting Inkfathom
            //Infiltrator"; deck62 vs14 s29 "Web targeting Yavimaya
            //Enchantress"). The spell prefix injected words absent from the
            //option AND the option's rules-text words were absent from the
            //echo, so BOTH echo passes missed and a CORRECT in-range index was
            //wrongly downgraded to stale_echo (the heuristic then bounced /
            //Web'd a worse target). When the parenthetical reads "<X>
            //targeting <Y>" and X is the pending target source, strip to <Y>
            //so the match runs on the target name. ANCHORED to the actual
            //source name - never a blanket "targeting" strip.
            if (pendingSource && !pendingSource->empty())
            {
                string el = echo;
                for (size_t i = 0; i < el.size(); i++)
                    el[i] = (char) tolower((unsigned char) el[i]);
                size_t tp = el.find(" targeting ");
                if (tp != string::npos)
                {
                    string x = el.substr(0, tp);
                    size_t xs = x.find_first_not_of(" \t");
                    size_t xe = x.find_last_not_of(" \t");
                    if (xs != string::npos)
                        x = x.substr(xs, xe - xs + 1);
                    string src = *pendingSource;
                    for (size_t i = 0; i < src.size(); i++)
                        src[i] = (char) tolower((unsigned char) src[i]);
                    if (!x.empty() && (src.find(x) != string::npos || x.find(src) != string::npos))
                        echo = echo.substr(tp + 11); //strlen(" targeting ") == 11
                }
            }
            //Hoist the (post-strip) lowercased echo for INDEX-WINS branch (a2).
            echoLc = echo;
            for (size_t i = 0; i < echoLc.size(); i++)
                echoLc[i] = (char) tolower((unsigned char) echoLc[i]);
            //#W53-N (D9): "Pass" IS the short name of row 0 wherever row 0 is
            //on the menu, so the same short-label rule lane G applied to "Cast
            //nothing right now" applies here - the NAME resolves the answer and
            //the index does not get to win. deck146 vs152 seq 91 answered
            //"CHOICE: 1 (Pass)" on a Kaya menu and the engine used the +1: the
            //only mis-execution in 3,063 CHOICE: n (name) parentheticals.
            if (passRowOffered && isReservedPassEcho(echoLc))
                passEchoNamed = true;
            //#W54-A (D2a): the HOLD row's own name binds the same way, at BOTH
            //seams (the casting menu carries the row and no row 0). A reserved
            //pass echo that is also a head of the hold row's text is AMBIGUOUS
            //after D2d and must let the coded index break the tie rather than
            //silently take row 0.
            if (holdRowIdx >= 0 && isReservedHoldEcho(echoLc))
                holdEchoNamed = true;
            //#W52-J (D6): the EXACT short name binds FIRST. The word pass below
            //drops "right"/"now" as render vocabulary and "cast" as filler, so
            //"Cast nothing right now" reduced to the single word "nothing" -
            //which the sibling row's "{right now: ... at 0 this does nothing}"
            //annotation also carries - and a unique row read as an ambiguous
            //conflict; the index then executed a dead Idyllic Tutor (deck123
            //vs125 seq 48). A parenthetical that IS the head of exactly one
            //row's annotation-stripped core (at a word boundary) names that
            //row, whatever its words; several rows sharing the head (the
            //"Plains" of "Plains #2 / #3") fall through to the word pass
            //unchanged. Decline filler alone ("pass", "none") never binds here.
            {
                string e = echoLc;
                size_t es = e.find_first_not_of(" \t");
                size_t ee = e.find_last_not_of(" \t.");
                e = (es == string::npos) ? string() : e.substr(es, ee - es + 1);
                if (e.size() >= 4 && e != "pass" && e != "none" && e != "hold" && e != "done"
                    && e != "skip" && e != "decline" && e != "nobody")
                {
                    vector<int> exact;
                    for (size_t o = 0; o < optionTexts->size(); o++)
                    {
                        string core = stripRenderAnnotationsLc((*optionTexts)[o]);
                        for (size_t k = 0; k < core.size(); k++)
                            core[k] = (char) tolower((unsigned char) core[k]);
                        size_t cs = core.find_first_not_of(" \t");
                        if (cs == string::npos)
                            continue;
                        core = core.substr(cs);
                        if (core.compare(0, e.size(), e) != 0)
                            continue;
                        //a word-boundary head of ANY row is a candidate: "Cast
                        //Vampire" heads both "Cast Vampire" and "Cast Vampire
                        //Nighthawk" (two candidates -> not unique -> the word
                        //pass and its index-wins pin decide, unchanged); "Cast
                        //nothing right now" heads only "Cast nothing right now
                        //(combat comes next this turn)".
                        if (core.size() == e.size() || !isalnum((unsigned char) core[e.size()]))
                            exact.push_back((int) o);
                    }
                    if (exact.size() == 1)
                    {
                        echoRemap = exact[0] + 1;
                        exactNameRemap = true;
                    }
                }
            }
            //significant words: lowercase, length >= 4
            string w;
            for (size_t i = 0; i <= echo.size(); i++)
            {
                char c = (i < echo.size()) ? (char) tolower((unsigned char) echo[i]) : ' ';
                if (isalnum((unsigned char) c))
                    w += c;
                else
                {
                    //Decline/pass filler is NOT a card name: an echo of
                    //"(pass)" / "(none)" / "(hold)" / "(done)" carries no
                    //significant words, so it can never read as a stale
                    //absent-echo (task constraint ii). Real card names never
                    //reduce to these tokens.
                    if (w.size() >= 4 && w != "cast" && w != "with" && w != "play"
                        && w != "pass" && w != "none" && w != "hold" && w != "done"
                        && w != "skip" && w != "decline" && w != "nobody"
                        && !isRenderVocabWord(w))
                        words.push_back(w);
                    w.clear();
                }
            }
            if (exactNameRemap)
            {
                //#W52-J (D6): bound by the exact short name above - the word
                //pass and its conflict/staleness verdicts do not run.
            }
            else if (!words.empty())
            {
                int match = -1;
                //#W52-G (D-2): tier 1 - the echo's LABEL words against each
                //row's LABEL (optionLabel: annotations and mana furniture cut
                //from both sides). Tier 2 - the old full-text pass - runs only
                //when no label matches at all, so a reply that echoes nothing
                //but annotation vocabulary still binds to the one row carrying
                //it; it can no longer make a row ambiguous with a row whose
                //NAME the model actually wrote.
                {
                    vector<string> labelWords;
                    echoSignificantWords(optionLabel(echo), labelWords);
                    if (!labelWords.empty())
                        for (size_t o = 0; o < optionTexts->size(); o++)
                        {
                            string low = optionLabel((*optionTexts)[o]);
                            for (size_t i = 0; i < low.size(); i++)
                                low[i] = (char) tolower((unsigned char) low[i]);
                            bool all = true;
                            for (size_t k = 0; k < labelWords.size() && all; k++)
                                all = low.find(labelWords[k]) != string::npos;
                            if (all)
                                echoMatches.push_back((int) o);
                        }
                }
                //#W54-B (D15): tier 2 is the FULL row minus its annotations,
                //not the raw row. Its purpose stands - a reply that echoes
                //nothing a label carries still reaches the row text past the
                //first annotation - but a name that occurs ONLY inside
                //"[finds only ...]" / "{kills: ...}" can no longer bind it.
                if (echoMatches.empty())
                    for (size_t o = 0; o < optionTexts->size(); o++)
                    {
                        string low = stripRenderAnnotationsLc((*optionTexts)[o]);
                        for (size_t i = 0; i < low.size(); i++)
                            low[i] = (char) tolower((unsigned char) low[i]);
                        bool all = true;
                        for (size_t k = 0; k < words.size() && all; k++)
                            all = low.find(words[k]) != string::npos;
                        if (all)
                            echoMatches.push_back((int) o);
                    }
                if (echoMatches.size() == 1)
                    match = echoMatches[0];
                else if (echoMatches.size() > 1)
                    echoConflict = true; //not unique on alpha words alone
                //Numeric disambiguation (W36 item 4, deck152 vs139 s19): the
                //alpha words of "add 5 counters" match every "add N counters"
                //option - the NUMBER is the discriminator the >=4-length
                //filter drops. Strictly narrowing: it only runs where pass 1
                //already conflicted (where index-wins used to stand silently),
                //and only an exact numeric-token match on a UNIQUE survivor
                //converts the conflict into a remap.
                if (echoConflict)
                {
                    vector<string> nums;
                    numericTokens(echoLc, nums);
                    if (!nums.empty())
                    {
                        int survivor = -1;
                        bool multiple = false;
                        for (size_t m = 0; m < echoMatches.size() && !multiple; m++)
                        {
                            bool allNums = true;
                            for (size_t k = 0; k < nums.size() && allNums; k++)
                                allNums = optionHasNumericToken(optionLabel((*optionTexts)[echoMatches[m]]), nums[k]); //#W52-G: label, not {right now: N} numbers
                            if (allNums)
                            {
                                if (survivor >= 0)
                                    multiple = true;
                                else
                                    survivor = echoMatches[m];
                            }
                        }
                        if (survivor >= 0 && !multiple)
                        {
                            match = survivor;
                            echoConflict = false;
                        }
                    }
                }
                //Fallback (option-subset-of-echo): the all-echo-words-in-
                //option pass above misses a SUPERSTRING echo - the model
                //named the card in FULLER detail than the offered option
                //("Attack with Yawgmoth, Thran Physician" echoing the option
                //"Attack with Yawgmoth"; deck133 vs140 s37 was wrongly ruled
                //stale and downgraded to the heuristic). Reverse the
                //containment: an option whose OWN significant words all appear
                //in the echo is the intended pick (extra detail, not a
                //different card). Unique match only - zero or multiple keep
                //the no-match / conflict verdict, so a genuinely stale echo
                //(none of its words subset any option) still routes to the
                //heuristic.
                if (match < 0 && !echoConflict)
                {
                    string echoLow = echo;
                    for (size_t i = 0; i < echoLow.size(); i++)
                        echoLow[i] = (char) tolower((unsigned char) echoLow[i]);
                    for (size_t o = 0; o < optionTexts->size(); o++)
                    {
                        //significant words of THIS option (same rule as the
                        //echo split above: lowercase, length >= 4, minus the
                        //verb/decline filler)
                        vector<string> ow;
                        string t;
                        //E-49a (deck21 s4): a PLAYER-target option carries the
                        //volatile "(player, life N)" suffix (describeTarget:
                        //"The opponent (player, life 20)"). Its words "player"
                        //and "life" are absent from an echo that names the
                        //spell+target ("Cast Lightning Bolt targeting The
                        //opponent"), so the option-subset test failed and a
                        //CORRECT answer was wrongly downgraded to stale_echo.
                        //Strip that suffix before extracting the option's anchor
                        //words - life is not identifying. Scoped to this exact
                        //pattern; index-wins and the uniqueness guard are intact.
                        string src = optionLabel((*optionTexts)[o]); //#W52-G: the row's anchor words are its label's
                        {
                            size_t pl = src.find("(player, life ");
                            if (pl != string::npos)
                            {
                                size_t close = src.find(')', pl);
                                if (close != string::npos)
                                    src.erase(pl, close - pl + 1);
                            }
                        }
                        for (size_t k = 0; k <= src.size(); k++)
                        {
                            char c = (k < src.size()) ? (char) tolower((unsigned char) src[k]) : ' ';
                            if (isalnum((unsigned char) c))
                                t += c;
                            else
                            {
                                if (t.size() >= 4 && t != "cast" && t != "with" && t != "play"
                                    && t != "pass" && t != "none" && t != "hold" && t != "done"
                                    && t != "skip" && t != "decline" && t != "nobody")
                                    ow.push_back(t);
                                t.clear();
                            }
                        }
                        if (ow.empty())
                            continue; //no anchor words -> cannot subset-match
                        bool all = true;
                        for (size_t k = 0; k < ow.size() && all; k++)
                            all = echoLow.find(ow[k]) != string::npos;
                        if (all)
                        {
                            if (match >= 0) { match = -1; echoConflict = true; break; } //not unique
                            match = (int) o;
                        }
                    }
                }
                if (match >= 0)
                    echoRemap = match + 1; //1-based option number
                else if (!echoConflict)
                    echoNoMatch = true; //named words matched NO offered option
            }
            else if (!echoLc.empty()) //(exactNameRemap is false here)
            {
                //AMOUNT-MENU echo (W36 item 4, deck152 vs139 s19: "CHOICE: 11
                //(add 5 counters)" executed option 11 = "add 10 counters").
                //Every alpha word of such an echo is render vocabulary
                //("counters") or under the 4-length floor ("add"), so the
                //significant-words pass above sees NOTHING and the whole
                //reconciliation is skipped - the NUMBER, the one
                //discriminator, was being dropped with the furniture.
                //Relaxed pass: length>=3 alpha tokens (vocabulary allowed,
                //substring match) plus numeric tokens (exact-token match,
                //so "5" cannot ride the "15" in a sibling option). Requires
                //at least one of EACH, so a bare stat echo "(3/3)" can never
                //remap, and only a UNIQUE full match does anything - this
                //pass is widen-only and sets no staleness signals.
                vector<string> nums;
                numericTokens(echoLc, nums);
                vector<string> alphaToks;
                {
                    string t;
                    bool hasAlpha = false;
                    for (size_t k = 0; k <= echoLc.size(); k++)
                    {
                        char c = (k < echoLc.size()) ? echoLc[k] : ' ';
                        if (isalnum((unsigned char) c))
                        {
                            if (!isdigit((unsigned char) c))
                                hasAlpha = true;
                            t += c;
                        }
                        else
                        {
                            if (t.size() >= 3 && hasAlpha)
                                alphaToks.push_back(t);
                            t.clear();
                            hasAlpha = false;
                        }
                    }
                }
                if (!nums.empty() && !alphaToks.empty())
                {
                    vector<int> relaxed;
                    //#W52-G: labels first (tier 0), the full row only when no
                    //label carries the tokens (tier 1) - same two tiers as the
                    //significant-words pass above.
                    for (int tier = 0; tier < 2 && relaxed.empty(); tier++)
                    for (size_t o = 0; o < optionTexts->size(); o++)
                    {
                        //#W54-B (D15): tier 1 is annotation-stripped too
                        string low = tier == 0 ? optionLabel((*optionTexts)[o])
                                               : stripRenderAnnotationsLc((*optionTexts)[o]);
                        for (size_t i2 = 0; i2 < low.size(); i2++)
                            low[i2] = (char) tolower((unsigned char) low[i2]);
                        bool all = true;
                        for (size_t k = 0; k < alphaToks.size() && all; k++)
                            all = low.find(alphaToks[k]) != string::npos;
                        for (size_t k = 0; k < nums.size() && all; k++)
                            all = optionHasNumericToken(low, nums[k]);
                        if (all)
                            relaxed.push_back((int) o);
                    }
                    if (relaxed.size() == 1)
                        echoRemap = relaxed[0] + 1;
                    else if (relaxed.size() > 1)
                    {
                        //several full matches: no remap, but the conflict is
                        //REAL - export it so an index foreign to every match
                        //gets the ambiguous-conflict signature downstream.
                        echoConflict = true;
                        echoMatches = relaxed;
                    }
                }
            }
        }
    }

    //#W53-N (D9): the parenthetical NAMES row 0. This binds ahead of every
    //index and staleness verdict below, for the same reason lane G's exact
    //short name binds ahead of the word pass: "Pass" is not an off-menu name
    //where a pass row exists, it is that row's own label, and the word-pass
    //below reads its only significant word ("priority") as matching no option
    //and fails the whole reply to the heuristic. deck146 vs152 seq 91 executed
    //Kaya's +1 on "CHOICE: 1 (Pass)" - the sole mis-execution in 3,063
    //parentheticals. A coded 0 already IS this row, so it earns no note.
    //#W54-A (D2a): the first coded number in the reply, shared by the two
    //reserved-name branches below (the index that would otherwise win).
    int firstCodedNum = -1;
    for (size_t pn = 0; pn < text.size(); pn++)
        if (isdigit((unsigned char) text[pn]))
        {
            size_t pe = pn;
            while (pe < text.size() && isdigit((unsigned char) text[pe]))
                pe++;
            firstCodedNum = atoi(text.substr(pn, pe - pn).c_str());
            break;
        }

    //#W54-A (D2a): the parenthetical NAMES the HOLD row. This binds ahead of
    //every index verdict for the same reason the pass name does: on a menu
    //carrying the row, its short name is not an off-menu name, it is that
    //row's own label. The failure this repairs is the reverse of D9's -
    //`CHOICE: 0 (Hold priority)` executed a plain PASS both times it was
    //written (125v146 seq 74, 146v125 seq 372), because the index-wins branch
    //refuses to remap a coded 0. Where the index disagrees, the NAMED row is
    //taken and the divergence is stamped; the third thing is never executed.
    if (holdEchoNamed)
    {
        if (firstCodedNum != holdRowIdx + 1)
        {
            appendParseNote(noteOut, "hold_row_named");
            appendParseNote(noteOut, "echo_index_conflict");
            //#W65-AO (G8, deck130 HIGH-1). 130v162 seq 57 wrote `CHOICE: 0
            //(pass)` and closed `CHOICE: 0 (Hold priority)`; the record reads
            //`choice: 4`. The number and the name pointed at different rows and
            //the NAME won silently - the third thing the reply never wrote. The
            //named row stays the answer of last resort (D9/D2a), but the
            //disagreement now earns the SAME one re-ask #W52-J D6 gives the
            //ambiguous shape, before anything executes.
            //(history: comment-archaeology.md AIPlayerGPTParse-appendParseNote-1863)
            appendParseNote(noteOut, "index_name_unique_name");
        }
        return holdRowIdx + 1;
    }

    if (passEchoNamed)
    {
        int firstNum = firstCodedNum;
        //#W55-A (D11a): an EXACT row label beats a prefix. A reserved pass echo
        //IS row 0's own name, so it no longer stamps merely for heading another
        //row's text (13 of wave 54's 16 stamps, all executing row 0 correctly).
        //One shape stays genuinely ambiguous and keeps the stamp: the coded
        //INDEX names the HOLD row while the echo names row 0 - the two halves of
        //one reply disagree, and the index is the only half that tells the rows
        //apart (146v126 seq 54/56/57, all executed on the hold row). The answer
        //is still never a third row.
        if (holdRowIdx >= 0 && firstNum == holdRowIdx + 1)
        {
            appendParseNote(noteOut, "pass_hold_ambiguous");
            appendParseNote(noteOut, "index_name_conflict"); //#W65-AO (G8)
            return holdRowIdx + 1;
        }
        if (firstNum != 0)
        {
            appendParseNote(noteOut, "pass_row_named");
            //#W66-AS (deck123 MED): row 0's own reserved label is unique too -
            //stamped, resolved, not re-asked. The genuinely ambiguous shape (the
            //index names the HOLD row while the echo names row 0) keeps the
            //conflict stamp and the re-ask, one branch above.
            appendParseNote(noteOut, "index_name_unique_name");
        }
        return 0;
    }

    //INDEX-WINS (wave-23 ITEM A). A well-formed, in-range POSITIVE index is
    //only DISCARDED as a stale echo when the echoed name is genuinely
    //out-of-context: FOREIGN to the option the index points at AND not a
    //reference to the decision's own source card. A dual-face label ("Transform:
    //Search for Azcanta" vs option "Transform:azcanta, the sunken ruin"), an
    //invented verb ("Activate Tergrid's Lantern" vs "Flip Side with Tergrid's
    //Lantern"), or a parameter-menu prefix ("Cast <spell> with X=1" vs the bare
    //option "X = 1") still shares content with the chosen option (or names the
    //announced spell) and is TRUSTED at its index. Only a wholly stale answer -
    //a prior land drop echoed into a cast menu, whose index maps to a DIFFERENT
    //KIND of option (deck133 forbidden Thoughtseize, deck140 dumped kill shot) -
    //is dropped. This is the family-level root fix that replaces the per-shape
    //echo band-aids: the echo guard now needs the label to match NO current
    //option AND to be foreign to the CHOSEN option before it fires.
    auto echoStaleForIndex = [&](int k1) -> bool {
        if (!echoNoMatch || words.empty())
            return false;
        //(a) the echo shares a significant word with the option the index
        //selects -> the label is consistent with that option, trust the index.
        if (optionTexts && k1 >= 1 && k1 <= (int) optionTexts->size())
        {
            string low = (*optionTexts)[k1 - 1];
            for (size_t li = 0; li < low.size(); li++)
                low[li] = (char) tolower((unsigned char) low[li]);
            for (size_t wi = 0; wi < words.size(); wi++)
                if (low.find(words[wi]) != string::npos)
                    return false;
        }
        //(a2) the CHOSEN option's OWN anchor words ALL appear in the echo -> the
        //echo is a FULLER phrasing that WRAPS this option (the model prefixed the
        //option's verb and/or named the source card). R-STALE-ECHO-QUALIFIER
        //residual, deck137 wave-24 s4: a shockland ETB menu offered the bare
        //options "pay 2 life" / "tap"; the model answered CHOICE: 2 (Tap Temple
        //Garden). Branch (a) missed because the echo's >=4-length words
        //("temple"/"garden") name the SOURCE card, which is absent from the bare
        //option "tap"; branch (b) had no anchor because the menu's source name did
        //not render ("Choose an option for :"). But the echo CONTAINS the option's
        //own label ("tap"), so the answer is coherent -> trust the index. Length
        //>=3 here (not the >=4 significance floor) so a short-but-meaningful label
        //like "tap" counts, and >=1 qualifying option word is REQUIRED so an
        //anchorless option ("X = 1") cannot vacuously pass (that must stay stale
        //without a source anchor - W23-A shape1).
        if (optionTexts && !echoLc.empty() && k1 >= 1 && k1 <= (int) optionTexts->size())
        {
            const string& el = echoLc;
            const string& src = (*optionTexts)[k1 - 1];
            vector<string> ow;
            string t;
            for (size_t k = 0; k <= src.size(); k++)
            {
                char c = (k < src.size()) ? (char) tolower((unsigned char) src[k]) : ' ';
                if (isalnum((unsigned char) c))
                    t += c;
                else
                {
                    if (t.size() >= 3 && t != "cast" && t != "with" && t != "play"
                        && t != "pass" && t != "none" && t != "hold" && t != "done"
                        && t != "the" && t != "for" && t != "skip" && t != "decline"
                        && t != "nobody")
                        ow.push_back(t);
                    t.clear();
                }
            }
            if (!ow.empty())
            {
                bool all = true;
                for (size_t k = 0; k < ow.size() && all; k++)
                    all = el.find(ow[k]) != string::npos;
                if (all)
                    return false; //option label fully echoed -> consistent, trust
            }
        }
        //(a3) Annotation-echo tolerance (deck22 wave-27 R-ANNOTATION-ECHO-PARSE):
        //the model copied a render annotation onto the echoed option name - a
        //bracketed "[changeling: counts as Giant]" tail (deck22 vs131 s3, the
        //stale_echo repro) or the "(land: taps for {B})" land tag this build adds -
        //injecting words foreign to the bare option AND, when the option carries a
        //"(1/1)" that the annotation follows, fooling the paren-extraction above
        //into grabbing the annotation instead of the name. Rescue by comparing
        //annotation-stripped cores: if the CHOSEN option's own core name appears
        //verbatim in the (likewise stripped) reply, the answer references THIS
        //option - trust the index. Strictly widen-only: it needs the chosen
        //option's core in the reply, so a DIFFERENT card's echo cannot cross-match,
        //and it never fires without an already-in-range index. Requires a
        //multi-token core (>= 6 chars, has a space) so a bare short label ("tap")
        //stays with branch (a2) and incidental prose cannot trip it.
        if (optionTexts && k1 >= 1 && k1 <= (int) optionTexts->size())
        {
            string optCore = stripRenderAnnotationsLc((*optionTexts)[k1 - 1]);
            if (optCore.size() >= 6 && optCore.find(' ') != string::npos)
            {
                string replyCore = stripRenderAnnotationsLc(text);
                if (replyCore.find(optCore) != string::npos)
                    return false; //chosen option's core name present -> consistent, trust
            }
        }
        //(b) every echoed word names the decision's own source card (the spell
        //whose X/mode/parameter is being announced) -> a self-reference, not a
        //stale prior answer. Handles the ANNOUNCE_X "Cast <spell> with X=N" echo
        //whose bare "X = N" option carries no anchor words of its own.
        if (pendingSource && !pendingSource->empty())
        {
            string src = *pendingSource;
            for (size_t si = 0; si < src.size(); si++)
                src[si] = (char) tolower((unsigned char) src[si]);
            bool allInSrc = true;
            for (size_t wi = 0; wi < words.size(); wi++)
                if (src.find(words[wi]) == string::npos) { allInSrc = false; break; }
            if (allInSrc)
                return false;
        }
        return true; //label foreign to the chosen option and the source -> stale
    };

    //The reply contract puts the chosen option number FIRST. Prefer an
    //in-range integer at the HEAD of the text ("N", "N.", "N)" - leading
    //whitespace tolerated). Keeping the LAST in-range integer (the
    //previous behavior) let echoed option text hijack the choice: a mana
    //cost {2}{r}{r}, a stat (3/3) or a prose life total that happened to
    //be in range outvoted the model's leading number (8 desyncs across 5
    //corpus games, one thrown-away exact-lethal). Fallback when the head
    //is not a number: the FIRST in-range integer anywhere - never the
    //last; the tail is where echoed stats and prose numbers live. The
    //in-range guard itself stays: taking the trailing digit blindly (the
    //oldest behavior) parsed "(2/4)" echoes as -1 and silently held
    //creatures back.
    size_t i = 0;
    while (i < text.size() && isspace((unsigned char) text[i]))
        i++;
    if (i < text.size() && isdigit((unsigned char) text[i]))
    {
        size_t j = i;
        while (j < text.size() && isdigit((unsigned char) text[j]))
            j++;
        int n = atoi(text.substr(i, j - i).c_str());
        if (n >= 0 && n <= optionCount)
        {
            //A disagreeing unique name-echo outranks the index (the index
            //is the observed failure mode; the name is the intent). A
            //deliberate 0 (pass priority) is never remapped - it carries
            //no option name of its own. The divergence is SIGNED into the
            //translog (W36 item 4) - it used to resolve invisibly.
            if (echoRemap > 0 && !echoConflict && n != 0 && echoRemap != n)
            {
                appendParseNote(noteOut, "echo_index_conflict");
                if (exactNameRemap)
                {
                    appendParseNote(noteOut, "name_over_index"); //#W52-J D6
                    //#W68-BB (J4): on the X menu the name IS the value. Stamped
                    //and resolved to the named row, never re-asked.
                    if (menuIsBareXAnnounce(optionTexts))
                    {
                        appendParseNote(noteOut, "x_value_named");
                        appendParseNote(noteOut, "index_name_unique_name");
                        return echoRemap;
                    }
                    //#W66-AR (MED, engine MED-1). #W65-AO G8/6 routed a
                    //number/name disagreement to ONE re-ask - but only inside
                    //parseChoice's RESERVED-NAME branches (hold/pass), so on an
                    //ordinary list the name still won silently: 130v125 s97 and
                    //s98 executed `Mountain #2` / `Mountain #3` off a coded 2
                    //and 3, `parse_note: echo_index_conflict;name_over_index`,
                    //no conflict note and no re-ask. The protocol says a
                    //conflict is re-asked; enforcing it on 3 windows and not on
                    //these 2 is the surface disagreeing with itself. Same stamp,
                    //same one re-ask at both seams; the named row still stands
                    //as the answer of last resort on the exhausted pass.
                    appendParseNote(noteOut, "index_name_conflict");
                }
                return echoRemap;
            }
            //Ambiguous conflict: the echo matched SEVERAL options and none of
            //them is the index's own - number and name genuinely disagree but
            //no unique name target exists, so the index stands (a re-ask has
            //no channel at this depth); the signature keeps it auditable.
            if (echoConflict && n >= 1 && n <= optionCount && !echoMatches.empty())
            {
                bool indexAmongMatches = false;
                for (size_t m = 0; m < echoMatches.size(); m++)
                    if (echoMatches[m] == n - 1)
                        indexAmongMatches = true;
                if (!indexAmongMatches)
                    appendParseNote(noteOut, "echo_index_conflict_ambiguous");
            }
            //Multi-pick comma list on a coded head ("CHOICE: 8 (Pest #3), 6
            //(Island), ..." - deck36 F2 s40): the FIRST pick already wins by
            //the head parse; stamp the overrun so it is visible (W36 item 2b).
            if (noteOut && n >= 1)
            {
                vector<string> segs;
                topLevelCommaSegments(text, segs);
                for (size_t sg = 1; sg < segs.size(); sg++)
                {
                    const string& t2 = segs[sg];
                    size_t d = t2.find_first_not_of(" \t");
                    if (d != string::npos && isdigit((unsigned char) t2[d]))
                    {
                        int n2 = atoi(t2.c_str() + d);
                        if (n2 >= 1 && n2 <= optionCount && n2 != n)
                        {
                            appendParseNote(noteOut, "multi_answer_first_taken");
                            break;
                        }
                    }
                }
            }
            //Absent-echo staleness: the echo named significant words that
            //match NO offered option (a parent-action echo at a target
            //sub-menu, or a card cast earlier this turn after the option
            //list shifted). The raw index would execute an unintended
            //in-list card - game-losing twice this corpus (deck133 forbidden
            //Thoughtseize, deck140 dumped kill shot). Route to the heuristic
            //instead. A deliberate 0 (decline/pass) carries no card name and
            //is never treated as stale. A multi-option echo (echoConflict)
            //and a no-significant-word echo ("(pass)") keep index-wins. Under
            //INDEX-WINS the discard fires only when the label is ALSO foreign to
            //the option this index selects (echoStaleForIndex) - a consistent
            //label (dual-face / invented-verb / source self-reference) trusts n.
            if (echoNoMatch && n != 0 && echoStaleForIndex(n))
            {
                if (staleEcho) *staleEcho = true;
                if (n >= 1 && n <= optionCount)
                    appendParseNote(noteOut, "stale_echo_in_range"); //#W51-C D3
                return -1;
            }
            return n;
        }
        //out-of-range index, but the echo names a real option: repair
        if (echoRemap > 0 && !echoConflict)
            return echoRemap;
        //#W49-S (D2/D8): an index PAST the menu whose parenthetical names no
        //offered row ("CHOICE: 3 (Cast Acererak the Archlich)" over two rows)
        //is a named row that is not on this list - not an unreadable reply.
        //Signed as its own class so the corpus counts it apart from
        //unparsed_reply, and returned here rather than falling through to the
        //digit scan below, which could only execute a number the model never
        //meant.
        if (n > optionCount && echoNoMatch)
        {
            appendParseNote(noteOut, "named_row_not_offered");
            return -1;
        }
    }
    else if (echoRemap > 0 && !echoConflict)
        return echoRemap; //non-numeric head, but the echo names the intent

    //NAME-LIST multi-pick reply on a single-pick ask (W36 item 2, deck36 F2
    //s29): "CHOICE: Pest #1, Pest #2, Pest #3, Pest #4" carries no coded
    //index and no parenthetical echo, so the trailing digit scan below used
    //to grab the "1" inside "#1" and execute OPTION 1 - a card the model
    //never named, with no fallback flag (the HL11 instrument gap). Resolve
    //the FIRST comma segment by name (honoring the model's own ladder order);
    //a single name that merely CONTAINS a comma ("Sigarda, Champion of
    //Light") resolves both halves to the same option and is not a list. When
    //the first segment cannot be resolved but at least two LATER segments
    //name two distinct options, the reply is provably a pick list whose head
    //is unreadable - fail it loudly (multi_answer_unresolved -> heuristic)
    //rather than let the digit scan execute an unintended option. Anything
    //else (prose with commas) falls through to the shipped scan unchanged.
    if (optionTexts && !optionTexts->empty())
    {
        vector<string> segs;
        topLevelCommaSegments(text, segs);
        if (segs.size() >= 2)
        {
            //the first segment may still carry the answer label when a raw
            //line (salvage path) rather than a consumePlan segment arrives
            int m0 = resolveNamedOption(stripAnswerLabelPrefix(segs[0]), *optionTexts);
            if (m0 >= 0)
            {
                int m1 = resolveNamedOption(segs[1], *optionTexts);
                if (m1 != m0) //a distinct/unresolved tail = a real list
                    appendParseNote(noteOut, "multi_answer_first_taken");
                return m0 + 1;
            }
            int firstHit = -1;
            for (size_t sg = 1; sg < segs.size(); sg++)
            {
                int mx = resolveNamedOption(segs[sg], *optionTexts);
                if (mx < 0)
                    continue;
                if (firstHit < 0)
                    firstHit = mx;
                else if (mx != firstHit)
                {
                    appendParseNote(noteOut, "multi_answer_unresolved");
                    return -1;
                }
            }
        }
    }
    //Real replies begin "CHOICE: N ...", so the head is non-numeric and the
    //index is resolved by the trailing scan below. Staleness is applied to
    //that resolved index (a positive index only - a resolved 0 is a
    //deliberate decline and is never stale), so the branch cannot preempt a
    //CHOICE: 0 before its index is known.
    int choice = -1;
    i = 0;
    while (i < text.size())
    {
        if (isdigit((unsigned char) text[i]))
        {
            size_t j = i;
            while (j < text.size() && isdigit((unsigned char) text[j]))
                j++;
            int n = atoi(text.substr(i, j - i).c_str());
            if (n >= 0 && n <= optionCount)
            {
                choice = n;
                break;
            }
            i = j;
        }
        else
        {
            i++;
        }
    }
    //Absent-echo staleness on the trailing-scan index (the common path for
    //real "CHOICE: N (...)" replies). A resolved 0 is a deliberate decline
    //and is exempt; a positive index whose echo named no offered option AND is
    //foreign to that option (INDEX-WINS) is stale -> defer to the heuristic.
    if (echoNoMatch && choice > 0 && echoStaleForIndex(choice))
    {
        if (staleEcho) *staleEcho = true;
        if (choice <= optionCount)
            appendParseNote(noteOut, "stale_echo_in_range"); //#W51-C D3
        return -1;
    }
    //Name-over-index on the trailing-scan path too (W36 item 4): a unique
    //echo naming a DIFFERENT option than the scanned index is the intent,
    //same rule as the head branch - and the divergence is signed.
    if (echoRemap > 0 && !echoConflict && choice > 0 && echoRemap != choice)
    {
        appendParseNote(noteOut, "echo_index_conflict");
        if (exactNameRemap)
        {
            appendParseNote(noteOut, "name_over_index"); //#W52-J D6
            appendParseNote(noteOut, "index_name_conflict"); //#W66-AR (MED): same rule, trailing-scan path
        }
        return echoRemap;
    }
    if (echoConflict && choice > 0 && !echoMatches.empty())
    {
        bool indexAmongMatches = false;
        for (size_t m = 0; m < echoMatches.size(); m++)
            if (echoMatches[m] == choice - 1)
                indexAmongMatches = true;
        if (!indexAmongMatches)
            appendParseNote(noteOut, "echo_index_conflict_ambiguous");
    }
    return choice;
}


//A verbatim reply-template line the model parroted instead of filling in:
//the system prompt's example "CHOICE: [Number] ([Name])" copied literally
//(deck62 wave-20 N7-template). Its "[Number]"/"[Name]" placeholders are not
//a real answer and must be dropped before disambiguation so the real
//CHOICE: line wins. Case-insensitive on the token.
bool isTemplatePlaceholderLine(const string& line)
{
    string low = line;
    for (size_t i = 0; i < low.size(); i++)
        low[i] = (char) tolower((unsigned char) low[i]);
    return low.find("[number]") != string::npos || low.find("[name]") != string::npos;
}


//A coded line whose parenthetical is a verbatim echo of the reply protocol's
//worked example ("CHOICE: 3 (Cast Example Card)", or its remainder "3 (Cast
//Example Card)") - the model parroting the format string, not a decision.
//(history: comment-archaeology.md AIPlayerGPTParse-isExampleEchoLine-1878)
bool isExampleEchoLine(const string& line)
{
    string low = line;
    for (size_t i = 0; i < low.size(); i++)
        low[i] = (char) tolower((unsigned char) low[i]);
    size_t open = 0;
    while ((open = low.find('(', open)) != string::npos)
    {
        size_t close = low.find(')', open);
        size_t ex = low.find(kExampleFakeCardLc, open);
        if (ex != string::npos && (close == string::npos || ex < close))
            return true;
        if (close == string::npos)
            break;
        open = close + 1;
    }
    return false;
}



//#W49-S (D2): the FIRST line-leading CHOICE line that parses to an offered
//option (same walk as salvageLoopedChoice, first-wins instead of last-wins).
//-1 when no line parses. Used only to make answer_replaced truthful: the
//field must be false when what EXECUTED is the reply's first coded answer.
int AIPlayerGPT::firstCodedChoice(const string& content, int optionCount,
                                  const std::vector<string> * optionTexts)
{
    size_t lineStart = 0;
    while (lineStart <= content.size())
    {
        size_t lineEnd = content.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? content.size() : lineEnd;
        size_t s = lineStart;
        while (s < end && (content[s] == ' ' || content[s] == '\t'
                           || content[s] == '*' || content[s] == '#' || content[s] == '-'))
            s++;
        if (end - s >= 7)
        {
            static const char * kLabel = "CHOICE:";
            bool m = true;
            for (int k = 0; k < 7 && m; k++)
                m = (toupper((unsigned char) content[s + k]) == kLabel[k]);
            if (m)
            {
                string line = content.substr(s + 7, end - (s + 7));
                if (!isTemplatePlaceholderLine(line) && !choiceLineIsRejection(line)) //#W50-Y D7
                {
                    bool st = false;
                    int c = parseChoice(line, optionCount, optionTexts, &st);
                    if (c >= 0)
                        return c;
                }
            }
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    return -1;
}


//#W70-BM (E2): `choiceRetractedNoReplacement` DELETED - the prose-retraction
//tolerance. Its whole search region was "after the answer line, before the
//PLAN: line", which is empty under PLAN-then-action, and its purpose - reading
//a sentence as a change of mind - is exactly what invariant 000 forbids the
//protocol to invite.

//A land-fetch activation, by its rendered option line (the same string the
//de-dup and decline maps key on). These are the ability names the fetchland
//scripts render ("Put in Play with Misty Rainforest targeting...", "search
//basic land with Prismatic Vista") - 664 offers in the wave-9 corpus, the
//top decision-count driver on control decks.
//#W52-L (D16): pure, so the shape is provable in PARSETEST.
//#W54-E (D20, wave-53 ledger MED = R180). The clause priced the crack in
//SOURCES and never in COLOURS, on a menu whose whole point is which colour you
//are fetching: `123v130` s36 offered Tundra and Underground Sea (both make {U})
//among seven colourless-looking rows, the mana line read `colours you can make:
//{b}{w}`, the hand held two {2}{u} spells, the reply said "I need a source that
//makes {U}" and answered Swamp. 90 renders at that seat, colourless every time.
//The colours are read with the SAME landColorFlags the hand line's
//"(land: taps for {W}{U})" uses, and the cannot-make half is measured against
//the same potential the mana line is built from - so no third opinion about
//either fact can enter. Pure over two flag arrays so both halves are provable.
//(history: comment-archaeology.md AIPlayerGPTParse-fetchLandColorsClause-1896)
string fetchLandColorsClause(const bool adds[5], const bool canMake[5],
                                    const int sourceCounts[5])
{
    static const char * kSym[5] = { "{W}", "{U}", "{B}", "{R}", "{G}" };
    string list, missing;
    int n = 0, m = 0;
    for (int k = 0; k < 5; k++)
    {
        if (!adds[k])
            continue;
        if (n++)
            list += " or ";
        list += kSym[k];
        if (!canMake[k])
        {
            if (m++)
                missing += " or ";
            missing += kSym[k];
        }
    }
    //A land whose colours the engine could not read says NOTHING about colours:
    //an unread script and a truly colourless land are indistinguishable here,
    //and a wrong colour claim on this menu is exactly the defect being fixed.
    if (!n)
        return "";
    string s = ", and it adds " + list;
    if (m)
        s += " (you cannot make " + missing + " right now)";
    else
    {
        //Every colour this row adds is already makeable: say so, and say how
        //REDUNDANT it would be. Same symbols, same per-SOURCE counting rule as
        //the mana line, so the two cannot be read against different totals.
        std::ostringstream o;
        o << " (you can already make " << list;
        if (sourceCounts)
        {
            o << " - you have";
            int said = 0;
            for (int k = 0; k < 5; k++)
                if (adds[k])
                    o << (said++ ? ", " : " ") << sourceCounts[k] << " source"
                      << (sourceCounts[k] == 1 ? "" : "s") << " of " << kSym[k];
            o << " right now";
        }
        o << ")";
        s += o.str();
    }
    return s;
}

string fetchMakesNoManaClause(int untapped, bool fetchedEntersTapped,
                                     const string& colorsClause)
{
    std::ostringstream o;
    o << " {this land makes no mana - crack it for a land";
    if (fetchedEntersTapped)
        o << " (the land it finds enters tapped)";
    else if (untapped >= 0)
        o << ": your untapped mana sources go from " << untapped << " to " << (untapped + 1);
    o << colorsClause; //#W54-E (D20)
    o << "}";
    return o.str();
}

bool isFetchCrackLine(const string& line)
{
    //W42-D4 renamed the rendered head ("Put in Play" -> "Put a card onto the
    //battlefield"); BOTH spellings stay live here - this detector keys on the
    //rendered option line, and a missed fetch line silently un-keys the
    //de-dup/decline maps that stop a fetch re-asking three times.
    return line.find("Put a card onto the battlefield with") != string::npos
        || line.find("Put in Play with") != string::npos
        || line.find("search basic land with") != string::npos;
}



//#W82-A (L1, audit-2026-09): the two-decline allowance, its board-scope
//re-opener (`declineBoardScope`) and the `{if you pass here, this option is not
//offered again until the board changes}` bracket (`lastOfferClause`) are
//DELETED with the cap they described. A legal row is offered at every window
//the engine offers it at; the model's own HOLD row is the only thing that stops
//a re-ask, and it is the model's own answer.
//#W64-AI (F4) / #W65-AM (G2, deck152 LOW): the W-row section's scope paragraph.
//It named the destination and what damage there does NOT do, and said nothing
//about what a planeswalker does in COMBAT. Under the trust doctrine a silent
//omission is worse than wrong text - the model confabulates rules into gaps -
//and the one combat question these rows invite is "can it block me back?".
//Stated where it is read, in the restriction-first register (no affirmative
//"planeswalkers can ..." substring for a pilot to latch).
const char * kAttackTargetScopeFacts =
    "Damage sent at a planeswalker does NOT reduce their life total, and a"
    " planeswalker they still control keeps activating its abilities every turn."
    " Blocking works the same either way: they may block an attacker with their"
    " CREATURES whichever it is aimed at - and a planeswalker is not a creature,"
    " so a planeswalker can never block.\n";


//#W48-D13: the loop-scoped consecutive count. Keyed on the (ability, click)
//pair the seam actually returned, so a second maker feeding the same loop
//restarts the count honestly rather than inheriting the first one's.
void AIPlayerGPT::noteLoopTake(MTGAbility * ability, MTGCardInstance * click)
{
    if (ability == mLoopAbility && click == mLoopClick && sameCardId(click, mLoopClickId))
    {
        mLoopCount++;
        return;
    }
    mLoopAbility = ability;
    mLoopClick = click;
    mLoopClickId = cardIdOf(click); //#W54-M (L8)
    mLoopCount = 1;
}


//#W48-F1: the plan's receipt. The pilot named N once and then saw nothing for
//however many ticks the engine took; the narration owes it the count that
//actually happened. Written on every exit - exhausted, no longer offered, no
//longer changing anything, or the turn ended - so the log never implies more
//activations than occurred.
void AIPlayerGPT::endRepeatPlan(const char * why)
{
    if (mRepeatTotal > 0)
    {
        std::ostringstream o;
        o << "Your repeated activation ran " << mRepeatDone << " of the "
          << mRepeatTotal << " times you named";
        if (why && *why)
            o << " (" << why << ")";
        narrateDecision(o.str());
    }
    mRepeatAbility = NULL;
    mRepeatClick = NULL;
    mRepeatRemaining = 0;
    mRepeatTotal = 0;
    mRepeatDone = 0;
    mRepeatBoardKey.clear();
    mRepeatNoProgress = 0;
    mRepeatAbsent = 0;
}


//#W73-BZ (N4, wave-72 HIGH-3): the priority menu is rendered in its own TEXT
//order so an unchanged board renders unchanged bytes (the wave-41 livelock fix).
//That order was a plain byte compare, and a byte compare puts "10/10" between
//"1/1" and "2/2": every creature-land rung menu with ten or more rungs came out
//1, 10, 11, 12, 13, 14, 2, 3 ... The pilot answered `CHOICE: 14 (becomes a 14/14
//hydra)` and index 14 was the 9/9 - the corpus's `echo_index_conflict` and one of
//its two fallbacks (152v125 seq 62; all 5 rung menus this corpus mis-ordered).
//Compare digit RUNS as numbers and everything else byte for byte: still a total
//order over the same texts (equal-number runs fall back to the run's own bytes,
//so no two distinct strings compare equal in both directions), so the prompt
//stays byte-stable for an unchanged state. Pure.
bool naturalTextLess(const string& a, const string& b)
{
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size())
    {
        const bool da = isdigit((unsigned char) a[i]) != 0;
        const bool db = isdigit((unsigned char) b[j]) != 0;
        if (da && db)
        {
            size_t ea = i, eb = j;
            while (ea < a.size() && isdigit((unsigned char) a[ea])) ea++;
            while (eb < b.size() && isdigit((unsigned char) b[eb])) eb++;
            const string na = a.substr(i, ea - i), nb = b.substr(j, eb - j);
            //strip leading zeros for the magnitude compare; the raw run breaks ties
            size_t za = na.find_first_not_of('0'), zb = nb.find_first_not_of('0');
            const string ma = (za == string::npos) ? string("0") : na.substr(za);
            const string mb = (zb == string::npos) ? string("0") : nb.substr(zb);
            if (ma.size() != mb.size())
                return ma.size() < mb.size();
            if (ma != mb)
                return ma < mb;
            if (na != nb)
                return na < nb; //"007" before "07" before "7": a stable tiebreak
            i = ea;
            j = eb;
            continue;
        }
        if (a[i] != b[j])
            return (unsigned char) a[i] < (unsigned char) b[j];
        i++;
        j++;
    }
    return a.size() - i < b.size() - j;
}

#if defined(WAGIC_GPT_PARSETEST_BUILD)
void AIPlayerGPTSelfTestAccess::appendParseNote(std::string * noteOut, const char * sig) { ::appendParseNote(noteOut, sig); }
string AIPlayerGPTSelfTestAccess::fetchLandColorsClause(const bool adds[5], const bool canMake[5], const int sourceCounts[5]) { return ::fetchLandColorsClause(adds, canMake, sourceCounts); }
string AIPlayerGPTSelfTestAccess::fetchMakesNoManaClause(int untapped, bool fetchedEntersTapped, const string& colorsClause) { return ::fetchMakesNoManaClause(untapped, fetchedEntersTapped, colorsClause); }
bool AIPlayerGPTSelfTestAccess::findAnswerLabelLine(const string& text, const char * expectedLabel, size_t& segStart, size_t& segEnd, size_t& labelLineStart, int * extraAnswerLines, int * rejectedLines, size_t * lastHeadLineStart) { return ::findAnswerLabelLine(text, expectedLabel, segStart, segEnd, labelLineStart, extraAnswerLines, rejectedLines, lastHeadLineStart); }
size_t AIPlayerGPTSelfTestAccess::findPlanMarker(const string& text, size_t labelLineStart, size_t * firstOut) { return ::findPlanMarker(text, labelLineStart, firstOut); }
size_t AIPlayerGPTSelfTestAccess::firstLineLeadingPlanPos(const string& text) { return ::firstLineLeadingPlanPos(text); }
bool AIPlayerGPTSelfTestAccess::gptAnswerSegment(const string& text, const char * expectedLabel, size_t& segStart, size_t& segEnd, size_t& labelLineStart, int * extraAnswerLines, int * rejectedLines, size_t * lastHeadLineStart, bool * labellessRead) { return ::gptAnswerSegment(text, expectedLabel, segStart, segEnd, labelLineStart, extraAnswerLines, rejectedLines, lastHeadLineStart, labellessRead); }
bool AIPlayerGPTSelfTestAccess::gptInlineChoiceOnPlanLine(const string& text, size_t& segStart, size_t& segEnd, size_t& lineStart) { return ::gptInlineChoiceOnPlanLine(text, segStart, segEnd, lineStart); }
string AIPlayerGPTSelfTestAccess::gptLineAtOffset(const string& text, size_t off) { return ::gptLineAtOffset(text, off); }
bool AIPlayerGPTSelfTestAccess::isFetchCrackLine(const string& line) { return ::isFetchCrackLine(line); }
bool AIPlayerGPTSelfTestAccess::lineIsCodedAnswerLine(const string& line) { return ::lineIsCodedAnswerLine(line); }
bool AIPlayerGPTSelfTestAccess::menuIsBareXAnnounce(const std::vector<string> * options) { return ::menuIsBareXAnnounce(options); }
bool AIPlayerGPTSelfTestAccess::naturalTextLess(const string& a, const string& b) { return ::naturalTextLess(a, b); }
size_t AIPlayerGPTSelfTestAccess::planBlockEndOffset(const string& text, size_t planPos) { return ::planBlockEndOffset(text, planPos); }
string AIPlayerGPTSelfTestAccess::planMenuDiffClause(const string& absentName) { return ::planMenuDiffClause(absentName); }
bool AIPlayerGPTSelfTestAccess::w80AnswerPrecedesPlan(size_t labelLineStart, size_t planMarkerPos) { return ::w80AnswerPrecedesPlan(labelLineStart, planMarkerPos); }
const char * AIPlayerGPTSelfTestAccess::kAttackTargetScopeFacts = ::kAttackTargetScopeFacts;
#endif //WAGIC_GPT_PARSETEST_BUILD

#endif //WITH_GPT_AI
