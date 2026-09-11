# Wave 77 — Astra adversarial review of the step-one diff (49c19f413..90643013b)

Seat: Codex `gpt-6-astra` via `codex exec --sandbox read-only`, prompt ~/.gatelogs/w77-review-prompt.md (lanes CR/CS/CT). Verdict and findings verbatim below; Lane CU (wave77/lane-brief.md) maps each to CONFIRM/REFUTE.

# Verdict: not safe to ship as-is

Read-only review; nothing written. Applied the prompt-design working-memory skill. The saved diff **matches** `49c19f413..HEAD` for the reviewed source/header/tools/fixture paths. Scoped `git diff --check` and `bash -n` passed; the golden protocol file is unchanged. I exercised the actual harvest Python with mocked filenames. Other counterexamples below are **source-traced, not executed games**; no build, suite, PARSETEST, or endpoint probe was run.

## 1. HIGH — The new “key-stability” pin explicitly preserves forbidden ask-key drift

[AIPlayerGPT.cpp:89644](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:89644)

**Defect:** The R6 pin asserts that the numerical annotation survives the ask-key stripper, rather than asserting equal keys across changing numbers.

**Trigger:** Use the pin’s own `rA`/`rB`: identical Hammer action, but cover figures change from total/life `8/9` to `9/4`. `stripDeclineReaskTags` removes only the decline-reask clause; the cover numbers remain in `keyTailStr`, which enters the ask key and asynchronous slot. Holding the board prefix constant does not produce equal keys. The pin rationalizes this because an older sibling does it too—precisely the forbidden precedent. CT’s roster pin likewise checks only hold/option-set keys while claiming ask-key coverage.

## 2. HIGH — ANNOUNCE_X treats single-target damage as a sweeper

[AIPlayerGPT.cpp:45521](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:45521)

**Defect:** The new per-X cover walk sums every killable target without checking `cbxSv.sweep`.

**Trigger:** A single-target `damage:X` spell, X=3, two opposing 3/3 crack-back attackers, seat at four life. The survey correctly classifies this as targeted, and the adjacent kill gloss says “one target only”; the new cover clause nevertheless subtracts both creatures, claiming six damage removed. Only one target dies. The parent-row path explicitly requires `sv.sweep`; this supposedly equivalent helper wiring drops that condition.

## 3. HIGH — Both new removal-cover paths resurrect wave-76’s regeneration bug

[AIPlayerGPT.cpp:43323](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:43323), [AIPlayerGPT.cpp:45531](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:45531)

**Defect:** Targeted and per-X removal forecasts subtract bodies without calling `w76CrackVictimTrulyRemoved`.

**Trigger:** Seat at two life, no blockers, opposing 3/3 with an active regeneration shield; offer Hammer of Bogardan or an X-damage sweeper at three. Both new paths credit the entire three-power removal and can promise survival. The creature regenerates, untaps next turn, and attacks. The parent X path already invokes the wave-76 guard for regeneration, ghostform, persist, and undying; the new submenu omits it.

## 4. HIGH — “Your loop is resolving” requires no loop event

[AIPlayerGPT.cpp:30503](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:30503)

**Defect:** `w77OwnLoopResolving` equates assembled loop pieces plus *any* unresolved stack object with an executing, self-sustaining win.

**Trigger:** Control Sanguine Bond and Exquisite Blood, with no gain/loss trigger pending; the opponent casts removal targeting Bond. A legal counterspell row receives “your loop is resolving on its own—this row is not needed to win it.” Holding lets the removal break a loop that never started. Neither stack-object identity nor the threatened component is examined. The five-link fixture supplies `true` directly, so it cannot catch this.

## 5. HIGH — A timed-out asked window still loses its history commit

[AIPlayerGPT.cpp:30454](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:30454)

**Defect:** An intervening telemetry record advances `mWindowSeq`, causing the asked window’s commit guard to reject its own staged menu.

**Trigger:** First cast window stages sequence N and reaches the model. Its response times out with the deadline spent. `pollCompletionRetry` calls `flushWallMissRecord("wall_miss_no_retry")`, consuming N, then returns to `askModel`, whose ask record consumes N+1 without restaging. `pendingSeq != windowSeq` rejects the commit. The next cast window says “this is the first window I have asked you” despite the predecessor request. The wall-miss record also consumes the hold-reference stamp intended for that ask.

## 6. MED — R2c’s pins miss target-preview drift without any target change

[AIPlayerGPT.cpp:88754](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:88754)

**Defect:** The refutation tests reorder simplified names, not the live target roster’s board-derived preview fields.

**Trigger:** Keep the same cast, cost, and legal target instance, but change its preview from `Goblin (creature 1/1)` to `Goblin (creature 2/2)`. The live roster appends `targetPreviewFacts`; `holdActionKeyRow` retains creature P/T parentheses because the shared stripper only specially removes land parentheses. The unchanged action becomes “new” and reopens the hold. Matching the engine’s counts with a Python port cannot establish compliance with the owner’s action-identity ruling.

## 7. HIGH — Optional blocking gain is counted even when gaining life is prohibited

[AIPlayerGPT.cpp:28315](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:28315)

**Defect:** The new optional-gain accumulator treats a recognized trigger as a reachable gain without checking life-gain prohibitions.

**Trigger:** Seat at two life controls Perimeter Captain; opponent controls Erebos. Captain can block one attacker while another three damage gets through. The trigger scanner meters two optional life, so the new folded figure becomes +1 and withdraws “no block saves you”; Erebos prevents that gain, leaving −1. This is not merely the model declining an available “may.” The new edict target gloss has a related false surface: its prohibition gate suppresses converter damage but still prints that the prohibited life is gained.

## 8. MED — R6 immediately introduces another unbounded victim roster

[AIPlayerGPT.cpp:26526](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:26526)

**Defect:** The new targeted-cover annotation enumerates every contributing victim individually, bypassing CT’s roster folding.

**Trigger:** Offer lethal targeted damage against 100 same-named one-toughness attackers. The kills roster folds, but the cover clause prints 100 separate `name takes N off that total, leaving M` entries. Its size grows linearly without a cap or grouping, defeating the simultaneous row-size work and invalidating whole-prompt bounds that account only for creature gloss additions.

## 9. MED — CT’s 37,121-byte bound omits bytes emitted by its snippet builder

[AIPlayerGPT.cpp:89161](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:89161)

**Defect:** The bound assumes `boardEffectSnippet` emits at most twice the tier, excluding separators, omission markers, and truncation overhead.

**Trigger:** Three 55-byte clauses at the 55-byte tier produce a head, an omitted-clause marker, and a tail: **163 snippet bytes**, not 110. With the charged copy/foreign-voice wrappers, that is **319 bytes**, exceeding the asserted 266-byte per-name ceiling. The pin tests one selected text, not this counterexample. Thus 37,121 is not a proved upper bound; this alone does not prove a corpus prompt exceeds 40 KB.

## 10. MED — Harvest membership still mixes pools and drops legitimate seats

[selfplay-harness.sh:140](/home/magi/Projects/wagicGPT/projects/mtg/tools/selfplay-harness.sh:140)

**Defect:** Deck membership plus a time tolerance is not game identity.

**Trigger:** With manifest `125v123` at epoch 1000, the actual function accepts foreign logs at 1005 for both `125v123` and `125v162`, but rejects the legitimate seat initialized at 1016. I executed these three cases against the extracted function. HEAD uses **15 seconds**, not six; same-pair concurrent pools remain indistinguishable.

**Shipping verdict:** Do not ship as-is. Protocol-file preservation and mechanical checks pass, but the diff violates the explicit key-stability ruling, misclassifies single-target removal, repeats a previously confirmed survival bug, invents reachable gains, and still loses asked-history commits. Several green pins validate helper assumptions—or explicitly preserve the wrong behavior—instead of exercising the live decision paths.