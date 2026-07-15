# Deck-110 wave-10 — strategy-writing-skill proposals

Each proposal is grounded in a wave-10 deck110 finding. Per the convergence gate, single-seat
items are flagged and do NOT license a core/method line on their own — they are offered to the
synthesizer as candidates that promote only with a 2nd witnessing seat.

## P1 (METHOD, multi-seat candidate) — a CONFIRMED-FIRED representation fix can land perfectly
## AND fail to kill the REASONING artifact it targeted; verify the fix by AGREEMENT-when-read, and
## keep the residual reasoning error OFF the guide when it is non-outcome-affecting.

Grounding: findings V-artifact-tag + RESIDUAL. Wave-9's E3 lever (tag EVERY artifact so a
re-derived count agrees with the summary) SHIPPED cleanly this wave — 177/177 windows now agree,
Glimmervoid/basics untagged, artifact creatures + Darksteel Citadel tagged. Yet the pilot STILL
folds Glimmervoid/basics into a hand-recomputed count (deck131 s6, deck135 s9/10, deck140 s24) and
now mis-types artifact creatures held in HAND (deck133 s17/18). The completer representation did
its job (the surfaced count is complete and agrees) but did not stop the model from recomputing
badly.

Method point: a representation fix's success signal is NOT "the false belief disappeared from the
reasoning" — a model will re-derive over an accurate surface. Score it by (a) the surface is now
complete/agrees (177/177 here) and (b) the defect is OUTCOME-neutralized (no Blast fired expecting
metalcraft-4 at surf<3; no metalcraft-gated pick flipped). When both hold, the loop is CLOSED even
though the reasoning artifact persists — do NOT re-open it with louder guide wording (the
escalate-at-a-wall anti-pattern). This sharpens the wave-7/8 confirmed-fired-representation
terminus: "confirmed fired + obeyed" for a COUNT-agreement fix means "surface agrees AND the class
is outcome-neutral," not "the model stopped mis-reasoning." (This is the shipped-and-obeyed-but-
the-reasoning-still-wanders case — the mirror of shipped-but-inert.) Multi-seat candidate: the
same pattern is the deck133 Thoughtseize-into-empty read-then-disobey from wave-9; promote to the
method if a 2nd seat shows a completer representation that agrees-but-doesn't-silence.

## P2 (METHOD, multi-copy-representation refinement — 2+ waves, this seat) — a per-INSTANCE "no-op"
## marker cannot catch a NET-ZERO shuffle between EQUIVALENT instances; the churn watch's
## representation lever must key on GAIN, not on same-target identity.

Grounding: finding F1. deck109 T8 bounced ONE Cranial Plating between TWO identical 2/4
Ornithopters 4x/turn (net-zero, ~4 mana, in a loss); the `(ALREADY attached to it - this would
change NOTHING)` marker fired only on the current holder, so each move to the OTHER Ornithopter was
UNmarked and looked like a real state change. deck131 T7 did the Skirge->Pest->Skirge A->B->A
version. This is the wave-7/8/9 multi-copy-churn watch RECURRING COSTLY, and it exposes that the
marker's predicate is wrong: "already attached to THIS target" is not the no-value predicate. The
value-neutral predicate is "moving the equipment here would not increase this target's resulting
power above its current carrier's" (equivalent-or-worse carrier) — that catches both the two-Platings
case (wave-9 E2) and the identical-carriers case (this wave) with one rule. Method note for the
representation rung: when a repeatable "move/attach" action is re-offered, the no-op/low-value
marker must be computed from the OUTCOME DELTA (power/board change), never from target-identity;
an identity-keyed marker has a structural blind spot exactly where multiple equivalent
sources/targets exist. Routes to the engine ledger, not a guide line (the pilot has no surfaced
gain signal to obey).

## P3 (DIAGNOSTIC, single-seat, WATCH) — under answer-after-plan, a residual plan/choice mismatch
## appears when the PLAN fixates on an UNLISTED card and never reconciles its conclusion against
## the actual option list.

Grounding: finding F2. deck133 seq6: plan reasons about casting Cranial Plating (not an option),
concludes "hold Galvanic Blast," then emits `CHOICE: 1` = Cast Galvanic Blast (the held card).
This is NOT the head-first intent-collapse the new protocol fixed (head==choice==label here); it is
plan-diverges-from-legality + a reconcile failure at the emit step. Add to the harness-sweep
taxonomy as a distinct sub-class of PLAN/CHOICE MISMATCH specific to the answer-after-plan protocol:
"plan centers on an unlisted action, so the final label is a poorly-mapped fallback that can
contradict the plan's own sub-conclusion about the listed options." Single instance -> route to
the reply-protocol/model layer, never re-word a guide over it, never score it against a guide rule
(the same routing discipline as intent-collapse). WATCH; promote to a reply-protocol refinement
only with a 2nd seat.

## P4 (no method change) — the pause-aware DEMOTION doctrine held cleanly this wave and needs no
## edit: a guardrail whose class was OBSERVED VIOLATED stays at full prominence.

Grounding: finding F1 + FREEZE-CHECK. The equip-churn class was OBSERVED VIOLATED (deck109 T8), so
Rule #1 correctly stays full-prominence and un-demoted; the same-instance re-equip marker class was
OBSERVED OBEYED (31/0) and stays frozen as-is. This is exactly the wave-9 demotion terminus
(demote only once OBSERVED OBEYED; a violated class is not a demotion candidate) applied correctly —
recorded as a clean confirmation, no method change proposed.
