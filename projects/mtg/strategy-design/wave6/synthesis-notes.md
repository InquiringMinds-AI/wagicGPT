# Wave-6 synthesis notes (development notes for step 4 "review and address")

Corpus: one round-robin run, 6 games/deck across decks 44, 109, 110, 131, 133, 135, 140. n=6/deck,
single run — win-rate is noise; behavior metrics are the signal. Adjudicated records (life-at-cap rule):
44 3/6 · 109 5/6 · 110 3/6 · 131 1/6+draw · 133 3/6+draw · 135 2/6+draw · 140 2/6+draw. 6/21 games were
timeouts, mostly latency/grind-bound (60-80s Spark spikes under -j3). This file carries the cross-deck
convergences, the ranked engine/representation ledger, the timeout/harness-knob recommendation, and what
wave-7 inherits. The skill and `general-strategy.txt` (unchanged copy) are the other deliverables;
`core-prompt-revision.md` documents the NO-core-edit verdict.

## Cross-deck convergences (the strongest merge signals)

1. **A shipped representation fix is a HYPOTHESIS until it FIRES — the wave's headline.** Same diagnosis,
   opposite outcomes, told apart only by grepping the corpus for the marker string:
   - **deck44 stack surfacing = the clean WIN.** The counter seam went 2/27 → 13/14, ZERO hallucinated
     resolutions, sweeper-first obeyed, and the two sweeper-loss matchups (vs140, vs110) FLIPPED to wins.
     End-to-end proof (bug → route → ship → win) that a first-class CURRENT SITUATION line beats louder
     wording. The wave-5 counter-tell retirement is a CLOSED, validated prediction.
   - **deck110 re-equip marker = shipped-but-INERT.** The "(ALREADY attached)" marker checks `auraParent`
     while equipment stores its host in `source->target` (the engine explicitly excludes TYPE_EQUIPMENT
     from `auraParent`), so the string appeared **0 times** and the no-op rate was unchanged (9/18, 50%).
     A shipped-but-inert fix looks identical to a shipped-but-weak fix in the win column; the remedy is
     opposite. → SKILL Step-0 addition (verify the marker fired). **The fix is landing for wave-7** (also
     test the equipment host field) — wave-7 authors verify it fires before crediting it.

2. **A #1 rule can UNDER-fire because the guide's OWN sibling rule forbids it (intra-guide contradiction).**
   deck140's "HELD FOR LETHAL" Rakdos rule was disobeyed 6/7 — VETOED by a cast-nothing clause that
   permitted no "hold" output for a non-sweeper burst. The executor resolves the contradiction toward the
   rule that NAMES THE OPTION IN FRONT OF IT: concreteness beats prominence (the intra-guide mirror of
   representation-beats-instruction). → SKILL Step-0 addition. Adjacent: **removing an engine guard
   resurfaces the behavior it masked** — 353f1a150 lifted the "2+ creatures" hint-veto and the latent
   "board wipes hit artifacts" false belief surfaced instantly (~7/22 sweeps into artifact boards). →
   SKILL Step-0 addition.

3. **Threat-before-value (wave-5 core Edit 2) CONFIRMED on 3 seats** — deck135 (8/10 conflicts to the
   creature, non-overshooting on its origin deck), deck133, deck131 (every value cast was a no-threat
   window). Keep verbatim.

4. **Transport layer CLEAN across all 7 seats** — 0 head-vs-choice desyncs corpus-wide (deck140 0/295,
   deck110 0/239, deck131/133/135/44/109 all clean), 0 cast no-ops in all 7 seats, near-0 fallbacks
   (~2/2035). Head-first parser fully validated; `options_text` now emitted per record (schema ask
   fulfilled). Do NOT revert.

5. **Reflexive high-life blocking — 2 seats, routes to REPRESENTATION not core.** deck109 wave-5 +
   deck44 wave-6 reproduce it; deck135 + deck131 CLEAN; deck110 N/A. Both witnessing seats agree: the
   core floor exists and the pilot disobeys at distance → surface life-vs-lethal at the blocker seam,
   keep per-deck floors, NO new core sentence, NO "racing creatures don't block" promotion. deck109's
   wave-5 per-seam-positive-floor prediction was VINDICATED (4 high-life blocks → 0). See
   `core-prompt-revision.md` item 1.

## Engine / representation ledger (converged; ranked by breadth × cost)

1. **[TOP — DECISIVE, fix landing for wave-7] Re-attach no-op marker INERT for equipment.** deck110 E1
   (primary witness): `AIPlayerGPT::describeAction` (`src/AIPlayerGPT.cpp:1155-1156`) gates on
   `auraParent`, which is permanently NULL for equipment (`AEquip::equip` sets `source->target`;
   `enchantmentStatus` excludes TYPE_EQUIPMENT). 9/18 no-op re-equips, unchanged from wave-5. **Fix (one
   line):** also test the equipment host field —
   `if (moved && (moved->auraParent == action.target || moved->target == action.target))`. Also add board
   lines `[attached to: host]`. This is the wave-7 A/B the wave-5 attachment-marker item promised; it
   ALSO defuses the two-power-number heuristic's false positives (lords/+1+1 counters/battle cry all
   render two numbers with no equipment attached).
2. **[HIGH — menu-contract violation, probe filed] Targeted spell offered with NO legal target** (deck44
   F1). `Cast Go for the Throat` offered on boards with no legal nonartifact creature; 9 stderr
   `fails validation; deferring to heuristic` across 2 games; the pick was silently dropped to a
   heuristic pass (a VALIDATION-DROP — chosen != happened, JSONL-invisible). Deck-agnostic: any deck with
   targeted removal/burn/counters can hit it, and it violates the prompt's universal "the game has
   already checked your... targets" contract. **Fix:** at option-build, suppress "Cast X" (or mark
   `[no legal target]` and reject the pick) when the target set is empty. Closes both the wasted-decision
   loop and the data corruption.
3. **[HIGH — converged 3+ seats, carried] Fetch/activation re-offer loop.** deck135 (Flooded/Strand
   re-asked across phases; feeds a timeout), deck133 (single-option priority re-ask inflates decision
   count: vs140 81 priority records in a 105-record game; vs131 20), deck131 (vs133 grind), deck44 wave-5
   (Sleep-Cursed untap 29×). The life payment / land arrival lands only on the LAST answer of a run.
   **Fix:** suppress a re-ask when an identical option set was answered within the same turn and only the
   phase changed (a within-turn "declined this exact single-option offer" memo). Highest inference-budget
   / timeout drain in the pool; factor it out BEFORE reading timeout / decision-count columns.
4. **[MED — 2 seats, NEW this wave] Reflexive-high-life-blocking representation.** Surface the
   life-vs-lethal number at the blocker seam: `Your life: N (unblocked this turn: D -> you would be at
   N-D)` or a `[not lethal]` marker when incoming ≪ life. deck109 + deck44. Same shape as the ON THE
   STACK win. Keep per-deck floors as interim.
5. **[MED — carried, still live] Gray Merchant drain display under-counts by 2.** deck133 E2,
   re-confirmed vs44 (shown 10 → actual 12): `{right now: drains N}` omits Gray Merchant's own {B}{B}=2
   devotion because he is still in hand when the option is built. **Fix:** compute the displayed drain
   as-if-resolved. Removes the guide's "real drain = shown N + 2" stopgap at once.
6. **[LOW — carried] Mana-ability mislabel** ("Cast Card Normally with Mox Opal / Mountain" reads as a
   spell cast; Collective Brutality escalate menu emits malformed/duplicated-index lists). Relabel to
   "Tap X for one mana"; clean up the modal assembler.
7. **[LOW — carried] Menu noise:** {B}{B} attach line with no black source (deck110 E2); empty
   `[counters:]` bracket on Sleep-Cursed Faerie at 0 stun counters (deck44); menu-adjacency index
   carryover (deck140 P2, immaterial). Bundle with #1/#6 menu cleanup.
8. **[CHEAP — carried] Turn-ownership stamping** on the choice-list header ("YOUR Main Phase 1") —
   deck135 E2. One narration change.

**NOW-SHIPPED / closed:** stack surfacing + labeled counter options (deck44, VALIDATED 13/14), X-menu
reversal (deck140, 7/7 max-X held), head-first parser (all seats), `options_text` per-record array
(deck140 confirms — closes the biggest schema ask), Hellrider cast no-op (353f1a150, 0 no-ops all 7
seats). The re-equip marker is shipping the equipment-host fix for wave-7 (item 1).

## Translog schema asks (still open, converged)

- **Per-kind decision counts + fallback count in a `gameend` record** (deck140, deck133, deck131).
- **`realized` magnitude field on X/drain asks** (deck140) — turns the X-regression check into a
  one-liner.
- **`reask_of: <seq>` marker** when the same activation repeats (deck135) — makes the fetch re-offer loop
  measurable without prompt-scraping.
- **`gameend` / results.tsv final-life on TIMEOUT rows** (`end_reason` stall/latency/cap, `final_life0/1`,
  `turns`) — timeout games currently write NO closing record, forcing every reviewer to re-derive
  win/lose/draw from the translog.
- **Per-seat game-id / opponent / result header** (deck44, deck131) — stop re-deriving seat→game→outcome
  by epoch arithmetic + opponent-card signature.
- FULFILLED this wave: `options_text` per record.

## Timeout adjudication + harness-knob recommendation for wave-7

Adjudicate capped games by life-at-cap (ahead → WIN, behind → LOSS, static → DRAW); latency-starved
games (both live, low turn, high latency) are infra, not play defects — exclude/re-run. 6/21 timeouts
this run were latency/grind-bound. Control mirrors (deck140/deck133) generate 2-4× the decisions of aggro
games, so a fixed wall-clock cap times them out regardless of play.

**Recommendation: -j2, not -T 3600.** The 60-80s Spark spikes are the -j3 concurrency artifact (vs a
~4-5s solo baseline); they bind outcomes before any model-quality question. Raising the cap (-T 3600)
treats the symptom — it lets latency-starved control mirrors finish but does NOT reduce per-decision
cost, and it lets genuine grind-stalls burn MORE inference budget. Pinning **-j2** reduces Spark
contention → lower median/spike latency → fewer latency-starved timeouts AND cheaper games; for a
decision-QUALITY corpus (which this is), latency fidelity beats throughput. Pair -j2 with the two cheap
harness fixes so genuine stalls end cleanly rather than eating wall clock:
- **Stalemate detector:** end as `draw`/`end_reason=stall` after ~8 consecutive full turns with no change
  in life totals, battlefield card count, and hand sizes (deck140).
- **`gameend`-on-cap** with final life (above), so timeouts stop costing every reviewer a translog dive.
Ideal long-term: cap by decisions consumed (cap = N_decisions × budget) so control archetypes aren't
penalized for decision density. Primary lever for wave-7: **-j2 + stalemate detector + gameend-on-cap.**

## Deck-construction flags (for the user / decklist owner)

- **deck44** — stack fix flipped 2 matchups; counter seam now healthy (13/14). Residual: 6 reactive cards
  clog a "kill by turn 10" flyer beatdown; manabase skew (16 Islands / 10 Swamps vs a black-heavy suite)
  still color-screws. Trim counters toward cheap flyers; rebalance mana. Also: over-countered a 1-mana
  Astrolabe cantrip with Arcane Denial (draws THEM 2) — per-deck counter-value floor added.
- **deck131** — #1 flag UNCHANGED: 14 Island / 8 Mountain but every win-con is RED; threat density 6/60.
  OBEYED-BUT-LOSING (1/6, obedience high). Manabase dominates even a perfect mulligan fix (a no-red 7→6
  still misses red). Move ~3-4 Islands → Mountains; 4 Prism Ring is the flex to cut.
- **deck133** — turnaround 3/6 rising (wave-5 guide working). Gray Merchant density 4/60 + no tutors =
  castability ~half the games; it is the only blocker-independent damage AND the stabilizer. Consider
  +1 Merchant. 8 one-mana discard spells (4 Thoughtseize at 2 life) are self-damage vs a racing pool.
- **deck135** — 1/6 → 2W-2L-1draw-1incomplete; the displacement fix landed. Standing manabase tax (2
  Snow-Islands / 2 Plains) still strands {U} cards and Diamond Faerie ({W}{U}{G}); a 3rd Island over a
  2nd Plains defuses both. Diamond Faerie is arguably mis-slotted for the GPT pilot.
- **deck140** — reach starvation is THE residual (well-bounded): 14 sweepers (PROVEN right — do NOT cut),
  ~2 bursts (Rakdos, now conserved), 2 Blightsteel {12} land too late, 4 Staff of Nin = 14-20-turn clock.
  "Add teeth, not text": a 3rd/4th X-reach finisher, a cheaper repeatable drain, or a {4}-{6} recursive
  attacker. The pilot now plays it well (X 7/7 max); the clock is the ceiling. Slowest closer in the pool.
- **deck109** — well-formed RDW, 5/6, no changes. The 133 loss was deck-construction (opp dead-adjacent
  at 6-7, obedience metrics all healthy). Hellrider "no-op" fully explained + fixed (353f1a150).
- **deck110** — 5/6 → 1/6 raw / 3/6 adjudicated = VARIANCE + META SHIFT (the surfaced stack revived
  opponents' dead counters against affinity's discrete threats), NOT a guide bug. Freeze, do not rewrite.
  Toughness fragility (every creature T1-2; Cranial Plating pumps power only) is a hard RPS weakness. The
  re-equip marker fix (ledger #1) is its single highest-leverage change.

## What the next wave's reviewers inherit

- **Verify every shipped representation/engine fix FIRED before crediting or re-diagnosing it** — grep
  the corpus for the marker string. The re-equip "(ALREADY attached)" marker is shipping the
  equipment-host fix (ledger #1); CONFIRM it now appears on re-equip decisions before trusting it. Same
  for `[attached to: host]` board lines. This is the wave's #1 method lesson.
- **The counter-tell remains RETIRED; the stack surfacing is VALIDATED** (2/27 → 13/14). Teach reading
  ON THE STACK. "Died with counters in hand" stays a MANA-DISCIPLINE per-deck item.
- **Factor out the fetch/single-option re-offer loop before reading timeouts / decision counts** — it
  inflates both, worst against lifegain/control decks; it is the top inference-budget drain and the
  wave-7 harness priority alongside -j2.
- **The core `hand -> stack` anchor is on the removal SHORTLIST** (0 no-ops all 7 seats; 353f1a150). If
  wave-7 is a second clean no-op corpus, retire it via a deliberate A/B (measure repick-loop / Cast-
  nothing rate). Do not retire mid-wave.
- **Convergence watches carried into wave-7:** (a) reflexive-high-life-blocking — routed to
  representation; if the surfaced life-vs-lethal marker ships, verify it fires; (b) a FREE reaction still
  costs a CARD (deck135, 1 seat); (c) burn-prior on scarce X-finisher (deck140, 1 seat); (d) mulligan
  guide-precedence over the land-count default (deck131, 1 seat); (e) attack-seam survival strengthening
  (deck110, 1 seat); (f) HALLUCINATED-BOARD phantom lethal (deck135, 1 seat) — routed to
  model-experiments. Promote any of (b)-(f) only on a second seat.
- **New harness-defect-sweep class: VALIDATION-DROP** (chosen recorded, heuristic played, stderr-only) —
  check `game-*.stderr` for `fails validation; deferring to heuristic` whenever a chosen targeted spell's
  board has no legal target; do not score it as what happened.
