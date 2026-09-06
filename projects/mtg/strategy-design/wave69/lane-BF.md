# Wave-69 lane BF — K2 / K3: the cap that ate the answer, the reversal nobody sees

Base: `master 169ee41f3` (wave-69 step-one brief), branch `w69-lane-BF`. Baselines on this tree:
PARSETEST **5272 / 0 failed**; suite `WAGIC_TESTSUITE_THREADS=1` **1277 / 0 failed**, **67 AI / 0**,
**0 timeouts**.

Every number in §1 is a read-only python pass over
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/` (42 seat JSONLs, 2,414 records, 2,282 of
them decisions, binary `9be85773b`) and was done BEFORE any code was written. Nothing here is
inferred from review prose.

---

## 1. MEASUREMENT (done first; it re-sized the fix twice and overturned half the brief's Ask)

### (a) Per seam: the bytes a decision NEEDS, where the cap bit, and what it cut

`needed bytes` = the reply's first non-blank byte through the end of its PLAN line (the same span
lane BA measured). A TRUNCATED reply is CENSORED — its true need is unknown and at least its cap —
so the distribution is taken over the **non-truncated** replies, which is the only uncensored
sample there is. `cap now` is the wave-68 set B.

| seam | n | n clean | cap now (tok) | needed p50 | p90 | p99 | p99.5 | max | truncated | of those: no label | no PLAN |
|---|---|---|---|---|---|---|---|---|---|---|---|
| ask | 1753 | 1135 | 640 | 325 | 602 | 1,566 | **1,768** | 2,301 | **46** | 4 | 5 |
| priority | 359 | 233 | 768 | 319 | 682 | 1,537 | **1,689** | 1,942 | **10** | 1 | 1 |
| attackers | 85 | 83 | 768 | 284 | 648 | 1,960 | **1,960** | 1,960 | **1** | 0 | 0 |
| blockers | 28 | 26 | 896 | 276 | 418 | 578 | **578** | 578 | **0** | 0 | 0 |
| discard | 41 | 38 | 384 | 279 | 492 | 961 | **961** | 961 | **1** | 1 | 1 |
| reveal | 9 | 9 | 384 | 224 | 383 | 383 | **383** | 383 | **0** | 0 | 0 |
| bottom | 7 | 7 | 384 | 343 | 622 | 622 | **622** | 622 | **0** | 0 | 0 |
| **all** | **2,282** | **1,531** | | **274** | **552** | 2,045 | 2,391 | 3,040 | **58 (2.54%)** | **6** | **7** |

**What each cap bit cut.** Of the 58 truncations, **52 had already written their coded label** —
the cap cut deliberation, not the answer; their `post_answer_overrun` runs 2,013-2,973 B, i.e. the
model answered and then wrote another two kilobytes. **6 had no label at all** (ask 4, priority 1,
discard 1) and those are the six decisions the cap could lose. **7 lost the PLAN line.** So the cap
is not eating decisions at a rate its byte count suggests: it is eating post-answer prose, and its
one true casualty class is the label-less reply, which is exactly what a re-ask fixes.

### (b) The bytes-per-token constant is no longer an assumption

Lane BA sized set B at an assumed ~3.5 B/token and flagged it as unverified. `max_tokens` is now on
every record, so it is measurable on the 58 replies that reached their cap: **median 3.605, mean
3.558, p10 3.15, p90 3.97** (ask 1.53-4.29, priority 3.02-3.96). BA's 3.5 was right to within 3%.
A cap must be sized at the **worst case 3.15 B/token**, not the median — a dense reply reaches the
cap sooner in bytes.

### (c) The re-size

Tokens a seam must be allowed = (needed p99.5 + one correction line ~180 B) / 3.15:

| seam | need p99.5 + correction | tokens required | cap now | **cap after** | slack at 3.15 B/tok |
|---|---|---|---|---|---|
| ask | 1,948 B | 618 | 640 | **768** | 68 B -> **471 B** |
| priority | 1,869 B | 593 | 768 | 768 | 549 B |
| attackers | 2,140 B | 679 | 768 | 768 | 279 B |
| blockers | 758 B | 241 | 896 | 896 | 2,064 B |
| discard | 1,141 B | 362 | 384 | **512** | 68 B -> **472 B** |
| reveal | 563 B | 179 | 384 | **512** | 1,050 B |
| bottom | 802 B | 255 | 384 | **512** | 811 B |

Only two seams — ask and discard — cleared their own need by under 70 bytes on the worst-case rate,
and the corpus's ONE lost decision was a discard. Those two rise; the small seams rise together
because they share one budget and one correction wording. **Nothing is tightened.** The brief's
warning is taken literally: the -46% generation headline is ~90% the short system prompt and ~9.5%
the caps (engine seat §2(9)), and what the caps buy is the variance guarantee (max reply
10,951 -> 3,040 B, max latency 1,233 -> 315 s). At 768 tokens the ask seam's worst case is ~3,050 B,
still 3.6x below the wave-67 tail, so the guarantee survives the raise.

### (d) K3: WHERE the reversal evidence actually is — the measurement that overturned the mechanism

`decision_reversed_in_prose` fired **22 times** (ask 16, priority 5, blockers 1, attackers 0) and
`plan_choice_conflict` **25** (the brief's "44" is the corpus's total fallback count, 44). For each
of the 22 I re-ran both halves of `planSaysPassThisWindow` and asked which half saw it, in which
region, and what the next record answered:

| where the evidence was | firings | re-ask returned the SAME answer | returned a different one |
|---|---|---|---|
| **correction zone** (between the coded line and `PLAN:`) | **0** | — | — |
| PLAN body, sentence-opener half | **16** | **12** | 4 |
| the PLAN line's own cue ("this window: pass") | 4 | 3 | 1 |
| (my re-implementation did not reproduce) | 2 | 2 | 0 |

**The correction zone was empty on all 22.** Every firing came from the plan body, and the
opener-half sentences are plan prose about a LATER window — "I will pass to combat", "We pass to
Main 2", "I will pass to second main phase" — 12 of whose 16 re-asks came back with the identical
answer. That is the 6.3x fallback rise the brief names, and it is 12 round trips bought for nothing.

**And it missed the one that mattered.** `123v162 s34` answered `CHOICE: 5 (Cast Damnation)` and
then wrote *"You should NOT cast Damnation. You should attack."* — no pass-opener matches that
sentence, because it is not a pass: it is a NEGATION of the row just taken, and it NAMES the row.
Row 5 swept deck123's own 21 Humans one turn before lethal.

I built that predicate and measured it over the corpus's **2,041** ask/priority records with a taken
row before wiring it: it fires **3 times** — `123v162 s34`; `146 s14` ("so I should not cast Soul
Shatter", already carrying `plan_contradicts_noop_row`); `130 s110` ("I cannot cycle Stone Rain",
already carrying `plan_choice_conflict`). **One new round trip per corpus, and it catches the
deciding decision.**

### (e) One thing the brief's Ask would have broken, and what I did instead

The brief asks for the predicate to read only the correction zone. Taken alone that is a **deletion**
of a mechanism that changed 4 answers, and it opens no replacement channel — the zone has zero
traffic. So the zone restriction ships **paired** with two things that give the zone traffic and
give the engine an evidence-bound reader: the protocol now says a correction must stand BEFORE the
PLAN line, and the announcement openers the protocol itself asks for (`CORRECTION:`,
`Re-evaluating:`, `Actually,`, `On second thought,`) are added to the predicate's lead table so a
correction that arrives in the shape the protocol asked for is actually read. The 4 answer-changing
firings are a real cost and are priced in prediction 4 below.

---

## 2. WHAT SHIPPED

Files: `projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/include/AIPlayerGPT.h`. All comments tagged
`#W69-BF (K2)` / `#W69-BF (K3)`. `git diff | /usr/bin/grep -c $'\357\277\275'` = **0** after every
edit; both files are ASCII/LF and were byte-spliced with python `rb`/`wb` regardless.

### K2a — the caps, re-fitted (`gptSeamMaxTokens`)

ask 640 -> **768**, discard/reveal/bottom 384 -> **512**; priority 768, attackers 768, blockers 896
unchanged. The table's comment carries §1(b)/§1(c) verbatim so the next wave re-fits from numbers
rather than from the headline. The configured value / `WAGIC_GPT_MAXTOKENS` is still the CEILING and
`WAGIC_GPT_SEAMTOKENS=0` still disables the mechanism whole.

### K2b — the truncation re-ask at EVERY seam

Lane BA wired it at ask/priority, lane BE R4 at attackers/blockers, and left discard/reveal/bottom
"on the grounds that no truncation was expected" there. New pure helper
`smallSeamTruncationReaskLine(maxTokens, wanted)` (one wording, because all three answer with `PUT:`)
quotes the cap that was actually used, states that nothing has been chosen yet, and asks for the
label first. Wired at all three on ONE shared latch keyed on the ask TEXT
(`mSmallTruncReaskKey`/`mSmallTruncReaskLine`), so it is one arm per window and cannot spend the
discard seam's distinct-index arm. Never Baka, no card chosen, no row withheld; the re-ask unwinds
the seam the way that seam already unwinds (`return 1` / `return 0` / `PREGAME_PENDING`).

### K2c — `reply_truncated_answer_changed`, and the honest third case

engine HIGH-2 says the metric "cannot distinguish 'the re-ask got the same answer' from 'the re-ask
got a different one'". Each re-ask now stores the coded choice the truncated reply had already named
(`mAskReaskPriorChoice` / `mPriorityReaskPriorChoice`, -1 = none) and the recovery stamps one of
three, beside the existing `_recovered`:

* `reply_truncated_answer_changed` — the executed answer differs from the truncated reply's coded line
* `reply_truncated_answer_unchanged` — it is the same
* `reply_truncated_reask_no_prior_answer` — the cut reply named none, so a completed **round trip**
  is all the record can honestly claim. **This is the s23 case**: the truncation re-ask fires only
  when `codedAnswerCount(content) == 0`, so at ask and priority the prior answer is empty *by
  construction* and `_recovered` could never have meant a recovered decision. The record now says so
  instead of implying otherwise. The small seams stamp the same class.

### K2d — the protocol: a correction stands BEFORE the PLAN line

The cap cuts from the END; s34's correction was below the PLAN line and 928 further bytes were
trimmed. The protocol already routed a correction (a repeated answer line under an announcement); it
now says WHERE, why, and in the model's own terms — and that region is the same one the engine reads
a reversal from. First-wins, the answer order, the `#W67-AV I3` section-naming fix and the 400-char
carry cap are untouched and PARSETEST-pinned as untouched.

### K3a — the reversal predicate reads the CORRECTION ZONE

`planSaysPassThisWindow` gains `untilStripped` (default `string::npos` = **byte-identical to every
shipped caller** and to every case pinned before this wave). `correctionZoneEnd(reply, from)` returns
the offset of the first LINE-LEADING `plan:` at or after the latched line ("my plan:" mid-line is not
the PLAN line); `string::npos` when the reply has no PLAN line, in which case the whole reply after
the answer IS the correction zone. `proseReversesInCorrectionZone(reply, seamVocab, from, out)` is
the new wrapper and is what all four seams call — ask, priority, attackers, blockers: one predicate,
one rule. **The PLAN-CUE half is untouched**: it reads the PLAN line's own arithmetic verdict
("this window: pass", "stop reached"), which is a different claim, fired 4 times, and is the
wave-51 `D4b` mechanism.

### K3b — `proseNegatesTakenRow`: the reversal that names the row it takes back

A declarative sentence after the coded line that opens with a negation-of-action cue and then names
the taken row is a reversal wherever it stands — it cannot be plan prose about a later window,
because it is about THIS row. Questions are rejected (a question is deliberation, not a claim) and
so are deferrals ("not yet", "not until", "next turn"). It needs a row name the model would actually
write, which `menuRowProseName` cannot supply: on a row carrying a NESTED brace group
(`{spends 4 ... Devour Flesh {1}{b} in your hand needs 2}` — s34's own row 5) its strip stops at the
inner close and yields `damnation in your hand needs 2}`, which matches nothing. New pure
`menuRowShortName(row)` cuts at the first `{`/`[`/`(` — the exact span the protocol tells the model
to copy — and cannot nest. Decline rows have no name and are exempt at every seam, as before.
Fires at ask and priority; the record stamps `reversal_names_taken_row` beside the broad
`decision_reversed_in_prose` so the census partitions by half, and the re-ask quotes the negating
sentence rather than claiming a pass the reply never wrote.

### K3c — `reversal_reask_same_answer`

When a reversal re-ask comes back with the identical coded choice, the record says so, at both
seams. The existing per-board / per-ask-key latch already means that window is never asked a third
time; what was missing was the count, and 16 of 22 is the number that makes the class worth
counting. Nothing is suppressed on the strength of it this wave.

---

## 3. GATE

Built from `<wt>/projects/mtg` with `rm -f bin/wagic && make -f Makefile.sdl -j4`; `ls -la bin/wagic`
after every make (final binary **15:43**, 58,391,744 B). `tools/check-ctor-init.py` prints
`check-ctor-init: OK (118 file(s))` before the link. Both legs ran DETACHED as
`systemd-run --user --unit=w69-BF-gate2 -p MemoryMax=4G -p MemorySwapMax=0`, waited on a foreground
done-file loop; the PARSETEST leg was also run interactively under
`systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0`.

| gate | baseline (169ee41f3) | this branch |
|---|---|---|
| PARSETEST | 5272 passed, **0 failed** | **5307 passed, 0 failed** (+35) |
| suite `WAGIC_TESTSUITE_THREADS=1` | 1277 tests, 0 failed | **1277 tests, 0 failed** |
| AI tests | 67, 0 failed | **67, 0 failed** |
| `==Test Failed !==` / `==Test timed out` | 0 / 0 | **0 / 0** |
| `git diff \| /usr/bin/grep -c U+FFFD` | — | **0** |
| kill under the 4G cap | — | none, on any run |

### RED on base

Verified by neutering ONLY the new mechanisms — `gptSeamMaxTokens` restored to 640/384,
`smallSeamTruncationReaskLine` returns empty, `correctionZoneEnd` returns `string::npos` (= the
wave-68 unbounded scan), `proseNegatesTakenRow` returns false, `menuRowShortName` returns
`menuRowProseName`, the protocol's correction-placement sentence reverted, `kLeads` back to its
seven — rebuilding, and re-running the corpus: **18 cases go RED**.

```
FAIL  #W68-BA J3 POSITIVE each seam carries its own budget, blockers the largest (it bundles)
FAIL  #W68-BA J3 POSITIVE the small seams share one budget
FAIL  #W69-BF K2 POSITIVE the ask cap clears its own p99.5 need plus a correction line at 3.15 B/token
FAIL  #W69-BF K2 POSITIVE the small seams rise together - 384 cleared 961 B by 68 B and ate a discard
FAIL  #W69-BF K2 POSITIVE the small-seam correction quotes the cap that was used and the count owed
FAIL  #W69-BF K2 POSITIVE it says the window is still open - no card has been discarded, no row withheld
FAIL  #W69-BF K2 POSITIVE the singular and the count-less (reveal) forms are grammatical
FAIL  #W69-BF K2 POSITIVE the protocol names the side of the PLAN line a correction must be on
FAIL  #W69-BF K2 POSITIVE it says WHY, in the model's own terms - the limit eats the tail
FAIL  #W69-BF K2 POSITIVE and names the region the engine now reads a reversal from
FAIL  #W69-BF K3 POSITIVE the correction zone ends where the PLAN line begins
FAIL  #W69-BF K3 MUST-NOT-MATCH "my plan:" mid-line is not the PLAN line - only a line-leading label is
FAIL  #W69-BF K3 REPRO 125v162 s19: and the correction zone is empty, so no round trip is bought for deliberation
FAIL  #W69-BF K3 REPRO 125v152 s47: a pass to Main 2 is a plan for a later window, not a reversal of this one
FAIL  #W69-BF K3 POSITIVE a reversal written where the protocol asks for it, in the words it asks for, IS read
FAIL  #W69-BF K3 POSITIVE the protocol's other two announcement openers reach the same predicate
FAIL  #W69-BF K3 POSITIVE the row's SHORT NAME is what the model writes in prose - the text up to the first brace
FAIL  #W69-BF K3 REPRO 123v162 s34: the reversal the wave-68 predicate missed, quoted back by the sentence that carries it
=== self-test: 5289 passed, 18 failed ===
```

The two `#W68-BA J3` lines are that lane's own cap pins, re-keyed to the wave-69 numbers rather than
duplicated. One case labelled MUST-NOT-MATCH goes red because it pins a `correctionZoneEnd` VALUE
(a positive in a negative's clothing) — said here rather than left to be found. **Every other
MUST-NOT-MATCH and every REPRO that asserts an unchanged behaviour passes in BOTH states**, including
`#W69-BF K3 REPRO 125v162 s19: the unbounded predicate still sees the plan body's "I will pass"`,
`#W69-BF K3 MUST-NOT-MATCH the PLAN-CUE half is untouched`,
`#W69-BF K3 REPRO 123v162 s34: no pass-verdict half sees it`,
`#W69-BF K3 MUST-NOT-MATCH menuRowProseName cannot serve here`, the question/deferral/other-card
negatives, the decline-row exemption, `#W69-BF K2 MUST-NOT-MATCH no cap is TIGHTENED`, and
`#W69-BF K2 MUST-NOT-MATCH first-wins and the carry cap are untouched`.
The file was restored **byte-for-byte** from the pre-neuter copy (`cmp` clean) and rebuilt to
5307/0 before the gate ran.

The WIDER red is the corpus itself, produced by the base binary: **58 truncations, 6 of them with no
label, 1 of those a discard whose seven cards the heuristic chose** (K2); **22 reversal firings, 0 of
them from the correction zone, 12 wasted round trips, and the corpus's deciding decision missed**
(K3).

**Corpus replies pinned VERBATIM**: `125v146 s133` (the 1,409 B discard the cap ate),
`123v125 s23` (the truncated fetch whose "recovery" passed), `125v162 s19` (the 2,282-byte PLAN
ending "I will pass" after a correct CHOICE), `125v152 s47` ("We pass to Main 2"; s48 returned the
identical `CHOICE: 1 (Cast Katilda, Dawnhart Prime)`), and `123v162 s34` head-through-negation with
its row 5 text.

---

## 4. PREDICTIONS (falsifiable, for the wave-69 corpus)

1. **K2a, the caps.** `reply_truncated` falls below **2.0%** of decisions (58/2,282 = 2.54%) and
   **no** record carries `reply_truncated` with no coded label at discard, reveal or bottom. Max
   true reply bytes stays under **3,300** (wave 68: 3,040) and max latency under **420 s** (315).
   Falsifier: if max reply exceeds 3,300 B or max latency exceeds 420 s, the raise cost more
   variance than it bought and the ask seam should go back to 640 with the re-ask kept.
2. **K2b/c, the re-ask.** Every `reply_truncated` record with no coded label carries a
   `reply_truncated_reask` beside it — **zero** bare label-less truncations at ANY seam. Every
   `reply_truncated_reask_recovered` carries exactly one of `_answer_changed` / `_answer_unchanged`
   / `_no_prior_answer`; at ask and priority all of them read `_no_prior_answer` (the branch's own
   gate guarantees it) and a single `_answer_changed` there would mean the gate has drifted.
3. **K2d, the protocol half.** At least **one** record carries a non-empty correction zone (a
   sentence between the coded line and `PLAN:`). Corpus-wide zero means the protocol words did
   nothing and K3's zone restriction is a pure deletion — in which case wave 70 restores the plan-body
   read for the OPENER half and keeps only the row-naming half, rather than spending more protocol
   text on placement.
4. **K3a, the zone.** `decision_reversed_in_prose` falls from 22 to **under 10**, and
   `reversal_reask_same_answer` is **under 4** (wave 68's shape would have produced 16). The cost
   side is explicit: the 4 firings whose re-ask CHANGED the answer came from the plan body and this
   change gives them up. If `plan_choice_conflict` drops to 0 AND prediction 3 also reads zero, the
   mechanism is dead rather than narrowed and must be reported as such, not as a clean census.
5. **K3b, the negation.** `reversal_names_taken_row` is **non-zero and under 8** (3 in this corpus's
   2,041 records; a 21-game corpus should see 2-6). **No record executes a row whose own reply
   names that row and negates it without a re-ask beside it.** If it fires more than 15 times the
   negation cue table is too loose and the corpus must be counted again before it is widened.
6. **Cost.** The re-asks this lane adds — three small seams' truncation arm plus the row-naming
   half — total **fewer than 15 extra round trips** across the corpus, against the 12+ the zone
   restriction removes, so the wave's fallback+re-ask total should FALL from 44.

---

## 5. WHAT I DID NOT VERIFY

* **No live model probe was run and no game was played.** Every claim about what the model will
  WRITE under the new protocol sentence or the raised caps (predictions 1, 3) is inference from this
  corpus, not measurement of the new build. §1 is measurement; §4 is not.
* **No suite fixture is RED on base for either item**, because neither is reachable from the `[AI]`
  harness: every changed branch lives inside the GPT seam and needs a model reply to enter. The RED
  evidence is the corpus plus the neutered-mechanism PARSETEST run; the suite gate here is a
  no-regression gate, not a proof. This is the honest gap in this lane, and it is the same gap lane
  BA declared for J3/J6.
* **The needed-bytes distribution is CENSORED and I did not correct for it.** The p99.5 figures in
  §1(a) are over non-truncated replies only; the 58 truncated ones needed at least their cap and
  possibly much more. The re-size therefore rests on a lower bound, and a seam could still be
  under-sized for a shape this corpus never completed. The mitigation is the re-ask, not the number.
* **I cannot say whether raising the discard cap to 512 would have saved s133.** That reply reached
  1,344 B without writing a label; 1,792 B might have sufficed or might not. The re-ask at that seam
  is what makes the outcome recoverable, and I have not claimed otherwise.
* **The 4 answer-changing plan-body reversals are given up and I did not check whether their new
  answers were BETTER.** Judging that needs the games, not the records. All I measured is that the
  answer moved.
* **My python re-implementation of `planSaysPassThisWindow` did not reproduce 2 of the 22 firings**
  (`146 s77`, `123 s82`) — both truncated replies, so the C++ likely read a `latchedFrom` offset my
  scan did not. The 22/16/12/4 split in §1(d) is therefore 20 classified and 2 unclassified; both
  unclassified ones returned the same answer, so the "12 of 16 wasted" figure is a floor, not a
  ceiling.
* **The BLOCKERS and ATTACKERS arms of the zone restriction have almost no corpus traffic** (1 and 0
  firings, and the 1 came from the PLAN-cue half, which is unchanged). They are scoped because the
  predicate is one predicate; their firing rate under the new rule is a wave-69 measurement, not a
  claim.
* **The reveal and bottom truncation arms have ZERO corpus windows** — 0 truncations in 9 and 7
  records. They are wired because the brief says every seam and because a pregame bottom is as
  unrecoverable as a cleanup discard, not because a window was observed.
* **`kLeads` gained eight entries and they apply to the UNBOUNDED callers too**, slightly broadening
  the legacy predicate at the PLAN-cue-less path. Every wave-51/52/56 case still passes, and no new
  corpus firing was measured for the added leads (they do not appear at any of the 22), but the
  broadening is real and unmeasured on future replies.
* **I did not measure parse cost.** `proseNegatesTakenRow` adds one more O(reply) scan at ask and
  priority; `correctionZoneEnd` adds a short one at four seams. Neither was timed.
* **I did not verify any card fact** against Scryfall or the primitives. Damnation, Emrakul, Marsh
  Flats, Katilda, Soul Shatter and Stone Rain appear here only as quoted corpus text; no claim rests
  on what any of them does.
* **Render-string stability (Amendment 332):** this lane changes no rendered option, row, tag or
  header. The only model-facing text it changes is the reply PROTOCOL (one added paragraph) and
  re-ask correction lines, neither of which a strategy guide keys on.
