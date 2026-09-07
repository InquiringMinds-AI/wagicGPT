# Wave-72 step-one adversarial review

Seat: Codex CLI 0.153.4, gpt-6-astra, 2026-09-07 ~16:20. Prompt `~/.gatelogs/w72-review-prompt.md`. Diff 0e4e6b9b8..32ab3855c (lanes BT+BU+BV+BW). Findings routed to fix lane BX.

---

# Verdict: not safe to ship as-is

The diff introduces legal-window suppression and removes legal card-name choices. Several new “truth” annotations are also false in reachable states.

**Verification:** read-only review of the saved diff, live source, rulings, evidence documents, all four lane reports and their weakest-evidence sections, and the previous review. Prompt-design working memory applied. Constructor guard passed (**118 files**); reply-instruction guard passed (**1 source, 0 guides**); harness `bash -n` and scoped `git diff --check` passed. No files written, builds, game runs, PARSETEST runs, or model calls. Counterexamples below are source-traced, not newly executed reproductions. Scryfall/Gatherer card requests returned 403; card examples were cross-checked against repository primitives.

## 1. HIGH — A casting HOLD suppresses unseen legal activations

[AIPlayerGPT.cpp:27361](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:27361)

**Defect:** sibling suppression checks board equality but never checks whether the held menu contained the sibling’s legal options.

**Trigger:** at upkeep, offer Starstorm on the casting menu while Spark Spray/Starstorm cycling is legal on the priority menu. The model selects HOLD over the **casting rows**. `takeHold()` records the board; `holdHonoured("priority", rows)` returns before examining the cycling rows.

The hold explicitly refers to “the rows above”; its casting-specific text prices casting windows, not unseen cycling or activations. An unchanged board does not make disjoint questions equivalent. The positive PARSETEST at **77340** expressly endorses this unsafe collapse without supplying either option set.

## 2. HIGH — An old stop acquires the date of an unrelated new plan

[AIPlayerGPT.cpp:34820](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:34820)

**Defect:** the collapse dates `mStatedStop` using `mPlanSetTurn`, although those fields are updated independently.

**Trigger:** turn N’s plan states `stop=29; M=29`. On turn N+1, a casting reply says `PLAN: Save mana for removal`, with no stop. Parsing updates `mPlanSetTurn` to N+1 but retains `mStatedStop=29` at **20040–20054**. A priority menu containing only the token-producing repeat family now auto-passes at 29 creatures.

The old stop was not restated this turn. The pure test’s “earlier turn” negative cannot catch this because it assumes the supplied date belongs to the stop.

Additionally, `repeatPlanStopAndCurrent()` has no ownership discriminator: `PLAN: Their stop=29; their M=29; develop my board` populates the same scalar.

## 3. MED — Open casting decisions still claim that casting is already answered

[AIPlayerGPT.cpp:24750](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:24750)

**Defect:** a turn/phase stamp is treated as proof that the current window offers no cast, even when another casting decision is open.

**Trigger:** cast one affordable spell in main phase one, resolve it, then receive another casting menu in that same phase with mana and another legal spell remaining. The second menu’s hand tag says “no cast row now” although its action list contains that cast.

The advertised re-ask fix also remains incomplete: **38348** stamps the phase before validation; if validation rejects the candidate, the `attempt > 0` iteration re-renders with the stamp already set.

## 4. HIGH — Token copies lose legitimate card-name options

[AllAbilities.cpp:4508](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:4508)

**Defect:** unconditional `isToken` filtering confuses the object being scanned with whether its name is a legal card name.

**Trigger:** the opponent controls a token copy of a real creature card, with no nontoken copy of that name in the opponent zones scanned by `chooseOppName`. That name previously entered the naming menu; now it disappears.

A copied card name remains legal even when its only scanned representative is a token. The rule explicitly permits token names that are also card names; the filter lacks that exception. [Comprehensive Rules, 201.4](https://media.wizards.com/2024/downloads/MagicCompRules%2004102024.pdf)

This affects human and heuristic menus as well as GPT. Creature-type selection is a separate branch; that is not the defect here.

## 5. MED — Public cards are falsely declared permanently uncastable

[AIPlayerGPT.cpp:30916](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:30916)

**Defect:** the naming header and remaining-copy tag equate “currently public” with “can never be cast again.”

**Trigger:** the opponent’s Hammer of Bogardan is in their graveyard, with mana to return it during upkeep and cast it again. The naming menu labels its public copy spent and excludes it from the purported count of copies that can still be cast. Its repository primitive explicitly implements that return.

Flashback, cast-from-exile permission and ordinary bounce produce the same contradiction. This is also directly inconsistent with this engine’s graveyard/exile cast enumeration. The tag assertion at **77414** pins the false exclusivity claim. [Hammer’s return ability](https://mtg.wtf/card/8ed/193/Hammer-of-Bogardan)

## 6. MED — Name-menu fallback is permuted twice

[AIPlayerGPT.cpp:40019](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:40019)

**Defect:** the display-to-engine mapping is applied to a heuristic answer that is already in engine index space.

**Trigger:** engine names are `[A, B]`, displayed order becomes `[B, A]`, and the endpoint fails. `AIPlayerBaka::selectMenuOption()` selects engine index 0, A; `nameOrder[0]` then changes the committed choice to engine index 1, B.

The ordinary model-answer permutation is correct; the fallback arm is not. Neither narration permutation nor short-name parsing tests exercise this path.

## 7. MED — A non-mana land erases a real reserve warning

[AIPlayerGPT.cpp:37544](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:37544)

**Defect:** every legal land not detected as entering tapped contributes one mana source, without checking whether it produces mana.

**Trigger:** during upkeep, two untapped Islands can pay for Howling Mine; casting a one-mana instant leaves one. The unused land drop is Dark Depths. The new code counts Dark Depths as another source and suppresses the STRANDS clause, although its repository primitive has no mana ability.

This fails even before colored-payment or conditional tapped-entry complications arise.

## 8. MED — “Every point is unblocked” ignores an available creature-land blocker

[AIPlayerGPT.cpp:23123](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:23123)

**Defect:** having no creatures now is converted into a categorical claim that nothing on the battlefield can stop next turn’s combat damage.

**Trigger:** control an untapped Mutavault and another available mana source, with no current creatures; the opponent has a blockable ground attacker. Mutavault can animate before blockers using the activation implemented in its primitive.

The new sentence nevertheless says every point is unblocked. The positive assertion at **77596** pins this unconditional inference. Instant-speed token production creates the same problem.

## 9. MED — LOOP COMPLETE ignores a live prohibition on lifegain

[AIPlayerGPT.cpp:21883](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:21883)

**Defect:** script-shaped halves on the battlefield are sufficient to announce inevitable lethal chaining, without checking whether the chain can operate.

**Trigger:** the opponent controls Sanguine Bond and Exquisite Blood; you control Erebos, whose repository ability is `nolifegainopponent`. Your life payment cannot cause the opponent to gain life and restart Bond, yet the new header declares every such payment fatal.

The helper receives only names and a side flag, so string pins cannot distinguish this board from a functioning loop.

## Shipping verdict

**Do not ship as-is.** Findings 1, 2 and 4 violate the owner’s legal-option boundary; the remaining findings undermine the render’s factual contract or commit the wrong fallback choice. BU’s three opponent-turn fixtures being green on base are regression pins, not an M5 repair—the report itself acknowledges that the lost-payment cause remains unfixed. I found no defensible dropped-block or surviving executable `Hold priority:` comparison defect in the inspected merge seams. Green guards and helper tests do not establish safety for the state transitions and cross-menu behavior above.