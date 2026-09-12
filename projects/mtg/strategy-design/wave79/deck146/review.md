# Wave 79 - deck 146 (Orzhov dungeon midrange) per-deck review

Seat files: `matchups-20260912-000748-final/*ai_baka_deck146-0x*-vs-*.jsonl` (6 games, single binary
`wagic-5af6f7a95-w79step1`). Live guide reviewed = the wave-78 edition installed at a37b2d5ce
(19,997 B). Opponent seats read for board and log context only.

Seat census: **427 model decisions** (ask 296, priority 91, attackers 35, blockers 2, bottom 2,
reveal 1) plus 1 `forced_close`. **0 fallbacks of any kind** - no `unparsed_reply`, `empty_reply`,
`reply_truncated`, `action_before_plan`, `defer` or `recovery` on this seat; the corpus's single
`reveal`/`engine_answered` fallback is not mine, so there is nothing to quote. `thinking` on and
native reasoning on 427/427 (median 5,118 chars, max 21,069 - the corpus max). Latency p50 72.6 s,
p95 189.7 s, max 436.7 s. `off_protocol_bytes>0` 42/427 (9.8 %), `plan_line_missing` 45/427
(10.5 %), `protocol_deviation` compliant 382 / `unlabelled_plan` 42 / `plan_absent` 3 /
`answer_label_absent` **0**. Max prompt 23,712 B; 97 of 427 prompts over 20 KB (22.7 %; wave 78:
26,923 max and 17.6 %). Gameend sums for this seat: `ask_replays_reserved` 93 /
`identical_ask_answers_reserved` **0**, `ask_key_continuation_differs` 12, `hold_windows_skipped`
207 (cast 188 / priority 19), `hold_released_turn` 12, `hold_verdict_safer_ignored` 2,
`hold_reopened_new_threat` 0, `crossphase_identical_reputs` 10 / `crossphase_board_unchanged` **0**
(fourth wave at zero), `main_phase_windows_skipped` 6, `own_turn_windows_skipped` 44,
`mana_only_windows_skipped` 0, `async_drops` 4, `forced_close_events` 1 / `unrecorded` 0,
`phase2_answer_recovered` 1 / missing 0, `blocker_forecast_rows` 32 (multi 12, gang 10, collapsed
0), `identical_option_asks_resolved` 5, `reserve_decline_windows_noted` 5,
`plan_names_stranded_card` 0, `plan_names_uncastable_zone_card` 0, `menu_pass_no_progress` 0.
**Zero renders/records on this seat of `stop_reached_*`, `stack_drain_windows_asked`,
`own_loop_windows_asked` / `own_loop_verdict_lines_rendered`, and the `AT BEST` header face** - all
UNTESTED here, not passed.

| # | Opponent | Result | Turn | Final life | What decided it |
|---|---|---|---|---|---|
| 1 | 152 | **LOSS** | 15 | -3 / 23 | **seq 21** - at 11 life with an EMPTY board the seat cast Acererak (no body) over Silverquill Command's live "return creature" on a mv-2 creature in its own graveyard; dead two turns later |
| 2 | 162 | **LOSS** | 16 | 0 / 13 | 2x Fate Unraveler + Underworld Dreams = 3 life per COMPULSORY draw step; one Verse for three punishers - unanswerable from that draw |
| 3 | 126 | **WIN** | 14 | 21 / -3 | Silencer on curve, Kaya -3 on Sanguine Bond (seq 38), full alpha strike at seq 56 on the "That IS lethal" line |
| 4 | 130 | **WIN** | 16 | 21 / 0 | Nadaar + two Silencers beat down; **seq 41 nearly threw it** - lethal on board, guide rule quoted, one attacker sent |
| 5 | 123 | **WIN** | 26 | 20 / -1 | Lolth Spiders + Hive animation ground out the Intruder Alarm deck; legend-rule reset at seq 66-67 taken correctly |
| 6 | 125 | **WIN** | 40 | 16 / -2 | 41-turn grind: Hive animated under CHECK 0b every turn, Kaya -3 on Lightmine Field (seq 172), Command held |

**4-2** (wave 78: 4-2). Hands are real; both losses carry one decision item each and the 130 win
carries the wave's most alarming one.

## 0. Wave-78 edits: did each pay?

- **Edit 1 (LETHAL is the FIRST check in COMBAT) - FIRED 4 times, PASSED 3, FAILED 1, and the
  failure names the missing fact.** PASSED: `146v126` seq 56 (opponent at 2, "That IS lethal",
  ATTACK: all four), `146v125` seq 226 (at 7, all four), `146v123` seq 115 (at 2, the Hive).
  **FAILED at `146v130` seq 41** - opponent on 1, `ATTACK TOTAL: 3 attackers listed, 12 total
  combat damage ... That IS lethal`, three untapped Goblins. The seat quoted the rule verbatim in
  its reasoning and then overrode it:
  > *"'LETHAL IS THE FIRST CHECK... SEND IT ALL.' But here, sending all risks being blocked.
  > ... If I attack with Nadaar (4/4), they can block with 1 Goblin (1/1). **Nadaar survives, deals
  > 4 damage. Opponent dies.** ... If I attack with A1, A2, A3, they can block all 3. I deal 0."*

  Both halves of that are wrong in the same way: a BLOCKED attacker with no trample deals the
  player ZERO, so sending one into three blockers guarantees nothing, and sending three is the only
  line that can get damage through at all. The rule was read; the arithmetic under it was not in
  the guide. Edit 1 below.
- **Edit 2 (ONE BLOCKER PER ATTACKER + the header-figure rule) - UNTESTED.** Only 2 blockers
  windows this wave (`146v162` seqs 29, 37), neither with a trample or menace attacker and neither
  with an `AT BEST` face. Both were answered correctly, but neither exercises the rule the edit
  was written for.
- **Edit 3 (the 3-life condition (b) in a printed number) - FIRED 3/3 and PASSED 3/3.** Every
  Emeria window (`146v152` seq 5, `146v162` seq 9, `146v125` seq 37) quotes the new wording back
  verbatim in its reasoning, all three are at 20 life with NO CRACK-BACK line printed, so (b) is
  satisfiable as written and the payment is right each time. Wave 78's failure case - a payment
  taken against a printed crack-back - did not recur.
- **T5 (the `AT BEST` false floor, the gang-block trample spill, the two-disagreeing-verdicts
  header) - the three wave-78 shapes did NOT recur, and none of them is TESTED here.** `AT BEST`
  renders **0** times in 427 prompts (wave 78: 2), `no assignment` 0, `LOSES THE GAME` 0. Both
  wave-79 blockers headers are the new named-assignment face and both name a TRUE assignment
  (verified by hand below). Only 3 real `GANG BLOCK:` clauses rendered, all on THEIR blockers and
  none on a trample attacker, so the spill clause never had a state to render in: **UNTESTED, not
  passed.**
- **T16 (`[doesn't untap ...]` reading as a tap STATE) - CLOSED.** 48 untap-restriction keywords on
  this seat, and the affirmative `[untapped]` face renders 32 times alongside them - e.g. `146v123`
  seq 94: `Spider #1 (2/1) [reach, doesn't untap during its controller's untap step, menace
  (can't be blocked except by two or more creatures)] [untapped]`. The 1,300-character
  confabulation of wave 78 has no counterpart this wave.
- **T3/CZ + DC F1 (the CG shape) - PASSES on this seat, cleanly.** Across all 427 prompts there are
  **0** same-turn, same-seam re-asks whose body is identical after stripping `{...}` and `[...]`
  groups (wave 78 corpus-wide: 135 + 98). `identical_ask_answers_reserved` is 0 against 93
  `ask_replays_reserved`.
- **Wave-77 Kaya -3 ladder - still paying, 2/2.** `146v126` seq 38 (-3 on `Sanguine Bond
  [opponent's battlefield]`, over four `{this hits YOUR permanent}` rows) and `146v125` seq 172
  (-3 on `Lightmine Field`) are both rung 2, "a noncreature permanent of theirs making cards,
  damage or life every turn". No token, no defender and no own permanent exiled in six games. The
  new `{this hits YOUR permanent}` tag on self-targeting rows is a clear render improvement.
- **S12 (`Opponent life trend` gained/lost split) and S14 (`{counter cost:}`) - hold.** Every
  loyalty row across the six games prints the counter price and every figure matches the
  `[counters: Nx loyalty]` on the same screen (spot-checked at `146v126` seq 38 and `146v125`
  seq 172).

## 1. Game by game - the deciding decisions

**vs 152 (LOSS, T15) - decided at `...vs-ai_baka_deck152.jsonl` seq 21 (T13 Main 1, 11 life vs 23,
empty board).** Hand: 2x Acererak, 2x Silverquill Command; 5 untapped sources; `Your battlefield
(6 permanents listed, of which 0 are creatures ...)`; `CRACK-BACK NEXT TURN: 3 of their creatures
will be able to attack ... for up to 7 ... you control NO creature, so every point of that is
unblocked`. Graveyard: `Silverquill Silencer {b}{w} (creature 3/2)` - mana value 2. Row 2's own
clause reads `{modes live right now: creature gains 3/3 and return creature, ... return creature
and you draw, ... ; dead (no legal object right now): none}` - "return creature" is LIVE and the
only body it can return is that 3/2. The seat took row 1, Acererak, reasoning:
> *"Let's verify if there's any reason to cast Silverquill Command instead. **No targets for modes
> really**, and Acererak ventures which is crucial."*

That is false on the face of the row, and it inverts the guide's own order: entry 5 (Command with a
live mode you want) sits three entries ABOVE entry 8 (Acererak). The line taken produced no body,
no blocker and no pressure; 11 -> 4 on the crack-back, dead at T15. **STRATEGY -> guide, Edit 2.**
The same inversion repeats at seq 23 and seq 25, where `Cast Silverquill Silencer` (entry 4, a real
3/2) was on the menu and Acererak (entry 8) was taken both times - by then the game was already
lost, so those two cost nothing, but three windows in one game is the pattern.

**vs 162 (LOSS, T16) - not a decision, a punisher clock.** From T13 the opponent's line carried
`DRAW PUNISHERS on the battlefield: theirs - Fate Unraveler #1, Underworld Dreams, Fate Unraveler
#2. Every card YOU draw costs you 3 life`, with `DRAW FORECAST: your next draw step draws 1 card =
1 x 3 = 3 life LOST BY YOU ... This draw step is COMPULSORY`. The seat had exactly one Vanishing
Verse left, spent it correctly on `Underworld Dreams` (seq 30-31), and still faced 2 per turn with
no removal and a 3-power board against 13 life. Its two blockers windows were both answered right
(below). Unanswerable. Two items fell out of it: **MED 1** (the combat header's lethality verdict
ignores the compulsory punisher damage the same prompt forecasts) and the second half of **HIGH 1**.

**vs 130 (WIN, T16) - won, with the wave's worst decision at seq 41.** Covered under Edit 1 above.
The opponent declined to block and the game ended; the seat was one opposing block away from giving
back a board it had dominated since T7 at 20 life to 1.

**vs 126 (WIN, T14), vs 123 (WIN, T26), vs 125 (WIN, T40) - clean.** 126: Silencer T3, Kaya cast
and -3'd the same turn at seq 37-38, full alpha at seq 56 off the lethal line. 123: the Intruder
Alarm deck never assembled; Hive animated under CHECK 0b at seqs 31/36/42/107, Lolth -3 twice for
Spiders, the legend-rule ask at seq 66-67 binned the spent copy as the guide says. 125: a 41-turn
grind won on the Hive clock, with `Cast nothing right now` / `Hold priority` taken correctly for
roughly 60 consecutive windows while Command and Soul Shatter sat dead.

## 2. Engine / interface / card items

### HIGH 1 - `{crack-back cover:}` counts a body the row does NOT leave on the battlefield
Repro `146v152` seq 21, row 1 (and `146v152` seq 25, `146v162` seqs 34 and 36 - 4 windows):
`1. Cast Acererak the Archlich {2}{b} (5/5) ... {card text: "When Acererak the Archlich enters, if
you have not completed the dungeon Tomb of Annihilation, return Acererak the Archlich to its
owner's hand and venture into the dungeon. ..."} {crack-back cover: ... This adds 1 body - a
creature that arrives this turn CAN block on their turn ... Counting only the checked bodies you
cover 2 of 7, leaving 5 -> you would be at 6.}`
Acererak's printed ETB, quoted in the SAME row, returns him to hand. He adds zero bodies and covers
zero damage; the clause's "you would be at 6" is off by the whole cover. The model caught it at
`146v152` seq 23 (*"casting Acererak now does NOT give me a 5/5 blocker this turn. He bounces to
hand"*) only because the guide states it - nothing in the render does.
**Same clause, second false shape:** `146v162` seq 32, row 1 is `Cast Silverquill Command`, a
SORCERY whose own live/dead clause on that row reads `HALF DEAD (one half of the pair has no legal
object ...): creature gains 3/3 and return creature ...; dead (no legal object right now): return
creature and you draw, return creature and opponent draws` - the graveyard held no creature card of
mana value 2 or less - and the crack-back cover on the same row still says `This adds 1 body ... you
cover 3 of 6, leaving 3 -> you would be at 1`. Two clauses on one row contradict each other.
Fix direction: the cover may count a row only when taking it leaves a creature on the battlefield -
a self-bouncing ETB, and a modal spell whose body-making mode the same row marks dead or half-dead,
add nothing. Pin both boards.

### HIGH 2 - the combat/crack-back "you SURVIVE" figures are TRUE, and the model does not believe them
Repro `146v162` seq 35, `{crack-back cover, STAY HOME: keeping all 3 of them back covers 6 of that
6, leaving 0 -> you would be at 1, which you SURVIVE}` (three 1/1s against two 3/4 attackers - true,
because a blocked non-trampler deals the player nothing), and `146v162` seq 37,
`best case with every blocker assigned: you would be at 1 ... Triumphant Adventurer blocks Fate
Unraveler #2; Skeleton Mad #1 blocks Fate Unraveler #1` (also true). The model spent roughly 2,000
reasoning characters on each fighting the arithmetic:
> *"If I block with two creatures, I take 3 damage and lose. ... **Why does it say 'you would be at
> 1, which you SURVIVE'?** Maybe my blockers have enough toughness or something? No, blockers just
> block. If I block 2 attackers with 2 blockers, I take 6 damage. I die."*

It followed the render both times and lived. The mirror-image error at `146v130` seq 41 (*"Nadaar
survives, deals 4 damage. Opponent dies"* about a BLOCKED attacker) shows the same missing fact
costing a decision. The render is not wrong here - this is an item because the cover and header
figures are the engine's highest-value output and the seat is one line of arithmetic away from
discarding them. Cheapest engine-side fix: name the mechanism in the clause once
(`... covers 6 of that 6 - a blocked attacker without trample deals you nothing`), the same way the
lone-blocker parentheticals already name deathtouch and first strike.

### MED 1 - a blockers header prints "NOT lethal" while the same prompt forecasts compulsory lethal damage
Repro `146v162` seq 29 (T14, 4 life): the header reads `INCOMING THIS COMBAT: 1 attacker, 3
unblocked damage - you would be at 1` and `Your life: 4. Unblocked, these attackers deal up to 3 -
you would be at 1 - NOT lethal: block only where the trade favors you.` Fourteen lines above it the
same prompt reads `DRAW FORECAST: your next draw step draws 1 card = 1 x 3 = 3 life LOST BY YOU ...
This draw step is COMPULSORY - no row on any menu declines it.` Declining every block puts the seat
at 1 and the compulsory draw then kills it; the combat verdict says the situation does not require
a block. The seat blocked for an unrelated reason (the guide's chump rung fires at 10 life or less)
and survived. Fix: fold the forecast the engine has already computed into the lethality verdict, or
qualify it (`NOT lethal from combat - the DRAW FORECAST above takes N more before you act again`).

### MED 2 - the live/dead mode clause names the MODE but never the OBJECT
Repro `146v152` seq 21, row 2: `{modes live right now: ... return creature and you draw, ...; dead
(no legal object right now): none}`. Soul Shatter's row on the very same screen names its object
(`{right now: they sacrifice Luminarch Aspirant (MV 2, their highest)}`); Vanishing Verse names its
(`- legal targets right now: Master of the Feast ...`). Command names none, so "return creature is
live" cannot be priced without the model cross-reading the graveyard line and applying the mana
value filter by hand - which is exactly what it declined to do (*"No targets for modes really"*),
at the decision that lost the game. Fix: print the object the way the other two rows do -
`return creature (right now: Silverquill Silencer (3/2) from your graveyard)`.

### MED 3 - the `Hold priority` row's own paragraph is 590 bytes and renders on nearly every ask
Repro any cast window, e.g. `146v152` seq 21 rows 3-4 plus the `[HOW A HOLD ENDS: ...]` block: the
row text, the hold-check bracket and the HOW-A-HOLD-ENDS block together are ~1.4 KB of every prompt,
unchanged window to window. With 97 of this seat's 427 prompts over 20 KB and a 16/12 KB narration
trim already in force, this is the largest fixed-cost block in the prompt that is not board state.
It could be stated once in the system text rather than per window.

### LOW 1 - `[castable now]` on a hand card the engine already knows is dead
Repro `146v125` seqs 6, 7, 8, 11, 14, 15, 17, 22: `Soul Shatter {2}{b} [instant] [castable now]`
in the hand header across eight windows whose opponent line reads `(... of which 0 are creatures
...)`. Same item as wave 78; the CAST ROW carries the truth, the hand header carries only
payability.

### LOW 2 - reasoning termination drift on a decided window
Repro `146v152` seq 21: after reaching its answer the model writes ~1,300 further characters of
`Done.` / `Output matches.` / `Proceeds.` / `[Output Generation] -> *Proceeds*`. It appears on
several windows of this seat and is pure decode cost (reasoning median 5,118 chars, p95 latency
189.7 s). Not a render defect; worth a line in the engine seat's latency section.

### LOW 3 - `146v152` is 26 records to 15 turns while `146v125` is 184 to 41
The 125 game is 43 % of this seat's whole decode budget. `hold_windows_skipped` 207 shows the
engine is already collapsing most of the static residue; the rest is the Hive animation re-opening
its seam every turn. Same item as wave 78.

## 3. Guide verdict: EDIT

Live 19,997 B -> revised **19,998 B** (`wave79/deck146/strategy.txt`). Two rules added, no rule and
no condition removed; every added byte is paid by the compression listed under Edit 3, so neither
new rule had to take bytes from an old one.

**Edit 1 - COMBAT gains the blocked-damage fact as its own rule, stated symmetrically, and the
LETHAL bullet closes the "send fewer" escape (A333: the bullet declares its own scope; A334: flat
statement, no concessive tail).**
- before: the fact existed only in the BLOCK LADDER, in the defender's voice and gated on a life
  threshold - `ONE BLOCKER STOPS ALL of a non-trampler's damage: a blocked non-trampler deals you
  ZERO, so "I take the damage either way" and "preserve it for future blocks" are false at N of 10
  or less.` - and the LETHAL bullet ended at `-> SEND IT ALL.`
- after, as the FIRST bullet of COMBAT, above LETHAL:
  `- BLOCKED DAMAGE IS ZERO, BOTH WAYS: a blocked creature without trample deals the PLAYER nothing
  at any size - your 1/1 in front of their 5/5 stops all 5, a cover line counting N of your bodies
  against N of their attackers is TRUE, and sending fewer attackers than their untapped blockers
  guarantees nothing.`
  and the LETHAL bullet now reads `-> SEND EVERY BODY LISTED, never a chosen few.` The BLOCK
  LADDER's copy shrinks to `Blocked damage is ZERO (see COMBAT), so "I take the damage either
  way" ... are false at N of 10 or less.`
- paid for by **`146v130` seq 41** (the rule quoted and then overridden by "Nadaar survives, deals
  4 damage. Opponent dies" about a blocked attacker, on a board the header called lethal), and
  reinforced by **`146v162` seqs 35 and 37**, where the same missing fact made the model disbelieve
  two TRUE engine figures for ~2,000 reasoning characters each. Every term is printed on the
  screen: the attacker's `deals N`, `They have N untapped creatures able to block`, and the cover
  or best-case life figure.

**Edit 2 - Acererak is pinned below any row that leaves a body, and the cast order names the mode an
empty board wants.**
- before: `8. Acererak` with the only qualifier living in VENTURE - `He IS a {2}{B} repeatable
  venture: cast him with spare mana only.` - and entry 5 reading `Silverquill Command, when its
  "{modes live right now: ...; dead ...}" clause names a mode you want BEFORE the semicolon.`
- after: entry 5 gains `- at 0 creatures of your own, "return creature" standing there is that
  mode.`, and the VENTURE bullet becomes `... he reaches no combat, and any clause on his row
  counting him as a blocker is FALSE. He IS a {2}{B} repeatable venture: cast him with spare mana
  only, never over a row that leaves a creature on your battlefield.`
- paid for by **`146v152` seq 21** (11 life, `of which 0 are creatures`, a 7-damage crack-back,
  Command's "return creature" live on a mv-2 Silencer in the graveyard, Acererak taken with the
  stated reason "No targets for modes really") and by **seqs 23 and 25** of the same game, where
  Acererak was taken over a listed `Cast Silverquill Silencer`. The "any clause counting him as a
  blocker is FALSE" half routes around HIGH 1 without depending on the broken figure, which is why
  HIGH 1 still needs the engine fix and the guide says nothing else about it.

**Edit 3 - the bytes.** Nothing was deleted but wording. Compressed: the deck header and ENGINE
line, cast-order entries 1/2/5's phrasing, Verse's hold sentence, Soul Shatter's tied-names
sentence, Command's live/dead and DRAW DIRECTION lines, the Silencer sort sentence, Pelakka's
reveal sentence, the pathway and land-backed bullets, the hand-header double-count line, the Hive
preamble and CHECK 2 wrap, COMBAT's opening, CHECK 0/A/B tails, the collapsed-blocker and lifegain
bullets, the converter bullet, the combat-math bullet, the BLOCK LADDER preamble and rungs 0/3,
the dungeon and room bullets, Lolth's and Kaya's bullets, the legend-reset paragraph, the hold
paragraph's line breaks and its run-of-declines sentence, the mulligan header and colour bullet,
the counter/artifact paragraph, the Sanguine Bond opening, and LEGEND RULE. CLEANUP DISCARD's
rungs (2) and (3) were MERGED into one rung - both are "a copy you already have", the merged rung
keeps both tests and both tiebreakers verbatim, and nothing it decided was lost.

## 4. What I did NOT check
- **`146v125` window by window.** 184 records over 41 turns; I read the attacker windows, every
  Kaya and Hive window, the Pelakka Predation reveal at seq 140-142 and the hand/battlefield lines
  at roughly every tenth record. I did not open the ~60 consecutive `Cast nothing right now` /
  `Hold priority` windows of T19-T33 individually.
- **Whether the seq-21 Command line actually wins the 152 game.** I established that "return
  creature" was live, that the only returnable body was the graveyard's 3/2 Silencer, that the
  board was creatureless and that the guide's own order puts entry 5 above entry 8. I did not
  simulate the rest of the game.
- **Whether any line beats deck162's punisher board.** I confirmed the seat had one Verse for three
  punishers and spent it on the highest-value one; I did not search T5-T11 for a faster clock.
- **The 93 `ask_replays_reserved` windows.** A replayed answer never reaches the model, so its
  correctness for the new board (DC F1's aliasing risk) is not decidable from a seat log. Same for
  `hold_windows_skipped` 207, `own_turn_windows_skipped` 44 and `main_phase_windows_skipped` 6 -
  skipped windows are not in the file.
- **`ask_key_continuation_differs` 12 and `hold_verdict_safer_ignored` 2** - I counted them off the
  gameend records and did NOT verify, window by window, that a legal continuation actually changed
  or that the ignored verdict was truly safer. The engine seat has the key.
- **`hold_check:` claim arithmetic.** I read the bracket on the windows I opened and found no false
  statement, but I did not re-run wave 78's full normalised diff of every bracketed window against
  its `hold_check_ref_seq`, so I am making no corpus-wide claim about those brackets this wave.
- **`stop_reached_*`, `stack_drain_windows_asked`, `own_loop_*`, `menu_pass_no_progress_suppressed`,
  `AT BEST`, the gang-block trample spill, `UNLESS <source> untaps them` on a seat-owned trigger,
  keep-X naming a counterspell, `mana_only_windows_skipped`, CLEANUP DISCARD** - 0 renders or 0
  records each on this seat. UNTESTED, not passed; the state never arose here.
- **Opponent seats** beyond board and log context; no audit of any opponent deck's play.
- I read no engine source. Every claim above is from the corpus bytes.

No `general-proposals.md` and no `skill-proposals.md`: Edit 1 is A333 and A334 applied to a section
ordering, Edit 2 is the installed skill's printed-fact discipline. Nothing here needs a new
amendment.
