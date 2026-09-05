# Wave-58 general strategy — RENDER / CORE-PROMPT proposals, layer-routed (wave-57 body carried verbatim, R319-R330 new)

**This file is `wave57/general-strategy.md` carried forward with the wave-58 synthesis's adopted
edits.** Everything from here to `## WAVE-58 ADOPTED` is the wave-57 text unchanged except for two
forward-pointers (R295, R225) that name the wave-58 item carrying their residual; the adopted
proposals from `wave58/deck{123,125,126,130}/general-proposals.md` are the new section at the end.
Adoption/rejection reasons: `wave58/synthesis-notes.md`. Wave-58 corpus
`matchups-20260904-015059`, binary `a3f0700d3`, 21 games (20 natural, 1 hung), 2,270 decisions,
108 fallbacks (4.8%); engine-side census and prediction adjudication in `wave58/engine-seat.md`.

---

# Wave-57 general strategy (carried) — R1-R292 revised, R293-R318 new

Corpus `matchups-20260903-174505`, binary `67b2bb38a`, model `qwen36-35b-a3b`. 42 seat JSONL,
3,317 decisions, 21/21 natural, 0 SEGV, 8 fallbacks (0.24%). Every figure below is re-derived on disk
by this step, **counting rows from the rendered `prompt`, never from `options_text`**, and with
`/usr/bin/grep` over the 21 stderr using the emitter's own quoted line. Where a seat and the harvest
disagree, the disk is quoted and BOTH predicates are named (#295, #298, #327).

**ENGINE items live in `wave57/engine-ledger.md`. This file carries RENDER, CORE-PROMPT, PROCESS and
the standing PASS/KEEP list.** An item appearing in both is cross-referenced; the ledger is
authoritative for the docket ranking.

---

## STATUS OF R1-R292, by the emitter's actual string

Verdicts carry from `wave56/general-strategy.md` except where a wave-57 lane (A-H, S, T, U) or this
corpus touched them. The table covers every item a wave-57 lane addressed plus every item a seat
re-filed. **Everything not listed carries its wave-56 verdict unchanged** — no wave-57 lane touched
R1-R224, R233, R239, R245, R249, R251-R252, and no seat re-filed any of them.

| item | wave-57 verdict | evidence (re-derived on disk) |
|---|---|---|
| **R225** the HOLD latch's re-open predicate | **PASS on the strict predicate, SPLIT overall — and it is #327's origin** | Strict adjacency (the NEXT record, same turn, byte-identical `options_text`): **0 of 450**. Same turn AND same phase: **8 of 450 (1.8%)**. Any later same-turn record with an identical `options_text`: **29 of 450 (6.4%)**. The wave-56 carried figure **14 of 320 (4.4%)** I could not reproduce under four predicates and it is recorded irreproducible, not wrong. Three readers filed PASS, FAIL and "the cleanest result in loop history" about the same behaviour. **The ledger pins ONE predicate for wave 58 (D8).** `hold_windows_skipped` totals **1,863** across 42 seats and the per-stderr companion line now reconciles 21/21 | **Wave-58: the DESIGN half of this row is now an ask — a life-total-only change re-opens a window the pilot itself held → R322**
| **R225(b)** the run-length half | **FAIL, and it moved the wrong way** | Identical-declined runs, wave-53 predicate (key `(turn, options_text)`, runs ≥ 3, a HOLD take breaks the run): **636 of 3,317 = 19.2%** in **96 runs**, **max run 13**, **2.26 h = 8.9% of inference** (wave 56: 14.99%, 68 runs, max 13, 5.5%). The engine seat reads 17.6% / 88 runs / 8.0% on its own predicate; the harvest read 17.1% / max 12. All three fail the wave-56 target (< 15%, max < 10) and all three agree on the SIGN. **The whole residual is one shape** — see R293 and ledger D3 |
| **R226** the ability-row life falsehood | **CLOSED, second corpus — PASS** | `they would be at K` renders **196 occurrences in 168 windows**, `at 164` / `at 165` **0**, and the engine seat re-verified `K == opp_life − damage` on **196 of 196**. `THIS WINS THE GAME` renders **3 times and was taken 3 of 3**. Stays on the PASS/KEEP list; no lane may trim it |
| **R227** the `[RE-ASK]` wrong-line quote | **UNTESTED (N = 0) for the CHOICE-line arm, THIRD wave** | `[RE-ASK]` renders on **2 prompts**, both the "not on this list" form. `plan_choice_conflict_recovered` **0**. `all_assignments_illegal` **0** across all 35 `blockers` records (`dropped_assignments: 0`, 35/35). Three recovery arms, three waves, no execution → ledger D14 |
| **R228 / R266** the bare discard rows | **PASS on the class, FAIL on the residual — and the census has FOUR live predicates** | Lane C shipped the unified verdict family. Bare-row counts by predicate: this step, tag family enumerated (#298) over 455 non-decline rows, **292 bare (64.2%)**; the engine seat, its own "computable" predicate, **334 of 455 (73.4%)** with only **6 rows the cast evaluator prices and the discard evaluator misses**; seat-125-126 **250 of 374 (66.8%)** at its own seats; the harvest's uncalibrated `{`-absent predicate **569 of 569**. Wave 56 was 83.8%. At deck123/deck130 the class is **closed: 36 of 36 annotated**. **The remaining item is not volume, it is the 6-row evaluator gap** → R294, ledger D11 |
| **R229 / R264** the missing incoming-combat total | **PASS on the emitter's window set, and the residual is the phase gate** | `INCOMING THIS COMBAT` renders **113 occurrences in 112 windows** (declared 108 / forecast 4 by the harvest; the engine seat resolves the four forms as declared 48 / closed 60 / forecast 4 / **settled 0, UNTESTED**). Windows in the emitter's own set with no line: **0 of 112**. The brief's "437 opponent-combat-phase windows" is a LOOSE phase predicate; the engine seat's true emitter set is **256** and the 144 silent windows all read `0 of them able to attack right now`. **Residual: the line is gated on COMBAT phases, so 20 opponent-turn windows with attackers ready at Main 1 / Upkeep / Draw print no total** (`125v146` s23: 3 life, 7 power incoming, no number) → **R295**, ledger D9 |
| **R230 / R262** `chosen_text` fidelity | **CLOSED — PASS, and it is the wave's cleanest lane result** | Lane A's D4: **0 of 2,960 single-row takes have `chosen_text != options_text[choice-1]`**; **0 records with `choice ≥ 0` and an empty `chosen_text`**; 7 `choice: -1` records all carry a refusal value (`<refused: named_row_reask>` ×6, `<refused: retracted_choice>` ×1, first renders ever, all correct). **The exact HOLD census reads 450 of 450 — it does, and the wave-56 prefix-match workaround is RETIRED.** Every literal census in this wave's files is trustworthy for the first time |
| **R231 / R267** the X mana-fit clause | **CLOSED — PASS 0 of N on three independent counts** | X rows with no fit clause: **0 of 55** (harvest), **0 of 52** (engine seat, 12 ANNOUNCE_X menus), **0 of 136** (deck123/130 seat), **0 of 28** (deck125 seat). Wave 56 was 40 of 40 without. `{X pricing:` renders **580 occurrences in 538 windows**. Residual is cosmetic: **three different wordings for one fact, and the X=0 form (7 rows) has no `{leaves …}` sibling** → R296 |
| **R232 / R259 / R260 / R261** the MDFC land row and its three siblings | **ALL FOUR CLOSED — the wave's biggest clean sweep, and the lane that broke a rule of Magic fixed it and the two counts underneath it** | `PLAY THIS AS A LAND` renders **108 occurrences in 76 windows / 15 takes** (wave 56: 65 rows / 6 takes / **44 illegal**). Lane W's legality gate holds. The pregame `counted TWICE above, on purpose` note renders on **6 pregame prompts**, and headers omitting a `forcetype(land)` card: **0 of 6** (was 4 of 4). `Flip Side` renders **0 times in 3,317 prompts** (was 109) — D14 PASS, and the display-toggle auto-pass line rose 760 → **1,309**, which is the fix WORKING (candidate-level drops with no prompt cost), max consecutive run **4**, unchanged. **A new HIGH opened underneath them: the pay-3-life arrival row carries no lethality verdict** → **R293**, ledger D1 |
| **R234** the opponent's open mana | **CLOSED — PASS, third corpus** | `Their untapped sources:` on **3,267 of 3,267** board-bearing prompts (100%); the 50 misses are pregame + `bottom`, which are HAND-ONLY by owner directive. PASS/KEEP |
| **R235 / R282** plan staleness | **UNTESTED, cause (a) — no window** | `is withdrawn (you stated it,` renders **0**. `plan_echo_count` present on 1,628 records. D11's two withdrawal rules have now gone two waves with no firing window; the ask is a forcing flag or a fixture → ledger D14 |
| **R237 / R263** the async slot | **HEADLINE FAIL, MECHANISM PASS — and the two must not be conflated** | `dropping stale async answer` **108** (target < 20; wave 56: 65). Arms, re-derived from the line's own text: **casting 63, land-drop 45**. Cross-tabbed against the next ask in each stderr: **land-arm drop → `Land drop:` ask 45 of 45; casting-arm drop → `Casting decision` 48, other seam 13, none in window 2. CROSS-ARM DISPLACEMENT: 0 of 108.** The second slot does exactly what lane A built it for; the residual is **intra-arm** drift and it is now the corpus's largest single inference sink. `consuming an in-flight answer whose prompt text drifted` fires **11** times (wave 56: 8). **`asyncSlotDriftKind` renders 0 times anywhere** — the carry's expectation of that literal was wrong; the real field is the arm-labelled string → ledger D4 |
| **R241 / R281** the plan-conflict census | **CLOSED — PASS** | `decision_reversed_in_prose` **6**, always co-stamped with `plan_choice_conflict_narrowed` **6**, firings **0**; broad == narrowed + firings holds. The wave-56 defect ("0 conflicts" indistinguishable from "0 counted") is gone |
| **R242 / R268** the nested blocking-trigger clause | **PASS — 0 of 51 B-lines nest a gain** (was 10 of 36), and 1,457 of 1,457 on the deck125 seat's own count. One residual: the ATTACKER's lifelink still nests at the attackers seam (`152v126` s20, 3 occurrences in 1 record) → R297 |
| **R243 / R270** the menu-level dead verdict | **SHIPPED AND IT IS FALSE ON 8 OF ITS 497 RENDERS — the wave's second HIGH** | `NO LIVE CAST ROW ON THIS MENU` renders **497 windows** (443 in the "all 1 cast row" form). Windows where a cast row's own `{right now:}` brace names a live effect underneath the header: **8** (deck123 ×7, deck126 ×1), **1 cast taken from one**. The deck123/130 seat reads **6 of its 92** on a slightly wider predicate that also counts the Tragic Slip class; both are true of different units. Specimen: `123v130` s45/s49/s50 — `Cast Devour Flesh … Rorix Bladewing (6/5) [flying, haste] is sacrificed` under a header saying every row reads zero; **the pilot obeyed the header three times and the flier killed it** → ledger D2, skill #301 |
| **R246** the unpriced equipment row | **CLOSED, second corpus — PASS 0 of 45 rows** | `you control 0 creatures - this equips nothing` renders 45 rows, **0 takes**, seventh consecutive corpus for the rung and the FIRST post-fix one (skill #305 re-dates it) |
| **R247 / R288** the duplicate-verdict pair | **FAIL — the tag never rendered** | `{identical verdict right now to row ` renders **0 times in 3,317 prompts**. Lane C predicted ≥ 1. One live unmarked cross-card window exists (`146v126` s25, Agadeem's Awakening vs Silverquill Command). D15's sibling tag rendered twice, so the mechanism is reachable and this specific tag is not → ledger D13 |
| **R248 / R277** the `[<- …]` marker | **PASS on coverage, FAIL on definition — and the failure is a lost game** | `[<- ` renders **518 windows / 21 takes**. Coverage: X cast rows with an affordable ladder and no marker **0** (526 rows, 506 marked, the 20 unmarked all X=0). **But `[<- best trade:` renders exactly ONCE in the corpus, was taken, and marked a 1-for-5**: `130v152` s33 turn 14, `X = 4 {X pricing: kills THEIRS: Sigarda, Champion of Light; YOURS: Dwarven Blastminer, Siege-Gang Commander, Goblin x3}`. deck130 lost that game at −20. And the sibling `[<- best X for this cast:` **moved seam** — 408 windows on the CAST row, where two guides located it "one screen later" → **R298**, ledger D5, skill #318/#319 |
| **R250 / R285** the `(xN)` log collapse | **CLOSED — PASS 0 adjacent duplicates** (was 1,767 in 670 records). `(x2)` renders 1,754 occurrences in 698 windows, `(x3)` 127 in 74; `drew N cards` 4,435. The narration is materially cheaper and nothing was lost |
| **R253** resource 404s | **OPEN, unchanged in character** | no correctness effect observed; not docketed |
| **R254** the DRAW FORECAST product | **PASS/KEEP, but its denominator collapsed (#303)** | `DRAW PUNISHERS` renders fell to **49 prompts at deck123/130** (was 109) and deck130's own to 2. A 0-breaks verdict on 2 renders is UNTESTED, not PASS |
| **R255** the `WAGIC_GPT_BOARDINDEX=0` segfault | **CLOSED — PASS, second corpus** | 0 `Segmentation` in 21 stderr, 21/21 natural. Lane F's D25 sweep and the six zeroed `MTGCardInstance` members ride this binary; **no ASAN surface reached the corpus** (the disable flag `WAGIC_NO_DEADREF_SWEEP=1` was not exercised — UNTESTED by construction) |
| **R256** `cache_hits` on `gameend` | **OPEN — FIFTH wave** | present on 0 of 42 gameends. Lane M's counters still reach no log → ledger D20 |
| **R257** per-decision inference at fixed `-j` | **CONFIRMED A THIRD TIME** | mean prompt ROSE 11,185 → **12,198 chars (+9.1%)** while per-decision inference FELL 27.97 → **27.55 s (−1.5%)** at the same `-j 21`. The prompt-size-drives-latency hypothesis predicts the opposite sign for the third consecutive wave. **Byte trims remain context-headroom items, never latency items**, and the log-window A/B must not be scored on latency alone |
| **R258 / R284** the log window | **OWNER ANSWERED — "measure." The mechanism is built and the control arm is verified** | `log_window` = `full` on **3,317 of 3,317** records; `log_window_kind` present on all. **The A/B has not run** — it is a wave-58 step (ledger D10). Two measurement defects found in the control arm: the field carrying the elided-turn count **does not exist in the record set at all** (46 keys enumerated), and `log_window_kind` labels only **28.3%** of decisions, leaving **71.7% `unclassified`** → **R299**, ledger D10 |
| **R265** the opponent-converter cast-row price | **PASS on the emitter's scope, and the docket's number was a scope error** | `{their converter:` renders **2 occurrences in 2 windows, 2 takes**. The brief's *"80 of 82 creature cast rows lack it"* counted the whole prompt, including boards where the converter is the seat's OWN; on the correct scope (converter on THEIR battlefield) the engine seat reads **0 of 2**. Thin but PASS. **The open half is arithmetic, not coverage**: the price counts one iteration while the same prompt's own paragraph names a Sanguine Bond + Exquisite Blood LOOP — a printed `52 → 37` against a true `52 → 0` (`125v126` s293) → **R300**, ledger D12 |
| **R269** the stack-aware `{right now:}` | **PASS 1 of 1 — thin** | edict rows naming an already-targeted victim with no tail: 0 of 2 (engine seat), 1 of 1 correct at deck126 (`126v125` s437/438); redundant second Tributes **0 of 12** (was 3 of 10) |
| **R271** the narration budget by ask kind | **MEASURED, NOT DECIDED — and the measurement moved** | GAME LOG above `--- CURRENT SITUATION ---`: **64.0%** of the mean prompt (wave 56: 54.0%); everything above the first option row: **87.8%** (was 82.7%). Per-deck the spread is real: deck123/130 seat **56.9%**, deck125/126 seat **67.6%**, deck162 **48.3%**. Per-kind: `ask` 2,642 records / 12,022 mean / **31.76 MB**, `priority` 481 / 12,895 / 6.20 MB, `discard` 66 / **15,872** / 1.05 MB, `attackers` 76 / 11,360, `blockers` 35 / 11,174, `reveal` 13 / 14,208, `bottom` 4 / **2,354** — the pregame class already ships at ~0% narration, which is the existence proof. **Total 40.46 MB.** Owner's ruling stands: the A/B decides |
| **R272** the `Flip Side` toggle | **CLOSED — PASS 0 renders** | see R232 |
| **R273** the opponent LIFE TREND | **CLOSED — PASS 3,267 of 3,267** (was 1 of 137). `since turn N` on 100% of board frames. Two seats independently confirm at 1,025/1,025 and 1,457/1,457. **Extension asked**: the pilot's OWN life has no trend line, and `130v126` t16 went 18 → 0 in one phase with no slope shown → R301 |
| **R274** the fetch row's positive colour form | **PASS 0 of 30** — positive form 117 occurrences in 29 records, negative 13 in 4. **One residual**: the negative form drops the colour it CAN make (3 renders, `123v126` s17, `123v130` s31/s40) → R302 |
| **R275** the 3-life follow-up on the MDFC row | **PASS on the arrival clause, and the CORRECTNESS half is now HIGH** | `enters TAPPED` renders 34, `pay 3 life` 116 occurrences in 104 windows / 9 takes; back-face land rows missing the arrival clause **0 of 7** (23 Pathways correctly silent); false untapped-arrival plans **0 of 4**. **But the row still carries no LETHALITY verdict, and a seat killed itself on it** → **R293**, ledger D1 |
| **R276** the assignable remainder | **SHIPPED, and it printed a SURVIVABLE number on a lethal board** | `best case with every blocker assigned` renders **34**; `one legal assignment gets you to` **6 occurrences in 5 windows**. Lethal-header blockers windows requiring the seat to subtract: **0 of 20** (deck146/152/162 seat), **1 of 6** (engine seat; the 1 is the documented 32-a-side cap), **1 of 8 FALSE** (deck125/126 seat). The false one is the item: `126v152` s14, turn 11, seat at 5 life, printed `one legal assignment gets you to 1` against a true floor of **−5** (trample damage dropped from the baseline). The model self-caught it and blocked correctly anyway → **R303**, ledger D3 |
| **R278** cycling row pricing | **PASS on the clause, UNTESTED on the behaviour** | cast-mode rows missing the displaced-path clause **0 of 32**. Cycle takes at opponent life ≤ 6: **2 of 11 (18.2%)** against wave 56's 3 of 17 (17.6%) — flat, and N is too small to resolve (#292) |
| **R279** the blocking-trigger total | **PASS 0 of 6 — thin, and the converter half is UNTESTED** | `BLOCKING THIS COMBAT:` renders **6**; declines-all-blocks-under-a-converter **0 of 6**. `{blocking trigger` 14 occurrences in 6 windows. 0 renders at three of the seven seats |
| **R280 / R283** `{feeds:}` in hand | **SHIPPED — PASS on the render, FAIL on the behaviour, and the failure is a GUIDE item** | `{feeds:` renders **88 occurrences in 60 windows / 20 takes**, and 88 of 88 carry the hand half. Takes at battlefield-converters = 0 with a hand converter: **4 of 20** (was 1 of 3). The engine seat's verdict is the right one — *the render is TRUE and the rule is being broken* — so this routes to deck162's guide (skill #322/#326), not to the render. The one render ask that stands: `{feeds:}` states a FACT where its neighbours state a VERDICT → R304 |
| **R286** the display-toggle census | **CLOSED — the census now says what it should** | `only display-toggle … auto-passing without a model call` **1,309 lines** (was 760), tail `(suppressed 1 land-face toggle row(s))`, max consecutive run **4**. Higher is the fix working |
| **R287** `hold_windows_skipped`'s stderr companion | **CLOSED — PASS 21 of 21** | per-stderr `windows held` totals reconcile to the sum of the two seats' `hold_windows_skipped` on every game; corpus total **1,863**. The `took the hold row at the priority seam` line renders **124** times |
| **R289-R292** the four measurement amendments | **ALL FOUR HELD, AND ALL FOUR EARNED THEIR KEEP THIS WAVE** | #294 (whitespace-normalised audits) found B1 below · #295 (quote the log line) is why `asyncSlotDriftKind` was caught as a non-existent literal · #298 (name the tag family) is why the discard census has four honest numbers instead of one wrong one · #300 (denominator in the verdict) is why R225 reads SPLIT rather than PASS. **#327 is added because #300 was not enough**: the denominator was printed and the PREDICATE still was not |

---

## NEW — RENDER, HIGH

### R293. The pay-3-life MDFC arrival row is the only menu in the game that can kill the pilot outright, and it is the only menu with no verdict on it.

`146v130` **seq 20**, re-derived by this step in full. The board frame reads `Your life: 1 |
Opponent life: 20`. The menu is two rows:

```
1. pay 3 life - Emeria, Shattered Skyclave enters UNTAPPED [usable (tap for mana / attack) this turn]
2. tap - Emeria, Shattered Skyclave enters TAPPED [decline the payment; unusable until your next untap step]
```

The seat took row 1. The **next record in the file is `gameend`, `my_life: -2`, `won: false`, turn 19,
opponent life 20** — the opponent did nothing. The reply's plan was to cast Kaya the Inexorable
(`{3}{b}{w}`) off the newly-untapped land, against a printed `Mana available: 1 total`.

Ten windows in this corpus put a `pay 3 life` row in front of a seat at ≤ 3 life, nine of them in this
one game. **The engine renders a lethality verdict on every other menu it prints** — `they would be at
K; THIS WINS THE GAME` on ability rows (196 occurrences, 3 lethal, 3 taken, 3 wins), `this KILLS you`
on blockers headers (27 renders), `best case with every blocker assigned` (34) — **and none on the one
row in the game that ends it**. The magnitude is not hidden: it is `life:-3` in the card's own script
(`borderline.txt:1199`), which is where the row's own text comes from.

**Ask.** Price the payment on its own row, unconditionally:
`{this payment puts you at N}`, and when `N ≤ 0`, `{you are at L life: paying 3 puts you at N and you
LOSE the game}`. Same register as `THIS WINS THE GAME`, opposite sign. **ENGINE/RENDER, ledger D1.**
The deck146 seat shipped the guide route-around (`146-A`: default to `tap`, two-clause release, hard
floor at 3 life) and it is correct to have done so — but a guide floor is a route-around under #285's
retirement machinery, and this render deletes it.

### R294. The discard evaluator misses six rows the cast evaluator prices, and the two evaluators disagree by a VERB WHITELIST.

Two independent findings, one root cause.

**(a) The whitelist.** `rowSaysNoOp` tests a fixed set of verb phrases — `does nothing`, `deals 0`,
`destroys 0`, `kills 0`, `removes 0`, `drains 0`, `does not apply`, `gains 0`, `draws 0` — and
**`exiles 0` is not in it**. Measured on disk: **9 rows across 8 windows** read
`{right now: exiles 0 of their creatures (0 without a restriction against attacking), 0 of yours}` —
a provably-zero cast — and **not one is badged `{dead right now:}`, and not one triggers the
`NO LIVE CAST ROW` header** (`hdr = False` on all 8). The identical-shape `destroys 0` rows on
Supreme Verdict and Damnation DO trigger it, 443 windows' worth. Every one of the 9 is a Final
Judgment at deck125's seats (`125v123` s187/s188/s189, `125v130` s14/s16/s98/s99/s100).

**(b) The evaluator gap.** The engine seat found the general form: **6 of 455 discard rows carry a
verdict the CAST evaluator computes and the DISCARD evaluator does not** — Lightning Greaves ×2
(`123v125` s77/s81), Silverquill Command and Barrowin (`146v162` s17/s24).

**Ask.** Replace the verb whitelist with the zero-magnitude predicate the row's own evaluated
magnitudes already carry (the number is computed; the badge is decided by string matching on the
sentence built from it), and route the discard menu through the same evaluator as the cast menu.
**ENGINE, ledger D11.** Note the honest half: the pilot answered 7 of the 9 correctly anyway.

### R295. `INCOMING THIS COMBAT` is gated on combat PHASES, so a seat at 3 life facing 7 power at Main 1 sees no number.

The line's own window set is clean — **0 of 112 missing**. The gate is the phase, not the ask: **20
opponent-turn windows carry attackers ready and no total**, split Main 1 ×10, Upkeep ×6, Draw ×4.
Sharpest: `125v146` **s23**, seat at **3 life**, **7 power** able to attack, no line on the prompt.

**Wave-58 status: SHIPPED and PASS on the window class it was gated to (lane C / D9: 10 of 10
opponent-turn windows carry the line, `wave58/engine-seat.md` §2). The window class this item's own
first sentence names — the seat's OWN turn — is still silent, and deck123 lost two games in it →
R319.**

This is the same shape as wave 56's R264 one level up: the number that decides the game is computed
and then withheld from the windows before combat, which are exactly the windows where a response is
still possible. **RENDER, MED-HIGH, ledger D9.** Ask: emit the `forecast` form
(`not declared yet - N of their creatures can attack, for up to M`) at every opponent-turn window from
Upkeep onward, not only inside the combat phases.

---

## NEW — MED

**R296. One fact, three wordings, and the X=0 form has no fit clause.** `{X pricing:` renders 580
occurrences; the fit clause exists in three different wordings across 401 cast rows, 28 announce rows
and 7 rows with none — and the 7 are all the `X = 0 {X pricing: X=0 - this cast does NOTHING}` form.
Give X=0 the same `{leaves N of your M floating mana unspent}` sibling its neighbours carry. Cosmetic,
cheap, and it removes a case where the pilot must infer that the cheapest row is also the emptiest.

**R297. The attacker's lifelink still nests inside the survival verdict at the ATTACKERS seam.** D10's
fix is clean at the blockers seam (0 of 51 B-lines). `152v126` s20 carries **3 nested occurrences in
one record** at the attackers seam. Lane B's own residual note says the un-nesting assumed a
blockers-only call site; this is the un-guarded case.

**R298. The `[<- best trade:` marker's definition is not the pilot's goal, and its single render this
corpus was a 1-for-5 that lost a game.** Print the two counts inline (`THEIRS 1 / YOURS 5`), or
suppress the `best trade` form when `YOURS > THEIRS` (the free-trade form already exists and is
correct). And record the seam move: `[<- best X for this cast:` now renders on the CAST row, 408
windows / 11 takes. See ledger D5, skill #318/#319. **This is #248's flip side**: the marker is the
most-followed annotation the render produces, which makes a wrong definition expensive rather than
harmless.

**R299. The log-window instrument cannot yet measure its own experiment.** Two defects in the control
arm, both found by the engine seat and both reproduced here: **(a)** the field carrying the
elided-turn count **does not exist in the record set** — 46 distinct keys across 3,408 records, none of
them it — so the A/B's arm-verification census has nothing to read; **(b)** `log_window_kind` labels
only **28.3%** of decisions (`land_drop` 377, `empty_stack_pass` 368, `combat` 111,
`cleanup_discard` 66, `target_or_reveal` 13, `pregame` 4) and **71.7% are `unclassified`** — but the
per-kind budget is defined ON those labels, so arm B (`kind:3`) would window ~28% of the corpus and
silently leave the rest at `full`. Lane H's own residual note predicted exactly this (the classifiers
are PARSETEST-proven and never corpus-observed) and named the control-arm census as the first job of
the harvest. **Fix both before the A/B runs** → ledger D10, and D10 blocks the owner's D43 decision.

**R300. `{their converter:}` prices one iteration where the same prompt names a LOOP.** `125v126` s293:
the prompt carries a Sanguine Bond + Exquisite Blood paragraph 40 lines above a creature cast row whose
price reads `52 → 37`. The true cost of feeding that board is `52 → 0`. Two renders, two takes, and it
is the fourth corpus in which this row's arithmetic is "the most reassuring number on the menu."
Ask: when a life-loop is on the board, the converter price prints the LOOP verdict, not the single
iteration.

**R301. The pilot's own life has no trend line.** `since turn N` is on 100% of board frames for the
OPPONENT and nowhere for the seat. `130v126` t16: 18 → 0 in one phase, no slope shown. Same emitter,
one more call.

**R302. The fetch row's negative colour form drops the colour it CAN make.** 3 renders (`123v126` s17,
`123v130` s31/s40). D19's positive form is correct and shipped (117 occurrences); the negative form
should carry both halves.

**R303. `assignableRemainderDamage` drops trample from its own baseline.** `126v152` s14, turn 11, seat
at 5 life: printed `one legal assignment gets you to 1` where the true floor is **−5** (−1 counting
triggers). The line is a BOUND the pilot is instructed to trust, and it was optimistic in the lethal
direction — the worst possible sign for a bound. deck125's #G2 states the general precondition and it
is adopted: **any printed bound must be conservative in the non-lethal direction, or it must not be
printed.** Ledger D3.

**R304. `{feeds:}` states a fact where every brace around it states a verdict.** 88 occurrences, 20
takes, 4 of them at zero battlefield converters with a hand converter. Its neighbours read
`{dead right now: …}`, `{right now: returns NOTHING}`, `[legendary: …]`. Give `{feeds:}` the same
outcome shape. **Not a legality ask** — deck162's reviewer states this explicitly and correctly.

**R305. The identical-declined note needs a SECOND number.** `[you declined this exact list N times
already this turn]` renders on **1,154 prompts**, **801 at N ≥ 2**, **max N = 43** (`130v126` s125,
against a wave-56 max of 19). The note counts option LISTS; the situation block underneath it changes
on the overwhelming majority of them (the deck146/152/162 seat's independent count: only **163**
corpus-wide windows genuinely repeat a situation block; at deck152 **0 of 66**). Ask: print
`the board has not changed since M of them`. This is the render half of skill #321, and it is what
turns a mis-keyed signal into a usable one. Ledger D7.

**R306. Cheap-cut candidate, for measurement only, not for removal.** The declined-note line renders
105-276 times per seat and produced **0 decline → cast flips in two consecutive corpora**, while the
HOLD row it sits above renders 2,490 times and is taken 450. deck123's G57-3 proposes the note as an
A/B arm alongside the log window. **Recorded as a measurement candidate, not adopted** — nothing is
removed on this evidence, and R305 may be what makes the line work.

---

## NEW — LOW

**R307.** The GAME LOG still lowercases script tokens where the row head no longer does. D28 fixed
`chosen_text` (0 hits) and the row head; the narration's `used:` emitter did not move. Re-derived by
this step over the log region only: **698 occurrences in 422 prompts** (`boulderloft` 313,
`tidechannel` 181, `grimclimb` 79, `agadeem` 75, `pelakka` 22, `emeria` 14, `skyclave` 14). The
deck146/152/162 seat reads 684; the engine seat reads 75 at its own seats on a narrower token list.
Specimen `123v146` s2: `- Opponent used: grimclimb pathway with Brightclimb Pathway`.

**R308.** The `NO LIVE CAST ROW` token disagrees with its own verb on 443 of 497 renders:
`all 1 cast row below carry a verdict`. One-character fix.

**R309.** A multi-select `chosen_text` loses the instance handle: `126v123` s25 reads three distinct
Walls of Omens as one blocker. Lane A's D4 fixed the single-row seam and explicitly did not audit the
multi-select seams.

**R310.** The pregame `Mana sources among those lands` line counts only the shown face of a land/land
Pathway. `152v125` s1; deck152 played the alternate back face 7 times this corpus. Same shape as D2's
fix, one class down. Lane E deliberately left Pathways untouched pending a "one of these, not both"
grammar — this is that item.

**R311.** A one-row decision menu still costs a full round trip (`130v162` s8, an X=0-only menu), as do
two-row target menus whose only alternative is a self-shoot (`130v125` s92/s94). **Recorded as UX, not
as a cap** — deck130's reviewer frames it that way explicitly and correctly; nothing is withheld and no
choice is constrained by saying so on the header.

**R312.** `repeat_n` renders on 14 records and has no harvest consumer, second wave.

---

## CORE-PROMPT

**R313. On a naturally-terminated reply carrying exactly two legal `CHOICE:` lines, take the LAST and
stamp `choice_revised`.** `126v146` s34 opened `CHOICE: 3`, reasoned ~900 words, closed
`So CHOICE: 1`, and was refused as `retracted_choice`. It was the seat's last live window; the seat
died at 2 life on turn 20. Rate **1 in 3,317**. The parser's current rule protects against a genuine
ambiguity that this shape does not have: a reply that terminated naturally and states one final choice
is not ambiguous, it is revised. **Ships with its negatives**: a truncated reply keeps the refusal, and
three or more CHOICE lines keep the refusal. Ledger D6. The GUIDE half is skill #314 and it does not
substitute for this.

**R314. The `[RE-ASK]` notice should say WHY the named row is not on the list.** 3 of 3 occurrences
this corpus name a card off a stale echoed PLAN (`126v123`: Sanguine Bond). Ask: append
`(it is on your hand line; nothing in this window can cast it)`. Cheap, and it converts a bare refusal
into the fact that would prevent the next one.

---

## PROCESS / MEASUREMENT

**R315. A prediction that counts a repeat, run, drift or adjacency states its PREDICATE inside the
prediction sentence** (skill #327). D1's same-turn re-ask reads **0 / 8 / 29** on three defensible
predicates this wave and **14** in the carry, which I could not reproduce under four. Three readers
filed three verdicts about one behaviour. #300 put the denominator in the verdict; that was not
enough.

**R316. A byte-size obligation names its BASELINE FILE** (skill #328). Five of seven reviewers' net
changes disagree with this step's, and every number is right — the reviewers measured against the
DEPLOYED guide (post-boundary-pass), this step against `wave56/deckN/strategy.txt`. Both columns are
published in the boundary section below, and the deployed file is the default baseline from now on,
because the deployed file is what the corpus served.

**R317. An experiment at a stable large ratio over three corpora is CLOSED and its control released**
(skill #329, discharging #299). See #312: the HOLD-row control's general finding — **an untaught row is
taken at ~1/20th the rate of a taught one** — is now a scoring calibration for every render proposal
this loop weighs.

**R318. A lane's residual list is a wave-58 docket input, not a footnote.** Eleven of this wave's
ledger items came from lane residuals rather than from corpus surfaces (lane V's two, lane F's four,
lane H's three, lane G's console/touch/divided-damage, lane E's Pathway grammar). Lanes are already
writing them; the docket must ingest them mechanically.

---

## PASS / KEEP — recorded so no later lane trims them

The corrected `they would be at K` tail and its `THIS WINS THE GAME` flag (196 / 3 / 3 wins) ·
`Their untapped sources:` (3,267/3,267) · `Opponent life trend:` / `since turn N` (3,267/3,267) ·
`INCOMING THIS COMBAT` on its own window set (0 of 112 missing) and its four forms ·
`best case with every blocker assigned` (34) · the un-nested blocking-trigger clauses ·
the X marker family, its largest-first header and its fit clause (0 of 55 unpriced) ·
the `PLAY THIS AS A LAND` row **and its legality gate** (108 rows / 15 takes / 0 illegal) ·
the pregame `counted TWICE above, on purpose` note (6 renders, 0 of 6 hands under-counted) ·
the `{taking this row skips …}` HOLD tail (2,490 rows, 450 exact takes, **450 of 450 byte-exact**) ·
the `(xN)` log collapse (0 adjacent duplicates) · `Flip Side` suppression (0 renders, 1,309 auto-passes) ·
the display-toggle census and its `(suppressed N land-face toggle row(s))` tail ·
`[NAMED BY THEIR …]` and its per-cast clause (16/16) · `<refused: …>` in `chosen_text` (7/7) ·
`reveal_wait_ticks` / `reveal_wait_secs` (13/13) · `mana_only_windows_skipped` ·
the per-stderr `windows held` companion (21/21) · `decision_reversed_in_prose` beside its narrowed stamp ·
`[second copy:` (79 renders / 23 takes) · the fetch row's positive colour form (117) ·
the `{dead right now:}` / `{right now:}` discard family and its legend.

---

## REJECTED, and why

- **"Skip the opponent-turn window whose `ON THE STACK` is empty at a phase this seat has never cast
  in"** — seat-125-126 MED #4, **SIXTH wave running**, on its biggest denominator yet (deck125 237 of
  376, deck126 279 of 355). **REJECTED again.** It removes a legal window from the model. The sanctioned
  levers are R305 (tell the pilot what changed instead of hiding the window), R306 (measure the note),
  and the HOLD row — which is at **20.9% on the opponent-turn arm corpus-wide** and which the model
  takes *itself*. #312 now prices the distance from 20.9% to 100% precisely: it is a GUIDE problem
  worth ~20× and not a licence.
- **"Do not ask when the menu has exactly one legal row"** (deck130 G57-5b) and **"extend
  `mana_only_windows_skipped` to more forced-ask classes"** (deck125 #G5). **NOT ADOPTED as engine
  behaviour; the HEADER halves ARE adopted** as R311. Both reviewers disclaim a cap explicitly and
  correctly, and both are right that the round trip is real — but "the menu has one row" is a decision
  the engine would be making for the model, and the standing ruling rejects it on sight. What the
  header may say is what is TRUE: *"every row on this menu spends the card; the decline was the
  previous window."*
- **A guide compensation for R293, R294 or R303.** **REJECTED as doctrine, ACCEPTED as debt.**
  deck146's `146-A` (default to `tap`, hard floor at 3 life) ships and is correct — it is built from
  TRUE surfaces (`Your life:` and the row's own text) and it carries a release condition, which is what
  #287 and #285 permit. No guide may hedge against a printed bound (#324, and R303 is exactly a printed
  bound that lied); the fix is the render's.
- **Reordering the decline rows / hoisting HOLD above `Cast nothing right now`.** **Not re-filed by any
  seat this wave** — recorded so it is not read as newly open. Standing owner ruling; declines last.
- **Any win-rate attribution.** deck152 4/6, and 123/126/162/125/130 all **3/6**, deck146 2/6 — the
  flattest band on record, n = 6 per deck, real hands, no seed pinning. Counts only (#87, #167, #217,
  #292). All seven seats correctly recorded **NO EXIT**.
- **The harvest's `569 of 569` bare discard rows, the brief's `437` opponent-combat windows, the
  brief's `80 of 82` unpriced converter rows, and the brief's `125v130` max-prompt file.** All four
  **OVERTURNED on disk** and none of them is an error of arithmetic — each is an uncalibrated
  predicate (#295, #298) or, in the last case, a filename transposition: the max prompt is **30,228
  chars at `125v162` seq 175**, turn 27, `ask`.

---

## MEASUREMENT — what wave 58 must report

1. **R293's row**, with the lethality verdict: `pay 3 life` takes at `my_life ≤ 3` — **0 of N**
   (wave 57: the row rendered at ≤ 3 life ten times, was taken once, and that once ended a game).
2. **R294**: rows reading `exiles 0` with no dead badge — **0 of N** (wave 57: 9 of 9); discard rows
   the cast evaluator prices and the discard evaluator does not — **0 of N** (wave 57: 6 of 455).
3. **R243/D2**: `NO LIVE CAST ROW` windows carrying a live-verdict cast row — **0 of N** (wave 57:
   8 of 497 on this step's predicate, 6 of 92 on the seat's).
4. **R237/D4**: `dropping stale async answer` **< 40** corpus-wide (wave 57: 108), cross-arm **0**
   (already 0), and the drop line states its OUTCOME per drop (fired / seam-changed / no matching slot).
5. **R225/D8**, in the pinned predicate (#327): *the NEXT record at the same seat, same turn, with a
   byte-identical `options_text`* — **0 of N** (wave 57: 0 of 450). Reported alongside the loose
   predicate (29 of 450) so the two never merge again.
6. **R225(b)/D3**: identical-declined share **< 15%**, max run **< 10**, on the wave-53 predicate
   (wave 57: 19.2%, 96 runs, max 13, 8.9% of inference).
7. **R299/D10**: the elided-turn field EXISTS on 100% of records, and `log_window_kind` labels
   **> 90%** of decisions (wave 57: field absent; 28.3% labelled). **Both must pass before the A/B
   can be read at all.**
8. **HOLD**, once more and then never again as an experiment (#317/#329): deck125 against **15.9%**,
   deck126 against **0.6%**, pool against **18.1% / N ≥ 3 9.6%**.
9. **The pool band and BOTH net-change columns** (#328), with each trim's own prediction (#270).

---

# WAVE-58 ADOPTED — R319-R330

Twelve items, from four seats' `general-proposals.md`, corroborated where noted by
`wave58/engine-seat.md` and by seat reviews that filed the same shape without proposing it.
Every item below **adds a true fact to a window**; none removes a row, caps a choice, or lets the
engine decide a menu is not worth asking (standing ruling, wave-57 REJECTED list).

## NEW — RENDER, HIGH

### R319. `INCOMING THIS COMBAT` shipped gated on the opponent's turn, so the window class R295 was written from — the seat's OWN turn — is still silent, and the count it needs is "creatures that WILL untap", not "creatures able to attack right now".

*(deck123 G1; R295 / ledger D9's residual. Lane C's own scope note: "untap, cleanup and the
post-end steps stay silent, as does the seat's own turn.")*

Two repros, two window classes, both on the pilot's own turn:

1. `1788504699-ai_baka_deck123-...-vs-ai_baka_deck162.jsonl` **s23** — Attackers, turn 12, own life
   **3**. The block prints, per attacker, only `[held back, it CANNOT block: Fog Bank (flying),
   Master of the Feast (flying)]` and `[their untapped blockers: Fog Bank (0/2) (neither dies …)]`.
   Their Master of the Feast (5/5 flier) and Fate Unraveler (3/4) were both `[tapped - cannot attack
   or block this turn]`. The seat sent all three untapped bodies for a non-lethal 13; **s24** then
   printed `INCOMING THIS COMBAT: 2 attackers, 8 unblocked damage - you would be at -7; this KILLS
   you … no block saves you`. Every fact in the s24 line was determinable at s23.
2. `1788504687-ai_baka_deck123-...-vs-ai_baka_deck152.jsonl` **s12** — Main 1, own turn, life **5**,
   facing 17 power (Katilda 3/3, Elite Spellbinder 5/3 flier, Intrepid Adversary 5/3, Briarbridge
   Tracker 4/5), all but the Tracker `[tapped]`. No incoming fact. The seat tapped out for a 3/3 and
   died at -12.

**Ask.** Emit the forecast on the pilot's own turn-adjacent windows, sourced from the creatures that
will be able to attack on the opponent's NEXT turn — which on both repros means counting the
`[tapped - cannot attack or block this turn]` bodies, because an able-to-attack-right-now count
renders **0** on the exact board that kills the pilot. At the Attackers ask it additionally carries
the delta the decision controls: what that total becomes against the blockers the pilot would have
left if it sends this list. Keep D6's under-claim rule (upper bound over able attackers, no trample
carry-over claim). **Second half, free:** the per-attacker tag enumerates only what a body CANNOT
block; at `vs162 s23` the fact that decided the game was what each body COULD block (Keeper 7/7 vs
the 5/5, a 4/4 vs the 3/4) — the same computation s24 runs one window too late. Emit the `may block`
side at the attackers ask.

*Guide interaction:* deck123's wave-58 E1 (`CHECK 0 - THE CRACK-BACK`) computes this by hand from
true surfaces, which #287 permits for an ABSENT number. When R319 ships, that paragraph takes
#310's `perception status: CLOSED, wave NN` clause.

### R320. A hand card that has no cast row is never told why, and three seats paid for it in re-asks and lost windows.

*(deck126 P3 + deck123 G2 + deck162 HIGH-3, independently filed.)*

`1788504695-ai_baka_deck126-...-vs-ai_baka_deck125.jsonl` **s34**: 4 untapped sources, hand line
carries `Exquisite Blood {4}{b} [enchantment]`, the menu is one Tribute-at-0 row plus decline plus
hold; reply `CHOICE: 1 (Cast Exquisite Blood)` → `<refused: named_row_reask>`, window spent. The
strings `not castable` / `needs N sources` occur **0 times in 305 prompts** of that seat; the only
appearance of the idea is a model-written PLAN doing the arithmetic itself and reaching the wrong
answer. deck123 spent **7** `named_row_reask` windows in six games, 6 of them in one game
(`...vs-ai_baka_deck126` s8, s14, s19, s22, s26, s29) all naming `Cast Thraben Doomsayer` against a
printed `Mana available: 2 total`. Corpus-wide: 10 `named_row_reask`, plus the 11
`stale_echo_in_range` → `named_row_reask_recovered` pairs (engine-seat MED-5), each a full extra
round trip.

**Ask.** Append one true clause to each hand-line card with no cast row this window, from the same
computation that suppressed the row: `Exquisite Blood {4}{b} [enchantment] [no cast row: needs 5
sources, you have 4]`; for a colour shortfall `[no cast row: needs 2 sources that make {b}, you have
1]`. The land-drop line already sets the precedent for saying why something is absent from the
choices. Adds a token rather than removing one, and it is the direct antidote to the class the reply
protocol currently spends a paragraph of every guide on. **Prediction:** wave-59 `named_row_reask`
falls to 0 on seats whose guides do not change.

### R321. A search / reveal row must carry the price its own cast row will carry one window later.

*(deck126 P2.)* `1788504664-ai_baka_deck126-...-vs-ai_baka_deck146.jsonl` **s47** (turn 30, life 1):
34 unannotated rows of the form `N. <name> (<type>) {text: …} [eligible for "choose card"]`,
including `10. Sanguine Bond` and `28. Exquisite Blood`, with `Silverquill Silencer {b}{w} (4/3)
[named: Sanguine Bond]` on the opponent battlefield line. Pick = Sanguine Bond. **s48**, the very
next window: `3. Cast Sanguine Bond {3}{b}{b} [NAMED BY THEIR Silverquill Silencer: casting this
costs you 3 life and draws them a card - you would be at -2; this KILLS you. This price is PER
CAST …]`. The seat correctly declined — and had nothing else. The search had spent the game's last
live window on a card the engine itself calls unsurvivable. (This is the wave's lategame specimen.)

**Ask.** On every search/reveal row whose card would enter the hand as a castable spell, echo the
finished verdict the cast row would print — at minimum the `[NAMED BY THEIR …]` family with its
`you would be at N` / `this KILLS you` tail. Every input is on the board at search time; no row is
removed and the pilot may still fetch it. **Prediction:** wave-59, 0 searches that fetch a
`this KILLS you`-priced card while an unpriced alternative of the same type is on the same list.

## NEW — RENDER, MED

**R322. A HOLD take must not be re-opened by a life-total change alone.** *(deck126 P1;
deck146 MED-3 files the same shape from the other side; R225 / ledger D8.)* The row promises *"do
not ask me again unless the board changes (any change re-opens this window; you give up no cast)"*.
While a decided Sanguine Bond + Exquisite Blood loop resolves, life totals change every iteration,
the predicate fires every iteration, and the row cannot absorb the run it exists for.
`1788504693-ai_baka_deck126-...-vs-ai_baka_deck130.jsonl` **seq 56-87**: 32 consecutive asks inside
turn 25's upkeep, menu byte-identical throughout, header life running 17/17 → 33/0, declined-note
reaching `[you declined this exact list 32 times already this turn]`; same shape at
`...deck126-...deck123.jsonl` seq 43-62 (20 asks). 52 model round-trips in two games narrating a win
that cannot be interrupted. **Ask:** scope the latch's re-open predicate to what a decision can act
on — the menu's `options_text`, the contents of `ON THE STACK`, and permanents entering/leaving. A
change confined to the header's life totals, with `options_text` byte-identical, does **not** re-open
a window the pilot itself closed. This removes no window the pilot did not close and adds no cap; it
makes the pilot's own hold mean what its printed sentence promises — the opposite of the standing
REJECTED "stop asking during a loop". **Prediction:** in a wave-59 game where the pair completes and
the hold row is taken once during the drain, `hold_windows_skipped` rises by the loop's iteration
count and the max declined-note in that game drops below 5. Report it in D8's pinned predicate
(#327) alongside R305's second number.

**R323. A damage-to-attackers row should print its KILL COUNT, the way the sweeper rows do.**
*(deck125 P3.)* `125v146` **s38**: `Cast Lightmine Field {2}{w}{w} {right now: they control 2
creatures able to attack - deals 2 to each if all 2 attack}` two rows from `Cast Final Judgment
{4}{w}{w} {right now: exiles 2 of their creatures …}`. The pilot can only reach the kill count by
joining the row's N to the toughnesses on the battlefield line; the evaluator already enumerates the
able-to-attack set. **Ask:** append `- kills N of those M (toughness <= N)`. 43 such rows in
deck125's six games, 1 take, four of the declines are one lost game. Skill #331 is the guide half
and does not substitute for this.

**R324. The `[<- largest affordable X]` marker must read the NET tail printed on its own row.**
*(deck125 P4; #248 / R298's shape at a second marker.)* `125v162` **s41** at 3 life:
`1. X = 2 {… NET -2 life for this cast} … [<- largest affordable X - X=2 gains 2 life and draws 2
cards; no listed X does more]`. The marker is computed from mana alone, so `no listed X does more`
is asserted over a row the render has just priced as lethal. The pilot took option 1; the game ended
at 0 with no further ask. 11 X menus corpus-wide, 1 carried a NET tail, 1 of 1 taken. **Ask:**
suppress the marker where the row's NET takes printed life to 0 or below, or move it to the largest
X that survives. Same rule as R303's: a printed bound or recommendation must be conservative in the
non-lethal direction or it must not be printed.

**R325. A REPEATABLE activation is priced one payment at a time, and the per-payment verdict reads
as a verdict on the window.** *(deck130 P2 = its own review's E5.)*
`1788504674-ai_baka_deck130-...-vs-ai_baka_deck162.jsonl` **seq110**, pilot at 2 life, 8 untapped
sources, three Goblin tokens: `1. Deal 2 damage with Siege-Gang Commander targeting Fate Unraveler
[opponent's battlefield] {right now: takes 2 damage - SURVIVES (toughness 4)} [cost: {1}{r},
Sacrifice]`. Two payments (4 of 8 sources, 2 of 3 Goblins) kill the 3/4. `SURVIVES` is true of one
payment and false of the window, and it is the row's only verdict. Identical at seq113. **Ask:**
where the cost is payable more than once from the CURRENT board and pool, print the affordable
repeat count and the cumulative verdict beside the per-payment one — `{right now: takes 2 damage -
SURVIVES (toughness 4); you can pay this 3 times right now - 2 payments = 4 damage - DIES}`. The
count is already computed: it gates the row's legality.

**R326. Damage already ON THE STACK is never summed against the pilot's own life, on the one window
where responding is still possible.** *(deck130 P3.)* Same file, **seq114**, `Your life: 1`: the
`ON THE STACK, waiting to resolve (top resolves FIRST - you can respond now)` block lists four
pending items, three of which take a point off a pilot at 1 (Underworld Dreams ×2, Fate Unraveler).
The frame forecasts a FUTURE draw step on the next line (`DRAW FORECAST: … = 3 x 2 = 6 life to the
punishers above`) and prints nothing about the damage already committed in front of it. **Ask:** one
line under the stack block in the DRAW FORECAST's own style — `ON THE STACK AGAINST YOU: 3 damage
pending (Underworld Dreams 1, Fate Unraveler 1, Underworld Dreams 1). Your life: 1 - THIS KILLS YOU
IF IT ALL RESOLVES.` Pessimistic in the lethal direction only: count what is certain, under-claim
what is conditional (D6/D7).

**R327. A "move a card to hand" activation says what it costs and not what the returned card then
does.** *(deck130 P1.)* `1788504683-ai_baka_deck130-...-vs-ai_baka_deck125.jsonl`, **seq 95, 99,
103, 105, 107, 109, 112, 116, 118, 121, 125, 128, 130, 132** — fourteen consecutive upkeeps, turns
30-56, hand `(0 cards): (none)`, 11-14 untapped sources, opponent at 4 life:
`1. Put a card into hand with Hammer of Bogardan [cost: {2}{r}{r}{r}] {card text: "… deals 3 damage
to any target. -- {2}{R}{R}{R}: Return Hammer of Bogardan from your graveyard to your hand …"}`.
`CHOICE: 0 (pass)` all fourteen times; the seat lost 0-11 on turn 57. Every CAST row on that seat
carries `{… and 3 to the opponent at life N leaves them at M}` and a mana-left clause; this row —
the only reach the deck had left — carries neither. **Ask:** for a `moveto(hand)` activation, append
the returned card's own damage/kill clause with the current opponent life, plus the sources left
after paying, in the existing brace shape: `{returns a card that deals 3 to any target; opponent at
4 - two returns reach it} {leaves 6 of your 11 untapped mana sources untapped}`. Facts only, no
recommendation.

## CORE-PROMPT

**R328. `empty_reply` is a ~2.5 s deadline being hit, not a model refusal — classify it as a timeout,
retry once, and log the transport seam.** *(deck125 P1; engine-seat HIGH-2 and HIGH-3, independently
derived.)* All **89 of 89** `empty_reply` records corpus-wide carry `latency_ms` in **2502-2558 ms**
(56 ms of spread across 89 events, median 2510) against a non-fallback median of 12,692 ms, and they
arrive in runs — 14 singletons but also runs of 3, 4, 5, 6, 8×3, 9 and **10 consecutive**
(`1788504678-ai_baka_deck152-...-vs-ai_baka_deck162.jsonl` seq 32-50). Repro of a second run head:
`125v126` s27, s30, s32, s34, s36, five in a row on turn 12, each `reply: ""`, `coded_answers: 0`.
stderr carries **no HTTP status, no retry, no body-length line** for any of them. This is **3.9% of
all decisions**, it is the corpus's single largest silent loss, and it is the mechanism that precedes
F2: the hung seat's last three records are three consecutive `empty_reply` fallbacks (seq 26/28/30,
2503/2502/2542 ms) landing on a may-trigger's chooser. **Ask:** find the ~2.5 s deadline in the
client path, raise or remove it, retry once before falling back, stamp the class `deadline_2500`
rather than `empty_reply`, and log the HTTP status / transport errno and elapsed time at the
empty-body seam. The current name has already been read as "the model refused" in guide-side
reviews, which is the wrong diagnosis of a lost decision. **Cross-filed to the engine ledger; it is
an ENGINE fix, recorded here because the misnaming is a CORE-PROMPT-level reading error.**

**R329. The `PUT:` parser must accept the trailing name gloss that `CHOICE:` requires.**
*(deck125 P2.)* Every `CHOICE:` ask instructs `e.g. "CHOICE: 1 (Cast Supreme Verdict)"`; the
cleanup/bottom asks instruct `e.g. "PUT: 2, 5"` with no gloss and refuse one. `125v130` **s83**
replied `PUT: 9, 1 (Supreme Verdict, Fall of the Gavel)` — the required numbers, in order, with the
protocol's own naming convention appended — and was refused `unparsed_reply` (`coded_answers: 1`,
`post_answer_overrun: 171`). 71 `PUT:` asks corpus-wide, 2 unparsed, 1 of them this shape. **Ask:**
strip one trailing parenthetical from a `PUT:` line before parsing. **Ships with its negatives**
(#R313's pattern): a `PUT:` line with no numbers still refuses, and a parenthetical containing digits
does not become a number.

**R330. The LIFE-TO-DAMAGE CONVERTER paragraph should close by naming the direction that is SAFE —
sourced from the converter actually on the board.** *(deck125 P5, ADOPTED WITH A SCOPE.)* The
paragraph renders on 57 windows; its body is right and its close — *"Any life they gain, or any life
you lose, chains until YOU are at 0 - so ANY nonzero payment on a tag above is fatal"* — reads as a
statement about life events in general. `125v126` **s44** and **s48**: the pilot wrote "Sanguine Bond
converts any gain to damage" into its plan and declined a `max affordable X=5` Revelation with eight
untapped lands. **Ask:** close the paragraph with the direction that is safe *for the converter named
in it* — for the Sanguine Bond on THEIR battlefield, `(life YOU gain is not converted by this one -
only life you PAY chains)`. **The scope is the adoption condition:** the clause is generated from the
converter's own primitive direction, never printed as a general law about lifegain, because a
converter that triggers on an opponent's life GAIN would make the general form false. deck125's
wave-58 guide edit E3 teaches the same fact from the primitive and is consistent with this; when
R330 ships, E3 takes #310's `perception status: CLOSED` clause.

## MEASUREMENT — added to the wave-57 list

10. **R320**: `named_row_reask` + `stale_echo_in_range` — **0 of N** on seats whose guides did not
    change (wave 58: 10 + 11).
11. **R328**: fallbacks in the 2500-2560 ms band — **0 of N** under the new class name, and every
    one of them carrying a logged HTTP status or errno (wave 58: 89 of 89 unlogged).
12. **R322**: consecutive byte-identical `options_text` asks inside one turn AFTER a HOLD take at
    that seat and turn — **max run < 5** (wave 58: 32).
13. **R319**: own-turn windows at `my_life <= the untapped-plus-tapped power on their line` carrying
    no forecast — **0 of N** (wave 58: 2 of 2 known repros, no census run).

---

# WAVE-62 ADOPTED — R331

**This section is the only change from `wave59/general-strategy.md`** (waves 60-61 wrote no
edition; the prefix above is byte-verbatim and was verified with `cmp`). Wave-62 corpus
`matchups-20260905-133058`, binary `12be0b79c`, 21 games (20 natural, 1 hung) + the single-game
rerun `matchups-20260905-161801` on `9230a94c3`; 2,251 decision windows, 2 fallbacks (0.09%).
Census and prediction adjudication: `wave62/engine-seat.md`. Adoption/rejection reasons:
`wave62/synthesis-notes.md`. One proposal adopted of two filed.

## NEW — RENDER, HIGH

**R331. The BLOCKERS emitters still print the bounded block-gain ceiling that D2 withdrew from the
attack side, so under an opposing life LOOP the header names the losing assignment as the best
case.** *(deck123 P1.)* Lane X's D2 gave `attackTotalLine` an `oppLifeLoop && blockGain > 0`
fail-closed branch: with both halves of a life loop on the opponent's side, the bounded `blockGain`
figure is withdrawn along with the kill claim it fed. Two emitters computing the same bounded
quantity were left alone — the blockers header's `best case with every blocker assigned: you would
be at N - one legal assignment that reaches it: …`, and the per-B-row clause `{their attacker's
lifelink, this block: they gain N from this block only, and this attacker deals nothing to your
life}`.

**Repro** — `1788633085-ai_baka_deck123-0x557b7b88b130-vs-ai_baka_deck126.jsonl` **s76**, turn 15,
`kind: blockers`, seat at 16 life with 83 available blockers against one 1/1 lifelink Vampire,
Sanguine Bond **and** Exquisite Blood on the opponent's battlefield line. The screen printed
`INCOMING THIS COMBAT: 1 attacker, 1 unblocked damage - you would be at 15 … - best case with every
blocker assigned: you would be at 16 - one legal assignment that reaches it: Thraben Doomsayer #1
blocks Vampire #1`, then every B row as `(you kill it, your blocker lives) {their attacker's
lifelink, this block: they gain 1 from this block only, …}` — while the same prompt's own converter
banner said the opposite: *"Any life they gain, or any life you lose, chains until YOU are at 0 - so
ANY nonzero payment on a tag above is fatal, not merely expensive."* The seat replied
`BLOCKS: B1:A1`, the assignment the header named. **s77** events: `Opponent gained 1 life (now 25)`.
**s78** events: eleven `You lost 1 life` / `Opponent gained 1 life` pairs, **16 → 0**, them 24 → 40;
**s79** `gameend`. `BLOCKS: none` ends that combat at 15. The primitives make the chain engine-real
(Sanguine Bond `mtg.txt:99005 @lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent`;
Exquisite Blood `mtg.txt:37967 @lifelostfoeof(player):life:thatmuch controller`) and the s78 event
list is it running. Two things are false on one screen: the word **"only"**, and a `best case` of 16
when the true best case is 15.

**Ask.** Feed both blockers emitters the same `oppLifeLoop` input `attackTotalLine` already takes,
from the same `playerHasLifeLoop()` scan the banner is built from, so header, row and banner cannot
disagree on one screen:
1. On a row whose block grants them any life while `oppLifeLoop` holds, replace `they gain N from
   this block only` with the loop's verdict — the gain is not bounded at N and the block is fatal —
   and qualify the row's favourable `(you kill it, your blocker lives)` result, which is true about
   deaths and silent about life.
2. Exclude every such block from the `best case with every blocker assigned` search, so the line
   cannot name a fatal assignment as the best case. If that leaves no assignment better than
   declining, the line says so rather than printing the seat's current life.

**Scope, honestly.** 1 of 21 blockers windows this corpus, and 1 of 1 in which a life loop stood on
the opposing side. It is adopted at HIGH not for volume but because it is a *false* surface that
named the losing line in a game the seat had won on board, and because it is the exact mirror of a
fix this wave already shipped on the attack side. deck123's wave-62 guide edits 1-3 teach the same
fact from the guide side; when R331 ships they become a restatement of a true surface rather than a
patch over a false one, and edit 1's `best case … line is unpriced and wrong` clause takes a
`perception status: CLOSED`.

## MEASUREMENT — added to the wave-57/58 list

14. **R331**: `from this block only` occurrences on a prompt that also carries a printed life
    `LOOP` on the opponent's side — **0 of N** (wave 62: 83 rows in 1 of 1 such window); and on
    every blockers header carrying both a `best case with every blocker assigned: you would be at N`
    figure and an opposing life LOOP, N at or below the same line's unblocked-damage figure —
    **N of N** (wave 62: 0 of 1).
