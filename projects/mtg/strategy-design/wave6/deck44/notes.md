# Notes for wagicGPT development — deck44 wave 6

Run: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260713-190011/`. deck44 = deck0 in all 6 games.
FIRST corpus on the stack-surfaced engine (ON THE STACK section + "can target on the stack" labels).
File/game map (jsonl epoch = results.tsv start_epoch + ~2-3; stderr filenames are now clean
`game-44vNNN-<start_epoch>.stderr`):
- `1783987213` = vs140 WIN  | `1783988143` = vs110 WIN | `1783989289` = vs135 TIMEOUT(loss)
- `1783991689` = vs133 LOSS | `1783994322` = vs131 WIN  | `1783994416` = vs109 LOSS

## Engine / representation defects (with repro)

1. **Go for the Throat offered with NO legal target -> validation-fail, silent heuristic deferral
   (NEW, prime engine defect this wave).** The harness offers `Cast Go for the Throat` whenever GFTT is
   castable (mana payable) WITHOUT checking a legal target exists, violating the prompt's own contract
   ("the game has already checked your mana, colors, and TARGETS"). When selected, targeting fails and
   the engine logs `model chose Go for the Throat but it fails validation; deferring to heuristic`, then
   plays a heuristic pass (`NextGamePhase`). **9 occurrences across 2 games.** Two board shapes:
   - vs140 (`game-44v140-1783987211.stderr` lines ~810, ~922; jsonl `e7213 s27, s42`): opponent board is
     all lands + Lightmine Field (an ENCHANTMENT), zero creatures. Model plans "destroy Lightmine Field"
     — GFTT is `Destroy target nonartifact CREATURE`, illegal type.
   - vs135 (`game-44v135-1783989286.stderr` lines 1080, 1094, 1319, 1378, 1526; jsonl `e9289 s17, s18,
     s25, s26`): opponent's only creature is Icehide Golem, a **Snow Artifact Creature**
     (`type=Snow Artifact Creature` in borderline.txt) — GFTT can't target artifact creatures. With only
     GFTT left in hand, the model re-picks it every window it re-appears (engine-defect waste loop).
   **NOT async target-drift** (the wave-6 hypothesis): the board had no legal target at both offer and
   consumption time; nothing left the battlefield in between. **Data-quality impact:** the JSONL records
   `chosen_text = "Cast Go for the Throat"` but that is NOT what happened (a chosen!=happened artifact).
   A reviewer working from JSONL alone mis-scores these as real casts.
   **Fix (engine):** suppress or mark `[no legal target]` any targeted-spell option whose legal target
   set is empty at option-build time.
   **Guide stopgap this wave (removal condition: engine target-check ships):** strategy.txt GFTT block
   now names enchantments (Lightmine Field) + Icehide Golem and adds the no-target behavioral fallback
   ("no nonartifact creature on their board -> GFTT has no target; do NOT pick it").

2. **JSONL should log the RESOLVED action, not the dropped model pick (harness ask, ties to #1).** When
   the engine defers to the heuristic, the JSONL still records the model's pick as `chosen_text`. Either
   log the action that actually resolved, or add a boolean flag (`deferred_to_heuristic: true`) so
   reviewers do not have to cross-reference stderr to detect validation-drops.

3. **Empty `[counters:]` bracket still renders** on Sleep-Cursed Faerie / other permanents once counters
   reach 0 (carried from wave-5 notes #4). Present 37x in the vs110 jsonl (`e8143`). Cosmetic, wastes
   tokens, looks like a glitch. Suppress the bracket when the counter list is empty.

4. **Sleep-Cursed untap re-offer-every-window (wave-5 notes #3) appears IMPROVED / not reproduced.** The
   `{1}{U}: Untap Sleep-Cursed` option was offered 0 times in vs140 and 1 in vs110 despite copies on the
   battlefield (wave 5 reported 29 offers in one game). If an engine change gated the untap offer to the
   controller's main phases, confirm and close the wave-5 ask; otherwise it is corpus variance (fewer
   Sleep-Cursed on board this run). Low priority — the wave-5 compressed guide line handles it either way.

## Harness asks

- **JSONL filename still lacks the opponent token.** The stderr filenames are now clean
  (`game-44v140-<epoch>.stderr`) — a real improvement over wave 5 — but the per-seat jsonl is still
  `<epoch>-ai_baka_deck44-<hex>.jsonl` with no opponent and an epoch offset from results.tsv. Add the
  opponent token (`...-deck44-vs140-...`) to the jsonl name to remove the signature-card guesswork.
- **Reviewers now MUST read `game-*.stderr`** to catch validation-drops (defect #1). Recommend the
  digest tooling grep stderr for `fails validation` per game as a standard sweep step (proposed as a
  skill method addition in skill.md P1).

## Deck-construction flags (for the user / decklist owner)

- **6 reactive cards + manabase skew (persists from wave-5 F2).** 2 Counterspell + 2 Arcane Denial + 2
  Go for the Throat on a 16 Island / 10 Swamp base against black-costed spells (Obyra {u}{b},
  Bladecrafter {2}{b}, GFTT {1}{b}). vs133 flooded on Islands and never cast a black spell; vs135 flooded
  on lands + counters with ~zero creatures until turn 16 (the timeout). **Reweighting vs wave 5:** the
  counters now EARN their slot — they won vs140 (Pyroclasm/Wrath/Rakdos's Return) and vs110 (Etched
  Champion). So "trim the counters" is weaker advice now; the sharper lever is the **manabase** (raise
  the black-source count against the black spell load) and/or 1-2 more cheap flyers over the 3rd/4th
  reactive card, to lower the all-reactive-no-clock trap-hand rate.
- Surveilling Sprite (3 copies) and the counter suite are intended per the wave-5 decklist read — no new
  list surprises this wave.
