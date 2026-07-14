# deck131 seat - wave-7 method contributions (for the wave-7 synthesis)

Mode this wave: OBEYED-BUT-LOSING for the third straight wave (1/6). The value was NOT in a
guide rewrite - it was in the representation rung catching a defect that three waves of
"manabase" framing had walked past. Proposed additions/refinements to the method:

## 1. [PROMOTE - strong, multi-instance] A guide rule can be UNEXECUTABLE because the option it
names is never OFFERED - run the representation rung on the MENU, not just the board.
The Mountain-first rule ("choose Play Mountain") assumed a combined land menu that does not
exist: the engine offers lands ONE AT A TIME as "Play <land> / Hold <land>" and NEVER co-offers
two lands. Verified by the strongest possible check - ZERO decisions in 6 games had both "Play
Mountain" and "Play Island" in the option list. When Islands were surfaced ahead of the Mountain
(hand order), the pilot could not play the Mountain for several turns (vs44 first Mountain
offered T7 with it in hand since T1). This is distinct from the wave-4/5 representation lessons
(which were about a FACT hidden in the board/log): here the ACTION ITSELF is absent from the
option list. METHOD: when a guide rule tells the pilot to PICK a specific option, grep the corpus
to confirm that option is actually OFFERED at the decisions it targets - a rule naming an
unofferable option is a platform defect, and rewording it forever is the trap. This is the
composition-seam rule pointed at the option list.

## 2. [PROMOTE] "Red screw / manabase" (a deck-construction attribution) can be PARTLY an interface
artifact - separate "the source wasn't drawn" from "the source was in hand but unofferable."
For three waves deck131's losses were attributed to a 14I/8M manabase. This wave showed that in
vs44 and vs109 the red source was IN HAND from turn 1 and the menu withheld it; vs110 (Mountains
hand-first) curved out perfectly as the control. Before attributing a color/mana loss to the
decklist, check WHERE the missing source was: not-drawn = deck construction; drawn-but-not-played
= representation. The two have opposite owners. (The decklist flag still stands independently -
vs109/133 were genuinely low on red too - but it is no longer the WHOLE story.)

## 3. [REFINE the sharpen-a-lever step] A sharpened COUNTABLE rule can INVERT into a false positive
when the executor cannot reliably compute the count. Wave-6 sharpened the mulligan to "count the
Mountains; zero = Mulligan." Wave-7 it fired correctly on the one true no-Mountain opener (1/1, up
from 1/3) BUT also FALSE-mulliganed a 2-Mountain keeper (vs133), the pilot citing the guide while
reading its own two-Mountain hand as "zero." The overshoot discipline (a cured leak returns
inverted) applies to COUNTABLE triggers specifically: if the count is over a list the executor
misreads, anchor the trigger to a LITERAL STRING PRESENCE test ("is the word 'Mountain' in your
hand list?") rather than an arithmetic count ("how many Mountains?"), and add the keep-side
guardrail. Track BOTH error directions: `(true-triggers acted)/(true-triggers)` AND
`(non-triggers correctly NOT acted)/(non-triggers)`.

## 4. [CONFIRM, additive] The "NEVER block with your one kill-condition creature" floor needs its
own line even when a general survival-first-at-8 rule exists. vs110 the pilot chump-blocked
Guttersnipe (its only reach) into a non-lethal attack at 14 life. The core high-life-blocking
floor did not catch it because 14 is not "high life" and the block was framed as survival. A
narrow-win-package deck needs an explicit per-card "this specific creature never blocks" line;
n=1 here in an unwinnable matchup, so per-deck, not core - but it is the reflexive-block
convergence-watch reappearing in a new guise (block with the protected creature, not just block
at high life).

## 5. [CONFIRM] OBEYED-BUT-LOSING + precondition-counting held up as the correct posture for a third
wave. n=6 win-rate is noise; the stable signal is per-seam obedience metrics and, this wave, a
representation defect. The one WIN and the timeout both showed the plan executing; every loss
lacked a precondition the guide cannot create (a playable red source early, a threat that sticks,
a matchup the 2-damage clock can beat). A loud rewrite off 1/6 would have churned frozen, obeyed
lines (deck110's faithful-rewrite -> 0/6 warning) and MISSED the land-menu defect entirely, since
that lives in the harness, not the guide.
