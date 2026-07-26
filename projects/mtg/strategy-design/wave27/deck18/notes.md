# deck18 notes (engine/harness/model items + ROTATION verdict) - wave-27 FIRST-GUIDED

Layer-routing: engine/harness/model items with seq repros, NOT guide prose. deck18 is
FIRST-GUIDED this wave (guide = wave-26 initial deploy, unchanged). Rotation verdict at end.

## Status of wave-26 notes items

- N-18a (lord/anthem P/T re-derivation tax): RESOLVED at guide lane. The trust-line
  collapsed the re-derivation tail (p95 10,923 -> 7,112; >10k spikes 8 -> 3; see findings
  Leak 3 A/B table). No engine action; the "(printed X/Y)" render was already correct and
  is now trusted. Close as guide-validated.
- N-18b (flying not applied when blocking): RESOLVED at guide lane. The ground-only race
  line eliminated the illegal-flyer-block class at the exact matchup that produced it
  (vs137 Faerie: Step-0 9.9k illegal block -> wave-27 "no blockers", 1.1k). The optional
  ENGINE angle (annotate per-blocker legality against each attacker on the blockers prompt)
  remains a broad, multi-seat change - do NOT open on one seat; guide line is sufficient.
- N-18c (Mobilization activated-ability discoverability): EXERCISED, no confusion this
  corpus. The guide interface note pre-empted the Step-0 "no Activate button" spiral; the
  model used "Create soldier with Mobilization" fluently at priority (vs134 seq10; vs136
  seq9/11/13/16/21/22/25/26/28/29). Durable engine angle (surface an activatable permanent's
  ability in the same window as casts when payable) stays a cross-seat convergence item,
  tag single-seat, not opened.
- N-18d (card-script oracle-verify divergences - Zealous Guardian missing defender {W/U}
  1/1 vs real {W} 2/2 Defender; Thistledown Liege 1/3 vs real 4/4): UNCHANGED, flagged for
  a later oracle pass, NOT guide changes. The guide uses engine behavior (correct). No new
  divergence surfaced this corpus.

## NEW ledger item

- L-18-1 (MODEL, correctness-neutral tax, below-bar): whole-turn planning tax on trivial
  single-option decisions. vs137 seq12 "Play Plains" = 7,112 chars; vs136 seq17 "Play
  Plains" = 5,844. The model plans a full turn's sequencing on a forced/trivial land-drop
  prompt. Distinct from the (now-fixed) stat re-derivation class. Single-seat; convergence-
  tracking only. Repro: matchups-20260725-193608, ai_baka_deck18-*-vs-deck137 seq12 /
  -vs-deck136 seq17.

## d1 (R-DUPLICATE-NAME-INSTANCE) corroboration - engine render CORRECT at this seat

#N handles rendered and bound correctly across creatures (Cenn's Heir #1/#2), tokens
(Soldier #1/#2), enchantments (Mobilization #1/#2), and an adventure flyer (Faerie
Guidemother // Gift of the Fae #1). Model bound them without error (vs137 seq24 dual Cenn's
Heir attack; vs136 seq24 gang-block B1/B2/B3 -> 3 distinct attackers; vs136 seq28 "I have
two Mobilizations, one token per window"). deck18 = corroborating evidence #2 for d1 (deck93
is the 36-Rats systematic auditor). No engine action.

## ROTATION verdict: NEAR-TERMINAL -> ROTATE at wave-28

deck18 was Step-0 (guideless) at wave-26 and FIRST-GUIDED at wave-27 - this corpus IS its
validation corpus in the deck59 pipeline. Applying the rotation test unsentimentally:
- Guide-mod this wave? NO. strategy.txt is byte-frozen (cmp-verified == wave-26 == deployed).
- All three targeted leaks validated? YES (Dodger evasion, flyer-race off-case, trust-line
  tail collapse - all measured, findings Leaks 1-3).
- New work signal? NONE that demands a guide-mod. The one loss (vs136) is a STRUCTURAL
  weenie-aggro-vs-mono-B-removal+lifelink matchup, not a teachable leak. The only new item
  (L-18-1 whole-turn planning tax) is below-bar and cross-seat, not a deck18 guide-mod.
- 0 fallbacks at this seat; d1/N-18c/parser all clean.

Per the brief ("if all three edits validated and nothing new, say NEAR-TERMINAL and project
the exit; guide-mod only if a live defect demands it"): deck18 is NEAR-TERMINAL. No live
defect demands a guide-mod, so the guide is frozen. Projection: deck18 is a ROTATION
CANDIDATE for wave-28 (validation complete, new-work exhausted). Rotate it out unless a
wave-28 pre-read surfaces a fresh live defect; replace from the guideless roster (141).
