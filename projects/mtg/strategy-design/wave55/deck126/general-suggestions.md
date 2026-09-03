# wave-55 deck126 - core-prompt / render proposals

Layer-routed; engine defects and repros are in `wave55/seats/seat-125-126.md`. All strings quoted
from rendered prompts in `matchups-20260903-074354`.

## G7 - The `(blocking trigger: ...)` clause is a real improvement placed inside the wrong
## parenthesis.
`126v146` seq 6:
`B1. Pride Guardian (0/3) [defender] [deals 0 - this block kills nothing, but it STOPS all 1 damage
from reaching you THIS COMBAT] - may block A1 (your blocker dies, attacker lives (blocking trigger:
you gain 3 and may gain 2 more))`.
Nesting the gain inside the survival verdict makes a losing block read as a priced one. Both facts
should be printed and they should be SEPARATE annotations, in the order the decision uses them:
survival first, then the one-combat gain, then the standing cost. Proposed shape:

    B1. Pride Guardian (0/3) [defender] - may block A1 {your blocker DIES, attacker lives}
        {this combat: you gain 3, and Perimeter Captain may gain 2 more} {after this combat: you
        control 1 fewer blocker}

The third clause is the number the seat actually needed and no annotation carries it today: at
seq 6 the seat had 2 blockers and at seq 16 it had 1 against five attackers.

## G8 - A `[named: <card>]` tag on the opponent's battlefield line prices nothing on the row it
## affects.
The tag rendered on only 2 prompts this corpus (wave 54: 54), so this is carried, not re-argued:
lane C's `[NAMED BY THEIR <src>: ... you would be at K]` row-side price rendered **0 times** at this
seat and remains UNTESTED. The asymmetry is the point - the cost lives on THEIR line and the
decision is made on YOUR row, and every other cost in this render is on the row. Keep it in the
docket until a corpus gives it a denominator.

## G9 - Skip opponent-turn windows this deck can never use.
(Shared with `deck125/general-suggestions.md` G3; deck126 is the extreme case.) **169 opponent-turn
windows, 0 casts, six games.** 91.4% of them are inside runs of 3+ byte-identical declined menus at
the same turn, max run 15. Repro: `1788439475-ai_baka_deck126-0x556d88ee7260-vs-ai_baka_deck125.jsonl`
seq 16-27, 31-42, 46-56, 60-70, 76-86, 91-102. The seat's only instant is Tribute to Hunger and 218
of its 228 rows read `at 0 this does nothing`; a window whose every cast row is engine-annotated
dead is a window with one answer, and the engine already knows it is dead because it wrote the
annotation.

## G10 - `{paying this taps: <defender> - it cannot block on their turn}` is a good clause with no
## counterpart on the decline side.
30 rows this corpus, **0 takes**. Every one was a Tribute to Hunger row, and every one was declined
- correctly, because 218 of the Tribute rows were at 0 creatures. So the clause has never yet been
read in the situation it was written for. No change proposed; recording it so the 0/30 is not
mistaken for the pilot ignoring the clause. D98's forecast-vs-receipt test needs a corpus in which
a tap-clause row is actually taken.

## G11 - The Overgrown Battlement `VARIABLE output` line renders on 224 prompts and the
## `Mana available:` total still counts it as one.
This is the render telling the truth about a hard case and it is working: the guide's own Rule #1
teaches the "+1 per defender past the first" correction, and there is no pip error at this seat
this corpus. Recorded as a KEEP, not a change - the alternative (folding the variable output into
the total) would make the total wrong for the coloured-pip count, which is the error this deck
actually used to make.

## G12 - `mana_only_windows_skipped` 499 in one game is the proof that G9's mechanism works.
`126v125` gameend reports 499 suppressed Battlement-tap windows and `126v152` reports 45; both games
show zero anomalies attributable to the suppression, and the seat's Battlement was untapped for
every block it wanted. Whatever predicate G9 uses should report the same way, on the gameend, so the
next wave can adjudicate it from a counter rather than from a run-length scan.
