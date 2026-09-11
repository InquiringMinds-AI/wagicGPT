# Wave 77 — deck 125 (Revelation Fracture, UW draw-go control)

Seat files: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-010157-final/*ai_baka_deck125-*`
(all six are RERUN games; the one first-run game of the corpus is 126v162, not mine). SINGLE binary.

**3 W / 3 L** (wave 76: 2/4). 757 seat decisions (ask 541, priority 176, discard 40) + 5 `recovery`
rows, reasoning on 757/757, reasoning median 6,357 chars / **max 35,886 = the corpus maximum**;
latency p50 70.8 s / max 397.5 s, `deadline_pct` max 43.8. `off_protocol_bytes>0` 21 (2.8%),
`plan_line_missing` 16 (2.1%), `async_drops` 17, `hold_released_turn` 54, `hold_windows_skipped` 945,
`crossphase_identical_reputs` 36, `crossphase_board_unchanged` 2, `forced_close_unrecorded` 3,
`phase2_answer_recovered` 4 / missing 0, `own_loop_windows_asked` 0, `main_phase_windows_skipped` 0,
`plan_names_uncastable_zone_card` 1, `plan_names_stranded_card` 0, `repeat_annotated_takes` 0.
**All 6 corpus-wide heuristic fallbacks are on this seat** (5 `unparsed_reply` + 1
`noop_row_zero_reask`), and so are all 5 `recovery` rows and both `crossphase_board_unchanged` renders.
No hang, crash or timeout.

## 1. Game-by-game

| opp | result | turns | end life | the decision that decided it |
|---|---|---|---|---|
| 123 | **WIN** | 55 | 42–0 | Grind win, no misplay decided it. 8 counters all on-list, 16/16 pings taken and all 16 at the face, 12 cleanups with no Staff sent. One pre-main Revelation at `seq 247/248` (t43 upkeep, X=7) in a game already won. §A-1 |
| 126 | **WIN** | 41 | 48–0 | Textbook: 21 of 23 ping rows taken, 20 at the face, 0 self-pings under two Exquisite Bloods. The two misses are `seq 107` (t27, `CHOICE: 0 (pass)`) and **`seq 253`** (t39 upkeep, own-turn **hold** on a priority menu — the ping row was on the list and the hold gave up that turn's ping). §B-3 |
| 130 | **WIN** | 72 | 43–6 | Won but glacial: 12 pings over 72 turns against a Hammer-of-Bogardan burn deck the counterspell table has no row for (7 counters spent on Hammer). `seq 116` is the `noop_row_zero_reask` — Supreme Verdict at K=0 "to reduce hand to 7" with **no DISCARD PUNISHERS paragraph on the screen**; the engine refused it and the re-ask corrected. §A-2 |
| 162 | LOSS | 35 | 0–10 | **`seq 180/181`, t29 Main 1, 21 life, opponent 11, two Staffs out: Sphinx's Revelation announced X=9, spending all 12 sources with Essence Scatter in hand.** The next turn the opponent resolved Ob Nixilis + Dictate of Kruphix + Howling Mine + Forced Fruition against a tapped-out seat and **no window was put to this seat on turn 30 at all**. Tap-out-gate violation (the gate's price was 2; the row left 0). Also three pre-main Revelations here (`seq 101/102` t23 upkeep X=6 the clearest). §A-1, §A-3 |
| 146 | LOSS | 35 | −2–14 | **`seq 125`, t31, 8 life, 3 sources: a second Essence Scatter cast at Silverquill Silencer while the seat's own first Essence Scatter was already on the stack aimed at it.** Two mana and a card burned on an already-countered spell; the seat took 7 from Hive of the Eye Tyrant that combat (8 → 1) and lost at −2. PERCEPTION — the row printed the target as live. §B-1. Also `seq 99/100`, t24 **upkeep**, Revelation X=5. §A-1 |
| 152 | LOSS | 13 | −6–26 | Variance: 5 lands at t10 against a t9 Sigarda (5/5 flying trample) + Luminarch Aspirant. Supreme Verdict at `seq 25` (t8) was correct on the header; it tapped the seat out, and it had no counter for either follow-up (hand was Cancel/Dream Fracture/Gavel, none castable at 1 open source). No guide item is bought here |

## 2. Engine / interface / card items

**HIGH — B-1. A counterspell row offers a spell that the seat's OWN counterspell is already on the
stack to counter, and says nothing about it.**
Repro `1789114854-ai_baka_deck125-…-vs-ai_baka_deck146.jsonl` **seq 125** (t31, 8 life, 3 sources).
The stack block reads:
```
  1 (top): ability: Lolth, Spider Queen's -3: create spiders [from their Lolth, Spider Queen] [triggered/activated ability]
  2: your Essence Scatter {1}{u} (instant) [spell] targeting Silverquill Silencer
  3: opponent's Silverquill Silencer {b}{w} (creature 3/2) [spell]
```
and both cast rows read `- can target on the stack: Silverquill Silencer {b}{w} (creature 3/2)` with no
note that item 2 already answers item 3. The seat cast its second Essence Scatter into it. The engine
already owns the "this cannot reach it" vocabulary — the same menu's Path row carries
`[this cannot target the spell on the stack - battlefield permanents only]`. Fix: on a stack target
that a spell already on the stack counters, tag the row (e.g. `{already countered by your Essence
Scatter above it on the stack}`). Cost here was the game: 8 → 1 that combat, final −2.

**HIGH — B-2. All 5 `unparsed_reply` fallbacks are the same reply shape — a PLAN line plus a bare
`<n> (<short name>)` answer line with the four characters `CHOICE: ` absent — and the parser counts
that line's bytes as `off_protocol_bytes` (17, 17, 26, 26, 26 = the exact length of the answer line)
before refusing it.** Every one verbatim, with the recovery that followed:

| file | seq | reply (verbatim) | heuristic executed |
|---|---|---|---|
| …vs-ai_baka_deck123 | 315 | `\n\nPLAN: Opponent is at 5 life, my two Staffs will deal 2 damage per turn, securing the win in 3 turns. Stop 3 prohibits Sphinx's Revelation, and there are no threats to answer. I will hold priority to close the turn.\n2 (Hold priority)` | `cast nothing` (seq 316) |
| …vs-ai_baka_deck126 | 140 | `\n\nPLAN: Let Staff pings resolve and hold priority to stop the menu loop.\n2 (Hold priority)` | **`cast Path to Exile`** (seq 141) |
| …vs-ai_baka_deck126 | 201 | `\n\nPLAN: Wait for Staff of Nin to ping the opponent down to 0.\n3 (Cast nothing right now)` | **`cast Path to Exile`** (seq 202) |
| …vs-ai_baka_deck126 | 219 | `\n\nPLAN: Exquisite Blood converts my damage to their life gain, so I will avoid dealing damage and hold priority to survive.\n5 (Cast nothing right now)` | `cast nothing` (seq 220) |
| …vs-ai_baka_deck162 | 49 | `\n\nPLAN: Cast nothing right now.\n4 (Cast nothing right now)` | `cast nothing` (seq 54) |

Two things, both engine-side. (a) The answer line names a legal row number AND its short name in
parentheses — the format the protocol asks for, minus the literal prefix — on its own physical line
directly after the PLAN line. A parser that reads it costs nothing and the model's intent is
unambiguous in all five. (b) Worse than the refusal: **at seq 140 and seq 201 the heuristic did the
opposite of a legible decline** — the model wrote `2 (Hold priority)` / `3 (Cast nothing right now)`
and the heuristic cast Path to Exile, at seq 201 at a lone `Pride Guardian #1 {w} (creature 0/3)
[defender]`, which is the row this deck's guide (§PATH step 4) answers "Cast nothing right now". A
fallback that takes an action after a reply that declined is not a neutral fallback. Both were in a
game won 48–0, so no result changed, but the mechanism is live.

**MED — B-3. The own-turn hold rule is unanswerable on a priority menu, and it cost a Staff ping.**
645 own-turn windows, 95 holds, 74 of them carried by the declined-count override. Of the 21
uncovered, **19 are `priority`-kind menus where no `Cast nothing right now` row exists** — the guide
names a row the menu does not print, so its only reachable answers are `0 (Pass priority)` and the hold
row, and the model took the hold. Repro `…vs-ai_baka_deck126.jsonl` **seq 253** (t39 Upkeep, own turn):
a `Deal 1 damage with Staff of Nin` row was on the menu and the reply was `CHOICE: 7 (Hold priority)`;
that turn's ping was skipped. Only 2 of the 21 are real ask-seam violations (`123` seq 231 at 35 life,
`126` seq 169 at 48 life), both in won games. Closed guide-side by edit 3 below; the engine note is
that a priority menu's row 0 is the only decline it offers and the hold row sits above it.

**MED — B-4. `[X pricing]`'s "the largest X that still leaves it payable this turn" names the cheapest
CARD in hand, never the cheapest counterspell.** `…vs-ai_baka_deck162.jsonl` **seq 180** (t29):
`Path to Exile {w} in your hand needs 1: the largest X that still leaves it payable this turn is X=8`
— the hand's only counterspell was `Essence Scatter {1}{u}`, and X=9 was announced. Every ordinary cast
row on this seat prints `{spends K of your M untapped mana sources this turn; <named card> in your hand
needs N}` and picks the same way. For a draw-go deck the reserve that matters is the counterspell one;
the guide's tap-out gate is keyed on exactly that number and the row never states it. A second named
figure (`the largest X that still leaves your cheapest counterspell payable`) would put the gate's
input on the row.

**MED — B-5. Prompt bytes grew 19% at the top end.** 757 prompts, p50 18,092 B, **max 36,211 B**
(`…vs-ai_baka_deck162.jsonl` seq 215; wave-76 seat max 30,375, CT's bound 38,657), and **316 of 757
(42%) over 20 KB** against 90 of 438 (21%) last wave. The largest are late 162 windows where a 27-card
hand line, a 29-permanent opponent battlefield with `{effect:}` glosses on every creature, and per-row
`[DRAW PRICE: …]` tails all render at once. No render is wrong; the growth is additive.

**LOW — B-6. `hold_check_ref_seq` is not resolvable against the translog.** 706 records carry it; 365
name a seq that is not a record in the same seat file (e.g. `…vs-ai_baka_deck123` seq 25 → ref 17, the
nearest record below being 15; `-1` for "no predecessor" is 18 of them and is fine). The counter is
evidently in the engine's window-sequence space, which includes skipped windows, so the brief's
"check the claim against the previous RECORD" cannot be run from the logs alone. Engine seat's call.

**LOW — B-7. A comma-list `CHOICE:` is silently resolved to the LAST number.**
`…vs-ai_baka_deck146.jsonl` **seq 118**: `CHOICE: 2, 3 (Deal 1 damage with Staff of Nin targeting
Spider #1, Spider #2)` — `choice` 3, `coded_answers` 1, `answer_replaced` False, no fallback. The seat
has one Staff and one ping, so the reply is a genuine non-answer and the engine picked a half of it
without a re-ask (wave 75's B-7 reported first-wins; this is last-wins).

**Verified TRUE / FIXED on this seat, so the engine seat does not read silence as absence:**
**R5 PASS, on the exact wave-76 repro card.** 492 creature battlefield entries, **347 glossed** (wave 76:
0 of 3,892). `…vs-ai_baka_deck130.jsonl` **seq 351**: `Dwarven Blastminer {1}{r} (1/1) {effect (THEIR
card, written from THEIR seat: …): "{2}{R}, {T}: Destroy target nonbasic land. -- Morph {R} …"}`. The
ungossed 145 are tokens (Angel, Skeleton, Goblin) and `#2` copies whose `(each copy of this card does
this)` gloss rides the first copy; I found no gloss cut mid-clause into a false statement. **The seat
read it**: at `…vs-ai_baka_deck130.jsonl` seq 32 it Essence-Scattered Dwarven Blastminer on the stack
at printed power 1 — the COUNTER ON SIGHT `"{T}:"` carve-out, which is the rule wave 76 said had no
channel, and the card that ground this deck to 0 permanents last wave never resolved.
**R8 / Q13 FIXED.** 35 `[this exact list was put to you …]` renders on this seat, 33 "HAS moved" and
**2 carrying the positive clause — both of the corpus's `crossphase_board_unchanged` pair**:
`…vs-ai_baka_deck126.jsonl` seq 199 `[this exact list was put to you 1 window ago at Combat begins and
you declined; nothing on the board has changed]` and seq 269 `[… 1 window ago at Main phase 2 and you
declined; nothing on the board has changed]`. Wave-76 B-3 (8 of 8 unreachable) is closed.
**Wave-76 B-4 FIXED**: 88 land-drop windows, **88 lands played, 0 declines**.
**Wave-76 edit 2 (SIZE FLOOR as printed stop (4)) LARGELY WORKED**: 13 Revelation casts, **1 stop-(4)
violation** (`130` seq 280, t57, 22 life, row `max affordable X=2`) against 2 of 10 last wave, and the
reasoning at `162` seq 101 and `146` seq 99 walks stops (1)(2)(3)(4) **by number and verbatim**. That
the numbered list is read at all is the evidence edit 1 below rests on.
**EMRAKUL PASS**: 4 cast rows offered, 4 declined — two with a Staff on the battlefield line
(`126` seq 181/186) and two under a row-level `[DRAW PRICE: … this KILLS you]` (`162` seq 222/227).
**Pings**: 84 ping rows offered, 77 taken, **0 self-pings** under two Exquisite Bloods and an
Underworld Dreams. 10 of `162`'s 21 pings went at Ob Nixilis, the Hate-Twisted while it was named in
the printed DRAW PUNISHERS paragraph, which is the guide's printed exception.
**Counterspells**: 37 cast; every one I traced is answerable from the table or from COUNTER ON SIGHT,
including the two hardest (`162` seq 204 Essence Scatter at Fate Unraveler off a `[DRAW PRICE: … you
would be at 22]` row, and seq 209 Dream Fracture at Forced Fruition at K=14 under the "draw text of its
own" clause). No off-list counter found.
**Q10 / ghostform UNTESTED here** — no `{removes:}` row on a ghostform body occurred.

## 3. Guide verdict: **EDIT** (`wave77/deck125/strategy.txt`, **19,999 bytes** against the live guide's
19,993 — it grows by 6 bytes. The three rules are paid out of seven prose trims listed as edit 4, none
of which removes a decision rule.)

| # | before → after | paid by |
|---|---|---|
| 1 | **The "not at an upkeep or draw window" test becomes numbered stop (5) of §SPHINX'S REVELATION.** `FOUR PRINTED STOPS` → `FIVE`; the list gains `(5) the "Phase:" on the CURRENT SITUATION line reads Upkeep or Draw and the same line says it is YOUR turn - the row is offered at those two phases and is never taken there, at any X, life or mana.`; `THESE FOUR ARE NOT HEURISTICS` → `THESE FIVE`; `LIFTS STOPS (2), (3) AND (4), never (1)` → `never (1) and never (5)`; the floor's trailing `IN A MAIN PHASE OF YOUR OWN TURN, NEVER AN UPKEEP OR DRAW STEP,` is deleted as now duplicated; ORDER OF OPERATIONS step 3's `A SPHINX'S REVELATION INCLUDED, at any X, any life and any mana` → `A SPHINX'S REVELATION INCLUDED, which is stop (5) of its own section` | **The wave-76 wording got WORSE, not better: 6 of this wave's 13 Revelation casts were at an upkeep** (`123` seq 247/248 t43, `130` seq 293/294 t59, seq 304/305 t63, seq 344/345 t67, `162` seq 101/102 t23, `146` seq 99/100 t24) — 46% against 30% last wave. The reasoning says why, twice, independently: at **`162 seq 101`** (t23 Upkeep) the model writes out stops (3), (4), (1), (2), the library ceiling, the low-life floor and the tap-out gate **by number**, concludes *"I will choose 1. Cast Sphinx's Revelation"*, and never reaches ORDER OF OPERATIONS at all; at **`146 seq 99`** (t24 Upkeep) it quotes stop (3) and the low-life floor verbatim and writes *"So casting is legal and good."* The numbered stop list is demonstrably the surface this model reads at a Revelation row; a rule about Revelations that lives three sections away is not. This is the same placement fix that made wave-76 edit 2 work — stop (4) has 0 violations this wave |
| 2 | **Step 4's DISCARD PUNISHERS guard moves into its own operative sentence, with the no-punisher case stated first.** `HAND OVER SEVEN ON YOUR OWN TURN WITH A "DISCARD PUNISHERS …" paragraph printed: cleanup bills you 2 life per card above seven …` → `READ THE SCREEN FOR A "DISCARD PUNISHERS on the battlefield: theirs" PARAGRAPH BEFORE THIS STEP IS A STEP. WITH NO SUCH PARAGRAPH PRINTED A HAND OVER SEVEN COSTS YOU NOTHING AT ALL, buys no cast, and this step is over - a hand count is never itself a reason to take a row. While that paragraph IS printed, …`; the dead-row licence ends `ONLY THERE does any rule reach a row this guide calls dead`; §SWEEPERS' `ONE RULE REACHES K OF 0 - the hand-over-seven cleanup bill, step 4` → `- step 4, and only under a printed DISCARD PUNISHERS paragraph`; §LIGHTMINE's `step 4 is the one rule that lifts it` → `only step 4 under a printed DISCARD PUNISHERS paragraph lifts it` | **Wave-76 edit 3 backfired: the model now fires step 4 on the hand count alone.** The cleanup-bill reasoning appears in 40 windows on this seat and **not one of them had a DISCARD PUNISHERS paragraph printed**. Two converted into dead casts. At **`162 seq 48`** (t15, 20 life, hand 8) the reasoning reads *"Lightmine Field at N of 0: 'Cast nothing right now, a second copy included; step 4 is the one rule that lifts it.' (Step 4 is cleanup discard, not relevant now)"* — and then, nine lines later, *"But the cleanup/main phase hand size rule explicitly overrides dead rows … So casting LM is correct to reduce hand size"*, spending 4 mana and a card on a row printing `they control 0 creatures able to attack - deals 0`. At **`130 seq 116`** (t31, 18 life, hand 8) it cast Supreme Verdict at K=0 on the same licence (*"cast one castable card per card you are over"*, *"Cheapest: SV"*) and only the engine's `noop_row_zero_reask` stopped it; the re-ask at seq 117 answered `Cast nothing right now`. The old sentence also stated a false fact unconditionally — with no punisher on the board a cleanup discard costs no life at all — and that false fact is what the model acted on |
| 3 | **The own-turn decline names the row a priority menu actually prints.** `ON YOUR OWN TURN THE HOLD ROW IS NOT YOUR ANSWER: take "Cast nothing right now", every time, whatever the menu and however dead the rows look.` → `… take "Cast nothing right now", and on a menu that prints no such row take "0 (Pass priority)" - every time, however dead the rows look.` | **19 of the 21 uncovered own-turn holds are priority menus with no `Cast nothing right now` row** (B-3). `126 seq 253` is the one with a price: a Staff ping row on the menu, `CHOICE: 7 (Hold priority)`, ping skipped. `130` owns 8 of the 19 (seq 186/193/219/228/233/245/262/267), all upkeep or main-phase priority menus in a game that took 72 turns to close a 14-point clock |
| 4 | Byte payers, no decision rule lost: the tap-out gate's three quoted row endings collapse to `Every cast row prints what it leaves untapped`; the Staff section's `[second copy: … no legend rule]` tag gloss (the SECOND STAFF sentence immediately after it carries the rule); Emrakul's `Until its own cast row is numbered it is blank`; §PATH's restatement of `{removes: A, B, C}`; §CLEANUP's restatement of the numbered-hand ask; §MULLIGAN's `Read the engine's counts and its`; §REVELATION's enumerated dead-row examples inside the EMPTY-menu sentence; `TWO ROWS DECLINE AND THEY ARE NOT THE SAME.` → `TWO ROWS DECLINE:` | bytes |

**KEEP, explicitly, with the evidence.** The ping rules — 77 of 84 offered rows taken, **0 self-pings**
across two Exquisite Bloods, an Underworld Dreams and a Fate Unraveler, and the 10 Ob Nixilis pings all
under the printed DRAW PUNISHERS exception. The counterspell table — **37 counters, none off-list**,
including the printed-power-1 Dwarven Blastminer countered on its `{T}:` text (`130` seq 32) and the
two Forced-Fruition-priced counters at `162` seq 204/209. The land-drop rule — **88 of 88**. The
never-discard-a-Staff step — 40 cleanups, no Staff sent. The EMRAKUL rule — 4 of 4 declines. Stop (4),
the SIZE FLOOR — 1 violation in 13 casts, down from 2 in 10, and the mechanism behind that fall is
edit 1's whole case.

**Disobeyed, not misread, so NOT re-capitalised** (the wave-74 ruling): `162 seq 180/181` is a plain
tap-out-gate violation — the gate is stated, the reasoning at that window does not reach it, and the
gate's own price (2, Essence Scatter) is not the number the row prints (B-4). It is not re-written
because the counter it stranded (Essence Scatter, creature spells only) could not have answered any of
the four permanents that resolved on turn 30, so nothing is bought; the interface item is B-4.

**Wave-76 items that recur:** §A-1 (the pre-main Revelation) recurs 6 times and is edit 1 — WORSE than
wave 76, 50% of casts against 30%. §A-2 (the cleanup bill) recurs as a NEW failure — the rule fires
with its condition false — and is edit 2. §A-3 (tap-out gate) is new this wave. Wave-76 B-1 (the
stripped creature battlefield line) is **FIXED** and paid off in play (`130` seq 32). Wave-76 B-3
(Q13's unreachable no-change clause) is **FIXED**, 2 of 35. Wave-76 B-4 (declined land drops) is
**FIXED**, 0 of 88. Wave-76 B-5 (a Revelation declined at the X menu) **recurs once**, and in the same
pair as the wave's one stop-(4) violation: `130` seq 280 casts at `max affordable X=2` and 22 life,
seq 281 answers `CHOICE: 4 (Decline)`, stranding the 5 mana the row had already paid off tapped
sources. The guide already answers both halves ("A DECLINE HERE IS NOT FREE … answer the stops at the
CAST row"); it was not consulted, and nothing new is bought by restating it a third time. Wave-76 B-2 (reasoning spent re-auditing the protocol tail) is R12 and
report-only: the tail is still there in bulk — `123 seq 315`'s 9,100-char reasoning and `130 seq 116`'s
4,836 both end in a dozen repetitions of the format check — and the seat now owns the corpus's
35,886-char maximum.

## 4. Not checked
No `blockers`, `attackers`, `reveal` or `bottom` seam — this deck fielded no creature in any of the six
games and took none of those asks; every mulligan was a `Keep this hand` (6 of 6), so no ship or bottom
branch of §MULLIGAN was exercised. No `LOOP`, `LIFE-TO-DAMAGE CONVERTER`, `DISCARD PUNISHERS` or
`{... CLEANUP PRICE at X=N ...}` string appeared in any of the 757 prompts, so §THEIR CONVERTER AND
THEIR LOOP, Revelation stop (2) and the punisher half of step 4 are UNTESTED this wave — I trimmed
prose there but removed no rule. I opened opponent seat logs only for the results row and reconstruct
no opponent's line of play. I verified no card against Scryfall or `primitives/*.txt`: Dwarven
Blastminer, Exquisite Blood, Forced Fruition and Ob Nixilis are quoted from the engine's own `{effect:}`
/ `{target text:}` renders, and I did not check any script against Oracle. I did not trace the 17
`async_drops`, did not open `askreplay/`, did not reconcile `ask_replays_reserved` / `identical_ask_
answers_reserved` against the code, did not reconcile this seat's 3 `forced_close_unrecorded` against
stderr, did not measure the CU F1 balanced-strip re-ask shape, and adjudicated no lane-{CR,CS,CT,CU}
prediction beyond the ones my own windows touch (R5, R8/Q13, R9, R12 above) — the rest is the engine
seat's job. One note I could not resolve: at `126 seq 219` the model's PLAN reads *"Exquisite Blood
converts my damage to their life gain"*, which inverts the gloss printed one screen up
(`"Whenever an opponent loses life, you gain that much life"`, with the THEIR-seat translation note);
the record is an `unparsed_reply` whose heuristic cast nothing, the game was won 48–0, and I bought no
edit with it. No general-guide or skill proposals: all three edits are placement fixes inside this
deck's own guide, and edit 1 is the wave-76 edit-2 mechanism applied to a second rule.
