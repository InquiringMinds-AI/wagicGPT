# Wave 74 — deck 162 ("Draw and Die!", UB forced-draw punish) per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260909-104713-final` — **MIXED BINARY**; all six of this
seat's games are on the **CG binary** (`wagic-60e9fe2ad-w74step1b`, source `1d1d3744d`), none on the CH
rerun binary, so every hold observation below is a **pre-CH observation** and says nothing about the CH
action-identity latch. Seat files `*-ai_baka_deck162-0x*-vs-*.jsonl` (6), `--thinking on`.

**203 model decisions, 0 fallbacks, 0 `reply_truncated`, 0 `action_before_plan`, native reasoning on
203/203** (`reasoning_chars` 2,139 / 6,462 / 20,923 min-med-max). **16 `plan_line_missing` (7.9%; wave 73
was 6 of 115 = 5.2%)**. Record **4-2**, unchanged from wave 73 but with a different pair of losses: beat
123 (18–0 t12), 126 (18–0 t19), 130 (2–−3 t15), 125 (18–0 t27); lost to 152 (−1–21 t9) and 146 (0–14 t19).
Wave 73 beat 126/125/130/146 and lost to 152/123.

**All three of the corpus's `plan_names_stranded_card` are this seat's** (123 seq10, 126 seq20, 130 seq27)
and **the corpus's single `identical_ask_answers_reserved` is this seat's** (125 seq99) — both quoted below.

## 1. Game by game

| Opp | Result | Deciding decisions |
|---|---|---|
| 152 | **LOSS** t9, −1–21 | **seq13 (t8 Main phase 1) is the loss.** Life 10, board 0 creatures, `CRACK-BACK NEXT TURN: 3 of their creatures ... for up to 10 from combat ... you would be at 0 or lower; that would KILL you`. Row 2 was `Cast Master of the Feast {1}{b}{b} (5/5)` with a `{crack-back cover: ... you cover 4 of 10, leaving 6 -> you would be at 4}` clause. CAST ORDER rung 0 (BODY TURN) fires on exactly this board — M=3 > N=0 and the KILL wording is printed — and names Master of the Feast in its creature list. The seat took row 1 (Underworld Dreams) and died on t9. Reasoning verbatim: *"Crack-back next turn kills me. I need to deal damage or block. I have no creatures."* — rung 0 is never mentioned; the model entered the list at rung 6 and quoted it: *"HOWLING MINE, DICTATE, PUZZLE BOX, FORCED FRUITION, MASTER OF THE FEAST - only when the row's 'draw converters ... on your battlefield:' number is 1 or more ... So brake applies."* **Guide item, paid edit B.** |
| 146 | **LOSS** t19, 0–14 | Removal war: Soul Shatter took Fate Unraveler (t9), Vanishing Verse exiled Ob Nixilis (t11) and a Master (t17), Kaya −3 exiled the other Master (t15) — five one-for-ones, mostly not guide-addressable. The one guide decision is **seq14 (t12 Main phase 1)**: hand held **two** Master of the Feast, board had 0 creatures, their line read `2 are creatures, 2 of them without a restriction against attacking`, 6 sources up, land drop spent. The seat passed. Reasoning verbatim: *"Wait, should I consider casting Master of the Feast anyway? The guide says K=0 brakes it. I have 0 converters. So it's braked. I must not cast it."* and *"Is there any chance I should cast Master of the Feast to get a blocker? ... The brake on Master at K=0 is strict. I will follow it."* The guide's own Master exception (their line ≥ 1 creature) is 15 lines below the braked list and never fired. Two more turns of a 5/5 flying blocker is not a proof of a win, but the seat died at 0 with two of them still in hand at t14. **Guide item, paid edit B.** |
| 123 | **WIN** t12 | Mine t3, Underworld Dreams t5 and t7, Dictate t9 (a STRANDS take, below), Master t11; opponent dead on their t12 draw step to 2 punishers. seq14/15 correctly counted the kill and held. |
| 126 | **WIN** t19 | Shield Sphere t2, Caress t6, Fate Unraveler t8, Ob Nixilis t12 (+ two −2 activations), Dictate t14, Master t14, Underworld Dreams + Puzzle Box t16, second Fate Unraveler + Fog Bank t18. **seq11 (t9) is a §FORCED SACRIFICE violation** — see edit C. seq30's second Tribute to Hunger took Fog Bank over Fate Unraveler, in guide order. `LOOP HALF PENDING` (Sanguine Bond out, Exquisite Blood in hand) printed at seq27–29 and cost nothing; the both-halves branch is still **UNTESTED**. |
| 130 | **WIN** t15, at 2 life | Shield Sphere t1 and t7, Mine t3, Master t7, Underworld Dreams t13, Dictate t15 (a STRANDS take), Mine #2 t15, lethal attack t15. **seq25 (t14 blockers) is the game**: `INCOMING THIS COMBAT: 5 attackers, 11 unblocked damage - you would be at -1 ... you SURVIVE at 2 if you block`; the seat answered `BLOCKS: B1:A1, B2:A2`, the engine's own best assignment, both Shield Spheres surviving. §BLOCKING rung 0 applied correctly. |
| 125 | **WIN** t27 | The longest game of the seat. **The K-of-0 brake held a full hand (Dictate, Forced Fruition, Puzzle Box, Master) from t12 to t22** — the opponent, a creature-light control deck, gave rung 4b/§Master no trigger, so the brake was correct and the seat won; but it cost ~50 pass windows (see HIGH item 1). Fog Bank t14, Shield Sphere t20, first Mine t22, Dictate t22, Ob Nixilis + Puzzle Box t24, Underworld Dreams ×2 t26 (one Dream Fractured, a Fate Unraveler Essence Scattered). Their t27 draw step drew 16 and took them from 29 to 2; the seat's Path-to-Exile put-onto-battlefield ask ended it. |

### The Ob Nixilis + Underworld Dreams chain vs deck125 (asked for explicitly)

It cost **six windows, one of which never reached the model**. Four at **t26 Main phase 1** (seq 91, 92, 95,
96 — two of them opponent counterspells on the stack, two punisher/ability windows) and two at **t27 Draw**
(seq 98, and seq 99 which the replay path served from seq 98's answer: `why: repeat_ask_reserved`,
`replayed_from: 98` — the corpus's only `identical_ask_answers_reserved`, and it is **genuine**, two Path to
Exile triggers offering the identical 7-row library list). The whole 39-life turn-27 chain cost **one** model
call. Max asks in any one (turn, phase) for this seat is **8** (125 t26 MP1) — the > 10 tripwire is not
approached anywhere in six games. **The hold did NOT hold across the chain: no hold was ever taken in it.**
A hold row WAS offered at seq 92 (row 3) and the seat answered `CHOICE: 0 (pass)`; that was correct — the
only acting rows were `-2: destroy target creature ... targeting Fog Bank #2 [your battlefield] {this hits
YOUR permanent}`, i.e. the window was `chain_windows_only_selfharm` (6 for the game, `chain_selfharm_rows`
12 of `chain_acting_rows` 12 — **100% of this seat's chain acting rows were self-harm**, against CH's
hand-measured corpus 17%). The `declined this exact list N times` clause reached **4** inside the chain.

## 2. Engine / interface / card items

**HIGH — the hold latch re-opens on its own declined-count annotation: the same 3-row list is put to the
seat 11 times in one turn, and 8 hold takes are all discarded.** `1788968869-ai_baka_deck162-0x55a382732ac0-vs-ai_baka_deck125.jsonl`
**seq 66→76, all turn 21**, seams Upkeep, Draw, Main phase 1 ×3, Combat begins, Attackers, Combat ends, Main
phase 2, End, Cleanup. The rows are byte-identical apart from one substring. seq 67 row 3:
`3. Cast nothing right now {this same question will be asked again this turn: taking this row closes this
window only, and you have already declined this exact list 1 time this turn} {closes ONLY this window ...}`
seq 68 row 3, same window one seam later: `... you have already declined this exact list 2 times this turn}`.
Every window from seq 67 on then prints
`[hold check: 1 row above is new since the last window at this seam - a row moving is what re-opens a hold,
so a hold taken here lasts only until one moves again]` — **and that claim is false**: no row moved, the
engine's own decline counter moved *inside* row 3's text. The seat answered `Hold priority` at seq 66, 68,
69, 70, 72, 73, 74, 75, 76 and was asked again every time; `[you declined this exact list N time(s)]`
climbed to **10** (this seat's maximum; wave 73's corpus maximum was ≤ 3). Board unchanged throughout — a
**re-put window, not a chain**. Diagnosis: CG moved the O11 count out of the ask/slot key but the count is
still rendered *inside the pass row's text*, so the hold latch's row-comparison sees a changed row at every
window. This is precisely what CH's "hold key = action identity with every render annotation stripped"
should fix; **I cannot confirm it, because none of this seat's six games ran on the CH binary** — the check
belongs to the engine seat's three rerun games.

**HIGH — `off_protocol_bytes` and `plan_line_missing` are the same event, not two.** All 16
`plan_line_missing` records in this seat are a plan sentence with the `PLAN: ` label dropped, and 12 of the
16 carry a non-zero `off_protocol_bytes` **equal to the length of that unlabelled sentence**: 126 seq21
(61 B, `'\n\nPlay Underground Sea to develop mana for Master of the Feast.\nCHOICE: 1 (Play Underground
Sea)'`), 146 seq23 (122 B), 125 seq94 (114 B), 125 seq76 (100 B), 126 seq30 (80 B), 126 seq29 (72 B), 125
seq80 (67 B), 130 seq27 (36 B), 146 seq15 (36 B), 125 seq89 (34 B), 125 seq85 (33 B), 125 seq75 (41 B). The
other four are replies with **no plan sentence at all** and 0 off-protocol bytes (126 seq25, 130 seq28, 125
seq64: `'\n\nCHOICE: 3 (Cast Underworld Dreams)'`). So the corpus-wide rise of `off_protocol_bytes` from
2.8% to 4.6% tracks the rise of `plan_line_missing` from 2.9% to 5.0% for the same reason, and
`off_protocol_bytes` here is **not** measuring prose in the reply — the reply protocol is being obeyed
except for one dropped label. Every answer line parsed; every choice was legal.

**MED — the STRANDS refusal is read as belonging to §ATTACKING, three times, in the model's own words.**
19 windows in this seat printed a `VERDICT: taking this row STRANDS` row (always row 1, always an upkeep
Dictate of Kruphix); **16 declined, 3 taken** — 123 seq10, 126 seq20, 130 seq27, i.e. all three of the
corpus's `plan_names_stranded_card`. Quoted in full as asked:

- **123 seq10** (t9 Upkeep, K=2): row `{reserve: TAKE THIS ROW AND YOU CANNOT CAST Master of the Feast AT
  ALL THIS TURN ... VERDICT: taking this row STRANDS Master of the Feast this turn - 2 sources cannot pay 3,
  and it has no window after your main phase}`; reply `PLAN: Cast Dictate of Kruphix in upkeep, play land,
  cast Master of the Feast in main phase 1. / CHOICE: 1 (Cast Dictate of Kruphix)`.
- **126 seq20** (t14 Upkeep, K=1): `VERDICT: taking this row STRANDS Teferi's Puzzle Box this turn - 2
  sources cannot pay 4`; reply `PLAN: Cast Dictate of Kruphix in Upkeep. Main phase: pass due to
  insufficient mana for Master or Puzzle. Next turn: cast Master of the Feast and Teferi's Puzzle Box,
  attack with Master. / CHOICE: 1 (Cast Dictate of Kruphix)`.
- **130 seq27** (t15 Upkeep, K=1): `VERDICT: taking this row STRANDS Teferi's Puzzle Box this turn`; reply
  `Win on their draw step with Dictate. / CHOICE: 1 (Cast Dictate of Kruphix)` (also a `plan_line_missing`).

  This is STRATEGY, not perception — the surface is correct and complete, and the reasoning quotes it. The
mechanism is new and specific: the model **mis-scopes the guide's own paragraph heading**. 123 seq10:
*"This is under 'LAST CHECK BEFORE YOU SEND.' which is under ATTACKING. It might not apply to casting."*
and again *"It's under 'LAST CHECK BEFORE YOU SEND.' which is in the ATTACKING section. It's safe."* and a
third time *"I will follow it just in case, but it's under ATTACKING."* 126 seq20 debates it for thirty
lines — *"the guide says 'A row you picked whose VERDICT begins taking this row STRANDS -> your answer is
WRONG' ... But Dictate is an engine. K=1"* — and resolves *"the brake overrides it for engines when K>=1"*.
130 seq27 dismisses it as informational: *"This is just informational. It doesn't prevent me from casting
it. It's fine."* The wave-73 edit A sentence lives in the K-of-1 bullet and none of the three quotes reached
it: 123 and 126 both quote the bullet's first clause and stop one sentence short. **Paid edit A.** Note the
rate did improve — wave 73 was 2 taken of 6 offered (33%), wave 74 is 3 of 19 (16%) — and all three takes
were in games the seat won, two of them with a correct lethal count. That is why edit A is a re-siting of
the rule, not a new prohibition.

**MED — no `blocker_forecast_*` or `wall_miss_*` event fired in 203 decisions**, and `sibling_window_asks_skipped`
and `chain_windows_collapsed` are 0 in all six games again. This seat's only two blocker windows (130 seq18,
seq25) printed full per-attacker rows with no collapse and no gang tag, so I cannot say whether the collapse
path is reachable; I did not read the render code.

**LOW — O2 attackers entry: PASS for this seat, 4 of 4.** Every own turn whose board line printed
`N of them able to attack right now` with N > 0 (130 t9, t11, t13, t15) has an `attackers` record. No misses,
and no turn where the seat never passed.

**Passes worth recording.** `reserve_decline_windows_noted` 4 / `skipped` 1 in the 152 game — the re-armed
decline latch fired and saved a call. `ask_replays_reserved` 1 = `identical_ask_answers_reserved` 1 (125
seq99, genuine, above): the two counters name the same event here, so **1 is not a regression in this seat**
— it is the number of times two identical windows actually occurred back to back. The t27 DRAW FORECAST
render self-corrects mid-chain and is correct: `(Teferi's Puzzle Box: their hand size 14) - 10 of them have
ALREADY been drawn and paid for in this step ... so 6 are still to come ... = 6 x 2 = 12 life LOST BY THEM
... they would be at -10; that KILLS them`. `main_phase_windows_skipped` is 2 across the whole seat (both in
125), so O10's corpus figure of 72 gets no evidence from here either way. Card facts re-checked against
`bin/Res/sets/primitives/mtg.txt`: Fate Unraveler `{3}{B}` 3/4 `auto=@drawfoeof(player):damage:1 opponent`,
Master of the Feast `{1}{B}{B}` 5/5 flying `auto=@each my upkeep:draw:1 opponent`, Ob Nixilis 5 loyalty with
`-2` destroy-and-draw-two — all match Oracle and match what the prompts rendered. No card behaved contrary
to its rendered text in these six games.

## 3. Guide verdict: **EDIT**

`wave74/deck162/strategy.txt` — **19,985 bytes, from 19,918 (net +67)**, under the 20,000 ceiling. Three
edits, each paid by a decision in this corpus; the 67 bytes and the room for the adds come from five cuts
named after the table.

| # | before → after | paid for by |
|---|---|---|
| A | §CAST ORDER preamble gains, before rung 0: `BEFORE ANY RUNG: a row whose VERDICT begins "taking this row STRANDS" is NOT ON YOUR MENU - strike it out and read on past it. That is a CASTING rule and nothing in ATTACKING scopes it, no rung reaches a struck row, and no count revives it.` — and the paragraph heading `LAST CHECK BEFORE YOU SEND.` → `LAST CHECK ON EVERY CASTING ANSWER - this paragraph is about CASTING, not attacking.`, its rule gaining `and however certain the win looks` | 123 **seq10**, 126 **seq20**, 130 **seq27**. The word SEND is §ATTACKING's own verb in this guide (`SEND when the A-line prints NO tag at all`), and the model said three times in one game that the check therefore belongs to attacking. The rule now also sits at the top of the list every casting ask is read from, ahead of the rung the three takes entered at. `however certain the win looks` answers 130 seq27's *"I don't need Puzzle Box"* and 126 seq20's lethal argument. |
| B | §CAST ORDER: new rung `4b. MASTER OF THE FEAST {1}{b}{b} whenever their line reads 1 or more creatures. A 5/5 flier is a BODY and K does not brake a body; the card it hands them is the price of the only real blocker this deck casts.`; Master struck from rung 6's engine list; rung 0 retitled `BODY TURN, READ THIS RUNG FIRST ON EVERY CASTING ASK`; the K-of-0 bullet leads with `MASTER OF THE FEAST IS NOT ON THIS BRAKE AT ALL - it is a body, and their line reading 1 or more creatures casts it at any K, this ask; only "of which 0 are creatures" on their line holds it.` and drops Master from its braked list; the old trailing Master exception in LAST-DITCH RELEASE deleted | 152 **seq13** (the loss: rung 0's exact trigger printed, rung 6 quoted instead, dead next turn) and 146 **seq14** (*"The guide says K=0 brakes it ... I must not cast it"* with two Masters in hand and two attackers on their line). The guide said Master-at-K-0-with-their-creatures three ways in three places — braked in the K-of-0 list, permitted in LAST-DITCH RELEASE, permitted again in LAST CHECK — and the model took the braked list both times. One statement now, in the bullet the model actually reads, plus a rung of its own. **This is the edit that grew the guide**; it earned its bytes with both of the wave's losses. |
| C | §FORCED SACRIFICE: `... and FATE UNRAVELER only as the sole row - take the wall even when its toughness pays them MORE life.` → `... and FATE UNRAVELER only as the sole row. THE LIFE THEY GAIN NEVER DECIDES THIS ROW, in either direction: the wall goes when it pays them MORE, and the punisher stays when it would pay them LESS - "the wall pays them more, so the punisher is the cheaper row" is the trade you are refusing.` | 126 **seq11** (t9, Tribute to Hunger; rows `1. Shield Sphere (0/6) ... [they gain 6 life]` and `2. Fate Unraveler (3/4) ... [they gain 4 life; THIS IS NOT JUST A BODY: DRAW PUNISHER]`). The seat sacrificed the punisher. Reasoning verbatim: *"the guide says to take the wall even when its toughness pays them MORE life, but here the wall pays MORE life anyway. So sacrificing Fate Unraveler is definitely better."* The concessive `even when` was read as the rule's **condition**, and inverted it. The new text states the ordering flatly and refuses the trade by name. |

**Bytes returned** (all five are either unexercised in two consecutive waves or now duplicated by edit A):
(i) the `The row prints "in your hand:" TWICE ...` gloss distinguishing the two in-hand counts — no decision
in 203 turned on it; (ii) the MULLIGAN colour-tiebreaker sentence — 6 of 6 keeps, colour never argued;
(iii) the trample "never put your last blocker on a trampler" tail — no trampler in the corpus and the two
sentences above it carry the rule; (iv) the §{reserve:} opening restatement of the STRANDS refusal, now the
first thing in CAST ORDER; (v) the CLEANUP DISCARD ordering rationale, keeping the `PUT: n, m` emit form —
no cleanup-discard ask appeared in this wave or the last.

**Not changed, deliberately.** The K-of-0 brake itself (a success verdict: 125's t12–t22 hand-lock was the
brake doing exactly its job against a creature-light deck, and the seat won that game; edit B narrows it to
engines, not to bodies, and touches nothing else). Rung 0's arithmetic. §BLOCKING (130 seq18/25, both the
engine's own best assignment). §ATTACKING (130's four sends, all clean tags). §LAND DROP (every land-drop ask
answered with a land). §MULLIGAN (6 of 6 two-lander keeps). The hold paragraph rewritten last wave — its
advice is sound and the 125 t21 failure is an engine latch, not the guide.

## 4. Not checked / limits

- Only this seat's six translogs, the six opponent translogs for outcomes, and the six stderr files for
  land-drop and attackers traces. No lane adjudication, no corpus counters beyond my six gameends, no other
  deck's seats.
- **All six of my games are CG-binary.** Every hold, `hold_released_turn` (6: one in 126, five in 125),
  `hold_windows_skipped` (22) and re-put observation above is pre-CH. I did not test the CH latch and cannot
  say whether HIGH item 1 survives it.
- I did not trace the one `async_drops` (152) or the two `phase2_answer_recovered` (130) to their windows,
  did not audit latency or `transport`, and did not open the engine source.
- **UNTESTED, not passed**: the both-halves Sanguine Bond / Exquisite Blood branch (only `LOOP HALF
  PENDING`, 126 seq27–29); the Ob Nixilis legend-rule exception (no `[legendary: ...]` row appeared); Peer
  into the Abyss (never drawn); the O19 X-announcement path (no `ANNOUNCE_X` window); O4 MDFC land menus
  (none); cleanup discard (none).
- The 152 counterfactual — that taking Master of the Feast at seq13 survives t9 — rests on the row's own
  `{crack-back cover: ... you cover 4 of 10, leaving 6 -> you would be at 4}` clause and its own warning that
  the total is a floor. It is the engine's arithmetic, not a replayed game.
- The 146 loss is called removal, not guide: five one-for-ones on five permanents. Edit B's claim there is
  only that seq14 gave up two turns of the deck's only blocker, not that it would have won.
