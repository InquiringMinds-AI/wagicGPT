# Wave-79 lane DB — render truth, bytes and tools (T8, T10, T11, T15, T17, T1 measure)

Base `06fad1449` (= master). Worktree `worktrees/lanes/w79-DB`, branch `w79-lane-DB`.
Corpus read (never written): `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-125420-final`
(42 seat logs, 2,629 prompts). Every fixture below is bytes taken off a record's own
`prompt` field or a card's own `text=`/`auto=` line in
`bin/Res/sets/primitives/borderline.txt` / `mtg.txt`.

Gate: PARSETEST **6698 passed / 0 failed** (base 6642/0, +56).
Suite THREADS=1: **1291 tests (0 failed), 76 AI tests (0 failed)**.

---

## T8 HIGH — a held counterspell reads as "not in hand"

### (a) the `[no cast row now: …]` tag on an instant

**RED, on the corpus.** `125v162` seq 52 (t19) prints, twice, on the hand line:
`Fall of the Gavel (copy 1 of 2 in your hand) {3}{u}{w} [instant] [no cast row now: it
must have a target and there is no legal target on the board]`, and the reply reasons
*"I have no counterspells in hand right now. So gate is open at any leaves-count."*
The seat tapped to 2 and lost on turn 21. The pin asserts the wave-78 tag **byte-exact**
and that it is no longer what an instant renders. The tag was true of the WINDOW and was
read as a fact about the HAND — the same wrong-scope class `discardDeadTargetClause`
already refuses to render for a stack-targeting card.

**Fix.** `handCastabilityTag` takes an `instantSpeed` flag (the card's own type/keyword
test, the one the sorcery gate four lines above already makes) and the `kHandNoLegalTarget`
arm appends `- HELD: this is an instant, so it is in your hand and castable the moment a
legal target appears, including on their turn; this tag is about THIS window, never about
your hand`. The literal head every guide keys on is byte-identical.

**GREEN.** `#W79-DB T8 GREEN an instant with no legal target now reads as HELD`, plus
MUST-NOT-MATCH pins that a sorcery-speed card, and every other verdict, are unchanged to
the byte.

**KEY.** The tag's live channel is the hand line inside `serializeGameState`, which *is*
the board half of the ask key — so the test that matters is that no board number reaches
it. Pinned: the HELD text is identical for `(need 5, sources 6, cost {3}{u}{w}, floating 0)`
and `(need 1, sources 0, cost {w}, "sorcery", floating 9)`, and contains neither number.
And were it ever to ride an option row, the JOINED-menu pins show the option-set key and
the hold-latch held set (built via `holdActionKeyRow` the way the live seam builds
`mLastMenuRows`) identical with and without it.

### (b) keep-X prices the cheapest counterspell

**RED, on the corpus.** `125v130` seq 315 (t57): `Holding up an instant: Path to Exile {w}
needs 1: the largest X that still leaves it castable this turn is X=8` with Essence Scatter
`{1}{u}`, Cancel and Dream Fracture all on the hand line. 47 renders on that seat, **10**
naming Path to Exile. Pinned byte-exact as the base render.

**Fix.** The hand scan tracks a COUNTERSPELL arm beside the cheapest-instant arm
(`w79CounterspellScript`, the `fizzle` test the existing `w78ConditionalCounterScript`
already uses, verified against the four real `auto=` lines); a counterspell in hand takes
the slot whatever a cheaper non-counter instant costs, and the clause says
`Holding up a counterspell: …`. The candidate filter is unchanged, so the price is the
printed cost through the same `selectAutoTapProducers` forecast — target or no target.

**GREEN.** `Holding up a counterspell: Essence Scatter {1}{u} needs 2 …`, with the
cheapest-card clause beside it still naming Path to Exile. MUST-NOT-MATCH: with no
counterspell in hand the clause is byte-identical to wave 78.

**Prediction.** Next corpus: the `Holding up an instant:` clause names a counterspell
(or a counterspell-free hand) in **0 of N** cases where a counterspell sits on the hand
line, against 10 of 47 in wave 78; and the two-copy `[no cast row now …]` shape at deck125
carries `HELD:` on every instant that reads it.

---

## T10 MED — S13 residuals: hoist by body, trim by bytes

### (a) the hoist gate

**RED, measured over all 2,629 corpus prompts.** 94 prompts still repeat one
`{card text: "…"}` body (2×62, 3×8, 4×22, 5×7, 6×3, 8×1, 9×1, **11×1**), costing
23,440 B gross — and **23,440 B of that, 100%, is bodies SHORTER than the wave-78
100-byte floor**. The named repro `126v152` seq 23 is 11 Staff of Nin rows whose shared
body is **96 bytes**. The hoist was never "instance-scoped": it already gathered by body;
the length floor is what refused the case.

**Fix.** The floor is replaced by an OR: hoist when the wave-78 floor says so, **or** when
`w79SharedCardTextPays(rows, bodyLen, rowListLen)` — the header this group is about to
print is smaller than the copies it removes, computed from the same literals the render
uses. Strict superset: nothing that hoisted in wave 78 stops hoisting.

**GREEN, through the live caller.** The eleven corpus rows (byte-exact off the prompt) are
driven through `joinNumberedRows`: one header `Card text shared by options 1-11`, the body
appearing exactly once in the whole menu, every row keeping its number and its other
annotations, and **> 900 B** saved on that one menu. MUST-NOT-MATCH: two rows of a 96-byte
body do **not** pay and are left alone; a 4-byte body never pays; the wave-78 long-body
two-row case still hoists.

**KEY.** Pinned on the JOINED menu: `w77KeyTailOf` and `asyncSlotKeyOf` equal for the
hoisted menu and the gloss-free menu (the header lead is stripped by
`w78StripSharedCardTextHeader`), `optionSetKeyOf` equal, and the `holdActionKeyRow` held
set equal — the hoist mints no re-ask and re-opens no hold.

### (b) the narration trim

**RED, on the corpus.** `130v125` seq 144 = 29,641 B of which the GAME LOG is **20,647 B**
and 46 turns of a 68-turn game; 57 of that seat's 321 windows are over the old trigger and
all 57 are in that one game. `126v125` seq 279 = 32,777 B with 23,156 B of log. The trim
was already by bytes on a line boundary — only where it fired was wrong.

**Fix.** `kNarrationTrimTrigger` 24,000 → **16,000** and the retained tail 20,000 →
**12,000**, both as named constants; `WAGIC_GPT_TRIM_V1=1` still restores the wave-78 pair
byte for byte. The audit-L L10 arithmetic pins are re-derived against the constants so they
cannot pass on a stale pair, plus a RED-ON-BASE pin that a 16,500-byte log is now near the
cap and a 15,000-byte one is not.

### the per-deck byte census (the falsifiable prediction)

New tool `tools/prompt-byte-census.py` (run dir is a required argument and is echoed;
`--selftest` covers the log-block reader, the trim model and the hoist arithmetic).
Computed on the wave-78 prompts themselves — "after" applies the wave-79 hoist gate and
maps a log above the new trigger down by the 8,000-byte difference between the two steady
bands (a log at or under 16,000 is untouched):

```
  deck123  n=571   mean  14032 ->  13157   max  27998 ->  24117   >20KB  124 ->   94
  deck125  n=783   mean  18772 ->  15869   max  34778 ->  26576   >20KB  348 ->  266
  deck126  n=309   mean  17555 ->  15267   max  32777 ->  25028   >20KB  131 ->  123
  deck130  n=321   mean  13916 ->  12447   max  29641 ->  22027   >20KB   77 ->   21
  deck146  n=296   mean  13295 ->  12874   max  26923 ->  25391   >20KB   52 ->   31
  deck152  n=225   mean  11707 ->  11598   max  28621 ->  28039   >20KB   21 ->   20
  deck162  n=124   mean   8807 ->   8807   max  19112 ->  19112   >20KB    0 ->    0
  TOTAL   n=2629  mean  15315 ->  13755   >20KB 753 -> 555   hoist 11038 B (43 prompts)
                  log trim 4,089,777 B   all prompts 40,264,773 -> 36,163,958 B (-10.2%)
```

**Predictions, falsifiable next wave:** deck125 over-20-KB share falls from 44.4% to
≈34%; deck130's maximum prompt falls from 29,641 B to ≈22,000 B and its over-24-KB count
from 57 to 0; corpus mean prompt bytes fall ≈10%; the count of prompts carrying a repeated
`{card text:}` body that PAYS to hoist falls from 43 to **0** (the 51 that do not pay are
expected to remain, and a census reading 0 for *both* would mean the measurement, not the
fix, changed). A hoist-header count of 0 on a menu with ≥3 identical bodies is a FAIL.

---

## T11 MED — `{effect:}` gloss residuals

Card text for all four cards is the primitives' own `text=`, verified against Oracle:
`Triumphant Adventurer`, `Barrowin of Clan Undurr`, `Ranger Class`, `Sigarda, Champion of
Light` — all in `bin/Res/sets/primitives/borderline.txt`.

### (a) clause priority

**RED, on the corpus.** `125v146` seq 51: `{target text: "Deathtouch -- As long as it's
your turn, Triumphant Adventurer has first strike. (...more)"}` — the clause behind the
marker is *"Whenever Triumphant Adventurer attacks, venture into the dungeon"*, the only
clause §PATH step 2 reads, and the seat Pathed a 1/1 deathtoucher over a 3/2. Pinned:
`w78ClauseCutAtOrBefore(adventurer, 140)` is exactly the corpus cut (79) — the wave-78 rule
takes the largest prefix that fits, and a card prints its keywords first.

**Fix.** `textSnippetCore` (whole-text path only — `completeClause`, so
`boardEffectSnippet`'s per-clause budgets and its own last-resort call cannot be re-ordered)
selects depth-0 clauses by PRIORITY within the cap: triggered/activated before
keyword/static, printed back in the card's own order, omission marked `(...more)` exactly
as before.

**GREEN.** The 140-byte render now carries `venture into the dungeon`, still opens with
`Deathtouch`, stays inside its budget plus the marker. MUST-NOT-MATCH pins on the
classifier (keyword and static clauses are not decision-bearing; a text that fits is
untouched; no clauses, or all clauses fitting, leaves the caller's own rule standing).
One found bug in my own first draft, kept as a comment: the bare stem `venture` matched
`Ad-venture-r` and made the keyword clause read as decision-bearing — the phrase is now
`venture into`.

### (b) the fallback cut

**RED, on the corpus.** `152` seq 22: `(Gain the next level as a sorcery to add...` — the
parenthesis is never closed (the text OPENS with the unit, so `w78UnitSafeCut` cannot back
out of it). `146` seq 17 Barrowin and `152` seq 8/19 Sigarda end on a bare `...` with no
omission marker.

**Fix.** The word-cut branch keeps its `...`, then appends `w79CloseOpenUnits(kept)` (one
`)` per unclosed `(`, one `"` for an open quote) and the `(...more)` marker.

**GREEN.** The Ranger Class opener renders balanced and marked, checked through
`w78UnitDepths` on the OUTPUT; plus a 12-case sweep — the four named cards at all three
board tiers (55/90/140) — asserting every render is balanced end to end and every
truncation is marked. Four wave-78 pins were re-derived rather than deleted (W41-4,
two #W48-D5, #W78-CW S8), each stating the change.

**KEY.** The gloss lives inside a `{card text: "…"}` group: ask key, async slot key,
option-set key and hold-latch key all equal between the truncated and full renders.

**Prediction.** Next corpus: **0** `{card text:}` / `{target text:}` / `{effect:}` renders
ending on a bare `...`; **0** renders with an unbalanced `(` or `"`; and where a card's text
carries both a keyword clause and a triggered/activated clause and does not fit, the
triggered clause is present.

---

## T15 MED — a measured zero prints as a zero

**RED, on the corpus.** `162v123` seq 13 prints `turn 5: 20, turn 7: 20, turn 9: 18, now 18
(-2 since turn 5)` over a span whose only life events are two Underworld Dreams pings —
gains 0, losses 2. 116 trend lines on that seat: 68 unchanged, **9 split, 39 net-only**, and
a reader told "net-only means no source" misreads 39 sourced lines.

**Fix.** The split gate `gained > 0 && taken > 0` becomes `gained >= 0 && taken >= 0`. The
CY-F7 rule it was shipped for is kept exactly: a figure `< 0` means no event stream reached
this site, and an UNMEASURED half still drops the split — a number that was never measured
may not print as 0.

**GREEN.** `… life-gain EVENTS put +0 on them and life-loss EVENTS took -2 off` on the
corpus fixture. MUST-NOT-MATCH: `(-1, -1)`, `(-1, 0)` and `(10, -1)` all print no split; a
flat trend still prints its own sentence. The wave-78 MUST-NOT-MATCH pin is re-derived.

**KEY.** No new axis of movement: when one half is zero the other equals the magnitude of
the net that is already on the line, so the two printed figures are determined by figures
the key already carries.

**Prediction.** Next corpus at deck162: split lines rise from 9 to ≈48 (9 + 39) and
net-only lines fall to 0 wherever the event stream reached the site; any residual net-only
line is a genuine "no event source" case and is the number to read.

---

## T17 MED — tools

**RED, run on the wave-78 corpus before the change:**

```
CENSUS CHECK: gameend protocol_replies 2629 vs record-kind sum 2670 (difference 41 = the
kinds that carry no round trip: forced_close 32, recovery 8)
CENSUS CHECK MISMATCH: the listed kinds sum to 40 but the difference is 41
```

The 41st is `152v123` seq 22 — `kind: reveal`, `fallback: engine_answered`,
`latency_ms: -1`, no prompt, no reply: a reveal whose eligibility filter selected zero rows.

**Fix.** `corpus-stats.py` counts, per RECORD, round-trip-kind records with
`fallback == "engine_answered"` or `latency_ms == -1`, and adds them to the no-round-trip
side of the CENSUS CHECK (the `reveal` KIND does not move — every other reveal did make a
round trip). The tool now also prints `CENSUS CHECK RECONCILES.` on success, so a green
result is a statement rather than an absence.

**GREEN, on the wave-78 corpus:**

```
  ...of which engine_answered 1 (round-trip kinds the engine answered itself, or whose latency_ms is -1)
CENSUS CHECK: gameend protocol_replies 2629 vs record-kind sum 2670 (difference 41 = the
records that carry no round trip: forced_close 32, recovery 8, engine-answered (no round trip) 1)
CENSUS CHECK RECONCILES.
```

`--selftest` gains four cases covering the engine-answered rule: the mixed-record count,
the `latency_ms == -1`-alone rule and its negative (a non-round-trip kind is not counted),
the full wave-78 corpus reconciliation from the real kind counts, and the invariant that a
census with zero engine-answered records lists exactly what wave 78 listed.

**The turn source.** The wave-78 late-check read the seat log's `turn` (62/81) while
results.tsv said 85/68: the record `turn` is `translogTurn`, this SEAT's own turn count;
results.tsv's is the GAME's. The tool now prints both, names which is which, and computes
`decisions/turn` off results.tsv when it is present (naming that in the label). On the
wave-78 corpus: record turns 1,016 vs results turns 974, decisions/turn 1.96.

**The harness banner.** `selfplay-harness.sh` now filters `engine_answered` out of the
silent-fallback tally before the class list is applied and prints it as its own line —
`engine-answered windows: N … NOT a fallback, no round trip was owed and no decision was
lost`. Two new selftest cases (pilot-stall-selftest is now **19 checks, 0 failed**) pin
that the `152v123` seq 22 record counts 0 silent fallbacks and does not trip the stall wire.

**Prediction.** Next corpus: `corpus-stats.py <run>` prints `CENSUS CHECK RECONCILES.` and
the harness summary shows the engine-answered count on its own line with 0 in the silent
fallback tally for that class.

---

## T1 HIGH — MEASURE ONLY (owner ruling)

No parser change. `w75ProtocolDeviationClass`, `answerSegmentStatic` and every "parser
unchanged" pin are untouched and green; a pin re-asserts that the unlabelled answer line
still yields no answer segment and that a no-PLAN answer line still classifies
`unlabelled_plan`.

**RED.** All six wave-78 `answer_label_absent` replies wrote a `recovery` record whose only
class field reads `recovers_fallback: "unparsed_reply"` (verified by reading the eight
recovery records in the corpus — seven `unparsed_reply`, one `engine_answered`), so the S1
population and the recovery population could not be joined without re-parsing every reply.

**Fix.** A pure `w79RecoveryLabelAbsent(devClass)` over the FINAL class (the same string
`w78LabelAbsentCounted` reads, so field and meter cannot disagree) is latched at the record
site and stamped on the recovery record as `answer_label_absent`, always present as a
boolean so `false` is a measured negative.

**GREEN.** `125v123` seq 83's reply (verbatim) classes `answer_label_absent`, IS handed to
the heuristic, and the field is true for it; every other class and no class at all are
false; the field round-trips through the translog as a boolean.

**Prediction / WATCH.** Next corpus: `answer_label_absent: true` on the recovery record
appears exactly `answer_label_absent_heuristic_played` times per seat, and the S1 join is
one grep. If deck125's count drops to 0 the guide's edit-4 was the cause (T1's own watch).

---

## Gate

* PARSETEST: **6698 passed, 0 failed** (base 6642 / 0).
* Suite THREADS=1: **1291 tests (0 failed), 76 AI tests (0 failed)**;
  `==Test Failed !==` 0, `Test timed out (game never ended)` 0. No flake hit.
* `check-reply-instructions.py`: OK (1 source file, 37 guides).
* `bash -n selfplay-harness.sh`: OK; `--selftest`: harvest 14/0, pilot-stall 19/0,
  window-loop 12/0, regime-gate 30/0.
* `corpus-stats.py --selftest`: OK. `prompt-byte-census.py --selftest`: OK.
* `git diff | grep -c U+FFFD`: 0.
* No VPK/EBOOT/APK built. No corpus run, no request to the pilot endpoint.

## Weakest evidence

1. **The T10 trim prediction is a MODEL, not a re-render.** I cannot replay the wave-78
   games, so "after" maps a log above the new trigger down by 8,000 B (the difference
   between the two steady bands) at the same point in its own cycle. The real post-fix
   distribution depends on where in the 4 KB cycle each window lands; the *maxima* are the
   solid half of the claim, the means less so. deck152's `max 28621 -> 28039` is the tell:
   that prompt's bulk is not log, so the model barely moves it.
2. **T11a drops a MIDDLE clause.** `Deathtouch -- Whenever … venture` omits "has first
   strike" while printing two clauses that were not adjacent on the card. It is marked
   `(...more)` and no clause is cut mid-sentence, but a reader who assumes the printed
   clauses are contiguous is now wrong in a way wave 78 was not. I did not find a corpus
   case where that matters; I did not look for one exhaustively.
3. **T11's decision-bearing classifier is a lead-and-shape test, not a parse.** It reads
   `Whenever/When/At the beginning of/Coven -/{T}:/{Q}:`, `venture into`, and a cost-then-
   colon head within 24 bytes. A card whose trigger is phrased some other way falls to tier
   0. The 12-case sweep covers four cards; the wide corpus was not re-glossed.
4. **T8(a) fires on TYPE, not on "is this actually held".** Every instant with no legal
   target now reads HELD, including one the seat can never cast this game. That is true of
   the card and false of no window, but it is a broader statement than "this counterspell
   is live".
5. **T8(b)'s counterspell test is `fizzle` in the script.** Same substring the wave-78
   redundant-counter join uses; a counter scripted some other way is not recognised, and a
   card whose script merely contains `fizzle` for another reason would be. Four real cards
   verified, no wider sweep.
6. **T17's `latency_ms == -1` arm is untested against a real record other than the one.**
   The corpus holds exactly one engine-answered record; the rule is pinned on mocks. If a
   future seam writes `latency_ms: -1` on a record that DID make a round trip, the census
   would under-count the round-trip side and print MISMATCH in the other direction — which
   is at least loud.
7. **T15's key argument is an argument, not a pin over two live windows.** I reasoned that
   a one-sided split adds no number the net does not already carry; I did not drive two
   corpus-derived windows differing only in that clause through the live key builders,
   because the clause is in `serializeGameState` and no corpus pair isolates it.
8. **Nothing here was run against the pilot.** Every number is arithmetic over wave-78
   bytes or a PARSETEST fixture. The counters that must read non-zero next wave are named
   above; a census reading 0 for them is a FAIL, not "untested".
