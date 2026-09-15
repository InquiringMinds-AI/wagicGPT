# GPT-layer audit

**The most consequential findings are not dead emitters. They are legal-option suppression, answer reuse without a board-state key, an unreachable watchdog on the ordinary AI path, and a new crack-back calculation that treats blockable damage as unblockable.** There is also genuine removable residue, but the census’s Tier A is not a safe deletion list.

This is a **read-only, static audit with corpus inspection**, not a runtime validation. I made no edits, builds, game runs, test runs, or commits. Searches used `/usr/bin/grep`; Python only read and sampled JSONL.

## Baseline and evidence

- The checkout is **`e71f73c5a`**, not the stated `62a5733ec`. However, `git diff --exit-code 62a5733ec HEAD --` for the two requested files returned **0 with no differences**. Their working-tree diff is also empty. The audit therefore applies to the requested versions of those files.
- The census describes the older, unsplit file. Its locations and classifications need revalidation, especially its [no-production-caller claim](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/audit-2026-09/gpt-layer-census.md:90) and [Tier A](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/audit-2026-09/gpt-layer-census.md:378).
- I read all 42 corpus files programmatically and confirmed **2,135 prompt-bearing records**. I inspected complete composed examples of opening-hand, land, priority, attacker, blocker, and discard decisions; additional longer priority/reveal documents and specific clause families were sampled. **The corpus predates some current fixes: an old prompt demonstrates exposure, not necessarily a still-current defect.**
- `AIPlayerGPTSelfTest.cpp` was not read or audited.

# 1. CRUD

## 1.1 Confirmed removable residue

| Item | Evidence and disposition |
|---|---|
| **Two abandoned combat-restatement helpers** | `combatDirectiveNegatedOnLine` and `combatDirectiveExampleEcho` have definitions but no production references in the searched source/header set. The following comment says their parent mechanism, `restatedCombatDirective`, was deleted. **DELETE both helpers and their obsolete explanatory block.** [Definitions](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:57986), [second helper and deletion comment](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:58016). |
| **Uncalled whole-board loop caution wrapper** | `loopCautionForBoards` has no production caller. The live successor, `loopCautionForLine`, adds the necessary ownership/scope distinction. **DELETE the old wrapper, not `loopHalvesInZone` or the underlying loop facts, which the successor still uses.** [Old wrapper and successor](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:27536). |
| **Old single-slot replay-run state** | `mAskReplayKey` is only cleared; `mAskReplayRun` is initialized/reset, not used to enforce the current replay bound. The live bound uses the per-window maps. **DELETE these two members and their resets.** [Header](/home/magi/Projects/wagicGPT/projects/mtg/include/AIPlayerGPT.h:716), [turn reset](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:48652), [answer reset](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:49290). |
| **Obsolete plan-block calculation** | `consumePlan` computes `planStart` and `planEnd`, including `planBlockEndOffset(...)`, then explicitly discards both. The comment says the demotion mechanism that used them was deleted. **DELETE these local calculations.** This does not change answer selection or the owner’s protocol. [Calculation and discarded results](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:25576). |
| **Duplicate assignments after plan processing** | `mPlanSetTurn` and `mCurrentPlan` are assigned before the stop-extraction block and assigned the same values again after it. The intervening block does not change either assigned value. **DELETE the trailing duplicate assignments.** [Plan update block](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:25722). |
| **One exact duplicate script scanner** | `lineIsActivatedCost` and `w80ScriptLineHasActivationCost` implement the same character walk, nesting rules, top-level brace test, and colon return. Differences are local variable names and comments. **DELETE one implementation after redirecting its callers to the other.** [First](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:16331), [second](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:29366). |

### Telemetry-only removal candidate

**`plan_echo_count`: DELETE candidate, with a narrowly stated evidence boundary.**

Its counter has no gameplay consumer; its remaining purpose is emitting the field. I found no consumer of `plan_echo_count` in `projects/mtg/tools/*.py`. It is reconstructible from replies if needed for an occasional investigation. Removing the counter, resets, increment, and field would change neither prompts nor decisions. However, I cannot establish that the owner never reads it manually or that an external tool does not consume it. [Writer’s explicit “REPORT field” comment](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:22444), [increment](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:25701), [member](/home/magi/Projects/wagicGPT/projects/mtg/include/AIPlayerGPT.h:1598).

## 1.2 Tier A candidates that are **not dead**

The census conflates three different things: a literal absent from prompts, a function absent from production, and functionality unnecessary to the game. Its proposed deletions do not survive those distinctions.

| Census candidate | Current-source verification | Disposition |
|---|---|---|
| **Own-loop “six emitters”** | These are not six interchangeable emitters. `w78OwnLoopVerdictLine` supplies state-specific prose; `w79OwnLoopVerdictLineFor` supplies fallbacks; `w80ProvenWinLoopLine` handles starter/no-starter wording; `w77OwnLoopVerdictKey` feeds hold state; `w79VerdictDangerRank` compares state; `w80LoopFaceOfLine` classifies telemetry. [Composer](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:34468), [key and ranking](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:34580), [telemetry classifier](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:36730), [production render call](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:47438). | **KEEP—live, not dormant.** I independently found **18 prompts** containing `[own loop verdict:`. |
| **Crack-back LETHAL/none variants** | The `none` path intentionally emits nothing. The lethal paths distinguish survival through a legal block, unavoidable damage within the search model, and an unknown best block. Zero corpus occurrences do not make those branches unreachable. [Emitter](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:36546). | **KEEP**, subject to the correctness finding in §3. |
| **`zeroPowerAttackerTag` / `[deals 0…]` family** | The attacker helper is called while constructing attacker rows. The blocker helper has conditional outputs for trample, menace, and damage ranges. [Attacker helper](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:6885), [blocker helper](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:6916), [attacker caller](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:58743). | **KEEP.** Individual absent strings are not proof that the family is dead. |
| **`removalVictimTag` and `spellRemovalVerb`** | The scan recognizes removal; the formatter distinguishes opposing, own, and indestructible targets. Both are used by cast rows. [Scan](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:41456), [formatter](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:41513), [call chain](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:51141). | **KEEP—live.** |
| **`namedCastPenaltyScan`** | Used both to annotate a named cast and to compute the life surcharge used elsewhere. Deleting it is not removing an unused sentence; it removes a fact scan from decision arithmetic. [Scan and surcharge](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:41328), [cast caller](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:50534). | **KEEP—live.** |
| **`stackAnswerYesRowClause`** | Called from `describeAction`; conditionally renders when life gain changes a lethal stack into survival. [Helper](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:39856), [caller](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:43490). | **KEEP.** Rare survival evidence is not CRUD. |
| **`w75ApplyChainFeed`** | Called on priority, cast, and target rows; its early return depends on the opponent’s loop state. [Helper](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:34336), [priority callers](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:46807), [cast caller](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:51612), [target caller](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:55754). | **KEEP—live.** |
| **`w80SacrificeSpendsBlockerClause`** | Conditional cost-consequence annotation, called from `describeAction`. [Helper](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:29683), [caller](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:43579). | **KEEP pending correctness/coverage work**, not DELETE by reach. |
| **`cannotPayNowClause`** | A live conditional annotation in `describeAction`. [Helper](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:9426), [caller](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:43673). | **KEEP**, not established dead. |
| **`asyncDropTraceLine`** | A diagnostic formatter, not a model emitter. Its production call is under `DebugTrace`. [Helper](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:19305), [caller](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:19740). | **KEEP as gated diagnostics**, unless the diagnostic itself is intentionally retired. |
| **`w81FoldDuplicateCoverParagraphs`** | Called on the cast menu. It substitutes a reference to an earlier identical cover paragraph. [Implementation](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:40991), [caller](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:51658). | **KEEP—live token-reduction mechanism.** Its absence from the older corpus does not prove uselessness. |
| **`gptPadlogFile`, `gptTracePromptDrift`** | Both are called through macros. The census’s no-caller classification missed macro-mediated reach. [Padlog macro](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:18116), [drift macro](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:19144), [live calls](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:19705). | **KEEP—compile-time-gated diagnostics.** |
| **`gptNoteEngineRevealFloor`, `noteRevealStall`** | They have external engine callers. [Reveal-floor caller](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:1489), [stall caller](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:1122), [virtual declaration](/home/magi/Projects/wagicGPT/projects/mtg/include/AIPlayerGPT.h:2555). | **KEEP—live hooks.** |
| **`revealSinglePickDeclineLegal`** | The current helper is a small arity parser, not the census’s purported 271-line emitter. Its reference in this file is a self-test access bridge, not the live reveal seam; the live seam receives `singlePickDeclineLegal` as an argument. [Helper](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:61240), [live signature](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:61511), [bridge](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:63291). | **Not approved for deletion in this audit.** It is test support; evaluating whether that support is redundant would cross the excluded self-test scope. |

### KEEP-DORMANT: missing card-class coverage

The following conditional capabilities should stay. Their source predicates describe real card classes, not vestigial wave branches:

- Changeling/tribal annotation: [5508](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:5508).
- Mutated piles: [9818](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:9818).
- Infect/toxic/wither annotations: [13942](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:13942).
- Hybrid-pip explanation: [49361](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:49361).
- Three-or-more-blocker restriction variant: [8114](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:8114).
- Planeswalker/battle-only attacker destination: [58625](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:58625), [battle row](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:59605).
- Unseen MDFC arrival variants: [50016](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:50016).
- Adventure spell text: [50492](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:50492).

**Do not classify the entire MDFC or cycling families as dormant:** real sampled prompts contain both. An absent variant is narrower than an absent mechanic.

## 1.3 The zero counters are not unread counters

`corpus-stats.py` **generically consumes every integer gameend field**, excluding only four metadata keys. Consequently, searching for a counter’s literal name in tools and finding nothing is insufficient. The 26 zero counters cannot be called unconsumed on that basis. [Generic consumer](/home/magi/Projects/wagicGPT/projects/mtg/tools/corpus-stats.py:465).

There is a genuine duplicate telemetry value: `force_close_arms_refused` and `force_close_arms_deferred` serialize the same member. This is a **schema-alias retirement candidate**, not two independent mechanisms. Removing an alias loses compatibility with readers of that name, not information. [Writer](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:22961).

## 1.4 Comment archaeology worth deleting or replacing

These comments actively misdescribe the current implementation:

- The priority sort commentary says the staleness key is the **full prompt**, although the live key is scope plus continuation digest plus normalized tail. [Old explanation](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:46626), [actual key](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:47393).
- `consumePlan` describes selecting the **last** answer label; the selector uses the current first-clean selection policy. [Comment](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:25556), [selection](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:25221).
- The repeat-cache comment says “the cache above wants the board too”; it no longer does. [Comment](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:48903), [actual key](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:48805).
- The removed combat-restatement explanation calls robust prose recovery forbidden. That is contrary to the current owner ruling, besides describing deleted behavior. [Archaeology](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:58033).

**DELETE obsolete history; retain concise descriptions of the current invariant.** Blanket removal of wave tags is not justified: some still locate real constraints.

# 2. NEEDLESS COMPLEXITY

| Current mechanism | Simpler equivalent | What it would lose |
|---|---|---|
| **Duplicate activation-cost scanners** | One shared scanner with the existing behavior. [16331](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:16331), [29366](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:29366). | No behavior; one independent maintenance surface. |
| **Nearly mirrored draw-forecast emitters plus a dispatcher** | Compute one forecast fact object; format with explicit subject/possessive/life-recipient fields. Preserve the own-side compulsory-draw sentence and opponent-side “you gain none” distinction. [Their formatter](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:16011), [own formatter](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:16079), [dispatcher](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:16137). | No intended output loss. A careless boolean-based merge could lose ownership distinctions; this is not “keep the largest emitter.” |
| **Verdict prose, string keys, danger-rank string parsing, then telemetry classification by prose substring** | A typed verdict containing status, source identities, quantities, and uncertainty; separate functions render it and encode telemetry. [Loop key/rank](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:34580), [prose classification](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:36730), [crack-back wrapper](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:36612). | No necessary decision information. Existing telemetry spellings need preservation or a schema migration. Current *incorrect* equivalences must not be preserved merely for byte identity. |
| **Two hold stores with parallel lifecycle operations** | One per-seam hold record, with the canonical ordered keys and a derived membership view when required. Both stores are created and erased together; the current check selects one representation and falls back to the other. [Members](/home/magi/Projects/wagicGPT/projects/mtg/include/AIPlayerGPT.h:1922), [dual lookup](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:38124), [dual construction](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:38242). | Potentially the compatibility fallback for a partially populated hold. I did not prove every route through that fallback unreachable; preserve it explicitly until checked. |
| **Ask cache → repeat-answer latch → cross-phase replay, with cross-invalidations and separate refusal maps** | One answer store keyed on a canonical **board state + question**, with consumption/replay provenance in its entry. Keep an explicitly chosen HOLD as a different object with its advertised lifetime. [Cache](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:48846), [repeat latch](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:48909), [cross-phase replay](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:49008). | It would lose some current round-trip savings, because some savings come from reusing answers after state changes. Those are not lossless optimizations under the owner’s key rule. |
| **Render a vocabulary, then maintain a large parallel vocabulary to strip it from narration and keys** | Have each option carry separate identity, narrative consequence, and prompt annotations. Render only at the boundary. [Narration stripping](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:33211), [action-key stripping](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:35913), [tail normalization](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:37235). | No intended information loss. This is a staged refactor, not a safe mass substitution: existing callers mix semantic text and decoration. |

**Important correction to the requested fingerprint concern:** the inspected primary keys do not include too much board state. They include **too little**. They replace the board with a selectively constructed legal-continuation summary. That is a correctness problem, not merely elaborate hashing.

# 3. FLAWS — ranked by effect on decisions

## F1 — High: legal actions are explicitly capped or suppressed

The priority seam contains independent policy filters after engine enumeration:

1. A face toggle is removed after two uses in the turn.
2. A non-X animation is removed once activated on an animated land.
3. Fetch rows receive one decline allowance; other rows receive two. An exhausted allowance removes the row.
4. An empty board stamp means a fetch action was consumed and **no board change reopens it**.

[Toggle and animation filters](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:46681), [decline caps and suppression](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:46727), [consume-on-choose fetch stamp](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:48045).

The animation comment asserts that repeating a fixed-size animation “changes nothing until the turn ends.” That is not generally true. A newly resolved setting effect has a new timestamp and can supersede an intervening setting effect; activation can also matter independently of the apparent final size. The relevant rules are [CR 613.4b](/home/magi/.gatelogs/engine-audit/cr.txt:5983), [613.7](/home/magi/.gatelogs/engine-audit/cr.txt:6021), and [613.7b](/home/magi/.gatelogs/engine-audit/cr.txt:6031). The normal activation permission is [CR 117.1b](/home/magi/.gatelogs/engine-audit/cr.txt:1859).

**Corpus effect:** a real priority prompt advertises numerous rows as:

> “if you pass here, this option is not offered again until the board changes”

That same document says instants and activated abilities held now remain usable later. The interface is teaching a layer-imposed restriction as part of the action surface. [Deck130 vs162, line 60, seq66](/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final/1789216937-ai_baka_deck130-0x562b68836ca0-vs-ai_baka_deck162.jsonl:60).

**Disposition:** remove strategic caps from enumeration. Preserve every legal option; only an explicit model-selected reservation can justify its own advertised suppression. This is not a request to alter engine legality.

## F2 — High: the ask and async keys are not BOARD STATE + QUESTION

The main ask key consists of turn, phase, optional continuation digest, and normalized question text. `boardStateKey` is computed nearby but omitted from it. The priority cache and async slot use the same scope substitution. [Ask key](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:48796), [priority key](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:47393), [async key](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:19312).

The digest contains target identities, mana-source reach, and selected extra terms—not the board. Its commentary explicitly excludes life totals. Moreover, only the cast seam stages this digest for generic `askModel`; ordinary target/cost/menu callers can use an empty one. [Digest collector](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:37138), [scope formatter and exclusions](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:36442), [staging](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:51948).

**Static counterexample:** keep the same legal targets, source count, turn, phase, and action labels; change a creature’s toughness or a player’s life. The desired action can change while this key stays equal. The repeat latch is more explicit: it intentionally serves an answer “with the board moved under it.” [Repeat path](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:48903).

**Effect:** a newly relevant question can be answered by an old choice without reaching the model. Legal-continuation equality proves neither tactical-state equality nor answer validity.

**Evidence boundary:** I did not identify a life-changing replay in the supplied main JSONL records. This finding is established by the source key construction, not an asserted corpus misplay.

## F3 — High: the ordinary AI wait gate prevents its watchdog from running

The intended abandonment mechanism is called only from `pollCompletion`. But the normal base-AI entry returns immediately when `decisionPending(dt)` is true. That override merely tests `asyncBusy`, extends the interrupt offer, and returns true.

- [Base Act early return](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerBaka.cpp:5878)
- [GPT pending gate](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:52317)
- [Only watchdog call](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:19410)
- [Watchdog](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:18855)

For a worker that never publishes completion, `status` stays 1; ordinary `Act` never reaches the decision seam; the seam never polls; the watchdog never observes its deadline.

The debug wedge branch explicitly creates that condition by returning without publication. [Wedge branch](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:18192).

**Effect:** indefinite opponent thinking on this path, notwithstanding the existence of deadline code. A normal HTTP timeout that returns is different and can complete normally.

**Disposition:** the watchdog must be serviced from a path that runs while waiting, with abandonment/fallback provenance retained. **Not executed or reproduced here**, per the no-run instruction.

## F4 — High, conditional lifetime defect: a discarded worker can destroy a locked mutex

Inside `WorkerMain`, a `lock_guard` locks `ctx->state->mtx`. In the generation-mismatch branch, the code executes `delete ctx` and returns **before the guard leaves scope**. [Publication block](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:18264).

If the player has been destroyed and this context holds the last `shared_ptr`, deleting it destroys `AsyncState`, including the mutex, while the mutex is locked. The guard then tries to unlock destroyed storage.

Required sequence:

1. A request is abandoned, changing its generation.
2. The owning player is destroyed.
3. The old worker returns and takes the mismatch branch.

The context’s shared ownership protects the state only until `delete ctx`; the mutex guard itself owns no reference. [Context ownership](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:18180), [player-owned references](/home/magi/Projects/wagicGPT/projects/mtg/include/AIPlayerGPT.h:1127).

**Disposition:** keep state ownership alive through guard destruction, or defer deleting the context until after unlocking. No crash was observed in the corpus; this is a conditional source-level lifetime finding.

## F5 — High: the new crack-back “best block” verdict adds blockable damage as unblockable

`crackBackFloorSources` adds animated-land power and direct ability damage into the same `sizeable` total. [Collector](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:28774), [animated power addition](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:28842).

`w81CrackBackVerdictLine` then computes:

- `addenda = addUp + compulsoryDraw`
- `floorNow = bestBlockFloor + addenda`

and describes the addenda as **“damage no block removes.”** [Calculation](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:36612).

That is false for animated creatures. The best-block search does not include those currently noncreature permanents, but **absence from the search does not make their future combat damage unblockable**. A blocked nontrampling creature assigns damage to blockers, not the defending player: [CR 510.1c](/home/magi/.gatelogs/engine-audit/cr.txt:4805).

**Static counterexample:** the current creature-only best block leaves 0 damage; a future 4-power nontrampling animated land is also blockable by another available creature; the seat is at 4 life. Adding that land’s 4 power directly to the “best block” floor falsely yields lethal.

There is a second aggregation problem: each candidate activation is priced independently against the available mana, then their contributions are summed. The collector does not establish that all those individually affordable actions are jointly affordable. [Per-card mana calculation](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:28817).

**Corpus motivation, not validation of the new code:** **60 prompts** contain `ADD THOSE UP`; one adds Lair of the Hydra’s 4 power to the combat figure. The new post-corpus formatter is precisely where those heterogeneous quantities become an asserted unavoidable total. [Deck130 vs152, line7, seq6](/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final/1789216918-ai_baka_deck130-0x559161469ed0-vs-ai_baka_deck152.jsonl:7).

**Disposition:** separate jointly realizable combat scenarios, blockable damage, direct loss, and unknown contributions. Do not obtain a damage floor by adding estimates excluded from the blocking model.

## F6 — Medium: an unambiguous label-less answer is rejected because prose follows it

`w79LabellessAnswerLine` rejects its unique answer candidate whenever any nonblank line follows it. `gptAnswerSegment` consequently reports no answer. [Rejection](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:9030), [selector](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:25250).

For example, with a valid row 2:

```text
PLAN: Cast the creature.
2 (Cast the creature)
Thank you.
```

The order is correct, and the action is unambiguous. The trailing courtesy text changes neither fact, but this path refuses the answer. That is a robust-parser violation, not grounds to change the reply protocol.

**Disposition:** distinguish answer ambiguity from extraneous text. Continue recording the latter as a protocol deviation; do not discard the former for it. This is a static case, not a corpus-observed failure.

## F7 — Medium: legal “no-op” answers are intercepted and re-asked

`askModel` recognizes a valid chosen row, then requests a replacement answer solely because `noopRowEarnsReask` considers the row a no-op. It has an additional budget-exempt re-ask when another correction already happened. [Predicate](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:39300), [first trigger](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:49136), [extra re-ask](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:49154).

This is not merely a tolerant parser resolving an unclear answer. It is a semantic veto of an unambiguous legal choice. The second answer may eventually execute, but the first answer is not committed and another round trip is imposed. A row’s local effect being zero does not prove choosing it has no desirable interactions.

**Disposition:** retain truthful no-effect annotations; do not use them to demand a different answer. I found no `[RE-ASK]` prompt in this corpus, so its current frequency is unmeasured.

## F8 — Medium: “enters tapped” is promoted to a turn-long prohibition

The ordinary land row says:

> “enters TAPPED — it makes no mana this turn … it taps for mana from your next turn on”

The implementation has no exception for untapping the land before then. [Formatter](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:49966).

The MDFC arrival formatter already uses the correct conditional phrasing: “unless something untaps it.” [Existing narrower wording](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:32959).

Tapping is a state, not a turn-duration prohibition: [CR 701.26a–b](/home/magi/.gatelogs/engine-audit/cr.txt:7047).

**Corpus effect:** the ordinary wording appears directly on an Arcane Sanctum land choice; the model chooses another land. That does not establish the wording caused a misplay, but it demonstrates the absolute statement in the composed decision surface. [Opening land decision, line4](/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final/1789216916-ai_baka_deck123-0x562a5c364ec0-vs-ai_baka_deck126.jsonl:4).

**Disposition:** state the actual dependency—unavailable while tapped—not a prohibition until next turn.

## F9 — Medium: land-play availability is mislabeled as land-drop usage

`landDropStatusLine` receives only `myTurn`, `playable`, and `haveLand`.

- `playable == true` produces **“NOT yet used this turn.”**
- `playable == false && haveLand == true` produces **“ALREADY USED this turn.”**

Neither implication is valid. An additional land permission can leave another legal land play after one was used; a restriction can prevent a land play before any was used. [Formatter](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:9694), [caller’s actual inputs](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:31612).

The rules explicitly distinguish the permitted number of land plays from the number already made: [CR 305.2](/home/magi/.gatelogs/engine-audit/cr.txt:3387), [305.2a–b](/home/magi/.gatelogs/engine-audit/cr.txt:3391).

**Disposition:** render “a land play is available/unavailable” from those inputs, or obtain the actual usage and allowance facts. Do not infer historical usage from present legality.

## F10 — Direct owner-rule violation: some legal menus are auto-answered

This is separate from whether the outcomes are strategically distinguishable:

- Generic one-option asks return 0 without asking.
- Byte-identical rendered rows return 0 without asking.
- A repeat-payment menu with all rows priced at zero automatically chooses “don’t add any counter.”
- A single legal cost target is returned directly.

[Generic shortcuts](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:48660), [repeat-payment shortcut](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:55074), [cost target](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:55164).

The current instruction explicitly says **never auto-answer a legal option**. The implementation’s “same outcome” rationale is not an exemption supplied by that ruling. Also, `identicalInterchangeableRows` proves equality of rendered strings, not equality of engine consequences. [Predicate](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:11608).

**Observed:** the corpus contains three `menu_single_outcome` records. One records an automatically answered 21-row Intrepid Adversary menu, with no prompt/reply for the choice. [Example](/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final/1789216918-ai_baka_deck152-0x559160132280-vs-ai_baka_deck130.jsonl:7).

This finding does **not** assert that choosing another row in that particular fixture would improve the outcome. It identifies the explicit instruction mismatch.

## F11 — Lower impact: a telemetry-only map grows for the entire game

`mAskScopeDigest` stores scope-plus-question strings to detect continuation changes. The key includes turn and phase. Its entries are inserted/read but never retired at a turn boundary, unlike the actual ask cache. [Member](/home/magi/Projects/wagicGPT/projects/mtg/include/AIPlayerGPT.h:2226), [insertion](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:48811), [neighboring cache retirement](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:48648).

Old-turn entries can never match again, yet retain question strings and target digests. This is **avoidable game-length memory growth**, not a proven permanent process leak.

**Disposition:** if this diagnostic comparison survives the key correction, retire its unreachable entries with their turn scope.

# 4. IMPROVEMENTS FOR THE OWNER’S REVIEW

These are proposals, not additional requirements.

## I1 — Render the final combat conclusion once, rather than a conclusion followed by successive qualifications

The sampled attacker document says, in sequence:

- total damage and resulting life;
- a blocks-only damage floor;
- that this is not actually a floor on damage landing;
- that attack triggers are not folded into either number;
- then a **FOLDED** calculation that says the numbers stand.

The model reaches the correct attack in this example, but the document makes it reconcile several scopes to recover one conclusion. [Complete attacker prompt, line21, seq20](/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final/1789216920-ai_baka_deck152-0x55a5a97d7280-vs-ai_baka_deck125.jsonl:21).

**Proposal:** calculate first, then render one conditional result with its assumptions and genuinely unknown terms. Keep per-attacker alternatives intact.

**Cost:** requires separating calculation from prose construction; byte-level fixtures and tools relying on existing phrases may need deliberate migration.

## I2 — Consolidate repeated timing explanations without hiding per-card exceptions

In the sampled discard prompt, the same sorcery-speed explanation appears twice and the long “no target—HELD instant” explanation five times. All rows and their distinct facts can remain while common reasons are defined once. [Discard prompt, line38, seq73](/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final/1789216949-ai_baka_deck125-0x556e5a3fa140-vs-ai_baka_deck123.jsonl:38).

**Proposal:** short per-card statuses plus one menu-local explanation for each status actually present. No legal-option removal, and no changes to the golden reply protocol.

**Cost:** some local repetition currently helps readers avoid looking elsewhere. The trade-off should be judged on composed documents, not byte reduction alone.

## I3 — Emit only the range grammar actually used by this menu

`kOptionRangeNote` explains several independent compression shapes, including ascending and descending X ranges, even when the current menu uses only ordinary instance ranges. [Constant](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:10967), [real priority prompt using the long explanation](/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final/1789216937-ai_baka_deck130-0x562b68836ca0-vs-ai_baka_deck162.jsonl:60).

**Proposal:** the row renderer returns which compression forms it used; append explanations only for those forms.

**Cost:** a small amount of renderer metadata and tests for combinations. Every number remains separately answerable.

## I4 — Consider a whole-order answer for blocker ordering

`orderBlockers` asks for successive positions, rebuilding a decision for the shrinking list. [Ordering seam](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:52132).

**Proposal:** if this engine’s ordering decision is to remain exposed, offer a whole permutation in one model decision, as combat declarations already bundle a whole assignment.

**Cost:** this may require an owner-approved action-format extension and permutation validation. **No protocol change is proposed for implementation without that approval.** I did not measure ordering round-trip savings in this corpus.

## I5 — Make the layer navigable by responsibility rather than wave lineage

There are coherent extraction boundaries already visible:

- prompt facts and presentation;
- answer selection/parsing;
- answer lifecycle, reservations, and transport;
- translog serialization;
- small adapters for each decision seam.

The existing `DecisionRequest` already snapshots menu context separately from mutable menu objects. [Contract](/home/magi/Projects/wagicGPT/projects/mtg/include/DecisionContract.h:28), [menu interception](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:52218).

**Proposal:** move coherent modules without changing behavior first; use typed fact/decision objects where prose is currently parsed back into state. Do not create one file per wave or one helper per sentence.

**Cost:** internal API design and incremental verification. This is a legibility investment, not an immediate decision-quality claim.

# 5. WHAT I COULD NOT DETERMINE

## Coverage

**Read fully at the named-function/block level:** the small CRUD candidates above; duplicate activation scanners; label-less answer detector; answer-segment selector; `consumePlan`; key formatter and continuation collectors; worker context/publication logic; watchdog and pending gate; land-status/arrival formatters; crack-back extra-source collector and W80/W81 verdict formatters; selected hold/cache lifecycle blocks; generic telemetry aggregation.

**Sampled rather than read end to end:**

- `chooseOrderedAction`: enumeration filters, key construction, replay, and commit bookkeeping.
- `askModel`: entry shortcuts, all three reuse mechanisms, key construction, parsing handoff, re-asks, and cache storage.
- `FindCardToPlay`: annotation call sites, digest staging, ask/commit exits—not its entire cast enumeration/payment flow.
- `chooseMenuAction`, `chooseTarget`, attacker/blocker declarations, reveal, cleanup, and mulligan seams.
- The large board renderer, narration machinery, full parser, retry-state machinery, and header.

**Skipped:** `AIPlayerGPTSelfTest.cpp`; exhaustive card-script/Oracle verification; transport implementations outside the immediate lifetime boundary; most individual render families; exhaustive engine-contract application internals.

## Unresolved limits

1. **No runtime confirmation.** The watchdog, lifetime, cache-aliasing, and parser findings are static paths, not reproduced crashes or test failures.
2. **No exhaustive 298-family disposition.** I rechecked the explicitly named Tier A candidates and selected dormant families. The census does not enumerate all 26 purported no-production-caller helpers; I did not invent a disposition for unnamed entries.
3. **No proof that unused telemetry is unused by humans or external scripts.** The in-repository gameend consumer is generic, which invalidates several easy “no consumer” conclusions.
4. **No complete system-message reconstruction from the corpus.** The corpus’s system records do not contain the whole sent system message; the census explicitly records that limitation. [Measurement limitation](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/audit-2026-09/gpt-layer-census.md:164).
5. **No causal decision-quality estimate.** A misleading sentence appearing beside a choice proves exposure, not that the sentence caused the choice.
6. **No approval to delete test-supported helpers or alter owner-ruled protocol/thinking behavior.** None is implied here.

**Recommended review order:** decide F1/F2/F10 against the standing choice/key rulings; fix the watchdog/lifetime paths; correct the crack-back aggregation; then take the small proven deletions and presentation simplifications. **Do not apply census Tier A wholesale.**