# deck137 skill proposal (wave 27)

## PROPOSAL 1 -- RESOLUTION-VERIFICATION for deferred-action classes: never certify a defer class "harmless" from the DECISIONS AROUND it; verify the DEFERRED ACTION actually RESOLVED.

This is the durable methodology lesson of wave 27, and it overturns a verdict that stood for FOUR
corpora. The convoke tap-payment defers were repeatedly certified "harmless by-design" because every
audit asked "did deck137 still ATTACK on defer turns?" (yes) -- a check on the decisions ADJACENT to
the defer. Wave 27 asked the different question -- "did the CAST the defer was paying for actually
RESOLVE?" -- and the answer is mostly NO (March of the Multitudes 0/17 resolutions; Venerated Loxodon
2/23). The deck's two payoff spells were dead the whole time, hidden behind a defer that looked benign
because the surrounding turns proceeded normally.

**The rung:** when a decision is DEFERRED to the heuristic (kind=defer / deferred_to_heuristic), the
defer record itself carries no evidence of success -- empty prompt/reply, latency -1. To judge the
class, trace the DOWNSTREAM game-state effect the deferred step was supposed to produce:
- a deferred CAST payment -> did the card leave hand and enter the battlefield / graveyard? did its
  tokens/effects appear?
- a deferred trigger ordering -> did the triggers resolve in a legal order?
- a deferred mana tap -> did the intended spell actually resolve?
Ground the verdict in the RESULT event, not the adjacency. "Harmless by-design" is a claim about
outcomes and must be proven against outcomes. A defer whose action silently fails is the WORST case:
no fallback log fires, no unparsed reply, the game just quietly proceeds with the play undone.

**Corollary for the guide layer:** an instruction of the form "just pick option X, do not recompute /
verify" is only safe if option X reliably RESOLVES. deck137's guide said "when convoke is offered you
CAN afford it -- just pick it" -- which, once the offer became unreliable, both steered the model into
the broken path and drove a re-pick loop that wasted whole turns. A guide directive that suppresses
verification inherits the reliability of the thing it tells the model to trust; when that thing is a
deferred engine step, the directive is only as good as the engine.

## PROPOSAL 2 -- "prefer the reliable variant when the engine offers two paths to the same result" is a general guide pattern (route around a known-flaky path when a sound one exists).

Wave 27 shipped a concrete instance: for convoke cards the engine offers BOTH a plain full-mana cast
and a convoke cast; the plain cast resolves, the convoke cast (currently) does not. The guide now says
PREFER the plain cast when mana covers it, convoke only when short. This is a guide-legitimate move --
it changes model BEHAVIOR toward a play that actually happens -- and is distinct from a clamp/paper-over
(it does not fake a result or cap anything; it selects the working option among offered legal options).
Generalizes: whenever a deck's guide steers toward a mechanic that has a known engine-side reliability
gap AND the engine offers an equivalent reliable alternative, the guide should name the reliable path
as the default and reserve the flaky one for when it is the only option. Keep this SEPARATE from the
engine ledger item -- the guide edit recovers the cases where an alternative exists; the engine fix is
still required for the cases where convoke is the ONLY way to make the play (early tempo casts).

## Reconfirmed doctrine (no new rung):
- The wave-25/26 ceiling clause + "trust the offered set both ways" both HELD this corpus: the
  B-list-authoritative clause fired correctly (vs131 s27, trusting the offered `#2` untapped copy),
  and the residual generic-seam spirals landed correct except under the (now-fixed) representation
  ambiguity. Wave-26 PROPOSAL 1(e) -- "a wrong commit under the ceiling is a FACT-surface gap, fix the
  representation" -- was VINDICATED: the d1 instance-handle fix removed the ambiguity and the breach
  did not recur.
- b2 last-answer-wins remains a spiral safety net; the c4 parser's first-legal-pair recovery caught
  both illegal-block-shape replies this corpus (vs18 s58, vs22 s39) without dropping a legitimate
  answer.
