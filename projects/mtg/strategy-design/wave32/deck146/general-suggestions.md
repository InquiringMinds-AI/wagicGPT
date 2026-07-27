# deck146 -> core (general prompt) suggestions, wave 32

## VERDICT: **PASS** (27th consecutive)

Nothing observed at this seat this corpus routes to the general system prompt. The reply protocol,
the narration/board/plan assembly, the decline-ordering and the bundled-combat contract all
behaved. Zero fallbacks in 153 model calls; zero parser contradictions; reply lengths on the
campaign p50 band; latency tail improved. The i13 card-text addition (a per-option render change,
not core) landed with no measurable cost.

Everything this seat found is layered elsewhere and is routed there:

- **N-146j** (Vanishing Verse cannot target artifacts or lands -- `[-multicolor]` compiles to an
  ARTIFACT + LAND colour-bit exclusion) -> engine, notes.md, with source lines and a repro.
- **N-146k** (`Mana available: 0 total (no untapped sources)` renders in the pre-game mulligan
  prompt, where no battlefield exists) -> render/harness, notes.md. **Deliberately NOT routed
  here**: it is a per-seam suppression in the situation-block assembler, not a core-prompt
  instruction, and the fix is to stop emitting a meaningless line rather than to explain it in
  prose. A core-prompt sentence saying "ignore the mana line during mulligans" would be prose
  arguing with a contradicting surface -- the losing side of the perception-vs-strategy routing.
- **N-146l** (the pilot's poison-counter total is never rendered anywhere) -> render, notes.md.
  Same reasoning: a missing STATE fact needs a state line, not a core-prompt rule.
- **The attack tax** (attacking taps your blockers; 3rd campaign seat) -> guide prose this wave,
  plus an OPTIONAL per-seam render proposal recorded in notes.md. Not core: the fact is
  per-creature (Nadaar has vigilance and is exempt), so it belongs on the attacker option lines
  where the exemption can be expressed, not in a global instruction that would be false for every
  vigilant creature in the pool.

**No core-prompt change proposed.**
