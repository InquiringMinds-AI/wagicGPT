# Wave-52 seat review — ENGINE / NARRATION

Corpus `matchups-20260901-223430` (binary = hermetic `make -B` of master dddb6f62b; lanes G 8c3e10e28,
H d58aeba6b, I 4bdca64bf, J aa8509282, K b1fba8eff, L 0f7dff3b0 + the owner batches bd515aad7 /
c15106f4d / 4abcf5dd0 / the Vita batch / c1a0bf814 / 5ca11d6e2). Model qwen36-35b-a3b (FP8) on Spark
:8084. **42 seat logs, 3,384 records, 3,300 asks** (ask 2,540 / priority 535 / attackers 107 / discard 54
/ blockers 41 / reveal 15 / bottom 8) + 42 gamestart + 42 gameend. **21/21 natural** — 42/42 seats carry
`gameend`. Max seat **353** records (`deck152-vs-deck125`, 53 turns). Max prompt **31,551**
(`deck146-vs-deck152` seq 107, priority, turn 27) — **7 prompts over 30,000**, every one of them 70-86%
log window. Total inference across the corpus **79,362 s (22.0 h)**.

Every string quoted is the emitter's ACTUAL output, read with python over the JSONL (`events`,
`options_text`, `prompt`, `reply`, `chosen_text`) and `/usr/bin/grep` over the `.stderr` files. `seq` is
the record's own field. Narration counts are DEDUPED per record from `events`. Card facts checked against
`projects/mtg/bin/Res/sets/primitives/*.txt` (Overgrown Battlement, Tovolar's Packleader, Fall of the
Gavel, Lightning Greaves — all four confirmed; none contradicted a render).

---

## Health — the 15 fallbacks, each classified

`fallback` field **15 / 3,300 = 0.45 %** (wave 51: 5 / 2,129 = 0.23 %). The jump is NOT a regression in
decision quality: **9 of the 15 are re-ask classes that did not exist in wave 51** (lane J's
`plan_choice_conflict`, and `multiblock_reask`), and 8 of those 9 recovered on the next record.
**Baka actually executed 6 decisions** (`chose -1 of N` in stderr, 6 occurrences, 0.18 % of asks;
wave 51: 2 / 0.09 %). `giving this decision to the heuristic` **0** — no stale-livelock handoff at all.

| class | n | root cause | Baka? | recovered |
|---|---|---|---|---|
| `plan_choice_conflict` | 5 | **lane J D14 working as designed** — a prose verdict after the coded line. `deck146-vs-deck125` 59, 133; `deck130-vs-deck125` 48, 66, 82 | no (choice 1/2/1/1/1) | **5/5** `plan_choice_conflict_recovered`; 3 re-answers went to pass, 2 confirmed the row |
| `empty_reply` | 3 | **900 s model timeout**, latency 900,018-900,021 ms. `deck130-vs-deck162` seq 18 (turn 10 Draw), `deck126-vs-deck146` seq 1, `deck123-vs-deck130` seq 3 (both **mulligan** decisions, turn 1) | **yes ×3** | n/a — Baka answered |
| `unparsed_reply` | 3 | **model degeneration**, short latency (3.2-3.6 s), replies `'跟 \.8'`, `'method4 * is_ k ind  *'`, `' '`. `deck125-vs-deck146` 50, `deck146-vs-deck125` 282, `deck125-vs-deck152` 136 | **yes ×3** | next window answered normally |
| `multiblock_reask` | 3 | one blocker assigned to several attackers (`BLOCKS: B2:A1, B2:A2`). `deck152-vs-deck146` 76, `deck152-vs-deck162` 26, `deck126-vs-deck152` 54 | no | **2/3** (`multiblock_reask_recovered`); **1 produced ZERO blocks — D-1 below** |
| `named_row_reask` | 1 | in-range index + off-menu name (`CHOICE: 2 (Cast Idyllic Tutor)` on a 2-row Greaves/nothing menu), `deck123-vs-deck126` 70 | no | **1/1** recovered at seq 71 |

By seat: deck130 4, deck146 3, deck152 2, deck125 2, deck123 2, deck126 2, deck162 0.

`parse_note` 20: `decision_reversed_in_prose` 5, `plan_choice_conflict_recovered` 5,
`attack_last_line_taken` 2, `blocks_last_line_taken` 2 (**new**), `multiblock_reask_recovered` 2,
`stale_echo_in_range` 1, `named_row_reask_recovered` 1, `echo_index_conflict` 1, **`name_over_index` 1**,
**`repeat_count_zero_pass` 1**. `answer_replaced` 16, `commit_retracted` **0**.
stderr: `stale_livelock` **0**, `consecutive stale drops` **0**, `dropping stale async answer` 47 (never
consecutive), `cast ask NOT issued - repeat plan in progress` **124** (lane G, firing), `repeat plan
iteration` 62, `auto-passing without a model call` 2,270, `all actions pass-declined` 801,
`land-drop ask NOT issued` 942, `with no source tapped` **0**, `index_name_conflict` 0, `plan_missing` 0.
`mana_only_windows_skipped` max **459** (`deck126-vs-deck125`), sum 733. Max priority windows in one turn
**7** (wave 51: 10).

**Reply-vs-executed-row audit (whole corpus).** 3,063 replies carried a `CHOICE: n (name)` parenthetical;
the executed row's short label disagreed with the name in **1** case only — `deck146-vs-deck152` seq 91,
`CHOICE: 1 (Pass)` executed `+1: don't target any creature with Kaya the Inexorable` (D-3 below). The
wave-51 D-2 class (a pass-row label matching an edict annotation) is **gone**: 543 rows carry
`at 0 this does nothing`, **0** were taken, and no reply naming "nothing" executed one.

**Narration.** `turn` == the last `=== Turn N` header in the prompt **3,240 / 3,240**. `- Paid` receipts
**503** deduped from `events`, **0** with no source, 4 read `from mana already floating` (legitimate),
**3** still have no payee (all the `{1}{w}` Intrepid Adversary ETB class — wave 51 D-6, 7 → 3, unfixed).
280 duplicate event lines inside a single record are legitimate repeats (139 of them `- Opponent drew a
card`) that the log's own `(xN)` collapse does not reach (D-8).

**Mulligans (new-baseline record).** 34 seats kept 7. 8 seats mulliganed: 6 to 6 cards
(`deck123-vs-deck146`, `deck146-vs-deck123`, `deck146-vs-deck162`, `deck152-vs-deck123`,
`deck125-vs-deck126`, `deck152-vs-deck126`), 2 to 5 cards (`deck146-vs-deck126` two mulligans, bottomed
`Nadaar, Selfless Paladin, Lolth, Spider Queen`; `deck123-vs-deck130` two mulligans, bottomed
`Intruder Alarm, Intruder Alarm`). **No mull-to-zero and no repeated mull against a guide floor.**
⚠ Two of the 42 opening keeps were **not the model's**: `deck126-vs-deck146` seq 1 and
`deck123-vs-deck130` seq 3 timed out at 900 s and the heuristic decided the mulligan (the deck123 seat's
THIRD mulligan ask). That is an engine/observability item (D-4), not a guide question.

---

## VERDICT TABLE — Set B, adjudicated by the emitter's actual strings

| Lane | Prediction | Verdict | Count / citation |
|---|---|---|---|
| G 8c3e10e28 | `stale_livelock` 0/N (was 1) | **PASS (0)** | 0 in the JSONL and 0 in stderr; `consecutive stale drops` 0; 47 single stale drops, none consecutive. The fix's own trace fires: `AIPlayerGPT: cast ask NOT issued - repeat plan in progress (k/N done); the plan is this priority sequence's decision` **124 times** over 62 `repeat plan iteration` lines |
| G | `Cast nothing` echoes never execute an edict row carrying "does nothing" (0/N) | **PASS (0/543)** | 543 rows render `{right now: they control 0 creatures or planeswalkers - at 0 this does nothing}`; 0 taken; 0 replies naming "nothing" executed one. The wave-51 shape (`deck123-vs-deck125` 48) did not recur |
| G (E-1) | PUT seams never consume a CHOICE: line | **PASS (0/77)** | 77 reveal/discard/bottom records; **0** replies carried both `PUT:` and `CHOICE:`; every `PUT:` parsed (`reveal put N of M to option one in one reply` ×15 in stderr) |
| H d58aeba6b | `{paying this taps: <wall>, <wall>}` for a cost one wall covers 0/N; receipts naming two Battlements for ≤ their single output 0/N | **PASS (0/0 and 0/0)** | 101 `{paying this taps:` forecasts, **0** name a Battlement/Academy twice. 13 receipts name a Battlement; the tell-tale correct forms are there: `- Paid {1}{g} for Overgrown Battlement with Overgrown Battlement` (one tap, two mana — primitive `auto={T}:foreach(creature[defender]|myBattlefield) add{G}` confirmed) and `- Paid {3}{b}{b} for Sanguine Bond with Overgrown Battlement #1; Overgrown Battlement #2; Chromatic Lantern #1; Sunpetal Grove #1` (4 sources, 5 pips) |
| I 4bdca64bf | opponent-turn windows with an instant in hand and no Cast row 0/N (was 41/51); Terror-class removal resolving on the opponent's turn N > 0 | **PASS — the wave's biggest capability change** | 1,034 opponent-turn windows; **885 carry a Cast row**. Of the 943 with an instant in hand, **873 carry a Cast row**; the 70 without are `priority` (54) and target-choice `ask` (10) / `blockers` (6) windows where the casting ask is a separate record. **913 `Casting decision (…, opponent's turn)` asks** (wave 51: ~0). Removal on the opponent's turn: Path to Exile 5, Soul Shatter 4, Tribute to Hunger 3, Vanishing Verse 1, Tragic Slip 1 = **14**; counterspells Cancel 16, Essence Scatter 14, Fall of the Gavel 12 (primitive `auto=fizzle` — a counter, confirmed), Dream Fracture 10 = **52**; 97 opponent-turn casts total. **Cost side: D-2 below** |
| J aa8509282 (D6) | exact short name outranks a disagreeing index; named row executed N/N | **PASS (1/1)** | `deck126-vs-deck152` seq 45, `CHOICE: 3 (Play no land right now)` on rows `1. Play Bayou / 2. Play Savannah / 3. Play Sunpetal Grove / 4. Play no land right now` → `parse_note: echo_index_conflict;name_over_index`, executed **4** (the named row). The wave-51 `deck123-vs-deck125` seq 48 shape (index executes a dead row) **0/N** |
| J (D14) | prose verdicts after the CHOICE line trigger `plan_choice_conflict` on ANY taken row; count + what the second answer did | **PASS (5 firings, 5 recovered)** | `deck146-vs-deck125` 59 (`"So I pass Main 1, go to Main 2"` → re-ask → pass), 133 (`"I will pass"` on a `becomes beholder` take → re-ask → **same action confirmed**); `deck130-vs-deck125` 48, 66 (both `"I will pass upkeep…"` → re-ask → pass), 82 (`"I will pass this turn"` on a Pyrite Spellbomb take → re-ask → **same action confirmed**). Emitter string: `[RE-ASK] Your CHOICE line takes row N ("…") but your reply says this window is a pass ("…"). Answer again: 0 (pass) if you meant to pass, or the number of the row you want performed now.` Net: 3 of 5 were genuine reversals the lane caught |
| J | `plan_missing` re-ask for a counted repeat take with no PLAN | **UNTESTED** | 0 renders, 0 in stderr; no counted repeat take arrived without a PLAN |
| C (carry) | `x0` on the repeat row = pass (`repeat_count_zero_pass`) | **PASS (1/1) — first live firing** | `deck123-vs-deck162` seq 35 `CHOICE: 2 (Create human with Thraben Doomsayer x0)` → `repeat_count_zero_pass`, executed **pass**. `ran 1 time (you named 0)` 0 |
| K b1fba8eff (D7) | Talisman/painland cast at ≤ the damage with no life clause 0/N; `- this KILLS you` | **PASS (0/N); lethal form UNTESTED** | 15 rows carry `{paying this costs you N life (Talisman of Impulse: 1 damage) - you would be at 19}` (and a 2-Talisman form `- you would be at 18`). 1 taken, at 20 life. The one Talisman self-damage narration in the corpus was preceded by a clause-carrying row (1/1). `- this KILLS you` **0 renders** — no window ever offered a life-cost row at life ≤ the cost |
| K (D9) | a self-hitting exile/bounce row taken at ≤ 1 life 0/N | **PASS (0)** | `{this hits YOUR permanent}` now reaches loyalty rows: `-3: exile non-land permanent` **15 tagged** (wave 51: 74 UNTAGGED), `-2: destroy target creature and draw two cards` 8, `-6: destroy up to three and reanimate` 1, plus damage rows 155. Wave-51 D-3 closed. 0 takes of a tagged self-hit row at any life. Residual untagged self-targets are benign by class (Equip 79, Kaya `+1: ghostform` 13, Sorin `-2: emblem` 16, `wolf of werewolf fights` first-half 17 — primitive confirms `target(other *[wolf;werewolf]|mybattlefield)`, a correct self-target) |
| K (D10) | a legendary second copy cast with no clause 0/N | **PASS (0/19)** | 19 rows, **all** carry `[legendary: you already control <name> - legend rule: casting this sends one copy to your graveyard (you choose which)]`; 4 taken with the clause in front of the model. `[already owned:` is **0 renders** — the retired literal is gone. Stackables now read `[second copy: you already control <name>; both stay on the battlefield - no legend rule]` (70 rows, 28 takes, all genuine stackables) |
| K (D10 tutor) | tutor rows warning against the half the pilot LACKS 0/N | **PASS (0/44)** | 4 variants, all state the truth: `[finds only an enchantment card - still in your library, not on your battlefield or in your hand: Sanguine Bond, Exquisite Blood]` 9, `- every enchantment left in your library is a copy of one you already control or hold: Intruder Alarm]` 26, `- none left in your library]` **9** (a real dead-row warning), and a compound `…: Sanguine Bond; also there, but you already control or hold…`. 10 takes, **0** of a "none left" row |
| K (D11) | `{castable from exile - your card, exiled by …, it costs {N} more …}` | **PARTIAL** | 17 `[from exile]` rows. Only **4** name the cause (`{castable from exile - your card, exiled by their Elite Spellbinder, which lets you cast it from there; it costs {2} more…}`); **13** fall back to `{castable from exile - a legal cast, not a dead row; it costs {2} more than printed, already counted in the cost shown}`. Pattern: the cause survives the first window or two after the exile and is then lost (`deck146-vs-deck152` seq 32 has it, seq 36/56/68/75/76/81/87/98/104 do not). D-6 below |
| L 0f7dff3b0 (D8) | ability target menus collapse to `#a-#b` | **FAIL** | `#a-#b` collapse is live on **cast-row target clauses** (97 uses, e.g. `Overgrown Battlement #1-#3 {1}{g} (creature 0/4) [defender] x3`) and on battlefield lines (`Vampire #1-#33 (4/4) … x33`) — but **not** on ability menus. `deck123-vs-deck162` seq **41 = 40 rows**, seq **42 = 39 rows** of `Equip with Lightning Greaves targeting Human #N [your battlefield] (Lightning Greaves is ALREADY attached to Thraben Doomsayer - this MOVES it to Human #N…)`; after masking `#N` there is exactly **1 distinct shape**. Worse than wave 51's 34. D-5 below |
| L (D12) | `becomes` re-taken in the same window 0/N (was 3/9) | **PASS (0)** | 138 `becomes` rows, **2 takes** corpus-wide (`deck146-vs-deck125` 134 turn 27, 296 turn 47); after each take **no `becomes` row is offered again that turn** — the next row reappears only on the seat's next turn (seq 137, turn 29) |
| L (D13) | animated land tapped for mana the turn it was animated 0/N | **PASS (0)** | Every `- Paid` receipt was cross-checked against the `You used: becomes … with <land>` narration in the same turn: **0** same-turn taps. Rows also carry `[this land is TAPPED: animated, it still cannot attack this turn]` |
| L (D15) | -N/-N rows carry DIES/SURVIVES N/N | **SPLIT: PASS on target rows (9/9), NOT APPLIED on cast rows (0/85)** | Target-choice rows: `{right now: -1/-1 - SURVIVES (toughness 2, -1/-1 more kills it)}` 3, `- SURVIVES (toughness 4, -3/-3 more kills it)` 1, `- DIES` 1, `-13/-13 - DIES` 4. The 85 **Tragic Slip cast rows** show only `{right now: -1/-1 (no creature has died this turn, so Morbid does NOT apply)}` — correct and useful, but no per-target verdict, so the spell is chosen before the verdict exists. D-7 below |
| L (D16) | fetch rows say "makes no mana" | **PASS (69)** | `{this land makes no mana - crack it for a land: your untapped mana sources go from 0 to 1}` |
| L (D17) | `{spends K of M}` on cast rows | **PASS (546)** | 685 uses total: **546 on `Cast …` rows**, 126 on ability rows, 64 on `becomes` rows. Companion `{leaves K of M}` 2,390 |
| L | longest non-log prompt segment < 30 KB, no 20+-row Equip block | **PASS on size, FAIL on the Equip block** | Max non-log segment **11,361** (`deck123-vs-deck126` seq 47, a 47-row reveal). The log window is now hard-capped: 165 prompts carry `(...earlier events trimmed…)`, log segment max **25,431**, p90 21,953, p50 8,452. Every prompt over 30 KB = a capped log + 5-6 KB of rows. But the 39/40-row Equip menu above is a 20+-row block |
| bd515aad7 | adventure/omen SORCERY half at instant speed 0/N; instant halves on the opponent's turn N > 0 | **UNTESTED** | No adventure / split / aftermath / morph card is in the seven-deck pool (checked the deck lists). The 278 `// ` rows are MDFC/pathway display toggles, auto-passed 1,537 times without a model call |
| c15106f4d | Calamity Bearer damage doubled at normal timing; Drake Hatcher; morph menus priced | **UNTESTED** | Neither card is in any pool deck |
| 4abcf5dd0 | upkeep-trigger reveals fire N/N when the phase is auto-skipped | **UNTESTED as stated** | No Delver-class upkeep reveal in the pool. Upkeep triggers that ARE in the pool fire normally (`- Phase: Upkeep / - You drew a card` from Staff of Nin across the 69-turn game) |
| Vita batch | an equip activation menu growing per cycle 0/N; evolve off an enters-with-counters creature N/N | **equip PASS (0), evolve UNTESTED** | The Greaves menus track the creature count exactly (40 targets ⇒ 40 rows, 39 ⇒ 39) with **one row per target** — no double registration, no per-cycle growth. No evolve / Hapatra / Wilt-Leaf / Hurkyl / Pridemate card in the pool |
| c1a0bf814 | forecast-vs-`- Paid` source mismatch 0/N (AI seat must pass preserveOptions=false) | **PASS (0/10)** | 10 takes carried a `{paying this taps: …}` forecast with a following receipt; in **10/10** the forecast set is a subset of the receipt's sources (the forecast lists only the creature sources it must warn about; the lands are added by the same plan). E.g. `{paying this taps: Luminarch Aspirant ×3 - they cannot attack this turn}` → `- Paid {4}{g}{w} for Katilda, Dawnhart Prime with Overgrown Farmland; Hengegate Pathway; Luminarch Aspirant #1; #2; #3; Deserted Beach`. The human-seat option-preserving autotap did not leak into the AI seat |
| 5ca11d6e2 | a Baka-fallback pump aimed at the opponent's creature 0/N | **UNTESTED** | Baka executed exactly 6 decisions (2 mulligans, 4 casting decisions). No pump/ability-target decision ever reached the heuristic |
| carry | 21/21 natural; no crash/timeout adjudication | **PASS** | 42/42 `gameend`; results.tsv has a winner for all 21 |
| carry | Baka-executed ≤ 0.31 % | **PASS (0.18 %)** | 6 / 3,300 |

---

## Prompt chars per decision kind — wave 51 → wave 52

| kind | n51 | mean51 | max51 | n52 | mean52 | p90-52 | max52 | p90 lat 51→52 (s) | max lat 52 |
|---|---|---|---|---|---|---|---|---|---|
| ask | 1,493 | 10,418 | 29,355 | **2,540** | 12,594 | 24,638 | 29,581 | 56.8 → **44.0** | 900.0 |
| priority | 445 | 14,302 | 33,536 | 535 | 14,935 | 25,937 | **31,551** | 59.3 → **53.3** | 393.0 |
| attackers | 110 | 11,324 | 26,911 | 107 | 12,497 | 21,735 | 30,277 | 62.9 → **61.0** | 382.1 |
| blockers | 31 | 10,374 | 20,779 | 41 | 14,350 | 25,354 | 30,307 | 79.1 → 80.8 | 377.7 |
| discard | 31 | 12,506 | 27,450 | 54 | 16,579 | 26,421 | 28,071 | 123.0 → **41.8** | 57.7 |
| reveal | 13 | 14,544 | 19,789 | 15 | 15,559 | 22,196 | 26,658 | 43.2 → 63.6 | 205.4 |
| bottom | 6 | 2,266 | 2,469 | 8 | 2,458 | 2,590 | 2,667 | 70.2 → 97.0 | 97.5 |

Ask volume **+70 %** (1,493 → 2,540) — lane I's opponent-turn casting windows are essentially all of it.
Means are up 6-33 % but the **tail came down**: max prompt 33,536 → 31,551 and only 7 prompts over 30 KB
(wave 51: 3, but out of half as many asks). 330 prompts exceed 25,000. Mean log share **69 %**; every
prompt above 29 KB is 70-86 % log. The log cap is doing its job (trim at ~21-25.4 KB, 165 firings), so
the remaining tail is 25 KB of capped log plus 5-6 KB of rows — i.e. the D23 log-compaction decision is
now the only lever on it, and it is the owner's.

---

## The three long games

| game | turns | seat records | winner | what kept it open |
|---|---|---|---|---|
| **125 v 130** | 69 | deck125 158 / deck130 117 | deck125, 25 to −5 | deck130 (red LD/burn) ran its hand to **zero for 36 of its 117 windows** while holding 11-17 Mountains; its only recurring action was `Deal 2 damage with Pyrite Spellbomb` / a 7-mana `Put a card into hand with Hammer of Bogardan` loop it argued itself out of three times (seq 48, 66, 82 — all three became `plan_choice_conflict` re-asks). deck125 stalled behind counters, then **hard-cast Emrakul, the Aeons Torn for {15} on turn 69** and swung. Nothing engine-side held it open: max prompt in the whole game was only 25,596 |
| **152 v 125** | 53 | deck152 **353** / deck125 150 | deck125, 28 to 0 | a 2.4:1 window asymmetry — the creature seat gets a priority stop per body per phase. deck152 spent **262 of its 353 decisions on "Cast nothing / pass"** and 4,566 s of inference; deck125 finished it with two Staff of Nin pings (`- Opponent's Staff of Nin dealt 1 damage to you (now 1)`) |
| **146 v 125** | 51 | deck146 **337** / deck125 148 | deck125, 53 to −10 | same shape: 258/337 no-ops, 5,659 s of inference for the losing seat vs 1,000 s for the winner. This game also carries the corpus's worst **stale PLAN**: one plan string was echoed unchanged for **51 consecutive windows** across turns 34-38 (seq 177-227) while the opponent went 27 → 35 life (D-9) |

Across the corpus the pattern is one-sided: deck125's stall-then-Emrakul/Staff inevitability produces
both the long games and the record counts. That is the deck seats' business; the engine consequence is
that **decision volume scales with the number of permanents, not with turns**.

---

## NEW DEFECTS (record + emitter string)

**D-1 HIGH — a `multiblock_reask` can end with ZERO blocks declared, silently.**
`deck152-vs-deck162` seq 26 (turn 14, blockers, life 24): rows `A1. Fate Unraveler #1 (3/4) deals 3 /
A2. Fate Unraveler #2 (3/4) deals 3 / B1. Katilda (2/2) … / B2. Elite Spellbinder (6/4) [flying] - may
block A1, A2 (all: you kill it, your blocker lives) / B3. Luminarch Aspirant #3 (1/1) …`; reply
`BLOCKS: B2:A1, B2:A2`. Record carries `fallback: multiblock_reask`, `dropped_assignments: 1`, **no
`chosen_text`**, and **no follower record**. stderr `game-152v162-1788320098.stderr:1123`
`AIPlayerGPT: one-blocker-many-attackers reply -> re-asking once` is **not** followed by a
`declared blocks from N assignment(s) in one reply` line — the other two re-asks in the corpus are
(`game-146v152:3539 → 3556`, `game-152v126:2354 → 2366`). Outcome at seq 27's `events`:
`- Phase: Combat damage / - Opponent's Fate Unraveler dealt 3 damage to you (now 21) / - Opponent's Fate
Unraveler dealt 3 damage to you (now 18)` — **both attackers unblocked**, although the reply's FIRST
assignment (`B2:A1`) was legal and would have killed a 3/4 for free. The seat went 24 → 18 → 3 and lost
the game. Two sub-defects: (a) an illegal duplicate should cost the duplicate, not the whole assignment;
(b) the re-ask's model call is **not logged at all** when it yields no assignment — the JSONL has no
record of what the second answer was.
Fix shape: keep the first legal assignment per blocker and drop only the extras (the parser already
counts them — `dropped_assignments`), and always write the re-ask record.

**D-2 HIGH (cost, not correctness) — the opponent-turn cast ask fires at every phase stop, and 657 of
913 of them re-present a menu identical to the previous window in the same turn.**
Lane I is a genuine capability win (97 opponent-turn casts, 14 removal + 52 counters that wave 51 could
not make). The cost: **913 opponent-turn casting windows, 97 acted (10.6 %), 11,343 s of inference
(14 % of the corpus's 22 h)**, versus 1,420 own-turn windows at 55.8 % acted.

| opp-turn phase | windows | acted | inference s |
|---|---|---|---|
| Main phase 1 | 197 | 59 | 3,467 |
| Upkeep | 183 | 21 | 1,761 |
| Draw | 99 | 8 | 1,248 |
| End | 78 | **0** | 724 |
| Main phase 2 | 77 | 6 | 897 |
| Combat begins | 71 | 3 | 846 |
| Cleanup | 68 | **0** | 739 |
| Attackers | 67 | **0** | 761 |
| Combat ends | 67 | **0** | 763 |
| Blockers / Combat damage | 6 | **0** | 137 |

**286 windows across five phases produced zero casts and cost 2,924 s.** 657 windows presented an
options list byte-identical to the immediately preceding window of the same turn whose answer had been
`Cast nothing` (7,442 s); restricting to windows where nothing but `- Phase:` lines happened since the
last ask still gives **401 windows / 4,432 s**. Honest counter-evidence: a strict "identical menu ⇒ reuse
the pass" cache would have suppressed **5** real casts (3 under the stricter form) — the model treats
phase progression as information (`deck130-vs-deck152` seq 13 declined Spark Spray at Upkeep/Draw/Main 1
and cast it at Combat begins). So the lever is not a blind cache. Options, in the doctrine's terms
(legality unconstrained, choice preserved, inference efficient): let the pilot answer *when* it wants
priority for the rest of the opponent's turn (a `hold until <stop>` answer that the engine honours and
re-opens on any board change), or skip only the stops that no reply has ever used AND where the board is
unchanged. Repro for the raw shape: `deck130-vs-deck152` seq 7/9/11/12 (four identical Spark Spray menus,
four `Cast nothing`), `deck146-vs-deck125` seq 216-227 (12 consecutive identical windows).

**D-3 MED — an off-menu `(Pass)` parenthetical with an in-range index executes the index, with no
re-ask and no note.** `deck146-vs-deck152` seq 91, priority, turn 23: reply
`CHOICE: 1 (Pass)\nPLAN: Activate Kaya's -3 to exile Tovolar's Packleader…` on rows
`1. +1: don't target any creature with Kaya the Inexorable … / 2. +1: target creature gets ghostform …
/ 3. Flip Side … / 4. Flip Side …` plus the standard `0. Pass priority`. Executed row 1 — seq 92's
`events` confirm `- You used: +1: don't target any creature with Kaya the Inexorable`. The name "Pass" is
the label of row **0**, which is on the menu; the lane-C/J machinery treats it as off-menu and lets the
index win instead of re-asking or preferring the pass row. Consequence here was small (a wasted +1 and a
plan the model then abandoned), but it is the only mis-execution in 3,063 parentheticals, so it is the
whole residual of the class. Fix: treat `pass` / `Pass priority` / `0` as a reserved echo that resolves
to the pass row.

**D-4 MED — a 900 s model timeout is invisible in stderr and can decide a mulligan.** Three records carry
`fallback: empty_reply` with `latency_ms` 900,018-900,021 and `reply: ""`; the stderr shows only
`… -> chose -1 of N`, with no timeout line and no retry. Two of the three were **mulligan decisions**
(`deck126-vs-deck146` seq 1 — the opening keep; `deck123-vs-deck130` seq 3 — the third mulligan ask,
after which the seat bottomed 2 and kept 5). All three occurred while 21 games were in flight, so the
proximate cause is server queueing, not the engine — but the engine gives the reviewer no way to
distinguish "the model refused" from "the model never answered", and a silent heuristic mulligan is a
game-shaping decision. Fix: log the timeout explicitly (`AIPlayerGPT: no reply after Ns - heuristic`),
stamp it in the record (`fallback: timeout`), and consider one retry before the handoff. Exactly **one**
record in the corpus carries a `retry` field (`deck125-vs-deck152` seq 126, `retry: 1`), so the retry
path exists and did not engage here.

**D-5 MED — lane L's `#a-#b` collapse does not reach ability target menus: 40 near-identical Equip rows.**
`deck123-vs-deck162` seq **41** (40 rows) and seq **42** (39 rows), turn 12. Every row is
`Equip with Lightning Greaves targeting Human #N [your battlefield] (Lightning Greaves is ALREADY attached
to Thraben Doomsayer - this MOVES it to Human #N, and Thraben Doomsayer loses what it grants) [repeat:
activated this turn 1 times already. This turn will not advance while you keep taking this option.]
{card text: "Equipped creature has haste and shroud…"}`; masking `#N` leaves **one** distinct shape.
The same collapse already works one line away, in the cast row's target clause
(`Overgrown Battlement #1-#3 {1}{g} (creature 0/4) [defender] x3`, 97 uses) and in the battlefield block
(`Vampire #1-#33 (4/4) … x33`). Primitive confirms `Lightning Greaves … auto={0}:equip` — the equip is
free, so this menu is offered every priority window while the tokens exist. Cost here is only 10.6 KB
(the log was young); in a turn-40 board it would be the whole prompt.

**D-6 MED — `{castable from exile}` loses the cause after the first window or two.** 17 rows; 4 read
`{castable from exile - your card, exiled by their Elite Spellbinder, which lets you cast it from there;
it costs {2} more…}` and 13 read the causeless `{castable from exile - a legal cast, not a dead row; it
costs {2} more than printed, already counted in the cost shown}`. Repro: `deck146-vs-deck152` seq **32**
(cause present) vs seq **36, 56, 68, 75, 76, 81, 87, 98, 104** (same card, same exile, cause gone); the
one take (seq 56, `Cast Kaya the Inexorable {5}{b}{w} [from exile]`) had no cause. Also
`deck123-vs-deck152` seq 39 (`Cast Damnation {4}{b}{b} [from exile]`) never had it. D11's prediction
asked for the cause; it ships, but it is not durable.

**D-7 MED — a -N/-N cast row prices the effect but not the outcome.** 85 Tragic Slip cast rows render
`{right now: -1/-1 (no creature has died this turn, so Morbid does NOT apply)}` and then a flat target
list; the DIES/SURVIVES verdict exists only on the **target-choice** sub-menu that comes after the spell
is already committed (9/9 correct there, e.g. `{right now: -1/-1 - SURVIVES (toughness 4, -3/-3 more
kills it)}`). Damage cast rows have the same asymmetry. The pilot therefore chooses the spell blind and
learns the verdict only once it cannot back out. Repro: `deck123-vs-deck146` seq 6 (cast row) vs
`deck123-vs-deck146` seq 19 (target row). One good counter-example already exists and could be the
template: `deck123-vs-deck125` seq 19 renders `- the only legal targets are YOUR OWN right now:` on the
cast row.

**D-8 LOW — the log's `(xN)` collapse reaches damage and repeated-activation lines, but not draws, counters or discards.** Within single records, 280 event lines
repeat verbatim; the biggest classes are `- Opponent drew a card` (139), `- Your/Opponent's Teferi, Who
Slows the Sunset lost a loyalty counter` (14), `- Your/Opponent's Lolth, Spider Queen created a 2/1 Spider
token` (12), `- Opponent discarded Supreme Verdict` (5). The collapse already exists for damage and for repeated activations
(`- Opponent's Fate Unraveler dealt 3 damage to Intrepid Adversary (x3)`, `- Opponent used: Create vampire
with Bloodline Keeper (x33)`) — it simply does not reach these classes. At a 69 % mean log share this
is free prompt space.

**D-9 LOW/MED — the PLAN block has no age, and can be 51 windows stale.** `deck146-vs-deck125` seq
177-227 (turns 34-38) all render `YOUR PLAN (as you last stated it): nothing right now: entry 1 Soul
Shatter was on this menu; it did not match because the opponent controls 0 creatures or planeswalkers, so
it does nothing. I have no other spells to cast. I will pass the turn.` — the model's own text, first
stated at seq 163 (turn 32) and never refreshed, while the opponent went 27 → 35 life and the seat 21 →
18. `plan_echo_count` (the record field) reaches **51**; 337 of 1,410 records carry an echo ≥ 10, mean
6.5. The render is honest but gives the pilot no staleness cue. Cheap fix: stamp it —
`YOUR PLAN (as you last stated it, N windows ago on turn T)`.

**D-10 LOW — a blocker handle used in the attacker slot is dropped silently.** `deck126-vs-deck162` seq
21: reply `BLOCKS: B1:A1, B2:A1, B3:B1, B4:B1, B5:B1`, `dropped_assignments: 6`, **no fallback and no
parse_note**. The engine executed the two legal walls
(`- You declared blockers: Overgrown Battlement blocks Fate Unraveler; Overgrown Battlement blocks Fate
Unraveler`) and silently discarded the three lifelink Vampires the model meant to add. Correct in effect
here; but it is the same family as D-1 and it is not re-asked or noted.

**D-11 LOW — receipts without a payee, unfixed.** 3 remain, all the Intrepid Adversary
pay-any-number-of-times ETB: `- Paid {1}{w} with Deserted Beach; Overgrown Farmland #2`
(`deck152-vs-deck123` 15, `deck152-vs-deck146` 16, `deck152-vs-deck162` 10). Wave 51 had 7; the class is
narrower, not closed.

**Observed, not defects.** `answer_replaced` 16 — all last-clean-line reads over a reasoning body; two
that look alarming in isolation are correct (`deck130-vs-deck152` seq 17 and `deck126-vs-deck123` seq 62
both open `CHOICE: 2 (Cast nothing right now)` and close, hundreds of words later, with
`CHOICE: 1 (Cast Talisman of Impulse)` / `So CHOICE: 1 (Cast Tribute to Hunger)`; the engine took the
last coded line and the events confirm it). `blocks_last_line_taken` 2 and `attack_last_line_taken` 2 are
the same mechanism on combat replies. `stale_echo_in_range` 1 (the recovered `named_row_reask`).
The `wolf of werewolf fights` two-step target flow (`deck152-vs-deck146` seq 49 → 50 → the fight
resolving in seq 51's events) is correct against the primitive; it simply carries no fight forecast on
either half, which is the D-7 gap in another costume.

---

## Lategame specimen for the owner's per-wave prompt review (invariant 0)

**Keep the harvest default as the "typical lategame" exhibit** —
`1788320112-ai_baka_deck125-0x55ded94e0310-vs-ai_baka_deck130.jsonl` seq **149**, turn **70**, Main
phase 1, 22,643 chars, 4 rows (already written to `wave52/lategame-specimen.txt`). It is the deepest turn
in the corpus and it shows three of this wave's new forms in one screen: `[second copy: you already
control Staff of Nin; both stay on the battlefield - no legend rule]`, `{X pricing: max affordable X=12
(15 mana total); each point of X gains you 1 life and draws you 1 card}`, and the self-hit form on a
sweeper `{right now: destroys 0 of their creatures (0 without a restriction against attacking), 1 of
yours}`.

**Add one render-dense companion**, because the default's board is small and it exercises 4 annotation
families where the corpus's best window exercises 8:
`1788320091-ai_baka_deck123-0x55b67b2f87a0-vs-ai_baka_deck126.jsonl` seq **88**, turn **26**, Main phase 1,
15,761 chars, 6 rows, seat at **5 life vs 29** with **34 creatures**. It carries the `#1-#33 x33`
battlefield collapse, the `LIFE-TO-DAMAGE CONVERTER` block for the opponent's Sanguine Bond, the
`{effect (each copy of this card does this): …}` form, `[finds only an enchantment card - …]`, the Morbid
clause, `{target text:}` and `{card text:}` — i.e. most of what the wave changed, at a size a human can
actually read.

**If the owner wants the size ceiling instead:**
`1788320088-ai_baka_deck146-0x5560cfe861d0-vs-ai_baka_deck152.jsonl` seq **107** (31,551 chars, turn 27,
priority, 79 % log) — the corpus maximum, and the exhibit for the D23 log question.

---

## WAVE-53 DOCKET PROPOSAL (engine / UX only), ranked

1. **D-1** blockers: a `multiblock_reask` that yields no assignment must not silently declare no blocks —
   keep the first legal assignment per blocker, drop only the extras, and always write the re-ask record.
   Repro: `deck152-vs-deck162` seq 26 + `game-152v162-1788320098.stderr:1123`. Cost: a free 6/4-kills-3/4
   block and the game.
2. **D-2** opponent-turn window economy: 913 windows, 97 acts, 11,343 s; 286 windows at five phases with
   **zero** acts ever. Give the pilot a way to hold priority for the rest of the opponent's turn (re-opened
   on any board change) rather than answering the same menu 3-12 times. Repro: `deck130-vs-deck152` 7/9/11/12;
   `deck146-vs-deck125` 216-227. ⚠ a blind identical-menu cache would have suppressed 5 real casts.
3. **D-5** collapse ability target menus the way cast-row target clauses already collapse
   (`deck123-vs-deck162` seq 41: 40 rows, 1 distinct shape).
4. **D-3** reserve the `pass` / `Pass priority` echo so an off-menu `(Pass)` parenthetical cannot let a
   disagreeing index execute a loyalty ability (`deck146-vs-deck152` seq 91).
5. **D-4** make the 900 s timeout visible (`fallback: timeout`, a stderr line, and one retry before the
   heuristic) — two opening-hand decisions were made by Baka with no trace.
6. **D-7** carry DIES/SURVIVES (and the fight forecast) on the CAST row's target list, not only on the
   post-commit target menu (`deck123-vs-deck146` seq 6 vs 19).
7. **D-6** make `{castable from exile - …, exiled by <source>}` durable (4/17 today).
8. **D-9** stamp the PLAN with its age (`plan_echo_count` reached 51).
9. **D-8** extend the `(xN)` log collapse to draws / loyalty counters / discards (139 `- Opponent drew a card` repeats).
10. **D-10** note or re-ask a `Bn:Bm` assignment instead of silently dropping it; **D-11** payee on the
    Intrepid Adversary receipt.
11. Watch (rendered correctly, deck seats' call): 3 takes on a `- SURVIVES` row (down from 4), 4 legendary
    second-copy casts made **with** the legend clause in front of the model, 0/78 opponent End-step casts.

---

## Falsifiable predictions for wave 53 (if the docket lands in order)

- `multiblock_reask` records with no follower and no `chosen_text`: **0/N**; every one-blocker-many-attackers
  reply is followed in stderr by `declared blocks from N assignment(s)`.
- Opponent-turn casting windows per game **down ≥ 30 %** with opponent-turn casts held at **≥ 90 %** of this
  wave's 97; the End / Cleanup / Attackers / Combat-ends stops contribute **0** acts again.
- No ability target menu with **> 12** rows of the same verb+source over `#N` handles; max non-log prompt
  segment stays **< 12 KB**.
- A `CHOICE: n (Pass)` reply on a menu carrying `0. Pass priority` executes the pass row **N/N**, or is
  re-asked; total name-vs-executed-row mismatches **≤ 1 / 3,000** parentheticals.
- Every 900 s non-answer is stamped `fallback: timeout` and carries a stderr line: **N/N**; Baka-executed
  mulligans **0**.
- Cast rows with a target list carry a per-target DIES/SURVIVES (or an explicit "verdict on the next ask"):
  **N/N** for -N/-N and damage spells.
- `{castable from exile}` naming the exiling source: **17/17**.
- Holding from this wave: `stale_livelock` 0; `with no source tapped` 0; `turn` == narration turn N/N;
  `- Paid` receipts with no source 0; `[already owned:` 0; legendary casts without the legend clause 0;
  animated land tapped the turn it was animated 0; `becomes` re-taken in the same window 0; Baka-executed
  fallbacks ≤ 0.20 %.

---

## What I did NOT check

- **Any Set A guide prediction, and any deck-strategy call.** Mulligans are recorded, not adjudicated; the
  two Baka-decided mulligans are reported as an engine item, not a guide question.
- **The guide-literal drift task** (deck123/deck126 re-keying) — that belongs to those decks' seats. I only
  confirmed the emitter side: `[already owned:` and `[from exile]`-without-clause are **0** and **0**
  respectively as bare literals; the live forms are `[legendary: …]`, `[second copy: …]`,
  `[finds only …]` and `{castable from exile - …}`.
- **The Vita/owner batches that have no pool card**: adventure/split/aftermath halves, Calamity Bearer,
  Drake Hatcher, Delver, Hapatra, Wilt-Leaf, Envoy bestow, Hurkyl, Pridemate, evolve, morph. I verified
  their absence from the seven deck lists rather than from the corpus alone; if the owner wants those
  lanes gated, the pool needs a deck that plays them.
- **The 5ca11d6e2 Baka pump-targeting fix** — the heuristic never received an ability-target decision.
- **D-1's fix site**: I read the stderr trace and the record fields only. I did not open `AIPlayerGPT.cpp`,
  build, or run wagic.
- **Lane B (autotap optimality) was not re-audited this wave** beyond the forecast-vs-receipt subset check
  (10/10) and the no-source / no-payee counts; I did not redo wave 51's exhaustive pip-matching over all
  503 receipts.
- Whether the 47 single stale drops share the wave-51 cast-ask-during-upkeep shape (none consecutive; not
  traced).
- `User/transcripts/` (the 1245b7d68 dev-build output) — the carry-list says it is not a review input this
  wave.
