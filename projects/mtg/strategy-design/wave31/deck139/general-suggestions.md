# deck139 -- wave-31 core (general prompt) suggestions

**PASS** -- 26th consecutive. Nothing in this corpus routes to the general prompt as a defect.

Justification: 0 fallbacks / 154 decisions at this seat; 0 unparsed, 0 retracted, 0
truncation-abandons, 0 retries. The answer-first CHOICE/ATTACK/BLOCK/PUT contract parsed the full
multi-ask mutate flow (cost line -> normal/mutate confirmation -> over/under -> host TARGET CHOICE ->
mutate trigger target) without a single drift, including a 13,337-char reply. Every defect this
wave routed cleanly to the deck guide (findings.md 2a-2c), the render/harness (notes.md
N-139i/j/k/l), or the roster owner (construction, findings.md 3b).

## One WATCH item, deliberately not raised as a proposal

The ~200 s latency tail is now in its third consecutive corpus at this seat and it **left the combat
seams**: of the six decisions above 195 s, three are casting asks, one is mulligan bottoming, one is
priority, and only one is attackers (findings.md 4e). All six produced 12.2-13.3 k-char replies --
i.e. the tail is a reply-LENGTH behaviour at the token cap, not a property of any decision kind, and
the guide-side "decide fast" teach cannot be expected to fix a seam-agnostic phenomenon (it was
already present, scoped to combat, and the tail simply moved).

Two observations that make this a core-prompt-shaped question rather than a guide one:
- Every long reply shows the same oscillation ("Wait, let me re-read the combat log"), and in every
  case the answer-first line was already committed before the self-correction -- so the extra tokens
  buy nothing and occasionally document the pilot talking itself out of the answer it already gave
  (vs137 seq19, 208 s: *"Wait, Arboreal Grazer is 0/3. It has 0 power."* after `ATTACK: A1, A2, A3`).
- On a wall-clock-capped harness the cost is real and one-directional: this seat lost one game by
  cap adjudication (vs137, adj0 at T16) and the pool ran 3 cap-adjudications total.

**Why it is a watch and not a proposal:** the plausible interventions (a max-token trim, a
"stop after N sentences" instruction, a reasoning-length brake) all trade decision quality for
wall-clock on a seat whose decision quality is already the problem, and a single seat's tail is not
evidence for a corpus-wide sampling change. The right owner is synthesis, with the cross-seat tail
distribution in hand. Raising it here as data, not as a recommendation.
