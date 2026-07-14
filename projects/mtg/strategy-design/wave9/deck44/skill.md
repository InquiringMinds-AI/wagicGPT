# Skill-revision proposals — deck44 wave 9 (FINAL wave before pause)

Frozen-spine seat, 6th wave running. One additive METHOD refinement; the rest confirm existing
method. Per the epistemics rule, single-seat evidence cannot cut method — proposals are additive.

## P1 (ADD to method) — an OWN-TARGETS-ONLY representation annotation collapses the reasoning tax at a "no useful target" seam; verify it by 0 mis-casts AND shorter plans, not by win column

**Observation.** The wave-9 binary shipped an own-targets annotation ("the only legal targets are
YOUR OWN right now") on a targeted-removal option whose only legal targets are the pilot's own
permanents. In deck44 it fired 47× (GFTT vs affinity/snow/izzet where the opponent had no nonartifact
creature). Two measurable effects: (a) **0/47 self-target casts** — the pilot never destroyed its own
creature; (b) the **reasoning tax dropped** — median plan length at the annotated decline windows was
519 chars vs 871 at the un-annotated "no target" windows, and the confused self-negotiation ("X is an
artifact? No, X is a creature... Wait...") appeared ONLY where the annotation was absent.

**Proposed method line (extends the confirmed-fired-representation DEMOTION terminus + its
shape-specific verification signals).** This is a FOURTH representation-fix shape to add to the
terminus (alongside weak-marker / absent-option / re-anchor-to-count): the **ambiguous-target
annotation** — the engine names WHOSE permanents are the only legal targets on an option the executor
would otherwise have to reason about. Its shape-specific verification signals are (1) the mis-cast
count -> 0 (did the pilot ever take the option against its own side?) and (2) a **reasoning-tax
drop** — SHORTER, cleaner plans at the annotated windows, measured against the un-annotated windows of
the same decision class. The reasoning-tax drop is a new, useful representation-fired signal in its
own right: an annotation can fire, be obeyed, AND still be worth measuring by whether it shortened the
executor's derivation, because a shorter derivation is fewer places for a weak model to talk itself
into the wrong branch.

**Demotion caveat (per-belief granularity, reconfirmed).** Do NOT demote the guide stopgap the
annotation replaces UNLESS the annotation's surface covers EVERY case the stopgap covered. Here the
annotation only fires when the pilot HAS its own creatures as legal targets; it cannot fire when the
option has ZERO legal targets (pilot holds no creature, opponent holds none), where the bare option
still renders and the executor still gets confused. So the stopgap is PARTIALLY double-covered -> keep
it, document the residual edge, finish the demotion only once the zero-target case is also
represented. (The wave-8 per-belief demotion rule, applied to a representation whose surface is
conditional.)

## P2 (ADD to method) — a counter/removal PRIORITY line that says "you have no other answer" must carry the WHY as a flat imperative, or the executor inverts it into "slow, ignore it"

**Observation.** deck44's counter category #2 ("An ARTIFACT creature or equipment you cannot Go for
the Throat — Master of Etherium, Etched Champion, Cranial Plating, Steel Overseer") named the right
targets but stated the priority as a category, not a reason. In the vs110 LOSS the executor read
"an artifact you cannot Go for the Throat," completed it with its own instinct — "can't remove it ->
but it's slow -> deal with it later" — and let Steel Overseer (pumps the whole board every turn)
resolve, THEN spent the counter on cheap face burn. The line's own intent (it's on the list BECAUSE
you can't answer it later) was the exact opposite of what the executor inferred.

**Proposed method line (extends the false-belief-bridge kill + "name the anti-pattern the executor
will otherwise invent," deck135 P1 wave-8).** When a priority entry's justification is "this is the
one you have NO other answer for," the executor's default instinct ("un-removable + slow = ignorable")
is the precise inversion of the rule. State the WHY as a flat imperative INSIDE the entry ("a counter
is your ONLY answer — counter ON SIGHT, there is no 'later'") and, for the specific card the executor
most under-weights, attach the growth fact that refutes "slow" (Steel Overseer pumps the whole board
+1/+1 every turn). This is the same authoring move as else-PERMISSION -> else-ORDER-with-scope-guard,
applied to a priority-LIST entry whose implicit rationale the executor inverts.

**Companion (value-floor completeness).** A let-resolve value floor must NAME the cheap-face-burn
class explicitly (Galvanic Blast, Searing Spear, Lightning Bolt, whiffing Cruel Edict) — an executor
that sees damage-to-me and does not find it in the let-resolve list defaults to countering it, even at
high life where taking it and racing is correct. The floor's coverage gap is where the counter leaks.

## P3 (CONFIRM, no change) — plan-binding line CLOSED the plan/choice-mismatch class again, largest sample yet
0 head/choice mismatches over ~18 counter windows + 47 GFTT windows. The wave-8 P2 closed watch stays
closed; recorded so the synthesis agent can retire it unless a second seat reopens it.

## No method was cut. No per-deck rule content proposed for the skill (Appendix ban respected).
