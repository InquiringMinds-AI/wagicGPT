/*
 *  Wagic, The Homebrew ?! is licensed under the BSD license
 *  See LICENSE in the Folder's root
 *
 *  AIPlayerGPT: an LLM-backed opponent. Inherits all of AIPlayerBaka's
 *  game mechanics (mana payment, clicking, targeting, combat) and overrides
 *  the decision seam: when the engine has enumerated the legal ranked
 *  actions, the choice among them is delegated to an OpenAI-compatible
 *  chat-completions endpoint instead of the efficiency heuristic.
 *
 *  The player is a persistent agent, not a stateless picker. Every request
 *  is exactly two messages, head and tail:
 *  - HEAD (system, built once per duel): a rules/flow primer, both decks'
 *    card names and rules text (own deck with counts, the opponent's
 *    without, like a player who knows the matchup), an optional
 *    user-written strategy guide for the AI's deck
 *    (<deckfile>_strategy.txt next to the deck, e.g. ai/baka/deck1_strategy.txt),
 *    and the reply protocol.
 *  - TAIL (user, rebuilt per decision): the game narration (events via
 *    receiveEvent() plus the model's own past decisions, append-only, as
 *    if narrating the game), then ONE fresh board serialization, the
 *    model's last stated PLAN line, and the legal choices. There is no
 *    chat transcript: old board snapshots never stack up (they diluted
 *    attention and defeated prefix caching), and the model is told its
 *    reasoning is dropped - each reply must restate its complete plan,
 *    of which only the most recent is carried forward.
 *
 *  Opt-in at launch, never default:
 *      WAGIC_AI=gpt              enable the GPT opponent
 *      WAGIC_GPT_URL=<base>      endpoint base URL (e.g. https://openrouter.ai/api)
 *      WAGIC_GPT_KEY=<key>       bearer token (OpenRouter / any keyed endpoint)
 *      WAGIC_GPT_MODEL=<id>      model id (default: first id from /v1/models)
 *      WAGIC_GPT_THINKING=1      enable thinking on local Qwen-style servers
 *      WAGIC_GPT_MAXTOKENS=<n>   completion budget override
 *
 *  Compiled only when WITH_GPT_AI is defined (Linux SDL build); on any
 *  failure (endpoint down, timeout, unparseable reply) every decision
 *  falls back to the AIPlayerBaka heuristic, so the game never stalls.
 */

#ifndef _AIPLAYERGPT_H_
#define _AIPLAYERGPT_H_

#ifdef WITH_GPT_AI

#include "AIPlayerBaka.h"

#include <set>
#include <utility>
#include <memory>

class WEvent;
class DecisionRequest;
class DecisionAction;

class AIPlayerGPT : public AIPlayerBaka
{
public:
    AIPlayerGPT(GameObserver *observer, string deckFile, string deckfileSmall, string avatarFile, MTGDeck * deck = NULL);

    //true when the player launched the game with WAGIC_AI=gpt
    static bool isEnabled();

    //Card-data "ishuman" gates route this player onto the interactive
    //lines (menus/choosers the decision seams answer) instead of the
    //heuristic dice-roll lines - but only while an endpoint is live; the
    //Baka fallback needs the heuristic lines intact.
    virtual bool isInteractiveAI() const
    {
        return !mEndpoint.empty();
    }

    //Adds the mulligan decision (the engine has no AI mulligan path at all -
    //the heuristic always keeps) and menu handling that can wait on an
    //in-flight model call, before delegating to the base loop.
    virtual int computeActions();

    //Model calls are asynchronous: the HTTP round trip runs on a worker
    //thread while the game loop keeps rendering. The base Act consults
    //decisionPending - while a call is in flight the AI neither acts NOR
    //passes (an empty clickstream normally commits a pass/decline), and the
    //interrupt-offer timer is kept alive so a slow model cannot time out of
    //its response window. Render draws the "thinking" indicator.
    virtual bool decisionPending(float dt);
    virtual void Render();

    //The patience prompt. A call still in flight after mPatienceLimit
    //seconds is one a human has been staring at for a full minute with no
    //way to act, so the duel screen offers the choice out. The HTTP timeout
    //stays what it always was - a liveness bound on a dead endpoint - and
    //stops being the thing a player waits on.
    virtual bool aiPatiencePromptDue();
    virtual void aiPatiencePromptAnswer(bool keepWaiting);

    //feeds the game narrative to the agent transcript
    virtual int receiveEvent(WEvent * event);

    //Engine game-over hook (Player::gameEnded): closes the translog with
    //the "gameend" record.
    virtual void gameEnded();

protected:
    virtual const OrderedAIAction * chooseOrderedAction(RankingContainer& ranking);
    //Deck hint scripts must not pre-empt the model's ranked decision.
    virtual int selectHintAbility();
    //Menus are decisions too: modal (choose-one) spells, may-prompts and X
    //announcements all route through the model, with the heuristic as the
    //fallback for anything unparseable. Since c3 they ride the
    //DecisionRequest contract (chooseMenuAction below); the base
    //selectMenuOption stays the heuristic policy.
    //Combat declarations are opposed choices too: route each creature's
    //attack / block decision through the model (heuristic when no endpoint).
    virtual int chooseAttackers();
    virtual int chooseBlockers();
    //Card play is the model's choice. For lands the heuristic proposes and
    //the model may veto (dropping a land is nearly always right); for spells
    //the model picks freely among every castable card - the heuristic's
    //legality/payment machinery then validates and prices the pick via
    //AIPlayerBaka::aiForcedCandidate.
    virtual MTGCardInstance * FindCardToPlay(ManaCost * potentialMana, const char * type);
    //Combat damage ordering: when one of our attackers is blocked by more
    //than one creature, damage is assigned lethal-first down the blockers
    //vector - the model chooses that order (the heuristic's own version is
    //a no-op that accepts declaration order).
    virtual int orderBlockers();
    //Spell/ability targeting: the model picks the target(s) among the legal
    //set; the clicks reuse the engine's own click helpers so the mechanics
    //stay identical to the heuristic path. checkOnly probes and forced
    //targets are mechanics, not decisions - those go straight to the base.
    virtual int chooseTarget(TargetChooser * tc = NULL, Player * forceTarget = NULL, MTGCardInstance * chosenCard = NULL, bool checkonly = false);
    //Extra-cost targets (what to sacrifice/tap/discard to pay a cost) are
    //decisions too - the base picks the FIRST legal candidate, which is
    //blind (sacrificing the best creature as readily as the worst). Rides
    //the CHOOSE_TARGET contract build over the cost's own chooser; NULL
    //while the model call is in flight aborts the payment attempt for the
    //tick (payTheManaCost re-reaches it on the next poll).
    virtual MTGCardInstance * chooseCostTarget(TargetChooser * tc, MTGCardInstance * source);

    //Interactive reveal/surveil decision (Player::decideReveal override). When
    //this player controls a reveal/surveil display, GenericRevealAbility routes
    //it onto the interactive MTGRevealingCards display (the ishuman lens: the
    //aicode moverandom substitute is for the heuristic AI's limits, not this
    //one). The display shows the model EVERY revealed card in ONE bundled ask
    //and the model picks which go to option one (surveil: the graveyard); the
    //rest default to option two (top of library). Async: returns 0 while the
    //call is in flight (the display waits), 1 once decided (selForOptionOne
    //filled), -1 on any failure (the display's safe default sends nothing to
    //option one). One JSONL "reveal" translog record per consumed decision.
    //eligibleForOptionOne (same size/order as revealed) is the engine's own
    //per-card verdict from option one's target chooser (tc->canTarget): true
    //when THIS card may actually go to option one. The reveal seam routes a
    //TUTOR/FILTERED reveal (Into the North's snow-land search, Search for
    //Azcanta's noncreature-nonland dig) here; without the flags the model was
    //offered the whole set with no hint of the filter and picked ineligible
    //cards -> zero to hand (deck135 wave-19 R3/R4). The seam surfaces the
    //eligibility per card + a readable filter line - annotation only, every
    //revealed card stays listed.
    virtual int decideReveal(const vector<MTGCardInstance*>& revealed,
                             const string& optOneLabel, const string& optTwoLabel,
                             const string& optOneEffect,
                             vector<int>& selForOptionOne,
                             const vector<bool>& eligibleForOptionOne = vector<bool>(),
                             int revealSource = 0, bool pickExactlyOne = false);

    //Pre-game (opening-hand) decisions, driven by PreGamePhase before turn 1.
    //Each routes through the model with the AIPlayerBaka heuristic as the
    //deadlock-safe fallback. Mulligan and leyline use the CHOICE ask machinery
    //(narrated + "ask" translog); bottoming is ONE bundled PUT-N ask over the
    //hand, mirroring decideReveal (parsed with the reveal PUT parser, "bottom"
    //translog), whose picks are cached and popped one card per call.
    virtual int pregameMulliganDecision(int mullsTaken);
    virtual MTGCardInstance * pregameChooseBottom(int need, int chosenSoFar, int & status);
    virtual int pregameLeylineDecision(MTGCardInstance * card);
    //State for the single bundled BOTTOM-N ask per keep.
    std::vector<MTGCardInstance*> mPregameBottomQueue;
    bool mPregameBottomAsked;
    int  mPregameBottomForMulls;
    //The mulligan count the engine last quoted us (N-139i): the bottom ask
    //used its own "cards to bottom" number wearing a mulligan label, which is
    //a different quantity once the count is clamped - and the model argued
    //with the contradiction out loud (200 s, 12.5k chars, one ask).
    int  mPregameMullsSeen;
    //N-158r: TRUE only while one of the three pregame asks (mulligan, London
    //bottoming, leyline) is assembling its prompt. The hand-only pregame frame
    //is gated on THIS, not on observer->turn - the turn counter is still 0
    //through the on-the-play player's whole first turn, so its first land drop
    //was served the opening-hand frame with no board and no mana line.
    //Set/cleared by PregameAskScope; the asks are re-entered every tick while a
    //model call is in flight, so the flag is re-armed on each poll.
    bool mInPregameAsk;
    struct PregameAskScope
    {
        AIPlayerGPT * p;
        PregameAskScope(AIPlayerGPT * _p) : p(_p) { p->mInPregameAsk = true; }
        ~PregameAskScope() { p->mInPregameAsk = false; }
    };
    string buildPregameBottomAskText(const vector<MTGCardInstance*>& hand, int need,
                                     int alreadyBottomed);

public:
    //Env-gated (WAGIC_GPT_PARSETEST) self-test of the reply parsers: feeds the
    //wave-19 failing replies + synthetic spirals through the real parse paths
    //(consumePlan-free: the static parsers + salvage) and prints before/after
    //to stdout. Invoked from main() before any game setup; no observer needed.
    static void runParseSelfTest();

private:
    //Ask the model to choose among options (0-based result, -1 to defer to
    //the heuristic). No model call when there is one option or none - that
    //is the "only one valid action" case. Answers are cached by board state
    //plus question (a map, not a single slot: several distinct questions
    //repeat every AI tick - land veto + card choice, one ask per creature in
    //combat - and a one-slot cache would re-fire the HTTP call for each on
    //every tick). The key deliberately excludes the narration and the plan:
    //consuming one answer updates both, and a full-prompt key would then
    //miss on the re-poll of a question already answered this state (e.g.
    //the earlier picks of a multi-target selection) and re-ask it.
    //narrateChoice: false for asks whose outcome is already visible as
    //events (land drop, casting - the zone changes narrate themselves) or
    //whose "no" answer is a non-action; true for choices that leave no
    //event trace (targets, modes, X values, damage order).
    int askModel(const string& decision, const vector<string>& options, bool narrateChoice = true,
                 const string& pendingSourceName = string());
    std::map<string, int> mAskCache;

    //Answer a menu-family DecisionRequest (CHOOSE_MENU / CHOOSE_MODE /
    //ANNOUNCE_X): ask the model over the snapshotted options, fall back to
    //the heuristic policy (base selectMenuOption, mapped into option space)
    //when the model defers. Returns kChoicePending while in flight, else 0
    //with `act` filled for DecisionManager::applyMenuChoice.
    int chooseMenuAction(const DecisionRequest & req, DecisionAction & act);

    //Can this card plausibly be paid for right now? Cheap pre-filter for the
    //model's casting menu; the authoritative check is the forced
    //AIPlayerBaka::FindCardToPlay validation pass.

    //Probe candidate endpoints (env override, then Spark vLLM, then local
    //llama.cpp) and remember the first one that answers /v1/models.
    void resolveEndpoint();

    //Build the per-duel system prompt; deferred to the first decision so
    //that the opponent and all zones exist.
    void buildSystemPrompt();
    string describeDeckCards(Player * p, bool withCounts);
    string loadStrategyGuide();

    string serializeGameState();
    //N-146k: the pregame (mulligan/bottoming/leyline) situation block. Hand only
    //- a pregame ask carries no board-state information at all.
    string serializePregameState();
    string describeAction(const OrderedAIAction& action);
    string describeEvent(WEvent * event);

    //Assemble the user message: narration head, one fresh board snapshot,
    //the carried plan, then the decision-specific tail (question + options
    //+ reply format).
    string assemblePrompt(const string& tail);
    //Append one line to the game narration (the model's own decisions join
    //the event narrative so the story stays complete without a transcript).
    void narrateDecision(const string& line);
    //Bound-checked narration append; flushes the pending phase marker first.
    void appendNarration(const string& line);
    //Split a reply at its "PLAN:" marker: stores the (complete, per the
    //protocol) plan into mCurrentPlan and returns the decision part, which
    //is the ONLY text the choice parsers may see - plan prose is full of
    //numbers that would misparse as option indices.
    //`expectedLabel` (e.g. "CHOICE:") restricts answer-line selection to this
    //decision's own label, so a chain-of-thought line beginning with a
    //DIFFERENT answer label ("Attack: Regent 6/6 vs...") in the reasoning body
    //is not mistaken for the answer. NULL keeps the legacy any-label behavior.
    string consumePlan(const string& content, const char * expectedLabel = NULL);

    //Decision seams return this while the model call for their prompt is
    //still in flight. Callers unwind for the current tick and re-poll on the
    //next one; the flow is deterministic while the AI neither acts nor
    //passes, so the same seam re-reaches the same prompt and consumes the
    //answer when it lands.
    static const int kChoicePending = -2;

    //Async completion: state shared with the worker thread. The worker owns
    //a shared_ptr copy, so a game that ends mid-request cannot leave the
    //thread writing into freed memory.
    struct AsyncState;
    //The worker's heap-allocated capture set + entry point, shaped for
    //gptSpawnWorker's bare function+void* contract (the least common
    //denominator across its platform implementations - see GptConfig.h).
    struct WorkerCtx;
    static void WorkerMain(void * p);
    std::shared_ptr<AsyncState> mAsyncState;
    float mThinkTime; //seconds the current request has been in flight (for the indicator)
    long mTimeoutMs;  //per-call HTTP timeout (config timeout= / WAGIC_GPT_TIMEOUT)
    //Seconds in flight before the duel screen offers "keep waiting / play
    //without the LLM". Distinct from mTimeoutMs on purpose: that one bounds
    //a dead connection, this one bounds how long a person is asked to sit
    //and watch. 0 disables the prompt (config patience= / WAGIC_GPT_PATIENCE).
    float mPatienceLimit;

    //Transient in-duel notice ("no endpoint reachable", "model timed out"),
    //drawn by Render for a few seconds. Frame-based decay: Render runs per
    //frame, Act does not run for both players every tick.
    string mNotice;
    int mNoticeTicks;
    void setNotice(const string& text, float seconds);

    //A transient notice is easy to miss - one line, five seconds, top of the
    //screen, while the player is looking at their hand. noticeFallback shows
    //the same message AND lights a standing marker, so a decision the model
    //did not make is visible after the flash is gone. Use it only where the
    //heuristic AI actually answered instead of the model; a RE-ASK is not a
    //fallback and counting one would overstate how badly things are going.
    void noticeFallback(const string& text, float seconds);
    int mFallbackCount;   //heuristic-answered decisions this duel
    int mDegradedTicks;   //frames the standing marker stays lit; self-clearing
                          //so a model that recovers stops being accused

    //Turn whose combat already had its bundled blocking declaration - the
    //whole combat is decided in ONE model reply, so once executed nothing
    //more may be asked this turn (leftover no-block creatures would
    //otherwise re-prompt against the changed board).
    int mBlocksDoneTurn;

    //Same for the bundled attacker declaration: the whole attack is decided
    //in ONE reply, so the seam commits once per turn's combat and does not
    //re-prompt the creatures it chose to hold.
    int mAttacksDoneTurn;

    //Priority-window decline memory: an activation line the model has
    //pass-declined TWICE in the same turn stops being offered for the rest
    //of that turn (a held fetch-crack was re-asked at 44-97 windows per
    //game - each a model call once any event changed the board). Two
    //declines keep a fresh look every turn plus one re-look; the map
    //clears on turn change. Keyed by rendered option line.
    std::map<string, int> mPassDeclineCount;
    int mPassDeclineTurn;

    //Modal-DFC flip-thrash cap (R-DFC-FLIP, deck102 wave-22): the in-hand
    //"Flip Side" pseudo-action is a no-op face toggle offered at every
    //priority window, and it CHANGES board state (the presented face) so the
    //no-progress deadlock breaker never catches it - deck102 flipped Tergrid
    //11x. Cap flips per source card per turn: enough to reach the wanted face
    //(and flip back once) but not to thrash. Keyed by the card INSTANCE
    //pointer - getId()/mtgid TOGGLES with the presented face, so keying by id
    //let each face accrue its own count (up to 4 flips/turn); the instance
    //pointer is stable across flips. Clears on turn change alongside
    //mPassDeclineCount.
    std::map<MTGCardInstance *, int> mFlipDoneCount;

    //Cast-seam livelock breaker (the priority seam's no-progress pass,
    //mirrored): a consumed cast pick that leaves the board byte-identical
    //did not execute (an unexecutable menu entry - e.g. a restricted cast
    //mode - or an engine no-op). The cached ask would replay it every tick
    //forever (135v133 wedged at turn 2 for 2400s, 903k re-picks). Suppress
    //that option line for the turn and re-ask over the remaining options -
    //self-healing when a sibling entry (the legal alternative mode) exists.
    string mLastCastBoard;
    string mLastCastLine;
    std::set<string> mStuckCastLines;
    int mStuckCastTurn;

    //Decision-transcript dump (config translog=1 / WAGIC_GPT_TRANSLOG):
    //one JSONL record per consumed decision - prompt-tuning raw material
    //and, accumulated, training data for a small shippable policy model.
    string mTransLogPath; //empty = disabled
    int mTransSeq;
    //Every record carries game context (turn/phase/life), the last round
    //trip's latency, the chosen option as TEXT (indexes rot, text does
    //not), and - when the heuristic answered instead - the fallback reason
    //(c2: the silent choice:-1 class becomes attributable).
    long mLastLatencyMs; //-1 = no round trip behind this record (cache/reuse)
    //Blocker assignments the apply-site canBlock gate PRUNED as illegal,
    //rendered "<blocker> -> <attacker>; ...". Set immediately before the
    //blockers translog write and consumed (cleared) by it. WAVE-33 N-152j:
    //the name-form parse resolves intent and the validator prunes it, so the
    //pruning must be visible or the two are indistinguishable in review.
    string mLastPrunedPairs;
    void ensureGameStartRecord();
    void writeTransLog(const char * kind, const string& userMsg, const string& reply, int choice, int optionCount,
                       const string& chosenText = "", const char * fallback = NULL,
                       const std::vector<string> * optionTexts = NULL,
                       const char * choiceSource = NULL);
    //One "gameend" record per duel: result + final life + turn count. The
    //per-decision records alone cannot say who WON - win-rate and timeout
    //adjudication both need it in the same file.
    bool mGameEndLogged;
    bool mGameStartLogged; //header record emitted (lazily, first write)
    size_t mNarrationLogged; //narration length already emitted to the translog
                             //(each record carries the delta = the events that
                             //landed since the previous record, so a consumed
                             //cast's OUTCOME is machine-readable - wave-7 7b)
    void logGameEnd();

    //True while a request is in flight whose answer has not been consumed.
    bool asyncBusy() const;
    //Poll the async slot for this exact prompt: starts the request when the
    //slot is idle. Returns kChoicePending while in flight, 0 with the reply
    //content once done. A finished answer for a DIFFERENT prompt (stale
    //state drift) is dropped and the new request started.
    int pollCompletion(const string& userMsg, string& content);
    //pollCompletion wrapped with ONE answer-locked retry: when a completed reply
    //is decode-garbage (isDecodeGarbage), fires a single re-ask of the same
    //decision with a short "answer only" prefix and a tight max_tokens, riding
    //the same kChoicePending machinery (a second pollCompletion round). Returns
    //kChoicePending while either attempt is in flight; 0 with the reply content
    //once done (the retry's reply, which may itself still be unusable -> the
    //caller's heuristic fallback). NEVER retries ordinary unparsed replies.
    int pollCompletionRetry(const string& userMsg, string& content);
    //True when a completed reply is a decode-COLLAPSE (token garbage): no
    //well-formed coded answer line AND a long reply that is either
    //repetition-signatured or >=30% markup/non-ASCII with near-zero prose. The
    //shape that burns 80-120s and yields nothing (deck27 vs137 s12-14). Kept
    //conservative: an ordinary long unparsed reply (real prose, no coded line)
    //is NOT garbage and is never retried.
    static bool isDecodeGarbage(const string& content);
    //Serialize the chat request (system prompt + the pending user message)
    //for the worker thread; built on the game thread, nothing shared.
    string buildRequestBody(const string& userMsg);
    //Extract the chosen action number from a model reply; -1 if unusable.
    //staleEcho (out, optional) is set true when the name-echo parses to
    //significant words that match NO offered option - a staleness signal
    //that routes the answer to the heuristic instead of the raw index.
    static int parseChoice(const string& content, int optionCount,
                           const std::vector<string> * optionTexts = NULL,
                           bool * staleEcho = NULL,
                           const std::string * pendingSource = NULL);

    //Salvage a decode-time repeat-loop reply: when the normal parse fails,
    //scan the raw reply for the LAST well-formed "CHOICE: N (name)" line
    //(one the model stated before spiraling) and re-parse it through the
    //same echo/staleness checks. Returns the 1-based choice or -1. Never
    //bypasses stale_echo protection (a stale line re-parses to -1 here too).
    static int salvageLoopedChoice(const string& content, int optionCount,
                                   const std::vector<string> * optionTexts = NULL);

    //True when a well-formed CHOICE was superseded by an explicit
    //self-retraction with no replacing CHOICE - the answer must route to the
    //heuristic instead of using the retracted digit (deck135 HARNESS-1).
    static bool choiceRetractedNoReplacement(const string& content, int optionCount,
                                             const std::vector<string> * optionTexts = NULL);

    string mEndpoint; //base URL, empty if nothing answered
    string mModel;
    string mApiKey;
    bool mThinking;
    //Codex-backend reasoning tier (config reasoning_effort / WAGIC_GPT_EFFORT,
    //validated against the server's set); empty = the built-in default (low).
    string mReasoningEffort;
    //Subscription preset: last plan-usage percent already noticed to the
    //player, so the gauge is surfaced when it MOVES, not every decision.
    string mCodexPctNotified;
    //from ai/gpt/endpoints.txt (the per-user copy shadows the shipped one);
    //environment variables override these
    vector<string> mConfigUrls;
    string mConfigModel;
    long mMaxTokens; // -1 = use the built-in/thinking-dependent default
    double mRepetitionPenalty; // vLLM repetition_penalty; 1.0 = OFF (not sent)
    string mProviderOnly; // OpenRouter provider pin, comma-separated; "" = off

    //the per-duel head of every request; empty until first built
    string mSystemPrompt;
    //append-only game narration: every noteworthy event plus the model's
    //own consumed decisions, as if narrating the game (bounded, tail kept)
    string mNarration;
    //Phase changes are narrated LAZILY: a phase change only updates this
    //marker (overwriting the last one), and the marker joins the narration
    //when a real event or decision lands in that phase - phases in which
    //nothing happened never enter the narration. TURN changes are the
    //exception: a turn header is always written, so it is contextually
    //clear whose turn it is when other things happen.
    string mPendingPhase;
    Player * mNarratedTurnOwner;
    int mNarratedTurnNumber;
    //The opening deal is collapsed to ONE line ("Your opening hand (7):
    //...") instead of seven draw lines: own library->hand moves buffer here
    //until the first flush (first prompt or first other narrated line), and
    //a taken mulligan re-opens the buffer for the redraw. The opponent's
    //hidden deal/mulligan churn during the mulligan window is not narrated
    //at all.
    vector<string> mOpeningHand;
    bool mDealDone;
    void flushOpeningHand();
    //the model's last stated PLAN line - the ONLY reply text carried
    //forward (the protocol tells it so, and to restate the plan in full)
    string mCurrentPlan;
    //Cards the opponent revealed that are now in their hand: public info a
    //human would remember. Tracked by name (instances are recreated on zone
    //moves), decremented when a card of that name leaves the hand.
    std::map<string, int> mKnownOppHand;
    //N-105a: last-narrated poison total per player ([0] this seat, [1] the
    //opponent), so a poison-gain line can state the real DELTA. The engine's
    //own WEventplayerPoisoned payload cannot be trusted for it - the toxic
    //path (Damage.cpp:239) fires the event carrying the DAMAGE dealt, not the
    //toxicity granted - while Player::poisonCount is settled by the time the
    //event lands, so the delta is derived from the settled totals instead.
    int mLastPoison[2];
    //Avoid re-querying the model every AI tick while nothing changed. Keyed
    //by board state + question (not the full prompt) for the same reason as
    //mAskCache: taking an action narrates it, and a full-prompt key would
    //read that as "state changed" and defeat the deadlock breaker.
    string mLastAskKey;
    int mLastChoice;
    //Answer-locked decode-garbage retry (ITEM: decode-collapse mitigation).
    //mRetryActivePrompt: the retry userMsg (prefix + base) currently in flight;
    //empty when no retry is pending. mRetryBase: the base userMsg that active
    //retry is for (abandon the retry if the decision drifts). mRetryDoneBase:
    //a base userMsg whose single retry was already spent (enforces ONE retry
    //and blocks re-detecting garbage on the retry's own reply). Latency of the
    //first (garbage) attempt, summed into mLastLatencyMs at retry completion.
    //mLastRetry: set when the reply handed back was a retry (translog retry=1).
    string mRetryActivePrompt;
    string mRetryBase;
    string mRetryDoneBase;
    long mRetryFirstLatencyMs;
    bool mLastRetry;

    //NATIVE REASONING (wave-34 #1a/#1b). With the post-answer scratch block
    //gone from the reply protocol, the auditable reasoning is the model's own
    //thinking window - captured here, written to the translog, and BOUNDED.
    //mLastReasoning: the last reply's thinking, from message.reasoning_content
    //or from an inline <think> block, whichever the server produced. Consumed
    //(cleared) by the translog write, like mLastLatencyMs.
    string mLastReasoning;
    //"empty_reply" (transport: nothing came back) vs "reasoning_only" (a
    //complete reply whose whole generation was filed as thinking). Same
    //outcome - the heuristic answers - but a seat review must not read a model
    //behaviour as an endpoint fault.
    const char * noAnswerClass() const;
    //Was reasoning asked for on this endpoint (thinking flag, or the Codex
    //effort tier)? Only ever used to tell a WITHHELD trace from a reply that
    //never reasoned - parsing and fallback never consult it.
    bool reasoningRequested() const;
    //Reasoning was requested, the answer arrived, and the provider withheld the
    //trace (OpenAI/Anthropic policy; OpenRouter depending on upstream). Normal,
    //parsed like any other reply, and marked reasoning_hidden in the translog.
    //The INVERSE of reasoning_only - never conflate the two.
    bool mLastReasoningHidden;
    //The last reply was REASONING-ONLY: an unclosed <think>, i.e. the thinking
    //budget (or max_tokens) cut the reply before any answer. Never parsed as
    //an answer; it triggers the forced close instead.
    bool mLastReasoningOnly;
    bool mLastFinishLength;  //the last reply stopped at the token cap
    long mLastReasoningTokens; //the server's own reasoning-token count when it
                             //reports one (-1 = it did not); the budget is in
                             //tokens, so this is the calibration number
    int mLastDroppedAssignments; //blocker seam: B:A pairs the reply asked for
                             //that never reached the battlefield (parser drops
                             //+ apply-site prunes). -1 = not a blocker record
    bool mLastBudgetHit;     //the forced close fired AND the decode had actually
                             //stopped at the cap (finish_reason == "length") -
                             //translog reasoning_budget_hit, the A/B's count of
                             //how often the BUDGET bound. Split from the line
                             //below in wave-35: the marker used to be set for
                             //every forced close, so a reply that merely stopped
                             //reasoning without answering (natural stop, no
                             //truncation) was logged as a budget hit - which is
                             //how a 12,058-char trace "hit" an 8,000-TOKEN budget
                             //that 29,027-char traces did not (wave-34 b6).
    bool mLastForcedClose;   //a forced close fired for this decision, whatever
                             //the reason (translog reasoning_forced_close). The
                             //rescue-rate metric; superset of mLastBudgetHit.
    //Repetition ratio of the last reply's reasoning trace: the share of its
    //40-char shingles that are copies of the single most-repeated one. A
    //healthy trace sits near 0; a decode collapse (415-repeat mojibake, a
    //13.8k-char "No. Okay." loop - both wave-34, both invisible to every answer
    //metric because the reply itself parsed fine) sits near 1. -1 = no
    //reasoning to measure.
    double mLastReasoningDegenerate;
    //The truncated thinking handed back to the model on the forced close, with
    //its "</think>" injected by the request builder.
    string mForceClosePrefill;
    //Thinking-window budget in TOKENS, thinking mode only (config
    //reasoning_budget / WAGIC_GPT_REASONING_BUDGET; 0 or less = unbounded).
    long mReasoningBudget;
};

#endif //WITH_GPT_AI

#endif //_AIPLAYERGPT_H_
