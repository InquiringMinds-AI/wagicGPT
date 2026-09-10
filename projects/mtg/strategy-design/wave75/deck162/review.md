# Wave 75 — deck 162 ("Draw and Die!", UB forced-draw punish) per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-003556-final`, SINGLE binary
`wagic-7a04cd6da-w75step1` (source `14fea99f3`), `--thinking on`. Seat files
`*-ai_baka_deck162-0x*-vs-*.jsonl` (6), opponent seats read for outcomes, six stderr files read.

**116 model decisions, 0 fallbacks of any kind, 0 `reply_truncated`, 0 `action_before_plan`, native
reasoning on 116/116** (`reasoning_chars` 1,993 / 5,461 / 20,789 min-med-max). **6 `plan_line_missing`
(5.2%)** and **6 `protocol_deviation_replies`** are the SAME six records (4 `unlabelled_plan` carrying
`off_protocol_bytes` 28/64/72/94 = the length of the unlabelled sentence, 2 `plan_absent` at 0 bytes) —
CL P23d's one-event class holds at 6/6 here. `plan_names_stranded_card` 0, `plan_names_uncastable_zone_card`
0, `identical_ask_answers_reserved` 0, `ask_replays_reserved` 1.

Record **3-3** (wave 74 was 4-2, a different pairing of results): beat 125 (20–0, t23), 123 (21–0, t13),
130 (14–0, t12); lost to 146 (−2–15, t13), 152 (−5–19, t13), 126 (0–40, t13). Two of the three losses are
hand/mana variance (below); **one, 152, is a guide failure at a single blocker window.**

## 1. Game by game

| Opp | Result | Deciding decisions |
|---|---|---|
| 152 | **LOSS** t13 | **seq15 (t9 Blockers) is the loss, and seq26 (t12 MP1) is the second half of it.** seq15: life 13, header `NOT lethal: block only where the trade favors you`, `DRAW PUNISHERS on the battlefield: yours - Fate Unraveler` (exactly one), and the sole blocker is that Fate Unraveler with two lines offered — `A1 (your blocker dies, attacker lives)` on a 5/5 Wolf and `A2 (you kill it, your blocker lives)` on a 1/1 Lair of the Hydra. §BLOCKING rule (a) puts the dying line OFF THE MENU below the lethal check. The seat blocked A1, spent its only punisher for 4 life, and never drew another; it made no further damage all game and died on t13. Reasoning verbatim, consecutive lines: *"Only if I think Fate Unraveler is more valuable than 4 life. / But the guide's rule is explicit. / 'even when that line kills your blocker'. / So I should follow the rule."* and, on the punisher: *"If Fate dies, I lose the punisher. / But the guide says 'even when that line kills your blocker'. / So life total is more important."* It quotes that clause **six times** in one trace and rule (a) not once — the words it quoted are rung 0's own **concessive clause**, and rungs 0 and 3 BOTH fire on this board (unblocked leaves 7, which is ≤ 8), while rule (a)'s only tie to them was the bare cross-reference `Bounded by (a) and (b)`. **Guide item, paid edit A.** seq26: t12, 6 life, 6 untapped sources, `Cast Howling Mine {2}` on the menu, no Mine on the battlefield, K = 0. The seat found the carve-out and then reversed itself: *"is there any chance I should cast Howling Mine as the 'FIRST HOWLING MINE'? The guide says: 'K (BATTLEFIELD) OF 0: the ONE engine you cast here is your FIRST HOWLING MINE.' But then it says: 'A Dictate, Puzzle Box or Forced Fruition row at K of 0 is BRAKED.' Howling Mine is an engine."* It answered `CHOICE: 5 (Cast nothing right now)` with six mana idle and died next turn. **Guide item, paid edit B.** |
| 146 | **LOSS** t13 | Not guide-addressable. The seat drew no creature until t10 (Fate Unraveler, seq11, correct at rung 1a) against a board that was at 3 attackers by t8; at t12 (seq13-15, life 4, `that would KILL you` printed) the hand held Forced Fruition, two Howling Mines, Peer and two lands and **no creature at all**, so §CAST ORDER rung 0 (BODY TURN) had no row to take. seq12 (t11 blockers) was correct on every rung: three attackers, one blocker, took `A2 (you kill it, your blocker lives)` on the biggest non-deathtouch attacker. |
| 126 | **LOSS** t13 | Mana screw: two Drowned Catacombs for the whole game, `Land drop: you have no land you could play right now` from t10 on, so nothing above three mana was ever castable. **seq10 (t11, Tribute to Hunger) is the guide item.** Sanguine Bond was out (`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs`), rows `1./2. Shield Sphere (0/6) ... [they gain 6 life (its toughness)]` and `3. Fog Bank (0/2) ... [they gain 2 life]`. §FORCED SACRIFICE's ORDER names SHIELD SPHERE first and adds `THE LIFE THEY GAIN NEVER DECIDES THIS ROW`; under the converter that row is a 12-point swing against a 4-point one. The seat answered `CHOICE: 3 (Fog Bank)` — correct — but only after ~1,900 characters fighting the guide (*"The guide says 'ORDER: SHIELD SPHERE first'. I will follow the explicit ordering if possible, but 6 life gain with Sanguine Bond is lethal threat"*, four times), and it broke the tie on the PROMPT's header (*"'the one that pays the least'. So paying the least is explicitly stated"*), not on the guide. A guide that has to be overridden to play correctly is a guide item. **Paid edit C.** seq12 (t12 Cleanup) discarded Master of the Feast {1}{b}{b} while holding two Teferi's Puzzle Box {4}: the guide's discard order is "most expensive first", reached only through the phrase `CLEANUP DISCARD is that order from the end`. **Paid edit D.** t13 was unwinnable: both loop halves live, three lifelink attackers, two walls. |
| 123 | **WIN** t13 | Caress t5, Mine t7, second Caress t7, Underworld Dreams t9, Fog Bank t9, Ob Nixilis t11, `-2` on **Bloodline Keeper** (seq15) — the guide's "aim it FIRST at a creature whose text makes more creatures" applied by name — then Peer into the Abyss at t13 (seq17) answered `CHOICE: 1 (the opponent)` at seq18. Both §CARD FACTS rules that decided the game fired. seq17 also carried a `[legendary: you already control Ob Nixilis ...]` row at loyalty 5; the seat correctly left it and took the lethal instead. |
| 130 | **WIN** t12 | Mine t3, Underworld Dreams t5, Master of the Feast t7 at **K = 0** (rung 4b, the wave-74 edit — their line read 1 creature), attacks t9 and t11 on clean unblockable tags, second Mine t11 at K = 1. `forced_close_unrecorded` 1 in this game (below). |
| 125 | **WIN** t23 | Shield Sphere t4, Master t6 (again K = 0, rung 4b), Ob Nixilis t8, Fate Unraveler t10 and t12, two Forced Fruitions t14/t16 at K ≥ 1, Dictate t18, Master t18, Mine t20; opponent from 23 to 0 over t18-t23 on the draw steps. The rung-4b edit is exercised in three of six games and cost nothing in any of them. |

### Wave-74 items that recur, and the two that do not

- **Wave 74's HIGH (the hold latch re-opening on its own decline annotation; 11 asks in one turn, 10 declines)
  DOES NOT RECUR.** Max asks in any one (turn, phase) across all six games is **3**; the highest
  `[you declined this exact list N times]` reached is **2** (125 t18, 152 t10). No re-put window anywhere.
- **Wave 74's MED (three STRANDS rows taken because the check read as §ATTACKING's) DOES NOT RECUR.** Two
  STRANDS rows offered this wave (152 seq16, seq24), **0 taken**, `plan_names_stranded_card` 0. Wave-74
  edit A is doing its job; nothing of it was cut.
- **The `off_protocol_bytes` = `plan_line_missing` identity recurs**, 6/6.
- **The K-of-0 brake recurs as a guide defect in a new place** (edit B) and the §FORCED SACRIFICE
  life-tie-break sentence rewritten in wave 74 (edit C then) recurs as **wrong under a converter** (edit C now).

## 2. Engine / interface / card items

**HIGH — the FORCED SACRIFICE row states half the price when a LIFE-TO-DAMAGE CONVERTER of theirs is on
the battlefield.** `1789018600-ai_baka_deck162-0x55c73a2a97c0-vs-ai_baka_deck126.jsonl` **seq 10**, t11.
Rendered: `1. Shield Sphere #1 (0/6) [defender] [your battlefield] - "..." [you SACRIFICE this; they gain
6 life (its toughness)]`, and the header's tie-break `- where the rows differ - the one that pays the
least`. With Sanguine Bond in play the true price of that row is 6 life to them **and 6 off the seat's own
total** (the log confirms it: `Opponent gained 2 life (now 23)` / `You lost 2 life (now 16)` for the Fog
Bank actually taken). The converter paragraph is 30 lines away in a different part of the prompt and the
row itself never names it. Fix: while a converter of theirs is live, the sacrifice row's bracket prints
both halves (`they gain 6 life (its toughness), and Sanguine Bond takes 6 off YOUR total in the same
event`), and the header's tie-break line says the cost is doubled. Same pattern as the P5 loop-row fix.

**MED — the `[hold check: ...]` bracket still calls a row NEW when only its mana annotations moved (CI P1b
/ CM F2 residual).** `1789018584-ai_baka_deck162-0x559bba7fa730-vs-ai_baka_deck152.jsonl`, the Upkeep seam,
**seq 16 (t10) and seq 24 (t12)**, both printing `[hold check: 1 row above is new since the last window at
this seam - a row that changes only in its annotations (a price, a forecast, a clock, a count) is the SAME
row and does not re-open a hold ...]`. Diff of the two menus, seq16 → seq24, row for row:
row 1 `Cast Dictate of Kruphix {1}{u}{u}` is byte-identical except `{leaves 1 of your 4 untapped mana
sources untapped}` → `{leaves 2 of your 5 untapped mana sources untapped}` plus an added `{spends 3 of your
5 untapped mana sources this turn; ...}` group; rows 2 and 3 (`Hold priority`, `Cast nothing right now`)
are byte-identical. Both changes are prices by the bracket's own definition, so the claim is FALSE.
seq12 → seq16 is the same shape (an added `{reserve: ...}` group on row 1). Caveat: I inferred the pairing
(previous window at the same phase, same seat, cross-turn) — the engine seat can confirm against
`holdActionKeyRow`. The suspect is that `{leaves ...}` / `{spends ...}` / `{reserve: ...}` are still inside
the comparison key while `[cost:]` was the group that got fixed. No decision was lost to it here (max
declines 2), so MED, not HIGH.

**MED — `main_phase_windows_skipped` is still over-counting (P13 not closed).**
`1789018575-...-vs-ai_baka_deck125.jsonl` gameend reads `main_phase_windows_skipped: 14`. The seat's own
turns in that game are t2 through t22 = 11 turns = **22 own-turn main phases**, of which **12 distinct
(turn, phase) pairs printed a `Casting decision (Main phase N, YOUR turn)` header** (t4 MP1, t6 MP1, t6 MP2,
t8 MP1, t10 MP1, t12 MP1, t14 MP1, t16 MP1, t18 MP1, t18 MP2, t20 MP1, t22 MP1). At most 10 main phases can
therefore be genuinely skipped, so **at least 4 of the 14 counts are false or double-counted**. The other
five games are consistent (146 0, 152 0, 130 0, 126 0, 123 2 with 5 offered of 14).

**MED — a forced close on this seat produced no decision record.** `game-162v130-1789018595.stderr` carries
**four** `AIPlayerGPT: unclosed <think> (budget/truncation); forcing the answer` lines (lines 711, 859, 1025,
1058) for the two seats in that game; the deck162 gameend reads `phase2_answer_recovered: 1`,
`phase2_answer_missing: 0` and **`forced_close_unrecorded: 1`**. CJ predicted 0. The game was won and the
seat's 13 records are contiguous, so the lost window is a pass or a land drop rather than a play, but the
arm is still dropping records. `game-125v162-1789018573.stderr` has one more `unclosed <think>` (line 1029)
against `forced_close_unrecorded: 0` in that seat, so the two are not one-to-one; the reconciliation is the
engine seat's, with these two files as the repro pair.

**LOW — the guide points at a render label that does not exist.** The live guide's Sanguine Bond section
says the loop is scoped by *the "LOOP SCOPE:" clause on the DRAW PUNISHERS and DRAW FORECAST lines*. The
string `LOOP SCOPE` appears **0 times in all six seat files**, including the 126 game where both halves were
live and the render instead said `Both halves of a life LOOP are on THEIR battlefield (Sanguine Bond +
Exquisite Blood): ...`. Either the clause was renamed or it never shipped; the guide sentence is cut in the
revision either way, and any other deck guide naming it should be checked.

**Passes worth recording.** P19: 64 hold rows rendered, **492 B min / 596 B median / 596 B max** (ceiling
650), and `[HOW A HOLD ENDS:` appears exactly once in each of the 64 prompts that render one — 64/64. P6:
`CRACK-BACK NEXT TURN` printed in own-turn Upkeep windows (152 seq12 t8, seq24 t12) as well as main phases,
58 renders across the seat, no window with an own-turn board and no line. P22: `Parser returned NULL {t}:add{`
0 in all six stderr files. P5/P10 family: 0 `you would be at 0 - this row feeds their chain`, 0 `reaches 0 in
N more turns`, 0 `another copy of` — none of those clauses had an occasion here, so they are UNTESTED, not
passed. The 126 seq13 loop render is TRUE and is the best line in the corpus: `NO best-case life figure is
given: both halves of their life LOOP are in play ... Blocking to cut the damage does not bound it, so no
survival claim is made from these figures. Declining every block does not escape it either`. `async_drops`
2 (125, 130), both re-asked — `game-162v130` line 1051: `dropping stale async answer (land-drop arm; the
question and board moved) [outcome: re-asked]`. Card facts re-checked against the prompts: Fate Unraveler
3/4 `{3}{B}`, Master of the Feast 5/5 flier `{1}{B}{B}`, Ob Nixilis 5 loyalty with `-2` destroy-and-draw-two,
Shield Sphere 0/6 taking `+0/-1` per block (126 seq12 shows `(0/5) (printed 0/6) [counters: 1x +0/-1]`),
Peer into the Abyss's two modes — all matched Oracle and matched what resolved. **No card behaved contrary
to its rendered text in these six games.**

## 3. Guide verdict: **EDIT**

`wave75/deck162/strategy.txt` — **19,958 bytes, from 19,985 (net −27)**, under the 20,000 ceiling. Four
edits, each paid by a decision in this corpus; the room comes from five cuts named after the table. **No
edit grows the guide** — the net is negative, so no rule has to earn new bytes.

| # | before → after | paid for by |
|---|---|---|
| A | §BLOCKING: `Then the rungs, in order:` → `Then the rungs, in order. WHILE (a) IS LIVE - your ONE punisher is the blocker offered - NO RUNG BELOW reaches its "(both die)" or "your blocker dies, attacker lives" line: that blocker takes its "(you kill it, your blocker lives)" line or stays home, and the attacker it declines to eat goes to another blocker or to nobody, whatever the unblocked total comes to.` Rung 0 loses its concessive: `block the BIGGEST "deals N" you may block - even when that line kills your blocker and a free kill on a small attacker sits there.` → `block the BIGGEST "deals N" you may block, and take that line when it kills your blocker.` (`Bounded by (a) and (b).` stays). Rung 3 loses `over one that kills it, even when the dying line stops more`. | **152 seq15**, the loss. The model quoted rung 0's concessive six times as its authority for spending the deck's only punisher, and rungs 0 AND 3 both fire on that board (7 unblocked, ≤ 8), so bounding rung 0 alone would not have held. Amendment 334 names this exact failure: the concessive clause described the rule's hard case and licensed it. (a)'s precedence now sits above every rung instead of inside one, which is why the edit is shorter than what it replaces. |
| B | §THE BRAKE, K-of-0 bullet: `the ONE engine you cast here is your FIRST HOWLING MINE. ... A Dictate, Puzzle Box or Forced Fruition row at K of 0 is BRAKED whether or not a Mine is out ... A SECOND Mine's row carries "[second copy: ...]" and a first does not.` → `THE BRAKE HOLDS EXACTLY THREE ROWS HERE - DICTATE, PUZZLE BOX, FORCED FRUITION. Those three arm THEIR deck and not yours ... YOUR FIRST HOWLING MINE IS NOT ONE OF THEM: take its row at K of 0, at any life, as the one card on a dead menu that can still find you a punisher. A Mine row carrying "[second copy: ...]" is a second Mine and the brake does hold it.` | **152 seq26**. The model read the carve-out and the rule in the order they were written and let the rule swallow the carve-out — *"'A Dictate, Puzzle Box or Forced Fruition row at K of 0 is BRAKED.' Howling Mine is an engine."* The brake is now stated as a CLOSED LIST of three named rows before the Mine is mentioned, and `whether or not a Mine is out` — the phrase that generalised it — is gone. Six mana sat idle at 6 life for want of this. |
| C | §FORCED SACRIFICE: `THE LIFE THEY GAIN NEVER DECIDES THIS ROW, in either direction: ...` → `WITH NO LIFE-TO-DAMAGE CONVERTER OF THEIRS IN PLAY, THE LIFE THEY GAIN NEVER DECIDES THIS ROW, in either direction: ...` plus a new sentence `A CONVERTER OF THEIRS REORDERS THE WALLS. Then "[they gain N life]" is also N off YOUR total in the same event, so among the WALLS take the LOWEST N on the board - Fog Bank over a fresh Shield Sphere - and read the number as damage to you. A punisher still goes last at every N.` The trailing `Any row named on that prompt's "DRAW PUNISHERS ... yours" line goes last.` is cut as a restatement of ORDER. | **126 seq10**. The rule as written is false on a converter board and the model spent ~1,900 reasoning characters overriding it, resolving on the prompt's header rather than the guide. Amendment 333: the rule now declares its own scope in its first clause instead of stating a universal it does not have. The reorder is bounded to WALLS, so the punisher-goes-last ordering that edit C of wave 74 bought is untouched. |
| D | §MULLIGAN tail: `BOTTOMING sends your most EXPENSIVE spell first ... CLEANUP DISCARD is that order from the end, answered "PUT: n, m" ... never Shield Sphere or your only punisher until nothing else is left.` → `BOTTOMING AND CLEANUP DISCARD BOTH SEND THE MOST EXPENSIVE SPELL IN HAND FIRST - Forced Fruition, Peer into the Abyss, Puzzle Box, a second copy of an engine - and a CREATURE goes only once every costlier noncreature has gone. KEEP in this order so any mana count is buildable: Shield Sphere (zero mana), your only punisher, lands up to four, then spells by cost; bottom a land only if the keep still holds FOUR OR MORE. Cleanup is answered "PUT: n, m" - the numbers ONLY, nothing in brackets.` | **126 seq12** — a cleanup discard of Master of the Feast {1}{b}{b} with two Teferi's Puzzle Box {4} in the same hand. The old text reached cleanup only through `that order from the end`, a cross-reference to a reversed list; the discard rule is now stated in its own right and names the creature case that came up. The "never Shield Sphere or your only punisher" clause is carried by the KEEP order it duplicated. |

**Bytes returned** (each unexercised across this wave and the last, or now duplicated):
(i) the §MULLIGAN `(keeping 6)` / `(keeping 5)` branches, collapsed to one line — **0 mulligans in twelve
games over two waves**, so no decision has ever entered them; (ii) the RELEASE (i) COLOUR per-card black
requirements (`Underworld Dreams needs {B} 3, Ob Nixilis {B} 2 and 5 total, Fate Unraveler {B} 1 and 4
total`) — the rule keeps its test against the printed `sources that can make each` line, and no decision in
two waves turned on the numbers; (iii) the `"LOOP SCOPE:"` sentence — the label does not exist in any render
(LOW item above); (iv) §BLOCKING rung 5's `Fate Unraveler and Master of the Feast block well ...` tail, now
covered by (a) hoisted above the rungs; (v) §FORCED SACRIFICE's `Any row named on that prompt's "DRAW
PUNISHERS ... yours" line goes last.`, a restatement of ORDER.

**Not changed, deliberately.** §CAST ORDER rung 4b and the wave-74 Master-of-the-Feast unbraking (**a success
verdict**: fired in three of six games, 130 seq9, 125 seq8, 146 seq9, cost nothing anywhere, and the seat's
only creature-light matchup still won). Wave-74 edit A's STRANDS re-siting (**a success verdict**: 2 offered,
0 taken, 0 `plan_names_stranded_card`, against 3 of 19 taken last wave) — nothing of it was cut. §ATTACKING
(three attack windows, all on `[no creature they control can block this attacker]` tags, all correct).
§LAND DROP (every land-drop ask answered with a land where one was playable). §CARD FACTS Ob Nixilis and
Peer (both decided the 123 game as written). The K-of-1-or-more branch. Rung 0's arithmetic.

## 4. Not checked / limits

- Only this seat's six translogs, the six opponent translogs for outcomes, and the six stderr files. No
  lane adjudication, no corpus counters beyond my six gameends, no other deck's seats, no engine source.
- **P7 (crack-back cover counting a legend-rule duplicate) is UNTESTED here**, though the wave-74 report
  traced it to 130v162: the deck130 seat renders **no** `{crack-back cover:` row at all in this wave's
  130v162, and the one cover clause in my six games (152 seq14, Fate Unraveler) has no legend-rule bracket
  to disagree with. The `[legendary: you already control ...]` rows that did appear (125 seq33, 123 seq17)
  are Ob Nixilis planeswalker rows with no cover clause.
- The `[hold check:]` MED rests on an inferred pairing of "the last window at this seam" across turns; I did
  not read the render code and cannot see the engine's own comparison window.
- The `main_phase_windows_skipped` MED is an upper-bound argument from distinct own-turn casting headers; if
  the counter counts windows rather than phases the arithmetic does not apply and the item is void.
- I did not trace the two `async_drops` beyond the stderr lines, did not audit latency, `deadline_pct` or
  `transport`, and did not open the `askreplay/` sidecar.
- **UNTESTED, not passed, in this seat**: P5 loop-fed `{right now:}` rows, P10 own-clock gate, P11
  `another copy of`, P15 X ladders, P16 short names, P18 MDFC land menus (0 `OTHER FACE of`, but also no MDFC
  in any hand), P9 inline `CHOICE:` on the PLAN line (0), P21 `reply_truncated` (0), the `{X}`-announcement
  path (0 `ANNOUNCE_X`), the Ob Nixilis legend-rule exception (both `[legendary:]` rows appeared at loyalty 5,
  so the guide's loyalty-2-or-less branch never opened), Teferi's Puzzle Box on the battlefield (never cast).
- The 152 counterfactual — that blocking A2 at seq15 wins or extends the game — rests on the engine's own
  `A2 (you kill it, your blocker lives)` tag and on the seat having produced zero damage after the punisher
  died. It is not a replayed game.
- The 146 and 126 losses are called variance, not guide: 146's hand held no creature on the turn rung 0
  wanted one, and 126 never made a third land.
