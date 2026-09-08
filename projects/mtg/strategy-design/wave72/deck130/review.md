# Wave 72 — deck 130 (mono-red land destruction) — per-deck review

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-163626`, binary
`archives/wagic-ba6af25f1-w72step1` (source `ceb9b8de0`), `--thinking on`. My six seat files are
`...ai_baka_deck130-0x<ptr>-vs-...`. Live guide = the wave-71 edit (19,949 bytes).

**Seat census.** 290 protocol replies over 6 games (wave 71: 353). **0 fallbacks, 0 truncations, 0
`action_before_plan`, 0 `menu_pass_no_progress`, 0 `ask_replays_refused`.** 14 `plan_line_missing`
(4.8%), 13 of those also `off_protocol_bytes` 19-125; 7 `async_drops`; 17 `ask_replays_reserved`;
3 `identical_option_asks_resolved`; 2 `repeat_annotated_takes`; 126 `hold_windows_skipped`.
New wave-72 instruments in this seat: `own_turn_windows_skipped` **45** (deck146 25, deck123 14,
deck152 6, others 0) — this is where the decision drop came from; `sibling_window_asks_skipped`
**0 in all six**; `stop_reached_windows_skipped` **0 in all six**.

---

## (1) Game-by-game, and the deciding decisions

| Opponent | Result | Turn | Final life | Replies |
|---|---|---|---|---|
| 123 | **WIN** | 22 | 14 / 0 | 56 |
| 125 | **WIN** | 36 | 18 / -1 | 95 |
| 126 | LOSS | 21 | 0 / 31 | 51 |
| 146 | LOSS | 17 | 0 / 22 | 27 |
| 152 | LOSS | 11 | 0 / 20 | 19 |
| 162 | LOSS | 10 | 0 / 16 | 42 |

### vs 123 — WIN. Wave-71 edit E (Siege-Gang as the clock) closed the game
Seq 45 casts the Commander at 7 mana; **seq 48, 50, 60, 62** are four `{1}{R}` face sacrifices
(15 → 13 → 11, attack for 3 → 8, then 6 → 4 → 2 → 0) with two Pyrite Spellbombs in between
(seq 54, 58). Seq 62's row read `they would be at 0; THIS WINS THE GAME`. The wave-70 guide called
this ability "REMOVAL, not a clock"; the wave-71 rewrite named exactly this line and the model ran
it. Also a clean execution of the LAND PLAN's three-number discipline: seq 35 and 38 write
`PLAN: Their lands 2; my life 14; their power 0.` verbatim. **No misplay found.**

### vs 125 — WIN. The 6-LIFE FLOOR release and the Hammer loop, with the Commander removed
Seq 36/46/60/78/83/87/96/101 are eight Hammer casts at the face; seq 41/50/66/81/86/92/99 are seven
upkeep returns. The wave-70 floor-release clause ("their line reading 0 creatures AND 2 or fewer
lands, once a land-kill of yours resolved") fired correctly at **seq 36** (opponent 2 lands, 0
creatures, Stone Rain resolved turn 6) and the face row stayed the clock for the rest of the game.
Wave-71 edit F's brake ("`Opponent life trend` showing +3 a turn means the return is not a clock")
did not misfire: the opponent's Elixir of Immortality gained 5 twice (opp 1 → 6, 3 → 6) but their
board was empty, so the loop still closed. Siege-Gang landed at seq 76 and was Path-to-Exiled the
same turn; **the absence of any sacrifice row on the 24 windows of turns 28-36 is correct, not an
engine miss** — the Commander was gone and the Goblins had no outlet.

### vs 152 — LOSS. **The deciding misplay of the wave**: a Starstorm that tapped out at upkeep
**seq 18 (turn 10, UPKEEP, life 9, `Mana available: 4`, `Land drop: NOT yet used this turn`, hand =
Lay Waste / Starstorm / Siege-Gang Commander / Mountain).** Opponent board: Sigarda, Champion of
Light (4/4 flying), Wolf (4/4), Katilda (2/2), Ranger Class L2, an animatable Lair of the Hydra —
and deck130 controlled **zero creatures**. The model cast Starstorm at X=2 (seq 19/20), killing the
one tapped 2/2, and spent all four mana at upkeep. Passing instead gives Mountain + 4 = 5 mana in
main 1 = **Siege-Gang Commander and four blockers**; it died on turn 11 with the Commander still in
hand. The guide's own upkeep exception forbids this on its second clause ("after paying it your
remaining Mana available: still covers every ORDER OF OPERATIONS entry"), but three other rules
pushed the other way — ORDER OF OPERATIONS entry 1 ("Sweep BEFORE you deploy"), STARSTORM'S X
SURVIVAL OVERRIDE (their printed power 10 ≥ life 9, and what X=2 leaves alive is 8, one short of
the "buys nothing" release), and SITUATIONS bullet 1. STRATEGY. Edits A/C/D.
Earlier: **seq 16 (turn 8, life 15)** cast Stone Rain with the frame printing
`CRACK-BACK NEXT TURN: … you would be at 8, and lower still` and `you control NO creature`. The LAND
PLAN's numbers (N=4, L=15, P=4) all read alive. It was still the best of a bad menu — Siege-Gang
needed 5 and it had 4 — so I do **not** propose a land-plan edit off it (see §3, rejected).

### vs 162 — LOSS. Draw-punisher deck again; turn 10 was 20 windows of an unstoppable stack
Life 20 → 18 → 14 → 10 and then bled out on turn 10 to a stack of draw-punisher triggers. Seq 28-47
are **20 consecutive windows on turn 10** (of 44 in the whole game), each a link of the same chain,
each answered "pass / hold / cast nothing" with the reasoning saying so; the model took the hold row
at seq 28, 35 and 42 and was re-asked every time. Nothing in those 20 windows was a decision. No
guide item: the deck has no answer to a 4-toughness punisher engine and the model refused every
optional draw correctly. ENGINE item below.

### vs 126 — LOSS to Sanguine Bond + Exquisite Blood; no misplay after the loop landed
Sanguine Bond resolved turn 17, Exquisite Blood turn 19. From turn 19 on, the render's new
`LOOP COMPLETE:` header is correct and complete, and every line the seat had was fatal: **seq 51**
(cast Rorix, correct — 6 flying over two defenders, opponent 17 → 11), **seq 53** (block or not,
both enter the chain; the render says so and the reasoning traces it exactly). The WALL CHECK
correctly held the board home at **seq 44** (Perimeter Captain 1/4 + Pride Guardian 1/3, blocking
triggers gain 2 and 3) and **seq 49** (one blocker, `blocking trigger: they may gain 2, and if they
do their converter takes 2 off you`). Attacking would have fed the converter. Matchup loss.

### vs 146 — LOSS on board, one low-cost guide-letter case
**seq 22 (turn 12, life 9, 4 mana, 0 creatures, their 2 creatures, `you would be at 5`)** casts
Stone Rain at a basic Plains. The LAND PLAN reads alive (N=4, L=9, P=4) and the reasoning says so —
but the alternatives were Lay Waste (same thing, 3 life to a Silverquill Silencer naming it) and
nothing, since Rorix needed 6 and the Commander 5. LOW, no edit. The game was decided turns 8-10
when it took 8 unblocked damage with an empty board.

---

## (2) Engine / interface / card items

### HIGH — a resolving trigger CHAIN the seat cannot interact with is put to it once per link
`...vs-ai_baka_deck162.jsonl`, **turn 10 = seq 28,29,30,31,32,34,35,36,37,39,40,41,42,43,44,45,46,47
(20 windows)**, ~90 s p50 each. Rendered line at seq 32:
`3. Cast nothing right now {answers the stack: NO - 13 damage is ALREADY ON THE STACK, and taking
this row lets it resolve: that puts you at -4 and KILLS you}`; at seq 37 the same row reads
`11 damage`, at seq 46 `6 damage`. The hold row was taken at seq 28, 35 and 42 and re-opened every
time, correctly per its own contract — the countdown is "a damage figure", and the contract's
same-row carve-out covers only a moved *life total*. So the collapse this deserves is not reachable
from the hold row as written. Deck130's only instant is Starstorm and it was not castable in any of
the 20. Suggested shape (engine's call): treat a run of windows whose only delta is the decreasing
total of the SAME resolving chain as one row for hold purposes — the same idea the wave-72
`[LOOP RUNNING: …]` bracket already applies to the Sanguine Bond chain in the deck126 file (seq 51),
which does say "If your answer would be the same for every link, the HOLD row answers all of them at
once" — but that bracket is scoped to the life LOOP and did not appear here.

### MED — `plan_line_missing` is still 100% a DROPPED LABEL, never a missing plan (wave-71 item, unchanged)
14 of 14 records: the model writes the plan sentence and omits the `PLAN:` label, so the sentence
scores as `off_protocol_bytes` and the carried plan silently reverts.
Repro: `...vs-ai_baka_deck123.jsonl` **seq 33**, reply verbatim
`"\n\nDestroy a nonbasic land.\nCHOICE: 1 (Cast Card Normally)"` (`off_protocol_bytes: 24`).
**New sub-shape worth naming: two of the 14 echo the prompt's own phrase** —
`...vs-ai_baka_deck125.jsonl` **seq 102** `"\n\nYOUR PLAN: Cast Hammer of Bogardan targeting the
opponent to win.\nCHOICE: 1 …"` and `...vs-ai_baka_deck162.jsonl` **seq 43** `"\n\nYOUR PLAN: I
cannot survive the 9 damage on the stack…"`. The frame prints `YOUR PLAN (as you last stated it…)`
and the model mirrors it. Accepting a leading `YOUR PLAN:` as the label is a one-token fix.
One record is a genuine label-and-sentence omission: deck125 **seq 80**, reply `"\n\nATTACK: A1, A2,
A3"`, `off_protocol_bytes: 0`. All 14 parsed to the right action.

### MED — a cast reversed at the modal menu costs two windows and leaves no record of the reversal
`...vs-ai_baka_deck162.jsonl` **seq 30 → 31** and **seq 40 → 41**: the casting ask is answered
`Cast Spark Spray`, then the `Choose an option for` menu is answered `Decline - do nothing` — twice,
in a position the reasoning already calls lost. Four windows, no state change. The wave-71 header
naming the pending cast is present and the reversals are deliberate (the reasoning at seq 31 says
"cycling would draw a card, adding more damage", which is correct against draw punishers), so this
is not a misread — it is the cast ask offering a row the model will abandon at the next screen. A
`{cycling here costs you N life: DRAW PUNISHERS}` annotation on the Spark Spray CAST row would close
it one window earlier.

### LOW — `{X}` announcement payment loss: **0 fires in this seat, and 0 corpus-wide**
`/usr/bin/grep -c 'dropped after its X was announced'` over all six of this seat's game stderr files
(`game-{123,125,126,146,152,162}v130-*.stderr`) returns **0**, and 0 across every stderr in the run.
The string is present in the shipped binary (`strings archives/wagic-ba6af25f1-w72step1` → 1 hit;
source `MTGRules.cpp:475`, `_DEBUG || WAGIC_DEVLOGS`), so this is a real zero, not an instrumentation
gap. Deck130 announced X twice — `...vs-ai_baka_deck123.jsonl` seq 27→28 and
`...vs-ai_baka_deck152.jsonl` seq 18→19→20 — and **both completed through `Cast Card Normally`**;
no X menu was declined. **Tapped-out turns following an abandoned announcement: 0** (there were no
abandonments). Docket item for wave 73 is UNTESTED from this seat.

### LOW — 45 `own_turn_windows_skipped` in the seat, concentrated in the two shortest games
deck146 25 and deck152 6 are the two games the seat lost fastest, and deck123 14 the longest win.
Nothing in either game reads as a lost decision, but the instrument's distribution is worth the
engine seat's eye: the skip count is highest exactly where the seat had least to do.

### Card facts — all verified against the primitive, no item
`Talisman of Impulse` (`mtg.txt:117529`, `{T}:Add{1}` free, `{T}:Add{R}/{G}` with
`damage:1 controller`); `Siege-Gang Commander` (105071, `{1}{R}{S(goblin|myBattlefield)}:Damage:2
target(anytarget)`, and the Commander is itself a Goblin so the ability can eat it — not observed);
`Starstorm` (112835, Instant, `{X}{R}{R}`, cycling {3}); `Hammer of Bogardan` (52046,
`autograveyard={2}{R}{R}{R}:moveto(hand) myUpkeepOnly`); `Dwarven Blastminer` (33251,
`destroy target(land[-basic])`); `Rorix Bladewing` (96836, Legendary, 6/5 flying haste);
`Lay Waste` (66043, `{3}{R}`, cycling {2}). No Scryfall disagreement worth reporting.

---

## (3) Guide verdict: **EDIT** — revised guide at `wave72/deck130/strategy.txt`

**19,990 bytes vs the live 19,949 — +41** (445 bytes added, 404 trimmed). The rule that earned those bytes is **the upkeep
tap-out gate (edit A)**: it is the only rule in the file that would have changed a decision this
corpus shows losing a game, and it is stated as arithmetic the model can execute, not a caution.
Four trims (listed at the end) pay for the rest.

**Edit A — the upkeep exception becomes a subtraction, and names cycling.**
Before: `…still covers every ORDER OF OPERATIONS entry this board matches. An upkeep payment that
leaves you short of your main phase is the wrong answer.`
After: `…still covers every ORDER OF OPERATIONS entry this board matches. A CYCLING ROW IS NEVER ONE
OF THEM. Do the subtraction before you answer: "Mana available:" minus this row's cost, plus 1 when
"Land drop:" reads NOT yet used - if that number cannot cast the cheapest creature in your hand,
pass. An upkeep row that taps you out is the wrong answer.`
Paid by **four upkeep tap-outs, one of which lost a game**:
- `...vs-ai_baka_deck152.jsonl` **seq 18** — the Starstorm above; the land drop was unused and the
  Commander was in hand, and the subtraction (4 − 4 + 1 = 1) refuses it.
- `...vs-ai_baka_deck123.jsonl` **seq 9** (turn 6 upkeep, 2 mana, `Land drop: NOT yet used`) —
  cycled Lay Waste on a row that printed `{spends 2 of your 2 untapped mana sources this turn;
  Talisman of Impulse {2} in your hand needs 2}`. Turn 6's main phase then cast **nothing** (seq 10
  is the land drop and the turn ends).
- `...vs-ai_baka_deck125.jsonl` **seq 12** (turn 8 upkeep, 4 mana) — cycled Lay Waste, and its own
  reply names the consequence: `"…In main phase, tap Talisman for {R} (pay 1 life), cast Molten Rain
  at Tundra."`
- `...vs-ai_baka_deck125.jsonl` **seq 70** (turn 24 upkeep, 3 mana) — cycled Starstorm for {3}; turn
  24's main phase cast nothing.
The live guide's UPKEEP exception list already excludes cycling by omission and the ORDER OF
OPERATIONS already says "Cycle in second main phase". Both were read past four times, so the fix is
placement and arithmetic, not new policy.

**Edit B — the cycling window is stated where the model decides it.**
Before: `Cycle in second main phase.`
After: `Cycle in your SECOND MAIN PHASE only, never at your own upkeep.`
Paid by the same four seqs. The old sentence sits in ORDER OF OPERATIONS; the model is in the
CYCLING section (or on an upkeep priority menu) when it takes the row.

**Edit C — STARSTORM'S X gains a third refusal, and SURVIVAL OVERRIDE stops lifting it.**
Added: `(c) paying it taps you out while a creature sits in your hand - count the land drop, cast
the body instead, and sweep on a later turn: one blocker beats a sweep that kills one creature.`
and `SURVIVAL OVERRIDE, which never lifts (c):`. `DO NOT CAST when either holds` → `when ANY holds`.
Paid by deck152 **seq 18/19/20**: SURVIVAL OVERRIDE's arithmetic (their printed power 4+4+2 = 10 ≥
life 9; what X=2 leaves alive is 8, one short of the "buys nothing" release) actively endorsed a
sweep that killed one tapped 2/2 and left the seat creatureless against 8 power.

**Edit D — SITUATIONS bullet 1 corrected (it was the third push toward seq 18).**
Before: `Two non-defender creatures on their line, Starstorm in hand, neither Rorix nor Siege-Gang
on your battlefield -> Starstorm, before the Commander. If Rorix is already attacking for 6, hold
it.`
After: `Two non-defender creatures on their line and Starstorm in hand -> Starstorm, but only at an
X that kills both and still leaves you the mana for a body; otherwise cast the body.`
Paid by seq 18: the old bullet's trigger (two non-defenders, Starstorm in hand, no Commander on the
battlefield) matched exactly, and at X=2 the sweep killed neither of the two.

**Bytes trimmed to pay for the above.**
- The modal-menu paragraph's third sentence (`Casting at X=0 is not cycling…`) — **0 X=0 takes** on
  either X menu this wave, and the engine now names the pending cast in the header; of 13 modal
  menus, 10 were completed with "Cast Card Normally", 1 chose cycling (deck162 seq 5, guide-correct: no creature to kill) and the 2 declines (deck162 seq 31, 41) were deliberate. −115.
- DRAW PUNISHERS: the `[DRAW PRICE]` scope sentence collapsed into one clause and Fate Unraveler's
  arithmetic replaced by the rule it encodes — the false-on-a-cast-row fire has not recurred for two
  waves. −132.
- BLOCKER TAGS' restatement of the render's own collapsed-tag wording — the render explains itself in
  every blockers prompt. −142.
- SITUATIONS bullet 1 shortened as part of edit D. −15. (404 trimmed, 445 added.)

### Rejected, though I looked hard at it
**THE LAND PLAN's third number.** The plan's `P` is summed from printed powers and misses what the
frame computes: at deck152 **seq 16** `P=4` while `CRACK-BACK NEXT TURN … you would be at 8, and
lower still` (animatable Lair of the Hydra + Ranger Class), and at deck146 **seq 22** `P=4` with
`you would be at 5 - you control NO creature`. Re-keying `P` to the crack-back line is tempting and
I am not proposing it, because in **both** cases the land-kill was the best row on the menu — the
bodies cost 5 and 6 and the seat had 4 mana — so no decision the corpus shows going wrong pays for
the rule.

---

## (4) Optional proposals

**None.** The two items worth generalizing are engine work (the resolving-chain hold collapse; the
`YOUR PLAN:` label acceptance), not general-guide or skill text. I will not restate an existing
amendment as a proposal.

## What I did NOT check
- The opponent seats' JSONL beyond reading their board state out of my own rendered prompts, and the
  deck126 opponent file entirely (the Sanguine Bond/Exquisite Blood sequencing was read from my
  seat's game log).
- Whether the `sibling_window_asks_skipped` / `stop_reached_windows_skipped` zeros are a subset rule
  that never held, a stop never stated, or an unwired counter — I confirmed only that both read 0 on
  all six of my gameend records; I did not read the code. Engine seat's call.
- The 126 `hold_windows_skipped` in the seat: whether any skipped window would have changed a
  decision. Not audited.
- THEY COUNTER (wave-71 edit D, the log-keyed predicate): **UNTESTED** — no opponent this wave
  countered a spell of mine, and no `was COUNTERED by` line appears in any of my six logs. Two waves
  now with the edit unexercised.
- MULLIGAN/BOTTOMING: six keeps at 7, no mulligan and no bottoming window in the whole seat. That
  whole section is untested this wave and I edited none of it.
- The `askreplay/` sidecar (29 files in the run dir; 17 `ask_replays_reserved` in this seat) — I did
  not open it; it is the engine seat's instrument.
