# Deck-135 wave-13b — general-strategy / core suggestions

## OBSERVATION 1 (synthesis attribution) — FORCE round-3 offer-legality fix VALIDATES cleanly at the seat that owns it; report it CONFIRMED.

deck135 owns the Force-of-Negation seam. Wave-13b confirms the round-3 shipment (a75e2625e):
- **0 standalone "exile a blue card" dead-end actions** anywhere in the corpus (the dead-end is gone from
  the GPT menu).
- **1 legal alt-cast offer** (deck44 s22), correctly gated on a payable exile target, correctly formed
  (`... exile a blue card from hand cost - can target on the stack: Go for the Throat`).
- The offer was **DECLINED CORRECTLY** — the opponent was Go-for-the-Throat-ing its OWN creature, so a
  Force would burn two cards to save the opponent's own creature. Correctly-reasoned, not reflexive.
- **0 Force casts → 0 RESIDUAL-RACE** at this seat (the async response-window race had no answered cast to
  eat). The engine RESOLUTION bug (free-alt-cost Force never counters, ledger #1) therefore got NO new
  data this corpus and remains next-cycle's top engine item — the interface layer (offer legality) is now
  clean; the resolution layer is still unvalidated. Recommend synthesis record round-3 as CONFIRMED in the
  validation table, alongside the standing note that ledger #1 is untouched.

## OBSERVATION 2 (corpus-wide, deck135 provides the SHARPEST fixture) — life-adjudication mis-scores board-winning positions, and the latency cap CONVERTS representation bugs into fallback decisions.

The brief owns the GPU_MEM_UTIL=0.75 latency → 2400s-cap → life-adjudication regime. deck135 supplies the
canonical illustration that the LIFE-only adjudication is not a neutral tiebreak:
- **vs110 (adj1, 9/10, T12) is a "won-board-lost-adjudication" fixture.** deck135 was ONE life behind but
  held an 8/8 trample Treefolk + 1/1 deathtouch-flying Coatl, both attacking, opp at 10 with a lone TAPPED
  5/2 blocker — the heavy favourite to win next turn. The clock cut the game a turn early and the life
  tiebreak recorded a loss. When the loop resumes clean-latency validation, this is the reference case for
  "was the seat ahead for the RIGHT reasons" — here, decisively ahead on BOARD, behind on life.
- **The latency cap is worse than slow: it turns representation bugs into surrendered decisions.** deck109
  s19 (findings Q4 / notes ENGINE #2): a board-line contradiction (stale `[attacking]` + inverted freeze
  annotation on a Treefolk-frozen Reckoner) inflated the model's derivation until it TIMED OUT into a
  heuristic block. Recommend synthesis weigh the engine-batch priority of BOTH the annotation/board-tag
  fixes AND the latency remediation jointly — each amplifies the other.

## OBSERVATION 3 (CORE candidate status update, NOT a promotion) — colored-pays-generic FIRED-then-self-corrected at deck135 this corpus; still parked.

Wave-11 reported the belief did not recur at this seat. Wave-13b: it DID surface in first-draft reasoning
at vs110 s5 (*"I cannot cast Icehide Golem because I have no generic mana available… {W}"*) and
SELF-CORRECTED within the same reply, casting Golem on curve — **no wrong action**. This confirms the
model-UNSTABLE read (fires wrong on some derivations, corrects on others). CORE stays PASS (the body
already states "any color can pay a generic cost"; a 3rd copy is the sanctioned-against escalation). The
promotion trigger is unchanged: a CROSS-SEAT 2nd deck's reviewer finding an ACTUAL offered-payable DECLINE
that cites inability to pay a generic pip with colored mana. deck135 contributes ZERO such declines this
wave (it corrected before acting). Durable fix if it ever promotes = REPRESENTATION (an option-line
affordability breakdown the engine already partly does via offered=payable), never louder core prose.

## OBSERVATION 4 (WATCH, latent, brief's flagged candidate) — attackers name→index reconcile arm.

The model emits mixed `ATTACK: A1, <Name>` declarations (findings Q5); the parser drops the trailing NAME.
Harmless this corpus (every dropped name was an illegal/summoning-sick attacker), but latent-dangerous.
Recommend an attackers name→index reconcile arm mirroring parseChoice's name-echo (match the name to its
A-index; drop explicitly if not a legal attacker). Engine/harness ledger, not guide text. Not a core edit.

## No other general/core proposals.
Everything else routes to the engine ledger (Force resolution; stale-[attacking]/freeze; attackers
name-parse; fetch re-ask), model-side (summoning-sickness over-declaration; colored-pays-generic self-
correct; stale YOUR PLAN echo), or construction (manabase / slow clock — user owns the decklist). The
guide is FROZEN byte-identical.
