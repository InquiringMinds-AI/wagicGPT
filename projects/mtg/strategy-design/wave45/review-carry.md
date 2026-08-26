# Wave-45 review carry-list (assembled 2026-08-26 at step-1 merge)

Corpus: SECOND fair-hand corpus (baseline = wave 44, matchups-20260826-111937). Binary = master
fd69fadc2 (four step-1 lanes merged; PARSETEST 1241/0). Serve: FP8-35B qwen36-35b-a3b @ spark:8084.
Same pool 146,152,125,126,162,123,130; same recipe (-j 21, no cap, timeout 900, maxtokens 6000).

## Discipline
- Wave 44 is the fair-hand baseline: win-rate deltas vs wave 44 are now hand-comparable but
  still n=6/seat — report rates with counts, never as results (feedback: rare-event power).
- Mulligan floors were OWNER-ROUTED in wave 44 (engine-ledger.md OWNER-CALL 1-3) — no floor was
  moved; record mulligan windows again, do NOT patch floors (skill amendment 38).
- Fallback baseline 0.28% (w44) / 0.17% (w43). The E-3 modal-menu source fix (dd8bdb503) predicts
  stale_echo on subject-less mode menus -> 0; the Teferi 'choose a land' sub-asks are prompt-side
  (G-6/G-7 in seat-146-152-162.md) and may persist — classify, don't conflate.

## Prediction set A — wave-44 GUIDE EDITS (7 guides, deployed 5cd6a860b)
Per wave44/deck*/edit-texts.md and the seat files' 'predictions for wave 45' lines. PASS / FAIL /
UNTESTED with counts. Headliners: deck123 four-CHECK attack ladder (wall-board STOP) and
lethal-biggest-first blocking; deck130 Starstorm DO-NOT-TAKE-IT half + lethal chump rule; deck125
sweeper-at-0-creatures gate hoisted, X<=N-2-Staffs, Elixir library<=16; deck126 Bond-alone clock +
survival floor 7b + Savannah/Battlement facts; deck146/152 lethal-survival gate ABOVE the block
ladder; deck162 S6 restored as affirmative rung; deck152 counters-into-Fog-Bank.

## Prediction set B — wave-45 ENGINE LANES (each falsifiable, from the lane reports)
- #W45-1 (lane A, e4589e7d1): bare 1-on-1 verdicts on menace-attacker B-rows 0/N; solo menace
  blocks 0; menace-caused dropped_assignments 0. Render: `menaceBlockPairingTag` text on B-rows;
  zeroPowerBlockerTag's 'stops NOTHING there' branch when any legal attacker has menace.
- #W45-2 (lane A): attacks taken on a COLLAPSED tag with N>=2 that end with the attacker dying: 0.
  Render: `GANG BLOCK: any N of them together deal D, enough to kill this attacker; each result
  above is a LONE blocker only`; threshold constant kPotentialBlockersEnumerateMax. Also count how
  often the gang price was priceable vs silently skipped (lane A did not measure).
- #W45-3 (lane B, 56e852a78): block-trigger life gained in any single deck146 game < 6;
  '(neither dies)' bodies sent into a 2+-defender board 0/N. Render: '(neither dies (blocking
  trigger: they gain 3))' family + one attackers-prompt converter line (Sanguine Bond class).
  First live observation of the TargetChooser leg — verify the tag appears on Perimeter Captain /
  Pride Guardian boards at all (lane B never saw it render).
- #W45-4 (lane D, 6b5c7d93c): plan-note land-drop fires 0/~400; strict false fires 0; overall fire
  rate < 25% (lane D's replay estimate ~12.5%); precision >= 95%. Measure with the ENGINE's own
  fires (note present in prompt), not a reconstruction.
- #W45-5 (lane C, 4a4df6ee4): plan-vs-chosen X divergence 0/N; casts at an X whose 'YOURS:' names
  the seat's clock 0 (was 2/2). Render: every 'X = N' row carries '{X pricing: kills THEIRS: ...;
  YOURS: ...}' or '(same as X=N+1)' (collapse points at the row already read — largest-first).
  Verify on a TARGETED X spell too (lane C only saw the sweep class).
- E-3 (dd8bdb503): subject-less 'A choice is required' headers on modal-spell menus 0/N; those
  menus render 'Choose an option for <spell>:'.

## Carried MED/LOW docket (wave44/engine-ledger.md) — not shipped this step
#W45-6 opponent non-creature permanents carry no text on the board line (Sanguine Bond, Staff of
Nin, Howling Mine, Lightmine Field); teferieffect label + `Transform:backside` lexicon leak (same
locus); singular loyalty '(now N)'; bare [defender] gloss; dropped_assignments conflates Bn:none;
no lethal headline on the attack side; retracted_choice / no-decode-stop (679s); #W44-9 mana-only
window auto-pass; Baka menace gang-block behaviour (red pin, probe deck owed).

## Owner prompt review (invariant 0)
Pull ONE verbatim lategame prompt into wave45/lategame-specimen.txt (wave-44's is still unread).
