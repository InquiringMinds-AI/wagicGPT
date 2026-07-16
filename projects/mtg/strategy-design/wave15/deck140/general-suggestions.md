# deck140 wave-15 — general suggestions (cross-deck; layer-routed)

Scope reminder: single-seat evidence cannot add/cut core prompt lines. These are cross-cutting
observations for the synthesis pass, not deck140 guide edits.

## 1. [ENGINE — HIGH VALUE, corpus-wide] Generate the "Mana available" line from the same reachability engine as the legal-move set
deck140 alone produced **48** display contradictions this corpus where an OFFERED (therefore legal)
"Cast X {…color…}" required a color the "Mana available" line did not show — because the line renders
one defaulted tap-plan (duals shown as their first color, hiding the second). It caused zero misplays
HERE only because the model happened to treat the offered list as authoritative and always had a
visible alternative. This is a latent land-mine: the day the hidden color is the ONLY path to a needed
cast (the wave-13 vs109 shape — a declined payable Cruel Edict at a mandated sweep), it converts to a
game-losing decline. The fix is structural and benefits EVERY multicolor deck: derive the affordability
annotation from the same color-reachability engine that builds the offered-options list (or show
per-source color options), so "offered" and "Mana available" can never disagree. This is the single
highest-leverage engine item touching deck140.

## 2. [METHOD — reviewer synthesis] Report engine-item recurrence as (exercised? / reproduced? / impact)
Two owned items this cycle illustrate why a raw "did it recur?" verdict misleads. The Rakdos
X-underpay was NOT EXERCISED (Rakdos never cast) — scoring it "no recurrence" risks retiring an
untested item. The dual-hiding WAS exercised 48x but with ZERO impact — scoring it by count alone
over-states harm. Synthesis should carry three fields per engine item: was the triggering action
present, was the defect reproduced when present, and did it change a decision. Keeps the ledger honest
across corpora where the round-robin draw omits the deck (deck109) that first surfaced a bug.

## 3. [DECK-CONSTRUCTION — user owns, restated not relitigated] deck140's loss column is threat-mismatch, not pilot error
The one loss (vs135) was an Abominable Treefolk (0/0 base, snow-pumped 8/8 trample, regrows past
-1/-1 counters) that outran Black Sun's Zenith's affordable X while Wrath/Damnation stayed
undrawn — the same reach/threat-mismatch shape as prior waves' loss column. 14 sweepers is the right
DENSITY; the deck wants (a) a cheaper high-toughness answer that doesn't need a big X, or (b) more
reliable early RED so Pyroclasm/Rakdos aren't display-hidden and mana-starved. Flag only; no guide text
fixes a drawn-too-late Wrath.

## 4. [WATCH — engine representation, needs a 2nd witness] Dynamic-P/T (snow / characteristic-defining 0/0) + -1/-1 counters render confusingly
vs135's Abominable Treefolk repeatedly showed a fluctuating live P/T (4/4->5/5->9/9, printed 0/0)
next to a static "[counters: 4x -1/-1]", and the model burned reasoning unsure whether it was dead.
Candidate representation improvement (show effective P/T after counters, or annotate "base set by
snow"), but it is opponent-side, single-seat, and did not by itself cause a deck140 misplay. Hold for
a 2nd witness before legislating.
