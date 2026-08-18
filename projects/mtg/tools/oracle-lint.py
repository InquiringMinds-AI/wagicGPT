#!/usr/bin/env python3
"""Best-effort Oracle-vs-script lint for primitives.

For each card whose text= (modern Oracle) contains an effect-family cue,
require the card block's script side (auto=, abilities=, aicode=, ...) to
contain that family's script vocabulary. A miss means the clause is likely
unimplemented, mis-implemented, or implemented through vocabulary this tool
does not know - every flag needs a human read before it is called a bug
(the checker's dictionary is itself a hypothesis; see Territorial Gorger,
whose energy handling is the @energizedof trigger, not alterenergy).

Reminder text (parentheticals) is stripped before matching so a keyword's
explanation cannot fire a cue. Quoted granted abilities are kept: they are
card function.

Usage: tools/oracle-lint.py [primitives.txt] [-f FAMILY] [--list-families]
Default primitives path: bin/Res/sets/primitives/mtg.txt relative to cwd.
Output: one line per miss, "FAMILY | card name | script side (truncated)",
plus a per-family summary. Exit code is always 0: this is a survey
instrument, not a gate.
"""
import re
import sys
import argparse

# (family, cue regex on cleaned Oracle text, script regex on the block's
# script side). Cues aim for the card TAKING the action; script vocab lists
# every way the engine is known to express the family.
FAMILIES = [
    ("draw",       r"draws? (a|one|two|three|four|five|six|seven|x|\d+) card",
                   r"draw|_loot_|cycling"),
    ("lifegain",   r"(you )?gains? (\d+|x) life|gain life equal",
                   r"life:|lifeleech|lifeset|lifegain|spiritlink"),
    ("lifeloss",   r"loses? (\d+|x) life|lose life equal",
                   r"life:-|lifeleech|lifeloss|damage[:(]|drain"),
    ("damage",     r"deals? (\d+|x) damage|deals? damage equal|deals? that much damage",
                   r"damage|lifeleech|strike|fog|absorb"),
    ("destroy",    r"destroy (target|all|each|that|it|up to)",
                   r"destroy|bury|treason"),
    ("exile",      r"exiles? (target|all|each|that|it|up to|the top)",
                   r"exile|removedfromgame|blink|impulse|unearth|l2e|ingest|\\{e\\(|serumpowder"),
    ("counterspell", r"counter (target|that|it).{0,40}spell|counter target ability",
                   r"fizzle|countershroud"),
    ("token",      r"creates? (a|one|two|three|x|\d+|that many)|puts? (a|one|two|three|x|\d+).{0,40}token",
                   r"token|copy|create\(|scion|spawn|clone"),
    ("p1p1counter", r"puts? (a|one|two|three|x|\d+|that many) \+1/\+1 counter",
                   r"counter\(1/1|counters|proliferate|vigor|\{c\(1/1"),
    ("m1m1counter", r"puts? (a|one|two|three|x|\d+|that many) -1/-1 counter",
                   r"counter\(-1/-1|counters|proliferate|wilting|\{c\(-1/-1|\{c\(1/1,-"),
    ("energy",     r"\{e\}|energy counter",
                   r"alterenergy|penergy|\{e[:}]|energized"),
    ("scry",       r"scry (\d+|x)",
                   r"scry"),
    ("surveil",    r"surveil (\d+|x)",
                   r"surveil"),
    ("mill",       r"mills? (a|one|two|three|x|\d+)|puts? the top .{0,30}(into (your|his or her|their) graveyard)",
                   r"deplete|mill|moveto\(.*grave|\{m[\}\{]"),
    ("discard",    r"discards? (a|one|two|three|x|\d+|that|all|their|his or her|your)",
                   r"discard|reject|madness|_loot_|\{d[\(\}]|moveto\(m?y?graveyard\)|graveexiler"),
    ("bounce",     r"returns? (target|all|each|that|it|up to|another).{0,60} to (its owner's|their owners?'|your|his or her) hand",
                   r"moveto\(.*hand|return|bounce|\{h[\(\}]"),
    ("tutor",      r"search(es)? (your|their|his or her) library",
                   r"library|shuffle|nonplay"),
    ("sacrifice",  r"sacrifices? (a|one|two|three|x|\d+|target|all|each|that|it|this)",
                   r"sacrifice|\{s[\(\}]|scion|spawn|treason|moveto\(.*grave|bury notatarget|saclands"),
    ("tap",        r"(?<!un)tap (target|all|each|up to|that|it)",
                   r"tap|freeze|frozen"),
    ("untap",      r"untap (target|all|each|up to|that|it|this)",
                   r"untap"),
    ("regenerate", r"regenerat(e|es|ed)\b",
                   r"regenerate"),
    ("proliferate", r"proliferate",
                   r"proliferate"),
    ("fight",      r"fights? (target|another|each|up to)",
                   r"fight|powerstrike"),
    ("gaincontrol", r"gains? control of",
                   r"mybattlefield|opponentbattlefield|ownerbattlefield|targetedpersons|newcontroller|steal|control|shackle"),
]

# Human-reviewed flags that are NOT bugs: engine workarounds judged
# acceptable, or clauses outside the engine's model. (family, card name).
ALLOW = {
    ("destroy", "Hornet Cannon"),        # destroy-at-end lives in the numeric token's own definition
    ("token", "Imperial Mask"),          # teammates-copy clause; engine has no team play
    ("untap", "Ith, High Arcanist"),     # removefromcombat bundles untap + no-damage
    ("counterspell", "Vigilant Martyr"), # "targets an enchantment" restriction not expressible; broad counter would be wrong
    ("destroy", "Suleiman's Legacy"),    # lord() sacrifice stands in for the destroy trigger
    ("damage", "Hydra Omnivore"),        # each-OTHER-opponent clause; moot in 1v1    # lord() sacrifice stands in for the destroy trigger
}

EVERGREEN = ["flying", "deathtouch", "lifelink", "trample", "haste", "vigilance",
             "menace", "reach", "defender", "hexproof", "indestructible",
             "first strike", "double strike", "flash"]


def strip_reminder(text):
    text = re.sub(r"\([^)]*\)", "", text)
    #double-faced cards carry the backside's text after a ---- separator, but
    #the backside is its own primitive block; audit each face on its own
    text = text.split("----")[0]
    #abilities granted to tokens/creatures are quoted; the grant itself is the
    #card's function (usually other cues), the quoted text is not
    text = re.sub(r'"[^"]*"', "", text)
    #negations that would fire an effect cue the card does not perform
    return re.sub(r"can.t be regenerated|couldn.t be regenerated", "", text,
                  flags=re.I)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("primitives", nargs="?",
                    default="bin/Res/sets/primitives/mtg.txt")
    ap.add_argument("-f", "--family", help="run one family only")
    ap.add_argument("--list-families", action="store_true")
    args = ap.parse_args()

    if args.list_families:
        for name, _, _ in FAMILIES:
            print(name)
        print("keyword:<each evergreen keyword>")
        return

    data = open(args.primitives, "rb").read().decode("latin-1")
    blocks = data.split("[/card]")
    #expand _MACRO_ references: a macro-implemented effect should be judged by
    #the macro's expansion, not its opaque name (_macros.txt, #AUTO_DEFINE)
    macros = {}
    try:
        import os
        mpath = os.path.join(os.path.dirname(args.primitives), "_macros.txt")
        for line in open(mpath, "rb").read().decode("latin-1").splitlines():
            mm = re.match(r"#AUTO_DEFINE (_[A-Za-z0-9_]+_)", line)
            if mm:
                macros[mm.group(1).lower()] = line.lower()
    except OSError:
        pass
    fams = [f for f in FAMILIES if not args.family or f[0] == args.family]
    checked = {f[0]: 0 for f in fams}
    missing = {f[0]: [] for f in fams}
    kw_checked = {}
    kw_missing = {}

    for b in blocks:
        m = re.search(r"^name=(.+)$", b, re.M)
        t = re.search(r"^text=(.+)$", b, re.M)
        if not m or not t:
            continue
        name = m.group(1).strip()
        #alias= cards inherit another card's whole script; nothing to audit
        if re.search(r"^alias=", b, re.M):
            continue
        text = strip_reminder(t.group(1)).lower()
        #script side = every key except identity/stat lines. Effect scripting
        #hides under many keys (autofacedown/autofaceup/autostack/other/mana
        #for additional costs, ...), so exclusion beats enumeration here.
        script_lines = [l for l in b.splitlines()
                        if "=" in l
                        and not re.match(r"(name|text|type|subtype|power|"
                                         r"toughness|id|rarity)=", l)]
        script = "\n".join(script_lines).lower()
        for mname, mbody in macros.items():
            if mname in script:
                script += "\n" + mbody

        for fam, cue, vocab in fams:
            if not re.search(cue, text):
                continue
            checked[fam] += 1
            if not re.search(vocab, script) and (fam, name) not in ALLOW:
                missing[fam].append((name, script.replace("\n", " | ")[:140]))

        if not args.family or args.family.startswith("keyword"):
            for kw in EVERGREEN:
                # the card's own keyword line ("Flying" alone or comma lists),
                # not "gains flying"-style grants (those are effect scripting
                # with wide vocab; too noisy for v1)
                if re.search(r"(^|\n|-- ?)((" + kw + r")([ ]*[,.;]|[ ]*--|$))",
                             text):
                    kw_checked[kw] = kw_checked.get(kw, 0) + 1
                    if kw.replace(" ", "") not in script.replace(" ", ""):
                        kw_missing.setdefault(kw, []).append(
                            (name, script.replace("\n", " | ")[:140]))

    print("family          checked  missing")
    for fam, _, _ in fams:
        print(f"{fam:15} {checked[fam]:7}  {len(missing[fam]):7}")
    for kw in sorted(kw_checked):
        n = len(kw_missing.get(kw, []))
        print(f"kw:{kw:12} {kw_checked[kw]:7}  {n:7}")
    print()
    for fam, _, _ in fams:
        for name, script in missing[fam]:
            print(f"{fam} | {name} | {script}")
    for kw in sorted(kw_missing):
        for name, script in kw_missing[kw]:
            print(f"kw:{kw} | {name} | {script}")


if __name__ == "__main__":
    main()
