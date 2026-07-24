# deck27 skill.md — proposals for the strategy-writing skill (wave-24)

Seat: Zombies (UB tribal), guided round 5. Single-seat by default; layer-routing enforced (single-seat
items CANNOT add/cut a skill core line). No new headline rung. This corpus CONFIRMS existing rungs and
PROMOTES one wave-23 candidate note (commit-lock vs decode-collapse classification) with a fresh,
harmful, NON-truncated instance — plus one new candidate on snap-first suitability.

## Confirmations (existing rungs, strong worked examples)

- **"A record swing is not evidence a change worked" — CONFIRMED (DOWN direction).** Record fell 4/6 ->
  3/6 while the guide was byte-frozen. The dip is variance/pool: vs109 flipped from a wave-23 dominant
  win to a wave-24 blowout loss on draw/matchup; vs102 stayed a loss but tightened to a near-miss race.
  Attribute per game; do not read the dip as a regression. Symmetric to wave-23's UP-direction case.

- **"Fix the residual at the layer the parser actually commits" — CONFIRMED, and now with a NEW harmful
  snap-committed decision kind (the bottom phase).** The vs137 bottom commit-lock is a snap-committed
  decision (answer-first line 1 wins). The right levers are exactly the two the skill predicts for a
  snap-committed kind: (a) GUIDE — front-load the correct snap heuristic (bottom-phase guidance, shipped);
  (b) PARSER — prefer the model's final coded line on non-truncated replies. Both routed. Reinforces
  layer-first routing for every new snap-committed decision kind the engine adds.

- **"Write the off-case even if the winning corpus never needs it" — off-case STILL unexercised, now 5
  corpora.** The durdle late-stall revision has gone five corpora without its trigger firing (deck27
  attacks every turn; the walled-stall-with-held-attackers state never occurs). Per the skill's own
  low-frequency-off-case note, hold and re-flag; do not cut. This corpus the followed prescription (swing
  every turn while grinding) WON the vs59 adjudication — the off-case remains latent, the prescription
  is doing its job.

## PROMOTED candidate note for synthesis: separate DECODE-COLLAPSE from COMMIT-LOCK (now 2 corpora)

The wave-23 skill.md raised this as a single-seat candidate. This corpus INDEPENDENTLY re-hits it with a
harmful instance of the OTHER member of the pair, so it should promote to a durable classification rung:

- **Decode-collapse** (wave-23, vs137 s12-14): reply is pure degenerate token-repetition, NO coded line
  -> unparsed_reply. Answer-first cannot save it (nothing committed); parser correct to reject (nothing
  to recover). Owner: MODEL/DECODE-sampling (rep_penalty, max-tokens fast-fail, answer-locked retry).
  DID NOT recur this corpus — the answer-locked retry firing 0x is the correct outcome, not a miss.
- **Commit-lock** (wave-22 vs109 s15 [truncated]; wave-24 vs137 seq5 [NON-truncated]): a WRONG coded
  line 1 is emitted, the model self-corrects to the right answer in later prose, but answer-first freezes
  line 1. There IS a coded answer, and it is WRONG. Owner: GUIDE (make the snap right) AND/OR PARSER
  (prefer last coded line). The wave-24 instance sharpens it: the freeze is NOT only a truncation
  artifact — even a fully-completed reply is frozen to line 1, so the parser fix must key on "prefer the
  model's FINAL coded line when the reply terminated naturally," not merely "recover a truncated reply."

**The reviewer test (refined):** does the failed/misplayed reply contain a coded line?
  - No coded line -> decode-collapse (sampling/harness/retry). Answer-locked retry is the right catcher.
  - A WRONG coded line + a later self-correction -> commit-lock (guide + parser-last-line). Distinguish
    truncated (line 1 is the only floor; keep answer-first) from non-truncated (prefer the final line).
Mis-routing either to the other's owner wastes engine work. Promote to a classification rung: 2 corpora,
2 distinct members, both harmful, clear owner split.

## NEW candidate note (single-seat, watch): the answer-first SNAP is unreliable for arithmetic-heavy
decision kinds. The bottom/mulligan decision requires color-counting and mana arithmetic; the model's
snap line 1 was wrong and its DELIBERATION was right (vs137 seq5). Answer-first is calibrated for
decisions where a fast correct instinct exists; for decisions that genuinely require computation
(bottom, X-payment, complex mana), the snap should either (a) be made right by front-loaded guide
heuristics, or (b) yield to the model's deliberated final line via the parser. Candidate skill note:
"when a decision kind requires arithmetic the model cannot do reliably at a glance, do not rely on
answer-first alone — pair it with either guide heuristics or a last-line parser preference." Single-seat
origin (one bottom-phase instance); promote only if a 2nd reviewer independently hits a snap-first
arithmetic misfire.

## No new core-prompt candidate from this seat
Every item is single-seat and/or owns a layer below core (bottom-phase guidance + first-strike caution
= per-deck guide; commit-lock parser + wither-FS annotation = engine/parser; verbosity/latency =
sampling/harness; durdle off-case = unexercised guide; matchup losses = construction). See
general-suggestions.md for routing.
