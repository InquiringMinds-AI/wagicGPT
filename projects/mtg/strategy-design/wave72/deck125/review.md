# Wave 72 — deck 125 (Revelation Fracture, UW draw-go control)

Seat files: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-163626/*ai_baka_deck125-*`.
416 model decisions across 6 games (**2 W / 4 L**, down from 3/3), reasoning on for all of them,
median `reasoning_chars` 4.8k–7.9k, p50 latency 67 s. 1 fallback (`noop_row_zero_reask`, vs162 seq
17 — **genuine**, §A-1); 0 `reply_truncated`, 0 `action_before_plan`, 0 `ask_replays_refused`,
`menu_pass_no_progress` 0, `async_drops` 6, `identical_ask_answers_reserved` 89,
`ask_replays_reserved` 161 with the `askreplay/` sidecar now written for 5 of the 6 seats
(wave-71 A-5 **FIXED**). 17 records (4.1%) carry `plan_line_missing` and/or `off_protocol_bytes>0`.
No game hung, crashed or timed out.

## 1. Game-by-game

| opp | result | turns | end life | the decision that decided it |
|---|---|---|---|---|
| 152 | **WIN** | 64 | 68–0 | Staff grind; **seq 79** took the crack-back ping row — wave-71 edit #3 paid (§B) |
| 123 | **WIN** | 33 | 51–0 | uncontested; two Staffs down by t14, five on-list counters, no deck-out (library floor 20) |
| 130 | **LOSS** | 36 | −1–18 | **seq 46** (t27 upkeep, 18 life) took the HOLD row instead of the "Cast nothing right now" row that was on the same menu — the seat then got **no window at all on turns 27–30** while life went 18 → 6. §A-2 |
| 162 | **LOSS** | 13 | 0–20 | **seq 16** (t9, 11 life) entered a Revelation cast under a printed DRAW PUNISHERS paragraph — guide stop (1) — then declined at the X menu (seq 17→18). 5 sources stayed tapped, so on t10 Howling Mine (a COUNTER-ON-SIGHT name) resolved with 0 untapped sources. §A-1, §C-2 |
| 126 | **LOSS** | 32 | 0–50 | Sanguine Bond + Exquisite Blood; the opponent's own Staff ping started the chain. The seat held 4 counters and 1 untapped source on t30 when Exquisite Blood resolved — unanswerable, see §A-3 |
| 146 | **LOSS** | 29 | −5–22 | hand at 1–2 cards from t22 to t29, its only spell a Fall of the Gavel with no legal target for six turns. Kaya resolved at 20 life with an empty grip. Variance, not guide |

Wave-71 edits, checked. **#3 (crack-back ping)** — the only trigger this corpus produced,
`…vs-ai_baka_deck152.jsonl` seq 79, was taken: row `Deal 1 damage with Staff of Nin targeting Elite
Spellbinder … {removes 3 from the CRACK-BACK total}`. **PAID**, and it is the exact miss wave 71 was
written for. **#4 (K=0 sweeper)** — 56 `destroys 0`/`exiles 0` sweeper rows offered, **0 taken**;
holds. **#2 (gate never decides whether a row does anything)** — 23 Lightmine-at-N=0 rows offered,
**1 taken** (§C-1); the sweeper half holds. **#5 (Elixir ahead of the hold row)** — 12 activations,
none deferred past the window that first met the rule. **#1 (drop the literal `YOUR PLAN`)** — the
echo class fell from 10/461 (2.2%) to 5/416 (1.2%); small numbers, consistent with the hypothesis.
Wave-70 **#5 (Emrakul)** still paying: the one Emrakul cast row this corpus (deck152 seq 220, two
Staffs out) was declined.

## 2. Engine / interface / card items

**HIGH — A-2. A hold taken on the seat's OWN turn cost four turns of windows, and the row that
would not have is on the same menu.** `…deck125-…-vs-ai_baka_deck130.jsonl` **seq 46**, turn 27
Upkeep, 18–18, 3 lands, hand `2× Supreme Verdict, 2× Path to Exile, Final Judgment`. Menu:
```
1. Cast Path to Exile {w} … {removes: Goblin #1, Goblin #2, Goblin #3} - legal targets right now: Goblin #1-#3 (creature 1/1) x3
2. Hold priority - pass now, and do not ask me again - this turn or later - until one of the rows above changes …
3. Cast nothing right now {closes ONLY this window - the same list can be put to you again this turn …}
```
The model's `reasoning` is explicit and correct up to the last line: *"The guide explicitly says for
power 0-1 and life > 12: 'answer Cast nothing right now at THAT window'. So passing is correct. …
Actually, row 3 is 'Cast nothing right now'. Row 2 is 'Hold priority'. **Both are fine. I'll use 2.**"*
Its PLAN line reads *"Hold priority to reach main phase"* — the opposite of what row 2 says.
The **next record in the file is seq 47, turn 31 Main phase 1, at 6 life**: the hold survived turns
27–30 because the only thing that moved was a life total, which the row itself excludes. The
`askreplay/` sidecar has **no** records for turns 27–31, so this gap is the hold, not the replay
reserve. In that window the seat drew Elixir of Immortality, Fall of the Gavel and Emrakul and could
not cast the Elixir until t31; it activated at t33 (1 → 6 life) and lost at **−1** on t35. One
earlier Elixir cycle is 5 life.
Corpus-wide the seat answered 121 own-turn pass windows: **80 hold, 41 "Cast nothing"**. The engine
side of this is cheap and worth doing on its own: on a menu belonging to the ACTIVE player, either
drop the hold row, or give it a tail saying what it costs in the reader's terms — *"this also
answers your remaining windows this turn, including your main phases"* is present, but the row is
1,050 characters long and that clause is in the middle of it. The guide edit below carries the other
half.

**MED — A-1. The wave-72 re-ask that names the refused row works; both of the corpus's
`noop_row_zero_reask` fires are GENUINE and both recovered.** Wave 71 recovered 1 of 3.
- `…deck125-…-vs-ai_baka_deck162.jsonl` **seq 17** (t9, 11 life), X menu for Sphinx's Revelation.
  Refused row: `3. X = 0 {X pricing: X=0 - this cast does NOTHING: you gain 0 life and draw 0 cards,
  and the spell is spent} {leaves 2 of your 5 floating mana unspent}`. Genuine — the model's own
  `reasoning` wanted to *"spend the base cost and preserve 2 mana for counters"*, which is a pool,
  not an untapped source. The re-ask printed `[RE-ASK] You chose row 3 ("X = 0"), whose own note on
  this list says it does nothing right now.` and **seq 18 changed to row 4 (Decline)**.
- `…deck152-…-vs-ai_baka_deck125.jsonl` **seq 62** (opponent seat, t49, 12 life). Refused row:
  `becomes a 14/14 hydra … with Lair of the Hydra [cost: {14}{g}] [this land is TAPPED: animated, it
  still cannot attack this turn] … {right now: does nothing this turn}`. **Genuine** — the Lair is
  tapped, so the verdict is true, not a false fire on a prospective zero. The re-ask corrected to
  the hold row at seq 63.
  Two side notes from that record, both LOW: the model answered `CHOICE: 14 (becomes a 14/14 hydra)`
  using the creature's power as the row number, and the **engine's short-name match rescued it** to
  row 6 — the re-ask text then said *"You chose row 6"*, which does not match what the model wrote
  and would confuse a reader trying to learn from the correction. Naming the row by its SHORT NAME
  as well as its number would close that.

**MED — A-3. The LOOP paragraph now states the thing wave 71 asked for, and it still has no answer
on the menu.** `…vs-ai_baka_deck126.jsonl` seq 92 renders `[LOOP RUNNING: both halves of the
opponent's life LOOP are on the battlefield and the chain is live …]` and the model's `reasoning`
quotes the new clause back verbatim — *"Any life YOU lose, and any life THEY gain, chains until you
are at 0"*. Wave-71 A-2 is **FIXED**. The game was still lost on t32 to the opponent's own Staff of
Nin ping (1 damage → Exquisite Blood → Sanguine Bond → 0/50 in one chain, deck126 seat log t32).
Deck 125 owns no enchantment removal, so the render is accurate and there is nothing to choose; the
remaining engine-side value would be pricing it once at the top (`this board kills you on the next
point of life you lose from any source`) rather than per-link.

**MED — A-4. The declined-count clause has now lost a quarter of its windows for three waves
running.** 54 windows printed `[you declined this exact list N times already this turn]`: hold 32,
`Cast nothing right now` **13 (24%)**, something else 9. Wave 70 ≈20%, wave 71 26%, wave 72 24%. The
guide states the rule in capitals and the number has not moved — this is a render problem. Wave 70's
and 71's suggestion stands: when that clause prints, drop the plain decline row or tag it
`(this same question will be asked again this turn)`.

**LOW — A-5. The `NO LIVE CAST ROW` header is doing its job.** 161 `ask_replay` records, all
`repeat_ask_reserved`, 0 refusals, and the reserved answers are exactly the "Cast nothing right now"
repeats the new own-turn guide rule will produce — so that rule costs no extra model calls.

**LOW — A-6. Cards verified against the primitives (all faithful to Oracle, no defect found):**
`Lightmine Field` (mtg.txt:67058), `Teferi's Puzzle Box` (mtg.txt:118332 — the hand-to-bottom then
draw-that-many shape matches, and the vs162 t9/t11 log renders it correctly), `Sphinx's Revelation`
(mtg.txt:110851), `Elixir of Immortality` (mtg.txt:34775). Cosmetic only: cast rows print
Revelation's cost as `{u}{u}{w}{x}` while the card is `{X}{W}{U}{U}`.

## 3. Guide verdict: **EDIT** (`wave72/deck125/strategy.txt`, **19,999 bytes** against the live
guide's 19,982 — it GROWS by 17 bytes, and the rule that earned them is **edit 1**, the own-turn
hold row: it is the only edit here paid for by a decision that cost a game the seat could still win
(vs130, §A-2), and the other three additions plus their trims come out net negative. Ceiling 20,000,
so there is 1 byte of headroom left: any further rule must trim first.)

| # | before → after | paid by |
|---|---|---|
| 1 | YOUR WINDOWS: `THE LAST ROW OF EVERY MENU is "Hold priority …". When the prompt carries "[you declined this exact list N times already this turn]", THAT ROW IS THE ANSWER …` → `TWO ROWS DECLINE AND THEY ARE NOT THE SAME. "Cast nothing right now" closes THIS window only. "Hold priority …" closes every later window this turn whose rows have not moved - and a life total moving is not a row moving, so a hold taken in your upkeep can cost you your own main phase, and your next window can be turns away. ON YOUR OWN TURN THE HOLD ROW IS NOT YOUR ANSWER: take "Cast nothing right now", every time, whatever the menu and however dead the rows look. Only on THEIR turn is the hold row right. ONE OVERRIDE, either turn: while the prompt carries "[you declined …]" …` | **vs130 seq 46** (§A-2). The old text told the reader when to TAKE the hold and never when not to; the model had the right rule, saw the right row, and called the two interchangeable. 80 of 121 own-turn passes this corpus were the hold row. The declined-count override is kept intact and now reads as the one exception rather than the whole rule |
| 2 | REVELATION, after stop (3): + `THESE THREE ARE NOT HEURISTICS AND HAVING NOTHING ELSE TO DO IS NOT AN EXCEPTION: unspent mana is the counterspell you hold on THEIR turn, and every source this cast taps stays tapped until your next untap step, so the answer stays "Cast nothing right now" when the Revelation is the only row printed.` | **vs162 seq 16**, `reasoning` verbatim: *"The guide says 'Answer Cast nothing right now while ANY of these is on the screen' … But I have no other option. I will cast it. **The guide is a heuristic. Survival overrides.**"* The prompt carried the DRAW PUNISHERS paragraph and the seat was at 11 life with Cancel in hand. It tapped 5 of 5 sources on its own turn 9; on turn 10 Howling Mine resolved against 0 untapped sources, and under Puzzle Box + Underworld Dreams the seat drew 7–10 cards a turn and died on t13 |
| 3 | X MENU: + `A DECLINE HERE IS NOT FREE - the payment is made and your sources stay tapped for the turn - so the stops above are answered at the CAST row, before this menu ever opens.` | **vs162 seq 17→18**. Declining after announcement is the correct answer once you are there, and the row says so, but the log line `Sphinx's Revelation was NOT cast: you declined after the payment above … the 5 mana that paid for it is still floating, unspent` is the whole cost of edit 2's miss. This sentence is where the reader learns the X menu is not an escape hatch |
| 4 | LIGHTMINE FIELD, `N of 0` bullet gains `- including a second copy, whose "[second copy: ... no legend rule]" tag says only that it would stand, never that it does anything.` | **vs126 seq 92**, the corpus's only zero-verdict take: row `Cast Lightmine Field {2}{w}{w} {right now: they control 0 creatures able to attack - deals 0 until they have an attacker} [second copy: you already control Lightmine Field; both stay on the battlefield … each line it repeats happens again]`. The model's PLAN called it *"a second damage engine"*. Note for the engine seat: this row is a **deliberate** non-no-op under lane BT M2 (prospective zeros are exempt), so the re-ask no longer backstops this rule and the guide has to carry it alone |
| 5 | Byte payers, no decision rule lost: the Emrakul "with an Emrakul of yours out, attack/block" bullet deleted (deck 125 has never had a creature on the battlefield in any corpus, and the guide's own rule forbids the cast whenever a Staff is out); `Sixteen, not twelve … at 90 life with 12 cards left you are losing` deleted (rationale, the 16 threshold is stated); `"[tapped]" says nothing about next turn and it untaps` deleted from PATH (Lightmine states it); `Ob Nixilis` dropped from the draw-punisher example list (the printed paragraph names its own permanents); `(it counts against every opponent)` dropped from the Essence Scatter price; the Emrakul, Staff, counterspell, sweeper, Revelation, Elixir, cleanup and mulligan paragraphs re-worded shorter | bytes only |

KEEP, explicitly: the counterspell type/power table (every counter this corpus spent was on-list —
Katilda and Overgrown Battlement on the `{T}: Add` exception, Idyllic Tutor and Bloodline Keeper and
Thraben Doomsayer on COUNTER ON SIGHT, Stone Rain at 3 lands, Rorix 6/5, Acererak 5/5, Nadaar 3/3);
PATH's ordered list (vs130 seq 44 took Siege-Gang Commander on rule 3, and rule 4 correctly declined
1/1 Goblins at 18 life); the `K IS 0` sweeper bullet (56 offers, 0 takes); LAND DROPS (71 land-drop
asks, 71 lands played, tapped ones included); CLEANUP DISCARD (18 windows, no keep this reader would
fault); MULLIGAN (6 keeps, all with 2–5 lands and a coverage line); the TAP-OUT GATE arithmetic (no
`{leaves N …}` misread in 416 decisions).

Two guide non-compliances I am **not** editing for. (a) The gate's shape (d) crack-back exception is
**UNTESTED for a second wave**: the only Staff cast row under a CRACK-BACK line (deck152 seq 78) was
at 34 life, above its 15-life trigger. (b) vs126 seq 86 announced Revelation X=7 with a live Sanguine
Bond on the board, leaving 1 untapped source for the turn Exquisite Blood resolved — but the seat's
hand held no counterspell at the cast, so the gate was open and the guide was followed; a rule
narrow enough to catch it ("hold {1}{U}{U} when half a combo is on their board") would be a new
special case with one instance behind it.

## 4. Not checked
Opponent seats were read only where deck 125's own log was ambiguous (deck126 t28–t32, deck152 t49).
I did not audit the `reveal`, `attackers` or `blocks` seams (this deck fielded no creature in any of
the six games), did not verify any card against Scryfall this wave (the four primitives above were
checked against the primitive text only), did not measure `own_turn_windows_skipped` (16 in vs126, 0
elsewhere) against the code, did not explain the corpus-wide `sibling_window_asks_skipped 0` /
`stop_reached_windows_skipped 0` zeros, and adjudicated none of the lane-{BT,BU,BV,BW,BX}
predictions — those are the engine seat's job. No general-guide or skill proposals: the hold-row
finding is about a row this deck answers differently from a deck with creatures, and nothing else
here generalises past this guide.
