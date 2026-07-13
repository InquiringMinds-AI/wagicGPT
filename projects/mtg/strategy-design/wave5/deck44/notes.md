# Notes for wagicGPT development - deck44 wave-5

Run: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260713-145947/`. deck44 = deck0 in all 6 games.
File->game map (the filename number is NOT the results.tsv start_epoch - it is offset by 1-5, which
made mapping fragile; verify via opponent signature cards, as done here):
- `1783972790...` = vs135 WIN (snow/simic)      | `1783972792...` = vs133 LOSS (mono-black)
- `1783975169...` = vs110 LOSS (affinity)        | `1783978528...` = vs140 LOSS (WU control)
- `1783980437...` = vs109 LOSS (mono-red aggro)  | `1783981248...` = vs131 WIN (izzet spells)

## Engine / representation defects (with repro)

1. **Stack not surfaced in CURRENT SITUATION (#25 representation half - PERSISTS, prime suspect for
   deck44).** The pending spell exists only in the game-log tail (`Opponent's X: hand -> stack` with
   no `stack ->` after). The `--- CURRENT SITUATION ---` block has no stack line; the counter option
   renders bare (`Cast Counterspell {u}{u}`). The pilot fabricates a false resolution over the tail
   and passes. Deck44 fired counters in **2 of 27** offered windows this corpus.
   Fix: add an `On the stack (you may respond): ...` line + label the counter option with its target.
   Repro: `e1248 s3` (Young Pyromancer pending, reply claims "moved to the battlefield"); `e5169 s30`
   (Cranial Plating pending, reply "no spells on the stack"); `e8528 s26`/`s49` (Wrath of God pending,
   reply plans to counter then picks Cast nothing).

2. **Counterspell reaction TIMING (#25 timing half) appears IMPROVED.** All 27 counter windows this
   corpus arrived while the spell was genuinely still on the stack (tell audit 27/27 live-pending),
   unlike wave-4's vs110 post-resolution window. If corroborated across decks, the timing half of #25
   can be closed and #25 narrowed to representation.

3. **Sleep-Cursed Faerie {1}{U} untap re-offered in EVERY priority window, including Blockers and
   Combat-damage phases** where it is meaningless. Repro: `e2790` offered the untap 29 times in one
   game (`s52` Blockers, `s53` Combat damage). Pure attention tax - the pilot correctly passed all 29.
   Ask: offer the untap only in windows where it can matter (the controller's own main phases).

4. **Empty `[counters:]` bracket renders on Sleep-Cursed Faerie once stun counters reach 0** (carried
   from wave-4 F4 nit). Cosmetic; wastes tokens; looks like a glitch. Confirm and suppress.

## Harness asks

- **results.tsv / filename mapping.** The jsonl filename integer is not the `start_epoch` in
  results.tsv (offset 1-5) and neither encodes the matchup. A matchup token in the filename
  (`...-deck44-vs131-...`) or a `winner`+`seat` column keyed to the filename would remove the
  signature-card guesswork this review needed.
- **PLAN/CHOICE MISMATCH as a model-experiment residual.** `e8528 s26/s49`: the pilot's PLAN names
  the correct counter, the option is listed, head-int == choice, yet it picks "Cast nothing." This is
  intent collapse under a passive default, not a parser desync (parser was 0-desync all 6 games). Best
  addressed by option-labeling (defect #1 fix); track as a litmus case for the reply layer.

## Deck-construction flags (for the user / decklist owner)

- **6 reactive cards clog a beatdown deck (persists from wave-4 F2).** 2 Counterspell + 2 Arcane
  Denial + 2 Go for the Throat in a "kill by turn 10" flyer deck. Given the counter seam fires ~7%,
  the four counters are near-dead weight and produce the trap hands in the color-screwed losses
  (`e2792` vs133: kept 3 Islands + Miscreant + 3 counters, drew more Islands, never found black).
  Both WINS came from racing with bodies and firing zero useful counters. Consider trimming counters
  toward more cheap flyers.
- **Manabase skew.** 16 Islands / 10 Swamps against a black-heavy spell suite (Obyra {u}{b},
  Bladecrafter {2}{b}, Go for the Throat {1}{b}) plus blue-only counters. vs133 and vs109 both stalled
  on single-color draws. Rebalancing the mana (or leaning the spells more mono-blue) would raise the
  floor more than any guide line.
- **Surveilling Sprite (3 copies) was absent from the wave-4 guide** - now added to strategy.txt (a
  flyer that draws on death, chumps aggro, drains 1 with Obyra). Confirm it is intended in the list.
