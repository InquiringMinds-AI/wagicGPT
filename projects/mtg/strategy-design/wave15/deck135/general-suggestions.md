# General suggestions (cross-seat / synthesis) — deck135 wave-15

Routed UP to synthesis; not deck135-guide changes. Ordered by value.

## 1. HEADLINE — Force of Negation resolution is CONFIRMED WORKING (first ever), but the free alt-cast has a NO-UNTAPPED-SOURCES execution bug. Both belong in the cross-seat Force ledger.

Two findings from the seat that owns the Force seam, both first-of-their-kind:

- **CONFIRMED FIXED — Force resolution counters for real.** deck140 seq18 (free exile alt-cast) and
  seq30 (hardcast) both drove their target `stack -> EXILE` (Black Sun's Zenith board wipe; Lightmine
  Field). This is the first end-to-end validation of ENGINE #1 (through wave-13 there were 0 casts,
  so the resolution layer had never been exercised live). **The alt-cast counter (seq18) directly won
  the vs140 game.** Every other seat with a Force in its list should re-verify the same on its own
  corpus and the ledger item ENGINE #1 should move from "resolution unvalidated" to "validated;
  watch for regression."
- **NEW ENGINE BUG (owned #4, reframed) — the free alt-cast silently fails to execute when the caster
  has NO untapped mana sources.** 3/3 answered alt-casts with "(no untapped sources)" produced no
  execution (deck21 s27/s29, deck110 s4); the 1 alt-cast that executed had `{w}` available (seq18).
  NOT latency-driven (seq30 executed at 93 s). The alt-cost needs zero mana, so gating execution on
  untapped sources is a bug — and it disables Force precisely in its intended window (tapped out on
  the opponent's turn). This should be a cross-seat engine-batch candidate; ANY seat whose Force is
  offered while fully tapped will hit it. See notes.md ENGINE #1 for the fix direction.

## 2. PROMOTE the colored-pays-generic / trust-the-option-list CORE candidate — the wave-13 promotion trigger FIRED this corpus.

Wave-13 parked this candidate and named the promotion trigger explicitly: "a cross-seat ACTUAL
offered-payable creature decline." **deck110 seq3 IS that instance** — an OFFERED `Cast Icehide Golem
with its alternative cost {1}` was DECLINED because the model hallucinated a `{1}{g}` requirement
("I lack green mana… I cannot cast the Golem"), casting Astrolabe instead, in a game it then lost.
This is no longer a fired-then-self-corrected belief (wave-13's deck110 s5) — it is a completed wrong
action. The core prompt already states "any color can pay a generic cost," but the failure here is
one step upstream: the model DISTRUSTS the authoritative offered-option list and invents a color
requirement to override it. Synthesis should weigh a CORE reinforcement of "an option shown in your
cast list is payable AS SHOWN — never re-derive its cost to decline it" (single-seat items cannot
edit core lines, hence this routes up). Confirm with a cross-seat grep for other offered-payable
declines before editing core.

## 3. Force DISCIPLINE is loosening model-wide — watch the reflexive-cast swing.

deck135 went from wave-13's perfect Force discipline (1 offer, correctly declined) to casting all 6
offers this corpus. It got the two WIPES right but over-Forced a Cranial Plating on an empty board
(deck110 s4) and Force-on-Volcanic-Hammer with a wrong damage premise (deck21 s27, thought 3 kills a
6/6). No face-burn/discard Force offers appeared this corpus, so the guide's decline spine was not
tested. If other Force-carrying seats show the same reflexive-cast swing against marginal noncreature
spells, it is a model-behavior drift worth a synthesis note — the guides already scope Force
correctly, so this is about the model over-applying, not under-teaching.

## 4. Model rules-gap cluster (damage-vs-toughness) recurs across seats.

deck21 s27 (thought Volcanic Hammer's 3 damage kills a 6/6) is the same damage-vs-toughness gap
flagged in deck110's ledger. It is model-side and cross-seat; not guide-fixable per deck. Track it in
the cross-seat model-experiments bucket, not in any single guide.

## 5. Latency-adjudication continues to dominate outcomes (corpus-wide, brief owns).

Seat latency med 12.0 s / p90 120.6 s / max 196.9 s (30 windows >60 s, 20 >120 s). 11 of 12 unparsed
fallbacks were pure timeouts. The vs17 WIN was an adjudication in deck135's favor — the mirror of
wave-13's vs110 robbery. As long as the ~0.75-preemption regime holds, adjudicated positions must be
read as board states, not results; the win/loss column is noisier than the play.
