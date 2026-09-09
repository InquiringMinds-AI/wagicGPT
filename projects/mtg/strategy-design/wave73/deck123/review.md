# Wave 73 — deck 123 (Intruders of Thraben, WBU token combo) — per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-225637-final`, binary `622b10827` /
`archives/wagic-4b90ed7d7-w73step1`, `--thinking on`. Six seat files
`...ai_baka_deck123-<ptr>-vs-<opp>.jsonl`. **2 wins (vs152, vs162), 4 losses.** Three of the six
are reruns (vs130, vs125, vs126); treated as ordinary corpus games.

Seat census: **634 decisions** (146: 14, 152: 70, 162: 26, 130: 73, 125: 372, 126: 79), 100%
native reasoning, **0 fallbacks**, 0 `reply_truncated`, 0 `action_before_plan`, **0
`plan_line_missing`**, **0 `off_protocol_bytes>0`** in all six games, `ask_replays_reserved` 608 /
refused 0, `async_drops` 15, `menu_pass_no_progress` 0, `repeat_annotated_takes` 0.
Gameend sums for this seat: `hold_windows_skipped` 752 (cast 472 / priority 280),
`hold_released_turn` 33, `own_turn_windows_skipped` 3, `main_phase_windows_skipped` 99,
`reserve_decline_windows_noted` 13 / skipped 1, `sibling_window_asks_skipped` 0,
`chain_windows_collapsed` 0, and **`stop_reached_windows_skipped` 366 — every one of the corpus's
366.**

**Headline: wave-72's edit 1 worked completely.** The `stop=<N>, M=<N>` template turned a dead
engine feature live — 64 of this seat's 634 PLAN lines now parse a stop, the
`{right now: M=…, your stated stop=…}` clause rendered **49 times** (wave 72: 0), and every
stop-reached collapse in the whole corpus belongs to this seat. Wave-72 HIGH-1 is CLOSED.
Wave-72 HIGH-2 (the permanent hold latch) is closed by the wave-73 untap release: two holds were
taken over a live Create row and neither ended the deck.

**The new headline is an attack rule.** In four separate games the seat attacked with a LONE
token-maker while Intruder Alarm was out, tapping the only permanent that can untap its board.
Once in the 76-turn game that was the loss.

## 1. Game by game

**vs deck162 — WIN, turn 9 (18 / −3).** Doomsayer t6, Alarm t8 (**seq 9**, maker already out —
correct P3), **seq 11** `PLAN: stop=24, M=2; …` + `CHOICE: 2 (… x22)` — exact, first try, and the
first machine-readable stop this deck has ever stated. Attacked t10 for the kill. Clean.

**vs deck152 — WIN, turn 17 (11 / −26).** Two Tragic Slips cast on `{kills: …}` rows (**seq 19**,
**seq 36**) — the Slip section applied exactly. Alarm t10, Keeper t14, chain fired at **seq 103**
(`stop=27, M=1; this window 26`) and again on the OPPONENT'S turn at **seq 110/111**, then
**seq 124** sent the Vampire army for lethal. This is the deck's line executed as written,
including the guide's own "fire on their turn" clause. One flaw, not decisive: **seq 73** cast
Tribute to Hunger at `they control 3 creatures - they choose which one` (guide: N not exactly 1,
no edict).

**vs deck146 — LOSS, turn 10 (−4 / 25).** Blowout; 15 decisions. Land drop taken every own turn.
One violation, not decisive: **seq 29** Devour Flesh at `they control 3 creatures - they choose
which one` naming the opponent, at 9 life; they gained 3. Matchup, not guide effect.

**vs deck130 — LOSS, turn 23 (−1 / 23).** Sixteen turns of holds and passes on a hand that could
not deploy; makers finally landed t17 (killed) and t21. Two decisions:
- **seq 103** (t16) Devour Flesh at `they control 3 creatures - they choose which one` naming the
  opponent — spent the last 2 mana and a card to give them 1 life. Guide violation, MED impact.
- **seq 142** (t23) **the lone-maker attack, instance 1.** The board was
  `33 permanents … of which 29 are creatures, 1 of them able to attack right now`; the only row was
  `A1. Bloodline Keeper (3/3) [flying, …]` with `ATTACK TOTAL: 1 attacker listed, 3 total combat
  damage … they survive at 23`. The seat had just made 27 Vampires (**seq 141**). It sent the
  Keeper for 3 and tapped its only untapper. STRATEGY.

**vs deck126 — LOSS, turn 12 (0 / 28). The guide was obeyed and lost anyway.** Mulligan at
**seq 2** (1 land, `would not cover any spell in it` → ship) and keep at **seq 3** (2 lands + a
spell at keeping 6 → keep) are both exactly the file. Doomsayer t6, Alarm t8, 34 creatures by t10,
**seq 75** attacked with 25 Humans and explicitly kept a maker home (`then attack with everything
except one untapped maker`) — CHECK 4's keep-home bullet firing correctly. The deciding window is
**seq 92** (t12): 36 attackers, `ATTACK TOTAL: … 38 total combat damage … That IS lethal … At
least 32 damage lands whatever they block`, against `LOOP COMPLETE: BOTH halves of a life LOOP
(Sanguine Bond + Exquisite Blood) are on THEIR battlefield`. The seat answered `ATTACK: none`.
That is CHECK 1 B2 applied correctly — blocking triggers resolve before combat damage, so any of
their four "may gain 2" blockers closes the loop first — and the file has no better answer,
because there is none: the loop plus four untapped walls is a lock. **Variance/matchup.** One
earlier violation: **seq 31** (t8) sent the **lone Doomsayer, instance 2**, into
`They have 4 untapped creatures able to block` with `up to 8 if all block` printed on the row;
all four blocked and they went 20 → 28. CHECK 3 alone forbade it.

**vs deck125 — LOSS, turn 76 (0 / 29). The 76-turn game, and the one worth the wave.**
Deck125 is a wrath-and-counter-and-lifegain deck (Supreme Verdict ×3, Final Judgment ×2, Fall of
the Gavel ×3, Cancel ×3, Dream Fracture, Path to Exile, plus Sphinx's Revelation, two Elixirs of
Immortality and two Staff of Nin as the clock). The seat assembled the combo **three times** and
lost it twice to a sweeper, then locked its own board on the third.

- **Build 1, t40.** Keeper + Greaves + Alarm, **seq 254** `stop=46, M=2; this window x44`,
  transform at **seq 258** — `62 permanents … of which 46 are creatures`. Built in its OWN main
  phase, passed the turn. By **seq 267** (t42) the battlefield read `of which 0 are creatures`.
- **Build 2, t52.** Same again: **seq 375** `stop=56, M=2; … create 54 vampires`, 56 creatures at
  **seq 445** — where the seat attacked with **Lord of Lineage alone (lone maker, instance 3)**
  while 45 Vampires sat summoning sick. `Their graveyard` on that very screen listed
  `Supreme Verdict {1}{u}{w}{w} x3; … Final Judgment {4}{w}{w} x2; Fall of the Gavel … x3;
  Cancel … x3`. At **seq 462** (t53): `of which 0 are creatures`. The `reasoning` at **seq 371**
  reads the graveyard out loud — *"They have Supreme Verdict, Final Judgment, Sphinx's Revelation
  in graveyard… But I have a win condition with Intruder Alarm + Bloodline Keeper"* — so this is
  read-correctly-decided-wrong: **STRATEGY**, not perception.
- **Build 3, t68, and the loss.** Doomsayer + Greaves, 102 Humans made (**seq 616-645**), stop
  stated as `stop=103` off `Opponent life: 100`. Then **seq 659: the lone-maker attack, instance 4
  and the game.** The only attackable creature was the hasted Doomsayer; the seat sent it for 2.
  Its own `reasoning` states the mechanism before overriding it: *"they won't untap next turn
  unless I make a token"*. From that moment the Doomsayer is tapped, and with Intruder Alarm out
  nothing untaps unless a creature ENTERS, which only an untapped maker causes. At **seq 686**
  (t70) the 102 Humans attacked — `ATTACK TOTAL: 102 attackers … puts them at 6. That is NOT
  lethal` — because the opponent's life had run 100 → 98 → **108** while the stop stayed at 103.
  After that swing every creature it owned was tapped forever: no attack window, no token, no
  untap. Opponent life went 6 → 11 → 29 while two Staff of Nin pinged the seat from 8 to 0 over
  six more turns. Two failures compose exactly: **the stop was one lifegain tick short (MED)** and
  **the maker had been spent as an attacker (HIGH)**.
- Also here: **seq 705** cast Devour Flesh at `they control 0 creatures - at 0 this does nothing`,
  naming the opponent — the row states its own uselessness. Fourth edict violation.

## 2. Engine / interface / card items

**HIGH-1 — the carried PLAN is stripped of the `stop=/M=` clause the engine itself demands and
parses.** The plan carry is served from `mPlanStepsDone` onward (`planStepsAfter`,
`include/GptPlanCaveat.h:972`), and `planStepEnds` (:920) opens a new step at any `;` followed by
whitespace. The guide's mandated template — the one the repeat row's own bracket asks for,
`"stop=<N>; M=<N>"` — therefore makes the counts clause **step one**, which the executed action
consumes. Repro pairs (written → carried):
- `...vs-ai_baka_deck130.jsonl` **seq 141** wrote `PLAN: stop=29; M=2; this window create 27
  vampires with Bloodline Keeper, then attack next turn.` → **seq 142** rendered
  `THE PLAN YOU LAST STATED … reads "M=2; this window create 27 vampires with Bloodline Keeper,
  then attack next turn."`
- `...vs-ai_baka_deck125.jsonl` **seq 251** wrote `PLAN: stop=46, M=1; this window tap Bloodline
  Keeper x45, …` → **seq 252** carried `"this window tap Bloodline Keeper x45, …"`.
50 carries in this seat lost their counts this way. The measurable harm is stop instability with
no screen memory to correct it: inside turn 52 of vs125 the stated stop went 56 → 46 → 51 → 106 →
51 (**seq 375, 380, 459, 460, 461**), and vs125 **seq 256** states `stop=23` with `M=45`. The
engine already persists the answer (`mStatedStop`, `AIPlayerGPT.cpp:20404`), so the cheap fix is
to re-prepend the persisted stop/count to the carry header; the structural fix is for
`planStepEnds` not to open a step on a clause that is `stop=`/`M=` state rather than an action.
**Do not fix this in the guide** — moving the counts off the front would fight the row's own
printed contract, which is the mistake wave-72 HIGH-1 diagnosed in the other direction.

**MED-2 — the stop clause prints only on the batch row, so the single `[cost: Tap]` Create row is
an unpriced way past the seat's own stop.** Repro: `...vs-ai_baka_deck162.jsonl` **seq 16**, where
row 2 carries `{right now: M=25, your stated stop=24, so this window would add to a count ALREADY
AT OR PAST your own stop - past your stop = a wasted window}` and row 1 carries nothing; the model
took row 1 and wrote a fresh stop to justify it (`PLAN: Create one token … to reach the stop of
26`). Same shape at vs125 **seq 624**. Printing the clause on every Create row of the seam, not
just the counted one, closes it. (The guide side is patched anyway — edit 5 below.)

**MED-3 — `Opponent life trend` is rendered but the stop clause is not reconciled against it.**
vs125 **seq 686** prints `Opponent life trend: turn 68: 100, turn 69: 98, turn 70: 108, now 108
(+8 since turn 68)` in the same prompt as a 102-attacker total. Nothing on screen relates the
seat's stated stop (103, set at t68) to a life total that has since moved past it; the
`{right now: M=…, your stated stop=…}` clause compares M to the stop, never the stop to L. A
`your stated stop was set when their life was N; it is now L` note on the repeat row would have
been decisive here.

**LOW-4 — `sibling_window_asks_skipped` 0 is a PASS for this seat, not a dead bar.** Twelve holds
were followed inside the same turn+phase by a window of the other kind (vs130 **seq 137→138**;
vs125 **seq 391→392, 443→444, 621→624, 625→628, 637→638, 644→645, 646→658, 647→658, 663→664**;
vs126 **seq 107→108, 135→136**). All twelve were refused by `w72HeldMenuShowedEveryRow`, correctly:
this deck's cast menus carry `Cast …` rows and its priority menus carry `Create / Equip /
Transform` rows, so the priority menu always shows an acting row the cast menu never did.
vs125 **seq 621 → 624** is the ideal trace — held on the cast ask, re-asked on the priority window,
took the Create row. No unseen acting row was ever suppressed.

**LOW-5 — `chain_windows_collapsed` 0 is structural for this deck.** The collapse needs the seat's
own mana policy to find no legal action on a running chain; every chain row here costs `Tap`, which
the policy always affords, so the path is unreachable from deck123. Not evidence either way.

**LOW-6 — `main_phase_windows_skipped` does not over-count in this seat.** 99 skips over six games
and ~150 own turns (vs125 alone: 65 over 77 turns) — under one per own main phase, as designed.

**No false render clause found.** Everything I checked against the opponent seat or the primitives
was true: `LOOP COMPLETE` (vs126 seq 92), the `up to 8 if all block` blocking-trigger price (vs126
seq 31, and the opponent did gain exactly 8), `no creature they control can block this attacker`
(vs125 seq 686, opponent board `of which 0 are creatures`), the summoning-sick exclusion list
(vs125 seq 659, 102 names), `ATTACK TOTAL` arithmetic in every window I recomputed, and the
`hold check` annotations. **Card facts verified** against `bin/Res/sets/primitives/`: Thraben
Doomsayer, Bloodline Keeper / Lord of Lineage (transform `{b}`, +2/+2 to other Vampires),
Intruder Alarm (symmetric, untap-all on any creature entering), Lightning Greaves (haste + shroud,
equip {0}), Tragic Slip, Devour Flesh, Tribute to Hunger, Damnation, Vision Skeins. **No
card-script defect found in this seat.**

## 3. Which wave-72 items recur

| wave-72 item | wave-73 status |
|---|---|
| HIGH-1 stop collapse unreachable | **CLOSED.** 64 parseable PLAN lines, 49 stop-clause renders, 366 collapses — all of the corpus's. |
| HIGH-2 hold over a Create row is permanent | **CLOSED by the engine's untap release.** Two such holds (vs125 seq 261, 391) and neither silenced the deck. |
| MED-3 hold scope text is framed as reassurance | Not re-tested; no hold in this seat turned out to be the wrong call. |
| MED-4 carried plan is a truncated fragment | **RECURS, and now with a mechanism** — see HIGH-1. The truncation is the step pointer, not a length cut. |
| LOW-5 new render clauses were all true | **RECURS as a pass** — still no false render. |
| wave-71 edit 1 (Alarm ordering bound) | No P3 violation this wave. |

## 4. Guide verdict: **EDIT**

Revised guide at `wave73/deck123/strategy.txt` — **19,909 bytes** (live guide 19,968; ceiling
20,000). Five changes; the file SHRINKS by 59 bytes, so nothing needs to earn extra bytes. No
citations, counts or history in the guide (Amendment 332); both PLAN templates are one physical
line; the words "YOUR PLAN" appear nowhere.

| # | before → after | seq that paid for it |
|---|---|---|
| 1 | **NEW `CHECK -1 - THE LONE MAKER`, ahead of every other attack line**: one creature listed and it is a maker → `ATTACK: none`, whatever the total or the carried plan says, because attacking taps the only thing that makes a creature enter and only a creature entering untaps the board; sole override is a row the engine itself calls lethal. | **vs125 seq 659** (the loss), **vs130 seq 142**, **vs125 seq 445**, **vs126 seq 31**. Four instances, one decisive. CHECK 4's keep-home bullet *works* when the list is long (vs126 **seq 75** names it in the PLAN) and cannot bite when the list has length one — with one attacker "send everything" and "keep one home" are the same sentence, so the bound has to sit above the checks, not inside CHECK 4. +~510. |
| 2 | TIMING THE CHAIN: `Your OWN upkeep is the worst window against a log showing Damnation, Supreme Verdict or Starstorm: pass there.` → a `SWEEPER SEEN` gate naming their log, graveyard **or hand**, stating that an army built in your own main phase sits summoning sick through their whole turn and is swept before it ever attacks, that the fix is to fire at THEIR END STEP, and that the Greaves' haste unlocks the maker and not the tokens. | **vs125 seq 371** (reasoning reads Supreme Verdict + Final Judgment off the screen and builds anyway), and the two boards it cost: **seq 258 → 267** (46 creatures → 0) and **seq 375 → 462** (56 → 0). The old line named only the UPKEEP and only three cards; the failure was in main phase 1 against a graveyard the model had already read aloud. +~330. |
| 3 | THE STOP: `Against a lifegain board add their last gain to the stop a second time` → add their **LARGEST single gain so far**, with the reason attached — an overshoot costs nothing while the maker is untapped, finishing one short leaves them alive with your board tapped. | **vs125 seq 620** (`stop=103` off `Opponent life: 100`) and **seq 686** (`… puts them at 6. That is NOT lethal`, life trend `+8 since turn 68`). "Last gain" was 2; the gain that mattered was 8. +~160. |
| 4 | THE TWO EDICTS: `N not exactly 1: no edict.` → `N NOT EXACTLY 1: NO EDICT AT THE OPPONENT` — not at 0 where the row says it does nothing, not at 2+ where they bin their worst and take the life; at any N but 1 the only use is naming Yourself. | **vs146 seq 29**, **vs152 seq 73**, **vs130 seq 103**, **vs125 seq 705** — four casts at N of 0, 3, 3 and 3, all naming the opponent, all against a printed tag. The bare gate was being overridden by the "naming Yourself" paragraph below it, which is the one legitimate N≠1 use; scoping the gate to the opponent removes the contradiction. +~140 net. |
| 5 | THE STOP: the plain `[cost: Tap]` row clause gains `taking that row is not a way past your own stop, and inventing a bigger stop to fit it is not either`; the hold rule loses `PERMANENT … this game` for `it stands until your untap` (the wave-73 engine now releases it). | **vs162 seq 16** and **vs125 seq 624** for the first; the untap-release change for the second, which made the old sentence FALSE — a guide must not state a mechanic the engine no longer has. ~0 net. |
| — | **Compressions paying for all of it (−~1,240):** deleted the `"THAT WOULD KILL YOU"` section and the MENACE line (neither header has rendered in 12 games across two waves); dropped the `ANSWER: CHOICE:` bullet (this seat has now recorded 0 `off_protocol_bytes` and 0 `plan_line_missing` in twelve games); dropped the catalogue of failing `stop=` spellings, because the repeat row now prints the required shape verbatim on screen; halved the prose (not the rules) of BLOCKING, DAMNATION, the Tutor, WHICH FETCH, the Greaves, Vision Skeins, MULLIGAN and the sacrifice ask. | — |

## 5. Optional proposals
**None.** Every edit is scoped to this deck's cards and this file's own wording, and the two
general-looking findings belong elsewhere: HIGH-1 is an engine defect in the plan carry (the guide
must NOT be bent around it), and MED-3 is a render request. Neither is a strategy-writing
amendment, and restating Amendment 332 or the existing stop-format rules would be noise.

## 6. What I did NOT check
- Opponent seat files were opened only for board context (deck125's sweeper counts, deck126's loop
  halves); I audited no opponent decision.
- I adjudicated no lane prediction corpus-wide. §2 LOW-4/LOW-5 answer the two zeros **for this
  seat's six games only**; whether the sibling bar and the chain collapse are inert elsewhere is
  the engine seat's call.
- **`hold_released_turn` (33 here) was not individually traced.** I confirmed the release rule from
  source (`w73HoldExpiredByUntap`, `AIPlayerGPT.cpp:27966`) and confirmed no hold in this seat
  silenced a live acting row for the rest of a game, but I did not pair each of the 33 releases to
  its holding window — the brief assigns that trace to the engine seat.
- I read every window carrying a Create row, every cast/attack/edict/discard/mulligan decision, and
  the `reasoning` of the deciding windows; I did not read the `reasoning` of all 752 held windows
  individually (I scanned all 59 taken holds programmatically for a live maker, Alarm, Tutor
  bracket or `{kills:}` row).
- The `askreplay/` sidecar was not opened: 608 reserved, 0 refused.
- My reading of `AIPlayerGPT.cpp` / `GptPlanCaveat.h` is static. No build, no run, no git; nothing
  under `bin/Res`, `src/` or outside `wave73/deck123/` was touched.
