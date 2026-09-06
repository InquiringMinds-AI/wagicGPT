# Wave-64 engine-narration seat

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-001533/` (binary master `dab6a6cba`).
42 seat JSONLs, 21 stderr logs. All counts below are mine, measured with python over the records and
`/usr/bin/grep` over the stderr; every render count is taken from the rendered `prompt`, never from
`options`. Nothing was built, run or edited.

---

## 1. Corpus census

| | |
|---|---|
| games | **21 natural, 0 hung, 0 crashed** (42 `gameend` records, 21 `won:true` / 21 `won:false`, 0 `draw`) |
| records | 2421 total; **2295 decisions** (2294 carry a `prompt`, +1 `wall_miss`), 42 `gamestart` / 42 `system` / 42 `gameend`, 1 `recovery` |
| kinds | ask 1744, priority 379, attackers 84, discard 57, blockers 14, reveal 12, bottom 3, wall_miss 1, recovery 1 |
| fallbacks | **1**, all kinds: `wall_miss_unrecorded_wall` (130v125 seq 8) |
| stale drops | `async_drops` **18 events over 17 records** (all `casting/...(or turn/phase)|board/re-asked`); `dropped_assignments` **0** |
| transport | **1** record carries a `transport` field: 130v125 seq 8 `curl=28,http=0,empty=1,connect_ms=20000,phase=wall`, `latency_ms 900022`, `deadline_pct 100.0`, recovered at seq 9. `transport_error` retries: **0** |
| timeouts | 1 (the record above). `deadline_pct > 100`: **0**; max `deadline_pct` 100.0; max latency 900,022 ms |
| wall misses | `wall_miss_events` **1**, `wall_miss_unrecorded` **1** |
| HOLD | **1556 windows render a `Hold priority` row** (prefix-matched on the numbered rows); **248 taken** (prefix-matched on `chosen_text`) = **15.9%**. `hold_windows_skipped` 2167, `identical_ask_answers_reserved` 248, `identical_option_asks_resolved` 4, `mana_only_windows_skipped` 808 |
| SEGV / abort / hang | **0** — `Segmentation`, `Aborted`, `Assertion`, `terminate called` all 0 across the 21 stderr files (2.5 MB); no translog gap past a watchdog |
| wave-58/59 signatures | `empty_reply` **0**, `transport_error` **0**, `bad_reply` **0**, `deadline_pct>100` **0**, `reveal_stall_forced` **0**, `reveal_stall` **0**, `unparsed_reply` **0** |
| new wave-64 records | `menu_pass_forced` **0**; `reveal_wait_driver_ticks/_secs` **12** (one per `reveal`); `latched_line_offset` + `plan_block_start/end` **51** (one per `latched_line`); `chosen_text_core` **1041**; `{crack-back cover:` **3 tags / 2 windows**; W# planeswalker rows **9 windows**, `A#>W#` replies **2** (`parse_note: attack_walker_target`) |
| other parse notes | `decision_reversed_in_prose;plan_choice_conflict_narrowed` 4, `echo_index_conflict` 2, `repeat_count_zero_pass` 2, `attack_last_line_taken` 1, `hold_row_named` 1, `plan_contradicts_noop_row` 1. `answer_replaced` 26, `commit_retracted` 0, `post_answer_overrun>0` 1626, `post_plan_overrun>0` 63 |

---

## 2. Prediction adjudication

Format: verdict — evidence (N of M, file+seq). UNTESTED means no window arose; I never inferred one.

### Lane AG

**1. F6 (modal rows priced) — FAIL.** 3 real `CHOOSE_MODE` windows arose, all on Silverquill Command:
146v130 seq 57 (9 rows), seq 74 (5 rows), 146v125 seq 33 (9 rows) — **23 mode rows, 0 carrying
`{this mode right now:`** (corpus-wide occurrence count of that string: 0). Two of them are exactly the
priced shape the lane names: `5. return creature and you draw` = `draw:1 controller && life:-1 controller`
(borderline.txt:102744) and `8./4. you draw and sacrifice creature` = `draw:1 controller && life:-1
controller && ability$!...!$ opponent` (:102747). The seat took `you draw and sacrifice creature` at
**5 life** (146v130 seq 57 -> 4 at seq 58) and again at **3 life** (seq 74 -> 2 at seq 75) and lost that
game at 0. See HIGH-1 for the mechanism I could locate.
The other two clauses are vacuously true: 0 rows carry `THIS KILLS YOU` at all, for either player.

**2. F7 (gang-block verdict derived) — PASS on the first-strike half, UNTESTED on the deathtouch half.**
29 `GANG BLOCK:` strings; 22 are the two protocol boilerplates, **7 distinct live verdicts** (wave 63: 0).
**4 of them carry `past everything its own first strike can kill`** — 146v152 seq 10 is the wave-63
repro shape restored: `A1. Triumphant Adventurer (1/1) [first strike, deathtouch] ... GANG BLOCK: any 2
of them together deal 1 past everything its own first strike can kill` over two candidates that each read
`(you kill it, your attacker lives)` 1-on-1. Also 146v152 seq 21, 152v146 seq 13 and seq 27. I checked the
arithmetic on all 4 by hand and none over-claims. `together KILL this attacker` (the deathtouch-decided
wording, no damage total): **0 renders — UNTESTED.**
Falsifier sweep: over the 14 `blockers` windows I parsed every A-line's toughness against the top-2 powers
of the B-lines legally able to block it, excluding attackers a lone blocker already kills — **0 suspects**.

**3. F8a (material claim truthful) — PASS, 6 of 6.** Every `chosen for your blockers' material as well as
for the life` clause (162v123 seq 10/14/18, 146v130 seq 31/32, 126v152 seq 9) names only pairings reading
`(neither dies)` or `(you kill it, your blocker lives)` in the same window's B rows, and each closes
`every blocker in it survives`. 0 material claims over a chump or a trade. 1 `chosen for the LIFE ONLY`
clause (123v152 seq 10) and it carries `- taking it SPENDS Bloodline Keeper`.

**4. F8b (block-trigger gain folded) — UNTESTED.** `before your blocking triggers` : **0 renders**;
no window in the corpus paired a `BLOCKING THIS COMBAT` line with a certain gain. `no block saves you`
appears once (123v152 seq 10) with an after-figure of 0, so the badge is not falsified either.

**5. F8c (OPTION on non-lethal headers) — PASS, 11 of 11.** Every `one legal assignment that reaches it`
clause carries `This assignment is an OPTION, not an instruction`; 0 lethal-header clauses carry it (the
one lethal header, 123v152 seq 10, uses `the assignment that lets in the LEAST damage is:` instead).
Secondary (behaviour): over the 9 `blockers` windows carrying the clause the named assignment was copied
**6 of 9** — three declines, all by the 152 seat against a 1/1 first-strike deathtoucher, which is the
right call. Wave 63 was 3 of 3; the rate fell as predicted.

**6. F9 (survival first, death verdict scoped) — PASS by its falsifier, UNTESTED on both new branches.**
146 `INCOMING THIS COMBAT` lines; **9 carry `this KILLS you`**, and every one is genuinely the
no-assignment-survives branch (best case 0 or worse, or no bodies): **0 unscoped death verdicts beside a
positive best case**. But `you SURVIVE at N if you block`: **0**; `no assignment is PROVEN to save you`:
**0**; `ONLY if you decline every block`: **0**. Neither new branch rendered once.

### Lane AH

**1. F1 (best X clamped to the library) — PASS by its falsifier, emitter UNTESTED.** 299 `best X for this
cast` badges, 278 of them on a drawing spell. For each I read `Your library: N cards` off the same prompt:
**0 badges where `X * drawPerX > library - 1`**; the tightest margin in the corpus was 9 spare cards
(lib 28 / X 19). No deck-out occurred in 21 games (`draw` 0 on all 42 `gameend`s; every loser was at <= 0
life). `LIBRARY CEILING:` therefore rendered **0 times — the clause itself is UNTESTED.**

**2. F2 (pass-row fact derived) — PASS, decisively.** `(this ask has no pass row)` appears on **191
windows**, all `ask`. On each I extracted the numbered rows and prefix-matched them against
`Cast nothing right now` / `Play no land right now` / `Hold priority` / `Decline -` /
`Done - no further targets`: **0 of 191 carry a decline row** (wave 63: 680 of 843). The other 1553 `ask`
windows carry `(the LAST row of this menu declines: ...)`. 191 + 1553 = 1744 = every `ask` record. The
middle form `(row N of this menu declines:` fired 0 times.

**3. F2 second order (hold-take rate) — PASS.** Over windows tagged `you declined this exact list N times
already this turn`: deck125 took the hold **36 of 144 = 25.0%** (wave 63: 18 of 390 = 4.6%). Corpus-wide
59 of 279 = 21.1%.

**4. F5 (stack-trap verb and decline sentence) — UNTESTED.** 62 `TARGET CHOICE` windows arose and the
stack-trap NOTE fired on **none** of them: `NOTE: these targets are battlefield permanents only` 0,
`There is NO decline row on this list` 0. The old lie is also gone — `bouncing` appears **0 times**
corpus-wide — but that is the same silence, not a measurement of the new wording.

**5. F10 (draw step compulsory) — MIXED: render PASS, behaviour FAIL.**
Render: every `DRAW PUNISHERS on the battlefield: theirs` paragraph (159) carries `Your DRAW STEP is
COMPULSORY` (159 of 159); every `DRAW FORECAST: your ...` line (159) carries `This draw step is
COMPULSORY` (159 of 159); **0 of 32 `DRAW FORECAST (theirs)` lines carry it**; the unscoped sentence
`Count that cost before choosing to draw.` : **0**.
Behaviour: the lane also predicted *"No reply in the corpus states a plan of holding priority in order to
avoid a draw STEP."* **9 replies do** — 130v123 seq 59 and 130v126 seqs 12, 13, 14, 15, 16, 31, 43, 44,
e.g. `CHOICE: 7 (Hold priority)\nPLAN: Hold priority through main phase to avoid drawing.` **None of those
9 windows carries a punisher paragraph or a draw forecast at all** — see MED-1.
Also see HIGH-5: the new sentence is printed with the wrong actor on the `punishers: yours` paragraph.

**6. F11 (crack-back cover + cost of attacking) — PASS on what arose.** 300 windows print
`CRACK-BACK NEXT TURN`, **32 of them lethal**. `{crack-back cover:` renders on 2 windows / 3 tags
(146v152 seq 19, 123v152 seq 9), both on lethal headers, both figures <= that window's crack-back total,
**0 tags on a survivable or absent header**. Both lethal ATTACKERS windows (146v152 seq 21, 126v123 seq 29)
carry `CRACK-BACK COST OF ATTACKING`: **2 of 2**. Correctness of the one SURVIVE verdict: see HIGH-4.

### Lane AI

**1. F3 (front face no longer ends main 1) — PASS.** 21 front-face takes (`chosen_text` contains
`(menu text: Play Land)`). **18 of 21 are followed by a same-turn Main-phase-1 window** (wave 63: 0 of 15).
The 3 exceptions are all turn-1 takes (152v123 seq 4, 152v162 seq 4, 152v126 seq 4) whose menu offered no
castable row — not the falsifier, which requires a castable spell on that turn's cast menu. Back-face
control: 5 of 6. The two rates are now indistinguishable, which is the prediction.

**2. F4 (planeswalker rows + `A#>W#`) — render PASS, apply FAIL.**
Render: **9 of 9** attackers windows whose opponent battlefield lists a `[planeswalker]` carry indented
`W#.` rows and the `A#>W#` grammar sentence; **0** windows without one carry either. `[battle]` rows: 0
(UNTESTED). Two replies used the grammar (123v126 seq 25 `ATTACK: A1>W1`, 125v126 seq 259
`ATTACK: A1>W1`), both parsed, both echoed in `chosen_text` as `<attacker> -> <walker>` and both carry
`parse_note: attack_walker_target`.
Apply: the prediction's own falsifier is "a `>W#` reply whose defender's life dropped". Neither did — but
**neither walker lost any loyalty either.** See HIGH-2.

**3. F12 (granted-ability actor) — FAIL.** 10 Tribute-to-Hunger `used:` lines over 5 events / 3 games
(plus 2 unrelated Elite Spellbinder lines that name the card as an exile target). **Every one still credits
the SACRIFICING player, not the caster.** The lane's own falsifier fires verbatim at 126v130 seq 16:
`- You cast Tribute to Hunger` / `- Your Tribute to Hunger resolved` / `- Opponent used: Gain life equal to
its toughness with Tribute to Hunger targeting Goblin #1` / `- You gained 1 life (now 21)`. Repeats at
126v130 seq 20 and 24 and at 126v125 seq 75; the mirrors (130v126 seq 31/47/67, 125v126 seq 197) read
`You used: ... Opponent gained 1 life`. See HIGH-3.

**4. F14 (reveal wait is the model) — PASS, 12 of 12.** Every `reveal` record's `reveal_wait_secs` is
within ~1 s of `latency_ms/1000` (10/10130, 15/15433, 19/19682, 25/25763, 38/37022, 116/116083, 48/47515,
44/44483, 29/29015, 54/53996, 35/35423, 36/36008) and **`reveal_wait_driver_secs` is 0 on all 12**, ticks 0.
0 `reveal_stall`, 0 `reveal_stall_forced`. Longest wait this wave 116 s (wave 63: 561 s). The new field
does exactly what it was added to do; no genuine engine stall arose to test the other direction.

### Lane AJ

**1. F13(a) (byte offsets) — PASS, 51 of 51.** Every record carrying `latched_line` carries
`latched_line_offset`, `plan_block_start` and `plan_block_end`. **0 records read
`latched_line_in_plan: true`**, so the "offset inside the block" clause is vacuous. I ran the slice check
on all **25 records with `reply_trimmed_bytes` absent or 0**: `reply[off:off+len(line)] == latched_line`
on every one. The other 26 are trimmed and, as the lane says, unsliceable.

**2. F13(c) (column-0 bound) — PASS on the hard clause, FAIL on the weaker outcome clause.**
`plan_answer_line_ignored` and `plan_answer_line_only` account for every `latched_line_in_plan: true`
record (there are none); `unparsed_reply` is **0**, below wave 63's 1. But the lane's weaker prediction —
*"`plan_answer_line_ignored` fires at least once"* — is **0 again**. Its stated reading ("the corpus is
telling us the indented-in-plan shape does not occur") no longer holds: see the second corpus question below.

**3. E9 (`chosen_text_core`) — MIXED.** `{removes ` in `chosen_text_core`: **0**; in the GAME LOG
(`events`): **0**; still present in `chosen_text` on 9 records, which is the design (the row is kept whole).
The literal clause *"0 `chosen_text_core` values contain `{` or `[`"* is **false: 383 of 1041 do** — but
every brace I sampled is a mana cost (`{1}{r}{r}`) or a quoted card-text fragment, not narration
decoration. Substance holds, letter does not; see MED-2.

**4. E14a (bracketed self-correction opener) — PASS.** 2 served `YOUR PLAN` blocks contain a bracketed
`(wait,` (123v125 seq 139 `...from Turn 34 (wait, it was countered).`; 126v125 seq 46 `Then cast Chromatic
Lantern (wait, it's already on battlefield?`), and **both are mid-sentence parentheticals, not
sentence-start markers**, so neither should cut and neither did. False-positive direction clean:
`(Wait until` appears 0 times.

**5. Hold / transient bracket — PASS.** 20 windows carry `[this cannot target the spell on the stack -
battlefield permanents only]`. I compared every same-turn same-kind window pair per file with that clause
stripped: **0 pairs whose rows are identical only after stripping it** (wave 63: the 125 seqs 72-74
sequence). `hold_windows_skipped` 2167, not fallen.

**6. E4b (multi-priced gang tag) — PASS.** The certain+ceiling shape renders 3 times
(`(1 certain if all block, and up to 37 if they also take every "may")`), second figure never smaller than
the first; the may-only shape (`(up to 42 if all block, and only if they take every "may")`) is a single
figure and unchanged. `plus up to N more`: **0**.

**7. Blocker-count scope — PASS, 5 of 5.** Of the windows with an `N > 0` "untapped creatures able to
block" header AND at least one `[no creature they control can block this attacker]` A-line, **all 5** carry
the scope clause (`but K of your M possible attackers cannot be blocked by any of them, so that count
applies to the other J only`): 152v123 seq 25, 123v126 seq 25 and seq 48, +2. **0 unscoped.** (59 further
windows have the header at N=0, which the clause does not govern.)

**8. Enters TAPPED — PASS, 110 of 110.** 96 `[enters TAPPED ...]` + 14 `[enters tapped UNLESS ...]`
brackets, every one ending `- it taps for mana from your next turn on`; **0 of the 68 `[enters UNTAPPED ...]`
brackets carry it.** Outcome half (deck125's declined-land-drop rate) is a per-deck question I did not compute.

### Lane AK

**Prediction 1 (R1, walker-only attackers admitted) — UNTESTED.** `[may NOT attack them:` : **0 renders**; no
`CANTATTACK`-without-`CANTPWATTACK` creature arose. None of the four `flyersonly` cards is in any of the
seven pool decks (checked `bin/Res/ai/baka/deck{123,125,126,130,146,152,162}.txt`), so this window cannot
occur in this pool at all.
**Prediction 2 (R2, `menu_pass_forced` gated on legality) — PASS, trivially.** 0 records; the floor never fired, so its
conditional half is UNTESTED.
**Prediction 3 (R4/R7, crack-back cover honesty) — PASS on the render, but see HIGH-4.** 2 of 2 tags print the
CHECKED/UNCHECKED split; 146v152 seq 19 correctly declines the verdict (`Counting only the checked bodies
you cover 4 of 15 ... which still KILLS you`) after excluding 1 of 5 attackers as unblockable; 123v152
seq 9 prints an earned `which you SURVIVE - and more blockers can only lower that`.
`THIS IS NOT A SURVIVAL VERDICT`: 0 renders (no unchecked-body window arose).
**Prediction 4 (R5, `plan_answer_line_ignored` 0 at any indentation) — PASS, 0.** Also 0 `answer_replaced` records
latching a line written after the one taken (26 `answer_replaced`, all with an in-block offset).
**Prediction 5 (R6, no `ability$!` magnitude priced) — UNTESTED**, vacuously: 0 mode tags rendered (F6 FAIL).
**Prediction 6 (R8, first-strike gang budget exact) — PASS on 4 of 4.** The four first-strike verdicts are exact:
146v152 seq 10 (1/1 FS+DT vs 1/1 + 3/1 -> "deal 1 past"), seq 21 (vs 3/3 + 2/2 -> "deal 2 past"),
152v146 seq 13 and 27. No verdict claims a kill the attacker's first-strike budget would prevent.
**Prediction 7 (R9, upkeep reserve) — UNTESTED.** 0 `LIBRARY CEILING:` renders anywhere, so no upkeep-window ceiling
existed to be one too low.

---

## 3. The two lane-flagged corpus questions

**(a) `flyersonly` vs modern CR.** The four `flyersonly` cards model **two different Oracle texts** with
one keyword, and that is where the real bug is:

| card | Wagic (`mtg.txt`) | real Oracle (Scryfall) |
|---|---|---|
| Form of the Dragon :42987 | `lord(creature\|opponentBattlefield) flyersonly` | "Creatures without flying can't attack **you**." |
| Teferi's Moat | `lord(creature[chosencolor]\|opponentBattlefield) flyersonly` | "Creatures of the chosen color without flying can't attack **you**." |
| **Moat** | `lord(creature) flyersonly` | "Creatures without flying **can't attack**." |
| **Magus of the Moat** | `lord(creature) flyersonly` | "Creatures without flying **can't attack**." |

So the answer splits. For Form of the Dragon and Teferi's Moat, "can't attack you" restricts attacking the
PLAYER only, and under current rules a planeswalker is a separate attack destination — Wagic letting such a
creature attack a planeswalker is **correct**, and AK's R1 is right to admit it. For Moat and Magus of the
Moat the Oracle has no "you" at all, so a non-flier must not attack **anything**, and the shared
`flyersonly` keyword makes Wagic **wrong** for those two — a card-script fidelity bug, not a
decision-surface bug, and one AK's R1 widens the reach of rather than causes. It is unreachable in this
pool (none of the four is in any of the seven decks; `[may NOT attack them:` rendered 0 times), so nothing
in this corpus is evidence either way. Fixing it belongs in the card script (Moat/Magus need a keyword that
also blocks `canAttack(true)`), not in the contract.

**(b) Does AK's R5 make wave-63 E6b unreachable for line-leading coded lines?** Yes, and the corpus cannot
say otherwise. AJ's prediction 2 offered `plan_answer_line_ignored == 0` as evidence that the
indented-in-plan shape does not occur, and that reading is no longer available: after R5 put
`lineIsCodedAnswerLine` on the answer seam's own skip set (`' '`, `'\t'`, `'*'`, `'#'`), a coded answer
line at ANY indentation terminates the plan block, so no coded answer line can sit inside it and the
counter is **0 by construction**. The measured 0 (and the 0 `latched_line_in_plan: true` over 51 latch
records) is consistent with that and with an empty population, and the corpus cannot separate them. The
E6b exclusion now governs only lines the answer seam would refuse anyway (placeholders, prompt echoes).
AK says the same in its own "R5 retires E6b in practice" note; the practical consequence for the ledger is
that **retiring E6b next wave cannot be justified by another 0** — it needs the code argument, not a count.

---

## 4. Cross-deck engine / render / narration defects

### HIGH-1 — the whole `ctx`-gated mode-annotation family is silent on real mode menus (F6 did not ship into the corpus)
**Repro:** `1788671762-ai_baka_deck146-0x55b6e5c95220-vs-ai_baka_deck130.jsonl` seq 57 (turn 29, 5 life).
Header: `Choose an option for Silverquill Command:`. Rows render bare:
```
1. creature gains 3/3 and return creature
...
8. you draw and sacrifice creature
```
Also seq 74 (5 rows, 3 life) and `...deck146-0x5647a6167190-vs-ai_baka_deck125` seq 33 (9 rows).
**23 mode rows, 0 annotations of any kind.** Corpus-wide: `{this mode right now:` 0,
`{this mode has a legal object right now}` 0, `{DEAD right now: no legal object for this mode}` 0 — the
wave-60 liveness tag and the wave-60 tap/untap tag are silent on the same windows.
**Why this is one defect, not three:** at `AIPlayerGPT.cpp:33348-33394` the liveness loop is gated on
`ctx &&`, the tap/untap loop on `if (ctx)`, and F6's price loop sits inside that same `if (ctx)` block,
while the HEADER is built from `req.contextName` (`req.nameChoiceMenu` branch below). The header names the
card and the annotations do not, which is what a NULL `ctx` with a live `contextName` looks like. I did not
run the binary, so treat the location as my inference and the silence as the measurement.
**Cost:** row 8 is `draw:1 controller && life:-1 controller && ability$!...!$ opponent`
(`borderline.txt:102747`; Scryfall agrees — "Target player draws a card and loses 1 life"). The seat took it
at **5 life** (seq 57 -> 4 at seq 58) and again at **3 life** (seq 74 -> 2 at seq 75), then went 2 -> 1 -> 0
and lost 146v130 with the opponent at 4. This is the wave-63 deck146 HIGH-1 defect, unchanged, on the same
card, in the same seat, twice in one game.

### HIGH-2 — `A#>W#` is declared and echoed, and the planeswalker takes no combat damage
**Repro:** `1788671754-ai_baka_deck123-0x5648ec95a530-vs-ai_baka_deck126.jsonl` seq 25 (turn 12).
Prompt: `A1. Lord of Lineage (5/5) [flying, ...] [no creature they control can block this attacker]`,
`W1. Sorin, Lord of Innistrad [planeswalker] [4 loyalty left: combat damage removes that many counters, and
it dies at 0]`, `ATTACK TOTAL: 1 attacker listed, 5 total combat damage`. Reply `ATTACK: A1>W1`.
`chosen_text: 'Lord of Lineage -> Sorin, Lord of Innistrad'`.
seq 26 events: `- Phase: Attackers` / `- You declared attackers: Lord of Lineage -> Sorin, Lord of
Innistrad`. seq 27 events: `- Phase: Main phase 2` — **no blockers, no combat damage event of any kind.**
Sorin's board line reads `[counters: 4x loyalty]` at seq 24 (before) and at seq 32 (after); the opponent's
life is 23 at seq 25 and 23 at seq 32. **5 damage vanished.** The seat's own plan said "attack with Lord of
Lineage at Sorin to kill him"; Sorin lived and kept activating (+1 every turn thereafter).
The second instance (125v126 seq 259, turn 51, Emrakul 15/15 -> Sorin at 15 loyalty) is inconclusive on its
own — Sorin's `-6` destroyed Emrakul at instant speed before damage — but it also delivered nothing.
Note the lane's stated falsifier ("a `>W#` reply whose defender's life dropped") does not catch this shape:
the failure is silent in both directions. **2 of 2 walker attacks in the corpus dealt 0 damage.**

### HIGH-3 — F12's actor fix does not reach a `targetedplayer` grant
**Repro:** `1788671748-ai_baka_deck126-0x5598d27aeee0-vs-ai_baka_deck130.jsonl` seq 16, verbatim:
```
- You cast Tribute to Hunger (that Tribute to Hunger was 1 of 3 copies in your hand; ...)
- Your Tribute to Hunger resolved and went to your graveyard
- Opponent used: Gain life equal to its toughness with Tribute to Hunger targeting Goblin #1
- Opponent's Goblin died ...
- You gained 1 life (now 21)
```
That is the lane's named falsifier word for word. Repeats at seq 20, seq 24 and 126v125 seq 75; mirrors at
130v126 seq 31/47/67 and 125v126 seq 197. **10 lines, 5 events, 3 games — unchanged from wave 63.**
Mechanism, offered as a reading of `AIPlayerGPT.cpp:18161-18163` and not as a measurement: the fix asks
`e->source->controller()`, and `e->source` on a `targetedplayer` grant IS the granted dummy, whose
controller is the GRANTEE — the same player the event's own `controller` names. `resolveOwningCardName`
already walks a granted dummy back to its granter for the NAME (`#W42-D3`, three lines above); the
controller is not walked the same way, so the two halves of the sentence disagree about which card is
speaking. `mtg.txt:124075` is the script; `text=` and Scryfall agree the life gain is the caster's.

### HIGH-4 — a crack-back SURVIVE verdict rests on a total the same line calls a FLOOR
**Repro:** `1788671744-ai_baka_deck123-0x5641f1c6dba0-vs-ai_baka_deck152.jsonl` seq 9, turn 8, seat at 9 life.
```
CRACK-BACK NEXT TURN: 3 of their creatures will be able to attack ... for up to 13 ... you would be at -4
 or lower; that would KILL you - and that number is a FLOOR, not a ceiling: triggers on their board add
 power before damage - Luminarch Aspirant ...
```
and on the cast row, three lines below:
```
{crack-back cover: ... Counting only the checked bodies you cover 6 of 13, leaving 7 -> you would be at 2,
 which you SURVIVE - and more blockers can only lower that, so nothing uncounted here overturns it.}
```
The monotonicity argument is sound in the BLOCKER dimension and false in the ATTACK-TOTAL dimension, which
the line above it has already flagged. Next window (seq 10) the real figure was **17, not 13**:
`INCOMING THIS COMBAT: 3 attackers, 17 unblocked damage - you would be at -8; this KILLS you ... best case
with every blocker assigned: you would be at 0; no block saves you`. The seat cast Bloodline Keeper on the
promise, answered `BLOCKS: none` at seq 10 and the game ended at **-8 on turn 9**. Only 1 of the 2 cover
tags in the corpus makes a survival claim, and that one was wrong. R4 taught the tag to fail closed on
BODIES; it still fails open on the total.

### HIGH-5 — `Your DRAW STEP is COMPULSORY ... the cost above is charged whether you act or not` printed under a paragraph whose cost is charged to the OPPONENT
**Repro:** `1788671742-ai_baka_deck162-0x55b97e38e880-vs-ai_baka_deck125.jsonl` seq 14, one paragraph:
```
DRAW PUNISHERS on the battlefield: yours - Underworld Dreams. Every card the OPPONENT draws costs them 1
life to yours. ... Your DRAW STEP is COMPULSORY: no row on this or any other menu declines it, and holding
priority does not prevent it - the cost above is charged whether you act or not. Count that cost before
choosing an OPTIONAL draw ...
```
`Your DRAW STEP is COMPULSORY` renders on **191 windows; 159 sit under a `punishers: theirs` paragraph
(correct) and 32 under a `punishers: yours` paragraph (wrong actor)** — the seat is told it pays a cost
Underworld Dreams charges only its opponent, and told to price its own optional draws against it. Same
class as HIGH-3 and as codex #6: a true sentence attached to the wrong chair. The `theirDrawStepForecast`
mirror got this right (0 of 32 carry it); the punisher SUMMARY did not.

### MED-1 — the compulsory-draw fact is board-gated, and the guide rule it must answer is not
The 9 replies that still plan `Hold priority ... to avoid drawing` (130v123 seq 59; 130v126 seqs 12, 13, 14,
15, 16, 31, 43, 44) are on windows with **no `DRAW PUNISHERS` paragraph and no `DRAW FORECAST` line at all**
— the seat is quoting its own guide (`deck130_strategy.txt:11` "#0 RULE - BEFORE ANY DRAW, READ THEIR
BATTLEFIELD FOR A DRAW PUNISHER"), twice saying so out loud (`Hold priority to avoid drawing into potential
draw punishers if any exist (none visible, but rule 0 applies)`, 130v126 seq 16). F10's fact only prints
where a punisher is on the board, so on exactly the windows where the misconception fires there is nothing
to contradict it. Two repairs are available and I take neither as the owner's: state the compulsory-draw
fact on the hold row itself regardless of punishers, or fix rule #0 in the deck guide. It is a guide/render
seam, so I flag it for the synthesis rather than ruling.

### MED-2 — `chosen_text_core`'s stated invariant is not the one it satisfies
383 of 1041 `chosen_text_core` values contain `{` or `[`, e.g. 130v123 seq 9
`Cast Molten Rain {1}{r}{r} - legal targets right now: Underground Sea (land), ...` and seq 13
`Isolated Chapel - "Isolated Chapel enters tapped unless you control a Plains or Swamp. -- {T}: Add {W} or
{B}."`. These are mana costs and quoted card text, not narration decoration, so nothing is leaking — but a
harvester written to AJ's prediction 3 ("0 values contain `{` or `[`") will treat these as failures or slice
them off. The field is doing its job (`{removes ` 0, `{taking this row` 0 in the GAME LOG); the predicate
that describes it should be "contains no `{`-decoration this emitter added", which is not testable from the
record alone. Worth one sentence in the field's definition, not a code change.

### LOW-1 — codex finding #3 (connect-timeout classification) is not observable in this corpus
The single `transport` record (130v125 seq 8) reads `curl=28,http=0,empty=1,connect_ms=20000,phase=wall`
with `latency_ms 900022` and `deadline_pct 100.0`: elapsed is the full 900 s deadline, 45x the 20 s connect
timeout, so `gptDeadlineMissed` classified it correctly and the finding's trigger did not occur. 0
`transport_error` records, so the bounded-remainder retry path is likewise unexercised. AK refuted #3 on
scope; the corpus neither confirms nor refutes it.

---

## 5. What I did NOT check

- **I did not run the game, build anything, or touch `src/`, `bin/Res` or git.** Every mechanism statement
  above that names a source line (HIGH-1's `ctx` gate, HIGH-3's `e->source->controller()`) is a reading of
  the source, explicitly marked; the measurements are all from the records.
- **F7's falsifier is not exhaustively proven.** I swept the 14 `blockers` windows with a parsed
  power/toughness model and found 0 suspects; I did NOT run the same sweep over the 84 `attackers` windows'
  opposing-blocker lists, whose render shape differs. A suppressed gang verdict on an attackers window would
  not be in my count.
- **F1's clamp is proven only where no clamp was needed.** The tightest library margin in the corpus was 9
  cards; I did not construct the deck-out case and cannot say the clause is correct, only that it never
  needed to fire.
- **I did not verify the 2 crack-back cover figures against a full pairwise block model** — I checked they
  are <= the printed crack-back total and that the CHECKED/UNCHECKED prose matches the A-line tags.
- **The `{feeds:}` wave-63 shape, the `; converters` string (26 occurrences / 13 windows), the Teferi's
  Puzzle Box truncation and the Kaya `-3` row collapse are still on the ledger and I did not adjudicate
  them** — lane AJ declared them out of scope and no lane owned them this wave.
- **Per-deck decision quality is not my seat.** Beyond the four games I traced for the HIGH items I did not
  read the transcripts for misplays; the deck agents own that.
- **`hold_windows_skipped` 2167 has no wave-63 baseline in my hands**, so "does not fall" (AJ prediction 5)
  is reported as a level, not a comparison.
- **The 3 `bottom` records, the 57 `discard` records and the mulligan windows were not examined at all.**
