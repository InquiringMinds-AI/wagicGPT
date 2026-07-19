# deck62 notes -- engine/harness items + rotation verdict (wave 20)

Seat: deck62 (Enchantresses), FOURTH guided corpus (revised-guide round 4). 152 decisions (ask 114,
attackers 24, blockers 14) across 6 games. Record 3/6 (wins vs102, vs14, vs27; losses vs135, vs49, vs35 --
all matchup floors). Fallbacks 7 = 4.6% (2 blockers, 4 ask, 1 stale_echo): vs14 s26 (blockers), vs135 s19
(blockers), vs135 s22 (ask), vs35 s17 (ask), vs14 s34 (ask), vs49 s18 (ask), vs27 s5 (stale_echo).

## Carryover checks (the brief's explicit asks)
- **Blanchwood recount-ban: HELD DEAD into round 4.** Tight-signature grep = 0 recount-of-printed-P/T. All
  16 Blanchwood mentions are prospective cast-value math (unbannable). Retired watch stays retired.
- **Computed-P/T distrust pocket: DEAD** (same evidence). Fourth consecutive corpus clean.
- **A-list-only attacks: ZERO attacker fallbacks, ZERO A-list unparses, ZERO 0-attacker durdles.** Every
  "no attackers" had opts=1 (empty eligible set). The engine name-tolerance + the guide A-list line remain
  belt-and-suspenders; neither was even stressed this corpus (no over-naming observed).

## Wave-20 engine-batch live validation (this seat's mandate)
- **Salvage extension (BLOCKS/ATTACK/PUT): validated NEGATIVE-CORRECT.** Both blockers-class fallbacks
  (vs14 s26, vs135 s19) truncated PRE-COMMITMENT -- vs135 s19 emitted no `BLOCKS:` line at all; vs14 s26
  had only `BLOCKS: none` (contradicting its stated intent). Nothing to salvage; the extension correctly
  did not fire. These are truncation-before-commit, NOT the mis-formatted/looped-choice shape the extension
  targets. Not a defect in the extension; a different class (decode/length) sits above it. See skill.md #1.
- **Duplicate-name / #N disambiguation: VALIDATED WORKING.** vs14 s36 the model named two bare "Yavimaya
  Enchantress" against a list with two distinct Yavimayas (A3 7/9, A4 7/7); engine mapped both, all three
  named attackers swung. vs135 s25 two Saprolings both blocked the same attacker, resolved. No #N ordinal
  even needed at this seat.
- **Combat options_text: VALIDATED PRESENT.** All deck62 blocker AND attacker records carry the A#/B#
  annotation lines. The wave-19 deck14 observability gap is closed at this seat.
- **Trade-outcome annotations: still accurate** (no mislead across 14 block decisions).

## Engine / harness / representation items (NOT guide prose)

- **N8 (ENGINE, MEDIUM, NEW) -- baka blocker-fallback sacrifices the 0/1 shroud engine.** When a block
  reply unparses -> heuristic, the baka blocker fallback gang-blocks a small attacker with the 0/1
  Argothian engine even when a single real blocker (Yavimaya) already kills it. Confirmed vs14 s26 (2/2
  Lord gang-blocked by two 1/2 Argothians, one Argothian lost; game won anyway) and vs135 s19 (2/2 Golem
  gang-blocked by Yavimaya 4/6 + Argothian 0/1, Argothian lost for nothing; game a floor). This is the
  behavior edit B forbids, but it lives in the HEURISTIC below the model's answer -- guide prose cannot
  reach it. Fix: the blocker heuristic should prefer non-0/1/non-shroud blockers and not add a second
  blocker to an already-dead attacker. Cross-seat (any deck with a valuable 0-power creature). See
  general-suggestions #1.

- **N7-template (HARNESS, LOW, NEW) -- literal answer-template parroted alongside a real choice.** vs49
  s18: reply reached "CHOICE: 1 (Dragonmaster Outcast)" but also emitted the literal placeholder "CHOICE:
  [Number] ([Name])" (x2) + a contradictory illegal "CHOICE: Argothian Enchantress" -> unparse. Drop
  template-placeholder CHOICE lines (`[Number]`/`[Name]`) before disambiguation; prefer the last in-range
  well-formed `CHOICE: N (Name)`. Cross-seat (template lives in the shared system prompt). LOW, floor game.
  Related to but distinct from wave-19 N7 (ATTACK-line-to-a-CAST-ask): both are cross-menu / stray-line
  reply-protocol leaks -- vs27 s5 this wave is the wave-19-N7 sibling (a "CHOICE: 1 (Play Forest)"
  land-drop echo answered inside a cast menu -> stale_echo, routed correctly to heuristic).

- **N6 (model/decode watch) -- REFRAMED, not retired.** Wave-19's dominant unparse driver was
  decode-REPETITION loops (4/7). Wave-20 the driver shifted to ADVANCING-spiral / over-deliberation
  truncations (5/7); only vs14 s26's tail is a true repetition loop, and it was fueled by a guide
  near-contradiction. One new sub-shape worth naming: over-deliberating a SETTLED decision (model reaches
  the right answer, keeps deliberating downstream turns, truncates before the CHOICE line -- vs35 s17). A
  guide "decide and answer, do not keep going past your conclusion" nudge MIGHT reach it (low-confidence).
  Route to the model/decode watch; not guide-addressable with confidence.

- **N-redundant-global (representation, LOW, re-filed).** vs35 s17 the model correctly reasons a 2nd Primal
  Rage and Gaea's Anthem are redundant (guide note obeyed) but still enumerates them exhaustively inside
  the over-deliberation. An engine tag on a hand card whose global effect is already active
  ("Primal Rage (already in play - a 2nd copy does nothing)") would shorten the enumeration. Cross-seat.

- **N2 / N3 (CLOSED / RETIRED).** N2 (name-tolerance) shipped wave-19, validated; not stressed this corpus.
  N3 (P/T distrust) stays retired.

## Guide edit shipped this wave (round 4, surgical, ONE edit)
- **(D) Scope-sharpened edit B's defensive protect-engine clause** -- added a decisive exception: the "do
  not chump" caution is ONLY about the 0/1 Argothian/Birds; a favorable block with a REAL creature (prompt
  tag "(you kill it, your blocker lives)" / "(both die)") is a trade, not a chump -- make it and answer, do
  not re-litigate. Aimed at the vs14 s26 guide-caused decode loop (echo-test positive). Purely additive
  (freezes all of edit B's active tokens); freeze-check clean (only diff is +5 lines after line 98).
  Low-confidence framing (the carrier is a decode loop; the edit removes the fuel, not a claimed fix).
  Everything else BYTE-FROZEN (auras-draw/pump, enchant-gift ban, Argothian-first, trust-printed-P/T,
  attack-every-turn, A-list, edit A, edit C).

## Rotation verdict
**NOT a rotation candidate -- GUIDE REVISED (surgical, round 4) + NEW-WORK-SIGNAL present.** Guide-mod =
YES (edit D). New-work-signal = YES and durable: (a) edit D needs a next corpus to confirm it stops the
favorable-block spiral without new harm; (b) N8 (blocker-fallback engine-sac) is a NEW engine item needing
a fix + validation; (c) the unparse-driver shift (repetition-loop -> advancing over-deliberation, incl. the
settled-decision variant) needs another corpus to characterize as persistent or corpus-specific. deck62
fails BOTH rotation gates and STAYS in the pool. Wave-21 must validate that (1) edit D shortens/prevents the
favorable-block litigation, (2) whether N8 was fixed and the engine stops sacrificing the 0/1 engine on
fallback blocks, and (3) whether the over-deliberation-of-a-settled-decision unparse is a persistent class.

## Engine items count: 3 open/active (N8 blocker-fallback engine-sac NEW MEDIUM; N7-template harness NEW LOW;
N-redundant-global representation LOW) + N6 model/decode watch (reframed) + 2 closed/retired (N2, N3).
