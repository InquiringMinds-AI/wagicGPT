# Wave 80 — deck 123 (Intruders of Thraben, WBU token combo) — per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final`, MIXED binary: five of my six
games on `archives/wagic-a7a6b8942-w80step1` (source cf72a002f); **`162v123` is the RERUN on
`archives/wagic-ba5cdc374-w80step1b` (337becbf2, + lanes DI/DJ)** after attempt 0 hung. Live guide =
the wave-79 edition (installed 36ae77199, byte-identical to `wave79/deck123/strategy.txt`).

**1 win, 5 losses** (wave 79: 0/6). Seat census: **392 records / 358 model calls** (ask 304,
priority 38, hold_event 28, forced_close 6, discard 7, attackers 5, blockers 3, reveal 2; by game
125: 181, 162: 74, 152: 60, 130: 35, 146: 34, 126: 26). `protocol_deviation` **compliant 358/358**,
**0 fallbacks of any kind, 0 `plan_line_missing`, 0 `off_protocol_bytes`, 0 `reply_truncated`,
0 `action_before_plan`**; `thinking` **on 358/358**, `reasoning_chars` median 5,774 / max 19,629 /
**zero 0**; `transport` `curl=0,http=200,empty=0` on 358/358, `deadline_pct` max 30.7. Latency p50
74.0 s, p95 193.6, max 276.7; 8.9 h of decode. Prompt mean 12,405 B, **max 22,578, 14 over 20 KB**
(wave 79 at this seat: max 20,286, 1 over) — MED-4 below. Gameend sums: `ask_replays_reserved`
**346** = `ask_replays_cache` 345 + `_repeat_latch` 0 + `crossphase_replayed` **1** (U9 identity
holds), `crossphase_board_unchanged` **0** (wave 79: 22 of the corpus's 26),
`crossphase_identical_reputs` 58, `ask_key_continuation_differs` 12, `hold_events` **28** =
`hold_verdict_safer_ignored` 12 + `hold_reopened_new_lethal` **1** + `hold_reopened_new_threat` 0 +
15 `reopen_rows_moved`, `hold_windows_skipped` 190 (cast 190 / priority 0), `hold_released_turn` 17,
`crackback_verdict_lines_rendered` **91** / `crackback_lethal_blocked_away` **19** /
`stack_death_verdict_lines_rendered` **9**, `own_loop_verdict_lines_rendered` **0** /
`own_loop_windows_asked` **0**, `plan_cast_steps_closed` 42, `menu_single_outcome` 0 /
`single_outcome_rows_spared` 0, **`cached_replay_reasked` 0**, `stop_reached_windows_skipped` 1 /
`_reputs_collapsed` 0, `plan_names_uncastable_zone_card` 11, `async_drops` **0**,
`forced_close_events` 6 / `_unrecorded` 3, `stack_drain_windows_asked` 9, `main_phase_windows_skipped`
1, `own_turn_windows_skipped` 50, `mana_only_windows_skipped` 0.

**THE DECK ASSEMBLED ITS ENGINE IN TWO GAMES THIS WAVE AND WON ONE OF THEM.** Intruder Alarm reached
the battlefield beside a live maker in vs146 (won) and vs152 (thrown away two windows later). In the
other four it was either never castable (162 offered it once, 130/125 drew it with every maker dead)
or cast onto an empty board (126). The three deciding decisions of the wave are all in the two games
where the engine existed, and all three are the guide losing at a window it already covers.

## 1. Game by game — the deciding decisions

**vs deck146 — WIN, turn 18 (11 / -2). The engine ran and the wave-79 stop edit is why.**
Doomsayer t6 (seq 6), Keeper t8 (seq 8), Damnation t10 and t14 (seqs 10/15, both `THEIRS 2 / YOURS
0` — the P0 entry fired correctly on the board it is written for), **Intruder Alarm t14 (seq 27)**,
Doomsayer + Greaves + equip t16 (seqs 28/29/30). Then the loop: seq 31 takes the single `[cost: Tap]`
Create row, **seq 32 takes the repeat row with `PLAN: stop=16; M=2; this window x16`** -> 18 creatures,
seqs 33/35 pass at `stop=23, M=18`, and **seq 36 at the END step writes `stop=23, M=18; this window
x5` — the subtraction exactly — and takes `x5`** -> 23 creatures. Attack at t18 (seq 41) with 16
Humans plus the rest: opponent 19 -> -2. **This is wave-79 edit 3 paying in full**: the seat wrote
the difference into the PLAN line and then executed it, where wave 79's seat wrote a count by feel
(`x3` under `stop=24, M=2`) and lost the turn.

**vs deck152 — LOSS, turn 17 (-2 / 20). The engine was online and the seat dismantled it in two
windows.** Tribute t6 (seq 41) and Keeper t8 (seq 44) are correct; **Intruder Alarm t10 (seq 50)**
with the Keeper out is P3 read correctly. Keeper blocks Katilda t11 (seq 69), Slip kills Sigarda
(seqs 71/72). At **t12 upkeep (seq 75)** the seat taps the Keeper for a vampire — the Alarm untaps it
— and at **seq 80** the menu offers `Create vampire with Bloodline Keeper, repeated then stop` with
`{right now: M=2, your stated stop=24, so this window has 22 to add before it reaches your own stop}`.
**The seat answered `CHOICE: 0 (pass)`.** Its reasoning is the whole miss and is quotable:

> "Since I don't need to make tokens yet (M=2, stop=24), and I have a land to play, I should pass
> priority in Upkeep." ... "Is there any reason to tap Bloodline Keeper now? No, tokens are summoning
> sick anyway, and Intruder Alarm untaps them. Tapping now just delays the token by a turn."

Both halves are wrong against the guide: the stop is a CEILING to build to, not a threshold the seat
may decline to approach (`Below it, take the line every time it returns`), and Intruder Alarm untapping
the maker is the reason the run is FREE, not a reason to defer it. Then at **seq 82**, in the main
phase it had deferred to, it **cast Damnation destroying its own Bloodline Keeper and the token** —
the row's `M of yours` list reads `YOURS: Bloodline Keeper (3/3) ..., Vampire (2/2)` and the marker
names the Keeper as a `TOKEN ENGINE`. Its reasoning is a survival computation ("I lose. So Damnation
NOW is mandatory"), never a priority read — **wave-79 edits 1 and 2 did not reach it.** 22 free 2/2
fliers at seq 80 both beat the crack-back it was afraid of and killed a 20-life opponent on t13.
Edits 1 and 2 below are paid here.

**vs deck162 — LOSS, turn 13 (0 / 26). THE RERUN. The hung state never arose.** Full answer to the
brief's five questions, in order.
- **Did the Doomsayer + Alarm engine come online? NO.** `Intruder Alarm` sat in hand from seq 11 (t8)
  to the end and was **offered a cast row exactly once in the whole game — seq 97, row 3** (t12 Main
  phase 1, 5 sources including Underground Sea, hand line `Intruder Alarm {2}{u} [enchantment]
  [castable now]`, a Thraben Doomsayer untapped on the battlefield). **The seat took row 5, a second
  Thraben Doomsayer**, which left 2 sources — not enough for the Alarm. That is P3 broken at the only
  window the deck's engine was ever available. No Alarm reached the battlefield, so the
  Doomsayer + Intruder Alarm cached-replay state **never occurred**: `cached_replay_reasked` 0 at
  this seat is **UNTESTED, not a FAIL**.
- **Were the Create rows re-asked?** Yes, and every one is a fresh model call, not a replay. The
  `Create human with Thraben Doomsayer #1 [cost: Tap]` row was put to the seat at **seqs 82, 86, 108,
  112, 114, 116, 128, 131, 133, 134, 135** (t12 upkeep through t13 main 1) plus seq 41 where it was
  taken; each carries its own `latency_ms` (29-90 s) and its own reasoning, `ask_replays_cache` 66 in
  this game contains **no** Create row. **The engine did not re-serve a cached activation once.**
- **Did the seat state its `stop=`? NO — not once in the whole game.** `stop=` appears in 0 of the 74
  records. The one Create actually taken, **seq 41**, carries `PLAN: Tap Thraben Doomsayer to make a
  token in upkeep. In main phase, cast Thraben Doomsayer or Idyllic Tutor to advance board, and hold
  Intruder Alarm until I can produce {U} mana.` — **a stopless PLAN on a `[cost: Tap]` Create row,
  the same shape as the answer the hung binary replayed 3,674 times.**
- **Tokens per turn: one, on turn 10.** Total tokens created in the game: 1.
- **Guide question — does `stop=<L+C+3>` give a stop at the FIRST Create window? NO, and this is
  measurable across the whole seat.** Windows offering ONLY the single `[cost: Tap]` Create row:
  **17** (162 seqs 41/82/86/108/112/114/116/128/131/133/134/135; 152 seqs 48/75; 146 seqs 31/37; 125
  seq 233). **`stop=` written on 0 of the 17.** Windows offering the `repeated then stop` row: 4
  (146 seqs 32/33/35/36 plus 152 seq 80, 125 none) — **`stop=` written on 4 of 5.** The stop rule as
  written is read as belonging to the repeat row: `TAKE THE REPEAT ROW ... with N = (L + C + 3) - M`
  sits immediately above the PLAN template, and the governance sentence that does cover the single
  row (`EVERY Create row is governed, the "[cost: Tap]" one included`) is a prohibition on exceeding
  the stop, not an instruction to state one. **Edit 3 below is paid by this.** It matters beyond
  tidiness: lane DI's re-ask (`a replay never passes a stated stop`) has nothing to compare against
  when the PLAN carries no stop, so the only thing standing between the seat and another runaway is
  the once-per-key bound.
- **What actually lost the game.** Two decisions before any of the above. At **seq 6/7 (t6)** the seat
  cast Devour Flesh at THE OPPONENT against their lone `Shield Sphere (0/6) [defender]`; the row said
  so verbatim — `{right now: they control 1 creature - Shield Sphere (0/6) [defender] is sacrificed,
  they gain 6 - the sacrificing player gains, not you}`. It killed a wall that threatened nothing and
  **moved the opponent from 20 to 26, where they stayed for the rest of the game.** The guide covers
  it exactly (`At N = 1 a "[defender]" or power-0 body is a decline` / `Never at the opponent at or
  below the life it gives them`) and it was read wrong. From t10 the opponent's Underworld Dreams x2 +
  Fate Unraveler under Howling Mine + Dictate of Kruphix billed the seat 6-9 life **per its own
  compulsory draw step**; it went 15 -> 9 -> 1 and died in the t13 draw step with the engine still in
  hand. At **seq 79** it sacrificed a Doomsayer to its own Devour Flesh for 2 life — forced (it needed
  2, and the 1/1 Human gains only 1), and it correctly picked the tapped copy over the untapped one.
- **Attempt 0 / harness (DJ).** I read `game-162v123-1789216936-HUNG-attempt0.stderr` only for the
  loop signature; the partial seat logs are gone, so nothing in §1 is derived from it. The rerun
  game's own log is clean: 74 records, gameend present, 0 fallbacks, `http=200` on 44/44 calls.

**vs deck125 — LOSS, turn 41 (0 / 26). The seat's worst game, and a three-window chain at t28.**
A Doomsayer at t22 (seq 143) and a Bloodline Keeper at t26 (seq 190) were both **COUNTERED by Essence
Scatter** (narration, quoted under HIGH-3). A third Keeper resolves t28 (seq 209) with the Greaves on
it (seqs 210/221 — haste + shroud, correct per the guide). Then:
- **seq 233**: the `Create vampire with Bloodline Keeper [cost: Tap]` row is offered and the seat
  answers `CHOICE: 0 (pass)` — `PLAN: Pass to attack with Bloodline Keeper in combat, then tap it in
  second main phase`. No `stop=`.
- **seq 234**: `ATTACK: Bloodline Keeper` — the lone maker, a flat CHECK -1 violation, and it taps it.
- **seqs 238/239**: in the second main phase it casts Devour Flesh naming **itself**, and the only
  creature it controls is the tapped Keeper. The cast row printed the answer —
  `{right now: they control 0 creatures - at 0 this does nothing; YOU control 1 creature - targeting
  yourself sacrifices Bloodline Keeper, and you gain 3}` — and the seat took it for 3 life at 8 life
  against a 26-life opponent. The guide forbids it in those words (`give up a token, never a maker,
  and never your only creature: an empty battlefield turns the Alarm and the Greaves in your hand
  into dead cards`). **It controlled no creature for the remaining 13 turns** and lost 8 -> 0 to a
  Staff of Nin pinging once a turn, holding Intruder Alarm, Lightning Greaves and Damnation (discarded
  to Cleanup at seq 370). Edits 2 and 4 are paid here.

**vs deck130 — LOSS, turn 24 (-1 / 18).** Doomsayer t9 (seq 25) and Doomsayer t11 (seq 27) both die
before an untapped, non-sick window — **0 Create rows are offered in the whole game**. Damnation t15
and t17 (seqs 41/43, `THEIRS 1 / YOURS 0` and `THEIRS 4 / YOURS 0`) are both correct. Idyllic Tutor
t19 finds the Alarm (seq 45/46) with no maker out — a live bracket, so the guide's P4 permits it, and
it is dead weight all the same. Keeper t23 at 2 life (seq 49) is too late. Hands are real.
**Nothing paid.**

**vs deck126 — LOSS, turn 11 (0 / 38).** The opponent had **Sanguine Bond down on t7 and Exquisite
Blood on t9**; the loop was assembled before the seat had a creature, and Tribute to Hunger at their
t11 ate the Bloodline Keeper cast at seq 28. Its one violation is **seq 25 (t8 Main phase 2): Intruder
Alarm cast onto `Your battlefield (4 permanents listed, of which 0 are creatures, ...)`** — the
guide's `of which 0 are creatures` rule broken again, and free again (nothing else was castable, and
the loop was already lethal). **Nothing paid.**

**Scoreboard.** Makers cast across six games: 12. Makers that reached an untapped, non-sick priority
window: **5** (146 x2, 152, 162, 125). Intruder Alarm resolved: **3** (146 t14, 152 t10, 126 t8 on an
empty board). Games in which a maker and the Alarm were on the battlefield together: **2**, won 1.

## 2. Engine / interface / card items

**HIGH-1 — THE CRACK-BACK VERDICT ASSERTS SURVIVAL ON A BOARD WHOSE OWN DRAW FORECAST, IN THE SAME
PROMPT, SAYS THE SEAT IS DEAD. 20 of the 80 crack-back-verdict prompts at this seat.** DH F9 folded
COMPULSORY draw damage into the BLOCKERS verdict; the crack-back verdict did not get the same fold.
Repro `1789241587-ai_baka_deck123-...-vs-ai_baka_deck162.jsonl` **seq 97**, two lines of the same
prompt, ~30 lines apart:

> `DRAW FORECAST: your next draw step draws 3 cards (1 + Howling Mine 1 + Dictate of Kruphix 1) =
> 3 x 3 = 9 life LOST BY YOU to their punishers above - if it resolves as forecast you would be at
> -8; that KILLS you. This draw step is COMPULSORY - no row on any menu declines it.`

> `[crack-back verdict: their UNBLOCKED total reaches your life total, but a legal block prevents it -
> with your best block: 0 damage still gets through, leaving you at 1. So passing this window does
> NOT hand them the game: you still get the blockers decision on their turn, and that assignment is
> what this figure is computed from.]`

"Passing this window does NOT hand them the game" is FALSE on that board: the seat never reaches their
blockers decision, because its own draw step kills it first — which is exactly what happened at t13.
The same pair prints at seqs 76, 77, 98, 108, 110, 112, 114, 115, 116, 118, 128, 129, 131, 132, 133,
134, 135, 137, 138 (the `you survive` short form at 76/77, the `best block` form at the rest). The
verdict line is the most authoritative-sounding thing on the window and the trust doctrine says the
model follows it. **Fix direction: the crack-back verdict's survival conclusion subtracts the
COMPULSORY draw-step cost that is already computed two lines above it, or it states no survival
conclusion at all when a DRAW FORECAST on the same prompt says KILLS.** Free cross-check: at seq 97
the seat's reasoning reads *"This implies I \*can\* survive if I block"* — it took the verdict.

**HIGH-2 — THE `TOKEN ENGINE` MARKER IS ABSENT FROM THE EDICT CAST ROW AND FROM THE PLAYER-TARGET
ASK, AND PRESENT ONLY ON THE CREATURE-PICK ASK — i.e. it renders where the pick is a real choice and
NOT where the victim is forced.** Near-A/B inside my own six games:
- **PRESENT**, vs162 **seq 79** (3 creatures, a genuine pick): `Thraben Doomsayer #1 (2/2) [your
  battlefield] [tapped] - "..." [you SACRIFICE this; you gain 2 life (its toughness); THIS IS NOT
  JUST A BODY: TOKEN ENGINE (it makes more permanents, one per activation)]`
- **ABSENT**, vs162 **seq 78**, the player-target ask one window earlier: `Yourself (player, life 8)
  {right now: you sacrifice one of your 3 creatures - YOUR pick - and you gain its toughness, between
  1 and 2}` — no engine named.
- **ABSENT**, vs125 **seq 238**, the CAST row, one creature controlled: `Cast Devour Flesh {1}{b}
  {right now: they control 0 creatures - at 0 this does nothing; YOU control 1 creature - targeting
  yourself sacrifices Bloodline Keeper, and you gain 3}` — it NAMES the Keeper and does not mark it.
- **ABSENT**, vs125 **seq 239**, the forced player-target ask: `Yourself (player, life 8) {right now:
  you sacrifice Bloodline Keeper and you gain 3 life (its toughness)}` — still unmarked, and this is
  the last window before the seat's engine leaves the board for good.
The engine detector plainly exists and is correct (U7 below). It runs on board-sweep rows and on the
per-creature victim ask; it does not run on the row that decides the cast. **Fix direction: run the
same test over the name the row already resolves (`targeting yourself sacrifices <name>`) and mark it
on the CAST row and on the player-target row.** This one decision cost 13 turns of an empty board.

**HIGH-3 — A COUNTERSPELL-BAIT SURFACE EXISTS ONLY AS A CARD NAME IN A GRAVEYARD LIST, AND TWO MAKERS
DIED TO IT IN ONE GAME.** vs125 **seq 143** (t22 Main phase 1): the prompt carries
`Their untapped sources: 11 (colours they could make: {u}{w})` and
`Their graveyard (8 cards): Essence Scatter {1}{u} x2; Dream Fracture {1}{u}{u}; ...; Cancel
{1}{u}{u}; ...`, and the seat cast Thraben Doomsayer as its first and only spell. The narration in the
next prompt:

> `- You cast Thraben Doomsayer` / `- Opponent cast Essence Scatter` / `- Your Thraben Doomsayer was
> COUNTERED by Essence Scatter and went to your graveyard`

Identically at **seq 190** (t26): three Essence Scatters and a Cancel in their graveyard, 11 untapped
sources, and the Bloodline Keeper is countered. There is no rendered clause anywhere on either prompt
that says a counterspell is live — the fact is reconstructible only if the model knows Essence
Scatter, Cancel and Dream Fracture are counterspells and thinks to cross the graveyard line against
the untapped-source line. **The prompt already has both facts and already classifies cards by
function elsewhere** (`a DRAW PUNISHER`, `a TOKEN ENGINE`, `LIFE-TO-DAMAGE CONVERTER`). Fix
direction: a `COUNTERSPELLS SEEN` clause of the same family — name the counterspells in their
graveyard/log beside their untapped-source count, on any window offering a cast. I paid for the guide
half of this myself (edit 4), but the guide half asks the model to do a card-class lookup the render
already does for four other classes.

**MED-4 — 14 PROMPTS OVER 20 KB, ALL IN ONE GAME'S TAIL** (vs125 seqs 332-371, turns 37-41, max
22,578 B at seq 371). Wave 79 at this seat was 1 of 495. This is append-only narration growth in a
41-turn game (the longest in the corpus), not a per-window regression — my five other games peak at
16.4 KB. Worth naming because the growth is unbounded in turns and this deck stalls: the seat's
narration at t40 is 41 turns of `Cast nothing right now` windows that changed nothing. **Fix
direction: a turn-window or a collapse for consecutive no-action turns in the GAME LOG.**

**MED-5 — ON A WINDOW WHOSE `[stack death verdict: the stack KILLS you]` IS LIVE, THE PER-ROW
`{answers the stack: NO ... KILLS you}` CLAUSE PRINTS ON `Cast nothing right now` AND NOT ON THE HOLD
ROW, AND THE SEAT TOOK THE HOLD ROW.** Repro vs162 **seq 137**: row 4 carries
`{answers the stack: NO - 8 damage is ALREADY ON THE STACK, and taking this row lets it resolve: that
puts you at -7 and KILLS you}`; row 3, the hold row, carries no such clause. The global bracket does
cover it (`Every row on this menu that declines, the hold row included, lets it resolve`), so nothing
here is FALSE — but the two decline rows are priced asymmetrically and the seat picked the unpriced
one. Cheap symmetry fix.

**LOW-6 — `plan_names_uncastable_zone_card` 11**, 9 of them in vs125 (seqs in the t12-t26 stall) and
1 each in 146/130. Every one I read is the seat naming Intruder Alarm or a maker still in hand while
the window offers neither; same verdict as wave 79 — too noisy to read as a defect.

**Render clauses and counters I checked and found TRUE (per-record, not per-counter).**
- **U7 / `board sweep:` NAMES YOURS — PASS, and wave-79 HIGH-1 IS CLOSED.** All 9 markers at this
  seat, quoted: 7 read `[<- board sweep: THEIRS N / YOURS 0 - the only row on this menu that prices a
  board sweep]` (126 seq 28, 146 seqs 8/10/13/15, 130 seqs 41/43) — one-sided is correct at YOURS 0.
  Both markers with YOURS non-zero now carry the symmetric sentence: **152 seq 82**
  `... / YOURS 2 (Bloodline Keeper (3/3) ..., Vampire (2/2) ...; of those, Bloodline Keeper - a TOKEN
  ENGINE (it makes more permanents, one per activation) - losing that is not losing a body, it is
  losing what the body keeps making) ... THAT COUNT IS BODIES, NOT VALUE: what it takes of YOURS
  includes the engine(s) named above ...`, and **162 seq 97** the same with BOTH halves rendered
  (`what it takes of THEIRS includes the engine(s) named above, and what it takes of YOURS includes
  the engine(s) named above`). The behavioural read is mixed and honest: at 162 seq 97 the seat
  **declined** the sweep with the symmetric marker present (wave 79's seat cast it with the one-sided
  one); at 152 seq 82 it cast anyway, but its reasoning is a survival argument that never mentions
  the body count, so the marker did not mislead it there — it was overridden.
- **U8 / cross-phase replay — PASS at this seat, and the replay was RIGHT.** `crossphase_replayed` 1,
  `crossphase_board_unchanged` **0** (wave 79: 22 of the corpus's 26 were mine). The one replay is
  traced: `askreplay/1789216922-ai_baka_deck123-...-vs-ai_baka_deck152.jsonl`, `{"choice":3, "seq":62,
  "turn":10, "replayed_from":57, "why":"crossphase_replay from Main phase 2 to End"}`. I diffed the
  two windows' rows: seq 57 (Main phase 2) and the End window offer the **same three rows** — `1. Cast
  Tragic Slip {b}`, `2. Hold priority`, `3. Cast nothing right now` — and **all three are
  instant-speed**. Main phase 2 -> End can only REMOVE sorcery-speed rows, and none was on the list, so
  **no phase-gated option could be hidden** (Astra's risk does not bite on this transition shape). The
  replayed answer (3) is what the seat wrote at 57 and again, live, at seq 63. My read on the
  population question: the board-unchanged 22 did not migrate into the replay counter — **they were
  not re-put at all.** `crossphase_identical_reputs` at this seat fell 124 -> 58 while my decision
  count fell 499 -> 392, i.e. the re-puts themselves are down by more than the seat shrank. My
  longest wave-79 offender (9 consecutive windows on a frozen turn 12) has no analogue this wave.
- **DE / DH F11 — `hold_event` records, and wave-79 MED-3 IS CLOSED.** All 28 read; every one carries
  `what`, `reason`, `seam`, `window_seq` and, where relevant, the clamped `face`. 15 are
  `reopen_rows_moved`. All **12** `verdict_clamped` events are the same shape and the clamp is toward
  the safer face: `held [stack death verdict: you survive the stack] over live [stack death verdict:
  nothing lethal on the stack]` — "nothing lethal" is strictly safer than "you survive", so **ignoring
  was right in 12 of 12**. 11 of them are ONE window (125 window_seq 135, t35) correctly suppressed
  across 11 phases; the twelfth is 162 seq 46. Wave 79 could locate none of these; they are now
  reviewable from the record alone.
- **DH F3 / `hold_reopened_new_lethal` 1 — the threat was REAL and the re-ask was owed.** 162
  **seq 136**: `new_lethal_stack: a NEW stack threat over the same [stack death verdict: the stack
  KILLS you] face at the cast seam - the objects behind it are not the ones this hold was taken over`,
  window_seq 60. The seat was re-asked at **seq 137**, where the prompt carries `[stack death verdict:
  the stack KILLS you - what is waiting to resolve takes you to 0 or below]` and the decline row's
  `{answers the stack: NO ...}`. **Did the seat act? It could not** — rows 1 and 2 were Vision Skeins
  (draws into the punishers, worse) and Tragic Slip (kills a creature, does not touch the stack). It
  took the hold row and died. The re-open is a true positive that arrived with no answer available.
  `hold_reopened_new_threat` 0 is **UNTESTED here**: no THREATENED face rendered in 392 records.
- **U1 / DH F5 — the cast step in the PLAN echo: PASS.** `plan_cast_steps_closed` 42. I read the echo
  on every completing menu I cite; it carries the cast clause until the cast event and drops the
  consumed object by name. 162 seq 97: `THE PLAN YOU LAST STATED (as you stated it, 1 window ago on
  turn 12; "Marsh Flats" is no longer on your menu) reads "Play Marsh Flats, cast Thraben Doomsayer,
  cast Intruder Alarm, ..."` — the played land is struck, the two un-cast steps stand. **0 declines on
  a completing menu at this seat**, so the stranded-mana print is UNTESTED here.
- **U3 / DF — `{crack-back cover:}` : 13 prints, and the mechanism sentence is trample-aware.** 3 of
  the 13 carry the trample clause; 162 seq 97 states it and then uses it: `Each blocker stops at most
  ONE attacker. ... an attacker that is BLOCKED deals its combat damage to the blocker, not to you -
  all of it, whatever the blocker's size, and a 1/1 in front of a 5/5 stops the whole 5. ... None of
  these attackers has trample. CHECKED: 1 of them is a body whose block legality against these
  attackers this row computed.` The body credited (the Doomsayer the row casts) really does arrive and
  really can block — **no Acererak-class credit at this seat**. The Damnation form at 152 seq 82
  correctly refuses a survival verdict on a FLOOR total (`THIS IS NOT A SURVIVAL VERDICT: the total it
  is subtracted from is a FLOOR`), which is the right shape — HIGH-1 is that the *bracketed verdict
  line* does not keep that discipline about the draw step.
- **U4 / one tap bill per row: PASS.** 11 prints, every one naming its own tapped attacker, e.g. 162
  seq 41 `{paying this taps: Thraben Doomsayer - it cannot attack this turn}`; no row bills a creature
  twice and no row bills a creature it does not tap.
- **`crackback_lethal_blocked_away` 19** (all vs162 t12-13) and `stack_death_verdict_lines_rendered`
  9 (162 x7, 125 x2). The 19 are the `with your best block: 0 damage still gets through, leaving you
  at 1` figure; the arithmetic is right against the board (one 3-power attacker, two legal blockers)
  and the "best block" floor is correct as combat arithmetic. It is the *conclusion drawn from it*
  that HIGH-1 is about.
- **The harness (DJ): clean, and the rerun passed on the new gate.** `http=200` on 358/358,
  `deadline_pct` max 30.7, **`async_drops` 0** (wave 79: 3), 0 truncations, 6 `forced_close` records
  (3 `_unrecorded`, none with an answer lost that I could find), gameend present on all six logs.
  The rerun game's own record stream is indistinguishable in shape from the twenty on the other
  binary.

**UNTESTED from this seat (0 renders in 392 records):** `[own loop verdict:` (`own_loop_windows_asked`
0, `own_loop_verdict_lines_rendered` 0 — third consecutive wave), `hold_reopened_new_threat`,
`menu_single_outcome` / `single_outcome_rows_spared` (0 / 0), `cached_replay_reasked` (the DI state
never arose — see §1 vs162), `[RE-ASK]` (0, third wave), `UNLESS <source> untaps them`, `AT BEST` and
the gang-block family (`blocker_forecast_rows` **0**, 3 blocker declarations in six games),
`GANG BLOCK` (3 prints, none decisive), `Transform:backside` (0 — the Keeper never had five
Vampires), MDFC faces (tenth wave with none in a deck123 hand), `mana_only_windows_skipped` (0),
`stop_reached_reputs_collapsed` (0; `_windows_skipped` 1, in vs146 t16 after the seat reached
`M=18` against `stop=23` — a real chain, correctly collapsed, and it hid nothing because the seat
then took `x5` at seq 36).

## 3. Which wave-79 items recur

| wave-79 item | wave-80 status |
|---|---|
| **edit 1**, the DAMNATION veto restated at P0 | **PARTIAL — 1 of 2.** Two boards put a maker in the row's `M of yours`: **162 seq 97 HONOURED** (Damnation declined, row 5 taken), **152 seq 82 BROKEN** (Keeper + token swept). Wave 79 was 0 of 1. The break is not a priority misread — the reasoning never reaches the list; it is a survival argument. Edit 2 answers it positively. |
| **edit 2**, `and no maker in M` on the arithmetic bullet | **NOT EXERCISED.** The bullet's precondition (`N over M with K at 1+`) held on neither Damnation board with a maker in M (162 seq 97: N=2, M=2 -> `M bigger than N` was not met either; 152 seq 82: N=2, M=2). The contradiction it removed did not recur; it was also not tested. |
| **edit 3**, `<x<stop-M>>` in the PLAN template | **PAID, and it won the wave's only game.** vs146 **seq 36** writes `stop=23, M=18; this window x5` — the subtraction exactly — and takes `x5`; seq 32 writes `stop=16; M=2; this window x16`. Wave 79's seat wrote `x3` under `stop=24, M=2`. **KEEP.** |
| **HIGH-1**, the one-sided `board sweep:` marker (U7) | **CLOSED.** Both YOURS-non-zero markers name the engine symmetrically; quoted in full in §2. |
| **HIGH-2**, cross-phase dead lists at full price (U8) | **CLOSED at this seat.** `crossphase_board_unchanged` 22 -> **0**; the single `crossphase_replayed` was right for the new phase and hid no phase-gated option (§2). |
| **MED-3**, `hold_verdict_safer_ignored` firing with nothing rendered | **CLOSED.** 12 clamps, each with its face and reason on the record; 12 of 12 clamped toward the safer face. |
| **MED-4**, the cast seam replaying the decline across the token loop | **DOES NOT RECUR in the loop.** `ask_replays_cache` 345, and **not one is a Create row** — the 11 vs162 Create re-puts are full model calls. |
| **LOW-5**, `plan_names_uncastable_zone_card` 16 | 11, still noise (LOW-6). |
| — new — | the crack-back verdict ignoring the compulsory draw step (HIGH-1), the unmarked engine on the edict cast row (HIGH-2), the invisible counterspell (HIGH-3), long-game prompt growth (MED-4), the unpriced hold row under a lethal stack (MED-5), **and the stop that is never stated on a single-Create window (§4 edit 3)**. |

## 4. Guide verdict: **EDIT**

Revised guide at `wave80/deck123/strategy.txt` — **19,952 bytes** (live guide 19,974; ceiling 20,000). **The guide SHRINKS by 22 bytes; no rule earns new bytes on net.** A332 grep (`seq`, `wave`,
`corpus`, `game N`, deck numbers, turn citations): 0 hits. `YOUR PLAN`: 0. A334 audit (`even when` /
`even if` / `regardless of` / `no matter` / `however` / `although`): 0 new; the one pre-existing
`unless` (CHECK 0 crack-back) is a scope condition, not a concession, and is untouched. Both PLAN
templates are one physical line each; the only line over 100 chars is the stop template, by design.
Headings unchanged and each still declares its own scope (A333).

| # | before -> after | seq that paid for it |
|---|---|---|
| **1** | **The loop's cost is stated where the decision is made.** Added to `--- TIMING THE CHAIN ---`, ahead of the existing ALARM-IN-HAND bullet: `- ALARM OUT AND AN UNTAPPED MAKER: each token entering untaps the maker, so the whole run to the / stop costs ONE window. Take it where it is offered; a later window this turn is not a reason to / pass, and neither is the tokens' summoning sickness - those bodies BLOCK the turn they arrive.` The guide had the ALARM-IN-HAND ordering rule and no rule for the board it is trying to reach. **+292 bytes.** | **`...vs-ai_baka_deck152.jsonl` seq 80.** Alarm out, Keeper untapped, `M=2, your stated stop=24, so this window has 22 to add`, and the seat passed — reasoning verbatim: *"Since I don't need to make tokens yet"* and *"tokens are summoning sick anyway, and Intruder Alarm untaps them. Tapping now just delays the token by a turn."* Both of the model's two stated reasons are answered by the two clauses of the new bullet. |
| **2** | **DAMNATION names the alternative instead of repeating the prohibition.** In `--- DAMNATION ---`, after the veto sentence: `... the / number is not written, whatever N and K say.` -> `... the / number is not written, whatever N and K say. With the Alarm out and an untapped maker, the blockers / you need are on the Create row, not on this one.` A334 forbids a concessive clause for the hard case, so this is positive: the survival argument that beat the veto has a better answer on the same screen. **+104 bytes.** | **`...deck152` seq 82**, one window after seq 80. The reasoning is *"they attack first, I block with token (dies), take 2, go to 2. Then they attack with Wolf, I take 2, go to 0. I lose. So Damnation NOW is mandatory."* The Create row it had just declined answered that exact fear 22 times over. |
| **3** | **The stop is written at EVERY Create window, not only at the repeat row.** Added to `--- THE TOKEN LINES AND THE STOP ---`, immediately above the repeat-row bullet: `- EVERY CREATE WINDOW IS A STOP DECISION, the "[cost: Tap]" row included: write stop= and M= in / the PLAN before the CHOICE, on the turn's FIRST Create row and on every one after it.` The existing governance sentence (`EVERY Create row is governed, the "[cost: Tap]" one included`) forbids EXCEEDING a stop; nothing instructed the pilot to STATE one on a single-row window. **+184 bytes.** | **17 single-`[cost: Tap]` Create windows at this seat, `stop=` on 0 of them**, against 4 of 5 where the repeat row was present. Chief among them **`...deck162` seq 41** (`PLAN: Tap Thraben Doomsayer to make a token in upkeep. ...`) and **`...deck125` seq 233** — the stopless-PLAN shape the first binary replayed into a dead loop, and the shape lane DI's "a replay never passes a stated stop" has nothing to bind against. |
| **4** | **The edict decision moves to the cast row, and the counterspells get names.** (a) `--- THE TWO EDICTS ---`: `You NEVER pick the victim; the only choice is whether to cast.` -> `... THE DECISION IS AT THE CAST ROW: at / YOUR count of 1 it names the victim ("targeting yourself sacrifices <name>") and the target ask / behind it has no pass row - a maker or your only creature named there answers the CAST row.` (+224) (b) the cast gate: `a counterspell in their log or graveyard` -> `a counterspell on their graveyard line (Essence Scatter, Cancel, Dream Fracture, Dissolve, Negate)`, and the bait list gains `- never Vision Skeins, which pays them a card`. (+104) | (a) **`...deck125` seqs 238/239.** The prohibition already existed under the *victim* bullet; the victim ask had no pass row by then, so the rule was unreachable when it mattered. The cast row printed `targeting yourself sacrifices Bloodline Keeper` and the seat read it as a life gain. (b) **`...deck125` seq 143** (Doomsayer countered by Essence Scatter, 3 counterspells in their graveyard, 11 untapped sources) and **seq 190** (Keeper countered; the seat did cast Vision Skeins first at seq 189, which the opponent happily let resolve for 2 cards, then countered the maker). The gate's trigger asked for a card-class judgement the guide never supplied names for. |
| — | **Paying for edits 1-4 (-569 - 356 = -925, no rule removed that the corpus exercised):** two protocol-echo bullets in `--- ANSWER FORMAT ---` (`PLAN line, then CHOICE line...` and `EVERY GATE HERE IS APPLIED BEFORE YOU WRITE...`) — **0 `protocol_deviation`, 0 `plan_line_missing`, 0 `off_protocol_bytes` in 358 replies this wave and 495 last wave**, and the reply protocol in the prompt states the format itself; the PLAN-vs-`{right now:}` precedence sentence is KEPT. Two pure cross-references in the printed-strings list (`STRANDS ... : see YOUR OWN UPKEEP AND DRAW STEP.` and `"(your attacker dies, their blocker lives)" ... : see CHECK 0.`) — both sections restate them verbatim within 20 lines. Two duplications of P0 and CHECK 0 in the opening summary. `CHECK 1 B` (exactly one of Bond/Blood: `3 x their untapped count + 3`) — **that board has not arisen in two corpora** (vs126 had BOTH, which is B2, now renamed B), and the arithmetic had no evidence behind it. The Greaves' one-equip-row-per-token sentence — 0 renders in two corpora. The Vision Skeins Cleanup-discard clause and `Colour never ships a hand.` — neither decided a window in two corpora. | — |

**What I deliberately did NOT change.**
- **P3 (Intruder Alarm above a second maker).** Broken once, at 162 seq 97, and the window was already
  lost: the same prompt's DRAW FORECAST put the seat at -8 on a compulsory draw step it could not
  decline, so no row on that menu wins the game. A rule is rewritten by a decision that cost
  something. The guide's P-order is not the reason the Alarm never landed in that game — it was
  offered exactly once, on the turn the seat died.
- **`of which 0 are creatures` / Alarm on an empty board.** Broken once (126 seq 25, t8), free again:
  the opponent's Sanguine Bond + Exquisite Blood loop was already assembled and no other row was
  castable. Third wave running this rule breaks only on boards where it costs nothing.
- **CHECK -1 (the lone maker never attacks).** Broken once, at 125 seq 234, and edits 1 and 3 attack
  the same window from the front — at seq 233 the Create row was there and the stop was unstated. I
  will not add a second rule to a window I have just given two.
- **CHECK 0 / CHECK 2 / CHECK 3 and the whole BLOCKING section.** `blocker_forecast_rows` **0**, 3
  blocker declarations and 5 attacker declarations in six games. No evidence in either direction.
- **The stop arithmetic `L + C + 3` itself.** Where it was computed it was computed right (146 seqs
  33/35/36 all rebuild `stop=23`; 152 seqs 57/63/64/66 oscillate 24/25 with the board still, which is
  `L=20, C=2` -> 25 against the engine's printed 24 — a 1 difference I did not chase and that changed
  no answer).
- **The MULLIGAN AND BOTTOMING section.** 6 keeps, 0 bottoms this wave.

## 5. Optional proposals

**None.** All four edits are this deck's own wording, and the discipline each restores is already in
the installed skill: A333's heading-scope remedy (edit 4a moves a rule to the heading whose window
reads it), A115/A330's "where one number governs, the number is WRITTEN in the PLAN line" (edit 3),
A334's no-concessive-clause rule (edit 2 is why it is phrased as an alternative, not an exception).
A skill or general-guide proposal restating them would be noise. HIGH-1, HIGH-2 and HIGH-3 are engine
items and belong to the core loop, not to a prompt layer.

## 6. What I did NOT check

- **I opened no card script, ran no build, ran no wagic, ran no git**; nothing outside
  `strategy-design/wave80/deck123/` was written, and nothing under `bin/Res`, `src/` was touched. The
  revised guide starts from the live `bin/Res/ai/baka/deck123_strategy.txt` (verified byte-identical
  to `wave79/deck123/strategy.txt` with `cmp`).
- **I read opponent seat logs for board context only, and only one of them** (the deck126 seat, to
  date Sanguine Bond and Exquisite Blood). **I audited no opponent decision.** Every other board fact
  above is read off my own seat's `prompt` field.
- **I adjudicated no lane prediction corpus-wide.** §2 answers U1, U3, U4, U7, U8, U9, DE/DH F11, DH
  F3, DH F5 and DJ **for my six games only**. U5 (the neutral exemplar / row-1 pick rate), U6
  (cycling — this deck has no cycler), U10 (`own loop verdict`, 0 renders), U11, U12, U13
  (`menu_single_outcome` 0), U14 (0 blocker-forecast rows), U16 and U17 I did not evaluate at all, or
  had no render to evaluate.
- **I did not verify all 346 replays.** I read the full `askreplay/` file for vs152 (58 entries) and
  confirmed the one `crossphase_replay` in full against both windows' row lists; the other five games'
  replay files I only counted and confirmed all `cache_replay`. **I re-derived no cached answer
  against its live board**, so "no replayed answer was wrong" is unproven outside the one I traced.
- **12 of 12 `ask_key_continuation_differs` events were NOT paired.** I counted them and read none.
- **The `hold_windows_skipped` 190 were not sampled** — I read the 28 `hold_event` records and the
  windows they name, not the 190 suppressed ones, which consume no `seq` and appear in no log.
- **The 3 `forced_close_unrecorded` were not reconciled against any stderr file**, and the 17
  `hold_released_turn` events were not paired to their holding windows.
- **I opened `game-162v123-1789216936-HUNG-attempt0.stderr` only far enough to confirm the loop
  signature.** I did not count its replays independently (I take 3,674 from the brief), did not
  reconstruct its board, and drew no §1 conclusion from it — the rerun game's own records are the
  whole basis for the DI answers.
- **I read no `…-rerun-01-162v123-attempt1-killed` artefact at all**; the DJ verdict above rests only
  on the rerun game's own clean log, not on the killed attempt's evidence.
- **The 20 HIGH-1 conflicts were detected by a scripted pairing of the `[crack-back verdict:` bracket
  against the `DRAW FORECAST:` line in the same prompt.** I verified the pairing by hand on seqs 97
  and 137 only; the other 18 I checked by their rendered text, not by recomputing the draw forecast
  against the board.
- **I read the `reasoning` of every window cited in §1 and §2 and of all 9 board-sweep windows.** I did
  not read the reasoning of the 190 skipped-hold windows, the vs125 t12-t26 stall, or the 11 vs162
  Create re-puts beyond their answers and latencies.
