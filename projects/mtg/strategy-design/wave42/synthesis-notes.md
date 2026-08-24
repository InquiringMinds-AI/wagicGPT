# Wave-42 synthesis (2026-08-24)

## Cross-seat synthesis
- The seven engine lanes VALIDATED at corpus scale (engine seat): D1 15.2%->0.44%, all
  lexicon classes zeroed, narration sourceless 0/611, attack-tag coverage 148/148,
  token collapse 0/4144 misses, delta boundaries 1820/1820, handles live. The lane-E
  loud-fail guard FIRED LIVE (stale_livelock, degenerate two-identical-candidate reveal
  ask) and converted a would-be hang into a legal resolution. One falsification:
  Brutal Cathar + Tovolar's Huntmaster still offer hand-flips (borderline.txt residual
  F3) -> wave-43 HIGH. Fallbacks 0.18%, best yet.
- MENACE found broken from BOTH sides independently (two seats): blocker-side silent
  discard (MTGRules.cpp:2747 strips <2-blocker assignments AFTER declaration; three
  surfaces lie in sequence; ~24-life swing in one game) + attack-side tags computing
  1-on-1 outcomes for menace attackers. The wave-43 headliner.
- GUIDE THEME OF THE WAVE: imperatives outvote their own stop-clauses. Three seats found
  the same shape: deck123's take-it imperative (stated 3x) vs its once-stated stop
  (72% obedience, "free body" line quoted in overrides); deck130's "send EVERY except X"
  splitting 9/9 when X is the only creature; deck130's "CYCLE IT FREELY" vs the upkeep
  pass rule. Skill amendment: a stop-clause must be restated wherever its imperative is,
  and absolutes ("freely", "every", "always") must carry their own exception inline.
- Wave-40's E7 (deck125), the campaign's most-celebrated edit, was INDICTED this wave:
  its unconditional largest-X and >=6-spend triggers caused two losses now that games
  run long enough for punisher engines (X=7 into Liliana's Caress). Lesson: unconditional
  numeric triggers age badly as the meta deepens — condition on the opponent's board.
- Seat 123 caught a wave-41 EDIT MISFIRE honestly: EDIT 7 ("fire the chain at the FIRST
  window") directed a 12-token own-main build into a t16 Starstorm sweep. Replaced by
  fire-on-their-turn (EDIT C).
- deck130's guide first corpus: 5 held / 5 falsified (3 narrowly) / 1 null; both wins =
  Rorix beatdown; the pilot EMPIRICALLY FOUND the Starstorm release condition the
  author predicted (all 4 rule-breaking cycles at 0 opponent creatures) — codified.
- P6 deck123 construction prediction RETIRED (it described the deck as-built). The
  supported construction measurement (assembly latency separates games; Vision Skeins +
  Greaves = dead slots) ROUTED TO OWNER — seats do not change decklists.

## Deployments this wave
25 guide edits across 7 guides (wave42/edit-texts.md, deploy lane): 162 D2-D5;
126 D6-D8; 152 R1(deletion)+R3a; 146 R2+R3b; 125 W1-W5; 123 A-H; 130 1-5.

## Rotation adjudication (recorded in wave39/rotation-selection.md)
NO EXIT again. Nothing is settled: every guide ships fresh edits; the menace engine fix
and the owner narration fixes need their first corpus; deck125's E7 rework and deck123's
obedience restructure are unvalidated. Wave-43 pool unchanged: 146,152,125,126,162,123,130.

## Owner items
- Lategame specimen: the deck162 X=7-into-Caress decision (seat 125's pick) — cleanest
  composition-level decision-value failure. Staged for delivery.
- ROUTED for Magic judgment: deck123 construction measurement (4x Vision Skeins + 3x
  Lightning Greaves doing no work; assembly latency the separator); deck130 vs
  creatureless-lifegain matchup ceiling (11 face reach).
- Owner narration-fix predictions (rules text, damage single-line, bare names) carry to
  the WAVE-43 corpus (this corpus predates the fixes — provenance-gated).
