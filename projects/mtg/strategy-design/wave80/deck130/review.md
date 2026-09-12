# Wave 80 — deck 130 (mono-red land destruction) — per-deck review

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final`. All six of this
deck's games ran on the 20-game binary `archives/wagic-a7a6b8942-w80step1` (= cf72a002f); the
`162v123` rerun on ba5cdc374 is not one of mine, so lanes DI/DJ are out of this seat's reach.
`--thinking on`, all six games finished naturally. Live guide under review = the wave-79 edit
(19,933 B), installed at 36ae77199. **Result: 4 wins / 6** (wave 79: 4/6; 78: 0/6; 77: 1/6).
Wins vs 126, 162, 123, 125; losses vs 152 and 146.

**Seat census.** 336 records, **301 model decisions**, **100% with native reasoning**
(`thinking: on` on every record; median `reasoning_chars` 6,225, max 15,545), **0 fallbacks**
(no record carries a `fallback` field), 0 `reply_truncated`, 0 `action_before_plan`,
0 `answer_label_absent_read`. Kinds: ask 209, priority 70, hold_event 13, attackers 11,
discard 7, gameend 6, gamestart 6, system 6, forced_close 4, blockers 3, bottom 1.
`protocol_deviation`: compliant 283 / `unlabelled_plan` 17 / `plan_absent` 1.
Latency p50 99.9 s, max 358.7 s. Gameend sums over the six games: `protocol_replies` 301,
`own_turn_windows_skipped` 106, `chain_acting_rows` 94 (`_cast` 18) / `chain_selfharm_rows` 43 /
`chain_windows_only_selfharm` 11, **`crackback_verdict_lines_rendered` 65** (NEW),
`hold_windows_skipped` 55 (cast 37 / priority 18), **`plan_cast_steps_closed` 52** (NEW),
`plan_line_missing` 18, `ask_replays_reserved` 17 = `_cache` 17 + `_repeat_latch` 0 +
`crossphase_replayed` 0 (U9 identity holds in this seat), **`hold_events` 13** (NEW),
`hold_released_turn` 8, `ask_key_continuation_differs` 7, `stack_drain_windows_asked` 7,
`blocker_forecast_rows` 12 / `_multi` 6, `async_drops` 4, `forced_close_events` 4 /
`_unrecorded` 2, **`stack_death_verdict_lines_rendered` 4** (NEW), `main_phase_windows_skipped` 3,
`crossphase_identical_reputs` 3, `hold_verdict_safer_ignored` 1, `identical_option_asks_resolved` 1,
and **0** for `crackback_lethal_blocked_away`, `hold_reopened_new_lethal`, `hold_reopened_new_threat`,
`crossphase_board_unchanged`, `crossphase_replayed`, `menu_single_outcome`/`single_outcome_rows_spared`,
`own_loop_*`, `stop_reached_*`, `mana_only_windows_skipped`, `blocker_forecast_gang`,
`plan_names_uncastable_zone_card`, `wall_miss_*`, every `force_close_arms_*`.
**Prompt bytes: max 26,330** (wave 79: 22,357), median 11,969 (10,456), **14 of 301 over 20 KB**
(11 of 322) — up, not down, despite U16's −91 B fold; see §2 LOW-2.

---

## (1) Game-by-game, and the deciding decisions

| Opponent | Result | Turn | Final life | Decisions | Priced to |
|---|---|---|---|---|---|
| 126 | **WIN** | 22 | 20 / 0 | 45 | Rorix over three 0/4 defenders; **six turns of correct-rules paralysis first (§2 HIGH-1)** |
| 162 | **WIN** | 24 | 5 / 0 | 86 | Siege-Gang + sacrifices; **cost four turns and 5 life at seq 75 (§2 HIGH-2)** |
| 123 | **WIN** | 24 | 18 / -1 | 47 | wave-79 Edit B: EMPTY-HAND Hammer return at seq 44 |
| 125 | **WIN** | 28 | 20 / 0 | 64 | Rorix from t22, uncontested |
| 152 | **LOSS** | 15 | -10 / 22 | 27 | matchup: Huntmaster + Sigarda on an empty board |
| 146 | **LOSS** | 17 | -9 / 21 | 32 | **a decision** (seq 13, turn 8) |

One of the two losses is priced to a decision of this seat's. Both WINS carry a bigger misplay than
either loss does, and both are engine-shaped.

### vs 146 — LOSS on turn 17 at -9. Turn 8 cast the land kill over the body, and the row said so
`1789216927-...-vs-ai_baka_deck146.jsonl`. At **seq 13** (t8, 20 life, **6 untapped sources**) the
casting menu offered, in order:
```
1. Cast Siege-Gang Commander {3}{r}{r} (2/2) {leaves 1 of your 6 untapped mana sources untapped}
   ... {leaves 1 source - row 2 needs more mana sources than the 1 this leaves}
2. Cast Lay Waste {3}{r} {leaves 2 of your 6 untapped mana sources untapped} ...
   {leaves 2 sources - row 1 needs more mana sources than the 2 this leaves}
```
Hand: Siege-Gang `[castable now]`, two Lay Waste, Spark Spray. **It took row 2.** The guide's ONE
OVERRIDE is exactly this case ("subtract its cost from 'Mana available:' - if what is left cannot
cast a creature your hand marks [castable now], cast THAT BODY FIRST"), and the row's own clause
had already done the subtraction. The body landed on **turn 10** instead of turn 8; the seat then
went 20 → 16 → 10 → 8 → 3 → -9 with the opponent's Nadaar/venture clock never contested. The rest
of the game is clean: **seq 32** correctly declined an attack into Nadaar
(`(your attacker dies, their blocker lives)`), **seq 33** double-blocked Nadaar, **seq 24**
sacrificed a Goblin onto Triumphant Adventurer with three bodies still standing. This is the guide
item paid this wave — **Edit A** — and it is a PLACEMENT failure, not a missing rule (§3).
One residual misplay, cost 1 life: **seq 25** (t11 blockers, 14 life, 4 inbound, NOT lethal) rows
`B1/B2 - may block A1 (your blocker dies, attacker lives), A2 (both die)`; it answered
`BLOCKS: none`. The guide says take a `(both die)` with a GOBLIN token always. One life in a game
lost by nine.

### vs 152 — LOSS on turn 15 at -10; a hand, not a decision
`1789216918-...-vs-ai_baka_deck152.jsonl`. Twenty-seven decisions, no creature of its own on the
board until turn 12. **seq 21** is the one soft miss: the X menu carried
`3. X = 3 {X pricing: same kills as X=4, for 1 less mana} ... [<- most kills at any affordable X
that costs you nothing - at X=3 that is THEIRS: Katilda, Dawnhart Prime, Luminarch Aspirant,
Wolf x2; YOURS: none]` and the seat answered `CHOICE: 4 (X = 4)` — against both halves of the
guide's X rule, for 1 floating mana and no board effect (nothing else was castable at 1 mana).
**seq 26** (t13 blockers, 13 life, `14 unblocked damage - you would be at -1`) is a good block: it
reproduced the engine's own best-case assignment exactly and survived at 9. **seq 27** (t14 upkeep,
9 life, 7 sources, hand = one Stone Rain, trend `unchanged at 22 since turn 12` = ZERO, and their
line 5 lands + 15 power so THE LAND PLAN is doubly DEAD) took the Hammer return the guide closes at
a zero total; the EMPTY-HAND override that would have opened it needs `Mana available:` 8 and it had
7. A violation with no recoverable price: the printed crack-back was `total to subtract from your
life is 18 and you would be at -9` against 0 creatures, and nothing in hand or graveyard answered it.

### vs 126 — WIN on turn 22, after turns 14-20 of correct-rules paralysis
`1789216933-...-vs-ai_baka_deck126.jsonl`. From **seq 23** to **seq 34** (t14-t18) the seat declined
every action it had — `CHOICE: 2 (Hold priority)` four times, Talisman twice — on this reasoning:
> *"Casting Talisman deals 1 damage to me. That triggers Sanguine Bond (they gain 1, I lose 1).
> Exquisite Blood triggers (they gain 1, I lose 1). This chains until ..."*
> *"dealing damage to them makes them lose life, which triggers Exquisite Blood, making them gain
> life, which triggers Sanguine Bond, making me lose life... loop"*
Both are correct Magic. The opponent's battlefield at seq 23 renders **Sanguine Bond** ("Whenever
you gain life, target opponent loses that much life") **and Exquisite Blood** ("Whenever an opponent
loses life, you gain that much life"). Declining the Talisman was RIGHT and is the guide's TALISMAN
rule firing. Then at **seq 42** it changed its mind, cast Hammer at the face — and **the engine did
not run the loop** (§2 **HIGH-1**). It won from there on Rorix. Its own PLAN at seq 43 reads
*"to trigger the infinite loop and win"*: run faithfully that loop kills the SEAT, so the win rests
on the engine contradicting its own rendered card text.

### vs 162 — WIN on turn 24 at 5, having swept its own lethal board away on turn 20
`1789216937-...-vs-ai_baka_deck162.jsonl`. At **seq 75** (t20 upkeep, **10 life, opponent 7**,
8 untapped sources + an unused land drop) the board was Siege-Gang Commander + three Goblins +
Dwarven Blastminer against three 0/6 Shield Spheres and a Fate Unraveler. ORDER OF OPERATIONS entry
0 is exact here: four Goblin bodies (the Commander is one) at {1}{R} each = 8 mana for 8 damage,
against `Opponent life: 7`. The casting row read
```
1. Cast Starstorm {r}{r}{x} {X pricing: ... smallest X that kills an OPPONENT creature: X=4 ...
   kills THEIRS: Fate Unraveler; YOURS: Dwarven Blastminer, Siege-Gang Commander, Goblin x3 ...}
   [<- best X for this cast: X=6 - the only X that kills anything of THEIRS costs you more of
   YOURS than it takes - THEIRS 4 / YOURS 5]
   ... [<- board sweep: THEIRS 4 / YOURS 5 - the only row on this menu that prices a board sweep
   (it takes more of YOURS than of THEIRS)]
```
Its reasoning at seq 75: *"Blastminer, Siege-Gang Commander, 3 Goblins. They die. That's fine, I'm
sacrificing them anyway for damage. So casting Starstorm X=4 is perfect. ... It says X=4 kills Fate
Unraveler."* — it read the marker's **leading words** ("best X for this cast: X=6") and never
processed the refusal in its tail (§2 **HIGH-2**). At **seq 77** it reached the X menu, where the
marker is refusal-only, and its reasoning quotes the guide against itself:
> *"Marker says: '[<- the only X that kills anything of THEIRS costs you more of YOURS than it
> takes - THEIRS 4 / YOURS 5]' ... Wait, the guide also says: 'A marker saying the sweep costs you
> more of YOURS than it takes of THEIRS is a REFUSAL: decline the cast.' — **But the plan explicitly
> says**: 'Remove Fate Unraveler with Starstorm at X=4 ...'"*

and answered `CHOICE: 5 (X = 5)`, killing Siege-Gang, three Goblins and the Blastminer to kill one
3/4. It had to rebuild: it won on turn 24 with a SECOND Siege-Gang, at 5 life, after four more
turns of Underworld Dreams draw damage (10 → 7 → 5). Three separate guide rules said no here
(entry 0; STARSTORM'S X DO-NOT-CAST (a), the YOURS list naming Siege-Gang; PLAN LINES' "A CARRIED
PLAN NEVER OUTRANKS THIS WINDOW'S PRINTED VERDICT ... killing more of YOURS than of THEIRS"), all
three in print and correctly worded. I make no guide edit for it: what the record shows is a
contradictory marker at the cast row and a carried plan beating a rule the model quoted verbatim.
The rest of the game is sound — **seq 84** took the marked X=6 on a `THEIRS 4 / YOURS 0` sweep,
and seqs 85-103 walked the opponent from 7 to 0 on Spellbombs and sacrifices.

### vs 123 — WIN on turn 24. Wave-79 Edit B paid, on the win path
`1789216941-...-vs-ai_baka_deck123.jsonl`. At **seq 44** (t22 upkeep, 20 life, opponent 5,
**hand `(0 cards): (none)`**, `Mana available: 8`, trend `-3 since turn 18`) the seat took
`Put a card into hand with Hammer of Bogardan`, recast it at **seq 45** and pointed it at the face at
**seq 46** (5 → 2). Turn 24 closed it: Siege-Gang, Spark Spray, one sacrifice, `-1`. The gate held
correctly on the four upkeeps before it (seqs 26/30/33/38) where the hand was not empty.

### vs 125 — WIN on turn 28 at 20. Uncontested, and the land plan outlived its own verdict
`1789216943-...-vs-ai_baka_deck125.jsonl`. Two of its spells were countered (`Your Molten Rain was
COUNTERED by Dream Fracture`, `Your Stone Rain was COUNTERED by Cancel`), so THEY COUNTER is live
content in this pool. At **seq 32** the seat wrote the land plan's three numbers verbatim —
`PLAN: Their lands 6; my life 20; their power 0. Cast Lay Waste at Tundra.` — and cast anyway:
`N at 5 or more is DEAD` is its own rule and 6 is the number it wrote. It then spent turns 14-20 on
four more land kills at an unchanged opponent life. Cost 0 (it won at 20 with Rorix from t22), so
no edit; recorded because the shape is "writes the verdict, ignores it", not "never did the check".

---

## (2) Engine / interface / card items

### HIGH-1 — Exquisite Blood does not trigger on life lost to damage, and its printed text paralysed the seat for six turns
Repro `...vs-ai_baka_deck126.jsonl`, board from **seq 23** onward:
`Exquisite Blood {4}{b} [enchantment] {effect (THEIR card ...): "Whenever an opponent loses life,
you gain that much life."}` alongside `Sanguine Bond`. Over turns 20-22 the seat dealt the opponent
**11 life of damage** in three separate events — the narration at seq 51 reads
```
- Your Rorix Bladewing dealt 6 damage to the opponent (now 11)
- Your Hammer of Bogardan dealt 3 damage to the opponent (now 8)
- Your Pyrite Spellbomb dealt 2 damage to the opponent (now 6)
```
and the same prompt's trend line reads `-11 since turn 18; over those turns life-gain EVENTS put
**+0** on them`. Under 118.2 damage to a player IS life loss, so Exquisite Blood should have
triggered three times and Sanguine Bond behind it. It fired **zero** times. The card script is
`auto=@lifelostfoeof(player):life:thatmuch controller` (mtg.txt:37966-37968) — the likely seam is
that the `@lifelost` trigger is raised only by explicit life-setting effects and not by damage; I
have not read the trigger code and do not claim that as the root cause, only as where to look.
**Why it is HIGH and not a curiosity:** the seat read the rendered text, reasoned it correctly and
completely (quoted in §1), and therefore declined EVERY action from turn 14 to turn 20 — six turns
of a 22-turn game — while holding Rorix in hand at 20 life. It won only because the board did not
do what its own card text says. This is the Trust Doctrine's exact failure mode with the polarity
reversed: the surface was TRUE as printed and the ENGINE was the lie.

### HIGH-2 — the `[<- best X for this cast: X=N ...]` marker opens as a recommendation on a cast it is refusing
Repro `...vs-ai_baka_deck162.jsonl` **seq 75**, quoted in full in §1. On a sweep whose own
companion marker says `THEIRS 4 / YOURS 5 - ... (it takes more of YOURS than of THEIRS)`, the X
marker still renders with the affirmative prefix **`best X for this cast: X=6`** and puts the
refusal in a trailing clause. The X MENU one seam later gets this right — at **seq 77** the same
verdict renders as `[<- the only X that kills anything of THEIRS costs you more of YOURS than it
takes - THEIRS 4 / YOURS 5]`, with no "best" prefix. The seat's seq-75 reasoning shows it consuming
the prefix and stopping (*"So casting Starstorm X=4 is perfect. ... It says X=4 kills Fate
Unraveler"*), and the cast was committed there; by the X menu the plan was already carried and the
refusal lost to it. **Fix shape:** when the priced sweep takes more of YOURS than of THEIRS, the
cast-row marker must not be a `best X for this cast:` marker at all — emit the X-menu's own
refusal wording, so no marker on any seam ever names a best and a refusal in one bracket.
This is the one item in this seat that cost a game four turns and half its life.

### MED-1 — `plan_line_missing` is the dropped `PLAN:` LABEL, 17 of 18, tenth wave unchanged
17 of the 18 `protocol_deviation` replies carry a first line that IS a plan, without the label:
`152` seq 14 `"\n\nCast Lay Waste targeting Overgrown Farmland.\nCHOICE: 1 (Cast Lay Waste)"`;
`146` seq 29 `"\n\nDestroy Hive of the Eye Tyrant.\nCHOICE: 1 (Cast Card Normally)"`; `162` seq 42
`"\n\nCast Rorix Bladewing\nCHOICE: 2 (Play no land right now)"`; `126` seq 52
`"\n\nAttack with Rorix Bladewing to deal 6 lethal damage. ...\nATTACK: A1"`. The 18th, `146`
**seq 33**, is the seat's one `plan_absent`: the whole reply is `"\n\nBLOCKS: B1:A1, B2:A1"`.
Under the owner's 2026-09-12 rule the plan unambiguously precedes the action in all 17; accepting
an unlabelled first line as the plan still zeroes 17 of 18.

### MED-2 — the reasoning channel still ends in output-formatting chatter, and it is next to the wave's worst decisions
`126` **seq 6** and **seq 11**, `162` **seq 15**, `125` **seq 39** all close with 20-40 lines of
`"Output matches. Proceeds. Done. [Final Check] ... Ready."` after the decision is made; three of
those four are the upkeep cycles of §2 MED-3. Recorded, not an item against any render: nothing
false is printed. It is a standing cost against the wave's `reasoning_chars` budget (median 6,225).

### MED-3 — nothing on an upkeep window tells the seat that a cycling row is spending the main phase's mana
Repro `...vs-ai_baka_deck126.jsonl` **seq 11** (t10 upkeep, 3 sources):
```
1. cycling with Lay Waste [cost: {2}, Cycle] {spends 2 of your 3 untapped mana sources this turn;
   Stone Rain {2}{r} in your hand needs 3} ...
```
It cycled, and its own PLAN on that reply was `"Main phase 1: cast Molten Rain at Chromatic
Lantern"` — a {1}{R}{R} spell it could no longer pay for; at seq 13 it cast a {1} Spellbomb
instead. Same shape at `162` **seq 15** (`spends 2 of your 3 ...; Stone Rain {2}{r} in your hand
needs 3`, plan `"cast Lay Waste on Drowned Catacomb in main phase 1"`, and the main phase went to a
{1}{R} Blastminer) and `126` **seq 6** (`spends 2 of your 2 ...; Pyrite Spellbomb {1} in your hand
needs 1` — it spent the whole turn's mana at upkeep and got no main-phase window at all).
The tap bill is correct and complete; what is missing is that it is *just a count* on a window
where the consequence is a whole phase. The seat's reasoning at seq 11 shows it reading the guide
instead and getting the pronoun wrong: *"the guide says: 'A CYCLING ROW IS NEVER ONE OF THEM.' This
refers to holding priority. It doesn't forbid cycling."* Guide side is **Edit B**; the render side
is this note, LOW confidence as an engine ask.

### LOW-1 — 65 crack-back verdict lines, every one TRUE, 0 on a suppressed window
I read all 54 that appear on a consumed record (the gameend counter is 65; I did not reconcile the
11 that do not appear on a record and make no claim about them). Every verdict matches the
arithmetic of its own CRACK-BACK line: `152` seq 22 `for up to 11 ... you would be at 2 or lower`
at 13 life -> `you survive`; `152` seq 27 `the total to subtract from your life is 18 and you would
be at -9` at 9 life -> `LETHAL if it is UNBLOCKED`; `162` seqs 36-59 `for up to 5 ... you would be
at 5` at 10 life -> `you survive`, all true. `crackback_lethal_blocked_away` is 0 in this seat and
the state never arose (every LETHAL verdict here is on a board with 0 creatures of the seat's).
`stack_death_verdict_lines_rendered` 4, all in `162`, all `nothing lethal on the stack` /
`you survive the stack` and all true.

### LOW-2 — prompt bytes went UP, not down
Max **26,330 B** (wave 79: 22,357), median 11,969 (10,456), 14 of 301 over 20 KB (11 of 322).
The U16 hold-row fold is a −91 B/prompt constant; the growth is elsewhere. The two largest
contributors I can see by eye are the `crack-back cover:` clause (a single one at `152` seq 19 runs
**1,540 B** on one row) and the new `crackback_verdict` / `hold check` / `HOW A HOLD ENDS` block.
No prompt in the seat carries a `trimmed` marker and no prompt lost its own last PLAN, so nothing
was cut — this is genuine growth, not a trim artifact.

### LOW-3 — `forced_close_unrecorded` 2 of 4, sixth wave, still unattributable from a seat's side
`126` seqs 46/48 and `123` seqs 8/9, empty replies, `outcome: dropped_decision_moved` on the
second of each pair, against `force_close_arms_refused` / `_deferred` / `_same_arm_deferred` /
`_defer_bound_hits` all 0. 0 fallbacks and 0 `phase2_answer_missing` are consistent with no
decision lost; unconfirmed from this side, the stderr being shared with the opponent seat.

### Wave-80 lane predictions this seat can adjudicate
- **U1/DG + DH F5 (`plan_cast_steps_closed` 376; the cast clause survives to the completing menu)
  — PASS, 13 of 13.** Every `Choose an option for <card>` menu in the seat echoes a plan whose
  cast clause is intact: `152` seq 15 `"Play Mountain, then cast Lay Waste targeting Overgrown
  Farmland."`; `152` seq 20 `"Clear their board with Starstorm to survive the lethal crack-back,
  then develop my own board."`; `146` seq 14 `"Cast Lay Waste at Hive of the Eye Tyrant, then cast
  Siege-Gang Commander next turn."`; `146` seq 29 `"Destroy Hive of the Eye Tyrant with Lay Waste
  ..."`; `162` seq 9 `"Cast Spark Spray at opponent, cycle a Mountain next turn, ..."`; `162`
  seq 76 `"Remove Fate Unraveler with Starstorm at X=4 ..."`; `162` seq 83 `"Cast Starstorm at X=6
  ..."`; `162` seq 91 `"Cast Spark Spray targeting the opponent to reduce their life to 2, ..."`;
  `123` seq 19 `"Cast Starstorm at Thraben Doomsayer now. ..."`; `123` seq 42 `"Cast Lay Waste to
  destroy Scrubland #3. ..."`; `123` seq 50 `"Cast Spark Spray to put opponent at 1, ..."`;
  `125` seq 22 `"Cast Lay Waste targeting Tundra. ..."`; `125` seq 33 `"Their lands 6; my life 20;
  their power 0. Cast Lay Waste at Tundra."`; `125` seq 67 `"Cast Spark Spray targeting opponent to
  win."` **Wave 79's HIGH-1 is CLOSED in this seat.** 12 of the 13 took `Cast Card Normally`.
  **The one non-cast answer is `162` seq 9** (took `cycling`), and its decline row printed the
  stranded mana in full: `3. Decline - do nothing {your 1 tapped source is SPENT either way: the
  payment for Spark Spray is already made ... the mana it paid stays floating in your pool, and
  whatever is still floating when the step ends is lost}` — **wave 79's MED-2 is also CLOSED**.
- **U6 (cycling = discard + draw) — UNTESTED here; the state never arose.** I quoted every cycling
  row in the seat (49 rows across 40 windows). **No board in any of the six games carried a discard
  punisher** — the only `DRAW PUNISHERS` lines are `162` seq 75 (Fate Unraveler) and seq 89
  (Underworld Dreams), and `DISCARD PUNISHERS` appears nowhere in any prompt. Every `[DRAW PRICE:]`
  bracket in the seat therefore prices the draw half only and is CORRECT to do so, e.g. `162`
  seq 89 `[DRAW PRICE: this draws 1 card, and the opponent's Underworld Dreams punishes every draw,
  so taking it costs you 1 life right now - you would be at 9]` and seq 103 (`... at 4`). I cannot
  distinguish "the discard fold is wired and charges 0" from "it is absent"; the engine seat's
  corpus-wide scan is the instrument. Wave 79's HIGH-2 is UNTESTED, not closed.
- **U3c (the spent-blocker clause on every sacrifice-cost row) — UNTESTED here, and the checkable
  half HOLDS.** 125 sacrifice-cost rows in the seat, **0 carrying the clause**. The only window
  where the state could have arisen is `146` **seq 24** — a CRACK-BACK of `3 of their creatures ...
  for up to 5` against 4 standing bodies, so blocking all three attackers is reachable with 4
  bodies and with 3, and the give-back is genuinely 0; DF's own pinned identity says nothing
  prints. The checkable half — 0 renders on a row whose cost cannot take a creature — holds: every
  Pyrite Spellbomb row (`152` seq 11, `126` seqs 14/29, `146` seq 5) sacrifices an artifact and
  carries no clause.
- **U3a/b (the cover names a real self-leave, mechanism trample-aware) — one instance, PASS.**
  `152` seq 19's Siege-Gang cover clause names its mechanism in full ("an attacker that is BLOCKED
  deals its combat damage to the blocker, not to you - all of it ... and a 1/1 in front of a 5/5
  stops the whole 5") and ends `None of these attackers has trample.` — true of that board. No
  trampler appeared among any crack-back attacker in the seat, so the trample branch is UNTESTED.
  No body is credited that the row does not leave: the 4 bodies at seq 19 are Siege-Gang plus its
  three ETB tokens, and the clause is explicit that 3 of them are `tokens this row has no instance
  to ask`.
- **U4 (one tap bill per row) — PASS.** 94 `spends N of your M untapped mana sources` clauses in
  the seat, **0 rows carrying two**.
- **U7 (`board sweep:` names YOURS) — PASS, 9 of 9.** `123` seq 18 `THEIRS 1 / YOURS 0`; `152`
  seq 19 `THEIRS 4 / YOURS 0`; `162` seqs 49/52/55 `THEIRS 2 / YOURS 2`, seq 57 `THEIRS 1 / YOURS
  1`, seq 69 `THEIRS 3 / YOURS 1`, seq 75 `THEIRS 4 / YOURS 5 ... (it takes more of YOURS than of
  THEIRS)`, seq 82 `THEIRS 4 / YOURS 0`.
- **U2/DE + DH F3/F11 (`hold_events`) — all 13 read.** 11 are `reopen_rows_moved` with a printed
  reason (`a printed row changed or is newly available` ×11, `a printed row it was held over is
  gone` ×2); one is the seat's single `hold_verdict_safer_ignored`, `162` **seq 97**: `held [stack
  death verdict: you survive the stack] over live [stack death verdict: nothing lethal on the
  stack]`. Both faces are non-lethal, so ignoring the safer one cost nothing — ignoring was right.
  `hold_reopened_new_lethal` 0 and `hold_reopened_new_threat` 0, and **no new equal-rank threat
  arose in this seat**: the only board that ever produced a lethal crack-back (`152` from seq 27)
  had no hold standing over it.
- **U8/U9 (cross-phase replay) — UNTESTED here.** `crossphase_replayed` 0, `crossphase_board_
  unchanged` 0, `crossphase_identical_reputs` 3 (all `162`). `ask_replays_cache` 17 +
  `_repeat_latch` 0 + `crossphase_replayed` 0 = `ask_replays_reserved` 17, so U9's identity holds.
  I did not open `askreplay/`.
- **U10/DH F6 (own-loop face), U13 (`menu_single_outcome`), U5 (neutral exemplar row-1 rate),
  U17 (per-seam cost), DI/DJ** — no instance in this seat, or out of its binary's reach.
- **`{X}-announcement payment loss`** — 0 `dropped after its X was announced` in the six games'
  stderr; three X announcements were completed and one (`152` seq 21) took a larger X than the
  marker named, which is a decision, not a payment loss.

### Card facts — verified, no item beyond HIGH-1
Sanguine Bond (mtg.txt:99004-99006) and Exquisite Blood (37966-37968) both carry the modern oracle
`text=` and the trigger-shaped `auto=`; the disagreement is engine behaviour, not card data.
Verified against the prompts and primitives: Underworld Dreams, Fate Unraveler, Dictate of Kruphix,
Howling Mine, Shield Sphere, Fog Bank, Master of the Feast, Teferi's Puzzle Box, Forced Fruition,
Ob Nixilis the Hate-Twisted; Tovolar's Huntmaster/Packleader (daybound/nightbound, 6/6 printed
rendering as 7/7 under Katilda — correct), Sigarda Champion of Light, Katilda, Luminarch Aspirant,
Teferi Who Slows the Sunset, Lair of the Hydra; Nadaar Selfless Paladin, Triumphant Adventurer,
Hive of the Eye Tyrant, Silverquill Silencer; Perimeter Captain, Pride Guardian, Overgrown
Battlement, Chromatic Lantern, Wall of Omens; Lightmine Field, Tundra, Dream Fracture, Cancel.
Own side: Starstorm, Hammer of Bogardan, Siege-Gang Commander, Rorix Bladewing, Dwarven Blastminer,
Molten Rain, Stone Rain, Lay Waste, Spark Spray, Pyrite Spellbomb, Talisman of Impulse, Forgotten
Cave. No Scryfall disagreement worth reporting.

---

## (3) Guide verdict: **EDIT** — revised guide at `wave80/deck130/strategy.txt`

**19,832 bytes vs the live 19,933 — net −101**, two edits (+56) paid by one trim (−157). No
addition is a concession and each is a precondition (A334); no citation, count, or history is in
the guide (A332); headings are unchanged scope declarations (A333); the PLAN template is untouched
and nothing quotes "YOUR PLAN". Neither edit grows the guide on net.

**Edit A — the ONE OVERRIDE moves INTO the entry it overrides, and names the clause that computes it.** (+195 / −157 = +38)
Before, entry 3 read `3. STONE RAIN, MOLTEN RAIN or LAY WASTE at one of their lands, under THE LAND
PLAN's numbers.` with the override printed four entries later, under entry 7:
`ONE OVERRIDE: before entry 3, subtract its cost from "Mana available:" - if what is left cannot
cast a creature your hand marks [castable now], cast THAT BODY FIRST and take the land later.`
After, entry 3 carries it and the trailing block is gone:
```
 3. STONE RAIN, MOLTEN RAIN or LAY WASTE at one of their lands, under THE LAND PLAN's numbers.
    ITS OWN "{leaves N sources - row M needs more mana sources than the N this leaves}" clause
    OUTRANKS this entry when row M is a creature marked [castable now]: cast THAT BODY FIRST.
```
Paid by `146` **seq 13**, the deciding decision of the 146 loss. The heading's own instruction is
"take the FIRST entry below matching a row on your menu **and stop there**" — a model that stops at
entry 3 never reads a modifier printed below entry 7, and this seat did exactly that. The rule is
not new and its wording is not softened; it is relocated to the window where it fires, and the
subtraction it asks for is replaced by the clause the engine already prints, so the model reads a
number instead of computing one.

**Edit B — "a cycling row is never one of them" becomes an unambiguous ban.** (+57 / −0)
Before, in YOUR OWN UPKEEP: `... still covers every ORDER OF OPERATIONS entry this board matches. A
CYCLING ROW IS NEVER ONE OF THEM.`
After: `... still covers every ORDER OF OPERATIONS entry this board matches. NEVER
TAKE A CYCLING ROW AT YOUR OWN UPKEEP: no cycling row is ever one of those exceptions.`
Paid by `126` **seq 11** — the seat quoted this exact sentence and resolved its pronoun the wrong
way: *"'A CYCLING ROW IS NEVER ONE OF THEM.' This refers to holding priority. It doesn't forbid
cycling."* — and by the three other upkeep cycles it made (`126` seq 6, `162` seq 15, `125`
seq 39), two of which cost that turn's land kill and one of which cost the whole turn's mana
(§2 MED-3). The CYCLING heading's `never at your own upkeep` clause, restored in wave 79, is intact
and unchanged; it is simply not the heading a model reads at an upkeep window.

**Bytes trimmed to pay for the above (−157):**
STARSTORM'S X (b)'s carve-out `- carve-out, if a GROUNDED attacker of yours is being held back by
exactly those defenders, the sweep buys your clock (never for Rorix, who flies over them).` — a
concessive clause for the rule's hard case (A334) with no instance in five waves. `162` is the only
board this wave whose line was all-`[defender]` plus one body, and the carve-out excludes Rorix by
its own terms, so it was dead text on the one board that resembled it. The rule keeps its plain
form: `(b) the THEIRS list names only "[defender]" creatures;`.

### What the wave-79 edits decided
- **Edit A (a cycle is a discard as well as a draw; the twice-a-turn and upkeep timing clause)** —
  **READ AND APPLIED, but the punisher case is UNTESTED.** Six records quote the new paragraph in
  their reasoning and check for discard punishers by name (`126` seqs 39/40, `162` seqs 22/27,
  `125` seqs 11/44 — e.g. *"'DISCARD PUNISHERS ... add that N to the draw N before both tests
  above'. There are no discard punishers mentioned."*). No board carried one, so the arithmetic
  never fired. **The timing half FAILED four times** and is what Edit B above rewrites.
- **Edit B (EMPTY-HAND overrides a ZERO total; the upkeep is the ability's only window)** —
  **PASS on the rule as a whole, UNTESTED on the ZERO clause specifically.** `123` seq 44 is the
  EMPTY-HAND take, on a MINUS trend, on the win path. No window this wave had an empty hand, a zero
  trend AND 8+ mana together; `152` seq 27 is the near-miss (zero trend, hand = a dead land spell,
  but 7 mana), and the seat took the return there against the closure with no recoverable cost.
- **Wave-79 HIGH-1 (the plan-step trimmer)** — **CLOSED**, 13 of 13 (§2 lane list).
- **Wave-79 HIGH-2 (the cycling DRAW PRICE)** — **UNTESTED**, state never arose (§2 lane list).
- **Wave-79 MED-1 (the spent-blocker clause on sacrifice rows)** — **UNTESTED**, 0 renders, and the
  one candidate window's give-back is genuinely 0 (§2 lane list).
- **Wave-79 MED-2 (the modal decline row does not say the mana is stranded)** — **CLOSED**
  (`162` seq 9).
- **Wave-79 MED-3 (the dropped `PLAN:` label)** — **recurs**, 17 of 18 (§2 MED-1).
- **Wave-79 LOW-1 (long runs of `Cast nothing right now` at an unchanged board)** — **did not
  recur**: the longest such run in this seat is 3 (`126` seqs 37-39) and the hold row was taken at
  the fourth.
- **Wave-78 Edit A (the face row as a PROHIBITION)** — **PASS with one boundary take.** `123` seqs
  45-52 and `162` seqs 85-103 are all inside the gate; the one edge is `162` seq 85 at
  `Opponent life: 7` exactly, which the gate closes at "7 or more". It was the first point of a
  lethal sequence and cost nothing.
- **Wave-78 Edit C (read the X marker)** — **1 PASS, 1 soft miss, 1 hard violation**: `162` seq 84
  took the marked X=6; `152` seq 21 took X=4 over a marker on X=3 for 1 floating mana; `162` seq 77
  took X=5 against an explicit refusal marker (§1, §2 HIGH-2).

### Rejected, though I looked at them
- **A rule about the Sanguine Bond / Exquisite Blood board.** The seat's reasoning was *correct*
  Magic and the engine was wrong (§2 HIGH-1). Teaching the guide to doubt a rendered card text is
  exactly what the Trust Doctrine forbids; the fix is the engine's.
- **A rule restating the X-marker refusal.** It is in print, correctly worded, and the model quoted
  it verbatim at `162` seq 77 before overriding it with a carried plan — which PLAN LINES also
  already forbids in the same words. Two rules already say it; a third is not the fix.
- **A blocker rule off `146` seq 25's declined `(both die)`.** The rule is in print
  ("a `(both die)` with a GOBLIN token always"), one instance, cost 1 life.
- **A land-plan rule off `125` seq 32.** The seat wrote the three numbers and ignored its own DEAD
  verdict. Nothing is missing from the guide there and the game was won at 20 life.
- **Raising the EMPTY-HAND mana floor from 8 to 7** off `152` seq 27. One instance, in a game that
  was already lost on the board, and lowering the floor would open the return on turns where
  Siege-Gang is the better spend.

## (4) Optional proposals
**None.** Both generalisable findings are engine work and are filed as engine items: §2 HIGH-1
(a trigger the card text promises and the engine does not raise — not deck130-specific; any
punisher, drain or loop enchantment on either side is the same shape) and §2 HIGH-2 (a single
bracket that names a best and a refusal at once, on the seam where the cast is committed). Nothing
this seat saw contradicts an existing amendment or needs a new one.

## What I did NOT check
- **The opponent seats' JSONL.** Every board fact here is read from my own rendered prompts and
  narration; the 126 Exquisite Blood finding is established from my own seat's game log, which
  records the opponent's life after each damage event and the `+0` gain figure on the trend line.
  I did not open `1789216933-ai_baka_deck126-...` to cross-check.
- **The engine's trigger code.** I read the two primitives (mtg.txt:37966, 99004) and no C++. The
  `@lifelostfoeof`-vs-damage hypothesis in HIGH-1 is a pointer for the core loop, not a diagnosis.
- **`askreplay/`, and the 17 `ask_replays_cache` / 7 `ask_key_continuation_differs` / 4
  `async_drops` / 1 `identical_option_asks_resolved` individually.** 0 fallbacks and 0
  `phase2_answer_missing` are consistent with no decision lost; unconfirmed.
- **11 of the 65 `crackback_verdict_lines_rendered`.** I read and checked the 54 that appear on a
  consumed record; I did not reconcile the counter's remainder and make no claim about it.
- **A full audit of the `[hold check:` counts.** I read the brackets on the windows I traced and
  did not reconstruct row identity across all 301 windows.
- **`{effect:}` gloss cuts and the six DG gloss fixes.** I read the glosses inside the windows I
  traced (Sanguine Bond, Exquisite Blood, Nadaar, Sigarda, Teferi, Lair of the Hydra, Perimeter
  Captain all render from the right seat) and saw one `(...more)` cut that IS mid-clause: Sigarda
  at `152` seqs 19/26/27, "Coven - Whenever Sigarda attacks, if you control three or more creatures
  with different powers, look at the top five (...more)", which stops mid-sentence and leaves the
  Coven payoff unnamed. One instance, on a card this seat never interacts with; I did not sample 30
  and make no PASS or FAIL claim on U12.
- **Whether `own_loop_*`, `menu_single_outcome`, `single_outcome_rows_spared`, `stop_reached_*`,
  `mana_only_windows_skipped`, `blocker_forecast_gang` and `crossphase_replayed` reading 0 here are
  unwired counters or honest zeroes.** No instance either way; the corpus-wide figures are non-zero
  for several, so the seat is the wrong instrument.
- **Whether 4/6 is a real strength change.** deck130 has run 4 / 1 / 0 / 4 / 4 across five waves on
  the same archetype. The hands here were ordinary (Rorix on time in three games, five lands by t10
  in both losses), and I priced exactly one loss to a decision.
