# deck134 -> development notes (wave 28). Corpus matchups-20260726-050449, binary ad8930961.

Layer routing: engine/harness/render/card-script/construction items (NOT guide, NOT core doctrine).
First-guided seat; seq repros included.

## ***CONSTRUCTION-TERMINAL FLAG -> ROSTER OWNER*** (the headline for this seat)
deck134 went 0/6 AGAIN with the guide live, AND the guide's teaches all VALIDATED independently of
the record (findings.md per-teach table): Map cracked at first payable window 6/6 (tron 3/3 in 4/6,
was 1/6 guideless); Mulldrifter hard-cast 100%; fogs/Pulse fired on trigger. The record did not
move and damage dealt stayed ~0. The residual losses are CONSTRUCTION, not decisions:
  (a) The Stonehorn+Ghostly Flicker lock - the deck's only hard anti-aggro plan and guide teach #2 -
      is an UNTUTORABLE 2-card combo. Across 6 games neither piece ever became a castable option;
      both only surfaced as library reveals from dig spells. Lock assembled in ZERO games.
  (b) No clock: the Mulldrifter flyers correctly block under aggro and never attack, so the deck
      deals ~0 damage (opp final life 20/20/20/22/17/18).
  (c) Under -T 3000 cap-adjudication a clockless control deck cannot win on life; every survival-
      to-cap while behind (vs146 T18, 4 vs 17) is scored a LOSS.
ROSTER-OWNER DECISION (not the reviewer's call - flagged with quantification):
  - ROTATE deck134 OUT: the guide instrument reached its ceiling in ONE corpus; it is a
    conversion-impossible seat in this pool (a terminal category distinct from deck18/deck59
    new-work-exhaustion - there the guide wins; here the guide works and the deck still cannot).
  - OR construction change: give it a real clock, lock redundancy, or a tutor for Stonehorn.
  - OR pool/format change: add a non-aggro seat, or stop scoring clockless-control-to-cap as an
    automatic loss.
ROTATION VERDICT (per the wave-28 rotation rule): deck134 IS a rotation candidate - NO guide-mod is
warranted (no teach FAILED for guide-fixable reasons) and the only new-work signal is engine/
construction (not guide work). The rule's clean "new-work-exhaustion" case does not quite fit (the
residual work exists, it is just NOT guide work), so the honest routing is: rotate OR escalate
construction - roster owner picks. Guide track EXHAUSTED after one corpus.

## N-134b (RENDER, LOW) Moment's Peace flashback option shows the wrong cost number - REPRODUCED
The graveyard-cast option renders "Cast Moment's Peace {1}{g} [from your graveyard]" (vs146 seq24,
seq25) - i.e. it shows the {1}{G} BASE cost, not the {2}{G} flashback cost the engine charges.
Carried from wave-27 N-134b; reproduced this corpus. Display-accuracy item, harmless to play
(still cheap). Verify the cost actually charged; if it charges flashback but displays base, the
label under-states the cost.

## N-134d (SEAM / WATCH) Mulldrifter double cast-mode menu - CONFIRMED (clean repro)
vs136 seq24 top-level "Cast Mulldrifter {4}{u} (2/2)" THEN seq25 "Cast Card Normally" sub-menu -
two adjacent asks for one cast. Repeated at seq29 (top) -> seq30 ("Cast Card Normally"). So the
top-level mode option AND a following normal/evoke sub-menu BOTH fire for the same card: Mulldrifter
is a two-step (top-menu mode THEN normal/evoke sub-menu). This wave both steps were CONSISTENT
(hard-cast then "Cast Card Normally"), so it was benign - unlike wave-27 vs131 seq12/13 where the
top-menu hard-cast then the sub-menu picked evoke = self-contradiction. The redundant double-ask
SEAM is confirmed; de-dupe candidate (pick the mode once). Low urgency now that the guide keeps the
two steps aligned, but the contradiction path still exists if the model answers the two asks
differently.

## N-134f (CORPUS-READER note) reasoning-tax amplifier PERSISTS on this control deck
Latency spikes 90-211 s recurred: vs18 seq15 211s (blockers), vs93 seq13 161s, vs146 seq22 189s /
seq24 202s / seq25 197s, vs137 seq10 125s. Worst in vs146 - the longest durdle (T18 cap) with the
biggest stalled board. The guide's pre-answering (Map/Mulldrifter/fog) cut tax on THOSE derivations
but the counter/dig/durdle decisions still spike, and the spikes still pushed vs146 to cap
adjudication. Not an engine bug; recorded so the two cap-adjacent losses (vs136 T15, vs146 T18) are
not re-read as pure play failures - they are durdle + latency to the -T 3000 cap, on a deck that
cannot win at the cap regardless.

## N-134g (confirmation, no action) fog-on-own-turn slip
vs136 seq39: cast Moment's Peace on its OWN Main phase 2 (T14 L6, no combat pending) = wasted. A
single model-execution slip against a correct guide instruction ("leave {1}{G} untapped on THEIR
turn"). Non-converting (the deck loses either way). Recorded so it is not mistaken for a guide
defect - the guide text is correct; the model slipped once under end-game reasoning tax.

## e-batch / d-c-batch steady state at this seat (all CLEAN)
- e4 land identity tags: rendering correctly and WITHOUT noise - "(land: taps for {U}{G})",
  Unknown Shores as "(land: taps for {W}{U}{B}{R}{G})". Accurate, no prompt confusion.
- e3 'Flip Side' pseudo-action: NO sighting (no DFCs in this matchup set).
- e5 annotation-echo: the one retracted_choice (vs146 seq25) is NOT the echo class - the CHOICE
  line matched option 1 exactly with no bracketed-tail mismatch; it was a genuine no-legal-effect
  retraction (fog on own main phase 2). e5 holding at this seat.
- Parser/protocol: 0 unparsed/empty replies across 141 decisions. Retry 0.

No parser precedence contradictions observed at this seat this corpus.
