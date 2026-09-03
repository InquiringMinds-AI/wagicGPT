# Wave-55 lane B — combat seams (D3 HIGH, D13 MED, D14 MED)

Base: master `8a1768d1c`. Branch `w55-lane-B`. Worktree `worktrees/lanes/w55-B`.
All three items are the `152v146` seq **58** window (turn 19, Blockers, life 6 vs 7): two
blockers spread one apiece across two menace attackers, the CR 509.1c pass pruned BOTH, the
seat declared nothing at a lethal window, no follower record of any kind was written, and it
lost the game.

## What changed

### D3 (HIGH, = R195) — `all_assignments_illegal` now gets ONE re-ask carrying the pruned pairs
`AIPlayerGPT::chooseBlockers`'s all-illegal exit no longer settles on its first firing.

- New latch `mBlockIllegalReaskTurn` + `mBlockIllegalReaskPairs`, **separate from
  `mBlockReaskTurn`**: a one-blocker-many-attackers reply and a reply whose every pairing was
  pruned are different failures, and one firing must not spend the other's single arm.
- First firing: the record is written, `mBlockIllegalReaskPairs = pruned`, `return 1`. The next
  tick rebuilds the prompt with `prunedPairsReaskClause(...)` appended — a `[RE-ASK]` block
  quoting the engine's own pruned pairs back as text. The changed text is also what makes this a
  DIFFERENT prompt, so the async machinery issues a fresh call rather than replaying the
  rejected answer (the gang-conflict re-ask's own mechanism).
- Lane M's echo rule is kept and PARSETEST-proved: the clause carries no `B<digit>`/`A<digit>`
  code and not the word `blocks`, and the engine's own ` -> ` is rewritten to ` assigned to `
  so a model parroting the correction onto its `BLOCKS:` line cannot hand the name-form parser a
  ready-made phantom pair.
- Second all-illegal reply: **not re-asked again**. It settles, stamped
  `all_assignments_illegal_exhausted`, and — as every other failure exit on this seam does —
  hands the combat to the heuristic. I did **not** convert the exhausted exit into a literal
  blanket no-blocks: wave-32 ledger #15 and the existing comment both rule a blanket no-blocks
  the worst combat default (it maximises incoming damage and is the adjudication tiebreaker).
  The docket's "may still declare no blocks" is read as *the second reply is not forced into a
  block*, which is what the heuristic handoff already gives. If the intended reading was the
  literal one, this is the one line to change.
- Bounded by construction: one arm per turn; every other exit below is unchanged, so a
  re-ask that comes back unparseable falls to the existing `pairs == 0` heuristic exit.

### D13 (MED, = R205) — a count header above the B-rows
New pure `menaceBlockerBudgetLine(needs, totalAttackers, blockersAvailable)`, emitted directly
above `Your available blockers (...)`, built from the engine's own `minBlockersRequired()` —
the same source both A-line tags read, so header and per-attacker tags can never disagree.

`2 of the 4 attackers need TWO or more blockers each; you have 2 blockers, enough to complete at most 1 of those blocks. A lone blocker assigned to one of them does not block it at all and is wasted.`

Every per-attacker tag states the RULE (it was stated three times per blocker at s58); none of
them is the ARITHMETIC over the whole window, which is what that answer turned on. TRUST
DOCTRINE: `maxSimul` spends the CHEAPEST minimums first and ignores per-blocker legality, which
can only reduce the true maximum — so "at most N" cannot over-claim. Silent when no attacker
carries a declaration minimum. No affirmative permission substring for a skimming model to latch.

### D14 (MED, = R206) — the fallback class names the pairs and the reason
`allAssignmentsIllegalClass(prunedPairs, exhausted)` →
`all_assignments_illegal: Elite Spellbinder -> Spider (needs 2 blockers, only 1 assigned); ...`
The class NAME is still the leading token, so every existing grep still hits, and
`pruned_pairs` stays on the record unchanged. One field; no new computation. Both firings
(first and exhausted) carry it.

## Files
- `projects/mtg/src/AIPlayerGPT.cpp` — three pure builders, the prompt clause, the D13 header
  emission, the rewritten all-illegal exit, ctor init, +23 PARSETEST cases under `[#W55-B]`.
- `projects/mtg/include/AIPlayerGPT.h` — `mBlockIllegalReaskTurn`, `mBlockIllegalReaskPairs`.

## Gate (this worktree, memory-capped, foreground-waited)
| | baseline (seeded 8a1768d1c binary) | after |
|---|---|---|
| PARSETEST | 2489 passed / 0 failed | **2512 passed / 0 failed** (+23) |
| suite | 1228 tests, **2 failed**, 51 AI / 0 failed, **0 timeouts** | **1228 tests, 2 failed, 51 AI / 0 failed, 0 timeouts** |

Both runs failed exactly `lifeline.txt` + `merrow_reejerey.txt` — the brief's named
concurrency-only pair, byte-identical before and after. `git diff | grep -c $'\357\277\275'` = 0.
Clean link.

## Predictions (falsifiable, for the wave-55 corpus)
- **D3** — `all_assignments_illegal` records with **no follower record of any kind: 0/N** (was
  1/1). Every first firing is followed within the same turn by either a declared blockers
  record, an `all_assignments_illegal_exhausted` record, or one of the existing failure classes.
  Secondary: of the all-illegal firings that get a re-ask, **≥1 in 2 recovers** (declares at
  least one block) — thin-N, on the `named_row_reask` rate (5 of 7).
- **D13** — the header renders on **100% of blockers records whose window contains at least one
  attacker with `minBlockersRequired() > 1`**, and **0%** of the rest. Menace under-fill
  (`pruned_pairs` naming "needs N blockers, only M assigned") **drops below 1 per corpus**
  (was 2 pairs on 1 record, and that record lost a game).
- **D14** — **every** `all_assignments_illegal*` record's fallback field carries `: ` followed
  by at least one pair; **0** records carry the bare class while `pruned_pairs` is non-empty.

## What I did NOT verify
- **No live model run at all.** The 8084 endpoint was not exercised; every claim about the
  re-ask arm's *runtime* behaviour is a code-path argument, not an observation. The
  `152v146` s58 shape was not replayed.
- **No suite pin, and none is honestly constructible.** All three items live in
  `AIPlayerGPT::chooseBlockers`, which is gated on a live endpoint (`mEndpoint.empty()`); the
  test-suite driver speaks clicks and there is no scripted-model-reply harness. This is lane
  M's documented finding for the very same seam (wave53/lane-M.md), re-confirmed here. The
  RULES truth the D13 header claims is already pinned by the existing
  `menace_defender_one_blocker.txt` / `menace_defender_two_blockers.txt` fixtures, which stayed
  green; I added no fixture rather than add a green-on-base duplicate.
- **The D13 header's token cost was not measured** against a real blockers prompt (it is one
  line, only on menace windows).
- **The exhausted branch has never executed** — it needs two consecutive all-illegal replies in
  one combat, which the wave-54 corpus never produced (the class fired once, ever).
- **Interaction with the gang-conflict re-ask** (both arms firing in one turn) is reasoned about
  (separate latches, separate clauses, both may append) but never observed; two `[RE-ASK]`
  blocks in one prompt is a shape no corpus has seen.
