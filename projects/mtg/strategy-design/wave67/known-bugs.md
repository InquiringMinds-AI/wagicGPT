# Wave-67 step-one input: KNOWN BUGS from the wave-66 evaluation (orchestrator list; details in the cited files)

Corpus: `matchups-20260906-064038` (21/21 natural, 0 crash/hang, 127 min, 2317 records / 2191 decisions; binary 9b797fb81).
Sources: `wave66/deck{146,152,125,126,162,123,130}/review.md`, `wave66/engine-seat.md`. Adjudication (engine seat): 22 PASS,
7 FAIL (AQ H1 clause-a benign trigger lag; AR H2a rate 66% vs its 65% bar; AR H2a/2 `later_answer_ignored` 33->48; AR H2c
32/48 equal; AT H5 outcome 5/16 vs 6/12; AT MED-3; the unpredicted H2a composition defect), 8 UNTESTED (AQ H4, AQ H9
punisher fold, AR H8/2, AR MED-1, AS H3b, AT deck130(b), AU R2, AU R5). Guides: 162/125/123 EDIT installed; 146/152/126/130
KEEP; no proposals. HOLD taken 393 vs 126 plain declines (wave 65: 4:182) — AS H7b inverted the ratio. THE WAVE'S HEADLINE:
lane AR's protocol reorder (REASONING -> ANSWER -> PLAN) cost 83 `unparsed_reply` -> Baka (3.6%; wave 65 had 0) and did NOT
produce reasoning-first (73% of replies still put the coded line on line 1). Items I1-I3 are that one defect in three parts;
fix them as ONE design, measured against the corpus replies verbatim.

## HIGH — decisions lost to a false or missing claim, or an answer thrown away
- **I1 `ANSWER: CHOICE: n (...)` is refused as unparsed (engine HIGH-1; 130 HIGH-1; 126 HIGH-1; 125 HIGH-1; 123 HIGH-3; 162
  HIGH-1).** 45 of 83. The model transcribes the protocol's three section names (REASONING / ANSWER / PLAN) as literal labels.
  `findAnswerLabelLine` (`AIPlayerGPT.cpp:17506`) skips only ` \t*#` before matching the four labels, so `ANSWER:` is prose and
  no head is found — while `ANSWER: BLOCKS:` / `ANSWER: ATTACK:` / `ANSWER: PUT:` parse at their seams (162v146 s19 vs
  162v123 s13; 126: 24/24 ask + 19/19 priority refused). Worst: 162v126 s22 an exactly-lethal Ob Nixilis -2 refused; 125v146
  s6 the model REFUSED to Essence Scatter into Silencer, Baka cast it (game lost 0-20). Same shape `CORRECTION: CHOICE: 3`
  (130 s52), `So PUT: 1, 8` (126 s258). Fix at the label scanner (~`:17538`): a heading word + colon (`ANSWER:`,
  `CORRECTION:`, `THE ANSWER LINE:`, `So`) before a label is skipped at EVERY seam; and stop naming the sections with words
  the model will write as labels.
- **I2 32 replies carry NO coded line at all (engine HIGH-2; 130 HIGH-3; 125 HIGH-1).** Complete replies, 31 with a `PLAN:`:
  the prose says "Therefore, I cast nothing." / "I will choose option 1." and the label is never written. The reorder pushed
  `PLAN:` past the 300-byte ramble guard at `:18137`, so the salvage that caught label-less replies under answer-first is
  gone. 125 s58 lost a decided Talisman to Baka's Stone Rain. A reply with a PLAN and zero coded lines is a clean salvage
  trigger: name-match the last "cast/choose/play X" sentence before PLAN against the rows, else ONE re-ask quoting the
  protocol line — never Baka on a complete reply.
- **I3 The reorder did not produce reasoning-first, and the retraction vocabulary is wrong (130 HIGH-2; 152 HIGH-2/3; 123
  HIGH-2; engine FAILs).** 73% of replies (191/263 on deck 130) put the coded line on reply line 1 with no reasoning above;
  overrun 60-79%; `later_answer_ignored` rose 33->48 and 125 s60 RAN `Cast Stone Rain` while its conclusion was `Cast
  Talisman of Impulse`. The model's real retraction token is "Wait," (152: 20/131) and "Correct Plan:" (123v126 s114:
  `CHOICE: 3 (Cast Damnation)`, retracted in prose, `CHOICE: 6`, first-wins cast the Damnation and killed its own 102
  creatures — decided the game); H2b's enumerated headers fired 8/131 and the one in-window `Correction:` (130 s22) was
  factually false, so 152 recommends NOT widening H2b. DESIGN, not patch: the answer needs a STRUCTURAL anchor the model
  already obeys — it writes `PLAN:` in 98% of replies; measure on the corpus how often the coded line IMMEDIATELY BEFORE
  `PLAN:` is the model's real conclusion vs the first coded line vs the last, and adopt the anchor with the best count,
  recording every override; the protocol says exactly that anchor in one sentence. The 152 overrun cost (50 of 161 min in 20
  replies; 493 s on a land drop) argues for a hard answer-then-stop shape.
- **I4 The LIBRARY CEILING ignores draws already on the stack (engine HIGH-3; 125 HIGH-2).** 125v123 s130 (T59): library
  14, two Staff of Nin draw triggers on the stack, the badge reserves 1 and recommends X=13; the seat drew 13, the triggers
  finished the library — lost at 44 life vs 3 (the lategame specimen). 125v126 s273: library 22, ceiling 19 -> X=21, decked.
  14/14 X windows took max X. Every drawing X row prints `{draws N of your L library cards - L-N left; M more draws pending
  on the stack/this turn}` from the pending-draw scan that already exists (`stackPendingDrawsFor`) plus the draw-step forecast.
- **I5 A refused reveal reply VOIDS the spell; reveal stalls are real engine stalls (126 HIGH-2; engine HIGH-4; AT MED-3
  FAIL).** 126v162 s10 `ANSWER: PUT: 44` (row 44 eligible) refused -> s12 "the reveal could not be completed - the engine
  returned the 52 revealed cards to your library": deck 126's only Idyllic Tutor voided on turn 5 (lost never holding a combo
  half). A refused reveal degrades to a legal pick (Baka's chooseCard), never a void. Both `reveal_stall_forced` (driver_secs
  600, latency -1) are genuine engine-side stalls — the wave-58/59 signature recurs; and `reveal_wait_unexplained_secs` is
  read (`:15684`) after `mLastLatencyMs` is consumed (`:15348` stamp, `:15414` consume), so 14/16 reveals report the whole
  wait (1622 s reported vs 1200 s real). Fix the field order, then root-cause the 600 s driver stall.
- **I6 The repeat-row re-ask is advisory and its verdict reads the wrong PLAN (123 HIGH-1/4).** 123v126 s84 told the model
  "you at 66 with your stop at 26"; s85 answered the identical `x34` and it RAN (M 66->100); same s81->82. 17/20 recovered
  to pass, 3 executed past the stop. And the `{right now: M=.., your stated stop=..}` clause appeared in 1 of the 20 refusal
  windows (43/91 renders) because it reads the CARRIED `YOUR PLAN` block while the guard reads the reply's own PLAN, and the
  carry drops after refusals. Read the same source for both; when the second answer repeats the refused take past the stated
  stop, execute the STOP the model itself stated (the model's number, not a cap) and record `repeat_clamped_to_own_stop`.
- **I7 The record does not store what Baka executed on a fallback (engine MED-1, load-bearing); a re-ask inside an unchanged
  step lets the model flip a reservation (162 HIGH-2).** `choice:-1`, `chosen_text:<refused>` on 76/83 — 51 unadjudicable.
  Store the executed row/action on every fallback. 162v130 s16->17, s18->19: a mana-reservation decline was re-asked in the
  same step with nothing changed and the model flipped (1 life, UWD#2 in hand = exactly lethal forecast, cast Dictate
  instead; lost). A decline whose board key is unchanged is honoured like a hold (the AS H7 machinery), not re-asked.
- **I8 The graveyard is never rendered, and LOOP HALF PENDING names an EXILED half (146 HIGH-1/2).** 146v130 s29/31/33:
  Silverquill Command's live-mode census was correct but no window lists the seat's graveyard; at 3 life it spent 165 s
  re-deriving from the log, concluded "contradiction", declined, died. Add `Your graveyard (N): ...` beside `Your
  battlefield`. 146v126 s21-33: both halves exiled by Vanishing Verses, the clause still says "one resolution from closing"
  (the three windows = 674 s). Gate the clause on a returnable zone.
- **I9 Repeat-pay mode menus print every unreachable rung; `plan_contradicts_noop_row` did not fire on the textbook case
  (152 HIGH-1; 126 HIGH-3).** 152v146 s11: 21 rows (~2.7 KB) for Intrepid Adversary when 1 payment is affordable, rows
  3-21 identical in effect (5 occurrences, ~13.5 KB); collapse unreachable rungs to one line naming the reachable max.
  126v125 s83/84: row `{they control 0 creatures - at 0 this does nothing}`, plan "this does nothing. This is a waste" —
  both Tributes burned; AR H8's conjunction missed it (find why: `planArguesAgainstRow` wording?).

## MED
- ATTACK TOTAL gives both numbers but never says lethal/not (146 MED; 130 s20 the model called "lethal" a line leaving
  them at 3; 3 windows).
- `[no cast row now: sorcery speed - only in your own main phase with an empty stack]` never says which half failed (130
  MED; 125 s52 7,212 chars arguing, no answer).
- MDFC land rows omit the `enters UNTAPPED` clause ordinary land rows carry (152 MED-2; model imported Overgrown Farmland's
  condition onto Hengegate Pathway — `borderline.txt:52388` says untapped).
- Opposing statics tagged on a creature without naming their source (152 MED-3; 8 KB of flailing in vs123).
- Exemplar names a HALF DEAD row (engine MED; 146 s22).
- 2 wall misses are 900 s curl-28 EMPTY replies with `connect_ms=20000` and no retry (146 s11; 162) — AP-R6's band logic
  says a connect timeout is `transport_error` with the bounded remainder; verify why a 900 s wall was reached at all.
- The X answer window gets no deliberation (125 HIGH-3 -> MED: cast row 48 s, X menu 3.3 s).
- Tribute rows at 20-23 creatures print no "you gain at least N" floor (126 MED).
- Hold churn: 8-window run in one draw step at 3 life (130; 162 s28-35); an 18-window drained turn with no new fact (123 MED;
  162 s51-68); `async_drops` 59.
- The turn-14 log prints ~200 per-token death lines while the board line is bucketed (123 MED; 126 s115).
- Cast row "max affordable X=20 (23 mana total)" while the board read 24 and the menu offered 21 (125 MED; 1 of 17).
- The vs-162 deciding deck126 reply carried no REASONING at 2,055 ms (126 MED) — the reorder is not obeyed under fast replies.

## Carried, untested (no window arose)
AQ H4 (X after stack), AQ H9 punisher fold, AR H8/2, AR MED-1, AS H3b (loop autopass — 0 proven-loop windows with no legal
action), AT deck130(b), AU R2 (provenance), AU R5 (all-dead exemplar); from wave 65: AN-5, AN-7, AP-R3, AP-R5, AP-R7 half.
