# Wave 79 — deck 130 (mono-red land destruction) — per-deck review

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-000748-final`, single binary
`archives/wagic-5af6f7a95-w79step1`, `--thinking on`, one run, all 21 games finished naturally.
Live guide under review = the wave-78 edit (19,959 B), installed 2026-09-11.
**Result: 4 wins / 6** (wave 78: 0/6; wave 77: 1/6; wave 76: 4/6). Wins vs 152, 125, 126, 123;
losses vs 146 and 162.

**Seat census.** 322 model decisions over 6 games (wave 78: 321), **100% with native reasoning**
(median `reasoning_chars` 6,293, max 19,110; `thinking: on` on every record), **0 fallbacks**
(no record in the seat carries a `fallback` field), 0 `reply_truncated`, 0 `action_before_plan`.
Kinds: ask 213, priority 89, attackers 17, forced_close 4, blockers 3.
`protocol_deviation`: compliant 304 / `unlabelled_plan` 17 / `plan_absent` 1 / 4 records carry no
class (the forced_close ones). Latency p50 113 s, max 331 s. Counter sums over the six gameends:
`hold_windows_skipped` 111 (cast 15 / priority 96), `chain_acting_rows` 83, `chain_selfharm_rows`
64, `chain_windows_only_selfharm` 34, `ask_replays_reserved` 30, `own_turn_windows_skipped` 25,
`plan_line_missing` 18, `crossphase_identical_reputs` 11, `async_drops` 6,
`ask_key_continuation_differs` 6, `forced_close_events` 4 / `_unrecorded` 2, `hold_released_turn` 7,
`blocker_forecast_rows` 12 / `_multi` 4, `identical_option_asks_resolved` 2,
`repeat_annotated_takes` 2, `menu_pass_no_progress_suppressed` 1, `main_phase_windows_skipped` 1,
`plan_names_uncastable_zone_card` 1, and **0** for each of `own_loop_windows_asked` /
`own_loop_verdict_lines_rendered`, `stop_reached_windows_skipped` / `_reputs_collapsed`,
`hold_verdict_safer_ignored`, `hold_reopened_new_threat`, `crossphase_board_unchanged`,
`blocker_forecast_gang` / `_collapsed`, `mana_only_windows_skipped`, `identical_ask_answers_reserved`,
`plan_names_stranded_card`, `engine_reveal_floor_picks`, `put_gloss_stripped`, every
`force_close_arms_*`, `phase2_answer_recovered` / `_missing`.
**Prompt bytes: max 22,357** (wave 78: 29,641), median 10,456 (12,492), **11 of 322 over 20 KB**
(77 of 321), max GAME-LOG segment 13,512 B, and the narration trim never fired in this seat
(0 prompts carry a `trimmed` marker) — see §2 T10.

---

## (1) Game-by-game, and the deciding decisions

| Opponent | Result | Turn | Final life | Decisions | Priced to |
|---|---|---|---|---|---|
| 152 | **WIN** | 21 | 11 / 0 | 53 | Starstorm X walk-down + Rorix clock |
| 125 | **WIN** | 17 | 20 / -2 | 60 | Rorix + three Goblins, uncontested |
| 126 | **WIN** | 35 | 15 / -3 | 68 | Goblin sacrifices raced the Bond loop |
| 123 | **WIN** | 19 | 25 / -1 | 45 | Starstorm X=3 on Bloodline Keeper (t10) |
| 146 | **LOSS** | 16 | 0 / 21 | 28 | matchup / mana; killed by a dungeon room at 1 |
| 162 | **LOSS** | 27 | 0 / 9 | 88 | **a decision** (seqs 32 / 35 / 37, turn 14) |

Only ONE of the six is priced to a decision of this seat's, and it is the 162 loss: a single upkeep
in which the seat abandoned a paid Starstorm and then cycled BOTH copies under two punishers, for
**6 life of a 20-life game it lost at exactly 0**. The other loss is a hand this deck cannot answer.

### vs 162 — LOSS on turn 27 at 0. Turn 14's upkeep spent 6 life and both Starstorms for two Mountains
`1789189694-...-vs-ai_baka_deck162.jsonl`. The board: **Underworld Dreams** (a second copy lands on
t17), **Liliana's Caress**, two Shield Spheres and a Fog Bank behind them. The seat's life ticks 2 a
turn from its own compulsory draws from t18 and it dies on t27 with the opponent at 9.

At **seq 31** (t14, upkeep, 19 life, 6 untapped sources) the casting menu offered
```
1. Cast Starstorm {r}{r}{x} {X pricing: max affordable X=4 (6 mana total); smallest X that kills an
   OPPONENT creature: X=2 (4 mana total) - kills THEIRS: Fog Bank; YOURS: none}
   [<- best X for this cast: X=2 - most kills at any affordable X that costs you nothing]
   [<- board sweep: THEIRS 1 / YOURS 0 - the only row on this menu that prices a board sweep]
```
and the seat took it, writing `PLAN: Cast Starstorm X=2 to kill Fog Bank, then attack with Rorix in
second main phase.` Six sources were tapped into the pool.

At **seq 32** the completing menu printed `Choose an option for Starstorm: (you answered "Cast
Starstorm" at the previous window - this menu completes THAT cast ...)` with rows `1. cycling`,
`2. Cast Card Normally`, `3. Decline - do nothing`. **It took row 3.** Its own reasoning reads
> *"If I choose 'Decline - do nothing', I abandon the cast. The 6 mana in pool might be lost, but
> that's fine."*

and the reason it had no objection is on the same screen: **the echoed plan on that prompt reads
only `"then attack with Rorix in second main phase."`** — the clause naming the cast this menu
completes had been stripped by the plan-step trimmer (§2 **HIGH-1**). The guide's rule for this menu
was in print and correct (`"CHOOSE AN OPTION FOR <card>" COMPLETES THE CAST YOU JUST ANSWERED`), and
the menu header said the same thing verbatim; what the seat no longer had was its own intent.

At **seqs 33/35** it re-took the cast row and then, at the priority menu, **cycled** — and at
**seq 37** cycled the second copy. Both cycles happened at its OWN UPKEEP, which the guide forbids
twice over (`Cycle in your SECOND MAIN PHASE only, never at your own upkeep` and `A CYCLING ROW IS
NEVER ONE OF THEM`). The cycling row priced itself
```
1. cycling with Starstorm [cost: {3}, Cycle] [DRAW PRICE: this draws 1 card, and the opponent's
   Underworld Dreams punishes every draw, so taking it costs you 1 life right now - you would be at 18]
```
and the seat's reasoning copied that figure: *"Check life: 19 -> 18."* The **true** price was 3 a
cycle, because the same prompt prints `DISCARD PUNISHERS on the battlefield: theirs - Liliana's
Caress. Every card YOU discard costs you 2 life to theirs` and a cycle is a discard. The log:
`You lost 2 life (now 17) ... You lost 2 life (now 15) ... Underworld Dreams dealt 1 (now 14) ...
(now 13)` — **19 → 13 for two Mountains**, on a board where the seat's clock to death was its own
life total. That is §2 **HIGH-2**, and the guide side of it is **Edit A**.

Fog Bank survived that turn, so Rorix did not attack; at t16 the seat had to spend Hammer on the
Fog Bank instead of a face, and traded Rorix into Master of the Feast. From t18 the deck had no
board at all and ground out land kills while its own draw step killed it.

The late residual, which I do NOT price to the result: at **seq 84** (t26 upkeep, 3 life, opponent
9, 11 untapped sources, Hammer in the graveyard) the menu offered `1. Put a card into hand with
Hammer of Bogardan [cost: {2}{r}{r}{r}]` and the seat took **Hold priority**. The Hammer return is
upkeep-only, so a hold there spends the ability for the turn. The guide sent it there: the trend
line read `unchanged at 9 since turn 24`, and the wave-77 gate closes the return at a zero total,
while the EMPTY-HAND rule that would reopen it requires a MINUS. At **seq 87** it then cast Stone
Rain at `Your life: 1` — THE LAND PLAN's own `L at 8 or less is DEAD` verdict, violated with no
excuse. It was dead on the next draw step either way. → **Edit B** (narrow).

### vs 146 — LOSS on turn 16 at 0; killed by a dungeon room, not by a decision
`1789189702-...-vs-ai_baka_deck146.jsonl`. Nadaar + Lost Mine of Phandelver + three Silverquill
Silencers. The seat drew its 5th land on **turn 14** while holding three Siege-Gang Commanders (5
mana) and two Rorix (6) from turn 8 — turn 12 produced **no window at all**, because nothing in hand
was castable at four lands. It took 3+4+4+4+4 = 19 unanswered damage over turns 7-13.
The blocks it did get are sound: at **seq 30** (1 life, 12 inbound, `best case with every blocker
assigned: you would be at 1`) it answered `BLOCKS: B1:A3, B2:A2, B3:A1, B4:A1`, covering all three
attackers and taking 0. It survived. At **seq 31** (t16) it cast the second Siege-Gang; the
alternative row, Molten Rain, was stamped `NAMED BY THEIR Silverquill Silencer #1 ... you would be
at -2; this KILLS you`, so the cast was forced and correct. It died on turn 17 during the opponent's
**Blockers** step, before any block could be declared, to `dark pool (room 5 of 7) {room effect:
Each opponent loses 1 life and you gain 1 life}` while holding three untapped Goblins.
**One misplay, cost 0:** at seq 30 the fourth blocker (`B4:A1`) added nothing — two 1/1s on a 4/4
Nadaar is 2 damage, and the header's own best-case line assigned only three blockers — so a body
that would have survived was spent. It did not matter: death came by drain, not by combat.

### vs 152 — WIN on turn 21. The X walk-down and a clean Rorix clock
`1789189675-...`. **seq 30** is the wave-78 Edit C rule working: rows read `X=1 {same kills as X=3,
for 2 less mana}` / `X=2 {same kills as X=3, for 1 less mana}` / `X=3 {kills THEIRS: Katilda,
Dawnhart Prime}` and the seat took **X=1**, walking the run to its lowest. Rorix from t18 closed it.
At **seqs 52/54** (11 life, opponent **6**) the seat PASSED a face row `Deal 2 damage with
Siege-Gang Commander targeting the opponent {they would be at 4}` under a printed CRACK-BACK — the
guide's ≤6 gate was open there and it declined; it won anyway on the swing.

### vs 123 — WIN on turn 19. The card that lost wave 78's 123 game, answered on turn 10
`1789189692-...`. At **seq 26** (t10) Starstorm `X = 3 {kills THEIRS: Bloodline Keeper; YOURS:
none}` — the Keeper that built a 24-Vampire board last wave never got a turn. Two Rorix and a
Siege-Gang closed it; **seqs 45/46/47** are three face sacrifices at opponent 5 → 3 → 1 → `-1; THIS
WINS THE GAME`, every one inside the guide's ≤6 gate.

### vs 125 — WIN on turn 17. Uncontested
`1789189680-...`. Rorix on t14 behind three Goblins; the opponent never stabilised. The one thing
worth recording is **seqs 56-64**, nine consecutive `Cast nothing right now` answers across two
turns at an unchanged board — see §2 LOW-1.

### vs 126 — WIN on turn 35. Goblin sacrifices raced the Sanguine Bond / Exquisite Blood loop
`1789189696-...`. At **seqs 56/57** (15 life, opponent **13** then **11**, their line `0 of them
without a restriction against attacking`) the seat sacrificed two Goblins at the face, its stated
reason being *"before they can close the Sanguine Bond/Exquisite Blood loop."* **That is a plain
violation of the live guide's face-row gate** (`while "Opponent life:" is 7 or more you do not take
it`) and of the sacrifice rule under it — in a game it went on to win, against the archetype that
beat it last wave with the same loop. I do **not** edit the rule on this: the violation has no
price attached, and one win is not evidence the threshold is wrong. Recorded so the next seat sees
it if it recurs.

---

## (2) Engine / interface / card items

### HIGH-1 — the plan echo marks a cast DONE one window before it is cast, and strips it out of the plan the completing menu shows
Repro `...vs-ai_baka_deck162.jsonl` **seq 31 → 32**. The seat's reply at seq 31 is
`PLAN: Cast Starstorm X=2 to kill Fog Bank, then attack with Rorix in second main phase.`
The prompt at seq 32 — the `Choose an option for Starstorm` menu that COMPLETES that cast — prints
`THE PLAN YOU LAST STATED (as you stated it, 1 window ago on turn 14) reads "then attack with Rorix
in second main phase."` The record at seq 31 carries `plan_step_count 2, plan_steps_done 1`: the
trimmer counted "Cast Starstorm" as executed at the CAST-ROW answer, when the cast is not complete
until the next window. The seat then declined the cast it had paid six sources for.
This is not a rare shape. Over the six games **120 of 275** comparable prompts echo a plan with its
leading clause removed, and the cut is by sentence/clause boundary, not by what was actually done:
`152` seq 46 `"Decline the block to preserve Dwarven Blastminer ...; taking 3 damage is safe."` is
echoed as `"taking 3 damage is safe."`; `162` seq 56 `"Opponent has 12 life and two 0/6 defenders;
Hammer of Bogardan cannot kill them ..."` loses the board description that the rest of the sentence
depends on. **Fix shape:** a step is DONE when the action resolves, not when its first menu is
answered; and a completing menu (`Choose an option for <card>`, the X menu, the target menu) must
echo the plan UNTRIMMED, because the clause that names the cast is the only thing on that screen
that says which row finishes it. **This is the one item in this seat that decided a game.**

### HIGH-2 — a cycling row's `[DRAW PRICE: ...]` prices the DRAW half only, while the DISCARD punisher is printed on the same screen
Repro `...vs-ai_baka_deck162.jsonl` **seq 35** (and **37**, the second copy). Same prompt:
```
DISCARD PUNISHERS on the battlefield: theirs - Liliana's Caress. Every card YOU discard costs you
2 life to theirs. ... They fire on a discard cost, ...
1. cycling with Starstorm [cost: {3}, Cycle] [DRAW PRICE: this draws 1 card, and the opponent's
   Underworld Dreams punishes every draw, so taking it costs you 1 life right now - you would be at 18]
```
Cycling is `{3}, **Discard this card**: Draw a card` — the row's own cost pays the discard punisher,
and the engine's own DISCARD PUNISHERS paragraph says so ("They fire on a discard cost"). The
printed figure was 1; the charged figure was 3. The model's reasoning at seq 35 reads
*"Check life: 19 -> 18."* — it used the number the row gave it. Two cycles took 19 → 13.
Fix shape: the `[DRAW PRICE:]` bracket on any row whose cost is a discard must fold the discard
punishers it already enumerates, and name both halves (`the discard costs N, the draw costs M`).
The same applies to Forgotten Cave's cycling row and to any future discard-cost ability.
The DRAW half alone is rendered correctly elsewhere in the seat: `162` seq 68 and 84 print
`Underworld Dreams #1, Underworld Dreams #2 punish every draw, so taking it costs you 2 life`, both
true for two copies.

### MED-1 — T9's sacrifice-row blocker pricing never reaches the rows this deck actually takes
T9's fix (`crackBackReliefClause`, `sacrificeGiveBack`) is attached to the `{removes N from the
CRACK-BACK total above}` relief clause, so it fires only on a sacrifice row that KILLS AN OPPOSING
ATTACKER. **No such row appeared in this seat this wave**, so the wired clause is **UNTESTED here**.
What did appear, 125 sacrifice-cost rows, is the other half: `152` **seqs 52/54** print
`8. Deal 2 damage with Siege-Gang Commander targeting the opponent {right now: takes 2 damage -
they would be at 4} [cost: {1}{r}, Sacrifice]` on a window whose situation block carries
`CRACK-BACK NEXT TURN: 1 of their creatures will be able to attack ... for up to 4 - you would be
at 7 or lower`, and the row prices **nothing** about the blocker the Sacrifice spends. Same at
`126` seqs 55-58 and `123` seqs 45-47. The wave-78 loss was a FACE-row take, not an attacker-kill
take, so the rows that lost that game are still the rows with no blocker arithmetic on them.
Fix shape: attach the spent-blocker clause to the SACRIFICE COST, not to the relief clause — any
`[cost: ..., Sacrifice]` row on a window that prints a CRACK-BACK line, whatever the row targets.

### MED-2 — the modal `Decline - do nothing` row does not say the paid mana is stranded; the X menu's decline row does
`162` **seq 32**, row 3 is bare: `3. Decline - do nothing`. Six sources were already tapped. The
X menu two seams away already solves this, and its wording is the model to copy — `123` **seqs
26/33**, row 5: `Decline - do not cast this after all (the announcement is cancelled and the card
stays in your hand) {declining strands the 5 mana already paid: the card comes back to your hand,
your sources STAY TAPPED, and the 5 mana stays floating in your pool. No row on this list can spend
it ...}`. The `Choose an option for <card>` menu's header does say "every other row abandons the
cast you just chose", but says nothing about the payment, and the seat's reasoning shows the gap
exactly: *"The 6 mana in pool might be lost, but that's fine."*

### MED-3 — `plan_line_missing` is the dropped `PLAN:` LABEL, 17 of 18, ninth wave unchanged
17 of the 18 `protocol_deviation` replies carry a first line that IS a plan, without the label:
`152` seq 24 `"\n\nCast Siege-Gang Commander.\nCHOICE: 1 (Play Mountain)"`; `152` seq 28
`"\n\nClear their board to deploy Siege-Gang Commander.\nCHOICE: 1 (Cast Starstorm)"`; `162` seq 43
`"\n\nKill Master of the Feast.\nATTACK: A1"`. The 18th, `162` **seq 42**, is the seat's one
`plan_absent`: the whole reply is `"\n\nCHOICE: 3 (Fog Bank (0/2) [flying, defender] [opponent's
battlefield])"`. Accepting an unlabelled first line as the plan still zeroes 17 of 18.

### LOW-1 — nine consecutive `Cast nothing right now` answers at an unchanged board, with no hold row taken
`...vs-ai_baka_deck125.jsonl` **seqs 56-64** (t16-t17, life 20/7 throughout). Each window re-put the
same list and the seat answered the non-closing decline each time rather than the hold row, spending
nine model round trips on a board that did not move. The seat DID take the hold row at seqs 64 and
67, so the row was present and understood; the shape is the cost of the decline row reading as the
safe answer. `crossphase_identical_reputs` for that game is 10 and
`menu_pass_no_progress_suppressed` 0 there, so the suppressor did not reach it.

### LOW-2 — `forced_close_unrecorded` 2 of 4, still unattributable from a seat's side
4 `forced_close` records (`123` seqs 20/21, `126` seqs 41/44), empty replies, against
`force_close_arms_refused` / `_deferred` / `_same_arm_deferred` / `_defer_bound_hits` all **0** for
the sixth wave. 0 fallbacks and 0 `phase2_answer_missing` are consistent with no decision lost;
unconfirmed from this side, the stderr being shared with the opponent seat.

### T-item checks this seat could make
- **T10 (narration bytes) — PASS, and better than predicted.** Max prompt **22,357 B** (wave 78:
  29,641; DB predicted ≈22 KB — exact), median 10,456, 11 of 322 over 20 KB (was 77 of 321). The
  largest GAME LOG segment in the seat is **13,512 B**, i.e. below the new 16 KB trigger, so the
  trim never fired here and **nothing was cut**: no prompt in the seat lost its own last PLAN (the
  28 prompts with no `THE PLAN YOU LAST STATED` block are all first-windows or windows whose prior
  reply carried no plan, not trims), no stated stop, no loop state. The hoist is also clean: I
  searched all 322 prompts for an identical `{card text: ...}` block appearing twice in one prompt
  and found **zero**.
- **T16 (`ON THE STACK` folds queued punisher damage) — UNTESTED here, state never arose.** Four
  `ON THE STACK: N damage to you` lines in the seat (`162` seqs 27/53/70/85), and every
  one of them has a single `Underworld Dreams's deal 1 damage` trigger and NOTHING else on the
  stack; each subtraction against the life line is correct (20→19, 6→5, 4→3, 2→1). No window in this
  seat ever had a queued DRAW trigger sitting under a punisher, which is the shape T16 fixes.
- **T9 — UNTESTED here** (see MED-1); the half that did render is unchanged from wave 78.
- **`[hold check:` brackets** — present on every re-put window I read, and `hold_check_ref_seq`
  resolves to a record in the same log everywhere I spot-checked (`146` seq 28 `4 windows in a row`,
  `162` seq 68 `7 windows in a row`). I did not audit all of them (see "did NOT check").
- **`Opponent life trend:` gained/lost split** — present and arithmetically true in every line I
  read. In the 162 game the gain stream is **+0 in every window**, so NET and GAINED are the same
  number there: **the wave-78 seat's flagged question has no instance in this corpus** and the
  seq-84 failure is NOT the NET-vs-GAINED axis — it is the ZERO-closes clause itself. Edit B says so.
- **`{X}-announcement payment loss`** — 0 `dropped after its X was announced` in the seat's stderr;
  the seq-32 abandonment is the adjacent shape at the MODAL menu, not the X menu (MED-2).

### Card facts — verified, no item
Underworld Dreams (1 damage per opponent draw, stacks — the two-copy summary is right), Liliana's
Caress (2 life per opponent discard; fires on a cycling cost — the engine's own paragraph says so,
its row does not), Fog Bank, Shield Sphere, Master of the Feast, Howling Mine-free this time,
Nadaar Selfless Paladin + Lost Mine of Phandelver (venture rooms, `dark pool` = each opponent loses
1 and you gain 1 — that is the killing blow in `146`), Silverquill Silencer (name a nonland card;
the named caster loses 3 and they draw — per cast, both copies charged), Vanishing Verse, Hive of
the Eye Tyrant, Grimclimb/Brightclimb Pathway, Bloodline Keeper, Thraben Doomsayer, Katilda,
Augur of Autumn, Brutal Cathar, Intrepid Adversary, Chromatic Lantern, Sanguine Bond + Exquisite
Blood. Own side: Starstorm (X to EACH creature, cycling {3}), Hammer of Bogardan (3 to any target,
upkeep-only {2}{R}{R}{R} return), Siege-Gang Commander, Rorix, Dwarven Blastminer (nonbasic only),
Molten Rain, Stone Rain, Lay Waste, Spark Spray, Pyrite Spellbomb, Talisman of Impulse, Forgotten
Cave. No Scryfall disagreement worth reporting.

---

## (3) Guide verdict: **EDIT** — revised guide at `wave79/deck130/strategy.txt`

**19,933 bytes vs the live 19,959 — net −26**, two edits (+513) paid by five trims (−539). Every
trim removes text that a rule above absorbs or that the row prints itself. No addition is a
concession and each is a precondition (A334); no citation, count, or history is in the guide
(A332); headings are scope declarations (A333) and the one heading that changed did so BECAUSE its
scope grew; the PLAN template is untouched and nothing quotes "YOUR PLAN".

**The edit that grows the guide earns its bytes this way:** Edit A is the only rule whose absence is
a directly-priced loss this wave — 6 of 20 life, on the deck's one decision-priced defeat — and it
is a rule the engine's own row cannot supply, because the row prints the wrong number (§2 HIGH-2).

**Edit A — a cycle is a DISCARD as well as a draw, and the row's price counts only the draw.** (+377)
The heading changes because its scope does:
Before: `=== DRAW PUNISHERS - READ BEFORE ANY OPTIONAL DRAW ===`
After: `=== DRAW AND DISCARD PUNISHERS - READ BEFORE ANY OPTIONAL DRAW ===`
and, before the existing `Killing a punisher outranks casting a creature` sentence:
`A CYCLE IS A DISCARD AS WELL AS A DRAW. When the frame also prints "DISCARD PUNISHERS on the
battlefield ... Every card YOU discard costs you N life", add that N to the draw N before both
tests above: a cycling row's own "[DRAW PRICE: ... costs you N life right now]" prices the DRAW
half alone and is never the price of the cycle.`
Plus, in the CYCLING heading (where a model reading about cycling arrives), the timing rule that
until now lived only in ORDER OF OPERATIONS, with the twice-a-turn clause restored:
`CYCLE IN YOUR SECOND MAIN PHASE ONLY - never at your own upkeep, and never twice in one turn.`
Paid by `162` **seqs 35 and 37** — the seat cycled at its own upkeep, twice, on a board with N=1 on
the draw line and a 2-life discard punisher the guide never mentioned, reasoning verbatim *"Check
life: 19 -> 18."* against a real charge of 3. The existing rules (`N is 1 and "Your life:" above 5
-> you may cycle`, and the upkeep ban two headings away) both said the wrong thing or said it
somewhere the model was not reading. The twice-a-turn clause was trimmed in wave 78 as
never-violated; it is now violated and priced, so it comes back.

**Edit B — the Hammer return's EMPTY-HAND rule overrides a ZERO total, and the upkeep is the only window.** (+136)
Before: `THE EMPTY-HAND RULE, at a MINUS total: when "Your hand" reads "(0 cards): (none)" - or holds
only a land spell the land plan has killed - and "Mana available:" is 8 or more, TAKE THE RETURN
EVERY UPKEEP and recast in main phase 1.`
After: `THE EMPTY-HAND RULE OVERRIDES A ZERO TOTAL, never a PLUS one: when "Your hand" reads "(0
cards): (none)" - or holds only a land spell the land plan has killed - and "Mana available:" is 8
or more, TAKE THE RETURN EVERY UPKEEP and recast in main phase 1. Your upkeep is that ability's only
window in the whole turn: a hold taken there spends it.`
Paid by `162` **seq 84**: 3 life, opponent 9, 11 untapped sources, hand = one Stone Rain whose land
plan its own `L at 8 or less is DEAD` verdict had already killed, trend `unchanged at 9` — every
condition of EMPTY-HAND met except the MINUS, so the gate closed the seat's only remaining reach and
it took the hold row on a once-per-turn upkeep ability. The PLUS closure is untouched (a gaining
opponent still closes it); only the zero case moves, and only when the hand is already empty of
anything the mana can buy.

**Bytes trimmed to pay for the above (−539):**
`NEVER SPEND DAMAGE ON A SURVIVOR: chip damage on a body you cannot kill buys nothing and the card
is gone.` (−110) — TARGETING step 2 three lines above already makes `"- SURVIVES (toughness N)"` the
row you decline, and the seat declined every SURVIVES row it was offered this wave;
`Exceptions: "SURVIVES (loyalty 3, 1 left)" is a planeswalker and IS a take.` (−67) — TARGETING
step 1 already aims at a planeswalker ahead of any creature and reads its loyalty counter;
the LIFEGAIN paragraph's tail `On a board that turns your life into their damage, treat every
"paying this costs you N life" bracket - the Talisman's own 1 among them - as lethal, not
expensive.` (−160) — the TALISMAN OF IMPULSE entry states the same rule for the only card in the
deck it applies to (`At 3 life or less, and on any life-loop board, that row is "Cast nothing right
now"`);
STARSTORM'S X (c)'s carve-out `A sweep whose THEIRS list names EVERY creature on their line is a
different row: it empties the board those blockers were for, so cast THAT and let the body wait.`
(−157) — a concessive clause for the rule's hard case with no instance in four waves (A334);
and ORDER OF OPERATIONS' `Cycle in your SECOND MAIN PHASE only, never at your own upkeep.` (−58),
which is not lost but MOVED into the CYCLING heading by Edit A.

### What the wave-78 edits decided
- **Edit A (the face row as a PROHIBITION, not a claim about the menu)** — **PASS where it was
  tested against the gate's own threshold, and openly overridden once.** `123` seqs 45/46/47 took
  the face at opponent 5 → 3 → 1 (inside the gate, and the last one won the game); `152` seqs 52/54
  DECLINED the face at opponent 6 with a crack-back printed. The override is `126` seqs 56/57 at
  opponent 13 and 11, reasoned as a race against the Bond/Blood loop, in a game the seat won. The
  rule no longer stakes itself on a false statement about the menu, which is what wave 78 bought;
  whether the threshold is right on a combo board is now an open question with one data point.
- **Edit B (every sacrifice spends a blocker)** — **UNTESTED.** No sacrifice was offered under a
  CRACK-BACK line the seat's bodies were needed against; the 152/126/123 sacrifices are all on
  boards whose opposing line reads `0 of them without a restriction against attacking`.
- **Edit C (the X-menu marker is read for WHAT IT SAYS; largest-X fallback gated)** — **PASS on the
  fallback half, one soft miss.** `152` seq 30 walked a `same kills as X=3, for 2 less mana` run
  down to X=1; `152` seq 27 and `123` seq 26 took the only killing X. `123` **seq 33** took X=3
  where row 2 read `same kills as X=3, for 1 less mana` — the walk-down says take the lowest of the
  run; it cost 1 floating mana and nothing else. No X menu in the seat carried a marker at all this
  wave (the markers sat on the CAST row), so the marker half of the edit is **UNTESTED**.
- **Wave-78 HIGH-1 (a sacrifice row prices the damage it removes, not the blocker it spends)** —
  the fix is wired but **UNTESTED here**, and the residual on face/own-target sacrifice rows stands
  (§2 MED-1).
- **Wave-78 MED-1 (prompt bytes / narration)** — **CLOSED in this seat** (§2 T10).
- **Wave-78 MED-2 (`ON THE STACK` excludes queued punisher damage)** — **UNTESTED here** (§2 T16).
- **Wave-78 MED-3 (the dropped `PLAN:` label)** — **recurs**, 17 of 18 (§2 MED-3).
- **Wave-78 MED-4 (reasoning degenerating into output-formatting chatter)** — **recurs and is now
  next to a lost game**: `162` seq 32's 10,569 reasoning characters end in ~30 lines of
  `"Output matches. Proceeds. Done. [Final Check] ... Ready."` after the decision, and seq 35 the
  same. Both are the turn-14 windows that cost 6 life. Recorded, not an item: no render is false.
- **Wave-77 residual (the Hammer gate keys on the NET figure, not GAINED)** — **decided, with the
  split printed**: in the only game where the gate fired, gain EVENTS were **+0 in every window**,
  so NET and GAINED were identical and the distinction had no instance. The gate's failure at `162`
  seq 84 is the ZERO-closes clause, and that is what Edit B moves. I did not touch the NET figure.

### Rejected, though I looked at them
- **A rule about racing a static-damage enchantment instead of killing lands** (`162` t18-t26: three
  land kills and one face Hammer while the opponent sat at 9-14 with 3 lands and needed none of
  them). THE LAND PLAN already kills itself on `L at 8 or less is DEAD` and the seat simply ignored
  the verdict at seq 87; a new heading restating a verdict already in print is not the fix.
- **Re-stating the `"CHOOSE AN OPTION FOR <card>"` rule.** It is in the guide, correctly worded, and
  the menu header says it verbatim. The seat broke it because the plan echo had deleted its own
  intent (§2 HIGH-1). That is engine work, not guide bytes.
- **A blocker-economy rule off `146` seq 30's fourth blocker.** One instance, cost 0, and the
  header's own best-case line already names the minimal assignment.
- **Moving the body ahead of land destruction in ORDER OF OPERATIONS.** Same as last wave: in `146`
  the body rows read `[cannot pay now: needs 5 mana, you have 4 untapped sources]` at every window,
  which is mana, not instruction. No window to price it with.

## (4) Optional proposals
**None.** The two generalisable findings are engine work and are stated as engine items: §2 HIGH-1
(a plan step marked done at the first menu of a multi-window cast, and the completing menu echoing
the trimmed plan — this is not deck130-specific; any deck with a modal, X or target sub-menu reads
the same truncated intent) and §2 HIGH-2 (a cost-side punisher the row's own price bracket omits
while the frame enumerates it two paragraphs above). Nothing this seat saw contradicts an existing
amendment or needs a new one.

## What I did NOT check
- **The opponent seats' JSONL, except one.** I opened `146`'s opponent log to establish how the seat
  died on turn 17 (the `dark pool` room) because my own log ends before it. Every other board fact
  is read from my own rendered prompts.
- **Any engine behaviour beyond one read of `crackBackReliefClause`'s comment block** (to establish
  T9's scope for MED-1). Every other claim about a counter, a skipped window, a trim trigger or an
  unfired instrument is a transcript observation.
- **All 6 `async_drops`, the 30 `ask_replays_reserved`, the 6 `ask_key_continuation_differs` and the
  2 `identical_option_asks_resolved` individually.** 0 fallbacks and 0 `phase2_answer_missing` are
  consistent with no decision lost; unconfirmed. The `askreplay/` sidecar I did not open.
- **A full audit of the `[hold check:` counts.** I read the brackets on the windows I traced and
  spot-checked their `hold_check_ref_seq`; I did not reconstruct the engine's row identity across
  all 322 windows, and I make no claim about count accuracy this wave.
- **Whether `own_loop_*`, `stop_reached_*`, `hold_verdict_safer_ignored`, `hold_reopened_new_threat`,
  `crossphase_board_unchanged`, `blocker_forecast_gang` and `mana_only_windows_skipped` being 0 in
  this seat are unwired counters or honest zeroes.** No instance either way; the corpus-wide figures
  are non-zero for several of them, so the seat is the wrong instrument.
- **`{effect:}` gloss cuts.** I read the glosses inside the windows I traced and saw no mid-clause
  cut, but I did not sample 30 and make no PASS claim on T11/DC F7.
- **Whether 4/6 is a real strength change.** deck130 has now run 4 → 1 → 0 → 4 across four waves on
  the same archetype; the hands here were good (Rorix on time in four games, five lands by t14 in
  neither loss) and I priced exactly one loss to a decision.
