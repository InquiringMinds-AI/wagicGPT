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

#include <utility>
#include <memory>

class WEvent;

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
    //thread while the game loop keeps rendering. Act is the base loop with
    //one insertion - while a call is in flight the AI neither acts NOR
    //passes (an empty clickstream normally commits a pass/decline), and the
    //interrupt-offer timer is kept alive so a slow model cannot time out of
    //its response window. Render draws the "thinking" indicator.
    virtual int Act(float dt);
    virtual void Render();

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
    //fallback for anything unparseable.
    virtual int selectMenuOption();
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
    int askModel(const string& decision, const vector<string>& options, bool narrateChoice = true);
    std::map<string, int> mAskCache;

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
    string consumePlan(const string& content);

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
    std::shared_ptr<AsyncState> mAsyncState;
    float mThinkTime; //seconds the current request has been in flight (for the indicator)
    long mTimeoutMs;  //per-call HTTP timeout (config timeout= / WAGIC_GPT_TIMEOUT)

    //Transient in-duel notice ("no endpoint reachable", "model timed out"),
    //drawn by Render for a few seconds. Frame-based decay: Render runs per
    //frame, Act does not run for both players every tick.
    string mNotice;
    int mNoticeTicks;
    void setNotice(const string& text, float seconds);

    //Turn whose combat already had its bundled blocking declaration - the
    //whole combat is decided in ONE model reply, so once executed nothing
    //more may be asked this turn (leftover no-block creatures would
    //otherwise re-prompt against the changed board).
    int mBlocksDoneTurn;

    //Same for the bundled attacker declaration: the whole attack is decided
    //in ONE reply, so the seam commits once per turn's combat and does not
    //re-prompt the creatures it chose to hold.
    int mAttacksDoneTurn;

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
    void writeTransLog(const char * kind, const string& userMsg, const string& reply, int choice, int optionCount,
                       const string& chosenText = "", const char * fallback = NULL);
    //One "gameend" record per duel: result + final life + turn count. The
    //per-decision records alone cannot say who WON - win-rate and timeout
    //adjudication both need it in the same file.
    bool mGameEndLogged;
    void logGameEnd();

    //True while a request is in flight whose answer has not been consumed.
    bool asyncBusy() const;
    //Poll the async slot for this exact prompt: starts the request when the
    //slot is idle. Returns kChoicePending while in flight, 0 with the reply
    //content once done. A finished answer for a DIFFERENT prompt (stale
    //state drift) is dropped and the new request started.
    int pollCompletion(const string& userMsg, string& content);
    //Serialize the chat request (system prompt + the pending user message)
    //for the worker thread; built on the game thread, nothing shared.
    string buildRequestBody(const string& userMsg);
    //Extract the chosen action number from a model reply; -1 if unusable.
    static int parseChoice(const string& content, int optionCount);

    string mEndpoint; //base URL, empty if nothing answered
    string mModel;
    string mApiKey;
    bool mThinking;
    //from ai/gpt/endpoints.txt (the per-user copy shadows the shipped one);
    //environment variables override these
    vector<string> mConfigUrls;
    string mConfigModel;
    long mMaxTokens; // -1 = use the built-in/thinking-dependent default

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
    //Avoid re-querying the model every AI tick while nothing changed. Keyed
    //by board state + question (not the full prompt) for the same reason as
    //mAskCache: taking an action narrates it, and a full-prompt key would
    //read that as "state changed" and defeat the deadlock breaker.
    string mLastAskKey;
    int mLastChoice;
};

#endif //WITH_GPT_AI

#endif //_AIPLAYERGPT_H_
