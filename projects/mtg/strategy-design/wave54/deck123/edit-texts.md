# deck123 (Intruders of Thraben) — wave-54 edits, before -> after

Baseline: the live guide `projects/mtg/bin/Res/ai/baka/deck123_strategy.txt` (70,230 bytes,
wave-53 reviewer revision + boundary pass). Revised file: **71,115 bytes (+885, +1.3 %)** — nine
substantive edits paid for by fourteen compressions of rungs that have now held for two or more
corpora. Every edit below cites the seq that bought it. Corpus:
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-034355/`, six deck123 seat logs, 558 decisions.

---

## A54-0 — MANDATORY RE-KEY (B1 REVERSE audit): the HOLD row's literal changed under lane A
**Finding.** The guide quoted the row twice as `"Hold priority - do not ask me again this turn
unless the board changes"`. That string rendered **0 times** in this corpus at either seat. The
emitter now writes `"Pass priority, and do not ask me again this turn unless the board changes
(any change re-opens this window; you give up no cast)"` — **275 renders** at deck123 alone. Two
sentences of this guide were false on every window they described.
**Also**: the row is now being taken (32 takes / 275, up from 1 / 249) but taken LATE —
`123v125` turn 41 declined ten byte-identical menus and held on the *Cleanup* ask (s181-s191),
turn 55 declined eleven and held at End (s241-s252), turns 43 and 57 declined twelve and thirteen
and never held at all (s198-s209, s256-s268). A hold in Cleanup saves nothing.

BEFORE (two places):
```
"Hold priority - do not ask me again this turn unless the board changes". When the only cast row
is dead ... decline once and take the Hold row on the very next ask of that turn.
```
```
  list, and the last row reads "Hold priority - do not ask me again this turn unless the board
  changes": take THAT row instead of declining again.
```
AFTER: both quotes replaced with the emitted string, and the timing re-keyed to
"take that row at the **FIRST** dead window of the turn, not the fifth", with the turn-41/43/55/57
evidence written in. (`123v125` s181-s191, s198-s209, s241-s252, s256-s268.)

---

## A54-1 — NEW: an all-dead menu is a "Cast nothing right now" menu
**Finding.** In `123v125` this deck spent **five cards off rows whose own annotation said they did
nothing**, all in one game, all with the reason in its own plan: s147 and s192 and s269
`Cast Intruder Alarm [second copy: ... a second copy changes nothing]` ("dead, but only way to
proceed"), s176 `Cast Damnation {right now: destroys 0 of their creatures ... 0 of yours}`
("PLAN: Damnation: N 0, K 0, M 0; cast."), s211 `Cast Tribute to Hunger {right now: they control 0
creatures - at 0 this does nothing}`. On four of the five, every other cast row on the menu was
also annotated dead. Classification: STRATEGY. The engine had already priced every row correctly.

BEFORE (cast list, entry 8):
```
 8. "Cast nothing right now" - the LAST entry, and correct only when no entry above appeared in
    your casting menu. Every card you can cast has a line above this one.
```
AFTER: entry 8 keeps its text and gains a paragraph — `WHEN EVERY CAST ROW ON THE MENU IS PRICED
DEAD, ENTRY 8 IS THE PLAY, AND IT IS NOT INACTION`, naming the three dead-clause shapes verbatim,
the five casts, and the pilot's own "dead, but only way to proceed" sentence, and closing:
"There is no rule in this file that says you must cast something."

---

## A54-2 — Intruder Alarm second copy: re-keyed to the new bracket, count updated
**Finding.** 3 takes this corpus (`123v125` s147, s192, s269) against 0/8 last corpus — a
regression, and P8 FAILS. The bracket now *ends in the verdict*: `[second copy: you already
control Intruder Alarm; both stay on the battlefield - no legend rule, but its effect is already
on the battlefield and a second copy changes nothing]` — the guide quoted only the older, shorter
form. Every one of the three replies wrote the row off in prose first ("second copy is dead",
"legal but redundant") and then answered its number.

BEFORE: `... both stay on the battlefield - no legend rule]". That clause answers "is this LEGAL"
... This deck cast a second Alarm under that bracket four times across three games ...`
AFTER: quotes the full current bracket including `a second copy changes nothing`, updates the
count to seven across four games with the three turn numbers, and adds the operative sentence:
"Writing DEAD next to the row and then answering its number is the whole failure. Read the bracket
as the word DEAD and then answer a DIFFERENT number."

---

## A54-3 — NEW: the PLAN line may not say "pass" / "stop reached" on a window that takes a row
**Finding.** Five `plan_choice_conflict` fallbacks in `123v126` (s29, s58, s62, s90, s95) and two
in `123v146` (s23, s26) — **7 of my seats' 16 fallbacks, and 7 of the corpus's 23**. Every one is
the repeat-row PLAN the guide itself prescribes, written in a shape the parser reads as a
reversal: `CHOICE: 2 (Create human with Thraben Doomsayer x2)` + `PLAN: ... creating 2 humans to
reach M=30 (stop reached).` Four of the seven re-asks returned the identical reply
(`plan_choice_conflict_exhausted`, s30/s59/s24/s27) and the window was simply spent. One was a
true positive and it corrected the play (s90 -> s91).

BEFORE: the PLAN template ended at
```
        PLAN: L <opponent life>, C <their creature count>, stop <L+C+3>; M is <M> now;
        this window: <x<N> / pass>
```
AFTER: the template plus `THE PLAN LINE MAY NOT CONTAIN THE WORD "PASS", OR "STOP REACHED", ON A
WINDOW WHERE YOUR CHOICE LINE TAKES A ROW`, both offending shapes quoted verbatim, the cost
(five re-asks in one game, two in another, four identical) and the closing rule: "If the stop IS
reached, the CHOICE line is CHOICE: 0 (pass) and the plan says pass. If it is not, neither line
says pass."

---

## A54-5 — NEW BRANCH B2: Sanguine Bond **and** Exquisite Blood is a closed loop, not arithmetic
**Finding.** `123v126` s88: 58 attackers, 6 untapped blockers, opponent on 28 life. Branch B's own
arithmetic (A >= 3B + 3 = 21) said ATTACK, and the guide's lethal override said ATTACK. The pilot
answered `ATTACK: none`. **The pilot was right and the guide was wrong.** Primitive check:
Sanguine Bond `auto=@lifeof(player) ... :life:-thatmuch opponent` (mtg.txt:99004) and Exquisite
Blood `auto=@lifelostfoeof(player):life:thatmuch controller` (mtg.txt:37966) form a closed loop;
Pride Guardian gains 3 **on block** (88356) and Perimeter Captain 2 (85398), and blocking triggers
resolve BEFORE combat damage — so one block ends the game at the pilot's zero, at any width. The
render says exactly this and says it in words: `(their life LOOP is in play: any life they gain or
you lose in this combat chains without limit - fatal to you, not a trade)` and `Both halves of
their life LOOP are in play`. The guide's width rule contradicted a true render.

BEFORE: `BRANCH B - SANGUINE BOND or EXQUISITE BLOOD is on the line: you are never removing it, so
you attack on ARITHMETIC instead of on the name.`
AFTER: a new **BRANCH B2** above it for BOTH names on the line — quotes the render's two clauses,
states that the gain resolves in declare-blockers before damage, records the 58-vs-6-at-28 board,
and gives the only real line ("an attack no life-gaining body can block"); the old Branch B is
re-scoped to "exactly ONE of" and keeps its arithmetic. The CHECK 1 preamble now says THREE
branches.

---

## A54-6 — the N = 0 edict row's own verdict, quoted
**Finding.** `123v125` s211, `Cast Tribute to Hunger {2}{b} {right now: they control 0 creatures -
at 0 this does nothing}` taken, off a three-row menu whose other rows were a dead Damnation and
"Cast nothing right now". Wave 53 was 0 of 238 dead edict rows; this is the first break.

BEFORE: `(2) If N is not exactly 1, do not cast an edict - not at 0, not at 2 or more.`
AFTER: the same, plus the emitted N=0 clause quoted verbatim, the turn-44 board, and
"A row that says it does nothing does nothing."

---

## A54-TRIM1 — RULE 3: the Damnation lane is exhausted, and the guide now says why
**Finding.** Four corpora at this rung. This corpus is the FIRST where the mandated three-number
PLAN line was actually written — `123v125` s176 `PLAN: Damnation: N 0, K 0, M 0; cast.` — and the
dead Damnation was cast anyway, on the same line. Writing the numbers does not gate the decision.
BEFORE: three stacked anecdotes ("THREE CORPORA RUNNING ... zero of the two Damnation windows in
the last corpus carried it").
AFTER: compressed to one sentence, and a new closing paragraph: `AND WRITING THE THREE NUMBERS IS
NOT THE SAME AS OBEYING THEM`, quoting s176's plan line. This is also the evidence for the
seat file's verdict that the guide lane here is dead and the remaining lever is the render
(see `general-suggestions.md` G1).

## A54-R6 — Lightning Greaves onto an empty board
**Finding.** `123v130` s29, `Cast Lightning Greaves {2}` with the battlefield line reading
"of which 0 are creatures", at turn 15 of a game it lost with its lands being stripped. Wave 53
was 0 of that shape; this is 1. The cast-list entry 5 already forbade it; RULE 6 did not.
AFTER: RULE 6 gains `AND DO NOT CAST THE GREAVES ONTO AN EMPTY BOARD`, with the seq's board line.

---

## A54-7 — NEW, and it is the whole `named_row_reask` story: the cast list's numbers were being answered as CHOICE numbers
**Finding.** All EIGHT `named_row_reask` / `named_row_not_offered` fallbacks at this seat are one
shape. The reply names a card that is **not on the menu**, with the number that card carries in
this guide's own cast list:
| seq | reply's first line | that number on the MENU | guide cast-list entry |
|---|---|---|---|
| `123v125` s37 | `CHOICE: 1 (Cast Bloodline Keeper)` | Cast Damnation | entry 1 = Bloodline Keeper |
| `123v125` s56 | `CHOICE: 7 (Cast Vision Skeins)` | (5-row menu; 7 out of range) | entry 7 = Vision Skeins |
| `123v125` s58 | `CHOICE: 7 (Cast Bloodline Keeper)` | out of range | - |
| `123v125` s100 | `CHOICE: 1 (Cast Bloodline Keeper)` | Cast Damnation | entry 1 |
| `123v125` s105 | `CHOICE: 7 (Cast Vision Skeins)` | out of range | entry 7 |
| `123v125` s107 | `CHOICE: 7 (Cast Bloodline Keeper)` | out of range | - |
| `123v125` s150 | `CHOICE: 1 (Cast Bloodline Keeper)` | Cast Damnation | entry 1 |
| `123v130` s26 | `CHOICE: 5 (Cast Bloodline Keeper)` | Cast nothing right now | entry 5 = Greaves |
Bloodline Keeper was not in hand in any of them. Lane B's name-over-index machinery caught every
one (`stale_echo_in_range` on the three in-range cases, `named_row_not_offered` on the rest) and
**zero wrong cards were cast** — but eight windows and seven `recovery` records were spent, and
five of the eight recovered to `Cast nothing right now`. The guide already warned about naming a
card with no Cast row; it did not notice that its own list was supplying the numbers.

BEFORE: `=== WHAT TO CAST WHEN SEVERAL THINGS ARE LISTED ===` / `Take the highest entry on this
list that appears in your casting menu:` / ` 0. "Cast Damnation ..."` ... ` 8. "Cast nothing ..."`
AFTER: the entries are relabelled **CL0 - CL8** (and the four cross-references at RULE 0, RULE 3,
INTRUDER ALARM and DECIDING SITUATIONS follow), under a new header paragraph: `THESE LABELS ARE
"CL0" TO "CL8" AND THEY ARE NOT CHOICE NUMBERS ... a card that is not on this window's menu has no
number and cannot be named`, with the table's two recurring strings quoted and the eight-window
cost named. RULE 0's `A CHOICE NAMES A NUMBERED ROW ON THIS WINDOW` paragraph now carries the
count and points here.
**Falsifier for wave 55.** `named_row_reask` + `named_row_not_offered` at deck123: 8 -> 0; replies
naming Bloodline Keeper or Vision Skeins off a menu that does not list them: 8 -> 0.

---

## Compressions that paid for the above (rules unchanged, evidence retained)
| where | why it was safe | bytes |
|---|---|---|
| RULE -1 mulligan comparison | verbatim duplicate of the MULLIGAN section; rung 8/8 this corpus, 12/12 last | 448 |
| RULE 3 "destroys 0" + ENTRY 0 anecdote stacks | superseded by A54-1 and A54-TRIM1 | 346 |
| RULE 0 mana/exile anecdotes (x3) | rung held; two anecdotes made one point | 342 |
| RULE 4 SURVIVES anecdotes (x2) | 1 Slip cast this corpus, on a `{kills: <name>}` row — correct | 336 |
| CHECK 5 / STEP 0 / STEP 2 / CHECK 2 / CHECK 3 combat anecdotes | attackers 8 windows 0 breaks, blockers 4 windows 0 breaks | 841 |
| cast-list entries 3, 4, 5 | rungs held; the Greaves anecdote moved to RULE 6 | 484 |
| repeat-row STOP narrative | the row prints M; stop held at every take | 455 |
| KEY CARDS: Keeper transform, Vision Skeins punisher paragraphs, counter paragraph | two near-duplicate paragraphs merged; punisher list extended to the four cards actually on deck162's board | 254 |
| MULLIGAN two-lander / one-lander / bottoming | replaced with this corpus's own evidence (a 3-land six kept under "would not cover" won on turn 10) | 121 |
| edicts / blockers / Rule 6 prose | rungs held | 405 |
