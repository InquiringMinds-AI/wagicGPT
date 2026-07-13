# Proposed revisions to the strategy-writing skill (from deck109 wave-5)

**Net: essentially no changes proposed — one small convergence note offered below.**

Writing this revision exercised the skill's wave-loop machinery and it held up cleanly:

- **The stopgap lifecycle worked as designed and is validated, not to be changed.** The skill's
  triple discipline (line, defect+citation, removal condition) let me confidently DELETE the guide's
  output-format block this wave: the corpus showed 0 desyncs, so the defect it patched (reply-parser
  mis-grab) is fixed, so the stopgap is deleted. That is exactly the "wave-N+1 author knows which
  stopgaps are still needed" procedure (check the new corpus for the defect) firing correctly. Keep
  it verbatim.

- **The negative-framing ruling correctly PREDICTED this wave's leak.** deck109's terminal "Do not
  block or play a control game - race" is a tail-placed DON'T targeting a decision SEAM (blocking),
  and it did not fire once — the model reflexively blocked 4 times at high life. This is precisely
  the skill's "silence at the blocker seam is block-by-default" + "a racing deck needs a positive
  per-item blocker floor, not a terminal DON'T." The skill already teaches the fix; the corpus is a
  confirmation, not a gap.

**One small convergence note (offered, low-confidence, single-seat):** the skill's blocker-seam
guidance in Step 2 covers reflexive blocking well ("a racing deck that blocks is spending its
clock"), but the failure here was specifically that a NEGATIVE terminal seam-rule looked like it was
"covering" the blocker seam while contributing nothing. Consider adding, to the Step-5 negative-
framing ruling or the cross-seam-leakage audit, a one-line flag: *a bare DON'T aimed at a decision
SEAM (blocking, attacking, mulligan) is the single weakest placement — it is both negative AND, if
terminal, out of position; a seam always needs its own positively-framed floor at a seam-labeled
block, never a tail-end prohibition.* This is implied by the existing per-seam-floor rule and the
"cross-seam leakage" pitfall; it may not earn its own line. Defer to the synthesis agent on whether
it is additive or redundant.
