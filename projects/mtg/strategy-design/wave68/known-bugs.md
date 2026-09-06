# Wave-68 step-one input: KNOWN BUGS from the wave-67 evaluation (orchestrator list; details in the cited files)

Corpus: `matchups-20260906-102638` (21/21 natural, 0 crash/hang, 101 min, 2436 records, 7 fallbacks, `unparsed_reply` 0 —
wave 66's 83 are gone; binary 14977f3af). Sources: `wave67/deck{146,152,125,126,162,123,130}/review.md`,
`wave67/engine-seat.md`. Adjudication (engine seat): 22 PASS, 8 FAIL (AV4/AV5 overrun — the ORDER lever is spent:
answer-first 98.9%, overrun 662,814 B; AW "row and header cannot differ" falsified; AX "zero retry budget" falsified;
AY/AZ own-stop unreachable on the plain row; I9a byte saving zero after AZ R4; others in the file), 6 UNTESTED
(reservation-decline latch 0 windows, `repeat_clamped_to_own_stop` 0, `label_missing_*` 0, AZ R2 boundary, AZ R5/R6).
Guides: 152/126/125/162/123/146 EDIT installed (all within 20 B of the 71,500 ceiling — the pool is at the band wall);
130 KEEP; no proposals. HOLD 435 takes / 1455 renders. Reveal fix reached all 8 Idyllic Tutor windows, 0 floor picks,
0 stalls.

## HIGH — decisions lost to a false or missing claim, or an answer thrown away
- **J1 The own-stop guard is blind to the row the model actually uses (engine HIGH-1; deck123 MED-1).** `repeatPastStop`
  (~`AIPlayerGPT.cpp:32639`) is gated on `namedCount >= 1`, i.e. only the counted `repeated N times` row — 4 records in the
  corpus. 20 records executed a repeat past the reply's OWN stated stop on the plain single-activation row (123 s27-s48,
  13 windows walking M 24->35 against stop=23). 12 of those 20 carry `decision_reversed_in_prose` ("This window: pass")
  but `proseNamesOtherMenuRow` narrowed 15/15 candidates away, so the re-ask fired 0 times. The stop guard reads the
  stated stop against ANY activation of the same ability (counted or single); a prose reversal that names PASS/hold is
  never narrowed away by a row-name mention.
- **J2 A 900 s wall miss retries with a FULL second deadline (engine HIGH-2; deck162 HIGH-1; deck130).** 162v126 s13:
  `latency_ms 1233411`, `deadline_pct 137`, `retry 1`, `phase=wall`; the wall arm (~`:15196`, `mRetryBudgetMs = 0` = "the
  full deadline again") re-spent 333 s, and `:15090` sums both legs into one `latency_ms` that `:16047` divides by one
  deadline. Lane AX's "zero retry budget by arithmetic" was wrong. A wall miss gets NO second full deadline: either no
  retry (Baka, recorded `wall_miss_no_retry`) or a bounded retry sized by a stated remainder; the record splits
  per-attempt latency (`attempt_ms: [900035, 333376]`) so p90/max read the truth.
- **J3 Overrun IS the latency tail, and the protocol order lever is spent (engine FAILs AV4/AV5; deck146 HIGH-3; deck126
  HIGH-1/2; deck152 81%; deck130 86%).** 146v125 s19: 5,675 B past the answer, 866 s (34 s under the wall); 126's two
  answer-LAST blockers replies (825 s, 462 s) score 0 overrun because the meter counts only post-answer bytes; 152: 50 of
  161 seat-minutes in 20 replies. Reasoning did not shorten — it moved. Two mechanisms, both required: (a) a HARD stop —
  the request's max_tokens per seam sized to answer + PLAN + a bounded reasoning allowance (the current
  `WAGIC_GPT_MAXTOKENS=6000` is the ceiling that lets a reply run 866 s; blockers/attackers/cast seams get their own,
  smaller caps), with a truncated-before-label reply handled by ONE re-ask that quotes the cap, never Baka; the protocol
  states the budget in words the model obeys ("at most N sentences before the PLAN line"); (b) the overrun meter counts
  bytes on BOTH sides of the label so answer-last replies are visible. Measure the cost-of-wait per seam in the corpus
  (which seams burn the minutes) before sizing; record `reply_truncated` when the cap bites.
- **J4 ANNOUNCE_X: the row clause and the ask header state different ceilings, and `; M more pending` renders 0 times
  (deck125 HIGH-1/2; AW claim falsified).** 125v130 s90: header "3 draws are already owed ... X=1 is the largest value the
  library pays for", option 6 `{library: this draws 4 of your 4 library cards - 0 left}`; seat took option 6 and emptied
  its library with 3 undeclinable draws owed (survived only by an Emrakul reshuffle). One ceiling function, both
  surfaces, pending draws folded into the ROW clause. And ANNOUNCE_X is the one menu where row name and index are both bare
  integers (`CHOICE: 2 (X = 2)` -> `index_name_conflict`, 125v162 s90) — the X menu's parser treats a bare-integer name as
  the X value, never as a row index conflict.
- **J5 A post-announcement decline strands paid mana with no warning; `[cannot pay now: needs N mana, you have 0 untapped
  sources]` ignores the floating pool (deck130 HIGH-1/2).** 130v126 s21-24 (6 sources + Talisman burned, T10), s27-31 (a
  false `Paid ... from mana already floating` line for a cast that vanished); 130v162 s70-73 lost the deck's own lethal
  line. The decline row on a post-announcement menu says `{declining now FORFEITS the N mana already paid}`; the
  cannot-pay tag folds the floating pool (55 of 82 floating-pool prompts wrong; 6 contradict an offered Cast row in the
  same prompt).
- **J6 Prose reversals are not honoured at three seams (deck130 HIGH-3; deck123 HIGH-2; engine MED).** `decision_reversed_
  in_prose` is not wired at ATTACKERS (0 of 74; 130v126 s24 executed `ATTACK: A1-A4` while the PLAN said "I must NOT
  attack ... I will pass combat" — 6 life to a lifegain deck, lost 0/34); `answer_replaced` never fires on a prose-only
  retraction (123v130 s43: answered Devour Flesh, wrote "at 0 creatures does nothing... I will hold priority", cast it);
  the no-op re-ask lost to the one-re-ask-per-board budget and a dead Devour Flesh was cast at 7 life (engine MED). Wire
  the reversal detector at every seam through the same predicate; a `{... does nothing}` / `destroys 0` row's re-ask is
  exempt from the per-board budget (it is a different question).
- **J7 CARD SCRIPT: ward leaks hidden zones; Teferi's +1 cascade shows nothing at stage 1 (deck152 HIGH-1/2).** Moonrage
  Brute `borderline.txt:73998` `target(*[instant;sorcery]|opponentzones)` — 152v130 s33 printed 12 opponent LIBRARY cards
  and a HAND card as pickable rows, mislabelled as the ward controller's target choice; the model "exiled Rorix" paying 3
  life. Should be `opponentstack` (198 uses in the same file); audit EVERY ward script in the pool (`/wagic-cardscript`;
  verify against Oracle: ward = counter unless the caster pays). Teferi +1 stage-1 gate renders `1. choose a creature /
  2. Decline` with no target info and no pass row after it (152v123 s39-41, 427 s, declined all three while the plan
  named its target; 152v125 s40/57 committed onto an all-`[tapped]` list) — the stage-1 ask lists the candidates the
  stage-2 pick will offer, and the pick order is declared.
- **J8 Modal and edict rows price the wrong side (deck146 HIGH-1/2; deck126 MED-3).** Silverquill Command's sacrifice
  mode prints no edict floor (146v123 s31: `{this mode has a legal object right now}` against 31 creatures — planned to
  edict Thraben Doomsayer, killed 2 of 31 tokens) while standalone edicts print `they control N creatures - they choose`;
  "opponent draws" modes are annotated as a benefit and the DRAW GRANT bracket prices their draw as `take 0 off them` —
  the model wrote "a 2-for-1 in card advantage (I spend 1 card, they draw 1)" (146v125 s16/20); Tribute to Hunger's row
  never prints `{crack-back cover:}` though creature rows do (126 g123 s12). One edict floor for modes and standalone; a
  granted draw is a PRICE (`#W47-R3` framing); the cover clause on every row that removes an attacker.
- **J9 The ON THE STACK death verdict sits ~40 lines from the answer; a hold does not re-open when CRACK-BACK crosses into
  lethal (deck123 HIGH-1; deck162 MED-3).** 123v162 s32: `ON THE STACK: 3 damage to you - you would be at -1; that would
  KILL you` with 5 mana and a self-targetable Devour Flesh on row 1; the model wrote "Stack resolves (damage to 2)", held,
  died holding 25 lethal attackers. Repeat the death verdict inside the option block (as the blockers seam does), on the
  row that answers it. 162v146 s19/20: `hold_windows_skipped 56` in the lost game — the hold key must re-open when the
  CRACK-BACK verdict word changes (survive -> lethal), which AU R1's six clauses do not cover.

## MED
- A `named_row_reask` recovery record carries neither `executed_seam` nor `recovered_by` (engine; 123 s42) — add the
  `noteHeuristicExecuted` site on that path.
- I9a's byte saving is ZERO after AZ R4 (7 band windows, 21/21 rungs, 2514-2737 B) — keep every rung but shorten each
  band row to its label + `(same effect right now)`, ~40 B.
- Narration still reaches 484 lines / 30.5 kB post-bucketing (engine); `async_drops` 22.
- The legend-rule ask prints neither the rule nor either copy's loyalty (146 s51 — model fabricated "4+4=8"); `NO LIVE
  CAST ROW` never says the land drop / manland / attack / walker ability are still live (146 MED).
- Undeclared pick order in multi-stage cascades (152 MED: land/creature/artifact); gates asked for types the opponent
  has none of (152 s59).
- The seat is never shown its opponent's library size as a deck-out forecast (162 LOW: won by opponent deck-out at 10
  life without seeing it coming).
- Five windows on rows priced dead (2 Damnations at `destroys 0`, 2 second-copy Alarms, 1 Devour Flesh) — the guide
  states it in 6 places; J6's budget-exempt re-ask is the engine answer (123 MED-2).
- Reasoning-first replies at the BLOCKERS seam are the whole latency problem there (126 HIGH-2: 60/62 non-blockers replies
  answer-first; both answer-last replies were blockers) — J3's per-seam cap.
- The pool of guides is at the band wall (six of seven within 20 B of 71,500): the next EDIT must cut before it adds.

## Carried, untested (no window arose)
Reservation-decline latch (0 windows on 42 seats), `repeat_clamped_to_own_stop` (0 — J1 explains why), `label_missing_
salvaged/reask` (0), AZ R2 allowed==1 boundary, AZ R5 may-draw reserve, AZ R6 graveyard recursion; from wave 66: AQ H4, AQ
H9 punisher fold, AS H3b loop autopass, AU R2 provenance, AU R5 all-dead; from wave 65: AN-5, AN-7, AP-R3, AP-R5.
