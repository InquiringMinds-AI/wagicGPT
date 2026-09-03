# Wave-54 engine ledger — THE WAVE-55 DOCKET (engine / UX only)

Corpus `matchups-20260903-034355` (ELEVENTH fair-hand corpus), binary master **caf023dbc**
(hermetic `make -B`; engine lanes A e21f81de5, B 351ad0098, C a2bc290d0, D 6c1555bb1, E 5fe998414,
F 3329c40f1; **wave-53 lane AA 37b7951d3 — first corpus exposure**; audit lanes G 6dee5d2cd,
H e078c2501, I 85c703453, J fbd46fa04, K afd88acd0, L 27f867152, M ba784277f, N c2f34b05a; owner
O1/O2 1afe6c8f6. Gate: suite 1226 with the 2 known concurrency-only failures, 51 AI/0, 0 timeouts,
PARSETEST 2489/0; archived `archives/wagic-caf023dbc-w54step1`). Pilot **qwen36-35b-a3b** on Spark
:8084. Corpus root `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-034355/`; results file
`corpus-results.tsv`. Seat names `deckA-vs-deckB` = the deckA seat's JSONL, written `AvB` below;
`seq` is the record's own field. **Every number in this file was re-derived on disk by the synthesis
step** with python over the JSONL and `/usr/bin/grep` over the `.stderr` files.

**INVARIANT 00: THIS CORPUS IS 21/21 NATURAL — wave-54 docket D1 is DISCHARGED.** 42 `gamestart`,
**42 `gameend`**, no seat log without a `gameend`, 0 timeouts, 0 crashes, 0 hangs, 0 adjudications,
and lane F's new no-progress supervisor never fired. Wall time 1 h 45 min for the whole corpus
against wave 53's ~21 h. Wave-53's Sigarda reveal hang did NOT recur: the four Sigarda Coven
`reveal:5` windows (`152v126` s20/29/35/45) all found a Human, asked the model and resolved.

**Health.** **2,306 records** = 42 `gamestart` + 42 `gameend` + **10 `recovery`** + **2,212
decisions** (ask 1,683 / priority 332 / attackers 99 / blockers 36 / discard 34 / reveal 20 /
bottom 8). Fallbacks **23 / 2,212 = 1.04%** (wave 53: 0.43%): `plan_choice_conflict` **12**,
`named_row_reask` **7**, `named_row_not_offered` 2, `stale_livelock` 1, `all_assignments_illegal` 1
— **no `empty_reply`, no `timeout`, and 0 records with `choice: -1` and no `fallback`** (was 1).
The rise is two classes at two seats and is diagnosed, not mysterious: 7 of the 12
`plan_choice_conflict` and all 7 `named_row_reask` belong to deck123's two guide-authoring shapes
(skill #211 and its cast-list numbering). **Baka executed 2 decisions = 0.09%** (wave 53: 0.11%),
both the `named_row_reask_exhausted` tail of `123v125` (`game-125v123-1788425036.stderr:1655`,
`:2121`); `giving this decision to the heuristic` **1**.
`parse_note` 61: `decision_reversed_in_prose` 18, **`pass_hold_ambiguous` 16** (new; 13 false / 3
true — **D11**), `named_row_not_offered` 6, `plan_choice_conflict_exhausted` 6,
`plan_choice_conflict_recovered` 6, `stale_echo_in_range` 5, `named_row_reask_recovered` 5,
`named_row_reask_exhausted` 2, **`latched_row_mismatch` 2** (new; both TRUE positives),
`blocks_last_line_taken` 2, `long_reply` 1, `echo_index_conflict` 1, `name_over_index` 1,
`repeat_count_under_two` 1, `multiblock_first_wins` 1. `hold_row_named` **0**,
`plan_contradicts_noop_row` **0**, `engine_answered` **0**, `force-close` **0**, `http_error` **0**,
`index_name_conflict` **0**, `plan_missing` **0**, `with no source tapped` **0**.
stderr: **`dropping stale async answer` 488** (wave 53: 63) and **`releasing so the game advances`
468** — **D1**; `auto-passing without a model call` 2,061; `all actions pass-declined` 474;
`land-drop ask NOT issued` 892; `cast ask NOT issued` 319; `repeat plan iteration` 89;
`combat decision suppressed` 397; `hold re-opened` 21; `declared blocks from` 36 (1:1 with the 36
`blockers` records); `no reply after 900s - one retry` 2; `AttemptNew failed to load (404)` **1,927**
over 118 distinct resources.
`gameend` counters: `hold_windows_skipped` sum **72**, non-zero on **7 of 42**;
`mana_only_windows_skipped` sum 118 on 2; `identical_option_asks_resolved` **5** on 2.
Max seat log **312** (`123v125`); max prompt **32,215** (`146v126` seq 86); **2 prompts > 30,000**
(was 12), **37 > 25,000** (was 588). `- Paid` **369 receipt lines across records' `events`**
(15,170 occurrences across all prompt texts — two units, both stated, #208). Total recorded
inference **14.67 h** over 2,211 timed decisions = **23.9 s per decision** (wave 53: 21.7 s, **+10.2%
on a corpus whose mean `ask` prompt is 28% smaller**).

**THE CONTAMINATION NOTICE — read before scoring any per-window rate below.** Wave-53 lane AA's
interrupt-window stall floor is denominated in `dt`; `WAGIC_FASTCLOCK=0.1` makes its 1,200-*game*-
second interactive-AI budget **12,000 ticks**. **936 of 936 `interrupt window held … for N ticks`
lines read N = 12002 — one value, no variance.** Opponent-turn ask/priority windows fell
**1,052 -> 441 (−58%)** while total decisions fell 36%; the opponent-turn SHARE of decisions fell
**30.2% -> 19.9%**. Rates whose denominator is an opponent-turn window are marked **UNTESTED
(denominator moved)**, not FAILED. This is **D1** and skill **#217**.

**Mulligans (real hands).** 42 opening asks; **9 mulligan decisions across 8 seats** (one seat twice,
`152v123`), **8 `bottom` records** (wave 53: 5) — the first corpus with a usable bottoming sample,
and per skill #222 it is reported as offers/ships/order-implied with **no verdict**. Bottomed:
Chromatic Lantern, Master of the Feast, Island, Vision Skeins, Peer into the Abyss, Teferi, Tragic
Slip, and `Barkchannel Pathway + Tovolar's Huntmaster`. No mull-to-zero and **no mulligan decided by
the heuristic**. **THE OWNER QUESTION, RULED HERE AND CLOSED:** the brief's standing trigger is a
*repeated* mulligan against an explicit guide floor. The corpus contains **no repeated mulligan
against a floor** — the one chain (`152v123`) was a 1-land seven at both looks, correct at both. The
one WRONG ship is `152v146` s1, a **single** mulligan of a 3-land seven under the new colour-family
coverage clause. Under the 2026-08-26 doctrine that is a deck-content call owned by that deck's
reviewer, who flagged it, diagnosed it (the coverage literal changed underneath the rule this wave),
and shipped the fix (152-B). **No deck-content question is open for the owner.** Its render half is
**D12**.
deck130 shipped **0 mulligans and 0 bottoms for a fourth corpus** — #132-UNTESTABLE-AT-THIS-SEAT.

**Ownership (owner doctrine 2026-08-26).** Engine/UX only below. Guide content is the reviewers';
guide-side halves are named only where a docket item retires one. **Hard caps on the model's legal
choices are rejected on sight**; every item adds a true token, a true row, a receipt, a stamp or one
re-ask, and deletes nothing. Card-behaviour defects go CARD-SCRIPT FIRST. Four window-removing
proposals are rejected this wave and are recorded in `general-strategy.md`; one (D2b) is adopted in
the narrower form of a key the model opts into by taking a row.

**STANDING PROCESS FINDING, and it applies to every lane from here on.** Lane AA shipped an
output-affecting change with **no disable flag**. It is the one wave-54-corpus change that could not
be A/B'd from the flag table, and it is the change that moved the corpus. The silent-instrument rule
("was it me?" is one env var) is not optional for engine lanes; **a lane whose change can alter any
emitted string, any window, or any timing ships its flag in the same commit.**

---

# THE DOCKET, RANKED

## D1 — HIGH — the interactive-AI interrupt-window stall floor counts `dt`, not wall clock: under `WAGIC_FASTCLOCK=0.1` its 1,200-second budget is 12,000 ticks, and 468 GPT windows were force-released with 488 model answers discarded. **HOTFIX LANE IN FLIGHT — carried as a wave-55 verification item**
(= `general-strategy.md` **R193**; engine seat **HIGH #1**, seat-125-126 **HIGH #1**,
seat-146-152-162 **E-1**; seat-123-130 **E2** proposed a competing mechanism and it is REFUTED here)
**Repro.** `game-126v123-1788425076.stderr:306-341`, turn 2, all on Player 2: seven consecutive
phase steps (Main 1 -> Combat begins -> Attackers -> Combat ends -> Main 2 -> End -> Cleanup), each
one `interrupt window held by Player 2 on 'NextGamePhase. (Current phase is: …)' for 12002 ticks
(turn 2 phase N) - releasing so the game advances`, each followed by
`AIPlayerGPT: dropping stale async answer`, **and zero records written**. Also
`game-125v162-1788425040.stderr:801` and `:918` — the same on `StackAbility. (Source: Howling Mine)`
and `(Source: Underworld Dreams)`, which are the exact permanents the deck125 seat spent its game
trying to answer.
**Mechanism (pinned from source by two seats, read-only).** `ActionStack::Update`
(`src/ActionStack.cpp:1386-1470`) releases on
`mHoldTicks >= 300 && mHoldSeconds >= (holder->isInteractiveAI() ? 1200.0f : 20.0f)` with
`mHoldSeconds += dt`; `JGE/src/SDLmain.cpp:110-131` feeds a FIXED, wall-decoupled dt (default 0.1)
under `WAGIC_FASTCLOCK`, which `tools/selfplay-harness.sh` sets for every corpus. Second half:
`AIPlayerGPT::decisionPending` (`AIPlayerGPT.cpp:20659-20675`) calls `extendInterruptOffer(this)`
every in-flight tick, but `ActionStack::extendInterruptOffer` (`include/ActionStack.h:236-248`) only
resets the counters while `askIfWishesToInterrupt == who`; once the seat has TAKEN the window the
holder is `observer->isInterrupting` and the keep-alive silently no-ops — **the seat's "I am still
thinking" signal is disconnected from the watchdog about to kill it.**
**Adjudication of the competing mechanism, re-derived over all 21 games.** drops vs releases
**r = 0.9963** (equal exactly in 10 of 21 games; mean |difference| **1.14**; **434 of 488 drops
within ±8 stderr lines of a release**) versus drops vs lane M's `chose to cast nothing` cache
replays **r = 0.5173**. The killing pair: `125v123` **809 replays / 45 drops**; `162v130` **5
replays / 37 drops**. seat-123-130's reading — that the replay count "tracks the drop count exactly"
and that lane AA is "the RESCUER, not the cause", to be flipped LAST — rests on two games and is
**REFUTED**. Its sibling claim that "each drop is a wasted model call is REFUTED" is also wrong: a
dropped answer writes no record, so a 41-decision seat log is entirely consistent with 37 extra
completed calls.
**Cost.** 488 completed round trips discarded. At the corpus mean of 23.9 s that is **≈3.2 h of
inference bought and thrown away, +22% on top of the recorded 14.67 h** — an ESTIMATE, since a
dropped call's true latency is never recorded, and stated as one.
**What the corpus does NOT establish.** The seat-level asymmetry is stark (`123v126`: all 64
releases on one seat; that seat records **0** opponent-turn ask/priority windows against its
counterpart's 70), but the aggregate correlation runs the OTHER way — a release presupposes an
offered window, and the four zero-release games have zero opponent-turn windows too. The defensible
corpus-level number is the SHARE: opponent-turn windows are 19.9% of decisions against 30.2% last
corpus, a collapse the 36% general shrinkage does not explain.
**Status: a hotfix lane is already rewriting the floor** to wall clock + never-while-pending +
`WAGIC_STALL_FLOOR=0`. Not re-designed here.
**The A/B that settles it.** `WAGIC_STALL_FLOOR=0` on the hotfixed binary, one variable, no rebuild
of the corpus recipe. `--realtime` (`FASTCLOCK=0`) is the fallback and is NOT preferred: it changes
game pacing wholesale and costs ~20 wall-minutes a game.
**Wave-55 prediction (this is the discharge).** `interrupt window held … releasing` on a GPT seat:
**0/N**; `dropping stale async answer` **< 80 corpus-wide** (was 488; wave 53: 63); seats recording
zero opponent-turn ask/priority windows **<= 12 of 42** (was 25 on this step's predicate);
opponent-turn share of decisions **> 26%** (was 19.9%); recorded inference per decision **< 24 s**
AND uncounted dropped-call inference **< 0.5 h**. **Falsifier:** drops stay >= 300 with the floor
disarmed — then the cause is a prompt rebuild and `WAGIC_GPT_AUDIT_M_OFF=1` is the next flag.
**Also owed:** lane AA's disable flag exists from this wave on (the standing process finding above).

## D2 — HIGH — a mandatory life-loss loop opens one full LLM window per iteration, and the de-duplication key cannot see it because the row's own pricing annotation moves with the life total: 69 dead model calls in two games, 40 in one turn
(= **R194**; seat-123-130 **E1 HIGH**, deck126 **G8**, deck130 general-suggestions)
**Repro, re-derived on disk.** `130v126` seq **67-106** — **40 consecutive decision records**, all
turn 18, all Main phase 1, life 20/25 -> 0/44 one point at a time, stack line
`1 (top): ability: Sanguine Bond's Life Loss [from their Sanguine Bond]`, menu two rows. **21
distinct `options_text` tuples across the 40**, because row 1 reads `Cast Spark Spray {r} {leaves 0
of your 1 untapped mana source untapped - casting this taps you out} {no creature target - and 1 to
the opponent at life 26 leaves them at 25}` and that number ticks every iteration.
`identical_option_asks_resolved` on that gameend: **0**. `hold_windows_skipped`: **0**. Mirror:
`123v126` seq **112-140** (29 records, the other side of the same game); the HOLD row was taken six
times inside it (s117, s119, s120, s127, s129, s136) and stopped nothing, because "any change
re-opens this window" is literally satisfied by a life tick.
Primitives (verified): Sanguine Bond `mtg.txt:99004` `auto=@lifeof(player) from(*[-lifefaker]|*):
life:-thatmuch opponent`; Exquisite Blood `mtg.txt:37966`
`auto=@lifelostfoeof(player):life:thatmuch controller` — a closed loop.
**Ask, two parts, neither a cap.** (a) Make the repeat/de-dup key the option SET — row names, costs
and ordinals — not the rendered string, so a numeric annotation that only tracks a moving life total
cannot defeat it. (b) Give the HOLD row's own key the same treatment: **the model asked not to be
asked again this turn, and a life total moving inside a mandatory loop is not the board change it
meant.** (b) honours a choice the model made by pressing the row; it withholds nothing from a seat
that has not pressed it, which is why the blanket "cap re-opens within one phase" version of this
proposal is rejected in `general-strategy.md`.
**The annotation is CORRECT and must not be trimmed** — only the key changes.
**Wave-55 prediction.** Decisions inside a single-phase mandatory life-loss loop: **< 5 per loop**
(was 40 and 29); `identical_option_asks_resolved > 0` on any gameend carrying such a loop;
identical-declined runs **< 12%** of decisions and max run length **< 20** (this corpus: 16.1% /
17.9% by predicate, max 50).

## D3 — HIGH — `all_assignments_illegal` declares NO blocks at a lethal window and does not re-ask: a new fallback class, its first firing, and it cost the game
(= **R195**; engine seat **HIGH #3**, seat-146-152-162 **E-6/E-7**, deck152 **R-5/R-6**; skill #225)
**Repro.** `152v146` seq **58**, turn 19, phase Blockers, **my_life 6 / opp_life 7**, latency
2,483 ms. Header: `Your life: 6. Unblocked, these attackers deal up to 6 - you would be at 0 -
LETHAL if it all connects (at 0 life you LOSE - 0 is not survival): block enough to survive.` Both
Spider attackers carry `[menace - only a block by TWO OR MORE of your creatures counts; one creature
alone does not block it at all]`, and each blocker row repeats it per attacker
(`A3 (no 1-on-1 result exists - …)`). Reply `BLOCKS: B1:A3, B2:A4`. Record:
`pruned_pairs: "Elite Spellbinder -> Spider (needs 2 blockers, only 1 assigned); Intrepid Adversary
-> Spider (needs 2 blockers, only 1 assigned)"`, `fallback: all_assignments_illegal`, `choice: 2`,
`chosen_text: None` — **no blocks declared, no follower re-ask, no `recovery` record**. Life 6 -> 2
at seq 59 and the seat lost the game.
**Why this is the loop's stated purpose inverted.** Legality is enforced structurally, but enforcing
it by discarding the whole answer is a constraint on choice by omission. `named_row_reask` re-asks
once when the model names an off-menu row (7 firings, 5 recovered, 2 exhausted); the blockers path
has no equivalent arm, and `rejectedSoFar` already exists on the cast path.
**Ask.** (a) One re-ask carrying the pruned pairs as text; a second all-illegal reply may still
declare no blocks and is stamped `all_assignments_illegal_exhausted`. (b) **D14** is the record half.
**Not an engine defect on the same record:** the model was told three times, in three places, that a
lone blocker does not block menace. That half is the deck seat's (152-D) and skill **#225**.
**Wave-55 prediction.** `all_assignments_illegal` records with no follower re-ask: **0/N** (was 1/1).

## D4 — HIGH — the HOLD row is excluded from the seat's OWN turn, and the corpus's longest dead run is 50 byte-identical own-turn windows with no HOLD row on the menu
(= **R196**; engine seat **HIGH #2**)
**Repro.** `126v125` seq **72-121**, turn 28, phase **Blockers**: 50 consecutive decisions on one
two-row menu (`1. Cast Tribute to Hunger {2}{b} {right now: they control 0 creatures - at 0 this
does nothing}` / `2. Cast nothing right now`), every reply `CHOICE: 2`, latency sum **242.8 s**, and
the last prompt reads `[you declined this exact list 49 times already this turn]`. The HOLD row is
absent because `observer->currentPlayer == this` (`AIPlayerGPT.cpp:18305-18311`, `20391-20397`).
**The exclusion's own rationale is refuted by this run.** `AIPlayerGPT.cpp:18296-18304` reasons that
"on the seat's OWN turn the board changes with every land drop and cast, so a hold there would be
taken and retired in the same breath." After the main phase is spent, the seat's own Blockers /
Combat-damage sequence produces 50 byte-identical windows with no board change at all — and
`holdBoardKeyOf` already guarantees retirement on any real change, so the guarantee needs no
weakening.
**Distribution, re-derived (key = `(turn, options_text)` on a declined `chosen_text`, runs >= 3):**
**356 / 2,212 = 16.1%** with a HOLD take counted as a non-decline, **396 = 17.9%** counted as a
decline; **47 runs, max 50**. Lane A's `< 15%` target FAILS on either. Second-largest runs are all
Upkeep in `123v125` (15, 13, 12, 12) and those DO carry the row, which the pilot took repeatedly —
**the mechanism works where it is offered.** **The harvest's 12.2% is not reproducible**: two
independent measurements got 16.1% / 17.9% and neither recovered it (#208).
**Ask.** Offer the HOLD row on own-turn windows once the land drop is spent and the seat is past
Main 1 — or simply always.
**Wave-55 prediction.** Identical-declined runs **< 12%** and max run length **< 20**.
**Falsifier:** the row is offered on own-turn windows and the 50-run class does not shrink — then
the row is not the remedy and D2's key is the whole answer.

## D5 — HIGH — a reveal chooser LIVELOCKS: the prompt is rebuilt on an unchanged board, six consecutive stale drops hand a real decision to the heuristic, and lane F's force-close does not cover the shape
(= **R197**; engine seat **HIGH #4**, seat-146-152-162 **E-2**)
**Repro.** `146v123` seq **15** (turn 9, Main phase 1, `kind: reveal`, 4 rows — Lightning Greaves /
Marsh Flats / Thraben Doomsayer / Damnation — `choice: -1`, `latency_ms: -1`,
`fallback: stale_livelock`, empty reply) with `recovery` at s16.
`game-146v123-1788425054.stderr:1335-1343`: **six `AIPlayerGPT: dropping stale async answer` back to
back with NO release line, no phase step and no stack action between them**, then
`AIPlayerGPT: 6 consecutive stale drops - giving this decision to the heuristic`, then
`Action added to stack: StackAbility. (Source: Pelakka Predation)`. That game carries **6 drops and
0 releases** — which is exactly what separates it from D1: the breaker counts drops only on the SAME
`serializeGameState()`, so **the board did not move; the prompt did.**
**The card (primitive, verified).** Pelakka Predation: `auto=name(reveal hand)
reveal:type:*:opponenthand revealzone(opponenthand) optionone name(Choose a card)
target(*[manacost>=3]|reveal) reject optiononeend optiontwo name(put back) all(*|reveal)
moveto(hand) optiontwoend revealend`. Option one is predicate-gated — the wave-53 hang's family —
but here it had LIVE targets (Thraben Doomsayer mv 3, Damnation mv 4) and an option two. The
recovery record shows option TWO ran: `- Opponent put the revealed Lightning Greaves into their
hand` — **nothing was discarded and the Predation did nothing.**
**Lane F's guard has no window on it**: its budget arms on "option one predicate-gated to zero
targets with NO option two", and `force-close` renders **0** corpus-wide (**UNTESTED, N = 0**, not
PASS). On the same path, `game-146v123:1250-1334` is 85 lines of the reveal display cycling
`AttemptNew failed to load (404) -> Destroying WCachedResource -> Can't locate` over the same four
card images (**D26**).
**Ask.** (a) Extend the force-close budget to ANY interactive reveal whose driver makes no progress,
not only the no-option-two shape, and write a `reveal_stall` field. (b) Localise the per-tick prompt
instability in the reveal menu build — the wave-52 deterministic-order fix does not cover
`revealzone(opponenthand)`. Flag order for the A/B: `WAGIC_GPT_AUDIT_M_OFF=1` (lane M's situation
prefill is the only wave-54 change that rewrites prompt text per tick), then
`WAGIC_GPT_BOARDINDEX=0`, then `WAGIC_GPT_SCRIPTMEMO=0`.
**Wave-55 prediction.** `stale_livelock` on a `reveal` record: **0**; any reveal driver making no
progress writes a `reveal_stall` field; `force-close` gets its first render or is reported UNTESTED
again with the window named.

## D6 — HIGH — the X menu's decisive marker rendered ZERO times in 42 logs, a rung was scored PASS twice on it, and the unmarked menu lost a game
(= **R198**; seat-123-130 **E3**, deck130 **G6/G7**; skill #221)
**Evidence.** `[<- most kills at any affordable X that costs you nothing]`: **0 renders across all
42 `.jsonl` files**, re-derived by this step. It prints only when some affordable X kills something
of theirs and nothing of yours, which held on neither X menu in the corpus. Wave 53 recorded the
rung "taken 2/2" and predicted N/N again.
**Repro.** `130v152` s17, 9 life against 25, largest-first per the owner ruling:
`1. X = 4 {kills THEIRS: Intrepid Adversary, Elite Spellbinder, Sigarda, Champion of Light; YOURS:
Siege-Gang Commander, Goblin x3}` / `2. X = 3 {kills THEIRS: Intrepid Adversary, Elite Spellbinder;
YOURS: …}` / `3. X = 2 {same kills as X=3, for 1 less mana}` / **`4. X = 1 {kills THEIRS: none;
YOURS: Goblin x3}`** / `5. X = 0`. **Answered 4**, under `PLAN: Kill Sigarda and Elite Spellbinder
with Starstorm X=1` — a sentence the row it named denies in its own words. It swept its own three
Goblins, killed nothing of theirs, and the game ended three life later. Starstorm is
`damage:X all(creature)` (`mtg.txt:112835`) — creatures only, so the render was true throughout.
**Second half, same menu.** Row 3 — the row the guide's STEP 2 wants — is the ONLY row with no kill
list, because the `same kills as X=N` collapse removed it. `130v126` s58 took the TOP of a collapsed
run for the third corpus running.
**Ask.** (a) Always mark exactly ONE row and say what the mark means: keep the current marker when
some X costs nothing, otherwise `[<- best trade: the most of THEIRS at the smallest cost to YOURS]`
on the cheapest row whose `kills THEIRS` list is maximal. (b) Put the mark on the BOTTOM row of a
collapsed run, or restate the kill list on every row. Neither removes a row; largest-first stands.
**Wave-55 prediction.** X menus rendering with no marker: **0** (was 2 of 2). Takes of a
`kills THEIRS: none` row while another affordable row names an opponent creature: **0** (was 1).
Takes of the TOP row of a collapsed `same kills as X=N` run: **0** (was 1, third corpus).

## D7 — HIGH — a `target=player` row enumerates BOTH players and prices only one: 178 of 180 Devour Flesh rows print `they control 0 creatures - at 0 this does nothing` while listing `you` as a legal target
(= **R199**; engine seat **D-2 HIGH**)
**Evidence, re-derived.** **180** `Cast Devour Flesh` rows carry a `legal targets right now:` list;
**178** enumerate `you`; **178** carry a `{right now: they …}` verdict that speaks only of the
opponent; **0 takes**. Emitter string, `123v125` seq 8 row 3: `Cast Devour Flesh {1}{b} {right now:
they control 0 creatures - at 0 this does nothing} {leaves 1 of your 3 untapped mana sources
untapped} - legal targets right now: the opponent, you {card text: "Target player sacrifices a
creature, then gains life equal to that creature's toughness."} …`.
**Primitive (verified).** `name=Devour Flesh / target=player / auto=ability$!name(sacrifice)
notaTarget(creature|mybattlefield) dynamicability<!mytgt toughnesslifegain targetcontroller!>
sacrifice!$ targetedplayer` — the **TARGETED** player sacrifices, so self-targeting is a real and
frequently relevant play and the verdict is FALSE for half the row's own enumerated target list.
**Cost, measured.** `123v126` seq **104** (turn 19, Combat begins, 58 Humans on board, Sanguine Bond
+ Exquisite Blood live): the reply spends ~900 words asking whether taking the row lets it pick a
target at all — *"It doesn't specify target yet… If I cannot target myself with this menu item, I
cannot trigger the loop."* — then abandons a genuine win attempt and takes the HOLD row.
**Ask.** (a) Per-player verdict when a `target=player` row enumerates both:
`{right now: they control 0 creatures - at 0 this does nothing; YOU control 58 - you would sacrifice
one and gain its toughness}`. (b) State whether the row COMMITS a target or opens a follow-up ask —
the model asked this explicitly and the prompt answers it nowhere.
**Wave-55 prediction.** Rows enumerating both players whose `{right now:}` verdict names only one:
**0/N** (was 178). A take of the self-target is NOT predicted — that is a deck-seat call.

## D8 — MED — the one-source × N-identical-target ability menu is uncollapsed and is now the corpus's largest render waste: 46 records, 403 redundant rows, ~160,800 chars
(= **R200**; engine seat **D-3**; lane D's explicitly-not-shipped 2-D collapse; R2/D8-wave-53 residual)
Re-derived (family key = row text with the ordinal masked; redundant = family size − 1; families of
>= 3 rows): **46 ask/priority records, 403 redundant rows, 160,803 characters**. Families:
`Equip with Lightning Greaves targeting Human #N` **190**, `… Vampire #N` **93**, `Mountain #N` 82,
`Deal 2 damage with Siege-Gang Commander targeting Goblin #N` 30, `Human #N` 29,
`Deal 2 damage with Pyrite Spellbomb targeting Goblin #N` 12, `Destroy with Dwarven Blastminer #N` 8.
Repro: `123v162` seq **26** (turn 10, priority, 23 rows, 9,207 chars) — every row
`Equip with Lightning Greaves targeting Vampire #N [your battlefield] (Lightning Greaves is ALREADY
attached to Bloodline Keeper - this MOVES it to Vampire #N …)` on 23 identical 2/2 tokens, answered
`pass`; same shape at s23/s24/s34 and `123v146` s22-s25. Adjacent: `123v126` seq **88** is a 58-row
attackers list and seq **106** a 61-row blockers list of interchangeable `Human #N` tokens.
Wave 53's 28-row Staff of Nin exhibit is the same class.
**Ask.** Extend lane P's `groupNumberedRows` permutation (proven index-safe with its map-back) to
ability, sacrifice and library-reveal menus, in the two-dimensional form (source × target). **The
answer is a COLLAPSE (`Human #1-#58 x58, all identical`), never a cap.**
**Wave-55 prediction.** Ask/priority menus containing more than 3 rows of one `#N` shape: **0**;
redundant row chars **< 20,000** (was ~160,800); menus over 40 rows survive only for
attackers/blockers and carry an `x N, all identical` line.

## D9 — MED — the `discard` ask is the only ask kind whose rows carry no verdict at all, and it is the seam one seat lost two games on
(= **R201**; deck125 **G1**, seat-125-126 **HIGH #2**)
`125v162` seq **47** (t17, **8 life**, `kind: discard`, send 2): the nine options are bare card names
— `['Fall of the Gavel','Lightmine Field','Final Judgment','Dream Fracture','Path to Exile','Fall of
the Gavel','Dream Fracture','Island','Path to Exile']` — with **no annotation on any row**, while
the same prompt already prints `Your battlefield (9 permanents … and 9 are lands)` and the
opponent's `0 are creatures`. It sent both Fall of the Gavel, its only answers to a noncreature, and
died two turns later. Same shape at `125v126` s73 (both Gavels sent on a ten-land board, the turn
before an uncountered Sanguine Bond resolved) and s80. Every other ask kind annotates.
**Ask, three clauses the emitter already computes elsewhere.** `{spare: you control N lands}` on a
land row (N from the header); `{dead right now: they control 0 creatures}` on a row whose card gates
on the opponent's creature count — the same string family this seat declines 40 of 40 times on cast
rows; `{you already control one}` on a row naming a card on its own battlefield line.
**Wave-55 prediction.** `discard` rows carrying no annotation of any kind: **0**. Cleanup discards
sending a counterspell while a land is on the numbered list and the header reads 6+ lands: **0**
(was 3 of 4).

## D10 — MED — a cast row whose price is set by a tag on the OPPONENT's battlefield line does not carry that price
(= **R202**; deck126 **G5**, seat-125-126 STRATEGY #2; skill #219)
`126v146` seq **42** (t26, 24 -> 21) and seq **61** (t32, **8 -> 5**, opponent on 5, four attackers
on their board): the row reads `Cast Exquisite Blood {4}{b} {leaves 3 of your 8 untapped mana
sources untapped} {card text: …}` with no mention of the 3 life, while the opponent battlefield line
reads `Silverquill Silencer {b}{w} (3/2) [named: Exquisite Blood]` and the events read `- You cast
Exquisite Blood | - You lost 3 life (now 21) | - Opponent drew a card`. The seat died at −5 against
5 having paid 6 to the Silencer. Magnitude and draw are both readable from the ability, not guessed:
`@movedto(*[chosenname]|opponentstack):life:-3 opponent && draw:1 controller`
(`borderline.txt:102764`).
**Ask.** `[NAMED BY THEIR <source>: casting this costs you 3 life and draws them a card - you would
be at K]` on the cast row, reusing lane C's `- you would be at K` subtraction.
**Bound on the item:** the TAG itself is the best-shaped new render in the corpus (54 records, the
#193 dedicated-line device) and **nothing about it should change** — two seats asked for that in
writing. Only the row's price is missing.
**Wave-55 prediction.** Casts of a card named in an opponent `[named: <name>]` tag with no price on
the row: **0** (was 2 of 2 at the seat it cost).

## D11 — MED — the reworded HOLD row shares its first two words with row 0, and the parser breaks the tie on the index: 16 `pass_hold_ambiguous` stamps, 13 false and 3 genuine
(= **R203**; seat-146-152-162 **E-5**, engine seat, seat-125-126 **LOW #5**, deck146 **R-1**,
deck125 **G4**)
**The reviewer brief's corpus note is CORRECTED here**, and two seats found the same correction
independently. **13 stamps** are `CHOICE: 0 (pass)` executing row 0 correctly — false positives,
because `pass` is an EXACT match for row 0's own label and only a case-insensitive PREFIX of `Pass
priority, and do not ask me again…`, and the matcher has no exact-match tiebreak (`152v125` 29/30/
45/46, `125v162` 21/23, `125v146` 20/21, `146v126` 55, `123v126` 72/74/101/110). **3 are genuine**:
`146v126` seq **54, 56, 57**, all `CHOICE: 3 (Pass priority)` on a 3-row menu whose row 3 IS the
HOLD row, all executed row 3, all counted among the 43 HOLD takes.
**Ask, two independent halves.** (a) Parser: an EXACT match against a row label beats a prefix match
— kills 13 of 16 with no behaviour change. (b) Prompt: make the two rows' leading words differ,
which also makes them distinguishable to the PILOT, or key the HOLD row's name-match on its tail
clause. The reword is lane A's call.
**Wave-55 prediction.** `pass_hold_ambiguous` **<= 4**, all of the `CHOICE: n (Pass priority)`-on-
the-HOLD-row shape; `CHOICE: 0 (pass)` never stamps again.

## D12 — MED — the mulligan coverage clause's two cause families call for OPPOSITE decisions and are rendered in one grammar
(= **R204**; seat-146-152-162 **E-4**, deck146 **R-2**; skill #210)
11 renders, **11 with a cause** (lane E PASSES), in two families: `(1 land is not enough for your
cheapest spell at mana value 2)` — a genuine count failure — and `(you have 3 lands; no spell in it
is castable off {W}{U} alone)` — a TURN-ONE colour statement about a hand three rungs above any
floor. `152v146` s1 shipped a THREE-land seven under the second, with the reply naming a colour as
its reason, for the fourth corpus that floor has broken on the same shape and the first in which the
trigger literal itself moved. Every word is true and the trust doctrine says the pilot must believe
it; the defect is that a turn-one statement is rendered in the grammar of a hand-level verdict,
directly above a Keep/Mulligan ask.
**Ask.** Append the scope the clause already implies — `… castable off {W}{U} alone **on turn one**`
— or split the render into the two families explicitly. The count family needs no change.
**Wave-55 prediction.** Mulligans taken under a COLOUR-family coverage clause at 2+ lands: **0**.

## D13 — MED — the blockers ask has no count header for the constraint that voided a whole answer
(= **R205**; deck152 **R-5**, seat-146-152-162 **E-6**)
Lane E's attackers header renders **99 of 99**; the blockers prompt has no equivalent for menace
arithmetic. At `152v146` s58 the menace rule was stated three times per blocker and the seat still
spread one blocker across two menace attackers. **Ask:** one header above the B-rows, on D17's
pattern — `2 of the 4 attackers need TWO or more blockers each; you have 2 blockers.` A count claim
over rows the engine already computes, in the seat's own reading order, visible BEFORE the answer is
written rather than after it is discarded. Pairs with D3; neither substitutes for the other.

## D14 — MED — the record for a discarded blockers answer does not surface the offending pair where the ledger reads it
(= **R206**; deck152 **R-6**)
The `all_assignments_illegal` fallback carries the class name; `pruned_pairs` IS on the record but
the fallback class does not name the cause the way `plan_choice_conflict` was made readable in wave
53, and the reviewer had to re-derive the menace rule from the prompt. **Ask:** carry the pairs and
the reason with the class (`all_assignments_illegal: B1:A3 (menace, 1 blocker), B2:A4 (menace, 1
blocker)`). One field; no new computation.

## D15 — MED — a single-target `{kills:}` list names the caster's own permanents under a consequence label, with no ownership mark
(= **R207**; deck130 **G8**, seat-123-130 **E6**)
`130v146` s23: `Cast Stone Rain {2}{r} {kills: Plains #1, Plains #2, Mountain #1, Mountain #2,
Mountain #3, Mountain #4, Mountain #5}` — five of the seven are the pilot's own Mountains, and the
row destroys exactly ONE. The target menu one screen later (s24) marks ownership on every row
(`Mountain #1 [land] [your battlefield] [tapped]`); the cast row does not. This is D4-wave-53's
land-destruction sibling and it is a live risk on the deck whose whole plan is land destruction.
**Ask.** `{kills whichever ONE you target: theirs - Plains #1, Plains #2; yours - Mountain #1-#5}`,
or name only the opponent's permanents and state the count of your own.
**Wave-55 prediction.** `{kills:` lists on a single-target destroy row with no ownership mark: **0**.

## D16 — MED — ability rows that point damage at a player still print no life total
(= **R208**; wave-53 D28 carried; deck130 **G9**, seat-123-130 **E9**)
Cast rows have carried the tail since lane C (`130v126` s66:
`- and 1 to the opponent at life 24 leaves them at 23`); ABILITY rows have not. `130v162` s25:
`Deal 2 damage with Pyrite Spellbomb targeting Ob Nixilis, the Hate-Twisted` beside a menu whose
player row reads `the opponent (player, life 12)`. Same clause, other emitter. Third wave carried.

## D17 — MED — fetch rows name a land and never its colours
(= **R209**; wave-53 D20 carried; deck123 **G4**, seat-123-130 **E7**)
21 renders at deck123 of `{this land makes no mana - crack it for a land: your untapped mana sources
go from N to N+1}` with no colour on any row. Behaviour improved anyway (4 of 5 fetches took a dual,
was 5 of 8 last corpus), so this is now correctness rather than bleeding. **Ask:** continue the
clause with the colours in the mana line's own words — `…, and it adds {W} or {U} (you cannot make
{U} right now)`. The emitter already prints `(land: taps for {W}{U})` on hand lines.

## D18 — MED — library-reveal menus do not collapse duplicates
(= **R210**; deck123 **G3**, seat-123-130 **E8**)
`123v125` s9 lists **50** rows with `Intruder Alarm` ×4, `Damnation` ×4, `Thraben Doomsayer` ×4 as
separate identical strings; `123v126` s23 lists 45 the same way. The battlefield line directly above
already collapses identical permanents with `xN` handles. Same `groupNumberedRows` fix as D8,
different seam; also the surface `146v123` s15's excellent per-row qualify tags live on.

## D19 — MED — the declined-count annotation does not reach the seat's own same-turn Main 1 / Main 2 pair
(= **R211**; deck162 **R-8**)
`162v126` s7-s12 and `162v146` s7-s11: the identical two-row ask re-issued at Main 1 and Main 2 of
one turn, then again next turn, unchanged board, unchanged answer.
`[you declined this exact list N times already this turn]` appears on **4 of that seat's 630
decisions** (327 corpus-wide). These are own-turn windows, so the HOLD row does not render and lane
A's suppression cannot reach them. **Ask: the ANNOTATION only.** The seat explicitly did not ask for
suppression and the suppression version would be a cap. Pairs with D4.

## D20 — MED — the `{feeds:` converter count counts a conditional converter unconditionally
(= **R212**; deck162 **R-10**)
The row prints `converters on your battlefield: K - <names>` counting Liliana's Caress, but a lone
Caress converts only DISCARDS, so the guide carries a hand-written qualifier ("K counts only while
the Opponent hand size line reads 7 or more"). **Ask:** have the row do the arithmetic —
`converters on your battlefield: 1 - Liliana's Caress (discards only; their hand is 5, so this
converts nothing yet)`. Same class as the D4/D5/D10 work of wave 53, and it would delete a paragraph
from the pool's smallest guide. **No window this corpus turned on it** — filed as an ask, not a
defect.

## D21 — MED — the HOLD row states its RISK and never its BENEFIT
(= **R213**; deck125 **G3**)
426 renders / 43 takes (10.1%, was 2.5%), and in windows already carrying the declined-note at
N >= 3, **22 of 212 = 10.4%** — a 25× improvement on 0.4% and half of lane A's >= 20% target. The
row's text is accurate and reassuring about the RISK (`any change re-opens this window; you give up
no cast`) and says nothing about what taking it SAVES, so a pilot unworried about the risk has no
reason to press it. **Ask:** append the count the engine already tracks and already prints elsewhere
in the same prompt — `(you have declined this same list N times this turn; taking this row skips the
rest of their turn's identical windows)`. A re-siting of an existing literal, measurable against
`hold_windows_skipped`, which now works.
**Wave-55 prediction.** HOLD takes in declined-note N >= 3 windows **>= 20%** (was 10.4%).

## D22 — MED — the `chooseaname` menu is headed as a MODE menu and carries no annotation, 9/9
(= **R214**; carried from lane C, confirmed independently by two seats)
The header renders **9** times, verbatim `Choose one mode for Silverquill Silencer:`, followed by
ten to fourteen rows of bare card names — no mana, no board effect, no card text, no dead marker.
Primitive: `auto=chooseanameopp transforms((,newability[@movedto(*[chosenname]|opponentstack):
life:-3 opponent && draw:1 controller])) forever chooseend nonland`,
`text=As Silverquill Silencer enters, choose a nonland card name.` **It is a card-NAME menu, not a
mode menu, and the header names the wrong thing.** The OBSERVING seat's surface (`[named: <card>]`)
renders correctly, so the defect is scoped to the ACTOR's menu — **fix both halves as one item** so
the same fact is not described two ways on the two seats. **Ask:** correct the header and annotate
the rows with what naming that card would do.

## D23 — LOW — a 900 s wall miss with no translog record at all
(= **R215**; engine seat **D-8**)
`no reply after 900s - one retry` appears twice. One produced a full record (`123v125` s1,
`retry: 1`, `deadline_pct: 165.4`, `long_reply: 1`, `latency_ms 1,488,661`, answered on the retry —
lane Q's arm working, **first live firing**). The other (`game-146v123-1788425054.stderr:1607`) was
retried and the window then auto-passed (`only display-toggle (Flip Side) options; auto-passing
without a model call`) with **no decision record written**. **Ask:** a `wall_miss_unrecorded`
counter on the gameend record, or write a zero-choice record.
**Wave-55 prediction.** `no reply after 900s` events each produce exactly one decision record
(this corpus: 2 events, 1 record).

## D24 — LOW — the `(xN)` log collapse still misses phase lines and draws
(= **R216**; wave-53 D26 carried)
**471** duplicate event lines inside single records (was 571 — improving): `- Phase: Draw` 217,
`- Opponent drew a card` 89, `- Phase: Main phase N` 79, token-ceased lines 9, `- Phase: Combat
damage` 5, loyalty-counter lines 3. The wave-53 verdict declining the ADJACENT collapse stands and
is not re-opened; this is the within-record class, and it is free space in a log that is ~56% of the
prompt.

## D25 — LOW — receipts without a payee: 3
(= **R217**; wave-53 D27 carried)
`152v125` seq 36 (`- Paid {1}{w} with Hengegate Pathway; Barkchannel Pathway #1` ×3, from
`- You chose add 19 counters for Intrepid Adversary`), the same pay-any-number-of-times ETB class.
Corpus-wide `- Paid` **369 receipt lines across records' `events`**, **0 with no source tapped** in
all 21 stderr files. The no-payee class runs 7 -> 3 -> 2 -> **3** across four waves.

## D26 — LOW — repeated resource 404s, and the resource-miss memo does not cover them
(= **R218**; engine seat **D-9**; lane J watch item)
**1,927** `AttemptNew failed to load (404)` lines over **118 distinct** resources; `mana.wav` 105×,
`land.wav` 105×, `creature.wav` 96×. The reveal display in `game-146v123:1250-1334` re-cycles the
same four card images ~20 times in 85 lines with `Destroying WCachedResource` between them — lane J's
memo is not reaching the audio loader or surviving the destroy. **Flag for the A/B:
`WAGIC_RENDER_NOCACHE=1`.** Desktop/Vita visual parity was eyeballed by nobody, including this step.

## D27 — LOW — the effect label still falls back to a generic when the ability has no name
(= **R219**; wave-53 D19's residual)
**401** `ability:` stack-line occurrences over 51 distinct forms, all correctly
`ability: <source>'s <effect> [from your/their <source>]`. **9** read `ability: Ob Nixilis, the
Hate-Twisted's triggered ability` — the name-twice bug is FIXED and the generic fallback survives.
Cosmetic.

## D28 — MEASUREMENT / DECISION — the log window: the trim WORKED and the cost story moved to inference — OWNER ITEM
(= **R224**; wave-53 D32 continued)
Prompts over 30,000: **2** (was 12). Over 25,000: **37** (was 588). `ask` mean **10,269** (was
14,245, −28%); max prompt **32,215** (`146v126` s86, priority, turn 31). Log share ~56% of prompts
carrying a CURRENT SITUATION marker, **80.6%** on the log-cost specimen. Lane L's 24 KB trim v2 is
the visible win and the tail is no longer the growth story. **What replaced it: recorded inference
per decision went UP — 23.9 s against 21.7 s, +10.2%, on a corpus whose mean prompt is 28% smaller —
and the 488 dropped calls are not in that total at all** (D1, ≈3.2 h uncounted, estimated).
Log compaction remains the owner's call; nothing here proposes one.

---

# CARRIED ENGINE ITEMS WITH NO CORPUS SURFACE (from the wave-54 docket, re-measured only)

## D29 — MED — replay is unusable on human-vs-Baka dumps, and it blocks every owner play report
(wave-54 D34) Four independent failures carried unchanged: the RNG-stream split (owner said YES,
after this corpus), the `read the bones` chooser-order divergence, lane W's two vpk12 dumps (lane I
re-checked: `exile=` was not their cause), and the equip auto-tap replay blocker (lane AA: the engine
auto-taps, then refuses the recorded target click). **This is the item that compounds** — every owner
play report is still investigated from a transcript plus a fixture rather than from the board that
produced it.

## D30 — MED — `kMaxOptSources` is 14 and the owner's option-preserving autotap is a no-op above it
(wave-54 D35) Unchanged; O4 is the named measurement lane. Not corpus-testable (human seat only).

## D31 — MED — `planPayment` / `potentialMana` / `selectAutoTapProducers` cannot see a `&&`-wrapped mana producer
(wave-54 D36) Unchanged; one call site fixed, the residual needs its own pin. Not corpus-testable.

## D32 — LOW — the Dominating Vampire livelock candidate, unreproduced
(wave-54 D37) Unchanged and stated as a hypothesis. Dominating Vampire is not in the pool, so this
is not corpus-testable; the next step that settles it is the per-tick instrumented run.

## D33 — LOW — `GuiPlay::receiveEventPlus` calls `Replace()` unconditionally per ETB
(wave-54 D33's second half) An O(n) play-zone relayout per token created; not measured on the
console. Frontend/human seat, the owner's Arena-grouping item, carried.

## D34 — MED (frontend / human seat) — Arena-style grouping + the `CardGui::Render` per-card cost
(wave-54 D33) Carried as the owner's, measured not redesigned. Lane W's bounded part shipped
(zone renders 10.0 -> 0.6 per frame, whole-frame Render 3.03 -> 1.86 ms) and every number is desktop;
**the console is the test and vpk13's telemetry is what decides it.**

## D35 — MED — the audit lanes' central watch items are not observable from a corpus, and one is unmeasurable by construction
Three seats independently reported **UNTESTED, not clean**, for lanes G/H/I/N (no RSS, no ASAN, no
per-tick verdict trace, no Vita exposure), and lane **M's cache-hit counters reach no log at all**,
so its own stated watch item ("cache hits per game with turn-boundary clears") could not be scored by
anybody. **Ask:** a `cache_hits` field on the `gameend` record. Note that D1 means any hit rate
measured on THIS corpus is contaminated by re-asks.

---

# DISCHARGED — SET B: the wave-54 docket D1-D38 and lanes A-N + AA, by the emitter's actual string

| item | lane / commit | verdict | counts (re-derived) |
|---|---|---|---|
| **D1** the reveal hang / 21/21 natural | X (wave 53), verified here | **SHIPPED — PASS. DISCHARGED** | 42 `gameend`, 0 seat logs without one, 0 hangs, 0 timeouts, 0 adjudications; the 4 Sigarda Coven reveals all resolved. Corpus wall time 1 h 45 min |
| **D2(a)** HOLD index-0 degradation | A e21f81de5 | **SHIPPED — PASS 16/16 stamped** | old row string **0 renders**, new **426**; `hold_row_named` 0; no HOLD take mis-executed. The shape now arrives as `CHOICE: 0 (pass)` / `CHOICE: 3 (Pass priority)` and all 16 carry `pass_hold_ambiguous` — 13 false, 3 true — **D11** |
| **D2(b)** the plan-conflict collision | A | **SHIPPED — PASS 0/12 (was 5)** | every `plan_choice_conflict` record's executed row checked against `options_text[choice-1]`; **none is the HOLD row**. CLOSED |
| **D2(c)** the hold key's hidden-zone counters | A | **SHIPPED — PASS 7/8 gameends, the 8th vacuous** | 8 seats took the row; 7 carry `hold_windows_skipped > 0` (31, 16, 9, 5, 5, 3, 3; sum **72**, was 102 on a much larger denominator, non-zero on 7 of 42 vs 2 of 40). The exception is `123v130` s17, a **Cleanup** take with no window left in the turn. No seat has skips without a take. `hold re-opened` 21 |
| **D2(d)** the reword | A | **SHIPPED — PASS 426/426, takes 4× the rate** | LAST row 426/426, opponent's turn only, one distinct string, by kind ask 403 / priority 23. **43 takes = 10.1%** (was 29 = 2.5%). Residuals: **D4**, **D11**, **D21** |
| **D2** economy predictions (opp-turn casts >= 90% of 58; identical-declined < 15%) | A | **UNTESTED (denominator moved) / FAIL** | opponent-turn ask/priority windows **441** (was 1,052), casts **26** — the ABSOLUTE target is not scoreable and the RATE went 5.5% -> 5.9%: **UNTESTED**, per #217. Identical-declined runs **16.1% / 17.9%** by predicate: **FAIL** on either, and NOT denominator-driven (its largest run is an own-turn window the row never covers) — **D4** |
| **D12** PLAN shape bound + stale-plan diff | A | **SHIPPED — PASS** | served PLAN blocks > one paragraph **0 of 2,036**; age stamp **2,036/2,036**; max `plan_echo_count` 37 (was 43); the `"<card>" is no longer on your menu` clause renders in **357** prompts. Honest limit: the ground-truth stale-plan set is not enumerable from the log, so the second half is a render count, not a completeness proof. Wave-53 H3's "unbounded capture" is **CLOSED** |
| **D13** the latch mismatch stamp | B 351ad0098 | **SHIPPED — PASS 2/2, 0 FAILs** | both firings adjudicated record-by-record and both are TRUE positives (`123v125` s147, `146v152` s63). **0 stamps on a legitimate name-over-index remap.** Genuine mis-executions corpus-wide **0** (was 2) over 1,828 parentheticals — the ≤1/3,000 carry met with room. One seat filed s63 as a telemetry defect; **refuted on disk** (skill #230) |
| **D14** `plan_contradicts_noop_row` | B | **SHIPPED — UNTESTED (0 renders)** | 0 / 0 / 0. All 12 `decision_reversed_in_prose` records take a LIVE row, so the arm had no window. deck126's own no-op stop went to **0 of 99**, which is why |
| **D15** annotation-only name binding | B | **SHIPPED — PASS 0** (was 1) | none of the 25 parenthetical disagreements names an annotation-only card. Watch item held: `stale_echo_in_range` 1 -> 5, `named_row_not_offered` 4 -> 6, **10 of 11 at one seat**, all a stale-plan `Cast Bloodline Keeper` / `Cast Vision Skeins` — a plan-staleness signature, not a parser regression |
| **D9** the `long_reply` stamp | B | **SHIPPED — PASS 0/1** | exactly one reply passed 95%: `123v125` s1 at **165.4%**, carrying `long_reply`, `deadline_pct` and `retry`. Residual: **D23** |
| **D4** player targets in the kill summary | C a2bc290d0 | **SHIPPED — PASS 0/586** | 586 rows enumerate a player in `legal targets right now:`; **none** carries a `{kills…}` summary that omits it. deck130's lethal declines **3 -> 0**. New siblings: **D7**, **D15** |
| **D10** the DRAW PRICE tail | C | **SHIPPED — PASS 43/43** | **0 rows of either form without `- you would be at K`** (was 58 of 85); `this KILLS you` **9 renders, 0 casts**. Sub-claim "Forced Fruition lethal tail stays 16/16": **UNTESTED — Forced Fruition rendered 0 times**; the tail worked on the five families that did arise (Barrowin, Acererak, Nadaar, Silverquill Silencer, Emeria's Call). It won `146v162` — skill #223 |
| **D11** unconditional removal victims | C | **SHIPPED — PASS** | `{kills: <names>}` **0 / 75 / 13 takes**; `{removes: <names>}` **0 / 38 / 11**; `{kills N of the …}` **0 / 12 / 0**. INDESTRUCTIBLE sub-claim **UNTESTED** — no such target arose |
| **D18** the menu-fit clause | C | **SHIPPED — PASS 1,203 renders / 218 takes** | 0 priced rows on a multi-priced menu without a fit clause. Four emitter forms all counted; a first pass looking only for the plural form scored 392 phantom misses (#208) |
| **D5** modal live/dead | C | **SHIPPED — PASS 17/17 rows, 11/11 Silverquill Command** | **0 / 17 / 7 takes**; the card was CAST twice (`146v126` s47, `146v152` s41) against 0 of 28 last wave. The wave's cleanest render-to-behaviour result. Residual: the sub-menu is a card-NAME menu — **D22** |
| **D3** gain-framed searches | D 6c1555bb1 | **SHIPPED — PASS 0/3, thin N** | old header **0 renders**; the gain header renders **2** (`162v125` s26, `126v125` s33). The one surviving loss framing is a genuine sacrifice to the graveyard and is correct. N = 2 against 8 |
| **D6** `your own effect` on an opponent's permanent | D | **SHIPPED — PASS 0 renders, N = 0 windows** | the literal renders **0** corpus-wide and the opponent branch now reads `FORCED sacrifice OF YOUR OWN CARD: the opponent's effect (<source>) …`. **No annihilator or forced-sacrifice menu reached any seat**, so this is a string PASS, not a behavioural one |
| **D8(b)** de-duplicated length-1 asks | D | **SHIPPED — PASS 0** (was 16) | `identical_option_asks_resolved` **5** on 2 gameends; the 110 length-1 `priority` menus are genuine single-ability windows with the virtual pass at index 0 |
| **D25** forced-loss list order | D | **SHIPPED — PASS 0** | no land below a non-land in any discard / bottom / AFFORD-TO-LOSE list. N thin |
| **D8(a)** the 2-D ability-menu collapse | D | **NOT SHIPPED, and the class GREW** | 46 records / 403 redundant rows / 160,803 chars; 13 menus over 12 rows collapse to few shapes (wave 53: 3) — **D8** |
| **D21** the ahead-on-life gate | E 5fe998414 | **SHIPPED — PASS 0 of 18** | 18 renders, none at `my_life <= opp_life`. **The corpus's only render FALSEHOOD is closed**, and deck123's A53-6 bullet retires with it (#187) |
| **D17** the blocker-count header | E | **SHIPPED — PASS 99/99** | every `attackers` record carries it, incl. the singular form. The BLOCKERS twin is missing — **D13** |
| **D16** the coverage cause clause | E | **SHIPPED — PASS 11/11** | two families, both true. **And the fix created a new problem** — the families call for opposite decisions and one shipped a three-land seven: **D12**, skill #210 |
| **D20** fetch-row colours | E | **NOT SHIPPED** | 0 empty colour strings, but the fetch rows still print no colour at all — **D17** |
| **D19** the doubled effect label | E | **SHIPPED — PASS 0/401** | `ability: X's X` **0** (was 53); generic fallback survives on 9 lines — **D27** |
| **D7(a)/(b)** `choice: -1` with no fallback; supervisor | F 3329c40f1 | **SHIPPED — PASS 0 / PASS 0** | every `choice: -1` carries a fallback (was 1 without); `engine_answered` never needed to render; 0 `hang` verdicts, supervisor never fired |
| **D7(c)** the reveal force-close | F | **SHIPPED — UNTESTED (N = 0), and its shape is too narrow** | `force-close` renders **0**. A reveal DID stall and the guard had no window on it because the shape has an option two — **D5** |
| **D38** `assertmtgid` + the flip-skip lift | F | **UNTESTED in corpus** | suite-side only; no DFC flip arose in the pool |
| **AA** (wave-53) the Vita softlock stall floor | AA 37b7951d3 | **FAIL — 468 declines on GPT seats, and it moved the corpus** | the lane's own falsifier was "0 such declines on GPT seats". 936 `held … for 12002 ticks` lines, one value; 488 stale drops; opponent-turn share 30.2% -> 19.9% — **D1**. **No disable flag shipped** |
| **G** ability ownership | 6dee5d2cd | **no drift observable; UNTESTED** | 0 crashes, 0 ASAN signatures, 21/21 natural, no dangling-ability trace. Three seats say "not observable", not "clean" |
| **H** ability epoch gate | e078c2501 | **not observable from a corpus** | `{right now:}` renders 1,025 times and every sampled verdict matched its own enumeration; no aslongas/lord/Isleback card in the pool exercised a delayed-condition bump visibly |
| **I** ManaCost / garbage / cleanup | 85c703453 | **no regression visible; RSS untested** | no dead-token dangle signature; `In GetCost Seems ManaCost was not properly initialized` is pre-existing noise |
| **J** render caches / resource memo | fbd46fa04 | **WATCH — 1,927 404 lines / 118 resources** | the memo is not covering the audio loader or surviving `Destroying WCachedResource` — **D26**; flag `WAGIC_RENDER_NOCACHE=1`. Visual parity eyeballed by nobody |
| **K** Baka `selectAbility` | afd88acd0 | **effectively UNTESTED** | Baka received 2 decisions + 1 handoff; both executions ordinary, no planner anomaly |
| **L** transport / narration / trim v2 | 27f867152 | **PASS on the falsifiable half** | `http_error` / `http_status` **0**; `ask` mean 14,245 -> **10,269 (−28%)**; `>25,000` prompts **588 -> 37**; prompt shapes wave-53-identical |
| **M** ask cache / prefill / combat cache | ba784277f | **PARTIAL — unmeasurable on its own watch item; first live exercise of the blocker helpers** | no cache-hit counter reaches any log (**D35**). 36 blockers records ↔ 36 `declared blocks from N assignment(s)` lines 1:1; `multiblock_first_wins` 1, `blocks_last_line_taken` 2, the one-blocker-many-attackers re-ask fired once — **and `all_assignments_illegal` fired for the first time and lost a game (D3)**. **NOT the cause of the stale-drop spike** (r = 0.5173) |
| **N** Vita NPOT textures | c2f34b05a | **UNTESTED** | no desktop exposure |
| carry: 21/21 natural | | **PASS** | invariant 00 satisfied |
| carry: Baka-executed <= 0.20% | | **PASS 0.09%** | 2 / 2,212 |
| carry: `with no source tapped` 0 | | **PASS 0** | all 21 stderr files |
| carry: name-vs-executed mismatch <= 1/3,000 | | **PASS 0/1,828** | wave 53 was 2/3,253, AT the ceiling |
| carry: `stale_livelock` 0 | | **FAIL 1** | **D5** — and it is a new mechanism, not the old one |
| carry: no prompt > 30,000 | | **FAIL 2** (was 12) — both ~75-80% log | **D28** |
| carry: fallbacks <= 0.5% | | **FAIL 1.04%** | diagnosed: 17 of 23 are one seat's two guide-authoring shapes (skill #211) |
| owner batches / O1 / O2 / lanes R, S, T, U, V, W | | **UNTESTED** | no `may pay` prompt, no equip-cost interrupt, no evolve/counters card, no adventure/split/morph, no Baka blocking decision, no DFC flip, no console exposure |

# DISCHARGED — SET A: wave-53 guide edits + the boundary pass, by deck (reviewer verdicts, summarised)

| deck | verdict summary | routed |
|---|---|---|
| **123** (2-4) | P1 fetch **FAIL 1 of 5** (was 3 of 8); P2 Tragic Slip **PASS 0, and 1/1 on a named row**; P3 **FAIL 2** (a PLAN asserting what the same window's `{right now:}` denies); P4 HOLD **SPLIT** (takes 32 of 275 and non-zero in every game with a repeated dead menu — PASS; runs of 3+ **43.7%** at this seat — FAIL); P5 Damnation **FAIL 1 of 1**, and the `N, K, M` line was WRITTEN for the first time in four corpora on the same line as the dead cast (skill **#212**); P8 `[second copy:` **FAIL 3 of 20**; P9a edicts **PASS 0**; five cards spent off all-dead menus in one game | skill #209/#211/#212/#213; **D2** |
| **125** (1-5) | W88 **PASS 0/1, near-vacuous**; W89 **FAIL 3/3 BY THE LETTER AND ALL THREE CORRECT** (skill **#214**); W90 tap-out **5 raw / 1 live** (#216); W91 **PASS 0/51 low-X and 6/6 at X=3**; W92 **2 raw / 0 live**; W93 **PASS 0/22**; W94 cleanup **PASS 0/18 on the Staff and FAIL 3 of 4 on steps 1-2 — the seat's two levered losses** (skill **#215**); W95 `hold_windows_skipped` **PASS 3/3**. **Three of five losses have no identifiable misplay and no edit was written for them** — the windows were not there (skill **#217**) | **D1**, **D9**; skill #214/#215/#216/#217 |
| **126** (4-2) | D92 **73 raw / 5 live / 0 consequential — PASS** (#198/#201); D93 **PASS 0/99** (was 2/39) and 0 PLAN-vs-CHOICE; D94 **PASS 0/3 and 10/17**; D95 **PASS 0/24**; D96 **PASS 0/15 and 0**; D97 **PASS 0**; D98 **PASS 5/5 receipts** and reveal picks **10/10** (was 7/9). **Every wave-53 prediction passed; three edits written** (skill **#220**). Two new one-sentence items: the `casting this taps you out` decline of the win button (skill **#218**) and the `[named:` tag (skill **#219**) | **D10**; skill #218/#219/#220 |
| **130** (2-4) | #0 draw punishers **PASS 0/12**, fourth corpus; #1 cycling **PASS 13/13**; #2 face damage **FAIL 1** (6 -> 4 -> 1); lethal declined **0** (was 3) — wave-53 P9 **PASSES**; land plan **PASS both clauses** (2 of 22 at a 5+-land line, 0 inside the damage in hand); Blastminer 0 self-targets; Talisman life **PASS 0**, fourth corpus; **X menu: 2 reached, 1 answered on the worst live row, 1 took the top of a collapsed run, and the marker rendered 0 times — wave-53 P15 is UNTESTED, not PASS** (skill **#221**); mulligan **0 ships, fourth corpus** | **D6**; skill #216/#221/#222 |
| **146** (5-1) | P1 mulligan **PASS 0 of 6 at 2+ lands**, second corpus, and the cross-guide control; P2 Emeria's Call **PASS 1/1**; P3 the ritual's deletion control **PASS 0/14**; the cast-order entry whose literal was unsatisfiable is **CLOSED from the engine side** by lane C's modal clause (11/11, 2 casts); cleanup superlative tail **FAIL 2 of 7** (skill **#224**); four DRAW-PRICE closes won a game with no guide sentence about them (skill **#223**) | **D9**; skill #210/#223/#224 |
| **152** (5-1) | P4 **FAIL 3 windows / 8 A-lines by the letter — adjudicated RULE STALENESS**, the engine now prints the price inside the same parenthesis and the seat won that game 20/-10 (skill **#226**); P5 mulligan **FAIL 1 of 4** on the same shape for a FOURTH corpus, with the trigger literal changed underneath it (skill #210, **D12**); P6 **PASS 0/6 and 0**; blocks: the corpus's only `all_assignments_illegal`, a pure strategy break under a rule that already renders (skill **#225**, **D3**) | **D3**, **D12**, **D13**; skill #225/#226 |
| **162** (2-4) | P8 **PASS both halves and the first-Mine over-brake lane CLOSES** (11 windows -> 0); P7 **UNTESTED** (no window met the precondition); the new lane is its exact mirror — a brake keyed to a CATEGORY firing on 10 own-turn windows across the deck's two window-carrying losses, with `{feeds:` available on the row the whole time (skill **#227**); the cast order gated a prerequisite behind the thing it enables (skill **#228**); three "keep mana open" declines in a deck with no instants (skill **#229**) | **D19**, **D20**; skill #227/#228/#229 |
| **pool** | wave-53 boundary B1-B10: **B1's REVERSE audit is the wave's most valuable process result** — it caught the HOLD literal in TWO guides (skill #209). B3 retires with D21. B4's D10 scope landed. B5's D4 re-key landed. B6's D5 re-key landed and the entry is alive. B7's D17 obligation landed. B9's watch did not fire. **B10 is discharged by this step: the band is restated from measurement at 40-70 KB and the pool grew +19,028 bytes (+5.5%) in one wave** (skill **#232**) | skill #209/#232 |

---

# FALLBACK CLASSES — 23 / 2,212 = 1.04%; Baka-executed 2 = 0.09%

| n | class | records | disposition |
|---|---|---|---|
| 12 | `plan_choice_conflict` | 5 at `123v126`, 2 at `123v146`, 1 each at `126v125`, `130v123`, `130v126`, `146v126`, `152v146` | all 12 co-stamped `decision_reversed_in_prose`; **0 executed the HOLD row** (D2b CLOSED). 6 `_recovered`, 6 `_exhausted`; **four of the exhausted re-asks returned the byte-identical reply and were executed anyway**, at a full extra model call each. Root cause is the PLAN template deck123's guide mandates — skill **#211**; the narrowing ask (fire only when the prose NAMES a different row) is deck123 G2 and is a CORE-PROMPT/parser item, not a general widening of the wave-52 rejection |
| 7 | `named_row_reask` | **all 7 at `123v125`** (s37, 56, 100, 105, 150 + 2) | every one names `Cast Bloodline Keeper` or `Cast Vision Skeins` off a stale plan; those are that guide's own cast-list ENTRY numbers answered as if they were row numbers. 5 `_recovered`, **2 `_exhausted`** — and those two are the corpus's 2 Baka executions. Guide-side (A54-7); the engine half is that the reply-format line does not echo the menu's numbering |
| 2 | `named_row_not_offered` | `123v125` s58, s107 | the exhausted tail of the above; `recovery` at s59 shows Baka cast Lightning Greaves |
| 1 | `stale_livelock` | `146v123` s15 | **D5** — a reveal livelock, first recurrence of this class in three corpora, and a different mechanism from the wave-52 one |
| 1 | `all_assignments_illegal` | `152v146` s58 | **D3** — new class, first firing, no re-ask, lost the game |
| 0 | `empty_reply`, `timeout`, `multiblock_reask`, `retracted_choice`, `engine_answered` | | all five at zero |
| — | **`recovery` contract** | 10 records | **PASS 10/10, 0 unpaired**: exactly 10 records carry `choice: -1` WITH a fallback and exactly 10 `recovery` records exist, each at `seq+1` with `recovers_seq` / `recovers_kind` / `recovers_fallback`. The 12 `plan_choice_conflict` and the 1 `all_assignments_illegal` have `choice != -1` and correctly get none |
| — | **not a fallback, and it is D1** | 488 `dropping stale async answer` | **no record of any kind is written**; the seat silently declined without being consulted |

---

# PROMPT CHARS PER DECISION KIND — wave 53 -> wave 54 (21/21 games)

| kind | n53 | max53 | **n54** | mean54 | p50 | p90 | **max54** | p90 lat (s) | max lat (s) |
|---|---|---|---|---|---|---|---|---|---|
| ask | 2,754 | 30,146 | **1,683** | **10,269** | 9,403 | 18,301 | 28,776 | 48.5 | **1,488.7** |
| priority | 519 | 32,068 | **332** | 12,515 | 11,469 | 21,699 | **32,215** | 64.6 | 346.8 |
| attackers | 120 | 28,743 | **99** | 11,265 | 9,729 | 20,636 | 28,108 | 63.2 | 99.5 |
| blockers | 30 | 22,499 | **36** | 12,548 | 10,350 | 24,354 | 26,764 | 82.5 | 150.2 |
| discard | 34 | 28,587 | **34** | 12,333 | 10,940 | 22,011 | 25,767 | 27.7 | 54.5 |
| reveal | 21 | 25,312 | **20** | 15,693 | 14,912 | 24,609 | 27,183 | 53.9 | 61.5 |
| bottom | 5 | 2,341 | **8** | 2,196 | 2,126 | 2,634 | 2,634 | 182.9 | 182.9 |

Overall: **2 prompts exceed 30,000** (was 12), **37 exceed 25,000** (was 588), `ask` mean **−28%**.
Total prompt volume 23.76 M chars over 2,212 decisions. **Recorded inference 23.9 s per decision
against 21.7 s last wave (+10.2%) on a 28%-smaller mean prompt, with ≈3.2 h of dropped-call
inference outside the total entirely — the cost story is now D1, not the log window.**

---

# LATEGAME SPECIMENS — invariant 0 (for the owner's per-wave prompt review)

**RECOMMENDED PRIMARY, and it is what is already written to `wave54/lategame-specimen.txt`:**
`1788425073-ai_baka_deck146-0x55cbe78957e0-vs-ai_baka_deck162.jsonl` seq **46**, turn 25, Main
phase 2, **23,237 chars, 7 rows, 6.9 s latency, 8 life vs 8 life**, 14-card hand, 4 untapped
sources, log share **64.4%**. It carries `[DRAW PRICE: … - you would be at 1]` with the life tail,
`{modes live right now: …; dead (no legal object right now): …}` on Silverquill Command's nine-mode
list, three distinct `{leaves N source(s) - …}` fit forms, the dungeons-completed line, the
`{castable from exile - …}` cause clause and the summoning-sick/counters battlefield vocabulary —
**every one of this wave's shipped render clauses on one screen a human reads end to end** — and the
seat answered `Cast nothing right now` at 8-8 with a full grip. That is the owner's criterion
(decision value per token) posed as a single question: **23 KB bought a decline at parity.**

**RECOMMENDED SECONDARY, relabelled as what it is** (`wave54/lategame-specimen-logcost.txt`):
`125v123` seq **188**, turn 63, Cleanup, `kind: discard`, **25,767 chars, 9 rows**. Its nine rows are
bare card names with no pricing, no verdict, no fit clause, no HOLD row and no modes — **80.6% of a
25.8 KB prompt is game log, to choose two cards to pitch.** It is the corpus's best exhibit of ONE
thing, which is the log-cost question and D9's missing verdicts, and a poor exhibit of everything
else. Ship it beside the primary, labelled, or not at all.

**seat-123-130's proposal (`123v125` seq 302-310) is DECLINED for invariant 0 and PROMOTED for the
lanes.** Nine consecutive records at turn 63 (Upkeep ×5, Main 1 ×2, Attackers, Combat ends) on a
44-permanent log, 22.0-23.8 KB each, carrying a same-phase repeat run, a HOLD take mid-run at s306
with `[you declined this exact list 4 times already this turn]` on the prompt, the declined-note
counter resetting across the phase boundary, and a correct opponent-turn edict at s310 after 300
decisions. It is the best available EXHIBIT for the window-economy lanes (D1, D4, D21) — but it is
nine prompts of ~23 KB each and the invariant-0 question is asked of ONE screen. **If the owner
wants one file, ship the primary.**

**Defect specimens (for the lanes, NOT for the prompt review):**
`game-126v123-1788425076.stderr:306-341` (D1, seven windows released in one turn) ·
`130v126` seq **67-106** and `123v126` seq **112-140** (D2, the two sides of one drain loop) ·
`152v146` seq **58** (D3, the lethal window whose whole answer was discarded) ·
`126v125` seq **72-121** (D4, the 50-window own-turn run) ·
`146v123` seq **15** + `game-146v123-1788425054.stderr:1335-1343` (D5, the reveal livelock) ·
`130v152` s17 (D6, the unmarked X menu that lost a game) ·
`123v126` seq **104** (D7, 900 words spent asking a question the prompt never answers) ·
`123v162` seq **26** (D8, 23 identical equip rows) ·
`125v162` seq **47** (D9, nine unannotated discard rows at 8 life) ·
`126v146` seq **61** (D10, the unpriced `[named:` cast that closed a 5-life margin) ·
`146v126` seq **54/56/57** (D11, the three genuine name collisions) ·
`146v162` seq **46-58** (the POSITIVE control — four DRAW-PRICE closes that won a game, skill #223).
