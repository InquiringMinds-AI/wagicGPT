# Wave 74 — deck 123 (Intruders of Thraben, WBU token combo) — per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260909-104713-final`, **MIXED BINARY**: five of my
six games (vs152, vs162, vs130, vs126, vs146) ran on `archives/wagic-60e9fe2ad-w74step1b`
(source 1d1d3744d, through lane CG); **vs125 is the CH rerun** (`archives/wagic-76c5fab24-w74step1c`,
source aca5e06f8, start_epoch 1788998173 ≥ 1788998000). `--thinking on`. Live guide = the wave-73
edition + lane CE's O17 rename. **3 wins (vs130 t9, vs126 t12, vs125 t24), 3 losses (vs152 t15,
vs162 t12, vs146 t17)** — up from 2/6.

Seat census: **344 decisions** (152: 37, 162: 42, 130: 31, 126: 53, 146: 51, 125: 130), 100% native
reasoning (median `reasoning_chars` 6,261, max 22,254), **0 `off_protocol_bytes`>0**, **1
`plan_line_missing`** (the fallback record), 342/344 PLAN lines, **34 in `stop=<N>, M=<N>` format**,
0 `action_before_plan`, 0 `reply_truncated`, latency p50 91.5 s / p95 215.6 s, `deadline_pct` p50
10.1 / max 49.8. **1 fallback: the corpus's only `http_error`** (below). Gameend sums for this seat:
`ask_replays_reserved` 206, `identical_ask_answers_reserved` 0, `hold_released_turn` 11 (126: 2,
146: 2, **125: 7 — the CH binary**), `hold_windows_skipped` 109 (cast 106 / priority 3),
**`stop_reached_windows_skipped` 30 — again every one of the corpus's 30** (162: 4, 130: 13,
125: 13), `main_phase_windows_skipped` 2, `reserve_decline_windows_noted` 3, `async_drops` 8,
`phase2_answer_recovered` 1 / `missing` 1, `plan_names_stranded_card` 0, `chain_acting_rows` 154,
`chain_selfharm_rows` 0, `sibling_window_asks_skipped` 0, `chain_windows_collapsed` 0,
`repeat_annotated_takes` 0, `menu_pass_no_progress` 0.

## 0. The three things the brief asked me to verify first

**(a) Wave-73 HIGH-1 — the plan carry stripping `stop=;M=` — is CLOSED (O7/F9 PASS).** I paired
every rendered `THE PLAN YOU LAST STATED ... reads "…"` against the PLAN it derives from, in all six
games. **20 carries derive from a stop-bearing PLAN; 20 of 20 keep the clause; 0 stripped.** The
clause now rides as state and the *executed* step is what gets consumed — e.g. vs130 **seq 14**
wrote `PLAN: stop=23, M=1; this window tap Doomsayer to create tokens, then attack with everything
next turn.` and **seq 15** carried `reads "stop=23, M=1; then attack with everything next turn."`;
vs126 **seq 56 → 57** likewise (`stop=25, M=24; … x1, then hold priority…` → `"stop=25, M=24; then
hold priority, then attack with all creatures on my turn."`). No wave-73-style stop drift followed:
the worst wobble is vs126 **seq 57/58** (`stop=46` invented off L=19) and vs125 **seq 175**
(`stop=80` with M=40), both self-corrected on the next window and neither cost a board.

**(b) `stop_reached_windows_skipped` 30 vs wave 73's 366 — fewer loops, NOT a broken parse.** The
stop still parses everywhere I can see it: 34 PLAN lines in stop format, the
`{right now: M=…, your stated stop=…}` clause rendered **38 times**, and all 30 collapses fired in
this seat. Wave 73's 366 came almost entirely from one 76-turn / 372-decision game where the seat
sat past its stop for dozens of turns; this wave's longest game is 24 turns / 130 decisions and
three games ended by turn 12. Per own-turn the rate is comparable; the denominator collapsed.

**(c) O7b and O7c are both TRUE as rendered.** O7b: I enumerated every Create/token row in the
seat's six games — **46 of 46 carry the stop clause**, including the single plain `[cost: Tap]` row
(vs125 **seq 170**, vs162 **seq 61**). The rows that lack it are Equip rows, not Create rows, so
wave-73 MED-2 is CLOSED. O7c rendered **6 times**, all in vs125 (**seq 196, 197, 198, 202, 204,
210**), all reading `your stated stop was set when their life was 37; it is now 40` — and that is
**true**: the seat wrote `stop=40` at **seq 147** with `opp_life` 37, and the opponent's Sphinx's
Revelation had since taken them to 40. **It also worked**: at **seq 210** the reasoning recomputes
`Stop = L + C + 3 = 40 + 0 + 3 = 43`, the seat made three more tokens (**seq 222, 228**) and
attacked at **seq 243** for exactly `-1`. This closes wave-73 MED-3 with a win attached. **No false
fact found in either clause.**

## 1. Game by game

**vs deck130 — WIN, turn 9 (16 / −4).** Doomsayer t5, Alarm t7, `PLAN: stop=23, M=1` at **seq 14**,
21 tokens at **seq 15**. **The wave-73 edit-1 fires and works: seq 16** — board
`23 creatures, 1 of them able to attack right now`, the lone attackable creature the Doomsayer,
`ATTACK TOTAL: 1 attacker … 2 total combat damage … That is NOT lethal` — the seat answered
`ATTACK: none to preserve the Doomsayer engine`. It then rebuilt on **their** turn (**seq 19**,
per TIMING THE CHAIN) and swung 23 for `-4` at **seq 38**. This is the whole guide executed in
order. **CHECK -1 is validated.**

**vs deck126 — WIN, turn 12 (18 / −86).** Slip, Keeper t8, Alarm t10, chain fired on the
opponent's turn 11 (**seq 54, 56, 58**), transform at **seq 59**, 26 fliers for 105 at **seq 70**.
Two violations, neither decisive:
- **seq 52 (t10) — the lone-maker attack, and the only CHECK -1 breach this wave.** The list was
  `A1. Bloodline Keeper (3/3) [flying, doesn't untap during its controller's untap step] [no
  creature they control can block this attacker]` with `ATTACK TOTAL: 1 attacker listed, 3 total
  combat damage … That is NOT lethal: they survive at 19`. The sole override did not apply; the seat
  sent it anyway and tapped its only untapper. **It got away with it for a reason worth recording:**
  on their turn 11 the opponent cast Wall of Omens and Overgrown Battlement, and the *symmetric*
  Alarm untapped the Keeper for them (**seq 53**, events). The guide's CHECK -1 rationale claimed
  the board would be "tapped for the rest of the game", which this game shows is not literally true
  — hence edit 3 below.
- **seq 42 (t4) — Tragic Slip cast on a row reading `{kills 0 of the 2 CREATURE targets at -1/-1}`,
  then targeted at **seq 43** a Perimeter Captain whose own row ended `- SURVIVES`.** Card burnt for
  nothing, against two separate printed gates.

**vs deck125 — WIN, turn 24 (14 / −1). The wave-73 76-turn disaster reversed.** Tutor→Alarm found
t6, but no maker stuck until t22 (deck125 is Cancel/Dream Fracture/Path/Verdict + Sphinx's
Revelation + two Staff of Nin). Doomsayer t22 (**seq 140**), Alarm (**seq 141**), Greaves equipped
(**seq 158**), 38 tokens (**seq 173**). The seat then held a 40-creature board through the whole of
turn 23 without a single wasted tap, corrected its stop off the O7c note (**seq 210**), and killed.
**No lone-maker attack, no over-tapping, no edict violation, no sweeper blowout.** The one soft spot
is that it built in its own main phase against a deck whose graveyard/log carries Supreme Verdict —
SWEEPER SEEN says fire at their end step. This time the sweeper never came; the rule was not
tested, and I am not scoring the win as evidence the rule is unnecessary.

**vs deck162 — LOSS, turn 12 (0 / 18). The deciding decision of my six games, and it is a
STRATEGY error against a printed price.** Deck162 runs two Underworld Dreams. Doomsayer t6, Alarm
t8, 21 tokens made on their turn 11 (**seq 60**, `PLAN: stop=24, M=3; this window x21`). At the
start of the seat's own turn 12 the board read
`31 permanents listed, of which 24 are creatures, 24 of them able to attack right now`, Fateful hour
active (`Human #1-#23 (3/3) (printed 1/1)`), opponent at 18. That is ~72 power against 18 life —
**lethal was already on the board and the only remaining job was to reach the attack step.**
- **seq 62** the seat read it correctly and declined: `PLAN: stop=22, M=24; this window pass, then
  attack for lethal with all creatures.` `CHOICE: 3 (Cast nothing right now)`.
- **seq 63**, the very next window, **the identical Vision Skeins row was re-put and the seat took
  it**, at 5 life, against a row that priced itself:
  `Cast Vision Skeins {1}{u} … [DRAW PRICE: this draws 2 cards, and the opponent's Underworld Dreams
  #1, Underworld Dreams #2 punish every draw, so taking it costs you 4 life right now - you would be
  at 1]`. Its plan: *"Cast Vision Skeins to draw into a creature, untap board with Intruder Alarm,
  and attack for lethal next turn."*
- It never reached combat. **seq 64–68** are five passes while Underworld Dreams triggers resolve;
  by **seq 67** it is at 2 with 5 more damage on the stack, and its own reasoning states
  *"I haven't reached main phase yet. I can't attack."* Game over at 0 / 18 with 24 lethal attackers
  untapped on its battlefield.
  The guide already forbade this three times over (P7's "only with no maker"; the Skeins bullet's
  "punishes drawing"; ORDER OF OPERATIONS' "lethal on the board — attack, do not build") and none of
  the three was placed where a cast window would hit it. **Edits 1 and 2 exist for this seq.**

**vs deck152 — LOSS, turn 15 (0 / 20). Variance, plus the corpus's only `http_error`.** The seat
drew five lands and no maker for fourteen turns (`of which 0 are creatures` on every own-turn board
line through **seq 59**), and the opponent's Elite Spellbinder exiled an Intruder Alarm from hand at
t13. Damnation was cast correctly at **seq 20** (`destroys 2 of their creatures (2 without a
restriction), 0 of yours`). One violation: **seq 51** cast Tribute to Hunger on
`{right now: they control 2 creatures - they choose which one - you gain at least 1 …}` — N = 2, the
gate says no edict at the opponent at any N but 1. They sacrificed Luminarch Aspirant, so it cost
nothing here; the gate still lost. **seq 64** is the fallback (below), recovered by the heuristic
casting Bloodline Keeper — the play the guide would have picked anyway.

**vs deck146 — LOSS, turn 17 (−2 / 17). Variance, and the wave's worst interface incident.** No
maker resolved before t10; the t14 Doomsayer died; the t16 rebuild came under a lethal board. Slip
at **seq 4** and Devour Flesh at **seq 52** (N = 1, Tribute not listed) both obey the file. Land
drop taken every turn. The game's real content is **the 13-ask re-put loop at turn 7 (HIGH-1
below)**.

## 2. Engine / interface / card items

**HIGH-1 — on the CG binary the hold latch re-opens on its OWN declined-counter, and the hold row's
printed promise is therefore a FALSE fact.** Repro:
`1788968878-ai_baka_deck123-0x5604db7ff540-vs-ai_baka_deck146.jsonl`, **turn 7, seq 39–51 — thirteen
asks in one Main phase 1**. From **seq 46 through seq 51 the `events` field is EMPTY** (board
unchanged) and the option list is three rows. I diffed the lists: **the only byte that changes
between consecutive asks is the number inside row 3's
`{… you have already declined this exact list N times this turn}`** (7 → 8 → 9 …); row 2, the hold
row, is byte-identical throughout. The seat took the hold at **seq 39, 42, 44, 45, 49 and 51** — six
times — and the window re-opened every time, while row 2 promised
`Hold priority - pass now, and do not ask me again - YOU CANNOT COME BACK AND TAKE ONE OF THE ROWS
ABOVE LATER THIS TURN … it stands until your next turn begins, or until one of the rows above
changes (any change re-opens this window)`. The counter is rendered *into* a row, so the "any change
re-opens" test fires on the annotation the previous re-open produced: a self-sustaining loop.
`declined this exact list 12 times` at **seq 51** is 4× wave 73's maximum of 3, and the thirteen
asks cost ~18 minutes of wall clock for zero decisions.

**Split by binary, and it looks like CH fixes it.** Across the seat's 40 taken holds I checked
whether the next ask of the same seam carried no board event:
| binary | games | holds taken | re-opened with no board event | of those, list identical mod the counter |
|---|---|---|---|---|
| CG (60e9fe2ad) | 152, 162, 130, 126, 146 | 24 | 12 | **8** (146 seq 17→18, 18→19, 19→20, 20→21, 21→22, 45→46, 49→50; 126 seq 25→26) |
| CH (76c5fab24) | 125 | 16 | 2 | **0** — both had genuinely different row lists (seq 42→43 and 132→133 lost castable rows to mana/phase) |
So on the CH binary this seat produced **no** counter-driven re-open. Recommend the engine seat
score CH's action-identity latch a PASS on this evidence and, if the annotation still reaches any
latch key anywhere, strip it there too.

**MED-2 — an 80-row menu built almost entirely from identical Equip rows (O18 not applied at the
Equip seam).** Repro: `…vs-ai_baka_deck125.jsonl` **seq 186**, 81 options: row 1 the plain Create,
row 41 the `repeated then stop` Create, and **rows 42–80 are `Equip with Lightning Greaves targeting
Human #1 … Human #39`, one row per token, every one with identical effect text**. Same shape at
**seq 222** (41 options) and **seq 242** (45). O18's range collapse of identical-effect target rows
covers the Create seam but not this one; a single `Equip with Lightning Greaves targeting a Human
(39 identical targets)` row would remove ~39 rows and remove the temptation the guide now has to
forbid in words (edit 5). No decision was lost to it — the seat passed at 186 and took row 1 at 222
— but it is the largest menu this deck renders.

**MED-3 — HTTP 400 on a 448 s request, with `deadline_pct` 49.8 and `max_tokens_reasoning: 0`.**
Repro: `…vs-ai_baka_deck152.jsonl` **seq 64**, `"fallback": "http_error"`, `"http_status": 400`,
`"attempt_ms": [448870, 60]`, `"max_tokens": 400`, `"max_tokens_answer": 400`,
`"max_tokens_reasoning": 0`. This is the corpus's only `http_error` and it is a *request-level*
rejection (the retry died in 60 ms), not a timeout — the deadline was only half spent. Worth the
engine seat's eyes: a 400 that reproduces instantly on retry is a malformed request, and
`max_tokens_reasoning: 0` on a `--thinking on` run is the field I would look at first. Harm was nil
here: **seq 65** shows the heuristic recovered with `cast Bloodline Keeper`, the correct play.

**LOW-4 — pathway MDFC card scripts fail to parse their `doubleside` handle (opponent-side, deck123
runs none).** Repro, `game-146v123-1788968874.stderr` and `game-162v123-1788968843.stderr`:
`ABILITYFACTORY Error parsing: {0}:doubleside(grimclimb pathway)` (also boulderloft, mistgate,
tidechannel) and `ABILITYFACTORY ERROR: Parser returned NULL {t}:add{g}` / `{t}:add{w}` (×6). The
source lines are `bin/Res/sets/primitives/borderline.txt:14068` (Brightclimb Pathway
`anyzone={0}:doubleside(Grimclimb Pathway)`), `:13564`, `:19785`, `:52390`. The `autohand=` line on
each front face is a separate mechanism, so the O4 both-faces land-drop menu may still work; **I did
not test that** — no deck123 game had an MDFC in hand, so O4 is UNTESTED from this seat. Flagging it
because a NULL `{t}:add{w}` would mean a resolved back face taps for nothing.

**LOW-5 — `sibling_window_asks_skipped` 0 and `chain_windows_collapsed` 0, again, and again for
structural reasons in this seat.** The sibling bar: this deck's cast menus carry `Cast …` rows and
its priority menus carry `Create / Equip / Transform` rows, so a priority menu always shows an acting
row the cast menu did not — every candidate suppression is correctly refused. The chain collapse:
every chain row here costs `Tap`, which the mana policy always affords, so the no-legal-action path
is unreachable from deck123. Consistent with CH's finding that the chain instruments sit at the
PRIORITY seam while this deck's chain windows are at the CAST seam — `chain_selfharm_rows` is **0**
for all six of my games while `chain_acting_rows` is 154, so O13's self-harm share is not measurable
from here.

**No false render clause found, and several were checked hard.**
- **Every `ATTACK TOTAL` recomputed, 6 of 6 TRUE**, each confirmed against the game's actual end
  state: vs162 seq 41 (1 → 20), vs130 seq 16 (2 → 18) and seq 38 (`23 attackers … 24 damage … at
  -4`, final −4), vs126 seq 52 (3 → 19) and seq 70 (`26 attackers … 105 total … at -86`; Lord of
  Lineage 5/5 + 25 Vampires at 4/4 = 105, final −86), vs125 seq 243 (`40 attackers … 41 … at -1`;
  39 Humans + a 2/2 Doomsayer, final −1).
- **O1 ghostform gloss: 9 renders in vs146 (seq 58–64), all TRUE.** `Nadaar, Selfless Paladin got a
  ghostform (when this creature dies or is exiled its owner returns it to hand and creates a 1/1
  white flying Spirit - killing it is not removal) counter` matches Kaya the Inexorable's +1
  verbatim in effect. Wave 73's 56/56-false does not recur here.
- **O16 `(keeping 7)` rendered on the first mulligan window of all six games** — PASS.
- The `[DRAW PRICE: … you would be at 1]` clause (vs162 seq 62/63) is arithmetically exact
  (2 draws × 2 Dreams = 4 off 5).
- O19 ANNOUNCE_X: **UNTESTED** — this seat used the `repeated then stop` row throughout and no
  stderr in my six games contains an X-announcement window or an `echo_index_conflict`.
- O9 own-clock tag: **UNTESTED** — no render in this seat.
- Land-drop compliance is a clean instrument reading: across all six games there is **not one
  window where a `Play <land>` row was listed and something else was chosen.**

## 3. Which wave-73 items recur

| wave-73 item | wave-74 status |
|---|---|
| HIGH-1 carried PLAN stripped of `stop=/M=` | **CLOSED.** 20/20 carries keep the clause. |
| MED-2 stop clause only on the batch Create row | **CLOSED.** 46/46 Create rows carry it. |
| MED-3 stop never reconciled against L | **CLOSED, and it paid.** O7c fired 6×, all true, and moved the stop 40 → 43 in the game it won. |
| LOW-4 sibling bar / LOW-5 chain collapse are structural zeros | **RECUR as passes.** |
| the lone-maker attack (4 instances, 1 decisive) | **Down to 1 of 2 opportunities** — obeyed at vs130 seq 16, breached at vs126 seq 52. CHECK -1 works when it is read. |
| the edict gate (4 breaches) | **Down to 1 of 2 casts** — vs152 seq 51 at N = 2. |
| — new — | **the re-put/hold loop (HIGH-1)** and **the DRAW PRICE decision (vs162 seq 63)**. |

## 4. Guide verdict: **EDIT**

Revised guide at `wave74/deck123/strategy.txt` — **19,943 bytes** (live guide 19,900; ceiling
20,000). **The guide grows by 43 bytes, and the rule that earns them is `LETHAL IS ALREADY ON THE
BOARD` + its `[DRAW PRICE:]` companion** — the pair that answers vs162 **seq 63**, the only
decision in my six games that turned a won board into a loss. Everything else is paid for by
compression. No citations, counts or history in the guide (Amendment 332); both PLAN templates are
one physical line; the words "YOUR PLAN" appear nowhere.

| # | before → after | seq that paid for it |
|---|---|---|
| 1 | **NEW top-level block `LETHAL IS ALREADY ON THE BOARD`**, immediately under ORDER OF OPERATIONS, above every section: add the power of the creatures counted by "able to attack right now" against "Opponent life:"; at or above it the game is won and the only job left is to REACH the attack step — every window before it is a pass, cast nothing, draw nothing, tap nothing, pay no life. Cross-referenced from TIMING THE CHAIN and from P7. | **vs162 seq 62 → 63** (the loss): the seat wrote the correct plan at 62 and cast at 63 on a re-put of the same row. The old text carried this only as a half-clause in ORDER OF OPERATIONS ("lethal on the board (attack, do not build)") that a *cast* window never surfaces. +~370. |
| 2 | **NEW bullet: `ANY ROW PRINTING "[DRAW PRICE: … you would be at X]" HAS ANSWERED ITSELF`** — X is your life the moment the row resolves, paid before anything drawn can be cast; decline at X ≤ 5, decline with damage already promised on screen, decline on any board where you have lethal. | **vs162 seq 63**, where the row printed `you would be at 1` at 5 life and was taken anyway. The existing Skeins bullet said "punishes drawing" in prose; the engine now prints the number, so the rule keys on the number. +~300. |
| 3 | CHECK -1's rationale: `a tapped maker makes no token, so nothing enters, so your whole board is tapped for the rest of the game` → `nothing of yours untaps until SOME creature enters - with the maker spent that is the opponent's choice, not yours`. The rule and its override are unchanged. | **vs126 seq 52 → 53**: the seat broke CHECK -1 and the *opponent's* Wall of Omens untapped its Keeper. The old sentence was a false absolute, and a guide must not state a mechanic the game does not have; the replacement is true and argues the rule better. ~0 net. |
| 4 | THE TWO EDICTS gains one sentence: the row's own `"you gain at least N … the pick is theirs, so plan on the floor"` states a PRICE, not a reason, and a named victim in your plan is not one either. | **vs152 seq 51** — Tribute cast at N = 2 with exactly that clause on the row and a plan naming the victim it hoped for. +~150. |
| 5 | TRAGIC SLIP's cast gate quoted a string the engine never prints: `"kills 0 of the N legal targets at -1/-1"` → `"kills 0 of the N … targets at -1/-1" (the render names the type, not "legal")`, plus `cast NOTHING, whatever creature your plan already named`. `kills 0 of the N` added to the ANSWER FORMAT and P8 dead-row lists. LIGHTNING GREAVES gains `NEVER ON A TOKEN, however many equip rows the menu lists`. | **vs126 seq 42/43** (cast on `kills 0 of the 2 CREATURE targets`, then targeted a `- SURVIVES` row) for the first; **vs125 seq 186** (39 equip-a-token rows) for the second. +~200. |
| — | **Compressions paying for all of it (−~1,000):** halved the prose (not the rules) of DAMNATION, BLOCKING, MULLIGAN, WHICH FETCH, the Tutor and the edict card lines; folded the two land-drop bullets into one; dropped the STEP-0 "life-maximiser, not advice" nuance and the "never send your only creature into (your attacker dies)" line (both subsumed); dropped `Haste is not a reason to fire early` from the Greaves (duplicated verbatim in TIMING THE CHAIN); dropped `a Damnation menu takes its own plan line` (the DAMNATION section says it two paragraphs later); rewrote the stop's `THE PLAIN "[cost: Tap]" CREATE ROW, WHICH PRINTS NO STOP CLAUSE` — that clause is now FALSE, every Create row prints it. | — |

**What I deliberately did NOT change.** The SWEEPER SEEN end-step rule stands untouched even though
vs125 was won by building in the seat's own main phase against a Verdict deck: the sweeper simply
did not arrive, and one unpunished violation is not evidence. I did not weaken CHECK -1 on the
strength of vs126's rescue — being bailed out by the opponent's own creature is not a plan. And I
did not add anything about the re-put loop: that is HIGH-1, an engine defect, and the guide must not
be bent around it.

## 5. Optional proposals
**None.** Every edit is scoped to this deck's cards and this file's own wording. The two
general-looking findings belong elsewhere: HIGH-1 is an engine latch defect, MED-2 is a render
request, and neither is a strategy-writing amendment. Restating Amendment 332 or the existing
stop-format rules would be noise.

## 6. What I did NOT check
- **Opponent seat files were opened only for board context** (deck125's sweeper counts, deck162's
  Underworld Dreams, deck126's blockers). I audited no opponent decision.
- **I adjudicated no lane prediction corpus-wide.** §0 and §2 answer O7/O7b/O7c, O1, O16, the two
  zeros and the hold family **for this seat's six games only**; O19 and O9 are UNTESTED here for
  lack of any such window, and O4 is UNTESTED because no deck123 game had an MDFC in hand.
- **The 11 `hold_released_turn` events were not individually paired to their holding windows.** I
  established the CG/CH split statistically (table in HIGH-1) and confirmed no hold in this seat
  silenced a live acting row for the rest of a game, but the per-release trace is the engine seat's.
- **`async_drops` 8 were not traced.** No decision in this seat is missing a record and only one
  fell to the heuristic (the http_error), so I saw no loss; the sampling trace is the engine seat's.
- **`main_phase_windows_skipped` 2** is too small a number to cross-reference usefully from here.
- The `askreplay/` sidecar was not opened (206 reserved, 0 refused in this seat).
- I read every window carrying a Create row, every cast/attack/edict/discard/mulligan/blocker
  decision, and the `reasoning` of the deciding windows; I did not read the `reasoning` of all 109
  held windows individually.
- **The pathway card-script defect (LOW-4) was diagnosed from stderr and the primitives text only.**
  No build, no run, no git; nothing under `bin/Res`, `src/` or outside `wave74/deck123/` was touched.
