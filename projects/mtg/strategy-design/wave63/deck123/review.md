# wave-63 per-deck review — deck123 "Intruders Of Thraben" (WBU Intruder Alarm token combo)

Seat logs: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-191148/1788653*-ai_baka_deck123-0x*.jsonl`
(6 games, all natural, binary 98d13050f). Record **2-4**. Opponent seat logs read for context.
Counts below are from the rendered `prompt`, not the `options` array.

## 1. Game by game, and the decisions that decided them

| opponent | result | turn | end life | the decision |
|---|---|---|---|---|
| 130 | **WIN** | 37 | 2 / -67 | clean grind; chain fired t35, transformed to Lord of Lineage (s87), 18 Vampires for lethal s95 |
| 126 | **WIN** | 14 | 18 / -68 | Alarm t6, Keeper t12, repeat-N row s29, transform s34, 25 Vampires s45 |
| 152 | LOSS | 13 | -3 / 21 | **s30** and **s31** — below |
| 162 | LOSS | 13 | -8 / 21 | **s32** — sent the lone Doomsayer into 2 blockers off a 26-creature board |
| 146 | LOSS | 17 | -2 / 21 | **s11** and **s15** — both token-makers spent as blockers, on the engine's own suggestion |
| 125 | LOSS | 31 | 0 / 40 | **s54** — 19 tokens into Lightmine Field for **0** damage; never rebuilt after |

The two wins and the four losses split cleanly on one axis: **whether the deck still owned an
untapped token-maker on the turn after the combo assembled.** In all four losses the maker was
gone by then; in both wins it was not. Hands were real and comparable (all six games kept at the
first look — 6 looks, 6 keeps, no mulligan, no bottom ask), so this is not a variance split.

**vs152, s30 (attackers, t12).** Board: Intruder Alarm + Thraben Doomsayer + 26 Humans (25 made
that turn, summoning-sick). Only A1 Doomsayer and A2 Human #1 could attack. A1's own row read
`[their untapped blockers: Elite Spellbinder (4/2) (both die); ...]` and the header read
`Their 4 untapped blockers can cover every attacker you could send, so none of that damage is
guaranteed.` The model wrote `ATTACK: A1, A2` with the plan "Next turn: Attack with all 30
creatures ... to win". Opponent blocked both; both died; Doomsayer, the engine, was gone.
STRATEGY — the live guide's CHECK 5 already forbids exactly this ("do not send a LONE Thraben
Doomsayer ... into a board that can block and kill it. Its own row says when: '(both die)'").

**vs152, s31 (blockers, t13).** Header: `you would be at -3; this KILLS you (of that, 4 from 1
attacker none of your creatures can block) - at least 4 of that lands whatever you block ... you
would be at 1 AT BEST (no assignment of your blockers does better)`. Twenty-six B-rows on the
screen (`B1-B26. Human #1-#26`). The model answered `BLOCKS: none` and died at -3, having
re-derived the arithmetic itself in the reply ("We take 8 damage, go to -3, and lose"). Blocking
A2 and A3 survives at 1 with 26 blockers and an army that wins the next turn. PERCEPTION-shaped
STRATEGY: the surface printed the survivable verdict; the model read only the `this KILLS you`
headline that precedes it. See HIGH-2.

**vs146, s11 and s15 (blockers, t9 / t11).** See HIGH-1. The engine named
`Bloodline Keeper blocks Silverquill Silencer` (14 life, header `NOT lethal`) and
`Thraben Doomsayer blocks Nadaar, Selfless Paladin` (12 life, header `NOT lethal`, that B-row
reading `your blocker dies, attacker lives`). The model copied both verbatim, losing both makers
while Intruder Alarm was already on the battlefield. From s16 on it held a live Alarm and an
empty board, cast Damnation twice into `0 of yours`, and lost.

**vs125, s54 (attackers, t18).** 19 Humans, opponent at 26, `They have 0 untapped creatures able
to block`, and the render's own warning: `Both figures are BEFORE their attack triggers:
Lightmine Field fires on the declaration, before any combat damage, and can kill your attackers
first`. The model wrote `ATTACK: A1-A19` and reasoned in the reply that it accepted the trade.
Outcome: **all 19 died and the opponent took 0** (opp_life 26 at s54, 26 at s55/s56, still 26 at
s57 Main phase 2). STRATEGY, against the guide's existing CHECK 2, which was right.

**vs162, s32 (attackers, t12).** 26 creatures, only Doomsayer could attack; `Their 2 untapped
blockers can cover every attacker you could send`. Sent it anyway. Dead on t13 (the 24 Humans
had no flying and legally could not block two Masters of the Feast — verified against the 162
seat log s16; the missing blockers ask at t13 is correct, not a defect).

## 2. Engine / interface / card items

**HIGH-1 — the blockers header's named assignment is followed as an instruction, and on a
not-lethal board it recommends spending the deck's only win condition.**
Repro: `1788653544-ai_baka_deck123-0x55f4f228e1a0-vs-ai_baka_deck146.jsonl` s15, t11, life 12:

    INCOMING THIS COMBAT: 2 attackers, 4 unblocked damage - you would be at 8 (your creatures can
    legally block at most 1 of those 2 attackers at once, so at least 1 of them go unblocked) -
    best case with every blocker assigned: you would be at 11 - one legal assignment that reaches
    it, chosen for your blockers' material as well as for the life: Thraben Doomsayer blocks
    Nadaar, Selfless Paladin; 1 of those blockers die
    ...
    Your life: 12. Unblocked, these attackers deal up to 4 - you would be at 8 - NOT lethal:
    block only where the trade favors you.
    B1. Thraben Doomsayer (2/2) ... - may block A1 (your blocker dies, attacker lives) ...,
        A2 (you kill it, your blocker lives)

Three things stack here. (a) The suggestion is a **life-maximiser**; "chosen for your blockers'
material as well as for the life" oversells it — material only breaks ties *within* the max-life
set, so at s15 it named the assignment that loses the blocker for nothing (`your blocker dies,
attacker lives`) over the one that kills the Goblin and keeps it (`you kill it, your blocker
lives`), because the first saves 3 life and the second saves 1. (b) It renders on **NOT lethal**
headers, two lines above the header's own "block only where the trade favors you" — a
recommendation contradicting the instruction beside it. (c) The model treats it as an
instruction: **3 renders in this deck's 6 games, 3 copied verbatim** (s11 and s15 vs146, s52
vs130), and the two on not-lethal boards each spent the only token-maker under a live Intruder
Alarm. This is the trust doctrine working exactly as designed and being pointed the wrong way:
the surface owes the model truth, and here it hands out advice its own header disowns.
Suggested direction (mechanism, not the owner's position): suppress the named assignment when
the header says NOT lethal, or rank material ahead of life once the header is not lethal, and
drop the "as well as for the life" claim unless material actually gated the choice.

**HIGH-2 — at a lethal blockers header the survivable verdict is printed last, after the
"this KILLS you" headline, and gets read as confirmation of death.**
Repro: `...-vs-ai_baka_deck152.jsonl` s31, t13, life 5:

    INCOMING THIS COMBAT: 3 attackers, 8 unblocked damage - you would be at -3; this KILLS you
    (of that, 4 from 1 attacker none of your creatures can block) - at least 4 of that lands
    whatever you block (trample/menace counted as unblocked): you would be at 1 AT BEST (no
    assignment of your blockers does better)

`at 1 AT BEST` is the answer to "does blocking save me", and it is TRUE and correct here — but it
arrives after `this KILLS you`, after a second death figure, and phrased as a ceiling ("AT BEST",
"no assignment does better") rather than as survival. With 26 legal blockers printed the model
answered `BLOCKS: none`. A clause that states the outcome affirmatively and first — e.g.
`BLOCKING SURVIVES: at least one assignment leaves you at 1` ahead of the `this KILLS you`
clause, or suppressing `this KILLS you` when the AT-BEST figure is above 0 — removes the whole
misread. Note the two clauses are literally contradictory as read left-to-right: one says the
combat kills you, the other says it does not.

**MED-3 — the hold row is re-opened by the model's own token loop, and the casting seam is
re-asked once per activation.**
Repro: `...-vs-ai_baka_deck125.jsonl` turn 16 — the `Casting decision (Main phase 1, YOUR turn)`
ask is put to the seat **19 times in that one main phase** (s28, 30, 31, 33, 35, 36, 37, 39, 40,
41 and on), interleaved one-for-one with the Doomsayer activations at s29/s34/s38. The seat takes
`Hold priority` at s30 and is asked the identical question at s31 under
`[hold check: 1 row above is new since the last window at this seam - a row moving is what
re-opens a hold ...]`. The "new row" is the same casting menu with its mana annotation moved
(`leaves N of your M untapped mana sources untapped`) because a token entered and untapped the
board. Across the six games: **80 `Hold priority` takes, 41 of them (51%) at a (turn, seam) where
a hold had already been taken**, and **162 of 293 ask+priority decisions (55%) resolved to a
no-op** (`pass` / `Cast nothing` / `Hold`). Median latency 18.7 s, p90 71 s. The repeat-N row
exists precisely to batch the loop; the casting seam re-opening between iterations undoes that
saving. Suggested direction: exclude mana-availability annotations and the activation counters
from the hold-key row comparison, or do not re-open the casting seam while the only board delta
since the last window is the effect of an activation the seat took at the neighbouring seam.

**MED-4 — one 900-second wall miss.**
`...-vs-ai_baka_deck130.jsonl` s11, t7, `kind=wall_miss`, `fallback=wall_miss_unrecorded_wall`,
`latency_ms=900027`, `deadline_pct=100.0`,
`transport=curl=28,http=0,empty=1,connect_ms=20000,phase=wall`. Recovered at s12
(`recovery`, `recovers_seq=11`) and the seat answered the attackers ask normally at s13, so it
cost no decision — but a 20 s connect plus a 15 min wall on one decision is the only transport
event in this deck's 293 windows and belongs in the census.

**LOW-5 — Lightmine Field's script fires a step later than its text says.**
`bin/Res/sets/primitives/mtg.txt:67059`
`auto=@each blockers:foreach(creature[attacking]|Battlefield) damage:1 all(creature[attacking]|Battlefield)`
— the trigger is the **declare-blockers** step; the printed and Oracle text is "Whenever one or
more creatures attack" (an attack trigger, declare-attackers step). Scryfall agrees with the
`text=` line. The attackers render says "fires on the declaration". Both land before combat
damage so the s54 outcome (19 attackers dead, 0 damage) is the same either way; the divergence
would matter to anything that acts between the two steps.

**Not a defect, checked and cleared** (recording so it is not re-opened): the missing blockers
ask on vs162 t13 (both attackers had flying, no Human had flying or reach — 162 seat s16); the
Human tokens rendering `(1/1)` at vs152 s31 while Fateful hour would make them 3/3 (Thraben
Doomsayer died in the s30 combat — 152 seat's t12 blockers reply `BLOCKS: B1:A1, B2:A2`, row
`(both die)`); the `CRACK-BACK NEXT TURN` clause appearing on only 4 of 11 attackers asks (the
other 7 all had `0 of them without a restriction against attacking` on the opponent's line — the
clause is complete); the `repeat_count_reask` refusal at vs126 s28 (the count-without-a-PLAN
guard firing correctly, answered on the re-ask at s29).

## 3. Guide verdict — **EDIT**

The live guide is already right about nearly everything this corpus punished: CHECK 5's
"one maker stays home", CHECK 2's Lightmine Field rule, STEP 1/STEP 2's "makers block last" —
each was violated, and each already carries a prior-corpus receipt. Adding more prose to teaches
the model is overriding would be noise. Four edits are made, and they are the four places where
the guide is **out of step with what the render now prints** (three) or where its own conditional
was read as unconditional (one). Revised guide: `wave63/deck123/strategy.txt`, **70991 bytes**
(live: 70995), ASCII, LF, all 14 sections intact.

**Edit 1 — COMBAT / CHECK 0: read the printed CRACK-BACK number, do not add it up.**
before: "Nothing on your own turn prints this number for you. ... Add it up yourself, here, every
attack window."
after: "THE WINDOW PRINTS THE NUMBER FOR YOU whenever they have a body that can swing:
`CRACK-BACK NEXT TURN: N of their creatures will be able to attack ... you would be at X or lower
- and that number is a FLOOR, not a ceiling`. READ X, do not re-derive it. ... NO CRACK-BACK line
means no creature of theirs can attack next turn: the check is free, go on."
Paid by: the clause is live and correct on every window where it can be — vs152 s26 prints it in
full; 4 of 11 attackers asks carry it and the other 7 have no attack-capable opposing creature.
Teaching DIY arithmetic over a true surface is the failure mode the trust doctrine names.

**Edit 2 — COMBAT: the `AT BEST` figure is the verdict at a lethal blockers header.**
before: "AND WHEN THE HEADER SAYS 'this KILLS you' AND NO ASSIGNMENT GETS YOU ABOVE 0,
'BLOCKS: none' IS THE RIGHT ANSWER. ... subtract the largest 'deals N' you can legally block from
the header's total, and if the result still reaches your life, decline."
after: names the `you would be at N AT BEST (no assignment of your blockers does better)` clause
as the decider — N of 1 or more is BLOCK, decline only when the clause is absent or N is 0 or
lower — and carries the receipt.
Paid by: **s31**, vs152 (guide's own condition satisfied in the other direction; the model
declined a survivable block at 5 life with 26 blockers and lost the game). Also removes the
second DIY-subtraction instruction.

**Edit 3 — COMBAT / new STEP 0b: the header's named assignment is a life-maximiser, not advice.**
new text: take the named assignment only when the header also says LETHAL / "this KILLS you";
on a "NOT lethal" header ignore the name and go on to STEP 1.
Paid by: **s11 and s15**, vs146 (3 of 3 suggestions copied verbatim; the two not-lethal ones each
spent the only maker under a live Intruder Alarm).

**Edit 4 — DECIDING SITUATIONS: two lookup rows for edits 2 and 3.**
`- The blockers header says "this KILLS you" and also prints "you would be at N AT BEST": N of 1
or more is BLOCK ...` and `- The blockers header says "NOT lethal" and names an assignment ...
whose X is Thraben Doomsayer, Bloodline Keeper or Lord of Lineage: do not copy that line.`

**Byte budget** (the pool ceiling is 71000; every pool guide sits at 70962-70999): the additions
are paid for by compressing RULE -1 (6 of 6 first-look keeps this corpus, no mulligan,
no bottom ask, no game decided there; the wave-62 guide reports twelve of twelve before that), the "instants on the opponent's turn" phase enumeration, CHECK 1 BRANCH B2,
CHECK 4, and three prior-corpus narratives — operative content preserved in every case, only
restatement removed.

## 4. Proposals

`wave63/deck123/general-proposals.md` — one item (the survivable-lethal-header decline), scoped
honestly as low-volume. No skill proposals: HIGH-1 and HIGH-2 are instances of the existing
perception-routing and trust doctrine, and a skill amendment restating them would be noise.

## 5. What I did NOT check

- I did not open the two hung games (152v162, 152v130) or their reruns — deck123 is in neither.
- I did not read the deck125/126/130/146/152/162 seat logs beyond the four lookups named above
  (152's t12 blockers, 162's t12-13, and the results.tsv rows).
- I did not adjudicate any lane-AB/AC/AD/AE/AF prediction — that is the engine seat's job. HIGH-1
  bears on lane AB's E3 (survivable-block suggestion ranks blocker material) and HIGH-2 on the
  same family; I report the evidence, not the verdict.
- I did not run wagic, build anything, or touch `bin/Res`, `src/` or git.
- I did not verify HIGH-1's suggestion-line generator against the source — the claim that life
  ranks ahead of material is inferred from the two s11/s15 renders plus their B-rows, not read
  off the code.
- Latency, fallback and transport figures above cover this deck's 6 seats only; the corpus-wide
  census is the engine seat's.
