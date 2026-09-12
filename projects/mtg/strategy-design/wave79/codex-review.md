# Wave 79 — Astra adversarial review of the step-one diff (06fad1449..244d8b5f8)

Seat: Codex `gpt-6-astra` via `codex exec --sandbox read-only`, prompt ~/.gatelogs/w79-review-prompt.md (lanes CZ/DA/DB). Verdict and findings verbatim below; Lane DC (wave79/lane-brief.md) maps each to CONFIRM/REFUTE.

# Verdict: not safe to ship as-is

Read-only review; nothing written. Applied the prompt-design working-memory guidance. Findings below are **source-traced counterexamples, not executed games**.

## Findings

### 1. HIGH — T3 caches row labels instead of the legal decision

[AIPlayerGPT.cpp:44662](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:44662)

Removing the board half while stripping row annotations aliases windows with different legal continuations.

**Trigger:** Decline `Cast Doom Blade` while one legal creature target exists; another legal target enters during the same phase. The cast row still names Doom Blade, but its downstream target set has expanded. Likewise, an X spell’s affordable ceiling can grow while its cast-row identity remains unchanged: the pricing annotation is stripped. The cached decline is returned without asking.

The same scope replacement affects async acceptance at **18566**. At the priority seam, **43540** additionally interprets identical normalized rows after a *successful* reusable activation as “no progress” and forces a pass. Key-equality pins establish none of these decisions’ equivalence; the replay limit only bounds repeated mistakes.

### 2. HIGH — T3 suppresses a new threat after an earlier threat was answered

[AIPlayerGPT.cpp:32559](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:32559)

The latch compares against the original held danger rank indefinitely, while the own-loop marker no longer identifies the threatened component or spell.

**Trigger:** Take hold over a THREATENED loop because an already-pending answer handles that removal. After it resolves, another removal threatens a different component, with the same cast/activation rows still available. The intermediate safer verdict is clamped to THREATENED; the new THREATENED verdict is also retained because its rank is equal. No question is reopened. “No more dangerous than the original threat” does not establish that the original answer covers this new threat.

### 3. HIGH — T5 promotes an incomplete combat model to an exact survival verdict

[AIPlayerGPT.cpp:30208](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:30208)

A successful DP search unconditionally sets `exactAssignment = true`, although its inputs omit both creatures’ strike abilities.

**Trigger:** At four life, face a 4/4 double-strike trampler with one ordinary 0/3 blocker. The DP computes `4 − 3 = 1` damage and the header promises survival at three; actual combat deals one overflow in first strike and four more afterward, killing you.

The error also runs the other way: a 6/1 first-strike blocker kills a 6/6 ordinary trampler before damage, beating the DP’s five-damage “best case.” Thus the lane’s claim that fidelity gaps are exclusively pessimistic is false.

### 4. HIGH — T9 adds back cover that was never subtracted

[AIPlayerGPT.cpp:27551](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:27551)

The sacrifice adjustment subtracts lost blocker coverage from life after subtracting the **full creature-power total**, double-pricing that coverage.

**Trigger:** Six life; your untapped Mogg Fanatic and 0/6 blocker; their 5/5 and 2/1. Sacrifice Fanatic to kill the 2/1. The capacity calculation loses two coverage points, and the clause computes `6 − (7 − 2) − 2 = −1`, declaring death. Only five opposing power remains: even declining every block leaves one life. The original total never credited either blocker.

### 5. HIGH — T9’s folded attack sentence bypasses opposing blockers

[AIPlayerGPT.cpp:7484](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:7484)

The new fold describes surviving attackers’ total power as damage that “reaches them,” rather than damage before blocking.

**Trigger:** Declare a 1/1 and an ordinary 5/5 into Lightmine Field and an untapped opposing 5/6. The field kills the 1/1; the sentence says five reaches the opponent and subtracts five life. The 5/6 can block the surviving attacker, yielding zero player damage. Disabling the separate kill-claim flag does not remove this explicit false life projection.

### 6. MED — T8 reserves a creature’s casting cost as a counterspell budget

[AIPlayerGPT.cpp:46346](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:46346)

The new counter candidate accepts `fizzle` anywhere in any card’s script, without distinguishing a spell effect from an activated ability.

**Trigger:** Hold Path to Exile, Ertai, Wizard Adept, and a more expensive actual counter while choosing an X spell. Ertai’s repository script contains `{2}{U}{U}{T}:fizzle`, so the candidate can replace the actual counter using Ertai’s three-mana **casting** cost. The resulting reserve does not leave a usable counter: Ertai must enter, overcome summoning sickness, and pay a separate activation cost. The pins test substring classification, not this live candidate-selection path.

### 7. MED — T11 drops the clause defining a retained ability’s resource

[AIPlayerGPT.cpp:703](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:703)

Priority selection treats dependent clauses as independently meaningful.

**Trigger:** Chrome Mox’s repository text has a 91-byte imprint clause and a 53-byte mana clause. At the live 120- or 140-byte snippet budgets, the selector prioritizes `{T}:`, then cannot fit the imprint clause. It prints mana “of any of the exiled card’s colors” without explaining which card is exiled or the nonartifact/nonland payment restriction. `(...more)` does not identify the missing dependency. This is a real primitive, not an invented clause arrangement.

### 8. MED — T7 identifies spent, opponent-only ETB effects as your untap engine

[AIPlayerGPT.cpp:7187](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:7187)

The scan recognizes script substrings without checking effect ownership or whether the effect remains available.

**Trigger:** Their Village Bell-Ringer is already on the battlefield; you tap creatures for mana. Its `untap all(creature|mybattlefield)` matches, so your payment clause names it as an exception that can untap your creatures before declaration. Its ETB already happened, and even another ETB affects its controller’s creatures, not yours. The emitter’s supplied-name pin cannot catch this board-walk error.

### 9. MED — T12 mistakes positional ordinals for persistent instance identities

[AIPlayerGPT.cpp:32667](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:32667)

Preserving every ordinal above one reopens holds when an unchanged permanent merely changes battlefield rank.

**Trigger:** Hold over an activation from Staff of Nin #3; remove an earlier, tapped Staff that supplied no offered row. The same source becomes #2 and reopens the hold despite no new legal action. `instanceHandle` at **4978** explicitly recomputes rank from current battlefield order. The lane’s uninspected naming assumption is false.

### 10. MED — T4 still counts verdicts that never reach a prompt

[AIPlayerGPT.cpp:47456](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:47456)

The new render counter and face stamp are populated before the live hold gate discards the window.

**Trigger:** Rebuild a casting window with a proven loop and a standing cast hold. The counter increments and `mOwnLoopVerdictFace` is assigned; **47483–47487** then suppress the window. The counter claims a rendered prompt that never existed, and the face remains available for a later record to consume. This reproduces the exact helper-versus-caller failure the wave-78 lesson forbids.

### 11. LOW — T17 invents a seat-specific meaning for record turns

[corpus-stats.py:288](/home/magi/Projects/wagicGPT/projects/mtg/tools/corpus-stats.py:288)

The new explanation calls record `turn` “this SEAT’s turn count,” but `translogTurn` at **31703** simply returns `observerTurn + 1`.

**Trigger:** Any ordinary record from either seat. The tool’s claimed distinction cannot explain the reported late-game discrepancy; it supplies an incorrect diagnosis instead of establishing why the compared populations differ.

## Verification and merge assessment

The saved diff exactly matches the scoped source/include/tools/test diff. `git diff --check`, both Python tools’ selftests, Python AST parsing, and `bash -n` passed. Golden protocol text is unchanged; I found no added unlabelled-answer parser acceptance. The DA lowercase key expectation is consistent with CZ’s normalization, and the constructor merge remains syntactically coherent. However, DA’s key pin exercises supplied row strings—not live board collection or synthetic verdict construction. The narration “after” census remains an estimate, not a re-render. No build, engine suite, PARSETEST binary, or endpoint was run.

**Do not ship as-is.** The principal blocker is semantic decision aliasing: cached answers and taken holds now survive changes they have not actually answered. Separately, combat and sacrifice annotations assert demonstrably incorrect survival outcomes. Green helper pins and reconciled telemetry cannot establish safety while these live-path counterexamples remain.