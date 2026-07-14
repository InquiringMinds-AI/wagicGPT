# Notes for wagicGPT development — deck44 wave 7

Run: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260713-222831/`. deck44 = deck0 in all 6 games.
**Record 5W-1L (top deck of the pool).** Corpus health: 2/21 timeouts total (135v140, 140v131 —
neither is a deck44 game), median latency ~7.5s (-j2 fixed the wave-6 spikes), 0 desyncs, 1 fallback
(non-decisive babble). All 6 deck44 games resolved in 12-18 turns.

Epoch->game map (map by gameend life/turn vs results.tsv — jsonl start-epoch order != game order):
- `1783999713` = vs131 WIN (15/-7 T14)   | `1783999716` = vs109 WIN (10/-9 T12)
- `1784000884` = vs140 WIN (19/-9 T12)   | `1784002523` = vs133 LOSS (0/13 T13)
- `1784005194` = vs135 WIN (18/-10 T18)  | `1784005681` = vs110 WIN (20/-3 T14)
stderr filenames are clean `game-44vNNN-<start_epoch>.stderr`.

## Engine / representation defects (with repro)

1. **Targeted spell offered with NO legal target -> validation-fail, silent heuristic deferral
   (wave-6 defect #1, now CONFIRMED MIGRATING across cards/seats).** deck44's GFTT instance is GONE
   (0 `fails validation` in all six deck44 stderr — the wave-6 guide stopgap + no-target fallback
   worked, down from 9). The deck-agnostic engine defect surfaced instead on the OPPONENTS' targeted
   spells this corpus:
   - **Liliana of the Veil: 4x** `model chose Liliana of the Veil but it fails validation; deferring
     to heuristic`
   - **Fatal Push: 2x** `model chose Fatal Push but it fails validation; deferring to heuristic`
   Grep: `/usr/bin/grep -h "fails validation" game-*.stderr | sed 's/.*model chose //' | sort | uniq -c`.
   These are the mono-black opponents' decisions (not deck44), so they do not appear in any deck44
   jsonl — a reviewer working from deck44 JSONL alone would never see them. **Root cause (engine,
   unchanged):** the harness offers `Cast <targeted spell>` whenever mana is payable WITHOUT checking a
   legal target exists, violating the prompt's own contract ("...has already checked your mana, colors,
   and TARGETS"). The pick fails targeting -> engine plays a heuristic move (`NextGamePhase` /
   heuristic action) -> JSONL records the dropped pick as `chosen_text` (a chosen!=happened artifact).
   **Fix (engine):** suppress or mark `[no legal target]` any targeted-spell option whose legal target
   set is empty at option-build time. **The per-deck guide stopgap can NEVER be the general fix** — it
   only silences the reviewer's own deck; the class walks to the next targeted spell (as this wave
   proves). Removal condition for all guide stopgaps of this class: engine target-check ships.

2. **JSONL should log the RESOLVED action, or flag `deferred_to_heuristic: true` (wave-6 ask, still
   open, ties to #1).** Without it, validation-drops are invisible in JSONL and reviewers must grep
   stderr per game. Recommend the digest tooling add a standard sweep: grep each `game-*.stderr` for
   `fails validation` and list card + count.

3. **Empty `[counters:]` bracket still renders** on Sleep-Cursed Faerie / permanents once counters
   reach 0 (carried from wave-5/6 notes). Still present this corpus. Cosmetic, wastes tokens, looks
   like a glitch. Suppress the bracket when the counter list is empty.

4. **Phyrexian Obliterator block feeds the sac punisher — a life-vs-punisher REPRESENTATION gap.**
   `vs133 s18`: Obyra (3/3) blocks Phyrexian Obliterator at L11 -> its `@damaged(this) ... sacrifice
   that many permanents` (mtg.txt) forces 3 sacrifices (Scion + 2 Islands, `s19-s21`). The block
   decision surfaces no marker that the attacker punishes its blocker. Same shape as the wave-6
   reflexive-block convergence (surfacing life-vs-lethal at the block seam): the durable fix is to
   annotate a blocker option with the attacker's punisher rider. Filed to the ENGINE/REPRESENTATION
   watch list (single seat this wave; per-deck guide fix shipped in strategy.txt). Repro: jsonl
   `1784002523-...-deck44-*.jsonl` s18-s21; opponent card `auto=` in `primitives/mtg.txt`
   (`name=Phyrexian Obliterator`).

## Harness asks
- **JSONL filename still lacks the opponent token.** stderr is clean (`game-44v133-<epoch>.stderr`) but
  the per-seat jsonl is `<epoch>-ai_baka_deck44-<hex>.jsonl` with no opponent AND the start-epoch order
  does not track game order — I had to map games by gameend life/turn against results.tsv, not by
  filename. Add the opponent token (`...-deck44-vs133-...`) to the jsonl name.
- **Reviewers MUST grep ALL `game-*.stderr` (every seat), not just their deck,** to catch the migrated
  validation-drop class (#1). Recommend as a standard digest step (proposed in skill.md P2).

## Deck-construction flags (for the user / decklist owner)
- **6 reactive cards + manabase skew (persists from wave-5/6 F2), but re-weighted UP.** 2 Counterspell
  + 2 Arcane Denial + 2 Go for the Throat on a 16 Island / 10 Swamp base against black-costed spells
  (Obyra {u}{b}, Bladecrafter {2}{b}, GFTT {1}{b}). The counters now clearly EARN their slots — they
  protected 4 of the 5 wins (Cyclonic Rift, Black Sun's Zenith, Master of Etherium, Etched Champion,
  Glacial Revelation). So "trim the counters" is the WRONG lever now. The sharper one is the
  **manabase**: the only loss (vs133) was threat-starved (2 creature-casts all game) partly because it
  could not reliably assemble black. Consider raising black sources against the black spell load,
  and/or 1-2 more cheap flyers over the 3rd/4th reactive card, to lower the all-reactive-no-clock
  trap-hand rate. Not guide-fixable.
- **vs133 (mono-black Phyrexian Obliterator devotion) is deck44's hardest matchup.** Even with perfect
  piloting the deck's only clean answer to Obliterator is 1-of-2 Go for the Throat; if GFTT is not
  drawn or is spent, the 5/5 trampler grinds the flyer clock out. The guide now teaches holding GFTT
  for it, but this remains a construction-level bad matchup, not a piloting bug.
