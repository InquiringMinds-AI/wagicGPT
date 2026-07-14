# General / core proposals — deck44 wave 6 (convergence-gated)

Per the layer-routing discipline, each item names its layer and argues the burden of proof. Single-seat
evidence cannot cut or add a core line; only cross-deck agreement does.

## 1. CONVERGENCE (2 seats) — reflexive high-life blocking -> ENGINE/REPRESENTATION, not a new core line

- **Evidence:** deck109 (wave 5, terminal "do not block" fired 0 times; pilot blocked 4x at high
  life) + deck44 (wave 6: `e9289 s4`/`s7` chump racing flyers into a 2/2 at **L20**; `e1689 s47`
  blocks Phyrexian Obliterator at **L20**, feeding its sacrifice punisher). Two seats now.
- **Routing — deliberately NOT a louder core sentence.** The core prompt ALREADY carries the floor
  (line 29: "When you would stay above 8, blocking is optional and 'no blockers' is often the right
  answer: block only when you can name the gain"). The pilot READS enough to state a (false) gain —
  "prevent lethal" at 20 life — and blocks anyway. This is a rule PRESENT and LOSING at distance, which
  the skill routes to REPRESENTATION, never to a second paragraph of the same instruction.
- **Proposal (ENGINE/REPRESENTATION):** at the blockers decision, surface the decision-relevant number
  the pilot is ignoring — e.g. annotate the blocker prompt with `Your life: 20 (unblocked damage this
  turn: 2 -> you would be at 18)` or a `[not lethal]` marker when the total incoming is well below your
  life. The pilot's false "prevent lethal" belief is exactly the kind of world-model error a surfaced
  number dissolves (same shape as the ON THE STACK win). Per-deck reinforcement shipped this wave
  (deck44 strategy.txt BLOCKING block: at 9+ life your flyers never chump; only Oona's Gatewarden
  blocks) as the interim until representation lands.
- **Do NOT promote "racing creatures don't block" to core** — it would misplay every control deck, per
  the skill's own PER-DECK example. The deck-agnostic floor (already core) stays core; the posture stays
  per-deck.

## 2. ENGINE — offer a targeted spell ONLY when a legal target exists (deck-agnostic, contract-guaranteed)

- **Evidence:** deck44 F1 — `Cast Go for the Throat` was offered on boards with no legal nonartifact
  creature (vs140 all-lands+enchantment; vs135 opponent's only creature is a Snow Artifact Creature),
  9 stderr validation-fails across 2 games; the model's pick was silently dropped to a heuristic pass.
- **Why this is core/engine, not per-deck:** the prompt makes a UNIVERSAL contract to every deck — "the
  game has already checked your mana, colors, and **targets**." GFTT violates it deck-agnostically; any
  deck running targeted removal/burn/counters can hit this. The burden-of-proof for core is met: the
  failure is mechanically guaranteed wherever a targeted spell is castable but has no legal target.
- **Proposal:** at option-build time, for a spell/ability whose only legal mode requires a target,
  suppress the "Cast X" option (or mark it `[no legal target]` and make selecting it a no-op the parser
  rejects) when the target set is empty. This closes both the wasted-decision loop and the
  chosen!=happened data corruption. Filed with repro in notes.md.

## 3. PER-DECK (single seat) — counter-value floor; not core

- deck44 `e9289 s20` spent Arcane Denial (draws THEM 2 cards) countering a 1-mana Arcum's Astrolabe
  cantrip. Core line 22 already says a reaction is wasted on "a target that does not matter"; the pilot
  disobeyed at a cheap target. SINGLE SEAT — reinforced per-deck this wave (strategy.txt counter block:
  "never spend a counter on a 1-2 mana cantrip or mana rock; Arcane Denial draws THEM two cards"). Not a
  core edit. Put on the watch list; promote only if a second seat over-counters cheap value.

## Summary of routing
- Reflexive high-life blocking: ENGINE/REPRESENTATION (surface life-vs-lethal) + per-deck interim. 2 seats.
- GFTT no-target offer: ENGINE (target-existence check at offer time). Deck-agnostic, contract-guaranteed.
- Counter-value floor: PER-DECK. Single seat.
