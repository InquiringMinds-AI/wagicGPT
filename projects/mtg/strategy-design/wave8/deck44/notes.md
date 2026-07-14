# Notes for wagicGPT development — deck44 wave 8

Run: PRIMARY control arm `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260714-044131/`. deck44 = deck0 in all
6 games (confirmed via the new `gamestart` record). **Record 5W-1L (top of the pool).** Corpus health for
deck44: 0 desyncs, 0 fallbacks, 0 timeouts, 0 validation-drops; all 6 games resolved in 9-18 turns; median
latency healthy. Win column is context only (doctrine).

game->file map (map by `gamestart` + gameend life/turn vs results.tsv — start-epoch order != game order):
- `1784023977` = vs109 **LOSS** (0/8 T9)   | `1784026019` = vs140 WIN (20/0 T15)
- `1784026789` = vs135 WIN (18/-5 T10)      | `1784027271` = vs131 WIN (20/-1 T14)
- `1784027527` = vs110 WIN (7/-11 T18)      | `1784028941` = vs133 WIN (16/0 T14)

## Engine / representation defects (with repro)

1. **Targeted spell still OFFERED with an empty target set (contract violation persists; validation-DROP
   class otherwise dead).** Corpus-wide `fails validation` = **1** (`land Glimmervoid`, deliberate gate),
   down from wave-7's 15. The pick-time deferral is effectively cured. BUT the OFFER is unchanged: `1. Cast
   Go for the Throat {1}{b}` renders on an empty opponent board with **no `[no legal target]` marker**
   (repro: `1784028941-...-deck44` s5, s7, s10, s12, s13 — PLAN each: "no legal target"). deck44 declined
   every time (0 drops) purely because of the GUIDE fallback. **Fix (engine, unchanged):** at option-build
   time suppress or mark `[no legal target]` any targeted-spell option whose legal target set is empty.
   **The 0-drop rate is NOT evidence the fix shipped — it is the guide stopgap working.** Do not demote the
   per-deck GFTT no-target fallback. Grep: `/usr/bin/grep -h "fails validation" game-*.stderr | sed 's/.*model chose //' | sort | uniq -c`.

2. **Block-seam has no "punisher / deals-0 / not-lethal" surface — REPRESENTATION gap.** `vs110 s19` (L11):
   Archmage of Echoes (4/4 win-con) reflexively blocked Ornithopter (0/2, deals 0). The block option shows
   no marker that the attacker deals 0 / is not lethal. Same family as wave-7's Phyrexian-Obliterator
   punisher-rider ask and the inherited reflexive-block convergence. Durable fix: annotate the blocker
   option with a `[deals 0]` / `[not lethal]` / punisher rider. Repro: `1784027527-...-deck44` s19.
   Single seat this wave; per-deck floor reinforced in strategy.txt (named the win-con card).

3. **Empty `[counters:]` bracket still renders** on Sleep-Cursed Faerie / permanents once counters reach 0
   (carried from waves 5-7). Cosmetic; wastes tokens; looks like a glitch. Suppress when the list is empty.

## Pilot combat-math gap routed to the GUIDE (pre-computable, not an engine bug)
- **Gatewarden's wither is negated by FIRST STRIKE.** `vs109 s6` (L20): Gatewarden (2/1 wither) blocked Ash
  Zealot (`abilities=first strike,haste`, verified mtg.txt) -> first strike killed Gatewarden before the
  -1/-1 counters landed, Gatewarden died for zero, Ash Zealot survived. Better legal block was Stromkirk
  Noble (`auto=@combatdamaged(player)...counter(1/1)`, the growing threat; Gatewarden is a Faerie so
  `cantbeblockedby(human)` did not stop it). The engine MODELS first strike correctly — this is a pilot
  combat-math gap, fixed in-guide (Gatewarden block-selection carve-out). Non-decisive (slow keep vs fast
  aggro), but a real opportunity conversion (Noble grew to 4/4).

## Harness / data-quality asks
- **`gamestart` record is a real improvement** — my_deck/opp_deck let me confirm deck44 = deck0 directly.
  But the per-seat jsonl FILENAME still lacks the opponent token and the start-epoch order still does not
  track game order, so game->file mapping still needs the results.tsv life/turn cross-reference. Add the
  opponent token to the jsonl name to remove the last manual step.
- **Reviewers must grep ALL `game-*.stderr` (every seat), not just their deck,** to see migrated
  validation-drops — still true, though the class is now near-zero.

## Deck-construction flags (for the user / decklist owner) — persists from waves 5-7
- **16 Island / 10 Swamp + 6 reactive cards (2 Counterspell, 2 Arcane Denial, 2 Go for the Throat).** The
  counters clearly EARN their slots (protected 4 wins: Damnation, Cyclonic Rift, Master of Etherium, Etched
  Champion, plus Young Pyromancer / Hydrolash / Geralf's Messenger). "Trim the counters" is the WRONG lever.
  The sharper one is the **manabase** (black-source density vs Obyra {u}{b} / Bladecrafter {2}{b} / GFTT
  {1}{b}) and/or **1-2 more cheap flyers over the 3rd/4th reactive card** — to lower the all-reactive
  slow-keep rate that lost vs109 this wave (and vs133 in wave 7). Not guide-fixable.
- **The loss ROTATES by matchup/draw.** Wave 7 lost vs133 (Obliterator drawn) and beat vs109; this wave
  beat vs133 (Obliterator NOT drawn) and lost vs109 (fast aggro on a Sleep-Cursed-only slow keep). This is
  a variance signature — the deck's reactive half is a slow keep against a faster ground clock, and whether
  the hard matchup's key card (Obliterator) appears swings the game. The guide's Obliterator no-block +
  GFTT-hold were UNTESTED this wave (Obliterator never hit the board) and stay frozen for when it does.
