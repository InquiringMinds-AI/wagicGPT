# Core-prompt / render proposals from the deck152 seat (wave 53)

## R-5 (MED, render). THE ATTACKERS PROMPT PRINTS EVERY POTENTIAL BLOCKER PER ATTACKER AND NEVER
## PRINTS HOW MANY BLOCKERS EXIST.

Each A-line carries `their untapped blockers: <name> (P/T) (<result>); <name> (P/T) (<result>)`,
repeated in full on every attacker. What decides an alpha strike is the OPPOSITE number: how many
of the pilot's attackers can be blocked at all. At `152v126` s26/s32/s40 the same two blockers
(Wall of Omens, Vampire, later Pride Guardian) were re-listed on four A-lines each time; the seat
declared two attackers into two blockers three combats running, held a 4/4 and a 7/5 whose worst
printed price was 1 life, and lost 0-21 with the opponent never below 16.

**Ask:** one line in the attackers header, next to the existing LETHAL/converter lines -
`They have N untapped creatures able to block; declaring more than N attackers leaves at least
(your attackers - N) of them unblocked.` The engine has N already (it built the per-attacker
lists from it). This is the attack-side twin of the block header's
`Unblocked, these attackers deal up to X - you would be at Y`, which does work: 9 block windows
across my seats, 0 floor breaks.

## R-6 (MED, engine/UX - routed here because the fix is "do not ask", not "print better").
## A MENU WHOSE OPTIONS ARE BYTE-IDENTICAL IS NOT A DECISION.

`152v123` T11: deck123 gang-blocked with a wall of identical tokens and the damage-assignment
order was asked one blocker at a time - s23 through s37, twelve to six rows per ask, every row
the literal string `Vampire (2/2) [flying, doesn't untap during its controller's untap step]`.
Fourteen consecutive model calls with no information to choose on. One of them (`s29`, a six-row
menu) came back `CHOICE: 7` - out of range, `unparsed_reply`, one of the corpus's 15 fallbacks -
and lane Q's new `recovery` record fired correctly at s30.

Corpus-wide: **16 asks whose full option list collapses to one distinct string, 147 rows, 2.9
minutes**, 14 of them at this one seat. **Ask:** when the de-duplicated option list has length 1,
answer it internally and log it, exactly as `mana_only_windows_skipped` (406 corpus-wide) already
does for the mana-only case. If the rows differ only by a `#N` handle they are still one choice.

## R-7 (LOW, render). THE `{paying this taps:` CLAUSE NAMES THE CREATURES BUT NOT THE COUNT, AND
## THE COUNT IS WHAT DECIDES.

Lane O's rework of this clause landed cleanly on the defender case - THREE-NUMBER AUDIT: old
string `- it cannot attack this turn` on a defender row **0 renders**, new string `- it cannot
block on their turn` **27 renders, 8 takes**, all at deck126's Overgrown Battlement. The
non-defender form is unchanged and correct (14 renders at deck152: Katilda, Luminarch Aspirant,
Intrepid Adversary, Elite Spellbinder - none of them defenders). What the clause still does not
say is how many attackers it removes, which is the whole trade: at `152v130` s11/s14 the clause
held ONE name and taking the row was right (it won that game 11 to -4); at `152v162` s29/s32 it
held two and passing was right. **Ask (cheap):** lead the clause with the count -
`{paying this taps 2 of your creatures: Luminarch Aspirant, Intrepid Adversary - they cannot
attack this turn}`.
