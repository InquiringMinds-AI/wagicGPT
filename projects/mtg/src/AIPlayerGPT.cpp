#include "PrecompiledHeader.h"

#ifdef WITH_GPT_AI

#include "AIPlayerGPT.h"
#include "LegalActions.h"
#include "DecisionContract.h"
#include <chrono>
#include "GptConfig.h"
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
#include "GuiCombat.h"
#include "DuelLayers.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <set>
#include <algorithm>
#include <thread>
#include <mutex>
#include <sys/stat.h>
#include <ctime>

using json = nlohmann::json;

namespace
{

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
    "All rules are subject to modification by the effects in play.\n"
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

//The reply protocol is appended in code AFTER the (user-editable) system
//prompt template, so a stale or hand-edited template cannot silently drop
//the contract the parsers and the plan carry-forward depend on.
const char * kReplyProtocol =
    "\nHOW TO REPLY (every decision):\n"
    "First give your choice, in exactly the format the decision asks for (usually a single number).\n"
    "Then, on a new line, write PLAN: followed by your complete game plan from here on.\n"
    "Nothing you write is kept except that PLAN line. At your next decision you will see only the "
    "game log, the current board, your last PLAN line, and the new choices - your reasoning and "
    "your earlier plans will have dropped out of context. So every PLAN must be complete and "
    "self-contained: state your full current plan, or your full revised plan if the situation "
    "changed. Never write a fragment like \"continue as before\". Keep the plan CONCISE - a few "
    "sentences of intent, not an analysis.\n";

//The card's rules text, single-line and bounded, for option/target lines:
//the deciding fact belongs ON the choice, not in a distant deck blob (the
//model picked discard/removal targets near-arbitrarily from bare names).
string cardTextSnippet(MTGCardInstance * card, size_t maxLen)
{
    string text = card->text;
    for (size_t i = 0; i < text.size(); i++)
        if (text[i] == '\n')
            text[i] = ' ';
    if (text.size() > maxLen)
    {
        size_t cut = text.rfind(' ', maxLen);
        text = text.substr(0, (cut == string::npos || cut < maxLen / 2) ? maxLen : cut) + "...";
    }
    return text;
}

//State-computed payoff magnitudes: cards like Gray Merchant carry their
//payoff as a live expression in the auto= script ("lifeleech:
//-type:manab:mybattlefield" = devotion to black). Static rules text
//cannot say what that is worth NOW, so evaluate the expression on the
//current board - with the same WParsedInt the resolution will use - and
//put the number on the option. Only dynamic (non-numeric) amounts are
//annotated: plain numbers are already in the rules text, "x" is
//unknowable before announcement, and "rand" would draw from the game
//RNG just by being rendered.
string dynamicMagnitudes(MTGCardInstance * card)
{
    static const struct { const char * key; const char * label; bool absValue; } kVerbs[] = {
        { "lifeleech:", "drains", true },
        { "damage:", "damage", true },
        { "life:", "life", false },
        { "draw:", "draws", true },
        { "prevent:", "prevents", true },
    };
    string text = card->magicText;
    for (size_t i = 0; i < text.size(); i++)
        text[i] = (char) tolower((unsigned char) text[i]);
    std::ostringstream out;
    std::set<string> seen;
    int count = 0;
    for (size_t v = 0; v < sizeof(kVerbs) / sizeof(kVerbs[0]) && count < 3; v++)
    {
        size_t pos = 0;
        while (count < 3 && (pos = text.find(kVerbs[v].key, pos)) != string::npos)
        {
            size_t start = pos + strlen(kVerbs[v].key);
            pos = start;
            size_t end = text.find_first_of(" \t\r\n", start); //the parser slices amounts at whitespace too
            string expr = text.substr(start, end == string::npos ? string::npos : end - start);
            if (expr.empty())
                continue;
            bool numeric = true;
            for (size_t k = (expr[0] == '-' || expr[0] == '+') ? 1 : 0; k < expr.size(); k++)
                if (!isdigit((unsigned char) expr[k]))
                {
                    numeric = false;
                    break;
                }
            if (numeric || expr == "x" || expr == "-x" || expr.find("rand") != string::npos)
                continue;
            if (!seen.insert(string(kVerbs[v].key) + expr).second)
                continue;
            WParsedInt val(expr, NULL, card);
            int n = val.getValue();
            if (kVerbs[v].absValue)
                n = abs(n);
            //Cast-option magnitudes evaluate with the source still in hand,
            //so a mybattlefield devotion count misses the caster's OWN pips
            //(Gray Merchant rendered "drains 10" when 12 would resolve -
            //4/4 across waves; the pilot hand-recounted and erred). Add the
            //card's own colored symbols exactly when the expression is a
            //bare my-battlefield mana-symbol count and the card - itself a
            //permanent - has not joined that battlefield yet: that is what
            //resolution's WParsedInt will see.
            {
                size_t manaPos = expr.find("type:mana");
                if (kVerbs[v].absValue && manaPos != string::npos
                    && expr.find(":mybattlefield") != string::npos
                    && expr.find("othertype") == string::npos
                    && manaPos + 9 < expr.size()
                    && !card->controller()->game->inPlay->hasCard(card)
                    && (card->isCreature() || card->hasType(Subtypes::TYPE_ARTIFACT)
                        || card->hasType(Subtypes::TYPE_ENCHANTMENT)
                        || card->hasType(Subtypes::TYPE_PLANESWALKER)))
                {
                    int colorIdx = ManaCost::parseManaSymbol(expr[manaPos + 9]);
                    if (colorIdx > 0 && card->getManaCost())
                        n += card->getManaCost()->getCost(colorIdx);
                }
            }
            out << (count ? ", " : "") << kVerbs[v].label << " " << n;
            count++;
        }
    }
    if (count)
        return " {right now: " + out.str() + "}";
    return "";
}

//The set keyword abilities (flying, first strike, can't block...) - the
//LIVE effective set, including granted/lost ones printed text cannot show.
string keywordList(MTGCardInstance * card)
{
    std::ostringstream out;
    bool first = true;
    for (int j = 0; j < Constants::NB_BASIC_ABILITIES; j++)
    {
        if (!card->basicAbilities[j])
            continue;
        out << (first ? "" : ", ") << Constants::MTGBasicAbilities[j];
        first = false;
    }
    return out.str();
}

//Primary type for non-creature option/target lines (a discard pick needs
//to know a bare name is a sorcery vs a land).
string typeTag(MTGCardInstance * card)
{
    if (card->isCreature())
        return "";
    if (card->isLand())
        return "land";
    if (card->hasType(Subtypes::TYPE_ARTIFACT))
        return "artifact";
    if (card->hasType(Subtypes::TYPE_ENCHANTMENT))
        return "enchantment";
    if (card->hasType(Subtypes::TYPE_PLANESWALKER))
        return "planeswalker";
    if (card->hasType(Subtypes::TYPE_INSTANT))
        return "instant";
    if (card->hasType(Subtypes::TYPE_SORCERY))
        return "sorcery";
    return "";
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
            out << " " << cost->toString(); //toString carries its own braces
        if (card->isCreature())
        {
            out << " (" << card->power << "/" << card->toughness << ")";
            //Surface the live delta the static decklist text cannot carry: a
            //creature pumped, counter'd, enchanted or equipped is no longer
            //its printed stats. Only meaningful in play (withStatus).
            if (withStatus && (card->power != card->basepower || card->toughness != card->basetoughness))
                out << " (printed " << card->basepower << "/" << card->basetoughness << ")";
        }
        else
        {
            //a bare name does not say what it IS (lands excepted: the name does)
            string tag = typeTag(card);
            if (!tag.empty() && tag != "land")
                out << " [" << tag << "]";
        }
        if (withStatus)
        {
            //the LIVE keyword set - granted/lost abilities the decklist
            //text cannot show (Bloodghast "can't block", taught flying...)
            if (card->isCreature())
            {
                string kw = keywordList(card);
                if (!kw.empty())
                    out << " [" << kw << "]";
            }
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
            //the forward direction too: the equipment/aura itself names its
            //current host, so "is this already attached?" is answerable from
            //either end of the relationship. Equipment keeps its host in
            //target (excluded from auraParent by the engine); auras in
            //auraParent.
            {
                MTGCardInstance * host = NULL;
                if (card->hasType(Subtypes::TYPE_EQUIPMENT) || card->hasType("fortification"))
                    host = card->target;
                else if (card->auraParent)
                    host = card->auraParent;
                if (host)
                    out << " [attached to: " << host->getDisplayName() << "]";
            }
            //A tapped creature reads as harmless and the pilot builds plans
            //on that (wave-7 deck140 collapse: "tapped = no threat" bridged
            //a sweeper-hold it should never have satisfied). Name the truth
            //at the flag: it untaps and attacks again next turn.
            if (card->isTapped())
                out << (card->isCreature() ? " [tapped - untaps and can attack next turn]" : " [tapped]");
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
    {
        o << " (" << c->power << "/" << c->toughness << ")";
        string kw = keywordList(c);
        if (!kw.empty())
            o << " [" << kw << "]";
    }
    else
    {
        string tag = typeTag(c);
        if (!tag.empty())
            o << " [" << tag << "]";
    }
    if (c->currentZone)
        o << " [" << (c->controller() == me ? "your " : "opponent's ") << zoneDesc(c->currentZone) << "]";
    if (c->isTapped())
        o << " [tapped]";
    //The deciding fact rides on the option: a discard or removal pick from
    //bare names is a coin flip for the model.
    string txt = cardTextSnippet(c, 110);
    if (!txt.empty())
        o << " - \"" << txt << "\"";
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
    std::chrono::steady_clock::time_point started; //request launch time
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
                mLastLatencyMs = (long) std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - mAsyncState->started).count();
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

    //Idle: build the request on the game thread (the prompt members are not
    //shared with the worker) and launch the round trip in the background.
    string requestBody = buildRequestBody(userMsg);
    string url = mEndpoint + "/v1/chat/completions";
    string key = mApiKey;
    std::shared_ptr<AsyncState> state = mAsyncState;
    {
        std::lock_guard<std::mutex> g(state->mtx);
        state->status = 1;
        state->prompt = userMsg;
        state->response.clear();
        state->started = std::chrono::steady_clock::now();
    }
    long timeoutMs = mTimeoutMs;
    std::thread([state, url, requestBody, key, timeoutMs]() {
        string body = gptHttpPost(url, requestBody, timeoutMs, key);
        std::lock_guard<std::mutex> g(state->mtx);
        state->status = 2;
        state->response = body;
    }).detach();
    return kChoicePending;
}

AIPlayerGPT::AIPlayerGPT(GameObserver *observer, string deckFile, string deckfileSmall, string avatarFile, MTGDeck * deck)
    : AIPlayerBaka(observer, deckFile, deckfileSmall, avatarFile, deck), mAsyncState(std::make_shared<AsyncState>()), mThinkTime(0), mNoticeTicks(0), mBlocksDoneTurn(-1), mAttacksDoneTurn(-1), mPassDeclineTurn(-1), mTransSeq(0), mLastLatencyMs(-1), mGameEndLogged(false), mGameStartLogged(false), mNarrationLogged(0), mNarratedTurnOwner(NULL), mNarratedTurnNumber(-1), mDealDone(false), mLastChoice(-1)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    //File config first, environment variables override.
    GptSettings cfg = GptSettings::load();
    mConfigUrls = cfg.urls;
    mConfigModel = cfg.model;
    mMaxTokens = cfg.maxTokens;
    mApiKey = cfg.key;
    mTimeoutMs = 1000L * cfg.timeoutSecs;
    if (const char * key = getenv("WAGIC_GPT_KEY"))
        mApiKey = key;
    if (const char * to = getenv("WAGIC_GPT_TIMEOUT"))
        mTimeoutMs = 1000L * atol(to);
    if (mTimeoutMs < 5000)
        mTimeoutMs = 5000;
    mThinking = getenv("WAGIC_GPT_THINKING") ? envFlag("WAGIC_GPT_THINKING") : (cfg.thinking == 1);
    //Telemetry consent implies local decision logging: the log IS the data
    //a future contribution/upload mechanism would share.
    bool translog = getenv("WAGIC_GPT_TRANSLOG") ? envFlag("WAGIC_GPT_TRANSLOG")
                                                 : (cfg.translog == 1 || cfg.telemetry == 1);
    if (translog)
    {
        if (const char * home = getenv("HOME"))
        {
            string dir = string(home) + "/.Wagic";
            mkdir(dir.c_str(), 0755);
            dir += "/ai"; mkdir(dir.c_str(), 0755);
            dir += "/gpt"; mkdir(dir.c_str(), 0755);
            dir += "/logs"; mkdir(dir.c_str(), 0755);
            std::ostringstream p;
            p << dir << "/" << time(NULL) << "-" << deckfileSmall << "-" << (void *) this << ".jsonl";
            mTransLogPath = p.str();
        }
    }
    resolveEndpoint();
    if (mEndpoint.empty())
    {
        fprintf(stderr, "AIPlayerGPT: no LLM endpoint reachable, falling back to Baka heuristics for every decision\n");
        setNotice("no LLM endpoint reachable - the heuristic AI is playing", 12.0f);
    }
    else
        fprintf(stderr, "AIPlayerGPT: using %s (model %s)\n", mEndpoint.c_str(), mModel.c_str());
}

bool AIPlayerGPT::isEnabled()
{
    //The environment overrides in both directions (WAGIC_AI=gpt forces the
    //LLM opponent on, any other value forces it off); without it, the GPT
    //options tab's master switch (enabled= in the config file) decides.
    const char * mode = getenv("WAGIC_AI");
    if (mode && *mode)
        return string(mode) == "gpt";
    return GptSettings::load().enabled == 1;
}

void AIPlayerGPT::setNotice(const string& text, float seconds)
{
    mNotice = text;
    mNoticeTicks = (int) (seconds * 60); //decremented per rendered frame
}

//One header record per seat log: decks, names, and a game_id BOTH seats
//share (the observer's address) - reviewers previously paired seat logs by
//filename epoch arithmetic, which broke on harvested copies (wave-7 7d).
void AIPlayerGPT::ensureGameStartRecord()
{
    if (mGameStartLogged || mTransLogPath.empty())
        return;
    mGameStartLogged = true;
    std::ostringstream gid;
    gid << (void *) observer;
    json rec = {
        {"seq", mTransSeq++},
        {"kind", "gamestart"},
        {"model", mModel},
        {"game_id", gid.str()},
        {"my_deck", deckFileSmall},
        {"my_deck_name", deckName},
        {"opp_deck", opponent() ? opponent()->deckFileSmall : ""},
        {"opp_deck_name", opponent() ? opponent()->deckName : ""},
    };
    std::ofstream f(mTransLogPath.c_str(), std::ios::app);
    if (f)
        f << rec.dump() << "\n";
}

void AIPlayerGPT::writeTransLog(const char * kind, const string& userMsg, const string& reply, int choice, int optionCount,
                                const string& chosenText, const char * fallback, const vector<string> * optionTexts)
{
    if (mTransLogPath.empty())
        return;
    ensureGameStartRecord();
    json rec = {
        {"seq", mTransSeq++},
        {"kind", kind},
        {"model", mModel},
        {"prompt", userMsg},
        {"reply", reply},
        {"choice", choice},
        {"options", optionCount},
        {"turn", observer->turn},
        {"phase", observer->getCurrentGamePhaseName()},
        {"my_life", life},
        {"opp_life", opponent() ? opponent()->life : 0},
        {"latency_ms", mLastLatencyMs},
    };
    mLastLatencyMs = -1; //consumed: the next record without a round trip is cache/reuse
    //Narration delta: the game events that landed since the previous
    //record. A consumed cast's outcome (resolved/countered/died) shows up
    //here on the NEXT record - machine-readable without re-parsing prompts.
    if (mNarration.size() > mNarrationLogged)
    {
        rec["events"] = mNarration.substr(mNarrationLogged);
        mNarrationLogged = mNarration.size();
    }
    if (!chosenText.empty())
        rec["chosen_text"] = chosenText;
    if (fallback)
        rec["fallback"] = fallback;
    //the exact option strings, when the seam has them as a list (ask /
    //priority) - offered-vs-taken tallies without re-parsing the prompt
    if (optionTexts)
        rec["options_text"] = *optionTexts;
    std::ofstream f(mTransLogPath.c_str(), std::ios::app);
    if (f)
        f << rec.dump() << "\n";
}

void AIPlayerGPT::gameEnded()
{
    logGameEnd();
}

void AIPlayerGPT::logGameEnd()
{
    //Idempotent: the game-over transition frame can repeat before the state
    //machine moves on, and only ONE gameend record may close the file.
    if (mGameEndLogged || mTransLogPath.empty())
        return;
    ensureGameStartRecord();
    mGameEndLogged = true;
    bool iWon = observer->didWin(this);
    bool oppWon = opponent() ? observer->didWin(opponent()) : false;
    json rec = {
        {"seq", mTransSeq++},
        {"kind", "gameend"},
        {"model", mModel},
        {"won", iWon},
        {"draw", !iWon && !oppWon},
        {"turn", observer->turn},
        {"my_life", life},
        {"opp_life", opponent() ? opponent()->life : 0},
    };
    std::ofstream f(mTransLogPath.c_str(), std::ios::app);
    if (f)
        f << rec.dump() << "\n";
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
        string firstModel;
        if (!gptProbeEndpoint(candidates[i], mApiKey, firstModel))
            continue;
        mEndpoint = candidates[i];
        if (const char * model = getenv("WAGIC_GPT_MODEL"))
            mModel = model;
        else if (!mConfigModel.empty())
            mModel = mConfigModel;
        else
            mModel = firstModel;
        return;
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
            out << " " << cost->toString();
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
    string tmpl = gptReadAsset("system_prompt.txt");
    if (!tmpl.empty())
    {
        replaceAllOccurrences(tmpl, "{MY_DECK}", myDeck);
        replaceAllOccurrences(tmpl, "{OPPONENT_DECK}", oppDeck);
        replaceAllOccurrences(tmpl, "{STRATEGY_GUIDE}", guideBlock);
        mSystemPrompt = tmpl + kReplyProtocol;
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
    sys << "\nDuring the game you will receive the game log (everything that has happened so far), the current "
           "board state (each card's current power/toughness, counters, and anything attached to it), your own "
           "last stated plan, and the choices that are legal for you right now.\n"
           "Before you choose, weigh each action by what it COSTS you against what it gains. A cost is anything "
           "you give up to take the action: mana, tapping a permanent, SACRIFICING one of your own permanents, "
           "paying life, or discarding. Many activated abilities cost more than mana - the action line states "
           "its cost in brackets, and a creature or other valuable permanent is rarely worth trading for "
           "something lesser. Then pick the play whose gain most clearly exceeds its cost on the current board: "
           "develop your position, hold interaction when nothing is urgent, and attack when the math favors you.\n"
           "Reason like a skilled human player.\n";
    sys << kReplyProtocol;

    mSystemPrompt = sys.str();
}

void AIPlayerGPT::flushOpeningHand()
{
    if (mDealDone && mOpeningHand.empty())
        return;
    if (!mOpeningHand.empty())
    {
        std::ostringstream o;
        o << "- Your opening hand (" << mOpeningHand.size() << " cards): ";
        for (size_t i = 0; i < mOpeningHand.size(); i++)
            o << (i ? "; " : "") << mOpeningHand[i];
        mNarration += o.str() + "\n";
        mOpeningHand.clear();
    }
    mDealDone = true;
}

string AIPlayerGPT::assemblePrompt(const string& tail)
{
    std::ostringstream u;
    flushOpeningHand();
    if (!mNarration.empty())
        u << "GAME LOG (everything that has happened so far):\n" << mNarration << "\n";
    u << "--- CURRENT SITUATION ---\n" << serializeGameState();
    if (!mCurrentPlan.empty())
        u << "\nYOUR PLAN (as you last stated it): " << mCurrentPlan << "\n";
    u << "\n" << tail;
    return u.str();
}

void AIPlayerGPT::appendNarration(const string& line)
{
    if (line.empty())
        return;
    flushOpeningHand(); //the deal precedes whatever is being narrated
    if (!mPendingPhase.empty())
    {
        mNarration += "- " + mPendingPhase + "\n";
        mPendingPhase.clear();
    }
    mNarration += "- " + line + "\n";
    //Bound a runaway narrative (very long games, degenerate combos); keep
    //the tail, cut on a line boundary. The narration is otherwise
    //append-only, which is what keeps the prompt prefix cacheable.
    if (mNarration.size() > 24000)
    {
        size_t nl = mNarration.find('\n', mNarration.size() - 20000);
        if (nl != string::npos)
            mNarration = "(...earlier events trimmed...)\n" + mNarration.substr(nl + 1);
    }
}

void AIPlayerGPT::narrateDecision(const string& line)
{
    appendNarration(line);
}

string AIPlayerGPT::consumePlan(const string& content)
{
    //Drop any inline think block first (same as parseChoice).
    string text = content;
    size_t thinkEnd = text.rfind("</think>");
    if (thinkEnd != string::npos)
        text = text.substr(thinkEnd + 8);

    //Find the "PLAN:" marker, case-insensitive; the colon is required so
    //prose like "I plan to attack" before the choice cannot truncate it.
    size_t pos = string::npos;
    for (size_t i = 0; i + 5 <= text.size(); i++)
    {
        if (tolower((unsigned char) text[i]) == 'p' && tolower((unsigned char) text[i + 1]) == 'l'
            && tolower((unsigned char) text[i + 2]) == 'a' && tolower((unsigned char) text[i + 3]) == 'n'
            && text[i + 4] == ':')
        {
            pos = i;
            break;
        }
    }
    if (pos == string::npos)
        return text; //no plan stated: keep the previous one

    string plan = text.substr(pos + 5);
    size_t s = plan.find_first_not_of(" \t\r\n");
    size_t e = plan.find_last_not_of(" \t\r\n");
    if (s != string::npos)
    {
        plan = plan.substr(s, e - s + 1);
        if (plan.size() > 1600)
        {
            //bound a runaway plan; cut at a sentence boundary when one
            //exists (a mid-sentence stump re-fed every decision reads
            //like an instruction fragment)
            size_t dot = plan.rfind(". ", 1600);
            plan = plan.substr(0, (dot != string::npos && dot > 400) ? dot + 1 : 1600);
        }
        //A reply cut off by max_tokens leaves a mid-word stump ("...value
        //by sac", observed live); trim back to the last complete sentence.
        char last = plan.empty() ? '.' : plan[plan.size() - 1];
        if (last != '.' && last != '!' && last != '?')
        {
            size_t dot = plan.find_last_of(".!?");
            if (dot != string::npos && dot > plan.size() / 2)
                plan = plan.substr(0, dot + 1);
        }
        mCurrentPlan = plan;
    }
    return text.substr(0, pos);
}

int AIPlayerGPT::receiveEvent(WEvent * event)
{
    int result = AIPlayerBaka::receiveEvent(event);

    //Opening deal & mulligans: collapse own draws into the buffered
    //opening-hand line, and keep the opponent's hidden deal/mulligan churn
    //("puts a card into their hand/library" x7) out of the narration.
    if (WEventZoneChange * z = dynamic_cast<WEventZoneChange *>(event))
    {
        if (z->card && z->to)
        {
            if (!mDealDone && z->from == game->library && z->to == game->hand)
            {
                mOpeningHand.push_back(z->card->getDisplayName());
                return result;
            }
            if (!mDealDone && z->from == game->hand && z->to == game->library)
            {
                mOpeningHand.clear(); //mulligan shuffle-back
                return result;
            }
            bool mulliganWindow = (observer->turn == 0)
                || (observer->turn == 1 && observer->getCurrentGamePhase() < MTG_PHASE_DRAW);
            if (mulliganWindow && z->to->owner && z->to->owner != this
                && (z->to == z->to->owner->game->hand || z->to == z->to->owner->game->library))
                return result;
        }
    }

    //Phase changes never enter the narration on their own: they park in the
    //pending marker and only get written as the header of the next real
    //line (appendNarration) - empty phases leave no trace. A change of TURN
    //is the exception: its header is written unconditionally, so the
    //narration is always contextually clear about whose turn it is.
    if (WEventPhaseChange * pe = dynamic_cast<WEventPhaseChange *>(event))
    {
        if (pe->to)
        {
            //Turn ownership comes from observer->currentPlayer, NOT from
            //pe->to->player: the ring builds the next turn's Phase objects
            //against a player id that has already flipped at the EOT step,
            //so their ->player is the WRONG side from turn 2 on (observed
            //live: every label inverted after turn 1). The engine's own
            //rules all read currentPlayer for the same reason. currentPlayer
            //and turn are flipped together (nextPlayer), so the pair is
            //coherent by the time the new turn's phase events fire.
            if (observer->currentPlayer != mNarratedTurnOwner || observer->turn != mNarratedTurnNumber)
            {
                if (mNarratedTurnOwner) //not before the game's first turn header
                    flushOpeningHand(); //the deal belongs to turn 1, before this header
                mNarratedTurnOwner = observer->currentPlayer;
                mNarratedTurnNumber = observer->turn;
                mPendingPhase.clear();
                std::ostringstream t;
                t << "=== Turn " << (observer->turn + 1) << " - "
                  << (observer->currentPlayer == this ? "YOUR turn" : "opponent's turn") << " ===";
                mNarration += t.str() + "\n";
            }
            //the turn header carries the owner; the marker only needs the phase
            mPendingPhase = string("Phase: ") + Constants::MTGPhaseNames[pe->to->id];
        }
        return result;
    }

    string line = describeEvent(event);
    if (!line.empty())
        appendNarration(line);
    return result;
}

string AIPlayerGPT::describeEvent(WEvent * event)
{
    std::ostringstream out;

    if (WEventZoneChange * e = dynamic_cast<WEventZoneChange *>(event))
    {
        if (!e->card || !e->to)
            return "";
        //No origin zone = the card was CREATED (a token, a conjured copy):
        //putInZone rewrites from = previousZone, which is NULL for a card
        //that never was anywhere. Dropping these hid every token from the
        //narration (Krenko's Command resolved and Goblins just appeared).
        if (!e->from)
        {
            out << (e->to->owner == this ? "Your " : "Opponent's ")
                << e->card->getDisplayName();
            if (e->card->isCreature())
                out << " (" << e->card->power << "/" << e->card->toughness << ")";
            out << ": created -> " << zoneDesc(e->to);
            return out.str();
        }
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

    //(phase changes are handled in receiveEvent: lazy marker + turn header)

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

    //The stack, top-first. Pending spells/abilities were previously invisible
    //in the situation block: the model saw "Cast Counterspell" offered but not
    //WHAT it could counter, and read post-cast game-log lines as already
    //resolved (wave-4 E1). Phase-transition plumbing is skipped as noise.
    {
        ActionStack * stack = observer->mLayers->stackLayer();
        std::vector<string> items;
        for (size_t i = 0; i < stack->mObjects.size(); i++)
        {
            Interruptible * it = (Interruptible *) stack->mObjects[i];
            if (!it || it->state != NOT_RESOLVED)
                continue;
            if (it->type != ACTION_SPELL && it->type != ACTION_ABILITY)
                continue; //phase steps / damage plumbing, not respondable objects
            std::ostringstream line;
            Player * ctrl = it->source ? it->source->controller() : NULL;
            line << (ctrl == this ? "your " : (ctrl ? "opponent's " : ""))
                 << it->getDisplayName()
                 << (it->type == ACTION_SPELL ? " [spell]" : " [triggered/activated ability]");
            if (it->type == ACTION_SPELL)
            {
                Spell * sp = (Spell *) it;
                std::ostringstream tgt;
                bool first = true;
                for (Targetable * t = sp->getNextTarget(); t; t = sp->getNextTarget(t))
                {
                    tgt << (first ? "" : ", ");
                    first = false;
                    if (MTGCardInstance * c = dynamic_cast<MTGCardInstance *>(t))
                        tgt << c->getDisplayName();
                    else if (Player * pl = dynamic_cast<Player *>(t))
                        tgt << (pl == this ? "you" : "opponent");
                    else
                        tgt << "something";
                }
                if (!first)
                    line << " targeting " << tgt.str();
            }
            items.push_back(line.str());
        }
        if (!items.empty())
        {
            out << "ON THE STACK, waiting to resolve (top resolves FIRST - you can respond now):\n";
            int n = 1;
            for (int i = (int) items.size() - 1; i >= 0; i--, n++)
                out << "  " << n << (n == 1 ? " (top): " : ": ") << items[i] << "\n";
        }
    }
    //Honest mana line: the pool being empty between actions is normal (the
    //engine taps lands automatically); what the player can actually spend is
    //the potential of its untapped producers. The old "Mana in your pool:
    //(none)" line was the model's #1 source of false can't-pay beliefs.
    string pool = this->getManaPool()->toString();
    ManaCost * potential = getPotentialMana();
    int sources = potential ? potential->getConvertedCost() : 0;
    string colors = potential ? potential->toString() : "";
    SAFE_DELETE(potential);
    out << "Mana available: ";
    if (sources)
        out << colors << " from " << sources << " untapped source" << (sources > 1 ? "s" : "")
            << " (tapped automatically when you cast)";
    else
        out << "(no untapped sources)";
    if (!pool.empty())
        out << " | Already in pool: " << pool;
    out << "\n";

    out << "Your hand: ";
    describeZoneCards(out, game->hand, false);
    //Surfaced creature COUNTS: a cluttered board line studded with
    //artifacts and [tapped] flags gets miscounted (wave-7 deck140: every
    //sweeper mistiming stood on a wrong creature tally). An integer at the
    //header is the representation the per-deck workarounds stood in for.
    int myCreatures = 0, oppCreatures = 0;
    for (int i = 0; i < game->inPlay->nb_cards; i++)
        if (game->inPlay->cards[i]->isCreature())
            myCreatures++;
    if (opp)
        for (int i = 0; i < opp->game->inPlay->nb_cards; i++)
            if (opp->game->inPlay->cards[i]->isCreature())
                oppCreatures++;
    out << "\nYour battlefield (creatures: " << myCreatures << "): ";
    describeZoneCards(out, game->inPlay, true);
    if (opp)
    {
        out << "\nOpponent battlefield (creatures: " << oppCreatures << "): ";
        describeZoneCards(out, opp->game->inPlay, true);
        out << "\nOpponent hand size: " << opp->game->hand->nb_cards
            << " | Opponent library: " << opp->game->library->nb_cards << " cards";
        //Artifact counts feed metalcraft/affinity-style decisions and are
        //tedious to re-count from the board lines.
        int myArtifacts = 0, oppArtifacts = 0;
        for (int i = 0; i < game->inPlay->nb_cards; i++)
            if (game->inPlay->cards[i]->hasType(Subtypes::TYPE_ARTIFACT))
                myArtifacts++;
        for (int i = 0; i < opp->game->inPlay->nb_cards; i++)
            if (opp->game->inPlay->cards[i]->hasType(Subtypes::TYPE_ARTIFACT))
                oppArtifacts++;
        if (myArtifacts || oppArtifacts)
            out << "\nArtifacts in play: you " << myArtifacts << " | opponent " << oppArtifacts;
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
    {
        out << " targeting " << action.target->getDisplayName();
        //Re-attaching to the current host is rules-legal but changes nothing.
        //The board line's cue (two power numbers / {attached:}) proved
        //insufficient at range - the pilot read it and re-equipped anyway
        //(wave-5 deck110: 15 of 35 equips were no-ops). Say it AT the option.
        //Host lookup is type-split: EQUIPMENT keeps its host in target (the
        //engine excludes it from auraParent - cf. the TYPE_EQUIPMENT carve-outs
        //in GameObserver's enchantment upkeep); auras use auraParent.
        MTGCardInstance * moved = action.click ? action.click
                                               : (action.ability ? action.ability->source : NULL);
        if (moved)
        {
            MTGCardInstance * host = NULL;
            if (moved->hasType(Subtypes::TYPE_EQUIPMENT) || moved->hasType("fortification"))
                host = moved->target;
            else if (moved->hasSubtype(Subtypes::TYPE_AURA))
                host = moved->auraParent ? moved->auraParent : moved->target;
            if (host && host == action.target)
                out << " (ALREADY attached to it - this would change NOTHING)";
        }
    }
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

    //The BENEFIT, not just the cost: menu texts like "Put in Play" (a
    //fetchland crack) read as pure downside next to their [cost: ...] -
    //the model never cracked a fetch. The source card's rules text states
    //what the activation is actually FOR.
    MTGCardInstance * src = action.click ? action.click
                                         : (action.ability ? action.ability->source : NULL);
    if (src)
    {
        string txt = cardTextSnippet(src, 140);
        if (!txt.empty())
            out << " {card text: \"" << txt << "\"}";
        out << dynamicMagnitudes(src);
    }
    return out.str();
}

string AIPlayerGPT::buildRequestBody(const string& userMsg)
{
    //Exactly two messages, always: the per-duel head and the assembled
    //decision tail. No transcript - the narration inside the user message
    //carries the history, and its append-only front keeps the prefix
    //cacheable across the whole game.
    json messages = json::array();
    messages.push_back({{"role", "system"}, {"content", mSystemPrompt}});
    messages.push_back({{"role", "user"}, {"content", userMsg}});

    //Room for the choice plus the mandatory complete PLAN restatement; a
    //truncated reply parsed as "no choice" and held creatures back (and a
    //qwen plan runs long despite the brevity instruction - observed 512
    //cutting one mid-sentence).
    long maxTokens = mThinking ? 2048 : 1024;
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
    //The official OpenAI API is the exception: it REJECTS unknown top-level
    //parameters with a 400, which would silently degrade every decision to
    //the heuristic - omit the field there.
    if (mEndpoint.find("api.openai.com") == string::npos)
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

    //The reply contract puts the chosen option number FIRST. Prefer an
    //in-range integer at the HEAD of the text ("N", "N.", "N)" - leading
    //whitespace tolerated). Keeping the LAST in-range integer (the
    //previous behavior) let echoed option text hijack the choice: a mana
    //cost {2}{r}{r}, a stat (3/3) or a prose life total that happened to
    //be in range outvoted the model's leading number (8 desyncs across 5
    //corpus games, one thrown-away exact-lethal). Fallback when the head
    //is not a number: the FIRST in-range integer anywhere - never the
    //last; the tail is where echoed stats and prose numbers live. The
    //in-range guard itself stays: taking the trailing digit blindly (the
    //oldest behavior) parsed "(2/4)" echoes as -1 and silently held
    //creatures back.
    size_t i = 0;
    while (i < text.size() && isspace((unsigned char) text[i]))
        i++;
    if (i < text.size() && isdigit((unsigned char) text[i]))
    {
        size_t j = i;
        while (j < text.size() && isdigit((unsigned char) text[j]))
            j++;
        int n = atoi(text.substr(i, j - i).c_str());
        if (n >= 0 && n <= optionCount)
            return n;
    }
    int choice = -1;
    i = 0;
    while (i < text.size())
    {
        if (isdigit((unsigned char) text[i]))
        {
            size_t j = i;
            while (j < text.size() && isdigit((unsigned char) text[j]))
                j++;
            int n = atoi(text.substr(i, j - i).c_str());
            if (n >= 0 && n <= optionCount)
            {
                choice = n;
                break;
            }
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
    if (mPassDeclineTurn != observer->turn)
    {
        mPassDeclineCount.clear();
        mPassDeclineTurn = observer->turn;
    }
    for (size_t c = 0; c < candidates.size(); c++)
    {
        string line = describeAction(*candidates[c]);
        if (!seenLines.insert(line).second)
            continue;
        //Twice pass-declined this turn: the model has said "hold" enough -
        //stop re-asking every window (the held-fetch tax; see header).
        std::map<string, int>::iterator dc = mPassDeclineCount.find(line);
        if (dc != mPassDeclineCount.end() && dc->second >= 2)
            continue;
        shown.push_back(candidates[c]);
        shownLines.push_back(line);
        index++;
        tail << index << ". " << line << "\n";
    }
    //Every distinct action is suppressed as already-declined: pass without
    //another model call (that IS the model's standing answer this turn).
    if (!index)
    {
        DebugTrace("AIPlayerGPT[ph" << phase << "]: all actions pass-declined this turn; passing");
        return NULL;
    }
    tail << "\nWhich action do you take? Reply with ONLY the number first (0 = pass priority) - the number must be the first character of your reply, with no option text before it - then your PLAN: line.";

    //The dedupe/deadlock key is board state + question, NOT the assembled
    //prompt: consuming an answer appends to the narration and updates the
    //plan, and a full-prompt key would read that as a state change.
    string askKey = serializeGameState() + tail.str();
    string userMsg = assemblePrompt(tail.str());
    bool unchanged = (askKey == mLastAskKey);

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
        //The plan is split off BEFORE choice parsing: plan prose is full of
        //numbers that would otherwise misparse as the chosen action.
        string decisionPart = consumePlan(content);
        choice = parseChoice(decisionPart, index);
        if (content.empty())
            setNotice("model reply failed or timed out - the heuristic decides", 5.0f);
        else if (choice >= 1 && choice <= index)
            narrateDecision("You: " + describeAction(*shown[choice - 1]));

        mLastAskKey = askKey;
        mLastChoice = choice;
        //A fresh deliberate pass declines every offered line (cached
        //replays of the same window don't re-count - one look, one vote).
        if (choice == 0)
            for (size_t s = 0; s < shownLines.size(); s++)
                mPassDeclineCount[shownLines[s]]++;
        {
            const char * fb = (choice >= 0) ? NULL : (content.empty() ? "empty_reply" : "unparsed_reply");
            string chosen = (choice >= 1 && choice <= index) ? describeAction(*shown[choice - 1])
                          : (choice == 0 ? string("pass") : string());
            writeTransLog("priority", userMsg, content, choice, index, chosen, fb, &shownLines);
        }
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

int AIPlayerGPT::askModel(const string& decision, const vector<string>& options, bool narrateChoice)
{
    //"Only one valid action": no decision to make, no model call.
    if (options.empty())
        return -1;
    if (options.size() == 1)
        return 0;
    if (mEndpoint.empty())
        return -1; //no endpoint: caller falls back to the heuristic

    if (mSystemPrompt.empty())
        buildSystemPrompt();

    std::ostringstream tail;
    tail << decision << "\n";
    for (size_t i = 0; i < options.size(); i++)
        tail << (i + 1) << ". " << options[i] << "\n";
    tail << "\nReply with ONLY the number of your choice first - the number must be the first character of your reply, with no option text before it - then your PLAN: line.";
    string tailStr = tail.str();

    //State-plus-question answer cache: the same questions are re-polled
    //every AI tick until the game state moves on, and several distinct
    //questions can alternate within one tick. The key excludes the
    //narration and the plan (see the header) so that consuming one answer
    //cannot invalidate another already given for this same state - the
    //earlier picks of a multi-target selection re-derive from this cache.
    string askKey = serializeGameState() + tailStr;
    std::map<string, int>::iterator cached = mAskCache.find(askKey);
    if (cached != mAskCache.end())
        return (cached->second >= 1 && cached->second <= (int) options.size()) ? cached->second - 1 : -1;

    string userMsg = assemblePrompt(tailStr);
    string content;
    if (pollCompletion(userMsg, content) == kChoicePending)
        return kChoicePending; //callers unwind this tick and re-poll

    //Plan split BEFORE choice parsing: plan prose is full of numbers.
    string decisionPart = consumePlan(content);
    int choice = parseChoice(decisionPart, (int) options.size());
    if (content.empty())
        setNotice("model reply failed or timed out - the heuristic decides", 5.0f);
    else if (narrateChoice && choice >= 1 && choice <= (int) options.size())
        //first line of the question only: multi-line asks (damage order)
        //would bloat a narration that persists all game
        narrateDecision(decision.substr(0, decision.find('\n')) + " -> " + options[choice - 1]);

    mAskCache[askKey] = choice;
    {
        bool valid = choice >= 1 && choice <= (int) options.size();
        const char * fb = valid ? NULL : (content.empty() ? "empty_reply" : "unparsed_reply");
        writeTransLog("ask", userMsg, content, choice, (int) options.size(),
                      valid ? options[choice - 1] : string(), fb, &options);
    }
    DebugTrace("AIPlayerGPT: " << decision << " -> chose " << choice << " of " << options.size());

    return (choice >= 1) ? choice - 1 : -1; //0 or parse-fail: defer to caller
}

namespace
{
    //Willingness policy for the oracle: same as the inherited heuristic's
    //(canHandleCost may pre-choose extra-cost payments).
    class GptManaPolicy : public ManaEngine::ManaPolicy
    {
    public:
        GptManaPolicy(AIPlayerBaka * _ai) : ai(_ai) {}
        int canHandle(MTGAbility * producer) { return ai->canHandleCost(producer); }
    private:
        AIPlayerBaka * ai;
    };
}

MTGCardInstance * AIPlayerGPT::FindCardToPlay(ManaCost * pMana, const char * type)
{
    //No endpoint, or a scripted combo is mid-execution: heuristic as-is.
    if (mEndpoint.empty() || comboCards.size())
        return AIPlayerBaka::FindCardToPlay(pMana, type);

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
        if (lands.empty())
            return NULL;

        //Play options FIRST (the model favors option 1, and playing a land
        //is nearly always right - decline goes LAST, house ordering rule).
        vector<string> opts;
        for (size_t li = 0; li < lands.size(); li++)
            opts.push_back("Play " + lands[li].card->getDisplayName() + lands[li].zoneLabel);
        opts.push_back(lands.size() == 1
                       ? "Hold " + lands[0].card->getDisplayName() + " - do not play it now"
                       : "Play no land right now");

        std::ostringstream q;
        if (lands.size() == 1)
            q << "Land drop: play " << lands[0].card->getDisplayName() << " now?";
        else
            q << "Land drop: which land do you play now, if any?";
        int pick = askModel(q.str(), opts, false); //the play narrates itself as a zone event
        if (pick == kChoicePending)
        {
            gotPayments.clear(); //nothing plays this tick; re-poll next tick
            return NULL;
        }
        if (pick == (int) lands.size())
        {
            DebugTrace("AIPlayerGPT: held the land drop");
            gotPayments.clear();
            return NULL;
        }
        if (pick < 0) //model deferred or endpoint failed: heuristic decides
            return AIPlayerBaka::FindCardToPlay(pMana, type);

        //Validate the pick with the heuristic's own machinery (residual
        //gates, dice, payment state) - same pattern as the cast seam below.
        MTGCardInstance * chosenLand = lands[pick].card;
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
    vector<string> opts; //"Cast nothing" is appended LAST (positional
                         //anchoring: the model favors option 1, and
                         //nothing-first likely drove the pass rate)

    //The legal cast set comes from the contract (oracle-backed: zone gates,
    //legendary rule, play restrictions, affordability, 601.2c target
    //validity); this seam only renders it for the model. The APPLY side of
    //a cast still rides Baka's pricing machinery below (aiForcedCandidate)
    //until c5 moves payment+clickstream into the manager.
    GptManaPolicy policy(this);
    DecisionRequest castReq;
    if (!DecisionManager::buildCastSpell(this, policy, pMana, instantWindow, castReq))
        return NULL; //nothing castable: only one outcome, no model call
    const vector<LegalActionsOracle::Cast> & casts = castReq.casts;
    for (size_t ci = 0; ci < casts.size(); ci++)
    {
        MTGCardInstance * card = casts[ci].card;
        ManaCost * cost = card->getManaCost();
        std::ostringstream o;
        if (!casts[ci].viaAlternative)
        {
            o << "Cast " << card->getDisplayName();
            if (cost && cost->getConvertedCost())
                o << " " << cost->toString();
            if (card->isCreature())
                o << " (" << card->power << "/" << card->toughness << ")";
            o << casts[ci].zoneLabel;
            o << dynamicMagnitudes(card);
        }
        else
        {
            o << "Cast " << card->getDisplayName() << " with its ";
            if (!cost->getAlternative()->alternativeName.empty())
                o << cost->getAlternative()->alternativeName << " cost";
            else
                o << "alternative cost";
            o << " " << cost->getAlternative()->toString();
            if (card->isCreature())
                o << " (" << card->power << "/" << card->toughness << ")";
            o << casts[ci].zoneLabel;
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
                            hits << (firstHit ? "" : ", ") << sz->cards[zi]->getDisplayName();
                            firstHit = false;
                        }
                }
                SAFE_DELETE(tc);
                if (!firstHit)
                    o << " - can target on the stack: " << hits.str();
            }
        }
        candidates.push_back(card);
        candidateUsesAlt.push_back(casts[ci].viaAlternative);
        opts.push_back(o.str());
    }

    //Nothing castable: only one outcome, no model call.
    if (candidates.empty())
        return NULL;
    opts.push_back("Cast nothing right now"); //the decline goes LAST

    //no narration: a cast narrates itself as zone events, "nothing" is a non-action
    int pick = askModel(string("Casting decision (") + observer->getCurrentGamePhaseName()
                        + (observer->currentPlayer == this ? ", YOUR turn" : ", opponent's turn")
                        + "): which card do you cast now, if any?", opts, false);
    if (pick == kChoicePending)
        return NULL; //no cast this tick; the answer is consumed on a later poll
    if (pick < 0) //model deferred or endpoint failed: heuristic decides
        return AIPlayerBaka::FindCardToPlay(pMana, type);
    if (pick == (int) candidates.size()) //"cast nothing": hold everything this window
    {
        DebugTrace("AIPlayerGPT: chose to cast nothing");
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
        return validated;
    }
    //The cheap menu filter let through something the real machinery rejects
    //(cast restriction, no legal target, unpayable kicker...). Fall back to
    //the heuristic's own pick rather than burning another model call.
    DebugTrace("AIPlayerGPT: model chose " << chosen->name
               << " but it fails validation; deferring to heuristic");
    writeTransLog("defer", "", "", -1, 0, chosen->name, "deferred_to_heuristic");
    gotPayments.clear();
    return AIPlayerBaka::FindCardToPlay(pMana, type);
}

int AIPlayerGPT::orderBlockers()
{
    if (mEndpoint.empty())
        return AIPlayerBaka::orderBlockers();
    if (!(ORDER == observer->combatStep && observer->currentPlayer == this))
        return 0;

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
            int pick = askModel(q.str(), opts);
            if (pick == kChoicePending)
                return 1; //stay in the ORDER step; re-poll next tick
            if (pick < 0)
            {
                deferred = true; //keep the declaration order
                break;
            }
            ordered.push_back(remaining[pick]);
            remaining.erase(remaining.begin() + pick);
        }
        if (!deferred && ordered.size())
        {
            ordered.push_back(remaining[0]);
            atk->blockers = ordered;
            DebugTrace("AIPlayerGPT: damage order for " << atk->card->getDisplayName()
                       << " set, first: " << ordered[0]->card->getDisplayName());
        }
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
            //heuristic click path, exactly as before the contract
            int doThis = AIPlayerBaka::selectMenuOption();
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
        int pick = askModel(q.str(), opts, false); //keep = non-action; a taken mulligan narrates below
        if (pick == kChoicePending)
            return 1; //decision in flight; poll again next tick
        if (pick == 1)
        {
            DebugTrace("AIPlayerGPT: taking a mulligan at " << game->hand->nb_cards << " cards");
            narrateDecision("You took a mulligan");
            mDealDone = false; //re-open the opening-hand collapse for the redraw
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
bool AIPlayerGPT::decisionPending(float dt)
{
    //c5d: the async gate is a policy hook now - the base AIPlayerBaka::Act
    //consults it at entry and again after computeActions, so the mirrored
    //Act body this class used to carry is gone.
    if (mEndpoint.empty())
        return false;
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

int AIPlayerGPT::chooseMenuAction(const DecisionRequest & req, DecisionAction & act)
{
    MTGCardInstance * ctx = req.contextCard;

    if (req.kind == DecisionRequest::ANNOUNCE_X)
    {
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
        int pick = askModel("Announce the value of X for "
                            + (ctx ? ctx->getDisplayName() : string("this spell"))
                            + " (every listed value is affordable; option 1 is the LARGEST X)."
                            + " Reply with the OPTION number, not the X value:", shown);
        if (pick == kChoicePending)
            return kChoicePending;
        if (pick >= 0 && pick < (int) shown.size())
            pick = (int) req.optionTexts.size() - 1 - pick; //shown space -> index==X space
        else if (pick < 0)
            pick = AIPlayerBaka::selectMenuOption(); //heuristic: max affordable X
        if (pick >= (int) req.optionTexts.size())
            pick = (int) req.optionTexts.size() - 1;
        act.choice = pick < 0 ? 0 : pick;
        return 0;
    }

    if (req.kind == DecisionRequest::CHOOSE_MODE)
    {
        int pick = askModel("Choose one mode for "
                            + (ctx ? ctx->getDisplayName() : string("this spell")) + ":", req.optionTexts);
        if (pick == kChoicePending)
            return kChoicePending;
        if (pick < 0)
            pick = AIPlayerBaka::selectMenuOption(); //heuristic: same index space
        if (pick < 0 || pick >= (int) req.optionTexts.size())
            pick = 0;
        act.choice = pick;
        return 0;
    }

    //CHOOSE_MENU
    vector<string> opts = req.optionTexts;
    if (req.canDecline)
        opts.push_back("Decline - do nothing");
    //One real option and no way to decline: only one outcome, no model call.
    if (opts.size() == 1)
    {
        act.choice = 0;
        return 0;
    }
    string decision = ctx ? ("Choose an option for " + ctx->getDisplayName() + ":")
                          : string("A choice is required - choose an option:");
    int pick = askModel(decision, opts);
    if (pick == kChoicePending)
        return kChoicePending;
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
    if (req.canDecline && pick == (int) opts.size() - 1)
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
    //spell being paid for, never the chooser's own source)
    vector<MTGCardInstance *> cands;
    vector<string> opts;
    for (size_t i = 0; i < req.targetCandidates.size(); i++)
    {
        MTGCardInstance * c = dynamic_cast<MTGCardInstance *>(req.targetCandidates[i]);
        if (!c || c == source || c == tc->source)
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
    int pick = askModel("Choose what to pay for " + what
                        + "'s additional cost (it will be consumed):", opts);
    if (pick == kChoicePending)
        return NULL; //payment attempt aborts this tick; re-polled next tick
    if (pick < 0 || pick >= (int) cands.size())
        return AIPlayerBaka::chooseCostTarget(tc, source);
    return cands[pick];
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
    DecisionRequest req; //the last round's request carries the offered set for apply
    for (;;)
    {
        vector<Targetable *> targets;
        vector<string> opts;
        //"Done" goes LAST (after the real targets): the model favors option
        //1, and an early-listed escape biased multi-target picks short.
        bool mayStop = multi && !picks.empty() && !tc->targetMin;

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
                opts.push_back(describeTarget(this, t));
            }
            req = round;
        }
        if (targets.empty())
            break;
        if (mayStop)
            opts.push_back("Done - no further targets");

        //Granted/inner abilities ride a nameless fake card - Liliana's "+1:
        //each player discards" chooser rendered 'Choose the target for '
        //(blank). The waiting action element's menu text names the effect.
        string effectName = tc->source->getDisplayName();
        if (effectName.empty())
        {
            MTGAbility * waiting = dynamic_cast<MTGAbility *>(observer->mLayers->actionLayer()->isWaitingForAnswer());
            if (waiting)
                effectName = waiting->getMenuText();
            if (effectName.empty())
                effectName = "this effect";
        }

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
        q << " for " << effectName;

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

//Scan a bundled-attacker reply for the set of attackers to send: "A<n>"
//tokens (or bare numbers) in [1..nAttackers]. Returns >0 = that many named,
//0 = an explicit decline (a "none/hold/pass" keyword with no numbers), and
//-1 = unusable (empty or no signal at all -> caller falls back to Baka).
//The 0/-1 split matters: attacking with nobody is a legitimate choice, so
//"none" must NOT trigger the heuristic override the way a garbled reply does.
static int parseAttackerSet(const string& content, size_t nAttackers, vector<bool>& out)
{
    out.assign(nAttackers, false);
    int named = 0;
    for (size_t i = 0; i < content.size(); i++)
    {
        //Accept "A3" or a bare "3"; skip digits that are part of a P/T echo
        //like "2/2" by ignoring a number immediately preceded/followed by '/'.
        char prev = (i > 0) ? content[i - 1] : ' ';
        bool aPrefixed = (content[i] == 'A' || content[i] == 'a')
                         && i + 1 < content.size() && isdigit(content[i + 1]);
        //A bare number, but not one glued to a letter or the tail of a "2/2".
        bool bareStart = isdigit(content[i]) && !isalnum(prev) && prev != '/';
        if (!aPrefixed && !bareStart)
            continue;
        size_t j = aPrefixed ? i + 1 : i;
        int n = 0;
        while (j < content.size() && isdigit(content[j]))
            n = n * 10 + (content[j++] - '0');
        if (j < content.size() && content[j] == '/')
            continue; //a "3/3" power echo, not an attacker index
        if (n >= 1 && n <= (int) nAttackers && !out[n - 1])
        {
            out[n - 1] = true;
            named++;
        }
        i = j; //advance past the number
    }
    if (named > 0)
        return named;
    //No numbers: distinguish an explicit decline from a garbled reply.
    string lower;
    for (size_t i = 0; i < content.size() && i < 200; i++)
        lower += (char) tolower(content[i]);
    if (lower.find("none") != string::npos || lower.find("hold") != string::npos
        || lower.find("no attack") != string::npos || lower.find("nobody") != string::npos
        || lower.find("pass") != string::npos || lower.find("don't attack") != string::npos)
        return 0; //explicit "attack with nobody" - valid, not a fallback
    return -1; //unusable
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

    //ONE bundled decision for the whole attack. Per-creature asks decided
    //each attacker in isolation (a bad line for alpha strikes and racing)
    //and cost N round trips; the model now plans the attack as a whole.
    if (mSystemPrompt.empty())
        buildSystemPrompt();
    std::ostringstream tail;
    tail << "Combat: declare ALL attackers for this turn in ONE decision.\n"
            "Your creatures that can attack:\n";
    for (size_t j = 0; j < attackers.size(); j++)
    {
        tail << "A" << (j + 1) << ". " << attackers[j]->name
             << " (" << attackers[j]->power << "/" << attackers[j]->toughness << ")";
        string kw = keywordList(attackers[j]);
        if (!kw.empty())
            tail << " [" << kw << "]";
        tail << "\n";
    }
    tail << "Reply with the numbers of the attackers you send, comma-separated"
            " (e.g. \"A1, A3\"), or \"none\" to attack with nobody this turn."
            " Then your PLAN: line.";
    string userMsg = assemblePrompt(tail.str());

    string content;
    if (pollCompletion(userMsg, content) == kChoicePending)
        return 1; //decision in flight; nothing declared yet, re-poll next tick

    //Plan split BEFORE the attacker-set parse: numbers (and words like
    //"hold") in the plan prose must not read as attack declarations.
    string decisionPart = consumePlan(content);
    vector<bool> send;
    int result = content.empty() ? -1 : parseAttackerSet(decisionPart, attackers.size(), send);

    if (result < 0)
    {
        //Unusable reply: the heuristic declares this turn's attack instead.
        writeTransLog("attackers", userMsg, content, result, (int) attackers.size(),
                      "", content.empty() ? "empty_reply" : "unparsed_reply");
        setNotice("model reply failed - the heuristic attacks", 5.0f);
        mAttacksDoneTurn = observer->turn;
        return AIPlayerBaka::chooseAttackers();
    }

    //Answer through the contract: the manager applies the declaration.
    //Attack-COST creatures still need the policy to pre-pay mana (see the
    //contract header) before the apply.
    DecisionAction act;
    string declared;
    for (size_t j = 0; j < attackers.size(); j++)
    {
        if (!send[j] || !attackers[j]->canAttack())
            continue;
        if (attackers[j]->attackCost)
        {
            MTGAbility * a = observer->mLayers->actionLayer()->getAbility(MTGAbility::ATTACK_COST);
            doAbility(a, attackers[j]);
            observer->cardClick(attackers[j], MTGAbility::ATTACK_COST);
        }
        act.attackers.push_back(attackers[j]);
        declared += (declared.empty() ? "" : ", ") + attackers[j]->name;
    }
    DecisionManager::applyDeclareAttackers(req, act);
    writeTransLog("attackers", userMsg, content, result, (int) attackers.size(),
                  declared.empty() ? string("no attackers") : declared, NULL);
    narrateDecision(declared.empty() ? string("You declared no attackers this turn")
                                     : ("You declared attackers: " + declared));
    mAttacksDoneTurn = observer->turn;
    DebugTrace("AIPlayerGPT: declared attack (" << result << " of " << attackers.size() << ") in one reply");
    return 1;
}

//Scan a bundled-blocking reply for "B<i>:A<j>" / "B<i>:none" pairs (any of
//": - > =" or spaces as separator). Unmentioned or malformed blockers stay
//out of combat. Returns how many well-formed pairs were found - zero means
//the reply is unusable and the caller falls back to the heuristic.
static int parseBlockAssignments(const string& content, size_t nBlockers, size_t nAttackers, vector<int>& out)
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
        int b = 0;
        while (j < content.size() && isdigit(content[j]))
            b = b * 10 + (content[j++] - '0');
        while (j < content.size() && (content[j] == ':' || content[j] == '-' || content[j] == '>'
                                      || content[j] == '=' || content[j] == ' '))
            j++;
        int a = -1; //-1 malformed; 0 none; else attacker number
        if (j < content.size() && (content[j] == 'A' || content[j] == 'a') && j + 1 < content.size()
            && isdigit(content[j + 1]))
        {
            a = 0;
            for (size_t k = j + 1; k < content.size() && isdigit(content[k]); k++)
                a = a * 10 + (content[k] - '0');
        }
        else if (content.compare(j, 4, "none") == 0 || content.compare(j, 4, "None") == 0
                 || content.compare(j, 4, "NONE") == 0)
            a = 0;
        if (a < 0 || b < 1 || b > (int) nBlockers || a > (int) nAttackers)
            continue;
        if (out[b - 1] != 0)
            continue; //first assignment wins: a creature blocks at most one
                      //attacker, so ignore a later "B1:A3" after "B1:A1"
                      //(the model occasionally double-assigns one blocker).
        out[b - 1] = a;
        pairs++;
    }
    return pairs;
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
        return 1;
    vector<MTGCardInstance *> & attackers = req.attackers;
    vector<MTGCardInstance *> & blockers = req.blockers;
    vector<vector<MTGCardInstance *> > & legal = req.legalPerBlocker;

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
    {
        int incoming = 0;
        for (size_t j = 0; j < attackers.size(); j++)
            if (attackers[j]->power > 0)
                incoming += attackers[j]->power;
        tail << "Your life: " << life << ". Unblocked, these attackers deal up to "
             << incoming << " - you would be at " << (life - incoming)
             << (life - incoming <= 0
                 ? " - LETHAL if it all connects: block enough to survive."
                 : " - NOT lethal: block only where the trade favors you; taking damage while ahead is often correct.")
             << "\n";
    }
    tail << "Attackers:\n";
    for (size_t j = 0; j < attackers.size(); j++)
    {
        tail << "A" << (j + 1) << ". " << attackers[j]->name
             << " (" << attackers[j]->power << "/" << attackers[j]->toughness << ")";
        string kw = keywordList(attackers[j]);
        if (!kw.empty())
            tail << " [" << kw << "]";
        //Punisher rider: an attacker whose text does something WHEN BLOCKED
        //(sacrifice your blocker, damage you, pump itself) is a trap the
        //bare name hides - surface the text at the line that decides.
        string txt = attackers[j]->text;
        for (size_t ti = 0; ti < txt.size(); ti++)
            txt[ti] = (char) tolower((unsigned char) txt[ti]);
        if (txt.find("block") != string::npos)
            tail << " {text: " << cardTextSnippet(attackers[j], 160) << "}";
        tail << "\n";
    }
    tail << "Your available blockers (with the attackers each may legally block):\n";
    for (size_t i = 0; i < blockers.size(); i++)
    {
        tail << "B" << (i + 1) << ". " << blockers[i]->name
             << " (" << blockers[i]->power << "/" << blockers[i]->toughness << ")";
        string kw = keywordList(blockers[i]);
        if (!kw.empty())
            tail << " [" << kw << "]";
        tail << " - may block";
        for (size_t j = 0; j < legal[i].size(); j++)
            for (size_t k = 0; k < attackers.size(); k++)
                if (attackers[k] == legal[i][j])
                    tail << (j ? "," : "") << " A" << (k + 1);
        tail << "\n";
    }
    tail << "Assign each blocker to AT MOST ONE attacker (a creature cannot block"
            " two attackers), but several DIFFERENT blockers may gang-block the same"
            " attacker. Blockers you do not mention stay out of combat.\nReply with"
            " the assignments, comma-separated, e.g. \"B1:A2, B3:A1, B2:none\"."
            " Then your PLAN: line.";
    string userMsg = assemblePrompt(tail.str());

    string content;
    if (pollCompletion(userMsg, content) == kChoicePending)
        return 1; //decision in flight; nothing declared yet, re-poll next tick

    //Plan split BEFORE the assignment parse: a "B2" or bare numbers in the
    //plan prose must not read as block assignments.
    string decisionPart = consumePlan(content);
    vector<int> pick;
    int pairs = content.empty() ? 0 : parseBlockAssignments(decisionPart, blockers.size(), attackers.size(), pick);

    if (pairs == 0)
    {
        //Unusable reply: the heuristic declares this combat instead.
        writeTransLog("blockers", userMsg, content, pairs, (int) blockers.size(),
                      "", content.empty() ? "empty_reply" : "unparsed_reply");
        setNotice("model reply failed - the heuristic blocks", 5.0f);
        mBlocksDoneTurn = observer->turn;
        return AIPlayerBaka::chooseBlockers();
    }

    //Answer through the contract: the manager owns the defenser-cycling
    //choreography and re-validates each assignment.
    DecisionAction act;
    string declared;
    for (size_t i = 0; i < blockers.size(); i++)
    {
        if (pick[i] < 1)
            continue;
        MTGCardInstance * chosen = attackers[pick[i] - 1];
        if (!blockers[i]->canBlock(chosen))
            continue; //model assigned an illegal block: that blocker stays home
        act.blocks.push_back(std::make_pair(blockers[i], chosen));
        declared += (declared.empty() ? "" : "; ") + blockers[i]->name + " blocks " + chosen->name;
    }
    DecisionManager::applyDeclareBlockers(req, act);
    writeTransLog("blockers", userMsg, content, pairs, (int) blockers.size(),
                  declared.empty() ? string("no blockers") : declared, NULL);
    narrateDecision(declared.empty() ? string("You declared no blockers")
                                     : ("You declared blockers: " + declared));
    mBlocksDoneTurn = observer->turn;
    DebugTrace("AIPlayerGPT: declared blocks from " << pairs << " assignment(s) in one reply");
    return 1;
}

#endif //WITH_GPT_AI
