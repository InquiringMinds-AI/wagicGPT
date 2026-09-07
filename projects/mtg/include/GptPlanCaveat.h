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
#include <cstring>
#include <sstream>

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

// #W45-4 (wave-44 corpus, the note fired on 870/2035 = 42.8% of decisions): the
// note's claim is about a menu that COULD have carried a cast/activation of the
// plan's card. Several asks are structurally cast-free - their option rows can
// never be a cast or an activation of a card in hand, whatever words they use:
//   - the LAND DROP ask ("1. Play Island"), whose own prompt says three lines
//     above "its absence from the choices below does not mean it is gone" - the
//     note asserted the opposite of the surface it was printed under, on
//     327/419 land-drop asks;
//   - the ANNOUNCE_X ask (the spell is already being cast; the menu is a number);
//   - the MODE / sub-ask family ("Choose an option for X:", "Choose one mode
//     for X:", dungeon-room and venture menus, target lists, the pregame
//     mulligan, the forced exile/sacrifice asks). The chosen card is not being
//     cast FROM this list at all.
// Matched on the ask HEADER at a line start, which is what names the ask. The
// board frame's own "Land drop: ..." availability line is NOT in this string -
// it lives in the CURRENT SITUATION block, above the plan, never in the tail.
inline bool castFreeAskHeader(const std::string& optsLower)
{
    static const char * heads[] = {
        "land drop:", "announce the value of x for ", "choose an option for ",
        "choose one mode for ", "venture - choose a dungeon", "dungeon room choice",
        "target choice for ", "pre-game mulligan decision",
        "exile one of your own cards:", "sacrifice one of your own cards:",
        "a choice is required - choose an option:"
    };
    size_t pos = 0;
    while (pos <= optsLower.size())
    {
        size_t eol = optsLower.find('\n', pos);
        size_t end = (eol == std::string::npos) ? optsLower.size() : eol;
        std::string line = optsLower.substr(pos, end - pos);
        size_t i = 0;
        while (i < line.size() && (line[i] == ' ' || line[i] == '\t'))
            i++;
        for (size_t h = 0; h < sizeof(heads) / sizeof(heads[0]); h++)
            if (line.compare(i, std::strlen(heads[h]), heads[h]) == 0)
                return true;
        if (eol == std::string::npos)
            break;
        pos = eol + 1;
    }
    return false;
}

// #W45-4: the forms of a card name a PLAN will use for the card the OPTION row
// spells out in full. Plans write "cast Acererak"; the row reads "Cast Acererak
// the Archlich" - the pre-comma head (wave-35) does not reach that one, so the
// plan's own execution of the offered action was invisible and the note fired
// on a menu whose first row literally began with the plan's card (deck146
// seq 84, deck146 seq 24/72). The head before " of " / " the " covers the
// "Name the Title" and "Name of Place" shapes. Used on the PLAN side of the
// OFFERED test only, which can only SUPPRESS the note, never arm it.
inline void planNameForms(const std::string& lowerName, std::vector<std::string>& out)
{
    out.push_back(lowerName);
    size_t comma = lowerName.find(',');
    if (comma != std::string::npos && comma >= 4)
        out.push_back(lowerName.substr(0, comma));
    static const char * seps[] = { " of ", " the " };
    for (size_t s = 0; s < 2; s++)
    {
        size_t i = lowerName.find(seps[s]);
        if (i != std::string::npos && i >= 4)
            out.push_back(lowerName.substr(0, i));
    }
}

// #W45-4: a plan clause that opens with "Next turn" / "then" is a statement
// about a LATER window, not about this menu. "Cast the found Sanguine Bond this
// turn if mana allows... Next turn, cast Perimeter Captain and a wall" was read
// as a commitment to Perimeter Captain and armed the note on the very ask whose
// option 1 was "Cast Perimeter Captain" (deck126 seq 9). `pos` is where the card
// name starts in `plan`; the clause is what precedes it back to the last
// sentence break.
inline bool planClauseIsFuture(const std::string& plan, size_t pos)
{
    size_t start = plan.find_last_of(".;!?\n", pos ? pos - 1 : 0);
    start = (start == std::string::npos) ? 0 : start + 1;
    while (start < pos && (plan[start] == ' ' || plan[start] == '\t'))
        start++;
    std::string clause = plan.substr(start, pos - start);
    static const char * openers[] = { "next turn", "then ", "then,", "later",
                                      "after that", "afterward", "eventually" };
    for (size_t i = 0; i < sizeof(openers) / sizeof(openers[0]); i++)
        if (clause.compare(0, std::strlen(openers[i]), openers[i]) == 0)
            return true;
    // "...attack with everything, then cast X" - the future marker sits mid
    // sentence, inside the same window the verb was found in.
    size_t ws = clause.size() > 40 ? clause.size() - 40 : 0;
    std::string win = clause.substr(ws);
    return win.find(", then ") != std::string::npos
        || win.find("next turn") != std::string::npos
        || win.find("after that") != std::string::npos;
}

// #W49-U D7: does the carried plan name NO action at all - none of the pilot's
// own cards and none of the verbs this action menu's rows open with ("Cast",
// "Activate", "Play" ...)? Decided only on a cast/activation menu (an A-row or
// B-row list has no verbs to match, and the echo-count rule covers it there).
// "Passing" is not a row verb: the rows are numbered actions, and pass is the
// 0 the tail names. True = the plan is not about this game's actions.
inline bool planNamesNoAction(const std::string& planRaw, const std::string& optsRaw,
                              const std::vector<std::string>& myCardNames)
{
    if (planRaw.empty())
        return false;
    std::string plan = toLower(planRaw);
    std::string opts = toLower(optsRaw);
    if (!optionsAreActionMenu(opts))
        return false;
    std::set<std::string> seen;
    for (size_t n = 0; n < myCardNames.size(); n++)
    {
        std::string full = toLower(myCardNames[n]);
        if (full.size() < 4)
            continue;
        std::string nm = shortName(full);
        if (nm.size() < 4 || !seen.insert(nm).second)
            continue;
        if (plan.find(nm) != std::string::npos || plan.find(full) != std::string::npos)
            return false;
    }
    // Row verbs: the first word after "N. " on each option line.
    std::string lines = optionLinesOnly(opts);
    size_t at = 0;
    while (at < lines.size())
    {
        size_t nl = lines.find('\n', at);
        std::string line = lines.substr(at, nl == std::string::npos ? std::string::npos : nl - at);
        at = (nl == std::string::npos) ? lines.size() : nl + 1;
        size_t p = line.find_first_not_of(" \t");
        if (p == std::string::npos || !isdigit((unsigned char) line[p]))
            continue;
        size_t dot = line.find('.', p);
        if (dot == std::string::npos)
            continue;
        size_t vs = line.find_first_not_of(" \t", dot + 1);
        if (vs == std::string::npos)
            continue;
        size_t ve = line.find_first_of(" \t", vs);
        std::string verb = line.substr(vs, ve == std::string::npos ? std::string::npos : ve - vs);
        if (verb.size() >= 3 && plan.find(verb) != std::string::npos)
            return false;
    }
    return true;
}

// planRaw / optsRaw as assembled; myCardNames = display names from the
// caster's own zones (the vocabulary the plan can name). True when the plan
// affirmatively commits, FOR THIS WINDOW, to acting on >=1 named card and the
// menu names none of the cards the plan names (as an action, not merely as
// another spell's listed target). #W45-4 fixed the asymmetry that made the
// note assert a falsehood about the list under it: what SILENCES the note is
// now decided from the rendered option rows' card NAMES alone, with no verb
// and no negation test, so any card the plan names and the menu offers is
// enough - while what ARMS it still needs a cast/activation verb, unnegated,
// in a clause about THIS turn.
inline bool planActionsStale(const std::string& planRaw, const std::string& optsRaw,
                             const std::vector<std::string>& myCardNames)
{
    if (planRaw.empty())
        return false;
    std::string plan = toLower(planRaw);
    std::string opts = stripTargetEnumerations(toLower(optsRaw));
    if (castFreeAskHeader(opts))
        return false; //#W45-4: this ask's rows are structurally not casts
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
        //#W45-4, the OFFERED half, decided FIRST and independently of any verb.
        //The note's claim is falsified the moment the plan names a card this
        //menu offers - however the plan phrases it ("I will not cast Tribute to
        //Hunger yet", "Next turn, cast Perimeter Captain", "cast Acererak").
        //The old test only looked at cards it had already accepted as verb-
        //affirmed, so a plan whose wording missed the verb table left the note
        //asserting that a row it could read verbatim was not there: 5 strictly
        //verified false fires, plus the 52/193 verb-mismatch subclass at
        //deck123 ("Tap Lord of Lineage" vs "Create vampire with Lord of
        //Lineage #1"). Matching NAMES off the rendered rows, not verbs.
        bool onMenu = (opts.find(nm) != std::string::npos
                       || opts.find(full) != std::string::npos);
        if (onMenu)
        {
            std::vector<std::string> forms;
            planNameForms(full, forms);
            if (nm != full)
                planNameForms(nm, forms);
            for (size_t fI = 0; fI < forms.size(); fI++)
                if (forms[fI].size() >= 4 && plan.find(forms[fI]) != std::string::npos)
                {
                    anyOffered = true;
                    break;
                }
        }
        size_t pos = 0;
        bool cardAffirmed = false;
        while ((pos = plan.find(nm, pos)) != std::string::npos)
        {
            size_t ws = pos > 40 ? pos - 40 : 0;
            std::string win = plan.substr(ws, pos - ws);
            //#W45-4: a clause about a LATER turn is not a claim about this menu.
            if (windowHasVerb(win) && !windowNegated(win) && !planClauseIsFuture(plan, pos))
            {
                cardAffirmed = true;
                break;
            }
            pos += nm.size();
        }
        if (cardAffirmed)
            anyAffirmative = true;
    }
    return anyAffirmative && !anyOffered;
}

// #W54-A (D12b, wave-53 ledger MED = R172): the one-clause DIFF. planActionsStale
// only fires when EVERY affirmed card has fallen off the menu, so a plan that
// names two cards and keeps one of them says nothing about the one that is gone
// (162v152 s11 -> s12: the plan named `Cast Master of the Feast` and the row the
// same reply took made Master unaffordable; s12 was served that plan verbatim).
// This returns the FIRST card the plan affirmatively commits to acting on THIS
// window that the current option list does not carry, so the header can name it.
// Same affirmation test as planActionsStale - a negated clause ("hold X"), a
// future clause ("next turn, cast X") and a non-action menu never fire.
inline bool planAbsentActionName(const std::string& planRaw, const std::string& optsRaw,
                                 const std::vector<std::string>& myCardNames,
                                 std::string& absentOut)
{
    absentOut.clear();
    if (planRaw.empty())
        return false;
    std::string plan = toLower(planRaw);
    std::string opts = stripTargetEnumerations(toLower(optsRaw));
    if (castFreeAskHeader(opts))
        return false;
    if (!optionsAreActionMenu(opts))
        return false;
    std::set<std::string> seen;
    for (size_t n = 0; n < myCardNames.size(); n++)
    {
        std::string full = toLower(myCardNames[n]);
        if (full.size() < 4)
            continue;
        std::string nm = shortName(full);
        if (nm.size() < 4)
            continue;
        if (!seen.insert(nm).second)
            continue;
        if (opts.find(nm) != std::string::npos || opts.find(full) != std::string::npos)
            continue; //this window offers it: nothing to say
        size_t pos = 0;
        while ((pos = plan.find(nm, pos)) != std::string::npos)
        {
            size_t ws = pos > 40 ? pos - 40 : 0;
            std::string win = plan.substr(ws, pos - ws);
            if (windowHasVerb(win) && !windowNegated(win) && !planClauseIsFuture(plan, pos))
            {
                absentOut = myCardNames[n];
                return true;
            }
            pos += nm.size();
        }
    }
    return false;
}

// #W50-Y D10 (wave-49 ledger MED): a plan that OPENS with a verdict is not a
// plan. deck123 vs125 seq 53-56 and deck130 vs125 seq 125 carried "The game is
// lost. ..." - text that goes on to name cards, so planNamesNoAction could not
// fire - and under it the pilot aimed at itself. The verdict sits in the FIRST
// sentence (or the first 80 chars when there is no sentence break): a resigned
// clause later in an otherwise action-naming plan ("cast X; if that fails the
// game is lost") is a contingency, not the plan's headline, and is kept.
// Content-keyed, never count-keyed: the raw echo count is a report field only
// (a verbatim echo of a CORRECT plan - deck152 vs126 s31-41's loop-lockout
// "ATTACK: none", deck125 vs126 s69-96's "ping their face" - is the model
// holding a line it should hold, and a count trigger expired both).
inline bool planOpensWithVerdict(const std::string& planRaw)
{
    if (planRaw.empty())
        return false;
    std::string plan = toLower(planRaw);
    size_t start = plan.find_first_not_of(" \t\r\n\"'*");
    if (start == std::string::npos)
        return false;
    size_t brk = plan.find_first_of(".!?;:\n", start);
    size_t end = (brk == std::string::npos) ? plan.size() : brk;
    if (end - start > 80)
        end = start + 80;
    std::string head = plan.substr(start, end - start);
    static const char * verdicts[] = {
        "the game is lost", "game is lost", "this game is lost", "the game is over",
        "game over", "we have lost", "i have lost", "we lose", "i lose", "we are dead",
        "i am dead", "no way to win", "cannot win", "can't win", "can not win",
        "no outs", "nothing to do", "nothing i can do", "nothing we can do",
        "nothing can be done", "no play", "no legal action", "no action",
        "no meaningful", "no relevant", "concede", "conceding", "give up", "resign",
        "it is over", "it's over", "we are lost", "i am lost", "lost cause",
        "no hope", "hopeless", "unwinnable"
    };
    for (size_t i = 0; i < sizeof(verdicts) / sizeof(verdicts[0]); i++)
        if (head.find(verdicts[i]) != std::string::npos)
            return true;
    return false;
}

// #W50-Y D10 (ii): the carried plan names a TARGET this target window does
// not list. deck130 G4: both self-hits (D6) carried a plan naming a target
// the window did not offer, and the pilot aimed at the row it did offer.
// `boardNames` are the permanents on BOTH battlefields (what a plan can aim
// at); a name counts as the plan's target only when a targeting verb sits in
// the 40 chars before it ("kill X", "bolt X", "target X", "aim at X", "exile
// X", "equip X" ...) - a card the plan merely mentions is not a target claim,
// and the note must never say something false about the list under it. True
// when >= 1 such target is named and NONE of them appears anywhere in the
// window (rows AND header - the header's own spell name never arms it).
inline bool planTargetAbsent(const std::string& planRaw, const std::string& optsRaw,
                             const std::vector<std::string>& boardNames)
{
    if (planRaw.empty() || boardNames.empty())
        return false;
    std::string plan = toLower(planRaw);
    std::string opts = toLower(optsRaw);
    static const char * verbs[] = {
        "target", "kill", "destroy", "exile", "remove", "bounce", "burn", "shoot",
        "shock", "bolt", "hit ", "aim", "point", "fire", " at ", "equip", "attach",
        "enchant", "tap down", "sacrifice their", "edict", "counter"
    };
    bool anyTarget = false;
    std::set<std::string> seen;
    for (size_t n = 0; n < boardNames.size(); n++)
    {
        std::string full = toLower(boardNames[n]);
        if (full.size() < 4)
            continue;
        std::string nm = shortName(full);
        if (nm.size() < 4 || !seen.insert(nm).second)
            continue;
        size_t pos = 0;
        bool targeted = false;
        while ((pos = plan.find(nm, pos)) != std::string::npos && !targeted)
        {
            size_t ws = pos > 40 ? pos - 40 : 0;
            std::string win = plan.substr(ws, pos - ws);
            for (size_t v = 0; v < sizeof(verbs) / sizeof(verbs[0]) && !targeted; v++)
                targeted = (win.find(verbs[v]) != std::string::npos);
            pos += nm.size();
        }
        if (!targeted)
            continue;
        anyTarget = true;
        if (opts.find(nm) != std::string::npos || opts.find(full) != std::string::npos)
            return false; //a named target IS on this window
    }
    return anyTarget;
}

// #W60-M (B13a). THE BOUND. The carried plan is the one part of the prompt the
// engine copies back to the model verbatim and unvetted, so its size is a
// direct decision-value-per-token cost and its content competes with the true
// surfaces beside it. Wave 59, one seat (126v125): 410 plan echoes, 148 over
// 400 characters, longest 1,599, and 46 carrying live deliberation markers.
// s48's 1,236-character stream ("...Wait, I have Sorin. ... No Sanguine Bond.
// No Exquisite Blood...") produced `CHOICE: 4` on a THREE-row menu, a
// named_row_reask and two extra model calls (137.0 s).
//
// The bound is the plan's FIRST PARAGRAPH (planParagraphBound, #W54-A, already
// applied at the emit site) and then `maxChars`, cut at the last sentence end
// at or below it. Nothing is silently dropped: a plan that was cut carries the
// marker, so the model can see that its own words were shortened and restate
// them. `maxChars < 1` disables the length bound and returns the plan as given.
// #W63-AD (E14, deck126 HIGH-1 second half). THE PLAN CARRY IS A SCRATCHPAD.
// `...deck126-vs-deck146` seq 36 carried, as YOUR PLAN, "With both Exquisite
// Blood and Sanguine Bond on the battlefield (Exquisite Blood is in hand,
// Sanguine Bond was exiled but wait-Sanguine Bond was exiled on turn 11. Let's
// re-read carefully." - a deliberation stream, quoted back as a stated belief.
// planParagraphBound keeps the FIRST PARAGRAPH, and a model that argues with
// itself writes the whole argument in one paragraph, so the shape survives it.
// The cut here is the point where the paragraph stops being a plan: the first
// SENTENCE that opens with a self-correction marker, and only when at least one
// complete sentence precedes it (so a plan that merely begins "Actually, cast
// the Bond" is never cut to nothing). Pure over the plan text; the size of the
// cut is stated by the caller through planTruncationNote, exactly as the length
// bound states its own. Nothing the model relies on is deleted - the model is
// told how much of what it wrote was dropped and asked to restate it.
//#W66-AR (MED, deck126; repro 126v125 seq 45-57). THE PLAN NAMES A CARD IN NO
//VISIBLE ZONE. planActionsStale is a claim about THIS MENU and it is silenced by
//two things the repro had: the plan's clause was about a LATER turn
//(planClauseIsFuture), and the card was in the seat's own vocabulary because
//that vocabulary includes the LIBRARY. So "On my next turn, cast Idyllic Tutor
//to find Exquisite Blood" was carried verbatim for fifteen turns while no card
//of that name was in the hand, on either battlefield, or in any graveyard - the
//model planned around a card it could not reach and never drew.
//This is a different claim from planActionsStale's and it is made separately: a
//card the plan affirmatively acts on is in the LIBRARY only. `libraryNames` is
//the seat's library, `visibleNames` every zone the prompt renders (both hands it
//can see, both battlefields, both graveyards, both exiles). FUTURE clauses count
//here - a plan for next turn is exactly the shape that goes stale unseen - but a
//NEGATED clause ("do not cast X") never does. Returns the first such name.
inline bool planNamesLibraryOnlyCard(const std::string& planRaw,
                                     const std::vector<std::string>& libraryNames,
                                     const std::vector<std::string>& visibleNames,
                                     std::string * outName)
{
    if (planRaw.empty() || libraryNames.empty())
        return false;
    const std::string plan = toLower(planRaw);
    std::set<std::string> visible;
    for (size_t v = 0; v < visibleNames.size(); v++)
    {
        const std::string full = toLower(visibleNames[v]);
        visible.insert(full);
        visible.insert(shortName(full));
    }
    std::set<std::string> seen;
    for (size_t n = 0; n < libraryNames.size(); n++)
    {
        const std::string full = toLower(libraryNames[n]);
        if (full.size() < 4)
            continue;
        const std::string nm = shortName(full);
        if (nm.size() < 4 || !seen.insert(nm).second)
            continue;
        if (visible.count(nm) || visible.count(full))
            continue; //a copy IS somewhere the model can see: nothing to say
        size_t pos = 0;
        while ((pos = plan.find(nm, pos)) != std::string::npos)
        {
            const size_t ws = pos > 40 ? pos - 40 : 0;
            const std::string win = plan.substr(ws, pos - ws);
            if (windowHasVerb(win) && !windowNegated(win))
            {
                if (outName)
                    *outName = libraryNames[n];
                return true;
            }
            pos += nm.size();
        }
    }
    return false;
}

inline std::string planScratchpadCut(const std::string& plan)
{
    if (plan.size() < 2)
        return plan;
    // #W63-AF (R4, wave-63 codex review finding 4). THE MARKERS ARE CORRECTION
    // SHAPES, NOT WORDS THE MODEL MIGHT ALSO USE AS A VERB. "wait", "hold on",
    // "hang on", "let me" and "let's" are all OPERATIVE in a plan - "Wait until
    // their end step, then cast it", "Hold on to the removal", "Let's attack
    // with everything" - and the bare-word list cut exactly those sentences
    // out, deleting the timing instruction the plan existed to carry. Every
    // marker below is a shape that can only be the model arguing with itself:
    // an interjection followed by a comma ("Wait, ...", "No, ..."), an explicit
    // correction word ("Actually", "Correction", "Scratch that", "On second
    // thought"), a re-reading, or a "but wait". "Wait, I have Sorin" (the
    // #W60-M B13a repro) and "...but wait-Sanguine Bond was exiled" (the
    // #W63-AD E14 repro) both still cut; "Wait until their end step" does not.
    static const char * kMarkers[] = {
        "wait,", "wait -", "wait no", "but wait", "hold on,", "hang on,",
        "hmm", "let me re", "let's re", "lets re",
        "actually", "on second thought", "scratch that", "correction",
        "re-read", "reread", "i need to re", "no,"
    };
    const std::string low = toLower(plan);
    size_t i = 0;
    bool sentenceSeen = false;
    while (i < low.size())
    {
        // advance to the start of the next sentence
        size_t stop = low.find_first_of(".!?", i);
        if (stop == std::string::npos)
            break;
        sentenceSeen = true;
        size_t s = stop + 1;
        // #W64-AJ (E14a, engine seat LOW-1): the guard skipped whitespace and
        // quotes only, so a self-correction that OPENS A BRACKET was not at a
        // sentence start by this walk and survived the cut - `130v126@1788653538`
        // seq 35 carried "... Cast nothing right now. (Wait, the choice is to
        // cast Starstorm." into the served plan (1 of 2,801). The opener is
        // punctuation the model wraps an aside in; it is not part of the marker
        // and it is not a word, so it is skipped exactly as a quote is. The
        // marker list itself is unchanged: "Wait until their end step" still has
        // no comma and still does not cut.
        while (s < low.size() && (low[s] == ' ' || low[s] == '\t' || low[s] == '\r'
                                  || low[s] == '\n' || low[s] == '"' || low[s] == '\''
                                  || low[s] == '(' || low[s] == '[' || low[s] == '*'
                                  || low[s] == '`'))
            s++;
        if (s >= low.size())
            break;
        if (sentenceSeen)
        {
            for (size_t k = 0; k < sizeof(kMarkers) / sizeof(kMarkers[0]); k++)
            {
                const size_t len = std::strlen(kMarkers[k]);
                if (low.compare(s, len, kMarkers[k]) != 0)
                    continue;
                const size_t after = s + len;
                // word boundary: the marker must not be the head of a longer word
                if (after < low.size() && (std::isalnum((unsigned char) low[after])
                                           || low[after] == '\''))
                    continue;
                std::string out = plan.substr(0, stop + 1);
                size_t e = out.find_last_not_of(" \t\r\n");
                return (e == std::string::npos) ? plan : out.substr(0, e + 1);
            }
        }
        i = s;
    }
    return plan;
}

//#W70-BM (E1): `correctionHeaderCue`, `answerHeadingSkip` and
//`textHasCorrectionHeader` were DELETED here. All three existed only to
//tolerate in-band reasoning in the reply - an announced retraction written
//between two answer lines, and a heading word ("ANSWER:", "CORRECTION:")
//standing before the label. Under invariant 000 the reply is a PLAN line and
//an action line and nothing else, so a heading before the label and a second
//answer line are both protocol VIOLATIONS, not shapes to salvage: tolerating
//them is what teaches the model to write them.
inline const char * planTruncationMarker()
{
    return " [...the rest of your plan was not carried - restate it if you still mean it]";
}

// #W62-Z (D16, deck152 MED-1). G3 seq 115 wrote ~3 KB of deliberation into its
// PLAN line; the bound above cut it once and marked the cut, and the surviving
// fragment - "...We can animate it again?" - was then quoted back as YOUR PLAN
// at seq 120 and 121, five and six windows later. The bound is not the gap: the
// marker is, because it says the rest "was not carried" and nothing says HOW
// MUCH, so a fragment reads like a plan that merely ended. State the size of the
// cut, which is the one fact that tells the model its PLAN line was a
// deliberation stream rather than a plan. Pure over (kept, original) so the
// composed shape is provable; a plan the bound did not touch is byte-identical.
//#W70-BM (E1): the correction clause is DELETED with correctionHeaderCue - a
//retraction inside a PLAN line is in-band reasoning, which the protocol no
//longer permits and this note must no longer describe as normal. What replaces
//it is the fact a SEQUENCE has: how many later STEPS the cut dropped. A plan is
//"swing all, trick after blockers, Rhino in main 2" - dropping step 3 silently
//is the failure this number exists to make impossible.
inline std::string planTruncationNote(size_t keptChars, size_t originalChars,
                                      size_t droppedSteps = 0)
{
    // The wave-60 literal is kept verbatim as the head - deck guides and the
    // shipped PARSETEST pins read it - and the measured size rides after it.
    std::string head = " [...the rest of your plan was not carried";
    std::ostringstream o;
    o << head;
    if (originalChars > keptChars)
        o << ": " << (originalChars - keptChars) << " further characters, of "
          << originalChars << " you wrote";
    if (droppedSteps > 0)
        o << ", and " << droppedSteps << (droppedSteps == 1 ? " later step" : " later steps")
          << " of the sequence";
    o << " - restate it in a sentence or two if you still mean it]";
    return o.str();
}

// #W65-AP (R7, wave-65 codex review finding 7). THE PROTOCOL SAYS CHARACTERS.
// kReplyProtocol and #W65-AO's pin both claim "the FIRST 400 CHARACTERS of your
// PLAN line", and the bound was over std::string::size() - BYTES - so a plan
// carrying accented card names or non-Latin text was cut early and could end
// mid-sequence, leaving invalid UTF-8 in a line the model is asked to trust.
// Counted in code points, the claim is true and the cut can never land inside a
// character. ASCII is one byte per code point, so every shipped case and every
// ASCII plan is byte-identical.
inline size_t utf8Length(const std::string& s)
{
    size_t n = 0;
    for (size_t i = 0; i < s.size(); i++)
        if ((static_cast<unsigned char>(s[i]) & 0xC0) != 0x80)
            n++;
    return n;
}

// Byte offset one past the `maxChars`-th code point, or s.size() when the
// string has no more than that many. Always a character boundary.
inline size_t utf8ByteLimit(const std::string& s, size_t maxChars)
{
    size_t n = 0, i = 0;
    while (i < s.size())
    {
        if ((static_cast<unsigned char>(s[i]) & 0xC0) != 0x80)
        {
            if (n == maxChars)
                return i;
            n++;
        }
        i++;
    }
    return s.size();
}

inline std::string planCarryBound(const std::string& plan, size_t maxChars)
{
    if (maxChars < 1)
        return plan;
    // #W65-AP (R7): the bound is denominated in CODE POINTS, and the byte offset
    // it resolves to is a character boundary.
    const size_t limit = utf8ByteLimit(plan, maxChars);
    if (limit >= plan.size())
        return plan;
    // Prefer a sentence boundary; a mid-sentence stump re-served every decision
    // reads like an instruction fragment. Only accept one past the halfway mark,
    // so a plan whose first sentence is itself longer than the bound is cut at
    // the bound rather than back to almost nothing.
    size_t cut = plan.find_last_of(".!?", limit);
    if (cut != std::string::npos && cut + 1 > limit / 2)
        cut = cut + 1;
    else
    {
        // Otherwise cut at the last word break at or below the bound.
        cut = plan.find_last_of(" \t\n", limit);
        if (cut == std::string::npos || cut < limit / 2)
            cut = limit; // #W65-AP (R7): a character boundary, never mid-sequence
    }
    std::string out = plan.substr(0, cut);
    size_t e = out.find_last_not_of(" \t\r\n");
    out = (e == std::string::npos) ? std::string() : out.substr(0, e + 1);
    if (out.empty())
        return plan; // nothing survived the cut: carry the plan as it stands
    //#W65-AP (R7): the note counts what the protocol counts - characters.
    //#W70-BM (E1): the retraction clause is gone with its predicate.
    return out + planTruncationNote(utf8Length(out), utf8Length(plan)); //#W62-Z (D16)
}

// #W60-Q (R8). THE COMPOSED PATH. planCarryBound's marker ends in ']', and the
// caller's next pass - a stump trim for a max_tokens cut - reads any tail that
// is not '.', '!' or '?' as an unfinished sentence and truncates back through
// the last terminator past the midpoint. On a bounded plan that terminator is
// inside the carried text, so the trim deleted the marker the bound had just
// promised: the model was shortened and never told. Composed here so the pair
// is one testable function rather than two correct halves in the wrong order.
// A plan the bound did not touch keeps the trim exactly as it was.
// #W63-AD (E14): the SCRATCHPAD cut runs FIRST, inside the same composition, for
// the reason #W60-Q wrote this function - two correct halves in the wrong order
// delete each other's marker. Order and denominators: the scratchpad cut chooses
// the text, the length bound then bounds THAT text, and whichever fired last the
// single marker states the drop against the number of characters the model
// actually wrote. A plan neither pass touches is byte-identical.
inline std::string planCarryCompose(const std::string& plan, size_t maxChars)
{
    const std::string core = planScratchpadCut(plan);
    std::string out = planCarryBound(core, maxChars);
    if (out != core)
    {
        if (core.size() == plan.size())
            return out; // only the length bound fired: the wave-62 shape, unchanged
        // Both fired. planCarryBound's note is denominated in `core`; restate it
        // against the plan the model wrote, so no number on the page is false.
        const std::string head = " [...the rest of your plan was not carried";
        const size_t m = out.rfind(head);
        const std::string kept = (m == std::string::npos) ? out : out.substr(0, m);
        return kept + planTruncationNote(utf8Length(kept), utf8Length(plan)); //#W65-AP (R7)
    }
    if (core.size() < plan.size())
        return core + planTruncationNote(utf8Length(core), utf8Length(plan)); //#W65-AP (R7)
    char last = out.empty() ? '.' : out[out.size() - 1];
    if (last != '.' && last != '!' && last != '?')
    {
        size_t dot = out.find_last_of(".!?");
        if (dot != std::string::npos && dot > out.size() / 2)
            out = out.substr(0, dot + 1);
    }
    return out;
}

//#W70-BM (E1). THE PLAN IS A SEQUENCE OF STEPS (invariant 000(e)): "swing all,
//use the combat trick after blockers are declared, then play the creature in
//second main". Three facts follow from that and none of them held before:
//  (1) a BYTE bound may cut inside step 2 and delete step 3 without saying so.
//      The carry is bounded by STEPS here, with a byte ceiling raised to a
//      sequence-sized number and cut only AT a step boundary, so the model is
//      never served half a step and never silently loses a later one.
//  (2) the action line is STEP ONE, so when the action executes step one is
//      CONSUMED - the next window is served the REMAINDER (planStepsAfter).
//  (3) the steps must be found without changing a byte of the plan: these
//      helpers return OFFSETS into the plan, so every join is the original text.
//A step ends at ';' ',' '.' '!' '?' or a newline that is followed by whitespace
//or the end of the string - so "1,000" and "3.5" do not split a step - and the
//last step needs no terminator. Pure over one string.
inline std::vector<size_t> planStepEnds(const std::string& plan)
{
    std::vector<size_t> ends;
    for (size_t i = 0; i < plan.size(); i++)
    {
        const char c = plan[i];
        if (c != ';' && c != ',' && c != '.' && c != '!' && c != '?' && c != '\n')
            continue;
        if (c != '\n' && i + 1 < plan.size()
            && !std::isspace((unsigned char) plan[i + 1]))
            continue; //"1,000" / "3.5": not a step boundary
        const size_t st = plan.find_first_not_of(" \t\r\n;,.!?");
        if (st == std::string::npos || i < st)
            continue; //nothing but punctuation so far
        ends.push_back(i + 1);
    }
    if (ends.empty() || ends.back() < plan.size())
    {
        const size_t last = plan.find_last_not_of(" \t\r\n");
        if (last != std::string::npos && (ends.empty() || ends.back() <= last))
            ends.push_back(last + 1);
    }
    return ends;
}

inline size_t planStepCount(const std::string& plan)
{
    return planStepEnds(plan).size();
}

//The plan from step `done` (0-based) onward, trimmed. `done` is CLAMPED to the
//last step: a fully-executed plan still carries its final step rather than
//vanishing, because a blank carry is indistinguishable from "no plan yet" and
//the model would be asked for a plan it already gave.
inline std::string planStepsAfter(const std::string& plan, size_t done)
{
    if (done == 0)
        return plan;
    const std::vector<size_t> ends = planStepEnds(plan);
    if (ends.size() <= 1)
        return plan;
    if (done > ends.size() - 1)
        done = ends.size() - 1;
    size_t at = ends[done - 1];
    while (at < plan.size() && (std::isspace((unsigned char) plan[at])
                                || plan[at] == ';' || plan[at] == ','))
        at++;
    if (at >= plan.size())
        return plan;
    return plan.substr(at);
}

//The carry bound, denominated in STEPS with a byte ceiling that only ever cuts
//AT a step end. Returns the plan untouched when it fits both; otherwise the
//kept steps plus a note that says how many characters AND how many steps the
//cut dropped.
inline std::string planStepBound(const std::string& plan, size_t maxSteps, size_t hardChars)
{
    const std::vector<size_t> ends = planStepEnds(plan);
    if (ends.empty())
        return plan;
    if (ends.size() <= maxSteps && utf8Length(plan) <= hardChars)
        return plan;
    size_t keepEnd = 0, kept = 0;
    for (size_t i = 0; i < ends.size() && i < maxSteps; i++)
    {
        if (utf8Length(plan.substr(0, ends[i])) > hardChars && kept > 0)
            break;
        keepEnd = ends[i];
        kept = i + 1;
    }
    if (kept == 0 || keepEnd >= plan.size())
        return plan;
    std::string out = plan.substr(0, keepEnd);
    const size_t e = out.find_last_not_of(" \t\r\n");
    out = (e == std::string::npos) ? std::string() : out.substr(0, e + 1);
    if (out.empty())
        return plan;
    return out + planTruncationNote(utf8Length(out), utf8Length(plan), ends.size() - kept);
}

//#W70-BM (E1): the composed carry for a SEQUENCE - the scratchpad cut first (it
//chooses the text), then the step bound (it bounds THAT text). Same order and
//same single-marker discipline as planCarryCompose, which stays for the callers
//that bound a paragraph rather than a sequence.
inline std::string planCarryComposeSteps(const std::string& plan, size_t maxSteps,
                                         size_t hardChars)
{
    const std::string core = planScratchpadCut(plan);
    std::string out = planStepBound(core, maxSteps, hardChars);
    if (core.size() < plan.size())
    {
        const std::string head = " [...the rest of your plan was not carried";
        const size_t m = out.rfind(head);
        const std::string keptText = (m == std::string::npos) ? out : out.substr(0, m);
        const size_t coreSteps = planStepCount(core), keptSteps = planStepCount(keptText);
        return keptText + planTruncationNote(utf8Length(keptText), utf8Length(plan),
                                             (coreSteps > keptSteps) ? coreSteps - keptSteps : 0);
    }
    return out;
}

// #W60-M (B13a). THE CONTRADICTION. A plan that DENIES a permanent the pilot's
// own battlefield line prints is not a stale plan, it is a false one - and the
// trust doctrine makes it worse than useless, because the model is instructed
// to believe what the prompt says and this prompt then says two things.
// 126v125 s48 carried "No Sanguine Bond. No Exquisite Blood." into a window
// whose battlefield line ended `...; Exquisite Blood {4}{b} [enchantment]...`.
//
// Detected NARROWLY, on the shape that was observed: a bare negation
// immediately governing the permanent's name ("no X", "not have X", "don't have
// X", "lack X") or a copular denial after it ("X is not on the battlefield").
// Two things keep it from firing on a TRUE sentence: a qualifier between the
// negation and the name ("no SECOND Sanguine Bond") does not match at all, and
// a zone qualifier following the name ("no Sanguine Bond IN HAND") is a
// different, true claim and is excluded explicitly. `inPlayNames` is the
// PILOT'S OWN battlefield only - a denial about the opponent's board is not
// contradicted by the pilot's.
inline bool planDeniesOwnPermanent(const std::string& planRaw,
                                   const std::vector<std::string>& inPlayNames,
                                   std::string& deniedOut)
{
    const std::string plan = toLower(planRaw);
    if (plan.empty())
        return false;
    static const char * kPre[] = { "no ", "not have ", "n't have ", "lack ", "lacks ",
                                   "without ", "missing " };
    static const char * kPost[] = { " is not on the battlefield", " isn't on the battlefield",
                                    " is not in play", " isn't in play",
                                    " is not on my battlefield", " isn't on my battlefield" };
    static const char * kZone[] = { " in hand", " in my hand", " in your hand",
                                    " in the graveyard", " in my graveyard",
                                    " in the library", " in my library", " in exile",
                                    " in my deck", " in the deck" };
    for (size_t n = 0; n < inPlayNames.size(); n++)
    {
        const std::string full = toLower(inPlayNames[n]);
        if (full.size() < 5)
            continue; // too short to name a permanent unambiguously in prose
        std::vector<std::string> forms;
        planNameForms(full, forms);
        for (size_t f = 0; f < forms.size(); f++)
        {
            const std::string& nm = forms[f];
            if (nm.size() < 5)
                continue;
            size_t pos = 0;
            while ((pos = plan.find(nm, pos)) != std::string::npos)
            {
                const size_t after = pos + nm.size();
                // the name must end here (not be a prefix of a longer word)
                bool wordEnd = (after >= plan.size()
                                || !(std::isalnum((unsigned char) plan[after])
                                     || plan[after] == '\''));
                bool zoneScoped = false;
                for (size_t z = 0; z < sizeof(kZone) / sizeof(kZone[0]) && !zoneScoped; z++)
                    zoneScoped = (plan.compare(after, std::strlen(kZone[z]), kZone[z]) == 0);
                if (wordEnd && !zoneScoped)
                {
                    for (size_t k = 0; k < sizeof(kPre) / sizeof(kPre[0]); k++)
                    {
                        const size_t len = std::strlen(kPre[k]);
                        if (pos >= len && plan.compare(pos - len, len, kPre[k]) == 0)
                        {
                            deniedOut = inPlayNames[n];
                            return true;
                        }
                    }
                    for (size_t k = 0; k < sizeof(kPost) / sizeof(kPost[0]); k++)
                        if (plan.compare(after, std::strlen(kPost[k]), kPost[k]) == 0)
                        {
                            deniedOut = inPlayNames[n];
                            return true;
                        }
                }
                pos = after;
            }
        }
    }
    return false;
}

// #W63-AD (E14). THE OPPOSITE CONTRADICTION to planDeniesOwnPermanent: the plan
// ASSERTS that a permanent is on a battlefield when no battlefield holds it.
// `...deck126-vs-deck146` seq 29 (turn 18) served "With both enchantments
// (Exquisite Blood and Sanguine Bond) on the battlefield, any life gain wins the
// game." into a window whose own battlefield line listed neither - Sanguine Bond
// was exiled on turn 11 and Exquisite Blood on turn 15 - and the seat reasoned
// from it for eight turns. The denial rule was already withdrawn on sight; the
// assertion is the same defect with the sign flipped, and is the more dangerous
// half, because it invents a board the model then plays to.
//
// Narrow by construction, the way planDeniesOwnPermanent is:
//  * the vocabulary is the pilot's OWN cards (`deckNames`), so an arbitrary
//    noun phrase never becomes a claim;
//  * `inPlayNames` is BOTH battlefields, so a permanent that is anywhere in play
//    contradicts nothing;
//  * the claim must be an explicit presence phrase - a backward window before
//    "on the battlefield" / "on my battlefield" / "in play", or a forward window
//    after "I control" / "I still control" / "I now control" - so a plan that
//    merely NAMES a card ("cast Sanguine Bond") is untouched;
//  * a negated, conditional or future clause is excluded (that is a plan, not a
//    false claim), and so is a zone-qualified mention ("in hand", "in exile").
// #W63-AF (R5, wave-63 codex review finding 5). OWNERSHIP WORDS ARE PART OF THE
// CLAIM. `inPlayNames` is BOTH battlefields, and the guard skipped any card that
// was in play ANYWHERE - so "I control Sanguine Bond" stayed carried as a true
// premise while the pilot's copy sat in exile and the OPPONENT'S copy was the
// only one on a battlefield. A claim that names an owner is only true of that
// owner's board. `myInPlayNames` / `theirInPlayNames` scope the check; a caller
// that passes neither gets the wave-62 behaviour byte for byte (both scopes
// resolve to the combined list), so no existing claim changes verdict.
inline bool planAssertsAbsentPermanent(const std::string& planRaw,
                                       const std::vector<std::string>& inPlayNames,
                                       const std::vector<std::string>& deckNames,
                                       std::string& assertedOut,
                                       const std::vector<std::string>& myInPlayNames
                                           = std::vector<std::string>(),
                                       const std::vector<std::string>& theirInPlayNames
                                           = std::vector<std::string>(),
                                       bool ownershipScoped = false)
{
    const std::string plan = toLower(planRaw);
    if (plan.empty())
        return false;
    // Each presence phrase carries the BOARD it is a claim about: 0 = either
    // battlefield (an unqualified "in play"), 1 = the pilot's own, 2 = the
    // opponent's. Only the phrase's own scope is checked against the board.
    static const char * kBack[] = { "on the battlefield", "on my battlefield",
                                    "on the board", "in play",
                                    "on their battlefield", "on their board",
                                    "on the opponent's battlefield",
                                    "on their side" };
    static const int kBackScope[] = { 0, 1, 0, 0, 2, 2, 2, 2 };
    static const char * kFwd[] = { "i control ", "i still control ", "i now control ",
                                   "we control ", "i have out ", "i already control ",
                                   "they control ", "they still control ",
                                   "the opponent controls ", "opponent controls " };
    static const int kFwdScope[] = { 1, 1, 1, 1, 1, 1, 2, 2, 2, 2 };
    static const char * kNeg[] = { " not ", "n't", " no longer", " never", " nothing",
                                   " if ", " once ", " when ", " unless ", " until ",
                                   " would ", " will ", " need ", " to get ", " resolves",
                                   " next turn", " was exiled", " were exiled", " destroyed",
                                   " gone", " lost" };
    static const char * kZone[] = { " in hand", " in my hand", " in exile", " in the graveyard",
                                    " in my graveyard", " in the library", " in my library" };
    // Which of the pilot's names are actually in play, per board. Index 0 is
    // "either battlefield"; 1 and 2 fall back to it when the caller supplied no
    // scoped list, which is what keeps an unscoped call byte-identical.
    std::vector<std::string> boards[3];
    for (size_t i = 0; i < inPlayNames.size(); i++)
        boards[0].push_back(toLower(inPlayNames[i]));
    for (size_t i = 0; i < myInPlayNames.size(); i++)
        boards[1].push_back(toLower(myInPlayNames[i]));
    for (size_t i = 0; i < theirInPlayNames.size(); i++)
        boards[2].push_back(toLower(theirInPlayNames[i]));
    //An EMPTY board is a fact, not a missing argument: the caller says so with
    //`ownershipScoped`. Inferring the fallback from emptiness would make "I
    //control X" true off the opponent's copy on exactly the board state where
    //the claim is most wrong (nothing of the pilot's own in play at all).
    if (!ownershipScoped)
    {
        boards[1] = boards[0];
        boards[2] = boards[0];
    }

    for (size_t n = 0; n < deckNames.size(); n++)
    {
        const std::string full = toLower(deckNames[n]);
        if (full.size() < 5)
            continue; // too short to name a permanent unambiguously in prose
        // Per-board presence. A card in play SOMEWHERE no longer discharges an
        // ownership-specific claim - only presence on the board the claim names
        // does (#W63-AF R5).
        bool live[3] = { false, false, false };
        for (int b = 0; b < 3; b++)
            for (size_t k = 0; k < boards[b].size() && !live[b]; k++)
                live[b] = (boards[b][k] == full);
        if (live[0] && live[1] && live[2])
            continue; // in play on every board a claim could name: nothing to say
        std::vector<std::string> forms;
        planNameForms(full, forms);
        for (size_t f = 0; f < forms.size(); f++)
        {
            const std::string& nm = forms[f];
            if (nm.size() < 5)
                continue;
            // a short form that matches a DIFFERENT permanent in play is not
            // evidence about this card (two cards can share a first word).
            bool formLive[3] = { false, false, false };
            for (int b = 0; b < 3; b++)
                for (size_t k = 0; k < boards[b].size() && !formLive[b]; k++)
                    formLive[b] = (boards[b][k].compare(0, nm.size(), nm) == 0);
            if (formLive[0] && formLive[1] && formLive[2])
                continue;
            size_t pos = 0;
            while ((pos = plan.find(nm, pos)) != std::string::npos)
            {
                const size_t after = pos + nm.size();
                const bool wordEnd = (after >= plan.size()
                                      || !(std::isalnum((unsigned char) plan[after])
                                           || plan[after] == '\''));
                bool zoneScoped = false;
                for (size_t z = 0; z < sizeof(kZone) / sizeof(kZone[0]) && !zoneScoped; z++)
                    zoneScoped = (plan.compare(after, std::strlen(kZone[z]), kZone[z]) == 0);
                // The CLAUSE the name sits in decides first: a conditional or
                // negated clause ("Once X is on the battlefield", "if X is in
                // play") is a plan, not a false board, and its markers sit
                // BEFORE the name where neither window below would see them.
                size_t cs = plan.find_last_of(".;!?\n", pos ? pos - 1 : 0);
                cs = (cs == std::string::npos) ? 0 : cs + 1;
                const std::string pre = " " + plan.substr(cs, pos - cs);
                bool preNegated = false;
                for (size_t g = 0; g < sizeof(kNeg) / sizeof(kNeg[0]) && !preNegated; g++)
                    preNegated = (pre.find(kNeg[g]) != std::string::npos);
                if (!wordEnd || zoneScoped || preNegated || planClauseIsFuture(plan, pos))
                {
                    pos = after;
                    continue;
                }
                // BACKWARD: "<name> ... on the battlefield", within one clause.
                const size_t winEnd = (after + 90 < plan.size()) ? after + 90 : plan.size();
                const std::string fwdWin = plan.substr(after, winEnd - after);
                for (size_t k = 0; k < sizeof(kBack) / sizeof(kBack[0]); k++)
                {
                    const int sc = kBackScope[k];
                    if (live[sc] || formLive[sc])
                        continue; // true of the board this phrase names
                    const size_t at = fwdWin.find(kBack[k]);
                    if (at == std::string::npos)
                        continue;
                    const std::string span = " " + fwdWin.substr(0, at);
                    if (span.find_first_of(".;!?") != std::string::npos)
                        continue; // a sentence break: not the same claim
                    bool negated = false;
                    for (size_t g = 0; g < sizeof(kNeg) / sizeof(kNeg[0]) && !negated; g++)
                        negated = (span.find(kNeg[g]) != std::string::npos);
                    if (negated)
                        continue;
                    assertedOut = deckNames[n];
                    return true;
                }
                // FORWARD: "I control <name>", the phrase preceding the name.
                const size_t backStart = (pos > 60) ? pos - 60 : 0;
                const std::string backWin = plan.substr(backStart, pos - backStart);
                for (size_t k = 0; k < sizeof(kFwd) / sizeof(kFwd[0]); k++)
                {
                    const int sc = kFwdScope[k];
                    if (live[sc] || formLive[sc])
                        continue; // true of the board this phrase names
                    const size_t at = backWin.rfind(kFwd[k]);
                    if (at == std::string::npos)
                        continue;
                    const std::string span = " " + backWin.substr(at);
                    if (span.find_first_of(".;!?") != std::string::npos)
                        continue;
                    bool negated = false;
                    for (size_t g = 0; g < sizeof(kNeg) / sizeof(kNeg[0]) && !negated; g++)
                        negated = (span.find(kNeg[g]) != std::string::npos);
                    if (negated)
                        continue;
                    assertedOut = deckNames[n];
                    return true;
                }
                pos = after;
            }
        }
    }
    return false;
}

} // namespace gptcaveat

#endif
