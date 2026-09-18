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

//#W82-P12: THE DECISION SEAMS - the engine seam overrides (casting, menus,
//targets, combat, reveal, pregame, discard) and askModel. Moved verbatim out of
//AIPlayerGPT.cpp; see AIPlayerGPTInternal.h.


const OrderedAIAction * AIPlayerGPT::chooseOrderedAction(RankingContainer& ranking)
{
    if (!ranking.size() || mEndpoint.empty())
        return AIPlayerBaka::chooseOrderedAction(ranking);

    //Combat declaration (attacker/blocker assignment and its costs) is a
    //multi-step, order-sensitive state machine driven by the dedicated
    //chooseAttackers/chooseBlockers seams, not a single ranked pick - the
    //model naming one of those entries mid-cycle leaves combat half-declared
    //and soft-locks the step. Filter the declaration mechanics out of the
    //model's menu, but DO offer everything else even during combat phases:
    //instants and activated abilities in combat are exactly the "hold
    //interaction, act at the latest useful moment" plays the strategy
    //priors call for, and the old blanket phase guard silenced them.
    vector<const OrderedAIAction *> candidates;
    int suppressedLandToggles = 0; //#W57-D (D14)
    for (RankingContainer::iterator it = ranking.begin(); it != ranking.end(); ++it)
    {
        MTGAbility * ab = it->first.ability;
        int t = ab ? ab->aType : (int) MTGAbility::UNKNOWN;
        if (t == MTGAbility::MTG_ATTACK_RULE || t == MTGAbility::MTG_BLOCK_RULE
            || t == MTGAbility::ATTACK_COST || t == MTGAbility::BLOCK_COST)
            continue;
        //The alternative-cost RULE on a HAND card (Force of Negation's
        //"exile a blue card") is a whole-spell cast, not a priority ability
        //- and it is already offered, with its stack target and 601.2c/cost
        //legality, by the CAST MENU (FindCardToPlay/legalCasts, viaAlternative).
        //Surfaced here as a standalone ordered action it is a DEAD END for
        //this seam: describeAction cannot express the spell's target, so a
        //single click only arms the extra payment (reactToClick returns 0,
        //nothing reaches the stack) and the action re-offers every window -
        //the model narrates "exile a blue card ..." forever while the spell
        //it should counter resolves (corpus 20260715 seq14+). Baka keeps its
        //own multi-tick follow-through (its chooseOrderedAction still sees
        //this entry), so pure-heuristic play is unchanged; only the GPT menu
        //drops the redundant dead end. Graveyard/exile/library alternative
        //casts (not offered by legalCasts' viaAlternative, hand-only) keep
        //their standalone entry.
        if (t == MTGAbility::ALTERNATIVE_COST && it->first.click
            && game->hand->hasCard(it->first.click))
            continue;
        //A normal cast-from-hand / land play (MTGPutInPlayRule, PUT_INTO_PLAY)
        //surfaced as a priority ordered action is a DEAD END for this seam,
        //exactly like the alternative-cost rule above: a single click only
        //arms it and nothing reaches the stack, so the option re-offers every
        //window. E6 (deck110 wave-17): a second Mox Opal held in hand rendered
        //"Cast Card Normally with Mox Opal {its own reminder text}" as the SOLE
        //priority option and looped as the seat's #1 fallback driver. The
        //dedicated cast menu (FindCardToPlay/buildCastSpell) and land menu
        //(legalLandPlays) already own every hand play, so drop the redundant
        //dead end here. Scoped to HAND cards so an exotic normal-cast from
        //library/graveyard/exile the cast menu may not enumerate is preserved.
        if (t == MTGAbility::PUT_INTO_PLAY && it->first.click
            && game->hand->hasCard(it->first.click))
            continue;
        //#W57-D (D14): the modal-DFC LAND display toggle never reaches this
        //seat's option list. Dropped HERE, before describeAction, so the ~650
        //character row is not even BUILT - the wave-56 corpus built and threw
        //away 760 such option sets below the prompt and rendered 109 rows above
        //it, taken 0 times. Scoped to a LAND back face (see the helper): the
        //spell back face's toggle is still the only route to that cast.
        if (isLandBackedDisplayToggle(ab, it->first.click))
        {
            suppressedLandToggles++;
            continue;
        }
        candidates.push_back(&(it->first));
    }
    //Nothing but declaration mechanics: the heuristic ranking drives those.
    if (candidates.empty())
    {
        //#W57-D (D14): if the ONLY thing this window offered was that toggle,
        //the window auto-passes exactly as the N-152b gate below already made
        //it do (return NULL, no model call, no Baka follow-through) - the
        //suppression must not turn a display-only window into a heuristic act.
        if (suppressedLandToggles > 0)
        {
            DebugTrace("AIPlayerGPT[ph" << observer->getCurrentGamePhase()
                       << "]: only display-toggle (Flip Side) options; auto-passing without a model call"
                       << " (suppressed " << suppressedLandToggles << " land-face toggle row(s))");
            return NULL;
        }
        return AIPlayerBaka::chooseOrderedAction(ranking);
    }

    int phase = observer->getCurrentGamePhase();

    if (mSystemPrompt.empty())
        buildSystemPrompt();

    //Turn ownership stamped ON the header: the phase line sits far above
    //the option list and the pilot sometimes answered as if it were the
    //other player's turn (wave-4/5 reviewers, 3 seats).
    std::ostringstream tail;
    tail << "Your legal actions (" << observer->getCurrentGamePhaseName()
         << (observer->currentPlayer == this ? ", YOUR turn" : ", opponent's turn") << "):\n";
    int index = 0;
    //De-dup identical option lines: a fetchland offers one action per
    //fetchable copy in the library (observed: 13 byte-identical "Put in
    //Play" lines) - from the model's seat they are ONE decision. Keep the
    //first candidate of each rendered line.
    vector<const OrderedAIAction *> shown;
    vector<string> shownLines; //ordered option texts, for the translog
    std::set<string> seenLines;
    //#W82-A (L1): the decline/flip maps this block cleared are DELETED; the
    //turn-boundary work that remains is the repeat-N plan's own expiry.
    if (mPriorityTurnSeen != observer->turn)
    {
        mPriorityTurnSeen = observer->turn;
        //#W48-F1: a repeat-N plan is an answer about THIS turn's priority
        //windows ("returns priority to you here"). It never rides a turn
        //boundary - the loop-scoped count in the [repeat:] tag does that job,
        //deliberately, and a plan that outlived the turn would be taking
        //actions in a window the model was never shown.
        if (mRepeatTotal > 0)
            endRepeatPlan(mRepeatRemaining > 0 ? "the turn ended" : "");
    }
    //LIVELOCK ROOT FIX (146v36, 2026-08-21): the ranking order is NOT stable
    //across priority windows - Baka seeds several ability efficiencies with
    //random() (deliberate heuristic exploration), so equal candidates of one
    //ability reorder on every rebuild. The staleness key is the full prompt
    //text, so a reordered menu made every in-flight answer stale: a permanent
    //drop/respawn loop (341 drops / 4.6h on a 15-option Kaya menu). The model
    //does not consume Baka's rank order - it needs a BYTE-STABLE prompt for an
    //unchanged state. Render the menu in its own text order: deterministic for
    //identical state, and ties (byte-identical lines) are de-duped below
    //anyway. stable_sort keeps this-build ranking order among equal texts.
    vector<std::pair<string, const OrderedAIAction *> > renderOrder;
    mWindowReach = -1; //#W54-M (A21): one potentialColorReach for the whole window
    mWindowReachArmed = true;
    for (size_t c = 0; c < candidates.size(); c++)
        renderOrder.push_back(std::make_pair(describeAction(*candidates[c]), candidates[c]));
    mWindowReachArmed = false;
    std::stable_sort(renderOrder.begin(), renderOrder.end(),
                     [](const std::pair<string, const OrderedAIAction *>& a,
                        const std::pair<string, const OrderedAIAction *>& b)
                     { return naturalTextLess(a.first, b.first); }); //#W73-BZ (N4)
    //W41-18 (wave-40 L-123b): "Flip Side with Bloodline Keeper -> DISPLAY TOGGLE
    //only" was offered at nearly every priority window a Bloodline Keeper was in
    //play - 38 of 285 prompts - changing nothing about the game state, and by
    //alphabetical text order it sat DIRECTLY ABOVE the real, once-offered
    //"Transform:backside with Bloodline Keeper [cost: {b}]": two near-identical
    //lines, the inert one first, at the seam that decides this deck's best card.
    //The option is not suppressed (it is a real face-data mutation, and the
    //never-hide-a-legal-play ruling stands) - it is ordered LAST, the same place
    //the declines go, so option-1 bias cannot land on the no-op and the two lines
    //no longer read as neighbours. stable_sort keeps the byte-stable text order
    //WITHIN each group, so an unchanged state still renders an unchanged prompt.
    std::stable_sort(renderOrder.begin(), renderOrder.end(),
                     [](const std::pair<string, const OrderedAIAction *>& a,
                        const std::pair<string, const OrderedAIAction *>& b)
                     { return (asTurnSide(a.second->ability) != NULL)
                              < (asTurnSide(b.second->ability) != NULL); });
    //The dedupe/deadlock key is board state + question, NOT the assembled
    //prompt: consuming an answer appends to the narration and updates the
    //plan, and a full-prompt key would read that as a state change.
    //#W65-AM (G7): computed HERE rather than after the option list, because the
    //decline allowance below is scoped to the board a decline was given on.
    //serializeGameState() only reads the game; nothing between here and its old
    //site mutates it, so the value is byte-identical to the one that was built
    //there.
    string boardKey = serializeGameState();
    vector<string> renderRows; //#W48 (D2): the rendered option rows, pre-collapse
    for (size_t c = 0; c < renderOrder.size(); c++)
    {
        const OrderedAIAction * cand = renderOrder[c].second;
        //#W82-A (L1, audit-2026-09): the modal-DFC flip cap, the animated-land
        //repeat withhold and the pass-decline cap all stood HERE and each
        //`continue`d a row the engine had already ruled legal. All three are
        //deleted. Owner ruling: "there are strange cards in magic that make
        //behavior that would normally be nonsensical into desirable behavior...
        //you cant decide that a play is bad and therefore never offer it." The
        //re-activation cases the withholds were built for are not even reliably
        //no-ops (CR 613.7b: a later continuous effect carries a later
        //timestamp), so the judgment was wrong as well as forbidden. The model's
        //own HOLD row is the seat's sanctioned way to stop being re-asked.
        const string& line = renderOrder[c].first;
        if (!seenLines.insert(line).second)
            continue;
        shown.push_back(cand);
        //#W47-R4: shownLines keeps the PURE line - it is the de-dup key and the
        //translog record; no rendered clause ever enters it.
        shownLines.push_back(line);
        index++;
        //#W48 (D2): rows are accumulated and emitted through the numbered-row
        //collapse below - one priority window carried 432 options / 116 KB, ~425
        //of them the same Staff of Nin activation differing only in its target's
        //instance ordinal. The NUMBERING is untouched (a collapsed row prints the
        //label range it covers), and shownLines stays uncollapsed, so the reply
        //parser reads exactly the option list it read before.
        //#W51-E D7: the Upkeep animation clause rides the rendered row only.
        bool upkeepAnim = observer->currentPlayer == this
            && observer->getCurrentGamePhase() == MTG_PHASE_UPKEEP && isAnimationRow(line);
        //#W82-A (L1): no `lastOfferClause` - nothing retires any more, and the
        //Upkeep clause no longer has a "last offer" face to spell.
        renderRows.push_back(line + (upkeepAnim ? upkeepAnimationClause() : string()));
    }
    //#W74-CE (O7b, deck123 MED-2): THE STOP CLAUSE ON EVERY CREATE ROW OF THE
    //SEAM. The `{right now: M=..., your stated stop=...}` verdict rode only the
    //appended REPEAT row, so the plain `[cost: Tap]` Create row beside it was an
    //unpriced bypass past the seat's own stop - `123v162` seq 16 and `123v125`
    //seq 624 both took the single-activation row over a stop the same window's
    //repeat row would have priced. Same two numbers, same builder, on the
    //RENDERED row only: `shownLines` stays the pure decline/option-set key, and
    //a `{...}` group is stripped from every key anyway.
    int carriedStop = mStatedStop;
    if (carriedStop < 0)
        repeatPlanStopAndCurrent(mCurrentPlan, &carriedStop, NULL);
    const int carriedStopOppLife = mStatedStopOppLife;
    const int oppLifeNow = opponent() ? opponent()->life : -1;
    if (carriedStop >= 0)
        for (size_t rr0 = 0; rr0 < renderRows.size() && rr0 < shown.size(); rr0++)
        {
            if (!makesCreatureToken(shown[rr0]->ability))
                continue;
            MTGCardInstance * rsrc0 = shown[rr0]->click ? shown[rr0]->click : shown[rr0]->ability->source;
            if (!rsrc0 || !rsrc0->controller())
                continue;
            renderRows[rr0] += repeatRowStopClause(creatureCountOnBattlefield(rsrc0->controller()),
                                                   carriedStop, carriedStopOppLife, oppLifeNow);
        }
    //#W75-CL (P5): the loop override, applied to BOTH the pure key list and the
    //rendered list before anything reads either - the collapse below, the
    //option-set key, the hold latch, the ask key and the translog all see one
    //set of bytes.
    w75ApplyChainFeed(shownLines, lifeLoopProvenWin(opponent()));
    w75ApplyChainFeed(renderRows, lifeLoopProvenWin(opponent()));
    //#W77-CR (R1): the seat's OWN proven loop, mid-resolution. Both lists, in
    //the same place and for the same reason as the chain feed above.
    {
        //#W77-CU (F4): one verdict, three faces.
        string loopSpell, loopPiece;
        const int loopState = w77OwnLoopStackState(loopSpell, loopPiece);
        const bool ownLoopResolving = (loopState == kW77LoopResolving);
        w77ApplyOwnLoopFeed(shownLines, ownLoopResolving);
        w77ApplyOwnLoopFeed(renderRows, ownLoopResolving);
        if (loopState == kW77LoopThreatened)
        {
            const string threat = w77OwnLoopThreatTag(loopSpell, loopPiece);
            w77ApplyOwnLoopThreatFeed(shownLines, threat);
            w77ApplyOwnLoopThreatFeed(renderRows, threat);
        }
        if (ownLoopResolving)
            w77CountOwnLoopWindow(); //the population HIGH-1 measured at 35
    }
    {
        //#W48 (D2): gather repeated rows before collapsing, permuting the action
        //list and the translog's option list with them so option N still names
        //shown[N-1] on every surface.
        vector<size_t> order;
        groupNumberedRows(renderRows, order);
        bool reordered = false;
        for (size_t k = 0; k < order.size() && !reordered; k++)
            reordered = (order[k] != k);
        if (reordered && order.size() == renderRows.size()
            && order.size() == shown.size() && order.size() == shownLines.size())
        {
            vector<string> rr(renderRows), sl(shownLines);
            vector<const OrderedAIAction *> sh(shown);
            for (size_t k = 0; k < order.size(); k++)
            {
                renderRows[k] = rr[order[k]];
                shownLines[k] = sl[order[k]];
                shown[k] = sh[order[k]];
            }
        }
        //W50-W (D6): every offered action hits the seat's own permanent.
        bool allSelf = !shownLines.empty();
        for (size_t s0 = 0; s0 < shownLines.size() && allSelf; s0++)
            allSelf = shownLines[s0].find(kSelfTargetClause) != string::npos;
        if (allSelf)
            tail << kSelfOnlyWindowNote;
        bool anyOptionRangeRow = false;
        unsigned rangeForms = 0; //#W82-P8
        tail << joinNumberedRows(renderRows, &anyOptionRangeRow, &rangeForms);
        if (anyOptionRangeRow)
            tail << optionRangeNote(rangeForms);
    }
    //#W82-A (L1): the "every distinct action is suppressed as already-declined"
    //auto-pass is DELETED with the cap that could produce it. Nothing suppresses
    //a row now, so `index` is 0 only when the engine offered nothing - and that
    //case is already handled before the render loop.
    const int baseIndex = index;

    //#W48-F1: the repeat-N rows, appended AFTER every real option (they are
    //shortcuts over a row that is already on the menu, not new plays) and
    //BEFORE the decline row (declines stay last). Each carries the same
    //OrderedAIAction pointer as the row it repeats, so every downstream
    //consumer - describeAction, the narration, the activation stamp - reads the
    //base action; only the rendered line and the plan differ.
    vector<int> repeatBaseRow;
    repeatBaseRow.assign(shown.size(), -1);
    vector<string> repeatRendered;   //#W74-CE (O18)
    vector<int> repeatRenderedIdx, repeatRenderedBase;
    //#W66-AS (H3): the stop the pilot last stated, read back onto the row that
    //asks for it.
    //(history: comment-archaeology.md AIPlayerGPTSeams-L379-1909)
    //#W74-CE (O7b): `carriedStop` is computed above, before the rows are
    //rendered, so the base Create rows and the repeat rows price one stop.
    //#W72-BT (M10, deck123 HIGH-1): does every base row on this menu belong to a
    //repeat family whose OWN rendered verdict says the stop is reached? Answered
    //from the same two numbers the row prints - the engine's live creature count
    //and the stop the model stated - so the collapse below and the clause the
    //model reads can never disagree.
    bool everyBaseRowIsStopReached = (baseIndex > 0);
    //#W78-CV (S3): the same question asked of the ROW'S OWN PRINTED VERDICT rather
    //than of the repeat row's eligibility - a token-maker at or past the stated stop
    //prints `ALREADY AT OR PAST your own stop` whether or not it has been activated
    //this turn, and on a fresh turn it has not, so the wave-72 predicate went blind
    //on exactly the twelve windows the docket names.
    bool everyBaseRowStopPriced = (baseIndex > 0);
    bool anyStopReachedRow = false;
    //#W81-DI: the SAME verdict, per base row, so the replay bound below can ask it
    //of the one row a cached answer would take rather than of the whole window
    //(`w79StopReachedRePutCollapses` needs EVERY row stop-priced; a replay needs
    //only its own). Filled from `rowStopPriced` inside this loop - one verdict,
    //two readers, so the row's printed clause and the bound cannot disagree.
    std::vector<char> w81StopReachedRow((size_t) (baseIndex > 0 ? baseIndex : 0), 0);
    for (int rb = 0; rb < baseIndex; rb++)
    {
        if (isManaOnlyAction(shown[rb]->ability))
            continue; //a mana window is auto-passed below; a repeat there buys nothing
        //#W78-CV (S3): the row's live count, read before the repeat-row gate so the
        //verdict arm can see a row the repeat arm cannot offer.
        int pricedCount = -1;
        if (makesCreatureToken(shown[rb]->ability))
        {
            MTGCardInstance * psrc = shown[rb]->click ? shown[rb]->click : shown[rb]->ability->source;
            if (psrc && psrc->controller())
                pricedCount = creatureCountOnBattlefield(psrc->controller());
        }
        const bool rowStopPriced = (pricedCount >= 0 && carriedStop >= 0
                                    && pricedCount >= carriedStop);
        if (rowStopPriced)
            w81StopReachedRow[(size_t) rb] = 1; //#W81-DI
        if (!rowStopPriced)
            everyBaseRowStopPriced = false;
        if (!repeatRowEligible(asActivatedForCount(shown[rb]->ability)))
        {
            everyBaseRowIsStopReached = false; //#W72-BT (M10): a live row of another kind
            if (rowStopPriced)
                anyStopReachedRow = true;      //#W78-CV (S3): its verdict still counts
            continue;
        }
        int creatureCount = pricedCount; //#W50-Z (D11)
        if (creatureCount >= 0 && carriedStop >= 0 && creatureCount >= carriedStop)
            anyStopReachedRow = true; //#W72-BT (M10)
        else
            everyBaseRowIsStopReached = false;
        string rline = repeatRowLine(repeatShortName(shownLines[rb]), index + 1, creatureCount)
                       + repeatRowStopClause(creatureCount, carriedStop,
                                             carriedStopOppLife, oppLifeNow); //#W66-AS (H3), #W74-CE (O7c)
        index++;
        shown.push_back(shown[rb]);
        shownLines.push_back(rline);
        //#W57-A (D4): the RENDERED list stays parallel to shownLines for every
        //row the model can press, so `chosen_text` and `options_text` can both
        //be the row the prompt actually printed.
        renderRows.push_back(rline);
        repeatBaseRow.push_back(rb);
        repeatRendered.push_back(rline);   //#W74-CE (O18): printed below, folded
        repeatRenderedIdx.push_back(index);
        repeatRenderedBase.push_back(rb + 1);
    }
    //#W74-CE (O18): print the repeat block, collapsing runs of rows that differ
    //only in their digits. Display only - `shown`, `shownLines` and `renderRows`
    //are untouched above, so the option set still reaches every target.
    {
        size_t r0 = 0;
        while (r0 < repeatRendered.size())
        {
            size_t r1 = r0 + 1;
            const string key = repeatRowFoldKey(repeatRendered[r0]);
            while (r1 < repeatRendered.size()
                   && repeatRenderedIdx[r1] == repeatRenderedIdx[r1 - 1] + 1
                   && repeatRowFoldKey(repeatRendered[r1]) == key)
                r1++;
            tail << repeatRenderedIdx[r0] << ". " << repeatRendered[r0] << "\n";
            if (r1 - r0 >= kBattlefieldCollapseFloor)
            {
                tail << repeatRunRangeLine(repeatRenderedIdx[r0 + 1], repeatRenderedIdx[r1 - 1],
                                           repeatRenderedIdx[r0], repeatRenderedBase[r0 + 1],
                                           repeatRenderedBase[r1 - 1])
                     << "\n";
                r0 = r1;
                continue;
            }
            for (size_t rk = r0 + 1; rk < r1; rk++)
                tail << repeatRenderedIdx[rk] << ". " << repeatRendered[rk] << "\n";
            r0 = r1;
        }
    }

    //#W72-BT (M10, wave-71 deck123 HIGH-1). THE ANSWER IS ALREADY GIVEN.
    //`123v152` seqs 28-38: eleven consecutive priority windows on ONE turn
    //(opponent's main 1 -> combat -> attackers -> blockers x4 -> combat damage ->
    //combat ends -> main 2 x2), every one carrying the same two Thraben Doomsayer
    //rows over `{right now: M=29, your stated stop=29, so this window would add to
    //a count ALREADY AT OR PAST your own stop}`, every one answered `CHOICE: 0
    //(pass)` - 1,287 s of wall clock and 11 model calls to advance nothing. Two
    //re-serve caches missed it: the ask cache is not on this seam, and the
    //repeat-ask reserve refuses a held choice of 0 (`heldChoice < 1`), which a
    //pass always is.
    //This is NOT a removal and NOT an auto-answer of an open question. It is the
    //same collapse the mana-only window already gets: the model stated the stop,
    //the engine's own count has reached it, and the ONLY live rows left are that
    //repeat family - so the window's answer is the one the model already wrote.
    //Three guards keep it to that: (a) EVERY base row must be a stop-reached
    //repeat family or a mana-only row - one live row of any other kind (a cast, a
    //different activation) and the window is asked as usual; (b) the stop must
    //come from a plan the model stated ON THIS TURN, so a stop persisted from an
    //earlier turn can never silence a later one; (c) it collapses only while the
    //count stays at or past the stop - the moment the board moves back under it,
    //repeatRowStopClause prints room again and the window re-opens.
    //#W72-BX (F2): the STOP's own date, not the last plan line's. mPlanSetTurn
    //is stamped by every reply that writes a PLAN, so a plan that states no stop
    //re-dated a stop from an earlier turn and let it silence this turn's windows.
    //#W53-N (D2, second half) + #W55-A (D2a/D19): how many times THIS EXACT
    //list was already declined this turn, keyed on the OPTION SET (see
    //optionSetKeyOf) rather than the rendered rows. Two defects retire here:
    //an annotation tracking a moving life total minted a fresh key at every
    //iteration of a mandatory loop (D2), and the cast decline row's Main-1
    //clause made the same own-turn list at Main 1 and Main 2 two lists, so the
    //count never reached the pair (D19, 162v126 s7-s12). Computed BEFORE the
    //HOLD row is appended: the row is on every window now, and it renders the
    //count this key produces.
    if (mListDeclineTurn != observer->turn)
    {
        mListDeclineCount.clear();
        mListDeclineIdent.clear(); //#W78-CY (F1)
        mListDeclineTurn = observer->turn;
    }
    const string listKey = optionSetKeyOf(shownLines);
    const int declinedN = mListDeclineCount.count(listKeyHash(listKey))
                          ? mListDeclineCount[listKeyHash(listKey)] : 0;
    string declinedNote = declinedListNote(declinedN);

    //#W78-CY (F1): the collapse identity for THIS window, built here - before the
    //HOLD row is appended - so the string recorded at the decline below and the
    //string compared at the next re-put are built over the same rows.
    //#W79-CZ (T2): the ACTION-KEY SET and the stated stop NUMBER - nothing else.
    const string rePutIdentity = w79RePutCollapseIdentity(shownLines, carriedStop);
    const bool rePutIdentityStands =
        w78RePutIdentityStands(mListDeclineIdent.count(listKeyHash(listKey))
                                   ? mListDeclineIdent[listKeyHash(listKey)] : string(),
                               rePutIdentity);

    //#W78-CV (S3): moved below `declinedN` - unchanged in every clause, so
    //the wave-72 arm fires on exactly the windows it always did; the new arm
    //beneath it needs the decline count, which is measured here.
    if (observer && w72StopReachedWindowCollapses(everyBaseRowIsStopReached, anyStopReachedRow,
                                                 carriedStop, mStatedStopTurn, observer->turn))
    {
        mStopReachedWindowsSkipped++;
        DebugTrace("AIPlayerGPT[ph" << phase << "]: every live row is a repeat family already at"
                   " the model's own stated stop (" << carriedStop << "); passing without a model"
                   " call (skipped " << mStopReachedWindowsSkipped << " this game)");
        mLastChoice = 0;
        return NULL;
    }

    //#W78-CV (S3, wave-77 deck123 HIGH-2): the RE-PUT arm. Same answer, already
    //given, on a list this seat has already declined this turn.
    if (observer && w79StopReachedRePutCollapses(everyBaseRowStopPriced, anyStopReachedRow,
                                                 carriedStop,
                                                 declinedN, rePutIdentityStands)) //#W79-CZ (T2)
    {
        mStopReachedWindowsSkipped++;
        mStopReachedRePutsCollapsed++;
        DebugTrace("AIPlayerGPT[ph" << phase << "]: every live row is already at the model's own"
                   " stated stop (" << carriedStop << ") and this exact list was declined "
                   << declinedN << " time(s) this turn; passing without a model call ("
                   << mStopReachedRePutsCollapsed << " re-puts collapsed this game)");
        mLastChoice = 0;
        return NULL;
    }


    //#W53-N (D2): the HOLD row. It sits LAST among the numbered rows, with the
    //other declines, and it is a row like any other - the model may take it or
    //ignore it. Taking it removes no window and hides no legal play: the
    //engine simply stops re-putting THIS question until the board moves (see
    //holdHonoured).
    //(history: comment-archaeology.md AIPlayerGPTSeams-L566-1922)
    int holdRow = ++index;
    {
        //#W72-BV (M6, deck126 HIGH-1): does THIS menu hold a live activation?
        //Read off the same actions the rows above were printed from, so the
        //sentence and the list cannot disagree: an ActivatedAbility that is not
        //a mana producer is a row the hold gives up. Mana-only rows are excluded
        //for the reason the repeat row excludes them - a mana window is not a
        //play - and a menu with none of either keeps the wave-53 sentence
        //byte-for-byte.
        bool activationLive = false;
        for (int ai = 0; ai < baseIndex && !activationLive; ai++)
            if (asActivatedForCount(shown[ai]->ability) && !isManaOnlyAction(shown[ai]->ability))
                activationLive = true;
        const string holdLine = holdRowLine(false, activationLive); //#W55-A (D21), #W72-BV (M6)
        shownLines.push_back(holdLine);
        renderRows.push_back(holdLine); //#W57-A (D4)
        tail << holdRow << ". " << holdLine << "\n";
    }
    //#W48-F3: on a single-option window the decline becomes a real, numbered
    //LAST row instead of a clause buried in the reply-format sentence. The
    //index is 0, which the parser has always accepted, so nothing about the
    //answer grammar changes - only the model's ability to see the choice.
    //(history: comment-archaeology.md AIPlayerGPTSeams-L610-1935)
    //#W75-CI (P1 a): #W74-CE's in-row re-ask TAG is REVERTED here. It was a
    //rising count rendered INSIDE a numbered row, and the wave-74 corpus billed
    //it twice: it is the byte that minted a fresh ask key on every answer (the
    //CG regression, 384 model calls on one window) and the byte that re-opened
    //a taken hold at every link of a chain (engine-seat HIGH-1). Its own metric
    //also moved the wrong way - the tagged plain decline row was taken 284/561
    //= 50.6%, against wave 73's 40% and CE's predicted < 30%. The row itself is
    //NEVER removed (the legal-option boundary) and the FACT is not withheld: it
    //still rides the prompt-only `[you declined this exact list N times already
    //this turn]` note, which is spliced in below the list and enters no key.
    tail << "0. " << kPassPriorityRowText
         << ownClockTagFor(this, opponent(), life, //#W74-CF (F7)
                           (mIncomingCombatTurn == observer->turn) ? mIncomingCombatDamage : 0,
                           pendingStackLifeLossToSeat(observer, this), //#W75-CL (P10)
                           lifeLoopProvenWin(opponent()),
                           statedCrackBackLossToSeat(this, opponent(), observer)) //#W76-CO (Q3 b)
         << "\n"; //#W75-CI (P1a): no in-row decline tag - the fact rides the prompt-only note below
    //#W53-N (D2): where the option list ends and the per-ask facts begin. The
    //prompt-only decline annotation is spliced in here, so it reads with the
    //list it is about and still never enters the ask key.
    const size_t optionsEnd = (size_t) tail.tellp(); //#W54-M (L5): the put position, not a copy

    //#W48-F1: a repeat-N plan the model already answered consumes THIS window
    //with no model call and no prompt assembly. Two exits, both taken here
    //rather than by a cap:
    // * the option is no longer on the menu - the engine has stopped offering
    //   it, which is the only honest payability re-check available at this
    //   seam (the ranking is rebuilt from isReactingToClick every tick), so the
    //   plan stops early exactly as the row promised;
    // * the activations have stopped moving the board - a repeat that changes
    //   nothing is a stall, not a repeat. Two consecutive unmoved
    //   serializations are required, because an activation resolving off the
    //   stack can leave one tick's board identical.
    //The dispatch is ONE action per tick onto the ordinary clickstream - never
    //a synchronous burst, which the latent ability-GC double-destroy makes
    //fatal (DecisionContract design section 4, and how planCastSpell's PLAN is
    //consumed). Each iteration stamps the activation so the shared
    //WEventAbilityActivated narration writes one line for the decision rather
    //than one per iteration.
    if (mRepeatTotal > 0 && mRepeatRemaining <= 0)
        endRepeatPlan("");
    if (mRepeatRemaining > 0)
    {
        int found = -1;
        for (int r = 0; r < baseIndex; r++)
            if (shown[r]->ability == mRepeatAbility && shown[r]->click == mRepeatClick
                && sameCardId(shown[r]->click, mRepeatClickId)) //#W54-M (L8): address reuse is not the same row
            {
                found = r;
                break;
            }
        if (found < 0)
        {
            //ABSENT THIS WINDOW is not the same fact as UNPAYABLE. The loop's
            //own untap (Intruder Alarm's ETB trigger) rides the stack, and this
            //seam is reached during that resolution too - the source is still
            //tapped, the option is legitimately off the menu, and it comes back
            //the moment the trigger resolves. Live probe 2026-08-27: ending the
            //plan on the first absence cut EVERY named 25 to 1 or 2. So end the
            //plan only where absence is definitive - an EMPTY stack, nothing
            //left to resolve that could restore the cost - and otherwise take no
            //action this window (which passes priority, letting the trigger
            //resolve) for a bounded number of windows.
            bool resolving = observer->mLayers->stackLayer()->count(0, NOT_RESOLVED) > 0;
            if (resolving && ++mRepeatAbsent <= kRepeatAbsentWindows)
            {
                DebugTrace("AIPlayerGPT[ph" << phase << "]: repeat plan waiting on the stack ("
                           << mRepeatAbsent << "/" << kRepeatAbsentWindows << ")");
                return NULL;
            }
            endRepeatPlan("the cost could no longer be paid");
        }
        else
        {
            mRepeatAbsent = 0;
            string board = serializeGameState();
            bool moved = (board != mRepeatBoardKey);
            if (moved)
            {
                mRepeatNoProgress = 0;
                mRepeatBoardKey = board;
            }
            else
                mRepeatNoProgress++;
            if (!moved && mRepeatNoProgress >= 2)
            {
                endRepeatPlan("it stopped changing anything");
            }
            else
            {
                mRepeatRemaining--;
                mRepeatDone++;
                noteLoopTake(shown[found]->ability, shown[found]->click);
                if (ActivatedAbility * revAct = eventRaisingActivation(*shown[found]))
                    stampSelfActivation(revAct);
                DebugTrace("AIPlayerGPT[ph" << phase << "]: repeat plan iteration "
                           << mRepeatDone << "/" << mRepeatTotal << " (no model call)");
                return shown[found];
            }
        }
    }
    //N-152b: cosmetic-only priority window. When the ONLY non-pass options are
    //modal-DFC Flip-Side display toggles (AATurnSide), the model can gain
    //nothing here and demonstrably loses - it burns a full round trip (deck152:
    //Tovolar's Huntmaster flipped a useless "backside" toggle across ~10
    //consecutive windows at ~200s each) and the bare toggle is a fabrication
    //attractor (vs136 seq27 hallucinated an off-menu "Cast Briarbridge Tracker"
    //-> the corpus's one unparsed fallback). Auto-pass instead of asking. This
    //never skips a real play: a modal-DFC LAND's other-color face is still
    //reachable through its OWN play menu ("Play Land / Flip Side / Decline",
    //where Flip Side is annotated as the route - N-152a), and a DFC spell's
    //other face still casts from the Cast menu. The gate is exact: EVERY shown
    //option must be a Flip-Side toggle; if any real action coexists, the toggle
    //stays offered (annotated) and the model is asked as before.
    bool allTurnSide = true;
    for (size_t s = 0; s < shown.size(); s++)
        if (!asTurnSide(shown[s]->ability))
        {
            allTurnSide = false;
            break;
        }
    if (allTurnSide)
    {
        DebugTrace("AIPlayerGPT[ph" << phase << "]: only display-toggle (Flip Side) options; auto-passing without a model call");
        return NULL;
    }
    //#W46-7 (carried #W44-9): mana-only priority window. Floating mana with
    //nothing to spend it on is not a play - the pool empties at the end of the
    //step and the board is where it was - yet 39 of deck126's 46 priority
    //windows across three games (85%) offered nothing but "Add 3 green mana
    //with Overgrown Battlement", each one a full prompt assembly and a round
    //trip to answer "pass". The gate is the ledger's, both halves required:
    //EVERY shown option is a mana activation (isManaOnlyAction - one real
    //action of any other kind and the window is asked as before), AND nothing
    //is waiting to be PAID. The payment half is what keeps the guide's "float
    //for the Tribute in this same window" legal: a pending extra payment, an
    //armed menu or target chooser, and mana already floating in the pool each
    //mean this window is mid-transaction, and the model keeps it.
    //This is the same shape as the N-152b display-toggle gate above and takes
    //nothing away: a mana ability is never the only route to a play - the cast
    //and land menus tap for their own costs (ManaEngine::autoTapForCost).
    bool allManaOnly = !shown.empty();
    for (size_t s = 0; s < shown.size(); s++)
        if (!isManaOnlyAction(shown[s]->ability))
        {
            allManaOnly = false;
            break;
        }
    //#W50-Z (D12): the floating-pool half is SCOPED to the seat's own float.
    //Wave 49 (deck126 vs125 seq 66/68/75/77/85/87): six single-row Battlement
    //windows reached the model, three of them with an EMPTY stack, every one
    //carrying `Already in pool: ...` left behind by a countered cast (D1's
    //residue) - mana the seat never chose to float and that buys nothing on
    //an all-mana menu (a cast row is offered on potential + pool, so floating
    //never unlocks a row). Only mana this seat floated by taking a mana row in
    //THIS step keeps the window; residue auto-passes like an empty pool.
    bool midTransaction = observer->mExtraPayment
        || observer->mLayers->actionLayer()->menuObject
        || observer->mLayers->actionLayer()->getCurrentTargetChooser();
    if (manaOnlyWindowSkippable(allManaOnly, midTransaction, getManaPool()->getConvertedCost(),
                                mSeatFloatStepKey, stepKey()))
    {
        mManaOnlyWindowsSkipped++;
        mSkipTrace.note("mana_only_windows_skipped"); //#W81-DK (V15)
        DebugTrace("AIPlayerGPT[ph" << phase << "]: only mana production and no pending cost; auto-passing without a model call (skipped "
                   << mManaOnlyWindowsSkipped << " this game)");
        return NULL;
    }
    //Wave-35 churn driver #5 (batch5 #12, batch6 P5): a priority ask named the
    //phase and nothing about what was still AHEAD, so the model re-derived the
    //turn structure from scratch at nearly every window - "did I miss the
    //chance to cast Sigarda?", "usually there is a Main Phase 1 before combat",
    //"perhaps the system filtered options out". Both statements below are
    //scoped so they are only made where they are TRUE (a true statement in the
    //wrong scope is a lie): the second main phase is only still ahead on YOUR
    //turn before it, and a later priority window only exists before the turn
    //has run out.
    //N-152i: BEFORE the first main (upkeep/draw windows on your own turn) the
    //relevant fact is that BOTH mains are ahead and main-phase cards are not
    //castable HERE - the second-main-only fact invited a hallucinated cast of
    //the pilot's planned creature (deck152 vs105, unparsed_reply). Between the
    //mains, the second-main fact stands as before. Each statement only where
    //it is true.
    if (observer->currentPlayer == this && phase < MTG_PHASE_FIRSTMAIN)
        tail << "\n" << kMainPhasesAheadFact;
    else if (observer->currentPlayer == this && phase < MTG_PHASE_SECONDMAIN)
        tail << "\n" << kSecondMainAheadFact;
    if (phase < MTG_PHASE_ENDOFTURN)
        tail << (observer->currentPlayer == this && phase < MTG_PHASE_SECONDMAIN ? "" : "\n")
             << kPriorityAgainFact;
    //#W49-S (D8): the casting question of this phase was already put to the
    //model - say so, where it is true.
    if (w72CastAnsweredFactApplies(
            w81CastAnsweredStampMatches(mCastAskTurn, mCastAskPhase, mCastAskStack,
                                        observer->turn, phase, w81StackStateStamp()), //#W81-DK (V4)
            mCastDecisionOpen)) //#W72-BX (F3)
    {
        const string sofar = tail.str(); //#W54-M (L5): one copy, not three
        tail << (sofar.empty() || sofar[sofar.size() - 1] == '\n' ? "" : "\n") << kCastAnsweredFact;
    }
    //#W70-BL (E2): PLAN first, then the one action line for this window.
    //#W78-CW (S2): the short-name sentence, carved out for the repeat row on
    //exactly the windows that carry one - read off the rows the model is
    //looking at, so the two surfaces cannot disagree.
    tail << "\nWhich action do you take? " << kPlanFirstLead << "on a line of its own CHOICE: followed by the number (0 = pass priority) and its SHORT NAME in parentheses "
         << w78ShortNameParenthetical(w78AnyRepeatRow(shownLines))
         << ", e.g. \"CHOICE: 3 (Cast Example Card)\" (a placeholder - copy a real number and short name from the list) or \"CHOICE: 0 (pass)\". Write nothing else.";

    //#W53-N (D2): the model's own hold, honoured. No model call, no window
    //removed from the record - the row the model took said this.
    //#W61-U (C14): measured over the same rows the latch reads, before the
    //latch consumes the window, so a held window still updates the memory.
    //#W68-BB (J9): the pending stack's death verdict, on the rows that decline
    //to answer it - BEFORE the hold latch reads them, so a hold taken on a
    //lethal screen is held over a row that says so.
    appendStackDeathToDeclineRows(shownLines,
                                  pendingStackLifeLossToSeat(observer, this), life);
    appendPlainDeclineScope(shownLines, holdRow > 0); //#W72-BW (M21)
    const string holdNote = holdReopenNote("priority", shownLines);
    if (holdRow > 0 && holdHonoured("priority", shownLines))
    {
        w76HoldWindowNotAsked(mHoldMemory, "priority"); //#W76-CN (Q1)
        mLastChoice = 0; //a hold is a pass for this window
        return NULL;
    }
    //#W66-AS (H3 second half): the same unwind, for a window with no legal row.
    if (loopAutoPassWindow())
    {
        w76HoldWindowNotAsked(mHoldMemory, "priority"); //#W76-CN (Q1)
        mLastChoice = 0;
        return NULL;
    }
    //#W74-CD (O13): MEASURE ONLY - this window is about to be asked exactly as
    //before. Counted only while a chain is actually resolving, which is the
    //population `chain_windows_collapsed` reports 0 for.
    if (observer->mLayers->stackLayer()->count(0, NOT_RESOLVED) > 0)
    {
        int acting = 0, empty = 0;
        for (size_t s13 = 0; s13 < shownLines.size(); s13++)
        {
            if (w72RowIsDeclineOrHold(holdKeyRow(shownLines[s13])))
                continue;
            acting++;
            if (w74RowIsDecisionEmpty(shownLines[s13]))
                empty++;
        }
        if (acting > 0)
        {
            mChainActingRows += acting;
            mChainSelfharmRows += empty;
            if (empty == acting)
                mChainWindowsOnlySelfharm++;
        }
    }
    //#W49-S (D8/D3): the one re-ask for this board state. Appending the
    //correction makes this a DIFFERENT question (its own askKey, a fresh
    //call); the board moving on retires it. It is deliberately NOT cleared on
    //exhaustion: the corrected question must stay this state's question so
    //the cached -1 replays instead of re-minting the original key and paying
    //another round trip (the W39-D1(b) trap).
    if (!mPriorityReaskBoard.empty() && mPriorityReaskBoard != boardKey)
    {
        mPriorityReaskBoard.clear();
        mPriorityReaskLine.clear();
        mPriorityReaskKind.clear();
    }
    if (!mPriorityReaskBoard.empty())
        tail << "\n" << mPriorityReaskLine;
    const string tailStr = tail.str(); //#W54-M (L5): the option list is copied once
    //#W74-CG: the KEY half of that list, with the one clause whose text moves
    //with the answer count taken out. The model still reads `tailStr`.
    const string keyTailStr = w77KeyTailOf(tailStr); //#W77-CU (F1)
    //#W82-A (L2): BOARD STATE + QUESTION. `boardKey` is this window's own
    //`serializeGameState()`, built above; `keyTailStr` is the normalised option
    //list. Nothing else enters the key.
    string askKey = w82WindowKey(boardKey, keyTailStr);
    //#W53-N (D2): the decline annotation goes into the PROMPT only - askKey is
    //built from tail.str() alone, so a count that rises with every answer can
    //never mint a fresh question and turn the cache into a call per tick.
    string userTail = tailStr;
    //#W73-BY (N5): does this menu carry a row that is a PLAY? Read off the rows
    //the menu is about to print, with the same decline/hold filter the hold
    //latch uses, so the bracket's claim and the list cannot disagree.
    bool loopNoteActingRows = false;
    bool loopNoteLifeRow = false; //#W73-CB (F4): does an acting row touch LIFE?
    for (size_t lnr = 0; lnr < shownLines.size(); lnr++)
    {
        if (w72RowIsDeclineOrHold(holdKeyRow(shownLines[lnr])))
            continue;
        loopNoteActingRows = true;
        if (w73RowTouchesLife(shownLines[lnr]))
            loopNoteLifeRow = true;
    }
    //#W78-CV (S6 + S4): the two verdict lines this seam owes, on the prompt-only
    //channel (never a row, never a key). The own-loop verdict is the RENDER of the
    //marker `w77OwnLoopVerdictKey` already joins to the held set; the drain clause
    //is gated on the hold-check bracket's own comparison.
    string w78SeamNotes;
    //#W79-DC (F10, Astra wave-79 review finding 10): THE SPLICE IS NOT THE SEND.
    //#W79-CZ (T4) moved the own-loop count and the face stamp from the row-build
    //site to the splice, which is where the bytes enter `userTail` - and two gates
    //BELOW this point still discard the window without sending anything (the
    //deadlock-breaker pass, and the ask-cache replay that answers from `mLastChoice`
    //with no round trip at all). A counted line and a stamped face for a prompt that
    //was never sent is the wave-78 helper-versus-caller failure in its own shape.
    //So the face is held here and applied where the prompt is actually handed to
    //`pollCompletionRetry`; a suppressed window CLEARS the face, so a later record
    //can never consume it.
    string w79PendingLoopFace;
    //#W80-DE (U2/U15): the two verdict lines and the drain clause are HELD here
    //with the loop face and applied at the send, so nothing is counted or stamped
    //for a prompt the gates below still discard.
    string w80PendingCrackBackFace, w80PendingStackDeathFace;
    bool w80PendingDrain = false;
    {
        //#W79-CZ (T4): the verdict, on the channel that reaches the model.
        string w78LoopSpell, w78LoopPiece;
        {
            const int lst = w77OwnLoopStackState(w78LoopSpell, w78LoopPiece);
            const string vline = w79OwnLoopVerdictLineFor(lst, lifeLoopProvenWin(this),
                                                          w78LoopSpell, w78LoopPiece,
                                                          w80LiveLoopStarterName());
            if (!vline.empty())
            {
                w78SeamNotes += vline;
                //#W80-DE (U10): the face is read off the LINE THAT PRINTED, so the
                //record names what the model saw instead of `none`.
                w79PendingLoopFace = w80LoopFaceOfLine(vline);
            }
        }
        //#W80-DE (U2): the crack-back and stack-death verdicts, RENDERED. Both
        //were latch keys only and printed 0 times in 2,203 wave-79 prompts.
        {
            //#W81-DL (V5): built on the ONE total the screen publishes.
            const string cbLine = w81CrackBackVerdictLineNow();
            if (!cbLine.empty())
            {
                w78SeamNotes += cbLine;
                //#W80-DH (F4): the face the LINE carries, not the latch's marker.
                w80PendingCrackBackFace = w80CrackBackFaceOfLine(cbLine)
                                          + w81CrackBackTotalFace(cbLine); //#W81-DL (V5)
                if (!w81CrackBackTotalFace(cbLine).empty())
                    mW81PendingEventFace += "crackback_one_total_folded;";
            }
            const string sdLine = w80StackDeathVerdictLineNow();
            if (!sdLine.empty())
            {
                w78SeamNotes += sdLine;
                w80PendingStackDeathFace = stackDeathVerdictNow();
            }
        }
        const string drain = w78StackDrainNote(w78TheirDrainingTriggerCount(),
                                               w78RowsUnchangedSinceLastAsk(holdNote),
                                               w78HoldRowShortName(shownLines));
        if (!drain.empty())
        {
            w78SeamNotes += drain;
            w80PendingDrain = true; //#W80-DE (U15): counted at the send, not here
        }
    }
    const string promptNotes = declinedNote + holdNote + w78SeamNotes //#W61-U (C14): same channel
        + loopChainingNote(lifeLoopProvenWin(opponent()), holdRow > 0,
                           loopNoteActingRows, loopNoteLifeRow) //#W66-AS (H3), #W73-BY (N5), #W73-CB (F4)
        //#W75-CI (P19): the hold's contract, once per prompt, on the same
        //prompt-only channel. This seam renders its own tail and does not route
        //through askModel's copy of this line - it is where HALF the corpus's
        //hold rows are printed, so both sites are needed (live probe: 5,222
        //windows rendered the row, 2,617 of them through askModel).
        + (holdRow > 0 ? holdContractParagraph() : string());
    if (!promptNotes.empty() && optionsEnd <= userTail.size())
        userTail = userTail.substr(0, optionsEnd) + promptNotes + userTail.substr(optionsEnd);
    //#W57-H (D43): this window's ask class, for the log window and the record.
    mLogWindowKind = askWindowKindForPriority(shownLines, logWindowStackRespondable());
    string userMsg = assemblePrompt(userTail, NULL, &keyTailStr); //#W62-fix: notes stay out of the slot key; #W74-CG: and so does the decline count
    bool unchanged = (askKey == mLastAskKey);
    //#W79-DC (F1, third part of Astra finding 1): the deadlock breaker's own
    //evidence. Its premise is "the action did not progress the game", and before
    //#W79-CZ (T3) the ask key carried `serializeGameState()`, so a SUCCESSFUL
    //activation that changed the board minted a different key and the breaker did
    //not fire. Under a scope-plus-rows key a reusable activation that creates a
    //token, taps a permanent or moves a card leaves every normalised row standing
    //and is read as "no progress" - and the seat is forced to pass priority after a
    //play that worked. The breaker therefore reads the board itself, which is the
    //only thing that can say whether the game moved; the ask CACHE is untouched
    //and still keys on the seam scope plus the continuation digest.
    const string& w79ProgressBoard = boardKey; //already rendered once this tick (#W54-M A19)
    const bool w79BoardMoved = (mLastProgressBoardKey != w79ProgressBoard);

    //Deadlock breaker: priority is decided every AI tick. If the game state
    //is unchanged since our last decision AND that decision was to TAKE an
    //action, the action did not progress the game (it was a no-op, or the
    //engine could not complete it). Repeating it loops forever while the AI
    //never passes priority - so the queued phase change never resolves and
    //the game freezes (silently, since the cached path made no model call).
    //The heuristic avoids this by passing when nothing is worth doing; the
    //model can name a dead action. So on an unchanged state we pass once,
    //yielding priority; a real state change then earns a fresh decision.
    if (w79ForcePassNoProgress(unchanged, mLastChoice, w79BoardMoved))
    {
        DebugTrace("AIPlayerGPT: state unchanged after action " << mLastChoice
                   << "; passing priority to avoid a loop");
        mLastChoice = 0;
        {
            bool w79c = false; //#W79-DC (F10): nothing was sent
            w79ApplyLoopFaceAtSend(false, w79PendingLoopFace, mOwnLoopVerdictFace, w79c);
            w80ApplyVerdictFacesAtSend(false, w80PendingCrackBackFace,
                                       w80PendingStackDeathFace, w80PendingDrain);
        }
        return NULL;
    }
    if (unchanged && mLastChoice > 0 && w79BoardMoved) //#W79-DC (F1): the same premise, negated
    {
        //#W79-DC (F1): the activation WORKED - same menu, moved board. Not a loop.
        mMenuPassNoProgressSuppressed++;
        DebugTrace("AIPlayerGPT: the menu is unchanged after action " << mLastChoice
                   << " but the board moved - that activation progressed the game,"
                      " so priority is not force-passed");
    }

    //#W82-A (L2): the #W81-DI replay bound is DELETED. It existed because "a
    //cached answer stood once for this window and the board moved under it" -
    //which the key now makes impossible: a moved board is a different key and a
    //different key is asked. The stop-verdict half of its trigger was a second
    //judgment about whether a legal row was worth replaying, which is L1's rule.
    string w81PendingReaskReason; //#W79-DC (F10): stamped at the SEND, never here
    int choice;
    //#W82-A (L2): the cache half IS the board now - `askKey` carries
    //`serializeGameState()`, so a life tick under an unchanged menu is a
    //different key and IS re-asked. That is the owner's rule and its price: the
    //233 same-phase re-asks #W79-CZ (T3) measured are back, bought with the
    //windows #W79-CZ's key was mis-serving.
    if (unchanged)
    {
        //Nothing changed since the model last answered: reuse the decision
        //instead of paying another round trip this AI tick.
        //#W79-DC (F10): no prompt is sent on this path, so no line is counted and
        //no face is stamped.
        {
            bool w79c = false;
            w79ApplyLoopFaceAtSend(false, w79PendingLoopFace, mOwnLoopVerdictFace, w79c);
            w80ApplyVerdictFacesAtSend(false, w80PendingCrackBackFace,
                                       w80PendingStackDeathFace, w80PendingDrain);
        }
        choice = mLastChoice;
    }
    else
    {
        //#W81-DK (V9, wave-80 engine-seat HIGH-4): the prompt is sent from here -
        //but WHETHER it was sent is the TRANSPORT's answer, not this caller's
        //literal. Wave 80 asserted `true` one statement before the call, so a tick
        //that found a request already in flight counted a line nothing had rendered
        //yet, and the priority seam's own stamped faces were then cleared by the
        //next suppressed window of a sibling seam
        //(`crackback_verdict_lines_rendered` 382 vs 329 prompts vs 291 records; 36 of
        //the 38 unstamped prompts are `priority` records). The count and the stamp
        //now happen on the HANDED-OFF disposition only; an IN-FLIGHT tick touches
        //nothing at all, so the face staged at the real send survives to the record.
        mTransportHandedOff = false;
        string content;
        const int w81Poll = pollCompletionRetry(userMsg, content, "priority");
        const int w81Disp = w81SendDisposition(mTransportHandedOff,
                                               w81Poll == kChoicePending);
        if (w81Disp == kW81SendHandedOff)
        {
            bool w79c = false;
            w79ApplyLoopFaceAtSend(true, w79PendingLoopFace, mOwnLoopVerdictFace, w79c);
            if (w79c)
                w79CountOwnLoopVerdictLine();
            w80ApplyVerdictFacesAtSend(true, w80PendingCrackBackFace,
                                       w80PendingStackDeathFace, w80PendingDrain);
            //#W81-DI: same discipline - the reason rides the record of the window
            //whose prompt was actually handed over, and an ordinary new-key send
            //writes the empty string, which clears any face a discarded window left.
            mReaskReasonFace = w81PendingReaskReason;
        }
        if (w81Poll == kChoicePending)
        {
            //Round trip in flight: no action this tick. The Act override
            //keeps the empty clickstream from being committed as a pass.
            return NULL;
        }
        //The plan is split off BEFORE choice parsing: plan prose is full of
        //numbers that would otherwise misparse as the chosen action. Restrict
        //the answer line to CHOICE: so a CoT "Attack:"/"Blocks:" line in the
        //reasoning body is not taken as the answer (stale-echo family B).
        int choiceRunLen = 0, rejectedLines = 0;
        int planAnswerNote = kPlanAnswerNoteNone;
        string laterIgnored; //#W66-AR (H2c)
        string decisionPart = consumePlan(content, "CHOICE:", &choiceRunLen, &rejectedLines,
                                          &planAnswerNote, &laterIgnored);
        if (!laterIgnored.empty())
            mLastIgnoredAnswerLine = laterIgnored;
        bool staleEcho = false;
        string parseNote;
        choice = parseChoice(decisionPart, index, &shownLines, &staleEcho, NULL, &parseNote,
                             true); //#W53-N (D9): row 0 = pass priority exists here
        //#W70-BM (E2): the adjacent-run FOLD is deleted. A run of answer lines is
        //not a formatting habit to be folded, it is more than one answer -
        //`extra_answer_line` names it. #W82-A (audit-2026-09): the
        //`multi_answer_first_taken` note itself was RE-ADDED in #W81-DM and is
        //emitted from parseChoice; the comment said it was deleted.
        if (rejectedLines > 0)
            appendParseNote(&parseNote, "rejected_line_skipped"); //#W50-Y D7
        //#W63-AD (E6b): sign which side of the plan bound this reply fell on.
        if (planAnswerNote == kPlanAnswerExtraLine)
            appendParseNote(&parseNote, "extra_answer_line"); //#W70-BM (E2)
        if (!parseNote.empty())
            mLastParseNote = parseNote;
        //#W70-BM (E2): the repeat-loop / bookend SALVAGE is DELETED. It walked
        //EVERY CHOICE: line of a reply and kept the last clean one - a
        //multi-answer tolerance, and the protocol now permits exactly one
        //answer line. An unusable answer routes to the re-ask ladder below,
        //which asks the model rather than choosing among its drafts.
        //#W71-BO (R9): the label-missing RE-ASK is DELETED at this seam - the
        //string `label_missing_reask` appears in none of the 42 wave-70 seat logs,
        //so the predicate never fired once. An unlabelled reply is already counted
        //by `off_protocol_bytes` and answered by the heuristic.
        //#W70-BM (E2): the RETRACTION GATE is DELETED. It read the prose
        //between the answer line and the PLAN line for a sentence taking the
        //answer back (and a coded index inside the plan block as a recode).
        //Under invariant 000 that region does not exist: the reply is a PLAN
        //line and an action line.
        //#W82-A (audit-2026-09): the `retracted` local and the `retracted_choice`
        //fallback class it selected are DELETED with it - the variable was never
        //assigned anywhere since #W70-BM, so the class was unwritable.

        //#W48-F1: N as the model named it (0 = this was not a repeat row, or no
        //count was named and the row collapses to a single activation).
        int repeatN = 0;
        string repeatReceipt; //#W49-S (D3): the narrated receipt of a countless take
        bool namedRowFail = (choice < 0 && !content.empty()
                             && parseNote.find("named_row_not_offered") != string::npos);
        //#W51-C (D3): an in-range index whose name echoes no row of the
        //previous window is an off-menu name -> the named_row re-ask lane
        //(the stale_echo label stays for a genuine echo of a prior row).
        //#W82-A (L8): same rule at the priority seam - an in-range coded answer
        //whose name disagrees earns a RE-ASK, never the heuristic.
        if (staleEcho && choice < 0 && !content.empty()
            && parseNote.find("stale_echo_in_range") != string::npos)
        {
            staleEcho = false;
            namedRowFail = true;
            appendParseNote(&mLastParseNote,
                            nameEchoesRow(headParenthetical(decisionPart.empty() ? content : decisionPart),
                                          mPrevWindowRows)
                                ? "prior_window_name_in_range" : "off_menu_name_in_range");
        }
        bool repeatRowTaken = (choice >= 1 && choice <= index && choice - 1 >= baseIndex
                               && choice - 1 < (int) repeatBaseRow.size()
                               && repeatBaseRow[choice - 1] >= 0);
        //#W50-Y D7 (the seq-12 half): the count is read off EVERY coded line of
        //the reply, last named wins - "CHOICE: 2 (... x19)" then "So CHOICE: 2
        //(... x20)." named 20 and ran 19. parseRepeatCount is CHOICE-anchored
        //(plan prose is never scanned) and skips quoted and rejection tokens.
        int namedCount = repeatRowTaken ? parseRepeatCount(content) : -1;
        //#W51-C (D4a): "x0" on the repeat row is the model's stated pass, not
        //one activation (deck123 vs162 seq 35: x0 under "M is 26 now. Pass."
        //ran once). Resolved to the pass row here; x1 stays a single.
        if (repeatRowTaken && namedCount == 0)
        {
            DebugTrace("AIPlayerGPT: repeat row named x0 -> pass");
            choice = 0;
            repeatRowTaken = false;
            namedCount = -1;
            appendParseNote(&mLastParseNote, "repeat_count_zero_pass");
        }
        string latchedChoiceLine;
        size_t latchedFrom = 0;
        const bool haveLatchedLine = latchedCodedChoiceLine(content, choice, index, &shownLines,
                                                            &latchedChoiceLine, &latchedFrom);
        //#W71-BO (R4, wave-70 census): THE WHOLE REVERSAL APPARATUS IS DELETED -
        //`planSaysPassThisWindow`, `passVerdictInProse`, `planChoiceConflict`, the
        //`decision_reversed_in_prose` stamp and the `plan_choice_conflict_*` census.
        //It was built to catch a reply that answered one row and then argued for
        //another in the same breath; that shape existed because reasoning was in
        //the reply. Under reasoning-on it is gone: `plan_choice_conflict_seen` fired
        //0 times in 2,119 decisions, where wave 69 had it as its single largest
        //fallback class (25). What the predicate scanned - a verdict read out of
        //the reply's own words and weighed against its action line - is exactly
        //what invariant 000 says the reply may not contain.
        //#W52-J (D14b): a counted repeat-row take with no PLAN line at all
        //has no stop arithmetic to hold it to -> one re-ask for the PLAN line.
        bool planMissing = (repeatRowTaken && namedCount >= 1 && !replyHasPlanLine(content));
        //#W75-CJ (P14, engine-seat MED-7). THE ROW'S OWN PROMISE, KEPT. The
        //repeat row's bracket says in as many words that "copying this row's name
        //alone, with no x<count> after it, names no count and is refused and
        //re-asked" - and since #W71-BO (R6) deleted the repeat-count arm (it read
        //0 in the wave-70 corpus) the engine has instead run the action ONCE and
        //said nothing to the model. O17 then removed the "N" placeholder from the
        //row's short name, which made the bare copy the natural thing to write:
        //`123v125` seq 58 answered `CHOICE: 3 (Create vampire with Bloodline
        //Keeper, repeated then stop)` over a PLAN that said "this window 21", and
        //got one token. Under the trust doctrine a rendered statement is an
        //instruction: either the engine refuses and re-asks, or the row must stop
        //claiming it does. This is the refusal, and it guesses NOTHING - no count
        //is invented, no row is withheld, the ceiling is unchanged, and the second
        //answer executes as given (including a bare copy again, which then runs
        //once exactly as it does today).
        bool repeatCountMissing = (repeatRowTaken && namedCount < 0);
        //#W66-AS (H3, deck162 HIGH-1): the PLAN the row demands, READ BACK. A
        //count taken over a plan whose own numbers say the stop is already
        //reached is refused exactly as a count with no plan is - ONE re-ask,
        //on the model's own stated stop, quoting both numbers. This is not a
        //cap: no count is clamped, no row is withheld, and the second answer
        //executes as given whatever it is (the exhausted stamp below). 162v123
        //wrote "stop=20" and then took x33 at M=68, x47 at M=153 and x200 at
        //M=230 - ten windows, each a full round trip, each past a stop the
        //reply itself restated in the same breath.
        //#W67-AY (I6): every reply's own two numbers reach the store, whatever
        //the reply answered - deck123 seq 76 stated "stop=26; M=40 now" on a
        //PASS and seq 77 then rendered no verdict at all. The guard below still
        //rests on the numbers of THIS reply (a refusal must quote the words the
        //model just wrote), and the store is what the row renders next window.
        {
            //#W70-BN (F4): the two permitted lines are the whole span.
            int stStop = -1, stNow = -1;
            //#W73-BZ (N3): the PLAN line alone, not both protocol lines.
            const string protOnly = planLineOnly(content);
            if (repeatPlanStopAndCurrent(protOnly, &stStop, &stNow)
                && repeatPlanStopIsOwn(protOnly)) //#W72-BX (F2)
            {
                mStatedStop = stStop;
                mStatedStopOppLife = (observer && opponent()) ? opponent()->life : -1; //#W74-CE (O7c)
                mStatedStopTurn = observer ? observer->turn : -1; //#W72-BX (F2)
            }
        }
        //#W71-BO (R6, wave-70 census): the STOP-GUARD RE-ASK and its clamp are
        //DELETED (`repeat_past_stop` 0 of 2,119, `repeat_count_reask` 0). The stop
        //the model states is still STORED above and still rendered on the row - the
        //information stays; only the extra round trip and the engine-performed
        //clamp go.
        //#W71-BO (R5, wave-70 census): the index/name conflict RE-ASK is DELETED
        //(0 fallbacks in 2,119; the parser's own notes - `echo_index_conflict`,
        //`index_name_unique_name`, `name_over_index` - recovered all 5 occurrences
        //without one). The notes are KEPT; only the round trip goes.
        //#W49-S (D8/D3): ONE re-ask per board state, before the heuristic (D8)
        //or before the single activation (D3). The corrected question is put
        //in flight now and answered on a later tick, exactly like a first ask;
        //the record written here carries the reply that earned the re-ask.
        //#W66-AR (H8): the same self-contradiction gate the ask seam runs - the
        //row's own zero-verdict plus the reply's own agreement with it. One
        //predicate, both seams; the second answer executes as given.
        //(history: comment-archaeology.md AIPlayerGPTSeams-L1262-1952)
        //#W82-A (L8, audit-2026-09): THE NO-OP RE-ASK IS DELETED at both seams and
        //in both its lanes. It recognised a VALID chosen row - in range, legal,
        //unambiguous - and then demanded a different answer solely because the
        //engine judged the row's local effect to be zero. That is a semantic veto
        //of a legal choice: "you cant decide that a play is bad and therefore never
        //offer it" reaches the ANSWER as well as the offer, and a row whose local
        //effect is zero can still be the play (a sacrifice outlet, a storm count, a
        //loop cycle, a "whenever you cast" trigger). The truthful no-effect
        //ANNOTATION on the row stays: the model is told, and the model decides.
        const bool noopRowZero = false;
        if (!content.empty() && mPriorityReaskBoard != boardKey
            && (namedRowFail || planMissing || repeatCountMissing)) //#W82-A (L8): noopRowZero is gone
        {
            std::ostringstream corr;
            const char * fb;
            //#W56-C (D3): quote what the engine ran, not the reply's first try.
            const string quotedChoiceLine = haveLatchedLine ? latchedChoiceLine
                                                            : firstLabelledLine(content, "choice:");
            if (namedRowFail)
            {
                const string offending = headParenthetical(decisionPart.empty() ? content : decisionPart);
                corr << "[RE-ASK] \"" << offending
                     << "\" is not on this list. Answer with a number from 1 to " << index
                     << ", or 0 (pass)."
                     << laterStepRouteClause(offending, &shownLines); //#W62-Z (D11)
                fb = "named_row_reask";
                mPriorityReaskKind = "named_row";
            }
            else if (planMissing)
            {
                corr << "[RE-ASK] Your CHOICE line names " << namedCount << " repeats (\""
                     << quotedChoiceLine << "\") but carries no PLAN line. Answer again with a"
                        " PLAN line carrying \"stop=<N>; M=<N>\" - the count you mean to finish"
                        " at and the count you are at now - plus how many you perform this"
                        " window, and then the CHOICE line; or 0 (pass)."; //#W70-BL (E4) #W73-BZ (N3)
                fb = "plan_missing";
                mPriorityReaskKind = "plan_missing";
            }
            else if (repeatCountMissing) //#W75-CJ (P14)
            {
                corr << "[RE-ASK] \"" << quotedChoiceLine << "\" names no count."
                        " That row repeats an action and needs one: answer again with"
                        " the same row number and \"x<count>\" after the name - e.g."
                        " \"CHOICE: " << choice << " (" << repeatRowShortName(shownLines[choice - 1])
                     << " x3)\" - where the count is a DIGIT you choose, at most "
                     << kRepeatRowMax << ", together with a PLAN line carrying"
                        " \"stop=<N>; M=<N>\"; or 0 (pass).";
                fb = "repeat_count_reask";
                mPriorityReaskKind = "repeat_count";
            }
            mPriorityReaskBoard = boardKey;
            mPriorityReaskLine = corr.str();
            if (!parseNote.empty())
                mLastParseNote = parseNote;
            writeTransLog("priority", userMsg, content, choice, index, "", fb, &renderRows); //#W57-A (D4)
            setNotice(namedRowFail ? "that answer named nothing on the list - asking again"
                      : planMissing ? "the repeat row was taken with no plan - asking again"
                      : repeatCountMissing ? "the repeat row was taken with no count - asking again"
                                    : "the chosen row's own note says it does nothing - asking again", 5.0f);
            DebugTrace("AIPlayerGPT: " << fb << " -> re-asking once");
            string corrected;
            pollCompletionRetry(assemblePrompt(userTail + "\n" + mPriorityReaskLine), corrected, "priority");
            //mLastAskKey is left as it was: the corrected question's own key
            //differs from it, so the next tick polls (and eventually consumes)
            //the corrected call rather than replaying this failed answer.
            return NULL; //in flight; decisionPending() holds the pass
        }
        if (mPriorityReaskBoard == boardKey && !mPriorityReaskKind.empty())
        {
            if (mPriorityReaskKind == "named_row")
                appendParseNote(&mLastParseNote, namedRowFail ? "named_row_reask_exhausted"
                                                              : (choice >= 0 ? "named_row_reask_recovered" : "named_row_reask_unanswered"));
            else if (mPriorityReaskKind == "plan_missing") //#W52-J D14b: executes as given either way
                appendParseNote(&mLastParseNote, planMissing ? "plan_missing_exhausted"
                                                             : (choice >= 0 ? "plan_missing_recovered" : "plan_missing_unanswered"));
            else if (mPriorityReaskKind == "noop_plan")
                appendParseNote(&mLastParseNote,
                    (noopRowZero && choice >= 1 && choice == mPriorityReaskPriorChoice)
                        ? "noop_row_retaken"
                        : (noopRowZero ? "plan_contradicts_noop_row_exhausted"
                                       : (choice >= 0 ? "plan_contradicts_noop_row_recovered"
                                                      : "plan_contradicts_noop_row_unanswered")));
            if (choice >= 0)
                noteReaskExecuted("priority", choice,
                                  (choice >= 1 && choice <= (int) shownLines.size())
                                      ? shownLines[choice - 1] : string());
            mPriorityReaskKind.clear();
        }
        mPrevWindowRows = shownLines; //#W51-C D3: this window is now the prior one
        if (content.empty())
            noticeFallback("model reply failed or timed out - the heuristic decides", 5.0f);
        else if (holdRow > 0 && choice == holdRow)
        {
            //#W53-N (D2): the model held. Nothing is narrated (a non-action
            //leaves no trace) and nothing is capped - the latch lives exactly
            //as long as the board it was taken on.
            takeHold("priority", shownLines);
        }
        else if (choice >= 1 && choice <= index)
        {
            //#W48-F1: resolve a repeat row to the action it repeats. The rows
            //share an OrderedAIAction pointer, so everything below reads the
            //base row; only the plan and the narration know about the N.
            int actRow = choice - 1;
            if (repeatRowTaken)
            {
                int named = namedCount;
                actRow = repeatBaseRow[actRow];
                if (named > kRepeatRowMax)
                {
                    named = kRepeatRowMax; //the ceiling the row states, enforced
                    appendParseNote(&mLastParseNote, "repeat_count_clamped");
                }
                if (named >= 2)
                    repeatN = named;
                else
                {
                    //The row was taken without the count it asks for. Perform
                    //the single activation the base row would have performed -
                    //never a guessed N - and record why in the translog AND in
                    //the narration (#W49-S D3: the pilot is owed the receipt).
                    std::ostringstream rc;
                    if (named < 0)
                    {
                        appendParseNote(&mLastParseNote, "repeat_count_missing");
                        rc << " - ran 1 time (you named no count)";
                    }
                    else
                    {
                        appendParseNote(&mLastParseNote, "repeat_count_under_two");
                        rc << " - ran 1 time (you named " << named << ")";
                    }
                    repeatReceipt = rc.str();
                }
            }
            //W42-D2: an ACTIVATION taken here is narrated in the SHARED
            //activation wording ("You used: <label> with <Card>"), so this
            //seat's line and the peer's event line are one sentence with the
            //subject swapped; and the act is stamped so the event that follows
            //the click does not write it a second time. Non-activations
            //(attacks, blocks, land drops) keep their conjugated form - no
            //other seat narrates those, so there is nothing to agree with.
            ActivatedAbility * evAct = eventRaisingActivation(*shown[actRow]);
            string takenLine = actionTakenNarration(
                stripNarrationDecoration(describeAction(*shown[actRow])),
                evAct != NULL);
            if (repeatN >= 2 && !takenLine.empty())
            {
                std::ostringstream rn;
                rn << takenLine << " (you named " << repeatN << " repeats of it)";
                takenLine = rn.str();
            }
            else if (!repeatReceipt.empty() && !takenLine.empty())
                takenLine += repeatReceipt;
            narrateDecision(takenLine);
            //Gated on a line having actually been written: a CAST narrates ""
            //here (its zone events tell that story instead), and stamping one
            //would suppress a line this seat never wrote - turning a de-dup
            //into a new asymmetry.
            if (!takenLine.empty())
                stampSelfActivation(evAct);
            //#W82-A (L1): the fetch consume-on-choose stamp and the DFC flip
            //thrash count are DELETED with the withholds they fed - a taken
            //fetch that is still legal next window is still offered next window.
            //#W48-D13: the loop-scoped consecutive count, which does NOT reset
            //at a turn boundary - that reset is exactly what made the per-turn
            //[repeat:] number unusable as a stopping signal.
            noteLoopTake(shown[actRow]->ability, shown[actRow]->click);
            //#W50-Z (D12): a mana row taken here is the seat's OWN float.
            if (isManaOnlyAction(shown[actRow]->ability))
                mSeatFloatStepKey = stepKey();
            //#W48-F1: arm the plan. One decision, one translog record, one
            //narration line; the remaining iterations are dispatched above with
            //no further model calls.
            if (repeatN >= 2)
            {
                mRepeatAbility = shown[actRow]->ability;
                mRepeatClick = shown[actRow]->click;
                mRepeatClickId = cardIdOf(shown[actRow]->click); //#W54-M (L8)
                mRepeatTotal = repeatN;
                mRepeatRemaining = repeatN - 1;
                mRepeatDone = 1;
                mRepeatBoardKey = boardKey;
                mRepeatNoProgress = 0;
            }
        }

        mLastAskKey = askKey;
        //#W81-DI: the model answered THIS key, so the latch is released and this
        //key's replay allowance starts again from nothing.
        //#W79-DC (F1): the board this decision was taken over, so the deadlock
        //breaker's next reading is "has the board moved SINCE the action", not
        //"do the rows still read the same".
        mLastProgressBoardKey = w79ProgressBoard;
        mLastChoice = choice;
        //#W53-N (D2): a hold is this window's pass, never a no-progress
        //action - the deadlock breaker must not read it as one.
        if (holdRow > 0 && choice == holdRow)
            mLastChoice = 0;
        //#W53-N (D2, second half): a decline of this EXACT list, counted once
        //per real answer (this branch does not run on a cached replay).
        if (choice == 0 || (holdRow > 0 && choice == holdRow))
        {
            mListDeclineCount[listKeyHash(listKey)]++;
            //#W78-CY (F1): ...under THIS window's collapse identity. A later re-put
            //whose cost, board or stop statement differs finds a mismatch and is asked.
            mListDeclineIdent[listKeyHash(listKey)] = rePutIdentity;
        }
        //#W48-D13: a pass, or a decision handed to the heuristic, is the loop
        //being broken - the consecutive count starts again from nothing.
        if (choice <= 0 || (holdRow > 0 && choice == holdRow))
        {
            mLoopAbility = NULL;
            mLoopClick = NULL;
            mLoopCount = 0;
        }
        {
            const char * fb = (choice >= 0) ? NULL : (content.empty() ? noAnswerClass() : (staleEcho ? "stale_echo" : (namedRowFail ? "named_row_not_offered" : unparsedReplyClass(content))));
            //#W49-S (D2): what executed IS the first coded line -> not replaced
            if (choice >= 0 && firstCodedChoice(content, index, &shownLines) == choice)
                mAnswerReplacedFalse = true;
            //#W57-A (D4): `chosen_text` IS the rendered row, byte for byte.
            //Wave 56 wrote `kHoldPriorityRowText` for the hold row (tail-less:
            //112 of 320 HOLD takes, and the reason an exact-match census read
            //208) and `describeAction(*shown[N-1])` for the rest - the PURE
            //line, without the last-offer and upkeep-animation clauses the
            //prompt printed, and the BASE action's text for a repeat row.
            //On the wave-56 corpus that is 124 of 2,416 single-row takes
            //disagreeing with the row they name. renderRows is what
            //joinNumberedRows printed; shownLines is deliberately the pure
            //de-dup/decline key and stays what it was.
            string chosen = (choice >= 1 && choice <= (int) renderRows.size())
                          ? renderRows[choice - 1]
                          : (choice == 0 ? string("pass") : string());
            //#W48-F1: ONE record carries the whole repeat - the action and the
            //N the model named for it. #W57-A (D4): the N rides its own field
            //(`repeat_n`) so the row text stays byte-equal to the printed row.
            if (repeatN >= 2)
                mLastRepeatN = repeatN;
            writeTransLog("priority", userMsg, content, choice, index, chosen, fb, &renderRows);
        }
        DebugTrace("AIPlayerGPT: model chose " << choice << " of " << index);
    }

    //Log every outcome (the cached paths were previously silent, which hid
    //a freeze): phase, whether the state was cached, and what we return.
    if (choice < 0) //transport/parse failure: defer to the heuristic
    {
        DebugTrace("AIPlayerGPT[ph" << phase << "]: defer to heuristic (cached=" << unchanged << ")");
        //#W67-AX (I7, engine MED-1): and what it answered. The row is the one
        //this window PRINTED for that action when the heuristic picked one the
        //model was offered; 0 says the heuristic passed. Report-only.
        const OrderedAIAction * heur = AIPlayerBaka::chooseOrderedAction(ranking);
        int heurRow = 0;
        string heurText("pass");
        if (heur)
        {
            heurText = stripNarrationDecoration(describeAction(*heur));
            for (size_t si = 0; si < shown.size(); si++)
                if (shown[si] == heur)
                {
                    heurRow = (int) si + 1;
                    if (si < renderRows.size())
                        heurText = renderRows[si];
                    break;
                }
        }
        noteHeuristicExecuted("priority", heurRow, heurText);
        return heur;
    }
    if (choice == 0) //deliberate pass
    {
        DebugTrace("AIPlayerGPT[ph" << phase << "]: pass priority (cached=" << unchanged << ")");
        return NULL;
    }

    if (holdRow > 0 && choice == holdRow)
    {
        DebugTrace("AIPlayerGPT[ph" << phase << "]: hold priority taken (cached=" << unchanged << ")");
        return NULL;
    }
    DebugTrace("AIPlayerGPT[ph" << phase << "]: take action " << choice << "/" << index << " (cached=" << unchanged << ")");
    return shown[choice - 1];
}


int AIPlayerGPT::selectHintAbility()
{
    //Deck hint scripts pre-empt the ranked seam (they push a scripted action
    //before selectAbility builds the menu), which silently takes the
    //decision away from the model. With a live endpoint the same abilities
    //still reach the model through the ranking; without one, keep the hints
    //(they are part of the Baka experience the fallback promises).
    if (!mEndpoint.empty())
        return 0;
    return AIPlayerBaka::selectHintAbility();
}


//The reply-format exemplar for a generic ask: option 1's own short name
//(annotations and bracket tags stripped, cut at the first annotation brace),
//so the example can never contain an action word absent from the menu.
//#W50-Y D8: a trailing P/T parenthetical ("Goblin (1/1)") is dropped from the
//example's short name - the example must be a shape the parser accepts
//verbatim, and a nested parenthetical is the one shape the wave-49 parser
//refused (the parser now balances it too; the example still stays flat).
static string stripTrailingPT(const string& core)
{
    size_t close = core.find_last_not_of(" \t");
    if (close == string::npos || core[close] != ')')
        return core;
    size_t open = core.rfind('(', close);
    if (open == string::npos || open == 0)
        return core;
    bool pt = false, slash = false;
    for (size_t k = open + 1; k < close; k++)
    {
        char c = core[k];
        if (c == '/') { if (slash) return core; slash = true; }
        else if (isdigit((unsigned char) c) || c == '*' || c == '+' || c == '-') pt = true;
        else return core;
    }
    if (!pt || !slash)
        return core;
    string out = core.substr(0, open);
    while (!out.empty() && isspace((unsigned char) out[out.size() - 1]))
        out.erase(out.size() - 1);
    return out.empty() ? core : out;
}


//#W66-AR (H8, deck126 HIGH-1). THE WORKED EXAMPLE IS AN ANSWER ATTRACTOR.
//All 3 `named_row_reask` windows in the wave-65 corpus recovered to the
//exemplified row VERBATIM, and twice that row was `Cast Tribute to Hunger
//{right now: they control 0 creatures - at 0 this does nothing}` - the engine's
//own annotation saying the row does nothing, printed inside the sentence that
//shows the model how to answer. 126v125 s34 then stamped
//`plan_contradicts_noop_row` and executed it. A format example must be about
//FORMAT: it may not put a dead row in the answer slot. The example is therefore
//built from the first row this menu's own zero-predicate (`rowSaysNoOp`, the
//single definition of dead) does NOT flag, and the row it used is reported so
//the sentence around it can name the right number - the number and the name in
//an example that says "e.g." must still point at the same row.
//#W66-AU (R5, codex finding 5 - CONFIRMED). Wave 66 left the all-dead menu on
//row 1, which is the very answer-attractor H8 was docketed against: a mandatory
//menu of `Cast A {right now: this does nothing}` / `Cast B {...}` handed the
//model `CHOICE: 1 (Cast A)` as its worked answer. The format still has to be
//stated, so it is - as a TEMPLATE that names no row. A decline row is preferred
//first where the menu has one: declining IS the least harmful answer, and it is
//a real row, not a fallback. `*usedRow` is 0 when no row is exemplified, and the
//caller's sentence changes with it.
//(history: comment-archaeology.md AIPlayerGPTSeams-rowSaysHalfDead-1968)
static bool rowSaysHalfDead(const string& row)
{
    string low = row;
    for (size_t i = 0; i < low.size(); i++)
        low[i] = (char) tolower((unsigned char) low[i]);
    return low.find("half dead") != string::npos;
}


//#W73-BZ (N10, deck146 MED-3): the worked example cut the row's core at a hard
//48 bytes, mid-token, and emitted the halves of quotes and parentheses it had
//opened - `(Nadaar, Selfless Paladin #1 (4/4) - "Vigilance -)`, `(goblin lair
//(room 2 of 7 in Lost Mine of Phandel)`, `(Hold priority - pass now, and do not
//ask me agai)`. 79 of 279 exemplified asks were malformed and 20 reasonings
//stopped to call the prompt a typo - the surface teaching the model to distrust
//it (trust doctrine). So: cut at the row's own first separator (` - ` or ` (`),
//which is where its NAME ends and its gloss begins; if the name alone is still
//over the ceiling, cut at a whitespace boundary; and never return a string whose
//quotes or parens are unbalanced. Pure, so every shape pins without a board.
static string exampleCoreTrim(const string& coreIn, size_t maxLen)
{
    string core = coreIn;
    if (core.size() <= maxLen)
        return core; //#W73-BZ (N10): a row that fits was never the defect
    size_t cut = string::npos;
    const size_t dash = core.find(" - ");
    const size_t paren = core.find(" (");
    if (dash != string::npos) cut = dash;
    if (paren != string::npos && (cut == string::npos || paren < cut)) cut = paren;
    if (cut != string::npos && cut > 0)
        core = core.substr(0, cut);
    if (core.size() > maxLen)
    {
        size_t at = core.rfind(' ', maxLen);
        core = (at == string::npos || at == 0) ? core.substr(0, maxLen) : core.substr(0, at);
    }
    //balance: drop trailing tokens until every quote is paired and no bracket is
    //left open. Bounded by the string's own token count.
    for (;;)
    {
        int paren2 = 0, brace = 0, square = 0, quote = 0;
        bool bad = false;
        for (size_t i = 0; i < core.size(); i++)
        {
            const char c = core[i];
            if (c == '(') paren2++;
            else if (c == ')') { if (--paren2 < 0) bad = true; }
            else if (c == '{') brace++;
            else if (c == '}') { if (--brace < 0) bad = true; }
            else if (c == '[') square++;
            else if (c == ']') { if (--square < 0) bad = true; }
            else if (c == '"') quote++;
        }
        if (!bad && paren2 == 0 && brace == 0 && square == 0 && (quote % 2) == 0)
            break;
        const size_t at = core.find_last_of(" ([{\"");
        if (at == string::npos)
        {
            core.clear();
            break;
        }
        core = core.substr(0, at);
    }
    while (!core.empty()
           && (isspace((unsigned char) core[core.size() - 1]) || core[core.size() - 1] == ','
               || core[core.size() - 1] == '-' || core[core.size() - 1] == ':'))
        core.erase(core.size() - 1);
    return core;
}

string askExemplar(const vector<string>& options, int * usedRow)
{
    size_t pick = 0;
    bool anyLive = false;
    //#W67-AY (MED): a WHOLLY live row first...
    for (size_t i = 0; i < options.size(); i++)
        if (!AIPlayerGPT::rowSaysNoOp(options[i]) && !rowSaysHalfDead(options[i]))
        {
            pick = i;
            anyLive = true;
            break;
        }
    //...and only then a half-dead one, which is still a legal answer.
    for (size_t i = 0; !anyLive && i < options.size(); i++)
        if (!AIPlayerGPT::rowSaysNoOp(options[i]))
        {
            pick = i;
            anyLive = true;
            break;
        }
    if (!options.empty() && !anyLive)
    {
        const int decline = declineRowIndexOf(options);
        if (decline >= 0)
            pick = (size_t) decline; //passing is legal: exemplify the pass
        else
        {
            if (usedRow)
                *usedRow = 0;
            return string("CHOICE: <row number> (<that row's short name>)");
        }
    }
    if (usedRow)
        *usedRow = options.empty() ? 1 : (int) pick + 1;
    string core = options.empty() ? string("Example option") : stripNarrationDecoration(options[pick]);
    size_t brace = core.find(" {");
    if (brace != string::npos)
        core = core.substr(0, brace);
    //#W74-CC (O21, wave-73 deck146 MED 4): the sentence carrying this example
    //says "copy nothing from the {...} annotations", and deck130 seq 14's
    //example was `CHOICE: 1 (Brightclimb Pathway (menu text: Play Land))` -
    //an annotation, in the example, under that instruction. The model called it
    //"slightly contradictory" and copied the string anyway. `(menu text: ...)`
    //is a RENDER gloss, not part of the row's short name, so it leaves the
    //example the same way a `{...}` tail does. Only that exact spelling is cut:
    //rows whose short name genuinely ends in parentheses (the mulligan row's
    //"(a keep after this one would keep 6 cards)") are untouched.
    {
        const size_t mt = core.find(" (menu text: ");
        if (mt != string::npos)
            core = core.substr(0, mt);
    }
    while (!core.empty() && isspace((unsigned char) core[core.size() - 1]))
        core.erase(core.size() - 1);
    core = stripTrailingPT(core); //#W50-Y D8
    {
        const string trimmed = exampleCoreTrim(core, 48); //#W73-BZ (N10)
        //a row whose whole core is one unbalanced token leaves nothing to quote:
        //fall back to the FORMAT template rather than print an empty name.
        if (trimmed.empty())
        {
            if (usedRow)
                *usedRow = 0;
            return string("CHOICE: <row number> (<that row's short name>)");
        }
        core = trimmed;
    }
    std::ostringstream ex;
    ex << "CHOICE: " << (options.empty() ? 1 : (int) pick + 1) << " (" << core << ")";
    return ex.str();
}


//#W66-AU (R5): the sentence that carries the example. When no row is
//exemplified (every row on the menu is dead and none of them declines) it says
//so, and says what to do instead, rather than pointing at a row that does
//nothing. Pure, so both faces are pinned without a game.
static string exemplarSentence(const string& exemplarText, int exemplarRow)
{
    std::ostringstream o;
    o << " and its SHORT NAME in parentheses (the name only - copy nothing from the"
         " {...} annotations), e.g. \"" << exemplarText << "\"";
    //#W80-DG (U5, wave-79 known-bugs HIGH): row -2 is the NEUTRAL template the
    //ask seam now uses everywhere wave 79 wrote a live row out. The example was
    //built from row 1 (`e.g. "CHOICE: 1 (Cast Dictate of Kruphix)"`) while the
    //priority menu's has always been a placeholder; deck162's reasoning quoted
    //the example three times and "picked Dictate as it's first in the menu", and
    //the seat took the exemplified row in 62 of 139 windows. The FORMAT is what
    //the example is for - see #W77-CS, which already made exactly this move on
    //the one seam where the row was a priced choice.
    if (exemplarRow == -2)
        o << " (a worked example of the FORMAT only - the number and the name in it are"
             " placeholders, not a recommendation: read the rows below and answer with the"
             " number and short name of the row YOU want)";
    //#W77-CS (R10): row -1 is the ANNOUNCE_X template - a FORMAT example with
    //no claim about the rows at all. Row 0 keeps its wave-66 dead-menu face.
    else if (exemplarRow < 0)
        o << " (a worked example of the FORMAT only - <n> is a placeholder, not a"
             " recommendation: read the rows below and choose the X YOU want)";
    else if (exemplarRow == 0)
        o << " (a worked example of the FORMAT only - every row on this list does nothing"
             " right now, so no row is exemplified: pick the least harmful)";
    else
        o << " (a worked example of the format, written out from row " << exemplarRow
          << " of this list - choose the option YOU want)";
    return o.str();
}


//#W59-J (K10): may the seat's own last answer stand for this window? Every
//clause is a fact about what the MODEL was shown, never about the board the
//engine holds: the same turn and phase, the same decision, the same printed
//rows byte for byte (`nowKey` carries all four), and the same plan it wrote
//that answer under. A valid answer only - a fallback is never re-served. The
//miss direction is asking again, which is the wave-49 default and costs a
//round trip, so every doubt resolves to false. Pure, so PARSETEST can walk a
//whole loop of windows without a game.
bool repeatAskAnswerStands(const string& heldKey, const string& nowKey,
                                  const string& heldPlan, const string& nowPlan,
                                  int heldTurn, int nowTurn, int heldChoice, int nOptions)
{
    if (heldTurn < 0 || nowTurn < 0 || heldTurn != nowTurn)
        return false;
    if (heldChoice < 1 || heldChoice > nOptions)
        return false;
    if (heldKey.empty() || heldKey != nowKey)
        return false;
    return heldPlan == nowPlan;
}


//#W59-J (K10): the key - turn, phase, the decision line and the printed rows,
//joined with a separator no row can contain.
static string repeatAskKey(int turn, int phase, const string& decision,
                           const vector<string>& rows)
{
    std::ostringstream o;
    o << turn << '\x1f' << phase << '\x1f' << decision;
    for (size_t i = 0; i < rows.size(); i++)
        o << '\x1f' << rows[i];
    return o.str();
}


//#W76-CN (Q13, wave-75 deck152 HIGH-3). THE SAME THREE-ROW MENU, PUT TEN TIMES
//ACROSS ONE OPPONENT TURN. `152v125` deck152 seqs 25-34 (t12, Upkeep ->
//Cleanup): rows byte-identical every time, each one a full model call, ~13 min.
//The hold-check said `11 windows in a row`, the prompt-only note said
//`declined 9 times`, and `identical_ask_answers_reserved` fired ZERO times
//inside it - because the replay key above includes the PHASE, and every re-put
//was at a different one. THE WINDOW ITSELF MUST STAY: a spell's value differs by
//phase, and no legal option may be removed, capped or auto-answered. So this is
//a MEASURE plus an ANNOTATION. The key is the repeat key with the phase taken
//out; the entry is written only when the model ANSWERED the window (LESSON OF
//WAVE 75: a bracket that refers to history refers to ASKED history).
//#W76-CQ (F3, Astra wave-76 finding 3). TAKING THE PHASE OUT OF THE KEY MEANT
//TAKING IT OUT OF THE DECISION TEXT TOO. The key above dropped the numeric
//phase and kept the `decision` STRING, and every seam header builds that string
//with the phase inside it - the live casting seam writes
//`Casting decision (Upkeep, opponent's turn): which card do you cast now, if
//any?` and, one phase later, `Casting decision (Draw, opponent's turn): ...`.
//So the ten-window `152v125` shape this measure exists for could never match,
//and the wave-76 pin supplied the artificial string `"Casting decision"`, which
//no caller ever passes. The turn-side clause is what identifies the group the
//seam headers all build (`(<phase>, YOUR turn)` / `(<phase>, opponent's turn)`),
//so it is removed by that shape, whatever the phase name is. Pure over the
//string, and the LIVE strings are what the pin feeds it.
static string w76PhaseFreeDecision(const string& decision)
{
    size_t a = decision.find('(');
    while (a != string::npos)
    {
        const size_t b = decision.find(')', a);
        if (b == string::npos)
            break;
        const string body = decision.substr(a + 1, b - a - 1);
        if (body.find(", YOUR turn") != string::npos
            || body.find(", opponent's turn") != string::npos)
            return decision.substr(0, a) + decision.substr(b + 1);
        a = decision.find('(', b + 1);
    }
    return decision;
}


//#W76-CQ (F4): the board this window was asked over, WITHOUT the phase line the
//state serialiser opens with ("Phase: <name> | It is <who>'s turn.\n"). Keeping
//that line would make the comparison false by construction at every phase, and
//a clause that can never print is not a check. Everything else the serialiser
//prints - permanents, zones, counters, life, the stack - is compared.
static string w76PhaseFreeBoardKey(const string& board)
{
    if (board.compare(0, 7, "Phase: ") != 0)
        return board;
    const size_t nl = board.find('\n');
    return (nl == string::npos) ? string() : board.substr(nl + 1);
}


static string w76CrossPhaseListKey(int turn, const string& decision,
                                   const vector<string>& rows)
{
    return repeatAskKey(turn, -1, w76PhaseFreeDecision(decision), rows); //#W76-CQ (F3)
}


//The annotation. Prompt-only, a bracket, outside every key - the same channel
//and the same shape as the declined count and the hold check.
//#W76-CQ (F4): ...and the board claim is made only when the board was compared
//and matched. An unrelated permanent entering between the two asks makes the old
//sentence false while turn, question and rows all still match - so with a moved
//board the note states the re-put and stops there.
static string w76CrossPhaseRePutNote(int windowsAgo, const string& phaseName,
                                     bool boardUnchanged)
{
    if (windowsAgo < 1 || phaseName.empty())
        return string();
    std::ostringstream o;
    o << "\n[this exact list was put to you " << windowsAgo << " window"
      << (windowsAgo == 1 ? "" : "s") << " ago at " << phaseName
      << " and you declined";
    if (boardUnchanged)
        o << "; nothing on the board has changed";
    else
        //#W77-CR (R8, wave-76 engine-seat MED-1). THE NEGATIVE SENTENCE SAID
        //THE ONE THING THAT UNDOES THE BRACKET. 104 of the corpus's 109 printed
        //`the board HAS moved since then` - and every one of those boards really
        //had moved (the ask-cache board key is the whole serialised state below
        //the phase line; between two phases of one turn a draw step, a life
        //change or a permanent entering moves it for real, so the comparison is
        //NOT too strict and widening it would make the positive clause a lie).
        //What was wrong is what the seat was left with: told a list it had
        //already declined was being re-put AND that the board had moved, the
        //model's cheapest reading is "re-read everything", which is the opposite
        //of the bracket's purpose. So the negative branch states the fact the
        //key GUARANTEES - the rows are byte-identical, which is what made this a
        //re-put at all - beside the fact that the board is not.
        o << "; the board has moved since then, but this list has not - not one"
             " row on it appeared, went away, or changed a card, cost or target";
    o << "]";
    return o.str();
}


int AIPlayerGPT::askModel(const string& decision, const vector<string>& optionsIn, bool narrateChoice,
                          const string& pendingSourceName, bool askEvenIfSingle,
                          bool suppressPlanRequest, bool declineRowOffered) //#W62-Y (D5)
{
    //W35: consume the caller's per-option register lines FIRST, on every exit
    //path, so a set of lines can never leak onto a later, unrelated ask.
    vector<string> askNarration;
    askNarration.swap(mNextAskNarration);
    //#W53-N (D2): the caller's prompt-only note, consumed on EVERY exit path
    //so it can never leak onto a later, unrelated ask.
    string promptOnlyNote;
    promptOnlyNote.swap(mNextAskPromptNote);
    //#W79-DC (F1): and the caller's legal-continuation digest, consumed on EVERY
    //exit path for the same reason - a digest staged for one menu must never key
    //a later ask at another seam (an unstaged seam keys on the scope alone, which
    //is #W79-CZ's shipped behaviour for it).
    //#W80-DH (F10): THE SEND BOUNDARY IS IN HERE. The caller stages its verdict
    //faces and this guard applies them exactly once - `true` at the line that hands
    //the prompt over, and `false` on EVERY other way out of this function (the ask
    //cache, the repeat latch, the cross-phase replay, a single/resolved option, no
    //endpoint). A destructor rather than a `false` call at each `return`, because
    //the exits are twelve and the next one added would be silently uncounted. The
    //staging is swapped out first, so it can never leak onto a later ask.
    struct W80SendFaceGuard
    {
        AIPlayerGPT * self;
        std::string loop, crackBack, stackDeath, castReopen; //#W81-DK (V4)
        bool drain, done;
        W80SendFaceGuard(AIPlayerGPT * s) : self(s), drain(false), done(false) {}
        void apply(bool sent)
        {
            if (done)
                return;
            done = true;
            bool counted = false;
            w79ApplyLoopFaceAtSend(sent, loop, self->mOwnLoopVerdictFace, counted);
            if (counted)
                self->w79CountOwnLoopVerdictLine();
            self->w80ApplyVerdictFacesAtSend(sent, crackBack, stackDeath, drain);
            //#W81-DK (V4): the cast-reopen reason rides the same boundary.
            if (sent)
                self->mCastReopenFace = castReopen;
        }
        ~W80SendFaceGuard() { apply(false); }
    } w80SendFaces(this);
    w80SendFaces.loop.swap(mNextSendLoopFace);
    w80SendFaces.crackBack.swap(mNextSendCrackBack);
    w80SendFaces.stackDeath.swap(mNextSendStackDeath);
    w80SendFaces.castReopen.swap(mNextSendCastReopen); //#W81-DK (V4)
    w80SendFaces.drain = mNextSendDrain;
    mNextSendDrain = false;
    //#W75-CI (P19): the hold contract, once per prompt, on exactly the windows
    //that offer the row - one site, so the three row spellings and both seams
    //cannot drift. Matched on the shared head every other consumer binds by.
    for (size_t hcp = 0; hcp < optionsIn.size(); hcp++)
        if (optionsIn[hcp].compare(0, strlen(kHoldPriorityRowShortHead),
                                   kHoldPriorityRowShortHead) == 0)
        {
            promptOnlyNote += holdContractParagraph();
            break;
        }
    //#W54-M (A19): a caller's already-rendered situation, consumed on EVERY
    //exit path (the discipline of the two swaps above) so a stale render can
    //never key a later ask. It is byte-identical to the render this function
    //would do itself: the caller rendered it in this same call stack with no
    //engine tick in between (FindCardToPlay's boardNow).
    string situationPrefill;
    situationPrefill.swap(mAskSituationPrefill);
    //#W60-M (B13c): this window has not been reserved until one of the two
    //answer-reuse returns below says so. Set here, once, so every other exit
    //of this function (a fresh model call, a heuristic handoff, a resolved
    //single/interchangeable option) leaves it false.
    mAskAnswerReserved = false;
    //#W54-M (A17): the state+question cache can only hit within a turn (the
    //key embeds the turn header and the board), so the turn boundary is a free
    //point to drop the dead keys - median 99 KB, max 1.59 MB per seat-game were
    //held for the whole game on the Vita heap. Same-turn re-polls (the
    //`latency_ms -1` replay records) stay cached: nothing is cleared mid-turn.
    if (!auditMOff() && mAskCacheTurn != observer->turn)
    {
        mAskCache.clear();
        mAskCacheSeq.clear(); //#W71-BP (L2): the provenance map is the cache's shadow
        mAskReplayRuns.clear(); //#W72-BT (M22): the per-window runs die with the keys
        //#W73-CA (N8): the repeat latch's runs are turn-scoped too - its own key
        //embeds the turn, so they are dead keys from here on either way.
        mRepeatAskRuns.clear();
        mAskCacheTurn = observer->turn;
    }
    //"Only one valid action": no decision to make, no model call.
    if (optionsIn.empty())
        return -1;
    //One option = no decision, so no model call - EXCEPT where the caller says
    //the question itself is owed (see askEvenIfSingle in the header: the X
    //announcement). A zero-slack {X} cast has exactly one announceable value,
    //and taking it silently is the very silence #W41-1 forbids.
    if (optionsIn.size() == 1 && !askEvenIfSingle)
        return 0;
    //#W82-A (L1, audit-2026-09): the byte-identical-rows auto-answer is DELETED.
    //`identicalInterchangeableRows` proved equality of the RENDERED STRINGS, not
    //equality of the engine consequences behind them, and the ruling carves out
    //no "same outcome" exemption: "you cant decide that a play is bad and
    //therefore never offer it". The one-option shortcut above stands - a menu
    //with exactly one legal row is a mechanism, not a choice.
    if (mEndpoint.empty())
        return -1; //no endpoint: caller falls back to the heuristic

    //#W53-P (D7, wave-52 ledger MED): the ABILITY/target menus reached
    //joinNumberedRows but never groupNumberedRows, so the collapse here was
    //ADJACENCY-ONLY - and the engine orders these lists lexicographically on
    //the target name ("Human #1, #10, #100, #11 ..."), which is precisely the
    //order in which consecutive ranks almost never sit together. The priority
    //menu has gathered repeated rows before collapsing since wave 48; this is
    //the same gather, one seam later. Nothing is deleted and no option is
    //merged away: the permutation is applied to the rows the MODEL reads and to
    //the caller-supplied per-option narration, and the chosen number is mapped
    //straight back to the caller's own index before it is returned or cached,
    //so every caller's index -> action mapping is byte-identical to before.
    vector<size_t> askOrder;
    groupNumberedRows(optionsIn, askOrder);
    bool askReordered = false;
    for (size_t k = 0; k < askOrder.size() && !askReordered; k++)
        askReordered = (askOrder[k] != k);
    if (askReordered && askOrder.size() != optionsIn.size())
        askReordered = false; //defensive: a non-permutation is not applied
    vector<string> askRows;
    if (askReordered)
    {
        askRows.reserve(optionsIn.size());
        for (size_t k = 0; k < askOrder.size(); k++)
            askRows.push_back(optionsIn[askOrder[k]]);
        if (askNarration.size() == optionsIn.size())
        {
            vector<string> permuted;
            permuted.reserve(askNarration.size());
            for (size_t k = 0; k < askOrder.size(); k++)
                permuted.push_back(askNarration[askOrder[k]]);
            askNarration.swap(permuted);
        }
    }
    const vector<string>& options = askReordered ? askRows : optionsIn;

    if (mSystemPrompt.empty())
        buildSystemPrompt();

    std::ostringstream tail;
    tail << decision << "\n";
    //#W48 (D2): the same numbered-row collapse the priority menu uses. A target
    //menu over a wide token board is the widest list this seam renders; the
    //option INDICES are unchanged (a collapsed row prints its label range) and
    //`options` itself is what parseChoice reads, so the answer surface is
    //untouched by the render.
    {
        bool anyOptionRangeRow = false;
        unsigned rangeForms = 0; //#W82-P8
        tail << joinNumberedRows(options, &anyOptionRangeRow, &rangeForms);
        if (anyOptionRangeRow)
            tail << optionRangeNote(rangeForms);
    }
    //#W53-N (D2): where the option list ends - the prompt-only decline note is
    //spliced in here, after the list it is about and out of the ask key.
    const size_t askOptionsEnd = (size_t) tail.tellp(); //#W54-M (L5)
    //#W47 (wave-46 E-2): the worked example names THIS window's option 1, not a
    //hard-coded "Cast Example Card" - on cast-free screens (damage order, mode
    //menus) that placeholder was the only affirmative "Cast ..." substring and
    //the model latched it (4 replies, 2 stale_echo fallbacks in wave 46).
    //(history: comment-archaeology.md AIPlayerGPTSeams-L2100-1980)
    //#W62-Y (D5): kNoPassRowFact is a claim about THIS menu, and on a menu the
    //caller gave a decline row it is false. Say which of the two is true; the
    //row itself is the caller's, this line only stops contradicting it.
    //#W66-AR (H8): built BEFORE the stream chain - the row number and the example
    //string must be the same evaluation, and argument order inside one chain of
    //<< is unsequenced.
    int exemplarRow = 1;
    string exemplarText = askExemplar(options, &exemplarRow);
    //#W77-CS (R10, wave-76 deck130 MED): on the ANNOUNCE_X menu the worked
    //example was written out from ROW 1 like every other seam, and row 1 of an
    //X ladder is a LIVE, PRICED choice. `130v162` deck130 seq 60 answered
    //`CHOICE: 1 (X = 1)` on a row reading `kills THEIRS: none; YOURS: Dwarven
    //Blastminer` - it copied the exemplar, which on this one menu names a real
    //X the seat is being asked to weigh. The format is what the example is for,
    //so on this seam it is a neutral template: the shape, with no number the
    //model can read as a recommendation. Nothing else about the menu moves.
    if (mInAnnounceXAsk)
    {
        exemplarText = "CHOICE: <n> (X = <n>)";
        exemplarRow = -1; //#W77-CS: a template, not a row - see exemplarSentence
    }
    //#W80-DG (U5): ...and the same on every other ask. A quoted LIVE row is a
    //recommendation the sentence never meant to make; the dead-menu face (row 0)
    //is a claim about the ROWS, not an example to copy, so it is untouched.
    else if (exemplarRow > 0)
    {
        exemplarText = "CHOICE: <row number> (<that row's short name>)";
        exemplarRow = -2;
    }
    tail << "\n" << kPlanFirstLead //#W70-BL (E2)
         << "on a line of its own CHOICE: followed by the number of your choice "
         //#W64-AH (F2): read off the ROWS this window renders, with the
         //caller's flag kept as the fallback - see declineFactForMenu.
         << declineFactForMenu(options, declineRowOffered)
         << exemplarSentence(exemplarText, exemplarRow) //#W66-AU (R5)
         << " Write nothing else.";
    string tailStr = tail.str();

    //State-plus-question answer cache: the same questions are re-polled
    //every AI tick until the game state moves on, and several distinct
    //questions can alternate within one tick. The key excludes the
    //narration and the plan (see the header) so that consuming one answer
    //cannot invalidate another already given for this same state - the
    //earlier picks of a multi-target selection re-derive from this cache.
    //#W74-CG: the KEY half of the rendered list - the decline row's re-ask
    //clause carries a count that rises with every answer, and leaving it in the
    //key minted a fresh question per window (384 model calls for one turn-4
    //Main-1 window, run 20260909-015553). The model still reads `tailStr`.
    string keyTailStr = w77KeyTailOf(tailStr); //#W77-CU (F1)
    //#W76-CQ (F4): the board half, named once so the cross-phase entry below can
    //carry the SAME bytes the ask cache keys on rather than a second rendering.
    const string boardStateKey =
        (situationPrefill.empty() || auditMOff()) ? serializeGameState() : situationPrefill;
    //#W82-A (L2): BOARD STATE + QUESTION. The `mAskScopeDigest` census that used
    //to sit here - and grow, unretired, for the length of the game (L10) - existed
    //only to count the windows the boardless key would have mis-served. There are
    //none left to count.
    string askKey0 = w82WindowKey(boardStateKey, keyTailStr);
    //#W49-S (D8): this state+question already earned its one re-ask - the
    //corrected question is THE question from here on (its own cache slot).
    bool reasked = (!mAskReaskKey.empty() && mAskReaskKey == askKey0);
    if (reasked)
    {
        tailStr += "\n" + mAskReaskLine;
        keyTailStr += "\n" + mAskReaskLine; //#W74-CG: the slot key tracks the render
    }
    string askKey = reasked ? askKey0 + "\n" + mAskReaskLine : askKey0; //#W54-M (A19): same bytes, no second render
    //#W81-DK (V1, wave-80 known-bugs V1): a STATE-BASED ACTION window is never
    //served from either re-serve cache. `146v162` seq 27 (`replayed_from` 24)
    //answered a LEGEND RULE window - "pick the copy that GOES TO ITS OWNER'S
    //GRAVEYARD now" - from this cache, with no round trip: a cached answer that
    //DESTROYS a permanent, which is the class #W81-DI bounds at the priority seam.
    //The rule applies to objects the engine re-derives every time it applies, so
    //"the same question over a board that has not moved" is never true of one.
    //Counted and traced at the SEND, below.
    //#W81-DK (V3): the key of the window being put RIGHT NOW, so `takeHold` can
    //record which cache entry produced the hold row and a re-open can invalidate it
    //(`125v162` seqs 18/20/22/24: four `reopen_new_lethal` events, the hold latch
    //re-clamped from this very cache each time, the seat was never asked, and died).
    mLastAskKeyBuilt = askKey;
    const bool w81SbaBypass = !w81AskCacheUsable(mInStateBasedActionAsk);
    std::map<string, int>::iterator cached = w81SbaBypass ? mAskCache.end()
                                                          : mAskCache.find(askKey);
    if (w81SbaBypass)
        DebugTrace("AIPlayerGPT: a state-based-action window (the legend rule) - neither"
                   " answer cache may serve it; asking the model");
    if (cached != mAskCache.end())
    {
        //#W71-BP (L1 c / L2). Two changes to a path that used to be completely
        //silent. (1) REFUSE a key that has been replayed kAskReplayRefuseMax times
        //running: in the wave-70 hang both halves of this key were byte-identical
        //every tick because the answer changed nothing, so the cache served the same
        //decline 2,584,190 times with no request, no trace and no record. Dropping
        //the entry sends the window back to the model, which either breaks the loop
        //or turns it into traffic a corpus can see. (2) RECORD every replay, with
        //the seq it came from - a hung seat log must never again end mid-game with
        //no error in it.
        std::map<string, int>::iterator cs = mAskCacheSeq.find(askKey);
        const int fromSeq = (cs == mAskCacheSeq.end()) ? -1 : cs->second;
        if (askReplayRefuseScoped(askKey, mAskReplayRuns, kAskReplayRefuseMax)) //#W72-BT (M22)
        {
            mAskReplaysRefused++;
            logAskReplay("cache_replay_refused", decision, cached->second,
                         (int) options.size(), fromSeq, kAskReplayRefuseMax);
            DebugTrace("AIPlayerGPT[" << deckFileSmall << "]: refusing the ask cache after "
                       << kAskReplayRefuseMax << " identical replays of the same state+question"
                       << " (answer " << cached->second << ", first served at seq " << fromSeq
                       << ", " << mAskReplaysRefused << " refusals this game): " << decision);
            mAskCache.erase(cached);
            mAskCacheSeq.erase(askKey);
            //#W71-BS (F4, Astra review finding 4): the refusal must invalidate BOTH
            //re-serve paths or it invalidates neither. `mRepeatAskKey` is the second
            //cache below, and it is keyed on turn+phase+question+rows, which in the
            //livelock is byte-identical too: erasing only the first cache dropped the
            //window straight into `repeatAskAnswerStands()`, which returned the SAME
            //answer without a request, and every later tick then MISSED the first
            //cache and never reached the refusal check again. A refused window goes to
            //the model or it is not refused.
            mRepeatAskKey.clear();
            mRepeatAskTurn = -1;
            mRepeatAskSeq = -1;
        }
        else
        {
            mAskAnswerReserved = true; //#W60-M (B13c): a replay, not a window the model saw
            mAskReplaysReserved++;
            mAskReplaysCache++; //#W80-DE (U9): which PATH re-served it
            logAskReplay("cache_replay", decision, cached->second, (int) options.size(),
                         fromSeq, mAskReplayRuns[askKey]); //#W72-BT (M22): this window's own run
            return (cached->second >= 1 && cached->second <= (int) options.size()) ? cached->second - 1 : -1;
        }
    }
    //#W59-J (K10): the same question, asked again with the board moved under it.
    //The cache above wants the board too, and a resolving drain loop moves it
    //every iteration - 32 identical windows in one upkeep, all answered by the
    //model, all with the same answer. Re-serve the seat's own answer while the
    //rows and the plan are untouched, and count it onto the gameend record the
    //way hold_windows_skipped is, so a corpus can still see the window.
    const string nowRepeatKey = repeatAskKey(observer ? observer->turn : -1,
                                             observer ? (int) observer->getCurrentGamePhase() : -1,
                                             decision, optionsIn);
    //#W81-DK (V1): BOTH re-serve paths are bypassed for an SBA window or neither is
    //(the #W71-BS F4 rule: a window that must go to the model goes to the model, or
    //it silently falls into the second cache).
    if (!w81SbaBypass
        && repeatAskAnswerStands(mRepeatAskKey, nowRepeatKey, mRepeatAskPlan, mCurrentPlan,
                                 mRepeatAskTurn, observer ? observer->turn : -1,
                                 mRepeatAskChoice, (int) optionsIn.size()))
    {
        //#W73-CA (N8, wave-72 engine-seat MED-2, second half): BOTH re-serve
        //paths are bounded or neither is. The ask cache above has refused after
        //kAskReplayRefuseMax identical replays since #W71-BP; this path - now 327
        //of the corpus's 521 replays, up from 68 of 725 - was bounded only by the
        //turn boundary, so the wave-71 drain loop's successor could live here
        //unrefusable. The same predicate, the same cap, the same consequence: the
        //latch is dropped and the window goes to the model.
        if (askReplayRefuseScoped(nowRepeatKey, mRepeatAskRuns, kAskReplayRefuseMax))
        {
            mAskReplaysRefused++;
            logAskReplay("repeat_ask_refused", decision, mRepeatAskChoice,
                         (int) optionsIn.size(), mRepeatAskSeq, kAskReplayRefuseMax);
            DebugTrace("AIPlayerGPT[" << deckFileSmall << "]: refusing the repeat latch after "
                       << kAskReplayRefuseMax << " identical re-serves of the same turn+question"
                       << " (answer " << mRepeatAskChoice << ", first served at seq "
                       << mRepeatAskSeq << ", " << mAskReplaysRefused
                       << " refusals this game): " << decision);
            mRepeatAskKey.clear();
            mRepeatAskTurn = -1;
            mRepeatAskSeq = -1;
        }
        else
        {
        mRepeatAskAnswersReserved++;
        mAskAnswerReserved = true; //#W60-M (B13c): the model was not shown this window
        //#W71-BP (L2): the OTHER silent re-serve path. Same argument, same record.
        mAskReplaysReserved++;
        mAskReplaysRepeatLatch++; //#W80-DE (U9): which PATH re-served it
        logAskReplay("repeat_ask_reserved", decision, mRepeatAskChoice,
                     (int) optionsIn.size(), mRepeatAskSeq,
                     mRepeatAskRuns[nowRepeatKey]); //#W73-CA (N8): this window's run
                                                    //#W73-CB (F7): the game total is the
                                                    //record's own combined counter
        DebugTrace("AIPlayerGPT[" << deckFileSmall << "]: the same ask again, unchanged - re-serving"
                   " this seat's own answer " << mRepeatAskChoice << " of " << optionsIn.size()
                   << " (" << mRepeatAskAnswersReserved << " this game): " << decision);
        return mRepeatAskChoice - 1;
        }
    }

    //#W76-CN (Q13): this exact list, at this turn, already ASKED and declined at
    //a DIFFERENT phase. Counted, and said on the prompt-only channel. Nothing is
    //collapsed: every row stays answerable, and the window is still put.
    const string crossPhaseKey = w76CrossPhaseListKey(observer ? observer->turn : -1,
                                                      decision, optionsIn);
    W76CrossPhaseAsk * w80PendingCrossPhase = NULL;  //#W80-DE (U15): counted at the send
    bool w80PendingCrossPhaseUnchanged = false;
    {
        //the key is turn-scoped, so nothing older than this turn can ever match:
        //drop it rather than grow a map for the length of a 77-turn game.
        const int turnNow = observer ? observer->turn : -1;
        if (turnNow != mCrossPhaseTurn)
        {
            mCrossPhaseAsks.clear();
            mCrossPhaseTurn = turnNow;
        }
    }
    {
        std::map<string, W76CrossPhaseAsk>::iterator cp = mCrossPhaseAsks.find(crossPhaseKey);
        const string phaseNow = observer ? observer->getCurrentGamePhaseName() : string();
        if (cp != mCrossPhaseAsks.end() && cp->second.declined
            && !cp->second.phaseName.empty() && cp->second.phaseName != phaseNow)
        {
            //#W76-CQ (F4): ONE COUNT PER ASK, not one per polling tick. This
            //prompt is rebuilt on every tick while an answer is in flight and
            //the increment sat above the pending-poll return, so a single
            //qualifying asynchronous ask was counted again on every tick. The
            //window's own seq is the identity of the ask.
            //...and the board claim is a comparison, not an assumption.
            const bool boardUnchanged =
                !cp->second.boardKey.empty()
                && cp->second.boardKey == w76PhaseFreeBoardKey(boardStateKey);
            //#W80-DE (U15): STAGED here, counted at the send below - the wave-79
            //census read 172 against 170 rendered notes because two counted
            //windows never reached a prompt.
            if (cp->second.countedSeq != mWindowSeq)
            {
                w80PendingCrossPhase = &cp->second;
                w80PendingCrossPhaseUnchanged = boardUnchanged;
            }
            promptOnlyNote += w76CrossPhaseRePutNote(mWindowSeq - cp->second.windowSeq,
                                                     cp->second.phaseName, boardUnchanged);
            //#W82-A (L2): the cross-phase REPLAY is DELETED. It served this
            //seat's own earlier answer at a LATER PHASE - a phase the board
            //serialisation's own header names, so under "board state + question"
            //it is a different question and the model is asked. The prompt-only
            //NOTE above stays: telling the pilot that it already declined this
            //exact list earlier in the turn is a fact, and it costs no window.
            //(4 windows in the wave-80 corpus; `w80CrossPhaseReplayable` and its
            //bookkeeping go with the branch.)
        }
    }
    string userTail = tailStr;
    if (!promptOnlyNote.empty() && askOptionsEnd <= userTail.size())
        userTail = userTail.substr(0, askOptionsEnd) + promptOnlyNote + userTail.substr(askOptionsEnd);
    //#W57-H (D43): this window's ask class, for the log window and the record.
    mLogWindowKind = askWindowKindForAsk(decision, options);
    string userMsg = assemblePrompt(userTail, NULL, &keyTailStr); //#W62-fix: notes stay out of the slot key; #W74-CG: and so does the decline count
    //#W80-DE (U15): the cross-phase re-put is counted HERE, at the send, on the
    //same one-per-window gate - so `crossphase_identical_reputs` is by
    //construction the number of prompts the note is in.
    if (w80PendingCrossPhase && w80PendingCrossPhase->countedSeq != mWindowSeq)
    {
        w80PendingCrossPhase->countedSeq = mWindowSeq;
        mCrossPhaseRePuts++;
        if (w80PendingCrossPhaseUnchanged)
            mCrossPhaseBoardUnchanged++;
    }
    //#W80-DH (F10): THE SEND. Everything that can answer without a round trip has
    //returned by now, so this is the boundary the counters and the record faces
    //belong to.
    //#W81-DK (V9): ...but the CALLER does not get to assert it. `sent` is what the
    //TRANSPORT did with this prompt (`mTransportHandedOff`, set in pollCompletion at
    //the stub and at the spawned worker), read AFTER the call. Three dispositions:
    //handed off (count and stamp), already in flight (touch nothing - the face
    //stamped at the real send must survive to the record), or suppressed with no
    //transport call at all (the guard's destructor, on the exits above).
    mTransportHandedOff = false;
    string content;
    const int w81Poll = pollCompletionRetry(userMsg, content, "ask");
    const int w81Disp = w81SendDisposition(mTransportHandedOff, w81Poll == kChoicePending);
    if (w81Disp == kW81SendHandedOff)
    {
        w80SendFaces.apply(true);
        //#W81-DK (V1): the SBA bypass is a fact about THIS window's prompt, so it is
        //stamped where the prompt was handed over, like every other send-time face.
        if (w81SbaBypass)
        {
            mSbaWindowsCacheBypassed++;
            mCacheBypassFace = "state_based_action";
        }
    }
    else if (w81Disp == kW81SendInFlight)
        w80SendFaces.done = true; //nothing was sent this tick and nothing is cleared
    if (w81Poll == kChoicePending)
        return kChoicePending; //callers unwind this tick and re-poll

    //Plan split BEFORE choice parsing: plan prose is full of numbers. Restrict
    //the answer line to CHOICE: so a CoT combat line ("Attack: ...") in the
    //reasoning body is not mistaken for the answer (stale-echo family B).
    int choiceRunLen = 0, rejectedLines = 0, planAnswerNote = kPlanAnswerNoteNone;
    string laterIgnored; //#W66-AR (H2c)
    string decisionPart = consumePlan(content, "CHOICE:", &choiceRunLen, &rejectedLines,
                                      &planAnswerNote, &laterIgnored);
    if (!laterIgnored.empty())
        mLastIgnoredAnswerLine = laterIgnored;
    bool staleEcho = false;
    string parseNote;
    int choice = parseChoice(decisionPart, (int) options.size(), &options, &staleEcho,
                             pendingSourceName.empty() ? NULL : &pendingSourceName, &parseNote);
    //#W70-BM (E2) / #W82-A: the adjacent-run fold is deleted; the
    //`multi_answer_first_taken` note is live again since #W81-DM - see
    //chooseOrderedAction.
    if (rejectedLines > 0)
        appendParseNote(&parseNote, "rejected_line_skipped"); //#W50-Y D7
    //#W63-AD (E6b): sign which side of the plan bound this reply fell on.
    if (planAnswerNote == kPlanAnswerExtraLine)
        appendParseNote(&parseNote, "extra_answer_line"); //#W70-BM (E2)
    if (!parseNote.empty())
        mLastParseNote = parseNote;
    //#W71-BO (R9): the label-missing re-ask is DELETED here too.
    //#W70-BM (E2): the RETRACTION GATE is DELETED here too - see the priority
    //seam. #W82-A (audit-2026-09): so are the `retracted` local and the
    //`retracted_choice` class it selected - unassigned, therefore unwritable.

    bool namedRowFail = (choice < 0 && !content.empty()
                         && parseNote.find("named_row_not_offered") != string::npos);
    //#W51-C (D3): an in-range index naming a card no row offers (deck126
    //vs125 seq 14, "CHOICE: 1 (Cast Sanguine Bond)" over Battlement/nothing)
    //is an off-menu name, not a stale echo, unless the name echoes a row of
    //the previous window -> the one named_row re-ask, never Baka first.
    //#W82-A (L8, audit-2026-09): EVERY in-range stale echo takes the named_row
    //RE-ASK lane. The `!nameEchoesRow(...)` guard let a number-vs-name
    //disagreement whose name echoed a PRIOR window's row fall through to
    //`return -1` and the HEURISTIC played the window - discarding a coded answer
    //that was in range and legal. The ruling's remedy for a disagreement is a
    //re-ask, not the heuristic.
    if (staleEcho && choice < 0 && !content.empty()
        && parseNote.find("stale_echo_in_range") != string::npos)
    {
        staleEcho = false;
        namedRowFail = true;
        appendParseNote(&mLastParseNote,
                        nameEchoesRow(headParenthetical(decisionPart.empty() ? content : decisionPart),
                                      mPrevWindowRows)
                            ? "prior_window_name_in_range" : "off_menu_name_in_range");
    }
    bool passOnNoPass = (choice == 0 && !content.empty());
    const bool noopRowZero = false; //#W82-A (L8): the no-op re-ask is DELETED
    if ((namedRowFail || passOnNoPass) && !reasked) //#W82-A (L8): noopRowZero is gone
    {
        std::ostringstream corr;
                if (namedRowFail)
        {
            const string offending = headParenthetical(decisionPart.empty() ? content : decisionPart);
            corr << "[RE-ASK] \"" << offending
                 << "\" is not on this list. Answer with a number from 1 to " << options.size() << "."
                 << laterStepRouteClause(offending, &options); //#W62-Z (D11)
            mAskReaskKind = "named_row";
        }
        else if (passOnNoPass)
        {
            corr << "[RE-ASK] This ask has no pass - 0 is not an answer here. Answer with a number from 1 to "
                 << options.size() << ".";
            mAskReaskKind = "no_pass";
        }
        const char * fb = namedRowFail ? "named_row_reask" : "no_pass_reask";
        mAskReaskKey = askKey0;
        mAskReaskLine = corr.str();
        if (!parseNote.empty())
            mLastParseNote = parseNote;
        writeTransLog("ask", userMsg, content, choice, (int) options.size(), "", fb, &options);
        setNotice(namedRowFail ? "that answer named nothing on the list - asking again"
                               : "that answer passed an ask that has no pass - asking again", 5.0f);
        DebugTrace("AIPlayerGPT: " << fb << " -> re-asking once");
        string corrected;
        //#W74-CG: the corrected leg's slot key is built from the same stripped
        //tail the next tick's rebuild will use, or every retry would drift.
        const string retryKeyTail = keyTailStr + "\n" + mAskReaskLine;
        pollCompletionRetry(assemblePrompt(tailStr + "\n" + mAskReaskLine, NULL, &retryKeyTail),
                            corrected, "ask");
        return kChoicePending; //the caller unwinds; the corrected call answers later
    }
    if (reasked)
    {
        if (mAskReaskKind == "no_pass")
            appendParseNote(&mLastParseNote, passOnNoPass ? "no_pass_reask_exhausted"
                                                          : (choice >= 1 ? "no_pass_reask_recovered" : "no_pass_reask_unanswered"));
        else if (mAskReaskKind == "noop_plan") //#W66-AR (H8) #W69-BH (K4c) #W69-BJ (F8)
            appendParseNote(&mLastParseNote,
                (noopRowZero && choice >= 1 && choice == mAskReaskPriorChoice)
                    ? "noop_row_retaken"
                    : (noopRowZero ? "plan_contradicts_noop_row_exhausted"
                                   : (choice >= 1 ? "plan_contradicts_noop_row_recovered"
                                                  : "plan_contradicts_noop_row_unanswered")));
        else
            appendParseNote(&mLastParseNote, namedRowFail ? "named_row_reask_exhausted"
                                                          : (choice >= 0 ? "named_row_reask_recovered" : "named_row_reask_unanswered"));
        //#W68-BC (MED, engine MED-1): same stamp at the casting seam.
        if (choice >= 0)
            noteReaskExecuted("cast", choice,
                              (choice >= 1 && choice <= (int) options.size())
                                  ? options[choice - 1] : string());
    }
    mPrevWindowRows = options; //#W51-C D3: this window is now the prior one
    if (content.empty())
        noticeFallback("model reply failed or timed out - the heuristic decides", 5.0f);
    else if (narrateChoice && choice >= 1 && choice <= (int) options.size())
    {
        //W35 owner ruling class (2): the consumed ask records its CONSEQUENCE,
        //never the question header plus the decorated option ("A choice is
        //required - choose an option: -> pay 2 life [this permanent then
        //enters ... UNTAPPED - usable ... this turn]"). A caller that knows the
        //consequence supplies a register line per option; otherwise the generic
        //form names the decision without the interrogative furniture. The
        //bracketed decision-time guidance never enters history either way.
        if ((size_t) choice <= askNarration.size() && !askNarration[choice - 1].empty())
            narrateDecision(askNarration[choice - 1]);
        else
            narrateDecision(menuConsequenceNarration(pendingSourceName,
                                stripNarrationDecoration(options[choice - 1])));
    }

    //#W53-P (D7): back to the CALLER's index before anything outside this
    //function sees the number - the cache is keyed on the state+question and is
    //read by the early-return above, which hands its value straight to the
    //caller.
    int callerChoice = choice;
    if (askReordered && choice >= 1 && choice <= (int) askOrder.size())
        callerChoice = (int) askOrder[choice - 1] + 1;
    //#W81-DK (V1): an SBA window's answer is not stored either - storing it would
    //only move the replay one window later.
    if (!w81SbaBypass)
    {
        mAskCache[askKey] = callerChoice;
        mAskCacheSeq[askKey] = mTransSeq; //#W71-BP (L2): where a later replay was served FROM
    }
    mAskReplayKey.clear(); //a real model answer ends any replay run
    //#W72-BT (M22): ...but only for THIS window. Wiping every window's run is
    //what let an interleaved answer hide an 18-deep replay loop.
    mAskReplayRuns.erase(askKey);
    //#W73-CA (N8): the same discipline for the repeat latch's run - a window the
    //model really answered starts its run over, and only THAT window's.
    mRepeatAskRuns.erase(nowRepeatKey);
    //#W59-J (K10): latch the answer for a re-ask of this exact window. Only a
    //VALID choice: a fallback is not an answer and is never re-served.
    //#W76-CN (Q13): remember this ASKED window's list, its phase and whether the
    //answer declined it, so a later re-put at another phase can say so.
    if (callerChoice >= 1 && callerChoice <= (int) optionsIn.size())
    {
        W76CrossPhaseAsk& e = mCrossPhaseAsks[crossPhaseKey];
        e.phaseName = observer ? observer->getCurrentGamePhaseName() : string();
        e.boardKey = w76PhaseFreeBoardKey(boardStateKey); //#W76-CQ (F4)
        e.countedSeq = -1;
        e.windowSeq = mWindowSeq;
        //#W80-DH (F12): mTransSeq is the seq the ask record written below will
        //take - the same snapshot mAskCacheSeq and mRepeatAskSeq take here.
        e.recordSeq = mTransSeq;
        e.declined = w72RowIsDeclineOrHold(holdKeyRow(optionsIn[callerChoice - 1]));
        //#W80-DE (U8): the answer itself, the rows it was given over, and this
        //window's speed class - the three facts a cross-phase replay is gated on.
        e.choice = callerChoice;
        e.rows = optionsIn;
        e.sorcerySpeed = w80SorcerySpeedWindowNow();
    }
    if (callerChoice >= 1 && callerChoice <= (int) optionsIn.size())
    {
        mRepeatAskKey = nowRepeatKey;
        mRepeatAskPlan = mCurrentPlan;
        mRepeatAskTurn = observer ? observer->turn : -1;
        mRepeatAskChoice = callerChoice;
        mRepeatAskSeq = mTransSeq; //#W71-BP (L2)
    }
    else
    {
        mRepeatAskTurn = -1;
        mRepeatAskKey.clear();
    }
    {
        bool valid = choice >= 1 && choice <= (int) options.size();
        const char * fb = valid ? NULL : (content.empty() ? noAnswerClass() : (staleEcho ? "stale_echo" : (namedRowFail ? "named_row_not_offered" : unparsedReplyClass(content))));
        //#W49-S (D2): what executed IS the first coded line -> not replaced
        if (choice >= 0 && firstCodedChoice(content, (int) options.size(), &options) == choice)
            mAnswerReplacedFalse = true;
        writeTransLog("ask", userMsg, content, choice, (int) options.size(),
                      valid ? options[choice - 1] : string(), fb, &options);
    }
    DebugTrace("AIPlayerGPT: " << decision << " -> chose " << choice << " of " << options.size());

    return (callerChoice >= 1) ? callerChoice - 1 : -1; //0 or parse-fail: defer to caller
}


//Hybrid-pip affordability clarifier for a cast option line. Cross-seat the
//model misreads hybrid mana pips and declines an OFFERED, payable cast - it
//reads {u/b} as needing BOTH colors, or a colored pip as generic (deck109
//{B/R}, deck17 {u/b} under-deployments, deck135 hallucinated {1}{g} decline,
//deck21 {R/G}). The option list is already authoritative; the sanctioned fix
//is REPRESENTATION - spell out, ONLY on costs that actually carry hybrid pips,
//that each pip pays with EITHER of its colors and how small the real total is.
//Non-hybrid costs get nothing (annotating every cost is noise, per the wave-15
//synthesis ruling).
//N-166n: the emitted tail of the hybrid gloss, split out as a PURE function of
//(the pip descriptions, the cost's converted total) so the "total N mana" claim
//has a provable shape. The total is the WHOLE cost's converted value - generic
//component included - which is what made the observed
//"{w}{w}{b/r} ... total 3 mana" beside a decklist {2}{BR}{W}{W} a contradiction
//worth chasing; see altCostPrintedNote for the divergence half of the fix.
static string hybridPipNoteText(const vector<string>& pips, int totalMana)
{
    if (pips.empty())
        return "";
    std::ostringstream note;
    note << " [hybrid: ";
    if (pips.size() == 1)
        note << "each " << pips[0];
    else
        for (size_t k = 0; k < pips.size(); k++)
            note << (k ? "; " : "") << pips[k];
    note << " - total " << totalMana << " mana]";
    return note.str();
}


static string hybridPipNote(ManaCost * c)
{
    if (!c)
        return "";
    vector<string> pips; //distinct color/color pips, first-seen order
    for (unsigned int i = 0; ; i++)
    {
        ManaCostHybrid * h = c->getHybridCost(i);
        if (!h)
            break;
        if (!h->color1 || !h->color2)
            continue; //only true color/color pips (what renders as {x/y})
        char lo1 = Constants::MTGColorChars[h->color1];
        char lo2 = Constants::MTGColorChars[h->color2];
        char hi1 = (char) toupper((unsigned char) lo1);
        char hi2 = (char) toupper((unsigned char) lo2);
        string desc = string("{") + lo1 + "/" + lo2 + "} pays with " + hi1 + " or " + hi2;
        bool seen = false;
        for (size_t k = 0; k < pips.size(); k++)
            if (pips[k] == desc) { seen = true; break; }
        if (!seen)
            pips.push_back(desc);
    }
    return hybridPipNoteText(pips, c->getConvertedCost());
}


//N-166n (wave-34 audit b1 F-07, the batch's highest-severity render bug and one
//of its two budget-hit traces): the mutate option printed
//`mutate cost {w}{w}{b/r} ... [hybrid: ... - total 3 mana]` for Snapdax, whose
//printed mutate cost is {2}{BR}{W}{W} - five mana. The model litigated it
//verbatim ("This is contradictory") and was rescued only by having 5 mana
//anyway. This is the class the core prompt's own rail ("trust the numbers the
//game prints") makes most dangerous: a WRONG NUMBER stated as engine ground
//truth. The live alternative-cost object is what the engine will actually
//charge - cost reduction mutates it in place - so the label must keep printing
//the live cost, and the fix is to stop the SILENT divergence from the decklist:
//when live != printed, say both and say which one is binding. Pure over the two
//strings so the branches are provable.
static string altCostPrintedNote(const string& liveCost, const string& printedCost)
{
    if (printedCost.empty() || liveCost == printedCost)
        return "";
    return " (the decklist prints this cost as " + printedCost + "; the cost shown"
           " here is the LIVE one after any cost changes, and it is what you actually pay)";
}


//The card's PRINTED alternative cost as a string ("" when unavailable).
static string printedAlternativeCostString(MTGCardInstance * card)
{
    if (!card || !card->model || !card->model->data)
        return "";
    ManaCost * printed = card->model->data->getManaCost();
    if (!printed || !printed->getAlternative())
        return "";
    return printed->getAlternative()->toString();
}


//Name the card(s) an EXTRA cost on an alternative cast will consume from
//hand. Force of Negation's free "exile a blue card" pitch auto-selected the
//caster's only blue card (its finisher, Abominable Treefolk) with nothing on
//the option line naming it, so the model committed to the counter believing
//it was pitching a different card (deck135 wave-18 #1d). Surface the actual
//pitch target; warn when it is the sole eligible card, and flag the >1 case
//(the engine currently auto-picks synchronously before the model can choose
//- documented as the deeper routing gap). Enumerates from the cost's own
//TargetChooser over the caster's hand (the pitch case; other zones no-op).
static string pitchCostNote(MTGCardInstance * card, ManaCost * altCost)
{
    if (!card || !altCost || !altCost->extraCosts)
        return "";
    Player * p = card->controller();
    if (!p || !p->game || !p->game->hand)
        return "";
    std::ostringstream note;
    ExtraCosts * aec = altCost->extraCosts;
    for (size_t i = 0; i < aec->costs.size(); i++)
    {
        ExtraCost * ec = aec->costs[i];
        if (!ec || !ec->tc)
            continue;
        ec->setSource(card);
        vector<MTGCardInstance *> cands;
        MTGGameZone * hand = p->game->hand;
        for (int z = 0; z < hand->nb_cards; z++)
        {
            MTGCardInstance * c = hand->cards[z];
            if (c == card)
                continue; //the "other" targeter already excludes the source
            if (ec->tc->canTarget(c))
                cands.push_back(c);
        }
        if (cands.empty())
            continue;
        if (cands.size() == 1)
            note << " (this exiles your ONLY eligible card, " << cands[0]->getDisplayName()
                 << " - you will lose it)";
        else
        {
            note << " (auto-exiles ONE of: ";
            for (size_t k = 0; k < cands.size() && k < 6; k++)
                note << (k ? ", " : "") << cands[k]->getDisplayName();
            if (cands.size() > 6)
                note << ", ...";
            note << " - the engine picks for you here)";
        }
    }
    return note.str();
}


//N-139d (wave-29 deck139/deck122/deck93): a mutate alt-cast's cost label was
//inconsistent - Gemrazer rendered "with its mutate cost" (alternativeName ==
//"Mutate"), but Migratory Greathorn (identical `other={..} name(Mutate)`) rendered
//"with its alternative cost" whenever the alternative object being priced had lost
//its name (e.g. a cost-reduced copy - Pollywog on board). The bare "alternative
//cost" label kept the model from recognizing the mutate line up front. Unify: any
//alt-cast of a card with the mutate keyword is a "mutate cost" (canonical
//lowercase, so Gemrazer and Migratory read identically); a NAMED non-mutate
//alternative keeps its own name; an unnamed non-mutate stays "alternative cost".
static string mutateAltCostLabel(bool hasMutate, const string& alternativeName)
{
    if (hasMutate && (alternativeName.empty() || alternativeName == "Mutate"))
        return "mutate cost";
    if (!alternativeName.empty())
        return alternativeName + " cost";
    return "alternative cost";
}


//#W61-T (C7, wave-60 deck123 H2): the land-drop row never said a land enters
//tapped. `123v152` seq 18 offered `1. Play Arcane Sanctum` under a parenthetical
//that reads as a promise the land is available now ("playing a land ... does not
//reduce what you can cast this turn" - true, and beside the point), the seat
//wrote "Cast Thraben Doomsayer turn 6" into its plan off that window, and turn 6
//opened on two sources with a three-mana creature in hand: four windows spent on
//a card that had no row. The engine has known the answer since the primitive was
//parsed - `auto=tap(noevent)` - and the conditional shape carries its condition
//in the same script (`Isolated Chapel`: `aslongas(plains,swamp|myBattlefield)
//tap(noevent) <1 oneshot`, printed "enters tapped unless you control a Plains or
//Swamp"). Read the branch off the SCRIPT and quote the CARD for the condition;
//this code asserts neither. A land whose script has no tap(noevent) gets nothing,
//so every untapped land's row is byte-identical to wave 60.
//#W61-V (R3, wave-61 codex review finding 3): is this script line an
//ACTIVATED ability's cost head? A Wagic auto line is either `cost:effect`
//(activated) or a bare effect (the card's own ETB/static). The cost head is
//made only of cost tokens - `{...}` symbol groups and the bare T/Q/S/E
//letters - so a head carrying words, parentheses or `$` is not a cost and the
//colon belongs to the effect body instead (`life:-2`, `damage:2`). Pure.
static bool scriptLineIsActivated(const string& line, size_t beforePos)
{
    const size_t colon = line.find(':');
    if (colon == string::npos || colon >= beforePos)
        return false; //no cost separator ahead of the effect
    const string head = line.substr(0, colon);
    bool sawSymbol = false;
    for (size_t i = 0; i < head.size(); i++)
    {
        const char c = head[i];
        if (c == '{' || c == '}')
        {
            sawSymbol = true;
            continue;
        }
        if (isalnum((unsigned char) c) || c == ' ' || c == '/' || c == '(' || c == ')'
            || c == '|' || c == '[' || c == ']' || c == ',' || c == '-')
            continue;
        return false; //`$`, `!`, `~` and friends: not a cost head
    }
    return sawSymbol; //a cost head always carries at least one {...} symbol
}


//#W61-T (C7) / #W61-V (R3): the classifier reads the card's OWN enters-tapped
//line, not any tap(noevent) anywhere in the script. Bant Panorama's
//tap(noevent) is inside its `{1}{T}{S}:` FETCH ability and taps the fetched
//basic, not the Panorama (mtg.txt:8934-8936); Blood Crypt's is one branch of
//its pay-2-life choice (mtg.txt:12034), which is a conditional entry, not an
//unconditional one. Both rendered as "enters TAPPED - it makes no mana this
//turn" in wave 60, which is a false availability claim in the first case and a
//false certainty in the second. Three tests decide it, in this order: an
//activated cost head or a `@` trigger means the tap is not this card's ETB at
//all; a tap that lands on a TARGET or a moved card is not this card's own; and
//a gate in front of it (aslongas / if( / restriction / a `choice` branch) makes
//the entry conditional.
static bool landEntersTappedLine(const string& line, bool * conditionalOut)
{
    const size_t at = line.find("tap(noevent)");
    if (at == string::npos)
        return false;
    if (!line.empty() && line[0] == '@')
        return false; //a triggered ability, not the card's own entry
    if (scriptLineIsActivated(line, at))
        return false; //an activated ability's effect (Bant Panorama's fetch)
    const string before = line.substr(0, at);
    if (before.find("target(") != string::npos || before.find("moveto(") != string::npos)
        return false; //the tap lands on another card, not on this one
    if (conditionalOut)
        *conditionalOut = before.find("aslongas") != string::npos
                          || before.find("if(") != string::npos
                          || before.find("restriction") != string::npos
                          //#W61-V (R3): a shockland's tapped entry is ONE branch
                          //of a choice its controller makes (Blood Crypt), so it
                          //is exactly as conditional as Isolated Chapel's.
                          || before.find("choice") != string::npos
                          //#W62-W (D1): `if <expr> then tap(noevent)` - the SPACE
                          //form, with no paren after the `if`. Wave 61 tested
                          //`if(` only, so Deserted Beach, Overgrown Farmland,
                          //Lair of the Hydra, Hive of the Eye Tyrant and every
                          //reveal-gated Snarl read as UNCONDITIONALLY tapped:
                          //56 of the 131 false rows (engine-seat HIGH-1).
                          || (before.find("if ") != string::npos
                              && before.find(" then") != string::npos);
    return true;
}


static string landGateTrim(const string& s)
{
    const size_t a = s.find_first_not_of(" \t");
    if (a == string::npos)
        return "";
    const size_t b = s.find_last_not_of(" \t");
    return s.substr(a, b - a + 1);
}


//A comma-separated list of BARE subtype words ("plains,swamp"). Anything else -
//`other land`, `land[basic]`, `*[plains;swamp]`, `creature[legendary]` - is
//refused here and leaves the gate opaque, which renders the hedged form. The
//refusal is deliberate: a selector this code cannot count exactly must never
//produce a resolved verdict.
static bool landGateSubtypeList(const string& sel, vector<string> * out)
{
    vector<string> parts;
    size_t pos = 0;
    while (true)
    {
        const size_t c = sel.find(',', pos);
        const string one = landGateTrim(sel.substr(pos, c == string::npos
                                                        ? string::npos : c - pos));
        if (one.empty())
            return false;
        for (size_t i = 0; i < one.size(); i++)
            if (!isalpha((unsigned char) one[i]))
                return false;
        parts.push_back(one);
        if (c == string::npos)
            break;
        pos = c + 1;
    }
    if (parts.empty())
        return false;
    *out = parts;
    return true;
}


//`<selector>|<zone>` out of a parenthesised head.
static bool landGateSplitSelector(const string& inner, string * sel, string * zone)
{
    const size_t bar = inner.find('|');
    if (bar == string::npos)
        return false;
    *sel = landGateTrim(inner.substr(0, bar));
    *zone = landGateTrim(inner.substr(bar + 1));
    return true;
}


//The balanced body of `head(` ... `)`; `head` ends in its own '('.
static string landGateParenBody(const string& line, const string& head)
{
    const size_t p = line.find(head);
    if (p == string::npos || head.empty())
        return "";
    const size_t open = p + head.size() - 1;
    int depth = 0;
    for (size_t i = open; i < line.size(); i++)
    {
        if (line[i] == '(')
            depth++;
        else if (line[i] == ')')
        {
            depth--;
            if (!depth)
                return line.substr(open + 1, i - open - 1);
        }
    }
    return "";
}


//#W62-W (D1) idiom 1: `aslongas(<sel>|myBattlefield) tap(noevent) <1 oneshot`
//(Isolated Chapel, mtg.txt:59411) and `... >2 oneshot` (Seachrome Coast,
//mtg.txt:101153). The effect holds while the count of <sel> in my battlefield
//passes the trailing comparator, so THAT is exactly when the land enters tapped.
static void landGateFromAslongas(const string& line, size_t tapPos, LandTapGate * g)
{
    const string inner = landGateParenBody(line.substr(0, tapPos), "aslongas(");
    string sel, zone;
    if (inner.empty() || !landGateSplitSelector(inner, &sel, &zone)
        || zone.find("mybattlefield") == string::npos)
        return; //opaque
    const string tail = line.substr(tapPos + 12); //past "tap(noevent)"
    const size_t c = tail.find_first_of("<>");
    if (c == string::npos)
        return;
    const int cmp = (tail[c] == '<') ? -1 : 1;
    size_t d = c + 1;
    while (d < tail.size() && tail[d] == ' ')
        d++;
    if (d >= tail.size() || !isdigit((unsigned char) tail[d]))
        return;
    const int n = atoi(tail.c_str() + d);
    if (sel == "other land" || sel == "land")
    {
        //`land` counts the ENTERING land too; this row is rendered while it is
        //still in hand, so the battlefield count in front of us is short by one.
        g->kind = kLandGateLandCount;
        g->cmp = cmp;
        g->threshold = (sel == "land") ? n - 1 : n;
        return;
    }
    vector<string> subs;
    if (cmp < 0 && n == 1 && landGateSubtypeList(sel, &subs))
    {
        g->kind = kLandGateSubtype;
        g->subtypes = subs;
    }
}


//#W62-W (D1) idiom 2: a BARE `tap(noevent)` whose condition sits on the NEXT
//line as a conditional untap - `auto=tap(noevent)` /
//`auto=aslongas(plains,island|myBattlefield) untap` (Glacial Fortress
//mtg.txt:46617-8, Drowned Catacomb 32505-6, Sunpetal Grove 115716-7). 70 of the
//131 false rows. An aslongas with no comparator holds while the count is at
//least one, so the land enters UNTAPPED exactly when one of those types is out.
static bool landGateFromUntapLine(const string& line, LandTapGate * g)
{
    const size_t up = line.find("untap");
    if (up == string::npos)
        return false;
    if (!line.empty() && line[0] == '@')
        return false;
    if (scriptLineIsActivated(line, up))
        return false;
    const string inner = landGateParenBody(line.substr(0, up), "aslongas(");
    string sel, zone;
    if (inner.empty() || !landGateSplitSelector(inner, &sel, &zone)
        || zone.find("mybattlefield") == string::npos)
        return false;
    g->kind = kLandGateOpaque;
    vector<string> subs;
    if (landGateSubtypeList(sel, &subs))
    {
        g->kind = kLandGateSubtype;
        g->subtypes = subs;
    }
    return true;
}


//#W62-W (D1) idiom 3: `if <expr> then tap(noevent)` - Deserted Beach
//(borderline.txt:27515), Overgrown Farmland (81143), Lair of the Hydra (64291),
//Hive of the Eye Tyrant (53392), and the reveal-gated Snarls (myhand, which this
//code deliberately leaves opaque - the seat's hand is not the battlefield the
//row is resolved against).
static void landGateFromIfThen(const string& before, LandTapGate * g)
{
    const size_t thenPos = before.rfind(" then");
    if (thenPos == string::npos)
        return;
    const size_t ifPos = before.rfind("if ", thenPos);
    if (ifPos == string::npos || ifPos + 3 > thenPos)
        return;
    const string expr = landGateTrim(before.substr(ifPos + 3, thenPos - ifPos - 3));
    const size_t t1 = expr.find('~');
    if (t1 == string::npos)
        return;
    const size_t t2 = expr.find('~', t1 + 1);
    if (t2 == string::npos || t2 + 1 >= expr.size()
        || !isdigit((unsigned char) expr[t2 + 1]))
        return;
    const string op = expr.substr(t1 + 1, t2 - t1 - 1);
    const int n = atoi(expr.c_str() + t2 + 1);
    const string head = landGateTrim(expr.substr(0, t1));
    int cmp = 0;
    if (op == "lessthan")
        cmp = -1;
    else if (op == "morethan")
        cmp = 1;
    else if (op != "equalto")
        return;
    if (head.compare(0, 8, "compare(") == 0)
    {
        //`compare(type:land:myBattlefield)` counts EVERY land I control, the
        //entering one included, so its threshold is one higher than the count
        //this row can see.
        const string body = landGateParenBody(head, "compare(");
        if (!cmp || body.find("type:land:mybattlefield") == string::npos)
            return;
        g->kind = kLandGateLandCount;
        g->cmp = cmp;
        g->threshold = n - 1;
        return;
    }
    if (head.compare(0, 5, "type(") != 0)
        return;
    const string body = landGateParenBody(head, "type(");
    string sel, zone;
    if (body.empty() || !landGateSplitSelector(body, &sel, &zone)
        || zone.find("mybattlefield") == string::npos)
        return;
    if (sel == "other land" || sel == "land")
    {
        if (!cmp)
            return;
        g->kind = kLandGateLandCount;
        g->cmp = cmp;
        g->threshold = (sel == "land") ? n - 1 : n;
        return;
    }
    vector<string> subs;
    if (!cmp && n == 0 && landGateSubtypeList(sel, &subs))
    {
        g->kind = kLandGateSubtype;
        g->subtypes = subs;
    }
}


//#W62-W (D1): the whole-script scan. Pure. An unconditional own-ETB tap still
//wins over a conditional one, EXCEPT when the script also carries the
//conditional-untap line that is the checkland idiom's other half - that line IS
//the bare tap's condition, and reading the tap alone is what produced 70 false
//unconditional rows.
LandTapGate landTapGateScan(const string& script)
{
    LandTapGate out, cond, untapGate;
    bool anyUnconditional = false, haveCond = false, haveUntap = false;
    size_t pos = 0;
    while (pos <= script.size())
    {
        const size_t eol = script.find('\n', pos);
        const string line = script.substr(pos, eol == string::npos ? string::npos
                                                                   : eol - pos);
        pos = (eol == string::npos) ? script.size() + 1 : eol + 1;
        const string lineLc = scriptLower(line);
        bool lineConditional = false;
        if (!landEntersTappedLine(lineLc, &lineConditional))
        {
            if (!haveUntap)
            {
                LandTapGate u;
                if (landGateFromUntapLine(lineLc, &u))
                {
                    untapGate = u;
                    haveUntap = true;
                }
            }
            continue;
        }
        out.found = true;
        if (!lineConditional)
        {
            anyUnconditional = true;
            continue;
        }
        if (haveCond)
            continue;
        const size_t at = lineLc.find("tap(noevent)");
        const string before = lineLc.substr(0, at);
        LandTapGate g;
        if (before.find("aslongas") != string::npos)
            landGateFromAslongas(lineLc, at, &g);
        else
            landGateFromIfThen(before, &g);
        cond = g;
        haveCond = true;
    }
    if (!out.found)
        return out;
    const LandTapGate& pick = (anyUnconditional && haveUntap) ? untapGate : cond;
    if (anyUnconditional && !haveUntap)
        return out; //genuinely unconditional (Arcane Sanctum)
    out.conditional = true;
    out.kind = pick.kind;
    out.subtypes = pick.subtypes;
    out.cmp = pick.cmp;
    out.threshold = pick.threshold;
    return out;
}


static string landGateSubtypeLabel(const string& lc)
{
    string s = lc;
    if (!s.empty())
        s[0] = (char) toupper((unsigned char) s[0]);
    return s;
}


//#W62-W (D1): resolve the gate against the pilot's OWN battlefield - the same
//battlefield the prompt prints a few lines above this row (deck123 MED-1: the
//condition is decidable and the seat declined the drop anyway, twice). Returns
//1 = enters untapped, 0 = enters tapped, -1 = not decidable here. `witness` is
//parallel to gate.subtypes and names a permanent carrying that type ("" = none).
//Pure.
//(history: comment-archaeology.md AIPlayerGPTSeams-L3129-1989)
static const char * kLandGateGrantedNote =
    " - a type it has on the battlefield now, not one printed on the card";

static int landTapResolve(const LandTapGate& g, int myLands,
                          const vector<string>& witness, string * evidence,
                          const vector<char> * witnessPrinted = NULL)
{
    if (!g.found || !g.conditional)
        return -1;
    if (g.kind == kLandGateSubtype)
    {
        if (g.subtypes.empty() || witness.size() != g.subtypes.size())
            return -1;
        //A witness whose own card prints the subtype first: it is the one the
        //pilot can check. Only when none exists does a granted one speak, and
        //then it names the reason it can be checked by.
        for (int pass = 0; pass < 2; pass++)
            for (size_t i = 0; i < witness.size(); i++)
            {
                if (witness[i].empty())
                    continue;
                const bool printed = witnessPrinted && i < witnessPrinted->size()
                                     ? (*witnessPrinted)[i] != 0 : true;
                if (pass == 0 && !printed)
                    continue;
                if (evidence)
                {
                    *evidence = "you control " + witness[i] + ", a "
                                + landGateSubtypeLabel(g.subtypes[i]);
                    if (!printed)
                        *evidence += kLandGateGrantedNote;
                }
                return 1;
            }
        if (evidence)
        {
            string s = "you control no ";
            for (size_t i = 0; i < g.subtypes.size(); i++)
                s += (i ? " and no " : "") + landGateSubtypeLabel(g.subtypes[i]);
            *evidence = s;
        }
        return 0;
    }
    if (g.kind == kLandGateLandCount)
    {
        if (myLands < 0)
            return -1;
        const bool tapped = (g.cmp < 0) ? (myLands < g.threshold)
                                        : (myLands > g.threshold);
        std::ostringstream o;
        o << "you control " << myLands << " other land" << (myLands == 1 ? "" : "s");
        if (evidence)
            *evidence = o.str();
        return tapped ? 0 : 1;
    }
    return -1;
}


//#W62-W (D1): the rendered bracket. A RESOLVED gate states the truth and names
//the evidence; an unresolved conditional keeps the wave-61 hedge; an
//unconditional tap keeps the wave-61 verdict byte for byte. Nothing is deleted:
//the card's own sentence is still quoted in every branch that has one, so the
//verdict and the sentence can be checked against each other on the row.
string landEntersTappedTagFrom(const LandTapGate& g, const string& printedText,
                               int resolution, const string& evidence)
{
    if (!g.found)
        return "";
    string sentence = sentenceNaming(printedText, "enters tapped", 200);
    if (sentence.empty())
        sentence = sentenceNaming(printedText, "enters the battlefield tapped", 200);
    const bool resolved = g.conditional && (resolution == 0 || resolution == 1);
    std::ostringstream o;
    o << " [";
    //#W64-AJ (deck125 MED-1): 44 of 44 declined land drops at that seat were
    //`[enters TAPPED]` rows. The tag priced THIS turn and stopped there, so the
    //only fact on the row was a cost; what the land is worth from the next turn
    //on - the reason a land drop is taken at all - was never stated, and a land
    //drop not taken this turn is not recoverable later. The clause states the
    //fact, not an instruction: it is true of every land that enters tapped and
    //it claims nothing about whether taking the row is right here.
    bool tappedNow = true;
    if (resolved && resolution == 1)
    {
        o << "enters UNTAPPED - it makes mana this turn";
        tappedNow = false;
    }
    else if (resolved)
        o << "enters TAPPED - it makes no mana this turn";
    else if (g.conditional)
        o << "enters tapped UNLESS its own condition holds, so it may make no mana"
             " this turn";
    else
        o << "enters TAPPED - it makes no mana this turn";
    if (resolved && !evidence.empty())
        o << " (" << evidence << ")";
    if (!sentence.empty())
        o << ": \"" << sentence << "\"";
    //#W64-AJ: last, so every verdict, its evidence and the card's own sentence
    //are byte-identical to wave 63 and only the missing half is added.
    //#W82-A (L9, audit-2026-09): "it taps for mana from your next turn on" made a
    //STATE into a turn-long prohibition. Tapped is a state (CR 701.26a: "To tap a
    //permanent, turn it sideways from an upright position. Only untapped
    //permanents can be tapped"), and anything that untaps it makes it a mana
    //source at once. The MDFC arrival twin has hedged this correctly since #W67-AW
    //("unless something untaps it"); this is the same hedge, on the ordinary land
    //row - 67 prompts carried the absolute wording.
    if (tappedNow)
        o << " - it makes mana as soon as it is untapped, which is your next untap"
             " step unless something untaps it sooner";
    o << "]";
    return o.str();
}


string landEntersTappedTag(const string& script, const string& printedText)
{
    return landEntersTappedTagFrom(landTapGateScan(script), printedText, -1, string());
}


//#W67-AW (M3): defined here because it consults landTapGateScan; declared beside
//mdfcLandArrivalTag, where the rationale is.
string mdfcLandArrivalTagFrom(const string& script, const string& printedText)
{
    int life = 0;
    const int cls = mdfcLandArrivalClass(script, life);
    if (cls != kMdfcArrivalPlain)
        return mdfcLandArrivalTag(cls, life);
    if (script.empty())
        return "";
    const LandTapGate g = landTapGateScan(script);
    if (!g.found)
        return " {it enters UNTAPPED - it makes mana this turn}";
    //A conditional gate this class did not catch: say exactly what the ordinary
    //land row says about it, in the brace channel this row uses.
    string t = landEntersTappedTag(script, printedText);
    if (t.size() < 4 || t[1] != '[')
        return "";
    return " {" + t.substr(2, t.size() - 3) + "}";
}


//#W62-W (D1): scan, resolve and render in one call - the exact pipeline the
//land-drop row runs, so the corpus proves what the row prints. Non-static: it
//is the PARSETEST entry point for the resolved shapes.
string landTapTagFor(const string& script, const string& printedText, int myLands,
                     const vector<string>& witness,
                     const vector<char> * witnessPrinted = NULL)
{
    const LandTapGate g = landTapGateScan(script);
    string ev;
    const int res = landTapResolve(g, myLands, witness, &ev, witnessPrinted);
    return landEntersTappedTagFrom(g, printedText, res, ev);
}


//#W62-W (D1): the impure half - the gate is asked of the pilot's own
//battlefield, using the engine's own subtype ids (findType with forceAdd FALSE,
//so a selector this code cannot recognise can never grow the global type list).
static string landEntersTappedTagResolved(MTGCardInstance * land, Player * me)
{
    if (!land)
        return "";
    const LandTapGate g = landTapGateScan(land->magicText);
    if (!g.found)
        return "";
    int myLands = -1;
    vector<string> witness;
    vector<char> witnessPrinted;
    if (me && me->game && me->game->inPlay)
    {
        myLands = 0;
        witness.assign(g.subtypes.size(), string());
        witnessPrinted.assign(g.subtypes.size(), (char) 0);
        vector<int> ids(g.subtypes.size(), 0);
        for (size_t s = 0; s < g.subtypes.size(); s++)
            ids[s] = MTGAllCards::findType(g.subtypes[s], false);
        for (int i = 0; i < me->game->inPlay->nb_cards; i++)
        {
            MTGCardInstance * c = me->game->inPlay->cards[i];
            if (!c || !c->isLand())
                continue;
            myLands++;
            for (size_t s = 0; s < ids.size(); s++)
            {
                if (!ids[s] || !c->hasType(ids[s]))
                    continue;
                //#W63-AE (E8): PRINTED means the card's own primitive carries
                //the subtype; the instance can carry it from a live grant
                //(Urborg's `lord(land) transforms((swamp))`). A printed witness
                //replaces a granted one, never the other way round.
                const bool printed = c->model && c->model->data
                                     && c->model->data->hasType(ids[s]);
                if (witness[s].empty() || (printed && !witnessPrinted[s]))
                {
                    witness[s] = c->getDisplayName();
                    witnessPrinted[s] = printed ? (char) 1 : (char) 0;
                }
            }
        }
    }
    string evidence;
    const int res = landTapResolve(g, myLands, witness, &evidence, &witnessPrinted);
    return landEntersTappedTagFrom(g, land->text, res, evidence);
}


//#W82-P9: the revert switch. Compile-time default ON; WAGIC_GPT_LAND_SEPARATE=1
//at runtime restores the separate land-drop ask (the pre-P9 behaviour).
static const bool kLandDropInCastMenu = true;

static bool landDropInCastMenu()
{
    static int v = -1;
    if (v < 0)
    {
        const char * e = getenv("WAGIC_GPT_LAND_SEPARATE");
        v = (e && *e && *e != '0') ? 0 : (kLandDropInCastMenu ? 1 : 0);
    }
    return v == 1;
}


//The land-drop rows, one per playable land (two for an MDFC: front face and
//back face), the same text on the separate land ask and on the combined
//casting menu (#W82-P9).
void AIPlayerGPT::buildLandDropRows(const vector<LegalActionsOracle::Cast>& lands,
                                    vector<string>& opts, vector<size_t>& rowLand,
                                    vector<bool>& rowBackFace)
{
    for (size_t li = 0; li < lands.size(); li++)
    {
        string backMana;
        const string backFace = w74MdfcLandBackFace(this, lands[li].card, backMana);
        const string base = "Play " + lands[li].card->getDisplayName() + lands[li].zoneLabel
                       //#W61-T (C7): the tapped-land fact, on the row that plays it.
                       //#W62-W (D1): resolved against the battlefield this
                       //same prompt prints, where the gate is decidable.
                       + landEntersTappedTagResolved(lands[li].card, this)
                       //#W66-AT (deck146 MED): and the half the mana clause
                       //never priced - this land is also a creature.
                       + landDropThreatTag(lands[li].card->magicText);
        if (backFace.empty())
        {
            opts.push_back(base);
            rowLand.push_back(li);
            rowBackFace.push_back(false);
            continue;
        }
        opts.push_back(base + w75MdfcFaceRowTag(false, lands[li].card->getDisplayName(),
                                                backFace, backMana));
        rowLand.push_back(li);
        rowBackFace.push_back(false);
        opts.push_back("Play " + backFace + lands[li].zoneLabel
                       + w75MdfcFaceRowTag(true, lands[li].card->getDisplayName(),
                                           backFace, backMana));
        rowLand.push_back(li);
        rowBackFace.push_back(true);
    }
}


//#W82-P9: a land row on the combined menu was taken - the same commit the
//separate ask makes (MDFC face pre-answer, forced-candidate validation).
MTGCardInstance * AIPlayerGPT::commitLandDropPick(ManaCost * pMana, MTGCardInstance * chosenLand,
                                                  bool backFace)
{
    {
        string preBackMana;
        const string preBack = w74MdfcLandBackFace(this, chosenLand, preBackMana);
        if (!preBack.empty())
        {
            mLandFacePreCard = chosenLand;
            mLandFacePreTurn = observer->turn;
            mLandFacePreBack = backFace;
            mLandFacePreBackName = preBack;
        }
    }
    aiForcedCandidate = chosenLand;
    MTGCardInstance * validated = AIPlayerBaka::FindCardToPlay(pMana, "land");
    aiForcedCandidate = NULL;
    if (validated)
        return validated;
    DebugTrace("AIPlayerGPT: model chose land " << chosenLand->name
               << " but it fails validation; deferring to heuristic");
    writeTransLog("defer", "", "", -1, 0, chosenLand->name, "deferred_to_heuristic");
    gotPayments.clear();
    return AIPlayerBaka::FindCardToPlay(pMana, "land");
}


MTGCardInstance * AIPlayerGPT::FindCardToPlay(ManaCost * pMana, const char * type)
{
    //No endpoint, or a scripted combo is mid-execution: heuristic as-is.
    if (mEndpoint.empty() || comboCards.size())
        return AIPlayerBaka::FindCardToPlay(pMana, type);

    //#W52-G (D-1): a repeat-N plan is executing (chooseOrderedAction dispatches
    //one iteration per tick). The plan IS this priority sequence's decision -
    //the row's contract reads "then stop" and priority returns to the model
    //when it ends - so this seam must not open a cast or land ask underneath
    //it. Every such ask was launched against a board the very next iteration
    //moved past, so each one came back stale and was dropped (deck123 vs
    //deck126 turn 14: six launches, six drops, zero consumed answers, and the
    //streak breaker gave the decision to Baka at 7/17). Not asking is the fix:
    //no wasted round trips, no drops, and the model gets its cast decision at
    //the window the plan promised, with the finished board in front of it.
    if (mRepeatRemaining > 0)
    {
        DebugTrace("AIPlayerGPT: " << (strcmp(type, "land") ? "cast" : "land-drop")
                   << " ask NOT issued - repeat plan in progress (" << mRepeatDone << "/"
                   << mRepeatTotal << " done); the plan is this priority sequence's decision");
        return NULL;
    }

    //Lands: enumerate every DISTINCT playable land as its own option. The
    //old shape (heuristic proposes ONE land, model keeps a veto) never
    //co-offered two land types, so guide rules like "play Mountain before
    //Island" were unexecutable whenever the heuristic's pick came first in
    //hand order (wave-7 deck131 finding: 0 of ~150 land decisions showed
    //two Play options). legalLandPlays already dedups by display name, so
    //four Mountains render as one "Play Mountain" line.
    if (!strcmp(type, "land"))
    {
        vector<LegalActionsOracle::Cast> lands = LegalActionsOracle::legalLandPlays(this);
        //#W82-A (L1, audit-2026-09): the land-face decline latch filter is
        //DELETED. A card whose own FACE menu this seat declined earlier in the
        //turn was removed from the LAND ask for the rest of the turn - a legal
        //land play (CR 305.1) withheld on the strength of an answer to a
        //different question - and when it was the only land in hand no land ask
        //was issued at all, under a `Land drop: NOT yet used this turn - you can
        //still play a land` line that the latch never consulted (the
        //true-statement-in-the-wrong-scope shape, L9). The wave-70 hang this
        //guarded is the ask cache's job: the key carries the board and the
        //question, so an unchanged board replays rather than re-asking.
        //#W43-12. An ask with no options must never reach the model seam, and
        //the skip must not be silent: a land in hand that the oracle declines
        //to offer (its own cast restrictions forbid the play - the flipped
        //modal-DFC case) reads to a reviewer exactly like "no land in hand"
        //unless the engine says which it was. One line per window is cheap and
        //it is the difference between "not offered" and "never had one".
        if (lands.empty())
        {
            int landsInHand = 0;
            for (int hz = 0; hz < game->hand->nb_cards; hz++)
                if (game->hand->cards[hz]->isLand())
                    landsInHand++;
            if (landsInHand)
                DebugTrace("AIPlayerGPT: land-drop ask NOT issued - " << landsInHand
                           << " land(s) in hand, none playable right now (restriction or"
                           " play-restriction); the pilot is not asked a question with no answers");
            return NULL;
        }

        //Play options FIRST (the model favors option 1, and playing a land
        //is nearly always right - decline goes LAST, house ordering rule).
        //#W75-CI (P18): EACH FACE IS ITS OWN ROW. #W74-CD (O4) told the model
        //that the face would be picked at the NEXT window, and the next window
        //then asked it - one land drop, two model calls, and the second menu's
        //own decline row could un-make the first answer. The card is the same
        //either way (the back face arrives through its own `{0}` flip ability,
        //not through a second land play), so the two rows carry the same land
        //entry and only the FACE differs; the answer is carried into the face
        //menu below instead of being asked twice.
        //#W82-P9 (owner: "try it. Revert if it degrades behavior"): on the seat's
        //own main phase the land rows ride the CASTING menu instead of a menu
        //of their own. The heuristic asks for the land first and the casts
        //second in the same tick, so this branch delegates to the cast branch
        //with the land rows staged; if that branch ASKED (a menu reached the
        //model, or is in flight), its answer is this branch's answer - a land
        //the model named comes back as the card to play, a spell as the card
        //to cast, and the cast question re-puts on the changed board after a
        //land enters. If the cast branch did NOT ask (no castable spell, a hold
        //standing, a no-progress marker), the separate land ask below runs as
        //before, so the land drop is never lost. WAGIC_GPT_LAND_SEPARATE=1
        //restores the separate ask unconditionally (the revert switch).
        if (landDropInCastMenu() && observer && observer->currentPlayer == this
            && mLandRowsForCast.empty() && mRepeatRemaining <= 0)
        {
            mLandRowsForCast = lands;
            mCastMenuAsked = false;
            MTGCardInstance * viaCast = FindCardToPlay(pMana,
                "planeswalker,creature,enchantment,artifact,sorcery,instant,battle");
            mLandRowsForCast.clear();
            if (mCastMenuAsked)
                return viaCast;
            //fall through: the cast branch never put a menu to the model
        }

        vector<string> opts;
        vector<size_t> rowLand;   //which lands[] entry this row plays
        vector<bool> rowBackFace; //...and which face of it
        buildLandDropRows(lands, opts, rowLand, rowBackFace);
        opts.push_back(kLandDropDeclineRow);
        //#W77-CR (R2 a): the land drop's own hold-check memory. It is a `kind:
        //ask` record like the casting menu, so wave 76 filed it in the casting
        //menu's history; it is a different question with a different row
        //vocabulary, and a bracket that compares the two is false by
        //construction. Measured at its own seam, printed on the same prompt-only
        //channel as every other bracket (askModel splices and consumes it).
        mNextAskPromptNote += holdReopenNote("land", opts);

        std::ostringstream q;
        q << landDropAskText(lands.size());
        //#W47-R13 (wave-46 docket R13): the ask already explains that the land
        //drop is its OWN decision (N-166m, above) - which answers a question the
        //pilot was not getting wrong. What it got wrong is the RESOURCE: all
        //nine declines this corpus reason the drop against a spell ("Cast
        //Intrepid Adversary. Next turn play land"), as if the two competed.
        //State the consequence instead of the mechanism.
        q << kLandDropConsequence;
        //#W47-R13, latency half: the corpus's two slowest two-option asks were
        //both this menu (453 s and 367 s, the second with answer_replaced), both
        //spent on a full-turn strategic monologue in the PLAN field over a
        //binary with one right answer. A land drop cannot falsify a plan, so the
        //request for one is pure cost here. Nothing is removed from the reply
        //rules: a volunteered PLAN is still parsed and carried.
        //#W80-DE (U10, wave-79 engine-seat MED-2 - THE SEAM DC F10 MISSED). All
        //SEVEN of the corpus's `[own loop verdict: none]` record writes with no
        //rendered line are two-row LAND DROP menus. This seam calls askModel
        //directly and never touched the own-loop face, so a face stamped by a cast
        //window still in flight was consumed by the NEXT record written - which is
        //this one. The land prompt carries no verdict line, so it stamps no face
        //and clears whatever was staged, exactly as every other non-verdict send.
        {
            bool w80c = false;
            w79ApplyLoopFaceAtSend(false, string(), mOwnLoopVerdictFace, w80c);
            w80ApplyVerdictFacesAtSend(false, string(), string(), false);
        }
        int pick = askModel(q.str(), opts, false, string(), false, true); //the play narrates itself as a zone event
        if (pick == kChoicePending)
        {
            gotPayments.clear(); //nothing plays this tick; re-poll next tick
            return NULL;
        }
        if (pick == (int) rowLand.size()) //#W75-CI (P18): the decline is still LAST
        {
            DebugTrace("AIPlayerGPT: held the land drop");
            gotPayments.clear();
            return NULL;
        }
        if (pick < 0) //model deferred or endpoint failed: heuristic decides
        {
            //#W67-AX (I7): the land seam reports its heuristic answer too.
            MTGCardInstance * heur = AIPlayerBaka::FindCardToPlay(pMana, type);
            noteHeuristicExecuted("land", heur ? 1 : 0,
                                  heur ? ("play " + heur->name) : string("no land drop"));
            return heur;
        }

        //Validate the pick with the heuristic's own machinery (residual
        //gates, dice, payment state) - same pattern as the cast seam below.
        //#W75-CI (P18): the row's own land entry, and the face it named.
        const size_t chosenRow = (size_t) pick;
        MTGCardInstance * chosenLand = lands[rowLand[chosenRow]].card;
        {
            string preBackMana;
            const string preBack = w74MdfcLandBackFace(this, chosenLand, preBackMana);
            if (!preBack.empty())
            {
                mLandFacePreCard = chosenLand;
                mLandFacePreTurn = observer->turn;
                mLandFacePreBack = rowBackFace[chosenRow];
                mLandFacePreBackName = preBack;
                DebugTrace("AIPlayerGPT: the land-drop answer named the "
                           << (mLandFacePreBack ? "BACK" : "FRONT") << " face of "
                           << chosenLand->getDisplayName()
                           << " - the face menu is answered from it, not re-asked");
            }
        }
        aiForcedCandidate = chosenLand;
        MTGCardInstance * validated = AIPlayerBaka::FindCardToPlay(pMana, type);
        aiForcedCandidate = NULL;
        if (validated)
            return validated;
        DebugTrace("AIPlayerGPT: model chose land " << chosenLand->name
                   << " but it fails validation; deferring to heuristic");
        writeTransLog("defer", "", "", -1, 0, chosenLand->name, "deferred_to_heuristic");
        gotPayments.clear();
        return AIPlayerBaka::FindCardToPlay(pMana, type);
    }

    //Spells: one free choice across every castable card, whatever type rung
    //of the heuristic's cast ladder we were called for. computeActions stops
    //at the first rung that returns a card, so answering the whole question
    //here means a single model call per casting window.
    //type=="" is the instant/interrupt window: computeActions will discard
    //anything that is not instant-speed, so only offer what can actually go.
    bool instantWindow = !*type;

    vector<MTGCardInstance *> candidates;
    vector<bool> candidateUsesAlt; //cast this entry with its alternative cost
    vector<int> rowUses; //#W54-C (D18): sources this row spends, -1 = unpriceable
    vector<int> rowSweepTheirs, rowSweepMine; //#W61-U (C10): the board this row clears
    vector<string> rowSweepEngines; //#W66-AQ (H10): which of THEIRS are engines
    vector<string> rowSweepMineNames, rowSweepMineEngines; //#W80-DF (U7)
    vector<string> rowNames; //#W56-B (D15): the card each row casts
    vector<int> rowCosts; //#W56-B (D15): the converted cost it would pay, -1 = unknown ({X})
    vector<string> opts; //"Cast nothing" is appended LAST (positional
                         //anchoring: the model favors option 1, and
                         //nothing-first likely drove the pass rate)

    //The legal cast set comes from the contract (oracle-backed: zone gates,
    //legendary rule, play restrictions, affordability, 601.2c target
    //validity); this seam only renders it for the model. The APPLY side of
    //a cast still rides Baka's pricing machinery below (aiForcedCandidate).
    //#W82-A (audit-2026-09): the comment said "until c5 moves payment+clickstream
    //into the manager" as though that were pending - `DecisionManager::planCastSpell`
    //exists (c5a) and this seam still does not use it. The sentence states the
    //present fact instead of a promise.
    GptManaPolicy policy(this);
    DecisionRequest castReq;
    if (!DecisionManager::buildCastSpell(this, policy, pMana, instantWindow, castReq))
    {
        //#W75-CI (P8, wave-74 deck130 HIGH): SAY SO. This is the ONLY silent
        //exit of the cast seam, and it is where a "main phase 1 did not re-ask
        //after my spell was countered" report has to be adjudicated. All 13 of
        //the corpus's own-main-phase post-counter windows that moved on are
        //this exit and all 13 are correct (the mana went with the countered
        //spell: `130v125` seq 61 pays 5 of 10 for Siege-Gang and the only other
        //row, Rorix, needs 6) - but nothing said so, which is the silent-no-op
        //shape the arrival-tracing rule exists for. One line per window; it
        //mirrors the land arm's own "ask NOT issued" trace.
        int handCards = game->hand ? game->hand->nb_cards : 0;
        DebugTrace("AIPlayerGPT: cast ask NOT issued - the oracle finds no legal cast in this"
                   << (instantWindow ? " instant-speed" : " sorcery-speed") << " window ("
                   << handCards << " card(s) in hand, "
                   << ManaEngine::potentialColorReach(this, policy, NULL)
                   << " untapped source(s)); the pilot is not asked a question with no answers");
        return NULL; //nothing castable: only one outcome, no model call
    }
    const vector<LegalActionsOracle::Cast> & casts = castReq.casts;

    //#W82-A (L1, audit-2026-09): `mStuckCastLines` - a legal cast row dropped
    //from the menu FOR THE TURN after one no-op - is DELETED. Its trigger could
    //be the model's own answer (a cast-mode sub-menu declined before any
    //payment), its identity was a hash of the fully annotated row so an
    //unrelated `{leaves N...}` change un-stuck it by accident, and it carried no
    //receipt on any prompt: the model was never told a row it had been shown had
    //gone. The livelock it guarded is now the ask key's job (the key carries the
    //board, so an unchanged board replays a cached answer rather than paying for
    //a new call) and the validation re-ask budget's.
    //#W47 (R14b) / #W82-A (L6): the untapped-source TOTAL this window. The
    //comment that stood here claimed the row's remainder and the header's total
    //"can never disagree" while the two ran DIFFERENT policies and did disagree
    //on four corpus screens. They now run the same `GptManaPolicy`, which is what
    //makes the claim true rather than asserted.
    int untappedSources = ManaEngine::potentialColorReach(this, policy, NULL);
    string boardNow = serializeGameState();
    if (!mLastCastLine.empty())
    {
        //#W82-A (L1): the no-progress fact arms a WINDOW-level deferral, not a
        //row withhold. Every row stays offered; this one window is conceded.
        if (boardNow == mLastCastBoard)
        {
            DebugTrace("AIPlayerGPT: cast pick made no progress (every row stays offered;"
                       " the next window over this board defers): " << mLastCastLine);
            mCastNoProgressBoard = boardNow;
        }
        mLastCastLine.clear();
    }
    if (!mCastNoProgressBoard.empty())
    {
        if (mCastNoProgressBoard == boardNow)
        {
            mCastNoProgressBoard.clear(); //spent by this one deferral
            DebugTrace("AIPlayerGPT: cast window deferred to the heuristic once - the previous"
                       " pick over this exact board did not execute");
            return NULL;
        }
        mCastNoProgressBoard.clear(); //the board moved: nothing to break
    }
    for (size_t ci = 0; ci < casts.size(); ci++)
    {
        MTGCardInstance * card = casts[ci].card;
        ManaCost * cost = card->getManaCost();
        int rowUsed = -1; //#W54-C (D18)
        CastRowBoardAnswer rowSweep; //#W61-U (C10)
        std::ostringstream o;
        if (!casts[ci].viaAlternative)
        {
            o << "Cast " << card->getDisplayName();
            o << manaCostToken(card); //N-36b: {0} is a cost, not an absence
            if (card->isCreature())
                o << " (" << card->power << "/" << card->toughness << ")";
            o << casts[ci].zoneLabel;
            o << exileCastNote(this, card, casts[ci].zoneLabel); //#W52-K D11
            o << hybridPipNote(cost);
            o << dynamicMagnitudes(card, &rowSweep); //#W61-U (C10)
            //W43-7: {X} spells are unpriceable by dynamicMagnitudes (it skips
            //the amount "x" - X is announced AFTER this pick). Price them here.
            o << xSpellPricing(card, this, &rowSweep); //#W61-U (C10)
            //#W51-E D8 / #W52-K D10: what is already controlled - a legendary
            //twin (legend rule), a stackable second copy (both stay), or what a
            //tutor's search type can still find in the library.
            {
                bool legendary = card->hasType(Subtypes::TYPE_LEGENDARY);
                bool stackClass = !legendary && !card->isCreature() && !card->hasType(Subtypes::TYPE_LAND)
                    && !card->hasType(Subtypes::TYPE_INSTANT) && !card->hasType(Subtypes::TYPE_SORCERY);
                string want = tutorSearchType(card->magicText);
                MTGGameZone * bf = game ? game->inPlay : NULL;
                MTGCardInstance * twin = NULL;
                std::set<string> heldOfType; //battlefield + hand names of the tutor's type
                for (int bi = 0; bf && bi < bf->nb_cards; bi++)
                {
                    MTGCardInstance * bc = bf->cards[bi];
                    if (!bc)
                        continue;
                    //#W75-CM (F8): the LEGEND-RULE mark counts a TOKEN copy - the
                    //legend rule does not care about tokens (CR 704.5j), and this
                    //bracket must state the same board fact the crack-back cover
                    //nets out (w75LegendTwinControlled). The non-legendary
                    //"second copy" mark keeps its own token exclusion.
                    if ((legendary || stackClass) && !twin && bc->name == card->name
                        && (legendary || !bc->isToken))
                        twin = bc;
                    if (bc->isToken)
                        continue;
                    if (!want.empty() && bc->hasType(want))
                        heldOfType.insert(bc->getDisplayName());
                }
                if (twin)
                    o << (legendary ? legendTwinTag(twin->getDisplayName(),
                                                    w80TwinLoyalty(twin)) //#W80-DG (U12)
                                    : secondCopyTag(twin->getDisplayName(), card->magicText));
                if (!want.empty() && game && game->library)
                {
                    for (int hi = 0; game->hand && hi < game->hand->nb_cards; hi++)
                        if (game->hand->cards[hi] && game->hand->cards[hi] != card
                            && game->hand->cards[hi]->hasType(want))
                            heldOfType.insert(game->hand->cards[hi]->getDisplayName());
                    vector<string> fresh, copies;
                    std::set<string> seenLib;
                    for (int li = 0; li < game->library->nb_cards; li++)
                    {
                        MTGCardInstance * lc = game->library->cards[li];
                        if (!lc || !lc->hasType(want) || !seenLib.insert(lc->getDisplayName()).second)
                            continue;
                        (heldOfType.count(lc->getDisplayName()) ? copies : fresh).push_back(lc->getDisplayName());
                    }
                    o << tutorFindsTag(want, fresh, copies);
                }
            }
        }
        else
        {
            o << "Cast " << card->getDisplayName() << " with its ";
            o << mutateAltCostLabel(card->has(Constants::MUTATE),
                                    cost->getAlternative()->alternativeName);
            o << " " << cost->getAlternative()->toString();
            //N-166n: never let the label's cost silently disagree with the
            //decklist's printed cost.
            o << altCostPrintedNote(cost->getAlternative()->toString(),
                                    printedAlternativeCostString(card));
            //Adventure alternative casts put an INSTANT/SORCERY spell onto the
            //stack, not the creature - the card's power/toughness belongs to the
            //creature face you may cast LATER from exile, so printing "(5/5)"
            //here read as "pay the adventure cost, get a 5/5 body now" and drove
            //a confirmed misfire (deck131 s15: cast Usher to Safety expecting a
            //creature, forced to bounce its own permanent). Suppress the P/T on
            //this face and append the ADVENTURE SPELL's effect text instead
            //(the creature-face option below still carries the P/T). Generalized
            //by the adventure mechanism, not by card name.
            bool isAdventureCast = card->has(Constants::ADVENTURE);
            if (card->isCreature() && !isAdventureCast)
                o << " (" << card->power << "/" << card->toughness << ")";
            o << casts[ci].zoneLabel;
            o << exileCastNote(this, card, casts[ci].zoneLabel); //#W52-K D11
            o << hybridPipNote(cost->getAlternative());
            //Name the card an exile/pitch extra cost will consume, so a free
            //alt-cast cannot silently eat the deck's finisher unseen (#1d).
            o << pitchCostNote(card, cost->getAlternative());
            if (isAdventureCast)
            {
                //The engine text= for an adventure is "<creature face> //
                //<adventure spell>" (or, when the creature is vanilla, just the
                //adventure spell with no separator - e.g. Shepherd of the Flock).
                //The adventure face is always the part after " // "; with no
                //separator the whole text IS the adventure effect.
                string advText = card->text;
                size_t sep = advText.find(" // ");
                if (sep != string::npos)
                    advText = advText.substr(sep + 4);
                for (size_t ti = 0; ti < advText.size(); ti++)
                    if (advText[ti] == '\n')
                        advText[ti] = ' ';
                if (advText.size() > 200)
                {
                    size_t cut = advText.rfind(' ', 200);
                    advText = advText.substr(0, (cut == string::npos || cut < 100) ? 200 : cut) + "...";
                }
                if (!advText.empty())
                    o << " {adventure spell: " << advText << "}";
            }
        }
        //#W56-B (D14): the equipment cast row, priced by the board it would
        //enter. Counted off this seat's battlefield with the same creature
        //predicate the board header uses, so the row cannot contradict the
        //"of which N are creatures" line the pilot reads above it.
        //#W60-L (B1): the life this row's own cast will cost the pilot BEFORE
        //the spell resolves - the naming permanent's surcharge and the pain the
        //payment plan will take. Accumulated here and handed to the win fold
        //below so the row cannot price its own death and then promise a win.
        int rowSelfLifeCost = 0;
        if (card->hasType(Subtypes::TYPE_EQUIPMENT))
        {
            int ownCreatures = 0;
            for (int ei = 0; game && game->inPlay && ei < game->inPlay->nb_cards; ei++)
            {
                MTGCardInstance * ec = game->inPlay->cards[ei];
                if (!ec || (ec->mutation && !ec->parentCards.empty()))
                    continue;
                if (ec->isCreature())
                    ownCreatures++;
            }
            const string lowEq = toLowerCopy(card->magicText) + " " + toLowerCopy(card->text);
            const bool ownBody = lowEq.find("living weapon") != string::npos
                                 || lowEq.find("germ") != string::npos
                                 || lowEq.find("token") != string::npos;
            o << equipmentCastPriceTag(true, ownCreatures, ownBody);
        }
        //#W55-C (D10): a naming permanent of THEIRS prices this cast. Scanned
        //over their battlefield, matched on the chosen name the tag already
        //renders, priced off that permanent's own trigger line.
        {
            Player * oppN = opponent();
            MTGGameZone * obf = (oppN && oppN->game) ? oppN->game->inPlay : NULL;
            for (int ni = 0; obf && ni < obf->nb_cards; ni++)
            {
                MTGCardInstance * nc = obf->cards[ni];
                if (!nc || nc->chooseaname.empty()
                    || toLowerCopy(nc->chooseaname) != toLowerCopy(card->name))
                    continue;
                int nLife = 0, nDraw = 0;
                if (!namedCastPenaltyScan(nc->magicText, nLife, nDraw))
                    continue;
                //#W60-Q (R1): every naming permanent prices this cast, not
                //just the first one found. Each tag is rendered against the
                //life the pilot has left AFTER the punishers already counted,
                //so the "you would be at" figures read as the sequence the
                //triggers actually resolve in and the last one is the truth.
                o << namedCastPriceTag(nc->getDisplayName() + instanceHandle(nc),
                                       nLife, nDraw, life >= 0 ? life - rowSelfLifeCost : life);
                if (nLife > 0)
                    rowSelfLifeCost += nLife; //#W60-L (B1)
            }
        }
        //#W47 (R14b): what this cast LEAVES UP, from the engine's own auto-tap
        //plan for this exact cost (ManaEngine::selectAutoTapProducers - the
        //producers the payment will really activate, pool-aware, no clicks).
        //Counted per SOURCE CARD, matching the mana line's counting rule, so a
        //dual scripted as two abilities is one source here too.
        {
            ManaCost * payCost = casts[ci].viaAlternative && cost
                                 ? cost->getAlternative() : cost;
            //An {X} cost has no remainder yet: X is announced AFTER this pick
            //(the {X pricing:} block above is where that window is priced), so
            //any number printed here would be about a payment the pilot has not
            //made. Print nothing rather than a figure that goes stale one
            //decision later.
            if (payCost && (payCost->hasX() || payCost->hasSpecificX()))
            {
                //#W62-Z (D14): ...and the row says WHY it carries no count, and
                //what the cast at its own badge's X actually leaves. Same
                //affordability call the {X pricing:} block above already made,
                //so the two brackets on this row cannot name different numbers.
                const int mx = ManaEngine::maxAnnounceableX(this, payCost,
                                                            card->has(Constants::ANYTYPEOFMANA));
                //#W74-CE (O8): the CHEAPEST castable card in hand - the one the
                //X is most likely to strand and the one a plan is built on. Same
                //counting rule as `strandsHandCardTag` (converted cost against
                //sources), so the two clauses on this menu cannot name different
                //numbers for the same card.
                MTGCardInstance * keep = NULL;
                int keepNeed = 0;
                //#W78-CW (S14, wave-77 deck125 seq 180): ...and the cheapest
                //INSTANT, separately. The tap-out gate a counterspell deck plays
                //by is "can I still hold up my cheapest ANSWER", and the O8
                //clause answers a different question: it names the cheapest
                //castable CARD in hand, which on a control seat is routinely a
                //sorcery-speed body that the seat had no intention of holding.
                //`162` deck125 seq 180 announced X=9, tapped out, and the
                //opponent resolved four permanents on the following turn with
                //two Essence Scatters stranded in hand. Same counting rule as
                //the keep candidate beside it (converted cost against sources)
                //so the two figures on one row cannot disagree.
                MTGCardInstance * keepInstant = NULL;
                int keepInstantNeed = 0;
                //#W78-CY (F9, Astra MED 9): CONVERTED COST AND A SOURCE COUNT ARE
                //NOT PAYABILITY. Essence Scatter against nothing but Mountains has
                //converted cost 2 and two sources, so the wave-78 clause promised a
                //blue answer the seat cannot cast. The candidate is now priced by the
                //SAME forecast the cast rows are priced by
                //(`ManaEngine::selectAutoTapProducers`, preserveOptions=false), which
                //is colour-aware and returns an empty plan for a cost this pool cannot
                //meet; the figure printed is the number of SOURCES that plan taps,
                //which is the currency `{leaves N of your M}` on this row counts in.
                bool keepAlt = false, keepInstantAlt = false;
                //#W79-DB (T8): the COUNTERSPELL arm. Tracked separately so the
                //cheapest-instant arithmetic above is untouched and the choice
                //between them is one comparison, made once, below.
                MTGCardInstance * keepCounter = NULL;
                int keepCounterNeed = 0;
                bool keepCounterAlt = false;
                for (int hi = 0; hi < game->hand->nb_cards; hi++)
                {
                    MTGCardInstance * hc = game->hand->cards[hi];
                    if (!hc || hc == card || hc->hasType(Subtypes::TYPE_LAND) || !hc->getManaCost())
                        continue;
                    const int need = w78PayableSourceCount(this, hc);
                    if (!w78KeepCandidateStands(need, untappedSources))
                        continue;
                    const bool alt = (hc->getManaCost()->getAlternative() != NULL);
                    if (!keep || need < keepNeed)
                    {
                        keep = hc;
                        keepNeed = need;
                        keepAlt = alt;
                    }
                    if (hc->hasType(Subtypes::TYPE_INSTANT)
                        && (!keepInstant || need < keepInstantNeed))
                    {
                        keepInstant = hc;
                        keepInstantNeed = need;
                        keepInstantAlt = alt;
                    }
                    //#W79-DC (F6): ...and it must be a spell this seat can actually
                    //hold up - an instant, or a card with flash. Ertai's activated
                    //`fizzle` is not an answer the reserve can buy.
                    if (w79CounterspellScript(hc->magicText,
                                              hc->hasType(Subtypes::TYPE_INSTANT)
                                              || hc->basicAbilities[Constants::FLASH] != 0) //#W79-DB (T8)
                        && (!keepCounter || need < keepCounterNeed))
                    {
                        keepCounter = hc;
                        keepCounterNeed = need;
                        keepCounterAlt = alt;
                    }
                }
                //#W79-DB (T8): the counter is the answer the gate is about, so it
                //takes the slot whenever one is in hand - even when a cheaper
                //non-counter instant exists, which is the case that misfired.
                const bool instIsCounter = (keepCounter != NULL);
                if (keepCounter)
                {
                    keepInstant = keepCounter;
                    keepInstantNeed = keepCounterNeed;
                    keepInstantAlt = keepCounterAlt;
                }
                if (untappedSources > 0)
                    o << xCastRemainderScopeTag(mx, payCost->getConvertedCost(),
                                                payCost->xColor > 0
                                                && !card->has(Constants::ANYTYPEOFMANA),
                                                keep ? keep->getDisplayName() : string(),
                                                keep ? w78KeepCostText(keep->getManaCost()->toString(),
                                                                       keepAlt) : string(),
                                                keep ? keepNeed : -1,
                                                untappedSources, //#W74-CF (F6)
                                                keep && card //#W75-CL (P11)
                                                    && keep->getDisplayName() == card->getDisplayName(),
                                                //#W78-CW (S14): the instant half
                                                keepInstant && keepInstant != keep
                                                    ? keepInstant->getDisplayName() : string(),
                                                keepInstant && keepInstant != keep
                                                    ? w78KeepCostText(keepInstant->getManaCost()->toString(),
                                                                      keepInstantAlt) : string(),
                                                keepInstant && keepInstant != keep
                                                    ? keepInstantNeed : -1,
                                                instIsCounter); //#W79-DB (T8)
                payCost = NULL; //hasX() alone answers 0 for a {X:colour} cost
            }
            int used = 0;
            std::vector<std::string> creatureTaps; //#W49-D11
            std::vector<int> creatureTapDefender; //#W53-O (D6)
            std::vector<std::string> painNames; //#W52-K D7
            std::vector<int> painDamage;
            if (payCost)
            {
                vector<MTGAbility*> picks = ManaEngine::selectAutoTapProducers(
                    this, card, payCost, card->has(Constants::ANYTYPEOFMANA), false);
                std::set<MTGCardInstance *> tapped;
                for (size_t pi = 0; pi < picks.size(); pi++)
                    if (picks[pi] && picks[pi]->source)
                    {
                        if (tapped.insert(picks[pi]->source).second)
                        {
                            if (picks[pi]->source->isCreature())
                            {
                                creatureTaps.push_back(picks[pi]->source->getDisplayName()
                                                       + animatedThisTurnNote(picks[pi]->source));
                                creatureTapDefender.push_back(
                                    picks[pi]->source->basicAbilities[Constants::DEFENDER] ? 1 : 0);
                            }
                            int selfDmg = ManaEngine::producerSelfDamageOf(picks[pi]);
                            if (selfDmg > 0)
                            {
                                painNames.push_back(picks[pi]->source->getDisplayName()
                                                    + instanceHandle(picks[pi]->source));
                                painDamage.push_back(selfDmg);
                            }
                        }
                    }
                used = (int) tapped.size();
            }
            if (payCost)
            {
                o << leavesUntappedTag(untappedSources, used);
                rowUsed = used; //#W54-C (D18): this row's price, for the menu pass
            }
            //#W52-L (D17): what this cast strands in the hand at INSTANT speed -
            //"Cast Staff of Nin {6} {leaves 1 of your 7}" was taken with Cancel
            //in hand and the next opponent turn resolved three threats with no
            //window (deck125 vs152 seq 24). The cheapest instant-speed card the
            //remainder cannot pay, same per-source count as the ability rows'
            //clause (lane E, D9).
            if (payCost && untappedSources > 0 && game && game->hand)
            {
                int left = untappedSources - used;
                if (left < 0)
                    left = 0;
                MTGCardInstance * best = NULL;
                int bestNeed = 0;
                for (int hi = 0; hi < game->hand->nb_cards; hi++)
                {
                    MTGCardInstance * hc = game->hand->cards[hi];
                    if (!hc || hc == card || hc->hasType(Subtypes::TYPE_LAND) || !hc->getManaCost())
                        continue;
                    if (!(hc->hasType(Subtypes::TYPE_INSTANT) || hc->has(Constants::FLASH)))
                        continue;
                    int need = hc->getManaCost()->getConvertedCost();
                    if (need <= 0 || need > untappedSources || need <= left)
                        continue;
                    if (!best || need < bestNeed)
                    {
                        best = hc;
                        bestNeed = need;
                    }
                }
                if (best)
                    o << strandsHandCardTag(used, untappedSources, best->getDisplayName(),
                                            best->getManaCost()->toString(), bestNeed);
                //#W66-AT (deck162 MED): the sorcery-speed half, and only in the
                //window where the two speeds differ - the seat's own turn, before
                //its main phase, on a row that can itself wait. The most
                //EXPENSIVE stranded sorcery-speed card is named: it is the one
                //the remainder is furthest from, and the one a plan is built on.
                if ((card->hasType(Subtypes::TYPE_INSTANT) || card->has(Constants::FLASH))
                    && observer && observer->currentPlayer == this
                    && observer->getCurrentGamePhase() < MTG_PHASE_FIRSTMAIN)
                {
                    MTGCardInstance * sorc = NULL;
                    int sorcNeed = 0;
                    for (int hi = 0; hi < game->hand->nb_cards; hi++)
                    {
                        MTGCardInstance * hc = game->hand->cards[hi];
                        if (!hc || hc == card || hc->hasType(Subtypes::TYPE_LAND) || !hc->getManaCost())
                            continue;
                        if (hc->hasType(Subtypes::TYPE_INSTANT) || hc->has(Constants::FLASH))
                            continue; //that card has the same freedom this row has
                        int need = hc->getManaCost()->getConvertedCost();
                        if (need <= 0 || need > untappedSources || need <= left)
                            continue;
                        if (!sorc || need > sorcNeed)
                        {
                            sorc = hc;
                            sorcNeed = need;
                        }
                    }
                    if (sorc)
                    {
                        //#W72-BV (M9): the land drop the seat has not spent. At
                        //most ONE land can be played per turn, so the bonus is 0
                        //or 1, and it counts only for a land that produces mana
                        //ON ARRIVAL - `castBodyEntersTapped` is the engine's own
                        //`tap(noevent)` idiom and is deliberately unqualified,
                        //so a card it mis-reads UNDER-counts the remainder and
                        //can only make this clause smaller.
                        //#W72-BX (F7): and only for a land that MAKES mana - the
                        //wave-72 walk counted any land not entering tapped, so an
                        //unused Dark Depths (or a fetchland, or Maze of Ith)
                        //erased a true reserve warning by contributing a source
                        //that produces nothing.
                        int landDropSources = 0;
                        {
                            std::vector<LegalActionsOracle::Cast> lp =
                                LegalActionsOracle::legalLandPlays(this);
                            for (size_t li = 0; li < lp.size() && !landDropSources; li++)
                            {
                                MTGCardInstance * lc = lp[li].card;
                                if (!lc || castBodyEntersTapped(lc))
                                    continue;
                                const bool basicMana = lc->hasType(Subtypes::TYPE_BASIC)
                                    || lc->hasSubtype("Plains") || lc->hasSubtype("Island")
                                    || lc->hasSubtype("Swamp") || lc->hasSubtype("Mountain")
                                    || lc->hasSubtype("Forest");
                                if (w72LandDropIsManaSource(basicMana, scriptAllZones(lc)))
                                    landDropSources = 1;
                            }
                        }
                        o << sorceryReserveClause(left, sorc->getDisplayName(),
                                                  sorc->getManaCost()->toString(), sorcNeed,
                                                  landDropSources); //#W72-BV (M9)
                    }
                }
            }
            //#W49-D11: the creatures this payment taps, and whether that costs
            //the attack (only on the caster's turn, before attackers are declared).
            if (!creatureTaps.empty())
            {
                bool beforeAttack = observer->currentPlayer == this
                    && observer->getCurrentGamePhase() < MTG_PHASE_COMBATATTACKERS;
                //#W53-O (D6): a defender's tap costs the BLOCK, not the attack.
                bool blockStillMatters = observer->currentPlayer == this
                    || observer->getCurrentGamePhase() < MTG_PHASE_COMBATBLOCKERS;
                std::vector<int> tapRestrict;
                for (size_t ri = 0; ri < creatureTaps.size(); ri++)
                {
                    bool def = ri < creatureTapDefender.size() && creatureTapDefender[ri];
                    tapRestrict.push_back(def
                        ? (blockStillMatters ? (int) TAP_RESTRICT_NO_BLOCK : (int) TAP_RESTRICT_NONE)
                        : (beforeAttack ? (int) TAP_RESTRICT_NO_ATTACK : (int) TAP_RESTRICT_NONE));
                }
                o << paymentTapsClause(creatureTaps, tapRestrict, //#W79-DA (T7)
                                       w79UntapEngineSources(this, opponent()));
            }
            //#W52-K D7: the life this payment plan spends, and where it leaves you.
            o << paymentLifeCostClause(painNames, painDamage, life);
            //#W66-AT (deck130 MED): and where it does not.
            {
                int painTotal = 0;
                for (size_t pz = 0; pz < painDamage.size(); pz++)
                    if (painDamage[pz] > 0)
                        painTotal += painDamage[pz];
                o << paymentNoLifeCostClause(!ManaEngine::selfDamageManaSources(this).empty(),
                                             painTotal, used);
            }
            //#W60-L (B1): same subtraction, into the row's win fold.
            for (size_t pdi = 0; pdi < painDamage.size(); pdi++)
                if (painDamage[pdi] > 0)
                    rowSelfLifeCost += painDamage[pdi];
        }
        //A response option offered because of pending stack objects names what
        //it can hit ("Cast Counterspell {u}{u} - can target on the stack:
        //Master of Etherium"): the offer used to be a bare cast line and the
        //model could not tell WHY the window opened (wave-4 E2). Cheap: only
        //stack-zone-targeting cards ever append anything.
        {
            TargetChooserFactory tcf(observer);
            TargetChooser * tc = tcf.createTargetChooser(card);
            if (tc)
            {
                std::ostringstream hits;
                bool firstHit = true;
                for (int pi = 0; pi < 2; pi++)
                {
                    MTGGameZone * sz = observer->players[pi]->game->stack;
                    if (!tc->targetsZone(sz))
                        continue;
                    for (int zi = 0; zi < sz->nb_cards; zi++)
                        if (tc->canTarget(sz->cards[zi]))
                        {
                            //W39-STACKFACTS: the counter option's own clause is
                            //the OFFER side of the same absent-field audit -
                            //annotate the hit with what it is worth countering
                            //for (type, cost, P/T).
                            //#W48-D7: keywords ride here too - this clause is
                            //the one the counter DECISION is read off.
                            hits << (firstHit ? "" : ", ") << sz->cards[zi]->getDisplayName()
                                 << targetPreviewFacts(sz->cards[zi])
                                 << stackTargetTextNote(sz->cards[zi]);
                            //#W78-CW (S5): does one of THIS seat's own pending
                            //counters already aim at this object? The join is
                            //over the live stack: an unresolved spell of ours,
                            //not this row's own card, whose script fizzles a
                            //spell and whose target list holds this object.
                            {
                                string mine;
                                //#W78-CY (F3): ...and whether that counter is
                                //CONDITIONAL, read from the same script the join
                                //found `fizzle` in.
                                bool mineConditional = false;
                                string mineEscape;
                                ActionStack * astk = observer->mLayers->stackLayer();
                                for (size_t ai = 0; astk && ai < astk->mObjects.size() && mine.empty(); ai++)
                                {
                                    Interruptible * ait = (Interruptible *) astk->mObjects[ai];
                                    if (!ait || ait->type != ACTION_SPELL || ait->state != NOT_RESOLVED)
                                        continue;
                                    if (!ait->source || ait->source == card
                                        || ait->source->controller() != this)
                                        continue;
                                    if (ait->source->magicText.find("fizzle") == string::npos)
                                        continue;
                                    Spell * asp = (Spell *) ait;
                                    for (Targetable * t = asp->getNextTarget(); t;
                                         t = asp->getNextTarget(t))
                                        if ((MTGCardInstance *) dynamic_cast<MTGCardInstance *>(t)
                                            == sz->cards[zi])
                                        {
                                            mine = ait->source->getDisplayName();
                                            mineConditional =
                                                w78ConditionalCounterScript(ait->source->magicText);
                                            mineEscape =
                                                w78CounterEscapeCost(ait->source->magicText);
                                            break;
                                        }
                                }
                                hits << w78RedundantCounterTag(mine, mineConditional, mineEscape);
                            }
                            //#W80-DG (U11): and the same fact on the CAST row, so
                            //the two surfaces the decision is read off agree.
                            if (sz->cards[zi] && sz->cards[zi]->currentZone)
                                hits << w80OwnStackSpellTag(
                                    sz->cards[zi]->controller() == this,
                                    zoneDesc(sz->cards[zi]->currentZone));
                            firstHit = false;
                        }
                }
                //Own-targets-only warning: a mandatory-target removal
                //spell whose only legal targets are the CASTER's own
                //permanents is offered legally (601.2c satisfied - you may
                //destroy your own creature) but reads as a trap from a
                //bare cast line (deck44 wave-8: GFTT offered with only its
                //own Faerie legal; the pilot correctly declined but paid a
                //reasoning tax every window). Say it at the option. And
                //when legal targets DO exist, NAME them (capped): a bare
                //targeted cast line invited a fabricated "it has no legal
                //target" belief over a perfectly legal opponent creature
                //(deck44 wave-9 s54, a 900-char plan arguing itself out of
                //its removal). The deciding fact rides the option.
                if (tc->maxtargets == 1)
                {
                    int ownT = 0, oppT = 0;
                    std::ostringstream tNames;
                    int tShown = 0;
                    //Enumerate ALL legal targets, OPPONENT side FIRST (the
                    //strategically-live burn/removal targets), then the
                    //caster's own. Do NOT truncate behind "(+N more)": a
                    //capped, own-creatures-first preview hid an opponent's
                    //Young Pyromancer (deck109 wave-25 vs131 s11/s8) and the
                    //pilot declined a guide-mandated on-sight kill because it
                    //read the truncated preview as the complete legal set - a
                    //legal play suppressed by omission. Owner ruling: never
                    //hide a legal play on a representation cap. A single-target
                    //preview is cheap (corpus p95 prompt sizes are fine), and
                    //the downstream chooseTarget window already shows the full
                    //list - so the option preview must not disagree with it.
                    //N-158h part 2: a spell whose SELF-COST is "life equal to that
                    //permanent's mana value" (script "life:-manacost") cannot be
                    //priced at option-build time, so the flat "{right now: life
                    //-2}" clause is dropped (see dynamicMagnitudes). Here the
                    //target IS known, so fold the real per-target cost into the
                    //list that already enumerates them - the deciding fact rides
                    //the target it belongs to.
                    bool lifeCostPerTarget = false;
                    {
                        string mtl = card->magicText;
                        for (size_t li = 0; li < mtl.size(); li++)
                            mtl[li] = (char) tolower((unsigned char) mtl[li]);
                        lifeCostPerTarget = mtl.find("life:-manacost") != string::npos;
                    }
                    //#W44-3 (wave-43 seat125/126 engine HIGH): the option line
                    //carries the SPELL's own {card text: "..."} and, for each
                    //legal target, name + cost + type + P/T + keywords - every
                    //fact EXCEPT what the target actually DOES. Both of deck125's
                    //HIGH losses lived in text nobody rendered: a 1-power
                    //Blastminer eating the manabase (15 Path declines) and a
                    //1-power Adventurer venturing four times (10 declines), each
                    //protected by a printed-power floor because the line the pilot
                    //read said "1/1" and nothing about the ability. This is the
                    //counter lane's W43-8 fix one emitter over, same helper, same
                    //140-char target-side length: the deciding fact rides the
                    //object the decision is ABOUT.
                    //Line budget: the battlefield summary quotes no text at all,
                    //so nothing here is a duplicate of the board render; a
                    //textless (vanilla) target adds nothing by construction; and
                    //two instances of the same card quote their shared text ONCE
                    //(the second is byte-identical prompt weight).
                    //#W50-X D5: (name, handle, fact-tail) triples + a per-name
                    //text note, joined by joinTargetEntries (ranged collapse).
                    std::vector<string> tgtNames, tgtHandles, tgtTails;
                    std::map<string, string> tgtNotes;
                    std::vector<MTGCardInstance *> tgtCards; //#W53-O (D5)
                    Player * ordered[2] = { this->opponent(), this };
                    for (int oi = 0; oi < 2; oi++)
                    {
                        Player * pp = ordered[oi];
                        if (!pp)
                            continue;
                        MTGGameZone * zz[] = { pp->game->inPlay, pp->game->graveyard, pp->game->hand, pp->game->exile, pp->game->commandzone };
                        for (int zi = 0; zi < 5; zi++)
                            if (tc->targetsZone(zz[zi]))
                                for (int cj = 0; cj < zz[zi]->nb_cards; cj++)
                                    if (tc->canTarget(zz[zi]->cards[cj]))
                                    {
                                        MTGCardInstance * tgt = zz[zi]->cards[cj];
                                        (tgt->controller() == this ? ownT : oppT)++;
                                        tShown++;
                                        //W41-8: cost + type + (P/T) + live
                                        //keywords, the SAME facts W39-STACKFACTS
                                        //put on the stack clause. At exactly one
                                        //legal target the engine makes no model
                                        //call for the target, so this preview is
                                        //the ONLY surface carrying them.
                                        string tail = targetPreviewFacts(tgt);
                                        //N-158k: the SAME helper the target menu
                                        //now uses, so the cast preview and the
                                        //commit seat cannot drift apart.
                                        if (lifeCostPerTarget)
                                            tail += perTargetLifeCostNote(tgt);
                                        tgtNames.push_back(tgt->getDisplayName());
                                        tgtHandles.push_back(instanceHandle(tgt));
                                        tgtTails.push_back(tail);
                                        tgtCards.push_back(tgt); //#W53-O (D5)
                                        //#W44-3: what the target DOES, quoted
                                        //once per distinct card (see above).
                                        if (tgtNotes.find(tgt->getDisplayName()) == tgtNotes.end())
                                            tgtNotes[tgt->getDisplayName()] = stackTargetTextNote(tgt);
                                    }
                        if (tc->canTarget(pp))
                        {
                            (pp == this ? ownT : oppT)++;
                            tShown++;
                            tgtNames.push_back(pp == this ? "you" : "the opponent");
                            tgtHandles.push_back("");
                            tgtTails.push_back("");
                            tgtCards.push_back(NULL); //#W53-O (D5): a player target
                        }
                    }
                    tNames << joinTargetEntries(tgtNames, tgtHandles, tgtTails, tgtNotes);
                    //#W53-O (D5): what this magnitude does to the set just
                    //enumerated - the same predicate the target ask's verdicts
                    //print, summarised on the refusable window.
                    {
                        bool castDeathtouch = false;
                        int castDmg = spellSingleDamageAmount(card, castDeathtouch);
                        int castDrop = castDmg ? 0 : spellPTDropAmount(observer, card);
                        if (castDmg > 0 || castDrop > 0)
                        {
                            std::vector<std::string> killed, killedMine; //#W55-C (D15)
                            int creatureTargets = 0;
                            for (size_t ki = 0; ki < tgtCards.size(); ki++)
                            {
                                MTGCardInstance * kc = tgtCards[ki];
                                if (!kc || !kc->isCreature() || !kc->controller()
                                    || !kc->controller()->game
                                    || kc->currentZone != kc->controller()->game->inPlay)
                                    continue;
                                creatureTargets++;
                                bool dies = castDmg > 0
                                    ? damageKillsTarget(castDmg, kc->life,
                                                        kc->basicAbilities[Constants::INDESTRUCTIBLE] != 0,
                                                        castDeathtouch)
                                    : ptDropKillsTarget(castDrop, kc->life);
                                if (dies)
                                {
                                    if (kc->controller() == this) //#W55-C (D15)
                                        killedMine.push_back(kc->getDisplayName() + instanceHandle(kc));
                                    else
                                        killed.push_back(kc->getDisplayName() + instanceHandle(kc));
                                }
                            }
                            std::ostringstream mag;
                            if (castDmg > 0)
                                mag << castDmg << " damage";
                            else
                                mag << "-" << castDrop << "/-" << castDrop;
                            //#W54-C (D4 part ii): the damage that reaches the
                            //PLAYER on the same enumeration, and what it means.
                            Player * oppP = this->opponent();
                            int m11Gain = 0, m11Turns = 0; //#W72-BW (M11)
                            oppLifeRise(m11Gain, m11Turns);
                            string playerTail = castPlayerDamageTail(
                                castDmg, oppP && tc->canTarget(oppP), oppP ? oppP->life : -1,
                                life, rowSelfLifeCost, m11Gain, m11Turns); //#W60-L (B1), #W72-BW (M11)
                            o << castKillSummaryTag(killed, creatureTargets, mag.str(), playerTail,
                                                    killedMine); //#W55-C (D15)
                            //#W77-CS (R6 a): and what each of those victims is
                            //worth off the CRACK-BACK NEXT TURN line above.
                            //Gated through the same crackBackScreenTotal as
                            //every sibling in the cover family, so this row can
                            //never point at a line that is not above it, and the
                            //per-body figure is the SAME crackBackBodyContribution
                            //walk the line itself is built from.
                            if (!killed.empty())
                            {
                                int r6Total = 0;
                                bool r6Floor = false;
                                if (crackBackScreenTotalNow(r6Total, r6Floor))
                                {
                                    std::vector<W77RemovalVictim> r6v;
                                    int r6Bodies = 0;
                                    Player * r6Opp = opponent();
                                    MTGGameZone * r6bf = (r6Opp && r6Opp->game)
                                                             ? r6Opp->game->inPlay : NULL;
                                    for (int r6i = 0; r6bf && r6i < r6bf->nb_cards; r6i++)
                                        if (crackBackBodyContribution(r6bf->cards[r6i]) > 0)
                                            r6Bodies++;
                                    for (size_t ki = 0; ki < tgtCards.size(); ki++)
                                    {
                                        MTGCardInstance * kc = tgtCards[ki];
                                        if (!kc || !kc->isCreature()
                                            || kc->controller() != r6Opp)
                                            continue;
                                        const bool dies = castDmg > 0
                                            ? damageKillsTarget(castDmg, kc->life,
                                                  kc->basicAbilities[Constants::INDESTRUCTIBLE] != 0,
                                                  castDeathtouch)
                                            : ptDropKillsTarget(castDrop, kc->life);
                                        if (!dies)
                                            continue;
                                        //#W77-CU (F3, Astra HIGH 3): the same
                                        //survival test the roster and the O1
                                        //forecast read. A regenerating,
                                        //indestructible, ghostformed or
                                        //persist/undying body is still NAMED as
                                        //a victim by the kill summary beside
                                        //this clause - only the SURVIVAL PROMISE
                                        //stops being made on its behalf, because
                                        //it untaps and attacks next turn. This is
                                        //CQ F5's bug, and it was re-introduced
                                        //here the wave it was fixed.
                                        const int c = w77CoverBodyContribution(kc, 1);
                                        if (c <= 0)
                                            continue;
                                        W77RemovalVictim v;
                                        v.name = kc->getDisplayName() + instanceHandle(kc);
                                        v.takes = c;
                                        r6v.push_back(v);
                                    }
                                    o << w77TargetedRemovalCoverTag(r6Total, life, r6Floor,
                                                                    r6v, r6Bodies);
                                }
                            }
                        }
                        else
                        {
                            //#W54-C (D11): unconditional removal has no
                            //magnitude to test, and a perfectly knowable
                            //victim list already printed one clause away.
                            string rverb = spellRemovalVerb(card);
                            if (!rverb.empty())
                            {
                                std::vector<std::string> victims, immune, mineV;
                                for (size_t ki = 0; ki < tgtCards.size(); ki++)
                                {
                                    MTGCardInstance * kc = tgtCards[ki];
                                    if (!kc || !kc->controller() || !kc->controller()->game
                                        || kc->currentZone != kc->controller()->game->inPlay)
                                        continue;
                                    string nm = kc->getDisplayName() + instanceHandle(kc);
                                    if (w76BodyReturnsInsteadOfDying(kc)) //#W76-CP (Q10)
                                        nm += kGhostformNotRemovalTail;
                                    if (rverb == "kills"
                                        && kc->basicAbilities[Constants::INDESTRUCTIBLE] != 0)
                                        immune.push_back(nm);
                                    else if (kc->controller() == this) //#W55-C (D15)
                                        mineV.push_back(nm);
                                    else
                                        victims.push_back(nm);
                                }
                                o << removalVictimTag(rverb, victims, immune, mineV);
                            }
                        }
                    }
                    if (ownT && !oppT && firstHit)
                    {
                        //W42-D5 (trust doctrine): this branch used to name
                        //NOTHING - 59 corpus option lines asserted a
                        //restriction and then withheld the set it restricted
                        //to, which is exactly the silent omission the model
                        //confabulates into (it cannot weigh "my own" without
                        //knowing WHICH of its own). `tNames` is already built
                        //with the same facts the opponent-side branch below
                        //carries (name, instance handle, cost/type/P-T/keywords
                        //via targetPreviewFacts), so name them here too and the
                        //two branches cannot drift apart.
                        o << ownTargetsOnlyClause(tNames.str());
                        //Owner ruling (2026-07-16): a legal play is never
                        //hidden on a strategy judgment - strange cards make
                        //normally-nonsensical lines correct (heroic, cast
                        //triggers, death-trigger value, aura-count scaling).
                        //The classifier's verdict rides the option line as a
                        //warning; the model decides.
                        if (effectBadOrGood(card, MODE_TARGET, tc) == BAKA_EFFECT_BAD)
                            o << " (warning: this would harm your own side - only"
                                 " correct if you are deliberately triggering"
                                 " something)";
                    }
                    else if (ownT + oppT > 0)
                    {
                        //Full list, opponent-first: no "(+N more)" truncation
                        //(see the enumeration comment above).
                        o << " - legal targets right now: " << tNames.str();
                        //#W55-C (D7 b): scoped to enumerations that include a
                        //PLAYER row - that is the shape the model asked about
                        //in writing (`123v126` seq 104), and a clause on every
                        //multi-target cast row would be prompt weight for a
                        //question nothing in the corpus asked there.
                        {
                            bool playerOnList = false;
                            for (size_t pci = 0; pci < tgtCards.size(); pci++)
                                if (!tgtCards[pci])
                                    playerOnList = true;
                            if (playerOnList)
                                o << targetCommitClause(tShown);
                        }
                        //Owner ruling (2026-07-16): same as the own-side case
                        //above - annotate, never hide. A beneficial cast with
                        //only opponent-side targets is usually futile (deck49
                        //War Paint loops) but not always (aura-count payoffs,
                        //cast triggers), so the warning carries the fact and
                        //the model decides.
                        if (oppT && !ownT && firstHit
                            && effectBadOrGood(card, MODE_TARGET, tc) == BAKA_EFFECT_GOOD)
                            o << " (warning: every legal target is the OPPONENT's -"
                                 " this would help their side unless you are"
                                 " deliberately triggering something)";
                    }
                    else if (firstHit)
                        o << " - NO legal target right now";
                    //else: the spell's targets live on the STACK - the
                    //stack line appended below names them; claiming "NO
                    //legal target" over an occupied stack contradicted the
                    //counterspell offers it decorated (wave-10, 2 seats).
                }
                //A hand-attack discard spell against a thin hand: the fact
                //that decides ("their hand is nearly/completely empty")
                //is surfaced in the board summary and STILL fabricated
                //over ("they may have a removal spell" at hand size 0,
                //deck133 wave-9 vs131 s58) - the last rung is the option
                //line itself.
                {
                    Player * oppP = this->opponent();
                    if (oppP && tc->canTarget(oppP))
                    {
                        string lowText = card->text;
                        for (size_t li = 0; li < lowText.size(); li++)
                            lowText[li] = (char) tolower((unsigned char) lowText[li]);
                        int oppHand = oppP->game->hand->nb_cards;
                        if (lowText.find("discard") != string::npos && oppHand <= 2)
                        {
                            if (oppHand == 0)
                                o << " - the opponent's hand is EMPTY: nothing to strip";
                            else
                                o << " - the opponent holds only " << oppHand << " card" << (oppHand > 1 ? "s" : "");
                        }
                    }
                }
                //Bounce-on-the-stack cast annotation (deck14 wave-20 E1): when
                //this instant is offered as a response while an opponent spell
                //resolves on the stack, but its chooser reaches only the
                //battlefield (not the stack), say so on the cast line - the
                //model repeatedly cast Unsummon/Boomerang believing it could
                //stop the spell being cast. firstHit stays true when no stack
                //card was targetable. Annotation only; the offer is unchanged.
                if (firstHit)
                {
                    bool oppSpellOnStack = false;
                    for (int pi = 0; pi < 2 && !oppSpellOnStack; pi++)
                    {
                        MTGGameZone * sz = observer->players[pi]->game->stack;
                        for (int zi = 0; zi < sz->nb_cards; zi++)
                        {
                            MTGCardInstance * sc = sz->cards[zi];
                            if (sc && sc != card && sc->controller() != this)
                            {
                                oppSpellOnStack = true;
                                break;
                            }
                        }
                    }
                    bool hitsBattlefield =
                        tc->targetsZone(observer->players[0]->game->inPlay) ||
                        tc->targetsZone(observer->players[1]->game->inPlay);
                    if (oppSpellOnStack && hitsBattlefield)
                        o << " [this cannot target the spell on the stack -"
                             " battlefield permanents only]";
                }
                SAFE_DELETE(tc);
                if (!firstHit)
                    o << " - can target on the stack: " << hits.str();
            }
        }
        //N-146i (wave-31 deck146): activated-ability and loyalty options carry
        //their full {card text: ...} and Cast options carried NONE - 0 of 163 at
        //that seat - so the pilot got rules text for abilities it activated but
        //not for the spells it cast. It cast Acererak the Archlich as a 5/5 body
        //across 59 reply passages planning to attack or block with a creature
        //whose own ETB returns it to hand, and at vs148 seq18 declined an offered
        //Vanishing Verse on the opponent's Kor lord to do it; the board swung 12
        //the next turn.
        //(history: comment-archaeology.md AIPlayerGPTSeams-if-2003)
        if (!card->isLand() && !(casts[ci].viaAlternative && card->has(Constants::ADVENTURE)))
        {
            //Same SHAPE as the ability-line emitter (quoted): one annotation
            //form across both surfaces, so the model reads one convention.
            //#W48 D5: and the same clause/face-aware budget.
            string ct = optionCardText(card, 220);
            if (!ct.empty())
                o << " {card text: \"" << ct << "\"}";
        }
        //#W54-C (D5): which printed MODE of a modal spell has a legal object
        //right now - the fact the engine must already compute to build the
        //mode sub-menu, carried on the refusable window that precedes it.
        {
            std::vector<std::string> liveM, halfM, deadM, liveObjM; //#W80-DG (U12)
            if (modalModeLiveness(observer, card, card->magicText, liveM, halfM, deadM,
                                  NULL, &liveObjM, this))
                o << modalModesTag(liveM, halfM, deadM, &liveObjM); //#W66-AQ (H9), #W80-DG (U12)
        }
        //#W48 D8: and the price of what it DRAWS, when the opponent punishes
        //draws. The summary line on the same screen states the standing rate;
        //the row that incurs it carries the number (deck130 G47-5's general
        //principle, applied to the cast row).
        //#W49-U D6 (c): and what CASTING it makes the caster draw, when an
        //opposing permanent punishes the cast that way (Forced Fruition).
        //#W61-S (C6): the two clauses are emitted from ONE block so the second
        //can fold the first's charge into a single resulting life. Both clauses
        //still print in full; only the "you would be at" is resolved once.
        {
            const int drawn = scriptSelfDrawCount(card->magicText);
            std::vector<std::string> castNames;
            int perCast = 0;
            castTriggerDrawScan(opponent(), castNames, perCast);
            vector<string> mineP, theirsP;
            int minePer = 0, theirsPer = 0;
            if (drawn > 0 || perCast > 0)
                drawPunisherScan(this, opponent(), mineP, minePer, theirsP, theirsPer);
            std::ostringstream pn;
            for (size_t ni = 0; ni < theirsP.size(); ni++)
                pn << (ni ? ", " : "") << theirsP[ni];
            //The second clause states a life total only when it prices punishers
            //too; if it will not, the first clause must keep its own total.
            const bool secondFolds = (perCast > 0 && !castNames.empty()
                                      && theirsPer > 0 && !theirsP.empty());
            int firstCharge = 0;
            if (drawn > 0)
            {
                const bool defer = secondFolds && theirsPer > 0 && !theirsP.empty();
                o << drawPriceRowTag(drawn, theirsPer, pn.str(), life, defer); //#W54-C (D10)
                if (defer)
                    firstCharge = drawn * theirsPer;
            }
            if (perCast > 0)
            {
                std::ostringstream cn;
                for (size_t ni = 0; ni < castNames.size(); ni++)
                    cn << (ni ? ", " : "") << castNames[ni];
                o << castDrawPriceRowTag(perCast, cn.str(), theirsPer, pn.str(), life, firstCharge);
            }
        }
        //#W51-F D11: and what casting it FEEDS the opponent, with the count of
        //the caster's own converters (draw AND discard punishers).
        {
            bool variable = false;
            int perTurn = opponentExtraDrawPerTurn(card->magicText, variable);
            int perCastFed = castTriggerDrawCount(card->magicText);
            if (perTurn > 0 || variable || perCastFed > 0)
            {
                std::vector<std::string> conv, discConv; //#W60-N (B5): split
                converterScan(this, conv, discConv);
                std::vector<std::string> handConv, handDiscConv; //#W57-E (D23)
                handConverterScan(this, handConv, handDiscConv);
                bool selfVariable = false; //#W65-AO (MED)
                const int selfPerTurn = ownExtraDrawPerTurn(card->magicText, selfVariable);
                //#W80-DG (U12): the live number behind a `countedamount` that
                //counts the opponent's hand. -1 everywhere else, which is the
                //byte-identical wave-79 face.
                int variableHand = -1;
                if (variable && opponent() && opponent()->game && opponent()->game->hand
                    && w80VariableDrawIsHandSize(card->magicText, "opponenthand"))
                    variableHand = opponent()->game->hand->nb_cards;
                o << feedsRowTag(perTurn, variable, perCastFed, conv, handConv,
                                 discConv, handDiscConv, selfPerTurn, variableHand);
            }
        }
        //#W61-S (C12): and the ONE-SHOT draw grant, which neither of the two
        //scans above sees. Same two numbers every other draw row prints - the
        //converter count on the pilot's own battlefield, and what the draws are
        //worth against it - on the row where the cast is decided.
        {
            std::vector<OneShotDrawBranch> grants;
            oneShotDrawGrantScan(card, grants);
            if (!grants.empty())
            {
                std::vector<std::string> conv, discConv;
                converterScan(this, conv, discConv);
                vector<string> mineP, theirsP;
                int minePer = 0, theirsPer = 0;
                drawPunisherScan(this, opponent(), mineP, minePer, theirsP, theirsPer);
                std::ostringstream pn;
                for (size_t ni = 0; ni < theirsP.size(); ni++)
                    pn << (ni ? ", " : "") << theirsP[ni];
                o << oneShotDrawGrantTag(grants, conv, minePer, theirsPer, pn.str(), life);
            }
        }
        //#W57-C (D7): and, on a CREATURE row, what a converter of THEIRS turns
        //this body into if they get life equal to its toughness out of it. The
        //fact is on the prompt already, in a paragraph the pilot did not carry
        //down to the row (the Emrakul-at-37-life game); this is that paragraph's
        //arithmetic, finished, on the row where the decision is made.
        if (card->isCreature())
        {
            std::vector<std::string> theirConv;
            theirConverterScan(opponent(), theirConv);
            o << theirConverterBodyTag(card->toughness, theirConv, life,
                                       lifeLoopProvenWin(opponent())); //#W65-AN (G6)
        }
        //#W64-AH (F11, deck130 HIGH-2): and, on a row that puts BODIES on the
        //board while the same screen's CRACK-BACK line is lethal, what those
        //bodies do to that line. Gated on the render's own conditions through
        //crackBackScreenTotal, so a row can never point at a line that is not
        //above it, and on the lethal case only.
        {
            //#W75-CK (P7): net of what this row's own text takes back - the
            //legend rule on a second copy of a legendary creature.
            bool cbLegendTwin = false;
            if (card->isCreature() && card->hasType(Subtypes::TYPE_LEGENDARY) && game && game->inPlay)
            {
                std::vector<W75LegendBoardCard> w75board;
                for (int bi = 0; bi < game->inPlay->nb_cards; bi++)
                {
                    MTGCardInstance * bc = game->inPlay->cards[bi];
                    if (!bc)
                        continue;
                    W75LegendBoardCard e;
                    e.name = bc->name;
                    e.isToken = (bc->isToken != 0);
                    e.isCastCard = (bc == card);
                    w75board.push_back(e);
                }
                cbLegendTwin = w75LegendTwinControlled(w75board, card->name);
            }
            //#W80-DF (U3): net of a body the row's OWN resolution takes off
            //the battlefield (Acererak's ETB bounce), too.
            const bool cbSelfLeaves = w80CastSelfLeavesOnResolution(card);
            const int cbRawBodies = castBodiesAdded(card);
            const int bodies = castBodiesNetOfOwnText(cbRawBodies,
                                                      card->isCreature() != 0, cbLegendTwin,
                                                      cbSelfLeaves); //#W80-DF (U3)
            int cbTotal = 0;
            bool cbFloor = false;
            if (bodies <= 0 && cbRawBodies > 0 && cbSelfLeaves //#W80-DF (U3)
                && crackBackScreenTotalNow(cbTotal, cbFloor))
                o << w80SelfLeavesNoCoverClause(card->getDisplayName(), cbTotal);
            cbTotal = 0;
            cbFloor = false;
            if (bodies > 0
                && crackBackScreenTotalNow(cbTotal, cbFloor))
            {
                //#W64-AK (R4/R7): the cover is computed against each attacker's
                //own block legality, and the bodies are split into checked and
                //unchecked - see crackBackCoverFacts.
                std::vector<CrackBackAttackerFact> cbAtk;
                int cbChecked = 0, cbUnchecked = 0, cbExisting = 0; //#W69-BH (K6a)
                //#W69-BJ (F2): this row renders in main 1 as well as main 2, and
                //in main 1 the seat has not declared attackers yet.
                const bool cbSettled = (observer && observer->currentPlayer == this
                                        && observer->getCurrentGamePhase()
                                               >= MTG_PHASE_COMBATATTACKERS);
                crackBackCoverFacts(opponent(), card, bodies, cbAtk, cbChecked, cbUnchecked,
                                    this, &cbExisting, cbSettled); //#W69-BH (K6a) #W69-BJ (F2)
                //#W65-AL (G4): and WHICH number this is - crackBackScreenTotal
                //already reports whether the line above calls it a floor.
                //#W79-DA (T6): the SAME ADD-THOSE-UP figure the line above
                //prints, read from the same scan the line reads it from.
                int cbExtra = 0;
                bool cbUnsized = false;
                crackBackFloorSources(opponent(), &cbExtra, &cbUnsized);
                o << crackBackBlockerRowTag(cbTotal, life, cbChecked, cbUnchecked,
                                            cbAtk, cbFloor, cbExisting, //#W69-BH (K6a)
                                            cbExtra); //#W79-DA (T6)
            }
        }
        //#W68-BD (J8, deck126 MED): and, on a row that REMOVES one of their
        //attackers, what it does to the same line. Gated through the same
        //crackBackScreenTotal, so this row can never point at a line that is
        //not above it; the bodies in the total come from the SAME
        //crackBackBodyContribution walk the line itself is built from, and the
        //creature count from edictFloorScan, the walk the row's own edict floor
        //came from.
        if (rowSweep.edictKind == 1)
        {
            int cbTotal2 = 0;
            bool cbFloor2 = false;
            if (crackBackScreenTotalNow(cbTotal2, cbFloor2))
            {
                int edT = 0, edMin = 0, edAt = 0;
                edictFloorScan(opponent(), edT, edMin, edAt, NULL);
                int bodies2 = 0, minPow = 0;
                MTGGameZone * obf = (opponent() && opponent()->game)
                                    ? opponent()->game->inPlay : NULL;
                for (int bi = 0; obf && bi < obf->nb_cards; bi++)
                {
                    const int pw = crackBackBodyContribution(obf->cards[bi]);
                    if (pw <= 0)
                        continue;
                    bodies2++;
                    if (!minPow || pw < minPow)
                        minPow = pw;
                }
                o << crackBackRemovalRowTag(cbTotal2, life, cbFloor2, edT, bodies2, minPow);
            }
        }
        //#W76-CO (Q5, wave-75 deck130 HIGH-1): and, on a row that KILLS one of
        //their crack-back attackers outright, the same clause from the bodies
        //it removes. Gated through the SAME crackBackScreenTotal as the two
        //blocks above, so this row can never point at a line that is not above
        //it; `rowSweep.crackRemoved` was summed with crackBackBodyContribution
        //at the row's own kill walk, which is the walk the line is built from.
        if (rowSweep.crackRemoved > 0 && rowSweep.edictKind != 1)
        {
            int cbTotal3 = 0;
            bool cbFloor3 = false;
            if (crackBackScreenTotalNow(cbTotal3, cbFloor3))
            {
                int bodies3 = 0;
                MTGGameZone * obf3 = (opponent() && opponent()->game)
                                     ? opponent()->game->inPlay : NULL;
                for (int bi = 0; obf3 && bi < obf3->nb_cards; bi++)
                    if (crackBackBodyContribution(obf3->cards[bi]) > 0)
                        bodies3++;
                o << crackBackKillRowTag(cbTotal3, life, cbFloor3,
                                         rowSweep.crackRemoved,
                                         rowSweep.crackRemovedBodies, bodies3);
            }
        }
        candidates.push_back(card);
        candidateUsesAlt.push_back(casts[ci].viaAlternative);
        opts.push_back(foldManaBillClauses(o.str())); //#W74-CE (O14)
        rowUses.push_back(rowUsed); //#W54-C (D18)
        rowSweepTheirs.push_back(rowSweep.theirs); //#W61-U (C10)
        rowSweepMine.push_back(rowSweep.mine);
        rowSweepEngines.push_back(rowSweep.engines); //#W66-AQ (H10)
        rowSweepMineNames.push_back(rowSweep.mineNames); //#W80-DF (U7)
        rowSweepMineEngines.push_back(rowSweep.mineEngines);
        //#W56-B (D15): identity + price of this row, for the menu pass below.
        rowNames.push_back(card->name);
        {
            ManaCost * dupCost = (casts[ci].viaAlternative && cost) ? cost->getAlternative() : cost;
            rowCosts.push_back((dupCost && !dupCost->hasX() && !dupCost->hasSpecificX())
                               ? dupCost->getConvertedCost() : -1);
        }
    }

    //Nothing castable: only one outcome, no model call.
    if (candidates.empty())
        return NULL;

    //DIVERGENCE-C (wave-31 ledger #10), the LOUD RE-ASK. The validation pass
    //below can reject a cast the oracle's offerable() approved. Three trigger
    //shapes were witnessed (a convoke shell whose creature reduction cannot
    //complete the printed cost; a PLAIN cast; an X spell with zero slack), and
    //they shared ONE exit: a silent defer that burned the priority window and
    //never told the model. Accepting on offerable() is NOT the fix - the
    //rejects were substantively RIGHT (an X=0 March of the Multitudes is a
    //scripted `this(X=0) donothing`). So keep the reject and RE-PUT the
    //question with the rejected entry removed, capped, and loud in the log.
    //
    //Cache/dedupe safety: askModel keys on board state + the rendered QUESTION
    //(decision line + option list), never the full prompt. A menu with one
    //entry removed - and a decision line carrying the [RE-ASK n] marker - is a
    //different question, so it takes its own cache slot and cannot collide
    //with the original entry, the priority seam's mLastAskKey, or the deadlock
    //breaker. It is also deterministic on re-poll: the full menu is rebuilt
    //identically each tick, replays its cached answer, rejects again, and the
    //reduced menu replays its own.
    //
    //(history: comment-archaeology.md AIPlayerGPTSeams-L4919-2021)
    const int kMaxCastReasks = 2; //3 asks per window, worst case
    string rejectedSoFar;
    //#W72-BX (F3): this seat's casting decision is OPEN from here until the loop
    //returns, however it returns. RAII so no exit path can leave it set.
    struct CastOpenGuard
    {
        int * flag;
        CastOpenGuard(int * f) : flag(f) { if (flag) (*flag)++; }
        ~CastOpenGuard() { if (flag && *flag > 0) (*flag)--; }
    } castOpenGuard(&mCastDecisionOpen);
    for (int attempt = 0; ; attempt++)
    {
        vector<string> menu(opts);
        //#W75-CL (P5): the loop override, before every tag and every key.
        w75ApplyChainFeed(menu, lifeLoopProvenWin(opponent()));
        //#W77-CR (R1): ...and the seat's own loop, on the casting menu too.
        {
            string loopSpell, loopPiece; //#W77-CU (F4)
            const int loopState = w77OwnLoopStackState(loopSpell, loopPiece);
            const bool ownLoopResolving = (loopState == kW77LoopResolving);
            w77ApplyOwnLoopFeed(menu, ownLoopResolving);
            if (loopState == kW77LoopThreatened)
                w77ApplyOwnLoopThreatFeed(menu,
                                          w77OwnLoopThreatTag(loopSpell, loopPiece));
            if (ownLoopResolving && attempt == 0)
                w77CountOwnLoopWindow();
        }
        //#W54-C (D18, wave-53 ledger MED = R178): the MENU pass. Every row
        //priced itself ALONE, so nothing said which rows fit TOGETHER in this
        //window - `162v152` s11 had two blockers affordable together against
        //four declared attackers, took the row that tapped it out, and its own
        //losing reply's plan named the row it had just made impossible. Run
        //per ATTEMPT, over the menu copy: a row cannot know its own number
        //until the suppression filter and any re-ask removal have settled, and
        //`opts` stays the untagged identity the livelock breaker keys on.
        //#W72-BV (M18b): the crack-back total THIS screen prints, if it prints
        //one - gated on exactly the render's own conditions, and taken from the
        //same walk the line itself is built from, so no row can point at a line
        //that is not above it.
        int fitCrackTotal = 0, fitCrackAttackers = 0;
        {
            const int fitPhase = (int) observer->getCurrentGamePhase();
            fitCrackTotal = crackBackTotalOver(opponent(), &fitCrackAttackers);
            if (!crackBackNextTurnDue(observer->currentPlayer == this, fitPhase,
                                      fitCrackAttackers, fitCrackTotal))
                fitCrackTotal = 0;
        }
        applyMenuFitTags(menu, rowUses, untappedSources, fitCrackTotal, fitCrackAttackers);
        //#W61-U (C10): and the board-sweep ranking, on the same menu copy and
        //for the same reason - a row cannot know its own number until the
        //suppression filter and any re-ask removal have settled.
        applyBoardSweepMark(menu, rowSweepTheirs, rowSweepMine, &rowSweepEngines,
                            &rowSweepMineNames, &rowSweepMineEngines); //#W80-DF (U7)
        //#W56-B (D15): and the same-card/same-verdict comparison, on the same
        //menu copy and for the same reason - a row cannot know its own number
        //until the suppression filter and any re-ask removal have settled.
        applyDuplicateEffectTags(menu, rowNames, rowCosts);
        //#W81-DM (V14): and the cover paragraph's per-prompt fold, on the same
        //settled menu copy - a row cannot know whether its cover duplicates
        //another until every row's cover has been built.
        w81FoldDuplicateCoverParagraphs(menu);
        //#W57-C (D12): the menu-level verdict, taken over the CAST rows only -
        //before the decline and hold rows join the list, since neither is a
        //cast and neither carries a board verdict.
        bool w81AnyTagDead = false; //#W81-DM (V13)
        const bool w81AllDead = everyCastRowDead(menu, &w81AnyTagDead);
        const string deadMenuNote = allCastRowsDeadNote(
            w81AllDead, (int) menu.size(),
            (int) LegalActionsOracle::legalLandPlays(this).size(),
            w81AnyTagDead); //#W68-BD (MED); #W81-DM (V13)
        //#W82-P9: the staged land rows, after the cast rows and before the
        //decline/hold rows. They are not cast candidates - `landRowFirst` says
        //where they start and `landRowLand`/`landRowBack` say what each plays.
        const int landRowFirst = (int) menu.size();
        vector<size_t> landRowLand;
        vector<bool> landRowBack;
        if (!mLandRowsForCast.empty())
        {
            vector<string> landRows;
            buildLandDropRows(mLandRowsForCast, landRows, landRowLand, landRowBack);
            for (size_t lr = 0; lr < landRows.size(); lr++)
                menu.push_back(landRows[lr]);
        }
        const int landRowCount = (int) menu.size() - landRowFirst;
        int declineRowIdx = -1; //#W66-AS (H7 second half): the decline's own row number
        //#W65-AL (G9, deck125 HIGH-1): and its PRICE, where it has one. The
        //cleanup step this clause prices is THIS turn's, so the clause is
        //stated only on the seat's own turn; on the opponent's turn the seat's
        //cleanup is a turn away and its hand is not this window's arithmetic.
        //The `{...}` group keeps it out of the option-set key below, like every
        //other priced fact.
        {
            string declineRow = castDeclineRow(observer->currentPlayer == this
                                               && observer->getCurrentGamePhase() == MTG_PHASE_FIRSTMAIN);
            if (landRowCount > 0) //#W82-P9: the decline covers the land rows on this menu too
                declineRow += " - and play no land in this window";
            int cuHand = -1, cuLimit = -1, cuPer = 0, cuStacked = 0;
            string cuPunishers;
            forcedCleanupInputs(NULL, this, opponent(), cuHand, cuLimit, cuPer,
                                cuPunishers, &cuStacked);
            declineRow += passRowCleanupPriceTag(observer->currentPlayer == this,
                                                 cuHand, cuLimit, cuPer, cuPunishers,
                                                 life, cuStacked);
            declineRow += ownClockTagFor(this, opponent(), life, //#W74-CE (O9) / #W74-CF (F7)
                                         (mIncomingCombatTurn == observer->turn)
                                             ? mIncomingCombatDamage : 0,
                                         pendingStackLifeLossToSeat(observer, this), //#W75-CL (P10)
                                         lifeLoopProvenWin(opponent()),
                                         statedCrackBackLossToSeat(this, opponent(),
                                                                   observer)); //#W76-CO (Q3 b)
            declineRowIdx = (int) menu.size();
            menu.push_back(declineRow); //the decline goes LAST among the cast rows
        }
        //#W53-N (D2, second half) + #W55-A (D2a/D19): the declined-list count,
        //keyed on the OPTION SET and taken BEFORE the HOLD row is appended (the
        //row is on every window now and renders this count). The key drops the
        //annotations that move without the question moving - the life-total
        //pricing inside a mandatory loop (130v126 seq 67-106) and the decline
        //row's Main-1 combat clause, which split the own-turn Main 1 / Main 2
        //pair into two lists and kept the count off it (162v126 s7-s12).
        if (mListDeclineTurn != observer->turn)
        {
            mListDeclineCount.clear();
            mListDeclineTurn = observer->turn;
        }
        const string listKey = optionSetKeyOf(menu);
        const int declinedN = mListDeclineCount.count(listKeyHash(listKey))
                              ? mListDeclineCount[listKeyHash(listKey)] : 0;
        mNextAskPromptNote = declinedListNote(declinedN);
        //#W75-CI (P1 a): the row-level half (#W74-CE O11) is REVERTED - see the
        //priority seam's note. The fact stays on the prompt-only note above the
        //list; nothing is removed from the menu.
        //#W53-N (D2): the HOLD row, last of all - 884 of the corpus's 966 dead
        //opponent-turn windows were THIS ask. The row is offered on the re-ask
        //menus too: a window the model is still being asked about is a window
        //it may still choose to close.
        //#W55-A (D4): and on the seat's OWN turn, for the reason the priority
        //seam carries - an own-turn run of byte-identical windows had no row to
        //close it. #W55-A (D21): the row states what taking it saves.
        //(history: comment-archaeology.md AIPlayerGPTSeams-w76ClosingRowInsertAt-2033)
        //(history: comment-archaeology.md AIPlayerGPTSeams-w76ClosingRowInsertAt-2049)
        const int holdRow = w76ClosingRowInsertAt(declineRowIdx, menu.size()); //#W76-CP (Q12)
        if (declineRowIdx >= 0)
        {
            menu.insert(menu.begin() + holdRow, holdRowLine(true)); //#W71-BR (L17)
            declineRowIdx++;
        }
        else
            menu.push_back(holdRowLine(true)); //#W71-BR (L17)
        //#W61-U (C14): the hold's own re-open rule for THIS menu, measured over
        //the rows the latch reads and BEFORE the latch consumes the window (a
        //held window still updates the memory). Only attempt 0 measures: a
        //re-ask menu is the same window with a row removed, not a new one.
        appendStackDeathToDeclineRows(menu,
                                      pendingStackLifeLossToSeat(observer, this),
                                      life); //#W68-BB (J9)
        appendPlainDeclineScope(menu, holdRow >= 0); //#W72-BW (M21)
        if (attempt == 0)
            mCastHoldNote = holdReopenNote("cast", menu);
        mNextAskPromptNote += mCastHoldNote;
        //#W78-CV (S6 + S4): the same two lines on the casting menu, same channel.
        string w79PendingCastLoopFace; //#W79-DC (F10): applied at the send, not here
        string w80PendingCastCrackBack, w80PendingCastStackDeath; //#W80-DE (U2)
        bool w80PendingCastDrain = false;                         //#W80-DE (U15)
        {
            string w78LoopSpell2, w78LoopPiece2;
            {
                //#W79-CZ (T4): same line, same counting rule, on the casting menu.
                const int lst2 = w77OwnLoopStackState(w78LoopSpell2, w78LoopPiece2);
                const string vline2 = w79OwnLoopVerdictLineFor(lst2, lifeLoopProvenWin(this),
                                                               w78LoopSpell2, w78LoopPiece2,
                                                               w80LiveLoopStarterName());
                if (!vline2.empty())
                {
                    mNextAskPromptNote += vline2;
                    //#W79-DC (F10): HELD, not stamped. Three gates below this point
                    //discard the window without sending anything (the honoured hold,
                    //the reservation decline and the loop auto-pass), and a counted
                    //line plus a stamped face for a prompt that was never sent is the
                    //wave-78 helper-versus-caller failure in its own shape. The face is
                    //applied at the askModel call, and a suppressed window clears it.
                    w79PendingCastLoopFace = w80LoopFaceOfLine(vline2); //#W80-DE (U10)
                }
            }
            //#W80-DE (U2): the same two verdict lines on the casting menu.
            {
                const string cb2 = w81CrackBackVerdictLineNow(); //#W81-DL (V5)
                if (!cb2.empty())
                {
                    mNextAskPromptNote += cb2;
                    //#W80-DH (F4): the face the LINE carries, not the latch's marker.
                    w80PendingCastCrackBack = w80CrackBackFaceOfLine(cb2)
                                              + w81CrackBackTotalFace(cb2); //#W81-DL (V5)
                    if (!w81CrackBackTotalFace(cb2).empty())
                        mW81PendingEventFace += "crackback_one_total_folded;";
                }
                const string sd2 = w80StackDeathVerdictLineNow();
                if (!sd2.empty())
                {
                    mNextAskPromptNote += sd2;
                    w80PendingCastStackDeath = stackDeathVerdictNow();
                }
            }
            const string drain2 = w78StackDrainNote(w78TheirDrainingTriggerCount(),
                                                    w78RowsUnchangedSinceLastAsk(mCastHoldNote),
                                                    w78HoldRowShortName(menu));
            if (!drain2.empty())
            {
                mNextAskPromptNote += drain2;
                if (attempt == 0)
                    w80PendingCastDrain = true; //#W80-DE (U15): counted at the send
            }
        }
        //#W66-AS (H3 second half): prompt-only, same channel, never in a key.
        bool loopNoteActingRows = false; //#W73-BY (N5): same rule as the priority seam
        bool loopNoteLifeRow = false;    //#W73-CB (F4): ...and the same per-row life test
        for (size_t lnr = 0; lnr < menu.size(); lnr++)
        {
            if (w72RowIsDeclineOrHold(holdKeyRow(menu[lnr])))
                continue;
            loopNoteActingRows = true;
            if (w73RowTouchesLife(menu[lnr]))
                loopNoteLifeRow = true;
        }
        mNextAskPromptNote += loopChainingNote(lifeLoopProvenWin(opponent()), holdRow >= 0,
                                               loopNoteActingRows, loopNoteLifeRow);
        //The model's own hold, honoured: no model call, no row withheld.
        if (attempt == 0 && holdHonoured("cast", menu))
        {
            w76HoldWindowNotAsked(mHoldMemory, "cast"); //#W76-CN (Q1)
            w77DropUnaskedCastNote(); //#W77-CR (R2 a)
            {
                bool w79c = false; //#W79-DC (F10): this window was never sent
                w79ApplyLoopFaceAtSend(false, w79PendingCastLoopFace,
                                       mOwnLoopVerdictFace, w79c);
                w80ApplyVerdictFacesAtSend(false, w80PendingCastCrackBack,
                                           w80PendingCastStackDeath, w80PendingCastDrain);
            }
            return NULL;
        }
        //#W67-AX (I7): and the reservation decline, on its own terms (the cast
        //set + the untapped-source count, inside this step).
        std::vector<string> castNames;
        for (size_t ci = 0; ci < candidates.size(); ci++)
            castNames.push_back((candidates[ci] ? candidates[ci]->name : string("?"))
                                + (ci < candidateUsesAlt.size() && candidateUsesAlt[ci]
                                   ? "|alt" : ""));
        const string castSetKey = castSetKeyOf(castNames);
        if (attempt == 0 && reserveDeclineHonoured(castSetKey, untappedSources, boardNow))
        {
            w76HoldWindowNotAsked(mHoldMemory, "cast"); //#W76-CN (Q1)
            w77DropUnaskedCastNote(); //#W77-CR (R2 a)
            {
                bool w79c = false; //#W79-DC (F10): this window was never sent
                w79ApplyLoopFaceAtSend(false, w79PendingCastLoopFace,
                                       mOwnLoopVerdictFace, w79c);
                w80ApplyVerdictFacesAtSend(false, w80PendingCastCrackBack,
                                           w80PendingCastStackDeath, w80PendingCastDrain);
            }
            return NULL;
        }
        //#W82-A (L11, audit-2026-09): the cast seam's loop/chain auto-pass is
        //DELETED. It is reached only with a NON-EMPTY cast menu (the
        //`candidates.empty() -> return NULL` above), and `loopAutoPassFor` passes
        //only when `hasAnyLegalAction(p, GptManaPolicy)` is false - so either the
        //two oracles agree and the call is dead, or they disagree and a RENDERED
        //cast row was auto-passed without the model ever seeing the window.
        //`LegalActions.cpp` prices with its own mana computation, so agreement is
        //not by construction: dead or a breach, and neither earns the call.
        //(`chain_windows_collapsed` 0 in the wave-80 corpus.) The PRIORITY seam's
        //call at the top of `chooseOrderedAction` is the legitimate one and stays.
        //#W72-BV (M9): the turn-scoped reserve carry, on the prompt-only channel
        //(the same one the declined count rides), computed after the cast-set
        //key it is measured on and before the ask is assembled.
        {
            if (mReserveDeclineSpanTurn != observer->turn)
            {
                mReserveDeclineSpanTurn = observer->turn;
                mReserveDeclineSpanKey.clear();
            }
            const bool carried = !mReserveDeclineSpanKey.empty()
                                 && mReserveDeclineSpanKey == castSetKey
                                 && menuHasReserveRow(menu);
            if (carried)
                mReserveDeclineNoted++;
            mNextAskPromptNote += reserveDeclineCarryNote(carried);
        }

        //#W75-CI (P12, wave-74 engine-seat MED-5): MEASURE ONLY - the O13 chain
        //census, at the seam that actually pays a chain. Same gate and same two
        //predicates as the priority arm (#W74-CD O13); this window is asked
        //exactly as before and no row is touched.
        if (observer->mLayers->stackLayer()->count(0, NOT_RESOLVED) > 0)
        {
            int actingCast = 0, emptyCast = 0;
            for (size_t s12 = 0; s12 < menu.size(); s12++)
            {
                if (w72RowIsDeclineOrHold(holdKeyRow(menu[s12])))
                    continue;
                actingCast++;
                if (w74RowIsDecisionEmpty(menu[s12]))
                    emptyCast++;
            }
            if (actingCast > 0)
            {
                mChainActingRowsCast += actingCast;
                mChainSelfharmRowsCast += emptyCast;
                if (emptyCast == actingCast)
                    mChainWindowsOnlySelfharmCast++;
            }
        }

        //#W81-DL (V8): the two row-marker events, staged off the MENU THAT IS
        //ABOUT TO BE SENT. Read from the rendered bytes rather than from the
        //emitter's own belief, so the counter and the literal are one figure by
        //construction (the wave-80 HIGH-4 boundary), and applied at the send.
        for (size_t w81r = 0; w81r < menu.size(); w81r++)
        {
            if (menu[w81r].find("[<- the only X that kills anything of THEIRS") != string::npos
                && mW81PendingEventFace.find("x_cast_row_refusal_marker") == string::npos)
                mW81PendingEventFace += "x_cast_row_refusal_marker;";
            if (w81XSweepMarkerNamesOwnLosses(menu[w81r])
                && mW81PendingEventFace.find("x_sweep_roster_marker") == string::npos)
                mW81PendingEventFace += "x_sweep_roster_marker;";
        }

        std::ostringstream q;
        q << "Casting decision (" << observer->getCurrentGamePhaseName()
          << (observer->currentPlayer == this ? ", YOUR turn" : ", opponent's turn")
          << "): which card do you cast now, if any?";
        q << deadMenuNote; //#W57-C (D12)
        q << kCastNoRowZeroFact; //#W66-AS (deck123 MED)
        if (landRowCount > 0) //#W82-P9
            q << " The \"Play <land>\" rows play that land instead of casting this window; you"
                 " have one land drop this turn, and this casting decision is asked again after"
                 " the land enters.";
        if (attempt > 0)
            q << "\n[RE-ASK " << attempt << "] The engine could not actually complete: "
              << rejectedSoFar << " - its cost or its targets cannot be satisfied right"
                 " now, so it has been REMOVED from the list below. Your priority window"
                 " is still open: decide again over what remains.";

        //no narration: a cast narrates itself as zone events, "nothing" is a non-action
        if (attempt == 0)
            mAskSituationPrefill = boardNow; //#W54-M (A19): the situation this call already rendered
        //#W82-A (L2): the legal-continuation digest staged here is DELETED - the
        //ask key is BOARD STATE + QUESTION, and the board carries every
        //continuation the digest was standing in for.
        //#W81-DK (V4): the census of the defect. This window's casting decision was
        //already answered THIS turn and phase, and the stack has moved under it - so
        //the stack term above has just re-opened a decision wave 80 would have served
        //from the cache. Counted once per window (`mWindowSeq` moves only when a
        //record is written) and stamped at the SEND, so the reason rides the record
        //of the prompt that was actually handed over.
        if (attempt == 0 && observer
            && mCastAskTurn == observer->turn
            && mCastAskPhase == (int) observer->getCurrentGamePhase()
            && !w81CastAnsweredStampMatches(mCastAskTurn, mCastAskPhase, mCastAskStack,
                                            observer->turn, (int) observer->getCurrentGamePhase(),
                                            w81StackStateStamp()))
        {
            if (mCastReopenCountedSeq != mWindowSeq)
            {
                mCastReopenCountedSeq = mWindowSeq;
                mCastDecisionReopenedNewStack++;
            }
            mNextSendCastReopen = "new_stack_object";
        }
        else
            mNextSendCastReopen.clear();
        //#W79-DC (F10): the prompt IS handed over on the next line, so this is where
        //the face is stamped and the line is counted -
        //`own_loop_verdict_lines_rendered` is by construction the number of prompts
        //the line is in.
        //#W80-DH (F10, Astra wave-80 review finding 10 - MED): the faces are STAGED
        //here and applied INSIDE askModel, at the point the prompt is actually
        //handed to the model. The caller asserting `sent=true` one line above the
        //call is not evidence from the send boundary: askModel's ask cache, its
        //repeat latch and its cross-phase replay each return an answer with no
        //round trip, so a rebuilt verdict-bearing casting menu with a cached
        //decline counted a rendered line for a prompt that never existed - the
        //wave-78 helper-versus-caller failure, one level up. `attempt` rides with
        //the staging because only the first attempt of a re-ask ladder may count.
        mNextSendLoopFace = (attempt == 0) ? w79PendingCastLoopFace : string();
        mNextSendCrackBack = w80PendingCastCrackBack;
        mNextSendStackDeath = w80PendingCastStackDeath;
        mNextSendDrain = w80PendingCastDrain;
        mCastMenuAsked = true; //#W82-P9: a menu reached the model (or is in flight)
        int pick = askModel(q.str(), menu, false);
        if (pick == kChoicePending)
            return NULL; //no cast this tick; the answer is consumed on a later poll
        if (landRowCount > 0 && pick >= landRowFirst && pick < landRowFirst + landRowCount)
        {
            //#W82-P9: a land row - play it; the casting decision stays OPEN and is
            //asked again on the changed board (nothing is marked answered here).
            const size_t lr = (size_t) (pick - landRowFirst);
            DebugTrace("AIPlayerGPT: land drop taken from the casting menu: "
                       << mLandRowsForCast[landRowLand[lr]].card->name);
            return commitLandDropPick(pMana, mLandRowsForCast[landRowLand[lr]].card, landRowBack[lr]);
        }
        //#W49-S (D8): the priority ask after this can say so.
        //#W72-BT (M13): set AFTER the window is answered, not before it is put.
        //#W72-BX (F3): and after VALIDATION, at the exits that actually close the
        //decision - a pick the engine then rejects re-renders this same window
        //with `attempt > 0`, and the stamp made that re-ask say the casting
        //decision had already been answered. `markCastDecisionAnswered` is called
        //on every closing exit below (decline, hold, a validated cast, and the
        //heuristic paths) and on none of the re-ask iterations.
        if (pick < 0) //model deferred or endpoint failed: heuristic decides
        {
            //#W67-AX (I7, engine MED-1): say what the heuristic did with the
            //window the model lost. 51 of the wave-66 corpus's 83 fallbacks were
            //unadjudicable for want of exactly this. Report-only; no key moves.
            MTGCardInstance * heur = AIPlayerBaka::FindCardToPlay(pMana, type);
            int heurRow = 0;
            if (heur)
                for (size_t ci = 0; ci < candidates.size(); ci++)
                    if (candidates[ci] == heur) { heurRow = (int) ci + 1; break; }
            noteHeuristicExecuted("cast", heurRow,
                                  heur ? ("cast " + heur->name) : string("cast nothing"));
            markCastDecisionAnswered(); //#W72-BX (F3)
            return heur;
        }
        //#W60-M (B13c): count a decline only when the MODEL was shown this
        //window. The sentence the count feeds is addressed to the model ("you
        //declined this exact list N times already this turn"), and after
        //#W59-J (K10) the ask seam answers some windows from its own latch or
        //its state+question cache without showing them - 125v126 turn 31
        //printed 23 in a turn with 9 asks, and 130v125 turn 67 printed 9 over 7
        //windows. Nothing is deleted: the engine still re-puts the question and
        //the model still sees every window it is actually asked; only the
        //number now describes what it says it describes.
        if (pick == declineRowIdx) //#W66-AS: "cast nothing", wherever it now sits
        {
            DebugTrace("AIPlayerGPT: chose to cast nothing");
            if (!mAskAnswerReserved)
                mListDeclineCount[listKeyHash(listKey)]++; //#W53-N (D2, second half)
            //#W67-AX (I7): a decline taken on a menu that carries a reserve row
            //is a decision about the whole step, not about this window.
            if (menuHasReserveRow(menu))
            {
                takeReserveDecline(castSetKey, untappedSources, boardNow);
                //#W72-BV (M9): and the turn-scoped half, which outlives the step.
                mReserveDeclineSpanTurn = observer->turn;
                mReserveDeclineSpanKey = castSetKey;
            }
            markCastDecisionAnswered(); //#W72-BX (F3)
            return NULL;
        }
        //#W53-N (D2): the model closed this turn's casting question itself.
        if (holdRow >= 0 && pick == holdRow)
        {
            takeHold("cast", menu);
            if (!mAskAnswerReserved)
                mListDeclineCount[listKeyHash(listKey)]++;
            markCastDecisionAnswered(); //#W72-BX (F3)
            return NULL;
        }

        //Validate and price the pick with the heuristic's own machinery: with
        //aiForcedCandidate set, AIPlayerBaka::FindCardToPlay examines only this
        //card, runs the full legality/restriction/target checks, and leaves
        //gotPayments / payAlternative set for exactly this play.
        MTGCardInstance * chosen = candidates[pick];
        aiForcedCandidate = chosen;
        aiForcedAlternative = candidateUsesAlt[pick];
        MTGCardInstance * validated = AIPlayerBaka::FindCardToPlay(pMana, "*");
        aiForcedCandidate = NULL;
        aiForcedAlternative = false;
        if (validated)
        {
            DebugTrace("AIPlayerGPT: casting " << validated->name << " (model's pick"
                       << (validated == chosen ? ")" : " via combo hint)"));
            mLastCastBoard = boardNow; //livelock breaker: next entry compares
            //#W54-C (D18): the livelock breaker keys on the row's identity
            //WITHOUT the menu-fit clause - that clause moves with the rest of
            //the menu, and a suppression key that drifted with it would never
            //match the next tick's rebuild.
            mLastCastLine = (pick >= 0 && pick < (int) opts.size()) ? opts[pick] : menu[pick];
            //#W71-BQ (L5): the model has now COMMITTED to casting this card.
            //Recorded on the card the engine validated (not the row text), so
            //the cast-mode menu one window later can say whose answer it is
            //completing. Stamped with the turn - see the header.
            mCommittedCastName = validated->getDisplayName();
            mCommittedCastTurn = observer->turn;
            mPlanCastOpenName = mCommittedCastName; //#W80-DH (F5): whose cast is open
            mPlanCastOpenTurn = observer->turn;
            //#W80-DG (U1): the cast step is now OPEN. The next window decides
            //whether it was one window or two (see w80CompletingCastMenu).
            mPlanCastCompletionState = 1;
            markCastDecisionAnswered(); //#W72-BX (F3)
            return validated;
        }

        //The cheap menu filter let through something the real machinery rejects
        //(cast restriction, no legal target, unpayable kicker, X-slack 0...).
        gotPayments.clear();
        rejectedSoFar += (rejectedSoFar.empty() ? "" : ", ") + chosen->getDisplayName();
        bool lastChance = (attempt >= kMaxCastReasks) || (candidates.size() <= 1);
        DebugTrace("AIPlayerGPT: model chose " << chosen->name
                   << " but it fails validation; "
                   << (lastChance ? "re-ask budget spent, deferring to heuristic"
                                  : "removing it and re-asking"));
        //Marked so a corpus reviewer can find every re-ask and every
        //exhaustion by fallback reason alone (the old single
        //"deferred_to_heuristic" recorded both the reject and the give-up).
        writeTransLog("defer", "", "", -1, (int) menu.size(), chosen->name,
                      lastChance ? "validation_reject_reask_exhausted"
                                 : "validation_reject_reask");
        //#W82-A (L1): the conceded row is NOT retired for the turn - it is
        //removed from THIS window's menu (the erase below) exactly as an
        //intermediate reject is, and offered again at the next window. What
        //stops the re-poll churn the old retirement was written for is the
        //window-level marker: this exact board has now burned its casting
        //window, so the next entry over it defers once instead of replaying.
        if (lastChance)
            mCastNoProgressBoard = boardNow;
        candidates.erase(candidates.begin() + pick);
        candidateUsesAlt.erase(candidateUsesAlt.begin() + pick);
        opts.erase(opts.begin() + pick);
        if (pick < (int) rowUses.size())
            rowUses.erase(rowUses.begin() + pick); //#W54-C (D18): stay index-parallel
        if (pick < (int) rowSweepTheirs.size()) //#W61-U (C10): same
        {
            rowSweepTheirs.erase(rowSweepTheirs.begin() + pick);
            rowSweepMine.erase(rowSweepMine.begin() + pick);
            if (pick < (int) rowSweepEngines.size()) //#W66-AQ (H10): stay parallel
                rowSweepEngines.erase(rowSweepEngines.begin() + pick);
            if (pick < (int) rowSweepMineNames.size()) //#W80-DF (U7): stay parallel
                rowSweepMineNames.erase(rowSweepMineNames.begin() + pick);
            if (pick < (int) rowSweepMineEngines.size())
                rowSweepMineEngines.erase(rowSweepMineEngines.begin() + pick);
        }
        if (pick < (int) rowNames.size()) //#W56-B (D15): same, for the duplicate pass
            rowNames.erase(rowNames.begin() + pick);
        if (pick < (int) rowCosts.size())
            rowCosts.erase(rowCosts.begin() + pick);
        if (lastChance)
        {
            noticeFallback("that cast could not be completed - the heuristic decides", 5.0f);
            markCastDecisionAnswered(); //#W72-BX (F3)
            return AIPlayerBaka::FindCardToPlay(pMana, type);
        }
        setNotice("that cast could not be completed - asking again", 5.0f);
    }
}


//#W82-P10: the WHOLE damage order in one answer. `ORDER: B2, B1, B3` names every
//blocker of one attacker in the order damage is dealt. A line is usable only
//when it is a full permutation of 1..n (each B# once, none missing, none out of
//range); the first clean usable ORDER: line wins (gptSelectAnswerIndex, the
//rule the other seams share), and a label-less line that is nothing but the
//permutation is read after the PLAN (the robust-parser ruling). Anything
//partial or malformed returns an empty order and the seam falls back to the
//per-pick asks - no blocker and no ordering is ever lost to the parse. Pure.
static void collectLabeledLines(const string& content, const char * label, vector<string>& out,
                                vector<string> * prevOut, vector<vector<string> > * windowOut);

static bool gptParseOrderPermutation(const string& line, size_t n, vector<int>& order)
{
    order.clear();
    vector<bool> seen(n + 1, false);
    size_t i = 0;
    while (i < line.size())
    {
        char c = line[i];
        if (c == 'B' || c == 'b')
        {
            i++;
            continue;
        }
        if (isdigit((unsigned char) c))
        {
            int v = 0;
            while (i < line.size() && isdigit((unsigned char) line[i]))
                v = v * 10 + (line[i++] - '0');
            if (v < 1 || (size_t) v > n || seen[(size_t) v])
            {
                order.clear();
                return false;
            }
            seen[(size_t) v] = true;
            order.push_back(v);
            continue;
        }
        if (c == ',' || c == ' ' || c == '\t' || c == ';' || c == '.' || c == '\r')
        {
            i++;
            continue;
        }
        order.clear();
        return false; //a word or another token: not a bare permutation line
    }
    if (order.size() != n)
    {
        order.clear();
        return false;
    }
    return true;
}

static bool gptOrderLineFromReply(const string& content, size_t n, vector<int>& order,
                                  string * takenText)
{
    order.clear();
    if (takenText)
        takenText->clear();
    if (content.empty() || n < 2)
        return false;
    string stripped = content;
    const size_t te = stripped.rfind("</think>");
    if (te != string::npos)
        stripped = stripped.substr(te + 8);
    vector<string> lines;
    collectLabeledLines(stripped, "ORDER:", lines, NULL, NULL);
    vector<bool> usable(lines.size(), false), clean(lines.size(), false);
    vector<vector<int> > perms(lines.size());
    for (size_t i = 0; i < lines.size(); i++)
    {
        usable[i] = gptParseOrderPermutation(lines[i], n, perms[i]);
        clean[i] = usable[i];
    }
    const int idx = lines.empty() ? -1 : gptSelectAnswerIndex(usable, clean);
    if (idx >= 0)
    {
        order = perms[(size_t) idx];
        if (takenText)
            *takenText = lines[(size_t) idx];
        return true;
    }
    //label-less: the line after the PLAN that is nothing but the permutation
    size_t at = 0;
    bool pastPlan = false;
    while (at < stripped.size())
    {
        const size_t nl = stripped.find('\n', at);
        string line = stripped.substr(at, nl == string::npos ? string::npos : nl - at);
        at = (nl == string::npos) ? stripped.size() : nl + 1;
        size_t b = 0;
        while (b < line.size() && (line[b] == ' ' || line[b] == '\t'))
            b++;
        line = line.substr(b);
        if (line.empty())
            continue;
        if (line.compare(0, 5, "PLAN:") == 0 || line.compare(0, 5, "plan:") == 0)
        {
            pastPlan = true;
            continue;
        }
        if (!pastPlan)
            continue; //an answer BEFORE the plan is not read
        vector<int> perm;
        if (gptParseOrderPermutation(line, n, perm))
        {
            order = perm;
            if (takenText)
                *takenText = line;
            return true;
        }
    }
    return false;
}


int AIPlayerGPT::orderBlockers()
{
    if (mEndpoint.empty())
        return AIPlayerBaka::orderBlockers();
    if (!(ORDER == observer->combatStep && observer->currentPlayer == this))
        return 0;
    if (mOrderDoneTurn != observer->turn)
    {
        mOrderDoneTurn = observer->turn;
        mOrderDone.clear();
        mOrderPerPick.clear();
    }

    //Damage is assigned lethal-first down each attacker's blockers vector
    //(GuiCombat::autoaffectDamage), so ordering damage = permuting that
    //vector before assignment. Sequential single picks, like the target
    //seam: each round asks which blocker is dealt damage next. Every ask is
    //prompt-cached, so the re-polling while a call is in flight replays the
    //already-decided prefix for free.
    GuiCombat * gc = observer->mLayers->combatLayer();
    for (size_t a = 0; a < gc->attackers.size(); a++)
    {
        AttackerDamaged * atk = gc->attackers[a];
        if (!atk->card || atk->card->controller() != this || atk->blockers.size() < 2)
            continue;
        if (mOrderDone.count(atk->card))
            continue; //ordered earlier this combat (the per-attacker asks re-enter each tick)

        //#W82-P10: ONE ask for the whole permutation. Falls through to the
        //per-pick asks below on a partial or malformed answer.
        if (!mOrderPerPick.count(atk->card))
        {
            const size_t n = atk->blockers.size();
            std::ostringstream tail;
            tail << "Combat damage order: your attacker " << atk->card->getDisplayName()
                 << " (" << atk->card->power << "/" << atk->card->toughness << ")"
                 << (atk->card->has(Constants::TRAMPLE) ? " with trample" : "")
                 << (atk->card->has(Constants::DEATHTOUCH) ? " with deathtouch" : "")
                 << " is blocked by " << n << " creatures. Damage is assigned in order, up to"
                    " each blocker's toughness. Its blockers:\n";
            vector<string> shownLines;
            for (size_t b = 0; b < n; b++)
            {
                std::ostringstream o;
                o << "B" << (b + 1) << ". " << atk->blockers[b]->card->getDisplayName()
                  << " (" << atk->blockers[b]->card->power << "/"
                  << atk->blockers[b]->card->toughness << ")";
                const string kw = keywordList(atk->blockers[b]->card);
                if (!kw.empty())
                    o << " [" << kw << "]";
                shownLines.push_back(o.str());
                tail << o.str() << "\n";
            }
            tail << kPlanFirstLead
                 << "on a line of its own ORDER: followed by ALL " << n << " of those B# labels"
                    " in the order damage is dealt, first to last, comma-separated (e.g."
                    " \"ORDER: B2, B1, B3\"). Write nothing else.";
            mLogWindowKind = kAskWindowCombat;
            const string userMsg = assemblePrompt(tail.str());
            string content;
            setAnswerFloorForSeam("order", (long) n, kPutSlotAnswerBytes);
            if (pollCompletionRetry(userMsg, content, "order") == kChoicePending)
                return 1; //in flight; stay in the ORDER step and re-poll next tick
            vector<int> order;
            string takenText;
            if (gptOrderLineFromReply(content, n, order, &takenText))
            {
                vector<DefenserDamaged *> ordered;
                string names;
                for (size_t k = 0; k < order.size(); k++)
                {
                    ordered.push_back(atk->blockers[(size_t) order[k] - 1]);
                    names += (k ? ", " : "") + ordered.back()->card->getDisplayName();
                }
                atk->blockers = ordered;
                mOrderDone.insert(atk->card);
                writeTransLog("order", userMsg, content, (int) order.size(), (int) n,
                              names, NULL, &shownLines);
                narrateDecision("You ordered " + atk->card->getDisplayName()
                                + "'s combat damage: " + names);
                DebugTrace("AIPlayerGPT: whole damage order for " << atk->card->getDisplayName()
                           << ": " << names);
                continue;
            }
            //partial or malformed: recorded, then the per-pick asks take over
            writeTransLog("order", userMsg, content, -1, (int) n, "",
                          content.empty() ? noAnswerClass() : "order_incomplete_per_pick",
                          &shownLines);
            mOrderPerPick.insert(atk->card);
            setNotice("damage order reply incomplete - asking one position at a time", 5.0f);
        }

        vector<DefenserDamaged *> ordered;
        vector<DefenserDamaged *> remaining = atk->blockers;
        bool deferred = false;
        while (remaining.size() > 1)
        {
            std::ostringstream q;
            q << "Combat damage order: your attacker " << atk->card->getDisplayName()
              << " (" << atk->card->power << "/" << atk->card->toughness << ")"
              << (atk->card->has(Constants::TRAMPLE) ? " with trample" : "")
              << (atk->card->has(Constants::DEATHTOUCH) ? " with deathtouch" : "")
              << " is blocked by " << atk->blockers.size()
              << " creatures. Damage is assigned in order, up to each blocker's toughness. "
              << "Choose the blocker dealt damage in position " << (ordered.size() + 1) << ".";
            vector<string> opts;
            for (size_t b = 0; b < remaining.size(); b++)
            {
                std::ostringstream o;
                o << remaining[b]->card->getDisplayName()
                  << " (" << remaining[b]->card->power << "/" << remaining[b]->card->toughness << ")";
                string kw = keywordList(remaining[b]->card);
                if (!kw.empty())
                    o << " [" << kw << "]";
                opts.push_back(o.str());
            }
            {
                vector<string> narr;
                for (size_t b = 0; b < opts.size(); b++)
                {
                    std::ostringstream n;
                    n << "You assigned " << atk->card->getDisplayName() << "'s combat damage to "
                      << stripNarrationDecoration(opts[b]) << " in position " << (ordered.size() + 1);
                    narr.push_back(n.str());
                }
                setAskNarration(narr);
            }
            int pick = askModel(q.str(), opts);
            if (pick == kChoicePending)
                return 1; //stay in the ORDER step; re-poll next tick
            if (pick < 0)
            {
                deferred = true; //model did not decide
                break;
            }
            ordered.push_back(remaining[pick]);
            remaining.erase(remaining.begin() + pick);
        }
        if (deferred)
        {
            //Fall back to the heuristic ranking, not to declaration order:
            //declaration order is the DEFENDER's choice, and keeping it
            //hands the attacker's CR 510.1a decision to the opponent.
            heuristicDamageOrder(atk);
        }
        else if (ordered.size())
        {
            ordered.push_back(remaining[0]);
            atk->blockers = ordered;
            DebugTrace("AIPlayerGPT: damage order for " << atk->card->getDisplayName()
                       << " set, first: " << ordered[0]->card->getDisplayName());
        }
        mOrderDone.insert(atk->card); //#W82-P10: whichever path decided it
    }

    observer->userRequestNextGamePhase();
    return 1;
}


int AIPlayerGPT::computeActions()
{
    //Menus must be intercepted here: the base loop reacts to
    //selectMenuOption's return in the same tick, and its contract has no
    //"not yet" value. Since c3 the menu rides the DecisionRequest contract:
    //the manager snapshots the options, the model (or the heuristic
    //fallback) picks in option space, and the manager applies the clicks.
    //While the model call is in flight we do nothing and re-poll.
    if (!mEndpoint.empty() && observer->currentlyActing() == this)
    {
        ActionLayer * object = observer->mLayers->actionLayer();
        if (object->menuObject)
        {
            DecisionRequest req;
            if (DecisionManager::buildMenuChoice(this, req))
            {
                DecisionAction act;
                if (chooseMenuAction(req, act) == kChoicePending)
                    return 1;
                DecisionManager::applyMenuChoice(req, act);
                return 1;
            }
            //no answerable shape (e.g. X no longer affordable): the
            //heuristic click path, exactly as before the contract.
            //#W41-1's standing requirement is that this path is never SILENT -
            //if the pilot structurally cannot be asked, the engine says which
            //option it took for it and on which menu, so a reviewer reading
            //the stderr can tell "not asked" from "asked and answered".
            int doThis = AIPlayerBaka::selectMenuOption();
            //#W78-CX (S10): the seat the deferred menu BELONGS to, and the rows
            //it was never shown. `currentlyActing()` is this seat (the guard
            //above), but an instant cast by the opponent arms its menu here -
            //the Starstorm X-announcement of `152v130` seq 21 - and the record
            //must land in that seat's file or the corpus's per-seat census is
            //off by one in BOTH directions.
            AIPlayerGPT * deferSeat = this;
            {
                Player * menuController = object->currentActionCard
                                          ? object->currentActionCard->controller() : NULL;
                AIPlayerGPT * asGpt = dynamic_cast<AIPlayerGPT *>(menuController);
                if (w78DeferRecordMoves(menuController != NULL, menuController == this,
                                        asGpt != NULL,
                                        asGpt && !asGpt->mTransLogPath.empty()))
                    deferSeat = asGpt;
            }
            std::vector<string> deferRowsRaw;
            if (object->abilitiesMenu)
            {
                for (size_t mi = 0; mi < object->abilitiesMenu->mObjects.size(); mi++)
                {
                    SimpleButton * b = dynamic_cast<SimpleButton *>(object->abilitiesMenu->mObjects[mi]);
                    deferRowsRaw.push_back(b ? b->getText() : string());
                }
            }
            const std::vector<string> deferRows = w78DeferOptionTexts(deferRowsRaw);
            //#W54-M (A33): the release channel for "not asked" is the translog
            //(the corpus reviewer's surface - a `defer` record with its own
            //class); the stderr line is a development diagnostic and compiles
            //out of release builds (owner rule: compile diagnostics out).
            {
                string menuCard = object->currentActionCard ? object->currentActionCard->getDisplayName()
                                                            : string("(unnamed)");
                std::ostringstream why;
                why << "menu on " << menuCard << " could not be put to the model (no answerable"
                       " shape - e.g. the announced X is no longer affordable); the heuristic"
                       " took option " << doThis;
                deferSeat->writeTransLog("defer", "", why.str(), doThis, (int) deferRows.size(),
                                         menuCard, "menu_not_askable", &deferRows);
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
                fprintf(stderr, "AIPlayerGPT: %s\n", why.str().c_str());
#endif
            }
            if (doThis >= 0)
            {
                if (object->abilitiesMenu->isMultipleChoice)
                    observer->mLayers->actionLayer()->ButtonPressedOnMultipleChoice(doThis);
                else
                    observer->mLayers->actionLayer()->doReactTo(doThis);
            }
            else
                observer->mLayers->actionLayer()->doReactTo(object->abilitiesMenu->mObjects.size() - 1);
            return 1;
        }
    }

    //The mulligan is now a PRE-GAME PHASE (PreGamePhase, before turn 1), not an
    //in-game window here: the old turn-0/turn-1 mulligan hack was removed with
    //the London pre-game phase. This player's opening-hand decisions arrive
    //through the pregameMulliganDecision / pregameChooseBottom / pregameLeyline
    //hooks below, driven by PreGamePhase while GameObserver::Update is gated.
    return AIPlayerBaka::computeActions();
}


//The base Act, with the async insertions. The base is monolithic: after
//computeActions it interprets an empty clickstream as "nothing to do" and
//COMMITS a pass (cancelInterruptOffer / userRequestNextGamePhase). While a
//model call is in flight the AI must do neither - not act, not pass - so
//the game loop keeps running (and rendering) until the answer lands and the
//re-polled flow consumes it.
bool AIPlayerGPT::decisionPending(float dt)
{
    //c5d: the async gate is a policy hook now - the base AIPlayerBaka::Act
    //consults it at entry and again after computeActions, so the mirrored
    //Act body this class used to carry is gone.
    //#W73-CB (F2): this hook runs on every tick this seat acts, so the hold
    //dies at its untap even in a turn where no menu ever reaches holdHonoured.
    releaseHoldIfUntapPassed();
    if (mEndpoint.empty())
        return false;
    //#W82-A (L3, audit-2026-09): THE IN-FLIGHT WATCHDOG RUNS HERE, on the gate
    //that IS reached every tick. `reapWedgedRequests` had exactly one caller -
    //`pollCompletion`, which is reached only through the decision seams - and
    //`AIPlayerBaka::Act` returns at its first statement (AIPlayerBaka.cpp:5878)
    //whenever `decisionPending(dt)` is true, which this function returns for as
    //long as `asyncBusy()`. So for a worker that never publishes, `status` stayed
    //1, no seam was ever re-entered, nothing ever polled, and the deadline was
    //never read: the bound "was born unreachable" (the entry gate is b192afd1c,
    //2026-07-12; the bound is b9feba4e0, 2026-09-03). Calling it from the hook
    //the base AI consults on every tick is the whole fix - the reap itself is
    //unchanged, and it bumps the generation rather than freeing anything, so a
    //worker that wakes up later still finds a safe slot.
    if (asyncBusy())
        reapWedgedRequests(std::shared_ptr<AsyncState>(), NULL);
    if (asyncBusy())
    {
        mThinkTime += dt;
        //Keep a pending interrupt offer from timing out while the model is
        //still deciding whether to respond.
        observer->mLayers->stackLayer()->extendInterruptOffer(this);
        return true;
    }
    mThinkTime = 0;
    return false;
}


//#W74-CD (O2, wave-73 deck152 HIGH-1): a decision whose SECOND LEG is armed and
//not yet launched. The three arms that schedule one (the two-phase forced close,
//the transport/deadline retry, the answer-ceiling re-ask) all CONSUME the first
//reply before they arm, so the async slot is empty and decisionPending is false
//for exactly the tick between arming and the next poll - and the pass at the
//bottom of AIPlayerBaka::Act fired in that window, closing the step the decision
//belonged to. Bounded, because an armed leg that is never re-entered (its seam
//no longer reached, the window gone) must not hold the phase for ever: after
//kRetryArmedHoldMs the pass goes through exactly as before, so nothing is
//removed, only deferred. Time rather than tick count so the pass gate and the
//[combatentry] trace can both ask in one tick without the reads disagreeing.
bool AIPlayerGPT::decisionArmed()
{
    //#W74-CF (F1): the storage is per arm now, so an armed leg may be sitting in
    //the park while the other arm runs. It is still an armed leg and the pass at
    //the bottom of Act must still wait for it.
    const string& armedPrompt = !mRetryActivePrompt.empty() ? mRetryActivePrompt
                                                            : mRetryPark.activePrompt;
    if (mEndpoint.empty())
    {
        mRetryArmedSeen.clear();
        return false;
    }
    //#W74-CG: the decision itself is the pure helper above, so PARSETEST can pin
    //the parked-leg release without a live seat.
    const long nowMs = (long) std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
    long sinceMs = (long) std::chrono::duration_cast<std::chrono::milliseconds>(
        mRetryArmedSince.time_since_epoch()).count();
    const bool held = w74RetryArmedHold(armedPrompt, mRetryArmedSeen, sinceMs, nowMs,
                                        (long) kRetryArmedHoldMs);
    mRetryArmedSince = std::chrono::steady_clock::time_point(
        std::chrono::milliseconds(sinceMs));
    return held;
}


bool AIPlayerGPT::attackDeclarationAnswered()
{
    return mAttacksDoneTurn == observer->turn;
}


//A call still in flight after the patience window is one a person has been
//watching with no way to act. The duel screen asks rather than deciding for
//them. No re-entry guard is needed: raising the prompt moves the duel into
//its menu phase, which stops calling game->Update, so mThinkTime freezes
//until the answer comes back and resets it.
bool AIPlayerGPT::aiPatiencePromptDue()
{
    if (mEndpoint.empty() || mPatienceLimit <= 0)
        return false;
    return asyncBusy() && mThinkTime >= mPatienceLimit;
}


void AIPlayerGPT::aiPatiencePromptAnswer(bool keepWaiting)
{
    //Re-arm rather than commit: waiting buys one more window, not an
    //unbounded one, so the choice comes back if the model is truly wedged.
    mThinkTime = 0;
    if (keepWaiting)
        return;
    //Switch off for the rest of the duel. Clearing the endpoint is the same
    //state a player with no endpoint configured is in: decisionPending goes
    //false, all seams fall through to AIPlayerBaka, and isInteractiveAI goes
    //false so card-data "ishuman" gates hand the heuristic AI its dice-roll
    //lines back. The in-flight worker is left alone - AsyncState is a
    //shared_ptr, so the detached thread stays safe and its answer is simply
    //never read.
    mEndpoint.clear();
    setNotice("LLM opponent off - the built-in AI is playing", 6.0f);
}


void AIPlayerGPT::Render()
{
    AIPlayerBaka::Render();
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::MAIN_FONT);
    if (!font)
        return;
    //Transient notices: endpoint unreachable at duel start, a reply that
    //failed or timed out. Frame-decayed so no game-thread timer is needed.
    if (mNoticeTicks > 0)
    {
        mNoticeTicks--;
        font->SetColor(ARGB(230, 255, 190, 120));
        font->DrawString(mNotice.c_str(), SCREEN_WIDTH / 2, 12, JGETEXT_CENTER);
    }
    //A standing marker, because the notice above is one line for five seconds
    //at the top of the screen while the player is looking at their hand at the
    //bottom of it - faithfully emitted and easily never seen. Two distinct
    //facts, never both: the LLM is OFF for the rest of this duel (switched off,
    //or no endpoint was ever reachable), or it is answering but has been
    //falling back. The second lapses on its own, so a model that recovers
    //stops being accused; a marker that latched forever would become wallpaper
    //and put us right back at effectively-silent failure.
    if (mDegradedTicks > 0)
        mDegradedTicks--;
    if (mEndpoint.empty())
    {
        font->SetColor(ARGB(200, 255, 150, 150));
        font->DrawString("LLM off - built-in AI", 4, SCREEN_HEIGHT - 12, JGETEXT_LEFT);
    }
    else if (mDegradedTicks > 0)
    {
        char dbuf[48];
        sprintf(dbuf, "LLM: %d fallback%s", mFallbackCount, mFallbackCount == 1 ? "" : "s");
        font->SetColor(ARGB(190, 255, 200, 120));
        font->DrawString(dbuf, 4, SCREEN_HEIGHT - 12, JGETEXT_LEFT);
    }

    //The visible answer to "is it frozen or thinking?": a small animated
    //line whenever this player's model call is in flight.
    if (mEndpoint.empty() || !asyncBusy())
        return;
    char buf[48];
    int dots = 1 + ((int) (mThinkTime * 2)) % 3;
    sprintf(buf, "opponent is thinking%.*s", dots, "...");
    font->SetColor(ARGB(220, 255, 255, 200));
    font->DrawString(buf, SCREEN_WIDTH / 2, 2, JGETEXT_CENTER);
}


//The ANNOUNCE_X ask header. States the mana CAP and its reason so the model
//does not reason about an X its mana cannot reach (wave-23 deck140 vs27 s46:
//the model computed X=20 "lethal" while the engine offered at most X=6). capX =
//the highest AFFORDABLE X = the last option's X value (option index == X value,
//bounded upstream by ManaEngine::maxAnnounceableX). Factored out so the PARSETEST
//covers the exact production string.
//(history: comment-archaeology.md AIPlayerGPTSeams-xAnnounceLibraryNote-2065)
//(history: comment-archaeology.md AIPlayerGPTSeams-xAnnounceLibraryNote-2083)
string xAnnounceLibraryNote(int capX, int drawPerX, int library, int reserve,
                                   const string& reserveWhy, int libX)
{
    if (drawPerX <= 0 || library < 0 || capX < 1 || libX >= capX)
        return "";
    std::ostringstream o;
    o << " YOUR LIBRARY IS THE SECOND CEILING on this menu: it holds " << library
      << " card" << (library == 1 ? "" : "s") << " and every point of X draws "
      << drawPerX << ", so X=" << capX << " draws " << (capX * drawPerX)
      << " of them. " << reserve << " draw" << (reserve == 1 ? " is" : "s are")
      << " already owed that you cannot decline (" << reserveWhy << "), and the"
         " draw you cannot make is the game. ";
    if (libX >= 1)
        o << "X=" << libX << " is the largest value on this menu the library pays for.";
    else
        o << "NO value on this menu above 0 leaves the library able to pay them.";
    return o.str();
}


//#W74-CF (O19 verification gap, Astra shipping note). The wave-74 O19 pins are
//hand-built permutations beside the seam, not the seam's own code: nothing in
//PARSETEST ran a menu through RENDER -> ANSWER -> APPLY in the new order, so a
//display permutation and a map-back that disagreed would both have passed. These
//two functions ARE the seam's code, lifted out unchanged, so the round trip is
//provable: permute the contract-space rows into display order, then map a picked
//display row back to the contract's index==X space.
static bool w74XPermuteToClimbing(vector<string>& shown, int capX)
{
    if (capX < 1 || shown.size() != (size_t)(capX + 1))
        return false;
    vector<string> disp;
    disp.reserve(shown.size());
    for (int xv = 1; xv <= capX; xv++)
        disp.push_back(shown[(size_t)(capX - xv)]);
    disp.push_back(shown[(size_t) capX]); //X = 0
    shown.swap(disp);
    return true;
}


static int w74XPickToContractIndex(int pick, int capX, int rowCount, bool climbing,
                                   int optionCount)
{
    if (pick < 0 || pick >= rowCount)
        return pick;
    return climbing ? ((pick < capX) ? pick + 1 : 0) : (optionCount - 1 - pick);
}


static string announceXHeader(const string& spell, int capX, bool canDecline = true,
                              const string& libraryNote = "") //#W67-AW (I4)
{
    std::ostringstream xa;
    xa << "Announce the value of X for " << spell
       << ". You can afford X up to " << capX << " with your current mana"
       << " - higher values are NOT offered (they are unaffordable), so do not"
       << " plan around an X above " << capX << ".";
    //#W74-CC (O19, wave-73 engine-seat MED-3): the ladder now CLIMBS, and the
    //option number IS the value. The descending menu was the corpus's only
    //index/name conflict class two waves running (`125v126` seq 100 answered
    //"CHOICE: 5 (X = 5)" on a list whose option 5 was X = 3; `125v123` seq 624
    //the same shape - both rescued by the name match, both a wasted seam). The
    //wave-23 reason for descending was that the model replies its intended
    //VALUE as the option number; with X = N at option N that slip is no longer
    //a slip, which is strictly better than either previous order. X = 0 is the
    //ladder's last rung rather than its first, so the numbering can hold.
    if (capX >= 1)
        xa << " Every listed value is affordable, and THE OPTION NUMBER IS THE X:"
              " option N announces X = N, up to option " << capX << " (X = " << capX
           << "), the largest X you can afford. X = 0 is the last rung, at option "
           << (capX + 1) << ".";
    else
        xa << " The only value your mana affords is X = 0, at option 1.";
    if (!canDecline)
        xa << " There is no decline row on this menu: this spell's costs are"
              " ALREADY PAID, so the announcement can no longer be cancelled and"
              " the card cannot go back to your hand from here. Every listed"
              " value spends it; X = 0 is the smallest commitment, not a way out.";
    xa << libraryNote; //#W67-AW (I4)
    xa << " Reply with the OPTION number:"; //#W74-CC (O19): they are now the same number
    return xa.str();
}


//Annotate a shockland-style ETB "pay life or enter tapped" menu in place. The
//engine offers only the bare labels "Pay N life" and "Tap", neither of which
//states the CONSEQUENCE the player is trading on: paying the life lets the
//permanent enter UNTAPPED (usable this turn), while "Tap" declines the payment
//so it instead enters TAPPED. The model saw only the labels and could not weigh
//tempo against life (deck137 wave-24). Append-only - labels and answer indices
//are untouched. Mechanism-matched by the CO-PRESENCE of a "Pay ... life" option
//and a "Tap" option (the pay-or-tapped ETB shape, the only menu that pairs
//them), NOT by card name. Returns true when it fired (for the self-test).
//Case-insensitive helpers: the ACTUAL rendered shock menu options are
//LOWERCASE ("pay 2 life" / "tap", confirmed in the real translog - deck137
//vs109 s4, vs59 s3), while the auto-script name() reads "Pay 2 life"/"Tap".
//The original capital-only matcher therefore never fired on the live menu and
//the options stayed bare (the b4 annotation reached the card TEXT, not the
//menu). Match both forms.
string toLowerCopy(const string & s)
{
    string r = s;
    for (size_t i = 0; i < r.size(); i++)
        r[i] = (char) tolower((unsigned char) r[i]);
    return r;
}

static bool isPayLifeOption(const string & opt)
{
    string lo = toLowerCopy(opt);
    return lo.compare(0, 4, "pay ") == 0 && lo.find(" life") != string::npos;
}

static bool isTapOption(const string & opt)
{
    //the bare tap branch is exactly "tap" (any case); guard against matching a
    //longer option that merely starts with "tap ..."
    return toLowerCopy(opt) == "tap";
}

//#W58-B (D1, wave-57 ledger HIGH): the pay-life row is the only menu row in
//the game that can end the game on the spot, and it was the only row with no
//number. `146v130` seq 20: a seat at 1 life took `pay 3 life - Emeria,
//Shattered Skyclave enters UNTAPPED [usable ...]` and the next record in the
//file is `gameend` at -2. The arrival clause prices USABILITY; nothing priced
//the LIFE. Every other menu in this engine prints its subtraction finished
//(`they would be at K`, `this KILLS you`), and this one is the one that loses
//games. The amount is on the row's own label, so the verdict is pure over
//(life, cost) and rides EVERY life-payment row, not just the shockland shape.
//The row is never removed - a legal choice stands, badged.
static bool payLifeAmountFromOption(const string & opt, int & amount)
{
    if (!isPayLifeOption(opt))
        return false;
    string lo = toLowerCopy(opt);
    size_t p = 4; //past "pay "
    while (p < lo.size() && isspace((unsigned char) lo[p]))
        p++;
    const size_t s = p;
    int v = 0;
    while (p < lo.size() && isdigit((unsigned char) lo[p]))
    {
        v = v * 10 + (lo[p] - '0');
        p++;
        if (v > 999999)
            return false;
    }
    if (p == s)
        return false; //"pay half your life" and friends: no computable amount
    while (p < lo.size() && isspace((unsigned char) lo[p]))
        p++;
    //the digits must be the amount of the LIFE clause itself - "pay 2 mana ...
    //life" is a different payment and is left alone.
    if (lo.compare(p, 4, "life") != 0)
        return false;
    amount = v;
    return v > 0;
}

//The verdict, in the same finished-subtraction shape the cast rows carry. A
//payment that reaches 0 or less is badged as lethal; the row still stands.
static string lifePaymentVerdict(int life, int cost)
{
    std::ostringstream o;
    const int after = life - cost;
    if (after > 0)
        o << " {this payment puts you at " << after << "}";
    else
        o << " {you are at " << life << " life: paying " << cost << " puts you at "
          << after << " and you LOSE the game}";
    return o.str();
}

//N-139q (wave-36): the pay-life consequence is CONDITIONAL. When the land was
//PUT onto the battlefield tapped by another effect (Arboreal Grazer's "put a
//land ... tapped"), the shock replacement still asks, but paying does NOT
//untap it - the engine keeps it tapped (verified in the 139v116 s8 board
//snapshot: "Steam Vents [tapped]" AT the ask, still tapped after the payment,
//life 20->18). The old unconditional "enters UNTAPPED - usable this turn"
//text was therefore a LIE in exactly that context, and the arm-C pilot paid
//2 life for nothing on its word. alreadyTapped selects the truthful text.
//W36 lane C: the ask also NAMES the land (all three arm-C fallbacks on this
//ask class clustered on the nameless shape); empty landName keeps the old
//generic wording.
//(history: comment-archaeology.md AIPlayerGPTSeams-etbUsableClause-2095)
static string etbUsableClause(bool canTapForMana, bool canAttackEver)
{
    if (canTapForMana && canAttackEver)
        return " [usable (tap for mana / attack) this turn]";
    if (canTapForMana)
        return " [usable (tap for mana) this turn]";
    if (canAttackEver)
        return " [usable (attack) this turn]";
    return " [it is on the battlefield untapped from now on]";
}


static bool annotateEtbPayOrTapMenu(vector<string>& opts, const string& landName,
                                    bool alreadyTapped = false,
                                    bool canTapForMana = true,
                                    bool canAttackEver = true)
{
    bool hasPayLife = false, hasTap = false;
    for (size_t i = 0; i < opts.size(); i++)
    {
        if (isPayLifeOption(opts[i]))
            hasPayLife = true;
        if (isTapOption(opts[i]))
            hasTap = true;
    }
    if (!(hasPayLife && hasTap))
        return false;
    //W36 #7 (139 N2: all three arm-C deck139 fallbacks landed on this one ask):
    //the bare "pay 2 life"/"tap" labels gave the echo no anchor - the pilot
    //reasons about the LAND's name. Put the land into the option SHORT NAME
    //(outside the [] tail, which answer matching strips), so its natural echo
    //matches. landName may be empty (recovery failed): generic wording then.
    string who = landName.empty() ? string("this permanent") : landName;
    for (size_t i = 0; i < opts.size(); i++)
    {
        if (isPayLifeOption(opts[i]))
        {
            if (alreadyTapped)
                opts[i] += " [NO-OP: " + who + " was put onto the battlefield"
                           " TAPPED by the effect that moved it, and it is already"
                           " tapped - paying life will NOT untap it. The payment"
                           " buys nothing; decline it]";
            else
                opts[i] += " - " + who + " enters UNTAPPED"
                           + etbUsableClause(canTapForMana, canAttackEver); //#W76-CP (Q14)
        }
        else if (isTapOption(opts[i]))
        {
            if (alreadyTapped)
                opts[i] += " [decline the payment - " + who + " is already"
                           " tapped and stays tapped either way]";
            else
                opts[i] += " - " + who + " enters TAPPED [decline the payment;"
                           " unusable until your next untap step]";
        }
    }
    return true;
}


//W36 #7, the name recovery: the menu's own instance is nameless (w26a probe -
//name cleared, model NULL), but by the time the pay-or-tap ask arms the land
//HAS entered the battlefield (the narration shows "You played X" before this
//ask). The newest fresh land in the chooser's own battlefield is the one that
//armed it; prefer the shock text shape as a discriminator, fall back to the
//newest fresh land, and return NULL when there is none (the caller then keeps
//the generic wording - a wrong name would be worse than no name).
//W37 #1: return the INSTANCE, not just the name - the wave-36 tapped-truth fix
//read ctx->isTapped() on the menu's STRIPPED COPY (always untapped), so every
//Grazer-put shockland ask still promised "enters UNTAPPED" live (validation
//139 N1, >=7 asks, 2 no-op pays). The tapped state must be read off the SAME
//recovered battlefield land the name comes from.
static MTGCardInstance * etbPayOrTapLand(Player * p)
{
    if (!p || !p->game || !p->game->inPlay)
        return NULL;
    MTGGameZone * bf = p->game->inPlay;
    MTGCardInstance * fallback = NULL;
    for (int zi = bf->nb_cards - 1; zi >= 0; zi--)
    {
        MTGCardInstance * lc = bf->cards[zi];
        if (!lc || !lc->isLand() || !lc->fresh)
            continue;
        if (!fallback)
            fallback = lc;
        string lt = lc->text;
        for (size_t i = 0; i < lt.size(); i++)
            lt[i] = (char) tolower((unsigned char) lt[i]);
        if (lt.find("enters tapped") != string::npos && lt.find("pay") != string::npos)
            return lc;
    }
    return fallback;
}


//Triggered/activated "may"-ability ask assembly (Tergrid's steal class). The
//card-script option label is a subject-less string ("Put onto battlefield
//under your control") and the source card is the TRIGGER SOURCE - so with no
//object named the model read the verb as acting on the source itself,
//concluded the prompt was a glitch ("Tergrid is already on the battlefield")
//and declined a free permanent (w26 deck199 re-probe). Given the source name
//and the resolved object's name + origin zone (both recovered upstream in
//DecisionManager::buildMenuChoice), build an ask header that NAMES the object
//and marks the source AS the trigger source, and annotate the real option
//(opts[0]; a decline, when offered, is the appended last entry) so its verb
//clearly refers to the object, not the source. Representation only - the
//caller leaves req.optionTexts (the staleness key) untouched. objName is
//non-empty by construction (the caller gates on a resolved object).
static string buildMayObjectAsk(const string & srcName, const string & objName,
                                const string & objOrigin, vector<string> & opts)
{
    string origin = objOrigin.empty() ? string() : (" (from " + objOrigin + ")");
    string src = srcName.empty() ? string("A triggered ability")
                                 : ("Triggered ability of " + srcName);
    string srcRef = srcName.empty() ? string("the source") : srcName;
    if (!opts.empty())
        opts[0] += " [" + objName + origin + " is the card this acts on, NOT " + srcRef + "]";
    return src + " - you MAY act on " + objName + origin + ". " + srcRef
         + " is the trigger SOURCE, not the object being moved. Choose:";
}


//N-152h (wave-36, deck152 vs139 s19): the Intrepid Adversary-class ETB menu
//("Don't add any counter" / "Add 1 counter" ... "Add 20 counters") offers the
//full printed range with no affordability information, and the engine then
//PARTIAL-PAYS whatever the chosen count's per-counter payments can cover
//(verified live: "add 10" chosen on ~3 spendable mana resolved exactly 2 valor
//counters). 21 options where ~2 are real is a mis-echo trap, and the
//partial-pay semantics were undocumented on the surface. The counts CANNOT be
//filtered (option indices are the contract's answer space and the staleness
//key), so state the true semantics in the ask header: payments are
//per-counter, an over-ask never fails - it stops at what the mana covers.
//Pure over the option texts; fires only on the pay-repeat shape (two or more
//"Add N counter(s)" modes).
//W37 #3: the LIVE menu renders lowercase "add N counters" (grep-confirmed in
//the wave-36 corpus - "Choose an option for Intrepid Adversary: 1. don't add
//any counter ... 3. add 2 counters"), while the auto-script name() reads
//"Add N counters". The original capital-only matcher never fired live (and its
//PARSETEST fixture tested the capitalized form only - the fixture-lies class).
//Match case-insensitively, like the shockland pay/tap matcher above.
static bool isAddNCountersOption(const string & optRaw)
{
    string opt = toLowerCopy(optRaw);
    if (opt.compare(0, 4, "add ") != 0)
        return false;
    size_t i = 4;
    if (i >= opt.size() || !isdigit((unsigned char) opt[i]))
        return false;
    while (i < opt.size() && isdigit((unsigned char) opt[i]))
        i++;
    return opt.find(" counter", i) == i;
}

//#W62-Y (D7, wave-61 deck152 HIGH-3, a vanished combat step). The pay-repeat
//menu states its partial-pay SEMANTICS (payRepeatModeNote, below) and not one
//NUMBER: `G2 seq 23/24` rendered "Choose an option for Intrepid Adversary:
//add 1 counter .. add 20 counters" with no mana line, no total and no
//`{paying this taps: ...}` clause, the seat answered "add 3 counters", the
//engine spent 6 more mana - Katilda, Dawnhart Prime and Elite Spellbinder, both
//creatures - and t11's Attackers step was gone. The guide's #2a stop is built
//entirely on that clause, so with the clause absent the stop cannot fire.
//
//The per-counter price is in the card's OWN script, on the line that names this
//very option: `choice name(Add 3 counters) thisforeach(variable{3}) ability$!may
//name(Pay to add counter) pay({1}{W}) ...` (Intrepid Adversary,
//borderline.txt:57864-57883). Read from there, never guessed: an option whose
//line carries no `pay(` is left exactly as the engine wrote it. The live menu
//lowercases its labels (W37 #3), so the match is case-insensitive like every
//other label matcher on this seam. Pure over the script and the label, so
//PARSETEST proves it without a board.
static string payRepeatPerCounterCost(const string& magicText, const string& optionLabel)
{
    if (optionLabel.empty() || magicText.empty())
        return "";
    const string want = "name(" + toLowerCopy(optionLabel) + ")";
    const string low = toLowerCopy(magicText);
    size_t lp = 0;
    while (lp <= low.size())
    {
        size_t nl = low.find('\n', lp);
        string line = low.substr(lp, nl == string::npos ? string::npos : nl - lp);
        lp = (nl == string::npos) ? low.size() + 1 : nl + 1;
        if (line.find(want) == string::npos)
            continue;
        size_t pp = line.find("pay({");
        if (pp == string::npos)
            continue;
        size_t open = pp + 4; //at the '{'
        size_t close = line.find(')', open);
        if (close == string::npos || close <= open)
            continue;
        return line.substr(open, close - open);
    }
    return "";
}


//#W62-Y (D7): the converted cost of a printed cost string ("{1}{w}" -> 2). A
//numeric symbol is its number, every other symbol is one. Pure.
static int manaCostTextCmc(const string& costText)
{
    int total = 0;
    size_t i = 0;
    while (i < costText.size())
    {
        if (costText[i] != '{')
        {
            i++;
            continue;
        }
        size_t e = costText.find('}', i);
        if (e == string::npos)
            break;
        string sym = costText.substr(i + 1, e - i - 1);
        bool numeric = !sym.empty();
        for (size_t k = 0; k < sym.size(); k++)
            if (!isdigit((unsigned char) sym[k]))
                numeric = false;
        total += numeric ? atoi(sym.c_str()) : 1;
        i = e + 1;
    }
    return total;
}


//#W62-Y (D7): the row's own bill. `available` is the seat's spendable mana
//right now (floating pool + untapped producers) or -1 when it could not be
//computed, in which case the row states the price and claims nothing about what
//the seat can pay. The "pays for K and stops" half is the partial-pay rule the
//header already states, applied to THIS row's number.
//#W62-AA (R4, wave-62 codex review finding 4): `paidCopies` is the MANA
//ENGINE's answer for this row - the largest number of copies of `perCost` the
//seat can actually pay, colours and all. Wave 62 derived it here as
//`available / perCmc`, which treats a source count as fungible mana: six
//untapped Mountains "paid for" three {1}{W} payments the seat cannot make at
//all. When the engine could not decide (-1) the row keeps the arithmetic bound
//but says out loud that it is conditional on colours, rather than claiming a
//payment count it has not checked. Pure over its five arguments.
static string payRepeatRowCostTag(int counters, const string& perCost, int perCmc, int available,
                                  int paidCopies = -1) //#W62-AA (R4)
{
    if (counters <= 0 || perCost.empty() || perCmc <= 0)
        return "";
    std::ostringstream o;
    o << " {repeat cost: " << counters << " x " << perCost << " = " << (counters * perCmc)
      << " mana for all " << counters;
    if (available >= 0)
    {
        int bound = available / perCmc;
        if (bound > counters)
            bound = counters;
        o << "; you have " << available << " spendable now, which pays for ";
        if (paidCopies >= 0)
        {
            int paid = paidCopies > counters ? counters : paidCopies;
            o << paid << " of them";
            if (paid < counters)
                o << " and stops";
        }
        else
        {
            o << "up to " << bound << " of them if your colours fit";
            if (bound < counters)
                o << ", then stops";
        }
    }
    o << "}";
    return o.str();
}


//#W63-AE (E18, D7's missing half): the taps clause on a REPEAT row. On a cast
//row "{paying this taps: ...}" is unambiguous - there is one payment. On a
//pay-repeat row the seat asked for `counters` payments and the engine will only
//make the `paid` of them the mana covers, so the bare clause let "this" read as
//all of them: the row would name two creatures while the same row's
//{repeat cost:} half said the mana stops after two of twenty. The two halves are
//fed the SAME `paid`, and where it is short of the ask the clause says which
//number it is pricing, so the mana half and the taps half cannot disagree.
//Prefix and brace shape are unchanged (stripNarrationDecoration keys on
//"{paying this taps: "). Pure over its four arguments.
static string payRepeatTapsClause(const std::vector<std::string>& names,
                                  const std::vector<int>& restrictions,
                                  int paid, int counters,
                                  const string& untapSource = "") //#W79-DA (T7)
{
    string clause = paymentTapsClause(names, restrictions, untapSource);
    if (clause.empty() || paid <= 0 || counters <= 0 || paid >= counters)
        return clause;
    const size_t close = clause.rfind('}');
    if (close == string::npos)
        return clause;
    std::ostringstream scope;
    scope << " (that is the " << paid << " payment" << (paid == 1 ? "" : "s")
          << " your mana covers, not all " << counters << ")";
    return clause.substr(0, close) + scope.str() + "}";
}


//#W67-AW (I9a, deck152 HIGH-1): the Intrepid Adversary ETB renders one row per
//rung the CARD offers - `borderline.txt:57863-57883` hard-codes 21 `auto=choice`
//lines - and `152v146` s11 printed all 21 (~2.7 kB) while the seat had 2
//spendable mana. Rows 3..21 were BEHAVIOURALLY IDENTICAL: each says "you have 2
//spendable now, which pays for 1 of them and stops", i.e. each adds the same
//ONE counter. 5 occurrences on the corpus, ~13.5 kB of menu.
//
//The engine's menu is untouched, and since #W67-AZ (R4) so is the PRINTED list:
//this decides which rows are DESCRIBED at length and which get one short clause
//saying they are identical right now. Every rung keeps its row, its number and
//its answerability; the band's LARGEST rung - the biggest ask the card offers,
//and the one that is never worse if the mana estimate moves before the payment -
//is the row that carries the band's arithmetic.
//
//`paid[i]` is the mana engine's own answer for row i (-1 = it could not decide).
//The collapse runs ONLY when every add-N row's `paid` is known and identical:
//an unknown or a varying count means the rows are not proven identical, and
//then the full list is printed exactly as wave 66 printed it. Pure over the
//engine labels and those counts, so PARSETEST proves the whole decision.
//Returns the index of the band's largest row (the one that carries the band's
//own clause), or -1 for "there is no identical band". `bandRows` marks the OTHER
//rows of the band - the ones whose description collapses.
static int payRepeatCollapse(const vector<string>& optionTexts, const vector<int>& paid,
                             vector<bool>& bandRows, int& bandLow, int& bandHigh, int& bandPaid)
{
    bandRows.assign(optionTexts.size(), false);
    bandLow = bandHigh = bandPaid = 0;
    if (optionTexts.size() != paid.size())
        return -1;
    int common = -1, keep = -1, keepN = 0, low = 0, addRows = 0;
    for (size_t i = 0; i < optionTexts.size(); i++)
    {
        if (!isAddNCountersOption(optionTexts[i]))
            continue;
        addRows++;
        const int n = atoi(optionTexts[i].c_str() + 4);
        if (n <= 0 || paid[i] < 0)
            return -1;
        //#W69-BI (K7, deck152 LOW / BD-6 FAIL). THE IDENTITY TEST INCLUDED THE
        //ROWS THAT ARE NOT IN THE BAND. It demanded the SAME `paid` on every
        //add-N row, but a rung the mana reaches in full pays its own N - so
        //`add 1` pays 1 while every capped rung pays the ceiling, and the test
        //returned -1 for every menu with a ceiling above 1. Measured on
        //`matchups-20260906-134120`: 6 band windows, 120 add-N rows, 19,782 B.
        //The three windows at ceiling 0 or 1 collapsed (rows 54-56 B); the three
        //at ceiling 2 collapsed NOTHING and ran 5,682 / 6,100 / 3,330 B with 18
        //rows each reading "pays for 2 of them and stops" - the deck-152 LOW's
        //"18 unusable rows" and BD-6's un-shortened band are ONE defect, here.
        //The band is the CAPPED rungs only, which is what the second loop below
        //(`n <= common` -> its own row) has always assumed; a fully-payable rung
        //is skipped rather than failing the whole menu. At a ceiling of 0 or 1
        //this selects exactly the rows it selected before, byte for byte.
        if (paid[i] >= n)
            continue; //the mana reaches this rung in full: it is its own row
        if (common < 0)
            common = paid[i];
        else if (paid[i] != common)
            return -1; //two different stopping points: not one identical band
    }
    //A menu whose rows the mana pays for in full has no unreachable band.
    if (addRows < 2 || common < 0)
        return -1;
    for (size_t i = 0; i < optionTexts.size(); i++)
    {
        if (!isAddNCountersOption(optionTexts[i]))
            continue;
        const int n = atoi(optionTexts[i].c_str() + 4);
        if (n <= common)
            continue; //a rung the mana reaches in full: it is its own row
        if (!low || n < low)
            low = n;
        if (n > keepN)
        {
            keepN = n;
            keep = (int) i;
        }
    }
    //Fewer than two identical rungs is nothing to collapse.
    if (keep < 0 || low >= keepN)
        return -1;
    for (size_t i = 0; i < optionTexts.size(); i++)
    {
        if ((int) i == keep || !isAddNCountersOption(optionTexts[i]))
            continue;
        if (atoi(optionTexts[i].c_str() + 4) > common)
            bandRows[i] = true;
    }
    bandLow = low;
    bandHigh = keepN;
    bandPaid = common;
    return keep;
}


//#W67-AW (I9a): what the kept row says about the rows it stands for. The band
//is NAMED, so an answer that reaches for a number inside it has a row to land
//on, and the reachable maximum is stated as the number that actually happens.
//#W67-AZ (R4, codex review finding 4): the tag says what is TRUE of the band
//now that every row of it is still listed and still answerable. Wave 67 hid
//rows 3..21 and told the model the kept row "stands for all of them" - a legal
//choice the model could no longer make, which is the render removing a decision
//rather than shortening it. The rows stay; only their descriptions collapse.
static string payRepeatCollapseTag(int bandLow, int bandHigh, int bandPaid)
{
    if (bandHigh <= bandLow || bandLow <= 0)
        return "";
    std::ostringstream o;
    o << " {rows for " << bandLow << " through " << bandHigh
      << " counters are all identical in effect right now: your mana pays for " << bandPaid
      << " payment" << (bandPaid == 1 ? "" : "s")
      << " and stops, so every one of them adds the same " << bandPaid
      << " counter" << (bandPaid == 1 ? "" : "s")
      << " - each of those rows is still on this list and any of them is a legal answer;"
         " this row is the largest ask the card offers}";
    return o.str();
}


//#W67-AZ (R4): what each OTHER row of the identical band says. The row keeps its
//own engine label and its own number - it is answerable exactly as it was - and
//its long price/taps description is replaced by the one fact that distinguishes
//it right now, which is that it does not differ. Pure.
static string payRepeatBandRowTag(int bandPaid)
{
    if (bandPaid < 0)
        return "";
    //#W68-BD (MED, wave-67 engine adjudication: I9a's byte saving is ZERO after
    //AZ R4). Measured on the corpus: the 7 band windows still run 2,514-2,737 B
    //because each of the 19 band rows carries EIGHTY-FIVE bytes of tag - this
    //clause at 46 B and `{this mode has a legal object right now}` at 39 B, the
    //same two sentences nineteen times. Every rung stays on the list and stays
    //answerable; the row is shortened to its engine label plus the one fact
    //that distinguishes it, which is that it does not differ. The legality of
    //the band is stated once, on the kept row's collapse tag ("any of them is a
    //legal answer"), instead of once per row.
    std::ostringstream o;
    o << " {same effect right now: adds " << bandPaid << " counter"
      << (bandPaid == 1 ? "" : "s") << "}";
    return o.str();
}



static string payRepeatModeNote(const vector<string>& opts)
{
    int addModes = 0;
    for (size_t i = 0; i < opts.size(); i++)
        if (isAddNCountersOption(opts[i]))
            addModes++;
    if (addModes < 2)
        return "";
    return " NOTE: the counters are paid for ONE AT A TIME (the card text names"
           " the per-counter cost), from mana you still have available now."
           " Choosing a number does NOT require that you can pay for all of"
           " them: the engine charges as many per-counter payments as your mana"
           " actually covers, adds exactly that many counters, and stops - an"
           " over-ask never fails and never loses the spell. With no spendable"
           " mana left, every option adds 0 counters.";
}


//N-146q (wave-36, ESCALATED - recurred with a harmful pick): a fused/compound
//mode's target ask carried NO mode attribution. Silverquill Command's
//"creature gains 3/3 and sacrifice creature" mode targets ONLY the pump half
//(the sacrifice half is `notaTarget ... opponent` - the OPPONENT chooses it),
//but the ask read as one target for the whole spell, and the arm-C pilot
//flagged the ambiguity in-trace ("It implies one target for the whole spell")
//then picked the opponent's Plague Stinger - +3/+3-and-flying'ing the
//opponent's creature (146 vs105 s31-32; arm B B3 s35 same shape). Pure over
//the mode name so the emitted claim is provable: fires only on a compound
//(" and " in the acting ability's name), and states the engine truth - each
//targeted part asks separately, this pick lands ONLY on the part it belongs
//to, and parts assigned to the opponent are the opponent's choice.
static string compoundModeTargetNote(const string& modeName)
{
    if (modeName.find(" and ") == string::npos)
        return "";
    return " NOTE: \"" + modeName + "\" is a COMPOUND effect. Each part that"
           " targets asks for its target SEPARATELY - THIS pick aims only the"
           " part this ask belongs to, and the chosen target is what that part"
           " ACTS ON (a gain/pump lands ON the creature you pick, so picking an"
           " enemy creature HELPS the enemy). Any part the effect assigns to the"
           " opponent (such as \"sacrifice creature\" demanded of the opponent)"
           " is chosen BY THE OPPONENT from their own cards - it is never chosen"
           " by this pick.";
}


//N-139a: role header for the mutate OVER/UNDER placement menu. Pure helper.
//W38: the header also asks for the intended HOST on the PLAN line - the host
//is a SEPARATE later ask, and the wave-37 corpus showed the intent dying at
//that boundary (139v152 s30-31: "over" chosen intending Dryad, then Gemrazer
//picked as host). A plan that names the host rides the existing plan carry
//into the host ask, where the carry line renders it back.
static string mutateOverUnderHeader(const string& ctxName)
{
    string who = ctxName.empty() ? string("this creature") : ctxName;
    return "MUTATE PLACEMENT for " + who + ": choose whether " + who + "'s card goes"
           " OVER (on top of) or UNDER the host. This is a middle step of the mutate"
           " cast - you pick WHICH of your non-Human creatures to mutate onto NEXT."
           " OVER keeps " + who + "'s name and P/T on the merged creature; UNDER keeps"
           " the host's. Either way the merged creature gains BOTH cards' abilities"
           " (CR 725). Choose, and state in a PLAN: line which creature you intend"
           " as the HOST - the host is picked at the NEXT ask, and your plan is"
           " shown there.";
}


//W38 (wave-37 validation #3): placement from a rendered label or ability name,
//case-insensitive. Card scripts pass through the parser LOWERCASED, so live
//menus and waiting-ability names read "mutate over"/"mutate under" while the
//primitives (and the old checks here) spell "Mutate Over" - the capitalized-
//only detection missed the live labels and dropped BOTH mutate role headers
//in the wave-37 corpus (139v152 s30 got the generic "Choose an option", s31
//the generic TARGET CHOICE). Returns 1 = over, 2 = under, 0 = not a mutate
//placement label.
static int mutatePlacementFromLabel(const string& s)
{
    string lc = s;
    for (size_t i = 0; i < lc.size(); i++)
        lc[i] = (char) tolower((unsigned char) lc[i]);
    if (lc.compare(0, 11, "mutate over") == 0)
        return 1;
    if (lc.compare(0, 12, "mutate under") == 0)
        return 2;
    return 0;
}


//W38 host-intent carry: the single candidate name that `text` mentions
//(case-insensitive substring). Exactly ONE distinct name must match - a plan
//that names several candidates (or none) is not a commitment to render.
static string uniqueNamedIn(const string& text, const vector<string>& names)
{
    string lct = text;
    for (size_t i = 0; i < lct.size(); i++)
        lct[i] = (char) tolower((unsigned char) lct[i]);
    string found;
    for (size_t i = 0; i < names.size(); i++)
    {
        if (names[i].empty() || names[i] == found)
            continue;
        string lcn = names[i];
        for (size_t j = 0; j < lcn.size(); j++)
            lcn[j] = (char) tolower((unsigned char) lcn[j]);
        if (lct.find(lcn) == string::npos)
            continue;
        if (!found.empty())
            return string(); //two distinct names named: ambiguous, no carry
        found = names[i];
    }
    return found;
}


//#W47 R2 (wave-46 HIGH; the same seat, the same card, two corpora running):
//the "Choose an option for <card>:" cast-mode menu is the only BARE menu left
//in the loop. Its rows render as "1. Cast Card Normally / 2. cycling / 3.
//Decline - do nothing": no cost on either path, no effect on either path, no
//draw named on the cycling row - while the CAST row one screen earlier carries
//a full {X pricing:} + {card text:} and the ANNOUNCE menu one screen later
//carries an annotation per option. deck130 vs125 arrived at this menu with the
//plan "Cast Starstorm at X=0 TO CYCLE IT ... per Strategy Guide Rule #1",
//answered "1. Cast Card Normally", then announced X=0 - a spell that draws
//nothing - and lost the game. The guide lane is declared exhausted for this
//failure (skill amendment 85); the remedy named in writing is this annotation.
//
//Everything below is read off the card's OWN cost and printed text; nothing is
//inferred about what a path does not do. Presentation only: req.optionTexts
//(the staleness key) is untouched, and only trailing text is appended, so the
//option count, ORDER and answer index are all unchanged.

//The " -- "-separated clause of a printed card text that BEGINS with `label`
//(case-insensitive): Starstorm's "Cycling {3} ({3}, Discard this card: Draw a
//card.)" for the "cycling" row. Empty when no clause starts with it - the row
//then carries its cost and nothing invented.
static string printedClauseFor(const string& cardText, const string& label)
{
    if (cardText.empty() || label.empty())
        return "";
    size_t lp = 0;
    while (lp <= cardText.size())
    {
        size_t sep = cardText.find(" -- ", lp);
        string clause = cardText.substr(lp, sep == string::npos ? string::npos : sep - lp);
        lp = (sep == string::npos) ? cardText.size() + 1 : sep + 4;
        size_t b = clause.find_first_not_of(" \t\r\n");
        if (b == string::npos)
            continue;
        clause = clause.substr(b);
        if (clause.size() < label.size())
            continue;
        bool match = true;
        for (size_t i = 0; i < label.size() && match; i++)
            match = tolower((unsigned char) clause[i]) == tolower((unsigned char) label[i]);
        if (match)
            return clause;
    }
    return "";
}


//The FIRST clause of a printed card text - what casting the card normally
//does. Same splitter, so the two halves of the menu cannot drift apart.
//#W50-X D17 (wave-49 ledger LOW; 6 bare `// <name>` endings on the
//`Choose an option for <MDFC>:` menu's Cast Card Normally row): this emitter
//took the first ` -- ` clause verbatim, and an MDFC whose front face is a
//single clause (Pelakka Predation) has its back face's bare name INSIDE that
//clause. Same face rule as optionCardTextCore: a face after the `//` with no
//'.', ':' or '(' is a name, not text, and says so.
//(markBareBackFaces now lives beside stackTargetTextCore - #W51-F D16 - so
//all three face emitters share one definition in one scope.)

string printedFirstClause(const string& cardText)
{
    size_t sep = cardText.find(" -- ");
    string clause = cardText.substr(0, sep == string::npos ? string::npos : sep);
    size_t b = clause.find_first_not_of(" \t\r\n");
    return (b == string::npos) ? string("") : markBareBackFaces(clause.substr(b));
}


//The cost tokens standing in front of the ability named `abilityName` in a
//script ("{3}{cycle}:name(cycling) draw:1" -> "{3}{cycle}"). Empty when the
//name is absent or the line has no cost prefix.
static string scriptAbilityCost(const string& script, const string& abilityName)
{
    if (abilityName.empty())
        return "";
    string lowName = abilityName;
    for (size_t i = 0; i < lowName.size(); i++)
        lowName[i] = (char) tolower((unsigned char) lowName[i]);
    string needle = "name(" + lowName + ")";
    size_t lp = 0;
    while (lp <= script.size())
    {
        size_t nl = script.find('\n', lp);
        string line = script.substr(lp, nl == string::npos ? string::npos : nl - lp);
        lp = (nl == string::npos) ? script.size() + 1 : nl + 1;
        string low = line;
        for (size_t i = 0; i < low.size(); i++)
            low[i] = (char) tolower((unsigned char) low[i]);
        if (low.find(needle) == string::npos)
            continue;
        size_t s = low.find_first_not_of(" \t\r");
        if (s == string::npos || low[s] == '@')
            continue; //a trigger, not an activation the pilot pays
        size_t colon = low.find(':', s);
        if (colon == string::npos || colon <= s)
            continue;
        return line.substr(s, colon - s);
    }
    return "";
}


//Read a script cost prefix as English. Mana symbols pass through untouched
//(they are the same tokens every other cost surface prints); the engine's
//non-mana cost braces are named, and an UNRECOGNISED brace is passed through
//verbatim rather than dropped - a silent omission is the gap the model
//confabulates a rule into (trust doctrine).
static string scriptCostGloss(const string& costTokens)
{
    string mana, extras;
    size_t i = 0;
    while (i < costTokens.size())
    {
        if (costTokens[i] != '{')
        {
            i++;
            continue;
        }
        size_t end = costTokens.find('}', i);
        if (end == string::npos)
            break;
        string tok = costTokens.substr(i, end - i + 1);
        i = end + 1;
        string low = tok;
        for (size_t k = 0; k < low.size(); k++)
            low[k] = (char) tolower((unsigned char) low[k]);
        const char * gloss = NULL;
        if (low == "{cycle}")
            gloss = "discard this card";
        else if (low == "{t}")
            gloss = "tap it";
        else if (low == "{q}")
            gloss = "untap it";
        if (gloss)
        {
            if (!extras.empty())
                extras += ", ";
            extras += gloss;
        }
        else
            mana += tok;
    }
    if (mana.empty())
        return extras;
    if (extras.empty())
        return mana;
    return mana + ", " + extras;
}


//One row's annotation: its own cost, then its own printed text. Both halves
//are optional (a free path prints no cost clause; a card with no printed text
//for that path prints no quote) and the tag is empty when neither exists.
//(history: comment-archaeology.md AIPlayerGPTSeams-castModeCastPriceTag-2111)
static string castModeCastPriceTag(const string& castCost, const string& castClause,
                                   int legalTargets)
{
    if (castCost.empty() && castClause.empty())
        return "";
    std::ostringstream o;
    o << " {the cast this replaces: ";
    if (!castCost.empty())
        o << "cost " << castCost;
    if (!castClause.empty())
        o << (castCost.empty() ? "" : " - ") << castClause;
    if (legalTargets >= 0)
        o << " (" << legalTargets << " legal target" << (legalTargets == 1 ? "" : "s")
          << " on the board right now)";
    o << "}";
    return o.str();
}


//#W71-BQ (L5, wave-70 deck130 HIGH): the header fact that makes the cast-mode
//menu the SECOND HALF of a decision instead of a fresh choice between two uses
//of the card. 7 confirmed reversals in one seat - `Cast Starstorm` answered at
//the casting ask, then the `cycling` row taken at "Choose an option for
//Starstorm:", discarding the card the same reply's PLAN said to cast (deck126
//seq 35 -> 36; also deck125 4->5, 37->38, 41->42, 47->48, deck146 23->24,
//deck126 4->5). 21 modal cycling menus, 4 answered "Cast Card Normally".
//Nothing is removed and nothing is answered: taking the alternative is legal
//and stays offered in its own place. Pure over the four facts, so the gate is
//provable without a game: the menu must BE a cast-mode menu (the engine's own
//"Cast Card Normally" label), it must be about the card the model committed to,
//and the commitment must be THIS turn - a menu armed by the heuristic's own
//pick, or by the same card a turn later, carries no claim about what the model
//said.
string castModeCommitmentNote(bool castModeMenu, const string& ctxName,
                                     const string& committedName, bool sameTurn)
{
    if (!castModeMenu || !sameTurn || committedName.empty() || ctxName != committedName)
        return "";
    return " (you answered \"Cast " + committedName + "\" at the previous window -"
           " this menu completes THAT cast, it is not a new offer: the \"Cast Card"
           " Normally\" row casts it, and every other row abandons the cast you"
           " just chose)";
}


//And the symmetric tag on the CAST row: the alternatives it spends. `draws` is
//the card count the alternative's own script draws (0 prints nothing about
//drawing rather than the false "draws 0" - a cycling-less alternative such as
//an equip or a channel path genuinely draws nothing and must not read as dead).
static string castModeAltPriceTag(const std::vector<std::string>& labels,
                                  const std::vector<std::string>& costs,
                                  const std::vector<int>& draws)
{
    if (labels.empty() || labels.size() != costs.size() || labels.size() != draws.size())
        return "";
    std::ostringstream o;
    o << " {the alternative" << (labels.size() == 1 ? "" : "s") << " this replaces: ";
    for (size_t i = 0; i < labels.size(); i++)
    {
        o << (i ? "; " : "") << labels[i];
        if (!costs[i].empty())
            o << " for " << costs[i];
        if (draws[i] > 0)
            o << " - draws " << draws[i] << " card" << (draws[i] == 1 ? "" : "s");
    }
    o << "}";
    return o.str();
}


static string castModeRowTag(const string& costBody, const string& clause)
{
    if (costBody.empty() && clause.empty())
        return "";
    std::ostringstream o;
    if (!costBody.empty())
        o << " [cost: " << costBody << "]";
    if (!clause.empty())
        o << " {card text: \"" << clause << "\"}";
    return o.str();
}


int AIPlayerGPT::chooseMenuAction(const DecisionRequest & req, DecisionAction & act)
{
    MTGCardInstance * ctx = req.contextCard;

    if (req.kind == DecisionRequest::ANNOUNCE_X)
    {
        //#W48 D9: the carried plan is suppressed for the whole assembly of this
        //ask (see mInAnnounceXAsk).
        AnnounceXAskScope announceScope(this);
        //option index IS the X value, and presenting X = 0 as option 1 set
        //an index/value trap: the model computed the X it wanted in its
        //plan, then replied that VALUE as the option number - one off in
        //its own disfavor every time (15/17 corpus X announcements landed
        //below the model's stated intent, four blanked to X = 0 - a
        //thrown-away Black Sun's Zenith among them). Present the options
        //LARGEST FIRST at the ask and map the pick back: the best X is
        //option 1, and a value-as-index slip now lands near the top
        //instead of at zero. The contract's index==X invariant and the
        //other consumers are untouched - this is presentation only.
        vector<string> shown(req.optionTexts.rbegin(), req.optionTexts.rend());
        //State the CAP and its reason (wave-23 ITEM, deck140 vs27 s46). The
        //offered X values top out at the highest AFFORDABLE X (mana-limited);
        //option index == X value, so the cap is the last option's X = the option
        //count minus one. Without this the model reasons about an X its mana
        //cannot reach (s46: computed X=20 as "lethal" while the engine offered
        //at most X=6) - a belief-vs-menu mismatch that costs a long reply and can
        //mis-place a kill. Cheap, truthful, no option changes: the cap already
        //bounds the option set built upstream (ManaEngine::maxAnnounceableX).
        int capX = (int) req.optionTexts.size() - 1;
        //Pass the spell being announced as the pending source: the model echoes
        //"Cast <spell> with X=N" against the bare "X = N" option, and INDEX-WINS
        //treats an echo that names the source spell as a self-reference rather
        //than a stale answer (wave-23 ITEM A shape 1, deck140 Black Sun's Zenith).
        //#W74-CC (O19): the ask narration is registered AFTER the display
        //permutation below, because it is indexed in SHOWN space - registering
        //it here would have narrated the wrong X for every row once the ladder
        //started climbing.
        //#W45-5: price each X row with what dies for it, on BOTH sides, off the
        //same survey that priced the cast row (see xKillRowCore). Presentation
        //only: req.optionTexts is untouched, and the option count and ORDER are
        //unchanged, so the shown-space -> index==X mapping below still holds.
        //The size guard is the contract rail - if the menu is ever built with
        //something other than one option per X from capX down to 0, nothing is
        //annotated rather than something mis-aligned being claimed.
        {
            std::vector<string> xKills;
            int markX = -1;
            string markText;
            if (xAnnounceRowKills(ctx, this, capX, xKills, &markX, &markText)
                && xKills.size() == shown.size())
            {
                for (size_t ki = 0; ki < shown.size(); ki++)
                    shown[ki] += xKills[ki];
                //#W77-CS (R6 b, wave-76 engine-seat MED-2): the X sub-menu
                //priced its kills and never priced what they take off the
                //CRACK-BACK NEXT TURN line, though the PARENT cast row carries
                //exactly that clause (#W76-CO Q5). One family, one wording,
                //per X - off the SAME survey these kill lists are built from
                //and the SAME crackBackBodyContribution walk the line itself
                //is built from, so no third number can drift in.
                {
                    int cbxTotal = 0;
                    bool cbxFloor = false;
                    XVictimSurvey cbxSv;
                    if (crackBackScreenTotalNow(cbxTotal, cbxFloor)
                        && xSurveyBoard(ctx, this, cbxSv) && cbxSv.priceable)
                    {
                        int cbxBodies = 0;
                        Player * cbxOpp = opponent();
                        MTGGameZone * cbxBf = (cbxOpp && cbxOpp->game)
                                                  ? cbxOpp->game->inPlay : NULL;
                        for (int ci = 0; cbxBf && ci < cbxBf->nb_cards; ci++)
                            if (crackBackBodyContribution(cbxBf->cards[ci]) > 0)
                                cbxBodies++;
                        for (size_t fi = 0; fi < shown.size(); fi++)
                        {
                            const int xv = capX - (int) fi;
                            int removed = 0, removedBodies = 0;
                            std::vector<W77RemovalVictim> cbxV; //#W77-CU (F2)
                            for (size_t vi = 0; vi < cbxSv.victims.size(); vi++)
                            {
                                const XDamVictim& v = cbxSv.victims[vi];
                                if (v.mine || v.isPlayer || !v.inst
                                    || v.lethalX <= 0 || v.lethalX > xv)
                                    continue;
                                //#W77-CU (F3, Astra HIGH 3): the wave-76 CQ F5
                                //survival test, which the parent X path
                                //(xSpellPricing) already applies and this
                                //sub-menu walk dropped.
                                const int c = w77CoverBodyContribution(v.inst, 1);
                                if (c <= 0)
                                    continue;
                                removed += c;
                                removedBodies++;
                                W77RemovalVictim rv;
                                rv.name = v.inst->getDisplayName() + instanceHandle(v.inst);
                                rv.takes = c;
                                cbxV.push_back(rv);
                            }
                            //#W77-CU (F2, Astra HIGH 2): A SINGLE-TARGET `damage:X`
                            //IS NOT A SWEEPER. The parent row's cover clause is
                            //gated on `sv.sweep` (xSpellPricing) and this walk was
                            //not, so a one-target X burn at X=3 against two 3/3
                            //attackers claimed SIX power removed and could promise
                            //survival off one kill. The classification the survey
                            //already carries now routes the clause: a sweep sums
                            //every victim; a TARGETED row prices each victim
                            //separately and says the pick is the seat's own - the
                            //same wording, and the same guarantee, the targeted
                            //cast row's own clause carries.
                            shown[fi] += w77CoverClauseFor(cbxSv.sweep, cbxTotal, life,
                                                           cbxFloor, removed, removedBodies,
                                                           cbxV, cbxBodies);
                        }
                    }
                }
                //#W56-C (D7 a): the mana-fit clause EVERY other priced cast row
                //carries. The cast row deliberately prints none (its comment
                //says why: "an {X} cost has no remainder yet - X is announced
                //AFTER this pick"), and this is the window where X is known, so
                //until now the single real trade-off on a monotone X menu -
                //what taking the big X leaves up - was on no screen at all.
                //Computed the same way as the cast row's: resolve the cost for
                //THIS row's X exactly as MTGPutInPlayRule does (copy, add the
                //announced total as generic - or as the X colour for a coloured
                //X - then clear the X flag) and ask the engine's own auto-tap
                //planner which sources it would spend. Counted per SOURCE CARD,
                //so it reads against the same total the mana line prints.
                {
                    //#W57-E (D9): the POOL route first - it is the one the
                    //corpus actually takes (9 of 9 menus). The pool covers the
                    //base cost plus capX exactly (that is how the menu was
                    //built), so the base is `pool - capX` and the row for X
                    //leaves `capX - X` floating. Derived from the MENU's own
                    //cap rather than from a second cost query, the same
                    //contract rail the annotations above ride: if the two ever
                    //disagree (a pay[[{X}]] announcement round, where the pool
                    //is empty by design) nothing is claimed here and the
                    //untapped-source form below runs instead.
                    int xPool = getManaPool() ? getManaPool()->getConvertedCost() : 0;
                    int xBaseCost = xPool - capX;
                    bool poolForm = (xPool > 0 && capX >= 0 && xBaseCost >= 0);
                    if (poolForm)
                        for (size_t fi = 0; fi < shown.size(); fi++)
                            shown[fi] += leavesFloatingTag(xPool,
                                                           xBaseCost + (capX - (int) fi));
                    ManaCost * xBase = ctx ? ctx->getManaCost() : NULL;
                    if (!poolForm && xBase && (xBase->hasX() || xBase->hasSpecificX()) && capX >= 0)
                    {
                        GptManaPolicy fitPolicy(this);
                        int xUntapped = ManaEngine::potentialColorReach(this, fitPolicy, NULL);
                        for (size_t fi = 0; xUntapped > 0 && fi < shown.size(); fi++)
                        {
                            int xv = capX - (int) fi;
                            ManaCost * resolved = NEW ManaCost();
                            resolved->copy(xBase);
                            if (xBase->xColor > 0)
                                resolved->add(xBase->xColor, xv);
                            else
                                resolved->add(Constants::MTG_COLOR_ARTIFACT, xv);
                            resolved->remove(7, 1); //clear the X flag
                            vector<MTGAbility*> fitPicks = ManaEngine::selectAutoTapProducers(
                                this, ctx, resolved, ctx->has(Constants::ANYTYPEOFMANA), false);
                            std::set<MTGCardInstance *> fitTapped;
                            for (size_t pi = 0; pi < fitPicks.size(); pi++)
                                if (fitPicks[pi] && fitPicks[pi]->source)
                                    fitTapped.insert(fitPicks[pi]->source);
                            shown[fi] += leavesUntappedTag(xUntapped, (int) fitTapped.size());
                            SAFE_DELETE(resolved);
                        }
                    }
                }
                //#W48 D9 / #W55-C (D6): mark exactly ONE row. shown is
                //largest-X-first, so the row for X is at index capX - X; the
                //guard is the same contract rail the annotations ride (nothing
                //is marked if the menu is ever built with something other than
                //one row per X).
                if (markX >= 0 && markX <= capX && !markText.empty()
                    && (size_t)(capX - markX) < shown.size())
                    shown[(size_t)(capX - markX)] += markText;
            }
        }
        //askEvenIfSingle: a zero-slack {X} cast offers exactly one value, and
        //the generic one-option shortcut answered it with no model call, no
        //stderr line and no translog record - X was committed for the pilot in
        //silence (wave-41 #W41-1/#W41-2; the deck198 zero-slack probe cast
        //Sphinx's Revelation ten times with ZERO ANNOUNCE_X records). X = 0 is
        //a legal and sometimes correct choice, so the question is real even
        //when the answer is forced: it is what puts the commitment in the log
        //and in front of the pilot.
        //#W62-Y (D5, deck130 HIGH-1): the DECLINE row. `130 s21` offered
        //`Cast Starstorm {X pricing: your mana affords only X=0 right now, which
        //deals 0 damage and kills nothing}`; taking it led to `s22`, whose whole
        //menu was one row (`X = 0 ... this cast does NOTHING: it deals 0 damage
        //and the spell is spent`) under the header "this ask has no pass row".
        //The card was unavoidably burned - offered in 7 windows, taken in 1.
        //The engine has always carried a Cancel row on this menu for the human
        //(MTGRules arms it with must=false); the decision contract simply never
        //exposed it (see DecisionContract.cpp #W62-Y (D5)). Nothing is capped and
        //no window is removed: one more TRUE row, the one that lets an X spell
        //whose only affordable X does nothing go back to the hand.
        //#W74-CC (O19): the LAST act of assembly, after every annotation above
        //has been placed at its `capX - X` index - permute the finished rows
        //into the order the pilot answers in, so that option N announces X = N
        //and X = 0 is the last rung. Nothing above this line moves, and the
        //pick is mapped back below.
        //#W74-CF: the permutation itself now lives in w74XPermuteToClimbing, whose
        //inverse (w74XPickToContractIndex, below) is pinned against it end to end.
        const bool xClimbing = w74XPermuteToClimbing(shown, capX); //#W74-CC (O19)
        {
            //Register: the announcement, not the menu question. In SHOWN space.
            string xName = ctx ? ctx->getDisplayName() : string("the spell");
            vector<string> narr;
            for (size_t xi = 0; xi < shown.size(); xi++)
                narr.push_back("You announced " + stripNarrationDecoration(shown[xi])
                               + " for " + xName);
            setAskNarration(narr);
        }
        const size_t xRowCount = shown.size();
        if (req.canDecline)
            shown.push_back(string("Decline - do not cast this after all"
                            " (the announcement is cancelled and the card stays in your hand)")
                            + xDeclineForfeitClause(getManaPool()->getConvertedCost())); //#W68-BB (J5)
        //#W67-AW (I4): the ceiling sentence, on the screen the number is picked
        //on. Built from the SAME reserve and the same ceiling function the rows
        //and the badge use, so the three surfaces cannot name different values.
        string xLibNote;
        if (ctx)
        {
            int hlPer = 0, hdPer = 0;
            if (xLifeDrawClauses(ctx->magicText, hlPer, hdPer) && hdPer > 0)
            {
                int hLib = -1, hRes = 1;
                string hWhy;
                xLibraryReserve(this, ctx, hLib, hRes, hWhy);
                xLibNote = xAnnounceLibraryNote(capX, hdPer, hLib, hRes, hWhy,
                                                xLibraryCeilingX(capX, hdPer, hLib, hRes));
            }
        }
        int pick = askModel(announceXHeader(ctx ? ctx->getDisplayName() : string("this spell"), capX,
                                            req.canDecline, xLibNote), //#W63-AD (E7) / #W67-AW (I4)
                            shown, true, ctx ? ctx->getDisplayName() : string(), true,
                            false, req.canDecline); //#W62-Y (D5)
        if (pick == kChoicePending)
            return kChoicePending;
        if (req.canDecline && pick == (int) xRowCount)
        {
            //#W68-BB (J5): the receipt above this line said the cost was paid.
            //Say that the spell it paid for is not happening.
            const string paidFor = ctx ? ctx->getDisplayName() : string();
            if (!mPaidPendingCard.empty() && mPaidPendingStep == stepKey()
                && (paidFor.empty() || paidFor == mPaidPendingCard))
            {
                appendNarration(castAbandonedNarration(mPaidPendingCard,
                                                       getManaPool()->getConvertedCost()));
                mPaidPendingCard.clear();
            }
            //#W74-CD (O23): the abandonment, on the path it actually takes.
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
            fprintf(stderr, "%s\n",
                    w74XAbandonTraceLine(paidFor, getManaPool()->getConvertedCost()).c_str());
#endif
            act.choice = -1; //applyMenuChoice clicks the menu's own Cancel row
            return 0;
        }
        mPaidPendingCard.clear(); //#W68-BB (J5): this announcement is going through
        //#W74-CC (O19): shown space -> the contract's index==X space. Display
        //row j (0-based) is X = j+1 while j < capX, and the last rung is X = 0.
        if (pick >= 0 && pick < (int) xRowCount)
            pick = w74XPickToContractIndex(pick, capX, (int) xRowCount, xClimbing,
                                           (int) req.optionTexts.size()); //#W74-CF
        else if (pick < 0)
            pick = AIPlayerBaka::selectMenuOption(); //heuristic: max affordable X
        if (pick >= (int) req.optionTexts.size())
            pick = (int) req.optionTexts.size() - 1;
        act.choice = pick < 0 ? 0 : pick;
        return 0;
    }

    if (req.kind == DecisionRequest::CHOOSE_MODE)
    {
        {
            string mName = ctx ? ctx->getDisplayName() : string("the spell");
            vector<string> narr;
            for (size_t mi = 0; mi < req.optionTexts.size(); mi++)
                //#W55-D (D22): a card-NAME choice is narrated with the same verb
                //the observing seat's "[named: <card>]" tag uses.
                narr.push_back(req.nameChoiceMenu
                               ? ("You named \"" + stripNarrationDecoration(req.optionTexts[mi])
                                  + "\" with " + mName)
                               : ("You chose " + mName + "'s mode \""
                                  + stripNarrationDecoration(req.optionTexts[mi]) + "\""));
            setAskNarration(narr);
        }
        //N-152h: a pay-repeat menu (Add N counters) states its partial-pay
        //semantics in the header - the counts themselves are the answer space
        //and stay untouched.
        //(history: comment-archaeology.md AIPlayerGPTSeams-L7351-2127)
        vector<string> shownModes = req.optionTexts;
        //#W55-D (D22): a card-NAME menu is not a mode menu. Its rows are card
        //names, so the mode live/dead clause has nothing to match and the
        //header must say what the choice IS. Each row gains only the PUBLIC
        //fact about that name; the list itself is the engine's.
        //#W72-BV (M7): the display permutation for a name menu. Identity for
        //every other menu kind, so nothing else in this seam changes.
        std::vector<size_t> nameOrder;
        if (req.nameChoiceMenu)
        {
            std::vector<int> notPublicTheirs(shownModes.size(), 0);
            for (size_t mi = 0; mi < shownModes.size(); mi++)
            {
                int tb = 0, tg = 0, mb = 0, mg = 0;
                int tAll = 0, mAll = 0, tPub = 0, mPub = 0;
                const string& want = req.optionTexts[mi];
                Player * players[2] = { this, opponent() };
                for (int pi = 0; pi < 2; pi++)
                {
                    Player * pl = players[pi];
                    if (!pl || !pl->game)
                        continue;
                    MTGGameZone * zs[2] = { pl->game->inPlay, pl->game->graveyard };
                    for (int zi = 0; zi < 2; zi++)
                    {
                        MTGGameZone * z = zs[zi];
                        for (int ci = 0; z && ci < z->nb_cards; ci++)
                            if (z->cards[ci] && z->cards[ci]->name == want)
                            {
                                if (pi == 0)
                                    (zi == 0 ? mb : mg)++;
                                else
                                    (zi == 0 ? tb : tg)++;
                            }
                    }
                    //#W72-BV (M7): the whole card pool of that seat, and the
                    //part of it that is already PUBLIC. The difference is the
                    //copies that can still be cast. Same nine zones the engine
                    //builds the name list from, so the two cannot disagree.
                    MTGGameZone * all[9] = { pl->game->inPlay, pl->game->graveyard,
                                             pl->game->hand, pl->game->library,
                                             pl->game->stack, pl->game->exile,
                                             pl->game->commandzone, pl->game->sideboard,
                                             pl->game->reveal };
                    const bool pub[9] = { true, true, false, false, true, true, true, false, true };
                    for (int zi = 0; zi < 9; zi++)
                        for (int ci = 0; all[zi] && ci < all[zi]->nb_cards; ci++)
                            if (all[zi]->cards[ci] && all[zi]->cards[ci]->name == want)
                            {
                                (pi == 0 ? mAll : tAll)++;
                                if (pub[zi])
                                    (pi == 0 ? mPub : tPub)++;
                            }
                }
                const int tRest = tAll - tPub > 0 ? tAll - tPub : 0;
                const int mRest = mAll - mPub > 0 ? mAll - mPub : 0;
                notPublicTheirs[mi] = tRest;
                shownModes[mi] += namedCardVisibilityTag(tb, tg, mb, mg);
                shownModes[mi] += namedCardRemainingTag(tRest, mRest); //#W72-BV (M7)
            }
            //#W72-BV (M7, deck146 HIGH-2): ORDER. The engine builds this list by
            //walking inPlay first, so the rows a cast trigger can never fire on
            //were rendered at the top of a menu whose own convention is
            //"usually-correct option first" - and the format example, written
            //out from row 1, then named the wrong card in the model's own words.
            //Re-ordered by the deciding number, highest first; ties keep the
            //engine's own order (stable). NOTHING is removed and the ANSWER
            //INDEX still means what applyMenuChoice thinks it means: the
            //permutation is inverted on the pick below, and the narration is
            //permuted with the rows so a narrated choice names the card taken.
            nameOrder.resize(shownModes.size());
            for (size_t k = 0; k < nameOrder.size(); k++)
                nameOrder[k] = k;
            std::stable_sort(nameOrder.begin(), nameOrder.end(),
                             [&notPublicTheirs](size_t a, size_t b)
                             { return notPublicTheirs[a] > notPublicTheirs[b]; });
            bool moved = false;
            for (size_t k = 0; k < nameOrder.size() && !moved; k++)
                moved = (nameOrder[k] != k);
            if (moved)
            {
                std::vector<string> rr(shownModes);
                for (size_t k = 0; k < nameOrder.size(); k++)
                    shownModes[k] = rr[nameOrder[k]];
                const std::vector<string> nn = mNextAskNarration;
                if (nn.size() == nameOrder.size())
                {
                    std::vector<string> pn(nn);
                    for (size_t k = 0; k < nameOrder.size(); k++)
                        pn[k] = nn[nameOrder[k]];
                    setAskNarration(pn);
                }
            }
            else
                nameOrder.clear();
        }
        else
        {
            //#W66-AQ (H9): the same three-bucket census as the cast row, so the
            //two screens cannot disagree about which pairs are only half alive.
            std::vector<std::string> liveM, halfM, deadM;
            if (ctx && modalModeLiveness(observer, ctx, ctx->magicText, liveM, halfM, deadM))
                for (size_t mi = 0; mi < shownModes.size(); mi++)
                {
                    string low = toLowerCopy(shownModes[mi]);
                    for (size_t li = 0; li < liveM.size(); li++)
                        if (toLowerCopy(liveM[li]) == low)
                            shownModes[mi] += modeLivenessRowTag(0);
                    for (size_t hi = 0; hi < halfM.size(); hi++)
                        if (toLowerCopy(halfM[hi]) == low)
                            shownModes[mi] += modeLivenessRowTag(1);
                    for (size_t di = 0; di < deadM.size(); di++)
                        if (toLowerCopy(deadM[di]) == low)
                            shownModes[mi] += modeLivenessRowTag(2);
                }
            //#W60-O (B8): which branch taps what, and the untap-step timing.
            //#W60-Q (R5): with the two engine states that falsify its general
            //rule counted off the OPPONENT's battlefield (the side a "TAPS
            //theirs" branch can pick from). Counted here, not asserted there.
            //#W64-AG (F6): and the price the bare label drops.
            //#W65-AL (G1): both now ride ONE function shared with the
            //CHOOSE_MENU seam, over a SCRIPT that survives a refused pointer -
            //`ctx` is NULL on precisely the `auto=choice` menus these tags were
            //written for. Presentation only: req.optionTexts (the staleness key)
            //and the row ORDER are untouched, so act.choice still means what
            //applyMenuChoice thinks it means.
            {
                const string modeScript = ctx ? ctx->magicText : req.contextText;
                int canBlockTapped = 0, doNotUntap = 0;
                modeTapCounts(opponent(), canBlockTapped, doNotUntap);
                //#W66-AQ (H9): the punisher term on this seam too - one
                //convention across the two menu kinds.
                vector<string> mMineP, mTheirsP;
                int mMinePer = 0, mTheirsPer = 0;
                drawPunisherScan(this, opponent(), mMineP, mMinePer, mTheirsP, mTheirsPer);
                for (size_t mi = 0; mi < shownModes.size() && mi < req.optionTexts.size(); mi++)
                    shownModes[mi] += modeRowAnnotations(modeScript, req.optionTexts[mi],
                                                         life,
                                                         opponent() ? opponent()->life : 0,
                                                         canBlockTapped, doNotUntap,
                                                         mTheirsPer);
            }
        }
        string modeHeader;
        if (req.nameChoiceMenu)
            modeHeader = chooseANameHeaderText(ctx ? ctx->getDisplayName() : req.contextName,
                                               ctx ? cardTextSnippet(ctx, 220) : string());
        else
            modeHeader = "Choose one mode for "
                         + (ctx ? ctx->getDisplayName() : string("this spell")) + ":"
                         + payRepeatModeNote(req.optionTexts);
        int pick = askModel(modeHeader, shownModes,
                            true, ctx ? ctx->getDisplayName() : string());
        if (pick == kChoicePending)
            return kChoicePending;
        //#W72-BX (F6, Astra review finding 6 - MED): WHOSE INDEX SPACE IS THIS?
        //The model answers over the DISPLAYED rows and must be permuted back; the
        //heuristic (`AIPlayerBaka::selectMenuOption`) and the clamp already answer
        //in the ENGINE's own index space, and permuting them a second time
        //committed a different name than the fallback chose (engine names [A, B]
        //displayed as [B, A], endpoint down: the heuristic picked A and the
        //engine was handed B). Only the model's answer is permuted.
        bool pickIsDisplayIndex = true;
        if (pick < 0)
        {
            pick = AIPlayerBaka::selectMenuOption(); //heuristic: ENGINE index space
            pickIsDisplayIndex = false;
        }
        if (pick < 0 || pick >= (int) req.optionTexts.size())
        {
            pick = 0;
            pickIsDisplayIndex = false;
        }
        //#W72-BV (M7): map the DISPLAYED row back to the engine's own index.
        pick = w72NameMenuEngineIndex(pick, pickIsDisplayIndex, nameOrder); //#W72-BX (F6)
        act.choice = pick;
        return 0;
    }

    //CHOOSE_MENU
    vector<string> opts = req.optionTexts;
    //#W48 D6: engine player tokens -> the players the reader knows. Presentation
    //only - req.optionTexts (the staleness key) and the option order are
    //untouched, so the answer index still means what applyMenuChoice thinks.
    for (size_t li = 0; li < opts.size(); li++)
        opts[li] = playerBranchLabel(opts[li]);
    if (req.canDecline)
        opts.push_back("Decline - do nothing");
    //One real option and no way to decline: only one outcome, no model call.
    if (opts.size() == 1)
    {
        act.choice = 0;
        return 0;
    }

    //#W75-CI (P18): THIS FACE WAS ALREADY CHOSEN, ONE WINDOW AGO. The land-drop
    //menu now lists each face as its own row, so by the time the engine puts the
    //card's own play menu the model has already answered this exact question -
    //re-asking it is the wave-73/74 double-ask (deck146 MED 3: 9 repro pairs,
    //18 of 322 decisions), and the second menu's decline row could un-make the
    //drop the first answer committed. The carry is scoped hard: the same card
    //pointer, the same turn, a menu that really is the two-face menu (it must
    //carry BOTH the back face's own row and a plain land-play row), and it is
    //consumed once. Anything that does not match falls through and is ASKED, as
    //before - nothing is auto-answered that the model did not already decide.
    if (ctx && ctx == mLandFacePreCard && mLandFacePreTurn == observer->turn
        && !mLandFacePreBackName.empty())
    {
        const string backLc = toLowerCopy(mLandFacePreBackName);
        int backRow = -1, frontRow = -1;
        for (size_t fi = 0; fi < req.optionTexts.size(); fi++)
        {
            const string lc = toLowerCopy(req.optionTexts[fi]);
            if (backRow < 0 && lc.find(backLc) != string::npos)
                backRow = (int) fi;
            else if (frontRow < 0 && lc.find("play land") != string::npos)
                frontRow = (int) fi;
        }
        if (backRow >= 0 && frontRow >= 0)
        {
            const int want = mLandFacePreBack ? backRow : frontRow;
            DebugTrace("AIPlayerGPT: the face menu for " << ctx->getDisplayName()
                       << " is answered from the land-drop row the model already took ("
                       << (mLandFacePreBack ? "back" : "front") << " face, engine row "
                       << want << " of " << req.optionTexts.size()
                       << ") - no second window, no second model call");
            mLandFacePreCard = NULL;
            mLandFacePreBackName.clear();
            mLandFacePreTurn = -1;
            act.choice = want;
            return 0;
        }
    }

    //#W41-14 LABEL TRUTH. A may-gain-life option arrives from the engine as the
    //bare verb "Life": no magnitude, no source, and the identical string for
    //ANY life-granting may. The deciding fact must ride the option (P1/P2/P4),
    //and the pilot was spending a median 5,702 reasoning characters per ask
    //re-deriving "2" off the battlefield line. The magnitude is evaluated at
    //build time (DecisionRequest::mayEffectLabel); name the source with it.
    //Presentation only - req.optionTexts is the staleness key and is untouched,
    //and the option COUNT and ORDER are unchanged, so the answer index still
    //means what applyMenuChoice thinks it means.
    string mayBatchSourceName;
    if (ctx)
    {
        mayBatchSourceName = ctx->getDisplayName();
        if (mayBatchSourceName.empty() && ctx->model && ctx->model->data)
            mayBatchSourceName = ctx->model->data->getName();
    }
    if (!req.mayEffectLabel.empty() && !opts.empty())
        opts[0] = mayBatchSourceName.empty()
                      ? req.mayEffectLabel
                      : (req.mayEffectLabel + " with " + mayBatchSourceName);

    //#W41-7 MASS ACCEPT / DENY. See AIPlayerGPT.h for the measured bill. The
    //engine pushes every sibling trigger onto the stack before resolving the
    //first, so the size of the window is known at the FIRST ask; the contract's
    //inspector counts only siblings it can positively verify identical (same
    //source, same controller, same rendered effect, no per-instance targets,
    //no per-instance condition). Anything it cannot verify never joins a batch
    //and keeps its own ask.
    DecisionManager::MayBatch batch;
    bool batchOffered = DecisionManager::inspectMayBatch(this, req, batch);
    bool inGroup = !batch.groupKey.empty();
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
    //dev-only probe (WAGIC_MAYPROBE=1): the batch verdict bookkeeping. This is
    //what caught the instance-vs-class keying bug during wave-41 validation.
    //Compiled out of release builds, per the project's diagnostics convention.
    if (getenv("WAGIC_MAYPROBE"))
        fprintf(stderr, "MAYPROBE seat offered=%d key='%s' held='%s' verdict=%d rem=%d total=%d\n",
                (int) batchOffered, batch.groupKey.c_str(), mMayBatchKey.c_str(),
                mMayBatchVerdict, mMayBatchRemaining, batch.totalInWindow);
#endif //_DEBUG || WAGIC_DEVLOGS
    //A verdict from earlier in this same window answers without a model call.
    if (inGroup && mMayBatchVerdict != kMayBatchNone && mMayBatchKey == batch.groupKey
        && mMayBatchRemaining > 0)
    {
        int verdict = mMayBatchVerdict;
        if (--mMayBatchRemaining <= 0)
        {
            mMayBatchVerdict = kMayBatchNone;
            mMayBatchKey.clear();
        }
        if (verdict == kMayBatchAcceptAll)
        {
            act.choice = 0;
            return 0;
        }
        if (verdict == kMayBatchDeclineAll)
        {
            act.choice = req.canDecline ? -1 : 0;
            return 0;
        }
        //kMayBatchIndividual: fall through to the normal single ask below.
    }
    else if (batchOffered)
    {
        //Ask once for the whole window. "Decide individually" is ALWAYS the
        //third answer, so the batched question can never make an answer
        //impossible - it falls back to exactly today's per-instance flow.
        std::ostringstream head;
        head << (mayBatchSourceName.empty() ? string("This trigger") : mayBatchSourceName)
             << " has triggered " << batch.totalInWindow << " times in this window, and every"
             << " one of them is the SAME choice with no targets to pick. Answer all "
             << batch.totalInWindow << " at once, or take them one at a time:";
        string effect = req.mayEffectLabel.empty() ? opts[0] : req.mayEffectLabel;
        std::ostringstream acceptAll, declineAll, individually;
        acceptAll << effect << " - accept ALL " << batch.totalInWindow << " triggers";
        declineAll << "Decline ALL " << batch.totalInWindow << " triggers - do nothing";
        individually << "Decide each of the " << batch.totalInWindow
                     << " triggers separately (you will be asked once per trigger)";
        vector<string> batchOpts;
        batchOpts.push_back(acceptAll.str());
        batchOpts.push_back(declineAll.str());
        batchOpts.push_back(individually.str());
        {
            //One narration line for the whole window - not N identical echoes.
            vector<string> narr;
            std::ostringstream a, d, s;
            a << "You accepted " << effect << " for all " << batch.totalInWindow
              << " triggers of " << (mayBatchSourceName.empty() ? string("that ability") : mayBatchSourceName);
            d << "You declined all " << batch.totalInWindow << " triggers of "
              << (mayBatchSourceName.empty() ? string("that ability") : mayBatchSourceName);
            s << "You chose to answer each of the " << batch.totalInWindow << " triggers separately";
            narr.push_back(a.str());
            narr.push_back(d.str());
            narr.push_back(s.str());
            setAskNarration(narr);
        }
        int bpick = askModel(head.str(), batchOpts, true, mayBatchSourceName);
        if (bpick == kChoicePending)
            return kChoicePending;
        if (bpick >= 0 && bpick <= 2)
        {
            mMayBatchKey = batch.groupKey;
            mMayBatchRemaining = batch.totalInWindow;
            if (bpick == 0)
            {
                mMayBatchVerdict = kMayBatchAcceptAll;
                mMayBatchRemaining--;
                act.choice = 0;
                return 0;
            }
            if (bpick == 1)
            {
                mMayBatchVerdict = kMayBatchDeclineAll;
                mMayBatchRemaining--;
                act.choice = req.canDecline ? -1 : 0;
                return 0;
            }
            mMayBatchVerdict = kMayBatchIndividual;
            mMayBatchRemaining--;
            //fall through: this instance gets the normal single ask
        }
        //model failure: no verdict recorded, normal per-instance flow below
    }
    //ENGINE-R6 (wave-21 deck135): a Transform/flip menu option is only ever
    //offered when its gating condition is already satisfied - the engine arms
    //the flip only when it is legal (Search for Azcanta's "7+ cards in
    //graveyard" upkeep trigger only presents the transform once the threshold
    //is met). Without that fact riding the option the model fills the vacuum
    //with an error-prone manual recount and declines an available transform
    //(vs14 seq 29/40: recounted its graveyard, got 5-6, Declined a legal flip
    //twice, losing the grind). The deciding fact rides the option. Annotation
    //only - the option stays legal and pickable; req.optionTexts (the
    //staleness key) is untouched, and only trailing text is appended so the
    //answer index is unchanged.
    for (size_t i = 0; i < opts.size(); i++)
        if (opts[i].compare(0, 10, "Transform:") == 0)
            opts[i] += " [available NOW - this transform is only offered because its"
                       " condition is already met; do not recount, it is legal this instant]";
    //W41-14: AALifer::getMenuText renders EVERY life-granting "may" as the bare
    //label "Life" (or "Life Loss") - no amount, no source. The deciding fact was
    //not on the option, and the pilot re-derived the magnitude off the
    //battlefield line at a median 5,702 reasoning chars per ask. Recover it from
    //the ability's own life: expression, evaluated on the live board with the
    //same WParsedInt the resolution uses, and name the card that raised the ask.
    //Omit rather than guess: a script carrying more than one life: clause cannot
    //be attributed to THIS option, a "rand" expression is never evaluated (it
    //would draw the game RNG), and a trigger-event amount ("thatmuch") is 0 here
    //(W41-10) - each of those keeps the bare label plus the card text the option
    //already sits beside.
    if (ctx)
    {
        string low = ctx->magicText;
        for (size_t li = 0; li < low.size(); li++)
            low[li] = (char) tolower((unsigned char) low[li]);
        size_t first = low.find("life:");
        bool unique = (first != string::npos && low.find("life:", first + 5) == string::npos);
        for (size_t i = 0; unique && i < opts.size(); i++)
        {
            bool loss = (opts[i] == "Life Loss");
            if (!loss && opts[i] != "Life")
                continue;
            size_t s = first + 5;
            size_t e = low.find_first_of(" \t\r\n", s);
            string expr = low.substr(s, e == string::npos ? string::npos : e - s);
            if (expr.empty() || expr.find("rand") != string::npos
                || expr.find("thatmuch") != string::npos)
                continue;
            bool numeric = true;
            for (size_t k = (expr[0] == '-' || expr[0] == '+') ? 1 : 0; k < expr.size(); k++)
                if (!isdigit((unsigned char) expr[k]))
                {
                    numeric = false;
                    break;
                }
            int amount;
            if (numeric)
                amount = atoi(expr.c_str());
            else
            {
                WParsedInt v(expr, NULL, ctx);
                amount = v.getValue();
            }
            opts[i] += lifeMenuLabelTag(abs(amount), !loss, ctx->getDisplayName(),
                                        cardTextSnippet(ctx, 140));
        }
    }
    //Modal-DFC "Flip Side" in the CLICK menu (deck199 Tergrid, observed live at
    //g1 seq21: "1. Cast Card Normally / 2. Flip Side / 3. Decline"). AATurnSide::
    //getMenuText renders it as a bare "Flip Side"; AATurnSide::resolve swaps the
    //hand card's displayed face (name/cost/P/T) but casts nothing, uses no stack,
    //and moves no zone - and BOTH faces stay castable from the Cast menu (the back
    //via its alternative cost), so it is a game no-op for the AI (the sanctioned
    //mechanically-no-op UI dead-end class). It is a real face-data mutation, not a
    //pure display redraw, so per the suppression rule it is ANNOTATED, not filtered
    //- and this keeps the CHOOSE_MENU seat CONSISTENT with the priority seam, which
    //already annotates the same pseudo-action "DISPLAY TOGGLE only" in describeAction.
    //Append-only text: the answer index and the req.optionTexts staleness key are
    //untouched.
    //A "Play Land" option sitting beside "Flip Side" means this IS the modal-DFC
    //LAND play menu (defaultPlayName is "Play Land" only for a land). deck152
    //N-152a: the old "the Cast menu offers the other face" text was a LIE here
    //(a land has no Cast menu). Name both faces + colors so the pilot knows the
    //other color exists. #W56-D (D8): its successor claim - "only the
    //currently-shown face can actually be played as a land" - is retired with
    //the primitives' isflipped gate; the back face now has its OWN row on this
    //same menu (the `{0}` flip-into-play entry), annotated just below. The
    //toggle is still unnecessary, but for the opposite reason. The non-land
    //(spell) DFC keeps the true-no-op text.
    bool landFaceMenu = false;
    for (size_t i = 0; i < opts.size() && !landFaceMenu; i++)
        if (opts[i] == "Play Land")
            landFaceMenu = true;
    string curFace, curMana, othFace, othMana;
    if (landFaceMenu && ctx && ctx->isLand())
    {
        curFace = ctx->getDisplayName();
        curMana = landTapMana(ctx->text);
        othFace = dfcOtherFaceName(ctx->text);
        if (!othFace.empty())
        {
            MTGCard * oc = MTGCollection()->getCardByName(othFace, ctx->setId);
            if (oc && oc->data && oc->data->isLand())
                othMana = landTapMana(oc->data->text);
            else
                othFace = ""; //other face is not a land - not the Pathway case
        }
    }
    for (size_t i = 0; i < opts.size(); i++)
        if (opts[i] == "Flip Side")
        {
            if (landFaceMenu && !othFace.empty())
            {
                opts[i] += " [DISPLAY TOGGLE only - this is a modal double-faced land."
                           " It currently shows \"" + curFace + "\"";
                if (!curMana.empty())
                    opts[i] += " (taps for " + curMana + ")";
                opts[i] += "; its other face is \"" + othFace + "\"";
                if (!othMana.empty())
                    opts[i] += " (taps for " + othMana + ")";
                opts[i] += ". Flipping only changes which face is DISPLAYED; it casts"
                           " nothing and uses no stack, and you do not need it: \"Play"
                           " Land\" plays the face shown and the row named \"" + othFace
                         + "\" plays that face as a land instead. Either row is one land"
                           " drop.]";
            }
            //N-152e sibling path (the path-scoped sweep): the CHOOSE_MENU seat
            //carried the SAME false "the Cast menu offers the other face as an
            //alternative-cost cast" promise that describeAction did, and it is
            //equally false for a TRANSFORMING DFC - that card is never cast as
            //its other face. The engine's own discriminator is the backSide
            //field (present on both faces of a transform DFC, absent on a modal
            //DFC spell, which declares its other face via other=/doubleside).
            else if (ctx && !ctx->backSide.empty())
            {
                //Same helper as describeAction's branch: one string, two
                //emitters, no drift. Wrapped in [] like every other menu tail.
                opts[i] += " ["
                    + transformDfcToggleNote(ctx->getDisplayName(), ctx->backSide, string())
                        .substr(4) //drop the leading " -> " arrow; this is a bracket tail
                    + "]";
            }
            //#W57-D (D14, live probe 20260903 seq 10): a modal-DFC SPELL whose
            //other face is a LAND reaches the CAST-MODE menu (`Cast Card
            //Normally / Flip Side / Decline`) with no "Play Land" row and no
            //backSide field, so it fell into the generic tail below - which
            //told the model "the Cast menu ... offers the other face as an
            //alternative-cost cast". For a LAND back face that is false at
            //every cost: a land is never cast. This is the same species of lie
            //#W56-D had to correct three times, one string per wave; say what
            //is true instead. The row is still OFFERED (this menu carries no
            //land row to duplicate, so D14's suppression does not apply here).
            else if (mdfcOtherFaceIsLand(ctx))
            {
                string ln = dfcOtherFaceName(ctx->text);
                opts[i] += " [DISPLAY TOGGLE only - this card's other face, \"" + ln
                         + "\", is a LAND: it is never CAST, at any cost, so flipping"
                           " puts nothing on this menu. That face is played as a LAND"
                           " DROP from its own row, which appears while a land drop is"
                           " still legal for you this turn - it is not on this menu now.]";
            }
            else
                opts[i] += " [display toggle only - no game effect: switches which face this"
                           " hand card shows; it casts nothing and uses no stack. The Cast menu"
                           " is where you cast, and it offers the other face as an"
                           " alternative-cost cast - you do not need this.]";
        }
    //#W59-I (K7, wave-58 deck146 HIGH-2): the two land-drop rows on this one
    //menu were rendered by different emitters, so the BACK face carried the
    //full wave-57 treatment and the FRONT face rendered as a bare "Play Land".
    //`146v123` seq 3, turn 1, verbatim:
    //  1. Grimclimb Pathway [PLAY THIS AS A LAND: puts "Grimclimb Pathway" onto
    //     the battlefield as a land (taps for {B}). ... USES YOUR LAND DROP ...]
    //  2. Play Land
    //  3. Decline - do nothing
    //Row 2 IS Brightclimb Pathway, which taps for {W}: no name, no colour, no
    //land-drop note, on a row taken 13 times across the corpus's 32 renders of
    //it. A rendered row that names nothing is the silent-omission half of the
    //trust doctrine - the model confabulates into the gap, and here the gap is
    //WHICH LAND it is playing. Same emitter as the sibling row (mdfcRowHead +
    //mdfcLandPlayRowTag + the D26 arrival clause), so the two halves of one
    //menu cannot drift again. Facts are read off the card the row actually
    //plays: the shown face by name, falling back to this hand card's own
    //script. Append-only text - the engine's option vector, the answer INDEX
    //and req.optionTexts (the staleness key) are untouched, and the "Play Land"
    //token stays in the row head so a bare-token echo still binds.
    //`curFace` is non-empty only on the land-face menu (landFaceMenu &&
    //ctx->isLand()), which is this shape and no other.
    if (!curFace.empty())
        for (size_t i = 0; i < opts.size(); i++)
        {
            const string raw = (req.optionTexts.size() > i) ? req.optionTexts[i] : opts[i];
            if (raw != "Play Land")
                continue;
            string faceMana = curMana, faceArrival;
            int flife = 0;
            MTGCard * fc = MTGCollection()->getCardByName(curFace, ctx->setId);
            if (fc && fc->data && fc->data->isLand())
            {
                if (faceMana.empty())
                    faceMana = landTapMana(fc->data->text);
                faceArrival = mdfcLandArrivalTagFrom(fc->data->magicText,
                                                     fc->data->text); //#W67-AW (M3)
            }
            else
                faceArrival = mdfcLandArrivalTagFrom(ctx->magicText,
                                                     ctx->text); //#W67-AW (M3)
            //Anything an earlier pass appended to this row is preserved: only
            //the head is rewritten and the tag is appended after it.
            const string extra = (opts[i].compare(0, raw.size(), raw) == 0)
                                     ? opts[i].substr(raw.size()) : string();
            opts[i] = mdfcLandFaceRow(raw, curFace, faceMana, faceArrival);
            if (!extra.empty())
                opts[i] += extra;
        }
    //#W56-D (D8): the OTHER row on that same click menu - the `{0}` entry that
    //puts the card's LAND back face onto the battlefield. The engine labels it
    //with nothing but the back face's NAME, which reads like a card reference
    //rather than a land drop; deck146 never took it because it was never
    //offered at all (the primitives' isflipped gate), and once offered a bare
    //name is not enough to act on. Same pure tag as the priority seam's row, in
    //the bracket form every other menu tail uses. Append-only: the answer index
    //and the req.optionTexts staleness key are untouched. This branch is
    //independent of the Pathway "Play Land" shape above - a modal-DFC SPELL
    //with a land back face (Emeria's Call) has no Play Land row at all.
    bool mdfcLandRowShown = false; //#W57-D (D14): the D8 row is ON this menu
    if (ctx)
    {
        string landBack = dfcOtherFaceName(ctx->text), landBackMana, landArrival;
        if (!landBack.empty())
        {
            MTGCard * lbc = MTGCollection()->getCardByName(landBack, ctx->setId);
            if (lbc && lbc->data && lbc->data->isLand())
            {
                landBackMana = landTapMana(lbc->data->text);
                //#W57-D (D26): same clause as the priority seam, same helper.
                landArrival = mdfcLandArrivalTagFrom(lbc->data->magicText,
                                                     lbc->data->text); //#W67-AW (M3)
                if (!lbc->data->name.empty())
                    landBack = lbc->data->name;
            }
            else
                landBack = "";
        }
        if (!landBack.empty())
            for (size_t i = 0; i < opts.size(); i++)
            {
                //the engine's menu label for this row is the SCRIPT token, which
                //is lowercased; dfcOtherFaceName reads the printed `text=` line
                //and is not. Compare case-insensitively or the tail never fires.
                string raw = (req.optionTexts.size() > i) ? req.optionTexts[i] : opts[i];
                string lraw = raw, lback = landBack;
                std::transform(lraw.begin(), lraw.end(), lraw.begin(), ::tolower);
                std::transform(lback.begin(), lback.end(), lback.begin(), ::tolower);
                if (lraw != lback)
                    continue;
                //#W57-D (D28): lead with the printed name, keep the engine's own
                //menu text second. The ANSWER INDEX and req.optionTexts (the
                //staleness key) are untouched - only the shown text changes.
                mdfcLandRowShown = true; //#W57-D (D14)
                if (opts[i].compare(0, raw.size(), raw) == 0)
                    opts[i] = mdfcRowHead(landBack, raw) + opts[i].substr(raw.size());
                opts[i] += " ["
                    + mdfcLandPlayRowTag(landBack, landBackMana, ctx->getDisplayName(),
                                         landArrival, ctx->isLand() != 0).substr(4) //#W73-CA (N14)
                    + "]";
            }
    }
    //#W47 R2: the cast-mode menu ("Cast Card Normally" beside one or more
    //alternative/hand paths). Give every row its own cost and its own printed
    //text, read off the card itself - see castModeRowTag above for the two lost
    //games. Self-gating on the engine's own "Cast Card Normally" label, so no
    //other menu shape is touched; append-only, so the answer index and the
    //req.optionTexts staleness key are unchanged.
    bool castModeMenu = false;
    for (size_t i = 0; i < opts.size() && !castModeMenu; i++)
        if (req.optionTexts.size() > i && req.optionTexts[i] == "Cast Card Normally")
            castModeMenu = true;
    if (castModeMenu && ctx)
    {
        //The hand-zone script carries the alternative paths (cycling and its
        //siblings live in autohand=); the battlefield script carries the rest.
        string handScript;
        for (map<string, string>::const_iterator mi = ctx->magicTexts.begin();
             mi != ctx->magicTexts.end(); ++mi)
            handScript += (handScript.empty() ? "" : "\n") + mi->second;
        //The draw punishers standing right now, so a path that DRAWS prices
        //that too (#W47 R1's row half, on the menu that offers the draw).
        vector<string> mineP, theirsP;
        int minePer = 0, theirsPer = 0;
        drawPunisherScan(this, opponent(), mineP, minePer, theirsP, theirsPer);
        std::ostringstream pnames;
        for (size_t ni = 0; ni < theirsP.size(); ni++)
            pnames << (ni ? ", " : "") << theirsP[ni];
        //#W57-C (D21): the facts each direction needs about the other, gathered
        //in the pass that already computes them.
        int castRowIdx = -1, castTargets = -1;
        string castCostStr, castClauseStr;
        vector<size_t> altIdx;
        vector<string> altLabels, altCosts;
        vector<int> altDraws;
        for (size_t i = 0; i < opts.size() && i < req.optionTexts.size(); i++)
        {
            const string& label = req.optionTexts[i];
            if (label == "Flip Side")
                continue; //already annotated above, with its own truth rails
            if (label == "Cast Card Normally")
            {
                string cost;
                if (ctx->model && ctx->model->data && ctx->model->data->getManaCost())
                    cost = ctx->model->data->getManaCost()->toString();
                else if (ctx->getManaCost())
                    cost = ctx->getManaCost()->toString();
                opts[i] += castModeRowTag(cost, printedFirstClause(ctx->text));
                //#W66-AT (deck130 MED): the cast row's own live verdict, on the
                //row that casts. Silent for every card the predicates cannot
                //price, exactly as on the cast menu.
                int m11Gain = 0, m11Turns = 0; //#W72-BW (M11)
                oppLifeRise(m11Gain, m11Turns);
                opts[i] += castKillVerdictNow(observer, this, ctx, m11Gain, m11Turns);
                castRowIdx = (int) i; //#W57-C (D21)
                castCostStr = cost;
                castClauseStr = printedFirstClause(ctx->text);
                {
                    //The same chooser count every other row gets, so the cast
                    //side of the comparison carries a NUMBER and not only prose.
                    string spec = ctx->spellTargetType;
                    if (!spec.empty() && toLowerCopy(spec).find("stack") == string::npos)
                        castTargets = modalSpecObjectCount(observer, ctx, spec);
                }
                continue;
            }
            string costTokens = scriptAbilityCost(handScript, label);
            if (costTokens.empty())
                costTokens = scriptAbilityCost(ctx->magicText, label);
            string tag = castModeRowTag(scriptCostGloss(costTokens),
                                        printedClauseFor(ctx->text, label));
            if (tag.empty())
                continue;
            opts[i] += tag;
            int cards = scriptAbilityDrawCount(handScript, label)
                        + scriptAbilityDrawCount(ctx->magicText, label);
            opts[i] += drawPriceRowTag(cards, theirsPer, pnames.str(), life); //#W54-C (D10)
            altIdx.push_back(i); //#W57-C (D21)
            altLabels.push_back(label);
            altCosts.push_back(scriptCostGloss(costTokens));
            altDraws.push_back(cards);
        }
        //#W57-C (D21): cross-price the two directions. Append-only, so the
        //answer index and req.optionTexts (the staleness key) are unchanged.
        {
            const string castTag = castModeCastPriceTag(castCostStr, castClauseStr, castTargets);
            if (!castTag.empty())
                for (size_t k = 0; k < altIdx.size(); k++)
                    opts[altIdx[k]] += castTag;
            if (castRowIdx >= 0)
                opts[castRowIdx] += castModeAltPriceTag(altLabels, altCosts, altDraws);
        }
    }
    //Dungeon room-BRANCH menu (deck146 N-146b): advancing WITHIN a dungeon offers
    //the next room as a bare name ("Veils of Fear" / "Oubliette") with ZERO effect
    //text - the deciding fact for the venture path is absent (standing P1/P4), and
    //option-1 bias then steers navigation. The active dungeon in the command zone
    //carries every room's readable effect in its text=; attach each branch option's
    //room effect. Self-gating: only options whose text matches a room name of the
    //active dungeon are touched, so every other bare-name menu is untouched.
    //Append-only - the answer index and req.optionTexts (staleness key) are unchanged.
    //N-146n: the effect text alone still left the model unable to say WHERE it
    //was. Add the position of each offered room in the dungeon's printed room
    //list, and (below) replace the generic header with one that names the
    //dungeon, the venture step, and the branching. Still append-only per option:
    //the answer index and req.optionTexts (the staleness key) are unchanged.
    string dungeonRoomHeader;
    if (MTGCardInstance * dng = activeDungeon(game))
    {
        std::vector<std::pair<string, string> > rooms;
        parseDungeonRooms(dng->text, rooms);
        string dngName = dng->getDisplayName();
        bool anyRoomOption = false;
        for (size_t i = 0; i < opts.size(); i++)
            for (size_t r = 0; r < rooms.size(); r++)
                if (ciStartsWith(opts[i], rooms[r].first))
                {
                    opts[i] += dungeonRoomPositionTag(dngName, r, rooms.size());
                    opts[i] += " {room effect: " + rooms[r].second + "}";
                    anyRoomOption = true;
                    break;
                }
        if (anyRoomOption)
        {
            int explores = 0;
            if (dng->counters)
                if (Counter * ec = dng->counters->hasCounter("Explore", 0, 0))
                    explores = ec->nb;
            dungeonRoomHeader = dungeonRoomBranchHeader(dngName, explores, rooms);
        }
    }
    //N-139a (wave-29 deck139): the mutate OVER/UNDER placement menu ("Mutate Over"/
    //"Mutate Under") and the cast-mode sub-menu's "mutate" option reached the model
    //as bare labels under the generic "Choose an option" header, asked mid-sequence
    //(before the host is chosen), so the model could not tell it was the mutate
    //placement step and spent minutes re-deriving. Name the role: annotate each
    //placement option with what over/under does, flag the menu for a role header
    //below, and annotate the cast-mode "mutate" option. Self-gating on the exact
    //labels; append-only (answer index + staleness key unchanged).
    bool mutatePlaceMenu = false;
    for (size_t i = 0; i < opts.size(); i++)
    {
        //W38: detection is case-insensitive - the parser lowercases card
        //scripts, so live labels read "mutate over"/"mutate under" and the
        //old capitalized-only compare missed them (wave-37 139v152 s30).
        int mpl = mutatePlacementFromLabel(opts[i]);
        if (mpl == 1)
        {
            mutatePlaceMenu = true;
            opts[i] += " [this card goes ON TOP: the merged creature keeps THIS card's"
                       " name and P/T, and ALSO gains the host's abilities]";
        }
        else if (mpl == 2)
        {
            mutatePlaceMenu = true;
            opts[i] += " [this card goes UNDERNEATH: the creature on top keeps its name"
                       " and P/T, and gains THIS card's abilities]";
        }
        else if (opts[i] == "mutate" || opts[i] == "Mutate")
            opts[i] += " [cast for the MUTATE cost: merge onto one of your non-Human"
                       " creatures (you pick over/under, then the host) instead of"
                       " casting a fresh separate body]";
    }
    //ETB "pay life or enter tapped" menu (shocklands - Temple Garden class, and
    //the fixed painland sibling): append the consequence each bare label omits,
    //and name the entering land in the option short-names (W36 #7 - the name is
    //recovered from the battlefield, see etbPayOrTapLandName; the menu's own
    //instance is nameless, w26a probe). deck137 wave-24 + deck139 wave-35.
    //(history: comment-archaeology.md AIPlayerGPTSeams-L8145-2139)
    string etbLandName;
    bool etbPayOrTap = false;
    {
        vector<string> probeOpts = opts;
        bool etbAlreadyTapped = false;
        if (annotateEtbPayOrTapMenu(probeOpts, string()))
        {
            //W37 #1: name AND tapped state come from the recovered battlefield
            //land - the menu's own ctx is a stripped copy whose isTapped() is
            //always false (that read left the wave-36 truth fix inert live).
            MTGCardInstance * entered = etbPayOrTapLand(this);
            if (entered)
            {
                etbLandName = entered->getDisplayName();
                etbAlreadyTapped = entered->isTapped();
            }
            else
                etbAlreadyTapped = ctx && ctx->isTapped();
        }
        //#W76-CP (Q14): the two board facts the usable bracket is allowed to
        //claim, read off the permanent that armed the menu. When the land could
        //not be recovered the wave-58 wording stands (both true), which is the
        //same conservative fallback the NAME already uses.
        bool etbCanTapForMana = true, etbCanAttackEver = true;
        if (MTGCardInstance * arrived = etbPayOrTapLand(this))
        {
            etbCanTapForMana = arrived->data
                               && !landTapMana(arrived->data->text).empty();
            etbCanAttackEver = arrived->isCreature() != 0 || permanentCanAnimate(arrived);
        }
        etbPayOrTap = annotateEtbPayOrTapMenu(opts, etbLandName, etbAlreadyTapped,
                                              etbCanTapForMana, etbCanAttackEver);
        //#W82-A (L1, audit-2026-09): the auto-click of the `tap` row on an
        //already-tapped ETB pay-or-tap menu is DELETED. Paying life IS losing
        //life (CR 119.4: "the payment is subtracted from their life total; in
        //other words, the player loses that much life"), so a "whenever you
        //lose life" board makes the payment a real play even when the untap it
        //would have bought is already moot. The ANNOTATION above stays - the
        //two board facts are rendered on the rows and the model decides.
    }
    //#W58-B (D1): every life-payment row on this menu carries the subtraction,
    //appended LAST so the option short name (and the echo anchor) is untouched
    //and the brace channel keeps it out of the narrated record.
    std::vector<bool> rowAlreadyLifePriced(opts.size(), false);
    for (size_t li = 0; li < opts.size(); li++)
    {
        int lifeCostRow = 0;
        if (payLifeAmountFromOption(opts[li], lifeCostRow))
        {
            opts[li] += lifePaymentVerdict(life, lifeCostRow);
            rowAlreadyLifePriced[li] = true; //#W65-AL (G1): priced once, not twice
        }
    }
    //#W65-AL (G1, engine HIGH-1 / deck146 HIGH-3): THE SEAM THE CORPUS
    //EXERCISES. A modal `auto=choice name(...)` list arms its menu while the
    //spell RESOLVES, so it has no currentActionCard and DecisionManager's
    //multiple-choice branch is never entered - the rows arrive HERE, as
    //CHOOSE_MENU under "Choose an option for <card>:", which is the header the
    //wave-64 records print. The wave-60 tap/untap tag and wave-64's F6 price
    //tag were wired to the CHOOSE_MODE branch only, so 23 real Silverquill
    //Command mode rows over 3 windows rendered BARE and the seat took
    //`draw:1 controller && life:-1 controller && ability$!...!$ opponent` at 5
    //life and again at 3 (146v130 seq 57/74) and lost that game at 0.
    //The same function the mode branch calls, over the same script (the
    //arm-time snapshot when the pointer is gone), matched against the ENGINE's
    //own labels: a row whose label is not a `name(...)` in that script matches
    //nothing and is left exactly as the engine wrote it, so this is silent on
    //every menu that is not modal. Presentation only - req.optionTexts (the
    //staleness key), the option ORDER and the answer index are untouched.
    {
        //#W71-BR (L15, deck162 MED): the `auto=` bucket is not the whole card.
        //A dungeon's rooms live in `autocommandzone=`, i.e. in the magicTexts
        //map, so `162v146` seq 21 offered "1. lose 2 life / 2. sacrifice cards"
        //for Tomb of Annihilation with NO annotation on either row while the
        //same seat's seq 16 (Annihilation Rooms, an `auto=` card) priced the
        //identical "lose 2 life" mode - the label matched nothing because the
        //script the match ran over did not contain the modes. Same rail the
        //cast-mode menu (`castModeMenu` above) and the ability-draw scan already
        //use: every zone script the subject carries, joined.
        const string menuScript = ctx ? scriptAllZones(ctx) : req.contextText;
        if (!menuScript.empty())
        {
            int canBlockTapped = 0, doNotUntap = 0;
            modeTapCounts(opponent(), canBlockTapped, doNotUntap);
            //#W66-AQ (H9, deck146 HIGH-1/2). Two facts the cast row one window
            //earlier already carries, and this ask - the one the seat answers -
            //did not. (a) The live/dead/half-dead CENSUS: `146v162` seq 23's
            //cast row named 7 of Silverquill Command's 9 pairs dead, and seq 24
            //offered rows 1 and 4 with no tag at all and priced rows 5 and 6 -
            //both HALF dead - as whole modes; the seat took row 6 and lost the
            //game. The subject is in no zone by now (ctx is NULL here), so it
            //is recovered by matching the arm-time script. (b) The opponent's
            //DRAW PUNISHER rate, which the cast row's DRAW GRANT bracket folds
            //and this ask did not: "you would be at 17" under Underworld Dreams
            //is 16.
            vector<string> modeMineP, modeTheirsP;
            int modeMinePer = 0, modeTheirsPer = 0;
            drawPunisherScan(this, opponent(), modeMineP, modeMinePer,
                             modeTheirsP, modeTheirsPer);
            std::vector<std::string> liveM, halfM, deadM;
            bool haveCensus = false;
            {
                MTGCardInstance * subj = ctx ? ctx : modalSubjectFromScript(this, menuScript);
                haveCensus = modalModeLiveness(observer, subj, menuScript,
                                               liveM, halfM, deadM);
            }
            //#W68-BD (J8, deck146 HIGH-1): the EDICT floor on the mode rows
            //that carry one. The mode list is re-read from the same script the
            //census read; the floor numbers come from edictFloorScan, the one
            //walk the standalone edict row uses, so a Silverquill Command
            //sacrifice mode and a Tribute to Hunger row in the same game state
            //cannot name different counts.
            std::vector<GptModalMode> edictModes;
            int edTheirs = 0, edMinTough = 0, edAtFloor = 0;
            string edOnlyName;
            if (modalChoiceModes(menuScript, edictModes))
            {
                bool anyEdict = false;
                for (size_t ei = 0; ei < edictModes.size(); ei++)
                    if (edictModes[ei].subOpponentEdict)
                        anyEdict = true;
                if (anyEdict)
                {
                    MTGCardInstance * edOnly = NULL;
                    edictFloorScan(opponent(), edTheirs, edMinTough, edAtFloor, &edOnly);
                    if (edOnly)
                        edOnlyName = edOnly->getDisplayName();
                }
                else
                    edictModes.clear();
            }
            else
                edictModes.clear();
            for (size_t li = 0; li < opts.size() && li < req.optionTexts.size(); li++)
            {
                //A pay-life row (the shockland pay-or-tap class) has ALREADY
                //been priced by lifePaymentVerdict two loops above, off the same
                //`life:-N controller` clause. One price per row: a second one
                //saying the same thing in different words is noise on the row,
                //not a fact the model did not have.
                if (rowAlreadyLifePriced[li])
                    continue;
                opts[li] += modeRowAnnotations(menuScript, req.optionTexts[li], life,
                                               opponent() ? opponent()->life : 0,
                                               canBlockTapped, doNotUntap,
                                               modeTheirsPer); //#W66-AQ (H9)
                //#W68-BD (deck152 MED-4): a stage-1 side gate says how many
                //objects each side it opens can reach - "NONE" instead of an
                //unanswerable ask. The subject is the same pointer the census
                //uses; with no subject there is no TargetChooser and no claim.
                {
                    string gA, gSA, gB, gSB;
                    MTGCardInstance * gSubj = ctx ? ctx
                                                  : modalSubjectFromScript(this, menuScript);
                    if (gSubj && gateSideBranches(menuScript, req.optionTexts[li],
                                                  gA, gSA, gB, gSB))
                        opts[li] += gateSideCensusTag(
                            gA, modalSpecObjectCount(observer, gSubj, gSA),
                            gB, modalSpecObjectCount(observer, gSubj, gSB));
                }
                //#W68-BD (J8): the edict floor rides the row whose mode owns it.
                {
                    const string edLow = toLowerCopy(req.optionTexts[li]);
                    for (size_t ei = 0; ei < edictModes.size(); ei++)
                        if (edictModes[ei].subOpponentEdict
                            && toLowerCopy(edictModes[ei].label) == edLow)
                            opts[li] += modeEdictFloorTag(edTheirs, edOnlyName,
                                                          edMinTough, edAtFloor);
                }
                if (haveCensus) //#W66-AQ (H9)
                {
                    const string rowLow = toLowerCopy(req.optionTexts[li]);
                    for (size_t ci = 0; ci < liveM.size(); ci++)
                        if (toLowerCopy(liveM[ci]) == rowLow)
                            opts[li] += modeLivenessRowTag(0);
                    for (size_t ci = 0; ci < halfM.size(); ci++)
                        if (toLowerCopy(halfM[ci]) == rowLow)
                            opts[li] += modeLivenessRowTag(1);
                    for (size_t ci = 0; ci < deadM.size(); ci++)
                        if (toLowerCopy(deadM[ci]) == rowLow)
                            opts[li] += modeLivenessRowTag(2);
                }
            }
        }
    }
    //Recover the subject name for the header. Some ETB menus arm on an instance
    //whose own name is cleared (getDisplayName() empty) but whose card TEMPLATE
    //(model->data) still carries the printed name - recover it there. The
    //shockland pay-or-tap menu arms on a stripped copy with name cleared AND
    //model == NULL (verified w26a probe), so its name comes from the
    //battlefield recovery above instead.
    //#W47 (wave-46 E-1): the same three-rung ladder the narration uses one
    //tick later (display name -> raw instance name -> granting card), so an
    //ability$!!$ carrier that names itself only through c->name (Silverquill
    //Command's auto=choice menu) gets the "Choose an option for X:" header
    //instead of the subject-less fallback.
    string ctxName = resolveOwningCardName(ctx);
    if (ctxName.empty() && ctx && ctx->model && ctx->model->data)
        ctxName = ctx->model->data->getName();
    //#W48 D6: the last rung, and the only one that survives a menu armed on a
    //card the game can no longer validate (an `auto=choice` list resolving off
    //the stack - Silverquill Command, Peer into the Abyss). The contract
    //captured the subject's name while the object was alive; a pointer ladder
    //cannot reach it, a string can. See DecisionContract::buildMenuChoice.
    if (ctxName.empty())
        ctxName = req.contextName;
    if (ctxName.empty() && etbPayOrTap)
        ctxName = etbLandName;
    string decision;
    if (!req.mayObjectName.empty())
        //Triggered "may"-ability ask (Tergrid's steal class): name the object
        //and its origin so the subject-less card-script verb stops reading as
        //acting on the trigger source (w26 deck199 re-probe seq 21 - a free
        //3/3 declined). Representation only; the object name is stable within a
        //board state, so the ask cache (board-state + question) does not churn.
        decision = buildMayObjectAsk(ctxName, req.mayObjectName, req.mayObjectOrigin, opts);
    //When the subject name is unrecoverable, fall to a clean generic prompt
    //rather than the grammatically-broken bare "Choose an option for :" the old
    //code emitted (a dangling empty subject after "for", deck137 wave-25).
    else
        decision = !ctxName.empty() ? ("Choose an option for " + ctxName + ":")
                                    : string("A choice is required - choose an option:");
    //#W71-BR (L13, deck146 MED-1): the log narrates a modal spell as finished
    //BEFORE its mode is asked. `146v130` seq 20's GAME LOG ends "You cast
    //Silverquill Command / Your Silverquill Command resolved and went to your
    //graveyard" and the very next thing the prompt asks is "Choose an option for
    //Silverquill Command:" - the seat spent reasoning on the contradiction
    //("the log says it already resolved? That's weird... it's a prompt
    //artifact") before answering. Both statements are true of the ENGINE (the
    //card moves to the graveyard as it resolves; the mode payload then arms off
    //the stack, which is exactly why `ctx` is NULL here) and the pair reads as
    //a contradiction to a reader who is instructed to believe the screen. So
    //the ORDER is stated where the reader meets it. Scoped to the seam where it
    //is true: a subject the game can no longer point at.
    if (!ctx && !ctxName.empty())
        decision += modalResolutionOrderNote(ctxName);
    //N-139a: a mutate placement menu overrides the generic header with a role header.
    if (mutatePlaceMenu)
        decision = mutateOverUnderHeader(ctxName);
    //N-146n: a room-branch menu says which dungeon and which venture step it is.
    //The generic header ("Choose an option for Tomb of Annihilation:") named the
    //dungeon but nothing else, and the position is the fact the spirals were
    //re-deriving.
    if (!dungeonRoomHeader.empty())
        decision = dungeonRoomHeader;
    //N-152h + W37 #3: the live Add-N-counters pay-repeat menu (Intrepid
    //Adversary class) arms as CHOOSE_MENU, not CHOOSE_MODE - the wave-36
    //corpus ask reads "Choose an option for Intrepid Adversary: 1. don't add
    //any counter ...". State the per-counter partial-pay semantics here too;
    //detection runs on the clean engine labels (req.optionTexts), and the
    //note is header-only (options and the staleness key untouched).
    //(history: comment-archaeology.md AIPlayerGPTSeams-castModeCommitmentNote-2151)
    decision += castModeCommitmentNote(castModeMenu, ctxName, mCommittedCastName,
                                       mCommittedCastTurn == observer->turn);
    decision += payRepeatModeNote(req.optionTexts);
    //#W62-Y (D7, deck152 HIGH-3): the header's semantics, now with the row's own
    //NUMBER. Read off the card's own script (payRepeatPerCounterCost) and the
    //seat's spendable mana, and - where paying the affordable share draws on
    //CREATURES - the same `{paying this taps: ...}` clause every priced cast row
    //carries, which is the clause the guide's combat stop is built on.
    //Presentation only: req.optionTexts (the staleness key), the option ORDER
    //and the answer index are untouched.
    std::vector<int> repeatPaid(req.optionTexts.size(), -1); //#W67-AW (I9a)
    std::vector<size_t> repeatBase(opts.size(), (size_t) -1); //#W67-AZ (R4)
    {
        int addRows = 0;
        for (size_t i = 0; i < req.optionTexts.size(); i++)
            if (isAddNCountersOption(req.optionTexts[i]))
                addRows++;
        if (addRows >= 2 && ctx)
        {
            int avail = getManaPool() ? getManaPool()->getConvertedCost() : 0;
            {
                GptManaPolicy repeatPolicy(this);
                avail += ManaEngine::potentialColorReach(this, repeatPolicy, NULL);
            }
            //#W62-AA (R4): `avail` is a MAGNITUDE (pool + distinct untapped
            //sources) and stays the magnitude the row prints. What it must not
            //do any more is decide how many payments it covers - that is a
            //colour question, and the mana engine owns it (below).
            const int anyMana = ctx->has(Constants::ANYTYPEOFMANAABILITY);
            const bool beforeAttack = observer->currentPlayer == this
                && observer->getCurrentGamePhase() < MTG_PHASE_COMBATATTACKERS;
            const bool blockStillMatters = observer->currentPlayer == this
                || observer->getCurrentGamePhase() < MTG_PHASE_COMBATBLOCKERS;
            for (size_t i = 0; i < opts.size() && i < req.optionTexts.size(); i++)
            {
                if (!isAddNCountersOption(req.optionTexts[i]))
                    continue;
                const int n = atoi(req.optionTexts[i].c_str() + 4);
                const string per = payRepeatPerCounterCost(ctx->magicText, req.optionTexts[i]);
                const int perCmc = manaCostTextCmc(per);
                if (n <= 0 || per.empty() || perCmc <= 0)
                    continue; //a row this parse cannot account for keeps the engine's own text
                //#W62-AA (R4): the largest K whose K copies of `per` the mana
                //engine can actually plan a payment for, from the floating pool
                //and the untapped producers. -1 only when the cost text will
                //not parse, and then the row hedges instead of claiming.
                int paid = -1;
                {
                    int fits = 0;
                    bool parsed = true;
                    for (int k = 1; k <= n && parsed; k++)
                    {
                        ManaCost * probe = NULL;
                        for (int j = 0; j < k; j++)
                            probe = ManaCost::parseManaCost(per, probe, ctx);
                        if (!probe)
                        {
                            parsed = false;
                            break;
                        }
                        bool ok = getManaPool()->canAfford(probe, anyMana);
                        if (!ok)
                        {
                            GptManaPolicy fitPol(this);
                            ok = !ManaEngine::planPayment(this, fitPol, ctx, probe, anyMana).empty();
                        }
                        SAFE_DELETE(probe);
                        if (!ok)
                            break;
                        fits = k;
                    }
                    if (parsed)
                        paid = fits;
                }
                repeatPaid[i] = paid; //#W67-AW (I9a)
                if (i < repeatBase.size()) //#W67-AZ (R4): where this row's own text ends
                    repeatBase[i] = opts[i].size();
                opts[i] += payRepeatRowCostTag(n, per, perCmc, avail, paid);
                if (paid <= 0)
                    continue;
                ManaCost * bill = NULL;
                for (int k = 0; k < paid; k++)
                    bill = ManaCost::parseManaCost(per, bill, ctx);
                if (!bill)
                    continue;
                //Only where the FLOATING pool cannot already cover it - the same
                //guard the cast row's clause rides; mana already in the pool taps
                //nothing.
                if (!getManaPool()->canAfford(bill, anyMana))
                {
                    vector<MTGAbility*> picks = ManaEngine::selectAutoTapProducers(
                        this, ctx, bill, anyMana, false);
                    std::set<MTGCardInstance *> seenSrc;
                    std::vector<std::string> taps;
                    std::vector<int> tapRestrict;
                    for (size_t pi = 0; pi < picks.size(); pi++)
                    {
                        MTGCardInstance * ps = picks[pi] ? picks[pi]->source : NULL;
                        if (!ps || !seenSrc.insert(ps).second)
                            continue;
                        if (!ps->isCreature())
                            continue;
                        taps.push_back(ps->getDisplayName() + instanceHandle(ps)
                                       + animatedThisTurnNote(ps));
                        tapRestrict.push_back(paymentTapRestrictionOf(ps, beforeAttack, blockStillMatters));
                    }
                    opts[i] += payRepeatTapsClause(taps, tapRestrict, paid, n, //#W63-AE (E18)
                                                   w79UntapEngineSources(this, opponent()));
                }
                SAFE_DELETE(bill);
            }
        }
    }
    //Thread the source card as the pending source (as ANNOUNCE_X does): the model
    //often echoes "<verb> <source card>" against a bare option ("Tap Temple
    //Garden" vs "tap"), and INDEX-WINS treats a source-naming echo as a
    //self-reference rather than a stale prior answer (deck137 wave-24 s4). Empty
    //when the source name did not render - then (a2) option-label-in-echo covers it.
    //#W57-D (D14): the "Flip Side" display toggle does not reach the AI seat
    //when the row it duplicates - the D8 back-face LAND play - is on this very
    //menu. 22 of the corpus's 109 toggle rows came through here (146v125 seq 5:
    //`Play Land / Flip Side (~650 chars) / grimclimb pathway / Decline`), taken
    //0 times. The gate is the strongest available: the toggle is dropped ONLY
    //where the same window already offers the other face as a real land drop,
    //so no face is ever made unreachable, and a SPELL back face's toggle (the
    //D33 gates, still the only route to that cast) is never touched. The
    //engine's option vector, req.optionTexts (the staleness key) and the answer
    //INDEX are untouched: only the shown list shrinks, and the model's pick is
    //mapped straight back through shownToFull before anything is clicked.
    //#W67-AW (I9a), as rewritten by #W67-AZ (R4, codex review finding 4): the
    //unreachable repeat-pay rungs keep their ROWS and lose their DESCRIPTIONS.
    //Wave 67 dropped rows 3..21 from the printed list, so a model that wanted
    //`Add 3` had no row to answer - a legal engine choice made unreachable by
    //the renderer, which is the breach the doctrine names. Every row is printed,
    //numbered and answerable exactly as the engine offers it; the ~2.7 kB of
    //byte-identical price and taps text on the band's rows is replaced by one
    //short clause each, and the band's own arithmetic is stated once on the
    //largest row. The engine option vector, req.optionTexts (the staleness key)
    //and the answer INDEX are untouched, and now so is the printed list.
    if (opts.size() == req.optionTexts.size())
    {
        vector<bool> bandRows;
        int bLow = 0, bHigh = 0, bPaid = 0;
        const int keep = payRepeatCollapse(req.optionTexts, repeatPaid, bandRows,
                                           bLow, bHigh, bPaid);
        if (keep >= 0 && keep < (int) opts.size() && bandRows.size() == opts.size())
        {
            size_t shortened = 0;
            for (size_t i = 0; i < bandRows.size(); i++)
            {
                if (!bandRows[i] || i >= repeatBase.size()
                    || repeatBase[i] == (size_t) -1 || repeatBase[i] > opts[i].size())
                    continue;
                //#W68-BD (MED): back to the ENGINE's own label, which drops the
                //per-row liveness tag the band repeats 19 times as well as the
                //price text. Only when the row provably still BEGINS with that
                //label - otherwise the wave-67 truncation point stands.
                if (i < req.optionTexts.size() && !req.optionTexts[i].empty()
                    && opts[i].size() >= req.optionTexts[i].size()
                    && opts[i].compare(0, req.optionTexts[i].size(),
                                       req.optionTexts[i]) == 0)
                    opts[i] = req.optionTexts[i];
                else
                    opts[i].resize(repeatBase[i]);
                opts[i] += payRepeatBandRowTag(bPaid);
                shortened++;
            }
            if (shortened > 0)
            {
                opts[(size_t) keep] += payRepeatCollapseTag(bLow, bHigh, bPaid);
                DebugTrace("AIPlayerGPT: shortened " << shortened
                           << " identical repeat-pay rung row(s); every rung stays on the list");
            }
        }
    }
    //#W80-DG (U1 second half): every row on a COMPLETING cast menu that abandons
    //the cast prices the payment already made. Scoped three ways so nothing else
    //moves: the menu must carry the engine's own "Cast Card Normally" label and
    //be about the card the model committed to this turn (the same gate
    //castModeCommitmentNote uses for the header), the engine must have queued a
    //payment for THAT card in THIS step, and the row must be one that does not
    //cast (the engine's own decline row, or a plain no-op). Nothing is removed,
    //nothing is answered: one more true clause on rows that already existed.
    if (castModeMenu && !mCommittedCastName.empty() && ctxName == mCommittedCastName
        && mCommittedCastTurn == observer->turn
        && mPaidPendingCard == mCommittedCastName && mPaidPendingStep == stepKey())
    {
        const string spent = w80CompletingDeclineSpentClause(mPaidPendingSources,
                                                             mCommittedCastName);
        if (!spent.empty())
            for (size_t i = 0; i < opts.size(); i++)
            {
                const bool engineDecline = (i >= req.optionTexts.size());
                const bool noOpRow = (i < req.optionTexts.size()
                                      && AIPlayerGPT::rowSaysNoOp(opts[i]));
                if (engineDecline || noOpRow)
                    opts[i] += spent;
            }
    }
    vector<string> shownOpts;
    vector<size_t> shownToFull;
    if (mdfcLandRowShown)
        for (size_t i = 0; i < opts.size(); i++)
        {
            if (mdfcLandRowShown && i < req.optionTexts.size()
                && req.optionTexts[i] == "Flip Side")
                continue;
            shownOpts.push_back(opts[i]);
            shownToFull.push_back(i);
        }
    //A menu that would be left with nothing but a decline keeps its full list.
    const bool toggleFiltered = mdfcLandRowShown
                                && shownOpts.size() >= 2
                                && shownOpts.size() < opts.size();
    if (toggleFiltered)
    {
        DebugTrace("AIPlayerGPT: dropped " << (opts.size() - shownOpts.size())
                   << " display-toggle (Flip Side) row(s) from the menu; the back-face land row is offered");
        opts.swap(shownOpts);
    }
    //#W82-A (L1, audit-2026-09): the single-outcome repeat-pay auto-answer is
    //DELETED. "Every rung priced at 0 payments" was the MANA PLANNER's word
    //(`GptManaPolicy` through `ManaEngine::planPayment`), not the engine's: when
    //the policy refuses a producer the rows only LOOK identical, and the record
    //it wrote carried a literal 0 with no source count, so no reviewer could
    //check it after the fact. The menu is asked.
    int pick = askModel(decision, opts, true, ctxName, false, false, req.canDecline); //#W62-Y (D5)
    if (pick == kChoicePending)
        return kChoicePending;
    //Back into the engine's index space before any use of `pick`.
    size_t fullCount = opts.size();
    if (toggleFiltered)
    {
        fullCount = shownOpts.size(); //the pre-filter list, swapped aside above
        if (pick >= 0 && pick < (int) shownToFull.size())
            pick = (int) shownToFull[pick];
    }
    //W38 host-intent carry: the host is a SEPARATE later model call. Record
    //whose over/under menu was just answered and the plan stated with that
    //answer (consumePlan already folded a reply PLAN into mCurrentPlan), so
    //the host ask can render the pilot's own commitment back to it.
    if (mutatePlaceMenu && pick >= 0)
    {
        mMutateIntentCard = ctxName;
        mMutateIntentPlan = mCurrentPlan;
    }
    if (pick < 0)
    {
        //heuristic fallback: base selectMenuOption picks in SimpleMenu item
        //space; map its pick back into option space
        int k = AIPlayerBaka::selectMenuOption();
        act.choice = req.canDecline ? -1 : 0;
        for (size_t i = 0; i < req.menuIndices.size(); i++)
            if (req.menuIndices[i] == k)
            {
                act.choice = (int) i;
                break;
            }
        return 0;
    }
    if (req.canDecline && pick == (int) fullCount - 1)
        act.choice = -1; //applyMenuChoice clicks the cancel item
    else
        act.choice = pick;
    return 0;
}


MTGCardInstance * AIPlayerGPT::chooseCostTarget(TargetChooser * tc, MTGCardInstance * source)
{
    if (mEndpoint.empty() || !tc)
        return AIPlayerBaka::chooseCostTarget(tc, source);

    DecisionRequest req;
    if (!DecisionManager::buildChooseTarget(this, tc, req))
        return AIPlayerBaka::chooseCostTarget(tc, source);

    //cost targets are cards; mirror chooseCard's exclusions (never the
    //spell being paid for, never the chooser's own source) - EXCEPT for a
    //source already on the battlefield, which an activated ability's cost may
    //legally consume ("{1}, Sacrifice a Scarecrow" on a Scarecrow). Excluding
    //it unconditionally left an activation whose only payment is itself with
    //no legal answer, so the payment was never made and the engine re-offered
    //the ability every priority window (wave-34 b6 F1). Legality stays the
    //TargetChooser's call - it is what put the card in targetCandidates.
    vector<MTGCardInstance *> cands;
    vector<string> opts;
    for (size_t i = 0; i < req.targetCandidates.size(); i++)
    {
        MTGCardInstance * c = dynamic_cast<MTGCardInstance *>(req.targetCandidates[i]);
        if (!c)
            continue;
        if ((c == source || c == tc->source) && !c->isInPlay(observer))
            continue;
        cands.push_back(c);
        opts.push_back(describeTarget(this, c));
    }
    if (cands.empty())
        return AIPlayerBaka::chooseCostTarget(tc, source);
    if (cands.size() == 1)
        return cands[0]; //only one legal payment - no model call

    string what = source ? source->getDisplayName()
                         : (tc->source ? tc->source->getDisplayName() : string("this spell"));
    {
        vector<string> narr;
        for (size_t ci = 0; ci < opts.size() && ci < cands.size(); ci++)
            //W43-R1: same render boundary, log side - the paid card is named,
            //its rules text is not.
            narr.push_back("You paid "
                           + stripNarrationDecoration(describeTarget(this, cands[ci], false))
                           + " as " + what + "'s additional cost");
        setAskNarration(narr);
    }
    int pick = askModel("Choose what to pay for " + what
                        + "'s additional cost (it will be consumed):", opts);
    if (pick == kChoicePending)
        return NULL; //payment attempt aborts this tick; re-polled next tick
    if (pick < 0 || pick >= (int) cands.size())
        return AIPlayerBaka::chooseCostTarget(tc, source);
    return cands[pick];
}


//#W54-D (D3): the AAMover buried inside the ability that is waiting for this
//chooser's answer. A granted search rides wrappers (TargetAbility, the `and!()`
//MultiAbility), so the direct dynamic_cast at the call site finds nothing; walk
//the nesting the way unwrapDamagerAbility does. Two movers = ambiguous, so no
//answer (a wrong destination would be worse than none). Depth-capped.
static AAMover * unwrapMoverAbility(MTGAbility * a, int depth)
{
    if (!a || depth > 5)
        return NULL;
    if (AAMover * mv = dynamic_cast<AAMover *>(a))
        return mv;
    if (NestedAbility * na = dynamic_cast<NestedAbility *>(a))
        return unwrapMoverAbility(na->ability, depth + 1);
    if (MultiAbility * ma = dynamic_cast<MultiAbility *>(a))
    {
        AAMover * found = NULL;
        for (size_t i = 0; i < ma->abilities.size(); i++)
            if (AAMover * mv = unwrapMoverAbility(ma->abilities[i], depth + 1))
            {
                if (found)
                    return NULL; //two movers: destination ambiguous
                found = mv;
            }
        return found;
    }
    return NULL;
}


//#W54-D (D3): the script's own `moveTo(...)` argument, read as a GAIN
//destination for the player who is choosing. Returns "battlefield" / "hand"
//when the chosen card ends up under the CHOOSER's control, "" for a loss
//destination (exile, graveyard, library) or an opponent-side one. Pure over the
//string the parser stored, so the branch is provable and never dereferences a
//zone with a NULL target.
static string moverGainDestination(const string& dest)
{
    string d;
    for (size_t i = 0; i < dest.size(); i++)
        d += (char) tolower((unsigned char) dest[i]);
    if (d.find("opponent") != string::npos || d.find("targetedplayer") != string::npos)
        return "";
    if (d.find("battlefield") != string::npos || d.find("inplay") != string::npos)
        return "battlefield";
    if (d.find("hand") != string::npos)
        return "hand";
    return "";
}


//N-139b: map a lowercased "what the engine will do" string (built from the chooser
//source name AND the acting ability's menu verb) to the human verb for a
//remove-from-your-own-cards ask. Returns "" when no remove/lose verb is present.
//Sets relocate=true for a keep-the-card relocation (put back into library) as
//opposed to an outright loss. Pure helper for a deterministic proof.
//#W54-D (D3, R163): `gainDest` is the DESTINATION the chosen card is headed
//for. Path to Exile's granted search reads `moveTo(mybattlefield)` and the
//chosen basic land is PUT ONTO THE BATTLEFIELD - but the detection string
//carries the source card's own name, "path to exile" contains "exile", and the
//verb was decided by the NAME before anything read the destination: 8 of 8
//renders told the beneficiary of a compensation search to pick the card it
//could best afford to LOSE. A gain destination is not a removal at all, so the
//verb table answers "" and the caller frames the ask as the gain it is.
static string handRemovalVerb(const string& lc, bool& relocate, const string& gainDest = "")
{
    relocate = false;
    if (!gainDest.empty())
        return "";
    if (lc.find("discard") != string::npos) return "discard";
    if (lc.find("exploit") != string::npos) return "sacrifice";
    if (lc.find("sacrifice") != string::npos) return "sacrifice";
    if (lc.find("exile") != string::npos) return "exile";
    if (lc.find("bury") != string::npos) return "sacrifice";
    if (lc.find("put in library") != string::npos || lc.find("put back") != string::npos)
    {
        relocate = true;
        return "put back into your library";
    }
    return "";
}


//N-139b: build the verb-labeled ask for a remove-from-your-own-cards selection.
//Three framings share the "pick your least valuable" logic but name WHO caused it
//and WHAT happens: a keep-the-card relocation, an OPPONENT-forced loss (the
//validated deck140 wording, kept byte-identical), and a self-inflicted loss (a
//loot's own discard). Pure helper so the exact emitted strings are provable.
static string buildHandRemovalAsk(const string& verb, bool byOpponent, bool relocate,
                                  const string& effectName, bool multi, bool unlimited,
                                  bool targetMin, int maxtargets, size_t pickIndex)
{
    std::ostringstream q;
    if (relocate)
    {
        q << "PUT ONE OF YOUR OWN CARDS BACK: " << effectName << "'s effect will "
          << verb << " a card you choose from the list below - these are YOUR OWN"
             " cards, this is NOT a target you attack or affect. Pick the card you"
             " least need to keep in hand right now. ";
    }
    else if (byOpponent)
    {
        q << "FORCED " << verb << " OF YOUR OWN CARD"
          << ((multi && maxtargets != 1) ? "S" : "")
          << ": the opponent's effect (" << effectName << ") forces YOU to "
          << verb << " one of your OWN cards from the list below - each"
             " option is a card YOU will LOSE, not something you affect or"
             " attack. Pick the card you can best AFFORD TO LOSE (usually your"
             " LEAST valuable: pitch a spare land or a redundant/dead card,"
             " and KEEP your best spells, answers, and threats). ";
    }
    else
    {
        string VERB = verb;
        for (size_t i = 0; i < VERB.size(); i++) VERB[i] = (char) toupper((unsigned char) VERB[i]);
        q << VERB << " ONE OF YOUR OWN CARDS: your own effect (" << effectName
          << ") makes you " << verb << " a card from the list below - each option is"
             " a card YOU will LOSE, NOT a target you attack or affect. Pick the card"
             " you can best AFFORD TO LOSE (usually your LEAST valuable: a spare land"
             " or a redundant/dead card; KEEP your best spells, answers, and threats). ";
    }
    if (!multi)
        q << (relocate ? "Choose the ONE card to put back" : "Choose the ONE card to give up");
    else
    {
        q << "Choose card " << (pickIndex + 1);
        if (!unlimited)
            q << " of " << (targetMin ? "exactly " : "up to ") << maxtargets;
    }
    q << " from the list below, and answer with the chosen card's row number."; //#W70-BL (E3)
    //#W81-DM (V12, wave-80 engine-seat MED-3 / known-bugs V12). THE SEAM ASKS
    //ONE CARD PER WINDOW AND NEVER SAID SO. `152` seqs 217 and 237 (Emrakul's
    //Annihilator, sacrifice 6) answered `CHOICE: 1, 2, 7, 8, 9, 11 (Boulderloft
    //Pathway, Forest, Plains #1, Plains #2, Plains #3, Hengegate Pathway)` and
    //`CHOICE: 5, 6, 7, 8, 9, 10 (...)`; the parser stamped
    //`parse_note: multi_answer_first_taken`, kept row 1, and the window said
    //NOTHING - the model plainly believed it had chosen all six. "Choose card 1
    //of exactly 6" is a true sentence a reader can take either way: as "pick the
    //first of the six you are naming now", or as "this is the first of six
    //windows". It is the second, and now it says which.
    //
    //The last clause states what the PARSER does with a longer list, because a
    //model told only "one number" that still writes six loses five picks
    //silently. Nothing is removed, capped or auto-answered: every row stays, the
    //answer is still the model's, and a multi-number reply is still read - it is
    //just described honestly. MULTI ASKS ONLY; the single-pick branch above is
    //byte-identical, and so is every gain ask (a different builder).
    if (multi)
    {
        q << " THIS WINDOW TAKES ONE ROW NUMBER, NOT A LIST: you are asked once"
             " per card";
        if (!unlimited)
            q << " and this is window " << (pickIndex + 1) << " of " << maxtargets;
        q << ", so the rest of your picks are made at the windows that follow this"
             " one. If you write several numbers here, the FIRST is the one that"
             " happens and the others are discarded.";
    }
    return q.str();
}


//#W54-D (D3): the ask for a search/move whose chosen card the DECIDING player
//GAINS. Same "these are your own cards, this is not a target you attack" frame
//as the loss ask - and the OPPOSITE value ordering, because the right pick is
//the card you most need rather than the one you can best spare. Pure helper so
//the emitted strings are provable.
static string buildHandGainAsk(const string& where, const string& effectName,
                               bool multi, bool unlimited, bool targetMin,
                               int maxtargets, size_t pickIndex)
{
    bool toHand = (where == "hand");
    std::ostringstream q;
    q << (toHand ? "CHOOSE A CARD TO PUT INTO YOUR HAND: "
                 : "CHOOSE A CARD TO PUT ONTO THE BATTLEFIELD: ")
      << effectName << "'s effect puts a card you choose from the list below "
      << (toHand ? "into YOUR hand" : "onto YOUR battlefield")
      << " - each option is a card you GAIN, NOT a card you lose and NOT a target"
         " you attack or affect. Pick the card you MOST NEED right now (with"
         " basic lands, the colour your hand and your board are shortest of). ";
    if (!multi)
        q << (toHand ? "Choose the ONE card to put into your hand"
                     : "Choose the ONE card to put onto the battlefield");
    else
    {
        q << "Choose card " << (pickIndex + 1);
        if (!unlimited)
            q << " of " << (targetMin ? "exactly " : "up to ") << maxtargets;
    }
    q << " from the list below, and answer with the chosen card's row number."; //#W70-BL (E3)
    return q.str();
}


//#W54-D (D25, R185): cheapest-first rank for a FORCED-loss list. The 23-row
//annihilator menu at 126v125 seq 111 arrived in battlefield order with the
//seat's only lifelink Vampire LAST, under a header telling the pilot to pick
//its least valuable card - and row 23 is what it picked. Lands rank below
//everything (they are the header's own example of the right pick), then
//ascending mana value; the sort is STABLE, so battlefield order survives inside
//each tier. Representation only - no option removed, no option merged.
static int sacrificeCostRank(Targetable * t)
{
    MTGCardInstance * c = dynamic_cast<MTGCardInstance *>(t);
    if (!c)
        return 1000; //a player or other targetable is never the cheap pick
    if (c->hasType(Subtypes::TYPE_LAND))
        return -1;
    ManaCost * mc = c->getManaCost();
    return mc ? mc->getConvertedCost() : 0;
}


//#W54-D (D3/D6): the third register hook. The own-card chooser's FRAMING - the
//verb, the destination, and whose effect is forcing it - is a render surface
//the suite cannot otherwise see: the zones are identical whether the header
//says "pick the card you can best afford to LOSE" or "pick the card you MOST
//NEED", and identical whether it says "your own effect" or "the opponent's
//effect". Runs the PRODUCTION builders over the live chooser, so a red here is
//a red render. Emits nothing unless the chooser is a removal/move over the
//seat's own cards, so every other fixture pays one scan and logs nothing.
string ownCardChooserRegister(GameObserver * observer, Player * seat)
{
    if (!observer || !observer->mLayers || !seat)
        return "";
    TargetChooser * tc = observer->getCurrentTargetChooser();
    if (!tc || !tc->source || tc->Owner != seat)
        return "";
    MTGAbility * waiting = dynamic_cast<MTGAbility *>(observer->mLayers->actionLayer()->isWaitingForAnswer());
    string effectName;
    if (waiting)
        effectName = resolveOwningCardName(waiting->source);
    if (effectName.empty())
        effectName = resolveOwningCardName(tc->source);
    if (effectName.empty())
        effectName = "this effect";
    string lc = effectName;
    if (AAMover * mv = dynamic_cast<AAMover *>(waiting))
    {
        lc += " ";
        lc += mv->getMenuText(tc);
    }
    else if (waiting)
    {
        lc += " ";
        lc += waiting->getMenuText();
    }
    for (size_t i = 0; i < lc.size(); i++)
        lc[i] = (char) tolower((unsigned char) lc[i]);
    string gainDest;
    if (AAMover * mvd = unwrapMoverAbility(waiting, 0))
        gainDest = moverGainDestination(mvd->destination);
    bool relocate = false, relocateIgnored = false;
    string lossVerb = handRemovalVerb(lc, relocate, gainDest);
    bool wouldBeLoss = !handRemovalVerb(lc, relocateIgnored).empty();
    MTGCardInstance * granter = tc->source;
    if (granter && granter->getDisplayName().empty() && granter->storedSourceCard)
        granter = granter->storedSourceCard;
    bool selfInflicted = granter && granter->controller() == seat;
    if (!lossVerb.empty())
        return "own-card chooser: "
             + buildHandRemovalAsk(lossVerb, !selfInflicted, relocate, effectName,
                                   false, false, false, 1, 0);
    if (wouldBeLoss && !gainDest.empty())
        return "own-card chooser: "
             + buildHandGainAsk(gainDest, effectName, false, false, false, 1, 0);
    return "";
}


//N-139a: role-named ask for the mutate-HOST target step (placement 1=over,
//2=under, already chosen). Names what this step is and what the placement means,
//so the model stops re-deriving the whole mutate sequence here. Pure helper.
static string mutateHostAsk(const string& effectName, int placement)
{
    std::ostringstream q;
    q << "MUTATE - CHOOSE THE HOST creature to mutate onto (placement step for "
      << effectName << "; the over/under choice is already made). You chose "
      << (placement == 1
            ? ("OVER: " + effectName + "'s card goes ON TOP, so the merged creature keeps "
               + effectName + "'s name and P/T and ALSO gains the host's abilities")
            : ("UNDER: " + effectName + " goes BENEATH, so the host keeps its name and P/T and gains "
               + effectName + "'s abilities"))
      << ". Each option is one of your non-Human creatures; the two become ONE merged"
         " creature (CR 725). Pick the ONE creature to mutate onto, and answer with its"
         " row number."; //#W70-BL (E3)
    return q.str();
}


//W37 #2: the waiting-for-answer element for a triggered/activated damage
//target ask is a WRAPPER, not the AADamager itself - the inline
//"damage:N target(...)" script builds a TargetAbility (NestedAbility) holding
//the damager, and may/multi shells nest further. Walk the nesting; a
//MultiAbility with two damage riders is ambiguous and yields nothing (a wrong
//number would be worse than none). Depth-capped against cyclic clones.
AADamager * unwrapDamagerAbility(MTGAbility * a, int depth)
{
    if (!a || depth > 5)
        return NULL;
    if (AADamager * ad = dynamic_cast<AADamager *>(a))
        return ad;
    if (NestedAbility * na = dynamic_cast<NestedAbility *>(a))
        return unwrapDamagerAbility(na->ability, depth + 1);
    if (MultiAbility * ma = dynamic_cast<MultiAbility *>(a))
    {
        AADamager * found = NULL;
        for (size_t i = 0; i < ma->abilities.size(); i++)
            if (AADamager * ad = unwrapDamagerAbility(ma->abilities[i], depth + 1))
            {
                if (found)
                    return NULL; //two damage riders: attribution ambiguous
                found = ad;
            }
        return found;
    }
    return NULL;
}


int AIPlayerGPT::chooseTarget(TargetChooser * _tc, Player * forceTarget, MTGCardInstance * chosenCard, bool checkOnly)
{
    //checkOnly is a castability probe (no clicks, no decision) and
    //forceTarget a scripted redirection - both are mechanics for the base.
    if (mEndpoint.empty() || checkOnly || forceTarget)
        return AIPlayerBaka::chooseTarget(_tc, forceTarget, chosenCard, checkOnly);

    if (observer->currentlyActing() != this)
        return 0;
    TargetChooser * tc = _tc ? _tc : observer->getCurrentTargetChooser();
    if (!tc || !tc->source || tc->maxtargets < 1)
        return 0;
    if (tc->Owner != observer->currentlyActing())
        return AIPlayerBaka::chooseTarget(_tc, forceTarget, chosenCard, checkOnly); //base has recovery for this

    bool multi = (tc->maxtargets != 1);
    bool unlimited = (tc->maxtargets == TargetChooser::UNLITMITED_TARGETS);
    if (multi)
        tc->initTargets(); //fresh selection, mirroring the heuristic path

    //Bounce-on-the-stack misconception (deck14 wave-20 E1): the model casts
    //Unsummon/Boomerang to "answer" a spell the opponent is CASTING, then at
    //this forced target menu - whose legal set is battlefield permanents only -
    //self-bounces or thrashes hunting for the on-stack spell that is not here.
    //When (a) the chooser cannot reach the stack but targets the battlefield,
    //and (b) an opponent spell is actually resolving on the stack, prepend a
    //one-line header naming that trap. Conditioned so it only appears when the
    //confusion can occur; representation only, the offered set is unchanged.
    bool stackTrapNote = false;
    {
        MTGGameZone * stk0 = observer->players[0]->game->stack;
        MTGGameZone * stk1 = observer->players[1]->game->stack;
        bool chooserHitsStack = tc->targetsZone(stk0) || tc->targetsZone(stk1);
        bool chooserHitsBattlefield =
            tc->targetsZone(observer->players[0]->game->inPlay) ||
            tc->targetsZone(observer->players[1]->game->inPlay);
        if (!chooserHitsStack && chooserHitsBattlefield)
        {
            for (int pi = 0; pi < 2 && !stackTrapNote; pi++)
            {
                MTGGameZone * sz = observer->players[pi]->game->stack;
                for (int zi = 0; zi < sz->nb_cards; zi++)
                {
                    MTGCardInstance * sc = sz->cards[zi];
                    //An opponent-controlled object on the stack (not the bounce
                    //spell itself) is what the model mistakes for a target.
                    if (sc && sc != tc->source && sc->controller() != this)
                    {
                        stackTrapNote = true;
                        break;
                    }
                }
            }
        }
    }

    //Multi-target selection runs as a sequence of single picks: each round
    //asks for one more target (with a "Done" escape once the minimum is
    //satisfiable), so the reply stays a single reliable number instead of a
    //free-form list. Prompt-keyed caching keeps repeated polling cheap.
    vector<Targetable *> picks;
    DecisionRequest req; //the last round's request carries the offered set for apply
    //N-158k (wave-32 HL5.3, the COMMIT seat): Feed the Swarm's per-target life
    //cost ("life:-manacost" - you pay life equal to the chosen permanent's mana
    //value) renders on the CAST line and won a game there, but the TARGET menu -
    //where the target is actually committed - carried rules text and no price at
    //all. deck152 answered it at 11 life and again at 6 with the number invisible.
    //The standing rule is that the deciding fact rides the seat where the choice
    //commits, so the identical "(costs you N life)" fragment is emitted here too.
    //(history: comment-archaeology.md AIPlayerGPTSeams-L9121-2164)
    int perilBeforeResolve = 0;
    {
        MTGCardInstance * psrc = tc ? tc->source : NULL;
        const int above = stackLifeLossBefore(observer, this, psrc);
        perilBeforeResolve = (above >= 0) ? above : namedCastLifeSurcharge(this, psrc);
    }
    bool lifeCostPerTarget = false;
    if (tc && tc->source)
    {
        string mtl = tc->source->magicText;
        for (size_t li = 0; li < mtl.size(); li++)
            mtl[li] = (char) tolower((unsigned char) mtl[li]);
        lifeCostPerTarget = mtl.find("life:-manacost") != string::npos;
    }
    //W36 #4 (158 P6): when the ability waiting for this target deals a knowable
    //damage amount, each creature line below carries the engine's kill verdict.
    //"rand" expressions draw the game RNG - never evaluated (standing rule);
    //0 or negative amounts render nothing (an authoritative 0 is worse than
    //silence - the sorcery-power lesson).
    //W37 #2 (wave-36 validation: 0 renders corpus-wide - the hook never fired):
    //the waiting element for a triggered/activated damage ask is the target
    //WRAPPER (TargetAbility/GenericTargetAbility around the AADamager -
    //Bowmasters' "damage:1 target(anytarget)" trigger, Foray's granted "Damage
    //creature"), so the direct dynamic_cast always failed - unwrap the nesting.
    //And a burn SPELL's target menu arms at CAST time with nothing waiting at
    //all: derive the amount from the spell's own single damage: rider instead.
    int dmgAmount = 0;
    bool dmgDeathtouch = false;
    //#W52-L (D15): the -N/-N shrink amount, when knowable (deck123 vs152 seq 11
    //"Elite Spellbinder (4/2)" read as "it is 3/1" and Slip'd a survivor).
    int ptDrop = 0;
    {
        MTGAbility * waiting = dynamic_cast<MTGAbility *>(observer->mLayers->actionLayer()->isWaitingForAnswer());
        int dp = 0, dt = 0; bool ueot = false; MTGCardInstance * mt = NULL;
        if (waiting && ptPumpModifierDelta(waiting, dp, dt, ueot, mt) && dt < 0 && dp == dt)
            ptDrop = -dt;
        //#W53-O (D5): the spell half now lives in one helper, shared with the
        //CAST row's kill summary so the two surfaces read the same script.
        else if (!waiting)
            ptDrop = spellPTDropAmount(observer, tc->source);
    }
    {
        AADamager * ad = unwrapDamagerAbility(
            dynamic_cast<MTGAbility *>(observer->mLayers->actionLayer()->isWaitingForAnswer()), 0);
        if (ad && ad->d.find("rand") == string::npos)
        {
            dmgAmount = ad->getDamage();
            dmgDeathtouch = ad->source && ad->source->has(Constants::DEATHTOUCH);
        }
        //#W53-O (D5): same helper as the CAST row's kill summary (the spell
        //path's scoping comment lives with the helper).
        else if (!ad)
            dmgAmount = spellSingleDamageAmount(tc->source, dmgDeathtouch);
    }
    //#W62-X (D19): the crack-back total THIS screen prints, if it prints one.
    //Gated on exactly the render's own conditions (`crackBackNextTurnDue` over
    //the same phase and the same seat), so a row never points at a line that is
    //not above it; and it uses `crackBackTotalOver`, which is the same walk the
    //line itself is built from.
    int crackTotal = 0, crackAttackers = 0;
    bool crackIsFloor = false;
    {
        Player * cbOpp = opponent();
        GameObserver * cbObs = getObserver();
        const int cbPhase = cbObs ? (int) cbObs->getCurrentGamePhase() : (int) MTG_PHASE_INVALID;
        const bool cbSeat = cbObs && cbObs->currentPlayer == this;
        crackTotal = crackBackTotalOver(cbOpp, &crackAttackers);
        if (!crackBackNextTurnDue(cbSeat, cbPhase, crackAttackers, crackTotal))
            crackTotal = 0;
        else
            crackIsFloor = !crackBackFloorSources(cbOpp).empty();
    }
    for (;;)
    {
        vector<Targetable *> targets;
        vector<string> opts;
        //W43-R1: the LOG's copy of each candidate, built at the same render
        //boundary from the same engine facts, minus the decision-time card-text
        //quote. Index-parallel to `targets`/the real-target prefix of `opts`.
        vector<string> narrOpts;
        //"Done" goes LAST (after the real targets): the model favors option
        //1, and an early-listed escape biased multi-target picks short.
        //#W82-A (L1, audit-2026-09): the `Done` row is offered from the FIRST
        //pick, not the second. CR 115.6: "Some effects... may allow zero targets
        //to be chosen" - an `<upto:N>` chooser with no minimum is exactly that,
        //and the engine accepts a zero-target answer (GameObserver.cpp:2330), so
        //the old `!picks.empty()` made a legal answer unreachable. It goes LAST
        //(after the real targets) because an early-listed escape biases
        //multi-target picks short; that ordering rule is untouched.
        bool mayStop = multi && !tc->targetMin;

        //Candidate enumeration comes from the contract; this seam only
        //renders it and excludes this selection round's earlier picks.
        DecisionRequest round;
        if (DecisionManager::buildChooseTarget(this, tc, round))
        {
            for (size_t i = 0; i < round.targetCandidates.size(); i++)
            {
                Targetable * t = round.targetCandidates[i];
                if (std::find(picks.begin(), picks.end(), t) != picks.end())
                    continue;
                targets.push_back(t);
                string priceNote = lifeCostPerTarget ? perTargetLifeCostNote(t) : string();
                narrOpts.push_back(describeTarget(this, t, false) + priceNote);
                string tdesc = describeTarget(this, t) + priceNote;
                //#W80-DG (U11): the seat's own spell on the stack, named as such.
                if (MTGCardInstance * ostc = dynamic_cast<MTGCardInstance *>(t))
                    if (ostc->currentZone)
                        tdesc += w80OwnStackSpellTag(ostc->controller() == this,
                                                     zoneDesc(ostc->currentZone));
                //W36 #4: the kill verdict rides the creature target line.
                if (dmgAmount > 0)
                    if (MTGCardInstance * dtc = dynamic_cast<MTGCardInstance *>(t))
                        if (dtc->isCreature() && dtc->controller() && dtc->controller()->game
                            && dtc->currentZone == dtc->controller()->game->inPlay)
                        {
                            tdesc += damageTargetVerdict(dmgAmount, dtc->toughness, dtc->life,
                                                         dtc->basicAbilities[Constants::INDESTRUCTIBLE],
                                                         dmgDeathtouch);
                            //#W62-X (D19): what killing it takes off the
                            //crack-back total the same screen printed. Only on
                            //a row whose verdict is DIES, and only for a body
                            //that is actually IN that total.
                            if (damageKillsTarget(dmgAmount, dtc->life,
                                                  dtc->basicAbilities[Constants::INDESTRUCTIBLE],
                                                  dmgDeathtouch))
                                tdesc += crackBackReliefClause(crackTotal,
                                             dtc->controller() == opponent()
                                                 ? crackBackBodyContribution(dtc) : 0,
                                             life, crackIsFloor,
                                             //#W63-AF (R7)
                                             crackBackBodyHoldsUpOthers(dtc, opponent(),
                                                                        getObserver())
                                                 ? dtc->getDisplayName() : string());
                        }
                //#W52-L (D15): the shrink verdict rides the creature target line.
                if (ptDrop > 0 && !dmgAmount)
                    if (MTGCardInstance * dtc = dynamic_cast<MTGCardInstance *>(t))
                        if (dtc->isCreature() && dtc->controller() && dtc->controller()->game
                            && dtc->currentZone == dtc->controller()->game->inPlay)
                        {
                            tdesc += ptDropTargetVerdict(ptDrop, dtc->toughness, dtc->life);
                            if (ptDropKillsTarget(ptDrop, dtc->life)) //#W62-X (D19)
                                tdesc += crackBackReliefClause(crackTotal,
                                             dtc->controller() == opponent()
                                                 ? crackBackBodyContribution(dtc) : 0,
                                             life, crackIsFloor,
                                             //#W63-AF (R7)
                                             crackBackBodyHoldsUpOthers(dtc, opponent(),
                                                                        getObserver())
                                                 ? dtc->getDisplayName() : string());
                        }
                //#W54-C (D4 part iii): the PLAYER rows of the same ask. They
                //carried nothing while their creature siblings carried a
                //verdict, and the row that won the game was the bare one.
                if (dmgAmount > 0)
                    if (Player * dtp = dynamic_cast<Player *>(t))
                        tdesc += damagePlayerVerdict(dmgAmount, dtp->life, dtp == this,
                                                     this->life, perilBeforeResolve, //#W60-L (B1)
                                                     lifeLoopProvenWin(this), //#W62-AA (R6)
                                                     //#W75-CM (F7)
                                                     sourceDealsPoisonInsteadOfDamage(tc->source),
                                                     dtp->poisonCount);
                //#W77-CS (R3 c): the player rows of a toughness-lifegain EDICT
                //carried nothing at all - both branches of `123v126` seq 71
                //were bare names. One clause shape, both branches.
                if (Player * etp = dynamic_cast<Player *>(t))
                    tdesc += w77EdictPlayerTargetTagFor(this, etp, tc->source);
                //#W54-C (D4): and a planeswalker's answer is its loyalty - the
                //helper existed and only the ability path was calling it, so
                //`130v162` seq 63's Ob Nixilis row was bare too.
                if (dmgAmount > 0)
                    if (MTGCardInstance * dtc = dynamic_cast<MTGCardInstance *>(t))
                        if (!dtc->isCreature() && dtc->hasType(Subtypes::TYPE_PLANESWALKER)
                            && dtc->counters && dtc->counters->hasCounter("loyalty", 0, 0)
                            && dtc->controller() && dtc->controller()->game
                            && dtc->currentZone == dtc->controller()->game->inPlay)
                            tdesc += damagePlaneswalkerVerdict(dmgAmount,
                                         dtc->counters->hasCounter("loyalty", 0, 0)->nb);
                opts.push_back(tdesc);
            }
            req = round;
        }
        if (targets.empty())
            break;
        if (mayStop)
            opts.push_back("Done - no further targets");
        //#W75-CL (P5): the loop override, on the target rows too - the
        //`{right now: takes N damage - you would be at M}` verdict is built by
        //the same helper here as on the priority rows.
        w75ApplyChainFeed(opts, lifeLoopProvenWin(opponent()));

        //Granted/inner abilities ride a nameless fake card - Liliana's "+1:
        //each player discards" chooser rendered 'Choose the target for '
        //(blank). The waiting action element's menu text names the effect.
        string effectName = tc->source->getDisplayName();
        //N-158d (wave-31 deck158): when a SPELL grants an ability to a permanent
        //and that ability then asks for a target (Foray of Orcs transforms a
        //"deal damage" ability onto the Army it just amassed), tc->source is the
        //PERMANENT, so the header read "TARGET CHOICE for Orc army" - naming the
        //creature the pilot controls as the thing being targeted, which invites a
        //self-target read. The waiting action element knows the EFFECT's own name
        //("Damage creature"); when it differs from the permanent, say both, so
        //the header names what is happening rather than who is holding it.
        string abilityName;
        MTGAbility * waiting = dynamic_cast<MTGAbility *>(observer->mLayers->actionLayer()->isWaitingForAnswer());
        if (waiting)
            abilityName = renderAbilityLabel(waiting->getMenuText()); //W42-D4
        if (effectName.empty())
        {
            //W41-3(d), OWNER RULING (verbatim: "'put in play' is not good. this
            //should be the effect source, not the effect"): a granted/inner
            //ability rides a NAMELESS fake card, and the old fallback promoted
            //the ability's own display name into the SOURCE slot - "You targeted
            //Swamp with Put in Play" names an EFFECT as though it were a card,
            //on the one surface whose whole job is to say what is doing this.
            //The ability knows the card that owns it; take the name from there
            //and leave the effect in the ability slot, where the header and the
            //narration both already render it: "You targeted Swamp with
            //Windswept Heath's Put in Play ability".
            //W42-D3: the ladder's rungs now run through resolveOwningCardName,
            //which adds the storedSourceCard hop. 29 corpus lines fell all the
            //way to the anonymous "this effect" - EVERY one of them a granted
            //`ability$!!$` ability (Tribute to Hunger's ToughLife, Soul
            //Shatter's Sacrifice, Path to Exile's Put in Play), whose dummy
            //carrier knows the card that granted it. A nameless fake card
            //carrying the ability is not an unnamed source; it is an unfollowed
            //link.
            if (waiting)
                effectName = resolveOwningCardName(waiting->source);
            if (effectName.empty())
                effectName = resolveOwningCardName(tc->source);
            //Never the ability's own name: an unnamed source is honestly
            //unnamed, and the ability is still named beside it.
            if (effectName.empty())
                effectName = "this effect";
        }

        //Forced self-loss inversion (deck140/deck102 wave-22, LOSS-CAUSING):
        //when an OPPONENT'S effect forces THIS seat to discard/sacrifice/exile
        //one of its OWN cards, the chooser is a TargetChooser whose legal set
        //is entirely the deciding player's own cards. The generic "TARGET
        //CHOICE ... pick the ONE target it will affect" wording is built for
        //choosing what a spell AFFECTS (an enemy permanent to remove -> pick
        //the MOST valuable) and inverts catastrophically here: you are picking
        //which of your OWN cards to LOSE, so the correct choice is the LEAST
        //valuable. deck140 pitched Damnation then Pyroclasm - its two live
        //sweepers - into an Archon-of-Cruelty forced discard and died T12.
        //Detect the shape (every candidate is my own card AND the effect verb
        //is a loss) and render inverted framing. Representation only - the
        //offered set and the apply path are unchanged.
        //(history: comment-archaeology.md AIPlayerGPTSeams-L9386-2175)
        bool forcedSelfLoss = false;
        //#W54-D (D3): the same shape with a GAIN destination - a compensation
        //search whose chosen card comes to the chooser, not away from it.
        bool forcedSelfGain = false;
        string gainDest;
        bool relocateNotLoss = false;
        bool selfInflicted = false;
        string lossVerb;
        {
            string lc = effectName;
            MTGAbility * waiting = dynamic_cast<MTGAbility *>(observer->mLayers->actionLayer()->isWaitingForAnswer());
            if (AAMover * mv = dynamic_cast<AAMover *>(waiting))
            {
                //AAMover's menu verb is zone-aware (dest + chooser source zone);
                //pass the current chooser so hand->library reads "Put in Library".
                lc += " ";
                lc += mv->getMenuText(tc);
            }
            else if (waiting)
            {
                lc += " ";
                lc += waiting->getMenuText();
            }
            for (size_t i = 0; i < lc.size(); i++) lc[i] = (char) tolower((unsigned char) lc[i]);
            //#W54-D (D3): read the DESTINATION off the move this chooser feeds,
            //before the verb table gets to decide from the source card's name.
            if (AAMover * mvd = unwrapMoverAbility(waiting, 0))
                gainDest = moverGainDestination(mvd->destination);
            lossVerb = handRemovalVerb(lc, relocateNotLoss, gainDest);
            bool relocateIgnored = false;
            //#W54-D (D3): the shape is still the removal shape (own cards, a
            //move verb) - only the direction is inverted. Gate the GAIN framing
            //on the loss detection having fired, so nothing else re-frames.
            bool wouldBeLoss = !handRemovalVerb(lc, relocateIgnored).empty();
            if ((!lossVerb.empty() || (wouldBeLoss && !gainDest.empty())) && !targets.empty())
            {
                bool allMine = true;
                for (size_t i = 0; i < targets.size(); i++)
                {
                    //the "Done" escape is a string option, never in targets.
                    MTGCardInstance * mc = dynamic_cast<MTGCardInstance *>(targets[i]);
                    if (!mc || mc->controller() != this) { allMine = false; break; }
                }
                if (!lossVerb.empty())
                    forcedSelfLoss = allMine;
                else
                    forcedSelfGain = allMine;
                //Self-inflicted (a loot, a sacrifice-as-cost of MY OWN spell) vs an
                //OPPONENT forcing the loss: only my own source flips to the self
                //wording; anything else keeps the validated opponent framing.
                //#W54-D (D6, R166): an ability GRANTED to a player is parsed onto
                //a NAMELESS dummy card controlled by the victim, so the raw
                //controller test read TRUE for the OPPONENT's annihilator and 18
                //of the corpus's 26 renders told this seat that Emrakul was "your
                //own effect". The real granter hangs off storedSourceCard - the
                //same indirection the stack line, token creation and mana
                //production inside that keyword already take.
                MTGCardInstance * granter = tc->source;
                if (granter && granter->getDisplayName().empty() && granter->storedSourceCard)
                    granter = granter->storedSourceCard;
                selfInflicted = granter && granter->controller() == this;
            }
        }

        //#W60-O (B10): the FORCED SACRIFICE shape. Independent of the
        //forced-loss detection above, which reads the effect NAME and the
        //ability's menu verb and never fired here (the waiting ability's label
        //is the life-gain rider, not the sacrifice). Read it off the acting
        //SCRIPT instead - a `sacrifice` over `notatarget(creature|mybattlefield)`
        //- and require that every candidate really is one of THIS seat's own
        //battlefield creatures, so nothing else can be re-framed. The
        //beneficiary of a `toughnesslifegain` rider is taken from the script's
        //own target word and left UNNAMED when it says neither (a silent
        //omission beats a wrong claim about who is paid). Representation only:
        //the offered set, its ORDER and the apply path are untouched.
        bool forcedSacrifice = false;
        bool sacByOpponent = true;
        int sacGain = 0; //0 = beneficiary not named, 1 = the opponent, 2 = this seat
        if (!forcedSelfLoss && !forcedSelfGain && !targets.empty())
        {
            vector<MTGCardInstance *> chain;
            MTGCardInstance * sc = tc->source;
            for (int d = 0; sc && d < 4; d++, sc = sc->storedSourceCard)
                chain.push_back(sc);
            MTGAbility * wsac = dynamic_cast<MTGAbility *>(observer->mLayers->actionLayer()->isWaitingForAnswer());
            sc = wsac ? wsac->source : NULL;
            for (int d = 0; sc && d < 4; d++, sc = sc->storedSourceCard)
                chain.push_back(sc);
            string script;
            for (size_t ci = 0; ci < chain.size() && script.empty(); ci++)
            {
                string low = toLowerCopy(chain[ci]->magicText);
                if (low.find("sacrifice") != string::npos
                    && low.find("notatarget(creature|mybattlefield)") != string::npos)
                    script = low;
            }
            if (!script.empty())
            {
                bool allMineCreatures = true;
                for (size_t ti = 0; ti < targets.size() && allMineCreatures; ti++)
                {
                    MTGCardInstance * mc = dynamic_cast<MTGCardInstance *>(targets[ti]);
                    if (!mc || !mc->isCreature() || mc->controller() != this
                        || !game || mc->currentZone != game->inPlay)
                        allMineCreatures = false;
                }
                //#W74-CC (O3): ...and only when the pick is the EDICT's own
                //untargeted chooser, not a sibling mode's target(...).
                forcedSacrifice = forcedSacrificeShape(script, allMineCreatures,
                                                       tc && tc->targetter != NULL);
                if (forcedSacrifice)
                {
                    if (script.find("toughnesslifegain targetopponent") != string::npos)
                        sacGain = 1;
                    else if (script.find("toughnesslifegain targetcontroller") != string::npos)
                        sacGain = 2;
                    //whose effect is forcing it: the first card in the chain
                    //that names itself (the ability$! dummies are nameless).
                    for (size_t ci = 0; ci < chain.size(); ci++)
                        if (!chain[ci]->getDisplayName().empty())
                        {
                            sacByOpponent = chain[ci]->controller() != this;
                            break;
                        }
                }
            }
        }
        //#W66-AQ (H10): the engines among the rows, collected once for both the
        //per-row tag and the header's tie-break scope.
        string sacEngineRows;
        if (forcedSacrifice)
            for (size_t ti = 0; ti < targets.size() && ti < opts.size(); ti++)
                if (MTGCardInstance * vc = dynamic_cast<MTGCardInstance *>(targets[ti]))
                {
                    const char * ek = engineKindForScript(vc->magicText); //#W66-AQ (H10)
                    //#W76-CO (Q6): what a converter of THEIRS turns their gain
                    //into. Same scan and same helper the edict row's converter
                    //clause uses (#W53-O D13), so the two surfaces cannot
                    //disagree about the same board.
                    string sacConvName;
                    int sacConvTakes = 0;
                    //#W76-CQ (F6): a converter prices a gain that HAPPENS. With
                    //a life-gain prohibition in force (Erebos on this seat's
                    //side) there is no gain event, so no Bond trigger and no
                    //loss - the clause is silent rather than false.
                    //#W77-CS (R3, deck126 MED): and the MIRROR of it. When the
                    //seat is the gainer (`sacGain == 2`), the converter that
                    //prices the event is the seat's OWN - same walk, same
                    //helper, the other battlefield.
                    Player * sacGainer = (sacGain == 1) ? opponent()
                                       : (sacGain == 2) ? this : NULL;
                    if (sacGainer && sacGainer->game && w76PlayerCanGainLife(sacGainer))
                    {
                        MTGGameZone * cbf = sacGainer->game->inPlay;
                        for (int cvi = 0; cbf && cvi < cbf->nb_cards && !sacConvTakes; cvi++)
                        {
                            MTGCardInstance * cvc = cbf->cards[cvi];
                            if (!cvc)
                                continue;
                            const int tk = lifeToDamageConverterTake(cvc->magicText,
                                                                    vc->toughness);
                            if (tk > 0)
                            {
                                sacConvName = cvc->name + instanceHandle(cvc);
                                sacConvTakes = tk;
                            }
                        }
                    }
                    //#W77-CU (F7): the SAME predicate that gated the converter
                    //lookup above now reaches the gain clause itself.
                    const bool sacGainReaches = (sacGainer != NULL)
                                                && w76PlayerCanGainLife(sacGainer);
                    opts[ti] += forcedSacrificeRowTag(sacGain, vc->toughness, ek,
                                                      sacConvName, sacConvTakes,
                                                      sacGainReaches);
                    if (ek)
                    {
                        if (!sacEngineRows.empty())
                            sacEngineRows += ", ";
                        sacEngineRows += vc->getDisplayName() + instanceHandle(vc)
                                         + " is a " + ek;
                    }
                }

        //#W54-D (D25, R185): the forced-loss list is ordered cheapest-first, so
        //the header's own advice ("usually your LEAST valuable: a spare land")
        //and the list agree. targets / opts / narrOpts stay index-parallel and
        //the trailing "Done" row (if any) stays last; picks are pointers, so the
        //apply path is untouched by the reordering.
        if (forcedSelfLoss && targets.size() >= 2 && narrOpts.size() == targets.size()
            && opts.size() >= targets.size())
        {
            vector<size_t> ord(targets.size());
            for (size_t i = 0; i < ord.size(); i++)
                ord[i] = i;
            std::stable_sort(ord.begin(), ord.end(), [&targets](size_t a, size_t b) {
                return sacrificeCostRank(targets[a]) < sacrificeCostRank(targets[b]);
            });
            vector<Targetable *> t2;
            vector<string> o2, n2;
            for (size_t k = 0; k < ord.size(); k++)
            {
                t2.push_back(targets[ord[k]]);
                o2.push_back(opts[ord[k]]);
                n2.push_back(narrOpts[ord[k]]);
            }
            for (size_t k = ord.size(); k < opts.size(); k++)
                o2.push_back(opts[k]); //the "Done - no further targets" escape
            targets.swap(t2);
            opts.swap(o2);
            narrOpts.swap(n2);
        }

        //Dungeon SELECTION (N-146c): the first venture reaches this target seam
        //with the sideboard dungeons as the candidate set. The generic "TARGET
        //CHOICE ... pick the ONE target it will affect" wording mis-frames it as
        //targeting a permanent (it is choosing WHICH dungeon to venture into).
        //Detect the shape (every candidate is a Dungeon) and reframe as a venture
        //choice. Representation only - the offered set and apply path are unchanged;
        //describeTarget already renders each dungeon's full room path + summary.
        bool dungeonSelect = !targets.empty();
        for (size_t i = 0; i < targets.size() && dungeonSelect; i++)
        {
            MTGCardInstance * dc = dynamic_cast<MTGCardInstance *>(targets[i]);
            if (!dc || !dc->hasType(Subtypes::TYPE_DUNGEON))
                dungeonSelect = false;
        }
        //#W68-BC (MED, deck146 s51): the LEGEND RULE pick. MTGNewLegend::
        //MoveLegend builds a `*[-mutated;legendary;share!name!]|mybattlefield`
        //chooser over an AAMover-to-owner's-graveyard, so the seat met it as a
        //bare "TARGET CHOICE for Lolth, Spider Queen - its \"put a card into
        //the graveyard\" ability" over two identically-described rows: the RULE
        //was never stated and neither copy's loyalty was printed. The reply
        //invented the merge ("4 + 4 = 8"). Detected off engine facts only -
        //two or more candidates, every one a legendary permanent on MY
        //battlefield sharing the source's name.
        bool legendRuleSelect = targets.size() >= 2 && tc->source;
        for (size_t i = 0; i < targets.size() && legendRuleSelect; i++)
        {
            MTGCardInstance * lc = dynamic_cast<MTGCardInstance *>(targets[i]);
            if (!lc || lc->controller() != this
                || !lc->hasType(Subtypes::TYPE_LEGENDARY)
                || lc->getName() != tc->source->getName())
                legendRuleSelect = false;
        }
        //#W60-O (B12): the source's own text, when it conditions on ONE of the
        //dungeons on this menu by name, rides that dungeon's row. Read off the
        //printed text of the card that is venturing (walked through the
        //ability$! payload dummies the same way effectName is), never asserted
        //from this code - if no offered dungeon appears in that text, no row
        //moves. Append-only: the option ORDER, the answer index and the apply
        //path are untouched.
        if (dungeonSelect && !effectName.empty() && effectName != "this effect")
        {
            string srcText;
            {
                vector<MTGCardInstance *> chain;
                MTGCardInstance * sc = tc->source;
                for (int d = 0; sc && d < 4; d++, sc = sc->storedSourceCard)
                    chain.push_back(sc);
                MTGAbility * wv = dynamic_cast<MTGAbility *>(observer->mLayers->actionLayer()->isWaitingForAnswer());
                sc = wv ? wv->source : NULL;
                for (int d = 0; sc && d < 4; d++, sc = sc->storedSourceCard)
                    chain.push_back(sc);
                for (size_t ci = 0; ci < chain.size(); ci++)
                    if (chain[ci]->getDisplayName() == effectName && !chain[ci]->text.empty())
                    {
                        srcText = chain[ci]->text;
                        break;
                    }
            }
            //#W61-T (C9): B12's clause RENDERS (wave-60 engine seat: 1 of 1 on
            //the Acererak ask) and the seat still took Lost Mine of Phandelver
            //14 of 14. The clause is not absent and it is not on the wrong row -
            //it is ASYMMETRIC: exactly one row carried a quote and the other two
            //carried nothing, and an annotation that appears on one row of three
            //reads as decoration rather than as a fact about the menu. Same
            //shape as the #W57-C discard finding ("while two dead cards render
            //differently no guide can teach which row is the keep"). So when the
            //venturing source's text names ANY offered dungeon, every offered
            //dungeon says which side of that line it is on. Still append-only,
            //still quoted from the card and never asserted by this code.
            if (!srcText.empty())
            {
                vector<string> ventureSentences(targets.size());
                bool anyNamed = false;
                for (size_t i = 0; i < targets.size(); i++)
                    if (MTGCardInstance * dc = dynamic_cast<MTGCardInstance *>(targets[i]))
                    {
                        ventureSentences[i] = sentenceNaming(srcText, dc->getDisplayName(), 220);
                        if (!ventureSentences[i].empty())
                            anyNamed = true;
                    }
                if (anyNamed)
                    for (size_t i = 0; i < targets.size() && i < opts.size(); i++)
                    {
                        if (!dynamic_cast<MTGCardInstance *>(targets[i]))
                            continue;
                        if (!ventureSentences[i].empty())
                            opts[i] += ventureSourceDungeonTag(effectName, ventureSentences[i]);
                        else
                            opts[i] += ventureSourceSilentTag(effectName);
                    }
            }
        }

        //N-139a (wave-29 deck139): the mutate cast is a scrambled multi-ask
        //sequence (cast -> over/under -> TARGET), and the mutate-HOST target step
        //reached the model as the same bare "TARGET CHOICE ... target it will
        //affect" line as any removal - so the model spent minutes re-deriving what
        //it was choosing. The over/under placement is asked BEFORE this step (engine
        //order), so by now it is decided; name the ROLE of THIS step. The waiting
        //ability is an AANewTarget carrying the placement (1 over / 2 under).
        int mutatePlacement = 0;
        {
            MTGAbility * waiting = dynamic_cast<MTGAbility *>(observer->mLayers->actionLayer()->isWaitingForAnswer());
            if (AANewTarget * nt = dynamic_cast<AANewTarget *>(waiting))
                mutatePlacement = nt->mutation;
        }
        //W38: in the live corpus the waiting element is a WRAPPER whose menu
        //text is the parser-lowercased label ("mutate over"), not the
        //AANewTarget itself - the cast yielded nothing and the host ask fell
        //to the generic TARGET CHOICE header (wave-37 139v152 s31). The
        //rendered ability name states the same fact; read placement from it.
        if (!mutatePlacement)
            mutatePlacement = mutatePlacementFromLabel(abilityName);

        //Target sub-menus reached the model as a bare "Choose ... for X" line
        //that read like a phase or cast decision, owning most of the corpus's
        //fallbacks: the pilot answered with an attack PLAN ("Choose the target
        //for Web" -> out-of-range CHOICE: 0, deck62 N1), echoed the SPELL name
        //as its pick instead of the target (deck49 E-49b, all 7 stale_echoes),
        //or invented "Cast nothing" on a mandatory chooser (deck135 s4). Frame
        //the step explicitly: name the pending spell/ability, state it is a
        //TARGET pick for something already on the stack (not a cast/phase step),
        //and tell the model to answer with the TARGET's name. Representation
        //only - the offered set and the apply path are unchanged.
        std::ostringstream q;
        if (forcedSelfLoss)
        {
            q << buildHandRemovalAsk(lossVerb, !selfInflicted, relocateNotLoss,
                                     effectName, multi, unlimited, tc->targetMin,
                                     tc->maxtargets, picks.size());
        }
        else if (forcedSelfGain)
        {
            q << buildHandGainAsk(gainDest, effectName, multi, unlimited,
                                  tc->targetMin, tc->maxtargets, picks.size());
        }
        else if (mutatePlacement)
        {
            q << mutateHostAsk(effectName, mutatePlacement);
            //W38 host-intent carry: if the plan stated WITH the over/under
            //answer names exactly ONE of the host candidates, render that
            //commitment back (annotation, never automation - the model sees
            //its own stated intent and may still override it).
            if (effectName == mMutateIntentCard && !mMutateIntentPlan.empty())
            {
                vector<string> candNames;
                for (size_t ci = 0; ci < targets.size(); ci++)
                    if (MTGCardInstance * cc = dynamic_cast<MTGCardInstance *>(targets[ci]))
                        candNames.push_back(cc->getDisplayName());
                string named = uniqueNamedIn(mMutateIntentPlan, candNames);
                if (!named.empty())
                    q << " At the over/under step your PLAN named " << named
                      << " as the host - you may still pick differently, but that"
                         " was your stated intent.";
            }
        }
        else if (forcedSacrifice)
        {
            q << buildForcedSacrificeAsk(effectName, sacByOpponent, sacGain,
                                         sacEngineRows); //#W66-AQ (H10)
        }
        else if (dungeonSelect)
        {
            //#W60-O (B12): name the venturing SOURCE. Eleven of eleven venture
            //asks across two waves rendered this header with no source at all.
            q << "VENTURE";
            if (!effectName.empty() && effectName != "this effect")
                q << " with " << effectName;
            //#W61-T (C9): the header used to tell the model to weigh "how many
            //rooms to completion" against a tag that carried the printed ROOM
            //LIST length - a different quantity, because a dungeon's rooms
            //branch. The tag now states VENTURES to completion off the engine's
            //explore ladder; the instruction names that same quantity.
            q << " - CHOOSE A DUNGEON to enter (you are picking WHICH dungeon"
                 " to venture into, NOT targeting a permanent). Each option below is a"
                 " dungeon; its tag shows how many VENTURES complete it and what the"
                 " completion reward is, and its full room path follows. A dungeon's"
                 " rooms BRANCH, so the number of rooms printed is not the number of"
                 " ventures it takes. Weigh the ventures to completion and whether the"
                 " completion payoff and the rooms en route fit your plan"
                 " (a dungeon whose completion turns on your payoffs is usually worth"
                 " the shorter path). Pick the ONE dungeon to venture into, and answer"
                 " with its name.";
        }
        else
        {
        if (stackTrapNote)
        {
            //#W64-AH (F5): the verb off the waiting ability, the decline
            //sentence off whether this list actually carries one.
            string trapVerbLabel;
            if (AAMover * trapMv = dynamic_cast<AAMover *>(waiting))
                trapVerbLabel = trapMv->getMenuText(tc);
            else if (waiting)
                trapVerbLabel = waiting->getMenuText();
            q << stackTrapNoteText(stackTrapEffectGerund(trapVerbLabel), mayStop);
        }
        if (legendRuleSelect) //#W72-BV (M12): its own header, not the stack one
            q << legendRuleHeaderText(tc->source ? tc->source->getName() : string(),
                                      (int) targets.size());
        else
        {
        q << "TARGET CHOICE for " << effectName;
        if (!abilityName.empty() && abilityName != effectName)
            q << " - its \"" << abilityName << "\" ability";
        q << " (this spell/ability is already on the stack and needs a target - "
          << "it is NOT a cast or phase step). Pick ";
        if (!multi)
            q << "the ONE target it will affect";
        else
        {
            q << "target " << (picks.size() + 1);
            if (!unlimited)
                q << " of " << (tc->targetMin ? "exactly " : "up to ") << tc->maxtargets;
        }
        q << " from the list below, and answer with the chosen TARGET's row number (not"
             " the row for \"" //#W70-BL (E3)
          << effectName << "\")";
        //N-146q: a compound mode's target ask names which part this pick feeds.
        q << compoundModeTargetNote(abilityName);
        } //#W72-BV (M12): end of the generic stack-target header
        //#W68-BC (MED, deck146 s51): name the rule this pick IS. Append-only:
        //no row is added, removed or reordered, and the answer index is
        //untouched - the model is told what it is deciding and what the choice
        //does NOT do.
        if (legendRuleSelect)
            q << legendRuleTargetClause(tc->source->getName(), (int) targets.size());
        }

        //W35 owner ruling class (3): the target ask's own instructional text
        //(and its rules-text tails) never enter the log - the RECORD is the
        //pick: "You targeted <X> with <source>'s <ability>".
        {
            vector<string> narr;
            for (size_t ti = 0; ti < opts.size(); ti++)
                narr.push_back(ti >= narrOpts.size()
                               ? ("You chose no further target for " + effectName)
                               : targetChoiceNarration(stripNarrationDecoration(narrOpts[ti]),
                                                       effectName, abilityName));
            setAskNarration(narr);
        }
        //Pass the pending source name so parseChoice can strip a "<spell>
        //targeting <target>" echo prefix down to the target name (stale-echo
        //family A). effectName is the exact spell/ability named in the prompt.
        //#W81-DK (V1): the legend rule is a STATE-BASED ACTION and its answer sends a
        //permanent to the graveyard - neither re-serve cache may answer it. The scope
        //is taken around the call only (RAII), so nothing else on this seam moves.
        int pick;
        {
            StateBasedActionAskScope w81Sba(this, legendRuleSelect);
            pick = askModel(q.str(), opts, true, effectName);
        }
        if (pick == kChoicePending)
            return 1; //chooser stays open; earlier picks re-derive from cache next tick
        //W38: the host ask is decided (model or heuristic) - the carried
        //intent is consumed; never let it leak onto a later mutate.
        if (mutatePlacement)
        {
            mMutateIntentCard.clear();
            mMutateIntentPlan.clear();
        }
        if (pick < 0)
        {
            //Model deferred (or transport failed). With nothing selected the
            //heuristic can own the whole decision; mid-selection of an
            //exact-N chooser it must too (a partial set would strand the
            //spell), while an "up to N" selection can simply stop here.
            if (picks.empty() || tc->targetMin)
                return AIPlayerBaka::chooseTarget(_tc, forceTarget, chosenCard, checkOnly);
            break;
        }
        if (mayStop && pick == (int) targets.size())
            break;
        picks.push_back(targets[pick]);
        if (!multi)
            break;
        if (!unlimited && (int) picks.size() >= tc->maxtargets)
            break;
        if (unlimited && picks.size() >= 12)
            break; //sanity bound for "any number of targets"
    }

    if (picks.empty())
        return AIPlayerBaka::chooseTarget(_tc, forceTarget, chosenCard, checkOnly);

    DebugTrace("AIPlayerGPT: targeting with " << tc->source->getDisplayName()
               << " -> " << picks.size() << " target(s), first: "
               << describeTarget(this, picks[0]));
    //The manager owns the click choreography (source-first ordering,
    //player clicks, card batch with done/autoChoice) and re-validates the
    //picks against the live chooser. chosenCard preserves the historical
    //"card already clicked upstream" suppression for single card targets.
    DecisionAction act;
    act.targets = picks;
    DecisionManager::applyChooseTarget(req, act, chosenCard != NULL);
    return 1;
}


//Significant name-words of a reply SEGMENT (lowercase, len>=4, minus the
//combat/protocol filler) - the tokens that select a named creature. Shared
//by the block name->label reconciler; mirrors parseAttackerSet's inline
//split and parseChoice's echo split.
static void significantWords(const string& seg, vector<string>& words)
{
    string w;
    for (size_t k = 0; k <= seg.size(); k++)
    {
        char c = (k < seg.size()) ? (char) tolower((unsigned char) seg[k]) : ' ';
        if (isalnum((unsigned char) c))
            w += c;
        else
        {
            if (w.size() >= 4 && w != "attack" && w != "with" && w != "cast"
                && w != "play" && w != "none" && w != "hold" && w != "pass"
                && w != "nobody" && w != "block" && w != "blocks")
                words.push_back(w);
            w.clear();
        }
    }
}


//Strip a leading protocol ANSWER LABEL ("BLOCKS:", "ATTACK:", "CHOICE:",
//"PUT:", "BLOCK:") - with any markdown decoration - off a reply SEGMENT.
//(history: comment-archaeology.md AIPlayerGPTSeams-stripAnswerLabelPrefix-2194)
string stripAnswerLabelPrefix(const string& seg)
{
    static const char * kLabels[] = { "blocks:", "block:", "attack:", "attacks:", "choice:", "put:" };
    size_t s = 0;
    while (s < seg.size() && (seg[s] == ' ' || seg[s] == '\t' || seg[s] == '*'
                              || seg[s] == '#' || seg[s] == '-' || seg[s] == '>'
                              || seg[s] == '`'))
        s++;
    for (size_t k = 0; k < sizeof(kLabels) / sizeof(kLabels[0]); k++)
    {
        size_t len = strlen(kLabels[k]);
        if (seg.size() - s < len)
            continue;
        bool match = true;
        for (size_t c = 0; c < len && match; c++)
            match = (tolower((unsigned char) seg[s + c]) == kLabels[k][c]);
        if (match)
            return seg.substr(s + len);
    }
    return seg;
}


//A trailing "#N" disambiguation ordinal on a reply segment: the model marks
//WHICH of several identically-named creatures it means ("Saproling (1/1) #1";
//deck135 wave-19 s27). Returns N (>=1) after the LAST '#', or 0 if none. The
//significant-word split already discards the '#N' and '(P/T)' as sub-4-char
//tokens, so the name match is unaffected; this recovers the ordinal the split
//throws away, to break a same-name tie the parser would otherwise drop.
//#W54-M (L7): one bounded digit-run reader for every label parser below. A
//run longer than the cap SATURATES at the cap (100000: no window has that
//many rows, no pump is that size), so a reply like "B4294967297:A1" can no
//longer wrap to B1 and declare a blocker the model never named - every
//caller's own range check then rejects it exactly as it rejects "B100000".
//'pos' advances past the whole run whatever its length.
static int readDigits(const string& s, size_t& pos, size_t end = string::npos)
{
    const int cap = 100000;
    if (end > s.size())
        end = s.size();
    int n = 0;
    while (pos < end && isdigit((unsigned char) s[pos]))
    {
        if (n < cap)
            n = n * 10 + (s[pos] - '0');
        if (n > cap)
            n = cap;
        pos++;
    }
    return n;
}

int nameOrdinal(const string& seg)
{
    size_t h = seg.rfind('#');
    if (h == string::npos)
        return 0;
    size_t k = h + 1;
    while (k < seg.size() && (seg[k] == ' ' || seg[k] == '\t'))
        k++;
    const size_t k0 = k;
    int n = readDigits(seg, k); //#W54-M (L7)
    return k > k0 ? n : 0;
}


//The listed name whose lowercased form contains ALL the given words, optionally
//restricted to a set of allowed indices. With a unique match, returns it.
//AMBIGUITY (duplicate creature names) normally drops the assignment (-1, the
//gang-block disambiguation guard) - EXCEPT when the reply carried a "#N"
//ordinal (ordinal>0): among the same-named matches, in label order, the N-th
//is the model's intent, so it is selected instead of dropped (identically
//named creatures blocking are interchangeable, so honoring the pick the model
//made is faithful, never a guess). Zero matches always -> -1.
int uniqueNameMatch(const vector<string>& words, const vector<string>& names,
                           size_t limit, const vector<int> * allowed,
                           int ordinal)
{
    if (words.empty())
        return -1;
    vector<int> matches;
    for (size_t o = 0; o < names.size() && o < limit; o++)
    {
        if (allowed)
        {
            bool ok = false;
            for (size_t a = 0; a < allowed->size(); a++)
                if ((*allowed)[a] == (int) o) { ok = true; break; }
            if (!ok)
                continue;
        }
        string low = names[o];
        for (size_t k = 0; k < low.size(); k++)
            low[k] = (char) tolower((unsigned char) low[k]);
        bool all = true;
        for (size_t k = 0; k < words.size() && all; k++)
            all = low.find(words[k]) != string::npos;
        if (all)
            matches.push_back((int) o);
    }
    if (matches.size() == 1)
        return matches[0];
    if (matches.size() > 1 && ordinal >= 1 && ordinal <= (int) matches.size())
        return matches[ordinal - 1]; //"#N" disambiguates equivalent duplicates
    return -1; //zero matches, or ambiguous with no ordinal to break the tie
}


//#W45-3 BLOCK-TRIGGERED LIFE. The combat preview modeled BODIES and never LIFE
//that does not ride a keyword on either combatant. `Perimeter Captain (0/4)
//(neither dies)` was the whole forecast for a block that also gains its
//controller 2 life per blocking defender, and `Pride Guardian` 3 - two decided
//games (deck146 vs126, the opponent racing 13 -> 42 while the AI read every
//wall as inert). The trigger sits on a THIRD permanent, not on the blocker, so
//the lifelink clause below - the shape this reuses - could not see it.
//
//One auto= line, classified. Returns true only for the shape this preview can
//price honestly: a top-level "whenever ~ blocks" trigger whose whole payload is
//a life gain for the trigger's own controller. `sourceSpec` is the filter that
//decides WHICH blocking creature fires it, `amountExpr` the unevaluated amount,
//`optional` the script's own "may".
//Deliberately strict - every rejection below is a case where a rendered number
//would be a claim this cannot support:
//  * the trigger must OPEN the line: a granted/nested copy (`transforms((,
//    newability[@combat(blocking)...]))`, an emblem, a lord) lives on a card
//    that is not the trigger's real source, so the source filter would be
//    resolved against the wrong permanent.
//  * `restriction{...}` is a condition evaluated at trigger time, `thisforeach`
//    a per-counter multiplier, `thatmuch` the resolution-time event variable and
//    `rand` the game RNG - which rendering must never draw (standing rule).
//  * the payload must be a life GAIN to the controller. `life:-N` is a loss and
//    `opponent` is the other seat; both are real cards (Vraska's, the demon
//    upkeep forms) and both would be voiced backwards by a "gain" clause.
//Pure over the lowercased script line, so PARSETEST proves it without a game.
static bool blockingLifeTriggerClause(const string& lowLine, string& sourceSpec,
                                      string& amountExpr, bool& optional)
{
    sourceSpec.clear();
    amountExpr.clear();
    optional = false;
    size_t s = lowLine.find_first_not_of(" \t");
    if (s == string::npos)
        return false;
    static const char kTrig[] = "@combat(blocking)";
    if (lowLine.compare(s, sizeof(kTrig) - 1, kTrig) != 0)
        return false;
    if (lowLine.find("restriction{") != string::npos
        || lowLine.find("thisforeach") != string::npos
        || lowLine.find("thatmuch") != string::npos
        || lowLine.find("rand") != string::npos)
        return false;
    //TrCombatTrigger's optional `from(...)` filters on the ATTACKER being
    //blocked (AllAbilities.h: fromTc->canTarget(blocked->opponent)), a second
    //condition this pairing-blind scan does not evaluate - so a line carrying
    //one is left unpriced rather than claimed unconditionally.
    if (lowLine.find("from(") != string::npos)
        return false;
    size_t sp = lowLine.find("source(", s);
    if (sp == string::npos)
        return false;
    size_t close = lowLine.find(')', sp + 7);
    if (close == string::npos || close <= sp + 7)
        return false;
    string spec = lowLine.substr(sp + 7, close - sp - 7);
    //The payload is whatever follows the FIRST ':' after the trigger header.
    size_t colon = lowLine.find(':', close);
    if (colon == string::npos)
        return false;
    size_t p = lowLine.find_first_not_of(" \t", colon + 1);
    if (p == string::npos)
        return false;
    if (lowLine.compare(p, 4, "may ") == 0)
    {
        optional = true;
        p = lowLine.find_first_not_of(" \t", p + 4);
        if (p == string::npos)
            return false;
    }
    if (lowLine.compare(p, 5, "life:") != 0)
        return false; //a payload that does anything else is not priced here
    p += 5;
    size_t end = lowLine.find_first_of(" \t\r\n", p);
    string expr = lowLine.substr(p, end == string::npos ? string::npos : end - p);
    if (expr.empty() || expr[0] == '-')
        return false; //a life LOSS is not what a "gain" clause may voice
    //Whatever follows the amount names the player: nothing at all is the
    //script's default (the ability's controller - Goldenglow Moth's bare
    //"life:4"), "controller" says it outright, and anything else (opponent,
    //targetedplayer, a chained "&&" clause) is a payload this does not price.
    if (end != string::npos)
    {
        size_t t = lowLine.find_first_not_of(" \t\r\n", end);
        if (t != string::npos)
        {
            size_t te = lowLine.find_first_of(" \t\r\n", t);
            string who = lowLine.substr(t, te == string::npos ? string::npos : te - t);
            if (who != "controller")
                return false;
            if (te != string::npos && lowLine.find_first_not_of(" \t\r\n", te) != string::npos)
                return false; //a second chained effect: out of scope, not guessed
        }
    }
    sourceSpec = spec;
    amountExpr = expr;
    return true;
}


//Does `spec` - the source filter of a blocking trigger on `src` - cover this
//particular blocker? "this" is the self-trigger (Pride Guardian); anything else
//is asked of the engine's own TargetChooser, the same machinery the rider gate
//uses, so a filter like `creature[defender]|mybattlefield` is resolved by the
//engine rather than re-derived from words. Fails CLOSED, unlike
//riderHasLegalTarget: an unreadable filter is not evidence the trigger fires,
//and the failure being fixed here is a MISSING fact, never an invented one.
static bool blockingTriggerCovers(MTGCardInstance * src, const string& spec,
                                  MTGCardInstance * blocker)
{
    if (!src || !blocker)
        return false;
    if (spec == "this")
        return src == blocker;
    GameObserver * obs = src->getObserver();
    if (!obs)
        return false;
    TargetChooserFactory tcf(obs);
    TargetChooser * tc = tcf.createTargetChooser(spec, src);
    if (!tc)
        return false;
    bool ok = tc->canTarget(blocker);
    SAFE_DELETE(tc);
    return ok;
}


//#W65-AN (G10, deck152 HIGH-1, `152` s12). What a BLOCKER's own lifelink
//gains its controller in the block this projection names. Lifelink gains on
//damage DEALT, so the term exists only where the blocker actually deals its
//damage: a blocker with no first strike facing a first-striker that can kill
//it outright never deals any (deathtouch makes 1 damage lethal). Every other
//way a blocker can be stopped from dealing damage - prevention, a pump, a
//removal spell in response - can only make this SMALLER, so the term is a
//fail-closed under-estimate of a figure the seat is told is a best case, which
//is the safe direction for a survival number. Pure over the seven facts, so
//PARSETEST pins the whole table without a board.
//#W65-AP (R5, wave-65 codex review finding 5 - HIGH): DOUBLE STRIKE IS TWO
//DAMAGE STEPS. Both callers pass `blkFirstStrike` as FIRSTSTRIKE||DOUBLESTRIKE,
//so a double striker was credited ONE power-sized gain: at 3 life a 3/3
//double-strike lifelink blocker facing 6 unblocked damage was priced at +3 and
//the screen projected death, where the real gain is 6 across the two steps and
//the seat survives at 3. A "no block saves you" verdict is exactly the claim
//that must never be made from an under-count, so the second step is priced.
//It is a FLOOR still: the first-strike step is always dealt (a double striker
//has first strike), and the second is credited only where the blocker is still
//there to deal it - i.e. where the attacker's own damage did not already kill
//it in the first step.
int blockerLifelinkGain(int blkPower, int blkToughness, bool blkLifelink,
                               bool blkFirstStrike, int atkPower, bool atkFirstStrike,
                               bool atkDeathtouch, bool blkDoubleStrike)
{
    if (!blkLifelink || blkPower <= 0)
        return 0;
    const int lethal = atkDeathtouch ? 1 : blkToughness;
    const bool killedByFirstStrike = atkFirstStrike && !blkFirstStrike && atkPower > 0
                                     && lethal > 0 && atkPower >= lethal;
    if (killedByFirstStrike)
        return 0; //killed before it ever deals damage: no lifelink
    if (!blkDoubleStrike)
        return blkPower;
    //Two steps. The blocker deals its first-strike damage; it deals the second
    //unless the attacker's damage in the FIRST step killed it (only possible
    //when the attacker also strikes first), in which case the attacker's own
    //regular damage never comes and neither does the blocker's second hit.
    const bool diedInFirstStep = atkFirstStrike && atkPower > 0 && lethal > 0
                                 && atkPower >= lethal;
    return diedInFirstStep ? blkPower : blkPower * 2;
}


//Total life `blocker`'s controller gains the moment this creature BLOCKS,
//summed over every permanent they control that carries such a trigger.
//Mandatory and optional ("may") gains are kept apart: a "may" is the pilot's
//own choice and stating it as certain would be the same overclaim the double-
//strike lifelink clause refuses. Scans the blocking side's battlefield only -
//`mybattlefield` and `this` are both resolved against the trigger's controller,
//so an opponent's Noble Stand does not fire on YOUR block.
void blockTriggeredLifeFor(MTGCardInstance * blocker, int& sure, int& may)
{
    sure = 0;
    may = 0;
    if (!blocker || !blocker->controller() || !blocker->controller()->game)
        return;
    MTGGameZone * bf = blocker->controller()->game->inPlay;
    if (!bf)
        return;
    for (int i = 0; i < bf->nb_cards; i++)
    {
        MTGCardInstance * src = bf->cards[i];
        if (!src || src->magicText.find("@combat(blocking)") == string::npos)
            continue;
        const string& mt = src->magicText;
        size_t lp = 0;
        while (lp <= mt.size())
        {
            size_t nl = mt.find('\n', lp);
            string line = mt.substr(lp, nl == string::npos ? string::npos : nl - lp);
            lp = (nl == string::npos) ? mt.size() + 1 : nl + 1;
            string spec, expr;
            bool optional = false;
            if (!blockingLifeTriggerClause(line, spec, expr, optional))
                continue;
            if (!blockingTriggerCovers(src, spec, blocker))
                continue;
            //Same WParsedInt path the lifelink/drain magnitudes use - the value
            //the resolution itself will read, off the trigger's own source.
            WParsedInt val(expr, NULL, src);
            int n = val.getValue();
            if (n <= 0)
                continue;
            (optional ? may : sure) += n;
        }
    }
}


//#W45-3, the second half: a "whenever you gain life, that player loses that
//much life" converter (Sanguine Bond and its class) turns every one of the
//gains above into DAMAGE, which is how deck146's opponent went 13 -> 42 while
//taking single turns of +12 and +13. Matched on the script rather than the
//name: the trigger must OPEN the line, and its payload must be a life LOSS
//aimed at the opponent. Exquisite Blood (`@lifelostfoeof(player):life:thatmuch
//controller`) is the mirror, not a converter, and is deliberately not matched.
//Pure over the lowercased script, so PARSETEST proves both halves.
bool lifeToDamageConverterScript(const string& magicText)
{
    size_t lp = 0;
    while (lp <= magicText.size())
    {
        size_t nl = magicText.find('\n', lp);
        string line = magicText.substr(lp, nl == string::npos ? string::npos : nl - lp);
        lp = (nl == string::npos) ? magicText.size() + 1 : nl + 1;
        size_t s = line.find_first_not_of(" \t");
        if (s == string::npos || line.compare(s, 8, "@lifeof(") != 0)
            continue;
        size_t colon = line.find(":life:-", s);
        if (colon == string::npos)
            continue;
        if (line.find("opponent", colon) != string::npos)
            return true;
    }
    return false;
}


//#W74-CF (F5, Astra review finding 5). THE CONVERTER HAS A RATE, AND THE RATE IS
//NOT ALWAYS ONE-FOR-ONE. `lifeToDamageConverterScript` is a PREDICATE - it
//accepts Sanguine Bond (`:life:-thatmuch opponent`, every point gained is a point
//lost) and Cliffhaven Vampire (`:life:-1 opponent`, ONE life per gain EVENT,
//whatever the size of the gain) alike, and O5's caller used it as proof that
//every lifelink point converts. On a 4-power lifelink attacker into an opponent
//at 8, Cliffhaven converts 1, not 4: the row printed 0 life (lethal) where the
//true unblocked result is 3 - a false fact in the dangerous direction, which is
//exactly the class O5 exists to close. This function returns the rate instead of
//a yes/no: `proportional` for the `thatmuch` form, otherwise the fixed number of
//life the opponent loses per gain event. Pure over the script text.
static int lifeToDamageConverterRate(const string& magicText, bool& proportional)
{
    proportional = false;
    int fixed = 0;
    size_t lp = 0;
    while (lp <= magicText.size())
    {
        size_t nl = magicText.find('\n', lp);
        string line = magicText.substr(lp, nl == string::npos ? string::npos : nl - lp);
        lp = (nl == string::npos) ? magicText.size() + 1 : nl + 1;
        size_t s = line.find_first_not_of(" \t");
        if (s == string::npos || line.compare(s, 8, "@lifeof(") != 0)
            continue;
        size_t colon = line.find(":life:-", s);
        if (colon == string::npos)
            continue;
        if (line.find("opponent", colon) == string::npos)
            continue;
        size_t d = colon + 7; //past ":life:-"
        if (line.compare(d, 8, "thatmuch") == 0)
        {
            proportional = true;
            continue;
        }
        int n = 0;
        bool digits = false;
        while (d < line.size() && isdigit((unsigned char) line[d]))
        {
            n = n * 10 + (line[d++] - '0');
            digits = true;
        }
        if (digits && n > 0)
            fixed += n;
    }
    return proportional ? 0 : fixed;
}


//#W49-U D5: the MIRROR half (Exquisite Blood: `@lifelostfoeof(player):life:
//thatmuch controller` - whenever an opponent loses life, its controller gains
//that much). Alone it is a gain engine; next to a converter of the same side
//it closes the loop. The trigger must OPEN the line and its payload must be a
//`thatmuch` gain for the controller (an "opponent" payload would be a drain,
//not the mirror). Pure over the lowercased script.
bool lifeLossMirrorScript(const string& magicText)
{
    size_t lp = 0;
    while (lp <= magicText.size())
    {
        size_t nl = magicText.find('\n', lp);
        string line = magicText.substr(lp, nl == string::npos ? string::npos : nl - lp);
        lp = (nl == string::npos) ? magicText.size() + 1 : nl + 1;
        size_t s = line.find_first_not_of(" \t");
        if (s == string::npos || line.compare(s, 15, "@lifelostfoeof(") != 0)
            continue;
        size_t colon = line.find(":life:thatmuch", s);
        if (colon == string::npos)
            continue;
        if (line.find("opponent", colon) == string::npos)
            return true;
    }
    return false;
}


//#W49-U D5: does this player hold BOTH halves (a converter and a mirror)?
bool playerHasLifeLoop(Player * p)
{
    if (!p || !p->game || !p->game->inPlay)
        return false;
    bool conv = false, mir = false;
    MTGGameZone * bf = p->game->inPlay;
    for (int i = 0; i < bf->nb_cards; i++)
    {
        MTGCardInstance * c = bf->cards[i];
        if (!c)
            continue;
        conv = conv || lifeToDamageConverterScript(c->magicText);
        mir = mir || lifeLossMirrorScript(c->magicText);
    }
    return conv && mir;
}


//#W62-AA (R6, wave-62 codex review finding 6): holding both halves is not the
//same as being able to CLOSE them. The chain is "I gain life -> they lose that
//much -> I gain that much -> ..."; it stops dead at either end. If their side
//reads cantchangelife / cantlifelose / cantlose their life does not move, and
//if this seat cannot gain life (nolifegain here, nolifegainopponent there) the
//converter is never fed. Wave 62 inferred THIS WINS THE GAME from the two
//cards alone, so a seat under Teferi's Protection was told an unlimited chain
//would kill it. Fail closed, as the C1 rule does for unpriced punishers: no
//win claim unless the board says the chain can run. The rule is pure so
//PARSETEST pins the whole table.
bool lifeLoopWinnable(bool loopClosed, bool theirLifeCanChange, bool iCanGainLife)
{
    return loopClosed && theirLifeCanChange && iCanGainLife;
}

//The board reader behind it. Every fact is a zone-level ability the prompt's
//own battlefield lines already show.
bool lifeLoopProvenWin(Player * me)
{
    if (!playerHasLifeLoop(me))
        return false;
    Player * them = me->opponent();
    if (!them || !them->inPlay() || !me->inPlay())
        return false; //cannot read the other side: claim nothing
    MTGGameZone * th = them->inPlay();
    MTGGameZone * mi = me->inPlay();
    const bool theirLifeCanChange = !th->hasAbility(Constants::CANTCHANGELIFE)
                                    && !th->hasAbility(Constants::CANTLIFELOSE)
                                    && !th->hasAbility(Constants::CANTLOSE);
    const bool iCanGainLife = !mi->hasAbility(Constants::CANTCHANGELIFE)
                              && !mi->hasAbility(Constants::NOLIFEGAIN)
                              && !th->hasAbility(Constants::NOLIFEGAINOPPONENT);
    return lifeLoopWinnable(true, theirLifeCanChange, iCanGainLife);
}


//The A-row clause for it (attackers window): the loop is THEIRS, so every life
//they gain off this combat, or every point you lose to it, chains. Worded from
//the mechanism (gain-or-loss chains), not from a prediction about damage.
static const char * kLifeLoopAttackerRowTag =
    " (their life LOOP is in play: any life they gain or you lose in this combat"
    " chains without limit - fatal to you, not a trade)";


//#W65-AN (G6, deck123 HIGH-1 / deck146 HIGH-1 / deck162 HIGH-1). The clause
//above was stamped on every attacker row wherever the board held the two
//halves, and it OVERRODE the verdict printed on the same screen. `123v126`
//seq 48: 51 creatures, `ATTACK TOTAL ... KILLS them whatever they block` and
//the "fatal to you" clause on every unblockable row at once; the seat answered
//`ATTACK: none` and lost 43-0. Two clauses that contradict each other in one
//window is the same class of false surface F9 fixed on the blockers header:
//the one that stops the action wins, and here it is the one that is WRONG.
//A life loop is a reason to fear a long game, never a reason to decline a
//kill - the chain cannot fire after they are dead - so where the ATTACK TOTAL
//has PROVEN the attack lethal (its own kill branch, which is already withheld
//under an unpriced attack punisher and under the loop's own no-claim branch)
//the clause says the order of resolution instead of the caution.
//`reachable` is the caller's per-row answer to "can a branch of the chain be
//entered from THIS row at all"; with nothing of theirs able to block this
//attacker, declaring it gains them no life off a block and the row's own
//decision enters the chain nowhere (`146v126` seq 29 stamped the clause on
//three rows against a CREATURELESS opponent, where the same prompt said every
//attacker was unblockable). Nothing else is deleted: the window's LOOP SCOPE
//paragraph and the loop banner are untouched, so the board fact is still on
//the screen. Pure over the two flags, so both faces are pinned.
static string lifeLoopAttackerRowTag(bool reachable, bool attackProvenLethal)
{
    if (!reachable)
        return "";
    if (attackProvenLethal)
        return " (their life LOOP is in play, but LETHAL COMES FIRST: the ATTACK"
               " TOTAL on this screen proves this attack kills them whatever they"
               " block, and the loop cannot fire after they are dead - the chain"
               " is a reason to FINISH this combat, not a reason to hold back)";
    return kLifeLoopAttackerRowTag;
}


//#W47-R3 (wave-46 ledger, HIGH). Does this player control a life-to-damage
//converter right now? The board question behind the per-tag binding below.
//Same scan converterSituationLine runs for the CURRENT SITUATION paragraph, so
//the tag and the paragraph can never disagree about whether one is in play.
bool playerHasLifeToDamageConverter(Player * p)
{
    if (!p || !p->game || !p->game->inPlay)
        return false;
    MTGGameZone * bf = p->game->inPlay;
    for (int i = 0; i < bf->nb_cards; i++)
    {
        MTGCardInstance * c = bf->cards[i];
        if (c && lifeToDamageConverterScript(c->magicText))
            return true;
    }
    return false;
}


//A terse, NAIVE single-block combat-trade preview: what happens if this ONE
//blocker blocks this ONE attacker, alone. The model keeps re-deriving
//first-strike / deathtouch / trample math it distrusts (deck35 wave-18: an
//11k-char loop re-computed a rendered P/T until it emitted an unparseable
//reply); handing it the outcome removes the surface it re-derives. Ignores
//gang-blocks, pump, damage prevention and regeneration - the section header
//hedges "before other blockers/tricks". Perspective: 'you' = the AI (the
//blocking side). Empty string when there is nothing decisive to say.
//Plain-stat combatant view for the trade preview, so the outcome logic is
//unit-testable without an engine-built MTGCardInstance (see runParseSelfTest).
//'wither' is set for wither OR infect (both deal -1/-1-counter damage);
//'infectLabel' only picks the word for the message.

//W41-5 PREVENTION KINDS. Damage-prevention replacement effects (Fog Bank's
//`preventAllCombatDamage to(this)` + `from(this)`, Guard Gomazoa, protection,
//Circle-of-Protection shields) sit BETWEEN power and death, and the trade
//preview computed straight through them: Fog Bank read "(your blocker dies,
//attacker lives)" against attackers that cannot damage it at all, and the
//engine refuted its own annotation inside one game (146v162 s5 - the Bank
//blocked a deathtouch first-striker and SURVIVED). The trust doctrine makes
//that printed lie an instruction, so the predictor now asks the engine.
//  kPreventNone - nothing prevents this damage.
//  kPreventFull - the damage is prevented ENTIRELY: none is dealt, so nothing
//                 dies from it and NOTHING THAT RIDES DEALT DAMAGE happens.
//                 Verified against the engine, not assumed: Damage::resolve
//                 returns before emitting WEventDamage when a replacement
//                 zeroes the damage, and MTGDeathtouchRule fires only on that
//                 event with damage > 0 - so prevention beats deathtouch here
//                 exactly as 702.2b says it should. Wither/infect counters ride
//                 dealt damage too and are likewise not applied.
//  kPreventPartial - a prevention effect applies but its residue is not
//                 exactly computable in a 1-on-1 preview (a finite "prevent the
//                 next N" shield, absorb, phantom, wilting/vigor conversions).
//                 The preview must NOT guess in either direction: it states the
//                 naive outcome and flags the omission explicitly, the same
//                 shape as the uncomputable becomes-blocked trigger below.
//                 Silent omission is the worse failure - the model confabulates
//                 rules into gaps.
extern const int kPreventNone = 0;

static const int kPreventFull = 1;

static const int kPreventPartial = 2;


//The pure trade-outcome logic. Perspective: 'b' = the blocking side, 'a' = the
//attacking side. Empty-ish returns handled by the caller.
//'preventAtoB' / 'preventBtoA' are the prevention kinds for each DIRECTION of
//combat damage between these two creatures; 'preventAtoFace' is the kind for
//the attacker's damage to the DEFENDING PLAYER, which is what the
//trample-through claim rides.
//W42-3: the MATH is seat-independent; only the PRONOUNS are not. 'attackerSeat'
//re-voices the same verdict for the attackers window, where 'a' is the reader's
//creature and 'b' is the opponent's - a true statement in the wrong scope is a
//lie, and the attackers window had no statement at all (the model derived its
//own and got "Silencer dies to Battlement" and a hallucinated deathtouch). One
//source of truth for the outcome; two voicings of it.
//W44 TRAMPLE LETHAL BASIS. 'bRemaining' is the blocker's REMAINING toughness
//(MTGCardInstance::life - marked damage already counted); -1 means "not known,
//use printed toughness", which is what the pure struct-only unit tests pass.
//It exists only for the trample-through clause, and only because CR 702.19b
//makes marked damage - and ONLY marked damage - shrink the lethal cut a blocker
//soaks. GuiCombat::autoaffectDamage computes the real assignment off card->life
//for exactly that reason, so a preview that priced printed toughness would
//UNDERSTATE the carry-over against a pre-damaged blocker: 6 power into a 4/4
//with 2 marked tramples 4, not 2. The trust doctrine makes the printed number
//an instruction, so it has to be the engine's number. Deliberately NOT wired
//into the who-dies verdicts above: those price printed toughness today
//(a pre-existing scope, unchanged here).
//(history: comment-archaeology.md AIPlayerGPTSeams-combatTradePreviewStats-2208)
//#W56-B (D13): `outBlockTrigger` and `outBlockerDies` are the D13 seam. With
//outBlockTrigger non-NULL the blocking-trigger gain is NOT nested inside the
//survival verdict - it is handed back so the caller can print it as its own
//annotation - and outBlockerDies reports whether the blocker of this pairing
//dies, which is the standing cost the caller then prices. Both default NULL,
//so every existing caller renders byte-identically.
string combatTradePreviewStats(const CombatTradeStat& b, const CombatTradeStat& a,
                                      int preventAtoB,
                                      int preventBtoA,
                                      int preventAtoFace,
                                      bool attackerSeat,
                                      int bRemaining,
                                      bool bGainConverted,
                                      string * outBlockTrigger,
                                      bool * outBlockerDies,
                                      string * outBlockerLifelink,
                                      string * outAttackerLifelink,
                                      bool * outAttackerDies, //#W63-AB (E3)
                                      bool foeLifeLoop) //#W63-AB (E1)
{
    int bp = b.power > 0 ? b.power : 0;
    int ap = a.power > 0 ? a.power : 0;
    int bt = b.toughness;
    int at = a.toughness;
    const bool aStopped = (preventAtoB == kPreventFull);
    const bool bStopped = (preventBtoA == kPreventFull);

    //Base lethality (before first-strike ordering): does X's damage kill Y?
    //Deathtouch makes ANY damage lethal; normal damage is lethal at
    //power>=toughness. Both DESTROY, which indestructible prevents. Wither/
    //infect do NOT change the lethality threshold (still power>=toughness) -
    //they only change the damage FORM to -1/-1 counters, which shrink a
    //survivor and, at a lethal hit, drop toughness to 0 for a state-based
    //death that indestructible does NOT prevent. Conflating wither with
    //deathtouch made a wither blocker read as auto-lethal ("both die") even
    //when its power could not kill (deck27 wave-20 item 1: Oona's Gatewarden
    //2/1 wither vs a 3/4 -> the attacker survives as a 1/2, it does NOT trade).
    //Fully prevented damage is never dealt, so it kills nothing - deathtouch,
    //wither counters and lethal-toughness math all ride damage that lands.
    //#W82-EC (H3): the lethal threshold is the REMAINING toughness - damage
    //already marked this turn counts (CR 704.5g reads marked damage against
    //toughness; a -1/-1 counter from wither lands on a body that is already
    //part-spent the same way). `remaining` 0 = unset = undamaged. The printed
    //toughness (`at`/`bt`) still feeds the wither SHRINK figures below, which
    //are about counters, not damage.
    const int aRem = (a.remaining > 0 && a.remaining < at) ? a.remaining : at;
    const int bRem = (b.remaining > 0 && b.remaining < bt) ? b.remaining : bt;
    bool aKillsB = !aStopped && (ap > 0) && ((a.wither && ap >= bRem) || (!b.indestructible && (a.deathtouch || ap >= bRem)));
    bool bKillsA = !bStopped && (bp > 0) && ((b.wither && bp >= aRem) || (!a.indestructible && (b.deathtouch || bp >= aRem)));
    //First strike / double strike ordering: a one-sided first striker that
    //kills its foe removes that foe before it can deal (the survivor's later
    //normal-step damage lands on a dead creature). A creature killed in the
    //first-strike step deals NO damage at all - so any effect that rides its
    //damage (trample, and the wither/infect SHRINK below) must not be claimed.
    bool bDiesToFirstStrike = a.firststrike && !b.firststrike && aKillsB;
    bool aDiesToFirstStrike = b.firststrike && !a.firststrike && bKillsA;
    if (bDiesToFirstStrike)
        bKillsA = false;
    else if (aDiesToFirstStrike)
        aKillsB = false;

    if (outBlockerDies) //#W56-B (D13): the blocker of this pairing does not survive
        *outBlockerDies = aKillsB;
    //#W63-AB (E3): the other half of the material question, handed out of the
    //SAME lethality computation so the header's ranking and the row's printed
    //verdict cannot disagree about who dies.
    if (outAttackerDies)
        *outAttackerDies = bKillsA;
    std::ostringstream o;
    if (aKillsB && bKillsA)
        o << "both die";
    else if (bKillsA && !aKillsB)
        o << (attackerSeat ? "your attacker dies, their blocker lives"
                           : "you kill it, your blocker lives");
    else if (aKillsB && !bKillsA)
        o << (attackerSeat ? "you kill it, your attacker lives"
                           : "your blocker dies, attacker lives");
    else
        o << "neither dies";
    //W41-5: say WHY, immediately after the verdict it justifies. A "neither
    //dies" with no cause reads as a stats coincidence the model may re-derive
    //and disbelieve; naming the prevention makes the verdict self-supporting,
    //and the restriction ("no combat damage is dealt") leads, per the
    //annotation-wording rung.
    if (aStopped && bStopped)
        o << " (no combat damage is dealt either way - prevented)";
    else if (aStopped)
        o << (attackerSeat ? " (your attacker deals NO damage to it - prevented)"
                           : " (the attacker deals NO damage to your blocker - prevented)");
    else if (bStopped)
        o << (attackerSeat ? " (their blocker deals NO damage to your attacker - prevented)"
                           : " (your blocker deals NO damage to the attacker - prevented)");
    //Trample-through to your face (attacker assigns lethal to the blocker,
    //rest carries over) - only when the attacker actually deals (not killed
    //first by a one-sided first-strike blocker). Wither trample still assigns
    //full toughness as lethal (only deathtouch reduces the lethal cut to 1).
    //W41-5: the assignment is made BEFORE prevention applies, so a prevented
    //blocker still soaks its full lethal cut - what changes is whether the
    //carried-over damage reaches YOU, which is a separate prevention question
    //(preventAtoFace) and the only one this clause may lean on.
    //#W46-9: the carry-over is ALSO what decides whether a blocked attacker
    //reaches the defending player's life at all, which the lifelink tail below
    //has to state. Captured here rather than recomputed there.
    int trampleThrough = 0;
    if (a.trample && !(b.firststrike && !a.firststrike && bKillsA))
    {
        int lethalBasis = bRemaining >= 0 ? bRemaining : bt;
        int lethalToB = a.deathtouch ? 1 : (lethalBasis > 0 ? lethalBasis : 0);
        int through = ap - lethalToB;
        if (through > 0)
        {
            trampleThrough = through;
            if (preventAtoFace == kPreventFull)
                o << (attackerSeat ? ", but its trample damage to them is prevented"
                                   : ", but its trample damage to you is prevented");
            else if (attackerSeat && preventAtoFace == kPreventPartial)
                o << ", up to " << through << " tramples through to them"
                     " (damage to them is partly prevented - not computed here)";
            else if (preventAtoFace == kPreventPartial)
                o << ", up to " << through << " tramples to your face"
                     " (damage to you is partly prevented - not computed here)";
            else if (attackerSeat)
                o << ", " << through << " tramples through to them";
            else
                o << ", " << through << " tramples to your face";
        }
    }
    //Wither/infect that does NOT kill still SHRINKS the survivor by its damage
    //(-1/-1 counters), so the model does not read the survivor as untouched -
    //or, pre-fix, as dead. Only when the target actually survives (!kills) AND
    //the wither creature actually LIVED TO DEAL its damage: a wither blocker
    //that dies to the attacker's first strike before the normal step never
    //applies its -1/-1 counters, so the shrink must not be claimed (deck27
    //wave-24: the shrink was printed for a wither blocker already dead to first
    //strike). bDiesToFirstStrike / aDiesToFirstStrike mark that no-damage case.
    //W41-5: prevented damage deals no -1/-1 counters either, so no shrink.
    if (b.wither && bp > 0 && !bKillsA && !bDiesToFirstStrike && !bStopped)
    {
        int np = ap - bp; if (np < 0) np = 0;
        int nt = at - bp; //survives => bp < at, so nt >= 1
        o << " (" << (b.infectLabel ? "infect" : "wither")
          << (attackerSeat ? " shrinks your attacker to " : " shrinks it to ")
          << np << "/" << nt << ")";
    }
    if (a.wither && ap > 0 && !aKillsB && !aDiesToFirstStrike && !aStopped)
    {
        int np = bp - ap; if (np < 0) np = 0;
        int nt = bt - ap; //survives => ap < bt, so nt >= 1
        o << " (" << (a.infectLabel ? "infect" : "wither")
          << (attackerSeat ? " shrinks their blocker to " : " shrinks your blocker to ")
          << np << "/" << nt << ")";
    }
    //Persist: a creature that DIES here but has persist (and no -1/-1 counter
    //yet) comes straight back with a -1/-1 counter, so "both die" / "dies"
    //understates what the model keeps: the body returns, one smaller. Note it
    //on whichever side dies. Keyed on the FINAL kill flags, so a persist blocker
    //that dies to first strike still returns (death is death, order aside).
    if (aKillsB && b.persist)
        o << (attackerSeat ? " (theirs returns with a -1/-1 counter (persist))"
                           : " (yours returns with a -1/-1 counter (persist))");
    if (bKillsA && a.persist)
        o << (attackerSeat ? " (yours returns with a -1/-1 counter (persist))"
                           : " (theirs returns with a -1/-1 counter (persist))");
    //#W44-LOW (wave-43 seat-162 E-3, D4's adjacent gap): the bodies are not the
    //whole trade. A lifelink creature that DEALS its combat damage swings its
    //controller's life by that much, and a verdict of "both die" told the pilot
    //nothing about the +5 that decided the race. Rides DEALT damage, exactly as
    //trample and the wither shrink do: nothing is claimed for a creature whose
    //damage is fully prevented, or that died in the first-strike step before it
    //could deal. The attacker's full power is dealt whether or not it tramples
    //(lethal cut plus carry-over is still all of it), so 'ap' is the swing in
    //both cases. A DOUBLE striker gains in both damage steps and this preview
    //models only one, so it names the effect and withholds the number rather
    //than printing an understated one (the same omit-when-unprovable rule the
    //partial-prevention clause follows).
    //(history: comment-archaeology.md AIPlayerGPTSeams-L10738-2229)
    const char * const kFoeLoopGainTail =
        " - and both halves of their life LOOP are in play, so ANY life they gain"
        " chains without limit until you are at 0: this is not a priced trade and"
        " the number above is not a ceiling";
    {
        bool bDeals = b.lifelink && bp > 0 && !bStopped && !bDiesToFirstStrike;
        bool aDeals = a.lifelink && ap > 0 && !aStopped && !aDiesToFirstStrike;
        if (bDeals)
        {
            const char * who = attackerSeat ? "they gain" : "you gain";
            //#W47-R3: the converter multiplies THIS number, so it says so here
            //rather than a paragraph away. Voiced from the reading seat: in the
            //attackers window the gainer is "they" and the loser is the reader.
            const char * conv = attackerSeat ? ", and their converter takes that"
                                               " much off you"
                                             : ", and your converter takes that"
                                               " much off them";
            //#W57-B (D10): D13's residual. The blocking-trigger gain was
            //un-nested and the LIFELINK gain - the same species, an own gain
            //printed INSIDE the survival verdict - was left where it was.
            //`126v123` seq 37: "B7. Vampire (1/1) [lifelink] - may block A1
            //(your blocker dies, attacker lives (lifelink: you gain 1))". Same
            //seam, same out-param shape: with the out-param NULL every existing
            //caller renders byte-identically.
            std::ostringstream body; //the clause WITHOUT its label
            if (b.doublestrike)
                body << who << " life in BOTH damage steps - total not computed here"
                     << (bGainConverted ? conv : "");
            else
            {
                body << who << " " << bp;
                if (bGainConverted)
                    body << (attackerSeat ? ", and their converter takes "
                                          : ", and your converter takes ")
                         << bp << (attackerSeat ? " off you" : " off them");
            }
            //#W63-AB (E1): on the attackers window `b` is the BLOCKING side -
            //theirs - so this own-gain clause is a THEIR-gain to the reader.
            if (foeLifeLoop && attackerSeat)
                body << kFoeLoopGainTail;
            const char * label = b.doublestrike ? "lifelink + double strike" : "lifelink";
            if (outBlockerLifelink)
                *outBlockerLifelink = string(label) + ", this block: " + body.str();
            else
                o << " (" << label << ": " << body.str() << ")";
        }
        if (aDeals)
        {
            //#W46-9 (wave-45 ledger, MED). A benefit inside a fatal parenthesis
            //reads as an ADDITION. deck152 vs162 seq 28 was handed
            //"(your attacker dies, their blocker lives (lifelink: you gain 3))"
            //and answered "gain 3 life each via lifelink (total +6 life) AND
            //deal 6 damage to the opponent" - both halves of a fight that has
            //exactly one branch. The bodies clause is honest and the number is
            //honest; nothing bound the number to the branch it belongs to. So
            //bind it: this gain exists only if this attacker is BLOCKED here,
            //and a blocked attacker's damage goes to the blocker, not to their
            //life. The no-face half is claimed only where it is exactly true -
            //a trampler that carries damage through gets the honest alternative
            //(the gain already counts that carry-over) instead of a false
            //restriction, and a PARTIALLY prevented carry-over is not computed
            //in either direction, so neither clause is claimed for it.
            const char * who = attackerSeat ? "you gain" : "they gain";
            const bool noFace = (trampleThrough <= 0 || preventAtoFace == kPreventFull);
            const bool someFace = (trampleThrough > 0 && preventAtoFace == kPreventNone);
            const char * whoseLife = attackerSeat ? " their life" : " your life";
            //#W57-B (D10): nine of the ten residual nested B-line gains are
            //this one - the ATTACKER's lifelink, a THEIR gain reported inside
            //the seat's own survival verdict (`126v152` s7/s11, `123v126`
            //s57/s65/s92, `162v152` s8, `146v126` s20). Same out-param shape as
            //the own-gain half above; NULL renders byte-identically.
            std::ostringstream body;
            if (a.doublestrike)
                body << who << " life in BOTH damage steps of this block - total not"
                        " computed here";
            else
            {
                //#W82-EC (H4, deck162 HIGH-1): "from this block only" was read as
                //"only if you block" - `162v152` s11 declined the block to avoid a
                //gain that lands either way and took 11 instead of 5. The gain is
                //a fact of the attacker's damage being dealt, not of the block;
                //the A-line now says so (attackerLifelinkAttackLineTag) and this
                //brace states the figure bare.
                body << who << " " << ap;
            }
            if (noFace)
                body << ", and this attacker deals nothing to" << whoseLife;
            else if (someFace)
                body << " - that number already counts the damage it tramples"
                        " through";
            //#W63-AB (E1): on the blockers window `a` is the ATTACKER - theirs -
            //so this gain is the one the B row prices against the reader.
            if (foeLifeLoop && !attackerSeat)
                body << kFoeLoopGainTail;
            const char * label = a.doublestrike ? "lifelink + double strike" : "lifelink";
            if (outAttackerLifelink)
                *outAttackerLifelink = string("their attacker's ") + label
                                       + ", this block: " + body.str();
            else
                o << " (" << label << ": " << body.str() << ")";
        }
    }
    //#W45-3: the block itself moves life. Unlike lifelink, trample and the
    //wither shrink, this rides the DECLARATION of the block and not the damage,
    //so it is deliberately NOT gated on prevention or on a blocker that dies in
    //the first-strike step - the trigger has already fired by then, and gating
    //it would delete a true fact. Voiced from the same seat as the verdict it
    //follows: 'b' is the blocking side, so the attackers window says "they".
    if (b.blockLife > 0 || b.blockLifeMay > 0)
    {
        const char * subj = attackerSeat ? "they" : "you";
        //#W56-B (D13): the clause is built into its own stream so it can be
        //handed out as a SIBLING annotation instead of nested inside the
        //survival verdict - `126v146` seq 6 read "(your blocker dies, attacker
        //lives (blocking trigger: you gain 3 and may gain 2 more))" and the
        //losing block read as a priced one. Nothing about the clause changes.
        std::ostringstream bt;
        {
        std::ostringstream & o = bt;
        //#W69-BI (K7): the ", this combat" marker belongs to the BLOCKERS
        //window's brace form (#W56-B D13). On the ATTACKERS line the clause is
        //handed out only to move it OUT of the verdict's parenthesis, and four
        //live guides quote its text verbatim - "(blocking trigger: they gain
        //N)" / "(blocking trigger: they may gain 2)" (deck130 l.177, deck146
        //l.128 and l.248, deck152 l.72). The words stay exactly as they print
        //today; only the parenthesis they sit in moves.
        o << "blocking trigger" << ((outBlockTrigger && !attackerSeat) ? ", this combat" : "")
          << ": ";
        //#W47-R3: when a converter of that side's is in play the gain and the
        //drain are ONE fact, so they are printed as one clause. The certain and
        //the "may" halves are kept apart exactly as above - a single number is
        //restated, a split total is bound with "that much" rather than inventing
        //a sum the script does not promise.
        const bool split = (b.blockLife > 0 && b.blockLifeMay > 0);
        const int lone = (b.blockLife > 0) ? b.blockLife : b.blockLifeMay;
        if (b.blockLife > 0)
        {
            o << subj << " gain " << b.blockLife;
            if (b.blockLifeMay > 0)
                o << " and may gain " << b.blockLifeMay << " more";
        }
        else
            o << subj << " may gain " << b.blockLifeMay;
        if (bGainConverted)
        {
            //A gain the script only MAY take is a drain that only may happen -
            //the condition travels with the number, never dropped to make the
            //clause read stronger.
            if (b.blockLife <= 0)
                o << (attackerSeat ? ", and if they do their converter takes "
                                   : ", and if you do your converter takes ");
            else
                o << (attackerSeat ? ", and their converter takes "
                                   : ", and your converter takes ");
            if (split)
                o << "that much";
            else
                o << lone;
            o << (attackerSeat ? " off you" : " off them");
        }
        //#W63-AB (E1): the blocking-trigger gain, when the blocking side is
        //THEIRS (the attackers window), is the same first link.
        if (foeLifeLoop && attackerSeat)
            o << kFoeLoopGainTail;
        } //#W56-B (D13): end of the clause's own scope
        //#W56-B (D13): out to the caller, or nested exactly as before.
        if (outBlockTrigger)
            *outBlockTrigger = bt.str();
        else
            o << " (" << bt.str() << ")";
    }
    //W41-5, the honest-weaker-claim path. A prevention effect applies but its
    //residue is not exactly computable here, so the verdict above is the NAIVE
    //one and says so - the omit-when-unprovable precedent, stated rather than
    //silent, and worded so no affirmative substring can be latched as a
    //prediction.
    if (preventAtoB == kPreventPartial && preventBtoA == kPreventPartial)
        o << " - damage prevention applies to BOTH creatures and is NOT included"
             " here: read their text";
    else if (preventAtoB == kPreventPartial)
        o << (attackerSeat ? " - damage prevention protecting their blocker is NOT"
                             " included here: read its text"
                           : " - damage prevention protecting your blocker is NOT"
                             " included here: read its text");
    else if (preventBtoA == kPreventPartial)
        o << (attackerSeat ? " - damage prevention protecting your attacker is NOT"
                             " included here: read its text"
                           : " - damage prevention protecting the attacker is NOT"
                             " included here: read its text");
    return o.str();
}

CombatTradeStat combatStatOf(MTGCardInstance * c)
{
    CombatTradeStat s;
    s.power = c->power;
    s.toughness = c->toughness;
    s.deathtouch = c->basicAbilities[Constants::DEATHTOUCH];
    s.wither = c->basicAbilities[Constants::WITHER] || c->basicAbilities[Constants::INFECT];
    s.infectLabel = c->basicAbilities[Constants::INFECT] && !c->basicAbilities[Constants::WITHER];
    s.firststrike = c->basicAbilities[Constants::FIRSTSTRIKE] || c->basicAbilities[Constants::DOUBLESTRIKE];
    s.indestructible = c->basicAbilities[Constants::INDESTRUCTIBLE];
    s.trample = c->basicAbilities[Constants::TRAMPLE];
    //Persist returns the creature once, with a -1/-1 counter - but only if it
    //does not already carry one (the counter is what ends the loop). Undying is
    //the +1/+1 mirror; scoped to persist here (deck59 wave-24).
    s.persist = c->basicAbilities[Constants::PERSIST] && c->counters && !c->counters->hasCounter(-1, -1);
    s.lifelink = c->basicAbilities[Constants::LIFELINK]; //#W44-LOW (E-3)
    s.doublestrike = c->basicAbilities[Constants::DOUBLESTRIKE];
    //#W45-3: meaningful only when this creature is the BLOCKER of the pairing,
    //which is the slot every caller puts it in; the attacker side computes it
    //and the verdict never reads it.
    blockTriggeredLifeFor(c, s.blockLife, s.blockLifeMay);
    //#W82-EC (H3): damage already marked this turn lowers what it takes to kill
    //this body. Damageable::life is toughness minus marked damage (dealDamage
    //subtracts in place); a body at 0 or less is already dying and is read as
    //undamaged for the pairing, exactly as the crack-back walk reads it.
    s.remaining = (c->life > 0 && c->life < c->toughness) ? c->life : 0;
    return s;
}


//#W82-EC (H3, deck130 HIGH-1). `130v123` s72: the battlefield printed `Rorix
//Bladewing (6/5) [untapped]`, the A-line's 1-on-1 tag said "(you kill it, your
//attacker lives)" against a 4/4, and the only surface that knew about the 4
//damage Starstorm had marked on Rorix that turn was a GAME LOG line 30 lines
//up. The seat attacked and lost its win condition. The fact goes on every
//surface that prints the body: the battlefield line and the combat rows. A
//BRACKET tag, so the reply scanner drops it whole and every row key
//(optionSetKeyOf, holdActionKeyRow, w77KeyTailOf) strips it: the number is
//outside every key by construction (PARSETEST pins it). Silent on an undamaged
//body and on one already at 0 or less (the "died" line is the fact there).
//#W82-EC (H4, deck162 HIGH-1). `162v152` s11: `A2. Intrepid Adversary (6/4)
//deals 6 [lifelink]` - a bare keyword, no owner, no figure - and the only
//number sat in the B-row brace as "they gain 6 from this block only". Read
//together, the menu said declining the block avoids the gain; the seat declined,
//took 11 instead of 5 at 15 life, and priced the swing as net 0. The gain rides
//DEALT damage: unblocked it lands on the player, blocked it lands on the blocker,
//and either way the attacker's controller gains it. Only prevention stops it.
//Bracketed like every A-line tag (echo-safe, outside every row key). `dealt` is
//the per-step figure; a double striker gains it in each damage step it deals in.
string attackerLifelinkAttackLineTag(int dealt, bool doublestrike)
{
    if (dealt <= 0)
        return " [lifelink: it deals 0, so THEY gain nothing from it]";
    std::ostringstream o;
    o << " [lifelink: THEY gain " << dealt;
    if (doublestrike)
        o << " in each of its two combat damage steps";
    o << " if it connects, blocked or not - a block only changes who takes the "
      << dealt << "; only prevented damage stops the gain]";
    return o.str();
}


string markedDamageTag(int toughness, int life)
{
    if (life <= 0 || life >= toughness)
        return "";
    std::ostringstream o;
    o << " [" << (toughness - life) << " damage marked this turn - " << life
      << " more damage kills it]";
    return o.str();
}


//W41-5: what would happen to COMBAT damage dealt by 'src' to 'tgt', asked of
//the engine's own machinery rather than re-derived from card text. Two
//sources, in the order Damage::resolve consults them:
//  (1) the registered replacement effects (every preventAllCombatDamage /
//      preventAllDamage / fog form compiles to a REDamagePrevention, so ONE
//      probe covers all 42 primitive occurrences of the keyword and every
//      to()/from() shape they use, including the ones that name a filtered
//      set rather than `this`);
//  (2) the per-card checks Damage::resolve applies after replacement -
//      protection (damage = 0, a full stop), and then the family that zeroes
//      damage while doing something ELSE with it (phantom eats a +1/+1
//      counter, wilting/vigor convert it to counters, absorb reduces it by N,
//      a finite `preventable` shield spends itself). That second family is
//      reported PARTIAL, never FULL: each one changes the body in a way this
//      1-on-1 preview does not model, and claiming "nothing happens" would be
//      its own lie.
static int combatPreventionKind(MTGCardInstance * src, MTGCardInstance * tgt)
{
    if (!src || !tgt)
        return kPreventNone;
    int k = kPreventNone;
    GameObserver * g = tgt->getObserver();
    if (g && g->replacementEffects)
        k = g->replacementEffects->preventionKindFor(src, tgt, (int) Damage::DAMAGE_COMBAT);
    if (k == kPreventFull)
        return kPreventFull;
    if (tgt->protectedAgainst(src))
        return kPreventFull;
    if (tgt->preventable > 0
        || tgt->has(Constants::PHANTOM)
        || tgt->has(Constants::ABSORB)
        || tgt->has(Constants::WILTING)
        || tgt->has(Constants::VIGOR))
        return kPreventPartial;
    return k;
}


//The same question for the attacker's damage to YOU (the defending player) -
//what the trample-through claim rides. Only the replacement layer and the
//player's own finite shield apply here; a player has no protection or counter
//conversions.
static int combatPreventionKindToPlayer(MTGCardInstance * src, Player * p)
{
    if (!src || !p)
        return kPreventNone;
    int k = kPreventNone;
    GameObserver * g = src->getObserver();
    if (g && g->replacementEffects)
        k = g->replacementEffects->preventionKindFor(src, p, (int) Damage::DAMAGE_COMBAT);
    if (k == kPreventNone && p->preventable > 0)
        return kPreventPartial;
    return k;
}


//#W56-B (D13): the standing cost of a block that loses the blocker - the third
//number `126v146` seq 6 needed and no annotation carried. The seat blocked with
//both walls at 20 life against a lone 1/1, banked 7 life, and at seq 16 met five
//attackers with one blocker and lost at -14. Pure over the available-blocker
//count; claims nothing about next turn's board beyond the body this block loses.
static string afterCombatBlockerCostText(int availableBlockers)
{
    std::ostringstream o;
    o << "after this combat: you control 1 fewer blocker";
    if (availableBlockers > 0)
        o << " - " << availableBlockers << " available now, "
          << (availableBlockers - 1) << " if this one dies here";
    return o.str();
}


//#W57-B (D22, wave-56 ledger MED): the per-row `{blocking trigger, this
//combat: ...}` clause (45 rows at deck126) renders BESIDE the 1-on-1 verdict
//`(your blocker dies, attacker lives)`, whose plain meaning is "don't".
//`126v146` seq 37: the seat read the death half, answered `no blockers`,
//forfeited 2 damage stopped + 2 life + 2 off their total, and lost that game at
//-10 against 10. The per-row clause stays; the TOTAL is the number the decision
//turns on, and this render lifts totals onto headers everywhere else
//(`INCOMING THIS COMBAT` is the same device). Pure over the counts.
//`uniform` = every triggered blocker carries the same (sure, may) pair, which
//is what licenses the distributive "each of your N". `totalGain` is the whole
//window's ceiling, so it is always voiced as "up to": how many of them block
//is the reader's own choice, and a "may" gain is theirs too.
static string blockingTriggerTotalLine(int triggered, int sure, int may, bool uniform,
                                       int totalGain, bool converter, int oppLife)
{
    if (triggered <= 0 || totalGain <= 0)
        return "";
    std::ostringstream o;
    o << "BLOCKING THIS COMBAT: ";
    if (uniform)
    {
        o << "each of your " << triggered << " blocker" << (triggered == 1 ? "" : "s")
          << " that blocks gains you " << sure;
        if (may > 0)
            o << " and may gain " << may << " more";
    }
    else
        o << "your " << triggered << " blockers with a blocking trigger gain you up to "
          << totalGain << " in total";
    if (converter)
        o << ", and your converter takes that much off them - up to " << totalGain
          << " off their " << oppLife;
    else
        o << " - up to " << totalGain << " life for you";
    return o.str();
}


static string combatBlockOutcome(CombatWindowCache& cw, MTGCardInstance * blocker, MTGCardInstance * attacker,
                                 string * outBlockTrigger = NULL, bool * outBlockerDies = NULL,
                                 string * outBlockerLifelink = NULL,
                                 string * outAttackerLifelink = NULL, //#W57-B (D10)
                                 bool foeLifeLoop = false) //#W63-AB (E1)
{
    //#W54-M (A22): stats and the converter scan come from the window memo
    return combatTradePreviewStats(cw.statOf(blocker), cw.statOf(attacker),
                                   combatPreventionKind(attacker, blocker),
                                   combatPreventionKind(blocker, attacker),
                                   combatPreventionKindToPlayer(attacker, blocker->controller()),
                                   false, blocker->life,
                                   cw.converterOf(blocker->controller()),
                                   outBlockTrigger, outBlockerDies, //#W56-B (D13)
                                   outBlockerLifelink, outAttackerLifelink, //#W57-B (D10)
                                   NULL, foeLifeLoop); //#W63-AB (E1)
}


//#W63-AB (E3). The material rank of one pairing, taken from the SAME forecast
//the B rows print: 4 = you kill it and your blocker lives, 3 = neither dies
//(the body is kept), 2 = a trade, 1 = a chump block. Nothing else in the engine
//re-derives lethality, so the header's suggestion and the row it points at
//cannot disagree. 0 only for an unreadable pairing.
int blockPairMaterialRank(MTGCardInstance * blocker, MTGCardInstance * attacker)
{
    if (!blocker || !attacker)
        return 0;
    bool bDies = false, aDies = false;
    combatTradePreviewStats(combatStatOf(blocker), combatStatOf(attacker),
                            combatPreventionKind(attacker, blocker),
                            combatPreventionKind(blocker, attacker),
                            combatPreventionKindToPlayer(attacker, blocker->controller()),
                            false, blocker->life, false,
                            NULL, &bDies, NULL, NULL, &aDies);
    if (aDies && !bDies)
        return 4;
    if (!aDies && !bDies)
        return 3;
    if (aDies && bDies)
        return 2;
    return 1;
}


//W42-3: the SAME fight, asked from the attacking seat. Identical arguments in
//the identical order - only the voicing flips - so the attackers window can
//never disagree with the blockers window about who dies. 'attacker' is the
//reader's creature; 'blocker' is one of the defender's untapped bodies.
//#W54-M (A22): 'blockerOnAttacker' is combatPreventionKind(blocker, attacker),
//asked once by the caller and shared with its gang/price tests (it was probed
//three times per pairing).
//#W69-BI (K7, deck146 MED): `outBlockTrigger` is threaded so the ATTACKERS
//window can print the gain as a SIBLING of the survival verdict, exactly as the
//blockers window has since #W56-B (D13). Left NULL the trigger nests inside the
//verdict and the row reads `(neither dies (blocking trigger: they may gain 2))`
//- 14 of the corpus's 18 `(neither dies` renders - so the closed literal three
//guides quote never matches. Default NULL keeps every other caller identical.
string combatAttackOutcome(CombatWindowCache& cw, MTGCardInstance * attacker, MTGCardInstance * blocker,
                                  int blockerOnAttacker,
                                  bool foeLifeLoop, //#W63-AB (E1)
                                  string * outBlockTrigger) //#W69-BI (K7)
{
    return combatTradePreviewStats(cw.statOf(blocker), cw.statOf(attacker),
                                   combatPreventionKind(attacker, blocker),
                                   blockerOnAttacker,
                                   combatPreventionKindToPlayer(attacker, blocker->controller()),
                                   true, blocker->life,
                                   cw.converterOf(blocker->controller()),
                                   outBlockTrigger, NULL, NULL, NULL, NULL, foeLifeLoop); //#W63-AB (E1)
}


//W36 #2 (139-tier P1 / 158 P3, engine-verified game-affecting): a "whenever ~
//becomes blocked" trigger changes the fight BEFORE damage, so a trade computed
//from printed stats lies about the block - Ichorclaw Myr (1/1, +2/+2 when
//blocked) rendered "you kill it, your blocker lives" for a 2/2 blocker the
//pumped 3/3 actually kills, and the trust doctrine makes that printed lie an
//instruction. Parse the attacker's oracle text for the simple SELF-pump form;
//the caller folds it into the computed trade and says so. Anything else a
//becomes-blocked trigger does (blocker debuffs, untaps, damage riders) is
//uncomputable in a 1-on-1 preview and gets an explicit not-included flag
//instead - silent omission is the failure mode this fixes. Pure, on the text
//line, so PARSETEST proves the classification without an engine card.
//Returns 0 = no becomes-blocked trigger; 1 = simple self-pump (dp/dt set);
//2 = a trigger this preview cannot compute.
static int becomesBlockedSelfPump(const string& text, int& dp, int& dt)
{
    dp = dt = 0;
    string low = text;
    for (size_t i = 0; i < low.size(); i++)
        low[i] = (char) tolower((unsigned char) low[i]);
    size_t pos = low.find("becomes blocked");
    if (pos == string::npos)
        return 0;
    size_t end = low.find('.', pos); //classify from the trigger's own sentence
    string sent = low.substr(pos, end == string::npos ? string::npos : end - pos);
    size_t g = sent.find("gets +");
    if (g == string::npos)
        return 2;
    //"each creature blocking it gets +N/+N" pumps the BLOCKER side - not a
    //self-pump; anything naming a creature/blocking before the verb bails.
    string pre = sent.substr(0, g);
    if (pre.find("creature") != string::npos || pre.find("blocking") != string::npos)
        return 2;
    size_t p = g + 6;
    int n = 0; bool anyN = false;
    { const size_t p0 = p; n = readDigits(sent, p); anyN = p > p0; } //#W54-M (L7)
    if (!anyN || p + 1 >= sent.size() || sent[p] != '/' || sent[p + 1] != '+')
        return 2;
    p += 2;
    int m = 0; bool anyM = false;
    { const size_t p0 = p; m = readDigits(sent, p); anyM = p > p0; } //#W54-M (L7)
    if (!anyM)
        return 2;
    dp = n; dt = m;
    return 1;
}

//#W64-AI (F4, deck152 HIGH-2): the ATTACK line's optional TARGET suffix.
//`A1>W2` (also `A1 -> W2`, `a1>w2`) sends attacker A1 at the opponent's second
//listed planeswalker/battle instead of at the player; an attacker written
//without a suffix attacks the player, which is every reply written before this
//wave. Deliberately a SEPARATE pass over the same line parseAttackerSet reads:
//the A-index grammar already skips a digit glued to a letter ("W2"), so a
//suffixed row still declares its attacker even if this pass drops the suffix,
//and the declaration can never be lost to a malformed target.
//out[] is 1-based walker numbers, 0 = the player. FIRST-WINS per attacker (the
//same rule the A-set uses); an out-of-range A# or W# is dropped, that pair
//only. Returns how many targets were bound. Pure.
static int parseAttackerTargets(const string& line, size_t nAttackers, size_t nTargets,
                                vector<int>& out)
{
    out.assign(nAttackers, 0);
    int bound = 0;
    if (!nTargets)
        return 0;
    for (size_t i = 0; i + 1 < line.size(); i++)
    {
        if (line[i] != 'A' && line[i] != 'a')
            continue;
        char prev = (i > 0) ? line[i - 1] : ' ';
        if (isalnum((unsigned char) prev))
            continue; //inside a word, not a row label
        size_t j = i + 1;
        if (!isdigit((unsigned char) line[j]))
            continue;
        int a = 0;
        while (j < line.size() && isdigit((unsigned char) line[j]))
            a = a * 10 + (line[j++] - '0');
        while (j < line.size() && (line[j] == ' ' || line[j] == '-'))
            j++;
        if (j >= line.size() || line[j] != '>')
            continue;
        j++;
        while (j < line.size() && line[j] == ' ')
            j++;
        if (j >= line.size() || (line[j] != 'W' && line[j] != 'w'))
            continue;
        j++;
        if (j >= line.size() || !isdigit((unsigned char) line[j]))
            continue;
        int w = 0;
        while (j < line.size() && isdigit((unsigned char) line[j]))
            w = w * 10 + (line[j++] - '0');
        if (a < 1 || a > (int) nAttackers || w < 1 || w > (int) nTargets)
            continue;
        if (out[a - 1])
            continue; //first wins
        out[a - 1] = w;
        bound++;
    }
    return bound;
}


static int parseBlockAssignments(const string& content, size_t nBlockers, size_t nAttackers, vector<int>& out,
                                 const vector<string> * blockerNames = NULL,
                                 const vector<string> * attackerNames = NULL,
                                 const vector<vector<int> > * legalPerBlocker = NULL,
                                 int * dropped = NULL,
                                 bool * gangConflict = NULL,
                                 bool * blockerInAttackerSlot = NULL);


//Every line of a reply whose FIRST token (after markdown decoration) is the
//given answer label, as the text AFTER the label to end of line, in reply
//order. The salvage path re-parses these through the full validation: a
//decode-time repeat-loop often states a well-formed BLOCKS:/ATTACK:/PUT: line
//before spiraling (the same premise as salvageLoopedChoice), so the last one
//that validates is recovered rather than lost to the heuristic.
//#W65-AP (R2): `prevOut`, when given, receives the nearest preceding non-blank
//line for each collected line - the correction-cue window the CHOICE seam
//already reads (#W65-AO G8). Optional, so every existing caller is unchanged.
//#W66-AR (H2b): `windowOut`, when given, receives for each collected line the
//up-to-THREE non-blank lines written since the PREVIOUS collected line - the
//same announcement window findAnswerLabelLine reads, so the correction rule is
//one rule at all three seams and not two that happen to agree.
static void collectLabeledLines(const string& content, const char * label, vector<string>& out,
                                vector<string> * prevOut = NULL,
                                vector<vector<string> > * windowOut = NULL)
{
    size_t labelLen = strlen(label);
    size_t lineStart = 0;
    string prevNonBlank;
    vector<string> recent; //#W66-AR (H2b)
    while (lineStart <= content.size())
    {
        size_t lineEnd = content.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? content.size() : lineEnd;
        size_t s = lineStart;
        while (s < end && (content[s] == ' ' || content[s] == '\t'
                           || content[s] == '*' || content[s] == '#' || content[s] == '-'))
            s++;
        const string rawLine = content.substr(lineStart, end - lineStart);
        if (end - s >= labelLen)
        {
            bool m = true;
            for (size_t k = 0; k < labelLen && m; k++)
                m = (toupper((unsigned char) content[s + k]) == toupper((unsigned char) label[k]));
            if (m)
            {
                out.push_back(content.substr(s + labelLen, end - (s + labelLen)));
                if (prevOut)
                    prevOut->push_back(prevNonBlank);
                if (windowOut)
                    windowOut->push_back(recent);
                recent.clear(); //#W66-AR (H2b): a coded line closes the window
                if (lineEnd == string::npos)
                    break;
                lineStart = lineEnd + 1;
                if (rawLine.find_first_not_of(" \t\r") != string::npos)
                    prevNonBlank = rawLine;
                continue;
            }
        }
        if (rawLine.find_first_not_of(" \t\r") != string::npos)
        {
            prevNonBlank = rawLine;
            recent.push_back(rawLine); //#W66-AR (H2b)
            if (recent.size() > 3)
                recent.erase(recent.begin());
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
}


//Salvage a bundled BLOCK reply from a decode spiral: the LAST BLOCKS: line that
//re-parses to >=1 well-formed pairing through the full validation path (range +
//name reconcile + legal-per-blocker). Fills 'out' and returns the pair count;
//0 = nothing salvageable (caller keeps its heuristic fallback). Never bypasses
//validation - a hallucinated/out-of-range line yields 0 pairs and is skipped.
static int salvageLoopedBlocks(const string& content, size_t nBlockers, size_t nAttackers,
                               const vector<string>& blockerNames, const vector<string>& attackerNames,
                               const vector<vector<int> >& legalIdx, vector<int>& out)
{
    vector<string> lines;
    collectLabeledLines(content, "BLOCKS:", lines);
    for (size_t idx = lines.size(); idx-- > 0; )
    {
        vector<int> pick;
        int pairs = parseBlockAssignments(lines[idx], nBlockers, nAttackers, pick,
                                          &blockerNames, &attackerNames, &legalIdx);
        if (pairs > 0) { out = pick; return pairs; }
    }
    return 0;
}


//Salvage a bundled SUBSET reply (ATTACK: attackers, or PUT: reveal picks) from a
//decode spiral: the LAST labeled line that names >=1 eligible item through
//parseAttackerSet. Fills 'out'; returns that count, or -1 if none salvageable.
//A bare "none" is NOT salvaged (declining is the safe default already, and a
//spiral that meant to decline would not have spiraled).
static int salvageLoopedSubset(const string& content, const char * label, size_t n,
                               const vector<string>& names, vector<bool>& out)
{
    vector<string> lines;
    collectLabeledLines(content, label, lines);
    for (size_t idx = lines.size(); idx-- > 0; )
    {
        vector<bool> send;
        int r = parseAttackerSet(lines[idx], n, send, &names);
        if (r >= 1) { out = send; return r; }
    }
    return -1;
}


//--- Prose-intent salvage (ITEM C, last resort before the heuristic) --------
//When a combat reply carried NO well-formed coded line (a >12k-char decode
//spiral truncated before BLOCKS:/ATTACK: - salvage has nothing to grab), scan
//the prose for an EXPLICIT declaration in a NARROW pattern and take it only
//when it maps unambiguously onto the legal set. Deliberately conservative: a
//wrong salvage is worse than a heuristic fallback. Scoped to combat, where
//the answer is an imperative declaration ("block A3", "attack with A1, A2");
//NOT extended to the cast menu, where reasoning prose about NOT casting a card
//would read as casting it (the wave-10 "cast the condemned spell" trap).


//#W70-BN (F1/F2, Astra review findings 1 and 2): THE COMBAT PROSE READERS ARE
//GONE. `proseAttackerOrdinal`, `salvageProseBlocks` and `salvageProseAttackers`
//manufactured a declaration out of an unlabelled sentence ("I will attack with
//A1 and A3") over the WHOLE reply, ahead of truncation recovery. Under invariant
//000 a reply is the PLAN line and the action line; every other byte is
//`off_protocol_bytes` and is NEVER an input to a decision. A combat window with
//no labelled declaration earns the seam's ONE re-ask and then the heuristic -
//never a declaration the model never wrote in the shape it was asked for.
//===================== WAVE-25 ITEM 2 =====================================
//Natural-stop vs truncation classifier + the unified answer-line precedence it
//drives across ALL coded-answer paths.
//
//PRECEDENCE RULES (CHOICE / ATTACK / BLOCKS / PUT), stated in one place:
//  * The reply protocol puts the ANSWER first, then optional scratch reasoning,
//    then PLAN: last. So the FIRST coded line is the answer-first commitment and
//    the LAST well-formed coded line is the model's final answer after reasoning.
//  * TRUNCATED reply (hit the token cap: no PLAN: line and a long/mid-word cut):
//    prefer the FIRST well-formed coded line. The tail is unreliable - a cut
//    reply's later lines are decode spiral or half-thoughts (deck133 s21 shape),
//    so answer-first wins ([C] truncated answer-first still takes line 1).
//  * NATURALLY TERMINATED reply (reached its PLAN: line, or a short compliant
//    answer ending on terminal punctuation): prefer the model's FINAL answer -
//    the LAST well-formed coded line, OR, when the correction lived ONLY in prose
//    (deck27 vs137 bottom: line 1 "PUT: 3,5,6" bottomed its only blue source, but
//    the reasoning reached and stated "So I bottom 5, 6, and 7."), the last
//    explicit prose restatement (salvageProsePutList).
//  * A "later"/"last" coded line only counts when it is a WELL-FORMED answer for
//    the CURRENT ask - proper labels, in range - NOT combat-math prose. The ATTACK
//    path keeps ANSWER-FIRST precisely because a CoT line ("Attack: Deal 1, Take 5.
//    Net -4 life.") shares the ATTACK label and its bare prose numbers parse to a
//    bogus subset (deck109 vs62 s21); taking the LAST such line would resurrect the
//    wave-23 bug, so ATTACK trusts the FIRST well-formed declaration and never a
//    trailing bare-number line (this is both the truncated->first rule AND the
//    well-formedness guard, so ATTACK needs no separate natural->last branch).
//  * INDEX-WINS / echo-staleness / retraction still gate every candidate line.
//
//A reply terminated naturally iff it reached the contract's terminal PLAN:
//section, or (legacy short answers) it is short and ends on terminal punctuation.
//A long reply with no PLAN: is a token-cap cut.
//#W70-BM (E5, audit B4.5). A PLAN LINE IS NOT A TERMINATOR ANY MORE. The old
//rule was "a reply terminated naturally iff it reached the contract's terminal
//PLAN: section" - true when the PLAN came LAST, and exactly backwards under
//invariant 000, where the PLAN comes FIRST: a reply the length cap cut after
//`PLAN:` and before `BLOCKS:` read as a NATURAL STOP, and the natural-stop test
//became trivially true for every compliant reply, so the truncated-commitment
//scan it gates could never run again. The engine already has the true signal and
//records it - `finish_reason == "length"` (mLastFinishLength, 15044) - and the
//other half of the question is whether the ANSWER LINE is there at all. The
//legacy short-answer arm (a bare reply ending on punctuation) goes with the
//legacy shape it was written for.
static bool replyTerminatedNaturally(const string& content, bool finishLength)
{
    if (finishLength)
        return false; //the cap cut it, wherever it stopped
    return hasCodedAnswerLine(content); //the action line is present
}


//===================== WAVE-29 ITEM 2 (N-18e) ============================
//A TRUNCATED blockers reply whose early coded BLOCKS: line committed to a
//block, then whose reasoning reversed to a no-block conclusion but ran out of
//tokens before emitting a corrected/terminal answer. The parser's last-coded-
//line precedence would honor the stale early commitment - a fatal reach-trade
//the model had ALREADY talked itself out of (deck18 vs93 s20: emitted
//"BLOCKS: B1:A1", looped "So I should NOT block ... blocking is strictly worse",
//hit the token ceiling mid-sentence with no terminator; the block executed and
//the Soldier died for nothing, contradicting the reply's own conclusion).
//
//Fires ONLY when BOTH hold, so real answers are never masked:
//  (1) the reply is TRUNCATED (replyTerminatedNaturally == false). A naturally
//      terminated reply is trusted as-is: the model's final answer stands and
//      the normal last-line precedence resolves any self-correction.
//  (2) a GLOBAL no-block decline conclusion appears AFTER the committing coded
//      line. A truncated reply whose early commit was NEVER contradicted keeps
//      it (unchanged) - the defect is specifically contradicted-then-truncated.
//The "not block" family is guarded against "cannot block" (a legality note, not
//a decline of intent). ATTACK keeps its own answer-first CoT-hijack guard and
//is untouched; this is scoped to the blockers seam where the defect was seen.
//WAVE-32 (N-122d): condition (2)'s GLOBALITY is now actually enforced - see the
//two helpers below and their call sites. The predicate had been firing on
//per-creature bookkeeping and throwing compliant answers away.
//
//#W70-BN (F1, Astra review finding 1): `truncatedBlockCommitmentAbandoned` and
//its two helpers are DELETED. The predicate scanned the PROSE after a legal
//`BLOCKS:` line for a decline phrase and replaced the declaration with NO
//BLOCKERS - so `PLAN: block their attacker / BLOCKS: B1:A1 / I should not block`
//let lethal damage through on the strength of a sentence the protocol forbids
//the model to write. A reply's decision is its action line. A truncated reply
//with no usable declaration is already answered by the seam's truncation re-ask
//(#W68-BE R4) and then the heuristic; nothing needs to read the tail.
//#W71-BO (R3, wave-70 census): `salvageProsePutList` DELETED. It read a PUT
//answer out of the reply's PROSE ("So I bottom 5, 6, and 7.") - the one thing
//invariant 000 forbids the reply to contain - and had already lost its only
//production caller. A reply's decision is its action line.

//W41-13: an echoed A-line drags its ANNOTATIONS with it, and an annotation is
//prose - it carries digits ("+2 more", "#1") and, now that the hold-back tag
//names the opposing creatures a held body could not block, NAMES that can
//collide with the model's own attack candidates in a mirror. Neither may vote
//on the declaration. Bracketed spans are where every annotation on these lines
//lives, so a span is dropped when its interior reads as PROSE - it contains an
//alphabetic word of 4+ characters. A label echo ("[A1, A3]", "[A2]") has no
//such word and survives untouched, which is the case that must not break: the
//model does write its answer in brackets sometimes.
static string stripAnnotationBrackets(const string& s)
{
    string out;
    size_t i = 0;
    while (i < s.size())
    {
        if (s[i] != '[')
        {
            out += s[i++];
            continue;
        }
        size_t close = s.find(']', i);
        if (close == string::npos)
        {
            out += s[i++]; //unclosed - not an annotation, leave it alone
            continue;
        }
        bool prose = false;
        size_t runLen = 0;
        for (size_t k = i + 1; k < close && !prose; k++)
        {
            if (isalpha((unsigned char) s[k]))
            {
                if (++runLen >= 4)
                    prose = true;
            }
            else
                runLen = 0;
        }
        if (prose)
            out += ' '; //the whole annotation goes, replaced by a separator
        else
            out += s.substr(i, close - i + 1);
        i = close + 1;
    }
    return out;
}


static bool combatLineIsClean(const string& line, const vector<string> * rosterA,
                              const vector<string> * rosterB); //#W62-Z (D9): defined below




//#W70-BM (E2): `restatedCombatDirective` DELETED. It read a PROSE SENTENCE
//after the last coded line ("So ATTACK: A1, A2.") as the declaration - the
//purest form of the tolerance invariant 000 forbids, and structurally dead
//under PLAN-then-action anyway (its region ran from the coded line to the first
//PLAN:, which a PLAN-first reply leaves empty).

//#W49-S (D2b): the LAST-CODED-LINE rule for combat replies. deck146 vs126
//seq 48 opened "ATTACK: A1, A2", reasoned for 2,500 chars ("Do not sacrifice
//Spider"), and ended "ATTACK: A1" - the engine sent both and Spider died. The
//wave-23 first-usable-line rule exists because a CoT combat-math line can
//share the answer's label ("Attack: Deal 1, Take 5. Net -4 life.", deck109
//vs62 s21) and parse to a bogus subset; so a LATER line replaces the first
//only when it is CLEAN - every word on it is a label index, a range, a count,
//a declension word, or a word of the roster it declares over. Prose fails
//that test; a re-answer passes it.
static bool combatLineIsClean(const string& line, const vector<string> * rosterA,
                              const vector<string> * rosterB)
{
    static const char * kWords[] = {
        "all", "none", "and", "with", "nobody", "hold", "back", "this", "turn", "no",
        "attack", "attacks", "attacking", "block", "blocks", "blocking", "blocked", "by",
        "everyone", "everything", "them", "the", "a", "an", "creature", "creatures", "my",
        "of", "only", "just", "alone", "it", "nothing", "none", "both", "each", "every"
    };
    string scan = stripAnnotationBrackets(line);
    //the roster's own words, lowercased, whole tokens
    vector<string> rosterWords;
    for (int r = 0; r < 2; r++)
    {
        const vector<string> * roster = r ? rosterB : rosterA;
        if (!roster)
            continue;
        for (size_t k = 0; k < roster->size(); k++)
        {
            string t;
            const string& nm = (*roster)[k];
            for (size_t q = 0; q <= nm.size(); q++)
            {
                char c = (q < nm.size()) ? (char) tolower((unsigned char) nm[q]) : ' ';
                if (isalnum((unsigned char) c) || c == '\'' || c == '#')
                    t += c;
                else
                {
                    if (!t.empty())
                        rosterWords.push_back(t);
                    t.clear();
                }
            }
        }
    }
    bool anyToken = false;
    string t;
    for (size_t q = 0; q <= scan.size(); q++)
    {
        char c = (q < scan.size()) ? (char) tolower((unsigned char) scan[q]) : ' ';
        if (isalnum((unsigned char) c) || c == '\'' || c == '#')
        {
            t += c;
            continue;
        }
        if (t.empty())
            continue;
        anyToken = true;
        bool ok = false;
        //A1 / B3 / 7 / #2 - a label index, a bare count, an ordinal
        {
            size_t d = 0;
            if (t[0] == 'a' || t[0] == 'b' || t[0] == '#')
                d = 1;
            bool digits = d < t.size();
            for (size_t z = d; z < t.size() && digits; z++)
                digits = isdigit((unsigned char) t[z]) != 0;
            ok = digits;
        }
        for (size_t w = 0; w < sizeof(kWords) / sizeof(kWords[0]) && !ok; w++)
            ok = (t == kWords[w]);
        for (size_t w = 0; w < rosterWords.size() && !ok; w++)
            ok = (t == rosterWords[w]);
        if (!ok)
            return false;
        t.clear();
    }
    return anyToken;
}


//Scan a bundled-attacker reply for the set of attackers to send: "A<n>"
//tokens (or bare numbers) in [1..nAttackers]. Returns >0 = that many named,
//0 = an explicit decline (a "none/hold/pass" keyword with no numbers) OR a
//declaration that named only INELIGIBLE creatures (the eligible subset is
//empty -> ATTACK: none, not a heuristic fallback; deck62 N2), and
//-1 = unusable (empty or no name/number signal at all -> caller falls back).
//The 0/-1 split matters: attacking with nobody is a legitimate choice, so
//"none" must NOT trigger the heuristic override the way a garbled reply does.
//#W52-G (wave-51 seat E-1): echoBinds - the reply's "N (name)" parentheticals
//are ECHOES of row N (the PUT: card lists: cleanup discard, reveal, London
//bottom), reconciled by the CHOICE rule - a name that uniquely names a
//DIFFERENT row wins over the index, a name naming NO row un-trusts the index
//(the caller's under-pick fallback then answers, announced), a name consistent
//with its index (or ambiguous but including it) keeps it. Off for the ATTACK
//grammar, whose parentheticals are free-form.
//(history: comment-archaeology.md AIPlayerGPTSeams-stripTrailingListGloss-2241)
static string stripTrailingListGloss(const string& line, vector<string> * namesOut)
{
    size_t end = line.find_last_not_of(" \t\r");
    if (end == string::npos || line[end] != ')')
        return line;
    int depth = 0;
    size_t open = string::npos;
    for (size_t i = end + 1; i-- > 0; )
    {
        if (line[i] == ')')
            depth++;
        else if (line[i] == '(')
        {
            depth--;
            if (depth == 0)
            {
                open = i;
                break;
            }
        }
        if (i == 0)
            break;
    }
    if (open == string::npos || open == 0)
        return line;
    const string gloss = line.substr(open + 1, end - open - 1);
    if (gloss.find('(') != string::npos || gloss.find(')') != string::npos)
        return line; //a nested parenthetical is not a flat list of names
    vector<string> parts;
    {
        size_t start = 0;
        for (size_t i = 0; i <= gloss.size(); i++)
        {
            if (i != gloss.size() && gloss[i] != ',')
                continue;
            string t = gloss.substr(start, i - start);
            start = i + 1;
            size_t a = t.find_first_not_of(" \t");
            size_t b = t.find_last_not_of(" \t");
            if (a == string::npos)
                return line; //an empty part: not a name list
            t = t.substr(a, b - a + 1);
            bool alpha = false;
            for (size_t z = 0; z < t.size() && !alpha; z++)
                alpha = isalpha((unsigned char) t[z]) != 0;
            if (!alpha)
                return line; //a number or punctuation, not a card name
            parts.push_back(t);
        }
    }
    if (parts.size() < 2)
        return line; //one name is the per-index echo #W52-G already reconciles
    const string head = line.substr(0, open);
    size_t indices = 0;
    for (size_t i = 0; i < head.size(); i++)
    {
        if (!isdigit((unsigned char) head[i]))
            continue;
        const char prev = i ? head[i - 1] : ' ';
        size_t j = i;
        while (j < head.size() && isdigit((unsigned char) head[j]))
            j++;
        if (!isalnum((unsigned char) prev) && prev != '/' && prev != '#' && prev != '+'
            && (j >= head.size() || head[j] != '/'))
            indices++;
        i = j - 1;
    }
    if (indices != parts.size())
        return line; //not one name per index: say nothing about it
    if (namesOut)
        for (size_t k = 0; k < parts.size(); k++)
            namesOut->push_back(parts[k]);
    string out = head;
    while (!out.empty() && isspace((unsigned char) out[out.size() - 1]))
        out.erase(out.size() - 1);
    return out + line.substr(end + 1);
}


//#W59-J (K9): the same, per LINE - a reply may state its answer on one line and
//ramble on the next, and a gloss only ever glosses its own line.
static string stripTrailingListGlossLines(const string& reply, vector<string> * namesOut)
{
    string out;
    size_t start = 0;
    for (size_t i = 0; i <= reply.size(); i++)
    {
        if (i != reply.size() && reply[i] != '\n')
            continue;
        out += stripTrailingListGloss(reply.substr(start, i - start), namesOut);
        if (i != reply.size())
            out += '\n';
        start = i + 1;
    }
    return out;
}

int parseAttackerSet(const string& content, size_t nAttackers, vector<bool>& out,
                            const vector<string> * optionNames, bool echoBinds,
                            int * repeatedOut)
{
    out.assign(nAttackers, false);
    int named = 0;
    if (repeatedOut)
        *repeatedOut = 0; //#W60-M (B3)
    bool sawNamedContent = false; //the reply named creature(s), eligible or not
    //W41-13: the INDEX and NAME passes read the de-annotated reply; the
    //decline detection below still reads the ORIGINAL, so a bracketed
    //"[none]" is still an explicit decline rather than an unusable reply.
    //#W59-J (K9): the PUT: grammars only (echoBinds) - the ATTACK grammar's
    //parentheticals are free-form prose and are not echoes of anything.
    //#W70-BM (E3, audit B4.6): the trailing GLOSS strip stays (it is the shape
    //of an echoed PUT list, not reasoning) and is now COUNTED, so the probe
    //corpus decides whether the model still writes glosses with thinking on.
    vector<string> listGloss;
    const string scan = stripAnnotationBrackets(echoBinds
                                                ? stripTrailingListGlossLines(content, &listGloss)
                                                : content);
    gLastPutGlossStripped = (echoBinds && !listGloss.empty());
    for (size_t i = 0; i < scan.size(); i++)
    {
        //Accept "A3" or a bare "3"; skip digits that are part of a P/T echo
        //like "2/2" by ignoring a number immediately preceded/followed by '/'.
        //W41-13: '#' and '+' are likewise never A-index prefixes - "#2" is an
        //instance ordinal on a named creature and "+2" is a quantity.
        char prev = (i > 0) ? scan[i - 1] : ' ';
        bool aPrefixed = (scan[i] == 'A' || scan[i] == 'a')
                         && i + 1 < scan.size() && isdigit(scan[i + 1]);
        //A bare number, but not one glued to a letter or the tail of a "2/2".
        bool bareStart = isdigit(scan[i]) && !isalnum(prev) && prev != '/'
                         && prev != '#' && prev != '+';
        if (!aPrefixed && !bareStart)
            continue;
        size_t j = aPrefixed ? i + 1 : i;
        int n = readDigits(scan, j); //#W54-M (L7)
        if (j < scan.size() && scan[j] == '/')
            continue; //a "3/3" power echo, not an attacker index
        //#W48 (D2): the A-rows now carry RANGE labels ("A4-A9" is nine identical
        //creatures printed once), so the grammar has to accept what the render
        //prints. "A4-A9", "A4-9" and "A4 - A9" all declare every attacker in the
        //range; a hyphen followed by anything else is ordinary prose and leaves
        //the single index exactly as it parsed before.
        int hi = n;
        {
            size_t k = j;
            while (k < scan.size() && scan[k] == ' ')
                k++;
            if (k < scan.size() && scan[k] == '-')
            {
                k++;
                while (k < scan.size() && scan[k] == ' ')
                    k++;
                if (k < scan.size() && (scan[k] == 'A' || scan[k] == 'a')
                    && k + 1 < scan.size() && isdigit((unsigned char) scan[k + 1]))
                    k++;
                if (k < scan.size() && isdigit((unsigned char) scan[k]))
                {
                    size_t e = k;
                    int m = readDigits(scan, e); //#W54-M (L7)
                    //not a range if the trailing number is a P/T echo's tail
                    if (m > n && (e >= scan.size() || scan[e] != '/'))
                    {
                        hi = m;
                        j = e;
                    }
                }
            }
        }
        for (int v = n; v <= hi; v++)
            if (v >= 1 && v <= (int) nAttackers)
            {
                if (!out[v - 1])
                {
                    out[v - 1] = true;
                    named++;
                }
                else if (repeatedOut && n == hi)
                    (*repeatedOut)++; //#W60-M (B3): a REPEATED index, not a range overlap
            }
        i = j; //advance past the number
    }
    //#W48 (D2): "ATTACK: all" - the range note offers it, so the grammar owes it.
    //Only when the reply named no index at all AND says nothing that reads as a
    //decline, so "hold all of them back" still declines rather than alpha-strikes.
    if (named == 0)
    {
        string low;
        for (size_t i = 0; i < scan.size() && i < 400; i++)
            low += (char) tolower((unsigned char) scan[i]);
        bool declines = low.find("none") != string::npos || low.find("hold") != string::npos
                        || low.find("no attack") != string::npos || low.find("nobody") != string::npos
                        || low.find("pass") != string::npos || low.find("don't attack") != string::npos
                        || low.find("not attack") != string::npos;
        bool sawAll = false;
        for (size_t i = 0; !declines && i + 3 <= low.size(); i++)
            if (low.compare(i, 3, "all") == 0
                && (i == 0 || !isalpha((unsigned char) low[i - 1]))
                && (i + 3 >= low.size() || !isalpha((unsigned char) low[i + 3])))
                sawAll = true;
        if (sawAll)
        {
            for (size_t v = 0; v < nAttackers; v++)
                if (!out[v])
                {
                    out[v] = true;
                    named++;
                }
        }
    }
    //Name->index reconcile (mirrors parseChoice's echo philosophy). The
    //A-index scan silently DROPPED any name token in a mixed reply
    //("ATTACK: A1, Rakdos Cackler" declared only A1 - deck109 lost 6
    //declarations across 3 games), and a pure-name reply ("ATTACK: Hellrider")
    //parsed to nothing -> heuristic. Split the reply on commas/newlines; each
    //segment's significant words (lowercase, len>=4, minus protocol filler)
    //select the UNIQUE listed attacker whose name contains ALL of them. A
    //unique match UNIONS in; zero or multiple matches drop (respecting
    //eligibility - optionNames holds only the legal candidates). A-index
    //segments carry no significant words and are skipped.
    if (optionNames)
    {
        size_t start = 0;
        for (size_t s = 0; s <= scan.size(); s++)
        {
            //';' is a separator here too (wave-33 N-158n symmetry): the ATTACK
            //name pass already tolerates names, but a ';'-joined list arrived
            //as ONE segment whose merged word set matched nothing, so an
            //otherwise-compliant "ATTACK: Hellrider; Rakdos Cackler" declared
            //NOBODY. The A#-index scan above is separator-independent and
            //unaffected.
            if (s != scan.size() && scan[s] != ',' && scan[s] != ';' && scan[s] != '\n')
                continue;
            string seg = scan.substr(start, s - start);
            start = s + 1;
            vector<string> words;
            significantWords(seg, words);
            if (words.empty())
                continue;
            sawNamedContent = true; //the model listed a named creature here
            //A "#N" ordinal breaks a same-name tie (two identically-named
            //attackers, or two identical revealed cards); without one an
            //ambiguous name still drops, unchanged.
            size_t limit = optionNames->size() < nAttackers ? optionNames->size() : nAttackers;
            int match = uniqueNameMatch(words, *optionNames, limit, NULL, nameOrdinal(seg));
            if (match >= 0 && !out[match])
            {
                out[match] = true;
                named++;
            }
        }
    }
    //#W52-G (E-1): index-vs-echo reconciliation on the PUT: grammars (see the
    //echoBinds note at the signature). deck162 vs deck146 seq 17 executed row 2
    //(Liliana's Caress) off "2 (Cast Liliana's Caress)" - that line was a
    //CHOICE: cast intent the label walk wrongly selected for a PUT: ask (fixed
    //at the seams, which now pass their own label) - and the same seam would
    //have discarded row 1 off "PUT: 1 (Liliana's Caress)" with the index
    //binding blind. Only an index the scan above accepted, immediately followed
    //by a balanced parenthetical, is reconciled.
    if (echoBinds && optionNames && named > 0)
    {
        size_t limit = optionNames->size() < nAttackers ? optionNames->size() : nAttackers;
        for (size_t i = 0; i < scan.size(); i++)
        {
            if (!isdigit((unsigned char) scan[i]))
                continue;
            char prev = (i > 0) ? scan[i - 1] : ' ';
            if (prev == '#' || prev == '+' || prev == '/' || (isalnum((unsigned char) prev) && prev != 'A' && prev != 'a'))
                continue;
            size_t j = i;
            int n = readDigits(scan, j); //#W54-M (L7)
            if (j < scan.size() && scan[j] == '/')
            {
                i = j;
                continue;
            }
            size_t k = j;
            while (k < scan.size() && scan[k] == ' ')
                k++;
            if (k >= scan.size() || scan[k] != '(')
            {
                i = j;
                continue;
            }
            int depth = 0;
            size_t e = k;
            for (; e < scan.size(); e++)
            {
                if (scan[e] == '(')
                    depth++;
                else if (scan[e] == ')' && --depth == 0)
                    break;
            }
            if (e >= scan.size())
                break;
            string echo = scan.substr(k + 1, e - k - 1);
            i = e;
            if (n < 1 || n > (int) nAttackers || !out[n - 1])
                continue;
            vector<string> words;
            significantWords(echo, words);
            if (words.empty())
                continue;
            vector<int> matches;
            for (size_t o = 0; o < limit; o++)
            {
                string low = (*optionNames)[o];
                for (size_t q = 0; q < low.size(); q++)
                    low[q] = (char) tolower((unsigned char) low[q]);
                bool all = true;
                for (size_t w = 0; w < words.size() && all; w++)
                    all = low.find(words[w]) != string::npos;
                if (all)
                    matches.push_back((int) o);
            }
            bool consistent = false;
            for (size_t m = 0; m < matches.size(); m++)
                if (matches[m] == n - 1)
                    consistent = true;
            if (consistent)
                continue;
            if (matches.size() == 1)
            {
                //the name wins over the index (the CHOICE echo_index_conflict rule)
                out[n - 1] = false;
                named--;
                if (!out[matches[0]])
                {
                    out[matches[0]] = true;
                    named++;
                }
            }
            else if (matches.empty())
            {
                //a stale echo: the index is not trusted (the CHOICE stale_echo rule)
                out[n - 1] = false;
                named--;
            }
            //ambiguous and foreign to the index: no better information, the index stands
        }
    }
    //#W59-J (K9): the list gloss, reconciled. It is a per-index echo written
    //once, so it is judged as a SET and not by position: every part must name
    //exactly one row, and only then may it speak. Agreeing with the indices it
    //changes nothing (the common case, and the `125v130` s83 shape); naming a
    //different set it wins, which is the CHOICE grammar's own echo rule; naming
    //anything the rows do not hold it is dropped and the indices stand.
    if (!listGloss.empty() && optionNames && named > 0)
    {
        const size_t limit = optionNames->size() < nAttackers ? optionNames->size() : nAttackers;
        vector<bool> byName(nAttackers, false);
        size_t matched = 0;
        for (size_t g = 0; g < listGloss.size(); g++)
        {
            vector<string> words;
            significantWords(listGloss[g], words);
            if (words.empty())
                break;
            const int m = uniqueNameMatch(words, *optionNames, limit, NULL, nameOrdinal(listGloss[g]));
            if (m < 0 || byName[m])
                break;
            byName[m] = true;
            matched++;
        }
        if (matched == listGloss.size() && byName != out)
        {
            out = byName;
            named = (int) matched;
        }
    }
    if (named > 0)
        return named;
    //#W70-BM (E6, audit B4.2): the explicit DECLINE TOKEN, on the answer line.
    //The old test lowercased the first 200 characters of the WHOLE reply and
    //accepted "none", "hold", "pass", "nobody", "no attack" or "don't attack"
    //anywhere in them - a prose-decline sniff, and under in-band reasoning those
    //200 characters were reasoning. The protocol's decline is `ATTACK: none`, so
    //that is what is read: the token, with no digits on the line.
    {
        string lower;
        for (size_t i = 0; i < content.size(); i++)
            lower += (char) tolower((unsigned char) content[i]);
        if (lower.find("none") != string::npos || lower.find("nobody") != string::npos)
            return 0; //explicit "attack with nobody" - valid, not a fallback
    }
    //The reply NAMED attacker(s) but NONE resolved to an eligible candidate
    //(all summoning-sick / absent from the A-list, or ambiguous duplicates).
    //The faithful reading of "attack with the eligible subset" is an EMPTY
    //set -> ATTACK: none, NOT a heuristic override that may swing at will
    //(deck62 N2: vs14 s21, vs21 s12 both named summoning-sick creatures; the
    //correct answer was ATTACK: none). optionNames scopes this to a real
    //attacker-declaration reply; consumePlan already nulls formless rambles.
    if (sawNamedContent)
        return 0;
    return -1; //unusable
}


//#W65-AP (R2): the ATTACK driver's line choice, as one pure function - so the
//rule it runs is the shared one (gptSelectAnswerIndex) and PARSETEST exercises
//what the DRIVER executes rather than a helper beside it. Every candidate the
//driver ever considered is here: the coded ATTACK: lines of the post-</think>
//reply, in order, plus #W62-Z (D9)'s prose restatement as the last candidate.
//`send` receives the declaration; `takenText` the exact line it was read from
//(#W64-AI F4 parses the >W# suffixes from THAT line and no other); `takenIndex`
//its position, so the caller can still sign what happened. Returns the size of
//the declaration, or -1 when no candidate parses.
//`restated` is #W62-Z (D9)'s prose restatement, computed by the caller because
//AIPlayerGPT::restatedCombatDirective is private to the class (both callers -
//the driver and PARSETEST - are members).
static int gptAttackLineFromReply(const string& content, size_t nAttackers,
                                  const vector<string>& attackerNames,
                                  vector<bool>& send, string * takenText,
                                  int * takenIndex, string * notes,
                                  string * ignoredLine = NULL) //#W66-AR (H2c)
{
    if (takenIndex)
        *takenIndex = -1;
    if (content.empty())
        return -1;
    string stripped = content;
    size_t te = stripped.rfind("</think>");
    if (te != string::npos)
        stripped = stripped.substr(te + 8);
    vector<string> lines, prevLines;
    vector<vector<string> > windows; //#W66-AR (H2b)
    collectLabeledLines(stripped, "ATTACK:", lines, &prevLines, &windows);
    (void) prevLines;
    (void) windows;
    if (lines.empty())
        return -1;
    vector<bool> usable(lines.size(), false), clean(lines.size(), false);
    vector<vector<bool> > sets(lines.size());
    vector<int> counts(lines.size(), -1);
    for (size_t i = 0; i < lines.size(); i++)
    {
        vector<bool> s;
        const int r = parseAttackerSet(lines[i], nAttackers, s, &attackerNames);
        if (r >= 0)
        {
            usable[i] = true;
            sets[i] = s;
            counts[i] = r;
        }
        clean[i] = combatLineIsClean(lines[i], &attackerNames, NULL);
    }
    const int idx = gptSelectAnswerIndex(usable, clean);
    if (idx < 0)
        return -1;
    if (ignoredLine && (size_t) idx + 1 < lines.size()) //#W66-AR (H2c)
        *ignoredLine = string("ATTACK:") + lines[lines.size() - 1];
    send = sets[(size_t) idx];
    if (takenText)
        *takenText = lines[(size_t) idx];
    if (takenIndex)
        *takenIndex = idx;
    if (notes)
        for (size_t i = (size_t) idx + 1; i < lines.size(); i++)
            if (usable[i])
            {
                //#W70-BM (E2): a second usable answer line. Counted as the
                //violation it is; the seam re-asks rather than refusing silently.
                appendParseNote(notes, "extra_answer_line");
                break;
            }
    return counts[(size_t) idx];
}


//#W64-AK (R1, wave-64 codex review finding 1): the row's own DESTINATION fact.
//A creature that may attack a planeswalker but not the player is a legal attack
//this seat may declare, and the only shape that declares it is `A#>W#` - a row
//that did not say so would be a row the model cannot legally take. Emitted only
//where it is TRUE and only where a target row exists to name; the ordinary
//attacker (may attack them, and a walker too when one is there) reads byte for
//byte as it did, so no window without this class changes. Pure over two facts.
static string attackerDestinationTag(bool mayAttackPlayer, bool mayAttackTarget)
{
    if (mayAttackPlayer || !mayAttackTarget)
        return "";
    return " [may NOT attack them: this one can only be sent at a planeswalker"
           " or battle, so it counts only when you write it as A#>W#]";
}


//#W64-AK (R1): and the aggregate below must not price it as damage to their
//life. Every aggregate says what it excludes (the standing rule); this is the
//sentence for the exclusion this class creates.
static string walkerOnlyExclusionLine(int walkerOnlyRows)
{
    if (walkerOnlyRows <= 0)
        return "";
    std::ostringstream o;
    o << "That total EXCLUDES " << walkerOnlyRows << " creature"
      << (walkerOnlyRows == 1 ? "" : "s") << " above that may NOT attack them at"
         " all: damage from "
      << (walkerOnlyRows == 1 ? "it" : "them")
      << " can only go at a planeswalker or battle, and never at their life"
         " total.\n";
    return o.str();
}


int AIPlayerGPT::chooseAttackers()
{
    //Only drive the declare-attackers step; anywhere else, stay out of the way.
    if (mEndpoint.empty() || observer->getCurrentGamePhase() != MTG_PHASE_COMBATATTACKERS)
        return AIPlayerBaka::chooseAttackers();
    if (mAttacksDoneTurn == observer->turn)
        return 1; //this turn's attack was already declared in one reply

    //The engine's request carries the legal candidates (contract c1).
    DecisionRequest req;
    if (!DecisionManager::buildDeclareAttackers(this, req))
        return 1;
    vector<MTGCardInstance *> & attackers = req.candidates;
    //#W49-R (D13): same-name candidates gathered adjacently, rank ascending,
    //BEFORE any row is built - the A# labels below index this permuted vector,
    //and so do the reply parser and the apply path (pointer membership).
    {
        vector<string> gn, gh;
        for (size_t j = 0; j < attackers.size(); j++)
        {
            gn.push_back(attackers[j]->name);
            gh.push_back(instanceHandle(attackers[j]));
        }
        vector<size_t> ord;
        groupCombatCandidates(gn, gh, ord);
        vector<MTGCardInstance *> perm;
        for (size_t j = 0; j < ord.size(); j++)
            perm.push_back(attackers[ord[j]]);
        attackers.swap(perm);
        //#W64-AK (R1): the contract's per-candidate destination legality is
        //PARALLEL to `candidates`, so it moves with the permutation or the A#
        //labels would carry another creature's legality.
        vector<bool> permPlayer, permTarget;
        for (size_t j = 0; j < ord.size(); j++)
        {
            permPlayer.push_back(ord[j] < req.candidateMayAttackPlayer.size()
                                 ? req.candidateMayAttackPlayer[ord[j]] : true);
            permTarget.push_back(ord[j] < req.candidateMayAttackTarget.size()
                                 ? req.candidateMayAttackTarget[ord[j]] : false);
        }
        req.candidateMayAttackPlayer.swap(permPlayer);
        req.candidateMayAttackTarget.swap(permTarget);
    }
    //#W64-AK (R1): a row's own destination legality, read only through these.
    #define W64AK_MAY_PLAYER(j) ((j) < req.candidateMayAttackPlayer.size() \
                                 ? req.candidateMayAttackPlayer[(j)] : true)
    #define W64AK_MAY_TARGET(j) ((j) < req.candidateMayAttackTarget.size() \
                                 ? req.candidateMayAttackTarget[(j)] : false)

    //ONE bundled decision for the whole attack. Per-creature asks decided
    //each attacker in isolation (a bad line for alpha strikes and racing)
    //and cost N round trips; the model now plans the attack as a whole.
    if (mSystemPrompt.empty())
        buildSystemPrompt();
    std::ostringstream tail;
    tail << "Combat: declare ALL attackers for this turn in ONE decision.\n"
            "Your creatures that can attack:\n";
    //Capture each presented option line for the translog (item: combat
    //options_text was EMPTY, which blocked wave-19's combat-decision review).
    vector<string> shownLines;
    bool anyHeldBack = false; //W41-13: at least one hold-back restriction shown
    bool anyPotentialBlockers = false; //W42-3: at least one trade forecast shown
    bool anyMenaceRestricted = false; //W43-1: at least one set-restricted attacker
    vector<string> aRowName, aRowHandle, aRowRest; //#W48 (D2): A-row collapse parts
    CombatWindowCache cw; //#W54-M (A22): per-window memo of the per-creature facts
    //#W63-AB (E1): and whether that loop can actually RUN (#W62-AA R6's rule).
    const bool oppLoopClosed = lifeLoopProvenWin(opponent());
    //#W60-L (B11): the parts of the aggregate line, gathered from the same pass
    //that builds the rows - the totals can never disagree with the rows above them.
    std::vector<int> rowPower;
    std::vector<bool> rowInfect; //#W60-Q (R4): damage that is poison, not life
    std::vector<bool> rowNoLegalBlock;
    std::vector<int> rowBlockersNeeded; //#W76-CO (Q4a): CR 509.1c, per attacker
    std::vector<int> aRowLoopAt; //#W65-AN (G6): reserved byte position per row
    for (size_t j = 0; j < attackers.size(); j++)
    {
        bool noLegalBlockForThisRow = false; //#W60-L (B11)
        string heldBackLine; //#W81-DL (V10): emitted on its own line below
        std::ostringstream ln;
        //#W48 (D2): the row is built WITHOUT its label/name/handle prefix so the
        //emitter below can collapse a run of rows that agree in every rendered
        //fact - the same three tests joinZoneEntries and R8's B-rows apply.
        ln << " (" << attackers[j]->power << "/" << attackers[j]->toughness << ")"
           << markedDamageTag(attackers[j]->toughness, attackers[j]->life); //#W82-EC (H3)
        string kw = keywordList(attackers[j]);
        if (!kw.empty())
            ln << " [" << kw << "]";
        //N-139j (wave-31 deck139): the BLOCKERS menu annotates a 0-power creature
        //("[deals 0 - this block kills nothing...]") and the ATTACKERS menu did
        //not, so 0/3 Arboreal Grazers were declared as attackers in FOUR separate
        //declarations, one of them reasoning out loud that "attacking with all
        //three 0/3 Grazers deals 3 damage". Effective power is already computed
        //for the "(P/T)" above; name the consequence at the line that decides,
        //mirroring the validated blocker text.
        ln << zeroPowerAttackerTag(attackers[j]->power);
        //#W64-AK (R1): which destinations THIS creature may legally be sent at.
        ln << attackerDestinationTag(W64AK_MAY_PLAYER(j), W64AK_MAY_TARGET(j));
        //W41-13: the hold-back half of the decision. Which of THEIR creatures
        //this body could not block if kept home, asked of the engine's own
        //pairwise gate rather than predicted from keywords.
        {
            vector<string> cannot;
            int totalOpposing = 0;
            Player * opp = opponent();
            if (opp && opp->game && opp->game->inPlay)
            {
                MTGGameZone * bf = opp->game->inPlay;
                for (int i = 0; i < bf->nb_cards; i++)
                {
                    MTGCardInstance * c = bf->cards[i];
                    if (!c || !c->isCreature())
                        continue;
                    totalOpposing++;
                    if (attackers[j]->couldBlockIfItAttacked(c))
                        continue;
                    std::ostringstream e;
                    e << c->name << instanceHandle(c);
                    const char * why = blockRestrictionReason(attackers[j], c);
                    if (why)
                        e << " (" << why << ")";
                    cannot.push_back(e.str());
                }
            }
            //#W81-DL (V10, wave-80 known-bugs V10 / deck146 - THE RENDER
            //PLACEMENT THAT COST A GAME). The tag sat INSIDE the offered
            //attacker's row, between the body's keywords and the forecast of
            //what can block it, and at `146v162` seq 52 most of two 245-byte
            //rows was this tag - a fact about NOT attacking, wedged into the
            //line that offers the attack, on the window deck146 lost with Ob
            //Nixilis at 3 loyalty reachable and never mentioned. It is a true
            //fact and nothing is deleted: it moves to a line of its own under
            //the row it is about. Held until every in-row annotation is emitted
            //so no bracket of the OFFER is separated from the offer.
            heldBackLine = heldBackBlockTag(cannot, totalOpposing);
            if (!heldBackLine.empty())
                anyHeldBack = true;
        }
        //W42-3: the attack half of the same fight. For every UNTAPPED creature
        //of theirs that could block this attacker, the computed 1-on-1 outcome
        //from the shared forecast - the machinery the blockers window already
        //uses, re-voiced for this seat. UNTAPPED-NOW is the honest scope: their
        //creatures do not untap before this combat's block step, so a tapped
        //body cannot block THIS attack (the hold-back tag above is the one that
        //speaks about next turn, and says so).
        {
            vector<string> entries;
            string biggest;
            int biggestPower = -1, biggestToughness = -1;
            //#W48-D4: the PRICE half of the collapsed representative. Life this
            //pairing hands the BLOCKING side - the blocking-declaration triggers
            //(Perimeter Captain, Pride Guardian; the same blockLife/blockLifeMay
            //the printed outcome quotes) plus lifelink on the blocker's own
            //damage. The lifelink half is dropped when that damage is prevented,
            //so the selector never ranks by a gain the forecast itself says
            //never happens; the "may" half counts, because a price the seat
            //cannot decline is exactly what this clause is for. The converter
            //doubling is deliberately NOT applied - it multiplies every
            //candidate on the same board equally and cannot change the ordering,
            //and the printed outcome states it where it belongs.
            string priciest;
            int priciestPrice = 0;
            //#W63-AB (E4b): the same prices, summed across the listed
            //candidates, plus how many of them carry a price at all.
            int priceSureSum = 0, priceMaySum = 0, pricedCandidates = 0;
            int bbP = 0, bbT = 0;
            int bbKind = becomesBlockedSelfPump(attackers[j]->text, bbP, bbT);
            //#W45-2: the gang price, computed HERE because this is the only
            //place each candidate's power is in hand. TRUST DOCTRINE: the
            //number is claimed only where it is exactly computable, so the
            //whole claim is abandoned (gangOk=false) the moment anything in the
            //fight is not plain simultaneous damage - first strike removes a
            //blocker's damage from the total before it lands, deathtouch/wither
            //kill on a number this sum does not model, indestructible and
            //persist/undying mean the sum kills nothing lasting, and a
            //prevention shield means the damage never arrives. A wrong N is
            //strictly worse than no N.
            //(history: comment-archaeology.md AIPlayerGPTSeams-L12312-2263)
            vector<int> gangPowers;
            vector<GangBlockerStat> gangStats; //#W64-AG (F7)
            bool gangOk = attackers[j]->toughness > 0
                          && !attackers[j]->basicAbilities[Constants::INDESTRUCTIBLE]
                          && !attackers[j]->basicAbilities[Constants::PERSIST]
                          && !attackers[j]->basicAbilities[Constants::UNDYING]
                          && bbKind == 0;
            Player * opp = opponent();
            if (opp && opp->game && opp->game->inPlay)
            {
                MTGGameZone * bf = opp->game->inPlay;
                for (int i = 0; i < bf->nb_cards; i++)
                {
                    MTGCardInstance * c = bf->cards[i];
                    if (!c || !c->isCreature())
                        continue;
                    //canBlock() is the engine's own solo gate (tapped without
                    //CANBLOCKTAPPED, can't-block, not in play); the pairwise
                    //half comes from couldBlockIfItAttacked, because at declare-
                    //attackers nothing is an attacker yet and canBlock(card)
                    //would refuse every pairing on isAttacker alone.
                    if (!c->canBlock())
                        continue;
                    if (!c->couldBlockIfItAttacked(attackers[j]))
                        continue;
                    string outcome;
                    //#W69-BI (K7): the blocking-trigger gain, taken OUT of the
                    //verdict string so `(neither dies)` closes as its own head
                    //and the price follows it as a sibling parenthesis.
                    string entryBlockTrigger;
                    //#W54-M (A22): the blocker's damage-to-attacker verdict is
                    //asked ONCE per pairing and read by the gang/price tests below.
                    const int cOnAttacker = combatPreventionKind(c, attackers[j]);
                    if (bbKind == 1)
                    {
                        //A when-blocked self-pump changes the fight before
                        //damage; fold it in exactly as the blockers window does.
                        CombatTradeStat as = cw.statOf(attackers[j]);
                        as.power += bbP;
                        as.toughness += bbT;
                        outcome = combatTradePreviewStats(cw.statOf(c), as,
                                                          combatPreventionKind(attackers[j], c),
                                                          cOnAttacker,
                                                          combatPreventionKindToPlayer(attackers[j], c->controller()),
                                                          true, c->life,
                                                          cw.converterOf(c->controller()),
                                                          &entryBlockTrigger); //#W69-BI (K7)
                    }
                    else
                        outcome = combatAttackOutcome(cw, attackers[j], c, cOnAttacker,
                                                      oppLoopClosed, //#W63-AB (E1)
                                                      &entryBlockTrigger); //#W69-BI (K7)
                    //#W69-BI (K7): the price, after the closed verdict, never
                    //inside it. Nothing is deleted - the same words, one
                    //parenthesis later.
                    const string entryText = attackerBlockerEntry(c->name + instanceHandle(c),
                                                                 c->power, c->toughness,
                                                                 outcome, entryBlockTrigger);
                    entries.push_back(entryText);
                    if (cOnAttacker != kPreventNone) //#W54-M (A22)
                        gangOk = false;
                    gangPowers.push_back(c->power > 0 ? c->power : 0);
                    {   //#W64-AG (F7): the stats the derived verdict reads.
                        GangBlockerStat gs;
                        gs.power = c->power > 0 ? c->power : 0;
                        gs.toughness = c->toughness;
                        gs.deathtouch = c->basicAbilities[Constants::DEATHTOUCH] != 0;
                        gs.firstStrike = c->basicAbilities[Constants::FIRSTSTRIKE] != 0
                                         || c->basicAbilities[Constants::DOUBLESTRIKE] != 0;
                        gangStats.push_back(gs);
                    }
                    //"biggest" is decided by POWER first and toughness as the
                    //tie-break: a board of 0-power walls is exactly the case the
                    //cap fires on, and naming a 0/2 Fog Bank as the biggest body
                    //over a 0/4 wall would be a false superlative.
                    if (c->power > biggestPower
                        || (c->power == biggestPower && c->toughness > biggestToughness))
                    {
                        biggestPower = c->power;
                        biggestToughness = c->toughness;
                        biggest = entryText;
                    }
                    //#W48-D4: and the most expensive one, by the life it gives.
                    {
                        CombatTradeStat bstat = cw.statOf(c); //#W54-M (A22)
                        int price = bstat.blockLife + bstat.blockLifeMay;
                        int sure = bstat.blockLife;
                        if (bstat.lifelink && c->power > 0
                            && cOnAttacker == kPreventNone)
                        {
                            price += c->power;
                            sure += c->power;
                        }
                        //#W63-AB (E4b): the same per-candidate price, added up.
                        //The certain and the "may" halves stay apart - a gain
                        //the script only MAY take is not summed into a figure
                        //this seat can rely on.
                        if (price > 0)
                            pricedCandidates++;
                        priceSureSum += sure;
                        priceMaySum += bstat.blockLifeMay;
                        if (price > priciestPrice)
                        {
                            priciestPrice = price;
                            priciest = entryText;
                        }
                    }
                }
            }
            string bbNote;
            if (!entries.empty() && bbKind == 1)
            {
                std::ostringstream n;
                n << "each outcome above INCLUDES its +" << bbP << "/+" << bbT
                  << " when-blocked trigger";
                bbNote = n.str();
            }
            else if (!entries.empty() && bbKind == 2)
                bbNote = "each outcome above OMITS its becomes-blocked trigger:"
                         " read its text";
            //W43-1: the declaration-set restriction rides the attacker's own
            //line, ahead of the forecast it governs, and BELOW the minimum the
            //forecast is dropped entirely - lane B's filter applied flying
            //legality but not menace, so a body nobody could legally block
            //alone still carried "(neither dies)" 1-on-1 tags and was read as
            //free damage in the wrong direction.
            //#W47-R11: read BEFORE the menace branch below can clear the
            //list, so a cleared set is never mistaken for an empty board.
            const bool noneCouldBlock = entries.empty();
            const int minB = attackers[j]->minBlockersRequired();
            ln << menaceAttackRestrictionTag(minB, (int) entries.size());
            if (minB > 1 && (int) entries.size() < minB)
            {
                anyMenaceRestricted = true;
                entries.clear();
            }
            else if (minB > 1)
                anyMenaceRestricted = true;
            //#W45-2: the cheapest lethal gang, from the powers of exactly the
            //candidates listed above. `need` is found from the LARGEST first
            //(the smallest number that can do it at all); `anyOfThem` re-runs
            //the same count from the SMALLEST, and is true only when even the
            //weakest `need` of them reach the toughness - which is what makes
            //the word "any" a fact rather than a flourish. need<2 means a lone
            //blocker already kills it and the listed 1-on-1 results say so.
            string gangNote;
            if (gangOk && entries.size() >= 2 && gangStats.size() == entries.size())
            {
                //#W64-AG (F7): the same combat math the 1-on-1 results use.
                const bool atkFS = attackers[j]->basicAbilities[Constants::FIRSTSTRIKE] != 0
                                   || attackers[j]->basicAbilities[Constants::DOUBLESTRIKE] != 0;
                const bool atkDT = attackers[j]->basicAbilities[Constants::DEATHTOUCH] != 0;
                int dmg = 0;
                bool anyOfThem = false, byDt = false;
                int soak = 0; //#W79-DA (T5)
                const int need = gangKillNeed(attackers[j]->power, attackers[j]->toughness,
                                              atkFS, atkDT, gangStats, minB,
                                              &dmg, &anyOfThem, &byDt, &soak);
                if (need >= 2)
                {
                    //#W79-DA (T5): the same carry-over, voiced for the seat that
                    //owns the trampler - a gang block of theirs that kills it
                    //still does not stop all of its damage.
                    int spill = -1;
                    if (attackers[j]->basicAbilities[Constants::TRAMPLE])
                    {
                        const int ap = attackers[j]->power > 0 ? attackers[j]->power : 0;
                        spill = ap - soak > 0 ? ap - soak : 0;
                    }
                    //#W63-AB (E4a): hoisted to the front of the tag, so the
                    //clause's own pointer at the 1-on-1 results inverts.
                    gangNote = gangBlockPriceTag(need, dmg, anyOfThem, false, true,
                                                 byDt, atkFS, spill, soak); //#W79-DA (T5)
                }
            }
            //#W63-AB (E4b): the summed life price of the listed candidates.
            const string sumNote = blockPriceSumTag(pricedCandidates, priceSureSum,
                                                    priceMaySum);
            string pb = potentialBlockersTag(entries, biggest, bbNote, gangNote,
                                             priciest, sumNote); //#W48-D4 / #W63-AB (E4b)
            if (!pb.empty())
            {
                anyPotentialBlockers = true;
                //#W69-BI (K7, deck146 MED): the census that tells a DEAD
                //surface from one whose window never arose. Counted on the
                //wave-68 corpus by hand: 48 rows carried this tag, 33 with one
                //candidate and 15 with two, none with more than four and none
                //needing two bodies to kill the attacker - so `- GANG BLOCK:`
                //and the collapsed form rendered 0 times while both emitters
                //were reachable. The record now says which of the four it was,
                //per game, instead of a grep over 42 prompt files.
                mBlockerForecastRows++;
                if (entries.size() >= 2)
                    mBlockerForecastMulti++;
                if (!gangNote.empty())
                    mBlockerForecastGang++;
                if (entries.size() > kPotentialBlockersEnumerateMax)
                    mBlockerForecastCollapsed++;
            }
            ln << pb;
            if (noneCouldBlock && minB < 2)
                ln << noPotentialBlockersTag();
            //#W60-L (B11): after the menace clearing, an empty candidate set IS
            //"no untapped creature of theirs may legally block this attacker" -
            //the same predicate the two tags above print, so the floor below and
            //the rows agree by construction.
            noLegalBlockForThisRow = entries.empty();
        }
        //#W65-AN (G6): the clause itself is composed after the ATTACK TOTAL
        //below - see lifeLoopAttackerRowTag - so only its byte position on this
        //row is reserved here, which keeps the composed row order identical.
        //It is reserved at all only where a branch of the chain can be entered
        //FROM THIS ROW: with nothing of theirs able to block this attacker,
        //declaring it gains them no life off a block, and `oppLoopClosed` is
        //the #W62-AA R6 gate that the row tails and the LOOP SCOPE paragraph in
        //this same window are already built from, so the three cannot disagree
        //about whether the chain can run at all.
        aRowLoopAt.push_back((oppLoopClosed && !noLegalBlockForThisRow)
                             ? (int) ln.tellp() : -1);
        //#W60-P (B9): an attacker holding one of theirs in exile can die in
        //this combat and hand it straight back. Same fact the blocker rows
        //carry, on the window where the seat chooses to expose the body.
        {
            vector<MTGCardInstance *> heldA;
            exileHostagesOf(attackers[j], heldA);
            vector<string> heldDesc;
            for (size_t hi = 0; hi < heldA.size(); hi++)
                heldDesc.push_back(exileHostageDescriptor(heldA[hi], this));
            ln << exileHostageRowTag(heldDesc);
        }
        //#W81-DL (V10): and now the hold-back fact, on a line of its own.
        if (!heldBackLine.empty())
            ln << "\n   " << heldBackLine;
        aRowName.push_back(attackers[j]->name);
        aRowHandle.push_back(instanceHandle(attackers[j]));
        aRowRest.push_back(ln.str());
        //#W60-Q (R4): what this attacker takes off a LIFE TOTAL if it connects.
        //Infect takes nothing (poison), double strike takes it twice.
        {
            int rp = attackers[j]->power > 0 ? attackers[j]->power : 0;
            //#W64-AK (R1): a creature that may not attack THEM contributes
            //nothing to a total about their life total. Counted separately and
            //disclosed under the aggregate, never silently folded in.
            if (!W64AK_MAY_PLAYER(j))
                rp = 0;
            if (attackers[j]->basicAbilities[Constants::INFECT])
            {
                rowInfect.push_back(true);
                rp = 0;
            }
            else
            {
                rowInfect.push_back(false);
                if (attackers[j]->basicAbilities[Constants::DOUBLESTRIKE])
                    rp *= 2;
            }
            rowPower.push_back(rp); //#W60-L (B11)
        }
        //#W76-CO (Q4a): how many bodies a LEGAL block of this attacker needs -
        //the ENGINE's own answer (menace 2, "three or more" 3), the same one
        //the per-attacker menace tag above is rendered from.
        {
            const int need = attackers[j]->minBlockersRequired();
            rowBlockersNeeded.push_back(need > 0 ? need : 1);
        }
        rowNoLegalBlock.push_back(noLegalBlockForThisRow);
    }
    //#W65-AN (G6, deck123 HIGH-1). The totals are built FIRST, into their own
    //buffer, because the A-row life-LOOP clause has to yield to the ATTACK
    //TOTAL's kill verdict and cannot read a line that has not been composed.
    //NOTHING moves in the rendered order: this buffer is emitted below, after
    //the rows, in exactly the position these lines occupied before.
    std::ostringstream totalsTail;
    bool attackTotalKillClaim = false; //#W65-AN (G6)
    //#W54-E (D17): the opposite number, once, from the engine's own solo block
    //gate - the same canBlock() the per-attacker lists above are filtered by,
    //so the header and the rows can never disagree. UNTAPPED-NOW is the honest
    //scope for the same reason the rows use it: their creatures do not untap
    //before this combat's block step.
    {
        int blockerCount = 0;
        Player * oppB = opponent();
        if (oppB && oppB->game && oppB->game->inPlay)
        {
            MTGGameZone * bf = oppB->game->inPlay;
            for (int i = 0; i < bf->nb_cards; i++)
            {
                MTGCardInstance * c = bf->cards[i];
                if (!c || !c->isCreature() || !c->canBlock())
                    continue;
                //#W77-CS (R7, wave-76 deck123 MED-2 / deck152 MED-1): the count
                //was a body count of everything UNTAPPED, printed as a claim
                //about THIS attack. `123v152` deck123 seq 104 said "Their 3
                //untapped blockers" over 11 fliers only Elite Spellbinder could
                //block, and the floor it feeds read 16 where it is 20. The
                //parameterless canBlock() is the SOLO gate (untapped, no
                //can't-block) and says nothing about whether the body can block
                //anything ON OFFER - exactly the distinction #W61-V (R8) drew
                //for the blocking-life ceiling one line below, on the same
                //pairwise map (couldBlockIfItAttacked, because nothing is an
                //attacker yet at declare-attackers). A body with no legal block
                //among the listed attackers is not a blocker of this attack.
                bool canBlockSomethingOffered = false;
                for (size_t aj = 0; aj < attackers.size() && !canBlockSomethingOffered; aj++)
                    if (attackers[aj] && c->couldBlockIfItAttacked(attackers[aj]))
                        canBlockSomethingOffered = true;
                if (canBlockSomethingOffered)
                    blockerCount++;
            }
        }
        //#W64-AJ (deck162 MED): how many of the offered attackers no untapped
        //creature of theirs may legally block - the same per-row predicate the
        //floor below counts, so the header cannot contradict the rows.
        int evasiveRows = 0;
        for (size_t ej = 0; ej < rowNoLegalBlock.size(); ej++)
            if (rowNoLegalBlock[ej])
                evasiveRows++;
        totalsTail << attackerBlockerCountLine(blockerCount, evasiveRows,
                                         (int) rowNoLegalBlock.size());
        //#W66-AT (H6): and the bodies the candidate walk left out because they
        //are summoning sick. Same predicate as the board line, membership tested
        //against the permuted candidate vector so a creature that IS offered can
        //never appear here.
        {
            std::vector<std::string> sickNames;
            std::vector<bool> sickNextTurn; //#W66-AU (R4)
            if (game && game->inPlay)
                for (int si = 0; si < game->inPlay->nb_cards; si++)
                {
                    MTGCardInstance * sc = game->inPlay->cards[si];
                    if (!sc || !sc->isCreature() || !sc->hasSummoningSickness())
                        continue;
                    bool offered = false;
                    for (size_t aj = 0; aj < attackers.size() && !offered; aj++)
                        if (attackers[aj] == sc)
                            offered = true;
                    if (!offered)
                    {
                        sickNames.push_back(sc->getDisplayName() + instanceHandle(sc));
                        //#W66-AU (R4): the NEXT-turn permission is claimed only when
                        //MTGCardInstance::canAttack's other gates would all pass once
                        //the sickness lifts - no CANTATTACK, no flyers-only bar, no
                        //Defender without CANATTACK, not a Battle, and no unpaid
                        //attack cost. Everything else is named as sick and no more.
                        const bool wouldAttack =
                            !sc->has(Constants::CANTATTACK)
                            && !(sc->has(Constants::FLYERSONLY) && !sc->has(Constants::FLYING))
                            && !(sc->has(Constants::DEFENSER) && !sc->has(Constants::CANATTACK))
                            && !sc->hasType(Subtypes::TYPE_BATTLE)
                            && sc->attackCost <= 0;
                        sickNextTurn.push_back(wouldAttack);
                    }
                }
            totalsTail << attackersHeldSickLine(sickNames, sickNextTurn);
        }
        //#W60-L (B11): and the arithmetic that count line stops one step short
        //of. `guaranteed` counts every attacker nothing of theirs may block, then
        //the SMALLEST powers among the rest that their blocker count cannot
        //cover - the worst case for the attacker, so the number is a proven floor.
        {
            int totalPower = 0, guaranteed = 0, infectExcluded = 0;
            std::vector<int> blockablePowers;
            std::vector<int> blockableNeeds; //#W76-CO (Q4a)
            int blockableTotal = 0;
            for (size_t j = 0; j < rowPower.size(); j++)
            {
                if (j < rowInfect.size() && rowInfect[j])
                {
                    infectExcluded++; //#W60-Q (R4): out of the life arithmetic
                    continue;
                }
                totalPower += rowPower[j];
                if (j < rowNoLegalBlock.size() && rowNoLegalBlock[j])
                    guaranteed += rowPower[j];
                else
                {
                    blockablePowers.push_back(rowPower[j]);
                    blockableNeeds.push_back(j < rowBlockersNeeded.size()
                                             ? rowBlockersNeeded[j] : 1);
                    blockableTotal += rowPower[j];
                }
            }
            //#W76-CO (Q4a): what is left after they block as much power as
            //their bodies legally CAN - each attacker costing the bodies CR
            //509.1c makes it cost. With every requirement 1 this is the old
            //sum-of-the-smallest-remaining, byte for byte.
            guaranteed += blockableTotal
                        - w76BlockableCoveredPower(blockablePowers, blockableNeeds,
                                                   blockerCount);
            Player * oppL = opponent();
            //#W60-Q (R4): a player who cannot lose life gets no life claim.
            bool suppressed = false;
            if (oppL && oppL->game && oppL->game->inPlay)
                for (int si = 0; si < oppL->game->inPlay->nb_cards && !suppressed; si++)
                {
                    MTGCardInstance * sc = oppL->game->inPlay->cards[si];
                    if (sc && (sc->basicAbilities[Constants::CANTLOSE]
                               || sc->basicAbilities[Constants::CANTLIFELOSE]))
                        suppressed = true;
                }
            //#W61-R (C1a): the life every blocker they own can gain them by
            //blocking, over the SAME `canBlock()` set the count line above
            //states - so the aggregate and the cap cannot disagree. Both the
            //certain and the "may" halves are in the ceiling because a "may"
            //is theirs to take and this number is what the seat must survive.
            int blockGain = 0;
            int blockLifelinkCeiling = 0; //#W65-AP (R4)
            std::vector<int> blockerLife;
            std::vector<bool> blockerCanBlockOffered;
            if (oppB && oppB->game && oppB->game->inPlay)
            {
                MTGGameZone * bfG = oppB->game->inPlay;
                for (int i = 0; i < bfG->nb_cards; i++)
                {
                    MTGCardInstance * c = bfG->cards[i];
                    if (!c || !c->isCreature() || !c->canBlock())
                        continue;
                    //#W61-V (R8, wave-61 codex review finding 8): parameterless
                    //canBlock() is the SOLO gate (untapped, no can't-block) and
                    //says nothing about whether this body can block anything on
                    //OFFER. A Perimeter Captain facing an all-shadow attack has
                    //no legal block, so it has no blocking trigger, so its
                    //lifegain does not exist - and folding it in produced a
                    //"life back" ceiling that can erase a proven guaranteed kill.
                    //The pairwise map the per-attacker rows are built from is the
                    //same one used here (couldBlockIfItAttacked, because nothing
                    //is an attacker yet at declare-attackers), so the ceiling and
                    //the rows cannot disagree.
                    bool blocksSomethingOffered = false;
                    for (size_t aj = 0; aj < attackers.size() && !blocksSomethingOffered; aj++)
                        if (attackers[aj] && c->couldBlockIfItAttacked(attackers[aj]))
                            blocksSomethingOffered = true;
                    int sure = 0, may = 0;
                    blockTriggeredLifeFor(c, sure, may);
                    blockerLife.push_back(sure + may);
                    blockerCanBlockOffered.push_back(blocksSomethingOffered);
                    //#W65-AP (R4): and the LIFELINK the same body gains them by
                    //dealing its combat damage. Kept in its own total because
                    //the line names it separately - it is not a blocking
                    //trigger, it is the block itself. A CEILING, like every
                    //other term here: the block they choose is theirs, so the
                    //number the seat must survive assumes the damage is dealt
                    //(double strike deals it twice).
                    if (blocksSomethingOffered && c->power > 0
                        && c->basicAbilities[Constants::LIFELINK])
                        blockLifelinkCeiling += c->power
                            * (c->basicAbilities[Constants::DOUBLESTRIKE] ? 2 : 1);
                }
            }
            blockGain = blockingLifeCeiling(blockerLife, blockerCanBlockOffered);
            //#W74-CC (O5, deck126 HIGH-1): the lifelink half of the seat's OWN
            //attack, when the seat controls a life-to-damage converter. Read off
            //the SAME offered rows the total above is summed from, so the two
            //cannot disagree; double strike deals its damage twice, exactly as
            //the blockers' ceiling counts it.
            int selfConvLifelink = 0;
            {
                //#W74-CF (F5): the converters' own RATES, off the battlefield -
                //a proportional converter (Sanguine Bond) converts every point
                //gained and TWO of them convert it twice; a fixed converter
                //(Cliffhaven Vampire) converts its printed number once per gain
                //EVENT, whatever the size of the gain. The boolean this replaced
                //priced a 1-life trigger as if it were the whole lifelink total.
                int propConverters = 0, fixedPerEvent = 0;
                if (game && game->inPlay)
                    for (int ci = 0; ci < game->inPlay->nb_cards; ci++)
                    {
                        if (!game->inPlay->cards[ci])
                            continue;
                        bool prop = false;
                        const int rate = lifeToDamageConverterRate(
                            game->inPlay->cards[ci]->magicText, prop);
                        if (prop)
                            propConverters++;
                        else if (rate > 0)
                            fixedPerEvent += rate;
                    }
                if (propConverters > 0 || fixedPerEvent > 0)
                {
                    int gainPoints = 0, gainEvents = 0;
                    for (size_t ai = 0; ai < attackers.size() && ai < rowPower.size(); ai++)
                    {
                        MTGCardInstance * ac = attackers[ai];
                        if (!ac || !ac->basicAbilities[Constants::LIFELINK] || ac->power <= 0)
                            continue;
                        const int mult = ac->basicAbilities[Constants::DOUBLESTRIKE] ? 2 : 1;
                        gainPoints += ac->power * mult;
                        gainEvents += mult;
                    }
                    selfConvLifelink = propConverters * gainPoints
                                       + fixedPerEvent * gainEvents;
                }
            }
            //#W79-DA (T9): the punisher the line already NAMES, sized for the
            //one declaration the line prices - all of the listed attackers.
            int punPer = 0, punKilled = 0, punKilledPower = 0;
            std::ostringstream punVictims;
            {
                bool punSized = false;
                punPer = attackDeclarationPunisherSize(oppL, (int) rowPower.size(), punSized);
                if (!punSized)
                    punPer = 0;
                if (punPer > 0)
                    for (size_t ai = 0; ai < attackers.size() && ai < rowPower.size(); ai++)
                    {
                        MTGCardInstance * ac = attackers[ai];
                        if (!ac || ac->basicAbilities[Constants::INDESTRUCTIBLE])
                            continue;
                        const int rem = ac->life > 0 ? ac->life : ac->toughness;
                        if (rem > punPer)
                            continue;
                        if (punKilled < 6)
                            punVictims << (punKilled ? ", " : "")
                                       << ac->getDisplayName() << instanceHandle(ac);
                        punKilled++;
                        punKilledPower += rowPower[ai] > 0 ? rowPower[ai] : 0;
                    }
            }
            totalsTail << attackTotalLine((int) rowPower.size(), totalPower,
                                    oppL ? oppL->life : -1, blockerCount, guaranteed,
                                    infectExcluded, suppressed, blockGain,
                                    attackDeclarationPunishers(oppL),
                                    playerHasLifeLoop(oppL), //#W62-X (D2)
                                    &attackTotalKillClaim, //#W65-AN (G6)
                                    blockLifelinkCeiling, //#W65-AP (R4)
                                    selfConvLifelink, //#W74-CC (O5)
                                    punPer, punKilled, punKilledPower, //#W79-DA (T9)
                                    punVictims.str());
            //#W64-AK (R1): and the exclusion this wave's new row class creates.
            {
                int walkerOnlyRows = 0;
                for (size_t j = 0; j < attackers.size(); j++)
                    if (!W64AK_MAY_PLAYER(j))
                        walkerOnlyRows++;
                totalsTail << walkerOnlyExclusionLine(walkerOnlyRows);
            }
        }
    }
    //#W65-AN (G6): and now the loop clause, in the byte position each row
    //reserved for it, with the verdict the ATTACK TOTAL above actually printed.
    for (size_t j = 0; j < aRowRest.size(); j++)
    {
        if (j >= aRowLoopAt.size() || aRowLoopAt[j] < 0
            || (size_t) aRowLoopAt[j] > aRowRest[j].size())
            continue;
        aRowRest[j].insert((size_t) aRowLoopAt[j],
                           lifeLoopAttackerRowTag(true, attackTotalKillClaim));
    }
    //The TRANSLOG keeps one entry per option, uncollapsed: it is the ordered
    //option list, not the rendered prompt. Built after the clause above so the
    //logged row is byte-identical to the rendered one.
    for (size_t j = 0; j < aRowRest.size(); j++)
    {
        std::ostringstream full;
        full << "A" << (j + 1) << ". " << aRowName[j] << aRowHandle[j] << aRowRest[j];
        shownLines.push_back(full.str());
    }
    {
        bool anyAttackerRangeRow = false;
        tail << joinBlockerRows(aRowName, aRowHandle, aRowRest, &anyAttackerRangeRow, "A");
        if (anyAttackerRangeRow)
            tail << kAttackerRangeNote;
    }
    tail << totalsTail.str(); //#W65-AN (G6): composed above, printed here
    //Wave-35 churn driver #5 (batch5 #12): the attackers ask stated neither of
    //the two facts that decide it, so the model derived them from scratch - one
    //26,457-char trace on a ONE-legal-attacker decision spent ~20,000 chars
    //asking whether it had already missed its chance to cast a creature, and
    //found "I could cast Sigarda today after combat" as the LAST novel element
    //at char 23,100. Both facts are unconditionally true at this ask: a declare-
    //attackers decision only exists on your own turn, so your second main phase
    //is always still ahead of you, and you always receive priority again during
    //combat. Stating them is cheaper than the model re-deriving them, and the
    //trust doctrine says the surface owes the model the truth rather than a gap
    //to confabulate into.
    //W41-13 SCOPE, stated once rather than on every line: the hold-back tag is
    //a legality fact about the board AS IT STANDS, not a forecast. Saying so
    //costs one line and keeps the tag from being read as a promise about what
    //the opponent will do - a true statement in the wrong scope is a lie.
    if (anyHeldBack)
        tail << "A \"held back\" tag lists their CURRENT creatures that body could"
                " not legally block if you keep it home. It says nothing about"
                " whether they will attack with those creatures."
                //#W73-CA (N14, deck146 MED 5): and which DIRECTION it runs in,
                //because the same names appear in a \"their untapped blockers\"
                //tag on the same line and the two were read as one claim.
                " It runs the opposite way from a \"their untapped blockers\" tag:"
                " that one lists creatures that could block THIS attacker, this one"
                " lists creatures THIS body could not block. The same name can"
                " honestly appear in both.\n";
    //W42-3 SCOPE, stated once rather than on every line - the same trade-trust
    //register the blockers window uses for its parentheses. The forecast is the
    //naive 1-on-1 fight and the defender chooses whether to block at all, so say
    //so; and say the outcome is computed, because the failure being fixed here
    //is the model deriving it itself and getting it wrong.
    //#W45-2: "before gang-blocks" was the whole disclaimer, and it is not one -
    //it says a gang block is unmodelled without saying it is available. Name the
    //assumption every listed result carries (ONE blocker), say that the tag may
    //be collapsed to the biggest body rather than enumerated, and point at the
    //GANG BLOCK clause that prices it when the price is exactly computable.
    if (anyPotentialBlockers)
        tail << "A \"their untapped blockers\" tag lists each of their currently"
                " untapped creatures that could legally block that attacker, with"
                " the computed 1-on-1 result if it does - before pump or combat"
                " tricks. EVERY such result assumes ONE blocker: two or more of"
                " theirs may block the same attacker together and add their power,"
                " which no listed result includes. On a wide board the tag is"
                " collapsed to a count plus their biggest body instead of the full"
                " list, so the results you see are not all the results there are;"
                " a \"GANG BLOCK:\" clause, when present, is the cheapest lethal"
                " group the listed creatures can field. They choose whether to"
                " block. Do not re-derive these outcomes; use them.\n"
    //#W47-R3, the companion half. Every parenthetical inside a listed result
    //that names life THEY gain is a COST attached to that outcome, and the
    //model read it as part of the outcome: deck162 sent a 3/4 into three 1/1
    //lifelinkers whose lines each said "(you kill it, your attacker lives
    //(lifelink: they gain 1))" and went 19 -> 0 in that combat. #W46-9 bound
    //the gain to the branch for the FATAL branch only; the friendly branches
    //carry the same tail unframed, so the framing is stated once here for all
    //of them.
                "Inside a listed result, a parenthesis naming life THEY gain is"
                " the PRICE of that outcome, not part of it: the fight result is"
                " the first clause and the gain happens as well as it, never"
                " instead of it.\n";
    //W43-1 SCOPE, stated once. The listed 1-on-1 outcomes on a set-restricted
    //attacker describe a fight that can only happen as part of a multi-creature
    //block, so say which way the restriction cuts rather than leaving the model
    //to reconcile two tags on the same line.
    if (anyMenaceRestricted)
        tail << "An attacker that cannot be blocked by fewer than two (or three)"
                " creatures is blocked only when that many of theirs block it"
                " TOGETHER. Any 1-on-1 result listed for it is what would happen"
                " to that one creature inside such a group block, never on its"
                " own.\n";
    //#W45-3, the summary half - #W46-3 re-scoped. The NAMING and the mechanism
    //now live in the CURRENT SITUATION block above (converterSituationLine), so
    //every decision kind carries them and a creatureless control seat, which is
    //never asked to declare attackers, finally reads the explanation of the
    //permanent that is killing it. What stays here is the one fact only THIS
    //screen owns: the "blocking trigger" gains annotated on the lines above are
    //life gains, so a converter converts them. Stated as a consequence of
    //blocking, not as a prediction that they will block.
    if (!converterSituationLine(this, opponent()).empty())
    {
        tail << "The \"blocking trigger\" gains listed above are LIFE GAINS, so the"
                " LIFE-TO-DAMAGE CONVERTER named in the CURRENT SITUATION block"
                " applies to them: a converter of THEIRS turns each such gain into"
                " that much life off YOUR total as well.";
        //#W49-U D5: with both halves of the loop on their side, the screen's own
        //rule is stated where the ATTACK: line is written.
        if (playerHasLifeLoop(opponent()))
            tail << " Both halves of their life LOOP are in play (see CURRENT"
                    " SITUATION): any life they gain or you lose in combat chains"
                    " until you are at 0, so an attack that lets them gain ANY"
                    " life, or costs you ANY life, is fatal.";
        tail << "\n";
    }
    //#W64-AH (F11, deck130 MED-3): the crack-back figure the same window prints
    //is identical whether the seat attacks with 0, 1 or 2 bodies - so no number
    //on the screen moved when the choice that killed the seat was made (rerun
    //seq 45, died at -4). Declaring an attacker TAPS it, and a tapped creature
    //cannot block: every attacker without vigilance is one fewer blocker against
    //the line above. Stated only where that line is printed and lethal, and it
    //names what it excludes.
    {
        int cbTotal = 0;
        bool cbFloor = false;
        //#W81-DL (V10, wave-80 known-bugs V10 / deck152). THE COVER PARAGRAPH
        //FIRED ONLY WHERE IT WAS ALREADY TOO LATE. The gate demanded the total
        //be ALREADY LETHAL, so of eight deck152 attack prompts that printed
        //`CRACK-BACK NEXT TURN`, exactly one carried the cover paragraph - the
        //one where the seat was already dead to it - and `152v146` seq 19, where
        //the choice still decided the game, got nothing. The fact the paragraph
        //states ("declaring an attacker taps it, and a tapped creature cannot
        //block") is true of every attackers window that prints the line, and the
        //STAY HOME tag beneath it prints its own survival verdict from the same
        //arithmetic, so the non-lethal face is a number, not a scare. The gate is
        //now exactly the one the LINE itself is printed under: the cover clause
        //count and the crack-back header count are one figure on attackers
        //prompts.
        if (w81AttackCoverDue(crackBackScreenTotalNow(cbTotal, cbFloor),
                              !attackers.empty())) //#W81-DL (V10)
        {
            mW81PendingEventFace += "attackers_crackback_cover_clause;"; //#W81-DL (V10)
            int vigilant = 0;
            for (size_t vi = 0; vi < attackers.size(); vi++)
                if (attackers[vi] && attackers[vi]->has(Constants::VIGILANCE))
                    vigilant++;
            tail << "CRACK-BACK COST OF ATTACKING: the CRACK-BACK NEXT TURN line"
                    " above (" << cbTotal << " damage, which puts you at "
                 << (life - cbTotal) << ") is computed over THEIR creatures only -"
                    " it does not change when you declare attackers, but what you"
                    " have left to BLOCK it with does. Declaring an attacker taps"
                    " it, and a tapped creature cannot block, so every attacker"
                    " you declare without vigilance is one fewer blocker against"
                    " that line, and each blocker you keep back stops at most ONE"
                    " attacker. Of the " << attackers.size() << " creature"
                 << (attackers.size() == 1 ? "" : "s") << " offered above, "
                 << vigilant << " ha" << (vigilant == 1 ? "s" : "ve")
                 << " vigilance and stay" << (vigilant == 1 ? "s" : "")
                 << " untapped after attacking.";
            //#W77-CS (R6 c): ...and the NUMBER for the other side of that
            //choice. Same walk, same greedy, same legality map as every sibling
            //in the cover family - the offered bodies plus any untapped
            //creature of the seat's that is not on the attack menu.
            {
                std::vector<MTGCardInstance *> homeBodies;
                if (game && game->inPlay)
                    for (int hi = 0; hi < game->inPlay->nb_cards; hi++)
                    {
                        MTGCardInstance * hc = game->inPlay->cards[hi];
                        if (!hc || !hc->isCreature() || hc->isTapped() || !hc->canBlock())
                            continue;
                        homeBodies.push_back(hc);
                    }
                std::vector<CrackBackAttackerFact> cbAtk;
                Player * cbOpp = opponent();
                MTGGameZone * cbBf = (cbOpp && cbOpp->game) ? cbOpp->game->inPlay : NULL;
                for (int ci = 0; cbBf && ci < cbBf->nb_cards; ci++)
                {
                    MTGCardInstance * cc = cbBf->cards[ci];
                    const int pw = crackBackBodyContribution(cc);
                    if (pw <= 0)
                        continue;
                    CrackBackAttackerFact f;
                    f.power = pw;
                    f.blockersNeeded = cc->minBlockersRequired();
                    f.trample = cc->basicAbilities[Constants::TRAMPLE] != 0; //#W80-DH (F8)
                    f.coverable = false;
                    for (size_t hj = 0; hj < homeBodies.size(); hj++)
                        if (homeBodies[hj]->canBlockPairwise(cc) != 0)
                            f.existingBlockers.push_back((int) hj);
                    cbAtk.push_back(f);
                }
                bool w80HomeTrample = false; //#W80-DH (F8)
                for (size_t ti = 0; ti < cbAtk.size() && !w80HomeTrample; ti++)
                    w80HomeTrample = cbAtk[ti].trample;
                tail << w77StayHomeCoverTag(cbTotal, life, cbFloor,
                                            (int) homeBodies.size(),
                                            w77StayHomeCoveredPower(cbAtk,
                                                (int) homeBodies.size()),
                                            w80HomeTrample);
            }
            tail << "\n";
        }
    }
    //#W64-AI (F4, deck152 HIGH-2): the attack's OTHER legal destination. The
    //engine has had MTGPlaneswalkerAttackRule for years and no prompt ever said
    //so - 0 of the corpus's attackers windows named a planeswalker, and at
    //152v162 seq 17 a 3-loyalty Ob Nixilis faced three unblockable attackers on
    //a creatureless board, survived, and dealt 6 of the damage that killed the
    //seat. Rows only (nothing is removed and nothing is suggested): the default
    //answer shape is unchanged, so a reply that names no target attacks the
    //player exactly as before. The order is the contract's, which is the
    //engine's own menu order, so the W# the model writes IS the index the apply
    //path clicks.
    if (!req.attackTargets.empty())
    {
        tail << "Their planeswalkers/battles - you may send any attacker at ONE of"
                " these INSTEAD of at them:\n";
        for (size_t w = 0; w < req.attackTargets.size(); w++)
        {
            MTGCardInstance * pw = req.attackTargets[w];
            int loyalty = 0;
            if (pw->counters && pw->counters->mCount)
                for (size_t c = 0; c < pw->counters->counters.size(); c++)
                {
                    Counter * ct = pw->counters->counters[c];
                    if (ct && ct->nb > 0 && ct->name.find("loyalty") != string::npos)
                        loyalty += ct->nb;
                }
            tail << " W" << (w + 1) << ". " << pw->getDisplayName() << instanceHandle(pw)
                 << (pw->hasType(Subtypes::TYPE_BATTLE) ? " [battle]" : " [planeswalker]");
            if (loyalty > 0)
                tail << " [" << loyalty << " loyalty left: combat damage removes that"
                         " many counters, and it dies at 0]";
            tail << "\n";
        }
        tail << kAttackTargetScopeFacts;
    }
    tail << kAttackersTurnFacts;
    tail << kPlanFirstLead //#W70-BL (E2)
         << "on a line of its own ATTACK: followed by the attackers you send,"
            " comma-separated (e.g. \"ATTACK: A1, A3\"), or \"ATTACK: none\" to"
            " attack with nobody this turn";
    //#W64-AI (F4): the suffix is stated ONLY when a target row exists, so a
    //window with no planeswalker on their board reads exactly as it did.
    if (!req.attackTargets.empty())
        tail << "; to send one at a planeswalker/battle instead of at them, write"
                " that attacker as A#>W# (e.g. \"ATTACK: A1>W1, A3\" sends A1 at W1"
                " and A3 at them); an attacker written without a >W# attacks them";
    tail << ". Write nothing else."; //#W70-BL (E2)
    if (mAttackReaskTurn != observer->turn)
        mAttackReaskLine.clear();
    const bool attackReasked = (mAttackReaskTurn == observer->turn && !mAttackReaskLine.empty());
    if (attackReasked)
        tail << "\n" << mAttackReaskLine;
    mLogWindowKind = kAskWindowCombat; //#W57-H (D43): combat keeps the whole log
    const string attackTail = tail.str(); //#W68-BA (J6): one copy, reused by the re-ask
    string userMsg = assemblePrompt(attackTail);

    string content;
    //#W71-BS (F5): the legal declaration is at most one line per attacker.
    setAnswerFloorForSeam("attackers", (long) attackers.size(), kPutSlotAnswerBytes);
    if (pollCompletionRetry(userMsg, content, "attackers") == kChoicePending)
        return 1; //decision in flight; nothing declared yet, re-poll next tick

    //Plan split BEFORE the attacker-set parse: numbers (and words like
    //"hold") in the plan prose must not read as attack declarations.
    string decisionPart = consumePlan(content, "ATTACK:");
    vector<bool> send;
    vector<string> attackerNames;
    attackerNames.reserve(attackers.size());
    for (size_t j = 0; j < attackers.size(); j++)
        attackerNames.push_back(attackers[j]->name);

    //ANSWER-FIRST primary (wave-23 ITEM B). The shipped contract puts the
    //declaration on the FIRST line; consumePlan's last-wins otherwise let a
    //LATER same-label CoT combat-math line hijack it - deck109 vs62 s21, where
    //the clean line-1 "ATTACK: A1, A2, A3, A4" was overridden by a line-179 CoT
    //"Attack: Deal 1, Take 5. Net -4 life. Opponent -1 life." whose bare prose
    //numbers (1, -4) parsed to a bogus A1+A4 subset. This is stale-echo family B
    //where the decoy shares the answer's OWN label, so the expectedLabel filter
    //cannot separate them: take the FIRST line-leading ATTACK: line that parses
    //to a usable declaration. (Prose-salvage was NOT the cause - it fires only
    //when result<0; here the mis-parse produced result>=1.)
    int result = -1;
    //#W64-AI (F4): the exact line the declaration was read from - the >W#
    //target suffixes are parsed from THAT line and no other, so a target can
    //never be picked up from a discarded CoT line.
    string takenText;
    //#W65-AP (R2, wave-65 codex review finding 2 - HIGH): the driver's own
    //last-CLEAN-wins walk lived here and never obeyed the first-wins rule the
    //protocol states and #W65-AO wrote into the CHOICE seam, so
    //`ATTACK: A1 / ... / ATTACK: none` executed `none`. The choice is now
    //gptAttackLineFromReply, which runs the shared gptSelectAnswerIndex - one
    //rule, one function, three seams.
    {
        int takenLine = -1;
        result = gptAttackLineFromReply(content, attackers.size(), attackerNames,
                                        send, &takenText, &takenLine, &mLastParseNote,
                                        &mLastIgnoredAnswerLine); //#W66-AR (H2c)
        if (takenLine == 0)
            mAnswerReplacedFalse = true; //what executed is the first coded line
    }
    //Fallback for a reply that named attackers WITHOUT a line-leading ATTACK:
    //label (consumePlan's short-bare-answer path) - unchanged behavior.
    if (result < 0 && !content.empty())
    {
        result = parseAttackerSet(decisionPart, attackers.size(), send, &attackerNames);
        if (result >= 0)
            takenText = decisionPart; //#W64-AI (F4)
    }

    //Repeat-loop salvage: an unparsed (non-empty) reply may be a decode spiral
    //that stated a valid ATTACK: line before degenerating - recover the last
    //well-formed one through the same validator (mirrors salvageLoopedChoice).
    if (result < 0 && !content.empty())
    {
        int sal = salvageLoopedSubset(content, "ATTACK:", attackers.size(), attackerNames, send);
        if (sal >= 1)
        {
            DebugTrace("AIPlayerGPT: salvaged looped ATTACK (" << sal << ")");
            result = sal;
        }
    }

    const char * attackSource = NULL;

    //#W71-BO (R3, wave-70 census): the truncation re-ask is DELETED at this seam
    //with every other. 0 truncations in 2,119 decisions - `reply_truncated` was
    //never once written - and the answer ceiling is now fitted to this corpus's
    //own bytes (gptSeamMaxTokens), so a cut before the declaration is no longer a
    //shape the engine has to recover from.
    if (result < 0)
    {
        //Unusable reply: the heuristic declares this turn's attack instead.
        writeTransLog("attackers", userMsg, content, result, (int) attackers.size(),
                      "", content.empty() ? noAnswerClass() : unparsedReplyClass(content), &shownLines);
        noticeFallback("model reply failed - the heuristic attacks", 5.0f);
        mAttacksDoneTurn = observer->turn;
        return AIPlayerBaka::chooseAttackers();
    }

    //#W70-BL (E5): the attackers seam's prose-reversal re-ask is DELETED with
    //the tolerance it served. It read the sentences around the ATTACK: line for a
    //verdict and quoted them back, which told the model prose in the reply is
    //read. The reply is two labelled lines; there is nowhere for a reversal to be
    //written, and the only contradiction left to price is between them.
    //#W64-AI (F4): read the >W# suffixes off the line the declaration came
    //from. A malformed or out-of-range suffix binds nothing and the attacker
    //simply attacks the player - the declaration is never lost to it.
    vector<int> attackTargetPick;
    if (!req.attackTargets.empty() && !takenText.empty())
    {
        int bound = parseAttackerTargets(takenText, attackers.size(),
                                         req.attackTargets.size(), attackTargetPick);
        if (bound)
            appendParseNote(&mLastParseNote, "attack_walker_target");
    }

    //Answer through the contract: the manager applies the declaration.
    //Attack-COST creatures still need the policy to pre-pay mana (see the
    //contract header) before the apply.
    DecisionAction act;
    string declared;
    for (size_t j = 0; j < attackers.size(); j++)
    {
        if (!send[j])
            continue;
        //#W64-AK (R1): the guard is the DESTINATION's predicate, not the
        //player's. `attackers[j]->canAttack()` deleted every walker-only
        //attacker here even after the contract offered it and the model named
        //it. A row that may only attack a planeswalker needs a target: when the
        //reply named one it is used; when their board has exactly ONE such
        //permanent that is the only legal reading of "attack with this", so it
        //is bound rather than dropped; with several, the seat cannot choose for
        //the model and the attacker is left home with the omission RECORDED.
        MTGCardInstance * pwTarget = NULL;
        if (j < attackTargetPick.size() && attackTargetPick[j] >= 1
            && attackTargetPick[j] <= (int) req.attackTargets.size())
            pwTarget = req.attackTargets[attackTargetPick[j] - 1];
        if (!W64AK_MAY_PLAYER(j))
        {
            if (!W64AK_MAY_TARGET(j))
                continue;
            if (!pwTarget)
            {
                if (req.attackTargets.size() == 1)
                    pwTarget = req.attackTargets[0];
                else
                {
                    appendParseNote(&mLastParseNote, "attack_walker_target_missing");
                    continue;
                }
            }
        }
        if (attackers[j]->attackCost)
        {
            MTGAbility * a = observer->mLayers->actionLayer()->getAbility(MTGAbility::ATTACK_COST);
            doAbility(a, attackers[j]);
            observer->cardClick(attackers[j], MTGAbility::ATTACK_COST);
        }
        act.attackers.push_back(attackers[j]);
        //#W64-AI (F4): the target this row named, parallel to act.attackers.
        //0 (or an absent/dropped suffix) is the player, which is every reply
        //this seat wrote before this wave. #W64-AK (R1) resolved it above,
        //because a walker-only row's legality depends on it.
        act.attackerTargets.push_back(pwTarget);
        declared += (declared.empty() ? "" : ", ") + attackers[j]->name;
        if (pwTarget)
            declared += " -> " + pwTarget->getDisplayName();
    }
    DecisionManager::applyDeclareAttackers(req, act);
    writeTransLog("attackers", userMsg, content, result, (int) attackers.size(),
                  declared.empty() ? string("no attackers") : declared, NULL, &shownLines, attackSource);
    narrateDecision(declared.empty() ? string("You declared no attackers this turn")
                                     : ("You declared attackers: " + declared));
    mAttacksDoneTurn = observer->turn;
    DebugTrace("AIPlayerGPT: declared attack (" << result << " of " << attackers.size() << ") in one reply");
    return 1;
}

#undef W64AK_MAY_PLAYER

#undef W64AK_MAY_TARGET


//#W53-M (D1): how many of a parsed block assignment's pairings the ENGINE
//already said this seat may make - counted against the same legal set the B#
//lines were rendered from (DecisionRequest::legalPerBlocker, indexed here).
//The blockers seam uses it to answer one question: does an illegal
//one-blocker-many-attackers reply still contain an assignment worth
//declaring? When it does, the duplicate costs the duplicate and the reply is
//declared; only a reply with NOTHING left is worth re-asking. canBlock()
//remains the gate on the declaration itself further down - this counts, it
//does not admit.
static int countLegalAssignments(const vector<int>& pick, size_t nAttackers,
                                 const vector<vector<int> >& legalPerBlocker)
{
    int n = 0;
    for (size_t i = 0; i < pick.size() && i < legalPerBlocker.size(); i++)
    {
        if (pick[i] < 1 || pick[i] > (int) nAttackers)
            continue;
        for (size_t k = 0; k < legalPerBlocker[i].size(); k++)
            if (legalPerBlocker[i][k] == pick[i] - 1)
            {
                n++;
                break;
            }
    }
    return n;
}


//Scan a bundled-blocking reply for "B<i>:A<j>" / "B<i>:none" pairs (any of
//": - > =" or spaces as separator). Unmentioned or malformed blockers stay
//out of combat. Returns how many well-formed pairs were found - zero means
//the reply is unusable and the caller falls back to the heuristic.
//(history: comment-archaeology.md AIPlayerGPTSeams-parseBlockAssignments-2275)
static int parseBlockAssignments(const string& content, size_t nBlockers, size_t nAttackers, vector<int>& out,
                                 const vector<string> * blockerNames,
                                 const vector<string> * attackerNames,
                                 const vector<vector<int> > * legalPerBlocker,
                                 int * dropped,
                                 bool * gangConflict,
                                 bool * blockerInAttackerSlot)
{
    out.assign(nBlockers, 0); //0 = no block; else attacker number
    int pairs = 0;
    for (size_t i = 0; i < content.size(); i++)
    {
        if (content[i] != 'B' && content[i] != 'b')
            continue;
        size_t j = i + 1;
        if (j >= content.size() || !isdigit(content[j]))
            continue;
        int b = readDigits(content, j); //#W54-M (L7)
        //#W48 (D2): R8's B-rows already print RANGE labels ("B4-B9"), and the
        //grammar refused them - "B4-B9:A2" parsed B4, met a 'B' where it wanted
        //an attacker, and dropped the whole pair. A hyphen followed by another
        //B-index is the range form; anything else is still the ordinary
        //"B4-A2" separator run below.
        //#W82-A (L8, audit-2026-09): the range grammar accepted ONLY the glued
        //both-labels form `B4-B9`. `B4 - B9:A2` dropped B4 (and stamped
        //`blockerInAttackerSlot`); `B4-9:A2` dropped the whole pair - while the
        //ATTACKERS grammar accepts `A4-9` and `A4 - A9`, and `kBlockerRangeNote`
        //teaches the shape on the very prompt this reads. The ruling is that the
        //parser reads the answer wherever it unambiguously is, so spaces around
        //the hyphen and a bare high index are read the same as the glued form.
        int bHi = b;
        {
            size_t p0 = j;
            while (p0 < content.size() && content[p0] == ' ')
                p0++;
            if (p0 < content.size() && content[p0] == '-')
            {
                size_t p1 = p0 + 1;
                while (p1 < content.size() && content[p1] == ' ')
                    p1++;
                if (p1 < content.size() && (content[p1] == 'B' || content[p1] == 'b'))
                    p1++;
                if (p1 < content.size() && isdigit(content[p1]))
                {
                    size_t k = p1;
                    int m = readDigits(content, k); //#W54-M (L7)
                    if (m > b)
                    {
                        bHi = m;
                        j = k;
                    }
                }
            }
        }
        while (j < content.size() && (content[j] == ':' || content[j] == '-' || content[j] == '>'
                                      || content[j] == '=' || content[j] == ' '))
            j++;
        //#W82-A (L8): `B1 blocks A1` is the third shape the ruling covers. The
        //separator run above consumes punctuation only, so it stopped on the 'b'
        //of "blocks", read no attacker and dropped a pair whose intent is plain;
        //the NAME pass could not rescue it either (`significantWords("B1")` is
        //empty at the four-character floor). The verb is a separator.
        {
            static const char * kBlockVerbs[3] = { "blocks", "blocking", "block" };
            for (int v = 0; v < 3; v++)
            {
                const size_t vl = strlen(kBlockVerbs[v]);
                if (j + vl <= content.size()
                    && strncasecmp(content.c_str() + j, kBlockVerbs[v], vl) == 0)
                {
                    j += vl;
                    while (j < content.size() && (content[j] == ':' || content[j] == '-'
                                                  || content[j] == '>' || content[j] == '='
                                                  || content[j] == ' '))
                        j++;
                    break;
                }
            }
        }
        int a = -1; //-1 malformed; 0 none; else attacker number
        if (j < content.size() && (content[j] == 'A' || content[j] == 'a') && j + 1 < content.size()
            && isdigit(content[j + 1]))
        {
            size_t k = j + 1;
            a = readDigits(content, k); //#W54-M (L7)
        }
        else if (content.compare(j, 4, "none") == 0 || content.compare(j, 4, "None") == 0
                 || content.compare(j, 4, "NONE") == 0)
            a = 0;
        if (a < 0 || b < 1 || b > (int) nBlockers || a > (int) nAttackers)
        {
            //A well-formed "B#:" whose numbers name nobody on this board. It
            //asked for something and got nothing - count it (wave-34 #1b(B)).
            if (dropped && b >= 1)
                (*dropped)++;
            //#W53-M (D19): name the shape when the attacker slot holds another
            //B-handle. Every range form ("B4-B9:A2", "B4 - B9:A2", "B4-9:A2")
            //consumes its high index above and never reaches here, so this cannot
            //fire on one (#W82-A L8).
            if (blockerInAttackerSlot && a < 0 && b >= 1 && j + 1 < content.size()
                && (content[j] == 'B' || content[j] == 'b') && isdigit(content[j + 1]))
                *blockerInAttackerSlot = true;
            continue;
        }
        if (bHi > (int) nBlockers)
            bHi = (int) nBlockers;
        for (int bb = b; bb <= bHi; bb++)
        {
            if (out[bb - 1] != 0)
            {
                if (dropped)
                    (*dropped)++;
                if (gangConflict && a >= 1 && out[bb - 1] != a)
                    *gangConflict = true; //same blocker, DIFFERENT attacker: the
                                          //illegal one-blocker-many-attackers shape
                continue; //first assignment wins: a creature blocks at most one
                          //attacker, so ignore a later "B1:A3" after "B1:A1"
                          //(the model occasionally double-assigns one blocker).
            }
            out[bb - 1] = a;
            pairs++;
        }
    }

    //Name->label reconcile pass (opt-in via the name tables). Fills only
    //blockers the coded scan left unassigned; never overrides a B#:A# pair.
    if (blockerNames && attackerNames)
    {
        size_t start = 0;
        for (size_t s = 0; s <= content.size(); s++)
        {
            //';' joins name-form pairs as readily as ',' does (deck158 vs152
            //s35: "Orc army: Sigarda, Champion of Light; Dunland Crebain:
            //Moonrage Brute") - without it the second pair never reaches a
            //segment of its own. WAVE-33 N-158n.
            if (s != content.size() && content[s] != ',' && content[s] != ';' && content[s] != '\n')
                continue;
            string seg = stripAnswerLabelPrefix(content.substr(start, s - start));
            start = s + 1;
            //Find the blocker/attacker separator: ':' or the word "blocks".
            //A leading colon whose left half carries NO name words (a stray
            //"Answer:"/"Declaring:" prefix the label strip does not know)
            //re-splits at the NEXT separator rather than dropping the pair.
            string low;
            for (size_t k = 0; k < seg.size(); k++)
                low += (char) tolower((unsigned char) seg[k]);
            size_t searchFrom = 0;
            while (searchFrom <= seg.size())
            {
                size_t sep = string::npos, sepLen = 0;
                size_t colon = seg.find(':', searchFrom);
                size_t blk = low.find(" blocks ", searchFrom);
                if (colon != string::npos) { sep = colon; sepLen = 1; }
                if (blk != string::npos && (sep == string::npos || blk < sep)) { sep = blk; sepLen = 8; }
                if (sep == string::npos)
                    break; //no blocker:attacker structure left in this segment
                string leftSeg = seg.substr(0, sep), rightSeg = seg.substr(sep + sepLen);
                vector<string> leftWords, rightWords;
                significantWords(leftSeg, leftWords);
                significantWords(rightSeg, rightWords);
                if (leftWords.empty())
                {
                    searchFrom = sep + sepLen; //label-ish prefix: try the next separator
                    continue;
                }
                int bMatch = uniqueNameMatch(leftWords, *blockerNames, nBlockers, NULL, nameOrdinal(leftSeg));
                if (bMatch < 0)
                    break; //no unique blocker for this segment
                const vector<int> * allowed = (legalPerBlocker && (size_t) bMatch < legalPerBlocker->size())
                                              ? &(*legalPerBlocker)[bMatch] : NULL;
                //"#N" on the attacker half ("Saproling (1/1) #1") disambiguates
                //two identically-named attackers the plain match would drop.
                int aMatch = uniqueNameMatch(rightWords, *attackerNames, nAttackers, allowed, nameOrdinal(rightSeg));
                //LEGALITY IS THE VALIDATOR'S JOB, NOT THE PARSER'S (wave-33).
                //Restricting the attacker match to the blocker's legal set is a
                //DISAMBIGUATOR, so it is tried first; when it fails, retry over
                //the whole attacker list so the model's actual intent is
                //RESOLVED and then PRUNED at the apply site's canBlock gate
                //(and counted in the translog), exactly as a coded "B1:A5"
                //naming an illegal attacker already is. Silently dropping it
                //here made an illegal intent indistinguishable from an
                //unparseable reply (deck139 s21, deck158 s35).
                if (aMatch < 0 && allowed)
                    aMatch = uniqueNameMatch(rightWords, *attackerNames, nAttackers, NULL, nameOrdinal(rightSeg));
                if (out[bMatch] != 0)
                {
                    //A named pair that resolves to a blocker already spoken
                    //for is the gang-of-one shape: "Illuna blocks A, Illuna
                    //blocks B, ..." - first-wins is correct, and the three
                    //discarded intents were previously invisible. When the
                    //discarded intent names a DIFFERENT attacker, flag the
                    //illegal multi-block so the seam can re-ask (W36 item 1).
                    if (dropped)
                        (*dropped)++;
                    if (gangConflict && aMatch >= 0 && out[bMatch] != aMatch + 1)
                        *gangConflict = true;
                    break;
                }
                if (aMatch < 0)
                {
                    if (dropped)
                        (*dropped)++;
                    break; //ambiguous / unmatched attacker -> drop this one only
                }
                out[bMatch] = aMatch + 1;
                pairs++;
                break;
            }
        }
    }
    return pairs;
}


//#W65-AP (R2): the BLOCKS driver's line choice, as one pure function, on the
//shared rule. Same shape as gptAttackLineFromReply: the coded BLOCKS: lines of
//the post-</think> reply in order, plus #W62-Z (D9)'s prose restatement last.
//The driver only overrides consumePlan's answer when the reply carried MORE
//THAN ONE candidate (the shipped guard, unchanged) - with a single candidate
//the two agree by construction. Returns true and fills `out` when a candidate
//was selected; `firstLine` reports whether it was the reply's first.
static bool gptBlocksLineFromReply(const string& content, const vector<string>& bNames,
                                   const vector<string>& aNames,
                                   string& out, bool * firstLine, string * notes,
                                   string * ignoredLine = NULL) //#W66-AR (H2c)
{
    if (content.empty())
        return false;
    string stripped = content;
    size_t te = stripped.rfind("</think>");
    if (te != string::npos)
        stripped = stripped.substr(te + 8);
    vector<string> lines, prevLines;
    vector<vector<string> > windows; //#W66-AR (H2b)
    collectLabeledLines(stripped, "BLOCKS:", lines, &prevLines, &windows);
    (void) prevLines;
    (void) windows;
    if (lines.size() < 2)
        return false; //one candidate (or none): consumePlan's answer stands
    //Every collected line is a candidate - as it was: this seam has never
    //gated a BLOCKS line on parseability (an unassignable line is a legal
    //"block nothing" and parseBlockAssignments answers 0 pairs for it), and
    //adding a gate here would change which reply is unusable, not which of
    //several is the answer.
    vector<bool> usable(lines.size(), true), clean(lines.size(), false);
    for (size_t i = 0; i < lines.size(); i++)
        clean[i] = combatLineIsClean(lines[i], &bNames, &aNames);
    const int idx = gptSelectAnswerIndex(usable, clean);
    if (idx < 0)
        return false;
    if (ignoredLine && (size_t) idx + 1 < lines.size()) //#W66-AR (H2c)
        *ignoredLine = string("BLOCKS:") + lines[lines.size() - 1];
    out = lines[(size_t) idx];
    if (firstLine)
        *firstLine = (idx == 0);
    if (notes && (size_t) idx + 1 < lines.size())
        appendParseNote(notes, "extra_answer_line"); //#W70-BM (E2)
    return true;
}


int AIPlayerGPT::chooseBlockers()
{
    //Only drive the declare-blockers step; anywhere else, stay out of the way.
    if (mEndpoint.empty() || observer->getCurrentGamePhase() != MTG_PHASE_COMBATBLOCKERS)
        return AIPlayerBaka::chooseBlockers();
    if (observer->currentPlayer == this) //never block on my own turn (Baka guard)
        return 0;
    if (mBlocksDoneTurn == observer->turn)
        return 1; //this combat's assignment was already declared

    //The engine's request carries the attackers and each available
    //blocker's legal set (contract c1).
    DecisionRequest req;
    if (!DecisionManager::buildDeclareBlockers(this, req))
    {
        //Nothing to declare (the builder found no answerable shape): the
        //declaration is vacuously satisfied. Marking it done keeps
        //blockersDeclarationDue honest - a due-forever verdict here would
        //hold every queued phase-advance (NextGamePhase's item-6 guard).
        mBlocksDoneTurn = observer->turn;
        return 1;
    }
    vector<MTGCardInstance *> & attackers = req.attackers;
    vector<MTGCardInstance *> & blockers = req.blockers;
    vector<vector<MTGCardInstance *> > & legal = req.legalPerBlocker;
    //#W49-R (D13): same-name cards gathered adjacently, rank ascending, on BOTH
    //lists before any row is built. `legal` is parallel to `blockers` and moves
    //with it; the A#/B# labels, the reply parser's index tables and the apply
    //path all read these same vectors, so a label still names its card.
    {
        vector<string> gn, gh;
        for (size_t j = 0; j < attackers.size(); j++)
        {
            gn.push_back(attackers[j]->name);
            gh.push_back(instanceHandle(attackers[j]));
        }
        vector<size_t> ord;
        groupCombatCandidates(gn, gh, ord);
        vector<MTGCardInstance *> perm;
        for (size_t j = 0; j < ord.size(); j++)
            perm.push_back(attackers[ord[j]]);
        attackers.swap(perm);
    }
    {
        vector<string> gn, gh;
        for (size_t i = 0; i < blockers.size(); i++)
        {
            gn.push_back(blockers[i]->name);
            gh.push_back(instanceHandle(blockers[i]));
        }
        vector<size_t> ord;
        groupCombatCandidates(gn, gh, ord);
        vector<MTGCardInstance *> perm;
        vector<vector<MTGCardInstance *> > permLegal;
        for (size_t i = 0; i < ord.size(); i++)
        {
            perm.push_back(blockers[ord[i]]);
            permLegal.push_back(legal[ord[i]]);
        }
        blockers.swap(perm);
        legal.swap(permLegal);
    }

    //ONE bundled decision for the whole combat. Sequential per-blocker asks
    //cannot coordinate (each one's local best answer piled every wall onto
    //the same attacker), identical attacker names were indistinguishable,
    //and N blockers cost N round trips. Labels disambiguate; the model
    //answers with the full assignment map in a single reply.
    if (mSystemPrompt.empty())
        buildSystemPrompt();
    std::ostringstream tail;
    tail << "Combat: declare blockers for this whole combat in ONE decision.\n";
    //The lethal arithmetic, done FOR the model (wave-6/7: reflexive
    //blocking at high life and missed must-blocks at low life are the same
    //miscount - 3 guises across 5 seats; routed to representation, not
    //core prose). Phrased conditionally: an attacker may be hitting a
    //planeswalker, so the sum is an upper bound on face damage.
    //N-105b: the swing is PARTITIONED by whether each attacker's damage to a
    //player is REPLACED. Infect/poisondamager damage becomes poison counters and
    //never touches life; toxic damage hits life AND adds counters; wither only
    //replaces damage dealt to CREATURES, so it is ordinary damage on this line
    //(its blocker consequence rides the attacker line instead).
    {
        int lifeIncoming = 0, poisonIncoming = 0;
        for (size_t j = 0; j < attackers.size(); j++)
        {
            int p = attackers[j]->power > 0 ? attackers[j]->power : 0;
            //#W82-A (L9, audit-2026-09): DOUBLE STRIKE assigns combat damage in
            //BOTH steps - CR 702.4b: "the phase gets a second combat damage step
            //... as well as the remaining attackers and blockers that currently
            //have double strike." The ATTACKERS seam has doubled its row power
            //since #W64; this seam - the one that decides the BLOCK - did not, so
            //an unblocked double striker was priced at half the life it takes.
            if (attackers[j]->basicAbilities[Constants::DOUBLESTRIKE])
                p *= 2;
            if (sourceDealsPoisonInsteadOfDamage(attackers[j]))
                poisonIncoming += p;
            else
            {
                lifeIncoming += p;
                if (p > 0)
                    poisonIncoming += attackers[j]->getToxicity();
            }
        }
        //#W80-DF (U14): the compulsory draw-step charge the SAME screen's DRAW
        //FORECAST states. Built from the forecast's own two scans, so the two
        //lines cannot disagree; this seam is the opponent's combat, so the
        //seat's next draw step is entirely ahead of it (no resolved-in-step
        //subtraction applies) and every card of it is undeclinable.
        int w80CycleLoss = 0;
        string w80CycleSrc;
        {
            //#W81-DL (V5): the same walk the crack-back verdict now folds, read
            //from ONE place so the two screens cannot publish two totals.
            int w80Cards = 0, w80OptionalCards = 0;
            vector<string> w80Theirs;
            w80CycleLoss = w81CompulsoryDrawStepLoss(this, opponent(), &w80Cards,
                                                     &w80OptionalCards, &w80Theirs);
            if (w80CycleLoss > 0)
            {
                std::ostringstream sn;
                sn << "your next draw step (" << w80Cards << " card"
                   << (w80Cards == 1 ? "" : "s") << " into their ";
                for (size_t ni = 0; ni < w80Theirs.size(); ni++)
                    sn << (ni ? ", " : "") << w80Theirs[ni];
                sn << " - see the DRAW FORECAST line above)";
                if (w80OptionalCards > 0) //#W80-DH (F9)
                    sn << ", plus up to " << w80OptionalCards << " more if you CHOOSE"
                          " to take the optional draw"
                       << (w80OptionalCards == 1 ? "" : "s")
                       << " your own permanent offers - that is your decision and is"
                          " NOT counted in the figure above";
                w80CycleSrc = sn.str();
            }
        }
        //#W54-E (D21): the opponent's life is what makes "ahead" a fact.
        //#W82-P6: the board frame's INCOMING THIS COMBAT line is latched for
        //this combat; when its figures are this seam's figures the header
        //prints once and this line carries only what the frame does not.
        const bool figureOnFrame = observer && mIncomingCombatTurn == observer->turn
                                   && mIncomingCombatAttackers == (int) attackers.size()
                                   && mIncomingCombatDamage == lifeIncoming;
        tail << combatDamageForecast(life, poisonCount, lifeIncoming, poisonIncoming,
                                     opponent() ? opponent()->life : life,
                                     lifeLoopProvenWin(opponent()), //#W76-CO (Q3 a)
                                     w80CycleLoss, w80CycleSrc, //#W80-DF (U14)
                                     figureOnFrame); //#W82-P6
    }
    //#W57-B (D22): and the OTHER total this window turns on - what the seat
    //gains, and (under a converter of its own) takes off them, simply for
    //declaring blocks. Read off the same blockTriggeredLifeFor() the per-row
    //`{blocking trigger, this combat:}` clause reads, so the header and the
    //rows beneath it can never disagree.
    {
        int triggered = 0, totalGain = 0, firstSure = 0, firstMay = 0;
        bool uniform = true;
        for (size_t i = 0; i < blockers.size(); i++)
        {
            int sure = 0, may = 0;
            blockTriggeredLifeFor(blockers[i], sure, may);
            if (sure <= 0 && may <= 0)
                continue;
            if (triggered == 0)
            {
                firstSure = sure;
                firstMay = may;
            }
            else if (sure != firstSure || may != firstMay)
                uniform = false;
            triggered++;
            totalGain += sure + may;
        }
        const string bt = blockingTriggerTotalLine(triggered, firstSure, firstMay, uniform,
                                                   totalGain,
                                                   playerHasLifeToDamageConverter(this),
                                                   opponent() ? opponent()->life : 0);
        if (!bt.empty())
            tail << bt << "\n";
    }
    //Capture each presented combat option line (attacker context + blocker
    //options WITH their trade annotations) for the translog: combat records
    //logged options_text EMPTY, which blocked wave-19's validation of the
    //trade-outcome annotations. Pure observability - the prompt text below is
    //byte-identical to before, just also accumulated per line.
    vector<string> shownLines;
    //W39-UNREACHABLE (wave-39 ledger #6, validator §D): the B-lines say which
    //attackers each blocker MAY block and NOTHING says why the others are
    //absent - the attacker line carried a POSITIVE "[flying]" tag on a
    //different line and the model read past it, answering "BLOCKS: B1:A1" with
    //B1 (no reach) legal only on A2/A3 and A1 an Ornithopter with flying (the
    //corpus's one all_assignments_illegal). Put the NEGATION on the attacker
    //line, derived from the ENGINE's own legal sets so it cannot over-claim.
    vector<int> canBlockCount(attackers.size(), 0);
    for (size_t i = 0; i < blockers.size(); i++)
        for (size_t j = 0; j < legal[i].size(); j++)
            for (size_t k = 0; k < attackers.size(); k++)
                if (attackers[k] == legal[i][j])
                    canBlockCount[k]++;
    //#W46-5: set when any attacker line carried a gang price, so the scope note
    //that explains the clause is stated once and only when it applies.
    bool anyGangPriced = false;
    tail << "Attackers:\n";
    vector<string> aRowName, aRowHandle, aRowRest; //#W48 (D2): A-row collapse parts
    //#W63-AB (E1): holding both halves is not the same as being able to CLOSE
    //them (#W62-AA R6's rule) - a price tail that says "chains without limit"
    //must not print over a board where the chain cannot run.
    const bool oppLoopClosed = lifeLoopProvenWin(opponent());
    for (size_t j = 0; j < attackers.size(); j++)
    {
        std::ostringstream ln;
        //#W48 (D2): the row is built WITHOUT its label/name/handle prefix so the
        //emitter below can collapse a run of rows that agree in every rendered
        //fact - the same three tests joinZoneEntries and R8's B-rows apply.
        ln << " (" << attackers[j]->power << "/" << attackers[j]->toughness << ")"
           << markedDamageTag(attackers[j]->toughness, attackers[j]->life); //#W82-EC (H3)
        //POWER is the damage number, not toughness. The model misread a
        //Saproling "(2/4)" as dealing 4 (deck35 wave-18 G1); state the damage
        //explicitly at the line that decides.
        {
            //#W82-A (L9): CR 702.4b - a double striker assigns its power in the
            //first-strike step AND again in the ordinary one, so the number the
            //block is decided against is twice the printed power. The (P/T) above
            //is the card's characteristic and is untouched.
            const int rowP = attackers[j]->power > 0 ? attackers[j]->power : 0;
            const bool dblRow = attackers[j]->basicAbilities[Constants::DOUBLESTRIKE] != 0;
            ln << " deals " << (dblRow ? rowP * 2 : rowP);
            if (dblRow && rowP > 0)
                ln << " (double strike: " << rowP << " in each of the two combat"
                      " damage steps)";
        }
        string kw = keywordList(attackers[j]);
        if (!kw.empty())
            ln << " [" << kw << "]";
        //N-105a part (d): spell out what infect / toxic / wither damage actually
        //DOES, at the line that decides the block. The engine keyword names for
        //this family are opaque, and the seat's own reasoning showed it pricing
        //an infect swing entirely on the life track.
        ln << attackerPoisonNote(attackers[j]);
        //#W82-EC (H4): the lifelink gain, both branches and the owner, on the
        //line the block is decided against.
        if (attackers[j]->basicAbilities[Constants::LIFELINK])
            ln << attackerLifelinkAttackLineTag(
                      attackers[j]->power > 0 ? attackers[j]->power : 0,
                      attackers[j]->basicAbilities[Constants::DOUBLESTRIKE] != 0);
        //Punisher rider: an attacker whose text does something WHEN BLOCKED
        //or WHEN DEALT DAMAGE (sacrifice permanents, damage you, pump
        //itself) is a trap the bare name hides - surface the text at the
        //line that decides. "block" alone missed the damage-trigger class:
        //Phyrexian Obliterator's "deals damage to" rider stayed hidden and
        //deck109 blocked into it (wave-8).
        string txt = attackers[j]->text;
        //W41-4: an attacking mutate pile is ONE creature with every pile card's
        //abilities, so the rider that decides the block may sit on an UNDER card.
        //Gate on the whole pile's text and render the pile's combined, per-card
        //attributed text.
        {
            std::vector<MTGCardInstance *> pile;
            collectMutatePile(attackers[j], pile);
            for (size_t pj = 0; pj < pile.size(); pj++)
                if (pile[pj] && pile[pj] != attackers[j])
                    txt += " " + pile[pj]->text;
        }
        for (size_t ti = 0; ti < txt.size(); ti++)
            txt[ti] = (char) tolower((unsigned char) txt[ti]);
        if (txt.find("block") != string::npos
            || txt.find("deals damage") != string::npos
            || txt.find("dealt damage") != string::npos
            || txt.find("deals combat damage") != string::npos)
            ln << " {text: " << pileAwareCardText(attackers[j], 160) << "}";
        //W39-UNREACHABLE: the restriction, first and negative. The REASON is
        //only stated when it is PROVABLE from the data on hand - flying
        //explains the pattern exactly when every blocker that may block this
        //attacker has flying or reach and every blocker that may not, has
        //neither. Anything else gets the bare (and still true) negation.
        if (!blockers.empty() && canBlockCount[j] < (int) blockers.size())
        {
            bool flyingExplains = attackers[j]->has(Constants::FLYING);
            for (size_t i = 0; flyingExplains && i < blockers.size(); i++)
            {
                bool canB = false;
                for (size_t g = 0; g < legal[i].size() && !canB; g++)
                    canB = (legal[i][g] == attackers[j]);
                bool air = blockers[i]->has(Constants::FLYING)
                           || blockers[i]->has(Constants::REACH);
                if (canB != air)
                    flyingExplains = false;
            }
            ln << unreachableAttackerTag(canBlockCount[j], (int) blockers.size(), flyingExplains);
        }
        //W43-1: the declaration-set restriction, on the line that decides it.
        //A menace attacker only reaches the B-lines at all when the engine's
        //legal sets say a legal block EXISTS (canBlock now refuses an
        //assignment that could never be completed), so this tag never appears
        //on a body the seat cannot block - it says what a legal answer for it
        //has to look like.
        ln << blockCountRequirementTag(attackers[j]->minBlockersRequired());
        //#W46-5 (wave-45 ledger, MED). The mirror of the attackers window's
        //#W45-2 clause, in the seat that actually fields the group. Every B-line
        //parenthetical below is a ONE-blocker forecast, so a board whose every
        //row reads "(your blocker dies, attacker lives)" says nothing about the
        //group that kills the attacker outright - and the pilot at deck123 vs152
        //seq 47 re-derived it by hand, ~250 words, against the trust note's own
        //"never re-derive it".
        //TRUST DOCTRINE, gate-for-gate identical to the attackers side: the
        //number is claimed ONLY where a sum of powers is the whole fight. First
        //or double strike on the attacker removes blockers before their damage
        //lands; deathtouch/wither/infect on a blocker kills on a threshold this
        //sum does not model; indestructible and persist/undying mean the sum
        //kills nothing lasting; a prevention shield means the damage never
        //arrives; a when-blocked self-pump changes the toughness being priced.
        //A wrong N is strictly worse than no N.
        {
            int bbSelfP = 0, bbSelfT = 0;
            //#W64-AG (F7): the same widening as the attackers window - first
            //strike and the blockers' deathtouch are computed, not silenced.
            bool gangOk = attackers[j]->toughness > 0
                          && !attackers[j]->basicAbilities[Constants::INDESTRUCTIBLE]
                          && !attackers[j]->basicAbilities[Constants::PERSIST]
                          && !attackers[j]->basicAbilities[Constants::UNDYING]
                          && becomesBlockedSelfPump(attackers[j]->text, bbSelfP, bbSelfT) == 0;
            //The candidate set is the ENGINE's own legal-per-blocker map, the
            //same source the B-lines and canBlockCount read - so the group can
            //never contain a body that may not legally block this attacker.
            vector<GangBlockerStat> gangStats; //#W64-AG (F7)
            for (size_t i = 0; i < blockers.size(); i++)
            {
                bool canB = false;
                for (size_t g = 0; g < legal[i].size() && !canB; g++)
                    canB = (legal[i][g] == attackers[j]);
                if (!canB)
                    continue;
                if (combatPreventionKind(blockers[i], attackers[j]) != kPreventNone)
                    gangOk = false;
                GangBlockerStat gs; //#W64-AG (F7)
                gs.power = blockers[i]->power > 0 ? blockers[i]->power : 0;
                gs.toughness = blockers[i]->toughness;
                gs.deathtouch = blockers[i]->basicAbilities[Constants::DEATHTOUCH] != 0;
                gs.firstStrike = blockers[i]->basicAbilities[Constants::FIRSTSTRIKE] != 0
                                 || blockers[i]->basicAbilities[Constants::DOUBLESTRIKE] != 0;
                gangStats.push_back(gs);
            }
            if (gangOk && gangStats.size() >= 2)
            {
                //W43-1 interaction: a group SMALLER than the declaration
                //minimum is not a legal block at all, so nothing below it is
                //ever priced - gangKillNeed starts its search there.
                const int minBlk = attackers[j]->minBlockersRequired();
                //#W64-AG (F7): the derived verdict, same math as the 1-on-1s.
                const bool atkFS = attackers[j]->basicAbilities[Constants::FIRSTSTRIKE] != 0
                                   || attackers[j]->basicAbilities[Constants::DOUBLESTRIKE] != 0;
                const bool atkDT = attackers[j]->basicAbilities[Constants::DEATHTOUCH] != 0;
                int dmg = 0;
                bool anyOfThem = false, byDt = false;
                int soak = 0; //#W79-DA (T5)
                const int need = gangKillNeed(attackers[j]->power, attackers[j]->toughness,
                                              atkFS, atkDT, gangStats, minBlk,
                                              &dmg, &anyOfThem, &byDt, &soak);
                if (need >= 2)
                {
                    //#W79-DA (T5): the carry-over the group does NOT stop, on
                    //the attackers this window's B-rows already price per body.
                    int spill = -1;
                    if (attackers[j]->basicAbilities[Constants::TRAMPLE])
                    {
                        const int ap = attackers[j]->power > 0 ? attackers[j]->power : 0;
                        spill = ap - soak > 0 ? ap - soak : 0;
                    }
                    string gp = gangBlockPriceTag(need, dmg, anyOfThem, true, false,
                                                  byDt, atkFS, spill, soak); //#W79-DA (T5)
                    if (!gp.empty())
                    {
                        anyGangPriced = true;
                        ln << " [" << gp << "]";
                    }
                }
            }
        }
        //#W65-AN (G6): the same #W62-AA R6 gate the attackers window and this
        //window's own price tails use - a clause that says "chains without
        //limit" must not print over a board where the chain cannot run. No
        //lethal verdict exists on this screen (there is no ATTACK TOTAL here:
        //these attackers are theirs), and the loss branch is reachable by
        //construction, so the caution is the only face this window can print.
        if (oppLoopClosed)
            ln << lifeLoopAttackerRowTag(true, false);
        //#W60-P (B9): an attacker holding one of theirs in exile can die in
        //this combat and hand it straight back. Same fact the blocker rows
        //carry, on the window where the seat chooses to expose the body.
        {
            vector<MTGCardInstance *> heldA;
            exileHostagesOf(attackers[j], heldA);
            vector<string> heldDesc;
            for (size_t hi = 0; hi < heldA.size(); hi++)
                heldDesc.push_back(exileHostageDescriptor(heldA[hi], this));
            ln << exileHostageRowTag(heldDesc);
        }
        aRowName.push_back(attackers[j]->name);
        aRowHandle.push_back(instanceHandle(attackers[j]));
        aRowRest.push_back(ln.str());
        //The TRANSLOG keeps one entry per option, uncollapsed: it is the ordered
        //option list, not the rendered prompt.
        {
            std::ostringstream full;
            full << "A" << (j + 1) << ". " << aRowName[j] << aRowHandle[j] << aRowRest[j];
            shownLines.push_back(full.str());
        }
    }
    bool anyAttackerRangeRowShown = false;
    {
        bool anyAttackerRangeRow = false;
        tail << joinBlockerRows(aRowName, aRowHandle, aRowRest, &anyAttackerRangeRow, "A");
        if (anyAttackerRangeRow)
            tail << kAttackerRangeNote;
        anyAttackerRangeRowShown = anyAttackerRangeRow;
    }
    //#W49-R (D1): a B-row's "may block A3-A257" uses the A-label range form;
    //when no A-row itself was ranged, the note that defines the form is printed
    //after the B-rows instead, so it never appears unexplained.
    bool anyMayBlockRange = false;
    //W36 #2: classify each attacker's becomes-blocked trigger once; the B-line
    //trades below fold a simple self-pump into the computed result and flag
    //anything else as not-included.
    vector<int> bbKind(attackers.size(), 0), bbP(attackers.size(), 0), bbT(attackers.size(), 0);
    for (size_t j = 0; j < attackers.size(); j++)
        bbKind[j] = becomesBlockedSelfPump(attackers[j]->text, bbP[j], bbT[j]);
    //#W55-B (D13): the window's declaration-minimum arithmetic, once, directly
    //above the rows it constrains. Built from the engine's own
    //minBlockersRequired() - the same source both A-line tags read - so the
    //header and the per-attacker tags can never disagree.
    {
        vector<int> minNeeds;
        for (size_t j = 0; j < attackers.size(); j++)
            minNeeds.push_back(attackers[j]->minBlockersRequired());
        tail << menaceBlockerBudgetLine(minNeeds, (int) attackers.size(),
                                        (int) blockers.size());
    }
    tail << "Your available blockers (with, for each attacker it may block, the"
            " naive 1-on-1 trade - before other blockers, pump or combat tricks):\n";
    //#W47 (R8, wave-46 HIGH): the B-rows are built into three parts - name,
    //instance handle, and the whole fact tail - so the emitter below can collapse
    //a RUN of rows that agree in every rendered fact, exactly as the battlefield
    //line five lines above already collapses the same bodies. deck123 vs130 seq
    //55 was 16,887 chars of which 2,887 were 22 B-rows, 21 of them differing only
    //in a #N handle and all reading the identical trade; blockers was the one
    //decision kind that got both bigger and slower this wave.
    vector<string> rowName, rowHandle, rowRest;
    CombatWindowCache cw; //#W54-M (A22): per-window memo of the per-creature facts
    for (size_t i = 0; i < blockers.size(); i++)
    {
        std::ostringstream ln;
        ln << " (" << blockers[i]->power << "/" << blockers[i]->toughness << ")"
           << markedDamageTag(blockers[i]->toughness, blockers[i]->life); //#W82-EC (H3)
        string kw = keywordList(blockers[i]);
        if (!kw.empty())
            ln << " [" << kw << "]";
        //A 0-power blocker kills nothing - it only absorbs damage. Wave-8:
        //a 0-power blocker was thrown in front of a 0/2 Ornithopter, a
        //block that neither killed nor saved anything (the [deals 0] gap
        //in the blocker-seam lethal family).
        //W39-WALLBLOCK / E-162c (wave-39 ledger #5): the two salient tokens
        //here were "deals 0" and "kills nothing", and the clause that actually
        //decides the question - that the block STOPS the damage - was the
        //trailing, unquantified "it only absorbs damage". The wave-29
        //annotation-wording rung (restriction/verb FIRST, no vague affirmative
        //tail) violated in its own register: at 146v162 s7/s10 the pilot wrote
        //"I take 3 damage (if one blocks) or 6 (if both)" and "blocking with a
        //defender doesn't prevent damage to me", declined both blocks, and
        //gave away 9 life in a game it won at 4. Put the STOP first and put the
        //NUMBER in it. TRUST DOCTRINE: N is only claimed where it is exactly
        //computable - a trampling attacker pushes damage past a 0-power body,
        //so that path gets the honest qualifier instead of a wrong N.
        if (blockers[i]->power <= 0)
        {
            int minP = -1, maxP = 0;
            bool anyTrample = false;
            //#W45-1: a set-restricted attacker in the legal list makes the
            //unqualified STOP claim false for that attacker, so the tag has to
            //know. Read from the engine's own minBlockersRequired(), the same
            //source both A-line tags use.
            bool anyMenace = false;
            for (size_t j = 0; j < legal[i].size(); j++)
            {
                int p = legal[i][j]->power > 0 ? legal[i][j]->power : 0;
                if (minP < 0 || p < minP)
                    minP = p;
                if (p > maxP)
                    maxP = p;
                if (legal[i][j]->basicAbilities[Constants::TRAMPLE])
                    anyTrample = true;
                if (legal[i][j]->minBlockersRequired() > 1)
                    anyMenace = true;
            }
            ln << zeroPowerBlockerTag(minP, maxP, anyTrample, anyMenace);
        }
        ln << " - may block";
        //#W49-R (D1): walk the ATTACKER list (so labels come out ascending)
        //and collect (label, parenthetical) pairs; renderMayBlockList ranges
        //consecutive labels with an identical parenthetical and factors an
        //all-identical one. The parenthetical text per pairing is unchanged;
        //`pn` collects it in place of the row stream.
        vector<int> mbLabels;
        vector<string> mbParens;
        bool rowBlockerDies = false; //#W82-P6
        for (size_t k = 0; k < attackers.size(); k++)
            for (size_t j = 0; j < legal[i].size(); j++)
                if (attackers[k] == legal[i][j])
                {
                    std::ostringstream pn;
                    string blockTrigger; //#W56-B (D13)
                    bool blockerDies = false; //#W56-B (D13)
                    string ownLifelink, theirLifelink; //#W57-B (D10)
                    mbLabels.push_back((int) (k + 1));
                    //The computed trade rides the B#:A# pairing so there is
                    //nothing left to re-derive (block-outcome annotation).
                    //W36 #2: a simple when-blocked self-pump is folded IN and
                    //said; an uncomputable trigger is flagged as not included.
                    string trade;
                    //#W45-1: below the declaration minimum there is no 1-on-1
                    //fight to forecast, so none is printed - the restriction
                    //takes the parenthetical instead. Checked BEFORE the
                    //forecast is computed so no favourable string is ever built
                    //for a pairing that cannot happen alone.
                    if (attackers[k]->minBlockersRequired() > 1)
                    {
                        mbParens.push_back(menaceBlockPairingTag(attackers[k]->minBlockersRequired()));
                        break;
                    }
                    if (bbKind[k] == 1)
                    {
                        CombatTradeStat as = cw.statOf(attackers[k]); //#W54-M (A22)
                        as.power += bbP[k];
                        as.toughness += bbT[k];
                        //W41-5: the pumped branch is the same fight, so it asks
                        //the same prevention questions - a self-pump that beats
                        //the blocker's toughness is still irrelevant when the
                        //damage is prevented.
                        trade = combatTradePreviewStats(cw.statOf(blockers[i]), as,
                                                        combatPreventionKind(attackers[k], blockers[i]),
                                                        combatPreventionKind(blockers[i], attackers[k]),
                                                        combatPreventionKindToPlayer(attackers[k], blockers[i]->controller()),
                                                        false, blockers[i]->life,
                                                        cw.converterOf(blockers[i]->controller()),
                                                        &blockTrigger, &blockerDies, //#W56-B (D13)
                                                        &ownLifelink, &theirLifelink, //#W57-B (D10)
                                                        NULL, oppLoopClosed); //#W63-AB (E1)
                        if (!trade.empty())
                        {
                            std::ostringstream bb;
                            bb << trade << " - includes its +" << bbP[k] << "/+"
                               << bbT[k] << " when-blocked trigger";
                            trade = bb.str();
                        }
                    }
                    else
                    {
                        //#W56-B (D13): same call, with the gain and the death
                        //taken OUT of the verdict string.
                        trade = combatBlockOutcome(cw, blockers[i], attackers[k],
                                                   &blockTrigger, &blockerDies,
                                                   &ownLifelink, &theirLifelink, //#W57-B (D10)
                                                   oppLoopClosed); //#W63-AB (E1)
                        if (bbKind[k] == 2 && !trade.empty())
                            trade += " - PLUS its becomes-blocked trigger, NOT"
                                     " included here: read its text";
                    }
                    if (!trade.empty())
                        pn << " (" << trade << ")";
                    //#W56-B (D13): the gain is a SIBLING of the survival verdict,
                    //never nested inside it, and the block's standing cost - the
                    //body this seat stops owning - is stated in its own right.
                    if (!blockTrigger.empty())
                        pn << " {" << blockTrigger << "}";
                    //#W57-B (D10): the two lifelink flavours, siblings of the
                    //verdict for the same reason the blocking trigger is.
                    if (!ownLifelink.empty())
                        pn << " {" << ownLifelink << "}";
                    if (!theirLifelink.empty())
                        pn << " {" << theirLifelink << "}";
                    //#W82-P6: the standing cost is a ROW fact (the body this seat
                    //stops owning does not depend on which attacker it blocks), so
                    //it prints once per row below the pairing list, not per pairing.
                    if (blockerDies)
                        rowBlockerDies = true;
                    mbParens.push_back(pn.str());
                    break;
                }
        {
            bool mbRange = false;
            ln << renderMayBlockList(mbLabels, mbParens, &mbRange);
            if (mbRange)
                anyMayBlockRange = true;
        }
        if (rowBlockerDies) //#W82-P6: once per row (was once per pairing)
            ln << " {" << afterCombatBlockerCostText((int) blockers.size()) << "}";
        //#W60-P (B9): the fact the `152v146` s35 chump had no way to know -
        //this body is holding one of theirs in exile, and losing it here gives
        //it back. A ROW-level clause, not a per-attacker one: the return does
        //not depend on which attacker it blocks.
        {
            vector<MTGCardInstance *> heldB;
            exileHostagesOf(blockers[i], heldB);
            vector<string> heldDesc;
            for (size_t hi = 0; hi < heldB.size(); hi++)
                heldDesc.push_back(exileHostageDescriptor(heldB[hi], this));
            ln << exileHostageRowTag(heldDesc);
        }
        rowName.push_back(blockers[i]->name);
        rowHandle.push_back(instanceHandle(blockers[i]));
        rowRest.push_back(ln.str());
        //The TRANSLOG keeps one entry per option, uncollapsed: it is the
        //ordered option list, not the rendered prompt.
        {
            std::ostringstream full;
            full << "B" << (i + 1) << ". " << rowName[i] << rowHandle[i] << rowRest[i];
            shownLines.push_back(full.str());
        }
    }
    //#W47 (R8): the collapse. Same rule as joinZoneEntries - a run must agree in
    //NAME, in the whole fact tail byte-for-byte, and hold CONSECUTIVE handle
    //ranks, so any difference (a counter, a keyword, a different trade against
    //any attacker) splits the run and no member is ever described by a fact that
    //is not its own. Nothing is deleted: both ranges are printed in full, the
    //count rides the row, and the note below states that every label inside the
    //range is still individually nameable - the reply grammar reads B# labels,
    //which are unchanged, so a collapsed row cannot narrow what is legal.
    bool anyBlockerRangeRow = false;
    tail << joinBlockerRows(rowName, rowHandle, rowRest, &anyBlockerRangeRow);
    if (anyBlockerRangeRow)
        tail << kBlockerRangeNote;
    if (anyMayBlockRange && !anyAttackerRangeRowShown) //#W49-R (D1)
        tail << kAttackerRangeNote;
    //W36 #1: the trade-trust rule, at the tail for salience (see the constant).
    tail << kBlockTradeTrustNote;
    //#W46-5 SCOPE, stated once rather than on every line - the mirror of the
    //attackers window's gang-block scope line. The trust note above ends on
    //"Only gang-blocks, pumps or combat tricks can change it", which names the
    //gang block without pricing it; this says the price is already on the
    //attacker line when it is exactly computable, so the group arithmetic is
    //not something to re-derive either.
    if (anyGangPriced)
        tail << "A \"GANG BLOCK:\" clause on an attacker line is the cheapest lethal"
                " group YOUR listed blockers can field against that attacker,"
                " computed from their powers: several DIFFERENT blockers may block"
                " one attacker and their power adds together, which no 1-on-1 trade"
                " above includes. Use that number; do not re-derive it. Each"
                " blocker can still join only ONE such group.\n";
    //Wave-35 churn driver #5, defender side. Main phase 2 is NOT true here (it
    //is the opponent's turn), so only the priority fact is stated - a true
    //statement in the wrong scope is a lie (trust doctrine).
    tail << kBlockersTurnFacts;
    tail << "Assign each blocker to AT MOST ONE attacker (a creature cannot block"
            " two attackers), but several DIFFERENT blockers may gang-block the same"
            " attacker. Blockers you do not mention stay out of combat.\n";
    tail << kPlanFirstLead //#W70-BL (E2)
         << "on a line of its own BLOCKS: followed by the assignments, comma-separated,"
            " e.g. \"BLOCKS: B1:A2, B3:A1, B2:none\" - each B-number at most ONCE,"
            " and several B-numbers may share one A-number - or exactly"
            " \"BLOCKS: none\" to"
            " block with nobody this turn. Write nothing else.";
    //W36 item 1, the one-per-combat RE-ASK: the previous reply assigned one
    //blocker to several different attackers, so the question is re-put with a
    //terse correction naming the violated constraint. The changed text makes
    //this a DIFFERENT prompt, so the async machinery issues a fresh call
    //rather than replaying the conflicted answer. Deliberately free of
    //B<digit>/A<digit> codes and of the word "blocks" so a model echoing the
    //correction cannot feed the assignment parser a phantom pair.
    bool reasking = (mBlockReaskTurn == observer->turn && mBlocksDoneTurn != observer->turn);
    if (reasking)
        tail << "\n[RE-ASK] Your previous reply assigned ONE creature to SEVERAL"
                " different attackers, which is illegal - each creature can"
                " block only ONE attacker. Re-answer the whole assignment now,"
                " naming each of your creatures at most once.";
    //#W55-B (D3): the ALL-ILLEGAL re-ask's own correction, quoting back the
    //pairs the CR 509.1c / canBlock passes pruned. Its own latch, because a
    //gang-conflict re-ask and an all-illegal re-ask are different failures and
    //one firing must not spend the other's arm. Changing the text is also what
    //makes this a DIFFERENT prompt, so the async machinery issues a fresh call
    //instead of replaying the rejected answer.
    if (mBlockIllegalReaskTurn == observer->turn && mBlocksDoneTurn != observer->turn)
        tail << prunedPairsReaskClause(mBlockIllegalReaskPairs);
    if (mBlockRevReaskTurn != observer->turn)
        mBlockRevReaskLine.clear();
    const bool blockRevReasked = (mBlockRevReaskTurn == observer->turn
                                  && !mBlockRevReaskLine.empty()
                                  && mBlocksDoneTurn != observer->turn);
    if (blockRevReasked)
        tail << "\n" << mBlockRevReaskLine;
    mLogWindowKind = kAskWindowCombat; //#W57-H (D43): combat keeps the whole log
    const string blockTail = tail.str(); //#W68-BA (J6): one copy, reused by the re-ask
    string userMsg = assemblePrompt(blockTail);

    string content;
    //#W71-BS (F5): the widest legal answer here is one `Bi:Aj` pair per blocker.
    setAnswerFloorForSeam("blockers", (long) blockers.size(), kBlockPairAnswerBytes);
    if (pollCompletionRetry(userMsg, content, "blockers") == kChoicePending)
        return 1; //decision in flight; nothing declared yet, re-poll next tick

    //Plan split BEFORE the assignment parse: a "B2" or bare numbers in the
    //plan prose must not read as block assignments.
    //#W49-S (D2b): the answer is a BLOCKS: line (consumePlan's default took
    //the last line-leading label of ANY kind, so a trailing CoT "Attack:" line
    //could stand in for the assignment); among several BLOCKS: lines the LAST
    //CLEAN one is the final answer, the first one otherwise (the same rule the
    //attackers seam applies - see combatLineIsClean).
    string decisionPart = consumePlan(content, "BLOCKS:");
    bool blocksFirstLine = true;
    //#W65-AP (R2, wave-65 codex review finding 2 - HIGH): this driver kept its
    //own last-CLEAN-wins walk, so `BLOCKS: B1:A1 / ... / BLOCKS: none` executed
    //the later unmarked line the protocol calls thinking-out-loud. The choice is
    //gptBlocksLineFromReply now, on the shared gptSelectAnswerIndex.
    {
        vector<string> bNames, aNames;
        for (size_t j = 0; j < blockers.size(); j++)
            bNames.push_back(blockers[j]->name);
        for (size_t j = 0; j < attackers.size(); j++)
            aNames.push_back(attackers[j]->name);
        string picked;
        if (gptBlocksLineFromReply(content, bNames, aNames,
                                   picked, &blocksFirstLine, &mLastParseNote,
                                   &mLastIgnoredAnswerLine)) //#W66-AR (H2c)
            decisionPart = picked;
    }
    //Name tables for the block name->label reconcile (mirror of the shipped
    //ATTACK reconcile): the display names as shown in the B#/A# labels, plus
    //each blocker's legal attacker index set so an ambiguous attacker name
    //can be pinned to the one it may actually block.
    vector<string> blockerNames, attackerNames;
    blockerNames.reserve(blockers.size());
    attackerNames.reserve(attackers.size());
    for (size_t j = 0; j < blockers.size(); j++)
        blockerNames.push_back(blockers[j]->name);
    for (size_t j = 0; j < attackers.size(); j++)
        attackerNames.push_back(attackers[j]->name);
    vector<vector<int> > legalIdx(blockers.size());
    for (size_t i = 0; i < blockers.size(); i++)
        for (size_t j = 0; j < legal[i].size(); j++)
            for (size_t k = 0; k < attackers.size(); k++)
                if (attackers[k] == legal[i][j])
                    legalIdx[i].push_back((int) k);
    vector<int> pick;
    //Per-decision assignment bookkeeping (wave-34 #1b(B)). Reset here so a
    //previous combat's prunes can never leak onto this record, and count the
    //parser's own silent drops - the half pruned_pairs never saw.
    mLastPrunedPairs.clear();
    mLastDroppedAssignments = 0;
    bool gangConflict = false;
    bool blockerInAttackerSlot = false;
    int pairs = content.empty() ? 0 : parseBlockAssignments(decisionPart, blockers.size(), attackers.size(), pick,
                                                             &blockerNames, &attackerNames, &legalIdx,
                                                             &mLastDroppedAssignments, &gangConflict,
                                                             &blockerInAttackerSlot);
    //#W53-M (D19): the drop is right, the silence was not.
    if (blockerInAttackerSlot)
        appendParseNote(&mLastParseNote, "blocker_handle_in_attacker_slot");
    if (blocksFirstLine && !content.empty())
        mAnswerReplacedFalse = true; //#W49-S (D2): what executes is the first coded line

    //W36 item 1: an illegal one-blocker-many-attackers reply is RE-ASKED once
    //(with the correction appended - see the tail above) instead of silently
    //keeping its first pair; the conflicted attempt is logged as its own
    //record so the corpus can count every re-ask. A second conflicted reply
    //falls through to the shipped first-wins behavior, marked in the note.
    //#W53-M (D1). That re-ask DISCARDED the whole reply, and a re-ask that
    //never came back cost the whole combat, silently: `152v162` seq 26 (turn
    //14) logged `multiblock_reask` with `chosen_text` null and then NO
    //follower record of any kind - no second model call, no declaration, no
    //stderr confirmation - and the two attackers the reply had asked to block
    //took the seat from 24 to 18. The legal half of that reply (`B2:A1`, a 6/4
    //eating a 3/4 for free) went out with the illegal duplicate.
    //The duplicate now costs the DUPLICATE only. When the first-wins reading
    //still holds an assignment this seat can legally execute, it is declared
    //on THIS tick, through the validator below - there is no longer a window
    //in which the declaration can go missing. The re-ask survives for the one
    //case with nothing to lose (a conflicted reply whose every pairing is
    //illegal or absent), and every exit below writes its record, empty result
    //included. Prevention moved to the prompt: the BLOCKS format line now
    //carries the one-per-blocker constraint (D23).
    const int conflictLegalPairs = gangConflict
        ? countLegalAssignments(pick, attackers.size(), legalIdx) : 0;
    if (gangConflict && conflictLegalPairs > 0)
    {
        appendParseNote(&mLastParseNote, "multiblock_first_wins");
        setNotice("a creature was assigned to several attackers - the extra assignment is dropped", 5.0f);
        DebugTrace("AIPlayerGPT: one-blocker-many-attackers reply -> duplicate dropped, "
                   << conflictLegalPairs << " legal assignment(s) kept");
        gangConflict = false; //handled here: the reply is not thrown away
    }
    if (gangConflict && mBlockReaskTurn != observer->turn)
    {
        mBlockReaskTurn = observer->turn;
        writeTransLog("blockers", userMsg, content, pairs, (int) blockers.size(),
                      "", "multiblock_reask", &shownLines);
        setNotice("a creature was assigned to several attackers - asking again", 5.0f);
        DebugTrace("AIPlayerGPT: one-blocker-many-attackers reply -> re-asking once");
        return 1; //next tick rebuilds the prompt WITH the correction line
    }
    if (reasking)
        appendParseNote(&mLastParseNote, gangConflict ? "multiblock_reask_exhausted"
                        : (pairs > 0 ? "multiblock_reask_recovered" : "multiblock_reask_unanswered"));

    //WAVE-29 N-18e: the reply committed a block in an early coded line, then
    //reasoned itself OUT of blocking but was cut off by the token ceiling before
    //a corrected answer. Honoring the stale commit is a fatal reach-trade the
    //model already rejected (deck18 vs93 s20). Declare the SAFE combat default
    //(no blockers). Only fires on truncated-AND-contradicted; a well-terminated
    //reply, or a truncated commit never contradicted, keeps its answer.
    //N-122d (wave-31 ledger #12) narrows this: the predicate now ignores a
    //LATER coded assignment (self-correction, owned by line precedence) and
    //per-creature "B3 will not block" bookkeeping, so a compliant head line
    //whose reasoning merely re-derives it is no longer discarded. What still
    //fires is a genuine GLOBAL prose reversal - and that IS the model's
    //answer, so no-blocks stands there. The blanket part of the old default is
    //gone: when the abandoned commit also has no legally executable
    //assignment left, the HEURISTIC blocks instead of declining combat
    //outright (a blanket no-blocks maximizes incoming damage and is the
    //adjudication tiebreaker - the worst answer to default to).
    //#W70-BN (F1): the truncated-abandoned arm is DELETED with the predicate that
    //drove it (see the deleted `truncatedBlockCommitmentAbandoned`). A legal
    //assignment on the BLOCKS: line stands whatever the bytes after it say.

    //A bare "BLOCKS: none" - the model's natural way to decline every
    //block - carries no B<n> pair, and falling back on it handed the
    //declaration to the heuristic, which BLOCKED (a fatal reach-trade
    //against the pilot's own correct no-block decision, wave-11 deck131
    //vs109). An explicit decline is a valid answer: declare no blockers.
    if (pairs == 0 && !decisionPart.empty())
    {
        string low;
        for (size_t i = 0; i < decisionPart.size() && i < 80; i++)
            low += (char) tolower((unsigned char) decisionPart[i]);
        if (low.find("none") != string::npos || low.find("no block") != string::npos
            || low.find("no assignment") != string::npos)
        {
            DecisionAction none;
            DecisionManager::applyDeclareBlockers(req, none);
            writeTransLog("blockers", userMsg, content, 0, (int) blockers.size(),
                          "no blockers", NULL, &shownLines);
            narrateDecision("You declared no blockers");
            mBlocksDoneTurn = observer->turn;
            DebugTrace("AIPlayerGPT: declared blocks from 0 assignment(s)"
                       " - explicit all-decline (no blockers) in one reply");
            return 1;
        }
    }

    //Repeat-loop salvage: a spiral may have stated a valid BLOCKS: line before
    //degenerating (the seat's fallbacks were 0-of-9 salvaged; the CHOICE
    //salvage did not cover this path). Recover the last well-formed assignment
    //through the full validator; a hallucinated/out-of-range line yields 0 and
    //is skipped, so this never invents a block. Not for the explicit-decline
    //case above (already handled).
    if (pairs == 0 && !content.empty())
    {
        int sal = salvageLoopedBlocks(content, blockers.size(), attackers.size(),
                                      blockerNames, attackerNames, legalIdx, pick);
        if (sal > 0)
        {
            DebugTrace("AIPlayerGPT: salvaged looped BLOCKS (" << sal << " pair(s))");
            pairs = sal;
        }
    }

    const char * blockSource = NULL;

    if (pairs == 0)
    {
        //Unusable reply: the heuristic declares this combat instead.
        writeTransLog("blockers", userMsg, content, pairs, (int) blockers.size(),
                      "", content.empty() ? noAnswerClass() : unparsedReplyClass(content), &shownLines);
        noticeFallback("model reply failed - the heuristic blocks", 5.0f);
        mBlocksDoneTurn = observer->turn;
        DebugTrace("AIPlayerGPT: declared blocks from 0 assignment(s)"
                   " - unusable reply, heuristic declares");
        return AIPlayerBaka::chooseBlockers();
    }

    //Answer through the contract: the manager owns the defenser-cycling
    //choreography and re-validates each assignment.
    DecisionAction act;
    string declared;
    int intended = 0;
    string pruned;
    for (size_t i = 0; i < blockers.size(); i++)
    {
        if (pick[i] < 1)
            continue;
        intended++;
        MTGCardInstance * chosen = attackers[pick[i] - 1];
        if (!blockers[i]->canBlock(chosen))
        {
            //Model assigned an illegal block: that blocker stays home. THIS is
            //the seam that keeps illegal choices structurally impossible, and
            //it is the only legality gate the name-form parse relies on - so
            //record what it pruned rather than losing it (wave-33 N-152j).
            pruned += (pruned.empty() ? "" : "; ") + blockers[i]->name + " -> " + chosen->name;
            mLastDroppedAssignments++;
            continue;
        }
        act.blocks.push_back(std::make_pair(blockers[i], chosen));
        declared += (declared.empty() ? "" : "; ") + blockers[i]->name + " blocks " + chosen->name;
    }
    //W43-1 (CR 509.1c): the SET-level legality pass. canBlock() above is
    //pairwise and cannot see it - the first blocker of a legal menace pair is
    //individually legal, so a reply naming only that one passed every gate and
    //was declared, narrated, and then silently deleted by the rules layer. The
    //assignment is under-filled or it is not made; and because it is dropped
    //HERE it is dropped through the same accounting every other prune uses,
    //never silently. (Under-filling is now the only way a menace block can
    //fail: the render never offers one the seat could not complete.)
    for (bool changed = true; changed; )
    {
        changed = false;
        for (size_t a = 0; a < act.blocks.size() && !changed; a++)
        {
            MTGCardInstance * atk = act.blocks[a].second;
            const int need = atk->minBlockersRequired();
            if (need < 2)
                continue;
            int assigned = 0;
            for (size_t b = 0; b < act.blocks.size(); b++)
                if (act.blocks[b].second == atk)
                    assigned++;
            if (assigned >= need)
                continue;
            for (size_t b = act.blocks.size(); b-- > 0; )
                if (act.blocks[b].second == atk)
                {
                    pruned += (pruned.empty() ? "" : "; ") + act.blocks[b].first->name
                              + " -> " + atk->name + " (needs " + (need >= 3 ? "3" : "2")
                              + " blockers, only " + (assigned == 1 ? "1" : "2") + " assigned)";
                    mLastDroppedAssignments++;
                    act.blocks.erase(act.blocks.begin() + b);
                }
            //`declared` is rebuilt from the surviving set below, so the
            //narration can never claim a block this pass took back.
            changed = true;
        }
    }
    declared.clear();
    for (size_t b = 0; b < act.blocks.size(); b++)
        declared += (declared.empty() ? "" : "; ") + act.blocks[b].first->name
                    + " blocks " + act.blocks[b].second->name;
    mLastPrunedPairs = pruned;

    //EVERY intended assignment was illegal (deck139 vs152 s21: a name-form
    //reply naming a flier its own B-line never offered). Declaring the empty
    //set here would silently convert "the model asked for an impossible
    //block" into "the model declined to block" - a blanket no-blocks is the
    //worst combat default (it maximises incoming damage), and wave-32 ledger
    //#15 flagged this exit as the silent one. Fall back to the heuristic, as
    //every other failure path does, and log it as its OWN fallback class so
    //the next corpus can count it separately from unparsed_reply.
    if (intended > 0 && act.blocks.empty())
    {
        //#W55-B (D3 = R195): ONE re-ask, carrying the pruned pairs as text. The
        //answer is not discarded silently any more - the rejection goes back to
        //the model in the same window, exactly as the cast path's rejectedSoFar
        //and the priority path's plan_choice_conflict already do. A SECOND
        //all-illegal reply is not re-asked again: it settles here, stamped
        //`all_assignments_illegal_exhausted`, and (as every other failure exit
        //does) hands this combat to the heuristic rather than declaring a
        //blanket no-blocks, which is the worst combat default there is
        //(wave-32 ledger #15). Bounded by construction: one arm per turn.
        //#W55-B (D14 = R206): both records now name the pairs and the reason
        //with the class.
        const bool illegalExhausted = (mBlockIllegalReaskTurn == observer->turn);
        const string illegalClass = allAssignmentsIllegalClass(pruned, illegalExhausted);
        if (!illegalExhausted)
        {
            mBlockIllegalReaskTurn = observer->turn;
            mBlockIllegalReaskPairs = pruned;
            writeTransLog("blockers", userMsg, content, pairs, (int) blockers.size(),
                          "", illegalClass.c_str(), &shownLines, blockSource);
            setNotice("every block the model asked for was illegal - asking again", 5.0f);
            DebugTrace("AIPlayerGPT: every assignment illegal (" << pruned
                       << ") -> re-asking once with the pruned pairs");
            return 1; //next tick rebuilds the prompt WITH the correction
        }
        writeTransLog("blockers", userMsg, content, pairs, (int) blockers.size(),
                      "", illegalClass.c_str(), &shownLines, blockSource);
        noticeFallback("every block the model asked for was illegal - the heuristic blocks", 5.0f);
        mBlocksDoneTurn = observer->turn;
        DebugTrace("AIPlayerGPT: declared blocks from 0 assignment(s) - all "
                   << intended << " assignment(s) illegal after a re-ask, heuristic declares");
        return AIPlayerBaka::chooseBlockers();
    }
    DecisionManager::applyDeclareBlockers(req, act);
    writeTransLog("blockers", userMsg, content, pairs, (int) blockers.size(),
                  declared.empty() ? string("no blockers") : declared, NULL, &shownLines, blockSource);
    narrateDecision(declared.empty() ? string("You declared no blockers")
                                     : ("You declared blockers: " + declared));
    mBlocksDoneTurn = observer->turn;
    DebugTrace("AIPlayerGPT: declared blocks from " << pairs << " assignment(s) in one reply");
    return 1;
}


//The async blockers declaration is still owed for this combat (W36 item 6).
//True only while this seat DEFENDS with a live endpoint and this turn's
//bundled declaration has not committed - every chooseBlockers exit
//(declared, declined, heuristic fallback, vacuous builder) stamps
//mBlocksDoneTurn, so "due" can never outlive the decision itself.
bool AIPlayerGPT::blockersDeclarationDue()
{
    return !mEndpoint.empty() && observer && observer->currentPlayer != this
           && mBlocksDoneTurn != observer->turn;
}


//A human-readable name for option one's eligibility filter, parsed from the
//card-script target clause (the same restriction the engine's canTarget
//enforces). "target(<upto:1>*[-land;-creature]|reveal)" -> "noncreature,
//nonland card"; "target(land[snow]|reveal)" -> "snow land". Empty string when
//there is no parseable restriction (the per-card [eligible] marks still carry
//the ground truth; this only makes the WHY legible). Representation only.
//#W72-BV (M17, wave-70 L21 / deck152 MED-2, unfixed for two waves): the reveal
//ask names option one by its SCRIPT LABEL ("get a human", "choose card") and
//never says where the card the label picks actually GOES. `152v162` seq 26/35
//and `126v146` seq 8 all read `Choose the ONE card that goes to "choose card"`
//over a search whose script is `moveto(hand)`; nothing on the screen said hand,
//library, battlefield or graveyard, and a mana plan built on the wrong zone is
//false. The destination is in the same script string the filter is parsed from,
//so it costs nothing to read: `moveto(<zone>)` / `movetoand(<zone>)`. Only the
//zones the engine's own mover names are translated; anything else returns "" and
//the ask says nothing rather than guessing. Pure over the script.
static string revealDestinationZone(const string& effect)
{
    string low = effect;
    for (size_t i = 0; i < low.size(); i++)
        low[i] = (char) tolower((unsigned char) low[i]);
    size_t m = low.find("moveto(");
    if (m == string::npos)
        return "";
    size_t open = m + 7;
    size_t close = low.find(')', open);
    if (close == string::npos || close <= open)
        return "";
    const string z = low.substr(open, close - open);
    if (z.find("battlefield") != string::npos || z.find("inplay") != string::npos)
        return z.find("opponent") != string::npos ? "the OPPONENT's battlefield"
                                                  : "YOUR battlefield";
    if (z.find("graveyard") != string::npos)
        return z.find("owner") != string::npos ? "its owner's graveyard" : "a graveyard";
    if (z.find("exile") != string::npos)
        return "exile";
    if (z.find("library") != string::npos)
        return "a library";
    if (z.find("hand") != string::npos)
        return z.find("opponent") != string::npos ? "the OPPONENT's hand" : "YOUR HAND";
    return "";
}


static string revealDestinationClause(const string& optOneLabel, const string& zone)
{
    if (zone.empty())
        return "";
    return " The card you send to \"" + optOneLabel + "\" goes to " + zone + ".";
}


//#W72-BV (M17, wave-70 L21): the reveal/search rows carried a copy ordinal
//INSIDE THE LIST and nothing about the copies the seat already holds, while
//every cast row on every other screen carries exactly that. `152v162` seq 38
//took a THIRD Katilda while holding one and controlling one; a legendary
//duplicate that reaches the battlefield is a state-based sacrifice, and one
//that reaches the hand is a dead card. Counts only, from the seat's own PUBLIC
//zone and its own hand - both are the reader's information - and the legend
//consequence is stated only for a card the engine itself calls legendary.
//Pure over four inputs, so every branch is provable.
static string revealDuplicateTag(bool legendary, int inMyHand, int onMyBattlefield)
{
    if (inMyHand <= 0 && onMyBattlefield <= 0)
        return "";
    std::ostringstream o;
    o << " {you already have this card: ";
    bool any = false;
    if (onMyBattlefield > 0)
    {
        o << onMyBattlefield << " on your battlefield";
        any = true;
    }
    if (inMyHand > 0)
    {
        o << (any ? ", " : "") << inMyHand << " in your hand";
        any = true;
    }
    if (legendary && onMyBattlefield > 0)
        o << " - it is LEGENDARY, so a second copy of it on your battlefield makes you put"
             " one of them into the graveyard at once (CR 704.5j)";
    o << "}";
    return o.str();
}


//#W72-BV (M17, wave-70 L21): the carried PLAN at a reveal window is the plan
//from an earlier window - 6 of 10 wave-70 reveal windows and 1 of 3 in wave 71
//were answered with a combat plan (`Attack with Sigarda.` then `PUT: 5`), and
//`126v146` seq 8's plan was "Next turn, cast Sanguine Bond and Exquisite
//Blood". Nothing about the reply FORM changes: the plan is still written, and
//this states only what the window can accept, which the seam already enforces.
const char * kRevealPlanScopeFact =
    "The PLAN carried above was stated at an EARLIER window; this window's answer is a card"
    " number off the list above, and no action that plan names can be taken here.\n";


static string describeRevealFilter(const string& effect)
{
    size_t t = effect.find("target(");
    if (t == string::npos)
        return "";
    size_t open = t + 7;
    size_t close = effect.find(')', open);
    if (close == string::npos || close <= open)
        return "";
    string inner = effect.substr(open, close - open);
    size_t bar = inner.find('|'); //drop the "|reveal" zone suffix
    if (bar != string::npos)
        inner = inner.substr(0, bar);
    if (!inner.empty() && inner[0] == '<') //drop the "<upto:N>"/"<anyamount>" amount
    {
        size_t g = inner.find('>');
        if (g != string::npos)
            inner = inner.substr(g + 1);
    }
    size_t br = inner.find('[');
    string base = (br == string::npos) ? inner : inner.substr(0, br);
    string attrs;
    if (br != string::npos)
    {
        attrs = inner.substr(br + 1);
        size_t brc = attrs.find(']');
        if (brc != string::npos)
            attrs = attrs.substr(0, brc);
    }
    //Each ';'-separated attribute: "-X" reads "nonX", "+X"/"X" reads "X".
    string adjectives;
    {
        string tok;
        for (size_t i = 0; i <= attrs.size(); i++)
        {
            char c = (i < attrs.size()) ? attrs[i] : ';';
            if (c == ';')
            {
                if (!tok.empty())
                {
                    string a = (tok[0] == '-') ? ("non" + tok.substr(1))
                             : (tok[0] == '+') ? tok.substr(1) : tok;
                    adjectives += (adjectives.empty() ? "" : ", ") + a;
                }
                tok.clear();
            }
            else
                tok += c;
        }
    }
    string noun = (base == "*" || base.empty()) ? "card" : base;
    string out = adjectives;
    if (!out.empty())
        out += " ";
    out += noun;
    return out;
}


//W37 #4: the ONE builder for the per-card eligibility tag, used by BOTH the
//ELIGIBILITY header's promise and the per-card option lines, so the header
//can never again instruct a match against a marker that does not render.
static string revealEligMarker(const string & optOneLabel)
{
    return " [eligible for \"" + optOneLabel + "\"]";
}


//#W73-BZ (N7, deck146 HIGH-2): the price of a card on a reveal/search list, in
//the same two facts the ELIGIBILITY header decides on - the PRINTED cost and its
//mana value. An empty cost string is a card that has none (a land); say so
//rather than print nothing (a silent gap is confabulated - the {0}-cost lesson).
//Pure over the two values, so both faces pin without a board.
static string revealCostTag(const string& printedCost, int manaValue)
{
    std::ostringstream o;
    if (printedCost.empty())
        o << " [no mana cost (mana value 0)]";
    else
        o << " [cost: " << printedCost << " (mana value " << manaValue << ")]";
    return o.str();
}


//#W61-V (R1): does option one's own target() spec license taking NONE? This is
//the engine's own arity grammar, read the way TargetChooser::createTargetChooser
//reads it (TargetChooser.cpp:503-528): a `<...>` prefix sets a MINIMUM equal to
//its amount unless it carries "upto:" (or "anyamount", which clears the minimum
//outright), and a spec with no `<...>` at all is a plain one-target chooser -
//a minimum of one. targetMin cannot be used for this: the chooser object stores
//false for BOTH a bare `target(x)` and an `<upto:1>` one, which is how a
//mandatory Pelakka-Predation-class choose-one came to be offered a decline.
bool revealSinglePickDeclineLegal(const string& targetSpec)
{
    const size_t lt = targetSpec.find('<');
    if (lt == string::npos)
        return false; //bare target(): the card chooses one, and must
    const size_t gt = targetSpec.find('>', lt);
    if (gt == string::npos)
        return false; //unparsable arity: fail closed, claim no decline
    const string arity = targetSpec.substr(lt + 1, gt - lt - 1);
    return arity.find("upto:") != string::npos
           || arity.find("anyamount") != string::npos;
}


//Pure builder for the reveal ask text (no game/observer/endpoint state).
//revealSource: 0 = top of library (surveil/dig), 1 = the opponent's hand, 2 =
//the decider's own hand. pickExactlyOne: option one is a fixed <1> chooser.
//
//Eligibility surfacing (unchanged): a FILTERED reveal (a snow-land tutor, a
//noncreature-nonland dig) only accepts a subset for option one; when the
//engine's per-card verdict marks some cards ineligible, name the filter and tag
//each card. Annotation only - every revealed card stays listed and pickable.
//(history: comment-archaeology.md AIPlayerGPTSeams-buildRevealAskText-2303)
static string buildRevealAskText(const vector<MTGCardInstance*>& revealed,
                                 const string& optOneLabel,
                                 const string& optTwoLabel,
                                 const string& optOneEffect,
                                 const vector<bool>& eligibleForOptionOne,
                                 int revealSource, bool pickExactlyOne,
                                 //#W61-T (C8): the single pick was reached by the
                                 //chooser's ARITY (a bare target()), not by a fixed
                                 //<1>. Ignored unless pickExactlyOne is set.
                                 bool singlePickBare,
                                 //#W61-V (R1): declining is a LEGAL answer here -
                                 //the script's arity carries a minimum of zero
                                 //("upto:"). A mandatory choose-one is never told
                                 //it may take none.
                                 bool singlePickDeclineLegal,
                                 bool wholeLibrary,
                                 //#W55-D (D18): the permutation the collapse used, so the
                                 //caller can map the reply's positions back to `revealed`.
                                 vector<size_t> * outOrder = NULL,
                                 //#W72-BV (M17): the reading seat, for the
                                 //already-have-this-card counts. NULL annotates
                                 //nothing, so the PARSETEST corpus is unaffected.
                                 Player * seat = NULL)
{
    if (outOrder)
    {
        outOrder->resize(revealed.size());
        for (size_t oi = 0; oi < revealed.size(); oi++)
            (*outOrder)[oi] = oi;
    }
    bool haveElig = (eligibleForOptionOne.size() == revealed.size());
    int eligCount = 0;
    if (haveElig)
        for (size_t j = 0; j < eligibleForOptionOne.size(); j++)
            if (eligibleForOptionOne[j]) eligCount++;
    bool restricted = haveElig && eligCount < (int) revealed.size();
    string filterPhrase = restricted ? describeRevealFilter(optOneEffect) : string();

    bool fromHand = (revealSource == 1 || revealSource == 2);
    bool selfHand = (revealSource == 2);
    std::ostringstream tail;
    if (fromHand)
    {
        tail << (selfHand ? "You revealed your hand"
                          : "The opponent revealed their hand")
             << " (" << revealed.size() << " card"
             << (revealed.size() == 1 ? "" : "s") << ").\n";
        if (pickExactlyOne)
            //#W61-T (C8): the bare-arity branch states what the ENGINE's chooser
            //accepts (one card, never two).
            //#W61-V (R1): and it says a decline is available ONLY where the
            //script's own arity carries a minimum of zero. Pelakka Predation's
            //current Oracle reads "You choose a card from it with mana value 3 or
            //greater" - a mandatory choice - so its ask no longer names a decline
            //it has no right to offer.
            tail << (singlePickBare ? "Choose ONE card to send to \""
                                    : "Choose the ONE card to send to \"")
                 << optOneLabel
                 << "\" - that is the card "
                 << (selfHand ? "you discard" : "they discard")
                 << "; every other card stays in "
                 << (selfHand ? "your" : "their") << " hand."
                 << (singlePickBare ? " This is a ONE-card choice: you can never take"
                                      " two."
                                    : "")
                 << (singlePickBare && singlePickDeclineLegal
                         ? " \"PUT: none\" declines." : "")
                 << (singlePickBare && !singlePickDeclineLegal
                         ? " This choice is NOT optional: while a card qualifies you"
                           " must take one." : "")
                 << revealDestinationClause(optOneLabel, revealDestinationZone(optOneEffect))
                 << "\n"; //#W72-BV (M17)
        else
            tail << "Decide, in ONE reply, which cards go to \"" << optOneLabel
                 << "\"; every card you do NOT pick goes to \"" << optTwoLabel
                 << "\".\n";
    }
    else
    {
        //N-166p (wave-34 audit b5): a TUTOR moves the WHOLE library into the
        //reveal zone, and this line registered it as "you looked at the top 48
        //cards of your library" - a top-of-library look, which is a different
        //game action with different information and a different cost. Name the
        //action for what it is when the library has nothing left behind it.
        if (wholeLibrary)
            tail << "Search: you are searching your ENTIRE library - all "
                 << revealed.size() << " card"
                 << (revealed.size() == 1 ? "" : "s") << " of it are listed below."
                    " This is a search, not a look at the top of your library:"
                    " every card you own that is still in the library is here.";
        else
            tail << "Reveal: you looked at the top " << revealed.size()
                 << " card" << (revealed.size() == 1 ? "" : "s") << " of your library.";
        if (pickExactlyOne)
            tail << (singlePickBare ? " Choose ONE card that goes to \""
                                    : " Choose the ONE card that goes to \"")
                 << optOneLabel
                 << "\"; every other card goes to \"" << optTwoLabel << "\"."
                 << (singlePickBare ? " This is a ONE-card choice: you can never take"
                                      " two."
                                    : "")
                 << (singlePickBare && singlePickDeclineLegal //#W61-V (R1)
                         ? " \"PUT: none\" declines." : "")
                 << (singlePickBare && !singlePickDeclineLegal
                         ? " This choice is NOT optional: while a card qualifies you"
                           " must take one." : "")
                 << revealDestinationClause(optOneLabel, revealDestinationZone(optOneEffect))
                 << "\n"; //#W72-BV (M17)
        else
            tail << " Decide, in ONE reply, which of them go to \"" << optOneLabel
                 << "\"; every card you do NOT pick goes to \"" << optTwoLabel
                 << "\".\n";
    }
    if (restricted)
    {
        //W37 #4 (validation-158-36-116 defect 1, 10/10): the header promised a
        //bare "[eligible]" marker while the per-card tag renders as
        //[eligible for "<option one>"] - a match instruction against a marker
        //that does not exist (trust doctrine). Promise the EXACT tag that
        //renders below (revealEligMarker builds both). Grammar: a/an by vowel.
        bool vowel = !filterPhrase.empty()
                     && string("aeiouAEIOU").find(filterPhrase[0]) != string::npos;
        tail << "ELIGIBILITY: only " << (filterPhrase.empty() ? "certain cards"
                                                              : ((vowel ? "an " : "a ") + filterPhrase))
             << " may go to \"" << optOneLabel << "\" - the rest do not qualify and go"
                " to \"" << optTwoLabel << "\" regardless. Pick ONLY from the cards"
                " marked" << revealEligMarker(optOneLabel) << " below";
        if (eligCount == 0)
            tail << " (none of these qualify - answer \"PUT: none\")";
        tail << ".\n";
    }
    //#W55-D (D18): the rows are built into a vector and collapsed the same way
    //every ask menu is. `123v125` s9 printed FIFTY rows with `Intruder Alarm`
    //x4, `Damnation` x4 and `Thraben Doomsayer` x4 as separate strings that
    //differed only in their copy ordinal, which the collapse could not see.
    //Nothing is deleted or capped: the copies are gathered (groupNumberedRows'
    //stable permutation) and printed as ONE row carrying the copy range, and
    //the caller un-permutes the reply so a number still means the card it named.
    vector<string> revealRows;
    for (size_t j = 0; j < revealed.size(); j++)
    {
        int rcopies = 0;
        int rrank = listCopyRank(revealed, j, rcopies);
        std::ostringstream row;
        row << revealed[j]->name << copyOfTag(rrank, rcopies, "this list");
        if (revealed[j]->isCreature())
            row << " (" << revealed[j]->power << "/" << revealed[j]->toughness
                 << " creature)";
        else
        {
            string tt = typeTag(revealed[j]);
            if (!tt.empty())
                row << " (" << tt << ")";
        }
        //#W73-BZ (N7, deck146 HIGH-2): the ELIGIBILITY header gates this list on
        //mana VALUE ("only a manacost>=3 card may go to ...") and the rows printed
        //no cost at all - the one number the header makes decisive was the one
        //number withheld, so "biggest body" read as "biggest threat" and 146v125
        //seq 14 took Emrakul over the Supreme Verdict that swept it at seq 39.
        //The cast menu already prices every row; so does this one now.
        {
            string mcText;
            int mv = 0;
            if (revealed[j]->data && revealed[j]->data->getManaCost())
            {
                mcText = revealed[j]->data->getManaCost()->toString();
                mv = revealed[j]->data->getManaCost()->getConvertedCost();
            }
            row << revealCostTag(mcText, mv);
        }
        string kw = keywordList(revealed[j]);
        if (!kw.empty())
            row << " [" << kw << "]";
        string txt = cardTextSnippet(revealed[j], 140);
        if (!txt.empty())
            row << " {text: " << txt << "}";
        //#W72-BV (M17): the copies the seat already holds, counted off its own
        //hand and its own battlefield - both the reader's information.
        if (seat && seat->game)
        {
            int dupHand = 0, dupPlay = 0;
            MTGGameZone * dz[2] = { seat->game->hand, seat->game->inPlay };
            for (int zi = 0; zi < 2; zi++)
                for (int ci = 0; dz[zi] && ci < dz[zi]->nb_cards; ci++)
                    if (dz[zi]->cards[ci] && dz[zi]->cards[ci] != revealed[j]
                        && dz[zi]->cards[ci]->name == revealed[j]->name)
                        (zi == 0 ? dupHand : dupPlay)++;
            row << revealDuplicateTag(revealed[j]->hasType(Subtypes::TYPE_LEGENDARY) != 0,
                                      dupHand, dupPlay);
        }
        //The eligibility tag rides the option line so the filter cannot be
        //missed (the same "deciding fact rides the option" principle as combat).
        if (restricted)
            row << (eligibleForOptionOne[j]
                     ? revealEligMarker(optOneLabel)
                     : " [does NOT qualify - goes to \"" + optTwoLabel + "\"]");
        revealRows.push_back(row.str());
    }
    {
        vector<size_t> revealOrder;
        groupNumberedRows(revealRows, revealOrder);
        vector<string> shownReveal;
        shownReveal.reserve(revealRows.size());
        for (size_t k = 0; k < revealOrder.size(); k++)
            shownReveal.push_back(revealRows[revealOrder[k]]);
        bool revealRanged = false;
        unsigned revealForms = 0; //#W82-P8
        tail << joinNumberedRows(shownReveal, &revealRanged, &revealForms);
        if (revealRanged)
            tail << optionRangeNote(revealForms);
        if (outOrder)
            *outOrder = revealOrder;
    }
    tail << kRevealWindowScopeFact; //#W71-BQ (L12)
    tail << kRevealPlanScopeFact;   //#W72-BV (M17)
    if (pickExactlyOne)
        tail << kPlanFirstLead //#W70-BL (E2)
             << "on a line of its own PUT: followed by the ONE card number you"
                " choose (e.g. \"PUT: 2\") - ONE number, never a list"
             //#W61-T (C8): when the pick itself is optional, declining is always
             //legal - state it whether or not the filter emptied the list. The
             //mandatory branch keeps the wave-20 wording byte for byte.
             //#W61-V (R1): "optional" is now the SCRIPT's arity, not targetMin, so
             //a mandatory bare chooser (Pelakka Predation) is no longer handed a
             //decline. The eligCount == 0 arm is unreachable from the live seam
             //(decideReveal never asks with an empty eligible set) and is kept for
             //a direct call.
             << (singlePickDeclineLegal ? ", or exactly \"PUT: none\" to choose no card"
                                        : (eligCount == 0 ? ", or \"PUT: none\" if none qualify" : ""))
             << ". Write nothing else."; //#W70-BL (E2)
    else
        tail << kPlanFirstLead //#W70-BL (E2)
             << "on a line of its own PUT: followed by the card numbers you send to \""
             << optOneLabel << "\", comma-separated (e.g. \"PUT: 1, 3\"), or exactly"
                " \"PUT: none\" to send none there (every revealed card then goes to \""
             << optTwoLabel << "\"). Write nothing else."; //#W70-BL (E2)
    return tail.str();
}


//Interactive reveal/surveil decision, driven from MTGRevealingCards for an
//interactive AI. ONE bundled ask over ALL revealed cards; the model picks the
//subset that goes to option one (surveil: the graveyard). Async: 0 while the
//call is in flight, 1 decided, -1 on failure (the display then sends nothing
//to option one - the safe keep-on-top default).
int AIPlayerGPT::decideReveal(const vector<MTGCardInstance*>& revealed,
                              const string& optOneLabel, const string& optTwoLabel,
                              const string& optOneEffect,
                              vector<int>& selForOptionOne,
                              const vector<bool>& eligibleForOptionOne,
                              int revealSource, bool pickExactlyOne,
                              bool singlePickBare, //#W61-T (C8)
                              bool singlePickDeclineLegal) //#W61-V (R1)
{
    selForOptionOne.clear();
    if (mEndpoint.empty() || revealed.empty())
        return -1; //no endpoint / nothing to choose: the display's default

    //eligibleForOptionOne is the engine's OWN per-card acceptance verdict for
    //option one (tc->canTarget - exactly what the click enforces). When it is
    //populated it is the same size/order as `revealed`; when it is empty the
    //reveal is not predicate-gated (a plain surveil/look-at-top-N) and every
    //card may go to option one.
    bool haveElig = (eligibleForOptionOne.size() == revealed.size());
    int eligCount = (int) revealed.size();
    if (haveElig)
    {
        eligCount = 0;
        for (size_t j = 0; j < eligibleForOptionOne.size(); j++)
            if (eligibleForOptionOne[j]) eligCount++;
    }

    //N-136a: a predicate-gated reveal with NOTHING eligible for option one is
    //not a decision - do NOT ask the model. Mausoleum Secrets with an empty
    //graveyard gates option one on "black card, MV <= creature cards in
    //graveyard" = MV<=0 = no card in the library qualifies. The seam still
    //LISTS every revealed card (soft "[does NOT qualify]" tag), so the model
    //happily "picked" an ineligible bomb (MV4 Ritual of Soot), the engine's
    //click filter bounced it, the tutor no-opped, and the model mis-planned for
    //turns around a card it never received. An empty legal set auto-resolves to
    //the card's fail branch (option two: shuffle/keep) via the display's safe
    //default (-1 -> nothing to option one) - offer and enforcement cannot
    //diverge because the offer never happens.
    if (haveElig && eligCount == 0)
    {
        writeTransLog("reveal", "", "", -1, (int) revealed.size(),
                      "none (no legal target)", NULL, NULL);
        narrateDecision("You revealed " + std::to_string(revealed.size())
                        + " card" + (revealed.size() == 1 ? "" : "s")
                        + " but none was a legal target - took none");
        return -1;
    }

    if (mSystemPrompt.empty())
        buildSystemPrompt();

    //#W55-D (D18): `revealOrder` is the collapse's stable permutation of the
    //printed rows. The reply is parsed in PRINTED positions and mapped back
    //through it, so a collapsed list cannot make a number mean another card.
    vector<size_t> revealOrder;
    mLogWindowKind = kAskWindowTargetOrReveal; //#W57-H (D43): keeps the whole log
    const string revealAskText =
        buildRevealAskText(revealed, optOneLabel, optTwoLabel, optOneEffect,
                           eligibleForOptionOne, revealSource, pickExactlyOne,
                           singlePickBare, //#W61-T (C8)
                           singlePickDeclineLegal, //#W61-V (R1)
                           //N-166p: a search empties the library zone into the
                           //reveal zone; a top-of-library look does not.
                           revealSource == 0 && game->library->nb_cards == 0
                               && revealed.size() > 1,
                           &revealOrder, this); //#W72-BV (M17)
    string userMsg = assemblePrompt(revealAskText);
    if (revealOrder.size() != revealed.size())
    {
        revealOrder.resize(revealed.size());
        for (size_t oi = 0; oi < revealed.size(); oi++)
            revealOrder[oi] = oi;
    }

    string content;
    setAnswerFloorForSeam("reveal", (long) revealed.size(), kPutSlotAnswerBytes); //#W71-BS (F5)
    if (pollCompletionRetry(userMsg, content, "reveal") == kChoicePending)
        return 0; //decision in flight; the display waits and re-polls next tick

    //Plan split BEFORE the subset parse: bare numbers in the plan prose must
    //not read as card picks.
    //#W52-G (E-1): this ask's own label, so a trailing CHOICE: line (a cast
    //intent the model appended) is never taken as the PUT: answer.
    string decisionPart = consumePlan(content, "PUT:");
    vector<bool> send;
    vector<string> names;
    names.reserve(revealed.size());
    //#W55-D (D18): names are in PRINTED order - the same order the numbers the
    //model answers with index.
    for (size_t j = 0; j < revealed.size(); j++)
        names.push_back(revealed[revealOrder[j]]->name);
    int result = content.empty() ? -1
                 : parseAttackerSet(decisionPart, revealed.size(), send, &names, true);

    //Repeat-loop salvage: the reveal seam's worst fallbacks are 12k+ decode
    //spirals; if one stated a valid PUT: line before degenerating, recover it
    //(the same net as salvageLoopedChoice, through parseAttackerSet's validator).
    if (result < 0 && !content.empty())
    {
        int sal = salvageLoopedSubset(content, "PUT:", revealed.size(), names, send);
        if (sal >= 1)
        {
            DebugTrace("AIPlayerGPT: salvaged looped PUT (" << sal << ")");
            result = sal;
        }
    }

    if (result < 0)
    {
        //#W67-AX (I5): where a decline is not a legal answer, an unusable reply
        //still owes the engine a legal pick - the safe default here is not safe,
        //it is a VOIDED spell (see revealRefusalMustPick). #W67-AZ (R7): the pick
        //is the HEURISTIC's, over the rows the engine's own chooser accepts -
        //cost-dominant, the preference `FindCardToPlay`'s "*" rank carries - and
        //not the first element of the vector. Recorded as the class
        //`reveal_fallback_pick` with the card it took, so a corpus reads what
        //answered instead of reconstructing it from a force-close line.
        std::vector<int> revealedCmc; //#W67-AZ (R7)
        for (size_t ri = 0; ri < revealed.size(); ri++)
            revealedCmc.push_back(revealed[ri] && revealed[ri]->getManaCost()
                                  ? revealed[ri]->getManaCost()->getConvertedCost() : 0);
        const int forced = revealRefusalMustPick(pickExactlyOne, singlePickDeclineLegal)
                           ? heuristicRevealIndex(revealedCmc, eligibleForOptionOne)
                           : -1;
        if (forced >= 0 && forced < (int) revealed.size())
        {
            selForOptionOne.push_back(forced);
            const string took = revealed[forced]->name;
            //#W67-AZ (R7): the record's row number is the PRINTED position - the
            //number the options list in the same record is indexed by - not the
            //position in the engine's reveal vector.
            int forcedRow = forced + 1;
            for (size_t rj = 0; rj < revealOrder.size(); rj++)
                if (revealOrder[rj] == (size_t) forced)
                {
                    forcedRow = (int) rj + 1;
                    break;
                }
            writeTransLog("reveal", userMsg, content, forcedRow, (int) revealed.size(),
                          took, "reveal_fallback_pick", &names);
            noticeFallback("model reply failed - the heuristic chose among the legal cards", 5.0f);
            narrateDecision(revealSummaryNarration(revealed.size(), took, optOneLabel));
            DebugTrace("AIPlayerGPT: reveal reply unusable and this chooser has no legal"
                       " decline - the heuristic took " << took);
            return 1;
        }
        //Unusable reply: the display falls back to its safe default (send
        //nothing to option one - every card keeps option two).
        writeTransLog("reveal", userMsg, content, result, (int) revealed.size(),
                      "", content.empty() ? noAnswerClass() : unparsedReplyClass(content), &names);
        noticeFallback("model reply failed - reveal kept the default", 5.0f);
        return -1;
    }

    //#W55-D (D18): back from PRINTED positions to `revealed` positions.
    unpermuteSelection(revealOrder, revealed.size(), send);
    string chosen;
    for (size_t j = 0; j < revealed.size(); j++)
        if (send[j])
        {
            //N-136a enforcement filter: never ACCEPT a pick the option-one
            //chooser will reject at the click - the same canTarget verdict in
            //eligibleForOptionOne. The soft "[does NOT qualify]" annotation is
            //advisory only; a model that picks an ineligible card anyway must
            //not have that pick recorded as chosen/narrated as tutored (the
            //engine would silently bounce it). Dropping it here keeps the
            //translog + narration consistent with what the engine does, so the
            //model is never told it holds a card that never reached hand.
            if (haveElig && j < eligibleForOptionOne.size() && !eligibleForOptionOne[j])
                continue;
            selForOptionOne.push_back((int) j);
            chosen += (chosen.empty() ? "" : ", ") + revealed[j]->name;
            //A fixed <1> chooser takes exactly ONE card: the engine's option-one
            //chooser auto-fires on the first eligible click (driveInteractiveReveal
            //phase 0), so any extra picks would silently drop. Trim to the first
            //selected card here so narration/translog match what the engine does.
            if (pickExactlyOne)
                break;
        }
    writeTransLog("reveal", userMsg, content, result, (int) revealed.size(),
                  chosen.empty() ? string("none") : chosen, NULL, &names);
    //#W42-D1: on a SEARCH (this seat's whole library went to the reveal zone,
    //so flushSearchReveal armed the mask) the echo would be a third line about
    //an event the log already carries twice - the collapsed "You searched your
    //library with <X>" above it, and the found card's own move line below it.
    //Suppressing it is also the more TRUTHFUL choice: `chosen` is what we
    //accepted, while the move line is what the engine actually did. A search
    //that took nothing still says so - that outcome has no move line at all.
    //PROBE CORRECTION (first pass): the empty-`chosen` branch is NOT "took
    //nothing" on a search. The engine's own option-one chooser can take the
    //card through chooseTarget instead of this reply, so `chosen` empty here
    //means "this ask did not decide it", not "no card was found" - narrating
    //the second would be a lie about a card the very next line puts in hand.
    //Both branches are suppressed on a search: the collapsed search line above
    //and the found card's own move line below carry the whole event, truthfully.
    //W42-D4 (grammar half): when the echo DOES print (not a search), the old
    //template spliced a bare count and an engine menu label straight into a
    //sentence ("You revealed 51 and put Exquisite Blood to choose card", 282
    //corpus lines). revealSummaryNarration pluralises the count, states the
    //noun, and demotes the engine's option label to a parenthetical.
    if (mSearchMaskOwner != this)
        narrateDecision(revealSummaryNarration(revealed.size(), chosen,
                                               chosen.empty() ? optTwoLabel : optOneLabel));
    DebugTrace("AIPlayerGPT: reveal put " << selForOptionOne.size() << " of "
               << revealed.size() << " to option one in one reply");
    return 1;
}


//---- Pre-game (opening-hand) decision hooks (PreGamePhase, before turn 1) ----

//#W47-R9a: the price of taking THIS mulligan, on the row that takes it. `next`
//is the hand size a keep AFTER this mulligan would leave (this hand's size
//minus the one further card the London mulligan bottoms). Clamped at zero, and
//zero is spelled out - "keeping 0" is the point at which the option stops being
//a search and becomes a concession, and two seats reached it this corpus.
//Pure so both branches are provable without a game.
static string mulliganRowLabel(int next)
{
    if (next < 0)
        next = 0;
    std::ostringstream o;
    o << "Mulligan (a keep after this one would keep " << next << " card"
      << (next == 1 ? "" : "s");
    if (next == 0)
        o << " - your entire hand goes to the bottom";
    o << ")";
    return o.str();
}


//#W74-CC (O16, wave-73 deck130 MED): the FRESH-7 window said nothing about what
//a keep keeps. Every later window carries "(keeping N)" - that spelling is what
//the deck guides' mulligan bullets key on - and the first one, which is the one
//every game has and most games only have, printed the size clause only when
//mullsTaken > 0. So on deck130 seq 2 (identical in all games) the bullets were
//unreachable. Same spelling, on every window. Pure over the two inputs.
static string mulliganKeepClause(int mullsTaken, int keepSize)
{
    std::ostringstream o;
    if (mullsTaken <= 0)
    {
        o << " (keeping " << keepSize << ")";
        return o.str();
    }
    o << ", and having already taken " << mullsTaken << " mulligan"
      << (mullsTaken > 1 ? "s" : "") << " you will bottom " << mullsTaken
      << " card" << (mullsTaken > 1 ? "s" : "") << " on a keep (keeping "
      << keepSize << ")";
    return o.str();
}


int AIPlayerGPT::pregameMulliganDecision(int mullsTaken)
{
    mPregameMullsSeen = mullsTaken; //the true count, for the bottom ask (N-139i)
    if (mEndpoint.empty())
        return AIPlayerBaka::pregameMulliganDecision(mullsTaken);
    PregameAskScope pregameScope(this); //N-158r: the hand-only frame is ask-scoped
    std::ostringstream q;
    int keepSize = startingHandSize() - mullsTaken;
    q << "Pre-game mulligan decision (London mulligan). You have a fresh "
      << game->hand->nb_cards << "-card opening hand";
    q << mulliganKeepClause(mullsTaken, keepSize); //#W74-CC (O16)
    q << ". Keep this hand, or mulligan (shuffle back and draw " << startingHandSize()
      << " again, bottoming one more at the next keep)?";
    vector<string> opts;
    opts.push_back("Keep this hand");
    //#W47-R9a: the Mulligan row is the one unpriced row in this loop - casts
    //carry {X pricing:}, targets carry {target text:}, blockers carry the
    //computed trade, and the mulligan's price lived only in the header sentence
    //ABOVE, never on the row being chosen. Price it where it is chosen, and
    //make the terminal case readable rather than arithmetic: at a keep of zero
    //the whole hand goes to the bottom.
    opts.push_back(mulliganRowLabel(startingHandSize() - mullsTaken - 1));
    //W35 addendum (6): the mulligan was the last full question-and-answer echo
    //in the log ("Pre-game mulligan decision (London mulligan). ... ? -> Keep
    //this hand"). It records its OUTCOME: a keep states the hand it kept, a
    //mulligan states the size it went to (this hand's size, minus the one more
    //card it will bottom on the next keep).
    vector<string> narr;
    narr.push_back(mulliganNarration(true, keepSize));
    narr.push_back(mulliganNarration(false, startingHandSize() - mullsTaken - 1));
    setAskNarration(narr);
    int pick = askModel(q.str(), opts, true);
    if (pick == kChoicePending)
        return PREGAME_PENDING;
    if (pick < 0)
        pick = AIPlayerBaka::pregameMulliganDecision(mullsTaken);
    //#W42-D9: a mulligan is about to walk the whole hand back into the library
    //one card at a time. Arm the collapse from the DECISION, not from a window.
    if (pick == 1)
        mPregameShufflingBack = true;
    return pick; //0 keep, 1 mulligan
}


int AIPlayerGPT::pregameLeylineDecision(MTGCardInstance * card)
{
    if (mEndpoint.empty() || !card)
        return AIPlayerBaka::pregameLeylineDecision(card);
    PregameAskScope pregameScope(this); //N-158r
    std::ostringstream q;
    q << "Pre-game action (CR 103.6): you may begin the game with " << card->name
      << " already on the battlefield (its static ability is live for the whole"
         " game). Put it onto the battlefield?";
    vector<string> opts;
    opts.push_back("Yes, begin the game with " + card->name + " on the battlefield");
    opts.push_back("No, keep it in hand");
    vector<string> narr;
    narr.push_back("You began the game with " + card->name + " on the battlefield");
    narr.push_back("You kept " + card->name + " in hand instead of starting it on the battlefield");
    setAskNarration(narr);
    int pick = askModel(q.str(), opts, true);
    if (pick == kChoicePending)
        return PREGAME_PENDING;
    if (pick < 0)
        return AIPlayerBaka::pregameLeylineDecision(card);
    return pick == 0 ? 1 : 0;
}


//Build the tail for the ONE bundled BOTTOM-N ask (reuses the reveal PUT: reply
//shape; parsed by parseAttackerSet / salvageLoopedSubset like decideReveal).
//#W82-A R4 (LEDGER v2, Astra genuine): THE ORDER THE ANSWER GAVE.
//`parseAttackerSet` returns a MEMBERSHIP MASK, and the queue was then filled by
//walking the hand - so `PUT: 6, 2` bottomed card 2 first and the model's stated
//order was discarded. CR 103.5 puts the bottoming order in the player's hands
//("in any order"), which makes it a LEGAL CHOICE, and the owner's ruling is that
//a legal choice is never removed; Astra ranks the loss MED for exactly that
//reason. This walk recovers the order: every integer in 1..handSize, in the order
//the reply wrote it, de-duplicated, with an ascending `a-b` range expanded where
//it appears. It decides ORDER ONLY - the mask stays the authority on WHICH cards
//go - so a malformed or name-only answer simply yields nothing here and the
//previous hand-order behaviour stands. Pure, so PARSETEST pins it.
void w82PutOrderFromReply(const string& text, size_t handSize, std::vector<int>& order)
{
    order.clear();
    if (handSize == 0)
        return;
    std::set<int> seen;
    size_t i = 0;
    while (i < text.size())
    {
        if (!isdigit((unsigned char) text[i]))
        {
            i++;
            continue;
        }
        size_t k = i;
        long lo = 0;
        while (k < text.size() && isdigit((unsigned char) text[k]) && lo < 100000)
            lo = lo * 10 + (text[k++] - '0');
        long hi = lo;
        //an ascending `a-b` range, expanded where it appears
        if (k < text.size() && text[k] == '-' && k + 1 < text.size()
            && isdigit((unsigned char) text[k + 1]))
        {
            size_t k2 = k + 1;
            long v = 0;
            while (k2 < text.size() && isdigit((unsigned char) text[k2]) && v < 100000)
                v = v * 10 + (text[k2++] - '0');
            if (v > lo)
            {
                hi = v;
                k = k2;
            }
        }
        for (long v = lo; v <= hi; v++)
            if (v >= 1 && v <= (long) handSize && seen.insert((int) v).second)
                order.push_back((int) v);
        i = k;
    }
}


string AIPlayerGPT::buildPregameBottomAskText(const vector<MTGCardInstance*>& hand, int need,
                                             int alreadyBottomed)
{
    std::ostringstream tail;
    //N-139i: every number here is now the number it claims to be. The old text
    //printed the CARDS-TO-BOTTOM count wearing a "mulligans" label (a different
    //quantity once the count is clamped, and it walked downward mid-loop), and
    //it never said that this one ask covers the whole set.
    int remaining = need - alreadyBottomed;
    if (remaining < 0)
        remaining = 0;
    if (remaining > (int) hand.size())
        remaining = (int) hand.size();
    int keep = (int) hand.size() - remaining;
    //mPregameMullsSeen is stamped by pregameMulliganDecision on every round, so
    //it is the engine's own count; the max() is a belt for a seat that somehow
    //reaches bottoming without having been asked (the two quantities are equal
    //whenever the count is not clamped).
    int mulls = (mPregameMullsSeen > need) ? mPregameMullsSeen : need;
    tail << "London mulligan bottoming (CR 103.5): you took " << mulls
         << " mulligan" << (mulls == 1 ? "" : "s") << " and kept, so you must put "
         << need << " card" << (need == 1 ? "" : "s")
         << " from your hand on the BOTTOM of your library";
    if (alreadyBottomed > 0)
        tail << " (" << alreadyBottomed << " already bottomed; " << remaining << " to go)";
    tail << ". Name EXACTLY " << remaining << " card" << (remaining == 1 ? "" : "s")
         //#W82-A R4 (LEDGER v2): the ENGINE now honours the order (see
         //`w82PutOrderFromReply`), so the promise this sentence makes is true
         //again. The L9 pass had made the TEXT match a hand-order engine; Astra
         //ranks that MED because CR 103.5 ("in any order") makes the order a LEGAL
         //CHOICE and the ruling is that a legal choice is never removed. Fixing
         //the engine keeps the choice and keeps the sentence honest.
         << " now - this is the ONLY ask for them, and they will be bottomed one at"
            " a time in the ORDER YOU NAME THEM: `PUT: 6, 2` puts card 6 on the"
            " bottom first and card 2 under it. ";;
    if (keep <= 0)
        tail << "Your ENTIRE hand goes to the bottom, so order them worst-first.\n";
    else
        tail << "You will be left with a " << keep << "-card hand, so keep your best "
             << keep << " and bottom your worst " << remaining << ".\n";
    for (size_t j = 0; j < hand.size(); j++)
    {
        int copies = 0;
        int rank = listCopyRank(hand, j, copies);
        tail << (j + 1) << ". " << hand[j]->name << copyOfTag(rank, copies, "your hand")
             << changelingAnnotation(hand[j]);
        if (hand[j]->isLand())
            tail << landTag(hand[j]); //land + colors it taps for (deck93 wave-27), not a bare "(land)"
        else
            //N-36b: the same symbolic token every other hand surface prints, so
            //a {0} card reads identically here and the colour pips the pregame
            //count header is reasoning about are visible on the card's own line.
            tail << manaCostToken(hand[j]);
        string txt = cardTextSnippet(hand[j], 120);
        if (!txt.empty())
            tail << " {text: " << txt << "}";
        tail << "\n";
    }
    tail << kPlanFirstLead //#W70-BL (E2)
         << "on a line of its own PUT: followed by the " << remaining << " card number"
         << (remaining == 1 ? "" : "s") << " you send to the bottom, comma-separated (e.g. \"PUT: "
         << (remaining == 1 ? "3" : "3, 5") << "\"). Write nothing else."; //#W70-BL (E2)
    return tail.str();
}


//#W42-D9: every card this seam hands back is about to become a hand->library
//move. Arming the flag HERE (rather than guessing from turn/phase) is what
//lets receiveEvent collapse the bottoming without ever swallowing a real
//hand->library move that happens to land in the same window.
MTGCardInstance * AIPlayerGPT::pregameChooseBottom(int need, int chosenSoFar, int & status)
{
    MTGCardInstance * picked = pregameChooseBottomInner(need, chosenSoFar, status);
    if (picked)
        mPregameBottomingNow = true;
    return picked;
}


MTGCardInstance * AIPlayerGPT::pregameChooseBottomInner(int need, int chosenSoFar, int & status)
{
    status = 0;
    if (mEndpoint.empty())
        return AIPlayerBaka::pregameChooseBottom(need, chosenSoFar, status);
    PregameAskScope pregameScope(this); //N-158r

    //ONE bundled ask per keep; then pop the queued cards one per call. `need`
    //is the TOTAL owed for this keep and is stable across the loop since the
    //N-139i clamp fix, so this asks exactly once; `remaining` keeps the ask
    //honest in the residual case where the engine re-enters with a new total.
    int remaining = need - chosenSoFar;
    if (remaining < 0)
        remaining = 0;
    if (!mPregameBottomAsked || mPregameBottomForMulls != need)
    {
        vector<MTGCardInstance*> hand;
        for (int i = 0; i < game->hand->nb_cards; i++)
            hand.push_back(game->hand->cards[i]);
        if ((int) hand.size() < remaining)
            remaining = (int) hand.size();
        if (mSystemPrompt.empty())
            buildSystemPrompt();
        mLogWindowKind = kAskWindowPregame; //#W57-H (D43): hand-only, no GAME LOG
        const string bottomAskText = buildPregameBottomAskText(hand, need, chosenSoFar);
        string userMsg = assemblePrompt(bottomAskText);
        string content;
        setAnswerFloorForSeam("bottom", (long) hand.size(), kPutSlotAnswerBytes); //#W71-BS (F5)
        if (pollCompletionRetry(userMsg, content, "bottom") == kChoicePending)
        {
            status = PREGAME_PENDING;
            return NULL; //call in flight; PreGamePhase re-polls next tick
        }
        string decisionPart = consumePlan(content, "PUT:"); //#W52-G (E-1): own label
        if (decisionPart.empty() && !content.empty())
            decisionPart = consumePlan(content);
        vector<bool> send;
        vector<string> names;
        for (size_t j = 0; j < hand.size(); j++)
            names.push_back(hand[j]->name);
        int result = content.empty() ? -1
                     : parseAttackerSet(decisionPart, hand.size(), send, &names, true);
        if (result < 0 && !content.empty())
        {
            int sal = salvageLoopedSubset(content, "PUT:", hand.size(), names, send);
            if (sal >= 1)
                result = sal;
        }
        mPregameBottomQueue.clear();
        string chosen;
        if (result >= 0)
        {
            //#W82-A R4: the ORDER the answer gave, over the SET the mask proved.
            //`w82PutOrderFromReply` decides order only; `send` still decides which
            //cards go, so an order the scan cannot read costs nothing - the tail
            //loop below appends whatever it missed in the old hand order.
            std::vector<int> putOrder;
            w82PutOrderFromReply(decisionPart.empty() ? content : decisionPart,
                                 hand.size(), putOrder);
            std::vector<bool> queued(hand.size(), false);
            for (size_t oi = 0; oi < putOrder.size()
                                && (int) mPregameBottomQueue.size() < remaining; oi++)
            {
                const size_t j = (size_t) (putOrder[oi] - 1);
                if (j >= hand.size() || j >= send.size() || !send[j] || queued[j])
                    continue;
                queued[j] = true;
                mPregameBottomQueue.push_back(hand[j]);
                chosen += (chosen.empty() ? "" : ", ") + hand[j]->name;
            }
            for (size_t j = 0; j < hand.size() && (int) mPregameBottomQueue.size() < remaining; j++)
                if (j < send.size() && send[j] && !queued[j])
                {
                    queued[j] = true;
                    mPregameBottomQueue.push_back(hand[j]);
                    chosen += (chosen.empty() ? "" : ", ") + hand[j]->name;
                }
        }
        //Enforce EXACTLY the owed count: the model under-picked or failed ->
        //fill with the highest-cost cards not already chosen (the heuristic
        //policy). We already capped above, so over-picks are trimmed.
        //This was the one seam that fell back to the heuristic and said
        //nothing - every other decision announces it, so this one does too.
        if (result < 0 || (int) mPregameBottomQueue.size() < remaining)
            noticeFallback("model reply failed - the heuristic bottoms cards", 5.0f);
        while ((int) mPregameBottomQueue.size() < remaining)
        {
            MTGCardInstance * fill = NULL;
            int fc = -1;
            for (int i = 0; i < game->hand->nb_cards; i++)
            {
                MTGCardInstance * c = game->hand->cards[i];
                bool already = false;
                for (size_t k = 0; k < mPregameBottomQueue.size(); k++)
                    if (mPregameBottomQueue[k] == c) { already = true; break; }
                if (already)
                    continue;
                int cost = c->getManaCost() ? c->getManaCost()->getConvertedCost() : 0;
                if (cost > fc) { fc = cost; fill = c; }
            }
            if (!fill)
                break;
            mPregameBottomQueue.push_back(fill);
        }
        writeTransLog("bottom", userMsg, content, result, (int) hand.size(), chosen,
                      result < 0 ? (content.empty() ? noAnswerClass() : unparsedReplyClass(content)) : NULL,
                      &names);
        mPregameBottomAsked = true;
        mPregameBottomForMulls = need;
    }
    //Pop the next queued card that is still in hand.
    while (!mPregameBottomQueue.empty())
    {
        MTGCardInstance * c = mPregameBottomQueue.front();
        mPregameBottomQueue.erase(mPregameBottomQueue.begin());
        if (game->hand->hasCard(c))
            return c;
    }
    return AIPlayerBaka::pregameChooseBottom(need, chosenSoFar, status);
}


//#W55-D (D9): the cleanup discard row's missing verdicts. `125v162` seq 47
//(t17, 8 life, send 2) listed nine cards with cost, type and card text and no
//VERDICT of any kind, while the same prompt's own header said the seat had 9
//permanents of which 9 were lands and the opponent controlled 0 creatures. It
//sent both Fall of the Gavel - its only answers to a noncreature - and died two
//turns later. Every other ask kind prices its rows; this one did not. Three
//clauses, each pure over facts the emitter already computes elsewhere.
//#W61-T (C11, wave-60 deck130 HIGH): the clause printed on EVERY land row
//whenever myLands >= 1, with no comparison against what the hand still costs.
//`130v123` seq 9 (turn 4, TWO lands down, Siege-Gang Commander {3}{r}{r} in
//hand) marked all three land rows `{spare: you control 2 lands already}` and
//the seat pitched two of them - the emitter marked exactly the cards the deck
//guide protects and marked nothing else, and the bait was taken 1/1.
//`myLands` is a FACT; "spare" is a VERDICT, and it is only true when the lands
//already down outrun what the hand can still need. So: gate the WORD on that
//comparison (lands down must EXCEED the most expensive card in hand plus one -
//the next drop the curve still wants) and print the same two numbers as a bare
//FACT otherwise. Nothing is withheld and no row loses its count; only the
//verdict word is conditioned. `haveHandCost` false = no non-land card in hand
//to compare against, so no verdict is available either way.
//#W63-AC (E16, deck125 MED-2): the comparison ran over EVERY non-land card in
//hand, so one uncastable card set the bar for the whole hand. `125v162` seq 94:
//eleven lands down, and all four land rows read `{you control 11 lands already;
//the most expensive card in your hand costs 15}` with no `{spare:` prefix,
//because Emrakul, the Aeons Torn {15} was on the list. In 125v126 seq 39 the
//same at six lands. The one deck in the pool that always holds an uncastable
//{15} is the one deck the tag never fires for, and the guide's cleanup ordering
//keys on the literal `{spare:` string.
//A card the seat cannot reach cannot be what the next land drop is FOR, so it
//is not the demand the verdict is about. `reachCap` is the largest cost this
//comparison still counts (the caller sets it to the land count plus the drops
//the curve can still make); a card above it is named with its cost and stated
//as excluded, so nothing is hidden and the reader can check the arithmetic.
//`beyondCost <= 0` means no such card, and then every byte is as before.
//#W63-AF (R3): the reach cap the spare verdict compares against - the lands
//already down plus the land cards still in the hand this ask is about. One more
//land drop per land card is the ceiling of what this hand can pay for on its
//own, and it is exactly the quantity the clause below already names ("N lands
//plus the drops this hand can still make"). Pure over the two counts.
int spareReachCap(int myLands, int handLandCards)
{
    const int m = myLands > 0 ? myLands : 0;
    const int h = handLandCards > 0 ? handLandCards : 0;
    return m + h;
}

//#W81-DL (V13, wave-80 known-bugs V13 / deck162). THE VERDICT WAS COLOUR-BLIND.
//`162v130` seq 47: five lands down, the most expensive reachable card costing 3,
//and the SWAMP marked `{spare: ...}` - while the hand held three Underworld
//Dreams at `{B}{B}{B}` and the seat's only other black source was one Drowned
//Catacomb. Discarding that Swamp leaves two black sources against a three-black
//cost, so the land is not surplus by any reading; "spare" is a VERDICT about
//whether the mana base still pays for the hand, and COUNT is only half of that
//question (#W61-T C11 fixed the other half). So the word is gated on the colour
//too: `blockColour` names the colour this land is one of the seat's sources for,
//`blockNeed` the most of that colour a single reachable card in hand demands, and
//`blockAfter` how many sources of it would be left if this row were discarded.
//With `blockNeed > blockAfter` the verdict word is withheld and the two numbers
//print as a bare FACT - nothing is hidden, no row loses its count, and the
//comparison the reader must make is on the screen. Pure over its inputs.
string discardSpareLandClause(int myLands, int highestHandCost, bool haveHandCost,
                              const string& beyondName = "", int beyondCost = 0,
                              int reachCap = -1,
                              const string& blockColour = "", //#W81-DL (V13)
                              int blockNeed = 0, int blockAfter = 0)
{
    if (myLands <= 0)
        return "";
    const bool colourBinds = !blockColour.empty() && blockNeed > 0
                             && blockAfter < blockNeed; //#W81-DL (V13)
    bool spare = haveHandCost && highestHandCost >= 0
                 && myLands > highestHandCost + 1
                 && !colourBinds; //#W81-DL (V13)
    std::ostringstream o;
    o << " {";
    if (spare)
        o << "spare: ";
    o << "you control " << myLands << " land" << (myLands == 1 ? "" : "s")
      << " already";
    if (haveHandCost && highestHandCost >= 0)
        o << "; the most expensive card in your hand you could still reach costs "
          << highestHandCost;
    if (!beyondName.empty() && beyondCost > 0)
    {
        o << "; " << beyondName << " at " << beyondCost << " is NOT counted here";
        if (reachCap >= 0)
            o << " - it is past what " << myLands << " land"
              << (myLands == 1 ? "" : "s") << " plus the drops this hand can still"
                 " make would pay for (this comparison counts costs up to "
              << reachCap << ")";
    }
    if (colourBinds) //#W81-DL (V13)
        o << "; COLOUR: a card in your hand you could still reach needs " << blockNeed
          << " " << blockColour << " and discarding this row leaves you " << blockAfter
          << " LAND source" << (blockAfter == 1 ? "" : "s") << " of it (lands only -"
             " any nonland producer you control is not in this count), so this land is"
             " not surplus however many lands you control";
    o << "}";
    return o.str();
}

//The dead-right-now clause names the COUNT the engine's own target chooser
//sees, never a guess about what the card is for. It is deliberately NOT
//rendered for a card that targets the stack: a counterspell has no legal
//target during a cleanup step and always would, so "dead" would be true of the
//moment and false of the card - the trust doctrine's wrong-scope lie, and the
//exact card class the seat wrongly discarded.
string discardDeadTargetClause(int legalTargets)
{
    if (legalTargets != 0)
        return "";
    return " {dead right now: 0 legal targets on the board for it}";
}

//#W57-C (D8, wave-56 ledger HIGH, second wave at HIGH): 357 of 426 discard rows
//carried no verdict, and the ASYMMETRY was the blocker, not the absence.
//`123v130` seq 22 is a 14-row cleanup on a board where the opponent controls no
//creature: the `Tragic Slip` rows read `{dead right now: 0 legal targets on the
//board for it}` (they carry a target spec, so the clause above fires) while
//`Tribute to Hunger`, `Devour Flesh` and `Damnation` on the SAME menu carried
//nothing - an edict at N=0 and a Damnation at `destroys 0` are exactly as dead,
//and the engine already computes both for the CAST menu in the same turn. While
//two dead cards render differently no guide can teach "the unannotated row is
//the keep".
//So: run the cast row's own evaluator (dynamicMagnitudes) over the discard menu
//and re-badge its output into ONE family - a verdict that reads zero prints under
//the same `{dead right now: ...}` tag the target-count clause already uses, and a
//verdict that states a real magnitude prints as `{right now: ...}`. rowSaysNoOp
//is the engine's own zero-predicate, so the two surfaces cannot disagree about
//what "dead" means. Pure over the rendered clause.
//(history: comment-archaeology.md AIPlayerGPTSeams-discardBoardVerdictTag-2315)
string discardBoardVerdictTag(const string& rightNowClause)
{
    const string open = " {right now: ";
    //size() <= open.size() + 1 rejects both a short string and an EMPTY payload
    //(" {right now: }"): an empty verdict is not a verdict.
    if (rightNowClause.size() <= open.size() + 1
        || rightNowClause.compare(0, open.size(), open) != 0
        || rightNowClause[rightNowClause.size() - 1] != '}')
        return "";
    //#W59-I (K4): per SCOPE, not over the whole string.
    if (!AIPlayerGPT::verdictReadsZero(rightNowClause.substr(1)))
        return rightNowClause;
    return " {dead right now: " + rightNowClause.substr(open.size());
}


//The legend, printed only when at least one row carries a verdict. Its last
//sentence is the load-bearing half: a bare row is a row this engine could not
//price, NOT a row it priced as live. Without it the family rule the guide wants
//to teach ("the unannotated row is the keep") would be a lie about every card
//whose text the evaluator does not model. Pure over a bool.
string discardVerdictLegend(bool anyVerdict)
{
    if (!anyVerdict)
        return "";
    return "Verdict tags above are computed from the board as it stands right now:"
           " {dead right now: ...} means the card changes nothing on the board as it"
           " stands, and {right now: ...} states what it would do. A row carrying"
           " NEITHER tag is one this engine could not price here - that is not a"
           " statement that the card is dead.\n";
}


string discardAlreadyControlClause(const string& onBattlefield)
{
    if (onBattlefield.empty())
        return "";
    //#W69-BG (K5, deck146 MED): the brace form was ignored 4 of 4 at this seam
    //while the cast menu's `[legendary: you already control ...]` mark was
    //obeyed. Same fact, so: the same words in the mark shape that is obeyed.
    //The `{dead right now: ...}` verdict is deliberately NOT re-shaped here -
    //it is one cross-seam verdict literal that the guides key on, and this
    //lane's evidence is about THIS clause.
    return " [you already control one: " + onBattlefield + "]";
}


//#W66-AT (H5, deck125 HIGH-2): the discard ask priced its rows correctly and
//then ORDERED against every price it printed. `125v126` seq 55: row 1 was
//Supreme Verdict `{right now: destroys 7 of their creatures ...}`, rows 5/6/8
//were lands carrying `{spare: you control 10 lands already ...}`, and the seat
//answered `PUT: 1`. Corpus-wide, 31 discard asks, 12 offered at least one
//`{spare:` row and only 6 discarded spares only. Two causes, both here: hand
//order is the row order (so the row the model favours is whichever card the
//zone happens to hold first), and the polarity is one-sided - the disposable
//rows are MARKED and the ones that must be kept are not, so a bare row reads
//as the neutral choice.
//Both are fixed by ADDING, never by removing: no row leaves the list, the
//numbers still map one-for-one onto hand positions through the existing
//`outOrder` permutation (the same rail the copy-collapse already rides,
//un-permuted by `unpermuteSelection` at the caller), and every verdict a row
//carried it still carries.
//`discardDisposabilityClass` is the ORDER, read off the row's OWN printed
//verdicts so the list cannot disagree with the text on it: a spare land first
//(the engine has already said this hand does not need it), then a card the
//engine priced as doing NOTHING on this board, then rows it could not price,
//then a card whose verdict says it still does something - the cast-row value
//ranking, inverted, which is what a DISCARD ask wants. Ties keep hand order.
int discardDisposabilityClass(bool isLand, const string& row)
{
    if (isLand)
        return row.find(" {spare: ") != string::npos ? 0 : 2;
    if (row.find(" {dead right now: ") != string::npos)
        return 1;
    if (row.find(" {right now: ") != string::npos)
        return 3;
    return 2;
}


//The order itself: a stable bucket sort over the class vector. Separated from
//the emitter so the permutation the answer parser depends on is provable
//without a game (the corpus-ask case in the self-test runs this exact call).
void discardDisposabilityOrder(const std::vector<int>& classes, std::vector<size_t>& order)
{
    order.clear();
    for (int cls = 0; cls <= 3; cls++)
        for (size_t c = 0; c < classes.size(); c++)
            if (classes[c] == cls)
                order.push_back(c);
}


//And the composition of two permutations: printed position k is `inner[k]` in
//the disposability list, which is `outer[inner[k]]` in HAND order. This is the
//value `outOrder` carries out to `cleanupDiscard`, where `unpermuteSelection`
//uses it to turn the model's numbers back into hand positions.
void composeRowOrder(const std::vector<size_t>& outer, const std::vector<size_t>& inner,
                     std::vector<size_t>& out)
{
    out.clear();
    for (size_t k = 0; k < inner.size(); k++)
        out.push_back(inner[k] < outer.size() ? outer[inner[k]] : inner[k]);
}


//The default sentence. Printed only when the list actually holds a spare row,
//so a hand with nothing spare reads exactly as it did before.
string discardSpareDefaultLine(int spareRows, int over)
{
    if (spareRows <= 0)
        return "";
    std::ostringstream o;
    //#W67-AV (I3): this tail printed a line-leading "DEFAULT ANSWER:" - the
    //engine teaching the model the very heading the corpus then wrote back over
    //its answer label. Same claim, no heading shape.
    o << "BY DEFAULT, discard the spare land. " << spareRows << " row"
      << (spareRows == 1 ? " below is" : "s below are") << " marked {spare: ...} - a"
         " surplus land this hand no longer needs, and the rows are ordered most"
         " disposable FIRST, so the spare rows are the low numbers. Every other row"
         " is marked {NOT spare: ...} and costs you whatever that row's own verdict"
         " says it does. Take a {NOT spare: ...} row"
      << (over == 1 ? "" : " for any of these discards")
      << " only when your plan names the reason.\n";
    return o.str();
}


//The other side of the polarity. A row that is not a spare land says so, in the
//same vocabulary, so the unmarked row is no longer the neutral-looking one.
string discardNotSpareTag(bool anySpare, bool isSpare)
{
    if (!anySpare || isSpare)
        return "";
    return " {NOT spare: a real card, not a surplus land}";
}


//W50-W (D4): the cleanup discard ask. Header is a free function so PARSETEST
//can pin its numbers without a game.
//#W60-N (B5, wave-59 deck125 HIGH-1): the header did the CR 514.1 arithmetic
//correctly and said nothing about the 44 life the discards were about to cost -
//`125v162` seq 112 discarded 11 cards into two Liliana's Caress at 13 life.
//The price is a fact about this ask, so it rides this ask. `perDiscard`/
//`punishers` default to "no punisher", which renders the old bytes exactly;
//`life` < 0 means "not supplied", the same convention as every other tag.
string cleanupDiscardHeaderText(int handN, int limit, int over, int perDiscard,
                                const string& punishers, int life)
{
    std::ostringstream o;
    o << "Cleanup step (CR 514.1): your hand has " << handN << " card" << (handN == 1 ? "" : "s")
      << " and your maximum hand size is " << limit << ", so you must discard exactly "
      << over << " card" << (over == 1 ? "" : "s") << " now.";
    if (perDiscard > 0 && !punishers.empty() && over > 0)
    {
        const int cost = over * perDiscard;
        o << " The opponent's " << punishers << punisherVerb(punishers) << " every discard"
             " for " << perDiscard << " life each, so these " << over << " discard"
          << (over == 1 ? "" : "s") << " cost you " << cost << " life";
        if (life >= 0)
        {
            o << " - you would be at " << (life - cost);
            if (life - cost <= 0)
                o << "; this KILLS you";
        }
        o << ". You cannot decline the discard; you choose only WHICH cards go.";
    }
    o << " Name EXACTLY " << over
      << " card number" << (over == 1 ? "" : "s") << " - this is the ONLY ask for them;"
         " keep the cards your plan needs and discard what you can spare.\n";
    return o.str();
}


string AIPlayerGPT::buildCleanupDiscardAskText(const vector<MTGCardInstance*>& hand, int limit, int over,
                                              vector<size_t> * outOrder)
{
    std::ostringstream tail;
    //#W66-AT (H5): the header is composed here and EMITTED below, after the rows
    //have been classified - the default sentence it now carries names how many
    //spare rows the list holds, which is not known until the rows exist. Nothing
    //else writes to `tail` in between, so the emitted order is unchanged.
    string headerText;
    {
        //#W60-N (B5): the price of the discard this ask is about to force.
        vector<string> mineD, theirsD;
        int minePerD = 0, theirsPerD = 0;
        discardPunisherScan(this, opponent(), mineD, minePerD, theirsD, theirsPerD);
        std::ostringstream dn;
        for (size_t di = 0; di < theirsD.size(); di++)
            dn << (di ? ", " : "") << theirsD[di];
        headerText = cleanupDiscardHeaderText((int) hand.size(), limit, over, theirsPerD,
                                              dn.str(), life);
    }
    if (outOrder)
    {
        outOrder->resize(hand.size());
        for (size_t oi = 0; oi < hand.size(); oi++)
            (*outOrder)[oi] = oi;
    }
    //#W55-D (D9): the board facts the three clauses are computed off, read
    //ONCE for the whole list.
    int myLands = 0;
    std::set<string> myBattlefieldNames;
    for (int bi = 0; game && game->inPlay && bi < game->inPlay->nb_cards; bi++)
    {
        MTGCardInstance * bc = game->inPlay->cards[bi];
        if (!bc)
            continue;
        if (bc->hasType(Subtypes::TYPE_LAND))
            myLands++;
        if (!bc->isToken)
            myBattlefieldNames.insert(bc->name);
    }
    //#W61-T (C11): the other half of the spare comparison - the most expensive
    //card this hand still has to pay for. Lands and cards with no cost carry no
    //demand; a hand with no such card leaves the verdict unavailable (the count
    //still prints).
    int highestHandCost = -1;
    bool haveHandCost = false;
    //#W63-AC (E16): a cost this seat cannot reach is not the demand the next
    //land drop answers. A card above the reach cap is EXCLUDED from the
    //comparison and NAMED on the row instead of silently setting the bar.
    //#W63-AF (R3, wave-63 codex review finding 3): AND THE CAP IS COUNTED, NOT
    //GUESSED. `myLands + 2` was a heuristic with no board fact behind it, and
    //the clause it feeds already claims to be "what N lands plus the drops this
    //hand can still make would pay for" - so at eleven lands holding two more,
    //a legal fourteen-drop was declared out of reach and every land was called
    //spare. The drops this hand can still make are the LAND CARDS IN IT; that
    //is the number the sentence has been promising all along.
    int handLandCards = 0;
    for (size_t lc = 0; lc < hand.size(); lc++)
        if (hand[lc] && hand[lc]->hasType(Subtypes::TYPE_LAND))
            handLandCards++;
    const int reachCap = spareReachCap(myLands, handLandCards);
    //#W81-DL (V13): the COLOUR half of the spare verdict. Two tallies over the
    //same five colours: how many LAND sources of each the seat controls (read
    //with `landColorFlags`, the engine-read the hand header and the per-card
    //land tag are both printed from, so the three cannot disagree), and the most
    //of each colour a single REACHABLE card in hand demands. A card past the
    //reach cap sets no demand here for the same reason it sets none for the cost
    //comparison above.
    int w81ColourSources[5] = { 0, 0, 0, 0, 0 };
    int w81ColourNeed[5] = { 0, 0, 0, 0, 0 };
    static const char * kW81ColourSym[5] = { "{W}", "{U}", "{B}", "{R}", "{G}" };
    static const int kW81ColourIdx[5] = { Constants::MTG_COLOR_WHITE,
                                          Constants::MTG_COLOR_BLUE,
                                          Constants::MTG_COLOR_BLACK,
                                          Constants::MTG_COLOR_RED,
                                          Constants::MTG_COLOR_GREEN };
    for (int bi2 = 0; game && game->inPlay && bi2 < game->inPlay->nb_cards; bi2++)
    {
        bool have[5];
        landColorFlags(game->inPlay->cards[bi2], have);
        for (int ci2 = 0; ci2 < 5; ci2++)
            if (have[ci2])
                w81ColourSources[ci2]++;
    }
    for (size_t hn = 0; hn < hand.size(); hn++)
    {
        if (!hand[hn] || hand[hn]->hasType(Subtypes::TYPE_LAND) || !hand[hn]->getManaCost())
            continue;
        ManaCost * hc2 = hand[hn]->getManaCost();
        if (hc2->getConvertedCost() <= 0 || hc2->getConvertedCost() > reachCap)
            continue;
        for (int ci2 = 0; ci2 < 5; ci2++)
        {
            const int n = hc2->getCost(kW81ColourIdx[ci2]);
            if (n > w81ColourNeed[ci2])
                w81ColourNeed[ci2] = n;
        }
    }
    string beyondName;
    int beyondCost = 0;
    for (size_t hc = 0; hc < hand.size(); hc++)
    {
        if (!hand[hc] || hand[hc]->hasType(Subtypes::TYPE_LAND) || !hand[hc]->getManaCost())
            continue;
        int cc = hand[hc]->getManaCost()->getConvertedCost();
        if (cc <= 0)
            continue;
        if (cc > reachCap)
        {
            if (cc > beyondCost)
            {
                beyondCost = cc;
                beyondName = hand[hc]->name;
            }
            continue;
        }
        haveHandCost = true;
        if (cc > highestHandCost)
            highestHandCost = cc;
    }
    vector<string> discardRows;
    vector<int> discardClass; //#W66-AT (H5): parallel to discardRows
    bool anyVerdict = false; //#W57-C (D8): does the legend below have anything to explain?
    for (size_t j = 0; j < hand.size(); j++)
    {
        int copies = 0;
        int rank = listCopyRank(hand, j, copies);
        std::ostringstream row;
        row << hand[j]->name << copyOfTag(rank, copies, "your hand");
        if (hand[j]->getManaCost())
        {
            string mc = hand[j]->getManaCost()->toString();
            if (!mc.empty())
                row << " " << mc;
        }
        if (hand[j]->isCreature())
            row << " (" << hand[j]->power << "/" << hand[j]->toughness << " creature)";
        else
        {
            string tt = typeTag(hand[j]);
            if (!tt.empty())
                row << " (" << tt << ")";
        }
        string txt = cardTextSnippet(hand[j], 140);
        if (!txt.empty())
            row << " {card text: " << txt << "}";
        //#W55-D (D9): the verdicts. A land row says how many lands are already
        //down; a permanent whose name is already on the battlefield says so;
        //a spell whose own target spec sees nothing on the board says that,
        //and a spell that targets the STACK is never given the clause.
        if (hand[j]->hasType(Subtypes::TYPE_LAND))
        {
            //#W81-DL (V13): does discarding THIS land leave a reachable card in
            //hand short of a colour? The worst-bound colour wins the clause -
            //one sentence, the shape the family already uses.
            string blockColour;
            int blockNeed = 0, blockAfter = 0;
            {
                bool have[5];
                landColorFlags(hand[j], have);
                int worst = -1;
                for (int ci3 = 0; ci3 < 5; ci3++)
                {
                    if (!have[ci3] || w81ColourNeed[ci3] <= 0)
                        continue;
                    //this land is not on the battlefield, so the sources left if
                    //it is discarded are the ones already down.
                    const int after = w81ColourSources[ci3];
                    if (after >= w81ColourNeed[ci3])
                        continue;
                    const int shortfall = w81ColourNeed[ci3] - after;
                    if (worst < 0 || shortfall > w81ColourNeed[worst] - w81ColourSources[worst])
                        worst = ci3;
                }
                if (worst >= 0)
                {
                    blockColour = kW81ColourSym[worst];
                    blockNeed = w81ColourNeed[worst];
                    blockAfter = w81ColourSources[worst];
                    //#W81-DL (V13): staged only where the COUNT gate would have
                    //printed the verdict word, so the counter names exactly the
                    //population this item changed.
                    if (haveHandCost && highestHandCost >= 0
                        && myLands > highestHandCost + 1
                        && mW81PendingEventFace.find("spare_colour_withheld")
                           == string::npos)
                        mW81PendingEventFace += "spare_colour_withheld;";
                }
            }
            row << discardSpareLandClause(myLands, highestHandCost, haveHandCost,
                                         beyondName, beyondCost, reachCap, //#W63-AC (E16)
                                         blockColour, blockNeed, blockAfter); //#W81-DL (V13)
        }
        else
        {
            if (myBattlefieldNames.count(hand[j]->name))
                row << discardAlreadyControlClause(hand[j]->name);
            string spec = hand[j]->spellTargetType;
            if (!spec.empty() && toLowerCopy(spec).find("stack") == string::npos)
            {
                int n = modalSpecObjectCount(observer, hand[j], spec);
                if (n >= 0)
                {
                    string dt = discardDeadTargetClause(n);
                    if (!dt.empty())
                    {
                        row << dt;
                        anyVerdict = true;
                    }
                }
            }
            //#W57-C (D8): and the cast row's own board evaluator, re-badged into
            //the same family - this is the half that reaches the sweepers and
            //the edicts, which is where the 357 bare rows are.
            string bv = discardBoardVerdictTag(dynamicMagnitudes(hand[j]));
            if (!bv.empty())
            {
                row << bv;
                anyVerdict = true;
            }
        }
        discardRows.push_back(row.str());
        //#W66-AT (H5): the row's disposability, read off the verdicts the row
        //itself just printed.
        discardClass.push_back(discardDisposabilityClass(hand[j]->hasType(Subtypes::TYPE_LAND),
                                                         discardRows.back()));
    }
    {
        //#W66-AT (H5): order by disposability, ties keeping hand order (a stable
        //selection over the class vector). This is a PERMUTATION - every row is
        //still on the list, and `dispOrder` is composed with the copy-collapse
        //order below so the number the model answers with still resolves to the
        //hand position it names.
        int spareRows = 0;
        for (size_t c = 0; c < discardClass.size(); c++)
            if (discardClass[c] == 0)
                spareRows++;
        vector<size_t> dispOrder;
        discardDisposabilityOrder(discardClass, dispOrder);
        //#W66-AT (H5): the polarity marker, applied before the permutation so a
        //row and its class can never come apart.
        for (size_t c = 0; c < discardRows.size(); c++)
            discardRows[c] += discardNotSpareTag(spareRows > 0, discardClass[c] == 0);
        vector<string> orderedRows;
        orderedRows.reserve(dispOrder.size());
        for (size_t k = 0; k < dispOrder.size(); k++)
            orderedRows.push_back(discardRows[dispOrder[k]]);
        //#W55-D (D18): the same collapse every other list gets - three copies of
        //one card in hand printed three byte-different rows only because of the
        //copy ordinal. The caller un-permutes the reply.
        vector<size_t> groupOrder;
        groupNumberedRows(orderedRows, groupOrder);
        vector<string> shownDiscard;
        shownDiscard.reserve(orderedRows.size());
        for (size_t k = 0; k < groupOrder.size(); k++)
            shownDiscard.push_back(orderedRows[groupOrder[k]]);
        //#W66-AT (H5): composition - printed position k is disposability
        //position groupOrder[k], which is hand position dispOrder[...].
        vector<size_t> discardOrder;
        composeRowOrder(dispOrder, groupOrder, discardOrder);
        bool discardRanged = false;
        unsigned discardForms = 0; //#W82-P8
        tail << headerText;
        tail << discardSpareDefaultLine(spareRows, over); //#W66-AT (H5)
        tail << joinNumberedRows(shownDiscard, &discardRanged, &discardForms);
        if (discardRanged)
            tail << optionRangeNote(discardForms);
        tail << discardVerdictLegend(anyVerdict); //#W57-C (D8)
        if (outOrder)
            *outOrder = discardOrder;
    }
    tail << kPlanFirstLead //#W70-BL (E2)
         << "on a line of its own PUT: followed by the " << over << " card number"
         << (over == 1 ? "" : "s") << " you discard"
         << (over == 1 ? " (e.g. \"PUT: 2\")" : ", comma-separated (e.g. \"PUT: 2, 5\")")
         //#W60-M (B3): the ask never said the numbers had to differ, and
         //"Name EXACTLY 16 card numbers" admits the reading the model used -
         //125v126 seq 246 wrote 16 numbers of which two were `19`, and seq 354
         //answered a 2-card ask with `PUT: 4, 4`. Stated here, once, in the
         //sentence that asks for them.
         << (over == 1 ? "" : "; the numbers must all be DIFFERENT (a number written twice"
                              " counts once)")
         << ". Write nothing else."; //#W70-BL (E2)
    return tail.str();
}


int AIPlayerGPT::cleanupDiscard(int over)
{
    if (over <= 0)
        return 0;
    vector<MTGCardInstance*> hand;
    for (int i = 0; i < game->hand->nb_cards; i++)
        hand.push_back(game->hand->cards[i]);
    if ((int) hand.size() < over)
        over = (int) hand.size();
    int limit = handsize + handmodifier;
    if (limit < 0)
        limit = 0;
    string userMsg, content, askText;
    int result = -1;
    int repeatedIdx = 0;   //#W60-M (B3)
    bool reasked = false;  //#W60-M (B3)
    vector<bool> send;
    vector<string> names;
    //#W55-D (D18): PRINTED order - filled after the ask text is built, which is
    //what fixes the permutation.
    vector<size_t> discardOrder(hand.size(), 0);
    for (size_t j = 0; j < hand.size(); j++)
        discardOrder[j] = j;
    if (!mEndpoint.empty())
    {
        if (mSystemPrompt.empty())
            buildSystemPrompt();
        mLogWindowKind = kAskWindowCleanupDiscard; //#W57-H (D43)
        askText = buildCleanupDiscardAskText(hand, limit, over, &discardOrder);
        //#W60-M (B3): this hand+limit already earned its one re-ask; the
        //corrected question is THE question from here on (the #W49-S D8 idiom
        //at the ask seam, applied to a channel that had no recovery window at
        //all - the prompt itself says "this is the ONLY ask for them").
        reasked = (!mDiscardReaskKey.empty() && mDiscardReaskKey == askText);
        userMsg = assemblePrompt(reasked ? askText + "\n" + mDiscardReaskLine : askText);
        if (discardOrder.size() != hand.size())
        {
            discardOrder.resize(hand.size());
            for (size_t j = 0; j < hand.size(); j++)
                discardOrder[j] = j;
        }
    }
    for (size_t j = 0; j < hand.size(); j++)
        names.push_back(hand[discardOrder[j]]->name);
    if (!mEndpoint.empty())
    {
        setAnswerFloorForSeam("discard", (long) hand.size(), kPutSlotAnswerBytes); //#W71-BS (F5)
        if (pollCompletionRetry(userMsg, content, "discard") == kChoicePending)
            return 1; //call in flight; the base Act neither acts nor passes
        //#W52-G (E-1): the discard ask's own label. deck162 vs deck146 seq 17:
        //"PUT: 3 (Forced Fruition)" twice, then a stray "CHOICE: 2 (Cast
        //Liliana's Caress)" as the reply's LAST coded line - the any-label walk
        //took the CHOICE line and the seat discarded Liliana's Caress.
        string decisionPart = consumePlan(content, "PUT:");
        result = content.empty() ? -1
                 : parseAttackerSet(decisionPart, hand.size(), send, &names, true, &repeatedIdx);
        if (result < 0 && !content.empty())
        {
            int sal = salvageLoopedSubset(content, "PUT:", hand.size(), names, send);
            if (sal >= 1)
                result = sal;
        }
        //#W60-M (B3): a repeated index is DEDUPED, never a rejection. When the
        //distinct set is still complete the answer stands and says why in the
        //record; when the repeat left the list SHORT (125v126 seq 246: 16
        //numbers, 15 distinct, on a 16-card discard) the seat buys ONE re-ask
        //that names the fault, rather than handing 1-16ths of a permanent,
        //unrecoverable decision to the heuristic with no receipt.
        if (repeatedIdx > 0)
            appendParseNote(&mLastParseNote, result >= over ? "duplicate_index_deduped"
                                                            : "duplicate_index_short");
        //#W71-BS (F6, Astra review finding 6): the `distinct_index_reask` is
        //RESTORED. Zero occurrences in one corpus is not evidence that removing it
        //is behaviour-neutral, because what it guarded is IRREVERSIBLE: a short
        //PUT list (`PUT: 1, 2, 2` on a discard of three) now falls straight into
        //the fill loop below, which discards the highest-mana-value card the model
        //did NOT name - on the shape the review cites, the finisher it wrote its
        //PLAN around. One labelled-line re-ask that states the arithmetic ("your
        //PUT line names N distinct cards; this discard needs M") costs one round
        //trip and asks the model to complete a selection it started. It reads
        //nothing but the PUT: line, it licenses no prose, and it fires ONCE per ask
        //text; the fill loop is still the floor behind it.
        if (repeatedIdx > 0 && result >= 0 && result < over && !reasked)
        {
            std::ostringstream corr;
            corr << "[RE-ASK] Your PUT: line repeated a card number, so it named only "
                 << result << " different card" << (result == 1 ? "" : "s")
                 << " and this discard needs " << over << ". Every number must be DIFFERENT."
                 << " Answer again with " << over << " different card numbers from the list above.";
            mDiscardReaskKey = askText;
            mDiscardReaskLine = corr.str();
            writeTransLog("discard", userMsg, content, result, (int) hand.size(), "",
                          "distinct_index_reask", &names);
            setNotice("that discard list repeated a number - asking again", 5.0f);
            DebugTrace("AIPlayerGPT: cleanup discard named " << result << " distinct of "
                       << over << " (" << repeatedIdx << " repeated) - re-asking once");
            string corrected;
            pollCompletionRetry(assemblePrompt(askText + "\n" + mDiscardReaskLine), corrected, "discard");
            return 1; //the caller unwinds; the corrected call answers later
        }
        if (reasked)
            appendParseNote(&mLastParseNote, result >= over ? "distinct_index_reask_recovered"
                                                            : "distinct_index_reask_exhausted");
    }
    //#W55-D (D18): back from PRINTED positions to hand positions.
    if (result >= 0 && !send.empty())
        unpermuteSelection(discardOrder, hand.size(), send);
    vector<MTGCardInstance*> chosen;
    string chosenText;
    if (result >= 0)
        for (size_t j = 0; j < hand.size() && (int) chosen.size() < over; j++)
            if (j < send.size() && send[j])
            {
                chosen.push_back(hand[j]);
                chosenText += (chosenText.empty() ? "" : ", ") + hand[j]->name;
            }
    bool heuristic = (result < 0 || (int) chosen.size() < over);
    //Exactly `over` cards: under-picks/failures fill from the base policy
    //(highest mana value first), announced like every other fallback.
    while ((int) chosen.size() < over)
    {
        MTGCardInstance * fill = NULL;
        int fc = -1;
        for (size_t i = 0; i < hand.size(); i++)
        {
            MTGCardInstance * c = hand[i];
            bool already = false;
            for (size_t k = 0; k < chosen.size(); k++)
                if (chosen[k] == c) { already = true; break; }
            if (already)
                continue;
            int cost = c->getManaCost() ? c->getManaCost()->getConvertedCost() : 0;
            if (cost > fc) { fc = cost; fill = c; }
        }
        if (!fill)
            break;
        chosen.push_back(fill);
        chosenText += (chosenText.empty() ? "" : ", ") + fill->name;
    }
    if (!mEndpoint.empty())
    {
        if (heuristic)
            noticeFallback("model reply failed - the heuristic discards to hand size", 5.0f);
        writeTransLog("discard", userMsg, content, result, (int) hand.size(), chosenText,
                      heuristic ? (content.empty() ? noAnswerClass() : "unparsed_reply") : NULL,
                      &names);
    }
    std::ostringstream narr;
    narr << "Cleanup discard (hand " << hand.size() << ", limit " << limit << "): "
         << (heuristic ? "the heuristic chose " : "you chose ") << chosenText;
    narrateDecision(narr.str());
    mDiscardReaskKey.clear();  //#W60-M (B3): spent with this discard
    mDiscardReaskLine.clear();
    cleanupDiscardCards(chosen);
    return 1;
}



//#W63-AF (R8): external door onto the pending-draw scan for the test suite.
//Defined out here, at file scope, because the scan itself lives in this file's
//anonymous namespace and would otherwise have internal linkage.
int gptStackPendingDrawsFor(GameObserver * observer, Player * seat, MTGCardInstance * exclude)
{
    return stackPendingDrawsFor(observer, seat, exclude);
}


//#W72-BW (M3): the same door onto the hand-replacement scan.
std::string gptStackHandReplacerFor(GameObserver * observer, Player * seat)
{
    return stackHandReplacerFor(observer, seat);
}


//Free-function entry so the JGE layer's main() can trigger the self-test
//without including the mtg-heavy AIPlayerGPT header.
void wagicGptParseSelfTest()
{
    AIPlayerGPT::runParseSelfTest();
}

#if defined(WAGIC_GPT_PARSETEST_BUILD)
string AIPlayerGPTSelfTestAccess::afterCombatBlockerCostText(int availableBlockers) { return ::afterCombatBlockerCostText(availableBlockers); }
string AIPlayerGPTSelfTestAccess::altCostPrintedNote(const string& liveCost, const string& printedCost) { return ::altCostPrintedNote(liveCost, printedCost); }
bool AIPlayerGPTSelfTestAccess::annotateEtbPayOrTapMenu(vector<string>& opts, const string& landName, bool alreadyTapped, bool canTapForMana, bool canAttackEver) { return ::annotateEtbPayOrTapMenu(opts, landName, alreadyTapped, canTapForMana, canAttackEver); }
string AIPlayerGPTSelfTestAccess::announceXHeader(const string& spell, int capX, bool canDecline, const string& libraryNote) { return ::announceXHeader(spell, capX, canDecline, libraryNote); }
string AIPlayerGPTSelfTestAccess::askExemplar(const vector<string>& options, int * usedRow) { return ::askExemplar(options, usedRow); }
string AIPlayerGPTSelfTestAccess::attackerDestinationTag(bool mayAttackPlayer, bool mayAttackTarget) { return ::attackerDestinationTag(mayAttackPlayer, mayAttackTarget); }
int AIPlayerGPTSelfTestAccess::becomesBlockedSelfPump(const string& text, int& dp, int& dt) { return ::becomesBlockedSelfPump(text, dp, dt); }
int AIPlayerGPTSelfTestAccess::blockerLifelinkGain(int blkPower, int blkToughness, bool blkLifelink, bool blkFirstStrike, int atkPower, bool atkFirstStrike, bool atkDeathtouch, bool blkDoubleStrike) { return ::blockerLifelinkGain(blkPower, blkToughness, blkLifelink, blkFirstStrike, atkPower, atkFirstStrike, atkDeathtouch, blkDoubleStrike); }
bool AIPlayerGPTSelfTestAccess::blockingLifeTriggerClause(const string& lowLine, string& sourceSpec, string& amountExpr, bool& optional) { return ::blockingLifeTriggerClause(lowLine, sourceSpec, amountExpr, optional); }
string AIPlayerGPTSelfTestAccess::blockingTriggerTotalLine(int triggered, int sure, int may, bool uniform, int totalGain, bool converter, int oppLife) { return ::blockingTriggerTotalLine(triggered, sure, may, uniform, totalGain, converter, oppLife); }
string AIPlayerGPTSelfTestAccess::buildHandGainAsk(const string& where, const string& effectName, bool multi, bool unlimited, bool targetMin, int maxtargets, size_t pickIndex) { return ::buildHandGainAsk(where, effectName, multi, unlimited, targetMin, maxtargets, pickIndex); }
string AIPlayerGPTSelfTestAccess::buildHandRemovalAsk(const string& verb, bool byOpponent, bool relocate, const string& effectName, bool multi, bool unlimited, bool targetMin, int maxtargets, size_t pickIndex) { return ::buildHandRemovalAsk(verb, byOpponent, relocate, effectName, multi, unlimited, targetMin, maxtargets, pickIndex); }
string AIPlayerGPTSelfTestAccess::buildMayObjectAsk(const string & srcName, const string & objName, const string & objOrigin, vector<string> & opts) { return ::buildMayObjectAsk(srcName, objName, objOrigin, opts); }
string AIPlayerGPTSelfTestAccess::buildRevealAskText(const vector<MTGCardInstance*>& revealed, const string& optOneLabel, const string& optTwoLabel, const string& optOneEffect, const vector<bool>& eligibleForOptionOne, int revealSource, bool pickExactlyOne, bool singlePickBare, bool singlePickDeclineLegal, bool wholeLibrary, vector<size_t> * outOrder, Player * seat) { return ::buildRevealAskText(revealed, optOneLabel, optTwoLabel, optOneEffect, eligibleForOptionOne, revealSource, pickExactlyOne, singlePickBare, singlePickDeclineLegal, wholeLibrary, outOrder, seat); }
string AIPlayerGPTSelfTestAccess::castModeAltPriceTag(const std::vector<std::string>& labels, const std::vector<std::string>& costs, const std::vector<int>& draws) { return ::castModeAltPriceTag(labels, costs, draws); }
string AIPlayerGPTSelfTestAccess::castModeCastPriceTag(const string& castCost, const string& castClause, int legalTargets) { return ::castModeCastPriceTag(castCost, castClause, legalTargets); }
string AIPlayerGPTSelfTestAccess::castModeCommitmentNote(bool castModeMenu, const string& ctxName, const string& committedName, bool sameTurn) { return ::castModeCommitmentNote(castModeMenu, ctxName, committedName, sameTurn); }
string AIPlayerGPTSelfTestAccess::castModeRowTag(const string& costBody, const string& clause) { return ::castModeRowTag(costBody, clause); }
string AIPlayerGPTSelfTestAccess::cleanupDiscardHeaderText(int handN, int limit, int over, int perDiscard, const string& punishers, int life) { return ::cleanupDiscardHeaderText(handN, limit, over, perDiscard, punishers, life); }
void AIPlayerGPTSelfTestAccess::collectLabeledLines(const string& content, const char * label, vector<string>& out, vector<string> * prevOut, vector<vector<string> > * windowOut) { ::collectLabeledLines(content, label, out, prevOut, windowOut); }
bool AIPlayerGPTSelfTestAccess::combatLineIsClean(const string& line, const vector<string> * rosterA, const vector<string> * rosterB) { return ::combatLineIsClean(line, rosterA, rosterB); }
string AIPlayerGPTSelfTestAccess::combatTradePreviewStats(const CombatTradeStat& b, const CombatTradeStat& a, int preventAtoB, int preventBtoA, int preventAtoFace, bool attackerSeat, int bRemaining, bool bGainConverted, string * outBlockTrigger, bool * outBlockerDies, string * outBlockerLifelink, string * outAttackerLifelink, bool * outAttackerDies, bool foeLifeLoop) { return ::combatTradePreviewStats(b, a, preventAtoB, preventBtoA, preventAtoFace, attackerSeat, bRemaining, bGainConverted, outBlockTrigger, outBlockerDies, outBlockerLifelink, outAttackerLifelink, outAttackerDies, foeLifeLoop); }
string AIPlayerGPTSelfTestAccess::markedDamageTag(int toughness, int life) { return ::markedDamageTag(toughness, life); } //#W82-EC (H3)
string AIPlayerGPTSelfTestAccess::attackerLifelinkAttackLineTag(int dealt, bool doublestrike) { return ::attackerLifelinkAttackLineTag(dealt, doublestrike); } //#W82-EC (H4)
void AIPlayerGPTSelfTestAccess::composeRowOrder(const std::vector<size_t>& outer, const std::vector<size_t>& inner, std::vector<size_t>& out) { ::composeRowOrder(outer, inner, out); }
string AIPlayerGPTSelfTestAccess::compoundModeTargetNote(const string& modeName) { return ::compoundModeTargetNote(modeName); }
int AIPlayerGPTSelfTestAccess::countLegalAssignments(const vector<int>& pick, size_t nAttackers, const vector<vector<int> >& legalPerBlocker) { return ::countLegalAssignments(pick, nAttackers, legalPerBlocker); }
string AIPlayerGPTSelfTestAccess::discardAlreadyControlClause(const string& onBattlefield) { return ::discardAlreadyControlClause(onBattlefield); }
string AIPlayerGPTSelfTestAccess::discardBoardVerdictTag(const string& rightNowClause) { return ::discardBoardVerdictTag(rightNowClause); }
string AIPlayerGPTSelfTestAccess::discardDeadTargetClause(int legalTargets) { return ::discardDeadTargetClause(legalTargets); }
int AIPlayerGPTSelfTestAccess::discardDisposabilityClass(bool isLand, const string& row) { return ::discardDisposabilityClass(isLand, row); }
void AIPlayerGPTSelfTestAccess::discardDisposabilityOrder(const std::vector<int>& classes, std::vector<size_t>& order) { ::discardDisposabilityOrder(classes, order); }
string AIPlayerGPTSelfTestAccess::discardNotSpareTag(bool anySpare, bool isSpare) { return ::discardNotSpareTag(anySpare, isSpare); }
string AIPlayerGPTSelfTestAccess::discardSpareDefaultLine(int spareRows, int over) { return ::discardSpareDefaultLine(spareRows, over); }
string AIPlayerGPTSelfTestAccess::discardSpareLandClause(int myLands, int highestHandCost, bool haveHandCost, const string& beyondName, int beyondCost, int reachCap, const string& blockColour, int blockNeed, int blockAfter) { return ::discardSpareLandClause(myLands, highestHandCost, haveHandCost, beyondName, beyondCost, reachCap, blockColour, blockNeed, blockAfter); }
string AIPlayerGPTSelfTestAccess::discardVerdictLegend(bool anyVerdict) { return ::discardVerdictLegend(anyVerdict); }
string AIPlayerGPTSelfTestAccess::etbUsableClause(bool canTapForMana, bool canAttackEver) { return ::etbUsableClause(canTapForMana, canAttackEver); }
string AIPlayerGPTSelfTestAccess::exampleCoreTrim(const string& coreIn, size_t maxLen) { return ::exampleCoreTrim(coreIn, maxLen); }
string AIPlayerGPTSelfTestAccess::exemplarSentence(const string& exemplarText, int exemplarRow) { return ::exemplarSentence(exemplarText, exemplarRow); }
string AIPlayerGPTSelfTestAccess::handRemovalVerb(const string& lc, bool& relocate, const string& gainDest) { return ::handRemovalVerb(lc, relocate, gainDest); }
string AIPlayerGPTSelfTestAccess::hybridPipNoteText(const vector<string>& pips, int totalMana) { return ::hybridPipNoteText(pips, totalMana); }
bool AIPlayerGPTSelfTestAccess::isAddNCountersOption(const string & optRaw) { return ::isAddNCountersOption(optRaw); }
string AIPlayerGPTSelfTestAccess::landEntersTappedTag(const string& script, const string& printedText) { return ::landEntersTappedTag(script, printedText); }
string AIPlayerGPTSelfTestAccess::landTapTagFor(const string& script, const string& printedText, int myLands, const vector<string>& witness, const vector<char> * witnessPrinted) { return ::landTapTagFor(script, printedText, myLands, witness, witnessPrinted); }
string AIPlayerGPTSelfTestAccess::lifeLoopAttackerRowTag(bool reachable, bool attackProvenLethal) { return ::lifeLoopAttackerRowTag(reachable, attackProvenLethal); }
bool AIPlayerGPTSelfTestAccess::lifeLoopWinnable(bool loopClosed, bool theirLifeCanChange, bool iCanGainLife) { return ::lifeLoopWinnable(loopClosed, theirLifeCanChange, iCanGainLife); }
bool AIPlayerGPTSelfTestAccess::lifeLossMirrorScript(const string& magicText) { return ::lifeLossMirrorScript(magicText); }
string AIPlayerGPTSelfTestAccess::lifePaymentVerdict(int life, int cost) { return ::lifePaymentVerdict(life, cost); }
int AIPlayerGPTSelfTestAccess::lifeToDamageConverterRate(const string& magicText, bool& proportional) { return ::lifeToDamageConverterRate(magicText, proportional); }
bool AIPlayerGPTSelfTestAccess::lifeToDamageConverterScript(const string& magicText) { return ::lifeToDamageConverterScript(magicText); }
int AIPlayerGPTSelfTestAccess::manaCostTextCmc(const string& costText) { return ::manaCostTextCmc(costText); }
string AIPlayerGPTSelfTestAccess::mdfcLandArrivalTagFrom(const string& script, const string& printedText) { return ::mdfcLandArrivalTagFrom(script, printedText); }
string AIPlayerGPTSelfTestAccess::moverGainDestination(const string& dest) { return ::moverGainDestination(dest); }
string AIPlayerGPTSelfTestAccess::mulliganKeepClause(int mullsTaken, int keepSize) { return ::mulliganKeepClause(mullsTaken, keepSize); }
string AIPlayerGPTSelfTestAccess::mulliganRowLabel(int next) { return ::mulliganRowLabel(next); }
string AIPlayerGPTSelfTestAccess::mutateAltCostLabel(bool hasMutate, const string& alternativeName) { return ::mutateAltCostLabel(hasMutate, alternativeName); }
string AIPlayerGPTSelfTestAccess::mutateHostAsk(const string& effectName, int placement) { return ::mutateHostAsk(effectName, placement); }
string AIPlayerGPTSelfTestAccess::mutateOverUnderHeader(const string& ctxName) { return ::mutateOverUnderHeader(ctxName); }
int AIPlayerGPTSelfTestAccess::mutatePlacementFromLabel(const string& s) { return ::mutatePlacementFromLabel(s); }
int AIPlayerGPTSelfTestAccess::nameOrdinal(const string& seg) { return ::nameOrdinal(seg); }
bool AIPlayerGPTSelfTestAccess::gptOrderLineFromReply(const string& content, size_t n, vector<int>& order, string * takenText) { return ::gptOrderLineFromReply(content, n, order, takenText); }
int AIPlayerGPTSelfTestAccess::parseAttackerSet(const string& content, size_t nAttackers, vector<bool>& out, const vector<string> * optionNames, bool echoBinds, int * repeatedOut) { return ::parseAttackerSet(content, nAttackers, out, optionNames, echoBinds, repeatedOut); }
int AIPlayerGPTSelfTestAccess::parseAttackerTargets(const string& line, size_t nAttackers, size_t nTargets, vector<int>& out) { return ::parseAttackerTargets(line, nAttackers, nTargets, out); }
int AIPlayerGPTSelfTestAccess::parseBlockAssignments(const string& content, size_t nBlockers, size_t nAttackers, vector<int>& out, const vector<string> * blockerNames, const vector<string> * attackerNames, const vector<vector<int> > * legalPerBlocker, int * dropped, bool * gangConflict, bool * blockerInAttackerSlot) { return ::parseBlockAssignments(content, nBlockers, nAttackers, out, blockerNames, attackerNames, legalPerBlocker, dropped, gangConflict, blockerInAttackerSlot); }
bool AIPlayerGPTSelfTestAccess::payLifeAmountFromOption(const string & opt, int & amount) { return ::payLifeAmountFromOption(opt, amount); }
string AIPlayerGPTSelfTestAccess::payRepeatBandRowTag(int bandPaid) { return ::payRepeatBandRowTag(bandPaid); }
int AIPlayerGPTSelfTestAccess::payRepeatCollapse(const vector<string>& optionTexts, const vector<int>& paid, vector<bool>& bandRows, int& bandLow, int& bandHigh, int& bandPaid) { return ::payRepeatCollapse(optionTexts, paid, bandRows, bandLow, bandHigh, bandPaid); }
string AIPlayerGPTSelfTestAccess::payRepeatCollapseTag(int bandLow, int bandHigh, int bandPaid) { return ::payRepeatCollapseTag(bandLow, bandHigh, bandPaid); }
string AIPlayerGPTSelfTestAccess::payRepeatModeNote(const vector<string>& opts) { return ::payRepeatModeNote(opts); }
string AIPlayerGPTSelfTestAccess::payRepeatPerCounterCost(const string& magicText, const string& optionLabel) { return ::payRepeatPerCounterCost(magicText, optionLabel); }
string AIPlayerGPTSelfTestAccess::payRepeatRowCostTag(int counters, const string& perCost, int perCmc, int available, int paidCopies) { return ::payRepeatRowCostTag(counters, perCost, perCmc, available, paidCopies); }
string AIPlayerGPTSelfTestAccess::payRepeatTapsClause(const std::vector<std::string>& names, const std::vector<int>& restrictions, int paid, int counters, const string& untapSource) { return ::payRepeatTapsClause(names, restrictions, paid, counters, untapSource); }
string AIPlayerGPTSelfTestAccess::printedClauseFor(const string& cardText, const string& label) { return ::printedClauseFor(cardText, label); }
string AIPlayerGPTSelfTestAccess::printedFirstClause(const string& cardText) { return ::printedFirstClause(cardText); }
bool AIPlayerGPTSelfTestAccess::repeatAskAnswerStands(const string& heldKey, const string& nowKey, const string& heldPlan, const string& nowPlan, int heldTurn, int nowTurn, int heldChoice, int nOptions) { return ::repeatAskAnswerStands(heldKey, nowKey, heldPlan, nowPlan, heldTurn, nowTurn, heldChoice, nOptions); }
string AIPlayerGPTSelfTestAccess::repeatAskKey(int turn, int phase, const string& decision, const vector<string>& rows) { return ::repeatAskKey(turn, phase, decision, rows); }
bool AIPlayerGPTSelfTestAccess::replyTerminatedNaturally(const string& content, bool finishLength) { return ::replyTerminatedNaturally(content, finishLength); }
string AIPlayerGPTSelfTestAccess::revealCostTag(const string& printedCost, int manaValue) { return ::revealCostTag(printedCost, manaValue); }
string AIPlayerGPTSelfTestAccess::revealDestinationClause(const string& optOneLabel, const string& zone) { return ::revealDestinationClause(optOneLabel, zone); }
string AIPlayerGPTSelfTestAccess::revealDestinationZone(const string& effect) { return ::revealDestinationZone(effect); }
string AIPlayerGPTSelfTestAccess::revealDuplicateTag(bool legendary, int inMyHand, int onMyBattlefield) { return ::revealDuplicateTag(legendary, inMyHand, onMyBattlefield); }
string AIPlayerGPTSelfTestAccess::revealEligMarker(const string & optOneLabel) { return ::revealEligMarker(optOneLabel); }
bool AIPlayerGPTSelfTestAccess::revealSinglePickDeclineLegal(const string& targetSpec) { return ::revealSinglePickDeclineLegal(targetSpec); }
int AIPlayerGPTSelfTestAccess::salvageLoopedBlocks(const string& content, size_t nBlockers, size_t nAttackers, const vector<string>& blockerNames, const vector<string>& attackerNames, const vector<vector<int> >& legalIdx, vector<int>& out) { return ::salvageLoopedBlocks(content, nBlockers, nAttackers, blockerNames, attackerNames, legalIdx, out); }
int AIPlayerGPTSelfTestAccess::salvageLoopedSubset(const string& content, const char * label, size_t n, const vector<string>& names, vector<bool>& out) { return ::salvageLoopedSubset(content, label, n, names, out); }
string AIPlayerGPTSelfTestAccess::scriptAbilityCost(const string& script, const string& abilityName) { return ::scriptAbilityCost(script, abilityName); }
string AIPlayerGPTSelfTestAccess::scriptCostGloss(const string& costTokens) { return ::scriptCostGloss(costTokens); }
int AIPlayerGPTSelfTestAccess::spareReachCap(int myLands, int handLandCards) { return ::spareReachCap(myLands, handLandCards); }
string AIPlayerGPTSelfTestAccess::stripAnnotationBrackets(const string& s) { return ::stripAnnotationBrackets(s); }
string AIPlayerGPTSelfTestAccess::stripTrailingListGloss(const string& line, vector<string> * namesOut) { return ::stripTrailingListGloss(line, namesOut); }
string AIPlayerGPTSelfTestAccess::stripTrailingListGlossLines(const string& reply, vector<string> * namesOut) { return ::stripTrailingListGlossLines(reply, namesOut); }
string AIPlayerGPTSelfTestAccess::toLowerCopy(const string & s) { return ::toLowerCopy(s); }
string AIPlayerGPTSelfTestAccess::uniqueNamedIn(const string& text, const vector<string>& names) { return ::uniqueNamedIn(text, names); }
bool AIPlayerGPTSelfTestAccess::w74XPermuteToClimbing(vector<string>& shown, int capX) { return ::w74XPermuteToClimbing(shown, capX); }
int AIPlayerGPTSelfTestAccess::w74XPickToContractIndex(int pick, int capX, int rowCount, bool climbing, int optionCount) { return ::w74XPickToContractIndex(pick, capX, rowCount, climbing, optionCount); }
string AIPlayerGPTSelfTestAccess::w76CrossPhaseListKey(int turn, const string& decision, const vector<string>& rows) { return ::w76CrossPhaseListKey(turn, decision, rows); }
string AIPlayerGPTSelfTestAccess::w76CrossPhaseRePutNote(int windowsAgo, const string& phaseName, bool boardUnchanged) { return ::w76CrossPhaseRePutNote(windowsAgo, phaseName, boardUnchanged); }
string AIPlayerGPTSelfTestAccess::w76PhaseFreeBoardKey(const string& board) { return ::w76PhaseFreeBoardKey(board); }
string AIPlayerGPTSelfTestAccess::walkerOnlyExclusionLine(int walkerOnlyRows) { return ::walkerOnlyExclusionLine(walkerOnlyRows); }
string AIPlayerGPTSelfTestAccess::xAnnounceLibraryNote(int capX, int drawPerX, int library, int reserve, const string& reserveWhy, int libX) { return ::xAnnounceLibraryNote(capX, drawPerX, library, reserve, reserveWhy, libX); }
void AIPlayerGPTSelfTestAccess::w82PutOrderFromReply(const string& text, size_t handSize, std::vector<int>& order) { ::w82PutOrderFromReply(text, handSize, order); }
const char * AIPlayerGPTSelfTestAccess::kLifeLoopAttackerRowTag = ::kLifeLoopAttackerRowTag;
const int AIPlayerGPTSelfTestAccess::kPreventNone = ::kPreventNone;
const int AIPlayerGPTSelfTestAccess::kPreventFull = ::kPreventFull;
const int AIPlayerGPTSelfTestAccess::kPreventPartial = ::kPreventPartial;
const char * AIPlayerGPTSelfTestAccess::kRevealPlanScopeFact = ::kRevealPlanScopeFact;
#endif //WAGIC_GPT_PARSETEST_BUILD

#endif //WITH_GPT_AI
