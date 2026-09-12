# Wave-80 lane DG — plan echo, menus and render (U1, U5, U11, U12, U16, U17)

Base `70345356d` (= master). Branch `w80-lane-DG`. Every fixture is built from the wave-79 corpus
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-000748-final` — the actual prompt/plan/menu bytes
the seat was shown — and driven through the entry the LIVE seam calls.

**Gate**: PARSETEST **6948 passed / 0 failed** (base 6902/0; +46 pins). Suite THREADS=1 as detached
unit `w80-DG-suite`: **1295 tests (0 failed), 76 AI tests (0 failed)**, `==Test Failed !==` **0**,
`Test timed out (game never ended)` **0**, no flake hit. `check-reply-instructions.py` OK
(1 source, 37 guides). `corpus-stats.py --selftest` OK. `prompt-byte-census.py --selftest` OK.
`git diff | grep -c U+FFFD` = **0**. Both link guards (`check-ctor-init.py`, header + ctor
initializer list) pass — two new members, `mPlanCastCompletionState` and `mPaidPendingSources`.

**Protocol-text verification (brief's gate)**: the per-window format-example sentence is
`exemplarSentence` / `askExemplar` in `AIPlayerGPT.cpp` — RENDER text assembled per ask. It is NOT
part of the golden protocol: `kReplyProtocol` / `Res/ai/gpt/reply-protocol.txt` carries its own
worked example (`"CHOICE: 3 (Cast Example Card)"`, with `kExampleFakeCardLc` as the placeholder) and
is untouched by this lane — `git diff` shows no change to either. #W77-CS already made exactly this
edit on the ANNOUNCE_X seam one wave ago, which is the precedent. Editing was therefore in scope.

---

## U1 (HIGH) — a cast step stays in the echo until its completing menu resolves

**RED.** `130v162` deck130 **seq 31 → 32**. The seq-31 reply is
`PLAN: Cast Starstorm X=2 to kill Fog Bank, then attack with Rorix in second main phase.` +
`CHOICE: 1 (Cast Starstorm)`. Seq 32 is the completing menu (`Choose an option for Starstorm: (you
answered "Cast Starstorm" at the previous window — this menu completes THAT cast…)`) and its echo
reads **`then attack with Rorix in second main phase.`** — the cast clause deleted — on a board whose
five Mountains and Talisman are already `[tapped]` and whose mana line reads `Mana available: 0`. The
seat answered `CHOICE: 3 (Decline - do nothing)`. `125v126` seqs 44 → 45 is the same shape
(`Cast Sphinx's Revelation at X=2 … then hold up mana for counterspells.` → echo
`then hold up mana for counterspells.`, answer `CHOICE: 4 (Decline)`), and so is `125v152` 9 → 10.
The pins reproduce both echoes byte-exact through `gptcaveat::planStepsAfter` and through the new
live entry with the latch off, so RED is the shipped behaviour, not a reconstruction.

**Fix.** A cast commitment now opens a three-state latch (`mPlanCastCompletionState`): armed where
`FindCardToPlay` validates the model's pick, promoted to *this-window-is-the-completing-menu* inside
`assemblePrompt` when `w80CompletingCastMenu(tail, mInAnnounceXAsk)` recognises the seam (the
engine's own commitment sentence, or the ANNOUNCE_X ask scope), and retired there on any other
window — so every non-completing window is byte-identical to wave 79. The echo calls one entry,
`w80CarriedPlanSteps(plan, stepsDone, pending)`, which steps the pointer back exactly one at the
completing menu; the record writer then consumes the cast's step AT the completing seam instead of
advancing a second time. Second half: every non-casting row of a completing cast-mode menu now
carries `w80CompletingDeclineSpentClause` — `{your 6 tapped sources are SPENT either way: the payment
for Starstorm is already made…}` — gated on the engine having actually queued a payment for that card
in that step (`mPaidPendingCard`/`mPaidPendingStep`, plus the new `mPaidPendingSources` counted off
the receipt), so it is a fact about a receipt already in the log.

**GREEN.** 14 pins. The corpus's own two plan strings round-trip to the FULL plan with the latch on
and to the wave-79 bytes with it off; both completing seams are recognised and a fresh casting menu
and an uncommitted cast-mode menu are not; the spent clause agrees in number and prints nothing with
no receipt; and a KEY pin shows two joined menus differing only in the source count give identical
`w77KeyTailOf` (the clause is a `{…}` group, out of the ask key and the async slot key).

**Prediction (falsifiable).** Next corpus: `dropped after its X was announced` = **0**, and **0**
leading-clause losses in echoes at completing menus — measured as: for every record whose prompt
contains `this menu completes THAT cast` or is an ANNOUNCE_X ask, the `THE PLAN YOU LAST STATED`
text must still contain the leading clause of the plan written at the immediately preceding record.
Non-zero on either is a FAIL, not "untested".

## U5 (HIGH) — neutral format placeholder on every ask seam

**RED.** `162v152` seq 6 and 138 more windows: `e.g. "CHOICE: 1 (Cast Luminarch Aspirant)" (a worked
example of the format, written out from row 1 of this list…)`. deck162's reasoning quoted the example
three times, once as *"picked Dictate as it's first in the menu"*; the seat took the exemplified row
in **62/139** windows. Pinned by driving `askExemplar` over a corpus-shaped menu: it returns row 1 and
that exact string, and `exemplarSentence` names the row.

**Fix.** `askModel` now replaces a quoted LIVE row with the FORMAT template
`CHOICE: <row number> (<that row's short name>)` under a new `exemplarRow == -2` face that says the
number and the name in it are *placeholders, not a recommendation*. Untouched: the ANNOUNCE_X
template (#W77-CS, row −1), the dead-menu face (row 0 — a claim about the rows, not an example to
copy), and the priority seam, which already printed a placeholder.

**GREEN.** 5 pins, including MUST-NOT-MATCH on both untouched faces and a KEY pin (the sentence is now
a constant on every menu, so it cannot move a key at all).

**Prediction.** The row-1 pick rate at the ask seam falls from 62/139 (44.6%) toward the priority
menu's rate for comparable menus. Measured as: over `kind == "ask"` records with ≥3 rows and a
decline row, the share whose `choice == 1`, compared against the same statistic at `kind ==
"priority"` in the same corpus. A rate that does not move is a FAIL.

## U11 (MED) — own-spell counter targets, and a pointer that points

**RED.** `125v146` deck125 **seqs 104 → 105**. Cancel's cast row named both stack objects; the target
menu listed `1. Vanishing Verse [instant] [opponent's stack]` and `2. Dream Fracture [instant] [your
stack]` — the seat's OWN counter, already aimed at that same Vanishing Verse (the cast row says so:
`{your Dream Fracture already targets this on the stack…}`) — and the seat answered **row 2**. Two
counters spent for nothing, in a game it lost. The only signal on the own-spell row was the zone tag,
which is a LOCATION. Separately, **135** wave-79 prompts print `the stack is not empty (see ON THE
STACK above)` with no such block (`125v162` seq 199 is the review's citation): the sorcery-speed gate
reads `count(0, NOT_RESOLVED)`, which counts every object, while the block prints only RESPONDABLE
ones — a phase step, a day/night marker or an emblem separates them.

**Fix.** `w80OwnStackSpellTag` marks the seat's own unresolved spell on BOTH surfaces the decision is
read off — the target row and the cast row's `can target on the stack:` list. `w80StackNotEmptyReason`
keeps the legality reason and prints the pointer only when the block actually rendered
(`stackBlockRendered`, set where the block is emitted in the same `serializeGameState` call).

**Wording deviation, flagged.** The brief's suggested literal was `[your own spell - never a counter
target]`. "Never" is advice, and it is not always true (a cast trigger or a fizzle you want makes it
correct), so the row states the FACT instead — `[THIS IS YOUR OWN spell, waiting to resolve on the
stack - aiming at it spends this effect on your own play, not on theirs]` — restriction-first with no
verb the model can read as a recommendation, which is the #W53-N D2 rule for the hold row. The option
is untouched; nothing is removed, capped or auto-answered.

**GREEN.** 5 pins (positive, three MUST-NOT-MATCH zones/sides, both faces of the stack reason).

**Prediction.** Next corpus: `0` prompts carrying `the stack is not empty (see ON THE STACK above)`
without an `ON THE STACK, waiting to resolve` block (wave 79: 135), and every target row naming a
card in the seat's own stack zone carries the marker.

## U12 (MED) — the six gloss/render residuals

1. **Mid-clause `(...more)` on a dangling connective.** RED: `146` seq 22 and **60 further windows**
   across the corpus render `…becomes a 3/3 black Beholder creature with menace and (...more)`, and
   `146` seq 54 `-8: You get an emblem with (...more)` — the reader is promised a term and never given
   one. Fix: `w80TrimDanglingTail` drops a trailing connective (a closed list of 28 conjunctions,
   prepositions and determiners — never a verb or a noun) before the marker is appended, on both cut
   paths.
2. **Two omission marks for one omission.** RED: `126v152` seq 15 Sigarda renders `…look at the top
   five... (...more)`. Fix: the mid-clause path drops the separate `...`; `(...more)` already opens
   with an ellipsis and already says text was omitted, and with (1) the cut now ends on a phrase that
   can end. The marker LITERAL is unchanged, which keeps every "is this render marked?" pin binding on
   one string. Three wave-78/79 literal pins re-derived in place, each with its reason.
3. **Teferi's Puzzle Box priced.** RED: `162v152` seq 18 renders `{feeds: … an amount that is not
   fixed (read the card)}` beside a 6-card hand on the same screen. The script names its own counter
   (`count(type:*:opponenthand) … draw:countedamount opponent`), so the row now reads `their whole
   hand: 6 cards right now (the amount is their hand size at the moment it fires…)`. KEY-pinned out of
   every key.
4. **`[legendary:` names the loyalty.** RED: `162v125` seq 41 spent 5 mana to replace a loyalty-5 copy
   under a clause that said "you choose which" and never told the two copies apart. `legendTwinTag`
   now carries `at N loyalty`; −1 (a legendary creature or artifact) is the wave-79 bytes.
5. **Sorin's dropped abilities are NAMED.** RED: every library-search row at deck126, all six games,
   rendered `+1: Put a 1/1 black Vampire creature token with lifelink onto the battlefield. (...more)`
   — three same-priority activated clauses (78/61/154 B) against a 140 B budget, so the selector keeps
   the `+1` and the `-6` the deck guide teaches is invisible. Fix: the omission now names the loyalty
   labels it dropped and points at the decklist, which carries every card's full text:
   `(...more - this card also has -2:, -6:; their full text is in the decklist in your instructions)`.
   Fires only where EVERY omitted clause has a loyalty label, so a body that drops a static or
   triggered clause keeps the plain marker.
6. **Mode liveness names the OBJECT.** RED: `146v152` seq 21 row 2 `{modes live right now: … return
   creature and you draw, …}` while Soul Shatter's row on the same screen named its object and
   Vanishing Verse named its targets; the seat wrote *"No targets for modes really"* and cast the other
   spell, at the decision that lost the game. `w80ModalSpecObjects` walks the same chooser
   `modalSpecObjectCount` counts (bounded at three names + a count, each with its zone) and
   `modalModesTag` prints `return creature and you draw (right now: Silverquill Silencer [your
   graveyard])`. An empty list prints nothing — never an empty parenthesis.

**GREEN.** 21 pins. **Prediction.** Next corpus: `0` renders matching ` (and|with|of|to|the|a|an|or)
\(\.\.\.more\)`; `0` renders containing `... (...more)`; `0` `{feeds:` clauses reading "an amount that
is not fixed" on a card whose script counts a hand; every `[legendary:` on a planeswalker twin carries
`at N loyalty`; every multi-loyalty body whose row is cut names its dropped labels; and ≥1 rendered
`(right now:` inside a `{modes live right now:` clause.

## U16 (MED) — the hold apparatus states its rule ONCE; deck146 census before/after

**Measured, before** (new `HOLD APPARATUS` block in `prompt-byte-census.py`, mean bytes per prompt):

| deck | hold row | `[hold check:` | `[HOW A HOLD ENDS:` | total | after fold | saved |
|---|---|---|---|---|---|---|
| 123 | 479 | 316 | 582 | 1377 | 1274 | 103 (0.87%) |
| 125 | 421 | 302 | 529 | 1253 | 1149 | 104 (0.84%) |
| 126 | 354 | 296 | 427 | 1077 | 987 | 90 (0.83%) |
| 130 | 338 | 260 | 438 | 1037 | 945 | 92 (0.85%) |
| **146** | **341** | **239** | **435** | **1015** | **924** | **91 (0.61%)** |
| 152 | 270 | 231 | 341 | 842 | 771 | 71 (0.68%) |
| 162 | 339 | 290 | 416 | 1045 | 962 | 83 (0.86%) |

**Fix.** The `[hold check:` bracket keeps the only thing it knows — the measurement — and its
restatement of the same-row rule becomes a pointer to `[HOW A HOLD ENDS:`, which states that rule in
full one block below and is spliced on exactly the windows that carry a hold row (both seams). The
condition is read off the rows with the same shared head `askModel` splices on, so the bracket cannot
point at a paragraph that is not there; the land seam, which has no hold row, keeps the rule itself
and loses nothing.

**What I did NOT do, and why.** I first folded the ROW's `or until one of the rows above changes (any
change re-opens this window` clause as well (a further ~52 B/prompt) and reverted it: #W61-U C14 put
that clause on the row from live evidence — the wave-55 literal promised a hold that expired with the
TURN, which is the deck146 HIGH-3 defect — and **three** existing pins assert the row promises
re-opening IN ITS OWN WORDS. Reversing a wave-61 truth fix for 52 bytes is the wrong trade; the revert
is recorded in the source comment at `kHoldPriorityRowHead` and pinned as a MUST-NOT-MATCH. So U16's
delivered saving is the bracket only: **deck146 1015 → 924 B/prompt (−91 B, −0.61% of the mean
prompt)**, corpus-wide 71–104 B/prompt. This is honestly smaller than the item's framing implies; the
remaining ~900 B is three surfaces that each have a live-evidence claim on their text.

**GREEN.** 6 pins (all three bracket faces fold, the no-paragraph face does not, the row is unchanged).

## U17 (MEASURE ONLY) — per-seam latency vs prompt bytes vs reasoning_chars

New block in `corpus-stats.py` (`w80_seam_cost_rows` + `w80_pearson`, both selftested on mocked
records, including the "a cache hit made no round trip" exclusion). On the wave-79 corpus:

```
seam            n  lat p50 s prompt B p50   reason p50   ms/reas ch
ask          1696       96.2        11089         5833       16.484
attackers      84       73.9        11969         4108       17.995
blockers       16      107.1        12530         5287       20.254
bottom         10      115.0         2021         4972       23.127
discard        26       74.7        14770         5478       13.632
priority      360       92.2        14703         6128       15.045
reveal         11       74.3        13106         4096       18.137
CORRELATION with latency over 2203 round trips: prompt bytes r=-0.302, reasoning_chars r=0.748
```

**The measurement answers U17's question.** Latency is bought by REASONING OUTPUT, not by prompt
bytes: r = **+0.748** against `reasoning_chars` and **−0.302** against prompt bytes, and the seam with
the *smallest* prompts (`bottom`, 2.0 KB) has the *highest* p50 latency (115 s) while the seam with the
largest (`discard`, 14.8 KB) is near the fastest (74.7 s). `ms per reasoning char` sits at 13–23 ms
across every seam, i.e. the decode rate. So the 78 → 94 s rise is NOT a prompt-size problem, and byte
work (U16, the hoist, the narration trim) will not move it; the levers are the reasoning budget, the
seam mix and server-side throughput. No behaviour changed.

---

## Weakest evidence (ranked)

1. **U1's latch is pinned PURE, never driven through a live game.** `w80CompletingCastMenu`,
   `w80EchoStepIndex` and `w80CarriedPlanSteps` are proven on the corpus's own bytes, but the STATE
   MACHINE (armed at `FindCardToPlay`, promoted in `assemblePrompt`, consumed in `writeTransLog`) has
   no fixture — no corpus and no pilot requests were allowed this wave. The specific untested edge:
   a completing menu that is asked, drops stale and is re-asked leaves the latch at state 2 across the
   re-ask (deliberate, so the clause survives), and if that window is then never answered the latch
   retires on the next non-completing window — one window late. Unproven that this never happens.
2. **U1's spent-sources clause depends on `notePaymentQueued` firing for the card the completing menu
   is about.** `mPaidPendingSources` is counted off the receipt, so it is exact when it prints — but
   whether a cast-mode menu ALWAYS has a queued receipt by the time it is built is inferred from one
   corpus board (`130v162` seq 32: six sources tapped, `Mana available: 0`), not measured. If the
   receipt is absent the clause prints nothing, which is the safe direction.
3. **U11's "the target roster matches the cast row" half is NOT done.** In the cited repro
   (`125v146` seq 104/105) the cast row listed BOTH stack objects and the target menu listed the same
   two — the roster did not under-report there, and I found no corpus instance where it did. Marking
   the own-spell on both surfaces is what I shipped; the under-report claim is carried forward
   unadjudicated.
4. **U5's prediction is a rate, not a count.** A fall in the row-1 pick rate could be confounded by a
   different menu mix in the next corpus. The stronger negative is available and should be checked
   too: `0` ask prompts whose format sentence contains a card name (the old shape is now impossible by
   construction, so a non-zero would mean the seam was bypassed somewhere I did not find).
5. **U12(6)'s object walk is a re-implementation of `TargetChooser::countValidTargets`'s zone walk,
   not a call into it.** The engine exposes only the count. I walk `inPlay, graveyard, hand, exile,
   stack` (the count also walks `library, commandzone, sideboard, reveal`), so a spec that reaches a
   hidden zone will print fewer names than the count implies. Bounded and conservative — it names
   fewer, never more — but the two can disagree.
6. **U12(5)'s pointer says the decklist carries the full text.** True of `describeDeckCards`, which
   aggregates library/hand/inPlay/graveyard and prints `card->text` untruncated — verified by reading
   the builder, not by reading a system-prompt record (the translog's `system` record carries only
   bytes and a hash, not the text).
7. **U16's census models the fold arithmetically** (it subtracts the three known rule tails and adds
   the pointer) rather than re-rendering a post-fix prompt. The tails are copied from the source, so a
   future edit to either would silently make the model wrong.
