# Wave-53 lane M — blockers seam (D1 HIGH, D23 LOW, D19 LOW)

Base: master `3dfd77d65`. Branch `w53-lane-M`.

## What changed

### D1 (HIGH) — a `multiblock_reask` could end with ZERO blocks and no record
The W36 re-ask **discarded the conflicted reply whole** and returned, hoping a second
model call would land. In `152v162` seq 26 it did not: record `multiblock_reask`,
`chosen_text` null, **no follower record of any kind**, no second call, zero blocks, and
the two attackers the reply had asked to block took the seat 24 -> 18. The reply's LEGAL
half (`B2:A1`, a 6/4 eating a 3/4 for free) went out with the illegal duplicate.

The illegal duplicate now costs the **duplicate only**:

- `countLegalAssignments(pick, nAttackers, legalPerBlocker)` (new file-static) asks whether
  the first-wins reading still holds a pairing the engine already offered — counted against
  `DecisionRequest::legalPerBlocker`, the same set the B# lines were rendered from.
- When it does (`conflictLegalPairs > 0`), `chooseBlockers` **declares that reading on the
  tick it parses it**, through the unchanged `canBlock()` / CR 509.1c validators, with
  parse note `multiblock_first_wins`. There is no longer a window in which the declaration
  can go missing, because there is no longer a tick between parsing and declaring.
- The re-ask survives for the ONE case with nothing to lose: a conflicted reply whose every
  pairing is illegal or absent. Every exit below it writes its record, empty result included.
- `mLastParseNote` is now **appended** to (`appendParseNote`) rather than overwritten, so the
  re-ask class no longer erases `multiblock_first_wins` / `blocker_handle_in_attacker_slot`.
- Every settled blockers exit now prints a line carrying the exact substring
  `declared blocks from N assignment(s)` — the declared path (unchanged wording), the
  explicit all-decline, both truncated-abandoned exits, `all_assignments_illegal`, and the
  unusable-reply heuristic handoff. A corpus can now pair each blockers ask with exactly one
  settlement line, and a missing one is loud. (The vacuous-builder early return is NOT
  covered: it never asks and never writes a record.)

Prevention moved into the prompt (D23), so the re-ask is a smaller surface, not a bigger one.

### D23 (LOW) — the BLOCKS format line carries the one-per-blocker constraint
`... e.g. "BLOCKS: B1:A2, B3:A1, B2:none" - each B-number at most ONCE, and several
B-numbers may share one A-number - or exactly "BLOCKS: none" ...`. No B<digit>/A<digit>
code in the new clause, so an echo cannot mint a phantom pair (PARSETEST echo-shape case).
Two of three corpus `multiblock_reask` fallbacks came from a repeated B-number.

### D19 (LOW) — a blocker handle in the attacker slot is dropped silently
`parseBlockAssignments` gains `bool * blockerInAttackerSlot` (defaulted NULL). It fires when
a well-formed `B<n>:` is followed by another B-handle where an attacker belongs (`B3:B1`).
The pair still drops — the fix is representation only: the record now carries parse note
`blocker_handle_in_attacker_slot` instead of reading as a clean parse that quietly discarded
three lifelink Vampires (`126v162` seq 21, `dropped_assignments: 6`).

## Files
- `projects/mtg/src/AIPlayerGPT.cpp` — parser flag + `countLegalAssignments` + the
  chooseBlockers decision + the settlement traces + the D23 format line + 11 PARSETEST cases.
- `projects/mtg/bin/Res/test/blocker_one_attacker_only_w53m.txt` — new fixture.
- `projects/mtg/bin/Res/test/_tests.txt` — registers it.

## Gate (worktree, memory-capped)
| | baseline (this worktree, base source) | after |
|---|---|---|
| PARSETEST | 2121 passed / 0 failed | **2132 passed / 0 failed** (+11 cases) |
| suite | 1210 tests, 2 failed; 44 AI, 0 failed; 0 timeouts | **1211 tests, 2 failed; 44 AI, 0 failed; 0 timeouts** |

The suite failure COUNT is the invariant, not the pair's identity: the baseline run failed
`lifeforce.txt` + `mephidross_vampire.txt`, the after run failed `library_of_alexandria3.txt`
+ `maze_of_ith.txt`, and the brief named `lifeline.txt` + `merrow_reejerey.txt`. Three
different pairs, always exactly two, always solo-green — the known concurrency-only class.
`git diff | grep -c $'\357\277\275'` = 0.

## The suite fixture — stated plainly
`blocker_one_attacker_only_w53m.txt` pins the rules truth the whole duplicate accounting
rests on (a creature is assigned to at most ONE attacker; the second assignment MOVES it,
so exactly one attacker connects and the defender goes to 17). It is **GREEN ON BASE** —
measured, by reverting `src/AIPlayerGPT.cpp` to `3dfd77d65`, rebuilding and running it
scoped. It is therefore a REGRESSION GUARD, not a RED-on-base/GREEN-after pin.

There is no RED-on-base suite pin for D1, and I could not honestly manufacture one: D1 lives
in `AIPlayerGPT::chooseBlockers`, which needs a live endpoint and the engine tick loop, and
no scripted-model-reply harness exists in the suite (`mEndpoint.empty()` gates every GPT
seam, and the test-suite driver speaks clicks). The pins that DO reach the changed code are
in PARSETEST under `[W53-M]`: the seq-26 shape, the seq-21 shape, the D23 echo shapes, and
three cases on `countLegalAssignments` — the decision the seq-26 record turned on. Those
eleven cases cannot compile against base at all (new signature, new helper), which is the
strongest "red on base" available here and is weaker than the brief asked for.

## Falsifiable wave-53 prediction
1. `multiblock_reask` records with **no follower record and no `chosen_text`**: **0/N**
   (wave-52: 1/3).
2. Every blockers ask in the corpus is followed in stderr by exactly one line containing
   `declared blocks from N assignment(s)`: **N/N**.
3. `multiblock_reask` records themselves: **strictly fewer than 3 per 3,300 decisions** and
   every one of them shows `dropped_assignments >= 1` with **zero** legal pairings kept —
   any re-ask whose reply held a legal pairing is a falsification of the new gate.
4. A new parse note `multiblock_first_wins` appears wherever wave-52 logged
   `multiblock_reask` on a reply that had a legal pairing (2 of the 3 corpus cases).
5. `blocker_handle_in_attacker_slot` appears on the `126v162` seq-21 family (3 records in
   wave-52, un-noted).
6. D23's clause should push (1)-(5) toward zero occurrences outright; if repeated B-numbers
   do NOT fall relative to wave-52's 3-per-corpus, D23 is worth nothing and should be cut.

## What I did NOT verify
- **The mechanism behind the missing second call is still not pinned.** I read the repro
  stderr closely and can rule some things in and out but not settle it: the `[combattrace]`
  suppression tracer printed only nine `no-legal-attacker` lines (all at the attackers
  phase, none at blockers) in that whole game, so `pendingCombatDecision` was never
  suppressed at the blockers step; and `NextGamePhase: held` (the `ActionStack.cpp:133`
  W36-item-6 guard) fired **twice in the entire corpus** and not in that game — so the
  phase-advance guard did not hold while the declaration was due. WHICH of that guard's
  gates was false at seq 26 is unknown to me. My fix routes around the question by removing
  the tick between parsing and declaring for the case that had something to lose; the
  question itself is still open and belongs to whoever takes the async-window seam.
- **No live-model run.** Nothing here was exercised against `qwen36-35b-a3b` or any endpoint;
  every claim about the new path is from PARSETEST plus reading. The prediction above is the
  test.
- I did not measure whether the ONE remaining re-ask case (nothing legal to keep) can still
  be lost the same way. It can, in principle — it is now a strictly smaller set (0 of the 3
  wave-52 cases had zero legal pairings), and its loss costs a heuristic block rather than a
  parsed one, but it is not closed.
- I did not check whether `mBlockReaskTurn`'s turn key is right for a game with two combats
  in one turn (extra combat phases). Pre-existing, untouched.
- I did not touch the engine, the render, `MTGRules.cpp`, `GameObserver.cpp` or any card
  script. `PrecompiledHeader.h.gch` is a build artifact and is not committed.
