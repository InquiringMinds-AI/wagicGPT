# Wave-80 deck126 review — SANGUINE BLOOD (WGB wall prison, Exquisite Blood + Sanguine Bond)

Corpus `matchups-20260912-074153-final`. All six of this seat's games are on the **20-game leg**
(`wagic-a7a6b8942-w80step1` = cf72a002f); **none** of my games is the `162v123` rerun, so **lanes DI
and DJ are untested at this seat** and the cached-replay re-ask is NOT in my binary. **3/6**
(wave 79: 3/6; 78: 4/6). Live guide under test: the wave-78 edition (19,985 B), `162/126 KEEP` at
wave 79 — unchanged. **Hands are real.**

**Seat census.** 6 files, 291 records, **178 with a prompt** (ask 139 / priority 15 / blockers 6 /
reveal 4 / bottom 1; plus hold_event 36, forced_close 6, attackers 3, gamestart/system/gameend).
`thinking: on` everywhere; **178/178 replies with native reasoning** (median 5,403 chars, 0
zero-reasoning) — invariant 000 holds. **0 fallbacks of every kind**, 0 `reply_truncated`, 0
`action_before_plan`, **0 `plan_line_missing`**, `answer_label_absent_read` 0, `async_drops` 0.
`protocol_deviation`: **177 compliant, 1 `prose_outside_two_lines`** (§2 LOW-7 — parsed correctly).
Transport `curl=0,http=200,empty=0` on all. Prompt bytes p50 10,840; max **22,315**; **3 of 178 over
20 KB, all in `126v146`** (seqs 24/26/27). Per-game prompt bytes p50: 123 6,597 / 130 9,190 /
125 10,840 / 162 10,962 / 152 11,910 / **146 12,018**.

Counters, per game (123/130/125/162/152/146): `ask_replays_reserved` 0/15/52/17/5/0 =
`ask_replays_cache` exactly (`_repeat_latch` 0, `crossphase_replayed` 0 everywhere — **U9 identity
holds, 89/89**); `hold_events` 0/0/26/10/0/0 = 36; `hold_verdict_safer_ignored` 0/0/16/3/0/0 = 19;
`hold_reopened_new_lethal` and `_new_threat` **0 in all six**; `hold_released_turn` 0/3/4/0/0/0;
`hold_windows_skipped` 0/39/159/9/0/0; `crackback_verdict_lines_rendered` 2/2/0/1/8/15 = **28**;
`crackback_lethal_blocked_away` 0 in all six; `stack_death_verdict_lines_rendered` 0/0/2/3/0/0 = 5;
`own_loop_verdict_lines_rendered` 2/10/7/1/0/0 = **20** (= the whole corpus's 20 again);
`own_loop_windows_asked` 0/0/1/0/0/0 = 1 (= the corpus's 1); `plan_cast_steps_closed`
5/10/17/8/6/12 = 58; `forced_close_events` 0/2/0/2/2/0 with `_unrecorded` 0/1/0/1/1/0;
`stack_drain_windows_asked` 0/0/0/2/0/0; `plan_names_uncastable_zone_card` 0/0/4/0/0/0.
**0 in all six**: `crossphase_*`(except 3 `identical_reputs` in 162), `menu_single_outcome`,
`single_outcome_rows_spared`, `identical_ask_answers_reserved`, `stop_reached_*`,
`blocker_forecast_*`, `chain_*`, `wall_miss_*`, `main_phase_windows_skipped`, `put_gloss_stripped`,
`menu_pass_no_progress`, `repeat_annotated_takes`, `engine_reveal_floor_picks`.

**Play quality.** **Land drop 35 of 35** (35 land-drop menus, zero `Play no land` — wave 79: 51/52).
**THE ZERO STOP: 47 exposures, 0 takes.** `[second copy:` **7 exposures, 1 take** (§3). Dead Tutor
(`[finds only ... a copy of one you already control or hold`) **19 exposures, 0 takes**.
Staff-of-Nin upkeep ping: **8 of 8 aimed at the OPPONENT**, never at a permanent of its own, never at
itself. `NO LIVE CAST ROW` menus: 25, hold row **20**, `Cast nothing right now` 5. Max
`[you declined this exact list N times]` reached **N=3** — no window loop.

## 1. Game by game

| # | opp | result | deciding decisions |
|---|-----|--------|--------------------|
| 1 | **123** | **WON** 38 to 0, t11 | Textbook. Tutor t5 (seq 9) -> Exquisite Blood; **Bond t7 (seq 12) from a menu printing BOTH halves — rule 1's `BOTH NAMES MISSING AND BOTH ROWS PRINTED: TAKE SANGUINE BOND`, obeyed**; Blood t9 (seq 14) -> `LOOP COMPLETE`. At **t11 seq 16** the Tribute row reads `Bloodline Keeper (3/3) [flying] is sacrificed, you gain 3 ... THIS WINS THE GAME` and the seat took it in that window. A `[second copy:` Exquisite Blood on the same menu was declined. **0 windows wasted.** |
| 2 | **130** | **LOST** 0 to 20, t22 | **`LOOP COMPLETE` from t11 (seq 13) and no starter ever existed — this is §2 HIGH-1's game.** Tribute spent t5 on the opponent's only creature (correct: a NAMED non-defender victim). Bond t9, Blood t11. From t11 to t21 the opponent controlled **ZERO creatures** (Rorix Bladewing lands t20), so every Tribute was gone from hand and no block could ever happen; both Idyllic Tutors were dead-tagged (`every enchantment left in your library is a copy of one you already control or hold`) and were correctly never cast (19/19 dead-Tutor declines corpus-wide at this seat). No Staff, no Sorin, no Vampire. The seat cast a wall or held every turn — the guide's `CAST SOMETHING EVERY TURN` clause — and died to Rorix (6, unblockable by sixteen Defenders) plus Hammer of Bogardan + Pyrite Spellbomb at t22. **No better play existed after t11.** The one rule miss is **seq 23** (t17 main 1, `Cast nothing right now` while a live Overgrown Battlement row was printed and both names were out); it was recovered in main 2 the same turn (seq 29 cast it), costing a window and nothing else. |
| 3 | **125** | **WON** 40 to 0, t26 | **The corpus's longest correct grind, and the Staff line at its best.** Staff of Nin cast t12 (seq 18) and **pinged the opponent on every one of its 8 upkeeps** (guide: `the one upkeep option you always TAKE`). Blood t14 (seq 26). Bond cast t16 — **COUNTERED by Fall of the Gavel**; the seat re-tutored it at t22 (seq 63/64, the LIVE Tutor form) and re-cast it t24 (seq 74), which is the guide's `A COUNTERED or EXILED copy is not on your line, so cast or tutor the next copy`, executed without hesitation. Loop closed t24 *after* that turn's ping had already fired, and the opponent held 0 creatures all game (47 dead Tribute rows, 0 taken), so the first window a starter existed was **t26 upkeep — seq 99, `Deal 1 damage with Staff of Nin targeting the opponent`**, 19 to 0 in one window. |
| 4 | **162** | **WON** 23 to 0, t13 | **Rung 0 and entry 1 in the same turn, from 8 life.** Sorin t7 (seq 12), `+1` every window it was offered (seqs 13, 21, 45, 61 — 4/4), Blood t9 (seq 20). At **t13 upkeep seq 58**, life 8, their only creature a 5/5 flier: the Tribute row's cover clause reads `5 - 5 = 0 is the MOST that can be left -> you could still be as low as 8, which you SURVIVE` and the seat cast it — the guide's `Their only creature is one your board cannot block ([flying] or similar): Tribute takes it at any life total, in the first window offered`. Then land (seq 59), **Sanguine Bond (seq 60) closing the pair**, Sorin `+1` (seq 61), and `ATTACK: A1, A2, A3` with three lifelink Vampires (seq 62) — 17 to 0. |
| 5 | **152** | **LOST** −3 to 20, t13 | **Not decidable, and the guide's two hard rules fired exactly.** Two Tutors (t8 -> Sanguine Bond, t10 -> Exquisite Blood) put BOTH halves in hand by t10, and neither was ever castable again: the opponent's curve took the seat from 32 to 15 to dead in two turns and the halves cost 5 each. At **seq 23** (t12, life 15) the prompt prints `CRACK-BACK NEXT TURN ... 20 ... that would KILL you`; the menu offers Wall of Omens (adds a blocker) or Idyllic Tutor (`casting this taps you out`). **RUNG 0 fired and was obeyed** — Wall of Omens — and it was right: the Tutor finds nothing castable and leaves 0 sources through a lethal attack. At **seq 25** the blockers header reads `this KILLS you ... no block saves you`; the guide's lethal rule (`give each blocker the BIGGEST "deals N" it may legally block`) picks **A2 Briarbridge Tracker (6)** over A4/A5 (4) and A1 (3), and the seat wrote `BLOCKS: B1:A2`. Exactly right, and it still lost. |
| 6 | **146** | **LOST** −9 to 9, t19 | **Not decidable; Exquisite Blood was never seen in 19 turns and no Idyllic Tutor was ever offered.** Sanguine Bond t12 (seq 15), Sorin t14 (seq 19) with `+1` taken at seqs 20 and 23, Staff of Nin t18 and pinged the same window (seq 27). Blocks: **seq 14** (3 blockers, 3 attackers, +7 life) and **seq 17** and **seq 24** all follow STEP 1/STEP 2 — at seq 24 Pride Guardian carried `{blocking trigger, this combat: you gain 3}` on every B-line, so it blocks, and among the two attackers it survives (A2/A3, both `deals 2`) it took one. The opponent's board reached **8 attackers, 21 unblocked, 2 of them menace**, against 1 blocker. The **one guide miss** is **seq 12** (§3). |

**Where the guide earned the three wins.** All three closed in the FIRST window a starter existed
(123 t11 Tribute, 125 t26 Staff ping, 162 t13 Tribute -> Bond -> Vampires), 0 windows wasted after
`LOOP COMPLETE` printed. Entry 1's `BOTH ROWS PRINTED: TAKE SANGUINE BOND` fired at `126v123` seq 12;
the counter/exile clause fired at `126v125` seq 63; rung 0 fired at `126v152` seq 23 and
`126v162` seq 58; the `this KILLS you` blocking rule fired at `126v152` seq 25.

## 2. Engine / interface / card items

**HIGH-1 — DH F6 LANDED, AND THE STARTER IT NAMES IS WRONG IN ALL 18 LINES: THE VERDICT NAMES A
BLOCK-TRIGGER BODY (OR A LOOP HALF) THAT CANNOT FIRE, AND THE SEAT PUT IT IN ITS PLAN FOR NINE
WINDOWS IN THE GAME IT LOST.** The wave-79 HIGH-1 shape (`a proven win` on a starter-less board) is
**not fixed; it is now specific and falsifiable.** Every rendered line names a starter, and the named
object exists on the battlefield — but nothing checks whether it can *fire*.

- **`1789216933-ai_baka_deck126-...-vs-ai_baka_deck130.jsonl` seqs 13, 15, 21, 22, 23, 29, 32, 38, 39**
  (t11-t21, life 17 falling to 11), every one verbatim:
  `[own loop verdict: your life loop is a proven win, and no link of it is on the stack right now -`
  `nothing of it is waiting to resolve, so a hold here covers no link of it. It starts from Perimeter`
  `Captain: one point of life THEY lose or YOU gain closes it]`
  Perimeter Captain's text, printed in the same prompt, is
  `Whenever a creature you control with defender blocks, you may gain 2 life.` **The opponent
  controlled ZERO creatures at seqs 13-38** — no attack, no block, no trigger, ever. At **seq 39**
  (t21) they control exactly one, and the **same prompt two lines above the verdict** says
  `6 from 1 attacker nothing you control can legally block`. The verdict and the crack-back line
  contradict each other inside one prompt.
  **The seat believed it.** Its own PLAN lines at seqs 23, 29, 32, 38 read
  `block with defenders to trigger Perimeter Captain/Pride Guardian and win via the loop` — the
  render's false starter, echoed back as the plan, for nine windows, in the game it lost at t22.
- **`...-vs-deck125.jsonl` seqs 75, 81, 82, 84, 86, 96, 99** (t24-t26): same `It starts from Perimeter
  Captain` while the opponent controls **0 creatures** and the seat controls a **Staff of Nin** whose
  ping is the actual starter — the same menu's decline row even prices it
  (`{the clock you already control: your Staff of Nin deals 1 damage a turn}`), and the seat won at
  seq 99 with exactly that row. The verdict named the one source that could not fire and ignored the
  one that could.
- **`...-vs-deck162.jsonl` seq 61** (t13, priority): `It starts from Pride Guardian` while the
  opponent controls **0 creatures** and the seat controls **three lifelink Vampires able to attack**
  plus a live Sorin `+1` row. The seat ignored it, took `+1`, attacked, and won.
- **`...-vs-deck123.jsonl` seq 16** (t11): `It starts from Exquisite Blood`. Exquisite Blood is a
  **half of the loop**, not a starter — `Whenever an opponent loses life, you gain that much` starts
  nothing by itself, which the guide states and the `LOOP COMPLETE:` header three lines above
  restates (`Your chain starts only from life THEY lose or life YOU gain`). The seat ignored it and
  cast the Tribute row that says `THIS WINS THE GAME`.

**So: 18 of 18 rendered lines name a source that could not start the loop in that window, and in 8 of
them a source that COULD was live on the seat's own board or menu.** The one behaviour that changed
between waves is that the seat now has a name to trust, and the game where it trusted it is the game
it lost. **Falsification**: any `[own loop verdict:` line whose named starter is a blocking trigger
while the opponent's attackable creature count is 0, or whose named starter is Sanguine Bond or
Exquisite Blood. **Fix shape**: rank the starter search the way THE WIN BUTTON does — an activated
row on the menu (Staff ping, Sorin `+1`), then a live Tribute row at 1+ creatures, then a lifelink
attacker, and only then a blocking trigger, and **exclude a blocking trigger whenever the opponent
controls no creature that can attack, or none this seat can legally block** (the engine already
computes exactly that sentence for the crack-back line). The gap branch DH F6 also added
(`no source of yours can start it`) **never printed once in 18 lines** — it is UNTESTED, and
`126v130` is precisely the board it was written for.

**HIGH-2 — WAVE-79's SEVEN PHANTOM `none` WRITES ARE GONE; TWO PHANTOM INCREMENTS REMAIN AND CANNOT
BE LOCATED.** 18 records carry `own_loop_verdict`, **all 18 render the line, and no record carries the
field without the line** — wave-79 HIGH-2's land-drop `none` stamping is **FIXED** (method: for every
record, compare the presence of the field against `'[own loop verdict:' in prompt`; 0 mismatches; no
prompt renders the line twice). But `own_loop_verdict_lines_rendered` reads **2 in `126v123` and 10 in
`126v130`** against **1 and 9** records, so **20 counted, 18 rendered**. Both surplus increments are in
games the field cannot explain: `126v123` has 0 ask-replays and a gapless seq run 0-17, and if the
counter incremented on replays `126v130` would be +15, not +1. Wave-79's ratio was 63:56; this is
20:18, so the identity is close but still open. **Falsification**: sum `'[own loop verdict:'` over all
prompts in a seat file and compare to the gameend counter.

**MED-3 — A MENU WHOSE ONLY CAST ROWS ARE A DEAD TUTOR AND A `[second copy:` HALF DOES NOT PRINT
`NO LIVE CAST ROW ON THIS MENU`.** Repro `...-vs-deck130.jsonl` **seqs 15, 21, 22, 32, 38**. Seq 32's
whole menu is: `1. Cast Idyllic Tutor {2}{w} [finds only an enchantment card - every enchantment left
in your library is a copy of one you already control or hold: Sanguine Bond]`, `2. Hold priority`,
`3. Cast nothing right now`. Seq 22 and 38 add `2. Cast Exquisite Blood {4}{b} [second copy: you
already control Exquisite Blood ... a second instance of an effect you already have]`. **No
`NO LIVE CAST ROW` marker prints on any of the five**, while the same marker prints 25 times
elsewhere at this seat on menus that are dead in exactly the same way. The deck guide's own
three-dead-rows list (`a Tribute reading "at 0 this does nothing"`; `a row tagged "[second copy:"`;
`a Tutor tagged "[finds only ... a copy of one you already control or hold"`) names two of the three
shapes the engine is not counting. The model had to classify these by itself and got 3 of 5 right
(holds at 21, 22, 38; `Cast nothing right now` at 15 and 32). **Falsification**: any menu whose every
cast row carries one of those three tags and whose prompt lacks the marker.

**MED-4 — THE CACHED-ANSWER REPLAY RUNS TO 39 IN A ROW ON ONE KEY IN THIS BINARY; DI's RE-ASK IS NOT
IN IT.** `askreplay/` holds 89 records for this seat, each with a `replayed_from` that resolves to a
real record seq in the same file and a `turn` matching that record (**DH F12: PASS, 89/89**), and
`ask_replays_cache` reconciles game by game (15/52/17/5). The distribution is the concern:
`126v125` **seq 96 replayed 39 times** in one upkeep, `126v162` **seq 33 replayed 17 times** at t11,
and three runs of 5 in `126v130`. Every replayed answer here is a decline (`Cast nothing right now`
or a hold), so nothing went wrong — but this is the same shape as the hung `162v123` attempt 0 (one
cached answer replayed 3,674×), and the seat that stopped the 39-run was the seat WINNING the game in
that window, not a bound. **`cached_replay_reasked` is 0 corpus-wide because lane DI ships only in the
rerun binary; on my six games the state that DI guards AROSE, four times.** Recommend the engine seat
adjudicate DI against this population, not only against Doomsayer+Alarm.

**MED-5 — 3 OF 178 PROMPTS EXCEED 20 KB, ALL IN `126v146`, AND THE BIGGEST IS A PRIORITY MENU.**
Repro `...-vs-deck146.jsonl` seq 27 (22,315 B), seq 24 (21,735 B, the 8-attacker blockers menu),
seq 26 (20,950 B). Wave 79 at this seat had 1 of 192 over 20 KB and a max of 20,359; the U16 fold
(−91 B/prompt) did not hold the ceiling on a wide opponent board. The driver is visible in seq 24:
fourteen opponent permanents each carrying a full `{effect (THEIR card, written from THEIR seat: ...)}`
body, including Lolth's four-ability text in a window where Lolth is neither attacking nor targetable.

**LOW-6 — `hold_verdict_safer_ignored` NOW HAS A PER-RECORD TRACE, AND THE IDENTITY CLOSES EXACTLY
(wave-79 LOW-7 answered).** 36 `hold_event` records at this seat: 19 `reopen_rows_moved`
(18 `a printed row changed or is newly available` + 1 `a printed row it was held over is gone`) and
**19 `verdict_clamped`**, against gameend `hold_verdict_safer_ignored` **19** — exact. Every clamp
reads `held [stack death verdict: you survive the stack] over live [stack death verdict: nothing
lethal on the stack]`, i.e. the held face is a strictly weaker statement of the same non-lethal fact,
so **ignoring it was harmless in all 19**. Two notes: the clamp re-fires per suppressed window rather
than per decision (`126v125` seqs 48-54 are 7 events all on `window_seq` 37, seqs 87-95 are 9 on
`window_seq` 55), so the counter measures suppressions; and `hold_reopened_new_lethal` /
`_new_threat` are both 0 at this seat with no equal-rank threat ever arising — **UNTESTED, not a
FAIL**.

**LOW-7 — THE CORPUS'S ONE `prose_outside_two_lines` DEVIATION IS THIS SEAT'S WINNING WINDOW, AND
THE PARSER READ IT CORRECTLY.** Repro `...-vs-deck162.jsonl` **seq 62**, `off_protocol_bytes` 194,
`answer_replaced` false, `plan_line_missing` false. Verbatim reply:
`The loop is complete and ready to trigger. Attacking with the Vampires will deal damage, cause me to`
`gain life, and trigger the Sanguine Bond/Exquisite Blood chain to drain the opponent to zero.` /
`PLAN: Attack with all three Vampires to trigger the Sanguine Bond/Exquisite Blood loop for victory.` /
`ATTACK: A1, A2, A3`. Prose precedes a correctly labelled PLAN which precedes the action line, so
under the owner's rule (*any plan must precede action*) the answer is unambiguous and the parser took
it — the game ended 23 to 0 in that window. This is a **different record** from the census's
`priority`/`unparsed_reply` fallback (this one is `attackers` and is not a fallback); the engine seat
should not conflate them.

**Wave-80 lane items adjudicated on this seat's six games.**
- **U10/DE + DH F6 — the own-loop verdict: the LAND-DROP LEAK IS FIXED (PASS), THE STARTER NAMING IS
  FALSE IN 18/18 (FAIL, HIGH-1), THE GAP BRANCH IS UNTESTED, the counter is 20 vs 18 (HIGH-2).**
  `own_loop_windows_asked` 1 = `126v125` seq 99, the winning priority window.
- **U2/DE + DH F11 — `hold_event`: PASS.** 36 records, every one located to a `window_seq` and a
  reason, identity with `hold_verdict_safer_ignored` exact (LOW-6).
- **U2/DE + DH F3/F4 — crack-back and stack-death verdict lines: PASS, 33 lines, all TRUE.**
  28 crack-back + 5 stack-death, all on ASKED prompts, **none on a suppressed window** (DH F10 holds
  here). Recomputed: `126v123` seq 16 `you survive` (life 20, crack-back 3); `126v162` seq 58 (8, 5 ->
  3); `126v152` seq 23 `LETHAL if it is UNBLOCKED` (15 vs 20, and the seat died at t13);
  `126v146` seqs 26/27 `LETHAL EVEN THROUGH YOUR BEST BLOCK ... lets 17 through` (12 life, dead at
  t19 at −9). `126v130` seq 39 prints `you survive` at life 11 against a 6-power crack-back and the
  seat died at t22 — **the line is still TRUE as scoped** (`the crack-back alone does not end the
  game`); it was Hammer of Bogardan + Pyrite Spellbomb, not combat, that took the 5 extra.
  `crackback_lethal_blocked_away` **0 — UNTESTED at this seat.**
- **U3/DF + DH F7/F8 — `{crack-back cover:}`: PASS, 12 clauses, all arithmetically correct, and the
  trample carve-out is present on 8 of 12** (every clause in `126v152`, where Sigarda has trample):
  `BUT AT LEAST ONE OF THESE ATTACKERS HAS TRAMPLE, and a trampler is the exception: it assigns only
  enough damage to kill its blockers ... The figures below subtract each blocked attacker's FULL
  power, which is right for the non-tramplers and too generous for the tramplers, so they are an
  upper bound`. **No body was credited that the row does not leave**, and `crack-back cover: NONE
  from this body` never printed (0 sites). The Tribute-row cover form is correct on all 4 of its
  sites (worst-case reasoning, e.g. `126v162` seq 58: `5 - 5 = 0 is the MOST that can be left -> you
  could still be as low as 8, which you SURVIVE`).
- **U1/DG + DH F5 — the PLAN echo's cast step: PASS, 58 closures, 0 leaks.** Sampled all 81 echoes
  that follow a cast answer: the cast clause survives into the next window and disappears at the CAST
  EVENT (`126v123` seq 13 still echoes `Cast Sanguine Bond`, seq 14 no longer does). No decline on a
  completing menu occurred, so the **stranded-mana print is UNTESTED at this seat**.
- **U12/DG — the Sorin gloss: PASS in the form it can be checked here.** 6 sites, all reading
  `{text: +1: Put a 1/1 black Vampire creature token with lifelink onto the battlefield. (...more -
  this card also has -2:, -6:; their full text is in the decklist in your instructions)}`. Wave-79
  MED-5's silent drop is **gone — the marker now NAMES the two missing abilities and routes to the
  decklist**. I could not verify the decklist body itself: **the translog stores only `system_hash` /
  `system_bytes` (47,516), not the system prompt text.**
- **U13/DF — `menu_single_outcome`: 0 records, `single_outcome_rows_spared` 0 — UNTESTED at this
  seat.** U5/DG (the neutral exemplar) — the ask seam's exemplar is
  `"CHOICE: <row number> (<that row's short name>)"` with the explicit
  `a worked example of the FORMAT only - the number and the name in it are placeholders`; **row-1
  pick rate at this seat 60 of 139 ask windows (43%)**, and the row-1 picks are overwhelmingly the only
  live row. U6 (cycling), U7 (`board sweep:`), U11 (own-spell markers / stack pointer), U14
  (compulsory draw fold), U8 (`crossphase_replayed`), U4 (tap bill — present and single on every
  priced row I read), `stop_reached_*`, `{X pricing}`, `HELD:`, menace-pair drops past 12×12:
  **UNTESTED here** — the state never arose (0 counterspells of mine, 0 X spells, 0 cycling rows,
  0 sweepers, 0 attackers decisions except `126v162` seqs 22/54/62, 0 stop chains).
- **U17 / PER-SEAM COST — at this seat latency is not stored per record** (`latency_ms` exists on ask
  records: p50 ≈ 46-90 s, `deadline_pct` max well under 50 on everything I read, **0 deadline
  pressure, 0 `reply_truncated`**).
- **DI / DJ — UNTESTED at this seat**: all six of my games are on the 20-game leg. See MED-4 for the
  population DI should be adjudicated against.

## 3. Guide verdict: **KEEP as is** (no `strategy.txt` written)

Nothing this wave is paid for by a decision the corpus shows going wrong. The three losses are a deck
that drew neither half of the pair in 19 turns under an 8-attacker curve (146), a deck that had both
halves in hand by t10 and was dead before either could be cast (152), and a deck whose pair closed at
t11 onto a board where no starter could ever exist (130). The three wins each pressed the first
starter the menu ever offered. The rules that carry this guide held at or above their best rates:
**land drop 35/35**, **ZERO STOP 47/47**, **dead-Tutor 19/19**, **Staff-at-the-opponent 8/8**,
**Sorin `+1` 4/4**, rule 1's `BOTH ROWS PRINTED: TAKE SANGUINE BOND` (`126v123` seq 12), rung 0
(`126v152` seq 23, `126v162` seq 58), the `this KILLS you` blocking rule (`126v152` seq 25), the
countered-copy clause (`126v125` seq 63), the no-starter `CAST SOMETHING EVERY TURN` clause
(`126v130`, ten turns of it), and the dead-menu hold row 20/25.

**REJECTED edits, with the evidence that rejects them.**
1. **A rule against a second Chromatic Lantern** (for `126v146` **seq 12**, t10, life 19: the menu
   offered `1. Cast Pride Guardian {w}` and `2. Cast Chromatic Lantern {3} [second copy: you already
   control Chromatic Lantern ... the effect it gives your OTHER permanents is one it already gives
   them]`, and the seat took row 2). **The guide already answers this twice** — entry 4 (any wall)
   sits above entry 7 (Lantern), and entry 7's own carve-out is scoped to `a half or Tutor on your
   HAND line whose coloured pips your untapped SOURCES cannot pay`, which was not the shape. One take
   in seven `[second copy:` exposures, in a game decided by an aggro curve, and the seat cast the
   Pride Guardian in the very next window anyway (seq 13). A printed ordering read and not applied is
   not answered by a fourth restatement — wave 78 and wave 79 reached the same conclusion on this
   deck's three absolutes.
2. **A restatement of `"Cast nothing right now" is never your answer`** (for `126v130` seq 23). The
   sentence is already absolute and the drop was recovered in the same turn's main 2 (seq 29).
3. **A hold-row rule for the 5 `Cast nothing right now` answers on `NO LIVE CAST ROW` menus**
   (`126v125` seqs 20, 55, 69, 96; `126v162` seq 7). The cost is window economy, not play, and at
   `126v125` **seq 96** the model's PLAN says `Pass the casting window to proceed to main phase and
   activate Staff of Nin to trigger the life loop and win the game` — it declined the hold row to
   protect the priority window it then won in (seq 99). The model is reading the hold row's scope
   correctly; the 470-byte row's exemption clause is what is hard to find, which is a render item.
4. **Any starter hedge for §2 HIGH-1.** The guide's `THE LOOP NEEDS A STARTER AND IT IS ALWAYS A CARD
   OF YOURS` paragraph already names the exact gap the render got wrong (`no creature on their line;
   no Staff, Sorin or Vampire of yours`) and already prescribes what `126v130` did for ten turns. The
   defect is a rendered statement contradicting the guide and contradicting the prompt's own
   crack-back line; A334 forbids writing the hard case in as a concession, and the cheaper fix is the
   verdict line.
5. **Anything about `126v125`'s fourteen idle turns or `126v130`'s eleven.** Opponent creature count
   0 in both for the whole span (47 `at 0 this does nothing` Tribute rows declined), every Tutor dead
   or already spent, and in 125 the seat pinged for 1 every upkeep it could and won the first window
   the ping mattered.

No general-guide proposal and no skill-proposal: every item in §2 is an engine/render item, and the
guide sentences they touch are already correct.

## 4. Not checked
I read **no engine source and built no fixture**; every mechanism sentence in §2 is a falsifiable
repro against the logs. I did **not** read the corpus census output (`~/.gatelogs/w80-stats.out`) —
every figure here is computed from my six seat logs, except the corpus totals quoted from the brief.
I opened **no `.stderr` file** this wave, so I contribute nothing on `dropped after its X was
announced`, on the HUNG game, on the DJ harness gate, or on the forced-close/stderr identity (6
`forced_close` records at this seat, 3 `_unrecorded`, **unreconciled**). I traced **no opponent seat
log except `126v130`'s**, and that only to establish how the seat died at t22 — opponent DECISIONS
(why deck130 held Rorix until t20, why deck125's Fall of the Gavel hit the Bond and not the Blood,
why deck123 never attacked) are unexamined. I could not adjudicate `hold_windows_skipped` (207 at
this seat), `own_turn_windows_skipped` (97), `mana_only_windows_skipped` (78) or
`ask_key_continuation_differs` (10) — **no per-record field exists for any of them**, so what those
skips suppressed is invisible. `plan_step_count` vs `plan_steps_done` is unaudited. The **system
prompt body is not in the translog** (hash + 47,516 bytes only), so the decklist gloss, the deck list
itself and the reply-protocol text beyond `reply_protocol` are unverifiable from here. **Card
verification**: I checked **Exquisite Blood, Sanguine Bond, Tribute to Hunger, Perimeter Captain,
Pride Guardian, Wall of Omens, Overgrown Battlement, Staff of Nin, Chromatic Lantern, Idyllic Tutor
and Sorin, Lord of Innistrad** against their rendered `{card text:}` / `{text:}` / `{effect:}` in the
prompts (Sorin only as far as the truncated gloss shows). **Rorix Bladewing, Hammer of Bogardan,
Pyrite Spellbomb, Molten Rain, Stone Rain, Lay Waste, Spark Spray, Dwarven Blastminer, Bloodline
Keeper, Intruder Alarm, Fall of the Gavel, Path to Exile, Sphinx's Revelation, Supreme Verdict,
Lightmine Field, Master of the Feast, Underworld Dreams, Ob Nixilis the Hate-Twisted, Sigarda
Champion of Light, Briarbridge Tracker, Brutal Cathar, Katilda, Ranger Class, Nadaar, Barrowin of
Clan Undurr, Triumphant Adventurer, Lolth Spider Queen** were **NOT verified** beyond what the render
stated.
