# Wave 76 — deck 146 (Orzhov dungeon midrange) per-deck review

Seat files: `matchups-20260910-141526-final/*ai_baka_deck146-0x*-vs-*.jsonl` (6 games, single binary
`wagic-d372a1153-w76step1`). Live guide reviewed = the wave-75 edition (19,973 B). `146v152` is a
first-run game; the other five are reruns after the pilot wedge.

Seat census: **336 model decisions** (ask 250, priority 53, attackers 29, reveal 2, bottom 1,
blockers 1), **0 fallbacks**, 0 `reply_truncated`, 0 `action_before_plan`, `transport
curl=0,http=200,empty=0` on 336/336, `thinking` true on 336/336, native reasoning on all 336 (median
5,436 chars, max 20,846). `plan_line_missing` 49/336 (14.6 %; wave 75 13.3 %), `off_protocol_bytes>0`
36/336 (10.7 %, max 157 B), `protocol_deviation_replies` 49 = `plan_line_missing` 49 exactly.
`reasoning_ngram_repeat` median 0.132, max 0.481. Latency p50 84.1 s, max 449 s. Gameend sums for
this seat: `ask_replays_reserved` 30, `identical_ask_answers_reserved` 25,
`identical_option_asks_resolved` 6, `hold_released_turn` 15, `hold_windows_skipped` 165 (cast 149 /
priority 16), `crossphase_identical_reputs` 16, `main_phase_windows_skipped` 10,
`own_turn_windows_skipped` 37, `phase2_answer_recovered` 2 / missing 0, **`forced_close_unrecorded`
0**, `chain_acting_rows` 84 (+ 120 cast) / `chain_selfharm_rows_cast` 85 /
`chain_windows_only_selfharm_cast` 71, `blocker_forecast_rows` 42 (multi 20, gang 8, collapsed 8),
`plan_names_uncastable_zone_card` **11 (now per-record — sampled below)**,
`plan_names_stranded_card` 0, `reserve_decline_windows_noted` 2, `async_drops` 0.

| # | Opponent | Result | Turn | Final life |
|---|---|---|---|---|
| 1 | 152 | **LOSS** | 11 | 0 / 17 |
| 2 | 123 | **LOSS** | 17 | -20 / 9 |
| 3 | 125 | **LOSS** | 41 | 0 / 36 |
| 4 | 126 | WIN | 26 | 14 / -10 |
| 5 | 130 | WIN | 12 | 16 / -2 |
| 6 | 162 | **LOSS** | 14 | 0 / 3 |

**2-4** (wave 75: 5-1). Three of the four losses are board/draw, not guide: 152 raced two 4-power
FLIERS while the seat drew three copies of a 1/1 ground body; 123 assembled Bloodline Keeper +
Intruder Alarm into 24 flying Vampires against a seat holding two Soul Shatters and no Verse; 162 is
a draw-punisher lock that killed the seat through its own compulsory draw step. **One loss is a
guide item**: 125 seq 38.

## 0. Wave-75 items: which recur, and what the wave-76 lanes closed on this seat

- **Wave-75 EDIT 2 (legend-rule reset) — FIRED CORRECTLY, first live test.** `146v126` seq 73: the
  menu offered `Cast Lolth, Spider Queen [legendary: you already control Lolth, Spider Queen ...]`
  with the battlefield copy at **loyalty 1**; the seat cast it and at seq 74 binned
  `Lolth, Spider Queen #1 ... [loyalty 1]`, keeping the fresh copy — exactly the case the edit was
  written for, and the opposite of the wave-75 misplay it was written against. The edit stands.
- **Wave-75 MED 3 / §Q14 (the 3-life land row said a plain land can attack) — CLOSED.** All 3
  `pay 3 life` rows this seat print `[usable (tap for mana) this turn]` with no `attack`
  (`146v152` seq 6, `146v130` seq 9, `146v125` seq 131). 0 regressions.
- **Wave-75 HIGH 1 / §Q3(a) (`NOT lethal` beside `LOOP COMPLETE`) — UNTESTED here.** 0 prompts in
  this seat render `LOOP COMPLETE` (deck123 never assembled Bond + Blood this corpus; deck126's
  Sanguine Bond and Exquisite Blood were each answered once before the pair completed — Verse seq 18
  on Exquisite Blood, Verse seq 26 on Sanguine Bond, Kaya `-3` seq 30 on the replacement Bond).
  0 `reaches 0 in N more turns` tags too, so Q3(b) is untested here as well.
- **§Q1 (hold-check bracket) — PARTLY CLOSED, with a clean residual.** See HIGH 1: 0 false claims in
  76 comparisons at a streak of 2 or more, **11 false in 43 at a streak of exactly 1**.
- **§Q13 (`crossphase_identical_reputs`) — the bracket prints, its board clause almost never does.**
  `[this exact list was put to you ...]` renders **15 times** (125: 11, 126: 4) against
  `crossphase_identical_reputs` 16; the `nothing on the board has changed` clause renders **2** of
  those 15. CQ F4's warning that the board comparison is too strict is borne out (13 %).
- **§Q9 (`plan_names_uncastable_zone_card`) — now per-record, and on this seat ~0/15 are real.**
  See MED 3.
- **§Q7 (`main_phase_windows_skipped`) — 10 for this seat, no contradiction visible from a seat
  file** (Main-2 phases that produce no window are invisible here). Engine seat's call.
- **Clean here:** 0 `http_error_body`, 0 non-200, 0 `reply_truncated`, `forced_close_unrecorded` 0,
  `plan_names_stranded_card` 0, `async_drops` 0, 2 `phase2_answer_recovered` / 0 missing.
- **CLEANUP DISCARD: 0 windows for the third wave running.** The block is again untested; not cut
  (wave 75's bottom-ask lesson).

## 1. Game by game — the deciding decisions

**vs 125 (LOSS, T41) — decided at `...vs-ai_baka_deck125.jsonl` seq 38 (T17 Main 1), and THE CAST
ORDER made the choice.** Board: seat 20, opponent 20, opponent's only permanent of note is
`Staff of Nin {6} [artifact]`, their graveyard already shows **`Essence Scatter {1}{u}`** and
`Supreme Verdict`, and the prompt reads **`Their untapped sources: 8 (colours they could make:
{u}{w})`**. The seat had 6 sources and a hand of Soul Shatter, Vanishing Verse, Silverquill Command,
Acererak, Silverquill Silencer, Kaya. It took row 6, `Cast Kaya the Inexorable ... {spends 5 of your
6 untapped mana sources this turn}`. Its reasoning closes with the guide as the whole argument:
> *"- Kaya costs {3}{W}{B}. I have 3W, 3B. Can cast. - **Kaya is priority 2 in cast order.** - Target
> for -3: Staff of Nin (artifact, nonland). Good."*

At seq 39 the stack reads `opponent's Fall of the Gavel {3}{u}{w} (instant) [spell] targeting Kaya
the Inexorable` over `your Kaya the Inexorable [spell]`; the plan line on that same window is
`"Cast Kaya the Inexorable, use -3 to exile Staff of Nin, attack with Goblin."` with the engine's
note `"Kaya the Inexorable" is no longer on your menu`. Kaya never resolved, the seat drew no second
one, and by T41 (seq 145) its hand is `Vanishing Verse x3 [no cast row now: it must have a target and
there is no legal target on the board]`, `Soul Shatter x2 {at 0 this does nothing}` against **two**
Staffs of Nin pinging 2 a turn behind Supreme Verdict + Final Judgment x2. Real alternatives existed
on that screen (rows 5 + 4 = Silencer 2 + Acererak 3 inside 6 sources; or Kaya one turn later at 7
lands alongside Verse). The guide's only counter-play sentence — `ONE SPELL A TURN INTO OPEN BLUE
MANA LOSES: prefer the turn you cast TWO things.` — lives in WHEN THEY COUNTER AND GAIN LIFE, states
no test, and was not reachable from cast-order entry 2. **STRATEGY item, Edit 1 below.**

**vs 162 (LOSS, T14, opponent at 3) — the seat was killed by its own draw step, and the last
decision was already dead.** `146v162` seq 35 (T15 Draw): life 4, **11 triggers on the stack, 9
damage, `ON THE STACK: 9 damage to you - you would be at -5; that would KILL you`**, of which 8 are
`Underworld Dreams's deal 1 damage`. Verse cannot remove a trigger already on the stack, so
`CHOICE: 1 (Cast Vanishing Verse)` targeting Underworld Dreams (seq 36) changed nothing; nothing on
that menu could. The kill was set up one turn earlier by `Teferi's Puzzle Box` entering on T14 and
the seat's **8-card hand** (`DRAW FORECAST: your draw step ... draws 11 cards (1 + Howling Mine 1 +
Dictate of Kruphix 1 + Teferi's Puzzle Box: your hand size 8)`). At T13 the Puzzle Box was not on
their board (seq 27's opponent battlefield lists Howling Mine, Liliana's Caress, Ob Nixilis,
Underworld Dreams only), so hand size was not yet a readable liability — no guide rule could have
been applied in time. Earlier play was guide-correct and good: Soul Shatter on `Ob Nixilis, the
Hate-Twisted (MV 5, their highest)` seq 27, Command pumping Nadaar to 6/6 flier for a 9-point swing
seq 29-32 that took the opponent 12 -> 3. **No guide item.**

**vs 152 (LOSS, T11) — no misplay found.** seq 16 (T11 Attackers) at 8 life under
`CRACK-BACK NEXT TURN: ... up to 8 - you would be at 0; that would KILL you - of that, 8 from 2
attackers nothing you control can legally block`. Both crack-back bodies are FLIERS
(Elite Spellbinder, Sigarda) and both of the seat's attackers print
`[held back, THIS creature could not block ANY of their 2 creatures]` — keeping them home stops
nothing, so `ATTACK: A1, A2` under the guide's `[no creature they control can block this attacker]`
override is right. The game was lost to three copies of a {b}{w} 1/1 and two hand cards exiled by
Elite Spellbinder (Emeria's Call T6, Acererak T10).

**vs 123 (LOSS, T17).** By seq 34 the opponent has **24 flying Vampires + Intruder Alarm** and the
crack-back line reads `50 - you would be at -29`. The seat's answers were used correctly and were
simply not enough: Silencer named `Damnation` (seq 8) and `Intruder Alarm` (seq 19) — both the
highest `{copies not yet in a public zone: 4 of theirs}` rows; Pelakka Predation took `Damnation`
off the reveal list (seq 14, a sweeper over a creature, guide-correct); Soul Shatter killed
`Bloodline Keeper (MV 4, their highest)` twice (seqs 22, 31); Lolth `-3` on the turn she landed
(seqs 25/26). No Verse was ever drawn. **No guide item.**

**vs 126 (WIN, T26) and vs 130 (WIN, T12) — clean.** 126: both loop halves answered
(Verse seq 18 Exquisite Blood, Verse seq 26 Sanguine Bond, Kaya `-3` seq 30 Sanguine Bond), the
legend-rule reset above, the Hive animated and swung at seq 87/93 for the kill. 130: Predation
reveal took `Lay Waste` (seq 20), a three-body swing at seq 22 took the opponent 16 -> 6, done at
T12 with 16 life left.

## 2. Engine / interface / card items

### HIGH 1 — the hold-check bracket asserts "every row above was also on the menu" on a streak of ONE, where it has no previous window to compare against
Repro `...vs-ai_baka_deck126.jsonl` **seq 29** (T15, Main 1 casting menu), rendered verbatim:
```
[hold check: every row above was also on the menu at the last window I asked you at this seam (1 window in a row now) ...]
```
The previous casting window at that seam is **seq 25** (T13), whose rows were
`1. Cast Acererak the Archlich` / `2. Cast Vanishing Verse` / hold / decline. Seq 29's rows are
`1. Cast Kaya the Inexorable` / `2. Cast Lolth, Spider Queen` / `3. Cast Acererak the Archlich` /
hold / decline — **both planeswalker rows are new**, and they are the two rows the guide forbids
ever holding away.
Census over this seat, comparing every record-adjacent pair at the same seam (seam reconstructed
from the `Casting decision (` / `Your legal actions` header, rows keyed after stripping balanced
`{...}` and `[...]` groups): **184 pairs, 119 "every row above was also" claims, 11 false — and
every one of the 11 is at `(1 window in a row now)`** (43 claims sit at that streak). At a streak of
2 or more: 76 claims, **0 false**. Of the 65 remaining pairs (an `N rows above are new` verdict),
5 are false-NEW (`1 row above is new` where my key sees none — the nested-annotation class, lower
severity) and 6 more disagree on the count. The other ten streak-1 falsehoods: `146v125` 25->27, 46->48, 54->57, 77->80;
`146v126` 19->21, 23->25, 38->45, 48->56; `146v130` 23->25; `146v162` 15->19.
So Q1's fix landed for a live streak and the residual is precisely the case Q1's own pin named: on
the FIRST window asked at a seam (memory empty or just reset) the bracket must say "first window
asked" or say nothing, not assert sameness. No decision was lost here — all 11 took a new row rather
than the hold — but the guide's HOLD rule reads this clause directly.

### MED 2 — `Silverquill Silencer`'s ordering annotation is right, but the `{copies not yet in a public zone: N of theirs}` list is the only readable engine surface, and it is unbounded by relevance
Not a defect; recorded so the engine seat has the counter-example: on this seat the sorted list did
its job twice (`146v123` seqs 8/19 named Damnation then Intruder Alarm — the two cards that
actually beat the seat).

### MED 3 — `plan_names_uncastable_zone_card` counts targets, opponents' permanents, stack objects and reanimation objects; ~0 of 15 on this seat are real
Full sample (the counter is 11; 15 records carry a non-zero field — the extra 4 have an empty PLAN
line):
- **Reanimation object** (the plan names the card a listed Command mode RETURNS from the seat's own
  graveyard — the menu row itself offers it): `146v125` seqs 50, 51, 122, 125; `146v162` seqs 29, 30,
  31. Seven. Repro `146v125` seq 125: `PLAN: Return Triumphant Adventurer from graveyard to put a
  body on the board ...` beside row `Cast Silverquill Command ... Return target creature card with
  mana value 2 or less from your graveyard to the battlefield.`
- **Target, not a cast**: `146v126` seq 31 (`cast Vanishing Verse targeting Exquisite Blood` — Verse
  is in hand, the Blood is on THEIR battlefield); `146v162` seq 27 (`Neutralize Ob Nixilis`).
- **Stack object**: `146v126` seq 39 (`Pass priority to let the stack resolve (Soul Shatter kills
  ...)` — Soul Shatter is ON THE STACK, cast the window before).
- **Opponent's hand**: `146v123` seq 14 (`Discard Damnation with Pelakka Predation`).
- **Empty plan line**: `146v123` 37, `146v125` 156, `146v126` 96, `146v162` 37.
Fix direction: the predicate should fire only on a card the plan says the SEAT will CAST that is in
no castable zone — exempting the object of a listed mode (return/reanimate/discard/exile), any card
on the current stack, any permanent of theirs, and a record with no PLAN line at all. As it stands
the field cannot be cited.

### MED 4 — `{this payment puts you at N}` is gone from the 3-life rows this corpus, and the seat answered `tap` all three times
`146v152` seq 6, `146v130` seq 9, `146v125` seq 131 all print
`1. pay 3 life - <land> enters UNTAPPED [usable (tap for mana) this turn] {this payment puts you at
N}` and `2. tap - ... [decline the payment; unusable until your next untap step]`, and all three were
answered `tap`. Wave-75's Edit 3 (condition (a) as a printed number) plus Q14's tag fix means this
row has stopped costing the seat life. Recorded as a PASS, no action.

### LOW 5 — one turn cost 33 model calls
`146v126` T23 = 17 windows (seqs 53-71), T25 = 15 (72-86), T27 = 9. As last wave, every link moves
the board (Acererak re-cast and re-bounced, Tomb rooms, drains, priority on their triggers); at p50
84 s that is ~24 minutes for one turn. Nothing to collapse, but it is where this seat's latency is.

## 3. Guide verdict: EDIT

Live 19,973 B -> revised **19,959 B** (`wave76/deck146/strategy.txt`). Two rules changed; the bytes
come from two deletions that remove no rule the corpus used.

**Edit 1 — Kaya and Lolth wait behind a counter test (the rule that earned its bytes).**
- before (WHEN THEY COUNTER AND GAIN LIFE): `KAYA IS THE ONLY ANSWER: her -3 goes at the ARTIFACT
  once her list names no planeswalker of theirs. ONE SPELL A TURN INTO OPEN BLUE MANA LOSES: prefer
  the turn you cast TWO things.`
- after: `KAYA IS THE ONLY ANSWER: her -3 goes at the ARTIFACT once her list names no planeswalker of
  theirs - so she is the card this deck cannot afford to lose to a counter. THE COUNTER TEST, from
  the turn a counterspell of theirs first resolves (their graveyard or the log names one): while
  "Their untapped sources" reads 2 or more, DO NOT LEAD WITH KAYA OR LOLTH. Take the cheapest
  entries that leave sources up, and cast her on a menu whose row says it leaves enough sources for
  a second row you also take that turn.`
- and cast-order entry 2 now carries the pointer: `... only under the loyalty test in PLANESWALKERS,
  and only past the counter test in WHEN THEY COUNTER.` Without it, entry 2's "Cast her NOW"
  outranks any condition written elsewhere — which is exactly what the reasoning at seq 38 shows
  ("Kaya is priority 2 in cast order").
- paid for by **`146v125` seq 38** (Kaya alone into `Their untapped sources: 8` with
  `Essence Scatter` already in their graveyard), **seq 39** (Fall of the Gavel on the stack over her)
  and **seq 145** (T41: three dead Verses, two dead Soul Shatters, two Staffs of Nin, no second
  Kaya). Every condition in the new test is a printed number or a named card on the same screen.

**Edit 2 — Soul Shatter's MV-0 sentence stops asserting a false fact about the board.**
- before (SOUL SHATTER bullet): `MV 0 means tokens: nothing there, or every name at MV 0 -> do not
  cast it.`
- after: `A NAMED PERMANENT IS A REAL KILL AT ANY MANA VALUE, 0 included; only "at 0 this does
  nothing" takes this card off the list.`
- paid for by **`146v162` seq 11**, where the row read `{right now: they sacrifice Shield Sphere
  (MV 0, their highest)}` — a real 0/6 artifact creature, the opponent's only blocker. The model cast
  it anyway (correctly: the seat's Nadaar then attacked freely, 20 -> 17 the same turn), which means
  it had to override the bullet; the bullet also contradicted cast-order entry 1, which only removes
  Soul Shatter on `"at 0 this does nothing"` or `"tied at MV 0"`. The new wording agrees with
  entry 1 and with the engine row.

**Edit 3 — the bytes.** DELETED: the VANISHING VERSE colour/type explanation (`Not multicolored, not
colorless (so most artifacts and lands), but the test is COLOR, not type: an artifact creature
costing {1}{B} is legal.`) — the very next sentence already rules that the printed target list is the
only truth, and every Verse cast in these six games was decided off that list (`146v152` seq 13,
`146v126` seqs 17/25, `146v162` seq 35); and the `COMPLETION MATTERS` bullet (Nadaar's post-dungeon
anthem and Barrowin's reanimation) — background, not a decision rule: the render prints the resulting
`(P/T)` directly (`Nadaar, Selfless Paladin (4/4) (printed 3/3)`, `146v162` seq 35) and Barrowin's
reanimation is asked at its own window, while WHICH DUNGEON already decides which dungeon to run.
Also compressed: Kaya's duplicate `[defender]` / power-0 paragraph, which restates the rule the
VANISHING VERSE bullet states two bullets above it.

## 4. What I did NOT check
- **`146v125`'s T27-T41 tail** decision by decision (60+ windows where the seat had no creature and
  nothing castable). I read enough to establish the board was unanswerable after seq 38-39; I did not
  audit each `Cast nothing right now`.
- **Opponent seats** beyond board context; no audit of any opponent deck's play.
- **The 123 game's earlier branches** — whether a different T7 Predation pick or T13 Lolth line beats
  Bloodline Keeper + Intruder Alarm. Both were guide-correct as played.
- **`main_phase_windows_skipped` 10, `own_turn_windows_skipped` 37, `hold_windows_skipped` 165** —
  not confirmable from a seat file.
- **Q3(a)/Q3(b), Q4, Q5, Q6, Q11, Q15, P14** — 0 renders in this seat this corpus (no LOOP COMPLETE,
  no own-clock tag, no `{crack-back cover:}`, no forced-sacrifice row, no up-to-N loyalty row, no
  forced close, no `repeat_count_reask`). UNTESTED here, not passed.
- **BLOCK LADDER: exactly ONE blockers decision in six games** (`146v162` seq 26,
  `Triumphant Adventurer blocks Fate Unraveler`, a rung-1 `you kill it, your blocker lives` take).
  The whole ladder is effectively untested for a second wave; not cut.
- **CLEANUP DISCARD: 0 windows, third wave.** Untested, not cut.
- **My HIGH-1 seam reconstruction is mine, not the engine's.** I keyed the seam off the prompt header
  and stripped balanced brace/bracket groups; if the engine's seam key is finer than
  cast-vs-priority, some of the 11 could be seam-crossings rather than false claims. The seq 25->29
  repro is checked by hand and is not.
- I read no engine source. Every claim above is from the corpus bytes.

No `general-proposals.md` and no `skill-proposals.md`: both edits are the installed skill's existing
discipline applied — one replaces an unreachable preference with a printed-number test, one deletes
a false claim about the board.
