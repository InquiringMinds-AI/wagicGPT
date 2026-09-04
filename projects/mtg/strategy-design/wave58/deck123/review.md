# Wave-58 deck123 review — "Intruders Of Thraben" (WBU Intruder Alarm combo), 0-6

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-015059/`. Seat files are
`1788504<nnn>-ai_baka_deck123-<ptr>-vs-ai_baka_deck<N>.jsonl`; below they are cited as
`vs<N> sNN`. 231 records, 164 `ask`s across 6 games. All card facts checked against
`bin/Res/sets/primitives/mtg.txt` (Thraben Doomsayer 120583, Bloodline Keeper 12582, Intruder
Alarm 58850, Damnation 25517) — no primitive/render disagreement found; no Scryfall call was
needed or made.

## 1. Game by game, and the decision that decided it

| game | result | turn | how it ended |
|---|---|---|---|
| vs152 | LOSS 30 / -12 | 11 | **decision** — see A1 |
| vs162 | LOSS 12 / -7 | 13 | **decision** — see A2 |
| vs146 | LOSS 22 / -5 | 17 | **decision** — see A1 (twice) |
| vs126 | LOSS 40 / 0 | 16 | stuck on 2 lands t4-t14; opp assembled Sanguine Bond + Exquisite Blood |
| vs130 | LOSS 20 / -6 | 28 | land destruction (Molten Rain ×2, Lay Waste) took it to 0 lands; not a decision |
| vs125 | LOSS 35 / 0 | 40 | both Bloodline Keepers countered (Essence Scatter); flooded on 3 Damnations + 4 edicts vs a creatureless Staff-of-Nin control deck |

Three of the six were lost at a nameable window; two (vs130, vs126) are matchup/variance and
one (vs125) is a structural mismatch — 17 of this deck's 60 cards are creature removal and the
opponent presented no creature worth killing after turn 12.

**A1 — the deck holds a live one-sided Damnation while dying, to keep assembling the combo.**
Three windows, three different games, same shape: the seat controls **0** creatures, the
Damnation row prints a large N and `0 of yours`, and the seat casts a combo piece instead.

- `vs152 s12` (t10, life 5, opp 25, opp board 4 creatures / 17 power): row 2 reads
  `Cast Damnation {2}{b}{b} {right now: destroys 4 of their creatures (4 without a restriction
  against attacking), 0 of yours}`. **Two** Damnations in hand. It answered
  `CHOICE: 6 (Cast Bloodline Keeper)` and died in the next combat at -12.
- `vs146 s22` (t14, life 9): row 2 `{right now: destroys 4 of their creatures (4 without a
  restriction against attacking), 0 of yours}` → `CHOICE: 4 (Cast Intruder Alarm)` — an Alarm
  with no maker on the battlefield.
- `vs146 s23` (t16, life 7): row 3 `{right now: destroys 5 of their creatures (5 without a
  restriction against attacking), 0 of yours}` → `CHOICE: 1 (Cast Thraben Doomsayer)`. Dead t17.

Classification: **STRATEGY**, and the guide already answers it — `CL0` in WHAT TO CAST WHEN
SEVERAL THINGS ARE LISTED is exactly "0 are creatures + K ≥ 2 → Damnation, nothing else first".
The mechanism by which the rule was bypassed is in §3 (E2).

**A2 — the deck attacks away the blockers that would have saved it.**
`vs162 s23` (t12, life **3**, opp 20). Board: Thraben Doomsayer #1 (4/4), Thraben Doomsayer #2
(4/4), Human (5/5), Bloodline Keeper (7/7 flying, summoning sick) — all inflated by Fateful hour
(`auto=this(controllerlife < 6) lord(other creature|mybattlefield) 2/2`, primitive 120584).
Their board: Master of the Feast (5/5 flier) and Fate Unraveler (3/4), both **tapped**, plus a
Fog Bank. Reply: `ATTACK: A1, A2, A3` — 13 power into a 20-life opponent, non-lethal. Next
window `vs162 s24` (t13) reads
`INCOMING THIS COMBAT: 2 attackers, 8 unblocked damage - you would be at -7; this KILLS you ...
best case with every blocker assigned: you would be at -2; no block saves you` — correct, and
correct only because three of its four bodies were tapped. Held home, the 7/7 blocks the 5/5 and
a 4/4 blocks the 3/4 and nothing connects. `vs162 s16` (t10, life 14) is the same play two turns
earlier. Classification: **STRATEGY** — and the guide's ATTACK ladder produced it (§3, E1).

## 2. Engine / interface / card items

**HIGH — I1. On the seat's own turn there is no crack-back number anywhere, at any life total.**
Repro: `vs162 s23`, the whole `Combat: declare ALL attackers` block. Every per-attacker tag is
`[held back, it CANNOT block: Fog Bank (flying), Master of the Feast (flying)]` — the tag
enumerates only what a body could **not** block, never what it **could**, and the block carries
no life total and no opposing-power total. The pilot was at 3 life facing 8 power that was
tapped-but-untapping, and nothing on the screen said so. Second repro, different window class:
`vs152 s12`, a Main-1 casting menu at 5 life against 4 untapped-next-turn creatures totalling 17
power — no incoming line either.
This is the *residual* of wave-57 R295 / D9. Lane C shipped D9 gated on the **opponent's** turn
(`lane-C.md`: "untap, cleanup and the post-end steps stay silent, as does the seat's own turn"),
and the original ask named a Main-1 window. The missing form has to count creatures that **will
untap**, not the ones untapped right now — at `vs162 s23` every relevant attacker was `[tapped -
cannot attack or block this turn]`, so a "currently able to attack" count renders 0 on the exact
board that kills the pilot.

**MED — I2. The cast menu omits unaffordable hand cards with no reason given, and the model
keeps naming them.** 7 `named_row_reask` fallbacks in 6 games (6 of them in vs126 alone:
`s8, s14, s19, s22, s26, s29`, every one `CHOICE: n (Cast Thraben Doomsayer)`), each costing a
whole window in a game the seat was already losing to a 2-land board. At `vs126 s28` the frame
prints `Mana available: 2 total`, `Your hand (8 cards): ... Thraben Doomsayer {1}{w}{w} (2/2)
[creature]; Bloodline Keeper {2}{b}{b} (3/3) [creature]; ...` and a cast menu whose only cast row
is Lightning Greaves — the absence is correct but unexplained. A line naming the hand cards left
off for mana and the shortfall would close it. (The `[RE-ASK]` recovery itself worked: 7 of 7
re-asks produced a legal answer, and no wrong cast followed.)

**MED — I3. The Cleanup-discard fallback threw away a token-maker.** `vs126 s32`, t12: the reply
was empty, `choice: -1`, and `chosen_text: Bloodline Keeper` — option **3** of 8. On that same
menu sat option 7 `Tribute to Hunger` (a second copy) and option 8 `Lightning Greaves`, which
this deck's guide names as its designated leftover. The fallback is not row-1 and is not
value-ordered; it discarded one of the deck's 8 combo halves while two spares were on the list.

**MED — I4. `empty_reply` arrives in bursts at a fixed ~2.51 s latency.** 11 `empty_reply` asks +
1 discard + 1 blockers across deck123's 6 games (≈8% of its `ask`s), latencies 2506-2554 ms
against 30-60 s for real replies — i.e. a transport short-circuit, which is what the code itself
calls it (`AIPlayerGPT.cpp:10301`, "the unreachable-endpoint word"). The re-ask recovery does not
help: `vs125 s28,s30,s32,s34,s36,s38,s40,s42` is **eight consecutive** empty replies spanning
t18 Main 2 through t19 Attackers, i.e. an entire turn played by fallback. One landed on a lethal
blockers ask (`vs162 s24`) — no block saved that one, but the class can decide a game.

**LOW — I5. Two `wall_miss_unrecorded` at latency 900,018 / 900,021 ms** (`vs130 s19`,
`vs146 s7`) — the 900 s wall. Both recovered. Noted for the engine seat, not diagnosed here.

**PASS — I6. The `NO LIVE CAST ROW ON THIS MENU` header did not lie once on this deck's slice.**
66 renders across the 6 games; every numbered `Cast <card>` row under them carried a
computed-zero `{right now: ...}` verdict, and the only rows without one were the
`Cast nothing right now` decline rows. Lane C's D2 fix holds here (deck123's slice only; the
corpus-wide adjudication is the engine seat's).

## 3. Guide verdict: **EDIT** → `wave58/deck123/strategy.txt`

Started from the live guide (`bin/Res/ai/baka/deck123_strategy.txt`, byte-identical to
`wave57/deck123/strategy.txt`). **70,921 → 70,998 bytes**, inside the 41-71 KB band; four
additions are paid for by nine compressions of text this corpus shows is settled.

**E1 — new `CHECK 0 - THE CRACK-BACK` at the head of the ATTACK ladder** (`ATTACK - five checks`
→ `six checks`). Paid by `vs162 s23` and `s16`. The ladder as written *produced* the fatal
attack: Check 1 (no wall/loop names) → Check 2 (no Lightmine) → Check 3 (`N is 1 and no GANG
BLOCK: the parenthesis is the whole story`) → Check 4 (3 attackers vs 1 untapped blocker, "your
number is bigger... Check 5 applies") → **Check 5 SEND EVERYTHING**. Nothing in five checks reads
the pilot's own life, and Check 5's only hold-back clauses are about keeping a maker untapped
under Intruder Alarm — which did not apply, because no Alarm was on the battlefield. Before →
after: the ladder now opens with "read YOUR life, then add the printed power of every creature on
THEIR line — the `[tapped - cannot attack or block this turn]` ones INCLUDED, because they untap
before they swing", answers `ATTACK: none` when that total reaches the pilot's life, and states
that nothing on the pilot's own turn prints this number (I1).

**E2 — Rule 1's PLAN ritual is scoped to token-activation windows.** Paid by `vs146 s22` and
`s23`. The guide carries two mandatory PLAN rituals: Rule 1's `PLAN: L <opp life>, C <their
creature count>, stop <L+C+3>; M is <M> now` and Rule 3's `PLAN: Damnation: N <>, K <>, M <>`.
Neither was scoped to a window class. At both A1 windows the seat wrote the *Rule 1* line —
`L=22, C=3, stop=26; M=0 now; this window: cast Idyllic Tutor` at `s22` — on a casting menu with
no `Create ...` row at all, and so never performed Rule 3's count, whose answer was already
sitting in its own line as `M=0`. Before → after: Rule 1's shape now says it belongs to the
token-activation window and nothing else, and names Rule 3's ritual as the one a Damnation menu
takes.

**E3 — Rule 3: the Damnation ritual is the ONLY plan line on a menu carrying a Damnation row,
and `hold` is unavailable at M=0 / K≥2 / own life ≤ 10.** Paid by `vs152 s12` (5 life, two
Damnations in hand, `destroys 4 ... 0 of yours`, cast a Bloodline Keeper, dead at -12).

**E4 — a counterweight on the Fateful-hour paragraph in KEY CARDS.** That paragraph currently
ends "at 5 or less, re-read 'Your creatures that can attack:' and add up the numbers ... before
you answer 'ATTACK: none'" — it is the only place in the file that argues *for* attacking at low
life, and `vs162 s23` is that argument executed (it added 4+4+5 = 13 against a 20-life opponent
and sent it anyway). After: the sum is lethal or it is nothing, and Fateful hour is a consolation
for a life total you are trying to leave, never a reason to stay on it or to decline a sweeper.

**Compressions (no rule removed, only settled evidence and restatement):** the menu-summary half
of "THE UNIT OF THIS TEST IS THE ROW" plus its own drop-condition — that condition is now met on
this deck's slice (I6, 66/66); the plan-reversal paragraph (0 `commit_retracted` and 0
`answer_replaced` in 231 records, two corpora running); three HOLD paragraphs folded to one (the
operative "take it at the FIRST dead window" survives); the opponent's-turn-instants anecdote;
two mulligan branches; the Greaves and 1,868-activation anecdotes; the duplicated Fateful-hour
text in the Doomsayer card entry; the repeat-row format and stop-recompute restatements.

**Not edited, deliberately — the MULLIGAN section.** All six opening hands were kept and all six
keeps are guide-legal: five 2-landers (guide: "keep. Full stop") and one 1-lander whose coverage
line named Tragic Slip (guide: "'would cover the cost of:' naming a card is a keep"). Only
`vs126` actually stalled (2 lands, t4→t14). One screwed game out of six is not evidence against a
24-land deck's keep rule, and the brief forbids reading variance as guide effect. Worth a note
for a future wave rather than an edit: the `vs162` keep was Scrubland + 2× Intruder Alarm {2}{U}
+ 2× Doomsayer, and the deck's only blue sources are Tundra/Underground Sea/Arcane Sanctum — it
reached four lands and still never cast either Alarm. If the colour clause is ever revisited, the
{U} half of this deck is the place to look, and one game is not enough to move it.

## 4. Proposals

`general-proposals.md` (1) and `skill-proposals.md` (1) accompany this file.

## 5. What I did NOT check

- I did not read the opponent-seat translogs in full — only my seat's `events` narration of their
  turns, plus the `results.tsv` row.
- I did not open the per-game `.stderr` files, so nothing here speaks to `WAGIC_BLOCKLOG`,
  `async_drops`, timeouts or the D4 drift receipts; that is the engine seat's slice.
- I did not adjudicate any lane prediction. I6 is a deck123-slice census of the D2 header only.
- I did not run wagic, build anything, or touch `bin/Res`, `src/` or git.
- No Scryfall call: the four cards I lean on matched their primitives and their rendered text,
  so there was no disagreement to report.
