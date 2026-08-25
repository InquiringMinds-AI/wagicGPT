/*
 *  Wagic, The Homebrew ?! is licensed under the BSD license
 *  See LICENSE in the Folder's root
 *
 *  Item-1 (wave-16): the AIPlayerGPT per-decision prompt carries the model's
 *  last "YOUR PLAN" prose forward to preserve intent across decisions. When
 *  the game state has ADVANCED past that plan - a card it planned to cast is
 *  already cast, the menu/phase changed - the carried plan re-injects STALE
 *  intent: the model executes a leftover plan against the wrong option
 *  (deck110-vs-deck21 20260716: after casting Steel Overseer the carried plan
 *  still said "cast Steel Overseer", so the model emitted a Galvanic Blast and
 *  name-matched its OWN just-cast Overseer as the target, a self-2-for-1).
 *
 *  This is the pure string test behind the fix, factored out of AIPlayerGPT so
 *  it can be exercised directly against captured translog prompts. It returns
 *  true when the plan AFFIRMATIVELY commits to acting on one or more named
 *  cards AND none of those cards appear among the current options - i.e. the
 *  plan's named actions have all fallen off the menu. The caller then appends
 *  a light-touch caveat (re-derive from the board) rather than dropping the
 *  carry, so plans that legitimately look ahead to a FUTURE action are only
 *  nudged, never silenced.
 */

#ifndef _GPT_PLAN_CAVEAT_H_
#define _GPT_PLAN_CAVEAT_H_

#include <string>
#include <vector>
#include <set>
#include <cctype>

namespace gptcaveat {

inline std::string toLower(const std::string& s)
{
    std::string r = s;
    for (size_t i = 0; i < r.size(); i++)
        r[i] = (char) std::tolower((unsigned char) r[i]);
    return r;
}

// A negation/deferral word anywhere in the [pre-window .. cardname] span flips
// an action from a live commitment ("I will cast X") into a non-commitment
// ("I will NOT cast X", "cannot cast X", "hold X", "saving X for later").
inline bool windowNegated(const std::string& win)
{
    static const char * neg[] = {
        "not ", "n't", "never", "cannot", "can not", "instead", "rather",
        "hold ", "holding", "save ", "saving", "without", "avoid", "unable"
    };
    for (size_t i = 0; i < sizeof(neg) / sizeof(neg[0]); i++)
        if (win.find(neg[i]) != std::string::npos)
            return true;
    return false;
}

// Does an action VERB governing this card sit in the pre-window? "play " and
// "playing"/"played" carry a trailing letter/space so "player" does not match.
// WAVE-35 (churn driver #2): "attack" is NOT a cast/activation verb, and it was
// the single largest false-fire source. A plan that says "attack with Grishnakh
// next turn" is a perfectly live intent, and the ask that shows it - a declare-
// attackers ask, or a pay-life sub-ask - never lists a "Cast Grishnakh" option,
// so anyOffered was false by construction and the caveat fired on a plan that
// was entirely on track. The caveat exists for ONE failure (a plan naming a cast
// or activation that has already happened), so only cast/activation-shaped verbs
// arm it now. See also optionsAreActionMenu() below, which requires the ask to
// be the kind of ask those verbs could be answered from.
// #W44-7 (wave-43 corpus, 4/2317 fallbacks): "resolve" is a CAST commitment in
// the vocabulary plans actually use. Two of the four fallback decisions carried
// the plan "Answer their threats, resolve Staff of Nin, ping their face every
// turn" and then answered a narrow window with "CHOICE: 3 (Cast Supreme
// Verdict)" / "Counter Idyllic Tutor with Dream Fracture" - a name that was
// never on the menu. The old verb table had no word for it, so the note that
// exists for exactly this failure was silent at both sites. "resolve" governs a
// spell the plan intends to put on the stack, which is the same commitment
// "cast" makes; it is NOT the attack-shaped verb class wave-35 removed.
inline bool windowHasVerb(const std::string& win)
{
    static const char * verbs[] = {
        "cast", "activat", "equip", "play ", "playing", "played", "resolve"
    };
    for (size_t i = 0; i < sizeof(verbs) / sizeof(verbs[0]); i++)
        if (win.find(verbs[i]) != std::string::npos)
            return true;
    return false;
}

// An offered action names its own card ("Cast Steel Overseer", "Play X"), but
// option lines ALSO enumerate the cards ANOTHER spell can hit ("Cast Galvanic
// Blast - legal targets right now: Steel Overseer, ...", "can target on the
// stack: X"). A card that appears ONLY inside such an enumeration is not an
// offered action - it is another action's target - so blank those tails out
// before deciding whether the plan's named card is on the menu.
inline std::string stripTargetEnumerations(const std::string& optsLower)
{
    static const char * markers[] = { "legal targets right now:", "can target on the stack:" };
    std::string out = optsLower;
    for (size_t m = 0; m < sizeof(markers) / sizeof(markers[0]); m++)
    {
        size_t pos = 0;
        while ((pos = out.find(markers[m], pos)) != std::string::npos)
        {
            size_t eol = out.find('\n', pos);
            size_t end = (eol == std::string::npos) ? out.size() : eol;
            out.erase(pos, end - pos); //drop the enumerated names, keep the line break
        }
    }
    return out;
}

// WAVE-35 (churn driver #2): the shortest form of a card name a plan will use.
// Plans say "cast Sigarda in main phase 2"; the display name is "Sigarda,
// Champion of Light". The old full-name-only scan therefore FAILED to see the
// plan's own affirmation on comma-named cards - and, worse, failed to see the
// SAME card sitting in the option list as "Cast Sigarda, Champion of Light", so
// a plan that was being executed literally that decision could still be told its
// actions were gone (batch5 finding 1, verbatim: "Option 4 IS available... I
// will ignore the implication"). Matching on the pre-comma head fixes both
// directions at once, and it is used symmetrically (plan side AND option side)
// so it can never make the caveat fire MORE often than the full name would.
inline std::string shortName(const std::string& lowerName)
{
    size_t comma = lowerName.find(',');
    if (comma == std::string::npos || comma < 4)
        return lowerName;
    return lowerName.substr(0, comma);
}

// WAVE-35 (churn driver #2): is this ask one whose options are cast/activation
// actions at all? The caveat's whole claim - "the actions your plan names are no
// longer among the options" - is only meaningful when the options COULD have
// carried such an action. On a pay-life sub-ask ("1. Pay 3 life / 2. Don't"), a
// declare-attackers ask (A1/A2 lines) or a blocker ask, they never can, so the
// claim was false ~100% of the time it was made there. Measured: the note fired
// on 24% of all prompts (15/102 at batch5, 30/146 at batch6) and was read as a
// LEGALITY RULING - "This confirms I cannot activate Amulet" - which is the
// opposite of the truth the surface owes the model.
// #W44-7: the assembled tail is not just the option list - it also carries the
// reply-protocol boilerplate and the standing phase facts, and BOTH of those
// say "cast". "e.g. \"CHOICE: 3 (Cast Example Card)\"" and "creatures,
// sorceries and other main-phase cards you do not cast now can still be cast
// then" match the marker table on windows that offer no cast at all, so the
// wave-35 gate had been silently dead since those sentences were added: it
// returned true on EVERY priority/attackers ask. Measured on the wave-43 corpus
// (2232 prompts, note fired 762 = 34.1%): 43/43 declare-attackers fires were
// this exact false positive, plus 58 target/mode/may-trigger asks - 101 fires,
// 13% of all of them, on menus that could not have carried the plan's action.
// So scan the NUMBERED OPTION LINES only, never the prose around them.
inline std::string optionLinesOnly(const std::string& optsLower)
{
    std::string out;
    size_t pos = 0;
    while (pos <= optsLower.size())
    {
        size_t eol = optsLower.find('\n', pos);
        size_t end = (eol == std::string::npos) ? optsLower.size() : eol;
        std::string line = optsLower.substr(pos, end - pos);
        size_t i = 0;
        while (i < line.size() && (line[i] == ' ' || line[i] == '\t'))
            i++;
        size_t d = i;
        while (d < line.size() && line[d] >= '0' && line[d] <= '9')
            d++;
        if (d > i && d < line.size() && line[d] == '.')
            out += line + "\n";
        if (eol == std::string::npos)
            break;
        pos = eol + 1;
    }
    return out;
}

// True when this ask's own option ROWS are card actions - the only shape on
// which "your plan's actions are not in this list" is a meaningful claim.
// Two families qualify. (1) The named verbs, as before. (2) #W44-7: an option
// row carrying the engine's own action annotations ([cost: ...] / {card text:
// ...}) - the NARROW windows the wave-43 fallbacks actually happened in, where
// the only offers are mana taps, cycling, manland activations or a single
// loyalty ability. A plan naming a cast is genuinely absent from those, and
// they are where the pilot answered with a card that was never listed.
// Everything else stays out by construction: declare-attackers/blockers rows
// are "A1."/"B1." and produce NO numbered lines at all; pay-life sub-asks,
// mode menus, dungeon-room choices and target lists carry neither a verb nor a
// cost annotation.
inline bool optionsAreActionMenu(const std::string& optsLower)
{
    std::string lines = optionLinesOnly(optsLower);
    if (lines.empty())
        return false;
    static const char * marks[] = { "cast ", "activate ", "play land", "play ", "equip " };
    for (size_t i = 0; i < sizeof(marks) / sizeof(marks[0]); i++)
        if (lines.find(marks[i]) != std::string::npos)
            return true;
    return lines.find("[cost:") != std::string::npos
        || lines.find("{card text:") != std::string::npos;
}

// planRaw / optsRaw as assembled; myCardNames = display names from the
// caster's own zones (the vocabulary the plan can name). True when the plan
// affirmatively commits to acting on >=1 named card and NONE of those cards
// appear among the current options (as an action, not merely as another
// spell's listed target).
inline bool planActionsStale(const std::string& planRaw, const std::string& optsRaw,
                             const std::vector<std::string>& myCardNames)
{
    if (planRaw.empty())
        return false;
    std::string plan = toLower(planRaw);
    std::string opts = stripTargetEnumerations(toLower(optsRaw));
    if (!optionsAreActionMenu(opts))
        return false; //not a cast/activation menu: the caveat has nothing to claim
    bool anyAffirmative = false;
    bool anyOffered = false;
    std::set<std::string> seen;
    for (size_t n = 0; n < myCardNames.size(); n++)
    {
        std::string full = toLower(myCardNames[n]);
        if (full.size() < 4)
            continue; //skip tiny names: too much substring noise
        std::string nm = shortName(full); //wave-35: match the head, both sides
        if (nm.size() < 4)
            continue;
        if (!seen.insert(nm).second)
            continue;
        size_t pos = 0;
        bool cardAffirmed = false;
        while ((pos = plan.find(nm, pos)) != std::string::npos)
        {
            size_t ws = pos > 40 ? pos - 40 : 0;
            std::string win = plan.substr(ws, pos - ws);
            if (windowHasVerb(win) && !windowNegated(win))
            {
                cardAffirmed = true;
                break;
            }
            pos += nm.size();
        }
        if (cardAffirmed)
        {
            anyAffirmative = true;
            if (opts.find(nm) != std::string::npos || opts.find(full) != std::string::npos)
                anyOffered = true;
        }
    }
    return anyAffirmative && !anyOffered;
}

} // namespace gptcaveat

#endif
