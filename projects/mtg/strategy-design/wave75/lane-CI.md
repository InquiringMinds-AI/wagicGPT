# Wave-75 lane CI — the window/hold family (P1, P8, P13, P18, P19, P12, P24)

Worktree `worktrees/lanes/w75-CI`, branch `w75-lane-CI`, base `207a7f410` (whose last
source commit is lane CH's `062166858`; the archived binary of that tree,
`archives/wagic-76c5fab24-w74step1c`, is the BASE every RED figure below was measured on).

Gate on this branch: `make -f Makefile.sdl -j8` clean, both link guards OK
(`check-ctor-init: OK (118 files)`, `check-reply-instructions: OK (1 source, 37 guides)`).
**PARSETEST 5,809 passed / 0 failed** (base binary, same corpus: **5,788 / 0** — +26 cases
added, 5 deleted with `#W74-CD` O4's land-drop face note, net **+21**).
**Suite THREADS=1, detached unit `w75-CI-suite`: 1,285 tests (0 failed), 76 AI tests
(0 failed)** — `==Test Failed !==` 0 and `==Test timed out` 0; none of the three known
flakes fired, so no solo rerun was needed. `bash -n tools/selfplay-harness.sh` OK;
`--selftest` 4 + **15** + 12 + 30 checks, 0 failed (pilot-stall 13 -> 15).
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**. Every wagic invocation ran under
`systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 --`; nothing was killed
under the cap.

Corpus for every repro: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260909-104713-final`.

---

## P1 (HIGH) — the O11 in-row tag is reverted, and the hold-check bracket keys on the ACTION

### (a) The tag

**Repro.** `146v125` (deck146 seat) turn 21, seqs **65 -> 66 -> 70 -> 72 -> 75**: the same
three-row dead menu put five times. Diffed byte for byte off the records' own
`options_text`, the ONLY delta between seq 65 and 66 is inside row 3:

```
... you have already declined this exact list 3 times this turn}
... you have already declined this exact list 4 times this turn}
```

**RED on base — the corpus IS the failing measurement.** The tagged plain decline row was
taken **284 of 561 = 50.6%** (wave 73: 40%; `#W74-CE` predicted < 30%), and its rising count
is the byte that (i) minted a fresh ask key on every answer and cost 384 model calls on one
window (the CG regression, run `20260909-015553`, killed under invariant 00) and (ii) re-opened
a taken hold at every link of a chain on the CG binary (engine-seat HIGH-1: deck125 t27 Draw,
35 asks, 18 holds taken, the seat's own life 26 -> 2; deck123 seqs 39-51 thirteen asks with an
EMPTY `events` field; deck162 seqs 66-76 eleven asks). Two of the wave's own lanes had already
had to defend against this one clause, each on a different key.

**Fix.** Both render sites are gone — the priority seam's row 0 and the casting seam's decline
row. **The row itself is untouched** (the legal-option boundary) and the FACT is not withheld:
it still rides the prompt-only `[you declined this exact list N times already this turn]` note,
spliced below the list, in no key. `declineRowReaskTag` is kept as the clause's canonical
spelling for `stripDeclineReaskTags` (CG's ask-key guard) and its pins; nothing renders it —
`/usr/bin/grep -n 'declineRowReaskTag' src/AIPlayerGPT.cpp` shows call sites in the PARSETEST
corpus only.

**GREEN.** CG's eight stripper pins still pass unchanged; a new KEY-STABILITY pin asserts that
the seq 65/66 pair moves no hold key, no option-set key and no ask key.

### (b) The bracket

**Repro.** The same five windows. The bracket oscillated on that unchanged menu:

```
seq 65: [hold check: every row above was also on the menu ... (1 window in a row now) ...]
seq 66: [hold check: 1 row above is new ...]        <- the delta is the declined COUNT
seq 70: [hold check: 2 rows above are new ...]      <- +the row's {leaves 3 of your 6} -> {leaves 1 of your 4}
seq 72: [hold check: 1 row above is new ...]
seq 75: [hold check: every row above was also on the menu ...]
```
The guide teaches "N rows above are new -> decline this window", so the seat declined at every
one: 13 wasted model calls at p50 92 s. Corpus-wide the bracket's second half
(`a row moving is what re-opens a hold`) rendered **1,210** times, **351** of them on the CH
binary where it is simply FALSE (engine-seat MED-4) — `#W74-CH` moved the LATCH to the action
identity and left the BRACKET on the render key.

**RED on base, pinned side by side.** `w74HoldUnseenRows` now takes the key function, so both
readings of the same two menus are asserted in one place:

| pair (verbatim rows) | render key (BASE) | action key (this branch) |
|---|---|---|
| 65 -> 66 | **1 new** | **0 new** |
| 65 -> 70 | **2 new** | **0 new** |

Those are exactly the numbers the corpus printed.

**Fix.** `holdReopenNote` counts and remembers through `holdActionKeyRow` — the latch's own key
— and both halves of the bracket now name the carve-out:

> `[hold check: 1 row above is new since the last window at this seam - a row that changes only
> in its annotations (a price, a forecast, a clock, a count) is the SAME row and does not
> re-open a hold; only a row appearing, disappearing, or naming a different card, cost or
> target does]`

**GREEN.** PARSETEST: the two corpus pairs under both keys; a verdict WORD inside an annotation
(`SURVIVES` -> `DIES`) is now the same row (this is `#W74-CH`'s stated, deliberate loss, and the
bracket says so out loud); MUST-NOT-MATCH a row naming a DIFFERENT object is still new and is
counted once; a newly available row counts; both halves of the bracket carry the carve-out
sentence.

**Prediction (falsifiable).** Next corpus, on any run of consecutive windows at one seam whose
rows differ only inside their brackets/braces: `[hold check: ...]` prints
`every row above was also on the menu ... (N windows in a row now)` with N advancing, and no
seat shows a `declined this exact list N` above **5** (wave 74: 23, 13, 12, 10, ...). If a seat
still walks the count past 10 while its `events` field is empty, the re-open is coming from the
window's own re-open condition and not from a key.

---

## P13 (MED) — `main_phase_windows_skipped` was 39% false, and it was never a third arm

**Repro.** All **72** skip traces in the corpus come from ONE arm
(`the instant-speed response arm answered it`, `AIPlayerBaka.cpp:3948`) — not three. Parsed out
of the stderr and grouped, **28 of them are (turn, 4) / (turn, 10) PAIRS** on the seat's own
turns: deck152 16, deck146 9, deck123 2, deck162 1 — exactly the 28 the engine seat called
false. Worked example: `152v126` deck152 counts (5, FIRSTMAIN) skipped while **seq 8** of that
same seat's translog is `Casting decision (Main phase 1, YOUR turn)`.

**Mechanism.** `offered` and `held` are SINGLE SLOTS holding the last (turn, phase) each was
noted for, and the pending skip is compared against them only at FLUSH time, one or more phases
later. Turn T main 1 offers casting (offered = (T,4)); the instant arm arms a pending skip for
(T,4); main 2 then offers casting and OVERWRITES offered = (T,10); main 2's arm flushes (T,4)
against a mark that now names main 2, the exemption misses, and both mains are counted. The
pairing in the data is the signature of that overwrite.

**RED on base (PARSETEST).** The wave-74 bookkeeping, replayed over that exact sequence, counts
**2**; the fixed rule counts **0**. Both are asserted in the same block, so the RED is a case in
the corpus rather than a description of one.

**Fix.** Decide while both facts are current: `noteMainPhaseCastingOffered` /
`noteMainPhaseHoldSuppressed` CANCEL a pending skip for their own phase, and
`noteMainPhaseWindowSkipped` refuses to arm one for a phase that has already offered or been
held (`w75MainPhaseSkipArms` / `w75MainPhaseSkipCancels`, both pure). `flushMainPhaseSkip` keeps
its comparison as belt and braces. No arm was added and no window changed.

**Prediction.** Next corpus: cross-referencing every counted `(turn, phase)` against
`Casting decision (Main phase N, YOUR turn)` headers in the same seat's translog yields **0**
false counts (wave 74: 28 of 72, wave 73: 233 of 375), and no `(turn, 4)`/`(turn, 10)` pair
appears in the traces of one turn.

---

## P8 (HIGH) — REFUTED on the cited repro and on all 13 corpus instances; the silent exit now speaks

**Repro attempted.** `130v125` deck130 seq 61 -> 62, t20: Siege-Gang Commander cast and
COUNTERED by Essence Scatter, no second Casting decision in that main phase, and
`main_phase_windows_skipped` 0.

**What the corpus says.** Census over every seat: **28** windows where this seat's own spell was
countered while it was in its own main phase. **15 re-asked in the same phase**; 13 moved on.
Of those 13, 5 had no other cast row at all, and in the other 8 the remaining cast row was
UNAFFORDABLE once the countered spell was paid for — read off the rows' own
`{leaves N of your M untapped mana sources untapped}` clauses:

| repro | countered cast leaves | the only other row needs |
|---|---|---|
| `130v125` seq 61 | Siege-Gang leaves **5** of 10 | Rorix Bladewing `{3}{r}{r}{r}` = **6** |
| `146v125` seq 9 | Silverquill Silencer leaves **1** of 3 | Pelakka Predation `{2}{b}` = **3** |
| `126v162` seq 6 | Idyllic Tutor leaves **0** of 3 | Tribute to Hunger / Chromatic Lantern |

The mana goes with the countered spell; the oracle then finds no legal cast and the seam
returns without a model call, which is the "no model call when only one outcome exists" rule
working. `main_phase_windows_skipped` reads 0 for that game CORRECTLY: the standard branch was
reached and noted the offer. **The reviewer's "6 sources were still untapped" is a read of the
PRE-cast prompt.** There is no defect here and no fixture to write.

**What was wrong, and is fixed.** The verdict was undiagnosable: `buildCastSpell`'s failure was
the seam's ONLY silent exit. It now traces, in the shape the land arm already uses:

> `AIPlayerGPT: cast ask NOT issued - the oracle finds no legal cast in this sorcery-speed
> window (7 card(s) in hand, 5 untapped source(s)); the pilot is not asked a question with no
> answers`

**Prediction.** Next corpus: every own-main-phase window that follows a countered spell with no
re-ask carries this line with an untapped-source count LOWER than the cheapest remaining cast.
If one appears with enough sources for a card the prompt marked `[castable now]`, THAT is the
P8 defect and the line names the window to look at.

---

## P18 (MED) — the land drop is one window, with a row per face

**Repro.** 9 pairs: `146v125` seq 3 -> 4, deck152 17 -> 18, deck126 23 -> 24 and 31 -> 32,
deck130 29 -> 30, deck162 4 -> 5, deck123 4 -> 5 / 7 -> 8 / 32 -> 33 — 18 of one seat's 322
decisions. The first window offered one row plus `#W74-CD` (O4)'s note that "the next window
asks which face"; the next window then asked it, and its own `Decline - do nothing` row could
un-make the drop the first answer had committed.

**RED on base (probe).** `tools/mdfc-face-probe.sh` (new; the stub-server pattern lanes CG and
CH used — the shape needs a seat that asks a MODEL, and the suite has no endpoint). One pinned
deck146-vs-deck125 game per binary:

| binary | two-faced land-drop windows | offering BOTH faces as rows | SECOND (face) windows asked |
|---|---|---|---|
| base `wagic-76c5fab24-w74step1c` | 1 | **0** | **1** |
| this branch | 5 | **5** | **0** |

The base's second window is verbatim `Choose an option for Brightclimb Pathway:` two asks after
the tagged land drop.

**Fix.** The land arm renders one row per FACE (`w75MdfcFaceRowTag`), both rows carrying the
same land entry — the card is the same either way, since the back face arrives through its own
`{0}` flip ability and not through a second land play, so the APPLY side is unchanged. The
answer is then carried into the face menu (`mLandFacePre*`) instead of being asked again: the
wave-71 L5 "carry the committed intent" pattern, scoped to the same card pointer, the same turn,
and a menu that really is the two-face menu, consumed once, and falling through to a normal ask
whenever any of that does not hold. **Nothing is removed**: both faces still reach the
battlefield and the decline row is untouched. `w74MdfcLandDropFaceTag` and its 5 pins are
DELETED — the window they describe is no longer asked.

**GREEN.** The probe above, plus 7 PARSETEST cases: both row texts, the MUST-NOT-MATCH that
neither defers the face to a later window, a single-faced land takes no tag and gets no second
row, an unknown tap mana says nothing rather than guessing, and the key-stability pin (neither
tag carries a board-derived number; the two-row menu keys as itself).

**Prediction.** Next corpus: **0** records whose prompt is headed `Choose an option for <card>:`
with an `OTHER FACE of` row, on any seat; every two-faced land renders a FRONT row and a BACK
row on the `Land drop:` menu, and the seat's reasoning stops re-deciding the face ("I'll stick
with 1... Actually, I'll go with Brightclimb").

---

## P19 (MED) — the hold row, cut by evidence, and made TRUE again

**Repro.** deck146 seq 72: a 3-row menu whose hold row is **1,074 bytes** of the options text,
more than rows 1 and 3 combined; the same row measures **1,287 B** at other windows. Holds were
taken in **11 of 322** decisions.

**The evidence CC asked for.** Over the **1,619** corpus windows that render the row, the share
of `reasoning` bodies quoting each clause:

| clause | quoted |
|---|---|
| head: "pass now, and do not ask me again" | **32.7%** |
| "CANNOT COME BACK AND TAKE ONE OF THE ROWS ABOVE" | **15.3%** |
| "second main phase" (the main-1/main-2 span) | **10.6%** |
| "it stands until your next turn begins" | **8.6%** |
| every remaining clause of the `{...}` brace | **3.5 - 5.3%** |

61 records (**3.8%**) echo the WHOLE row, which is the floor those last numbers sit on: outside
a whole-row echo the brace is not quoted at all.

**Fix.** The head (the three quoted sentences) is UNCHANGED — it is inside both ask keys, and
moving it would move every ask key corpus-wide. The brace keeps the one sentence above the floor:

> ` {a hold taken in your first main phase also covers your second main phase while these rows do not change}`

Everything else moves BELOW the list, once per prompt, on the prompt-only channel the declined
count already rides (outside every key, and carrying no number so it could not move one even if
that channel changed) — and is REWRITTEN TRUE. Since `#W74-CH` the latch keys on the ACTION
identity, so the old closing sentence — *"Any OTHER change re-opens this window, including a
kill count, a damage figure, a survivor count, a price"* — has been a FALSE fact on every one of
those 1,619 windows. The paragraph now says what the key does: annotations are the same row;
a row appearing, disappearing, or naming a different card, cost or target re-opens it, and so
does a change in whether the board kills you (the two synthetic verdict markers).

**GREEN, measured live** on the probe run above: the hold ROW is **493 bytes** median, 596 max
(was 1,074-1,287), and 5,040 of the 5,041 windows that render it carry exactly one
`[HOW A HOLD ENDS: ...]` paragraph. The first cut of this fix put the paragraph only in `askModel`, and the probe caught
it: 5,222 windows rendered the row and only 2,617 got the paragraph, because the PRIORITY seam
builds its own tail. Both sites now emit it. PARSETEST: the brace's literal, its size, every
moved sentence present in the paragraph, the truth fix and its MUST-NOT-MATCH, and the
no-number/bracket shape.

**Prediction.** Next corpus: the hold row measures under 650 B at every window; `[HOW A HOLD
ENDS:` appears exactly once in every prompt that renders a hold row and never in one that does
not; hold takes per window do not fall below wave 74's 11/322 (this is a truth-and-size change,
not a persuasion change — if takes DROP, the removed sentences were load-bearing after all and
the item reopens).

---

## P12 (MED, MEASURE ONLY) — the chain census at the casting seam, and the clause it could not see

**Repro.** `125v126` deck126's 41-window Sanguine Bond / Exquisite Blood chain was paid entirely
at the CASTING seam, and contributed **nothing** to `chain_*`: the O13 instrument runs over the
priority seam's `shownLines` only. On the CH binary 113 windows offered nothing but rows
annotated `this does nothing` while `chain_selfharm_rows` read **2**; the honest corpus figure,
measured off the records' own `options_text`, is **174 / 2,396 = 7%** (deck126 85, deck123 58,
deck146 31).

**Two defects, both measure-only.**
1. The instrument runs at one seam. The same census, with the SAME gate and the same two
   predicates, now runs at the casting seam and reports as its own triple —
   `chain_windows_only_selfharm_cast`, `chain_selfharm_rows_cast`, `chain_acting_rows_cast` —
   so wave 74's numbers stay comparable.
2. `w74RowIsDecisionEmpty` enumerated four clauses and **none of them is the one the dead rows
   actually print**: `{right now: they control 0 creatures - at 0 this does nothing}`. That is
   why the counter read 2 against a real 174. The predicate is widened by that clause.

Nothing is withheld, collapsed or auto-answered; the predicate is read by the two census arms
and by nothing that removes a row.

**RED on base / GREEN.** PARSETEST, on the deck126 chain window verbatim: one acting row, and it
is decision-empty (the base predicate reads 0 empty); MUST-NOT-MATCH a window carrying one live
cast row is not only-selfharm; and no live row matches the widened clause.

**Prediction.** Next corpus: `chain_selfharm_rows` + `chain_selfharm_rows_cast` summed over all
gameends lands within ~20% of the count of windows whose every acting row carries
`this does nothing` (wave 74: 560 counted against 174 such windows at the priority seam alone,
two quantities that cannot be compared). If the cast-seam triple is still 0 on a corpus with a
chain in it, the gate — not the seam — is wrong.

---

## P24 (LOW, loop tooling)

### (a) `corpus-early-check.py`, keyed on the run

**RED on base.** The wave-74 tool selects seats by `mtime(seat log) >= mtime(launch script)`.
Run against the wave-74 corpus with the correct launch epoch it reports **80 seats**, and its
worst offender list names **`ai_baka_deck198`** — lane CH's hold-chain PROBE, not a corpus seat
at all. mtime is also rewritten by the end-of-run harvest, so the predicate is meaningless
against a finished run (with the outdir itself as the argument it reports **0 seats**).

**Fix.** `projects/mtg/tools/corpus-early-check.py` (new; the `~/.gatelogs` copy is left alone)
takes the RUN OUTDIR. The harness creates `game-<d0>v<d1>-<gstart>.stderr` in the outdir at the
moment it starts each game, live, so the outdir IS a manifest of which games this run started,
when, and with which decks. A seat log belongs to the run when its FILENAME epoch (written once,
never rewritten) is within 15 s of one of those `gstart`s AND its deck is one of that game's two
decks. It degrades to the old launch-epoch window when there is no manifest yet, and says which
rule it used on its first line.

**GREEN.** Same corpus: **48 seats**, `deck198` gone, and the three seats the corpus's own
`known-bugs` names (deck126 41 asks in one phase, deck125 38, deck152 17) at the top. The 48 is
42 harvested seats plus the 6 belonging to the three games that were RERUN on the CH binary and
whose first attempts are still in the shared dir — correct for an early check, which runs while
the run is live.

### (b) The PILOT STALL K-pool, bounded per seat

**RED on base, in the selftest.** One wedged seat (its log the newest, twenty consecutive
timeouts) beside five seats the pilot is answering normally: the pool was filled seat by seat
from the newest log, so that one seat supplied all K=6 records and
`pilot_stall_verdict` returns **`STALL 6`** — a corpus stopped on the evidence of one game.

**Fix.** ACROSS SEATS is the predicate's own word: no seat may contribute more than
`ceil(K / live seats)`. The cap is strict, so a lone silent seat leaves the pool short of K and
the verdict is OK (that seat is the game watchdog's business). With one live seat the cap IS K,
so the wave-74 case (i) — a stalled tail that must still be caught — is unchanged.

**GREEN.** `pilot_stall_verdict` on the same crafted directory: base **`STALL 6`**, this branch
**`OK 1`**. Two selftest checks added (the wedged-seat-among-answering-seats case and the
lone-live-seat case); `--selftest` 4 + **15** + 12 + 30, 0 failed; `bash -n` OK.

**Prediction.** Next corpus: PILOT STALL does not fire while any seat is answering; if it fires,
its banner's count equals K and the run really is wedged. The early check, pointed at the run's
own outdir, reports a seat count equal to 2x the games started and no probe or GUI log.

---

## Weakest evidence

* **P1(b) widens the forgiveness at a SECOND surface.** `#W74-CH` accepted the loss of
  verdict-word re-opens for the LATCH and recorded it as its own weakest evidence; this lane
  extends the same key to the BRACKET, so the model is now also told that such a row is the
  same row. If a verdict inside an annotation turns out to be a fact the model must be re-asked
  over, it needs its own marker row (as the two lethality classes have) and neither surface will
  catch it. Nothing has been measured against a live corpus.
* **P1(a) is a REVERT measured on a confounded population.** The 50.6% take rate is over 561
  windows, 4x wave 73's, and much of that growth is the HIGH-1 loop itself, so the two rates are
  not measured over comparable boards. The honest claim is only that the clause cost two
  corpus-killing regressions and did not move its own metric the right way.
* **P13's fix is pinned at the PREDICATE level, not end to end.** The two decision functions are
  pure and the wave-74 sequence is replayed over them, but the six lines of member plumbing that
  call them are exercised by no test — the meter has no fixture and, being a meter, its real
  gate is the next corpus's cross-reference.
* **P8 is a refutation, not a fix.** It rests on the `{leaves N of your M}` clauses the rows
  themselves printed; I did not re-derive the payment from the engine, and a row whose remainder
  clause is wrong would make one of those 8 a real lost window. The new trace is what makes the
  next one adjudicable.
* **The P18 probe is stochastic** (real hands, no seed control), and the base column rests on
  ONE two-faced land window in one game. The categorical claim — base offers 0 of 1 windows with
  both faces and asks a second window; this branch offers 5 of 5 and asks none — is the honest
  discriminator; the counts are not.
* **P18's carry is a heuristic MATCH on the face menu's option text** (the back face's name, and
  a plain land-play row). If the engine ever renders that menu differently the carry silently
  stops firing and the second window comes back — which is safe (it is asked, not mis-answered)
  but would be invisible without the DebugTrace.
* **P19's cut is measured on QUOTATION, which is not use.** A sentence the model obeys without
  quoting reads as unquoted here. The prediction is written so that a DROP in hold takes reopens
  the item.
* **P19 changes no ask key**, but it does change what a window's prompt contains; no corpus has
  run against it. The first draft of it missed half the windows and only the live probe caught
  that — the same class of miss could hide in a seam the probe did not exercise.
* **P12 is a substitute measurement.** `this does nothing` is the annotation I could count off
  the corpus, not a classifier of "this row cannot change the game"; a row that does nothing for
  another reason is still invisible.
* **The stub answers instantly.** Both probe columns reproduce control flow faithfully and say
  nothing about the wall-clock arithmetic that actually freezes a corpus.
* **No live-endpoint validation.** Nothing in this lane ran against Spark.
