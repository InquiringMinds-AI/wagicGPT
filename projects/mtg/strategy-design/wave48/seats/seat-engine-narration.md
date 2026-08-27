# Wave-48 seat review — ENGINE / NARRATION

Corpus `matchups-20260827-072004`, binary master eccf8284c (E1 5ce8f8a5a + lanes P/O/N/Q).
42 seat logs, **1,813 records**, **1,729 asks** (ask 1240 / priority 364 / attackers 71 /
blockers 26 / reveal 20 / bottom 8) + 42 gamestart + 42 gameend. **21/21 natural** (every
seat carries a `gameend`; max seat 133 records, `deck130-vs-deck125`). Wave 47 was 5,726
decisions with one seat at 1,921 records and no gameend.

Every string quoted below is the emitter's ACTUAL output, read out of the corpus with python
(`/usr/bin/grep` + `python3`, never bare grep). Seat names are `deckA-vs-deckB` = the deckA
seat's JSONL (`...ai_baka_deckA-<ptr>-vs-ai_baka_deckB.jsonl`); `seq` is the record's own
field. Seat scope: engine + narration only. No deckN/ outputs, no guide edits.

**Health.** Fallbacks **5 / 1,729 = 0.289%** (baseline 0.10-0.31%: in band, upper edge).
`retracted_choice` **0** (was 2). `answer_replaced` **21** (was 0 — this is E1 working, see
below). `commit_retracted` 0. `parse_note`: `repeat_count_missing` x4, `echo_index_conflict`
x1. Explicit `CHOICE: 0` passes: **214**.

⚠ Two corrections to the numbers I was handed ("known so far"): (1) the longest prompt is
**61,240 chars** (`deck126-vs-deck123` seq **15**, blockers), not 135,718 — that figure is the
JSON *line* length (136,018 incl. the 326-char `events` field and the reply); there is exactly
ONE prompt over 60 KB, and `deck123-vs-deck126` seq 32 / `deck126-vs-deck123` seq 16 are the
**gameend** records (no prompt). (2) "repeat-N row offered 14 times and chosen 0" is **false**:
it was offered 14 times and **TAKEN 12 times** (`choice == 2` on the repeat row), 8 of them
with a parseable count that the engine executed. Details in lane N.

---

## VERDICT TABLE — Set B, by the emitter's string

| Lane | Prediction | Verdict | Count / citation |
|---|---|---|---|
| E1 5ce8f8a5a | retraction-to-Baka on replies whose LAST coded `CHOICE:` is a real re-answer: 0/N | **PASS** | `retracted_choice` 0/1,729; `answer_replaced` 21/21 audited: chosen index == the reply's last coded line in 21/21 (e.g. `deck123-vs-deck126` seq 25: line-1 `CHOICE: 2 (... repeated N times, then stop)`, final `CHOICE: 0 (pass)` → choice 0) |
| N 7a858904d | no game reaches 500 priority windows in one turn | **PASS** | max **9** priority windows in one turn (`deck123-vs-deck126` turn-field 10); next 7 (`deck123-vs-deck162` t13) |
| N | the three deck123 token games close in < 60 priority records each | **PASS** | `deck123-vs-deck126` **17** (was 1,550); `deck123-vs-deck162` **12** (was 1,871, dead-looped); `deck123-vs-deck146` **8** (was 223) |
| N | 21/21 natural | **PASS** | 42/42 seats carry `gameend`; results.tsv 21 rows |
| N | `[repeat:]` tag maximum reconcilable with the seat's own activation count | **PASS** | `deck123-vs-deck126` max `activated this turn 203 times` at seq 25 = 2 singles + 50 + 50 + 1 (countless echo) + 50 + 50 — matches the five `ran 50 of the 50` receipts + the single; `deck123-vs-deck162` max 20 at seq 32 = 1+1+1+1+15+1 (seq 23/27/28/29 singles, seq 30 x15, seq 31 single) |
| N (OPEN) | does qwen ever TAKE the repeat row? | **YES — 12/14** | `deck123-vs-deck126` seq 14, 20, 21, 23, 24 (`x50` each, executed); `deck123-vs-deck162` seq 30 (`x13` line 1 → `x15` last line, E1 took 15, executed); seq 28/29/31 + `deck123-vs-deck126` seq 22 taken WITHOUT a parseable count (`repeat_count_missing`) → one activation each. Pass row chosen on the repeat-row window 2/14 (seq 25, 15 of `deck123-vs-deck126`) |
| N | `0. Pass priority (take no action this window)` row present on single-option priority windows | **PASS** | 139/139 single-option priority windows carry the row (+14 repeat-row windows = 153 total); on those 139 the model chose 0 in **75**, 1 in **64** — a decline that was 0/1,871 in wave 47 is now 54% |
| O 96a07e864 | no prompt > 60 KB | **FAIL (by 1.2 KB, 1/1,729)** | `deck126-vs-deck123` seq 15 blockers = **61,240** chars; every other prompt ≤ 27,987. Cause below (B-row `may block` list not ranged). Was 233,662 / 116,128 / 74,063 |
| O | blockers p90 latency ≤ 60 s | **PASS** | n=26, p90 **56.1 s**, max 62.1 s (`deck130-vs-deck162` seq 20) |
| O | attackers p90 latency ≤ 60 s | **PASS** | n=71, p90 **50.6 s**; max 347.4 s (`deck130-vs-deck152` seq 24, 8,887-char prompt, 4,118-char reply — reasoning, not prompt size) |
| O (watch) | mixed-body attack may still interleave A-rows | **OBSERVED 2/71** | `deck146-vs-deck162` seq 15, 20: `A1. Triumphant Adventurer #1 ... A3. Triumphant Adventurer #2` identical rows split by `A2. Silverquill Silencer` (order is lexicographic on name+#, so `#1`/`#2` straddle). Cosmetic at 4 rows; same mechanism as the 61 KB defect |
| P 9b19fbfe6 | header number below the `[tapped - attacking]` row count: 0/N | **PASS** | 26 prompts carry `N of them are attacking right now`; header ≥ row count in 26/26 |
| P | reply pricing a collapsed swing by the representative alone while a larger price sits on the line: 0/N | **PASS (thin)** | the collapsed tag `they have N untapped creatures that could block this one, biggest ...` appeared in only **2** prompts (`deck146-vs-deck126` seq 72, `deck152-vs-deck130` seq 22); `most expensive to attack into:` rendered **1** time (seq 72); neither reply priced by the representative alone |
| P | counters spent on a stack object of printed power 0 / `{target text:}` beginning "Defender": 0/N | **PASS** | 33 windows offered a counterspell whose `can target on the stack:` clause carried a `{target text: "Defender ...` object (32 in `deck125-vs-deck126`, +`deck146-vs-deck126` 4, `deck130-vs-deck126` 9); chosen `Cast nothing right now` or a non-counter row in 33/33 |
| Q 7246c4a95 | `{card text:}` mid-word cuts: 0/N | **PASS** | 2,077 card-text blocks; 13 end in `...`, all 13 after a full stop (`...put it into your hand....`); 0 end on a letter followed by `...`. BUT see defect D-4: 118 blocks end UNMARKED on a bare back-face name (`// Agadeem, the Undercrypt`) |
| Q | `A choice is required` 0/N | **PASS** | 0/1,729; `Choose an option for <name>:` rendered 57 times (e.g. `deck152-vs-deck123` seq 4 `Choose an option for Barkchannel Pathway:`) |
| Q | `target controller` in any label: 0 | **PASS** | 0/1,729 (case-insensitive) |
| Q | draw tag on every controller-draw row under a punisher | **FAIL (1 row class)** | tagged rows 11 (e.g. `deck130-vs-deck162` seq 11 `[DRAW PRICE: this draws 1 card, and the opponent's Fate Unraveler punish every draw, so taking it costs you 1 life right now]`); UNTAGGED: `deck125-vs-deck162` seq 38 row 2 `Cast Sphinx's Revelation {u}{u}{w}{x} {X pricing: max affordable X=8 ... draws you 1 card}` with Fate Unraveler on the opponent's board — `draw:X` is not a plain number so `scriptSelfDrawCount` returns 0 by design. Same seat seq 41 again untagged (X=9) |
| Q | stale-plan X restated while a larger free-kill row is on screen: 0 | **PASS (0/4)** | 4 windows carried `[<- most kills at any affordable X that costs you nothing]`; no plan restatement (ANNOUNCE_X asks carry no plan block: 0/28 X-menus contain a PLAN line). NEW observation D-5: in 1/4 the model chose X=6 (row 1) while the marker sat on row 5 `X = 2 {X pricing: same kills as X=6, for 4 less mana}` — overpaid 4 mana (`deck130-vs-deck146` seq 47, reply just `CHOICE: 1 (X = 6)`) |

Set A is not this seat's (guide edits belong to the deck seats).

---

## Lane N in detail — the loop is CLOSED, and here is exactly how

### The three token games, record by record (priority windows only)
`deck123-vs-deck126` (opp ended at -226, turn 11): t9(field) Main-1 seq 12-15: single, single,
**`x50`** (seq 14, tag `activated this turn 2 times already ... taken it 3 times in a row`),
then **`CHOICE: 0 (pass)`** at seq 15 with the receipt on screen. t10(field, opponent's turn)
seq 18-25: single, single, x50, x50, countless echo (1 activation), x50, x50, then pass at seq
25 citing the guide's hard stop verbatim in its reasoning (`"This is a hard stop to prevent
infinite loops in the UI."`) — line 1 was the repeat row, last line `CHOICE: 0 (pass)`, E1
took the pass. Then seq 31 attackers `ATTACK: A1, A2, A3-A257` (range grammar USED) → 257
declared → game over. **17 priority windows, 1,550 → 17.**

`deck123-vs-deck162` (the wave-47 dead-looper; now ends at turn 15 with deck162 winning 15 to 0 —
results.tsv row `162 123 0 15 0`): t13 Main-1 seq 27-32: single, `N=45`
(unparsed → single), countless echo (single), `x13`→`x15` (15 run), countless echo (single),
pass. Never above 22 creatures. **12 priority windows, 1,871 → 12.**

`deck123-vs-deck146`: the loop never assembled (no Alarm); 8 priority windows, all passes or
fetchland cracks.

### The receipt string, verbatim (seq 15 `deck123-vs-deck126`)
```
- You used: Create human with Thraben Doomsayer (you named 50 repeats of it)
- Your Thraben Doomsayer created 50 1/1 Human tokens
- Your repeated activation ran 50 of the 50 times you named
```
Truthful, and the `created 50 1/1 Human tokens` line is `collapsedRunNarration`'s token shape
doing its job. The `x255`/`#1-#255` battlefield collapse and `A3-A257. Human #1-#255 (1/1)
deals 1 [...] x255` A-row collapse both held (seq 31).

### The four windows where the count was MISSING (defect D-2, MED)
| seat / seq | line-1 of reply | what the engine did |
|---|---|---|
| `deck123-vs-deck162` 28 | `CHOICE: 2 (Create human with Thraben Doomsayer #1, repeated N times, then stop, N=45)` (last line `N=17`) | `repeat_count_missing` → ONE activation (tag 2→3 at seq 29) |
| `deck123-vs-deck162` 29 | `CHOICE: 2 (Create human with Thraben Doomsayer #1, repeated N times, then stop)` | one activation |
| `deck123-vs-deck162` 31 | same verbatim echo | one activation |
| `deck123-vs-deck126` 22 | `CHOICE: 2 (Create human with Thraben Doomsayer, repeated N times, then stop)` | one activation (tag 102→103) |

`scanRepeatCountInLine` (AIPlayerGPT.cpp ~10700) accepts only `x50` / `x 50` / `50 times`.
The row's own text says *"you name N on the CHOICE line"*, so `N=45` is the most literal
possible compliance and it was thrown away. The three verbatim echoes are the model copying
the row label as its SHORT NAME (which the reply rules tell it to do) and never getting to a
number. In all four the engine silently ran once and NOTHING in the next prompt said the count
was missing — the narration shows a single `created a 1/1 Human token`, and the model re-echoed
(seq 29 → 31). Cost: 4 wasted round trips, ~1 min each. Fix shape (no choice removed): accept
`N=45`, `N = 45`, `, 45)`; and when the row is chosen with no count, either RE-ASK once
("you chose the repeat row but named no N") or narrate the receipt as `ran 1 time (you named
no count)` so the next window is informed.

### The mana-only gate, now the whole cost
`mana_only_windows_skipped`: **835** (`deck126-vs-deck123`, the seat opposite the 203-token
turn), **103** (`deck126-vs-deck152`), **0** in the other 40 seats. Wave 47 max was 8,610.
The gate is correct; 835 auto-passed windows for 203 activations (~4 per token: trigger +
untap + priority passes) is the residual, and it is invisible to the opposite seat (0 priority
records in `deck126-vs-deck123`), which is the intended outcome.

---

## Lane O in detail — the one prompt over 60 KB (defect D-1, HIGH)

`deck126-vs-deck123` seq **15**, blockers, **61,240 chars**, latency 28.7 s, reply
`BLOCKS: B1:A1, B2:A3, B3:A4`. The A side collapsed perfectly (3 rows):
```
A1. Thraben Doomsayer (2/2) deals 2 [doesn't untap during its controller's untap step]
A2. Bloodline Keeper (3/3) deals 3 [flying, ...] [NONE of ...
A3-A257. Human #1-#255 (1/1) deals 1 [doesn't untap during its controller's untap step] x255
```
The B side did not: **B1 alone is 22,926 chars**, because its `may block` clause enumerates
`A1 (neither dies (blocking trigger: you gain 3, and your converter takes 3 off them)), A3 (...),
A4 (...) ... A257 (...)` — **256 handles, ONE distinct parenthetical**. B2 and B3 repeat it.
The ranged handle `A3-A257` exists on the A side and is not reused on the B side; the identical
parenthetical is not factored. Two independent collapses would each fix it:
(a) the B-row `may block` list emits ranges over consecutive handles with an identical
parenthetical → `may block A1 (...), A3-A257 (neither dies (...))`; (b) factor the
parenthetical when every entry's is identical → `may block A1, A3-A257 (all: neither dies (...))`.
(a) alone takes this prompt to ~5 KB. Secondary: `B1. Pride Guardian #1` and `B3. Pride Guardian
#2` are identical rows split by `B2. Overgrown Battlement` (the same lexicographic `#n`
straddle as the attackers watch case) — grouping identical B rows across a gap would have halved
it again, but (a) is the fix that matters.

Ranged replies: the grammar was exercised **2** times, both accepted — `deck123-vs-deck126`
seq 31 `ATTACK: A1, A2, A3-A257` (257 attackers declared) and `deck130-vs-deck152` seq 24
`ATTACK: A1, A2-A4, A5-A7`. `kAttackerRangeNote` rendered 9 times (every prompt with an A-range
row). Numbered-option ranges (`2-4. -2: destroy target creature ... targeting Goblin #1-#3`)
rendered in 50 prompts; a choice landed INSIDE a collapsed range 2 times, both accepted.
`NarrationCycleHolder`: the log's period-1 collapse is visible as `created 50 1/1 Human
tokens`; no `- ... N times` sentence form appeared in any prompt (the loop no longer produces
180-deep identical pairs for it to collapse — the receipt lines are distinct events, 5 of them
in one turn at seq 25, each true).

---

## Lane P / Q samples (verbatim, for the record)
- Header: `deck152-vs-deck123` seq 20 `Your battlefield (9 permanents listed, of which 3 are creatures, 1 of them is attacking right now, 0 more able to attack right now):`
- Collapsed blockers + priciest: `deck146-vs-deck126` seq 72 (the only `most expensive to attack into:` in the corpus).
- Stack rows with live keywords: 33 `can target on the stack:` clauses, e.g. `deck125-vs-deck146` seq 5 `... Nadaar, Selfless Paladin {2}{w} (creature 3/3) [vigilance] {target text: "Vigilance -- ...`.
- `auto=choice` menus: `Choose an option for Barkchannel Pathway:` (57 renders, 0 generic).
- ANNOUNCE_X: 28 X-menus, 0 with a plan block; marker `[<- most kills at any affordable X that costs you nothing]` on 4.

---

## Prompt chars per decision kind — wave 47 → wave 48

| kind | n47 | mean47 | max47 | n48 | mean48 | max48 |
|---|---|---|---|---|---|---|
| ask | 1,342 | 10,496 | 164,397 | 1,240 | 8,162 | 27,987 |
| priority | 4,128 | 23,546 | 116,148 | 364 | 11,331 | 27,721 |
| attackers | 109 | 12,866 | 233,662 | 71 | 9,588 | 24,153 |
| blockers | 38 | 12,380 | 74,063 | 26 | 12,733 | **61,240** |
| reveal | 19 | 13,623 | 23,365 | 20 | 11,405 | 15,555 |
| bottom | 8 | 2,196 | 2,403 | 8 | 2,107 | 2,370 |

Priority mean halved and its count fell 11x (the loop windows are gone). Blockers is the one
kind whose max did not fall below 30 KB — D-1.

**Max priority windows in any single turn:** 9 (`deck123-vs-deck126`), 7, 4, 4, 4, 4, 4, 3.
**Fallbacks (5):** `deck146-vs-deck125` 80 priority (`CHOICE: 3 (Cast Acererak the Archlich)`
on a 2-row menu — a card not on the menu), `deck126-vs-deck146` 24 blockers
(`all_assignments_illegal`: `BLOCKS: B1:A2, B2:A3` single-blocking two MENACE Spiders whose A-lines
say `[menace - only a block by TWO OR MORE of your creatures counts; ...]` → heuristic blocked,
no re-ask), `deck130-vs-deck126` 30 ask (`CHOICE: 6 (Cast Rorix Bladewing)` on a 5-row menu),
`deck162-vs-deck123` 10 ask (`CHOICE: 3 (Cast Fate Unraveler)` on a 2-row menu — 4-drop with 3
mana), `deck123-vs-deck126` 29 ask (`CHOICE: 5 (Attack with all creatures)` on a 4-row cast
menu at Main-1 with 257 creatures — the model wanted combat and the menu's exit row is
`4. Cast nothing right now`, which does not say it leads to combat). Four of five are
"index past the menu + a name not on it"; the fifth is menace.

---

## NEW DEFECTS (each with a record citation and the emitter string)

**D-1 HIGH — B-row `may block` list is unranged and unfactored.** `deck126-vs-deck123` seq 15:
`- may block A1 (neither dies (blocking trigger: you gain 3, and your converter takes 3 off them)), A3 (neither dies (...)), ... A257 (...)` — 256 handles x 3 B-rows = 61,240 chars while
the A side shows `A3-A257 ... x255`. Fix (a)/(b) above. This is the last uncollapsed
enumeration on the combat screens; the 21 other blockers prompts max at 12.7 KB mean.

**D-2 MED — repeat-row count parser rejects the row's own phrasing.** 4 windows
(`deck123-vs-deck162` 28/29/31, `deck123-vs-deck126` 22) — `N=45`, and the verbatim label
echo, both → `repeat_count_missing` → silent single activation with no narration that the
count was missing. See lane N table.

**D-3 MED — out-of-menu index + unknown name goes straight to Baka.** 4/5 fallbacks this
corpus (`deck146-vs-deck125` 80, `deck130-vs-deck126` 30, `deck162-vs-deck123` 10,
`deck123-vs-deck126` 29): the reply names a card that is NOT a row (hand card not castable, or
`Attack with all creatures` at a cast menu). The reask machinery exists for validation rejects;
an index > N with no name match is the same class and should get one re-ask with `"<name>" is
not on this list`. For seq 29 specifically the exit row `Cast nothing right now` could carry
`(moves on to combat)` when the attack step is still ahead — the model's stated intent was
exactly that.

**D-4 MED — `//` face budgeting drops the whole back face with no marker.** 118 `{card text:}`
blocks end on a bare face name: `... a value X or less. // Agadeem, the Undercrypt` (44),
`... player discards that card. // Pelakka Caverns` (30), `... five or more Vampires. // Lord of
Lineage` (12), `// Moonrage Brute` (11), pathway lands (16), `// Tovolar's Packleader` (1).
The budget names the face and gives it zero text and no `...`. Not a mid-word cut (prediction
held) but an unmarked truncation: the reader cannot tell "the back face is a land with no
text" from "the back face's text was cut". Emit `// <face> (text omitted)` or at least `...`.

**D-5 LOW — the X free-kill marker sits on the cheapest row, which largest-first ordering
puts LAST; the model took row 1 anyway.** `deck130-vs-deck146` seq 47: rows `1. X = 6 {kills
THEIRS: Silverquill Silencer; YOURS: none}` ... `5. X = 2 {X pricing: same kills as X=6, for 4
less mana} [<- most kills at any affordable X that costs you nothing]`; reply `CHOICE: 1
(X = 6)`, no reasoning. 4 mana overpaid on a turn it then cast Siege-Gang with 3 floating
(seq 48). The owner ruling is largest-first; within that ruling the marker could ALSO be
echoed on the top row (`... [X=2 kills the same for 4 less mana - see row 5]`) so the
information is where the option-1 bias lands. 1/4 marker windows; the other 3 chose the marked
row (it was row 1 in all three).

**D-6 LOW — `[DRAW PRICE:]` skips `draw:X` rows.** `deck125-vs-deck162` seq 38 and 41,
`Cast Sphinx's Revelation ... each point of X gains you 1 life and draws you 1 card` under
Fate Unraveler, untagged. The X-pricing clause is the natural carrier: `... draws you 1 card
(and costs you 1 life per card while Fate Unraveler is out)`.

**D-7 LOW — menace single-block → silent Baka.** `deck126-vs-deck146` seq 24 (above). The
A-line warning is correct and explicit; the reply ignored it; `all_assignments_illegal` went to
the heuristic without the one re-ask that `multiblock_reask` gives gang conflicts. Carried
from the wave-47 "Baka menace gang-block probe" pin; now has a corpus record.

**D-8 LOW (record hygiene) — the JSONL `turn` field is the log's turn minus one.**
`deck123-vs-deck126` seq 3: `turn: 1, phase: Main phase 1` while the prompt's last log header is
`=== Turn 2 - YOUR turn ===`. Every citation that mixes "turn 10" from the field with the
narration's "Turn 11" is one off; seat files (including wave 47's "turn 10") have been
quoting the field. Emit the same 1-based number in both.

**NOT a defect (checked because the model claimed it):** `deck123-vs-deck126` seq 16 attackers,
the model's reply asserts `Human #1 ... [summoning sick - cannot attack this turn, but CAN
block]` contradicts `A2. Human #1`. The battlefield row is actually `Human #1 (1/1) [doesn't
untap during its controller's untap step]` — no summoning-sick tag; the header
`55 are creatures, 2 of them able to attack right now` was right (50 tokens made that turn).
Model-side misread, no render item.

---

## Rotation evidence (engine seat)
- Decline affordance: 0 → 214 explicit passes; 75/139 single-option windows declined.
- Loop cost: 1,871 + 1,550 + 223 priority windows → 12 + 17 + 8. Longest game 133 records.
- Wave-47 `Cast Example Card` exemplar (seat PRIORITY 3) still renders verbatim in every
  priority tail (`e.g. "CHOICE: 3 (Cast Example Card)" (a placeholder ...)`); `echo_index_conflict`
  1 this corpus — not re-raised, in band.

## Lategame specimen candidate (invariant 0)
`1787833243-ai_baka_deck123-0x56097312ac50-vs-ai_baka_deck126.jsonl` seq **31** (attackers,
11,274-char prompt for 257 attackers, reply `ATTACK: A1, A2, A3-A257`) paired with the opposing
`...deck126-0x560971e0cf50-vs-ai_baka_deck123.jsonl` seq **15** (the 61 KB blockers screen for
the same combat). One combat, both seats, both new collapses and the one that is missing.

---

## WAVE-49 DOCKET PROPOSAL (engine / UX only), ranked
1. **D-1** B-row `may block` ranged + factored parenthetical (blockers max 61 KB → ~5 KB;
   the last uncollapsed enumeration).
2. **D-2** repeat-row count grammar (`N=45`, `N = 45`, `, 45)`) + a receipt/re-ask when the
   count is missing (4 wasted trips in 2 games; the row is otherwise proven — 8/8 counted
   takes executed exactly).
3. **D-3** one re-ask for index-past-menu with an unmatched name (4/5 fallbacks), and
   `Cast nothing right now (moves on to combat)` when attackers are still ahead this turn.
4. **D-4** `//` back-face truncation marker (118 blocks).
5. **D-7** menace single-block re-ask (reuse `multiblock_reask`), then Baka.
6. **D-5** echo the free-kill marker on the top X row under the largest-first ruling.
7. **D-6** `[DRAW PRICE:]` on `draw:X` rows via the X-pricing clause.
8. **D-8** 1-based `turn` in the JSONL.
9. Watch only: attackers same-name interleave across `#n` (2/71, 4-row boards) — cheap to
   fix together with D-1's gap-grouping if that route is taken.
Carried, not re-derived: D10, D14-D19 from wave47/engine-ledger.md.

## Falsifiable predictions for wave 49 (if the docket lands in order)
- No prompt > 30 KB (blockers max was 61,240; next-largest of any kind 27,987).
- `repeat_count_missing` 0/N; every repeat-row take carries a receipt with N ≥ 2 or a
  narrated "no count named".
- Fallbacks from index-past-menu 0/N (was 4/5); corpus fallback rate stays ≤ 0.31%.
- `{card text:}` blocks ending on a bare `// <name>` 0/N (was 118).
- 21/21 natural, max priority windows per turn ≤ 10, `mana_only_windows_skipped` max ≤ 1,000.

## What I did NOT check
- Any deck seat's Set A guide predictions (not my seat).
- Whether the 8 counted repeat takes stopped EARLY for any reason — all 8 receipts read
  `ran N of the N times you named` with no parenthetical, so no early exit occurred to test.
- `NarrationCycleHolder` on a period > 1 cycle — none arose (the only long runs were the
  token receipts, which are distinct events).
- The `[repeat:]` tag's `in a row` count across a turn boundary (`deck123-vs-deck126` seq 14:
  `this turn 2` vs `in a row 3`, the third being the previous turn's upkeep activation with only
  passes in between) — plausibly intended ("no other action" excludes passes); not raised.
- Card-fact verification against primitives: no card-behaviour claim is made in this file.
- wave47 latency baselines beyond the p90/max columns above.
