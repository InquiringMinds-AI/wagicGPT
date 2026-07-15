# Strategy-writing-skill proposals — deck44 wave 10

Two proposals, both single-seat and flagged as such (they cannot cut a core method line). Both are
DIAGNOSTIC refinements to the Step-2 harness-defect sweep, not guide-authoring rules — they sharpen
how a reviewer classifies a recorded mis-cast under the answer-after-plan protocol.

## PROPOSAL 1 (single-seat; DIAGNOSTIC add to the harness-defect sweep) — a recorded self/mis-cast under the token cap can be a TRUNCATION-PARSE artifact, not an annotation-ignore. Read the reply's TAIL before scoring it against a representation fix.

Grounding: deck44 vs140 s68. The wave-9 own-targets annotation ("the only legal targets are YOUR
OWN right now") was obeyed 35/36 cleanly, but ONE window recorded a GFTT self-cast (`choice=1`,
`fallback=None`). Reading the reply shows it is NOT a read-then-disobey: the model spiralled on an
unrelated infect-rules problem (Blightsteel Colossus, 2000+ chars), hit the 2048-token cap, and
NEVER emitted a `CHOICE:` line — while the derivation EXPLICITLY obeyed the annotation
("Go for the Throat: The only legal targets are YOUR OWN right now. **So no.**"). The parser then
grabbed a stray `1` from the reply body ("1. Attack", "1. Cast Go for the Throat") and recorded it.

The methodological point specific to the ANSWER-AFTER-PLAN protocol: because the answer token is
emitted LAST, a reply that runs out of budget mid-reasoning has NO answer at all, and the recorded
`choice` is then whatever the parser scavenges — which can be a valid-looking option index that
CONTRADICTS the reasoning. This is a NEW failure surface the head-first protocol did not have (there
the head was emitted first, so truncation lost the reasoning, not the answer). A reviewer scoring an
annotation/representation fix by "mis-cast count -> 0" must therefore FIRST check, for every recorded
mis-cast: did the reply reach a `CHOICE:` line? A truncated reply with a scavenged digit is a HARNESS
defect (the 2048-cap class), not evidence the annotation was ignored — do not let it inflate the
mis-cast count or re-open the representation. (The reasoning-tax-drop signal from wave 8 already
points the same way: an annotation that keeps plans SHORT is also insurance against this — the
windows that blew the cap were the ones where the model spiralled on an UNRELATED unanswerable
problem, not the annotated decision.)

Status: SINGLE SEAT. Propose as a one-line addition to the Step-2 ENGINE-NO-OP / truncation bullet,
NOT a core method headline. Promote only if a second seat shows a scavenged-digit mis-cast scored as
a representation failure.

## PROPOSAL 2 (single-seat; DIAGNOSTIC) — a representation can fire INCORRECTLY (a mis-annotation) and be robustly OVERRIDDEN; that is an engine-ledger item, not a validation of the annotation, and not a licence to add compensating guide text.

Grounding: deck44 this corpus, every counter window (vs110 s7/s12, vs140 s24/s29/s69, vs109 s17).
The Counterspell/Arcane Denial option rendered `- NO legal target right now` even when a spell WAS
on the stack and the SAME option line separately listed `can target on the stack: <name>` — a
self-contradictory mis-annotation. The pilot IGNORED the false "NO legal target" clause every time
(trusting the ON THE STACK section + the "can target" clause) and countered correctly (0 misplays).

The method note: the wave-6+ terminus teaches "verify a representation FIRED before crediting it."
The mirror case is a representation that fires but fires WRONG. When the pilot correctly OVERRIDES a
mis-firing annotation, the reviewer must NOT (a) score the annotation as validated (it is buggy), nor
(b) reach for a guide line to compensate (the pilot is already handling it via the reliable channel).
The correct routing is engine-ledger ONLY (fix the mis-annotation), with the guide left frozen —
adding "ignore the NO-legal-target text on counters" would be exactly the restatement-of-a-transient-
engine-artifact the skill bans (cf. the stale one-land-at-a-time caution). A mis-firing representation
that the pilot robustly overrides is a LATENT-risk engine item (a weaker moment could obey the wrong
text), tracked with a repro, not prompt work.

Status: SINGLE SEAT. Not a core headline. Consistent with existing layer-routing doctrine; offered
as a named case the synthesizer may fold into the "verify a representation fired" terminus as its
fired-but-WRONG sibling.

## No core method headline from this seat.
deck44 returned a clean FREEZE with the wave-9 tightening validated. Nothing here rises to a 2+-seat
method change; both proposals are diagnostic refinements the synthesizer should weigh against other
seats before adopting.
