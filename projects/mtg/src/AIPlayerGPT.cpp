#include "PrecompiledHeader.h"

#ifdef WITH_GPT_AI

#include "AIPlayerGPT.h"
#include "LegalActions.h"
#include "GptPlanCaveat.h"
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
#include "ManaCostHybrid.h"
#include "ManaEngine.h"
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
#include <iostream>
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
    "Put your ANSWER on the VERY FIRST line, using exactly the label the decision asks for "
    "(CHOICE: for numbered choices, ATTACK: for attack declarations, BLOCKS: for block "
    "assignments), e.g. \"CHOICE: 2 (Cast Fatal Push)\". Answer first so a long reply can never "
    "lose it.\n"
    "After the answer line you may think the decision through briefly if you need to - that scratch "
    "text is discarded. If that thinking changes your mind, write a NEW answer line with the "
    "corrected answer; the LAST well-formed answer line is the one taken. If instead you realize "
    "your answer was a mistake and are unsure, it is fine to stop - the game's own reliable player "
    "will step in.\n"
    "Then, on the LAST line of your reply, a line starting with PLAN:, write your complete game "
    "plan from here on - CONCISE, a few sentences of intent, not an analysis.\n"
    "Nothing you write is kept except that PLAN line. At your next decision you will see only the "
    "game log, the current board, your last PLAN line, and the new choices - your reasoning and "
    "your earlier plans will have dropped out of context. So every PLAN must be complete and "
    "self-contained: state your full current plan, or your full revised plan if the situation "
    "changed. Never write a fragment like \"continue as before\".\n";

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
        //Tag-completeness for artifacts (battlefield lines): the "Artifacts
        //in play: you N" summary counts EVERY artifact, but the per-
        //permanent tags used to mark only non-creature non-land artifacts -
        //a pilot that re-derived the count from the lines got it wrong in
        //BOTH directions (untagged artifact creatures/lands under-counted,
        //Glimmervoid over-added; deck110 wave-9). Tag the cases typeTag
        //cannot: artifact creatures and artifact lands.
        if (withStatus && card->hasType(Subtypes::TYPE_ARTIFACT)
            && (card->isCreature() || card->isLand()))
            out << " [artifact]";
        if (withStatus)
        {
            //R-PAINLAND (wave-21 deck102): a mana source that damages its
            //controller when tapped for mana (Ancient Tomb 2, painlands 1)
            //shows nothing here otherwise - a bare land name - so the pilot
            //taps it blind and pays life it never priced. Name the cost at
            //the permanent. Neutral "its controller" phrasing stays correct
            //for either player's board.
            if (int selfDmg = ManaEngine::selfDamageOnTap(card))
                out << " [tapping for mana deals " << selfDmg << " damage to its controller]";
            //the LIVE keyword set - granted/lost abilities the decklist
            //text cannot show (Bloodghast "can't block", taught flying...)
            if (card->isCreature())
            {
                string kw = keywordList(card);
                if (!kw.empty())
                    out << " [" << kw << "]";
            }
            //Only open the bracket when a counter is actually present: a
            //list whose entries all reached 0 rendered an empty
            //"[counters:]" that reads like a glitch (deck44, waves 5-9).
            bool anyCounter = false;
            if (card->counters && card->counters->mCount)
                for (size_t c = 0; c < card->counters->counters.size() && !anyCounter; c++)
                    anyCounter = card->counters->counters[c] && card->counters->counters[c]->nb > 0;
            if (anyCounter)
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

//A line-leading coded answer label (CHOICE:/ATTACK:/BLOCKS:/PUT:) with SOME
//payload after it. Used only by the garbage detector: if the model emitted any
//such line, the normal parse + salvage can work it and we must NOT retry.
static bool hasCodedAnswerLine(const string& content)
{
    static const char * kLabels[] = { "choice:", "attack:", "blocks:", "put:" };
    string low = content;
    for (size_t i = 0; i < low.size(); i++)
        low[i] = (char) tolower((unsigned char) low[i]);
    size_t lineStart = 0;
    while (lineStart <= low.size())
    {
        size_t lineEnd = low.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? low.size() : lineEnd;
        size_t s = lineStart;
        while (s < end && (low[s] == ' ' || low[s] == '\t'
                           || low[s] == '*' || low[s] == '#' || low[s] == '-'
                           || low[s] == '>' || low[s] == '`'))
            s++;
        for (size_t k = 0; k < sizeof(kLabels) / sizeof(kLabels[0]); k++)
        {
            size_t len = strlen(kLabels[k]);
            if (end - s >= len && low.compare(s, len, kLabels[k]) == 0)
            {
                //require a non-space payload char after the label
                size_t p = s + len;
                while (p < end && (low[p] == ' ' || low[p] == '\t')) p++;
                if (p < end)
                    return true;
            }
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    return false;
}

bool AIPlayerGPT::isDecodeGarbage(const string& content)
{
    //Conservative floor: a decode collapse is always long (the 80-120s spirals
    //were 6.4-10.8k chars). Short/normal replies are never garbage-retried.
    const size_t kMinLen = 800;
    if (content.size() < kMinLen)
        return false;
    //A well-formed coded answer line means the parser/salvage can act on it -
    //not a collapse; never retry (this is what keeps ordinary unparsed replies
    //with a real answer line out of the retry path).
    if (hasCodedAnswerLine(content))
        return false;

    size_t total = content.size();
    size_t letters = 0, junk = 0;
    for (size_t i = 0; i < total; i++)
    {
        unsigned char c = (unsigned char) content[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            letters++;
        else if (c >= 0x80 || c == '*' || c == '#' || c == '`')
            junk++; //non-ASCII (incl. U+FFFD collapse bytes) + markup symbols
    }
    //Signal 1: near-zero prose. A real reply - even a truncated one - is mostly
    //English; a token collapse is punctuation/markup/garbage. <5% letters over a
    //long reply cannot be a genuine unparsed answer (deck27 s12/s13/s14 ~0%).
    bool lowProse = (letters * 100 < total * 5);
    //Signal 2: markup/non-ASCII density (s13's U+FFFD **, s14's ``` ### fences).
    bool markupHeavy = (junk * 100 >= total * 30);
    //Signal 3: a short line repeated many times (the literal decode loop). Find
    //the most frequent non-empty trimmed line; a collapse repeats ONE tiny line.
    bool repetition = false;
    {
        std::map<string, int> freq;
        size_t lineStart = 0, nonEmpty = 0, top = 0;
        while (lineStart <= content.size())
        {
            size_t lineEnd = content.find('\n', lineStart);
            size_t end = (lineEnd == string::npos) ? content.size() : lineEnd;
            size_t a = lineStart, b = end;
            while (a < b && (unsigned char) content[a] <= ' ') a++;
            while (b > a && (unsigned char) content[b - 1] <= ' ') b--;
            if (b > a && (b - a) <= 12) //only short lines feed the loop signature
            {
                string ln = content.substr(a, b - a);
                int c = ++freq[ln];
                if ((size_t) c > top) top = c;
            }
            if (b > a) nonEmpty++;
            if (lineEnd == string::npos)
                break;
            lineStart = lineEnd + 1;
        }
        repetition = (top >= 20 && top * 10 >= nonEmpty * 3); //>=20x and >=30% of lines
    }
    return lowProse || markupHeavy || repetition;
}

int AIPlayerGPT::pollCompletionRetry(const string& userMsg, string& content)
{
    //Mid-retry: poll the retry prompt (buildRequestBody sees mRetryActivePrompt
    //and uses the tight retry max_tokens). If the decision drifted, abandon the
    //pending retry and fall through to a fresh poll of the new decision.
    if (!mRetryActivePrompt.empty())
    {
        if (userMsg == mRetryBase)
        {
            int r = pollCompletion(mRetryActivePrompt, content);
            if (r == kChoicePending)
                return kChoicePending;
            //Retry finished: sum both attempts' latency into the record's field,
            //spend this decision's one retry, and hand back the retry reply
            //(possibly still unusable -> the caller's heuristic answers).
            if (mLastLatencyMs >= 0 && mRetryFirstLatencyMs >= 0)
                mLastLatencyMs += mRetryFirstLatencyMs;
            mRetryDoneBase = userMsg;
            mRetryActivePrompt.clear();
            mRetryBase.clear();
            mRetryFirstLatencyMs = -1;
            mLastRetry = true;
            return 0;
        }
        //Decision changed under a pending retry: drop it, poll the new decision.
        mRetryActivePrompt.clear();
        mRetryBase.clear();
        mRetryFirstLatencyMs = -1;
    }

    int r = pollCompletion(userMsg, content);
    if (r == kChoicePending)
        return kChoicePending;

    mLastRetry = false;
    //Fire ONE answer-locked retry iff the reply is decode-garbage AND this
    //decision's retry has not already been spent. isDecodeGarbage is
    //conservative - ordinary unparsed replies (real prose, no coded line) are
    //not garbage and fall straight through to the heuristic.
    if (!content.empty() && userMsg != mRetryDoneBase && isDecodeGarbage(content))
    {
        static const char * kAnswerLockPrefix =
            "IMPORTANT: your previous reply was corrupted (garbled, looping text with"
            " no answer line). Reply with ONLY the required coded answer line"
            " (CHOICE: / ATTACK: / BLOCKS:) and nothing else - no reasoning, no PLAN.\n\n";
        mRetryFirstLatencyMs = mLastLatencyMs;
        mRetryBase = userMsg;
        mRetryActivePrompt = string(kAnswerLockPrefix) + userMsg;
        setNotice("previous reply was corrupted - re-asking briefly", 3.0f);
        DebugTrace("AIPlayerGPT: decode-garbage reply; launching one answer-locked retry");
        return kChoicePending; //next tick polls the retry prompt
    }
    return 0;
}

AIPlayerGPT::AIPlayerGPT(GameObserver *observer, string deckFile, string deckfileSmall, string avatarFile, MTGDeck * deck)
    : AIPlayerBaka(observer, deckFile, deckfileSmall, avatarFile, deck), mAsyncState(std::make_shared<AsyncState>()), mThinkTime(0), mNoticeTicks(0), mBlocksDoneTurn(-1), mAttacksDoneTurn(-1), mPassDeclineTurn(-1), mStuckCastTurn(-1), mTransSeq(0), mLastLatencyMs(-1), mGameEndLogged(false), mGameStartLogged(false), mNarrationLogged(0), mNarratedTurnOwner(NULL), mNarratedTurnNumber(-1), mDealDone(false), mLastChoice(-1), mRetryFirstLatencyMs(-1), mLastRetry(false),
      mPregameBottomAsked(false), mPregameBottomForMulls(-1)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    //File config first, environment variables override.
    GptSettings cfg = GptSettings::load();
    mConfigUrls = cfg.urls;
    mConfigModel = cfg.model;
    mMaxTokens = cfg.maxTokens;
    mRepetitionPenalty = cfg.repetitionPenalty;
    if (const char * rp = getenv("WAGIC_GPT_REPPENALTY"))
        mRepetitionPenalty = atof(rp);
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
    //Finalize the filename with the OPPONENT deck token now that the
    //opponent exists (it does not at construction time): reviewers mapped
    //game->file by cross-referencing results.tsv epochs, fragile on
    //harvested copies (wave-9 ledger 5b). This runs before the first
    //append, so the file has not been created yet under the old name.
    if (opponent() && !opponent()->deckFileSmall.empty())
    {
        size_t dot = mTransLogPath.rfind(".jsonl");
        if (dot != string::npos && mTransLogPath.find("-vs-") == string::npos)
            mTransLogPath = mTransLogPath.substr(0, dot) + "-vs-" + opponent()->deckFileSmall + ".jsonl";
    }
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
                                const string& chosenText, const char * fallback, const vector<string> * optionTexts,
                                const char * choiceSource)
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
    //Answer-locked decode-garbage retry: mark the record and note the first
    //(garbage) attempt's latency separately; latency_ms above already carries the
    //SUMMED first+retry round trip (set in pollCompletionRetry).
    if (mLastRetry)
    {
        rec["retry"] = 1;
        mLastRetry = false;
    }
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
    //Provenance for an answer recovered by prose-intent salvage (no coded
    //line existed) - so corpus review can audit every prose salvage.
    if (choiceSource)
        rec["choice_source"] = choiceSource;
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
    {
        u << "\nYOUR PLAN (as you last stated it): " << mCurrentPlan << "\n";
        //Item-1: the carried plan preserves intent across decisions, but when
        //the state has advanced past it (a card it planned to cast is already
        //cast, the menu changed) it re-injects STALE intent - the model then
        //executes a leftover plan against the wrong option (a self-Galvanic-
        //Blast on its own just-cast Steel Overseer, deck110-vs-deck21). When
        //the plan's AFFIRMATIVELY named card-actions have ALL fallen off the
        //current options, append a caveat so the model re-derives. Light-touch
        //(not a drop) so a plan that legitimately looks ahead to a future
        //action is only nudged. Vocabulary = names in the caster's own zones.
        std::vector<string> myNames;
        MTGGameZone * zs[] = { game->library, game->hand, game->inPlay, game->graveyard };
        for (int z = 0; z < 4; z++)
            for (int i = 0; i < zs[z]->nb_cards; i++)
                myNames.push_back(zs[z]->cards[i]->getDisplayName());
        if (gptcaveat::planActionsStale(mCurrentPlan, tail, myNames))
            u << "(note: the actions your plan names are no longer among the options available "
                 "right now - the game state has advanced past that plan; re-derive your choice "
                 "from the current board and the options below.)\n";
    }
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

//Defined below (near salvageLoopedChoice): drops verbatim reply-template lines.
static bool isTemplatePlaceholderLine(const string& line);

//Find the LAST line-leading answer-label line in `text`, returning its
//remainder (after the label token) as [segStart, segEnd) plus the line's start
//offset. Template-placeholder lines are skipped. When `expectedLabel` is
//non-NULL, ONLY lines with that exact label count - the other answer labels are
//treated as chain-of-thought and ignored.
//
//The expectedLabel filter is stale-echo family B (wave-22). deck49 vs35 s18: a
//5.7k-char CHOICE deliberation contained the combat-math line "Attack: Regent
//(6/6) vs Striking Sliver (2/2, First Strike)..." AFTER its answer-first
//"CHOICE: 1 (...)". The unfiltered scan kept that later "Attack:" line as the
//"answer", handing parseChoice reasoning prose ("Regent (6/6) vs ... 2/2 ...")
//whose trailing 2/2 parsed to option 2, which then matched no echoed option and
//was dropped as stale_echo - discarding a valid, in-range CHOICE. A CHOICE ask
//can only be answered by a CHOICE: line; combat labels in its body are CoT.
static bool findAnswerLabelLine(const string& text, const char * expectedLabel,
                                size_t& segStart, size_t& segEnd, size_t& labelLineStart)
{
    static const char * kAnswerLabels[] = { "CHOICE:", "ATTACK:", "BLOCKS:", "PUT:" };
    const int kNumAnswerLabels = (int) (sizeof(kAnswerLabels) / sizeof(kAnswerLabels[0]));
    bool found = false;
    size_t lineStart = 0;
    while (lineStart <= text.size())
    {
        size_t lineEnd = text.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? text.size() : lineEnd;
        size_t s = lineStart;
        while (s < end && (text[s] == ' ' || text[s] == '\t' || text[s] == '*' || text[s] == '#'))
            s++; //tolerate markdown bullet/heading decoration on the label line
        for (int li = 0; li < kNumAnswerLabels; li++)
        {
            if (expectedLabel && strcmp(kAnswerLabels[li], expectedLabel) != 0)
                continue; //restricted to this decision's own answer label
            size_t len = strlen(kAnswerLabels[li]);
            if (end - s >= len)
            {
                bool match = true;
                for (size_t k = 0; k < len && match; k++)
                    match = (toupper((unsigned char) text[s + k]) == kAnswerLabels[li][k]);
                if (match && !isTemplatePlaceholderLine(text.substr(s, end - s)))
                {
                    segStart = s + len;
                    segEnd = end;
                    labelLineStart = lineStart;
                    found = true;
                }
            }
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    return found;
}

string AIPlayerGPT::consumePlan(const string& content, const char * expectedLabel)
{
    //Drop any inline think block first (same as parseChoice).
    string text = content;
    size_t thinkEnd = text.rfind("</think>");
    if (thinkEnd != string::npos)
        text = text.substr(thinkEnd + 8);

    //The reply contract puts the answer AFTER the plan, on a labeled final
    //line (CHOICE:/ATTACK:/BLOCKS:). Head-first answers committed the
    //choice token BEFORE the model reasoned in its PLAN; when the plan
    //concluded a DIFFERENT option for the same window, the stale head
    //stayed locked in (intent-collapse: 4 seats across waves 8-9, one
    //game-losing false Keep). The answer label anchors the parse so plan
    //prose full of numbers cannot hijack it - the reason head-first
    //existed. Find the LAST label at a line start (a plan that quotes the
    //protocol loses to the real trailing answer).
    //Last line-leading answer-label line (template placeholders skipped, as
    //in the deck62 N7-template fix). `expectedLabel` restricts the match to
    //this decision's own label so a CoT combat line ("Attack: ...") inside a
    //CHOICE deliberation is not mistaken for the answer (stale-echo family B).
    size_t answerStart = string::npos, answerEnd = 0, labelLineStart = string::npos;
    {
        size_t ss = 0, se = 0, ls = 0;
        if (findAnswerLabelLine(text, expectedLabel, ss, se, ls))
        {
            answerStart = ss;
            answerEnd = se;
            labelLineStart = ls;
        }
    }

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
    {
        //No plan stated: keep the previous one. The answer segment (when
        //labeled) is still the decision.
        if (answerStart != string::npos)
            return text.substr(answerStart, answerEnd - answerStart);
        //No label AND no plan: a compliant legacy reply is a short bare
        //answer; a multi-KB text here is a truncated formless ramble, and
        //scanning it for integers is how a quoted "option 1" cast the
        //spell the reasoning had just condemned (wave-10: two seats, one
        //self-Fatal-Push). Rambles parse as FAIL - the heuristic answers.
        if (text.size() > 300)
            return string();
        return text;
    }

    //The plan ends where a trailing answer label begins - the answer line
    //must not be re-fed as plan text at the next decision.
    size_t planEnd = (labelLineStart != string::npos && labelLineStart > pos)
                     ? labelLineStart : string::npos;
    string plan = (planEnd == string::npos) ? text.substr(pos + 5)
                                            : text.substr(pos + 5, planEnd - (pos + 5));
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
    //Labeled answer (the contract): the decision is the label line's
    //remainder, wherever the label sits relative to the plan. An answer
    //wrapped onto the next line ("CHOICE:\n2") leaves the label line
    //empty - extend to the rest of the reply rather than failing.
    if (answerStart != string::npos)
    {
        string ans = text.substr(answerStart, answerEnd - answerStart);
        bool hasContent = false;
        for (size_t k = 0; k < ans.size() && !hasContent; k++)
            hasContent = isalnum((unsigned char) ans[k]) != 0;
        if (!hasContent)
            ans = text.substr(answerStart);
        return ans;
    }
    //No label. A reply that LEADS with its plan followed the new contract
    //but lost its answer line (max_tokens truncation) - return nothing
    //parsable so the caller falls back, rather than letting plan prose
    //integers hijack the choice. A reply that leads with something else is
    //the legacy head-first shape: the head is the decision.
    size_t head = text.find_first_not_of(" \t\r\n");
    if (head != string::npos && pos == head)
        return string();
    //Same ramble guard as above: a legacy head is a short answer line, not
    //pages of prose ahead of a PLAN: marker.
    if (pos > 300)
        return string();
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
    //R-PAINLAND (wave-21 deck102): the count above hides that some of your
    //usable sources DAMAGE you when tapped for mana (Ancient Tomb: 2 damage;
    //painlands: 1 per colored tap) - and auto-tap, which taps sources for you
    //when you cast, can spend them. A pilot self-killed paying life through an
    //Ancient Tomb it never priced (died at -1). List the life the auto-tap can
    //cost so the pilot counts it before committing to a cast.
    vector<string> harmSources = ManaEngine::selfDamageManaSources(this);
    if (!harmSources.empty())
    {
        out << "CAUTION - some usable mana sources DAMAGE YOU when tapped for mana"
               " (auto-tap when you cast may spend them - count this life loss first): ";
        for (size_t i = 0; i < harmSources.size(); i++)
            out << (i ? "; " : "") << harmSources[i];
        out << "\n";
    }

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

//A modal-DFC "Flip Side" activation reaches the seat wrapped: the doubleside
//`{0}` cost makes it a GenericActivatedAbility (a NestedAbility) whose
//getMenuText delegates to the inner AATurnSide, so a direct dynamic_cast on
//the ranked ability misses it. Unwrap one NestedAbility layer.
static AATurnSide * asTurnSide(MTGAbility * a)
{
    if (!a)
        return NULL;
    if (AATurnSide * direct = dynamic_cast<AATurnSide *>(a))
        return direct;
    if (NestedAbility * na = dynamic_cast<NestedAbility *>(a))
        return dynamic_cast<AATurnSide *>(na->ability);
    return NULL;
}

string AIPlayerGPT::describeAction(const OrderedAIAction& action)
{
    std::ostringstream out;
    if (action.ability)
        out << action.ability->getMenuText();
    if (action.click)
        out << " with " << action.click->getDisplayName();

    //Kaldheim-style modal DFC in hand (R-DFC-FLIP, deck102 wave-22): the
    //engine surfaces a double-faced card ONLY as a repeatable "Flip Side"
    //pseudo-action that toggles which face the hand card presents - it casts
    //nothing and touches no stack. deck102's Tergrid, God of Fright //
    //Tergrid's Lantern was flipped 11x as a no-op with no idea what it did.
    //Name the other face + its cost and state plainly that the actual cast
    //happens via the Cast menu once the wanted face is showing. The current
    //face's cast already appears (with cost) in the Cast menu when affordable.
    if (AATurnSide * ats = asTurnSide(action.ability))
    {
        MTGCardInstance * fc = action.click ? action.click : ats->source;
        if (fc)
        {
            string otherName = (fc->isFlipped > 0) ? fc->nameOrig : ats->_SideName;
            if (!otherName.empty())
            {
                out << " -> DISPLAY TOGGLE only: switches this hand card to show"
                       " its other face \"" << otherName << "\"";
                MTGCard * oc = MTGCollection()->getCardByName(otherName, fc->setId);
                if (oc && oc->data && oc->data->getManaCost()
                    && oc->data->getManaCost()->getConvertedCost())
                    out << " (" << oc->data->getManaCost()->toString() << ")";
                out << ". It does NOT cast anything and uses no stack. You"
                       " usually do NOT need it: the Cast menu is where you cast,"
                       " and it lists every face you can afford (the other face"
                       " appears there as an alternative-cost cast). This only"
                       " changes which face is displayed.";
            }
        }
    }
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

    //Room for scratch reasoning + the complete PLAN + the trailing answer
    //line. The answer-last contract makes truncation COSTLY (a cut reply
    //loses the decision, not just plan tail): the first live game under
    //1024 lost 13 of 24 decisions to mid-reasoning cuts, and the first
    //full corpus under 2048 still lost 60/1369 (4.4%, median cut length
    //~6.9k chars - long combat math plus some repetition loops). Be
    //generous; the protocol text carries the brevity pressure, and the
    //truncation guard turns any residual cut into a safe heuristic answer.
    long maxTokens = 4096;
    if (mMaxTokens > 0)
        maxTokens = mMaxTokens;
    if (const char * mt = getenv("WAGIC_GPT_MAXTOKENS"))
        maxTokens = atol(mt);
    //Answer-locked retry: the re-ask needs only the coded line, so cap it tight
    //to fail fast to the heuristic instead of burning another long spiral. This
    //wins over any larger configured/env default for the retry request only.
    if (!mRetryActivePrompt.empty() && userMsg == mRetryActivePrompt)
        maxTokens = 512;

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

    //Repetition damping (decode-side guard against 12-16k-char spirals).
    //vLLM accepts repetition_penalty as a non-standard top-level field; the
    //official OpenAI API rejects unknown top-level params with a 400, so omit
    //it there (same reasoning as chat_template_kwargs). Inert at 1.0, so only
    //sent when explicitly configured != 1.0 (sampling changes play quality
    //and must be corpus-validated before it is ever defaulted on).
    if (mRepetitionPenalty != 1.0 && mEndpoint.find("api.openai.com") == string::npos)
        request["repetition_penalty"] = mRepetitionPenalty;

    return request.dump();
}

int AIPlayerGPT::parseChoice(const string& content, int optionCount,
                             const std::vector<string> * optionTexts,
                             bool * staleEcho,
                             const std::string * pendingSource)
{
    if (staleEcho) *staleEcho = false;
    //Drop any inline think block first.
    string text = content;
    size_t thinkEnd = text.rfind("</think>");
    if (thinkEnd != string::npos)
        text = text.substr(thinkEnd + 8);

    //Name-echo reconciliation: the answer line carries the chosen option's
    //name in parentheses ("CHOICE: 2 (Cast Fatal Push)"). When the plan
    //fixates on a card that is not among the options, the trailing index
    //mis-maps in BOTH directions - casting over a hold and holding over an
    //offered cast (waves 10-11, 2 seats, ~1.3-1.9% of decisions, one
    //wasted kill shot). The echo makes the intent checkable: an index
    //whose option does not contain the echoed words remaps to the UNIQUE
    //option that does (this also repairs out-of-range indices), and an
    //echo matching nothing parses as FAIL - the heuristic answers, never
    //an arbitrary cast.
    int echoRemap = -1;
    bool echoConflict = false;
    bool echoNoMatch = false;
    vector<string> words; //echo's significant words (hoisted for INDEX-WINS)
    if (optionTexts && !optionTexts->empty())
    {
        //the LAST "(...)" following a digit on the answer-ish tail
        size_t close = text.rfind(')');
        size_t open = (close == string::npos) ? string::npos : text.rfind('(', close);
        if (open != string::npos && close != string::npos && close > open + 1)
        {
            string echo = text.substr(open + 1, close - open - 1);
            //Target-menu spell-name prefix (stale-echo family A, wave-22).
            //Target sub-menus instruct "answer with the TARGET's name (not
            //'<spell>')", yet the model routinely echoes "<spell> targeting
            //<target>" (deck14 vs27 s29 "Unsummon targeting Inkfathom
            //Infiltrator"; deck62 vs14 s29 "Web targeting Yavimaya
            //Enchantress"). The spell prefix injected words absent from the
            //option AND the option's rules-text words were absent from the
            //echo, so BOTH echo passes missed and a CORRECT in-range index was
            //wrongly downgraded to stale_echo (the heuristic then bounced /
            //Web'd a worse target). When the parenthetical reads "<X>
            //targeting <Y>" and X is the pending target source, strip to <Y>
            //so the match runs on the target name. ANCHORED to the actual
            //source name - never a blanket "targeting" strip.
            if (pendingSource && !pendingSource->empty())
            {
                string el = echo;
                for (size_t i = 0; i < el.size(); i++)
                    el[i] = (char) tolower((unsigned char) el[i]);
                size_t tp = el.find(" targeting ");
                if (tp != string::npos)
                {
                    string x = el.substr(0, tp);
                    size_t xs = x.find_first_not_of(" \t");
                    size_t xe = x.find_last_not_of(" \t");
                    if (xs != string::npos)
                        x = x.substr(xs, xe - xs + 1);
                    string src = *pendingSource;
                    for (size_t i = 0; i < src.size(); i++)
                        src[i] = (char) tolower((unsigned char) src[i]);
                    if (!x.empty() && (src.find(x) != string::npos || x.find(src) != string::npos))
                        echo = echo.substr(tp + 11); //strlen(" targeting ") == 11
                }
            }
            //significant words: lowercase, length >= 4
            string w;
            for (size_t i = 0; i <= echo.size(); i++)
            {
                char c = (i < echo.size()) ? (char) tolower((unsigned char) echo[i]) : ' ';
                if (isalnum((unsigned char) c))
                    w += c;
                else
                {
                    //Decline/pass filler is NOT a card name: an echo of
                    //"(pass)" / "(none)" / "(hold)" / "(done)" carries no
                    //significant words, so it can never read as a stale
                    //absent-echo (task constraint ii). Real card names never
                    //reduce to these tokens.
                    if (w.size() >= 4 && w != "cast" && w != "with" && w != "play"
                        && w != "pass" && w != "none" && w != "hold" && w != "done"
                        && w != "skip" && w != "decline" && w != "nobody")
                        words.push_back(w);
                    w.clear();
                }
            }
            if (!words.empty())
            {
                int match = -1;
                for (size_t o = 0; o < optionTexts->size(); o++)
                {
                    string low = (*optionTexts)[o];
                    for (size_t i = 0; i < low.size(); i++)
                        low[i] = (char) tolower((unsigned char) low[i]);
                    bool all = true;
                    for (size_t k = 0; k < words.size() && all; k++)
                        all = low.find(words[k]) != string::npos;
                    if (all)
                    {
                        if (match >= 0) { match = -1; echoConflict = true; break; } //not unique
                        match = (int) o;
                    }
                }
                //Fallback (option-subset-of-echo): the all-echo-words-in-
                //option pass above misses a SUPERSTRING echo - the model
                //named the card in FULLER detail than the offered option
                //("Attack with Yawgmoth, Thran Physician" echoing the option
                //"Attack with Yawgmoth"; deck133 vs140 s37 was wrongly ruled
                //stale and downgraded to the heuristic). Reverse the
                //containment: an option whose OWN significant words all appear
                //in the echo is the intended pick (extra detail, not a
                //different card). Unique match only - zero or multiple keep
                //the no-match / conflict verdict, so a genuinely stale echo
                //(none of its words subset any option) still routes to the
                //heuristic.
                if (match < 0 && !echoConflict)
                {
                    string echoLow = echo;
                    for (size_t i = 0; i < echoLow.size(); i++)
                        echoLow[i] = (char) tolower((unsigned char) echoLow[i]);
                    for (size_t o = 0; o < optionTexts->size(); o++)
                    {
                        //significant words of THIS option (same rule as the
                        //echo split above: lowercase, length >= 4, minus the
                        //verb/decline filler)
                        vector<string> ow;
                        string t;
                        //E-49a (deck21 s4): a PLAYER-target option carries the
                        //volatile "(player, life N)" suffix (describeTarget:
                        //"The opponent (player, life 20)"). Its words "player"
                        //and "life" are absent from an echo that names the
                        //spell+target ("Cast Lightning Bolt targeting The
                        //opponent"), so the option-subset test failed and a
                        //CORRECT answer was wrongly downgraded to stale_echo.
                        //Strip that suffix before extracting the option's anchor
                        //words - life is not identifying. Scoped to this exact
                        //pattern; index-wins and the uniqueness guard are intact.
                        string src = (*optionTexts)[o];
                        {
                            size_t pl = src.find("(player, life ");
                            if (pl != string::npos)
                            {
                                size_t close = src.find(')', pl);
                                if (close != string::npos)
                                    src.erase(pl, close - pl + 1);
                            }
                        }
                        for (size_t k = 0; k <= src.size(); k++)
                        {
                            char c = (k < src.size()) ? (char) tolower((unsigned char) src[k]) : ' ';
                            if (isalnum((unsigned char) c))
                                t += c;
                            else
                            {
                                if (t.size() >= 4 && t != "cast" && t != "with" && t != "play"
                                    && t != "pass" && t != "none" && t != "hold" && t != "done"
                                    && t != "skip" && t != "decline" && t != "nobody")
                                    ow.push_back(t);
                                t.clear();
                            }
                        }
                        if (ow.empty())
                            continue; //no anchor words -> cannot subset-match
                        bool all = true;
                        for (size_t k = 0; k < ow.size() && all; k++)
                            all = echoLow.find(ow[k]) != string::npos;
                        if (all)
                        {
                            if (match >= 0) { match = -1; echoConflict = true; break; } //not unique
                            match = (int) o;
                        }
                    }
                }
                if (match >= 0)
                    echoRemap = match + 1; //1-based option number
                else if (!echoConflict)
                    echoNoMatch = true; //named words matched NO offered option
            }
        }
    }

    //INDEX-WINS (wave-23 ITEM A). A well-formed, in-range POSITIVE index is
    //only DISCARDED as a stale echo when the echoed name is genuinely
    //out-of-context: FOREIGN to the option the index points at AND not a
    //reference to the decision's own source card. A dual-face label ("Transform:
    //Search for Azcanta" vs option "Transform:azcanta, the sunken ruin"), an
    //invented verb ("Activate Tergrid's Lantern" vs "Flip Side with Tergrid's
    //Lantern"), or a parameter-menu prefix ("Cast <spell> with X=1" vs the bare
    //option "X = 1") still shares content with the chosen option (or names the
    //announced spell) and is TRUSTED at its index. Only a wholly stale answer -
    //a prior land drop echoed into a cast menu, whose index maps to a DIFFERENT
    //KIND of option (deck133 forbidden Thoughtseize, deck140 dumped kill shot) -
    //is dropped. This is the family-level root fix that replaces the per-shape
    //echo band-aids: the echo guard now needs the label to match NO current
    //option AND to be foreign to the CHOSEN option before it fires.
    auto echoStaleForIndex = [&](int k1) -> bool {
        if (!echoNoMatch || words.empty())
            return false;
        //(a) the echo shares a significant word with the option the index
        //selects -> the label is consistent with that option, trust the index.
        if (optionTexts && k1 >= 1 && k1 <= (int) optionTexts->size())
        {
            string low = (*optionTexts)[k1 - 1];
            for (size_t li = 0; li < low.size(); li++)
                low[li] = (char) tolower((unsigned char) low[li]);
            for (size_t wi = 0; wi < words.size(); wi++)
                if (low.find(words[wi]) != string::npos)
                    return false;
        }
        //(b) every echoed word names the decision's own source card (the spell
        //whose X/mode/parameter is being announced) -> a self-reference, not a
        //stale prior answer. Handles the ANNOUNCE_X "Cast <spell> with X=N" echo
        //whose bare "X = N" option carries no anchor words of its own.
        if (pendingSource && !pendingSource->empty())
        {
            string src = *pendingSource;
            for (size_t si = 0; si < src.size(); si++)
                src[si] = (char) tolower((unsigned char) src[si]);
            bool allInSrc = true;
            for (size_t wi = 0; wi < words.size(); wi++)
                if (src.find(words[wi]) == string::npos) { allInSrc = false; break; }
            if (allInSrc)
                return false;
        }
        return true; //label foreign to the chosen option and the source -> stale
    };

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
        {
            //A disagreeing unique name-echo outranks the index (the index
            //is the observed failure mode; the name is the intent). A
            //deliberate 0 (pass priority) is never remapped - it carries
            //no option name of its own.
            if (echoRemap > 0 && !echoConflict && n != 0 && echoRemap != n)
                return echoRemap;
            //Absent-echo staleness: the echo named significant words that
            //match NO offered option (a parent-action echo at a target
            //sub-menu, or a card cast earlier this turn after the option
            //list shifted). The raw index would execute an unintended
            //in-list card - game-losing twice this corpus (deck133 forbidden
            //Thoughtseize, deck140 dumped kill shot). Route to the heuristic
            //instead. A deliberate 0 (decline/pass) carries no card name and
            //is never treated as stale. A multi-option echo (echoConflict)
            //and a no-significant-word echo ("(pass)") keep index-wins. Under
            //INDEX-WINS the discard fires only when the label is ALSO foreign to
            //the option this index selects (echoStaleForIndex) - a consistent
            //label (dual-face / invented-verb / source self-reference) trusts n.
            if (echoNoMatch && n != 0 && echoStaleForIndex(n))
            {
                if (staleEcho) *staleEcho = true;
                return -1;
            }
            return n;
        }
        //out-of-range index, but the echo names a real option: repair
        if (echoRemap > 0 && !echoConflict)
            return echoRemap;
    }
    else if (echoRemap > 0 && !echoConflict)
        return echoRemap; //non-numeric head, but the echo names the intent
    //Real replies begin "CHOICE: N ...", so the head is non-numeric and the
    //index is resolved by the trailing scan below. Staleness is applied to
    //that resolved index (a positive index only - a resolved 0 is a
    //deliberate decline and is never stale), so the branch cannot preempt a
    //CHOICE: 0 before its index is known.
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
    //Absent-echo staleness on the trailing-scan index (the common path for
    //real "CHOICE: N (...)" replies). A resolved 0 is a deliberate decline
    //and is exempt; a positive index whose echo named no offered option AND is
    //foreign to that option (INDEX-WINS) is stale -> defer to the heuristic.
    if (echoNoMatch && choice > 0 && echoStaleForIndex(choice))
    {
        if (staleEcho) *staleEcho = true;
        return -1;
    }
    return choice;
}

//A verbatim reply-template line the model parroted instead of filling in:
//the system prompt's example "CHOICE: [Number] ([Name])" copied literally
//(deck62 wave-20 N7-template). Its "[Number]"/"[Name]" placeholders are not
//a real answer and must be dropped before disambiguation so the real
//CHOICE: line wins. Case-insensitive on the token.
static bool isTemplatePlaceholderLine(const string& line)
{
    string low = line;
    for (size_t i = 0; i < low.size(); i++)
        low[i] = (char) tolower((unsigned char) low[i]);
    return low.find("[number]") != string::npos || low.find("[name]") != string::npos;
}

int AIPlayerGPT::salvageLoopedChoice(const string& content, int optionCount,
                                     const std::vector<string> * optionTexts)
{
    //Decode-time repeat-loops spiral a phrase 60-100x and truncate; the
    //model usually states a tentative "CHOICE: N (name)" BEFORE the loop
    //(deck135 wave-18 HARNESS#2). Walk every line, keep the LAST one whose
    //CHOICE: label re-parses to a valid, offered option through the normal
    //parseChoice (so echo + staleness protection still apply - a stale line
    //re-parses to -1 and is skipped, never used). Literal template-placeholder
    //lines ("CHOICE: [Number] ([Name])") are dropped first (deck62 N7).
    int salvaged = -1;
    size_t lineStart = 0;
    while (lineStart <= content.size())
    {
        size_t lineEnd = content.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? content.size() : lineEnd;
        size_t s = lineStart;
        while (s < end && (content[s] == ' ' || content[s] == '\t'
                           || content[s] == '*' || content[s] == '#' || content[s] == '-'))
            s++; //tolerate markdown/list decoration before the label
        if (end - s >= 7)
        {
            static const char * kLabel = "CHOICE:";
            bool m = true;
            for (int k = 0; k < 7 && m; k++)
                m = (toupper((unsigned char) content[s + k]) == kLabel[k]);
            if (m)
            {
                string line = content.substr(s + 7, end - (s + 7));
                if (!isTemplatePlaceholderLine(line))
                {
                    bool st = false;
                    int c = parseChoice(line, optionCount, optionTexts, &st);
                    if (c >= 0)
                        salvaged = c; //keep the last well-formed, offered choice
                }
            }
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    return salvaged;
}

//True when the resolved (last line-leading) coded CHOICE is not the model's
//committed answer - route to the heuristic instead of executing the digit.
//
//REWORKED (wave-23 N9 residual, deck133 vs137/vs140 evidence). The wave-22
//detector fired on any tight self-correction PHRASE ("Correction:"/"made a
//mistake"/"Actually, no") that referenced the answer before PLAN:, and it
//OVER-FIRED when the model revised a SUB-POINT of its reasoning while the coded
//index stood unchanged (deck133 vs137 s29: a single sustained "CHOICE: 1", a
//"*Correction:*" about Bloodghast's haste, the index never moved - yet a bare
//number match on "1" in "2/1 attacker" tripped it). INDEX-WINS now trusts a
//sustained in-range coded index; a generic self-correction phrase alone, with a
//single sustained index and no contradictory coded sibling, NO LONGER fires.
//retracted_choice now fires on exactly the two shapes that are genuinely not the
//committed answer:
//  (a) a SECOND, CONTRADICTORY coded "CHOICE: N" appearing AFTER the resolved
//      answer (the last line-leading CHOICE, = salvageLoopedChoice's pick) and
//      naming a DIFFERENT in-range option - the model talked itself onto another
//      index in prose after answering (deck133 vs140 s9: line-1 "CHOICE: 2
//      (Decline)" then, buried mid-reasoning, "So CHOICE: 1"). A clean
//      line-leading RE-ANSWER is NOT this: salvage promotes the last line-leading
//      CHOICE to the resolved answer, so nothing contradictory sits after it -
//      the [B] retraction-then-new case still takes the new coded line.
//  (b) an explicit PAYABILITY/LEGALITY disavowal of the CHOSEN action - the model
//      says it cannot actually pay/cast/afford the option it coded (deck133 vs137
//      s26: the engine offered an unpayable "Sacrifice another creature" Yawgmoth
//      cost with no other creature; the model coded it, then proved it could not
//      pay). Routing that to the heuristic is always safe. The phrase set is
//      TIGHT and scoped to the pre-PLAN region AND to the chosen action (a
//      name-word window, or the sole option of a one-option ask), so a payability
//      aside about a DIFFERENT card (deck62 vs27 s10 "I cannot cast Fists ...",
//      which also sits after PLAN:) cannot fire it.
//A false fallback here only routes to the (safe) heuristic; a false take (a used
//retracted digit) is the harmful case we still catch.
bool AIPlayerGPT::choiceRetractedNoReplacement(const string& content, int optionCount,
                                               const std::vector<string> * optionTexts)
{
    string text = content;
    size_t thinkEnd = text.rfind("</think>");
    if (thinkEnd != string::npos)
        text = text.substr(thinkEnd + 8);

    //End offset AND parsed number of the LAST well-formed CHOICE line
    //(placeholders skipped, as in salvage - they never parse anyway).
    long lastChoiceEnd = -1;
    int chosenNum = -1;
    size_t planPos = string::npos; //first LINE-LEADING "PLAN:" marker
    size_t lineStart = 0;
    while (lineStart <= text.size())
    {
        size_t lineEnd = text.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? text.size() : lineEnd;
        size_t s = lineStart;
        while (s < end && (text[s] == ' ' || text[s] == '\t'
                           || text[s] == '*' || text[s] == '#' || text[s] == '-'))
            s++;
        if (end - s >= 7)
        {
            static const char * kLabel = "CHOICE:";
            bool m = true;
            for (int k = 0; k < 7 && m; k++)
                m = (toupper((unsigned char) text[s + k]) == kLabel[k]);
            if (m)
            {
                string line = text.substr(s + 7, end - (s + 7));
                if (!isTemplatePlaceholderLine(line))
                {
                    bool st = false;
                    int c = parseChoice(line, optionCount, optionTexts, &st);
                    if (c >= 0) { lastChoiceEnd = (long) end; chosenNum = c; }
                }
            }
        }
        //First line-leading PLAN: marker (the answer-first plan line). A
        //quoted mid-line "Your plan:" is NOT line-leading, so it is ignored -
        //the deck35 correct fire quotes the prompt's "Your plan:" at 5217 but
        //its real PLAN: line is at 7416.
        if (planPos == string::npos && end - s >= 5)
        {
            if (tolower((unsigned char) text[s]) == 'p' && tolower((unsigned char) text[s + 1]) == 'l'
                && tolower((unsigned char) text[s + 2]) == 'a' && tolower((unsigned char) text[s + 3]) == 'n'
                && text[s + 4] == ':')
                planPos = s;
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    if (lastChoiceEnd < 0)
        return false; //nothing well-formed to retract

    //Condition (a): the search region ends at the first line-leading PLAN:
    //marker (when it follows the answer). No PLAN: -> the whole tail. A PLAN:
    //BEFORE the answer (legacy plan-first shape) leaves no valid region and
    //cannot fire - the safe direction.
    size_t regionEnd;
    if (planPos == string::npos)
        regionEnd = text.size();
    else if (planPos > (size_t) lastChoiceEnd)
        regionEnd = planPos;
    else
        return false;

    //Significant name-words of the chosen option (same filter as parseChoice:
    //lowercase, length >= 4, minus the verb/decline filler) for condition (b)'s
    //answer-reference test.
    vector<string> chWords;
    if (optionTexts && chosenNum >= 1 && chosenNum <= (int) optionTexts->size())
    {
        const string& src = (*optionTexts)[chosenNum - 1];
        string t;
        for (size_t k = 0; k <= src.size(); k++)
        {
            char c = (k < src.size()) ? (char) tolower((unsigned char) src[k]) : ' ';
            if (isalnum((unsigned char) c))
                t += c;
            else
            {
                if (t.size() >= 4 && t != "cast" && t != "with" && t != "play"
                    && t != "pass" && t != "none" && t != "hold" && t != "done"
                    && t != "skip" && t != "decline" && t != "nobody")
                    chWords.push_back(t);
                t.clear();
            }
        }
    }

    string low = text;
    for (size_t i = 0; i < low.size(); i++)
        low[i] = (char) tolower((unsigned char) low[i]);

    //(a) A contradictory coded "CHOICE: N" AFTER the resolved answer's line.
    //lastChoiceEnd is the END offset of the LAST line-leading well-formed CHOICE
    //(salvage's resolved answer), so any "choice:" token found from there is
    //necessarily mid-line prose - the model re-stating a DIFFERENT index after it
    //already answered (deck133 vs140 s9: line-1 CHOICE: 2, then "So CHOICE: 1").
    //A clean line-leading recode ([B] retraction-then-new) resolves lastChoiceEnd
    //to the LATER line, so its earlier superseded index sits BEFORE the scan and
    //never fires. The parroted template "CHOICE: [Number]" is skipped.
    {
        size_t scan = (size_t) lastChoiceEnd;
        while ((scan = low.find("choice:", scan)) != string::npos)
        {
            size_t d = scan + 7;
            while (d < low.size() && (low[d] == ' ' || low[d] == '\t'))
                d++;
            if (d < low.size() && low[d] == '[') { scan += 7; continue; }
            if (d < low.size() && isdigit((unsigned char) low[d]))
            {
                size_t e = d;
                while (e < low.size() && isdigit((unsigned char) low[e]))
                    e++;
                int n = atoi(low.substr(d, e - d).c_str());
                if (n >= 0 && n <= optionCount && n != chosenNum)
                    return true; //genuine second, contradictory coded index
            }
            scan += 7;
        }
    }

    //(b) RETIRED (wave-24 corpus, 2026-07-24): the payability-disavowal branch
    //over-fired 131 times in ONE live corpus (11% fallback rate) - the phrase
    //set matched hypothetical rules deliberation ("if they have no creatures
    //they can't sacrifice") and the name-word reference gate passed on common
    //words like "opponent"/"mountain" that appear in any reasoning. Its single
    //justifying case (the lone unpayable Yawgmoth activation, deck133 vs137
    //s26) is now structurally impossible: unpayable sacrifice offers are gated
    //at the root by SacrificeCost::canPay candidate-existence (same wave-24
    //batch). Retraction now requires shape (a) alone - a genuine second,
    //contradictory coded index.
    return false;
}

//A land-fetch activation, by its rendered option line (the same string the
//de-dup and decline maps key on). These are the ability names the fetchland
//scripts render ("Put in Play with Misty Rainforest targeting...", "search
//basic land with Prismatic Vista") - 664 offers in the wave-9 corpus, the
//top decision-count driver on control decks.
static bool isFetchCrackLine(const string& line)
{
    return line.find("Put in Play with") != string::npos
        || line.find("search basic land with") != string::npos;
}

//The decline/consume key for a fetch line: the fetch's identity WITHOUT its
//" targeting <land>" tail. The proposed target flips as the board changes,
//which minted a fresh map key per window and let a chosen-but-unresolved
//crack re-ask three times (wave-10 deck44 s5-s7: one crack, three windows).
static string fetchLineKey(const string& line)
{
    size_t t = line.find(" targeting ");
    return (t == string::npos) ? line : line.substr(0, t);
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
        //The alternative-cost RULE on a HAND card (Force of Negation's
        //"exile a blue card") is a whole-spell cast, not a priority ability
        //- and it is already offered, with its stack target and 601.2c/cost
        //legality, by the CAST MENU (FindCardToPlay/legalCasts, viaAlternative).
        //Surfaced here as a standalone ordered action it is a DEAD END for
        //this seam: describeAction cannot express the spell's target, so a
        //single click only arms the extra payment (reactToClick returns 0,
        //nothing reaches the stack) and the action re-offers every window -
        //the model narrates "exile a blue card ..." forever while the spell
        //it should counter resolves (corpus 20260715 seq14+). Baka keeps its
        //own multi-tick follow-through (its chooseOrderedAction still sees
        //this entry), so pure-heuristic play is unchanged; only the GPT menu
        //drops the redundant dead end. Graveyard/exile/library alternative
        //casts (not offered by legalCasts' viaAlternative, hand-only) keep
        //their standalone entry.
        if (t == MTGAbility::ALTERNATIVE_COST && it->first.click
            && game->hand->hasCard(it->first.click))
            continue;
        //A normal cast-from-hand / land play (MTGPutInPlayRule, PUT_INTO_PLAY)
        //surfaced as a priority ordered action is a DEAD END for this seam,
        //exactly like the alternative-cost rule above: a single click only
        //arms it and nothing reaches the stack, so the option re-offers every
        //window. E6 (deck110 wave-17): a second Mox Opal held in hand rendered
        //"Cast Card Normally with Mox Opal {its own reminder text}" as the SOLE
        //priority option and looped as the seat's #1 fallback driver. The
        //dedicated cast menu (FindCardToPlay/buildCastSpell) and land menu
        //(legalLandPlays) already own every hand play, so drop the redundant
        //dead end here. Scoped to HAND cards so an exotic normal-cast from
        //library/graveyard/exile the cast menu may not enumerate is preserved.
        if (t == MTGAbility::PUT_INTO_PLAY && it->first.click
            && game->hand->hasCard(it->first.click))
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
        mFlipDoneCount.clear();
        mPassDeclineTurn = observer->turn;
    }
    for (size_t c = 0; c < candidates.size(); c++)
    {
        //Modal-DFC flip-thrash cap (see header): an in-hand "Flip Side"
        //toggle already taken 2x this turn stops being offered - it is a
        //no-op that mutates the presented face, so the no-progress deadlock
        //breaker below never catches it (deck102 flipped Tergrid 11x). Two
        //flips reach the wanted face and allow one flip back; the cast itself
        //rides the Cast menu on the showing face.
        if (AATurnSide * fats = asTurnSide(candidates[c]->ability))
        {
            MTGCardInstance * fcard = candidates[c]->click ? candidates[c]->click : fats->source;
            if (fcard && mFlipDoneCount[fcard] >= 2)
                continue;
        }
        string line = describeAction(*candidates[c]);
        if (!seenLines.insert(line).second)
            continue;
        //Pass-declined this turn: the model has said "hold" enough - stop
        //re-asking every window (the held-fetch tax; see header). A land
        //fetch gets ONE ask per turn (196 of 216 fetch windows in the
        //wave-9 corpus offered NOTHING else - a declined crack re-asked at
        //every window was the #1 control-deck decision driver); other lines
        //keep the two-decline allowance since their value genuinely moves
        //within a turn.
        bool fetchLine = isFetchCrackLine(line);
        std::map<string, int>::iterator dc = mPassDeclineCount.find(fetchLine ? fetchLineKey(line) : line);
        if (dc != mPassDeclineCount.end() && dc->second >= (fetchLine ? 1 : 2))
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
    tail << "\nWhich action do you take? On the FIRST line write CHOICE: followed by the number (0 = pass priority) and the chosen action's name in parentheses, e.g. \"CHOICE: 2 (Cast Fatal Push)\" or \"CHOICE: 0 (pass)\"; then any brief reasoning; then your PLAN: line last.";

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
        if (pollCompletionRetry(userMsg, content) == kChoicePending)
        {
            //Round trip in flight: no action this tick. The Act override
            //keeps the empty clickstream from being committed as a pass.
            return NULL;
        }
        //The plan is split off BEFORE choice parsing: plan prose is full of
        //numbers that would otherwise misparse as the chosen action. Restrict
        //the answer line to CHOICE: so a CoT "Attack:"/"Blocks:" line in the
        //reasoning body is not taken as the answer (stale-echo family B).
        string decisionPart = consumePlan(content, "CHOICE:");
        bool staleEcho = false;
        choice = parseChoice(decisionPart, index, &shownLines, &staleEcho);
        //Repeat-loop AND absent-card-bookend salvage (wave-23 ITEM A part 2).
        //salvageLoopedChoice walks EVERY CHOICE: line through the full validator
        //(echo + INDEX-WINS staleness still apply), keeping the last that
        //resolves clean. Running it on a stale primary too lets a hallucinated
        //CHOICE naming a card absent from the option set (which re-parses to -1
        //and is skipped) yield to a clean sibling line - deck140 vs102 s9, where
        //the real answer "CHOICE: 0 (pass)" bookended a middle "CHOICE: 1 (Cast
        //Black Sun's Zenith)" that named a sorcery not offered in the upkeep.
        //A lone genuine stale echo has no clean sibling, so it still routes to
        //the heuristic (salvage returns -1, choice unchanged).
        if (choice < 0 && !content.empty())
        {
            int sal = salvageLoopedChoice(content, index, &shownLines);
            if (sal >= 0)
            {
                DebugTrace("AIPlayerGPT: salvaged looped/bookend CHOICE " << sal << " of " << index);
                choice = sal;
            }
        }
        //Retraction gate: a parsed/salvaged choice that the model explicitly
        //took back (with no replacing CHOICE) is not its decision - defer to
        //the heuristic rather than execute the retracted digit.
        bool retracted = false;
        if (choice >= 0 && !content.empty()
            && choiceRetractedNoReplacement(content, index, &shownLines))
        {
            DebugTrace("AIPlayerGPT: CHOICE " << choice << " retracted with no replacement; deferring");
            choice = -1;
            retracted = true;
        }
        if (content.empty())
            setNotice("model reply failed or timed out - the heuristic decides", 5.0f);
        else if (choice >= 1 && choice <= index)
        {
            narrateDecision("You: " + describeAction(*shown[choice - 1]));
            //Consume-on-choose: a taken land fetch is done for the turn -
            //an identical line (a second copy of the same fetch, or the
            //same crack re-proposed at a different land) re-asking at the
            //next window is churn, not a decision (deck133's single-option
            //re-ask multiplier; deck44's target-flip re-ask). Next turn
            //re-offers.
            if (isFetchCrackLine(shownLines[choice - 1]))
                mPassDeclineCount[fetchLineKey(shownLines[choice - 1])] = 2;
            //Count a consumed DFC flip toward the per-turn thrash cap.
            if (AATurnSide * cats = asTurnSide(shown[choice - 1]->ability))
            {
                MTGCardInstance * ccard = shown[choice - 1]->click ? shown[choice - 1]->click : cats->source;
                if (ccard)
                    mFlipDoneCount[ccard]++;
            }
        }

        mLastAskKey = askKey;
        mLastChoice = choice;
        //A fresh deliberate pass declines every offered line (cached
        //replays of the same window don't re-count - one look, one vote).
        if (choice == 0)
            for (size_t s = 0; s < shownLines.size(); s++)
                mPassDeclineCount[isFetchCrackLine(shownLines[s])
                                  ? fetchLineKey(shownLines[s]) : shownLines[s]]++;
        {
            const char * fb = (choice >= 0) ? NULL : (content.empty() ? "empty_reply" : (retracted ? "retracted_choice" : (staleEcho ? "stale_echo" : "unparsed_reply")));
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

int AIPlayerGPT::askModel(const string& decision, const vector<string>& options, bool narrateChoice,
                          const string& pendingSourceName)
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
    tail << "\nOn the FIRST line write CHOICE: followed by the number of your choice and its name in parentheses, e.g. \"CHOICE: 2 (Cast Fatal Push)\"; then any brief reasoning; then your PLAN: line last.";
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
    if (pollCompletionRetry(userMsg, content) == kChoicePending)
        return kChoicePending; //callers unwind this tick and re-poll

    //Plan split BEFORE choice parsing: plan prose is full of numbers. Restrict
    //the answer line to CHOICE: so a CoT combat line ("Attack: ...") in the
    //reasoning body is not mistaken for the answer (stale-echo family B).
    string decisionPart = consumePlan(content, "CHOICE:");
    bool staleEcho = false;
    int choice = parseChoice(decisionPart, (int) options.size(), &options, &staleEcho,
                             pendingSourceName.empty() ? NULL : &pendingSourceName);
    //Repeat-loop AND absent-card-bookend salvage (see chooseOrderedAction):
    //walk every CHOICE: line through the validator, recovering a clean sibling
    //when the primary line was a decode spiral OR a stale absent-card echo. A
    //lone genuine stale echo has no clean sibling and still defers to heuristic.
    if (choice < 0 && !content.empty())
    {
        int sal = salvageLoopedChoice(content, (int) options.size(), &options);
        if (sal >= 0)
        {
            DebugTrace("AIPlayerGPT: salvaged looped/bookend CHOICE " << sal << " of " << options.size());
            choice = sal;
        }
    }
    //Retraction gate (see chooseOrderedAction): a choice the model explicitly
    //took back with no replacement is not its decision.
    bool retracted = false;
    if (choice >= 0 && !content.empty()
        && choiceRetractedNoReplacement(content, (int) options.size(), &options))
    {
        DebugTrace("AIPlayerGPT: ask CHOICE " << choice << " retracted with no replacement; deferring");
        choice = -1;
        retracted = true;
    }
    if (content.empty())
        setNotice("model reply failed or timed out - the heuristic decides", 5.0f);
    else if (narrateChoice && choice >= 1 && choice <= (int) options.size())
        //first line of the question only: multi-line asks (damage order)
        //would bloat a narration that persists all game
        narrateDecision(decision.substr(0, decision.find('\n')) + " -> " + options[choice - 1]);

    mAskCache[askKey] = choice;
    {
        bool valid = choice >= 1 && choice <= (int) options.size();
        const char * fb = valid ? NULL : (content.empty() ? "empty_reply" : (retracted ? "retracted_choice" : (staleEcho ? "stale_echo" : "unparsed_reply")));
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

//Hybrid-pip affordability clarifier for a cast option line. Cross-seat the
//model misreads hybrid mana pips and declines an OFFERED, payable cast - it
//reads {u/b} as needing BOTH colors, or a colored pip as generic (deck109
//{B/R}, deck17 {u/b} under-deployments, deck135 hallucinated {1}{g} decline,
//deck21 {R/G}). The option list is already authoritative; the sanctioned fix
//is REPRESENTATION - spell out, ONLY on costs that actually carry hybrid pips,
//that each pip pays with EITHER of its colors and how small the real total is.
//Non-hybrid costs get nothing (annotating every cost is noise, per the wave-15
//synthesis ruling).
static string hybridPipNote(ManaCost * c)
{
    if (!c)
        return "";
    vector<string> pips; //distinct color/color pips, first-seen order
    for (unsigned int i = 0; ; i++)
    {
        ManaCostHybrid * h = c->getHybridCost(i);
        if (!h)
            break;
        if (!h->color1 || !h->color2)
            continue; //only true color/color pips (what renders as {x/y})
        char lo1 = Constants::MTGColorChars[h->color1];
        char lo2 = Constants::MTGColorChars[h->color2];
        char hi1 = (char) toupper((unsigned char) lo1);
        char hi2 = (char) toupper((unsigned char) lo2);
        string desc = string("{") + lo1 + "/" + lo2 + "} pays with " + hi1 + " or " + hi2;
        bool seen = false;
        for (size_t k = 0; k < pips.size(); k++)
            if (pips[k] == desc) { seen = true; break; }
        if (!seen)
            pips.push_back(desc);
    }
    if (pips.empty())
        return "";
    std::ostringstream note;
    note << " [hybrid: ";
    if (pips.size() == 1)
        note << "each " << pips[0];
    else
        for (size_t k = 0; k < pips.size(); k++)
            note << (k ? "; " : "") << pips[k];
    note << " - total " << c->getConvertedCost() << " mana]";
    return note.str();
}

//Name the card(s) an EXTRA cost on an alternative cast will consume from
//hand. Force of Negation's free "exile a blue card" pitch auto-selected the
//caster's only blue card (its finisher, Abominable Treefolk) with nothing on
//the option line naming it, so the model committed to the counter believing
//it was pitching a different card (deck135 wave-18 #1d). Surface the actual
//pitch target; warn when it is the sole eligible card, and flag the >1 case
//(the engine currently auto-picks synchronously before the model can choose
//- documented as the deeper routing gap). Enumerates from the cost's own
//TargetChooser over the caster's hand (the pitch case; other zones no-op).
static string pitchCostNote(MTGCardInstance * card, ManaCost * altCost)
{
    if (!card || !altCost || !altCost->extraCosts)
        return "";
    Player * p = card->controller();
    if (!p || !p->game || !p->game->hand)
        return "";
    std::ostringstream note;
    ExtraCosts * aec = altCost->extraCosts;
    for (size_t i = 0; i < aec->costs.size(); i++)
    {
        ExtraCost * ec = aec->costs[i];
        if (!ec || !ec->tc)
            continue;
        ec->setSource(card);
        vector<MTGCardInstance *> cands;
        MTGGameZone * hand = p->game->hand;
        for (int z = 0; z < hand->nb_cards; z++)
        {
            MTGCardInstance * c = hand->cards[z];
            if (c == card)
                continue; //the "other" targeter already excludes the source
            if (ec->tc->canTarget(c))
                cands.push_back(c);
        }
        if (cands.empty())
            continue;
        if (cands.size() == 1)
            note << " (this exiles your ONLY eligible card, " << cands[0]->getDisplayName()
                 << " - you will lose it)";
        else
        {
            note << " (auto-exiles ONE of: ";
            for (size_t k = 0; k < cands.size() && k < 6; k++)
                note << (k ? ", " : "") << cands[k]->getDisplayName();
            if (cands.size() > 6)
                note << ", ...";
            note << " - the engine picks for you here)";
        }
    }
    return note.str();
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

    //Livelock breaker (see header): a consumed cast pick that left the
    //board byte-identical did not execute - suppress that option line for
    //the turn so the re-ask decides over the remaining entries instead of
    //replaying the cached pick every tick.
    if (mStuckCastTurn != observer->turn)
    {
        mStuckCastLines.clear();
        mStuckCastTurn = observer->turn;
    }
    string boardNow = serializeGameState();
    if (!mLastCastLine.empty())
    {
        if (boardNow == mLastCastBoard)
        {
            DebugTrace("AIPlayerGPT: cast pick made no progress, suppressing: " << mLastCastLine);
            mStuckCastLines.insert(mLastCastLine);
        }
        mLastCastLine.clear(); //progress or suppression: either way, consumed
    }
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
            o << hybridPipNote(cost);
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
            //Adventure alternative casts put an INSTANT/SORCERY spell onto the
            //stack, not the creature - the card's power/toughness belongs to the
            //creature face you may cast LATER from exile, so printing "(5/5)"
            //here read as "pay the adventure cost, get a 5/5 body now" and drove
            //a confirmed misfire (deck131 s15: cast Usher to Safety expecting a
            //creature, forced to bounce its own permanent). Suppress the P/T on
            //this face and append the ADVENTURE SPELL's effect text instead
            //(the creature-face option below still carries the P/T). Generalized
            //by the adventure mechanism, not by card name.
            bool isAdventureCast = card->has(Constants::ADVENTURE);
            if (card->isCreature() && !isAdventureCast)
                o << " (" << card->power << "/" << card->toughness << ")";
            o << casts[ci].zoneLabel;
            o << hybridPipNote(cost->getAlternative());
            //Name the card an exile/pitch extra cost will consume, so a free
            //alt-cast cannot silently eat the deck's finisher unseen (#1d).
            o << pitchCostNote(card, cost->getAlternative());
            if (isAdventureCast)
            {
                //The engine text= for an adventure is "<creature face> //
                //<adventure spell>" (or, when the creature is vanilla, just the
                //adventure spell with no separator - e.g. Shepherd of the Flock).
                //The adventure face is always the part after " // "; with no
                //separator the whole text IS the adventure effect.
                string advText = card->text;
                size_t sep = advText.find(" // ");
                if (sep != string::npos)
                    advText = advText.substr(sep + 4);
                for (size_t ti = 0; ti < advText.size(); ti++)
                    if (advText[ti] == '\n')
                        advText[ti] = ' ';
                if (advText.size() > 200)
                {
                    size_t cut = advText.rfind(' ', 200);
                    advText = advText.substr(0, (cut == string::npos || cut < 100) ? 200 : cut) + "...";
                }
                if (!advText.empty())
                    o << " {adventure spell: " << advText << "}";
            }
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
                //Own-targets-only warning: a mandatory-target removal
                //spell whose only legal targets are the CASTER's own
                //permanents is offered legally (601.2c satisfied - you may
                //destroy your own creature) but reads as a trap from a
                //bare cast line (deck44 wave-8: GFTT offered with only its
                //own Faerie legal; the pilot correctly declined but paid a
                //reasoning tax every window). Say it at the option. And
                //when legal targets DO exist, NAME them (capped): a bare
                //targeted cast line invited a fabricated "it has no legal
                //target" belief over a perfectly legal opponent creature
                //(deck44 wave-9 s54, a 900-char plan arguing itself out of
                //its removal). The deciding fact rides the option.
                if (tc->maxtargets == 1)
                {
                    int ownT = 0, oppT = 0;
                    std::ostringstream tNames;
                    int tShown = 0;
                    for (int pi = 0; pi < 2; pi++)
                    {
                        Player * pp = observer->players[pi];
                        MTGGameZone * zz[] = { pp->game->inPlay, pp->game->graveyard, pp->game->hand, pp->game->exile, pp->game->commandzone };
                        for (int zi = 0; zi < 5; zi++)
                            if (tc->targetsZone(zz[zi]))
                                for (int cj = 0; cj < zz[zi]->nb_cards; cj++)
                                    if (tc->canTarget(zz[zi]->cards[cj]))
                                    {
                                        (zz[zi]->cards[cj]->controller() == this ? ownT : oppT)++;
                                        if (tShown < 4)
                                            tNames << (tShown++ ? ", " : "") << zz[zi]->cards[cj]->getDisplayName();
                                    }
                        if (tc->canTarget(pp))
                        {
                            (pp == this ? ownT : oppT)++;
                            if (tShown < 4)
                                tNames << (tShown++ ? ", " : "") << (pp == this ? "you" : "the opponent");
                        }
                    }
                    if (ownT && !oppT && firstHit)
                    {
                        o << " - the only legal targets are YOUR OWN right now";
                        //Owner ruling (2026-07-16): a legal play is never
                        //hidden on a strategy judgment - strange cards make
                        //normally-nonsensical lines correct (heroic, cast
                        //triggers, death-trigger value, aura-count scaling).
                        //The classifier's verdict rides the option line as a
                        //warning; the model decides.
                        if (effectBadOrGood(card, MODE_TARGET, tc) == BAKA_EFFECT_BAD)
                            o << " (warning: this would harm your own side - only"
                                 " correct if you are deliberately triggering"
                                 " something)";
                    }
                    else if (ownT + oppT > 0)
                    {
                        o << " - legal targets right now: " << tNames.str();
                        if (ownT + oppT > tShown)
                            o << " (+" << (ownT + oppT - tShown) << " more)";
                        //Owner ruling (2026-07-16): same as the own-side case
                        //above - annotate, never hide. A beneficial cast with
                        //only opponent-side targets is usually futile (deck49
                        //War Paint loops) but not always (aura-count payoffs,
                        //cast triggers), so the warning carries the fact and
                        //the model decides.
                        if (oppT && !ownT && firstHit
                            && effectBadOrGood(card, MODE_TARGET, tc) == BAKA_EFFECT_GOOD)
                            o << " (warning: every legal target is the OPPONENT's -"
                                 " this would help their side unless you are"
                                 " deliberately triggering something)";
                    }
                    else if (firstHit)
                        o << " - NO legal target right now";
                    //else: the spell's targets live on the STACK - the
                    //stack line appended below names them; claiming "NO
                    //legal target" over an occupied stack contradicted the
                    //counterspell offers it decorated (wave-10, 2 seats).
                }
                //A hand-attack discard spell against a thin hand: the fact
                //that decides ("their hand is nearly/completely empty")
                //is surfaced in the board summary and STILL fabricated
                //over ("they may have a removal spell" at hand size 0,
                //deck133 wave-9 vs131 s58) - the last rung is the option
                //line itself.
                {
                    Player * oppP = this->opponent();
                    if (oppP && tc->canTarget(oppP))
                    {
                        string lowText = card->text;
                        for (size_t li = 0; li < lowText.size(); li++)
                            lowText[li] = (char) tolower((unsigned char) lowText[li]);
                        int oppHand = oppP->game->hand->nb_cards;
                        if (lowText.find("discard") != string::npos && oppHand <= 2)
                        {
                            if (oppHand == 0)
                                o << " - the opponent's hand is EMPTY: nothing to strip";
                            else
                                o << " - the opponent holds only " << oppHand << " card" << (oppHand > 1 ? "s" : "");
                        }
                    }
                }
                //Bounce-on-the-stack cast annotation (deck14 wave-20 E1): when
                //this instant is offered as a response while an opponent spell
                //resolves on the stack, but its chooser reaches only the
                //battlefield (not the stack), say so on the cast line - the
                //model repeatedly cast Unsummon/Boomerang believing it could
                //stop the spell being cast. firstHit stays true when no stack
                //card was targetable. Annotation only; the offer is unchanged.
                if (firstHit)
                {
                    bool oppSpellOnStack = false;
                    for (int pi = 0; pi < 2 && !oppSpellOnStack; pi++)
                    {
                        MTGGameZone * sz = observer->players[pi]->game->stack;
                        for (int zi = 0; zi < sz->nb_cards; zi++)
                        {
                            MTGCardInstance * sc = sz->cards[zi];
                            if (sc && sc != card && sc->controller() != this)
                            {
                                oppSpellOnStack = true;
                                break;
                            }
                        }
                    }
                    bool hitsBattlefield =
                        tc->targetsZone(observer->players[0]->game->inPlay) ||
                        tc->targetsZone(observer->players[1]->game->inPlay);
                    if (oppSpellOnStack && hitsBattlefield)
                        o << " [this cannot target the spell on the stack -"
                             " battlefield permanents only]";
                }
                SAFE_DELETE(tc);
                if (!firstHit)
                    o << " - can target on the stack: " << hits.str();
            }
        }
        if (mStuckCastLines.count(o.str()))
            continue; //this exact entry no-op'd this turn; do not re-offer
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
        mLastCastBoard = boardNow; //livelock breaker: next entry compares
        mLastCastLine = opts[pick];
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

    //The mulligan is now a PRE-GAME PHASE (PreGamePhase, before turn 1), not an
    //in-game window here: the old turn-0/turn-1 mulligan hack was removed with
    //the London pre-game phase. This player's opening-hand decisions arrive
    //through the pregameMulliganDecision / pregameChooseBottom / pregameLeyline
    //hooks below, driven by PreGamePhase while GameObserver::Update is gated.
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

//The ANNOUNCE_X ask header. States the mana CAP and its reason so the model
//does not reason about an X its mana cannot reach (wave-23 deck140 vs27 s46:
//the model computed X=20 "lethal" while the engine offered at most X=6). capX =
//the highest AFFORDABLE X = the last option's X value (option index == X value,
//bounded upstream by ManaEngine::maxAnnounceableX). Factored out so the PARSETEST
//covers the exact production string.
static string announceXHeader(const string& spell, int capX)
{
    std::ostringstream xa;
    xa << "Announce the value of X for " << spell
       << ". You can afford X up to " << capX << " with your current mana"
       << " - higher values are NOT offered (they are unaffordable), so do not"
       << " plan around an X above " << capX << ". Every listed value is"
       << " affordable; option 1 is the LARGEST X (X = " << capX << ")."
       << " Reply with the OPTION number, not the X value:";
    return xa.str();
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
        //State the CAP and its reason (wave-23 ITEM, deck140 vs27 s46). The
        //offered X values top out at the highest AFFORDABLE X (mana-limited);
        //option index == X value, so the cap is the last option's X = the option
        //count minus one. Without this the model reasons about an X its mana
        //cannot reach (s46: computed X=20 as "lethal" while the engine offered
        //at most X=6) - a belief-vs-menu mismatch that costs a long reply and can
        //mis-place a kill. Cheap, truthful, no option changes: the cap already
        //bounds the option set built upstream (ManaEngine::maxAnnounceableX).
        int capX = (int) req.optionTexts.size() - 1;
        //Pass the spell being announced as the pending source: the model echoes
        //"Cast <spell> with X=N" against the bare "X = N" option, and INDEX-WINS
        //treats an echo that names the source spell as a self-reference rather
        //than a stale answer (wave-23 ITEM A shape 1, deck140 Black Sun's Zenith).
        int pick = askModel(announceXHeader(ctx ? ctx->getDisplayName() : string("this spell"), capX),
                            shown, true, ctx ? ctx->getDisplayName() : string());
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
    //ENGINE-R6 (wave-21 deck135): a Transform/flip menu option is only ever
    //offered when its gating condition is already satisfied - the engine arms
    //the flip only when it is legal (Search for Azcanta's "7+ cards in
    //graveyard" upkeep trigger only presents the transform once the threshold
    //is met). Without that fact riding the option the model fills the vacuum
    //with an error-prone manual recount and declines an available transform
    //(vs14 seq 29/40: recounted its graveyard, got 5-6, Declined a legal flip
    //twice, losing the grind). The deciding fact rides the option. Annotation
    //only - the option stays legal and pickable; req.optionTexts (the
    //staleness key) is untouched, and only trailing text is appended so the
    //answer index is unchanged.
    for (size_t i = 0; i < opts.size(); i++)
        if (opts[i].compare(0, 10, "Transform:") == 0)
            opts[i] += " [available NOW - this transform is only offered because its"
                       " condition is already met; do not recount, it is legal this instant]";
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

    //Bounce-on-the-stack misconception (deck14 wave-20 E1): the model casts
    //Unsummon/Boomerang to "answer" a spell the opponent is CASTING, then at
    //this forced target menu - whose legal set is battlefield permanents only -
    //self-bounces or thrashes hunting for the on-stack spell that is not here.
    //When (a) the chooser cannot reach the stack but targets the battlefield,
    //and (b) an opponent spell is actually resolving on the stack, prepend a
    //one-line header naming that trap. Conditioned so it only appears when the
    //confusion can occur; representation only, the offered set is unchanged.
    bool stackTrapNote = false;
    {
        MTGGameZone * stk0 = observer->players[0]->game->stack;
        MTGGameZone * stk1 = observer->players[1]->game->stack;
        bool chooserHitsStack = tc->targetsZone(stk0) || tc->targetsZone(stk1);
        bool chooserHitsBattlefield =
            tc->targetsZone(observer->players[0]->game->inPlay) ||
            tc->targetsZone(observer->players[1]->game->inPlay);
        if (!chooserHitsStack && chooserHitsBattlefield)
        {
            for (int pi = 0; pi < 2 && !stackTrapNote; pi++)
            {
                MTGGameZone * sz = observer->players[pi]->game->stack;
                for (int zi = 0; zi < sz->nb_cards; zi++)
                {
                    MTGCardInstance * sc = sz->cards[zi];
                    //An opponent-controlled object on the stack (not the bounce
                    //spell itself) is what the model mistakes for a target.
                    if (sc && sc != tc->source && sc->controller() != this)
                    {
                        stackTrapNote = true;
                        break;
                    }
                }
            }
        }
    }

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

        //Forced self-loss inversion (deck140/deck102 wave-22, LOSS-CAUSING):
        //when an OPPONENT'S effect forces THIS seat to discard/sacrifice/exile
        //one of its OWN cards, the chooser is a TargetChooser whose legal set
        //is entirely the deciding player's own cards. The generic "TARGET
        //CHOICE ... pick the ONE target it will affect" wording is built for
        //choosing what a spell AFFECTS (an enemy permanent to remove -> pick
        //the MOST valuable) and inverts catastrophically here: you are picking
        //which of your OWN cards to LOSE, so the correct choice is the LEAST
        //valuable. deck140 pitched Damnation then Pyroclasm - its two live
        //sweepers - into an Archon-of-Cruelty forced discard and died T12.
        //Detect the shape (every candidate is my own card AND the effect verb
        //is a loss) and render inverted framing. Representation only - the
        //offered set and the apply path are unchanged.
        bool forcedSelfLoss = false;
        string lossVerb;
        {
            string en = effectName;
            for (size_t i = 0; i < en.size(); i++) en[i] = (char) tolower((unsigned char) en[i]);
            if (en.find("discard") != string::npos) lossVerb = "discard";
            else if (en.find("sacrifice") != string::npos) lossVerb = "sacrifice";
            else if (en.find("exile") != string::npos) lossVerb = "exile";
            if (!lossVerb.empty() && !targets.empty())
            {
                bool allMine = true;
                for (size_t i = 0; i < targets.size(); i++)
                {
                    //the "Done" escape is a string option, never in targets.
                    MTGCardInstance * mc = dynamic_cast<MTGCardInstance *>(targets[i]);
                    if (!mc || mc->controller() != this) { allMine = false; break; }
                }
                //The source is the opponent's forcing effect (Baka fallback is
                //fine either way, but self-costs of my OWN spells are also a
                //"pick least valuable" so the source test only strengthens the
                //wording, it does not gate the fix).
                forcedSelfLoss = allMine;
            }
        }

        //Target sub-menus reached the model as a bare "Choose ... for X" line
        //that read like a phase or cast decision, owning most of the corpus's
        //fallbacks: the pilot answered with an attack PLAN ("Choose the target
        //for Web" -> out-of-range CHOICE: 0, deck62 N1), echoed the SPELL name
        //as its pick instead of the target (deck49 E-49b, all 7 stale_echoes),
        //or invented "Cast nothing" on a mandatory chooser (deck135 s4). Frame
        //the step explicitly: name the pending spell/ability, state it is a
        //TARGET pick for something already on the stack (not a cast/phase step),
        //and tell the model to answer with the TARGET's name. Representation
        //only - the offered set and the apply path are unchanged.
        std::ostringstream q;
        if (forcedSelfLoss)
        {
            q << "FORCED " << lossVerb << " OF YOUR OWN CARD"
              << ((multi && tc->maxtargets != 1) ? "S" : "")
              << ": the opponent's effect (" << effectName << ") forces YOU to "
              << lossVerb << " one of your OWN cards from the list below - each"
                 " option is a card YOU will LOSE, not something you affect or"
                 " attack. Pick the card you can best AFFORD TO LOSE (usually your"
                 " LEAST valuable: pitch a spare land or a redundant/dead card,"
                 " and KEEP your best spells, answers, and threats). ";
            if (!multi)
                q << "Choose the ONE card to give up";
            else
            {
                q << "Choose card " << (picks.size() + 1);
                if (!unlimited)
                    q << " of " << (tc->targetMin ? "exactly " : "up to ") << tc->maxtargets;
            }
            q << " from the list below, and answer with the chosen card's name.";
        }
        else
        {
        if (stackTrapNote)
            q << "NOTE: these targets are battlefield permanents only - the spell"
                 " being cast on the stack is NOT a legal target and is NOT in this"
                 " list. If your only goal was to stop that spell, this cannot do"
                 " it; pick a battlefield permanent that is worth bouncing, or"
                 " decline.\n";
        q << "TARGET CHOICE for " << effectName
          << " (this spell/ability is already on the stack and needs a target - "
          << "it is NOT a cast or phase step). Pick ";
        if (!multi)
            q << "the ONE target it will affect";
        else
        {
            q << "target " << (picks.size() + 1);
            if (!unlimited)
                q << " of " << (tc->targetMin ? "exactly " : "up to ") << tc->maxtargets;
        }
        q << " from the list below, and answer with the chosen TARGET's name (not \""
          << effectName << "\")";
        }

        //Pass the pending source name so parseChoice can strip a "<spell>
        //targeting <target>" echo prefix down to the target name (stale-echo
        //family A). effectName is the exact spell/ability named in the prompt.
        int pick = askModel(q.str(), opts, true, effectName);
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

//Significant name-words of a reply SEGMENT (lowercase, len>=4, minus the
//combat/protocol filler) - the tokens that select a named creature. Shared
//by the block name->label reconciler; mirrors parseAttackerSet's inline
//split and parseChoice's echo split.
static void significantWords(const string& seg, vector<string>& words)
{
    string w;
    for (size_t k = 0; k <= seg.size(); k++)
    {
        char c = (k < seg.size()) ? (char) tolower((unsigned char) seg[k]) : ' ';
        if (isalnum((unsigned char) c))
            w += c;
        else
        {
            if (w.size() >= 4 && w != "attack" && w != "with" && w != "cast"
                && w != "play" && w != "none" && w != "hold" && w != "pass"
                && w != "nobody" && w != "block" && w != "blocks")
                words.push_back(w);
            w.clear();
        }
    }
}

//A trailing "#N" disambiguation ordinal on a reply segment: the model marks
//WHICH of several identically-named creatures it means ("Saproling (1/1) #1";
//deck135 wave-19 s27). Returns N (>=1) after the LAST '#', or 0 if none. The
//significant-word split already discards the '#N' and '(P/T)' as sub-4-char
//tokens, so the name match is unaffected; this recovers the ordinal the split
//throws away, to break a same-name tie the parser would otherwise drop.
static int nameOrdinal(const string& seg)
{
    size_t h = seg.rfind('#');
    if (h == string::npos)
        return 0;
    size_t k = h + 1;
    while (k < seg.size() && (seg[k] == ' ' || seg[k] == '\t'))
        k++;
    int n = 0;
    bool any = false;
    while (k < seg.size() && isdigit((unsigned char) seg[k]))
    {
        n = n * 10 + (seg[k++] - '0');
        any = true;
    }
    return any ? n : 0;
}

//The listed name whose lowercased form contains ALL the given words, optionally
//restricted to a set of allowed indices. With a unique match, returns it.
//AMBIGUITY (duplicate creature names) normally drops the assignment (-1, the
//gang-block disambiguation guard) - EXCEPT when the reply carried a "#N"
//ordinal (ordinal>0): among the same-named matches, in label order, the N-th
//is the model's intent, so it is selected instead of dropped (identically
//named creatures blocking are interchangeable, so honoring the pick the model
//made is faithful, never a guess). Zero matches always -> -1.
static int uniqueNameMatch(const vector<string>& words, const vector<string>& names,
                           size_t limit, const vector<int> * allowed = NULL,
                           int ordinal = 0)
{
    if (words.empty())
        return -1;
    vector<int> matches;
    for (size_t o = 0; o < names.size() && o < limit; o++)
    {
        if (allowed)
        {
            bool ok = false;
            for (size_t a = 0; a < allowed->size(); a++)
                if ((*allowed)[a] == (int) o) { ok = true; break; }
            if (!ok)
                continue;
        }
        string low = names[o];
        for (size_t k = 0; k < low.size(); k++)
            low[k] = (char) tolower((unsigned char) low[k]);
        bool all = true;
        for (size_t k = 0; k < words.size() && all; k++)
            all = low.find(words[k]) != string::npos;
        if (all)
            matches.push_back((int) o);
    }
    if (matches.size() == 1)
        return matches[0];
    if (matches.size() > 1 && ordinal >= 1 && ordinal <= (int) matches.size())
        return matches[ordinal - 1]; //"#N" disambiguates equivalent duplicates
    return -1; //zero matches, or ambiguous with no ordinal to break the tie
}

//A terse, NAIVE single-block combat-trade preview: what happens if this ONE
//blocker blocks this ONE attacker, alone. The model keeps re-deriving
//first-strike / deathtouch / trample math it distrusts (deck35 wave-18: an
//11k-char loop re-computed a rendered P/T until it emitted an unparseable
//reply); handing it the outcome removes the surface it re-derives. Ignores
//gang-blocks, pump, damage prevention and regeneration - the section header
//hedges "before other blockers/tricks". Perspective: 'you' = the AI (the
//blocking side). Empty string when there is nothing decisive to say.
//Plain-stat combatant view for the trade preview, so the outcome logic is
//unit-testable without an engine-built MTGCardInstance (see runParseSelfTest).
//'wither' is set for wither OR infect (both deal -1/-1-counter damage);
//'infectLabel' only picks the word for the message.
struct CombatTradeStat
{
    int power;
    int toughness;
    bool deathtouch;
    bool wither;       //wither or infect: damage as -1/-1 counters
    bool infectLabel;  //message word: true=infect, false=wither
    bool firststrike;  //first strike or double strike
    bool indestructible;
    bool trample;
};

//The pure trade-outcome logic. Perspective: 'b' = the blocking side ('you'),
//'a' = the attacker. Empty-ish returns handled by the caller.
static string combatTradePreviewStats(const CombatTradeStat& b, const CombatTradeStat& a)
{
    int bp = b.power > 0 ? b.power : 0;
    int ap = a.power > 0 ? a.power : 0;
    int bt = b.toughness;
    int at = a.toughness;

    //Base lethality (before first-strike ordering): does X's damage kill Y?
    //Deathtouch makes ANY damage lethal; normal damage is lethal at
    //power>=toughness. Both DESTROY, which indestructible prevents. Wither/
    //infect do NOT change the lethality threshold (still power>=toughness) -
    //they only change the damage FORM to -1/-1 counters, which shrink a
    //survivor and, at a lethal hit, drop toughness to 0 for a state-based
    //death that indestructible does NOT prevent. Conflating wither with
    //deathtouch made a wither blocker read as auto-lethal ("both die") even
    //when its power could not kill (deck27 wave-20 item 1: Oona's Gatewarden
    //2/1 wither vs a 3/4 -> the attacker survives as a 1/2, it does NOT trade).
    bool aKillsB = (ap > 0) && ((a.wither && ap >= bt) || (!b.indestructible && (a.deathtouch || ap >= bt)));
    bool bKillsA = (bp > 0) && ((b.wither && bp >= at) || (!a.indestructible && (b.deathtouch || bp >= at)));
    //First strike / double strike ordering: a one-sided first striker that
    //kills its foe removes that foe before it can deal (the survivor's later
    //normal-step damage lands on a dead creature).
    if (a.firststrike && !b.firststrike && aKillsB)
        bKillsA = false;
    else if (b.firststrike && !a.firststrike && bKillsA)
        aKillsB = false;

    std::ostringstream o;
    if (aKillsB && bKillsA)
        o << "both die";
    else if (bKillsA && !aKillsB)
        o << "you kill it, your blocker lives";
    else if (aKillsB && !bKillsA)
        o << "your blocker dies, attacker lives";
    else
        o << "neither dies";
    //Trample-through to your face (attacker assigns lethal to the blocker,
    //rest carries over) - only when the attacker actually deals (not killed
    //first by a one-sided first-strike blocker). Wither trample still assigns
    //full toughness as lethal (only deathtouch reduces the lethal cut to 1).
    if (a.trample && !(b.firststrike && !a.firststrike && bKillsA))
    {
        int lethalToB = a.deathtouch ? 1 : (bt > 0 ? bt : 0);
        int through = ap - lethalToB;
        if (through > 0)
            o << ", " << through << " tramples to your face";
    }
    //Wither/infect that does NOT kill still SHRINKS the survivor by its damage
    //(-1/-1 counters), so the model does not read the survivor as untouched -
    //or, pre-fix, as dead. Only when the target actually survives (!kills).
    if (b.wither && bp > 0 && !bKillsA)
    {
        int np = ap - bp; if (np < 0) np = 0;
        int nt = at - bp; //survives => bp < at, so nt >= 1
        o << " (" << (b.infectLabel ? "infect" : "wither")
          << " shrinks it to " << np << "/" << nt << ")";
    }
    if (a.wither && ap > 0 && !aKillsB)
    {
        int np = bp - ap; if (np < 0) np = 0;
        int nt = bt - ap; //survives => ap < bt, so nt >= 1
        o << " (" << (a.infectLabel ? "infect" : "wither")
          << " shrinks your blocker to " << np << "/" << nt << ")";
    }
    return o.str();
}

static CombatTradeStat combatStatOf(MTGCardInstance * c)
{
    CombatTradeStat s;
    s.power = c->power;
    s.toughness = c->toughness;
    s.deathtouch = c->basicAbilities[Constants::DEATHTOUCH];
    s.wither = c->basicAbilities[Constants::WITHER] || c->basicAbilities[Constants::INFECT];
    s.infectLabel = c->basicAbilities[Constants::INFECT] && !c->basicAbilities[Constants::WITHER];
    s.firststrike = c->basicAbilities[Constants::FIRSTSTRIKE] || c->basicAbilities[Constants::DOUBLESTRIKE];
    s.indestructible = c->basicAbilities[Constants::INDESTRUCTIBLE];
    s.trample = c->basicAbilities[Constants::TRAMPLE];
    return s;
}

static string combatBlockOutcome(MTGCardInstance * blocker, MTGCardInstance * attacker)
{
    return combatTradePreviewStats(combatStatOf(blocker), combatStatOf(attacker));
}

//Forward declarations: the salvage helpers below re-parse a labeled line
//through the same validators the primary path uses (defined further down).
static int parseAttackerSet(const string& content, size_t nAttackers, vector<bool>& out,
                            const vector<string> * optionNames);
static int parseBlockAssignments(const string& content, size_t nBlockers, size_t nAttackers, vector<int>& out,
                                 const vector<string> * blockerNames, const vector<string> * attackerNames,
                                 const vector<vector<int> > * legalPerBlocker);

//Every line of a reply whose FIRST token (after markdown decoration) is the
//given answer label, as the text AFTER the label to end of line, in reply
//order. The salvage path re-parses these through the full validation: a
//decode-time repeat-loop often states a well-formed BLOCKS:/ATTACK:/PUT: line
//before spiraling (the same premise as salvageLoopedChoice), so the last one
//that validates is recovered rather than lost to the heuristic.
static void collectLabeledLines(const string& content, const char * label, vector<string>& out)
{
    size_t labelLen = strlen(label);
    size_t lineStart = 0;
    while (lineStart <= content.size())
    {
        size_t lineEnd = content.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? content.size() : lineEnd;
        size_t s = lineStart;
        while (s < end && (content[s] == ' ' || content[s] == '\t'
                           || content[s] == '*' || content[s] == '#' || content[s] == '-'))
            s++;
        if (end - s >= labelLen)
        {
            bool m = true;
            for (size_t k = 0; k < labelLen && m; k++)
                m = (toupper((unsigned char) content[s + k]) == toupper((unsigned char) label[k]));
            if (m)
                out.push_back(content.substr(s + labelLen, end - (s + labelLen)));
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
}

//Salvage a bundled BLOCK reply from a decode spiral: the LAST BLOCKS: line that
//re-parses to >=1 well-formed pairing through the full validation path (range +
//name reconcile + legal-per-blocker). Fills 'out' and returns the pair count;
//0 = nothing salvageable (caller keeps its heuristic fallback). Never bypasses
//validation - a hallucinated/out-of-range line yields 0 pairs and is skipped.
static int salvageLoopedBlocks(const string& content, size_t nBlockers, size_t nAttackers,
                               const vector<string>& blockerNames, const vector<string>& attackerNames,
                               const vector<vector<int> >& legalIdx, vector<int>& out)
{
    vector<string> lines;
    collectLabeledLines(content, "BLOCKS:", lines);
    for (size_t idx = lines.size(); idx-- > 0; )
    {
        vector<int> pick;
        int pairs = parseBlockAssignments(lines[idx], nBlockers, nAttackers, pick,
                                          &blockerNames, &attackerNames, &legalIdx);
        if (pairs > 0) { out = pick; return pairs; }
    }
    return 0;
}

//Salvage a bundled SUBSET reply (ATTACK: attackers, or PUT: reveal picks) from a
//decode spiral: the LAST labeled line that names >=1 eligible item through
//parseAttackerSet. Fills 'out'; returns that count, or -1 if none salvageable.
//A bare "none" is NOT salvaged (declining is the safe default already, and a
//spiral that meant to decline would not have spiraled).
static int salvageLoopedSubset(const string& content, const char * label, size_t n,
                               const vector<string>& names, vector<bool>& out)
{
    vector<string> lines;
    collectLabeledLines(content, label, lines);
    for (size_t idx = lines.size(); idx-- > 0; )
    {
        vector<bool> send;
        int r = parseAttackerSet(lines[idx], n, send, &names);
        if (r >= 1) { out = send; return r; }
    }
    return -1;
}

//--- Prose-intent salvage (ITEM C, last resort before the heuristic) --------
//When a combat reply carried NO well-formed coded line (a >12k-char decode
//spiral truncated before BLOCKS:/ATTACK: - salvage has nothing to grab), scan
//the prose for an EXPLICIT declaration in a NARROW pattern and take it only
//when it maps unambiguously onto the legal set. Deliberately conservative: a
//wrong salvage is worse than a heuristic fallback. Scoped to combat, where
//the answer is an imperative declaration ("block A3", "attack with A1, A2");
//NOT extended to the cast menu, where reasoning prose about NOT casting a card
//would read as casting it (the wave-10 "cast the condemned spell" trap).

//A "block"/"attack" verb occurrence is disqualified when it is negated by a
//nearby preceding word ("do not block", "won't attack", "without blocking").
static bool proseNegatedBefore(const string& low, size_t verbPos)
{
    size_t from = (verbPos >= 12) ? verbPos - 12 : 0;
    string ctx = low.substr(from, verbPos - from);
    static const char * kNeg[] = { "not ", "n't ", "without", "avoid", "instead of", "rather than", "cannot", "can not" };
    for (size_t i = 0; i < sizeof(kNeg) / sizeof(kNeg[0]); i++)
        if (ctx.find(kNeg[i]) != string::npos)
            return true;
    return false;
}

//The 1-based attacker ordinal named by an "A<n>" token starting at or after
//'from', within the same clause (stops at a sentence/newline). -1 if none.
static int proseAttackerOrdinal(const string& low, size_t from, size_t clauseEnd)
{
    for (size_t k = from; k + 1 < clauseEnd; k++)
    {
        if (low[k] == '.' || low[k] == '\n')
            break;
        if (low[k] == 'a' && isdigit((unsigned char) low[k + 1]))
        {
            int n = 0; size_t d = k + 1;
            while (d < low.size() && isdigit((unsigned char) low[d]))
                n = n * 10 + (low[d++] - '0');
            return n;
        }
    }
    return -1;
}

//Prose block salvage: an explicit "block A<n>" (attacker named, blocker left
//implicit) resolves when EXACTLY ONE legal blocker can block that attacker.
//Multiple candidate blockers (or multiple attackers colliding on one blocker)
//-> ambiguous -> nothing. deck14 vs27 s47: "block A3 (Lord of Atlantis)".
static int salvageProseBlocks(const string& content, size_t nBlockers, size_t nAttackers,
                              const vector<vector<int> >& legalIdx, vector<int>& pick)
{
    string low;
    for (size_t i = 0; i < content.size(); i++)
        low += (char) tolower((unsigned char) content[i]);
    std::set<int> wanted;
    size_t pos = 0;
    while ((pos = low.find("block", pos)) != string::npos)
    {
        size_t verb = pos;
        pos += 5;
        if (proseNegatedBefore(low, verb))
            continue;
        //the attacker token must sit right after the verb (a short window)
        size_t window = (pos + 14 < low.size()) ? pos + 14 : low.size();
        int a = proseAttackerOrdinal(low, pos, window);
        if (a >= 1 && a <= (int) nAttackers)
            wanted.insert(a);
    }
    if (wanted.empty())
        return 0;
    pick.assign(nBlockers, 0);
    int pairs = 0;
    for (std::set<int>::iterator it = wanted.begin(); it != wanted.end(); ++it)
    {
        int a = *it;
        int only = -1;
        bool ambiguous = false;
        for (size_t b = 0; b < nBlockers; b++)
        {
            if (pick[b] != 0)
                continue;
            bool legal = false;
            for (size_t j = 0; j < legalIdx[b].size(); j++)
                if (legalIdx[b][j] == a - 1) { legal = true; break; }
            if (legal)
            {
                if (only >= 0) { ambiguous = true; break; }
                only = (int) b;
            }
        }
        if (ambiguous) { pick.assign(nBlockers, 0); return 0; }
        if (only >= 0) { pick[only] = a; pairs++; }
    }
    if (pairs == 0)
        pick.assign(nBlockers, 0);
    return pairs;
}

//Prose attacker salvage: an explicit "attack with A1, A2" declaration - the
//"A<n>" tokens in the first non-negated "attack" clause, mapped straight onto
//the send set. Returns the count sent (0 = nothing usable).
static int salvageProseAttackers(const string& content, size_t nAttackers, vector<bool>& send)
{
    string low;
    for (size_t i = 0; i < content.size(); i++)
        low += (char) tolower((unsigned char) content[i]);
    size_t pos = 0;
    while ((pos = low.find("attack", pos)) != string::npos)
    {
        size_t verb = pos;
        pos += 6;
        if (proseNegatedBefore(low, verb))
            continue;
        size_t clauseEnd = low.find_first_of(".\n", pos);
        if (clauseEnd == string::npos)
            clauseEnd = low.size();
        send.assign(nAttackers, false);
        int count = 0;
        for (size_t k = pos; k + 1 < clauseEnd; k++)
        {
            if (low[k] == 'a' && isdigit((unsigned char) low[k + 1]))
            {
                int n = 0; size_t d = k + 1;
                while (d < clauseEnd && isdigit((unsigned char) low[d]))
                    n = n * 10 + (low[d++] - '0');
                if (n >= 1 && n <= (int) nAttackers && !send[n - 1]) { send[n - 1] = true; count++; }
                k = d - 1;
            }
        }
        if (count > 0)
            return count;
    }
    return 0;
}

//Scan a bundled-attacker reply for the set of attackers to send: "A<n>"
//tokens (or bare numbers) in [1..nAttackers]. Returns >0 = that many named,
//0 = an explicit decline (a "none/hold/pass" keyword with no numbers) OR a
//declaration that named only INELIGIBLE creatures (the eligible subset is
//empty -> ATTACK: none, not a heuristic fallback; deck62 N2), and
//-1 = unusable (empty or no name/number signal at all -> caller falls back).
//The 0/-1 split matters: attacking with nobody is a legitimate choice, so
//"none" must NOT trigger the heuristic override the way a garbled reply does.
static int parseAttackerSet(const string& content, size_t nAttackers, vector<bool>& out,
                            const vector<string> * optionNames = NULL)
{
    out.assign(nAttackers, false);
    int named = 0;
    bool sawNamedContent = false; //the reply named creature(s), eligible or not
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
    //Name->index reconcile (mirrors parseChoice's echo philosophy). The
    //A-index scan silently DROPPED any name token in a mixed reply
    //("ATTACK: A1, Rakdos Cackler" declared only A1 - deck109 lost 6
    //declarations across 3 games), and a pure-name reply ("ATTACK: Hellrider")
    //parsed to nothing -> heuristic. Split the reply on commas/newlines; each
    //segment's significant words (lowercase, len>=4, minus protocol filler)
    //select the UNIQUE listed attacker whose name contains ALL of them. A
    //unique match UNIONS in; zero or multiple matches drop (respecting
    //eligibility - optionNames holds only the legal candidates). A-index
    //segments carry no significant words and are skipped.
    if (optionNames)
    {
        size_t start = 0;
        for (size_t s = 0; s <= content.size(); s++)
        {
            if (s != content.size() && content[s] != ',' && content[s] != '\n')
                continue;
            string seg = content.substr(start, s - start);
            start = s + 1;
            vector<string> words;
            significantWords(seg, words);
            if (words.empty())
                continue;
            sawNamedContent = true; //the model listed a named creature here
            //A "#N" ordinal breaks a same-name tie (two identically-named
            //attackers, or two identical revealed cards); without one an
            //ambiguous name still drops, unchanged.
            size_t limit = optionNames->size() < nAttackers ? optionNames->size() : nAttackers;
            int match = uniqueNameMatch(words, *optionNames, limit, NULL, nameOrdinal(seg));
            if (match >= 0 && !out[match])
            {
                out[match] = true;
                named++;
            }
        }
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
    //The reply NAMED attacker(s) but NONE resolved to an eligible candidate
    //(all summoning-sick / absent from the A-list, or ambiguous duplicates).
    //The faithful reading of "attack with the eligible subset" is an EMPTY
    //set -> ATTACK: none, NOT a heuristic override that may swing at will
    //(deck62 N2: vs14 s21, vs21 s12 both named summoning-sick creatures; the
    //correct answer was ATTACK: none). optionNames scopes this to a real
    //attacker-declaration reply; consumePlan already nulls formless rambles.
    if (sawNamedContent)
        return 0;
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
    //Capture each presented option line for the translog (item: combat
    //options_text was EMPTY, which blocked wave-19's combat-decision review).
    vector<string> shownLines;
    for (size_t j = 0; j < attackers.size(); j++)
    {
        std::ostringstream ln;
        ln << "A" << (j + 1) << ". " << attackers[j]->name
           << " (" << attackers[j]->power << "/" << attackers[j]->toughness << ")";
        string kw = keywordList(attackers[j]);
        if (!kw.empty())
            ln << " [" << kw << "]";
        shownLines.push_back(ln.str());
        tail << ln.str() << "\n";
    }
    tail << "On the FIRST line write ATTACK: followed by the attackers you send,"
            " comma-separated (e.g. \"ATTACK: A1, A3\"), or \"ATTACK: none\" to"
            " attack with nobody this turn; then any brief reasoning; then your"
            " PLAN: line last.";
    string userMsg = assemblePrompt(tail.str());

    string content;
    if (pollCompletionRetry(userMsg, content) == kChoicePending)
        return 1; //decision in flight; nothing declared yet, re-poll next tick

    //Plan split BEFORE the attacker-set parse: numbers (and words like
    //"hold") in the plan prose must not read as attack declarations.
    string decisionPart = consumePlan(content, "ATTACK:");
    vector<bool> send;
    vector<string> attackerNames;
    attackerNames.reserve(attackers.size());
    for (size_t j = 0; j < attackers.size(); j++)
        attackerNames.push_back(attackers[j]->name);

    //ANSWER-FIRST primary (wave-23 ITEM B). The shipped contract puts the
    //declaration on the FIRST line; consumePlan's last-wins otherwise let a
    //LATER same-label CoT combat-math line hijack it - deck109 vs62 s21, where
    //the clean line-1 "ATTACK: A1, A2, A3, A4" was overridden by a line-179 CoT
    //"Attack: Deal 1, Take 5. Net -4 life. Opponent -1 life." whose bare prose
    //numbers (1, -4) parsed to a bogus A1+A4 subset. This is stale-echo family B
    //where the decoy shares the answer's OWN label, so the expectedLabel filter
    //cannot separate them: take the FIRST line-leading ATTACK: line that parses
    //to a usable declaration. (Prose-salvage was NOT the cause - it fires only
    //when result<0; here the mis-parse produced result>=1.)
    int result = -1;
    if (!content.empty())
    {
        string stripped = content;
        size_t te = stripped.rfind("</think>");
        if (te != string::npos)
            stripped = stripped.substr(te + 8);
        vector<string> attackLines;
        collectLabeledLines(stripped, "ATTACK:", attackLines);
        for (size_t li = 0; li < attackLines.size(); li++)
        {
            vector<bool> s;
            int r = parseAttackerSet(attackLines[li], attackers.size(), s, &attackerNames);
            if (r >= 0) { send = s; result = r; break; } //first usable declaration
        }
    }
    //Fallback for a reply that named attackers WITHOUT a line-leading ATTACK:
    //label (consumePlan's short-bare-answer path) - unchanged behavior.
    if (result < 0 && !content.empty())
        result = parseAttackerSet(decisionPart, attackers.size(), send, &attackerNames);

    //Repeat-loop salvage: an unparsed (non-empty) reply may be a decode spiral
    //that stated a valid ATTACK: line before degenerating - recover the last
    //well-formed one through the same validator (mirrors salvageLoopedChoice).
    if (result < 0 && !content.empty())
    {
        int sal = salvageLoopedSubset(content, "ATTACK:", attackers.size(), attackerNames, send);
        if (sal >= 1)
        {
            DebugTrace("AIPlayerGPT: salvaged looped ATTACK (" << sal << ")");
            result = sal;
        }
    }

    //Prose-intent salvage (no coded ATTACK: line at all): recover an explicit
    //"attack with A1, A2" stated in prose before the reply spiralled/truncated.
    const char * attackSource = NULL;
    if (result < 0 && !content.empty())
    {
        int ps = salvageProseAttackers(content, attackers.size(), send);
        if (ps >= 1)
        {
            DebugTrace("AIPlayerGPT: prose-salvaged ATTACK (" << ps << ")");
            result = ps;
            attackSource = "prose";
        }
    }

    if (result < 0)
    {
        //Unusable reply: the heuristic declares this turn's attack instead.
        writeTransLog("attackers", userMsg, content, result, (int) attackers.size(),
                      "", content.empty() ? "empty_reply" : "unparsed_reply", &shownLines);
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
                  declared.empty() ? string("no attackers") : declared, NULL, &shownLines, attackSource);
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
//
//When blockerNames/attackerNames are supplied, a SECOND pass resolves a
//block written with creature NAMES ("BLOCKS: Memnite: Boggart Ram-Gang",
//"Ornithopter blocks Dragonmaster Outcast") back to its B#/A# labels - the
//exact analogue of the shipped ATTACK name->index reconcile and the CHOICE:
//name-echo. A legal, correctly-shaped name-form block was previously dropped
//WHOLE -> unparsed -> the heuristic made a different, sometimes game-losing
//block (deck110 vs21 s18: a 9/1 chumped into a 1/1 wither, game lost by 2).
//Each comma/newline segment splits at a ':'/'blocks' separator into a
//blocker phrase (left) and an attacker phrase (right); each resolves to the
//UNIQUE matching name (attacker restricted to that blocker's legal set when
//given). Ambiguous/no-match drops THAT assignment only; already-coded
//assignments and the first-wins rule are respected.
static int parseBlockAssignments(const string& content, size_t nBlockers, size_t nAttackers, vector<int>& out,
                                 const vector<string> * blockerNames = NULL,
                                 const vector<string> * attackerNames = NULL,
                                 const vector<vector<int> > * legalPerBlocker = NULL)
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

    //Name->label reconcile pass (opt-in via the name tables). Fills only
    //blockers the coded scan left unassigned; never overrides a B#:A# pair.
    if (blockerNames && attackerNames)
    {
        size_t start = 0;
        for (size_t s = 0; s <= content.size(); s++)
        {
            if (s != content.size() && content[s] != ',' && content[s] != '\n')
                continue;
            string seg = content.substr(start, s - start);
            start = s + 1;
            //Find the blocker/attacker separator: ':' or the word "blocks".
            string low;
            for (size_t k = 0; k < seg.size(); k++)
                low += (char) tolower((unsigned char) seg[k]);
            size_t sep = string::npos, sepLen = 0;
            size_t colon = seg.find(':');
            size_t blk = low.find(" blocks ");
            if (colon != string::npos) { sep = colon; sepLen = 1; }
            if (blk != string::npos && (sep == string::npos || blk < sep)) { sep = blk; sepLen = 8; }
            if (sep == string::npos)
                continue; //no blocker:attacker structure in this segment
            string leftSeg = seg.substr(0, sep), rightSeg = seg.substr(sep + sepLen);
            vector<string> leftWords, rightWords;
            significantWords(leftSeg, leftWords);
            significantWords(rightSeg, rightWords);
            int bMatch = uniqueNameMatch(leftWords, *blockerNames, nBlockers, NULL, nameOrdinal(leftSeg));
            if (bMatch < 0 || out[bMatch] != 0)
                continue; //no unique blocker, or already assigned by a code
            const vector<int> * allowed = (legalPerBlocker && (size_t) bMatch < legalPerBlocker->size())
                                          ? &(*legalPerBlocker)[bMatch] : NULL;
            //"#N" on the attacker half ("Saproling (1/1) #1") disambiguates
            //two identically-named attackers the plain match would drop.
            int aMatch = uniqueNameMatch(rightWords, *attackerNames, nAttackers, allowed, nameOrdinal(rightSeg));
            if (aMatch < 0)
                continue; //ambiguous / unmatched attacker -> drop this one only
            out[bMatch] = aMatch + 1;
            pairs++;
        }
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
    //Capture each presented combat option line (attacker context + blocker
    //options WITH their trade annotations) for the translog: combat records
    //logged options_text EMPTY, which blocked wave-19's validation of the
    //trade-outcome annotations. Pure observability - the prompt text below is
    //byte-identical to before, just also accumulated per line.
    vector<string> shownLines;
    tail << "Attackers:\n";
    for (size_t j = 0; j < attackers.size(); j++)
    {
        std::ostringstream ln;
        ln << "A" << (j + 1) << ". " << attackers[j]->name
           << " (" << attackers[j]->power << "/" << attackers[j]->toughness << ")";
        //POWER is the damage number, not toughness. The model misread a
        //Saproling "(2/4)" as dealing 4 (deck35 wave-18 G1); state the damage
        //explicitly at the line that decides.
        ln << " deals " << (attackers[j]->power > 0 ? attackers[j]->power : 0);
        string kw = keywordList(attackers[j]);
        if (!kw.empty())
            ln << " [" << kw << "]";
        //Punisher rider: an attacker whose text does something WHEN BLOCKED
        //or WHEN DEALT DAMAGE (sacrifice permanents, damage you, pump
        //itself) is a trap the bare name hides - surface the text at the
        //line that decides. "block" alone missed the damage-trigger class:
        //Phyrexian Obliterator's "deals damage to" rider stayed hidden and
        //deck109 blocked into it (wave-8).
        string txt = attackers[j]->text;
        for (size_t ti = 0; ti < txt.size(); ti++)
            txt[ti] = (char) tolower((unsigned char) txt[ti]);
        if (txt.find("block") != string::npos
            || txt.find("deals damage") != string::npos
            || txt.find("dealt damage") != string::npos
            || txt.find("deals combat damage") != string::npos)
            ln << " {text: " << cardTextSnippet(attackers[j], 160) << "}";
        shownLines.push_back(ln.str());
        tail << ln.str() << "\n";
    }
    tail << "Your available blockers (with, for each attacker it may block, the"
            " naive 1-on-1 trade - before other blockers, pump or combat tricks):\n";
    for (size_t i = 0; i < blockers.size(); i++)
    {
        std::ostringstream ln;
        ln << "B" << (i + 1) << ". " << blockers[i]->name
           << " (" << blockers[i]->power << "/" << blockers[i]->toughness << ")";
        string kw = keywordList(blockers[i]);
        if (!kw.empty())
            ln << " [" << kw << "]";
        //A 0-power blocker kills nothing - it only absorbs damage. Wave-8:
        //a 0-power blocker was thrown in front of a 0/2 Ornithopter, a
        //block that neither killed nor saved anything (the [deals 0] gap
        //in the blocker-seam lethal family).
        if (blockers[i]->power <= 0)
            ln << " [deals 0 - this block kills nothing, it only absorbs damage]";
        ln << " - may block";
        for (size_t j = 0; j < legal[i].size(); j++)
            for (size_t k = 0; k < attackers.size(); k++)
                if (attackers[k] == legal[i][j])
                {
                    ln << (j ? "," : "") << " A" << (k + 1);
                    //The computed trade rides the B#:A# pairing so there is
                    //nothing left to re-derive (block-outcome annotation).
                    string trade = combatBlockOutcome(blockers[i], attackers[k]);
                    if (!trade.empty())
                        ln << " (" << trade << ")";
                }
        shownLines.push_back(ln.str());
        tail << ln.str() << "\n";
    }
    tail << "Assign each blocker to AT MOST ONE attacker (a creature cannot block"
            " two attackers), but several DIFFERENT blockers may gang-block the same"
            " attacker. Blockers you do not mention stay out of combat.\nOn the"
            " FIRST line write BLOCKS: followed by the assignments, comma-separated,"
            " e.g. \"BLOCKS: B1:A2, B3:A1, B2:none\", or exactly \"BLOCKS: none\" to"
            " block with nobody this turn; then any brief reasoning; then your PLAN:"
            " line last.";
    string userMsg = assemblePrompt(tail.str());

    string content;
    if (pollCompletionRetry(userMsg, content) == kChoicePending)
        return 1; //decision in flight; nothing declared yet, re-poll next tick

    //Plan split BEFORE the assignment parse: a "B2" or bare numbers in the
    //plan prose must not read as block assignments.
    string decisionPart = consumePlan(content);
    //Name tables for the block name->label reconcile (mirror of the shipped
    //ATTACK reconcile): the display names as shown in the B#/A# labels, plus
    //each blocker's legal attacker index set so an ambiguous attacker name
    //can be pinned to the one it may actually block.
    vector<string> blockerNames, attackerNames;
    blockerNames.reserve(blockers.size());
    attackerNames.reserve(attackers.size());
    for (size_t j = 0; j < blockers.size(); j++)
        blockerNames.push_back(blockers[j]->name);
    for (size_t j = 0; j < attackers.size(); j++)
        attackerNames.push_back(attackers[j]->name);
    vector<vector<int> > legalIdx(blockers.size());
    for (size_t i = 0; i < blockers.size(); i++)
        for (size_t j = 0; j < legal[i].size(); j++)
            for (size_t k = 0; k < attackers.size(); k++)
                if (attackers[k] == legal[i][j])
                    legalIdx[i].push_back((int) k);
    vector<int> pick;
    int pairs = content.empty() ? 0 : parseBlockAssignments(decisionPart, blockers.size(), attackers.size(), pick,
                                                             &blockerNames, &attackerNames, &legalIdx);

    //A bare "BLOCKS: none" - the model's natural way to decline every
    //block - carries no B<n> pair, and falling back on it handed the
    //declaration to the heuristic, which BLOCKED (a fatal reach-trade
    //against the pilot's own correct no-block decision, wave-11 deck131
    //vs109). An explicit decline is a valid answer: declare no blockers.
    if (pairs == 0 && !decisionPart.empty())
    {
        string low;
        for (size_t i = 0; i < decisionPart.size() && i < 80; i++)
            low += (char) tolower((unsigned char) decisionPart[i]);
        if (low.find("none") != string::npos || low.find("no block") != string::npos
            || low.find("no assignment") != string::npos)
        {
            DecisionAction none;
            DecisionManager::applyDeclareBlockers(req, none);
            writeTransLog("blockers", userMsg, content, 0, (int) blockers.size(),
                          "no blockers", NULL, &shownLines);
            narrateDecision("You declared no blockers");
            mBlocksDoneTurn = observer->turn;
            DebugTrace("AIPlayerGPT: explicit all-decline (no blockers) in one reply");
            return 1;
        }
    }

    //Repeat-loop salvage: a spiral may have stated a valid BLOCKS: line before
    //degenerating (the seat's fallbacks were 0-of-9 salvaged; the CHOICE
    //salvage did not cover this path). Recover the last well-formed assignment
    //through the full validator; a hallucinated/out-of-range line yields 0 and
    //is skipped, so this never invents a block. Not for the explicit-decline
    //case above (already handled).
    if (pairs == 0 && !content.empty())
    {
        int sal = salvageLoopedBlocks(content, blockers.size(), attackers.size(),
                                      blockerNames, attackerNames, legalIdx, pick);
        if (sal > 0)
        {
            DebugTrace("AIPlayerGPT: salvaged looped BLOCKS (" << sal << " pair(s))");
            pairs = sal;
        }
    }

    //Prose-intent salvage (no coded BLOCKS: line at all): recover an explicit
    //"block A<n>" stated in prose, when exactly one legal blocker maps to it
    //(deck14 vs27 s47's truncated reply). Conservative; ambiguity -> nothing.
    const char * blockSource = NULL;
    if (pairs == 0 && !content.empty())
    {
        int ps = salvageProseBlocks(content, blockers.size(), attackers.size(), legalIdx, pick);
        if (ps > 0)
        {
            DebugTrace("AIPlayerGPT: prose-salvaged BLOCKS (" << ps << " pair(s))");
            pairs = ps;
            blockSource = "prose";
        }
    }

    if (pairs == 0)
    {
        //Unusable reply: the heuristic declares this combat instead.
        writeTransLog("blockers", userMsg, content, pairs, (int) blockers.size(),
                      "", content.empty() ? "empty_reply" : "unparsed_reply", &shownLines);
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
                  declared.empty() ? string("no blockers") : declared, NULL, &shownLines, blockSource);
    narrateDecision(declared.empty() ? string("You declared no blockers")
                                     : ("You declared blockers: " + declared));
    mBlocksDoneTurn = observer->turn;
    DebugTrace("AIPlayerGPT: declared blocks from " << pairs << " assignment(s) in one reply");
    return 1;
}

//A human-readable name for option one's eligibility filter, parsed from the
//card-script target clause (the same restriction the engine's canTarget
//enforces). "target(<upto:1>*[-land;-creature]|reveal)" -> "noncreature,
//nonland card"; "target(land[snow]|reveal)" -> "snow land". Empty string when
//there is no parseable restriction (the per-card [eligible] marks still carry
//the ground truth; this only makes the WHY legible). Representation only.
static string describeRevealFilter(const string& effect)
{
    size_t t = effect.find("target(");
    if (t == string::npos)
        return "";
    size_t open = t + 7;
    size_t close = effect.find(')', open);
    if (close == string::npos || close <= open)
        return "";
    string inner = effect.substr(open, close - open);
    size_t bar = inner.find('|'); //drop the "|reveal" zone suffix
    if (bar != string::npos)
        inner = inner.substr(0, bar);
    if (!inner.empty() && inner[0] == '<') //drop the "<upto:N>"/"<anyamount>" amount
    {
        size_t g = inner.find('>');
        if (g != string::npos)
            inner = inner.substr(g + 1);
    }
    size_t br = inner.find('[');
    string base = (br == string::npos) ? inner : inner.substr(0, br);
    string attrs;
    if (br != string::npos)
    {
        attrs = inner.substr(br + 1);
        size_t brc = attrs.find(']');
        if (brc != string::npos)
            attrs = attrs.substr(0, brc);
    }
    //Each ';'-separated attribute: "-X" reads "nonX", "+X"/"X" reads "X".
    string adjectives;
    {
        string tok;
        for (size_t i = 0; i <= attrs.size(); i++)
        {
            char c = (i < attrs.size()) ? attrs[i] : ';';
            if (c == ';')
            {
                if (!tok.empty())
                {
                    string a = (tok[0] == '-') ? ("non" + tok.substr(1))
                             : (tok[0] == '+') ? tok.substr(1) : tok;
                    adjectives += (adjectives.empty() ? "" : ", ") + a;
                }
                tok.clear();
            }
            else
                tok += c;
        }
    }
    string noun = (base == "*" || base.empty()) ? "card" : base;
    string out = adjectives;
    if (!out.empty())
        out += " ";
    out += noun;
    return out;
}

//Pure builder for the reveal ask text (no game/observer/endpoint state).
//revealSource: 0 = top of library (surveil/dig), 1 = the opponent's hand, 2 =
//the decider's own hand. pickExactlyOne: option one is a fixed <1> chooser.
//
//Eligibility surfacing (unchanged): a FILTERED reveal (a snow-land tutor, a
//noncreature-nonland dig) only accepts a subset for option one; when the
//engine's per-card verdict marks some cards ineligible, name the filter and tag
//each card. Annotation only - every revealed card stays listed and pickable.
//
//deck102 wave-20 E1: a HAND reveal (Thoughtseize/Duress-class targeted discard)
//was mislabelled "the top N cards of your library" and used choose-a-SUBSET
//framing on a pick-EXACTLY-ONE discard. Render the true source, and frame a
//fixed <1> chooser as choose-ONE. Library/multi-pick reveals (Glacial
//Revelation, Into the North, surveil - wave-20 ENGINE-R1) hit revealSource 0 +
//pickExactlyOne=false and keep byte-identical prior text.
static string buildRevealAskText(const vector<MTGCardInstance*>& revealed,
                                 const string& optOneLabel,
                                 const string& optTwoLabel,
                                 const string& optOneEffect,
                                 const vector<bool>& eligibleForOptionOne,
                                 int revealSource, bool pickExactlyOne)
{
    bool haveElig = (eligibleForOptionOne.size() == revealed.size());
    int eligCount = 0;
    if (haveElig)
        for (size_t j = 0; j < eligibleForOptionOne.size(); j++)
            if (eligibleForOptionOne[j]) eligCount++;
    bool restricted = haveElig && eligCount < (int) revealed.size();
    string filterPhrase = restricted ? describeRevealFilter(optOneEffect) : string();

    bool fromHand = (revealSource == 1 || revealSource == 2);
    bool selfHand = (revealSource == 2);
    std::ostringstream tail;
    if (fromHand)
    {
        tail << (selfHand ? "You revealed your hand"
                          : "The opponent revealed their hand")
             << " (" << revealed.size() << " card"
             << (revealed.size() == 1 ? "" : "s") << ").\n";
        if (pickExactlyOne)
            tail << "Choose the ONE card to send to \"" << optOneLabel
                 << "\" - that is the card "
                 << (selfHand ? "you discard" : "they discard")
                 << "; every other card stays in "
                 << (selfHand ? "your" : "their") << " hand.\n";
        else
            tail << "Decide, in ONE reply, which cards go to \"" << optOneLabel
                 << "\"; every card you do NOT pick goes to \"" << optTwoLabel
                 << "\".\n";
    }
    else
    {
        tail << "Reveal: you looked at the top " << revealed.size()
             << " card" << (revealed.size() == 1 ? "" : "s") << " of your library.";
        if (pickExactlyOne)
            tail << " Choose the ONE card that goes to \"" << optOneLabel
                 << "\"; every other card goes to \"" << optTwoLabel << "\".\n";
        else
            tail << " Decide, in ONE reply, which of them go to \"" << optOneLabel
                 << "\"; every card you do NOT pick goes to \"" << optTwoLabel
                 << "\".\n";
    }
    if (restricted)
    {
        tail << "ELIGIBILITY: only " << (filterPhrase.empty() ? "certain cards"
                                                              : ("a " + filterPhrase))
             << " may go to \"" << optOneLabel << "\" - the rest do not qualify and go"
                " to \"" << optTwoLabel << "\" regardless. Pick ONLY from the cards"
                " marked [eligible] below";
        if (eligCount == 0)
            tail << " (none of these qualify - answer \"PUT: none\")";
        tail << ".\n";
    }
    for (size_t j = 0; j < revealed.size(); j++)
    {
        tail << (j + 1) << ". " << revealed[j]->name;
        if (revealed[j]->isCreature())
            tail << " (" << revealed[j]->power << "/" << revealed[j]->toughness
                 << " creature)";
        else
        {
            string tt = typeTag(revealed[j]);
            if (!tt.empty())
                tail << " (" << tt << ")";
        }
        string kw = keywordList(revealed[j]);
        if (!kw.empty())
            tail << " [" << kw << "]";
        string txt = cardTextSnippet(revealed[j], 140);
        if (!txt.empty())
            tail << " {text: " << txt << "}";
        //The eligibility tag rides the option line so the filter cannot be
        //missed (the same "deciding fact rides the option" principle as combat).
        if (restricted)
            tail << (eligibleForOptionOne[j]
                     ? " [eligible for \"" + optOneLabel + "\"]"
                     : " [does NOT qualify - goes to \"" + optTwoLabel + "\"]");
        tail << "\n";
    }
    if (pickExactlyOne)
        tail << "On the FIRST line write PUT: followed by the ONE card number you"
                " choose (e.g. \"PUT: 2\")"
             << (eligCount == 0 ? ", or \"PUT: none\" if none qualify" : "")
             << "; then any brief reasoning; then your PLAN: line last.";
    else
        tail << "On the FIRST line write PUT: followed by the card numbers you send to \""
             << optOneLabel << "\", comma-separated (e.g. \"PUT: 1, 3\"), or exactly"
                " \"PUT: none\" to send none there (every revealed card then goes to \""
             << optTwoLabel << "\"); then any brief reasoning; then your PLAN: line last.";
    return tail.str();
}

//Interactive reveal/surveil decision, driven from MTGRevealingCards for an
//interactive AI. ONE bundled ask over ALL revealed cards; the model picks the
//subset that goes to option one (surveil: the graveyard). Async: 0 while the
//call is in flight, 1 decided, -1 on failure (the display then sends nothing
//to option one - the safe keep-on-top default).
int AIPlayerGPT::decideReveal(const vector<MTGCardInstance*>& revealed,
                              const string& optOneLabel, const string& optTwoLabel,
                              const string& optOneEffect,
                              vector<int>& selForOptionOne,
                              const vector<bool>& eligibleForOptionOne,
                              int revealSource, bool pickExactlyOne)
{
    selForOptionOne.clear();
    if (mEndpoint.empty() || revealed.empty())
        return -1; //no endpoint / nothing to choose: the display's default

    if (mSystemPrompt.empty())
        buildSystemPrompt();

    string userMsg = assemblePrompt(
        buildRevealAskText(revealed, optOneLabel, optTwoLabel, optOneEffect,
                           eligibleForOptionOne, revealSource, pickExactlyOne));

    string content;
    if (pollCompletionRetry(userMsg, content) == kChoicePending)
        return 0; //decision in flight; the display waits and re-polls next tick

    //Plan split BEFORE the subset parse: bare numbers in the plan prose must
    //not read as card picks.
    string decisionPart = consumePlan(content);
    vector<bool> send;
    vector<string> names;
    names.reserve(revealed.size());
    for (size_t j = 0; j < revealed.size(); j++)
        names.push_back(revealed[j]->name);
    int result = content.empty() ? -1
                 : parseAttackerSet(decisionPart, revealed.size(), send, &names);

    //Repeat-loop salvage: the reveal seam's worst fallbacks are 12k+ decode
    //spirals; if one stated a valid PUT: line before degenerating, recover it
    //(the same net as salvageLoopedChoice, through parseAttackerSet's validator).
    if (result < 0 && !content.empty())
    {
        int sal = salvageLoopedSubset(content, "PUT:", revealed.size(), names, send);
        if (sal >= 1)
        {
            DebugTrace("AIPlayerGPT: salvaged looped PUT (" << sal << ")");
            result = sal;
        }
    }

    if (result < 0)
    {
        //Unusable reply: the display falls back to its safe default (send
        //nothing to option one - every card keeps option two).
        writeTransLog("reveal", userMsg, content, result, (int) revealed.size(),
                      "", content.empty() ? "empty_reply" : "unparsed_reply", &names);
        setNotice("model reply failed - reveal kept the default", 5.0f);
        return -1;
    }

    string chosen;
    for (size_t j = 0; j < revealed.size(); j++)
        if (send[j])
        {
            selForOptionOne.push_back((int) j);
            chosen += (chosen.empty() ? "" : ", ") + revealed[j]->name;
            //A fixed <1> chooser takes exactly ONE card: the engine's option-one
            //chooser auto-fires on the first eligible click (driveInteractiveReveal
            //phase 0), so any extra picks would silently drop. Trim to the first
            //selected card here so narration/translog match what the engine does.
            if (pickExactlyOne)
                break;
        }
    writeTransLog("reveal", userMsg, content, result, (int) revealed.size(),
                  chosen.empty() ? string("none") : chosen, NULL, &names);
    narrateDecision(chosen.empty()
                    ? ("You revealed " + std::to_string(revealed.size())
                       + " and kept them all (" + optTwoLabel + ")")
                    : ("You revealed " + std::to_string(revealed.size()) + " and put "
                       + chosen + " to " + optOneLabel));
    DebugTrace("AIPlayerGPT: reveal put " << selForOptionOne.size() << " of "
               << revealed.size() << " to option one in one reply");
    return 1;
}

//---- Pre-game (opening-hand) decision hooks (PreGamePhase, before turn 1) ----

int AIPlayerGPT::pregameMulliganDecision(int mullsTaken)
{
    if (mEndpoint.empty())
        return AIPlayerBaka::pregameMulliganDecision(mullsTaken);
    std::ostringstream q;
    int keepSize = startingHandSize() - mullsTaken;
    q << "Pre-game mulligan decision (London mulligan). You have a fresh "
      << game->hand->nb_cards << "-card opening hand";
    if (mullsTaken > 0)
        q << ", and having already taken " << mullsTaken << " mulligan"
          << (mullsTaken > 1 ? "s" : "") << " you will bottom " << mullsTaken
          << " card" << (mullsTaken > 1 ? "s" : "") << " on a keep (keeping "
          << keepSize << ")";
    q << ". Keep this hand, or mulligan (shuffle back and draw " << startingHandSize()
      << " again, bottoming one more at the next keep)?";
    vector<string> opts;
    opts.push_back("Keep this hand");
    opts.push_back("Mulligan");
    int pick = askModel(q.str(), opts, true);
    if (pick == kChoicePending)
        return PREGAME_PENDING;
    if (pick < 0)
        return AIPlayerBaka::pregameMulliganDecision(mullsTaken);
    return pick; //0 keep, 1 mulligan
}

int AIPlayerGPT::pregameLeylineDecision(MTGCardInstance * card)
{
    if (mEndpoint.empty() || !card)
        return AIPlayerBaka::pregameLeylineDecision(card);
    std::ostringstream q;
    q << "Pre-game action (CR 103.6): you may begin the game with " << card->name
      << " already on the battlefield (its static ability is live for the whole"
         " game). Put it onto the battlefield?";
    vector<string> opts;
    opts.push_back("Yes, begin the game with " + card->name + " on the battlefield");
    opts.push_back("No, keep it in hand");
    int pick = askModel(q.str(), opts, true);
    if (pick == kChoicePending)
        return PREGAME_PENDING;
    if (pick < 0)
        return AIPlayerBaka::pregameLeylineDecision(card);
    return pick == 0 ? 1 : 0;
}

//Build the tail for the ONE bundled BOTTOM-N ask (reuses the reveal PUT: reply
//shape; parsed by parseAttackerSet / salvageLoopedSubset like decideReveal).
string AIPlayerGPT::buildPregameBottomAskText(const vector<MTGCardInstance*>& hand, int need)
{
    std::ostringstream tail;
    int keep = (int) hand.size() - need;
    tail << "London mulligan bottoming (CR 103.5): you kept after " << need
         << " mulligan" << (need > 1 ? "s" : "") << ", so you must put EXACTLY "
         << need << " card" << (need > 1 ? "s" : "")
         << " from your hand on the BOTTOM of your library. Keep your best "
         << keep << ", bottom your worst " << need << ".\n";
    for (size_t j = 0; j < hand.size(); j++)
    {
        tail << (j + 1) << ". " << hand[j]->name;
        if (hand[j]->isLand())
            tail << " (land)";
        else if (hand[j]->getManaCost())
            tail << " (cost " << hand[j]->getManaCost()->getConvertedCost() << ")";
        string txt = cardTextSnippet(hand[j], 120);
        if (!txt.empty())
            tail << " {text: " << txt << "}";
        tail << "\n";
    }
    tail << "On the FIRST line write PUT: followed by the " << need << " card number"
         << (need > 1 ? "s" : "") << " you send to the bottom, comma-separated (e.g. \"PUT: "
         << (need == 1 ? "3" : "3, 5") << "\"); then brief reasoning; then your PLAN: line last.";
    return tail.str();
}

MTGCardInstance * AIPlayerGPT::pregameChooseBottom(int need, int chosenSoFar, int & status)
{
    status = 0;
    if (mEndpoint.empty())
        return AIPlayerBaka::pregameChooseBottom(need, chosenSoFar, status);

    //ONE bundled ask per keep; then pop the queued cards one per call.
    if (!mPregameBottomAsked || mPregameBottomForMulls != need)
    {
        vector<MTGCardInstance*> hand;
        for (int i = 0; i < game->hand->nb_cards; i++)
            hand.push_back(game->hand->cards[i]);
        if (mSystemPrompt.empty())
            buildSystemPrompt();
        string userMsg = assemblePrompt(buildPregameBottomAskText(hand, need));
        string content;
        if (pollCompletionRetry(userMsg, content) == kChoicePending)
        {
            status = PREGAME_PENDING;
            return NULL; //call in flight; PreGamePhase re-polls next tick
        }
        string decisionPart = consumePlan(content);
        vector<bool> send;
        vector<string> names;
        for (size_t j = 0; j < hand.size(); j++)
            names.push_back(hand[j]->name);
        int result = content.empty() ? -1
                     : parseAttackerSet(decisionPart, hand.size(), send, &names);
        if (result < 0 && !content.empty())
        {
            int sal = salvageLoopedSubset(content, "PUT:", hand.size(), names, send);
            if (sal >= 1)
                result = sal;
        }
        mPregameBottomQueue.clear();
        string chosen;
        if (result >= 0)
            for (size_t j = 0; j < hand.size() && (int) mPregameBottomQueue.size() < need; j++)
                if (j < send.size() && send[j])
                {
                    mPregameBottomQueue.push_back(hand[j]);
                    chosen += (chosen.empty() ? "" : ", ") + hand[j]->name;
                }
        //Enforce EXACTLY N: the model under-picked or failed -> fill with the
        //highest-cost cards not already chosen (the heuristic policy). We already
        //capped at N above, so over-picks are trimmed.
        while ((int) mPregameBottomQueue.size() < need)
        {
            MTGCardInstance * fill = NULL;
            int fc = -1;
            for (int i = 0; i < game->hand->nb_cards; i++)
            {
                MTGCardInstance * c = game->hand->cards[i];
                bool already = false;
                for (size_t k = 0; k < mPregameBottomQueue.size(); k++)
                    if (mPregameBottomQueue[k] == c) { already = true; break; }
                if (already)
                    continue;
                int cost = c->getManaCost() ? c->getManaCost()->getConvertedCost() : 0;
                if (cost > fc) { fc = cost; fill = c; }
            }
            if (!fill)
                break;
            mPregameBottomQueue.push_back(fill);
        }
        writeTransLog("bottom", userMsg, content, result, (int) hand.size(), chosen,
                      result < 0 ? (content.empty() ? "empty_reply" : "unparsed_reply") : NULL,
                      &names);
        mPregameBottomAsked = true;
        mPregameBottomForMulls = need;
    }
    //Pop the next queued card that is still in hand.
    while (!mPregameBottomQueue.empty())
    {
        MTGCardInstance * c = mPregameBottomQueue.front();
        mPregameBottomQueue.erase(mPregameBottomQueue.begin());
        if (game->hand->hasCard(c))
            return c;
    }
    return AIPlayerBaka::pregameChooseBottom(need, chosenSoFar, status);
}

//Env-gated (WAGIC_GPT_PARSETEST) self-test: drives the wave-19 failing replies
//and synthetic decode-spiral strings through the REAL static parse/salvage
//paths and prints before/after. No game/observer needed - called from main()
//before setup. Same-TU access to the file-static parsers is the point.
void AIPlayerGPT::runParseSelfTest()
{
    using std::cout;
    int passed = 0, failed = 0;
    #define CHECK(cond, label) do { \
        if (cond) { cout << "  PASS  " << label << "\n"; passed++; } \
        else { cout << "  FAIL  " << label << "\n"; failed++; } } while (0)

    cout << "=== AIPlayerGPT parse self-test (wave-20 3a/3b/3c + wave-21 A/B/C + wave-22 N9/A2/B3) ===\n";

    // ---- ITEM 3b: "#N" ordinal disambiguates duplicate names ----
    cout << "\n[3b] deck135 wave-19 s27: 'BLOCKS: Ice-Fang Coatl: Saproling (1/1) #1'\n";
    cout << "     board: B1=Ice-Fang Coatl; A1=Saproling A2=Argothian Enchantress A3=Saproling\n";
    {
        vector<string> bn; bn.push_back("Ice-Fang Coatl");
        vector<string> an; an.push_back("Saproling"); an.push_back("Argothian Enchantress"); an.push_back("Saproling");
        vector<vector<int> > legal(1); legal[0].push_back(0); legal[0].push_back(1); legal[0].push_back(2);
        // WITH the "#1" ordinal (the real reply): resolves to the 1st Saproling = A1.
        vector<int> out;
        int pairs = parseBlockAssignments("Ice-Fang Coatl: Saproling (1/1) #1", 1, 3, out, &bn, &an, &legal);
        cout << "     with '#1': pairs=" << pairs << " B1->A" << (out.empty()?0:out[0]) << "\n";
        CHECK(pairs == 1 && out[0] == 1, "3b with '#1' -> B1 blocks A1 (was dropped/unparsed pre-fix)");
        // WITHOUT the ordinal: 'Saproling' is genuinely ambiguous -> still dropped (guard intact).
        vector<int> out2;
        int pairs2 = parseBlockAssignments("Ice-Fang Coatl: Saproling", 1, 3, out2, &bn, &an, &legal);
        cout << "     no ordinal: pairs=" << pairs2 << " (ambiguous duplicate -> drop, guard intact)\n";
        CHECK(pairs2 == 0, "3b ambiguity guard: bare duplicate name still drops (no arbitrary guess)");
    }
    // parseAttackerSet ordinal (two identical attacker names + '#2')
    {
        vector<string> an; an.push_back("Saproling"); an.push_back("Saproling");
        vector<bool> send;
        int r = parseAttackerSet("ATTACK: Saproling #2", 2, send, &an);
        cout << "     ATTACK 'Saproling #2': result=" << r << " send=["
             << (send.size()>0&&send[0]) << "," << (send.size()>1&&send[1]) << "]\n";
        CHECK(r == 1 && !send[0] && send[1], "3b attacker '#2' selects the 2nd of two Saprolings");
    }

    // ---- ITEM 3c: mixed reply keeps valid pairing, drops only the invalid ----
    cout << "\n[3c] mixed 'BLOCKS: B1:A1, B2:A5' with only 2 attackers (A5 hallucinated)\n";
    {
        vector<int> out;
        int pairs = parseBlockAssignments("B1:A1, B2:A5", 2, 2, out, NULL, NULL, NULL);
        cout << "     pairs=" << pairs << " B1->A" << out[0] << " B2->A" << out[1] << "\n";
        CHECK(pairs == 1 && out[0] == 1 && out[1] == 0, "3c valid B1:A1 kept, invalid B2:A5 ignored");
        // attacker analogue: 'ATTACK: A1, A9' with 3 attackers keeps A1
        vector<bool> send;
        int r = parseAttackerSet("A1, A9", 3, send, NULL);
        cout << "     ATTACK 'A1, A9' (3 atk): result=" << r << " A1=" << (int)send[0] << " A9-dropped\n";
        CHECK(r == 1 && send[0] && !send[1] && !send[2], "3c valid A1 kept, out-of-range A9 ignored");
    }

    // ---- ITEM 3a: repeat-loop salvage of BLOCKS / ATTACK / PUT ----
    cout << "\n[3a] salvage a valid labeled line from a decode spiral (last-valid wins)\n";
    {
        // A spiral that stated a valid block, looped, then emitted a bad final line.
        string spiral = "PLAN: I am at 5 life vs a 3/3.\nBLOCKS: B1:A1\n"
                        "wait wait wait wait wait wait wait wait\nBLOCKS: B1:A9";
        vector<string> bn; bn.push_back("Wall");
        vector<string> an; an.push_back("Ogre");
        vector<vector<int> > legal(1); legal[0].push_back(0);
        vector<int> out;
        int sal = salvageLoopedBlocks(spiral, 1, 1, bn, an, legal, out);
        cout << "     BLOCKS spiral salvage: pairs=" << sal << " B1->A" << (out.empty()?0:out[0])
             << " (final line B1:A9 is out-of-range and skipped)\n";
        CHECK(sal == 1 && out[0] == 1, "3a BLOCKS salvage recovers the earlier valid B1:A1");
    }
    {
        string spiral = "PLAN: swing wide.\nATTACK: A1, A3\nloop loop loop loop loop\nATTACK:";
        vector<string> an; an.push_back("Bear"); an.push_back("Bird"); an.push_back("Beast");
        vector<bool> send;
        int sal = salvageLoopedSubset(spiral, "ATTACK:", 3, an, send);
        cout << "     ATTACK spiral salvage: result=" << sal
             << " A1=" << (send.size()>0?(int)send[0]:-1) << " A3=" << (send.size()>2?(int)send[2]:-1) << "\n";
        CHECK(sal == 2 && send[0] && !send[1] && send[2], "3a ATTACK salvage recovers 'A1, A3'");
    }
    {
        string spiral = "PLAN: keep the bomb.\nPUT: 2\nhmm hmm hmm hmm hmm hmm hmm\nblah";
        vector<string> nm; nm.push_back("Island"); nm.push_back("Dragon"); nm.push_back("Forest");
        vector<bool> send;
        int sal = salvageLoopedSubset(spiral, "PUT:", 3, nm, send);
        cout << "     PUT spiral salvage: result=" << sal
             << " picks card #2=" << (send.size()>1?(int)send[1]:-1) << "\n";
        CHECK(sal == 1 && !send[0] && send[1] && !send[2], "3a PUT salvage recovers 'PUT: 2'");
    }

    // ---- Real wave-19 hallucinations: MUST still fall back (nothing valid) ----
    cout << "\n[verify] wave-19 hallucinated out-of-range replies stay unsalvageable\n";
    {
        // deck14 s31 / deck27 s9 shape: model answered B1:A2 but only A1 exists.
        vector<string> bn; bn.push_back("Dragon");
        vector<string> an; an.push_back("Outcast"); // only ONE attacker
        vector<vector<int> > legal(1); legal[0].push_back(0);
        string reply = "PLAN: block the 5/5 dragon to survive.\nBLOCKS: B1:A2";
        vector<int> pout;
        int primary = parseBlockAssignments("B1:A2", 1, 1, pout, &bn, &an, &legal);
        vector<int> sout;
        int sal = salvageLoopedBlocks(reply, 1, 1, bn, an, legal, sout);
        cout << "     s31/s9 'B1:A2' (only A1): primary pairs=" << primary
             << " salvage pairs=" << sal << " -> correct heuristic fallback\n";
        CHECK(primary == 0 && sal == 0, "hallucinated out-of-range attacker: no fabricated block");
    }

    // ================= WAVE-21 items (A wither / B parser / C salvage) =====

    // ---- ITEM A: wither trade-annotation lethality + survivor shrink ----
    cout << "\n[A] wither blocker trade annotation (combatTradePreviewStats)\n";
    {
        // fields: power, toughness, deathtouch, wither, infectLabel, firststrike, indestructible, trample
        CombatTradeStat oona = { 2, 1, false, true, false, false, false, false }; // Oona's Gatewarden 2/1 wither
        CombatTradeStat a34  = { 3, 4, false, false, false, false, false, false };
        CombatTradeStat a88  = { 8, 8, false, false, false, false, false, false };
        CombatTradeStat a13  = { 1, 3, false, false, false, false, false, false };
        CombatTradeStat dt11 = { 1, 1, true,  false, false, false, false, false }; // 1/1 deathtouch
        string s34 = combatTradePreviewStats(oona, a34);
        string s88 = combatTradePreviewStats(oona, a88);
        string s13 = combatTradePreviewStats(oona, a13);
        string sdt = combatTradePreviewStats(oona, dt11);
        cout << "     Oona 2/1 wither vs 3/4: \"" << s34 << "\"  (pre-fix said \"both die, 2 tramples\")\n";
        cout << "     Oona 2/1 wither vs 8/8: \"" << s88 << "\"\n";
        cout << "     Oona 2/1 wither vs 1/3: \"" << s13 << "\"\n";
        cout << "     Oona 2/1 wither vs 1/1 deathtouch attacker: \"" << sdt << "\"\n";
        CHECK(s34 == "your blocker dies, attacker lives (wither shrinks it to 1/2)", "A vs 3/4 survives 1/2, not 'both die'");
        CHECK(s88 == "your blocker dies, attacker lives (wither shrinks it to 6/6)", "A vs 8/8 survives 6/6");
        CHECK(s13 == "your blocker dies, attacker lives (wither shrinks it to 0/1)", "A vs 1/3 survives 0/1");
        CHECK(sdt == "both die", "A deathtouch attacker vs Oona correctly both die (lethal wither kills the 1/1)");
        // Symmetric: a WITHER ATTACKER whose power cannot kill the blocker.
        CombatTradeStat b14 = { 1, 4, false, false, false, false, false, false };
        CombatTradeStat witherAtk = { 2, 1, false, true, false, false, false, false };
        string ssym = combatTradePreviewStats(b14, witherAtk);
        cout << "     blocker 1/4 vs 2/1 wither attacker: \"" << ssym << "\"\n";
        CHECK(ssym == "you kill it, your blocker lives (wither shrinks your blocker to 0/2)",
              "A symmetric: wither attacker shrinks the surviving blocker, does not auto-kill it");
        // Indestructible attacker still dies to a LETHAL wither hit (0 toughness SBA).
        CombatTradeStat oona2 = { 2, 1, false, true, false, false, false, false };
        CombatTradeStat aIndest = { 3, 2, false, false, false, false, true, false }; // 3/2 indestructible
        string sind = combatTradePreviewStats(oona2, aIndest);
        cout << "     Oona 2/1 wither vs 3/2 indestructible: \"" << sind << "\"\n";
        CHECK(sind == "both die", "A lethal wither (bp>=at) kills even an indestructible attacker");
    }

    // The final-precedence CHOICE resolver used at the call sites, as the
    // test's model: last well-formed CHOICE wins (salvage authority), UNLESS
    // it was retracted with no replacement -> heuristic fallback (-1).
    // (parseChoice on consumePlan's single answer line agrees for these.)
    #define RESOLVE(reply, oc, opts) ([&]() -> int { \
        int _c = salvageLoopedChoice((reply), (oc), &(opts)); \
        if (_c >= 0 && choiceRetractedNoReplacement((reply), (oc), &(opts))) _c = -1; \
        return _c; }())

    vector<string> opts;
    opts.push_back("Cast Icehide Golem");   // 1
    opts.push_back("Attack for 3");         // 2
    opts.push_back("Hold up mana");         // 3
    opts.push_back("Cast nothing right now");// 4
    opts.push_back("Play a land");          // 5

    // ---- ITEM B: retracted CHOICE + template-parrot ----
    cout << "\n[B] retracted / duplicate CHOICE lock-in + template-parrot lines\n";
    {
        //Wave-23 rework: a generic self-correction phrase ("made a mistake") with
        //a single SUSTAINED coded index and no contradictory coded sibling no
        //longer retracts - INDEX-WINS trusts the coded digit (deck133 s29 class).
        string retractNoRepl = "CHOICE: 4 (Cast nothing right now)\n"
                               "Wait, I made a mistake in my reasoning.\n"
                               "cast cast cast cast the golem instead\n";
        int r = RESOLVE(retractNoRepl, 5, opts);
        cout << "     generic-phrase + sustained CHOICE 4 -> " << r << " (INDEX-WINS: trust 4)\n";
        CHECK(r == 4, "B generic self-correction with a sustained coded index is trusted (not retracted)");

        string parrot = "PLAN: deploy the threat.\n"
                        "CHOICE: [Number] ([Name])\n"
                        "CHOICE: 1 (Cast Icehide Golem)\n"
                        "CHOICE: [Number] ([Name])\n"
                        "CHOICE: Argothian Enchantress\n";
        int p = RESOLVE(parrot, 5, opts);
        cout << "     template-parrot + real CHOICE 1 -> " << p << "\n";
        CHECK(p == 1, "B template-parrot lines dropped; real CHOICE 1 recovered");

        string retractThenNew = "CHOICE: 4 (Cast nothing right now)\n"
                                "Wait, I made a mistake. The golem is better.\n"
                                "CHOICE: 1 (Cast Icehide Golem)\n";
        int n = RESOLVE(retractThenNew, 5, opts);
        cout << "     retraction-then-new-choice -> " << n << "\n";
        CHECK(n == 1, "B retraction followed by a NEW choice takes the new one");

        string plain = "CHOICE: 2 (Attack for 3)\nquick note\nPLAN: race them\n";
        int pl = RESOLVE(plain, 5, opts);
        cout << "     plain single choice -> " << pl << "\n";
        CHECK(pl == 2, "B plain single choice unchanged");
    }

    // ---- ITEM C: answer-first parse composition + prose-intent salvage ----
    cout << "\n[C] answer-first + prose salvage\n";
    {
        string afReason = "CHOICE: 2 (Attack for 3)\n"
                          "I attack because I am ahead on board and want to pressure their life total.\n"
                          "PLAN: keep attacking each turn.\n";
        int a = RESOLVE(afReason, 5, opts);
        cout << "     answer-first then reasoning -> " << a << "\n";
        CHECK(a == 2, "C answer-first-then-reasoning parses");

        string afTrunc = "CHOICE: 3 (Hold up mana)\n"
                         "I hold because I have an instant and want to represen"; // truncated mid-word, no PLAN
        int t = RESOLVE(afTrunc, 5, opts);
        cout << "     truncated answer-first -> " << t << "\n";
        CHECK(t == 3, "C truncated answer-first still parses (first line survived)");

        // prose block salvage: "block A3", one blocker legal for A3 -> B1:A3
        {
            vector<vector<int> > legal(1); legal[0].push_back(0); legal[0].push_back(1); legal[0].push_back(2);
            string reply = "I have thought about this combat for a long time and considered every line. "
                           "In the end I should block A3 (Lord of Atlantis) to trade with their lord.";
            vector<int> pick;
            int ps = salvageProseBlocks(reply, 1, 3, legal, pick);
            cout << "     prose 'block A3' (1 legal blocker) -> pairs=" << ps
                 << " B1->A" << (pick.empty()?0:pick[0]) << "\n";
            CHECK(ps == 1 && pick[0] == 3, "C prose 'block A3' salvages to the unique legal blocker");
        }
        // ambiguous: TWO blockers legal for A3 -> no salvage
        {
            vector<vector<int> > legal(2); legal[0].push_back(2); legal[1].push_back(2);
            string reply = "I should block A3 with something.";
            vector<int> pick;
            int ps = salvageProseBlocks(reply, 2, 3, legal, pick);
            cout << "     prose 'block A3' (2 legal blockers) -> pairs=" << ps << " (ambiguous -> fallback)\n";
            CHECK(ps == 0, "C ambiguous prose does NOT salvage");
        }
        // negated: "I will not block A3" -> no salvage
        {
            vector<vector<int> > legal(1); legal[0].push_back(2);
            string reply = "I will not block A3, it is too risky to trade here.";
            vector<int> pick;
            int ps = salvageProseBlocks(reply, 1, 3, legal, pick);
            cout << "     prose 'will NOT block A3' -> pairs=" << ps << " (negation guard)\n";
            CHECK(ps == 0, "C negated prose ('not block') does NOT salvage");
        }
        // prose attacker salvage: "attack with A1 and A3"
        {
            string reply = "After a long deliberation I will attack with A1 and A3 to close the game.";
            vector<bool> send;
            int ps = salvageProseAttackers(reply, 3, send);
            cout << "     prose 'attack with A1 and A3' -> count=" << ps
                 << " send=[" << (send.size()>0?(int)send[0]:-1) << ","
                 << (send.size()>1?(int)send[1]:-1) << "," << (send.size()>2?(int)send[2]:-1) << "]\n";
            CHECK(ps == 2 && send[0] && !send[1] && send[2], "C prose 'attack with A1 and A3' salvages");
        }
    }

    // ================= WAVE-22 items (N9 retract over-fire / stale-echo A+B) ==
    // Built from the real matchups-20260723-084938 corpus replies (seq cited).

    // ---- ITEM 1 (HARNESS-N9, REWORKED wave-23): retracted_choice fires ONLY on
    //      (a) a contradictory second coded index or (b) a payability disavowal ----
    cout << "\n[N9] retracted_choice: over-fire guards stay clean; generic self-correction no longer retracts a sustained index\n";
    {
        // deck62 vs27 s10 (real, FALSE POSITIVE): correct CHOICE 1 (Play Forest)
        // never retracted; "Correction:" lives in the DOWNSTREAM PLAN prose
        // (a mana/Shroud aside), AFTER the first line-leading PLAN: marker.
        vector<string> land; land.push_back("Play Forest");
        land.push_back("Hold Forest - do not play it now");
        string fpLand =
            "CHOICE: 1 (Play Forest)\n"
            "The opponent has a flying blocker and an unblockable attacker. I need to establish a board presence and draw cards.\n"
            "PLAN: Play Forest to increase mana base. Cast Fists of Ironwood targeting a Saproling. Wait, Fists requires a creature target and Argothian has Shroud.\n"
            "Correction: I cannot cast Fists or Web because I have no legal target (Argothian has Shroud). I must play the Forest and pass the turn.\n"
            "PLAN: Play Forest. Pass the turn without casting any spells since I lack a legal target for my auras.\n";
        int r1 = RESOLVE(fpLand, 2, land);
        cout << "     deck62 vs27 s10 (Correction: after PLAN) -> " << r1 << " (must be 1, not fallback)\n";
        CHECK(r1 == 1, "N9 FP land-drop: 'Correction:' in PLAN prose does NOT retract the CHOICE");

        // deck135 vs27 s26 (real, FALSE POSITIVE): CHOICE 2 (Ice-Fang Coatl);
        // "Correction: **Islandwalk** means..." is rules trivia after PLAN:.
        vector<string> cast; cast.push_back("Cast Glacial Revelation {2}{g}");
        cast.push_back("Cast Ice-Fang Coatl {g}{u} (1/1)");
        cast.push_back("Cast nothing right now");
        string fpRules =
            "CHOICE: 2 (Cast Ice-Fang Coatl)\n"
            "It is a creature and casting a creature takes priority. It draws a card and adds a flyer.\n"
            "PLAN: Cast Ice-Fang Coatl. Then attack with it since the opponent's ground creatures cannot block a flyer. Wait, Vodalian Zombie has islandwalk.\n"
            "Correction: **Islandwalk** means the creature can't be blocked as long as the defender controls an Island. It does not let it block a flyer.\n";
        int r2 = RESOLVE(fpRules, 3, cast);
        cout << "     deck135 vs27 s26 (rules 'Correction:' after PLAN) -> " << r2 << " (must be 2)\n";
        CHECK(r2 == 2, "N9 FP rules-aside: downstream 'Correction:' about islandwalk does NOT retract");

        // deck35 vs62 s18 (real): snap CHOICE 3 (Heart), "Actually, no." before
        // PLAN referencing Heart, PLAN switches to Fury IN PROSE (no recoded
        // CHOICE). Wave-23 rework: a PROSE-only reversal that never re-issues a
        // coded CHOICE line is now trusted at the coded index (deck133 ruling:
        // "where one in-range coded index is sustained, INDEX-WINS trusts it").
        vector<string> slivers;
        slivers.push_back("Cast Spinneret Sliver {1}{g} (2/2)");
        slivers.push_back("Cast Fury Sliver {5}{r} (3/3)");
        slivers.push_back("Cast Heart Sliver {1}{r} (1/1)");
        slivers.push_back("Cast nothing right now");
        string proseSwitch =
            "CHOICE: 3 (Cast Heart Sliver)\n"
            "I will follow the plan and cast Heart Sliver to add a creature and hope for the best. Actually, no. The strategy guide says attack every turn; I must maximize damage now and Fury Sliver deals more.\n"
            "PLAN: Cast Fury Sliver to add a 3/3 double strike creature to my board, then attack with all four creatures for lethal.\n";
        int r3 = RESOLVE(proseSwitch, 4, slivers);
        cout << "     deck35 vs62 s18 (prose-only switch, no recoded CHOICE) -> " << r3 << " (INDEX-WINS: trust 3)\n";
        CHECK(r3 == 3, "N9 rework: a prose-only reversal with no recoded CHOICE is trusted, not retracted");

        // Guard: the wave-20 abut shape (generic phrase, single sustained index)
        // no longer retracts either - it is generic self-correction, not a coded
        // contradiction or a payability disavowal.
        string abut = "CHOICE: 1 (Play Forest)\nWait, I made a mistake. Hold the land instead.\n";
        int r4 = RESOLVE(abut, 2, land);
        cout << "     generic phrase abutting a sustained CHOICE -> " << r4 << " (INDEX-WINS: trust 1)\n";
        CHECK(r4 == 1, "N9 rework: generic self-correction abutting a sustained coded index is trusted");
    }

    // ---- WAVE-23 N9 residual: the three real deck133 replies (matchups-20260724-013710) ----
    cout << "\n[N9-W23/24] contradictory-coded-index (a) fires; payability prose does NOT ((b) retired); sub-point Correction does NOT\n";
    {
        // deck133 vs140 s9 (real, GENUINE - (a)): line-1 "CHOICE: 2 (Decline)",
        // then the reasoning oscillates and buries "So CHOICE: 1" in prose. The
        // resolved line-leading answer (2) is contradicted by a later coded index
        // (1) -> retract. (Excerpt preserves the two coded tokens + order.)
        vector<string> bg; bg.push_back("Put in Play"); bg.push_back("Decline - do nothing");
        string genuine =
            "CHOICE: 2 (Decline - do nothing)\n"
            "According to your strategy guide you should prioritize Gray Merchant. "
            "But wait, I have 3 Swamps, Gray Merchant costs 5, I cannot cast him. "
            "The best sequence: cast Bloodghast, then Inquisition.\n"
            "So CHOICE: 1.\n"
            "PLAN: Cast Bloodghast to build the board and devotion, then Inquisition.\n";
        int g9 = RESOLVE(genuine, 2, bg);
        cout << "     deck133 vs140 s9 (line-1 CHOICE 2, prose CHOICE 1) -> " << g9 << " (must be -1: (a) contradictory index)\n";
        CHECK(g9 == -1, "N9-W23 (a): a prose coded index contradicting the resolved answer retracts (genuine s9)");

        // deck133 vs137 s26 (real, ENGINE-CAUSED): branch (b) originally retracted
        // this. Branch (b) is RETIRED (wave-24 corpus: 131 live false positives -
        // the phrase set matched hypothetical rules prose and the reference gate
        // passed on common words). The engine cause (unpayable offer) is fixed at
        // the root by SacrificeCost::canPay, so this shape can no longer occur
        // legitimately; the sustained coded index is now TRUSTED.
        vector<string> yaw; yaw.push_back("-1/-1 Counter with Yawgmoth, Thran Physician targeting Venerated Loxodon [cost: Life, Sacrifice]");
        string unpayable =
            "CHOICE: 1 (-1/-1 Counter with Yawgmoth, Thran Physician targeting Venerated Loxodon)\n"
            "The prompt says 'Sacrifice another creature'. I have no other creatures on the battlefield. "
            "My Bloodghast is in the graveyard. I cannot sacrifice a creature I don't control, and I cannot pay the cost. "
            "But the game lists it as legal, so I will choose 1.\n"
            "PLAN: Activate Yawgmoth to place a -1/-1 counter and draw a card.\n";
        int u26 = RESOLVE(unpayable, 1, yaw);
        cout << "     deck133 vs137 s26 (sole option, payability prose, branch (b) retired) -> " << u26 << " (must be 1: index trusted)\n";
        CHECK(u26 == 1, "N9-W24: payability prose no longer retracts a sustained coded index ((b) retired)");

        // wave-24 corpus LIVE over-fire (deck137 vs Liliana targeting ask, one of
        // 131): sustained "CHOICE: 2 (The opponent)", hypothetical rules prose
        // "if they have no creatures they can't sacrifice" with "opponent"
        // nearby. The retired branch (b) fired on this constantly; the index
        // must be trusted.
        vector<string> lil; lil.push_back("You (player, life 14)"); lil.push_back("The opponent (player, life 19)");
        string hypo =
            "CHOICE: 2 (The opponent)\n"
            "Using Liliana of the Veil's -2 ability to force a sacrifice is the correct play. "
            "if they have no creatures they can't sacrifice, but the prompt implies a choice is needed. "
            "Ah, the Giant Killer is on the battlefield! Targeting the opponent forces them to sacrifice it.\n"
            "PLAN: Clear their blocker, then attack with Bloodghast.\n";
        int lv = RESOLVE(hypo, 2, lil);
        cout << "     wave-24 live FP shape (hypothetical 'can't sacrifice' prose) -> " << lv << " (must be 2)\n";
        CHECK(lv == 2, "N9-W24 regression: hypothetical payability prose never retracts (the 131-FP shape)");

        // deck133 vs137 s29 (real, OVER-FIRE the rework KILLS): a single sustained
        // "CHOICE: 1 (Play Polluted Delta)", a "*Correction:*" about Bloodghast's
        // haste (a reasoning sub-point), a bare "2/1 attacker" number. No second
        // coded index, no payability disavowal of the LAND play -> must NOT fire.
        vector<string> pd; pd.push_back("Play Polluted Delta"); pd.push_back("Decline - do nothing");
        string subpoint =
            "CHOICE: 1 (Play Polluted Delta)\n"
            "Playing Polluted Delta fetches a Swamp and triggers Bloodghast, "
            "returning it as a 2/1 attacker with haste (opponent at 18 life). "
            "*Correction:* Bloodghast's haste is only at 10 or less; opponent is at 18, "
            "so it returns with summoning sickness. Still huge - it adds devotion.\n"
            "PLAN: Play Polluted Delta, fetch a Swamp, return Bloodghast, then Gray Merchant next turn.\n";
        int s29 = RESOLVE(subpoint, 2, pd);
        cout << "     deck133 vs137 s29 (sub-point Correction, sustained CHOICE 1) -> " << s29 << " (must be 1: no over-fire)\n";
        CHECK(s29 == 1, "N9-W23: a sub-point 'Correction:' with a sustained coded index does NOT retract (s29 over-fire killed)");
    }

    // ---- ITEM 2 (stale-echo family A): spell-name-prefix on a TARGET echo ----
    cout << "\n[A2] target menu: strip '<spell> targeting ' prefix anchored to the pending source\n";
    {
        // deck14 vs27 s29 (real): "Unsummon targeting Inkfathom Infiltrator";
        // N=1 is the correct target but the spell prefix + rules-text words
        // made both echo passes miss -> stale_echo. Source = "Unsummon".
        vector<string> tgt;
        tgt.push_back("Inkfathom Infiltrator (3/2) [islandwalk, unblockable, cantblock] [opponent's battlefield] [tapped] - \"Inkfathom Infiltrator can't block and is unblockable.\"");
        tgt.push_back("Lord of Atlantis (2/2) [opponent's battlefield] - \"Other Merfolk creatures get +1/+1 and have islandwalk.\"");
        tgt.push_back("Zombie Master (2/3) [opponent's battlefield] - \"Other Zombie creatures have swampwalk.\"");
        string src = "Unsummon";
        bool st = false;
        int c = parseChoice("1 (Unsummon targeting Inkfathom Infiltrator)", 3, &tgt, &st, &src);
        cout << "     with source 'Unsummon' -> " << c << " (must be 1, was stale_echo)\n";
        CHECK(c == 1 && !st, "A2 deck14 s29: spell-prefix stripped, target name matches option 1");
        // INDEX-WINS (wave-23): even WITHOUT the source hint (so the spell
        // prefix is not stripped), the in-range index 1 is trusted because the
        // echoed label still names option 1's target ("Inkfathom Infiltrator").
        // The wave-22 stale_echo drop here is superseded by the root fix.
        bool st0 = false;
        int c0 = parseChoice("1 (Unsummon targeting Inkfathom Infiltrator)", 3, &tgt, &st0, NULL);
        cout << "     without source -> " << c0 << " (INDEX-WINS: trust the index)\n";
        CHECK(c0 == 1 && !st0, "A2 INDEX-WINS: unstripped prefix echo still trusts the in-range index whose target matches option 1");

        // deck62 vs14 s29 (real): "Web targeting Yavimaya Enchantress"; N=4 correct.
        vector<string> tgt2;
        tgt2.push_back("Birds of Paradise (0/1) [trample, flying] [your battlefield] [tapped]");
        tgt2.push_back("Saproling (1/1) [trample] [your battlefield]");
        tgt2.push_back("Saproling (1/1) [trample] [your battlefield]");
        tgt2.push_back("Yavimaya Enchantress (4/4) [trample] [your battlefield] - \"Yavimaya Enchantress gets +1/+1 for each enchantment on the battlefield.\"");
        tgt2.push_back("Saproling (1/1) [trample] [your battlefield]");
        tgt2.push_back("Saproling (1/1) [trample] [your battlefield]");
        tgt2.push_back("Coral Merfolk (2/1) [opponent's battlefield] [tapped]");
        string src2 = "Web";
        bool st2 = false;
        int c2 = parseChoice("4 (Web targeting Yavimaya Enchantress)", 7, &tgt2, &st2, &src2);
        cout << "     with source 'Web' -> " << c2 << " (must be 4)\n";
        CHECK(c2 == 4 && !st2, "A2 deck62 s29: 'Web targeting' stripped, Yavimaya matches option 4");

        // Anchor guard + INDEX-WINS: the "X targeting Y" strip is still anchored
        // (a mismatched prefix "Boomerang" vs source "Unsummon" is NOT stripped -
        // no blanket 'targeting' strip). But under INDEX-WINS the outcome is now
        // the CORRECT index 1: the echoed label names option 1's target
        // ("Inkfathom Infiltrator"), so the well-formed in-range index is trusted
        // regardless of the wrong spell name. (Pre-INDEX-WINS this dropped stale.)
        bool st3 = false;
        int c3 = parseChoice("1 (Boomerang targeting Inkfathom Infiltrator)", 3, &tgt, &st3, &src);
        cout << "     mismatched source ('Boomerang' vs 'Unsummon') -> " << c3 << " (INDEX-WINS: index names option 1's target)\n";
        CHECK(c3 == 1 && !st3, "A2 INDEX-WINS: a mismatched spell prefix still trusts the in-range index whose target matches option 1");
    }

    // ---- WAVE-23 X-menu cap header: the ANNOUNCE_X ask states the affordable cap ----
    cout << "\n[W23-X] ANNOUNCE_X header states the mana cap + reason (deck140 vs27 s46)\n";
    {
        // capX = highest affordable X (option index == X value). The header must
        // state the cap so the model cannot plan an unaffordable X (s46: reasoned
        // X=20 "lethal" while the engine offered at most X=6).
        string h6 = announceXHeader("Black Sun's Zenith", 6);
        cout << "     header(cap=6): " << h6 << "\n";
        CHECK(h6.find("X up to 6") != string::npos, "W23-X header states the numeric cap");
        CHECK(h6.find("Black Sun's Zenith") != string::npos, "W23-X header names the spell");
        CHECK(h6.find("above 6") != string::npos, "W23-X header warns against planning above the cap");
        CHECK(h6.find("LARGEST X (X = 6)") != string::npos, "W23-X header ties option 1 to the cap value");
        // A different cap threads the same value everywhere (data-driven, no hardcode).
        string h2 = announceXHeader("Rakdos's Return", 2);
        CHECK(h2.find("X up to 2") != string::npos && h2.find("(X = 2)") != string::npos,
              "W23-X cap value is data-driven, not hardcoded");
    }

    // ---- WAVE-23 decode-collapse detector: retry the garbage shape, never normal replies ----
    cout << "\n[W23-G] isDecodeGarbage: the deck27 vs137 s12-14 collapse shapes fire; normal replies do NOT\n";
    {
        // s12 (real shape): '**' + whitespace repetition, ASCII, near-zero prose.
        string g12; for (int i = 0; i < 250; i++) g12 += "\n\n    **";
        cout << "     s12 (** repetition, len " << g12.size() << ") -> " << AIPlayerGPT::isDecodeGarbage(g12) << "\n";
        CHECK(AIPlayerGPT::isDecodeGarbage(g12), "W23-G s12 shape (** repetition, no prose) is decode-garbage");
        // s13 (real shape): '**' interleaved with U+FFFD collapse bytes.
        string g13; for (int i = 0; i < 250; i++) g13 += "\xef\xbf\xbd**\n\n";
        CHECK(AIPlayerGPT::isDecodeGarbage(g13), "W23-G s13 shape (U+FFFD ** repetition) is decode-garbage");
        // s14 (real shape): '### N.' inside ``` code fences, repeated.
        string g14; for (int i = 0; i < 200; i++) g14 += "```\n### 0.\n```\n";
        CHECK(AIPlayerGPT::isDecodeGarbage(g14), "W23-G s14 shape (### fenced repetition) is decode-garbage");

        // NORMAL replies must NOT trigger (retry only on the collapse shape):
        // (1) a long English reply WITH a coded line (the 12k mana-rederivation class).
        string normLong = "CHOICE: 2 (Cast Damnation)\n";
        for (int i = 0; i < 60; i++)
            normLong += "I re-derive my mana: three Swamps make black, and I need double black for the sweeper this turn.\n";
        CHECK(!AIPlayerGPT::isDecodeGarbage(normLong), "W23-G a long prose reply WITH a coded line is not garbage");
        // (2) a long English reply with NO coded line (truncated reasoning) - NEVER retry.
        string normNoCode;
        for (int i = 0; i < 40; i++)
            normNoCode += "I weigh attacking versus holding my creatures back given the opposing board and my open mana.\n";
        cout << "     ordinary unparsed prose (no coded line, len " << normNoCode.size() << ") -> " << AIPlayerGPT::isDecodeGarbage(normNoCode) << " (must be 0)\n";
        CHECK(!AIPlayerGPT::isDecodeGarbage(normNoCode), "W23-G an ordinary long unparsed prose reply is NOT garbage (no retry)");
        // (3) short replies are never garbage.
        CHECK(!AIPlayerGPT::isDecodeGarbage("**"), "W23-G a short reply is not garbage");
        // (4) a coded ATTACK line present -> not garbage even amid trailing markup noise.
        string atk = "ATTACK: A1, A2, A3\n"; for (int i = 0; i < 200; i++) atk += "**\n";
        CHECK(!AIPlayerGPT::isDecodeGarbage(atk), "W23-G a coded ATTACK line present -> not garbage");
    }

    // ==================== WAVE-23 items (ITEM A: INDEX-WINS echo root fix, +
    // absent-card bookend; ITEM B: answer-first ATTACK over a CoT same-label
    // line). Cases built from the real matchups-20260723-173843 corpus replies.
    cout << "\n[W23-A] INDEX-WINS: trust an in-range index whose parenthetical name mismatches\n";
    {
        // Shape 1 - ANNOUNCE_X child menu (deck140 vs102 s12, real reply). The
        // bare "X = N" options carry no anchor words; the echo "Cast Black Sun's
        // Zenith with X=1" names the SOURCE spell. With the source threaded
        // (chooseMenuAction now passes it), INDEX-WINS trusts index 1.
        vector<string> xopts; xopts.push_back("X = 1"); xopts.push_back("X = 0");
        string bszSrc = "Black Sun's Zenith";
        bool sx = false;
        int cx = parseChoice("1 (Cast Black Sun's Zenith with X=1)", 2, &xopts, &sx, &bszSrc);
        cout << "     ANNOUNCE_X 'Cast <spell> with X=1' vs 'X = 1' -> " << cx << " (must be 1)\n";
        CHECK(cx == 1 && !sx, "W23-A shape1: ANNOUNCE_X source-echo trusts the in-range index (was stale_echo)");
        // Without the source the bare "X = N" options still have no shared word,
        // so this correctly stays stale (no false accept) - the source thread is
        // what makes it safe, exactly why chooseMenuAction now passes it.
        bool sx0 = false;
        int cx0 = parseChoice("1 (Cast Black Sun's Zenith with X=1)", 2, &xopts, &sx0, NULL);
        cout << "     ANNOUNCE_X without source -> " << cx0 << " (stale without the anchor; heuristic max-X)\n";
        CHECK(cx0 < 0 && sx0, "W23-A shape1: without the source anchor the bare-X echo is (safely) stale");

        // Shape 2 - transform dual-face (deck135 vs27 s27, real). Echo names the
        // PRE-transform face; option names the POST-transform face. They share
        // "azcanta"/"transform", so INDEX-WINS trusts index 1.
        vector<string> tf;
        tf.push_back("Transform:azcanta, the sunken ruin [available NOW - this transform is only offered because its condition is already met; do not recount, it is legal this instant]");
        tf.push_back("Decline - do nothing");
        bool s2 = false;
        int c2b = parseChoice("1 (Transform: Search for Azcanta)", 2, &tf, &s2, NULL);
        cout << "     transform dual-face 'Search for Azcanta' vs 'Transform:azcanta...' -> " << c2b << " (must be 1)\n";
        CHECK(c2b == 1 && !s2, "W23-A shape2: transform dual-face echo trusts the index (shares 'azcanta'/'transform')");

        // Shape 3 - DFC invented verb (deck102 vs109 s42, real). Echo "Activate
        // Tergrid's Lantern" vs option "Flip Side with Tergrid's Lantern"; they
        // share "Tergrid's Lantern", so INDEX-WINS trusts index 1 (activate).
        vector<string> dfc;
        dfc.push_back("Flip Side with Tergrid's Lantern {card text: \"{T}: Target player loses 3 life unless they sacrifice a nonland permanent or discard a card.\"}");
        bool s3 = false;
        int c3b = parseChoice("1 (Activate Tergrid's Lantern)", 1, &dfc, &s3, NULL);
        cout << "     DFC invented-verb 'Activate ...' vs 'Flip Side with ...' -> " << c3b << " (must be 1)\n";
        CHECK(c3b == 1 && !s3, "W23-A shape3: DFC invented-verb echo trusts the index (shares 'Tergrid's Lantern')");

        // TRUE ECHO that must STILL be caught: a prior land drop ("Play Forest")
        // echoed into a cast menu; the index maps to a DIFFERENT KIND of option
        // (a cast) foreign to the echoed card -> genuinely stale -> heuristic.
        vector<string> cast;
        cast.push_back("Cast Damnation {2}{b}{b}");
        cast.push_back("Cast Pyroclasm {1}{r}");
        cast.push_back("Cast nothing right now");
        bool se = false;
        int ce = parseChoice("2 (Play Forest)", 3, &cast, &se, NULL);
        cout << "     TRUE echo 'Play Forest' in a cast menu -> " << ce << " (must be -1: stale)\n";
        CHECK(ce < 0 && se, "W23-A true-echo: a foreign land-drop echo is STILL dropped stale (index-wins does not blindly trust)");
        // And with a source that does NOT account for it, still stale.
        string dmSrc = "Damnation";
        bool se2 = false;
        int ce2 = parseChoice("2 (Play Forest)", 3, &cast, &se2, &dmSrc);
        cout << "     TRUE echo with unrelated source -> " << ce2 << " (must be -1)\n";
        CHECK(ce2 < 0 && se2, "W23-A true-echo: an unrelated source does not rescue a foreign echo");
    }

    cout << "\n[W23-A2] absent-card bookend: a clean sibling CHOICE line beats a hallucinated middle one\n";
    {
        // deck140 vs102 s9 (real): the only option is the Elixir; the model's
        // real answer is "CHOICE: 0 (pass)" on line 1, but a middle "CHOICE: 1
        // (Cast Black Sun's Zenith with X=1)" names a sorcery NOT offered in the
        // upkeep. salvageLoopedChoice walks both lines: the absent-card line
        // re-parses to stale (-1) and is skipped, the pass survives.
        vector<string> el; el.push_back("Life with Elixir of Immortality [cost: {2}, Tap]");
        string reply =
            "CHOICE: 0 (pass)\n"
            "Reasoning: I cannot pay {2} generic for the Elixir.\n"
            "CHOICE: 1 (Cast Black Sun's Zenith with X=1)\n"
            "Wait, the phase is Upkeep; I cannot cast a sorcery. So the answer is CHOICE: 0 (pass).\n";
        int sal = salvageLoopedChoice(reply, 1, &el);
        cout << "     bookend salvage (0 pass / absent 1 / 0 pass) -> " << sal << " (must be 0: pass)\n";
        CHECK(sal == 0, "W23-A2 bookend: the hallucinated absent-card CHOICE is skipped; the pass is recovered");
        // The middle absent-card line, parsed alone, is stale (its name matches
        // no option and is foreign to the Elixir option).
        bool sb = false;
        int cb = parseChoice("1 (Cast Black Sun's Zenith with X=1)", 1, &el, &sb, NULL);
        cout << "     absent-card line alone -> " << cb << " (must be -1: stale)\n";
        CHECK(cb < 0 && sb, "W23-A2 the absent-card CHOICE alone is stale (foreign to the only option)");
    }

    cout << "\n[W23-B] answer-first ATTACK: the FIRST declaration beats a later CoT 'Attack:' line\n";
    {
        // deck109 vs62 s21 (real): line-1 "ATTACK: A1, A2, A3, A4" (all eligible)
        // was overridden by a line-179 CoT "Attack: Deal 1, Take 5. Net -4 life.
        // Opponent -1 life." whose bare prose numbers parsed to a bogus A1+A4.
        // Answer-first (first usable ATTACK: line) fixes it; last-wins was the bug.
        vector<string> anames;
        anames.push_back("Goblin"); anames.push_back("Goblin");
        anames.push_back("Ash Zealot"); anames.push_back("Legion Loyalist");
        string reply =
            "ATTACK: A1, A2, A3, A4\n"
            "Reasoning: with Legion Loyalist my team gets first strike and trample...\n"
            "Attack: Deal 1, Take 5. Net -4 life. Opponent -1 life.\n"
            "I will declare no attackers. Wait, the guide says attack every turn...\n";
        // strip think (none here) then collect ATTACK: lines in reply order
        vector<string> lines;
        collectLabeledLines(reply, "ATTACK:", lines);
        cout << "     collected ATTACK: lines = " << lines.size() << " (line1 + CoT decoy)\n";
        CHECK(lines.size() == 2, "W23-B two same-label lines collected (declaration + CoT decoy)");
        // Answer-first: FIRST usable declaration wins -> all 4.
        int afResult = -1; vector<bool> afSend;
        for (size_t li = 0; li < lines.size(); li++)
        {
            vector<bool> s;
            int r = parseAttackerSet(lines[li], 4, s, &anames);
            if (r >= 0) { afSend = s; afResult = r; break; }
        }
        cout << "     answer-first -> count=" << afResult
             << " send=[" << (afSend.size()>0?(int)afSend[0]:-1) << (afSend.size()>1?(int)afSend[1]:-1)
             << (afSend.size()>2?(int)afSend[2]:-1) << (afSend.size()>3?(int)afSend[3]:-1) << "] (must be 4, all)\n";
        CHECK(afResult == 4 && afSend[0] && afSend[1] && afSend[2] && afSend[3],
              "W23-B answer-first ATTACK takes line-1 A1,A2,A3,A4 (not the CoT line)");
        // Document the bug: the LAST line (CoT decoy) parses to the bogus 2-subset.
        vector<bool> bug;
        int bugR = parseAttackerSet(lines[lines.size() - 1], 4, bug, &anames);
        cout << "     last-wins (bug) -> count=" << bugR
             << " send=[" << (int)bug[0] << (int)bug[1] << (int)bug[2] << (int)bug[3] << "] (the CoT numbers 1,-4 -> A1,A4)\n";
        CHECK(bugR == 2 && bug[0] && !bug[1] && !bug[2] && bug[3],
              "W23-B bug repro: the CoT 'Attack: Deal 1... -4 life' line parses to a bogus A1,A4 under last-wins");
    }

    // ---- ITEM 3 (stale-echo family B): CoT answer-label line hijacks the answer --
    cout << "\n[B3] a CoT 'Attack:' line in a CHOICE reply must not be taken as the answer\n";
    {
        // deck49 vs35 s18 (real): valid answer-first CHOICE 1, then the combat
        // math line "Attack: Regent (6/6) vs Striking Sliver (2/2, ...)" as CoT.
        vector<string> opts3;
        opts3.push_back("Cast Goblin War Paint {1}{r} - legal targets right now: Dragonmaster Outcast, Firespitter Whelp, Thunderbreak Regent, Muscle Sliver (+1 more)");
        opts3.push_back("Cast nothing right now");
        string s18 =
            "CHOICE: 1 (Cast Goblin War Paint)\n"
            "The opponent has two 2/2 creatures with First Strike.\n"
            "Attack: Regent (6/6) vs Striking Sliver (2/2, First Strike). Striking deals 2 to Regent. Regent deals 6 to Striking.\n"
            "PLAN: Cast Goblin War Paint on Thunderbreak Regent to make it a 6/6 flyer.\n";
        size_t ss = 0, se = 0, ls = 0;
        bool okF = findAnswerLabelLine(s18, "CHOICE:", ss, se, ls);
        string seg = okF ? s18.substr(ss, se - ss) : string();
        bool st = false;
        int c = parseChoice(seg, 2, &opts3, &st);
        cout << "     expectedLabel 'CHOICE:' seg=\"" << seg << "\" -> " << c << " (must be 1)\n";
        CHECK(okF && c == 1 && !st, "B3 CHOICE-filtered selection keeps the real answer-first CHOICE 1");
        // Bug repro: the unfiltered (any-label) scan grabs the later 'Attack:' line.
        size_t bs = 0, be = 0, bl = 0;
        bool okB = findAnswerLabelLine(s18, NULL, bs, be, bl);
        string segBug = okB ? s18.substr(bs, be - bs) : string();
        bool stBug = false;
        int cBug = parseChoice(segBug, 2, &opts3, &stBug);
        cout << "     unfiltered picks the CoT line seg=\"" << segBug << "\" -> " << cBug << " (bug: stale)\n";
        CHECK(cBug < 0, "B3 bug repro: any-label scan hands parseChoice the CoT 'Attack:' prose -> dropped");
    }

    #undef RESOLVE

    // ---- Pre-game London BOTTOM-N reply parse (reuses the reveal PUT parser) ----
    cout << "\n[bottom] London mulligan 'PUT: 1, 3' bottoms exactly those cards; salvage from a spiral\n";
    {
        // 5-card hand; the model was asked to bottom N=2 -> "PUT: 1, 3".
        vector<string> names;
        names.push_back("Mountain"); names.push_back("Lightning Bolt");
        names.push_back("Shivan Dragon"); names.push_back("Island"); names.push_back("Counterspell");
        vector<bool> send;
        int r = parseAttackerSet("PUT: 1, 3", names.size(), send, &names);
        cout << "     'PUT: 1, 3': count=" << r
             << " picks=[" << (send.size()>0?(int)send[0]:-1) << ","
             << (send.size()>2?(int)send[2]:-1) << "]\n";
        CHECK(r == 2 && send[0] && !send[1] && send[2] && !send[3] && !send[4],
              "bottom: 'PUT: 1, 3' bottoms cards 1 and 3, only those");
        // Name form: the model bottomed by name instead of number.
        vector<bool> send2;
        int r2 = parseAttackerSet("PUT: Shivan Dragon", names.size(), send2, &names);
        cout << "     'PUT: Shivan Dragon': count=" << r2
             << " picks card3=" << (send2.size()>2?(int)send2[2]:-1) << "\n";
        CHECK(r2 == 1 && !send2[0] && !send2[1] && send2[2],
              "bottom: name-form 'PUT: Shivan Dragon' resolves to card 3");
        // Decode-spiral salvage: recover the earlier well-formed PUT: line.
        vector<bool> send3;
        int sal = salvageLoopedSubset(
            "PLAN: keep the cheap curve.\nPUT: 3\nhmm hmm hmm hmm hmm hmm\nblah",
            "PUT:", names.size(), names, send3);
        cout << "     spiral salvage: count=" << sal
             << " picks card3=" << (send3.size()>2?(int)send3[2]:-1) << "\n";
        CHECK(sal == 1 && send3[2] && !send3[0], "bottom: salvage recovers 'PUT: 3' from a spiral");
    }

    cout << "\n=== self-test: " << passed << " passed, " << failed << " failed ===\n";
    cout.flush();
    #undef CHECK
}

//Free-function entry so the JGE layer's main() can trigger the self-test
//without including the mtg-heavy AIPlayerGPT header.
void wagicGptParseSelfTest()
{
    AIPlayerGPT::runParseSelfTest();
}

#endif //WITH_GPT_AI
