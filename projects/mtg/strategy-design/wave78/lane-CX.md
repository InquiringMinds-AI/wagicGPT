# Wave-78 lane CX — records and transport (S1 measure-only, S10, the census helper)

Base: master `1e465fff7` (seeded worktree `worktrees/lanes/w78-CX`, branch `w78-lane-CX`).
Gate on this branch: `make -f Makefile.sdl -j8` clean, both link guards OK
(`check-ctor-init: OK (118 file(s))`, `check-reply-instructions: OK (1 source file(s), 37 guide(s))`).
**Suite THREADS=1, detached unit `w78-CX-suite`: 1291 tests (0 failed), 76 AI tests (0 failed)** —
`==Test Failed !==` **0** and `==Test timed out` **0**, so none of the three known flakes fired and
no solo rerun was needed. **PARSETEST 6402 → 6428, 0 failed (+26 cases = exactly this lane's
additions;** base 6402/0 measured on the seeded tree before any edit). `git diff | grep -c $'\357\277\275'`
= **0**. Every wagic invocation ran under `systemd-run --user --scope -q -p MemoryMax=4G -p
MemorySwapMax=0 --` (the detached suite unit carries the same two `-p` flags); nothing was killed
under the cap. **No legal option is removed, capped or auto-answered anywhere in this lane, and the
reply parser is not changed** — `bin/Res/ai/gpt/reply-protocol.txt` and `kReplyProtocol` are untouched.

Corpus for every repro and census below:
`/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-010157-final` (42 seat logs, 21 games,
2,746 decision records).

---

## S1 (HIGH, MEASURE ONLY — the parser is NOT changed; the owner's ruling is PENDING)

**THE OWNER'S RULING IS STILL OUTSTANDING AND THIS LANE DOES NOT PRE-EMPT IT.** Whether the parser
may READ an answer line whose `CHOICE: ` label is absent is his call (invariant 000 — "no parser
reads outside the two LABELLED parts"), and nothing here reads it: the five replies are still
refused, the heuristic still answers those windows, and the shipped protocol text is byte-unchanged.
What ships is the INSTRUMENT — the record now says what the reply actually was, and a measure says
what that cost.

### Repro (`125v126` deck125 seq 140, verbatim off the record)

```
"\n\nPLAN: Let Staff pings resolve and hold priority to stop the menu loop.\n2 (Hold priority)"
```

`fallback: unparsed_reply`, `off_protocol_bytes: 17` (= the length of the answer line),
`coded_answers: 0`, `protocol_deviation: prose_outside_two_lines` — and there is **no prose anywhere
in the reply**. The `recovery` record that trails it (seq 141) reads
`executed_text: "cast Path to Exile"`: the model asked to hold, the heuristic spent a removal spell.

### RED on base (executed)

* **Corpus census (python mirror of the shape test over all 42 logs):** the shape —
  exactly one off-protocol line, matching `^\d+\s*\(.+\)$`, with no coded answer line anywhere in
  the reply — fires on **5 records and no others**, and those 5 are the corpus's **entire**
  `prose_outside_two_lines` population (`{'compliant': 2594, 'unlabelled_plan': 115,
  'plan_absent': 26, 'prose_outside_two_lines': 5}`). So the shipped stamp is false on 100% of the
  records that carry it.
* **PARSETEST, executed RED:** with `w78AnswerLabelAbsentShape` forced to `return false` (the base
  behaviour — base has no such function and no fourth argument), the run is **6416 passed, 4
  failed**, and the four are this lane's pins:
  `#W78-CX S1 GREEN the shape test names it`,
  `#W78-CX S1 RED-ON-BASE the class is answer_label_absent`,
  `#W78-CX S1 RED-ON-BASE ...stated as the negative the wave-77 engine seat raised`,
  `#W78-CX S1 RED-ON-BASE all FIVE wave-77 records take the new class`.
  Log: `~/.gatelogs/w78-CX-parsetest-S1-red.log`.

### Fix (instrument only)

* `w78AnswerLabelAbsentShape(offLines, hadLabelledAnswer)` (pure) — true only when the reply carried
  **no** coded answer line at all, there is **exactly one** off-protocol line, and that line is
  `<digits>` `(` non-empty text `)` and **ends at the `)`**. Every clause is a gate against absorbing
  a reply that really did write prose.
* `w75ProtocolDeviationClass` takes a fourth argument, defaulted `false`, and the new arm sits
  **after** the forced-close-echo arm and **before** the prose arm. The three other arms are
  byte-unchanged and every shipped caller and case reads as before.
* Gameend counter **`answer_label_absent_heuristic_played`** — of the records in that class, the ones
  the heuristic then played, counted on `handedToHeuristic(choice, fallback)`, the same predicate the
  `recovery` handoff latches on, so the measure and the recovery records cannot disagree.
* **PIN that nothing was recovered:** `answerSegmentStatic(repro, "CHOICE:")` is still **empty** and
  `gptInlineChoiceOnPlanLine` (P9's inline reader) still refuses it — the window still falls to the
  heuristic. That pin is what keeps the measure honest: the class says what happened; it does not
  take the decision back.

### GREEN

+18 cases: the repro's instrument values (17 bytes, one off line, `coded_answers 0`), the class on
the repro and on all five corpus replies, the two parser-unchanged pins, and eight MUST-NOT-MATCH —
real prose after a valid pair, a bare row number written BESIDE a real `CHOICE:` line, two off-lines,
bytes after the closing paren, an unlabelled sentence, an empty short name, a missing plan line
(still `unlabelled_plan`), and a forced-close prefill echo (still `forced_close_prefill_echo`).

### `recovered_seq` — already shipped, verified, NOT renamed

The brief asks that `recovery` records "carry `recovered_seq` naming the ask they recovered". The
field exists and is named **`recovers_seq`** (`flushRecoveryRecord`, written from
`rec["seq"]` of the record that was handed off). I **verified the join rather than renaming it**:
on all 5 recovery records in the corpus `recovers_seq` resolves to a record that exists, is
`kind: ask`, and carries `fallback: unparsed_reply` — 5/5, no off-by-one. This is NOT the S7 defect
(a window ordinal masquerading as a record seq); it is a record seq and it joins. Renaming a field
every existing reader and every prior wave note uses would break the joins the wave-77 lesson asks
to pin, so the name stands and the join is now stated as evidence. **If the owner wants the field
spelled `recovered_seq`, it is a one-line change plus a sweep of the readers — say so and it ships.**

### Prediction (falsifiable)

Next corpus: **0** records whose reply is exactly a line-leading `PLAN:` line plus a bare
`<n> (<short name>)` line carry `protocol_deviation: prose_outside_two_lines`; every one of them
reads `answer_label_absent`, and the gameend sum of `answer_label_absent_heuristic_played` equals the
number of `unparsed_reply` ask records of that shape (wave 77 would have read **5**, all one seat).
Falsifiers: a record classed `answer_label_absent` whose reply contains any prose (the class is too
wide), or a `prose_outside_two_lines` record whose reply is two lines only (the class is too narrow).

---

## S10 (MED) — the `defer` record: the ACTING seat's log, and the rows it was never shown

### Repro (`152v130` deck152 seq 21, turn 15 Main phase 1)

`kind: defer`, `fallback: menu_not_askable`, `chosen_text: "Starstorm"`, **`options: 0`,
`options_text: null`**, in the **deck152** seat's file — and Starstorm is not in deck152
(`/usr/bin/grep -il starstorm deck152.txt deck130.txt` matches deck130 only). The deck130 seat's log
shows seq 49 `CHOICE: 1 (Cast Starstorm)` and seq 50 `CHOICE: 1 (Cast Card Normally)` at the same
t15 and then no X row; it re-announces at t16. So the corpus's **only** X-announcement payment loss
is attributed to the wrong deck and its rows are unrecoverable.

### RED on base

* The record above, read straight out of the corpus: `options 0`, `options_text` absent. 1 of 1
  defers in 21 games.
* **Live, through the real defer site** (see the live probe below, run on a build with the forced
  gate): on base the same site writes `options: 0` and no `options_text` on every defer, whatever
  the menu held.

### Fix

At the `menu_not_askable` branch of `AIPlayerGPT::computeActions` (the one site that writes a
`defer`):

* `w78DeferRecordMoves(haveMenuController, controllerIsSelf, controllerIsGptSeat,
  controllerHasTransLog)` (pure) decides WHOSE log. `computeActions` runs on the seat that holds the
  action, but the ActionLayer's menu belongs to the card that armed it — an instant cast by the other
  seat arms a menu while this seat is acting, which is exactly the Starstorm shape. The record moves
  to the menu card's controller **only** when that controller is a GPT seat with a live translog;
  with no menu card, a heuristic opponent, or a logless one it stays here rather than vanishing (a
  record in the wrong file is a misattribution; a record nowhere is a silent defer, which #W41-1
  forbids).
* `w78DeferOptionTexts(rawRows)` (pure) turns the live `abilitiesMenu->mObjects` (`SimpleButton::
  getText()`, menu order) into `options_text`: trimmed at the ends, spacers dropped, interior bytes
  **verbatim** — the reviewer's question is what the seat was not asked, and an edited row cannot
  answer it. `optionCount` on the record is now that list's size, not 0.

### GREEN — live, through the real defer path

The live defer is 1 record in 2,746 and the random pool will not produce it on demand, so the path
was driven directly: a **temporary probe build** (an env gate `W78CX_FORCE_DEFER` that makes
`buildMenuChoice` return false, plus one stderr line naming self / resolved seat / menu controller /
row count) against the model-free stub endpoint (`tools/gpt-stub-server.py`, `--prefer starstorm`),
`WAGIC_AI=gpt` both seats, probe decks 198 (Starstorm ×8 + Mountain ×32) / 199 (Grizzly Bears ×12 +
Forest ×28) in the numbering gap. **The gate and the stderr line were reverted before the gate runs
and are not in the commit** (`grep -c W78CX_FORCE_DEFER src/AIPlayerGPT.cpp` = 0); the probe decks
were deleted (`bin/Res/ai/baka` highest deck is 164 again).

Four live defers, written by the real site, with the real menus:

```
seq  6 defer options 2 options_text ["0","Cancel"]                        chosen_text Starstorm  t4  Main phase 1
seq  8 defer options 2 options_text ["0","Cancel"]                        chosen_text Starstorm  t6  Upkeep
seq 13 defer options 3 options_text ["Cast Card Normally","cycling","Cancel"] chosen_text Starstorm t8 Upkeep
seq 14 defer options 3 options_text ["0","1","Cancel"]                    chosen_text Starstorm  t8  Upkeep
```

seq 14 is the **X-announcement menu** — the shape the wave-77 record lost — and it now names its
rows. The seat resolution ran on every one (`W78CXPROBE self=ai_baka_deck198
deferSeat=ai_baka_deck198 ctrl=ai_baka_deck198 rows=2|3`): the site reads the menu card's controller,
and because every menu in that game was the acting seat's own, the record correctly stayed put.

### GREEN — PARSETEST

+8 cases: the four routing states (the wave-77 cross-seat shape moves; own menu, no menu card,
heuristic opponent and logless GPT opponent all stay), the row list (order, trim, spacers dropped),
the empty-menu case, and the verbatim-interior pin.

### A root cause this lane did NOT fix, and it is the reason the defer fires at all

`DecisionManager::buildMenuChoice(p, req)` computes the X branch's ceiling from **`p`'s** mana pool
(`p->getManaPool()->getConvertedCost() - source cost`). When the menu belongs to the OTHER seat, `p`
is the wrong player, the pool is empty, `maxX < 0`, and the menu is declared unaskable — which is
exactly why `152v130` seq 21 deferred. The record is now correct about whose decision was lost; the
decision is still lost. Fixing it means building and asking the menu against the MENU'S controller
(and `AIPlayerBaka::selectMenuOption` clicks the same foreign menu), which is a decision-seam change,
not a record change. **Docket it for a seam lane.**

### Prediction (falsifiable)

Next corpus: every `defer` record carries a non-empty `options_text` whose length equals its
`options` field, and the deck named in its `chosen_text` is in the DECKLIST of the seat whose file
holds it (the wave-77 cross-file mismatch cannot recur), so a per-seat `kind` census of `defer` is no
longer off by one in both directions. Falsifiers: a `defer` with `options: 0` on a menu that had
rows (the row walk missed the menu), or a `defer` whose named card is in the other deck's list (the
routing did not fire where it should have).

---

## The census helper — `projects/mtg/tools/corpus-stats.py`

`~/.gatelogs/w77-stats.py` was a throwaway, and the wave-74 edition **ignored its own argument** and
reported the wrong corpus with full confidence (wave-75 brief, LESSON 2). It is now a repo tool:
the run dir is a **required argument** (several may be given), it is **echoed on the first output
line**, seat logs are de-duplicated by **basename** (so a run dir and its `-final` twin cannot
double-count), and a log is EXCLUDED on stderr when it has no `gameend` or carries any
`fallback: timeout`. The kind census sums **every** record kind, calls out `recovery` / `defer` /
`wall_miss` by name, and prints a CENSUS CHECK line reconciling the gameend counter
`protocol_replies` against the record-kind sum (wave-77 engine-seat L4). Corpus-wide and ask-seam
figures are printed on separate, labelled lines. The `protocol_deviation` class histogram is new
(it is how S1's population will be read next wave).

Run on the wave-77 `-final` corpus, verbatim:

```
CORPUS: /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-010157-final
dirs 1 clean seat logs 42 = games 21
ALL KINDS {'gamestart': 42, 'system': 42, 'ask': 1999, 'reveal': 21, 'discard': 51, 'gameend': 42, 'attackers': 82, 'priority': 567, 'recovery': 5, 'blockers': 18, 'bottom': 2, 'defer': 1}
RECORD KINDS SUM (all kinds except gamestart/system/gameend) 2746
  ...of which recovery  5
  ...of which defer     1
  ...of which wall_miss 0
fallbacks ALL kinds {('ask', 'unparsed_reply'): 5, ('ask', 'noop_row_zero_reask'): 1, ('priority', 'repeat_count_reask'): 1, ('defer', 'menu_not_askable'): 1}
fallbacks (ask seam) {'unparsed_reply': 5, 'noop_row_zero_reask': 1} total 6
ask records 1999 model decisions 1999
with reasoning 1999 thinking values {'on'}
CORPUS-WIDE (all kinds): model decisions 2740 off_protocol_bytes>0 120 plan_line_missing 141 latency p50 82.5 s
ASK-SEAM ONLY: reply_truncated 0 off_protocol_bytes>0 103 plan_line_missing 118 action_before_plan 0
latency p50 83.0 s p95 175.6 s n=1999
reasoning_chars median 5817 max 35886
protocol_deviation classes {'compliant': 2594, 'unlabelled_plan': 115, 'plan_absent': 26, 'prose_outside_two_lines': 5}
decisions/turn (model decisions / sum of max turn per seat) 2.02
GAMEEND SUMS:
{"action_before_plan_replies": 0, "ask_replays_refused": 0, "ask_replays_reserved": 1334, "async_drops": 53, "blocker_forecast_collapsed": 2, "blocker_forecast_gang": 4, "blocker_forecast_multi": 178, "blocker_forecast_rows": 218, "chain_acting_rows": 1374, "chain_acting_rows_cast": 1490, "chain_selfharm_rows": 479, "chain_selfharm_rows_cast": 593, "chain_windows_collapsed": 0, "chain_windows_only_selfharm": 32, "chain_windows_only_selfharm_cast": 284, "crossphase_board_unchanged": 2, "crossphase_identical_reputs": 119, "declined_face_latches": 0, "engine_reveal_floor_picks": 0, "force_close_arms_deferred": 0, "force_close_arms_refused": 0, "force_close_defer_bound_hits": 0, "force_close_same_arm_deferred": 0, "forced_close_unrecorded": 10, "hold_released_turn": 132, "hold_windows_skipped": 2128, "hold_windows_skipped_cast": 1433, "hold_windows_skipped_priority": 695, "identical_ask_answers_reserved": 1073, "identical_option_asks_resolved": 8, "main_phase_windows_skipped": 89, "mana_only_windows_skipped": 69, "menu_pass_no_progress": 0, "own_loop_windows_asked": 1, "own_turn_windows_skipped": 596, "phase2_answer_missing": 1, "phase2_answer_recovered": 15, "plan_line_missing": 141, "plan_names_stranded_card": 4, "plan_names_uncastable_zone_card": 52, "protocol_deviation_replies": 146, "protocol_replies": 2740, "put_gloss_stripped": 0, "repeat_annotated_takes": 2, "reserve_decline_windows_noted": 28, "reserve_decline_windows_skipped": 4, "sibling_window_asks_skipped": 0, "stop_reached_windows_skipped": 7, "wall_miss_events": 0, "wall_miss_no_retry": 0, "wall_miss_unrecorded": 0}
CENSUS CHECK: gameend protocol_replies 2740 vs record-kind sum 2746 (difference 6 = the kinds that carry no round trip: recovery 5, defer 1)
askreplay files: 29
WINS per deck (from results.tsv):
deck152 6/6 deck146 5/6 deck162 3/6 deck125 3/6 deck123 2/6 deck126 1/6 deck130 1/6
```

Every figure reproduces `~/.gatelogs/w77-stats.out`, and the kind-sum **2,746** is now printed as the
census number with the 2,740/2,746 difference named rather than left to arithmetic.

---

## Weakest evidence

* **The cross-seat routing has never been observed to MOVE a record.** All four live defers in the
  probe were the acting seat's own menus, so `w78DeferRecordMoves` returned false every time; the
  move itself is pinned only by the pure function and by reading the site. A corpus with another
  cross-seat X announcement is the real test, and the wave-77 corpus produced exactly one in 21
  games. If the deeper `buildMenuChoice` defect is fixed first, the move may never fire again — and
  that would be the right outcome, not a falsification.
* **The live GREEN for S10 rode a FORCED gate.** `buildMenuChoice` was made to fail; the natural
  failure (a foreign seat's empty pool) was not reproduced. What the probe proves is that the defer
  SITE, with real menu objects, now emits real rows — not that the natural trigger still reaches it.
  The site is the same code on both paths, but the trigger is untested since the base corpus.
* **S1 is an instrument over corpus bytes, plus a census.** Nothing here was validated against a
  live model: the five replies are historical strings. Whether the next corpus produces the same
  shape at all is unknown — it appeared for the first time in three corpora, at one seat, in one
  deck's games.
* **`answer_label_absent_heuristic_played` is per-seat and per-game** like every gameend counter, so
  a corpus figure is a SUM across seats; a reviewer reading one seat's gameend sees only that seat.
  The measure also cannot distinguish the two of five where the heuristic AGREED with the model from
  the two that spent a card the model was holding — that distinction lives in the `recovery`
  record's `executed_text`, and joining the two is the reviewer's job, not the counter's.
* **The census helper's exclusion rules are inherited, not re-derived.** "No gameend" and "any
  `fallback: timeout`" are the wave-73..77 rules; I did not re-examine whether a seat log can finish
  a game and still be tainted in some way neither rule catches. The basename de-dup likewise assumes
  two runs never produce the same basename, which holds because the name carries the epoch and the
  seat pointer.
* **The suite cannot exercise any of this.** Both items live on the translog and the deviation
  class, and `TestSuiteAI` derives from `AIPlayerBaka` — there is no GPT seat in the suite. The
  1291/76 green says only that nothing else broke.
