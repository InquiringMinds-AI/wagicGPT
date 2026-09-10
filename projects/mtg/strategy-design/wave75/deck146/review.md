# Wave 75 — deck 146 (Orzhov dungeon midrange) per-deck review

Seat files: `matchups-20260910-003556-final/*ai_baka_deck146-0x*-vs-*.jsonl` (6 games, single binary
`wagic-7a04cd6da-w75step1`). Live guide reviewed = the wave-74 edition (19,989 B).

Seat census: **330 model decisions** (ask 228, priority 56, attackers 36, blockers 8, reveal 1,
bottom 1), **0 fallbacks**, 0 `reply_truncated`, 0 `action_before_plan`, native reasoning on all 330
(median 5,433 chars, max 18,723), transport `http=200` on 330/330. `plan_line_missing` 44/330
(13.3 %), `off_protocol_bytes>0` 37/330 (11.2 %), `protocol_deviation_replies` 44. Latency p50
93.4 s, max 411 s. Gameend sums for this seat: `ask_replays_reserved` 15,
`identical_ask_answers_reserved` 5, `hold_released_turn` 2, `hold_windows_skipped` 32 (cast 14 /
priority 18), `main_phase_windows_skipped` 32, `own_turn_windows_skipped` 120, `async_drops` 3,
`phase2_answer_recovered` 1 / missing 0, **`forced_close_unrecorded` 0**, `chain_acting_rows` 158 /
`chain_selfharm_rows` 98 (cast seam 59 / 25), `chain_windows_only_selfharm` 2 (+14 cast),
`blocker_forecast_rows` 42 (multi 20, gang 8, collapsed 12), `plan_names_uncastable_zone_card` 9,
`plan_names_stranded_card` 0, `repeat_annotated_takes` 0, `declined_face_latches` 0 (5th wave),
`sibling_window_asks_skipped` 0, `chain_windows_collapsed` 0, `stop_reached_windows_skipped` 0,
`menu_pass_no_progress` 0, `wall_miss_events` 0.

| # | Opponent | Result | Turn | Final life |
|---|---|---|---|---|
| 1 | 126 | **LOSS** | 22 | 0 / 52 |
| 2 | 123 | WIN | 21 | 20 / -9 |
| 3 | 152 | WIN | 17 | 17 / -7 |
| 4 | 162 | WIN | 13 | 12 / -2 |
| 5 | 130 | WIN | 13 | 11 / 0 |
| 6 | 125 | WIN | 21 | 20 / -9 |

**5-1** (wave 74: 5-1; the loss moved from 152 to 126). The loss is again not variance: it traces to
one Kaya `-3` target choice whose reasoning quotes the guide's own rule as the reason.

## 0. Wave-74 items: which recur, and what the wave-75 lanes closed on this seat

- **Wave-74 HIGH 1 / §P P1b (`[hold check:]` calls a row new on an annotation move) — CLOSED on this
  seat.** 145 consecutive same-seam comparisons, **0 cases** where byte-identical rows were called
  new. The one that looked like the wave-74 shape, `deck123` seqs 49→50 (`1 row above is new` on an
  apparently unchanged 4-row casting menu), is TRUE: two new Spider tokens entered the Verse row's
  legal-target list between the windows, so the row does name different targets. The decline-run
  ceiling fell from 7 (wave 74) to **6** (`deck123` seq 68) and to **2** everywhere else; four of six
  games never printed the decline note at all.
- **Wave-74 MED 3 / §P P18 (land drop decided at TWO windows) — CLOSED.** 7 prompts render
  `{TWO FACES, ONE CARD: ...}` and each names the face the row plays (`deck152` seq 3 took
  `Play Grimclimb Pathway ... the BACK face`); **0** `OTHER FACE of` rows and **0** second face
  windows in the seat. Wave 74 had 9 such pairs.
- **Wave-74 MED 4 / §P P19 (hold row ~1,100 B) — CLOSED.** 162 hold rows, **max and median 596 B**
  (< 650), and `[HOW A HOLD ENDS:` appears **exactly once** in each of the 162 prompts that render a
  hold row and never in the other 168. Holds taken: 8/330.
- **Wave-74 HIGH 2 / §P P20 (`reasoning_degenerate` blind) — CONFIRMED, and the new meter sees it.**
  `reasoning_degenerate` median 0.0034, **max 0.0107, 0 records above 0.5** — clean on every
  decision; `reasoning_ngram_repeat` on the same records runs median **0.139**, max **0.476**
  (`deck130` seq 6, an 18.7 KB trace). The two are not rank-correlated (CJ P20's claim holds here).
  The underlying waste recurs unchanged: the format-verification loop is still what the tail of a
  trace spends itself on (`deck162` seq 22, `deck125` seq 26 quoted below).
- **§P P6 (`CRACK-BACK NEXT TURN` phase-gated) — PASS.** Own-turn renders: Upkeep 23/33, Draw 1/2,
  Main 1 49/196, Main 2 0/7. Every non-rendering own-turn window I opened has an opponent line
  reading `0 of them without a restriction against attacking` — the suppression is honest, not the
  wave-74 gate.
- **§P P23d (one event class) — CONFIRMED.** 37 of 44 `plan_line_missing` records also carry
  `off_protocol_bytes>0` (84 %); the residue is 7 replies that carry only the answer line
  (`deck126` 39/49/69, `deck152` 10/23, `deck125` 32/50) — the owner-ruled `plan_absent` class, not
  a second defect.
- **§P P9/F3 (inline `CHOICE:` on the PLAN line) — UNTESTED here:** 0 of 330 replies put the answer
  on the PLAN's physical line, and there are 0 `unparsed_reply` records in the seat.
- **§P P12 (casting-seam chain census) — measured:** 59 acting / 25 self-harm rows at the casting
  seam, 14 windows offering only self-harm rows, against 158 / 98 / 2 at the priority seam.
- **§P P13 (`main_phase_windows_skipped`) — no contradiction found, not confirmable from a seat.**
  In `146v130` the counter reads 12 while only 3 own Main-1 phases put no `Casting decision`; the
  rest must be Main-2 phases that produced no window of any kind, which a seat file cannot see.
  Engine seat's call.
- **§P P2 / P21 — clean here:** 0 `http_error_body`, 0 non-200 transports, 0 `reply_truncated`,
  `forced_close_unrecorded` 0, one `phase2_answer_recovered` (`146v130`).
- **BOTTOM ASK rendered for the first time in six waves** (`deck162` seq 4), after wave-74 Edit 3 cut
  the guide's bottom block. The seat handled it correctly with no rule (mulligan to 6 on a
  zero-real-land 7, then bottomed the second Triumphant Adventurer, keeping both lands, Silencer,
  Soul Shatter, Barrowin; game won on T13). The cut stands.

## 1. Game by game — the deciding decisions

**vs 126 (LOSS, T22) — decided at `...vs-ai_baka_deck126.jsonl` seq 47 (T15 Main 1), and the GUIDE
made the choice.** At 20 vs 24 with Kaya at exactly 3 loyalty (one `-3` left, and the `-3` kills her),
the list held both `8. -3 ... targeting Sorin, Lord of Innistrad [opponent's battlefield]` (7 loyalty)
and `11. -3 ... targeting Staff of Nin [opponent's battlefield]`. The model considered Sorin and
rejected him **by quoting the guide**:
> *"Wait, should I check if Kaya's -3 on Sorin is better? Sorin has 7 loyalty. Exiling him removes the
> +1 vampire maker and the -2/+0 emblem and the -6 board wipe. That's huge. But Staff of Nin is an
> artifact, and the guide specifically says 'KAYA IS THE ONLY ANSWER: her -3 goes at the ARTIFACT.'
> for Staff of Nin/Elixir. So Staff of Nin is prioritized."*

It exiled the Staff (1 damage a turn). Three turns later, seq 63's log: *"Opponent used: -6: destroy
up to three and reanimate with Sorin ... targeting Barrowin of Clan Undurr, Nadaar, Selfless Paladin,
Lolth, Spider Queen"* — all three left the seat's board and entered the opponent's, and the opponent's
stolen Lolth immediately made two Spiders. The seat never recovered a board; at T20 the opponent
re-assembled Sanguine Bond + Exquisite Blood with the seat's two Verses and its only Kaya already
spent, and the loop killed it at T22 from 20 life. **STRATEGY item, Edit 1 below.** (Earlier play in
that game was guide-correct and good: Verse on a lifelink Vampire seq 21, Kaya `-3` on Sanguine Bond
the turn she landed seq 33, Verse on Exquisite Blood seq 37 — both halves answered once each.)

**vs 125 (WIN T21) — contains the clearest cost of wave-74 Edit 1, seq 26 (T13 Main 1).** Kaya was on
the battlefield at **6 loyalty**. The menu was `1. Cast Silverquill Silencer` / `2. Cast Kaya the
Inexorable [legendary: you already control Kaya the Inexorable ...]` / hold / decline. The model took
row 2, citing the guide:
> *"the guide says: 'A second Kaya or Lolth in hand is a loyalty reset, NOT A DEAD CARD. A cast row
> tagged "[legendary: you already control ..." is LIVE: casting it bins ONE copy (you pick) and the
> survivor is the NEW one, at FULL loyalty.' So casting Kaya is definitely correct."*

At the legend-rule ask (seq 27) it correctly binned the 5-loyalty new copy and kept the 6-loyalty one
(`deck125` seq 28/30 both show `[counters: 6x loyalty]`) — so the whole line spent **a card and the
turn's five mana to change nothing**, and Silverquill Silencer went uncast that turn. The game was won
anyway. **STRATEGY item, Edit 2 below** — and note the guide sentence it quoted states a false fact:
the survivor is whichever copy you pick, not necessarily the new one.

**vs 162 (WIN T13) — seq 22 pays 3 life for nothing.** After playing Emeria as a land, the 3-life menu
printed `{this payment puts you at 17}` with 2 untapped sources. The reasoning applies the guide's
test and gets condition (a) wrong: *"If untapped, I have 3 mana ... I can cast Barrowin {2}{b}{w} or a
Silverquill Command {2}{b}{w}. So yes"* — both hand rows on that same screen read
`[cannot pay now: needs 4 mana, you have 2 untapped sources]`. Nothing was castable at 3; the payment
bought nothing and the seat spent the rest of the game between 12 and 15 life. The surface was
correct, so this is a STRATEGY item — the test was not written against a printed number. **Edit 3.**

**vs 123 (WIN T21), vs 152 (WIN T17), vs 130 (WIN T13).** Clean. Highlights: `deck123` seqs 3/4 took
the land-backed rows only because the hand held no real land (guide-correct), seq 35 `-3` on Intruder
Alarm, seq 85 the five-body lethal swing; `deck152` seq 3 took the Grimclimb (BACK, black) face with
the pathway colour rule, seq 28 `-3` on Brutal Cathar the turn Kaya landed, seq 48 the Silencer block;
`deck130` seq 23 took both rung-1 blocks the ladder offers and won two turns later.

## 2. Engine / interface / card items

### HIGH 1 — the combat header prints `NOT lethal: block only where the trade favors you` on the same screen as a live `LOOP COMPLETE` paragraph
`...vs-ai_baka_deck126.jsonl` **seq 70** (T22 Blockers), rendered verbatim:
```
Combat: declare blockers for this whole combat in ONE decision.
Your life: 20. Unblocked, these attackers deal up to 1 - you would be at 19 - NOT lethal: block only where the trade favors you.
```
Twenty lines above it, on the same prompt:
```
LOOP COMPLETE: BOTH halves of a life LOOP (Sanguine Bond + Exquisite Blood) are on THEIR battlefield right now. Any life YOU lose, and any life THEY gain, chains until you are at 0 ...
INCOMING THIS COMBAT: 1 attacker, 1 unblocked damage - you would be at 19 ... NO best-case life figure is given: both halves of their life LOOP are in play ... Blocking to cut the damage does not bound it, so no survival claim is made from these figures.
```
The INCOMING block refuses to make a survival claim; the combat header makes one, three lines later,
and it is false — the combat WAS lethal and the game ended on it. The blocker rows themselves carry
the correct loop clause (`this is not a priced trade and the number above is not a ceiling`), so the
defect is the header emitter alone. This is P5's family outside P5's declared scope (lane CL scoped
its override to `{right now: ...}` groups). Census: the pair renders **8 times corpus-wide**, 3 of
them mine (`deck146` vs 126 seqs **64, 69, 70**), the rest on `deck126` vs146 45/59, `deck123` vs126
206, `deck126` vs162 37, `deck162` vs126 13. It is the exact line the BLOCK LADDER's "SURVIVAL FIRST"
rule reads. Fix: while a LOOP paragraph is live, the combat header states no lethality verdict and no
"trade favors you" advice — the same override the INCOMING block already applies.

### MED 2 — the own-clock tag prints `reaches 0 in N more turns` under a live loop paragraph, 55 times in one game
`125v126` deck125 seqs 177-345 (T25-T47). P10's fix gates the tag on a STATED lethal (stack, combat,
loop is named in the fix text but the census I can run only sees the tag and the paragraph
co-rendering). Not my seat, so I did not read the decisions; flagged for the engine seat because it is
the same false-promise class as HIGH 1 and P10 was adjudicated on the stack shape.

### MED 3 — the 3-life land row says a plain land can attack
Every `pay 3 life` row in this corpus (8 of 8, all in my seat) prints
`[usable (tap for mana / attack) this turn]`: `deck123` seq 5, `deck152` seq 8, `deck162` seq 22,
`deck130` seqs 15/20/26, `deck125` seqs 17/22. Emeria, Shattered Skyclave and Agadeem, the Undercrypt
are ordinary lands with no animation ability — nothing about them can ever attack. The clause reads as
a benefit on the paid row and is on the screen where the seat paid 3 life for nothing (`deck162`
seq 22). Fix: emit `attack` in that bracket only for a permanent that can become a creature.

### LOW 4 — `plan_names_uncastable_zone_card` is a gameend-only field
The counter reads 9 for this seat (`deck125` 6, `deck123` 2, `deck152` 1) but appears on no decision
record, so no seat agent can sample which plans it counted. P17 asked for a sample of 10; that has to
come from the engine, or the field has to be written per record like `plan_line_missing`.

### LOW 5 — one turn cost 14 model calls, and every link was genuine
`deck125` T21 Main 1 = 14 asks, `deck123` T19 Main 1 = 12, `deck123` T17 = 10. I checked each: the
board moves at every link (Acererak re-cast and re-bounced, dungeon rooms, land drops, Command modes),
so this is the Acererak engine, not a re-put window. At p50 93 s that is ~22 minutes of wall clock for
one turn; nothing to fix in the loop family, but it is where this seat's latency lives.

## 3. Guide verdict: EDIT

Live 19,989 B → revised **19,973 B** (`wave75/deck146/strategy.txt`). Three rules changed or added;
the bytes come from deleting one never-used bullet, one cast-order-redundant sentence, and eleven
compressions that remove no rule.

**Edit 1 — an opposing PLANESWALKER is the first target on Kaya's -3 list (the rule that earned its
bytes).**
- before (KAYA bullet): `THE TARGET IS THE BODY HITTING YOU, NOT THE ONE IN YOUR WAY: take the name
  the CRACK-BACK line prices ...`
- after: `A PLANESWALKER OF THEIRS ON THE -3 LIST GOES FIRST, ahead of every artifact and every
  creature: it makes a threat every turn it lives, its big minus takes your board, and nothing else
  in this deck answers one. Otherwise THE TARGET IS THE BODY HITTING YOU, NOT THE ONE IN YOUR WAY:
  ...` (the rest of the bullet unchanged)
- and, in WHEN THEY COUNTER AND GAIN LIFE, before: `KAYA IS THE ONLY ANSWER: her -3 goes at the
  ARTIFACT.` → after: `KAYA IS THE ONLY ANSWER: her -3 goes at the ARTIFACT once her list names no
  planeswalker of theirs.`
- paid for by **`146v126` seq 47** (the losing decision, whose reasoning names Sorin's `-6` and then
  defers to the artifact sentence) and **seq 63's log** (that `-6` taking Barrowin, Nadaar and Lolth
  in one activation). The absolute artifact clause was written for deck125's Elixir/Staff permission
  and had no exit; this gives it one.

**Edit 2 — the legend-rule reset is stated truthfully and gated on the loyalty you can read.**
- before: `A SECOND KAYA OR LOLTH IN HAND IS A LOYALTY RESET, NOT A DEAD CARD. A cast row tagged
  "[legendary: you already control ...]" is LIVE: casting it bins ONE copy (you pick) and the
  survivor is the NEW one, at FULL loyalty. When the copy on the battlefield is below 3 loyalty ...`
- after: `A SECOND KAYA OR LOLTH IN HAND IS A LOYALTY RESET FOR A SPENT COPY AND NOTHING ELSE. A cast
  row tagged "[legendary: you already control ...]" is LIVE: casting it bins ONE of the two copies
  and YOU CHOOSE WHICH at the legend-rule ask, so the survivor is whichever you keep. DECIDE BY THE
  LOYALTY ON YOUR BATTLEFIELD LINE. Below 3, so it cannot pay a -3: CAST THE SECOND COPY, bin the
  spent one, and -3 the fresh one the same turn - the only way this deck fires a second -3, and
  skipping the row as "legendary, useless" loses the game. At 3 or more the cast buys nothing and
  costs a card and the turn's mana: take the next entry on THE WHOLE CAST ORDER instead.`
- cast-order entry 2 now carries the pointer: `LOLTH or KAYA if listed - a row tagged "[legendary:
  you already control ...]" only under the loyalty test in PLANESWALKERS.` Without it, entry 2's
  "Cast her NOW" outranks any condition written elsewhere, which is what the seat followed.
- paid for by **`146v125` seq 26** (cast at 6 loyalty, quoting the false "the survivor is the NEW
  one" sentence) and **seqs 27/28** (the new copy binned, board unchanged). The wave-74 edit's
  intended case is preserved verbatim and still fires.

**Edit 3 — the 3-life test's condition (a) is now a printed number.**
- before: `(a) the untapped land turns a card in hand into a cast you make THIS window;`
- after: `(a) a card in hand prints "[cannot pay now: needs N mana ...]" with N no higher than your
  untapped source count PLUS ONE, and you cast it in this window;`
- paid for by **`146v162` seq 22**, where the old wording was applied and got the arithmetic wrong
  against two hand rows that printed the true number on the same screen.

**Edit 4 — the bytes.** DELETED: the `EMERIA'S CALL also gives your NON-ANGEL creatures
indestructible` bullet (Emeria's Call was never cast as a spell in these six games — it was played as
a land at `deck123` seq 4, `deck152` seq 7, `deck162` seq 21, `deck130` seqs 14/19, `deck125` seq 16 —
and the cast-order entry for it stays); the HOLD paragraph's `A "[hold check: N rows above are new]"
line does NOT override that - a row whose only change is a count of your untapped mana sources is the
same row` (the engine's own `[HOW A HOLD ENDS:` paragraph now states exactly this, once per prompt,
and P1b made the bracket honest — 0 false NEW verdicts in 145 comparisons); and
`Triumphant Adventurer (1/1 deathtouch) is your best blocker; keep it untapped under pressure` (the
ladder already rules by the engine's computed parenthesis, which prices deathtouch itself). Eleven
compressions with no rule removed: the cast-order preamble and entries 1-2, the Verse, Soul Shatter,
Command, Silencer, Predation and Acererak bullets, the pathway and land-backed bullets, the
converter, gang-block and chump rungs, the mulligan and discard wording.

## 4. What I did NOT check
- **The 126 game's earlier branches.** I traced seq 47 as the deciding decision but did not evaluate
  whether a different T11/T13 Kaya line (e.g. `-3` on Sorin at seq 33 instead of Sanguine Bond) beats
  the guide-correct one; the guide's Bond/Blood rule is explicit and was followed.
- **Opponent seats** beyond board context and the two cross-seat censuses (HIGH 1's 8 renders, MED 2's
  55) — no audit of any opponent deck's play.
- **`main_phase_windows_skipped` 32** — not confirmable from a seat file (see §0); nor
  `own_turn_windows_skipped` 120, `mana_only_windows_skipped` 0.
- **The 3 `async_drops`** (all in `146v123`): I confirmed no seq gap in the seat is unaccounted for
  once `ask_replays_reserved` 15 is subtracted, and that 0 decisions fell to the heuristic — not
  traced to a seam.
- **`identical_ask_answers_reserved` 5 / `ask_replays_reserved` 15** — I did not separate the two
  replay paths; the corpus-wide 1,332 / 1,801 jump is the engine seat's item (l).
- **CLEANUP DISCARD: 0 windows** in this seat for a second wave, so the whole block is untested here
  and I did not cut it (the bottom-ask cut is a warning against 0-render cuts — a bottom ask rendered
  this wave, one wave after that block was cut).
- BLOCK LADDER: 8 blocker decisions in six games, all correct, but only two involved a rung-3 choice.
  `[repeat: activated ...]` 0 renders, `repeat_annotated_takes` 0. No ANNOUNCE_X window, no
  `{crack-back cover: ...}`, no `[legendary: you already control ...]` on a creature row: O19, P7/F8
  and the legend-rule cover gloss are **UNTESTED** here.
- I read no engine source. Every claim above is from the corpus bytes.

No `general-proposals.md` and no `skill-proposals.md`: all three edits are the installed skill's
existing discipline applied (one deletes a false rules claim the guide introduced, one gives an
absolute rule the exit its own evidence demands, one replaces an unobservable test with a printed
number).
