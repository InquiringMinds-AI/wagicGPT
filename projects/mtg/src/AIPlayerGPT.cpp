#include "PrecompiledHeader.h"

#ifdef WITH_GPT_AI

#include "AIPlayerGPT.h"
#include "GameObserver.h"
#include "MTGDefinitions.h"
#include "WEvent.h"
#include "Damage.h"
#include "PhaseRing.h"
#include "JFileSystem.h"

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

const char * kRulesPrimer =
    "You are playing a duel of Magic: The Gathering. You win by reducing the opponent's life from 20 to 0.\n"
    "Turn structure: untap, upkeep, draw, first main phase, combat (declare attackers, declare blockers, "
    "damage), second main phase, end. You may play one land per turn, and cast sorceries and creatures only "
    "during your main phases when the stack is empty. Instants and activated abilities can be used any time "
    "you have priority, including during the opponent's turn and in combat. Spells use the stack and resolve "
    "last-in-first-out; players may respond before a spell resolves.\n"
    "Creatures cannot attack the turn they enter play (summoning sickness) unless they have haste. Tapped "
    "creatures cannot block. Combat damage is dealt simultaneously; a creature dies if damage reaching it "
    "this turn is at least its toughness.\n"
    "Mana costs are written like {2}{R} (two generic plus one red). W=white U=blue B=black R=red G=green.\n"
    "General play principles: develop your mana, use your mana efficiently each turn, trade resources "
    "favorably, hold instant-speed interaction for the opponent's threats, and attack when the math favors "
    "you. Think about what the opponent's untapped lands and hand size let them do in response.\n";

size_t curlWriteToString(void * contents, size_t size, size_t nmemb, void * userp)
{
    static_cast<string *>(userp)->append(static_cast<char *>(contents), size * nmemb);
    return size * nmemb;
}

//Returns response body, or empty string on any transport error.
string httpRequest(const string& url, const string& postBody, long timeoutMs, const string& bearer)
{
    CURL * curl = curl_easy_init();
    if (!curl)
        return "";

    string response;
    struct curl_slist * headers = NULL;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 2500L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutMs);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    if (!bearer.empty())
        headers = curl_slist_append(headers, ("Authorization: Bearer " + bearer).c_str());
    if (!postBody.empty())
    {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postBody.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)postBody.size());
    }
    if (headers)
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

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
        if (withStatus)
        {
            if (card->isTapped())
                out << " [tapped]";
            if (card->isAttacker())
                out << " [attacking]";
            else if (card->isDefenser())
                out << " [blocking " << card->isDefenser()->getDisplayName() << "]";
        }
    }
    if (first)
        out << "(none)";
}

bool envFlag(const char * name)
{
    const char * v = getenv(name);
    return v && *v && string(v) != "0" && string(v) != "off";
}

} //namespace

AIPlayerGPT::AIPlayerGPT(GameObserver *observer, string deckFile, string deckfileSmall, string avatarFile, MTGDeck * deck)
    : AIPlayerBaka(observer, deckFile, deckfileSmall, avatarFile, deck), mLastChoice(-1)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    if (const char * key = getenv("WAGIC_GPT_KEY"))
        mApiKey = key;
    mThinking = envFlag("WAGIC_GPT_THINKING");
    mShowHints = envFlag("WAGIC_GPT_HINTS");
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
        string body = httpRequest(candidates[i] + "/v1/models", "", 4000, mApiKey);
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

string AIPlayerGPT::describeDeckCards(Player * p, bool withCounts)
{
    //Aggregate every card the player brought to the duel, wherever it
    //currently is. Dedupe by name; rules text comes from the primitives.
    MTGGameZone * zones[] = { p->game->library, p->game->hand, p->game->inPlay, p->game->graveyard };
    std::map<string, int> counts;
    std::map<string, MTGCardInstance *> samples;
    for (size_t z = 0; z < sizeof(zones) / sizeof(zones[0]); z++)
    {
        for (int i = 0; i < zones[z]->nb_cards; i++)
        {
            MTGCardInstance * card = zones[z]->cards[i];
            counts[card->getDisplayName()]++;
            samples[card->getDisplayName()] = card;
        }
    }

    std::ostringstream out;
    for (std::map<string, int>::iterator it = counts.begin(); it != counts.end(); ++it)
    {
        MTGCardInstance * card = samples[it->first];
        out << "- ";
        if (withCounts)
            out << it->second << "x ";
        out << it->first;
        ManaCost * cost = card->getManaCost();
        if (cost && cost->getConvertedCost())
            out << " {" << cost->toString() << "}";
        if (card->isCreature())
            out << " (" << card->power << "/" << card->toughness << ")";
        string text = card->text;
        if (!text.empty())
        {
            for (size_t i = 0; i < text.size(); i++)
                if (text[i] == '\n')
                    text[i] = ' ';
            out << ": " << text;
        }
        out << "\n";
    }
    return out.str();
}

string AIPlayerGPT::loadStrategyGuide()
{
    //ai/baka/deck3.txt -> ai/baka/deck3_strategy.txt
    string path = deckFile;
    size_t dot = path.rfind(".txt");
    if (dot != string::npos)
        path = path.substr(0, dot) + "_strategy.txt";
    string guide;
    if (JFileSystem::GetInstance()->readIntoString(path, guide) && !guide.empty())
        return guide;
    return "";
}

void AIPlayerGPT::buildSystemPrompt()
{
    std::ostringstream sys;
    sys << kRulesPrimer << "\n";

    sys << "YOUR DECK (cards and rules text):\n" << describeDeckCards(this, true) << "\n";

    Player * opp = this->opponent();
    if (opp)
        sys << "CARDS IN THE OPPONENT'S DECK (you know the matchup, but not how many copies of each):\n"
            << describeDeckCards(opp, false) << "\n";

    string guide = loadStrategyGuide();
    if (!guide.empty())
        sys << "STRATEGY GUIDE FOR YOUR DECK:\n" << guide << "\n";

    sys << "\nDuring the game you will receive the events that happened, the current board state, and a "
           "numbered list of every action that is legal for you right now. Reason about the best play like "
           "a skilled human player: consider tempo, card advantage, combat math, what the opponent may be "
           "holding, and your strategy guide. Then reply with ONLY the number of the chosen action. "
           "Reply 0 to deliberately pass priority and do nothing.";

    mMessages.insert(mMessages.begin(), std::make_pair(string("system"), sys.str()));
}

int AIPlayerGPT::receiveEvent(WEvent * event)
{
    int result = AIPlayerBaka::receiveEvent(event);
    string line = describeEvent(event);
    if (!line.empty())
    {
        mEventLog += "- " + line + "\n";
        //Bound runaway narrative (e.g. long combos); keep the tail.
        if (mEventLog.size() > 6000)
            mEventLog = "(...earlier events trimmed...)\n" + mEventLog.substr(mEventLog.size() - 5000);
    }
    return result;
}

string AIPlayerGPT::describeEvent(WEvent * event)
{
    std::ostringstream out;

    if (WEventZoneChange * e = dynamic_cast<WEventZoneChange *>(event))
    {
        if (!e->card || !e->from || !e->to)
            return "";
        Player * owner = e->to->owner;
        bool mine = (owner == this);
        //Hidden information: never name cards entering the opponent's
        //(the human's) hand or library.
        string toName = e->to->getName();
        if (!mine && (toName == "hand" || toName == "library"))
        {
            out << "Opponent puts a card into their " << toName;
            return out.str();
        }
        out << (mine ? "Your " : "Opponent's ") << e->card->getDisplayName()
            << ": " << e->from->getName() << " -> " << toName;
        return out.str();
    }

    if (WEventDamage * e = dynamic_cast<WEventDamage *>(event))
    {
        if (!e->damage)
            return "";
        out << "Damage: " << e->damage->damage << " dealt";
        if (e->damage->source)
            out << " by " << e->damage->source->getDisplayName();
        if (Player * p = dynamic_cast<Player *>(e->damage->target))
            out << " to " << (p == this ? "you" : "the opponent");
        else if (MTGCardInstance * c = dynamic_cast<MTGCardInstance *>(e->damage->target))
            out << " to " << c->getDisplayName();
        return out.str();
    }

    if (WEventLife * e = dynamic_cast<WEventLife *>(event))
    {
        if (!e->player)
            return "";
        out << (e->player == this ? "Your" : "Opponent's") << " life "
            << (e->amount >= 0 ? "+" : "") << e->amount << " (now " << e->player->life << ")";
        return out.str();
    }

    if (WEventPhaseChange * e = dynamic_cast<WEventPhaseChange *>(event))
    {
        if (!e->to || !e->to->player)
            return "";
        out << "Phase: " << Constants::MTGPhaseNames[e->to->id]
            << " (" << (e->to->player == this ? "your" : "opponent's") << " turn)";
        return out.str();
    }

    if (WEventCounters * e = dynamic_cast<WEventCounters *>(event))
    {
        if (!e->targetCard)
            return "";
        out << (e->added ? "Counter added to " : "Counter removed from ") << e->targetCard->getDisplayName();
        return out.str();
    }

    return ""; //everything else (mana plumbing, taps, micro-steps) is noise
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
        out << "\nOpponent hand size: " << opp->game->hand->nb_cards
            << " | Opponent library: " << opp->game->library->nb_cards << " cards";
    }
    out << "\nYour library: " << game->library->nb_cards << " cards\n";
    return out.str();
}

string AIPlayerGPT::describeAction(const OrderedAIAction& action)
{
    std::ostringstream out;
    if (action.ability)
        out << action.ability->getMenuText();
    if (action.click)
        out << " with " << action.click->getDisplayName();
    if (action.target)
        out << " targeting " << action.target->getDisplayName();
    else if (action.playerAbilityTarget || action.player)
        out << " targeting a player";
    return out.str();
}

string AIPlayerGPT::requestCompletion()
{
    json messages = json::array();
    for (size_t i = 0; i < mMessages.size(); i++)
        messages.push_back({{"role", mMessages[i].first}, {"content", mMessages[i].second}});

    long maxTokens = mThinking ? 2048 : 64;
    if (const char * mt = getenv("WAGIC_GPT_MAXTOKENS"))
        maxTokens = atol(mt);

    json request = {
        {"model", mModel},
        {"messages", messages},
        {"max_tokens", maxTokens},
        {"temperature", 0.5},
    };
    //Thinking toggle is a local-server (Qwen-style) extension; keyed cloud
    //endpoints (OpenRouter etc.) manage reasoning themselves.
    if (mApiKey.empty())
        request["chat_template_kwargs"] = {{"enable_thinking", mThinking}};

    string body = httpRequest(mEndpoint + "/v1/chat/completions", request.dump(), 120000, mApiKey);
    if (body.empty())
        return "";

    try
    {
        json reply = json::parse(body);
        return reply["choices"][0]["message"]["content"].get<string>();
    }
    catch (json::exception&)
    {
        return "";
    }
}

int AIPlayerGPT::parseChoice(const string& content, int optionCount)
{
    //Drop any inline think block, then take the LAST integer in the reply
    //("...so the best play is 3" parses as 3).
    string text = content;
    size_t thinkEnd = text.rfind("</think>");
    if (thinkEnd != string::npos)
        text = text.substr(thinkEnd + 8);

    size_t end = text.find_last_of("0123456789");
    if (end == string::npos)
        return -1;
    size_t start = end;
    while (start > 0 && isdigit(text[start - 1]))
        start--;
    int choice = atoi(text.substr(start, end - start + 1).c_str());
    if (choice < 0 || choice > optionCount)
        return -1;
    return choice;
}

const OrderedAIAction * AIPlayerGPT::chooseOrderedAction(RankingContainer& ranking)
{
    if (!ranking.size() || mEndpoint.empty())
        return AIPlayerBaka::chooseOrderedAction(ranking);

    if (mMessages.empty() || mMessages[0].first != "system")
        buildSystemPrompt();

    std::ostringstream user;
    if (!mEventLog.empty())
        user << "Events since your last decision:\n" << mEventLog << "\n";
    user << serializeGameState();
    user << "\nYour legal actions:\n";
    int index = 0;
    for (RankingContainer::iterator it = ranking.begin(); it != ranking.end(); ++it)
    {
        index++;
        user << index << ". " << describeAction(it->first);
        if (mShowHints)
        {
            OrderedAIAction action = it->first; //copy: getEfficiency() is not const
            user << " (heuristic score " << action.getEfficiency() << ")";
        }
        user << "\n";
    }
    user << "\nWhich action do you take? Reply with ONLY the number (0 = pass).";

    string userMsg = user.str();
    int choice;
    if (userMsg == mLastUserMsg)
    {
        //Nothing changed since the model last answered: reuse the decision
        //instead of paying another round trip this AI tick.
        choice = mLastChoice;
    }
    else
    {
        mMessages.push_back(std::make_pair(string("user"), userMsg));
        string content = requestCompletion();
        choice = parseChoice(content, index);
        if (content.empty())
            mMessages.pop_back(); //transport failure: keep transcript clean
        else
            mMessages.push_back(std::make_pair(string("assistant"), content));
        //Window the transcript: keep the system prompt plus the most recent
        //20 exchanges; old states are superseded by the snapshot we resend.
        while (mMessages.size() > 41)
            mMessages.erase(mMessages.begin() + 1, mMessages.begin() + 3);

        mEventLog.clear();
        mLastUserMsg = userMsg;
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
