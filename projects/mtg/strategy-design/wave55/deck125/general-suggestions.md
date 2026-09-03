# wave-55 deck125 - core-prompt / render proposals

Layer-routed. Engine defects and their repros are in `wave55/seats/seat-125-126.md`; what follows
is prompt/render text and window policy only. Every string below is quoted from a rendered prompt
in `matchups-20260903-074354`.

## G1 - Print the incoming-combat total, the way every other price in this engine is printed.
(seat file HIGH #1; guide half W-A1.)
`125v146` seq 36. The opponent line prints `3 of them are attacking right now` and then each
attacker with its live P/T - `Barrowin of Clan Undurr (5/5) (printed 3/3) [counters: 1x +1/+1]
[tapped - attacking]; Goblin (2/2) (printed 1/1) [tapped - attacking]; Nadaar, Selfless Paladin
(4/4) [vigilance] [attacking]` - against `Your life: 10`. Eleven power, ten life, and the prompt
prints no total anywhere. Contrast the same prompt's siblings: `[DRAW PRICE: ... - you would be at
K]`, `{right now: destroys 3 of their creatures (3 without a restriction against attacking), 0 of
yours}`, `{leaves N of your M untapped mana sources untapped}`. Every one of those is a finished
subtraction; the one that ends games is not.

Proposed line, on ANY window during the opponent's combat (priority windows included - a seat with
no creature never reaches a `blockers` ask and so never sees the "you would be at N" header that
already exists there):

    INCOMING THIS COMBAT: 3 attackers, 11 unblocked damage - you would be at -1.

with the blockable/unblockable split when the seat has bodies. It is one line, it reuses the header
the blockers ask already computes, and it removes a three-string derivation from the pilot.

## G2 - Give discard rows the `{right now:}` evaluator the cast rows already have.
(seat file HIGH #2.)
Lane D shipped two verdicts and they work: `{spare: you control 12 lands already}` (29 rows, 19
takes) and `{dead right now: 0 legal targets on the board for it}` (17 rows). **267 of 313 discard
rows are still bare**, and the bare ones are where this deck's dead cards live. The engine already
computes each of these for the SAME cards' cast rows in the same turn:

| discard row | verdict it should carry | source already computed |
|---|---|---|
| Supreme Verdict / Final Judgment | `{dead right now: they control 0 creatures}` | the cast row's `{right now: destroys 0 of their creatures}` |
| Lightmine Field | `{dead right now: they control 0 creatures able to attack}` | the cast row's `{right now: they control 0 creatures able to attack - deals 0 ...}` |
| Essence Scatter | `{dead right now: every creature they have cast is a wall}` | the counter row's stack-line typing |
| Emrakul, the Aeons Torn | `{dead right now: you control 13 lands, this needs 15}` | the mana line |

Repro asks: `125v162` seq 64, `125v126` seq 166, `125v152` seq 104, `125v123` seq 45.

## G3 - Skip the empty-stack opponent-turn window the way mana-only windows are already skipped.
(seat file MED #3.)
deck125: 291 opponent-turn windows at nine phases, **zero casts**, four corpora running for
End / Cleanup / Combat-begins / Combat-ends. deck126: 162 of 169 opponent-turn windows, zero casts.
`mana_only_windows_skipped` already proves the mechanism and the counter (`126v125` reports 499).
The predicate that matches the observed waste exactly: opponent's turn AND `ON THE STACK` empty AND
the seat has cast nothing at this phase in this game. Repro block:
`1788439475-ai_baka_deck126-0x556d88ee7260-vs-ai_baka_deck125.jsonl` seq 16-27 (twelve
byte-identical windows, one opponent turn, nothing on the stack, `Cast nothing right now` twelve
times).

## G4 - The HOLD row is being offered where it cannot help and skipped where it could.
Takes 87 of 1,086 (8.0%); in windows already carrying `[you declined this exact list N times
already this turn]` with N >= 3, **19 of 459 (4.1%)**. The row's own text promises a saving the
pilot has no way to price: `{taking this row skips the rest of this turn's identical windows}` does
not say HOW MANY. Proposal: print the number the engine already knows -
`{taking this row skips the N identical windows left in this turn}` - and, where N is 0, do not
print the row at all. That converts the row from a policy choice into a priced one, which is the
device the rest of this render already uses.

## G5 - The 63-turn game log is 26 KB in front of a five-row menu.
`125v152` seq 218: 29,808 chars, `kind: "priority"`, 5 options, at turn 63. The decision needs the
board, the stack and the last turn or two; it is handed sixty. This is not a legality or a
decision-quality item - it is the efficient-inference third of the core loop, and it is the biggest
single lever in the corpus's 22.98 h. A `=== Turn N ===` block older than some horizon could
collapse to one line per turn (`Turn 31 - YOUR turn: played Island; Staff of Nin pinged (now 9)`)
with the last three turns kept verbatim. Falsifier: max prompt below 20 K with no rise in
`stale_echo`, `plan_choice_conflict` or reveal-pick errors.

## G6 - `reveal_stall` fires on every reveal, so it cannot be read as an alarm.
(seat file MED #4.) Five reveals, five stamps, `reveal_stall_secs` equal to the reply latency in all
five (24, 42, 60, 148, 474 s) against a poll-churn floor of max(1800 s, 3x deadline). Emit above the
floor only, or rename it `reveal_wait_secs` and add a separate boolean park flag.
