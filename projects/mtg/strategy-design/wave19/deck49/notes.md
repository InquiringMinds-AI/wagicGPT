# notes.md — deck49 (Dragons) engine/representation ledger + rotation verdict (wave 19)

## Engine / representation ledger items (status after this corpus)

### E-49c — beneficial-aura enemy-only SUPPRESSION -> ANNOTATION — CLOSE (validated this corpus).
The owner-ruled swap shipped in the wave-19 batch. The mask lifted: enemy-only Goblin War Paint
reappeared at deck135 s4/s5/s7 with the warning annotation "every legal target is the OPPONENT's -
this would help their side unless you are deliberately triggering something." The model declined all
three, 0 loops, keeping the play legal. The fatal wave-17 C4 loop had a live opportunity and did NOT
recur. This closes the wave-18 WATCH (C4 "masked, re-test when mask lifts") — C4 now proven-solved.
CLOSE E-49c. (Boundary preserved: the annotation fires only on the enemy-ONLY menu; the mixed-menu
case is carried by the guide's never-aura rule — deck62 s10/s11 — so the guide rule stays.)

### E-batch19a — TRADE OUTCOME block annotations — VALIDATED at this seat (confirm, keep).
First corpus with per-pairing outcome tags on block options ("(both die)", "(you kill it, your blocker
lives)", "(your blocker dies, attacker lives, 1 tramples to your face)"). All ACCURATE where checked
(deck14 s23, deck110 s19, deck35 s16); the model READ them for correct blocks/no-blocks; NO misleading
annotation found. Bonus: at deck14 s23 the annotation corrected a guide/card-script stat mismatch
(Ashenmoor Liege engine-4/1 vs guide-4/4) by rendering the true outcome. No action; confirm and keep.

### E-batch19b — combat name tolerance / salvageLoopedChoice — no bleed at this seat (confirm).
0 discarded block replies (0 fallbacks); deck49 contributes 0 to the corpus's 21 unparsed. deck135
s11 self-corrected a malformed "B1:A1, B1:A2" opener into a valid single block and parsed cleanly. No
new block shape observed here. (The batch's value showed at 110/14/62, not deck49.)

### Card-script divergences (Oracle-verification queue) — UNCHANGED, still queued.
No card was mis-executed in a game-losing way this corpus. Standing list holds (Bloodmark Mentor
engine 1/1 vs real 2/1; War Paint engine {1}{R}/+2+2+haste vs real {R}/+1+1; Crucible engine {3}{R} vs
real {2}{R}{R}; **Ashenmoor Liege engine 4/1 vs real 4/4** — confirmed again this corpus at deck135
s13 / deck14 s23; guide describes ENGINE values). NEW queue note (G-49c): trade-outcome annotations
now MASK stat divergences at the decision surface, so card-script audits cannot rely on decisions
looking wrong. Queue, do not rush.

**Engine items this corpus: 3 active (E-49c CLOSE; E-batch19a VALIDATE/keep; E-batch19b confirm) +
1 standing queue (Oracle card-script list, unchanged).**

## Rotation verdict

**FREEZE byte-identical (no guide-mod). ROTATION CANDIDATE — with one honest caveat.**

- **No guide-mod warranted.** Every taught class is dead or persisted-correct across TWO consecutive
  guided corpora; 0 fallbacks both times; the compressed Rule #1 and trimmed Rule #2 held with no
  regression; the last open item (C4 / never-aura) was finally TESTED this wave (mask lifted) and
  PASSED. The one plausible trim candidate — the never-aura rule, now that the engine ships an
  enemy-only annotation — is proven STILL load-bearing (it carries the mixed-menu case the annotation
  does not, deck62 s10/s11). Nothing to add, nothing to cut. strategy.txt is byte-identical to wave-18
  (md5 a7d4813a51e89069222a2299b3fbafde).

- **No DECISION-level new-work signal.** The genuine new result this wave (E-49c validation / C4
  closed) is an ENGINE-LEDGER close, not guide work; it resolves a carried item rather than opening
  one. Per the brief's test (no-guide-mod AND no-new-work-signal = rotation candidate), deck49
  qualifies.

- **The one caveat (soft hold reason, not a blocker):** the flood/no-curve OFF-CASE added in wave-18
  has never been EXERCISED — the winning corpus never floods, so the "mild burn-hold dies under the
  off-case" claim is still unvalidated. This is an OPEN validation thread, not a defect. If synthesis
  prefers one more confirming cycle before rotating deck49 out, THIS is the reason: keep deck49 in
  until a flood game validates the off-case. If synthesis rotates it out, schedule the flood off-case
  re-test for whenever deck49 next appears.

**Recommendation to synthesis:** treat deck49 as CONVERGED on the no-guide-mod + zero-fallback +
C4-closed profile (a rotation candidate), with the flood off-case as the single unvalidated rule to
weigh. Judge future corpora by taught-class survival + tax, not the record (rule 1).
