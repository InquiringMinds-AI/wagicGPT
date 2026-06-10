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

    //feeds the game narrative to the agent transcript
    virtual int receiveEvent(WEvent * event);

protected:
    virtual const OrderedAIAction * chooseOrderedAction(RankingContainer& ranking);

private:
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
