# Deck-140 wave-6 notes — everything that doesn't live in the other four files

Companion to findings.md / strategy.txt / skill.md / general-suggestions.md.
Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260713-190011/` (6 deck140 games).
Record ~ 1 raw W, 2 L, 1 adj-W, 1 adj-L, 1 draw. Digest scripts in the scratchpad
(`digest.py`, `audit.py`, `rakdos.py`, `verify.py`, `opts.py`, `end.py`).

Game → file map (epoch prefix):
- 1783987213 = vs44   1783989086 = vs131   1783989616 = vs133
- 1783992016 = vs110  1783993523 = vs109   1783995180 = vs135

---

## STOPGAP / DEFUSAL LEDGER (each = line, defect+citation, removal condition)
- **X-menu value-anchored pick** (strategy.txt "THE X MENU"). Defect: the ANNOUNCE_X menu is index-
  space; a positional pick inverts on reorder. Status: engine shipped largest-first + "option 1 is the
  largest X" ask text; the guide line is now a value-anchored CONFIRM, not a stopgap driving the pick.
  Removal condition: none needed — it degrades to a harmless confirm. Keep compressed. (7/7 max-X this
  wave.)
- **Sweeper creatures-only corrective FACT** (strategy.txt "YOUR SWEEPERS HIT CREATURES ONLY"). Defect:
  model false-belief that board wipes destroy artifacts, surfaced when the combo-hold hint-veto was
  removed (findings Finding 2; PLANs cite it explicitly in vs110). Removal condition: if the board
  representation ever tags "0 creatures on opponent battlefield" as a first-class line the model
  reliably reads, OR the false-belief stops appearing in PLANs for 2 waves. Until then KEEP.
- **Rakdos hold vs cast-nothing** (strategy.txt #1 RULE + CAST NOTHING/HOLD). Not a defect stopgap — a
  guide-internal contradiction FIX (skill.md Proposal 1). Permanent guide content, not removable.
- **Anti-first-option hold line** ("even at option 1, pick Cast nothing"). Defect: intent-collapse /
  first-option bias lands on Rakdos's Return (~11 windows at option 1; general-suggestions ENGINE).
  Removal condition: when the reply-protocol/representation fix for intent-collapse ships. Interim.

## FREEZE-CHECK (turnaround-ish: guide lines the logs showed OBEYED, and where they went)
- **X-menu pick option 1 / biggest X** — OBEYED 7/7 → kept, compressed to one confirm line. FROZEN.
- **Sweeper selection table** (Edict/Pyroclasm/Wrath/BSZ by board) — OBEYED on every real creature
  board → kept verbatim in shape. FROZEN.
- **Staff pings to face** — OBEYED ~all → compressed to one line, load-bearing tokens kept ("face by
  default; a creature only when toughness shown as 1"). FROZEN-compressed.
- **Lands every turn / mulligan 2-4 lands / no-instants override** — OBEYED → kept verbatim. FROZEN.
- **Rakdos lethal check (X = untapped lands − 2; if ≥ opp life, cast now)** — OBEYED where it fired
  correctly (vs44 wave-5, vs131 X=11 this wave) → kept verbatim inside the reframed #1 rule. FROZEN.
- NEW text aimed only at the two new leaks (hold-permission + burn-override + creatures-only fact +
  Blightsteel-vs-lifegain). Nothing obeyed was rewritten away.

## WATCH NEXT (wave 7) — did the fixes land?
1. **Rakdos's Return timing table** (rakdos.py): did the widened cast-nothing exception + by-name
   burn-override stop the early X=1..4 chips? Target: early fires drop from 6/7 toward 0, WITHOUT
   tipping into never-firing-the-kill (vs131-style held X=11 must still happen). Watch for the opposite
   failure — holding Rakdos past a lethal window.
2. **Sweeper-into-empty-board count** (audit.py, creature count at each sweep): did the creatures-only
   fact stop the artifact-board wastes? Target: ~0 sweeps into 0-creature boards. vs110 (affinity) is
   the key regression check.
3. **Intent-collapse recurrence**: did Rakdos-at-option-1 still get picked against a plan that says
   hold? (representation, but check the PLAN-vs-choice on Rakdos windows.)
4. **vs110 specifically**: the two fixes both target it. If it converts to a win/ahead, they landed.

## DO NOT RE-LITIGATE (settled, with evidence)
- X value/index trap — engine-fixed, 7/7 max-X two waves running.
- Staff targeting — face by default, obeyed.
- Pool-panic / false "can't pay" — every cast-nothing with a mana mention was a correct decline
  (vs131 19/19 held only sweepers-vs-empty; vs110 holds were correct).
- Sweeper SELECTION (which sweeper for which board) — followed correctly four waves running; the wave-6
  leak was TIMING (empty-board waste), not selection.
- Un-veto — working; do not re-add the combo-hold restriction.

## DAY-ONE SCRIPTS FOR NEXT REVIEWER (~20-30 lines each; `options_text` is now a FIELD)
1. per-game digest: kinds/desync/latency/maxturn per file (digest.py).
2. sweeper-timing table: every sweeper/edict cast with the opponent creature-count at cast (audit.py's
   `count_opp_creatures`) — 0 = waste.
3. Rakdos timing table: turn, X, opp life, my life, opp hand at each Rakdos cast (rakdos.py).
4. X-regression: every `chosen_text` starting "X =" vs option-1 value (confirm max-X persists).
5. cast-nothing correctness: for each "Cast nothing", was every non-nothing option a sweeper-vs-empty
   or a below-lethal Rakdos? (opts.py pattern).

## KEY NUMBERS THIS CORPUS
- Rakdos's Return casts: 7 total; 1 held-for-kill (vs131 X=11), 6 early chips (X 1,3,3,3,3,4).
- Sweeper/edict casts: ~22; ~7 wasted into 0-creature boards (5 vs110 artifact, 2 vs109).
- X picks: 7/7 option-1 (max) by intent; 1 deliberate X=0 cycle into empty board.
- Desyncs 0/295; fallbacks 0; latency spikes to 61.7s under `-j`.
- vs44 dealt 0 damage across 20 turns (opp at 20 throughout); 2 Counterspells fired.
