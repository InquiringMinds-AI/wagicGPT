# Deck-135 wave-10 — development notes (engine / harness / model / construction)

Run: PRIMARY `matchups-20260715-031825`. deck135 jsonl epochs + opponents in findings.md game->file map.
Doctrine: win column context-only; layer-routing enforced (engine items live HERE with repros, never in
prompt/guide text). Each item self-contained with seq repros.

## ENGINE LEDGER

- **ENGINE #1 (HIGH PRIORITY) — Force of Negation's FREE ALTERNATIVE COST never counters; the spell
  resolves anyway and the pilot bleeds blue cards.** This closes the carried "free-alt-cost side of Force
  of Negation" watch as a CONFIRMED engine defect (not a strategy misfire), witnessed in TWO seats.
  REPRO A (deck131, seat 1784103512): s40 T11 Force cast on opponent's Downsize targeting the 10/10
  Abominable Treefolk -> EVENTS s41 `Opponent's Downsize: stack -> graveyard` and Treefolk rendered 6/10
  (Downsize RESOLVED, power reduced, NOT countered). s41-44 Force on Elixir of Immortality -> EVENTS s42
  `Opponent's Elixir of Immortality: stack -> battlefield` (RESOLVED) while `You: exile a blue card from
  hand with Force of Negation` fires at s42/s43/s44. s51-52 Force on Cyclonic Rift -> EVENTS s52
  `Your Abominable Treefolk: battlefield -> hand` (Rift bounced it) + `Cyclonic Rift: stack -> graveyard`
  (RESOLVED). Seat total: 3 Force casts, 0 counters, 7 `exile a blue card` cost-actions.
  REPRO B (deck140, seat 1784108897): s58 T21 Force on Rakdos's Return -> EVENTS s59
  `Opponent's Rakdos's Return: stack -> graveyard | Your Force of Negation: hand -> graveyard | Your life
  -6 (now 7)` — Rakdos RESOLVED for 6 to the face, Force to graveyard, no counter.
  MECHANISM (two coupled faults): (1) the Force option renders a CONTRADICTION —
  `Cast Force of Negation with its exile a blue card from hand cost  - NO legal target right now - can
  target on the stack: <SpellName>`. The wave-9 no-legal-target annotation (built for permanent-targeting
  removal) is being applied to a COUNTERSPELL, whose target is a spell on the stack, so it prints "NO
  legal target right now" even when a counterable noncreature spell IS on the stack. (2) With no target
  registered, the Force resolves as a pure cost-payment (exile blue card) and the noncreature spell
  resolves. FIX DIRECTION: (a) Force's counter-target resolution must bind the stack noncreature spell as
  its legal target (the `can target on the stack: X` hint already knows it exists — the two code paths
  disagree); (b) suppress the "NO legal target right now" annotation for counter-type options entirely —
  it is the wrong annotation class for a spell whose targets are on the stack, and it is causing large
  reasoning-tax (deck140 s58 reply ~470 words of "can Force exile itself?" oscillation). DETECT
  REGRESSION: a Force cast where the targeted stack spell's EVENTS show `stack -> graveyard/battlefield`
  (resolved) rather than a counter, and/or the option text carrying both "NO legal target right now" and
  "can target on the stack". NOTE the interaction with the guide: the deck135 guide actively directs
  Force onto board wipes; until this is fixed, that guidance pitches the blue cards the deck needs — the
  guide is intentionally NOT changed (layer-routing), so this fix is on the engine's critical path.

- **ENGINE #2 (minor) — Force offered when the only blue card in hand is Force itself (unpayable alt-cost
  offered).** deck131 s26/s34, deck140 s58: the engine offered Force via its "exile a blue card" alt-cost
  while the pilot held no OTHER blue card (only Force). Correct MTG requires exiling a DIFFERENT blue card;
  the pilot repeatedly (and correctly) reasoned it could not pay and declined (s26/s34) — a violation of
  the "offered = payable" interface contract. Coupled to ENGINE #1 (Force's alt-cost path is generally
  mis-modeled). Low harm this corpus (the declines were on non-qualifying card-draw spells anyway) but it
  wastes reasoning budget every opponent turn a Force sits in hand. Fold into the ENGINE #1 fix.

- **ENGINE #3 (minor residual) — a fetch-crack CHOSEN but not yet RESOLVED is re-offered across a few
  windows.** The consume-on-choose fix WORKS overall (fetch-crack option windows fell to 66 total /
  ~11/game from the wave-9 control's ~21.3/game; cracks fire and resolve freely; crack-for-missing-color
  works both ways). Residual: deck44 (seat 1784108791) s5/s6/s7 chose `Put in Play with Misty Rainforest`
  three times (targets flipping Island/Forest/Island as the board changed) before it resolved at s8
  (`Misty Rainforest: battlefield -> graveyard | Snow-Covered Island: library -> battlefield`, `life -1`).
  Only ONE crack resolved, ONE life paid, ONE land fetched — so this is DECISION-COUNT inflation (3 windows
  for 1 crack), not resource loss. If consume-on-choose is meant to withdraw the crack the instant it is
  CHOSEN (before resolution), it is not doing so for a pending activation; if it consumes on RESOLVE, this
  is expected and low-impact. DETECT: consecutive `Put in Play with <fetch>` / `search basic land with
  <fetch>` chosen records with no intervening land `library -> battlefield`.

## HARNESS

- **HARNESS #1 — `tools/intent-collapse-metric.py` "reversal + concluded-option != recorded choice" count
  is unreliable (index-offset false positives).** On the deck135 seat dir it reports 30 such flags; on
  manual read ALL 30 are false positives — the recorded CHOICE equals the plan's FINAL concluded option in
  every case (the `plan_concluded_option` matcher returns 0-indexed guesses and matches intermediate plan
  mentions, so it fires on off-by-one and mid-plan asides; e.g. deck109 s18 concluded `[0,1]` but the true
  final conclusion and CHOICE were both option 4 = Icehide Golem). The RELIABLE signals are
  `non-numeric heads` (246/246 = the answer-last signature) and `last-bare-int differs from head = 0`.
  RECOMMENDATION for synthesis: use the reversal-mismatch flags only as pointers to READ the reply, never
  as a defect count; the head-structure counts are the metric's trustworthy output. (The tool also only
  accepts a DIRECTORY arg and globs `*.jsonl` inside — passing files yields "total records: 0"; pass a
  dir of the seat's files.)

- **HARNESS #2 — unparsed_reply fallbacks (2048-token cap): 11 in this seat, all self-healing, none
  changed a game.** deck131 s25/s47, deck133 s10/s12/s25, deck44 s34, deck140 s30/s61, deck110 s11/s23/s33
  (deck109 0). Spot-checked the closest game (deck133, adj loss BY ONE life): s10 (Into the North land
  pick) fallback still fetched a Forest (EVENTS s11 `Snow-Covered Forest: library -> battlefield`); s12
  fallback passed and s13 cast Search + Golem — no misplay attributable. Consistent with the brief's
  4096-token bump already shipped; noting only that the adj-loss-by-one is where recovered tokens matter
  most, so the bump is well targeted.

## MODEL (route to model-experiments; NOT guide text)

- **Uncastable-plan / mana-count HALLUCINATION — the pilot plans an UNOFFERED bigger creature it believes
  is castable, and under-develops.** deck44 s12 (plans unoffered Treefolk, Coatl WAS offered, casts
  nothing at MP1, casts Coatl at MP2 — self-corrected); deck110 s19 (plans unoffered Treefolk citing "my
  two Arcum's Astrolabes", casts nothing instead of Search/Into the North — dug next turn, won); deck109
  s35/s36 (plans unoffered Coatl claiming an untapped blue source while `Mana available: {g}{g}{g}` and
  Coatl is NOT offered — lost position). The option list already surfaces uncastability and the guide
  already carries the corrective anchors (Rule #1 point 1 "read the OPTION LIST, not your hand"; KEY CARDS
  Treefolk/Coatl; DECIDING SITUATIONS "plan says cast Treefolk ... but NOT in your cast list"); the pilot
  fabricates a mana count over all of it = the representation-beats-instruction wall. KEEP anchors (class
  still leaking -> not demotable); no 3rd copy. This is the mirror of the colored-pays-generic false
  belief (there it under-counts payability; here it over-counts it) — both are the model's shaky mana
  arithmetic under a long derivation, and the durable lever is model-side (or an option-line
  affordability annotation, which the engine already partly does via offered=payable).

- **"colored mana can't pay generic" false belief (deck110 s21) — routed to CORE candidate.** See
  general-suggestions.md OBSERVATION 1 (deck-agnostic; single-seat/2-waves; cross-seat grep = promotion).

## DECK-CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)

- **Manabase strands the deck's own 2-color win creatures — STANDS.** Ice-Fang Coatl {G}{U}, Ohran Viper
  {1}{G}{G}, Abominable Treefolk {2}{G}{U}, Diamond Faerie {2}{G}{W}{U} are gold/double-pip off a base
  with only 2 Snow-Covered Island + 2 Snow-Covered Plains + fetches. vs109 (LOSS): green/blue-tight, 1
  creature attempted late; vs140 (LOSS): a 23-turn Azcanta-grind where the bodies (1/1-3/3) never out-race
  a 36-life Wipe-Them-Out. The pilot-side underuse is now addressed (develop-branch validated); the
  construction ceiling remains. Direction (user's call): raise effective double-pip sources, or shave a
  pip off the creature suite.
- **Slow clock is structural.** Abominable Treefolk is the only real closer and lands T6-T11; small bodies
  can't race lifegain (vs140 opp to 36; vs133 drain grind to a 1-life adj loss). Construction, not guide.
- **vs110 marginal keep — no longer a datum this wave.** The comparable blue-clog keep WON (adj0 16-7);
  the wave-9 low-priority mulligan watch is not reinforced this corpus. Leave the mulligan rule FROZEN.

## FREEZE-CHECK (turnaround / OBEYED-BUT-LOSING mode; guide is a BYTE-IDENTICAL copy of the live guide)

The guide was NOT edited (cmp: byte-identical). No new guide-fixable class was found — every residual
routes to engine (Force, fetch), core (colored-pays-generic), model (uncastable-plan), or construction.
Every wave-9 line survives verbatim; where each stood this corpus:
- **RULE #1** — point 1 (creature on sight) OBEYED across all games (Golem/Coatl/Viper/Treefolk/Druid/
  Faerie taken when offered); the deck44 s12 / deck110 s19 slips are the uncastable-plan model residual,
  covered by the same block's own anchors. Point 3 develop-branch (option-list gate + fetch-crack-for-
  missing-color + offered=payable) VALIDATED — vs133 dead-board class died (findings Q5). Board-read
  anchor: 0 phantom-lethal windows this corpus = UNTESTED, so KEEP at full prominence (pause-aware: do not
  demote off absence). FROZEN verbatim.
- **RULE #2 + WHO IS THE BEATDOWN** — combat healthy (profitable blocks vs109; Treefolk pressure;
  Boreal-Druid carve-out held). FROZEN verbatim.
- **MANA** — fetch-crack clauses (Island short-blue / Forest short-green) exercised and obeyed. FROZEN.
- **KEY CARDS — Force of Negation** — the NEVER-list (face-burn 0; discard-exclusion OBSERVED OBEYED once
  at deck133 s12) held; the section is strategically correct but the CARD is engine-broken (ENGINE #1).
  Deliberately NOT changed (layer-routing). FROZEN verbatim.
- **KEY CARDS — Treefolk/Coatl uncastable anchors** — DISOBEYED at deck44 s12 / deck110 s19 / deck109 s35
  (still leaking) -> KEEP at full prominence, no demotion, no 3rd copy. FROZEN verbatim.
- **MULLIGAN / LETHAL CHECK / DECIDING SITUATIONS** — no counterexamples; the vs110 marginal-keep watch
  did not reinforce (won this corpus). FROZEN verbatim.
No frozen line dropped, moved, or demoted. Guide diff = zero bytes.

## VALIDATION TODO (for the next run)
Confirm after the Force engine fix (ENGINE #1): a Force cast on a board wipe / mass bounce actually
COUNTERS (target EVENTS show a counter, not `stack -> graveyard/battlefield` resolution) and stops
bleeding blue cards; the "NO legal target right now" annotation no longer appears on counter options.
Confirm the colored-pays-generic decline (deck110 s21 class) drops to 0 if the core sentence ships.
The record is construction-bound — do not read a flat win-rate as a guide regression until the manabase
flag is addressed.
