# Wave 79 — deck 123 (Intruders of Thraben, WBU token combo) — per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-000748-final`, SINGLE binary
`archives/wagic-5af6f7a95-w79step1` (source b1e1db7be), `--thinking on`, one run, no reruns.
Live guide = the wave-78 edition (installed a37b2d5ce, byte-identical to `wave78/deck123/strategy.txt`).

**0 wins, 6 losses** (wave 78: 4/6). Seat census: **499 decisions** (ask 440, priority 43,
discard 9, forced_close 4, attackers 2, bottom 1; by game 126: 137, 146: 114, 130: 82, 162: 65,
152: 53, 125: 48), `protocol_deviation` **compliant 495/495**, **0 fallbacks of any kind, 0
`plan_line_missing`, 0 `off_protocol_bytes`, 0 `reply_truncated`, 0 `action_before_plan`**,
`thinking` **on 495/495** with `reasoning_chars` median 5,887 / max 17,597. `transport`
`curl=0,http=200,empty=0` on 495/495, `deadline_pct` max 40.7. Latency p50 90.6 s, p95 182.3,
max 366.4; 13.7 h of decode. **Prompt max 20,286 B, mean 11,872, only 1 of 495 over 20 KB**
(wave 78 at this seat: max 27,998, 124 over) — T10's trim is the largest measurable improvement
at this seat. Gameend sums: `ask_replays_reserved` **525**, `identical_ask_answers_reserved` **0**,
`ask_key_continuation_differs` **22**, `hold_windows_skipped` 327 (cast 300 / priority 27),
`hold_released_turn` 25, `hold_verdict_safer_ignored` **13**, `hold_reopened_new_threat` **0**,
**`stop_reached_windows_skipped` 8 / `stop_reached_reputs_collapsed` 5 — all 8 and all 5
corpus-wide are mine**, `crossphase_identical_reputs` 124 (of 172 corpus-wide) /
`crossphase_board_unchanged` **22** (of 26 corpus-wide), `stack_drain_windows_asked` 12,
`plan_names_uncastable_zone_card` 16, `async_drops` 3, `forced_close_events` 4 / `_unrecorded` 2,
`own_loop_windows_asked` 0, `blocker_forecast_rows` 4, `reserve_decline_windows_noted` 10.

**THE 4/6 -> 0/6 SWING IS NOT A WAVE-79 RENDER EFFECT, AND I CHECKED EACH CANDIDATE.** The deck
assembled its engine in **one** of six games. `stop=` is written in **3 replies out of 495**, all
in the vs162 game, and **3 Create rows are taken in the whole seat** (vs162 seqs 85/98/127); the
other five games take **0**. In those five the maker was countered on the stack (125), edicted off
on the opponent's upkeep before it ever got an untap (126), Starstormed twice (130), killed with no
replacement drawn for 20 turns (146), or arrived on turn 18 (152). **The collapse, the new ask key
and the hold latch are all downstream of a maker being on the board, and in five games there was
none to gate.** The one game where they did fire is traced window-by-window below and none of the
three took a decision away.

## 1. Game by game — the deciding decisions

**vs deck162 — LOSS, turn 14 (0 / 20). The only game the deck played, and it was thrown at turn 8.**
Thraben Doomsayer t6 (seq 34). At **seq 42** (t8, life 16, their board Fate Unraveler 3/4 +
Shield Sphere 0/6 [defender]) the seat **cast Damnation, destroying its own Doomsayer** —
the row printed `destroys 2 of their creatures (1 without a restriction against attacking), 1 of
yours - ... YOURS: Thraben Doomsayer (2/2)`. The guide's DAMNATION section forbids this in those
exact words (`A TOKEN-MAKER OF YOURS IN THE "M of yours" LIST IS A VETO OF ITS OWN`). The
reasoning shows precisely how the veto was missed, and it is quotable:

> "Damnation is the best play here. ... Wait, the prompt says: "P0 Damnation {2}{b}{b} - your line
> reads "of which 0 are creatures" and the row's K is 2+. Ahead of any maker, Tutor or Alarm."
> ... So Damnation is priority 0. I will cast Damnation."

The model quoted P0's **two** conditions, dropped the first (its own line read *1* creature, not 0),
and never read as far as the DAMNATION heading 66 lines below. The second-order failure is worse:
the arithmetic bullet under that heading, `N over M with K at 1+: cast it.`, **licenses this exact
board** (N=2, M=1, K=1) and contradicts the veto three lines above it. **Both are fixed below
(edits 1 and 2).** Cost: the engine restarted four turns late — Bloodline Keeper t10 (seq 49),
Intruder Alarm t12 (seq 74), 24 creatures by t12 (seq 127). The seat then died **in its own turn-14
draw step** (seqs 133-136, 9 damage off two Underworld Dreams + Ob Nixilis under two opposing
Howling Mines) at 3 life with 24 flying vampires on the board and Fog Bank already Slipped away —
**two turns short of a lethal swing.** The line that wins is Bloodline Keeper at seq 42 (row 2 on
that menu, same mana) with the Doomsayer kept: Alarm on t10, army on t10-11, attack t12 at ~13 life.
Contrast **seq 74**, where a second Damnation row printed `board sweep: THEIRS 1 / YOURS 1` with
**no** editorial sentence attached and the seat declined it for the Alarm. Same trade shape, one
clause different, opposite answer (HIGH-1).
Two smaller violations inside the one engine turn, neither of which cost damage here but both of
which are the stop rule failing: **seq 98** takes `x3` under its own `stop=24, M=2` (the rule's
N = stop - M = 22), and **seq 115** answers `CHOICE: 0 (pass)` at `M=4` against a printed
`{right now: M=4, your stated stop=24, so this window has 20 to add before it reaches your own
stop}`, reasoning *"I already have 3 tokens. It doesn't change much."* — the rule says take the
line every time it returns below the stop. Twelve windows later, at seq 127, the seat took the
`x20` it had just refused. **Edit 3** puts the subtraction inside the PLAN line.

**vs deck126 — LOSS, turn 37 (0 / 38).** Mulligan to 6 (seq 2), bottomed Vision Skeins. The only
maker of the game is cast at **seq 63** (t10, Thraben Doomsayer) and is dead at the **opponent's
turn-11 upkeep** to Tribute to Hunger, before it ever had an untapped, unsick window. The seat's
Tragic Slip at seq 75 correctly killed Pride Guardian off Morbid. From t13 the opponent assembles
Sanguine Bond + Exquisite Blood and 93 of the seat's prompts print `LOOP RUNNING`; from there the
seat is correct on every window I read (it never pays life, never blocks, never casts Devour Flesh
into the loop) and has nothing to play. Damnation at **seq 251** (t36, `THEIRS 4 / YOURS 0`) is the
right cast and is 20 turns too late. **No decision of this game is a guide failure.**

**vs deck146 — LOSS, turn 27 (0 / 20).** Thraben Doomsayer t6 (seq 21) dies before t8; the seat
draws **no further maker for 19 turns** and ends the game having controlled a creature on 2 of 27
turns. Damnation at **seq 24** (t8) is correct and the guide's P0 fires correctly there
(`THEIRS 2 / YOURS 0`, the reasoning states *"P0 Damnation applies because M=0, K=2"*) — the same
entry, read correctly, on the board it is written for. Intruder Alarm cast t24 with no creature
(seq 167) is the guide's `of which 0 are creatures` rule broken, and it is free: the seat had 11
untapped sources and nothing else to do. Hands are real; **nothing here is paid.**

**vs deck130 — LOSS, turn 19 (-1 / 25).** Doomsayer t7 (seq 36), Bloodline Keeper t9 (seq 39),
Doomsayer #2 t11 (seq 42) — **all three killed by Starstorm / Pyrite Spellbomb before a single
untapped, unsick window**; the graveyard line at seq 93 reads `Thraben Doomsayer x2; Bloodline
Keeper`. The seat's repeated plan *"tap Doomsayer on the opponent's turn"* (seqs 53/54/64) is
correct and never became legal. Intruder Alarm cast t17 (seq 118) onto an empty board is the
`of which 0 are creatures` rule broken again, free again. This game contains the seat's worst
interface waste: **turn 12, seqs 72-80, nine consecutive full model calls on a frozen board**
(MED-3). **Nothing paid.**

**vs deck152 — LOSS, turn 19 (-3 / 26).** No maker until **turn 18** (seq 83). Damnation at seq 33
(`THEIRS 4 / YOURS 0`) and Tribute at seq 34 are both correct. Greaves equipped to the Keeper at
seq 86 per the guide. The attack at **seq 89** sends the lone Bloodline Keeper — a CHECK -1
violation on its face, but the reasoning states *"I am going to die next turn regardless"* and the
crack-back line agreed; I do not pay a rule for a window whose alternative was also a loss.
**Nothing paid.**

**vs deck125 — LOSS, turn 32 (0 / 33).** **One** maker drawn in 32 turns, Bloodline Keeper at
**seq 110** (t26), **countered by Essence Scatter**. I checked whether the guide's counterspell
bait gate should have fired: the gate's trigger is "a counterspell in their log or graveyard" and
at seq 110 the prompt contains **no** counterspell anywhere — their graveyard held Final Judgment
and Supreme Verdict, both sweepers. The gate could not fire, and it would not have helped:
Essence Scatter counters creature spells only, so a dead Greaves cast first is not bait for it.
49 decisions in 32 turns, 0 creatures ever on the battlefield. **Pure variance.**

**Scoreboard.** Makers cast across the six games: 9. Makers that ever reached an untapped,
non-sick priority window: **1** (the vs162 Keeper). Create rows offered: 4 windows, all vs162 t12.

## 2. Engine / interface / card items

**HIGH-1 — THE `board sweep:` MARKER ARGUES ONE SIDE OF THE TRADE. It names the engines it takes
off THEIRS and has no sentence for the engine it takes off YOURS — and this deck's engine is a
creature.** Repro: `...vs-ai_baka_deck162.jsonl` **seq 42**, the Damnation row's tail:

> `[<- board sweep: THEIRS 2 (including Fate Unraveler - a DRAW PUNISHER (it bills every card the
> other player draws)) / YOURS 1 - the only row on this menu that prices a board sweep. THAT COUNT
> IS BODIES, NOT VALUE: what it takes of THEIRS includes the engine(s) named above, and an engine
> keeps producing while it is on the board, so a body-for-body comparison does not price it]`

Every clause after the slash argues for the cast. The engine plainly *has* an engine detector — it
identified Fate Unraveler as a DRAW PUNISHER by class — and runs it only over the opponent's half.
The `YOURS 1` is `Thraben Doomsayer (2/2)`, a token producer by the same test, and it is this
deck's entire win condition. The seat cast it and lost the only game it could have won.
5 `board sweep:` markers at this seat; the editorial sentence prints on exactly the one with
`YOURS` non-zero, i.e. on the only board where the asymmetry can mislead. **Near-A/B control in the
same game:** seq 74, `THEIRS 1 / YOURS 1`, editorial **absent**, seat declined the sweep for the
Alarm. Fix direction: run the same engine/producer test over the `YOURS:` names and print the
symmetric sentence, or drop the editorial when `YOURS` is non-zero.

**HIGH-2 — `crossphase_board_unchanged` 22 OF THE CORPUS'S 26, AND THEY ARE 22 FULL MODEL CALLS ON
A BOARD THE ENGINE ITSELF CERTIFIES HAS NOT MOVED.** The bracket is TRUE in all 22 —
`...vs-ai_baka_deck130.jsonl` **seq 75**: `[this exact list was put to you 1 window ago at Main
phase 1 and you declined; nothing on the board has changed]`, and the same at **seq 76** (Combat
begins), **seq 79** (Main phase 2), **seq 80** (End), **seq 93** (t13 Main phase 2), **seq 96**
(t14 Main phase 1). Worst run: `...deck130` **seqs 72-80**, nine consecutive windows on turn 12
(one `Hold priority` at seq 73, eight `Cast nothing right now`), **746 s of decode**, four of them
carrying the board-unchanged bracket and the opponent doing nothing in between. The
ask cache does not serve these because the *phase* is part of the seam scope (`w79AskScopeKey`
takes `observer->getCurrentGamePhase()`), so a dead list crosses every phase of the turn at full
price. The engine already prints the predicate that would authorise the collapse. This is the
single largest decode line at this seat and it is not a model failure — the answer was right 22
times out of 22.

**MED-3 — `hold_verdict_safer_ignored` 13 FIRES WITH NOTHING RENDERED.** 13 events (vs162 9,
vs146 4) and **0 prints** of any `[crack-back verdict:` / `[stack death verdict:` /
`[own loop verdict:` face in 495 prompts at this seat (`own_loop_windows_asked` 0). The counter is
incremented at `AIPlayerGPT.cpp:35476` when a held marker is clamped to its safer live face, so
the seat is being told nothing about a verdict that moved under its own hold. Since every one of
the 13 was a clamp toward *safer*, none of them is a missed lethal — but the census cannot be read
against the prompt, and I could not verify a single one from the corpus. **`hold_reopened_new_threat`
0 is UNTESTED at this seat, not a pass**: no THREATENED face was ever rendered, so no new threat
identity could arise to re-open one.

**MED-4 — THE CAST SEAM REPLAYS THE DECLINE ACROSS THE WHOLE TOKEN LOOP.** All **525** replays at
this seat are `why: cache_replay` and **every one is a "Casting decision"** — zero priority-seam
replays. That is mostly correct, but the longest runs sit exactly where the board is moving fastest:
`...deck162` **seqs 76-86** (10 replays of seq 75's `Cast nothing right now`) and **88-97** (10
replays of seq 87's) span the turn-12 Intruder Alarm loop, with the seat's creature count going
1 -> 4 underneath. I could not find a replayed answer that was wrong for the new board (the seat
had 0-1 untapped sources through both runs and the seam re-asked at seq 99, where it did cast
Devour Flesh) — but the run length is set by how long the *continuation digest* stays still, and
a token entering does not move it. Flagged, not proven.

**LOW-5 — `plan_names_uncastable_zone_card` 16, TWELVE OF THEM IN ONE GAME** (vs130), against
4 last wave. Every one I read is the seat naming Intruder Alarm or a maker still in hand while the
window offers neither; the measure is drifting back up and is again too noisy to read.

**Render clauses I checked and found TRUE.**
- **T3 / DC F1 — the CG shape: PASS, and it is the wave's clearest win at this seat.** Same-turn
  same-phase **byte-identical** re-asks: **3 in 499 decisions** (`...deck126` 277->280,
  `...deck146` 98->100 and 102->104). Wave 78 at this seat had runs of 28 in a single turn.
  `identical_ask_answers_reserved` **0** against `ask_replays_reserved` **525**: the replays did not
  stop, the counter moved — every reservation is now logged in `askreplay/` as `cache_replay`
  (77 entries for vs162, 174 for vs126, 525 total = the gameend figure exactly).
  I verified one of the 22 `ask_key_continuation_differs` events in full: `...deck146` **seq 98 vs
  seq 100**, identical Marsh Flats rows, and the full-prompt diff is `Opponent battlefield ...
  of which 0 are creatures` -> `of which 1 is a creature` (Hive of the Eye Tyrant animated) plus a
  new `INCOMING THIS COMBAT` line and Devour Flesh going from `[castable now]` to having a target.
  **A legal continuation genuinely moved; the ask was owed.**
- **T2 / CZ — the stop-reached collapse: PASS, and it hid nothing.** All 8
  `stop_reached_windows_skipped` and all 5 `stop_reached_reputs_collapsed` in the corpus are in
  `...deck162` **turn 12, after seq 127** (`x20` -> M=24 against the stop=24 stated at seq 98 and
  re-stated at 115/127). The wave-72 arm's `planTurn == nowTurn` gate is satisfied there (stop
  stated and reached on the same turn), which is why both arms fire on the same turn — 3 old-arm
  skips and 5 new-arm collapses. **No collapse hid a legal option**, for two independent reasons:
  (a) the new arm requires `everyBaseRowStopPriced` (`AIPlayerGPT.cpp:36244`, call site 44004), so
  a single non-create live row forces the ask — the Transform row that Bloodline Keeper's second
  ability could have offered would have done exactly that; and (b) at those windows the seat had
  **0 untapped sources** (seq 129's Tragic Slip printed `casting this taps you out`), so `{B}` for
  the Transform was not payable and the only live rows were the free-tap Create rows, all past the
  seat's own stop. **Why 5 and not the predicted 20-35: the state arose on one turn of six games.**
  `stop=` is written in 3 of 495 replies; five of the six games never had a maker with an untapped
  window, so there were no other chains to collapse. This is a fewer-chains answer, not a refusing
  arm — the arm fired on 5 of the 5 re-puts that occurred.
- **T10 / DB — prompt bytes: PASS.** Per-game max 20,286 (vs152) / 19,926 (vs126) / 18,795 /
  18,348 / 17,458 / 12,804; **1 of 495 over 20 KB** against 124 of 571 last wave. I found **no**
  prompt that lost the seat's own last PLAN (`THE PLAN YOU LAST STATED` present on every window I
  read, including the 20,286 B one) or a stated stop; the `stop=24` carried correctly from seq 98
  through seq 127 and printed in the `{right now:}` clause at both 115 and 127.
- **T11 / DC F7 — the `{effect:}` and `{card text:}` glosses: PASS at this seat.** 0 mid-clause
  cuts in the distinct glosses I read; every `(...more)` lands after a whole clause
  (`{T}: Put a 1/1 white Human creature token onto the battlefield. (...more)`; `Flying -- {T}: Put
  a 2/2 black Vampire creature token with flying onto the battlefield. (...more)`; `Whenever Nadaar,
  Selfless Paladin enters or attacks, venture into the dungeon. (Enter the first room or advance to
  the next room.) (...more)`). The Marsh Flats text is hoisted once per BODY at `...deck126`
  seq 276 (`Card text shared by options 1-4 ... printed once here instead of on each of them`) and
  never repeated inside a row.
- **T8 / DB — `HELD:` on a targetless instant: PASS.** e.g. `...deck130` seq 93,
  `Tragic Slip {b} [instant] [no cast row now: it must have a target and there is no legal target
  on the board - HELD: this is an instant, so it is in your hand and castable the moment a legal
  target appears, including on their turn; this tag is about THIS window, never about your hand]`.
  True on that board.
- **T6 — `{crack-back cover:}` subtracts from the header's own total: PASS** on all 4 I recomputed
  (`...deck162` seq 42 `4 - ... leaving 0 -> you would be at 16`; `...deck146` seq 24 twice,
  `4 - 1 = 3 ... you could still be as low as 14` and `takes 4 off the total, leaving 0 -> 17`).
- **T13 — the drain clause: 8 prints, all scoped to the live stack**, all in `...deck162`.
- **S12 — `Opponent life trend`: PASS on the 3 I recomputed**, including the split form at
  `...deck125` seq 117 (`+5 since turn 24; life-gain EVENTS put +5 on them and life-loss EVENTS
  took -0 off`) against a log of one 5-point Elixir gain.
- **The harness: clean.** `http=200` on 495/495, `deadline_pct` max 40.7, 0 truncations,
  4 `forced_close` records — two `armed` / `dropped_decision_moved` pairs (`...deck126` seqs 51/52
  on window_seq 16, `...deck152` seqs 81/82 on window_seq 44), no answer lost in either.

**UNTESTED from this seat (0 renders in 495 decisions):** `[own loop verdict:` (T4/DC F10 —
`own_loop_windows_asked` 0, `own_loop_verdict_lines_rendered` 0), `hold_reopened_new_threat`
(DC F2), `AT BEST` and the whole blockers header family (T5/T9/DA — `blocker_forecast_rows` **4**,
0 GANG BLOCK verdicts, 2 attacker declarations in six games), `UNLESS <source> untaps them`
(T7/DC F8, 0 prints), keep-X / COUNTERSPELL pricing (DC F6, 0), `[RE-ASK]` (0 — second wave),
`menu_pass_no_progress_suppressed` (0), `[X pricing]` and loyalty rows (0), MDFC faces (ninth wave
with none in a deck123 hand), `identical_option_asks_resolved` (0), `repeat_annotated_takes` (0),
`mana_only_windows_skipped` (0), `main_phase_windows_skipped` (0), `phase2_answer_recovered` (0).

## 3. Which wave-78 items recur

| wave-78 item | wave-79 status |
|---|---|
| **HIGH-1**, the stop-reached collapse dead behind `planTurn == nowTurn` | **CLOSED.** `w79StopReachedRePutCollapses` drops the turn gate (`AIPlayerGPT.cpp:36244`); 5 of 5 re-puts collapsed, 8 windows skipped, and the collapse hid nothing (§2). E1 CHECK -1 and E2 CHECK 3 are separately addressed below. |
| **HIGH-2**, the hold latch under a moving board | **CLOSED at this seat as a re-ask engine**: byte-identical same-seam re-asks fall from runs of 28-in-a-turn to **3 in the whole seat**. The latch's *new* half (`hold_verdict_safer_ignored`) is unobservable here — MED-3. |
| **HIGH-3**, the 77-window loop turn | **DOES NOT RECUR.** The vs126 loop turns this wave run 4-9 windows, not 77; `LOOP RUNNING` prints 93 times across the game rather than in one turn. |
| **MED-4**, the drain clause's false hold promise | **Not re-observable**: 8 drain prints, 0 followed by a byte-identical re-put at the same seam. |
| **MED-5 / MED-6**, the 49-window Create no-op and the cross-phase re-puts | **MED-6 RECURS AND IS NOW THE SEAT'S LARGEST WASTE** — `crossphase_board_unchanged` 0 -> **22**, the bracket TRUE every time (HIGH-2 above). The Create no-op does not recur (4 Create windows total). |
| **LOW-7**, a two-step target ask losing the plan's target | **Not re-observed** (5 edict casts, every target ask carries a plan naming the same side). |
| **wave-78 edit 1** (CHECK -1 de-scoped from the Alarm, "Two damage is not this deck's clock") | **PAID AND HONOURED.** Lone-maker attack windows this wave: 2. `...deck162` **seq 116** answers `ATTACK: none` into Fog Bank; `...deck152` **seq 89** sends the lone Keeper with the reasoning *"I am going to die next turn regardless"* — the row's own crack-back agreed. 1 of 2 by the letter, 2 of 2 by the board. Wave 78 was 3 of 7. **KEEP.** |
| **wave-78 edit 2** (CHECK 3 de-conditioned from the Alarm) | **UNTESTED.** 2 attack declarations in six games, neither with two makers on the board. No window could exercise it. |
| — new — | the sweep marker's one-sided editorial (HIGH-1), the cross-phase full-price re-ask (HIGH-2), the unrenderable safer-verdict census (MED-3), **and the P0-vs-veto contradiction in the guide itself (§4)**. |

## 4. Guide verdict: **EDIT**

Revised guide at `wave79/deck123/strategy.txt` — **19,974 bytes** (live guide 19,978; ceiling
20,000). **The guide shrinks by 4 bytes; no rule earns new bytes on net.** A332 grep (`seq`,
`wave`, `corpus`, `game N`, deck numbers, turn citations): 0 hits. `YOUR PLAN`: 0. A334 audit
(`even when` / `even if` / `regardless of` / `no matter` / `however` / `although`): 0. Both PLAN
templates are one physical line each. Headings unchanged, each declaring its own scope (A333).

| # | before -> after | seq that paid for it |
|---|---|---|
| **1** | **P0 carries the DAMNATION veto, at the list the model actually reads.** ` P0 Damnation {2}{b}{b} - your line reads "of which 0 are creatures" and the row's K is 2+. / Ahead of any maker, Tutor or Alarm.` -> ` P0 Damnation {2}{b}{b} - ahead of any maker, Tutor or Alarm, on two conditions read off the row / before the rank: your line says "of which 0 are creatures", and the row's K is 2+. A maker of / yours inside the row's "M of yours" list takes this entry off the list.` The old entry stated the gate as a description ("your line reads X") and the rank as an imperative, and the model kept the imperative and dropped the description in its own words. The new entry puts the rank first, names its conditions as conditions, and restates the veto at the first-match list the casting window is read from — A333's own remedy for a rule filed under another heading, and A24/A51's "a rule gaining an exception gives it to every line summarising it". **+114 bytes.** | **`...deck162` seq 42.** The reasoning quotes P0 verbatim including `"of which 0 are creatures"` and then writes *"So Damnation is priority 0"* on a line reading **1** creature — its own Thraben Doomsayer, printed in the row as `YOURS: Thraben Doomsayer (2/2)`. The engine restarted four turns late and the seat died two turns short with 24 attackers on board. |
| **2** | **The arithmetic stops contradicting the veto three lines above it.** `N over M with K at 1+: cast it.` -> `N over M with K at 1+ and no maker in M: cast it.` Two rules quantify over the same card set and the guide did not read them against each other: the veto says a maker in `M of yours` is an absolute refusal; this bullet says cast at N>M with K>=1, which is **true of seq 42's board** (N=2, M=1, K=1). A pilot that reaches this line has a written licence for the play the section's own headline forbids. **+18 bytes** (+2 for the rewrap). | **`...deck162` seq 42** — same window. N=2, M=1, K=1: the old bullet is a positive instruction to make that cast. |
| **3** | **The PLAN template names N.** `this window <x<N> or pass>` -> `this window <x<stop-M> or pass>`. The subtraction is stated once under the repeat-row bullet and the model writes the template without it, then picks a count by feel. Putting the arithmetic inside the line it must write is the skill's "where one number governs, the number is WRITTEN in the PLAN line". **+5 bytes.** | **`...deck162` seq 98** wrote `stop=24, M=2; this window x3` (the rule's N is 22) and **seq 115** wrote `stop=24, M=4; this window pass` against a printed `this window has 20 to add before it reaches your own stop`, reasoning *"I already have 3 tokens. It doesn't change much."* Both are the same miss: the template asked for a count and not for the difference. |
| — | **Paying for edits 1-3 (-141, no rule removed):** the `[RE-ASK]` sentence deleted from the stop section — ` "[RE-ASK] ... your own / PLAN puts you at <M> with your stop at <S>" -> "CHOICE: 0 (pass)": not the same count, not a / smaller one, not a new stop invented to fit it.` The string `[RE-ASK]` prints **0 times in 495 prompts this wave and 0 times in 571 last wave**; `identical_ask_answers_reserved` is 0 corpus-wide and the re-ask path did not fire once at this seat in two corpora. A rule keyed to a token that never renders is inert bytes. The surviving sentence next to it (`Recompute from THIS window's row ... rebuild it from L`) keeps the stale-stop case. | — |

**What I deliberately did NOT change.**
- **CHECK -1 and CHECK 3** (wave-78 edits 1 and 2). CHECK -1 was honoured on the board that tested
  it (162 seq 116) and the one apparent violation (152 seq 89) had no better alternative on the
  row's own crack-back figure. CHECK 3 got no window. A rule is rewritten by a decision that went
  wrong.
- **The counterspell bait gate.** vs125 seq 110 lost the game's only maker to Essence Scatter, but
  the gate's trigger ("a counterspell in their log or graveyard") is **absent from that prompt** —
  no counterspell had been seen — and a dead Greaves cast first is not bait for a spell that only
  counters creatures. Widening the trigger to "any untapped sources" would fire on most of this
  deck's maker casts and would not have saved this one.
- **`of which 0 are creatures` / the Alarm-on-an-empty-board cast.** Broken twice (146 seq 167 t24,
  130 seq 118 t17), both times with 7+ untapped sources, an empty hand of threats and nothing else
  to do; neither cast cost a number on the board. The rule is not the reason those games were lost.
- **The `[cost: Tap]` single-Create governance bullet and the stop's L+C+3 arithmetic.** Both were
  computed correctly every time they were exercised (98/115/127 all rebuild stop=24 from
  `20 + 1 + 3`).
- **The blocking and gang-block sections.** 4 `blocker_forecast_rows` in six games; no evidence.

## 5. Optional proposals

**None.** Edits 1-3 are this deck's own wording, and the discipline each one restores is already
written in the installed skill (A333's heading-scope remedy, A95/A241's "two rules quantifying over
the same card set are read against each other", A115/A330's "the number is WRITTEN in the PLAN
line"). A general-guide or skill proposal restating them would be noise. HIGH-1 and HIGH-2 are
engine items; MED-3 is a counter that needs a render before any seat can review it.

## 6. What I did NOT check

- **Opponent seat files were opened for board context only** and I read no opponent transcript at
  all this wave — every board fact above is read off my own seat's `prompt` field (the GAME LOG and
  CURRENT SITUATION blocks). **I audited no opponent decision.**
- **I adjudicated no lane prediction corpus-wide.** §2 answers T2, T3/DC F1, T6, T8, T10, T11/DC F7,
  T13 and S12 **for this seat's six games only**; T4/DC F10, T5/T7/T9/DA, DC F2, DC F6, DC F8 and
  the X/loyalty family had zero renders here and are listed UNTESTED rather than PASS.
- **The 5 stop-reached collapses are located by reasoning, not by a trace.** The engine's
  `DebugTrace` for the collapse goes to stderr and **no stderr file was opened**; skipped priority
  windows consume no `seq` and appear in no log. I placed them at `...deck162` turn 12 after
  seq 127 from the only board in the corpus where `carriedStop >= 0`, `M >= stop` and
  `declinedN >= 1` all hold, and I read `w79StopReachedRePutCollapses` and its call site to
  establish that a non-create live row would have forced an ask. I did **not** read
  `w79RePutCollapseIdentity` or `w79AskScopeKey`'s digest construction, so "a token entering does
  not move the continuation digest" (MED-4) is my inference from the replay runs, not a reading of
  that code.
- **I did not verify all 525 replays.** I read the full `askreplay/` listing for vs162 (77 entries,
  every `replayed_from`/`replay_run`/`choice`), sampled the other five, and confirmed all 525 are
  `why: cache_replay` on a "Casting decision" seam. **I re-derived no replayed answer against its
  live board**, so "no replayed answer was wrong for the new board" is unproven, not established.
- **1 of the 22 `ask_key_continuation_differs` events was verified in full** (146 seq 98 vs 100);
  the other 21 were not paired.
- **The 13 `hold_verdict_safer_ignored` events could not be located at all** — no matching face
  renders at this seat. I read the incrementing site (`AIPlayerGPT.cpp:35476`) and stopped there.
- **The 3 `async_drops` were not traced to re-asks**, the `forced_close_unrecorded` 2 were not
  reconciled against stderr, and the 25 `hold_released_turn` events were not paired to their
  holding windows.
- **I read the `reasoning` of every window cited above and of the 5 board-sweep windows**; I did
  not read the reasoning of the 327 skipped-hold windows, the 93 `LOOP RUNNING` windows beyond
  their answers, or the 22 board-unchanged windows beyond their answers.
- **I opened no card script, ran no build, ran no wagic, ran no git**; nothing under `bin/Res`,
  `src/` or outside `wave79/deck123/` was written.
