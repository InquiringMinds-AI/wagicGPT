# Core-prompt / render proposals from the deck126 seat (wave 46)

## G-D - The blockers screen prints a rival number beside the verdict it wants read, and does not
## say which wins
Every wall B-line on this seat carries two facts and no precedence between them:
```
B1. Perimeter Captain (0/4) [defender] [deals 0 - this block kills nothing, but it STOPS all 1
damage from reaching you] - may block A1 (your blocker dies, attacker lives (blocking trigger:
you may gain 2))
```
`STOPS all 1 damage` is a quantity; `your blocker dies` is a verdict. Both are true. The seat's one
loss is two windows where the quantity beat the verdict (turn 4: a 0/4 Perimeter Captain traded for
ONE point of stopped damage against a 1/1 first-strike deathtoucher; turn 6: a 0/3 Pride Guardian
took the 3-damage attacker that kills it over the 1-damage attacker it survives). The same
carve-out fired correctly two games later on a Vampire B-line - which carries no STOPS tag.
This is a guide problem FIRST and I have fixed it there (F1 states the precedence outright). The
render proposal is narrower and I want it judged on its own: the STOPS clause is currently unscoped
about DURATION, and one clause would scope it truthfully -
```
[deals 0 - this block kills nothing, but it STOPS all 1 damage from reaching you THIS COMBAT]
```
Four characters of scope on a clause that is otherwise read as a standing property of the wall.
I am NOT proposing the render carry the verdict ("do not block") - that is strategy and belongs in
the guide. I am proposing the existing quantity stop overstating its own reach.

## G-E - `ATTACK: none` has no rendered basis when the A-line carries no blocker clause
An attackers A-line comes in three shapes on this seat: with a named blocker parenthesis, with the
collapsed `they have N untapped creatures that could block this one` tag, and BARE
(`A1. Vampire (1/1) [lifelink, doesn't untap during its controller's untap step]`, no blocker
clause at all - `...vs-ai_baka_deck123.jsonl` seq 18). The first two say what happens if you swing;
the third says nothing, and the pilot answered `ATTACK: none` to it and threw away a free point.
The bare form is presumably "they have no untapped creatures", but the prompt never says that, and
absence-of-a-tag is the one thing a reader cannot distinguish from an omission. A positive
statement on the bare case - `[their untapped blockers: none]` - costs one line and closes the
inference. Note the trust doctrine cuts the same way: teaching a pilot to read meaning into a
MISSING clause is teaching it to doubt the render.

## G-F - The auto-pass is working and its counter should stay in the gameend record
`mana_only_windows_skipped` did exactly what the ledger asked of it: 180 skips, all 180 on this
seat, ~85% of the seat's priority churn removed, zero plays lost (the 3 leaks were all passed
anyway). Two notes for whoever owns lane I:
 - the counter belongs in `gameend` permanently, not just for this lane's validation wave. It is
   the only way a reviewer can tell a rate change from a denominator change, and this wave it
   was the difference between reading 1/16 as a regression from 2/39 and reading it correctly.
 - the three leaks all follow a declined CAST menu in the same phase and none of them shows a
   floating pool, so the residual is `menuObject` or `getCurrentTargetChooser()`, not
   `getManaPool()`. Repro in the seat file, MED item #2.
