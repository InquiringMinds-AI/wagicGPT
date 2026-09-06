# deck123 (Intruders of Thraben, WBU token combo) - wave-64 per-deck review

Corpus: matchups-20260906-001533. Six seat files, 321 decision windows, **0 fallbacks, 0
`named_row_reask`, 0 hangs/crashes**. Record 0-6. Hands were real: five keeps at the first look,
one mulligan-to-6 (vs130) that the ask's own header supported; no game in this set was decided at
the mulligan or the bottoming ask.

Files below are named by opponent; all are `.../matchups-20260906-001533/<ts>-ai_baka_deck123-0x...-vs-ai_baka_deck<N>.jsonl`.

## 1. Game by game, and what decided it

| vs | end | turns | what decided it | class |
|---|---|---|---|---|
| 152 | -8 / 25 | 9 | seq 9 (T8, 9 life): took `Cast Bloodline Keeper` over `Cast Damnation ... [<- board sweep: THEIRS 3 / YOURS 0]` with `CRACK-BACK NEXT TURN ... up to 13 ... would KILL you` on the screen. Dead at seq 10 with "no block saves you". | STRATEGY (guide already forbids it) + ENGINE (H3 below encouraged it) |
| 130 | -2 / 20 | 28 | Opponent land destruction (Molten Rain / Lay Waste) took every land; seq 13-21 are hand-size discards with `Land drop: you have no land you could play right now`. Every "Cast nothing" was forced (seq 7/17/24 menus offered only dead rows). | MATCHUP / variance - no guide or engine cause |
| 146 | -1 / 22 | 11 | Combo assembled (Doomsayer seq 20 T6, Alarm seq 22 T8); Doomsayer removed, never replaced. seq 7-18 = 12 consecutive windows answered "Cast nothing right now" on one identical 3-row menu while Devour Flesh was live against their single creature; by seq 24 they had 4 creatures and the edict was dead. | STRATEGY (soft) + interface waste (M3) |
| 162 | -4 / 14 | 15 | **seq 73**: Intruder Alarm resolved in main 1 with Doomsayer and Bloodline Keeper untapped - and both `Create ...` rows were gone from the menu (three Lightning Greaves equips + hold were the whole list). The deck could not fire its combo on the turn it assembled it. | ENGINE (H2) |
| 126 | 0 / 43 | 15 | **seq 48**: 51 creatures, `ATTACK TOTAL: 50 attackers listed, 201 total combat damage ... that KILLS them whatever they block`, every row tagged `[no creature they control can block this attacker]` - answered `ATTACK: none`, citing the Sanguine Bond + Exquisite Blood loop. Dead next turn. | GUIDE (its BRANCH B2 says exactly this) + ENGINE (H1) |
| 125 | 0 / 61 | 45 | Counter/removal grind (Cancel, Essence Scatter, Fall of the Gavel; Alarm countered at seq 96). Board was empty from T14 on. Late discipline: seq 113 and seq 124 cast Damnation into `destroys 0 of their creatures ... 0 of yours`, the same reply writing "Damnation does nothing". Game was already lost by then. | MATCHUP + discipline |

Two of the six losses have a non-variance cause that a fix can move: **162 (engine)** and **126
(guide)**. 152 is a guide-compliance failure with an engine assist. 130, 125 and most of 146 are
matchup and draw.

## 2. Engine / interface items

### HIGH-1 - the attackers screen's life-LOOP warning outranks its own lethal verdict, and is stamped on attackers it says cannot be blocked
`...vs-ai_baka_deck126.jsonl` seq 48 (turn 14). The same prompt carries all three of these:

```
A2-A50. Vampire #1-#49 (4/4) [flying, ...] [no creature they control can block this attacker]
        (their life LOOP is in play: any life they gain or you lose in this combat chains
        without limit - fatal to you, not a trade) x49
They have 4 untapped creatures able to block, but 50 of your 50 possible attackers cannot be
blocked by any of them, so that count applies to the other 0 only.
ATTACK TOTAL: 50 attackers listed, 201 total combat damage to a player - ... At least 201 damage
lands whatever they block - they would be at -178; that KILLS them whatever they block.
```
Reply: `ATTACK: none`. The loop clause is unconditional boilerplate: it is printed on rows the
same render has already computed as unblockable, where no blocking trigger can fire and no life
can be gained. It is also the loudest thing on the screen ("fatal to you", "ANY nonzero payment
... is fatal"), so it beat a lethal verdict two lines below it. Candidate fixes (mine, not the
owner's): (a) suppress the loop clause on rows carrying `[no creature they control can block this
attacker]`; (b) when ATTACK TOTAL is lethal, say so in the loop paragraph - lethal combat damage
is a state-based loss before any of those triggers resolve. **(b) needs engine verification** - I
reasoned it from CR, I did not test whether Wagic checks SBA ahead of the Sanguine Bond trigger.

### HIGH-2 - the two-decline cap retires a free `{T}` token-maker row for the whole turn, and no board change re-opens it
`...vs-ai_baka_deck162.jsonl`, three times in one game:

* seq 66 (T14 Upkeep): rows `1. Create human with Thraben Doomsayer [cost: Tap]`, `2. Create
  vampire with Bloodline Keeper [cost: Tap]` -> `CHOICE: 0 (pass)`
* seq 69 (T14 Draw): same two rows, now each ending `{if you pass here, this option is not
  offered again this turn}` -> `CHOICE: 0 (pass)`
* seq 72 (T14 Main 1): `CHOICE: 4 (Cast Intruder Alarm)` - the combo is now live
* seq 73 (T14 Main 1, priority): battlefield line reads `... of which 4 are creatures, 4 of them
  able to attack right now`, both makers untapped - and the whole menu is
  `1./2./3. Equip with Lightning Greaves targeting ...` + `4. Hold priority`. No Create row.

Same shape at seq 31/33 -> seq 36 (T10) and seq 42/45 -> seq 50 (T12). Mechanism is
`AIPlayerGPT::chooseOrderedAction`, `src/AIPlayerGPT.cpp` ~28896-28904: `declineCap = 2` on a key
of `stripRepeatAnnotation(line)`, reset per turn, `if (declines >= declineCap) continue;`. The
Hold row re-opens on any row change; this cap has no such release, so two passes taken while the
row was worth one body retire it for the window in which it became the deck's win condition.
Candidate fixes (mine): exempt rows whose whole cost is `[cost: Tap]` and that put a permanent on
the battlefield; or clear `mPassDeclineCount` for a row when a permanent enters the battlefield
during that turn (the same event Intruder Alarm keys on).

### HIGH-3 - `{crack-back cover:}` prints a survival verdict built on a number the same prompt calls a FLOOR
`...vs-ai_baka_deck152.jsonl` seq 9 (T8, 9 life). Header: `CRACK-BACK NEXT TURN: 3 of their
creatures will be able to attack ... for up to 13 ... that number is a FLOOR, not a ceiling:
triggers on their board add power before damage - Luminarch Aspirant`. Row 1's cover then says:
`Counting only the checked bodies you cover 6 of 13, leaving 7 -> you would be at 2, which you
SURVIVE - and more blockers can only lower that, so nothing uncounted here overturns it`. Next
turn (seq 10) the real number was 17 and the blockers header read `no block saves you ... best
case with every blocker assigned: you would be at 0`. The cover's closing clause ("nothing
uncounted here overturns it") contradicts the FLOOR caveat that named the growth trigger six
lines above it. Suggest: no survival verdict from a cover when the crack-back line carries a
named FLOOR trigger - or state it as "at their current stats".

### MED-1 - plan carry truncates mid-sentence and can invert a self-correcting one
`...vs-ai_baka_deck146.jsonl` seq 24 -> 25: the model wrote 676 characters, 274 were carried, and
the cut fell at `... I have no creatures and no mana to cast a creature (Thraben Doomsayer costs
{1}{w}{w} but I have no {W} sources untapped?` - the *next* clause was the model correcting
itself. What seq 25/26 carried is a flat false statement of board fact
(`[...the rest of your plan was not carried: 402 further characters, of 676 you wrote]`).
Bigger instance: `...vs-ai_baka_deck126.jsonl` seq 41, `[+5522 bytes written past the PLAN line
trimmed from this record]` - inside that text the model derived the correct answer to the game
("I can attack with all 50 ... They are at 23. I win"); none of it survived to seq 48, which
answered `ATTACK: none`. The interface gives the model no channel for working, so working goes
into PLAN and is destroyed between windows. Suggest: truncate at a sentence boundary, or add a
scratch line that is retained for exactly one window.

### MED-2 - a coded answer inside PLAN prose silently replaces the first-line CHOICE
`...vs-ai_baka_deck126.jsonl` seq 41: `reply` begins `CHOICE: 1 (Create vampire with Lord of
Lineage)`; record carries `coded_answers: 2`, `answer_replaced: true`, `chosen_text: "pass"`,
`choice: 0`. A token activation was lost at 51 creatures because a second coded answer appeared
in 5.5 KB of prose below. Last-answer-wins is documented and the guide warns about it, but paired
with MED-1 it makes stray prose the answer. Suggest: count a coded answer only on the first line,
or refuse-and-re-ask on a second (the repeat row already has that path).

### MED-3 - decline traffic: 200 of 321 windows answered "Cast nothing"/pass, 16 hold takes
Worst run: `...vs-ai_baka_deck146.jsonl` seq 7-18, twelve consecutive windows on one identical
3-row menu (`1. Cast Devour Flesh`, `2. Cast nothing right now`, `3. Hold priority ...`), all
answered `CHOICE: 2`. Each is a model call. Note the annotation the model reads while deciding:
`[hold check: 1 row above is new since the last window at this seam - a row moving is what
re-opens a hold, so a hold taken here lasts only until one moves again]`, which advertises the
hold as short-lived at exactly the seams where it would save the most. Flagged for the engine
seat as a cost driver, not as a defect.

### LOW-1 - land drop declined in main 1, re-offered and taken in main 2
`...vs-ai_baka_deck146.jsonl` seq 23 (`CHOICE: 4 (Play no land right now)`) then seq 25
(`CHOICE: 1 (Play Tundra)`), same turn 10. The re-offer made the Rule-0 violation free. Behaviour
is correct; recorded so it is not mistaken for a lost drop.

### LOW-2 - `x0` on the repeat row is handled cleanly
`...vs-ai_baka_deck126.jsonl` seq 36 and 40: `CHOICE: 2 (Create vampire with Lord of Lineage x0)`
resolved to `chosen_text: pass`, no re-ask, no wasted tick. No engine work needed.

### Card check - no items
Verified against `bin/Res/sets/primitives/mtg.txt`: Thraben Doomsayer (`auto={T}:token(Human,...)`,
mana `{1}{W}{W}`, Fateful hour as `controllerlife < 6`), Bloodline Keeper, Intruder Alarm
(`lord(creature) doesnotuntap` + `@movedTo(creature|...):untap all(creature)`), Pride Guardian,
Sanguine Bond, Exquisite Blood. Scryfall agrees on Thraben Doomsayer (`{T}: Create a 1/1 white
Human creature token.` - the activation is tap-only, the primitive is faithful). Nothing in this
deck's six games is a card-script bug.

## 3. Guide verdict: EDIT

The guide is in good shape - Rule -1, Rule 0, Rule 2 and the mulligan/bottoming sections were
followed and cost nothing this corpus, and Rule 3's numbers were on the screen every time. One
rule in it **lost a game outright**, and one adjacent rule now needs a caveat the engine forced.
Revised guide: `wave64/deck123/strategy.txt`, 70,993 bytes (live file 70,991; band 41-71 KB). All
additions are paid for below; the offsetting cuts are wordiness and one duplicated statement, no
rule removed.

**Edit 1 - CHECK 1, BRANCH B2 (paid by vs126 seq 48).**
before: `So: "ATTACK: none" while both are on the line and any untapped creature they own can gain
them life; keep your bodies home as blockers. The ONE real attack is one no life-gaining body can
block - every untapped creature of theirs tapped or gone, and your total power at or above their
printed life.`
after: gates on **block legality instead of creature count** - `"ATTACK: none" while any untapped
creature they own can BLOCK what you would send ... NO BLOCK, NO GAIN ... When that covers EVERY
attacker you would send and the ATTACK TOTAL says lethal - "that KILLS them whatever they block" -
SEND THEM ALL`, with the 50-tokens/201-damage/43-to-0 cost line.
Why: the old exemption required their creatures "tapped or gone". At seq 48 they were neither -
they simply could not block a flier, which the render had already computed. The loop reasoning
itself is correct and stays (Pride Guardian's `@combat(blocking) ... life:3 controller` into
Sanguine Bond into Exquisite Blood is lethal); only its gate was wrong.

**Edit 2 - BRANCH B's closing sentence (consistency, same seq).**
before: `This is the ONE place in this file where a damage total does override a "do not attack"
rule ...` after: names two places, here and B2's unblockable-lethal line. Without this the file
contradicts itself.

**Edit 3 - DECIDING SITUATIONS, the Sanguine Bond / Exquisite Blood entry (same seq).** Rewritten
to carry the same UNLESS clause, so the quick-reference cannot re-impose the rule the check drops.

**Edit 4 - Rule 1, after the "maker waits untapped until the Alarm has resolved" rule (paid by
vs162 seq 66/69 -> 73, and the same shape at 31/33 -> 36 and 42/45 -> 50).** Added: passing a
`Create ...` row twice in one turn retires it for that turn, the second offer says so on the row,
and the Alarm resolving does not bring it back - so on a turn you mean to cast the Alarm, the
first Create window may be passed and the second may not. This is a **hedge against HIGH-2, not a
fix**: if the engine re-opens the row on a permanent entering, this line becomes harmless.

**Edit 5 - Rule 3, `"DESTROYS 0 OF THEIR CREATURES" IS A CARD THROWN AWAY` (paid by vs125 seq 113,
124).** The N = 0 rule was already stated twice; the two statements are merged into the loud one
and given this corpus's cost line (two Damnations thrown away at N = 0, "Damnation does nothing"
written in the same reply that cast it). No new rule - the model broke one that was already there.

Deliberately **not** changed: Rule 3's sweeper-priority rule (vs152 seq 9 violated it while it was
correct as written - `at M of 0, K of 2 or more, with YOUR life at 10 or less, "hold" is not an
available answer` - and the row said `THEIRS 3 / YOURS 0`; the render's false survival promise is
HIGH-3, not a guide gap); the hold-row rule (already explicit, ignored 12 windows running in the
146 game - a compliance problem, and a fourth restatement is not the fix); the mulligan and
bottoming sections (nothing went wrong at either ask).

## 4. Proposals

None filed. One candidate I judged worth naming but not filing: the B2 failure generalises to
"write the gate on the quantity the screen computes (can it block? is this lethal?), never on the
name of a permanent that is present" - but I did not read all 331 amendments of
`wave59/strategy-writing-skill.md`, so I cannot tell whether that restates an existing one, and a
restatement is noise. The general guide is not loaded at runtime, so nothing here was worth a
general-guide proposal either.

## 5. What I did NOT check

Opponent seat files (read only for the deck123 games' context, and not decision-by-decision); the
falsifiable predictions in `wave64/lane-{AG,AH,AI,AJ,AK}.md` (engine seat's job - I adjudicated
none of them); the `game-*.stderr` logs; latency/deadline fields; whether Wagic actually applies
state-based actions ahead of the Sanguine Bond trigger (HIGH-1(b) is reasoned from the rules, not
tested); the 5,522 bytes trimmed past the PLAN line at 126 seq 41 (not in the record, so MED-2's
second coded answer is inferred from `coded_answers: 2` + `answer_replaced: true`); the strategy-
writing skill's amendment list; decks other than 123.
