# deck36 -> proposals for `strategy-writing-skill.md` (wave-32)

Seat context: Step-0 guideless debut, 4/6 with **4 clean kills and 0 adjudication-ahead wins** in a
pool with a 1/21 adjudication rate; 0 fallbacks in 144 decisions. Every proposal below is anchored
to a witness at this seat and states its seat count honestly.

---

## P1 (strongest; promotion-grade candidate) — THE ABSENT-FIELD AUDIT: a value that renders as an EMPTY STRING is a card-fact hole the pilot fills by INVENTION, and no existing rung can see it

**Witness.** deck36's three {0}-cost cards render with **no cost token at all**, at all three
emitters (decklist, cast option, hand/board): the guard is `if (cost && cost->getConvertedCost())`
and a converted cost of 0 is falsy. 347 renders at this seat, zero carrying `{0}`. The pilot
manufactured four *different* rules to explain the absence and declined a free spell in 4 of its 7
cast-nothing windows — *"they are not spells you can cast from your hand"*, *"putting them onto the
battlefield requires a land drop slot"*, *"Moxes... cost mana to cast. I have no extra mana"*,
*"Ornithopter/Moxes are low priority"* — and burned its two largest cast-seam replies (8,600 and
7,040 chars) deriving whether a Mox is free.

**Why the existing machinery is blind to it.** The verify-oracle rail (wave-28 HL3, wave-30 HL3) and
the retroactive card-fact audit (wave-31 HL1) both check numbers that ARE written, against the
primitive. Neither can fire on a number that is ABSENT: there is no wrong value to catch, no
contradiction to grep, and the option line still parses and still resolves correctly, so the
fallback/defer counters record nothing. It is the OFFER-side analogue of the castable-but-not-offered
audit (wave-31 HL9): *an option that is silently ABSENT leaves no trace in any counter* — here it is
a FIELD that is silently absent, inside an option that is present.

**Proposed rung (Step 2, one grep per deck, bounded and one-time).** **ABSENT-FIELD AUDIT: for each
of the deck's cards, diff the primitive's declared fields (`mana=`, `power=`, `toughness=`,
`abilities=`, `subtype=`) against what the corpus render actually emits for that card, and treat any
field the render SUPPRESSES as a first-class representation defect with a guide stopgap.** The
general failure shape is `if (value) print(value)`, which deletes every legitimate ZERO: a {0} mana
cost, a 0-power creature, a 0-counter permanent, an empty subtype. Practical procedure: pick the
deck's cheapest and its most unusual card, grep every render of its name in the corpus, and list
which primitive fields never appear.

**Authoring consequence.** When a field is suppressed, the guide states the missing fact as a
FALSITY-FACT interception naming the *general rule of the surface*, not just the card:
"a card with no cost printed on its line costs {0}" beats "Mox Jet is free", because the pilot meets
the same empty field on Ornithopter next turn. (deck36 RULE #1 ships both halves.)

---

## P2 — HL8's "#1 slot goes to the leak that traces to the LOSS" needs a third case: when the losses route to ENGINE or CONSTRUCTION, the #1 slot goes to the largest leak measured across the WINS

**Witness.** deck36's two non-wins decompose to (a) a cap adjudication lost to an **invisible clock**
— poison counters appear nowhere in the render and the blockers header computed a false life
projection against infect attackers in 5 of 5 windows, which the pilot quoted verbatim while
declining to block (notes N-36a); and (b) a death that is mostly **construction** — a 7-mana
Akroma's Memorial drawn in a deck that kills on turn 8, bottomed once and drawn again, uncastable at
both decisions where it sat in hand — plus one real over-attack at 10 life.

Neither can hold the #1 slot. (a) is a PERCEPTION failure and guide prose against a contradicting
rendered number loses by the established routing; (b) is a decklist flag. Following HL8 literally
would leave a strong seat with no #1 rule, or invite the reviewer to manufacture a posture — the
exact failure HL8 was written to prevent.

**Proposed rung.** *When a Step-0 seat's losses route to ENGINE/REPRESENTATION or to CONSTRUCTION,
take the #1 slot from the largest measured leak across the WON games.* A won game is still evidence:
deck36 won on turn 7 and turn 8 having declined a free spell four times and burned its two biggest
replies on the same question. The win column hides leaks; the cast-nothing split and the tax table do
not. State explicitly in findings which bucket each loss routed to, so the #1 choice is auditable.

---

## P3 — the HL9 cast-nothing SPLIT has a structurally empty branch on some decks; count the deck's instants first and say so

**Witness.** HL9's split is (a) windows offering only instant-speed removal (a legitimate hold) vs
(b) windows offering at least one creature (a decline against the floor). deck36 contains **zero
instants and zero sorceries** — every card in the deck is a permanent. Branch (a) therefore cannot
exist by construction, and the raw rate IS the split: 7 cast-nothing windows, **7/7 declines of a
payable permanent, 0/7 legitimate holds**.

**Proposed addition, one line to HL9.** *Before running the split, state the deck's instant count.
On a permanent-only deck the legitimate-hold branch is structurally empty and the RAW rate is the
finding; on an instant-rich deck the raw rate is uninformative and only the split is.* Cheap, and it
stops a reviewer reporting "7 cast-nothings, needs the split" when the split is already done.

---

## P4 (single-seat, WATCH) — the SELF-EXCLUSIVE MAGNITUDE: whenever a rendered number counts a class the card itself belongs to, check for the off-by-one at cast time

**Witness.** `Cast Master of Etherium {2}{u} (6/6)` at `Artifacts in play: you 6` produced a **7/7**
on the battlefield (deck139 s14->s15); `(5/5)` at `you 5` produced a **6/6** (deck116 s10->s11). The
CDA is correct and tracks live in hand across seven consecutive renders (0/0 -> 6/6 in lockstep with
the count line); the option is simply built while the card is still in hand, so its self-inclusive
count is one short. The pilot's own estimates then drifted in BOTH directions (planned a "4/4" off a
printed (5/5); planned a "9/9" at `you 10`, i.e. an 11/11).

This is the third member of one family, after Gray Merchant's `{drains N}` (under-counted by 2, same
cause — the card is not yet on the battlefield when the option is built) and the surfaced
`Artifacts in play: you N` read-swap.

**Proposed rung, generalizing the GM-specific one in Step 5.** *Whenever a card's rendered magnitude
counts a CLASS the card itself belongs to — artifacts, devotion, creature count, permanents, cards in
a zone — verify the cast-time value against the resolved value before teaching the read, and expect a
self-exclusion offset.* Guide ships the offset as a stopgap with an explicit removal condition; the
durable fix builds the magnitude against the post-resolution state.

---

## P5 (instrument, single-seat, cheap) — measure PLAN-FIELD length; a fat plan field is the belief-spiral's fuel line

**Witness.** **51 of 132 prompts (39%)** at this seat carry a `YOUR PLAN (as you last stated it):`
block over 400 characters, several 600-800. The splitter anchors on an in-reasoning quotation: at
deck152 s3 the model wrote `But wait, the prompt says "Your Plan: Play Ancient Den..."` inside its
deliberation, and s4's plan field is that quote plus the next ~20 lines — the entire unresolved
"Moxes have no cost listed / I cannot pay {1}" derivation. s5 then carries s4's confusion the same
way. The pilot's own unsettled reasoning is returned to it as authoritative plan text on the very
next decision.

**Proposed addition to the reviewer's instrument list (Step 2).** *One regex: report the fraction of
prompts whose plan block exceeds ~400 chars. A high fraction means the pilot is being fed its own
unresolved reasoning as a plan, which amplifies whatever belief the reviewer is otherwise chasing —
and it is a PARSER item, not a guide item.* Pairs naturally with the tax table: the polluted prompts
and the tax spikes were the same decisions here.

---

## P6 (housekeeping note, not a rung) — length band

The deck36 initial guide landed at **108 lines / 7.6 KB**, above the ~40-80 band and above deck17's
85 lines. Justification: this is an ENGINE deck under the deck62 rule-3 face, so rule #1 is a
CAPABILITY correction plus a capability table, and the deck's identity is a mechanic (artifact count)
that touches ten cards. Per Step 0-bis rule 6 the first revision should trim whatever the wave-33
corpus shows as pure tax — most likely the KEY CARDS entries for cards the corpus never exercised
(Scarecrone, Academy Ruins, Coiled Tinviper, Vedalken Archmage). Flagging it rather than hiding it.
