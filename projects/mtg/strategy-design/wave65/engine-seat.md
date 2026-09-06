# Wave-65 engine-narration seat

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-033121/` (binary master
`0f5e0c7be`). Everything below is counted from the rendered `prompt` (python over the JSONL),
never from the `options` array; HOLD/pass censuses are prefix matches. Card facts checked against
`bin/Res/sets/primitives/*.txt`. Nothing was built, run or edited.

---

## 1. Corpus census

| | wave 65 | wave 64 (`matchups-20260906-001533`) |
|---|---|---|
| games | **21 natural, 0 hung, 0 crashed** (42 seat logs, 42 `gameend`) | 21 |
| records | 2339 | 2421 |
| model-facing decisions (records carrying `prompt`+`reply`) | **2209** | 2294 |
| kinds | ask 1600, priority 422, attackers 87, discard 50, blockers 30, reveal 16, bottom 4, recovery 4 | ask 1744, priority 379, attackers 84, discard 57, blockers 14, reveal 12, bottom 3, recovery 1, wall_miss 1 |
| avg game length | 20.6 turns | 22.0 turns |
| heuristic fallbacks | **0** | 0 |
| re-asks (`fallback`) | **8** — `named_row_reask` 3, `index_name_conflict` 3, `no_pass_reask` 1, `stale_echo` 1 | — |
| re-asks recovered | **8 of 8** (4 explicit `recovery` records + 4 `*_recovered` parse notes; 0 exhausted) | — |
| `async_drops` | **16** (`casting/question (or turn/phase)/re-asked` 8, `casting/question and board/re-asked` 6, `casting/board/re-asked` 2) — all re-asked | — |
| transport failures / `transport_error` retries | **0** (no `transport` field, no `transport_error` token anywhere in the corpus) | — |
| timeouts / wall misses | **0** `wall_miss_events`, **0** `wall_miss_unrecorded` across all 42 `gameend` records | 1 `wall_miss` record |
| `deadline_pct` | n=2209, max **94.2**, **0 > 100**, **0 >= 95** | — |
| slowest decisions | 848 037 ms (`130v123` s31, 94.2%), 822 290 ms (`130v126` s24, 91.3%), 810 191 ms (`123v146` s2, 90.0%) | — |
| HOLD rows rendered / taken | **1448 / 129 (8.9%)**; `hold_windows_skipped` 1104, `mana_only_windows_skipped` 630, `identical_ask_answers_reserved` 187, `identical_option_asks_resolved` 16 | — |
| reveal stalls | 16 reveal windows, engine wait **835 s total**, max **139 s / 16 086 ticks**; `reveal_wait_driver_secs` 0 on all 16 | — |
| SEGV / abort / hang | **0** — 21 stderr logs, 0 hits for `Segmentation|SIGSEGV|abort|terminate called|assert|watchdog|timeout`; every log ends with a `WAGIC_SELFPLAY_RESULT` line | — |

**Wave-58/59 signatures: none recur.** `empty_reply` 0, `transport_error` 0, `bad_reply` 0,
`reveal_stall_forced` 0, `deadline_pct > 100` 0, no translog gap past the watchdog (no watchdog
line in any stderr), and every game closed with its own `game end` banner.

Reply-protocol hash is a single value corpus-wide (`f3f219d680b40848`) — one protocol, 42 seats.
`post_answer_overrun` is set on **1598 of 2209 (72.3%)** decisions; `post_plan_overrun` on 43;
23 replies were trimmed.

---

## 2. Prediction adjudication

Counts are N of M over the wave-65 corpus, with file+seq. **UNTESTED** = no window of the
predicted class arose; nothing is inferred from an adjacent class.

### Lane AL

| # | verdict | evidence |
|---|---|---|
| **AL-1 (G1 modal price)** | **PASS** | `{this mode right now:` renders **12 times over 3 windows** (wave 64: 0 over 23 rows). Silverquill Command is the pool's only `auto=choice name(...)` list carrying `life:`/`draw:` outside an `ability$` block (borderline.txt:102739-102748; the other ten `Choose an option for` cards — the four Pathways, Lay Waste, Starstorm, Spark Spray, Pelakka Predation, Intrepid Adversary, Agadeem — carry none). All 3 windows (`146v152` s24, `146v126` s62, `146v162` s24) render 6 of 9 modes (the 3 graveyard-return modes were illegal), **4 of 4 priced rows tagged, 0 bare**, rows 1/4 (grant-only) correctly untagged. Chair is correct in both directions: `you LOSE 1 life - you would be at 18` on the `controller` modes, `they LOSE 1 life - they would be at 23` on the `opponent` modes — **0 seat-facing kill tags on an opponent-directed cost**. One price per row holds: Agadeem (`146v152` s31, `146v125` s22) renders `{this payment puts you at 16}` from `lifePaymentVerdict` and no mode tag. |
| **AL-2 (G4 floor)** | **PASS** | 31 windows carry `{crack-back cover:`; **28 are FLOOR windows and all 28 carry `THIS IS NOT A SURVIVAL VERDICT`, 0 carry `which you SURVIVE`**. The only 2 `which you SURVIVE` clauses in the corpus (`123v162` s155, `130v162` s114) are on CEILING windows with no `is a FLOOR` line — wave-64 behaviour, unchanged, and the 3 ceiling windows carry no withdrawal. Second falsifier (a lethal-floor window with a body-adding cast row and no cover clause, the `162 vs146` s12 shape) not found: **0 of 112 floor windows**. |
| **AL-3 (G5 draw chair)** | **PASS** | 389 `DRAW PUNISHERS on the battlefield:` paragraphs; **389 of 389 carry `Your DRAW STEP is COMPULSORY`**. **318 name `theirs`** and all 318 carry `the cost above is charged whether you act or not`; **71 name only `yours`** and **0 of 71** carry that sentence or `Count that cost before choosing an OPTIONAL draw`, while **71 of 71** carry `Your own draws cost you nothing on this board.` (wave 64: 32 mis-billed). Repro window `162v126` s28 reads the corrected text verbatim. |
| **AL-4 (G9 pass price)** | **PASS (N=2)** | 16 windows have a `theirs` DISCARD PUNISHERS paragraph + own turn + a `Cast nothing right now` row; **2 of them have a hand above the maximum, and both carry `{CLEANUP PRICE OF PASSING:`** (`126v162` s38: 9 cards, "up to 2 discards … up to 8 life - at worst you would be at 6"; s39: 8 cards, 1 discard, 4 life). **0 spurious tags**: none on the opponent's turn, none at or under the maximum (`126v162` s40, hand exactly 7, correctly bare). Secondary (behaviour): the seat did **not** pass either window — it cast at s38 and s39, taking the hand 9 -> 8 -> 7 and never reaching the cleanup (wave 64: 18 of 18 passed). N is small; deck125, whose HIGH-1 this was, produced no qualifying window this wave. |

### Lane AM

| # | verdict | evidence |
|---|---|---|
| **AM-G2 (`A#>W#`)** | **PASS (4 of 4)** | 4 walker declarations, all `parse_note: attack_walker_target`. `152v146` s27 Briarbridge Tracker (3/3) -> Lolth: *"Your Briarbridge Tracker dealt 3 damage to Lolth, Spider Queen / lost 3 loyalty counters (now 0) / was put into the opponent's graveyard"*, defender's life untouched. `162v152` s18 Master of the Feast (5/5) -> Teferi: 5 damage, Teferi dies, life untouched. `126v146` s25 Vampire -> Kaya: Kaya loses 3 loyalty (now 0) and dies. `162v126` s22 Fate Unraveler -> Sorin: **blocked** — *"Your Fate Unraveler dealt 3 damage to Overgrown Battlement"* — a legal block, and the log says so, so the wave-64 shape (declaration toggled out, damage silently at the player, no explanation) does not recur: **0 of 4 fell through to the player**. Second half: `a planeswalker is not a creature` renders on 15 windows. |
| **AM-G3 (granted actor)** | **PASS (82 of 82)** | 82 `used: … with Tribute to Hunger` lines rendered across the corpus; the `used:` chair matches the `cast` chair in **every one**, and the falsifier (`Opponent used: … with <a card this seat cast>` followed by `You gained`) appears **0 times**. `126v146` s30 renders the wave-64 repro line inverted-correct: *"You cast Tribute to Hunger / You used: Gain life equal to its toughness with Tribute to Hunger targeting Goblin / You gained 2 life (now 41)"*. |
| **AM-G7 (decline re-opener)** | **PASS** | The clause is fully swapped: `not offered again until the board changes` 222 occurrences / 99 windows, `not offered again this turn` **0**. 25 candidate strict-subset row losses were found across all seats and **every one is explained by a board change, not a retirement** — e.g. `125v123` s50 -> s51: the seat tapped Staff of Nin #1, and both its rows vanish (verified against the rendered row lists); `152v126` Lair of the Hydra rows shrink with available mana. **0 unexplained retirements.** The narrow falsifier is answered in the fix's favour: in `123v162` the seat assembled Intruder Alarm + Lord of Lineage and the token row kept being re-offered across turns 10-12 — **the combo fired (277 tokens)**, which is exactly the turn wave 64's cap killed. |

### Lane AN

| # | verdict | evidence |
|---|---|---|
| **AN-1 (G6a LETHAL COMES FIRST)** | **falsifier PASS, positive UNTESTED** | 14 windows print `KILLS them whatever they block`; **0 of them carry `fatal to you, not a trade`** (wave 64: `123v126` s48 carried both). But `LETHAL COMES FIRST` renders **0 times**, because **0 of 87 attacker windows carry any life-LOOP clause at all** — the co-occurrence window never arose. The one surviving caution (`162v126` s28) is a BLOCKERS window, which is the face the lane says is correct there. |
| **AN-2 (G6b reachability gate)** | **PASS (vacuous)** | 0 attacker-row loop clauses corpus-wide, so **0 on unblockable rows and 0 where `lifeLoopProvenWin` is false**. Both falsifier shapes absent. The gate is now tight enough that the loop is named on exactly **1 of 2339** decisions; see MED-2. |
| **AN-3 (G6c converter)** | **PASS (10 of 10 / 4 of 4)** | 14 `{their converter:` rows. **10 rows over 5 windows (`130v126` s28/30/31/32/34) are closed-loop and every one prints `ANY nonzero payment here is fatal` with NO `life N -> M` figure**; board verified — `130v126` s28's opponent battlefield holds both Sanguine Bond and Exquisite Blood. The **4 open-loop rows** (`152v126` s32/s43, `125v126` s125, `162v126` s25) keep the wave-64 arithmetic byte for byte (`takes 5 off YOU: life 3 -> -2; that KILLS you`); `152v126` s32's board has Sanguine Bond and no Exquisite Blood. The `162 vs126` s13 shape (a closed-loop row printing a resulting life) does not recur. |
| **AN-4 (G10a LONE blocks)** | **PASS (6 of 6)** | Every `no better material reaches that life figure` (`146v152` s17, `152v146` s9, `126v152` s15/s23, `126v146` s26/s28) is followed by `among LONE blocks`; **0 unqualified**. Secondary also PASS: **3 replies name two blockers on one attacker** (wave 64: 0 of 10), and `126v152` s15 is the predicted case exactly — the window carries both the LONE clause and a `GANG BLOCK:` verdict and the reply reads `BLOCKS: B1:A3, B2:A2, B3:A2`. |
| **AN-5 (G10b committed blocker)** | **UNTESTED** | 17 windows show a `[blocking …]` body and 25 windows carry a named assignment, but **the two sets do not intersect** — no after-blockers-declared window with a named assignment arose, so the `126v152` s9 shape had no window. (Consistent with the prediction: 0 named assignments contain a committed blocker — but that is 0 of 0.) |
| **AN-6 (G10c blocker lifelink)** | **PASS (2 of 2, 5 of 5)** | 7 windows print a `best case with every blocker assigned` figure. The **2 whose named assignment includes a lifelink blocker** (`126v152` s23, s31) both carry `and your blockers' lifelink`; the **5 without** correctly omit it (a sweep for a lifelink-tagged named blocker in a pairless window returns 0). `no block saves you` renders 3 times and **0 of them on an after-figure above 0**: s23 (after 9) carries no such badge, s31 (after -6) does. |
| **AN-7 (MED forced sacrifice)** | **UNTESTED** | All **3** `FORCED SACRIFICE` asks (`146v126` s33/s56, `162v126` s15) are the branch where the OPPONENT is paid, and all 3 correctly read `the one that pays the least`. The inverted branch (the toughness is life YOU gain — the `126 vs123` s10 shape) never arose, so `GAINS YOU THE MOST` renders 0 times and the falsifier could not fire. |

### Lane AO

| # | verdict | evidence |
|---|---|---|
| **AO-1 (first-wins)** | **PASS (2 of 2)** | `answer_replaced: true` on exactly **2 of 2209** records, and **both carry an explicit marker**: `126v125` s27 (`CHOICE: 2` … `Correction: …` … `CHOICE: 4`, executed 4) and `162v126` s27 (`ATTACK: A1>W1` … `Correction: Rule 0c says …` … executed `ATTACK: none`, `parse_note: attack_correction_line_taken`). **0 unmarked replacements** (wave 64: 3 of 3 silent flips in deck146 alone). |
| **AO-2 (`later_answer_ignored`)** | **PASS (33 of 33)** | 33 records carry the note, and **all 33 executed the reply's FIRST coded line** (the 3 whose `choice` is a count rather than an index — `146v126` s31, `126v162` s23, `162v123` s18 — match the first line's content). |
| **AO-3 (number/name conflict)** | **FAIL by the letter, falsifier PASS** | **3 of 3** reserved-name records (`123v162` s29/s68/s111, all `hold_row_named;echo_index_conflict`) carry `index_name_conflict` and route to the re-ask, all 3 recovered — the stated falsifier does not fire. But **2 records execute a row whose number and name disagree with no conflict note**: `130v125` s97 (`CHOICE: 2 (Mountain #2 …)` -> executed index 1, `chosen_text` "Mountain #2") and s98 (`CHOICE: 3 (Mountain #3 …)` -> index 1), both `parse_note: echo_index_conflict;name_over_index`. The rule was wired into `parseChoice`'s reserved-name branches only, so `name_over_index` on an ordinary target list still resolves silently. See MED-1. |
| **AO-4 (plan from the answer's block)** | **PASS (6 of 6)** | 6 records carry both `later_answer_ignored` and two PLAN lines. In all 6 the plan served to the next window is the FIRST block's, matching the executed first coded line — e.g. `130v162` s61 serves *"Pass main phase 1. Ob Nixilis is at 1 loyalty …"* (block 1) after executing block 1's `CHOICE: 3`. The wave-64 `130v146` s47 defect (answer from block 2, plan from block 1) has no instance. |
| **AO-5 (plan carry cap)** | **PASS** | Truncation-notice rate fell corpus-wide **224/2294 (9.8%) -> 187/2209 (8.5%)**, and on deck126 — the cited baseline — **62/253 (24.5%) -> 29/194 (14.9%)**. |
| **AO-6 (`{feeds:}` symmetric)** | **PASS (29 of 29 / 34 of 34)** | 63 `{feeds:` tags. The **29** on symmetric engines — Howling Mine (15) and Dictate of Kruphix (14), both verified symmetric in the primitive (`mtg.txt` "each player's draw step, that player draws an additional card"; `borderline` Dictate's two `@each` lines) — **all** carry `and so do YOU`. The **34** on one-sided feeders (`each opponent draws a card` 18, the library-order draw 12, the per-spell seven 4) carry it **0** times. |

### Lane AP

| # | verdict | evidence |
|---|---|---|
| **AP-R1 (phase in the decline scope)** | **PASS** | Same evidence as AM-G7: 0 unexplained row retirements over 25 candidate subset losses, several of which cross a phase boundary within one turn (`123v130` s38->s40 Main 1 -> Main 2, `130v123` s39->s47 Upkeep -> Main 1) and are explained by tap/mana/X changes. **Counter-measurement (the cost R1 buys), which the lane left unmeasured:** decline-clause rows rendered 215 (wave 64: 224); **re-offers of an already-offered decline row 119 (wave 64: 70, +70%)**, of which 90 follow a decline (wave 64: 61) and 29 follow a take (wave 64: 9). Priority windows 422 vs 379 (+11.3%), but **total decisions fell 2421 -> 2339** and games are 6% shorter. Per seat-game that is **2.8 re-offers**, far under R1's own "one per phase" trigger. Scope is not the dial that needs moving. |
| **AP-R2 (ATTACK/BLOCKS first-wins)** | **PASS** | The 3 ATTACK/BLOCKS records carrying `later_answer_ignored` (`146v126` s31 `ATTACK: A2, A3, A5`; `126v162` s23 `ATTACK: A1, A2, A3`; `162v123` s18 `BLOCKS: B1:A1`) all executed the FIRST coded line, and the one attackers record that took a later line (`162v126` s27) had an explicit `Correction:` above it. **0 unmarked later-line declarations.** |
| **AP-R3 (adjacent marked correction)** | **UNTESTED** | A scan of all 2209 replies for two coded lines on consecutive lines with a retraction cue on the second returns **0**. Both real corrections in the corpus are prose-separated. |
| **AP-R4 (lifelink in the ATTACK TOTAL ceiling)** | **PASS (1 of 1, 13 of 13)** | Of the 14 `KILLS them whatever they block` windows, exactly **one** (`152v126` s62) has a lifelink blocker on the opponent's board (a 1/1 lifelink Vampire from Sorin). Its total renders the new term: *"At least 18 damage lands whatever they block - that damage alone puts them at -9, but every blocker they declare with LIFELINK gains them the damage it deals (twice for a double striker) - up to 1 life …"*, and 1 < the 9-point shortfall, so the kill claim is sound. The other 13 windows have no lifelink blocker and are byte-identical to wave 64. **0 kill claims over a lifelink body that covers the shortfall.** |
| **AP-R5 (double-strike lifelink)** | **UNTESTED** | **0 windows** in the corpus show a friendly body carrying both `lifelink` and `double strike`. |
| **AP-R6 (curl 28 band)** | **UNTESTED** | **0** `transport_error` records, **0** `wall_miss` events, **0** `deadline_pct >= 95`. No connect timeout occurred, so neither the reclassification nor its falsifier could be exercised. Nearest approach: `130v123` s31 at 94.2% (848 s), i.e. inside the deadline and below the band. |
| **AP-R7 (UTF-8 plan bound)** | **PASS (half), UNTESTED (half)** | **0** served `YOUR PLAN` lines contain U+FFFD or an incomplete sequence. But **0 served plans contain any non-ASCII byte at all**, so the accented-text half (carried to 400 characters rather than ~200) never arose. |

**Tally: 15 PASS, 1 FAIL, 6 UNTESTED** (AN-1 and AP-R7 counted as PASS on the half the corpus
reached and flagged above; AO-3's FAIL is by the prediction's letter, not by its own falsifier).

---

## 3. Engine / render / narration defects

### HIGH-1 — the DRAW FORECAST re-charges the whole draw step on every window inside it, and turns a false death verdict on the seat

`123v162`, seq **138 through 153** (16 consecutive windows, all inside turn 12's Draw step) render
the **identical** line while the seat's life falls 7 -> 3:

```
DRAW FORECAST: your draw step, resolving NOW, draws 6 cards (1 + Howling Mine #1 1 + Howling Mine #2 1
+ Howling Mine #3 1 + Dictate of Kruphix #1 1 + Dictate of Kruphix #2 1) = 6 x 1 = 6 life LOST BY YOU
to their punishers above - if it resolves as forecast you would be at 1. This draw step is COMPULSORY -
no row on any menu declines it.
```

At s138 (life 7) that tail reads `you would be at 1`. At s152 (life 4), after four of the six draws
have already resolved and been paid, the same line reads **`you would be at -2; that KILLS you`** —
a death claim over a cost of which **1 point remained**. The seat survived at 3 and won the game.
The forecast is recomputed from the full step size on every priority/cast window served inside the
step, with no term for the portion already resolved, and `resolving NOW` is asserted 16 times for
one resolution.

Corpus-wide: **12 runs of >= 3 identical `resolving NOW` forecasts** (2 seats, `123v162` and
`130v162`; longest run 10 windows). It is a false claim of the class this seat exists to catch —
a narration that lies about a number the seat is told to price its decisions against — and a
per-deck reader would read the repetition as the punisher board, not as a stale emitter. Repro:
`1788683516-ai_baka_deck123-…-vs-ai_baka_deck162.jsonl`, seq 152, the line above.

### HIGH-2 — first-wins executes the answer the model wrote *before* it reasoned; 72% of replies overrun the answer line and only 2 of 2209 use the correction marker

`kReplyProtocol` requires the coded answer on the **FIRST** line, and #W65-AO/#W65-AP make the first
coded line binding everywhere. Those two rules compose into a hazard the lanes did not price:
`post_answer_overrun` is set on **1598 of 2209 (72.3%)** decisions, i.e. on nearly three quarters of
windows the model keeps writing — and reasoning — after its binding answer.

Specimen `130v162` **seq 61** (turn 10, 15 life, 1 untapped source). The reply, verbatim in order:

```
CHOICE: 3 (Cast nothing right now)
PLAN: … Ob Nixilis is at 1 loyalty; Spark Spray (1 damage) … can both kill it. … Spark Spray costs
{R}. I have 1 mana. I can cast Spark Spray. It will kill Ob Nixilis. This removes the draw punisher.
This is the best play. I should cast Spark Spray.

Re-evaluating:
… If I cast Spark Spray, it deals 1 damage to Ob Nixilis. Ob Nixilis dies (0 loyalty).
This removes the draw punisher. This is a huge advantage. I should cast Spark Spray.

CHOICE: 1 (Cast Spark Spray)
PLAN: Cast Spark Spray targeting Ob Nixilis, the Hate-Twisted to remove the planeswalker and the draw
punisher. Then pass.
```

Row 1 (`Cast Spark Spray {r}`) was on the menu and affordable. The engine executed row 3 and stamped
`later_answer_ignored`. The lane accepted this cost in the abstract; the corpus prices it:
**33 of 2209 records (1.5%) carry `later_answer_ignored`; 27 are pure restatements at no cost, and
6 name a different row** — `123v126` s36, `146v152` s35, `152v162` s43, `126v125` s34, `123v162`
s155 and this one. Of the 6, s61 is unambiguously the worse outcome by the model's own stated
reasoning. The escape hatch exists and works (both `answer_replaced` records used it), but the model
reached for it **2 times in 2209 windows** while overrunning its answer 1598 times, so in practice
the marker is not the mechanism carrying the load — the first line is.

### MED-1 — `name_over_index` still resolves a number/name disagreement silently outside the reserved-row classes

#W65-AO G8/6 stamps `index_name_conflict` in `parseChoice`'s reserved-name branches
(`hold_row_named` / `pass_row_named` / `pass_hold_ambiguous`) only. `130v125` **seq 97**: reply
`CHOICE: 2 (Mountain #2 - "R")`, record `choice: 1`, `chosen_text: Mountain #2 [land] …`,
`parse_note: echo_index_conflict;name_over_index` — the NAME won over the NUMBER with no re-ask.
Same shape at s98. Both landed on the row the model named, so nothing was lost here; the defect is
that the rule the protocol now states ("number/name conflict is a re-ask") is enforced on 3 windows
and not on these 2.

### MED-2 — the life-LOOP fact is now named on 1 of 2339 decisions

`lifeLoopProvenWin` + the per-row reachability gate together removed the clause from **every**
attacker window in the corpus: 87 attacker windows, **0** loop clauses, `LETHAL COMES FIRST` never
rendered. The only surviving instance is `162v126` s28 (a blockers window). The lane predicted the
narrowing ("a seat reading only the rows sees the loop named once fewer"); the corpus shows the
narrowing is total on the attacker side. That is correct-by-construction and it is also the reason
AN-1's positive half is UNTESTED — nothing here is wrong, but the wave-64 defect and its fix are
both now unobservable at the attacker seam, and a future wave should not read that silence as PASS.

### MED-3 — reveal windows stall the engine for up to 139 s each

16 reveal windows, `reveal_wait_secs` summing to **835 s** (max 139 s / 16 086 ticks, `123v146`
class), with `reveal_wait_driver_secs` 0 on all 16 — the wait is engine-side, not model-side. No
`reveal_stall_forced` fired, so this is under the wave-58/59 threshold, but it is ~4% of wall time
across the run for 0.7% of the decisions.

### MED-4 — three decisions inside 10% of the deadline

`130v123` s31 848 037 ms (94.2%), `130v126` s24 822 290 ms (91.3%), `123v146` s2 810 191 ms (90.0%).
None crossed and `wall_miss_events` is 0 corpus-wide, but the headroom on the slowest window is
52 s of 900 s. This is also why AP-R6 is UNTESTED: the classifier's band was never entered.

### LOW-1 — the token loop consumed 92% of a game's decisions and the seat nearly killed itself grinding it

`123v162` (the corpus's `life -1089` game) is answered in §4; as an engine item, the loop cost
**154 of that seat's 167 records** and produced 3 of the corpus's 8 re-asks (all three
`index_name_conflict`, seq 29/68/111, all recovered to `Cast nothing right now`). The
`repeated N times, then stop` row exists and was used 22 times, but it was interleaved with ~40
single-activation windows; `repeat_count_zero_pass` and `repeat_count_under_two` fired 5 times
between them.

---

## 4. The lane-flagged questions

**(a) G7 re-offer / decline churn (AP: cost unmeasured).** Measured above under AP-R1:
decline-clause rows rendered **215** (wave 64: 224); **re-offers 119 vs 70 (+70%)**, of which 90
follow a decline and 29 follow a take; priority windows **422 vs 379 (+11.3%)**; **total decisions
2339 vs 2421 (-3.4%)** over games 6% shorter. Normalised, that is +2.8 re-offers per seat-game.
The re-opener costs about one extra priority window per seat-game and the run got cheaper overall,
because the rows it re-offers end turns sooner. Nothing here argues for tightening the scope.

**(b) `later_answer_ignored` census — how often would the later unmarked line have been better?**
**33 of 2209 records (1.5%)**. **27 of 33 are restatements** of the same row (no cost at all).
**6 name a different row**: `123v126` s36 (`x30` -> `pass`), `146v152` s35 (`+1 Kaya` -> `Hold`),
`152v162` s43 (`Hold` -> `Pass`), `126v125` s34 (`Tribute to Hunger` -> `Overgrown Battlement`),
`123v162` s155 (`Bloodline Keeper` -> `Cast nothing`; the seat won anyway), and `130v162` s61 —
the one where the ignored line is clearly better by the model's own written reasoning (HIGH-2).
So the measured price of first-wins in this corpus is **1 clearly-worse decision in 2209**, plus
5 where the later line is arguable. Against it, first-wins removed **3 of 3** silent row flips and
**1 of 1** cross-block plan mismatch that wave 64 recorded. The rule is paying for itself; HIGH-2
is about the *protocol's* first-line requirement, not about reverting the rule.

**(c) G4 — is a decision visibly starved of the withheld ceiling NUMBER?** **No.** 28 windows carry
the withdrawn verdict; **8 of them are lethal floors**. On **7 of the 8** the seat took a
body-adding cast row anyway — the act the clause prices — so the withheld verdict did not deter the
defensive play. The one exception, `146v152` **seq 41** (7 life, floor 15), is not starvation: the
clause's own arithmetic is fully quantitative and self-defeating — *"Counting only the checked bodies
you cover 6 of 15, leaving 9 -> you would be at -2"* — so a ceiling number could not have changed the
answer; the seat had no cover at any total. The clause gives the seat a working figure in every
instance and withholds only the survive/die verdict, which is the honest read of a lower bound.

**(d) `162v123` ending at life -1089 on turn 11.** The loop is **Intruder Alarm + Bloodline Keeper
transformed to Lord of Lineage**, run by **deck123** (the winner), not by the seat that died.
Trace on the deck123 seat log: Intruder Alarm cast s8 (turn 6), Bloodline Keeper s10 (turn 8),
transformed at s38 (turn 10). Lord of Lineage's `{T}: create a 2/2 Vampire` re-untaps through
Intruder Alarm on every token ETB, so the activation is unbounded. From s44 to s161 the seat ground
it up in ~60 priority windows across turns 10-12: the rendered counts read
`you control 24 / 48 / 101 / 152 / 200 / 253 / 276 / 318 creatures`. At s165 (turn 12) it declared
**`ATTACK: A1, A2-A277`** — *"ATTACK TOTAL: 277 attackers listed, 1109 total combat damage to a
player … they would be at -1085; that KILLS them whatever they block"*. The deck162 seat, at 15
life, had one blocker (`s18: Master of the Feast blocks Lord of Lineage`), absorbed 5, and took
1104 to reach **-1089**. The engine handled it cleanly: no crash, no hang, prompt held to 19.5 KB
by A-row range folding, and the whole game closed naturally in 20 records on the losing seat.

**Did the loop verdicts print before it fired?** Yes, and correctly. Every window of the grind
carried `DRAW PUNISHERS on the battlefield: theirs - Underworld Dreams` and the compulsory-draw
sentence; the deck162 seat's blockers window at s18 carried `INCOMING THIS COMBAT` and a lethal
verdict. Two caveats: (i) the *life*-LOOP family (Sanguine Bond / Exquisite Blood) has nothing to
say about a TOKEN loop and printed nothing — there is no combo/unbounded-activation verdict in the
prompt at all, so neither seat was ever told a loop was assembling; (ii) HIGH-1's stale DRAW
FORECAST is measured on precisely these windows, and at s152 it told the looping seat it was about
to die when it was not. The seat that ran the loop nearly lost to its own grind: its life fell
**14 -> 1** across turns 10-12 while it ground, from the opponent's Underworld Dreams billing every
Howling Mine / Dictate of Kruphix draw. It won by one turn.

---

## 5. What I did NOT check

- **No binary was built or run and no game was driven** — everything is a count over the recorded
  translogs, the stderr logs, the lane reports and the primitives. Every claim about what the code
  does is a claim about what it *rendered*.
- **AL-4 rests on N=2** qualifying windows and AP-R4 on N=1. Both are real verdicts on the windows
  that arose; neither is a rate.
- **AM-G7 / AP-R1's "no unexplained retirement" is a proxy**, not the board key. The record carries
  no `declineBoardScope` value, so I compared the rendered battlefield lines and read the row lists
  by hand on the 25 candidates. A retirement that coincided with an unrelated battlefield change
  would look explained to me.
- **AN-5 and AP-R5 are UNTESTED because the window class is absent from this pool**, not because I
  could not measure them; the same is true of AN-7's inverted branch, AP-R3's adjacency, AP-R6's
  connect timeout, and AP-R7's non-ASCII half.
- **AN-1's positive face was never rendered**, so the composed `LETHAL COMES FIRST` string has been
  verified by no instrument outside lane AN's own PARSETEST.
- **I did not verify the `A#>W#` blocked case (`162v126` s22) against the blocker's legality** —
  Overgrown Battlement is a 0/4 defender and blocking a walker's attacker is legal, but I did not
  confirm from the primitive that nothing on that board should have prevented the block.
- **HIGH-1's mechanism is inferred from the rendered string, not from the emitter.** I did not read
  `drawForecast`'s source to confirm it recomputes from the full step size rather than from a
  remaining count; what is measured is that the string, its total and its verdict do not move
  across 16 windows while the life it is subtracted from moves by 4.
- **I did not audit the guides**, per-deck play, mulligans or any strategy question, and I did not
  re-derive any wave-64 count except the ones I recomputed from the wave-64 corpus directly
  (decision volume, re-offers, truncation-notice rate).
