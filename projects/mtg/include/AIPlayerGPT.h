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
 *  The player is a persistent agent, not a stateless picker:
 *  - The system prompt, built once per duel, contains a rules/flow primer,
 *    both decks' card names and rules text (own deck with counts, the
 *    opponent's without, like a player who knows the matchup), and an
 *    optional user-written strategy guide for the AI's deck
 *    (<deckfile>_strategy.txt next to the deck, e.g. ai/baka/deck1_strategy.txt).
 *  - Game events (cards played, damage, life, phases...) accumulate via
 *    receiveEvent() and are fed to the model as the game's narrative, so
 *    it can reason about what the human did, not just the current board.
 *  - The whole game is one chat transcript: every decision sees the
 *    conversation so far (windowed to bound context growth).
 *
 *  Opt-in at launch, never default:
 *      WAGIC_AI=gpt              enable the GPT opponent
 *      WAGIC_GPT_URL=<base>      endpoint base URL (e.g. https://openrouter.ai/api)
 *      WAGIC_GPT_KEY=<key>       bearer token (OpenRouter / any keyed endpoint)
 *      WAGIC_GPT_MODEL=<id>      model id (default: first id from /v1/models)
 *      WAGIC_GPT_THINKING=1      enable thinking on local Qwen-style servers
 *      WAGIC_GPT_HINTS=1         include Baka heuristic scores in the prompt
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

class WEvent;

class AIPlayerGPT : public AIPlayerBaka
{
public:
    AIPlayerGPT(GameObserver *observer, string deckFile, string deckfileSmall, string avatarFile, MTGDeck * deck = NULL);

    //true when the player launched the game with WAGIC_AI=gpt
    static bool isEnabled();

    //Adds the mulligan decision (the engine has no AI mulligan path at all -
    //the heuristic always keeps) before delegating to the base loop.
    virtual int computeActions();

    //feeds the game narrative to the agent transcript
    virtual int receiveEvent(WEvent * event);

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
    //Spell/ability targeting: the model picks the target(s) among the legal
    //set; the clicks reuse the engine's own click helpers so the mechanics
    //stay identical to the heuristic path. checkOnly probes and forced
    //targets are mechanics, not decisions - those go straight to the base.
    virtual int chooseTarget(TargetChooser * tc = NULL, Player * forceTarget = NULL, MTGCardInstance * chosenCard = NULL, bool checkonly = false);

private:
    //Ask the model to choose among options (0-based result, -1 to defer to
    //the heuristic). No model call when there is one option or none - that
    //is the "only one valid action" case. Answers are cached by full prompt
    //(a map, not a single slot: several distinct questions repeat every AI
    //tick - land veto + card choice, one ask per creature in combat - and a
    //one-slot cache would re-fire the HTTP call for each on every tick).
    int askModel(const string& decision, const vector<string>& options);
    std::map<string, int> mAskCache;

    //Can this card plausibly be paid for right now? Cheap pre-filter for the
    //model's casting menu; the authoritative check is the forced
    //AIPlayerBaka::FindCardToPlay validation pass.
    bool roughlyPayable(MTGCardInstance * card, ManaCost * pMana);

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

    //POST the transcript; returns assistant content, empty on any error.
    string requestCompletion();
    //Extract the chosen action number from a model reply; -1 if unusable.
    static int parseChoice(const string& content, int optionCount);

    string mEndpoint; //base URL, empty if nothing answered
    string mModel;
    string mApiKey;
    bool mThinking;
    bool mShowHints;
    //from ai/gpt/endpoints.txt (the per-user copy shadows the shipped one);
    //environment variables override these
    vector<string> mConfigUrls;
    string mConfigModel;
    long mMaxTokens; // -1 = use the built-in/thinking-dependent default

    //chat transcript: (role, content); [0] is the system prompt once built
    vector<std::pair<string, string> > mMessages;
    string mEventLog; //narrative accumulated since the last decision
    //Cards the opponent revealed that are now in their hand: public info a
    //human would remember. Tracked by name (instances are recreated on zone
    //moves), decremented when a card of that name leaves the hand.
    std::map<string, int> mKnownOppHand;
    //Avoid re-querying the model every AI tick while nothing changed.
    string mLastUserMsg;
    int mLastChoice;
};

#endif //WITH_GPT_AI

#endif //_AIPLAYERGPT_H_
