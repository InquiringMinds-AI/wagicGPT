# Wave-71 step-one adversarial review

Seat: Codex CLI 0.153.4, gpt-6-astra (`codex exec --sandbox read-only`), 2026-09-07 ~08:30. Prompt `~/.gatelogs/w71-review-prompt.md`. Diff 77d82bbe6..e94565edc (lanes BP+BQ+BO+BR). Findings routed to fix lane BS.

---

# Verdict: not safe to ship as-is

The diff introduces a shared lifegain regression, removes legal land choices beyond the declined window, and contains a cache-refusal path that immediately re-serves the refused answer.

**Verification:** read-only source/diff review, including lane weaknesses and prior findings; prompt-design working memory applied. Constructor guard: **OK, 118 files**. Reply-instruction guard: **OK, 1 source and 37 guides**. Harness `bash -n`: **passed**. No files written, build/game/PARSETEST run, or model endpoint contacted. Triggers below are source-traced counterexamples, not newly executed game reproductions. Scryfall/Gatherer requests returned 403; the two cited card texts were cross-checked through mtg.wtf.

## 1. HIGH — The stack stamp disables legitimate death-trigger lifegain

[AllAbilities.cpp:6762](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:6762), stamp at [ActionStack.cpp:449](/home/magi/Projects/wagicGPT/projects/mtg/src/ActionStack.cpp:449).

**Defect:** “target no longer on the battlefield” suppresses lifegain whose triggering condition specifically requires that creature to have died.

**Trigger:** control Proper Burial; your Grizzly Bears dies. The stamp walks `GenericTriggeredAbility → ALord → AADynamic`; the cloned dynamic payload inherits the stamp and targets the graveyard object. `isInPlay()` is false, so resolution returns before granting the required two life. Grim Feast and Death Watch contain the same affected primitive family. [Proper Burial’s card text](https://mtg.wtf/card/dis/16/Proper-Burial) expressly grants life on death.

Condemn passing does not validate this distinction: these are legitimate **triggered abilities**, not spell-resolution payloads.

## 2. HIGH — Tribute’s second-creature case remains broken

[AllAbilities.cpp:6768](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:6768).

**Defect:** the fix suppresses the entire second edict payload instead of choosing a remaining creature when that edict resolves.

**Trigger:** the opponent controls Bears and another creature; both queued Tribute-granted abilities selected Bears. The first sacrifices Bears. The second now returns zero, leaving the other creature alive and paying no life, although the opponent remains a valid target and must sacrifice a remaining creature. [Tribute’s card text](https://mtg.wtf/card/fdn/614/Tribute-to-Hunger) does not target the previously selected creature.

This is an acknowledged **residual defect**, not a newly introduced targeting bug. The one-creature fixture cannot establish the claimed general engine fix.

## 3. HIGH — A face-menu decline removes a legal land for the entire turn

[AIPlayerGPT.cpp:36057](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:36057), latch at [DecisionContract.cpp:634](/home/magi/Projects/wagicGPT/projects/mtg/src/DecisionContract.cpp:634).

**Defect:** the latch has neither a land-proposal discriminator nor a board/window invalidation condition.

**Trigger:** decline Hengegate Pathway in main phase one, then combat or another spell changes the board so playing its blue face in main phase two enables the desired spell. The same card remains in hand, the land drop remains legal, but the GPT list erases it and Baka skips it until the turn changes.

Additionally, any declined hand-card menu can arm this land latch; the code never proves that the declined proposal was a land drop. A local decline is being expanded into a turn-long prohibition, contrary to the governing ruling.

## 4. HIGH — Cache refusal falls straight into another cache

[AIPlayerGPT.cpp:34979](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:34979), second return at [35004](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:35004).

**Defect:** erasing `mAskCache[askKey]` does not invalidate `mRepeatAskKey`, so the supposedly refused window need not reach the model.

**Trigger:** a valid answer has populated both caches; the same state, question, rows and plan repeat 64 times. The refusal erases the first cache, then `repeatAskAnswerStands()` succeeds and returns the identical answer. Subsequent calls miss the first cache and re-serve indefinitely through the second, without another refusal check.

The new replay records also keep translog mtime advancing, defeating **both** harness silence-based watchdog arms. PARSETEST tests the counter predicate, not this fall-through.

## 5. HIGH — Fixed answer caps cannot represent large legal declarations

[AIPlayerGPT.cpp:31288](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:31288), execution at [45350](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:45350).

**Defect:** the 256-token blockers ceiling is independent of legal assignment cardinality, while the truncation re-ask has been deleted.

**Trigger:** a token board with 512 legal one-to-one blocks requires `BLOCKS: B1:A1, …, B512:A512`, even with a minimal PLAN. That declaration cannot fit the configured answer allowance; thinking-off makes the total ceiling exactly 256. A cut after complete pairs can still produce `pairs > 0`, so the surviving prefix executes and omitted blockers remain unassigned.

Large PUT lists have the analogous problem. Pins establishing that historical percentiles round to 256 establish arithmetic—not sufficient space for the current legal action.

## 6. MED — Deleting the distinct-index re-ask changes an irreversible live path

[AIPlayerGPT.cpp:46867](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:46867).

**Defect:** a duplicate-short discard now automatically selects an additional card instead of asking the model to complete its selection.

**Trigger:** discard three cards from an eleven-card hand; reply `PLAN: keep the finisher` followed by `PUT: 1, 2, 2`. Deduplication selects two cards. The fill loop then discards the highest-mana-value unselected card—potentially that finisher.

The deleted mechanism guarded exactly this shape. Zero occurrences in the latest corpus do not make its removal behavior-neutral.

## 7. MED — Puzzle Box’s warning names the wrong player’s hand

[AIPlayerGPT.cpp:4614](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:4614).

**Defect:** the scan identifies the affected hand from the source controller and the source’s entire script, not the pending ability’s payload.

**Trigger:** you control Teferi’s Puzzle Box during the opponent’s draw step. Its pending opponent-draw trigger replaces **their** hand. Your source script also contains the separate `myhand` trigger, so your prompt falsely announces that every count from **your** hand is about to become void.

Conversely, the opponent’s renderer skips your Box because you control its source. The wording-only tests cannot detect either actor inversion.

## 8. HIGH — Damage modes are declared incapable of changing life

[AIPlayerGPT.cpp:29894](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:29894).

**Defect:** absence of literal `life:` and `draw:` is treated as proof that a mode changes no life total.

**Trigger:** Jeskai Charm’s `name(4 damage) damage:4 target(opponent,planeswalker)` mode against an opponent at four life. It contains neither searched token, so the row receives “it changes no life total and draws no cards” despite offering lethal damage.

This is not an uncertain forecast; the new negative contradicts an existing primitive. Damage, dynamic lifegain, and indirect payloads need not use either searched token.

## 9. MED — A new PARSETEST pin endorses rejecting a useful Devour Flesh cast

[AIPlayerGPT.cpp:75294](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:75294), predicate at [32044](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:32044).

**Defect:** the test explicitly requires a whole cast row to be classified as no-op because one possible targeting branch does nothing.

**Trigger:** the opponent controls Rorix; you control no creatures. The pinned row says Rorix is sacrificed, then says targeting yourself does nothing. `rowSaysNoOp()` returns true because it finds the latter clause, causing the useful opponent-targeted cast to earn the no-op re-ask.

The new kill-tag exception does not rescue this edict wording. This pin preserves the wrong semantic expectation.

## Shipping verdict

**Do not ship as-is.** The strongest blockers are the broad death-trigger regression, the turn-long removal of a legal land, and the ineffective cache refusal. The cap reduction and deleted recovery paths additionally lack coverage for legal high-cardinality answers. I found no defensible dropped-block or initializer-union defect in the reviewed merge seams; nor does a pending model call alone prove the suggested pass-floor failure, because `decisionPending(0)` returns before that floor. Those non-findings do not offset the concrete failures above: green guards and lane suites do not establish safe behavior for these states.