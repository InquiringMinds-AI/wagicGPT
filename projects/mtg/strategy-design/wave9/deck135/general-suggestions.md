# Deck-135 wave-9 — core-prompt / general-layer proposals (layer-routed)

Layer routing per the skill: engine items -> notes.md (never prompt text); model/reply-protocol
items -> here as OBSERVATIONS with the owning layer named; guide items -> strategy.txt.

## No core PROMPT-BODY edit proposed (PASS continues).

deck135 returns no core-prompt-body edit. Both guide changes this wave (develop-branch option-list
gate + fetch-crack shape; Force discard exclusion) are DECK-SPECIFIC — they name this deck's cards
(Boreal Druid, Into the North, Snow-Covered Forest/Island, Arcum's Astrolabe, Inquisition) — and
live in strategy.txt. Two OBSERVATIONS below carry a general flavor and are recorded for the
synthesis agent to cross-check against other seats; neither is a single-seat core cut.

## OBSERVATION 1 (general false-belief candidate; owning layer = core prompt IF it converges) — "colored mana can't pay a generic cost" and "offered != payable."

vs140 s24: the engine OFFERED "Cast Arcum's Astrolabe with its alternative cost {1}" and the pilot
Cast nothing, reasoning "I have no generic mana (my only land is tapped for {G})" — a FALSE belief
that {G} cannot pay a {1} generic cost, compounding a violation of the offered=payable contract
(if the engine lists a cast, it is affordable). This is the same FAMILY as the core's existing
"Mana in your pool: (none) read as I cannot cast" correction, a different instance. It is guide-
patched locally this wave (strategy.txt point 3b names Astrolabe {1} paid by green). **Promotion
trigger: if a SECOND deck shows a pilot declining an OFFERED cast on a "colored can't pay generic"
or "I can't afford the listed option" belief, that argues for a core line — "any spell shown in
your options is payable right now; your colored mana pays generic costs." One seat cannot cut
core.** Recorded for the synthesis agent to grep other seats' declines of offered casts.

## OBSERVATION 2 (reply-protocol; owning layer = the code-appended reply protocol, NOT the prompt) — answer-before-reasoning intent-collapse, now 2nd instance across two waves.

vs131 s14: reply head `4` (Cast nothing) while the PLAN self-corrects in text ("wait, I *can* cast
Ice-Fang Coatl!"). Same class as wave-8 vs131 s48 (self-corrected s18, no game cost). The
number-first protocol forces the choice to commit before the model reasons in the PLAN.

- NOT a prompt-body candidate and NOT a guide candidate — a property of the code-appended reply
  protocol. The SECOND confirmed instance discharges the wave-8 "single datum, watch" hold: the
  armed A/B is now DUE (run it, don't keep watching).
- Candidate protocol experiments (symmetric-core A/B, judge by decision-quality not win-rate):
  (a) accept the LAST bare number in the reply rather than the first; (b) move the number to AFTER
  the PLAN ("PLAN: ...\nCHOICE: N"); (c) reconcile — if the reply text ends by naming a specific
  option index different from the head, prefer the later one. The metric: does the recorded choice
  match the reply's OWN concluded option? vs131 s14 is a clean labeled positive to score against.

## A/B note (already folded by synthesis, no action)
The `hand -> stack` cast-happened anchor removal is live (post-A/B core); deck135 shows no
same-name double-cast and no cast-nothing regression tied to it (Rule #1 point 1 obeyed, no
phantom-board). Nothing for deck135 to add.
