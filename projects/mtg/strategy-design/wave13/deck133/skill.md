# deck133 wave-13 — strategy-writing-skill proposals

Corpus: `matchups-20260715-183228` (wave-13b, name-echo protocol live). One WATCH-level proposal,
grounded in the wave's one deciding finding. It sharpens the confirmed-fired-representation /
name-echo terminus; single-seat, so it cannot add or cut a core method line.

## Proposal 1 (WATCH — refines the name-echo-reconcile evaluation rung)

**A name-echo reconcile that trusts the INDEX on conflict is safe only when the echoed card names
an OFFERED option. When the echoed card matches NO option (a card-PRESENCE hallucination), index-
wins does not "resolve to the echo's intent" — it executes an unrelated in-list option at that
index, whose safety is a coin-flip. A reviewer scoring a name-echo protocol must therefore split
echo-index conflicts into two mechanisms with different owners, and must NOT credit "index wins on
conflict" as uniformly safe.**

Wave-11 (deck133 P1) established the two-leak taxonomy for an own-targets annotation: (a) reasoning
disobeys vs (b) number/parse leaks. Wave-13 supplies the analogous split for the name-echo layer:

- **Mechanism A — echo names an OFFERED option ≠ the index.** The reconcile can remap to the echo's
  unique match (or, on genuine ambiguity, index-wins is a defensible tiebreak). This is the case the
  protocol was designed for and it behaves.
- **Mechanism B — echo names a card that is in NO option (hallucinated presence).** There is no
  remap target; index-wins fires; the executed card is whatever sits at that index, unrelated to the
  model's stated intent. deck133 vs140 seq45: echo "Gray Merchant" (not offered) + index 1 →
  executed the guide-forbidden thin-hand Thoughtseize, paying the 2 life that flipped a 1-life
  adjudication loss. Five sibling firings the same corpus (vs110 s31, vs140 s47/s49, vs135 s17,
  vs109 s17) landed on pass/nothing/decline and cost nothing — the harm is purely which index the
  hallucination happened to name.

**Authoring consequence (refines the terminus's evaluation signal):** when scoring a shipped
name-echo protocol, a "100% echo-compliance" statistic and an "index-wins-on-conflict" rule are
BOTH necessary-but-insufficient. The reviewer must additionally count Mechanism-B events
(echo-names-no-option) and their executed-card outcomes, because a hallucinated-absent-card echo
turns index-wins into an uncontrolled in-list cast. The durable fix lives at the HARNESS (fall back
to heuristic when the echo matches no option and the index's card ≠ the echo), NOT at the guide —
the guide's "confirm the card you name appears in the list; else cast the best listed card" was
PRESENT-AND-DISOBEYED, so a louder guide line is the sanctioned-against escalation
(representation/harness-beats-instruction).

Convergence status: **single seat (deck133), 6 firings / 1 deciding, all in one corpus.** Per the
epistemics rule it does not add or cut a core method line; offered as a SHARPENING of the name-echo
evaluation rung (add the "Mechanism-B: echo-names-no-option → count executed-card outcomes"
qualifier). Adopt into the method only if a second seat shows an echo-names-no-option conflict
executing an unintended in-list cast this wave; otherwise WATCH.

## No other skill changes.
The freeze/demotion doctrine, the pause-aware precondition, the bare-N/shown==resolved terminus, the
own-targets stack-targeting representation item (recurred once, non-deciding), and reply-protocol
layer-routing all held and required no revision from deck133's evidence.
