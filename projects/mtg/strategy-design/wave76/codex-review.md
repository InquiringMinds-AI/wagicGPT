# Wave 76 — Astra adversarial review of the step-one diff (77b920612..f3eff598d)

Seat: Codex `gpt-6-astra` via `codex exec --sandbox read-only`, prompt ~/.gatelogs/w76-review-prompt.md (lanes CN/CO/CP; attack each lane's weakest evidence). Verdict and findings verbatim below; Lane CQ (wave76/lane-brief.md) maps each to CONFIRM/REFUTE.

# Verdict: not safe to ship as-is

Read-only review; nothing written. Applied the prompt-design working-memory skill. The saved diff matches the source/header diff for `77b920612..HEAD`. Constructor guard passed **118 files**; reply-instruction guard passed **1 source, 0 guides**; scoped `git diff --check` passed. No duplicate constructor initializers or replacement characters found; removed scalar initializers correspond to the new state struct. The golden protocol file and inline constant are unchanged.

Findings below are **source-traced counterexamples**, not executed games. No build, suite, PARSETEST, or endpoint probe was run. Current Oracle retrieval failed; card examples below are grounded in inspected repository scripts and engine behavior.

## 1. HIGH — Q1 can fail to commit a genuinely asked window

[AIPlayerGPT.cpp:29360](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:29360)

**Defect:** The same-window early return bypasses staging after `w76HoldWindowNotAsked` has erased the pending entry.

**Trigger:** Last asked menu contains actions `{A,B}` plus closing rows. An identical held window is built, measured, then suppressed, clearing `pending` without advancing `mWindowSeq`. Before another record, B becomes unavailable. Building `{A}` finds zero unseen rows and the same sequence, returns the cached note, and stages nothing. The changed menu reopens the hold and earns a record, but cannot commit. When B returns, the bracket falsely calls it seen at the last asked window, which actually contained only A.

## 2. HIGH — Q8 refuses the rescue and executes the heuristic

[AIPlayerGPT.cpp:17696](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:17696)

**Defect:** Refusing a forced close neither parks the unanswered decision nor schedules its re-ask.

**Trigger:** Land has an armed close parked; casting receives a successful HTTP response containing native reasoning but no answer. The refusal increments `mForceCloseArmsRefused`, then falls through: there is no transport failure, timeout, declaration payload, or decode-garbage payload to trigger another retry. `pollCompletionRetry` returns success with empty content; casting parses failure and calls the heuristic.

This is not the claimed “window unwinds and is re-asked.” The swap-only pin never follows the refused branch through its caller.

## 3. MED — Q13 retains the phase inside the supposedly phase-free key

[AIPlayerGPT.cpp:39834](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:39834)

**Defect:** Removing the numeric phase does not remove the phase embedded in `decision`.

**Trigger:** Identical casting rows at opponent Upkeep and Draw still receive different keys because the live caller constructs `Casting decision (Upkeep, opponent's turn)…` versus `Casting decision (Draw, opponent's turn)…` at line 42670.

The reported ten-window casting reproduction therefore cannot match across phases. The pin supplies the artificial phase-free string `"Casting decision"`, bypassing the production defect.

## 4. MED — Q13 asserts board equality without comparing boards

[AIPlayerGPT.cpp:40144](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:40144)

**Defect:** Matching turn, question, and rows is treated as proof that nothing on the board changed.

**Trigger:** A phase-independent optional-choice question offers the same action and `Do nothing` at two phases; the seat previously declined, but an unrelated opposing permanent entered between asks. The key still matches and the note asserts “nothing on the board has changed.” Neither the saved entry nor the comparison contains a board snapshot.

Additionally, `mCrossPhaseRePuts++` runs before the pending-poll return: one qualifying asynchronous ask is counted again on every polling tick.

## 5. HIGH — Q5 subtracts creatures that survive the removal

[AIPlayerGPT.cpp:2555](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:2555)

**Defect:** The new removal total counts an eligible creature’s entire crack-back contribution without checking regeneration or return effects.

**Trigger:** During the seat’s turn, at two life with no blockers, the opponent’s only attacker is a vanilla 3/3 with an active regeneration shield. A Day of Judgment row records `crackRemoved=3`; the new clause subtracts the entire attack and promises survival. The engine instead consumes the shield, taps the creature, and keeps it alive; it untaps and attacks next turn.

The pure arithmetic pins cannot expose this incorrect victim classification.

## 6. HIGH — Q6 invents converter damage when life gain is prohibited

[AIPlayerGPT.cpp:46437](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:46437)

**Defect:** The converter scan prices hypothetical gain without consulting the engine’s life-gain prohibitions.

**Trigger:** The seat controls Erebos, God of the Dead; the opponent controls Sanguine Bond and forces the seat to sacrifice a six-toughness creature through the priced toughness-life-gain effect. The row newly claims six life lost through Bond. However, `Player::gainOrLoseLife` returns before generating a life event because Erebos prohibits the opponent’s gain. Bond receives no triggering gain.

## 7. MED — Q2’s roster remains unbounded with only two shapes

[AIPlayerGPT.cpp:8750](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:8750)

**Defect:** The twelve-group cap bounds shapes, but each group can still emit arbitrarily many individual bodies.

**Trigger:** A battlefield has 100 same-named Vampires, alternating between two rendered P/T shapes—for example, every second body has a +1/+1 counter. `instanceHandle` assigns consecutive battlefield ranks, so each shape contains only alternating ranks. No group has a consecutive run long enough to collapse; all 100 entries print in full despite there being only two shapes.

This survives copy-row folding and defeats the promised bounded roster without needing thirteen distinct shapes.

## 8. MED — Q9 exempts ordinary targeted bounce as self-recursion

[AIPlayerGPT.cpp:7342](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:7342)

**Defect:** Scanning ordinary `magicText` for `moveto(hand)` confuses moving a target with returning the card itself.

**Trigger:** Unsummon is stranded in the graveyard, no cast permission or recovery effect exists, and the PLAN says to cast Unsummon. Its inspected primitive contains `auto=moveTo(hand)`, so the new exemption suppresses the measurement. That script moves Unsummon’s creature target when cast; it provides no graveyard recursion.

This remains measurement-only, but removes genuine members of the measured population.

## 9. MED — Q15 relabels arbitrary forced-close prose as prefill echo

[AIPlayerGPT.cpp:7267](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:7267)

**Defect:** The classifier checks that a forced close occurred, not whether the off-protocol bytes match its prefill.

**Trigger:** A forced-close response contains a valid PLAN/action pair followed by newly generated commentary absent from the supplied prefill. Any positive off-protocol byte count becomes `forced_close_prefill_echo`, concealing an actual `prose_outside_two_lines` deviation.

The new pins supply only the forced-close flag and byte count, so they cannot distinguish these cases.

## 10. MED — Q1’s new key pin institutionalizes the forbidden drift

[AIPlayerGPT.cpp:85939](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:85939)

**Defect:** The new PARSETEST explicitly requires unequal option-set keys for rows differing only in annotation-derived board numbers.

**Trigger:** The supplied Dictate rows contain a nested `{4}` inside `{reserve: …}` and differ only in remaining-source counts. The balanced hold stripper removes the complete annotation; the shared option-set stripper stops at the inner brace and retains the changing suffix. The test requires this mismatch rather than catching it.

This is an inherited production defect newly pinned as correct, directly contrary to the governing key-stability ruling.

**Shipping verdict:** Do not ship as-is. The mechanical merge checks and protocol preservation passed, but the diff still loses an earned model rescue, misidentifies asked history, issues false survival and damage forecasts, and leaves the large-roster failure reachable. Several green pins encode helper assumptions rather than exercise their live callers; one explicitly preserves prohibited key drift. These findings require fixes and caller-level verification before the reported green gates can support shipping.