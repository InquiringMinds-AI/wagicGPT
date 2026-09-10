# Wave 74 — deck 125 (Revelation Fracture, UW draw-go control)

Seat files: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260909-104713-final/*ai_baka_deck125-*`.
**MIXED BINARY**: vs130 / vs146 / vs162 are the CG-binary run (`start_epoch` 17889688xx); vs152 /
vs126 / vs123 are the CH-binary reruns (17889981xx). Every claim about holds below is split by binary.

**1 W / 5 L** (wave 73: 1/5). 653 seat decisions, reasoning on for all, 25 `plan_line_missing` (3.8%,
down from wave 73 in rate), 25 `off_protocol_bytes>0`, **1 fallback** (`noop_row_zero_reask`, vs162
seq 148 — genuine and useful, §A-6), 0 `reply_truncated`, 0 `action_before_plan`, 0 `ask_replays_refused`,
247 `ask_replays_reserved`, 0 `identical_ask_answers_reserved`, 0 `sibling_window_asks_skipped`,
0 `chain_windows_collapsed`, 0 `stop_reached_windows_skipped`, 0 `main_phase_windows_skipped`,
0 `plan_names_stranded_card`, `async_drops` 12, `hold_released_turn` 19. No hang, crash or timeout.

**The loop's question — can the guide close a stabilised game? YES, and it did once; the other three
stabilised games were each thrown away by a DIFFERENT single decision, and none of the three was a
clock failure.** vs130 is the clean proof: one Staff from t21, the wave-73 second-Staff rule fired at
t39 (`seq 107`, the model's own line: *"Cast the second Staff of Nin to double the clock"*), the clock
halved from 18 turns to 5 and the game closed 59–0 on t49. Against that, vs126 was at **42 life vs 7
with two Staffs** and lost on its LAST window by pointing its own Staff at itself; vs123 was at **40
life vs 14 with a Staff** and lost by tapping out in its own UPKEEP for a Revelation with a lethal
crack-back coming — a crack-back line the engine does not print at that seam; vs162 was at **33 vs 18
with a Staff** and lost by casting a Revelation under three draw punishers for a net −7 life.

## 1. Game-by-game

| opp | binary | result | turns | end life | the decision that decided it |
|---|---|---|---|---|---|
| 130 | CG | **WIN** | 49 | 59–0 | Not a misplay: `seq 107` t39, second Staff cast over a Revelation and an Elixir on the same menu (§3 KEEP). 46/48 Staff pings taken; opponent 20→0 with no other damage source |
| 146 | CG | LOSS | 23 | −10–18 | `seq 84`, t13, 22 life, 4 mana, **Kaya the Inexorable on the stack**. Row 1 was a Revelation at `max affordable X=1` whose own keep-X clause read *"Essence Scatter {1}{u} in your hand needs 2: no X on this row leaves it payable this turn, not even X=0"*. The seat cast it: a SIZE-FLOOR violation (X=1 at 22 life), one card and one life for its only Revelation, and Kaya resolved unopposed. The rest (t18 Verdict at K=3, t21 Gavel at 5 life) is guide-correct play from a lost position |
| 162 | CG | LOSS | 27 | 0–18 | `seq 147`, t27 Draw, 17 life, **Underworld Dreams + Ob Nixilis + Dictate of Kruphix all printed under `DRAW PUNISHERS`**, 9 damage already on the stack. Revelation stop (1) says "Cast nothing right now"; the seat cast for X=7 (`seq 149`). Each card drawn cost 2 life, so X=7 gained 7 and paid 14 — the engine's own badge said `NET (-7)`. Life went 17→24→2→0 inside the same Draw step. §B-1 |
| 152 | CH | LOSS | 27 | 0–20 | Mana-light game (25 asks in 27 turns), no Staff ever drawn. Last two windows, `seq 28` and `seq 29` (t26, 6 life): the seat planned *"Cast Sphinx's Revelation for X=5/X=6 to stabilize"* — **the Revelation was in its GRAVEYARD, not its hand**, and `plan_names_stranded_card` did not fire (§A-4). At `seq 28` it also answered "Play no land right now" (the only land-drop decline in 73 asks) after reasoning *"The guide says 'Play one land, every turn.' It's a hard rule… I'll just pick 4 to be safe"* |
| 126 | CH | LOSS | 53 | 0–48 | **`seq 267`, t53, 42 life vs 7, two Staffs, `LOOP RUNNING` and `LOOP COMPLETE` both printed.** Row 1 pings them (they would be at 6); row 2 pings the seat, `{right now: takes 1 damage - you would be at 41}`. The seat took **row 2**, having reasoned exactly backwards: *"Pinging opponent = opponent loses life = triggers loop = I die. Pinging me = I lose life = no triggers = safe."* Sanguine Bond + Exquisite Blood are BOTH theirs; the prompt states the correct direction in full. 42 → 0, opponent 7 → 48. §A-1, §3 edit 1 |
| 123 | CH | LOSS | 24 | −1–14 | **`seq 73`, t23 UPKEEP, 40 life, 8 untapped sources, Final Judgment and a Path in hand, 40 Humans on their board under Intruder Alarm.** The seat cast a Revelation for X=3, leaving 1 source; Final Judgment ({4}{W}{W}) was then unpayable in the main phase it was legal in, and the seat Pathed one 1/1 (`seq 79`) into a 41-power swing. **No `CRACK-BACK NEXT TURN` line is printed in ANY upkeep window** — it appears at `seq 79` (Main phase 1) reading *"40 of their creatures… you would be at -1; that would KILL you"* and nowhere earlier. §A-2, §3 edit 2 |

**Wave-73 edits, checked.**
- **#1 (EMRAKUL, "having nothing else to do is not an exception") PASSED, 4/4.** Emrakul cast rows were
  offered exactly four times (vs130 seq 195, 196, 207, 208; t45, 52 life vs 4, three Staffs out) and the
  seat answered "Cast nothing right now" every time. 0 Emrakul casts corpus-wide.
- **#2 (cleanup: a step whose condition is false is not a step) PASSED.** 32 cleanup discards audited;
  no guarded step was taken as unguarded except one Emrakul sent at 16 lands (vs130 seq 254, a won game).
- **#3 (a second Staff outranks a Revelation; never send a Staff at cleanup) PASSED, and it is the win.**
  **Zero Staff of Nin discards** across 32 cleanups (wave 73: five). vs130 seq 107 is the rule firing.
- **#4 (gate shape (d) priced off the crack-back forecast at 12) HELD where testable.** vs146 seq 92 is the
  only Staff cast under a printed crack-back; the line read "-9 or lower; that would KILL you" and the
  guide's exception says answer that line first — but the menu carried no sweeper, Path or Revelation, so
  the exception had nothing to point at. Not a guide failure; the game was already lost.
- **Wave-72 #1 (never hold on your own turn) IMPROVED AGAIN.** 359 own-turn declinable windows, 63 holds,
  **13 without the declined-count override = 3.6%** (wave 73: 8%; wave 72: 66%). Best rule in this guide.
- **The SIZE FLOOR is being disobeyed, not misread.** Four casts below the floor: X=1 at 20 life
  (vs130 seq 27), X=1 at 19 (vs130 seq 38), X=1 at 22 (vs146 seq 84), X=2 at 22 (vs162 seq 36). The rule
  is already written at the cast row and already tells the reader the `[<- best X…]` badge does not know
  it. **No edit** — a fourth capitalisation is not a new rule.

## 2. Engine / interface / card items

**HIGH — A-1. Under a printed `LOOP RUNNING` / `LOOP COMPLETE`, a row that costs YOU life still prints a
bare "you would be at N", and that N is false.** `…deck125-…-vs-ai_baka_deck126.jsonl` **seq 267**, t53.
The row reads `2. Deal 1 damage with Staff of Nin targeting you {right now: takes 1 damage - you would be
at 41} [cost: Tap]`. On that board it does not leave the seat at 41 — it leaves it at 0, because the same
prompt's loop paragraph says *"Any life YOU lose, and any life THEY gain, chains until you are at 0"*.
The loop paragraph is correct, complete and prominent; what it cannot do is contradict a per-row forecast
sitting six lines below it. **Repro/fix:** while a loop paragraph is live, every row whose own `{right
now:}` moves the seat's life DOWN, or the opponent's life UP, should print `you would be at 0 - this row
feeds their chain` instead of the arithmetic figure, exactly as the `[NAMED BY THEIR …]` price clause
already overrides the mana line. This is the single decision that lost a 42–7 game and it is the same
option-set/false-forecast shape as wave 73's A-2, one layer lower.

**HIGH — A-2. `CRACK-BACK NEXT TURN` is phase-gated to the seat's main phases and never printed in its
upkeep or draw step.** Corpus-wide on this seat: 40 windows carry the paragraph, **all 40 in Main phase 1
or 2**; **0 of 173 own-turn Upkeep windows and 0 of 38 own-turn Draw windows carry it**. For a draw-go
deck this is the wrong way round — the Staff ping, the Elixir activation and (as here) the Revelation are
all taken in the upkeep, so the mana is committed in the windows where the lethality forecast is missing
and re-read in the windows where it is too late. **Repro:** `…vs-ai_baka_deck123.jsonl` seq 68, 71, 72 and
**73** (t23 Upkeep, 40 Humans + Thraben Doomsayer on their board, no CRACK-BACK line anywhere in the
prompt) against **seq 79** (t23 Main phase 1, same board, `CRACK-BACK NEXT TURN: 40 of their creatures
will be able to attack … for up to 41 - you would be at -1; that would KILL you`). Fix: render the
paragraph in every own-turn window, upkeep and draw included.

**MED — A-3. The X-menu boilerplate still says the rows run largest-X-first, after O19 made them
ascending.** 28 X menus on this seat; **index == X in all 28** (O19 PASS, 0 `echo_index_conflict`), yet
**14 of the 28** still carry the range-explanation paragraph ending *"A row reading "X = 12 down to X = 2"
is one option per X in that range, largest X first: its first number is X = 12, the next X = 11…"*. That
sentence is now false about the menu it is printed on, and it sits directly above the answer line. Repro:
`…vs-ai_baka_deck162.jsonl` **seq 148** (row 9 is `X=9`, row 10 is `X = 0`, worked example reads
`CHOICE: 1 (X = 1)`, and the paragraph above the answer line says largest-first). Fix: delete the clause
or rewrite it ascending.

**MED — A-4. `plan_names_stranded_card` does not fire when the plan names a card that is not in hand at
all.** `…vs-ai_baka_deck152.jsonl` **seq 28 and 29**, t26, 6 life, the seat's last two windows. Both plans
read *"cast Sphinx's Revelation for X=5 / X=6 to stabilize"*; the hand line reads `Your hand (4 cards):
Cancel …; Plains; Island; Tundra` and the graveyard line reads `Sphinx's Revelation {u}{u}{w}{x}`. The
instrument counted 0 for this game. Widening it to "the plan names a card in no zone you can cast from"
would have caught the seat's final plan in a game it lost with 9 untapped sources.

**MED — A-5. The declined-count override is now failing 45% of its windows, and O11's new tag did not
move it.** 179 windows carried `[you declined this exact list N times already this turn]`; 87 took the
hold row, 11 acted, **81 (45%) answered "Cast nothing right now" or "Pass priority"** — the row the guide
names as the wrong one, and the row that now carries O11's tag `{this same question will be asked again
this turn: taking this row closes this window only, and you have already declined this exact list N times
this turn}`. Wave 70 ≈20%, 71 26%, 72 24%, 73 40%, **74 45%**. Five waves of guide capitals and one wave
of an engine tag have all failed in the same direction. Repro: `…vs-ai_baka_deck162.jsonl` **seq 146**
(t27, clause reads 8 times, tag present, answer "Cast nothing right now"). The wave-70/71/72/73
recommendation stands and is now the only untried one: **stop printing the plain decline row once the
clause fires.**

**MED — A-6. Two render clauses added this wave are TRUE on every instance on this seat, and one of them
did nothing.** (a) The **O9 own-clock tag**: 202 instances, `ceil(opponent_life / rate)` matched the
printed turns-to-zero **202/202**, rate correctly doubled with the second Staff (vs130 seq 107 "1 damage
a turn … 11 more turns" → seq 108 "your Staff of Nin #1-#2 deal 2 damage a turn … 6 more turns"). One
cosmetic defect: with a single Staff it reads *"your Staff of Nin deal 1 damage a turn between them"* —
plural verb and "between them" for one permanent. (b) The **O8 keep-X line**: 55 instances on Revelation
cast rows, arithmetic correct **55/55** (`keep-X == max affordable X − the named card's cost`), so wave
73's A-1 is fixed. But it changed no behaviour: it printed on 11 of the seat's 23 Revelation casts and the
seat announced **above** it in 8 of those 11, stranding a Path to Exile each time. It also never named a
Staff of Nin — it names the CHEAPEST unpayable card in hand, which on this deck is always the {W} Path,
never the {6} artifact that wave 73 said was the card being stranded. Worth naming the most EXPENSIVE
still-payable card instead of the cheapest.

**LOW — A-7. The CH hold-latch fix is visible in a clean binary split on this seat.** Maximum
`declined this exact list N times` per game: **CG binary 7 (vs130), 13 (vs146), 23 (vs162)** against
**CH binary 3 (vs123), 4 (vs126), none printed (vs152)**. The vs162 t27 Draw step is the CG failure mode
in one place — 38 asks in a single step, board unchanged except the seat's own life ticking down one
point per link, with `[hold check: 2 rows above are new since the last window at this seam - a row moving
is what re-opens a hold]` printed each time. All 19 `hold_released_turn` releases on this seat land at
the holder's next untap; no hold outlived its holder's untap on either binary.

**LOW — A-8. The one fallback on this seat is genuine and beneficial.** `…vs-ai_baka_deck162.jsonl`
**seq 148 → 149**: the model answered `CHOICE: 10 (X = 0)` while its own plan line read *"Do not cast
Sphinx's Revelation due to active draw punishers"* — it wanted the Decline row (11) and took the
do-nothing row. The `[RE-ASK] You chose row 10 ("X = 0"), whose own note on this list says it does
nothing right now.` recovered a real answer. `noop_row_zero_reask` is doing its job.

**LOW — A-9. Untested on this seat, stated so the engine seat does not read silence as PASS:** O12
(tapped + no-untap exclusion from K) — **0** windows in this corpus showed a creature carrying both tags;
O1 (ghostform gloss) — Kaya's ghostform text appeared in 8 vs146 windows but no creature ever carried a
ghostform counter, so no gloss rendered; `CLEANUP PRICE at X=`, `DISCARD PUNISHERS` and Forced Fruition —
**0** occurrences, so Revelation stop (2) and two PUNISHER BOARDS bullets went untested for a second wave.

**LOW — A-10. Cards verified against the primitives, all faithful, no defect found:** `Sanguine Bond`,
`Exquisite Blood`, `Staff of Nin`, `Sphinx's Revelation`, `Intruder Alarm`, `Ob Nixilis, the Hate-Twisted`.
The vs126 t53 chain (self-ping → Exquisite Blood → Sanguine Bond → 42→0 while they went 7→48) and the
vs123 t23 board (39 Humans untapping off Intruder Alarm under Thraben Doomsayer + Lightning Greaves) are
both the rules-correct behaviour of those boards.

## 3. Guide verdict: **EDIT** (`wave74/deck125/strategy.txt`, **19,991 bytes** against the live guide's
19,979 — it grows by 12 bytes; the three new rules are paid for out of the rationale trims listed as
edit 4, so no rule has to earn new bytes.)

| # | before → after | paid by |
|---|---|---|
| 1 | **The EMRAKUL converter sentence was FALSE and is deleted.** `That converter fires on life THEY gain, never yours: your Revelation, Elixir and Gavel are safe.` → a new section `=== THEIR CONVERTER, AND THEIR LOOP ===`: `A converter of THEIRS fires on life THEY GAIN … A "LOOP" paragraph is the other half and it reads the opposite way: life YOU LOSE chains too, without limit. WHILE A LOOP PARAGRAPH IS PRINTED, PAY NO LIFE OF YOUR OWN FOR ANYTHING - not a self-ping, not a life cost on a cast row - at any life, because the chain does not stop where that row's "you would be at N" gloss stops. Life THEY lose does not enter their chain: damage you point at THEM is always safe.` Plus, in STAFF OF NIN: `THE TARGET IS ALWAYS THEIRS: a ping row "targeting you" is NEVER your answer, at any life and under any paragraph on the screen.` | **vs126 seq 267.** The old sentence told the reader the converter direction is "life THEY gain, never yours" — under a loop that is exactly the half that is wrong, and the model's reasoning inverted the direction the same way and killed itself from 42 life with the opponent on 7. A guide that states a false fact about the board is worse than a guide that is silent, and this deck's only win condition is a targeted damage source, so "which way does the chain run" is a decision-time question every upkeep |
| 2 | ORDER OF OPERATIONS step 3: `On YOUR turn: ping with every Staff, then spend mana ABOVE the price …` → `On YOUR turn CAST NOTHING BEFORE YOUR MAIN PHASE: upkeep and draw are for the Staff ping, an Elixir activation and a counter at the stack, nothing else - a sweeper, a Staff, a Lightmine Field and an Elixir CAST are main-phase-only, and mana spent earlier is mana that phase will not have. Then spend mana ABOVE the price …` | **vs123 seq 73** (t23 upkeep, 40 life, 8 sources, Final Judgment in hand, 40 attackers, Revelation X=3 cast, dead on t24 from 40 life). The tap-out gate already covers WHAT the mana buys but says nothing about WHEN; the engine's own screens make the upkeep the cheapest place to spend and the main phase the only place the sweepers are legal, and A-2 means the lethal forecast is not even on the screen at the earlier seam. 15 of the seat's 23 Revelation casts were taken in an upkeep or draw step this corpus |
| 3 | SPHINX'S REVELATION, before "THESE THREE ARE NOT HEURISTICS": + `STOP (1) IS ARITHMETIC: with P such permanents printed, X cards drawn cost X times P life, so this gains X and pays X times P - a NET LOSS at every X, and a badge naming "the largest X that leaves you alive" names the smallest loss, never a gain.` And in THE X MENU: `announce that X` → `announce that X - unless a DRAW PUNISHERS paragraph is printed, and then take "Decline": no listed X is a gain.` | **vs162 seq 147 and 149.** Stop (1) was on the screen and the model overrode it *to gain life* (`"Cast Sphinx's Revelation for X=7 to survive the draw triggers"`) — it never computed that the cast is net negative. Worse, the existing X-menu clause `When option 1 carries a life bill naming a surviving X, announce that X` **actively directed** the losing X: the engine's badge read `X=7 is the largest listed X whose NET (-7) leaves you alive, at 1` and the guide told the reader to take it. That clause had to be gated or it would keep overriding stop (1) from inside the menu |
| 4 | Byte payers, no decision rule lost: the tap-out gate's "where M minus K is left and the card named is the cheapest one you cannot pay for" gloss; the Staff second-copy "on an Elixir each activates on its own" aside; the DIES-ping rationale tail; Revelation stop (3)'s rationale; "unspent mana is the counterspell you hold on THEIR turn …"; the X-menu "so '0 left' with no warning can deck you" tail; the DECLINE-IS-NOT-FREE rationale; the K-IS-0 enumeration ("being ahead, being BEHIND" → "being ahead or BEHIND"; dropped "your only cast row"); the LIGHTMINE second-copy and toughness glosses; PATH's "a creature that has already resolved"; SWEEPERS' "can't be countered" and "anything that returns from a graveyard"; the Elixir "every counterspell and Path you have spent goes back" restatement; the INCOMING pointer tail; Idyllic Tutor's gloss; LAND DROPS' "with an UNTAPPED row and a TAPPED row both offered"; CLEANUP's "at end of turn" and the discard-punisher restatement | bytes |

KEEP, explicitly, with the evidence: the second-Staff rule and the never-discard-a-Staff cleanup step
(0 Staff discards in 32 cleanups; vs130 seq 107 is the wave's only win); the own-turn hold rule
(3.6% violation, down from 8% and 66%); the EMRAKUL refusal (4/4 declines with Staffs out at 52 life vs 4);
LAND DROPS (73 asks, a land played in 72); the Staff ping rule (46 of 48 offered windows taken);
the counterspell type/power table (every counter this corpus was on-list, including three planeswalker
counters in vs146/vs162); the SIZE FLOOR and Revelation stop (1) — both were disobeyed, not misread,
and both already say in capitals what a fifth capitalisation would say again.

## 4. Not checked
Opponent seats were read only for the vs126 t53 chain and the vs123 t24 swing. I did not audit the
`reveal`, `attackers` or `blocks` seams (this deck fielded no creature in any of the six games); did not
verify any card against Scryfall (the six in A-10 were checked against the primitive text only); did not
trace the 12 `async_drops` individually or confirm whether any dropped answer fell to the heuristic; did
not explain the corpus-wide `identical_ask_answers_reserved` 1 / `sibling_window_asks_skipped` 0 /
`chain_windows_collapsed` 0 / `stop_reached_windows_skipped` 0 / `main_phase_windows_skipped` 0 zeros
against the code (all zero on this seat, and the last is the O10 figure the engine seat must cross-check);
did not measure `chain_selfharm_rows` (72 on this seat) against the acting-window share O13 asks for;
did not read the `askreplay/` sidecar; and adjudicated none of the lane-{CC,CD,CE,CF,CG,CH} predictions
beyond the six that touched my own windows (O8, O9, O11, O19 and the CG/CH hold split above) — those are
the engine seat's job. No general-guide or skill proposals: edit 1 is a deck-card fact, edit 2 is this
deck's own phase discipline, and edit 3 is arithmetic about one card in this deck.
