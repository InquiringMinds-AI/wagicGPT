# Wave-73 step-one adversarial review

Seat: Codex CLI 0.153.4, gpt-6-astra, 2026-09-07 ~22:40. Prompt `~/.gatelogs/w73-review-prompt.md`. Diff 90f8d1d68..d2086e41b (lanes BZ+CA+BY). Findings routed to fix lane CB.

---

# Verdict: not safe to ship as-is

The diff still suppresses legal decisions in reachable states. Its strongest green evidence tests helper predicates with assumed inputs, rather than establishing that the live callers supply truthful inputs.

**Verification:** read-only review of `90f8d1d68..7e078f725`, the saved diff, rulings, evidence documents, lane reports and weakest-evidence sections, and previous review. Prompt-design working memory applied. Constructor guard passed (**118 files**); reply-instruction guard passed (**1 source, 0 guides**); scoped `git diff --check` passed. No files written, builds, game runs, PARSETEST runs, or model calls. Counterexamples below are source-traced, not newly executed game reproductions. Direct Scryfall retrieval failed; card examples were checked against repository primitives, with official release notes additionally supporting finding 5.

## 1. HIGH — Chain collapse trusts an oracle that omits legal payments and activations

[AIPlayerGPT.cpp:28231](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:28231)

**Defect:** the generalized collapse treats a restricted display/response oracle as proof that the actual casting and priority menus cannot contain legal actions.

**Trigger:** the opponent has a counterable spell on the stack; your hand contains Counterspell, your only mana sources are two untapped Lotus Petals, and you have no other responses. The casting seam uses `GptManaPolicy`, which permits their sacrifice costs. `hasInstantResponse()` instead uses `FreeProducerPolicy`, which rejects sacrifice-cost producers. Its ability scan also excludes mana producers. Thus the cast menu can contain Counterspell while `loopAutoPassWindow()` returns true and closes it before `askModel()`.

There is **another independent blind spot**: [LegalActions.cpp:405](/home/magi/Projects/wagicGPT/projects/mtg/src/LegalActions.cpp:405) excludes phase-restricted activations whenever `sorcerySpeedOk` is false, including abilities legal during the current upkeep. A zero-counter Balduvian Hydra, three Mountains, empty hand, and an unresolved upkeep trigger leave its legal upkeep-only counter-adding activation invisible to this oracle.

PARSETEST at **79392** supplies `anyLegalAction=true`; it never tests whether either live board produces that value.

## 2. HIGH — A hold can survive past the holder’s next untap

[AIPlayerGPT.cpp:27885](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:27885)

**Defect:** expiry is checked only when a later menu reaches `holdHonoured()` during the holder’s turn, not when that turn actually begins.

**Trigger:** hold Ghost Town’s return-to-hand activation during opponent turn 14. During your turn 15, that activation is unavailable; suppose you have no castable spells or other non-mana activations. Casting returns before the hold check when there are no candidates, and mana-only priority menus return before it too. On opponent turn 16, Ghost Town’s unchanged activation returns, but `ownTurnNow=false`, so the hold still suppresses it.

The promised untap occurred, but no latch recorded it. This can persist across multiple turns. The own-turn predicate pins at **79258** miss the essential sequence: **opponent-turn hold → no qualifying own-turn call → next opponent-turn call**.

## 3. HIGH — Natural-language thresholds become binding creature stops

[AIPlayerGPT.cpp:8487](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:8487)

**Defect:** accepting filler words without checking the number’s meaning turns non-creature thresholds and opponent plans into the seat’s binding repeat stop.

**Trigger:**

```text
PLAN: Stop at 3 life; M=10 creatures; make two more creatures this window.
CHOICE: 2 (Create human with Thraben Doomsayer)
```

The scanner stores `stop=3`, `M=10`; ownership passes because the preceding token is `PLAN:`. Subsequent repeat-only windows compare the live creature count against **3**, incorrectly concluding that the model’s creature stop was reached.

Likewise, `PLAN: Their intended stop at 24; M=30; develop my board` passes ownership: `repeatPlanStopIsOwn()` checks only the immediately preceding word, **intended**, not **Their**.

The new positive pin at **78960** proves acceptance of `stop at N`, not that such a number authorizes suppression. Restricting the scan to the PLAN line does not resolve this semantic ambiguity.

## 4. MED — The new loop annotation declares chain-starting actions unrelated to the chain

[AIPlayerGPT.cpp:27193](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:27193)

**Defect:** any acting row causes the renderer to categorically declare that the rows are “NOT part of the chain,” without inspecting their costs or effects.

**Trigger:** the opponent controls a functioning Sanguine Bond–Exquisite Blood pair; your priority menu offers Phyrexian Reclamation with a creature available to return and sufficient mana/life. Paying its life cost directly enters the opponent’s chain. Nevertheless, `actingRowsPresent=true` selects the new sentence declaring the row unrelated.

The caller supplies only **whether an acting row exists**, not whether it participates in the loop. PARSETEST at **79375** endorses the unrelated-row wording using booleans alone. This does not implement the brief’s requirement to distinguish rows inside versus outside the chain.

## 5. MED — Ghostform’s gloss promises protection after its granted ability is removed

[AIPlayerGPT.cpp:2965](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:2965)

**Defect:** the counter-name formatter unconditionally equates a ghostform counter with a functioning death/exile trigger.

**Trigger:** Kaya grants ghostform to a creature; Sudden Spoiling subsequently removes that creature’s abilities for the turn. The counter remains, so the board still prints that it returns and creates a Spirit—“killing it is not removal”—although its granted trigger is gone.

The formatter receives only the counter name and cannot distinguish these states. Ghostform counters are reminders, not the ability itself; Wizards explicitly documents their lack of functional significance. [Kaldheim release notes](https://media.wizards.com/2021/downloads/KHM_Release_Notes/EN_MTGKHM_ReleaseNotes_20210113.pdf)

The new tests check that reassuring words appear, not that the ability producing those outcomes remains active.

## 6. MED — The missing-main-phase instrument counts ordinary stack processing

[AIPlayerBaka.cpp:4380](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerBaka.cpp:4380)

**Defect:** `main_phase_windows_skipped` increments whenever a main-phase tick takes the unresolved-stack branch, regardless of whether that phase already offered casting or will offer it afterward.

**Trigger:** cast a spell normally in main phase one, then process its unresolved spell or triggered abilities. The standard empty-stack branch is correctly unavailable, so this hook counts a supposedly skipped casting window. Repeated ticks can increment it repeatedly within that same phase.

There is no per-phase deduplication or “phase ended without casting opportunity” check. Consequently, the lane’s proposed comparison against turns missing a main-one record cannot validate N16: normal games generate false positives.

## 7. LOW — The replacement replay-total field still changes meaning between paths

[AIPlayerGPT.cpp:36913](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:36913)

**Defect:** `replays_this_game` receives the combined replay total on the cache path but only the repeat-latch subtotal on the repeat path.

**Trigger:** ten cache replays followed by the first repeat-latch replay produce totals **10 → 1**, although eleven answers have been re-served. Refusal records switch back to the combined total.

`replay_run` was repaired, but the new field reproduces the same mixed-semantics defect under another name.

## Remaining attack surfaces and shipping verdict

I found no defensible index-shift defect from printing row 0: it remains outside the action vectors. The natural sort retains action-pointer mappings; the inspected CA/BY brace seam is balanced. I did not establish an unseen-row collision in sibling coverage or the re-armed reserve latch. Repeat refusal reopens the model question rather than capping a legal action. **Do not ship as-is:** findings 1–3 violate the legal-option boundary, and the render/instrument findings make the next corpus less trustworthy. Green helper tests and guards do not establish safety for these payment-policy mismatches, missed lifecycle events, or misidentified plan numbers.