# deck123 - wave-57 guide edits, before -> after

Start point: the LIVE guide `projects/mtg/bin/Res/ai/baka/deck123_strategy.txt` (70,887 bytes =
the wave-56 reviewer revision `1e1dcd5ab` + the wave-56 boundary pass `a93aad889`).
Result: `wave57/deck123/strategy.txt`, **70,921 bytes (+34, effectively flat)**: five additions
paid for by ten trims of rungs that held on large denominators this corpus.

Every number quoted below is counted from the RENDERED `prompt` of my six deck123 seat logs in
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-174505/`, never from the `options` array.

---

## A57-1 (RULE 5 / WHAT TO CAST) - the deadness test is the ROW, never the menu
**Finding.** `123v130` seq 45 (turn 22, 13 life) and seq 49/50 (turn 24, 7 life): a
`Cast Devour Flesh {1}{b} {right now: they control 1 creature - Rorix Bladewing (6/5) [flying,
haste, doesn't untap during its controller's untap step] is sacrificed, they gain 5}` row - an
N = 1 edict on the only creature they controlled, the 6/5 that was hitting for 6 a turn - sat
under the menu-level line `NO LIVE CAST ROW ON THIS MENU: all N cast rows below carry a verdict
computed from the board that reads zero`. The pilot took the hold row all three times and lost
that game at -2 on turn 28. RULE 5 had already answered it (N = 1, the body is killing you);
what it lost to was a menu-level summary that contradicts the row under it. **6 of my 92
all-dead menus carry a demonstrably live cast row** (`123v152` s18, `123v130` s7/s45/s49/s50,
`123v146` s16) - see ENGINE E1 in the seat file. Perception, not strategy; the guide edit only
states the SCOPE of its own test and does not hedge against the render.

**Before**
```
whose rows all read "destroys 0 of their creatures ... 0 of yours", "they control 0 creatures -
at 0 this does nothing" and "[second copy: ... a second copy changes nothing]" is a menu with
nothing on it, and "Cast nothing right now" is the only row that keeps a card. Last corpus 85 of
this deck's 197 casting menus were that menu and it spent ONE card off them - the Greaves above.
Take that to zero. There is no rule in this file
```
**After**
```
... is a menu with
nothing on it, and "Cast nothing right now" is the only row that keeps a card. Last corpus 92 of
this deck's 345 casting menus were that menu. There is no rule in this file
that says you must cast something. ...
THE UNIT OF THIS TEST IS THE ROW, NEVER THE MENU. Every rule in this file that says "cast" or
"do not cast" reads ONE row's own {right now: ...} clause and nothing else: Rule 3 reads the
Damnation row's two counts, Rule 4 reads "{kills: <name>}", Rule 5 reads "they control N
creature(s)", Rule 6 reads "this equips nothing". Those clauses are the input. A line printed
ABOVE the numbered rows that describes the menu as a whole is a SUMMARY of them and is not one
of the inputs to any rule here - when it is on the screen, still read row by row and answer the
row. [the Rorix repro] ...
(Drop this paragraph once every menu-level summary on this deck's windows agrees with the row
verdicts under it.)
```
The removal condition is the doctrine's requirement: the rule routes around the false surface
by naming which true number it reads, and retires when the surface is fixed.

---

## A57-2a (RULE 1, timing) and A57-2b (COMBAT CHECK 5) - a maker you attack with cannot fire the chain
**Finding.** `123v162` seq 31, turn 12, 14 life. Board: Thraben Doomsayer x2, Bloodline Keeper,
Human x2, all five on the attack list, every one carrying `[doesn't untap during its
controller's untap step]` (its own Intruder Alarm). Reply: `ATTACK: A1, A2, A3, A4, A5` /
`PLAN: L=20, C=1, stop=24; M=5 now; this window: attack with all 5. Fire chain on opponent's
turn.` The chain never fired again: under the Alarm a tapped creature untaps only when a
creature ENTERS, and only an UNTAPPED maker makes one enter. At seq 67 (turn 19) the same five
creatures read `(4/4) (printed 2/2)` / `(7/7)` / `(5/5)` and `[tapped - cannot attack or block
this turn]` - **26 power stood tapped for seven turns** while Underworld Dreams x2 took it from
14 to 0. The guide's own two rules ("fire the chain on the OPPONENT'S turn" and CHECK 5 "send
every creature on that list") combine into this line and neither said the makers are the
exception. STRATEGY: the render printed the untap tag on every creature.
Primitive check: `Intruder Alarm` (mtg.txt:58850) `auto=lord(creature) doesnotuntap` +
`@movedTo(creature|myBattlefield):untap all(creature)`.

**A57-2a, after "Fire on your OWN turn only when ..." - ADDED**
```
AND THE MAKER YOU SEND AS AN ATTACKER IS THE MAKER THAT CANNOT FIRE IT. Attacking taps the
creature; under your own Intruder Alarm a tapped creature untaps only when another creature
ENTERS, and the only thing that makes one enter is an UNTAPPED maker tapping for a token. So a
turn that attacks with every maker ends the chain for good ... ONE MAKER STAYS HOME, every
attack, while the Alarm is on your battlefield - or fire the chain to its stop in your own main
phase FIRST, so the last token entering untaps the team and the makers you send have already
done their work this turn.
```
**A57-2b, CHECK 5, before -> after**
```
-  So before you send everything, find a
-  token-maker on your battlefield line. If one is there, the whole team is back ...
+  So before you send everything, find a
+  token-maker on your battlefield line AND KEEP IT OFF THE ATTACK. A maker that attacks is
+  tapped too, and a tapped maker cannot tap for the token that untaps everyone: sending all of
+  them is the one attack that cannot be undone. With one maker held back the whole team is back ...
```

---

## A57-3 (RULE 6) - Greaves block cut from 11 lines to 4
**Finding.** P1 of wave 56 - the prediction my predecessor said it cared about most - **PASSES**:
**45 rows carried `{right now: you control 0 creatures - this equips nothing}`, 0 taken**
(wave 56: 62 rows / 1 taken). The rung is now a one-sentence rung. The two worked examples and
the "sixty-two of sixty-four" bookkeeping are removed; the clause and the verdict stay.

---

## A57-4 (KEY CARDS) - Fateful hour, ADDED
**Finding.** `123v162` seq 67 (turn 19, 1 life): `Thraben Doomsayer #1 (4/4) (printed 2/2)`,
`Bloodline Keeper (7/7) (printed 3/3)`, `Human #1 (5/5) (printed 1/1)`. Primitive:
`Thraben Doomsayer` (mtg.txt:120583) `auto=this(controllerlife < 6) lord(other creature|
mybattlefield) 2/2`. The render prints the buffed numbers, but nothing in the guide told the
pilot that the life total the rest of the file treats as losing is the total at which its attack
list doubles in size. Added six lines under KEY CARDS.

---

## A57-5 (INTRUDER ALARM) - the second-copy bracket broke again
**Finding.** `123v130` seq 55, turn 25: `Cast Intruder Alarm {2}{u} [second copy: you already
control Intruder Alarm; ... a second copy changes nothing]` - TAKEN, with an Alarm already on
the battlefield line and {2}{u} the only mana that turn. **1 of 6 `[second copy:` rows**
(wave 56: 0 of 24 Alarms). The paragraph was on my trim list and comes OFF it: the stale
"seven times across four games" is replaced by this corpus's repro.

---

## A57-6 (RULE 0) - the named-row re-ask now has an emitted string
**Finding.** Both of this seat's two fallbacks are `named_row_reask` and both now render
`<refused: named_row_reask>` as `chosen_text` (lane A's D4 refusal path, UNTESTED at these seats
last corpus, TESTED now): `123v126` s35 named "Equip with Lightning Greaves" on a menu of casts;
`123v130` s9 named "Cast Bloodline Keeper" on a menu that did not list it. The stale "EIGHT last
corpus" count is replaced by the two repros and the emitted string.

---

## Trims (each on a rung that held this corpus, and each falsifiable - see P4 in the seat file)
| # | where | what came out | the rung's number this corpus |
|---|---|---|---|
| T1 | RULE 0 | three paragraphs of land-drop justification -> one sentence | 55 asks, 55 lands played |
| T2 | RULE 3 | the four-corpora case list under the three-number device | 54 rows, 3 casts, all at M = 0 |
| T3 | RULE 3 | "ENTRY 0 HAS NO EXCEPTIONS" worked example | - |
| T4 | RULE 3 | the N = 0 second-main-phase example | 0 casts at `destroys 0` |
| T5 | RULE 2 | added the denominator, removed the {U}-fetch death anecdote | 4 cracks / 4 first-window, 0 colour breaks |
| T6 | RULE 5 | the "FIVE of their creatures" anecdote | 267 rows, 4 casts, all at N = 1 |
| T7 | RULE 5 | the 113/125 wave-55 counts -> this corpus's | as above |
| T8 | RULE 5 | the Wall-of-Omens and Katilda anecdotes | 0 casts on a defender/power-0 body |
| T9 | RULE 1 | the hold-row re-ask bookkeeping | 140 hold takes, 0 same-turn identical re-asks |
| T10 | RULE 3 | the "-208" sweep anecdote and the M = 5 anecdote | 0 casts at M >= 4 (one row printed "246 of yours") |
