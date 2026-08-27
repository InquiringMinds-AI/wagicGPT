# Wave-48 review carry-list (assembled 2026-08-27 at step-1 merge)

Corpus: fifth fair-hand corpus. Binary = master eccf8284c (E1 parser fix 5ce8f8a5a + lanes
N/O/P/Q merged in order P 9b19fbfe6, O 96a07e864, N 7a858904d, Q 7246c4a95). PARSETEST
1655/0 (1524 + 19 + 40 + 29 + 43). Guides = wave-47 reviewer revisions (4b4ade1bc) +
boundary pass (8b25c325a). Same pool/recipe (146,152,125,126,162,123,130, -r 1, -j 21, -T 0).

## Discipline (owner doctrine 2026-08-26)
Reviewers own guides; synthesis owns skill + general strategy; the core loop (engine
correctness, LLM UX, bugs) is the purpose. Deck-content questions are the reviewer's call.
Adjudicate render predictions by the emitter's actual string. Baselines: fallbacks
0.10-0.31%; 21/21 natural x7, wave 47 = 20/21 (deck123 vs deck162 Doomsayer+Alarm dead loop,
1,871 priority windows in one turn, killed at ~9h).

## Set B — engine changes since the wave-47 corpus (each with its lane's prediction)
- E1 5ce8f8a5a: a contradictory mid-line index whose LAST coded `CHOICE:` is a real
  re-answer is now TAKEN (replacement) instead of retracted to fallback. Prediction:
  retraction-to-Baka on replies carrying a final coded CHOICE 0/N.
- N 7a858904d (D1 F1/F2/F3 + D13): `0. Pass priority (take no action this window)` real last
  row on single-option priority windows; `<action>, repeated N times, then stop` row when an
  activated ability with counters>=2 this turn costs only a self-tap (ceiling 200, one
  AIAction per tick, exits on exhaustion / board-unmoved x2 / turn boundary, narration
  receipt with the actual count); `[repeat:]` tag names BOTH scopes (`this turn` counter +
  loop-scoped `taken it N times in a row`). Predictions: no game reaches 500 priority
  windows in one turn; the three deck123 token games close in < 60 priority records each
  (was 1,871 / 1,550 / 223); 21/21 natural; tag maximum reconcilable with the seat's own
  activation count. OPEN: does qwen ever TAKE the repeat row (only a stub did, live)?
- O 96a07e864 (D2/D11/D12): ranged collapse on A-rows of both combat screens
  (`kAttackerRangeNote`) + numbered option lists (`joinNumberedRows`, rows regrouped by
  ascending rank because option order is lexicographic on name); grammar accepts
  `ATTACK: A2-A5`, `ATTACK: all` (not inside a decline), `BLOCKS: B4-B6:A2`;
  `NarrationCycleHolder` collapses log cycles up to period 4 with exact counts;
  battlefield entries group by identical text with stable reorder. Predictions: no prompt
  > 60 KB (was 233,662 / 116,128 / 74,063 chars); blockers/attackers p90 latency <= 60 s.
  Watch: a mixed-body attack may still interleave A-rows (adjacency-only there).
- P 9b19fbfe6 (D3/D4/D7): header `N of them are attacking right now, M more able to
  attack right now` counted from isAttacker() in the same pass; collapsed potential-blockers
  tag adds `most expensive to attack into: <name> (P/T) (...)` when priciest != biggest;
  stack rows and `can target on the stack:` clauses carry the live keyword set. Predictions:
  header number below the `[tapped - attacking]` row count 0/N; replies pricing a collapsed
  swing by the representative alone while a larger price sits on the line 0/N; counters
  spent on a stack object of printed power 0 / `{target text:}` beginning "Defender" 0.
- Q 7246c4a95 (D5/D6/D8/D9): `{card text:}` budgeted per ` -- ` clause and per `//` face,
  no mid-word cuts; `auto=choice` menus name their spell via `DecisionRequest::contextName`
  (the resolving spell is in no zone — validateCardPointer refused it) and `target
  opponent/controller` -> `the opponent`/`you`; `[DRAW PRICE:]` reaches cycling/cast draw
  rows (macros now expanded via AutoLineMacro::Process; `scriptSelfDrawCount`); ANNOUNCE_X
  asks carry no plan block and bracket-mark the cheapest maximal free-kill X. Predictions:
  mid-word cuts 0/N; `A choice is required` 0/N; `target controller` in any label 0;
  draw tag on every controller-draw row under a punisher; stale-plan X restated while a
  larger free-kill row is on screen 0.

## Set A — wave-47 guide edits: per wave47/deck*/edit-texts.md + wave47/reviewer-boundary-pass.md
(B2 Master of the Feast = draw engine; B4 tripwire re-keyed to `of which M are creatures`
ceiling 50; B7 deck146 CHECK 2 any-(P/T); B5 roll-call rejected).

## Carried (wave47/engine-ledger.md remaining + residuals)
D10, D14–D19 not taken this wave (see engine-ledger.md); damage OBJECT bare; Clue token
textless; ability$ pay-or-lose punishers undetected; R7 name-substring skip; Baka menace
gang-block probe (red pin); `mana_only_windows_skipped` mass from opponent-untap loops
(8,610 in one game — gate correct, cost is the window count itself → lane N's target).
