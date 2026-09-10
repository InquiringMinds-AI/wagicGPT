# Wave 74 — deck 146 (Orzhov dungeon midrange) per-deck review

Seat files: `matchups-20260909-104713-final/*ai_baka_deck146-0x*-vs-*.jsonl` (6 games). **All six of
this seat's games are on the CG binary** (`start_epoch` 1788968851-1788968878, i.e. the 18-game
w74step1b set) — **none of the three CH reruns involve deck 146**, so every hold/latch observation
below is CG-binary evidence only and says nothing about the CH hold-key fix.

Seat census: **355 records, 322 model decisions** (ask 220, priority 70, attackers 29, blockers 2,
reveal 1), **0 fallbacks**, 0 `reply_truncated`, 0 `action_before_plan`, 0 `menu_pass_no_progress`,
native reasoning on all 322 (median 5,403 chars, max 24,734). `plan_line_missing` 37/322 (11.5 %;
wave 73: 11.7 %), `off_protocol_bytes>0` 35/322 (10.9 %; wave 73: 10.5 %). Latency p50 92.2 s /
p95 175.7 s / max 422.3 s. Two `parse_note` records, both recovered (below). Gameend sums for this
seat: `own_turn_windows_skipped` 74, `main_phase_windows_skipped` 26, `hold_windows_skipped` 15
(cast 3 / priority 12), `hold_released_turn` 3, `ask_replays_reserved` 21 / refused 0,
`identical_ask_answers_reserved` 0, `async_drops` 17, `chain_acting_rows` 187 /
`chain_selfharm_rows` 64 / `chain_windows_only_selfharm` 0, `reserve_decline_windows_noted` 2,
`phase2_answer_recovered` 2 / missing 0, `sibling_window_asks_skipped` 0,
`stop_reached_windows_skipped` 0, `chain_windows_collapsed` 0, `repeat_annotated_takes` 0,
`blocker_forecast_rows` 6 (4 multi, 2 gang), `engine_reveal_floor_picks` 0, `plan_names_stranded_card` 0.

| # | Opponent | Result | Turn | Final life | Wave-73 result vs same deck |
|---|---|---|---|---|---|
| 1 | 125 | WIN | 23 | 18 / -10 | WIN |
| 2 | 152 | **LOSS** | 16 | -2 / 13 | LOSS |
| 3 | 126 | WIN | 15 | 14 / -6 | WIN |
| 4 | 130 | WIN | 29 | 11 / 0 | WIN |
| 5 | 162 | **WIN** | 19 | 14 / 0 | LOSS |
| 6 | 123 | WIN | 17 | 17 / -2 | WIN |

**5-1** (wave 73: 4-2). The 162 loss flipped to a win and the wave-73 pathway edit is why (below).
The one loss repeats last wave's opponent and, again, is not variance: it traces to a single cast
choice that **the live guide's own text forbade**. All six mulligans were untouched-7 keeps; no
bottom ask rendered (fifth wave running).

## 0. Did the wave-73 edits pay, and did the wave-74 engine changes land here?

- **Wave-73 Edit 1 (walker activates the turn she lands) — PAID, 6 of 7.** Cast->activate in the
  same turn at deck152 seq 17->18, deck125 seq 17->18 (Lolth) and 21->23 (Kaya), deck162 seq 26->29
  and 34->36, deck130 seq 39->40 and 42->43. The "activate from next turn" belief is **gone from
  every reasoning trace in the seat**. One exception, harmless: deck123 seq 15, where the only rows
  were a targetless +1 and a self-targeting -3, and the seat took HOLD instead (a technical breach
  of the guide's "NEVER hold a list offering a Kaya or Lolth loyalty ability"; game won at T17).
- **Wave-73 Edit 2 (Lolth's Spiders are blockers) — PAID.** deck125 seq 18 and deck162 seq 29 both
  -3 for Spiders the turn she arrived; deck162's Spiders then attacked into the win at seq 38/45.
- **Wave-73 Edit 3 (pathway colour) — PAID and it flipped the 162 game.** The rule is quoted by name
  inside the reasoning at deck152 seq 3 (*"checking the guide: 'A PATHWAY IS ONE CARD WITH TWO LAND
  FACES... TAKE THE FACE THAT MAKES THE COLOUR YOUR HAND IS SHORT OF'"*), and the seat took the
  BLACK face where black was short (deck152 seq 4 Grimclimb, deck123 seq 5 Grimclimb, deck130
  seq 63 Grimclimb). No colour-screw game this wave; the wave-73 vs-162 loss does not recur.
- **Wave-73 Edit 4 (tapped land still goes down) — PAID.** No land drop was declined for tapped-ness
  in 6 games; deck130 seq 20/56 and deck152 seq 32 all played a Hive printing `enters TAPPED`.
- **Wave-73 Edit 5 (cutting the DRAW-PRICE block) — VINDICATED.** `[DRAW PRICE:` rendered **8 times**
  this wave (all in the vs-162 win, Underworld Dreams x2 + Forced Fruition) after 0 renders in three
  waves, and the seat handled every one correctly with no guide rule — including declining a cast
  tagged `draws YOU 7 cards ... this KILLS you` at deck162 seq 51/54.
- **Wave-73 HIGH 1 (forced-sacrifice header on a pump picker) — FIXED, verified both ways.** The
  header now keys on the asking ability: deck152 seq 24 (mode `creature gains 3/3 and sacrifice
  creature`) renders as `TARGET CHOICE ... a gain/pump lands ON the creature you pick`, and
  deck126 seq 33 renders `FORCED SACRIFICE ... the opponent's effect (Tribute to Hunger)` for a
  genuine enemy edict, with a correct per-row `they gain N life (its toughness)` price. No inverted
  picker anywhere in the seat.
- **Wave-73 HIGH 2 (land-drop menu hiding a pathway face) — PARTIALLY fixed; see MED 3.** The
  `Land drop:` row now carries `{TWO FACES, ONE CARD: ... the next window asks which face enters ...
  Choose the face there, not here}` and that annotation demonstrably works. The faces are still not
  rows on that menu and the drop is still decided at two windows (9 occurrences).
- **Wave-73 MED 4 (worked example copying `(menu text: ...)`) — FIXED.** 0 occurrences in 322 prompts.
- **`sibling_window_asks_skipped`, `chain_windows_collapsed`, `stop_reached_windows_skipped` are all
  0 for this seat**; `chain_windows_only_selfharm` 0 with 187 acting / 64 self-harm chain rows. I did
  not read the code — engine seat.

## 1. Game by game — the deciding decisions

**vs 152 (LOSS, T16) — the whole game is `...vs-ai_baka_deck152.jsonl` seq 22 (T13 main 1), and the
GUIDE caused it.** At 13 life, 6 untapped sources, with
`CRACK-BACK NEXT TURN: 4 of their creatures ... for up to 13 ... you would be at 0 or lower; that
would KILL you` printed on the board line, the casting menu was:
```
1. Cast Silverquill Command {2}{b}{w} {right now: returns NOTHING ...}
2. Cast Kaya the Inexorable {3}{b}{w} [legendary: you already control Kaya the Inexorable -
   legend rule: casting this sends one copy to your graveyard (you choose which)] {leaves 1 ...}
3. Cast Pelakka Predation {2}{b} ...
```
The Kaya on the battlefield had **2 loyalty** — she could not pay a -3. Casting row 2 legend-rules
the spent copy away and leaves a **fresh 5-loyalty Kaya who -3s Katilda, Dawnhart Prime (4/4, their
lord) the same turn**, taking 4 power and a counter-engine out of a 13-point lethal swing. The model
read the row correctly and wrote it off in one sentence:
> *"I have Kaya already on the battlefield. I can cast another Kaya? The menu says 'legendary: you
> already control Kaya the Inexorable - legend rule: casting this sends one copy to your graveyard'.
> **So casting the second Kaya is useless.**"*
It took Silverquill Command instead (pump its own 1/1 + an edict the opponent answers with their
worst body), went 13 -> 6 on the crack-back (seq 30), and died at T16. **That reading came from the
guide**: cast-order entry 9's note lists `"[legendary: you already control"` alongside
`"at 0 this does nothing"` as an engine DEAD marker, so the guide told it the row was dead. This is
a STRATEGY item and Edit 1 below — the same shape as wave 73's "activate from next turn": the guide
was the only place the false belief could come from. The proof it is winnable: **two turns later the
seat played exactly that line** (seq 33 cast Kaya #2, seq 34 legend-ruled Kaya #1 away, seq 37 -3
on Katilda) — the guide only permitted it once the row was the ONLY row on the menu.
Minor in the same game: seq 22's reasoning also planned an illegal `-3` off the 2-loyalty Kaya, then
caught itself in the same trace (*"Kaya has 2 loyalty. -3 would put her to -1, she dies"*). Not a
misplay; the loyalty arithmetic surface is adequate.

**vs 125 (WIN T23) — the win is clean; the interface cost is HIGH 1.** Creatureless permission
again. Good play throughout (seq 17->18 Lolth + -3 on arrival, seq 34/46 Hive animations at the
guide's affordability, seq 49 Nadaar for lethal). But turn 21 alone put the SAME 3-row casting menu
to the model **8 times** and turn 23 **5 more** (HIGH 1). Both `parse_note` records live here and
both recovered: **seq 46** answered two `CHOICE:` lines for two separate Hive animations
(`extra_answer_line;latched_row_mismatch`; the engine took row 1, correct); **seq 49** wrote
`CHOICE: 3 (Cast Nadaar, Selfless Paladin)` where Nadaar was row 4
(`echo_index_conflict;name_over_index`; the name won and Nadaar was cast, which is what the plan
line asked for). Neither was an ANNOUNCE_X window, so neither bears on O19.

**vs 162 (WIN T19, wave 73's loss) — reversed by the pathway rule and by DRAW PRICE discipline.**
seq 5 took the pathway face window with the colour rule in hand, the curve came out on time
(Adventurer T5, Acererak T7, Soul Shatter T9), and at T15-T19 the seat declined every draw effect
the two Underworld Dreams were pricing while attacking for the win.

**vs 126 (WIN T15).** Vanishing Verse on Wall of Omens (seq 16), Soul Shatter (seq 18), Silencer
naming **Exquisite Blood** at seq 27 — the Sanguine-Bond rule firing as a Silencer name is the one
place the guide says NOT to use Silencer, but the row it took was the highest-N row anyway and the
game was already won on board. seq 33 answered a genuine Tribute to Hunger edict by binning the
tapped Goblin (the cheapest toughness) — correct.

**vs 130 (WIN T29), vs 123 (WIN T17).** Clean. deck130 is the long grind: two Kayas cast and -3'd
in sequence (seq 39/40, 42/43), Acererak used as a repeatable venture exactly as the guide frames
him (seq 52, 57, 61), Grimclimb taken for {B} at seq 63.

## 2. Engine / interface / card items

### HIGH 1 — `[hold check:]` calls a row NEW when only its mana-count annotation moved, and the guide-correct answer is then to decline; result: the same dead menu re-put 8 times in one turn
`...vs-ai_baka_deck125.jsonl` **seqs 65, 66, 70, 72, 75** (turn 21) and **90, 91, 94, 98** (turn 23).
The menu is identical text at every one of those windows: `1. Cast Soul Shatter {2}{b} {right now:
they control 0 creatures or planeswalkers - at 0 this does nothing} ...`, `2. Hold priority ...`,
`3. Cast nothing right now`. The trailer oscillates between the two hold-check verdicts on that
unchanged list:
```
seq 65: [hold check: every row above was also on the menu at the last window at this seam (1 window in a row now) ...]
seq 66: [hold check: 1 row above is new since the last window at this seam - a row moving is what re-opens a hold ...]
seq 72: [hold check: 1 row above is new ...]   [you declined this exact list 6 times already this turn]
seq 75: [hold check: every row above was also on the menu ...]   [you declined this exact list 7 times already this turn]
```
The only thing that moved between those windows is the Soul Shatter row's own annotation
`{leaves N of your M untapped mana sources untapped}`, which tracks mana spent elsewhere. So the
hold-check key is **annotation-sensitive** — exactly the class CH fixed for the hold LATCH key
(action identity, render annotations stripped), applied at a different surface and not fixed there.
Consequence: the guide instructs "N rows above are new -> decline this window only", so the model
declines, correctly, and the run continues. **`declined this exact list N times` reached 7 in this
seat** (wave-73 ceiling was 3) — a regression on the O11 axis, and it also shows the O11 decline tag
did NOT move the behaviour: the row printed `{this same question will be asked again this turn:
taking this row closes this window only, and you have already declined this exact list 6 times this
turn}` and the model took it anyway, at seqs 66, 70, 72 and 75. Cost here: ~13 wasted model calls in
two turns at p50 92 s each. Fix: key the hold-check "new row" count on the same stripped action
identity CH uses for the latch.

### HIGH 2 — `reasoning_degenerate` cannot see the degeneracy that is actually present (319 of 322 traces)
`reasoning_degenerate` is a repetition RATIO (`reasoningRepetitionRatio`, AIPlayerGPT.cpp: best
exact-40-char window / total windows). Across this seat it is **median 0.0034, max 0.0112, zero
records above 0.5** — i.e. it reads clean on every decision. Meanwhile **319 of 322 traces** re-quote
the reply-protocol block verbatim inside the reasoning and loop self-verification boilerplate; a
typical tail (deck152 seq 23, 5.3 KB of reasoning) is
`"[Final Check] - Option 4 is chosen. - Format matches. - No extra text. - Correct. Done. [Output
Generation] (Proceeds) ... (Self-Correction/Note during generation prep) The prompt says 'Write your
PLAN: line first ...' I will write: ... Done. Proceeds. [Output] -> *Proceeds*"`.
The loops are near-duplicates with small edits, so a byte-exact 40-char window never repeats and the
meter is blind by construction. Two items for the core loop: (a) the instrument needs a normalised
or shingled comparison before any wave can claim reasoning is healthy; (b) a large share of a median
5.4 KB reasoning budget is being spent re-deriving the reply format rather than on the board. Repro:
any record; `deck152` seq 22, 23, 29, 30 are the clearest.

### MED 3 — the land drop is still decided at TWO windows, and the second one is where the faces live
Nine repro pairs: deck125 seq 3->4->5 and 27->28, deck152 seq 3->4, deck126 seq 4->5, deck130
seq 62->63, deck162 seq 4->5, deck123 seq 4->5, 7->8 and 32->33. The first window offers
`Play Brightclimb Pathway {TWO FACES, ONE CARD: ... the next window asks which face enters ...
Choose the face there, not here}`; the second offers `Grimclimb Pathway` / `Brightclimb Pathway
(menu text: Play Land)` / `Decline - do nothing`. The O4 annotation **works** — it is what lets the
seat switch to the black face at deck152 seq 4 and deck130 seq 63 — so this is downgraded from
wave 73's HIGH to MED, but it is still one land drop costing two model calls (18 of this seat's 322
decisions), and the second window's `Decline - do nothing` row can un-make a drop the first window
already committed. Fix as before: put both faces on the `Land drop:` menu itself.

### MED 4 — the HOLD row has grown to ~1,100 bytes and is now the largest single object on most menus
deck125 seq 72 is a 3-row menu; the hold row alone is 1,074 bytes of the options text — more than
row 1 and row 3 combined, on a window where the only live question is "do you cast a dead Soul
Shatter". Wave 73 flagged this at ~450 bytes; it has more than doubled. Holds taken in this seat:
**11 of 322 decisions**. The paragraph is now paying ~1 KB per casting/priority window (roughly
290 windows) for 11 uses.

### LOW 5 — `ask_replays_reserved` 21 = 21 seq numbers reserved with no model call, and it is working
Missing seq numbers exactly match the counter per game: deck125 12 missing (12 reserved), deck130 4
(4), deck162 5 (5), and the three games with `ask_replays_reserved` 0 have contiguous seqs. So the
CG replay path is live in this seat and answered 21 identical re-puts without a model call, with 0
refusals. `identical_ask_answers_reserved` is **0** here, which is consistent with the brief's
corpus-wide 1: the two counters name different paths and only the replay path is firing. This is the
counter-balance to HIGH 1 — replay caught 21 re-puts, and the ones it did NOT catch are precisely
the ones whose mana annotation had moved.

### LOW 6 — `plan_line_missing` shape unchanged from wave 73
34 of 37 are a plan SENTENCE with the literal `PLAN:` label dropped (deck125 seq 20 `Play Shineshadow
Snarl. Cast Silverquill Silencer and Acererak the Archlich.`, seq 29, seq 47, seq 54; deck152 seq 22,
23); 3 are action-line only. All parsed, 0 fallbacks, 0 `action_before_plan`. `off_protocol_bytes`
is small in every case (33 of 35 under 100 bytes) and is the same missing-label residue.

### LOW 7 — `main_phase_windows_skipped` 26 for this seat looks honest
26 skips against ~59 of the seat's own turns. The vs-162 game (which the seat spent casting on
curve) counted 0, and the two long games counted 16 and 8 — the meter now tracks games where the
seat actually had nothing to do, which is the opposite of wave 73's suspected under-count in the
vs-162 game. Cross-check against the casting records is the engine seat's job.

## 3. Guide verdict: EDIT

Live guide 19,939 bytes -> revised **19,989 bytes** (`wave74/deck146/strategy.txt`). Two rules added;
one false clause deleted; nine compressions and one dead block pay for them (net +50 bytes).

**Edit 1 — DELETE the false dead-marker `"[legendary: you already control"`, and TEACH THE RESET
(the rule that earned its bytes).**
- before, cast-order entry 9's note: `unless the engine marked a row dead ("at 0 this does nothing",
  "[legendary: you already control"), the reason a row does not match must be a fact on THIS screen.`
- after: `unless the engine marked a row dead ("at 0 this does nothing"), the reason a row does not
  match must be a fact on THIS screen.`
- and added under PLANESWALKERS: `- A SECOND KAYA OR LOLTH IN HAND IS A LOYALTY RESET, NOT A DEAD
  CARD. A cast row tagged "[legendary: you already control ...]" is LIVE: casting it bins ONE copy
  (you pick) and the survivor is the NEW one, at FULL loyalty. When the copy on the battlefield is
  below 3 loyalty - so it cannot pay a -3 - and a crack-back line is aimed at you, CAST THE SECOND
  COPY, bin the spent one at the legend-rule ask, and -3 the fresh one the same turn. That is the
  only way this deck fires a second -3; skipping the row as "legendary, useless" loses the game.`
- paid for by **deck152 seq 22** (the losing decision, whose reasoning quotes the legend-rule
  bracket and concludes "so casting the second Kaya is useless"), and proven playable by the same
  seat at **deck152 seq 33 -> 34 -> 37** (cast Kaya #2, legend-ruled the 2-loyalty copy away, -3 on
  Katilda) and **deck130 seq 39/40, 42/43** (two Kayas cast and -3'd in one game). The deleted
  clause was the guide teaching a false fact about a live row — the same failure class as wave 73's
  "activate from next turn".
- knock-on: CLEANUP DISCARD step (2) previously read "a second Kaya or Lolth is dead in hand and
  goes before any live card"; it now reads `unless the copy you control is a walker below 3 loyalty,
  which makes the one in hand a reset (see KAYA)`, so the two sections cannot contradict.

**Edit 2 — the HOLD paragraph names the decline-run failure.**
- added: `A RUN OF DECLINES IS THE FAILURE MODE: once a decline row says you have already declined
  this exact list 2 or more times this turn, and nothing on it will become payable this turn, TAKE
  HOLD. A "[hold check: N rows above are new]" line does NOT override that - a row whose only change
  is a count of your untapped mana sources is the same row.`
- paid for by **deck125 seqs 65/66/70/72/75** (declines 3 -> 7 on one unchanged dead menu at T21)
  and **seqs 90/91/94/98** (T23). Without it the guide's existing `"N rows above are new" -> decline
  this window only` rule keeps the seat inside HIGH 1's loop for as long as the engine mislabels the
  row. This is a guide-side mitigation, not a substitute for the HIGH 1 engine fix.

**Edit 3 — the bytes.** CUT the BOTTOM ASK block entirely (~430 bytes): it has rendered **0 times in
five consecutive waves** (all mulligan windows in this seat, four waves running, are untouched-7
keeps, so no bottom ask is reachable). Compressed with no rule removed: CLEANUP DISCARD steps 1-3,
the hand-header double-count note, the Verse/Predation/Silencer/Hive bullets, the combat
"attacker dies" and GANG BLOCK bullets, the converter bullet, the mulligan colour and carried-plan
bullets, the counter-and-gain-life block, and the deck header. No cut removed a rule; `[DRAW PRICE:`
stays cut and this wave's 8 renders were all handled correctly without it.

## 4. What I did NOT check
- **All six of my games are CG-binary**; I have no CH-binary evidence, so I cannot say whether the CH
  hold-key fix would close HIGH 1's oscillation. Engine seat, split by binary.
- The 17 `async_drops` (8 in vs 125, 4 in vs 130, 2 each in vs 123/162, 1 in vs 126) — not traced to
  a seam; I confirmed only that no seq gap in this seat is unaccounted for once
  `ask_replays_reserved` is subtracted, and that no decision fell to the heuristic (0 fallbacks).
- The 2 `phase2_answer_recovered` (deck152, deck126) — recovered, contents not read.
- Opponent seat files beyond board context; no audit of any opponent deck's play.
- I did not read the engine code for `sibling_window_asks_skipped` / `chain_windows_collapsed` /
  `stop_reached_windows_skipped`, all 0 here; nor for `chain_acting_rows` 187 vs
  `chain_selfharm_rows` 64 (O13's share question) — measure only, engine seat.
- No ANNOUNCE_X window rendered in this seat, so **O19 is UNTESTED here**; likewise `ATTACK TOTAL`
  rendered 29 times but I did not verify the converter arithmetic against the board (O5), and the
  ghostform gloss (O1) never rendered in these six games.
- BOTTOM ASK: 0 windows for a fifth wave. BLOCK LADDER: **two** blocker decisions in six games
  (deck152 seq 30, deck130 seq 45) — still effectively untested; `[repeat: activated ...]` 0 renders.
- Whether HIGH 1's oscillating hold-check has cost a game in any other seat — here it cost calls and
  latency, not the game.

No `general-proposals.md` and no `skill-proposals.md`. Both edits are
wave68/strategy-writing-skill-v2.md's existing discipline applied (one deletes a false rules claim
the guide itself introduced; one names a failure mode the corpus shows recurring) — neither is a gap
in the method the skill teaches.
