# Adversarial findings

1. **HIGH — [DecisionContract.cpp:353](/home/magi/Projects/wagicGPT/projects/mtg/src/DecisionContract.cpp:353):** The ordinary cast-route `ANNOUNCE_X` branch sets `req.kind` but never sets `req.canDecline`, so the advertised D5 fix still removes the engine’s legal Cancel option.  
   **Trigger:** Cast Starstorm or Ivy Elemental with only X=0 affordable through the non-`menu->announcing && menu->announceCost` branch; `req.canDecline` remains its initialized `false`, no Decline row is rendered, and X=0 is auto-committed. The lane report’s claim that both branches assign `canDecline` is contradicted by the merged source.

2. **HIGH — [AIPlayerGPT.cpp:34349](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:34349):** The new combat-restatement scanner ignores the prose preceding `ATTACK:`/`BLOCKS:`, so a negated or rejected candidate is executed as the final answer.  
   **Trigger:** `BLOCKS: B1:A1\nI should not use BLOCKS: B2:A2.\nPLAN: ...`; payload `B2:A2` passes `combatLineIsClean`, replaces the coded answer, and executes precisely the assignment the model said not to use. PARSETEST covers affirmative “So BLOCKS” prose but no negated context.

3. **MED — [AIPlayerGPT.cpp:19485](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:19485):** The cross-lane D3/D8 merge emits each draw forecast twice, retaining the old direction-agnostic loop warning immediately before the corrected scoped version.  
   **Trigger:** The opponent controls Sanguine Bond plus Exquisite Blood while your draw punisher forecasts life they lose: the first `DRAW FORECAST (theirs)` carries `LOOP CAUTION`, then the second carries `LOOP SCOPE`, presenting contradictory claims. The D3 PARSETEST constructs only one helper call and cannot detect the doubled integration output.

4. **MED — [AIPlayerGPT.cpp:31387](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:31387):** Pay-repeat affordability treats the number of mana sources as fungible mana and divides only by converted cost, making its “pays for N” price claim false for colored costs.  
   **Trigger:** Intrepid Adversary’s “Add 3 counters” row with six untapped Mountains and no floating mana prints that six spendable mana pays all three `{1}{W}` payments, although the player cannot make white and pays for none. PARSETEST supplies an already-assumed scalar availability and therefore pins no color-fit invariant.

5. **MED — [AIPlayerGPT.cpp:18135](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:18135), [AIPlayerGPT.cpp:18052](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:18052):** The next-turn crack-back gate counts the animated land itself as available mana and compares only total CMC, so it can falsely claim that a manland both pays for its activation and attacks.  
   **Trigger:** Hive of the Eye Tyrant plus three Plains is counted as four sources and its `{3}{B}` activation passes the scalar cost test; using Hive for black leaves it tapped, while leaving it untapped provides no black, so no attacking 3/3 exists despite the rendered crack-back floor.

6. **MED — [AIPlayerGPT.cpp:1396](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:1396), [AIPlayerGPT.cpp:32939](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:32939):** `THIS WINS THE GAME` is inferred solely from the two loop cards being present, without checking whether the opponent’s life can change or the converter can affect them.  
   **Trigger:** Sanguine Bond plus Exquisite Blood gains one life while the opponent is under Teferi’s Protection/`cantchangelife`; the loss is prevented and the loop does not close, but the new edict or damage-row tail asserts an unlimited chain and a win.

7. **MED — [GptConfig.cpp:903](/home/magi/Projects/wagicGPT/projects/mtg/src/GptConfig.cpp:903):** The transport layer discards a received HTTP status whenever libcurl later fails, preventing the supposedly status-first classifier from observing that the server answered.  
   **Trigger:** A server sends HTTP 503 headers and then stalls its body until curl returns 28; `CURLINFO_RESPONSE_CODE` is 503 but `codeOut` is forced to 0, so `gptDeadlineMissed` classifies it as `timeout` and grants the full wall-miss retry rather than recording the HTTP failure.

8. **MED — [AIPlayerGPT.cpp:11608](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:11608), [AIPlayerGPT.cpp:50475](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:50475):** A completed HTTP 200 with an empty body at ≥95% of the deadline is classified as `timeout` rather than `empty_reply`, and PARSETEST explicitly pins that wrong ordering.  
   **Trigger:** The endpoint returns status 200 and zero bytes after 899 seconds of a 900-second deadline; the request completed successfully at the transport layer, but it opens the wall-miss account and receives a fresh full-deadline retry.

## Verdict

**Not safe to ship as-is.** The principal D5 behavior remains unfixed on the cast route, the new last-answer combat rule can invert explicit negation, and the merged render emits contradictory loop forecasts. Several new affordability and win claims are not implied by the state inspected, while transport status loss and a rules-wrong PARSETEST expectation preserve misclassification that materially changes retry behavior.