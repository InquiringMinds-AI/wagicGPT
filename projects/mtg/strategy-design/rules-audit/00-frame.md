# Rules-conformance audit — frame (owner order, 2026-07-24)

Owner's mandate: "compare the engine against current comprehensive rules for its supported game
types... a document flagging all unsupported or incorrectly implemented rules... they are not to
make any changes, only identify unimplemented rules, implemented rules, and incorrectly
implemented rules."

## Ground rules
- **READ-ONLY audit.** No engine, script, or asset changes. The output is documentation only.
- **Rules source**: `data/MagicCompRules-20260227.txt` (official WotC CR text, effective
  2026-02-27, downloaded — NOT committed, WotC copyright; sliced into `data/sec-*.txt`).
  Auditors read the local slices, never memory or the web.
- **Supported game types** = two-player constructed/limited "classic" Magic (the engine's demo,
  selfplay, and duel modes). OUT OF SCOPE: CR 8xx (multiplayer), CR 9xx (casual variants),
  Momir (non-CR house variant), Vanguard/Planechase/Archenemy/Conspiracy/Commander. Rules
  inside 1xx-7xx that exist only for excluded formats are classified N/A.
- **Granularity**: classify at the sub-rule level (e.g. 601.2c), one verdict per rule id.
- **Verdicts**: IMPLEMENTED (engine honors it; cite code evidence file:line) · PARTIAL (some
  sub-behavior honored, some not — explain the split) · INCORRECT (engine does something
  contradicting the rule — cite code AND state what the engine does vs what the rule says) ·
  UNIMPLEMENTED (no engine machinery for it) · N/A (excluded game types / physical-play-only
  rules e.g. shuffling etiquette, sideboard physicality).
- **Evidence discipline**: every IMPLEMENTED/PARTIAL/INCORRECT verdict cites engine code
  (`src/File.cpp:line`) or a test fixture. Known-truth inputs the auditors should consult:
  `strategy-design/lexicon-triage/engine-bugs.md` (catalogued known defects — an audit hit that
  matches a catalogue entry should say so), the test suite (`bin/Res/test/`), and
  `docs/testsuite-fixture-authoring.md`. UNIMPLEMENTED needs a good-faith search (grep the
  obvious identifiers) — absence of evidence after a named search is acceptable evidence.
- **Completion bar per section**: `python3 check-coverage.py data/<section>.txt <report>.md`
  reports MISSING: 0. A section report without a green checker run is not done.

## Section assignment (7 auditors)
| report | CR scope | rule ids |
|---|---|---|
| 10-concepts-cardparts.md | 1xx + 2xx | 623 |
| 11-types-zones.md | 3xx + 4xx | 241 |
| 12-turns.md | 5xx | 148 |
| 13-casting.md | 601-608 | 168 |
| 14-effects.md | 609-616 | 151 |
| 15-additional-except-702.md | 7xx minus 702 | 733 |
| 16-keywords-702.md | 702 | 754 |

Report format (uniform, machine-mergeable): a markdown table per hundred-block —
`| rule | verdict | evidence / note |` — followed by a per-section summary counting each
verdict class, followed by a HIGHLIGHTS list (the audit's real payload: the incorrect and
notable-unimplemented findings, in priority order for the engine ledger).

Synthesis: 99-summary.md merges the per-section counts + all HIGHLIGHTS into the flagship
flagging document the owner asked for.
