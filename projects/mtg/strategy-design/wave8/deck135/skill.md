# Deck-135 wave-8 — proposals for the strategy-writing skill

Mode this wave: OBEYED-BUT-LOSING (record 0W/4L/2TO, all 4 losses opp at/near 20;
obedience metrics high; losses = construction + slow clock + ONE new model-fixable lever).
Guide revised as a constrained diff off the live deployed guide; frozen lines preserved (see
notes.md freeze-check). Proposals below, ranked by strength of evidence.

## P1 (convergent-with-existing-method, promote into the method) — DISPLACEMENT into the ELSE branch: a deploy-before-value rule can be fully OBEYED while the leak moves to the "nothing to deploy" branch, and a PERMISSION clause is not enough — the else-branch needs its own IMPERATIVE scoped to a countable condition.

deck135's Rule #1 ("deploy a creature before value") was obeyed all wave, no overshoot. The loss
lever moved to its ELSE branch: when NO creature was in the cast list, the pilot Cast nothing and
"held mana for a creature" (verbatim) that it could not cast and had not drawn — freezing instead
of developing its own fixing/engine. vs44: `Into the North` (the deck's blue FIX) was castable at
T5 and T7 and DECLINED four times to "hold mana"; the pilot cast it only at T9, at 5 life, and
died T10. The blue screw was substantially self-inflicted.

The guide DID carry an else-clause — "(or none is castable) do you spend leftover mana on value" —
but as a PERMISSION, and the weak executor's invented instinct ("hold mana for a creature / don't
waste mana on non-threats") overrode it. **A permission does not beat a self-generated shallow
default; the else-branch of a priority rule needs its own IMPERATIVE, front-loaded, scoped to the
countable condition that triggers it, and it must NAME the anti-pattern the executor will
otherwise invent** ("Cast nothing / hold mana for a creature is WRONG when no creature is in your
list"). This is the same shape as wave-7 deck140's "state BOTH branches of a hold as countable
conditions together" (method headline 3) and the wave-5 DISPLACEMENT lesson (a fixed rule pushes
the leak to the adjacent/else seam) — deck135 supplies a clean third witness and the concrete
authoring move: turn the else-permission into an else-ORDER with a scope guard ("this fires ONLY
when the list has no castable creature — a listed creature still wins point 1") so it cannot
invert into "cast value over a creature."

## P2 (completes a wave-7 line) — when attributing a color/mana loss to CONSTRUCTION, check whether the deck's own FIXING was CASTABLE-AND-DECLINED before crediting the decklist.

Wave-7 filed vs44 as "manabase strand, short blue all game." This corpus shows the fix (`Into the
North` -> Snow-Covered Island) was in hand, OFFERED, and declined twice. The construction flag
(blue-thin, 2 Islands) still stands, but a piloting layer sits on top of it. **Extend wave-7's
deck131 rule (separate "source not drawn" = construction from "source in hand but unofferable" =
representation) with a THIRD bucket: "fixing in hand, offered, and DECLINED" = piloting.** Before
routing a mana loss upstream, grep the losing seat's own turns for a castable fixing spell that
was passed — a construction attribution that a legible in-hand-and-declined fix contradicts is
half a piloting bug wearing a decklist costume. (Method note: join the board state before
counting — my raw "paralysis" tally flagged 6 vs110 windows that were all a redundant 2nd
Astrolabe correctly declined; the real signal was 4 vs44 windows where a NON-redundant fixing
spell was declined. The wave-7 "don't score a raw HELD-count without joining the deciding board
fact" rule applied cleanly and cut the count from 13 to 4.)

## P3 (watch, not a promotion) — answer-before-reasoning intent-collapse: a reply whose PLAN unambiguously concludes a DIFFERENT same-window option than the emitted head number is a REPLY-PROTOCOL artifact, not a guide or judgment defect.

vs131 s48: the reply's first char is `4` (Cast nothing); its PLAN then reasons in full to "I must
cast a creature first... I will choose Option 2 as the standard cast." The "number first, then
PLAN" protocol commits the head token BEFORE the model finishes reasoning inside the PLAN, so a
model that reasons itself to a different answer is already committed. Self-corrected next phase
(~1 phase tempo). **Diagnostic rule for the skill: when a decline/pass is recorded but the reply's
PLAN concludes a specific DIFFERENT option FOR THE SAME WINDOW (not a "cast X next turn" future
intent), classify it as reply-protocol intent-collapse and route to the reply-protocol/core layer
— never re-word the guide over it, and never score it against a guide rule.** Distinguish from the
existing PLAN/CHOICE MISMATCH class (there the head int matches a passive option and the reasoning
is genuinely for a different action): here the reasoning and the head number are for the same
window but the head was emitted first and never revised. Single instance -> watch. (If a future
wave shows this recurring, it argues for a core reply-protocol change — put the number AFTER the
PLAN, or re-read the last number in the reply — but that is a core/harness A/B, not a guide item.)

## Non-proposals (freeze / hold, recorded for continuity)
- Rule #1, Rule #2, WHO-IS-BEATDOWN, MULLIGAN, LETHAL CHECK, the board-read anchor, the
  uncastable-Treefolk anchor, and the Force scoping line all held and are FROZEN. No churn.
- Free-reaction-card-cost (Force "free = always fire") — still SINGLE-SEAT (deck135). No 2nd
  free-alt-cost deck in this corpus; cannot promote. Keep the per-deck face-burn redirect line.
- No new core prompt-body sentence proposed (the one cross-cutting item, P3, is reply-protocol,
  layer-routed).
