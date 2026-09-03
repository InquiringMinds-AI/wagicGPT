# deck152 -> core-prompt / render proposals (wave 55)

## R-1 (MED, render) - print the opponent's OPEN MANA as a number

Every prompt lists the opponent's permanents with `[tapped]` markers and the pilot must count the
unmarked ones to know whether a counterspell or an instant is live. Our own side already gets
`Mana available: N total (untapped sources ...)` with the sources enumerated. Suggested one line
under the opponent battlefield line: `Their untapped sources: N (colours they could make: {u}{w})`.
Repro `152v125`: eight creature spells countered over 40 turns, every one cast with 5-8 unmarked
opponent permanents on screen; the single cast made at 2 unmarked (seq 101, Tovolar's Huntmaster)
resolved. This is a derived fact the engine already has, and it is the number the wave-55 152-B
posture makes the pilot compute by hand.

## R-2 (LOW, render) - `reveal_stall` is a duration field on 100% of `reveal` records, and
`reveal_stall_phase` is constant

12 of 12 `reveal` records corpus-wide carry `reveal_stall` (ticks, range 1,405-87,917),
`reveal_stall_secs` (4-474) and `reveal_stall_phase: 0`. So it is a stamp with real values, not a
zero-valued park - but a signature that fires on every record of a kind carries no signal for a
consumer looking for stalls, and `reveal_stall_phase` never varies. Suggested: emit the pair only
above the structural threshold the lane intended, or rename the always-present pair to
`reveal_wait_ticks` / `reveal_wait_secs` and reserve `reveal_stall` for the guarded case. Repro:
`152v125` s41 (211 s) and s47 (55 s) versus `152v123` s65 (4 s) - all three identically stamped.

## R-3 (LOW, prompt) - the served PLAN block should be dropped, not carried, once it contradicts
the current menu

`152v125` seq 101 served
`YOUR PLAN (as you last stated it, 6 windows ago on turn 39; "Katilda, Dawnhart Prime" is no longer
on your menu): Cast Katilda. Attack with nothing (no creatures) ...` followed by the excellent
`(note: this decision's list does not contain the actions your plan names ...)`. The note is right
and the plan is still six windows and two turns stale. Proposal: when the note fires AND the plan's
age exceeds ~4 windows, serve the note alone and drop the plan text - it is prompt bytes spent
carrying a statement the next line retracts, at a seat whose prompts already reach 30,031 chars
(`152v125` seq 314, the corpus maximum).
