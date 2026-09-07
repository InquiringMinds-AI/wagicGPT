# Wave 71 — deck 123 (Intruders of Thraben, WBU token combo) — per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-085638`, binary master b31f66cf3, `--thinking on`.
Six seat files `...ai_baka_deck123-<ptr>-vs-<opp>.jsonl`; record 1 win, 5 losses.
Seat census: 460 decisions, all with native reasoning; **1 fallback** (`noop_row_zero_reask`, and it is
a TRUE fire — see §2), 0 `action_before_plan`, 0 `plan_line_missing`, 0 `reply_truncated`,
0 `menu_pass_no_progress`, 0 `ask_replays_refused`. `hold_windows_skipped` is 0 in five of six games
and 464 (81 cast / 383 priority) in the 49-turn deck125 game.

**Headline: four of the six losses were decided by lands, not by decisions.** vs126 and vs162 were
stuck on TWO lands from turn 4 to the end (`Land drop: you have no land you could play right now`
at 126 seq 49/56/67 and 162 seq 13/18/23/28); vs130 was a land-destruction game (Molten Rain,
Dwarven Blastminer) that reached turn 22 on one to two lands; vs146 was on four lands at turn 14.
That is variance and matchup, and I have not read it as guide effect.

## 1. Game by game

**vs deck152 — WIN, turn 14 (7 / −16). The combo assembled and the arithmetic was right.**
Doomsayer t6, Idyllic Tutor t8 (live bracket, named Intruder Alarm), Alarm t10. At **seq 27** (t11,
opponent's main 1) the model took the repeat row in one shot: stop = L 22 + C 4 + 3 = 29, M = 3,
`CHOICE: 2 (Create human with Thraben Doomsayer x26)` — exact, no re-ask, no overshoot. That is the
line the guide is written for, fired on the OPPONENT'S turn as the timing section says. Cost noted:
one single-tap window (**seq 26**) before it found the batch row, and eleven wasted passes after it
(**seq 28–38**, MED-3 below). The t12 attack (**seq 44**) sent 24 of 28 and kept 4 home; I checked
the render before calling it a misplay and it is NOT one — the ATTACK TOTAL line prints
`At least 23 damage lands whatever they block ... but every blocker they declare with LIFELINK gains
them the damage it deals - up to 4 life back across their 5 blockers, so blocking can leave them as
high as 3`, so all-in was not printed as lethal either, and the crack-back line said 16 into 9 life.

**vs deck125 — LOSS, turn 49 (0 / 65). THE DECIDING DECISION IS seq 435.** A prison/control game
(Lightmine Field, two Staff of Nin, Elixir, Supreme Verdict, Cancel, Dream Fracture, Essence
Scatter). Turn 40, life 9, **13 untapped sources**, and a menu carrying `5. Cast Bloodline Keeper`
next to three rows the engine itself marks dead — `1. Cast Tribute to Hunger {right now: they
control 0 creatures - at 0 this does nothing}`, `3. Cast Damnation {right now: destroys 0 ...}`,
`4. Cast Idyllic Tutor [finds only an enchantment card - every enchantment left in your library is a
copy of one you already control or hold: Intruder Alarm]`. The log showed Supreme Verdict cast on
turn 11 and Cancel / Dream Fracture / Supreme Verdict discarded **eleven times** between turns 22
and 39. The model cast the Keeper unbaited; **seq 438**'s own plan line reads "Bloodline Keeper is
countered by Essence Scatter". Its `reasoning` at 435 enumerates every other row — *"Idyllic Tutor?
Dead. ... So Bloodline Keeper is the only logical play"* — and never reaches the guide's bait rule.
**Classification: STRATEGY.** The rule exists (wave-70 edit 4 already widened its trigger to the
graveyard) but it lives in the Tutor/Alarm section, 100 lines from the P-list the model reads when
it is about to write a maker's number. Guide edit 3.

**vs deck146 — LOSS, turn 21 (0 / 21).** Four lands at t14 against a Silverquill Silencer board.
The guide-attributable decision is **seq 84** (t14, main 1, 3 sources, `Your battlefield (4
permanents listed, of which 0 are creatures ...)`): it cast `1. Cast Intruder Alarm {2}{u} {leaves 0
of your 3 untapped mana sources untapped - casting this taps you out}` on a creatureless board.
This is the interesting one, because **wave-70 edit 3 worked and was then overruled**: the
`reasoning` quotes the new P3 gate verbatim, says *"I have 0 creatures on the battlefield. The guide
says Alarm is not a play if I have 0 creatures"* — and then finds
*"ALARM IN HAND AND CASTABLE THIS TURN: ... Land, Alarm, then tap"* in TIMING THE CHAIN and
concludes *"This implies casting Alarm first is correct."* Two guide lines contradict each other and
the later one won. **Classification: STRATEGY / guide self-contradiction.** Guide edit 1.
Damnation at **seq 87** (M 0, N 1, K 1, killing the Silencer that taxed every Keeper cast 3 life at
7 life) is correct by the guide and I flag it as such: no self-sweep anywhere in this seat.

**vs deck162 — LOSS, turn 12 (0 / 22).** Two lands from t4; Fate Unraveler + Teferi's Puzzle Box +
Liliana's Caress turned every forced draw into 3 life. Not winnable from that mana. One real guide
violation, not decisive: **seq 39** (t10, life 8, 2 sources) cast
`2. Cast Devour Flesh {right now: they control 2 creatures - they choose which one; YOU
control 0 creatures - targeting yourself does nothing}` at N = 2 and named the opponent (**seq 40**),
tapping out and moving their life 20 → 22. The guide's edict rule is explicit — "N not exactly 1: no
edict" — and was simply not applied. No edit proposed: the rule is already as plain as it can be
written, and one violation at a lost board does not pay for bytes.

**vs deck126 — LOSS, turn 13 (0 / 42).** Tundra t2, Marsh Flats t4 cracked for Underground Sea, then
nothing: `Land drop: you have no land you could play right now` at t8, t10 and t12 while holding two
Bloodline Keepers, two Intruder Alarms and a Doomsayer. Pure screw. The three cleanup discards
(**seq 50, 61, 72**) all picked Idyllic Tutor, which is genuinely dead (an Alarm in hand, two Tutors
already in the graveyard) — a small deviation from the guide's "a listed copy 2 of 2 is always the
discard", and the model's choice was the better one. Noted, not edited.

**vs deck130 — LOSS, turn 22 (0 / 20).** Mono-red land destruction. Two Isolated Chapels on t1/t3
were the ONLY rows offered; five forced cleanup discards at one land. Nothing decision-shaped.

## 2. Engine / interface / card items

**RESOLVED — the wave-70 HIGH-1 false `noop_row_zero_reask` is gone (lane-BQ L3 PASS in this seat).**
Three non-Morbid Tragic Slip casts were ACCEPTED this wave with no fallback and no re-ask:
`...vs-ai_baka_deck162.jsonl` **seq 11**, `...vs-ai_baka_deck146.jsonl` **seq 52**,
`...vs-ai_baka_deck130.jsonl` **seq 5** — each on the exact rendered shape that fired last wave
(`{right now: -1/-1 (no creature has died this turn, so Morbid does NOT apply)} ... {kills: <name>}`).
The seat's single remaining fire is TRUE: `...vs-ai_baka_deck152.jsonl` **seq 10**, row
`2. Cast Lightning Greaves {2} {right now: you control 0 creatures - this equips nothing}` — a real
no-op the guide also declines; the re-ask worked and **seq 11** switched to Devour Flesh. 1 of 1
fires correct (wave 70: 0 of 3).

**HIGH-1 — the guide's "a Create row passed TWICE is retired for the turn" is FALSE against this
build; the row is re-offered at every phase seam of the opponent's turn.**
Repro: `1788789409-ai_baka_deck123-0x55fb0f650a60-vs-ai_baka_deck152.jsonl`, **seq 28 through 38** —
eleven consecutive priority windows on turn 11 (opponent's main 1 → combat begins → attackers →
blockers ×4 → combat damage → combat ends → main 2 ×2), every one carrying the identical pair
```
1. Create human with Thraben Doomsayer [cost: Tap] [repeat: you control 29 creatures; ...]
2. Create human with Thraben Doomsayer, repeated N times, then stop [...]
```
and every one answered `CHOICE: 0 (pass)` because M was already at the stop of 29. Latency cost:
**1,287 s of wall clock (21.5 minutes) and 11 model calls to advance nothing.** The hold row (row 3)
would have collapsed all eleven, but the guide's own P8 hold rule listed "a Create row" as a live
row, so the hold was forbidden — I have fixed the guide side (edit 2). The engine side is still
worth pricing: a Create row whose repeat bracket has not moved since the last window at this seam is
not new information, and lane BQ L4's extra own/opponent-turn windows multiply it.

**MED-2 — `{repeat: ...}` is on the plain Create row before the batch row exists (carried from
wave-70 MED-3, still true).** `...vs-ai_baka_deck152.jsonl` **seq 26** offers only
`1. Create human with Thraben Doomsayer [cost: Tap] {card text: ...}` with no repeat bracket and no
batch row; the batch row appears at **seq 27**. One wasted window per loop start (166 s here). Much
better than wave 70's five, but the same shape.

**MED-3 — the Idyllic Tutor "castable from exile" price and the `[NAMED BY THEIR Silverquill
Silencer]` tax both render correctly and both are outside every guide line that names a cost.**
`...vs-ai_baka_deck146.jsonl` **seq 86**: `1. Cast Bloodline Keeper {2}{b}{b} (3/3) [NAMED BY THEIR
Silverquill Silencer: casting this costs you 3 life and draws them a card - you would be at 4. This
price is PER CAST ...]`. The render is exemplary; noting it only because the guide's P-list prices
makers in mana alone, and at 7 life a 3-life-per-cast tax is a real reordering fact the guide cannot
see. Not edited (one game, and the model paid it correctly after clearing the Silencer first).

**LOW-4 — `YOUR PLAN (as you last stated it, N windows ago on turn M)` is still rendered as a
truncated fragment.** `...vs-ai_baka_deck146.jsonl` **seq 84**: the carried plan reads
`"then crack it for a Plains to expand the mana base before casting Intruder Alarm."` — opening on
"then", and the model spends four lines of `reasoning` re-deriving what it had meant. Same shape at
125 seq 435 (`"Pass through the opponent's turn, waiting for my next main phase ..."` is intact) and
126 seq 72. Carried from wave-70 LOW-4, unchanged.

**Card facts verified against `bin/Res/sets/primitives/mtg.txt`**: Tragic Slip (l.123128,
`ifnot morbid then -1/-1` / `if morbid then -13/-13`), Devour Flesh (l.29133, target=player,
`toughnesslifegain targetcontroller`), Intruder Alarm (l.58850, `lord(creature) doesnotuntap` plus
`@movedTo(creature|myBattlefield)` AND `@movedTo(creature|opponentBattlefield)` untap-all — the
symmetry the guide states), Lightning Greaves (l.67192, `{0}:equip`, shroud + haste). All four match
Oracle text and match what the prompts rendered. No card-script defect found in this seat.

## 3. Guide verdict: **EDIT**

Revised guide at `wave71/deck123/strategy.txt` — **19,968 bytes** (live guide 19,936; ceiling 20,000).
Three additions are paid for by two deletions, so the file grows by 32 bytes. No citations, counts or
history in the guide itself.

| # | before → after | seq that paid for it |
|---|---|---|
| 1 | `- ALARM IN HAND AND CASTABLE THIS TURN: the maker WAITS UNTAPPED until it resolves ... Land, Alarm, then tap; with no castable Alarm, tap freely.` → `- ALARM IN HAND AND A MAKER ALREADY ON YOUR BATTLEFIELD: the maker WAITS UNTAPPED until the Alarm resolves ... Land, Alarm, then tap; with no castable Alarm, tap freely. ORDERING ONLY: it never licenses an Alarm with no maker out, and "a maker next turn" is not a maker on your battlefield.` | 146 **seq 84**. This is the only edit whose evidence is a `reasoning` trace rather than an outcome, and it is the strongest kind: the model quoted wave-70's new P3 gate correctly, then found this line and overrode itself with it. +105 bytes: the gate does not need restating, the line that contradicts it needs its precondition. |
| 2 | hold rule `One live row - a maker, a live Tutor bracket, a "{kills: <name>}" removal row, a Create row - and the hold is wrong` → `... a Create row BELOW the stop - ...`; and stop rule `Once M is at or past the stop, EVERY Create row is a pass.` → `... EVERY Create row is a pass - and past the stop that row is DEAD: once it is all that is left on the window, take the HOLD, not a dozen more passes.` | 152 **seq 28–38** (eleven identical passes, 21.5 minutes). +137 bytes, and it is the only rule in the file that can buy back a double-digit window run: the stop already makes the row dead by the guide's own arithmetic, so P8's definition of a dead menu just has to admit it. Both halves are edited together on purpose — a bound in one place and not the other is exactly what produced edit 1's failure. |
| 3 | P1/P2 line, appended: `Before a maker's number: a counterspell or sweeper ANYWHERE in their log or graveyard, plus mana for TWO spells, means a dead row on this menu is cast FIRST - see the Tutor section.` | 125 **seq 435**. Wave-70 edit 4 widened the bait rule's TRIGGER and the model still never reached the rule — the fix this wave is placement, not wording: the test now sits on the line whose number the model is about to write. +185 bytes. |
| — | **CUTS paying for it (−395 bytes):** the whole `--- SITUATIONS AND THE RIGHT ANSWER ---` block (the stop-arithmetic worked example), and the Vision Skeins bullet's parenthetical card list. | The example was kept in wave 70 as "the only one carrying a number the model cannot derive". 152 **seq 27** derived exactly that number from the rule alone, first try, on a board the example does not describe — it has now been shown to be derivable, so it is bytes with no decision behind them. The `(Howling Mine, Underworld Dreams)` list names two cards that appear in no game of this seat and are not needed to read the rule. |
| — | **DELETED (−212 bytes):** `- BUT THE ROW DOES NOT WAIT WITH IT: pass a Create row TWICE in one turn and it is retired for the turn, Alarm or no Alarm. On an Alarm turn the FIRST Create window may be passed, the SECOND not.` | 152 **seq 28–38**: eleven passes of the same Create row inside one turn, the row returning every time. The claim is false against this build, and a false fact in a decision-time guide is worse than a missing one — the model that believes it will pass twice expecting the row to go away. (Counted inside the −395 above as a separate line; see HIGH-1 for whether the engine ever behaved this way.) |

### Did wave 70's edits pay off?
- **Edit 2 (hold bound): YES, cleanly.** Scanning every `Hold priority` and `Cast nothing right now`
  answer in all six files: **zero** were taken on a menu carrying a maker row, an Alarm row, a live
  Idyllic Tutor bracket (`still in your library, not on your battlefield or in your hand`) or a
  `{kills: ...}` row. Wave 70's vs146 seq 17 pattern did not recur once.
- **Edit 1 (Damnation maker-veto): no counter-evidence.** One Damnation cast in the seat (146 seq 87)
  and it passes the gate; the M ≥ 4 and maker-in-the-M-list cases never arose. UNTESTED, not proven.
- **Edit 3 (P3 Alarm gate): half.** Read and quoted correctly at 146 seq 84, then overridden by a
  contradicting line — hence edit 1 above.
- **Edit 4 (bait trigger widened to the graveyard): NO.** 125 seq 435 is the same failure with the
  wider trigger present; hence edit 3 above.

## 4. Optional proposals
None. The three edits are scoped to this deck's cards and to a self-contradiction inside this file;
HIGH-1 / MED-2 / MED-3 / LOW-4 are engine and render items. The one thing that generalises —
"a rule added in one section can be overruled by an unbounded older line in another, and the model
will quote both" — is already covered by the existing amendments on single-surface gates, so
restating it would be noise.

## 5. What I did NOT check
- Opponent seat files were read only for board context; I did not audit the opponents' decisions.
- The 282-record deck125 game was traced at its cast/attack/discard decisions and sampled across the
  383 priority holds rather than read window by window; I checked all 464 hold/decline answers
  programmatically for live rows (none) but did not read each one's `reasoning`.
- I did not adjudicate any prediction from `wave71/lane-{BO,BP,BR,BS}.md`; lane-BQ L3 is reported
  above only for my six files (1 of 1 fire correct, 3 previously-refused shapes accepted), which is
  not corpus-wide adjudication — that is the engine seat's.
- I did not check whether HIGH-1's re-offer behaviour is new in wave 71 or predates it; the guide
  line I deleted asserts the opposite, so one of the two is stale and I could not tell which
  without running the engine, which this seat does not do.
- The empty `askreplay/` sidecar: my six games report `ask_replays_reserved` 8–284 and
  `ask_replays_refused` 0, so no re-serve was refused here; whether the sidecar path is wrong is the
  engine seat's item.
- No build, no run, no git; nothing under `bin/Res` or `src/` was touched.
