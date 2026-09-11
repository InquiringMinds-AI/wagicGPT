# Wave 77 — deck 146 (Orzhov dungeon midrange) per-deck review

Seat files: `matchups-20260911-010157-final/*ai_baka_deck146-0x*-vs-*.jsonl` (6 games, single binary
`wagic-42c41fc66-w77step1`). All six are RERUNS after the pilot wedge (the first-run game is
126v162, not mine). Live guide reviewed = the wave-76 edition (19,959 B).

Seat census: **489 model decisions** (ask 344, priority 103, attackers 36, reveal 5, bottom 1,
**blockers 0, discard 0**), **0 fallbacks** — 0 `unparsed_reply`, 0 `noop_row_zero_reask`, 0
`repeat_count_reask`, 0 `defer`, 0 `recovery` records on this seat, so none of the six corpus-wide
fallbacks is mine and there is nothing to quote verbatim here. 0 `reply_truncated`, 0
`action_before_plan`, `transport curl=0,http=200,empty=0` on 489/489, `thinking` on and native
reasoning on 489/489 (median 5,613 chars, max 20,471). Latency p50 80.2 s, p95 169.6 s, max 440.6 s;
`deadline_pct` p50 8.9, max 48.2. `off_protocol_bytes>0` 44/489 (9.0 %), `plan_line_missing` 65/489
(13.3 %) and `protocol_deviation_replies` 65 — the two are equal to the record, the dropped label
again. `reasoning_ngram_repeat` median 0.131, max 0.443. Gameend sums for this seat:
`ask_replays_reserved` 73, `identical_ask_answers_reserved` 50, `identical_option_asks_resolved` 7,
`hold_released_turn` 12, `hold_windows_skipped` 161 (cast 94 / priority 67),
`crossphase_identical_reputs` 8 / `crossphase_board_unchanged` **0**, `main_phase_windows_skipped`
32, `own_turn_windows_skipped` 103, `phase2_answer_recovered` 1 / missing 0,
`forced_close_unrecorded` 1, `async_drops` 14, `blocker_forecast_rows` 46 (multi 36, gang 2),
`plan_names_uncastable_zone_card` **0** (wave 76: 11), `plan_names_stranded_card` 0,
`own_loop_windows_asked` 0, `declined_face_latches` 0, `wall_miss_events` 0,
`stop_reached_windows_skipped` 0, `menu_pass_no_progress` 0. Max prompt 31,701 B (`146v125` seq
201); 176 prompts over 20 KB.

| # | Opponent | Result | Turn | Final life | What decided it |
|---|---|---|---|---|---|
| 1 | 123 | **WIN** | 21 | 22 / 0 | Silencer named the two cards that beat the seat; Command's draw mode pointed at THEM for the last point |
| 2 | 125 | **WIN** | 35 | 14 / -2 | the MANLAND: every spell was countered or swept, Hive was not |
| 3 | 126 | **WIN** | 21 | 21 / -3 | Verse on Exquisite Blood before the pair completed; Spiders + Adventurer through three 0/4 walls |
| 4 | 130 | **WIN** | 23 | 5 / -6 | Kaya -3 on Siege-Gang Commander, then dungeon drains; finished at 5 life |
| 5 | 152 | **LOSS** | 18 | -14 / 16 | **seq 18** — Kaya's -3 spent on a 2/2 token; she died at seq 33 and the recast Sigarda killed the seat |
| 6 | 162 | **WIN** | 11 | 11 / 0 | four venture bodies on curve, Kaya -3 on Fate Unraveler, raced the draw-punisher before it set up |

**5-1** (wave 76: 2-4). Hands are real; the 152 loss is the only game with a guide item in it, and
the same guide gap fired harmlessly in two of the wins.

## 0. Wave-76 items: which recur, and what the wave-77 lanes closed here

- **R2a/R2b — CLOSED on this seat, and wave-76's HIGH 1 does NOT recur.** 358 `[hold check: ...]`
  brackets. **18 print "this is the first window I have asked you at this seam"** — exactly 3 per
  game, one each for `land_drop`, the casting seam and `empty_stack_pass`, and every one of the 18 is
  genuinely the first record of that `log_window_kind` in its file (0 follow an asked predecessor).
  The land drop has its own seam key now (`log_window_kind: land_drop`), which is R2a. Of the
  remaining 340 brackets — 186 "every row above was also on the menu", 154 counts — **0 are
  confirmed false** once the pass row that differs only by naming the next step is folded (the
  render says it folds it); my key found 4 residual disagreements and 3 of those are Verse rows
  whose target text changed inside the row, which the engine folds and I do not. Wave 76 had 11
  false claims in 43 at streak 1; this wave has none at any streak.
- **R2's `hold_check_ref_seq` — present on every asked record, but NOT joinable from a seat log.**
  See MED 2. This is the one lane deliverable a per-deck seat cannot use as instructed.
- **R5 (creature `{effect:}` gloss) — PASS on this seat.** 241 prompts carry a `{effect:}` clause;
  26 distinct creature glosses, **4 truncated and all four end in an explicit `...`**; none of the
  26 is cut into a false statement (the two nearest misses are honest: `Brutal Cathar ... exile
  target creature an opponent controls until this...` and `Acererak ... return Acererak the...`).
  Prompt cost: max 31,701 B (wave 76: 30,375), 176 of 489 prompts over 20 KB.
- **R9 — CLOSED here, possibly over-corrected.** `plan_names_uncastable_zone_card` is **0** across
  all six games (wave 76: 11, of which ~0 were real). No false positives left to cite; whether the
  predicate can still fire on a real case is not testable from this seat.
- **R8 — still inert here.** 8 `crossphase_identical_reputs`, 7 brackets rendered, and
  `crossphase_board_unchanged` is **0**: the positive clause never printed on this seat for the
  second wave running.
- **R1 / R10 / R6 — UNTESTED here.** 0 `LOOP COMPLETE`, 0 `[own loop verdict:]`, 0 ANNOUNCE_X
  windows, 1 `{crack-back cover:}` render in 489 decisions.
- **Wave-76 EDIT 1 (THE COUNTER TEST) — FIRED ONCE, FAILED ONCE, and the failure is the reason for
  Edit 1 below.** Both halves were testable only in `146v125` (the sole opponent with counterspells).
  FAIL at seq 15; PASS at seq 120. See HIGH 2.
- **Wave-76 EDIT 2 (Soul Shatter at MV 0) — UNTESTED.** 0 rows print `MV 0` or `tied at MV 0` this
  corpus; 128 prompts carry `at 0 this does nothing` and the seat correctly never cast into one.
- **Wave-75 EDIT 2 (legend-rule reset) — FIRED CORRECTLY again.** `146v130` seq 56 took the
  `[legendary: you already control Kaya the Inexorable ...]` row with the battlefield copy at
  **loyalty 1**, and seq 57 binned `Kaya the Inexorable #1 ... [loyalty 1]`. Second clean live test.
- **Clean:** 0 `http_error_body`, 0 non-200, 0 `plan_names_stranded_card`, 0 `wall_miss_events`,
  1 `phase2_answer_recovered` / 0 missing.
- **BLOCK LADDER: 0 blockers decisions for the THIRD wave. CLEANUP DISCARD: 0 windows for the
  FOURTH.** Both untested, neither cut.

## 1. Game by game — the deciding decisions

**vs 152 (LOSS, T18) — decided at `...vs-ai_baka_deck152.jsonl` seq 18 (T9 Main 1), a Kaya -3 spent
on a 2/2 token.** Board: 20-20, Kaya has just resolved at **5 loyalty**, the seat controls no
creature, and the opponent's whole board is `Wolf (2/2)` plus three lands. The crack-back line reads
`1 of their creatures will be able to attack ... for up to 2 - you would be at 18`. The menu offered
`1. +1: don't target any creature`, `2. -3 ... targeting Kaya the Inexorable [your battlefield]`,
`3. -3 ... targeting Wolf [opponent's battlefield]`. The seat took row 3; reasoning:
> *"Check Kaya: -3 on Wolf is correct."*

and nothing else — because the guide says so. `PLANESWALKERS` opens `USE THEM THE TURN THEY LAND AND
EVERY TURN AFTER; an idle walker is wasted`, cast-order entry 2 says `Cast her NOW, AND USE A LOYALTY
ABILITY THE SAME TURN`, and the KAYA bullet's target rule was `THE TARGET IS THE BODY HITTING YOU ...
take the name the CRACK-BACK line prices` — which on that screen is the Wolf. The only escape,
`Take the +1 when no name on the -3 list is worth it`, states no test. Kaya went 5 -> 2.
At seq 26 she +1'd to 3; at seq 33 (T13) she -3'd `Sigarda, Champion of Light` and **died at 0**.
T14 the opponent simply recast Sigarda; T16 Sigarda (6/6 flying trample) hit for 6 and Luminarch
Aspirant for 4, seat 12 -> 2; T18 dead. Had seq 18 been the +1, Kaya sits at 7 before the T13 -3,
survives at 4, and answers the second Sigarda — the 11 damage that killed the seat is exactly the
two Sigarda connections. **STRATEGY item, Edit 2 below.**

Two more of the same shape in the wins, neither fatal but both the same rule:
- `146v130` **seq 61** (T21, 10-12): Kaya at **5 loyalty**, `-3 ... targeting Goblin [opponent's
  battlefield]` — a **tapped 1/1 token**, their only creature, against a seat board of Spirit Cai,
  a 2/2 first-strike deathtouch Adventurer and Barrowin 3/3. Loyalty 5 -> 2.
- `146v126` **seq 38** (T13, 20-16): Kaya at **5 loyalty**, `-3 ... targeting Perimeter Captain #1
  [opponent's battlefield]` — one of **three** 0/4 `[defender]` bodies, with the opponent line
  reading `3 are creatures, 0 of them without a restriction against attacking`. The guide's
  `never target a "[defender]", a power of 0` clause is guarded by `While an attacker of theirs is on
  the list` — with no attacker anywhere on their board the guard is false and the clause is inert,
  so the guide licensed exiling one of three identical walls. This is A334's failure mode exactly
  (the hard case carried in a conditional clause), and it is why Edit 2 states the ordering flat.

**vs 125 (WIN, T35) — the counter test failed at seq 15 and the seat won anyway, on the manland.**
At seq 15 (T9 Main 1) the prompt's own log reads `Opponent cast Cancel` / `Your Pelakka Predation was
COUNTERED by Cancel`, `Their graveyard (2 cards): Path to Exile {w}; Cancel {1}{u}{u}`, and
`Their untapped sources: 4`. Both conditions of the wave-76 COUNTER TEST held. The seat cast Lolth
into it and `Your Lolth, Spider Queen was COUNTERED by Dream Fracture`. The reasoning never mentions
the test — it reasons entirely from the carried plan (*"Last plan: ... cast Lolth Spider Queen ...
The plan is solid"*). The test was unreachable from where the decision was made: it lived under
`WHEN THEY COUNTER AND GAIN LIFE`, and cast-order entry 2 only pointed at it. **Edit 1 moves it to
the seam.** The second half of the test PASSED at seq 120 (T23, 14 mana): Lolth + Silencer + Acererak
all in one turn, which is the "leaves enough sources for a second row you also take" shape.
The game was then won by `Hive of the Eye Tyrant` alone against three Supreme Verdicts, two Fall of
the Gavel, two Essence Scatter, two Path to Exile and two Staffs of Nin — a manland a sweeper cannot
catch.

**vs 123 (WIN, T21) — clean, and a guide rule won it.** Silencer named `Damnation` (seq 16) then the
Predation reveal took `Intruder Alarm` (seq 11), Verse (seq 19) and Kaya `-3` on `Intruder Alarm`
(seq 33) answered the engine; at T21 with the opponent on 1, seq 48 took Silverquill Command's
`return creature and opponent draws {this mode right now: they LOSE 1 life - they would be at 0}` —
the guide's `DRAW DIRECTION ... at the opponent only when that 1 life finishes them`, fired exactly.

**vs 126 (WIN, T21) and vs 162 (WIN, T11) — clean.** 126: Verse on `Exquisite Blood` (seqs 59-60)
before the Bond pair completed, Lolth `-3` the turn she landed, Spiders (reach + menace) through
three 0/4 walls. 162: four venture bodies on curve and Kaya `-3` on `Fate Unraveler` at seq 27; the
draw-punisher never assembled.

**vs 130 (WIN, T23, finished at 5 life) — two deviations from the guide's own printed rules, both
survivable.**
- **seq 75** (T23, **8 life vs 8 life**): the three-dungeon list appeared after a completion and the
  seat answered **TOMB OF ANNIHILATION**, which the guide allows `only when racing and clearly ahead
  on life`. It then paid 1 life entering and, at seq 77, took `lose 2 life {this mode right now: you
  LOSE 2 life - you would be at 5}` over discarding. Three life for one venture step in a game that
  ended at 5. `clearly ahead on life` is not a printed number, so the rule could not refuse the row;
  Edit 3 makes it one.
- **seq 42 and seq 38 in `146v152`** (both Upkeep): the seat animated the Hive at UPKEEP with
  CHECK 0 not firing, which the guide's `CHECK 1 - "Phase:" says Upkeep and CHECK 0 did not fire ->
  PASS` forbids. At T17 that spent 4 of 8 sources and left **4** — Kaya, drawn that same draw step,
  needed 5 and seq 43 answered `Cast nothing right now`. A compliance failure against a rule that is
  already correct and already printed; no edit.
- **seq 63** (T21): paid 3 life for Emeria at 10 life. Condition (a) of the 3-life rule was
  unreadable — the only hand card printed `[no cast row now: you already answered this phase's
  Casting decision]`, not `[cannot pay now: needs N mana ...]`. The seat paid anyway and the mana DID
  buy `Cast Pelakka Predation` at seq 64, so the principle (`Life you pay buys a cast THIS window`)
  held while the written test did not. Edit 4 restates (a) in terms the render supports.

## 2. Engine / interface / card items

### MED 1 — `crossphase_board_unchanged` is 0 on this seat for the second consecutive wave: R8's positive clause still never prints
Repro `146v125` seq 70 (and 72, 219, 228, 243; `146v126` seq 56): the bracket
`[this exact list was put to you ...]` renders 7 times against `crossphase_identical_reputs` 8, and
the string `nothing on the board has changed` renders **0** times in 489 prompts. Wave 76 measured
2 of 15 here; wave 77's ask-cache board key has not moved the number off the floor on this seat.
Either the 7 boards really did all move, or the key is still too strict — not decidable from a seat
log, but the "0 of 7" shape is the same one CQ F4 flagged.

### MED 2 — `hold_check_ref_seq` cannot be joined to a record from a seat log
The brief instructs per-deck seats to use it; it is on all 358 bracketed records, and it is not a
record `seq`. Repro `146v152` seq 36 carries `hold_check_ref_seq 29` while the file's record seqs run
0-48 with 22/32/34/35/44 absent (those five are the game's five `ask_replays_reserved` windows, which
are counted but not written). Read as a 0-based ordinal over the file's decision records it is right
~80 % of the time (`146v152` 27/29, `146v123` 23/32, `146v162` 6/9) and off by 1-3 otherwise, because
the ordinal it counts includes windows the translog does not carry and its seam key is finer than
`log_window_kind` (`146v126` seq 47 `ref 38` lands on seq 46, a dungeon-room sub-menu, while the true
predecessor at that seam is seq 44). Fix direction: emit the referenced record's `seq` (or -1), not
an internal window ordinal; the whole point of the field was to let a seat adjudicate the bracket
without re-implementing the key, and it cannot.

### MED 3 — the `-3` menu prints the loyalty cost nowhere on the row
Repro `146v152` seq 18, `146v130` seq 61, `146v126` seq 38: every `-3: exile non-land permanent with
Kaya the Inexorable targeting X` row is tagged only `[cost: Counters]`, with the number of counters
it spends and the loyalty that survives nowhere on the row. The battlefield line does carry
`Kaya the Inexorable {3}{b}{w} [planeswalker] [counters: 5x loyalty]`, so the arithmetic is on the
screen but split across two places, and in all three records the reasoning never performs it. The
Hive animation row by contrast prints a full `{rung ceiling: ...}` and a `{death price: ...}`.
A `{spends 3 of 5 loyalty; she is at 2 after this, and dies at 0}` clause on each loyalty row would
put the whole decision on one line. This is the render half of Edit 2.

### LOW 4 — `146v126` `forced_close_unrecorded` 1 with every force-close counter at 0
`force_close_arms_refused` / `_deferred` / `_same_arm_deferred` / `_defer_bound_hits` are all 0 for
all six games and one close still went unrecorded in the 126 game. Not identifiable from the seat
file (no record carries the arm); the engine seat owns naming it among the corpus-wide 10.

### LOW 5 — latency is concentrated in the 125 game's priority chains
`146v125` is 215 decisions to 35 turns (the other five average 55); T29-T35 alone is ~45 windows of
`Cast Soul Shatter {right now: they control 0 creatures ... at 0 this does nothing}` / `Cast Vanishing
Verse - the only legal targets are YOUR OWN right now` / hold / decline, every one of which the seat
correctly answered `Cast nothing`. At p50 80 s that is ~1 h of decode for a board that could not
change. As last wave: no single link is collapsible (the Hive animation and the attack move it), but
this is where the seat's wall-clock lives.

## 3. Guide verdict: EDIT

Live 19,959 B -> revised **19,998 B** (`wave77/deck146/strategy.txt`). Four rules changed. The
guide grew by 39 B net; the bytes are bought from compression listed under Edit 5, and the rule that
earned them is Edit 2 (Kaya's -3 ladder), which is the only guide gap that decided a game.

**Edit 1 — the counter test moves to the seam where the cast is chosen (A333).**
- before, cast-order entry 2: `LOLTH or KAYA if listed - a row tagged "[legendary: ...]" only under
  the loyalty test in PLANESWALKERS, and only past the counter test in WHEN THEY COUNTER. Cast her
  NOW, ...`, with the test itself living under `WHEN THEY COUNTER AND GAIN LIFE`.
- after, entry 2 carries the test: `THE COUNTER TEST FIRST, off two printed facts on this screen:
  their graveyard line or the log names a card of theirs that counters spells, AND "Their untapped
  sources" reads 2 or more -> SKIP THIS ENTRY THIS WINDOW; take the cheapest entries below that leave
  sources up, and take her on a later menu whose row leaves sources for a second row you also take
  that turn.` The `WHEN THEY COUNTER` section keeps the Elixir/Staff artifact rule and now points at
  entry 2 instead of restating the test.
- paid for by **`146v125` seq 15** — `Cancel` in their graveyard on that screen, `Their untapped
  sources: 4`, Lolth cast into it and countered by Dream Fracture, with the reasoning citing only the
  carried plan. The heading `WHEN THEY COUNTER AND GAIN LIFE` scopes to a matchup, not to a decision
  seam, so the rule was never read at the window it has to fire in.

**Edit 2 — Kaya's -3 gets a loyalty floor, stated flat (A334), and the defender clause loses its
guard. THE RULE THAT EARNED THE BYTES.**
- before: `Land her and -3 at once when behind. A PLANESWALKER OF THEIRS ON THE -3 LIST GOES FIRST
  ... Otherwise THE TARGET IS THE BODY HITTING YOU ... take the name the CRACK-BACK line prices ...
  While an attacker of theirs is on the list, never target a "[defender]", a power of 0, a lord or
  an anthem.`
- after: `SHE ENTERS AT 5 AND DIES AT 0: each -3 spends three of her five counters and can cost the
  second -3 this deck otherwise never gets. THE -3 LADDER, top first, stopping at the first name the
  list offers: a PLANESWALKER of theirs ...; a noncreature permanent of theirs making cards, damage
  or life every turn; the body the CRACK-BACK line prices at 4 OR MORE ... EVERY OTHER BOARD IS THE
  +1, a board naming nothing on that ladder included: a "[defender]", a power of 0, a lord, an
  anthem, a token, a body priced at 3 or less. She keeps the counters and takes a real name the turn
  one lands. ONE LINE OUTRANKS THAT: the crack-back line says the attack KILLS you and exiling a
  listed body stops it - then -3 it at any size.` And in PLANESWALKERS: `KAYA: USING HER IS NOT
  SPENDING HER - her +1 is a USE. Her -3 runs the ladder in SPELLS.`
- every condition is a printed number on the same screen: the crack-back `for up to N`, the
  `[counters: Nx loyalty]` line, the `[defender]` / `(0/4)` tags, the word `token` on the row.
- paid for by **`146v152` seq 18** (5 loyalty spent on a 2/2 token priced at 2, Kaya dead at seq 33,
  the recast Sigarda then dealing 11 of the 18 damage that killed the seat), **`146v130` seq 61**
  (5 loyalty on a tapped 1/1 Goblin token) and **`146v126` seq 38** (5 loyalty on one of three 0/4
  `[defender]` bodies with `0 of them without a restriction against attacking` on the same line —
  the guarded clause that could not refuse it). The `USING HER IS NOT SPENDING HER` line is aimed at
  the heading `USE THEM THE TURN THEY LAND AND EVERY TURN AFTER; an idle walker is wasted`, which is
  what the seq 18 reasoning was obeying.

**Edit 3 — the Tomb of Annihilation gate becomes a number.**
- before: `TOMB OF ANNIHILATION only when racing and clearly ahead on life`
- after: `TOMB OF ANNIHILATION only while your printed life is 5 or more ABOVE theirs`
- paid for by **`146v130` seq 75** (Tomb taken at 8 life against 8) and **seq 77** (`lose 2 life ...
  you would be at 5`), in a game that finished at 5 life.

**Edit 4 — the 3-life menu's condition (a) is restated in text the render actually prints.**
- before: `(a) a card in hand prints "[cannot pay now: needs N mana ...]" with N no higher than your
  untapped source count PLUS ONE, and you cast it in this window`
- after: `(a) a card in hand needs exactly one more mana than your untapped sources make, and a
  casting window this turn can still take it`
- paid for by **`146v130` seq 63**, where the only hand card printed `[no cast row now: you already
  answered this phase's Casting decision]` — condition (a) was unreadable, the seat paid 3 life
  anyway, and the mana correctly bought `Cast Pelakka Predation` one window later at seq 64.

**Edit 5 — the bytes, and one A334 audit fix.** No rule deleted. Compressed without losing a
condition: the deck header's inventory clause, the Verse colour sentence's tail, Soul Shatter's
target sentence, the Command live/dead bullet, Pelakka's reveal sentence, the Silencer `{visible
now:}` sentence, Agadeem's explanation, the pathway and land-backed paragraphs, Lolth's Spiders
sentence, the mulligan carried-plan bullet (its three ship prohibitions are kept verbatim), and the
`WHEN THEY COUNTER` section after Edit 1 removed its duplicate. Also, per A334's audit clause, the
one pre-existing `even when` in the guide: `CHECK A ... whatever its tag says and even when empty`
-> `whatever its tag says, an empty tag included`. Finally the hold paragraph now names the third
verdict the engine prints: `"N rows above are new" or "first window I have asked you at this seam"
-> decline this window only` (the sentence renders 18 times on this seat and the guide had no
instruction for it).

## 4. What I did NOT check
- **`146v125`'s T24-T35 tail decision by decision** (~60 windows with nothing castable and no legal
  Verse target). I read enough to establish the board was static and the Hive was the whole clock.
- **Opponent seats** beyond board and log context; no audit of any opponent deck's play.
- **Whether a different T9 line beats deck152** — I checked only that the +1 at seq 18 leaves Kaya
  alive through both Sigardas. I did not simulate the alternative game.
- **`main_phase_windows_skipped` 32, `own_turn_windows_skipped` 103, `hold_windows_skipped` 161,
  `mana_only_windows_skipped` 0** — not confirmable from a seat file.
- **`forced_close_unrecorded` 1** — no record carries the arm; engine seat's call.
- **R1 (own-loop), R10 (ANNOUNCE_X plan echo), R6 (crack-back cover on removal/X/attacker rows),
  R7 (legality-filtered blocker counts), R3 (life-gain-on-sacrifice), R4 (blockers AT BEST)** — 0 to
  1 renders each in 489 decisions on this seat. UNTESTED here, not passed.
- **BLOCK LADDER (0 blockers decisions, third wave) and CLEANUP DISCARD (0 windows, fourth wave)** —
  untested, neither cut.
- **My hold-check seam reconstruction is mine, not the engine's.** I keyed the seam off
  `log_window_kind`, stripped balanced brace/bracket groups, and folded the pass row's
  "(<step> comes next this turn)" parenthetical because the render says it folds it. The 18
  first-window claims are checked directly against each file and do not depend on that key.
- I read no engine source. Every claim above is from the corpus bytes.

No `general-proposals.md` and no `skill-proposals.md`: Edit 1 is A333 applied, Edit 2 is A334
applied, and Edits 3-4 are the installed skill's printed-number discipline. Nothing here needs a new
amendment.
