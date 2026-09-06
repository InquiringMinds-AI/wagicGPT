# Wave-66 engine-narration seat

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-064038/` (42 seat JSONLs, 21 games),
binary master `9b797fb81`. Every number below is counted from the rendered `prompt`/`reply`
bytes with python; card facts checked against `bin/Res/sets/primitives/*.txt`. Nothing was
built, run or edited; no game was driven.

---

## 1. Corpus census

| | |
|---|---|
| games | **21 natural / 0 hung / 0 crashed** (42 `gameend`, 21 `won:true`, 0 `draw`) |
| records | 2317 total = 42 `gamestart` + 42 `system` + 42 `gameend` + **2191 model decisions** |
| decisions with a round trip (`prompt`+`reply`) | 2098 |
| bookkeeping-only `recovery` records | 93 (one per fallback; they carry no prompt and cost no round trip) |
| by kind | ask 1440, priority 475, attackers 80, discard 57, blockers 19, reveal 16, bottom 9, wall_miss 2 |
| model / protocol | `qwen36-35b-a3b`, one `reply_protocol_hash` `b3c0d80dc77a6c66` across all 42 seats (the H2a reorder is live everywhere) |
| **fallbacks** | `unparsed_reply` **83**, `repeat_past_stop` **20**, `named_row_reask` **6**, `reveal_stall_forced` **2**, `wall_miss_unrecorded_wall` **2** — 113 total (4.9% of 2317) |
| parse notes | `later_answer_ignored` 48, `repeat_past_stop_recovered` 18 / `_exhausted` 2, `latched_row_mismatch` 15, `decision_reversed_in_prose` 13, `plan_choice_conflict_narrowed` 13, `multi_answer_first_taken` 7, `named_row_reask_recovered` 6, `named_row_not_offered` 4, `echo_index_conflict` 4, `stale_echo_in_range` 3, `rejected_line_skipped` 2, `attack_correction_line_taken` 1, `hold_row_named` 1, `index_name_unique_name` 1, `repeat_count_zero_pass` 1, `attack_walker_target` 1 |
| stale drops | `async_drops` **59** (59 `async_drop_events`), spread over the corpus |
| transport | `transport` field present on **2** records only, both `curl=28,http=0,empty=1,connect_ms=20000,phase=wall`; **0** `transport_error` retries |
| timeouts | max `latency_ms` **900 026 ms**; `deadline_pct` max **100.0**, **0 records above 100** |
| latency | n=2096, p50 **12 s**, p90 **94 s**, p99 **457 s**, max **900 s** |
| wall misses | `wall_miss_events` 2, `wall_miss_unrecorded` 2 — both are the 900 s empty-transport records |
| HOLD | rendered on **1329** windows, plain `Cast nothing right now` on 854; hold row printed AHEAD of the plain decline on **696 of the 854** windows carrying both. Prefix-matched takes: **Hold 393 : plain decline 126**. `hold_windows_skipped` summed over the 42 seats = **3565** |
| SEGV / abort / assertion | **0** across all 21 `.stderr` files |
| wave-58/59 signatures | `empty_reply` **0**, `bad_reply` **0**, `transport_error` **0**, `deadline_pct > 100` **0**, no translog gap past the watchdog. **`reveal_stall_forced` RECURS: 2** (see HIGH-2). `wall_miss_unrecorded_wall` 2. |

---

## 2. HIGH / MED / LOW defects

### HIGH-1 — `ANSWER:` in front of the coded label: 45 of the 83 refusals, and it is the protocol's own word

**Repro.** `1788694853-ai_baka_deck162-0x560b2dde5340-vs-ai_baka_deck126.jsonl` seq **22**
(priority, T20, life 18 vs 5), reply verbatim:

```
REASONING: I have 4 draw punishers on the battlefield ... Each of those 2 draws triggers my 4
draw punishers, dealing 4 damage per draw, for a total of 8 damage. This is lethal (8 > 5).
... I will choose option 2 (Overgrown Battlement #1).

ANSWER: CHOICE: 2 (-2: destroy target creature and draw two cards with Ob Nixilis, the
Hate-Twisted targeting Overgrown Battlement #1)

PLAN: Use Ob Nixilis's -2 to destroy Overgrown Battlement #1. The opponent draws 2 cards,
triggering 4 damage per draw (8 total), reducing them from 5 to -3 life, winning the game.
```

An exactly-lethal, correctly-reasoned line, refused; `chosen_text` = `<refused: unparsed_reply>`.

**The seam.** `AIPlayerGPT.cpp:17506 findAnswerLabelLine`. Its label scan (`:17538-17541`)
advances past only `' '`, `'\t'`, `'*'`, `'#'` before comparing against
`{"CHOICE:","ATTACK:","BLOCKS:","PUT:"}`. `ANSWER:` is none of those, so the line never matches
at any of the four labels. `gptSelectAnswerIndex` then finds no head, `findPlanMarker` finds the
`PLAN:` far past 300 bytes, and the ramble guard at **`:18137`** (`if (firstPos > 300) return
string();`) returns empty -> `unparsed_reply` -> Baka.

**Why the model writes it.** `kReplyProtocol` (`:231`) now says *"Write your REASONING first,
then your ANSWER, then your PLAN"* and heads the middle section **"THE ANSWER LINE"**. The model
took the three section names as three literal labels and wrote `REASONING:` / `ANSWER:` / `PLAN:`.
Two of the three are harmless prose; the middle one is fatal. This is a composition defect
between H2a's new section naming and a parser that was never told `ANSWER:` is a heading.
It is the single largest new cost of the wave: **45 of 83 records** (ask 24, priority 19,
reveal 2), across **9 seats** — the shape did not exist in wave 65.

Cheapest fixes, in order: (a) let the label scan skip a leading `ANSWER:` (and `A:`) heading —
three lines at `:17538`; (b) stop calling the section "ANSWER" in the protocol, or add one
sentence "do not write the word ANSWER in front of the label". (a) is strictly safer: it also
catches the 1 `CORRECTION: CHOICE: 3 (...)` record (125v130 s52) and any future heading.

### HIGH-2 — 32 replies write no coded label at all, and H2a's reorder removed the salvage that used to catch them

The other shape. All 32 are COMPLETE replies (none ends mid-sentence; no `max_tokens` cut), 31 of
them carry a `PLAN:` line — the model reasoned, planned, and simply never wrote the answer line.
Verbatim, `1788694840-ai_baka_deck126-0x55e5f2ecd420-vs-ai_baka_deck146.jsonl` seq **7** (ask, T6):

```
REASONING: I have Exquisite Blood on the battlefield but lack Sanguine Bond. Rule #3 and Rule #7
Entry 2 dictate that I must cast Idyllic Tutor to find Sanguine Bond. ... I will cast Idyllic
Tutor to find Sanguine Bond.

PLAN: Cast Idyllic Tutor to find Sanguine Bond. ...
```

and `...deck162-0x560b2dde5340...` seq 8 (T8) ends `Therefore, I cast Liliana's Caress.` +
`PLAN: Cast Liliana's Caress.` with no `CHOICE:` anywhere.

**The seam is the same function, a different branch.** With no label, `gptSelectAnswerIndex`
falls to `:18132-18140`. Under the wave-65 ANSWER-FIRST order a label-less reply led with a
short head and `text.substr(0, firstPos)` salvaged it. Under REASONING-FIRST the `PLAN:` marker
is always thousands of bytes down, so `firstPos > 300` is true on every one of these 32 and the
guard returns empty. **The reorder did not create the missing label; it deleted the salvage
path for it.** Any future dial here belongs at `:18137`, not in more prose.

Two further records are correct refusals, not defects: `...deck126-0x55913c9defa0...` s17 and
`...deck123-0x55ac3cfdf640...` s142 wrote `ATTACK: A1 ...` on an `ask` (CHOICE) window, and
`expectedLabel` rightly refused a wrong-label answer. The remaining 4 are label text embedded
mid-prose (`So PUT: 2, 3 or 8, 3?`, `4. Attack: I have no Vampires.`).

**Full split of the 83:** 45 `ANSWER:`-prefixed · 32 no coded label anywhere · 4 mid-prose ·
2 wrong-label. (The brief's 33/50 split reads the same population one record differently.)

**What the Baka fallback did.** *This is where the record fails the reviewer.* On a discard
window the translog keeps the heuristic's pick (`chosen_text` = real card names, 7 records) and
the narration says *"the heuristic chose X"*. **At the CHOICE, priority, reveal and ask seams it
does not**: `choice` = -1 and `chosen_text` = `<refused: unparsed_reply>` on all 76 of them, so
the executed action is not in the record at all and has to be reconstructed from the next
window's narration. Recording the heuristic's chosen row on a fallback is a one-field change and
it is the difference between "83 refusals" and "83 adjudicable refusals" (MED-1 below).

**Games decided:** counting only windows I could trace end to end, **1 of the 83 sits on a
game-deciding decision** — `...deck125-0x55a1ef3af260-vs-...deck123` seq **130**, the
Sphinx's Revelation cast at T59 that began the deck-out described in HIGH-3 (the seat was at
44 life vs 3 and lost). **A further 31 sit in critical windows** (within 2 turns of the end, or
at a life total <= 5, or on a reply whose own text says "lethal"/"I die"), of which the sharpest
is the refused lethal at 162v126 s22 above — that seat won anyway one turn later on its draw
punishers, so the refusal cost a turn, not the game. For the remaining 51 I could NOT establish
a causal verdict, because of the missing-executed-action defect just named; I am not inferring
one. What IS measurable is the churn: **4 consecutive refusals in one turn** at
`...deck130-0x55a5b9d79c20-vs-...deck125` T6 seqs 13/15/17/19, the model writing the identical
`ANSWER: CHOICE: 1 (cycling with Spark Spray)` each time and being refused each time — 4 round
trips, ~0 decisions, in a game that seat lost 0-67.

### HIGH-3 — the LIBRARY CEILING holds back one card and ignores the draw triggers already on the stack; a seat at 44 life lost to it

**Repro.** `1788694859-ai_baka_deck125-0x55a1ef3af260-vs-ai_baka_deck123.jsonl` seq **130**,
T59 (the wave-66 lategame specimen; full prompt in `wave66/lategame-specimen.txt`). The prompt
prints, three blocks apart:

```
ON THE STACK, waiting to resolve (top resolves FIRST - you can respond now):
  1 (top): ability: Staff of Nin's Draw 1 [from your Staff of Nin] [triggered/activated ability]
  2: ability: Staff of Nin's Draw 1 [from your Staff of Nin] [triggered/activated ability]
...
Your library: 14 cards
...
1. Cast Sphinx's Revelation {u}{u}{w}{x} ... [<- best X for this cast: X=13 - largest affordable
X - ... but LIBRARY CEILING: X=14 draws 14 from a library of 14 cards: it leaves 0 cards, and
your next draw step, which you cannot decline, then draws you out. X=13 is the largest listed X
the library pays for (14 cards, holding 1 back for your next draw step, which you cannot
decline)]
```

The ceiling reserves **1** card. It should have reserved **3**: the two Staff of Nin draw
triggers on this window's own stack are as undeclinable as the draw step, and the badge's own
prompt prints them. The seat obeyed the badge exactly — seq **132** answered `CHOICE: 2 (X = 13)`
— library went 14 -> 1 (seq 133/134 render `Your library: 1 cards`), the pending triggers
finished it, and the game ended at seq 135 turn 59 with this seat at **44 life against 3** and
`won: false`. The badge is a false surface in the strict sense: it computed a safety margin,
named it, and the margin was wrong by the contents of a block printed above it.

This is the same defect family lane AQ H4 fixed for the X *life* ladder — a ladder computed
without the pending stack — applied to the *library* term instead. `pendingStackLifeLossToSeat`
already demonstrates the walk; the ceiling needs the analogous `pendingStackDrawsToSeat`.

### HIGH-4 — `reveal_wait_unexplained_secs` reads the whole wait as unexplained on 14 of 16 reveals: read-after-consume

AT MED-3 shipped this field precisely so the wave-65 false "835 s of engine stall" could not
recur. It does not work. The record's own `latency_ms` is stamped from `mLastLatencyMs` at
**`AIPlayerGPT.cpp:15348`**; **`:15414`** then consumes it (`mLastLatencyMs = -1; //consumed`);
the residual at **`:15684`** reads `mLastLatencyMs` **after** that, always sees -1, and
`revealWaitUnexplainedSecs` (`:3614`, which is itself correct) returns the whole wait.

Measured, 16 reveal records:

| seat / seq | `reveal_wait_secs` | `latency_ms` | reported unexplained | correct |
|---|---|---|---|---|
| deck126-0x55e5f2ecd420 s12 | 38 | 37 425 | **38** | 0 |
| deck126-0x560b2cac8e40 s10 | 114 | 114 340 | **114** | 0 |
| deck152-0x55b4a19f6ee0 s36 | 70 | 69 881 | **70** | 0 |
| … 11 more of the same shape | | | | |
| deck126-0x560b2cac8e40 s12 | 600 | −1 | 600 | 600 |
| deck126-0x55ac3bcc4ff0 s53 | 600 | −1 | 600 | 600 |

Corpus sum reported **1622 s**; correct sum **1200 s**; the 422 s difference is pure instrument
error. Note what the bug hides rather than invents: the 1200 s IS real. The two `latency_ms: −1`
records are the two `reveal_stall_forced` fallbacks, `reveal_wait_driver_secs` **600** on both —
a driver half that genuinely accumulated with no round trip in flight. **That is the first
measured engine-side reveal stall in the project, 20 minutes of a 127-minute run**, and it is
the wave-58/59 signature recurring. Fix the ordering (move the residual above `:15414`, or cache
the value), then the field says 1200 and points straight at those two windows.

### MED-1 — a fallback record does not say what the engine executed

Named in HIGH-2. `choice: -1`, `chosen_text: "<refused: unparsed_reply>"` on 76 of 83; the
discard seam alone records the heuristic's pick. Every "did the fallback cost the game" question
this wave — and the brief's own question (7) — is bounded by this, not by analysis effort.

### MED-2 — the worked example still points at a half-dead row

`1788694840-ai_baka_deck146-0x55e5f1b97220-...` seq **22**, the Silverquill Command mode menu:
row 1 renders `creature gains 3/3 and return creature {HALF DEAD right now: one half of this mode
has NO legal object on the board ...}` and the ask closes
`e.g. "CHOICE: 1 (creature gains 3/3 and return creature)" (a worked example of the format,
written out from row 1 of this list ...)`. AR H8 / AU R5 moved the exemplar off `rowSaysNoOp`
rows; HALF DEAD is a third bucket AQ H9 added in the same wave and `askExemplar` does not know
about it. Exactly the defect H8 was written for, one bucket over. 1 of 4 half-dead windows.

### MED-3 — 2 wall misses are 900 s empty transports

`...deck130-0x55d9b7bf94f0-...` s11 and `...deck162-0x55c39de74170-...` s9: `kind: wall_miss`,
`latency_ms` 900 026 / 900 022, empty reply,
`transport: curl=28,http=0,empty=1,connect_ms=20000,phase=wall`. curl 28 is an operation
timeout at the full wall with a 20 s connect. Both are the only records in the corpus carrying a
`transport` field and both are the only two `deadline_pct` = 100.0 records. 1800 s of a
127-minute run, 24% of the total wall-clock overrun. Not a regression (2 in 2317) but the
retry ladder never fired: 0 `transport_error` records.

### LOW-1 — `later_answer_ignored`'s `ignored_line` equals `latched_line` on 32 of 48

Recorded because AR prediction 4 forbids it (adjudicated below). Every case is a reply that
writes the SAME coded line twice — once as the answer, once again inside the PLAN prose — e.g.
`...deck126-0x55e5f2ecd420-...` s20, `CHOICE: 4 (Cast Sanguine Bond)` on line 1 and again inside
a 2641-byte trimmed plan. The stamp is correct and the answer is correct; the field is just
uninformative in exactly the half of cases it is most likely to be read.

### LOW-2 — the DRAW FORECAST remainder lags the punisher triggers by one window

11 window pairs render the same `= N x M =` product at two different life totals inside one draw
step, e.g. `...deck123-0x55c39f18d2e0-...` seqs 55/57 (`= 3 x 2 = 6`, life 7 -> 6 -> 5) and 61/62
(`= 2 x 2 = 4`, life 5 -> 4 -> 3). This is not the wave-65 re-charge: the `ALREADY been drawn`
count climbs correctly (2 -> 3 -> 4 -> 5 across seqs 53-68) and the remainder shrinks. It is that
two punishers' triggers resolve in separate windows, so life moves 1 while the DRAW count moves
0. The render is arithmetically true at both windows; only AQ prediction 1's wording is
falsified. No fix is obviously worth it.

---

## 3. Prediction adjudication

Format: **VERDICT** — evidence, N of M, file+seq.

### Lane AQ

1. **H1 — FAIL (first clause) / PASS (second and third)**
   * clause a (0 repeated identical products at differing life): **FAIL, 11 of 57** NOW-forecast
     windows. `deck123-0x55c39f18d2e0` s55/57, s61/62, s65/66; `deck130-0x55ec5f6672a0` s31,
     s51, s54. Cause and why it is benign: LOW-2 above.
   * clause b (every window after the first draw carries `ALREADY been drawn and paid for`):
     **PASS, 57 of 57** `resolving NOW` windows carry it.
   * clause c (0 `KILLS you` verdicts a life read contradicts): **PASS, 0 of 22** windows
     carrying a `that KILLS you` forecast state a remaining product smaller than the rendered
     life. The `123v162` seq-152 shape does not recur.
   * `= 0 life LOST ... from the rest of this step` with no life total and no KILLS verdict:
     rendered on **16** windows, correct shape on all (`deck130-0x55ec5f6672a0` s34, s57).
2. **H4 — UNTESTED.** Only **3** X-ladder windows in the whole corpus and **0** of them carry an
   `ON THE STACK: N damage to you` line, so no window exercised `lifeAfterPendingStack`,
   `xNetLadder`'s new base or the `ALREADY ON THE STACK` clause. 0 falsifiers, 0 confirmations.
   (The one X window that mattered failed on the LIBRARY term instead — HIGH-3.)
3. **H9 — PASS on the two clauses a window arose for; UNTESTED on the third.**
   * every modal row carries exactly one census tag: **PASS, 24 of 24 rows on 4 windows**
     (`deck146-0x55e5f1b97220` s22, `deck146-0x555bfeca02e0` s41 and two more). **0 bare rows.**
     Wave 65 rendered 6 of 6 bare. `{HALF DEAD right now: ...}` fires on 4 windows and the
     census reaches the CHOOSE_MENU seam — `modalSubjectFromScript` recovered the subject, which
     was the lane's own weakest evidence.
   * cast-row / ask-row census agreement: **PASS**, no disagreement found in the 4 pairs.
   * punisher fold (`their draw punishers take N more`): **UNTESTED, 0 firings** — no modal
     window in the corpus also carried `DRAW PUNISHERS on the battlefield: theirs`.
4. **H10 — PASS.**
   * sweep bracket: **1 of 1**. `deck125-0x55a1ef3af260` s117 renders `board sweep: THEIRS 59
     (including Thraben Doomsayer - a TOKEN ENGINE (it makes more permanents, one per
     activation)) / YOURS 0 ... THAT COUNT IS BODIES, NOT VALUE: ...`. 0 engine-bearing sweep
     markers rendered bare.
   * forced sacrifice: **3 of 3** asks (`deck123-0x55ac3cfdf640` s151/s158/s166) carry
     `NOT ALL OF THESE ARE BODIES: Thraben Doomsayer is a TOKEN ENGINE ...` and the row itself
     carries `THIS IS NOT JUST A BODY`. 0 unmarked.
   * secondary behavioural half: **PASS** — the sweep row at s117 with a named token engine on
     THEIRS **was taken** (`chosen_text: Cast Supreme Verdict ... destroys 59 of their
     creatures`), which is the behaviour the bracket was written to enable.

### Lane AR

1. **H2a — PASS (weakly) on the rate, FAIL on the counter-measurement.**
   `post_answer_overrun` **1390 of 2098 = 66.3%** (wave 65: 1598/2209 = 72.3%). The falsifier
   was "holds at or above 65%" — it lands at 66.3%, i.e. **the falsifier fires**. I record this
   as FAIL of the stated bar and a real 6-point improvement, because both readings are true and
   the bar is the lane's own. The counter-measurement the lane asked for: **3** records execute
   a first coded line that sits inside the reasoning block —
   `deck123-0x55ac3cfdf640` s160, `deck126-0x55ac3bcc4ff0` s37 and s55. The class the lane said
   "if it appears at all, the instruction is not carrying" **appeared**, at 3/2098. The far
   larger cost of the reorder is HIGH-1/HIGH-2, which the lane did not predict.
2. **H2a/2 — FAIL.** `coded_answers == 1` on **1941 of 2098 (92.5%)**, but `coded_answers == 0`
   on **89** records (the label-less shape) and `later_answer_ignored` **ROSE from 33/2209 to
   48/2098** (1.5% -> 2.3%). The prediction's own falsifier ("`later_answer_ignored` rises")
   fires.
3. **H2b — PASS.** **0** records execute a coded line that a header-form announcement within
   three non-blank lines above a later coded line retracted. `answer_replaced: true` on **0**
   records; **1** `attack_correction_line_taken` (the combat driver taking a marked correction).
   No `Correction:`/`Re-evaluating:`/`Actually,`/`On second thought,` header appears above a
   `later_answer_ignored` record's refused line. The 123v126 s36 shape does not recur.
4. **H2c — PASS on presence, FAIL on the inequality.** **48 of 48** `later_answer_ignored`
   records carry `ignored_line` (0 missing — the field ships and never leaks). But
   `ignored_line == latched_line` on **32 of 48**, and the prediction says "never equal".
   See LOW-1: benign cause, real falsification.
5. **H8 — PASS.** **0 of 1856** exemplar-bearing windows name a row the same prompt flags
   `at 0 this does nothing` / `does not apply` / `dead right now`. The 126v125 s34/s47
   `Cast Tribute to Hunger {right now: ... at 0 this does nothing}` exemplar shape does not
   recur. (The half-dead gap is MED-2, a different bucket and not this prediction's claim.)
6. **H8/2 — UNTESTED.** **0** `plan_contradicts_noop_row` stamps and **0**
   `plan_contradicts_noop_row_reask` records in the corpus. No window arose. The lane's
   counter-measurement (re-ask volume) is therefore also 0 — the churn worry is unfounded so far.
7. **MED-1 — UNTESTED.** **0** `name_over_index` records, so the new `index_name_conflict`
   pairing never had a window. `echo_index_conflict` fired 4 times (a different branch) and
   `index_name_conflict` **0** times. The 130v125 s97/s98 `Mountain #2 / #3` shape does not
   recur; the deck123 churn complaint did not materialise.
8. **MED-2 — PASS.** `planLibraryOnlyNote` renders on **109 windows across 17 seats**, e.g.
   `deck126-0x55e5f2ecd420` s9/s10: *"(note: your plan names \"Sanguine Bond\", and no
   \"Sanguine Bond\" is in your hand, on either battlefield, in a graveyard, in exile or on the
   stack right now - a copy is still in your LIBRARY ...)"*. I found **0** renders naming a card
   visible in a rendered zone (spot-checked 12). The 126v125 s45-s57 shape (an absent card
   re-served for 15 turns unmarked) does not recur — where it recurs, the note is printed.
9. **MED-2b — PASS.** **15** truncation notices carry the dropped-correction clause; 0 notices
   carry it without a retraction header opening the dropped span (spot-checked 6). The lane's
   own caveat is visible: some of the 15 are `planScratchpadCut` firings, not carry-cap cuts.

### Lane AS

1. **H3a — PASS.** **20** `repeat_past_stop` refusals, **18 recovered / 2 exhausted**; the
   second answer executed as given in all 20 (`deck123-0x55ac3cfdf640` s33/34, s47/48, s52/53,
   s55/56, s62/63, s64/65, s67/68, s77/78, s79/80, s81/82, s84…; `deck123-0x55940c9233e0`
   s41/42; `deck123-0x55c39f18d2e0` s43/44). **0** counted takes overshoot a stated stop with no
   stamp (searched every record with `repeat_n >= 1`, n=9, plus every reply carrying both an `M`
   and a `stop` number). The `{right now: M=.., your stated stop=..}` clause renders on **43**
   windows. The `123v162` sixteen-window overshoot shape does not recur.
2. **H3b — UNTESTED.** **0** `loop_autopass` records and **0** `Their life LOOP ran N links`
   narrations; the `[LOOP RUNNING: ...]` prompt note rendered **once**. As the lane itself said,
   0 is consistent with the fix, not evidence for it. No falsifier: no run of `Cast nothing`
   answers on a two-decline menu under a proven loop was found.
3. **H7a — PASS.** Longest run of consecutive same-seam windows whose row set differs only in
   bracketed/braced digits (or not at all): **4** (`deck130-0x55a5b9d79c20` s80,
   `deck123-0x5576fc53b2f0` s8); only **3** such runs of length >= 3 in the whole corpus.
   The `152v162` 9-window / 334 s draw-step class and the `130v126` 19-window casting run do NOT
   recur at any seat. `hold_windows_skipped` totals **3565** across 42 seats — the latch is
   doing the work. Cross-check on AU R1 below.
4. **H7b — PASS, decisively.** Hold takes **393** vs plain-decline takes **126** (3.1 : 1).
   Wave 65 was 4 : 182 and 29 : 213 on the two seats the item was written for. Decisions per
   game did not rise: 2191 decisions over 21 games = 104/game against wave 65's 105/game.
   The falsifier (plain decline outnumbering the hold by more than 20:1) is nowhere near.
5. **MED unique-name — PASS (N=1).** **1** `index_name_unique_name` stamp, **0**
   `index_name_conflict` re-asks anywhere in the corpus. The `123v162` s29/s68/s111 shape does
   not recur. Thin evidence, but no falsifier.
6. **MED no-row-0 — PASS.** `kCastNoRowZeroFact` renders on **854** casting windows and there
   are **0** `no_pass_reask` records and **0** `CHOICE: 0` answers to an `ask`-kind window.

### Lane AT

1. **H5 (order) — PASS.** **16 of 16** discard asks holding a `{spare:` row render row 1 as a
   spare row, with no `{right now:`-priced row before any spare, every non-spare row carrying
   `{NOT spare: a real card, not a surplus land}`, no row carrying both tags or neither, and
   `DEFAULT ANSWER: discard the spare land.` as the first line after the CR 514.1 header with a
   count matching the spare rows. (The one apparent count mismatch,
   `deck123-0x55c39f18d2e0` s47, is a collapsed copy group `1-3. Tundra (copies 1-3 of 3)` —
   5 spare cards on 2 printed lines; the line's count of 5 is right.)
   Outcome half: **FAIL** — spares-only discards **5 of 16** (31%) against wave 65's 6 of 12
   (50%). The render is fixed; the behaviour did not follow.
2. **H5 (indices) — PASS.** **0 of 57** discard records name a `chosen_text` absent from the row
   its number maps to. The composed permutation holds.
3. **H6 (target rows) — PASS.** `[summoning sick - cannot attack this turn` renders on **371**
   windows. **0** bare summoning-sick battlefield target rows found controlled by the active
   player; **0** tags on hand/graveyard/library/exile targets or on the non-active player's
   board (spot-checked 20). The `152v162` s35-37 shape (three counters onto a sick body under a
   plan naming it as an attacker) does not recur.
4. **H6 (attackers window) — PASS.** `attackersHeldSickLine` renders on **40** windows, e.g.
   `deck146-0x55e5f1b97220` s19: *"NOT offered above and NOT able to attack this turn (summoning
   sick) - Silverquill Silencer. A plan that names one of them as an attacker THIS turn cannot
   be executed. Of these, Silverquill Silencer can attack from your NEXT turn on if nothing else
   changes."* **0** names appear in both the A-rows and the held line. **0** lines carry the
   retracted `entered this turn without haste` claim (AU R4, below).
5. **deck130 (a) — PASS.** `CAUTION - some usable mana sources DAMAGE YOU` renders on **149**
   windows; on those, **39** rows carry `paying this costs you NO life` and **15** carry
   `paying this costs you N life`; **0** bare priced rows found on a caution board
   (`deck130-0x5564dc831d80` s11/15/17/20/21/25, `deck130-0x55ec5f6672a0` s9/14/25/37/39/59/79,
   `deck130-0x55a5b9d79c20` s60/62/64/66/71/72/80/81/82). Outcome: the "no safe play exists"
   pass streak on a Talisman board does not recur — `deck130-0x55a5b9d79c20` cast Talismans at
   s60-s71 on exactly that board.
6. **deck130 (b) — UNTESTED.** `Cast Card Normally` rows render on **134** windows, but I found
   **0** of them for a card the cast menu would have priced (a single-`damage:` spell or a flat
   P/T drop) — no `castKillVerdictNow` firing and no falsifier. The `130v152` s77 shape did not
   arise.
7. **deck146 — PASS.** `{also a THREAT: this land ANIMATES ...}` renders on **14** land-drop
   rows, every one for Hive of the Eye Tyrant or Lair of the Hydra, and reads
   `cheapest rung {3}{b} for 3 power` on the Hive rows exactly as predicted
   (`deck146-0x55d9b68c3270` s3/s4/s19, `deck146-0x555bfeca02e0` s3/s4/s7,
   `deck146-0x558da8ba0140` s3/s5/s35, `deck146-0x55940b5ed1e0` s16,
   `deck152-0x55b4a19f6ee0` s5). **0** rows for a land with no animate rung carry it. Card fact
   checked against the primitive: Hive's `becomes(Creature Beholder^3/3^black^menace)` at
   `{3}{b}` — the lowercased match the lane added is what makes it fire. Outcome half:
   the 146 seat's manland decline rate — Hive was PLAYED at s3/s4 rather than declined.
8. **deck162 — PASS.** `{reserve: this row is INSTANT SPEED ...}` renders on **31** rows. Every
   one checked (12 of 31) is an instant-speed card, on the seat's own turn, at a phase before
   its first main, naming a sorcery-speed stranded card whose `needs M` exceeds the row's
   `leaves N` (`deck162-0x55ec5e34e0a0` s9/12/16/17/18/19 naming Dictate of Kruphix;
   `deck146-0x557f1bc54090` s8/13/19/24/25; `deck126-0x560b2cac8e40` s30). 0 violations of the
   four gates.
9. **MED-3 — FAIL.** Every reveal record carrying `reveal_wait_ticks` also carries
   `reveal_wait_unexplained_secs` (16 of 16), so half the prediction holds — but the value
   equals `max(0, reveal_wait_secs - latency_ms/1000)` on only **2 of 16**, and the corpus sum is
   **1622 s**, not "under 20 s". Root cause and the true figure (1200 s, two real engine-side
   stalls) in HIGH-4. This is the falsifier the lane wrote, and it fired for two reasons at once:
   a broken instrument AND a real stall it was pointing at.

### Lane AU

1. **R1 — PASS on the observable side; UNTESTED on suppression.** **9** windows show a hold
   taken and then the same seam re-asked with a bracketed non-life-total number changed
   (`deck130-0x55ec5f6672a0` s49, s73; `deck123-0x55c39f18d2e0` s31;
   `deck123-0x55ac3cfdf640` s127; `deck125-0x5583a84a5320` s123, and 4 more) — i.e. in 9 of 9
   such cases the window **re-opened**, which is R1's whole claim. What the translog cannot show
   is the negative: a window the latch suppressed. `hold_windows_skipped` = 3565 gives the size
   of that blind set but not its contents. The falsifier as worded ("two consecutive records …
   differ in a bracketed count that is not a life total") is unsatisfiable as a falsifier —
   a second record at the seam is evidence the hold did NOT survive.
2. **R2 — UNTESTED.** `drawsUnattributedClause` (`you also drew N cards this step from a spell
   or an ability, which this forecast never counted`) fires **0** times in 2098 windows: no
   cantrip, cracked Clue or ETB draw resolved inside a rendered draw step in this corpus. **0**
   under-charging forecasts found either (this is also the brief's question 6: no DRAW FORECAST
   under-charges; every remainder I checked matches its `ALREADY drawn` count). The lane's own
   "weakest evidence" is still unmeasured.
3. **R3 — PASS.** `TOKEN ENGINE` renders on **4** windows, on Thraben Doomsayer only, whose
   primitive line is `{T}:token(Human,...)` — a cost head, correctly repeatable. **0** bodies
   whose only `token(` is an `@movedto(...)` ETB are named an engine.
4. **R4 — PASS.** **0 of 40** held-sick lines carry `entered this turn without haste`, and every
   one that promises a NEXT-turn attack names the creature individually
   (`Of these, <name> can attack from your NEXT turn on if nothing else changes`) rather than
   promising it for the whole set. No Defender, CANTATTACK or attack-cost body appears in a
   next-turn promise (the 40 lines name 12 distinct creatures, all checked against their
   primitives).
5. **R5 — UNTESTED for the all-dead branch, PASS otherwise.** **0** all-dead menus arose, so the
   FORMAT-only sentence (`every row on this list does nothing right now`) never rendered. On the
   1856 ordinary exemplar windows, 0 exemplify a `rowSaysNoOp` row (= AR prediction 5). The
   half-dead gap in MED-2 is a bucket R5 does not cover.

**Tally: PASS 22 · FAIL 7 · UNTESTED 8.**
FAILs: AQ H1 clause-a, AR H2a (rate bar), AR H2a/2, AR H2c (inequality half), AT H5 outcome
half, AT MED-3, and — counted once, not per lane — the composition defect HIGH-1/HIGH-2 that no
lane predicted.
UNTESTED: AQ H4, AQ H9 punisher fold, AR H8/2, AR MED-1, AS H3b, AT deck130(b), AU R2,
AU R5 all-dead branch.

---

## 4. Verification of shipped fixes at the LIVE seam

Every PASS above is a count over rendered corpus bytes at the seam the corpus exercised, not a
PARSETEST re-run. Where a mechanism had **no live window** I wrote UNTESTED rather than reading a
fixture verdict across. Three lane self-flagged weak spots came out well and are worth
recording: `modalSubjectFromScript` **does** recover the CHOOSE_MENU subject (AQ H9's weakest
evidence — 4 live windows, 24 tagged rows); `holdRowLine()`'s insertion ahead of the plain
decline **does** ship and the consumer indices hold (AS H7b, no misrouted answers in 1329
hold-rendered windows); and `composeRowOrder`'s permutation is intact on all 57 discard records
(AT H5's unwired emitter). The three that stayed weak are AQ H4, AU R2 and AS H3b, all UNTESTED
for want of a window.

## 5. What I did NOT check

* **Which row Baka actually took on 76 of the 83 refusals** — the record does not contain it
  (MED-1). Every causal claim about the fallbacks is bounded by that, and I did not infer one.
* **Guides.** I read no `deck<N>_strategy.txt` and made no strategy judgement; every misplay
  above is classified as a surface defect or left unclassified.
* **The two known concurrency-only suite failures** and the intermittent — no suite was run.
* **Card-fact verification** was done for the cards named in H10, deck146 and AU R4 (primitives
  only, Scryfall not consulted — no disagreement arose that needed it). Cards named in the
  discard, reserve and payment items were not individually verified against Oracle text.
* **`async_drops` (59)** were counted, not characterised: I did not check whether any dropped a
  window that mattered.
* **The `latched_row_mismatch` (15), `decision_reversed_in_prose` (13) and
  `plan_choice_conflict_narrowed` (13) classes** were counted only. None is a wave-66 mechanism
  and none had a lane prediction attached, but 41 records is not nothing and no one has read them.
* **HIGH-3's fix** is described from the shape of AQ H4's stack walk; I did not read
  `xNetLadder`'s library term to confirm where the ceiling is computed.
