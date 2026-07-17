# Deck-110 wave-18 — skill deltas

**None to the guide-writing methodology.** The wave-17 strategy-writing skill held cleanly again; every
move it prescribes was exercised and none broke. One small METHOD reinforcement worth carrying forward
(not a rule change).

- **Layer routing worked, and paid off as a discriminator.** This wave produced a NEW failure class (the
  block-echo name-format parse failure, deck21 s18) that *looked* decision-adjacent — it flipped a
  winnable game — but is a HARNESS matcher gap, not a strategy defect. The skill's "unit = the DECISION
  with a seq repro, then ROUTE by layer" discipline is exactly what kept it out of the guide: the model's
  block was legal and correctly-shaped; only the matcher failed. Without layer-routing the temptation
  would be to bolt a "write blocks as B#:A#" line onto a strategy guide — crossing the strategy/protocol
  layer, redundant with the prompt's own example, and wrong-scoped (cross-deck harness bug, not deck110
  strategy). Froze the guide (11th), filed the item to the engine ledger.

- **The canary-by-absence move generalized cleanly a second time.** Last wave closed E6 by proposing the
  suppress-the-no-op lever; this wave CONFIRMED it by a zero-count grep ("Cast Card Normally" = 0
  corpus-wide). "Confirm a shipped representation fix by ABSENCE of the old label, not by re-reasoning"
  is now a proven two-wave pattern (E6 close + confirm; E2 validated-by-absence x4). Already implicit in
  the skill; no text change.

- **Position-partition for rambles.** The 4 long unparsed rambles all fell in the 3 LOST games and all
  in desperate (2 life) or confused (post-combat / phase-ambiguous) positions, while the 3 WINS carried
  at most 1 non-biting fallback. Partitioning ramble fallbacks by POSITION (won/comfortable vs
  losing/confused) before attributing them to a decision defect is a cheap guard against reading a
  giant-reply overthink as a strategy regression. Sibling to deck35's 0-attacker durdle instrument;
  worth keeping as a reviewer habit, not a new skill rule.

Net: no edits to the skill text. The skill's existing instruments (layer routing, seq-repro unit,
validated-by-absence, canary construction) covered a wave that contained a genuinely new class without
strain.
