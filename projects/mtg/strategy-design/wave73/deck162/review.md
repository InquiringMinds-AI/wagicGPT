# Wave 73 — deck 162 ("Draw and Die!", UB forced-draw punish) per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-225637-final`, binary source `622b10827`,
`--thinking on`. Seat files `*-ai_baka_deck162-0x*-vs-*.jsonl` (6).
**115 model decisions, 0 fallbacks, 0 `reply_truncated`, 0 `action_before_plan`, reasoning on 115/115**
(`reasoning_chars` 1,656 / 5,950 / 13,511 min-med-max). **6 records with `plan_line_missing` (5.2%;
wave 72 was 1 of 103)**. Record **4-2**: beat 126 (10–0 t17), 125 (19–0 t14), 130 (13–0 t14),
146 (20–−6 t14); lost to 152 (−7–16 t11) and 123 (−3–18 t10). Wave 72 was 3-3 with the parent guide;
the two decks it lost to last wave (126, 146) are both wins now.

## 1. Game by game

| Opp | Result | Deciding decisions |
|---|---|---|
| 126 | **WIN** t17 | Fog Bank t6, three Fate Unravelers t8/10/12, Mine t12, Dreams t14, Master t14, Puzzle Box + 2nd Mine t16. Their Sanguine Bond + two Tribute to Hunger cost the seat Fog Bank and one Unraveler; the forced-sacrifice order (seq 15 Fog Bank, seq 16 the *tapped* Unraveler) is exactly §FORCED SACRIFICE. seq 20/24 `ATTACK: none` under a printed LIFE-TO-DAMAGE CONVERTER with blocking-trigger prices on every tag — the §ATTACKING converter rule, correctly applied; the punishers won it without combat. |
| 125 | **WIN** t14 | First Mine at K=0 t4 (sanctioned), Dreams t6 and t8, Master + 2nd Mine t10, Dictate t12, then seq 22/23 Peer into the Abyss with the modal answer `the opponent`. Both STRANDS rows (seq 12, seq 20) declined. |
| 130 | **WIN** t14 | First Mine at K=0 t3, Dreams t7, Dictate t9, Unraveler t11, Fog Bank t13, attack t13 for the kill. **seq 11 took a STRANDS row** — see below; the game was won anyway. |
| 146 | **WIN** t14 | Shield Sphere t2 and t8, Unraveler t8, Master t10, Dreams t12, attacks t12/t14. Four STRANDS declines (seq 11, 15, 18, and the two replayed windows), one STRANDS take at seq 25 with lethal already on board. |
| 152 | LOSS t11, −7–16 | Elite Spellbinder **exiled Fate Unraveler out of hand** t7 (log, seq 11 prompt); the seat then drew four lands in five turns and its whole hand at t10 was `Dismal Backwater; Island; Underground Sea; Liliana's Caress`. seq 8 and seq 10 are both rung-2 `(neither dies)` Shield Sphere blocks, per guide. seq 12 casts the only castable card. **Flood + targeted discard, not guide.** |
| 123 | LOSS t10, −3–18 | Thraben Doomsayer + Intruder Alarm again: 1 creature t7 → 25 t9 → 26 t10. seq 11's `CRACK-BACK ... for up to 26 - you would be at -5` fired rule 0 and the only creature row (Master of the Feast) was taken; the row's own `{crack-back cover:}` said it still leaves −3. seq 12's engine header printed `no block saves you - no assignment of your blockers survives this` and the seat took the minimum-damage assignment. **Matchup, not guide** — same shape as wave 72's 123 loss. |

## 2. Recurrence check against wave 72's review

- **Wave-72 edit A (K-of-0 brake, all engine names) — PASS, unambiguously.** 14 `{feeds:}` rows were taken
  across the seat; the only two at `on your battlefield: 0` are the FIRST Howling Mine (125 seq 6 t4,
  130 seq 5 t3), which the branch permits. Zero Dictate/Puzzle Box/Fruition/Master casts at K of 0 —
  the exact failure that lost 146 last wave did not recur, and the seat beat 146 this wave.
- **Wave-72 edit B (Ob Nixilis legend exception) — UNTESTED.** No Ob Nixilis was ever cast; the two copies
  sat uncastable in hand in the 123 game.
- **Wave-72 edit C (Sanguine Bond / the −2 does not enter the chain) — UNTESTED.** Only ONE half
  (Sanguine Bond) appeared, in the 126 game; there was no `LOOP COMPLETE` prompt and no Ob Nixilis. The
  wave-72 HIGH render item (LOOP COMPLETE contradicting LOOP SCOPE) therefore **does not recur in this
  seat** — I cannot say it is fixed, only that it is not exercised here.
- **`{reserve:}` survive-the-re-ask — RECURS, and is the one real misplay of the wave.** Wave 72 was
  3 STRANDS rows / 3 declined. Wave 73 is **6 STRANDS rows / 4 declined, 2 taken** (detail below).
- **`plan_line_missing` at the same seam — RECURS and grew** (1/103 → 6/115). Same shape every time.

## 3. Engine / interface / card items

**HIGH — the guide's K-of-1 branch outranks the `{reserve:}` STRANDS verdict in the model's own words,
and the decision it produced is self-contradictory.**
`1788839828-ai_baka_deck162-0x561b64093bd0-vs-ai_baka_deck130.jsonl` **seq 11** (t9 Draw step, Howling
Mine's trigger on the stack, 3 sources). Row 1 rendered:
`Cast Dictate of Kruphix {1}{u}{u} {leaves 0 of your 3 untapped mana sources untapped - casting this taps
you out} {reserve: TAKE THIS ROW AND YOU CANNOT CAST Master of the Feast AT ALL THIS TURN ... VERDICT:
taking this row STRANDS Master of the Feast this turn - 1 source cannot pay 3, and it has no window after
your main phase}`. The **identical row was declined one window earlier at seq 10** (t9 Upkeep,
`CHOICE: 3 (Cast nothing right now)`). At seq 11 the reasoning quotes the K branch as the authority —
*"the guide says: 'K (A BATTLEFIELD COUNT) OF 1 OR MORE: cast every engine offered, every turn' ...
Underworld Dreams is on the battlefield, so K=1 ... So I cast Dictate"* — reads the reserve clause
(*"If I cast it, I have 0 mana ... So I can't cast Master this turn"*) and takes the row anyway. It then
writes `PLAN: Cast Dictate of Kruphix, then play a land and cast Master of the Feast in main phase 1` —
a plan the row it just took has explicitly denied, and Master of the Feast was never cast in that game.
This is STRATEGY (the surface was correct and complete), and the guide's own override sentence
("This outranks BOTH K branches") lives in a *different section* from the branch the model quoted.
Paid edit A below. Note for the engine seat: a PLAN line that names a card the chosen row's reserve
clause says cannot be cast this turn is mechanically detectable and would be a cheap protocol check.

**MED — a STRANDS row taken with lethal already on the board, and the reasoning is right.**
`...deck162-0x557a6c3fbd80-vs-ai_baka_deck146.jsonl` **seq 25** (t14 Upkeep, opponent at 4, seat has
Fate Unraveler + Master of the Feast untapped = 8 power). Same Dictate STRANDS row, taken; the reasoning:
*"Since I win in combat anyway, stranding MotF is irrelevant."* It did win that turn. Recorded not as a
misplay but because it is the second half of the pattern: the absolute rule is being *reasoned around*,
not obeyed, and the argument that works here (seq 25) is the argument shape that failed at 130 seq 11.
I deliberately did **not** write the "unless you have lethal" exception into the guide — one escape hatch
is what re-opens the rule everywhere.

**MED — the three `Hold priority` takes contradict the live guide, and the guide is the thing that is
wrong.** 146 seq 10 (t7 Upkeep, opponent's turn), 125 seq 20, 130 seq 13. In all three the
`[you declined this exact list N times already this turn]` line the guide demands is **absent** — only
`[hold check: ... N window(s) in a row]` was printed, which the guide explicitly says is NOT that line —
so the guide's HOLD paragraph says the answer should have been "Cast nothing right now". All three holds
were harmless or better: 146 seq 10's hold **skipped 8 cast windows** (`hold_windows_skipped 8`, all
`_cast`) on a menu whose only row was a K-of-0 braked Dictate, and it is the seat's only
`hold_released_turn` (1) — taken on the *opponent's* turn t7 and released at the seat's own untap t8,
exactly the wave-73 semantics. The 125 and 130 holds dissolved at the next window because a row moved
(both games then cast normally in main phase 1: 125 seq 22 Peer, 130 seq 15 Fate Unraveler), which is why
their gameends read `hold_released_turn 0` / `hold_windows_skipped 0` — no window was lost, but note that
the row's absolute text (`YOU CANNOT COME BACK AND TAKE ONE OF THE ROWS ABOVE LATER THIS TURN`) was in
fact false in both, because the escape clause that saved it is 200 words further down the same row.
Paid edit B below.

**LOW — `plan_line_missing` fires 6 times, always as the bare plan sentence with the label dropped.**
126 seq 23 (`'\n\nCast Master of the Feast to add a draw engine and trigger upkeep damage.\nCHOICE: 1 ...'`,
72 off-protocol bytes), 126 seq 26 (24 B), 125 seq 21 (79 B), 130 seq 19 (134 B), 130 seq 20 (28 B), and
126 seq 24 which answered `'\n\nATTACK: none'` with no plan sentence at all. Every answer line parsed and
every choice was legal. Rate is up from wave 72 (1.0% → 5.2%) but it is one shape, not a new one; a
lowercase `Plan:` (126 seq 20) is accepted by the parser and not counted.

**LOW — the cleanup-discard ask was answered against the guide's named order, and against its own
annotation.** 130 **seq 20**: hand of 8, the guide says bottom/discard "your most EXPENSIVE spell first -
Forced Fruition, Peer into the Abyss, Puzzle Box". Row 4 of that very prompt reads
`the most expensive card in your hand you could still reach costs 4; Forced Fruition at 6 is NOT counted
here - it is past what 3 lands plus the drops this hand can still make would pay for`. The seat discarded
Teferi's Puzzle Box (row 3, castable at 4) and kept the uncastable Forced Fruition. Cost nothing — the
game ended the next turn — and the guide already says the right thing, so no edit.

**Not defects, recorded as passes**: `ask_replays_reserved` reused seq 18's STRANDS **decline** for the two
identical windows at 146 seq 19/20 (`askreplay/` sidecar, `why: repeat_ask_reserved`) — the replay path
reserved the correct answer, and `reserve_decline_windows_skipped 1` in the same game shows the re-armed
decline latch firing. `main_phase_windows_skipped` totals 4 across the seat, all in the 146 game and all
inside that one hold — plausible for this seat, though 4 windows cannot speak to the corpus's 375.
`chain_windows_collapsed 0` and `sibling_window_asks_skipped 0` in all six games; this seat's own mana
policy always found a legal action or a pass row, so I cannot say whether the collapse path is reachable.
`stop_reached_windows_skipped 0` in all six — no PLAN of this seat ever wrote a parseable stop count.
Card facts verified against `bin/Res/sets/primitives/mtg.txt`: Fate Unraveler (`auto=@drawfoeof(player):
damage:1 opponent`, {3}{B} 3/4) and Master of the Feast (`auto=@each my upkeep:draw:1 opponent`, {1}{B}{B}
5/5 flying) both match Oracle and match what the prompts rendered; no card behaved contrary to its
rendered text in these six games.

## 4. Guide verdict: **EDIT**

`wave73/deck162/strategy.txt` — **19,918 bytes, from 19,997 (net −79)**, under the ceiling. Two adds,
both paid; the bytes come from three cuts named below.

| # | before → after | paid for by |
|---|---|---|
| A | K-of-1 bullet `... a punisher ALSO in the list is cast first, engines with what is left.` → same, plus `"EVERY engine" never reaches a row whose VERDICT begins "taking this row STRANDS" - that row is refused at every K, on every board, and with the game already won; "K is 1, so cast every engine" is not an answer to a strand.` | 130 **seq 11**, reasoning verbatim: *"K (A BATTLEFIELD COUNT) OF 1 OR MORE: cast every engine offered, every turn ... So I cast Dictate"*, taken one window after declining the identical row. The override existed only in the `{reserve:}` section; it now sits in the branch the model actually quotes. Second instance 146 seq 25. |
| B | `THE HOLD-PRIORITY ROW ... Passing AND "[you declined this exact list N times already this turn]" printed -> answer the HOLD row; that line absent -> "Cast nothing right now"; a "[hold check: ...]" line is NOT that line.` → the hold `ends on its own at the start of your next turn - so it can never cost you a turn, and any row that moves re-opens it`, and: your answer is a pass AND every row above is braked/stranded/unaffordable → **take the HOLD row**; keep "Cast nothing right now" for a menu you expect to improve this turn (a land drop, or a spell resolving into the mana a row needs) | 146 **seq 10** (hold skipped 8 dead windows on a K-of-0 menu and released at the seat's own untap — `hold_released_turn 1`), plus 125 seq 20 and 130 seq 13. The old rule keyed on a line that was printed in none of the three, and the wave-73 change (a hold expires at the holder's untap) makes its caution obsolete. |

**Bytes returned:** (i) the `NO {reserve:} CLAUSE PRINTED -> subtract yourself` paragraph — it tells the
seat to re-derive a mana reservation from `Mana available`, the land drop and the `{leaves N of your M}`
count, which the §LAND DROP AND MANA section forbids in the same guide, and every flashed-engine row in
this seat's corpus carried a printed `{reserve:}` clause, so it never had a case to answer; (ii) the
MULLIGAN "tiebreakers between hands that already clear the land count" sentence, which describes a choice
between two hands that no mulligan ask offers; (iii) the Puzzle Box card-fact tail about a
"draw converters in your hand" count read in your own draw step, a case that has not appeared.

**Not changed, deliberately:** the K-of-0 brake (14 `{feeds:}` takes, 0 violations — a success verdict,
it is doing exactly the job wave 72 bought it for); CAST ORDER rule 0 BODY TURN (123 seq 11); the
LIFE-TO-DAMAGE CONVERTER attacking rule (126 seq 20/24 vs Sanguine Bond); FORCED SACRIFICE (126 seq 15/16,
both rows in guide order); BLOCKING rungs (152 seq 8/10, 130 seq 16, 123 seq 12); LAND DROP (38 of 38 land-drop
asks answered with a land, 0 declines); Peer into the Abyss ungated + the modal answer (125 seq 22/23);
MULLIGAN (6 of 6 keeps on two-plus-lands sevens).

## 5. Not checked / limits

- Only this seat's six translogs plus the six opponent seats for outcomes. No lane adjudication, no other
  deck's seats, no corpus-wide counters beyond this seat's six gameends.
- **Wave-72 edits B and C are UNTESTED here, not passed** — no Ob Nixilis was cast in 115 decisions and no
  both-halves life loop appeared. The wave-72 HIGH `LOOP COMPLETE` render item is likewise unexercised in
  this seat; I did not read the render code to see whether it was changed.
- `own_turn_windows_skipped` totals 150 across the six games and no record exists for a skipped window, so
  "no lost window" is an absence of evidence. Same for `main_phase_windows_skipped` — my "plausible" is
  about this seat's 4, not the corpus's 375.
- The 130 seq 11 counterfactual (that declining the Dictate and casting Master of the Feast is better) is
  argued from the reserve clause and the seat's own falsified plan, not measured; that game was won.
- The 152 loss is called flood + Elite Spellbinder's hand-exile on the strength of the t7 log line and the
  all-land t10 hand; I did not model whether a different t2–t6 line survives. The 123 loss rests on the
  engine's own `no block saves you` header.
- I did not audit latency, `async_drops`, or `transport`, and I did not open the engine source.
- No general-guide or skill proposals: edit A is a deck-162 sectioning fault (the override was in the
  wrong section of *this* guide), and edit B follows a wave-73 engine change rather than a general
  principle. Neither generalises without restating an existing amendment.
