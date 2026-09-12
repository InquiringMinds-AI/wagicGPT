# Wave 78 — deck 123 (Intruders of Thraben, WBU token combo) — per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-125420-final`, SINGLE binary
`archives/wagic-47ef378c8-w78step1` (source 7c0f15835), `--thinking on`, one run, no reruns.
Live guide = the wave-77 edition (installed 031db297b, byte-identical to `wave77/deck123/strategy.txt`).

**4 wins (vs152 t9 at 1/−46, vs146 t9 at 16/−8, vs162 t9 at 16/−4, vs130 t14 at 16/−76), 2 losses
(vs126 t12 at 0/32, vs125 t62 at 0/57)** — 2/6 → 4/6. Three of the four wins are the same turn-9
combo kill. Hands are real; I score none of the 2/6↔4/6 movement as guide effect and read the wins
only for what they prove about the wave-78 render changes.

Seat census: **571 decisions** (ask 365, priority 181, attackers 12, discard 10, blockers 2,
bottom 1; by game 125: 286, 126: 121, 130: 67, 146: 31, 152: 31, 162: 35), 100% native reasoning
(`reasoning_chars` median 6,100, max 17,847), `protocol_deviation` **compliant 571/571**, **0
`off_protocol_bytes`, 0 `plan_line_missing`, 0 `reply_truncated`, 0 `action_before_plan`, 0
fallbacks of any kind** — second wave running with a clean seat, and this wave the corpus's 8
fallbacks are all other seats. Latency p50 75.5 s, max 393.9 s; `transport` `curl=0,http=200,empty=0`
on 571/571; `deadline_pct` max 43.7. Prompt max **27,998 B** (`...deck125` seq 595), 124/571
over 20 KB — **all 124 in the vs125 game** (43% of that game's prompts; 0% in the other five).
Gameend sums for this seat: `ask_replays_reserved` 469, `identical_ask_answers_reserved` 329,
`hold_windows_skipped` 356 (cast 324 / priority 32), `hold_released_turn` 31,
**`stop_reached_windows_skipped` 44 — all 44 corpus-wide are mine**, `stop_reached_reputs_collapsed`
**0**, `stack_drain_windows_asked` 36, `own_loop_windows_asked` 0, `forced_close_events` 8 /
`forced_close_unrecorded` 4, `crossphase_identical_reputs` 53 / `_board_unchanged` 0,
`plan_names_uncastable_zone_card` **4** (wave 77: 36), `plan_names_stranded_card` 0, `async_drops` 10,
`own_turn_windows_skipped` 28, `reserve_decline_windows_noted` 14, `blocker_forecast_rows` 114,
`phase2_answer_recovered`/`_missing` 0/0, `main_phase_windows_skipped` 0 (sixth wave),
`mana_only_windows_skipped` 0, `repeat_annotated_takes` 0, `identical_option_asks_resolved` 0,
`declined_face_latches` 0.

**Wave-77 HIGH-1 (S2) is CLOSED at this seat.** The closing sentence now carves the count out —
`the ONE exception is a row whose name ends ", repeated then stop": on that row you also write the
count, as "<short name> x<N>"` (146 seq 15) — and the model wrote a count on **8 of 8** repeat rows
with no loop and no re-ask (146 seqs 14/15 x5/x18, 152 seqs 13/14 x20/x4, 162 seqs 32/34 x17/x3,
130 seqs 80/84 x5/x17). 0 `repeat_count_reask` corpus-wide; the 342-second refusal does not recur.
**Wave-77 MED-4 (S7) is CLOSED**: `hold_check_ref_seq` now resolves to a real decision record on
**519 of 519** brackets that carry one (0 dangling, 0 pointing at a non-decision), and
`hold_check_ref_window` carries the ordinal beside it.

## 1. Game by game — the deciding decisions

**vs deck152 — WIN, turn 9 (1 / −46).** The clean line: Doomsayer t6 (seq 8), Alarm t8 (seq 11),
then seqs 12/13/14 build 1 + x20 + x4 to M=26 against a stated `stop=26`, `ATTACK: none` at
**seq 15** citing CHECK -1 *by name* ("CHECK -1 applies; attacking the lone maker taps it and locks
the engine under Intruder Alarm"), and 27 attackers for 80 at **seq 41**. The seat took 1/16 life on
the way in and won anyway.

**vs deck146 — WIN, turn 9 (16 / −8).** Same shape: Doomsayer t6, Alarm t8 (seq 10), x5 then x18 at
seqs 14/15, `ATTACK: none` at **seq 16** under a stated `stop=26, M=1`, and 27 attackers for 28 at
**seq 43**. Note **seq 22**: with M=26 and the printed stop 26, the seat rebuilt the stop to 27 from
`21 life + 3 creatures + 3` and took one more Create row — the guide's own recompute rule, applied
without prompting, and it is the token that put the total over the line.

**vs deck162 — WIN, turn 9 (16 / −4).** Doomsayer t7, Alarm t8, x17 then x3 at seqs 32/34, 23
attackers for 24 at **seq 59**. The one discretionary call is **seq 35**: Tribute to Hunger on the
seat's own upkeep under `VERDICT: taking this row STRANDS Bloodline Keeper this turn` — an override
of the reserve rule, and the right one (it removed Fog Bank, whose text prevents all combat damage
dealt to and by it, i.e. the only body that could have eaten an attacker). Won by 4. Noted, not paid
(§4).

**vs deck130 — WIN, turn 14 (16 / −76).** Keeper t9 (seq 55), Alarm t10 (seq 61), Devour Flesh at
the opponent at **seqs 74/75** on N = 1 against **Dwarven Blastminer** — the land-destroyer, the card
that ground this seat out in wave 77 — for 1 life to them, which is the one shape the guide licenses
at the opponent. Then x5 and x17 at seqs 80/84, `ATTACK: none` at **seq 88** (lone maker, CHECK -1
honoured), Transform to Lord of Lineage at **seq 90**, 24 attackers for 97 at **seq 121**.
One wrinkle: at **seq 74** the PLAN line read *"Cast Devour Flesh on myself to gain 2 life and
sacrifice the token"* and the target ask one seq later answered **The opponent** — the answer was
right and the plan it was written under was not (LOW-6).

**vs deck126 — LOSS, turn 12 (0 / 32). The engine never started, and then the game was over.**
The opponent assembled Sanguine Bond t9, Exquisite Blood t11, Staff of Nin t13; from the first Staff
activation the chain is deterministic and the prompt says so (`LOOP COMPLETE`, and
`[LOOP RUNNING: ... Nothing on this list stops the chain]` on all 77 windows of that turn). Nothing
after **seq 89** could have changed the result. What could have: the seat had a Thraben Doomsayer
from t8 and a Bloodline Keeper from t10 and **made its first token at seq 189, at 1 life** — 49 of
50 Create rows declined across 13 turns. The three attack windows are why:
**seq 26** (t8) sends the LONE Doomsayer into `[their untapped blockers: Overgrown Battlement (0/4)
(neither dies)]` with the ATTACK TOTAL saying *"Their 1 untapped blocker ... can cover every attacker
you could send, so none of that damage is guaranteed"* — **0 damage dealt** (the trend line at
seq 33 reads `unchanged at 20 since turn 6`) and the maker tapped through the opponent's turn 9. The
reasoning never mentions CHECK -1 and asserts *"Opponent life goes to 18"* after having just worked
out that the Battlement blocks it. **seq 33** (t10) sends the lone Doomsayer again. **seq 50** (t12)
sends Doomsayer #1 *and* the Keeper, leaving both makers tapped through turn 13. Meanwhile
**seqs 37 / 49** decline live Create rows on the seat's own turn with the plan *"tap for tokens on
their turn"* — a turn the attack had already spent the maker on.

**vs deck125 — LOSS, turn 62 (0 / 57). The same failure, in slower motion.** Keeper t11 (seq 53),
Alarm countered (seq 58/61), Alarm again t13 (seq 81), a second Doomsayer, and **0 of 7 Create rows
taken in 286 decisions**. Seven consecutive windows state the deferral and never execute it —
seq 56 *"tap Bloodline Keeper for token"*, seq 61 *"Tap Keeper on opponent's turn to build tokens"*,
seq 72, seq 75, seq 76 *"wait until opponent's turn to tap"*, seq 80, seq 92 *"Hold priority until
opponent's turn, then tap Bloodline Keeper for a token to build toward the stop"* — and the two
attack windows in between spend the maker: **seq 77** (no Alarm out) and **seq 93**, where the Alarm
IS out, the A-row itself prints `[doesn't untap during its controller's untap step]`, and the
reasoning reads *"attacking with Bloodline Keeper deals 3 damage and keeps the engine going"* after
computing `L + C + 3 = 25` and `M = 1`. That is the guide's rule inverted in the model's own words.
From t20 on the seat holds 8-14 cards it cannot use against two Staffs of Nin and an Elixir.
One override I do not fault: **seqs 137/138** cast Devour Flesh naming **Yourself**, sacrificing the
only maker, with an uncounterable Supreme Verdict on the stack that was going to kill it anyway — the
guide's "never your only creature" clause has no carve-out for a resolving sweeper and the model
supplied one correctly.

**CHECK -1 scoreboard, this wave: 3 honoured of 7 lone-maker windows** (130 seq 88, 146 seq 16,
152 seq 15 — all three cite the rule or its arithmetic) **and 4 violated** (125 seqs 77/93,
126 seqs 26/33) — **and all four violations are in the two lost games.** Wave 77 was 3 of 3
honoured. The three honoured windows all sit on a 31-permanent board with a stop already stated;
all four violations sit on a 6-to-8-permanent board with no stop stated and an A-row advertising a
free hit (`no creature they control can block this attacker`, or haste+shroud from the Greaves).
**Edit 1 exists for this.** Across the two losses the seat took **1 of 57** Create rows; across the
four wins, **16 of 62**.

## 2. Engine / interface / card items

**HIGH-1 — THE STOP-REACHED COLLAPSE CANNOT FIRE ACROSS A TURN BOUNDARY, AND THAT IS WHERE EVERY
RE-PUT CHAIN LIVES: 44 stop-reached windows asked, `stop_reached_reputs_collapsed` 0, 90.8 MINUTES
OF DECODE, 40 OF THE 44 ANSWERED `pass`.** The wave-77 S3 shape RECURS in four chains:
`...vs-ai_baka_deck152.jsonl` **seqs 16-24** (nine consecutive windows, every one printing
`M=26, your stated stop=26, so this window would add to a count ALREADY AT OR PAST your own stop`,
every one answered `CHOICE: 0 (pass)`), `...deck162` **seqs 36-41** (six), `...deck146`
**seqs 17-22** (six), `...deck130` **seqs 91-99** (nine). Two findings, both checkable:

(a) **Which identity leg moved: none of the three. The gate that blocks the collapse is the STOP
STATEMENT'S TURN.** `w78StopReachedRePutCollapses` delegates to `w72StopReachedWindowCollapses`
(AIPlayerGPT.cpp:34029), which returns false unless `planTurn == nowTurn`. In deck152 the stop was
stated on **turn 8** (seq 13/14, `stop=26`) and all nine re-puts are on **turn 9** — no reply between
them restates a stop, so `mStatedStopTurn` stays 8 and both arms are dead for the whole opponent
turn. deck162 is identical (`stop=23` stated turn 9 at seq 34; re-puts on turn 10). deck130 has no
`stop=` anywhere in seqs 84-99 at all. The action key and the board key are irrelevant — the
collapse never gets as far as comparing them. The counter reads 0 because the arm is never reached,
not because an identity moved. **The render and the collapse disagree about the same number**: the
row prints `your stated stop=26` off the carried stop on turn 9 while the collapse refuses to use
that stop because it is a turn old. Repro: `...deck152` seq 17, row 1's `{right now:}` clause versus
`stop_reached_windows_skipped` 20 for that game (all 20 on turn 8, none on turn 9).

(b) **The collapse also requires the repeat row, and 40 of the 44 windows have no repeat row.**
CY F1's gate is "the ONLY new-action row is the stop-reached repeat row". Counting the 44:
**4 carry a `repeated then stop` row** (130 seqs 87/89/90, 152 seq 26) and **40 carry only the plain
`Create ... [cost: Tap]` row** — a maker at or past the stop on a turn it has not yet been activated
does not render a repeat row at all. `everyBaseRowStopPriced` was added for exactly this
(AIPlayerGPT.cpp:41700) and is correct; it is (a) that kills it.
Per-chain decode: deck152 16-26 = 1,474 s, deck146 17-22 = 866 s, deck162 36-41 = 978 s,
deck130 91-99 = 559 s; 44 windows = **5,448 s (90.8 min)**, answers `pass` 40, `Hold` 1, an action 3.

**HIGH-2 — THE HOLD LATCH DOES NOT HOLD WHEN THE BOARD IS THE THING THAT MOVES: 87 HOLDS TAKEN, 56
FOLLOWED IMMEDIATELY BY A BYTE-IDENTICAL LIST AT THE SAME SEAM.** Repro:
`...vs-ai_baka_deck126.jsonl` **seq 93** answers `CHOICE: 2 (Hold priority)`; **seq 96** is the next
priority window at that seam and its `options_text` is **byte-identical** to seq 93's, and its own
bracket says so: `[hold check: every row above was also on the menu at the last window I asked you at
this seam (6 windows in a row now), and no row that was on it is gone - a hold taken here holds until
one of them appears, disappears, or names a different card, cost or target]`. The full-prompt diff
between 93 and 96 is three things and no row: opponent life 13 → 14, a new game-log line, and the
stack's top object. The seat then held again at 98, 100, 109, 120, 121, 122, 123 — **28 holds in one
turn, 0 of them skipping anything** (`hold_windows_skipped_priority` is 0 for that whole game).
Same pattern in the vs125 game (29 of 53). **The hold-check sentence is therefore a render clause
stating a false fact**, and it is false in precisely the situation it is printed for.

**HIGH-3 — ONE TURN, 77 WINDOWS, 1 HOUR 47 MINUTES OF DECODE, ON A CHAIN THE PROMPT ITSELF SAYS
NOTHING CAN STOP.** Repro: `...vs-ai_baka_deck126.jsonl` **seqs 89-191**, the opponent's turn 13.
Every one of the 77 windows prints `LOOP COMPLETE: BOTH halves of a life LOOP (Sanguine Bond +
Exquisite Blood) are on THEIR battlefield right now` **and**
`[LOOP RUNNING: both halves of the opponent's life LOOP are on the battlefield and the chain is live,
so this same question is put to you again for every link of it, with only its numbers moved. Nothing
on this list stops the chain.]`. The seat's 77 answers are 28 `Hold priority`, 25 `Cast nothing right
now`, 23 `pass`, and one Create at 1 life. **28 windows at the ask seam (seqs 89-163) are
byte-identical in `options_text`.** Decode 6,404 s. This is HIGH-2's mechanism at its worst: the
looping life total is part of the ask-cache board key, so every link mints a fresh key. Concrete
fix direction: when `LOOP RUNNING` is printed and the seat has already declined this exact option
set this turn, the board key for that seam should exclude the two life totals and the loop's own
stack objects — the engine already knows the chain is live, it prints so.

**MED-4 — THE DRAIN ANNOTATION PROMISES A HOLD THAT THE LATCH DOES NOT KEEP.** 29 prints, all TRUE
about the stack (`[their stack is draining 2 triggers - each link will put this same list to you;
HOLD (Hold priority) covers every link. The rows above are what is legal NOW - this says nothing
about what will still be legal after their stack resolves]`; N = 2 against exactly 2 opposing
Staff of Nin triggers at `...deck125` seqs 360/379/395/412/450 — S4's "stays available after"
promise is GONE corpus-wide, 0 occurrences). But the clause's own advice fails: `...deck125`
**seq 379** takes the hold and **seq 382** is the byte-identical list again; same at 395 → 398,
539 → 541, 541 → 544. The N and the wording are right; `HOLD ... covers every link` is not.

**MED-5 — THE SINGLE `[cost: Tap]` CREATE ROW IS A 49-WINDOW NO-OP THAT NOTHING COLLAPSES.** Repro:
`...deck126` **seqs 76-188** — the same one-row-plus-hold menu 47 times, with
`{if you pass here, this option is not offered again until the board changes}` printed from seq 98
onward while the board changes at every link of the opponent's loop. The promise is true only of a
board that stands still, and the row it is printed on is the one row the deck exists to take. The
useful annotation here is not the promise but its negation: name what the row is worth NOW
(`M=3, your stop=17` was computable at seq 91 and is not printed, because the seat never wrote a
`stop=` line that turn).

**MED-6 — `crossphase_identical_reputs` 53 WITH `_board_unchanged` 0, AND THE 53 ARE REAL.** 53
`[this exact list was put to you N windows ago ...]` brackets, all printing `the board has moved
since then, but this list has not`; 0 print `nothing on the board has changed`, consistent with the
corpus-wide 4 being other seats. R8 holds a third wave. Recorded because it is the same root as
HIGH-2/3: the board is what moves, and every dedupe in this engine is keyed on the board.

**LOW-7 — A TWO-STEP TARGET ASK LOSES THE PLAN'S TARGET.** `...deck130` **seq 74** writes
`PLAN: Cast Devour Flesh on myself`; **seq 75** is the target ask and answers `2 (The opponent)`
with a plan line that names no target at all. The answer was correct and the stated plan was not, so
nothing was lost here — but the cast row's plan is where the target intent is written and the ask one
seq later neither carries it forward nor contradicts it.

**Render clauses I checked and found TRUE.**
- **S8 — the `{effect:}` gloss: PASS.** 25 distinct glosses across 571 prompts, **0 mid-clause cuts**;
  all four `(...more)` cuts land after a complete clause or sentence (`-- Equip {0} (...more)`;
  `When Briarbridge Tracker enters, investigate. (...more)`; `you may pay {1}{W} any number of times.
  (...more)`; `look at target opponent's hand. (...more)`). **Howling Mine now renders whole**
  (`At the beginning of each player's draw step, if Howling Mine is untapped, that player draws an
  additional card.`, 33 prints) — S8's named regression is fixed. **Brutal Cathar did not appear in
  any of my six games: S8's Cathar half is UNTESTED at this seat.**
- **S7/S9 — the hold-check bracket: PASS.** 537 brackets; 18 print the first-window sentence and all
  18 carry `hold_check_ref_seq: -1`; of the 519 with a referent, **519 resolve to a real decision
  record** (0 dangling). 380 carry a `gone` clause and 347 say `no row that was on it is gone`. Of
  33 numeric `N gone` claims I could recompute, 25 matched my key exactly and the 8 that did not are
  Tragic Slip rows whose target roster changed (`126` seq 66/190, `162` seq 21/25) — a row "naming a
  different target", so the engine is right and my brace-stripped key is the coarse one.
- **S12 — `Opponent life trend`: PASS on the sample I checked.** 79 of 563 lines print the split;
  484 are net-only. `126` seq 96 reads `now 14 (-6 since turn 10; over those turns life-gain EVENTS
  put +1 on them and life-loss EVENTS took -7 off)` against a log of −2 (t12 combat), −5 (t12
  combat) and +1 (Exquisite Blood) — exact.
- **S4 — the drain annotation's arithmetic: PASS** (see MED-4 for the half that is not).
- **R7 / ATTACK TOTAL: PASS.** 146 seq 43 (27 attackers, 28 power, `At least 24 damage lands`),
  152 seq 41 (27 / 80 / `At least 71`), 162 seq 59 (23 / 24 / `At least 24`), 130 seq 121
  (24 / 97 / lethal at −76) — every floor recomputes exactly, and every one of the four is the
  window that won a game.
- **R6(a) crack-back cover: PASS.** `130` seq 74's `{crack-back cover: ... 1 - 1 = 0 is the MOST that
  can be left -> you could still be as low as 17, which you SURVIVE}` is arithmetically exact.
- **The reserve-VERDICT rule holds a third wave: 15 renders, 14 declined**, the one override
  (162 seq 35) correct. `plan_names_stranded_card` 0.
- **`plan_names_uncastable_zone_card` 36 → 4**, and the 4 are all in the 286-decision vs125 game.
- **Harness: clean.** `transport` `http=200` on 571/571, `deadline_pct` max 43.7, 0 truncations,
  8 `forced_close` records (all arm `cast`: 125 seqs 120/121, 126 seqs 141/142, 130 seqs 76/77/81/82)
  against `forced_close_unrecorded` 4.

**UNTESTED from this seat (0 renders in 571 decisions):** S5 conditional-counter clause
(`{your <counter> already targets this ...}` — this deck runs no counterspell), S6
`[own loop verdict:]` (`own_loop_windows_asked` 0), S10 `defer` / X-announcement (0 X menus, 0
`defer` records), S13's hoisted shared card text on a shared-SOURCE **target** menu (72 prompts
carry `Card text shared by options N-M`, all on equip/activation menus, and the hoisted header never
appears inside a row), S14 `[X pricing]` (0) and loyalty rows (0), S15 folded no-op bands (0
`folded` strings; the Intrepid Adversary menu in `152` was 15 prints of a creature gloss, not a
21-row menu), S1 `answer_label_absent` (0 at this seat), `[RE-ASK]` (0), `DRAW PRICE` (0),
MDFC faces (eighth wave with none in a deck123 hand), `identical_option_asks_resolved` (0),
`mana_only_windows_skipped` (0), `repeat_annotated_takes` (0).

## 3. Which wave-77 items recur

| wave-77 item | wave-78 status |
|---|---|
| **HIGH-1 / S2**, the repeat row vs the closing sentence | **CLOSED.** The carve-out prints (146 seq 15); 8 of 8 repeat takes carry a count; 0 `repeat_count_reask` corpus-wide; max `reasoning_ngram_repeat` at this seat is unremarkable and the 342 s refusal does not recur. |
| **HIGH-2 / S3**, the stop-reached row re-offered 12× | **RECURS, four chains, 90.8 min** — and the collapse shipped for it never fires. Root cause named in HIGH-1 above: `planTurn == nowTurn`. |
| **MED-3**, the whole-turn passive run | **WORSE where the board moves** (126: 28 byte-identical ask windows in one turn; 125: 15) and **gone where it does not** (146/152 longest identical run = 1). HIGH-2/HIGH-3. |
| **MED-4 / S7**, `hold_check_ref_seq` as an ordinal | **CLOSED.** 519/519 referents resolve; `hold_check_ref_window` carries the ordinal. |
| **MED-5 / S8**, the gloss cutting mid-clause | **CLOSED at this seat** (0 of 25 distinct glosses cut mid-clause; Howling Mine whole). Brutal Cathar absent — UNTESTED. |
| **MED-6**, `plan_names_uncastable_zone_card` carrying no signal | **36 → 4**; the measure is now small enough to be worth reading. |
| **LOW-7**, the edict's opponent branch misread | **Not re-observed.** The three edicts cast this wave (130 seqs 74/75 at the opponent on N=1; 125 seqs 137/138 at Yourself; 162 seq 35 Tribute) are all guide-legal or correctly-overridden, and no PLAN claims the seat gains life the opponent gains. |
| **wave-77 edit 1** (CHECK 1 A naming the removal that qualifies) | **UNTESTED.** No Perimeter Captain or Pride Guardian in any of the six games (0 prints); the wall that did appear, Overgrown Battlement, is not named by the entry — but CHECK -1 answers that window first, so I do not pay a second rule for it (§4). |
| **wave-77 edit 2** (the hold bullet, run-ending) | **TESTED AND MIXED.** Where the rows are genuinely dead the seat now takes the hold (87 holds vs 0 last wave in the win games' shape) — but the latch does not keep it (HIGH-2), so the run does not end. The edit did what a guide can do; the rest is engine. |
| CHECK -1 (the lone maker) | **REGRESSED: 3 of 7, from 3 of 3.** All four failures in the two losses. **Edit 1 below.** |
| the Alarm-on-an-empty-board cast (`of which 0 are creatures`) | **Does not recur** — 0 Alarm casts onto a creature-less board this wave. |
| the `copy N of N` / `never a Damnation` discard contradiction | **Not re-triggered** (10 discard asks, no Damnation in any of them). |
| — new — | the collapse's turn gate (HIGH-1), the hold latch vs a moving board (HIGH-2), the 77-window loop turn (HIGH-3), the drain clause's false hold promise (MED-4). |

## 4. Guide verdict: **EDIT**

Revised guide at `wave78/deck123/strategy.txt` — **19,978 bytes** (live guide 19,995; ceiling 20,000).
The guide **shrinks by 17 bytes**; no rule earns new bytes on net. A332 grep (`seq`, `wave`,
`corpus`, `game N`, deck numbers, turn citations): 0 hits. `YOUR PLAN`: 0. A334 audit
(`even when` / `even if` / `regardless of` / `no matter` / `however` / `although`): 0. Both PLAN
templates are one physical line each. Headings unchanged, each declaring its own scope (A333).

| # | before → after | seq that paid for it |
|---|---|---|
| **1** | **CHECK -1's reason no longer depends on the Alarm, and it prices the tap the model keeps treating as free.** `Attacking TAPS it, and with the Alarm out / nothing untaps until SOME creature enters - with the maker spent that is their choice. The sole / override is a total the row itself calls lethal.` → `Attacking TAPS it, and a tapped maker makes / no token on THEIR turn - the turn the tokens are for; with the Alarm out it does not untap at all / until SOME creature enters, and the maker is spent. Two damage is not this deck's clock. The sole / override is a total the row itself calls lethal.` The old reason was scoped to a board with the Alarm out, and three of the four violations are on boards without it; the new first clause is true on every board and names the thing the attack actually spends. The last sentence answers the bait directly — a positive statement of what the deck's clock is, not an exception to the free hit. **+102 bytes.** | **125 seq 93** (Alarm out, the A-row printing `[doesn't untap during its controller's untap step]`, the reasoning reading *"attacking with Bloodline Keeper deals 3 damage and keeps the engine going"*), **126 seq 26** (lone maker into a 0/4 wall, **0 damage**, maker tapped through their turn 9), **126 seq 33**, **125 seq 77**. 4 of 7 lone-maker windows, all four in the two losses. |
| **2** | **CHECK 3's "keep one maker home" stops being conditional on the Alarm.** `- KEEP ONE UNTAPPED MAKER HOME, every attack, while the Alarm is out: a tapped creature untaps / only when a creature ENTERS, which only an untapped maker reliably causes.` → `- KEEP ONE UNTAPPED MAKER HOME, every attack: it makes your token on their turn, and with the / Alarm out a tapped creature untaps only when a creature ENTERS, which a maker causes.` The rule's reason is stated first and holds on any board; the Alarm clause becomes the second, sharper reason rather than the gate. **+10 bytes.** | **126 seq 50** — both makers sent on t12 with no Alarm out, leaving the seat with no untapped maker through turn 13, the turn the opponent assembled the kill; the first token of the game came at seq 189 at 1 life. |
| — | **Paying for edits 1 and 2 (−129, no rule removed):** (a) `With lethal listed, do not fire at all.` deleted from TIMING THE CHAIN — it restates `LETHAL IS ALREADY ON THE BOARD` at the head of the guide, which A333 makes that heading's job (−40). (b) `"a second copy changes nothing", ` deleted from THE PRINTED STRINGS — the string prints in **0 of the 42 logs** in this corpus (−33). (c) `A take with no number is re-asked and the window spent.` deleted from the repeat-row bullet — the repeat row's own bracket now prints that sentence verbatim, and so does the window's closing sentence (146 seq 15); restating a printed string is the noise A332 names (−56). | — |

**What I deliberately did NOT change.**
- **CHECK 1 A's two proper names.** The wall the seat actually met (Overgrown Battlement, 0/4
  `[defender]`) is not named by the entry, so wave-77's edit 1 never fired — but CHECK -1 answers
  126 seq 26 before CHECK 1 is reached, and edit 1 above fixes CHECK -1. Generalising the entry to
  any `[defender]` would also widen a hole the rule already has (a wide non-lethal attack into one
  wall should still go), on evidence that contains no such board.
- **The reserve-VERDICT rule** (162 seq 35 overrode it and won by 4). A rule is rewritten by a
  decision that went wrong.
- **`never your only creature` in the edict entry** (125 seqs 137/138). The override was correct
  under an uncounterable Supreme Verdict already on the stack; writing that carve-out costs bytes I
  need for edit 1 and licenses the sacrifice on every board where the model *thinks* a sweeper is
  coming.
- **The Greaves re-equip rule.** 126 seq 64 moved the Greaves off its attacking Doomsayer onto a
  summoning-sick one — licensed by `a new maker arrived bare` and wasteful, but it cost nothing
  measurable (the game was the loop two turns later).
- **The stop-recompute bullet.** 146 seq 22 rebuilt the stop from L and C unprompted and that token
  is what carried the total over the line; the rule works as written.

## 5. Optional proposals

**None.** Both edits are this deck's own wording. HIGH-1 through MED-6 are engine items; LOW-7 is a
render observation. A general-guide or skill proposal restating A332/A333/A334 would be noise.

## 6. What I did NOT check

- **Opponent seat files were opened for board context only** (deck126's Sanguine Bond / Exquisite
  Blood / Staff of Nin line; deck125's Supreme Verdict, Staffs of Nin and Elixir; deck130's Dwarven
  Blastminer; deck152's Sigarda board; deck146's Triumphant Adventurers; deck162's Fog Bank).
  **I audited no opponent decision.**
- **I adjudicated no lane prediction corpus-wide.** §2 answers S2, S4, S7, S8, S9, S12 and the R6/R7/
  R8 family **for this seat's six games only**; S1, S5, S6, S10, S13's target-menu half, S14 and S15
  had zero renders here and are listed UNTESTED rather than PASS.
- **The 10 `async_drops` were not traced to re-asks** (no record kind carries them), and the 4
  `forced_close_unrecorded` were not reconciled against stderr — no stderr file was opened.
- **The 53 cross-phase re-puts were not individually paired to their answers**; I established the
  print rate and read the shape.
- **`askreplay/` was not opened**, and the 31 `hold_released_turn` events were not paired to their
  holding windows.
- **HIGH-2's mechanism is named from one full-prompt diff (126 seq 93 vs 96) plus the counter
  `hold_windows_skipped_priority` 0.** I read `w72StopReachedWindowCollapses` and its call site for
  HIGH-1 and quote the line numbers, but I did **not** read the hold-latch or ask-cache key
  construction, so "the moving life total is part of the board key" is my inference from the diff
  and the counters, not a reading of that code.
- **I read 4 of 4 `plan_names_uncastable_zone_card` records**, all 25 distinct `{effect:}` glosses
  (by distinct string, not all 1,600 prints), 33 of 380 `gone` claims, and the `reasoning` of every
  window cited above; I did not read the `reasoning` of the 356 held windows or of the 77-window
  loop turn beyond the answers.
- **I did not open any card script**, run a build, run wagic, or run git; nothing under `bin/Res`,
  `src/` or outside `wave78/deck123/` was written.
