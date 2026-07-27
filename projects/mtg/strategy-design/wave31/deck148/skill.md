# Skill proposals from deck148 (wave 31) - the CEILINGED-SEAT EXIT corpus

Two proposals. The first is a new named TEACH CATEGORY with a fresh witness; the second is a
CLOSING refinement to the construction-ceilinged rung that this seat's exit completes.

---

## PROPOSAL 1 (NEW TEACH CATEGORY) - PAYOFF-PERMANENT PRESERVATION: "the engine is not a trade"

**Shape.** When a deck's plan hangs on ONE fragile enabler (a lord, an anthem body, a cost-reducer,
a combo half), the guide must say explicitly that the enabler **is not tradeable in combat** - and
say it at the BLOCKING seam, not only in the card's value paragraph. Naming a card "your best
engine" raises its perceived value but supplies no COMBAT rule, so at the blockers seam the model
falls back on the generic even-trade prior and throws the engine away.

**Witness (deck148 vs152 seq10, t7).** Guide rule #2 said "GEAR ARMAMENT MASTER ITSELF FIRST; IT IS
YOUR BEST ENGINE" and the model demonstrably internalized it (it quotes that sentence verbatim at
vs146 seq19). At vs152 seq10, on **20 life**, holding the equipment in hand, one turn after writing
in its own PLAN "next turn equip Captain's Claws to Armament Master to activate its team-wide pump",
it blocked a 2/2 Wolf token with the ungeared Armament Master and traded it off, reasoning
*"trading my 2/2 for their 2/2 is the correct tempo move."* The game was lost by 2 life.

**Why this is a SKILL item and not a deck148 guide edit.** The failure is not deck-specific
knowledge; it is a systematic interaction between two things every guide has: (a) an operative
teach that RAISES a permanent's value without constraining its use, and (b) an engine annotation at
the blockers seam that presents the trade in perfectly SYMMETRIC, value-blind terms
(`B1. Armament Master (2/2) - may block A1 (both die)`). Per the perception-vs-strategy routing
rule this is a STRATEGY failure - the model read the state correctly and decided wrong - so the fix
belongs in guide prose authored by the skill, not in the render.

**Proposed addition to the skill's teach-category catalogue** (drafting rule, not literal prose):

> **PAYOFF-PERMANENT PRESERVATION.** Identify the ONE permanent the deck's plan cannot proceed
> without, then ask: is it a fragile body (small, no protection, no haste, no evasion)? If yes, the
> guide owes a POSITIVE, seam-labeled combat rule alongside the value teach - e.g. *"When <engine>
> is your only untapped blocker and the incoming damage is not lethal, TAKE THE DAMAGE. <engine>
> alive next turn is worth more than any creature it can trade with; the block line will offer you
> an even trade and you decline it."* Place it at the BLOCKING seam block (the seam where the
> decision is actually made), never as a tail-end prohibition, and state the life-total condition
> so the rule has an off-ramp when the damage IS lethal.

**Cross-references it strengthens.** This is the combat-seam sibling of the existing rung "a seam
always needs its own positive floor at a seam-labeled block" (deck109 wave 5). It also supplies a
second face for the wave-30 headline-7 sub-observation ("for a FRAGILE lord, gear-the-lord-first is
gated on the lord SURVIVING") - wave 30 read that as a pure construction pair; wave 31 shows that
one of the two survival failures was a DECISION the guide could have prevented, so the rung should
say: **when a fragile-lord teach underperforms, split the cause into (i) opponent removal /
construction and (ii) voluntary trades the guide never forbade, and only route (i) upstream.**

**Confidence: single seat, single witness -> tag as a proposal, promote on a second seat.** The
prior is strong (it is a named generic-prior failure with a quoted mechanism) but it has one
occurrence; a second witness at any lord/anthem/combo deck should promote it to an established
category and add it to the DoD checklist.

---

## PROPOSAL 2 (CLOSING refinement) - the CONSTRUCTION-CEILINGED rung gains its EXIT-CONFIRMATION clause

Wave-30 headline 7 defined the exit ("ONE validation corpus, THEN freeze + hand off"). deck148 has
now WALKED that exit, and the walk surfaces one thing the rung does not yet say: **the confirming
corpus must be read against POOL COMPOSITION, because a ceilinged deck's record is the pool's
property, not the deck's.**

deck148 went 1/6 -> 2/6 -> **4/6** across three corpora with a byte-identical guide and an
unchanged decklist. Four of six matchups flipped between the last two corpora; the pool's
cap-adjudication rate fell 12/21 -> 3/21 (deck148 had lost two wave-30 games by adjudication while
alive); and one grindy midrange was swapped for an aggro deck that this shell races well. Its
combined guided record is 6/12.

**Proposed clause (append to the construction-ceilinged rung):**

> **Reading the confirming corpus.** A construction-ceilinged deck has, by definition, a realized
> record dominated by pool composition rather than by its guide. Do NOT read a record MOVE on the
> confirming corpus as evidence the ceiling moved. Confirm the ceiling on decision-level evidence
> instead: (a) every teach fires again under the new pool; (b) the LOSSES show the ceiling
> signature - decompose what the deck could not do, not how far it was behind (a ceilinged aggro
> deck that leaves BOTH its conquerors on 1-2 life is displaying its ceiling precisely, and is
> better evidence than a win); (c) name the pool deltas that moved the record (adjudication rate,
> archetype swaps, matchup flips) so the synthesis does not credit them to the guide. A ceilinged
> seat exits on a stable TEACH record and a decomposed loss profile, never on a win count.

This mirrors, with the sign reversed, wave-30 headline 6's adjudication-alignment caveat (which
protected the pipeline from OVER-crediting a perfecta): the same discipline must protect it from
UNDER- or over-crediting a ceilinged deck's record swing in either direction.

---

## Non-proposals (checked and deliberately not raised)
- **Mulligan "zero creatures" misread** (vs146 seq1): a STATE misread with an existing precedent fix
  in the code (the deck93 land-tag). Representation layer, notes.md N-148d - not a skill or guide
  item.
- **>10k-char spirals on low-stakes seams** (6 this corpus, max 14,120 ch): the wave-25 headline-5
  ceiling predicts exactly this and calls it latency, not correctness. Confirmed here (zero wrong
  commits, zero truncations). No skill change - this is a confirming data point, and the rung
  already forbids adding more per-seat floors for it.
