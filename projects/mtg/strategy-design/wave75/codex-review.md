# Wave 75 — Astra adversarial review of the step-one diff (207a7f410..301a30a37)

Seat: Codex `gpt-6-astra` via `codex exec --sandbox read-only`, prompt ~/.gatelogs/w75-review-prompt.md (lanes CI/CJ/CK/CL; attack each lane's weakest evidence + the three hand-resolved merge seams). Verdict and findings verbatim below; Lane CM (wave75/lane-brief.md) maps each to CONFIRM/REFUTE.

# Verdict: not safe to ship as-is

Read-only review; nothing written. Applied the prompt-design working-memory skill. Constructor guard passed **118 files**; reply-instruction guard passed **1 source, 0 guides**; shell syntax and scoped `git diff --check` passed. Constructor comparison found **no removed or duplicate initializers**. The golden protocol file is unchanged.

The harness counterexamples below execute its extracted Python against **in-memory logs**. C++ counterexamples are source-traced, not newly executed games. No build, suite, PARSETEST, or endpoint probe was run.

## 1. HIGH — P4 excludes Sorin when he is a legal creature target

[planeswalkers.txt:3059](/home/magi/Projects/wagicGPT/projects/mtg/bin/Res/sets/primitives/planeswalkers.txt:3059)

**Defect:** `other *[creature;planeswalker]` excludes the source from both alternatives, whereas Sorin’s text restricts only the planeswalker alternative.

**Trigger:** Sorin has seven loyalty, has not activated this turn, and Sarkhan the Masterless has made him a creature. Sorin can target himself as a creature with −6. The new global `other` flag makes `TargetChooser::canTarget` reject him before evaluating that distinction. The fixture tests ordinary Sorin and Serra Angel, not animated Sorin.

The [lane’s proposed “general latch hole”](/home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave75/lane-CK.md:133) is also misdiagnosed: a planeswalker that leaves and returns is a new permanent and may activate again. A player-/Oracle-identity latch would remove legal activations. See [CR 400.7 and 606.3](https://media.wizards.com/2026/downloads/MagicCompRules%2020260619.pdf). Scryfall/Gatherer retrieval failed; the targeting distinction above follows the repository’s printed card text.

## 2. HIGH — P1b calls a genuinely new activation “seen”

[AIPlayerGPT.cpp:28561](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:28561)

**Defect:** the new census adopts a key that erases actual activation costs, not just forecasts.

**Trigger:** a scripted permanent’s held menu contains `Draw with Tome [cost: {3}]`; it subsequently gains a second `name(Draw)` activation costing `{T}`, while the first remains legal. The renderer places those differing costs inside `[cost: …]`. `holdActionKeyRow` strips **every square-bracket group**, including its enclosed mana symbols, so both rows become `draw with tome`.

The new row contributes **zero unseen rows**; the existing set-based hold also covers it. CI newly extends this inherited collision to the model-facing census and promises that different costs reopen the window. Standalone mana braces survive; mana inside a square-bracket cost does not. New tests distinguish card names, not this action-identity collision.

## 3. HIGH — P9’s “clean tail” gate accepts an explicit non-choice

[AIPlayerGPT.cpp:20725](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:20725)

**Defect:** `choiceLineIsClean` returns true when the payload has no numeric head, and the rejection checker likewise declines to judge it.

**Trigger:**

```text
PLAN: Do not take CHOICE: not 2 (Cast Rorix Bladewing)
```

This has one label, whitespace anchoring, and a PLAN-leading physical line. Payload `not 2 (…)` passes both checks because `codedHeadEnd` returns `npos`. The downstream parser can bind the exact parenthetical name or rescue the embedded integer, executing the explicitly negated cast.

The new tests put rejection words **after** a numeric answer; none covers a negated payload prefix.

## 4. MED — P21 force-closes an answer P9 already accepts

[AIPlayerGPT.cpp:17107](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:17107)

**Defect:** the new forced-close gate uses a different definition of “coded answer” from the updated answer parser.

**Trigger:** native reasoning accompanies a response ending exactly at the token limit:

```text
PLAN: Play the land. CHOICE: 1 (Play Sunpetal Grove)
```

P9 accepts this complete answer, but `codedAnswerCount` still calls the exclusively **line-leading** scanner and returns zero. P21 therefore discards the usable first answer and requests another decode, which can choose differently or fail.

The P21 pins supply an integer count directly; they never count P9’s actual inline response.

## 5. MED — P2’s unrecorded counter is not per retry arm

[AIPlayerGPT.cpp:17117](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:17117)

**Defect:** `mForceCloseArmed` remains seat-global although forced-close retry storage is parked separately for land and casting.

**Trigger:** land arms a close; `selectRetryArm` parks it; casting then arms another close. The second arm increments `mForceCloseUnrecorded`, although land’s close remains safely parked. Both closes can subsequently complete and be recorded.

Result: **two recovered closes plus one “unrecorded” for two arms**. Recording one close also clears the shared flag while the other remains outstanding. The accounting identity fails without losing either request; a serial stub cannot expose this interleaving.

## 6. MED — P2’s fixed-point pin duplicates an incorrect whitespace model

[AIPlayerGPT.cpp:15451](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:15451)

**Defect:** six ASCII whitespace characters are treated as equivalent to Python/Jinja `strip()`.

**Trigger:** the reasoning trace ends with U+00A0, for example `Choose row 1.\u00a0`. The helper preserves that character inside the prefill; the template’s Unicode-aware trim removes it. The complete final-message content is then absent from the rendered template—the same continuation-rejection condition this fix addresses.

An in-memory comparison returned **false** for prefill containment after the template transform. `QwenTpl::trim` repeats the helper’s ASCII-only implementation, so its green fixed-point assertion cannot catch this.

## 7. HIGH — P5 claims an infect self-ping feeds a life loop

[AIPlayerGPT.cpp:28425](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:28425)

**Defect:** the rewrite treats a rendered damage subtraction as proof that life actually decreases.

**Trigger:** an established Prodigal Pyromancer enchanted with Glistening Oil targets its controller at 20 life and zero poison; the opponent controls Sanguine Bond and Exquisite Blood. The activation renderer supplies `takes 1 damage - you would be at 19`, which P5 upgrades to **zero life / feeds their chain**.

But [Damage.cpp:204](/home/magi/Projects/wagicGPT/projects/mtg/src/Damage.cpp:204) routes infect damage to poison counters without life loss. The player receives one poison counter; neither loop intake is fed. The underlying arithmetic was already wrong, but this diff escalates it into a categorical false death verdict.

## 8. HIGH — P7 still invents a blocker when the existing legend is a token

[AIPlayerGPT.cpp:41175](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:41175)

**Defect:** the duplicate walk excludes tokens although the legend rule does not.

**Trigger:** in second main, at three life, control an untapped legendary Rorix token and cast another Rorix against two opposing vanilla 5/5 creatures. The token can be retained from an earlier Cackling Counterpart legend choice.

The cover calculation counts the existing blocker plus one “new” blocker, covering ten damage and predicting survival. The legend rule leaves only one Rorix, regardless of which copy survives; one attacker still deals lethal damage. The helper pins bypass the faulty battlefield walk by supplying `legendTwinControlled` themselves.

## 9. HIGH — P24 can still terminate a healthy corpus

[selfplay-harness.sh:257](/home/magi/Projects/wagicGPT/projects/mtg/tools/selfplay-harness.sh:257)

**Defect:** bounding contributions does not ensure every live seat participates before the pool reaches K.

**Executed, K=6:**

- Four live seats: three newer logs each contain two timeouts; the fourth contains a successful decision → **`STALL 6`**.
- Twenty-one live seats: six newer logs contain timeouts; fifteen contain successful decisions → **`STALL 6`**.
- One remaining live seat with twenty timeouts, other seats finished → **`STALL 6`**, correctly.

The first two results contradict the termination banner’s claim about **every live seat**. `allHaveEvidence` checks presence of records, not whether omitted seats are failing.

**Shipping verdict:** do not ship as-is. The merge seams inspected are intact, the O11 renderer has no remaining production call, and changing the P5 held-set pin to `holdActionKeyRow` correctly matches the live census. I found no behavior gate consuming P12/P17/P20’s new measurements. Those positives do not resolve the legal-target loss, newly accepted non-answer, inconsistent retry/accounting paths, false lethal forecasts, or demonstrated corpus termination failures.