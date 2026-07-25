# deck137 skill proposal (wave 25)

## PROPOSAL 1 — PROMOTE wave-24's PROPOSAL 2 with a CEILING clause: per-seat floors relocate the spiral; they cannot eliminate it.

Wave-24 tagged single-seat WATCH: "when a guide/representation fix eliminates a decode spiral at one
seam, the model's worst spiral RELOCATES to the next seam that lacks a commit-rule," with the
promotion trigger "if a SECOND deck's worst spiral is a lethal-block after its attackers seam was
floored." deck137 alone now supplies a THIRD data point in its OWN causal chain, which is stronger
than a second-deck coincidence — the relocation is reproducible within one seat across three waves:
- wave-23: worst spiral = ATTACKERS (15,901 ch, "ATTACK: none" passivity). Guide added an attack floor.
- wave-24: attackers spiral-free; worst spiral = BLOCKERS (13,642 ch, illegal one-on-four). Guide
  added a blocking-under-lethal commit rule.
- wave-25: BOTH floored (max blockers reply 1,328 ch, zero illegal); worst spirals (5 replies,
  12,787–14,987 ch) are now on GENERIC land-drops, priority passes, single-option casts, and library
  reveals — seams that CANNOT be enumerated and floored one at a time.

**Authoring implication to record as a named rung (refining, not just watching):** per-seat commit
rules are worth adding where a spiral concentrates on a HIGH-STAKES seam (attackers, blockers) — they
demonstrably collapse those spirals ~10x and the correct answer survives. BUT they have a CEILING:
the spiral does not vanish, it migrates, and it terminates on ubiquitous low-stakes seams (every land
drop, every priority window) that no per-seat guide can reach. Therefore:
(a) DO floor the named high-stakes seams (proven ROI, and they are where a spiral can also produce an
    ILLEGAL commit, which matters);
(b) DO NOT keep chasing the residual with ever-more per-seat guide text once the high-stakes seams are
    clean — the residual generic-seam over-deliberation is a MODEL property, harmless to correctness
    (choices land right; see PROPOSAL 2), and belongs to latency/harness management, not the guide;
(c) a general "commit briefly, don't re-derive rules" directive, IF ever wanted, is a CORE/representation
    touch (it applies to every seat and every seam), never a per-deck guide line — route to synthesis,
    do not multiply per-deck floors.
This closes the wave-24 WATCH: the prediction held, the mechanism is confirmed, and the actionable
doctrine is "floor the high-stakes seams, then stop — the rest is latency, not guidance."

## PROPOSAL 2 (skill authoring note) — trust-the-engine-legality guides need BOTH directions, and the "resolved adventure body" is a model-belief face.

deck137 s21 (deck27 game) is a clean witness for a general authoring gap: a guide that says "the
engine's offered list already drops ineligible options — trust it" was written ONE-directional
(wave-24 guide: "if Lovestruck is MISSING it's because you control no 1/1"). The model then hit the
INVERSE case — Lovestruck was PRESENT (engine confirmed a 1/1) — and OVERRODE the offered list,
re-deriving the 1/1 condition itself and wrongly concluding the creature was ineligible because it
misread a battlefield `[adventure]`-tagged 1/1 as "not a real body." Two skill takeaways:
- **When a guide tells the model to trust an engine-computed legal set, state BOTH directions
  explicitly** — "if X is absent, the condition failed" AND "if X is present, the condition is
  already satisfied, so act; do not re-derive it." The one-directional form leaves the model free to
  re-litigate the present case and talk itself out of a legal play.
- **"A resolved adventure creature is not a real body" is a genuine MODEL-BELIEF face** (contrast the
  wave-23 R-ADV-FACE MISRENDER, which was a representation bug the option-annotation fix eliminated).
  Here the render is CORRECT — the board line shows `(1/1) [flying, adventure]` — and the model still
  misbelieves the tagged body is not a 1/1. That is a belief no FACT surface fully cures (the fact is
  already present), so it is guide/skill territory, not a ledger item. Recorded as a deck-layer guide
  paragraph this wave; watch whether the SAME misread (a [adventure]/[from exile]/token tag read as
  "not a real creature") appears at another seat, in which case the belief is corpus-general and the
  fix should move to the representation layer (e.g. drop the [adventure] tag once the card is a
  resolved battlefield body, or annotate it "(a real 1/1 creature)") rather than N per-deck guides.

## Reconfirmed doctrine (no new rung) — b2 last-answer-wins is a spiral SAFETY NET.
The relocated generic-seam spirals are harmless in large part BECAUSE b2 NATURAL-STOP precedence takes
the model's FINAL coded line: 4 replies this seat opened with a worse snap answer, spiraled, and
CONVERGED on a better final choice that the parser correctly consumed (deck27 s19/s40, deck59 s46,
deck131 s37). This is worth keeping visible in the skill's evidence base: flooring reduces spiral
LENGTH (latency), while b2 protects spiral CORRECTNESS (the answer). They are complementary, not
substitutes — which is why the residual generic-seam spiral can be left as a latency concern rather
than a correctness one.
