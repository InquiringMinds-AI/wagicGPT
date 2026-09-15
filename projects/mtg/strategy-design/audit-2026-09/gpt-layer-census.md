# AIPlayerGPT.cpp — measured census and debloat proposal

**Read-only measurement, 2026-09-15.** Master `982394151`. No engine change, no build, no game run.
Subject: `projects/mtg/src/AIPlayerGPT.cpp` = **106,056 lines** / 6,254,621 bytes;
`projects/mtg/include/AIPlayerGPT.h` = 2,808 lines.
Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final` — 42 seat logs, 21 games,
**2,135 prompts** carrying text (1,661 ask + 290 priority + 80 attackers + 52 discard + 28 blockers
+ 20 reveal + 5 bottom, all `thinking=on`), 28,033,436 prompt bytes, mean 13,130 B.

Everything below is a count from the file, from `git blame`, or from the corpus. Where a number is
produced by a classifier rather than read directly, the method and its error are stated with it.

---

## 0. Growth, attributed

`git log … -- src/AIPlayerGPT.cpp` = **474 commits**, first `eead0d56d` 2026-06-09, last `8826da7c9` 2026-09-14.

Per-line `git blame`, by authorship date (the 2026-08-24 line is the owner's reference point, commit `2d09ea75c`):

| surviving lines authored | production | PARSETEST | total |
|---|---|---|---|
| on or before 2026-08-24 | 14,339 | 6,677 | **21,016** |
| after 2026-08-24        | 48,652 | 36,388 | **85,040** |

So of the ~83,000 lines added in three weeks, **44%** (36,388) is self-test corpus and 56% is
production code. Only 21,016 lines of the 22,899-line file the owner last looked at still survive
verbatim; the rest was rewritten in place.

Per-wave, blame grouped by the wave number in the commit subject (9,165 lines sit on commits whose
subject carries no wave number — merges and early history):

```
W42 1932  W43 1859  W44  752  W45 1426  W46  891  W47 1525  W48 1927  W49 1324
W50 1358  W51  867  W52 1186  W53 1823  W54 3060  W55 1830  W56 1604  W57 3595
W58 1054  W59  911  W60 2755  W61 3153  W62 1976  W63 2755  W64 2743  W65 2136
W66 2942  W67 2311  W68 2243  W69 1651  W70 1309  W71 2066  W72 2193  W73 1595
W74 2542  W75 2381  W76 3020  W77 3448  W78 3525  W79 4972  W80 3704  W81 3277
```

Mean **2,177 surviving lines per wave** over waves 42-81, and the trend is up, not down: the last
six waves (W76-W81) contribute 21,946 lines, 26% of the whole file, of which 10,602 are PARSETEST.
Split per wave (production / PARSETEST) the self-test share rises from ~40% in W42-W50 to
**50% in W76-W79** (W79: 2,473 production / 2,499 test; W77: 1,628 / 1,820; W76: 1,419 / 1,601).

---

## 1. SIZE MAP

**Method.** A brace-tracking pass extracts all 1,313 top-level function bodies (the file's code lives
in one anonymous namespace plus `AIPlayerGPT::` members; both are treated as top level). Each function
is assigned a region by its name, with a hand-written override table for the 40 largest. PARSETEST is
bracketed by line, not by name: `runParseSelfTest` begins at **L62,992** and the file ends at L106,056
— everything from L62,992 on is self-test. Coverage is 105,959 of 106,056 lines (97 lines of includes
and file-statics uncovered).

**Error.** Name-based region assignment is exact for parse/telemetry/keys (their names are literal:
`parse*`, `*translog*`, `*hold*`/`*Key*`/`*latch*`) and approximate at the render boundary: 21,620
lines fall in helper functions whose names do not say what they serve (`gangKillNeed`, `xSurveyBoard`,
`crackBackFloorSources`). Those were re-attributed by call site — for each such helper, the dominant
category among its non-PARSETEST callers. That is reported separately below so the reader can see the
size of the judgment call. Treat the render figure as **±3,000 lines**.

### Line-kind view (exact, no classifier)

| kind | lines | % of file |
|---|---:|---:|
| comment-only lines | **29,105** | 27.4% |
| blank | 2,495 | 2.4% |
| code | 74,456 | 70.2% |

Comments split 22,224 production / 6,881 PARSETEST. **More than one line in four in this file is prose.**

### Region view

| region | lines | % | comment-only within |
|---|---:|---:|---:|
| **PARSETEST self-test** (L62,992-106,056) | **43,066** | 40.6% | 6,881 |
| **Rendering** — emitters (board/option/annotation/verdict/narration) | 21,877 | 20.6% | 7,687 |
| **Support helpers** (fact scans feeding the above) | 21,620 | 20.4% | 7,118 |
| **Decision seams** (chooseX / FindCardToPlay / computeActions / mulligan) | 9,282 | 8.8% | 3,287 |
| **Reply parsing** (parser + salvage + label walk + answer segment) | 3,707 | 3.5% | 1,241 |
| **Transport** (askModel, poll, request body, worker, Render) | 2,543 | 2.4% | 1,083 |
| **Keys / caches / holds** (ask keys, replay, latch, hold) | 2,360 | 2.2% | 1,046 |
| **Telemetry** (translog writer, gameend census, recovery record) | 1,321 | 1.2% | 668 |
| utility + preamble | 286 | 0.3% | 71 |

Re-attributing the 21,620 support lines by dominant non-PARSETEST caller:
RENDER 5,525 · SEAM 5,242 · other support 7,142 · transport 982 · keys 591 · parse 592 · telemetry 476
· **825 lines sit in 26 file-static helpers with no production caller at all** (19 of them, 667 lines,
are called only from PARSETEST — largest `revealSinglePickDeclineLegal` L61,438 at 271 lines; 7, 158
lines, are referenced from nowhere in this translation unit: `gptPadlogFile`, `combatDirectiveExampleEcho`,
`combatDirectiveNegatedOnLine`, `loopCautionForBoards`, `gptTracePromptDrift`, `gptNoteEngineRevealFloor`,
`noteRevealStall`). Virtual overrides called by the engine are excluded from this count by checking the header.

Folded to the owner's categories: **rendering ≈ 27,400 lines (26%)**, PARSETEST 43,066 (41%),
seams 9,282, parsing 3,707, keys/holds ≈ 2,950, telemetry ≈ 1,800.

### Inside PARSETEST

43,066 lines, 661 sections (median section 43 lines), **6,865 `CHECK(` assertions**.

| composition | lines | % of PARSETEST |
|---|---:|---:|
| bare string literals (fixture input/expected text) | 11,533 | 26.8% |
| code | 9,831 | 22.8% |
| comment | 6,881 | 16.0% |
| `CHECK(` assertion | 6,865 | 15.9% |
| code carrying a literal | 5,908 | 13.7% |
| `cout` section narration | 1,111 | 2.6% |
| blank | 937 | 2.2% |

**Only 7,661 of the 43,066 lines (17.8%) name any production function.** The other 82% are fixture
strings, echo text and commentary. 870 distinct production functions are referenced at least once.
The self-test is `#if defined(WAGIC_GPT_PARSETEST_BUILD)`-gated — desktop only, stubbed on Vita/PSP/Android.

---

## 2. WAVE ATTRIBUTION

Per-wave line counts are in §0 (blame-derived, exact).

The `//#W<NN>-<lane>` markers: **3,898 marker lines, 206 distinct tags.** Attributing each line to the
nearest preceding tag inside the same function (a tag heads a block; a function boundary resets it)
gives the table below. **This method over-attributes inside PARSETEST**, where one tag heads a long
fixture block — read the counts as "lines standing under this tag", not "lines this tag added".

| tag | lines | what it added (first phrase of the tag's own comment) |
|---|---:|---|
| W81-DM | 3,644 | V16 — "THE ECHO HEADING" (reply-echo / byte fixes) |
| W80-DG | 2,186 | U12 — loyalty / level-up clause labels ("+1:", "-2:") |
| W70-BL | 2,048 | E1 — "THE PROTOCOL IS A GOLDEN FILE, AND THE REPLY IS TWO LINES" |
| W81-DK | 2,017 | V9 — "the stub IS the transport here" |
| W79-DC | 1,726 | F7 — does this clause refer back to an earlier clause |
| W75-CI | 1,704 | P18 — self-harm chain rows (cast) |
| W77-CU | 1,594 | F9 — "THE CEILING, STATED AND ENFORCED" |
| W68-BD | 1,546 | J8 — ONE edict floor for mode rows and standalone rows |
| W43-10 | 1,546 | how a target is NAMED in the append-only log |
| W66-AR | 1,514 | MED deck126 — a carried plan naming a card |
| W63-AB | 1,489 | E3 — per-pairing MATERIAL rank behind survivable-block |
| W66-AU | 1,475 | R4 — "the line claimed two things its …" |
| W77-CS | 1,290 | R3 — one clause (combat / sacrifice truth) |
| W71-BR | 1,263 | L7 |
| W76-CP | 1,189 | Q10 — ghostform-liveness predicate |
| W79-CZ | 1,127 | T3 / F1 |
| W78-CY | 1,105 | F6 — "A SENTENCE STOP INSIDE A QUOTED ABILITY IS NOT A …" |
| W71-BO | 1,091 | R3 — `smallSeamTruncationReaskLine` |
| W68-BE | 1,082 | R3 — WHICH takes the stated stop clamps |
| W77-CR | 1,079 | R9 — wave-76 deck126/deck146 measure item |

Runner-up tags: W80-DH 1,054 (F9, a "MAY" draw is not a cost), W79-DA 996 (T16), W54-E 950 (D16,
"playing every land in this hand"), W65-AO 932 (deck162, 41 of 41 windows).

---

## 3. RENDER REACH — the dead-render list

**Method.** Every string literal in production code (L1-62,991) is scanned for bracket fragments
`[…` / `{…` carrying at least three lowercase letters. That yields **298 distinct rendered clause/
annotation heads** ("families"), each attributed to the function that emits it. Each head is then
counted against the full text of all 2,135 corpus prompts (substring match).

**Error / limits.** (a) Heads assembled at runtime from a bare `"["` plus a variable are invisible to
this method — the 298 are the *named* families, not every possible rendered token. (b) The per-duel
system prompt is not stored in the corpus (the `system` record carries only `system_hash`,
`system_bytes` and `reply_protocol`), so rules-primer text cannot be reach-measured; one family
(`{mana cost`) renders in the reply protocol and is excluded from the dead list. (c) Zero reach means
*this* 7-deck, 21-game corpus never rendered it — for card-mechanic families it means the pool lacks
the cards, not that the code is wrong (see the split below).

### Distribution

| reach in 2,135 prompts | families |
|---|---:|
| 0 prompts | **84** |
| 1-20 prompts (<1%) | **103** |
| 1-10% | 73 |
| 10-50% | 24 |
| ≥50% | 14 |

**187 of 298 families (63%) render in under 1% of prompts. 84 render never.**

Top of the live end, for contrast: `[tapped` 1,809 (84.7%) · `[artifact` 1,781 · `{effect` 1,738 ·
`[hold check:` 1,715 · the hold-contract paragraph 1,383 (64.8%) · `{card text:` 1,347 ·
`[castable now` 1,307 · `{leaves` 1,105 · `{closes ONLY this window…` 1,093 ·
`[no cast row now: sorcery speed…` 821 · `[cannot pay now: needs` 782 · `{right now:` 724.

### The 84 zero-render families, split by cause

**(a) Dormant because the deck pool has no such card — 20 families.** Corpus contains zero
occurrences of "mutate", "infect", "changeling", "hybrid", "poison", "toxic", "Battle " anywhere in
2,135 prompts. Deleting these removes capability, not bloat.
`changelingAnnotation` L5530 · `mutatedPileTag` L9846 · `attackerPoisonNote` L13,971/13,975/13,978/13,981
(infect / toxic / wither) · `hybridPipNoteText` L49,536 · `blockCountRequirementTag` L8136 ·
`attackerDestinationTag` L58,827 (planeswalker-only attacker) · `chooseAttackers`'s `[battle` L59,803 ·
the four `chooseMenuAction` MDFC/mutate menu annotations L54,335/54,431/54,480/54,733/54,739/54,743 ·
`mdfcLandArrivalTagFrom` L50,196 · `exileCastRegister` L40,754 · `scriptCostGloss` `{cycle` L53,546 ·
`FindCardToPlay`'s `{adventure spell:` L50,662.

**(b) Verdict-family variants that never fired, while the family itself renders — 31 families.**
These are the real measured-dead strings: the family head renders, the specific verdict string does not.
- `[own loop verdict: …` — **all 15 variant strings render 0 times** across six emitters
  (`w78OwnLoopVerdictLine` L34,555, `w79OwnLoopVerdictLineFor` L34,613, `w77OwnLoopVerdictKey` L34,725,
  `w79VerdictDangerRank` L34,761, `w80LoopFaceOfLine` L36,876, `w80ProvenWinLoopLine` L36,902).
  The gameend counter agrees: `own_loop_verdict_lines_rendered` = 20 over 42 seats, `own_loop_windows_asked` = **1**.
- `[crack-back verdict: LETHAL …` (three variants, L36,843-36,847), `[crack-back verdict: none` L36,180 —
  0 renders, while the family as a whole rendered 382 times (counter `crackback_verdict_lines_rendered`).
- `[stack death verdict: nothing lethal on the stack` L36,095 — 0 (family total 36).
- `[deals 0 …` — 4 variants (L6,897/6,936/6,941/6,974), 0 renders.
- `[summoning sick — cannot attack this turn, and it cannot block either` L5,735 and
  `[tapped - cannot attack this turn` / `[tapped - blocking` L5,568/5,570 — 0 renders of these exact strings.
- `{it enters UNTAPPED` L33,541/50,196, `{kills nothing: every legal target is INDESTRUCTIBLE (` L41,691,
  `{answers the stack: YES -` L40,011, `{DEAD right now: no legal object for this mode` L42,981,
  `{this row's cost SPENDS A BLOCKER` L29,787, `{this same question will be asked again this turn:` L36,412,
  `{crack-back cover: identical to the one printed in full on row …` L41,164 (the W81 fold itself),
  `{you cannot pay this right now: 0 mana available` L9,453,
  `[your loop is resolving on its own …` L34,509, `[no cast row now: a play restriction forbids casting it` L31,428,
  `[granter in play:` L40,754, `[<- best trade: …` L17,652, `[upkeep offer: …` L40,377,
  `[decline the payment -` L52,889, `[it is on the battlefield untapped from now on` / `[usable (…) this turn` L52,844-52,849,
  `[protected from this damage's source` L3,276 — all 0.

**(c) Not emitters at all — 33 families, excluded from the dead count.** `stripNarrationDecoration`
(L33,356, 301 lines) holds a copy of 58 head literals because it *removes* them from narration;
`replaceAllOccurrences` holds primer placeholders; `renderProbeDump` (L26,275-26,290) is a debug dump;
`isTemplatePlaceholderLine` holds a template token.

**Dead-render count, emitters only: 23 functions, 760 lines, every one of whose heads renders in 0 of
2,135 prompts.** Of those, the card-conditional ones (group a) should be kept; the measured-dead
verdict variants (group b) are the deletable set.

### The <1% list (renders, but 1-20 prompts of 2,135)

53 emitter functions, 2,442 lines. Largest: `theirConverterBodyTag` L27,866 (340 lines, best head 10
prompts = 0.47%) · `revealSinglePickDeclineLegal` L61,438 (271 lines, 11 prompts — and it is called
**only from PARSETEST**) · `describeTarget` L17,996 (171, 3) · `wipeSurvivorTail` L3,251 (68, 2) ·
`heldBackBlockTag` L7,025 (68, 7) · `xKillRowAnnotations` L17,459 (62, 17) ·
`castDrawPriceRowTag` (61, 2) · `forcedSacrificeRowTag` (60, **1**) · `zeroPowerBlockerTag` (58, 12) ·
`w80SelfLeavesNoCoverClause` (57, 3) · `xKillRowCore` (54, 4) · `w77StayHomeCoverTag` (52, 3) ·
`splitNoOpBandRow` (38, 3) · `blockCountRequirementTag` (28, 1) · `reserveDeclineCarryNote` (28, 1) ·
`w76UpToNTargetClause` (16, 1) · `tappedSourceAnimateClause`/`tappedAnimateNeedsVerdict` (33, 1) ·
`w80OwnStackSpellTag` (8, 1). Full per-head table is reproducible from the method above.

---

## 4. COUNTER REACH

**Method.** Counter keys read directly from the gameend record writer (`logGameEnd`, L22,888-23,186):
**90 declared keys**, of which 13 are metadata (`won`, `turn`, `my_life`, …). Values summed over the
42 gameend records in the corpus.

**72 counters appear in the corpus records. 26 of them (36%) are 0 corpus-wide:**

```
action_before_plan_rejected_replies   action_before_plan_replies
answer_label_absent_read              ask_replays_refused
ask_replays_repeat_latch              cached_replay_reasked
chain_windows_collapsed               crossphase_board_unchanged
engine_reveal_floor_picks             force_close_arms_deferred
force_close_arms_refused              force_close_defer_bound_hits
force_close_same_arm_deferred         hold_reopened_new_threat
identical_ask_answers_reserved        menu_pass_no_progress
menu_pass_no_progress_suppressed      phase2_answer_missing
phase2_answer_recovered               put_gloss_stripped
repeat_annotated_takes                sibling_window_asks_skipped
stop_reached_reputs_collapsed         wall_miss_events
wall_miss_no_retry                    wall_miss_unrecorded
```

Per-record tracing: of those 26, **only two have a corresponding per-record field** —
`plan_line_missing`-adjacent protocol fields cover `action_before_plan_replies`
(record fields `plan_line_missing`, `protocol_deviation`, `off_protocol_bytes` exist and are non-zero:
62, 63, 58 respectively), and `answer_label_absent_read` is shadowed by the record's `coded_answers` /
`answer_replaced`. **The other 24 are counter-only: if the counter reads 0 there is no other evidence
in the log that the path exists at all.** That is the shape the loop's own dropped rule
("a counter that reads 0 after the fix is a FAIL") was built on, and it is unfalsifiable as written.

A further **11 keys are declared in the writer but absent from every corpus record** — they were added
after the corpus binary (W80/W81): `attackers_crackback_cover_clauses`, `cast_decision_reopened_new_stack`,
`counter_intel_rendered`, `crackback_verdict_folded_total`, `hold_reopen_answer_invalidated`,
`multi_answer_first_taken`, `opp_countered_spells`, `sba_windows_cache_bypassed`,
`spare_colour_verdict_withheld`, `verdict_faces_dropped_unrecorded`, `x_cast_row_refusal_markers`,
`x_sweep_roster_markers`. They have **no measurement at all** yet.

The 46 non-zero counters, for contrast: `protocol_replies` 2,135 · `chain_acting_rows_cast` 1,280 ·
`ask_replays_reserved` 1,176 · `ask_replays_cache` 1,172 · `hold_windows_skipped` 1,051 ·
`own_turn_windows_skipped` 722 · `crackback_verdict_lines_rendered` 382 · … down to
`own_loop_windows_asked` 1, `plan_names_stranded_card` 1, `reserve_decline_windows_skipped` 1,
`stop_reached_windows_skipped` 1.

Cost of the 26 zero counters in source: **74 lines in the .cpp + 24 lines in the header = 98 lines**
(member declaration, every increment site, the gameend row).

---

## 5. PIN DENSITY

**Method.** For each PARSETEST line, the identifiers are intersected with the set of 1,313 production
function names; a line "pins" every production function it names. 870 functions are pinned at least once.

Families with **>200 pin lines — three**:

| function | pin lines | corpus reach |
|---|---:|---|
| `parseChoice` (L45,403, 923 lines) | 349 | parser, not a render family |
| `holdActionKeyRow` | 263 | key builder |
| `stripNarrationDecoration` (L33,356) | 236 | strip table, 1,383 prompts |

Next tier: `joinNumberedRows` 94 · `parseAttackerSet` 82 · `w77KeyTailOf` 78 · `holdKeyRow` 63 ·
`w76HoldReopenNote` 63 · `asyncSlotKeyOf` 62 · `holdStillStands` 59 · `parseBlockAssignments` 58 ·
`noAnswerClassFor` 49 · `zoneChangeNarration` 48 · `optionSetKeyOf` 47 · `crackBackVerdictKey` 45.

**Pins on families with zero corpus render:** `w77OwnLoopVerdictKey` 23 · `removalVictimTag` 19 ·
`w79OwnLoopVerdictLineFor` 15 · `w78OwnLoopVerdictLine` 13, plus 15 smaller families — measured by
name reference, **115 PARSETEST lines** pin the 15 deletable zero-render emitters (187 including the no-caller helpers); by head-literal match
(a stricter test, catching only the fixtures that contain the dead string) **58 lines**. Add the
<1% families and the total is **343 PARSETEST lines** on emitters that reach <1% of prompts.

The blunt shape of the pin corpus is the real finding here: **6,865 assertions, and 82% of the
self-test's lines never name the thing under test.** The self-test grew ~50/50 with production code
in the last six waves (§0) and was 36,388 of the 83,000 lines added since 2026-08-24.

---

## 6. DUPLICATION

**Code-level** (2-line shingle Jaccard over comment-stripped, wave-prefix-normalised bodies, functions
≥15 lines): near-duplicate pairs are rare and small — **11 pairs, 373 lines of lower-bound redundancy**.
The clearest:
`lineIsActivatedCost` L16,397 (31) ~ `w80ScriptLineHasActivationCost` L29,446 (38), J=0.52 ·
`drawStepForecastText` L16,141 (63) ~ `theirDrawStepForecastText` L16,078 (63), J=0.41 ·
`scriptActivatedAnimateCost` L3,343 ~ `scriptHasActivatedAnimate` L3,319, J=0.40 ·
`codedChoiceOrdinal` L38,668 (80) ~ `firstCodedChoice` L46,377 (67), J=0.38 ·
`offProtocolBytes` L21,362 ~ `protocolLinesOnly` L21,422, J=0.28 ·
`discardPunisherScan` L15,568 ~ `drawPunisherScan` L14,858, J=0.25.

**Name-family duplication** (same base name, different wave prefix): 418 lines.
`w76HoldReopenNote` L35,103 (89) + `holdReopenNote` L35,641 (110) ·
`w80CrackBackVerdictLine` L36,692 (66) + `w81CrackBackVerdictLine` L36,758 (68) ·
`w79BestBlockDamage` L30,802 (67) + a second `w79BestBlockDamage` L30,890 (63) ·
`holdStillStands` L36,191 (46) + `w78HoldStillStands` L36,330 (35) ·
`w74ClockSourceRecurs` L42,633+L42,645 · `w78StopReachedRePutCollapses` L39,094 +
`w79StopReachedRePutCollapses` L39,080 · `w78LoopVerdictFrom` L35,337 + `w77LoopVerdictFrom` L35,353 ·
`w78RePutCollapseIdentity` L39,028 + `w79RePutCollapseIdentity` L39,070 ·
`w80CrackBackVerdictLineNow` L37,674 + `w81CrackBackVerdictLineNow` L37,698.

**Output-shape duplication — the real one.** Grouping the 298 heads by their first two words and
counting distinct emitters, **17 rendered shapes are produced by more than one function**, 1,061 lines
foldable (keeping the largest emitter of each shape):

| rendered shape | emitters | reach | foldable |
|---|---:|---:|---:|
| `{right now: …` | 7 (`dynamicMagnitudes` 168, `damagePlayerVerdict` 79, `repeatRowStopClause` 33, `ptDropTargetVerdict` 22, `graveyardRefillRowClause` 22, `equipmentCastPriceTag` 15, `tappedSourceAnimateVerdict` 8) | 724 | 179 |
| `{X pricing: …` | 4 (`xDamageSweepCore` 96, `xKillRowAnnotations` 62, `xKillRowCore` 54, `splitMonotoneXRow` 39) | 229 | 155 |
| `[own loop verdict: …` | 6 (see §3b) | **18** | 138 |
| `[crack-back verdict: …` | 5 (`w81CrackBackVerdictLine` 68, `w80CrackBackVerdictLine` 66, `w79VerdictDangerRank` 28, `crackBackVerdictKey` 14, `w80CrackBackFaceOfLine` 13) | 329 | 121 |
| `{crack-back cover: …` | 3 (`crackBackBlockerRowTag` 221, `w80SelfLeavesNoCoverClause` 57, `w81FoldDuplicateCoverParagraphs` 52) | 61 | 109 |
| `[no cast row now: …` | 2 (`handCastabilityTag` 89, `oppLifeRise` 75) | 821 | 75 |
| `[stack death verdict: …` | 3 | 36 | 45 |
| `{DEAD right now …` | 4 | 39 | 43 |
| `[deals 0 …` | 2 | 12 | 38 |
| `{kills whichever …` | 2 | 126 | 35 |
| 7 more groups (≤34 lines each) | | | 123 |

Note the pattern: **the crack-back verdict and the own-loop verdict each have one emitter per wave that
touched them (W77, W78, W79, W80, W81), all still resident.** That is the growth mechanism in one
picture — each wave added a face/key/line variant beside the last instead of replacing it.

Separately, **58 head literals are duplicated between the emitter that writes them and
`stripNarrationDecoration` (301 lines) that removes them** — a second copy of the render vocabulary
that must be kept in sync by hand.

---

## 7. PROPOSAL — three tiers, numbers only

No rewrite is proposed. Each tier is a line count backed by the measurements above.

### Tier A — delete (evidence: rendered in 0 of 2,135 prompts, or reachable from no production caller)

| item | .cpp lines | .h lines |
|---|---:|---:|
| zero-render emitter functions, card-conditional ones **excluded** (the own-loop verdict six, the crack-back LETHAL/none variants, stack-death-nothing, the `[deals 0` pair, `removalVictimTag`, `spellRemovalVerb`, `namedCastPenaltyScan`, `stackAnswerYesRowClause`, `w75ApplyChainFeed`, `w80SacrificeSpendsBlockerClause`, `cannotPayNowClause`, `asyncDropTraceLine`, `w81FoldDuplicateCoverParagraphs`, `zeroPowerAttackerTag`, the W77-W80 own-loop key/face emitters) — 15 functions | **508** | — |
| file-static helpers with **no production caller** and no header declaration — 26 functions, of which 19 are called only from PARSETEST (667 lines, largest `revealSinglePickDeclineLegal` L61,438 = 271) and 7 are referenced nowhere at all (158 lines: `gptPadlogFile` 34, `combatDirectiveNegatedOnLine` 30, `combatDirectiveExampleEcho` 30, `loopCautionForBoards` 21, `gptTracePromptDrift` 18, `gptNoteEngineRevealFloor` 18, `noteRevealStall` 11) | **825** | — |
| PARSETEST pins on both sets (union, measured by name reference) | **187** | — |
| the 26 zero counters (member + every increment + gameend row) | **74** | **24** |
| **Tier A total** | **1,594** | 24 |

The two sets do not overlap (checked: empty intersection).

**Projected file size after A: 104,462 lines (−1.5%).**

If the 8 card-conditional zero-render families were also cut (252 lines + pins), add ~300 — but that
removes mutate / infect / changeling / MDFC / hybrid / battle / adventure / cycling capability the
corpus simply never had a card for. Recommend keeping them.

### Tier B — fold (evidence: same rendered shape from multiple emitters)

| item | lines |
|---|---:|
| output-shape folds, 17 groups, keep the largest emitter of each | **1,061** |
| name-family duplicates (wave-prefixed twins) | **418** |
| code-level near-duplicate pairs (11) | **373** |
| PARSETEST pins attached to the folded-away emitters (measured share) | **~200** |
| **Tier B total** | **~2,052** |

**Projected file size after A+B: 102,410 lines (−3.4%).**

### Tier C — keep

Everything else: the 14 families rendering in ≥50% of prompts and the 97 rendering in 1-50%; the 46
non-zero counters; the parse region (3,707 lines — the parser is the surface the owner ruled must stay
robust); the seams (9,282); transport (2,543); the translog writer (1,321). The 53 emitters in the
1-20-prompt band (2,442 lines) stay in C until the experiment below discharges them: low reach is not
evidence of low value.

### The number this measurement actually forces

Tier A + Tier B together remove **3.4%** of the file. **They are not where the bloat is.** The two
measured concentrations are:

1. **PARSETEST: 43,066 lines, 40.6% of the file**, `#if`-gated to desktop builds, 82% of it fixture
   literals that never name the function under test, growing at ~50% of every wave's output. It is
   already conditionally compiled — moving it verbatim into its own translation unit
   (`AIPlayerGPTParseTest.cpp`, added to `wagic.pri`) is a **zero-behaviour-change** edit that leaves
   `AIPlayerGPT.cpp` at **62,990 lines (−40.6%)**. Combined with A+B (3,259 of whose lines are outside
   PARSETEST and so still apply): **59,731 lines (−43.7%).**
2. **Comments: 29,105 lines, 27.4%**, of which 22,224 are in production code — i.e. the production
   half of the file is 35% prose. No reduction is proposed here without the owner's call: the comments
   carry the wave provenance (`//#W<NN>-<lane>` × 3,898) that made this census possible in the first place.

### What this measurement cannot tell you

Reach is **not** value. A clause that renders in 12 of 2,135 prompts may be the clause that prevents a
specific catastrophic misplay in exactly those 12 windows; a clause that renders in 1,383 prompts may
be ignored by the model every time. **Nothing here measures whether removing a line changes a decision.**
This census measures presence, provenance, duplication and dead code — not behaviour.

**The one experiment that would.** Build two binaries from the same commit differing only by the Tier
A+B removal (a compile-time flag is the cheapest form, so the same source ships both). Run the same
21-game round-robin over the same 7 decks, same seeds where the harness allows, `--thinking on`, `-j 3`,
uncapped per Invariant 00. Compare the two corpora **on decision quality, not win rate** (win rate is
blind to a symmetric core prompt and 21 games has no power to resolve it anyway): per seat, the same
review seats read both transcripts blind to which is which and count (a) misplays of the classes the
removed clauses were written for — the crack-back, own-loop, stack-death, zero-power and X-pricing
windows specifically — (b) `protocol_deviation_replies` and `plan_line_missing` rates, (c) fallback
rate, (d) mean prompt bytes and latency. The removal is safe when the per-class misplay counts are
statistically indistinguishable and the byte/latency curve improves; it is unsafe the moment a class
the removed clause served shows more misplays in the debloated arm. Given how rare those windows are
(own-loop windows asked: **1** in the whole corpus), expect that several removed families will need
**more than 21 games** to be discharged either way — which is itself an argument for deleting them:
a surface that cannot be shown to matter in 2,135 prompts is not carrying the decision.
