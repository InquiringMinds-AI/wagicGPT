# Wave-63 deck-130 review (mono-red land destruction)

Seat files: `matchups-20260905-191148/*deck130-*.jsonl` (5 natural + 1 hung) and the rerun
`matchups-20260905-215345/1788663228-ai_baka_deck130-0x5558e5494a20-vs-ai_baka_deck152.jsonl`.
Record: **2-4** (beat 146 and 126; lost to 162, 125, 123 and the 152 rerun).
Binary 98d13050f for the five, 0026a9d87 for the rerun.

## 1. Game by game, and the decisions that decided them

**vs 146 - WON 20 to 0, turn 18.** Clean. Blastminer sent turn 6/8 into "(neither dies)" tags
(seq 12, 17 - the wave-62 floor fix holding), Siege-Gang turn 12, then the sacrifice engine at
seq 47/49/51/52 finished from 13. No misplay found.

**vs 126 - WON 20 to -4, turn 22.** Correct WALL CHECK at seq 29: the model reasoned out loud
that attacking the Goblins into Pride Guardian / Wall of Omens only gains them life and cut them
from its own attack, ending on `ATTACK: A4` (Rorix alone). Starstorm declined at seq 24 and taken
at seq 62 when the THEIRS list named two bodies. No misplay found.

**vs 123 - LOST -67 to 2, turn 37.** The seat built a working Hammer of Bogardan lock (seq 76-87:
return in upkeep, recast in main 1, four consecutive turns, opponent 14 -> 11 -> 8 -> 5 -> 2) and
lost by ONE turn to a 20-Vampire Intruder Alarm board. Seq 88 (the last decision, opponent at 2)
was forced: 2 untapped sources, Hammer needs 3, Lay Waste 4, board empty - the only rows were a
cycle and a hold. **No guide-attributable error in this game;** it is a race the deck lost on
tempo, not on decisions.

**vs 162 - LOST -5 to 20, turn 15. Two decisions decided it, both guide-attributable.**
- Seq 28-47: the carried plan reads *"I will hold priority through main phase to avoid drawing"*
  and the seat took the hold row at ten consecutive windows (28, 32, 33, 34, 35, 36, 37, 41, 44,
  47) while its own draw steps under Underworld Dreams + two Fate Unravelers took it 16 -> 1. The
  turn-based draw step is mandatory; no row skips it. The belief is a misread of rule #0's
  "Pass instead" (written for optional draws) plus the hold row's "do not ask me again".
- **Seq 43 (turn 14, 1 life) is the game.** Row 2 read
  `Cast Siege-Gang Commander {3}{r}{r} (2/2) {leaves 0 of your 5 untapped mana sources untapped ...}`
  with the hand line marking it `[castable now]`, under
  `CRACK-BACK NEXT TURN: 2 of their creatures ... for up to 6 - you would be at -5; that would KILL you`.
  The seat answered `CHOICE: 4 (Cast nothing right now)` and wrote *"I cannot cast Siege-Gang
  Commander because it enters the battlefield, which triggers the draw punishers"*. Verified
  against the primitive (`mtg.txt:105071-105080`: `auto=_GOBLINTOKEN_*3`, no draw anywhere): the
  Commander draws nothing. Commander + three Goblins is four blockers against two attackers; it
  survives the turn it died on.

**vs 125 - LOST -3 to 77, turn 50.** Rorix (seq 44/46) took them to 12 before Final Judgment;
after that the seat held Hammer of Bogardan in hand from turn 22 to turn 50 and cast nothing at
all from turn 30 onward (seq 67, 78, 83, 93, 95, 97, 99, 103, 109 all "Cast nothing"), at
10 -> 17 untapped sources, while the opponent's Elixir engine went 17 -> 77. The replies are
guide-conformant: "THE RELEASE IS NOT A LICENCE AGAINST A LIFEGAIN ENGINE" tells it three a turn
from a recurring Hammer does not beat five from an Elixir, and it repeats that verbatim. Against
Emrakul-plus-Elixir the game was likely unwinnable from turn 30; **I am not charging a guide edit
to it** - a 3/turn clock genuinely loses to +11/turn, and inventing a "cast something anyway"
rule would contradict a rule the corpus paid for last wave.

**vs 152 (rerun) - LOST -4 to 13, turn 25.** Correct Starstorms at seq 24 (X=4, three kills) and
seq 30, correct Hammer at Luminarch Aspirant at seq 42/43. **Seq 45 is the game.** At 3 life,
under `CRACK-BACK NEXT TURN: 1 of their creatures will be able to attack ... for up to 4 from
combat as their board stands - you would be at -1 or lower; that would KILL you`, the seat
declared `ATTACK: A1, A2` - both its Goblins, its only two blockers. Goblins have no vigilance,
so both were tapped and the 4/5 Briarbridge Tracker connected for lethal. One Goblin held home
chumps it and the game continues. The guide's floor exception did not fire because it is written
on **printed** power: the Tracker prints 2/3 (`borderline.txt:13996-14005`) and carried two +1/+1
counters from Ranger Class, so printed 2 < life 3 while the real number was 4. Note the render was
correct throughout - `(4/5) (printed 2/3) [vigilance] [counters: 2x +1/+1]` - this is a guide bug,
not an engine one.

## 2. Engine / interface / card items

**HIGH-1 - the draw-punisher block prices the draw but never says the draw step is compulsory,
and the hold row reads as if it were the way out.** File
`matchups-20260905-191148/1788653515-ai_baka_deck130-0x556cfb48b140-vs-ai_baka_deck162.jsonl`,
seq 35 (turn 12, life 10). The frame renders:
`DRAW PUNISHERS on the battlefield: theirs - Underworld Dreams, Fate Unraveler #1, Fate Unraveler #2. Every card YOU draw costs you 3 life to theirs. ... Count that cost before choosing to draw.`
and `DRAW FORECAST: your next draw step draws 1 card = 1 x 3 = 3 life LOST BY YOU ... you would be at 7.`
"before choosing to draw" is an invitation to look for the row that declines, and the only row on
that screen that sounds like one is
`3. Hold priority: pass now, and do not ask me again - this turn or later ...`. The seat took it
at ten straight windows under the plan "hold priority through main phase to avoid drawing" and
drew every turn regardless. Fix: on the FORECAST line, state that the draw step is mandatory and
that no row on any menu prevents it (the punisher sentence can keep "choosing to draw" for the
optional draws, which are the ones a row does control). This is the same shape as D2/E1 - a
number given without the fact that bounds it - and it cost this seat a whole game.

**HIGH-2 - a cast row that adds blockers is priced only against mana, never against the
crack-back the same frame flags as lethal.** Same file, seq 43 (turn 14, life 1). The frame prints
`CRACK-BACK NEXT TURN: 2 of their creatures will be able to attack ... for up to 6 - you would be
at -5; that would KILL you` and, five lines below, `2. Cast Siege-Gang Commander {3}{r}{r} (2/2)
{leaves 0 of your 5 untapped mana sources untapped - casting this taps you out} ... {card text:
"When Siege-Gang Commander enters, put three 1/1 red Goblin creature tokens onto the battlefield.`.
Nothing on the row connects the four bodies it makes to the "would KILL you" line above it. The
cast row already computes the mana it leaves and the card text it will fire; on a crack-back
flagged lethal it could also say how much of that total the resulting bodies can absorb
(e.g. `{their crack-back above is 6 across 2 attackers; this adds 4 blockers}`). The seat instead
confabulated a rule to justify passing.

**MED-3 - the attackers window never says that attacking removes those creatures from the
crack-back the same window prints.** Rerun file, seq 45. The window prints
`CRACK-BACK NEXT TURN: ... for up to 4 ... that would KILL you` and, at the bottom,
`Declaring attackers taps only the attacking creatures - never your lands.` It says taps and stops
there. The crack-back figure is identical whether the seat attacks with 0, 1 or 2 Goblins, so no
number on the screen moves when the choice that kills the seat is made. A conditional
(`each attacker you declare is one fewer blocker for the crack-back above`), or a crack-back figure
recomputed against the untapped remainder, would have made the cost visible. Vigilance attackers
are the exception the clause has to name.

**MED-4 - a hold survives a transient stack clause badly: the rows return byte-identical and the
window is asked again.** File `...deck130-0x562d1d6ffa10-vs-ai_baka_deck125.jsonl`, seqs 72/73/74,
all turn 33, all `ask`, all the same seam. Seq 72's rows are held (`chosen_text` = the hold row).
Seq 73's Spark Spray row differs from seq 72's by exactly one inserted clause -
`[this cannot target the spell on the stack - battlefield permanents only]` - which correctly
re-opens the hold under lane AD's per-seam retirement. Seq 74's `options_text` is then **byte-identical
to seq 72's**, and the window is asked a third time. Lane AD erases the seam entry when the rows
move, so a return to a previously held row set does not re-arm. Either remember the held set per
seam so an identical return re-arms it, or exclude stack-contents clauses from `holdKeyRow` the way
the phase clause already is (#W63-AD). One occurrence in this deck's corpus; cheap, and it is the
same class of promise-breaking the E10 item exists for.

**MED-5 - `latched_line_in_plan` is written, but neither `plan_answer_line_ignored` nor
`plan_answer_line_only` appears anywhere in this deck's 462 records, so which path E6(b) took
cannot be read off the record.** File `...deck130-0x5644d5b59430-vs-ai_baka_deck126.jsonl`, seq 29
(`attackers`, turn 16): `answer_replaced: true`, `latched_line: "ATTACK: A4"`,
`latched_line_in_plan: true`, `parse_note: "attack_last_line_taken"`, `latched_coded_line: null`.
The reply opens `ATTACK: A2, A3, A4`, then a `PLAN:` paragraph, and the model's real deliberation
(a correct WALL CHECK) happens INSIDE that paragraph and ends `ATTACK: A4` - the right answer, and
the one the engine took. So the in-plan exclusion would have discarded the correct answer here had
it applied at this seam. The engine got it right; the record cannot say why. Worth the two stamps
firing (or a third saying "seam not bounded") on every record carrying `latched_line_in_plan`.

**LOW-6 - the land-drop ask is offered with no cost line and gets declined for free.** Same 125
file, seq 47 (turn 22): `1. Play Mountain / 2. Play no land right now`, the seat answered 2 while
holding the Mountain and at 8 lands, and answered 2 again at seq 36 of the rerun. Harmless in both
games, but the row says "playing a land costs no mana and uses up no cast" and the model still
declines; a one-clause reason why a decline is ever right (there is essentially none for this deck)
would close it.

Verified against primitives: Siege-Gang Commander (`mtg.txt:105071`), Hammer of Bogardan
(`mtg.txt:52046`), Briarbridge Tracker (`borderline.txt:13996`). All three render faithfully in
the prompts; no card-script defect found for this deck this wave.

**Nothing for the Vita softlock report.** The hung 152v130 seat's last record
(`...deck130-0x55dc3471b9e0-vs-ai_baka_deck152.jsonl`, seq 26) is a Stone Rain target ask in main
phase 1, not a Blockers window, so this deck's corpus does not corroborate
`softlock-blockers-1788650768.md`.

**Deck-level observations that the engine seat should fold into its census, not conclusions:**
across the seven files the hold row rendered in 297 windows and was TAKEN in 102 (against wave-62's
corpus-wide 6 of 459), and E7's decline row is present on 7 of 9 ANNOUNCE_X asks with the other two
carrying the new "costs are ALREADY PAID" header instead - both consistent with lanes AD passing,
but the counts of record are the engine seat's.

## 3. Guide verdict: **EDIT**

Revised guide at `wave63/deck130/strategy.txt` (70,953 B, from the live 70,999 B - inside the
41-71 KB band; the added material is paid for by trimming restatement, listed at the end).

**Edit 1 - rule #0 gains the fact that bounds it.** Paid for by 162 seqs 28/32-37/41/44/47.
- before: rule #0 ended at *"There is no answer worth finding at the cost of the game you are
  still in."*
- after: a new paragraph, `THE DRAW STEP IS NOT ONE OF THE DRAWS YOU CAN REFUSE`, scoping "DO NOT
  DRAW AT ALL" to the optional draws, naming that the draw step is mandatory and that no row -
  including the Hold priority row, whose text promises only that the same QUESTION is not re-asked -
  skips it, quoting the failure verbatim, and giving the replacement action (spend the mana on the
  board that is charging you).

**Edit 2 - draw punishers punish draws only.** Paid for by 162 seq 43.
- before: nothing in the guide said what a draw punisher does NOT trigger on.
- after: `DRAW PUNISHERS PUNISH DRAWS AND NOTHING ELSE`, listing casting / land drops / attacking /
  blocking / activating / tokens entering as free, noting `[castable now]` is not a draw, and
  quoting seq 43's own sentence and its cost.

**Edit 3 - the combat numbers come off the crack-back line, not off printed power.** Paid for by
the rerun seq 45.
- before (RACE CHECK): *"THEIR turns to kill you = your "Your life:" number divided by the total
  printed power of their untapped creatures"*.
- after: divided by the `CRACK-BACK NEXT TURN` line's own "for up to N", with the reason (it already
  counts counters, lords and vigilance).
- before (carve-out): *"if their untapped creatures' total printed power is more than your life
  PLUS ..."* -> after: the crack-back line's figure.
- before (floor exception): *"ONE EXCEPTION INSIDE THE FLOOR: if their untapped creatures' printed
  power totals your life or more, send only Rorix this turn and keep the Goblins home."*
- after: the exception is stated to OUTRANK "send every creature listed", is keyed to the
  crack-back line saying "that would KILL you", explains that attacking taps the blocker and that
  the crack-back figure does not know what you are about to declare, covers a Goblins-only list
  (the old wording only had an action for Rorix), and carries seq 45's numbers.
- the floor's *"not to 'hold blockers'"* sentence was amended to defer to this exception so the two
  no longer contradict.

**Edit 4 - DECIDING SITUATIONS.** The two old draw bullets were folded into one keyed to the
DRAW PUNISHERS line's N, and three bullets added: delete any plan sentence treating the draw step
as optional; a `[castable now]` creature or artifact under a punisher line is not a draw, cast it,
especially when the crack-back says "would KILL you".

**Trims that paid for the additions** (restatement only, no rule removed): the Starstorm cycling
entry stated the X=0 lesson four times, now once; "decide WHETHER at the cast row, not on the X
menu" appeared four times, now twice; duplicate token-maker anecdote in cast-order entry 1;
duplicate one-lander rule in MULLIGAN; the Cleanup-discard anecdote; four other anecdotes shortened
without dropping their lesson.

## 4. What I did NOT check
- I did not read the opponents' seat files except where the deck-130 prompt's own GAME LOG carried
  the fact; every opponent action cited above comes from deck 130's rendered log, not from the
  other seat's translog.
- I did not adjudicate any lane prediction. The E7 and hold counts above are deck-scoped
  observations offered to the engine seat, not verdicts; I ran no prefix-match census of HOLD
  renders/takes to the brief's definition, and no `async_drops` / `transport` / timeout census.
- I did not check whether E1/E3/E4's combat clauses appear at all in this deck's windows - deck 130
  faced almost no life-loop or gang-block boards, and 3 of its 6 games had zero or one blockers ask.
- I did not test the revised guide against the engine; it is not compiled, parsed or run, and I did
  not build, run wagic, or touch `bin/Res`, `src/` or git.
- I did not verify the 125-game verdict ("likely unwinnable from turn 30") by counting an alternate
  Hammer-lock line to lethal against the Elixir; I judged it not worth a guide rule and left it.
- The Briarbridge Tracker 4/5 was reconciled against the primitive and Ranger Class's counter
  trigger by arithmetic (printed 2/3 + 2 counters), not against Scryfall.
