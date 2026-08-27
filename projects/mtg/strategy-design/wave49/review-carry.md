# Wave-49 review carry-list (assembled 2026-08-27 at step-1 merge)

Corpus: sixth fair-hand corpus. Binary = master 7e94544f1 (owner Vita fixes 19e2b04f7 +
6819183f7; lanes R fa1fddf02, U 2d6b16d8f, T a2fd9684a, S 63e456fdf merged in that order).
PARSETEST 1783/0 (1655 + 19 + 43 + 21 + 45). Suite 1139/0 + 37/0 expected (1137 + lane T's
2 autotap pins). Guides = wave-48 reviewer revisions (87103fd01) + boundary pass (a4d27c299).
Same pool/recipe (146,152,125,126,162,123,130, -r 1, -j 21, -T 0).

## Discipline (owner doctrine 2026-08-26)
Reviewers own guides (mulligan sections are odds-based, no floors/forced keeps — skill #119);
synthesis owns skill + general; the core loop (engine correctness, LLM UX, bugs) is the purpose.
Adjudicate render predictions by the emitter's actual string. Baselines: fallbacks 0.10-0.31%;
21/21 natural (wave 48: 21/21 in ~1h, max 133 records/seat, max 9 priority windows/turn).

## Set B — engine changes since the wave-48 corpus (each with its lane's prediction)
- Vita/human-seat oracle fixes (19e2b04f7, 6819183f7): not LLM-visible; no corpus prediction.
- R fa1fddf02 (D1/D13/D9): B-row `may block A1, A3-A257 (all: ...)` ranged + factored;
  same-name A/B rows grouped across the `#1/#10` sort; vigilance attackers counted once.
  Predictions: no blockers prompt > 15,000 chars, no prompt > 30,000; no B-row repeats a
  parenthetical > 2x; 0 same-name rows split by another name; header attacking + "more able"
  <= creature count.
- U 2d6b16d8f (D5/D6/D7/D12): converter PAIR loop clause + direction sentence + A-row loop tag;
  `[DRAW PRICE:]` on draw:X / Clue / opposing cast-trigger rows + `DRAW FORECAST` line under
  DRAW PUNISHERS; carried plan expires after 5 identical echoes or when it names no action;
  `// <Name> (text omitted)` back-face marker. Predictions: every Bond+Blood board prompt says
  "Both halves of a life LOOP", 0 non-`none` ATTACK lines into it, 0 "net 0" replies; every
  Sphinx's Revelation / Clue row under a punisher priced; DRAW FORECAST present wherever DRAW
  PUNISHERS is; no PLAN echoed verbatim > 5 windows, 0 "the game is lost" carries; bare
  back-face endings 0/N (was 118).
- T a2fd9684a (D4/D10/D11): generic pips paid from least-flexible sources (scarcity DESC,
  colour-count ASC, layer order) — human seat too; `{paying this taps: <creatures> - they
  cannot attack this turn}` on cast/ability rows, `[this land is TAPPED: ...]` on becomes
  rows, `{tapping <name> now: ...}` on Tap-cost activations; edict `{right now: they control
  N creatures ...}`, sweeper `destroys N of theirs (K able to attack), M of yours`, attack
  punisher counts. Predictions: 0 casts tapping a colour-C source while a non-C source could
  pay the generic; creature-tapping casts in a lethal-on-board pre-combat window 0; edict casts
  at N=0 0/N; sweeper casts on `destroys 0` rows 0/N.
- S 63e456fdf (D2/D3/D8): E1 replacement validated name-vs-index (`named_row_not_offered`);
  last CLEAN `ATTACK:`/`BLOCKS:` line taken (`attack_last_line_taken`/`blocks_last_line_taken`);
  `answer_replaced` never true when the first coded line executed; repeat count grammar
  `x17|17 times|N=17|N = 17|N:17|, 17)` + one `[RE-ASK]` when countless then `ran 1 time (you
  named no count)`; index-past-menu with unmatched name → one `[RE-ASK]` (`named_row_reask`,
  then `_recovered`/`_exhausted`); exit row `Cast nothing right now (combat comes next this
  turn)` on own first main; `kCastAnsweredFact` on the priority ask after a cast ask.
  Predictions: off-menu-name replacements executed 0; false `answer_replaced` 0;
  `repeat_count_missing` without a preceding `repeat_count_reask` 0; index-past-menu Baka
  fallbacks without a `named_row_reask` first 0 (was 4/5), >= 50% recovered.

## Set A — wave-48 guide edits: per wave48/deck*/edit-texts.md + wave48/reviewer-boundary-pass.md
(pool-wide mulligan re-shape to the 146-F odds form; converter pair line in 146/162; deck123
repeat-row spelling positive only).

## Carried (wave48/engine-ledger.md D14-D19 + D20 frontend, not taken this wave)
X free-kill marker row position (D14); menace single-block re-ask (D15); overrun measurement
(D16); JSONL `turn` off by one (D17); parity residuals (D18); keyword canon (D19); D20 frontend
grouping + O(n^2) RenderSpell (human seat). Lane R's note: `parseBlockAssignments` re-scans a
B-range's upper label and counts one `dropped` per range reply (cosmetic count).
