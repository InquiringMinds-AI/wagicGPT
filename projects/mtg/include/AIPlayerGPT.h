/*
 *  Wagic, The Homebrew ?! is licensed under the BSD license
 *  See LICENSE in the Folder's root
 *
 *  AIPlayerGPT: an LLM-backed opponent. Inherits all of AIPlayerBaka's
 *  game mechanics (mana payment, clicking, targeting, combat) and overrides
 *  only the decision seam: when the engine has enumerated the legal ranked
 *  actions, the choice among them is delegated to an OpenAI-compatible
 *  chat-completions endpoint instead of the efficiency heuristic.
 *
 *  Opt-in at launch, never default:
 *      WAGIC_AI=gpt            enable the GPT opponent
 *      WAGIC_GPT_URL=<base>    override endpoint base URL (e.g. http://host:8081)
 *      WAGIC_GPT_MODEL=<id>    override model id (default: first id from /v1/models)
 *
 *  Compiled only when WITH_GPT_AI is defined (Linux SDL build); on any
 *  failure (endpoint down, timeout, unparseable reply) every decision
 *  falls back to the AIPlayerBaka heuristic, so the game never stalls.
 */

#ifndef _AIPLAYERGPT_H_
#define _AIPLAYERGPT_H_

#ifdef WITH_GPT_AI

#include "AIPlayerBaka.h"

class AIPlayerGPT : public AIPlayerBaka
{
public:
    AIPlayerGPT(GameObserver *observer, string deckFile, string deckfileSmall, string avatarFile, MTGDeck * deck = NULL);

    //true when the player launched the game with WAGIC_AI=gpt
    static bool isEnabled();

protected:
    virtual const OrderedAIAction * chooseOrderedAction(RankingContainer& ranking);

private:
    //Probe candidate endpoints (env override, then Spark vLLM, then local
    //llama.cpp) and remember the first one that answers /v1/models.
    void resolveEndpoint();

    string serializeGameState();
    string describeAction(const OrderedAIAction& action);

    //Ask the model to pick one of [1..optionCount] or 0 to pass.
    //Returns the chosen number, or -1 on any error (caller falls back).
    int queryModel(const string& prompt, int optionCount);

    string mEndpoint; //base URL, empty if nothing answered
    string mModel;
    //Avoid re-querying the model every AI tick while the board is unchanged.
    string mLastPrompt;
    int mLastChoice;
};

#endif //WITH_GPT_AI

#endif //_AIPLAYERGPT_H_
