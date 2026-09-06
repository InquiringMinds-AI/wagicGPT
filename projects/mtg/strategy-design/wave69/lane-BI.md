# Wave-69 lane BI — K7: the record that says nothing, the surfaces nobody sees

Base: master `169ee41f3`. Branch `w69-lane-BI`, worktree `worktrees/lanes/w69-BI`.
Files changed: `projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/include/AIPlayerGPT.h` (only).

Every count below is a read-only python pass over
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/` (42 seat JSONLs, 2,414 records,
2,282 decisions) made BEFORE any code was written. Every edit was a python `rb`/`wb` byte
splice; `git diff | /usr/bin/grep -c $'\357\277\275'` = **0** after every one. Every wagic run
memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`); **no kill under the cap on any run**. The
gate ran DETACHED (`w69-BI-gate2`) with a foreground until-loop wait on `~/.gatelogs/W69_BI_DONE`
(removed before launch); the binary was freshness-checked (`ls -la bin/wagic`, 15:43) after every
make and no build was ever chained behind a counting grep. Every comment tagged `#W69-BI (K7)`.
Nothing added here reaches `mPromptTail`, the ask key, the async slot key or `optionSetKeyOf`:
the new fields are translog/gameend only, the four render changes are pure functions of the
board and the row, and the three new counters are game totals that never enter a rendered string.

---

## 0. THE VERIFICATION FIRST — `transport` / `attempt_ms` (engine MED-3)

**Finding: the wiring LANDED and never FIRED. Both fields were conditional emitters and this
corpus contained none of the three events that trigger them.**

* `transport` (`#W59-H K1`, wave 59 — not wave 68) was pushed onto `mLastTransportOutcomes`
  ONLY at `if (body.empty() || mLastHttpStatus != 200)`. Corpus: **0 non-200 replies, 0 empty
  bodies** (`http_status` absent from all 2,414 records; `empty_reply` 0), so the vector was
  empty on every record and the field was never written.
* `attempt_ms` (`#W68-BC J2`) was written only when `mLastAttemptFirstMs`/`SecondMs` were set,
  which happens only in `pollCompletionRetry`'s retry arm. Corpus: **0 retries, 0 wall misses**
  (`wall_miss_events` 0 on all 42 gameends), so both legs stayed -1.

The code is present and correct in the built binary; the brief's expectation that `transport`
"should appear on every round trip" was not what wave 59 shipped. What wave 68's engine seat
could not do — tell "conditional" from "not wired" without a failure window — is the actual
defect, and it is now impossible:

**Shipped.** Two pure functions above the record writer, `transportRecordField(failures,
roundTripStamp, latencyMs)` and `attemptMsField(first, second, latencyMs)`, own the rule. Every
record that spent a round trip now carries `transport` (a success stamps `curl=0,http=200,empty=0`
from the SAME `transportOutcomeStamp` builder, so a failed trip's stamp is byte-identical to what
shipped) and `attempt_ms` (one entry on the ordinary path, the two legs on a retry, unchanged).
A record with no round trip (cache/reuse, `latency_ms` -1) still writes neither — its absence now
means exactly one thing. `deadline_pct` is computed from the same legs as before (the
`mLastAttemptFirstMs >= 0` branch is untouched), so no record's `deadline_pct` moves.

---

## 1. WHAT ELSE SHIPPED (counts first, in every case)

### (a) `async_drops` is absent from `gameend` (engine MED-2)
COUNTED: 49 decision records carry `async_drops` (2.15%); **0 of 42 gameend records** carry it,
while `wall_miss_events`, `hold_windows_skipped` and six others are written unconditionally.
A reader taking the per-game total off the gameend record reads an absent field as zero.
New member `mAsyncDropsGame`, incremented **at the drop** (not at the flush, so a drop that never
reaches a record is still counted), written as `async_drops` on the gameend record.

### (b) 2,727 suppressed hold windows leave no record (engine MED-5)
COUNTED: `hold_windows_skipped` sums to **2,727** over 25 nonzero seats (max 669). There is
exactly ONE increment site (`holdHonoured`) and exactly TWO suppression classes — the two seams
that call it, `"priority"` (`chooseOrderedAction`) and `"cast"` (the casting menu). The record
carried the sum and not the split. `hold_windows_skipped_priority` and `hold_windows_skipped_cast`
now ride the gameend record and sum to the existing total (the total is untouched, so no
existing reader breaks). This is the counter-per-class the brief asked for; the per-window LINE
that MED-5 also wants (seq, seam, matched key) is NOT shipped — see "what I did NOT verify".

### (c) the repeat row's short name contains the literal `N` (deck123 MED-2)
COUNTED: 2 `repeat_count_reask`, both at `deck123v162-5f8310` (s23, s25), both recovered.
Both replies wrote `CHOICE: 2 (Create human with Thraben Doomsayer, repeated N times, then stop)`
— they copied the ROW'S OWN NAME, which carries the letter N, while the row's worked example
says to append `x<N>` and the prompt's CHOICE-format instruction says to copy "the name only".
Both PLANs stated the count ("this window: Create human x6").

⚠ The reviewer's cheapest fix — print the placeholder in the row's short NAME — is NOT taken:
**deck123's live guide keys on the literal `"repeated N times, then stop"`** (`deck123_strategy.txt`
l.69), and Amendment 332 forbids moving a string a guide keys on. The row's head is therefore
byte-identical. What changed is inside the row's own bracket, where the contradiction lives:
`N is a DIGIT you choose, and copying this row's name with the letter N still in it names no
count and is refused and re-asked`. The refusal itself stops pointing at "the row's own format"
(the string that was copied) and instead quotes what was written and names the substitution,
with the BASE row's short name and this row's own index in the example.

### (d) `[repeat: activated this turn N times]` taken past 4, no `repeat_past_stop` (deck130 LOW; BA-7 FAIL)
COUNTED: **4 takes of an annotated row**, all `deck130v126` (s27, s36, s37, s38 — the fourth
Goblin sacrifice at the face), and `repeat_past_stop` fired once in the whole corpus, at a
different seat (`deck123v162` s27); `repeat_clamped_to_own_stop` = 0.

**Why the guard did not see the tagged row: it never looks at the tag.** `repeatPastStop` is
built from the model's OWN two numbers — `repeatPlanStopAndCurrent(reply)` must find a stop and
a current count — and `[repeat: activated this turn N times already]` is an ENGINE fact about
this turn. I read all four deck130 PLANs: **not one states a stop or a count**, so the predicate
returns false and there is no stop to violate. (The row test BA J1 widened — `repeatBaseTaken` —
was not the blocker; the missing input is the PLAN.) Inventing a stop from the annotation would
be a ceiling of the engine's own, which the doctrine forbids, so what is owed is a RECORD: new
pure `repeatAnnotationCount(row)` stamps `repeat_annotated_take(n=N)` on the record of any take
whose chosen row carries the annotation (only where the answer STANDS — a refusal record carries
its own fallback and executed nothing), and `repeat_annotated_takes` totals them on gameend.
Whether the tag changes behaviour is now a cross-tab, not a prompt grep.

### (e) `- GANG BLOCK:` and the collapsed multi-blocker tag render 0 times (deck146 MED)
**Verdict: NEITHER deleted NOR rewired — both emitters are REACHABLE and their windows did not
arise. Counted, not argued.** 48 attacker rows carried `[their untapped blockers: ...]` across
the corpus: **33 with one candidate, 15 with two, none with three or more.** The collapsed form
needs **more than four** candidates (`kPotentialBlockersEnumerateMax`), so it could not fire; the
`GANG BLOCK:` verdict needs `gangKillNeed(...) >= 2`, i.e. an attacker no SINGLE listed blocker
kills, and on all 15 two-candidate rows one blocker already killed it. Deleting a correct
surface because a six-game corpus never produced a wide board would be deleting the answer to
the exact trap that produced it (`162v123` s13, three 2/2s gang-blocking a 5/5).

What was actually missing is the evidence to decide it: four gameend counters —
`blocker_forecast_rows`, `_multi` (>= 2 candidates), `_gang` (carried the verdict), `_collapsed`
(> 4 candidates) — so the next corpus answers "dead or unreached" from the record. Removal
condition for the five guide lines keyed to it, restated in falsifiable form: `blocker_forecast_multi`
> 0 with `blocker_forecast_gang` = 0 across two more corpora.

### (f) `(neither dies)` gains a price clause so the guides' literal never matches (deck146 MED)
COUNTED: 18 `(neither dies` renders; **14 of them are the nested form**
`(neither dies (blocking trigger: they may gain 2))`. The blockers window has printed the gain as
a SIBLING since `#W56-B (D13)`; the attackers window never adopted it, because
`combatAttackOutcome` passed `outBlockTrigger = NULL` and the clause then nests itself.
`combatAttackOutcome` now threads the out-param, and a new pure `attackerBlockerEntry(name, P, T,
outcome, blockTrigger)` composes the candidate row — bare verdict closed, price after it:
`Perimeter Captain (0/4) (neither dies) (blocking trigger: they may gain 2)`.

⚠ Guide-literal check, both directions: this RESTORES `"(neither dies)"` for deck123 l.187,
deck152 l.58/l.91 and keeps the open `"(neither dies"` form deck146 l.113/125/145 uses. It would
have BROKEN four guides that quote `"(blocking trigger: they gain N)"` / `"(blocking trigger:
they may gain 2)"` verbatim (deck130 l.177, deck146 l.128 + l.248, deck152 l.72), because
`#W56-B`'s hand-out path appends `", this combat"`. That suffix is now gated on
`outBlockTrigger && !attackerSeat`: the blockers window's brace form is byte-identical to today,
and the attacker line prints the exact words the four guides quote.

### (g) band rows still 1,425 B (BD-6 FAIL) and 18 unusable counter rows (deck152 LOW) — ONE defect
COUNTED: **6 Intrepid Adversary band windows, 120 `add N counters` rows, 19,782 B.** Three
collapsed to 54-56 B rows; **three collapsed nothing** and ran **5,682 / 6,100 / 3,330 B**, with
18 rows each reading `... pays for 2 of them and stops` (`152v125` s24, `152v162` s21,
`152v146` s24). The two findings are the same defect.

`payRepeatCollapse`'s identity test demanded the SAME `paid[i]` on **every** add-N row. But a
rung the mana reaches in full pays its own N: at a ceiling of 2, `add 1` pays 1 and every capped
rung pays 2, so the test returned -1 and the whole menu printed long. The test was effectively
"ceiling <= 1" — which is exactly why the three ceiling-0/1 windows collapsed and the three
ceiling-2 windows did not. The second loop already implemented the right semantics (`n <= common`
-> its own row); only the first loop's guard was wrong. It now skips a fully-payable rung
(`paid[i] >= n`) instead of failing the menu. **At a ceiling of 0 or 1 it selects exactly the
rows it selected before, byte for byte** (PARSETEST pins this). At the corpus's ceiling of 2 it
collapses 17 rows and keeps the two the mana reaches — an expected ~3.9 kB off each of those
three windows and ~10.4 kB over the corpus. No row is removed, renumbered or made unanswerable;
`req.optionTexts` (the staleness key) and the answer index are untouched.

### (h) 435 past-turn `- Paid ... with` lines survive the fold (BD-9 FAIL / engine LOW-1)
COUNTED, and the branch is named: **467 of 467 surviving lines** in the last prompt of each of
the 42 seats reach `narrationFoldPaidSources`'s final guard and are skipped by
`if (o.str().size() >= L.size()) continue;`. ` (paid with 2 sources)` is **exactly as long** as
` with Plains #1; Swamp` (the corpus's own `deck146v125` s35 line) and ` (paid with 1 source)` is
LONGER than ` with Plains #1`. The guard is right — a fold must not grow a line — the four words
in front of the count were the cost. The replacement is now ` (N sources)`, which is shorter than
every source list it replaces: **466 of those 467 fold, 2,374 B off one prompt per seat**, and
the 435 multi-source lines the finding names are all inside that. No guide keys on the old
literal (checked against all seven live `*_strategy.txt`). The current turn is still untouched.

### (i) two literals for one tap fact (deck152 LOW)
COUNTED: `{tapping ` renders in 23 prompts, `{paying this taps: ` in 19, and `152v162` s25 prints
BOTH on one row. `tapCostBeforeCombatClause` now composes the SAME clause from
`paymentTapsClause`, with the same restriction words:
`{paying this taps: Katilda, Dawnhart Prime - it cannot attack this turn}`. The surviving literal
is the one deck152's live guide quotes (l.37); no guide quotes `{tapping ` (checked). Because
both clauses now share a spelling, the self-tap clause is suppressed when the payment clause
already names that source, so no row prints the sentence twice. `{tapping ` stays on
`stripNarrationDecoration`'s list — older narration still carries it.

---

## 2. GATE

| gate | base (`169ee41f3`) | this branch |
|---|---|---|
| build | — | clean link, MAKE_EXIT=0, binary 15:43 |
| PARSETEST | 5272 passed, 0 failed | **5310 passed, 0 failed** (+38) |
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1277, 0 failed | **1277 tests, 0 failed** |
| AI tests | 67, 0 failed | **67, 0 failed** |
| `==Test Failed !==` / `==Test timed out` | 0 / 0 | **0 / 0** |
| `tools/check-ctor-init.py` | — | **OK** (8 new members, all in the init list) |
| U+FFFD in the diff | 0 | **0** |
| killed under the 4G cap | — | **none, on any run** |

Logs: `~/.gatelogs/w69-BI-parsetest.log`, `~/.gatelogs/w69-BI-suite.log`.
Three existing cases were UPDATED, not deleted, because a string they pin moved
(`#W49-T D11` the tap clause, `#W68-BD MED` the fold's literal and its measured saving); each
keeps its original assertion.

### RED on base — measured, not asserted

Seven mechanisms reverted to their wave-68 behaviour with the new cases already in place
(`transportRecordField`/`attemptMsField` back to conditional; the band guard's fully-payable
skip disabled; the fold's literal back to `(paid with N sources)`; the tap clause back to
`{tapping X now:}`; the repeat row's placeholder sentence removed; `repeatAnnotationCount`
returning -1; `attackerBlockerEntry` dropping the sibling), rebuilt and freshness-checked:

**`=== self-test: 5295 passed, 15 failed ===`** — 15 RED, 0 with the mechanisms in:

| item | RED cases |
|---|---|
| (0) transport / attempt_ms on the ordinary path | 2 |
| (c) the repeat row's placeholder sentence | 1 |
| (d) the repeat annotation's count | 2 |
| (f) the composed attacker entry | 1 |
| (g) the band at a ceiling of 2 | 2 |
| (h) the past-turn payment fold | 3+2 (2 are wave-68's, re-pinned) |
| (i) one tap literal | 2 |

The file was restored byte-for-byte from the pre-neuter copy
(`~/.gatelogs/w69-BI-AIPlayerGPT.cpp.keep`) and rebuilt to 5310/0 before the gate ran.
The wider RED is the corpus itself, produced by the base binary: **0 `transport` and 0
`attempt_ms` fields on 2,414 records**, **0 `async_drops` on 42 gameends**, **2,727 hold
suppressions with no class**, **2 `repeat_count_reask` on a copied placeholder**, **4 annotated
takes with no stamp**, **0 GANG BLOCK renders with no census to read it from**, **14 nested
`(neither dies (...))`**, **three band windows at 5,682 / 6,100 / 3,330 B**, **467 unfolded
`- Paid ... with` lines**, **two spellings of one tap fact on one row**.

---

## 3. PREDICTIONS (falsifiable, for the wave-69 corpus)

1. **transport / attempt_ms.** EVERY decision record whose `latency_ms >= 0` carries BOTH a
   `transport` field and an `attempt_ms` list; every record with `latency_ms == -1` carries
   NEITHER. On a clean corpus every `transport` reads exactly `curl=0,http=200,empty=0` and every
   `attempt_ms` has ONE entry equal to `latency_ms`. **Falsifier**: one record with a latency and
   no `transport`; or a single-attempt record whose one leg differs from `latency_ms`.
2. **async_drops.** Every gameend record carries `async_drops`, and the sum over the 42 gameends
   equals the sum of the per-decision `async_drops` fields. **Falsifier**: the two totals differ
   (that would mean drops are being taken on a seat whose record never lands).
3. **hold classes.** `hold_windows_skipped_priority + hold_windows_skipped_cast ==
   hold_windows_skipped` on all 42 gameends. **Falsifier**: any seat where they do not sum.
4. **repeat row.** `repeat_count_reask` is **0** (it was 2). **Falsifier**: one reply that takes
   the repeat row with the letter N still in its coded line.
5. **repeat annotation.** `repeat_annotated_takes` is non-zero wherever a `[repeat: activated`
   row is taken, and every such record carries `repeat_annotated_take(n=N)` in its parse note.
   A second, weaker prediction I do NOT claim: that the count falls.
6. **blocker forecast.** `blocker_forecast_rows` is non-zero, and `blocker_forecast_gang` is
   non-zero **iff** some window offered an attacker no single listed blocker kills. **Falsifier
   for the emitter being dead**: `blocker_forecast_multi` > 0 with the gang need >= 2 and
   `blocker_forecast_gang` = 0.
7. **(neither dies).** **0** prompts contain `(neither dies (blocking trigger` (was 14), and the
   closed literal `(neither dies)` appears on every attacker candidate whose verdict is that and
   whose price is a blocking trigger. `(blocking trigger: they` still renders — the four guides'
   literal is unchanged. **Falsifier**: one nested render, or one attacker line reading
   `blocking trigger, this combat`.
8. **band rows.** Every Intrepid Adversary window is **under 1,900 B** of option text (BD-6's
   own bar), every rung still printed and numbered, and no `named_row_reask` /
   `distinct_index_reask` / `unparsed_reply` names a rung inside a band. **Falsifier**: a band
   window above 2,400 B, or an answer landing on the wrong engine index.
9. **the payment fold.** **0** past-turn `- Paid ... with A; B` lines survive (was 435
   multi-source, 2,720 total), every folded line reads `(N sources)`, and no CURRENT-turn payment
   is folded. **Falsifier**: either.
10. **one tap literal.** `{tapping ` renders **0** times in any prompt (was 23) and no row
    carries two tap clauses. **Falsifier**: one render of the retired spelling.

---

## 4. WHAT I DID NOT VERIFY

* **No live model probe and no corpus run.** Everything is proven at the PARSETEST seam against
  the corpus's own numbers, or by the suite. Nothing here is verified end to end against a
  decision a model made differently.
* **The nine new gameend fields have no PARSETEST reach.** They are members incremented at one
  site each and serialised in `logGameEnd`; what PARSETEST proves is the pure functions beside
  them (`repeatAnnotationCount`), not that `logGameEnd` runs. Predictions 2, 3, 5 and 6 are
  where a mis-wire shows. **This is the weakest evidence in the lane.**
* **The blocker-forecast census counts what the RENDERER did, not what the board held.** A
  window in which the tag was suppressed upstream (menace clearing, empty candidate set) is not
  counted at all, by design — `blocker_forecast_rows` is "rows that printed the tag".
* **MED-5's per-window LINE is not shipped**, only the class split. One record per suppressed
  window is 2,727 extra records per corpus and a schema decision (a new `kind`), which is more
  than this item's "one gameend counter per suppression class" asked for; the counters make the
  split falsifiable, they do not make the individual window recoverable.
* **The band fix is proven on the SELECTION, not on a rendered menu.** PARSETEST pins
  `payRepeatCollapse`'s band, keep row and byte-identical behaviour at ceilings 0/1; the shortening
  itself happens at the caller in `chooseMenuAction`, which needs a DecisionRequest and is
  code-reviewed only (the same limit lane BD recorded for the same block). Prediction 8 is the check.
* **`(neither dies)` is fixed on the ATTACKERS line only.** The blockers window already printed
  the sibling form; the bbKind==2 branch's `- PLUS its becomes-blocked trigger` tail is untouched.
* **The tap-clause de-duplication is a substring test** (`paidTapsClause.find(displayName)`).
  A source whose display name is a substring of another tapped source's name would suppress the
  self-tap clause; I did not construct that case, and no corpus row shows it.
* **Pre-existing guide/render mismatch found while checking literals, NOT fixed (out of lane):**
  `deck126_strategy.txt` l.106 quotes the blockers-window brace form as
  `"{blocking trigger: you may gain 2, ...}"`, but that form has printed
  `{blocking trigger, this combat: you may gain 2, ...}` since `#W56-B (D13)`. That literal has
  not matched for eleven waves. It is the deck reviewer's guide to fix; I am recording it because
  I had the render and the guide side by side.
* **`repeat_annotated_take` is stamped on the record whose answer stands**, judged by
  `fallback` being empty. A record written by a path that leaves `fallback` empty AND does not
  execute would stamp falsely; I read the call sites and found none, but that is code review.
* **Nothing was checked against the two known concurrency-only failures** (`lifeline`,
  `merrow_reejerey`) or the intermittent `intrepid_adversary_repeated_payment`: the gate ran
  single-threaded and read 0 failures.
