# Wave-74 adversarial review — seat: Codex (GPT-6 Astra, `codex exec --sandbox read-only`), diff c0551632e..92843101e, prompt ~/.gatelogs/w74-review-prompt.md

# Verdict: not safe to ship as-is

**Verification:** read-only review of `c0551632e..92843101e`, the saved diff, rulings, evidence, lane reports and previous review. Prompt-design working memory applied. Constructor guard passed **118 files**; reply-instruction guard passed **1 source / 37 guides**; shell syntax and scoped `git diff --check` passed. Constructor comparison found **no removed or duplicate initializers**; golden `reply-protocol.txt` is byte-identical. No dropped PARSETEST block was established at the merge seams.

I executed the harness’s embedded Python against **in-memory logs**, without writing files. C++ counterexamples below are source-traced, not newly executed games. No build, suite or PARSETEST run. Scryfall/Gatherer retrieval attempts failed; card examples were checked against repository scripts, not independently verified current Oracle text.

## 1. HIGH — O2 still lets one arm overwrite another arm’s retry

[AIPlayerGPT.cpp:16658](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:16658)

**Defect:** `retryArmMatches` protects consumption and abandonment, but the subsequent retry-arming branches still overwrite the single shared retry record regardless of its owner.

**Trigger:** land’s primary reply arms a forced close; casting then launches its primary request. Once casting finishes, land can launch its pending forced close, after which casting consumes its completed primary reply in the same `computeActions()` and arms another forced close. That overwrites land’s `mRetryBase`, prompt and prefill while land’s second leg is outstanding. Land subsequently polls its ordinary prompt, not its forced-close prompt; their slot keys distinguish forced-close requests, so the completed second leg can be discarded.

The boolean ownership flag is not per-arm retry storage. The delivery-ratio experiment does not exclude this interleaving, and the two-second pass hold does not protect it.

## 2. MED — O2 omitted the decode-garbage retry’s ownership assignment

[AIPlayerGPT.cpp:16860](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:16860)

**Defect:** the fourth retry-arming branch sets `mRetryActivePrompt` without setting the newly required `mRetryArmLand`.

**Trigger:** a fresh seat’s land-drop reply satisfies `isDecodeGarbage()`. The constructor leaves ownership false, so the next land poll fails `retryArmMatches` and does not launch the answer-locked retry. A casting poll can instead abandon it as a changed casting question. The new ownership check breaks a retry path that the three updated assignments do not cover.

## 3. HIGH — O26 can kill a slow healthy run and miss a genuinely stalled tail

[selfplay-harness.sh:194](/home/magi/Projects/wagicGPT/projects/mtg/tools/selfplay-harness.sh:194)

**Defect:** “last three per unfinished log” is neither the newest K decisions nor evidence that every active seat has timed out.

**Executed counterexamples, default K=6:**

- Twenty-one live logs: six contain one initial timeout each; fifteen contain only `gamestart` while their first requests remain pending. Result: **`STALL 6`**. The sweep terminates the corpus without establishing that those fifteen requests failed.
- One unfinished seat has twenty consecutive timeouts; other seats finished. Result: **`OK 3`**. This seat can never satisfy K.
- One old success in an unfinished log plus six newer failures elsewhere: **`OK 6`**. An arbitrarily old record vetoes the newest failure streak.

The seven selftests exercise none of these lifecycle/cardinality cases.

## 4. MED — O26’s banner can certify heuristic games as fully model-answered

[selfplay-harness.sh:876](/home/magi/Projects/wagicGPT/projects/mtg/tools/selfplay-harness.sh:876)

**Defect:** the results census counts only exact `fallback == "timeout"` but then claims every decision was model-answered when that count is zero.

**Trigger:** a log contains `wall_miss_no_retry` fallbacks without an exact `timeout` record. The tripwire explicitly recognizes these as endpoint silence, yet the banner prints “none — every decision … was answered by the model.” The extracted counting helper returned **0** for this input. `empty_reply` and `http_error` have the same omission.

The selftest validates an exact-timeout fixture, not agreement between the tripwire’s classes and the published result.

## 5. HIGH — O5 treats fixed-one-life triggers as damage-proportional converters

[AIPlayerGPT.cpp:47097](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:47097)

**Defect:** the caller uses `lifeToDamageConverterScript()` as proof that every lifelink point causes another point of opponent life loss, although that predicate also accepts `:life:-1 opponent`.

**Trigger:** the seat controls Cliffhaven Vampire and attacks only with a 4-power lifelink creature against an opponent at 8. Cliffhaven’s repository script loses **one** life per gain event. The new computation supplies `selfConvLifelink=4`, rendering opponent life **0 / lethal**; the unblocked result is **3**.

Conversely, two Sanguine Bonds are reduced to one boolean converter, undercounting their combined effect. The O5 pins supply the converted amount directly; they never test the battlefield-to-amount calculation.

## 6. MED — O8 subtracts generic card cost from a colour-limited X budget

[AIPlayerGPT.cpp:6653](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:6653)

**Defect:** `maxX - keepNeed` is presented as remaining purchasing power even when `maxX` measures only available mana of X’s colour.

**Trigger:** Drain Life and Staff of Nin in hand; four untapped Swamps and seven untapped colourless one-mana sources. `maxAnnounceableX()` returns **3** for Drain Life’s black-only X. Paying its fixed cost and X=3 leaves six colourless mana—enough for Staff. The annotation instead computes `3−6` and says **no X, not even zero, preserves Staff**.

This is not merely a possible off-by-one from multi-mana sources: the printed categorical conclusion is reversed on ordinary one-mana sources.

## 7. MED — O9 invents a repeatable clock from an unusable printed ability

[AIPlayerGPT.cpp:32349](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:32349)

**Defect:** the clock scans scripts without checking whether their tap abilities can function or recur.

**Trigger:** the sole damage source has `{T}:damage:1 target(anytarget)`, is tapped, carries `DOESNOTUNTAP`, and has no available untap mechanism. Against an opponent at 2, the decline row nevertheless promises zero life in **two more turns**. There is no functioning two-turn clock.

The admitted missing lethal-incoming gate is separate: even the claimed rate itself can be false. Pure script/rate pins cannot establish repeatability.

## 8. MED — O15 declares haste-granted tokens unable to attack

[AIPlayerGPT.cpp:33405](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:33405)

**Defect:** token attack eligibility is inferred from “haste” appearing in the planeswalker’s own text, ignoring battlefield-granted haste.

**Trigger:** activate Sorin, Lord of Innistrad’s Vampire-making loyalty ability while controlling Fervor. Fervor’s repository lord grants the token haste, but Sorin’s text contains none, so the new clause says it **cannot attack until next turn**. The negative pin manually passes `hasteText=true`; it never exercises this caller.

## 9. MED — O7 corrupts a thousands-formatted state count during carry

[GptPlanCaveat.h:951](/home/magi/Projects/wagicGPT/projects/mtg/include/GptPlanCaveat.h:951)

**Defect:** the prefix scanner treats a thousands separator as a clause delimiter and preserves only the number’s leading digits.

**Trigger:** `M=1,000; cast Staff; then attack.` The accepted prefix is `M=1,`; after consuming the first step, `planStepsAfter()` produces **`M=1,cast Staff; then attack.`** The carry now asserts one creature rather than one thousand. The new thousands pin checks only step count, not preservation of the carried number.

**Shipping verdict:** do not ship as-is. The async repair still loses armed retries, the harness can both terminate viable work and mislabel fallback-contaminated results, and several new annotations assert demonstrably false consequences. I found no O13 behavior change or defensible O19 answer-mapping defect; however, O19’s hand-built permutation pins do not exercise the actual menu/parser/application path. Green helper tests and intact merge seams do not resolve the findings above.