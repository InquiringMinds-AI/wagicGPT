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
inline bool windowHasVerb(const std::string& win)
{
    static const char * verbs[] = {
        "cast", "activat", "attack", "equip", "play ", "playing", "played"
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
    bool anyAffirmative = false;
    bool anyOffered = false;
    std::set<std::string> seen;
    for (size_t n = 0; n < myCardNames.size(); n++)
    {
        std::string nm = toLower(myCardNames[n]);
        if (nm.size() < 4)
            continue; //skip tiny names: too much substring noise
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
            if (opts.find(nm) != std::string::npos)
                anyOffered = true;
        }
    }
    return anyAffirmative && !anyOffered;
}

} // namespace gptcaveat

#endif
