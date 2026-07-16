# Deck-135 wave-17 — cross-seat observations

## 1. Reflexive Force-of-Negation on FACE BURN persists — 3rd corpus, cross-check the burn seats.
deck135's 2 Force casts this corpus (deck21 s6 @19 life exiling Search for Azcanta; s9 @10 life
exiling Ice-Fang Coatl) both countered a Volcanic Hammer that was `[spell] targeting you` — face
burn — against the guide's hardest verbatim "NEVER Force a burn spell pointed at your own face"
line. This is the SAME model over-application seen wave-15 (Cranial Plating) and wave-16 (s19
Volcanic Hammer face). The guide already scopes Force correctly and bold-forbids this; the failure
is MODEL discipline, not a guide gap. Cross-seat signal: any seat whose deck holds a free/cheap
counter and faces a burn/aggro opponent is exposed to the same reflex (spend 2 cards to stop 3
face damage). Route to model-experiments, NOT to more guide restatement (deck135's line is already
maximal — a 4th restatement would be sanctioned-against escalation with no evidence it helps).

## 2. Multi-target ORDERING sub-menus are a likely CROSS-CARD representation defect.
The "Choose target N of exactly 4 for Azcanta, the Sunken Ruin" cluster (findings charge c) is the
engine exposing a "look at top N, take one, order the rest onto the bottom" mechanic as a bare
shrinking-option ordering loop — no effect statement, no hand-eligibility marks, no running state.
deck135 whiffed BOTH Azcanta activations to zero cards-in-hand (it kept picking ineligible creatures
as the "keep"). The SAME shape likely appears wherever the engine resolves reveal-and-order effects:
Glacial Revelation ("put snow permanents into hand"), Scrying Sheets, and any fetch/scry that orders
multiple library cards. This is the same FAMILY as deck110's promoted E6 Mox Opal degenerate-label
item. Synthesis should treat "internal-mechanic-exposed-as-bare-menu" as one engine theme with (at
least) two members (Azcanta ordering, Mox Opal cast-label), and prioritize a general representation
pass: (a) name the EFFECT in plain terms, (b) mark which options are ELIGIBLE for the beneficial
slot, (c) collapse or clearly label immaterial ordering steps (bottom-of-library order the player
will likely never see), (d) show already-assigned picks.

## 3. Model card-KNOWLEDGE errors (type / cost / color) recur cross-seat.
This corpus: (i) Azcanta "grabs a creature" — the model tried to put Abominable Treefolk / Ohran
Viper / Diamond Faerie (all creatures) into hand via an ability that only takes NONCREATURE-NONLAND;
(ii) Treefolk mana-miscount — vs17 s19/s20 the model asserted `{g}{u}{u}` (3 mana) could pay
Treefolk's `{2}{g}{u}` (4 mana) and passed three decisions "waiting" on an unaffordable creature the
engine correctly never offered. These join the carried wave-16 family (Into the North called a "blue
card"; Flooded Strand "fetches a Forest"). Common root: the model reasons from a hallucinated
card property (type/cost/color) instead of the option list / battlefield line the engine gives it.
Cross-seat, model-side; the OPTIONS-are-ground-truth discipline the guides already teach is the
right frame, but the failure is knowledge, not framing. Route to model-experiments.
