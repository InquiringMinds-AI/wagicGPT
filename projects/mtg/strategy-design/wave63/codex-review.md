# Adversarial review findings

1. **HIGH — [DecisionContract.cpp:346](/home/magi/Projects/wagicGPT/projects/mtg/src/DecisionContract.cpp:346), [AIPlayerGPT.cpp:30951](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:30951):** The ordinary `ANNOUNCE_X` route silently caps the legal menu at X=50, while the renderer falsely states every higher value is unaffordable.  
   **Trigger:** A player with 60 available mana casts an X spell whose base cost is zero; X=51–60 are affordable but absent, and the prompt says they are unaffordable.

2. **HIGH — [AIPlayerGPT.cpp:21665](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:21665), [AIPlayerGPT.cpp:30561](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:30561):** The new hold-key normalization deliberately erases the Main-1/combat distinction and auto-answers the legally distinct Main-2 casting window without asking the model.  
   **Trigger:** Take HOLD in Main 1 with an instant-speed removal spell available, pass through an empty combat without changing rows, then reach Main 2; `holdHonoured("cast", menu)` returns true and suppresses the opportunity to cast before ending the turn.

3. **MED — [AIPlayerGPT.cpp:39207](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:39207), [AIPlayerGPT.cpp:60618](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:60618):** `{spare:}` is decided using an arbitrary `current lands + 2` ceiling, so PARSETEST now pins a false “spare land” expectation rather than card reachability.  
   **Trigger:** At eleven lands, hold a six-drop and a legal fourteen-mana spell; the fourteen-drop is excluded by the cap, every land is labelled spare, and the test protects that classification even though additional land drops advance a castable card.

4. **MED — [GptPlanCaveat.h:600](/home/magi/Projects/wagicGPT/projects/mtg/include/GptPlanCaveat.h:600):** `planScratchpadCut` treats any later sentence beginning with “Wait” as discarded deliberation, even when “wait” is the plan’s operative action.  
   **Trigger:** `PLAN: Hold removal. Wait until their end step, then cast it.` is carried only as `Hold removal.`, deleting the timing instruction before the next decision.

5. **MED — [GptPlanCaveat.h:869](/home/magi/Projects/wagicGPT/projects/mtg/include/GptPlanCaveat.h:869), [AIPlayerGPT.cpp:15458](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:15458):** The absent-permanent guard treats a same-named permanent on either battlefield as satisfying ownership-specific claims such as “I control X” or “on my battlefield.”  
   **Trigger:** The pilot’s Sanguine Bond has been exiled while the opponent controls another Sanguine Bond; `I control Sanguine Bond` remains carried as a true plan premise.

6. **MED — [AIPlayerGPT.cpp:19471](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:19471):** Material improvement compares the sum of ordinal ranks rather than preserving blockers lexicographically, so an assignment that loses a blocker can tie and survive over an assignment that loses none.  
   **Trigger:** Two legal assignments have ranks `[4,2]` (kill-and-live plus trade) and `[3,3]` (neither blocker dies); both sum to six, the strict `>` rejects the second, and the header nevertheless says the line was chosen for blocker material.

7. **MED — [AIPlayerGPT.cpp:19194](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:19194), [AIPlayerGPT.cpp:24672](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:24672):** The crack-back relief clause subtracts only the killed creature’s current power and falsely presents the result as the post-removal total, without recomputing static effects.  
   **Trigger:** Goblin King and two Goblin tokens contribute 6 power; killing the 2/2 King renders `6 -> 4`, although losing its +1/+1 effect leaves two 1/1 tokens for an actual total of 2.

8. **MED — [AIPlayerGPT.cpp:4047](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:4047):** The new pending-draw scan accepts only `StackAbility` objects and ignores unresolved `DrawAction` objects that carry explicit `player` and `nbcards` fields.  
   **Trigger:** An unresolved `DrawAction(seat, 3)` exists on `ActionStack`; `stackPendingDrawsFor` returns zero, so an X-draw row underprices the eventual cleanup hand by three cards.

9. **MED — [AIPlayerGPT.cpp:11870](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:11870), [AIPlayerGPT.cpp:12463](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:12463):** Receiving HTTP headers is incorrectly equated with completing the round trip, causing a body-phase deadline expiry to classify as `transport_error` instead of `timeout`.  
   **Trigger:** The server sends HTTP 200 headers, emits no body, and stalls until curl returns 28 at the deadline; `gptDeadlineMissed` returns false solely because status is 200, after which curl wins classification as `transport_error`.

## Verdict

**Not safe to ship as-is.** The diff preserves one hard legal-X cap, newly widens an auto-answer across strategically distinct phases, and introduces multiple false decision facts involving land reach, stale-plan ownership, blocker material, and crack-back arithmetic. The transport classifier still misorders a real body-phase timeout, and the stack-draw implementation does not cover every stack object representing a pending draw.