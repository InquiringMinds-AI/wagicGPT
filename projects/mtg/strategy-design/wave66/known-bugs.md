# Wave-66 step-one input: KNOWN BUGS from the wave-65 evaluation (orchestrator list; details in the cited files)

Corpus: `matchups-20260906-033121` (21/21 natural, 0 crash/hang, 2339 decisions, 0 heuristic fallbacks, 8 re-asks; binary
0f5e0c7be). Sources: `wave65/deck{146,152,125,126,162,123,130}/review.md`, `wave65/engine-seat.md`. Adjudication (engine
seat): 15 PASS, 1 FAIL (AO-3 by its letter only), 6 UNTESTED (AN-5, AN-7, AP-R3, AP-R5, AP-R6, AP-R7 accented half).
Guides: 162/146 EDIT installed; 152/125/126/130/123 KEEP; no proposals, no synthesis. Lane-flagged answers: G7's
re-opener costs +2.8 re-offers/seat-game for -3.4% total decisions (scope stands); G4 starves nothing (7/8 lethal-floor
windows still took the body row); 162v123's -1089 was deck123's Intruder Alarm + Lord of Lineage token loop rendered
correctly (`277 attackers, 1109 unblocked`) — the defect is upstream (H3), not the verdicts.

## HIGH — decisions lost to a false or missing claim
- **H1 DRAW FORECAST re-charges the whole draw step on every window inside it and turns a false death verdict (engine
  HIGH-1; deck152 HIGH-2).** `drawStepForecastText` (~`AIPlayerGPT.cpp:10931-10967`) never decrements for draws already
  resolved: 123v162 seq 138-153 render the identical `resolving NOW ... = 6 life LOST BY YOU` for 16 windows while life
  falls 7->3, and at s152 (life 4, 1 point still owed) the tail says `you would be at -2; that KILLS you` — seat lived at
  3 and won. 152: seqs 41-49 `draws 4 cards = 4 x 4 = 16` beside `ON THE STACK: 7 damage` (23 claimed vs ~15 true); s42
  cost 190 s and a 2927-byte overrun re-deriving it. 12 runs of >=3 identical re-forecasts across 2 seats. Charge only the
  draws still ahead (count resolved draw events in the current step).
- **H2 First-wins executes the answer the model wrote BEFORE it reasoned (engine HIGH-2; deck130 HIGH-1; deck123 HIGH-1;
  deck126 HIGH-2).** `post_answer_overrun` on 1598/2209 replies (72%); the CORRECTION marker was used 2 times. 130v162 s61:
  `CHOICE: 3 (Cast nothing)` then "Re-evaluating:" then `CHOICE: 1 (Cast Spark Spray)` to kill Ob Nixilis at 1 loyalty —
  ignored, game lost 0-to-6. 123v126 s36: took `x30` on the repeat row, then "Correction: ... M (41) is already above stop
  (33)" + `CHOICE: 0 (pass)` two lines below the original — AP's adjacency test missed it, 30 activations ran. Measured
  price of first-wins: 33 `later_answer_ignored` (1.5%), 27 restatements, 6 a different row, 1 clearly worse; it removed 3
  silent flips + 1 plan mismatch — KEEP the rule. Fix the two halves: (a) the protocol must demand reasoning BEFORE the
  coded line (answer last, or a mandatory one-line `DECIDE:` preamble) so the first coded line is the reasoned one;
  (b) the retraction-marker set must accept a "Correction:"/"Re-evaluating:"/"Actually," HEADER line within 2 lines
  before the later coded line, not only the adjacent line. Also: the `later_answer_ignored` record trims everything past
  the PLAN (`[+2069 bytes ... trimmed]`) so the ignored line itself is lost — record the ignored line verbatim.
- **H3 The unbounded repeat row has no `{right now:}` verdict and no stop guard; loops bill one round trip per iteration
  (deck123 HIGH-1/2; deck130 HIGH-2; deck162 HIGH-1; engine LOW-1).** 162 seq 73 prints M but not L, C, or any usefulness
  clause; the seat wrote stop=20 in its PLAN and took x33/x10/x21/x20/x47/x30/x200 in 10 of 13 windows, reaching 266
  creatures vs an opponent on 15 (92% of that game's decisions). The row already DEMANDS a PLAN with stop/current/this
  window: read it back and refuse the row when stop - current <= 0 exactly as it refuses a missing plan; print
  `{right now: M=.., stop=.., this window adds ..}`. Separately 130v126 seqs 37-55: Sanguine Bond + Exquisite Blood billed
  19 windows (life 19->1) all answered "Cast nothing" — a proven-closed loop (`lifeLoopProvenWin`) whose responder has no
  instant should resolve without asking (auto-pass under the existing hold/decline machinery, recorded).
- **H4 X-pricing's "leaves you alive" ignores stack damage (deck125 HIGH-1).** 125v162 s36: the same prompt prints
  `ON THE STACK: 13 damage to you - you would be at -8; that would KILL you` and `X=4 is the largest listed X whose NET
  (-4) leaves you alive, at 1`; recurs s36-39, 5 of 8 stack-damage windows. The X bracket must start from the post-stack
  life the prompt already computed.
- **H5 Cleanup-discard menu prints `{spare:}` then orders against it (deck125 HIGH-2).** 125v126 s55: row 1 is the
  sweeper that destroys 7, three `{spare:}` lands sit at rows 5/6/8, model took row 1. 31 discard asks, 12 offered a spare
  row, only 6 discarded spares only. The discard seam's row order/heading must put the spare rows first and say "discard
  the spare land" as the default, or the ask inverts the cast menu's value polarity.
- **H6 Target-choice rows omit summoning sickness (deck152 HIGH-1).** `summoningSickTag()` (`:3553`) has one caller
  (`:7641`, the battlefield serializer); target rows carry `[doesn't untap ...]` but never `[summoning sick]`. 152v162 s35-37
  put all three Luminarch counters on a Tracker cast that turn, plan "attack with ... Tracker" — lost the only 152 game.
  Also deck152 MED-1 (Aspirant self-target) is the same row.
- **H7 Hold cannot hold: the hold key includes volatile annotation payload (deck152 HIGH-3; deck123 MED; deck125 MED).**
  Rows differing only by the DRAW PRICE projection ("you would be at 4" -> "at 3") break the hold; 152's loss game T15
  draw step ran 9 consecutive no-op windows, 334 s; deck-152 total 16 windows / ~630 s in 4 runs. And the decline row wins
  over hold almost always (123: hold 4 vs decline 182; 125: "Cast nothing" 213 vs hold 29) — the churn cost is the plain
  decline, not G7's re-opener. Key the hold on row identity with numeric payload stripped (the hold-key normalisation of
  wave 63 covered phase, not annotation numbers), and make the hold row the default decline when the seat holds an
  instant-speed answer.
- **H8 The re-ask's worked example is an answer attractor (deck126 HIGH-1).** All 3 `named_row_reask` in the corpus are
  this seat and all 3 recovered to the exemplified row 1 verbatim; twice that row was `Tribute to Hunger {right now: they
  control 0 creatures - at 0 this does nothing}` and s34 stamps `plan_contradicts_noop_row` and executes anyway. Both
  Tributes burned T22/T26; at T35 the same card was gain 15 / drain 15. `askExemplar` (~`:15019`) must not name a row
  `rowSaysNoOp` flags (~`:30714`), and `plan_contradicts_noop_row` must re-ask, not execute.
- **H9 Modal mode ask contradicts the casting row's own live/dead census and ignores the opponent's draw punishers (deck146
  HIGH-1/2).** 146v162 s23->24, 146v126 s61->62: the cast row's census says which Silverquill modes are dead, the mode ask
  prices half-dead pairs as whole modes and gives dead rows 1/4 no tag; both times the seat took a DEAD pair (s24 lost the
  game). The mode ask's life forecast says "at 17" where Underworld Dreams (printed in the same prompt) makes it 16 — the
  cast row's DRAW GRANT bracket already does this arithmetic. One census, both seams (AL's modeRowAnnotations is the place).
- **H10 Threat-blind pricing at two seams (deck130 HIGH-3; deck162 HIGH-2).** Sweep bracket: 130v123 s44 `board sweep:
  THEIRS 1 / YOURS 2 ... (it takes more of YOURS than of THEIRS)` where THEIRS-1 was Bloodline Keeper under Intruder
  Alarm; model cycled Starstorm and died at -78 to 40+ vampires. Forced sacrifice: 162v126 s15 offered Shield Sphere (pays 5)
  and Fate Unraveler (pays 4) while the same prompt printed `DRAW PUNISHERS on the battlefield: yours - Fate Unraveler`;
  "pays the least" pointed at the punisher. Both brackets must name engines/punishers (token makers, draw punishers, loop
  halves) as not-a-body, using the detectors the prompt already runs.

## MED
- `name_over_index` resolves number/name disagreement silently outside the reserved-row classes (engine MED-1; 130v125
  s97/98) — route every conflict through `index_name_conflict`.
- Plan carry truncates the correction and keeps the false premise (deck126 MED; 130 s21->22, 29/183 windows); the
  staleness annotation flags only names that left the MENU — a 15-turn "cast Idyllic Tutor" plan naming a card in no zone
  went unflagged (125 s45->57) and drove H8's off-menu replies.
- Reveal windows stall the engine 835 s total, max 139 s (engine MED-3); 3 decisions at 90-94.2% of deadline (MED-4).
- Converter clause gives no per-row auto-tap verdict though the row knows its tap set (deck130 MED; 130v126 s32 "No safe
  play exists" with 6 castable cards); mode menu drops the cast row's `{kills 0 of the 1 CREATURE target}` arithmetic
  (130v152 s77->78).
- Land-drop rows price mana only, never that the land is a threat (deck146 MED; 146v125 s84/85 Hive declined twice at
  2 life).
- `index_name_conflict` re-asks on UNIQUE names burning windows (deck123 MED; 162 s29/68/111, all 3 corpus-wide);
  `no_pass_reask` because casting menus have no row 0 while priority menus do (s45).
- A flash punisher offered in the seat's own draw step shows no reservation against the sorcery-speed punisher stranded in
  hand (deck162 MED; 162v123 s11/14); `= 6 x 1 = 6` never states the margin of one more punisher vs one more engine.
- Annihilator 6 costs six full-prompt round trips (deck126 LOW; 125 s57-62).
- The life-LOOP fact is named on 1 of 2339 decisions (engine MED-2) — AN-1's positive half stays unproven.

## Carried, untested (no window arose)
AN-5 (committed blocker excluded from the header), AN-7 (you-gain forced-sacrifice branch), AP-R3 (adjacent marked
correction), AP-R5 (double-strike lifelink), AP-R6 (curl 28 band — 0 transport errors this corpus), AP-R7 accented half.
