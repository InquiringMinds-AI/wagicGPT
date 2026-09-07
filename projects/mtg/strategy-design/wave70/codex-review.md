# Wave-70 step-one adversarial review

Seat: Codex CLI 0.153.4, model gpt-6-astra (`codex exec --sandbox read-only`), 2026-09-06 ~21:50 local. Prompt: `~/.gatelogs/w70-review-prompt.md`. Diff reviewed: master 52ab71610..71b1e22be (lanes BK+BL+BM merged). Findings are routed to fix lane BN (lane-brief.md, section BN).

---

# Verdict: not safe to ship as-is

The merged code still executes and vetoes actions using reply prose, can turn a legal counted action into a pass, and contains regime-gate paths that reject valid records or accept insufficient evidence.

Read-only verification: inspected the merged source, saved diff, audit and lane reports; ran the instruction guard (**OK**) and exercised the actual Python gate with in-memory records. No files were written. I did not rebuild, run the game/PARSETEST, or contact a model endpoint.

## Findings

### 1. HIGH — Trailing prose still vetoes a legal block declaration

[AIPlayerGPT.cpp:45596](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:45596), predicate at [42668](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:42668).

`truncatedBlockCommitmentAbandoned` scans prose after `BLOCKS:` and replaces otherwise legal assignments with **no blockers**.

**Trigger:** `finish_reason=length`, legal B1→A1, and:
```text
PLAN: block their attacker
BLOCKS: B1:A1
I should not block
```
The tail satisfies the global-decline predicate; the driver applies an empty declaration. This directly survives the supposedly deleted prose-reversal handling and can make lethal damage connect.

### 2. HIGH — Combat still manufactures declarations from unlabelled prose

[AIPlayerGPT.cpp:44449](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:44449), [45674](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:45674).

`salvageProseAttackers` and `salvageProseBlocks` remain live fallback paths over the **whole reply**.

**Trigger:** reply `I will attack with A1 and A3` executes those attackers without either required line; `I will block A3` selects the unique legal blocker when one exists. These paths precede truncation recovery. PARSETEST still positively pins prose salvage at [47930](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:47930) and [47958](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:47958), protecting the forbidden behavior.

### 3. MED — The “census-only” no-op predicate still changes re-ask text and classification

[AIPlayerGPT.cpp:31632](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:31632), consumer at [35115](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:35115).

`planArguesAgainstRow` takes everything after `PLAN:`, including the action and trailing prose; `noopPlanConflict` still selects “and your reply says so too” and the fallback classification.

**Trigger:** choose a zero-effect Tribute to Hunger row with:
```text
PLAN: cast Tribute to Hunger
CHOICE: 1 (Cast Tribute to Hunger)
This is a waste.
```
The action names the row, then the pronoun rule attributes the trailing argument to it. Removing that prose changes the re-ask and record. This is not an unused diagnostic.

### 4. HIGH — Prose-derived stop numbers can replace a legal repeat action with pass

[AIPlayerGPT.cpp:33969](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:33969), [34279](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:34279).

`repeatPlanStopAndCurrent(content, ...)` scans the **whole reply**, and the retry-exhaustion arm clamps the counted action to zero.

**Trigger:** a legal repeat-row `x3`, a number-free `PLAN: create three humans`, and trailing `Correction: stop 20; M 25`. The first answer earns the stop re-ask; repeating it sets `allowed=0` and resolves as pass. Thus off-protocol text both supplies the decision constraint and removes a legal selected action.

### 5. HIGH — A configured answer ceiling still bounds native reasoning

[AIPlayerGPT.cpp:31042](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:31042), test at [75490](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:75490).

The zero-budget branch computes reasoning from the same configured ceiling that was designated an **answer** ceiling.

**Trigger:** thinking on, `reasoning_budget=0`, `max_reply_tokens=400` (or matching environment override), ask seam. Resolution is answer=400, reasoning=0, total=400, while `enable_thinking=true`. Those 400 tokens bound the entire reasoning-bearing completion. PARSETEST explicitly accepts this starvation rather than refuting it.

### 6. HIGH — Successful forced-close recovery fails the thinking-on gate

[regime-gate.py:124](/home/magi/Projects/wagicGPT/projects/mtg/tools/regime-gate.py:124), emitter at [AIPlayerGPT.cpp:16949](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:16949).

The gate mistakes the **last request’s** zero reasoning allocation for the entire decision having no reasoning.

**Trigger:** phase one produces native reasoning; phase two successfully returns the answer with thinking disabled. The record retains native reasoning and `reasoning_forced_close=true`, but writes `max_tokens_reasoning=0` from phase two. The actual gate returned **FAIL** for this record shape. There is no forced-close exception.

### 7. MED — Engine-only records can falsely identify a current binary as stale

[regime-gate.py:80](/home/magi/Projects/wagicGPT/projects/mtg/tools/regime-gate.py:80), emitter at [AIPlayerGPT.cpp:16630](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:16630).

The gate treats presence of `reply` without `reasoning_chars` as missing instrumentation, although `writeTransLog` always writes `reply` and deliberately omits `reasoning_chars` when the prompt is empty.

**Trigger:** the early sweep sees engine-answered records before any completed model round trip. `unstamped>0`, `records=[]` returns **FAIL — rebuild**, not WAIT. I reproduced that gate verdict.

### 8. HIGH — Five records from one seat permanently approve an unchecked corpus

[regime-gate.py:95](/home/magi/Projects/wagicGPT/projects/mtg/tools/regime-gate.py:95), [selfplay-harness.sh:412](/home/magi/Projects/wagicGPT/projects/mtg/tools/selfplay-harness.sh:412).

The minimum is pooled across files, not enforced per seat/game; PASS permanently disables further checks.

**Trigger:** one fast seat supplies five valid thinking-on records while another has no completed calls. The gate returns **PASS**—reproduced in memory. If the slower seat subsequently returns no reasoning, it is never inspected. The required per-seat evidence was never established.

### 9. HIGH — Thinking-off accepts affirmative evidence of hidden reasoning

[regime-gate.py:138](/home/magi/Projects/wagicGPT/projects/mtg/tools/regime-gate.py:138).

The off branch checks only `reasoning_chars`, ignoring `reasoning_hidden` and positive `reasoning_tokens`.

**Trigger:** five records stamped off, two-line replies, `reasoning_chars=0`, `reasoning_hidden=true`, `reasoning_tokens=1200`. The actual gate returns **PASS**. The engine already emits those evidence fields; withheld reasoning is not proof of thinking off.

### 10. HIGH — Plan consumption tracks parsing, not successful execution

[AIPlayerGPT.cpp:19347](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:19347), [19413](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:19413).

A new plan starts at zero consumed steps, while repeated or omitted plans advance during `consumePlan`, before validation/execution.

**Trigger:** first reply plans `cast Bear; attack; cast Rhino` and successfully casts Bear: the next window still receives `cast Bear`. Conversely, an invalid echoed answer advances the pointer before its re-ask; another echoed reply can advance again although nothing executed. Even an empty reply can advance the omission branch.

### 11. MED — Ordinary commas split card names and timing clauses into fake steps

[GptPlanCaveat.h:893](/home/magi/Projects/wagicGPT/projects/mtg/include/GptPlanCaveat.h:893).

Every punctuation boundary followed by whitespace becomes a step, without distinguishing action separators from punctuation inside an action.

**Trigger:** `cast Thalia, Guardian of Thraben; attack` becomes three steps; consuming one carries `Guardian of Thraben; attack`. Likewise, `after blockers, cast Giant Growth` splits timing from its action. These are ordinary valid sequence descriptions, not malformed output.

### 12. MED — Rewritten guide templates still request extra ledger output

[deck123_strategy.txt:70](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave70/guides/deck123_strategy.txt:70), [deck130_strategy.txt:113](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave70/guides/deck130_strategy.txt:113).

The replacement examples retain multiline board/count ledgers beneath `PLAN:`.

**Trigger:** following deck123’s literal template emits the plan, a separate `stop … M is …` line, then CHOICE; deck130 similarly requests a separate `Rorix/Commander <yes/no>` continuation. Joining the lines still produces board-state ledger content rather than solely intended actions. The link guard scans neither guide.

## Shipping verdict

**Do not ship as-is.** The green instruction guard demonstrably misses surviving reply-shaping defects, existing PARSETEST cases endorse forbidden prose execution and reasoning starvation, and the regime gate has reproducible false-PASS and false-FAIL paths. The merged behavior does not yet implement the governing two-line contract or reliably establish corpus regime; successful native-reasoning recovery can itself kill the corpus.