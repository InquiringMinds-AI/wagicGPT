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
#include <fstream> //audit-L (L4): the translog stream

class WEvent;
class DecisionRequest;
class DecisionAction;
class MTGCardInstance;
class ActivatedAbility;

//=== W35/W40 narration register - shared surface ============================
//Both are defined in AIPlayerGPT.cpp. They are exposed so the TESTSUITE seat
//can drive the SAME production code the live GPT seat does: a fixture that
//re-implemented the counter gate would test a copy of the defect's neighbour
//instead of the defect (see Res/test/countered_spell_narration.txt).

//Does this zone-departure card descend from the marked (countered) stack
//instance? putInZone raises its event with a CLONE, so identity comparison
//against the stack instance can never match - the rationale, and the 0-hits
//measurement that exposed it, are on the definition.
bool counterMarkerMatches(const MTGCardInstance * moved, const MTGCardInstance * marker);

//#W53-P (D4/D8): the two register hooks the wave-53 suite fixtures pin. Both
//run the PRODUCTION emitters over the LIVE game state - the same functions the
//prompt renders from - so a red fixture points at the shipped render, never at
//a copy of it. `seat` is the reading chair (whose "your"/"their" the lines use).
//stackAbilityRegister: one line per UNRESOLVED ability on the stack, in the
//stack block's own wording. exileCastRegister: one line per card in `seat`'s
//exile that carries a stamped canplayfromexile grant. Both return "" when
//there is nothing to say, so a fixture that does not exercise them pays only
//the two empty scans.
string stackAbilityRegister(GameObserver * observer, Player * seat);
string exileCastRegister(Player * seat);
//#W54-D (D3/D6): the framing header the own-card chooser would print, built by
//the production emitters over the live chooser ("" when this window is not one).
string ownCardChooserRegister(GameObserver * observer, Player * seat);

//The register form of one zone change (pure; PARSETEST-covered).
//#W57-H (D43): the game-log window's ask classes and arms. Declared here (not
//beside their functions in the .cpp) because AIPlayerGPT's constructor and its
//translog writer both sit above the window block in that file.
enum GptAskWindowKind
{
    kAskWindowUnknown = 0,      //unclassified: keeps the full log under `kind`
    kAskWindowLandDrop,
    kAskWindowCleanupDiscard,
    kAskWindowEmptyStackPass,
    kAskWindowDisplayMenu,
    kAskWindowCast,
    kAskWindowCombat,
    kAskWindowTargetOrReveal,
    kAskWindowPregame           //never windowed: the pregame frame is hand-only
};
enum GptLogWindowMode { kLogWindowFull = 0, kLogWindowByKind = 1, kLogWindowEveryKind = 2 };
const int kLogWindowDefaultTurns = 3;
const int kLogWindowMaxTurns = 999;
const char * askWindowKindName(int kind);

string zoneChangeNarration(bool mine, const string& cardName, const string& from,
                           const string& to, bool isCreature, bool isLand,
                           bool countered, const string& counterSource,
                           const string& targets = "");

//#W48 (D11): the narration log's repetition holder, extracted as a PURE object
//so a whole loop can be pinned in PARSETEST through the same code the game runs.
//#W43-11 held a run of ONE byte-identical line; the loop it was aimed at repeats
//a CYCLE ("You used: Create vampire" / "Your Lord of Lineage created a 2/2
//Vampire token", alternating ~180 times - ~90% of a 26 KB prompt, with no count
//anywhere), so the holder recognises a repeating block of up to
//kNarrationCycleMaxPeriod lines and the single-line run is its period-1 case.
//`add` appends to `out` exactly the lines that are ready to be written (the
//holder keeps the rest); `flush` empties it. Nothing is ever dropped: a block
//below the collapse floor is written verbatim, and a collapsed block writes one
//sentence per DISTINCT line carrying the exact number of times it happened.
//#W57-D (D14): the suite's probe into the GPT seat's display-toggle
//suppression. 1 = suppressed (land back face), 0 = offered (spell back
//face), -1 = this card has no "Flip Side" toggle. See the definition.
class ActionLayer;
int gptDisplayToggleSuppressed(MTGCardInstance * c, ActionLayer * al);

struct NarrationCycleHolder
{
    vector<string> cycle;      //the established repeating block (empty = none)
    vector<int> totals;        //per-position family total of the LAST repetition
    int reps;                  //completed repetitions of `cycle`
    vector<string> pend;       //lines since the last complete repetition, which
    vector<int> pendTotals;    //double as the candidate buffer while none is held
    NarrationCycleHolder() : reps(0) {}
    bool empty() const { return cycle.empty() && pend.empty(); }
    void add(const string& line, int total, vector<string>& out);
    void flush(vector<string>& out);
};

class AIPlayerGPT : public AIPlayerBaka
{
public:
    //#W50-Y D7: the clean-line grammar and the rejection test for a coded
    //CHOICE line's payload (everything after the "CHOICE:" label). Public and
    //static so PARSETEST pins the grammar directly.
    static bool choiceLineIsClean(const string& payload);
    static bool choiceLineIsRejection(const string& payload);
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
    //#W54-R: the const, side-effect-free half of the same fact, for the
    //stall floor. An endpoint-less seat is never in flight (all its seams
    //fall through to the heuristic).
    virtual bool aiDecisionInFlight() const { return !mEndpoint.empty() && asyncBusy(); }
    //#W57-T: the seat's async/pending state as one line for the softlock
    //diagnostics dump - per arm: status (idle/in-flight/done-unconsumed),
    //seconds in flight, the slot key the request was built for, the HTTP
    //status of the last round trip - plus the standing notice and fallback
    //count. Takes each arm's mutex briefly, exactly as asyncBusy does.
    virtual std::string softlockDiagnostic() const;
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

    //The async blockers declaration is still owed for this combat (see
    //AIPlayer::blockersDeclarationDue) - true while this seat defends with
    //a live endpoint and this turn's bundled declaration has not committed.
    virtual bool blockersDeclarationDue();

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
                             int revealSource = 0, bool pickExactlyOne = false,
                             //#W61-T (C8): single-pick reached by ARITY (a bare
                             //target() chooser) - see Player::decideReveal.
                             bool singlePickBare = false,
                             //#W61-V (R1): the script's arity permits taking none.
                             bool singlePickDeclineLegal = false);

    //Pre-game (opening-hand) decisions, driven by PreGamePhase before turn 1.
    //Each routes through the model with the AIPlayerBaka heuristic as the
    //deadlock-safe fallback. Mulligan and leyline use the CHOICE ask machinery
    //(narrated + "ask" translog); bottoming is ONE bundled PUT-N ask over the
    //hand, mirroring decideReveal (parsed with the reveal PUT parser, "bottom"
    //translog), whose picks are cached and popped one card per call.
    virtual int pregameMulliganDecision(int mullsTaken);
    virtual MTGCardInstance * pregameChooseBottom(int need, int chosenSoFar, int & status);
    //#W42-D9: the real body; the override above only arms the bottoming flag.
    MTGCardInstance * pregameChooseBottomInner(int need, int chosenSoFar, int & status);
    virtual int pregameLeylineDecision(MTGCardInstance * card);
    //W50-W (D4): the cleanup-step hand-size discard, put to the model as ONE
    //bundled "name exactly N cards" ask over the hand (the bottoming ask's
    //shape, "discard" translog kind); a failed/absent reply takes the base
    //highest-mana-value policy, announced. Called from the base computeActions
    //before the phase advance would discard hand->cards[0] silently.
    virtual int cleanupDiscard(int over);
    //#W55-D (D18): outOrder receives the collapse's stable permutation of the
    //printed rows; the caller maps the reply's positions back through it.
    string buildCleanupDiscardAskText(const vector<MTGCardInstance*>& hand, int limit, int over,
                                      vector<size_t> * outOrder = NULL);
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
    //Set/cleared by PregameAskScope. The pregame asks are driven by the
    //PreGamePhase poll and re-entered on each poll while a model call is in
    //flight, so the flag is re-armed per poll. (audit-L L3: this is NOT the
    //seams' general cost model - the Act-driven seams return at
    //decisionPending in AIPlayerBaka::Act and are entered once per answer;
    //two audits were steered to a per-tick model by the old wording.)
    bool mInPregameAsk;
    struct PregameAskScope
    {
        AIPlayerGPT * p;
        PregameAskScope(AIPlayerGPT * _p) : p(_p) { p->mInPregameAsk = true; }
        ~PregameAskScope() { p->mInPregameAsk = false; }
    };
    //#W48 D9 (wave-47 D9 = R10): TRUE only while the ANNOUNCE_X ask is
    //assembling its prompt. Same shape as mInPregameAsk, same reason: the
    //carried plan named an X chosen ONE SCREEN EARLIER, at the cast row, before
    //the kill lists this menu prints existed - and the pilot restated it
    //verbatim over a strictly better row (deck130 vs126 seq 37: "Cast Starstorm
    //at X=3 to kill both Pride Guardians", replied X=3, with "X = 4 {kills
    //THEIRS: Pride Guardian x2, Perimeter Captain; YOURS: none}" on screen).
    //A plan written before the evidence is not evidence about it.
    bool mInAnnounceXAsk;
    struct AnnounceXAskScope
    {
        AIPlayerGPT * p;
        AnnounceXAskScope(AIPlayerGPT * _p) : p(_p) { p->mInAnnounceXAsk = true; }
        ~AnnounceXAskScope() { p->mInAnnounceXAsk = false; }
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
    //askEvenIfSingle: normally an ask with exactly ONE option is answered
    //without a model call - there is no decision to make. The X announcement
    //is the one seam where that optimisation is WRONG (owner ruling, wave-41
    //#W41-1: "the pilot must be ASKED for X on every {X} cast it commits.
    //Removing the question is the same wrong as removing the option"). A
    //zero-slack {X} cast has exactly one announceable value, and answering it
    //silently committed X with no stderr line and no translog record - the
    //same silent-discard shape the ruling forbids. Scoped to ANNOUNCE_X; every
    //other seam keeps the no-call shortcut.
    //#W47-R13: suppressPlanRequest drops the trailing "then a PLAN: line ..."
    //clause from the reply instruction. Set it only where a PLAN cannot be
    //owed - the land-drop binary, whose two slowest windows this corpus were
    //453 s and 367 s of full-turn monologue over a decision with one right
    //answer. It suppresses the REQUEST, never the parse: a plan the model
    //volunteers anyway is still consumed and carried.
    //#W62-Y (D5): `declineRowOffered` says this ask's LAST row is a decline the
    //caller appended (CHOOSE_MENU's "Decline - do nothing", the X announcement's
    //cancel). kNoPassRowFact - "(this ask has no pass row)" - is false on such a
    //menu, and the seat reads that line as the reason it must take something.
    int askModel(const string& decision, const vector<string>& options, bool narrateChoice = true,
                 const string& pendingSourceName = string(), bool askEvenIfSingle = false,
                 bool suppressPlanRequest = false, bool declineRowOffered = false);
    std::map<string, int> mAskCache;
    // audit-M (#W54-M): wave-54 audit lane M members (A17/A19/A21/L8) - see
    // strategy-design/wave54/lane-M.md. WAGIC_GPT_AUDIT_M_OFF=1 disables every
    // lane-M behaviour at runtime (the "was it me?" flag).
    int mAskCacheTurn = -1;          //A17: the turn mAskCache was last cleared on
    //#W66-AQ (H1): draws already RESOLVED inside the current draw step, per
    //seat, keyed on the turn they were counted in. Read by the DRAW FORECAST so
    //it charges only the draws still ahead; a stale turn key reads as 0.
    int mDrawStepTurn = -1;
    int mDrawStepDrawsMine = 0;
    int mDrawStepDrawsTheirs = 0;
    string mAskSituationPrefill;     //A19: a caller's already-rendered situation, consumed by the NEXT askModel
    int mWindowReach = -1;           //A21: potentialColorReach memo for one render window (-1 = not yet)
    bool mWindowReachArmed = false;  //A21: armed by chooseOrderedAction around its describeAction loop
    int windowReach();
    int mRepeatClickId = -1;         //L8: mtgid of mRepeatClick when the plan was armed (address-reuse guard)
    int mLoopClickId = -1;           //L8: mtgid of mLoopClick when the loop was noted

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
    //#W63-AD (E6c, engine HIGH-7): the system message is in ZERO logged prompts
    //(it is a system-role message, and `prompt` holds the user turn only), so
    //every protocol-wording claim of the last three waves shipped unreviewable.
    //Written once per seat-game as its own record, with the reply protocol
    //verbatim and a hash of the whole system message.
    void recordSystemPrompt();
    string describeDeckCards(Player * p, bool withCounts);
    string loadStrategyGuide();

    string serializeGameState(const std::string * optionText = NULL);
    //N-146k: the pregame (mulligan/bottoming/leyline) situation block. Hand only
    //- a pregame ask carries no board-state information at all.
    string serializePregameState();
    string describeAction(const OrderedAIAction& action);
    //#W41-6 / #W48-D13: the repeat-activation countable rendered onto an
    //option line. A member since W48 - it reads the loop-scoped count above,
    //which no free function can see.
    string repeatActivationNote(const OrderedAIAction& action);
    string stepKey() const; //#W50-Z (D12): "turn:phase"
    //#W50-Z (D18): the payment receipt, narrated as "Paid {cost} for X with A; B".
    virtual void notePaymentQueued(ManaCost * cost, MTGCardInstance * target, const vector<MTGCardInstance*>& sources);
public:
    //#W51-D (D5): the receipt text itself, "" when the cost carries no mana.
    //Static so the suite's scripted seat records the SAME line the live seat
    //narrates (a fixture then pins the receipt, not a copy of its format).
    static string paymentReceipt(ManaCost * cost, MTGCardInstance * target,
                                 const vector<MTGCardInstance*>& sources, int poolConvertedCost);
    //#W51-D (D18): the translog `turn` field = the narrated turn number.
    static int translogTurn(int observerTurn);
private:
    //#W48-D13: record that this seat has just taken (ability, click) at a
    //priority window, maintaining the loop-scoped consecutive count.
    void noteLoopTake(MTGAbility * ability, MTGCardInstance * click);
    //#W48-F1: end the repeat-N plan in flight and write its receipt into the
    //narration (how many of the named N actually happened, and why it stopped).
    //`why` empty means the plan ran to the count the model named.
    void endRepeatPlan(const char * why);
    string describeEvent(WEvent * event);

    //Assemble the user message: narration head, one fresh board snapshot,
    //the carried plan, then the decision-specific tail (question + options
    //+ reply format).
    string assemblePrompt(const string& tail);

    //--- audit-L (wave-54 audit lane L: GPT seat region 1 + transport) ------
    //A19: ONE heavy render for both the ask key and the prompt. serialize-
    //GameStatePair renders the situation once and returns the prompt variant
    //(the &tail render) while writing the key variant (the optionText == NULL
    //render) to *keyVariant - byte-identical to two separate calls by
    //construction, the own-battlefield block being the only difference.
    //assemblePrompt(tail, &situation) splices a situation the caller already
    //rendered instead of rendering again. The region-2 seams (lane M) call
    //these; a seam still calling the one-argument forms is unchanged.
    string serializeGameStatePair(const string& tail, string * keyVariant);
    string assemblePrompt(const string& tail, const string * situation);
    //#W62-fix (wave-61 corpus livelock): keyTail, when given, is what mPromptTail
    //(the seam half of the async slot key) is set to instead of `tail` - the
    //question and options WITHOUT the prompt-only notes spliced after them.
    string assemblePrompt(const string& tail, const string * situation, const string * keyTail);
    string serializeGameStateImpl(const std::string * optionText, std::string * keyVariant);
    //A24: HTTP status of the last consumed round trip (0 = no status came
    //back: transport-level failure or the Codex path; 200 = answered). Read by
    //noAnswerClass, stamped on the record as http_status, consumed once.
    long mLastHttpStatus;
    long mLastCurlResult; //#W59-H (K1): result for the last consumed attempt
    //#W59-H (K1): every failed transport attempt for the current decision,
    //including an attempt recovered by the one retry. Joined into the
    //translog's `transport` field and consumed with that decision record.
    std::vector<std::string> mLastTransportOutcomes;
    static const char * noAnswerClassFor(bool staleLivelock, bool timedOut,
                                         bool hasReasoning, long httpStatus);
    static const char * noAnswerClassFor(bool staleLivelock, bool timedOut,
                                         bool hasReasoning, long httpStatus, long curlCode);
    //#W60-Q (R9): the transport delivered a body and the CLIENT could not make
    //an answer out of it - a 200 whose JSON does not parse, or parses to a
    //shape with no message content (`{"choices":[]}`). curl is 0, the status is
    //200 and the body was not empty, so every existing class was false: it
    //filed as `empty_reply` and read as "the model said nothing", hiding a
    //protocol/schema failure that no amount of retrying the same endpoint fixes.
    static const char * noAnswerClassFor(bool staleLivelock, bool timedOut,
                                         bool hasReasoning, long httpStatus, long curlCode,
                                         bool badReply);
    static bool retryableTransportFailure(long curlCode, long httpStatus, bool emptyBody);
    static long remainingTransportRetryMs(long deadlineMs, long firstLatencyMs);
    static std::string transportOutcomeStamp(long curlCode, long httpStatus, bool emptyBody);
    //#W61-U (C13): the same stamp plus the two facts that make the failure's
    //PHASE provable (the connect budget in force, the elapsed round trip), and
    //the phase verdict itself. "" when the numbers do not settle it.
    static std::string transportOutcomeStamp(long curlCode, long httpStatus, bool emptyBody,
                                             long connectBudgetMs, long latencyMs,
                                             long deadlineMs);
    static const char * transportPhaseFor(long curlCode, long latencyMs,
                                          long connectBudgetMs, long deadlineMs);
    //L4: one translog stream per seat, opened on the first record (after the
    //-vs- rename), flushed per record, closed by the game-end record.
    std::ofstream mTransLog;
    void transLogWrite(const string& line);
    //Dev builds only (defined under _DEBUG || WAGIC_DEVLOGS): the render
    //byte-diff instrument, WAGIC_GPT_RENDERPROBE=<file>.
    void renderProbeDump();
    //--- end audit-L ----------------------------------------------------------
    //Append one line to the game narration (the model's own decisions join
    //the event narrative so the story stays complete without a transcript).
    void narrateDecision(const string& line);
    //W42-D2: the ability this action will activate IF clicking it raises
    //WEventAbilityActivated - NULL when nothing is announced (not an
    //ActivatedAbility, or a mana producer, which activateAbility returns on
    //before the announcement). Decides both the narration's verb and the stamp.
    ActivatedAbility * eventRaisingActivation(const OrderedAIAction& action);
    //Record that this seat has just narrated that activation as its own
    //consumed decision, so the event the click raises does not repeat it.
    void stampSelfActivation(ActivatedAbility * aa);
    //Consume one stamp for (source, abilityText); true means this seat already
    //wrote the line and the event must stay silent.
    bool consumeSelfActivationStamp(MTGCardInstance * source, const string& abilityText);
    //Bound-checked narration append; flushes the pending phase marker first.
    //#W43-11: `runTotal`, when >= 0, is the family total this line settled at
    //(a counter event's settledNb) - the resulting number a collapsed run of
    //identical lines is allowed to state.
    void appendNarration(const string& line, int runTotal = -1);
    //#W43-11: write (and clear) the pending run of identical lines - as the one
    //line itself when it never repeated, as both when it ran to two, and as ONE
    //collapsed line from three up. Runs wherever flushBulkMove runs.
    void flushEventRun();
    //#W43-11: the raw (pre-collapse) narration write. Everything appendNarration
    //did before the run buffer was put in front of it.
    void writeNarration(const string& line);
    //#W57-H (D43): the game-log window (WAGIC_GPT_LOGWINDOW). Default OFF -
    //logWindowApply then returns the narration unchanged. See the pure block
    //above assemblePrompt in the .cpp for the whole design.
    void logWindowSetting(int& mode, int& turns);
    bool logWindowStackRespondable();
    string logWindowApply(const string& narration, int * elidedTurns);
    string logWindowLabel();
    //#W57-D (D29): the adjacent-duplicate collapse at the write seam.
    //Returns true when the line was folded into the log's last line.
    bool collapseAdjacentDuplicate(const string& line);
    //#W43-11: narrate a day/night transition, once, when it actually changes.
    void noteDesignationChange();
    //#W50-X D14: narrate a permanent's chosen name once it is known.
    void noteChosenNames();
    //#W44-4: the game-wide day/night designation read LIVE off both
    //battlefields ("Day", "Night", or empty). Shared by the narration's change
    //line and the CURRENT SITUATION state line.
    string scanDayNightDesignation();
    //Zone-duty digest for the trim marker (see trimMarkerLine).
    string zoneNameDigest(MTGGameZone * z);
    //W41-3(c): write (and clear) the pending collapsed bulk move, if any. Must
    //run before ANY other narration line and before the prompt is assembled,
    //so the collapsed line lands in the log at the point the moves happened.
    void flushBulkMove();
    //#W42-D1: write (and clear) the pending library->reveal run - as ONE
    //sourced "searched their library" line when the run emptied the library,
    //otherwise as the named lines it buffered. Runs everywhere flushBulkMove
    //does, for the same reason: the log's ordering must be untouched.
    void flushSearchReveal();
    //#W42-D9: write (and clear) the pending pregame bottoming as one counted
    //line. Named to its owner only.
    void flushPregameBottom();
    //W43-R2: write (and clear) the held damage-caused life change as its OWN
    //standalone line. Only ever runs when the paired WEventDamage did not
    //arrive to carry it (no line is ever silently dropped).
    void flushDamageLife();
    //W41-3(c): the card whose spell/ability is resolving right now (the latest
    //NOT_RESOLVED stack object's source), used to attribute a bulk move.
    MTGCardInstance * resolvingStackSource();
    //#W42-D1: the card whose Reveal/Scry ability is walking `p`'s library now.
    //`wholeZone` (optional out) is the mechanism key: true when the live
    //ability walks the WHOLE zone (a search), false for a fixed reveal-N.
    MTGCardInstance * revealingAbilitySource(Player * p, bool * wholeZone = NULL);
    //Split a reply at its "PLAN:" marker: stores the (complete, per the
    //protocol) plan into mCurrentPlan and returns the decision part, which
    //is the ONLY text the choice parsers may see - plan prose is full of
    //numbers that would misparse as option indices.
    //`expectedLabel` (e.g. "CHOICE:") restricts answer-line selection to this
    //decision's own label, so a chain-of-thought line beginning with a
    //DIFFERENT answer label ("Attack: Regent 6/6 vs...") in the reasoning body
    //is not mistaken for the answer. NULL keeps the legacy any-label behavior.
    //choiceRunLen (out, optional): the length of the CONSECUTIVE run of
    //CHOICE: lines the answer line heads (>=2 = the model listed several
    //picks line-by-line for a single-pick ask; the FIRST of the run is the
    //answer taken - see findAnswerLabelLine).
    //#W50-Y D7: `rejectedLines` (out, optional) counts the line-leading coded
    //CHOICE lines the answer scan refused as rejections/unclean trailers.
    //#W63-AD (E6b): planAnswerNote reports whether a coded answer line inside the
    //PLAN BLOCK was demoted (1) or was the reply's only one and still answered
    //the window (2). 0 = neither. Report only; the seams stamp it.
    string consumePlan(const string& content, const char * expectedLabel = NULL,
                       int * choiceRunLen = NULL, int * rejectedLines = NULL,
                       int * planAnswerNote = NULL);

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
    //#W57-A (D5): the SECOND async slot. The land-drop arm and the casting arm
    //shared one slot, so an in-flight casting answer was thrown away the moment
    //a `Land drop:` ask reached the poll - 44 of the wave-56 corpus's 65 stale
    //drops, every one of the 32 preceded by the Baka opponent's own land
    //auto-tap among them. Each arm now owns its storage; CONCURRENCY is
    //unchanged (one round trip at a time across both arms, see asyncBusy), so
    //this separates what is remembered, not what is spent.
    std::shared_ptr<AsyncState> mAsyncLandState;
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

    //Turn on which the illegal one-blocker-many-attackers RE-ASK already
    //fired (W36 lane-B item 1, 116-fp8 vs105 seq25: "BLOCKS: B1:A1, B1:A2,
    //B1:A3" had its first pair taken and lost the game). One re-ask per
    //combat, with a terse correction appended to the prompt; a second
    //conflicted reply falls back to the shipped first-wins behavior.
    int mBlockReaskTurn;

    //#W55-B (D3 = R195). Turn on which the ALL-ILLEGAL blockers re-ask already
    //fired, and the pruned pairs that re-ask quotes back. Deliberately SEPARATE
    //from mBlockReaskTurn above: a one-blocker-many-attackers reply and a reply
    //whose every pairing was pruned as illegal are different failures, and one
    //firing must not spend the other's single arm. `152v146` seq 58: both
    //pairings under-filled a menace attacker, both were pruned, no blocks were
    //declared at a lethal window and no follower record was written at all.
    int mBlockIllegalReaskTurn;
    string mBlockIllegalReaskPairs;

    //Parse-shape signature for the NEXT translog record (parseChoice noteOut,
    //blocker re-ask provenance). Consumed and cleared by writeTransLog so a
    //note can never leak onto a later, unrelated record.
    string mLastParseNote;

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
    //#W65-AM (G7, deck123 HIGH-2, DOCTRINE): the BOARD the last decline of each
    //line was made over. A two-decline cap with no re-opener is a hard cap on
    //legal choices - 162 seq 66/69 retired the free {T} token-maker rows for the
    //turn, and when Intruder Alarm resolved in main 1 (seq 73) the menu held
    //only three equips, so the combo could not fire on the turn it assembled.
    //A decline is an answer about the board it was given on: when that board
    //moves, the allowance starts again. Nothing is cached blind and nothing is
    //rendered from this map - it never enters mPromptTail, the ask key or the
    //option-set key (the wave-61 livelock class is untouched).
    std::map<string, string> mPassDeclineBoard;
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

    //#W48-D13 (wave-47 docket D13) - the LOOP-SCOPED activation count.
    //`ActivatedAbility::counters` is the engine's own per-TURN number and is
    //reset to 0 at MTG_PHASE_AFTER_EOT; the loop it annotates does not stop at
    //a turn boundary. Measured on deck123 vs126, the [repeat:] tag read
    //"activated this turn 15 times already; you control 1539 creatures" on a
    //board that had already run 1,099 activations of the same loop - a fresh
    //small number under a huge board, and the deck's guide tripwire (">= 20")
    //could not fire in the game shape it was written for.
    //This pair counts CONSECUTIVE takes of the SAME activation by this seat
    //with no other action in between. It never resets on a turn boundary; it
    //resets on a different action, on a pass, and on a heuristic answer - i.e.
    //exactly when the loop it is counting has actually been broken. Pointers
    //are compared, never dereferenced.
    MTGAbility * mLoopAbility;
    MTGCardInstance * mLoopClick;
    int mLoopCount;

    //#W48-F1 - the repeat-N plan in flight (wave-47 docket D1, affordance F1).
    //The model names N once, on one CHOICE line; this seam then dispatches the
    //SAME activation one per AI tick - never a synchronous click burst, which
    //the latent ability-GC double-destroy makes fatal (DecisionContract design
    //section 4) - re-checking payability by the only honest test available
    //here: is the option still in the engine's ranking THIS tick. It is not a
    //cap: the row is an addition to the menu, nothing is removed, and the plan
    //ends the moment the engine stops offering the action.
    //mRepeatAbility/mRepeatClick are compared, never dereferenced, and the plan
    //is bounded by mRepeatRemaining, so a freed ability can at worst end it.
    MTGAbility * mRepeatAbility;
    MTGCardInstance * mRepeatClick;
    int mRepeatRemaining;
    int mRepeatTotal;
    int mRepeatDone;
    //Board serialization at the previous repeat dispatch, and the count of
    //consecutive dispatches that did not move it: a repeat whose activations
    //change nothing is not a repeat, it is a stall, and it ends.
    string mRepeatBoardKey;
    int mRepeatNoProgress;
    //Consecutive windows the plan's option has been missing from the menu while
    //something is still resolving on the stack (the loop's own untap trigger).
    int mRepeatAbsent;

    //#W46-7: priority windows auto-passed because every option was a mana
    //activation and nothing was pending payment. Counted, not silent: an
    //auto-pass writes no per-decision record, so the closing gameend record
    //carries the count - a corpus can still see how many windows the seat was
    //offered and how many of them never reached the model.
    int mManaOnlyWindowsSkipped;
    //#W54-D (D8b): asks whose entire option list rendered as one
    //interchangeable, ordinal-free row and were answered without a model call.
    int mIdenticalOptionAsksResolved;
    //#W59-J (K10, wave-58 deck126 HIGH-2): the REPEATED ask. While a decided
    //Sanguine Bond / Exquisite Blood drain resolved, the seat was handed the
    //same two-row menu 32 times in one turn's upkeep (20 more in a second
    //game): byte-identical rows, byte-identical plan, only the life totals in
    //the header moving - and every one of them was a model round trip, because
    //the state-plus-question cache keys on the rendered BOARD and the board
    //moved every iteration. This is the same answer, re-served: the latch holds
    //the seat's OWN last answer for this turn, this phase, this decision and
    //this exact printed row list, and is dropped the instant any of them moves.
    //Not a cache of the board (the doctrine's blind cache): a row whose price
    //changed is a different row list, and a plan the model has since rewritten
    //re-opens the question. mRepeatAskTurn < 0 = nothing latched.
    string mRepeatAskKey;
    string mRepeatAskPlan;
    int mRepeatAskTurn;
    int mRepeatAskChoice;   //the CALLER's 1-based index, exactly as returned
    int mRepeatAskAnswersReserved; //gameend report field
    //#W50-Z (D12): the "turn:phase" step in which THIS seat last took a mana
    //activation of its own choosing. Floating mana keeps a mana-only window
    //open only in that step (the seat is mid-float by its own decision); mana
    //left floating by anything else - an auto-tap overpay, a countered cast's
    //residue (D1) - buys nothing on an all-mana menu and no longer leaks the
    //window to the model.
    string mSeatFloatStepKey;

    //W42-D2 activation de-dup. WEventAbilityActivated is the ONE source of
    //truth for activation lines and fires on both seats; where this seat has
    //ALREADY written the same activation as a consumed decision, the event must
    //not write a second line. Keyed by the two values the event itself carries -
    //the ability's own source card and its menu text - so the match holds
    //however the decision line was rendered (a pump activation narrates its
    //delta, not its menu text). A COUNT, not a flag: the same ability activated
    //twice in a window suppresses twice and no more. Cleared on turn change so
    //a decision that never reached the click path cannot swallow a later
    //genuine activation. Pointers are compared, never dereferenced.
    std::map<std::pair<MTGCardInstance *, string>, int> mSelfActivationStamp;

    //Cast-seam livelock breaker (the priority seam's no-progress pass,
    //mirrored): a consumed cast pick that leaves the board byte-identical
    //did not execute (an unexecutable menu entry - e.g. a restricted cast
    //mode - or an engine no-op). The cached ask would replay it every tick
    //forever (135v133 wedged at turn 2 for 2400s, 903k re-picks). Suppress
    //that option line for the turn and re-ask over the remaining options -
    //self-healing when a sibling entry (the legal alternative mode) exists.
    string mLastCastBoard;
    string mLastCastLine;
    std::set<size_t> mStuckCastLines; //#W54-M (L6): std::hash of the line - equality is all that is tested
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
    //#W55-E (D23): write the abandoned wall-missed ask down before it is lost.
    void flushWallMissRecord();
    void writeTransLog(const char * kind, const string& userMsg, const string& reply, int choice, int optionCount,
                       const string& chosenText = "", const char * fallback = NULL,
                       const std::vector<string> * optionTexts = NULL,
                       const char * choiceSource = NULL);
    //One "gameend" record per duel: result + final life + turn count. The
    //per-decision records alone cannot say who WON - win-rate and timeout
    //adjudication both need it in the same file.
    bool mGameEndLogged;
    bool mGameStartLogged; //header record emitted (lazily, first write)
    //Each record carries the narration DELTA - the events that landed since the
    //previous record - so a consumed cast's OUTCOME is machine-readable
    //(wave-7 7b). W42-D8: this used to be a byte OFFSET into mNarration
    //(`substr(mNarrationLogged)`), but mNarration is NOT append-only - a game
    //past 24k chars is trimmed FROM THE FRONT - so a stale absolute offset
    //indexed into a shifted buffer and sliced a line mid-word (wave-41
    //1787543863-deck130 seq 62 began "f Nin into their library"). The delta is
    //now its own accumulator, written line-at-a-time alongside the log and
    //consumed/cleared by each record: line boundaries are structural, so no
    //record can begin mid-line and the trim cannot lose or duplicate a line.
    string mNarrationPending;
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
    //#W50-Y D19: the fallback class of a reply that carried no usable answer -
    //"degenerate_decode" when isDecodeGarbage says the text is a decode
    //collapse (mojibake, CJK spray, punctuation soup, a looping token), else
    //"unparsed_reply" (real prose that simply never coded an answer).
    static const char * unparsedReplyClass(const string& content);
    //Serialize the chat request (system prompt + the pending user message)
    //for the worker thread; built on the game thread, nothing shared.
    string buildRequestBody(const string& userMsg);
    //Extract the chosen action number from a model reply; -1 if unusable.
    //staleEcho (out, optional) is set true when the name-echo parses to
    //significant words that match NO offered option - a staleness signal
    //that routes the answer to the heuristic instead of the raw index.
    //noteOut (out, optional) receives a parse-shape signature for the
    //translog when the reply expressed more than one pick for a single-pick
    //ask ("multi_answer_first_taken" / "multi_answer_unresolved") or when the
    //echoed name and the coded index named DIFFERENT options
    //("echo_index_conflict" - the unique name wins; "_ambiguous" - the index
    //stood because the echo matched several options, none of them the
    //index's own). Silent divergence between what the model said and what
    //executed was the W36 lane-B items 2-4 instrument gap.
    //#W53-N (D9): passRowOffered says this seam carries a row 0 = pass
    //priority. Where it does, the reserved echoes "pass" / "pass priority" /
    //the pass row's own full text / "0" NAME row 0 and resolve there, whatever
    //index the line carries (deck146 vs152 seq 91: "CHOICE: 1 (Pass)" executed
    //Kaya's +1). Where it does NOT (every askModel menu - kNoPassRowFact says
    //so on the format line), the flag stays false and nothing changes.
    static int parseChoice(const string& content, int optionCount,
                           const std::vector<string> * optionTexts = NULL,
                           bool * staleEcho = NULL,
                           const std::string * pendingSource = NULL,
                           std::string * noteOut = NULL,
                           bool passRowOffered = false);

    //Salvage a decode-time repeat-loop reply: when the normal parse fails,
    //scan the raw reply for the LAST well-formed "CHOICE: N (name)" line
    //(one the model stated before spiraling) and re-parse it through the
    //same echo/staleness checks. Returns the 1-based choice or -1. Never
    //bypasses stale_echo protection (a stale line re-parses to -1 here too).
    //#W50-Y D9: `minChoice` = 1 skips coded 0 lines - on an ask with no pass
    //row a "CHOICE: 0 (pass)" sibling is not an offered choice.
    //#W50-Y D7: rejection lines (see choiceLineIsRejection) are never taken;
    //the last CLEAN line wins, else the last non-rejection line.
    static int salvageLoopedChoice(const string& content, int optionCount,
                                   const std::vector<string> * optionTexts = NULL,
                                   int minChoice = 0);
    //#W49-S (D2): the FIRST well-formed line-leading CHOICE line's parse (-1
    //when none parses) - the seams compare it with what executed so
    //answer_replaced states whether the EXECUTED answer differs from the
    //first coded one, not merely whether the reply's first and last coded
    //lines differ as strings.
    static int firstCodedChoice(const string& content, int optionCount,
                                const std::vector<string> * optionTexts = NULL);

    //True when a well-formed CHOICE was superseded by an explicit
    //self-retraction with no replacing CHOICE - the answer must route to the
    //heuristic instead of using the retracted digit (deck135 HARNESS-1).
    //#W49-S (D2): `namedRowNotOffered` is set when the trailing re-answer's
    //parenthetical names a row that is NOT on the menu at that index (or any
    //index) - the earlier, offered answer is kept and the function returns
    //false; the caller records `named_row_not_offered` as a parse note.
    //#W62-Z (D9): the LAST combat directive the model re-states in prose after
    //its coded answer line and before its PLAN: line, or "" when there is none.
    static string restatedCombatDirective(const string& content, const char * label,
                                          const std::vector<string> * rosterA = NULL,
                                          const std::vector<string> * rosterB = NULL);
    static bool choiceRetractedNoReplacement(const string& content, int optionCount,
                                             const std::vector<string> * optionTexts = NULL,
                                             int * replacement = NULL,
                                             bool * namedRowNotOffered = NULL,
                                             bool * planProseRecodeOut = NULL); //#W62-Z (D10)

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
    string mSystemHash; //#W63-AD (E6c): hash of mSystemPrompt, stamped on every ask record
    //append-only game narration: every noteworthy event plus the model's
    //own consumed decisions, as if narrating the game (bounded, tail kept)
    string mNarration;
    //#W57-H (D43): the ask class of the window currently being assembled (a
    //GptAskWindowKind), set by each seam before it calls assemblePrompt, and
    //the number of turns that window's log elided (0 = none, and 0 whenever
    //the lever is off). Both ride the translog record.
    int mLogWindowKind;
    int mLogWindowElided;
    //#W57-D (D29): the last line WRITTEN, its rendered form as it stands
    //in the log right now, and how many identical occurrences that form
    //already carries. Empty/0 whenever the tail is not a countable run.
    string mRunLastLine;
    string mRunLastRendered;
    int mRunLastCount;
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
    //#W49-U D7: consecutive replies whose PLAN: was byte-identical to the one
    //already carried. #W50-Y D10: a REPORT FIELD only (translog plan_echo_count)
    //- the wave-49 "5 echoes expire the carry" trigger is RETIRED; expiry is
    //keyed on the plan's CONTENT (gptcaveat::planOpensWithVerdict /
    //planNamesNoAction), never on how often the model re-states it.
    int mPlanEchoCount;
    //W38 mutate host-intent carry (wave-37 validation #3, 139v152 s30-31):
    //the over/under placement and the host pick are SEPARATE model calls,
    //and the pilot's host intent did not survive the boundary (chose "over"
    //intending Dryad, then picked Gemrazer as host). Record which card's
    //over/under menu was just answered and the PLAN stated with that answer,
    //so the host ask can render the pilot's own commitment back to it
    //(annotation, never automation - the model may still override). Cleared
    //when the host ask consumes an answer.
    string mMutateIntentCard;
    string mMutateIntentPlan;
    //#W41-7 mass accept/deny. When ONE `may` ability fires N>1 times in a
    //single window with an identical, target-free effect (Perimeter Captain's
    //"a defender you control blocked -> you may gain 2 life" raised EIGHT
    //identical asks in one combat; 22 asks cost 1,377s of inference and 132k
    //reasoning characters for 22 identical answers), the seat asks ONCE:
    //accept all / decline all / decide individually. The verdict below then
    //answers the remaining siblings locally, with NO model call.
    //Structurally safe by construction: "decide individually" restores the
    //per-instance flow exactly, so the batched ask can never make an answer
    //impossible, and the engine still resolves every trigger separately - this
    //batches the ASKING, never the effect. AI seat only; no human UI changes.
    enum MayBatchVerdict { kMayBatchNone = 0, kMayBatchAcceptAll, kMayBatchDeclineAll, kMayBatchIndividual };
    string mMayBatchKey;        //DecisionManager::MayBatch::groupKey it applies to
    int mMayBatchVerdict;       //MayBatchVerdict
    int mMayBatchRemaining;     //asks still covered; the verdict expires at 0
    //Cards the opponent revealed that are now in their hand: public info a
    //human would remember. Tracked by name (instances are recreated on zone
    //moves), decremented when a card of that name leaves the hand.
    std::map<string, int> mKnownOppHand;
    //W35 narration register, owner addendum (4): "stack -> graveyard" is the
    //SAME move for a spell that resolved and a spell that was countered.
    //ActionStack::Fizzle raises WEventSpellCountered immediately before it
    //moves the countered card and the observer's event queue is FIFO, so this
    //marker names the card whose NEXT zone event is the countering move. It is
    //consumed by that line and can never colour an unrelated later move.
    MTGCardInstance * mCounteredSpell;
    string mCounteredBy;
    //W35: per-option register lines for the NEXT askModel call, set by a caller
    //that can state the CONSEQUENCE of each option ("You mulliganed to 6",
    //"You targeted X with Y"). Consumed (and cleared) by askModel, which
    //otherwise falls back to a generic "You chose <option>" - never the
    //question header, never the decorated option text.
    vector<string> mNextAskNarration;
    void setAskNarration(const vector<string>& lines) { mNextAskNarration = lines; }
    //N-105a: last-narrated poison total per player ([0] this seat, [1] the
    //opponent), so a poison-gain line can state the real DELTA. The engine's
    //own WEventplayerPoisoned payload cannot be trusted for it - the toxic
    //path (Damage.cpp:239) fires the event carrying the DAMAGE dealt, not the
    //toxicity granted - while Player::poisonCount is settled by the time the
    //event lands, so the delta is derived from the settled totals instead.
    int mLastPoison[2];
    //#W57-E (D15): the opponent's life at this seat's first look in each of the
    //last three turns, newest at [2], with the turn number each sample belongs
    //to. mOppLifeLastTurn is the sampling gate (-1 = nothing sampled yet).
    int mOppLifeByTurn[3];
    int mOppLifeTurnNo[3];
    int mOppLifeSamples;
    int mOppLifeLastTurn;
    //W43-R2 (owner report: "damage is receiving 2 entries, which may be
    //confusing to the model, and is also unnecessarily verbose"). Damage::
    //resolve raises WEventLife(fromDamage) and then the WEventDamage for the
    //SAME delta, so the log printed the effect ("- You lost 1 life (now 34)")
    //and its cause ("- Dwarven Blastminer dealt 1 damage to you") as two
    //entries. The flagged life event is HELD here instead of printed, and the
    //damage line that follows carries the result: "- Dwarven Blastminer dealt 1
    //damage to you (now 34)". `mDamageLifePlayer` non-NULL = one is held.
    //
    //SIMULTANEOUS MULTI-SOURCE DAMAGE: each Damage::resolve raises its own
    //life+damage pair in sequence, and settledLife is captured at fire time, so
    //EVERY damage line carries the running total after ITS OWN damage - the
    //last line therefore shows the final total, and the intermediate ones are
    //true statements about the step they describe (the same shape wave-34 b6 F5
    //established for batched life changes).
    //
    //PREVENTED / REPLACED damage never reaches the raise site (Damage::resolve
    //returns at `if (!damage)`, and the infect/wither branches replace the
    //damage with counters and raise no life event at all), so nothing is held
    //and no life result is printed for damage that caused none.
    Player * mDamageLifePlayer;
    int mDamageLifeAmount;
    int mDamageLifeSettled;
    //#W43-11 / #W48 (D11): the pending REPEATING BLOCK of narration lines, held
    //rather than written until the repetition breaks or the prompt is assembled -
    //the same flush-buffer discipline W41-3(c) and #W42-D1 use, so the collapsed
    //lines land in the log exactly where their repetition happened. #W43-11
    //collapsed a run of ONE byte-identical line; the loop it was aimed at repeats
    //a CYCLE ("You used: Create vampire" / "Your Lord of Lineage created a 2/2
    //Vampire token", alternating ~180 times - ~90% of a 26 KB prompt, with no
    //count anywhere), so the holder is a cycle of up to kNarrationCycleMaxPeriod
    //lines and the single-line run is the period-1 case of it.
    //`mCycleLines` is the established block (empty when none), `mCycleReps` its
    //completed repetitions, `mCycleTotals` the per-position family total the LAST
    //repetition settled at (a counter event's own settledNb, or -1 when the
    //emitter has none - the only number a collapsed line may claim), and
    //`mPendLines`/`mPendTotals` the lines received since the last complete
    //repetition, which double as the candidate buffer while no cycle is held.
    //#W48 (D11): the run holder above is RUN-LENGTH over a single line, and the
    //loop it exists for does not repeat one line - it repeats a CYCLE ("You used:
    //Create vampire" / "Your Lord of Lineage created a 2/2 Vampire token",
    //alternating ~180 times, ~90% of a 26 KB prompt with no count anywhere).
    //`mCycleLines` is the established repeating block (empty when none),
    //`mCycleReps` its completed repetitions, `mCycleTotals` the per-position
    //family total the LAST repetition settled at, and `mPendLines`/`mPendTotals`
    //the lines received since the last complete repetition - which double as the
    //candidate buffer while no cycle is established. Same flush-buffer discipline
    //as every holder above: flushEventRun writes them, clearing before it writes.
    NarrationCycleHolder mEventRun;
    //#W43-11: the game-wide day/night designation as this seat last narrated it
    //("Day", "Night", or empty for neither). The marker card itself is filtered
    //off every object surface, so this is the only place the fact lives.
    string mDayNight;
    //#W50-X D14: (instance, chosen name) pairs already narrated.
    std::set<string> mNarratedChosenNames;
    //W41-3(c): the pending bulk graveyard->library run. `mBulkMoveFirstLine` is
    //the ordinary named line for the FIRST move, kept so a run of length one
    //flushes as itself rather than as a "1 card" count.
    int mBulkMoveCount;
    bool mBulkMoveMine;
    string mBulkMoveFirstLine;
    string mBulkMoveSource;
    //#W42-D1: the pending library->reveal run. `mSearchRevealLines` holds the
    //ordinary named line for EVERY card in the run, so a run that turns out to
    //be a genuine reveal (not a whole-library search) replays unchanged.
    //`mSearchIsWholeZone` is the MECHANISM key, read off the live ability's
    //script ("Reveal:type:..." = a search); `mSearchIsFullDump` (the origin
    //library hit zero) is only the fallback for a path with no ability to ask.
    std::vector<string> mSearchRevealLines;
    bool mSearchRevealMine;
    bool mSearchIsFullDump;
    bool mSearchIsWholeZone;
    string mSearchRevealSource;
    Player * mSearchRevealOwner;
    //#W42-D1: while set, this player's reveal->library moves are the return
    //trip of a search already narrated as one line - a no-op the model must
    //never read as N events (and, on the observer seat, never read at all).
    Player * mSearchMaskOwner;
    //#W42-D9: the pregame bottoming, collapsed. `mPregameBottomingNow` is armed
    //by pregameChooseBottom for exactly the one hand->library move the engine
    //makes with the card it just handed back, so a real turn-1 hand->library
    //move can never be swallowed by a window guess.
    bool mPregameBottomingNow;
    bool mPregameShufflingBack;
    int mPregameBottomedCount;
    string mPregameBottomedNames;
    //Avoid re-querying the model every AI tick while nothing changed. Keyed
    //by board state + question (not the full prompt) for the same reason as
    //mAskCache: taking an action narrates it, and a full-prompt key would
    //read that as "state changed" and defeat the deadlock breaker.
    string mLastAskKey;
    int mLastChoice;
    //#W49-S (D8/D3): the priority seam's ONE re-ask per board state. When a
    //reply names an index past the menu with a card that matches no row
    //(D8), or takes the repeat row without naming its count (D3), the
    //question is re-put once with a terse correction line appended. Keyed on
    //the serialized board so the corrected question has its own askKey (a
    //different question, a fresh call) and so the second answer, whatever it
    //is, is final for this state. Cleared when the board moves on.
    string mPriorityReaskBoard;
    string mPriorityReaskLine;
    string mPriorityReaskKind; //"named_row" | "repeat_count" | "plan_choice" (#W51-C D4) | "plan_missing" | "index_name" (#W52-J)
    //Same mechanism for askModel (cast menus, targets, modes): keyed on the
    //ORIGINAL askKey (state + question), the corrected question is asked once.
    string mAskReaskKey;
    string mAskReaskLine;
    string mAskReaskKind; //"named_row" | "no_pass" (#W50-Y D9)
    //#W60-M (B3): the cleanup-discard channel's own one re-ask. Keyed on the
    //ask TEXT (the hand and the count are in it), cleared when the discard
    //completes. The discard prompt states it is the only ask for those cards,
    //so a reply that repeats an index and lands short had no recovery at all.
    string mDiscardReaskKey;
    string mDiscardReaskLine;
    //#W60-M (B13c): true when askModel answered from the K10 latch or the
    //state+question cache - a window the model was NOT shown. The declined-list
    //counter (and anything else reporting what the MODEL did) must not count it.
    bool mAskAnswerReserved;
    //#W51-C (D3): the rows of the LAST consumed window (priority or ask). An
    //in-range index whose parenthetical names no row on THIS menu is a stale
    //echo only when the name echoes one of these rows; otherwise it is an
    //off-menu name and earns the one named_row re-ask, never Baka.
    std::vector<string> mPrevWindowRows;
    //#W53-N (D12b): the 1-based ordinal, among the reply's line-leading
    //CHOICE: lines, of the first that resolves to <choice>. On a record with
    //coded_answers >= 2 it is the only way to tell WHICH of the model's coded
    //lines the engine latched - the five wave-52 plan_choice_conflict records
    //carried the FIRST answer in `reply` and the LATCHED row in `choice` with
    //nothing tying the two together. -1 when no line resolves to it.
    static int codedChoiceOrdinal(const string& content, int choice, int optionCount,
                                  const std::vector<string> * optionTexts);

    //#W56-C (D3): the same walk, reporting the LATCHED line's TEXT and where it
    //starts in the post-think reply, so a re-ask can quote what the engine ran
    //and read its prose evidence from that line's own region.
    static bool latchedCodedChoiceLine(const string& replyIn, int choice, int optionCount,
                                       const std::vector<string> * optionTexts,
                                       string * lineOut, size_t * fromOut);

    //#W49-S (D2): answer_replaced is FALSE whenever the answer that EXECUTED
    //is the reply's first coded line - the seams set this right before their
    //translog write; writeTransLog consumes and clears it.
    bool mAnswerReplacedFalse;
    //#W49-S (D8, the wave-47 D10 sentence): the turn/phase on which this
    //seat's Casting decision was actually put to the model, so the priority
    //ask that follows it in the same phase can say the casting question is
    //already answered (the off-menu "CHOICE: 8 (Cast Acererak)" shape).
    int mCastAskTurn;
    int mCastAskPhase;
    //#W53-N (D2): the model-owned HOLD. mHoldTurn is the turn it was taken on;
    //mHoldRows records, per seam, the rows that were PRINTED when it was taken.
    //#W56-A (D1): the rows are the whole predicate - the board key is gone, and
    //a row whose printed text moved re-opens the window. Nothing here is a
    //cache of an ANSWER: the engine only replays a hold the model itself chose,
    //and only while the screen it was chosen on is still the screen.
    int mHoldTurn;
    std::map<string, std::set<string> > mHoldRows;
    int mHoldWindowsSkipped; //gameend report field
    //#W57-A (D4): the repeat count of the take being logged, consumed by
    //writeTransLog into `repeat_n`. Report only; nothing in the engine reads it.
    int mLastRepeatN;
    //#W53-N (D2, second half): per-turn declines of an EXACT option list,
    //keyed by the joined rows. Rendered as a PROMPT-ONLY annotation (never
    //part of an ask key - see declinedListNote).
    std::map<size_t, int> mListDeclineCount; //#W54-M (L6): keyed by std::hash of the joined rows
    int mListDeclineTurn;
    //#W57-B (D6): the opponent's declared attack, latched for the combat it
    //belongs to. The engine's `attacker` flag is false before the declaration
    //and clears once combat is over, so without this the incoming total is
    //absent from most of the windows during their combat. mIncomingCombatTurn
    //< 0 = no latch; it is only ever read for the turn it was written on.
    int mIncomingCombatTurn;
    int mIncomingCombatAttackers;
    int mIncomingCombatDamage;
    //#W53-N (D12a): when the carried plan was last WRITTEN by the model -
    //the translog seq (= window) and the turn. mPlanSetSeq < 0 = no plan.
    int mPlanSetSeq;
    int mPlanSetTurn;
    //#W53-N (D2): a prompt-only note for the NEXT askModel call, spliced in
    //after its option list and deliberately kept out of its cache key.
    string mNextAskPromptNote;

    //#W53-N (D2): honour a hold the model took, at seam <seam>, for a window
    //whose PRINTED rows are <rows> (#W56-A D1: the rows are the key). Returns true
    //when the hold still stands (the caller passes without a model call);
    //clears the latch and returns false on any re-opener.
    bool holdHonoured(const char * seam, const std::vector<string>& rows);
    //#W61-U (C14): the prompt-only note stating which of the two hold regimes
    //this menu is in, measured against the previous window's rows at this seam.
    string holdReopenNote(const char * seam, const std::vector<string>& rows);
    std::map<string, std::set<string> > mLastMenuRows; //per seam, previous window
    std::map<string, int> mMenuRepeatRun;              //consecutive unchanged windows
    //#W62-fix: the translog seq at which each seam's note was last MEASURED. A
    //prompt is rebuilt every tick while an answer is in flight; the same window
    //re-rendered (same seq, same rows) must return the same note, not count.
    std::map<string, int> mMenuRepeatSeq;
    std::map<string, string> mMenuRepeatNote;
    string mCastHoldNote;                              //measured once per cast window
    //#W53-N (D2): record the model's hold answer at this seam.
    void takeHold(const char * seam, const std::vector<string>& rows);
    //#W53-N (D12a): ", N windows ago on turn T" for the carried plan, empty
    //when the model stated it at the window now being rendered.
    string planAgeClause() const;
    //#W56-A (D18): the async slot's identity - (seam, turn, phase, boardKey).
    //mPromptTail is the seam: the question-and-options tail assemblePrompt was
    //last handed. An answer whose slot still matches is an answer to the
    //question being asked now, whatever the narration header did meanwhile.
    string mPromptTail;
    string asyncSlotKey(const string& userMsg);
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
    //#W59-H (K1): the deadline the retry currently in flight must run under.
    //0 = the ordinary full decision deadline (the D10 wall-miss retry and the
    //force-close/answer-lock retries all keep that); a POSITIVE value is the
    //remainder of this decision's deadline, which is what a TRANSPORT retry
    //gets so that first attempt plus retry never exceed one deadline.
    long mRetryBudgetMs;
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
    //#W53-Q (D10): the same table as a PURE function of the three facts that
    //decide it, so the classification is provable in PARSETEST without a game
    //or an endpoint. `timedOut` is the HTTP deadline expiring with nothing
    //back - a 900 s non-answer that used to be filed as "empty_reply" and read
    //as an endpoint fault. Precedence: a livelock give-up first (the engine
    //stopped asking), then the deadline (an empty body cannot be reasoning),
    //then the transport/model split the wave-34 A/B is scored on.
    static const char * noAnswerClassFor(bool staleLivelock, bool timedOut,
                                         bool hasReasoning);
    //#W53-Q (D10): the last consumed reply came back EMPTY at the wall - the
    //worker's round trip reached the configured timeout. Set on consume,
    //cleared by any reply that carried a body, and read by noAnswerClass and
    //by the one-retry gate. A retry is fired ONCE per decision; a second wall
    //hit hands the decision to the heuristic with the stderr line printed.
    bool mLastTimeout;
    //#W60-Q (R9): the last consumed reply CARRIED a body that yielded no
    //content - JSON that would not parse, or a schema with no
    //choices[0].message.content. Set on consume, cleared by any reply that
    //produced content, read by noAnswerClass.
    bool mLastBadReply;
    //#W57-U (D-U, the vpk16 in-flight softlock): the last poll ABANDONED a
    //request that had been in flight past its own deadline plus a grace with
    //nothing published. Latched in SECONDS so the translog record for the
    //decision the heuristic then answered names the cause
    //(`fallback: abandoned_in_flight`) and how long the call had been out.
    //-1 = nothing to stamp. A REPORT plus one fallback class; no dial rides it.
    long mAbandonedInFlightSecs;
    //#W57-U: how long ONE in-flight request may run before the game thread
    //abandons it - the request's OWN deadline (mTimeoutMs, i.e. WAGIC_GPT_TIMEOUT
    //or config `timeout=`) plus a grace of half that deadline clamped to
    //[1 s, 30 s]. Honouring the deadline is the transport's job; this is the
    //bound for a transport that does not do it (a wedged socket, a worker that
    //died without publishing), which is otherwise unbounded because
    //decisionPending re-extends the interrupt offer every tick.
    long inFlightAbandonMs() const;
    //#W57-U: the same arithmetic, PURE, so PARSETEST pins the bound itself
    //rather than a run that happened to fire. Public and static for that.
public:
    static long inFlightAbandonMsFor(long deadlineMs);
private:
    //#W57-U: abandon any arm whose request has passed inFlightAbandonMs with
    //nothing published. Returns true iff the arm the caller is POLLING was the
    //one abandoned - the caller then answers this decision through the same
    //heuristic path a refused worker thread takes.
    bool reapWedgedRequests(const std::shared_ptr<AsyncState>& polled, const char * polledArm);
    //#W53-Q (D24): the seq/class of the last record that handed its decision
    //to the heuristic (choice < 0 with a fallback class). The NEXT record - or
    //the game-end record - flushes a `recovery` record naming it and the
    //narration the heuristic produced, so "nothing says what answered" cannot
    //stand. -1 = nothing pending.
    int mRecoverySeq;
    string mRecoveryClass;
    string mRecoveryKind;
    void flushRecoveryRecord();
    //#W53-Q (D24): the latch's gate, pure so PARSETEST can pin it.
    static bool handedToHeuristic(int choice, const char * fallback);
    //#W54-B (D9): a reply that ANSWERED after eating the whole deadline is
    //not a timeout - noAnswerClassFor only ever classifies a NON-answer, so
    //the wave-53 corpus's six replies above 600 s (one at 868,729 ms = 96.5%
    //of a 900 s wall, and the seat that lost that game) arrived at review
    //indistinguishable from a five-second one. These two are the stamp's
    //arithmetic, pure so PARSETEST pins the whole table. deadlineTenthsPct
    //returns the elapsed fraction in TENTHS of a percent (965 = 96.5%), -1
    //when unknowable (a cache/reuse record carries latency -1). isLongReply
    //is the >= 95% gate the wave-53 timeout arm already uses, on the ANSWERED
    //side of it. Neither touches WAGIC_GPT_TIMEOUT: the deadline is the
    //owner's dial and this item is observability.
    static long deadlineTenthsPct(long latencyMs, long timeoutMs);
    static bool isLongReply(long latencyMs, long timeoutMs, bool answered);
    //#W54-B (D13): the latched coded line's index AND its parenthetical both
    //disagree with the row that EXECUTED (deck126 vs125 seq 13/14: "CHOICE: 2
    //(Cast Idyllic Tutor)" on a three-row menu, row 1 Cast Perimeter Captain
    //ran, no note anywhere). A legitimate name-over-index remap keeps its
    //parenthetical on the executed row and is never stamped. Pure.
    static bool latchedRowMismatch(const string& reply, int choice, int optionCount,
                                   const std::vector<string> * optionTexts);
    //#W54-B (D14): the chosen row's OWN annotation says the action does
    //nothing, and the reply's PLAN argues against that same row (deck126
    //vs125 seq 73/74: "CHOICE: 1 (Cast Tribute to Hunger)" + "PLAN: ... Avoid
    //casting Tribute to Hunger as there are no creatures to target."). The
    //pair is a STAMP only - no re-ask, no suppression: whether it should ever
    //narrow a choice is a decision for after the count exists. Pure.
public:
    //#W57-C (D8/D12): PUBLIC because the zero-predicate is now the shared
    //definition of "dead" across three surfaces - the cast row that computes
    //it, the discard row that re-badges its verdict into the same family, and
    //the menu-level header that fires only when every cast row carries one.
    //Two surfaces that disagree about what "dead" means are the exact asymmetry
    //D8 was filed against, so they read ONE function.
    static bool rowSaysNoOp(const string& row);
    //#W58-C (D2): the MENU-level fold. A `{right now:}` verdict is a list of
    //per-scope verdicts; the header may claim a row reads zero only when EVERY
    //scope of it does, judged on the text outside each scope's parenthetical
    //qualifier. PUBLIC so PARSETEST pins it without a board.
    static bool verdictReadsZero(const string& verdictClause);
private:
    static bool planArguesAgainstRow(const string& reply, const string& row);
    //#W54-F (D7b): an engine-answered decision (no prompt, no reply, nothing
    //executed) has no class of its own and so was invisible to the recovery
    //contract above. Pure, pinned in PARSETEST.
    static bool engineAnsweredNoModel(int choice, const char * fallback,
                                      bool emptyPrompt, bool emptyReply);
public:
    //#W54-F (D7a): the engine resolved a decision this seat could not - the
    //reveal driver's stall force-close. Writes a translog record so a
    //force-close is never silent and earns its own recovery record.
    virtual void logEngineResolution(const char * kind, const string& what,
                                     int optionCount, const char * fallbackClass);
    //#W55-E (D5a): the reveal driver's own no-structural-progress figures for
    //the tick about to run. Consumed by the next `reveal` record as
    //reveal_stall / reveal_stall_secs / reveal_stall_phase - a REPORT, nothing
    //in the engine reads it.
    virtual void noteRevealStall(int ticks, long secs, int driverPhase, bool parked,
                                 int driverTicks = 0, long driverSecs = 0);
    //#W55-E (D5a): the deadline the reveal stall guard sizes its wall floor on.
    virtual long decisionDeadlineMs() { return mTimeoutMs; }
private:
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
    //LIVELOCK BREAKER (2026-08-21, 146v36): consecutive stale-answer drops
    //with no consume in between. A drop is legitimately rare (~2%, state
    //moved mid-flight); a RUN of them means the prompt text is not stable
    //for an unchanged state (the Kaya-menu random-rank reorder livelocked a
    //seat for 4.6h). At kStaleLivelockLimit the ask gives up to the bounded
    //heuristic fallback for THIS decision instead of looping forever.
    int mStaleDropStreak;
    //#W52-G (D-1): the board the streak is being counted ON. A drop is only a
    //livelock symptom when the state did NOT move between drops; a drop
    //because the state legitimately moved (a scripted repeat plan advancing
    //the board every tick) is the async design working, and restarts the count.
    string mStaleDropBoard;
    bool mLastStaleLivelock; //the last no-answer was the breaker firing
    //#W58-C (D4): every stale drop since the last record was written, one
    //token each: "<arm>/<which half of the slot key moved>/<what the drop
    //did>". The wave-57 corpus took 108 drops (0.82 h of inference, the
    //largest sink after the identical-declined runs) and the ONLY evidence
    //anywhere was a stderr line naming the arm - so a drop's outcome could be
    //recovered only by reading the next `AIPlayerGPT:` line of a 40 KB stderr,
    //and a release build (no DebugTrace) recorded nothing at all. The stderr
    //line is diagnostics; this is the RECORD, and it ships unconditionally on
    //the next translog record for this seat. Consumed when written, so a drop
    //is stamped exactly once.
    std::vector<std::string> mAsyncDropStamps;
    //#W55-E (D5a): reveal-driver stall figures for the record being written.
    int mRevealStallTicks;
    long mRevealStallSecs;
    int mRevealStallPhase;
    bool mRevealStallParked; //#W56-C (D12): the driver's own threshold verdict
    //#W64-AI (F14): the DRIVER-ONLY half of the same wait - polls count as
    //progress here, so this is time the driver was frozen with no model call
    //in flight. Reported beside the structural figures; nothing reads it.
    int mRevealStallDriverTicks;
    long mRevealStallDriverSecs;
    //#W55-E (D23): a WALL MISS - the deadline reached with an empty reply - is
    //an event the seat review counts, and wave 54 had one that produced no
    //record at all (the decision was abandoned when the window auto-passed, so
    //nothing ever wrote it down). mWallMissBase is the prompt that missed;
    //whichever comes first, the record that consumes that prompt stamps
    //wall_miss, or the abandonment writes a zero-choice `wall_miss` record.
    bool mWallMissPending;
    string mWallMissBase;
    long mWallMissLatencyMs; //#W61-U (C13): the round trip that missed the wall
    int mWallMissEvents;
    int mWallMissUnrecorded;
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

//#W63-AF (R8, wave-63 codex review finding 8): the pending-draw scan, exported
//so the test suite can pin it directly. It answers "how many cards will the
//UNRESOLVED stack hand `seat` before it empties" - the quantity every X-draw and
//cleanup row prices its hand against - and the suite's `assertpendingdraws`
//command is the only harness that can put a bare DrawAction in front of it.
class GameObserver;
class Player;
class MTGCardInstance;
int gptStackPendingDrawsFor(GameObserver * observer, Player * seat, MTGCardInstance * exclude);

#endif //WITH_GPT_AI

#endif //_AIPLAYERGPT_H_
