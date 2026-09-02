# deck123 -> core-prompt / render proposals (wave 52)

Layer-routed. Engine defects are in `wave52/seats/seat-123-130.md`; everything below is a change
to what the prompt SAYS or to how the parser reads a reply, not a bug.

## G1 (HIGH, render). A second copy of a card whose effect does not stack should say so.
The lane-K bracket `[second copy: you already control Intruder Alarm; both stay on the
battlefield - no legend rule]` (14 renders at this seat) answers a legality question. It replaced
`[already owned: Intruder Alarm on your battlefield - this finds only an enchantment card]`,
which answered a usefulness question. Casts of a dead second Alarm under the new bracket: **4**
(v126 s74, v162 s39, v125 s41 at 15 life, v125 s59 at 6 life). Under the old bracket, wave 51
recorded 1 in 68 renders.
Ask: keep the legality clause and append the verdict when the card's own script has no stacking
term - `[second copy: legal, but its effect is already on the battlefield and a second copy
changes nothing]`. Intruder Alarm's primitive is `lord(creature) doesnotuntap` plus two
`@movedTo(creature|...):untap all(creature)` triggers; Talisman of Impulse (deck130, 6 renders,
5 correct casts) genuinely stacks and should keep the current wording. The distinguishing test is
in the script, not the card type.

## G2 (HIGH, render). Spell CAST rows should carry a kill summary the way X-spell cast rows do.
deck130's `Cast Starstorm` row prints `{X pricing: max affordable X=N; ... even at X=N NOTHING
the OPPONENT controls dies}` and that clause produced 0 wasted casts in 60+ windows this corpus.
deck123's `Cast Tragic Slip {b}` row prints `{right now: -1/-1 (no creature has died this turn,
so Morbid does NOT apply)}` and `- legal targets right now: Nadaar, Selfless Paladin, Triumphant
Adventurer, Goblin` - and nothing about whether any of them die. The pilot took it (v146 s18) and
the target ask (s19) then offered three `- SURVIVES` rows with "this ask has no pass row".
Ask: on a targeted spell's cast row, append the same shape the target rows already compute -
`{kills 0 of the 3 legal targets at -1/-1}` / `{kills: Elite Spellbinder}`. The engine already
has the number; it prints it one screen too late to be refusable.

## G3 (MED, render). The LIFE-TO-DAMAGE CONVERTER block is not folded into the rows it prices.
v126 s66: the board frame printed the converter block for Sanguine Bond, and the edict row
printed `they control 1 creature - Overgrown Battlement is sacrificed, they gain 4`. The
consequence - 4 off the pilot too - appears in neither. Result: 14 -> 10 and 26 -> 30 in one
window.
Ask: where a converter is on the battlefield, any row whose annotation contains "they gain N"
should continue `- and their Sanguine Bond takes N off YOU: life 14 -> 10`. This is the same
construction lane K already applies for `{paying this costs you N life ... you would be at K}`,
which rendered 120 times at deck130 and was never broken.

## G4 (MED, render). The edict row does not repeat the guide-relevant facts about the body it
## will take.
The same row named `Overgrown Battlement` without repeating that it is a `[defender]` with power
0 - both of which are on the battlefield line, two screens up, among ten permanents.
Ask: at N=1 the row already names the creature; append its printed `(P/T)` and any `[defender]`
tag, as the target rows do.

## G5 (MED, parser). A PLAN template whose slot disagrees with its own arithmetic should re-ask.
v162 s33: `PLAN: Stop creating humans at M=40 (L+C+3 = 20+2+3=25)` under
`CHOICE: 2 (Create human with Thraben Doomsayer x17)` at a row reading `you control 23 creatures`.
Lane J's `plan_choice_conflict` compares the CHOICE to a taken/pass verdict in the prose; it has
nothing to say about a number in the plan contradicting the sum printed beside it.
Ask: when a reply's PLAN line contains both `= <k>` and a differently-valued `M = <j>` / `stop
<j>` naming the same quantity, re-ask once (`plan_arithmetic_conflict`), the same way
`plan_missing` re-asks a counted repeat with no PLAN.

## G6 (MED, prompt). The opponent-turn casting ask repeats a decision the board has not changed.
Lane I is a clear win (140 opponent-turn casting asks at these two seats, 0 in wave 51; three
real casts, two of them the best plays of their games). The cost: **36 of the 140** offered no
row but a dead one - `Cast Devour Flesh {1}{b} {right now: they control 0 creatures - at 0 this
does nothing}` and `Cast nothing right now` - re-asked at Upkeep, Draw, Main 1, Combat begins,
Attackers, Combat ends, Main 2, End and Cleanup of the same turn. 26 of them are one game
(v130, turns 4-10, 11 windows a turn), which is 26 model calls, ~4 minutes of latency and ~26
full prompts to answer "no" to the same one-row menu.
Ask (prompt/orchestrator, NOT the legality layer - the window must stay open): when a casting ask
on the opponent's turn would offer the identical option set with an unchanged stack and unchanged
battlefield lines since the last such ask this turn, and every cast row on it carries a
"does nothing" clause, skip it and record it in `mana_only_windows_skipped`'s sibling counter.
Choice is not constrained: any board change, any new stack object, any newly-affordable row
re-opens it.

## G7 (LOW, prompt). Reply-length cost is concentrated in arithmetic the frame already did.
`post_answer_overrun` was non-zero on 258 of 306 deck123 decisions, max 6,369 characters. The
longest example at either seat (deck130 v152 s17, 577 s, 3,857 characters after the answer) is
900 words of the model computing which turn it will reach five mana - a number the frame prints
per-window but never projects. Latency > 60 s: 25/306 here, 36/381 at deck130; two decisions hit
the 900 s wall exactly (v130 s3, deck130 v162 s18) and returned empty.
Ask: consider adding one projected line to the mana block - `Next turn, with a land drop, this
becomes N+1 sources` - and consider whether the reply rules should say that arithmetic the frame
has already printed does not need to be redone in the reply.
