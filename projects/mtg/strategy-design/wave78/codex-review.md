# Wave 78 — Astra adversarial review of the step-one diff (1e465fff7..818f4d338)

Seat: Codex `gpt-6-astra` via `codex exec --sandbox read-only`, prompt ~/.gatelogs/w78-review-prompt.md (lanes CV/CW/CX). Verdict and findings verbatim below; Lane CY (wave78/lane-brief.md) maps each to CONFIRM/REFUTE.

# Verdict: not safe to ship as-is

Read-only review; nothing written. Applied the prompt-design working-memory skill. The saved diff matches the scoped `1e465fff7..HEAD` diff. Scoped `git diff --check` and Python syntax validation passed. Counterexamples below are **source-traced, not executed games**; I did not run the build, suite, PARSETEST, or an endpoint.

## Findings

### 1. HIGH — S3 collapses without the required board-key comparison

[AIPlayerGPT.cpp:41311](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:41311)

The new collapse treats a historical option-list decline as an answer to the current board and stop.

**Trigger:** Decline a list containing two token-making activations while already above the stated stop. Change one activation’s actual `[cost:]` while keeping both legal and the same named rows. `optionSetKeyOf` strips that cost annotation, so `declinedN` remains positive; both token-makers remain stop-priced and the window disappears. Unlike the hold key, this ledger does not preserve cost groups. It also contains **no board key or stop-generation identity**: restating a different already-reached stop this turn does not invalidate the earlier decline. The twelve-window pin supplies `declinedN` directly.

### 2. HIGH — S4 predicts future legality from a comparison with the past

[AIPlayerGPT.cpp:31145](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:31145)

An unchanged current menu does not establish that every remaining stack resolution preserves it.

**Trigger:** Two opposing triggers remain pending, one removing the permanent supplying your offered activation. Before either resolves, the menu matches the previous asked menu, so the annotation promises that the activation “stays available after.” It does not. The counter also counts **every unresolved opposing object**, including spells, rather than only triggers. The seven-link pin fixes rows unchanged by construction and cannot falsify either claim.

### 3. HIGH — S5 calls a conditional counter a guaranteed answer

[AIPlayerGPT.cpp:44709](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:44709)

Finding `fizzle` anywhere in a pending spell’s script does not prove that its target will be countered.

**Trigger:** Your Mana Leak targets their spell; they can pay three; you also hold an unconditional counter. Mana Leak’s repository primitive contains `donothing?fizzle`, so the new join prints that the second counter is spent for nothing unless yours is countered or removed. Paying three defeats Mana Leak without either exception occurring. The pointer-based target join is not the problem; the promised outcome is.

### 4. HIGH — S6 renders “the loop closes” through an opposing sweeper

[AIPlayerGPT.cpp:31122](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:31122)

The rendered RESOLVING verdict inherits a threat detector that recognizes targeting, not destruction of the loop.

**Trigger:** Your loop trigger is pending beneath an opposing untargeted enchantment sweeper that will remove both components. `w77OwnLoopStackState` finds your component-sourced event but no targeted component and returns RESOLVING. The new visible instruction says the loop closes without another action. Wave-77’s ordinary targeted-removal case was addressed; this non-targeting destruction case is now rendered more confidently.

### 5. MED — S9’s new “gone” count is bypassed on same-window rebuilds

[AIPlayerGPT.cpp:31441](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:31441)

The note-reuse guard checks `unseen` but ignores the newly computed `gone`.

**Trigger:** Commit `{A,B}`, build an unchanged held window at sequence N and suppress it, then rebuild `{A}` at the same N after B disappears. `unseen == 0` returns the cached “no row … is gone” note despite `gone == 1`. Separately, ordinal normalization affects only the bracket: `Goblin #1` becoming `Goblin #2` can produce “unchanged” while the live hold latch still compares the ordinal-bearing keys and reopens.

### 6. MED — S8 recognizes sentence boundaries inside unfinished quoted abilities

[AIPlayerGPT.cpp:539](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:539)

The clause cutter tracks neither quotation nor parenthesis nesting.

**Trigger:** Give it `Enchanted creature has "Tap: Draw a card. Activate only if you control a Wizard."` with a budget ending after the first sentence. It emits the granted activation followed by `(...more)`, omitting the activation condition and leaving the quotation unfinished. A period followed by a capital is not sufficient evidence of an independently truthful clause.

### 7. MED — S12 labels sampled net changes as actual gains and losses

[AIPlayerGPT.cpp:28040](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:28040)

Differences between turn snapshots cannot reconstruct life-gain and life-loss events.

**Trigger:** Samples are 20, 15, 18; between the first two, the opponent gains seven and loses twelve, then gains three. The new text reports GAINED +3 and LOST −5; actual events total +10/−12. If every sampled interval nets negative, the split disappears entirely—the original hidden-lifegain failure. No event stream feeds this calculation.

### 8. HIGH — S13 moves stripped annotations into unstripped ask-key text

[AIPlayerGPT.cpp:10137](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:10137)

Hoisted card text is emitted outside annotation delimiters and subsequently enters the ask and async-slot keys.

**Trigger:** Two rows share a ≥100-byte card-text gloss; rebuild the same action rows with only that gloss changed. Previously both glosses were stripped. Now `w77KeyTailOf(joinNumberedRows(rows))` retains the changed shared header. The new stability pin compares individual rows before/after erasure and **never includes the emitted header**, missing precisely the live-caller failure.

### 9. MED — S14’s “castable” ceiling ignores alternative costs and mana colors

[AIPlayerGPT.cpp:44465](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:44465)

Converted cost and source count do not establish that the retained instant is castable.

**Trigger:** An X-spell menu with Force of Will, another blue card, sufficient life, and a cheaper non-instant reports a five-mana reserve even though the repository supports Force’s alternative payment. Conversely, enough Mountains can satisfy the numerical reserve for Essence Scatter while providing no blue. The pin supplies already-selected costs directly instead of exercising candidate selection and payment feasibility.

### 10. MED — S1’s heuristic counter includes records outside its named class

[AIPlayerGPT.cpp:20640](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:20640)

The counter tests the raw shape flag, not the final deviation class.

**Trigger:** Reply only `2 (Hold priority)`, with no PLAN, then hand the window to heuristics. Classification is `unlabelled_plan`, but `w78LabelAbsent` is true and increments `answer_label_absent_heuristic_played`. Thus the counter cannot reconcile with records classified `answer_label_absent`.

### 11. MED — S11’s same-arm bound pin does not exercise the live continuation

[AIPlayerGPT.cpp:92477](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:92477)

The pin repeatedly supplies “busy=true,” whereas the continuation checks only the parked arm.

**Trigger:** Enter continuation with a deferred close, this arm still armed, and an empty park—the state the pin purports to cover. At line 18756, the live predicate receives `mRetryPark.forceCloseArmed == false` and arms immediately, superseding this arm rather than waiting eight ticks. The fixture’s counter reading one is local simulation, not a call through `pollCompletionRetry`.

### 12. MED — The census reconciliation omits this diff’s `forced_close` kind

[corpus-stats.py:154](/home/magi/Projects/wagicGPT/projects/mtg/tools/corpus-stats.py:154)

The total includes forced-close records but its explanatory reconciliation excludes them.

**Trigger:** One ask, one `forced_close`, and gameend `protocol_replies: 1`. Executing the actual Python tool with mocked read-only inputs prints a difference of one followed by an empty explanatory kind list.

## Checks and shipping conclusion

Golden protocol bytes are unchanged; I found no new unlabelled-answer acceptance in the diff. The constructor initializer survives the comment, though the list is not declaration-ordered; the merged PARSETEST blocks close correctly. Deck130’s maximum is indeed 30,125 bytes with five rows and roughly 21,918 bytes of preceding narration. Narration has a 24,000-byte trim trigger, not a whole-prompt bound.

**Do not ship as-is.** The collapse exceeds its authorized identity test, several new instructions assert outcomes the engine has not established, and key/caller coverage remains weaker than the green pins claim.