#include "PrecompiledHeader.h"

#ifdef WITH_GPT_AI

#include "AIPlayerGPT.h"
#include "GameObserver.h"
#include "MTGDefinitions.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <cstdlib>
#include <sstream>

using json = nlohmann::json;

namespace
{

const char * kDefaultEndpoints[] = {
    "http://100.116.136.74:8081", //Spark vLLM
    "http://127.0.0.1:8080",      //local llama.cpp
};

size_t curlWriteToString(void * contents, size_t size, size_t nmemb, void * userp)
{
    static_cast<string *>(userp)->append(static_cast<char *>(contents), size * nmemb);
    return size * nmemb;
}

//Returns response body, or empty string on any transport error.
string httpRequest(const string& url, const string& postBody, long timeoutMs)
{
    CURL * curl = curl_easy_init();
    if (!curl)
        return "";

    string response;
    struct curl_slist * headers = NULL;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 1500L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutMs);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    if (!postBody.empty())
    {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postBody.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)postBody.size());
    }

    CURLcode res = curl_easy_perform(curl);
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    if (headers)
        curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK || httpCode != 200)
        return "";
    return response;
}

void describeZoneCards(std::ostringstream& out, MTGGameZone * zone, bool withStatus)
{
    bool first = true;
    for (int i = 0; i < zone->nb_cards; i++)
    {
        MTGCardInstance * card = zone->cards[i];
        if (!first)
            out << "; ";
        first = false;
        out << card->getDisplayName();
        ManaCost * cost = card->getManaCost();
        if (cost && cost->getConvertedCost())
            out << " {" << cost->toString() << "}";
        if (card->isCreature())
            out << " (" << card->power << "/" << card->toughness << ")";
        if (withStatus && card->isTapped())
            out << " [tapped]";
    }
    if (first)
        out << "(none)";
}

} //namespace

AIPlayerGPT::AIPlayerGPT(GameObserver *observer, string deckFile, string deckfileSmall, string avatarFile, MTGDeck * deck)
    : AIPlayerBaka(observer, deckFile, deckfileSmall, avatarFile, deck), mLastChoice(-1)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    resolveEndpoint();
    if (mEndpoint.empty())
        fprintf(stderr, "AIPlayerGPT: no LLM endpoint reachable, falling back to Baka heuristics for every decision\n");
    else
        fprintf(stderr, "AIPlayerGPT: using %s (model %s)\n", mEndpoint.c_str(), mModel.c_str());
}

bool AIPlayerGPT::isEnabled()
{
    const char * mode = getenv("WAGIC_AI");
    return mode && string(mode) == "gpt";
}

void AIPlayerGPT::resolveEndpoint()
{
    vector<string> candidates;
    if (const char * url = getenv("WAGIC_GPT_URL"))
        candidates.push_back(url);
    else
        for (size_t i = 0; i < sizeof(kDefaultEndpoints) / sizeof(kDefaultEndpoints[0]); i++)
            candidates.push_back(kDefaultEndpoints[i]);

    for (size_t i = 0; i < candidates.size(); i++)
    {
        string body = httpRequest(candidates[i] + "/v1/models", "", 2000);
        if (body.empty())
            continue;
        try
        {
            json models = json::parse(body);
            mEndpoint = candidates[i];
            if (const char * model = getenv("WAGIC_GPT_MODEL"))
                mModel = model;
            else if (models.contains("data") && !models["data"].empty())
                mModel = models["data"][0]["id"].get<string>();
            return;
        }
        catch (json::exception&)
        {
            continue;
        }
    }
}

string AIPlayerGPT::serializeGameState()
{
    std::ostringstream out;
    Player * opp = this->opponent();

    out << "Phase: " << observer->getCurrentGamePhaseName();
    out << " | It is " << (observer->currentPlayer == this ? "your" : "the opponent's") << " turn.\n";
    out << "Your life: " << this->life << " | Opponent life: " << (opp ? opp->life : 0) << "\n";
    out << "Mana in your pool: " << this->getManaPool()->toString() << "\n";

    out << "Your hand: ";
    describeZoneCards(out, game->hand, false);
    out << "\nYour battlefield: ";
    describeZoneCards(out, game->inPlay, true);
    if (opp)
    {
        out << "\nOpponent battlefield: ";
        describeZoneCards(out, opp->game->inPlay, true);
        out << "\nOpponent hand size: " << opp->game->hand->nb_cards;
    }
    out << "\n";
    return out.str();
}

string AIPlayerGPT::describeAction(const OrderedAIAction& action)
{
    std::ostringstream out;
    if (action.ability)
    {
        string menuText = action.ability->getMenuText();
        out << menuText;
    }
    if (action.click)
        out << " with " << action.click->getDisplayName();
    if (action.target)
        out << " targeting " << action.target->getDisplayName();
    else if (action.playerAbilityTarget || action.player)
        out << " targeting a player";
    return out.str();
}

int AIPlayerGPT::queryModel(const string& prompt, int optionCount)
{
    json request = {
        {"model", mModel},
        {"messages", {
            {{"role", "system"}, {"content",
                "You are an expert Magic: The Gathering player making in-game decisions. "
                "Reply with ONLY a single integer and nothing else."}},
            {{"role", "user"}, {"content", prompt}},
        }},
        {"max_tokens", 16},
        {"temperature", 0.3},
        {"chat_template_kwargs", {{"enable_thinking", false}}},
    };

    string body = httpRequest(mEndpoint + "/v1/chat/completions", request.dump(), 30000);
    if (body.empty())
        return -1;

    try
    {
        json reply = json::parse(body);
        string content = reply["choices"][0]["message"]["content"].get<string>();
        //tolerate stray whitespace/markup around the integer
        size_t pos = content.find_first_of("0123456789");
        if (pos == string::npos)
            return -1;
        int choice = atoi(content.c_str() + pos);
        if (choice < 0 || choice > optionCount)
            return -1;
        return choice;
    }
    catch (json::exception&)
    {
        return -1;
    }
}

const OrderedAIAction * AIPlayerGPT::chooseOrderedAction(RankingContainer& ranking)
{
    if (!ranking.size() || mEndpoint.empty())
        return AIPlayerBaka::chooseOrderedAction(ranking);

    std::ostringstream prompt;
    prompt << serializeGameState();
    prompt << "\nLegal actions (heuristic score in parens, higher = better by the old engine):\n";
    int index = 0;
    for (RankingContainer::iterator it = ranking.begin(); it != ranking.end(); ++it)
    {
        index++;
        OrderedAIAction action = it->first; //copy: getEfficiency() is not const
        prompt << index << ". " << describeAction(it->first) << " (" << action.getEfficiency() << ")\n";
    }
    prompt << "\nWhich action do you take? Answer 0 to pass priority and do nothing.";

    string promptStr = prompt.str();
    int choice;
    if (promptStr == mLastPrompt)
    {
        //Board unchanged since the model last answered: reuse the decision
        //instead of paying another network round trip this AI tick.
        choice = mLastChoice;
    }
    else
    {
        choice = queryModel(promptStr, index);
        mLastPrompt = promptStr;
        mLastChoice = choice;
        DebugTrace("AIPlayerGPT: model chose " << choice << " of " << index);
    }

    if (choice < 0) //transport/parse failure: defer to the heuristic
        return AIPlayerBaka::chooseOrderedAction(ranking);
    if (choice == 0) //deliberate pass
        return NULL;

    RankingContainer::iterator it = ranking.begin();
    std::advance(it, choice - 1);
    return &(it->first);
}

#endif //WITH_GPT_AI
