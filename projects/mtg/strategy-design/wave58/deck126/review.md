# Wave-58 seat review — deck126 (Sanguine Blood, WGB wall prison + Sanguine Bond/Exquisite Blood)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-015059/` (binary master a3f0700d3, model
`qwen36-35b-a3b`), plus the F2 rerun `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-042235/` for
the deck152 pairing. Every count below is re-derived from the rendered `prompt` of the seat JSONL
(`/usr/bin/grep`, python), never from `options`. 7 seat logs, **305 decisions**, 17 fallbacks,
**114.7 min** of model wall time. Card facts verified against
`bin/Res/sets/primitives/*.txt` (Perimeter Captain, Pride Guardian, Sanguine Bond, Exquisite Blood,
Tribute to Hunger, Staff of Nin — all match the guide and the rendered card text; no Scryfall
disagreement found, so no both-sides report is needed).

Record **4-2**. Four wins are combo kills; both losses are traced below.

---

## 1. Game by game

| # | pairing | result | turn | how it ended |
|---|---|---|---|---|
| 1 | 126 vs 146 | **LOSS** 0-21 | 30 | flooded to 10 lands, walls traded off, Kaya exiled the Blood, Silencer named the Bond |
| 2 | 126 vs 162 | **LOSS** 0-20 | 11 | Underworld Dreams + Howling Mine + Teferi's Puzzle Box; died in a draw step |
| 3 | 126 vs 123 | **WIN** 40-0 | 16 | Bond t9, Blood t11, Tribute on Thraben Doomsayer t16 → loop |
| 4 | 125 vs 126 | **WIN** 39-0 | 16 | Bond t8, Blood t14, Staff of Nin t16 → ping → loop |
| 5 | 126 vs 130 | **WIN** 33-0 | 24 | Bond t17, Blood t23, Staff ping at t25 upkeep → loop |
| 6 | 152 vs 126 | **HUNG** (F2), rerun **WIN** 25-0 | 18 (rerun) | Bond t14, Blood from exile t18, two lifelink Vampires connect → loop |

**Deciding decisions.**

*Game 3 (vs 123), `1788504668-ai_baka_deck126-0x557bd3249f10-vs-ai_baka_deck123.jsonl`* — seq 9 cast
Sanguine Bond at the first entry-1 row; seq 11 Idyllic Tutor on a **live** row, seq 12 revealed
Exquisite Blood (the half not on the battlefield line — Rule #3 point 4, correct); seq 13 cast it at
"casting this taps you out". seq 42 turn 16: `Cast Tribute to Hunger {2}{b} {right now: they control
1 creature - Thraben Doomsayer (2/2) is sacrificed...}` taken **in its first window** — Rule 7b,
and it won the game on the spot (seq 43-62 are the loop resolving 20/20 → 40/0).

*Game 4 (vs 125), `...deck126-0x55a199fd93d0-vs-ai_baka_deck125.jsonl`* — seq 8 Bond, seq 47 Blood,
seq 59 Staff of Nin, seq 60 `Deal 1 damage with Staff of Nin targeting the opponent` → 39-0. Exactly
Rule #7a. One misplay, seq 34 (below, item MED-3).

*Game 5 (vs 130), `...deck126-0x557cda71a050-vs-ai_baka_deck130.jsonl`* — seq 19 Tribute on
`Dwarven Blastminer (1/1) is sacrificed` in the first window (the guide's own named case); seq 42
Blood completes the pair on t23; seq 55 Staff ping at the next own upkeep → 33-0.

*Game 6 rerun* — seq 16 is the **only HOLD take in the whole seat** (`hold_windows_skipped: 5`);
seq 31 `Cast Exquisite Blood {6}{b} [from exile]` completes the pair; seq 33 `ATTACK: Vampire,
Vampire` → 25-0. Rule #2's lifelink-attack entry, correctly.

*Game 1 (vs 146), `...deck126-0x55f8e7326c20-vs-ai_baka_deck146.jsonl`* — mostly variance (ten lands
on the battlefield by t30, hand of one Tribute for four turns) plus a very strong opponent board
(Lolth, Kaya, Nadaar, Barrowin, Silverquill Silencer). Two real decisions:
 - seq 47 (turn 30, **1 life**): the Idyllic Tutor search list offered `10. Sanguine Bond`,
   `24. Sanguine Bond`, `28. Exquisite Blood`; the opponent battlefield line read
   `Silverquill Silencer {b}{w} (4/3) [named: Sanguine Bond]`. The seat picked **Sanguine Bond**.
   One window later (seq 48) the engine priced it exactly: `Cast Sanguine Bond {3}{b}{b} [NAMED BY
   THEIR Silverquill Silencer: casting this costs you 3 life ... you would be at -2; this KILLS
   you]`. **The search bought the one card the seat could not cast**, and it was the last live
   window of the game. STRATEGY (guide gap — see §3 edit E2).
 - seq 48-50 themselves are **correct**: the seat read the `this KILLS you` price and took
   Perimeter Captain, then Chromatic Lantern, over the entry-1 row. That is the guide's one
   entry-1 override working, off a lane-B-class rendered verdict.

*Game 2 (vs 162), `...deck126-0x55fec2117090-vs-ai_baka_deck162.jsonl`* — a matchup this deck has no
card for (their whole clock is Underworld Dreams + Fate Unraveler; deck126 runs no enchantment or
artifact removal, and 16 walls block nothing). The one decision that was still available: seq 17,
turn 9, 16 life, 3 untapped sources, `DRAW FORECAST: your next draw step draws 7 cards (1 + Howling
Mine 1 + Teferi's Puzzle Box: your hand size 5) = 7 x 1 = 7 life`. Menu rows: `1. Cast Idyllic
Tutor`, `2. Cast Pride Guardian {w}`, `3. Cast Overgrown Battlement {1}{g}`. The seat took the
Tutor — **hand-size neutral**. Rows 2+3 together cost exactly the 3 sources it had and would have
taken the hand from 5 to 3, i.e. the forecast from 7 draws to 5. The seat's own PLAN that window
read "keeping hand size minimal (ideally 0-1 cards)" and its choice did not execute it. STRATEGY
(guide gap — see §3 edit E3). Note the seat then had **no further decision**: it died in its own
turn-11 draw step after deck162 added a second punisher (Fate Unraveler, their seq 11).

---

## 2. Engine / interface / card items (primary deliverable)

### HIGH-1 — `empty_reply` is a fixed-latency TRANSPORT failure, and it is being recorded as a model behaviour
17 of this seat's 305 decisions are fallbacks; 16 are `empty_reply`. **Every `empty_reply` in the
whole 21-game corpus (89 of 89) has `latency_ms` in 2502-2558 ms**, against a median of 12,692 ms
and a range of 992-963,791 ms for answered windows. A constant ~2.51 s with a zero-length reply is a
server/HTTP-level rejection, not a model that declined — and `AIPlayerGPT.cpp:10315` already knows
this ("`empty_reply` - the same word an unreachable endpoint writes"): the classifier only separates
`reasoning_only` and `timeout`, so a 5xx/connect failure and a genuinely empty completion land in the
same bucket. The stderr carries nothing either: `game-126v123-1788504665.stderr` has two
`AIPlayerGPT: using http://100.116.136.74:8084` lines and **no** HTTP or curl line at all.
Repro: `1788504668-ai_baka_deck126-0x557bd3249f10-vs-ai_baka_deck123.jsonl` seq 17,19,21,23,25,27,29,31
— eight consecutive refusals covering *every phase of turn 14*, each `latency_ms` 2507-2514, each
followed by a `recovery` record. Also `...deck126-...deck130` seq 13,15 (that pair contains this
seat's **only** missed land drop — seq 13, turn 15, a `Play` menu answered `<refused: empty_reply>`;
Rule #0 is otherwise 49 of 49).
**Ask:** stamp the HTTP status / curl error on the record and split the class (`http_error` vs
`empty_reply`), so 89 lost windows stop reading as model refusals. This also matters for F2 below.

### HIGH-2 — 32 byte-identical asks in one turn while a decided drain loop resolves
Once both enchantments are on the battlefield, each iteration of the Bond/Blood loop hands the seat
priority again. `...deck126-...deck130` seq **56-87**: 32 consecutive asks in turn 25's Upkeep, the
same two-row menu (`1. Cast Tribute to Hunger {right now: they control 0 creatures - at 0 this does
nothing}` / `2. Cast nothing right now` / `3. Hold priority...`), the only difference between
prompts being the life totals in the header as they run 17/17 → 33/0. The declined-note reached
`[you declined this exact list 32 times already this turn]` (seq 87). Same shape in
`...deck126-...deck123` seq 43-62 (20 asks, 20/20 → 40/0). **52 model round-trips across two games
narrating a win that cannot be interrupted**, and `identical_option_asks_resolved` reads **0** in
every one of this seat's `gameend` records.
The HOLD row cannot absorb these: its re-open predicate is "any change on the board", and the life
totals change every iteration. Repro line above; proposal in `general-proposals.md` (P1).

### HIGH-3 — F2 correction: the Sigarda menu DOES have a translog receipt, and the -1 came from the empty-reply fallback
`wave58/engine-findings.md` §F2 states "The Sigarda menu itself has NO translog record" and reads the
deck152 seat's last record as a Blockers decision. Both are off.
`matchups-20260904-015059/1788504666-ai_baka_deck152-0x55a7f2ef7f70-vs-ai_baka_deck126.jsonl`
**seq 30** (`kind: ask`, turn 11, phase Blockers, `latency_ms 2542`, `fallback: empty_reply`,
`choice: -1`, `chosen_text: <refused: empty_reply>`) renders exactly:
```
Choose an option for Sigarda, Champion of Light:
1. get a human
2. put on bottom
```
So the `-1` in stderr is **this record's choice**, written by the empty-reply fallback path — not a
silent no-model path. The three records before it are the same 2.5 s failure class: seq 26
(`attackers`, 2503 ms), seq 28 (`reveal` — the "top 5, PUT: which go to get a human" list, 2502 ms),
seq 30. So the F2 precondition chain is: **transport failure → fallback answers -1 on a `may` menu →
the trigger's chooser is never released → no tick for 3640 s.** HIGH-1 is therefore the *trigger*
for F2, and a lane-G fixture should drive the menu with a forced -1 rather than hunting a model
behaviour. Two further facts for lane G: (a) the engine asked the **reveal** (which cards go to "get
a human") at seq 28 *before* asking the may-menu (whether to "get a human") at seq 30 — the ordering
is inverted, and a -1 on the second makes the first meaningless; (b) the deck126 seat's log
(`1788504666-ai_baka_deck126-0x55a7f422e400-...`, 13 records) ends cleanly at seq 12, turn 10 — it
was never asked anything during the hang, which is consistent with a parked chooser on the 152 seat.

### MED-1 — a gang block prints no result for either blocker
`...deck126-...deck146` seq 41 (turn 25). A1 is `Spider (3/2) [reach, menace ...]`; both B-lines read
`may block A1 (no 1-on-1 result exists - only a block by TWO OR MORE of your creatures counts; this
one alone does not block it at all)`. The header correctly says "1 of the 2 attackers need TWO or
more blockers each; you have 2 blockers, enough to complete at most 1 of those blocks" — but nothing
anywhere prices the block the seat is actually being asked to make. The seat assigned B1+B2 to A1
(guide-correct) and Pride Guardian died; it had no way to know which of the two would. A menace
attacker's A-line should carry the *pair* result the engine already computes for the legal gang
(who dies when N blockers of these sizes block it), not only the 1-on-1 that cannot legally happen.

### MED-2 — a search/reveal row is never priced, though the cast row one window later is
`...deck126-...deck146` seq 47: a 34-row Idyllic Tutor search list, each row
`<name> (enchantment) {text: ...} [eligible for "choose card"]`, with **no annotation of any kind**.
The `[NAMED BY THEIR Silverquill Silencer: ... this KILLS you]` verdict that the engine printed on
the cast row at seq 48 was fully computable at seq 47 (the naming permanent is on the board, the life
total is known). A search is a cast decision made one turn early; the row that fetches a card the
pilot cannot legally survive casting should carry the same finished price. This is the single
highest-value render item in this seat's corpus — it decided the last live window of game 1.

### MED-3 — the hand line never says WHY a hand card has no cast row
`...deck126-...deck125` seq 34, turn 12, Main 2, 4 untapped sources. Hand line:
`Exquisite Blood {4}{b} [enchantment]`. Menu: one Tribute-at-0 row, decline, hold. The seat replied
`CHOICE: 1 (Cast Exquisite Blood)` → `<refused: named_row_reask>`, and burned the window. The prompt
prints the mana line and the hand line but never the shortfall the engine already computed when it
declined to emit the row. One true clause on the hand card — *"needs 5 sources; you have 4"* — closes
this class. (`not castable` / `needs N sources` occurs **0 times** in 305 prompts of this seat; the
only near-hit is a model-written PLAN echo trying to do the arithmetic itself and getting it wrong.)

### MED-4 — the PLAN echo re-injects a paragraph of self-debate as fact
The reply protocol asks for "a PLAN: line"; the engine echoes whatever came back, whole.
`...deck126-...deck146` seq 43 renders `YOUR PLAN (as you last stated it, 1 window ago on turn 26):
Exquisite Blood is now on the battlefield. I have both halves of the combo (Exquisite Blood and
Sanguine Bond is not yet on battlefield, wait - I do NOT have Sanguine Bond. ...)` — 9 sentences of
visible self-correction, the first of which is false, re-served to the seat as its own standing plan.
Same shape at `...deck126-...deck125` seq 34 (a "Correction: I just cast Exquisite Blood" paragraph
about a card still in hand). Either cap the echo at its first sentence or echo only a PLAN that is a
single line; a multi-sentence echo makes a false premise durable across windows.

### LOW-1 — `mana_only_windows_skipped` is large and silent, which is good; say so once
125 / 127 / 198 / 14 across four games (and 0 in two). No complaint — recorded as PASS so no later
lane trims it. Likewise `[you declined this exact list N times already this turn]` renders correctly
(max 32) and `hold_windows_skipped` reconciles (5 on the one game with a take).

### LOW-2 — the Battlement's VARIABLE-output note is doing its job
243 prompts print the `Overgrown Battlement {g} (VARIABLE output: ... the total above counts it as
one source, not as its mana)` clause. **3** menus printed an `Add N green mana with Overgrown
Battlement` row and **0** were taken. Rule #1 is at 100%; the render can be left alone.

---

## 3. Guide verdict: **EDIT** (three additions, paid for by trims of now-learned material)

**What is working, and should not be touched** (all re-derived this step):
* Rule #0 land drop: **49 of 49** answered windows played a land (the 50th was HIGH-1's empty reply).
* Rule #1 upkeep Battlement tap: **3 rows, 0 takes**.
* Rule #4 zero stop: **160 `at 0 this does nothing` rows, 0 casts**.
* Rule #7 entry 1: **14 windows with a live entry-1 row for a missing name, 10 taken**; of the four
  not taken, three carried `this KILLS you` (correct declines) and one was an empty reply. On
  answerable, unpriced windows this entry is **10 of 10** — #313's read instruction is confirmed and
  the wave-57 4-of-11 break is closed.
* Rule #7 entry 2: **11 live-Tutor rows, 10 taken; 4 dead-Tutor rows, 0 taken.**
* Rule 7b: taken in the first printed window both times it fired (Thraben Doomsayer, Dwarven
  Blastminer) and both won or nearly won their game.
* Second-copy rows: **0 rendered** — the guide's stop cost nothing and is kept in compressed form.

**Why EDIT anyway.** Three gaps, each paid for by a citation above.

### E1 — ship the HOLD-row block (new RULE #8)
Skill amendment **#299** sunset the withheld-affordance control at three corpora and **#312** ruled
it **RELEASED** ("any future wave that wants the block may ship it as an ordinary guide improvement,
with no experimental obligation on either side"). Wave 58 is the free observation #329 asks for:

| corpus | deck125 (taught) | deck126 (control) | ratio |
|---|---|---|---|
| wave 55 | 10.1% | 1.5% | 6.7x |
| wave 56 | 18.5% | 2.7% | 6.9x |
| wave 57 | 15.9% | 0.6% | 26x |
| **wave 58** | **65 of 414 = 15.7%** | **1 of 228 = 0.4%** | **36x** |

Opponent-turn arm, wave 58: deck125 **18 of 156 = 11.5%**, deck126 **0 of 95 = 0.0%**.
`hold_windows_skipped` **287 against 5**. Price paid this corpus: **147** answers of "Cast nothing
right now", **149** windows under the engine's own `NO LIVE CAST ROW ON THIS MENU` header, ~half of
114.7 minutes of inference. No game was lost to it, but game 1's turn 19 is eleven windows and game
5's turn 25 is thirty-two.
*before:* the string `Hold priority` appears **0 times** in the guide.
*after:* a `=== RULE #8 - THE HOLD ROW ENDS THE TURN'S DEAD WINDOWS ===` section quoting the row's own
promise, naming the two printed triggers (`NO LIVE CAST ROW ON THIS MENU`, `[you declined this exact
list N times already this turn]`), stating the one case where "Cast nothing right now" is still
right (waiting on a Tribute target or on the stack), and reconciling with Rule #2's
"never your answer" line. Paid for by seq 56-87 / seq 43-62 (HIGH-2) and the 228:1 census.

### E2 — Rule #3 point 4: the search must skip a half their Silverquill Silencer has named
*before:* "...if you have NEITHER, pick SANGUINE BOND ... THE LINE YOU READ IS YOUR OWN BATTLEFIELD
LINE, EVERY TIME, INCLUDING THE FIFTH SEARCH: name the half that is NOT printed there."
*after:* same, plus "AND SKIP A HALF THEIR Silverquill Silencer HAS NAMED. When the OPPONENT
battlefield line prints `[named: Sanguine Bond]` (or Exquisite Blood), casting that half costs you 3
life, and the search list is where you choose which half you are going to have to cast. Take the
OTHER one - even when it is the half this entry would otherwise send you after, and even when you
hold neither." with the seq 47 → seq 48 record written in.
*paid for by:* `...deck126-...deck146` seq 47 (pick) / seq 48 (`you would be at -2; this KILLS you`).
Note the guide's existing `[named:]` section covers only *casting*; nothing in it reached a search.

### E3 — a SITUATIONS entry for DRAW PUNISHERS / DRAW FORECAST
*before:* the guide names neither string; it has no line about hand size at all.
*after:* a bullet reading the `DRAW PUNISHERS on the battlefield` and `DRAW FORECAST: your next draw
step draws N cards ... = N life` renders as a clock, with the one lever this deck owns (hand size,
because Teferi's Puzzle Box redraws the whole hand), the instruction to prefer two cheap permanents
to a card-neutral Idyllic Tutor, and the note that on such a board a Tribute on a `[defender]` is
simply life (nothing of theirs attacks — which also retires the awkwardness of the guide's
10-life carve-out being violated at 12 life in that same game, seq 15, harmlessly).
*paid for by:* `...deck126-...deck162` seq 17 (forecast printed, Tutor taken over two castable
walls) and the gameend at turn 11 with no further decision.

**Trims** (no rule deleted; all are anecdotes for behaviours now measured at 100%, and the entry-1
read instruction, the zero-stop, the second-copy stop and the dead-Tutor stop all survive as rules):
the Rule #4 zero-stop anecdote, two Rule #1 anecdotes, the Rule #7 entry-1 "broke this corpus" case
list, the entry-1 second-copy and taps-you-out anecdotes, the Chromatic Lantern second-copy tail, and
four SITUATIONS bullets merged or shortened.

**Size:** live guide **70,111 B** → revised **70,997 B**, inside the 41-71 KB pool band.
Revised guide: `wave58/deck126/strategy.txt`. The exact byte-splice that produced it (every
before/after pair, asserted unique) is kept at `wave58/deck126/make-strategy.py`.

---

## 4. Proposals filed
* `wave58/deck126/general-proposals.md` — three render/core-prompt items (P1 the identical-ask loop
  and the HOLD re-open predicate, P2 the unpriced search row, P3 the hand-line shortfall).
* **No `skill-proposals.md`.** The three lessons this seat could have offered are already
  amendments: #312 (untaught row ≈ 1/20th take rate — this step only adds a fourth data point),
  #313 (an ordered list ships a read instruction — confirmed PASS here at 10/10), and #314 (a guide
  may not invite a mid-reply change of mind — no violation this corpus). Restating any of them
  would be noise.

## 5. What I did NOT check
* I did not read the deck146, deck123, deck125, deck130, deck162 or deck152 **guides**; opponent
  translogs were read only for context (deck162's Fate Unraveler cast, deck152's F2 tail).
* I did not run wagic, build anything, or open a debugger; the F2 core/stderr beyond the lines
  quoted above is the engine seat's and lane G's.
* I did not verify the `empty_reply` mechanism in code beyond reading the classifier comments at
  `src/AIPlayerGPT.cpp:10279-10322` — I have **not** confirmed that the 2.5 s figure is a curl
  connect/retry budget, only that it is constant across 89 of 89 records and unlike every answered
  window. Someone should check the endpoint's own logs for that window.
* I did not adjudicate any lane A-E prediction (that is the engine seat's job); where a lane's
  output touched a string I quoted (D1's lethality verdict at seq 48, D2's header at 149 windows,
  D3's zero-verdict badges at 160 Tribute rows) I report only what the seat's prompts show.
* I did not re-derive deck125's HOLD numbers beyond the same census script; they are from this
  corpus's deck125 seat logs, not from that seat's own review.
* The two losses were not counterfactually simulated — the vs162 line in E3 is an argument from the
  printed forecast and the printed mana, not a proof the seat survives.
