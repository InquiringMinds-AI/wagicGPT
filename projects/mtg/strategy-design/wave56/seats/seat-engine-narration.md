# Wave-56 seat review — ENGINE / NARRATION

Corpus `matchups-20260903-120952` (binary = hermetic `make -B` of master **d0515fae3**; wave-56 step-1
lanes A 7a2e243c7, B 5c391be1e, C 7193e4bb5, D e60a7c15d, E 039af56c6). Model qwen36-35b-a3b on
Spark :8084. **42 seat logs, 2,854 records** = 42 `gamestart` + 42 `gameend` + 6 `recovery` +
2 `wall_miss` + **2,762 decisions** (ask 2,127 / priority 456 / attackers 88 / discard 55 /
blockers 25 / reveal 9 / bottom 2). **21/21 natural, 0 SEGV, 0 timeouts, 0 draws.** Recorded
inference **21.46 h = 28.0 s per decision**. Max prompt **26,298**; prompts > 30 K: **0**.

Every string quoted is the emitter's ACTUAL output, read with python over the JSONL (`prompt`,
`options_text`, `reply`, `chosen_text`, gameend fields) and `/usr/bin/grep` over the `.stderr`
files. **All row counts are taken from the rendered `prompt`** (numbered `N. ` lines), never from
the `options` array. HOLD takes are PREFIX-matched (the wave-55 boundary-pass rule — and it matters
again this corpus: exact-match reads 208 where the true figure is **320**). Card facts verified
against `projects/mtg/bin/Res/sets/primitives/`: Silverquill Silencer (`borderline.txt:102764`),
Sphinx's Revelation, Starstorm, Spark Spray, Staff of Nin. Read-only throughout: no build, no run,
no git, no edit under `bin/Res` or `src/`.

---

## 0. Headline

**The wave's biggest single correctness win is D2, and it decided three games.** With
`AIAction::targetedSeat()` in place, `they would be at K` is right on **145 / 145** ability rows
(wave 55: wrong on 238 / 248, all reading the garbage constant 166), and for the first time an
ability menu could print a LETHAL face row: **4 rows across 3 seats read
`{right now: takes 1 damage - they would be at 0; THIS WINS THE GAME}` and the model took the row
in 3 of 3 games — winning each one** (`125v123` seq 166 turn 33, `126v130` seq 95 turn 27,
`125v130` seq 115 turn 61). Under the wave-55 render those same windows would have offered
`they would be at 165`.

**Two harvest numbers are wrong and are corrected here.** (a) **D8 did NOT fail on arrival.** The
harvest read 21 rows / 0 takes / 0 arrivals; counted from the rendered prompt the corpus carries
**65 `PLAY THIS AS A LAND` rows in 62 records, 6 takes, and back faces that ARRIVED** — `Agadeem,
the Undercrypt` (146v125 t15, 146v152 t9), `Emeria, Shattered Skyclave` (146v162 t9),
`Pelakka Caverns` (146v126) for deck146, and Boulderloft / Branchloft / Hengegate / Barkchannel
Pathway back faces for deck152 — with `Flip Side` takes **0** (was 11) and **0 turns with two land
drops**. D8 is a clean PASS on both arms. (b) **The phase-7 `only display-toggle` line is NOT the
livelock**: 760 lines corpus-wide, but the longest CONSECUTIVE run in any game is **4** (146v126
263 lines in 187 runs, 146v152 156/121, 146v130 158/111, 146v125 56/42). It is per-window churn of
≤ 4 auto-passes, not a phase that stops advancing.

---

## 1. Set-B adjudication — every prediction, by the emitter's actual strings

`#188` three-number audits appear as **old renders / new renders / takes**.

| Lane | Prediction | Verdict | Evidence |
|---|---|---|---|
| **A D1** | byte-identical same-turn re-asks after a HOLD take **< 20 of N** (was 113/282) | **PASS (14 / 320)** | HOLD row **2,016 renders / 320 takes (15.9 %)**, prefix-matched. My predicate: for each take, the next same-turn decision whose full rendered row tuple is byte-identical. 14 firings, listed §5. Harvest's 10 is the same class on a stricter adjacency rule |
| A D1 | identical-declined runs **< 15 %**, max run **< 10**, inference in runs **< 6 %** | **share PASS (14.99 %), max FAIL (13), inference PASS (5.5 %)** | Wave-53 predicate (key `(turn, options_text)`, run ≥ 3, a HOLD take breaks the run): **414 / 2,762 = 14.99 %, 68 runs, max 13, 1.18 h of 21.46 h = 5.5 %**. #208 both denominators: **own-turn 47 / 1,930 = 2.4 % (max run 9)**, **opponent-turn 367 / 832 = 44.1 % (max run 13)** — vs wave 55's opponent-turn 57.8 %, a **−13.7 pt** genuine improvement |
| A D1 | decisions inside one mandatory life-loss loop phase **< 5** | **UNTESTED (no window)** | No mandatory life-loss loop arose. Largest run inside one `(turn, phase)` anywhere: **11** (`125v126`), an ordinary priority seam |
| A D1 | stated risk: games lost while a hold stood with a legal answer — **report N** | **N = 0** | 21 lost seats; 9 took a HOLD in their last two turns. Only `130v162` also had lethal-INCOMING windows after a hold (seq 90/91, turn 11, `2 attackers, 8 unblocked - you would be at -6; this KILLS you`) — and **the seat was still asked at both**, with the full menu, after taking the hold at seq 82. The hold suppressed nothing lethal anywhere in the corpus |
| **A D18** | `dropping stale async answer` **< 20** (was 43) | **FAIL (65)** | See §2. The rescue line `consuming an in-flight answer whose prompt text drifted (same seam, turn, phase and board)` **DID fire — 8 times** |
| A D18 | 0 remaining drops preceded by a draw/phase resolution on an **unchanged** seam | **PASS (0)** | 65/65 drops are the turn-flip seam alternation: **44 land-drop arm, 21 cast arm**; 55/65 have a `10DrawAction` in between (the hand changed), **0** are phase-only-on-unchanged-board |
| **A D16** | `plan_choice_conflict` **≤ 4** (was 9); the narrowed shape stamps instead | **PASS (0)** | `plan_choice_conflict` **0**, `plan_choice_conflict_narrowed` **14**. I read 6 of the 14 replies: in every one the prose names only the row the CHOICE took (or reasons about it), so **0 genuine rivals were suppressed**. ⚠ INSTRUMENT: the promised BROAD census `decision_reversed_in_prose` renders **0** (wave 55: 14) — the narrowed stamp appears to have replaced it, not to sit beside it. Audit: **0 / 14 / —** |
| **A D11** | plan echo max **< 60** (was 143); over-40 served plans **0** | **PASS (max 20, p90 3, over-40 0)** — *but the mechanism never fired* | 2,635 served plan blocks. The withdrawal sentence `is withdrawn (you stated it,` renders **0 / 0 / —**, and there is **no window in which either rule could fire**: 0 records with a plan older than 40, and 0 records where the menu-diff note (`"<card>" is no longer on your menu`, **473 renders**) fires on a plan more than 4 windows old. The improvement is a consequence of shorter games / fewer echo chains, not of D11. Both D11 rules are effectively **UNTESTED** |
| **A D4** | records with `choice >= 0` and empty `chosen_text` **0** (was 9) | **PASS (0)** on the first half; **FAIL (0 / 6)** on the second | No record with `choice >= 0` lacks `chosen_text`. But the promised `<refused: ...>` value renders **0 times**: all 6 `choice: -1` records (3 `named_row_reask`, 1 `stale_echo`, 2 `wall_miss_unrecorded`) still carry **no** `chosen_text` at all. ⚠ AND the boundary-pass defect is UNFIXED: **112 of 320 HOLD takes carry a `chosen_text` truncated at `…you give up no cast)`**, dropping the `{taking this row skips…}` tail — see MED #1 |
| **B D2** | ability rows with `K != opp_life - damage` **0 of N** (was 238/248); a lethal face row ≥ 1 | **PASS (0 / 145), lethal 4 rows / 3 takes / 3 wins** | See §0. `at 164` / `at 165` render **0**. Audit: **0 / 145 / 99** |
| **B D6** | opponent-combat windows with attackers and no incoming total **0 of N** (was 50/50) | **PASS (0 / 38)** | Counted on Blockers-phase opponent-turn windows: **29 / 29 carry the line**. Counted on ALL opponent-turn combat-phase windows carrying an attacking marker: **38 / 38**. The 85 opponent-turn combat-phase windows without the line have **no declared attacker** (Attackers-phase pre-declaration, Combat-ends post-damage). Arithmetic checked against `my_life`: **38/38 correct**, and the `; this KILLS you` flag matches `K <= 0` on **38/38**. Audit: **0 / 38 / 0** |
| **B D10** | prompts whose opponent block carries no untapped-source count **0** | **PASS (0 / 2,716)** | Literal (from lane-B.md): `Their untapped sources: N (colours they could make: {u}{w})`. **2,716 / 2,716** prompts carrying a `CURRENT SITUATION` block have it; the tapped-out form `Their untapped sources: 0` renders **576**. Audit: **0 / 2,716 / —** |
| **B D9** | plans asserting an enumerating clause removes > 1 object **0 of N** | **PASS (1 / 1) — thin** | The new literal `they sacrifice ONE of these 2, their choice - all tied at MV 3 (their highest)` renders **once** (`146v152` seq 30, Soul Shatter) and was taken; that reply's PLAN says "either Elite Spellbinder **or** Brutal Cathar" — singular. The 127 renders of `- they choose which one` are a DIFFERENT emitter (the Tribute to Hunger / Devour Flesh multi-creature verdict), untouched by D9. Audit: **127 (other emitter) / 1 / 1** |
| **B D13** | B-lines whose gain prints INSIDE the survival verdict **0** | **FAIL (10 / 36)** | The new clauses ship and are correct — `{blocking trigger, this combat: you may gain 2}` **22 occurrences / 6 records**, `{after this combat: you control N fewer blocker - M available now, …}` **26 / 18** — but 10 of the 36 rendered B-lines still nest a gain in the verdict. 9 are the ATTACKER's lifelink (`(you kill it, your blocker lives (lifelink: they gain 1 from this block only…))`), arguably outside D13's out-params; **1 is squarely the predicted class**: `126v123` seq 37 `B7. Vampire (1/1) [lifelink…] - may block A1 (your blocker dies, attacker lives (lifelink: you gain 1))` — an OWN gain, nested, with no un-nested tag |
| **B D14** | equipment casts taken at `of which 0 are creatures` **0** (was 1/3) | **FAIL (1 / 62)** — rate 33 % → 1.6 % | `{right now: you control 0 creatures - this equips nothing}` renders on **62 rows / 62 records**; the one take is `123v125` seq 45, Lightning Greaves at 7 untapped sources. The render told the truth; this is now a deck123 guide item, not a render item. Audit: **0 / 62 / 1** |
| **B D15** | takes of the dearer of two same-card/same-verdict rows **0** (was 1/1) | **UNTESTED (N = 0)** | `{same effect as row N, for K more mana}` renders **0 / 0 / 0**, and I confirmed the WINDOW never arose: **0 menus** carry two `Cast <same card>` rows with an identical `{right now:}` verdict at different cost |
| **B (carry)** | menace header on 100 % of blockers windows with a menace attacker, 0 % otherwise | **PASS 3/3 — the positive arm is TESTED for the first time** | `need TWO or more blockers` renders **3 / 3** (all `126v146`: seq 32 t21, seq 37 t25, seq 41 t27), **0** elsewhere. Numbers verified: seq 32 `2 of the 3 attackers need TWO or more blockers each; you have 2 blockers, enough to complete at most 1`; seq 37 `you have 1 blocker, enough to complete none`. Both correct against the A-lines |
| **B (carry)** | `all_assignments_illegal` + its `[RE-ASK]` arm | **UNTESTED (N = 0)** | The class never fired; all 25 blockers records carry `dropped_assignments: 0` |
| **C D3** | `[RE-ASK]` notices whose quoted `CHOICE: n` differs from the row the sentence names **0 of N** (was 2/8) | **UNTESTED for the fix (0 of the CHOICE-line form arose)** | **3** `[RE-ASK]` notices, all three the OTHER form: `[RE-ASK] "Cast Thraben Doomsayer" is not on this list. Answer with a number from 1 to 5.` (`123v146` 15, `126v130` 14, `126v130` 60). The `Your CHOICE line takes row N ("CHOICE: n …")` shape — the one D3 rewrote — **never rendered**. 0/0 mismatches is vacuous. `plan_choice_conflict_recovered` **0** |
| **C D12** | `reveal_stall` on a reveal answered inside the poll-churn floor **0** (was 12/12); wait fields on 100 % | **PASS (0 / 9 and 9 / 9)** | 9 reveals; every one carries `reveal_wait_ticks` + `reveal_wait_secs` and **none** carries `reveal_stall`. `_secs` tracks `latency_ms/1000` to within 1 s on **9/9** (24↔23.4, 48↔48.4, 26↔25.7, 16↔16.3, 42↔41.3, 58↔58.2, 38↔38.0, 58↔58.3, 21↔21.1); ticks are uncorrelated with seconds (3,797↔21 s vs 15,475↔48 s), the structural signature working. Max `_secs` **58**, floor 2,700 |
| **C D7 (a)** | X rows carrying no mana-fit clause **0** (was 11 menus of 11) | **FAIL (40 / 40 X rows, 9 / 9 menus)** | 9 ANNOUNCE_X menus, 40 `X = n` rows: **not one** carries `{leaves N sources}` or `{taps you out}`. Lane C's own "D7(a) has never been observed rendering" is confirmed live — the code path does not reach the emitted row. Audit: **0 / 0 / 0** |
| **C D7 (b)** | monotone X menus carrying no marker **0** (was 11/11) | **PASS (0 / 9)** | All 9 X menus carry exactly one `[<- …]` marker. New literal `[<- largest affordable X - X=4 gains 4 life and draws 4 cards; no listed X does more]` **3 renders / 1 take**; `[<- most kills …]` **6 / 6**; `[<- best trade …]` **0**. Header `Every listed value is affordable; option 1 is the LARGEST X` **9 / 9**, largest-first **9/9** (owner ruling) |
| **C D7 (c)** | > 4 identical-middle rows collapse to a range row; answers stay in range | **UNTESTED on the collapse (no menu big enough), PASS on range** | Largest X menu is **7 rows** (`130v126` seq 70), largest Sphinx menu **5** — the 4-row run threshold was never crossed, so no range row rendered. **0 out-of-range answers, 0 `unparsed_reply` on any ANNOUNCE_X record.** Wave-55 HIGH #3's other half survives: of the 3 monotone menus where a larger X was affordable, **2 still answered X = 3 with the marker on the largest row** (`125v146` seq 51 max 4 → 3; `125v130` seq 72 max 6 → 3) — per wave-55's own falsifier, **the anchor is deck125's guide, not the render** |
| **C D17** | **0 SEGV** in 21 games | **PASS (0)** | `Segmentation` / `ASAN` / `assert` **0** in all 42 stderr files; 21/21 natural. The base binary's 2/5 stub crash rate does not recur, and `WAGIC_GPT_BOARDINDEX=0` is usable for the first time |
| **C (carry)** | phase-7 `only display-toggle` livelock | **does NOT reproduce** | 760 lines, **longest consecutive run 4**, in 11 seats (all deck146/152). Per-window churn, not a livelock. Still worth a wave-57 item as inference-free waste (§6 LOW #2) |
| **D D8** | back-face land rows **> 0**; arrival ≥ 1 per affected deck; `Flip Side` takes **0** | **PASS on all three** | **65 rendered `PLAY THIS AS A LAND` rows / 62 records / 6 takes**, by seat 146v125 21, 152v162 11, 146v162 8, 146v152 8, 152v123 5, 146v126 4, others 1–2. Arrivals traced in the game log for 4 of the 5 takes I followed (`- You played Agadeem, the Undercrypt` etc.). `Flip Side` takes **0** (was 11). **0 own turns with two land drops.** The three retired claims render **0 / 0 / 0**: `only the currently-shown face`, `gains you nothing playable`, `Use the face that is showing`. Audit: **0 / 65 rows (309 string occurrences) / 6** |
| **E O8** | win rate per matchup within the wave 53-55 band; 0 new fixture-class anomalies | **PASS** | 152 5/6, 162 5/6, 146 3/6, 126 3/6, 125 2/6, 130 2/6, 123 1/6 — every deck inside the 1/6–5/6 band of waves 54-55; 0 draws, 0 crashes, max seat 61 turns. Draw-order probe: **0 non-monotonic library counts** that are not Elixir of Immortality shuffles (13 increases, all deck125 seats, all with Elixir in play), and the only cards drawn > 4 times in one seat are deck125's post-Elixir re-draws. Opening hands **42/42 at 7 cards** |
| **carry** | Baka-executed ≤ 0.20 % | **PASS (0 / 2,762 = 0 %)** | `chose -1 of N` **0** in stderr; `giving this decision to the heuristic` **0**. The 6 `choice: -1` records all recovered through the re-ask path |
| **carry** | genuine mis-executions ≤ 1 / 3,000 | **PASS (0)** | 2,583 replies carry a `CHOICE: n (label)` parenthetical; **13 labels disagree with the executed row = 5.03 / 1,000**, and all 13 are cosmetic short-forms (`Plains - "W"` vs `Plains [land] [your library] - "W"`, `Create Vampire Token` vs the `+1: create a 1/1 vampire with Sorin…` row). The one that looked substantive — `130v125` seq 43 labelled `Cycle Spark Spray` — took the numbered CAST row it coded, which is the doctrine. **0 genuine mis-executions** |
| **carry** | recovery rule: one `recovery` per `choice: -1` | **PASS 6/6** | 6 `recovery` records for 6 `choice: -1` records |
| **carry** | 21/21 natural | **PASS** | corpus-results.tsv: 0 timeouts / draws / crashes / hangs |

### Fallback census — 6 / 2,762 = **0.22 %** (wave 55: 0.44 %)
`named_row_reask` 3 (`123v146` 13, `126v130` 12, `126v130` 58 — all 3 `named_row_reask_recovered`,
`_exhausted` **0**), `stale_echo` 1 (`123v126` 68), `wall_miss_unrecorded` 2 (`146v123` seq 19,
`130v162` seq 7). **No `plan_choice_conflict`, no `all_assignments_illegal`, no `stale_livelock`,
no `unparsed_reply`, no `empty_reply`, no `engine_answered`.**
`parse_note` totals: `plan_choice_conflict_narrowed` 14, `echo_index_conflict` 10, `hold_row_named` 6,
`stale_echo_in_range` 3, `named_row_reask_recovered` 3, `name_over_index` 3, `latched_row_mismatch` 2,
`plan_contradicts_noop_row` 1, `named_row_not_offered` 1, `blocks_last_line_taken` 1,
`attack_last_line_taken` 1, `long_reply` 1, `repeat_count_under_two` 1. **`decision_reversed_in_prose` 0**,
`pass_hold_ambiguous` 0, `answer_replaced` / `commit_retracted` 0.
gameend sums: `hold_windows_skipped` **1,236** (non-zero on **30 / 42** seats, max 153),
`mana_only_windows_skipped` 1,482, `identical_option_asks_resolved` 6, `wall_miss_events` 3,
`wall_miss_unrecorded` 2. stderr: `auto-passing without a model call` 2,242, `all actions
pass-declined` 158, `hold re-opened` 144, `no reply after 900s` 3, `http_error` 0, `force-close` 0.

### Mulligans (new-baseline record)
**40 of 42 seats kept 7**; **2 mulliganed once and both then kept 6** (2 `bottom` records, both the
model's). Zero double-mulligans, zero mull-to-zero, **no OWNER QUESTION**. N is thin — the 9
mulligans of wave 55 are not a trend to compare against.

---

## 2. D18 — the rescue fired, and it is not the class that costs

`dropping stale async answer` **65** (wave 55: 43; target < 20) → **FAIL**. The new rescue line
`AIPlayerGPT: consuming an in-flight answer whose prompt text drifted (same seam, turn, phase and
board)` **fires 8 times**, so the mechanism works — it simply is not where the volume is.

**All 65 drops are the turn-flip seam alternation** lane A said would survive:

| class | n | shape |
|---|---|---|
| **E1 — land-drop arm** | **44** | the in-flight answer for the last window of one turn is displaced by the seat's own `Land drop:` ask at Main 1 |
| **E2 — cast arm** | **21** | same flip, displaced by a `Casting decision (…)` ask at the new turn's phase |
| E3–E6 (stack resolution, phase-only, reveal double-drop, targeting sub-ask) | **0** | wave 55's 15 non-alternation drops are **gone** |

**55 of 65 have a `10DrawAction` resolving in between** (the hand changed — a real board change),
and **0 are phase-only on an unchanged board**. So D18's second prediction PASSES exactly while its
headline fails: the residual is not drift, it is a genuinely different question arriving in the one
async slot. Cost: 65 × 28.0 s ≈ **0.51 h** (2.4 % of 21.46 h). Repro:
`game-125v123-1788455392.stderr:932` (hold taken at the cast seam turn 10 → `Land drop: which land
do you play now` at the flip); `game-123v130-1788455402.stderr:475`.

**The rise 43 → 65 is not a regression in the slot; it tracks the window mix.** The corpus has 13 %
fewer decisions but **HOLD takes up 282 → 320**, and a hold taken at the end of a turn is precisely
what leaves an in-flight answer to be displaced by the next turn's land-drop ask: 5 of the 65 drops
have a `holding priority` line in the 4 lines before them. **Fix shape for wave 57 (cheap, and it
is a second slot, not a key change):** give the land-drop ask its own async slot so the two arms do
not share one. **Falsifier:** with two slots, drops fall below 20 and no drop's next ask is a
`Land drop:`.

---

## 3. Narration correctness — 436 events cross-checked, 0 false statements

Both seats of a matchup narrate the same game from opposite sides, so every own-side event has a
mirror. Method: parse the final decision's GAME LOG in each of the 21 mirrored seat pairs, normalise
the copy-disambiguator (`(that X was 1 of 2 copies in your hand…)`) and the exile possessive, and
diff per `(turn, event)` up to the last turn both logs reached.

- **436 own-side events cross-checked across 5 classes** (cast, land play, resolve-and-enter,
  attack, death). **Asymmetries: 0.** Every `You cast X` on one side has its `Opponent cast X` on
  the other, at the same turn.
- **Life mirror**: 477 `(turn, phase)` pairs. 35 disagree by 1–3 points; I traced the largest
  (`123v125` t15 Upkeep: A says 18/20, B says 20/17) and it is **sampling time, not falsehood** —
  deck125's Staff of Nin activation resolved between the two seats' windows (`- Your Staff of Nin
  dealt 1 damage to the opponent (now 17)` sits between them in the log). **0 false life statements.**
- **Mana line**: `Mana available: N total` vs the per-source enumeration — **2,511 checks, 0
  mismatches** (parsing top-level `;` only; a naive split reports 430 false hits, which is why the
  first pass is not the finding).
- **Ability-row arithmetic**: 145/145 (§1 D2). **Incoming-combat arithmetic**: 38/38, and the
  `; this KILLS you` flag matches `K <= 0` on 38/38.
- **Price claim traced end to end**: `123v146` seq 7 offered `Cast Intruder Alarm {2}{u} [NAMED BY
  THEIR Silverquill Silencer: casting this costs you 3 life and draws them a card - you would be at
  14]` at 17 life; the model took it and the log reads `- You cast Intruder Alarm / - You lost 3
  life (now 14) / - Opponent drew a card`. Primitive verified (`borderline.txt:102764`,
  `auto=chooseanameopp … life:-3 opponent && draw:1 controller`). The wave-55 UNTESTED NAMED-BY
  price is now **TESTED and TRUE: 4 renders / 1 take** (`123v146` seq 7, 13, 15, 16).

I found **no false or missing statement** in any sampled surface. The only narration *quality*
residue is byte cost, below.

---

## 4. Prompt economics

| kind | n | mean | p50 | p90 | max | lat p50 | lat p90 | lat max |
|---|---|---|---|---|---|---|---|---|
| ask | 2,127 | 10,854 | 10,106 | 19,079 | 24,732 | 14.0 s | 56.7 s | 935.5 s |
| priority | 456 | 12,653 | 12,407 | 20,900 | 25,947 | 16.9 s | 59.4 s | 745.0 s |
| attackers | 88 | 10,910 | 10,271 | 18,586 | 24,969 | 26.6 s | 58.0 s | 257.0 s |
| discard | 55 | 12,842 | 12,354 | 20,222 | 26,298 | 11.1 s | 52.7 s | 460.2 s |
| blockers | 25 | 10,706 | 9,448 | 17,612 | 25,023 | 40.3 s | 72.2 s | 491.8 s |
| reveal | 9 | 10,681 | 10,731 | 14,528 | 14,528 | 38.0 s | 58.3 s | 58.3 s |
| bottom | 2 | 2,422 | — | — | 2,527 | 78.1 s | — | 78.1 s |

Total prompt volume **30.89 M** chars (wave 55: 38.60 M over 3,171); mean **11,185** (12,174, −8.1 %);
**prompts > 30 K: 0** (was 1); > 25 K: **6** (was 252). **GAME LOG share 53.9 %** of the mean prompt
(wave 55: 58.1 %). Per-decision inference **28.0 s** (26.1) at 21 concurrent games — the wave-55
finding holds: prompt bytes fell 8 % and per-decision latency **rose** 7 %, so latency tracks queue
depth, not prompt size. That is wave-55 prediction #10 **CONFIRMED** and it retires prompt-trimming
as a latency lever.

Where the 21.46 h went: opponent-turn windows **5.78 h (26.9 %)**; windows carrying `[you declined
this exact list ≥ 3 times]` **2.02 h (9.4 %)** (wave 55: 11.9 %); identical-declined runs **1.18 h
(5.5 %)**; the 65 dropped calls ≈ **0.51 h**; the three 900 s wall misses **0.75 h**.

**HOLD adoption improved and the target is now within reach.** Overall take rate **320 / 2,016 =
15.9 %** (wave 55: 12.0 %); in `N ≥ 3` declined windows **60 / 501 = 12.0 %** (was 8.9 %, target
≥ 20 %) — **still FAIL, but the trend is right and the cost of those windows fell 11.9 % → 9.4 %**.
By side: own-turn 150 / 1,273 (11.8 %), opponent-turn 170 / 743 (**22.9 %**) — the opponent-turn arm
has **passed 20 %**. `hold_windows_skipped` sum **1,236** (was 735) on 30/42 seats.

---

## 5. Engine / render items, ranked

**HIGH #1 — D7(a) never renders: 40 of 40 X rows carry no mana-fit clause.**
Repro: `125v130` seq 72 (turn 47, Sphinx's Revelation, 3 rows) — `X = 6 {X pricing: X=6 - you gain 6
life and draw 6 cards} [<- largest affordable X - X=6 gains 6 life and draws 6 cards; no listed X
does more]` with no `{leaves N sources}` anywhere on the
menu, while every CAST row in the same corpus carries one (3,498 `{leaves` occurrences). Lane C
shipped the per-row `selectAutoTapProducers` call and flagged that it had never been observed
rendering; this corpus confirms the path is dead. The marker (b) landed, so the pilot now sees WHICH
X is largest but still cannot see what any X leaves up — and it answered X = 3 on 2 of the 3 menus
where more was affordable. **Falsifier for 57:** X rows with no fit clause **0 of N** (this corpus
40/40).

**HIGH #2 — the async slot is shared by the land-drop and casting arms: 65 drops, 0.51 h.**
See §2. Repro `game-125v123-1788455392.stderr:932`. Ask: a second slot for the land-drop ask.

**HIGH #3 — D4's refusal marking did not ship where it is needed, and `chosen_text` still lies about
HOLD takes.** Two faces of one field: (a) all 6 `choice: -1` records carry **no** `chosen_text` and
**0** `<refused: …>` strings render, so a harvest still cannot tell a refusal from a missing field;
(b) **112 of 320 HOLD takes** record `chosen_text` truncated at `…you give up no cast)`, dropping the
`{taking this row skips the rest of this turn's identical windows}` tail that is on **2,016 / 2,016**
rendered rows. Any exact-match census reads **208** where the truth is **320** — a 35 % undercount,
and it is why the wave-56 harvest's D8 numbers were wrong too. Repro: `130v162` seq 82; census in
§1. **Falsifier:** `chosen_text` byte-equal to the rendered row on 100 % of takes.

**MED #1 — D13 residual: 10 of 36 blockers B-lines still nest a gain in the survival verdict.**
Repro `126v123` seq 37 (own lifelink, the predicted class); 9 more are attacker lifelink
(`126v152` seq 7/11, `123v126` 57/65/92, `162v152` 8, `146v126` 20). The un-nested clauses that DID
ship are correct and readable. Ask: route both lifelink flavours through the same out-params.

**MED #2 — the `[RE-ASK]` CHOICE-line form and the `all_assignments_illegal` arm are still
un-executed** (0 windows, two waves running), as is `plan_choice_conflict_recovered`. Not a defect;
a standing instrument gap — three shipped recovery paths with **no live evidence** that they work.

**MED #3 — D11's two withdrawal rules never fired** (0 renders) and the census stamp
`decision_reversed_in_prose` disappeared (0, was 14). Both are silent-instrument items: a rule that
cannot fire and a census that no longer counts read identically to "fixed".

**LOW #1 — event-log duplicate lines: 1,767 adjacent duplicates in 670 records**, dominated by
`- Opponent drew a card` ×1,348 (Howling Mine games, where two draws in one turn is TRUE but is
never collapsed to `(xN)`). Wave-55's `- Phase: Draw` duplicate class is **GONE (0)**. With the GAME
LOG at 53.9 % of the prompt this is the cheapest remaining byte trim — and, per §4, worth **nothing
in latency**; propose it only as a context-window headroom item.

**LOW #2 — 760 `only display-toggle (Flip Side) options; auto-passing` lines, max run 4**, all in
deck146/152 seats (146v126 263, 146v130 158, 146v152 156, 146v125 56). Bounded and inference-free,
but it is 760 windows of engine work whose only purpose is a display toggle the model took **0**
times this corpus. With D8 shipped, the Flip-Side row's remaining reason to exist is a spell back
face; ask whether the toggle-only window can auto-pass without entering the seam at all.

---

## 6. Specimen candidates

1. **PRIMARY — `1788455432-ai_baka_deck126-…-vs-ai_baka_deck146.jsonl` seq 41** (`126v146`), turn 27,
   **Blockers, 25,023 chars, 25.0 s, log share 59 %.** It carries the wave's whole new render surface
   on one page: the menace header (`2 of the 9 attackers need TWO or more blockers each; you have 2
   blockers, enough to complete at most 1 of those blocks`), 9 A-lines with per-attacker menace
   annotations, `INCOMING THIS COMBAT`, `{blocking trigger, this combat: …}`, `{after this combat:
   you control 1 fewer blocker …}` and `Their untapped sources:` — and the model blocked the two
   Goblins with its two Walls, which is the right read of that header.
2. **SECONDARY — `1788455395-ai_baka_deck125-…-vs-ai_baka_deck123.jsonl` seq 166** (`125v123`),
   turn 33, Upkeep, 16,328 chars, 5 rows. The D2 exhibit: two `Deal 1 damage with Staff of Nin …
   {right now: takes 1 damage - they would be at 0; THIS WINS THE GAME}` rows, taken, game over.
   One page showing what the wave-55 render (`they would be at 165`) was hiding.

---

## 7. Falsifiable predictions for wave 57

1. **Async slot.** With the land-drop ask on its own slot, `dropping stale async answer` **< 20** and
   **0** drops whose next ask is a `Land drop:` (this corpus 44 of 65).
2. **`chosen_text` fidelity.** HOLD takes whose `chosen_text` is not byte-equal to the rendered row:
   **0** (this corpus 112 / 320); `choice: -1` records with no `<refused: …>` value: **0** (6 / 6).
3. **X fit clause.** X rows with no `{leaves N sources}` / `{taps you out}`: **0 of N** (40 / 40).
   *Falsifier:* still 0 renders → the per-row autotap call is unreachable at that seam and the item
   is a wiring bug, not a builder bug.
4. **D13.** B-lines nesting any gain inside the survival verdict: **0 of N** (10 / 36).
5. **HOLD adoption.** Takes in `N ≥ 3` declined windows **> 20 %** (12.0 %; opponent-turn arm already
   22.9 %), and identical-declined max run **< 10** (13).
6. **Menace / recovery arms.** With N ≥ 3 menace windows again, header on 100 % / 0 % elsewhere
   (3/3 this corpus); and at least one live firing of `all_assignments_illegal`, the CHOICE-line
   `[RE-ASK]`, or `plan_choice_conflict_recovered` — three arms now **two waves un-executed**.
7. **D11.** If a plan reaches 40 windows or the menu-diff note fires on a plan > 4 windows old, the
   withdrawal sentence renders on 100 % of those windows (this corpus: 0 windows, 0 renders).
8. **Display-toggle churn.** `only display-toggle` lines **< 200** corpus-wide (760) with no run > 4.
9. **Latency.** Held at 21 concurrent games, per-decision inference stays within ±10 % of 28.0 s
   regardless of prompt size — wave-55 #10 confirmed twice; a third confirmation retires the trim
   lever permanently.
10. **D2 stays closed.** Ability rows with `K != opp_life - damage`: **0 of N**; `at 164`/`at 165`: 0.

---

## 8. Wave-57 docket seeds (from review-carry §"Carried", with this corpus's evidence)

- **Phase-7 livelock (lane C's second defect).** Evidence: **does not reproduce here** — 760 lines,
  max consecutive run **4**. Keep the docket item for the stub repro (no `timeout`), but it is not a
  corpus-visible risk after D8. Re-scope to LOW #2 above.
- **`MTGAbility::target` ownership.** 0 SEGV in 21 games with the deref guards; `a->source` remains
  unguarded. No new evidence either way — the fix held, the ownership item stands.
- **Third replay defect (E).** Nothing in a translog can see it; unchanged.
- **The 30 spell back-face gates (D).** Untouched and unexercised: `Flip Side` takes 0, and no
  alternative-cost back-face cast appears in the corpus.
- **Baka took the new back-face land row 0 times in a stacked game.** Still open (heuristic-scorer
  item); this corpus has 0 Baka executions at all, so no new evidence.
- **Attackers-window nesting (B D13 residual).** Now measured on the BLOCKERS side too: 10/36 —
  promote from residual to a real item (MED #1).
- **D18 rescue never observed firing (A).** **CLOSED**: it fired **8 times** with the exact stderr
  line. The open item is the shared slot, not the rescue.
- **Wave-55 boundary obligation 2** (deck130 :779 dead HOLD quotation): guide-side, untouched here;
  the wave-54 HOLD strings render **0** corpus-wide, so any live quotation of them is still dead.
- **NEW — the census stamp `decision_reversed_in_prose` renders 0.** Lane A promised the broad census
  would be kept beside `plan_choice_conflict_narrowed`. It is not in any record. Docket it as a
  silent-instrument item: without it, "0 conflicts" cannot be distinguished from "0 counted".

---

## What I did NOT check

- **I did not build, run, or A/B anything.** Every ask is a proposal; `WAGIC_GPT_BOARDINDEX=0` is
  usable now but no reviewer ran it.
- **I did not diff prompts across polls** (only the final prompt is logged), so the 65 drops are
  classified from stderr ordering, not from two prompt strings side by side.
- **I could not test** the CHOICE-line `[RE-ASK]` (D3), `all_assignments_illegal`, D15's duplicate
  tag, D11's two withdrawal rules, D7(c)'s range collapse, or the life-loss loop — **no window arose
  for any of them**. Those are UNTESTED, not PASS.
- **Lanes G / H / I / K / N remain unobservable here**: no RSS telemetry, no ASAN, no per-tick
  verdict trace, **0 Baka executions**, no Vita exposure. `In GetCost Seems ManaCost was not properly
  initialized` still appears in stderr, unchanged in character.
- **Lane M's cache-hit counters still reach no log** — a fourth corpus running.
- **I own no deck guide** and read none; the X = 3 anchor (deck125), the Lightning Greaves cast at 0
  creatures (deck123) and deck123's 1/6 third wave are named here as evidence and left to those seats.
- **Card facts verified:** Silverquill Silencer, Sphinx's Revelation, Starstorm, Spark Spray, Staff
  of Nin (primitives only; no Scryfall cross-check was needed — no claim turns on a printing detail).
