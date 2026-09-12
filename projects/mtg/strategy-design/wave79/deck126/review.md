# Wave-79 deck126 review — SANGUINE BLOOD (WGB wall prison, Exquisite Blood + Sanguine Bond)

Corpus `matchups-20260912-000748-final`, SINGLE binary `wagic-5af6f7a95-w79step1`, one run, all 21
games natural. **3/6** (wave 78: 4/6; 77: 1/6; 76: 3/6). Live guide under test: the wave-78 edition
installed at a37b2d5ce (19,985 B). **Hands are real — I do not read 4/6 -> 3/6 as guide effect.**
All three wins were pressed on the WIN BUTTON's Staff-of-Nin bullet in the FIRST window that offered
it; all three losses are boards where the pair never closed, or closed with no starter in the deck's
reach. No decision in six games went wrong against the guide in a way that changed a result.

**Seat census.** 6 files, 216 records, **192 with a prompt**. Kinds: ask 169 / priority 7 /
blockers 5 / reveal 9 / bottom 3 / forced_close 6 (+ gamestart/system/gameend). `thinking: on` on all
198 records carrying the field; **192/192 replies with native reasoning** (median 5,090 chars, max
14,556; 0 zero-reasoning) — invariant 000 holds at this seat. **0 fallbacks** of every kind, **0
`reply_truncated`, 0 `action_before_plan`, 0 `post_answer_overrun`, 0 `off_protocol_bytes`, 0
`plan_line_missing`, `protocol_deviation` = compliant 192/192** (wave 78 at this seat: 3 deviations).
Transport `curl=0,http=200,empty=0` on 192/192. Latency p50 **92.9 s**, p95 188.9 s, max 359.5 s;
`deadline_pct` p50 10.3, max 39.9 — no deadline pressure. Prompt bytes p50 **11,293**, max **20,359**,
**1 of 192 over 20 KB** (wave 78 at this seat: max 32,777, 131 of 309 over 20 KB). `async_drops` 1
(deck125), re-asked.

Counters, per game (152/162/130/123/146/125): `ask_key_continuation_differs` 3/1/5/8/2/2;
`ask_replays_reserved` 5/5/0/36/4/12 with `identical_ask_answers_reserved` **0 in all six**;
`hold_windows_skipped` 5/13/0/200/0/222; `hold_released_turn` 0/0/0/9/0/1;
`hold_verdict_safer_ignored` 0/**3**/0/**32**/0/0; `hold_reopened_new_threat` **0 in all six**;
`own_loop_verdict_lines_rendered` 0/**5**/**4**/**42**/0/**12** (= **63, the whole corpus's 63**);
`own_loop_windows_asked` 0/0/0/1/0/3 (= **4, the whole corpus's 4**); `stack_drain_windows_asked`
0/**6**/0/0/0/0; `mana_only_windows_skipped` 11/20/30/68/0/0; `own_turn_windows_skipped`
0/0/43/14/0/45; `crossphase_identical_reputs` 0/2/0/2/0/0 with `crossphase_board_unchanged` **0 in
all six**; `plan_names_uncastable_zone_card` 0/0/0/0/**3**/0; `plan_names_stranded_card` 0/0/0/1/1/0;
`forced_close_events` 0/2/2/0/0/2, `_unrecorded` 0/1/1/0/0/1. **0 in all six**:
`stop_reached_windows_skipped`, `stop_reached_reputs_collapsed`, `phase2_answer_recovered`/`_missing`,
`menu_pass_no_progress_suppressed`, `repeat_annotated_takes`, `identical_option_asks_resolved`,
`blocker_forecast_*`, `declined_face_latches` (retired).

**Play quality.** Land drop **51 of 52** (the one decline, `126v146` seq 17, was recovered in Main 2
the same turn). THE ZERO STOP: **48 exposures, 0 takes**. `[second copy:` **12 exposures, 1 take** —
a second Chromatic Lantern (`126v130` seq 11), the known guide over-reach that wave 78 re-rejected;
both Exquisite Blood and both Sanguine Bond second copies were declined. `NO LIVE CAST ROW`: 27
windows, hold row **23**, `Cast nothing right now` 4. Max `[you declined this exact list N times]`
reached **N=3** (wave 78: 4; wave 76: 31) — no window loop anywhere. Menus printing a live Sorin
loyalty row and answered with a hold: **2** (wave 78: 7), and one of the two is the window the engine
itself told the seat to hold on (see §2 HIGH-1's RESOLVING face).

## 1. Game by game

| # | opp | result | deciding decisions |
|---|-----|--------|--------------------|
| 1 | **152** | **LOST** −7 to 18, t17 | **Not decidable.** Mulliganed a seven, kept the six, bottomed the duplicate Perimeter Captain (seqs 2-4) — the bottoming order exactly. Blocked with Wall of Omens in all three combats it was offered (seqs 11, 15, 21), each the guide's STEP 1 "lives" pick. Sanguine Bond landed t12 (seq 20); **Exquisite Blood was never drawn or tutored in 17 turns** and no Idyllic Tutor was ever offered. At **seq 24 (t16, life 8)** the only cast row was a SECOND Sanguine Bond and the seat declined it correctly (`[second copy:`), then held (seq 30). Three Tributes were spent at t8/t10/t14 on "they choose which one" rows — see the structural note below §3. |
| 2 | **162** | **WON** 21 to 0, t11 | **The guide's whole line, executed from 2 life.** Mulligan a seven, keep the six, bottom a surplus Tribute (seqs 2-4). Tutor t5 (seq 10) -> **Sanguine Bond** (seq 11, rule 4's "holding neither, pick SANGUINE BOND"); Bond t7 (seq 23); Exquisite Blood t9 (seq 27) — `LOOP COMPLETE` from seq 28. At t11 the seat is at **2 life** under `LOOP COMPLETE`, plays the land (seq 30), casts **Staff of Nin** (seq 31, entry 8 — Staff to the top once both names are out) and in the very next window **seq 32 `Deal 1 damage with Staff of Nin targeting the opponent`** closes it 20 -> 0. Zero windows wasted. |
| 3 | **130** | **LOST** −3 to 15, t36 | **`LOOP COMPLETE` from t27 and no starter existed — the game that falsifies the verdict line (§2 HIGH-1).** Tutor t9 -> Bond (seqs 13/14), Bond t11 (seq 16), second Tutor t13 -> Exquisite Blood (seqs 18/19). Blood was not castable until t27 ({4}{b} against 3-5 sources). **seq 28 (t25, life 8)** Tribute on a NAMED Rorix Bladewing (6/5 flying), gaining 5 — forced and correct: the row's own cover clause reads `you could still be as low as 8, which you SURVIVE`, and without it the crack-back put the seat at 2 with Rorix alive. Blood lands t27 (seq 30). From seq 31 the prompt prints `LOOP COMPLETE` AND `[own loop verdict: your life loop is a proven win]` on **t27, t29, t33, t35** — and the seat's only remaining life-gain source is **Pride Guardian's block trigger, which cannot block the flier that is killing it** (the engine says so itself at seq 36: `1 of their 1 could not legally be blocked by it at all`). The last Tribute was spent at t25, Staff and Sorin never appeared, and the seat has no attacker (all sixteen creatures are Defenders). It cast a wall every turn — exactly the guide's `with both names out and no starter available CAST SOMETHING EVERY TURN` — and died at t36. **No better play existed in any window after t27.** |
| 4 | **123** | **WON** 38 to 0, t37 | **The longest correct wait in the corpus.** Tutor t5 -> Bond, Tutor t7 -> Blood (seqs 7-11), Bond t9 (seq 14), Blood t13 (seq 19) — `LOOP COMPLETE` from seq 20. Then **twelve turns of a locked board**: the opponent never declared an attacker (0 blockers asks at this seat), their creature count stayed at 0 so every Tribute row read `at 0 this does nothing` (**declined 30 times**), and there was no Staff, Sorin or Vampire. The seat held or declined every window and cast the walls the guide's no-starter clause names. At **t37 it drew Staff of Nin, cast it (seq 83) and activated it in the first window that offered the row (seq 96 `Deal 1 damage with Staff of Nin targeting the opponent`)** -> 17 to 0 in one window. Cost of the wait: nothing — nothing else was ever legal. |
| 5 | **146** | **LOST** −3 to 21, t15 | **Not decidable; the guide's own rungs were applied correctly at every window.** Tutor t6 -> Bond (seqs 7/8). Pride Guardian blocked Silverquill Silencer at t7 (seq 9, +3 life) — the `{blocking trigger, this combat: you gain 3}` rule. Tributes t8/t10. At **seq 16 (t12, life 14)** the menu was Chromatic Lantern or Sanguine Bond tagged `[NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life ... you would be at 11]`, under a crack-back of 12 with the seat controlling NO creature. **Wave-78 edit C's rule (`PAY THE 3 ... WHEN IT IS SANGUINE BOND`) fired and was obeyed** — and it was right: the Lantern was the only alternative and changed nothing; at 14 or at 11 the seat was dead on the same turn. The **one rule violation in six games** is **seq 17**: `Play no land right now` at 2 life, taken on a plan that named an Idyllic Tutor the seat did not hold (`plan_names_uncastable_zone_card` 3 in this game); the land was played in Main 2 (seq 24), so it cost a window and nothing else. Final combat **seq 25 (life 1, 11 incoming, one blocker)**: the seat blocked A3 `Silverquill Silencer (4/3) deals 4`, the biggest `deals N` its blocker could legally block — the guide's `this KILLS you` rule character for character, and the engine's own header agrees `no block saves you`. |
| 6 | **125** | **WON** 44 to 0, t20 | Two Tutors (t6 -> Blood, t8 -> Bond), Bond t10 (seq 15), first Blood t12 (seq 17) — **countered or removed**, because `LOOP HALF PENDING` returns at seq 20 and a third Tutor at t14 fetched a second Blood (seqs 18/19) which landed t16 (seq 21) -> `LOOP COMPLETE` from seq 22. Sorin cast t18 (seq 24). At **t20 the seat cast Staff of Nin (seq 30), declined the dead cast menu (seq 31) and pinged the opponent (seq 43)** -> 29 to 0 through the loop. **seq 45/46 are the corpus's only `RESOLVING` own-loop verdicts** and the seat held on both, with its own Exquisite Blood trigger on the stack — the engine's verdict and the seat agreed, and the game ended there. |

**Where the guide earned the three wins.** All three closed on THE WIN BUTTON's Staff bullet in the
FIRST window that offered it (162 t11 from 2 life, 123 t37, 125 t20), 0 windows wasted after
`LOOP COMPLETE` printed. Entry 1 and entry 2 (Tutor when a name is missing from BOTH lines) drove
**six Tutors across five games, each cast the first turn it was listed**, and rule 4's "holding
neither, pick SANGUINE BOND" was followed on every search that had the choice. Entry 8 (Staff/Sorin
to the top once both names are out) fired in all three wins. THE ZERO STOP held 48 of 48.

## 2. Engine / interface / card items

**HIGH-1 — T4 LANDED, AND THE VERDICT IT NOW RENDERS ASSERTS A FALSE FACT: `your life loop is a
proven win` PRINTED FOUR TIMES IN THE GAME THIS SEAT LOST, ON A BOARD WITH NO STARTER.** Repro
`1789189696-ai_baka_deck126-0x55f60fcf91e0-vs-ai_baka_deck130.jsonl` **seqs 31, 32, 35, 36** (t27,
t29, t33, t35; life 9, 9, 9, 3). Every one prints, verbatim:
`[own loop verdict: your life loop is a proven win, and no link of it is on the stack right now -
nothing of it is waiting to resolve, so a hold here covers no link of it]`. The seat lost at t36 at
−3. The loop was assembled and the pair never left the battlefield; what did not exist was any way to
START it — the seat controls sixteen Defenders (no attacker), its last Tribute was spent at t25, no
Staff or Sorin was ever drawn, and its only life-gain source (Pride Guardian's block trigger) could
not block the flier, which **the same prompt says at seq 36**: `1 of their 1 could not legally be
blocked by it at all (evasion, protection, or a block needing more bodies than this row adds)`. The
engine already computes the qualifier two lines above: the `LOOP COMPLETE:` header says
`any one point of either, from any source, ends the game in your favour` — but nothing checks whether
a source is on the seat's board, hand or stack before the verdict promotes that conditional to
`a proven win`. Under the trust doctrine a rendered statement is an instruction, so this one
instructs the seat that it has already won for the nine turns in which it is being killed.
Falsification: any `[own loop verdict:` line in a prompt whose seat holds no life-gain or
opponent-life-loss source. **Fix shape**: the verdict names the starter it is proven by
(`proven win — your Staff of Nin's ping starts it`) or states the gap
(`both halves are out and NO source of yours can start it: nothing on your board, in your hand or on
the stack gains you life or takes theirs`). Counting rows it already prices (Tribute victims, ping
rows, lifelink attackers, blocking triggers) is the same search the WIN BUTTON bullets enumerate.

**HIGH-2 — EVERY `[own loop verdict:` LINE IN THE CORPUS IS AT THIS SEAT, AND 56 OF THE 63 THE
COUNTER CLAIMS WERE ACTUALLY RENDERED.** T4 is otherwise **PASS**: 56 lines print in prompts across
four games, all on ASKED prompts, and **all 56 are TRUE against the stack** (method: for each, pull
the prompt's `ON THE STACK, waiting to resolve` block; **48** of the `none` faces have no stack block
at all and **6** have a stack holding no link of this seat's loop — `Howling Mine's Draw 1` +
`Ob Nixilis's triggered ability` (`126v162` seq 29), `Intruder Alarm's Untap` (`126v123` seqs 49, 72),
`opponent's Vision Skeins` (seq 79), `opponent's Essence Scatter` over `your Overgrown Battlement`
(`126v125` seq 27); the 2 `RESOLVING` faces
(`126v125` seqs 45, 46) sit on a stack whose top is `ability: Exquisite Blood's Life [from your
Exquisite Blood]` — a genuine loop link, nothing of theirs targeting it — and the game ended 44 to 0
in that window). **One `none` face is true only on the narrow reading**: `126v123` **seq 97** prints
`none` while the stack's top is `ability: Staff of Nin's deal 1 damage [from your Staff of Nin]` —
no link of the Bond/Blood pair, but the seat's own IGNITER, the very trigger that was about to win
the game (it did, in that window, 17 to 0). If the verdict's purpose is to tell the seat whether a
hold covers what is resolving, its scope should be the starter as well as the links.
Only **two faces occur in the whole corpus: `none` (54) and `resolving` (2)** —
`THREATENED` and `UNPROVEN` never printed, so CY F4's silencing risk is **UNTESTED**, not discharged.
**The defect**: `own_loop_verdict` is stamped on **7 records whose prompt carries no verdict line at
all** — `126v162` seq 30, `126v123` seqs 20, 30, 32, 44, 61, `126v125` seq 23 — **every one of them a
LAND DROP menu**. All 7 are stamped `[own loop verdict: none]`, i.e. the face was not cleared. So the
gameend counter `own_loop_verdict_lines_rendered` reads 63 where 56 lines were rendered, and DC F10's
"suppressed windows clear the face" does not hold on the land-drop seam. Falsification: any record
with a non-empty `own_loop_verdict` whose `prompt` lacks `[own loop verdict:`. Fix: clear the face on
any window the render suppresses, or rename the counter to what it counts (verdict COMPUTATIONS).
Related, and the reason this matters more than 7: **`own_loop_windows_asked` is 4 and
`own_loop_verdict_lines_rendered` is 63** — those two counters are measuring different populations
with names that read as the same one.

**MED-3 — THE HOLD ROW'S BAN ON HOLDING OVER A LIVE ACTIVATED ROW AND THE `RESOLVING` VERDICT NOW
CONTRADICT EACH OTHER ON THE SAME MENU.** Repro `126v125` **seq 45** (ask) and **seq 46** (priority),
t20. Both menus print a live `+1: create a 1/1 vampire with Sorin, Lord of Innistrad` row AND
`[own loop verdict: RESOLVING - a piece of your own proven life loop is on the stack and nothing of
theirs targets it: the loop closes without another action from you, and the HOLD row covers every
remaining link]`. The seat took the hold row on both and won in that window. The deck guide states
`NEVER HOLD ON A MENU THAT PRINTS ONE` as an absolute; the render tells it to hold. The model
followed the render, which is the right outcome and the right precedence — reported so the core loop
knows the two surfaces now disagree by construction, not so a guide hedge gets written (see §3).

**MED-4 — T11 RESIDUAL: THE PINNED CARD IS STILL CUT MID-CLAUSE, AND NOW CARRIES BOTH A BARE `...`
AND A `(...more)`.** Repro `1789189673-ai_baka_deck126-0x55bc7ab36dd0-vs-ai_baka_deck152.jsonl`
**seq 15** (and seq 9's Ranger Class in the same file). Sigarda renders
`"Flying, trample -- Humans you control get +1/+1. -- Coven - Whenever Sigarda attacks, if you
control three or more creatures with different powers, look at the top five... (...more)"` — the cut
is still inside the Coven clause, after `top five`, exactly where wave 78 MED-6 and §T T11 put it.
What DID land: the `(`/`"` closing (Ranger Class now reads
`"(Gain the next level as a sorcery to add...) (...more)"` with its parenthesis closed) and the
`(...more)` marker itself. 13 distinct `(...more)` sites at this seat, **0 with an unclosed bracket or
quote**; this is the only one cut mid-clause.

**MED-5 — THE DECKLIST GLOSS FOR SORIN, LORD OF INNISTRAD DROPS TWO OF HIS THREE LOYALTY ABILITIES,
IN ALL SIX GAMES.** Repro: every system prompt in the corpus at this seat — `126v162` seq 11,
`126v130` seq 14, `126v123` seq 8, `126v146` seq 8, `126v125` seqs 10 and 13 — both copies render
`{text: +1: Put a 1/1 black Vampire creature token with lifelink onto the battlefield. (...more)`.
The `-2` and the `-6` are never shown. The `-6` is the card's sweeper, and the deck guide devotes a
paragraph to taking it over the `+1`; the model can read that teach and never see the ability in the
decklist it is given. T11/DC F7's clause priority (triggered/activated before keyword/static) cannot
separate these — all three are activated — so the cap cuts by length and always keeps the first.
Fix shape: for a multi-ability loyalty/level body, budget the cap per ability rather than per body,
or keep the LAST clause as well as the first.

**MED-6 — `stack_drain_windows_asked` COUNTS 6 IN `126v162` BUT ONLY 3 WINDOWS RENDER THE CLAUSE.**
The 3 that render are seqs **13, 25, 29**, and all three are **TRUE and T13-clean**:
`[their stack is draining N triggers - each link will put this same list to you; HOLD (Hold priority)
covers every link OF THIS STACK, and is released at your next untap - a new stack on a later turn
asks you again ...]` with N = 2, 3, 4 matching the stack height in the same prompt, and the
this-stack scoping present. The other three increments have no rendered clause and no per-record
field, so they cannot be named — the identical shape wave 78 reported as MED-5, at a different ratio.

**LOW-7 — TWO GAMEEND COUNTERS AT THIS SEAT HAVE NO PER-RECORD FIELD AND CANNOT BE LOCATED.**
`hold_verdict_safer_ignored` **35** (`126v123` 32, `126v162` 3) and `ask_key_continuation_differs`
**21** (all six games) are on the gameend record only; no record field and no rendered marker joins
them to a window. I can neither confirm the ignored verdict was safer nor verify that a legal
continuation actually changed. Same for `chain_selfharm_rows_cast` (6/12/3) and
`chain_windows_only_selfharm_cast`.

**Wave-79 lane items, adjudicated on this seat's six games.**
- **T3/CZ + DC F1 — the CG shape: PASS, 0 occurrences.** Method: group every `ask`/`priority` prompt
  by (turn, phase, kind), compare the menu section byte-for-byte and after collapsing every balanced
  `{…}`/`[…]` group. Across **192 prompts, 0 byte-identical and 0 strip-identical re-asks** (wave 78
  at this seat had the same measurement come out non-zero corpus-wide). `identical_ask_answers_reserved`
  is **0 in all six** against `ask_replays_reserved` 62 — and I found **no replayed answer that was
  wrong for its board**, because no two same-seam windows in the same phase ever carried the same rows.
- **T12/T13/T14/CZ + DC F9 — `[hold check:` brackets: PASS, 0 false in 151 adjudications.** 167
  brackets at this seat, 16 first-window (`hold_check_ref_seq` −1, and no earlier record exists at
  that seam in the file — all 16 genuine), 151 with a ref. **Every one of the 151 refs resolves to a
  real record in the same file** and **every `N new` / `N gone` figure matches** a recount of the two
  menus (method: rows normalised to the action text before the first `{`/`[`/` - legal targets`, with
  the engine's own equivalences applied — a pass row naming a different next step is the same row, an
  annotation change is the same row). 0 mismatches. The renumbering case of S9(a) is **UNTESTED** —
  no row handle at this seat ever renumbered.
- **T7/DA + DC F8 — `UNLESS <source> untaps them`: PASS, 32 clauses, all one source, all legitimate.**
  All 32 are `126v123`: `UNLESS Intruder Alarm (THEIRS) untaps it first: that untaps creatures
  OUTSIDE the untap step, so a body tapped for mana here ca…`. Intruder Alarm is a live both-sides
  permanent that untaps ALL creatures, so it can untap this seat's tapped mana creature — never a
  spent ETB. 0 violations.
- **T5/T6/DA + DC F3/F4/F5 — blockers headers: PASS on all 5 headers at this seat.** Note the string
  is `best case with every blocker assigned`, not `AT BEST`. Verified by recomputation:
  `126v152` seq 11 (20, 2 incoming, Wall blocks Wolf -> 20 ✓), seq 15 (21 -> 21 ✓), seq 21 (19, 8
  incoming of which 5 from an unblockable flier -> 14 ✓), `126v146` seq 9 (`20 before your blocking
  triggers, 23 after the 3 they gain you` ✓ — the trigger fold is correct and stated separately), and
  seq 25 (life 1, 4 attackers, 1 blocker: `you would be at -6 before your blocking triggers, -3 after
  the 3` ✓, `no block saves you` ✓, `your creatures can legally block at most 1 of those 4 attackers`
  ✓). **No header prints two disagreeing verdicts** — seq 25 prints two assignments but labels their
  objectives (`the one above is the lowest life, this one is the lowest material cost`) and its
  ALTERNATIVE arithmetic (`lets in 9 combat damage instead of 7`) is correct. `126v146` seq 9 also
  carries the GANG BLOCK caveat truthfully (only one blocker existed, so no gang was possible).
  `{crack-back cover:}` — 16 clauses, every one recomputed against its own header total and correct
  (`126v130` seq 31: header 5 total, cover 3, `leaving 2 -> you would be at 7` from life 9 ✓;
  `126v146` seq 19: header 17, cover 4, `leaving 13 -> at -11` from life 2 ✓; `126v130` seq 36:
  header 6, cover 0, `at -3, which still KILLS you` from life 3 ✓, and the reason given — the flier
  cannot be blocked by the 0/3 — is true). **Sacrifice-cost rows and past-12×12 drops: UNTESTED** —
  none occurred.
- **T9/DA + DC F5 — ATTACK TOTAL / Lightmine Field: UNTESTED at this seat.** This seat made **zero
  attackers decisions in six games** (all sixteen creatures are Defenders and no Vampire was ever
  made — Sorin reached play once and his `+1` was never activated). `ATTACK TOTAL`, `Lightmine`,
  `N power survives the field` return **0 hits in 192 prompts**. Wave 78's HIGH-2 at this seat cannot
  be re-checked here.
- **T10/DB — prompt bytes and the hoist: PASS at this seat, decisively.** Max **20,359 B**, **1 of
  192 over 20 KB** (wave 78: 32,777 and 131 of 309), p50 11,293. **Exactly ONE prompt in 192 repeats
  a card BODY, and only twice** (`126v123` seq 96, the Staff-of-Nin priority menu) — against wave
  78's eleven rows sharing one `{card text:}`. I checked no prompt lost the seat's own last PLAN: the
  `THE PLAN YOU LAST STATED` block appears on every window that had a prior plan, with its correct
  window-age and out-of-date notes (`126v146` seq 19: `"Exquisite Blood" is no longer on your menu` +
  the zone note naming the library — both true).
- **T15/S12 — `Opponent life trend:` PASS.** 182 lines at this seat; 167 `unchanged at N`, **15
  distinct split lines, all TRUE** on recomputation against the turn checkpoints printed in the same
  line, and **the `+0` face prints on every line where one side is zero** (`life-gain EVENTS put +0 on
  them and life-loss EVENTS took -2 off`, and the mirror `took -0 off` on gain-only spans).
- **T14/CZ — forced closes reconcile with stderr.** 6 `forced_close` records at this seat, in three
  games, each an `armed` + `dropped_decision_moved` pair naming the superseding window
  (`{"arm":"land"|"cast", "event":2, "outcome":"dropped_decision_moved", "unrecorded_so_far":1,
  "window_seq":N}`). `game-126v162-1789189683.stderr`, `game-126v130-1789189693.stderr` and
  `game-125v126-1789189709.stderr` each carry a seat line reading `forced closes: 2 events, 1
  superseded (dropped_decision_moved); phase-2 answer recovery: 0 recovered, 0 missing` — identity
  holds on all three. `declined_face_latches` is gone from gameend (retired) ✓.
- **T2/CZ (stop-reached), T8/DB (`HELD:` on a targetless instant, keep-X naming a counterspell),
  `menu_pass_no_progress_suppressed`, `hold_reopened_new_threat`, `[X pricing]`, folded no-op bands,
  repeat `x<N>` rows — UNTESTED at this seat.** `ALREADY AT OR PAST your own stop`, `HELD:`,
  `[X pricing`, `AT BEST` return 0 hits in 192 prompts; no counterspell, no X spell and no stop-reached
  chain occurred in six games. `hold_reopened_new_threat` 0 with `hold_verdict_safer_ignored` 35 —
  I cannot show whether a new equal-rank threat ever arose, because neither counter has a per-record
  field (LOW-7), so this is **UNTESTED at this seat, not a FAIL**.

## 3. Guide verdict: **KEEP as is** (no `strategy.txt` written)

Nothing this wave is paid for by a decision the corpus shows going wrong. The three losses are a
deck that never saw its second half in 17 turns (152), a deck whose pair closed with every starter
already spent or absent (130), and a deck that was dead to an aggro curve by t14 with the guide's
own rungs correctly applied in every window (146). The three wins each pressed the WIN BUTTON's
Staff bullet in the first window that offered it, from 2 life in one of them. The rules that have
carried this guide held at their best rates yet: **land drop 51/52**, **ZERO STOP 48/48**,
`[second copy:` **11/12**, the dead-menu hold row **23/27**, Tutor-the-first-turn-listed **6/6**,
rule 4's SANGUINE-BOND-first **on every search with a choice**, and the `this KILLS you` blocking
rule executed exactly at `126v146` seq 25. Wave-78's own four edits all paid or held:

- **Edit D (the `LOOP COMPLETE:` / `LOOP HALF PENDING:` header names) — PAID, and it is now the
  guide's load-bearing sentence.** Both headers printed on schedule in all six games, the guide's
  description of them is byte-accurate, and every win was taken under `LOOP COMPLETE`. The false
  sentence wave 78 cut (`NO HEADER ANNOUNCES A COMPLETE PAIR`) would have been actively wrong 56
  times this wave.
- **Edit C (`PAY THE 3 ONLY WHEN THE CAST CLOSES THE PAIR IN THIS WINDOW, OR WHEN IT IS SANGUINE
  BOND`) — EXERCISED once and obeyed**, at `126v146` seq 16 (life 14, Bond tagged by Silverquill
  Silencer, `you would be at 11`). The wave-78 inversion (the model reading a life threshold
  backwards) **did not recur** — there is no threshold left to invert, and the seat's reasoning at
  seq 16 names the Bond carve-out, not a life number.
- **Edit A (`RUNG 0 IS ABOVE IT AND STILL BINDS`) — UNTESTED.** No window in six games printed a
  `that would KILL you` crack-back on a menu that also printed an entry-1 row; at `126v146` seq 16
  the header read `you would be at 2`, not lethal. Neither paid nor refuted.
- **Edits B and E — no exposure.** No Silencer-tagged pair-closing cast, no hold over a menu with a
  row 0 that mattered, no Tutor-count or lifelink-duplicate decision.

**REJECTED edits, with the evidence that rejects them.**
1. **A `RESOLVING` carve-out in THE HOLD ROW** (for §2 MED-3). The model already resolved the
   conflict the right way without guide help — it read the engine's verdict, held, and won in that
   window (`126v125` seqs 45/46). Writing the exception in would weaken an absolute that is obeyed
   25 of 27 times, and A334 forbids stating a rule's hard case as a concession. If the two surfaces
   should be reconciled, the cheaper place is the verdict line (§2 HIGH-1's fix), not the guide.
2. **A land-drop restatement for `126v146` seq 17.** The guide's sentence is already absolute
   (`"Play no land right now" is never your answer while a land is listed`), the row was printed, the
   prompt's own header says `playing a land costs no mana and uses up no cast`, and the drop was
   recovered in Main 2. A printed absolute, read and not applied, is not answered by a fourth
   restatement — the same conclusion wave 78 reached on its three absolutes.
3. **A hold-row restatement for the 4 `Cast nothing right now` answers on `NO LIVE CAST ROW` menus**
   (`126v123` seqs 49, 57, 58; `126v125` seq 31). Both rows are declines; the cost is window economy,
   not play. Worth recording as a mechanism note rather than a rule: at `126v125` **seq 31** the
   model's own plan says `Pass casting to activate Staff of Nin for lethal damage` and at `126v123`
   **seq 58** it says `Hold priority ...` while answering `Cast nothing` — in both the model is
   declining the hold row to protect a later activation, even though the hold row's own text says the
   priority seam `is a different question at a different seam and you will still be asked it`. That
   is a render-comprehension item for the core loop (the hold row is 470 B long and the exemption is
   in the middle of it), not a guide teach.
4. **Narrowing `[second copy:` to exclude artifacts.** One take in twelve exposures, a second
   Chromatic Lantern in a game the seat won; re-rejected on wave 78's grounds.
5. **A rule about spending Tribute to Hunger early on "they choose which one" rows.** Five such casts
   across the two aggro losses (`126v152` seqs 12/16/23 at life 20/21/14; `126v146` seqs 12/13 at
   23/19), and in `126v130` the last Tribute was spent at t25 and was missed at t27. But **holding
   would not have won any of the three**: 152 and 146 never saw Exquisite Blood at all, and in 130 I
   replayed the life line — holding the t25 Tribute leaves the seat at 2 by t27 with Rorix alive, and
   it dies before the pair closes. Worth the core loop's attention as a SEAM shape rather than a
   guide rule: Tribute is this deck's only instant, so it is offered ALONE at upkeep windows where no
   other rung of the cast list is printed, and a ranked list degenerates when only its lowest rung is
   on the menu. The guide's `KEEP THE LAST ONE` rule is the only brake and it binds only once a half
   is already on the battlefield.
6. **Anything about `126v123`'s twelve idle turns.** Zero creatures on the opponent's board (every
   Tribute row read `at 0 this does nothing`, declined 30 times), no Staff, Sorin or Vampire drawn
   until t37, no attacker possible. The seat cast a wall or held in every window and won in the first
   window a starter existed. That is the guide's no-starter clause working, not a stall to fix.

No general-guide proposal and no skill-proposal: every item in §2 is an engine/render item, and the
one guide-shaped observation (MED-3) is a conflict between two surfaces that the engine owns.

## 4. Not checked
I read **no engine source and built no fixture**; every mechanism sentence in §2 is a falsifiable
repro against the logs, not a claim about the code. I did not read the corpus-wide census output
(`~/.gatelogs/w79-stats.out`) — all figures are computed from my six seat logs. I opened **stderr only
for the six games involving this seat**, and only to reconcile the forced-close identity; I contribute
nothing on `Parser returned NULL` noise or on any other game's stderr, and I did **not open the
`askreplay/` directory**. I traced **no opponent seat log**: every opponent action in §1 is read off
my own seat's GAME LOG and board lines, so opponent DECISIONS (why deck123 never attacked for 37
turns, why deck125's counterspell hit the first Exquisite Blood, why deck130 held Rorix back before
t25) are unexamined. I could not adjudicate `hold_windows_skipped` (440 at this seat),
`mana_only_windows_skipped` (129), `own_turn_windows_skipped` (102), `hold_verdict_safer_ignored`
(35) or `ask_key_continuation_differs` (21) — no per-record field exists for any of them, so what
those skips suppressed is not visible from the translog. The 3 unrendered
`stack_drain_windows_asked` increments (MED-6) and the 7 face-stamped-but-unrendered own-loop records
(HIGH-2) are located but **not reconciled**. `plan_step_count` sums 332 against `plan_steps_done` 108
at this seat and I did not audit that ratio. Card verification: I checked **Exquisite Blood, Sanguine
Bond, Tribute to Hunger, Pride Guardian, Perimeter Captain, Wall of Omens, Overgrown Battlement,
Staff of Nin, Chromatic Lantern, Idyllic Tutor and Sorin, Lord of Innistrad** against their rendered
`{card text:}` / `{text:}` in the prompts (Sorin only as far as the gloss shows — see MED-5), and
**Sigarda, Champion of Light** against the truncated gloss only. **Rorix Bladewing, Intruder Alarm,
Howling Mine, Ob Nixilis the Hate-Twisted, Vision Skeins, Essence Scatter, Silverquill Silencer,
Silverquill Command, Briarbridge Tracker, Katilda, Luminarch Aspirant, Ranger Class, Fateful Absence,
Nadaar, Triumphant Adventurer and Hive of the Eye Tyrant were NOT verified** beyond what the render
stated.
