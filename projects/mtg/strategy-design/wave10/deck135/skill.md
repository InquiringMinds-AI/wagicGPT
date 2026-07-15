# Deck-135 wave-10 — strategy-writing-skill proposals

Two proposals, both grounded in wave-10 findings. Neither cuts a core method line; both either add a
verification refinement or reconfirm an existing rung with the strongest exemplar yet. The dominant
deck-135 result this wave is FREEZE (byte-identical guide), so these are method-verification refinements,
not new authoring headlines.

## Proposal 1 (grounded in findings Q1) — a REPLY-PROTOCOL-SHIFT fix is verified by the STRUCTURAL head signal, and the reversal-mismatch heuristic must be DISTRUSTED (read the flagged replies).

Extends the existing "verify a shipped representation/engine fix FIRED" rung to the reply-protocol
layer, which wave-9 promoted to firm doctrine (never re-word a guide over a reply-protocol defect; the
durable fix is the A/B). Wave-10 is the first corpus where that A/B effectively SHIPPED (head-first ->
answer-after-plan), so the method needs a way to verify the prior protocol-artifact class is DEAD.

Adopt: when a reply-protocol change ships, the verification signal is STRUCTURAL and lives in the head,
not in a fuzzy plan-vs-choice matcher.
- The head-first intent-collapse class is confirmed dead by `tools/intent-collapse-metric.py` showing
  **all records now non-numeric-headed** (every reply opens `PLAN:`, so there is no head token to lock)
  AND **last-bare-int-differs-from-head = 0**. That pair is the answer-last signature; it cannot be
  faked by a still-broken protocol (deck135 seat: 246/246 non-numeric, 0 differ).
- **DISTRUST the metric's "reversal-language + concluded-option != recorded choice" count** — it is a
  fuzzy heuristic that reports 0-indexed guesses and matches intermediate plan mentions, so it throws
  index-offset FALSE POSITIVES (deck135 seat: 30 flags, 0 genuine on manual read; every recorded CHOICE
  equalled the plan's FINAL conclusion — e.g. deck109 s18 reverses TO Icehide Golem and picks Golem).
  The rule: a reversal-mismatch flag is a POINTER TO READ THE REPLY, never a defect count. This is the
  reply-protocol twin of the wave-7 "read the REPLY of the lone counterexample before re-opening the
  representation" — a fuzzy detector's hit is a lead, its total is not a metric.
Single-reviewer caveat: this is a corpus-wide property (the shared protocol), so synthesis can confirm
it at every seat trivially; I flag it as a candidate the whole pool should carry, not a deck135-only note.

## Proposal 2 (grounded in findings Q2) — RECONFIRM "chosen/cast happened is not EFFECT happened" with the strongest exemplar yet: a FREE-ALT-COST reaction that pays its cost and no-ops the effect.

Reconfirms, does not add, the wave-6 resolved-vs-asserted rung (133 P1) and the Step-0 end-to-end audit
("cast happened is not effect happened; follow the card to its effect RESOLVING"). Force of Negation's
free alternative cost gave the corpus its cleanest instance: the option is OFFERED, the pilot CHOOSES it,
the exile-a-blue-card cost IS PAID — and the targeted noncreature spell RESOLVES ANYWAY (deck131 Downsize
s40 / Elixir s41 / Cyclonic Rift s51; deck140 Rakdos's Return s58, all `stack -> graveyard/battlefield`
= resolved, never countered; Force to graveyard, blue cards bled). A reviewer who scored Force by "was
it offered and chosen" would have credited a working counter seam; only following the TARGET spell to
`stack -> graveyard` as a COUNTER (vs a normal resolution) catches it.

Authoring test to carry: to verify a COUNTER/reaction fired, do not stop at offered-and-chosen; confirm
the TARGET spell's fate in EVENTS — a countered spell and a resolved spell both leave the stack, so the
distinguishing read is whether the target's EFFECT landed (life change, zone change, power reduction),
not that the stack emptied. A free-alt-cost reaction adds a trap: it can pay its own visible cost
(exiled card) while the counter silently no-ops, so a "cost was paid" read is not "effect happened"
either. (This is engine-ledgered, not a guide edit — the point is the VERIFICATION method.)

## Not proposed
- No change to the demotion terminus, the false-belief-per-surface rule, or the else-branch develop-order
  rung — all held this wave (the develop-branch rewrite VALIDATED; the false belief recurred but routes
  to core, not a method change). No single-seat method promotion; both proposals above are corpus-wide or
  reconfirmations, per the epistemics gate.
