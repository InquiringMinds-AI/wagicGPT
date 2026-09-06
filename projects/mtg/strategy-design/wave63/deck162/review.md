# Wave-63 per-deck review — deck 162 ("Draw and Die!", UB forced-draw punish)

Corpus: `matchups-20260905-191148` (5 games) + rerun `matchups-20260905-214946` (152v162).
Seat files are `*-ai_baka_deck162-*-vs-ai_baka_deck<opp>.jsonl` in those directories.
Record: **6-0**. 0 fallbacks, 0 transport errors, 0 timeouts, max `deadline_pct` 22.6, 141 prompts.

## 1. Game by game

| opp | result | turns | decisions | the decision that decided it |
|---|---|---|---|---|
| 123 | WIN 21 to -8 | 13 | 16 | seq 6 t5 Underworld Dreams on curve, seq 8 a second one t7; two Masters of the Feast then out-raced a 25-token Intruder Alarm board (seq 16 lethal swing). |
| 130 | WIN 20 to -5 | 15 | 18 | seq 6/10/12: Dreams t5, Fate Unraveler t9 and t11 — three converters by t11; seq 16/18 unblockable Unravelers finished it. |
| 125 | WIN 20 to 0 | 23 | 35 | The brake held: seq 9/10/19 declined Master of the Feast / Dictate at K of 0 with an empty opposing board; seq 21 Ob Nixilis t16 released it, seq 27/28 Dreams + Puzzle Box, opp 12 -> 2 on one draw step. |
| 126 | WIN 9 to 0 | 15 | 28 | seq 19/23 Ob Nixilis -2 twice into a Sanguine Bond + Exquisite Blood board; **seq 28 ATTACK: none** off the summed / life-LOOP tag with the win already on the draw step. |
| 146 | WIN 9 to 0 | 15 | 19 | seq 8 declined Dictate at K of 0, seq 13/15/16 Fate Unraveler + two Dreams t10-t12, seq 18/19 Puzzle Box + second Dictate = 9 to 0 on their draw step. |
| 152 (rerun) | WIN 3 to 0 | 15 | 21 | **seq 18 BLOCKS: none** at 13 life into 10 damage, with the opponent on 3 and the DRAW FORECAST reading "1 x 3 = 3 ... that KILLS them". Blocking as the header suggested would have handed 5 lifelink life and killed the third punisher. |

The hung 152v162 (`1788653542-...deck162-...`) is used here only as hang evidence: this seat's last
record is seq 20, an **answered** land-drop ask at t14 Main phase 1 (`CHOICE: 1 (Play Island)`), then
nothing. It is **not** the `softlock-blockers-1788650768.md` shape — no Blockers window with a calm
stack on this side. Its seq 9/13/16/19 block decisions are excluded from the analysis above per brief.

Two windows that look like guide breaks and are not: 125 seq 9/10 and 146 seq 8 are the Rule-2 brake
firing correctly (K of 0, opponent board empty or one deathtouch 1/1); 126 seq 28 is the gang-block
STOP obeyed. Nothing in these six games was lost to a misplay.

## 2. Engine / interface / card items

**HIGH-1 — the blockers header's suggested assignment can contradict the same prompt's own DRAW
FORECAST, and taking it loses the won game.**
`matchups-20260905-214946/1788662989-ai_baka_deck162-0x55bde38fe250-vs-ai_baka_deck152.jsonl` seq 18
(t13, life 13, opp 3). Rendered:
`best case with every blocker assigned: you would be at 8 - one legal assignment that reaches it, chosen for your blockers' material as well as for the life: Fate Unraveler blocks Intrepid Adversary; 1 of those blockers die`
The same prompt prints `DRAW PUNISHERS ... yours - Underworld Dreams, Ob Nixilis, the Hate-Twisted, Fate Unraveler` and
`DRAW FORECAST (theirs): their next draw step draws 1 card = 1 x 3 = 3 ... they would be at 0; that KILLS them.`
The suggested block kills Fate Unraveler (3 punishers -> 2) **and** hands 5 lifelink life (opp 3 -> 8):
after it the forecast is 2 damage against 8 life, i.e. the header's "best case" destroys the kill the
same prompt just announced. This is E3's rank (survive AND keep material) extended: the suggestion has
no term for *what the block does to the printed forecast*. Fix: when a block line removes a name from
the DRAW PUNISHERS line or adds life to their total, recompute the forecast for that line and say so on
it; the suggestion must not name an assignment that invalidates the prompt's own lethal claim.
(The seat survived it only because the latch took the CoT's corrected `BLOCKS: none` — see LOW-1.)

**MED-2 — "They have N untapped creatures able to block" is not filtered by evasion and contradicts the
per-attacker tags in the same prompt.**
`.../1788653540-...deck162-...-vs-ai_baka_deck123.jsonl` seq 16: both attackers carry
`[no creature they control can block this attacker]` while the next line reads
`They have 25 untapped creatures able to block; declaring more than 25 attackers leaves at least (your attackers - 25) of them unblocked.`
Same shape at `1788653525-...vs-ai_baka_deck126` seq 15 (`They have 2 untapped creatures able to block`
against a lone flier and a board of ground walls). The ATTACK TOTAL line below it is correct
("At least 10 damage lands whatever they block"), so one prompt states both. Count the creatures that
can legally block *the listed attackers*, or drop the line when every attacker is unblockable.

**MED-3 — a forced-sacrifice row does not price the opponent's own LIFE-TO-DAMAGE CONVERTER, though the
same prompt names it and the attackers prompt does fold it.**
`.../1788653525-...vs-ai_baka_deck126.jsonl` seq 16 (Tribute to Hunger). Rows:
`1. Master of the Feast (5/5) ... [you SACRIFICE this; they gain 5 life (its toughness)]`
`2. Shield Sphere (0/6) ... [you SACRIFICE this; they gain 6 life (its toughness)]`
with `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond` above. The real prices were
10 and 12 life of swing; the log confirms `Opponent gained 5 life (now 20) / You lost 5 life (now 15)`.
The attackers prompt at seq 28 of the same file *does* append `and their converter takes N off you` to
each price — so the converter fold exists and is simply missing at this seam. Card fact verified: the
primitive's text is `Target opponent sacrifices a creature of their choice. You gain life equal to that
creature's toughness.` (mtg.txt:124076) — the toughness wording on the row is correct.

**MED-4 — the E13 draw/discard split gave the `{feeds:}` row two clauses ending in the same literal
`in your hand: N`, and they count different cards.**
`.../1788653517-...vs-ai_baka_deck125.jsonl` seq 9 row 1:
`{feeds: ... draw converters (they fire when the opponent DRAWS) on your battlefield: 0 (...); draw converters in your hand: 2 - Fate Unraveler, Fate Unraveler; discard punishers (...): on your battlefield: 1 - Liliana's Caress; in your hand: 0}`
A reader keying on the bare literal `in your hand: 0` (which is how every deck-162 guide edition since
wave 59 states the brake release) reads the CARESS count and holds a brake that the punisher count has
released. Suggest suffixing the second clause (`discard punishers in your hand: N`) so the two keys are
distinct. The rename also silently invalidated the old K literal — see §3.

**LOW-1 — E6's latch is now auditable for the LINE but not for the WHY.**
Same file/seq as HIGH-1: `answer_replaced: true`, `coded_answers: 2`,
`latched_line: "BLOCKS: none"`, `reply_trimmed_bytes: 5211`. The verbatim latched line is recorded
(that is the wave-63 fix working, and here the latch was *correct* — the first line was
`BLOCKS: B1:A3`, which loses the kill), but the 5,211 bytes containing the reasoning that produced it
are gone, so "why did the latch win" is still unanswerable from the record. Only 2 latches in 141
prompts this seat (the other, `vs125` seq 31 `latched_coded_line: 1`, `latched_line: "CHOICE: 1 (Cast Shield Sphere)"`, agreed with line 1).

**LOW-2 — self-target-only planeswalker menu contradicts the guide's "the offer comes back".**
`.../1788653517-...vs-ai_baka_deck125.jsonl` seq 23: all three Ob Nixilis -2 rows target the seat's own
walls and each carries `{if you pass here, this option is not offered again this turn}`. The deck guide
tells the seat to pass "the offer comes back". Not wrong of the engine — flagged so the guide and the
render agree on which is true; the seat took the Hold row and lost nothing.

**Worked as designed (no item):** the E4 gang-block hoist landed — `1788662989-...vs-ai_baka_deck152`
seq 21 renders `[their untapped blockers: GANG BLOCK: any 2 of them together deal 5, enough to kill this attacker; each result below is a LONE blocker only - ...]`
with the verdict FIRST, and the reply named it and answered `ATTACK: none`. That is exactly the deck-162
wave-62 HIGH (162v146 seq 16) closed. The summed-price form also landed and was obeyed
(`1788653525-...vs-ai_baka_deck126` seq 28: `these life prices SUM ... (1 if all block), plus up to 16 more`).
The reply protocol is now in the `system` record (`reply_protocol`, `reply_protocol_hash af947d2bc622a08a`).

## 3. Guide verdict: **EDIT** (literal repair only — no strategy change)

The strategy is validated at 6-0 and every rule that fired, fired correctly; nothing in the play was
wrong and no rule content is changed. What is wrong is that wave 63's E13 rename made the guide's most
load-bearing *literal test* unmatchable: across this seat's 141 prompts the string
`converters on your battlefield:` occurs **0 times** and
`draw converters (they fire when the opponent DRAWS) on your battlefield:` occurs **44 times**.
The live guide quotes the dead literal **19 times**, including the final pre-send check
("read that row's own text one more time: if it contains \"converters on your battlefield: 0\"").
A guide that instructs a string match on a string the engine never prints is a rule that can only be
obeyed by ignoring its own instruction — the model got these windows right semantically
(`vs146` seq 8, `vs126` seq 10/11, `vs125` seq 9/10/19), which is why the record is clean and why this
is a latent, not a realised, failure.

`wave63/deck162/strategy.txt` = the live guide with these edits and nothing else (58,932 -> 60,745 B,
inside the 41-71 KB band):

1. **19x, every quoted occurrence of the K key.** before: `converters on your battlefield:` →
   after: `draw converters (they fire when the opponent DRAWS) on your battlefield:`.
   Paid for by the rendered rows at `vs146` seq 8, `vs126` seq 10, `vs125` seq 9 (44/141 prompts carry
   the new form, 0/141 the old).
2. **The `{feeds:}` shape paragraph (Rule 2 opening).** before: the two-clause paraphrase ending
   "…the cards you hand them are free until a converter is out". after: the row's actual five-part
   shape, plus two new sentences naming (a) which clause K comes from and (b) that the row prints
   `in your hand:` TWICE and the trailing one is the Caress count. Paid for by `vs125` seq 9 (MED-4).
3. **4x the release-test literal.** before: `"in your hand: 0"` / `"in your hand: 1"` /
   `prints "in your hand: N - <names>"` → after: the same with the `draw converters ` prefix. Prose
   uses of "in your hand" are untouched. Same repro as edit 2.
4. **1x the `vs152` seq-11 worked example's quoted row** updated to the wave-63 rendering so the
   example matches what the seat will actually see.

No rung, threshold, ordering, mulligan or blocking rule is altered.

## 4. Proposals

None. No general-guide or skill change is paid for by this seat's evidence: the deck went 6-0, every
guide rule that had a window fired correctly, and the two engine-side items above (HIGH-1, MED-3) are
render fixes, not instruction problems.

## 5. What I did NOT check

- The 4 block decisions inside the hung `1788653542` 152v162 partial (excluded per brief); I used it
  only to establish the hang's shape from this seat. Note in passing, unverified as corpus evidence:
  that partial contains two-Fog-Bank gang blocks onto one trampler whose per-row
  "N tramples to your face" is the lone-blocker number — if that recurs in a natural game it is a
  render item for the engine seat.
- The opponents' own seat translogs beyond the log excerpts embedded in deck 162's prompts.
- Lane-AB/AC/AD/AE/AF prediction adjudication (engine seat's deliverable); the E4/E6/E13 observations
  above are incidental, not a census.
- `async_drops` / `transport` / `reveal_stall_forced` fields corpus-wide — I checked only this seat's
  141 records (none present, no fallback, no re-ask).
- Scryfall cross-checks: I verified Shield Sphere, Fog Bank, Master of the Feast, Tribute to Hunger and
  Intruder Alarm against the primitives only; all five matched the rendered text and the observed
  behaviour, so no Scryfall disagreement arose to report.
- I did not build, run wagic, or touch `bin/Res`, `src/` or git.
