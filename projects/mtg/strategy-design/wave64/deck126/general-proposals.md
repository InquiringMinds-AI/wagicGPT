# deck126 — general proposal (one item)

## P1. State the PLAN carry budget as a NUMBER in the reply protocol

**Layer note for synthesis**: the text this changes is the code-appended reply protocol
(`src/AIPlayerGPT.cpp:233`, the `LINE 2 is a PLAN: line` block), not `general-strategy.md` prose.
If the routing rule sends prompt-text-emitted-by-the-engine to the ledger rather than the general
guide, route it there — the argument below is the same either way.

**Current text**: `"LINE 2 is a PLAN: line - your complete game plan from this point on, CONCISE,
a few sentences of intent, not an analysis."`

**Proposed**: append one clause naming the actual bound, e.g.
`"...not an analysis. Only the first 400 characters of it are carried to your next decision; past
that it is cut and the cut is shown to you, so anything you write beyond about two sentences is
work you will not get back."`

**Why this is not a restatement of an existing amendment.** The skill already records the ~400-char
observation (wave-59 edition, line 1524: deck36, 51 of 132 prompts over the bound) and the engine
already ships the truncation NOTICE and the `kPlanCarryMaxChars = 400` cut
(`AIPlayerGPT.cpp:395`, W60-M B13a). Both of those are downstream repairs — they fix what the
reader is shown AFTER the tokens are spent. Nothing anywhere states the budget to the writer
BEFORE it writes. "CONCISE" and "a few sentences" are adjectives with no observable test; 400 is
one.

**Evidence (deck 126, wave 64, 271 windows / 6 games).**
- 191 replies carried a PLAN line: median 312 chars, max **5,618**.
- **62 prompts** carried `[...the rest of your plan was not carried: N further characters, of M
  you wrote]` — i.e. the seat was told, 62 times, that it had overspent, and kept overspending.
  Largest single discards: 3,315 of 3,542 chars
  (`1788671775-ai_baka_deck126-0x55c433487cc0-vs-ai_baka_deck125.jsonl` seq 149) and 2,917 of 3,259
  (same file, seq 155).
- The four slowest decisions of this seat's whole corpus are all long replies, against a 13.3 s
  median: **505 s / 5,794 chars** (`1788671754-...-vs-...deck123` seq 21), 443 s / 2,507
  (`...deck125` seq 41), 277 s / 1,573 (`...deck125` seq 32), 231 s / 2,184
  (`1788671748-...-vs-...deck130` seq 15). This seat spent 141 minutes of model time total; the
  long tail of discarded plan prose is a large fraction of it.

**Falsifiable prediction if adopted**: next wave, the count of windows carrying the
`the rest of your plan was not carried` notice falls materially below this wave's 62/271 (23%)
for this seat, and the maximum PLAN length falls below ~1,500 chars, with no change in the
`plan_choice_conflict_*` or `decision_reversed_in_prose` counters (the clause budgets the plan; it
must not suppress plans, so case (a)/(b) PLAN-line presence should be unchanged).

**Risk**: a hard number can be read as a hard rule and suppress a genuinely needed re-plan.
Mitigation is in the wording above — it prices the overspend ("work you will not get back") rather
than forbidding length, and leaves the three-case mechanical test for WHETHER to write a plan
completely untouched.
