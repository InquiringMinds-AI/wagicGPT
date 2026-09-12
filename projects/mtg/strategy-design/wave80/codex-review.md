# Wave 80 — Astra adversarial review of the step-one diff (70345356d..a66df551f)

Seat: Codex `gpt-6-astra` via `codex exec --sandbox read-only`, prompt ~/.gatelogs/w80-review-prompt.md (lanes DE/DF/DG). Verdict and findings verbatim below; Lane DH (wave80/lane-brief.md) maps each to CONFIRM/REFUTE. F1 (nine dropped ctor initializers) was CONFIRMED and repaired by the orchestrator before DH launched (commit "wave80 merge repair"); DH adds the guard.

# Verdict: not safe to ship as-is

Read-only review; nothing written. These are source-traced counterexamples, not executed-game reproductions.

## Findings

### 1. HIGH — The merge dropped nine constructor initializers

[AIPlayerGPT.cpp:20485](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:20485)

The merged constructor leaves DE’s seven new counter/sequence integers and DF’s two single-outcome counters indeterminate.

**Trigger:** Construct any GPT seat, then render a verdict, replay an answer, or engine-answer an Adversary menu. Comparisons and increments read uninitialized integers; telemetry and once-per-window guards are undefined. I compared the lane commits: DE and DF initialize their respective members; HEAD does not. Consequently, the U9 sum identity is not established even though each replay branch increments its intended components.

`check-ctor-init.py` passes because it detects **commented-out** initializers, not missing ones.

### 2. HIGH — Labelled answers preceding PLAN are still accepted

[AIPlayerGPT.cpp:24558](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:24558)

The labelled-answer branch returns before applying any PLAN-order constraint.

**Trigger:** `CHOICE: 2 (Cast Wall of Omens)\nPLAN: block and draw.` with that offered row. `consumePlan` returns the choice; order is merely recorded as telemetry. PARSETEST at **88753** explicitly expects this forbidden acceptance.

This is inherited, not introduced by wave 80, but violates the owner’s explicit review criterion. It is **not** a complaint about parser tolerance.

### 3. HIGH — A hold survives a second lethal stack after the first was answered

[AIPlayerGPT.cpp:36368](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:36368)

The comparison retains the original lethal marker through safer intermediate states, so renewed lethality cannot exceed its remembered rank.

**Trigger:** At three life, hold with Lightning Bolt pending below your Healing Salve and an unchanged Counterspell cast row available. Salve resolves, then Bolt; the seat survives at three. Another Bolt arrives before untap. The remembered stack face remains `KILLS`; the new face is identical, and normalized cast rows remain unchanged.

The hold suppresses the question. **Rendering the lethal paragraph does not save the seat:** the suppressed window never reaches the model.

### 4. HIGH — The rendered crack-back verdict converts unblocked power into certain death

[AIPlayerGPT.cpp:35597](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:35597)

The new paragraph says passing leaves a board that kills you, but its underlying calculation does not subtract blockers.

**Trigger:** Four life, their ordinary 5/5, your untapped 0/6, no other damage. `crackBackVerdictNow()` sums five and selects `LETHAL`; the paragraph says the board kills you next turn. Blocking prevents all player damage. Passing priority now does not surrender the later blockers decision.

A threat-threshold marker has acquired a stronger, false survival claim.

### 5. HIGH — U1 retires the cast latch at the first completing menu, not the completed cast

[AIPlayerGPT.cpp:21686](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:21686)

Any successful completing-menu answer clears state 2, although another completing menu may follow.

**Trigger:** Commit Starstorm with `PLAN: Cast Starstorm X=2, then attack…`; answer its “Cast Card Normally” menu while retaining that PLAN. The record clears the latch. The subsequent X announcement—built by `MTGPutInPlayRule`—therefore echoes from the already-advanced step and drops the cast clause again.

The new tests exercise the echo helpers, not this two-menu lifecycle.

### 6. MED — “Live starter” still means script substring, not an available action

[AIPlayerGPT.cpp:36014](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:36014)

The battlefield scan declares a starter without checking activation legality, timing, or whether its trigger can happen.

**Trigger:** Sanguine Bond and Exquisite Blood are active; the only other potential starter is a tapped Prodigal Sorcerer, with no untap effect or other available life-changing action. Its `damage:` script immediately supplies the named “proven win.”

Likewise, Soul Warden qualifies with no creature able to enter. Naming an existing permanent does not prove an entry into the loop.

### 7. HIGH — The ETB detector mistakes activated self-bounce for leaving on entry

[AIPlayerGPT.cpp:28564](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:28564)

`w80EtbSelfLeavesLine` accepts untriggered lines without excluding activated-cost prefixes.

**Trigger:** Cast Fleeting Image under a lethal crack-back. Its repository script is `{1}{U}:moveto(hand)`. The detector returns true, subtracts the creature, and prints that its **enters-the-battlefield text** immediately removes it.

The bounce requires a separate activation; the flying creature remains available to block. This is not merely conservative arithmetic—the explanatory mechanism is false.

### 8. HIGH — The new blocking lesson is false for trample

[AIPlayerGPT.cpp:28803](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:28803)

The unconditional mechanism sentence teaches that a blocker absorbs the attacker’s full power regardless of toughness.

**Trigger:** At four life, cast an ordinary 1/1 against a 5/5 trampler. The cover machinery checks block legality but not trample spill, and the new sentence explicitly says the 1/1 stops all five. Four damage can reach the player, killing the seat.

The updated literal expectations embed this false rule rather than testing its applicability.

### 9. MED — U14 charges optional draws as compulsory lethal damage

[AIPlayerGPT.cpp:58014](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:58014)

The blockers verdict imports `drawStepExtrasScan` without distinguishing optional draw effects.

**Trigger:** Four life, two incoming combat damage, your Sylvan Library, their Underworld Dreams. The scan counts Library’s two additional draws despite its `may`, producing three compulsory damage and declaring loss at −1. Declining those draws leaves one life after the ordinary draw.

The optionality is explicit in both the primitive and [Wizards’ Sylvan Library wording](https://media.wizards.com/2026/downloads/SOS_Release_Notes_FwhcBWdFIE/EN_MTGSOS_ReleaseNotes_20260410.pdf). Sharing an erroneous forecast does not validate the new verdict.

### 10. MED — “Send-time” verdict counters still execute before cache suppression

[AIPlayerGPT.cpp:49854](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:49854)

The casting caller counts and stamps verdicts before entering `askModel`, whose cache and cross-phase replay branches can return without sending.

**Trigger:** Rebuild a verdict-bearing casting menu with a cached decline, no applicable hold/reservation suppression, and a fresh window ordinal. The caller increments; `askModel` returns the cached answer at **46855**. No model prompt was sent.

The helper’s `sent=true` is an assertion by an upstream caller, not evidence from the actual send boundary.

### 11. MED — Hold-event fields overwrite events and migrate to unrelated records

[AIPlayerGPT.cpp:36377](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:36377)

Each clamp increments immediately but overwrites one shared string, which is consumed only by a later `writeTransLog`.

**Trigger:** A standing hold clamps both crack-back and stack faces, then suppresses repeated polling windows. Multiple increments produce at most one retained field; the next unrelated target or discard record inherits the last event.

The stderr messages help investigation, but the promised per-record, send-associated event accounting still does not reconcile.

### 12. MED — Cross-phase replay records use the wrong source-sequence namespace

[AIPlayerGPT.cpp:46978](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:46978)

The new replay path passes `cp->second.windowSeq` into `logAskReplay`’s `replayed_from`, where other paths supply a record sequence.

**Trigger:** Any cross-phase replay after record and window ordinals diverge—for example, after sidecar replay records advance `mTransSeq` without advancing `mWindowSeq`. The source field points to the wrong record. The cross-phase entry stores no source record sequence from which to repair the join.

## Verification and limits

The saved diff exactly matches the scoped source/include/tools/test diff. `git diff --check`, both Python selftests, the constructor guard, and the source reply-instruction guard passed. Both protocol representations are unchanged. No missing closing braces were apparent at the PARSETEST tail union. Direct Scryfall/Gatherer retrieval was blocked; the card-script counterexamples above identify the inspected repository mechanisms. No build, engine suite, PARSETEST binary, or model endpoint was run.

**Do not ship as-is.** The constructor merge damage alone invalidates the telemetry gate. Independently, the live paths retain forbidden answer ordering, suppress renewed lethal threats, retire cast intent prematurely, and print false survival mechanisms. Green helper pins cannot establish safety while those caller-level failures remain.