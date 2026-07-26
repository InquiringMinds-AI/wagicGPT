# deck18 skill (wave-28) - the SECOND complete Step-0 -> guided -> validated -> EXIT arc

deck18 is now the second deck to run the FULL deck59 pipeline arc end to end (after deck59
itself): Step-0 guideless read (wave-26, 4/6) -> initial guide targeting three named leaks
(wave-26) -> first-guided validation (wave-27, 5/6, all three leaks converted) -> 6/6 on the
FROZEN guide + rotation (wave-28). This is the first time the loop has been observed through
its terminal EXIT gate, not just its validation gate. Bar: proposals ride evidence; PASS/EXIT
is a success verdict.

## PIPELINE TEACHING #1 - the arc TERMINATES cleanly in TWO corpora, not one.

deck59 taught the loop's front half (guideless -> guide -> validate). deck18 completes the
picture: a first-guided seat needs ONE validation corpus to confirm the edits (wave-27), and
ONE MORE corpus to confirm the guide is DONE (wave-28: leaks still hold on a frozen guide,
record improves 5/6 -> 6/6, no new GUIDE-work). The exit is not "we got bored" - it is
positively demonstrated: every targeted leak re-validated on a corpus the author did not
touch, and the only new signal routed OFF the guide. TEACHING: a first-guided guide that
validates should get exactly ONE frozen-guide audit corpus before rotation; that audit is
where you PROVE the guide is complete rather than merely assume it. deck18 is the reference
example of that audit reaching a clean EXIT.

## PIPELINE TEACHING #2 - a perfect record is a REASON to audit harder, not to coast.

deck18 went 6/6 and STILL surfaced a worsened latency tail (max 12,915 -> 14,600; >10k rate
~2.2x) and a genuine truncation->stale-line slippage (vs93 seq20) that the wins concealed.
The auditing discipline that mattered: (a) profile reply-length AND latency, not just wins;
(b) decompose EVERY tail spike to decision-correct-vs-tax; (c) check whether a long reply
actually TERMINATED (finish_reason/terminator presence), because a truncated reply that still
parses to a legal move is invisible in the win column. TEACHING for every frozen-guide audit:
"6/6 hides taxes" is a literal instruction - the record cannot tell you a decision was a
201-second non-terminating loop that fell back to a stale coded line. Only the transcript can.

## CONFIRMATION - EVASION-AS-ATTACK-ORDER and the ground-only RACE line are DURABLE.

Both wave-27 validations HELD on a frozen guide across a fresh pool: Dodger swung 9/9 turns
it could; the one flyer/reach attacker was correctly raced ("no blockers"), 0 illegal
ground-blocks. This is stronger than the wave-27 validation - the fix survives a SECOND corpus
with a different opponent set. Keep the dual-statement evasion treatment and the ground-only
first-pass RACE line as standard; they are now twice-durable, not merely once-effective.

## SHARPENING (NEW) - the trust-line fixed re-derivation, but the TAX MIGRATED, not vanished.

The guide's trust-the-rendered-number line killed the P/T re-derivation spiral (p50/mean flat,
no lord-stack recompute at the top). But the model's over-deliberation ENERGY did not
disappear - it migrated to WHOLE-TURN / MULTI-TURN LETHAL PLANNING on low-stakes decisions
while far ahead (L-18-1, now at 11-14.6k chars, bumping the token ceiling). TEACHING for guide
authors: a directive that removes ONE over-computation trigger does not reduce total
deliberation - the model finds the next-most-plausible thing to over-compute. Fixing a tax at
one decision surface predicts its RE-APPEARANCE at an adjacent surface; watch for migration on
the validation/audit corpus rather than declaring the tax "gone." The right home for the
migrated form is a SKILL/CORE rung ("answer the decision in front of you; commit briefly when
far ahead"), not another per-deck line - the shape is archetype-independent.

## PASS - density, mulligan rung, sixth-face template unchanged.
- Density: the frozen 85-line / 5.9 KB guide needed no trim - every leak section still earned
  its place (each re-validated). A first-guided guide should NOT be trimmed at the audit
  corpus either, when all targeted leaks re-hold. No leak proved dead across two corpora.
- Mulligan (FIDELITY-vs-JUDGMENT): 6 clean keeps again; surface remains UNEXERCISED
  (mono-white trivial keeps). Carry as unexercised through rotation.
- Sixth face (symmetric-effect-asymmetric recursion): N/A (no sweepers/persist). Correctly
  not applied.

## NOTE for synthesis - two cross-seat items leave with deck18, NOT as guide debt.
L-18-1 (planning tax, now token-ceiling-bumping) and N-18e (truncated reply -> stale coded
line fallback) are handed to the cross-seat ledger. Neither is guide-fixable; both appear at a
generic model/harness layer. deck18's rotation does NOT close them - it just stops using
deck18 as their witness. If a SKILL rung for L-18-1 is ever written, deck18 vs146 seq13 /
vs93 seq17 are the cleanest low-stakes-while-ahead witnesses; if N-18e gets a harness fix,
vs93 seq20 is the canonical repro.
