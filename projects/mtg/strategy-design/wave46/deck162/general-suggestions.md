# deck162 -> core-prompt / render proposals (wave 46)

Engine defects are in the seat file's E-list; this file is core-prompt and render convention only.

---

## G1 (HIGH) — an A-line with no "their untapped blockers" tag should say so affirmatively

**Evidence.** Six of deck162's eight attack windows this corpus have A-lines with **no** blockers
tag, and the render is honest in every case (the opponent's battlefield line reads `(0 permanents
listed)`, or their only creature is a flier the attacker is a flier past). The model declined the
attack twice — at opponent life 13 and at opponent life 4 — reasoning "preserve Fate Unraveler as
a blocker" against a player with no creatures at all. Absence is the least legible state a surface
can be in: the model cannot tell "nothing can block this" from "the tag was not computed".

**Proposal.** Print the affirmative form: `[no creature they control can block this attacker]`,
alongside the existing menace and held-back tags, which are already written in exactly this
positive style (`[menace - cannot be blocked by fewer than two creatures; they have 0 untapped
creatures that could join such a block, so it cannot be blocked at all this combat]` — that one
renders and works). This costs a line and removes an inference the model demonstrably gets wrong
in the direction of passivity. It also gives every guide something to key a rung to.

## G2 (HIGH) — the shared attack-tag footnote explains gang blocks but not lifegain

**Evidence.** The footnote under every attackers prompt is thorough about gang blocks and about
the collapsed form. It says nothing about what the parenthetical tails mean. deck162's tag read
`(you kill it, your attacker lives (lifelink: they gain 1))` three times over and the reply
treated it as an all-free tag — which, on the send criterion the guide uses, it structurally is.
That attack was a 19-to-0 turn.

**Proposal.** One sentence in the footnote: a parenthetical that names a benefit to THEM
(`(blocking trigger: ...)`, `(lifelink: they gain N)`) is a PRICE on that outcome, not part of
the outcome — the fight result is the first clause, the price is the second, and both happen. The
wave-46 lifelink binding did this beautifully for the fatal branch ("you gain 4 from this block
only, and this attacker deals nothing to their life"); the friendly branches carry the tail with
no such framing.

## G3 (MED) — the mana line's per-source enumeration is right; the generic clause is missing

**Evidence.** The mana block renders fully — total, colour list, per-colour source counts, and
`Those sources, one per untapped card: Drowned Catacomb {u} or {b}; Island {u}; Swamp {b}`.
Coloured-pip overcommits on my seats: **0**. The single blemish is a 350-second reply on deck162
that reasons about whether it "has generic mana" (vs152 seq 10). deck152's guide contains the
correction; deck162's did not.

**Proposal.** This is guide work first (done — deck162 RULE 0b), but the core prompt could close
it for every deck with four words in the mana line itself: `mana of ANY colour pays a generic
cost like {2}` already appears — it could be strengthened to state that generic is never a
separate resource you can lack. Low cost, and it would retire a recurring latency sink.

## G4 (MED) — the land-drop question is the only decision in the corpus with exactly one right
## answer and it costs full deliberation

**Evidence.** Across the corpus the two slowest 2-option asks are both land-drop binaries (453 s
and 367 s). deck162 declined its land drop four times, three of them in one game, each time
reasoning that it preferred to cast something. The prompt already carries the counter-argument on
its own line ("The land drop is its OWN decision ... its absence from the choices below does not
mean it is gone"), and it is not enough.

**Proposal.** Two options, either or both: (a) suppress the PLAN request on land-drop asks, since
a land drop cannot falsify a plan; (b) state the consequence rather than the mechanism — `playing
this land does not reduce what you can cast this turn`. The current sentence explains the
question's independence; it never says the thing the model gets wrong.

## G5 (LOW) — the history line's damage OBJECT is still unprefixed

**Evidence.** Lane E is a clean pass on the subject: 273 damage lines on my three seats, every one
`Your X dealt N damage to ...` / `Opponent's X dealt N damage to ...`, and a sweep for an
unprefixed card subject anywhere in my seats' history returns zero. **27 of those 273 lines have a
bare creature as the OBJECT** (`- Your Wolf dealt 2 damage to Perimeter Captain`). Harmless while
the name is unique; ambiguous the moment both players control the same creature, which this pool
does routinely (Silverquill Silencer appears on both sides of 146-vs-152's log in adjacent lines).

**Proposal.** Same treatment, same wave, other half of the sentence.
