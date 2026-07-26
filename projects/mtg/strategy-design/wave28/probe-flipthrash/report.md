# R-FLIPTHRASH-LIVELOCK — engine/scorer fix (wave-28 step-1)

Fix on HEAD ec4e532d4 (worktree agent-a48dc2651cb3ab184). Two files, uncommitted
per wave protocol:
- src/AIPlayerBaka.cpp (OrderedAIAction::getEfficiency, AATurnSide branch ~line 639)
- src/AllAbilities.cpp (AATurnSide::resolve, ~line 5421)

## The bug (carried from waves 22/26/27)
Kaldheim modal-DFC decks (Tergrid probe, deck199) FLIP-THRASH-LIVELOCKED live GPT
games: a game reached turn 9 in ~50 min then stalled on a flood of "dropping stale
async answer", never casting again (wave-27 live_g1_flipthrash_livelock.txt). It
blocked all live Kaldheim-DFC play and was why the wave-27 back-face cast fix
(MTGRules.cpp reroute) never got a clean live exercise.

### Mechanism (two halves of one hack)
1. AIPlayerBaka::OrderedAIAction::getEfficiency, AATurnSide branch, toggled
   ats->source->isFlipped via std::rand() % 2 AS A SCORING SIDE EFFECT.
   getEfficiency runs on EVERY ranking pass, on BOTH the Baka seat and the GPT
   seat (GPT extends Baka and uses Baka's computeActions/ranking) - so the flag
   oscillated every AI tick.
2. AATurnSide::resolve's AI-only branch (if(controller()->isAI() && isFlipped>0)
   isFlipped=0) cleared the flag and fell through to the early return 0 WITHOUT
   reverting the displayed face.

Together: the isFlipped FLAG desynced from the displayed face, and the AI could
oscillate the flag forever without ever flipping the display or casting. The
desync is also exactly what the wave-27 cast fix had to route around (a card
DISPLAYING the back Lantern while the flag read front -> the front "Cast Card
Normally" was wrongly offered for the reduced cost).

## Fix
Both halves were the same hack, so both were removed:

- SCORER MUTATION REMOVED (AIPlayerBaka.cpp): the std::rand toggle is gone; the
  AATurnSide branch now just sets efficiency = 0 (no state mutation). This is the
  branch's own stated default ("AI does not have to use the doubleside ability to
  avoid loops"). The rand toggle never produced a correct back-face cast anyway -
  a flag-only flip does not swap the card's name/cost, so nothing downstream
  actually cast the back. The back face is reached directly through its
  alternative-cost cast option in the cast menu (the modal-DFC alt-cost path,
  proven by tergrid_lantern_altcost_wave24), so the display toggle is not needed
  to cast it.
- COHERENCE RESTORED (AllAbilities.cpp): the AI-only flag-clear-without-display-
  revert line was removed. isFlipped now changes ONLY through the full face swap
  at the bottom of AATurnSide::resolve (name + cost + P/T + types + the flag, all
  together), so the flag and the displayed face stay coherent for AI and human
  alike. This removes the desync SOURCE the wave-27 reroute guards against; the
  reroute remains correct defensive code (its predicate keys on the displayed
  NAME, never the flag).

### Why Baka's legitimate flip play still works
Werewolves / battlefield transforms use AAFlip (day/night triggers), a separate
class untouched by this change - AATurnSide::resolve explicitly refuses to act on
the battlefield (line 5416). The only behavior dropped is Baka PROACTIVELY
toggling which face a modal-DFC hand card DISPLAYS, which was broken (livelocking)
and never produced a correct cast. Scripted flip paths
(effect_doubleside_turns_back_face, tergrid_lantern_altcost_wave24) drive
AATurnSide::resolve directly and are unaffected (all green).

## GPT display-toggle pseudo-actions (requirement d - investigation; AIPlayerGPT.cpp out of scope)
The "Flip Side" display toggle still reaches the GPT seat via TWO seams:
- the priority list (chooseOrderedAction), where describeAction annotates it
  "DISPLAY TOGGLE only ..." and a 2/turn flip cap applies;
- the modal-DFC CLICK MENU (chooseMenuAction), rendered as a bare "Flip Side"
  with no annotation and no filter (observed live at g1 seq 21: "1. Cast Card
  Normally / 2. Flip Side / 3. Decline").

Both are mechanically-no-op UI dead-ends for GPT (the back face is castable via
alt-cost without flipping), so the owner's sanctioned no-op-dead-end suppression
DOES apply and could remove "Flip Side" from both seams. That code lives in
AIPlayerGPT.cpp, which is out of this agent's scope - flagged for the AIPlayerGPT
owner. Note: in g1 the model handled the bare "Flip Side" menu option correctly
on its own - it reasoned "The 'Flip Side' option just changes the visual
representation of the card and does not cast it" and cast the front God (choice 1,
0 fallbacks).

## Validation
Gates (fixed binary, single-thread WAGIC_TESTSUITE_THREADS=1, WAGIC_AI=baka):
- PARSETEST (WAGIC_GPT_PARSETEST): 101 passed, 0 failed.
- main (test/_tests.txt): 1027 tests (0 failed), 11 AI tests (0 failed).
- lexicon (test/lexicon/_tests.txt): 821 tests (0 failed).
- Both failure strings (==Test Failed !==, ==Test timed out) via /usr/bin/grep: 0
  across all runs. git diff U+FFFD count: 0.
- wave-27 counterfactual milestone tergrid_lantern_altcost_wave24: PASS (inside
  the lexicon set - the R-DFC-BACKFACE reroute is preserved).

LIVE (fixed binary, Spark qwen35 up @ :8081; deck199 Tergrid (*) *8 vs deck198,
WAGIC_AI=gpt WAGIC_SELFPLAY_ONESHOT=1, timeout 3000):
- g1: clean natural end - winner=1 life0=0 life1=8 turn=17; 108 cast/stack events;
  0 "dropping stale async answer"; 0 isFlipped oscillation; Tergrid (front God)
  cast and resolved coherently. 34 translog records, 0 fallbacks.
- g2: clean natural end - winner=1 life0=-2 life1=20 turn=27; 318 cast/stack
  events; 1 stale drop (normal ~2% self-heal); 0 isFlipped oscillation.

THE LIVELOCK IS GONE: both games progressed at normal pace and reached decisive
conclusions in the timeout window, versus the wave-27 ~50-min stall at turn 9. No
isFlipped oscillation, no stale-answer flood.

The wave-27 back-face cast (a displayed Lantern resolving as the Lantern) remains
UNOBSERVED live - in g1 the model cast the front God (a 4/5 Menace it judged the
stronger line and explicitly recognized Flip Side as visual-only), and in g2 the
Tergrid seat lost before casting Tergrid. The wave-27 deterministic counterfactual
remains the primary proof of that reroute; the desync it guarded against can no
longer arise from the AI scorer.

## Files
- live_g1.stderr, live_g2.stderr - full game stderr.
- g1-deck199-translog.jsonl / g1-deck198-translog.jsonl - g1 per-seat translogs.
- g2-deck199-translog.jsonl / g2-deck198-translog.jsonl - g2 per-seat translogs.
- deck199.txt / deck198.txt - the exact probe decks used (deleted from the live
  pool after harvest; highest real deck = 164).
