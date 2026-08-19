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
#include "AbilityParser.h" //N-158m: AutoLineMacro::Process, for macro-defined magnitudes
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
#include "PreGamePhase.h" //PreGamePhase::bottomTarget, driven by the self-test

#ifndef WAGIC_NO_CURL
#include <curl/curl.h>
#endif
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
#ifdef _WIN32
#include <direct.h>
#define GPT_MKDIR(p) _mkdir(p) //Windows mkdir takes no mode argument
#else
#define GPT_MKDIR(p) mkdir((p), 0755)
#endif
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
    "creatures cannot block, but a SUMMONING-SICK creature CAN block - summoning sickness restricts "
    "attacking and {T} abilities only. Combat damage is dealt simultaneously; a creature dies if damage reaching it "
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

//The de-fanged worked-example card name (wave-25 ITEM 1, part 3). The example
//in kReplyProtocol MUST use an obviously-FAKE card that can never collide with a
//real option and MUST NOT reuse a real option's card name. The old example,
//`CHOICE: 2 (Cast Fatal Push)`, poisoned the parsers two ways: (1) when a real
//option WAS "Cast Fatal Push" (deck133 vs131 s21), the model quoted the example
//obsessively while debating output format, and the retraction detector's coded-
//index scan latched the quoted "CHOICE: 2" as a contradictory second index -> a
//false retraction; (2) the concrete index 2 seeded value-as-index slips into
//real asks (deck102 vs133 s5, index 2 into a 1-option ask). A fake name defuses
//both: a quoted/echoed "(Cast Example Card)" matches NO real option, so the
//name-echo cannot validate a wrong index, and the retraction scan's example-echo
//guard recognizes it. `kExampleFakeCardLc` is the lowercase form that guard
//matches. The FORMAT is stated index-agnostically ("<number> (<action name>)")
//so the example's own digit is presented as illustrative, not a value to copy.
const char * kExampleFakeCardLc = "example card";

//The reply protocol is appended in code AFTER the (user-editable) system
//prompt template, so a stale or hand-edited template cannot silently drop
//the contract the parsers and the plan carry-forward depend on.
const char * kReplyProtocol =
    "\nHOW TO REPLY (every decision):\n"
    "Your reply is ONE line, or TWO when your plan changed. Nothing else.\n"
    "LINE 1 is your ANSWER, using exactly the label the decision asks for (CHOICE: for numbered "
    "choices, ATTACK: for attack declarations, BLOCKS: for block assignments). Format: the label, "
    "then the NUMBER of your choice FROM THE LIST, then that option's name in parentheses - "
    "CHOICE: <number> (<action name exactly as listed>). For example, \"CHOICE: 3 (Cast Example "
    "Card)\" (Example Card is a placeholder - always copy the real number and name from the options "
    "in front of you, never this example's).\n"
    "LINE 2, ONLY IF your plan has changed: a line starting with PLAN:, holding your complete game "
    "plan from here on - CONCISE, a few sentences of intent, not an analysis. If your plan is "
    "unchanged, OMIT the PLAN line entirely - your last stated plan carries forward automatically "
    "and will be shown to you again.\n"
    "Write no reasoning, no commentary, no restatement of the board and no working in the reply "
    "itself: think the decision through BEFORE you answer, then give only the answer. (If you do "
    "write more than one answer line, the LAST well-formed answer line is the one taken.)\n"
    "Nothing you write is kept except that PLAN line. At your next decision you will see only the "
    "game log, the current board, your last PLAN line, and the new choices - your thinking and "
    "your earlier plans will have dropped out of context. So every PLAN you do write must be "
    "complete and self-contained: state your full current plan, or your full revised plan if the "
    "situation changed. Never write a fragment like \"continue as before\".\n";

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
//N-158a/N-158c (wave-31 deck158): AMASS. The amass macro puts N +1/+1 counters
//on an Army you control, creating a 0/0 Orc Army token first if you control
//none - and everything the card then does keys off the POST-amass Army. Two
//defects fell out of not modelling that: the cast option carried no Army-size
//preview at all (the pilot re-derived it every single time, and this seat's
//top-2 reply-length spikes are those re-derivations), and "damage:power" was
//evaluated against the SOURCE - a sorcery, power 0 - so Foray of Orcs rendered
//"{right now: damage 0}" in 100% of cases while resolving for 2 and for 9. The
//pilot argued with the annotation out loud at 13.1k and 9.8k characters.
//Detect the macro from the script and report the count so both surfaces can use
//the real post-amass number. Returns 0 when the card does not amass.
//Pure over the script text so the real primitives can be replayed in PARSETEST.
static int amassCountersFromScript(const string& magicText)
{
    string mt = magicText;
    for (size_t i = 0; i < mt.size(); i++)
        mt[i] = (char) tolower((unsigned char) mt[i]);
    //ON-RESOLUTION ONLY. magicText is the auto= lines joined by '\n'
    //(CardPrimitive::addMagicText), and a line that opens with an '@' TRIGGER or
    //carries the _DIES_ macro amasses on some LATER event, not when the offered
    //option resolves: Grim Initiate amasses when it DIES, Saruman the White on
    //your second spell each turn, Lazotep Chancellor on a discard. Previewing
    //"Army 0/0 -> 1/1" on their cast line would assert something that is not
    //about to happen. Keep only the untriggered lines - Foray of Orcs and
    //Surrounded by Orcs (plain sorcery resolution) and Grishnakh (an ETB on the
    //creature being cast) all survive this filter.
    //  N-158m (wave-32, coverage 3 of 52 live offers = 5.8%): the scan below
    //looks for literal `counter(1/1...)` text, but a card may express its amass
    //through a MACRO (`_AMASSORC1_`), and macro expansion runs on a LOCAL COPY
    //inside the ability factory (MTGAbility.cpp:6650) that is never written back
    //to CardPrimitive::magicText. Every macro-defined amass card therefore
    //presented this scanner with an opaque token and scored 0 - Easterling
    //Vanguard 0/17, Mordor Muster 0/13, March from the Black Gate 0/9. Only
    //Foray of Orcs, whose script is spelled out by hand, ever previewed.
    //  Expansion happens PER LINE and AFTER the trigger filter has read the RAW
    //line, deliberately: `_DIES_` expands to `@movedTo(this|graveyard)`, so a
    //whole-string expansion first would have destroyed the `_dies_` half of the
    //suppression guard. Both the raw and the expanded line are checked for '@',
    //so neither spelling of a trigger can slip through. Expanding through the
    //engine's own AutoLineMacro::Process (rather than mapping the five amass
    //macro names) closes the CLASS: any later macro carrying a magnitude is
    //covered for free, and the render can never disagree with what the ability
    //factory actually built.
    {
        string kept;
        size_t p = 0;
        while (p <= mt.size())
        {
            size_t nl = mt.find('\n', p);
            string line = mt.substr(p, nl == string::npos ? string::npos : nl - p);
            bool triggered = (line.find('@') != string::npos)
                             || (line.find("_dies_") != string::npos);
            if (!triggered)
            {
                //Only pay for expansion on a line that could carry a macro token.
                if (line.find('_') != string::npos)
                    line = AutoLineMacro::Process(line);
                if (line.find('@') == string::npos)
                    kept += line + "\n";
            }
            if (nl == string::npos)
                break;
            p = nl + 1;
        }
        mt = kept;
        //The macro's two halves: retarget an existing Army, or create the 0/0
        //token. This gate reads the EXPANDED text - reading it before expansion
        //is precisely what made the scanner macro-blind.
        if (mt.find("army|mybattlefield") == string::npos)
            return 0;
    }
    //ATTRIBUTION GUARD, same shape as the N-146g planeswalker skip above: a
    //card-level scan cannot say WHICH sub-ability the offered option is. Several
    //amass cards enumerate one branch per X value in a single magicText -
    //Barad-dur spells out Amass Orcs 1 through 10, Fall of Cair Andros 1 through
    //20 - so taking the first (or any) literal would stamp a confidently WRONG
    //Army size onto an option that amasses something else. A wrong number here is
    //strictly worse than the "damage 0" this fix exists to remove. Emit ONLY when
    //the whole card agrees on one count; otherwise say nothing. An "x" amount is
    //excluded for free (atoi -> 0), matching the existing unknowable-X rule.
    std::set<int> values;
    for (size_t pos = 0; (pos = mt.find("counter(1/1.", pos)) != string::npos; pos += 12)
    {
        int n = atoi(mt.c_str() + pos + 12);
        if (n > 0)
            values.insert(n);
        else
            return 0; //an unresolved/variable amount: unknowable before announcement
    }
    //N-158m: the bare `counter(1/1)` form (amass 1) has to join the SAME set, not
    //sit in a fallback below it. Pre-expansion this could not co-occur with a
    //dotted form on one card, so the fallback was safe; macro expansion makes
    //`_AMASSORC1_` + `_AMASSORC3_` on one card reachable, and a fallback would
    //have reported "3" for a card that also amasses 1 - exactly the confidently
    //wrong number the attribution guard exists to prevent.
    if (mt.find("counter(1/1)") != string::npos)
        values.insert(1);
    if (values.size() == 1)
        return *values.begin();
    return 0; //nothing found, or a multi-branch card: unattributable here
}

static int amassCounters(MTGCardInstance * card)
{
    return card ? amassCountersFromScript(card->magicText) : 0;
}

//The Army this player already controls, or NULL. An Army token is created 0/0
//and grows only by counters, so its CURRENT power is the amass baseline.
static MTGCardInstance * findMyArmy(MTGCardInstance * card)
{
    if (!card || !card->controller() || !card->controller()->game)
        return NULL;
    MTGGameZone * bf = card->controller()->game->battlefield;
    if (!bf)
        return NULL;
    for (int i = 0; i < bf->nb_cards; i++)
        if (bf->cards[i] && bf->cards[i]->hasSubtype("army"))
            return bf->cards[i];
    return NULL;
}

string dynamicMagnitudes(MTGCardInstance * card)
{
    //N-146g (deck146 Lolth, deck152): a planeswalker's magicText bundles EVERY
    //loyalty ability plus the emblem/transforms payload in one string, so this
    //card-level scan cannot attribute a magnitude to the specific loyalty (or
    //cast) option being offered - it grabbed Lolth's -8 emblem expression
    //(damage:8minusoplifelostminusend) and stamped "{right now: damage 8}"
    //(later "damage 4") onto her CAST and her +0/-3 loyalty options, none of
    //which deal that damage. A walker's loyalty effects are plain-numeric
    //loyalty costs already visible in the {card text} snippet, so an unscopable
    //number this pass evaluates from a NON-offered sub-ability is worse than
    //none (same spirit as skipping "rand"). Skip walkers.
    if (card && card->hasType(Subtypes::TYPE_PLANESWALKER))
        return "";
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
    //Amass preview FIRST (it is also the baseline every other magnitude on this
    //card keys off). "Army 7/7 -> 9/9" is the whole arithmetic, done.
    int amassN = amassCounters(card);
    int amassResultP = 0;
    if (amassN > 0)
    {
        MTGCardInstance * army = findMyArmy(card);
        int curP = army ? army->power : 0;
        int curT = army ? army->toughness : 0;
        amassResultP = curP + amassN;
        out << "Army " << curP << "/" << curT << " -> " << amassResultP << "/"
            << (curT + amassN);
        //Creature type stays UNSPOKEN: the macro creates an Orc Army on Foray of
        //Orcs and a ZOMBIE Army on Widespread Brutality (both detected here), so
        //naming one would be wrong half the time. The card text on the same line
        //says which; the size is what the pilot could not compute.
        if (!army)
            out << " (a new 0/0 Army token is created first)";
        count++;
    }
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
            //N-158h (wave-31 deck158, ~44/44 renders wrong): "life:-manacost" on a
            //TARGETED spell means "the DESTROYED permanent's mana value", not the
            //spell's own - Feed the Swarm printed a constant "{right now: life -2}"
            //while the true self-cost ran -1 (Serra Ascendant), -2, -3 (Nadaar) and
            //0 (a token). The evaluator cannot know the target at option-build
            //time, and this deck finishes its wins at 3-15 life, so a 1-life error
            //is live. Drop the clause here; the per-target cost is folded into the
            //cast option's "legal targets right now:" list instead, where the
            //target IS known.
            if (expr.find("manacost") != string::npos && !card->spellTargetType.empty())
                continue;
            if (!seen.insert(string(kVerbs[v].key) + expr).second)
                continue;
            //An expression reading the SOURCE's own power/toughness is
            //unevaluable when the source is not a creature: a sorcery has power
            //0, and "damage 0" printed with authority is worse than no
            //annotation (owner-facing rule from the ledger). The amass case is
            //the one place the true number IS knowable - the post-amass Army's
            //power - so use it and suppress everywhere else.
            if ((expr == "power" || expr == "toughness") && !card->isCreature())
            {
                if (!(amassN > 0 && expr == "power"))
                    continue;
            }
            WParsedInt val(expr, NULL, card);
            int n = (amassN > 0 && expr == "power" && !card->isCreature())
                    ? amassResultP : val.getValue();
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

//N-105a part (d): the poison family's ENGINE keyword names are opaque to a
//reader - "poisontoxic" IS toxic 1, "poisondamager" IS "combat damage to a
//player becomes poison counters". A keyword the model cannot decode is not a
//rendered fact, so the family is translated at this ONE point, which every
//surface listing keywords goes through (attacker lines, blocker lines, target
//lines, the board snapshot). Every other keyword is returned byte-identical.
//Pure, so PARSETEST can prove both halves.
string legibleKeywordName(const string& engineName)
{
    if (engineName == "poisondamager")
        return "deals its damage to players as poison counters";
    //poisontoxic .. poisontentoxic -> "toxic N"
    if (engineName.size() > 10 && engineName.compare(0, 6, "poison") == 0
        && engineName.compare(engineName.size() - 5, 5, "toxic") == 0)
    {
        static const char * kWords[] = { "", "", "two", "three", "four", "five",
                                         "six", "seven", "eight", "nine", "ten" };
        string mid = engineName.substr(6, engineName.size() - 11);
        int n = 0;
        if (mid.empty())
            n = 1;
        else
            for (int w = 2; w <= 10; w++)
                if (mid == kWords[w]) { n = w; break; }
        if (n > 0)
        {
            std::ostringstream t;
            t << "toxic " << n;
            return t.str();
        }
    }
    return engineName;
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
        const char * kw = Constants::MTGBasicAbilities[j];
        out << (first ? "" : ", ") << legibleKeywordName(kw ? kw : "");
        first = false;
    }
    return out.str();
}

//N-36b (wave-32 deck36, HIGH, 3 emitters): a converted mana cost of ZERO is
//FALSY, so the guard `if (cost && cost->getConvertedCost())` deleted the cost
//token outright from every {0} card - 347 renders of three {0} cards at deck36
//and not one carried a "{0}", including the system-prompt decklist line. From
//that absence the pilot manufactured four distinct false rules ("they are not
//spells you can cast from your hand", "putting them onto the battlefield
//requires a land drop slot", "Moxes ... cost mana to cast") and declined a free
//permanent in 4 of its 7 cast-nothing windows. ONE helper for every cost
//emitter, so the shape cannot come back a fourth time.
//  ManaCost::toString() itself returns "" for a zero cost, so the synthesized
//"{0}" is emitted only when the real string is genuinely empty - an X-only cost
//(also converted-0) still renders as "{x}" through the string, not as "{0}".
//Lands and tokens are excluded: neither is cast for a mana cost, and "{0}" on a
//Forest would be a new false fact rather than a restored one.
//Pure core, so the {0} restoration is provable in PARSETEST without a card.
string manaCostTokenText(const string& costString, bool castForMana)
{
    if (!costString.empty())
        return " " + costString;
    return castForMana ? " {0}" : "";
}

string manaCostToken(MTGCardInstance * card)
{
    if (!card)
        return "";
    ManaCost * cost = card->getManaCost();
    if (!cost)
        return "";
    //carries its own braces; "" for a zero cost, "{x}" for an X-only cost
    return manaCostTokenText(cost->toString(), !card->isLand() && !card->isToken);
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

//The colours a LAND can produce, as a W/U/B/R/G flag array. Basic land types -
//including dual/tri lands that carry them (e.g. Underground Sea = subtype
//"Island Swamp") - map by subtype; other lands read produced colors from the
//auto script's "Add{...}" clauses (Karplusan Forest: Add{R}/Add{G}). Extracted from
//landTag so the pregame hand header (N-139n) counts colour sources from the
//SAME engine-read source the per-card tag prints - a count that disagreed with
//the tags beside it would be worse than no count at all.
void landColorFlags(MTGCardInstance * card, bool have[5])
{
    for (int i = 0; i < 5; i++)
        have[i] = false;
    if (!card || !card->isLand())
        return;
    static const char * kSub[5] = { "plains", "island", "swamp", "mountain", "forest" };
    static const int kIdx[5] = { 0, 1, 2, 3, 4 };
    for (int i = 0; i < 5; i++)
        if (card->hasSubtype(kSub[i]))
            have[kIdx[i]] = true;
    if (!(have[0] || have[1] || have[2] || have[3] || have[4]))
    {
        //Nonbasic without a basic land type: read produced colors from the auto
        //script's "Add{...}" mana clauses. Anchored on the '{' so an "add" inside
        //another word cannot trip it; generic "Add{1}" contributes no color.
        string mt = card->magicText;
        for (size_t k = 0; k < mt.size(); k++)
            mt[k] = (char) tolower((unsigned char) mt[k]);
        size_t pos = 0;
        while ((pos = mt.find("add", pos)) != string::npos)
        {
            size_t b = pos + 3;
            pos += 3;
            while (b < mt.size() && (mt[b] == ' ' || mt[b] == '\t'))
                b++;
            if (b >= mt.size() || mt[b] != '{')
                continue;
            for (b++; b < mt.size() && mt[b] != '}'; b++)
            {
                switch (mt[b])
                {
                    case 'w': have[0] = true; break;
                    case 'u': have[1] = true; break;
                    case 'b': have[2] = true; break;
                    case 'r': have[3] = true; break;
                    case 'g': have[4] = true; break;
                    default: break;
                }
            }
        }
    }
}

//A compact land-identity tag for the DECISION surfaces where a bare land name -
//especially a basic "Swamp" - carried no land/mana signal and the pilot
//miscounted lands as unplayable spells (deck93 wave-27: mulliganed the identical
//3-Swamp opening hand 5 of 6 games claiming "zero lands", once while writing
//"Swamps produce black mana"). Names that the card IS a LAND and the color(s) it
//taps for. Returns "" for a non-land. Leading space so it appends after a name.
string landTag(MTGCardInstance * card)
{
    if (!card || !card->isLand())
        return "";
    bool have[5];
    landColorFlags(card, have);
    static const char * kSym[5] = { "{W}", "{U}", "{B}", "{R}", "{G}" };
    string colors;
    for (int i = 0; i < 5; i++)
        if (have[i])
            colors += kSym[i];
    if (colors.empty())
        return " (land)";
    return " (land: taps for " + colors + ")";
}

//---- N-146k + N-139n: the PREGAME hand header ---------------------------------
//OWNER DIRECTIVE (2026-07-27): a pregame ask carries NO board-state information
//at all. His mechanism: "0 mana available reads as an INSTRUCTION that the hand
//doesn't have mana - the model follows the explicit instruction rather than
//deriving the real count." A rendered statement outranks the model's own
//derivation, so a board-scoped line in a prompt with no board is not noise, it
//is an active override. deck146 declared a land-rich hand landless in 5 of 14
//mulligan replies, echoing "Mana available: 0 total (no untapped sources)"
//almost word for word; deck139 and deck116 spiralled the same way.
//  N-139n is the other half. Per-item tags assert MEMBERSHIP, and membership is
//exactly what the pilot argues with: the guide layer fixed "zero lands" in
//wave-31 and the belief MUTATED to "zero GREEN mana sources" in wave-32 (6 false
//mulligans of 9, one of them reciting the guide's own list while holding a
//literal Forest). Only an engine-computed AGGREGATE closes it, so the counts
//below are computed here and are STRICTLY hand-derived - nothing on this surface
//refers to a battlefield, because at this point there isn't one.
//Pure core: every sentence of the pregame header, from tallies alone. Nothing
//here can name a battlefield, a life total, a phase or a mana pool, because
//none of those is an input - the no-board-state rule is enforced by the
//function's SIGNATURE rather than by remembering to filter.
string pregameHandHeaderText(int handSize, int lands, int spells, const int sources[5],
                             const string& cheapestLabel, int cheapestCmc,
                             const vector<string>& reachable)
{
    static const char * kSym[5] = { "{W}", "{U}", "{B}", "{R}", "{G}" };
    std::ostringstream o;
    o << "Your hand (" << handSize << " card" << (handSize == 1 ? "" : "s")
      << "), counted by the engine: " << lands << " land" << (lands == 1 ? "" : "s")
      << ", " << spells << " spell" << (spells == 1 ? "" : "s") << ".\n";
    o << "Mana sources among those lands, counted by the engine: ";
    if (!lands)
        o << "none - this hand holds no lands at all.";
    else
    {
        bool any = false;
        for (int k = 0; k < 5; k++)
            if (sources[k])
            {
                o << (any ? ", " : "") << kSym[k] << " " << sources[k];
                any = true;
            }
        if (!any)
            o << "no coloured sources (every land here makes only colourless mana)";
        o << ". A land in your hand IS a mana source: it produces mana from the turn"
             " you play it, so count these when you judge this hand.";
    }
    o << "\n";
    if (cheapestCmc >= 0)
        o << "Cheapest spell in this hand: " << cheapestLabel << " (mana value "
          << cheapestCmc << ").\n";
    if (!reachable.empty())
    {
        o << "Playing every land in this hand would cover the cost of: ";
        for (size_t i = 0; i < reachable.size(); i++)
            o << (i ? ", " : "") << reachable[i];
        o << ".\n";
    }
    else if (lands)
        o << "Playing every land in this hand would not cover any spell in it.\n";
    return o.str();
}

static string pregameHandHeader(MTGGameZone * hand)
{
    if (!hand)
        return "";
    //Index map from this file's W/U/B/R/G order to the engine's colour enum.
    static const int kEngineColor[5] = { Constants::MTG_COLOR_WHITE, Constants::MTG_COLOR_BLUE,
                                         Constants::MTG_COLOR_BLACK, Constants::MTG_COLOR_RED,
                                         Constants::MTG_COLOR_GREEN };
    int lands = 0, spells = 0;
    int sources[5] = { 0, 0, 0, 0, 0 };
    for (int i = 0; i < hand->nb_cards; i++)
    {
        MTGCardInstance * c = hand->cards[i];
        if (!c)
            continue;
        if (c->isLand())
        {
            lands++;
            bool have[5];
            landColorFlags(c, have);
            for (int k = 0; k < 5; k++)
                if (have[k])
                    sources[k]++;
        }
        else
            spells++;
    }
    //The castability summary, hand-only: what these lands could pay for if every
    //one of them were played. A NECESSARY condition (total lands cover the mana
    //value, and each coloured pip has at least that many lands able to make it),
    //stated as what it is so the pilot is never handed a claim it can later find
    //false. Capped at six names so a large hand cannot run away with the header.
    string cheapest;
    int cheapestCmc = -1;
    vector<string> reachable;
    for (int i = 0; i < hand->nb_cards; i++)
    {
        MTGCardInstance * c = hand->cards[i];
        if (!c || c->isLand())
            continue;
        ManaCost * mc = c->getManaCost();
        int cmc = mc ? mc->getConvertedCost() : 0;
        if (cheapestCmc < 0 || cmc < cheapestCmc)
        {
            cheapestCmc = cmc;
            cheapest = c->getDisplayName() + manaCostToken(c);
        }
        bool payable = (cmc <= lands);
        if (payable && mc)
            for (int k = 0; k < 5 && payable; k++)
                if (mc->getCost(kEngineColor[k]) > sources[k])
                    payable = false;
        if (payable && reachable.size() < 6)
            reachable.push_back(c->getDisplayName() + manaCostToken(c));
    }
    return pregameHandHeaderText(hand->nb_cards, lands, spells, sources,
                                 cheapest, cheapestCmc, reachable);
}

string instanceHandle(MTGCardInstance * card); //defined below; used by describeAttachments

//The auras/equipment attached to a permanent. There is no forward list, so
//find them the way the engine does (cf. MTGCardInstance::hasTotemArmor):
//every attachment carries a reverse pointer to its host - but the pointer is
//TYPE-SPLIT, and that split is what defeated the original scan (N-148a,
//wave-31 deck148: 112 reverse "[attached to: X]" renders and ZERO forward
//"{attached:" renders across six games, because EVERY attachment in that deck
//was Equipment). An AURA keeps its host in auraParent; an EQUIPMENT (and a
//fortification) keeps its host in target and is deliberately EXCLUDED from
//auraParent by the engine - the very fact the reverse render at the
//battlefield-line block already encodes. Match both, using the same type test
//the reverse block performs, so the relationship is answerable from either end.
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
            if (!att || att == host)
                continue;
            bool attached = (att->auraParent == host);
            if (!attached
                && (att->hasType(Subtypes::TYPE_EQUIPMENT) || att->hasType("fortification")))
                attached = (att->target == host);
            if (!attached)
                continue;
            out << (first ? " {attached: " : ", ") << att->getDisplayName()
                << instanceHandle(att);
            first = false;
        }
    }
    if (!first)
        out << "}";
}

//A stable per-instance handle for a battlefield permanent whose NAME collides
//with another permanent its controller has in play. A bare name cannot bind a
//board-snapshot entry to an offered A#/B#/target line: wave-26 deck137 s36 had
//TWO Lovestruck Beasts (one tapped, one untapped), the B-line said only
//"Lovestruck Beast", and the model resolved the collision against itself and
//declined a free kill-and-survive block (10.8k-char spiral). Give each colliding
//copy a "#N" suffix. N is the card's 1-based rank among same-named permanents on
//ITS controller's battlefield, in battlefield array order (i.e. enter/play
//order) - a deterministic function of the instance, so every surface (board
//snapshot, A/B option lines, target lines) prints the SAME number for the SAME
//physical card and an offered label binds unambiguously. Returns "" for a
//singleton name (no collision -> no noise) or a card not on its controller's
//battlefield. The name reconcile already reads a trailing "#N"
//(nameOrdinal/uniqueNameMatch), so a reply that answers by name+handle resolves
//too; the A#/B# label binding is positional and correct regardless of this
//decoration.
string instanceHandle(MTGCardInstance * card)
{
    if (!card)
        return "";
    Player * ctrl = card->controller();
    if (!ctrl || !ctrl->game)
        return "";
    MTGGameZone * bf = ctrl->game->battlefield;
    if (!bf)
        return "";
    int total = 0, rank = 0;
    for (int i = 0; i < bf->nb_cards; i++)
    {
        MTGCardInstance * o = bf->cards[i];
        if (!o || o->name != card->name)
            continue;
        total++;
        if (o == card)
            rank = total;
    }
    if (total < 2 || rank == 0)
        return ""; //singleton name, or card not on its controller's battlefield
    std::ostringstream h;
    h << " #" << rank;
    return h.str();
}

//Read the alnum/hyphen token at 'start' and, if it is a creature subtype, add
//its proper-cased form to 'out'. Helper for collectTribalTypes.
void addTribeToken(const string& mt, size_t start,
                   const std::map<string, string>& proper, std::set<string>& out)
{
    size_t e = start;
    while (e < mt.size() && (isalnum((unsigned char) mt[e]) || mt[e] == '-'))
        e++;
    if (e <= start)
        return;
    std::map<string, string>::const_iterator it = proper.find(mt.substr(start, e - start));
    if (it != proper.end())
        out.insert(it->second);
}

//Creature subtypes that some card in p's deck/battlefield mechanically CONSUMES
//- a count, anthem or gate keyed on that type: lord(other TYPE), foreach(other
//TYPE), aslongas(other TYPE), or a "[...&TYPE]" source filter (Calamity Bearer).
//These are the tribes for which a changeling's "counts as every creature type"
//actually changes a decision. Extracted from each card's raw auto script
//(magicText): a vanilla creature's own subtype lives on its subtype= line, NOT
//in magicText, so a creature-type word appearing in an "other TYPE"/"&TYPE"
//context pinpoints a genuine tribal consumer and skips incidental mentions
//(token makers etc.). Wave-26 deck22: Universal Automaton (a Changeling) was
//mulliganed/bottomed as "not a Giant" while it enabled Blind-Spot Giant and fed
//Calamity Bearer / Sunrise Sovereign / Borderland Behemoth.
void collectTribalTypes(Player * p, std::set<string>& out)
{
    if (!p || !p->game)
        return;
    const vector<string>& vocab = MTGAllCards::getCreatureValuesById();
    std::map<string, string> proper; //lowercased -> proper-cased subtype name
    for (size_t i = 0; i < vocab.size(); i++)
    {
        string low = vocab[i];
        for (size_t k = 0; k < low.size(); k++)
            low[k] = (char) tolower((unsigned char) low[k]);
        if (!low.empty())
            proper[low] = vocab[i];
    }
    MTGGameZone * zones[] = { p->game->library, p->game->hand, p->game->inPlay, p->game->graveyard };
    for (size_t z = 0; z < sizeof(zones) / sizeof(zones[0]); z++)
    {
        MTGGameZone * zone = zones[z];
        if (!zone)
            continue;
        for (int i = 0; i < zone->nb_cards; i++)
        {
            MTGCardInstance * c = zone->cards[i];
            if (!c || c->magicText.empty())
                continue;
            string mt = c->magicText;
            for (size_t k = 0; k < mt.size(); k++)
                mt[k] = (char) tolower((unsigned char) mt[k]);
            //"other <type>" - the lord/foreach/aslongas(other TYPE) tribal form.
            //Require a boundary before "other" so "another" does not match.
            for (size_t q = 0; (q = mt.find("other ", q)) != string::npos; q += 6)
                if (q == 0 || !isalnum((unsigned char) mt[q - 1]))
                    addTribeToken(mt, q + 6, proper, out);
            //"&<type>" - a source/count filter that keys on a type.
            for (size_t q = 0; (q = mt.find('&', q)) != string::npos; q += 1)
                addTribeToken(mt, q + 1, proper, out);
        }
    }
}

//If 'card' is a changeling AND its controller's deck/battlefield holds a
//tribal consumer, name the live-relevant type(s) so the model chains
//keyword->type->tribal role (it renders "[changeling]" today but never made
//that inference; wave-26 deck22 rule #1). Capped at 3 types; "" when there is
//no live consumer (no noise).
string changelingAnnotation(MTGCardInstance * card)
{
    if (!card || !card->basicAbilities[Constants::CHANGELING])
        return "";
    Player * ctrl = card->controller();
    if (!ctrl)
        return "";
    std::set<string> tribes;
    collectTribalTypes(ctrl, tribes);
    if (tribes.empty())
        return "";
    std::ostringstream o;
    o << " [changeling: counts as ";
    int n = 0;
    for (std::set<string>::iterator it = tribes.begin(); it != tribes.end() && n < 3; ++it, ++n)
        o << (n ? ", " : "") << *it;
    o << "]";
    return o.str();
}

//N-93c (wave-29 deck93/deck137): the tapped tag USED to read "[tapped - untaps
//and can attack next turn]". That trailing "can attack" is an attractive-nuisance
//affirmative substring: the model latched onto it and called a [tapped] creature
//"untapped", listing it as a present-turn attacker (deck137 s18). The
//summoning-sick tag works precisely because it is restriction-first and carries
//no affirmative verb ("cannot attack this turn"). Reword the tapped tag the same
//way. Rules-correct: a tapped creature cannot be declared as an attacker (only
//untapped creatures attack) and cannot block unless it has CANBLOCKTAPPED
//(MTGCardInstance::canBlock returns 0 for tapped without that keyword) - so the
//"or block" clause is dropped only for the rare CANBLOCKTAPPED case. Pure helper
//so the exact emitted shape is deterministically provable (runParseSelfTest).
//
//N-122c (wave-31, HIGH cross-seat, cost deck122 a game): the reword above was
//correct for a tapped NON-combatant and a self-contradiction for everything
//else. An attacker is TAPPED BY ATTACKING (CR 508.1f), so every declared
//attacker printed "[tapped - cannot attack or block this turn] [attacking]" -
//331 occurrences across 42 corpus logs, verbalized as a paradox by models at
//FOUR separate seats ("this is a contradiction in the log description ... I
//will assume it does NOT attack"), which dropped two correct chump blocks and
//lost the game at -16. The restriction clause describes a creature that is NOT
//in combat; for one that IS, the tapped state has a cause and that cause is the
//whole story. State the cause instead. Still restriction-first in spirit and
//still carries no affirmative FUTURE-permission substring ("attacking" is the
//present declared fact the [attacking] line already asserts, not a "can attack
//next turn" invitation - that was the N-93c failure mode).
static string tappedCreatureTag(bool canBlockTapped, bool attacking, const string& blockedName)
{
    if (attacking)
        return " [tapped - attacking]";
    if (!blockedName.empty())
        return " [tapped - blocking " + blockedName + "]";
    return canBlockTapped
        ? " [tapped - cannot attack this turn]"
        : " [tapped - cannot attack or block this turn]";
}

//N-139k: summoning sickness restricts ATTACKING and {T} abilities only (CR
//302.6) - the creature can still block. The old tag stated only the attack
//restriction and, beside the tapped tag that spells BOTH out, read as "cannot
//block either"; a seat declared lethal on that belief and lost. Pure helper so
//the exact clause is provable.
static string summoningSickTag()
{
    return " [summoning sick - cannot attack this turn, but CAN block]";
}

//N-139j: the blockers menu annotates a 0-power creature and the attackers menu
//did not, so 0/3 walls were repeatedly declared as attackers "dealing 3". Same
//shape as the validated blocker text, phrased for the attack side.
static string zeroPowerAttackerTag(int power)
{
    return power <= 0 ? " [deals 0 - this attack deals no damage to the opponent]" : "";
}

//N-152d: the printed-P/T delta tag. "printed" must mean the DISPLAYED face's
//printed stats - AAFlip::resolve resyncs origpower/origtoughness on transform
//and leaves basepower on the FRONT face, so a transformed DFC read as a pumped
//small creature. Returns "" when there is no delta (which is what suppresses
//the bogus tag once the face is corrected).
//N-152d LAYER 2 (wave-32; MECHANISM CORRECTED at fix time): the docket recorded
//this as "fixed on the attacker/target enumerators, still false on the board
//snapshot". That is not what the source says - the board snapshot
//(describeZoneCards) is the ONLY emitter of a "(printed X/Y)" tail in the whole
//file, and it was ALREADY routed through this helper on the corpus binary
//7fabd9bd0. The real defect is this helper's DISCRIMINATOR: `isFlipped` is not a
//reliable "which face is showing" flag. AAFlip::testDestroy (AllAbilities.cpp:
//5811-5817) RESETS isFlipped back to 0 immediately after a transform resolves,
//while the card keeps its back-face name and back-face origpower/origtoughness
//and its FRONT-face basepower/basetoughness - so a day/night werewolf renders
//with isFlipped == 0, basepower 2 (Brutal Cathar) and power 3 (Moonrage Brute),
//producing exactly the observed false "(3/3) (printed 2/2)". (TestSuiteAI.cpp:
//898 already carries the same finding: "isFlipped is NOT usable as the
//discriminator - the day/night transform path leaves it 0".) AATurnSide, the
//modal-DFC display toggle, DOES keep isFlipped coherent, so the flag stays in
//the test as one of two signals rather than being replaced by it.
//  The reliable signal is the NAME: both face-swapping paths stash the original
//name in nameOrig and rename the instance (AAFlip 5618, AATurnSide 5474), and
//both restore name == nameOrig when the card goes back. `showsOtherFace` is the
//OR of the two signals, so neither path can go unnoticed again.
static string printedPTTag(int power, int toughness, int basepower, int basetoughness,
                           int origpower, int origtoughness, int showsOtherFace)
{
    int printedP = (showsOtherFace > 0) ? origpower : basepower;
    int printedT = (showsOtherFace > 0) ? origtoughness : basetoughness;
    if (power == printedP && toughness == printedT)
        return "";
    std::ostringstream o;
    o << " (printed " << printedP << "/" << printedT << ")";
    return o.str();
}

//Is this instance currently displaying a face other than the one it was printed
//as? See printedPTTag: isFlipped alone misses the transform path.
static int cardShowsOtherFace(MTGCardInstance * card)
{
    if (!card)
        return 0;
    if (card->isFlipped > 0)
        return 1;
    return (!card->nameOrig.empty() && card->nameOrig != card->name) ? 1 : 0;
}

//N-158g: the mana line, COUNT-FIRST. Leading with the colour set bound the
//symbols to the pool size ("I have {R}{B} available, but Snarling Warg costs
//{3}{B} ... I cannot afford it") and lost a game at 1 life. The count is the
//number the arithmetic needs; spell it in words too so a digit cannot be
//misbound to a colour, and demote the colours to a named sub-clause.
static string manaAvailableLine(int sources, const string& colors)
{
    static const char * kNumWord[] = { "zero", "one", "two", "three", "four", "five",
                                       "six", "seven", "eight", "nine", "ten" };
    if (sources <= 0)
        return "0 total (no untapped sources)";
    std::ostringstream o;
    o << sources << " total (";
    if (sources <= 10)
        o << kNumWord[sources] << " ";
    o << "untapped source" << (sources > 1 ? "s" : "")
      << ", tapped automatically when you cast; colours you can make: " << colors << ")";
    return o.str();
}

//N-152e: the truthful Flip-Side note for a TRANSFORMING double-faced card. The
//h4 rewrite fixed the modal-DFC LAND branch and left this class promising that
//the back face "appears in the Cast menu as an alternative-cost cast" - false:
//a transform DFC is never CAST as its other face, it only transforms through
//its own printed condition. ONE helper feeds BOTH emitters (describeAction's
//priority-seam render and the CHOOSE_MENU option annotation) so the two paths
//cannot drift apart again - this defect existed only because they had.
static string transformDfcToggleNote(const string& curFace, const string& otherName,
                                     const string& otherCost)
{
    string s = " -> DISPLAY TOGGLE only (this is a TRANSFORMING double-faced card):"
               " it currently shows \"" + curFace + "\"; its other face is \""
             + otherName + "\"";
    if (!otherCost.empty())
        s += " (" + otherCost + ")";
    s += ". Flipping only changes which face is DISPLAYED - it casts nothing, uses"
         " no stack, and gains you nothing playable: this card is NEVER cast as its"
         " other face (there is no alternative-cost cast for it), it only TRANSFORMS"
         " through its own printed transform condition. Ignore this option.";
    return s;
}

//The truthful Flip-Side note for a modal-DFC SPELL whose two faces are both
//castable (Tergrid class). Kept as a helper so all four Flip-Side classes -
//land//land (N-152a), transform DFC (N-152e), spell//spell and spell//land
//(N-152h) - are emitted from one place and cannot drift apart again, which is
//the only reason the stale wording survived on a third card class at all.
static string mdfcSpellToggleNote(const string& curFace, const string& otherName,
                                  const string& otherCost)
{
    string s = " -> DISPLAY TOGGLE only: switches this hand card to show its other"
               " face \"" + otherName + "\"";
    if (!otherCost.empty())
        s += " (" + otherCost + ")";
    s += ". It does NOT cast anything and uses no stack. You usually do NOT need it:"
         " the Cast menu is where you cast, and it lists every face you can afford"
         " (the other face appears there as an alternative-cost cast). This only"
         " changes which face is displayed. Current face: \"" + curFace + "\".";
    return s;
}

//N-152h (wave-32, third emitter of the stale-Flip-Side class; deck146 seq36,
//Emeria's Call): a modal-DFC SPELL whose OTHER face is a LAND. The retired
//wording promised that "the Cast menu ... lists every face you can afford (the
//other face appears there as an alternative-cost cast)" - false here for the
//same reason it was false on the transform class: a LAND is never cast, so it
//can never appear in a Cast menu, at any cost. Say what the other face IS, say
//the toggle changes nothing playable, and stop - no promise about a route that
//has not been observed to work.
static string mdfcSpellLandBackNote(const string& curFace, const string& otherName,
                                    const string& otherMana)
{
    string s = " -> DISPLAY TOGGLE only (this card's other face is a LAND): it"
               " currently shows \"" + curFace + "\"; its other face is \""
             + otherName + "\", a land";
    if (!otherMana.empty())
        s += " (taps for " + otherMana + ")";
    s += ". A land is never CAST, so that face will NOT appear in the Cast menu -"
         " not at any cost and not as an alternative-cost cast. Flipping only"
         " changes which face is DISPLAYED: it casts nothing, uses no stack, and"
         " gains you nothing playable. Use the face that is showing.";
    return s;
}

//N-139c (wave-29 deck139/deck146): a mutate pile is ONE creature (CR 725) but the
//engine keeps the mutated-down cards as separate battlefield instances, so the
//board render split the pile into two adjacent lines - the top card with P/T +
//keywords, then the under card as a bare name (no P/T, no tag) - reading as "one
//creature or two?" and hiding that the merged creature carries the under card's
//abilities too. The under lines are folded out (skipped) by the caller; this tag
//rides the TOP card's line to say it is ONE merged creature and name what is
//underneath (so the model recalls those cards' abilities from the decklist). Pure
//helper for a deterministic proof.
static string mutatedPileTag(const std::vector<string>& underNames)
{
    if (underNames.empty())
        return "";
    std::ostringstream o;
    o << " [mutated pile - ONE merged creature (top card's name + P/T shown above,"
         " plus the combined abilities of every card in the pile); "
      << underNames.size() << " card" << (underNames.size() > 1 ? "s" : "")
      << " underneath: ";
    for (size_t i = 0; i < underNames.size(); i++)
        o << (i ? ", " : "") << underNames[i];
    o << "]";
    return o.str();
}

void describeZoneCards(std::ostringstream& out, MTGGameZone * zone, bool withStatus)
{
    bool first = true;
    for (int i = 0; i < zone->nb_cards; i++)
    {
        MTGCardInstance * card = zone->cards[i];
        //N-139c: fold mutated-DOWN cards into the top card's line. A card that is
        //underneath a mutate pile satisfies the engine's own "follows the fate of
        //the top-card" predicate (mutation set AND it has a parent above it) and is
        //rendered as part of the top card via mutatedPileTag below - do not list it
        //as its own battlefield entry. Battlefield renders only (withStatus); an
        //under card never appears off the battlefield.
        if (withStatus && card->mutation && !card->parentCards.empty())
            continue;
        if (!first)
            out << "; ";
        first = false;
        out << card->getDisplayName();
        //Disambiguate same-named permanents so a board entry binds to the
        //A#/B#/target line that offers it (battlefield lines only; "" otherwise).
        if (withStatus)
            out << instanceHandle(card);
        out << manaCostToken(card); //N-36b: {0} is a cost, not an absence
        if (card->isCreature())
        {
            out << " (" << card->power << "/" << card->toughness << ")";
            //Surface the live delta the static decklist text cannot carry: a
            //creature pumped, counter'd, enchanted or equipped is no longer
            //its printed stats. Only meaningful in play (withStatus).
            //N-152d (wave-31 deck152 seat, five witnesses across four opponents):
            //basepower/basetoughness are NOT resynced when a card TRANSFORMS -
            //AAFlip::resolve updates origpower/origtoughness to the new face and
            //leaves basepower on the FRONT face - so every transformed DFC
            //presented as a pumped small creature ("Moonrage Brute (3/3) (printed
            //2/2)" for a card whose back face is a printed 3/3; "Tovolar's
            //Packleader (7/7) (printed 6/6)" for a true 7/7). Read the DISPLAYED
            //face's printed stats, exactly as name/keywords/text/mana already do:
            //origpower/origtoughness ARE the displayed face's printed values once
            //flipped. basepower stays authoritative for the unflipped case, where
            //it carries characteristic-SETTING effects ("becomes a 1/1") that a
            //genuine buff annotation must keep showing. The inequality test then
            //suppresses the tag by construction when the corrected delta is zero.
            if (withStatus)
                out << printedPTTag(card->power, card->toughness,
                                    card->basepower, card->basetoughness,
                                    card->origpower, card->origtoughness,
                                    cardShowsOtherFace(card));
            //N-148d (wave-31 deck148 seq1): typeTag() returns "" for creatures by
            //design, so on a HAND line creature-ness was signalled ONLY by the
            //"(P/T)" parenthetical while every other type got an explicit
            //[artifact]/[enchantment]/[instant]/(land: taps for {W}) marker. The
            //pilot read a hand holding Kor Duelist {w} (1/1) + two Plains as
            //"this hand has zero creatures ... the lack of a body makes this a
            //dead hand" and mulliganed against its guide's explicit KEEP rule.
            //Same shape as the landTag fix above (deck93 wave-27, "zero lands" on
            //a 3-Swamp hand): an implicit signal loses to an explicit one at
            //distance. Scoped to the non-battlefield surfaces - a battlefield
            //creature already carries keywords/combat/summoning-sick tags, and
            //the board is where prompt budget is spent.
            if (!withStatus)
                out << (card->hasType(Subtypes::TYPE_ARTIFACT)
                        ? " [artifact creature]" : " [creature]");
        }
        else
        {
            //a bare name does not say what it IS
            string tag = typeTag(card);
            if (!tag.empty() && tag != "land")
                out << " [" << tag << "]";
            //A bare land name (esp. a basic "Swamp") gave the pilot no land/mana
            //signal and it miscounted lands as unplayable spells at the mulligan
            //(deck93 wave-27). Scoped to the HAND surfaces (withStatus == false:
            //the hand render feeding the mulligan/bottom asks and CURRENT
            //SITUATION) - a battlefield land already carries deployment/painland
            //tags, and tagging every board render would just be prompt-budget noise.
            else if (card->isLand() && !withStatus)
                out << landTag(card);
        }
        //A changeling is every creature type: name the tribe(s) a card in this
        //deck/battlefield actually consumes so the model chains it to a role.
        //On hand lines (withStatus false) this is the only changeling signal -
        //keywordList below is battlefield-only - so it also covers mulligan.
        out << changelingAnnotation(card);
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
                //N-148b (wave-31 deck148, a game lost by 2): the host NAME alone
                //cannot bind to a board line when the host's name collides -
                //three identical "Kor ally #1/#2/#3" and a Plating that said only
                //"attached to: Kor ally". The model attributed the buff to the
                //wrong copy, re-equipped the same Plating and swung 9 instead of
                //15. Every surface that NAMES a permanent carries its handle.
                if (host)
                    out << " [attached to: " << host->getDisplayName()
                        << instanceHandle(host) << "]";
            }
            //A tapped creature reads as harmless and the pilot builds plans
            //on that (wave-7 deck140 collapse: "tapped = no threat" bridged
            //a sweeper-hold it should never have satisfied). Name the truth
            //at the flag: it untaps and attacks again next turn.
            MTGCardInstance * blockedAtk = card->isDefenser();
            string blockedName = blockedAtk
                ? blockedAtk->getDisplayName() + instanceHandle(blockedAtk)
                : string("");
            if (card->isTapped())
                out << (card->isCreature()
                        ? tappedCreatureTag(card->has(Constants::CANBLOCKTAPPED),
                                            card->isAttacker() != 0, blockedName)
                        : string(" [tapped]"));
            //A summoning-sick creature renders identically to an attack-ready one
            //otherwise, so the pilot pattern-completes its ATTACK line from the
            //board count and lists slots the A-lines never offered (deck93 N-93a:
            //11/15 attack decls padded with phantom attackers, plausibly cost the
            //vs18 game on a false-lethal tap). Annotate the restriction inline on
            //the surface the model reads, using the ENGINE's own predicate
            //(hasSummoningSickness = entered this turn, no haste, is a creature) -
            //exactly what canAttack() enforces, no re-derivation. Guide prose
            //alone ("count from the A-lines") already failed to override this.
            //N-139k (wave-31 deck139, HIGH, cost a game): this tag stated ONLY
            //the attack restriction, and beside the neighbouring "[tapped -
            //cannot attack or block this turn]" - which spells BOTH out - the
            //shorter clause read as the same class of statement, i.e. "cannot
            //block either". The pilot declared lethal on a board whose untapped
            //summoning-sick Sauron then blocked, and it lost at t11. Summoning
            //sickness restricts attacking and {T} abilities only (CR 302.6); a
            //summoning-sick creature CAN block. Say the permission out loud -
            //this is the one place an affirmative substring is REQUIRED, because
            //the omission is what the model was completing wrongly, and "CAN
            //block" cannot be misread as a present-turn ATTACK licence.
            if (card->hasSummoningSickness())
                out << summoningSickTag();
            //Combat status. When the creature is tapped the tapped tag above
            //already named the cause (N-122c), so do not print it twice.
            if (!card->isTapped())
            {
                if (card->isAttacker())
                    out << " [attacking]";
                else if (blockedAtk)
                    out << " [blocking " << blockedName << "]";
            }
            //N-139c: if this is the TOP of a mutate pile (it has mutated-down cards
            //beneath it), render the whole pile as this one line - gather the under
            //card names (BFS over childrenCards, bounded) and append the merged tag.
            if (card->mutation && !card->childrenCards.empty())
            {
                std::vector<string> underNames;
                std::vector<MTGCardInstance *> stack(card->childrenCards.begin(),
                                                     card->childrenCards.end());
                while (!stack.empty() && underNames.size() < 8)
                {
                    MTGCardInstance * u = stack.back();
                    stack.pop_back();
                    if (!u)
                        continue;
                    underNames.push_back(u->getDisplayName());
                    for (size_t k = 0; k < u->childrenCards.size(); k++)
                        stack.push_back(u->childrenCards[k]);
                }
                out << mutatedPileTag(underNames);
            }
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

//Dungeon rooms: a dungeon card's text= lists every room as
//"<Room Name> - <effect>. -- <Room Name> - <effect>. -- ...". The engine
//surfaces room CHOICES - the first-venture dungeon SELECTION and the in-dungeon
//room BRANCH menu - carrying only the room/dungeon NAME; each room's effect
//lives in dense auto= script the model cannot read (deck146 N-146b/c). Parse
//the readable text= into (name, effect) pairs so the deciding fact can ride the
//option, mirroring the land/target/magnitude annotations already in place.
void parseDungeonRooms(const string& t, std::vector<std::pair<string, string> >& rooms)
{
    size_t pos = 0;
    while (pos <= t.size())
    {
        size_t sep = t.find(" -- ", pos);
        string piece = t.substr(pos, sep == string::npos ? string::npos : sep - pos);
        size_t a = piece.find_first_not_of(" \t\r\n");
        size_t b = piece.find_last_not_of(" \t\r\n");
        if (a != string::npos)
        {
            piece = piece.substr(a, b - a + 1);
            //split name from effect on the FIRST " - " (room names never contain
            //it; an effect that does keeps the rest intact via find's first-match).
            size_t dash = piece.find(" - ");
            if (dash != string::npos)
                rooms.push_back(std::make_pair(piece.substr(0, dash), piece.substr(dash + 3)));
        }
        if (sep == string::npos)
            break;
        pos = sep + 4;
    }
}

//The dungeon a player is currently venturing lives (exactly one) in the command
//zone; the dungeons being SELECTED sit in the sideboard as chooseTarget cands.
MTGCardInstance * activeDungeon(MTGPlayerCards * g)
{
    if (!g || !g->commandzone)
        return NULL;
    for (int i = 0; i < g->commandzone->nb_cards; i++)
        if (g->commandzone->cards[i] && g->commandzone->cards[i]->hasType(Subtypes::TYPE_DUNGEON))
            return g->commandzone->cards[i];
    return NULL;
}

//case-insensitive: is `name` a prefix of `s` (used to match a menu option, which
//may carry trailing annotation, against a room name).
bool ciStartsWith(const string& s, const string& name)
{
    if (name.empty() || s.size() < name.size())
        return false;
    for (size_t i = 0; i < name.size(); i++)
        if (tolower((unsigned char) s[i]) != tolower((unsigned char) name[i]))
            return false;
    return true;
}

//One targetable thing, described for the model's target menu.
//---- N-105a / N-105b (wave-32; SEVEN seats; corpus-integrity) ------------------
//Poison counters were rendered NOWHERE to this seat. The engine tracks
//Player::poisonCount and renders it FOR THE HUMAN (GuiStatic.cpp:138) - a
//textbook ishuman-lens gap. With no number the pilot invented one ("I need 10
//poison counters to win; I currently have 0" at an actual 6), conflated the
//10-counter threshold with the opponent's LIFE total, and twice reasoned that
//the game should already have ended. WORDING RULE (perception lane): the
//threshold number is always explicit, the sentence leads with what ENDS the
//game, and no affirmative substring ("safe", "fine", "room to spare") is
//offered for a model to latch onto. Pure over their inputs, so PARSETEST can
//prove the emitted shape without a game.
static const int kPoisonLoseAt = 10; //Player::afterDamage: poisonCount >= 10 loses

//"Poison counters (you): 6 of 10 - ..." - one player's status sentence.
static string poisonCountPhrase(bool mine, int count)
{
    std::ostringstream o;
    const char * who = mine ? "you" : "opponent";
    const char * loses = mine ? "you LOSE" : "the opponent LOSES";
    o << "Poison counters (" << who << "): " << count << " of " << kPoisonLoseAt
      << " - " << loses << " the game at " << kPoisonLoseAt
      << " poison counters, whatever the life total is";
    int togo = kPoisonLoseAt - count;
    if (togo <= 0)
        o << "; that threshold is already reached";
    else
        o << "; " << togo << " more end" << (togo == 1 ? "s" : "") << " it";
    o << ". Poison is not life: it does not reset between turns, and gaining"
         " life or preventing damage does not remove a poison counter.";
    return o.str();
}

//The CURRENT SITUATION status line for both players. Empty when neither player
//is poisoned, so a game with no infect/toxic card in it is untouched.
static string poisonStatusLine(int mine, int opp)
{
    if (mine <= 0 && opp <= 0)
        return "";
    std::ostringstream o;
    if (mine > 0)
        o << poisonCountPhrase(true, mine) << "\n";
    if (opp > 0)
        o << poisonCountPhrase(false, opp) << "\n";
    return o.str();
}

//The narration line for a poison GAIN. WEventplayerPoisoned fires on all three
//engine paths - infect combat damage (Damage.cpp:199), toxic (Damage.cpp:239)
//and the `alterpoison` script ability (AllAbilities.cpp:1468, which narrated
//NOTHING at all) - so one handler closes the class. The DELTA is computed from
//this seat's own last-seen total rather than the event payload: the toxic path
//fires the event carrying the DAMAGE dealt, not the toxicity granted, so that
//payload is wrong there while the player's settled poisonCount is not.
static string poisonGainLine(bool mine, int delta, int total)
{
    std::ostringstream o;
    o << "Poison: " << (mine ? "you" : "the opponent");
    if (delta > 0)
        o << (mine ? " take " : " takes ") << delta << " poison counter"
          << (delta == 1 ? "" : "s") << " - now ";
    else
        o << (mine ? " now have " : " now has ");
    o << total << " of " << kPoisonLoseAt << " ("
      << (mine ? "you LOSE" : "the opponent LOSES") << " at " << kPoisonLoseAt
      << " poison counters";
    int togo = kPoisonLoseAt - total;
    if (togo > 0)
        o << "; " << togo << " more end" << (togo == 1 ? "s" : "") << " it";
    else
        o << "; that threshold is now reached";
    o << ")";
    return o.str();
}

//Does this source REPLACE its damage to a PLAYER with poison counters?
//Damage.cpp:192 branches on INFECT or POISONDAMAGER: the life total never moves.
static bool sourceDealsPoisonInsteadOfDamage(MTGCardInstance * c)
{
    return c && (c->has(Constants::INFECT) || c->has(Constants::POISONDAMAGER));
}

//N-105b: the blocker-seam forecast. The old single sentence was built from raw
//attacker power with NO infect check, so against infect attackers it asserted
//three falsehoods at once - a life total that will not change, a "NOT lethal"
//verdict about a resource the attack does not touch (INVERTED on any swing
//crossing 10 poison), and the advice "taking damage while ahead is often
//correct", which is the exact inverse of correct play against a counter that
//never resets. deck36 s25 t12 lost the game to that sentence. Partition the
//swing by whether its damage is REPLACED, price each half against its own
//threshold, and DROP the take-the-damage advice entirely whenever any poison is
//incoming (an accurate number with stale advice attached is still a misteach).
static string combatDamageForecast(int life, int poison, int lifeIncoming, int poisonIncoming)
{
    std::ostringstream o;
    o << "Your life: " << life << ".";
    if (poisonIncoming <= 0)
    {
        //Non-poison swing: byte-identical to the validated wording.
        o << " Unblocked, these attackers deal up to " << lifeIncoming
          << " - you would be at " << (life - lifeIncoming)
          << (life - lifeIncoming <= 0
              ? " - LETHAL if it all connects: block enough to survive."
              : " - NOT lethal: block only where the trade favors you; taking damage while ahead is often correct.")
          << "\n";
        return o.str();
    }
    o << " Your poison counters: " << poison << " of " << kPoisonLoseAt << ".\n";
    int after = poison + poisonIncoming;
    o << "Unblocked, these attackers put up to " << poisonIncoming
      << " POISON COUNTER" << (poisonIncoming == 1 ? "" : "S")
      << " on you - you would be at " << after << " of " << kPoisonLoseAt << " poison";
    if (after >= kPoisonLoseAt)
        o << " - LETHAL if it all connects: at " << kPoisonLoseAt
          << " poison counters you lose the game no matter what your life total is."
             " Your life total does not answer this; block the poison.";
    else
        o << " - that is not yet " << kPoisonLoseAt << ", but poison counters never"
             " reset and nothing here removes them, so every counter you take is"
             " permanent progress toward losing. Life total is not the resource"
             " under attack; price the block against the poison count.";
    o << "\n";
    if (lifeIncoming > 0)
        o << "Those same attackers also deal up to " << lifeIncoming
          << " ORDINARY damage - your life would be " << (life - lifeIncoming)
          << (life - lifeIncoming <= 0 ? " - LETHAL on life as well." : " on the life track.")
          << "\n";
    return o.str();
}

//What an attacker's damage actually DOES, stated on the line that decides the
//block. The engine's own keyword names for this family are opaque
//("poisontoxic", "poisondamager"), so the effect is spelled out here.
static string attackerPoisonNote(MTGCardInstance * c)
{
    if (!c)
        return "";
    std::ostringstream o;
    if (sourceDealsPoisonInsteadOfDamage(c))
        o << " [its combat damage to YOU is dealt as POISON COUNTERS, not life loss -"
             " your life total will not move and blocking is the only thing that stops it]";
    int tox = c->getToxicity();
    if (tox > 0)
        o << " [toxic " << tox << ": if it damages you it ALSO puts " << tox
          << " poison counter" << (tox == 1 ? "" : "s") << " on you, on top of the damage]";
    if (c->has(Constants::WITHER))
        o << " [wither: the damage it deals to a CREATURE arrives as -1/-1 counters,"
             " which do not wear off at end of turn - a blocker that survives stays shrunk]";
    else if (c->has(Constants::INFECT))
        o << " [infect also damages CREATURES as -1/-1 counters, which do not wear off"
             " at end of turn - a blocker that survives stays shrunk]";
    return o.str();
}

//N-158k: the per-target life price for a "life:-manacost" spell (Feed the Swarm
//class - you pay life equal to the chosen permanent's mana value). Byte-identical
//wording to the cast-line preview, so the two surfaces cannot disagree and an
//echo of either binds the same way. "" for a player target (no mana value).
string perTargetLifeCostNote(Targetable * t)
{
    MTGCardInstance * c = dynamic_cast<MTGCardInstance *>(t);
    if (!c)
        return "";
    ManaCost * mc = c->getManaCost();
    std::ostringstream o;
    o << " (costs you " << (mc ? mc->getConvertedCost() : 0) << " life)";
    return o.str();
}

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
    //Bind a target line to the specific same-named body (battlefield only).
    o << instanceHandle(c);
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
    //Dungeon SELECTION target (N-146c): the generic 110-char snippet TRUNCATED
    //the room list at ~1-2 rooms, hiding the completion room + payoff the value
    //pick depends on (Lost Mine completes in 3 rooms vs Tomb's 4 vs Mad Mage's
    //8). Show the full room path plus a compact room-count / completion-payoff
    //summary (the final text= room is the completion reward) instead of the cut
    //snippet. Dungeons only ever surface as targets in the venture selection, so
    //this special-case does not touch normal removal/target rendering.
    if (c->hasType(Subtypes::TYPE_DUNGEON))
    {
        std::vector<std::pair<string, string> > rooms;
        parseDungeonRooms(c->text, rooms);
        if (!rooms.empty())
        {
            o << " [dungeon: " << rooms.size() << " rooms; completion reward (\""
              << rooms.back().first << "\"): " << rooms.back().second << "]";
            o << " - full room path: \"" << c->text << "\"";
            return o.str();
        }
    }
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
    //GptMutex, not std::mutex: on Vita std::mutex operations are NO-OPS
    //(inactive gthreads layer), which was harmless while no worker could
    //start but is a real race now that gptSpawnWorker supplies one.
    GptMutex mtx;
    int status;      //0 idle, 1 in flight, 2 done (answer not yet consumed)
    string prompt;   //the userMsg the in-flight/done request was built for
    string response; //raw HTTP body once status == 2
    std::chrono::steady_clock::time_point started; //request launch time
    AsyncState() : status(0) {}
};

//Heap-allocated capture set for the HTTP worker. The worker owns and frees
//it; the shared_ptr inside keeps AsyncState alive if the player is destroyed
//WAGIC_PADLOG (Android: flag file User/padlog.on): async-lifecycle slice of
//the input/UI trace - spawn/publish/consume/stale transitions, to pin the
//"opponent is thinking forever with no worker thread" stall (2026-08-17).
//Dev builds only - diagnostics are compiled out of release builds.
#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)
static FILE * gptPadlogFile()
{
    static FILE * out = NULL;
    static int state = 0;
    if (state == 0)
    {
        if (getenv("WAGIC_PADLOG"))
            { out = stderr; state = 1; }
#ifdef ANDROID
        else if (access("/sdcard/Wagic/User/padlog.on", F_OK) == 0)
            { out = fopen("/sdcard/Wagic/User/padlog.txt", "a"); state = out ? 1 : -1; }
#endif
        else state = -1;
    }
    return (state == 1) ? out : NULL;
}
#define GPTASYNCLOG(...) do { FILE * f_ = gptPadlogFile(); if (f_) { fprintf(f_, __VA_ARGS__); fflush(f_); } } while (0)
#else
#define GPTASYNCLOG(...) ((void)0)
#endif //_DEBUG || WAGIC_DEVLOGS

//mid-request.
struct AIPlayerGPT::WorkerCtx
{
    std::shared_ptr<AsyncState> state;
    string url;
    string requestBody;
    string key;
    long timeoutMs;
    bool codex; //ChatGPT-subscription preset: /responses + SSE, not chat completions
    WorkerCtx() : timeoutMs(0), codex(false) {}
};

void AIPlayerGPT::WorkerMain(void * p)
{
    WorkerCtx * ctx = reinterpret_cast<WorkerCtx *>(p);
    string body;
    if (ctx->codex)
    {
        //The subscription backend answers in the Responses shape over SSE.
        //gptCodexComplete owns auth/refresh and hands back the assistant
        //text; wrapping it in the chat-completions envelope here lets
        //pollCompletion and every downstream reply parser stay unchanged.
        string err;
        string text = gptCodexComplete(ctx->url, ctx->requestBody, ctx->timeoutMs, err);
        if (!text.empty())
        {
            json wrapped = {{"choices", json::array({{{"message", {{"content", text}}}}})}};
            body = wrapped.dump();
        }
        else
        {
            //Empty body = the seams' existing transport-failure path (Baka
            //answers). Log the WHY once per distinct cause, not per decision.
            //Mutex-guarded: both selfplay seats can fail concurrently, and a
            //racing write to a shared string is UB, not just a double log.
            static GptMutex logMtx;
            static string lastErr;
            std::lock_guard<GptMutex> lg(logMtx);
            if (err != lastErr)
            {
                lastErr = err;
                gptLogLine("subscription request failed: " + err);
            }
        }
    }
    else
    {
        GPTASYNCLOG("gpt worker start url=%s body=%zu\n", ctx->url.c_str(), ctx->requestBody.size());
        body = gptHttpPost(ctx->url, ctx->requestBody, ctx->timeoutMs, ctx->key);
    }
    {
        std::lock_guard<GptMutex> g(ctx->state->mtx);
        ctx->state->response = body;
        ctx->state->status = 2;
    }
    GPTASYNCLOG("gpt worker publish bytes=%zu\n", body.size());
    delete ctx;
}

//NATIVE REASONING CAPTURE (wave-34 #1b(A)). With the post-answer scratch
//block gone from the protocol, the reasoning the dev loop audits comes from
//the model's own thinking window - and the server decides where it lands.
//A vLLM running a reasoning parser for the served model puts it in
//message.reasoning_content and hands back clean content; one that does NOT
//leaves the raw "<think> ... </think>" inline at the head of content. Both
//happen on this stack, so the client implements BOTH paths unconditionally
//rather than betting on the server's configuration.
//
//This splits an inline block out of `content` into `reasoning`:
//  * a leading "<think>" closed by "</think>"  -> reasoning = the block,
//    content = what follows (the answer);
//  * no opening tag but a "</think>" present   -> the opener was consumed by
//    the chat template (a real vLLM shape): everything before it is reasoning;
//  * UNCLOSED "<think>"                        -> the reply is REASONING-ONLY
//    (truncation, or a model that opened and never closed). Returns FALSE with
//    content EMPTIED: think text must never be parsed as an answer - a
//    truncated deliberation ends mid-arithmetic and its stray integers are
//    exactly what the index scanners latch onto. The caller re-asks.
//Everything downstream - every length meter, every answer scanner, the
//translog `reply` field - then runs on the STRIPPED text, or the fields would
//be measuring thinking tokens instead of reply tokens.
//Internal slot key for the forced-close (phase-2) request. It only has to be
//a string no assembled prompt can equal - the async slot keys on it, and
//buildRequestBody recognises it, strips it, and rebuilds the SAME user
//message with the assistant prefill appended.
static const char * kForceCloseTag = "\x01wagic-force-close\x01";

//The answer reserve, in tokens: everything the reply itself needs once the
//thinking window is spent. Derived from this corpus, not guessed - p95 PLAN
//line 592 chars (~200 tokens) + p95 coded choice line 74 chars (~30 tokens),
//with margin. It is both the head-room added to the thinking budget in phase
//1 and the whole cap of the phase-2 forced close.
static const long kAnswerReserveTokens = 400;

//Default thinking budget, in tokens, when thinking is ON and nothing is
//configured. The formula (owner, 2026-08-19) is b = 1.5 * (t - p - c) where t
//is the time bound expressed as tokens; at a 240s bound and ~30 tok/s that
//gives 10450. The owner set the shipped starting value at a flat 8000
//("oof. thats a lot. lets make it 8000"). Config/env override it; 0 or less
//means unbounded (the pre-budget behaviour).
static const long kDefaultReasoningBudget = 8000;

//Which endpoints accept an assistant prefill (vLLM's continue_final_message /
//llama.cpp's equivalent). The subscription (Codex) and OpenRouter paths keep
//reasoning server-side - there is nothing for the client to close - and
//api.openai.com rejects unknown top-level fields with a 400, which would turn
//every budget hit into a lost decision instead of a forced answer.
static bool gptForceCloseSupported(const string& endpoint)
{
    if (gptCodexEndpoint(endpoint))
        return false;
    return endpoint.find("api.openai.com") == string::npos
        && endpoint.find("openrouter.ai") == string::npos;
}

static bool splitReasoningBlock(string& content, string& reasoning)
{
    size_t head = content.find_first_not_of(" \t\r\n");
    if (head == string::npos)
        return true; //empty/whitespace reply: the transport-failure path owns it
    bool opensWithThink = (content.compare(head, 7, "<think>") == 0);
    size_t close = content.find("</think>");
    if (!opensWithThink && close == string::npos)
        return true; //no thinking block in this reply at all
    if (close == string::npos)
    {
        //Opened and never closed: reasoning-only.
        string r = content.substr(head + 7);
        size_t rs = r.find_first_not_of(" \t\r\n");
        reasoning = (rs == string::npos) ? string() : r.substr(rs);
        content.clear();
        return false;
    }
    size_t rStart = opensWithThink ? head + 7 : 0;
    string r = content.substr(rStart, close - rStart);
    size_t rs = r.find_first_not_of(" \t\r\n");
    size_t re = r.find_last_not_of(" \t\r\n");
    if (rs != string::npos)
        reasoning = r.substr(rs, re - rs + 1);
    string rest = content.substr(close + 8);
    size_t as = rest.find_first_not_of(" \t\r\n");
    content = (as == string::npos) ? string() : rest.substr(as);
    return true;
}

bool AIPlayerGPT::asyncBusy() const
{
    std::lock_guard<GptMutex> g(mAsyncState->mtx);
    return mAsyncState->status == 1;
}

int AIPlayerGPT::pollCompletion(const string& userMsg, string& content)
{
    {
        std::lock_guard<GptMutex> g(mAsyncState->mtx);
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
                mLastReasoningOnly = false;
                if (!body.empty())
                {
                    try
                    {
                        json reply = json::parse(body);
                        json choice0 = reply["choices"][0];
                        content = choice0["message"]["content"].is_string()
                                  ? choice0["message"]["content"].get<string>() : string();
                        //PATH 1: the server ran a reasoning parser for this
                        //model - the thinking arrives in its own field and
                        //`content` is already clean. Log it verbatim.
                        if (choice0["message"].contains("reasoning_content")
                            && choice0["message"]["reasoning_content"].is_string())
                        {
                            string rc = choice0["message"]["reasoning_content"].get<string>();
                            if (!rc.empty())
                                mLastReasoning = rc;
                        }
                        //Decode stopped at the cap: with a thinking budget in
                        //force this is the expected phase-1 exit, and the
                        //unclosed-think test below decides whether the answer
                        //still has to be forced out of the model.
                        mLastFinishLength = (choice0.contains("finish_reason")
                                             && choice0["finish_reason"].is_string()
                                             && choice0["finish_reason"].get<string>() == "length");
                    }
                    catch (json::exception&)
                    {
                        content.clear();
                    }
                }
                //PATH 2 (unconditional, whatever path 1 found): an inline
                //"<think> ... </think>" the server did not parse out. Strip it
                //into the same field so every scanner downstream measures REPLY
                //text. An unclosed block empties `content` - reasoning-only, and
                //the caller forces the answer rather than parsing think text.
                {
                    string inlineReasoning;
                    if (!splitReasoningBlock(content, inlineReasoning))
                        mLastReasoningOnly = true;
                    if (!inlineReasoning.empty())
                        mLastReasoning = inlineReasoning;
                }
                //Subscription preset: the backend prices every reply against a
                //rolling plan window and reports the gauge on each response.
                //Surface it when it moves - a player burning their ChatGPT plan
                //on a card game deserves to see the meter, and a static gauge
                //re-noticed every decision would be wallpaper.
                GPTASYNCLOG("gpt consume body=%zu content=%zu latency=%ldms\n",
                            body.size(), content.size(), mLastLatencyMs);
                if (gptCodexEndpoint(mEndpoint))
                {
                    string pct = gptCodexUsedPercent();
                    if (!pct.empty() && pct != mCodexPctNotified)
                    {
                        mCodexPctNotified = pct;
                        setNotice("ChatGPT plan: " + pct + "% of the weekly limit used", 6.0f);
                    }
                }
                return 0;
            }
            //An answer for a prompt the game state has moved past (should
            //not happen while the AI neither acts nor passes; drop safely).
            DebugTrace("AIPlayerGPT: dropping stale async answer");
            GPTASYNCLOG("gpt stale drop (prompt moved) resp=%zu\n", mAsyncState->response.size());
            mAsyncState->status = 0;
            mAsyncState->response.clear();
        }
    }

    //Idle: build the request on the game thread (the prompt members are not
    //shared with the worker) and launch the round trip in the background.
    string requestBody = buildRequestBody(userMsg);
    bool codex = gptCodexEndpoint(mEndpoint);
    string url = codex ? mEndpoint + "/responses" : mEndpoint + "/v1/chat/completions";
    string key = mApiKey;
    std::shared_ptr<AsyncState> state = mAsyncState;
    {
        std::lock_guard<GptMutex> g(state->mtx);
        state->status = 1;
        state->prompt = userMsg;
        state->response.clear();
        state->started = std::chrono::steady_clock::now();
    }
    GPTASYNCLOG("gpt spawn ask prompt=%zu endpoint=%s\n", userMsg.size(), mEndpoint.c_str());
    long timeoutMs = mTimeoutMs;
    //The worker runs through gptSpawnWorker - the platform threading seam.
    //On Vita that is a native sceKernelCreateThread (std::thread construction
    //throws there - no active gthreads layer); elsewhere it is a detached
    //std::thread. WAGIC_GPT_NOTHREAD (checked inside the seam) emulates a
    //refusing platform on desktop, where the refusal path is otherwise
    //unreachable and therefore untestable exactly where it is easiest to test.
    WorkerCtx * ctx = NEW WorkerCtx();
    ctx->state = state;
    ctx->url = url;
    ctx->requestBody = requestBody;
    ctx->key = key;
    ctx->timeoutMs = timeoutMs;
    ctx->codex = codex;
    if (!gptSpawnWorker(WorkerMain, ctx))
    {
        delete ctx;
        //The platform refused a thread - resource limits, a thread cap, or an
        //inactive threading runtime. The guarantee every other seam here keeps
        //must hold: a transport failure degrades to the heuristic AI, it never
        //takes the game down. Publishing an empty reply is the same shape as an
        //unreachable endpoint, so the caller falls back to Baka on its existing
        //path. Resolve THIS tick rather than reporting a round trip that does
        //not exist. Returning kChoicePending here means "no action yet", and
        //the only thing that stops an empty clickstream being committed as a
        //pass is decisionPending(), which is true only while asyncBusy() - i.e.
        //while status == 1. Publishing status = 2 synchronously and then
        //claiming pending left the seam waiting and the gate reporting idle, so
        //the engine passed the turn. Every decision became attempt-once,
        //get-passed: on hardware that is an opponent that draws and passes
        //forever, and in the harness 107 turns with both players still at 20
        //life. Going straight back to idle with an empty reply is the same
        //shape as a synchronous transport failure, which the seams already
        //answer with the heuristic AI, in this tick.
        {
            std::lock_guard<GptMutex> g(state->mtx);
            state->status = 0;
            state->response.clear();
            state->prompt.clear();
        }
        content.clear();
        DebugTrace("AIPlayerGPT: could not start the worker thread; falling back to the heuristic AI");
        //Log this ONCE. A platform that refuses one thread refuses all of them,
        //so the message is identical every time and repeats once per decision -
        //209 and 344 identical lines in two Vita sessions. Writing it each time
        //puts a file open/write/close on the game thread on the single platform
        //where storage I/O per game event is a known cause of lag, to say
        //something already known after the first line.
        static bool refusalLogged = false;
        if (!refusalLogged)
        {
            refusalLogged = true;
            gptLogLine(string("worker thread refused")
                       + " - falling back to the heuristic AI for every decision"
                       + " (logged once; this platform cannot start threads)");
        }
        return 0; //answer now, with an empty reply -> the seam's heuristic
    }
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
            mForceClosePrefill.clear();
            mRetryFirstLatencyMs = -1;
            mLastRetry = true;
            return 0;
        }
        //Decision changed under a pending retry: drop it, poll the new decision.
        mRetryActivePrompt.clear();
        mRetryBase.clear();
        mForceClosePrefill.clear();
        mRetryFirstLatencyMs = -1;
    }

    int r = pollCompletion(userMsg, content);
    if (r == kChoicePending)
        return kChoicePending;

    mLastRetry = false;
    //BUDGET-FORCED ANSWER (wave-34 #1a/#1b, owner ruling 2026-08-19). Native
    //thinking is BOUNDED, and a bound whose only mechanism is max_tokens loses
    //the decision - the reply comes back mid-thought with no answer at all,
    //which is exactly the truncation class that already costs this project
    //decisions. So the budget is two-phase: phase 1 decodes with
    //max_tokens = budget + the answer reserve; if it comes back with an
    //UNCLOSED <think> (the reply is reasoning-only), phase 2 hands the model
    //its own truncated thinking back with the "</think>" close INJECTED and a
    //tight answer-sized cap, so it must answer from what it already has. This
    //is Qwen's documented budget-forcing pattern, expressed through vLLM's
    //assistant-prefill (continue_final_message) rather than a raw /v1/
    //completions prompt render, because the client has no copy of the chat
    //template and a mis-rendered prompt would be a silent quality change.
    //Scoped to endpoints that accept the prefill (local vLLM/llama.cpp): the
    //subscription and OpenRouter paths hide their reasoning entirely and have
    //nothing to close, and api.openai.com 400s unknown fields.
    if (mLastReasoningOnly && userMsg != mRetryDoneBase && gptForceCloseSupported(mEndpoint))
    {
        mRetryFirstLatencyMs = mLastLatencyMs;
        mRetryBase = userMsg;
        mForceClosePrefill = mLastReasoning;
        mRetryActivePrompt = string(kForceCloseTag) + userMsg;
        mLastBudgetHit = true;
        setNotice("thinking hit its budget - asking for the answer", 3.0f);
        DebugTrace("AIPlayerGPT: unclosed <think> (budget/truncation); forcing the answer");
        return kChoicePending; //next tick polls the forced-close request
    }
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
    : AIPlayerBaka(observer, deckFile, deckfileSmall, avatarFile, deck), mAsyncState(std::make_shared<AsyncState>()), mThinkTime(0), mNoticeTicks(0), mFallbackCount(0), mDegradedTicks(0), mBlocksDoneTurn(-1), mAttacksDoneTurn(-1), mPassDeclineTurn(-1), mStuckCastTurn(-1), mTransSeq(0), mLastLatencyMs(-1), mGameEndLogged(false), mGameStartLogged(false), mNarrationLogged(0), mNarratedTurnOwner(NULL), mNarratedTurnNumber(-1), mDealDone(false), mLastChoice(-1), mRetryFirstLatencyMs(-1), mLastRetry(false),
      mPregameBottomAsked(false), mPregameBottomForMulls(-1), mPregameMullsSeen(0),
      mLastReasoningOnly(false), mLastFinishLength(false), mLastBudgetHit(false), mReasoningBudget(0)

{
    mLastPoison[0] = mLastPoison[1] = 0; //N-105a: poison deltas start from zero
#ifndef WAGIC_NO_CURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
#endif
    //File config first, environment variables override.
    GptSettings cfg = GptSettings::load();
    mConfigUrls = cfg.urls;
    mConfigModel = cfg.model;
    mMaxTokens = cfg.maxTokens;
    mRepetitionPenalty = cfg.repetitionPenalty;
    if (const char * rp = getenv("WAGIC_GPT_REPPENALTY"))
        mRepetitionPenalty = atof(rp);
    mProviderOnly = cfg.providerOnly;
    if (const char * po = getenv("WAGIC_GPT_PROVIDER"))
        mProviderOnly = po;
    mApiKey = cfg.key;
    mTimeoutMs = 1000L * cfg.timeoutSecs;
    if (const char * key = getenv("WAGIC_GPT_KEY"))
        mApiKey = key;
    if (const char * to = getenv("WAGIC_GPT_TIMEOUT"))
        mTimeoutMs = 1000L * atol(to);
    if (mTimeoutMs < 5000)
        mTimeoutMs = 5000;
    mPatienceLimit = (float) cfg.patienceSecs;
    if (const char * pt = getenv("WAGIC_GPT_PATIENCE"))
        mPatienceLimit = (float) atof(pt);
    if (mPatienceLimit < 0)
        mPatienceLimit = 0;
    mThinking = getenv("WAGIC_GPT_THINKING") ? envFlag("WAGIC_GPT_THINKING") : (cfg.thinking == 1);
    //Thinking budget: configured value wins, else the shipped default, and
    //only in thinking mode (a non-thinking reply has no window to bound).
    mReasoningBudget = (cfg.reasoningBudget >= 0) ? cfg.reasoningBudget : kDefaultReasoningBudget;
    if (const char * rb = getenv("WAGIC_GPT_REASONING_BUDGET"))
        mReasoningBudget = atol(rb);
    if (!mThinking)
        mReasoningBudget = 0;
    //Timeout FLOOR for thinking mode, applied only to the DEFAULT. Worst case
    //is the whole budget decoded at this stack's ~30 tok/s: (8000 + 230) / 30
    //~= 275s, so a shorter default would turn every budget-spending decision
    //into a transport timeout - a latency bound masquerading as a model
    //failure. An explicit timeout (env or config) is the user's call and is
    //never raised over their head; the built-in default (600s) already clears
    //this floor, so the guard exists for a future default, not for today.
    if (mThinking && mReasoningBudget > 0 && !getenv("WAGIC_GPT_TIMEOUT")
        && cfg.timeoutSecs == GptSettings().timeoutSecs && mTimeoutMs < 330000)
        mTimeoutMs = 330000;
    mReasoningEffort = cfg.reasoningEffort;
    if (const char * re = getenv("WAGIC_GPT_EFFORT"))
        mReasoningEffort = re;
    //Only the server's own tiers pass; anything else falls to the built-in
    //default rather than 400-ing every decision on a typo.
    if (!mReasoningEffort.empty() && !gptCodexEffortValid(mReasoningEffort))
    {
        gptLogLine("reasoning_effort '" + mReasoningEffort + "' is not one of none/low/medium/high/xhigh/max - using the default");
        mReasoningEffort.clear();
    }
    //Telemetry consent implies local decision logging: the log IS the data
    //a future contribution/upload mechanism would share.
    bool translog = getenv("WAGIC_GPT_TRANSLOG") ? envFlag("WAGIC_GPT_TRANSLOG")
                                                 : (cfg.translog == 1 || cfg.telemetry == 1);
    if (translog)
    {
        //gptUserRoot, NOT getenv("HOME"): this was the fifth raw-HOME site
        //(the Aug 1 sweep fixed four) and it silently disabled the translog
        //on Vita - the one platform where a decision log is hardest to
        //recover any other way.
        string dir = gptUserRoot();
        if (!dir.empty())
        {
            GPT_MKDIR(dir.c_str());
            dir += "/ai"; GPT_MKDIR(dir.c_str());
            dir += "/gpt"; GPT_MKDIR(dir.c_str());
            dir += "/logs"; GPT_MKDIR(dir.c_str());
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

//The model did not answer this decision; the heuristic did. Say so, and keep
//saying so quietly for a while after the notice fades - a run of fallbacks is
//a different thing from one, and the player deserves to see which they are in.
void AIPlayerGPT::noticeFallback(const string& text, float seconds)
{
    setNotice(text, seconds);
    mFallbackCount++;
    mDegradedTicks = 45 * 60; //~45s since the last one; lapses if it recovers
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

//WAVE-33 COMMIT-FAILURE INSTRUMENT (wave-32 synthesis §5). Two cheap,
//string-only counters written onto EVERY decision record. They change NO
//behaviour: the retraction semantics are exactly as they were, and nothing
//reads these back. They exist so the wave-33 seat reviews can count the
//COMMIT-FAILURE class the synthesis promised - a reply that produces a
//complete, protocol-compliant answer and then keeps writing past its own
//PLAN: line ("Wait, looking at the opponent's board...") without ever
//re-committing (deck158 vs139 s16: 12,180 chars at 2 life; the correct
//answer was discarded and Baka answered).

//post_plan_overrun: the number of characters the reply emitted AFTER the end
//of its FIRST "PLAN:" line, trailing whitespace excluded. 0 = the reply
//stopped at its plan, which is what the protocol asks for (answer, brief
//reasoning, PLAN: last). >0 = the reply kept going; the magnitude is the size
//of the un-committed tail. Measured on the same normalisation consumePlan
//uses (post-</think>, case-insensitive "PLAN:"), so the two agree on where
//the plan starts. A reply with NO PLAN: line scores 0 - it never committed a
//plan to overrun, and the empty/unparsed fallbacks already name that case.
static long postPlanOverrun(const string& reply)
{
    string text = reply;
    size_t thinkEnd = text.rfind("</think>");
    if (thinkEnd != string::npos)
        text = text.substr(thinkEnd + 8);
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
        return 0;
    size_t lineEnd = text.find('\n', pos);
    if (lineEnd == string::npos)
        return 0; //the plan line runs to the end: nothing after it
    size_t last = text.find_last_not_of(" \t\r\n");
    if (last == string::npos || last <= lineEnd)
        return 0;
    return (long) (last - lineEnd);
}

//commit_retracted: TRUE when the reply DID emit a line-leading coded answer
//(CHOICE:/ATTACK:/BLOCKS:/PUT:) and the retraction machinery then refused to
//execute it - i.e. the model answered and then took its answer back, so the
//heuristic decided instead. It is exactly the disjunction of the three
//existing retraction/abandonment exits, not a new judgement:
//  retracted_choice              - choiceRetractedNoReplacement fired
//  truncated_abandoned           - truncatedBlockCommitmentAbandoned, safe no-blocks
//  truncated_abandoned_heuristic - the same, with no legal assignment left
//FALSE on every other record, including empty_reply / unparsed_reply /
//stale_echo (no answer was ever committed to retract) and on every record the
//model's own answer carried. Cross-tab with post_plan_overrun to get the
//promised count: replies that overran, and of those, how many retracted.
static bool commitRetracted(const char * fallback, const string& reply)
{
    if (!fallback)
        return false;
    if (strcmp(fallback, "retracted_choice") != 0
        && strcmp(fallback, "truncated_abandoned") != 0
        && strcmp(fallback, "truncated_abandoned_heuristic") != 0)
        return false;
    return hasCodedAnswerLine(reply);
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
    //NATIVE REASONING (wave-34 #1b(A)): the model's own thinking for THIS
    //decision, from message.reasoning_content or from an inline <think> block -
    //captured on both paths because the server decides which one we get. This
    //is what seat review reads in place of the deleted post-answer scratch
    //text, so an absent field means the model genuinely returned none, not
    //that the client only implemented the other path. `reply` above is the
    //STRIPPED text, so every length/answer meter measures reply tokens.
    if (!mLastReasoning.empty())
    {
        rec["reasoning"] = mLastReasoning;
        mLastReasoning.clear();
    }
    //The thinking budget bound on this decision and the answer had to be
    //forced out of the model (phase-2 close). Counting these is how the A/B
    //tells "the budget is never reached" from "the budget is shaping every
    //decision".
    if (mLastBudgetHit)
    {
        rec["reasoning_budget_hit"] = true;
        mLastBudgetHit = false;
    }
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
    //Commit-failure counters (see postPlanOverrun / commitRetracted above).
    //Written on EVERY record, present or zero, so a seat review can divide by
    //the record count without inferring absence.
    rec["post_plan_overrun"] = postPlanOverrun(reply);
    rec["commit_retracted"] = commitRetracted(fallback, reply);
    //Assignments the combat validator pruned as illegal on this record.
    if (!mLastPrunedPairs.empty())
    {
        rec["pruned_pairs"] = mLastPrunedPairs;
        mLastPrunedPairs.clear();
    }
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
        //The Codex backend accepts exactly its verified roster. A foreign id
        //can sit in a config the GUI never re-touches (a preset saved by an
        //older build, a hand edit), and it would fail EVERY decision while
        //looking configured. Heal to the default and say so, rather than play
        //a whole match on the heuristic with a plausible-looking config.
        if (gptCodexEndpoint(mEndpoint) && !mModel.empty())
        {
            bool inRoster = false;
            size_t n = 0;
            const char * const * roster = gptCodexModels(n);
            for (size_t j = 0; j < n; j++)
                if (mModel == roster[j])
                    inRoster = true;
            if (!inRoster)
            {
                gptLogLine("model '" + mModel + "' is not available on the subscription backend - using " + kGptCodexDefaultModel);
                mModel = kGptCodexDefaultModel;
            }
        }
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
        out << manaCostToken(card); //N-36b: {0} is a cost, not an absence
        if (card->isCreature())
            out << " (" << card->power << "/" << card->toughness << ")";
        //A changeling in the decklist: name the tribe(s) this deck consumes so
        //the identity is known before a card is ever drawn (mulligan/keep).
        out << changelingAnnotation(card);
        //Name LAND + colors so the deck's mana base is legible before any draw
        //(deck93 wave-27 land-blindness); "" for non-lands. A basic land also
        //carries no rules text below, so this is its only identity signal here.
        out << landTag(card);
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
    flushOpeningHand(); //always: preserves the opening-hand log line for turn 1+
    //N-93b de-dup: at pregame (turn 0 - mulligan/leyline/bottom asks) the only
    //narration is the just-flushed opening-hand line, and the live "Your hand:"
    //render below shows the SAME cards tagged (strictly more info). Rendering
    //both double-lists the hand at the mulligan prompt. Suppress the GAME LOG
    //here only - the flush above still keeps the line in mNarration, so the
    //first real (turn 1+) decision logs the opening hand as history.
    bool pregame = (observer && observer->turn == 0);
    if (!mNarration.empty() && !pregame)
        u << "GAME LOG (everything that has happened so far):\n" << mNarration << "\n";
    //N-146k, OWNER DIRECTIVE (2026-07-27): the pregame asks (mulligan, London
    //bottoming, leyline) get the HAND and nothing else. The full situation block
    //is a battlefield report - phase, both life totals, the stack, the mana line,
    //both battlefields, opponent hand/library counts, artifact tallies - and every
    //one of those describes a game that has not begun. The mana line in
    //particular ("Mana available: 0 total (no untapped sources)") was read as an
    //instruction that the hand held no mana and drove false mulligans at three
    //seats. Suppressing that ONE line would have left the same frame standing, so
    //the whole board frame is replaced here rather than filtered.
    if (pregame)
        u << "--- YOUR OPENING HAND ---\n" << serializePregameState();
    else
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
        //N-146k: the caveat's own wording points at "the current board", so it is
        //itself a board-scoped statement and is suppressed in pregame. A mulligan
        //reply can carry a PLAN: line, so the second mulligan ask really can reach
        //this branch - the whole board frame has to go, including the sentences
        //that only MENTION one.
        if (!pregame && gptcaveat::planActionsStale(mCurrentPlan, tail, myNames))
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
//Defined below: drops a coded line that merely echoes the protocol's worked
//example (its parenthetical names the placeholder card kExampleFakeCardLc).
static bool isExampleEchoLine(const string& line);

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
                if (match && !isTemplatePlaceholderLine(text.substr(s, end - s))
                    && !isExampleEchoLine(text.substr(s, end - s)))
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
        //Persist / undying return: the just-dead creature re-enters as a fresh
        //copy carrying its signature counter (persist: -1/-1, undying: +1/+1).
        //The engine routes it through a temp zone, but putInZone stamps the event
        //'from' as the card's PREVIOUS zone - the GRAVEYARD - so the raw line
        //reads "<card>: graveyard -> battlefield" and the model loses that the
        //creature it just saw DIE is back (verified live, deck59). Name the
        //mechanic on the return event so the death and the comeback read as one
        //thing (deck59 wave-24). The counter is added only AFTER this zone event,
        //so it is rule-mandated (not yet attached here); the next board snapshot
        //shows the resulting P/T. Gated on graveyard -> battlefield entry of a
        //persist/undying creature (a reanimation of such a creature is rare and
        //the mechanic tag still names its ability faithfully).
        if (owner && e->to == owner->game->inPlay && e->from == owner->game->graveyard
            && e->card->isCreature())
        {
            if (e->card->basicAbilities[Constants::PERSIST])
            {
                out << (mine ? "Your " : "Opponent's ") << cardName
                    << " returns to the battlefield with a -1/-1 counter (persist)";
                return out.str();
            }
            if (e->card->basicAbilities[Constants::UNDYING])
            {
                out << (mine ? "Your " : "Opponent's ") << cardName
                    << " returns to the battlefield with a +1/+1 counter (undying)";
                return out.str();
            }
        }
        out << (mine ? "Your " : "Opponent's ") << cardName
            << ": " << zoneDesc(e->from) << " -> " << toName;
        return out.str();
    }

    if (WEventDamage * e = dynamic_cast<WEventDamage *>(event))
    {
        if (!e->damage)
            return "";
        MTGCardInstance * dsrc = e->damage->source;
        Player * dp = dynamic_cast<Player *>(e->damage->target);
        MTGCardInstance * dc = dynamic_cast<MTGCardInstance *>(e->damage->target);
        //N-105a part (b): infect/poisondamager damage to a PLAYER is REPLACED by
        //poison counters (Damage.cpp:192-199) and infect/wither damage to a
        //CREATURE is REPLACED by -1/-1 counters (Damage.cpp:180-189). Rendered in
        //the ordinary "Damage: N dealt to you" form, both classes read as life
        //loss, and the ONLY tell that the life total never moved was a MISSING
        //life line - an absent line is not a rendered fact.
        if (dp && sourceDealsPoisonInsteadOfDamage(dsrc))
        {
            out << "Infect damage: " << e->damage->damage;
            if (dsrc)
                out << " from " << dsrc->getDisplayName();
            out << " to " << (dp == this ? "you" : "the opponent")
                << " - dealt as POISON COUNTERS, not life loss: no life was lost"
                   " (see the Poison line)";
            return out.str();
        }
        if (dc && dsrc && (dsrc->has(Constants::WITHER) || dsrc->has(Constants::INFECT)))
        {
            out << (dsrc->has(Constants::WITHER) ? "Wither damage: " : "Infect damage: ")
                << e->damage->damage << " from " << dsrc->getDisplayName()
                << " to " << dc->getDisplayName() << " - dealt as " << e->damage->damage
                << " -1/-1 counter" << (e->damage->damage == 1 ? "" : "s")
                << ", a permanent shrink that does NOT wear off at end of turn";
            return out.str();
        }
        out << "Damage: " << e->damage->damage << " dealt";
        if (dsrc)
            out << " by " << dsrc->getDisplayName();
        if (dp)
            out << " to " << (dp == this ? "you" : "the opponent");
        else if (dc)
            out << " to " << dc->getDisplayName();
        if (dp && dsrc && dsrc->getToxicity() > 0)
            out << " - and toxic " << dsrc->getToxicity() << ": it ALSO puts "
                << dsrc->getToxicity() << " poison counter"
                << (dsrc->getToxicity() == 1 ? "" : "s") << " on that player"
                   " (see the Poison line)";
        return out.str();
    }

    //N-105a part (c): every poison GAIN. The engine fires this event on all
    //three paths; the seat had no handler at all, so a non-combat `alterpoison`
    //(Serpent's Slither, a proliferate-class effect) changed the game's actual
    //win condition and narrated NOTHING.
    if (WEventplayerPoisoned * e = dynamic_cast<WEventplayerPoisoned *>(event))
    {
        if (!e->player)
            return "";
        int idx = (e->player == this) ? 0 : 1;
        int total = e->player->poisonCount;
        int delta = total - mLastPoison[idx];
        mLastPoison[idx] = total;
        return poisonGainLine(e->player == this, delta, total);
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
        //N-158b (wave-31 deck158): "- Counter added to Orc army" carried neither
        //the counter's SIZE nor its SOURCE, so a run of identical lines could not
        //be turned back into a board state - four lines meant +8 in one game and
        //two lines meant +2 in another, and the Mauhur bonus was invisible. The
        //engine sends this event AFTER applying the counter (Counters::addCounter
        //increments, then fires), so the permanent's CURRENT P/T is the settled
        //result: print it and there is no arithmetic left to do.
        out << (e->added ? "Counter added to " : "Counter removed from ")
            << e->targetCard->getDisplayName() << instanceHandle(e->targetCard);
        if (!e->name.empty() && e->name != " ")
            out << ": " << e->name;
        else if (e->power || e->toughness)
            out << ": " << (e->power >= 0 ? "+" : "") << e->power << "/"
                << (e->toughness >= 0 ? "+" : "") << e->toughness;
        if (e->targetCard->isCreature())
            out << " (now " << e->targetCard->power << "/" << e->targetCard->toughness << ")";
        //N-158l: the source attribution rendered an EMPTY "[from ]" in 38 distinct
        //prompts - always right after "- Your Mordor Muster: stack -> graveyard".
        //A sorcery that has already finished resolving is in the graveyard by the
        //time its counter event lands, and getDisplayName() comes back empty for
        //it. An empty bracket is a rendered non-fact on this surface's own new
        //annotation: emit the bracket only when a name actually resolves, and
        //fall back to the instance's raw name field before giving up.
        if (e->source && e->source != e->targetCard)
        {
            string sn = e->source->getDisplayName();
            if (sn.empty())
                sn = e->source->name;
            if (!sn.empty())
                out << " [from " << sn << "]";
        }
        return out.str();
    }

    return ""; //everything else (mana plumbing, taps, micro-steps) is noise
}

//N-146f: the "Dungeons completed" status line for CURRENT SITUATION. Empty when
//neither player has completed one (non-dungeon games untouched). Pure helper so
//the emitted shape is deterministically provable (runParseSelfTest).
static string dungeonsCompletedLine(int mine, int opp)
{
    if (mine <= 0 && opp <= 0)
        return "";
    std::ostringstream o;
    if (mine > 0)
        o << "Dungeons completed (you): " << mine
          << " - effects that check \"completed a dungeon\" (e.g. a completed-dungeon"
             " anthem) are ACTIVE for you.\n";
    if (opp > 0)
        o << "Dungeons completed (opponent): " << opp << "\n";
    return o.str();
}

//N-146k + N-139n: the ENTIRE situation block for a pregame ask. Hand only: the
//engine-computed count header, then the tagged hand list. No phase, no life
//totals, no stack, no mana line, no battlefield, no opponent counts, no library
//count - every one of those is information about a game that has not started,
//and a rendered statement outranks the model's own derivation from the cards it
//can see. See pregameHandHeader for the owner's mechanism and the evidence.
string AIPlayerGPT::serializePregameState()
{
    std::ostringstream out;
    out << pregameHandHeader(game->hand);
    out << "Your hand: ";
    describeZoneCards(out, game->hand, false);
    out << "\n";
    return out.str();
}

string AIPlayerGPT::serializeGameState()
{
    std::ostringstream out;
    Player * opp = this->opponent();

    out << "Phase: " << observer->getCurrentGamePhaseName();
    out << " | It is " << (observer->currentPlayer == this ? "your" : "the opponent's") << " turn.\n";
    out << "Your life: " << this->life << " | Opponent life: " << (opp ? opp->life : 0) << "\n";
    //N-105a: poison counters, for BOTH players, whenever either is nonzero.
    //The life line above was the ONLY resource line this seat ever saw, so an
    //infect game presented its entire win/loss condition as invisible - and the
    //pilot filled the gap with invented numbers in 100% of windows across six
    //games. The engine has the number and shows it to the human.
    out << poisonStatusLine(this->poisonCount, opp ? opp->poisonCount : 0);
    //Re-baseline the narration's delta tracker off the live totals. A poison
    //DECREASE fires no event at all (AllAbilities.cpp carries the engine's own
    //"todo loses poison event"), so without this a later gain would report a
    //delta measured from a total that no longer exists. The status line above
    //is always built from the live counts, so this is the natural sync point.
    //Heal DOWNWARD only: a tracked total above the live one can only mean a
    //silent decrease, while a tracked total below it belongs to a gain whose
    //narration has not run yet - clamping that would swallow the delta. The
    //returned string is untouched, so the ask-cache key this function also
    //feeds is unaffected.
    int live[2] = { this->poisonCount, opp ? opp->poisonCount : 0 };
    for (int i = 0; i < 2; i++)
        if (mLastPoison[i] > live[i])
            mLastPoison[i] = live[i];
    //N-146f (wave-29 deck146): dungeon-completion is a hidden boolean the engine
    //tracks (Player::dungeonCompleted) and payoffs check ("as long as you've
    //completed a dungeon" - the Nadaar anthem). With no status line the model could
    //only INFER completion from anthem'd P/T and audibly second-guessed it. Surface
    //the count in the player-status area (only when >0, so non-dungeon games are
    //untouched). Opponent's completions shown too when present (rare, but symmetric).
    out << dungeonsCompletedLine(this->dungeonCompleted, opp ? opp->dungeonCompleted : 0);

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
    //Color REACH, not a flat ability sum: a dual source (Snarl: add{B}/add{W})
    //must show BOTH its colors, and count as ONE source, or the model believes
    //a second-color pip unpayable (N-146d) - the same dual-land collapse that
    //denied casts in the oracle (N-146a). potentialColorReach dedups colors and
    //counts distinct source cards; the engine now really can assemble any single
    //color per dual (the planPayment fix).
    ManaEngine::FreeProducerPolicy manaReachPolicy;
    ManaCost * potential = NEW ManaCost();
    int sources = ManaEngine::potentialColorReach(this, manaReachPolicy, potential);
    string colors = potential->toString();
    SAFE_DELETE(potential);
    //N-158g (wave-31 deck158, HIGH, cost a game at 1 life): this line USED to
    //read "Mana available: {r}{b} from 5 untapped sources" - colour set FIRST -
    //and the pilot bound the leading symbols to the POOL SIZE, concluding "I have
    //{R}{B} available, but Snarling Warg costs {3}{B} (four mana total). I cannot
    //afford to cast it" on an offered, payable cast; it died at -4 the next turn
    //to damage that Warg could have blocked. Eight of that seat's ten largest
    //replies were mana arithmetic. The core prompt says the right thing in three
    //places and loses at distance, so this is a REPRESENTATION fix: lead with the
    //COUNT (the number the arithmetic needs), spell it in words so a digit cannot
    //be misbound to a colour, and demote the colours to a named sub-clause.
    out << "Mana available: " << manaAvailableLine(sources, colors);
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

//The mana a land face taps for, read off its rules text ("{T}: Add {W}" ->
//"{W}"; "{T}: Add {W} or {U}" -> "{W} or {U}"). Empty if the text produces no
//mana. Used to name BOTH faces' colors on a modal-DFC LAND's flip options
//(N-152a): a Pathway's back-face color is otherwise invisible until flipped.
static string landTapMana(const string& text)
{
    string lc = text;
    for (size_t i = 0; i < lc.size(); i++)
        lc[i] = (char) tolower((unsigned char) lc[i]);
    std::vector<string> syms;
    size_t pos = 0;
    //Anchor on "add {": the "// OtherFace" tail and any reminder text never
    //carry a mana symbol right after the word "add", so only real produced
    //mana is captured.
    while ((pos = lc.find("add {", pos)) != string::npos)
    {
        size_t b = pos + 4; //the '{'
        size_t e = lc.find('}', b);
        if (e == string::npos)
            break;
        string sym = text.substr(b, e - b + 1); //original-case "{W}"
        bool dup = false;
        for (size_t k = 0; k < syms.size(); k++)
            if (syms[k] == sym) { dup = true; break; }
        if (!dup)
            syms.push_back(sym);
        //"Add {W} or {U}": keep scanning the same line for chained pips.
        size_t nextAdd = lc.find("add {", e);
        size_t orPip = lc.find(" or {", e);
        if (orPip != string::npos && (nextAdd == string::npos || orPip < nextAdd))
        {
            size_t ob = orPip + 4, oe = lc.find('}', ob);
            if (oe != string::npos)
            {
                string osym = text.substr(ob, oe - ob + 1);
                bool od = false;
                for (size_t k = 0; k < syms.size(); k++)
                    if (syms[k] == osym) { od = true; break; }
                if (!od)
                    syms.push_back(osym);
                pos = oe + 1;
                continue;
            }
        }
        pos = e + 1;
    }
    string out;
    for (size_t k = 0; k < syms.size(); k++)
        out += (k ? " or " : "") + syms[k];
    return out;
}

//The other face's printed name from a DFC face's rules text tail ("{T}: Add
//{G}. // Boulderloft Pathway" -> "Boulderloft Pathway"). Empty when the text
//carries no "// " separator (a non-DFC card).
static string dfcOtherFaceName(const string& text)
{
    size_t p = text.find("// ");
    if (p == string::npos)
        return "";
    string name = text.substr(p + 3);
    size_t nl = name.find_first_of("\r\n");
    if (nl != string::npos)
        name = name.substr(0, nl);
    while (!name.empty() && (name[name.size() - 1] == ' '
           || name[name.size() - 1] == '.' || name[name.size() - 1] == '\t'))
        name.erase(name.size() - 1);
    return name;
}

//A power/toughness-pump activation (Restless Apparition's "{W/B}{W/B}{W/B}:
//gets +3/+3 until end of turn", and every ability of that CLASS) renders its
//menu text as a bare "%i/%i" (APowerToughnessModifier) - which reads as a
//RESULTING P/T stat block and contradicts the "+3/+3" in the option's card
//text. The model cannot reconcile the two and spirals (R-RESTLESS-PUMP-OPTION-
//AMBIGUOUS, deck59 wave-25: 4 of the corpus's 5 largest priority spirals; every
//pump still resolved right, so a latency/verbosity tax). Recover the signed
//modifier delta (dp/dt) and whether it is the until-end-of-turn variant so the
//option can render "+X/+Y until EOT" instead. The GenericActivatedAbility that
//is offered nests the PTInstant (UEOT pump) which owns the APowerToughnessModifier;
//a static/lord modifier is a bare APowerToughnessModifier (no UEOT). Unwrap the
//NestedAbility layers to reach whichever it is.
static bool ptPumpModifierDelta(MTGAbility * a, int& dp, int& dt, bool& ueot,
                                MTGCardInstance *& modTarget)
{
    ueot = false;
    modTarget = NULL;
    APowerToughnessModifier * ptm = NULL;
    MTGAbility * cur = a;
    for (int guard = 0; cur && guard < 6; guard++)
    {
        if (PTInstant * pti = dynamic_cast<PTInstant *>(cur)) { ptm = pti->ability; ueot = true; break; }
        if (APowerToughnessModifier * m = dynamic_cast<APowerToughnessModifier *>(cur)) { ptm = m; break; }
        NestedAbility * na = dynamic_cast<NestedAbility *>(cur);
        if (!na || !na->ability || na->ability == cur)
            break;
        cur = na->ability;
    }
    if (!ptm || !ptm->wppt)
        return false;
    dp = ptm->wppt->power.getValue();
    dt = ptm->wppt->toughness.getValue();
    //The creature the modifier applies to (self for Restless Apparition's
    //self-pump). Used to compute current -> resulting stats only when it is a
    //known creature; a not-yet-chosen "target creature" pump leaves this NULL.
    modTarget = dynamic_cast<MTGCardInstance *>(ptm->target);
    return true;
}

string AIPlayerGPT::describeAction(const OrderedAIAction& action)
{
    std::ostringstream out;
    if (action.ability)
    {
        //PT-pump activations: render the modifier as a signed DELTA with its
        //duration (and, where the pumped creature is identifiable, its
        //current -> resulting P/T) instead of the ambiguous bare "N/N". The
        //"+X/+Y" now agrees with the card text appended below, and the pilot
        //no longer burns reasoning reconciling "3/3" with "+3/+3".
        int dp = 0, dt = 0; bool ueot = false; MTGCardInstance * modTarget = NULL;
        if (ptPumpModifierDelta(action.ability, dp, dt, ueot, modTarget))
        {
            out << (dp >= 0 ? "+" : "") << dp << "/" << (dt >= 0 ? "+" : "") << dt;
            if (ueot)
                out << " until EOT";
            //The pumped creature: an explicit chosen target, else the modifier's
            //own target (the source itself for a self-pump like Restless
            //Apparition). When neither is a known creature, show only the delta -
            //never guess a creature whose stats would be wrong.
            MTGCardInstance * pumped = (action.target && action.target->isCreature())
                                       ? action.target : modTarget;
            if (pumped && pumped->isCreature())
                out << " (" << pumped->power << "/" << pumped->toughness << " -> "
                    << (pumped->power + dp) << "/" << (pumped->toughness + dt) << ")";
        }
        else
            out << action.ability->getMenuText();
    }
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
            //N-152e: on a TRANSFORM DFC the script is "doubleside(backside)" and
            //"backside" is a PLACEHOLDER the engine resolves against the card's
            //backSide field only at resolution time (AAFlip::resolve). The render
            //printed the placeholder verbatim - deck152 saw the option name the
            //other face as literally "backside". Resolve it here the same way.
            if (otherName == "backside")
                otherName = fc->backSide;
            //A TRANSFORM DFC (Innistrad werewolf class) carries a backside= field
            //on BOTH faces; a modal DFC spell (Tergrid class) carries none - it
            //declares its other face through other=/doubleside(<real name>) and
            //an alternative cost. That field is the engine's own discriminator.
            bool isTransformDfc = !fc->backSide.empty() || ats->_SideName == "backside";
            if (!otherName.empty())
            {
                MTGCard * oc = MTGCollection()->getCardByName(otherName, fc->setId);
                bool otherIsLand = oc && oc->data && oc->data->isLand();
                if (fc->isLand() && otherIsLand)
                {
                    //N-152a: a modal-DFC LAND (Pathway class). The old "you do
                    //not need it, the Cast menu lists every face" text was a LIE
                    //here (a land has no Cast menu). Name both faces + colors so
                    //the pilot at least knows the other color exists - but be
                    //HONEST about the engine limitation: verified live (deck199
                    //probe) that flipping DOES swap the hand card's displayed
                    //face, yet the flipped back face then FAILS to enter (the
                    //land-drop offers "Play <back>" but the put-into-play
                    //defers - the front-face isflipped==0 play-restriction
                    //blocks the normal drop once flipped, and the engine's
                    //autohand play path is not reachable from the AI land seam).
                    //So flipping gains NOTHING playable; do not send the model
                    //into a flip-then-fail loop. Display-only, stated plainly.
                    string thisMana = landTapMana(fc->text);
                    string otherMana = landTapMana(oc->data->text);
                    out << " -> DISPLAY TOGGLE only (this is a modal double-faced"
                           " land): it currently shows \"" << fc->getDisplayName() << "\"";
                    if (!thisMana.empty())
                        out << " (taps for " << thisMana << ")";
                    out << "; its other face is \"" << otherName << "\"";
                    if (!otherMana.empty())
                        out << " (taps for " << otherMana << ")";
                    out << ". Flipping only changes which face is DISPLAYED - it"
                           " casts nothing, uses no stack, and gains you nothing"
                           " playable: in this engine only the currently-shown"
                           " face can actually be played as a land. Just play the"
                           " current face.";
                }
                else if (isTransformDfc)
                {
                    //N-152e (wave-31 deck152 seat; Tovolar's Huntmaster vs148
                    //seq36, Brutal Cathar vs139 seq28): the h4 rewrite fixed the
                    //modal-DFC LAND branch and left this one on the OLD text,
                    //which promises the back face "appears there as an
                    //alternative-cost cast". That is FALSE for a transforming
                    //DFC: its back face has no cast route at all - the card only
                    //ever becomes its other face through its own printed
                    //transform condition (daybound/nightbound, an ETB, an
                    //activated transform). Sending the pilot to a Cast menu that
                    //will never list it is the same flip-then-fail loop the land
                    //branch was rewritten to prevent. State the truth and stop.
                    string oCost;
                    if (oc && oc->data && oc->data->getManaCost())
                        oCost = oc->data->getManaCost()->toString();
                    out << transformDfcToggleNote(fc->getDisplayName(), otherName, oCost);
                }
                else if (otherIsLand)
                {
                    //N-152h: a modal-DFC SPELL with a LAND back face (Emeria's
                    //Call // Emeria, Shattered Skyclave). Reaches this branch
                    //because the CURRENT face is not a land, so the land//land
                    //branch above does not catch it - the third card class of
                    //the stale-Flip-Side family.
                    out << mdfcSpellLandBackNote(fc->getDisplayName(), otherName,
                                                 landTapMana(oc->data->text));
                }
                else
                {
                    //Standard modal DFC spell (Tergrid class): both faces cast
                    //from the Cast menu (back via its alternative cost), so the
                    //toggle is a true game no-op.
                    string oCost;
                    if (oc && oc->data && oc->data->getManaCost())
                        oCost = oc->data->getManaCost()->toString();
                    out << mdfcSpellToggleNote(fc->getDisplayName(), otherName, oCost);
                }
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
        //N-36b sweep (same falsy-zero shape): gate on the rendered STRING, not on
        //the converted cost. An {X}-only activation cost also converts to 0, so
        //the old guard dropped it silently; a genuinely free ability still renders
        //nothing, which is the truthful outcome for an ability with no cost.
        {
            string cs = c->toString();
            if (!cs.empty())
            {
                cost << cs;
                any = true;
            }
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
    //Phase-2 of the thinking budget: the same decision prompt, plus the
    //model's own truncated thinking with its "</think>" close injected, so
    //the next tokens it decodes MUST be the answer. The slot key carries the
    //tag; the request carries the base prompt.
    bool forceClose = (userMsg.compare(0, strlen(kForceCloseTag), kForceCloseTag) == 0);
    const string baseMsg = forceClose ? userMsg.substr(strlen(kForceCloseTag)) : userMsg;

    //ChatGPT-subscription preset: the Codex backend speaks the Responses
    //shape, not chat completions - system prompt rides "instructions", the
    //user message rides "input". The backend REJECTS any field Codex CLI
    //does not send (probed 2026-08-09: max_output_tokens and stream:false
    //are both 400s), so this body carries ONLY the six accepted fields; the
    //truncation guard has no decode-side cap here and relies on the reply
    //protocol's brevity pressure. reasoning.effort must be set explicitly:
    //the default (medium) is the same hidden-reasoning latency trap the
    //OpenRouter fix closed. The configured tier wins; unset means "low"
    //(some reasoning, sane latency), and the thinking flag - which belongs
    //to the other provider families - is not consulted here at all.
    if (gptCodexEndpoint(mEndpoint))
    {
        string effort = mReasoningEffort.empty() ? string("low") : mReasoningEffort;
        json request = {
            {"model", mModel.empty() ? string(kGptCodexDefaultModel) : mModel},
            {"instructions", mSystemPrompt},
            {"input", json::array({
                {{"type", "message"}, {"role", "user"},
                 {"content", json::array({{{"type", "input_text"}, {"text", userMsg}}})}},
            })},
            {"reasoning", {{"effort", effort}}},
            {"store", false},
            {"stream", true},
        };
        return request.dump();
    }

    //Exactly two messages, always: the per-duel head and the assembled
    //decision tail. No transcript - the narration inside the user message
    //carries the history, and its append-only front keeps the prefix
    //cacheable across the whole game.
    json messages = json::array();
    messages.push_back({{"role", "system"}, {"content", mSystemPrompt}});
    messages.push_back({{"role", "user"}, {"content", baseMsg}});
    if (forceClose)
    {
        //ASSISTANT PREFILL, not a new instruction. The model resumes its own
        //turn from the closed thinking block, so what it writes next is the
        //answer it would have written had it finished thinking - no
        //re-deliberation, no fresh prompt to spiral on. add_generation_prompt
        //must be false alongside continue_final_message or the template opens
        //a second assistant turn and the prefill becomes context, not prefix.
        messages.push_back({{"role", "assistant"},
                            {"content", "<think>\n" + mForceClosePrefill + "\n</think>\n\n"}});
    }

    //Room for scratch reasoning + the complete PLAN + the trailing answer
    //line. The answer-last contract makes truncation COSTLY (a cut reply
    //loses the decision, not just plan tail): the first live game under
    //1024 lost 13 of 24 decisions to mid-reasoning cuts, and the first
    //full corpus under 2048 still lost 60/1369 (4.4%, median cut length
    //~6.9k chars - long combat math plus some repetition loops). Be
    //generous; the protocol text carries the brevity pressure, and the
    //truncation guard turns any residual cut into a safe heuristic answer.
    long maxTokens = 4096;
    //THINKING BUDGET (owner ruling 2026-08-19). Thinking tokens are decode
    //tokens, so an unbounded native window is an unbounded decision. The
    //budget is a TOKEN COUNT for the thinking window; the request cap is that
    //plus the answer reserve (the p95 PLAN line + the coded choice line +
    //margin), so a model that spends its whole budget still has room to
    //answer - and when it does not, the forced close above recovers the
    //answer instead of losing the decision.
    if (mThinking && mReasoningBudget > 0)
        maxTokens = mReasoningBudget + kAnswerReserveTokens;
    if (mMaxTokens > 0)
        maxTokens = mMaxTokens;
    if (const char * mt = getenv("WAGIC_GPT_MAXTOKENS"))
        maxTokens = atol(mt);
    //Answer-locked retry: the re-ask needs only the coded line, so cap it tight
    //to fail fast to the heuristic instead of burning another long spiral. This
    //wins over any larger configured/env default for the retry request only.
    if (!mRetryActivePrompt.empty() && userMsg == mRetryActivePrompt)
        maxTokens = forceClose ? kAnswerReserveTokens : 512;

    json request = {
        {"model", mModel},
        {"messages", messages},
        {"max_tokens", maxTokens},
        {"temperature", 0.5},
    };
    //SAMPLING FOLLOWS THE MODE (wave-34 #1a). Native thinking and the terse
    //non-thinking reply are two different decode regimes and the model card
    //prescribes different sampling for each; shipping thinking under the
    //non-thinking numbers is a confound the A/B cannot separate from the
    //protocol change itself. Thinking ON -> the thinking preset (temp 0.6 /
    //top_p 0.95); OFF -> the settled non-thinking default (temp 0.5, top_p
    //unset), byte-identical to every corpus before this one. No top_k /
    //presence_penalty is sent in either mode (none ever was here);
    //repetition_penalty is orthogonal to the mode and rides both, below.
    if (mThinking)
    {
        request["temperature"] = 0.6;
        request["top_p"] = 0.95;
    }
    //Qwen-style thinking toggle. Unknown-field-tolerant providers
    //(OpenRouter etc.) ignore this; local vLLM/llama.cpp honor it, keyed
    //or not. Matters: qwen3.6 thinks by default (~6x decision latency).
    //The official OpenAI API is the exception: it REJECTS unknown top-level
    //parameters with a 400, which would silently degrade every decision to
    //the heuristic - omit the field there.
    if (mEndpoint.find("api.openai.com") == string::npos)
        request["chat_template_kwargs"] = {{"enable_thinking", forceClose ? false : mThinking}};
    if (forceClose)
    {
        request["continue_final_message"] = true;
        request["add_generation_prompt"] = false;
    }

    //OpenRouter ignores chat_template_kwargs, so hybrid-reasoning models
    //think anyway. Measured on deepseek-v4-flash-0731 (pinned first-party):
    //~3.5k HIDDEN reasoning tokens per decision behind a ~140-token answer
    //(40-50s wall), and reasoning spirals that hit max_tokens spend the whole
    //budget before any content - the reply comes back EMPTY and the decision
    //falls back to the heuristic. Their unified reasoning field is the switch
    //that actually works there: the same decision replayed 41.9s -> 2.2s.
    //Scoped to openrouter.ai - the field is their extension.
    if (mEndpoint.find("openrouter.ai") != string::npos)
        request["reasoning"] = {{"enabled", mThinking ? true : false}};

    //Repetition damping (decode-side guard against 12-16k-char spirals).
    //vLLM accepts repetition_penalty as a non-standard top-level field; the
    //official OpenAI API rejects unknown top-level params with a 400, so omit
    //it there (same reasoning as chat_template_kwargs). Inert at 1.0, so only
    //sent when explicitly configured != 1.0 (sampling changes play quality
    //and must be corpus-validated before it is ever defaulted on).
    if (mRepetitionPenalty != 1.0 && mEndpoint.find("api.openai.com") == string::npos)
        request["repetition_penalty"] = mRepetitionPenalty;

    //OpenRouter provider pinning (config provider_only=Name[,Name...]). The
    //user named exactly which upstream providers may serve them - send the
    //list with fallbacks DISABLED so a routing miss is a visible error, never
    //a silent detour through an unnamed provider. Same unknown-field rule as
    //above: omitted for api.openai.com.
    if (!mProviderOnly.empty() && mEndpoint.find("api.openai.com") == string::npos)
    {
        json only = json::array();
        std::istringstream ps(mProviderOnly);
        string p;
        while (std::getline(ps, p, ','))
        {
            p.erase(0, p.find_first_not_of(" \t"));
            p.erase(p.find_last_not_of(" \t") + 1);
            if (!p.empty())
                only.push_back(p);
        }
        if (!only.empty())
            request["provider"] = {{"only", only}, {"allow_fallbacks", false}};
    }

    return request.dump();
}

//Strip the render-only annotations a model may copy out of the board/hand text
//into its echoed answer - a bracketed "[changeling: counts as Giant]" /
//"[artifact]" / "[tapped]" tail, or the parenthesized land-identity tag "(land:
//taps for {B})" / "(land)" this build adds at the hand/mulligan surfaces. These
//annotations are NEVER part of a canonical OPTION name, so removing them from
//both an echo and an option before an option-core comparison lets an annotated
//echo reconcile with its bare option (deck22 wave-27 R-ANNOTATION-ECHO-PARSE)
//WITHOUT loosening the match to a different card. ONLY a "(land...)" parenthetical
//is dropped (its content begins with "land"); meaningful parentheticals - (1/1),
//(player, life 20), (printed 2/2) - are preserved. Returns a lowercased,
//whitespace-collapsed core.
static string stripRenderAnnotationsLc(const string& s)
{
    string o;
    for (size_t i = 0; i < s.size(); )
    {
        if (s[i] == '[')
        {
            size_t c = s.find(']', i);
            if (c == string::npos)
                break; //unterminated bracket - stop (rest is annotation-ish)
            i = c + 1;
            continue;
        }
        if (s[i] == '(')
        {
            size_t c = s.find(')', i);
            if (c != string::npos)
            {
                size_t k = i + 1;
                while (k < c && (s[k] == ' ' || s[k] == '\t'))
                    k++;
                if (c - k >= 4
                    && tolower((unsigned char) s[k]) == 'l'
                    && tolower((unsigned char) s[k + 1]) == 'a'
                    && tolower((unsigned char) s[k + 2]) == 'n'
                    && tolower((unsigned char) s[k + 3]) == 'd')
                {
                    i = c + 1; //a land-identity parenthetical - drop it
                    continue;
                }
            }
        }
        o += (char) tolower((unsigned char) s[i]);
        i++;
    }
    //collapse whitespace runs to single spaces + trim
    string r;
    bool sp = false;
    for (size_t i = 0; i < o.size(); i++)
    {
        char ch = o[i];
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
        {
            sp = true;
            continue;
        }
        if (sp && !r.empty())
            r += ' ';
        sp = false;
        r += ch;
    }
    return r;
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

    //Protocol-example parrot (belt-and-suspenders for the example-leak, deck133
    //vs131 s21 class). A coded token whose parenthetical verbatim-echoes the
    //reply protocol's worked example (kExampleFakeCardLc) is a copied format
    //string, not a decision, so exclude it from index resolution. This is the
    //universal choke point: findAnswerLabelLine segments, salvage lines and the
    //retraction walk all resolve through parseChoice, so guarding here closes the
    //real consumePlan flow (not just the salvage path). The de-fang makes the
    //example name NON-LIVE, so this is inert today (no real option is named it) -
    //it keeps the resolver safe should a FUTURE example text again collide with a
    //live option (the exact wave-24 shape). A drop routes to the safe heuristic,
    //and salvageLoopedChoice still recovers any real sibling line.
    if (isExampleEchoLine(text))
        return -1;

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
    string echoLc;        //lowercased echo string (hoisted for INDEX-WINS (a2))
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
            //Hoist the (post-strip) lowercased echo for INDEX-WINS branch (a2).
            echoLc = echo;
            for (size_t i = 0; i < echoLc.size(); i++)
                echoLc[i] = (char) tolower((unsigned char) echoLc[i]);
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
        //(a2) the CHOSEN option's OWN anchor words ALL appear in the echo -> the
        //echo is a FULLER phrasing that WRAPS this option (the model prefixed the
        //option's verb and/or named the source card). R-STALE-ECHO-QUALIFIER
        //residual, deck137 wave-24 s4: a shockland ETB menu offered the bare
        //options "pay 2 life" / "tap"; the model answered CHOICE: 2 (Tap Temple
        //Garden). Branch (a) missed because the echo's >=4-length words
        //("temple"/"garden") name the SOURCE card, which is absent from the bare
        //option "tap"; branch (b) had no anchor because the menu's source name did
        //not render ("Choose an option for :"). But the echo CONTAINS the option's
        //own label ("tap"), so the answer is coherent -> trust the index. Length
        //>=3 here (not the >=4 significance floor) so a short-but-meaningful label
        //like "tap" counts, and >=1 qualifying option word is REQUIRED so an
        //anchorless option ("X = 1") cannot vacuously pass (that must stay stale
        //without a source anchor - W23-A shape1).
        if (optionTexts && !echoLc.empty() && k1 >= 1 && k1 <= (int) optionTexts->size())
        {
            const string& el = echoLc;
            const string& src = (*optionTexts)[k1 - 1];
            vector<string> ow;
            string t;
            for (size_t k = 0; k <= src.size(); k++)
            {
                char c = (k < src.size()) ? (char) tolower((unsigned char) src[k]) : ' ';
                if (isalnum((unsigned char) c))
                    t += c;
                else
                {
                    if (t.size() >= 3 && t != "cast" && t != "with" && t != "play"
                        && t != "pass" && t != "none" && t != "hold" && t != "done"
                        && t != "the" && t != "for" && t != "skip" && t != "decline"
                        && t != "nobody")
                        ow.push_back(t);
                    t.clear();
                }
            }
            if (!ow.empty())
            {
                bool all = true;
                for (size_t k = 0; k < ow.size() && all; k++)
                    all = el.find(ow[k]) != string::npos;
                if (all)
                    return false; //option label fully echoed -> consistent, trust
            }
        }
        //(a3) Annotation-echo tolerance (deck22 wave-27 R-ANNOTATION-ECHO-PARSE):
        //the model copied a render annotation onto the echoed option name - a
        //bracketed "[changeling: counts as Giant]" tail (deck22 vs131 s3, the
        //stale_echo repro) or the "(land: taps for {B})" land tag this build adds -
        //injecting words foreign to the bare option AND, when the option carries a
        //"(1/1)" that the annotation follows, fooling the paren-extraction above
        //into grabbing the annotation instead of the name. Rescue by comparing
        //annotation-stripped cores: if the CHOSEN option's own core name appears
        //verbatim in the (likewise stripped) reply, the answer references THIS
        //option - trust the index. Strictly widen-only: it needs the chosen
        //option's core in the reply, so a DIFFERENT card's echo cannot cross-match,
        //and it never fires without an already-in-range index. Requires a
        //multi-token core (>= 6 chars, has a space) so a bare short label ("tap")
        //stays with branch (a2) and incidental prose cannot trip it.
        if (optionTexts && k1 >= 1 && k1 <= (int) optionTexts->size())
        {
            string optCore = stripRenderAnnotationsLc((*optionTexts)[k1 - 1]);
            if (optCore.size() >= 6 && optCore.find(' ') != string::npos)
            {
                string replyCore = stripRenderAnnotationsLc(text);
                if (replyCore.find(optCore) != string::npos)
                    return false; //chosen option's core name present -> consistent, trust
            }
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

//A coded line whose parenthetical is a verbatim echo of the reply protocol's
//worked example ("CHOICE: 3 (Cast Example Card)", or its remainder "3 (Cast
//Example Card)") - the model parroting the format string, not a decision.
//
//Belt-and-suspenders for the protocol-example leak (deck133 vs131 s21 class,
//wave-24 HARNESS/PARSER). The wave-25 de-fang made the example card name
//(kExampleFakeCardLc) NON-LIVE, so such a line already resolves stale TODAY
//(its foreign echo name matches no offered option -> echoStaleForIndex). But
//that closure is trigger-removal, not a parser guarantee: were a FUTURE example
//text to again collide with a live option name (exactly the wave-24 shape, where
//the old example "Cast Fatal Push" equalled a real option), a line-anchored
//parrot of it would be TAKEN. Excluding it structurally, in the same line-start
//coded-index walk that drops template placeholders, makes the primary resolution
//independent of the de-fang. Scoped to a parenthetical that names the example
//card so an incidental "for example card advantage" in prose cannot trip it, and
//a false drop only routes to the (safe) heuristic.
static bool isExampleEchoLine(const string& line)
{
    string low = line;
    for (size_t i = 0; i < low.size(); i++)
        low[i] = (char) tolower((unsigned char) low[i]);
    size_t open = 0;
    while ((open = low.find('(', open)) != string::npos)
    {
        size_t close = low.find(')', open);
        size_t ex = low.find(kExampleFakeCardLc, open);
        if (ex != string::npos && (close == string::npos || ex < close))
            return true;
        if (close == string::npos)
            break;
        open = close + 1;
    }
    return false;
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
                    //parseChoice drops an example-echo parenthetical (returns -1),
                    //so a parroted example line is skipped here without a separate
                    //guard - the last REAL line-leading choice still wins.
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
                    //An example-echo line resolves to -1 in parseChoice, so it
                    //never becomes the retraction anchor (chosenNum/lastChoiceEnd).
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
    //
    //WAVE-25 ITEM 1 (protocol-example leak, deck133 vs131 s21): a mid-line coded
    //index is only a genuine re-answer when the model WROTE it as prose intent, NOT
    //when it QUOTED the reply protocol's own worked example while agonizing over
    //output format. s21's reply re-affirmed line-1 "CHOICE: 1" repeatedly and never
    //changed its decision, but quoted the prompt's `e.g. "CHOICE: 2 (Cast Fatal
    //Push)"` four times (the old example used option 1's very card name), and the
    //scan latched that quoted "CHOICE: 2" as a contradictory index -> false
    //retraction. TWO exclusions make the scan segment-anchored to real prose:
    //(1) a "choice:" token immediately preceded by a quote character is the model
    //quoting a format string, not answering; (2) a token whose parenthetical names
    //the protocol's fake example card ("Example Card") is a verbatim echo of the
    //example. Both are skipped; a bare prose re-code like "So CHOICE: 1" (deck133
    //vs140 s9) is neither quoted nor an example echo and still fires. Part 3
    //(the fake-card de-fang) prevents the shape at the source going forward; this
    //guard keeps replies generated under the old example (and any future quoting)
    //from mis-firing.
    {
        size_t scan = (size_t) lastChoiceEnd;
        while ((scan = low.find("choice:", scan)) != string::npos)
        {
            //(1) inside quotation marks -> the model is quoting the format/example,
            //not restating an answer. Straight quotes and backtick code spans.
            if (scan > 0 && (low[scan - 1] == '"' || low[scan - 1] == '\''
                             || low[scan - 1] == '`'))
            { scan += 7; continue; }
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
                //(2) verbatim echo of the protocol example: the parenthetical right
                //after the index names the fake example card. Bounded to the same
                //line so a distant paren cannot spuriously match.
                size_t p = e;
                while (p < low.size() && (low[p] == ' ' || low[p] == '\t')) p++;
                bool exampleEcho = false;
                if (p < low.size() && low[p] == '(')
                {
                    size_t close = low.find(')', p);
                    size_t lineEnd = low.find('\n', e);
                    size_t ex = low.find(kExampleFakeCardLc, p);
                    if (ex != string::npos && close != string::npos && ex < close
                        && (lineEnd == string::npos || close <= lineEnd))
                        exampleEcho = true;
                }
                if (exampleEcho) { scan += 7; continue; }
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
    //N-152b: cosmetic-only priority window. When the ONLY non-pass options are
    //modal-DFC Flip-Side display toggles (AATurnSide), the model can gain
    //nothing here and demonstrably loses - it burns a full round trip (deck152:
    //Tovolar's Huntmaster flipped a useless "backside" toggle across ~10
    //consecutive windows at ~200s each) and the bare toggle is a fabrication
    //attractor (vs136 seq27 hallucinated an off-menu "Cast Briarbridge Tracker"
    //-> the corpus's one unparsed fallback). Auto-pass instead of asking. This
    //never skips a real play: a modal-DFC LAND's other-color face is still
    //reachable through its OWN play menu ("Play Land / Flip Side / Decline",
    //where Flip Side is annotated as the route - N-152a), and a DFC spell's
    //other face still casts from the Cast menu. The gate is exact: EVERY shown
    //option must be a Flip-Side toggle; if any real action coexists, the toggle
    //stays offered (annotated) and the model is asked as before.
    bool allTurnSide = true;
    for (size_t s = 0; s < shown.size(); s++)
        if (!asTurnSide(shown[s]->ability))
        {
            allTurnSide = false;
            break;
        }
    if (allTurnSide)
    {
        DebugTrace("AIPlayerGPT[ph" << phase << "]: only display-toggle (Flip Side) options; auto-passing without a model call");
        return NULL;
    }
    tail << "\nWhich action do you take? On the FIRST line write CHOICE: followed by the number (0 = pass priority) and the chosen action's name in parentheses, e.g. \"CHOICE: 3 (Cast Example Card)\" (a placeholder - copy a real number and name from the list) or \"CHOICE: 0 (pass)\"; then, ONLY if your plan changed, a final PLAN: line. Write nothing else.";

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
            noticeFallback("model reply failed or timed out - the heuristic decides", 5.0f);
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
    tail << "\nOn the FIRST line write CHOICE: followed by the number of your choice and its name in parentheses, e.g. \"CHOICE: 3 (Cast Example Card)\" (a placeholder - copy a real number and name from the list); then, ONLY if your plan changed, a final PLAN: line. Write nothing else.";
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
        noticeFallback("model reply failed or timed out - the heuristic decides", 5.0f);
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

//N-139d (wave-29 deck139/deck122/deck93): a mutate alt-cast's cost label was
//inconsistent - Gemrazer rendered "with its mutate cost" (alternativeName ==
//"Mutate"), but Migratory Greathorn (identical `other={..} name(Mutate)`) rendered
//"with its alternative cost" whenever the alternative object being priced had lost
//its name (e.g. a cost-reduced copy - Pollywog on board). The bare "alternative
//cost" label kept the model from recognizing the mutate line up front. Unify: any
//alt-cast of a card with the mutate keyword is a "mutate cost" (canonical
//lowercase, so Gemrazer and Migratory read identically); a NAMED non-mutate
//alternative keeps its own name; an unnamed non-mutate stays "alternative cost".
static string mutateAltCostLabel(bool hasMutate, const string& alternativeName)
{
    if (hasMutate && (alternativeName.empty() || alternativeName == "Mutate"))
        return "mutate cost";
    if (!alternativeName.empty())
        return alternativeName + " cost";
    return "alternative cost";
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
            o << manaCostToken(card); //N-36b: {0} is a cost, not an absence
            if (card->isCreature())
                o << " (" << card->power << "/" << card->toughness << ")";
            o << casts[ci].zoneLabel;
            o << hybridPipNote(cost);
            o << dynamicMagnitudes(card);
        }
        else
        {
            o << "Cast " << card->getDisplayName() << " with its ";
            o << mutateAltCostLabel(card->has(Constants::MUTATE),
                                    cost->getAlternative()->alternativeName);
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
                    //Enumerate ALL legal targets, OPPONENT side FIRST (the
                    //strategically-live burn/removal targets), then the
                    //caster's own. Do NOT truncate behind "(+N more)": a
                    //capped, own-creatures-first preview hid an opponent's
                    //Young Pyromancer (deck109 wave-25 vs131 s11/s8) and the
                    //pilot declined a guide-mandated on-sight kill because it
                    //read the truncated preview as the complete legal set - a
                    //legal play suppressed by omission. Owner ruling: never
                    //hide a legal play on a representation cap. A single-target
                    //preview is cheap (corpus p95 prompt sizes are fine), and
                    //the downstream chooseTarget window already shows the full
                    //list - so the option preview must not disagree with it.
                    //N-158h part 2: a spell whose SELF-COST is "life equal to that
                    //permanent's mana value" (script "life:-manacost") cannot be
                    //priced at option-build time, so the flat "{right now: life
                    //-2}" clause is dropped (see dynamicMagnitudes). Here the
                    //target IS known, so fold the real per-target cost into the
                    //list that already enumerates them - the deciding fact rides
                    //the target it belongs to.
                    bool lifeCostPerTarget = false;
                    {
                        string mtl = card->magicText;
                        for (size_t li = 0; li < mtl.size(); li++)
                            mtl[li] = (char) tolower((unsigned char) mtl[li]);
                        lifeCostPerTarget = mtl.find("life:-manacost") != string::npos;
                    }
                    Player * ordered[2] = { this->opponent(), this };
                    for (int oi = 0; oi < 2; oi++)
                    {
                        Player * pp = ordered[oi];
                        if (!pp)
                            continue;
                        MTGGameZone * zz[] = { pp->game->inPlay, pp->game->graveyard, pp->game->hand, pp->game->exile, pp->game->commandzone };
                        for (int zi = 0; zi < 5; zi++)
                            if (tc->targetsZone(zz[zi]))
                                for (int cj = 0; cj < zz[zi]->nb_cards; cj++)
                                    if (tc->canTarget(zz[zi]->cards[cj]))
                                    {
                                        MTGCardInstance * tgt = zz[zi]->cards[cj];
                                        (tgt->controller() == this ? ownT : oppT)++;
                                        tNames << (tShown++ ? ", " : "") << tgt->getDisplayName()
                                               << instanceHandle(tgt);
                                        //N-158k: the SAME helper the target menu
                                        //now uses, so the cast preview and the
                                        //commit seat cannot drift apart.
                                        if (lifeCostPerTarget)
                                            tNames << perTargetLifeCostNote(tgt);
                                    }
                        if (tc->canTarget(pp))
                        {
                            (pp == this ? ownT : oppT)++;
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
                        //Full list, opponent-first: no "(+N more)" truncation
                        //(see the enumeration comment above).
                        o << " - legal targets right now: " << tNames.str();
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
        //N-146i (wave-31 deck146): activated-ability and loyalty options carry
        //their full {card text: ...} and Cast options carried NONE - 0 of 163 at
        //that seat - so the pilot got rules text for abilities it activated but
        //not for the spells it cast. It cast Acererak the Archlich as a 5/5 body
        //across 59 reply passages planning to attack or block with a creature
        //whose own ETB returns it to hand, and at vs148 seq18 declined an offered
        //Vanishing Verse on the opponent's Kor lord to do it; the board swung 12
        //the next turn.
        //
        //BROAD over narrow, deliberately. The narrow option - annotate only
        //permanents whose ETB bounces them - is itself an instance of THIS wave's
        //dominant defect shape (a fix landing on one shape of one class): it
        //closes Acererak and misses every other card whose deciding text is not
        //on the line. The asymmetry with ability lines is the actual defect, so
        //remove the asymmetry. Cost measured against the corpus: card text= runs
        //mean 157 chars, 143 capped at 220, so a 3-8 option casting ask grows by
        //~0.4-1.1k against prompts already at 10-13k - single-digit percent, and
        //this seat's largest replies are exactly the re-derivations it removes.
        //Skipped where it would add nothing or duplicate: no text, lands (landTag
        //already names what they tap for), and adventure alt-casts (the
        //{adventure spell: ...} clause above is the same text, better scoped).
        if (!card->isLand() && !(casts[ci].viaAlternative && card->has(Constants::ADVENTURE)))
        {
            //Same SHAPE as the ability-line emitter (quoted): one annotation
            //form across both surfaces, so the model reads one convention.
            string ct = cardTextSnippet(card, 220);
            if (!ct.empty())
                o << " {card text: \"" << ct << "\"}";
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

    //DIVERGENCE-C (wave-31 ledger #10), the LOUD RE-ASK. The validation pass
    //below can reject a cast the oracle's offerable() approved. Three trigger
    //shapes were witnessed (a convoke shell whose creature reduction cannot
    //complete the printed cost; a PLAIN cast; an X spell with zero slack), and
    //they shared ONE exit: a silent defer that burned the priority window and
    //never told the model. Accepting on offerable() is NOT the fix - the
    //rejects were substantively RIGHT (an X=0 March of the Multitudes is a
    //scripted `this(X=0) donothing`). So keep the reject and RE-PUT the
    //question with the rejected entry removed, capped, and loud in the log.
    //
    //Cache/dedupe safety: askModel keys on board state + the rendered QUESTION
    //(decision line + option list), never the full prompt. A menu with one
    //entry removed - and a decision line carrying the [RE-ASK n] marker - is a
    //different question, so it takes its own cache slot and cannot collide
    //with the original entry, the priority seam's mLastAskKey, or the deadlock
    //breaker. It is also deterministic on re-poll: the full menu is rebuilt
    //identically each tick, replays its cached answer, rejects again, and the
    //reduced menu replays its own. (The rejected line is deliberately NOT put
    //into mStuckCastLines: that would change the next tick's FULL menu and so
    //the re-ask's question text, turning a cache hit into a fresh HTTP call.)
    const int kMaxCastReasks = 2; //3 asks per window, worst case
    string rejectedSoFar;
    for (int attempt = 0; ; attempt++)
    {
        vector<string> menu(opts);
        menu.push_back("Cast nothing right now"); //the decline goes LAST

        std::ostringstream q;
        q << "Casting decision (" << observer->getCurrentGamePhaseName()
          << (observer->currentPlayer == this ? ", YOUR turn" : ", opponent's turn")
          << "): which card do you cast now, if any?";
        if (attempt > 0)
            q << "\n[RE-ASK " << attempt << "] The engine could not actually complete: "
              << rejectedSoFar << " - its cost or its targets cannot be satisfied right"
                 " now, so it has been REMOVED from the list below. Your priority window"
                 " is still open: decide again over what remains.";

        //no narration: a cast narrates itself as zone events, "nothing" is a non-action
        int pick = askModel(q.str(), menu, false);
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
            mLastCastLine = menu[pick];
            return validated;
        }

        //The cheap menu filter let through something the real machinery rejects
        //(cast restriction, no legal target, unpayable kicker, X-slack 0...).
        gotPayments.clear();
        rejectedSoFar += (rejectedSoFar.empty() ? "" : ", ") + chosen->getDisplayName();
        bool lastChance = (attempt >= kMaxCastReasks) || (candidates.size() <= 1);
        DebugTrace("AIPlayerGPT: model chose " << chosen->name
                   << " but it fails validation; "
                   << (lastChance ? "re-ask budget spent, deferring to heuristic"
                                  : "removing it and re-asking"));
        //Marked so a corpus reviewer can find every re-ask and every
        //exhaustion by fallback reason alone (the old single
        //"deferred_to_heuristic" recorded both the reject and the give-up).
        writeTransLog("defer", "", "", -1, (int) menu.size(), chosen->name,
                      lastChance ? "validation_reject_reask_exhausted"
                                 : "validation_reject_reask");
        candidates.erase(candidates.begin() + pick);
        candidateUsesAlt.erase(candidateUsesAlt.begin() + pick);
        opts.erase(opts.begin() + pick);
        if (lastChance)
        {
            noticeFallback("that cast could not be completed - the heuristic decides", 5.0f);
            return AIPlayerBaka::FindCardToPlay(pMana, type);
        }
        setNotice("that cast could not be completed - asking again", 5.0f);
    }
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
                deferred = true; //model did not decide
                break;
            }
            ordered.push_back(remaining[pick]);
            remaining.erase(remaining.begin() + pick);
        }
        if (deferred)
        {
            //Fall back to the heuristic ranking, not to declaration order:
            //declaration order is the DEFENDER's choice, and keeping it
            //hands the attacker's CR 510.1a decision to the opponent.
            heuristicDamageOrder(atk);
        }
        else if (ordered.size())
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

//A call still in flight after the patience window is one a person has been
//watching with no way to act. The duel screen asks rather than deciding for
//them. No re-entry guard is needed: raising the prompt moves the duel into
//its menu phase, which stops calling game->Update, so mThinkTime freezes
//until the answer comes back and resets it.
bool AIPlayerGPT::aiPatiencePromptDue()
{
    if (mEndpoint.empty() || mPatienceLimit <= 0)
        return false;
    return asyncBusy() && mThinkTime >= mPatienceLimit;
}

void AIPlayerGPT::aiPatiencePromptAnswer(bool keepWaiting)
{
    //Re-arm rather than commit: waiting buys one more window, not an
    //unbounded one, so the choice comes back if the model is truly wedged.
    mThinkTime = 0;
    if (keepWaiting)
        return;
    //Switch off for the rest of the duel. Clearing the endpoint is the same
    //state a player with no endpoint configured is in: decisionPending goes
    //false, all seams fall through to AIPlayerBaka, and isInteractiveAI goes
    //false so card-data "ishuman" gates hand the heuristic AI its dice-roll
    //lines back. The in-flight worker is left alone - AsyncState is a
    //shared_ptr, so the detached thread stays safe and its answer is simply
    //never read.
    mEndpoint.clear();
    setNotice("LLM opponent off - the built-in AI is playing", 6.0f);
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
    //A standing marker, because the notice above is one line for five seconds
    //at the top of the screen while the player is looking at their hand at the
    //bottom of it - faithfully emitted and easily never seen. Two distinct
    //facts, never both: the LLM is OFF for the rest of this duel (switched off,
    //or no endpoint was ever reachable), or it is answering but has been
    //falling back. The second lapses on its own, so a model that recovers
    //stops being accused; a marker that latched forever would become wallpaper
    //and put us right back at effectively-silent failure.
    if (mDegradedTicks > 0)
        mDegradedTicks--;
    if (mEndpoint.empty())
    {
        font->SetColor(ARGB(200, 255, 150, 150));
        font->DrawString("LLM off - built-in AI", 4, SCREEN_HEIGHT - 12, JGETEXT_LEFT);
    }
    else if (mDegradedTicks > 0)
    {
        char dbuf[48];
        sprintf(dbuf, "LLM: %d fallback%s", mFallbackCount, mFallbackCount == 1 ? "" : "s");
        font->SetColor(ARGB(190, 255, 200, 120));
        font->DrawString(dbuf, 4, SCREEN_HEIGHT - 12, JGETEXT_LEFT);
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

//Annotate a shockland-style ETB "pay life or enter tapped" menu in place. The
//engine offers only the bare labels "Pay N life" and "Tap", neither of which
//states the CONSEQUENCE the player is trading on: paying the life lets the
//permanent enter UNTAPPED (usable this turn), while "Tap" declines the payment
//so it instead enters TAPPED. The model saw only the labels and could not weigh
//tempo against life (deck137 wave-24). Append-only - labels and answer indices
//are untouched. Mechanism-matched by the CO-PRESENCE of a "Pay ... life" option
//and a "Tap" option (the pay-or-tapped ETB shape, the only menu that pairs
//them), NOT by card name. Returns true when it fired (for the self-test).
//Case-insensitive helpers: the ACTUAL rendered shock menu options are
//LOWERCASE ("pay 2 life" / "tap", confirmed in the real translog - deck137
//vs109 s4, vs59 s3), while the auto-script name() reads "Pay 2 life"/"Tap".
//The original capital-only matcher therefore never fired on the live menu and
//the options stayed bare (the b4 annotation reached the card TEXT, not the
//menu). Match both forms.
static string toLowerCopy(const string & s)
{
    string r = s;
    for (size_t i = 0; i < r.size(); i++)
        r[i] = (char) tolower((unsigned char) r[i]);
    return r;
}
static bool isPayLifeOption(const string & opt)
{
    string lo = toLowerCopy(opt);
    return lo.compare(0, 4, "pay ") == 0 && lo.find(" life") != string::npos;
}
static bool isTapOption(const string & opt)
{
    //the bare tap branch is exactly "tap" (any case); guard against matching a
    //longer option that merely starts with "tap ..."
    return toLowerCopy(opt) == "tap";
}
static bool annotateEtbPayOrTapMenu(vector<string>& opts)
{
    bool hasPayLife = false, hasTap = false;
    for (size_t i = 0; i < opts.size(); i++)
    {
        if (isPayLifeOption(opts[i]))
            hasPayLife = true;
        if (isTapOption(opts[i]))
            hasTap = true;
    }
    if (!(hasPayLife && hasTap))
        return false;
    for (size_t i = 0; i < opts.size(); i++)
    {
        if (isPayLifeOption(opts[i]))
            opts[i] += " [this permanent then enters the battlefield UNTAPPED -"
                       " usable (tap for mana / attack) this turn]";
        else if (isTapOption(opts[i]))
            opts[i] += " [decline the payment; this permanent instead enters the"
                       " battlefield TAPPED - unusable until your next untap step]";
    }
    return true;
}

//Triggered/activated "may"-ability ask assembly (Tergrid's steal class). The
//card-script option label is a subject-less string ("Put onto battlefield
//under your control") and the source card is the TRIGGER SOURCE - so with no
//object named the model read the verb as acting on the source itself,
//concluded the prompt was a glitch ("Tergrid is already on the battlefield")
//and declined a free permanent (w26 deck199 re-probe). Given the source name
//and the resolved object's name + origin zone (both recovered upstream in
//DecisionManager::buildMenuChoice), build an ask header that NAMES the object
//and marks the source AS the trigger source, and annotate the real option
//(opts[0]; a decline, when offered, is the appended last entry) so its verb
//clearly refers to the object, not the source. Representation only - the
//caller leaves req.optionTexts (the staleness key) untouched. objName is
//non-empty by construction (the caller gates on a resolved object).
static string buildMayObjectAsk(const string & srcName, const string & objName,
                                const string & objOrigin, vector<string> & opts)
{
    string origin = objOrigin.empty() ? string() : (" (from " + objOrigin + ")");
    string src = srcName.empty() ? string("A triggered ability")
                                 : ("Triggered ability of " + srcName);
    string srcRef = srcName.empty() ? string("the source") : srcName;
    if (!opts.empty())
        opts[0] += " [" + objName + origin + " is the card this acts on, NOT " + srcRef + "]";
    return src + " - you MAY act on " + objName + origin + ". " + srcRef
         + " is the trigger SOURCE, not the object being moved. Choose:";
}

//N-139a: role header for the mutate OVER/UNDER placement menu. Pure helper.
static string mutateOverUnderHeader(const string& ctxName)
{
    string who = ctxName.empty() ? string("this creature") : ctxName;
    return "MUTATE PLACEMENT for " + who + ": choose whether " + who + "'s card goes"
           " OVER (on top of) or UNDER the host. This is a middle step of the mutate"
           " cast - you pick WHICH of your non-Human creatures to mutate onto NEXT."
           " OVER keeps " + who + "'s name and P/T on the merged creature; UNDER keeps"
           " the host's. Either way the merged creature gains BOTH cards' abilities"
           " (CR 725). Choose:";
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
                            + (ctx ? ctx->getDisplayName() : string("this spell")) + ":", req.optionTexts,
                            true, ctx ? ctx->getDisplayName() : string());
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
    //Modal-DFC "Flip Side" in the CLICK menu (deck199 Tergrid, observed live at
    //g1 seq21: "1. Cast Card Normally / 2. Flip Side / 3. Decline"). AATurnSide::
    //getMenuText renders it as a bare "Flip Side"; AATurnSide::resolve swaps the
    //hand card's displayed face (name/cost/P/T) but casts nothing, uses no stack,
    //and moves no zone - and BOTH faces stay castable from the Cast menu (the back
    //via its alternative cost), so it is a game no-op for the AI (the sanctioned
    //mechanically-no-op UI dead-end class). It is a real face-data mutation, not a
    //pure display redraw, so per the suppression rule it is ANNOTATED, not filtered
    //- and this keeps the CHOOSE_MENU seat CONSISTENT with the priority seam, which
    //already annotates the same pseudo-action "DISPLAY TOGGLE only" in describeAction.
    //Append-only text: the answer index and the req.optionTexts staleness key are
    //untouched.
    //A "Play Land" option sitting beside "Flip Side" means this IS the modal-DFC
    //LAND play menu (defaultPlayName is "Play Land" only for a land). deck152
    //N-152a: the old "the Cast menu offers the other face" text was a LIE here
    //(a land has no Cast menu). Name both faces + colors so the pilot knows the
    //other color exists - but be HONEST: verified live (deck199 probe) that
    //flipping swaps the DISPLAYED face yet the flipped back face then FAILS to
    //enter (the land-drop offers "Play <back>" but the put-into-play defers -
    //the front-face isflipped==0 restriction blocks the normal drop once
    //flipped). So flipping gains nothing PLAYABLE; do not steer the model into a
    //flip-then-fail loop. The non-land (spell) DFC keeps the true-no-op text.
    bool landFaceMenu = false;
    for (size_t i = 0; i < opts.size() && !landFaceMenu; i++)
        if (opts[i] == "Play Land")
            landFaceMenu = true;
    string curFace, curMana, othFace, othMana;
    if (landFaceMenu && ctx && ctx->isLand())
    {
        curFace = ctx->getDisplayName();
        curMana = landTapMana(ctx->text);
        othFace = dfcOtherFaceName(ctx->text);
        if (!othFace.empty())
        {
            MTGCard * oc = MTGCollection()->getCardByName(othFace, ctx->setId);
            if (oc && oc->data && oc->data->isLand())
                othMana = landTapMana(oc->data->text);
            else
                othFace = ""; //other face is not a land - not the Pathway case
        }
    }
    for (size_t i = 0; i < opts.size(); i++)
        if (opts[i] == "Flip Side")
        {
            if (landFaceMenu && !othFace.empty())
            {
                opts[i] += " [DISPLAY TOGGLE only - this is a modal double-faced land."
                           " It currently shows \"" + curFace + "\"";
                if (!curMana.empty())
                    opts[i] += " (taps for " + curMana + ")";
                opts[i] += "; its other face is \"" + othFace + "\"";
                if (!othMana.empty())
                    opts[i] += " (taps for " + othMana + ")";
                opts[i] += ". Flipping only changes which face is DISPLAYED; it casts"
                           " nothing and gains nothing playable - in this engine only the"
                           " currently-shown face can actually be played as a land. Choose"
                           " Play Land to play the current face.]";
            }
            //N-152e sibling path (the path-scoped sweep): the CHOOSE_MENU seat
            //carried the SAME false "the Cast menu offers the other face as an
            //alternative-cost cast" promise that describeAction did, and it is
            //equally false for a TRANSFORMING DFC - that card is never cast as
            //its other face. The engine's own discriminator is the backSide
            //field (present on both faces of a transform DFC, absent on a modal
            //DFC spell, which declares its other face via other=/doubleside).
            else if (ctx && !ctx->backSide.empty())
            {
                //Same helper as describeAction's branch: one string, two
                //emitters, no drift. Wrapped in [] like every other menu tail.
                opts[i] += " ["
                    + transformDfcToggleNote(ctx->getDisplayName(), ctx->backSide, string())
                        .substr(4) //drop the leading " -> " arrow; this is a bracket tail
                    + "]";
            }
            else
                opts[i] += " [display toggle only - no game effect: switches which face this"
                           " hand card shows; it casts nothing and uses no stack. The Cast menu"
                           " is where you cast, and it offers the other face as an"
                           " alternative-cost cast - you do not need this.]";
        }
    //Dungeon room-BRANCH menu (deck146 N-146b): advancing WITHIN a dungeon offers
    //the next room as a bare name ("Veils of Fear" / "Oubliette") with ZERO effect
    //text - the deciding fact for the venture path is absent (standing P1/P4), and
    //option-1 bias then steers navigation. The active dungeon in the command zone
    //carries every room's readable effect in its text=; attach each branch option's
    //room effect. Self-gating: only options whose text matches a room name of the
    //active dungeon are touched, so every other bare-name menu is untouched.
    //Append-only - the answer index and req.optionTexts (staleness key) are unchanged.
    if (MTGCardInstance * dng = activeDungeon(game))
    {
        std::vector<std::pair<string, string> > rooms;
        parseDungeonRooms(dng->text, rooms);
        for (size_t i = 0; i < opts.size(); i++)
            for (size_t r = 0; r < rooms.size(); r++)
                if (ciStartsWith(opts[i], rooms[r].first))
                {
                    opts[i] += " {room effect: " + rooms[r].second + "}";
                    break;
                }
    }
    //N-139a (wave-29 deck139): the mutate OVER/UNDER placement menu ("Mutate Over"/
    //"Mutate Under") and the cast-mode sub-menu's "mutate" option reached the model
    //as bare labels under the generic "Choose an option" header, asked mid-sequence
    //(before the host is chosen), so the model could not tell it was the mutate
    //placement step and spent minutes re-deriving. Name the role: annotate each
    //placement option with what over/under does, flag the menu for a role header
    //below, and annotate the cast-mode "mutate" option. Self-gating on the exact
    //labels; append-only (answer index + staleness key unchanged).
    bool mutatePlaceMenu = false;
    for (size_t i = 0; i < opts.size(); i++)
    {
        if (opts[i].compare(0, 11, "Mutate Over") == 0)
        {
            mutatePlaceMenu = true;
            opts[i] += " [this card goes ON TOP: the merged creature keeps THIS card's"
                       " name and P/T, and ALSO gains the host's abilities]";
        }
        else if (opts[i].compare(0, 12, "Mutate Under") == 0)
        {
            mutatePlaceMenu = true;
            opts[i] += " [this card goes UNDERNEATH: the creature on top keeps its name"
                       " and P/T, and gains THIS card's abilities]";
        }
        else if (opts[i] == "mutate" || opts[i] == "Mutate")
            opts[i] += " [cast for the MUTATE cost: merge onto one of your non-Human"
                       " creatures (you pick over/under, then the host) instead of"
                       " casting a fresh separate body]";
    }
    //ETB "pay life or enter tapped" menu (shocklands - Temple Garden class, and
    //the fixed painland sibling): append the consequence each bare label omits.
    //Annotation only, mechanism-matched (see annotateEtbPayOrTapMenu). deck137
    //wave-24.
    annotateEtbPayOrTapMenu(opts);
    //Recover the subject name for the header. Some ETB menus arm on an instance
    //whose own name is cleared (getDisplayName() empty) but whose card TEMPLATE
    //(model->data) still carries the printed name - recover it there. The
    //shockland pay-or-tap menu is the WORST case: its entering instance is a
    //stripped copy with name cleared AND model == NULL (verified w26a probe -
    //currentActionCard/menuObject/option-source all point to the same nameless,
    //model-less instance), so no name is recoverable at this seam. Naming it
    //would need an engine-level change to the ETB menu-arming flow.
    string ctxName;
    if (ctx)
    {
        ctxName = ctx->getDisplayName();
        if (ctxName.empty() && ctx->model && ctx->model->data)
            ctxName = ctx->model->data->getName();
    }
    string decision;
    if (!req.mayObjectName.empty())
        //Triggered "may"-ability ask (Tergrid's steal class): name the object
        //and its origin so the subject-less card-script verb stops reading as
        //acting on the trigger source (w26 deck199 re-probe seq 21 - a free
        //3/3 declined). Representation only; the object name is stable within a
        //board state, so the ask cache (board-state + question) does not churn.
        decision = buildMayObjectAsk(ctxName, req.mayObjectName, req.mayObjectOrigin, opts);
    //When the subject name is unrecoverable, fall to a clean generic prompt
    //rather than the grammatically-broken bare "Choose an option for :" the old
    //code emitted (a dangling empty subject after "for", deck137 wave-25).
    else
        decision = !ctxName.empty() ? ("Choose an option for " + ctxName + ":")
                                    : string("A choice is required - choose an option:");
    //N-139a: a mutate placement menu overrides the generic header with a role header.
    if (mutatePlaceMenu)
        decision = mutateOverUnderHeader(ctxName);
    //Thread the source card as the pending source (as ANNOUNCE_X does): the model
    //often echoes "<verb> <source card>" against a bare option ("Tap Temple
    //Garden" vs "tap"), and INDEX-WINS treats a source-naming echo as a
    //self-reference rather than a stale prior answer (deck137 wave-24 s4). Empty
    //when the source name did not render - then (a2) option-label-in-echo covers it.
    int pick = askModel(decision, opts, true, ctxName);
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

//N-139b: map a lowercased "what the engine will do" string (built from the chooser
//source name AND the acting ability's menu verb) to the human verb for a
//remove-from-your-own-cards ask. Returns "" when no remove/lose verb is present.
//Sets relocate=true for a keep-the-card relocation (put back into library) as
//opposed to an outright loss. Pure helper for a deterministic proof.
static string handRemovalVerb(const string& lc, bool& relocate)
{
    relocate = false;
    if (lc.find("discard") != string::npos) return "discard";
    if (lc.find("exploit") != string::npos) return "sacrifice";
    if (lc.find("sacrifice") != string::npos) return "sacrifice";
    if (lc.find("exile") != string::npos) return "exile";
    if (lc.find("bury") != string::npos) return "sacrifice";
    if (lc.find("put in library") != string::npos || lc.find("put back") != string::npos)
    {
        relocate = true;
        return "put back into your library";
    }
    return "";
}

//N-139b: build the verb-labeled ask for a remove-from-your-own-cards selection.
//Three framings share the "pick your least valuable" logic but name WHO caused it
//and WHAT happens: a keep-the-card relocation, an OPPONENT-forced loss (the
//validated deck140 wording, kept byte-identical), and a self-inflicted loss (a
//loot's own discard). Pure helper so the exact emitted strings are provable.
static string buildHandRemovalAsk(const string& verb, bool byOpponent, bool relocate,
                                  const string& effectName, bool multi, bool unlimited,
                                  bool targetMin, int maxtargets, size_t pickIndex)
{
    std::ostringstream q;
    if (relocate)
    {
        q << "PUT ONE OF YOUR OWN CARDS BACK: " << effectName << "'s effect will "
          << verb << " a card you choose from the list below - these are YOUR OWN"
             " cards, this is NOT a target you attack or affect. Pick the card you"
             " least need to keep in hand right now. ";
    }
    else if (byOpponent)
    {
        q << "FORCED " << verb << " OF YOUR OWN CARD"
          << ((multi && maxtargets != 1) ? "S" : "")
          << ": the opponent's effect (" << effectName << ") forces YOU to "
          << verb << " one of your OWN cards from the list below - each"
             " option is a card YOU will LOSE, not something you affect or"
             " attack. Pick the card you can best AFFORD TO LOSE (usually your"
             " LEAST valuable: pitch a spare land or a redundant/dead card,"
             " and KEEP your best spells, answers, and threats). ";
    }
    else
    {
        string VERB = verb;
        for (size_t i = 0; i < VERB.size(); i++) VERB[i] = (char) toupper((unsigned char) VERB[i]);
        q << VERB << " ONE OF YOUR OWN CARDS: your own effect (" << effectName
          << ") makes you " << verb << " a card from the list below - each option is"
             " a card YOU will LOSE, NOT a target you attack or affect. Pick the card"
             " you can best AFFORD TO LOSE (usually your LEAST valuable: a spare land"
             " or a redundant/dead card; KEEP your best spells, answers, and threats). ";
    }
    if (!multi)
        q << (relocate ? "Choose the ONE card to put back" : "Choose the ONE card to give up");
    else
    {
        q << "Choose card " << (pickIndex + 1);
        if (!unlimited)
            q << " of " << (targetMin ? "exactly " : "up to ") << maxtargets;
    }
    q << " from the list below, and answer with the chosen card's name.";
    return q.str();
}

//N-139a: role-named ask for the mutate-HOST target step (placement 1=over,
//2=under, already chosen). Names what this step is and what the placement means,
//so the model stops re-deriving the whole mutate sequence here. Pure helper.
static string mutateHostAsk(const string& effectName, int placement)
{
    std::ostringstream q;
    q << "MUTATE - CHOOSE THE HOST creature to mutate onto (placement step for "
      << effectName << "; the over/under choice is already made). You chose "
      << (placement == 1
            ? ("OVER: " + effectName + "'s card goes ON TOP, so the merged creature keeps "
               + effectName + "'s name and P/T and ALSO gains the host's abilities")
            : ("UNDER: " + effectName + " goes BENEATH, so the host keeps its name and P/T and gains "
               + effectName + "'s abilities"))
      << ". Each option is one of your non-Human creatures; the two become ONE merged"
         " creature (CR 725). Pick the ONE creature to mutate onto, and answer with its name.";
    return q.str();
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
    //N-158k (wave-32 HL5.3, the COMMIT seat): Feed the Swarm's per-target life
    //cost ("life:-manacost" - you pay life equal to the chosen permanent's mana
    //value) renders on the CAST line and won a game there, but the TARGET menu -
    //where the target is actually committed - carried rules text and no price at
    //all. deck152 answered it at 11 life and again at 6 with the number invisible.
    //The standing rule is that the deciding fact rides the seat where the choice
    //commits, so the identical "(costs you N life)" fragment is emitted here too.
    bool lifeCostPerTarget = false;
    if (tc && tc->source)
    {
        string mtl = tc->source->magicText;
        for (size_t li = 0; li < mtl.size(); li++)
            mtl[li] = (char) tolower((unsigned char) mtl[li]);
        lifeCostPerTarget = mtl.find("life:-manacost") != string::npos;
    }
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
                opts.push_back(describeTarget(this, t)
                               + (lifeCostPerTarget ? perTargetLifeCostNote(t) : string()));
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
        //N-158d (wave-31 deck158): when a SPELL grants an ability to a permanent
        //and that ability then asks for a target (Foray of Orcs transforms a
        //"deal damage" ability onto the Army it just amassed), tc->source is the
        //PERMANENT, so the header read "TARGET CHOICE for Orc army" - naming the
        //creature the pilot controls as the thing being targeted, which invites a
        //self-target read. The waiting action element knows the EFFECT's own name
        //("Damage creature"); when it differs from the permanent, say both, so
        //the header names what is happening rather than who is holding it.
        string abilityName;
        {
            MTGAbility * waiting = dynamic_cast<MTGAbility *>(observer->mLayers->actionLayer()->isWaitingForAnswer());
            if (waiting)
                abilityName = waiting->getMenuText();
        }
        if (effectName.empty())
        {
            //Granted/inner abilities ride a nameless fake card.
            effectName = abilityName;
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
        //N-139b (wave-29 deck139/deck93, PRIORITY): a loot / draw-then-discard (and
        //any remove-from-your-own-hand cost/effect) reaches this same seam - the
        //legal set is entirely the deciding player's OWN cards - but its chooser
        //SOURCE is the CARD name (e.g. "Pollywog Symbiote"), so the old
        //source-name-only verb check MISSED it and rendered the generic "TARGET
        //CHOICE ... pick the ONE target it will affect": the model read its own hand
        //as spell targets and discarded a land it needed (deck93 s20, a 14,181-char
        //reply one class short of the truncation cliff). Fix: derive the VERB the
        //engine will apply from the ACTING ABILITY too - AADiscardCard::getMenuText
        //is "Discard", AASacrificeCard "Sacrifice"/"Exploit", AAMover's zone-aware
        //menu names the destination ("Put in Library"/"Exile"). Build the detection
        //string from the source name AND that ability verb, and split the framing:
        //self-inflicted (your own loot) vs opponent-forced (the validated deck140
        //path, kept byte-identical) vs a keep-the-card relocation (put back).
        bool forcedSelfLoss = false;
        bool relocateNotLoss = false;
        bool selfInflicted = false;
        string lossVerb;
        {
            string lc = effectName;
            MTGAbility * waiting = dynamic_cast<MTGAbility *>(observer->mLayers->actionLayer()->isWaitingForAnswer());
            if (AAMover * mv = dynamic_cast<AAMover *>(waiting))
            {
                //AAMover's menu verb is zone-aware (dest + chooser source zone);
                //pass the current chooser so hand->library reads "Put in Library".
                lc += " ";
                lc += mv->getMenuText(tc);
            }
            else if (waiting)
            {
                lc += " ";
                lc += waiting->getMenuText();
            }
            for (size_t i = 0; i < lc.size(); i++) lc[i] = (char) tolower((unsigned char) lc[i]);
            lossVerb = handRemovalVerb(lc, relocateNotLoss);
            if (!lossVerb.empty() && !targets.empty())
            {
                bool allMine = true;
                for (size_t i = 0; i < targets.size(); i++)
                {
                    //the "Done" escape is a string option, never in targets.
                    MTGCardInstance * mc = dynamic_cast<MTGCardInstance *>(targets[i]);
                    if (!mc || mc->controller() != this) { allMine = false; break; }
                }
                forcedSelfLoss = allMine;
                //Self-inflicted (a loot, a sacrifice-as-cost of MY OWN spell) vs an
                //OPPONENT forcing the loss: only my own source flips to the self
                //wording; anything else keeps the validated opponent framing.
                selfInflicted = tc->source && tc->source->controller() == this;
            }
        }

        //Dungeon SELECTION (N-146c): the first venture reaches this target seam
        //with the sideboard dungeons as the candidate set. The generic "TARGET
        //CHOICE ... pick the ONE target it will affect" wording mis-frames it as
        //targeting a permanent (it is choosing WHICH dungeon to venture into).
        //Detect the shape (every candidate is a Dungeon) and reframe as a venture
        //choice. Representation only - the offered set and apply path are unchanged;
        //describeTarget already renders each dungeon's full room path + summary.
        bool dungeonSelect = !targets.empty();
        for (size_t i = 0; i < targets.size() && dungeonSelect; i++)
        {
            MTGCardInstance * dc = dynamic_cast<MTGCardInstance *>(targets[i]);
            if (!dc || !dc->hasType(Subtypes::TYPE_DUNGEON))
                dungeonSelect = false;
        }

        //N-139a (wave-29 deck139): the mutate cast is a scrambled multi-ask
        //sequence (cast -> over/under -> TARGET), and the mutate-HOST target step
        //reached the model as the same bare "TARGET CHOICE ... target it will
        //affect" line as any removal - so the model spent minutes re-deriving what
        //it was choosing. The over/under placement is asked BEFORE this step (engine
        //order), so by now it is decided; name the ROLE of THIS step. The waiting
        //ability is an AANewTarget carrying the placement (1 over / 2 under).
        int mutatePlacement = 0;
        {
            MTGAbility * waiting = dynamic_cast<MTGAbility *>(observer->mLayers->actionLayer()->isWaitingForAnswer());
            if (AANewTarget * nt = dynamic_cast<AANewTarget *>(waiting))
                mutatePlacement = nt->mutation;
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
            q << buildHandRemovalAsk(lossVerb, !selfInflicted, relocateNotLoss,
                                     effectName, multi, unlimited, tc->targetMin,
                                     tc->maxtargets, picks.size());
        }
        else if (mutatePlacement)
        {
            q << mutateHostAsk(effectName, mutatePlacement);
        }
        else if (dungeonSelect)
        {
            q << "VENTURE - CHOOSE A DUNGEON to enter (you are picking WHICH dungeon"
                 " to venture into, NOT targeting a permanent). Each option below is a"
                 " dungeon; its tag shows the room count and the completion reward, and"
                 " its full room path follows. Weigh how many rooms to completion and"
                 " whether the completion payoff and the rooms en route fit your plan"
                 " (a dungeon whose completion turns on your payoffs is usually worth"
                 " the shorter path). Pick the ONE dungeon to venture into, and answer"
                 " with its name.";
        }
        else
        {
        if (stackTrapNote)
            q << "NOTE: these targets are battlefield permanents only - the spell"
                 " being cast on the stack is NOT a legal target and is NOT in this"
                 " list. If your only goal was to stop that spell, this cannot do"
                 " it; pick a battlefield permanent that is worth bouncing, or"
                 " decline.\n";
        q << "TARGET CHOICE for " << effectName;
        if (!abilityName.empty() && abilityName != effectName)
            q << " - its \"" << abilityName << "\" ability";
        q << " (this spell/ability is already on the stack and needs a target - "
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

//Strip a leading protocol ANSWER LABEL ("BLOCKS:", "ATTACK:", "CHOICE:",
//"PUT:", "BLOCK:") - with any markdown decoration - off a reply SEGMENT.
//
//WAVE-33 N-152j/N-158n. The name->label reconciler splits the reply into
//segments and takes the FIRST ':' as the blocker/attacker separator. On the
//segment that carries the head label that colon is the LABEL'S OWN, so
//"BLOCKS: Orc army: Sigarda" split into leftSeg="BLOCKS" (no significant
//words -> no blocker match) and the whole assignment was dropped. Every
//name-form BLOCKS reply in the wave-32 corpus died here - both of that
//corpus's unparsed_reply fallbacks - even though the name pass has shipped
//since wave-19. Removing the label makes the FIRST colon the real separator.
static string stripAnswerLabelPrefix(const string& seg)
{
    static const char * kLabels[] = { "blocks:", "block:", "attack:", "attacks:", "choice:", "put:" };
    size_t s = 0;
    while (s < seg.size() && (seg[s] == ' ' || seg[s] == '\t' || seg[s] == '*'
                              || seg[s] == '#' || seg[s] == '-' || seg[s] == '>'
                              || seg[s] == '`'))
        s++;
    for (size_t k = 0; k < sizeof(kLabels) / sizeof(kLabels[0]); k++)
    {
        size_t len = strlen(kLabels[k]);
        if (seg.size() - s < len)
            continue;
        bool match = true;
        for (size_t c = 0; c < len && match; c++)
            match = (tolower((unsigned char) seg[s + c]) == kLabels[k][c]);
        if (match)
            return seg.substr(s + len);
    }
    return seg;
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
    bool persist;      //has persist AND no -1/-1 counter yet -> returns once if it dies
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
    //normal-step damage lands on a dead creature). A creature killed in the
    //first-strike step deals NO damage at all - so any effect that rides its
    //damage (trample, and the wither/infect SHRINK below) must not be claimed.
    bool bDiesToFirstStrike = a.firststrike && !b.firststrike && aKillsB;
    bool aDiesToFirstStrike = b.firststrike && !a.firststrike && bKillsA;
    if (bDiesToFirstStrike)
        bKillsA = false;
    else if (aDiesToFirstStrike)
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
    //or, pre-fix, as dead. Only when the target actually survives (!kills) AND
    //the wither creature actually LIVED TO DEAL its damage: a wither blocker
    //that dies to the attacker's first strike before the normal step never
    //applies its -1/-1 counters, so the shrink must not be claimed (deck27
    //wave-24: the shrink was printed for a wither blocker already dead to first
    //strike). bDiesToFirstStrike / aDiesToFirstStrike mark that no-damage case.
    if (b.wither && bp > 0 && !bKillsA && !bDiesToFirstStrike)
    {
        int np = ap - bp; if (np < 0) np = 0;
        int nt = at - bp; //survives => bp < at, so nt >= 1
        o << " (" << (b.infectLabel ? "infect" : "wither")
          << " shrinks it to " << np << "/" << nt << ")";
    }
    if (a.wither && ap > 0 && !aKillsB && !aDiesToFirstStrike)
    {
        int np = bp - ap; if (np < 0) np = 0;
        int nt = bt - ap; //survives => ap < bt, so nt >= 1
        o << " (" << (a.infectLabel ? "infect" : "wither")
          << " shrinks your blocker to " << np << "/" << nt << ")";
    }
    //Persist: a creature that DIES here but has persist (and no -1/-1 counter
    //yet) comes straight back with a -1/-1 counter, so "both die" / "dies"
    //understates what the model keeps: the body returns, one smaller. Note it
    //on whichever side dies. Keyed on the FINAL kill flags, so a persist blocker
    //that dies to first strike still returns (death is death, order aside).
    if (aKillsB && b.persist)
        o << " (yours returns with a -1/-1 counter (persist))";
    if (bKillsA && a.persist)
        o << " (theirs returns with a -1/-1 counter (persist))";
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
    //Persist returns the creature once, with a -1/-1 counter - but only if it
    //does not already carry one (the counter is what ends the loop). Undying is
    //the +1/+1 mirror; scoped to persist here (deck59 wave-24).
    s.persist = c->basicAbilities[Constants::PERSIST] && c->counters && !c->counters->hasCounter(-1, -1);
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

//===================== WAVE-25 ITEM 2 =====================================
//Natural-stop vs truncation classifier + the unified answer-line precedence it
//drives across ALL coded-answer paths.
//
//PRECEDENCE RULES (CHOICE / ATTACK / BLOCKS / PUT), stated in one place:
//  * The reply protocol puts the ANSWER first, then optional scratch reasoning,
//    then PLAN: last. So the FIRST coded line is the answer-first commitment and
//    the LAST well-formed coded line is the model's final answer after reasoning.
//  * TRUNCATED reply (hit the token cap: no PLAN: line and a long/mid-word cut):
//    prefer the FIRST well-formed coded line. The tail is unreliable - a cut
//    reply's later lines are decode spiral or half-thoughts (deck133 s21 shape),
//    so answer-first wins ([C] truncated answer-first still takes line 1).
//  * NATURALLY TERMINATED reply (reached its PLAN: line, or a short compliant
//    answer ending on terminal punctuation): prefer the model's FINAL answer -
//    the LAST well-formed coded line, OR, when the correction lived ONLY in prose
//    (deck27 vs137 bottom: line 1 "PUT: 3,5,6" bottomed its only blue source, but
//    the reasoning reached and stated "So I bottom 5, 6, and 7."), the last
//    explicit prose restatement (salvageProsePutList).
//  * A "later"/"last" coded line only counts when it is a WELL-FORMED answer for
//    the CURRENT ask - proper labels, in range - NOT combat-math prose. The ATTACK
//    path keeps ANSWER-FIRST precisely because a CoT line ("Attack: Deal 1, Take 5.
//    Net -4 life.") shares the ATTACK label and its bare prose numbers parse to a
//    bogus subset (deck109 vs62 s21); taking the LAST such line would resurrect the
//    wave-23 bug, so ATTACK trusts the FIRST well-formed declaration and never a
//    trailing bare-number line (this is both the truncated->first rule AND the
//    well-formedness guard, so ATTACK needs no separate natural->last branch).
//  * INDEX-WINS / echo-staleness / retraction still gate every candidate line.
//
//A reply terminated naturally iff it reached the contract's terminal PLAN:
//section, or (legacy short answers) it is short and ends on terminal punctuation.
//A long reply with no PLAN: is a token-cap cut.
static bool replyTerminatedNaturally(const string& content)
{
    string t = content;
    size_t thinkEnd = t.rfind("</think>");
    if (thinkEnd != string::npos)
        t = t.substr(thinkEnd + 8);
    //A line-leading PLAN: marker = the model completed the full reply contract.
    size_t lineStart = 0;
    while (lineStart <= t.size())
    {
        size_t lineEnd = t.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? t.size() : lineEnd;
        size_t s = lineStart;
        while (s < end && (t[s] == ' ' || t[s] == '\t' || t[s] == '*'
                           || t[s] == '#' || t[s] == '-'))
            s++;
        if (end - s >= 5
            && tolower((unsigned char) t[s]) == 'p' && tolower((unsigned char) t[s + 1]) == 'l'
            && tolower((unsigned char) t[s + 2]) == 'a' && tolower((unsigned char) t[s + 3]) == 'n'
            && t[s + 4] == ':')
            return true;
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    //No PLAN: marker. Trim trailing whitespace; a long remainder is a cut spiral.
    size_t e = t.find_last_not_of(" \t\r\n");
    if (e == string::npos)
        return false; //empty
    if (e + 1 > 1200)
        return false; //long with no PLAN: -> token-cap truncation
    char last = t[e];
    return last == '.' || last == '!' || last == '?' || last == ')' || last == '"';
}

//===================== WAVE-29 ITEM 2 (N-18e) ============================
//A TRUNCATED blockers reply whose early coded BLOCKS: line committed to a
//block, then whose reasoning reversed to a no-block conclusion but ran out of
//tokens before emitting a corrected/terminal answer. The parser's last-coded-
//line precedence would honor the stale early commitment - a fatal reach-trade
//the model had ALREADY talked itself out of (deck18 vs93 s20: emitted
//"BLOCKS: B1:A1", looped "So I should NOT block ... blocking is strictly worse",
//hit the token ceiling mid-sentence with no terminator; the block executed and
//the Soldier died for nothing, contradicting the reply's own conclusion).
//
//Fires ONLY when BOTH hold, so real answers are never masked:
//  (1) the reply is TRUNCATED (replyTerminatedNaturally == false). A naturally
//      terminated reply is trusted as-is: the model's final answer stands and
//      the normal last-line precedence resolves any self-correction.
//  (2) a GLOBAL no-block decline conclusion appears AFTER the committing coded
//      line. A truncated reply whose early commit was NEVER contradicted keeps
//      it (unchanged) - the defect is specifically contradicted-then-truncated.
//The "not block" family is guarded against "cannot block" (a legality note, not
//a decline of intent). ATTACK keeps its own answer-first CoT-hijack guard and
//is untouched; this is scoped to the blockers seam where the defect was seen.
//WAVE-32 (N-122d): condition (2)'s GLOBALITY is now actually enforced - see the
//two helpers below and their call sites. The predicate had been firing on
//per-creature bookkeeping and throwing compliant answers away.
//
//N-122d helper: is there ANOTHER line-leading, committing BLOCKS: line after
//`from`? (Same line shape the commit scan below accepts: a digit in the
//remainder and not a "none".) A truncated reply that re-states its assignment -
//or states a corrected one - has not abandoned it.
static bool laterCodedBlockAssignment(const string& content, size_t from)
{
    size_t lineStart = from;
    while (lineStart < content.size())
    {
        size_t lineEnd = content.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? content.size() : lineEnd;
        size_t s = lineStart;
        while (s < end && (content[s] == ' ' || content[s] == '\t' || content[s] == '\n'
                           || content[s] == '*' || content[s] == '#' || content[s] == '-'))
            s++;
        static const char * kLabel = "BLOCKS:";
        if (end - s >= 7)
        {
            bool m = true;
            for (int k = 0; k < 7 && m; k++)
                m = (toupper((unsigned char) content[s + k]) == kLabel[k]);
            if (m)
            {
                bool hasDigit = false;
                string remLow;
                for (size_t i = s + 7; i < end; i++)
                {
                    if (isdigit((unsigned char) content[i]))
                        hasDigit = true;
                    remLow += (char) tolower((unsigned char) content[i]);
                }
                if (hasDigit && remLow.find("none") == string::npos
                    && remLow.find("no block") == string::npos)
                    return true;
            }
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    return false;
}

//N-122d helper: is the decline phrase at `pos` scoped to a NAMED combat
//participant rather than global? Looks at the enclosing sentence (bounded by
//. ! ? ; : newline) for a B<n>/A<n> label token. `low` is already lowercased.
static bool declineSentenceIsLabelScoped(const string& low, size_t pos)
{
    size_t start = 0;
    for (size_t i = pos; i > 0; i--)
    {
        char c = low[i - 1];
        if (c == '.' || c == '!' || c == '?' || c == ';' || c == ':' || c == '\n')
        {
            start = i;
            break;
        }
    }
    size_t end = low.size();
    for (size_t i = pos; i < low.size(); i++)
    {
        char c = low[i];
        if (c == '.' || c == '!' || c == '?' || c == ';' || c == '\n')
        {
            end = i;
            break;
        }
    }
    for (size_t i = start; i + 1 < end; i++)
    {
        if (low[i] != 'b' && low[i] != 'a')
            continue;
        if (i > start && (isalnum((unsigned char) low[i - 1]) || low[i - 1] == '_'))
            continue; //mid-word letter, not a label
        if (!isdigit((unsigned char) low[i + 1]))
            continue;
        return true;
    }
    return false;
}

static bool truncatedBlockCommitmentAbandoned(const string& content)
{
    if (content.empty())
        return false;
    if (replyTerminatedNaturally(content))
        return false; //natural stop: the reply's final answer is trustworthy

    //Locate the FIRST line-leading BLOCKS: that COMMITS a block (a digit in the
    //remainder and not a bare "none"); scan only the tail after it.
    size_t commitLineEnd = string::npos;
    size_t lineStart = 0;
    while (lineStart <= content.size())
    {
        size_t lineEnd = content.find('\n', lineStart);
        size_t end = (lineEnd == string::npos) ? content.size() : lineEnd;
        size_t s = lineStart;
        while (s < end && (content[s] == ' ' || content[s] == '\t'
                           || content[s] == '*' || content[s] == '#' || content[s] == '-'))
            s++;
        static const char * kLabel = "BLOCKS:";
        if (end - s >= 7)
        {
            bool m = true;
            for (int k = 0; k < 7 && m; k++)
                m = (toupper((unsigned char) content[s + k]) == kLabel[k]);
            if (m)
            {
                bool hasDigit = false;
                string remLow;
                for (size_t i = s + 7; i < end; i++)
                {
                    if (isdigit((unsigned char) content[i]))
                        hasDigit = true;
                    remLow += (char) tolower((unsigned char) content[i]);
                }
                bool none = remLow.find("none") != string::npos
                    || remLow.find("no block") != string::npos;
                if (hasDigit && !none)
                {
                    commitLineEnd = end;
                    break;
                }
            }
        }
        if (lineEnd == string::npos)
            break;
        lineStart = lineEnd + 1;
    }
    if (commitLineEnd == string::npos)
        return false; //no committing coded line -> nothing to abandon

    //N-122d guard 1 (wave-31 ledger #12): a LATER complete coded assignment in
    //the tail is not an abandonment - it is a self-correction, and the shipped
    //line-anchored precedence (last well-formed BLOCKS: line wins) already owns
    //that case. Only the prose-only reversal is this predicate's business.
    if (laterCodedBlockAssignment(content, commitLineEnd))
        return false;

    string tail;
    for (size_t i = commitLineEnd; i < content.size(); i++)
        tail += (char) tolower((unsigned char) content[i]);
    static const char * kDecline[] = {
        "not block", "no block", "won't block", "wont block",
        "do not block", "don't block", "dont block", "not to block", "no benefit"
    };
    for (size_t d = 0; d < sizeof(kDecline) / sizeof(kDecline[0]); d++)
    {
        const char * phrase = kDecline[d];
        size_t plen = strlen(phrase);
        size_t pos = 0;
        while ((pos = tail.find(phrase, pos)) != string::npos)
        {
            //Guard "not block" against "cannot block" (legality, not intent).
            bool legality = false;
            if (strcmp(phrase, "not block") == 0 && pos >= 3
                && tail.compare(pos - 3, 3, "can") == 0)
                legality = true;
            //N-122d guard 2: only a GLOBAL decline abandons the commit. The
            //comment above this function always claimed globality; the code
            //never checked it, so per-creature bookkeeping in the SAME
            //deliberation that re-derives the head line ("B3 will not block",
            //"B4 blocking A1 has no benefit") read as a reversal and threw a
            //compliant answer away - twice in the wave-31 corpus, both times
            //defaulting to the worst possible blockers answer. A decline
            //sentence that names a specific B<n>/A<n> participant is scoped to
            //that creature, not a decision to sit out combat.
            if (!legality && declineSentenceIsLabelScoped(tail, pos))
                legality = true;
            if (!legality)
                return true;
            pos += plen;
        }
    }
    return false;
}

//Natural-stop PUT/bottom prose reconciler (the deck27 shape). A bottom reply whose
//answer-first coded line is wrong but whose reasoning reaches the right set and
//states it in PROSE ("So I bottom 5, 6, and 7."). Scan for the LAST "bottom"/"put"
//statement (negation-guarded) immediately followed by a CLEAN comma/and-separated
//list of EXACTLY `need` distinct in-range card numbers, and return that set. The
//strict shape - a bottom verb, then only digits + ", "/" and "/whitespace up to the
//exact count, terminating at the first other token - keeps ordinary reasoning prose
//(full of numbers) from matching. Caller gates on natural termination (a truncated
//reply's late prose is unreliable) and prefers this over the coded first line only
//when it yields the full count. Returns the count, or -1 when no clean full list.
static int salvageProsePutList(const string& content, size_t n, int need, vector<bool>& send)
{
    if (need <= 0)
        return -1;
    string low;
    for (size_t i = 0; i < content.size(); i++)
        low += (char) tolower((unsigned char) content[i]);
    int best = -1;
    long bestPos = -1; //document position of the winning list (LAST wins across verbs)
    vector<bool> bestSend;
    static const char * kVerbs[] = { "bottom", "put" };
    for (size_t vi = 0; vi < sizeof(kVerbs) / sizeof(kVerbs[0]); vi++)
    {
        const char * verb = kVerbs[vi];
        size_t vlen = strlen(verb);
        size_t pos = 0;
        while ((pos = low.find(verb, pos)) != string::npos)
        {
            size_t verbPos = pos;
            pos += vlen;
            if (proseNegatedBefore(low, verbPos))
                continue;
            //skip a short run of separators (": ", spaces) to the first digit
            size_t k = pos;
            size_t guard = (pos + 6 < low.size()) ? pos + 6 : low.size();
            while (k < guard && !isdigit((unsigned char) low[k])
                   && (low[k] == ' ' || low[k] == ':' || low[k] == ',' || low[k] == '\t'))
                k++;
            if (k >= low.size() || !isdigit((unsigned char) low[k]))
                continue;
            //read a CLEAN list: digits, and only ", " / " and " / whitespace between
            std::set<int> nums;
            bool clean = true;
            size_t q = k;
            while (q < low.size())
            {
                if (isdigit((unsigned char) low[q]))
                {
                    int num = 0; size_t d = q;
                    while (d < low.size() && isdigit((unsigned char) low[d]))
                        num = num * 10 + (low[d++] - '0');
                    if (d < low.size() && low[d] == '/') { clean = false; break; } //"2/2" stat
                    if (num < 1 || num > (int) n) { clean = false; break; }
                    nums.insert(num);
                    q = d;
                    if ((int) nums.size() > need) { clean = false; break; }
                }
                else if (low[q] == ',' || low[q] == ' ' || low[q] == '\t')
                    q++;
                else if (low.compare(q, 3, "and") == 0)
                    q += 3;
                else
                    break; //any other char terminates the list
            }
            if (clean && (int) nums.size() == need && (long) verbPos > bestPos)
            {
                vector<bool> s(n, false);
                for (std::set<int>::iterator it = nums.begin(); it != nums.end(); ++it)
                    s[*it - 1] = true;
                bestSend = s; best = need; bestPos = (long) verbPos; //LAST in document wins
            }
        }
    }
    if (best > 0) { send = bestSend; return best; }
    return -1;
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
            //';' is a separator here too (wave-33 N-158n symmetry): the ATTACK
            //name pass already tolerates names, but a ';'-joined list arrived
            //as ONE segment whose merged word set matched nothing, so an
            //otherwise-compliant "ATTACK: Hellrider; Rakdos Cackler" declared
            //NOBODY. The A#-index scan above is separator-independent and
            //unaffected.
            if (s != content.size() && content[s] != ',' && content[s] != ';' && content[s] != '\n')
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
        ln << "A" << (j + 1) << ". " << attackers[j]->name << instanceHandle(attackers[j])
           << " (" << attackers[j]->power << "/" << attackers[j]->toughness << ")";
        string kw = keywordList(attackers[j]);
        if (!kw.empty())
            ln << " [" << kw << "]";
        //N-139j (wave-31 deck139): the BLOCKERS menu annotates a 0-power creature
        //("[deals 0 - this block kills nothing...]") and the ATTACKERS menu did
        //not, so 0/3 Arboreal Grazers were declared as attackers in FOUR separate
        //declarations, one of them reasoning out loud that "attacking with all
        //three 0/3 Grazers deals 3 damage". Effective power is already computed
        //for the "(P/T)" above; name the consequence at the line that decides,
        //mirroring the validated blocker text.
        ln << zeroPowerAttackerTag(attackers[j]->power);
        shownLines.push_back(ln.str());
        tail << ln.str() << "\n";
    }
    tail << "On the FIRST line write ATTACK: followed by the attackers you send,"
            " comma-separated (e.g. \"ATTACK: A1, A3\"), or \"ATTACK: none\" to"
            " attack with nobody this turn; then, ONLY if your plan changed, a"
            " final PLAN: line. Write nothing else.";
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
        noticeFallback("model reply failed - the heuristic attacks", 5.0f);
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
            //';' joins name-form pairs as readily as ',' does (deck158 vs152
            //s35: "Orc army: Sigarda, Champion of Light; Dunland Crebain:
            //Moonrage Brute") - without it the second pair never reaches a
            //segment of its own. WAVE-33 N-158n.
            if (s != content.size() && content[s] != ',' && content[s] != ';' && content[s] != '\n')
                continue;
            string seg = stripAnswerLabelPrefix(content.substr(start, s - start));
            start = s + 1;
            //Find the blocker/attacker separator: ':' or the word "blocks".
            //A leading colon whose left half carries NO name words (a stray
            //"Answer:"/"Declaring:" prefix the label strip does not know)
            //re-splits at the NEXT separator rather than dropping the pair.
            string low;
            for (size_t k = 0; k < seg.size(); k++)
                low += (char) tolower((unsigned char) seg[k]);
            size_t searchFrom = 0;
            while (searchFrom <= seg.size())
            {
                size_t sep = string::npos, sepLen = 0;
                size_t colon = seg.find(':', searchFrom);
                size_t blk = low.find(" blocks ", searchFrom);
                if (colon != string::npos) { sep = colon; sepLen = 1; }
                if (blk != string::npos && (sep == string::npos || blk < sep)) { sep = blk; sepLen = 8; }
                if (sep == string::npos)
                    break; //no blocker:attacker structure left in this segment
                string leftSeg = seg.substr(0, sep), rightSeg = seg.substr(sep + sepLen);
                vector<string> leftWords, rightWords;
                significantWords(leftSeg, leftWords);
                significantWords(rightSeg, rightWords);
                if (leftWords.empty())
                {
                    searchFrom = sep + sepLen; //label-ish prefix: try the next separator
                    continue;
                }
                int bMatch = uniqueNameMatch(leftWords, *blockerNames, nBlockers, NULL, nameOrdinal(leftSeg));
                if (bMatch < 0 || out[bMatch] != 0)
                    break; //no unique blocker, or already assigned by a code
                const vector<int> * allowed = (legalPerBlocker && (size_t) bMatch < legalPerBlocker->size())
                                              ? &(*legalPerBlocker)[bMatch] : NULL;
                //"#N" on the attacker half ("Saproling (1/1) #1") disambiguates
                //two identically-named attackers the plain match would drop.
                int aMatch = uniqueNameMatch(rightWords, *attackerNames, nAttackers, allowed, nameOrdinal(rightSeg));
                //LEGALITY IS THE VALIDATOR'S JOB, NOT THE PARSER'S (wave-33).
                //Restricting the attacker match to the blocker's legal set is a
                //DISAMBIGUATOR, so it is tried first; when it fails, retry over
                //the whole attacker list so the model's actual intent is
                //RESOLVED and then PRUNED at the apply site's canBlock gate
                //(and counted in the translog), exactly as a coded "B1:A5"
                //naming an illegal attacker already is. Silently dropping it
                //here made an illegal intent indistinguishable from an
                //unparseable reply (deck139 s21, deck158 s35).
                if (aMatch < 0 && allowed)
                    aMatch = uniqueNameMatch(rightWords, *attackerNames, nAttackers, NULL, nameOrdinal(rightSeg));
                if (aMatch < 0)
                    break; //ambiguous / unmatched attacker -> drop this one only
                out[bMatch] = aMatch + 1;
                pairs++;
                break;
            }
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
    //N-105b: the swing is PARTITIONED by whether each attacker's damage to a
    //player is REPLACED. Infect/poisondamager damage becomes poison counters and
    //never touches life; toxic damage hits life AND adds counters; wither only
    //replaces damage dealt to CREATURES, so it is ordinary damage on this line
    //(its blocker consequence rides the attacker line instead).
    {
        int lifeIncoming = 0, poisonIncoming = 0;
        for (size_t j = 0; j < attackers.size(); j++)
        {
            int p = attackers[j]->power > 0 ? attackers[j]->power : 0;
            if (sourceDealsPoisonInsteadOfDamage(attackers[j]))
                poisonIncoming += p;
            else
            {
                lifeIncoming += p;
                if (p > 0)
                    poisonIncoming += attackers[j]->getToxicity();
            }
        }
        tail << combatDamageForecast(life, poisonCount, lifeIncoming, poisonIncoming);
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
        ln << "A" << (j + 1) << ". " << attackers[j]->name << instanceHandle(attackers[j])
           << " (" << attackers[j]->power << "/" << attackers[j]->toughness << ")";
        //POWER is the damage number, not toughness. The model misread a
        //Saproling "(2/4)" as dealing 4 (deck35 wave-18 G1); state the damage
        //explicitly at the line that decides.
        ln << " deals " << (attackers[j]->power > 0 ? attackers[j]->power : 0);
        string kw = keywordList(attackers[j]);
        if (!kw.empty())
            ln << " [" << kw << "]";
        //N-105a part (d): spell out what infect / toxic / wither damage actually
        //DOES, at the line that decides the block. The engine keyword names for
        //this family are opaque, and the seat's own reasoning showed it pricing
        //an infect swing entirely on the life track.
        ln << attackerPoisonNote(attackers[j]);
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
        ln << "B" << (i + 1) << ". " << blockers[i]->name << instanceHandle(blockers[i])
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
            " block with nobody this turn; then, ONLY if your plan changed, a final"
            " PLAN: line. Write nothing else.";
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

    //WAVE-29 N-18e: the reply committed a block in an early coded line, then
    //reasoned itself OUT of blocking but was cut off by the token ceiling before
    //a corrected answer. Honoring the stale commit is a fatal reach-trade the
    //model already rejected (deck18 vs93 s20). Declare the SAFE combat default
    //(no blockers). Only fires on truncated-AND-contradicted; a well-terminated
    //reply, or a truncated commit never contradicted, keeps its answer.
    //N-122d (wave-31 ledger #12) narrows this: the predicate now ignores a
    //LATER coded assignment (self-correction, owned by line precedence) and
    //per-creature "B3 will not block" bookkeeping, so a compliant head line
    //whose reasoning merely re-derives it is no longer discarded. What still
    //fires is a genuine GLOBAL prose reversal - and that IS the model's
    //answer, so no-blocks stands there. The blanket part of the old default is
    //gone: when the abandoned commit also has no legally executable
    //assignment left, the HEURISTIC blocks instead of declining combat
    //outright (a blanket no-blocks maximizes incoming damage and is the
    //adjudication tiebreaker - the worst answer to default to).
    if (pairs > 0 && truncatedBlockCommitmentAbandoned(content))
    {
        int legalPairs = 0;
        for (size_t i = 0; i < blockers.size(); i++)
            if (pick[i] >= 1 && pick[i] <= (int) attackers.size()
                && blockers[i]->canBlock(attackers[pick[i] - 1]))
                legalPairs++;
        if (!legalPairs)
        {
            writeTransLog("blockers", userMsg, content, 0, (int) blockers.size(),
                          "", "truncated_abandoned_heuristic", &shownLines);
            noticeFallback("model reply cut off mid-reversal - the heuristic blocks", 5.0f);
            mBlocksDoneTurn = observer->turn;
            DebugTrace("AIPlayerGPT: truncated-abandoned commit with no legal assignment"
                       " -> heuristic declares blockers");
            return AIPlayerBaka::chooseBlockers();
        }
        DecisionAction none;
        DecisionManager::applyDeclareBlockers(req, none);
        writeTransLog("blockers", userMsg, content, 0, (int) blockers.size(),
                      "no blockers", "truncated_abandoned", &shownLines);
        noticeFallback("model reply cut off mid-reversal - no blocks (safe default)", 5.0f);
        narrateDecision("You declared no blockers");
        mBlocksDoneTurn = observer->turn;
        DebugTrace("AIPlayerGPT: truncated-abandoned block commit -> safe no-blocks default");
        return 1;
    }

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
        noticeFallback("model reply failed - the heuristic blocks", 5.0f);
        mBlocksDoneTurn = observer->turn;
        return AIPlayerBaka::chooseBlockers();
    }

    //Answer through the contract: the manager owns the defenser-cycling
    //choreography and re-validates each assignment.
    DecisionAction act;
    string declared;
    int intended = 0;
    string pruned;
    for (size_t i = 0; i < blockers.size(); i++)
    {
        if (pick[i] < 1)
            continue;
        intended++;
        MTGCardInstance * chosen = attackers[pick[i] - 1];
        if (!blockers[i]->canBlock(chosen))
        {
            //Model assigned an illegal block: that blocker stays home. THIS is
            //the seam that keeps illegal choices structurally impossible, and
            //it is the only legality gate the name-form parse relies on - so
            //record what it pruned rather than losing it (wave-33 N-152j).
            pruned += (pruned.empty() ? "" : "; ") + blockers[i]->name + " -> " + chosen->name;
            continue;
        }
        act.blocks.push_back(std::make_pair(blockers[i], chosen));
        declared += (declared.empty() ? "" : "; ") + blockers[i]->name + " blocks " + chosen->name;
    }
    mLastPrunedPairs = pruned;

    //EVERY intended assignment was illegal (deck139 vs152 s21: a name-form
    //reply naming a flier its own B-line never offered). Declaring the empty
    //set here would silently convert "the model asked for an impossible
    //block" into "the model declined to block" - a blanket no-blocks is the
    //worst combat default (it maximises incoming damage), and wave-32 ledger
    //#15 flagged this exit as the silent one. Fall back to the heuristic, as
    //every other failure path does, and log it as its OWN fallback class so
    //the next corpus can count it separately from unparsed_reply.
    if (intended > 0 && act.blocks.empty())
    {
        writeTransLog("blockers", userMsg, content, pairs, (int) blockers.size(),
                      "", "all_assignments_illegal", &shownLines, blockSource);
        noticeFallback("every block the model asked for was illegal - the heuristic blocks", 5.0f);
        mBlocksDoneTurn = observer->turn;
        DebugTrace("AIPlayerGPT: all " << intended << " assignment(s) illegal -> heuristic declares blockers");
        return AIPlayerBaka::chooseBlockers();
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
             << "; then, ONLY if your plan changed, a final PLAN: line. Write nothing else.";
    else
        tail << "On the FIRST line write PUT: followed by the card numbers you send to \""
             << optOneLabel << "\", comma-separated (e.g. \"PUT: 1, 3\"), or exactly"
                " \"PUT: none\" to send none there (every revealed card then goes to \""
             << optTwoLabel << "\"); then, ONLY if your plan changed, a final PLAN: line. Write nothing else.";
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

    //eligibleForOptionOne is the engine's OWN per-card acceptance verdict for
    //option one (tc->canTarget - exactly what the click enforces). When it is
    //populated it is the same size/order as `revealed`; when it is empty the
    //reveal is not predicate-gated (a plain surveil/look-at-top-N) and every
    //card may go to option one.
    bool haveElig = (eligibleForOptionOne.size() == revealed.size());
    int eligCount = (int) revealed.size();
    if (haveElig)
    {
        eligCount = 0;
        for (size_t j = 0; j < eligibleForOptionOne.size(); j++)
            if (eligibleForOptionOne[j]) eligCount++;
    }

    //N-136a: a predicate-gated reveal with NOTHING eligible for option one is
    //not a decision - do NOT ask the model. Mausoleum Secrets with an empty
    //graveyard gates option one on "black card, MV <= creature cards in
    //graveyard" = MV<=0 = no card in the library qualifies. The seam still
    //LISTS every revealed card (soft "[does NOT qualify]" tag), so the model
    //happily "picked" an ineligible bomb (MV4 Ritual of Soot), the engine's
    //click filter bounced it, the tutor no-opped, and the model mis-planned for
    //turns around a card it never received. An empty legal set auto-resolves to
    //the card's fail branch (option two: shuffle/keep) via the display's safe
    //default (-1 -> nothing to option one) - offer and enforcement cannot
    //diverge because the offer never happens.
    if (haveElig && eligCount == 0)
    {
        writeTransLog("reveal", "", "", -1, (int) revealed.size(),
                      "none (no legal target)", NULL, NULL);
        narrateDecision("You revealed " + std::to_string(revealed.size())
                        + " card" + (revealed.size() == 1 ? "" : "s")
                        + " but none was a legal target - took none");
        return -1;
    }

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
        noticeFallback("model reply failed - reveal kept the default", 5.0f);
        return -1;
    }

    string chosen;
    for (size_t j = 0; j < revealed.size(); j++)
        if (send[j])
        {
            //N-136a enforcement filter: never ACCEPT a pick the option-one
            //chooser will reject at the click - the same canTarget verdict in
            //eligibleForOptionOne. The soft "[does NOT qualify]" annotation is
            //advisory only; a model that picks an ineligible card anyway must
            //not have that pick recorded as chosen/narrated as tutored (the
            //engine would silently bounce it). Dropping it here keeps the
            //translog + narration consistent with what the engine does, so the
            //model is never told it holds a card that never reached hand.
            if (haveElig && j < eligibleForOptionOne.size() && !eligibleForOptionOne[j])
                continue;
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
    mPregameMullsSeen = mullsTaken; //the true count, for the bottom ask (N-139i)
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
string AIPlayerGPT::buildPregameBottomAskText(const vector<MTGCardInstance*>& hand, int need,
                                             int alreadyBottomed)
{
    std::ostringstream tail;
    //N-139i: every number here is now the number it claims to be. The old text
    //printed the CARDS-TO-BOTTOM count wearing a "mulligans" label (a different
    //quantity once the count is clamped, and it walked downward mid-loop), and
    //it never said that this one ask covers the whole set.
    int remaining = need - alreadyBottomed;
    if (remaining < 0)
        remaining = 0;
    if (remaining > (int) hand.size())
        remaining = (int) hand.size();
    int keep = (int) hand.size() - remaining;
    //mPregameMullsSeen is stamped by pregameMulliganDecision on every round, so
    //it is the engine's own count; the max() is a belt for a seat that somehow
    //reaches bottoming without having been asked (the two quantities are equal
    //whenever the count is not clamped).
    int mulls = (mPregameMullsSeen > need) ? mPregameMullsSeen : need;
    tail << "London mulligan bottoming (CR 103.5): you took " << mulls
         << " mulligan" << (mulls == 1 ? "" : "s") << " and kept, so you must put "
         << need << " card" << (need == 1 ? "" : "s")
         << " from your hand on the BOTTOM of your library";
    if (alreadyBottomed > 0)
        tail << " (" << alreadyBottomed << " already bottomed; " << remaining << " to go)";
    tail << ". Name EXACTLY " << remaining << " card" << (remaining == 1 ? "" : "s")
         << " now - this is the ONLY ask for them, and they will be bottomed one at a"
            " time in the order you give. ";
    if (keep <= 0)
        tail << "Your ENTIRE hand goes to the bottom, so order them worst-first.\n";
    else
        tail << "You will be left with a " << keep << "-card hand, so keep your best "
             << keep << " and bottom your worst " << remaining << ".\n";
    for (size_t j = 0; j < hand.size(); j++)
    {
        tail << (j + 1) << ". " << hand[j]->name << changelingAnnotation(hand[j]);
        if (hand[j]->isLand())
            tail << landTag(hand[j]); //land + colors it taps for (deck93 wave-27), not a bare "(land)"
        else
            //N-36b: the same symbolic token every other hand surface prints, so
            //a {0} card reads identically here and the colour pips the pregame
            //count header is reasoning about are visible on the card's own line.
            tail << manaCostToken(hand[j]);
        string txt = cardTextSnippet(hand[j], 120);
        if (!txt.empty())
            tail << " {text: " << txt << "}";
        tail << "\n";
    }
    tail << "On the FIRST line write PUT: followed by the " << remaining << " card number"
         << (remaining == 1 ? "" : "s") << " you send to the bottom, comma-separated (e.g. \"PUT: "
         << (remaining == 1 ? "3" : "3, 5") << "\"); then, ONLY if your plan changed, a final PLAN: line. Write nothing else.";
    return tail.str();
}

MTGCardInstance * AIPlayerGPT::pregameChooseBottom(int need, int chosenSoFar, int & status)
{
    status = 0;
    if (mEndpoint.empty())
        return AIPlayerBaka::pregameChooseBottom(need, chosenSoFar, status);

    //ONE bundled ask per keep; then pop the queued cards one per call. `need`
    //is the TOTAL owed for this keep and is stable across the loop since the
    //N-139i clamp fix, so this asks exactly once; `remaining` keeps the ask
    //honest in the residual case where the engine re-enters with a new total.
    int remaining = need - chosenSoFar;
    if (remaining < 0)
        remaining = 0;
    if (!mPregameBottomAsked || mPregameBottomForMulls != need)
    {
        vector<MTGCardInstance*> hand;
        for (int i = 0; i < game->hand->nb_cards; i++)
            hand.push_back(game->hand->cards[i]);
        if ((int) hand.size() < remaining)
            remaining = (int) hand.size();
        if (mSystemPrompt.empty())
            buildSystemPrompt();
        string userMsg = assemblePrompt(buildPregameBottomAskText(hand, need, chosenSoFar));
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
        //Natural-stop reconciliation (wave-25 ITEM 2): the coded FIRST line is
        //answer-first, but a reply that reasoned to a DIFFERENT bottom set and
        //stated it in PROSE while terminating naturally (deck27 vs137: line 1
        //"PUT: 3, 5, 6" bottomed its only blue source, then "So I bottom 5, 6, and
        //7.") meant the prose conclusion. On a non-truncated reply prefer the LAST
        //clean full-count prose list over the answer-first coded line. A truncated
        //reply keeps answer-first (its late prose is unreliable).
        if (!content.empty() && replyTerminatedNaturally(content))
        {
            vector<bool> prose;
            int pr = salvageProsePutList(content, hand.size(), need, prose);
            if (pr == need)
            {
                send = prose;
                result = pr;
                DebugTrace("AIPlayerGPT: bottom prose-reconciled to the final stated set");
            }
        }
        if (result < 0 && !content.empty())
        {
            int sal = salvageLoopedSubset(content, "PUT:", hand.size(), names, send);
            if (sal >= 1)
                result = sal;
        }
        mPregameBottomQueue.clear();
        string chosen;
        if (result >= 0)
            for (size_t j = 0; j < hand.size() && (int) mPregameBottomQueue.size() < remaining; j++)
                if (j < send.size() && send[j])
                {
                    mPregameBottomQueue.push_back(hand[j]);
                    chosen += (chosen.empty() ? "" : ", ") + hand[j]->name;
                }
        //Enforce EXACTLY the owed count: the model under-picked or failed ->
        //fill with the highest-cost cards not already chosen (the heuristic
        //policy). We already capped above, so over-picks are trimmed.
        //This was the one seam that fell back to the heuristic and said
        //nothing - every other decision announces it, so this one does too.
        if (result < 0 || (int) mPregameBottomQueue.size() < remaining)
            noticeFallback("model reply failed - the heuristic bottoms cards", 5.0f);
        while ((int) mPregameBottomQueue.size() < remaining)
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

    cout << "=== AIPlayerGPT parse self-test (wave-20 3a/3b/3c + wave-21 A/B/C + wave-22 N9/A2/B3 + wave-25 1/2/3 + wave-26 example-echo) ===\n";

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

    // ---- ITEM 3b-w27: R-DUPLICATE-NAME-INSTANCE render->reply round-trip ----
    // The board/B-lines now print a "#N" handle for same-named permanents
    // (wave-26 deck137 s36: two Lovestruck Beasts, one tapped one untapped; the
    // undecorated B-line let the model decline a free block). Verify a reply
    // that answers with the RENDERED handle binds to the right blocker, and that
    // the positional B# label binds correctly no matter the name decoration.
    cout << "\n[3b-w27] two 'Lovestruck Beast' blockers; B2 is the untapped copy offered vs A1\n";
    {
        vector<string> bn; bn.push_back("Lovestruck Beast"); bn.push_back("Lovestruck Beast");
        vector<string> an; an.push_back("Zealous Guardian");
        // only B2 (the untapped copy) may legally block A1 in this scenario
        vector<vector<int> > legal(2); legal[1].push_back(0);
        // name+handle reply: "#2" picks the 2nd Lovestruck = the offered untapped one
        vector<int> out;
        int pairs = parseBlockAssignments("Lovestruck Beast #2: Zealous Guardian", 2, 1, out, &bn, &an, &legal);
        cout << "     name+'#2': pairs=" << pairs << " B1->A" << out[0] << " B2->A" << out[1] << "\n";
        CHECK(pairs == 1 && out[0] == 0 && out[1] == 1, "3b-w27 'Lovestruck Beast #2' blocks A1 (right instance)");
        // positional label reply: "B2:A1" binds to blockers[1] regardless of name
        vector<int> out2;
        int pairs2 = parseBlockAssignments("B2:A1", 2, 1, out2, &bn, &an, &legal);
        cout << "     label 'B2:A1': pairs=" << pairs2 << " B1->A" << out2[0] << " B2->A" << out2[1] << "\n";
        CHECK(pairs2 == 1 && out2[0] == 0 && out2[1] == 1, "3b-w27 label B2:A1 binds positionally (decoration-independent)");
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

    // ---- WAVE-25 items: wither-under-first-strike + persist return ----
    // fields: power, toughness, deathtouch, wither, infectLabel, firststrike, indestructible, trample, persist
    cout << "\n[A2] wither shrink respects first-strike order (deck27 wave-24)\n";
    {
        // A wither blocker that DIES to the attacker's first strike deals no
        // damage - no shrink may be claimed.
        CombatTradeStat witherBlk   = { 2, 1, false, true,  false, false, false, false, false };
        CombatTradeStat fsAtk34     = { 3, 4, false, false, false, true,  false, false, false };
        string sfs = combatTradePreviewStats(witherBlk, fsAtk34);
        cout << "     2/1 wither blocker vs 3/4 FIRST STRIKE attacker: \"" << sfs << "\"  (pre-fix falsely appended 'wither shrinks it to 1/2')\n";
        CHECK(sfs == "your blocker dies, attacker lives",
              "A2 wither blocker dead to first strike deals no wither - no shrink claimed");
        // Symmetric: a wither ATTACKER that dies to the blocker's first strike
        // never shrinks the blocker.
        CombatTradeStat fsBlk22     = { 2, 2, false, false, false, true,  false, false, false };
        CombatTradeStat witherAtk21 = { 2, 1, false, true,  false, false, false, false, false };
        string sfa = combatTradePreviewStats(fsBlk22, witherAtk21);
        cout << "     2/2 FIRST STRIKE blocker vs 2/1 wither attacker: \"" << sfa << "\"\n";
        CHECK(sfa == "you kill it, your blocker lives",
              "A2 wither attacker dead to first strike deals no wither - no shrink claimed");
        // Control: a wither blocker that ITSELF has first strike deals in the
        // first-strike step before it dies, so the shrink IS still claimed.
        CombatTradeStat witherFsBlk = { 2, 1, false, true,  false, true,  false, false, false };
        CombatTradeStat plainAtk34  = { 3, 4, false, false, false, false, false, false, false };
        string sfsb = combatTradePreviewStats(witherFsBlk, plainAtk34);
        cout << "     2/1 wither+FIRST STRIKE blocker vs 3/4 attacker: \"" << sfsb << "\"\n";
        CHECK(sfsb == "your blocker dies, attacker lives (wither shrinks it to 1/2)",
              "A2 wither+first-strike blocker still shrinks (dealt before dying)");
    }

    cout << "\n[C-persist] persist return noted on the block-outcome annotation (deck59 wave-24)\n";
    {
        CombatTradeStat persistBlk22 = { 2, 2, false, false, false, false, false, false, true };
        CombatTradeStat plainAtk22   = { 2, 2, false, false, false, false, false, false, false };
        string sboth = combatTradePreviewStats(persistBlk22, plainAtk22);
        cout << "     2/2 persist blocker vs 2/2 (both die): \"" << sboth << "\"\n";
        CHECK(sboth == "both die (yours returns with a -1/-1 counter (persist))",
              "C both die - your persist blocker returns");
        CombatTradeStat persistBlk11 = { 1, 1, false, false, false, false, false, false, true };
        CombatTradeStat plainAtk33   = { 3, 3, false, false, false, false, false, false, false };
        string sdies = combatTradePreviewStats(persistBlk11, plainAtk33);
        cout << "     1/1 persist blocker vs 3/3 (blocker dies): \"" << sdies << "\"\n";
        CHECK(sdies == "your blocker dies, attacker lives (yours returns with a -1/-1 counter (persist))",
              "C your persist blocker dies and returns");
        CombatTradeStat plainBlk33   = { 3, 3, false, false, false, false, false, false, false };
        CombatTradeStat persistAtk22 = { 2, 2, false, false, false, false, false, false, true };
        string skill = combatTradePreviewStats(plainBlk33, persistAtk22);
        cout << "     3/3 blocker vs 2/2 persist attacker (you kill it): \"" << skill << "\"\n";
        CHECK(skill == "you kill it, your blocker lives (theirs returns with a -1/-1 counter (persist))",
              "C the attacker's persist return is noted when you kill it");
        // No persist -> no return clause (guards the gate).
        CombatTradeStat noPersistBlk = { 2, 2, false, false, false, false, false, false, false };
        string snone = combatTradePreviewStats(noPersistBlk, plainAtk22);
        CHECK(snone == "both die", "C non-persist both-die is unchanged");
    }

    cout << "\n[A-shockland] ETB pay-life-or-tapped menu annotation (deck137 wave-24)\n";
    {
        // The shockland menu the engine builds: bare "Pay N life" / "Tap".
        vector<string> shock; shock.push_back("Pay 2 life"); shock.push_back("Tap");
        bool fired = annotateEtbPayOrTapMenu(shock);
        cout << "     opt1: \"" << shock[0] << "\"\n     opt2: \"" << shock[1] << "\"\n";
        CHECK(fired, "A shockland shape (Pay N life + Tap) is recognized");
        CHECK(shock[0].compare(0, 10, "Pay 2 life") == 0 && shock[0].find("UNTAPPED") != string::npos,
              "A the pay-life option keeps its label and gains the UNTAPPED consequence");
        CHECK(shock[1].compare(0, 3, "Tap") == 0 && shock[1].find("TAPPED") != string::npos
              && shock[1].find("untap step") != string::npos,
              "A the Tap option keeps its label and gains the enters-TAPPED consequence");
        // A larger life cost (Pay 3 life) is the same shape.
        vector<string> shock3; shock3.push_back("Pay 3 life"); shock3.push_back("Tap");
        CHECK(annotateEtbPayOrTapMenu(shock3), "A 'Pay 3 life' + Tap also recognized");
        // A "Pay N life" menu WITHOUT a "Tap" sibling (e.g. a damage/drain menu)
        // must NOT be touched - only the pay-or-tapped ETB shape.
        vector<string> other; other.push_back("Pay 2 Life"); other.push_back("Deal 2 damage");
        string before = other[0];
        CHECK(!annotateEtbPayOrTapMenu(other) && other[0] == before,
              "A a Pay-life option with no Tap sibling is left untouched");
        // A lone "Tap" (no pay-life sibling) is untouched.
        vector<string> tapOnly; tapOnly.push_back("Tap"); tapOnly.push_back("Do nothing");
        CHECK(!annotateEtbPayOrTapMenu(tapOnly), "A a Tap option with no pay-life sibling is untouched");
    }

    cout << "\n[A-mayobject] triggered may-ability ask names the object + origin (deck199 wave-26)\n";
    {
        // The Tergrid steal: bare card-script label + trigger-source contextCard.
        vector<string> may; may.push_back("Put onto battlefield under your control");
        may.push_back("Decline - do nothing");
        string header = buildMayObjectAsk("Tergrid, God of Fright", "Bog Wraith",
                                          "opponent's graveyard", may);
        cout << "     header: \"" << header << "\"\n     opt1: \"" << may[0] << "\"\n";
        // Requirements: the object NAME appears, the source is marked AS the
        // trigger source (not the thing being moved), and the origin is stated.
        CHECK(header.find("Bog Wraith") != string::npos, "A-mayobject header names the object");
        CHECK(header.find("Triggered ability of Tergrid, God of Fright") != string::npos,
              "A-mayobject header marks the source as the trigger source");
        CHECK(header.find("opponent's graveyard") != string::npos, "A-mayobject header states the origin");
        CHECK(header.find("not the object being moved") != string::npos,
              "A-mayobject header disambiguates source vs object");
        CHECK(may[0].find("Bog Wraith") != string::npos && may[0].find("NOT Tergrid") != string::npos,
              "A-mayobject real option is annotated with the object, not the source");
        CHECK(may[1] == "Decline - do nothing", "A-mayobject decline option is left untouched");
        // Missing origin: still names the object, no dangling "(from )".
        vector<string> may2; may2.push_back("Put onto battlefield under your control");
        string h2 = buildMayObjectAsk("Tergrid, God of Fright", "Bog Wraith", "", may2);
        CHECK(h2.find("Bog Wraith") != string::npos && h2.find("(from ") == string::npos,
              "A-mayobject unknown origin omits the origin clause cleanly");
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

    // ---- WAVE-29 RENDER PROOF (deterministic): the dungeon room parser +
    // room-branch annotation (N-146b) and the dungeon-selection summary (N-146c)
    // reproduce the exact strings the seams emit, from the real Tomb of
    // Annihilation text= (bin/Res/sets/primitives/borderline.txt).
    cout << "\n[W29-R] dungeon room parse + branch/selection render proof\n";
    {
        string tomb =
            "Trapped Entry - Each player loses 1 life. "
            "-- Veils of Fear - Each player loses 2 life unless they discard a card. "
            "-- Sandfall Cell - Each player loses 2 life unless they sacrifice an artifact, a creature, or a land. "
            "-- Oubliette - Discard a card and sacrifice an artifact, a creature, and a land. "
            "-- Cradle of the Death God - Create The Atropal, a legendary 4/4 black God Horror creature token with deathtouch.";
        std::vector<std::pair<string, string> > rooms;
        parseDungeonRooms(tomb, rooms);
        cout << "     parsed " << rooms.size() << " rooms (must be 5)\n";
        CHECK(rooms.size() == 5, "W29-R parseDungeonRooms splits the 5 Tomb rooms");
        CHECK(rooms.size() > 1 && rooms[1].first == "Veils of Fear"
              && rooms[1].second == "Each player loses 2 life unless they discard a card.",
              "W29-R room name/effect split is clean (Veils of Fear)");
        CHECK(!rooms.empty() && rooms.back().first == "Cradle of the Death God",
              "W29-R the final text= room is the completion reward");

        // N-146b branch-menu render: a bare room option gains its {room effect: ...}.
        // (case-insensitive prefix match tolerates the engine's lowercased name.)
        CHECK(ciStartsWith("veils of fear", "Veils of Fear"),
              "W29-R ciStartsWith matches the engine's lowercased room-option name");
        string opt = "Veils of Fear";
        for (size_t r = 0; r < rooms.size(); r++)
            if (ciStartsWith(opt, rooms[r].first)) { opt += " {room effect: " + rooms[r].second + "}"; break; }
        cout << "     branch option -> " << opt << "\n";
        CHECK(opt == "Veils of Fear {room effect: Each player loses 2 life unless they discard a card.}",
              "W29-R N-146b: branch option carries its room effect");

        // N-146c selection summary: room count + completion reward + full path.
        std::ostringstream sel;
        sel << "[dungeon: " << rooms.size() << " rooms; completion reward (\""
            << rooms.back().first << "\"): " << rooms.back().second << "]";
        cout << "     selection summary -> " << sel.str() << "\n";
        CHECK(sel.str() == "[dungeon: 5 rooms; completion reward (\"Cradle of the Death God\"): "
              "Create The Atropal, a legendary 4/4 black God Horror creature token with deathtouch.]",
              "W29-R N-146c: dungeon selection shows room count + completion payoff");
    }

    // ---- WAVE-30 RENDER PROOFS (deterministic): the wave-30 representation fixes
    // reproduce the EXACT strings the seams emit, via the same pure helpers the
    // render sites call. No engine/observer needed.
    cout << "\n[W30-R] wave-30 representation fixes (mutate + tapped-tag + dungeons-completed)\n";
    {
        // Item 2 / N-93c: tapped tag is restriction-first, no "can attack" nuisance.
        // (Wave-32 N-122c added the combat-participant arms; the NON-combatant
        // arm below is the original wave-30 contract, unchanged.)
        CHECK(tappedCreatureTag(false, false, "") == " [tapped - cannot attack or block this turn]",
              "W30-R N-93c: tapped NON-COMBATANT tag is restriction-first (cannot attack or block)");
        CHECK(tappedCreatureTag(true, false, "") == " [tapped - cannot attack this turn]",
              "W30-R N-93c: CANBLOCKTAPPED creature drops the 'or block' clause (still cannot attack)");
        CHECK(tappedCreatureTag(false, false, "").find("can attack") == string::npos
              && tappedCreatureTag(true, false, "").find("can attack") == string::npos,
              "W30-R N-93c: no affirmative 'can attack' substring remains in the tapped tag");

        // Item 1d / N-139d: mutate alt-cost label is unified to "mutate cost".
        CHECK(mutateAltCostLabel(true, "Mutate") == "mutate cost",
              "W30-R N-139d: a named mutate alternative -> 'mutate cost' (Gemrazer)");
        CHECK(mutateAltCostLabel(true, "") == "mutate cost",
              "W30-R N-139d: an UNNAMED mutate alternative (cost-reduced copy) -> 'mutate cost' (Migratory Greathorn)");
        CHECK(mutateAltCostLabel(false, "flashback") == "flashback cost",
              "W30-R N-139d: a non-mutate NAMED alternative keeps its own name");
        CHECK(mutateAltCostLabel(false, "") == "alternative cost",
              "W30-R N-139d: a non-mutate UNNAMED alternative stays 'alternative cost'");

        // Item 1c / N-139c: the mutate pile renders as ONE line; the under-card names ride the top.
        {
            std::vector<string> under; under.push_back("Dryad of the Ilysian Grove");
            string tag = mutatedPileTag(under);
            CHECK(tag.find("ONE merged creature") != string::npos
                  && tag.find("1 card underneath: Dryad of the Ilysian Grove") != string::npos,
                  "W30-R N-139c: merged pile tag = ONE creature + names the under card");
            CHECK(mutatedPileTag(std::vector<string>()).empty(),
                  "W30-R N-139c: no pile tag for a non-pile creature (empty under list)");
            std::vector<string> two; two.push_back("Pollywog Symbiote"); two.push_back("Arboreal Grazer");
            CHECK(mutatedPileTag(two).find("2 cards underneath: Pollywog Symbiote, Arboreal Grazer") != string::npos,
                  "W30-R N-139c: a deeper pile lists every under card with a plural count");
        }

        // Item 3 / N-146f: the dungeons-completed line only renders when >0.
        CHECK(dungeonsCompletedLine(0, 0).empty(),
              "W30-R N-146f: no dungeons-completed line when neither player has completed one");
        CHECK(dungeonsCompletedLine(2, 0).find("Dungeons completed (you): 2") != string::npos,
              "W30-R N-146f: your completion count renders when >0");
        CHECK(dungeonsCompletedLine(0, 1).find("Dungeons completed (opponent): 1") != string::npos,
              "W30-R N-146f: opponent completions render when >0");

        // Item 1b / N-139b: the remove-from-hand verb is derived from the acting
        // ability, not the source name - a loot's "Pollywog Symbiote Discard" -> discard.
        bool rel = false;
        CHECK(handRemovalVerb("pollywog symbiote discard", rel) == "discard" && !rel,
              "W30-R N-139b: loot detection string ('<card> discard') -> verb 'discard' (not a relocate)");
        CHECK(handRemovalVerb("archon of cruelty discard", rel) == "discard" && !rel,
              "W30-R N-139b: an opponent-forced discard still resolves to 'discard'");
        CHECK(handRemovalVerb("some spell sacrifice", rel) == "sacrifice",
              "W30-R N-139b: 'sacrifice' verb resolves");
        CHECK(handRemovalVerb("bogey exploit", rel) == "sacrifice",
              "W30-R N-139b: 'exploit' maps to sacrifice");
        CHECK(handRemovalVerb("x exile", rel) == "exile",
              "W30-R N-139b: 'exile' verb resolves");
        CHECK(handRemovalVerb("x put in library", rel) == "put back into your library" && rel,
              "W30-R N-139b: 'put in library' is a RELOCATE (keep the card), not a loss");
        CHECK(handRemovalVerb("just a target", rel).empty(),
              "W30-R N-139b: a plain target (no remove verb) is NOT treated as a self-loss");

        // Item 1b framing: self-inflicted vs opponent-forced vs relocate. The
        // opponent branch is kept BYTE-IDENTICAL to the validated deck140 wording.
        string oppAsk = buildHandRemovalAsk("discard", true, false, "Archon of Cruelty",
                                            false, false, false, 1, 0);
        CHECK(oppAsk.find("FORCED discard OF YOUR OWN CARD:") == 0
              && oppAsk.find("the opponent's effect (Archon of Cruelty) forces YOU to discard") != string::npos
              && oppAsk.find("Choose the ONE card to give up") != string::npos,
              "W30-R N-139b: opponent-forced framing (validated deck140 wording) preserved");
        string selfAsk = buildHandRemovalAsk("discard", false, false, "Pollywog Symbiote",
                                             false, false, false, 1, 0);
        CHECK(selfAsk.find("DISCARD ONE OF YOUR OWN CARDS:") == 0
              && selfAsk.find("your own effect (Pollywog Symbiote) makes you discard") != string::npos
              && selfAsk.find("NOT a target you attack or affect") != string::npos,
              "W30-R N-139b: self-inflicted loot framing names the verb + own source, not 'target it will affect'");
        string relAsk = buildHandRemovalAsk("put back into your library", false, true,
                                            "Brainstorm", false, false, false, 1, 0);
        CHECK(relAsk.find("PUT ONE OF YOUR OWN CARDS BACK:") == 0
              && relAsk.find("Choose the ONE card to put back") != string::npos,
              "W30-R N-139b: relocate (put-back) framing keeps the card, no 'afford to lose' language");

        // Item 1a / N-139a: the mutate-host target step and the over/under menu
        // name their role instead of the bare 'target it will affect' / 'Choose an option'.
        string hostOver = mutateHostAsk("Gemrazer", 1);
        CHECK(hostOver.find("MUTATE - CHOOSE THE HOST") == 0
              && hostOver.find("You chose OVER") != string::npos
              && hostOver.find("Gemrazer's card goes ON TOP") != string::npos,
              "W30-R N-139a: mutate-host target step names the role + the chosen OVER placement");
        string hostUnder = mutateHostAsk("Gemrazer", 2);
        CHECK(hostUnder.find("You chose UNDER") != string::npos
              && hostUnder.find("Gemrazer goes BENEATH") != string::npos,
              "W30-R N-139a: mutate-host target step reflects the UNDER placement");
        string ouHeader = mutateOverUnderHeader("Illuna, Apex of Wishes");
        CHECK(ouHeader.find("MUTATE PLACEMENT for Illuna, Apex of Wishes:") == 0
              && ouHeader.find("OVER (on top of) or UNDER the host") != string::npos,
              "W30-R N-139a: over/under menu carries a role header, not the generic 'Choose an option'");
        CHECK(mutateOverUnderHeader("").find("MUTATE PLACEMENT for this creature:") == 0,
              "W30-R N-139a: over/under header degrades cleanly when the source name is unrecoverable");
    }

    // ---- WAVE-30: the new render annotations that ride OPTION lines (mutate
    // over/under [..] tails) must still echo-match; the board tags ([tapped - ...],
    // [mutated: ...]) ride board renders only and are stripped by
    // stripRenderAnnotationsLc like every other bracket, so they never reach the
    // parser as a distinct shape - proven here for the option-borne ones.
    cout << "\n[W30-A] wave-30 mutate over/under option annotations still echo-match\n";
    {
        vector<string> ou;
        ou.push_back("Mutate Over [this card goes ON TOP: the merged creature keeps THIS card's name and P/T, and ALSO gains the host's abilities]");
        ou.push_back("Mutate Under [this card goes UNDERNEATH: the creature on top keeps its name and P/T, and gains THIS card's abilities]");
        bool so = false;
        int co = parseChoice("1 (Mutate Over)", 2, &ou, &so, NULL);
        cout << "     'Mutate Over' echo vs annotated option -> " << co << " (must be 1)\n";
        CHECK(co == 1 && !so, "W30-A mutate 'Mutate Over' echo matches its annotated option");
        bool su = false;
        int cu = parseChoice("2 (Mutate Under)", 2, &ou, &su, NULL);
        cout << "     'Mutate Under' echo vs annotated option -> " << cu << " (must be 2)\n";
        CHECK(cu == 2 && !su, "W30-A mutate 'Mutate Under' echo matches its annotated option");
    }

    // ---- WAVE-29: deciding-fact annotations added to OPTION lines this wave must
    // not break the option-echo match. The model echoes the bare room/action NAME
    // against an option that now carries a trailing brace/bracket annotation - the
    // same shape family as the existing {card text:}/[Transform:] tails, guarded
    // here explicitly. (The N-93a summoning-sick tag rides BOARD renders only -
    // never an option or target line - so it does not enter the choice parser and
    // needs no separate case; it reuses the existing "[tapped - ...]" bracket shape.)
    cout << "\n[W29-A] option-line deciding-fact annotations ({room effect}/[display toggle]) still echo-match\n";
    {
        // Dungeon room BRANCH (deck146 N-146b): each option is a room name carrying
        // its effect text; the reply echoes the bare room name.
        vector<string> rooms;
        rooms.push_back("Veils of Fear {room effect: Each player loses 2 life unless they discard a card.}");
        rooms.push_back("Oubliette {room effect: Discard a card and sacrifice an artifact, a creature, and a land.}");
        bool sr = false;
        int cr = parseChoice("2 (Oubliette)", 2, &rooms, &sr, NULL);
        cout << "     room-branch 'Oubliette' vs annotated option -> " << cr << " (must be 2)\n";
        CHECK(cr == 2 && !sr, "W29-A dungeon room-branch: a bare room-name echo matches its {room effect: ...} option");

        // Modal-DFC Flip Side no-op annotation (Item 3): the option gains a trailing
        // "[display toggle only - no game effect: ...]" tail; a "Flip Side" echo must
        // still land on it, and "Cast Card Normally" still lands on option 1.
        vector<string> dfcMenu;
        dfcMenu.push_back("Cast Card Normally");
        dfcMenu.push_back("Flip Side [display toggle only - no game effect: switches which face this hand card shows; it casts nothing and uses no stack. The Cast menu is where you cast, and it offers the other face as an alternative-cost cast - you do not need this.]");
        bool sf = false;
        int cf = parseChoice("2 (Flip Side)", 2, &dfcMenu, &sf, NULL);
        cout << "     annotated 'Flip Side' echo -> " << cf << " (must be 2)\n";
        CHECK(cf == 2 && !sf, "W29-A modal-DFC: a 'Flip Side' echo still matches its annotated no-op option");
        bool sf1 = false;
        int cf1 = parseChoice("1 (Cast Card Normally)", 2, &dfcMenu, &sf1, NULL);
        CHECK(cf1 == 1 && !sf1, "W29-A modal-DFC: 'Cast Card Normally' still matches option 1 alongside the annotated Flip Side");
    }

    // ---- WAVE-31 (N-152a): the MDFC LAND flip annotations are ANSWER TARGETS.
    // The play-land face menu ("Play Land / Flip Side / Decline") and the
    // in-hand priority Flip-Side option now carry a land-aware tail naming both
    // faces + colors. A bare "Play Land" / "Flip Side" echo must still bind, and
    // the landTapMana / dfcOtherFaceName helpers that build the tail must read a
    // Pathway's text correctly.
    cout << "\n[W31-A] MDFC land flip annotations echo-match + helper extraction\n";
    {
        // landTapMana: pull the produced color(s) off a land face's rules text.
        CHECK(landTapMana("{T}: Add {G}. // Boulderloft Pathway") == "{G}",
              "W31-A landTapMana reads the front (G) face's tapped color");
        CHECK(landTapMana("{T}: Add {W}. // Branchloft Pathway") == "{W}",
              "W31-A landTapMana reads the back (W) face's tapped color");
        CHECK(landTapMana("{T}: Add {W} or {U}.") == "{W} or {U}",
              "W31-A landTapMana reads a two-color 'or' tap line");
        CHECK(landTapMana("2/2 Wolf, no mana ability").empty(),
              "W31-A landTapMana is empty for a face that produces no mana");
        // dfcOtherFaceName: the other face's name from the "// X" tail.
        CHECK(dfcOtherFaceName("{T}: Add {G}. // Boulderloft Pathway") == "Boulderloft Pathway",
              "W31-A dfcOtherFaceName extracts the other face's printed name");
        CHECK(dfcOtherFaceName("Whenever this enters, draw a card.").empty(),
              "W31-A dfcOtherFaceName is empty for a non-DFC card (no '//')");

        // The land-face play menu: the annotated Flip Side + Play Land still bind
        // to their indices under the exact tail shape the code emits.
        vector<string> landMenu;
        landMenu.push_back("Play Land");
        landMenu.push_back("Flip Side [DISPLAY TOGGLE only - this is a modal double-faced land."
                           " It currently shows \"Branchloft Pathway\" (taps for {G}); its other"
                           " face is \"Boulderloft Pathway\" (taps for {W}). Flipping only changes"
                           " which face is DISPLAYED; it casts nothing and gains nothing playable -"
                           " in this engine only the currently-shown face can actually be played as"
                           " a land. Choose Play Land to play the current face.]");
        landMenu.push_back("Decline - do nothing");
        bool sfl = false;
        int cfl = parseChoice("2 (Flip Side)", 3, &landMenu, &sfl, NULL);
        cout << "     land-menu 'Flip Side' echo -> " << cfl << " (must be 2)\n";
        CHECK(cfl == 2 && !sfl, "W31-A land-menu: a 'Flip Side' echo binds to its land-aware annotated option");
        bool spl = false;
        int cpl = parseChoice("1 (Play Land)", 3, &landMenu, &spl, NULL);
        CHECK(cpl == 1 && !spl, "W31-A land-menu: 'Play Land' still binds to option 1 alongside the annotated Flip Side");
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

    // ================= WAVE-25 items =========================================
    // ITEM 1: protocol-example leak (retraction FP + index seed). ITEM 2:
    // natural-stop prefer-last / prose reconcile. ITEM 3: R-STALE-ECHO-QUALIFIER
    // residual. Cases built from the real matchups-20260724-125739 corpus.

    // ---- ITEM 1: the quoted-`e.g.`-example must not fire retracted_choice ----
    cout << "\n[W25-1] protocol-example leak: quoted example CHOICE tokens do NOT retract; a genuine mid-line recode still does\n";
    {
        vector<string> fp;
        fp.push_back("Cast Fatal Push {b} - legal targets right now: Bloodghast, Geralf's Messenger, Guttersnipe");
        fp.push_back("Cast nothing right now");
        // deck133 vs131 s21 (real shape, truncated): line-1 clean CHOICE 1,
        // re-affirmed; the ONLY contradicting numeric coded index is the prompt's
        // OWN worked example quoted verbatim x4 - `"CHOICE: 2 (Cast Fatal Push)"`
        // (the old example reused option 1's card name). No PLAN (token-cap cut).
        string s21 =
            "CHOICE: 1 (Cast Fatal Push)\n"
            "The opponent cast Downsize at Geralf's Messenger. I could Push Guttersnipe instead.\n"
            "Let's look at the example: \"CHOICE: 2 (Cast Fatal Push)\". The example shows the format.\n"
            "So I must say CHOICE: 1. I will stick to the format \"CHOICE: 1 (Cast Fatal Push)\".\n"
            "But the example \"CHOICE: 2 (Cast Fatal Push)\" uses the same card name, confusing me.\n"
            "Is the answer the number or the target name? The example \"CHOICE: 2 (Cast Fatal Push)\"\n"
            "and \"CHOICE: Guttersnipe\" - which contract applies here? Let me re-read once more,\n";
        int r = RESOLVE(s21, 2, fp);
        cout << "     s21 (line-1 CHOICE 1, quoted example CHOICE 2 x4) -> " << r << " (must be 1, no retraction)\n";
        CHECK(r == 1, "W25-1 s21: quoted protocol-example CHOICE tokens do not retract the sustained CHOICE 1");
        // A GENUINE mid-line recode (unquoted, not the example) still retracts.
        string genuine =
            "CHOICE: 1 (Cast Fatal Push)\n"
            "On reflection, holding removal for their bomb is the better line. So CHOICE: 2.\n"
            "PLAN: hold up Fatal Push.\n";
        int g = RESOLVE(genuine, 2, fp);
        cout << "     genuine unquoted 'So CHOICE: 2' -> " << g << " (must be -1: genuine contradiction still fires)\n";
        CHECK(g == -1, "W25-1 genuine: an unquoted mid-line recode contradicting line 1 still retracts");
        // ITEM 1b (deck102 vs133 s5): the example's index seeded into an ask whose
        // named card is NOT an option -> must NOT take that index (heuristic).
        vector<string> flip;
        flip.push_back("Flip Side with Tergrid's Lantern {card text: \"{T}: Target player loses 3 life...\"}");
        bool sst = false;
        int s5 = parseChoice("2 (Cast Commander's Sphere)", 1, &flip, &sst);
        cout << "     s5 (index 2, card absent from a 1-option ask) -> " << s5 << " (must be < 0)\n";
        CHECK(s5 < 0, "W25-1b: an example/stale-seeded out-of-range coded index is not taken");
    }

    // ---- WAVE-26 ITEM 2: line-anchored coded-index scan + example-echo exclusion
    // (belt-and-suspenders for the protocol-example leak, deck133 reviewer). The
    // de-fang removed the LIVE-collision trigger; these guard the PARSER itself so
    // it holds even if a FUTURE example text again collides with a real option.
    cout << "\n[W26] line-anchored coded-index scan; a verbatim example-echo line is never taken as the answer\n";
    {
        // A future example whose card name EQUALS a live option (the exact wave-24
        // shape). Under the old resolver a line-leading parrot of it would be taken.
        vector<string> opts;
        opts.push_back("Cast Doom Blade");
        opts.push_back("Cast Example Card");

        // (1) LINE-ANCHORED POSITIVE: a normal coded answer at line start resolves.
        string ok = "CHOICE: 1 (Cast Doom Blade)\nPLAN: kill the threat.\n";
        int rp = RESOLVE(ok, 2, opts);
        cout << "     line-anchored 'CHOICE: 1 (Cast Doom Blade)' -> " << rp << " (must be 1)\n";
        CHECK(rp == 1, "W26 line-anchored positive: a real coded answer at line start still resolves");

        // (2) EXAMPLE-ECHO NEGATIVE: a line-leading verbatim echo of the protocol
        // example (its card collides with option 2) is a parrot, not a decision ->
        // heuristic (-1). Pre-change this resolved to 2 (the leak).
        string echoLine = "CHOICE: 2 (Cast Example Card)\nPLAN: follow the format.\n";
        int re = RESOLVE(echoLine, 2, opts);
        cout << "     line-leading example echo 'CHOICE: 2 (Cast Example Card)' -> " << re << " (must be < 0)\n";
        CHECK(re < 0, "W26 example-echo negative: a verbatim protocol-example line is not taken as the answer");

        // (3) MID-LINE QUOTED NEGATIVE: a quoted example token that does NOT begin
        // its line is not a coded answer (line-anchoring) -> nothing salvageable.
        string quoted = "The example \"CHOICE: 2 (Cast Example Card)\" shows the format.\nPLAN: decide next.\n";
        int rq = RESOLVE(quoted, 2, opts);
        cout << "     mid-line quoted example only -> " << rq << " (must be < 0: not line-anchored)\n";
        CHECK(rq < 0, "W26 mid-line quoted negative: a non-line-start quoted CHOICE token is never the answer");

        // (4) The example-echo exclusion must not swallow a REAL answer that merely
        // mentions the phrase in prose (not inside the coded parenthetical).
        string prose = "CHOICE: 1 (Cast Doom Blade) - good for example card advantage later.\nPLAN: go.\n";
        int rr = RESOLVE(prose, 2, opts);
        cout << "     real answer with 'example card' in trailing prose -> " << rr << " (must be 1, not dropped)\n";
        CHECK(rr == 1, "W26 precision: 'example card' outside the coded parenthetical does NOT drop a real answer");
    }

    // ---- ITEM 2: natural-stop prefer-last / prose reconcile ----
    cout << "\n[W25-2] natural-stop: the deck27 bottom reply's PROSE conclusion wins over its answer-first coded line; truncated keeps line 1\n";
    {
        // deck27 vs137 bottom (real shape): line 1 "PUT: 3, 5, 6" bottomed its ONLY
        // blue source (Underground Sea = card 3); the reply reasoned to the correct
        // set and ended NATURALLY with "So I bottom 5, 6, and 7." + a matching PLAN.
        string deck27 =
            "PUT: 3, 5, 6\n"
            "Reasoning: keep mana and the cheap creature. Wait - Metathran Zombie needs {1}{U},\n"
            "and Underground Sea (card 3) is my ONLY blue source, so I MUST keep it.\n"
            "Re-evaluating: keep Metathran Zombie (2) and Underground Sea (3); bottom the two\n"
            "expensive creatures and one redundant Swamp.\n"
            "So I bottom 5, 6, and 7.\n"
            "PLAN: Put cards 5, 6, and 7 on the bottom. Play Underground Sea, cast Metathran Zombie.\n";
        vector<string> hand;
        hand.push_back("Swamp"); hand.push_back("Metathran Zombie"); hand.push_back("Underground Sea");
        hand.push_back("Swamp"); hand.push_back("Sanguine Guard"); hand.push_back("Wasp Lancer");
        hand.push_back("Swamp");
        // The answer-first coded line alone yields the WRONG 3,5,6 (the bug).
        vector<bool> coded;
        int cr = parseAttackerSet("3, 5, 6", hand.size(), coded, &hand);
        cout << "     answer-first coded '3, 5, 6' -> count=" << cr
             << " picks=[" << (int)coded[2] << (int)coded[4] << (int)coded[5] << "] (the bug: bottoms Underground Sea)\n";
        CHECK(cr == 3 && coded[2] && coded[4] && coded[5], "W25-2 bug repro: answer-first takes 3,5,6");
        // The reply terminated naturally (has a PLAN: line).
        CHECK(replyTerminatedNaturally(deck27), "W25-2 deck27 reply is classified as NATURALLY terminated (has PLAN:)");
        // Natural-stop prose reconcile recovers the model's final 5,6,7.
        vector<bool> prose;
        int pr = salvageProsePutList(deck27, hand.size(), 3, prose);
        cout << "     prose reconcile -> count=" << pr
             << " picks=[" << (prose.size()>4?(int)prose[4]:-1) << (prose.size()>5?(int)prose[5]:-1)
             << (prose.size()>6?(int)prose[6]:-1) << "] keep-UndergroundSea=" << (prose.size()>2?(int)prose[2]:-1) << "\n";
        CHECK(pr == 3 && prose[4] && prose[5] && prose[6] && !prose[2] && !prose[0] && !prose[1] && !prose[3],
              "W25-2 deck27: prose reconcile yields 5,6,7 (keeps the blue source)");
        // A TRUNCATED reply (no PLAN:, long, mid-word cut) is NOT natural -> the
        // caller keeps answer-first (deck109-style tail is unreliable).
        string trunc = "PUT: 3, 5, 6\n";
        for (int i = 0; i < 40; i++)
            trunc += "I keep re-deriving my mana base and which lands make blue versus black here,\n";
        cout << "     truncated (no PLAN, len " << trunc.size() << ") natural? " << replyTerminatedNaturally(trunc) << " (must be 0)\n";
        CHECK(!replyTerminatedNaturally(trunc), "W25-2 a long no-PLAN reply is classified TRUNCATED (answer-first kept)");
        // Omit-when-unchanged (2026-08-17): the protocol now tells the model to
        // DROP the PLAN line when its plan is unchanged, so a short compliant
        // reply with no PLAN: is the COMMON shape, not an edge case. It must
        // classify NATURAL (last-answer precedence, prior plan carried), and a
        // short mid-word cut must still classify truncated.
        string noplan = "CHOICE: 2 (Cast Example Card)\nCurve play, nothing changed.";
        CHECK(replyTerminatedNaturally(noplan), "OMIT short no-PLAN reply ending on punctuation is NATURALLY terminated");
        string shortcut = "CHOICE: 2 (Cast Example Card)\nBecause the opp";
        CHECK(!replyTerminatedNaturally(shortcut), "OMIT short no-PLAN mid-word cut is still TRUNCATED");
        // The deck109 CoT-hijack ATTACK reply still yields the FIRST declaration
        // (answer-first), NOT the trailing bare-number CoT line - unchanged.
        vector<string> anames;
        anames.push_back("Goblin"); anames.push_back("Goblin");
        anames.push_back("Ash Zealot"); anames.push_back("Legion Loyalist");
        string atkReply =
            "ATTACK: A1, A2, A3, A4\n"
            "Reasoning: with Legion Loyalist my team gets first strike and trample...\n"
            "Attack: Deal 1, Take 5. Net -4 life. Opponent -1 life.\n"
            "I will declare no attackers. Wait, the guide says attack every turn...\n";
        vector<string> alines;
        collectLabeledLines(atkReply, "ATTACK:", alines);
        int af = -1; vector<bool> as;
        for (size_t li = 0; li < alines.size(); li++)
        {
            vector<bool> s;
            int rr = parseAttackerSet(alines[li], 4, s, &anames);
            if (rr >= 0) { as = s; af = rr; break; }
        }
        cout << "     deck109 answer-first ATTACK -> count=" << af << " (must be 4, all)\n";
        CHECK(af == 4 && as[0] && as[1] && as[2] && as[3],
              "W25-2 ATTACK stays answer-first (not the CoT bare-number line): no wave-23 regression");
    }

    // ---- ITEM 3: R-STALE-ECHO-QUALIFIER residual (shockland ETB menu) ----
    cout << "\n[W25-3] shockland ETB menu: 'Tap Temple Garden' echo trusts option 'tap' under INDEX-WINS\n";
    {
        // deck137 vs102 s4 (real): options "pay 2 life" / "tap"; source name did
        // NOT render ("Choose an option for :"). The echo "Tap Temple Garden" names
        // the SOURCE card (absent from the bare option "tap"), so it was wrongly
        // dropped stale. The option label "tap" IS inside the echo -> INDEX-WINS.
        vector<string> shock;
        shock.push_back("pay 2 life");
        shock.push_back("tap");
        bool st = false;
        int c = parseChoice("2 (Tap Temple Garden)", 2, &shock, &st, NULL);
        cout << "     'CHOICE: 2 (Tap Temple Garden)' vs ['pay 2 life','tap'] -> " << c << " (must be 2, not stale)\n";
        CHECK(c == 2 && !st, "W25-3 deck137 s4: option label 'tap' echoed inside 'Tap Temple Garden' trusts index 2");
        // Guard: an anchorless bare-X option with a source-naming echo and NO source
        // passed still stays stale (the (a2) option-in-echo test needs a real option
        // word; "X = 1" has none) - W23-A shape1 invariant preserved.
        vector<string> xopts; xopts.push_back("X = 1"); xopts.push_back("X = 0");
        bool sx = false;
        int cx = parseChoice("1 (Cast Black Sun's Zenith with X=1)", 2, &xopts, &sx, NULL);
        cout << "     anchorless 'X = 1' echo w/o source -> " << cx << " (must be < 0: still stale)\n";
        CHECK(cx < 0 && sx, "W25-3 guard: an anchorless bare-X option without a source anchor stays stale");
    }

    // ---- WAVE-28: R-ANNOTATION-ECHO-PARSE (the model echoes a render annotation
    // onto the option name) + the Item-1 land-tag echo shape ----
    cout << "\n[W28-ECHO] annotation-tolerant name match: an echoed [changeling]/(land) tail resolves; a different card still stays stale\n";
    {
        // deck22 vs131 s3 (real): the model appended the hand-render annotation
        // "[changeling: counts as Giant]" to its echoed option name; the canonical
        // cast option has no annotation, the number+name reconcile failed -> stale_echo
        // -> Baka. The chosen option's core ("Cast Universal Automaton {1} (1/1)")
        // still appears verbatim in the reply, so the in-range index is trusted.
        vector<string> autom;
        autom.push_back("Cast Universal Automaton {1} (1/1)");
        autom.push_back("Cast Mogg Sentry {r} (1/1)");
        autom.push_back("Cast nothing right now");
        bool sa = false;
        int ca = parseChoice("CHOICE: 1 (Cast Universal Automaton {1} (1/1) [changeling: counts as Giant])",
                             3, &autom, &sa, NULL);
        cout << "     changeling-annotated echo -> " << ca << " (must be 1, was stale_echo)\n";
        CHECK(ca == 1 && !sa, "W28-ECHO deck22 s3: a '[changeling: ...]' tail on the echoed name still trusts the in-range index");

        // NEGATIVE: a DIFFERENT card echoed (with the same annotation) must NOT
        // match option 1 - its core name is absent from the reply, so it stays stale.
        bool sn = false;
        int cn = parseChoice("CHOICE: 1 (Cast Grizzly Bears {1}{g} [changeling: counts as Giant])",
                             3, &autom, &sn, NULL);
        cout << "     different card + annotation -> " << cn << " (must be -1: stale, no cross-match)\n";
        CHECK(cn < 0 && sn, "W28-ECHO negative: a different card name with an annotation does NOT cross-match option 1");

        // Item-1 land-tag echo shape: the model copied "(land: taps for {B})" out of
        // the hand render onto a bare "Play Swamp" option. The '(land...)' tail is
        // stripped, so the option core "Play Swamp" reconciles -> trust the index.
        vector<string> lands;
        lands.push_back("Cast Dark Ritual {b}");
        lands.push_back("Play Swamp");
        lands.push_back("Cast nothing right now");
        bool sl = false;
        int cl = parseChoice("CHOICE: 2 (Play Swamp (land: taps for {B}))", 3, &lands, &sl, NULL);
        cout << "     land-tag-annotated echo -> " << cl << " (must be 2, tolerant of the new land tag)\n";
        CHECK(cl == 2 && !sl, "W28-ECHO Item-1 land tag: a '(land: taps for {B})' tail on the echoed name still trusts the in-range index");

        // NEGATIVE land: a different land echoed must not rescue option 2 (Swamp).
        bool sl2 = false;
        int cl2 = parseChoice("CHOICE: 2 (Play Mountain (land: taps for {R}))", 3, &lands, &sl2, NULL);
        cout << "     different land + land tag -> " << cl2 << " (must be -1: stale)\n";
        CHECK(cl2 < 0 && sl2, "W28-ECHO negative land: a different land name does NOT cross-match the Swamp option");
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

    // ---- WAVE-29 N-18e: truncated block commit abandoned by the reasoning ----
    cout << "\n[W29-N18e] truncated blockers reply: early BLOCKS commit reversed in prose -> safe no-blocks\n";
    {
        // deck18 vs93 s20 shape: an early coded block commit, then a repeated
        // no-block conclusion, cut off mid-sentence with NO PLAN: terminator.
        string reversal;
        for (int i = 0; i < 40; i++)
            reversal += "So I should NOT block: blocking is strictly worse, I lose a card for no benefit. ";
        string vs93 = "BLOCKS: B1:A1\n" + reversal + "If I don't block, I have 13 life.";
        cout << "     len=" << vs93.size() << " terminatedNaturally=" << replyTerminatedNaturally(vs93)
             << " abandoned=" << truncatedBlockCommitmentAbandoned(vs93) << "\n";
        CHECK(!replyTerminatedNaturally(vs93), "W29-N18e vs93 shape classified TRUNCATED (long, no PLAN:)");
        CHECK(truncatedBlockCommitmentAbandoned(vs93),
              "W29-N18e truncated+contradicted commit is abandoned (-> safe no-blocks, not the stale B1:A1)");

        // Non-contradicted truncation: same early commit, long prose that never
        // reverses the block. Must KEEP the first line (abandoned == false).
        string affirm;
        for (int i = 0; i < 40; i++)
            affirm += "Trading my Soldier for the 4/4 is correct here, the block holds the line and buys tempo. ";
        string keep = "BLOCKS: B1:A1\n" + affirm + "That is the plan I commit to and";
        cout << "     non-contradicted len=" << keep.size() << " terminatedNaturally=" << replyTerminatedNaturally(keep)
             << " abandoned=" << truncatedBlockCommitmentAbandoned(keep) << "\n";
        CHECK(!replyTerminatedNaturally(keep), "W29-N18e non-contradicted reply is also TRUNCATED (control)");
        CHECK(!truncatedBlockCommitmentAbandoned(keep),
              "W29-N18e truncated but NOT contradicted -> keeps the early commit (no masking)");

        // Naturally terminated reply (reached PLAN:), even with a reversal in
        // prose: NOT this path's business - the normal last-line precedence
        // owns self-correction on a complete reply.
        string terminated = "BLOCKS: B1:A1\nOn reflection I should NOT block this one.\nPLAN: hold up blockers next turn.";
        cout << "     terminated terminatedNaturally=" << replyTerminatedNaturally(terminated)
             << " abandoned=" << truncatedBlockCommitmentAbandoned(terminated) << "\n";
        CHECK(replyTerminatedNaturally(terminated), "W29-N18e a reply with PLAN: is NATURALLY terminated");
        CHECK(!truncatedBlockCommitmentAbandoned(terminated),
              "W29-N18e a terminated reply is untouched (unchanged precedence)");

        // "cannot block" legality note must NOT read as a decline of intent.
        string legality = "BLOCKS: B1:A1\n";
        for (int i = 0; i < 40; i++)
            legality += "Note A2 cannot block because it is tapped, so my assignment stands and is fine here. ";
        cout << "     cannot-guard len=" << legality.size() << " abandoned=" << truncatedBlockCommitmentAbandoned(legality) << "\n";
        CHECK(!truncatedBlockCommitmentAbandoned(legality),
              "W29-N18e 'cannot block' (legality) is not a decline -> commit kept");
    }

    // ==== WAVE-32 step-1 (decision-flow) cases ====

    // ---- N-122d: a compliant head BLOCKS: line survives truncation ----
    cout << "\n[W32-N122d] truncated blockers reply whose reasoning AGREES with the head line -> head kept\n";
    {
        // deck122 vs137 s22 / vs158 s26 shape: an answer-first assignment, then a
        // long per-creature deliberation that re-derives the SAME pick while
        // saying "will not block" ABOUT THE CREATURES IT LEAVES HOME, cut off
        // with no PLAN:. The old predicate read those as a global reversal and
        // declared no blocks - the worst answer at this seam.
        string perCreature;
        for (int i = 0; i < 40; i++)
            perCreature += "B3 will not block A1 here, and B4 blocking A2 has no benefit either. ";
        string agree = "BLOCKS: B1:A3, B2:A4\n" + perCreature
                     + "So B1 still takes A3 and B2 still takes A4, which is the line I want and";
        cout << "     len=" << agree.size() << " terminatedNaturally=" << replyTerminatedNaturally(agree)
             << " abandoned=" << truncatedBlockCommitmentAbandoned(agree) << "\n";
        CHECK(!replyTerminatedNaturally(agree), "W32-N122d witness shape is TRUNCATED (control)");
        CHECK(!truncatedBlockCommitmentAbandoned(agree),
              "W32-N122d per-creature 'B3 will not block' is scoped, not a global decline -> head kept");

        // A LATER complete coded assignment is a self-correction, owned by the
        // shipped line-anchored precedence - this predicate must stand down.
        string later;
        for (int i = 0; i < 30; i++)
            later += "Reconsidering the trade math for this attacker set once more here. ";
        string corrected = "BLOCKS: B1:A1\n" + later + "\nBLOCKS: B2:A1\nI should NOT block with B1 after all and";
        cout << "     later-line abandoned=" << truncatedBlockCommitmentAbandoned(corrected) << "\n";
        CHECK(!truncatedBlockCommitmentAbandoned(corrected),
              "W32-N122d a LATER coded BLOCKS: assignment defers to line precedence (not abandoned)");

        // NEGATIVE control: the wave-29 global reversal must still fire. Same
        // text as the W29-N18e case, re-asserted here against the new guards.
        string reversal;
        for (int i = 0; i < 40; i++)
            reversal += "So I should NOT block: blocking is strictly worse, I lose a card for no benefit. ";
        string vs93b = "BLOCKS: B1:A1\n" + reversal + "If I don't block, I have 13 life.";
        CHECK(truncatedBlockCommitmentAbandoned(vs93b),
              "W32-N122d a GLOBAL prose reversal with no label still abandons (N-18e preserved)");

        // A "BLOCKS: none" tail is not a competing assignment, so a global
        // reversal that also codes the decline still abandons.
        string declineLine = "BLOCKS: B1:A1\n" + reversal + "\nBLOCKS: none\nand therefore";
        CHECK(truncatedBlockCommitmentAbandoned(declineLine),
              "W32-N122d a 'BLOCKS: none' tail line is not a competing assignment (still abandoned)");
    }

    // ---- N-139i: London bottoming holds the loop to the full N ----
    cout << "\n[W32-N139i] London bottoming target: the clamp is against the START hand, not the shrinking one\n";
    {
        // deck139 vs122: kept after 7 mulligans on a 7-card hand. Drive the real
        // PG_BOTTOM arithmetic one card at a time; the old live clamp produced
        // 7,6,5,4 -> stop at 4 bottomed with a 3-card hand (CR 103.5 violation).
        int mulls = 7, hand = 7, bottomed = 0, guard = 0;
        int firstTarget = PreGamePhase::bottomTarget(mulls, bottomed, hand);
        while (guard++ < 20)
        {
            int need = PreGamePhase::bottomTarget(mulls, bottomed, hand);
            if (bottomed >= need || hand <= 0)
                break;
            bottomed++;
            hand--;
        }
        cout << "     first target=" << firstTarget << " bottomed=" << bottomed
             << " hand left=" << hand << "\n";
        CHECK(firstTarget == 7, "W32-N139i a 7-mulligan keep owes 7 bottoms up front");
        CHECK(bottomed == 7 && hand == 0,
              "W32-N139i the loop bottoms all 7 and ends on an empty hand (CR 103.5)");

        // Ordinary keep: 2 mulligans on a 7-card hand -> exactly 2, hand of 5,
        // and the target never moves while the loop runs (the AI seam asks once).
        int m2 = 2, h2 = 7, b2 = 0, targets = 0, distinct = -1;
        bool stable = true;
        while (targets++ < 20)
        {
            int need = PreGamePhase::bottomTarget(m2, b2, h2);
            if (distinct < 0) distinct = need;
            if (need != distinct) stable = false;
            if (b2 >= need) break;
            b2++; h2--;
        }
        CHECK(b2 == 2 && h2 == 5, "W32-N139i a 2-mulligan keep bottoms exactly 2, keeping 5");
        CHECK(stable, "W32-N139i the bottom target is CONSTANT across the loop (one honest ask)");

        // Degenerate guard: more mulligans than cards in hand clamps to the hand
        // and still terminates.
        CHECK(PreGamePhase::bottomTarget(9, 0, 4) == 4,
              "W32-N139i target clamps to the cards actually available");
        CHECK(PreGamePhase::bottomTarget(9, 4, 0) == 4,
              "W32-N139i already-bottomed cards count toward what was available");
    }

    // ---- WAVE-32 RENDER PROOFS (deterministic): the wave-32 step-1 render/
    // annotation fixes reproduce the EXACT strings the seams emit, through the
    // same pure helpers the render sites call, plus the ECHO shape of every new
    // bracketed annotation (the model echoes annotations back into its answers,
    // and answer matching strips "[...]" tails only on already-anchored
    // candidates - so a new tail that breaks binding is a live regression).
    cout << "\n[W32-R] wave-32 representation fixes (combat tags, mana line, DFC faces, magnitudes)\n";
    {
        // ---- N-122c: the declared-ATTACKER path. The wave-30 reword landed only
        // on the non-combatant arm, so every attacker printed
        // "[tapped - cannot attack or block this turn] [attacking]" - 331
        // occurrences, verbalized as a paradox at four seats, one lost game.
        CHECK(tappedCreatureTag(false, true, "") == " [tapped - attacking]",
              "W32-R N-122c: a tapped ATTACKER names the cause, not a false restriction");
        CHECK(tappedCreatureTag(false, true, "").find("cannot attack") == string::npos,
              "W32-R N-122c NEGATIVE: the attacker tag carries NO 'cannot attack' clause (the paradox substring)");
        CHECK(tappedCreatureTag(false, true, "").find("cannot block") == string::npos,
              "W32-R N-122c NEGATIVE: the attacker tag carries no 'cannot block' clause either");
        CHECK(tappedCreatureTag(false, false, "Grizzly Bears #2")
              == " [tapped - blocking Grizzly Bears #2]",
              "W32-R N-122c: a tapped BLOCKER names the attacker it blocks (handle included)");
        CHECK(tappedCreatureTag(true, true, "Grizzly Bears #2") == " [tapped - attacking]",
              "W32-R N-122c: attacking wins over every other arm (a creature cannot do both)");
        CHECK(tappedCreatureTag(false, true, "").find("can attack next") == string::npos
              && tappedCreatureTag(false, false, "Bear").find("can attack") == string::npos,
              "W32-R N-122c: no affirmative future-permission substring in any combat arm");
        {
            // Echo shape: a board/answer echo carrying the new tail still binds.
            vector<string> opts;
            opts.push_back("Attack with Orc army");
            opts.push_back("Attack with nobody");
            bool stale = false;
            int c = parseChoice("1 (Attack with Orc army [tapped - attacking])", 2, &opts, &stale, NULL);
            cout << "     N-122c echo with [tapped - attacking] tail -> " << c << " (must be 1)\n";
            CHECK(c == 1 && !stale, "W32-R N-122c: an echo carrying the [tapped - attacking] tail still binds");
        }

        // ---- N-139k: summoning sickness restricts attacking only.
        CHECK(summoningSickTag() == " [summoning sick - cannot attack this turn, but CAN block]",
              "W32-R N-139k: the summoning-sick tag states the BLOCK permission explicitly");
        CHECK(summoningSickTag().find("but CAN block") != string::npos,
              "W32-R N-139k: the permission clause is present (the omission is what was mis-completed)");
        CHECK(summoningSickTag().find("cannot attack this turn") != string::npos,
              "W32-R N-139k NEGATIVE: the attack restriction is NOT weakened by adding the permission");
        CHECK(summoningSickTag().find("can attack") == string::npos,
              "W32-R N-139k NEGATIVE: still no affirmative 'can attack' substring");

        // ---- N-139j: the attackers menu gains the blockers menu's 0-power tag.
        CHECK(zeroPowerAttackerTag(0) == " [deals 0 - this attack deals no damage to the opponent]",
              "W32-R N-139j: a 0-power attacker option names the consequence");
        CHECK(zeroPowerAttackerTag(-1) == zeroPowerAttackerTag(0),
              "W32-R N-139j: a negative-power attacker gets the same tag");
        CHECK(zeroPowerAttackerTag(1).empty() && zeroPowerAttackerTag(6).empty(),
              "W32-R N-139j NEGATIVE: an attacker with power does NOT get the deals-0 tag");
        {
            // The attackers seam answers through parseAttackerSet (bundled
            // "ATTACK: A1, A3"), NOT parseChoice - so THAT is the parser the new
            // tail must survive. The live hazard is concrete: the tag contains a
            // bare digit ("deals 0"), and this parser accepts bare numbers as
            // attacker indices. A model that echoes the whole annotated option
            // line back must still declare exactly the creature it named.
            vector<string> names;
            names.push_back("Arboreal Grazer");
            names.push_back("Llanowar Elves");
            names.push_back("Sauron");
            vector<bool> send;
            int r = parseAttackerSet("ATTACK: A2", 3, send, &names);
            cout << "     N-139j plain 'ATTACK: A2' -> " << r << " (must be 1), A2=" << (int) send[1] << "\n";
            CHECK(r == 1 && send[1] && !send[0] && !send[2],
                  "W32-R N-139j: a plain labelled declaration is unaffected by the new tag");
            string echo = "ATTACK: A2. Llanowar Elves (1/1)\n"
                          "I am NOT sending A1. Arboreal Grazer (0/3)"
                          " [deals 0 - this attack deals no damage to the opponent].";
            vector<bool> send2;
            int r2 = parseAttackerSet(echo, 3, send2, &names);
            cout << "     N-139j echo carrying the [deals 0 ...] tail -> " << r2
                 << " (A1=" << (int) send2[0] << " A2=" << (int) send2[1] << ")\n";
            CHECK(r2 >= 1 && send2[1],
                  "W32-R N-139j: an echo carrying the [deals 0 - this attack...] tail still declares its attacker");
            CHECK(send2.size() == 3 && !send2[2],
                  "W32-R N-139j NEGATIVE: the bare '0' inside the new tag is NOT read as an extra attacker index");
        }

        // ---- N-152d: (printed X/Y) must read the DISPLAYED face.
        // The ledger's own requested case: transformed DFC, no counters, no
        // anthem -> NO "(printed ...)" tail at all. Moonrage Brute is a printed
        // 3/3 whose FRONT face (Brutal Cathar) is a 2/2; pre-fix this rendered
        // "(printed 2/2)" and presented an unbuffed creature as pumped.
        CHECK(printedPTTag(3, 3, /*base*/2, 2, /*orig*/3, 3, /*isFlipped*/1).empty(),
              "W32-R N-152d: a TRANSFORMED DFC with no counters renders NO (printed ...) tail");
        CHECK(printedPTTag(7, 7, 6, 6, 7, 7, 1).empty(),
              "W32-R N-152d: Tovolar's Packleader (true 7/7) renders no (printed 6/6) tail");
        CHECK(printedPTTag(5, 5, 2, 2, 3, 3, 1) == " (printed 3/3)",
              "W32-R N-152d: a transformed DFC that IS buffed reports the BACK face's printed stats");
        CHECK(printedPTTag(4, 4, 2, 2, 2, 2, 0) == " (printed 2/2)",
              "W32-R N-152d NEGATIVE: an unflipped, genuinely buffed creature is unchanged");
        CHECK(printedPTTag(1, 1, 1, 1, 4, 4, 0).empty(),
              "W32-R N-152d NEGATIVE: an unflipped set-P/T creature at its base renders no tail");

        // ---- N-158g: the mana line leads with the COUNT, not the colour set.
        {
            string ml = manaAvailableLine(5, "{r}{b}");
            cout << "     N-158g mana line -> " << ml << "\n";
            CHECK(ml.compare(0, 8, "5 total ") == 0,
                  "W32-R N-158g: the mana line LEADS with the source count");
            CHECK(ml.find("five untapped sources") != string::npos,
                  "W32-R N-158g: the count is also spelled in words (a digit cannot bind to a colour)");
            CHECK(ml.find("colours you can make: {r}{b}") != string::npos,
                  "W32-R N-158g: the colours survive, demoted to a named sub-clause");
            CHECK(ml[0] != '{',
                  "W32-R N-158g NEGATIVE: the line no longer OPENS with a mana symbol (the misbinding)");
            CHECK(manaAvailableLine(1, "{w}").find("one untapped source,") != string::npos,
                  "W32-R N-158g: a single source is singular");
            CHECK(manaAvailableLine(0, "") == "0 total (no untapped sources)",
                  "W32-R N-158g: the empty case still leads with a count");
            CHECK(manaAvailableLine(12, "{g}").find("12 total (untapped sources,") != string::npos,
                  "W32-R N-158g: past the word table the digit stands alone (no out-of-range read)");
        }

        // ---- N-152e: the transform-DFC Flip-Side text is truthful, and the two
        // emitters share ONE string (this defect existed only because they had
        // drifted - the h4 rewrite landed on the LAND branch alone).
        {
            string tn = transformDfcToggleNote("Brutal Cathar", "Moonrage Brute", "");
            cout << "     N-152e transform note -> " << tn.substr(0, 96) << "...\n";
            CHECK(tn.find("alternative-cost cast for it") != string::npos
                  && tn.find("NEVER cast as its other face") != string::npos,
                  "W32-R N-152e: the transform-DFC note DENIES the alternative-cost cast route");
            CHECK(tn.find("appears there as an alternative-cost cast") == string::npos,
                  "W32-R N-152e NEGATIVE: the old false promise is gone from this branch");
            CHECK(tn.find("Moonrage Brute") != string::npos && tn.find("backside") == string::npos,
                  "W32-R N-152e: the back face resolves to its real NAME, not the literal placeholder");
            CHECK(transformDfcToggleNote("Tergrid", "Tergrid's Lantern", "{3}{B}").find("({3}{B})") != string::npos,
                  "W32-R N-152e: an other-face cost is rendered when known");
            {
                vector<string> dfc;
                dfc.push_back("Cast Card Normally");
                dfc.push_back("Flip Side [" + tn.substr(4) + "]");
                dfc.push_back("Decline");
                bool stale = false;
                int c = parseChoice("2 (Flip Side)", 3, &dfc, &stale, NULL);
                cout << "     N-152e annotated transform 'Flip Side' echo -> " << c << " (must be 2)\n";
                CHECK(c == 2 && !stale,
                      "W32-R N-152e: a 'Flip Side' echo still binds to the transform-annotated option");
                int c1 = parseChoice("1 (Cast Card Normally)", 3, &dfc, &stale, NULL);
                CHECK(c1 == 1, "W32-R N-152e NEGATIVE: the sibling options are unaffected by the new tail");
            }
        }

        // ---- N-148d: a creature in HAND carries an explicit type marker.
        {
            vector<string> hand;
            hand.push_back("Kor Duelist {w} (1/1) [creature]");
            hand.push_back("Plains (land: taps for {W})");
            bool stale = false;
            int c = parseChoice("1 (Kor Duelist {w} (1/1) [creature])", 2, &hand, &stale, NULL);
            cout << "     N-148d hand echo with [creature] tail -> " << c << " (must be 1)\n";
            CHECK(c == 1 && !stale, "W32-R N-148d: an echo carrying the [creature] tail still binds");
            int c2 = parseChoice("2 (Plains)", 2, &hand, &stale, NULL);
            CHECK(c2 == 2, "W32-R N-148d NEGATIVE: the land line's own tag is untouched");
        }

        // ---- N-148b: the reverse attachment render carries the host's handle.
        {
            vector<string> board;
            board.push_back("Cranial Plating {1} [artifact] [attached to: Kor ally #2]");
            board.push_back("Captain's Claws {2} [artifact] [attached to: Kor ally #1]");
            bool stale = false;
            int c = parseChoice("1 (Cranial Plating [attached to: Kor ally #2])", 2, &board, &stale, NULL);
            cout << "     N-148b echo with handled [attached to: ...] tail -> " << c << " (must be 1)\n";
            CHECK(c == 1 && !stale,
                  "W32-R N-148b: an echo carrying the handled [attached to: X #N] tail still binds");
            int c2 = parseChoice("2 (Captain's Claws [attached to: Kor ally #1])", 2, &board, &stale, NULL);
            CHECK(c2 == 2,
                  "W32-R N-148b NEGATIVE: the two same-host lines are now DISTINGUISHABLE and each binds to itself");
        }

        // ---- N-146i: Cast option lines carry card text, like ability lines do.
        {
            vector<string> casts;
            casts.push_back("Cast Acererak the Archlich {2}{b} (5/5) {card text: \"When Acererak enters,"
                            " if you haven't completed Tomb of Annihilation, return it to its owner's hand.\"}");
            casts.push_back("Cast Vanishing Verse {w}{b}");
            casts.push_back("Cast nothing right now");
            bool stale = false;
            int c = parseChoice("1 (Cast Acererak the Archlich)", 3, &casts, &stale, NULL);
            cout << "     N-146i cast echo against a {card text: ...} line -> " << c << " (must be 1)\n";
            CHECK(c == 1 && !stale, "W32-R N-146i: a bare cast echo binds to a card-text-annotated option");
            int c3 = parseChoice("3 (Cast nothing right now)", 3, &casts, &stale, NULL);
            CHECK(c3 == 3, "W32-R N-146i NEGATIVE: the decline option still binds LAST");
        }

        // ---- N-158c / N-158a: the amass detector, replayed against the REAL
        // primitive scripts (verified against bin/Res/sets/primitives).
        {
            // Foray of Orcs: "amass 2, THEN damage:power" - the whole reason the
            // magnitude rendered 0 (evaluated on a power-0 sorcery, pre-amass).
            CHECK(amassCountersFromScript(
                      "if type(army|mybattlefield)~morethan~0 then name(Put 1/1 counters)"
                      " notaTarget(army|myBattlefield) transforms((Orc,newability[counter(1/1.2)],"
                      "newability[name(Damage creature) damage:power target(creature|opponentbattlefield)])) forever")
                  == 2,
                  "W32-R N-158c: Foray of Orcs is detected as amass 2");
            CHECK(amassCountersFromScript(
                      "if type(army|mybattlefield)~morethan~0 then transforms((Orc,newability[counter(1/1.3)],"
                      "newability[choice name(Opponent mills cards) deplete:power opponent])) forever")
                  == 3,
                  "W32-R N-158a: Surrounded by Orcs is detected as amass 3");
            // ATTRIBUTION GUARD (the N-146g shape). Barad-dur enumerates one
            // branch per X in ONE magicText; stamping any single number onto the
            // offered option would be confidently WRONG - worse than silence.
            CHECK(amassCountersFromScript(
                      "{12}{b}{t}:if type(army|mybattlefield)~morethan~0 then transforms((Orc,newability"
                      "[counter(1/1.6)])) forever {14}{b}{t}: transforms((Orc,newability[counter(1/1.7)]))"
                      " {16}{b}{t}: transforms((Orc,newability[counter(1/1.8)]))")
                  == 0,
                  "W32-R N-158c NEGATIVE: a MULTI-BRANCH amass card (Barad-dur/Fall of Cair Andros) emits NO preview");
            CHECK(amassCountersFromScript(
                      "if type(army|mybattlefield)~morethan~0 then transforms((Orc,newability[counter(1/1.x)])) forever")
                  == 0,
                  "W32-R N-158c NEGATIVE: an announced-X amass is unknowable before announcement -> no preview");
            CHECK(amassCountersFromScript(
                      "lord(*[orc;goblin]|mybattlefield) newability[counter(1/1.2)]") == 0,
                  "W32-R N-158c NEGATIVE: a non-amass card is not detected as amass (no army|mybattlefield)");
            CHECK(amassCountersFromScript("") == 0,
                  "W32-R N-158c NEGATIVE: an empty script is not amass");
            // ON-RESOLUTION guard: a LATER-event amass must not be previewed on
            // the cast line as though it were about to happen.
            CHECK(amassCountersFromScript(
                      "_dies_if type(army|mybattlefield)~morethan~0 then counter(1/1)"
                      " notatarget(army|mybattlefield)") == 0,
                  "W32-R N-158a NEGATIVE: Grim Initiate amasses when it DIES -> no cast-line preview");
            CHECK(amassCountersFromScript(
                      "@movedto(*|mystack) restriction{thisturn(*|mystack)~equalto~1,"
                      "type(army|mybattlefield)~morethan~0}:transforms((Orc,newability[counter(1/1.2)]))") == 0,
                  "W32-R N-158a NEGATIVE: Saruman the White's TRIGGERED amass gets no cast-line preview");
            CHECK(amassCountersFromScript(
                      "@discarded(*|myhand):name(Pay 1 and amass 2)\n"
                      "ifnot paid(alternative) then target(army|mybattlefield)"
                      " transforms((Orc,newability[counter(1/1.2)]))") == 2,
                  "W32-R N-158a: a card mixing a triggered line with an on-resolution amass keeps the on-resolution one");
        }

        // ---- N-158h: the per-target self-cost fold. The clause that used to
        // print a CONSTANT "life -2" for every target now rides each target.
        {
            vector<string> opts;
            opts.push_back("Cast Feed the Swarm {1}{b} - legal targets right now:"
                           " Nadaar, Selfless Paladin (costs you 3 life), Serra Ascendant (costs you 1 life)");
            opts.push_back("Cast nothing right now");
            bool stale = false;
            int c = parseChoice("1 (Cast Feed the Swarm)", 2, &opts, &stale, NULL);
            cout << "     N-158h per-target cost line echo -> " << c << " (must be 1)\n";
            CHECK(c == 1 && !stale,
                  "W32-R N-158h: a cast echo binds against the per-target life-cost annotation");
        }
    }

    // ================= WAVE-33 items (N-152j / N-158n name-form BLOCKS,
    // ================= plus the COMMIT-FAILURE counters) ==================
    cout << "\n=== WAVE-33: name-form BLOCKS parsing + commit-failure counters ===\n";
    {
        // ---- N-152j, the deck139 vs152 s21 witness, VERBATIM head line.
        // Board: B1 = Pollywog Symbiote, may block A1..A4; A5 = Elite
        // Spellbinder [flying] is NOT in B1's legal set.
        cout << "\n[W33 N-152j] 'BLOCKS: Pollywog Symbiote blocks Elite Spellbinder'\n";
        vector<string> bn; bn.push_back("Pollywog Symbiote");
        vector<string> an;
        an.push_back("Grizzly Bears"); an.push_back("Llanowar Elves");
        an.push_back("Memnite"); an.push_back("Ornithopter");
        an.push_back("Elite Spellbinder");
        vector<vector<int> > legal(1);
        legal[0].push_back(0); legal[0].push_back(1); legal[0].push_back(2); legal[0].push_back(3);
        {
            // The parser RESOLVES the intent (the pair is now visible); the
            // apply site's canBlock gate is what refuses it. Pre-fix this whole
            // line was dropped -> pairs 0 -> unparsed_reply -> Baka.
            vector<int> out;
            int pairs = parseBlockAssignments("BLOCKS: Pollywog Symbiote blocks Elite Spellbinder",
                                              1, 5, out, &bn, &an, &legal);
            cout << "     pairs=" << pairs << " B1->A" << (out.empty() ? 0 : out[0])
                 << " (illegal: the validator prunes it, not the parser)\n";
            CHECK(pairs == 1 && out[0] == 5,
                  "W33 N-152j: name-form 'X blocks Y' past the BLOCKS: label resolves (was unparsed_reply)");
        }
        {
            // Same line, LEGAL variant: A5 in B1's legal set -> a real block.
            vector<vector<int> > legalAll(1);
            for (int k = 0; k < 5; k++) legalAll[0].push_back(k);
            vector<int> out;
            int pairs = parseBlockAssignments("BLOCKS: Pollywog Symbiote blocks Elite Spellbinder",
                                              1, 5, out, &bn, &an, &legalAll);
            CHECK(pairs == 1 && out[0] == 5,
                  "W33 N-152j LEGAL variant: the same reply declares B1 blocks A5 when A5 is offered");
        }

        // ---- N-158n, the deck158 vs152 s35 witness, VERBATIM head line.
        // Board: A1 = Moonrage Brute, A2 = Sigarda [flying]; B1 = Orc army,
        // may block A1 only. "Dunland Crebain" is tapped and is NOT a B-line.
        cout << "\n[W33 N-158n] 'BLOCKS: Orc army: Sigarda, Champion of Light; Dunland Crebain: Moonrage Brute'\n";
        vector<string> bn2; bn2.push_back("Orc army");
        vector<string> an2; an2.push_back("Moonrage Brute"); an2.push_back("Sigarda, Champion of Light");
        vector<vector<int> > legal2(1); legal2[0].push_back(0);
        {
            vector<int> out;
            int pairs = parseBlockAssignments(
                "BLOCKS: Orc army: Sigarda, Champion of Light; Dunland Crebain: Moonrage Brute",
                1, 2, out, &bn2, &an2, &legal2);
            cout << "     pairs=" << pairs << " B1->A" << (out.empty() ? 0 : out[0])
                 << " (Sigarda flies -> pruned at apply; 'Dunland Crebain' is no B-line -> dropped here)\n";
            CHECK(pairs == 1 && out[0] == 2,
                  "W33 N-158n: NAME:NAME with a ';' pair separator resolves past the label colon");
        }
        {
            // The SAME shape with a legal pair: this is the forward-looking
            // value of the item - a name-form block that actually happens.
            vector<int> out;
            int pairs = parseBlockAssignments("BLOCKS: Orc army: Moonrage Brute", 1, 2, out, &bn2, &an2, &legal2);
            cout << "     legal name-form: pairs=" << pairs << " B1->A" << (out.empty() ? 0 : out[0]) << "\n";
            CHECK(pairs == 1 && out[0] == 1,
                  "W33 N-158n POSITIVE: a LEGAL name-form block declares B1:A1");
        }
        {
            // AMBIGUITY GUARD: two identically-named blockers, no "#N" handle
            // -> the pair drops whole. A guess would pick the wrong creature.
            vector<string> bnDup; bnDup.push_back("Orc army"); bnDup.push_back("Orc army");
            vector<vector<int> > legalDup(2); legalDup[0].push_back(0); legalDup[1].push_back(0);
            vector<int> out;
            int pairs = parseBlockAssignments("BLOCKS: Orc army: Moonrage Brute", 2, 2, out, &bnDup, &an2, &legalDup);
            cout << "     duplicate blocker name, no handle: pairs=" << pairs << " (must drop)\n";
            CHECK(pairs == 0 && out[0] == 0 && out[1] == 0,
                  "W33 N-158n NEGATIVE: an ambiguous duplicate blocker name with no #N handle drops the pair");
            // ...and the RENDERED handle resolves it.
            vector<int> out2;
            int pairs2 = parseBlockAssignments("BLOCKS: Orc army #2: Moonrage Brute", 2, 2, out2, &bnDup, &an2, &legalDup);
            cout << "     handle form '#2': pairs=" << pairs2 << " B1->A" << out2[0] << " B2->A" << out2[1] << "\n";
            CHECK(pairs2 == 1 && out2[0] == 0 && out2[1] == 1,
                  "W33 N-158n: the rendered '#2' handle picks the second same-named blocker");
        }
        {
            // A name that matches NOTHING on either list must not invent a block.
            vector<int> out;
            int pairs = parseBlockAssignments("BLOCKS: Serra Angel: Shivan Dragon", 1, 2, out, &bn2, &an2, &legal2);
            CHECK(pairs == 0 && out[0] == 0,
                  "W33 N-158n NEGATIVE: names on neither list fabricate no block");
        }
        {
            // REGRESSION: the coded form is untouched by the label strip and by
            // the ';' separator - both the pure and the mixed shapes.
            vector<int> out;
            int pairs = parseBlockAssignments("BLOCKS: B1:A1", 1, 2, out, &bn2, &an2, &legal2);
            CHECK(pairs == 1 && out[0] == 1, "W33 regression: coded 'BLOCKS: B1:A1' still parses");
            vector<string> bn3; bn3.push_back("Wall"); bn3.push_back("Bear"); bn3.push_back("Bird");
            vector<string> an3; an3.push_back("Ogre"); an3.push_back("Troll");
            vector<vector<int> > legal3(3);
            for (int i = 0; i < 3; i++) { legal3[i].push_back(0); legal3[i].push_back(1); }
            vector<int> out2;
            int pairs2 = parseBlockAssignments("BLOCKS: B1:A2, B3:A1, B2:none", 3, 2, out2, &bn3, &an3, &legal3);
            cout << "     coded triple: pairs=" << pairs2 << " -> " << out2[0] << "," << out2[1] << "," << out2[2] << "\n";
            CHECK(pairs2 == 3 && out2[0] == 2 && out2[1] == 0 && out2[2] == 1,
                  "W33 regression: the protocol example 'B1:A2, B3:A1, B2:none' is unchanged");
            // MIXED coded+name: the coded pair wins its blocker, the name pair
            // fills a DIFFERENT one (the name pass never overrides a code).
            vector<int> out3;
            int pairs3 = parseBlockAssignments("BLOCKS: B1:A2, Bird blocks Ogre", 3, 2, out3, &bn3, &an3, &legal3);
            CHECK(pairs3 == 2 && out3[0] == 2 && out3[1] == 0 && out3[2] == 1,
                  "W33: a mixed coded+name reply keeps both, code first");
        }
        {
            // ATTACK ';' symmetry (the same separator gap on the attackers seam).
            vector<string> an4; an4.push_back("Hellrider"); an4.push_back("Rakdos Cackler"); an4.push_back("Grizzly Bears");
            vector<bool> send;
            int r = parseAttackerSet("ATTACK: Hellrider; Rakdos Cackler", 3, send, &an4);
            cout << "     ATTACK ';' list: result=" << r << " [" << (int) send[0] << ","
                 << (int) send[1] << "," << (int) send[2] << "]\n";
            CHECK(r == 2 && send[0] && send[1] && !send[2],
                  "W33: a ';'-joined ATTACK name list declares both (was one merged unmatchable segment)");
            // NEGATIVE: an unlisted name still declares nobody, not everybody.
            vector<bool> send2;
            int r2 = parseAttackerSet("ATTACK: Shivan Dragon", 3, send2, &an4);
            CHECK(r2 == 0 && !send2[0] && !send2[1] && !send2[2],
                  "W33 NEGATIVE: an ineligible ATTACK name declares the empty set, never a guess");
        }

        // ---- COMMIT-FAILURE counters (instrument only; no behaviour rides them).
        cout << "\n[W33 commit-failure] post_plan_overrun / commit_retracted\n";
        {
            string compliant = "CHOICE: 1 (Cast Mordor Muster {1}{b})\nIt is the best line.\nPLAN: build the army.\n";
            CHECK(postPlanOverrun(compliant) == 0,
                  "W33: a compliant reply ending at its PLAN: line scores post_plan_overrun 0");
            string overrun = "CHOICE: 1 (Cast Mordor Muster {1}{b})\nPLAN: build the army.\n"
                             "Wait, looking at the opponent's board... Actually, let's re-evaluate.";
            long n = postPlanOverrun(overrun);
            cout << "     overrun tail measured: " << n << " chars\n";
            //69 = strlen("Wait, looking at the opponent's board... Actually,
            //let's re-evaluate.") - the un-committed tail, exactly.
            CHECK(n == 69,
                  "W33: the deck158 vs139 s16 shape scores its exact post-PLAN tail length");
            CHECK(postPlanOverrun("CHOICE: 1\nno plan here at all") == 0,
                  "W33 NEGATIVE: a reply with no PLAN: line scores 0 (it never committed a plan)");
            CHECK(postPlanOverrun("<think>PLAN: ignore me\nlots of hidden reasoning</think>CHOICE: 1\nPLAN: go.") == 0,
                  "W33 NEGATIVE: a PLAN: inside a </think> block is not the reply's plan");
            // commit_retracted is the disjunction of the three retraction exits
            // AND a coded answer line actually existing.
            CHECK(commitRetracted("retracted_choice", overrun),
                  "W33: retracted_choice on a reply carrying a coded answer sets commit_retracted");
            CHECK(commitRetracted("truncated_abandoned", "BLOCKS: B1:A1\nActually no, I should not block."),
                  "W33: truncated_abandoned sets commit_retracted");
            CHECK(commitRetracted("truncated_abandoned_heuristic", "BLOCKS: B1:A1\nActually no."),
                  "W33: truncated_abandoned_heuristic sets commit_retracted");
            CHECK(!commitRetracted("unparsed_reply", overrun),
                  "W33 NEGATIVE: unparsed_reply is NOT a retraction (no answer was committed)");
            CHECK(!commitRetracted("empty_reply", ""),
                  "W33 NEGATIVE: empty_reply is not a retraction");
            CHECK(!commitRetracted(NULL, overrun),
                  "W33 NEGATIVE: a record the model's own answer carried is never commit_retracted");
            CHECK(!commitRetracted("retracted_choice", "I think option one is fine but actually no."),
                  "W33 NEGATIVE: no line-leading coded answer -> nothing was committed to retract");
        }
    }

    // ==== WAVE-33 step-1 (render lane) cases ====

    // ---- N-105a: the poison status line ----
    cout << "\n[W33-N105a] poison counters render, with the 10-counter threshold explicit\n";
    {
        // NEGATIVE: a game with no poison in it must be byte-untouched.
        CHECK(poisonStatusLine(0, 0).empty(),
              "W33-N105a no poison anywhere -> no status line at all");
        // POSITIVE: this seat's own counters, threshold and remainder explicit.
        string mine = poisonStatusLine(6, 0);
        cout << "     " << mine;
        CHECK(mine.find("Poison counters (you): 6 of 10") != string::npos,
              "W33-N105a the seat's own poison total renders against the 10 threshold");
        CHECK(mine.find("4 more end it") != string::npos,
              "W33-N105a the distance to the loss threshold is stated as a number");
        CHECK(mine.find("(opponent)") == string::npos,
              "W33-N105a an unpoisoned opponent contributes no line");
        // Both players, and the opponent's line names the OPPONENT as the loser.
        string both = poisonStatusLine(6, 3);
        CHECK(both.find("Poison counters (opponent): 3 of 10") != string::npos
              && both.find("the opponent LOSES the game at 10") != string::npos,
              "W33-N105a both players' totals render, each against its own loss clause");
        // The threshold-reached case must not print a negative remainder.
        string over = poisonStatusLine(10, 0);
        CHECK(over.find("-") == string::npos || over.find(" -1 more") == string::npos,
              "W33-N105a at/over the threshold no negative remainder is emitted");
        CHECK(over.find("already reached") != string::npos,
              "W33-N105a at the threshold the line says so outright");
        // The wording must not hand the model an affirmative all-clear to latch.
        CHECK(mine.find("safe") == string::npos && mine.find("plenty") == string::npos,
              "W33-N105a no affirmative all-clear substring in the poison line");
    }

    // ---- N-105a (d): the poison keyword family is decodable ----
    cout << "\n[W33-N105a] opaque engine keyword names for the poison family are translated\n";
    {
        CHECK(legibleKeywordName("poisontoxic") == "toxic 1",
              "W33-N105a 'poisontoxic' renders as toxic 1");
        CHECK(legibleKeywordName("poisonthreetoxic") == "toxic 3",
              "W33-N105a the spelled-out toxic magnitudes decode");
        CHECK(legibleKeywordName("poisontentoxic") == "toxic 10",
              "W33-N105a the two-syllable top of the range decodes");
        CHECK(legibleKeywordName("poisondamager")
                  == "deals its damage to players as poison counters",
              "W33-N105a 'poisondamager' is spelled out as what it does");
        // NEGATIVES: every other keyword - including the poison-adjacent ones -
        // must come back byte-identical, or 100+ existing option lines shift.
        CHECK(legibleKeywordName("infect") == "infect", "W33-N105a 'infect' is unchanged");
        CHECK(legibleKeywordName("wither") == "wither", "W33-N105a 'wither' is unchanged");
        CHECK(legibleKeywordName("poisonshroud") == "poisonshroud",
              "W33-N105a a poison-prefixed keyword that is NOT toxic is unchanged");
        CHECK(legibleKeywordName("flying") == "flying" && legibleKeywordName("") == "",
              "W33-N105a ordinary keywords and the empty name are untouched");
        // Echo shape: an attacker line carrying the translated keyword still binds.
        vector<string> blockOpts;
        blockOpts.push_back("Block A1. Phyrexian Crusader (2/2) [infect, toxic 1]");
        blockOpts.push_back("Do not block");
        bool stale = false;
        int c = parseChoice("1 (Block A1. Phyrexian Crusader)", 2, &blockOpts, &stale, NULL);
        cout << "     translated-keyword echo -> " << c << " (must be 1)\n";
        CHECK(c == 1 && !stale,
              "W33-N105a an echo binds against a line carrying the translated keywords");
    }

    // ---- N-105a: poison GAIN narration (covers alterpoison, which narrated nothing) ----
    cout << "\n[W33-N105a] every poison gain narrates, with a real delta and total\n";
    {
        string g = poisonGainLine(true, 3, 6);
        cout << "     " << g << "\n";
        CHECK(g.find("you take 3 poison counters - now 6 of 10") != string::npos,
              "W33-N105a a poison gain states delta AND settled total");
        CHECK(g.find("4 more end it") != string::npos,
              "W33-N105a the gain line carries the distance to the threshold");
        string one = poisonGainLine(false, 1, 1);
        CHECK(one.find("the opponent takes 1 poison counter - now 1 of 10") != string::npos,
              "W33-N105a singular/plural and the opponent's side both render");
        // A gain the seat cannot attribute a delta to still states the total.
        string nod = poisonGainLine(true, 0, 2);
        CHECK(nod.find("you now have 2 of 10") != string::npos,
              "W33-N105a an unknown delta degrades to the settled total, never to silence");
    }

    // ---- N-105b: the blocker-seam forecast is infect-aware ----
    cout << "\n[W33-N105b] the block forecast prices poison against 10, not life against 20\n";
    {
        // CONTROL: a non-poison swing keeps the validated wording byte-for-byte.
        string plain = combatDamageForecast(20, 0, 5, 0);
        cout << "     " << plain;
        CHECK(plain == "Your life: 20. Unblocked, these attackers deal up to 5 - you would be at 15"
                       " - NOT lethal: block only where the trade favors you; taking damage while"
                       " ahead is often correct.\n",
              "W33-N105b a swing with no poison in it is unchanged");
        CHECK(combatDamageForecast(4, 0, 5, 0).find("LETHAL if it all connects") != string::npos,
              "W33-N105b the lethal branch of the non-poison forecast is unchanged");

        // The deck36 s25 t12 GAME-LOSING window: 5 infect power onto 6 poison.
        // The old line said "you would be at 15 - NOT lethal" and the seat took it.
        string inf = combatDamageForecast(20, 6, 0, 5);
        cout << "     " << inf;
        CHECK(inf.find("you would be at 15") == string::npos,
              "W33-N105b an infect swing emits NO life-total forecast for its damage");
        CHECK(inf.find("11 of 10 poison") != string::npos
              && inf.find("LETHAL if it all connects") != string::npos,
              "W33-N105b crossing 10 poison is reported as LETHAL");
        CHECK(inf.find("taking damage while ahead is often correct") == string::npos,
              "W33-N105b the take-the-damage advice is DROPPED on any poison-relevant swing");
        CHECK(inf.find("Your poison counters: 6 of 10") != string::npos,
              "W33-N105b the forecast restates the current poison count it is pricing against");

        // Sub-threshold poison: not lethal, but never framed as affordable.
        string sub = combatDamageForecast(20, 2, 0, 3);
        CHECK(sub.find("5 of 10 poison") != string::npos
              && sub.find("taking damage while ahead is often correct") == string::npos,
              "W33-N105b a sub-threshold poison swing still drops the stale advice");
        CHECK(sub.find("permanent progress toward losing") != string::npos,
              "W33-N105b sub-threshold poison is framed as permanent, not as slack");

        // MIXED swing (a toxic attacker): BOTH halves must be priced.
        string mixed = combatDamageForecast(9, 7, 4, 2);
        cout << "     " << mixed;
        CHECK(mixed.find("9 of 10 poison") != string::npos
              && mixed.find("ORDINARY damage - your life would be 5") != string::npos,
              "W33-N105b a mixed swing prices poison AND life, both explicitly");
    }

    // ---- N-36b: a {0} cost is a cost, not an absence ----
    cout << "\n[W33-N36b] {0} renders instead of being deleted by the falsy-zero guard\n";
    {
        CHECK(manaCostTokenText("{2}{b}", true) == " {2}{b}",
              "W33-N36b an ordinary cost renders unchanged");
        CHECK(manaCostTokenText("", true) == " {0}",
              "W33-N36b an EMPTY cost string on a castable card renders {0}");
        CHECK(manaCostTokenText("", false) == "",
              "W33-N36b a land/token gets no synthesized {0}");
        CHECK(manaCostTokenText("{x}", true) == " {x}",
              "W33-N36b an X-only cost (also converted-0) survives as {x}, not {0}");
        // Echo shape: a Mox cast line now carries a {0} the reply may echo back.
        vector<string> opts;
        opts.push_back("Cast Mox Ruby {0} [artifact]");
        opts.push_back("Cast nothing right now");
        bool stale = false;
        int c = parseChoice("1 (Cast Mox Ruby {0})", 2, &opts, &stale, NULL);
        cout << "     {0} cast echo -> " << c << " (must be 1)\n";
        CHECK(c == 1 && !stale, "W33-N36b a cast echo carrying the new {0} token still binds");
        int c2 = parseChoice("2 (Cast nothing right now)", 2, &opts, &stale, NULL);
        CHECK(c2 == 2 && !stale, "W33-N36b the decline option is unaffected by the {0} token");
    }

    // ---- N-152d layer 2: the printed-face discriminator ----
    cout << "\n[W33-N152d] a day/night transform (isFlipped left 0) no longer fakes a buff\n";
    {
        // Moonrage Brute: power 3/3, back-face orig 3/3, FRONT-face base 2/2.
        // AAFlip::testDestroy resets isFlipped to 0, so the old call site passed 0
        // here and printed "(printed 2/2)" - Brutal Cathar's stats - 22x in corpus.
        CHECK(printedPTTag(3, 3, 2, 2, 3, 3, /*showsOtherFace*/1).empty(),
              "W33-N152d a transformed DFC identified by NAME renders no (printed ...) tail");
        // The pre-fix behaviour, pinned as the thing that must not come back.
        CHECK(printedPTTag(3, 3, 2, 2, 3, 3, /*showsOtherFace*/0) == " (printed 2/2)",
              "W33-N152d control: the old front-face reading is what produced the false tag");
        // The genuinely-buffed transformed case still reports the BACK face.
        CHECK(printedPTTag(5, 5, 2, 2, 3, 3, 1) == " (printed 3/3)",
              "W33-N152d a buffed transformed DFC still reports its displayed face's stats");
        // An unflipped buffed creature is untouched.
        CHECK(printedPTTag(4, 4, 2, 2, 2, 2, 0) == " (printed 2/2)",
              "W33-N152d an ordinary pumped creature is unchanged");
    }

    // ---- N-158m: the amass scanner sees through macros ----
    cout << "\n[W33-N158m] amass magnitudes survive macro indirection; both guards hold\n";
    {
        // Register the REAL macros from Res/sets/primitives/_macros.txt. PARSETEST
        // runs before the collection loads, so without this the expansion path is
        // a no-op and the test would pass vacuously.
        AutoLineMacro::AddMacro("_DIES_ @movedTo(this|graveyard) from(battlefield):");
        AutoLineMacro::AddMacro("_AMASSORC1_ if type(army|mybattlefield)~morethan~0 then name(Put 1/1 counter)"
                                " notatarget(army|myBattlefield) transforms((Orc,newability[counter(1/1)])) forever"
                                " else name(Create Orc Army) token(Orc Army^Creature Orc Army^0/0^black)"
                                " and!( name(Put 1/1 counter) counter(1/1) notatarget(army|myBattlefield) )!");
        AutoLineMacro::AddMacro("_AMASSORC3_ if type(army|mybattlefield)~morethan~0 then name(Put 1/1 counters)"
                                " notatarget(army|myBattlefield) transforms((Orc,newability[counter(1/1.3)])) forever"
                                " else name(Create Orc Army) token(Orc Army^Creature Orc Army^0/0^black)"
                                " and!( name(Put 1/1 counters) counter(1/1.3) notatarget(army|myBattlefield) )!");

        // Mordor Muster class: a plain untriggered macro line. 0 before this fix.
        int muster = amassCountersFromScript("_AMASSORC3_");
        cout << "     macro-defined amass 3 -> " << muster << " (must be 3)\n";
        CHECK(muster == 3, "W33-N158m a macro-defined amass reports its real magnitude");
        CHECK(amassCountersFromScript("_AMASSORC1_") == 1,
              "W33-N158m the no-dot counter(1/1) macro form reports 1");

        // GUARD 1, trigger-gated: Easterling Vanguard is "_DIES_ _AMASSORC1_".
        // The filter reads the RAW line, so _DIES_ is caught before expansion -
        // and its expansion starts with '@', so the second check catches it too.
        CHECK(amassCountersFromScript("_DIES_ _AMASSORC1_") == 0,
              "W33-N158m a DIES-triggered amass still previews NOTHING");
        CHECK(amassCountersFromScript("@movedTo(this|graveyard) from(battlefield): _AMASSORC1_") == 0,
              "W33-N158m an already-expanded trigger line is still suppressed");

        // GUARD 2, multi-branch: an X-spell enumerating several magnitudes.
        CHECK(amassCountersFromScript("_AMASSORC1_\n_AMASSORC3_") == 0,
              "W33-N158m a card agreeing on no single count emits no number");

        // The hand-written control (Foray of Orcs) is unchanged by the new path.
        CHECK(amassCountersFromScript(
                  "if type(army|mybattlefield)~morethan~0 then notatarget(army|myBattlefield)"
                  " transforms((Orc,newability[counter(1/1.2)])) forever") == 2,
              "W33-N158m the already-expanded scan is byte-unchanged");
        CHECK(amassCountersFromScript("") == 0, "W33-N158m an empty script amasses nothing");
        CHECK(amassCountersFromScript("token(Goblin)") == 0,
              "W33-N158m a non-amass script is untouched by the expansion pass");

        // ---- The REAL deck158 cards, scripts copied verbatim from
        // Res/sets/primitives/borderline.txt. This is the coverage claim itself,
        // proven rather than predicted: 1 of 7 amass cards previewed before.
        AutoLineMacro::AddMacro("_AMASSORC2_ if type(army|mybattlefield)~morethan~0 then name(Put 1/1 counters)"
                                " notatarget(army|myBattlefield) transforms((Orc,newability[counter(1/1.2)])) forever"
                                " else name(Create Orc Army) token(Orc Army^Creature Orc Army^0/0^black)"
                                " and!( name(Put 1/1 counters) counter(1/1.2) notatarget(army|myBattlefield) )!");
        AutoLineMacro::AddMacro("_TREASURE_ token(Treasure^Treasure Artifact^0/0) and!( transforms((,"
                                "newability[{T}{S}:Add{W}],newability[{T}{S}:Add{U}],newability[{T}{S}:Add{B}],"
                                "newability[{T}{S}:Add{R}],newability[{T}{S}:Add{G}])) forever )!");

        struct { const char * name; const char * script; int want; } kReal[] = {
            { "Dunland Crebain",           "_AMASSORC2_", 2 },
            { "March from the Black Gate",
              "_AMASSORC1_\n@combat(attacking) source(army|mybattlefield):name(Amass orcs 1) _AMASSORC1_", 1 },
            { "Mordor Muster",
              "draw:1 controller\nlife:-1 controller\nability$!name(Amass orcs 1) _AMASSORC1_!$ controller", 1 },
            { "Orcish Bowmasters",
              "ability$!name(Amass orcs 1) _AMASSORC1_!$ controller\n"
              "name(Damage any target) damage:1 target(anytarget)\n"
              "@drawfoeof(player) restriction{compare(odrewcount)~morethan~1}:name(Amass Orcs 1)"
              " ability$!name(Amass orcs 1) _AMASSORC1_!$ controller", 1 },
            { "Swarming of Moria",         "_TREASURE_\n_AMASSORC2_", 2 },
            //Trigger-gated: amasses only when it DIES. Must stay silent.
            { "Easterling Vanguard",       "_DIES_ _AMASSORC1_", 0 },
        };
        for (size_t r = 0; r < sizeof(kReal) / sizeof(kReal[0]); r++)
        {
            int got = amassCountersFromScript(kReal[r].script);
            cout << "     " << kReal[r].name << " -> " << got
                 << " (must be " << kReal[r].want << ")\n";
            CHECK(got == kReal[r].want,
                  string("W33-N158m real primitive: ") + kReal[r].name);
        }

        // NEGATIVE CONTROL the docket names explicitly: Assault on Osgiliath's
        // nine fullpaid branches carry counter(1/1) through counter(1/1.10), so
        // the card agrees on no single count and must emit NOTHING. Note the
        // bare counter(1/1) branch now joins the same value set - as a fallback
        // it would have let a 10-branch X-spell report "10".
        {
            string osgiliath;
            for (int k = 1; k <= 10; k++)
            {
                std::ostringstream ln;
                ln << "alternative if compare(fullpaid)~equalto~" << k
                   << " then notatarget(army|myBattlefield) transforms((Orc,newability[counter(1/1";
                if (k > 1)
                    ln << "." << k;
                ln << ")])) forever\n";
                osgiliath += ln.str();
            }
            int got = amassCountersFromScript(osgiliath);
            cout << "     Assault on Osgiliath (10 branches) -> " << got << " (must be 0)\n";
            CHECK(got == 0,
                  "W33-N158m NEGATIVE CONTROL: a 10-branch X-spell still emits no amass number");
        }
    }

    // ---- N-152h: the third stale Flip-Side path (MDFC spell with a LAND back) ----
    cout << "\n[W33-N152h] an MDFC spell whose other face is a LAND stops promising a Cast-menu route\n";
    {
        string note = mdfcSpellLandBackNote("Emeria's Call", "Emeria, Shattered Skyclave", "{W}");
        cout << "     " << note << "\n";
        CHECK(note.find("appears there as an alternative-cost cast") == string::npos,
              "W33-N152h the retired false promise is GONE from the spell//land path");
        CHECK(note.find("other face is a LAND") != string::npos
              && note.find("A land is never CAST") != string::npos,
              "W33-N152h the note says what the other face IS and why it has no cast route");
        CHECK(note.find("taps for {W}") != string::npos,
              "W33-N152h the land face's colour is named");
        // The spell//spell path keeps its (true) Cast-menu statement.
        string spell = mdfcSpellToggleNote("Tergrid, God of Fright", "Tergrid's Lantern", "{3}");
        CHECK(spell.find("appears there as an alternative-cost cast") != string::npos,
              "W33-N152h control: the spell//spell class keeps its accurate wording");
        // Echo shape: a "Flip Side" reply still binds against the new annotation.
        vector<string> menu;
        menu.push_back("Cast Card Normally");
        menu.push_back("Flip Side" + note);
        menu.push_back("Decline");
        bool stale = false;
        int c = parseChoice("2 (Flip Side)", 3, &menu, &stale, NULL);
        cout << "     land-back Flip Side echo -> " << c << " (must be 2)\n";
        CHECK(c == 2 && !stale,
              "W33-N152h a 'Flip Side' echo binds to the land-back annotated option");
        int c1 = parseChoice("1 (Cast Card Normally)", 3, &menu, &stale, NULL);
        CHECK(c1 == 1 && !stale,
              "W33-N152h the normal cast still binds alongside the annotated Flip Side");
    }

    // ---- N-158k: the per-target life price at the COMMIT seat ----
    cout << "\n[W33-N158k] the target menu carries the same (costs you N life) the cast line does\n";
    {
        // The chooseTarget option now = describeTarget(...) + perTargetLifeCostNote.
        vector<string> opts;
        opts.push_back("Ornithopter (0/2) [flying] [opponent's battlefield] (costs you 0 life)");
        opts.push_back("Cathodion (3/3) [opponent's battlefield] (costs you 3 life)");
        bool stale = false;
        int c = parseChoice("1 (Ornithopter)", 2, &opts, &stale, NULL);
        cout << "     priced target echo -> " << c << " (must be 1)\n";
        CHECK(c == 1 && !stale,
              "W33-N158k a target-name echo binds against the new price annotation");
        int c2 = parseChoice("2 (Cathodion (costs you 3 life))", 2, &opts, &stale, NULL);
        cout << "     price-repeating echo -> " << c2 << " (must be 2)\n";
        CHECK(c2 == 2 && !stale,
              "W33-N158k an echo that repeats the price annotation still binds");
        // NEGATIVE: a target list for a spell with no per-target life cost must
        // not acquire one (the annotation is gated on the script shape upstream).
        vector<string> free_;
        free_.push_back("Ornithopter (0/2) [flying] [opponent's battlefield]");
        CHECK(free_[0].find("costs you") == string::npos,
              "W33-N158k control: an unpriced target line carries no price fragment");
    }

    // ---- N-158l: no empty [from ] bracket ----
    cout << "\n[W33-N158l] a counter-source bracket is emitted only when a name resolves\n";
    {
        // The emitter is inside describeEvent (needs an event), so the shape is
        // pinned at the parser boundary: an EMPTY bracket must never appear in a
        // counter line, and a NAMED one must still bind on echo.
        string good = "Counter added to Orc army: +1/+1 (now 3/3) [from Mauhur, Uruk-hai Captain]";
        CHECK(good.find("[from ]") == string::npos,
              "W33-N158l a resolved source keeps its bracket");
        string bare = "Counter added to Orc army: +1/+1 (now 3/3)";
        CHECK(bare.find("[from") == string::npos,
              "W33-N158l an unresolvable source emits NO bracket rather than an empty one");
    }

    // ---- N-146k / N-139n: the pregame block is hand-only and engine-counted ----
    cout << "\n[W33-N146k] a pregame ask carries the hand and NOTHING about a board\n";
    {
        int sources[5] = { 2, 0, 1, 0, 0 }; //two {W}, one {B}
        vector<string> reach;
        reach.push_back("Triumphant Adventurer {1}{b}");
        string h = pregameHandHeaderText(7, 3, 4, sources, "Triumphant Adventurer {1}{b}", 2, reach);
        cout << h;
        // THE DIRECTIVE: no board-state string may appear in a pregame prompt.
        CHECK(h.find("Mana available") == string::npos,
              "W33-N146k the pregame header contains no 'Mana available' line");
        CHECK(h.find("battlefield") == string::npos && h.find("Battlefield") == string::npos,
              "W33-N146k the pregame header contains no battlefield frame");
        CHECK(h.find("life") == string::npos && h.find("Phase") == string::npos
              && h.find("untapped") == string::npos,
              "W33-N146k no life total, phase or tap state reaches a pregame ask");
        // N-139n: the counts the guide layer twice failed to install.
        CHECK(h.find("3 lands, 4 spells") != string::npos,
              "W33-N139n the land/spell split is an engine-computed AGGREGATE");
        CHECK(h.find("{W} 2, {B} 1") != string::npos,
              "W33-N139n colour sources are counted per colour, not merely tagged per card");
        CHECK(h.find("A land in your hand IS a mana source") != string::npos,
              "W33-N139n the header states outright that a hand land counts");
        CHECK(h.find("would cover the cost of: Triumphant Adventurer {1}{b}") != string::npos,
              "W33-N139n the castability summary names what these lands reach");
        // The landless hand: the count must say zero rather than go silent.
        int none_[5] = { 0, 0, 0, 0, 0 };
        vector<string> noReach;
        string z = pregameHandHeaderText(7, 0, 7, none_, "Vanishing Verse {w}{b}", 2, noReach);
        CHECK(z.find("0 lands, 7 spells") != string::npos
              && z.find("none - this hand holds no lands at all") != string::npos,
              "W33-N139n a genuinely landless hand says so with the same counted shape");
        CHECK(z.find("would not cover any spell") == string::npos,
              "W33-N139n with zero lands no 'would not cover' claim is made");
        // Colourless-only lands still count as lands.
        string cl = pregameHandHeaderText(7, 2, 5, none_, "Steel Wall {1}", 1, noReach);
        CHECK(cl.find("2 lands, 5 spells") != string::npos
              && cl.find("no coloured sources") != string::npos,
              "W33-N139n colourless lands are counted as lands with an explicit colour verdict");
    }

    // ---- OpenAI-subscription adapter: Codex SSE text extraction ----
    // The backend answers ONLY as an SSE stream (stream:false is a 400), and
    // its response.completed event arrives with an EMPTY output array - the
    // text rides response.output_text.done / .delta (probed 2026-08-09).
    cout << "\n[CODEX-SSE] assistant text out of a Codex Responses SSE stream\n";
    {
        // The normal healthy stream: deltas plus a final done event carrying
        // the full text. done wins; the deltas must NOT double the text.
        string sse =
            "event: response.output_text.delta\n"
            "data: {\"type\":\"response.output_text.delta\",\"delta\":\"CHOICE\"}\n\n"
            "event: response.output_text.delta\n"
            "data: {\"type\":\"response.output_text.delta\",\"delta\":\": 1 (Cast Example Card)\"}\n\n"
            "event: response.output_text.done\n"
            "data: {\"type\":\"response.output_text.done\",\"text\":\"CHOICE: 1 (Cast Example Card)\"}\n\n"
            "event: response.completed\n"
            "data: {\"type\":\"response.completed\",\"response\":{\"status\":\"completed\",\"output\":[]}}\n";
        string t = gptCodexExtractText(sse);
        cout << "     healthy stream -> \"" << t << "\"\n";
        CHECK(t == "CHOICE: 1 (Cast Example Card)",
              "CODEX-SSE done event wins and deltas do not double the text");
        // Two content parts: their done events concatenate in stream order.
        string two =
            "data: {\"type\":\"response.output_text.done\",\"text\":\"CHOICE: 2\"}\n"
            "data: {\"type\":\"response.output_text.done\",\"text\":\"\\nPLAN: hold up mana.\"}\n";
        CHECK(gptCodexExtractText(two) == "CHOICE: 2\nPLAN: hold up mana.",
              "CODEX-SSE two content parts concatenate in order");
        // A stream cut before the done event: the deltas are the salvage.
        string cut =
            "data: {\"type\":\"response.output_text.delta\",\"delta\":\"BLOCKS: \"}\n"
            "data: {\"type\":\"response.output_text.delta\",\"delta\":\"B1:A1\"}\n";
        CHECK(gptCodexExtractText(cut) == "BLOCKS: B1:A1",
              "CODEX-SSE a cut stream salvages the accumulated deltas");
        // CRLF line endings (curl hands the wire bytes through untouched).
        string crlf =
            "data: {\"type\":\"response.output_text.done\",\"text\":\"CHOICE: 0 (pass)\"}\r\n";
        CHECK(gptCodexExtractText(crlf) == "CHOICE: 0 (pass)",
              "CODEX-SSE CRLF-terminated data lines parse");
        // Future-proofing: a completed event that DOES carry output is honored
        // when no output_text events exist.
        string comp =
            "data: {\"type\":\"response.completed\",\"response\":{\"output\":["
            "{\"type\":\"reasoning\",\"summary\":[]},"
            "{\"type\":\"message\",\"role\":\"assistant\",\"content\":["
            "{\"type\":\"output_text\",\"text\":\"ATTACK: A1\"}]}]}}\n";
        CHECK(gptCodexExtractText(comp) == "ATTACK: A1",
              "CODEX-SSE completed-event output is the fallback extraction");
        // NEGATIVES: an HTML body (web tier / wrong route) and garbage data
        // lines yield an EMPTY string - the seams' transport-failure shape -
        // never a fabricated answer.
        CHECK(gptCodexExtractText("<html><body>Just a moment...</body></html>").empty(),
              "CODEX-SSE an HTML body extracts to empty (falls back to the heuristic)");
        CHECK(gptCodexExtractText("data: {not json at all\ndata: 42\n").empty(),
              "CODEX-SSE unparseable data lines are skipped, not answers");
    }

    // ==== WAVE-34 step-1 (decision-flow lane) cases ====

    // ---- #1b(A): native reasoning capture, BOTH server paths ----
    cout << "\n[W34-1b] the thinking block is captured, and never parsed as an answer\n";
    {
        // POSITIVE: an inline block the server did not parse out. The block
        // leaves `content`, the answer stays, and the reasoning is kept whole.
        string c = "<think>Option 2 trades badly, so option 1.</think>CHOICE: 1 (Cast Mordor Muster)";
        string r;
        bool ok = splitReasoningBlock(c, r);
        cout << "     inline think -> content=\"" << c << "\" reasoning=\"" << r << "\"\n";
        CHECK(ok && c == "CHOICE: 1 (Cast Mordor Muster)",
              "W34-1b an inline <think> block is stripped off the answer");
        CHECK(r == "Option 2 trades badly, so option 1.",
              "W34-1b the stripped thinking is captured verbatim for the translog");
        // The opener eaten by the chat template (a real vLLM shape): the text
        // before </think> is still reasoning, not an answer.
        string c2 = "I should hold up mana here.</think>CHOICE: 0 (pass)";
        string r2;
        CHECK(splitReasoningBlock(c2, r2) && c2 == "CHOICE: 0 (pass)"
              && r2 == "I should hold up mana here.",
              "W34-1b a closing tag with no opener still splits reasoning from answer");
        // NEGATIVE: an ordinary reply is byte-untouched and captures nothing.
        string c3 = "CHOICE: 2 (Cast Ichor Rats)\nPLAN: race on poison.";
        string r3;
        CHECK(splitReasoningBlock(c3, r3) && c3 == "CHOICE: 2 (Cast Ichor Rats)\nPLAN: race on poison."
              && r3.empty(),
              "W34-1b NEGATIVE a reply with no thinking block is passed through unchanged");
        // UNCLOSED: reasoning-only. The answer-shaped text INSIDE the thinking
        // must not survive as an answer - this is the budget-truncation shape,
        // and its stray integers are exactly what the index scanners latch on.
        string c4 = "<think>Maybe CHOICE: 3 (Cast Emrakul)? No, wait, 15 mana. Let me count again";
        string r4;
        bool ok4 = splitReasoningBlock(c4, r4);
        CHECK(!ok4 && c4.empty(),
              "W34-1b an UNCLOSED <think> is reasoning-only: the content is emptied, never parsed");
        CHECK(r4.find("Cast Emrakul") != string::npos,
              "W34-1b the truncated thinking is still captured (it is the forced close's prefill)");
        CHECK(!hasCodedAnswerLine(c4),
              "W34-1b NEGATIVE no coded answer survives an unclosed think block");
        // Every length/answer meter runs on the STRIPPED text: a PLAN: and a
        // CHOICE: inside the thinking must not score as reply text.
        string c5 = "<think>PLAN: ignore me, and lots of hidden deliberation</think>CHOICE: 1\nPLAN: go.";
        string r5;
        splitReasoningBlock(c5, r5);
        CHECK(postPlanOverrun(c5) == 0 && hasCodedAnswerLine(c5),
              "W34-1b the overrun/answer scanners measure the STRIPPED reply, not the thinking");
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
