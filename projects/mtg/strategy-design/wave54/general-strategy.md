# Wave-54 general strategy — RENDER / CORE-PROMPT proposals, layer-routed (R1-R192 revised, R193-R224 new)

Corpus `matchups-20260903-034355` (ELEVENTH fair-hand corpus), binary master **caf023dbc**
(engine lanes A e21f81de5 / B 351ad0098 / C a2bc290d0 / D 6c1555bb1 / E 5fe998414 / F 3329c40f1;
wave-53 lane AA 37b7951d3 in its FIRST corpus; audit lanes G 6dee5d2cd, H e078c2501, I 85c703453,
J fbd46fa04, K afd88acd0, L 27f867152, M ba784277f, N c2f34b05a; owner O1/O2 1afe6c8f6.
Gate on caf023dbc: suite 1226 with the 2 known concurrency-only failures, 51 AI/0, 0 timeouts,
PARSETEST 2489/0). Pilot **qwen36-35b-a3b** on Spark :8084.

42 seat logs, **2,306 records** = 42 `gamestart` + **42 `gameend`** + 10 `recovery` + **2,212
decisions** (ask 1,683 / priority 332 / attackers 99 / blockers 36 / discard 34 / reveal 20 /
bottom 8). **21/21 GAMES NATURAL — invariant 00 SATISFIED and wave-53 D1 DISCHARGED**: 0 timeouts,
0 crashes, 0 hangs, and lane F's no-progress supervisor never fired. **23 `fallback` fields =
1.04%** (`plan_choice_conflict` 12, `named_row_reask` 7, `named_row_not_offered` 2,
`stale_livelock` 1, `all_assignments_illegal` 1 — no `empty_reply`, no `timeout`, and **0 records
with `choice: -1` and no fallback**). **Baka executed 2 decisions = 0.09%**; one
`giving this decision to the heuristic`. Max seat log **312** (`123v125`); max prompt **32,215**
(`146v126` seq 86, priority, turn 31); **2 prompts over 30,000** (wave 53: 12) and **37 over
25,000** (wave 53: 588). Total recorded inference **14.67 h** over 2,211 timed decisions.
Sources: the four seat files, seven `deckN/general-suggestions.md`, seven `deckN/skill.md`, seven
`deckN/edit-texts.md`, the fifteen lane reports, and the corpus — **every load-bearing number below
was re-derived on disk by this step** with python over the JSONL and `/usr/bin/grep` over the
`.stderr` files.

**Routing rule (owner doctrine 2026-08-26).** The core loop — engine mechanics, LLM UX, bugs — is
the purpose. Everything below is a change to what the ENGINE RENDERS, what the PARSER executes, or
the CORE PROMPT's own text. Guide-side answers are named only where a rule must stay correct after
the render ships. Engine BUGS with call sites are in `engine-ledger.md` (the wave-55 docket); this
file is the ranked routing view with the evidence and the wave-55 falsifier for each item.

**Governing constraint.** Legality is enforced structurally; the model's CHOICE is never
constrained. Every proposal ADDS a true token, a true row, a receipt, a stamp or one re-ask, and
DELETES nothing. **Hard caps on legal choices are rejected on sight.** Rejections this wave are
recorded under R193 and R194.

**THE CONTAMINATION NOTICE, and it governs how the lane predictions below are scored.** Wave-53
lane AA's interrupt-window stall floor is denominated in `dt`, and the corpus recipe pins
`WAGIC_FASTCLOCK=0.1`, so its 1,200-*game*-second budget for an interactive-AI seat is 12,000 ticks
of a headless pump. **936 of 936 `interrupt window held … for N ticks` lines read N = 12002 — one
value, no variance** — with 468 releases and 488 `dropping stale async answer`. Opponent-turn
ask/priority windows fell **1,052 -> 441 (−58%)** while total decisions fell 36%: the opponent-turn
SHARE of all decisions fell **30.2% -> 19.9%**. Every rate whose denominator is an opponent-turn
window is therefore measured against an instrument that moved, and is marked **UNTESTED (denominator
moved)** rather than FAILED. Rates measured INSIDE a class the floor does not gate are scored
normally. This is skill amendment **#217**, applied.

---

## STATUS OF R1-R160, by the emitter's actual string

Verdicts carry from `wave53/general-strategy.md` except where a wave-54 lane touched them.

| item | wave-54 verdict | evidence (re-derived on disk) |
|---|---|---|
| **R1** loop affordance | **PARTIAL — `stale_livelock` returns at 1 after two clean corpora** | `146v123` seq 15, a `reveal`, six consecutive stale drops with NO release between them and the corpus's only heuristic handoff — **R197**. `repeat plan iteration` 89, `cast ask NOT issued` 319 |
| **R2** A-row collapse | **STILL FAILS, and the class is now the corpus's biggest render waste** | 46 ask/priority records carry an `#N` family of >= 3 identical-shape rows = **403 redundant rows / 160,803 chars** (method: family key = row text with the ordinal masked; redundant = family size − 1). Led by `Equip with Lightning Greaves targeting Human #N` **190**, `… Vampire #N` 93, `Mountain #N` 82 — **R200** |
| **R13/R71/R104/R132/R192** GAME LOG share | **THE TAIL IS FIXED AND THE BODY SHRANK** | prompts > 30,000: **2** (was 12); > 25,000: **37** (was 588); `ask` mean 14,245 -> **10,269 (−28%)**. Lane L's 24 KB trim v2 is the visible win. Log share of prompts carrying a CURRENT SITUATION marker ~56%. Carried as **R224**, still the owner's |
| **R19/R38/R108** what paying taps | **PASS** | `{paying this taps:` 6 rows at deck152 (1 take, a one-name clause on a body cast), 7 at deck126 (5 takes) with **5/5 receipts naming exactly the sources the clause named** |
| **R32/R64/R73** auto-tap order | **HOLDS** | `with no source tapped` **0** in all 21 stderr files; `- Paid` 369 receipt lines across `events` (15,170 occurrences across all prompt texts — two units, both stated per #208) |
| **R67/R79/R114/R146** second copy | **HOLDS on the mechanism, one regression at one seat** | 97 `[second copy:` rows / 12 takes corpus-wide; deck162 5 takes all at K >= 1, deck126 **0 of 24**, deck130's Talismans 4 (correct, they stack). deck123 took **3 of 20** Intruder Alarm rows carrying the dead tail — the seat's own guide item (A54-2), not a render failure |
| **R103/R133** latency | **the arm FIRED for the first time** | exactly one reply passed 95% of the wall — `123v125` seq 1 at **165.4%**, `latency_ms 1,488,661`, carrying `long_reply: 1`, `deadline_pct: 165.4`, `retry: 1`, and it ANSWERED on the retry. `no reply after 900s - one retry` appears twice; the second produced **no record at all** — **R215** |
| **R136-R159** wave-52 shipped set | **HOLD by string** | no failing window; the items that shipped and passed in wave 53 are re-measured under their wave-54 successors below |

## STATUS OF R161-R192 — the wave-53 items, adjudicated by the emitter's actual string

| item | ledger | wave-54 verdict | evidence |
|---|---|---|---|
| **R161** interactive-reveal hang | D1 | **SHIPPED — PASS. 21/21 natural** | 42 `gameend`, 0 hangs, 0 seat logs without a `gameend`. The 4 Sigarda Coven `reveal:5` windows (`152v126` s20/29/35/45) all found a Human, asked the model and resolved. **A SECOND reveal path is open and it is a LIVELOCK, not a stall — R197** |
| **R162** the HOLD row's three defects | D2 | **SHIPPED — PASS on all three mechanisms; the ECONOMY is untestable this corpus** | (a) old string **0 renders**, new string **426**, LAST row 426/426, opponent's turn only, **43 takes (10.1%, was 2.5%)**; `hold_row_named` 0 and no HOLD take mis-executed. (b) HOLD takes tripping `plan_choice_conflict`: **0 of 12** (was 5) — **CLOSED**. (c) `hold_windows_skipped` non-zero on **7 of the 8 gameends where the row was taken** (was 2 of 6), sum 72, `hold re-opened` 21; the eighth is `123v130` s17, a **Cleanup** take with no window left in the turn to skip. Residuals: **R196** (own-turn exclusion), **R203** (the reworded name collides with row 0), **R213** (the row states its risk and not its benefit) |
| **R163** compensation search framed as a loss | D3 | **SHIPPED — PASS 0/3, thin N** | old header `EXILE ONE OF YOUR OWN CARDS: your own effect (Path to Exile)` **0 renders**; the gain header renders **2** (`162v125` s26, `126v125` s33). The one surviving loss framing (`126v146` s33) is a genuine sacrifice to the graveyard and is correct. N = 2 against 8 last corpus |
| **R164** kill summary hiding a player target | D4 | **SHIPPED — PASS 0/586** | 586 rows enumerate a player in `legal targets right now:`; **none** carries a `{kills…}`/`{removes…}` summary that omits it. deck130's lethal declines went **3 -> 0**. New sibling: **R207** (a single-target `{kills:}` list naming the caster's own permanents with no ownership mark) and **R199** (a `target=player` row pricing only one of its two enumerated players) |
| **R165** modal rows with no live/dead clause | D5 | **SHIPPED — PASS 17/17 rows, 11/11 Silverquill Command** | `{modes live right now: …; dead (no legal object right now): …}` **0 old / 17 new / 7 takes**; the card was **cast twice** (`146v126` s47, `146v152` s41) against **0 of 28** last wave. This is the wave's cleanest render-to-behaviour result. Residual: the `Choose one mode for Silverquill Silencer:` sub-menu is a card-NAME menu and is unannotated **9/9** — **R214** |
| **R166** the annihilator's `your own effect` | D6 | **SHIPPED — PASS 0 renders** | the literal `your own effect` renders **0** times corpus-wide; the opponent-driven branch now reads `FORCED sacrifice OF YOUR OWN CARD: the opponent's effect (<source>) forces YOU to sacrifice…`. **N is thin**: no annihilator or forced-sacrifice mega-menu reached any seat this corpus, so this is a string PASS, not a behavioural one |
| **R167** reveal wait / watchdog | D7 | **SHIPPED — PASS on (a) and (b), UNTESTED on the force-close** | records with `choice: -1` and no `fallback`: **0 of 2,212** (was 1); `engine_answered` never needed to render; 0 games stopped with verdict `hang`. `force-close` renders **0** — **UNTESTED, N = 0**, and **R197** shows a reveal that stalled in a shape the guard does not cover |
| **R168** the `#a-#b` collapse / length-1 asks | D8 | **SPLIT: length-1 asks PASS, the collapse FAILS worse** | de-duplicated length-1 asks **0** (was 16), `identical_option_asks_resolved` 5 on 2 gameends. But menus over 12 rows collapsing to few shapes: **13** (wave 53: 3), and the 2-D one-source × N-target family was explicitly NOT shipped — **R200** |
| **R169** the unstamped 96.5% reply | D9 | **SHIPPED — PASS 0/1** | the single reply above 95% carries `long_reply` + `deadline_pct`. New residual: a 900 s wall miss that produced no record at all — **R215** |
| **R170** the DRAW PRICE cost form | D10 | **SHIPPED — PASS 43/43** | **0 rows of either form without `- you would be at K`** (was 58 of 85). `this KILLS you` **9 renders, 0 casts**. The Forced-Fruition 16/16 sub-claim is **UNTESTED — Forced Fruition rendered 0 times**; the tail worked on the five families that did arise. It won `146v162` — see skill #223 |
| **R171** unconditional removal with no victim | D11 | **SHIPPED — PASS** | `{kills: <names>}` **0 old / 75 new / 13 takes**; `{removes: <names>}` **0 / 38 / 11**; conditional `{kills N of the …}` **0 / 12 / 0**. The INDESTRUCTIBLE sub-claim is **UNTESTED** — no indestructible removal target arose |
| **R172** the PLAN shape bound + stale-plan diff | D12 | **SHIPPED — PASS** | served PLAN blocks longer than one paragraph **0 of 2,036**; every served block carries the age stamp (2,036/2,036); max `plan_echo_count` 37 (was 43). The `"<card>" is no longer on your menu` clause renders in **357** prompts. **Honest limit:** the ground-truth set of stale-plan decisions is not enumerable from the log, so this is a render count, not a completeness proof |
| **R173** the latch executing a third row | D13 | **SHIPPED — PASS 2/2** | `latched_row_mismatch` fired twice and **both are true positives**, adjudicated record-by-record: `123v125` s147 (an off-menu `Cast Bloodline Keeper` last line, executed the first line whose index and name agree) and `146v152` s63 (a header naming row 3, a final coded line `So, CHOICE: 5 (Cast nothing right now).`, executed row 5). **0 stamps on a legitimate name-over-index remap.** Genuine mis-executions corpus-wide: **0** (was 2) over 1,828 parentheticals — the ≤1/3,000 carry is met with room. One seat filed s63 as a telemetry defect; that reading is **refuted on disk** — see skill #230 |
| **R174** PLAN contradicting its own CHOICE | D14 | **SHIPPED — UNTESTED (0 renders)** | `plan_contradicts_noop_row` **0**. All 12 `decision_reversed_in_prose`/`plan_choice_conflict` records take a LIVE row, not a no-op row, so the arm had no window. Old 0 / new 0 / takes 0 |
| **R175** echo binding an annotation-only name | D15 | **SHIPPED — PASS 0** | no reply naming a card that occurs on the menu only inside an annotation executed a row (was 1). Watch item held: `stale_echo_in_range` 1 -> 5 and `named_row_not_offered` 4 -> 6, and **10 of the 11 are one seat** echoing `Cast Bloodline Keeper` / `Cast Vision Skeins` off a stale plan — a plan-staleness signature, not a parser regression |
| **R176** the mulligan coverage cause | D16 | **SHIPPED — PASS 11/11** | every `would not cover any spell` render carries a cause, in two families. **And the fix created a new problem**: the two families call for opposite decisions and one of them shipped a three-land seven — **R204**, skill #210 |
| **R177** the blocker-count header | D17 | **SHIPPED — PASS 99/99** | every `attackers` record carries `They have N untapped creature(s) able to block …`. The BLOCKERS side has no twin and that gap voided an answer — **R205** |
| **R178** the menu-fit clause | D18 | **SHIPPED — PASS 1,203 renders / 218 takes** | 0 priced rows on a multi-priced menu without a fit clause. Four emitter forms, all counted (`no other row on this menu needs more than N` 579, the `need more mana sources than the N this leaves` form 221, the singular 164, `taps you out - …` 197+) — a first pass looking only for the plural form scored 392 phantom misses (#208) |
| **R179** the doubled effect label | D19 | **SHIPPED — PASS 0/401** | `ability: X's X` **0** (was 53); wave 53's `Ob Nixilis, the Hate-Twisted's Ob Nixilis, the Hate-Twisted` is now `… 's triggered ability`. The generic fallback label survives on 9 lines — **R219**, LOW |
| **R180** fetch rows with no colours | D20 | **NOT SHIPPED — carried** | 21 renders of `{this land makes no mana - crack it for a land …}` at deck123 with no colour on any row. Behaviour improved anyway (4 of 5 fetches took a dual, was 5 of 8), so this is correctness, not bleeding — **R209** |
| **R181** the ahead-on-life falsehood | D21 | **SHIPPED — PASS 0 of 18** | 18 renders of `while ahead on LIFE`, **none** at a record where `my_life <= opp_life`. **The corpus's only render FALSEHOOD is closed**, and the guide bullet it was keyed to (deck123 A53-6) retires with it per #187 |
| **R182** the tap clause's count | D22 | carried, not shipped | 13 rows corpus-wide at the two seats that render it; no window turned on the count this corpus |
| **R183** the CONVERTER block's consequence | D23 | carried, not shipped | the block renders 129 times at deck125/deck126; the per-row fold now exists inside blocker annotations and inside the attack tag (`(blocking trigger: they gain 3, and their converter takes 3 off you)`), which is what let deck152 race a Sanguine Bond board and win 20/-10 — see **R221** |
| **R184** the `{spends}` clause naming the wrong card | D24 | carried, not shipped | no window turned on it |
| **R185** the forced-sacrifice list order | D25 | **SHIPPED — PASS 0**, N thin | a land below a non-land in a forced-loss list: **0**, checked across every discard / bottom / AFFORD-TO-LOSE list. No annihilator mega-menu arose |
| **R186** the `(xN)` log collapse | D26 | **carried, improving** | 471 duplicate event lines inside single records (was 571): `- Phase: Draw` 217, `- Opponent drew a card` 89, `- Phase: Main phase N` 79 — **R216** |
| **R187** receipts without a payee | D27 | **carried at 3** (7 -> 3 -> 2 -> 3) | `152v125` seq 36, the Intrepid Adversary pay-any-number-of-times ETB, unchanged class — **R217** |
| **R188** ability rows with no life total | D28 | **NOT SHIPPED — carried** | cast rows now carry `- and N to the opponent at life A leaves them at B` (lane C); ability rows do not (`130v162` s25) — **R208** |
| **R189** the bare `[second copy:` as the "safe" tag | D29 | **RISK DID NOT MATERIALISE** | 97 rows / 12 takes, every take on a genuinely stacking script. Recorded closed |
| **R190** the priority seam's turn attribution | D30 | carried, not shipped | no window turned on it |
| **R191** `pass_row_named` never renders | D31 | **SUPERSEDED** | the reword replaced the stamp's shape: `pass_hold_ambiguous` now fires **16** times and its adjudication is **R203** |
| **R192** the log window | D32 | **RE-MEASURED — the trim WORKED** | see R224. Still the owner's call |

---

# NEW — R193-R224, ranked

## HIGH

### R193 — the interactive-AI interrupt-window stall floor is denominated in `dt`, so under `WAGIC_FASTCLOCK` its 1,200-second budget is 12,000 ticks: 468 GPT windows force-released, 488 model answers discarded, and the corpus's whole window economy moved. (= ledger **D1**; engine seat **HIGH #1**, seat-125-126 **HIGH #1**, seat-146 **E-1**, seat-123-130 **E2** with a competing mechanism, REFUTED)
**Evidence, re-derived on disk over all 21 stderr files.** `dropping stale async answer` **488**
(wave 53: **63**, +675%); ` - releasing so the game advances` **468**; `interrupt window held by …
for N ticks` **936 lines, one distinct N = 12002**. Per game, drops track releases and nothing else:
**r = 0.9963** against releases (equal exactly in 10 of 21 games, mean |difference| 1.14, and
**434 of 488 drops fall within ±8 stderr lines of a release**) versus **r = 0.5173** against lane
M's `chose to cast nothing` cache replays. The pair that settles it: `125v123` has **809 replays
and 45 drops**; `162v130` has **5 replays and 37 drops**. Four games produced zero releases
(`152v130`, `152v162`, `146v152`, `146v123`) and three of them carry the wave-53 baseline drop rate.
**Repro:** `game-126v123-1788425076.stderr:306-341` — seven consecutive phase steps of turn 2, seven
held windows released at 12002 ticks, seven answers dropped, **zero records written**.
**Mechanism** (pinned by two seats from source, read-only): `ActionStack::Update`
(`src/ActionStack.cpp:1386-1470`) releases on
`mHoldTicks >= 300 && mHoldSeconds >= (holder->isInteractiveAI() ? 1200.0f : 20.0f)` with
`mHoldSeconds += dt`, and `JGE/src/SDLmain.cpp:110-131` feeds a FIXED wall-decoupled dt of 0.1 s
under `WAGIC_FASTCLOCK`. Second half: `AIPlayerGPT::decisionPending` calls
`extendInterruptOffer(this)` on every in-flight tick, but `ActionStack::extendInterruptOffer`
(`include/ActionStack.h:236-248`) only resets the counters while `askIfWishesToInterrupt == who` —
once the seat has TAKEN the window the holder is `observer->isInterrupting` and the keep-alive
silently no-ops, so **the seat's only "I am still thinking" signal is disconnected from the watchdog
about to kill it**.
**Cost.** 488 completed model round trips discarded with no translog record. At the corpus mean of
23.9 s that is **≈3.2 h of inference bought and thrown away, +22% on top of the recorded 14.67 h** —
an ESTIMATE (a dropped call's true latency is unrecorded), stated as one.
**What the corpus does NOT establish, and this matters for the wave-55 falsifier.** The claim that
the floor caused the opponent-turn window collapse is supported at the SEAT level and refuted at the
aggregate: in `123v126` all 64 releases fall on one seat and that seat records **0** opponent-turn
ask/priority windows against its counterpart's 70; but games with many releases also have many
opponent-turn windows (a release presupposes an offered window), and the four zero-release games
have zero opponent-turn windows too. What IS measurable and unexplained by general shrinkage: the
opponent-turn share of decisions fell **30.2% -> 19.9%** while total decisions fell 36%.
**Layer: ENGINE.** A hotfix lane is already rewriting the floor to wall clock, never-while-pending,
and a `WAGIC_STALL_FLOOR=0` disable — do not re-design it here.
**The A/B that settles it, and the flag to prefer.** `WAGIC_STALL_FLOOR=0` on the hotfixed binary is
the clean single-variable A/B; `--realtime` (`FASTCLOCK=0`) is the no-rebuild fallback but changes
game pacing wholesale and costs ~20 wall-minutes a game. seat-123-130's recommendation to flip
`WAGIC_GPT_AUDIT_M_OFF=1` FIRST and lane AA LAST is declined on the correlation above.
**Standing note: lane AA shipped an output-affecting change with NO disable flag**, which is the
silent-instrument rule violated, and is why this could not be A/B'd from the flag table at all.
**Wave-55 prediction.** `interrupt window held … releasing` on a GPT seat: **0**;
`dropping stale async answer` **< 80 corpus-wide** (was 488, wave 53: 63); seats recording zero
opponent-turn ask/priority windows **<= 12 of 42** (was 25 by this step's predicate); opponent-turn
share of decisions back above 26%. **Falsifier:** drops stay >= 300 with the floor disarmed — then
the cause is a prompt rebuild and `WAGIC_GPT_AUDIT_M_OFF=1` is next.

### R194 — a mandatory life-loss loop opens one full LLM window per iteration, and the de-duplication key cannot see it because the row's own pricing annotation moves with the life total. 69 dead model calls in two games, 40 in one turn. (= ledger **D2**; seat-123-130 **E1 HIGH**, deck126 G8, deck130 general-suggestions)
**Repro, re-derived.** `130v126` seq **67-106**: **40 consecutive records**, all turn 18, all Main
phase 1, life 20/25 -> 0/44 one point at a time, stack line
`1 (top): ability: Sanguine Bond's Life Loss [from their Sanguine Bond]`, menu two rows. **21
distinct `options_text` tuples across the 40** — because row 1 reads
`Cast Spark Spray {r} … {no creature target - and 1 to the opponent at life 26 leaves them at 25}`
and that number ticks every iteration. `identical_option_asks_resolved` on that gameend: **0**.
`hold_windows_skipped`: **0**. Mirror at `123v126` seq **112-140** (29 records, the other side of
the same game). The HOLD row was taken six times inside the `123v126` loop and stopped nothing,
because "any change re-opens this window" is literally satisfied by a life tick.
**Ask, and the shape matters because a cap would be rejected.** (a) Make the repeat/de-dup key the
option SET — row names, costs and ordinals — rather than the rendered string, so a numeric
annotation that only tracks a moving life total cannot defeat it. (b) Give the HOLD row's own key
the same treatment: **the model asked not to be asked again this turn, and a life total moving
inside a mandatory loop is not the board change it meant.** This is honouring a choice the model
made, not removing a window it was never offered — which is why (b) is adopted and a blanket
suppression of the window is not. Nothing is withheld from a seat that has not taken the row.
**Layer: ENGINE (key), with a RENDER companion** — the annotation is correct and must not be
trimmed; only the key changes.
**Wave-55 prediction.** Decisions inside a single-phase mandatory life-loss loop: **< 5 per loop**
(was 40 and 29). `identical_option_asks_resolved` > 0 on any gameend carrying such a loop.
Identical-declined runs (wave-53 method) below 12% and max run length below 20.

### R195 — `all_assignments_illegal` declares NO blocks at a lethal window and does not re-ask: new fallback class, first firing, and it cost the game. (= ledger **D3**; engine seat **HIGH #3**, seat-146-152-162 **E-6/E-7**, deck152 R-5/R-6)
**Repro.** `152v146` seq **58**, turn 19, Blockers, **my_life 6 / opp_life 7**, latency 2,483 ms.
Header: `Your life: 6. Unblocked, these attackers deal up to 6 - you would be at 0 - LETHAL if it
all connects (at 0 life you LOSE - 0 is not survival): block enough to survive.` Both Spider
attackers carry `[menace - only a block by TWO OR MORE of your creatures counts; one creature alone
does not block it at all]`, and both blocker rows repeat it per-attacker. Reply `BLOCKS: B1:A3,
B2:A4`. The engine pruned both (`pruned_pairs: "Elite Spellbinder -> Spider (needs 2 blockers, only
1 assigned); Intrepid Adversary -> Spider (needs 2 blockers, only 1 assigned)"`), stamped
`fallback: all_assignments_illegal`, **declared no blocks at all**, and there is no follower record
and no `recovery`. The seat took 6 at 6 life and lost (`results.tsv`: 146 beat 152, life1 −11).
**Why this is the loop's own purpose inverted.** `named_row_reask` re-asks once when the model names
an off-menu row (7 firings, 5 recovered). A window where EVERY assignment was pruned is precisely
the window that should be re-asked with the rejection quoted back — `rejectedSoFar` already exists
on the cast path. Legality is enforced structurally, but enforcing it by *discarding the answer
whole* is a constraint on choice by omission.
**Ask.** (a) One re-ask carrying the pruned pairs as text; a second all-illegal reply may still
declare no blocks and is logged `all_assignments_illegal_exhausted`. (b) The record carries the
pairs and the reason (`B1:A3 (menace, 1 blocker)`), as `plan_choice_conflict` already does.
**Layer: ENGINE (a) + observability (b).** The guide half is deck152's 152-D and is skill #225 —
they are independent and both are owed.
**Wave-55 prediction.** `all_assignments_illegal` records with no follower re-ask: **0/N** (was 1/1).

### R196 — the HOLD row is excluded from the seat's OWN turn, and the corpus's longest dead run is 50 byte-identical own-turn windows with no HOLD row on the menu. (= ledger **D4**; engine seat **HIGH #2**)
**Repro.** `126v125` seq **72-121**, turn 28, phase Blockers: **50 consecutive decisions**, one
two-row menu (`1. Cast Tribute to Hunger … {right now: they control 0 creatures - at 0 this does
nothing}` / `2. Cast nothing right now`), every reply `CHOICE: 2`, latency sum **242.8 s**, and the
last prompt reads `[you declined this exact list 49 times already this turn]`. The HOLD row is not
on it, because `observer->currentPlayer == this`.
**The exclusion's stated rationale is refuted by this run.** `AIPlayerGPT.cpp:18296-18304`: "on the
seat's OWN turn the board changes with every land drop and cast, so a hold there would be taken and
retired in the same breath." After the seat's main phase is spent, its own Blockers / Combat-damage
sequence produces 50 byte-identical windows with no board change at all. The hold key already
guarantees retirement on any real change, so nothing about the guarantee needs weakening.
**Re-derived distribution.** Runs of >= 3 byte-identical declined menus: **356 / 2,212 = 16.1%**
counting a HOLD take as a non-decline, **396 = 17.9%** counting it as a decline; 47 runs, max **50**.
Lane A's target was < 15% and **fails on either predicate**. Second-largest runs are all Upkeep,
all in `123v125`, and those DO carry the row and the pilot took it repeatedly — the mechanism works
where it is offered. **The harvest's 12.2% is not reproducible**: two independent measurements (the
engine seat and this step) obtained 16.1% / 17.9% and neither could recover it (#208).
**Ask.** Offer the HOLD row on own-turn windows once the land drop is spent and the seat is past
Main 1 — or simply always. **Falsifier:** the row is offered on own-turn windows and the 50-run
class does not shrink; then the row is not the remedy and R194's key is the whole answer.
**Layer: ENGINE/RENDER.**

### R197 — a reveal chooser LIVELOCKS: the prompt is rebuilt on an unchanged board, six consecutive stale drops hand the decision to the heuristic, and lane F's force-close does not cover the shape. (= ledger **D5**; engine seat **HIGH #4**, seat-146-152-162 **E-2**)
**Repro.** `146v123` seq **15**, turn 9, Main phase 1, `kind: reveal`, 4 rows (Lightning Greaves /
Marsh Flats / Thraben Doomsayer / Damnation), `choice: -1`, `latency_ms: -1`, `fallback:
stale_livelock`, empty reply, `recovery` at s16. `game-146v123-1788425054.stderr:1335-1343` — **six
`dropping stale async answer` back to back with NO release, no phase step and no stack action
between them**, then `6 consecutive stale drops - giving this decision to the heuristic`. That game
carries **6 drops and 0 releases**, which is what makes it a different class from R193: the breaker
counts drops only on the SAME `serializeGameState()`, so the board did not move — **the prompt did**.
**The card.** Pelakka Predation, `auto=name(reveal hand) reveal:type:*:opponenthand
revealzone(opponenthand) optionone name(Choose a card) target(*[manacost>=3]|reveal) reject
optiononeend optiontwo name(put back) all(*|reveal) moveto(hand) optiontwoend revealend` —
predicate-gated option one, the same family as wave-53's hang, but here with LIVE targets (Thraben
Doomsayer mv 3, Damnation mv 4) and with an option two. The recovery record shows option TWO ran and
the Predation did nothing.
**Lane F's guard has no window on it**: its budget arms on "option one predicate-gated to zero
targets with NO option two", and `force-close` renders **0** times corpus-wide — **UNTESTED, not
PASS**.
**Ask.** (a) Extend the force-close budget to ANY interactive reveal whose driver makes no progress,
not only the no-option-two shape, and write a `reveal_stall` field. (b) Find the per-tick prompt
instability in the reveal menu build — the wave-52 deterministic-order fix evidently does not cover
`revealzone(opponenthand)`. First flag to flip is `WAGIC_GPT_AUDIT_M_OFF=1` (lane M's situation
prefill is the only wave-54 change that rewrites prompt text per tick), then `WAGIC_GPT_BOARDINDEX=0`.
**Layer: ENGINE.**
**Wave-55 prediction.** `stale_livelock` on a `reveal` record: **0**; any reveal driver making no
progress writes a `reveal_stall` field.

### R198 — the X menu's decisive marker rendered ZERO times in 42 logs, and the unmarked menu lost a game. (= ledger **D6**; seat-123-130 **E3**, deck130 **G6/G7**; skill #221)
**Evidence.** `[<- most kills at any affordable X that costs you nothing]`: **0 renders across all
42 `.jsonl` files**, re-derived by this step. It prints only when some affordable X kills something
of theirs and nothing of yours. **Wave 53 scored this rung "taken 2/2" and predicted N/N again.**
**Repro.** `130v152` s17, 9 life against 25, largest-first per the owner ruling:
`1. X = 4 {kills THEIRS: Intrepid Adversary, Elite Spellbinder, Sigarda; YOURS: Siege-Gang
Commander, Goblin x3}` / `2. X = 3 {kills THEIRS: Intrepid Adversary, Elite Spellbinder; YOURS: …}`
/ `3. X = 2 {same kills as X=3, for 1 less mana}` / **`4. X = 1 {kills THEIRS: none; YOURS: Goblin
x3}`** / `5. X = 0`. Answered **4**, under `PLAN: Kill Sigarda and Elite Spellbinder with Starstorm
X=1` — a sentence the row it named denies in its own words. It swept its own three Goblins, killed
nothing, and the game was over three life later.
**Second half, and it is the same menu.** Row 3 — the row the guide's own STEP 2 wants taken — is
the ONLY row with no kill list on it, because the `same kills as X=N` collapse removed it. At
`130v126` s58 the pilot took the TOP of a collapsed run for the third corpus running.
**Ask.** (a) Always mark exactly ONE row and say what the mark means: keep the current marker when
some X costs nothing, otherwise `[<- best trade: the most of THEIRS at the smallest cost to YOURS]`
on the cheapest row whose `kills THEIRS` list is maximal. (b) Put the mark on the BOTTOM row of a
collapsed run, so the row that should be answered is the row carrying the annotation — or restate
the kill list on every row. Neither removes a row or re-orders the menu (largest-first stands).
**Layer: RENDER.**
**Wave-55 prediction.** X menus rendering with no marker: **0** (was 2 of 2). Takes of a
`kills THEIRS: none` row while another affordable row names an opponent creature: **0** (was 1).
Takes of the TOP row of a collapsed `same kills as X=N` run: **0** (was 1, third corpus).

### R199 — a `target=player` row enumerates BOTH players and prices only one of them: 178 of 180 Devour Flesh rows say `they control 0 creatures - at 0 this does nothing` while listing `you` as a legal target. (= ledger **D7**; engine seat **D-2 HIGH**)
**Evidence, re-derived.** **180** `Cast Devour Flesh` rows carry a `legal targets right now:` list;
**178** enumerate `you`; **178** carry a `{right now: they …}` verdict that speaks only of the
opponent. Takes: 0. Primitive: `name=Devour Flesh / target=player / auto=ability$!name(sacrifice)
notaTarget(creature|mybattlefield) dynamicability<!mytgt toughnesslifegain targetcontroller!>
sacrifice!$ targetedplayer` — **the TARGETED player sacrifices**, so self-targeting is a real and
frequently relevant play, and the verdict is FALSE for half the row's own enumerated target list.
**Cost, measured.** `123v126` seq **104** (turn 19, Combat begins, 58 Humans on board, Sanguine Bond
+ Exquisite Blood live): the reply spends ~900 words working out whether taking the row lets it pick
a target at all — *"It doesn't specify target yet… If I cannot target myself with this menu item, I
cannot trigger the loop."* — then abandons a genuine win attempt and takes the HOLD row.
**Ask.** (a) Give the verdict per player when a `target=player` row enumerates both
(`{right now: they control 0 creatures - at 0 this does nothing; YOU control 58 - you would
sacrifice one and gain its toughness}`). (b) State whether the row COMMITS a target or opens a
follow-up ask — the model asked this question explicitly and the prompt answers it nowhere.
**Layer: RENDER (a) + CORE PROMPT (b).**
**Wave-55 prediction.** Rows enumerating both players whose `{right now:}` verdict names only one:
**0/N** (was 178). A take of the self-target is NOT predicted — that is a deck-seat call.

## MED

### R200 — the one-source × N-identical-target ability menu is uncollapsed and is now the corpus's largest render waste: 46 records, 403 redundant rows, ~160,800 chars. (= ledger **D8**; engine seat **D-3**, lane D's explicitly-not-shipped 2-D collapse; R2/R168's residual)
Re-derived (family key = row text with the ordinal masked; redundant = family size − 1, families of
>= 3): **46 ask/priority records, 403 redundant rows, 160,803 characters**. Families:
`Equip with Lightning Greaves targeting Human #N` **190**, `… Vampire #N` 93, `Mountain #N` 82,
`Deal 2 damage with Siege-Gang Commander targeting Goblin #N` 30, `Human #N` 29,
`Deal 2 damage with Pyrite Spellbomb targeting Goblin #N` 12, `Destroy with Dwarven Blastminer #N` 8.
Repro: `123v162` seq **26** — 23 rows, every one `Equip with Lightning Greaves targeting Vampire #N
[your battlefield] (Lightning Greaves is ALREADY attached to Bloodline Keeper - this MOVES it …)`,
on 23 identical 2/2 tokens, answered `pass`. Adjacent: `123v126` seq **88** is a 58-row attackers
list and seq **106** a 61-row blockers list of interchangeable `Human #N` tokens.
**Ask.** Extend lane P's `groupNumberedRows` permutation (already index-safe with its map-back) to
ability, sacrifice and library-reveal menus, in the two-dimensional form (source × target). **The
answer is a COLLAPSE (`Human #1-#58 x58, all identical`), never a cap** — the standing ruling.
**Wave-55 prediction.** Ask/priority menus containing more than 3 rows of one `#N` shape: **0**;
redundant row chars **< 20,000** (was ~160,800).

### R201 — the `discard` ask is the only ask kind whose rows carry no verdict at all, and it is the seat that loses games on it. (= ledger **D9**; deck125 **G1**, seat-125-126 **HIGH #2**)
`125v162` seq **47** (t17, 8 life, `kind: discard`, send 2): the nine options are bare card names —
`['Fall of the Gavel','Lightmine Field','Final Judgment','Dream Fracture','Path to Exile','Fall of
the Gavel','Dream Fracture','Island','Path to Exile']`. On the SAME prompt the engine already prints
`Your battlefield (9 permanents … and 9 are lands)` and the opponent's `0 are creatures`. It sent
both Fall of the Gavel — its only answers to a noncreature — and died two turns later. Same shape at
`125v126` s73 (both Gavels sent on a ten-land board, the turn before an uncountered Sanguine Bond)
and s80. Every other ask kind annotates: `{right now: …}`, `{kills:}`, `{removes:}`,
`at 0 this does nothing`, `[second copy: …]`, `[DRAW PRICE: …]`, `{leaves N …}`,
`[finds only an enchantment card …]`.
**Ask, three clauses the emitter already computes elsewhere:** `{spare: you control N lands}` on a
land row (N from the header line), `{dead right now: they control 0 creatures}` on a row whose card
gates on the opponent's creature count — the same string family the seat declines 40 of 40 times on
cast rows — and `{you already control one}` on a row naming a card on its own battlefield line.
**Layer: RENDER.** Guide half shipped as W99/W100; skill #215 says why the guide alone cannot fix it.
**Wave-55 prediction.** `discard` rows carrying no annotation of any kind: **0**. Cleanup discards
sending a counterspell while a land is on the list and the header reads 6+ lands: **0** (was 3 of 4).

### R202 — a cast row whose price is set by a tag on the OPPONENT's battlefield line does not carry that price. (= ledger **D10**; deck126 **G5**, seat-125-126 STRATEGY #2; skill #219)
`126v146` seq **42** (t26, 24 -> 21) and seq **61** (t32, **8 -> 5**, opponent on 5): the row reads
`Cast Exquisite Blood {4}{b} {leaves 3 of your 8 untapped mana sources untapped} {card text: …}`
with no mention of the 3 life, while the opponent battlefield line one screen up reads
`Silverquill Silencer {b}{w} (3/2) [named: Exquisite Blood]` and the events that follow read
`- You cast Exquisite Blood | - You lost 3 life (now 21) | - Opponent drew a card`. The seat died at
−5 against 5 having paid 6 to the Silencer. Every other cost this engine surfaces is priced ON the
row. The magnitude is readable from the ability, not guessed:
`@movedto(*[chosenname]|opponentstack):life:-3 opponent && draw:1 controller`.
**Ask.** `[NAMED BY THEIR <source>: casting this costs you 3 life and draws them a card - you would
be at K]`, on the cast row, reusing lane C's `- you would be at K` subtraction.
**Note, and it bounds the item:** the tag itself is the corpus's best-shaped new render (54 records,
#193's dedicated-line device) and **nothing about it should change**. Two seats asked for that in
writing.
**Wave-55 prediction.** Casts of a card named in an opponent `[named: <name>]` tag with no price on
the row: **0** (was 2 of 2 at the seat it cost).

### R203 — the reworded HOLD row shares its first two words with row 0, and the parser resolves the collision by index: 16 `pass_hold_ambiguous` stamps, 13 false positives and 3 genuine. (= ledger **D11**; seat-146-152-162 **E-5**, engine seat, seat-125-126 **LOW #5**, deck146 **R-1**, deck125 **G4**)
**Adjudicated record by record, and the reviewer brief's corpus note is CORRECTED.** 13 stamps are
`CHOICE: 0 (pass)` executing row 0 correctly — false positives, because `pass` is an EXACT match for
row 0's own label and only a case-insensitive PREFIX of `Pass priority, and do not ask me again…`,
and the matcher has no exact-match tiebreak. **3 are genuine**: `146v126` seq **54, 56, 57**, all
`CHOICE: 3 (Pass priority)` on a 3-row menu whose row 3 IS the HOLD row, all executed row 3, all
counted in the 43 HOLD takes — a real name collision the index happened to resolve.
**Ask, two independent halves.** (a) Parser: an EXACT match against a row label beats a prefix match
— this kills 13 of 16 stamps with no behaviour change. (b) Prompt: make the two rows' leading words
differ, which also makes them distinguishable to the PILOT (`0. Pass this window` vs the HOLD row's
tail-led wording), or key the HOLD row's name-match on its tail clause. The reword is lane A's call.
**Wave-55 prediction.** `pass_hold_ambiguous` firings **<= 4**, all of the `CHOICE: n (Pass
priority)`-on-the-HOLD-row shape; `CHOICE: 0 (pass)` never stamps again.

### R204 — the mulligan coverage clause's two cause families call for OPPOSITE decisions and are rendered in one grammar. (= ledger **D12**; seat-146-152-162 **E-4**, deck146 **R-2**; skill #210)
11 renders, **11 with a cause** (R176 PASSES), in two families: `(1 land is not enough for your
cheapest spell at mana value 2)` — a genuine count failure — and `(you have 3 lands; no spell in it
is castable off {W}{U} alone)` — a TURN-ONE colour statement about a hand three rungs above any
floor. `152v146` s1 shipped a three-land seven under the second, with the reply naming a colour as
its reason, for the FOURTH corpus that floor has broken on the same shape. Every word of the clause
is true and the trust doctrine says the pilot must believe it; the defect is that a turn-one
statement is rendered in the grammar of a hand-level verdict, directly above a Keep/Mulligan ask.
**Ask.** Append the scope the clause already implies — `… castable off {W}{U} alone **on turn one**`
— or split the render into the two families explicitly. The count family needs no change.
**Layer: RENDER.** The guide half is deck152's 152-B re-key and skill #210; both are owed.
**Wave-55 prediction.** Mulligans taken under a COLOUR-family coverage clause at 2+ lands: **0**.

### R205 — the blockers ask has no count header for a constraint that voided a whole answer. (= ledger **D13**; deck152 **R-5**, seat-146-152-162 **E-6**)
Lane E gave the attackers prompt `They have N untapped creature(s) able to block …` and it renders
**99 of 99**. The blockers prompt has no equivalent for the menace arithmetic. At `152v146` s58 the
menace rule was stated three times per blocker and the seat still spread one blocker across two
menace attackers. **Ask:** one header above the B-rows, on D17's pattern —
`2 of the 4 attackers need TWO or more blockers each; you have 2 blockers.` A count claim over rows
the engine already computes, in the seat's own reading order, visible BEFORE the answer is written
rather than after it is discarded. Pairs with R195; neither substitutes for the other.

### R206 — the record for a discarded blockers answer names no offending pair. (= ledger **D14**; deck152 **R-6**)
`all_assignments_illegal` carries the class name and nothing about the cause; the reviewer had to
re-derive the menace rule from the prompt. **Ask:** carry the pairs and the reason in the record
(`all_assignments_illegal: B1:A3 (menace, 1 blocker), B2:A4 (menace, 1 blocker)`), as
`plan_choice_conflict` was made readable in wave 53. One field. (The engine already computes
`pruned_pairs` and the RECORD carries it — what is missing is that the fallback class does not
surface it in the same place the ledger reads, so this is a small item, not a new computation.)

### R207 — a single-target `{kills:}` list names the caster's own permanents under a consequence label with no ownership mark. (= ledger **D15**; deck130 **G8**, seat-123-130 **E6**)
`130v146` s23: `Cast Stone Rain {2}{r} {kills: Plains #1, Plains #2, Mountain #1, Mountain #2,
Mountain #3, Mountain #4, Mountain #5}` — five of the seven are the pilot's own Mountains, and the
row destroys exactly ONE of them. The target menu one screen later (s24) marks every row
`[your battlefield]` / `[opponent's battlefield]`; the cast row does not. This is R164's
land-destruction sibling, and it is a live risk on a deck whose whole plan is land destruction.
**Ask.** `{kills whichever ONE you target: theirs - Plains #1, Plains #2; yours - Mountain #1-#5}`,
or name only the opponent's permanents and state the count of your own.

### R208 — ability rows that point damage at a player still print no life total. (= ledger **D16**; R188/D28 carried, deck130 **G9**, seat-123-130 **E9**)
Cast rows have carried the tail since lane C (`130v126` s66:
`- and 1 to the opponent at life 24 leaves them at 23`); ABILITY rows have not. `130v162` s25:
`Deal 2 damage with Pyrite Spellbomb targeting Ob Nixilis, the Hate-Twisted` beside a menu whose
player row reads `the opponent (player, life 12)`. Same clause, other emitter.

### R209 — fetch rows name a land and never its colours. (= ledger **D17**; R180/D20 carried, deck123 **G4**, seat-123-130 **E7**)
21 renders at deck123 of `{this land makes no mana - crack it for a land: your untapped mana sources
go from N to N+1}` with no colour on any row, while the mana line reads `colours you can make:
{b}{w}`. Behaviour improved without it (4 of 5 fetches took a dual, was 5 of 8), so this is now
correctness rather than bleeding. **Ask:** continue the clause with the colours in the mana line's
own words — `…, and it adds {W} or {U} (you cannot make {U} right now)`. The emitter already prints
`(land: taps for {W}{U})` on hand lines.

### R210 — library-reveal menus do not collapse duplicates. (= ledger **D18**; deck123 **G3**, seat-123-130 **E8**)
`123v125` s9 lists **50** rows with `Intruder Alarm` ×4, `Damnation` ×4, `Thraben Doomsayer` ×4 as
separate identical strings; `123v126` s23 lists 45 the same way. The battlefield line directly above
already collapses identical permanents with `xN` handles. Same `groupNumberedRows` fix as R200,
different seam.

### R211 — the declined-count annotation does not reach the seat's OWN same-turn Main 1 / Main 2 pair. (= ledger **D19**; deck162 **R-8**)
`162v126` s7-s12 and `162v146` s7-s11: the identical two-row ask re-issued at Main 1 and Main 2 of
one turn, then again next turn, unchanged board, unchanged answer. These are own-turn windows, so
the HOLD row does not render and lane A's suppression cannot reach them.
`[you declined this exact list N times already this turn]` appears on **4 of that seat's 630
decisions**. **Ask: the ANNOTATION only** — extend it to the same-turn Main 1 / Main 2 pair so the
second ask carries the count. The seat explicitly did not ask for the suppression, and the
suppression version would be a cap. Pairs with R196.

### R212 — the `{feeds:` converter count counts a conditional converter unconditionally. (= ledger **D20**; deck162 **R-10**)
The row prints `converters on your battlefield: K - <names>` counting Liliana's Caress, but a lone
Caress converts only DISCARDS, so the guide carries a hand-written qualifier ("K counts only while
the Opponent hand size line reads 7 or more"). **Ask:** have the row do the arithmetic —
`converters on your battlefield: 1 - Liliana's Caress (discards only; their hand is 5, so this
converts nothing yet)`. Same class as R164/R165/R170, and it would delete a paragraph from the
pool's smallest guide. **No window in this corpus turned on it** — filed as an ask, not a defect.

### R213 — the HOLD row states its RISK and never its BENEFIT. (= ledger **D21**; deck125 **G3**)
426 renders, 43 takes (10.1%), and in windows already carrying the declined-note at N >= 3, **22 of
212 = 10.4%** — a 25× improvement on wave 53's 0.4% and half of lane A's >= 20% target. The row's
text is accurate and reassuring about the risk (`any change re-opens this window; you give up no
cast`) and says nothing about what taking it saves, so a pilot that is not worried about the risk
has no reason to press it. **Ask:** append the count the engine already tracks and already prints
elsewhere in the same prompt — `(you have declined this same list N times this turn; taking this row
skips the rest of their turn's identical windows)`. A re-siting of an existing literal, not a new
fact, measurable against `hold_windows_skipped`, which now works.

### R214 — the `chooseaname` menu is headed as a MODE menu and carries no annotation, 9/9. (= ledger **D22**; lane C's carried item, confirmed by two seats)
The header renders 9 times, verbatim `Choose one mode for Silverquill Silencer:`, followed by ten to
fourteen rows of bare card names, with no mana, no board effect, no card text, no dead marker.
Primitive: `auto=chooseanameopp transforms((…)) forever chooseend nonland`,
`text=As Silverquill Silencer enters, choose a nonland card name.` **It is a card-NAME menu, not a
mode menu, and the header names the wrong thing.** The OBSERVING seat's surface (`[named: <card>]`
on the battlefield line) renders correctly, so the defect is scoped to the ACTOR's menu — and both
halves should be fixed as one item so the same fact is not described two ways on two seats.
**Ask:** correct the header, and annotate the rows with what naming that card would do (the trigger
is on the ability: `@movedto(*[chosenname]|opponentstack):life:-3 opponent && draw:1 controller`).

## LOW

### R215 — a 900 s wall miss with no translog record at all. (= ledger **D23**; engine seat **D-8**)
`no reply after 900s - one retry` appears twice. One produced a full record (`123v125` s1, `retry:
1`, `deadline_pct: 165.4`, answered on the retry — lane Q's arm working, first live firing). The
other (`game-146v123-1788425054.stderr:1607`) was retried and the window then auto-passed
(`only display-toggle (Flip Side) options; auto-passing without a model call`) with **no decision
record written**. **Ask:** a `wall_miss_unrecorded` counter on the gameend record, or write a
zero-choice record.

### R216 — the `(xN)` log collapse still misses phase lines and draws. (= ledger **D24**; R186 carried)
**471** duplicate event lines inside single records (wave 53: 571 — improving): `- Phase: Draw` 217,
`- Opponent drew a card` 89, `- Phase: Main phase N` 79, token-ceased lines 9, `- Phase: Combat
damage` 5, loyalty-counter lines 3. The wave-53 verdict on ADJACENT collapsing stands and is not
re-opened; this is the within-record class.

### R217 — receipts without a payee: 3. (= ledger **D25**; R187 carried)
`152v125` seq 36, the Intrepid Adversary pay-any-number-of-times ETB. `- Paid` renders **369 receipt
lines across records' `events`** (15,170 occurrences across all prompt texts — the log repeats
them), **0 with no source tapped** in all 21 stderr files. The no-payee class runs 7 -> 3 -> 2 -> 3.

### R218 — repeated resource 404s, and the memo does not cover them. (= ledger **D26**; engine seat **D-9**, lane J watch)
**1,927** `AttemptNew failed to load (404)` lines over **118 distinct** resources; `mana.wav` 105×,
`land.wav` 105×, `creature.wav` 96×. The reveal display in `game-146v123:1250-1334` re-cycles the
same four card images ~20 times in 85 lines with `Destroying WCachedResource` between them. Lane J's
resource-miss memo is not reaching the audio loader or surviving the destroy. **Flag for the A/B:
`WAGIC_RENDER_NOCACHE=1`.** Desktop/Vita visual parity was eyeballed by nobody, including this step.

### R219 — the effect label still falls back to a generic when the ability has no name. (= ledger **D27**; R179's residual)
401 `ability:` stack-line occurrences over 51 distinct forms, all correctly possessive-and-sourced;
**9** read `ability: Ob Nixilis, the Hate-Twisted's triggered ability`. The name-twice bug is fixed;
the generic label survives. Cosmetic.

## PASS / KEEP — recorded so no later lane trims them
- The `{feeds:` clause and its K-of-0 long form (deck162 **R-9**): it carries a whole deck's central
  rule on the row that needs it, and **its ABSENCE on non-gift rows is now that deck's brake key** —
  collapsing it onto a header would destroy the absence signal.
- The `[named: <card>]` battlefield tag (deck126 **G5/G6**, deck125 LOW #6): 54 records, correct and
  legible, the #193 device. Only the ROW's price is missing (R202).
- The life-loop clause `(you kill it, your attacker lives (blocking trigger: they gain 3, and their
  converter takes 3 off you))` and `(their life LOOP is in play …)` (deck152 **R-7**): it let deck152
  race a Sanguine Bond board with two fliers and win 20/-10 without dropping a life point, and it is
  what made deck123's `ATTACK: none` at 58 attackers CORRECT (skill #213). **Do not trim the second
  half of the parenthesis — the decision turns on it.**
- The reveal chooser's per-row `[does NOT qualify - goes to "put back"]` / `[eligible for "choose a
  card"]` verdicts (deck146 **R-4**): the clearest annotation in the corpus; the pattern to reuse
  when R200's collapse is written.
- The coexistence of `{modes live right now:}` + `[DRAW PRICE:]` + the menu-fit clause on one row
  (`146v162` s46 row 5, deck146 **R-3**): three lane-C clauses on one row without displacing each
  other. A future lane must not "simplify" one off.
- The mulligan coverage clause's cause parenthetical (deck126 **G7**): a request that it not be
  SHORTENED, only scoped (R204).

## REJECTED, and why
- **"Skip the opponent-turn window whose stack is empty at a phase this seat has never cast in"**
  (deck125 **G2**, third wave running, restated at 41 windows / 0 casts). **REJECTED again.** A
  phase's cast history is not a legality fact; wave 53 supplied the counter-example (`125v152` seq
  15, a real cast at the Attackers step). **And this wave the evidence is inadmissible on its own
  terms**: the seat's opponent-turn window count is exactly the number R193 moved, so the 41/0 is
  measured on a contaminated denominator (#217). The independent lever the seat wants is R193's fix.
- **"Cap re-opens within one phase when the only board change is a life total"** (seat-123-130 E1's
  second half), *in that form*. A blanket cap suppresses windows the model never asked to give up.
  **Adopted in the narrower form under R194(b): the HOLD row's own key, which the model opts into by
  taking the row, ignores a life-only change.** Nothing is withheld from a seat that has not taken it.
- **The suppression half of deck162 R-8.** The seat itself asked only for the annotation; the
  annotation is adopted (R211) and the suppression is not proposed.
- **Any blind identical-menu cache.** Not re-opened; wave 52's 30 real actions after a decline on a
  byte-identical set stand.
- **seat-123-130's E2 mechanism for the stale-drop spike** (lane M's ask cache, with lane AA flipped
  LAST as "the RESCUER"). **REFUTED on disk**: drops vs releases r = 0.9963, drops vs cache replays
  r = 0.5173, and `125v123` (809 replays / 45 drops) against `162v130` (5 / 37) kills the two-point
  fit. Its sibling claim that "each drop is a wasted model call is REFUTED" is itself wrong — a
  dropped answer writes no record, so a 41-decision seat log is perfectly consistent with 37 extra
  calls. Recorded as skill **#231**.
- **seat-146-152-162's E-3** (the record's `chosen_text` naming a row the engine did not execute,
  and `latched_row_mismatch` firing on bad fields). **REFUTED on disk**: the stderr block it cites
  sits five engine turns before the record's window and has the wrong option arity; seq 63's own line
  is `-> chose 5 of 5` under a later turn banner. The telemetry is right, the stamp is a true
  positive, and the alarm that "everything downstream that reads `chosen_text` is affected" is
  unfounded. Recorded as skill **#230**.
- **A guide compensation for any of R193-R199.** Trust doctrine: a guide describes cards and printed
  strings, never the engine's window cadence.

## MEASUREMENT
- **The window economy must be reported per seat-turn** next wave, alongside `dropping stale async
  answer` and `releasing so the game advances` counts per game — the two numbers that say whether
  R193 is closed and whether any other rate is comparable (skill #217).
- **Lane M's cache-hit counters reach no log.** Its central watch item is unmeasurable from this
  corpus; a `cache_hits` field on the `gameend` record is the cheap fix. Note also that R193 means
  any hit rate measured this corpus is contaminated by re-asks.
- **Lanes G / H / I / N are not observable from a corpus at all** (no RSS, no ASAN, no per-tick
  verdict trace, no Vita exposure) and are recorded UNTESTED by three seats — not clean.
- Report the marker RENDER count for every optional annotation a guide rung keys to (skill #221) —
  the X-menu marker was scored PASS twice at 0 renders.
- Report `N raw / M live / K consequential` for every rung (skill #216).

### R224 — MEASUREMENT / DECISION: the log window — OWNER ITEM, and the trim WORKED. (= ledger **D28**; R192/D32 continued)
Prompts over 30,000: **2** (was 12). Over 25,000: **37** (was 588). `ask` mean **10,269** (was
14,245, −28%); max prompt **32,215** (`146v126` s86, priority, turn 31). Log share ~56% of prompts
carrying a CURRENT SITUATION marker, ~80.6% on the log-cost specimen. Lane L's 24 KB trim v2 is the
visible win and the tail is no longer the growth story. **What replaced it as the cost story:
recorded inference per decision went UP — 23.9 s (14.67 h / 2,212) against wave 53's 21.7 s, +10.2%,
on a corpus whose mean prompt is 28% smaller — and the 488 dropped calls are not in that total at
all.** Closing R193 is worth more than any prompt edit this wave. Log compaction remains the owner's
call and nothing here proposes one.
