# Findings

1. **HIGH — [DecisionContract.cpp:22](/home/magi/Projects/wagicGPT/projects/mtg/src/DecisionContract.cpp:22), [DecisionContract.cpp:104](/home/magi/Projects/wagicGPT/projects/mtg/src/DecisionContract.cpp:104), [AIPlayerGPT.cpp:38426](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:38426):** The new planeswalker route still requires `canAttack()`—the player-attack predicate—so it removes every legal attacker that can attack a planeswalker but cannot attack the defending player.  
   **Trigger:** A creature with `CANTATTACK` but not `CANTPWATTACK` faces an opposing planeswalker; `canAttack(true)` is true, but the creature is omitted from `req.candidates`, and both later guards would drop it anyway.

2. **HIGH — [AIPlayerBaka.cpp:5550](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerBaka.cpp:5550), [AIPlayerBaka.h:226](/home/magi/Projects/wagicGPT/projects/mtg/include/AIPlayerBaka.h:226):** The generic seat driver forcibly passes priority after 24 consecutive menu answers, removing whatever legal post-menu action remains solely because a hard counter expired.  
   **Trigger:** A repeatable payment or chained mandatory-choice sequence resolves 25 menus while the seat retains priority and then exposes another legal cast/activation; answer 25 falls through to `userRequestNextGamePhase()` instead of giving that action a decision window.

3. **HIGH — [AIPlayerGPT.cpp:12344](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:12344), [GptConfig.cpp:871](/home/magi/Projects/wagicGPT/projects/mtg/src/GptConfig.cpp:871):** Transport classification treats every curl result 28 as a full decision timeout before considering elapsed time, although curl uses the same result for the separately bounded connection timeout.  
   **Trigger:** With a 900-second decision deadline and the configured short `CURLOPT_CONNECTTIMEOUT_MS`, connection establishment expires after tens of seconds with `(http=0, curl=28, empty=true)`; `gptDeadlineMissed` returns true, producing `timeout` and a fresh full-deadline retry instead of `transport_error` and the bounded remainder retry.

4. **HIGH — [AIPlayerGPT.cpp:20093](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:20093):** The crack-back tag states “which you survive” after subtracting attackers the new bodies may be unable to block, while admitting only afterward that evasion was excluded.  
   **Trigger:** At 3 life, the opponent’s only projected attacker is a 5/5 flyer and the cast row is for a ground creature; the tag computes `5 -> 0`, says the caster survives, but the creature cannot block and the actual crack-back is lethal.

5. **HIGH — [AIPlayerGPT.cpp:16914](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:16914), [AIPlayerGPT.cpp:63414](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:63414):** Plan termination disagrees with the actual answer scanners by refusing leading whitespace, so a valid indented top-level correction is demoted as plan prose—and PARSETEST explicitly pins that wrong expectation.  
   **Trigger:** `CHOICE: 1\nPLAN: reconsidering\n  CHOICE: 2` is recognized as a coded line by `codedAnswerLineAt`, `findAnswerLabelLine`, and the menu parser, but `lineIsCodedAnswerLine` returns false; the correction loses to choice 1 and the test requires this behavior.

6. **MED — [AIPlayerGPT.cpp:25276](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:25276):** Modal pricing skips only the individual `&&` clause containing `ability$!`, not the complete granted-ability block, allowing later clauses inside that block to be attributed to the wrong player and to emit false kill verdicts.  
   **Trigger:** `choice name(Gift) ability$!name(x) draw:1 controller && life:-3 controller!$ opponent` at 3 life skips the first clause but prices the second as “you LOSE 3 … THIS KILLS YOU,” although the entire payload is granted to the opponent.

7. **MED — [AIPlayerGPT.cpp:20031](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:20031), [AIPlayerGPT.cpp:20095](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:20095):** `castBodiesAdded` counts every creature spell as a blocker without checking `CANTBLOCK` or whether it enters tapped, so the row can claim blockers that will not exist as legal blockers next turn.  
   **Trigger:** Casting a creature with “enters the battlefield tapped” during second main against a lethal next-turn attack prints “This adds 1 blocker” and may claim survival, although it remains tapped throughout the opponent’s combat.

8. **MED — [AIPlayerGPT.cpp:4941](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:4941):** Gang-block simulation silently caps a first-striking attacker’s damage-assignment budget at 64, which can falsely claim that a large attacker dies to a gang it actually kills before normal damage.  
   **Trigger:** A 100/25 first-striker is blocked by four 20/30 creatures; the capped DP can remove only two and reports the remaining 40 as lethal, while the real 100 first-strike damage kills three, leaving only 20 damage—insufficient to kill the attacker.

9. **MED — [AIPlayerGPT.cpp:11255](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:11255):** The library “best X” reserve counts every permanent’s next upkeep draw without considering that the current upkeep trigger may already have occurred, thereby falsely lowering the claimed best X.  
   **Trigger:** During the caster’s upkeep after Staff of Nin’s trigger has resolved, casting an X-draw instant with 12 library cards reserves both the imminent draw step and another Staff draw, calling X=10 best even though X=11 safely leaves the card needed for the imminent draw.

## Verdict

**Not safe to ship as-is.** The diff contains two direct doctrine violations—a legal planeswalker-attack class remains unreachable and the shared AI driver hard-passes after a fixed menu count—plus false lethal/survival guidance and a transport-order regression that can buy an entire extra deadline. The green suite is not discriminating for these states, and one new PARSETEST block actively enshrines the plan-boundary defect.