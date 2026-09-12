# Wave 78 — deck 125 (Revelation Fracture, UW draw-go control)

Seat files: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-125420-final/*ai_baka_deck125-*`
(six seats, all with `gameend`, 0 timeout-tainted). SINGLE binary `wagic-47ef378c8-w78step1`.

**3 W / 3 L** (wave 77: 3/6). 795 seat records = ask 566 + priority 177 + discard 40 + forced_close 6 +
recovery 6; `protocol_replies` 783, reasoning on 783/783 (median 6,056 chars, **max 17,078** — down from
this seat's 35,886 corpus maximum last wave); latency p50 58.5 s, max 357.7 s. `off_protocol_bytes>0` and
`plan_line_missing` 16 (2.0%), `protocol_deviation_replies` 22, `async_drops` 17, `hold_released_turn` 49,
`hold_windows_skipped` 767, `crossphase_identical_reputs` 26 / `_board_unchanged` 4 (all four corpus-wide),
`forced_close_events` 6 / `_unrecorded` 3 (all on `125v130`), `phase2_answer_recovered` 0 / `_missing` 0,
`stack_drain_windows_asked` 1, `own_loop_windows_asked` 0, `main_phase_windows_skipped` 0,
`mana_only_windows_skipped` 0, `plan_names_stranded_card` 1, `repeat_annotated_takes` 0.
**All six corpus `answer_label_absent` fallbacks are on this seat** (§2 A-1). No hang, crash or timeout.

## 1. Game-by-game

| opp | result | turns | end life | the decision that decided it |
|---|---|---|---|---|
| 123 | **WIN** | 62 | 57–0 | Grind win; 8 counters all on-list, 3 Revelations all in a main phase, 0 self-pings. Four of the six `answer_label_absent` fallbacks are here and none changed the result (`seq 83` cost one round trip, `seq 215` turned a hold into a decline; 140/305 the heuristic played the row the model named) |
| 126 | **WIN** | 85 | 76–4 | Textbook. **`seq 311` is the S5 fix paying**: the seat's own Dream Fracture sat on the stack aimed at Idyllic Tutor, the Cancel row carried `{your Dream Fracture already targets this on the stack …}` and the seat answered `Cast nothing right now` — the exact wave-77 HIGH B-1 shape, now declined. §3 |
| 130 | **WIN** | 69 | 40–0 | Glacial but clean. **`seq 303` is the one fallback with a real price**: the model wrote `2 (Hold priority)` at t55 Upkeep and the heuristic instead announced Sphinx's Revelation; the model then took the `Decline` row, stranding all 12 paid sources, so turn 55 had no main phase and cleanup discarded Revelation + Fall of the Gavel. §2 A-1 |
| 162 | LOSS | 22 | 0–20 | **`seq 52`, t19 Main 1, 20 life, 6 untapped, hand: 2 × Fall of the Gavel + Supreme Verdict + Final Judgment + Staff + 2 × Lightmine Field.** The seat cast Supreme Verdict (K of **1**, Fate Unraveler 3/4) leaving 2 sources. Both Gavels were tagged `[no cast row now: it must have a target and there is no legal target on the board]` and the reasoning reads **"I have no counterspells in hand right now. So gate is open at any leaves-count."** On turn 20, tapped low, the seat was put **no window at all** while Underworld Dreams + Howling Mine + **Teferi's Puzzle Box** resolved; turn 21's draw step shuffled the hand back and drew 11 under Underworld Dreams: **20 → 9**. Dead t22. With the 6 sources untouched Fall of the Gavel (5) was payable and one of the three was answerable. §2 A-2, §3 edits 1–3 |
| 152 | LOSS | 16 | −2–28 | Variance: a t9 six-creature board, 17 incoming at t14. `seq 79` (t14, 18 life, `max affordable X=2`) is the wave's one stop-(4) violation; the reasoning quotes stop (4) and the low-life floor **by name**, finds the floor does not lift it at 18 life, and casts anyway because nothing else was castable against a lethal swing. Disobeyed, not misread — nothing bought (see §4) |
| 146 | LOSS | 34 | 0–14 | Flood: 12 lands on the battlefield and **1 card in hand** at t34 against 4 attackers for 6. `seq 51` Pathed Triumphant Adventurer #1 (1/1 deathtouch) over Silverquill Silencer (3/2); the row's target text was cut `"Deathtouch -- As long as it's your turn, Triumphant Adventurer has first strike. (...more)"` and the clause behind `(...more)` is *"Whenever Triumphant Adventurer attacks, venture into the dungeon"* — the only clause §PATH step 2 keys on. §2 B-1. No guide item is bought: nothing in hand saved the turn |

## 2. Engine / interface / card items

**HIGH — A-2. A counterspell held for THEIR turn reads `[no cast row now: it must have a target and there
is no legal target on the board]` on the hand line, and both the model and the engine's own keep-X forecast
treat that as "not in hand".** Two repros, one of them the game.
(a) `1789149300-ai_baka_deck125-…-vs-ai_baka_deck162.jsonl` **seq 52** (t19): hand line prints
`Fall of the Gavel (copy 1 of 2 in your hand) {3}{u}{w} [instant] [no cast row now: it must have a target
and there is no legal target on the board]` twice; the reasoning concludes *"I have no counterspells in hand
right now. So gate is open at any leaves-count."* The seat tapped to 2 and lost the game on turn 21.
(b) `1789149302-ai_baka_deck125-…-vs-ai_baka_deck130.jsonl` **seq 315** (t57): the CY-F9 keep-X clause reads
`Holding up an instant: Path to Exile {w} needs 1: the largest X that still leaves it castable this turn is
X=8` while Essence Scatter `{1}{u}`, Cancel and Dream Fracture are all on the hand line — each under the same
`[no cast row now …]` tag. Across the seat the clause fires 47 times and names **Path to Exile in 10 of
them**: the forecast picks the cheapest instant that has a *live cast row*, and on a draw-go seat's own turn
a counterspell never has one. S14/CY-F9 therefore does not yet deliver the number the tap-out gate needs.
Fix, engine side, one of two: (i) the keep-X forecast selects over counterspells on the HAND LINE rather than
over rows with a live target, or (ii) the tag itself says what it means for a held instant, e.g.
`[no cast row THIS window - it needs a target and nothing of theirs is on the stack; {3}{u}{w} held open
casts it on their turn]`. Guide side this is edits 1 and 2 below.

**MED — B-1. `(...more)` cuts a target row at a clause boundary but drops the clause that decides the play.**
Repro `…vs-ai_baka_deck146.jsonl` **seq 51/52**, row 1: `legal targets right now: Triumphant Adventurer #1
{b}{w} (creature 1/1) [deathtouch] {target text: "Deathtouch -- As long as it's your turn, Triumphant
Adventurer has first strike. (...more)"}`. The card's real text (verified against
`bin/Res/sets/primitives/borderline.txt`, `text=` line, which matches Oracle) ends
*"-- Whenever Triumphant Adventurer attacks, venture into the dungeon."* The S8 rule is honoured — the cut is
clause-clean and marked — but the two clauses kept are decorative and the one dropped is the only trigger
§PATH step 2 and the §COUNTERSPELLS carve-out read. Fix: when a target row's text is truncated, keep clauses
beginning `{T}:`, `Whenever`, `At the beginning of` or containing `venture` ahead of static keywords.
264 `(...more)` cuts on this seat; I sampled 30 and found no false statement, only this ranking problem.

**MED — B-2. The declined-count override closes a turn the seat had a stated plan for.**
`…vs-ai_baka_deck130.jsonl` **seq 309** (t55 Upkeep, cast seam): the prompt carries
`[you declined this exact list N times already this turn]`, the seat takes the hold row (the guide's own
override tells it to), and the hold — taken at an UPKEEP — stands for the turn: no main-phase window is put
again, and cleanup discards Sphinx's Revelation and Fall of the Gavel. 59 of this seat's 72 own-turn holds are
under that override. The override is doing its job (it breaks a decline loop) but on a seat's OWN turn the
cheapest thing it can close is the main phase the seat has not reached yet. Engine suggestion: a hold taken at
Upkeep or Draw on the seat's own turn expires at its first main phase rather than at its next untap step, or
the row says which phases it is about to close.

**MED — B-3. Prompt bytes: the peak fell, the mass did not.** 783 prompts, p50 17,113 B, **max 34,778 B**
(`…vs-ai_baka_deck126.jsonl`, wave 77: 36,211) but **348 of 783 (44.4%) over 20 KB** against 42% last wave.
Per game: `126` 57% over 20 KB, `130` 63%, `123` 28%, `146` 3%, `152`/`162` 0%. The S13 hoist never fired on
this seat — no window here puts a multi-row target menu off one shared source; the growth is the 29-permanent
battlefield line with a `{effect:}` gloss per permanent in the three long games. Not wrong, additive.

**LOW — B-4. `plan_names_stranded_card` 1** (`125v162`) and **`forced_close_unrecorded` 3 of 6** (`125v130`)
— named here only so the engine seat does not read this seat's silence as absence. I did not reconcile either
against stderr.

**Verified TRUE / FIXED on this seat, with the evidence:**
**S5 PASS, on its own repro shape.** The clause rendered once, `…vs-ai_baka_deck126.jsonl` **seq 311**:
`{your Dream Fracture already targets this on the stack - a second counter on the same spell is spent for
nothing unless yours is itself countered or removed first}` — TRUE against that window's stack (item 2 = the
seat's Dream Fracture targeting item 3 = Idyllic Tutor), correctly unconditional (Dream Fracture is not a
Mana-Leak-class counter), and **the seat declined**. Wave-77 HIGH B-1's double-counter does not recur:
35 counters cast, none into a spell one of the seat's own counters already targets.
**S7 PASS.** 712 records carry `hold_check_ref_seq` and **712 of 712 resolve to a real record seq in the same
seat file** (wave 77: 365 unresolvable). `hold_check_ref_window` carries the ordinal beside it.
**S9(b) PASS.** The disappearance clause prints, e.g. `[hold check: 1 row that was on the menu at the last
window I asked you at this seam is gone and no row above is new - a row disappearing re-opens a hold exactly
as a row appearing does …]`. I found no `Goblin → Goblin #1` renumbering counted as new on this seat.
**Counterspells**: 35 cast, **none off-list** — including Overgrown Battlement (0/4 [defender], countered on
its `{T}: Add {G}` carve-out, `126` seq 256), Staff of Nin twice, Sanguine Bond twice, Idyllic Tutor three
times and seven Hammer of Bogardan.
**Pings**: 268 ping rows over 63 windows, 61 taken, **55 at the face / 6 at a body, 0 self-pings**.
**Land drops**: 93 windows, **93 lands played, 0 declines**. **Emrakul**: 26 cast rows offered, **0 taken**.
**Cleanup**: 40 discards, no Staff sent.

## 3. Guide verdict: **EDIT** (`wave78/deck125/strategy.txt`, **19,997 bytes** against the live 19,999 — it
SHRINKS by 2; the three rules are paid out of seven prose trims, none of which removes a decision rule.)

| # | before → after | paid by |
|---|---|---|
| 1 | **§TAP-OUT GATE's price sentence gains the no-cast-row case.** `… Fall of the Gavel 5. Take the row only when what is left …` → `… Fall of the Gavel 5. A COUNTERSPELL TAGGED "[no cast row now: it must have a target and there is no legal target on the board]" IS ON THAT LINE AND SETS THAT PRICE: that tag is what every counterspell reads on your own turn, and it names THIS window, never your hand. Take the row only when what is left …` | **`162 seq 52`, the game.** The reasoning's own words are *"I have no counterspells in hand right now. So gate is open at any leaves-count."* — with two Fall of the Gavel on the hand line under that tag. The gate's price was 5, the row left 2, and the turn after it three COUNTER-ON-SIGHT permanents resolved against a seat that could have paid 5. The tag is not rare: every counterspell in this deck reads it in every window of the seat's own turn (A-2b: 47 keep-X clauses, 10 naming Path to Exile while three counters sat in hand under the tag) |
| 2 | **Gate shape (c)'s denominator changes from the row to the pool.** ` (c) the cheapest counter in hand costs MORE than the row's own M;` → ` (c) the cheapest counter in hand costs MORE than your whole "Mana available:" total, so nothing you do or decline here keeps it up;` | Same window. Cheapest counter = Gavel 5, row M = 4, so shape (c) as written declared the gate OPEN — while the seat's 6 untapped sources paid 5 with one to spare. Shape (c) is meant for a counter you could never hold up; comparing it to one row's cost makes any row cheaper than the counter open the gate, which is backwards |
| 3 | **§SWEEPERS' K-of-1 line loses its concessive clause (A334) and splits by power.** `- K of 1 with power 2 or more and no Path: cast the sweeper at power 4 or more; otherwise keep it unless your life is 10 or less and nothing else in your list answers it.` → `- K of 1, printed power 4 or more, no Path: cast the sweeper.` / `- K of 1, printed power 2 or 3, no Path: KEEP THE SWEEPER, at any mana - that mana is the counter you hold for their turn. Your life of 10 or less with nothing else in your list answering it: cast it.` | **`162 seq 52` again.** The model quotes that sentence **five times** in one reasoning body and each time concludes *"Fate Unraveler is 3/4. Power 3. So I cast the sweeper."* — the opposite of what it says. The `cast … at power 4 or more; otherwise keep it unless …` shape puts the verb `cast the sweeper` in front of a threshold and a double negative behind it; the model reads the verb. Splitting it puts KEEP in the power-3 branch as its own affirmative sentence, and the reason the branch exists (the mana IS the counter) is now in the branch |
| 4 | **§YOUR WINDOWS' one answer-shaped quote gains the label.** `take "0 (Pass priority)"` → `take "CHOICE: 0 (Pass priority)"` | SPECULATIVE, stated as such, and 8 bytes: **all six corpus `answer_label_absent` replies are on this deck** and this is the only ~20 KB guide in the pool whose text never contains the literal string `CHOICE:` (deck123 5, deck126 1, deck130 1, deck146 1; deck130's one bare `"0 (pass)"` is paired with a labelled `CHOICE: 0 (pass)` eleven lines earlier). The bare `<n> (<name>)` form the six replies produced is the form this guide shows. It is correlation, not proof — the wave-77 instances predate the line that introduced it — so the edit pairs the label with the form rather than removing the form |
| 5 | Byte payers, no decision rule lost: ORDER step 3's `and mana spent earlier is mana that phase will not have`; §STAFF's ping-target tail `at any life and under any paragraph on the screen` (0 self-pings in 61 pings, 6 waves); §GATE's two worked examples of a zero-verdict row (the rule keeps its sentence); §GATE's `"Mana available:"` exception compressed; §EMRAKUL's because-clause (26 rows offered, 26 declined); §REVELATION stop (1)'s per-permanent arithmetic restatement; §CLEANUP's false-condition sentence compressed | bytes |

**KEEP, explicitly, with the evidence — all three wave-77 edits paid.**
**Edit 1 (Revelation stop (5), never at an upkeep or draw window of your own turn): 16 Revelation casts, ZERO
at an upkeep or draw**, against 6 of 13 last wave. Every one is a main phase (`123` 85/238/257, `126`
194/267/313, `130` 113/132/198/273/286/338, `146` 14, `152` 79/82, `162` 34). The single upkeep Revelation in
the six games is `130 seq 303-305`, where the **heuristic** announced it after refusing the model's reply and
the model then declined it. Placing the rule inside the numbered stop list is confirmed twice now.
**Edit 2 (step 4 is not a step without a printed DISCARD PUNISHERS paragraph): 87 windows whose reasoning
reaches hand size or cleanup, and ZERO dead rows taken in any of them** (wave 77: 2 dead casts, one stopped
only by the engine's `noop_row_zero_reask`). The paragraph was printed for the first time in six waves
(`162`, 31 windows, Liliana's Caress) and the false-condition firing did not recur.
**Edit 3 (the own-turn decline names the row a priority menu prints): 498 own-turn windows offering a hold row,
72 holds — 59 under the printed declined-count override, 9 on menus with no `Cast nothing right now` row, and
4 uncovered violations** (`123` 45/294, `126` 92, `162` 15), against 21 uncovered last wave. Only one hold in
the whole seat costs a live row: `126 seq 488`, t85 of a game won 76–4, where 12 Staff ping rows were on the
menu. The other 8 no-row holds are Elixir ACTIVATION rows at an upkeep priority menu.
Also KEEP: the counterspell table (35 casts, 0 off-list), the ping rules (0 self-pings, 55 of 61 at the face),
the land rule (93/93), EMRAKUL (26/26 declines), the never-discard-a-Staff step (40 cleanups).

**Disobeyed, not misread, so NOT re-capitalised** (the wave-74 ruling): `152 seq 79` is a plain stop-(4)
violation — the reasoning quotes stop (4) and the low-life floor by name, correctly finds the floor does not
lift it at 18 life, and casts at X=2 anyway because 17 unblockable damage was on the crack-back line and
nothing else in hand was castable. Nothing is bought: 2 life and 2 cards did not change a game lost at −2 two
turns later, and the rule that would have to be re-written is the one it read correctly.

## 4. Not checked
No `blockers`, `attackers`, `reveal` or `bottom` seam — this deck fielded no creature in any of the six games
and was asked none of those; all 6 mulligans were `Keep this hand`, so no ship or bottom branch of §MULLIGAN
ran. No `LOOP`, `LIFE-TO-DAMAGE CONVERTER`, `Forced Fruition` or `{... CLEANUP PRICE at X=N ...}` string
appeared in any of the 783 prompts, so §THEIR CONVERTER AND THEIR LOOP, Revelation stop (2) and the
Forced-Fruition paragraph are UNTESTED this wave; I trimmed no rule there. `[second copy:` rendered 11 times
and I did not trace whether the model read it as an upside. I did not trace the 17 `async_drops`, did not open
`askreplay/`, did not reconcile `ask_replays_reserved` / `identical_ask_answers_reserved` or this seat's 3
`forced_close_unrecorded` against stderr or the code, and did not measure the S3 stop-reached or S4 drain
shapes (0 and 1 occurrence here). I opened opponent seat logs for nothing but the results row and reconstruct
no opponent's line of play. I verified exactly one card against `primitives/*.txt` (Triumphant Adventurer,
B-1) and checked it against no external Oracle source; Fate Unraveler, Teferi's Puzzle Box, Underworld Dreams
and Liliana's Caress are quoted from the engine's own `{effect:}` / `{target text:}` renders. I adjudicated no
lane-{CV,CW,CX,CY} prediction beyond the ones my own windows touch (S5, S7, S9(b), S8, S13, S14/CY-F9 above) —
the rest is the engine seat's job. Edit 4 is explicitly speculative and I could not test it. No general-guide
or skill proposals: edits 1–3 are all inside this deck's own guide, and edit 3 is Amendment 334 applied to a
sentence that already existed.
