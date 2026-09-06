# Wave-69 step-one input: KNOWN BUGS from the wave-68 evaluation (orchestrator list; details in the cited files)

Corpus: `matchups-20260906-134120` (21/21 natural, 74 min, 2414 records; binary 9be85773b; FIRST corpus on the rewritten
~20 KB guides and the per-seam token caps). Sources: `wave68/deck{146,152,125,126,162,123,130}/review.md`,
`wave68/engine-seat.md`. Adjudication (engine seat): 13 PASS, 9 FAIL, 12 UNTESTED (all wall-miss/retry/`attempt_ms`,
every ward claim, legend rule, deck-out countdown, Teferi, ANNOUNCE_X decline, single-activation execution, ATTACK
reversal — no window arose). Guides: all seven EDIT, all installed <= 20,000 B and grep-clean; no proposals.
THE WAVE'S TWO HEADLINES: (a) the rewrite lost nothing — every seat traced its losses to KEPT rules keyed to strings the
render never prints (dead literals at 0 renders), and re-keyed them; (b) the -46% generation time is ~90% the short
system prompt (100,328 -> 51,210 B median; per-byte decode rate fell with the halved KV prefix) and ~10% the caps, which
cut post-answer bytes only 3% — the caps bought a VARIANCE guarantee (max 1,233 -> 315 s), not throughput; wave 69 must
not tighten them on the headline number.

## HIGH — decisions lost to a false or missing claim, or an answer thrown away
- **K1 ENGINE: a game ended with no loss condition met (deck123 HIGH-1).** `game-146v123-1788720096.stderr:1952`
  `WAGIC_SELFPLAY_RESULT winner=0 life0=19 life1=11 turn=17`, libraries 42/43, immediately after Idyllic Tutor left an
  unresolved `StackAbility` (`reveal put 1 of 42`, `index_name_conflict` re-ask, drifted in-flight answer consumed) and
  Vision Skeins resolved two `10DrawAction`s. Cost deck123 one of six games. Root-cause by REPLAY of that game (the seat
  translogs + stderr are the record; `WAGIC_REPLAY_NOCONSUME=1` for synthetic lines) — which loss check fired
  (`MTGGameZones::drawFromLibrary`? the reveal's returned-cards path? a `CANTLOSE` inversion from BE R7?) — and fixture
  it. Related: Idyllic Tutor never decrements the printed library after the reveal (deck123 MED; deck125 126 s111-114).
- **K2 The caps ate decisions where no re-ask exists, and "recovered" a changed answer (engine HIGH-1/2/3; deck123
  HIGH-2; deck152 MED; BE-R3 FAIL).** 125v146 s133 (discard, cap 384, reply 1,409 B): `reply_truncated` +
  `unparsed_reply` -> the heuristic discarded 7 cards incl. Emrakul. 123v125 s23-25: truncated AFTER resolving a fetch, the
  "recovery" answered pass, stamped `reply_truncated_reask_recovered`. 123v162 s34: `CHOICE: 5 (Cast Damnation)` then
  "You should NOT cast Damnation. You should attack." — the correction was CUT by the cap (`post_answer_overrun 2253`),
  row 5 swept deck123's own 21 Humans one turn before lethal = the deciding decision of the corpus. Combat re-ask fired 0
  times (123v162 s19 truncated, no stamp). Lane BA sized the small-seam caps on wave 67's distribution; the rewrite moved
  it. FIX: the truncation re-ask at EVERY seam (discard/reveal/bottom/attackers/blockers included); a recovery whose
  executed answer differs from the truncated reply's coded line is `reply_truncated_answer_changed`, never "recovered";
  re-size every cap from THIS corpus's needed-bytes distribution (first byte -> end of PLAN) with headroom for a
  correction line; and the protocol tells the model a correction must come BEFORE the PLAN line (the cap cuts from the end).
- **K3 The reversal predicate misses the real reversals and fires on deliberation (deck123 HIGH-2; deck162 MED-3;
  deck152 LOW; BA-8 FAIL: 44 fallbacks = 6.3x wave 67, 25 `plan_choice_conflict`).** It missed 123v162 s34 (above);
  it fired on a 2,276-char PLAN ending "I will pass" after a correct CHOICE (125 s19) and on think-aloud whose re-ask
  returned the identical choice (125 s47/48; 130: 5/5 fallbacks same shape). The predicate reads the sentences BETWEEN
  the coded line and PLAN (the correction zone), never the PLAN body; a reversal names the alternative row or pass; a
  re-ask that returns the same answer is recorded and never re-asked again that window.
- **K4 The X library ceiling STILL under-counts owed draws with Staffs out; two rows print no verdict (deck125 HIGH).**
  126 s110: Staff draws owed, ceiling read one too high, deck-out at 65 life (BB's fold was meant to close this — verify
  which term is still missing: `@each my upkeep` permanents vs draw-step extras vs stack). `Life with Elixir` prints no
  `{right now:}` verdict while every neighbour does (412 renders; 126 s60/s111). `plan_contradicts_noop_row_reask` fires
  only off PLAN text — 14 K=0 sweeper takes (`destroys 0`) it could catch off the row's own zero (130 s20 shows it working).
- **K5 ENGINE rows that do nothing are offered and taken (deck146 HIGH-1/MED; deck125 MED).** `becomes beholder` rows
  tagged `[this land is TAPPED: animated, it still cannot attack this turn]` taken 2/2 (146: 162 s22 at 1 life, 125
  s243) — a whole turn's mana for a body that cannot attack or block: price it dead (`{right now: does nothing this
  turn}`) or fold it into the cast row's dead census. Cleanup rows' `{you already control one: Kaya}` / `{dead right
  now: ...}` ignored 4/4 while the cast menu's `[legendary: you already control` mark IS obeyed — one wording at both
  seams. An activation row offered at `Mana available: 0 total` (125: 126 s111/113).
- **K6 Crack-back and blockers surfaces still mislead (deck162 MED-1/2; engine MED; deck152 MED).** `{crack-back cover:}`
  projects a life total counting only the row's OWN new body — 162v152 s15 "you would be at 4" with two untapped
  blockers already in play, s19 "at 0" — the only survival arithmetic on screen reads "this cast does not save you";
  fold the existing blockers. `{reserve:}` ends in two operands ("leaves 3 sources, and ... needs 5") where every sibling
  ends in a verdict word. `{answers the stack: NO}` has no positive twin (66 s burned at 125v162 s68/69). The blockers
  header names a life-only assignment that spends material and the pilot copied it 2/2 (152: 146 s15/s19) — name the
  best-material assignment beside it.
- **K7 Record and surface debts that hide behaviour (engine MED; deck123 MED; deck146; BD-6/BD-9 FAIL).**
  `transport`/`attempt_ms` fields ABSENT everywhere (BC's record wiring never fired or never landed — verify);
  `async_drops` 2.15% and absent from gameend; 2,727 suppressed hold windows leave no record; the repeat row's short name
  contains the literal `N` (2 `repeat_count_reask`); `[repeat: activated this turn N times]` taken past 4 with no
  `repeat_past_stop` (BA-7 FAIL: 0 clamps); `- GANG BLOCK:` row tags and the collapsed multi-blocker tag render 0 times
  in 42 files (dead surface — delete or wire); `(neither dies)` gains a price clause so the literal never matches; band
  rows still 1,425 B; 435 past-turn `- Paid ... with` lines survive the fold; two literals for one tap fact
  (`{tapping X now:}` vs `{paying this taps:}`); 18 unusable counter rows on the Adversary menu.
- **K8 The `[second copy: adds nothing new]` tag is FALSE for stacking triggers (deck126 MED).** A second Sanguine Bond
  doubles the drain (mtg.txt:99004), a second Staff of Nin is a second ping (mtg.txt:112425); the tag must read the script
  (a triggered/static effect that stacks is "one more of the same effect"), not assume legend-rule semantics.

## MED
- The blockers seam's reasoning-first replies remain the latency tail (max 315 s is a blockers reply).
- Katilda upkeep activation taken 4/17 against "MAIN 2 ONLY" — arithmetic favoured it (152: reported, not edited).
- Soul Shatter "about three tied cards" was quoted back verbatim into a bad cast (146; re-keyed in the guide).
- Reply degeneration/unparsed at 123 s64, 146 s133, 126 s104 (deck125 MED) — the 1 `unparsed_reply` is K2's s133.

## Carried, untested (no window arose)
Wall miss / retry / `attempt_ms` (0 wall misses — but the fields are absent, K7), every ward claim (no ward trigger
fired), legend rule, deck-out countdown, Teferi stage 1, ANNOUNCE_X decline, `stop_conflict_single_activation_executed`,
ATTACK-reversal firing; from earlier waves: reservation-decline latch, `label_missing_*`, AZ R2/R5/R6, AN-5, AN-7, AP-R3/R5.
