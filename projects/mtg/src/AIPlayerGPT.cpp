#include "PrecompiledHeader.h"

#ifdef WITH_GPT_AI

#include "AIPlayerGPT.h"
#include "GameObserver.h"
#include "MTGDefinitions.h"
#include "WEvent.h"
#include "Damage.h"
#include "PhaseRing.h"
#include "JFileSystem.h"
#include "MTGAbility.h"
#include "CardDescriptor.h"
#include "ManaCost.h"
#include "ExtraCost.h"
#include "Counters.h"
#include "ActionLayer.h"
#include "AllAbilities.h"
#include "ActionStack.h"
#include "WFont.h"
#include "WResourceManager.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <set>
#include <algorithm>
#include <thread>
#include <mutex>

using json = nlohmann::json;

namespace
{

//Runtime configuration, user-editable: Res/ai/gpt/endpoints.txt, shadowed
//by the per-user copy (~/.Wagic/ai/gpt/endpoints.txt) so private endpoints
//and keys never have to live in a tracked file. Environment variables
//override file values. No endpoints are compiled in beyond localhost.
struct GptConfig
{
    vector<string> urls;
    string model;
    string key;
    int thinking;  // -1 unset
    int hints;     // -1 unset
    long maxTokens; // -1 unset
    GptConfig() : thinking(-1), hints(-1), maxTokens(-1) {}
};

//Read a GPT asset, private copy first. The SDL build collapses
//JFileSystem's user and system roots onto Res/ (no ~/.Wagic shadow like
//the Qt build), so a config kept OUT of the tracked Res tree has to be
//read directly: try $HOME/.Wagic/ai/gpt/<filename> first (the documented
//private location for endpoints/keys), then fall back to the bundled Res
//copy through JFileSystem. Returns "" when neither exists.
string readGptAsset(const char * filename)
{
    if (const char * home = getenv("HOME"))
    {
        string path = string(home) + "/.Wagic/ai/gpt/" + filename;
        std::ifstream f(path.c_str(), std::ios::binary);
        if (f)
        {
            std::ostringstream ss;
            ss << f.rdbuf();
            string c = ss.str();
            if (!c.empty())
                return c;
        }
    }
    string content;
    JFileSystem::GetInstance()->readIntoString(string("ai/gpt/") + filename, content);
    return content;
}

GptConfig loadGptConfig()
{
    GptConfig cfg;
    string content = readGptAsset("endpoints.txt");
    if (content.empty())
        return cfg;
    std::istringstream stream(content);
    string line;
    while (std::getline(stream, line))
    {
        size_t hash = line.find('#');
        if (hash != string::npos)
            line = line.substr(0, hash);
        size_t eq = line.find('=');
        if (eq == string::npos)
            continue;
        string k = line.substr(0, eq);
        string v = line.substr(eq + 1);
        k.erase(0, k.find_first_not_of(" \t")); k.erase(k.find_last_not_of(" \t\r") + 1);
        v.erase(0, v.find_first_not_of(" \t")); v.erase(v.find_last_not_of(" \t\r") + 1);
        if (v.empty())
            continue;
        if (k == "url") cfg.urls.push_back(v);
        else if (k == "model") cfg.model = v;
        else if (k == "key") cfg.key = v;
        else if (k == "thinking") cfg.thinking = (v != "0" && v != "off") ? 1 : 0;
        else if (k == "hints") cfg.hints = (v != "0" && v != "off") ? 1 : 0;
        else if (k == "maxtokens") cfg.maxTokens = atol(v.c_str());
    }
    return cfg;
}

void replaceAllOccurrences(string& text, const string& token, const string& value)
{
    size_t pos = 0;
    while ((pos = text.find(token, pos)) != string::npos)
    {
        text.replace(pos, token.size(), value);
        pos += value.size();
    }
}

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

//General strategy priors, mirrored from Res/ai/gpt/system_prompt.txt so the
//compiled fallback (used only when that asset is missing) carries the same
//guidance. Priors, not laws - they bias the model's reasoning without
//scripting the choice.
const char * kStrategyPriors =
    "STRATEGY (priors to reason from, not absolute rules):\n"
    "- The only goal is to win before the opponent does. Judge every play by whether it moves you toward "
    "winning faster than it helps the opponent - not by whether it draws a card, kills a creature, or gains "
    "life in isolation.\n"
    "- Decide who is the beatdown. From the two decklists and the board, work out who wins if nothing "
    "changes: compare each side's clock - how many turns of attacks to deal the opponent's remaining life. "
    "The side that wins that race has the advantage and should play to control: trade, remove threats, "
    "preserve life, don't overcommit. The side that would LOSE the race must be the aggressor: deploy "
    "threats, attack, and force the opponent to react before the game reaches the state they lose. This "
    "role can flip mid-game as the board changes.\n"
    "- Two resources decide most games: cards and mana (tempo). Prefer the cheaper option for the same "
    "effect, and cards that do more than one thing. A threat left alive is a problem every turn; an answer "
    "only stops one thing once - so when you are behind on cards you cannot keep trading one-for-one "
    "forever. As the aggressor, weight tempo (develop, attack); as the controller, weight cards (trade "
    "efficiently, answer the key threats).\n"
    "- Act at the latest useful moment. Hold instants, removal, and tricks until you must use them or until "
    "the opponent's turn or end step - waiting costs nothing and gives you information. Play a land before "
    "your spells for mana flexibility. When you expect the opponent to hold an answer, prefer committing a "
    "lesser threat before your best card. Pass priority (choose 0) whenever there is nothing worth doing "
    "right now.\n"
    "- Control the opponent through the information you show them. They see your untapped mana, your "
    "attacks, and your board; your hand stays hidden unless a card reveals it. Leaving mana open represents "
    "an instant (counter, trick, removal) and can deter an attack or push the opponent into a worse line "
    "even when you hold nothing. Make some plays for their effect on what the opponent believes, not only "
    "their direct board effect - and back the representation with the real card when you have it, so the "
    "bluff stays credible.\n"
    "- Combat is arithmetic. Before attacking, check whether enough damage gets through to matter, and "
    "whether the opponent's untapped mana threatens a trick or removal that turns the attack into a blowout "
    "- assume the relevant trick unless the board says otherwise. Attacking taps your creatures: do not tap "
    "out into a lethal swing back. When blocking, prefer trades that kill a creature worth more than the one "
    "you lose, or that cost you nothing; take the damage when your blockers are worth more alive than the "
    "life saved; chump-block only to buy a turn you genuinely need. Only race if you win the race.\n"
    "- Don't overcommit into a likely sweeper when you are ahead on board - keep reinforcements in hand and "
    "make the opponent find it. But don't over-respect answers either: if you only win in the world where "
    "the opponent lacks the answer, play as though they do.\n"
    "- Common errors to avoid: playing the wrong role; racing a faster opponent; tapping out when you "
    "needed interaction held up; firing removal or tricks too early or at the wrong target; overextending "
    "into a sweeper; leading with your best threat into open mana; sloppy combat math (missing lethal, "
    "making bad trades, forgetting the opponent's trick).\n";

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

//The auras/equipment attached to a permanent. There is no forward list, so
//find them the way the engine does (cf. MTGCardInstance::hasTotemArmor):
//every attachment carries a reverse pointer (auraParent) to its host.
void describeAttachments(std::ostringstream& out, MTGCardInstance * host)
{
    GameObserver * obs = host->getObserver();
    if (!obs)
        return;
    bool first = true;
    for (int p = 0; p < 2; p++)
    {
        MTGGameZone * bf = obs->players[p]->game->battlefield;
        for (int x = 0; x < bf->nb_cards; x++)
        {
            MTGCardInstance * att = bf->cards[x];
            if (att->auraParent != host)
                continue;
            out << (first ? " {attached: " : ", ") << att->getDisplayName();
            first = false;
        }
    }
    if (!first)
        out << "}";
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
        {
            out << " (" << card->power << "/" << card->toughness << ")";
            //Surface the live delta the static decklist text cannot carry: a
            //creature pumped, counter'd, enchanted or equipped is no longer
            //its printed stats. Only meaningful in play (withStatus).
            if (withStatus && (card->power != card->basepower || card->toughness != card->basetoughness))
                out << " (printed " << card->basepower << "/" << card->basetoughness << ")";
        }
        if (withStatus)
        {
            if (card->counters && card->counters->mCount)
            {
                out << " [counters:";
                for (size_t c = 0; c < card->counters->counters.size(); c++)
                {
                    Counter * ct = card->counters->counters[c];
                    if (!ct || ct->nb <= 0)
                        continue;
                    out << " " << ct->nb << "x ";
                    if (!ct->name.empty() && ct->name != " ")
                        out << ct->name;
                    else
                        out << (ct->power >= 0 ? "+" : "") << ct->power << "/"
                            << (ct->toughness >= 0 ? "+" : "") << ct->toughness;
                }
                out << "]";
            }
            describeAttachments(out, card);
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

//The reveal zone is a plain MTGGameZone, so getName() returns "zone";
//detect it by identity instead.
bool isRevealZone(MTGGameZone * z)
{
    return z && z->owner && z == z->owner->game->reveal;
}

string zoneDesc(MTGGameZone * z)
{
    if (isRevealZone(z))
        return "reveal";
    return z->getName();
}

//One targetable thing, described for the model's target menu.
string describeTarget(Player * me, Targetable * t)
{
    std::ostringstream o;
    if (Player * p = dynamic_cast<Player *>(t))
    {
        o << (p == me ? "Yourself" : "The opponent") << " (player, life " << p->life << ")";
        return o.str();
    }
    MTGCardInstance * c = dynamic_cast<MTGCardInstance *>(t);
    if (!c)
        return "(unknown)";
    o << c->getDisplayName();
    if (c->isCreature())
        o << " (" << c->power << "/" << c->toughness << ")";
    if (c->currentZone)
        o << " [" << (c->controller() == me ? "your " : "opponent's ") << zoneDesc(c->currentZone) << "]";
    if (c->isTapped())
        o << " [tapped]";
    return o.str();
}

} //namespace

//State shared between the game thread and the HTTP worker. The worker owns
//a shared_ptr copy: if the game (and this player) is destroyed while a
//request is still running, the worker finishes writing into memory that
//only it still references, then frees it.
struct AIPlayerGPT::AsyncState
{
    std::mutex mtx;
    int status;      //0 idle, 1 in flight, 2 done (answer not yet consumed)
    string prompt;   //the userMsg the in-flight/done request was built for
    string response; //raw HTTP body once status == 2
    AsyncState() : status(0) {}
};

bool AIPlayerGPT::asyncBusy() const
{
    std::lock_guard<std::mutex> g(mAsyncState->mtx);
    return mAsyncState->status == 1;
}

int AIPlayerGPT::pollCompletion(const string& userMsg, string& content)
{
    {
        std::lock_guard<std::mutex> g(mAsyncState->mtx);
        if (mAsyncState->status == 1)
            return kChoicePending; //one request at a time; whatever asked, wait
        if (mAsyncState->status == 2)
        {
            if (mAsyncState->prompt == userMsg)
            {
                string body = mAsyncState->response;
                mAsyncState->status = 0;
                mAsyncState->response.clear();
                content.clear();
                if (!body.empty())
                {
                    try
                    {
                        json reply = json::parse(body);
                        content = reply["choices"][0]["message"]["content"].get<string>();
                    }
                    catch (json::exception&)
                    {
                        content.clear();
                    }
                }
                return 0;
            }
            //An answer for a prompt the game state has moved past (should
            //not happen while the AI neither acts nor passes; drop safely).
            DebugTrace("AIPlayerGPT: dropping stale async answer");
            mAsyncState->status = 0;
            mAsyncState->response.clear();
        }
    }

    //Idle: build the request on the game thread (mMessages is not shared
    //with the worker) and launch the round trip in the background.
    string requestBody = buildRequestBody(userMsg);
    string url = mEndpoint + "/v1/chat/completions";
    string key = mApiKey;
    std::shared_ptr<AsyncState> state = mAsyncState;
    {
        std::lock_guard<std::mutex> g(state->mtx);
        state->status = 1;
        state->prompt = userMsg;
        state->response.clear();
    }
    std::thread([state, url, requestBody, key]() {
        string body = httpRequest(url, requestBody, 120000, key);
        std::lock_guard<std::mutex> g(state->mtx);
        state->status = 2;
        state->response = body;
    }).detach();
    return kChoicePending;
}

AIPlayerGPT::AIPlayerGPT(GameObserver *observer, string deckFile, string deckfileSmall, string avatarFile, MTGDeck * deck)
    : AIPlayerBaka(observer, deckFile, deckfileSmall, avatarFile, deck), mAsyncState(std::make_shared<AsyncState>()), mThinkTime(0), mLastChoice(-1)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    //File config first, environment variables override.
    GptConfig cfg = loadGptConfig();
    mConfigUrls = cfg.urls;
    mConfigModel = cfg.model;
    mMaxTokens = cfg.maxTokens;
    mApiKey = cfg.key;
    if (const char * key = getenv("WAGIC_GPT_KEY"))
        mApiKey = key;
    mThinking = getenv("WAGIC_GPT_THINKING") ? envFlag("WAGIC_GPT_THINKING") : (cfg.thinking == 1);
    mShowHints = getenv("WAGIC_GPT_HINTS") ? envFlag("WAGIC_GPT_HINTS") : (cfg.hints == 1);
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
    else if (!mConfigUrls.empty())
        candidates = mConfigUrls;
    else
        candidates.push_back("http://127.0.0.1:8080"); //local llama.cpp

    for (size_t i = 0; i < candidates.size(); i++)
    {
        string body = httpRequest(candidates[i] + "/v1/models", "", 4000, mApiKey);
        if (body.empty())
            continue;
        try
        {
            json models = json::parse(body);
            //A real /v1/models reply carries a non-empty "data" array.
            //An auth error or unrelated JSON ({"error":"Unauthorized"})
            //parses fine but is NOT a usable endpoint - reject it so a
            //bad/missing key falls through to the next candidate (or to
            //the honest "no endpoint reachable" message) instead of being
            //accepted with an empty model and silently failing every call.
            if (!models.contains("data") || !models["data"].is_array() || models["data"].empty())
                continue;
            mEndpoint = candidates[i];
            if (const char * model = getenv("WAGIC_GPT_MODEL"))
                mModel = model;
            else if (!mConfigModel.empty())
                mModel = mConfigModel;
            else
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
    Player * opp = this->opponent();
    string myDeck = describeDeckCards(this, true);
    string oppDeck = opp ? describeDeckCards(opp, false) : string("(unknown)");
    string guide = loadStrategyGuide();
    string guideBlock = guide.empty() ? string("") : ("STRATEGY GUIDE FOR YOUR DECK:\n" + guide);

    //The prompt is a user-editable runtime file (Res/ai/gpt/system_prompt.txt,
    //or a private $HOME/.Wagic/ai/gpt/system_prompt.txt that takes
    //precedence) so players can tune it without rebuilding; see
    //Res/ai/gpt/README.txt for the placeholders.
    string tmpl = readGptAsset("system_prompt.txt");
    if (!tmpl.empty())
    {
        replaceAllOccurrences(tmpl, "{MY_DECK}", myDeck);
        replaceAllOccurrences(tmpl, "{OPPONENT_DECK}", oppDeck);
        replaceAllOccurrences(tmpl, "{STRATEGY_GUIDE}", guideBlock);
        mMessages.insert(mMessages.begin(), std::make_pair(string("system"), tmpl));
        return;
    }

    //Fallback when the prompt asset is missing: the same text, compiled in.
    std::ostringstream sys;
    sys << kRulesPrimer << "\n";
    sys << "YOUR DECK (cards and rules text):\n" << myDeck << "\n";
    if (opp)
        sys << "CARDS IN THE OPPONENT'S DECK (you know the matchup, but not how many copies of each):\n"
            << oppDeck << "\n";
    if (!guideBlock.empty())
        sys << guideBlock << "\n";
    sys << "\n" << kStrategyPriors;
    sys << "\nDuring the game you will receive the events that happened, the current board state (each card's "
           "current power/toughness, counters, and anything attached to it), and a numbered list of every "
           "action that is legal for you right now.\n"
           "Before you choose, weigh each action by what it COSTS you against what it gains. A cost is anything "
           "you give up to take the action: mana, tapping a permanent, SACRIFICING one of your own permanents, "
           "paying life, or discarding. Many activated abilities cost more than mana - the action line states "
           "its cost in brackets, and a creature or other valuable permanent is rarely worth trading for "
           "something lesser. Then pick the play whose gain most clearly exceeds its cost on the current board: "
           "develop your position, hold interaction when nothing is urgent, and attack when the math favors you.\n"
           "Reason like a skilled human player, then reply with ONLY the number of the chosen action. "
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
        string toName = e->to->getName();
        string cardName = e->card->getDisplayName();

        //A card of a known name leaving the opponent's hand consumes that
        //knowledge (it is now public anyway, or gone).
        if (e->from->owner && e->from->owner != this && e->from == e->from->owner->game->hand)
        {
            std::map<string, int>::iterator known = mKnownOppHand.find(cardName);
            if (known != mKnownOppHand.end() && --(known->second) <= 0)
                mKnownOppHand.erase(known);
        }

        //Reveals are public information, whoever's card it is.
        if (isRevealZone(e->to))
        {
            out << (mine ? "Your " : "Opponent's ") << cardName
                << " is revealed (from " << zoneDesc(e->from) << ")";
            return out.str();
        }
        if (isRevealZone(e->from))
        {
            if (!mine && (toName == "hand" || toName == "library"))
            {
                //Revealed cards stay known even in hidden zones: remember
                //the ones a human player would remember.
                if (toName == "hand")
                    mKnownOppHand[cardName]++;
                out << "Opponent puts the revealed " << cardName << " into their " << toName;
                return out.str();
            }
            out << (mine ? "Your revealed " : "Opponent's revealed ") << cardName
                << " goes to " << toName;
            return out.str();
        }

        //Hidden information: never name cards entering the opponent's
        //(the human's) hand or library unrevealed.
        if (!mine && (toName == "hand" || toName == "library"))
        {
            out << "Opponent puts a card into their " << toName;
            return out.str();
        }
        out << (mine ? "Your " : "Opponent's ") << cardName
            << ": " << zoneDesc(e->from) << " -> " << toName;
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
        if (!mKnownOppHand.empty())
        {
            out << "\nCards you have seen in the opponent's hand: ";
            bool first = true;
            for (std::map<string, int>::iterator it = mKnownOppHand.begin(); it != mKnownOppHand.end(); ++it)
            {
                if (!first)
                    out << "; ";
                first = false;
                if (it->second > 1)
                    out << it->second << "x ";
                out << it->first;
            }
        }
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

    //Localize the cost onto the action itself. The menu text describes the
    //EFFECT ("Destroy target enchantment") but never the price, so a sacrifice
    //ability reads as free unless the cost is stated here: mana from the cost's
    //string, plus every extra cost's human render string ("Sacrifice <card>",
    //"Pay 2 life"). This is the connective tissue that was missing - the model
    //had the rules text in its briefing but not the cost beside the choice.
    if (action.ability && action.ability->getCost())
    {
        ManaCost * c = action.ability->getCost();
        std::ostringstream cost;
        bool any = false;
        if (c->getConvertedCost())
        {
            cost << c->toString();
            any = true;
        }
        if (c->extraCosts)
        {
            for (size_t i = 0; i < c->extraCosts->costs.size(); i++)
            {
                ExtraCost * ec = c->extraCosts->costs[i];
                if (!ec || ec->mCostRenderString.empty())
                    continue;
                if (any)
                    cost << ", ";
                cost << ec->mCostRenderString;
                any = true;
            }
        }
        if (any)
            out << " [cost: " << cost.str() << "]";
    }
    return out.str();
}

string AIPlayerGPT::buildRequestBody(const string& userMsg)
{
    json messages = json::array();
    for (size_t i = 0; i < mMessages.size(); i++)
        messages.push_back({{"role", mMessages[i].first}, {"content", mMessages[i].second}});
    //The pending user message rides in the request only; it joins the real
    //transcript together with the assistant reply at consumption time.
    messages.push_back({{"role", "user"}, {"content", userMsg}});

    //A bare number needs almost nothing, but the model sometimes prefaces
    //it with a short justification; give enough room that the number is not
    //truncated away (which parsed as "no choice" and held creatures back).
    long maxTokens = mThinking ? 2048 : 200;
    if (mMaxTokens > 0)
        maxTokens = mMaxTokens;
    if (const char * mt = getenv("WAGIC_GPT_MAXTOKENS"))
        maxTokens = atol(mt);

    json request = {
        {"model", mModel},
        {"messages", messages},
        {"max_tokens", maxTokens},
        {"temperature", 0.5},
    };
    //Qwen-style thinking toggle. Unknown-field-tolerant providers
    //(OpenRouter etc.) ignore this; local vLLM/llama.cpp honor it, keyed
    //or not. Matters: qwen3.6 thinks by default (~6x decision latency).
    request["chat_template_kwargs"] = {{"enable_thinking", mThinking}};

    return request.dump();
}

int AIPlayerGPT::parseChoice(const string& content, int optionCount)
{
    //Drop any inline think block first.
    string text = content;
    size_t thinkEnd = text.rfind("</think>");
    if (thinkEnd != string::npos)
        text = text.substr(thinkEnd + 8);

    //Scan every integer and keep the LAST one that is a VALID option
    //[0, optionCount]. Taking the last digit blindly (the old behavior)
    //failed whenever the model echoed a power/toughness like "(2/4)" or
    //the option text, because the trailing "4" was out of range and the
    //whole decision parsed as -1 - which silently held creatures back.
    //Ignoring out-of-range numbers reads the real choice out of a noisy
    //reply instead.
    int choice = -1;
    size_t i = 0;
    while (i < text.size())
    {
        if (isdigit((unsigned char) text[i]))
        {
            size_t j = i;
            while (j < text.size() && isdigit((unsigned char) text[j]))
                j++;
            int n = atoi(text.substr(i, j - i).c_str());
            if (n >= 0 && n <= optionCount)
                choice = n;
            i = j;
        }
        else
        {
            i++;
        }
    }
    return choice;
}

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
    for (RankingContainer::iterator it = ranking.begin(); it != ranking.end(); ++it)
    {
        MTGAbility * ab = it->first.ability;
        int t = ab ? ab->aType : (int) MTGAbility::UNKNOWN;
        if (t == MTGAbility::MTG_ATTACK_RULE || t == MTGAbility::MTG_BLOCK_RULE
            || t == MTGAbility::ATTACK_COST || t == MTGAbility::BLOCK_COST)
            continue;
        candidates.push_back(&(it->first));
    }
    //Nothing but declaration mechanics: the heuristic ranking drives those.
    if (candidates.empty())
        return AIPlayerBaka::chooseOrderedAction(ranking);

    int phase = observer->getCurrentGamePhase();

    if (mMessages.empty() || mMessages[0].first != "system")
        buildSystemPrompt();

    std::ostringstream user;
    if (!mEventLog.empty())
        user << "Events since your last decision:\n" << mEventLog << "\n";
    user << serializeGameState();
    user << "\nYour legal actions:\n";
    int index = 0;
    for (size_t c = 0; c < candidates.size(); c++)
    {
        index++;
        user << index << ". " << describeAction(*candidates[c]);
        if (mShowHints)
        {
            OrderedAIAction action = *candidates[c]; //copy: getEfficiency() is not const
            user << " (heuristic score " << action.getEfficiency() << ")";
        }
        user << "\n";
    }
    user << "\nWhich action do you take? Reply with ONLY the number (0 = pass).";

    string userMsg = user.str();
    bool unchanged = (userMsg == mLastUserMsg);

    //Deadlock breaker: priority is decided every AI tick. If the game state
    //is unchanged since our last decision AND that decision was to TAKE an
    //action, the action did not progress the game (it was a no-op, or the
    //engine could not complete it). Repeating it loops forever while the AI
    //never passes priority - so the queued phase change never resolves and
    //the game freezes (silently, since the cached path made no model call).
    //The heuristic avoids this by passing when nothing is worth doing; the
    //model can name a dead action. So on an unchanged state we pass once,
    //yielding priority; a real state change then earns a fresh decision.
    if (unchanged && mLastChoice > 0)
    {
        DebugTrace("AIPlayerGPT: state unchanged after action " << mLastChoice
                   << "; passing priority to avoid a loop");
        mLastChoice = 0;
        return NULL;
    }

    int choice;
    if (unchanged)
    {
        //Nothing changed since the model last answered: reuse the decision
        //instead of paying another round trip this AI tick.
        choice = mLastChoice;
    }
    else
    {
        string content;
        if (pollCompletion(userMsg, content) == kChoicePending)
        {
            //Round trip in flight: no action this tick. The Act override
            //keeps the empty clickstream from being committed as a pass.
            return NULL;
        }
        choice = parseChoice(content, index);
        if (!content.empty())
        {
            mMessages.push_back(std::make_pair(string("user"), userMsg));
            mMessages.push_back(std::make_pair(string("assistant"), content));
        }
        //Window the transcript: keep the system prompt plus the most recent
        //20 exchanges; old states are superseded by the snapshot we resend.
        while (mMessages.size() > 41)
            mMessages.erase(mMessages.begin() + 1, mMessages.begin() + 3);

        mEventLog.clear();
        mLastUserMsg = userMsg;
        mLastChoice = choice;
        DebugTrace("AIPlayerGPT: model chose " << choice << " of " << index);
    }

    //Log every outcome (the cached paths were previously silent, which hid
    //a freeze): phase, whether the state was cached, and what we return.
    if (choice < 0) //transport/parse failure: defer to the heuristic
    {
        DebugTrace("AIPlayerGPT[ph" << phase << "]: defer to heuristic (cached=" << unchanged << ")");
        return AIPlayerBaka::chooseOrderedAction(ranking);
    }
    if (choice == 0) //deliberate pass
    {
        DebugTrace("AIPlayerGPT[ph" << phase << "]: pass priority (cached=" << unchanged << ")");
        return NULL;
    }

    DebugTrace("AIPlayerGPT[ph" << phase << "]: take action " << choice << "/" << index << " (cached=" << unchanged << ")");
    return candidates[choice - 1];
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

int AIPlayerGPT::askModel(const string& decision, const vector<string>& options)
{
    //"Only one valid action": no decision to make, no model call.
    if (options.empty())
        return -1;
    if (options.size() == 1)
        return 0;
    if (mEndpoint.empty())
        return -1; //no endpoint: caller falls back to the heuristic

    if (mMessages.empty() || mMessages[0].first != "system")
        buildSystemPrompt();

    std::ostringstream user;
    if (!mEventLog.empty())
        user << "Events since your last decision:\n" << mEventLog << "\n";
    user << serializeGameState();
    user << "\n" << decision << "\n";
    for (size_t i = 0; i < options.size(); i++)
        user << (i + 1) << ". " << options[i] << "\n";
    user << "\nReply with ONLY the number of your choice.";
    string userMsg = user.str();

    //Prompt-keyed answer cache: the same questions are re-polled every AI
    //tick until the game state moves on, and several distinct questions can
    //alternate within one tick. Identical prompt => identical answer, with
    //no repeated HTTP round trip. Does not touch the priority cache
    //(mLastUserMsg/mLastChoice) used by chooseOrderedAction.
    std::map<string, int>::iterator cached = mAskCache.find(userMsg);
    if (cached != mAskCache.end())
        return (cached->second >= 1 && cached->second <= (int) options.size()) ? cached->second - 1 : -1;

    string content;
    if (pollCompletion(userMsg, content) == kChoicePending)
        return kChoicePending; //callers unwind this tick and re-poll

    int choice = parseChoice(content, (int) options.size());
    if (!content.empty())
    {
        //user and assistant join the transcript together at consumption, so
        //a failed round trip can never leave a half exchange behind
        mMessages.push_back(std::make_pair(string("user"), userMsg));
        mMessages.push_back(std::make_pair(string("assistant"), content));
    }
    while (mMessages.size() > 41)
        mMessages.erase(mMessages.begin() + 1, mMessages.begin() + 3);

    mEventLog.clear();
    mAskCache[userMsg] = choice;
    DebugTrace("AIPlayerGPT: " << decision << " -> chose " << choice << " of " << options.size());

    return (choice >= 1) ? choice - 1 : -1; //0 or parse-fail: defer to caller
}

bool AIPlayerGPT::roughlyPayable(MTGCardInstance * card, ManaCost * pMana)
{
    ManaCost * cost = card->getManaCost();
    if (!cost)
        return true;
    if (!cost->getConvertedCost() && !cost->extraCosts)
        return true;
    if (pMana->canAfford(cost, card->has(Constants::ANYTYPEOFMANA)))
        return true;
    if (cost->getAlternative() && pMana->canAfford(cost->getAlternative(), 0))
        return true;
    if (cost->getMorph() && pMana->canAfford(cost->getMorph(), 0))
        return true;
    if (cost->getFlashback() && pMana->canAfford(cost->getFlashback(), 0))
        return true;
    if (cost->getRetrace() && pMana->canAfford(cost->getRetrace(), 0))
        return true;
    //specific-producer payments (e.g. sac-for-mana rocks) that potential
    //mana does not cover
    if (canPayMana(card, cost, card->has(Constants::ANYTYPEOFMANA)).size())
        return true;
    return false;
}

MTGCardInstance * AIPlayerGPT::FindCardToPlay(ManaCost * pMana, const char * type)
{
    //No endpoint, or a scripted combo is mid-execution: heuristic as-is.
    if (mEndpoint.empty() || comboCards.size())
        return AIPlayerBaka::FindCardToPlay(pMana, type);

    //Lands: the heuristic proposes (dropping a land is nearly always right),
    //the model keeps its veto (e.g. a land into four Ankh of Mishra).
    if (!strcmp(type, "land"))
    {
        MTGCardInstance * proposed = AIPlayerBaka::FindCardToPlay(pMana, type);
        if (!proposed)
            return NULL;

        std::ostringstream q;
        q << "Land drop: play " << proposed->name << " now?";
        vector<string> opts;
        opts.push_back("Hold " + proposed->name + " - do not play it now");
        opts.push_back("Play " + proposed->name);
        int pick = askModel(q.str(), opts);
        if (pick == kChoicePending)
        {
            gotPayments.clear(); //nothing plays this tick; re-poll next tick
            return NULL;
        }
        if (pick == 0)
        {
            DebugTrace("AIPlayerGPT: vetoed playing " << proposed->name);
            gotPayments.clear(); //drop the heuristic's payment plan for this card
            return NULL;
        }
        return proposed; //play it (pick 1), or defer to the heuristic (-1)
    }

    //Spells: one free choice across every castable card, whatever type rung
    //of the heuristic's cast ladder we were called for. computeActions stops
    //at the first rung that returns a card, so answering the whole question
    //here means a single model call per casting window.
    //type=="" is the instant/interrupt window: computeActions will discard
    //anything that is not instant-speed, so only offer what can actually go.
    bool instantWindow = !*type;

    vector<MTGCardInstance *> candidates;
    vector<string> opts;
    opts.push_back("Cast nothing right now");
    std::set<string> seen; //same name+zone = same decision; list it once

    struct ZoneScan { MTGGameZone * zone; const char * label; };
    ZoneScan scans[] = {
        { game->hand, "" },
        { game->graveyard, " [from your graveyard]" },
        { game->exile, " [from exile]" },
        { game->commandzone, " [from your command zone]" },
    };
    for (size_t s = 0; s < sizeof(scans) / sizeof(scans[0]); s++)
    {
        MTGGameZone * zone = scans[s].zone;
        for (int i = 0; i < zone->nb_cards; i++)
        {
            MTGCardInstance * card = zone->cards[i];
            if (card->isLand())
                continue; //the land rung handles those
            if (instantWindow
                && !card->hasType(Subtypes::TYPE_INSTANT) && !card->has(Constants::FLASH)
                && !card->has(Constants::ASFLASH))
                continue;
            if (zone == game->graveyard
                && !card->has(Constants::CANPLAYFROMGRAVEYARD) && !card->has(Constants::TEMPFLASHBACK)
                && !(card->getManaCost() && card->getManaCost()->getFlashback())
                && !(card->getManaCost() && card->getManaCost()->getRetrace()))
                continue;
            if (zone == game->exile && !card->has(Constants::CANPLAYFROMEXILE))
                continue;
            if (card->hasType(Subtypes::TYPE_LEGENDARY) && game->inPlay->findByName(card->name))
                continue;
            if (game->playRestrictions->canPutIntoZone(card, game->stack) == PlayRestriction::CANT_PLAY)
                continue;
            if (!roughlyPayable(card, pMana))
                continue;
            string key = card->getDisplayName() + scans[s].label;
            if (!seen.insert(key).second)
                continue;

            std::ostringstream o;
            o << "Cast " << card->getDisplayName();
            if (card->getManaCost() && card->getManaCost()->getConvertedCost())
                o << " {" << card->getManaCost()->toString() << "}";
            if (card->isCreature())
                o << " (" << card->power << "/" << card->toughness << ")";
            o << scans[s].label;
            candidates.push_back(card);
            opts.push_back(o.str());
        }
    }

    //Nothing castable: only one outcome, no model call.
    if (candidates.empty())
        return NULL;

    int pick = askModel("Casting decision: which card do you cast now, if any?", opts);
    if (pick == kChoicePending)
        return NULL; //no cast this tick; the answer is consumed on a later poll
    if (pick < 0) //model deferred or endpoint failed: heuristic decides
        return AIPlayerBaka::FindCardToPlay(pMana, type);
    if (pick == 0) //"cast nothing": hold everything this window
    {
        DebugTrace("AIPlayerGPT: chose to cast nothing");
        return NULL;
    }

    //Validate and price the pick with the heuristic's own machinery: with
    //aiForcedCandidate set, AIPlayerBaka::FindCardToPlay examines only this
    //card, runs the full legality/restriction/target checks, and leaves
    //gotPayments / payAlternative set for exactly this play.
    MTGCardInstance * chosen = candidates[pick - 1];
    aiForcedCandidate = chosen;
    MTGCardInstance * validated = AIPlayerBaka::FindCardToPlay(pMana, "*");
    aiForcedCandidate = NULL;
    if (validated)
    {
        DebugTrace("AIPlayerGPT: casting " << validated->name << " (model's pick"
                   << (validated == chosen ? ")" : " via combo hint)"));
        return validated;
    }
    //The cheap menu filter let through something the real machinery rejects
    //(cast restriction, no legal target, unpayable kicker...). Fall back to
    //the heuristic's own pick rather than burning another model call.
    DebugTrace("AIPlayerGPT: model chose " << chosen->name
               << " but it fails validation; deferring to heuristic");
    gotPayments.clear();
    return AIPlayerBaka::FindCardToPlay(pMana, type);
}

int AIPlayerGPT::computeActions()
{
    //Menus must be intercepted here: the base loop reacts to
    //selectMenuOption's return in the same tick, and its contract has no
    //"not yet" value (negative = click the cancel item). While the menu
    //decision's model call is in flight we simply do nothing and re-poll;
    //once decided, mirror the base reaction exactly.
    if (!mEndpoint.empty() && observer->currentlyActing() == this)
    {
        ActionLayer * object = observer->mLayers->actionLayer();
        if (object->menuObject)
        {
            int doThis = selectMenuOption();
            if (doThis == kChoicePending)
                return 1;
            if (doThis >= 0)
            {
                if (object->abilitiesMenu->isMultipleChoice)
                    observer->mLayers->actionLayer()->ButtonPressedOnMultipleChoice(doThis);
                else
                    observer->mLayers->actionLayer()->doReactTo(doThis);
            }
            else if (doThis < 0 || object->checkCantCancel())
                observer->mLayers->actionLayer()->doReactTo(object->abilitiesMenu->mObjects.size() - 1);
            return 1;
        }
    }

    //Mulligan: the engine offers it to humans through the game menu and the
    //heuristic AI simply never mulligans. The window predicate mirrors the
    //GUI's (GameStateDuel): the AI's own first turn, nothing developed yet.
    //Each distinct hand is asked about once (prompt-keyed cache); a mulligan
    //redraws the hand, which changes the prompt and earns a fresh ask.
    if (!mEndpoint.empty()
        && ((observer->turn == 0 && observer->getCurrentGamePhase() == MTG_PHASE_FIRSTMAIN)
            || (observer->turn == 1 && observer->getCurrentGamePhase() < MTG_PHASE_DRAW))
        && observer->currentPlayer == this && observer->currentlyActing() == this
        && game->hand->nb_cards > 0 && game->inPlay->nb_cards == 0
        && game->graveyard->nb_cards == 0 && game->exile->nb_cards == exiledBySerum)
    {
        std::ostringstream q;
        q << "Mulligan decision: you are on " << game->hand->nb_cards
          << " cards. Keep this opening hand, or mulligan (shuffle it back and draw one fewer)?";
        vector<string> opts;
        opts.push_back("Keep this hand");
        opts.push_back("Mulligan");
        int pick = askModel(q.str(), opts);
        if (pick == kChoicePending)
            return 1; //decision in flight; poll again next tick
        if (pick == 1)
        {
            DebugTrace("AIPlayerGPT: taking a mulligan at " << game->hand->nb_cards << " cards");
            observer->Mulligan(this);
            return 1;
        }
        //keep (0), or defer/failure (-1): play on
    }
    return AIPlayerBaka::computeActions();
}

//The base Act, with the async insertions. The base is monolithic: after
//computeActions it interprets an empty clickstream as "nothing to do" and
//COMMITS a pass (cancelInterruptOffer / userRequestNextGamePhase). While a
//model call is in flight the AI must do neither - not act, not pass - so
//the game loop keeps running (and rendering) until the answer lands and the
//re-polled flow consumes it.
int AIPlayerGPT::Act(float dt)
{
    if (mEndpoint.empty())
        return AIPlayerBaka::Act(dt);

    if (asyncBusy())
    {
        mThinkTime += dt;
        //Keep a pending interrupt offer from timing out while the model is
        //still deciding whether to respond.
        observer->mLayers->stackLayer()->extendInterruptOffer(this);
        return 0;
    }
    mThinkTime = 0;

    //--- base AIPlayerBaka::Act body, with the post-computeActions check ---
    if (!(observer->currentlyActing() == this))
        return 0;

    oldGamePhase = observer->getCurrentGamePhase();

    if (mFastTimerMode)
        timer -= dt * 3;
    else
        timer -= dt;
    if (timer > 0)
        return 0;
    initTimer();

    if (combatDamages())
        return 0;
    interruptIfICan();

    if (!(observer->currentlyActing() == this))
    {
        DebugTrace("Cannot interrupt");
        return 0;
    }
    if (clickstream.empty())
        computeActions();
    if (clickstream.empty())
    {
        //A model call started during computeActions: the decision is not
        //made yet, so neither pass priority nor decline the interrupt.
        if (asyncBusy())
            return 0;
        if (observer->isInterrupting == this)
        {
            if (observer->mExtraPayment && observer->mExtraPayment->source->controller() == this)
            {
                ExtraManaCost * check = dynamic_cast<ExtraManaCost *>(observer->mExtraPayment->costs[0]);
                if (check)
                {
                    vector<MTGAbility *> CostToPay = canPayMana(observer->mExtraPayment->source, check->costToPay, check->source->has(Constants::ANYTYPEOFMANAABILITY));
                    if (CostToPay.size())
                    {
                        payTheManaCost(check->costToPay, check->source->has(Constants::ANYTYPEOFMANAABILITY), check->source, CostToPay);
                    }
                    else
                    {
                        observer->mExtraPayment->action->CheckUserInput(JGE_BTN_SEC);
                        observer->mExtraPayment = NULL;
                    }
                }
                return 0;
            }
            observer->mLayers->stackLayer()->cancelInterruptOffer();
        }
        else
        {
            if (observer->currentActionPlayer == this)
                observer->userRequestNextGamePhase();
        }
    }
    else
    {
        while (clickstream.size())
        {
            AIAction * action = clickstream.front();
            action->Act();
            SAFE_DELETE(action);
            clickstream.pop();
        }
    }
    return 1;
}

void AIPlayerGPT::Render()
{
    AIPlayerBaka::Render();
    //The visible answer to "is it frozen or thinking?": a small animated
    //line whenever this player's model call is in flight.
    if (mEndpoint.empty() || !asyncBusy())
        return;
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::MAIN_FONT);
    if (!font)
        return;
    char buf[48];
    int dots = 1 + ((int) (mThinkTime * 2)) % 3;
    sprintf(buf, "opponent is thinking%.*s", dots, "...");
    font->SetColor(ARGB(220, 255, 255, 200));
    font->DrawString(buf, SCREEN_WIDTH / 2, 2, JGETEXT_CENTER);
}

int AIPlayerGPT::selectMenuOption()
{
    if (mEndpoint.empty())
        return AIPlayerBaka::selectMenuOption();
    ActionLayer * object = observer->mLayers->actionLayer();
    if (!object->menuObject)
        return AIPlayerBaka::selectMenuOption();

    MTGCardInstance * ctx = object->currentActionCard;

    if (object->abilitiesMenu->isMultipleChoice && ctx)
    {
        MenuAbility * currentMenu = NULL;
        for (size_t m = object->mObjects.size() - 1; m > 0; m--)
        {
            MenuAbility * ability = dynamic_cast<MenuAbility *>(object->mObjects[m]);
            if (ability && ability->triggered)
            {
                currentMenu = ability;
                break;
            }
        }
        if (!currentMenu || currentMenu->abilities.empty())
            return AIPlayerBaka::selectMenuOption();

        //X announcement: the menu's buttons ARE the X values, so the pick
        //index is the X value itself (the heuristic always dumped all mana).
        if (dynamic_cast<AAWhatsX *>(currentMenu->abilities[0]))
        {
            int maxX = manaPool->getConvertedCost()
                       - currentMenu->abilities[0]->source->getManaCost()->getConvertedCost();
            if (maxX < 0)
                return AIPlayerBaka::selectMenuOption();
            int shown = maxX > 20 ? 20 : maxX; //bound the menu for huge pools
            vector<string> opts;
            for (int x = 0; x <= shown; x++)
            {
                std::ostringstream o;
                o << "X = " << x;
                opts.push_back(o.str());
            }
            int pick = askModel("Announce the value of X for " + ctx->getDisplayName()
                                + " (you can afford up to the largest listed value):", opts);
            if (pick == kChoicePending)
                return kChoicePending;
            if (pick < 0)
                return AIPlayerBaka::selectMenuOption();
            return pick;
        }

        vector<string> opts;
        for (size_t mk = 0; mk < currentMenu->abilities.size(); mk++)
            opts.push_back(currentMenu->abilities[mk]->getMenuText());
        int pick = askModel("Choose one mode for " + ctx->getDisplayName() + ":", opts);
        if (pick == kChoicePending)
            return kChoicePending;
        if (pick < 0)
            return AIPlayerBaka::selectMenuOption();
        return pick;
    }

    //Regular menu: items with GetId() > 0 map to action-layer abilities;
    //the trailing cancel item (when the menu is cancellable) is the decline.
    vector<string> opts;
    vector<int> indices;
    for (unsigned int k = 0; k < object->abilitiesMenu->mObjects.size(); k++)
    {
        if (object->abilitiesMenu->mObjects[k]->GetId() <= 0)
            continue;
        MTGAbility * ab = (MTGAbility *) object->mObjects[object->abilitiesMenu->mObjects[k]->GetId()];
        opts.push_back(ab ? ab->getMenuText() : string("(option)"));
        indices.push_back((int) k);
    }
    if (opts.empty())
        return AIPlayerBaka::selectMenuOption();
    bool canDecline = !object->checkCantCancel();
    if (canDecline)
        opts.push_back("Decline - do nothing");
    //One real option and no way to decline: only one outcome, no model call.
    if (opts.size() == 1)
        return indices[0];
    string decision = ctx ? ("Choose an option for " + ctx->getDisplayName() + ":")
                          : string("A choice is required - choose an option:");
    int pick = askModel(decision, opts);
    if (pick == kChoicePending)
        return kChoicePending;
    if (pick < 0)
        return AIPlayerBaka::selectMenuOption();
    if (canDecline && pick == (int) opts.size() - 1)
        return -1; //computeActions reacts to -1 by clicking the cancel item
    return indices[pick];
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

    //Multi-target selection runs as a sequence of single picks: each round
    //asks for one more target (with a "Done" escape once the minimum is
    //satisfiable), so the reply stays a single reliable number instead of a
    //free-form list. Prompt-keyed caching keeps repeated polling cheap.
    vector<Targetable *> picks;
    for (;;)
    {
        vector<Targetable *> targets;
        vector<string> opts;
        int stopOffset = 0;
        bool mayStop = multi && !picks.empty() && !tc->targetMin;
        if (mayStop)
        {
            opts.push_back("Done - no further targets");
            stopOffset = 1;
        }

        for (int i = 0; i < 2; i++)
        {
            Player * p = observer->players[i];
            if (tc->canTarget((Targetable *) p) && !tc->alreadyHasTarget(p)
                && std::find(picks.begin(), picks.end(), (Targetable *) p) == picks.end())
            {
                targets.push_back(p);
                opts.push_back(describeTarget(this, p));
            }
            MTGPlayerCards * pz = p->game;
            MTGGameZone * zones[] = { pz->hand, pz->library, pz->inPlay, pz->graveyard, pz->stack, pz->exile, pz->commandzone, pz->sideboard, pz->reveal };
            for (int j = 0; j < 9; j++)
            {
                MTGGameZone * zone = zones[j];
                for (int k = 0; k < zone->nb_cards && targets.size() < 40; k++)
                {
                    MTGCardInstance * t = zone->cards[k];
                    if (!tc->canTarget(t) || tc->alreadyHasTarget(t))
                        continue;
                    if (std::find(picks.begin(), picks.end(), (Targetable *) t) != picks.end())
                        continue;
                    targets.push_back(t);
                    opts.push_back(describeTarget(this, t));
                }
            }
        }
        if (targets.empty())
            break;

        std::ostringstream q;
        q << "Choose ";
        if (!multi)
            q << "the target";
        else
        {
            q << "target " << (picks.size() + 1);
            if (!unlimited)
                q << " of " << (tc->targetMin ? "exactly " : "up to ") << tc->maxtargets;
        }
        q << " for " << tc->source->getDisplayName();

        int pick = askModel(q.str(), opts);
        if (pick == kChoicePending)
            return 1; //chooser stays open; earlier picks re-derive from cache next tick
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
        if (mayStop && pick == 0)
            break;
        picks.push_back(targets[pick - stopOffset]);
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
    //Reuse the engine's own click paths so the mechanics (source-first
    //ordering, player clicks, card-batch click, clickstream flushing) stay
    //byte-identical to the heuristic player's.
    if (!multi)
        return clickSingleTarget(tc, picks, chosenCard);
    return clickMultiTarget(tc, picks);
}

int AIPlayerGPT::chooseAttackers()
{
    //Only drive the declare-attackers step; anywhere else, stay out of the way.
    if (mEndpoint.empty() || observer->getCurrentGamePhase() != MTG_PHASE_COMBATATTACKERS)
        return AIPlayerBaka::chooseAttackers();

    //Every creature that can legally attack is an opposed decision: attack
    //or hold. Already-declared attackers drop out of the candidate set, so
    //repeated ticks converge; held creatures simply never get clicked and
    //the phase advances when priority passes (no clickstream => pass).
    CardDescriptor cd;
    cd.init();
    cd.setType("creature");
    MTGCardInstance * card = NULL;
    while ((card = cd.nextmatch(game->inPlay, card)))
    {
        if (card->isAttacker() || !card->canAttack())
            continue;

        std::ostringstream q;
        q << "Combat: declare attackers. Attack with " << card->name
          << " (" << card->power << "/" << card->toughness << ")?";
        vector<string> opts;
        opts.push_back("Hold " + card->name + " back");
        opts.push_back("Attack with " + card->name);
        int pick = askModel(q.str(), opts);
        if (pick == kChoicePending)
            return 1; //declared attackers stand; the rest re-poll next tick
        if (pick != 1)
            continue; //hold, or model deferred -> do not attack with this one

        if (card->attackCost)
        {
            MTGAbility * a = observer->mLayers->actionLayer()->getAbility(MTGAbility::ATTACK_COST);
            doAbility(a, card);
            observer->cardClick(card, MTGAbility::ATTACK_COST);
        }
        observer->cardClick(card, MTGAbility::MTG_ATTACK_RULE);
    }
    return 1;
}

int AIPlayerGPT::chooseBlockers()
{
    //Only drive the declare-blockers step; anywhere else, stay out of the way.
    if (mEndpoint.empty() || observer->getCurrentGamePhase() != MTG_PHASE_COMBATBLOCKERS)
        return AIPlayerBaka::chooseBlockers();
    if (observer->currentPlayer == this) //never block on my own turn (Baka guard)
        return 0;

    vector<MTGCardInstance *> attackers;
    CardDescriptor ca;
    ca.init();
    ca.setType("creature");
    MTGCardInstance * a = NULL;
    while ((a = ca.nextmatch(opponent()->game->inPlay, a)))
        if (a->isAttacker())
            attackers.push_back(a);
    if (attackers.empty())
        return 1;

    CardDescriptor cb;
    cb.init();
    cb.setType("creature");
    cb.unsecureSetTapped(-1);
    MTGCardInstance * blk = NULL;
    while ((blk = cb.nextmatch(game->inPlay, blk)))
    {
        if (blk->defenser || !blk->canBlock()) //already blocking, or cannot
            continue;

        vector<MTGCardInstance *> legal;
        for (size_t j = 0; j < attackers.size(); j++)
            if (blk->canBlock(attackers[j]))
                legal.push_back(attackers[j]);
        if (legal.empty())
            continue; //nothing this creature can block -> no decision

        std::ostringstream q;
        q << "Combat: declare blockers. Which attacker should " << blk->name
          << " (" << blk->power << "/" << blk->toughness << ") block?";
        vector<string> opts;
        opts.push_back("Do not block with " + blk->name);
        for (size_t j = 0; j < legal.size(); j++)
        {
            std::ostringstream o;
            o << "Block " << legal[j]->name << " (" << legal[j]->power << "/" << legal[j]->toughness << ")";
            opts.push_back(o.str());
        }
        int pick = askModel(q.str(), opts);
        if (pick == kChoicePending)
            return 1; //committed blocks stand; the rest re-poll next tick
        if (pick <= 0)
            continue; //do-not-block, or model deferred

        MTGCardInstance * chosen = legal[pick - 1];
        //Clicking the block rule cycles this creature's defenser through its
        //legal attackers (and NULL); click until it lands on the chosen one.
        //Bounded so an unexpected cycle can never spin forever.
        int guard = (int) attackers.size() + 2;
        observer->cardClick(blk, MTGAbility::MTG_BLOCK_RULE);
        while (blk->defenser != chosen && guard-- > 0)
            observer->cardClick(blk, MTGAbility::MTG_BLOCK_RULE);
    }
    return 1;
}

#endif //WITH_GPT_AI
