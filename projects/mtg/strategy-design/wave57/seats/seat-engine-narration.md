# Wave-57 seat review — ENGINE / NARRATION

Corpus `matchups-20260903-174505` (binary = hermetic `make -B` of master **67b2bb38a**; wave-57
step-1 lanes A e67d3534c, B 002ae42c6, C 6a07a6571, D ba2afebd6, E a81f05695, F 59789dce2,
G b1e06895b, H 01fe39f1f, S c461135c7, T 9cfdd71d1, U b9feba4e0). Model qwen36-35b-a3b on
Spark :8084. **42 seat logs, 3,408 records** = 42 `gamestart` + 42 `gameend` + 7 `recovery` +
**3,317 decisions** (ask 2,642 / priority 481 / attackers 76 / discard 66 / blockers 35 /
reveal 13 / bottom 4). **21/21 natural, 0 SEGV, 0 timeouts, 0 draws, 0 hangs.** Recorded
inference **25.38 h = 27.5 s per decision**. Max prompt **30,228**; prompts > 30 K: **4**.

Every string quoted is the emitter's ACTUAL output, read with python over the JSONL (`prompt`,
`options_text`, `chosen_text`, `parse_note`, gameend fields) and `/usr/bin/grep` over the 21
`.stderr` files. **All row counts are taken from the rendered `prompt`** (numbered `N. ` lines,
plus the `A#`/`B#` combat lines), never from the `options` array. Card facts verified against
`projects/mtg/bin/Res/sets/primitives/`: Wall of Omens, Perimeter Captain, Pride Guardian,
Overgrown Battlement (and the 55-card `forcetype(land)` MDFC set derived from the primitives for
the D2 audit). Read-only throughout: no build, no run, no git, no edit under `bin/Res` or `src/`.

---

## 0. Headline

**D4 landed completely and it re-opens exact-match censusing.** `chosen_text != options_text[choice-1]`
on **0 of 2,960** single-row takes (wave 56: 124 of 2,416), and **every** `choice: -1` record now
carries a `<refused: …>` value (7 of 7; wave 56: 0 of 6). The HOLD census re-run EXACTLY reads
**450 takes of 450** — the wave-56 prefix workaround is retired and every number in this file is an
exact match.

**D5 fails on its headline and PASSES on its mechanism, and the two must not be confused.**
`dropping stale async answer` is **108** (target < 20, wave 56: 65) — a clear FAIL. But the new
drop line carries the arm, and the classification is unambiguous: **0 of 108 drops are cross-arm.**
Every land-drop-arm drop's next ask is a `Land drop:` (45/45) and every casting-arm drop's next ask
is a `Casting decision` (61/63, 2 unresolvable inside 8 lines). The class the second slot was built
to kill — a casting answer thrown away by a land-drop ask — is **gone**. The residual is
*intra*-arm: the same arm's next window displacing its own unclaimed in-flight answer. §2.

**The corpus grew 20 % and the prompt grew 9 %, and per-decision latency FELL.** 3,317 decisions
(+20 %), mean prompt 11,185 → **12,197** (+9.0 %), GAME LOG share 53.9 % → **63.4 %** — and
per-decision inference **28.0 s → 27.5 s**. That is wave-55 prediction #10 confirmed a **third**
time at 21 concurrent games: latency tracks queue depth, not prompt size. **The prompt-trim lever
is retired.** Any D13 narration budget must be justified as context-window headroom, never latency.

---

## 1. Set-B adjudication — every prediction, by the emitter's actual strings

`#188` three-number audits are given as **old renders / new renders / takes** (old = wave-56 corpus).

### Lane A

| Prediction | Verdict | Evidence |
|---|---|---|
| **D5** stale drops **< 20** (was 65) | **FAIL (108)** | 16 of 21 stderr carry drops; worst `126v130` 29, `125v123` 16, `125v126` 15. Audit: **65 / 108 / —**. §2 |
| **D5** drops whose next ask is a `Land drop:` **0** (was 44) | **FAIL (45) — but see §2** | All 45 are **land-drop-ARM** drops (wave 56's 44 were casting-arm answers killed by a land-drop ask). The cross-arm class the prediction was proxying for is **0 of 108** |
| **D5** the D18 consume line still fires | **PASS (11)** | `consuming an in-flight answer whose prompt text drifted (same seam, turn, phase and board)` **11** (was 8) |
| **D5** falsifier 2: a rise in `pending` stalls or a parked game | **PASS (no park)** | 21/21 natural, 0 timeouts, max seat 76 turns. `pending` 385 lines, no run that fails to advance |
| **D4** single-row takes with `chosen_text != options_text[choice-1]` **0 of N** (was 124/2,416) | **PASS (0 / 2,960)** | Exact byte compare at `priority` + `ask` |
| **D4** `choice: -1` records with no `chosen_text` **0** (was 6) | **PASS (0 / 7)** | 6 `<refused: named_row_reask>`, 1 `<refused: retracted_choice>`. Audit: **0 / 7 / —** |
| **D4** an exact-match HOLD census reads 320-class, not 208 | **PASS (450 / 450)** | Every HOLD take's `chosen_text` is byte-equal to a rendered row of its own menu |
| **D16** `decision_reversed_in_prose` non-zero when narrowed is non-zero; `broad == narrowed + firings` | **PASS (6 == 6 + 0)** | `decision_reversed_in_prose` **6** (wave 56: 0), always co-stamped with `plan_choice_conflict_narrowed` (6); `plan_choice_conflict` firings **0**. The silent-instrument defect is closed. Audit: **0 / 6 / —** |
| **D31** per-stderr `windows held this game` == sum of that file's two seats' `hold_windows_skipped` | **PASS (21 / 21)** | 1,863 lines corpus-wide, 1,863 summed over 42 gameend records, and equal file-by-file |
| **D31** each seat's closing line reproduces its gameend fields; hold lines separable by seat token | **PASS (42 / 42)** | 42 `AIPlayerGPT[ai_baka_deckN]: game end (turn N, won/lost) - windows held …` lines (21 won + 21 lost); every hold line leads with the seat token |

### Lane B

| Prediction | Verdict | Evidence |
|---|---|---|
| **D6** windows during their combat with no `INCOMING THIS COMBAT:` line **0 of N** (was 209/218) | **PASS on the emitter's window set (0 of 112); 144 silent windows, all provably empty** | **256** opponent-turn combat-phase decisions (the brief's loose 437 over-counts). 112 carry the line, **144 do not — and all 144 read `Opponent battlefield (… 0 of them able to attack right now …)`**: nothing was declared and nothing *could* attack. Verified by the engine's own board-header count on 144/144. The four forms: **declared 48, closed 60, forecast 4, settled 0** |
| **D6** the four forms | **3 of 4 TESTED; `settled` UNTESTED (0 renders)** | `already dealt or removed this combat` **0 / 0 / —**. Its window (flags cleared, same turn, past damage) never arose in a logged decision |
| **D6** arithmetic | **PASS 48/48 + 4/4** | declared form: `my_life - unblocked == K` on **48 / 48**, `; this KILLS you` matches `K <= 0` on **48 / 48**; forecast form: `my_life - up-to == K` on **4 / 4** |
| **D6** two-combat-turn latch (unguarded, lane B's own flag) | **UNTESTED** | No turn in the corpus has two combat phases |
| **D24** lethal-header blockers windows where the seat must subtract itself **0 of N** | **PASS (1 of 6, and the 1 is the documented cap)** | 6 blockers windows carry `this KILLS you`; 5 print `best case with every blocker assigned` / `one legal assignment gets you to`. The miss is `126v123` seq 25 — **74 attackers**, above lane B's stated 32-a-side ceiling; the seat blocked with all 5 bodies anyway. Audit: **0 / 34 bestcase + 6 onelegal / —** |
| **D24** cleared best case + `BLOCKS: none` below 1 of 1 | **PASS (0)** | 0 blockers windows decline all blocks under a best case that clears 0 |
| **D22** `BLOCKING THIS COMBAT:` declines-all-blocks-under-a-converter **< 1 of 1** | **PASS (0 of 6) — thin, and the converter half is UNTESTED** | Header renders **6 / 6 records**, all deck126 blockers windows, and the seat blocked in **6 of 6**. The `and your converter takes that much off them` half renders **0** — no window had a converter of the seat's *and* a triggered blocker. Audit: **0 / 6 / —** |
| **D10** B-lines nesting a gain inside the survival verdict **0** (was 10/36) | **PASS (0 of 51 B-lines)** | `(lifelink:` appears **0** times on any B-line. The un-nested clauses ship: `{blocking trigger, this combat: you may gain 2}` 11, `{… you gain 3}` 3. Audit: **10 / 0 / —** |

### Lane C

| Prediction | Verdict | Evidence |
|---|---|---|
| **D7** creature cast rows under THEIR converter with no `{their converter:` **0 of N** | **PASS (0 of 2) — thin** | Re-derived on the correct scope (converter on **their** battlefield, per `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs`, 301 renders): only **2** creature cast rows arose under it and **both** are priced (`130v126` seq 63 Siege-Gang Commander, `125v126` seq 293 Emrakul). The brief's 80-of-82 counted the whole prompt including OWN-converter boards. Audit: **0 / 2 / 2** |
| **D7** the lethal branch fires ≥ 1 | **UNTESTED (0)** | `; that KILLS you` renders **0** on the converter tag |
| **D8** discard rows with a computable dead verdict and none **0 of N** (was 357/426 bare) | **FAIL (6), but the class shrank hard** | **455 discard rows: 89 `{dead right now:`, 32 `{right now:`, 334 bare** (bare share 83.8 % → 73.4 %). The legend prints on **51 of 51** records that carry any verdict. Strongest external calibration — bare rows whose card IS priced on a cast row elsewhere in the corpus — is **6 rows / 3 records**: Lightning Greaves at 0 own creatures (`123v125` seq 77, 81), Silverquill Command + Barrowin (`146v162` seq 17, 24). Audit: **0 / 89 dead + 32 live (56 records) / —** |
| **D8** sweeper/edict sent from a cleanup menu at 0 opposing creatures below 4-in-one-game | **UNTESTED** | No cleanup discard of a sweeper at 0 opposing creatures arose |
| **D11** cast rows naming an already-targeted victim with no tail **0 of N** (was 3/3) | **PASS (0 of 2) — thin** | `but an effect already on the stack is aimed at that same creature` **2 renders / 2 records**, **0 takes**. Audit: **0 / 2 / 0** |
| **D30** menus with two rows, identical `{right now:}` verdict, different price, and no mark **0 of N** | **FAIL (1 of 3)** | 3 such windows. **2 are marked — by D15's same-card tag**, which renders for the FIRST time (`126v152` seq 7 and 11, the `Tribute to Hunger {2}{b}` / `{4}{b} [from exile]` pair). The cross-card window is **unmarked**: `146v126` seq 25, `Cast Agadeem's Awakening {b}{b}{b}{x}` and `Cast Silverquill Command {2}{b}{w}` both reading `{right now: returns NOTHING - your graveyard has no creature …}`. Audit: **0 / 0 / 0** |
| **D30** the tag renders at least once | **FAIL (0)** | `{identical verdict right now to row ` **0 renders**. (D15's `{same effect as row ` **2 renders** — a PASS for D15, which had never rendered) |
| **D21** cast-mode rows carrying no clause about the path they displace **0 of N** | **PASS (0 of 32)** | 16 cast-mode menus; `the alternative this replaces:` on **16 / 16** `Cast Card Normally` rows, `the cast this replaces:` on **16 / 16** `cycling` rows. `ctx->spellTargetType` IS populated: the count renders (`(6 legal targets on the board right now)`). Audit: **0 / 16 + 16 / 13 + 1** |
| **D21** cycle takes at opp life ≤ 6 rise above 3 of 17 | **FAIL (2 of 11)** | 172 windows offer a cycling row, 19 cycle takes. At `Opponent life ≤ 6`: **2 of 11 (18.2 %)** vs wave 56's 3 of 17 (17.6 %) — flat, and N is far too small to resolve a real move |
| **D12** all-dead menus with no menu-level line **0 of N** | **PASS (0 of 2,009)** | The token renders on **497 records**. Predicate: for every menu with ≥ 1 real cast row, is every one of its verdict strings in the set the emitter has itself classified as zero? **0 non-token menus qualify.** Audit: **0 / 497 / —** |
| **D12** spends off an all-dead menu below deck126's 6-in-one-game | **PASS (1)** | One cast off a token menu corpus-wide: `123v130` seq 7, Tragic Slip |

### Lane D

| Prediction | Verdict | Evidence |
|---|---|---|
| **D14** `Flip Side` rows on an AI seat whose other face is a LAND **0 of N** (was 108 of 109) | **PASS (0)** | `Flip Side` renders **0 times anywhere in 3,317 prompts**. Audit: **109 / 0 / 0** |
| **D14 (b)** option sets built and discarded for a land-backed toggle-only window **0** | **PASS — and this is what the 1,309 lines prove** | `only display-toggle (Flip Side) options; auto-passing without a model call` is now **1,309** (was 760), and the line's NEW tail settles it: **`(suppressed 1 land-face toggle row(s))`**. The row is dropped at the CANDIDATE level, before the option set is built — the window auto-passes with **no prompt and no model call**. The rise 760 → 1,309 is the mechanism converting former Flip-Side windows into toggle-only auto-passes; it is **not** a cost. Longest consecutive run **4** (histogram 1×782, 2×95, 3×103, 4×7), unchanged from wave 56 — still not a livelock |
| **D26** back-face land rows carrying the arrival clause | **PASS (0 of 7 missing; 23 Pathways correctly silent)** | 30 `PLAY THIS AS A LAND` rows. 7 non-Pathway rows: **6 pay-3-life** (`{taking this row then ASKS you to pay 3 life …}`, 4 takes) + **1 unconditional-tapped** (1 take). The 23 without a clause are **all Pathways** (land/land, the deliberate "neither" class). Audit: **0 / 6 + 1 / 4 + 1** |
| **D27** `[NAMED BY THEIR …]` per-cast clause | **PASS (16 / 16)** | Every `[NAMED BY THEIR` render carries `This price is PER CAST, not one-off`; 2 takes. Audit: **4 / 16 / 2** |
| **D28** lowercased script tokens in `chosen_text` **0 of N** (was 54 narration renders from one take) | **PASS on `chosen_text` (0); FAIL on narration (75)** | 0 `chosen_text` anywhere carries a lowercased MDFC token, and `(menu text: …)` renders 0 (the case-variant path, as designed). But the GAME LOG still prints **`- Opponent used: agadeem, the undercrypt with Agadeem's Awakening` × 75** (`130v146`). That is the generic `used:` ability-name emitter, not the MDFC row head — a different, broader class (2,338 `cycling`, 1,137 `choose card`, …). Audit: **54 / 0 chosen_text, 75 narration / 0** |
| **D29** adjacent byte-identical GAME LOG lines **0 of N** (was 1,767 in 670 records) | **PASS (0)** | Zero adjacent duplicates in any of 3,317 rendered logs. Audit: **1,767 / 0 / —** |
| **D29** records carrying a collapsed count line **> 0** | **PASS** | `- X drew N cards` **4,435** occurrences; `(xN)` fallback **2,740** in 724 records (top: `- Opponent drew 2 cards (x2)` 414, `- Opponent put a card from their hand into their library (x8)` 311) |

### Lane E

| Prediction | Verdict | Evidence |
|---|---|---|
| **D2** pregame headers omitting a `forcetype(land)` card **0 of N** (was 4/4) | **PASS (0 of 6)** | 50 pregame prompts. Against the 55-name MDFC set derived from the primitives: **10 hands hold an MDFC, 6 hold a spell/land MDFC and all 6 carry the note**; the 4 without it are **Pathway-only hands** (land/land — nothing is counted twice, correctly silent). Audit: **0 / 6 / —** |
| **D9** X rows carrying no mana-fit clause **0 of N** (was 40/40) | **PASS (0 of 52)** | 12 ANNOUNCE_X menus, 52 `X = n` rows, every one carrying `{leaves …}` or `taps you out`. The wave-56 HIGH #1 wiring bug is CLOSED. Audit: **0 / 52 / —** |
| **D15** `since turn N` life trend on 100 % of board frames (was 1/137) | **PASS (3,267 / 3,267)** | Every prompt carrying `Your life: … | Opponent life: …` carries `Opponent life trend:`. Audit: **1 / 3,267 / —** |
| **D19** fetch windows with no colour tag of either polarity **0 of N** (was 1/5) | **PASS (0 of 30)** | Both polarities live: positive **117 occurrences / 29 records**, negative **13 / 4**. One wording residual, §5 LOW #3 |
| **D20** X cast rows with an affordable ladder and no `[<- …]` marker **0 of N** | **PASS (0)** | 526 X cast rows; 506 marked; **all 20 unmarked read `X pricing: your mana affords only X=0 right now`** — no ladder to mark. Audit: **— / 518 records / 21 takes** |
| **D23** `{feeds:}` takes at `converters on your battlefield: 0` with a converter in hand **0 of N** (was 1/3) | **FAIL (4 of 20) — but the render is TRUE, so this is a deck162 guide item** | `{feeds:` renders 88 / 60 records, and **88 / 88** carry the `; in your hand: N` half. 20 feeds-row takes; **4** are at battlefield 0 with a converter in hand: `162v125` seq 4 (Howling Mine), 43 (Dictate of Kruphix), 54 (Howling Mine), `162v126` seq 10 (Howling Mine). Audit: **— / 88 / 20** |

### Lanes F / G / H / S / T / U

| Prediction | Verdict | Evidence |
|---|---|---|
| **F** 0 SEGV, 0 phase-stuck games | **PASS** | `Segmentation` / `ASAN` / `assert` **0** across all 21 stderr; 21/21 natural, 0 timeouts, 0 hangs, max seat 76 turns |
| **F** replay stall floor: `0<name>` refusals 60 → 2 | **UNTESTED** | **0** in this corpus — but no replay is loaded in a selfplay run, so this is absence of the window, not a pass |
| **F** the Intruder Alarm / Thraben Doomsayer activation livelock | **does NOT reproduce, and the shape is now visible** | `126v123` reached **74 attackers** on turn 16 and ended naturally on turn 15/16 with deck123 winning at 126's −39. The token engine ran to a decision, not to a livelock |
| **G** (human seat only) | **UNTESTED by construction** | No human seat in a selfplay corpus. Note the AI seat's own A/B lines DO use the range-collapse grammar (`A2-A74. Human #1-#73 … x73`), and it read correctly here |
| **H** `log_window` = `full` on 100 % of records | **PASS (3,317 / 3,317)** | `log_window: "full"` on every decision |
| **H** the `log_window_kind` census reproduces wave-56 ask-kind shares (land-drop 10.1 %) | **PASS (11.4 %)** | land_drop 377 (11.4 %), empty_stack_pass 368 (11.1 %), combat 111 (3.3 %), cleanup_discard 66 (2.0 %), target_or_reveal 13 (0.4 %), pregame 4 (0.1 %), **unclassified 2,378 (71.7 %)** |
| **H** `elided` is 0 on 100 % of records | **UNTESTED — the field does not exist** | I enumerated all 46 distinct keys across all 3,408 records: the only window keys are `log_window` and `log_window_kind`. **There is no elided-turn count on any record.** §5 HIGH #3 |
| **S/T/U** `abandoned_in_flight` **0** | **PASS (0)** | 0 in every record and 0 in stderr; 0 `no reply after`, 0 `http_error`, 0 `force-close` |
| **S** `WAGIC_STALLPROBE` never shows a held window > 1230 s | **UNTESTED by construction** | `WAGIC_STALLPROBE` / `stallprobe` appear **0 times** in 21 stderr files — the probe is dev-gated and this was a release run, so the arm produced no evidence either way. Say so, do not read it as a pass |
| **T** the softlock dump writes `softlock-<epoch>.txt` | **UNTESTED (no softlock)** | No `softlock-*` file exists in `~/.Wagic/` or the run directory, and `softlock` / `SoftlockAbort` appear 0 times in stderr — correct, because no softlock occurred. The menu entry is unexercisable in headless selfplay; the Vita play report is its only gate |
| **V** (Baka blocker policy) | **UNTESTED by construction** | Merged after launch; not in this binary. Belongs to wave 58 |

### Carried standing claims

| Claim | Verdict | Evidence |
|---|---|---|
| D2 (wave 56) stays closed: ability rows with `K != opp_life - damage` **0 of N**; `at 164`/`at 165` 0 | **PASS (0 / 196; 0)** | 196 `takes N damage - they would be at K` rows, all correct. **3 lethal face rows, 3 takes** (`THIS WINS THE GAME`) |
| Baka-executed ≤ 0.20 % | **PASS (0)** | `giving this decision to the heuristic` **0**; one `chose -1 of 6` line, which is the recorded `retracted_choice` refusal, recovered |
| recovery rule: one `recovery` per `choice: -1` | **PASS (7 / 7)** | 7 `recovery` records for 7 `choice: -1` records |
| 21/21 natural | **PASS** | 0 timeouts / draws / crashes / hangs |
| D1 (wave 54/56 carry): byte-identical same-turn re-asks after a HOLD take **< 20** | **FAIL (29 of 450)** | Wave-56 predicate exactly (forward-search inside the same turn for a byte-identical rendered row tuple): 14/320 = 4.4 % → **29/450 = 6.4 %**. On strict adjacency it is **0 of 450** — the harvest's 14 is a third rule again; §5 LOW #4 |
| D1: identical-declined share **< 15 %**, max run **< 10** | **FAIL (17.6 %, max 13)** | 584 / 3,317 in 88 runs, **2.02 h of 25.38 h (8.0 %)**. Wave 56: 14.99 %, max 13, 5.5 % |
| HOLD adoption in `N ≥ 3` declined windows **≥ 20 %** | **FAIL (9.6 %) and REGRESSED** | 62 of 644 (wave 56: 12.0 %). Overall HOLD **450 / 2,490 = 18.1 %** (up from 15.9 %); own-turn 247/1,521 = 16.2 %, opponent-turn 203/969 = **20.9 %** (wave 56: 22.9 %) |

### Fallback census — 8 / 3,317 = **0.24 %** (wave 56: 0.22 %)
`named_row_reask` 6, `retracted_choice` 1, `unparsed_reply` 1. **No `plan_choice_conflict`, no
`all_assignments_illegal`, no `stale_livelock`, no `empty_reply`, no `engine_answered`, no
`wall_miss`, no `abandoned_in_flight`.**
`parse_note` totals: `echo_index_conflict` 22, `latched_row_mismatch` 19, `decision_reversed_in_prose` 6,
`plan_choice_conflict_narrowed` 6, `named_row_reask_recovered` 6, `stale_echo_in_range` 5,
`name_over_index` 4, `attack_last_line_taken` 3, `repeat_count_under_two` 3, `hold_row_named` 2,
`blocks_last_line_taken` 1, `named_row_not_offered` 1.
gameend sums: `hold_windows_skipped` **1,863** (stderr agrees, 21/21 files),
`mana_only_windows_skipped` 385, `identical_option_asks_resolved` 13, `wall_miss_events` **0**,
`wall_miss_unrecorded` **0**. stderr: `auto-passing without a model call` 1,694, `hold re-opened` 224,
`Mana cost is NULL` 310, `In GetCost Seems ManaCost was not properly initialized` 528 (unchanged in
character), `http_error` 0, `no reply after 900s` 0.

### Mulligans (new-baseline record)
**38 of 42 seats kept 7**; **4 mulliganed once and all 4 then kept 6** (4 `bottom` records, all the
model's). Zero double-mulligans, zero mull-to-zero, **no OWNER QUESTION**. Kept land counts across
the 7-card keeps: 2 lands ×13, 3 ×10, 4 ×13, 5 ×4 — a normal spread; nothing here is attributable
to a guide floor.

---

## 2. D5 — all 108 drops classified: the arm split worked, the residual is intra-arm

`dropping stale async answer` **108** (was 65; target < 20) → **FAIL on the count**. The line now
carries `(<arm> arm; the <what> moved)`, so the classification is read off the drop itself:

| arm | drift kind | n | next ask |
|---|---|---|---|
| **land-drop** | question (or turn/phase) | **45** | `Land drop:` **45 / 45** |
| **casting** | question (or turn/phase) | **51** | `Casting decision` 47, none-in-8-lines 4 |
| **casting** | question and board | **12** | `Casting decision` 1, `Combat damage order` 1, none-in-8-lines 10 |
| any | `same slot` / `board` alone / `unknown` | **0** | — |

**No drop is cross-arm.** Wave 56's 44 land-drop-next-ask drops were casting-arm answers displaced
by a land-drop ask; this corpus has **zero** of those. **The second slot works.**

**The residual, by preceding context (6 lines before the drop):**

| class | n | shape |
|---|---|---|
| **L1 — land-drop arm, opponent's own land auto-tap** | **35** | `AIPlayerBaka: AI attempting to pay a mana cost. / - Cost: NULL / Mana cost is NULL.` immediately precedes; the seat's own `Land drop:` ask then arrives on a new turn and displaces the previous turn's unclaimed land-drop answer. Repro `game-125v123-1788475508.stderr:409` |
| **L2 — land-drop arm, draw only** | **8** | `10DrawAction` in between; same turn-flip shape |
| **L3 — land-drop arm, neither** | **2** | — |
| **C1 — casting arm, stack-trigger churn** | **55** | `Action added to stack: StackAbility` in the preceding lines; **17 of these are preceded by `hold re-opened`** — the Sanguine Bond ↔ Exquisite Blood ping-pong re-opens the hold, the seat is re-asked, and the previous ask's answer is still in flight. Repro `game-126v130-1788475528.stderr:1617` |
| **C2 — casting arm, other** | **8** | — |

**The rise 65 → 108 is concentrated, not diffuse.** Two games hold 44 of the 108: `126v130` (29,
the converter loop) and `125v123` (16, the L1 class). The corpus is +20 % decisions and +224
`hold re-opened` lines, and `hold re-opened` is precisely what manufactures a same-arm re-ask
while an answer is in flight.

**Cost: 108 × 27.5 s ≈ 0.82 h of 25.38 h (3.3 %)** — up from 2.4 % but still small.

**Verdict for the docket:** the D5 arm split is DONE and should not be re-opened. The open item is
narrower and different: **an in-flight answer for window N of an arm is discarded when window N+1
of the same arm opens, even inside one turn.** The two sub-shapes want different fixes — L1/L2 is a
turn-flip (the answer is genuinely for a dead question), C1 is a *re-ask of the same seam* after a
hold re-open, where the rescue's board half is what blocks the consume. §6.

---

## 3. Narration correctness — 766 mirrored events + 3,267 mana lines, 0 false statements

Both seats of a matchup narrate the same game from opposite sides. Method: parse the final
decision's GAME LOG in each of the 21 mirrored pairs, normalise the copy-disambiguator and the
`(xN)` fold, and diff per `(turn, event)` up to the last turn both logs reached.

- **766 own-side cast / land-play events cross-checked; 11 raw asymmetries, and all 11 resolve to
  sampling or wording, not falsehood:**
  - **6** in the `162v125` pair only — both logs open with `(...earlier events trimmed …)` at
    DIFFERENT points, so the pre-trim events exist on one side only. Log-window trim, not a lie.
  - **2** are the exile possessive: `- You cast Tribute to Hunger from your exile` mirrors
    `- Opponent cast Tribute to Hunger from the opponent's exile` (`123v152` t14, `126v152` t10).
    Correct on both sides.
  - **3** are within-last-turn truncation — the two seats' final windows sit at different points
    inside the same turn (`126v130` t16 second Tribute, `146v125` t19 Hive of the Eye Tyrant,
    `123v152` t24 Lightning Greaves). Traced each in the log; every event is present on the side
    that had reached it.
- **Life mirror**: 276 `(turn, phase)` pairs; **8 disagree, all by ≤ 2 points, all sampling time.**
  Traced the largest (`146v162` t20 Main 1: A reads 1/20, B reads 20/3) — B's window is EARLIER in
  the turn, and `- Opponent lost 2 life (now …)` twice sits between the two. **0 false life
  statements.**
- **Mana line**: `Mana available: N total` vs the per-source enumeration — **3,267 checks, 0
  mismatches**, splitting on top-level `;` only (a naive split reports 154 false hits, all of them
  the semicolon inside the VARIABLE-output parenthetical — that is why the naive pass is not the
  finding). `Their untapped sources: N` on **3,267 / 3,267** `CURRENT SITUATION` blocks.
- **Ability-row arithmetic**: 196 / 196. **Incoming-combat arithmetic**: 48 / 48 declared + 4 / 4
  forecast, `; this KILLS you` matching `K <= 0` on 52 / 52.
- **Card facts traced end to end.** `126v146` seq 11 prints
  `{blocking trigger, this combat: you may gain 2}` on a **Wall of Omens** B-line, which looked
  wrong (Wall of Omens has no blocking trigger). The primitive settles it: Perimeter Captain reads
  `@combat(blocking) source(creature[defender]|mybattlefield):may life:2 controller` — *any* defender
  of yours blocking triggers it. The clause and the `BLOCKING THIS COMBAT: each of your 2 blockers
  that blocks gains you 0 and may gain 2 more - up to 4 life for you` header are **both correct**.
  Pride Guardian (`source(this):life:3`) and Overgrown Battlement (`foreach(creature[defender])`,
  the VARIABLE-output source) verified likewise.

**I found no false or missing statement in any sampled surface.** The three narration residues
below are wording, not truth.

---

## 4. Prompt economics

| kind | n | mean | p50 | p90 | max | lat p50 | lat p90 | lat max |
|---|---|---|---|---|---|---|---|---|
| ask | 2,642 | 12,021 | 11,240 | 21,507 | 30,228 | 13.7 s | 56.0 s | 717.0 s |
| priority | 481 | 12,894 | 12,676 | 20,209 | 30,090 | 17.6 s | 67.5 s | 637.5 s |
| attackers | 76 | 11,359 | 10,730 | 18,627 | 24,732 | 32.7 s | 88.9 s | 289.4 s |
| discard | 66 | 15,871 | 15,635 | 23,760 | 30,127 | 13.7 s | 32.4 s | 131.7 s |
| blockers | 35 | 11,174 | 9,839 | 17,473 | 23,717 | 37.0 s | 77.3 s | 327.4 s |
| reveal | 13 | 14,207 | 12,506 | 18,702 | 22,824 | 28.0 s | 47.3 s | 57.5 s |
| bottom | 4 | 2,354 | 2,165 | 2,249 | 2,864 | 65.0 s | 68.1 s | 75.3 s |

Total prompt volume **40.46 M** chars (wave 56: 30.89 M over 2,762); mean **12,197** (11,185,
**+9.0 %**); **prompts > 30 K: 4** (was 0); > 25 K: **108** (was 6); max **30,228** (`125v162`
seq 175 — the brief's `125v130` attribution is wrong, the file is the deck125-vs-deck162 seat).
**GAME LOG share 63.4 %** of the mean prompt (wave 56: 53.9 %). Per-decision inference **27.5 s**
(28.0) at 21 concurrent games.

Where the 25.38 h went: opponent-turn windows **4.96 h (19.5 %)**; windows carrying
`[you declined this exact list ≥ 3 times]` **2.58 h (10.2 %)**; identical-declined runs
**2.02 h (8.0 %)**; the 108 dropped calls ≈ **0.82 h (3.3 %)**; wall misses **0**.

---

## 5. Engine / render items, ranked

**HIGH #1 — the async slot still discards an in-flight answer on a SAME-ARM re-ask: 108 drops, 0.82 h.**
The arm split closed the cross-arm class completely (0 of 108) and must not be re-opened. The
residual is two shapes with two different fixes. (a) **`hold re-opened` churn, 17 drops** — the
seam, turn and phase are unchanged and only the BOARD moved (a stack trigger resolved), so the D18
rescue refuses to consume; the same question is being re-asked and a fresh round trip is paid.
Repro `game-126v130-1788475528.stderr:1617` (Sanguine Bond ↔ Exquisite Blood, 5 drops in 40 lines).
Ask: let the rescue consume across a board change **when the rendered row tuple is byte-identical**
— that is exactly the condition the hold key already computes, so it costs no new state.
(b) **turn-flip land-drop, 45 drops** — the answer is genuinely dead; the fix is not to consume it
but to not *launch* it: the land-drop ask fires at Main 1 of every turn, so its previous-turn
launch is predictably wasted. Repro `game-125v123-1788475508.stderr:409`.
**Falsifier:** drops < 40 corpus-wide, and 0 drops preceded by `hold re-opened`.

**HIGH #2 — HOLD adoption in the windows that cost the most REGRESSED, and the ≥3-declined class now
costs 10.2 % of all inference.** Takes in `N ≥ 3` declined windows fell **12.0 % → 9.6 %** (62 of 644)
while the class grew 501 → 644 windows and 2.02 → 2.58 h. Overall adoption is up (15.9 % → 18.1 %)
and the opponent-turn arm slipped (22.9 % → 20.9 %). Identical-declined share rose 14.99 % → **17.6 %**,
max run still **13**. The HOLD row's text and price are unchanged, so this is a *render-economics*
item, not a guide item: the seat is being asked the same byte-identical question 3+ times and is
answering rather than holding. Repro: `125v123` seq 163 / 165 (byte-identical menus, both declined,
`NO LIVE CAST ROW ON THIS MENU` on both, HOLD taken only at 163). **Falsifier:** ≥ 20 % in the
`N ≥ 3` class, max identical-declined run < 10.

**HIGH #3 — lane H's elided-turn count reaches no record, which makes the §D A/B unverifiable.**
I enumerated every key across all 3,408 records: `log_window` and `log_window_kind` exist; **no
elided count does.** On the `full` arm the true value is 0 everywhere, so "0 elided" and "0 counted"
are byte-identical readings — the exact silent-instrument shape lane A's D16 just fixed on
`decision_reversed_in_prose`. The §D A/B's own arm-verification census depends on this field, so it
must ship **before** the three-arm run, not after. Related but smaller: `log_window_kind` labels
only 28.3 % of decisions; **71.7 % are `unclassified`**, so a D13 per-kind budget currently has no
handle on seven of every ten decisions. **Falsifier:** every record carries an elided count, and
the `unclassified` share is either named or explained.

**MED #1 — D8's discard evaluator misses 6 rows that the cast evaluator prices.**
`123v125` seq 77 and 81 offer `Lightning Greaves {2} (artifact) {card text: …}` **bare** on a
discard menu at `Your battlefield (… 0 are creatures …)` — the same card, on the same board, gets
`{right now: you control 0 creatures - this equips nothing}` on a cast row. Also `146v162` seq 17
and 24 (Silverquill Command, Barrowin of Clan Undurr). 6 rows of 455, 3 records. The legend
(`A row carrying NEITHER tag is one this engine could not price here`) makes these rows *read* as
un-priceable, which is now false for them. **Falsifier:** 0 bare discard rows whose card carries a
verdict on a cast row on the same board.

**MED #2 — D30's cross-card tag has never rendered; the one live window is unmarked.**
`{identical verdict right now to row ` **0 renders**. `146v126` seq 25 offers
`Cast Agadeem's Awakening {b}{b}{b}{x}` and `Cast Silverquill Command {2}{b}{w}` with byte-identical
`{right now: returns NOTHING - your graveyard has no creature …}` verdicts at different prices, and
neither is marked. The same-card sibling (D15) DID fire twice this corpus, so the seam is reached —
it is the cross-card branch that does not. **Falsifier:** the tag renders ≥ 1, and all such windows
are marked (this corpus 2 of 3 marked, by D15).

**MED #3 — the attackers-window preview still nests the lifelink gain inside the survival verdict.**
D10 cleared the B-lines (0 of 51). The A-line side did not move: `152v126` seq 20 reads
`A1. Intrepid Adversary (4/2) [lifelink] [their untapped blockers: Perimeter Captain (0/4)
(you kill it, your attacker lives (lifelink: you gain 4 from this block only, and this attacker
deals nothing to their life) …` — 3 nested occurrences in 1 record. Small N, same species, same
out-param fix. **Falsifier:** `(lifelink:` on any combat line: 0.

**MED #4 — three recovery paths remain un-executed for a THIRD wave.**
`all_assignments_illegal` 0 (all 35 blockers records carry `dropped_assignments: 0`), the CHOICE-line
`[RE-ASK]` form 0, `plan_choice_conflict_recovered` 0, `plan_choice_conflict` 0. Not a defect; a
standing instrument gap. Add `WAGIC_GPT_WEDGE`-style dev emulators for these three, or accept that
they ship unverified.

**LOW #1 — D12's token has a number/verb disagreement on 443 of its 497 renders.**
`NO LIVE CAST ROW ON THIS MENU: all 1 cast row below carry a verdict …` — singular noun, plural
verb (443 renders at N=1, 31 at N=2, 23 at N=3). One-line fix at the emitter; the sentence is
otherwise exactly right and the token is a clean PASS.

**LOW #2 — the GAME LOG still prints lowercased script tokens for ability names.**
`- Opponent used: agadeem, the undercrypt with Agadeem's Awakening` × **75** (`130v146`). D28 fixed
the MDFC *row head*; the `used:` narration emitter is a separate and much broader class (`cycling`
2,338, `choose card` 1,137, `choose a land` 646, `goblin lair` 336 …). Most read as engine internals
in a sentence otherwise written for a reader. Ask: title-case or map the token through the printed
name at the `used:` write seam.

**LOW #3 — the fetch row's NEGATIVE colour form drops the colour it CAN make.**
`... and it adds {U} or {B} (you cannot make {U} right now)` renders 3 times on boards where the
seat **can** make {B} (`123v126` seq 17 at `colours you can make: {b}`; `123v130` seq 31, 40 at
`{b}{w}`). The statement is true and the positive form names both colours, so the asymmetry invites
the wrong inference. Ask: the mixed case should say both halves.

**LOW #4 — `chosen_text` at the multi-select seams loses the instance handle.**
`126v123` seq 25 records `Wall of Omens blocks Thraben Doomsayer; Wall of Omens blocks Human;
Wall of Omens blocks Human; …` — three DIFFERENT Walls (the menu renders `B1-B3. Wall of Omens
#1-#3`), but the record reads as one creature blocking three attackers, i.e. as an illegal
assignment. `dropped_assignments: 0` and the reply (`BLOCKS: B1:A1, B2:A2, B3:A3, B4:A4, B5:A5`)
prove it was legal. Lane A scoped multi-select out of D4 deliberately; this is the cost of that
scope, and it will mislead any future harvest. Ask: carry the `#N` handle into the joined list.

**LOW #5 — the same-turn byte-identical re-ask after a HOLD take rose 4.4 % → 6.4 %** (29 of 450, by
wave 56's own predicate; 0 on strict adjacency). Three different predicates have now produced three
different numbers for D1 (harvest 14, mine 29, adjacency 0). Pin ONE definition in the ledger before
the next wave re-adjudicates it.

---

## 6. Specimen candidates

1. **PRIMARY (lategame / pathological) — `1788475516-ai_baka_deck126-…-vs-ai_baka_deck123.jsonl`
   seq 25** (`126v123`), turn 16, **Blockers, 14,766 chars, 54.1 s.** The corpus's extreme combat
   and the best single page of the wave's new render surface under stress: `INCOMING THIS COMBAT:
   74 attackers, 75 unblocked damage - you would be at -48; this KILLS you`, the A/B **range
   collapse** (`A2-A74. Human #1-#73 (1/1) deals 1 … x73`, `B1-B3. Wall of Omens #1-#3`) with its
   two explanatory paragraphs, `BLOCKING THIS COMBAT:`, and the D24 cap declining to claim a best
   case above 32 a side. It is also the LOW #4 repro.
2. **SECONDARY (largest prompt) — `1788475520-ai_baka_deck125-…-vs-ai_baka_deck162.jsonl` seq 175**
   (`125v162`), turn 27, Main 1, **30,228 chars, 17.6 s** — the corpus max, and the exhibit for the
   63.4 % GAME LOG share: a page where two thirds of the bytes are history and the decision is a
   land drop's worth of new information.

---

## 7. Falsifiable predictions for wave 58

1. **Async slot.** With the rescue widened to a byte-identical row tuple across a board change,
   `dropping stale async answer` **< 40** and **0** drops preceded by `hold re-opened` (this corpus
   108 / 17). *Falsifier:* the `hold re-opened` class survives → the block is the seam key, not the
   board half.
2. **HOLD economics.** Takes in `N ≥ 3` declined windows **> 15 %** (9.6 %) and identical-declined
   share **< 15 %** (17.6 %). *Falsifier:* a third wave of decline with the render unchanged →
   the lever is not the HOLD row's text.
3. **Elided instrument.** Every decision record carries an elided-turn count, and on the `full` arm
   it reads 0 on 100 % of records (this corpus: the field does not exist). Without this the §D A/B
   is not measurable — this is a gate on the A/B, not a nice-to-have.
4. **D8 residual.** Bare discard rows whose card carries a verdict on a cast row on the same board:
   **0** (this corpus 6 of 455).
5. **D30.** `{identical verdict right now to row ` renders ≥ 1, and cross-card identical-verdict
   windows carry a mark: **0 unmarked of N** (this corpus 1 of 1 unmarked, 2 of 3 rescued by D15).
6. **D10 residual.** `(lifelink:` on any A-line or B-line: **0** (this corpus 3 in 1 record).
7. **D6 `settled` form.** With a logged decision after the attacker flags clear in the same turn,
   the settled form renders on 100 % of those windows (this corpus: 0 windows, 0 renders). And the
   144 pre-declaration `able-to-attack 0` windows: state whether silence there is the intent, or
   whether a fifth "nothing can attack this combat" form is owed.
8. **Recovery arms.** At least one live firing of `all_assignments_illegal`, the CHOICE-line
   `[RE-ASK]`, or `plan_choice_conflict_recovered` — **three waves un-executed**.
9. **Latency.** Held at 21 concurrent games, per-decision inference stays within ±10 % of 27.5 s
   regardless of prompt size — confirmed three waves running (26.1 → 28.0 → 27.5 while bytes moved
   −8 % then +9 %). A fourth confirmation retires the trim lever permanently.
10. **D2 / D9 / D15 / D29 stay closed.** Ability rows with `K != opp_life - damage` 0; X rows with
    no fit clause 0; board frames with no life trend 0; adjacent duplicate log lines 0.

---

## 8. Wave-58 docket seeds (from wave57/review-carry §"Carried", with this corpus's evidence)

- **Suite SIGSEGV in `GameStateDuel::End` → `SAFE_DELETE(transcriptMenu)` under concurrent lanes.**
  No new evidence — this corpus is a single harness with one `~/.Wagic/` and produced **0 SEGV in
  21 games**. The shared-shutdown-race item stands untouched.
- **Intruder Alarm / Thraben Doomsayer activation livelock (F).** **Does not reproduce here, and
  the shape is now on record**: `126v123` built to **74 attacking Humans** by turn 16 and the game
  ENDED naturally (deck123 win, 126 at −39). The engine reached a decision at every step. Keep the
  item for the stub repro; it is not a corpus-visible risk.
- **Replay defects 4 and 5 (F); D34's whose-board term; the chooser as a per-seat pointer.**
  Nothing in a translog can see any of these. `0<name>` refusals are 0 because no replay was
  loaded, not because the fix was exercised — mark the replay-stall arm UNTESTED, not PASS.
- **D33 spell back-face gates (D).** Still unexercised: `Flip Side` renders **0**, and no
  alternative-cost back-face cast appears. The D14 residue (lane D predicted 1 spell-backed row)
  is **0** this corpus, so the residue is currently invisible rather than measured.
- **Baka blocker policy (V).** Not in this binary; 0 Baka executions corpus-wide, so wave 58 gets
  the first evidence either way.
- **D6's two-combat-turn latch (B).** Still unguarded and still untested — no turn in the corpus
  had two combats.
- **D24 model audit for non-trample blocked-hits-face effects (B).** Untouched; and add the
  **32-a-side cap** to it: `126v123` seq 25 is a live lethal window where the cap suppressed the
  number. Decide whether the cap should degrade to `one legal assignment gets you to K` instead of
  silence.
- **`repeat_n` has no harvest consumer (A).** Confirmed: the field exists on 14 `priority` records
  and nothing reads it. Any wave-58 harvest counting repeats must read the field, not the old
  `" xN"` suffix (which is now correctly absent from `chosen_text`).
- **`WAGIC_SELFPLAY_SEED` (H).** Still unbuilt, and §D's A/B cannot pair arms without it.
- **NEW — the elided-turn count reaches no record (H).** Promote to a gate on the §D A/B. §5 HIGH #3.
- **NEW — `log_window_kind` leaves 71.7 % of decisions `unclassified`.** A D13 per-kind budget
  cannot act on the bucket that holds most of the bytes; either the label is wrong or the classifier
  is incomplete.
- **NEW — deck162's `{feeds:}` reads.** 4 of 20 feeds-row takes at `converters on your battlefield: 0`
  with a converter in hand (`162v125` seq 4, 43, 54; `162v126` seq 10). The render is TRUE and now
  prints the hand half on 88/88; this is a **deck162 guide** item and is left to that seat.
- **NEW — deck123's cycle reads.** 2 of 11 cycle takes at opponent life ≤ 6, flat against wave 56
  despite D21's cross-price shipping on 32/32 rows. Guide-side, left to deck123's seat.

---

## What I did NOT check

- **I did not build, run, A/B, or drive anything.** Every ask is a proposal. `WAGIC_GPT_BOARDINDEX=0`,
  `WAGIC_INFLIGHT_BOUND=0`, `WAGIC_GPT_WEDGE=1`, `WAGIC_NO_DEADREF_SWEEP=1`, `WAGIC_HANG_GUARD=1`
  and `WAGIC_STALLPROBE` were all unexercised by this corpus and by me.
- **I did not diff prompts across polls** (only the final prompt is logged), so the 108 drops are
  classified from the drop line's own arm/drift label plus stderr ordering, never from two prompt
  strings side by side. In particular I could NOT verify that the second slot ever HELD an answer
  across an arm switch — lane A's own unverified claim stands unverified; what I can show is that no
  drop was cross-arm.
- **UNTESTED, no window arose:** D6's `settled` form; D7's lethal converter branch; D22's converter
  half; D8's cleanup-sweeper class; D30's tag; `all_assignments_illegal`; the CHOICE-line `[RE-ASK]`;
  `plan_choice_conflict_recovered`; D7(c)'s range collapse on an X menu (largest X menu is small);
  the mandatory life-loss loop; a two-combat turn.
- **UNTESTED by construction:** lane G entirely (no human seat); lane V (post-launch merge);
  lane T's softlock menu entry and lane S's `WAGIC_STALLPROBE` (dev-gated / needs a real softlock);
  lane F's replay-stall arm (no replay loaded). These are absences of the window, not passes — the
  brief asked me to say so and I am saying so.
- **D8's "computable" predicate is mine, not the engine's.** I calibrated bare discard rows against
  cards the CAST evaluator prices elsewhere in this same corpus. That is the strongest external
  check available from a translog, but it under-counts: a card priced by neither surface anywhere
  would not appear in my 6.
- **I own no deck guide and read none.** The deck162 `{feeds:}` reads, the deck123 cycle reads and
  the HOLD-decline behaviour are named here as evidence and left to those seats.
- **Card facts verified:** Wall of Omens, Perimeter Captain, Pride Guardian, Overgrown Battlement
  (primitives only). No Scryfall cross-check was needed — no claim here turns on a printing detail;
  lane D's own Oracle checks for Pelakka Caverns / Agadeem / Silverquill Silencer were taken as filed
  and not re-run.
- **Lanes M / N and RSS/ASAN telemetry remain unobservable here.** `In GetCost Seems ManaCost was
  not properly initialized` still appears 528 times in stderr, unchanged in character.
