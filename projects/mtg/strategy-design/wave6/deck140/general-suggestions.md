# Deck-140 wave-6 — cross-layer suggestions (routing IS the deliverable)

## CORE PROMPT — one candidate, argued to stay PER-DECK (no core edit)
- **The burn-prior mis-fires on scarce X-burst finishers.** The core says *"direct damage and burn are
  part of your CLOCK… send them at the opponent's face rather than saving them."* deck140's Rakdos's
  Return (X to face) pattern-matches "burn" and the prior drove 6/7 early chips (findings Finding 1b).
  BUT this is **per-deck by construction** — the prior is correct for real burn decks (deck109's Lava
  Spike lines want it); only a deck whose "burn" is a scarce one-shot X-kill wants the opposite.
  Single seat cannot cut a core line. **Route to per-deck** (the wave-6 guide overrides it by name).
  Convergence-watch: revisit ONLY if a second deck runs a scarce X-reach finisher the burn-prior
  chips away. Do not touch core on this evidence.

## ENGINE / REPRESENTATION
- **PLAN/CHOICE MISMATCH (intent collapse), 2 instances — first-option bias lands on Rakdos's Return.**
  vs135 t5 seq4 (PLAN: "cast Pyroclasm, hold Rakdos" → chose option 1 = Rakdos, X=1) and vs110 seq33
  (PLAN: "casting Pyroclasm is a waste, I must not cast it" → chose Pyroclasm). Both are correct-plan-
  betrayed-at-the-number, the deck44 class. Rakdos's Return sat at option 1 in ~11 of its offered
  windows, so the first-option prior repeatedly lands on the exact card the guide says to hold.
  Weakly guide-mitigable (the wave-6 guide adds an anti-first-option hold line); the durable fix is
  representation/reply-protocol. **File as a repro, not a guide problem.** No new engine bug beyond the
  known intent-collapse class.
- **Un-veto confirmed working — no regression.** 21 sweeper/edict picks resolved, ~0 validation
  failures; the hint-veto removal did exactly what it should. The new WASTE it exposed (sweepers into
  artifact boards) is a MODEL false-belief, fixed in the guide (findings Finding 2), not an engine bug.
  Do not re-add the veto — it would re-block the correct sweeper picks the fix enabled.

## HARNESS
- **RESOLVED this run: `options_text` is now emitted per record** (array of the numbered options). The
  wave-5 schema ask is fulfilled — offered-vs-taken and X tables are now a clean field read, no
  prompt-scrape. Thank you. Still open: per-kind decision counts + fallback count in `gameend`, and a
  `realized` magnitude field on X asks (would make the X-regression check a one-liner).
- **Stalemate detector + cap-by-decisions (open from wave 3/4/5, still costing analysis).** Two of
  three deck140 timeouts this run were latency-starved, not stalls (vs133: 94 records, spikes 41.6s;
  vs135: 19 records, one 63.7s spike). Control mirrors generate 2-4× the decisions of aggro games, so a
  fixed 2400s wall-clock cap times them out regardless of play. Ask, unchanged: end as `draw`/
  `end_reason=stall` after N (~8) turns with no life/board/hand change, AND cap by decisions consumed
  (cap = N_decisions × budget) OR pin decision-quality corpora at `-j <= 3`. deck140 is a control deck
  and eats the timeout column for this structural reason.
- **`-j` latency saturation is severe this run** — spikes to 60.9s / 61.7s (vs110, vs109) vs a ~4-5s
  solo baseline. This binds outcomes before any model-quality question; read `latency_ms` before
  attributing a timeout to play.

## DECK BUILDER — reach starvation is the residual (unchanged from wave 4/5, now well-bounded)
With the X fix and the guide's holds, deck140 still can't close two games because it drew **no castable
clock**: vs44 dealt **0 damage across 20 turns** (opp at 20 the whole game) into a flyer-tempo +
Counterspell deck; vs109 swept ~8 times while bleeding 20→0 with no reach. The two wins/near-wins both
required the reach to actually arrive (vs131 Blightsteel t32-34; vs133 ahead). Win-condition inventory:
2× Blightsteel {12} (lands too late without ramp), 4× Staff of Nin (1 dmg/turn — a 14-20 turn clock),
2× Rakdos's Return (the only burst, now conserved). The 14-sweeper density is proven right; the deck
needs **teeth, not text**: a 3rd/4th X-reach finisher, a cheaper repeatable drain to complement Staff,
or a {4}-{6} recursive attacker that survives its own sweepers (Blightsteel is the template but {12}
with zero ramp arrives too late). Any of these moves the clock more than a guide line can. Expect
deck140 to stay the pool's slowest closer until it gets faster reach — do not read that record as a
piloting failure.
