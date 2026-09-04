# Wave-59 deck123 review — "Intruders Of Thraben" (WBU Intruder Alarm combo), 1-5

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-130044/`, seat files
`1788544<nnn>-ai_baka_deck123-<ptr>-vs-ai_baka_deck<N>.jsonl`, cited below as `vs<N> sNN`.
388 records, 306 `ask`s, 373 timed decisions across 6 games. Card facts checked against
`bin/Res/sets/primitives/mtg.txt`: Thraben Doomsayer 120583 ({1}{W}{W}, 2/2), Bloodline Keeper
12582 ({2}{B}{B}), Damnation 25517 ({2}{B}{B}), Intruder Alarm 58850 ({2}{U}), Devour Flesh
29133 ({1}{B}) — every rendered cost, P/T and text line matched its primitive, so no Scryfall
call was needed or made.

Corpus-slice health, against wave 58's same-deck slice: **0 `empty_reply` / transport fallbacks
in 373 decisions** (wave 58: 11 in this slice) — lane H holds here. 3 `named_row_reask`
(all vs130, §2 I2). 0 stale drops, 0 unparsed replies, 0 fallback discards. Median latency
19.5 s; one 977,594 ms outlier (§2 I5).

## 1. Game by game, and the decision that decided it

| game | result | turn | how it ended |
|---|---|---|---|
| vs162 | **WIN** 6 / -71 | 12 | combo assembled t10, 24 Vampires swung t12 — see A0 |
| vs152 | LOSS 0 / 22 | 11 | **decision** — declined its own land drop twice, A1 |
| vs146 | LOSS -1 / 22 | 11 | **decision** — cast a maker over a one-sided Damnation at 2 life, A2 |
| vs126 | LOSS -1 / 28 | 29 | **decision** — Damnation destroyed its OWN 31 creatures, A3 |
| vs125 | LOSS -9 / 66 | 64 | structural (creatureless lifegain control, opp to 66); two dead casts, A4 |
| vs130 | LOSS 0 / 17 | 22 | mana/colour development — never reached 4 sources with {B}{B}; 3 re-asks (I2) |

**A0 — the win is the guide executing.** `vs162 s17/s22` took the repeat row with an explicit
count (`CHOICE: 2 (Create vampire with Lord of Lineage x18)`), `s22-s25` stopped at the guide's
`L + C + 3` stop with the ritual plan line, and `s28` correctly attacked with **A1 only** — the
attacker list offered exactly one creature because the 24 tokens were summoning sick, and the
reply said so ("24 sick Vampires block next turn"). `s43` then sent `ATTACK: A1-A24` for 92
damage. Rule 1's stop, the repeat row and the summoning-sick teach all fired. This is the guide
working, not variance.

**A1 — `vs152`: it refused its own land drop twice, believing a fetch had spent it.**
`vs152 s17` (t8 Upkeep): it cracked Marsh Flats targeting Tundra. The narration is truthful —
`- Your Tundra entered the battlefield from your library`. At `s19` (t8 Main 1) the frame prints
`Land drop: NOT yet used this turn - you can still play a land`, the hand line lists **Isolated
Chapel, Marsh Flats, Underground Sea**, and rows 1-3 are `Play Isolated Chapel / Play Marsh
Flats / Play Underground Sea`. Reply: `CHOICE: 4 (Play no land right now)`. The identical ask at
`s21` (Main 2) got the same answer, and `s22`'s plan line says it aloud:
`PLAN: No land to play (already played Tundra). No creatures to cast (B/L Keeper too
expensive...)`. It stayed on 3 sources; Bloodline Keeper ({2}{B}{B} = 4) was uncastable until
t10; at `s24` it discarded Isolated Chapel — a land — to hand size. Dead t11 having resolved one
spell all game. Classification: **STRATEGY** (every surface was true, complete and present), and
the guide's Rule 0 says "If ANY 'Play' line is listed, take one. Every turn" — but nothing in the
file said a Marsh Flats crack does not consume the drop. That seam is §3 E1/E2.

**A2 — `vs146 s11`: the wave-58 A1 shape survives, on the exact board the wave-58 edit named.**
t10, **life 2**, opp 21. The frame prints
`CRACK-BACK NEXT TURN: 4 of their creatures will be able to attack (tapped ones untap first),
for up to 12 - you would be at -10; that would KILL you`, and row 3 is
`Cast Damnation {2}{b}{b} {right now: destroys 4 of their creatures (4 without a restriction
against attacking), 0 of yours}`. Reply: `CHOICE: 5 (Cast Bloodline Keeper)` with **no PLAN line
at all** — Rule 3's mandatory `Damnation: N, K, M` ritual was simply not written. Dead t11.
Classification: **STRATEGY**. The guide already contains this rule verbatim ("Your number is 0
and theirs is 2 or more: that is the best Damnation there is ... it is CL0"), and the wave-58 E3
edit that hardened it did not change the answer. More prose is not the fix; §4 is.

**A3 — `vs126 s86`: it destroyed its own 31-creature board while its own plan line said hold.**
t18, life 13, opp 22. Its battlefield line reads `(37 permanents listed, of which 31 are
creatures, 31 of them able to attack right now, and 5 are lands)` — 29 Humans + 2 Doomsayers.
Row 2: `Cast Damnation {2}{b}{b} {right now: destroys 6 of their creatures (all of them carry a
restriction against attacking), **31 of yours**}`. The reply was:

```
CHOICE: 2 (Cast Damnation)
PLAN: Damnation: N 6, K 0, M 31; hold (M > N, K=0 walls)
PLAN: L 22, C 6, stop 29; M 31 now; this window: pass (M >= stop)
```

Both plan lines say hold/pass; the answer line cast. After it the seat had no creature and no
token-maker (both Doomsayers died in its own sweeper) and lost 11 turns later without ever
rebuilding. Classification: **STRATEGY**, but the *mechanism* is a protocol seam, §4.
The same window two turns earlier (`vs126 s69`, t16) is the counter-example that proves the
mechanism: the reply opened `CHOICE: 2 (Cast Damnation)`, then ran 4,049 characters of
deliberation (431,938 ms — 7.2 minutes, the corpus's second-longest decision) and closed with
`CHOICE: 7 (Hold priority for the rest of this turn)`. The parser's separated-line supersession
rule took the LAST line and the board survived. When the model reasons, it gets it right; the
protocol asks it to commit first.
For the record, its `ATTACK: none` at `vs126 s71` was **correct**, not a misplay: 33 power into
5 untapped defenders whose block triggers gain 3+3+2+2 with **two** Sanguine Bonds out would
have taken the seat from 14 to below 0 by its own attack. The render's LIFE-TO-DAMAGE CONVERTER
block and the per-attacker blocking-trigger prices are what got that right.

**A4 — `vs125`: two casts the render priced at zero.** `s88` (t48):
`Cast Damnation ... {right now: destroys 0 of their creatures (0 without a restriction against
attacking), 0 of yours}`, reply `CHOICE: 1 (Cast Damnation) / PLAN: Damnation: N 0, K 0, M 0;
cast`. `s90` (t48, same turn): `Cast Devour Flesh {1}{b} {right now: they control 0 creatures -
at 0 this does nothing; YOU control 0 creatures - targeting yourself does nothing}`, cast, then
`s91` targeted the opponent. `s129` (t62) repeats the Damnation shape and this time the plan
line says `N 0, K 0, M 0; **hold**` while the CHOICE line casts — A3's seam again.
The guide's edict rule ("If N is not exactly 1, do not cast an edict - not at 0") covers `s90`;
Rule 3 covers `s88` only by inference from "cast when THEIR number is bigger than yours", which
is why §3 E3 states N = 0 explicitly. The matchup itself was unwinnable-shaped: deck125 presented
no creature after t12 and climbed to 66 life, against a deck that is 17/60 creature removal.

## 2. Engine / interface / card items

**HIGH — I1. The attackers ask has no aggregate lethality line; the blockers ask has one.**
Repro: `vs126 s71`, the whole `Combat: declare ALL attackers` block. It prints per-attacker
blocker tags, then `They have 5 untapped creatures able to block; declaring more than 5
attackers leaves at least (your attackers - 5) of them unblocked.` — and that is the entire
arithmetic offered for 31 attackers against a 22-life opponent. Nowhere is the total power of
the listed attackers, the minimum damage that gets through, the opponent's resulting life, or —
with a converter of THEIRS on the battlefield — the total life the block triggers would take off
the *attacker*. The defender's ask has exactly that line
(`INCOMING THIS COMBAT: N attackers, X unblocked damage - you would be at Y; this KILLS you`,
seen at `vs162 s9`). Making the model derive it costs whole inference budgets: the neighbouring
cast window `vs126 s69` spent 431,938 ms and 4,049 characters computing this by hand, and got a
correct answer only by reversing its own committed first line. Concrete ask: a mirror line on the
attackers ask — total power listed, blockers available, minimum unblocked damage, the opponent's
resulting life, and (when a converter of theirs is in play) the attacker's own resulting life
after the listed block-trigger gains.

**MED — I2. Cast menus still omit unaffordable hand cards with no reason given** (wave-58 I2,
unchanged). Repro `vs130 s29` (t19): the hand line prints `Bloodline Keeper {2}{b}{b} (3/3)
[creature]` and `Thraben Doomsayer {1}{w}{w} (2/2) [creature]`, `Mana available: 3 total ...
({W} 1, {U} 3, {B} 3)`, and the cast menu's five rows are Lightning Greaves / Devour Flesh /
Vision Skeins / nothing / hold. Neither creature appears and nothing says why (Keeper needs 4
sources; Doomsayer needs a second {W}). Reply `CHOICE: 5 (Cast Thraben Doomsayer)` →
`<refused: named_row_reask>`, a whole window spent. All 3 of this deck's re-asks are this shape
(`vs130 s23, s29, s37`). The on-menu rows already carry `{leaves 1 source - rows 2 and 3 need
more mana sources than the 1 this leaves}`; the same sentence applied to the cards left OFF the
menu would close it.

**MED — I3. Identical-permanent collapse is applied to the target list but not to the
kill-preview clause on the same row.** Repro `vs126 s86` row 4: the row prints
`{kills whichever you target: THEIRS - none; YOURS - Human #1, Human #2, ... Human #29}` — 308
characters enumerating 29 identical tokens one by one — while the `legal targets right now:`
list immediately below on the SAME row collapses them to `Human #1-#29 (creature 1/1) ... x29`.
Same defect on `vs126 s69`. Pure inference cost, on every Tragic Slip row against a wide own
board; the collapse routine used by the target list should run over the kill list too.

**MED — I4. The reply protocol forbids the reasoning the deck guides then require, and commits
the answer before it.** `AIPlayerGPT.cpp` `kReplyProtocol` (line ~193): "Your reply is ONE line,
or TWO when you write a plan ... LINE 1 is your ANSWER ... LINE 2 is a PLAN: line". Three deck
guides (this one included) mandate an arithmetic ritual on the PLAN line and say "WRITE THE
THREE NUMBERS BEFORE YOU WRITE THE CHOICE" — structurally impossible under LINE 1 / LINE 2.
Observed outcome: `vs126 s86` and `vs125 s129`, plan says hold, answer line casts; `vs126 s69`,
the model reasons anyway (4,049 chars) and the answer only survives because the parser's
separated-line supersession rule takes the later CHOICE. Routed as a general proposal (§4), not
a deck guide edit — the guide text is already correct and repeating it is the failure mode the
brief names.

**LOW — I5. One decision at 977,594 ms** (`vs146 s7`, t6, `Cast Idyllic Tutor`) — 77 s past the
900 s wall, with **no** `wall_miss` marker and no `fallback` field on the record, and the answer
was consumed normally. Corpus median for this deck is 19,544 ms. Flagged for the engine seat; I
did not open the `.stderr` files.

**PASS — I6. Wave-58's HIGH I1 (no crack-back number on the seat's own turn) is fixed.** The
`CRACK-BACK NEXT TURN: N of their creatures will be able to attack (tapped ones untap first),
for up to P - you would be at L` line now renders on the seat's own turn, in casting and land
windows, and counts creatures that *will untap* — `vs152 s19` (own-turn Main 1, "for up to 7 -
you would be at 7", two of the three counted creatures tagged `[tapped - cannot attack or block
this turn]`) and `vs146 s11` ("for up to 12 - you would be at -10; that would KILL you"). 100%
of this deck's own-turn casting/land windows from t6 on carried it.

**PASS — I7. Name-over-number recovery works.** `vs125 s138`: reply `CHOICE: 4 (Scrubland #1)`
on a menu where Scrubland #1 was row **1**; the engine resolved by name (`choice: 1`,
`chosen_text: Scrubland #1 [land] [your battlefield] [tapped]`), no re-ask, no fallback.

## 3. Guide verdict: **EDIT** → `wave59/deck123/strategy.txt`

Started from the live guide (`bin/Res/ai/baka/deck123_strategy.txt`, byte-identical to
`wave58/deck123/strategy.txt`). **70,998 → 70,982 bytes**, inside the 41-71 KB band. Three
additions and one correction, each paid by a seq; every byte paid back by compressions of
receipts this corpus falsified or discharged. No rule was removed.

**E1 — Rule 0 gains the fetch clause and loses a receipt this corpus falsified.**
Paid by `vs152 s19` and `s21`.
Before: `... and the drop is not spent by casting a spell. Held 55 of 55 last corpus.`
After: `... and the drop is not spent by casting a spell, by a Marsh Flats crack, or by anything
else that puts a land onto the battlefield from somewhere other than your hand. The screen says
so above the question, in these words: "Land drop: NOT yet used this turn - you can still play a
land". That line outranks your own memory of the turn.` + the WHAT THIS COST record of `s19/s21`.
(The 55-of-55 receipt was false this corpus: 2 refusals.)

**E2 — Rule 2 names the seam at the crack itself.** Paid by the same seqs, because that is where
the false belief was formed.
Before: `... the row does the arithmetic for you. Passing it in six windows across four turns lost
a game at -2 with no token made.`
After: `... the row does the arithmetic for you. CRACKING IT IS NOT YOUR LAND DROP: the fetched
land comes off your library, so the "Land drop:" question still arrives that same turn and Rule 0
still answers it with a "Play" line.` (The passed-fetch anecdote is discharged: 6 fetch cracks taken
this corpus and none passed — `vs152 s17`, `vs162 s5`, `vs125 s66/s76/s112/s122`; I checked the
per-record `chosen_text`, not every window's option list.)

**E3 — Rule 3 states N = 0 instead of leaving it to inference.** Paid by `vs125 s88` (cast at
`destroys 0 ... 0 of yours`) and `s129`.
Before: `Equal numbers are not a reason to cast it: keep the Damnation and take another action.`
After: `... take another action - and that includes 0 against 0, where the row itself prints
"destroys 0 of their creatures ... 0 of yours" and the card does nothing whatever. At N of 0 the
answer is "hold", whatever M is.`

**E4 — a false claim about reply reversals is replaced by what the engine actually does.**
Paid by `vs126 s86` (the answer line executed over its own "hold" plan line) and `vs126 s69` (a
reversal that was NOT thrown away — the later CHOICE won).
Before: `... a reply that reverses itself is thrown away and asked again. Zero such reversals in
two corpora - keep it there.`
After: `A reply that reverses itself is NOT thrown away: the engine executes the LAST answer line
you wrote, so a first line you have since argued out of is a play you have made.` + the s86
record + `Never leave a CHOICE line standing that your own plan line contradicts.`
This is the one place a guide edit can reach A3 at all; the general fix is §4.

**Compressions (settled receipts and discharged anecdotes only, no rule touched):**
`Held 55 of 55 last corpus` (falsified, E1); `Held 4 of 4 last corpus, 0 colour breaks`;
`Last corpus: 54 rows, 3 casts, every one at M = 0`; `Held three corpora: 267 edict rows last
corpus, 4 casts, every one at N = 1` (falsified by `vs125 s90`, rule kept); `It holds: 140 of
140 takes last corpus...`; `Held last corpus: 45 rows carrying that clause, 0 taken`; the second
half of the Rule-1 WHAT THIS COST anecdote; the second-Alarm, Alarm-at-14-life, dead-Tutor and
lone-Doomsayer anecdotes (all four rules kept, only their wave-57/58 stories dropped); the
`That is the whole test.` restatement in Rule 3.

**Not edited, deliberately — the MULLIGAN section and the ATTACK ladder.** All six hands were
kept and all six keeps are guide-legal; no game in this slice was lost to a mulligan decision.
The ATTACK ladder's new CHECK 0 (wave-58 E1) produced the correct `ATTACK: none` at `vs126 s71`
against a converter board and the correct all-in at `vs162 s43`; it has nothing to answer for
here.

## 4. Proposals

`general-proposals.md` — one proposal, on the LINE 1 / LINE 2 reply protocol vs. guide-mandated
arithmetic (I4). No `skill-proposals.md`: everything I would have said about ritual placement is
a property of the protocol, not of how a deck guide is written, and the current skill edition
(wave58, amendments 1-331) already carries the ritual-scoping and receipt-discipline amendments
this review leans on.

## 5. What I did NOT check

- I did not open the opponents' seat translogs, only my seat's narration of their turns plus
  `results.tsv`.
- I did not open the per-game `.stderr` files: nothing here speaks to `async_drops`, transport
  retries, `WAGIC_BLOCKLOG` or timeouts beyond what the JSONL records carry. I5 is unexplained.
- I did not adjudicate any lane prediction from `wave59/lane-{I,J,H,K}.md` or
  `wave58/lane-{F,G}.md`. I6 and the zero-`empty_reply` census are deck123-slice observations
  only; the corpus-wide adjudication is the engine seat's.
- I did not run wagic, build anything, or touch `bin/Res`, `src/` or git; `AIPlayerGPT.cpp` was
  read only, for the I4 protocol quote and the supersession rule.
- No Scryfall call: all five cards I lean on matched their primitives and their rendered rows.
