# General / core proposals — deck44 wave 7 (convergence-gated)

Frozen-spine seat. Per layer-routing discipline, single-seat evidence cannot cut or add a core line;
each item names its layer and its burden of proof.

## 1. ENGINE — target-existence check before offering a targeted spell (RE-FILE, prediction CLOSED)

- **Evidence:** wave-6 filed this from deck44's GFTT (9 validation-drops). This wave the deck44
  stopgap held (0 GFTT drops) and the class MIGRATED to **Liliana of the Veil (4x) + Fatal Push (2x)**
  in the opponents' seats — the deck-agnostic engine defect walking to the next targeted spell exactly
  as predicted. This is now a two-wave, multi-card confirmation of a mechanically-guaranteed contract
  violation ("the game has already checked your mana, colors, and TARGETS").
- **Routing: ENGINE, unchanged from wave 6.** Burden of proof for core is more than met — it is not a
  guide problem on ANY deck. At option-build time, for a spell/ability whose only legal mode requires
  a target, suppress or mark `[no legal target]` the "Cast X" option when the legal target set is
  empty, and make selecting it a parser-rejected no-op rather than a silent heuristic deferral.
- **Also (harness data-quality):** when the engine defers a validation-failed pick to the heuristic,
  the JSONL still logs the dropped pick as `chosen_text`. Add `deferred_to_heuristic: true` so
  reviewers do not have to cross-reference stderr. (Carried from wave-6 notes; still open.) Filed with
  repro in notes.md.

## 2. ENGINE/REPRESENTATION — the Phyrexian-Obliterator block is a life-vs-punisher visibility gap (single seat; per-deck fixed this wave)

- **Evidence:** deck44 `vs133 s18` — the pilot blocked Phyrexian Obliterator at L11 (feeding its
  sacrifice punisher) after correctly declining the same block at L20. The guide forbids it; the pilot
  broke the rule under life pressure because the prohibition was nested inside the low-life chump rule.
- **Routing: PER-DECK this wave** (repositioned to a standalone absolute in strategy.txt), NOT a core
  edit — this is a named-card interaction, single seat. But it belongs on the ENGINE/REPRESENTATION
  watch list as an instance of the SAME shape as wave-6's reflexive-block convergence: a punisher/
  no-block fact the pilot cannot see at the block seam. If a second deck shows a pilot blocking a
  damage-punisher creature (Phyrexian Obliterator, Stuffy Doll, spikeshell/creatures with "when dealt
  damage") the shared fix is REPRESENTATION — annotate the blocker option with the punisher rider
  ("blocking this makes YOU sacrifice/take X"), the same surfacing that fixed the stack seam. One seat
  now; do NOT add core text.

## 3. WATCH (no edit) — plan/choice mismatch on attractive labeled counter options

- deck44 `vs133 s13`: PLAN says "let it resolve," reply picks the labeled Arcane Denial number. The
  wave-5 representation win (labeling counter options with what they can hit) made counters attractive
  enough that the pilot occasionally picks the counter number over "Cast nothing" against its own
  plan. Single instance this wave; wave-6 said the class was "gone." Keep on the watch list as a
  low-frequency residual of a GOOD representation fix — promote to a REPLY-PROTOCOL / plan-binding
  intervention only if a second seat shows the same "plan says no-counter, picks counter" mis-bind.

## Summary of routing
- GFTT/Liliana/Fatal Push targetless offers: ENGINE (target-existence check). Deck-agnostic,
  two-wave / multi-card confirmation. Prediction closed.
- Phyrexian-Obliterator no-block: PER-DECK now; ENGINE/REPRESENTATION watch (1 seat, punisher-rider
  surfacing shares the reflexive-block convergence fix).
- Plan/choice mismatch on counter labels: WATCH, single seat.
