# deck134 -> development notes (wave 27). Corpus matchups-20260725-193608, binary ffec79fd8.

Layer routing: these are engine/harness/render/card-script items (NOT guide, NOT core doctrine).
Step-0 seat; seq repros included. None is a confirmed defect blocking correctness except where marked.

## N-134a (card-script divergence, LOW) Moment's Peace flashback cost
primitives/mtg.txt: `flashback={2}{G}`, but real Oracle Moment's Peace flashback is {5}{G}. Engine
version is much cheaper (a near-free repeatable fog). Route to Oracle-verification per the
wagicgpt-verify-oracle-text discipline: decide whether to correct the card script to {5}{G} or keep
the divergence. The GUIDE uses the engine behavior (recastable fog) as-is. Not urgent.

## N-134b (RENDER, LOW) Moment's Peace flashback option shows the wrong cost number
The graveyard-cast option renders "Cast Moment's Peace {1}{g} [from your graveyard]" (vs18 seq18,
seq20; vs93 seq48) - i.e. it shows the {1}{G} BASE cost, not the {2}{G} flashback cost the engine
actually charges. Likely the option builder prints `mana=` rather than the flashback cost for a
graveyard cast. Verify the cost actually charged; if it charges flashback but displays base, the
label under-states the cost. Harmless to play here (still cheap) but a display-accuracy item.

## N-134c (RENDER / WATCH) Pulse of Murasa target-warning annotation vs a rider-value card
Options render "Cast Pulse of Murasa ... - the only legal targets are YOUR OWN right now"
(vs137 seq19) and "... warning: every legal target is the OPPONENT's" (vs22 seq9). The annotation
is accurate about the TARGET, but Pulse's primary value is the RIDER (`life:6 controller` = gain 6
life) which fires regardless of target. For a card whose value is a rider not the target, a
target-only warning can over-discourage a correct low-life cast (the pilot under-cast Pulse at low
life). WATCH: consider annotating rider magnitude on such option lines (e.g. "{gain 6 life}")
alongside the target warning, mirroring the Gray Merchant `{drains N}` treatment. Single card, low.

## N-134d (SEAM / WATCH) Mulldrifter double cast-mode menu (possible redundant ask)
vs131 seq12 top-level cast menu offered BOTH "Cast Mulldrifter {4}{u} (2/2)" AND "Cast Mulldrifter
with its evoke cost {2}{u} (2/2)"; after the pilot picked the {4}{u} hard-cast, seq13 asked AGAIN
"Cast Card Normally / evoke" and the pilot then picked evoke - a self-contradiction across two
adjacent asks. Also vs137 seq15 (single top-level pair) resolved in one ask. So Mulldrifter is
sometimes a one-step choice and sometimes a two-step (top-menu mode THEN a normal/evoke sub-menu).
If both the top-level evoke option AND a following normal/evoke sub-menu can fire for the same
card, that is a redundant/confusable double-ask seam worth de-duping (pick the mode once). Verify
whether the top-level "with its evoke cost" and the sub-menu are the same decision surfaced twice.

## N-134e (RENDER, LOW) Compelling Argument cycling option shows the MILL text
The cycling activation renders "cycling with Compelling Argument [cost: {u}, Cycle] {card text:
"Target player puts the top five cards..."}" (vs137 seq3, seq9; vs131 seq16). The annotation shows
the SPELL's mill text, not the cycling effect (discard-to-draw). Minor: the cost label ({u}, Cycle)
is correct, but the card-text snippet describes the cast effect for a cycling action. Low-priority
annotation nit.

## N-134f (CORPUS-READER note, not a defect) reasoning-tax amplifier on control decks
deck134's latencies: medians ~17-24 s but frequent 90-207 s spikes (vs93 seq44 206964 ms; vs131
seq27 202594 ms; vs137 seq19 188305 ms; vs136 seq15 199229 ms). A control deck with many instants
and a large stalled board reproduces the wave-18 PRISON/attrition amplifier (more objects ->
longer replies). These spikes drove BOTH cap-adjudication losses (vs131 T18, vs136 T17 - both
winnable). Not an engine bug; recorded so a future reviewer does not read the two cap losses as
pure play failures - they are durdle + latency to the -T 3000 cap. The guide mitigates by
pre-answering the recurring derivations (Map/Flicker/role).

## N-134g (confirmation, no action) Expedition Map crack was OFFERED and PAYABLE when passed
The "search card with Expedition Map [cost: {2}, Tap, Sacrifice]" action appeared at bare priority
windows and the pilot passed it ~10x (vs18) with lands untapped - the engine offered a legal,
payable activation and the model declined. NOT an engine defect (the offer is correct); it is the
#1 model leak the guide addresses (skill.md Proposal 1). Recorded to pre-empt a re-diagnosis as a
representation bug.

## N-134h (construction flag, upward) deck134 may be a poor fit for an all-aggro pool
Flagged for the roster/deck-construction lane, not the guide: a glacial durdle-control deck with
essentially no clock (opp final life 20/20/16/19/32/20) in a 6-deck all-aggro pool, under cap
adjudication that it cannot win on life, is a structural underdog independent of pilot quality. If
the guided corpus does not move the record, consider whether the pool composition (or the -T cap)
is scoring this archetype unfairly rather than escalating the guide.
