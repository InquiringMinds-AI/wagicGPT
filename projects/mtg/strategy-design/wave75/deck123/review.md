# Wave 75 — deck 123 (Intruders of Thraben, WBU token combo) — per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-003556-final`, SINGLE binary
`archives/wagic-7a04cd6da-w75step1` (source 14fea99f3), `--thinking on`. Live guide = the wave-74
edition, installed at cce04d4af. **1 win (vs126 t22), 5 losses (vs152 t11, vs162 t13, vs146 t21,
vs130 t26, vs125 t83)** — wave 74 was 3/6.

Seat census: **976 decisions** (125: 588, 126: 142, 130: 101, 146: 68, 162: 47, 152: 30), 100%
native reasoning, 0 fallbacks of any kind, 0 `reply_truncated`, **0 `answer_replaced` and 0
`[RE-ASK]` renders in any of the six games**. Gameend sums for this seat: `ask_replays_reserved`
952, `identical_ask_answers_reserved` 664, `hold_released_turn` 41 (125: 31, 130: 5, 126: 5),
`hold_windows_skipped` 475 (cast 368 / priority 107), `stop_reached_windows_skipped` 19 (all 125),
`main_phase_windows_skipped` **0**, `own_turn_windows_skipped` 6, `reserve_decline_windows_noted` 15
(162: 7, 125: 7, 126: 1) / skipped 2, `plan_names_stranded_card` 2 (162), `plan_names_uncastable_
zone_card` 21, `async_drops` 32, `phase2_answer_recovered` 5 / `missing` 0, `forced_close_
unrecorded` 3 (125: 2, 162: 1), `chain_acting_rows_cast` 944 / `chain_selfharm_rows_cast` 577,
`repeat_annotated_takes` 0, `sibling_window_asks_skipped` 0, `chain_windows_collapsed` 0.

**Hands are real.** Three of the five losses (146, 130, 125) are games in which this seat's own
battlefield line read `of which 0 are creatures` for the whole game or for 50 consecutive turns.
The 3/6 → 1/6 swing is not a guide effect and I do not score it as one. What I do score is that in
those same games the seat spent castable cards on rows whose own printed text refused them.

## 1. Game by game — what decided each result

**vs deck126 — WIN, turn 22 (20 / −30).** The clean execution. Keeper out, one Alarm cast at
**seq 91** with `of which 1 is a creature` (legal P3), lone-maker attack correctly declined twice
under CHECK -1 (**seq 97**, **seq 206**), the **seq 206** decline reasoned explicitly off the
Sanguine Bond + Exquisite Blood loop board (CHECK 1 B2 working), then 26 fliers for lethal at
**seq 257**. CHECK -1, CHECK 1 B2 and the stop all fired correctly in the same game.

**vs deck152 — LOSS, turn 11 (0 / 23). The deciding decision is seq 40 and it is a STRATEGY error
against a printed per-attacker tag.** Board: one Human token and Thraben Doomsayer, the seat's only
maker, wearing the Lightning Greaves it had cast and equipped that same turn (**seq 37**, **seq
38**). The attackers window listed exactly two rows and both carried the same tag:
`A1. Human (1/1) [their untapped blockers: Brutal Cathar (3/3) (your attacker dies, their blocker
lives)]` / `A2. Thraben Doomsayer (2/2) [haste, shroud] [their untapped blockers: Brutal Cathar
(3/3) (your attacker dies, their blocker lives)]`. The reply was `PLAN: Attack with all available
creatures to maximize damage and apply pressure. / ATTACK: A1, A2`. The **seq 41** events read
`Opponent's Brutal Cathar dealt 3 damage to Thraben Doomsayer` ... `Your Thraben Doomsayer died`.
One damage got through and the deck's engine was gone; the game ended the next turn.
**Nothing in the guide forbade it.** CHECK -1 needs a list of exactly ONE creature (this list had
two). CHECK 2 needs the GANG BLOCK clause or 2+ possible blockers (there was one). CHECK 3 fired
only when `their untapped count is at or above what you send` — 1 against 2, so CHECK 3 *permitted*
the attack. CHECK 4's only prohibition was `never into "(both die)"`, and this tag is strictly worse
than that one. Edit 3 exists for this seq. Second, smaller: **seq 21** cast Tragic Slip on a row
reading `{kills 0 of the 1 CREATURE target at -1/-1}` against a 3/3 Luminarch Aspirant — the
printed gate the guide already carries, broken anyway.

**vs deck162 — LOSS, turn 13 (0 / 21). Killed by the opponent's Peer into the Abyss into two
Liliana's Caress plus Underworld Dreams and Ob Nixilis; the seat's own turn 10–12 threw away three
cards and its board first.** In order:
- **seq 30 (t10, DRAW step)** cast Tribute to Hunger on a row printing
  `{reserve: ... VERDICT: taking this row STRANDS Bloodline Keeper this turn - 1 source cannot pay 4,
  and it has no window after your main phase}`, and its own PLAN on that same line read
  *"In Main Phase 1, play Marsh Flats, cast Bloodline Keeper, tap it for a 2/2 Vampire token..."*.
  The row named the card, the plan named the card, and the row was taken anyway.
- **seq 31 (same DRAW step)** cast Tragic Slip on `{kills 0 of the 2 CREATURE targets at -1/-1}`,
  then at **seq 32** targeted the Fog Bank row reading
  `{right now: -1/-1 - SURVIVES (toughness 2, -1/-1 more kills it)}`. Two printed gates, both in
  the live guide, both broken in one card.
- **seq 39 (t12, DRAW step)** cast Devour Flesh naming **Yourself** at **seq 40**, sacrificing the
  Vampire token that was its ONLY creature, for 2 life at 10 — on a row that again printed
  `VERDICT: taking this row STRANDS Bloodline Keeper this turn`.
- **seq 45 (t12, Main 1)** then cast Intruder Alarm into `of which 0 are creatures`, tapping out.
  The live guide says verbatim that with that line "the Alarm is not a play at any price".
  From **seq 46** on the seat has 0 sources and no creature; **seq 52–57** are six passes narrating
  its own death to the draw triggers.

**vs deck146 — LOSS, turn 21 (−9 / 20). Variance, with two P3 breaches inside it.** The seat's
battlefield line reads `of which 0 are creatures` at every window from **seq 5** to the end: the
turn-8 Doomsayer (**seq 34**) died and nothing replaced it. It nonetheless cast Intruder Alarm at
**seq 74 (t16)** and a second Intruder Alarm at **seq 97 (t20, at 3 life)**, both on
`of which 0 are creatures` boards. Damnation at **seq 92** (`destroys 3 of their creatures (3
without a restriction), 0 of yours`) is a correct cast. **seq 35 (t10, Upkeep)** is a third
reserve-VERDICT take (Tribute to Hunger, `STRANDS Idyllic Tutor this turn`) with the plan's own next
step reading "proceed to main phase to ... tutor for Intruder Alarm".

**vs deck130 — LOSS, turn 26 (−2 / 26). Variance (never one creature all game), plus both edict
gates.** `of which 0 are creatures` at every single window of the game; killed by recurring Hammer
of Bogardan. Breaches: **seq 81 (t13)** Intruder Alarm on a 0-creature board, tapping out;
**seq 138 (t20)** Devour Flesh at the opponent on `they control 4 creatures - they choose which
one`; **seq 140 (t21)** Tribute to Hunger on `they control 2 creatures - they choose which one -
you gain at least 1`, which also printed `VERDICT: taking this row STRANDS Idyllic Tutor this turn`.
Opponent life went 25 → 26 across the pair.

**vs deck125 — LOSS, turn 83 (0 / 97). The corpus's marathon, and the seat's largest single misplay
is seq 1208.** Deck125 showed two **Lightmine Field** and two Staff of Nin. The seat read the
Fields correctly for sixty-eight turns: **seq 106, 158, 187, 233, 290, 329, 376** are all
`ATTACK: none` with reasoning that names the Field and the board wipe it would cause. Its board was
swept at t28 and it sat with `of which 0 are creatures` from t28 to t66. At t76–78 it rebuilt to
100 creatures behind a transformed Lord of Lineage.
- At **seq 1208 (t78)** the window rendered, truthfully:
  `ATTACK TOTAL: 100 attackers listed, 401 total combat damage to a player - declaring all of them
  with none blocked puts them at -304. At least 401 damage lands whatever they block - they would be
  at -304. Both figures are BEFORE their attack triggers: Lightmine Field, Lightmine Field fires on
  the declaration, before any combat damage, and can kill your attackers first - how much it deals
  depends on how many you declare, so it is not folded into either number.`
  The reply was `PLAN: Attack with all 100 creatures to deal 401 damage, winning the game. /
  ATTACK: all`, and the reasoning ends *"Lightmine Field interaction acknowledged but irrelevant to
  win condition."*
- **seq 1210**'s events are the whole board dying to the two Fields before combat damage, ending
  `Your Lord of Lineage died`. Opponent life never moved off 97. The seat was dead to Staff of Nin
  pings by t83.
- The information was on screen and correct; the guide is what licensed it. `LETHAL IS ALREADY ON
  THE BOARD` and CHECK 1's `Lethal overrides all three - power at or above their printed life, send
  it all` both told the seat to send everything, and no rule anywhere covered a trigger that fires
  on the declaration and scales with the number declared. The correct declaration was ONE creature
  — Lord of Lineage, 5/5, taking 1 per Field per attacker and surviving. Edit 2 exists for this seq.
  I am not claiming it wins the game; at 5 life against two Staffs it does not. It is the play.

## 2. Engine / interface / card items

**MED-1 — `ATTACK TOTAL`'s "At least 401 damage lands whatever they block" is asserted as a floor on
a board where the same sentence says the attackers may die first.** Repro:
`1789018561-ai_baka_deck123-0x560a35b4ce40-vs-ai_baka_deck125.jsonl` **seq 1208**, the block quoted
above. The clause "At least N damage lands whatever they block - they would be at X" is the render's
strongest lethal statement and it is FALSE in the presence of a declaration-time attack trigger;
the qualifier that repairs it arrives two clauses later and is scoped only to "Both figures are
BEFORE their attack triggers", which reads as an understatement rather than a negation. The model's
reasoning shows exactly that reading. **Suggested fix**: when an attack trigger of this class is on
their board, suppress the "At least N damage lands whatever they block" sentence entirely, or
replace its number with the per-attacker survival arithmetic the trigger implies (`declaring K
attackers deals K damage to each of them from each of the 2 copies; at K=100 nothing you control
survives`). The engine already knows the trigger's scaling — it says so.

**MED-2 — `plan_names_stranded_card` is an under-count of the behaviour it names: the reserve
VERDICT was overridden 4 times in this seat and flagged twice.** Repros, all four with the row's own
`{reserve: ... VERDICT: taking this row STRANDS <card> this turn}` printed on the taken row:
`...vs-ai_baka_deck162.jsonl` **seq 30** (STRANDS Bloodline Keeper) and **seq 39** (STRANDS
Bloodline Keeper); `...vs-ai_baka_deck146.jsonl` **seq 35** (STRANDS Idyllic Tutor);
`...vs-ai_baka_deck130.jsonl` **seq 140** (STRANDS Idyllic Tutor). Only the 162 pair is counted by
`plan_names_stranded_card` (2). In three of the four the PLAN's next step names the very card the
row strands, which is a cheap and exact detector: the counter appears to require the stranded card
to appear in the plan text under a narrower match than "the plan's later steps". Worth widening —
this is a decision-losing class and the instrument currently sees half of it.

**MED-3 — the `[hold check: ...]` bracket now fires on windows separated by a phase boundary, and
"every row above was also on the menu at the last window at this seam (N windows ago)" reaches
N = 17.** Repro: any of the six files; the bracket rendered 858 times in this seat, with 63
instances of `1 row above is new since the last window at this seam`. I found no FALSE claim — I
diffed the row lists behind the 63 "1 row is new" brackets in the 152 and 162 games and each had a
genuinely new or newly-payable row. Reported as a volume observation, not a defect: the prompt-only
`you declined this exact list N times already this turn` note reached **10** (125 seq 279), **9**
(146 seq 28, 130 seq 78, 126 seq 38), **8** (162 seq 57) and **3** (152 seq 19) — well above wave
74's post-CG maximum of 1, though every high-water instance I checked is a legitimately re-put
identical dead list across successive phase seams, not a re-put within one phase.

**LOW-4 — Lightmine Field's damage is applied as N separate 1-damage events rather than one
N-damage event.** Repro: `...vs-ai_baka_deck125.jsonl` **seq 1210** events —
`Opponent's Lightmine Field dealt 1 damage to Lord of Lineage` / `Opponent's Lightmine Field dealt 1
damage to Vampire (x99)`, repeated, with deaths accumulating as the repetitions land. The Oracle
text the prompt itself prints is *"deals damage to each of those creatures equal to the number of
attacking creatures"* — one event of 100, not 100 events of 1. The net outcome here was the same
(everything died), but the shapes differ under damage prevention, damage-triggered abilities, and
lifelink, and the LOG is what the model reads. I did not open the card script; this is diagnosed
from the rendered events only.

**LOW-5 — `sibling_window_asks_skipped` 0 and `chain_windows_collapsed` 0 again, for the same
structural reason as wave 74.** Every chain row this deck offers costs `Tap`, which the mana policy
always affords, so the no-legal-action collapse path is unreachable from deck123; and the cast menus
always carry a `Cast` row the priority menus do not, so no sibling suppression is ever a candidate.
Third wave running. This is a correct zero from this seat, not an untested one.

**Render clauses I checked and found TRUE.**
- **P6 `CRACK-BACK NEXT TURN` on own-turn windows: 47 renders, all own-turn, including 8 at
  Upkeep** (146 seq 29/30/32/35, 152 seq 23, 130 seq 140, 126). Each figure tracks the board:
  146 seq 29 `for up to 1 - you would be at 19` against a lone Triumphant Adventurer 1/1; 146 seq 32
  `for up to 3` after Barrowin of Clan Undurr (3/3) landed. No false instance.
- **P19 hold row size: 866 hold rows, median 593 B, max 723 B, exactly one above 650** (125 seq
  1276). `[HOW A HOLD ENDS:` appears exactly 866 times — one per prompt carrying a hold row.
- **P7/F8 `{crack-back cover: ...}`: 7 renders, 0 `[legendary: you already control ...]`.** The
  152 seq 23 cover (`you cover 3 of 3, leaving 0 -> you would be at 18`) is arithmetically exact
  against the 3-damage floor it quotes, and it labels itself NOT a survival verdict.
- **Every `ATTACK TOTAL` in this seat recomputes true**, including the 401 at 125 seq 1208 (100
  attackers: Lord of Lineage 5 + 99 Vampires at 4 = 401) — the number is right, its *floor* claim
  is what MED-1 is about.

**UNTESTED from this seat (0 renders in 976 decisions):** P5 loop-fed self-damage and its POISON
variant, P10 own-clock gate (`reaches 0 in N more turns`), P11 `another copy of`, P18 MDFC land-drop
faces and `OTHER FACE of` (no deck123 game had an MDFC in hand — fifth wave running), P13
`main_phase_windows_skipped` (0 in all six games), P3 `[cannot pay now]` (deck126's Exquisite Blood
is the opponent's), P14 the repeat-row refusal-and-re-ask (0 `[RE-ASK]` renders and 0
`answer_replaced` — every repeat-row take this seat made carried a well-formed count), P21/F4
`reasoning_forced_close` (0 truncations), ANNOUNCE_X (0 windows).

## 3. Which wave-74 items recur

| wave-74 item | wave-75 status |
|---|---|
| HIGH-1 counter-driven hold re-open (CG binary) | **CLOSED.** Max asks in any (turn, phase) is 19, at 125 turn 77 Upkeep, and every one of the 19 carries a board event (token created, transform, Staff ping). No re-put run anywhere in the seat; the only other >10 is 126 t20 Main 1 at 11. |
| MED-2 the 80-row identical-Equip menu (O18 at the Equip seam) | **Does not recur** — no Greaves-on-a-token menu of that size appeared this corpus. |
| MED-3 the HTTP 400 / `max_tokens_reasoning: 0` fallback | **CLOSED for this seat.** 0 fallbacks of any kind in 976 decisions. |
| LOW-4 pathway `doubleside` parse errors | Not re-checked; no deck123 game had an MDFC in hand. |
| LOW-5 structural zeros (sibling bar, chain collapse) | **RECUR as passes.** |
| the Tragic Slip `kills 0 of the N` cast + `- SURVIVES` target (wave 74, vs126 seq 42/43) | **RECURS, twice** — 152 seq 21 and 162 seq 31/32, despite the wave-74 edit that added the exact string to the ANSWER FORMAT gate list. Restating it a third time is not the fix; see edit 1. |
| the edict gate at N ≠ 1 (wave 74, vs152 seq 51) | **RECURS, twice** — 130 seq 138 (N = 4) and seq 140 (N = 2). |
| the lone-maker attack (CHECK -1) | **Held, 4 of 4** (125 seq 50, 152 seq 29, 126 seq 97, 126 seq 206). The maker was still lost at 152 seq 40, on a two-creature list CHECK -1 does not reach. |
| `LETHAL IS ALREADY ON THE BOARD` (the wave-74 addition) | Fired correctly at 126 seq 257; fired WRONGLY at 125 seq 1208, where the board's lethal was cancelled by an attack trigger the rule did not know about. Edit 2 closes it. |
| — new — | the reserve-VERDICT override (4×), the Alarm-on-an-empty-board cast (4×), the attacker-dies tag (1×, decisive), the all-in into a scaling attack trigger (1×, decisive). |

## 4. Guide verdict: **EDIT**

Revised guide at `wave75/deck123/strategy.txt` — **19,995 bytes** (live guide 19,943; ceiling
20,000). A332 grep: 0 hits. `YOUR PLAN`: 0. `even when` / `even if` / `regardless of` / `no matter`
(A334 audit): 0. Both PLAN templates are one physical line each. Headings declare scope (A333): the
one rule that must hold at every seam, `THE PRINTED STRINGS THAT ANSWER THEIR OWN ROW`, names its
own scope in its first clause and is not filed under any decision-kind heading.

**Which rules earned bytes.** The guide grows by 52 bytes net and adds ~2,400 bytes of new rule; the
rest is paid by deleting rules, not by shaving prose. The four new rules are edits 1–4 below and
each is paid by a decision in this corpus that went the other way.

| # | before → after | seq that paid for it |
|---|---|---|
| 1 | **NEW block `THE PRINTED STRINGS THAT ANSWER THEIR OWN ROW`**, immediately under ANSWER FORMAT, above every decision-kind section: one first-match list of the exact strings that kill a row — `kills 0 of the N`, `- SURVIVES`, `they choose which one` / `at 0 this does nothing`, `of which 0 are creatures`, `destroys 0 of their creatures` / `a second copy changes nothing` / `this equips nothing`, the reserve VERDICT, and the attacker-dies tag. **Their per-section restatements are DELETED** — the Slip bullet, the edicts' N paragraph, P3's `0 are creatures` clause, P8's dead-row list and the Greaves' `this equips nothing` sentence all now defer to it. | **152 seq 21**, **162 seq 31/32** (Slip on `kills 0`, target on `- SURVIVES`); **130 seq 138/140** (edict at N = 4 and N = 2); **146 seq 74/97**, **130 seq 81**, **162 seq 45** (Alarm on a 0-creature board). All eight strings were already in the guide, all eight in their card's own section — where a *casting* menu of six rows never reaches them. Net after deletions: **+~150**. |
| 2 | **NEW `CHECK 0` sub-rule, `AN ATTACK TRIGGER ON THEIR SIDE`**, first item under CHECK 0 and therefore above every send-all instruction: a trigger the render says "fires on the declaration, before any combat damage, and can kill your attackers first" makes the ATTACK TOTAL neither lethal nor a floor, and scales with how many you declare — send ONE, the most power that survives one copy of the trigger per copy on their board, and send one again next turn. `LETHAL IS ALREADY ON THE BOARD` and CHECK 1's `Lethal overrides all three` each gain the inline carve-out that points here. | **125 seq 1208 → 1210**: 100 attackers declared into two Lightmine Fields, whole board destroyed before combat damage, opponent life unmoved at 97. **+~600**. |
| 3 | **NEW `CHECK 0` sub-rule, `THE TAG ON YOUR OWN A-ROW`**: `(your attacker dies, their blocker lives)` is that creature's answer — it stays home; a maker, a Greaves wearer or your only creature carrying that tag is never sent, not for pressure, not to force a block, not because the rest of the list is going. CHECK 0 is renamed `WHAT THE DECLARATION COSTS BEFORE ANY DAMAGE IS DEALT` and now holds this, the attack-trigger rule and the old crack-back rule. | **152 seq 40 → 41**: both A-rows carried the tag, both were sent, the Doomsayer died, the game ended two turns later. **+~350**. |
| 4 | **NEW section `YOUR OWN UPKEEP AND DRAW STEP`**, between the cast-priority list and HOLDS: before your own main phase an instant spends the mana your sorcery-speed maker/Alarm/Tutor needs; a row printing `VERDICT: taking this row STRANDS <card> this turn` on YOUR turn is answered `Cast nothing right now`, because the instant keeps its window at the END of their turn — and if the plan's next step names the stranded card, the row is refused, not the plan. The one carve-out (a row whose own text stops something on the stack or on their attack that would kill you this turn) is stated first. | **162 seq 30** (STRANDS Bloodline Keeper, plan's next step names Bloodline Keeper), **162 seq 39**, **146 seq 35** (STRANDS Idyllic Tutor, plan says "tutor for Intruder Alarm"), **130 seq 140**. **+~750**. |
| 5 | `CHECK 3 - THE COUNT` **DELETED**; its one durable fact (one blocker blocks one attacker, they pick the matchups, outnumbering protects nobody) folded into CHECK 2's last sentence, and the section is now `ATTACKING - FOUR CHECKS IN ORDER` with CHECK 4 renumbered to CHECK 3. | The check compared two moving counts (`their untapped count is at or above what you send`) rather than reading a printed tag, and at **152 seq 40** its own boundary — 1 untapped blocker against 2 sent — *permitted* the attack that cost the game. Edit 3 answers that board off the row's own tag. **−~330**. |
| 6 | A334 audit: `the hold is wrong however poor the window looks` → `the hold is wrong; a poor window is not a dead one`. `NEVER ON A TOKEN, however many equip rows the menu lists` → `NEVER ON A TOKEN; the menu lists one equip row per token and every one is wrong`. `"this equips nothing" is a decline even as the only cast row` → the concessive dropped with the sentence (edit 1). | A334, applied as drafted. ~0 net. |
| 7 | `NAMING "Yourself"` gains: `and never your only creature: an empty battlefield turns the Alarm and the Greaves in your hand into dead cards`. | **162 seq 39/40**, where the self-edict removed the last creature and made the **seq 45** Alarm dead on arrival. **+~120**. |
| — | **Compressions and deletions paying for the rest (−~1,700):** BLOCKING's four STEP headings collapsed to three unlabelled paragraphs with no content lost; DAMNATION's four branches folded to three lines; MULLIGAN's keep test de-duplicated; the TIMING bullet `Nothing untaps in your untap step ... never what you made this turn` deleted (its content is the first TIMING bullet plus CHECK 3's maker-home rule); ORDER OF OPERATIONS' `and lethal on the board - attack, do not build` deleted (the block immediately below it says exactly that); TRAGIC SLIP folded to one paragraph; the card-fact restatements of printed mana costs and stat lines dropped; prose trims through the Tutor, Greaves, Skeins, fetch and discard sections. | — |

**What I deliberately did NOT change.** SWEEPER SEEN stands untouched: this corpus tested it once
(125, against a deck that swept the seat's board at t28) and the seat had built in its own main
phase, but the sweeper's arrival is confounded with the Lightmine Field stall and I will not rewrite
a rule on one ambiguous board. I did not weaken CHECK -1 — it went 4 for 4. I did not add anything
about the 50-turn empty-board stretch in the 125 game: no guide rule reaches a board with no
creatures and no castable maker. And I did not bend the guide around MED-1; the render is the thing
to fix there, and edit 2 is written to survive the fix (it keys on the trigger clause, which is
true, not on the floor sentence, which is not).

## 5. Optional proposals

**None.** Edits 1–4 are scoped to this deck's cards and this file's own wording. The two
general-looking findings belong elsewhere: MED-1 is a render defect for the engine seat, and MED-2
is a counter-widening request. A skill proposal restating A332/A333/A334 would be noise; edit 1 is
an *application* of A333 (one first-match list, scope named in its own first clause), not a new
amendment, and edit 5 is an application of §4.1's existing rule that an inert threshold is replaced
by an evidenced condition.

## 6. What I did NOT check

- **Opponent seat files were opened only for board context** (deck125's Lightmine Fields and Staff
  of Nin, deck162's draw punishers, deck130's Hammer of Bogardan). I audited no opponent decision.
- **I adjudicated no lane prediction corpus-wide.** §2 answers P6, P7/F8, P19 and the hold family
  **for this seat's six games only**; the UNTESTED list in §2 names what this seat could not reach.
- **The 41 `hold_released_turn` events were not individually paired to their holding windows.** I
  established that no (turn, phase) in the seat exceeded 19 asks and that the one 19-ask window is a
  genuine chain; the per-release trace is the engine seat's.
- **The 3 `forced_close_unrecorded` in this seat (125 ×2, 162 ×1) were not reconciled against
  stderr.** `phase2_answer_recovered` is 5 and `phase2_answer_missing` is 0 here, and no decision in
  this seat is missing a record, so I saw no loss — the reconciliation is the engine seat's.
- **`async_drops` 32 were not traced**, and the `askreplay/` sidecar was not opened.
- **`plan_names_uncastable_zone_card` 21 was not sampled** for how many are real graveyard/exile
  plans.
- **I did not open any card script.** LOW-4 is diagnosed from the rendered event log only; no build,
  no run, no git, and nothing under `bin/Res`, `src/` or outside `wave75/deck123/` was touched.
- I read every attackers, discard, reveal and bottom window, every window carrying a Create, Cast,
  Equip or Transform row that was taken, and the `reasoning` of the deciding windows; I did not read
  the `reasoning` of all 475 held windows individually.
