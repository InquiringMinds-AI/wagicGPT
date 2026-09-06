# wave-64 deck130 (mono-red land destruction) — per-deck review

Corpus `matchups-20260906-001533`. My seat's six files (all cited by the `deck130-...-vs-<opp>` name):

| file (deck130 seat) | seq |
|---|---|
| `1788671736-ai_baka_deck130-0x55878e8960d0-vs-ai_baka_deck123.jsonl` | 0-62 |
| `1788671746-ai_baka_deck130-0x561b1c2c3b40-vs-ai_baka_deck125.jsonl` | 0-117 |
| `1788671748-ai_baka_deck130-0x5598d3ac9430-vs-ai_baka_deck126.jsonl` | 0-83 |
| `1788671760-ai_baka_deck130-0x558212a36ae0-vs-ai_baka_deck152.jsonl` | 0-28 |
| `1788671762-ai_baka_deck130-0x55b6e6fcb1a0-vs-ai_baka_deck146.jsonl` | 0-86 |
| `1788671766-ai_baka_deck130-0x557db95d3140-vs-ai_baka_deck162.jsonl` | 0-64 |

Seat census: 424 decisions, 0 fallbacks answered wrong, 1 `wall_miss` (recovered), 5 `async_drops`,
12 `answer_replaced`, 0 records with `deadline_pct > 100`, 0 `transport_error`, 0 `reveal_stall_forced`,
0 `commit_retracted`, 0 `plan_answer_line_ignored`, 0 `latched_line_in_plan: true`.
Record 3-3.

## 1. Game by game

**WIN vs123, 20 to -2, turn 28.** Clean. No creature ever resolved, so no attackers window in the
whole game; the seat won on Hammer of Bogardan recursion plus burn. Deciding line: seq 55 took the
upkeep return, seq 60/61 cast it at the face on the row tagged
`{right now: takes 3 damage - they would be at -2; THIS WINS THE GAME}`. The lethal tag was correct
and was obeyed. One wasted turn (seq 48/49, Stone Rain on Scrubland with the opponent at 3) cost
nothing.

**WIN vs125, 20 to 0, turn 30.** Dwarven Blastminer attacked in 9 of 9 attackers windows and carried
the whole clock (20 -> 8 by turn 28) while the Blastminer ability ate the lands. One 900 s wall
miss at seq 8 (`transport: curl=28,http=0,empty=1,connect_ms=20000,phase=wall`,
`chosen_text: <refused: wall_miss_unrecorded_wall>`, `choice: -1`), recovered at seq 9; no game
effect. seq 101 is a two-block reply (item HIGH-1) that flipped a face-burn into a cycle; the game
was already won.

**LOSS vs126, 0 to 42, turn 25.** Not a misplay loss. The opponent assembled
Sanguine Bond + Exquisite Blood; from that point every path is lethal and the render said so
truthfully. At the last blockers window (seq 82) the header read `INCOMING THIS COMBAT: 1 attacker,
1 unblocked damage - you would be at 19 ... NO best-case life figure is given: both halves of their
life LOOP are in play ... Declining every block does not escape it either`, and the seat answered
`BLOCKS: none` for exactly the stated reason. The header is internally consistent and does not
oversell (contrast wave-63 F9). The real loss window was earlier: five `no attackers` answers
(seq 35, 45, 65, 73, 81) into an opponent board of six 0/4 defenders — attacking was in fact bad,
and the seat's own plan named the Perimeter Captain blocking trigger. What this game *does* expose
is item MED-1 (six identical windows at turn 19).

**LOSS vs152, -5 to 18, turn 13.** Variance, not guide effect. Deck130 resolved zero creatures in
the game; from turn 10 its hand was four land-destruction spells and nothing else (seq 19 hand:
`Stone Rain x2; Lay Waste; Molten Rain`), against a Ranger Class Wolf that grew to 6/6. At seq 23
(life 5) the `CRACK-BACK NEXT TURN ... you would be at -4 or lower; that would KILL you` line was
present and correct, and the seat had no card that answered it. No engine item; I flag only the
confabulated plan at seq 26 ("Stone Rain targets Elite Spellbinder ... kill the draw punisher") —
Stone Rain destroys lands, Elite Spellbinder is not a legal target and is not a punisher, and the
menu directly above said so. Cost nothing here.

**WIN vs146, 4 to 0, turn 40.** Long grind won on Goblins. F4's planeswalker rows worked: seq 70
printed `W1. Kaya the Inexorable [planeswalker] [2 loyalty left ...]` with the `A#>W#` grammar and
the honest caveat that walker damage does not reduce life. seq 47 is the clearest instance of
HIGH-1. 5 `async_drops` (seq 26, 43, 44, 53) with no visible effect on play.

**LOSS vs162, 0 to 20, turn 12.** The deciding thread is strategic and is what my guide edit is
paid by. Opponent landed Howling Mine (t3), Fate Unraveler (t7), Fate Unraveler (t9), Howling Mine
#2 — a compulsory 6 life per draw step by turn 12. The seat's `#0 RULE` behaviour was correct: it
never took an optional draw once the punisher line was up, and the F10 fix rendered
(`Your DRAW STEP is COMPULSORY: no row on this or any other menu declines it`) in all 41 windows
that carried a punisher line. It lost anyway because it never deployed:
- seq 27 (turn 8, life 20) and seq 34 (turn 10, life 15) spent `{2}{R}` on Blastminer land kills.
  seq 34 in particular left 1 untapped source, so Siege-Gang Commander — in the opening hand from
  seq 2 — was uncastable that turn, and `Siege-Gang Commander` was **never cast in the game**
  (0 occurrences of `You cast Siege-Gang Commander` in the file).
- seq 29/30/31 fired the seat's only instant, Spark Spray, at Fate Unraveler #1 on a row that read
  `{right now: takes 1 damage - SURVIVES (toughness 4)}`, with the cast row already saying
  `{kills whichever you target: THEIRS - none ...}`. The surface was true and complete; the guide
  told it to do this (see §3 edit 2).

Card facts verified against the primitives: Spark Spray (`mtg.txt:110142`, `auto=damage:1`,
`target=anytarget`), Fate Unraveler (`mtg.txt:39182`, `power=3 toughness=4`,
`auto=@drawfoeof(player):damage:1 opponent`), Dwarven Blastminer (`mtg.txt:33251`,
`auto={2}{R}{T}:destroy target(land[-basic])`). All three match Oracle; no engine/card discrepancy.

## 2. Engine / interface items

### HIGH-1 — a two-block reply has its ANSWER taken from block 2 and its PLAN harvested from block 1
The reply protocol says `On the FIRST line write CHOICE:`. When the model instead emits a
deliberation block and then a second, corrected `CHOICE:` at the end, the engine takes the LAST
`CHOICE` (correct — it is the model's conclusion) but the plan it latches and re-prints next window
is the FIRST block's `PLAN:` — the scratchpad that argued for the choice that was *not* taken. The
record has everything needed to notice this and does not use it: `plan_block_end < latched_line_offset`
in every instance.

Repro (`...deck130-...-vs-ai_baka_deck146.jsonl` seq 47, turn 26, life 11 vs 9):
- record: `answer_replaced: true`, `coded_answers: 2`, `latched_line: "CHOICE: 3 (Draw 1 with Pyrite Spellbomb)"`,
  `latched_line_offset: 2467`, `plan_block_start: 71`, `plan_block_end: 2042`, `post_answer_overrun: 2436`.
- block 1 read `CHOICE: 1 (Deal 2 damage with Pyrite Spellbomb targeting the opponent)`; block 2 read
  `CHOICE: 3 (Draw 1 with Pyrite Spellbomb)`. Engine took 3.
- the very next prompt (seq 48) then rendered:
  `YOUR PLAN (as you last stated it, 1 window ago on turn 24): Opponent at 9 life. 2 damage is not
  lethal. No creatures on board to target. Rule #2 Step 0b: Starstorm not in hand. ... [...the rest of
  your plan was not carried: 1585 further characters, of 1964 you wrote ...]` — block 1's raw
  reasoning, truncated mid-argument, arguing for the row the engine did not take. It persisted
  through seq 49 as well.

Same shape, all with `plan_block_end < latched_line_offset`:
`...vs-deck162` seq 33 (plan 35-2950, answer at 3692; a short, correct second `PLAN:` sat at
offset 3733 and was discarded), `...vs-deck125` seq 101 (plan 74-2123, answer at 3098; block 1 said
`CHOICE: 3 (Deal 2 damage ... targeting the opponent)`, block 2 said `CHOICE: 11 (cycling with Spark
Spray)` — the flip changed the action class), `...vs-deck126` seq 74/75, `...vs-deck146` seq 27/69/79,
`...vs-deck123` seq 48. 11 two-`CHOICE` replies over 424 decisions in my seat (2.6%); 12
`answer_replaced` records.

Concrete fix shape: harvest the plan from the SAME block the latched answer came from — i.e. when
`latched_line_offset > plan_block_end`, re-scan for a `PLAN:` after `latched_line_offset` and prefer
it; when there is none, carry no plan forward rather than the contradicted one. Recording a
`plan_block_from_dropped_answer` flag would make it auditable either way.

### HIGH-2 — the number and the name in a `CHOICE:` line can disagree, and the name silently wins
`...deck130-...-vs-ai_baka_deck162.jsonl` seq 57 (turn 12, life 4). Both `CHOICE:` lines in the
reply say `0`: `CHOICE: 0 (pass)` and `CHOICE: 0 (Hold priority)`. The row list has
`0 = pass priority` and row `4. Hold priority: pass now ...`. The record shows `choice: 4` — the
engine resolved on the *name*, not the number, and the translog stores a number neither `CHOICE`
line contains. Nothing in the prompt tells the model that the parenthetical name can override the
number, and nothing in the record marks that it did. Benign here (the game was already lost to a
lethal compulsory draw step), but on any live window `pass` and a turn-long `Hold` latch are
materially different answers. Fix: record which field was authoritative (`choice_from: number|name`)
and, on a number/name conflict, prefer the NUMBER or refuse — the prompt asks for the number first.

### MED-1 — six byte-identical Casting decisions in one turn, across six rendered phases
`...deck130-...-vs-ai_baka_deck126.jsonl` seq 53-58 (turn 19). Identical three-row menus
(`1. Cast Starstorm {r}{r}{x} ...`, `2. Cast nothing right now`, `3. Hold priority: ...`) rendered
at, in order, `Combat begins`, `Attackers`, `Combat ends`, `Main phase 2`, `End`, `Cleanup`. The
prompt's own counter escalated truthfully across them —
`[you declined this exact list 2 times already this turn]` ... `... 7 times already this turn` — and
the hold check read `every row above was also on the menu at the last window at this seam (8 windows
in a row now)`. The engine kept the E10 promise: the rows genuinely did not move, and the collapse
row was offered every time. **This is a seat problem, not an engine bug** — the seat answered row 2
(`Cast nothing right now`, one window only) rather than row 3. Seat-wide: 297 windows offered a Hold
row, 75 were answered with it, and **127 were answered with a non-latching decline**
(`Cast nothing right now` / `pass` / `Play no land right now`). Same-turn identical-row re-asks:
62 across 37 groups of my 424 decisions (14.6%). I record it here because the render already carries
the counter that would fix it and nobody reads it; if the engine wanted to help, the escalation could
name the row that ends it ("row 3 answers all of these") rather than only counting.

### MED-2 — a land-destruction cast row lists the seat's own lands in its target list unflagged
`...vs-ai_baka_deck152.jsonl` seq 26: `1. Cast Stone Rain {2}{r} ... {kills whichever you target:
THEIRS - Boulderloft Pathway, Branchloft Pathway, Hengegate Pathway, Deserted Beach; YOURS -
Mountain #1, Mountain #2, Mountain #3, Mountain #4, Mountain #5, Mountain #6}`. Six of the ten legal
targets are the seat's own lands; the `YOURS -` half is the only marker and it reads identically to
the `YOURS -` half of a damage row, where it means "this kills your creature". Compare the Blastminer
ability rows, which carry an explicit `[opponent's battlefield]` / `[your battlefield]` bracket per
target and which the guide can and does key on. LOW impact in this corpus (the seat never mis-targeted
its own land), but it is a self-destruct row one number away from the right one.

### LOW-1 — `wall_miss` at the 900 s wall recurs
`...vs-ai_baka_deck125.jsonl` seq 8: `latency_ms: 900022`, `deadline_pct: 100.0`,
`transport: curl=28,http=0,empty=1,connect_ms=20000,phase=wall`, `fallback: wall_miss_unrecorded_wall`.
Recovered at seq 9 (`kind: recovery`, `recovers_seq: 8`). Same signature and same 1-per-corpus rate
as wave 63; no `transport_error` retries anywhere in my seat.

### Lane predictions my seat can speak to (counts are mine only; the engine seat adjudicates)
- **F2 (`kNoPassRowFact`) — PASS in my seat, 47 of 47.** Every window carrying
  `this ask has no pass row` genuinely had no decline row (all mulligan / target-picking asks).
  Last wave this string was false on 680 windows.
- **F10 (compulsory draw step) — PASS, 41 of 41.** Every window with a `DRAW PUNISHERS on the
  battlefield:` line also carried `Your DRAW STEP is COMPULSORY: no row on this or any other menu
  declines it`, and the `DRAW FORECAST:` repeated it. The seat never again wrote "hold priority to
  avoid drawing" (0 occurrences).
- **F4 (planeswalker attack rows) — PASS.** `...vs-deck146` seq 70 rendered `W1.` plus the `A#>W#`
  grammar and the "damage sent at a planeswalker does NOT reduce their life total" caveat.
- **F13 (latch byte offsets) — PASS, 12 of 12.** Every `answer_replaced` record carries
  `latched_line_offset`, `plan_block_start`, `plan_block_end`, `latched_coded_line`, and
  `latched_line_in_plan` (false in all 12 — all were second top-level answers, correctly classified).
  The offsets are what let me prove HIGH-1.
- **F11 (`CRACK-BACK COST OF ATTACKING`) — UNTESTED in my seat, 0 of 20 attackers windows.** The
  emitter (`AIPlayerGPT.cpp:38480`) is gated on `life - cbTotal <= 0`; no attackers window of mine
  had a lethal crack-back, so the gate never opened. Not a defect — no window arose.
  `{crack-back cover:` likewise 0 of 424 (no body-adding row under a printed crack-back line).
- **F1/F5/F6/F7/F8/F9/F12/F14 — UNTESTED here.** My seat had 6 `GANG BLOCK` renders but exactly one
  blockers window in six games (`...vs-deck126` seq 82), and no modal `choice` row, no bounce/destroy
  seam, no `targetedplayer`-granted ability, no reveal wait.

## 3. Guide verdict: **EDIT**

Written to `wave64/deck130/strategy.txt` (started from the live
`bin/Res/ai/baka/deck130_strategy.txt`). 71,227 bytes, from 70,953 — **+274 bytes**, inside the
41-71 KB pool band (69.6 KiB; the live pool clusters at 70,98x-70,99x, so if the band is being
applied as a hard 71,000-byte ceiling this file is 227 bytes over and the synthesis seat should say
so). Every insertion is paid for below; two of the five edits are net-negative trims of prose the
edits made redundant.

**Edit 1 — the hold-row rule quoted a literal that is on no screen.**
before (one entry in the trigger list):
> `- The last row of the menu - on your own turn as well as theirs - reads "Hold priority for the`
> `  rest of this turn: pass now, and do not ask me again unless the board changes (any change`
> `  re-opens this window; you give up no cast) {taking this row skips the rest of this turn's`
> `  identical windows}" -> take THAT row, at the FIRST such ask of the turn, not after four`
> `  declines. It is the same decision said once for the whole turn ...`

after: quotes the live prefix — `"Hold priority: pass now, and do not ask me again - this turn or
later - until one of the rows above changes"` — says to quote no more than that, and adds the
contrast the seat is actually getting wrong: `"Cast nothing right now" answers ONE window and the
same list returns at the next seam; the hold row answers all of them`.

Paid by: `Hold priority for the rest of this turn` appears **0 times in any prompt in the entire
21-game corpus**, and appears in exactly one place — `...deck130-...-vs-ai_baka_deck125.jsonl`
seq 69, in the seat's own **reply**, where the model copied the guide's dead string verbatim as its
answer's short name: `CHOICE: 3 (Hold priority for the rest of this turn: pass now, and do not ask
me again unless the board changes ...)`. Also paid by 127 of 297 hold-offering windows answered with
a non-latching decline, and by MED-1's seq 53-58. This is the same defect class as wave-63's E13
`converters on your battlefield:` dead literal.

**Edit 2 — the Spark Spray rule ordered a cast at any opponent creature, with no toughness gate,
and contradicted the guide's own SURVIVES rule.**
before (trigger list): `- "Choose an option for Spark Spray:" appears and its target list holds an
opponent creature -> "Cast Card Normally", then aim at that creature.`
after: `... holds an opponent creature whose row reads "takes 1 damage - DIES" -> "Cast Card
Normally", then aim at that creature. Every row says "SURVIVES" -> cycle; 1 into a 3/4 spends the
card and changes nothing.`
The same unconditioned form in the cycling table (`Spark Spray -> cycle it when ... shows no
opponent creature. If it shows one, cast it instead (rule #2).`) got the same gate, and its
now-duplicated example paragraph was compressed (net -210 bytes there).

Paid by: `...vs-ai_baka_deck162.jsonl` seq 29/30/31 — the seat's only instant fired at
`Fate Unraveler (3/4) ... {right now: takes 1 damage - SURVIVES (toughness 4)}` off a cast row
already reading `{kills whichever you target: THEIRS - none ...}`. The guide's own rule at the
other end of the file (`A damage row ... reads "{right now: takes N damage - SURVIVES ...}" -> not
that row this window`) says the opposite; the seat followed the Spark Spray entry, which is the more
specific one. This is a guide-internal contradiction, not a perception failure — the surface was
right.

**Edit 3 — new precedence line: a body outranks the Blastminer activation under a draw-punisher clock.**
after (added directly under the existing Blastminer entry):
`- A "DRAW PUNISHERS on the battlefield:" line is up AND a body is in "Your hand" -> the Blastminer
activation is OFF this turn, at any life and land count: their lands are not what is killing you,
and {2}{R} on a land is the {2}{R} the body needed. Body first, activate with the rest.`
plus a one-line `Exception: the DRAW PUNISHERS entry below outranks this one.` on the neighbouring
"cast the land destruction now" trigger, so the two do not fight.

Paid by: `...vs-ai_baka_deck162.jsonl` seq 34 — at life 15 with 4 untapped sources and two Fate
Unravelers forecasting 4 a turn, the seat spent 3 of 4 on `Destroy with Dwarven Blastminer targeting
Drowned Catacomb`, leaving Siege-Gang Commander (held since the opening hand at seq 2) uncastable;
seq 27 is the same shape at turn 8. `You cast Siege-Gang Commander` occurs 0 times in that file.
The existing `#0 RULE` already says "kill the punisher, or put down the biggest body you have" but
gates it on "a forecast that is a number you cannot pay" — at 15 life the forecast *was* payable, so
the rule stayed silent while the land-destruction rule (their board showed 4 lands, life above 8)
fired. The new line closes that gap without touching either rule's own conditions.

**Edit 4 — dead-literal repair.** `"Put in Hand with Hammer of Bogardan [cost: {2}{r}{r}{r}]"` ->
`"Put a card into hand with Hammer of Bogardan [cost: {2}{r}{r}{r}]"`, which is the live string and
is what the guide already quotes correctly in three other places (lines 96, 460, 723). Found by
diffing every quoted literal in the guide against the whole corpus; net -20 bytes with the
surrounding sentence tightened.

Not changed, and deliberately so: the `#0 RULE` draw-punisher block (it worked — 0 optional draws
taken under a punisher line across six games), the life-loop / converter material (the vs126 loss
vindicated it: the seat read the loop correctly and declined every block for the stated reason), the
mulligan and bottoming table, and the Starstorm X rules (never a losing X in this corpus).

## 4. Proposals
None. Nothing I found generalises past deck130 with evidence I can pay for: HIGH-1 and HIGH-2 are
engine items for the engine seat, MED-1 is the seat ignoring a counter the render already prints
correctly, and edits 1/2/4 are deck-local literal and precedence repairs. I am not proposing a
general-guide or skill amendment on a 2.6% two-block rate seen in one seat.

## 5. What I did NOT check
- I read the opponent seat's file only for `results.tsv` outcomes and never traced an opponent's
  decisions; a misplay by an opponent that handed deck130 a win would not appear here.
- I did not read `wave64/lane-{AG,AH,AI,AJ,AK}.md` or `codex-review.md` line by line — only the
  brief's summary of them plus the emitters I grepped in `src/AIPlayerGPT.cpp` for F11. My
  lane verdicts in §2 are counts from my six files only and are not corpus-wide adjudications.
- I did not read `wave59/strategy-writing-skill.md` (I am proposing no skill amendment, so a
  restatement check was not needed).
- I did not verify the Scryfall side for any card — the three primitives I leaned on
  (Spark Spray, Fate Unraveler, Dwarven Blastminer) matched Oracle text as printed in
  `mtg.txt`, so no disagreement arose to report. I did not verify Howling Mine, Sanguine Bond,
  Exquisite Blood or Perimeter Captain against their primitives; my claims about them are read off
  the rendered `{effect: ...}` text in the prompts, not off `mtg.txt`.
- I did not build or run wagic, and did not check whether my revised guide parses/loads.
- HOLD census is prefix-matched on `chosen_text` per the brief; I did not separately audit whether
  any hold was honoured or silently broken (wave-63's MED-4 transient-clause item) — my seat's
  62 same-turn re-asks all followed rows the `[hold check:]` line declared changed, except the
  seq 53-58 run, where the seat had not taken the hold row at all.
