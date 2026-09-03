# deck125 -> proposals for the strategy-writing skill (wave 54)

Only what this seat's evidence supports. Numbering continues the wave-53 edition (187-208).

## S1 - A FLAT CONSTANT FLOOR IS WRONG IN ONE DIRECTION, AND #192 ONLY WARNED ABOUT THE OTHER ONE.

Wave 53 replaced deck125's moving-relation threshold ("cast a counter only when the price is below
your life minus 1") with a constant read off a printed literal ("take it only when `- you would be
at K` is 15 or more"), and #192 recorded the rule: a constant against a printed number is
checkable and cannot silently widen. It also cannot NARROW, and this corpus is the other half of
the lesson. The seat broke the new floor three times - `125v162` seq 29, 48 and 49, at K = 14, 6
and 4 - and **all three were correct plays**: seq 48 countered a Howling Mine and seq 49 a
Teferi's Puzzle Box, two permanents whose own `{target text:}` gives every player an extra draw
every draw step, under two `Whenever an opponent draws` punishers. The counters bought back 2 and
6+ damage **per turn, for the rest of the game**, for 2 life each.

Rule: **a constant floor is a comparison with one side missing.** When the quantity on the other
side is not fixed - here the thing on the stack costs 1 life per draw per turn forever, or costs
nothing - the fix is not a bigger or smaller constant, it is a SECOND printed literal that sorts
the cases. deck125's rule now reads the target's own `{target text:}` for "draws an additional
card" / "draws that many cards" / "draws seven cards" and takes any K above 1 in that branch, 15
otherwise. Both branches are still read off strings the row prints; neither is arithmetic.
Companion to #192 and to #202's "the correction to an over-aggressive absolute is a PRICE
COMPARISON, not a mirrored prohibition."

Review corollary, and it is the cheap one: **when a rule is broken and the game is still lost,
check whether the break was the misplay before you write the edit.** All three breaks here scored
as failures on a mechanical count and none of them was the reason the seat died.

## S2 - AN ORDERED PROCEDURE'S STEP IS ONLY AS RELIABLE AS THE STRING IT READS, AND A STEP THAT
## SAYS "BEYOND THE SECOND IN HAND" IS A DERIVATION, NOT A STRING.

deck125's cleanup-discard rule is a correct seven-step ordered list. Step 2 read "a land beyond
the second in hand" - a count of the pilot's own hand, minus a constant. It was skipped in **three
of the four windows where a land was on the numbered list**, and two of those three are two of the
seat's five losses (`125v162` seq 47, both Fall of the Gavel sent at 8 life on a NINE-land
battlefield; `125v126` seq 73, both sent at t23 on TEN lands, with the opponent's Sanguine Bond
resolving uncountered the next turn). The step now reads the battlefield header's own printed
`and N are lands` and sends every land while N is 6 or more.

Rule: extends #189 and #193 to ordered lists. **For each step of a procedure, name the string on
screen that satisfies it.** A step whose satisfaction requires the pilot to count a zone and
subtract is the step that gets skipped under pressure, and the pilot then falls through to the
step it can evaluate - which is always the last one, because the last step of a discard order is
"a counterspell". #199 established that the step that WINS must be step 0; this is its companion:
**the step that is hardest to evaluate falls to the bottom of the list in practice regardless of
where you wrote it.**

## S3 - #201'S LIVE/RAW SPLIT NEEDS A THIRD COLUMN: RAW, LIVE, AND CORRECT-ANYWAY.

Three of this seat's rules produced counts that are honest only with two numbers, and one needed
three. The tap-out gate: **5 raw breaks, 1 live** - the other four were a sweeper at a header of 5,
a sweeper at 4, an Elixir at 2 life and a turn-1 cast with a 5-mana counter in hand that no board
could have cast. The Path rule: **2 raw breaks, 0 live** - both "1-power" bodies had repeating
text. deck126's D92: **73 raw, 5 live, 0 consequential** - 65 of the 73 were one won game's
endgame. In every case the fix was to write the exemption INTO the rule, not to loosen it: a count
that includes cases the rule never meant is a count the next reviewer cannot compare against.

Rule: report `N raw / M live / K consequential` and score K, and then **put the exemptions in the
guide text**, because an exemption that lives only in the seat file has to be rediscovered every
wave. Extends #201 and #198 (which already asks for the distinct-turn count).

## S4 - WHEN A CORPUS'S WINDOW COUNT MOVES, EVERY BEHAVIOURAL RATE AT THAT SEAT IS SUSPECT BEFORE
## IT IS EVIDENCE.

deck125 went 4-1 to 1-5 while its decisions fell 1,075 -> 405 and its opponent-turn windows fell
479 -> 109 against a turn count that fell only 261 -> 169. The cause is an engine defect (seat file
HIGH #1) and it removed the seat's entire mode of play from three of its six games - in `125v130`
the deck received **one** opponent-turn window in twenty turns. Three of the five losses have no
identifiable misplay and no edit was written for them.

Rule: the NEW-BASELINE discipline (do not attribute win-rate variance to guide edits) has a
sibling that is not about hands. **Before scoring any per-window rate, compare this corpus's
window count per seat-turn against the last one; if it moved by more than a factor, say so at the
top of the seat file and mark the rates as incomparable.** A ceiling that "held at 40/40" when the
denominator was 155 last corpus is not the same evidence. This is #188's denominator discipline
applied to the corpus rather than to a literal.
