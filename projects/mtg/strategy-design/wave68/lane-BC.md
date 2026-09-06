# Wave-68 lane BC — J2, J7 + the record MEDs

Base: master `4837e76a1`. Branch `w68-lane-BC`, worktree `worktrees/lanes/w68-BC`.
Every edit byte-spliced with python `rb`/`wb`; `git diff | /usr/bin/grep -c $'\357\277\275'` = **0**
after every edit. Every wagic run memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`); nothing was
killed under the cap. Every comment tagged `#W68-BC (<item>)`. No live model probe, no corpus run.

Files changed:
`src/AIPlayerGPT.cpp`, `include/AIPlayerGPT.h`,
`bin/Res/sets/primitives/{_macros,mtg,borderline,planeswalkers}.txt`,
`bin/Res/test/_tests.txt`, new fixture `bin/Res/test/w68BC_ward_targets_the_spell_not_a_random_zone.txt`.

---

## J2 — the wall arm bought a SECOND full deadline. THE COUNT FIRST.

The brief said 3 wall misses; the corpus carries **4**, and lane AX's "zero retry budget by
arithmetic" is falsified by every one of them. Counted over all 64 translogs (records whose
`transport` stamp carries `phase=wall`):

| record | kind | latency_ms | deadline_pct | what it did |
|---|---|---|---|---|
| `deck162-0x56442565e5c0` (v126) s13 | blockers | 1,233,411 | **137.0** | armed a retry, **consumed a second 333,376 ms deadline**, answered `BLOCKS: B3:A1` |
| `deck130-0x5644fdfb91d0` (v162) s20 | wall_miss | 900,035 | 100.0 | armed a retry; decision drifted -> abandoned (`wall_miss_unrecorded_wall`), heuristic answered |
| `deck123-0x55ab261b61b0` (v162) s7 | wall_miss | 900,025 | 100.0 | same |
| `deck123-0x55931a3db9d0` (v125) s16 | wall_miss | 900,025 | 100.0 | same |

So: **4 armed, 1 consumed, 3 abandoned.** All four took `:15196`'s
`mRetryBudgetMs = mLastTimeout ? 0 : transportBudgetMs`, and `0` is read by `buildRequestBody` as
"no override" — a fresh, FULL `WAGIC_GPT_TIMEOUT`. AX's premise is true of
`remainingTransportRetryMs` and is pinned as such in PARSETEST; the wall arm simply never called it.
The seat's worst case on ONE decision was 2 x the deadline (1800 s on this dial).

**Shipped (a) — one remainder rule for both arms.** New pure
`retryFitsInDeadline(deadlineMs, firstLatencyMs)` = `remainingTransportRetryMs(...) > 0`.
`pollCompletionRetry` now computes ONE `retryBudgetMs` and gates BOTH arms on it, so:
* a **transport** failure (connect died at 20 s of a 900 s deadline) still retries, inside the
  remainder — unchanged;
* a **wall** miss has a remainder of 0 by definition, buys nothing, and is written down at the
  moment the deadline is spent as a `wall_miss` record with the new class **`wall_miss_no_retry`**
  (new gameend counter of the same name; the stderr line reads
  "the deadline is spent, no retry - heuristic"). `flushWallMissRecord` takes a class override and
  counts a no-retry miss separately from an abandoned ("unrecorded") one.

No window is removed and nothing is capped: the heuristic answers the same window it would have
answered 900 s later. `WAGIC_GPT_TIMEOUT` is untouched — it is the owner's dial, and this change
makes it mean what it says (one decision, one deadline).

**Shipped (b) — the meter stops lying.** `:15090` still sums both legs into `latency_ms` (that IS
the seat's total wait and the harness's cost accounting reads it), but the LEGS are now kept:
`mLastAttemptFirstMs` / `mLastAttemptSecondMs`, published as **`attempt_ms: [900035, 333376]`**, and
`deadline_pct` is computed by the new pure `deadlineTenthsPctOfAttempts` = the worst per-ATTEMPT
fraction. s13 would now read `deadline_pct 100.0` with `attempt_ms [900035, 333376]` instead of
`137.0`. A single-attempt record is byte-identical to what shipped (both legs -1 -> the old
`deadlineTenthsPct(latency, timeout)` path). Both legs are consumed with the record, like
`latency_ms`.

The two legs also make the force-close retry (`:15145`, which sums the same way) legible for the
first time.

**Slot-key discipline (wave61/corpus-livelock.md).** Nothing here reaches `mPromptTail`, the ask
key, the async slot key or `optionSetKeyOf`. `attempt_ms`, `deadline_pct`, `wall_miss_no_retry` are
translog/gameend fields; `retryFitsInDeadline` reads two longs. Shared choke point checked:
`remainingTransportRetryMs` had one caller (the transport arm) and now has two — both read it, no
caller's semantics change.

## J7 — ward: the rider's host is the SPELL, not any card in any opponent zone

**Oracle verified first** (`/wagic-cardscript` rule 1). Moonrage Brute (MID #7 back face) is
"First strike -- Ward—Pay 3 life. -- Nightbound" (Scryfall/MTGSalvation), and CR 702.21b is
"Whenever this permanent becomes the target of a spell or ability an opponent controls, **counter
that spell or ability** unless that player pays [cost]." The primitive's COST (3 life) is right;
its host zone is not.

**The mechanism.** `TargetChooser.cpp:476` expands `opponentzones` to OPPONENT_BATTLEFIELD,
**STACK**, GRAVEYARD, **LIBRARY**, **HAND**, EXILE, SIDEBOARD, COMMANDZONE — battlefield first.
Every ward script chose its `pay-or-fizzle` rider's host with `target(...|opponentzones)`, so the
interactive-AI seat's target ask enumerated all eight zones: `152v130` s33 printed **12 opponent
LIBRARY cards and a HAND card** as pickable ward targets, labelled as the ward controller's choice.
`opponentstack` is the zone the rule names, and the corpus already contained one script that used it
(`borderline.txt:110321`).

**The audit (all four primitive files, every ward script in the pool).**

| file | ward `target(...|opponentzones)` occurrences | fixed |
|---|---|---|
| `_macros.txt` (`_WARD1_`, `_WARD2_`, `_WARD3_`, `_WARD4_`, `_WARD8_`) | 5 | 5 |
| `mtg.txt` | 1 | 1 |
| `borderline.txt` | 78 | 78 |
| `planeswalkers.txt` | 1 | 1 |
| `unsupported.txt` | 0 | 0 |
| **total** | **85** | **85** |

The five `_macros.txt` lines are the shared choke point: **116** card lines in the pool expand a `_WARDn_`
macro (114 borderline, 1 mtg, 1 planeswalkers), so the macro fix reaches all of them without touching their card blocks. The 80 in-card
lines are the hand-written wards (life wards, experience/power-scaled ward ladders, the
"ward—sacrifice a permanent" and "ward—discard a card" shapes whose rider is a nested
`choice name(Counter spell) target(*|mystack) fizzle` rather than `donothing?fizzle` — 6 of those,
all found and fixed). Selection predicate: a line carrying `@targeted(`, `transforms((,newability[`
and `|opponentzones)` inside a `target(...)`. **Every** `target(...|opponentzones)` in the pool was
a ward; there were no other users, so no non-ward chooser moved. Only the `target(...)` was
narrowed — the trigger's own `from(...|opponentzones)` is untouched (see residuals).

Card scripts are DATA: no recompile was needed for this fix, and the PARSETEST card-parse count did
not drop (5120 self-test cases pass and the suite loads 1275 fixtures against the edited files).

**Fixture** (`w68BC_ward_targets_the_spell_not_a_random_zone.txt`, registered in `_tests.txt`,
`git add -f`): P1 taps out for Shock at a warded Tomakul Honor Guard (3/1, `auto=_WARD2_`), cannot
pay the {2}, and Shock must be countered with the 3/1 alive; Mind Stone is P1's battlefield decoy.
**It is GREEN on base as well as after** — see "what I did NOT verify". It is a rules PIN on the
outcome, not a repro of the leak.

### Teferi's +1 cascade stage 1 — NOT SHIPPED, deliberately

`152v123` s39-41 (427 s over three windows) and `152v125` s40/57 are real, and the cause is now
located: Teferi, Who Slows the Sunset's +1 is THREE separate `@counteradded(...TeferiEffect)` may-
triggers (`planeswalkers.txt`), each arriving as its own CHOOSE_MENU
("1. choose a land / 2. Decline - do nothing"), each carrying an `ability$!...!$` payload whose
inner `choice name(Choose your land) target(land|myBattlefield) untap _ choice name(Choose opponent
land) target(land|opponentbattlefield) tap` is stage 2. To list stage 2's candidates on the stage-1
row the seat must reach the live `MenuAbility::abilities[i]` (public) and walk that payload's
choosers; `DecisionRequest` carries only `optionTexts`, so nothing in the seat's current inputs can
do it.

I did not ship it because the only cheap alternative — deriving the candidate list from the row's
TYPE WORD ("choose a land") against a battlefield — would print a set the engine had not agreed to,
which is exactly the false surface the trust doctrine forbids, and the pick order I would have had
to declare (the corpus shows land -> creature -> artifact, the reverse of registration order) is a
LIFO artifact I could not prove holds in general. The reachable design is recorded above for
whoever takes it: annotate from `MenuAbility::abilities`, script-derived in the shape
`modeRowAnnotations` already uses, never from the label.

## MEDs

**(1) `named_row_reask` recovery records carried no evidence (engine MED-1; 123 s42).**
AX's I7 wired `noteHeuristicExecuted` at three HEURISTIC sites; a class that recovers by
**re-asking** reaches none of them, and `flushRecoveryRecord` correctly stayed silent — a record
that named a failure and said nothing about the outcome. New `noteReaskExecuted(seam, row, text)`
writes the same three fields plus a fourth, **`executed_by`** (`"heuristic"` | `"reask"`), so no
reader can mistake a re-ask for the heuristic; `executed_by` is written on every record that carries
`executed_seam` (existing records read `"heuristic"`, which is what they were). Stamped where the
second answer becomes FINAL, at both seams that re-ask: the priority seam
(before `mPriorityReaskKind.clear()`) and the casting seam (after the `mAskReaskKind` ladder), both
of which run before `writeTransLog` flushes the pending recovery record. `choice == 0` is a pass and
is stated as one; a second answer that still failed to parse (`choice < 0`) stamps nothing and
leaves the heuristic's own site to speak.

**(2) The legend-rule ask printed neither the rule nor either copy's loyalty (146 s51).**
`MTGNewLegend::MoveLegend` builds a `*[-mutated;legendary;share!name!]|mybattlefield` chooser over
an AAMover-to-owner's-graveyard, so the seat met it as a bare
`TARGET CHOICE for Lolth, Spider Queen - its "put a card into the graveyard" ability` over two rows
reading `[planeswalker] [your battlefield]` with **no number on either**. The reply invented the
merge: "Lolth #2 enters with 4 + 4 = 8 loyalty counters." Two true tokens added:
* `describeTarget` now prints `[loyalty N]` on **every** planeswalker target row, off
  `counters->hasCounter("loyalty",0,0)->nb` — the same counter the board line already prints.
* the ask gains `legendRuleTargetClause(name, copies)` (pure, PARSETEST-pinned) naming CR 704.5j,
  the copy count, and the one thing most easily assumed wrong: the copies do NOT merge — no
  counters, damage, attachments or abilities move, so the survivor has exactly what its row shows.
  Detected off engine facts only (>= 2 candidates, every one a legendary permanent on MY battlefield
  sharing the source's name); append-only, no row added, removed or reordered.

**(3) `async_drops` census (report only — nothing to fix).** 22 drops over **2,304** round trips =
**0.95%**, down from wave 66's 59/2,098 = 2.8%. Zero `stale_livelock`, zero give-ups.

| dimension | census |
|---|---|
| by drop ARM (the stamp's own first field) | casting **22** — every one; no combat, target or menu arm dropped anything |
| by the KEY HALF that moved | `question (or turn/phase)` **18** · `board` 3 · `question and board` 1 |
| by the record that stamped them | ask 20 · priority 2 |
| by seat | deck146 12 · deck130 5 · deck125 4 · deck123 1 |
| by phase | Main phase 1 14 · Upkeep 4 · Blockers 3 · Combat damage 1 |
| outcome | **re-asked 22 of 22** |

Same verdict as wave 67 and the same one lead: every drop is on the CASTING arm, so a wave that
wants the rate lower looks at that arm's prompt-rebuild cadence, not at the async layer.

---

## Gate

Detached unit `w68-BC-gate`, `MemoryMax=4G MemorySwapMax=0`, foreground until-loop wait on
`~/.gatelogs/W68_BC_DONE` (removed before launch). Binary rebuilt from clean
(`rm -f bin/wagic && make -f Makefile.sdl -j4`), freshness-checked by mtime after every make; qmake
untouched, no new sources. No build was chained behind a counting grep.

| leg | this tree | base |
|---|---|---|
| build | clean link, 0 errors | — |
| PARSETEST | **5120 passed, 0 failed** | 5107 (**+13**) |
| suite (`WAGIC_TESTSUITE_THREADS=1`) | **1275 tests, 0 failed** | 1274 (**+1**, the new fixture) |
| AI tests | **67, 0 failed** | 67 / 0 |
| `==Test Failed !==` | 0 | 0 |
| `==Test timed out` | 0 | 0 |

Log: `~/.gatelogs/w68-BC-gate.log`. Nothing was killed under the memory cap.

## RED-on-base evidence

**One RED-on-base result, and it is honest about what it covers: ZERO of the three shipped items
has a suite fixture that goes red under reversion.** Stated plainly rather than dressed up:

* **J2** is seat-side transport arithmetic. There is no endpoint under the suite, so the
  instrument is PARSETEST, and the falsifiable content is carried by cases that RUN AGAINST THE
  CORPUS'S OWN NUMBERS: `deadlineTenthsPct(1233411, 900000) == 1370` is the shipped meter's lie
  reproduced as a case; `deadlineTenthsPctOfAttempts(900035, 333376, 900000) == 1000` is the fix,
  with a MUST-NOT-MATCH that it is never 1370; `retryFitsInDeadline` is pinned false at all three
  of the corpus's wall latencies (900035 / 900025 / 900022) and true at a connect-phase 20,000 ms,
  with MUST-NOT-MATCH cases for an unknowable latency and no deadline. The three new pure functions
  did not exist on base, so "red under reversion" is meaningless for them — what is falsifiable is
  the corpus number each case carries.
* **J7** ships a fixture that is **green on base**. I verified this the only way that counts: I
  reverted the five `_macros.txt` lines in the built tree and re-ran the scoped registry — still
  `==Test Successful !==`. On the scripted CLICK path the chooser already settles on the stack
  card (the Mind Stone decoy was never taken), so the outcome does not discriminate. The leak is in
  the interactive seat's enumerated ROW LIST, and that seam has no endpoint under the suite — the
  RED evidence is the corpus record (`152v130` s33: 12 library cards + a hand card offered) plus
  `TargetChooser.cpp:476`. Per the skill's harness-vs-live adjudication rule I am scoping this as a
  LIVE-path defect with a harness PIN, not claiming a harness repro I do not have.
* **the MEDs** are record fields and a render clause; the legend clause is PARSETEST-pinned
  (positive, number agreement, and a MUST-NOT-MATCH that the clause carries no per-copy number —
  the loyalty rides the ROW).

## Predictions (falsifiable, for the wave-68 corpus)

1. **J2 (budget).** **0** records carry `retry: 1` together with a `phase=wall` transport stamp,
   and **0** decisions spend more than `WAGIC_GPT_TIMEOUT` in total. Every wall miss appears as a
   `wall_miss` record classed **`wall_miss_no_retry`**, and `wall_miss_no_retry` on the gameend
   record equals the number of `phase=wall` records. **Falsifier**: one record whose `latency_ms`
   exceeds the configured deadline, or one `wall_miss_unrecorded_*` class (that class can now only
   arise from a transport-arm retry that drifted).
   Counter-measurement (the cost this buys): wall misses now go to the heuristic instead of getting
   a second chance at the model. In this corpus that would have cost ONE model answer
   (162v126 s13's `BLOCKS: B3:A1`) and saved 333 s; if the next corpus shows several wall misses
   whose heuristic answer visibly loses a game, the remainder rule is right and the DIAL
   (`WAGIC_GPT_TIMEOUT`) is what should move — it is the owner's.
2. **J2 (meter).** **0** records carry `deadline_pct > 100`. Every record carrying `retry: 1` also
   carries `attempt_ms` with two entries, and its `deadline_pct` equals the larger of the two legs
   divided by the deadline. **Falsifier**: either.
3. **J7 (ward).** **0** ward target asks list a card in a hidden zone. Concretely: no `ask` record
   whose prompt contains "TARGET CHOICE" for a ward trigger offers a row naming a card that the
   same prompt's board section does not show in a public zone; and the ward ask's option count is
   at most the number of spells on the stack. **Falsifier**: one ward ask offering more rows than
   there are objects on the stack.
4. **MED-1.** **0** `recovery` records at the `ask` or `priority` seam carry neither
   `executed_seam` nor `recovered_by`. Every `*_reask` recovery carries `executed_by: "reask"`.
   **Falsifier**: one blind recovery record at either seam.
5. **MED-2.** Every planeswalker TARGET row carries `[loyalty N]`, and every legend-rule pick
   carries the `LEGEND RULE (CR 704.5j)` clause. **0** replies to a legend-rule ask assert a
   loyalty total that is the SUM of the two copies. **Falsifier**: a reply doing arithmetic across
   the copies, or a legend-rule ask without the clause.

## What I did NOT verify

* **No live model probe and no corpus run.** Every seat-side change is verified as a pure function
  over recorded bytes, as a suite fixture, or as a string — never as a decision a model made
  differently.
* **The J7 fixture is GREEN on base** (verified by reverting the macro lines and re-running). It
  pins the rule, not the leak. I did NOT build an instrument that shows the leaked ROW LIST under
  the suite, because the seam that builds it is the interactive seat's and has no endpoint there.
* **Ward covers spells AND abilities; this fix only reaches spells.** An ability that targets a
  warded permanent puts no CARD on the stack, so after the fix the chooser finds no candidate and
  the ability resolves unpaid — the same outcome as before, by a different route. That half is an
  ENGINE gap (there is no construct for countering an ability on the stack via a card chooser), it
  is pre-existing, and I did not widen the lane to it.
* **Ward filters remain as their authors wrote them.** 41 of the 85 lines filter
  `*[instant;sorcery]`, so an Aura or a creature spell that targets the warded permanent does not
  trigger those wards at all. That is a second faithfulness gap against CR 702.21b, it is a
  behaviour change to the TRIGGER (not the row list), and it was out of this item's scope. Listed
  so the next wave can price it.
* **The trigger's own `from(...|opponentzones)` is untouched.** Narrowing it would change WHEN
  wards fire; the finding was about what the chooser OFFERS.
* **`wall_miss_no_retry` has never fired.** The branch is exercised by no test — the wall path
  needs a real 900 s deadline against a hung endpoint, which I did not stand up (the same limit
  lane AX recorded for its transport verdict). Prediction 1 is where it shows.
* **`attempt_ms` on the FORCE-CLOSE retry is unobserved.** That path sums latencies through the
  same lines, so it should publish two legs; this corpus has no force-close record to check against.
* **The legend-rule detection is unexercised by any test.** It needs two same-named legendary
  permanents on one battlefield; no suite fixture builds that, and the predicate is engine facts
  (controller, type, name), not a string, so PARSETEST cannot reach it. Only the CLAUSE is pinned.
  If the predicate ever mis-fires it would ADD a rule sentence to a non-legend ask — additive, but
  wrong, and prediction 5 is the check.
* **`[loyalty N]` widens every planeswalker TARGET row, not just legend-rule rows.** That is
  intended (a walker's loyalty is the deciding fact of any pick that touches it) but it is a byte
  cost on rows the finding did not name, and I did not measure it against the pool's byte ceiling.
* **Nothing was checked against the two known concurrency-only failures** (`lifeline`,
  `merrow_reejerey`) or the intermittent `intrepid_adversary_repeated_payment`: the gate ran
  single-threaded and read 0 failures.
* **Teferi stage 1 is not shipped** (see above). 152's three 427 s gates will recur.
