# Wave 78 — Lane CY: the Astra adversarial review of step one, worked

Base = master `dd88b21b8` (step-one gate 1: suite 1291/0 + 76 AI/0, PARSETEST 6547/0).
Final on this branch: **suite 1291 / 0 failed + 76 AI / 0 failed (THREADS=1), PARSETEST 6642 / 0 failed**
(+95 checks), `==Test Failed !==` 0 and `Test timed out (game never ended)` 0 in the log,
`check-reply-instructions` OK (1 source, 37 guides), `bash -n` OK,
`selfplay-harness.sh --selftest` harvest 14/0 + pilot-stall 17/0 + window-loop 12/0 + regime 30/0,
`corpus-stats.py --selftest` OK, `git diff | grep -c U+FFFD` = 0.

**Twelve findings: twelve CONFIRM, zero REFUTE.** Every source-traced claim held against the code.
No parser change (S1 stays MEASURE ONLY — F10 touches the counter only, and a pin asserts the
unlabelled answer line still yields no answer segment). No golden-protocol byte moved. No VPK.

---

## F1 (HIGH, S3) — the collapse's decline ledger had no cost, no board and no stop identity. **CONFIRM**

**Repro.** `mListDeclineCount` is keyed on `listKeyHash(optionSetKeyOf(shownLines))`, and
`optionSetKeyOf` → `stripRenderAnnotationsLc` drops every `[...]` group — the `[cost: ...]` group
included. So a token-maker whose REAL cost changed while the row names stayed reads as the same
already-declined list, and `w78StopReachedRePutCollapses` drops the window: a legal option lost,
which the owner's allowance does not cover. The ledger also carried no board and no stop identity.

**RED evidence (executed).** Two facts inside the pin, both evaluated on the base composition:
`optionSetKeyOf(rows) == optionSetKeyOf(priced)` where `priced` differs only by
`[cost: Tap, Sacrifice a Goblin]`, and `w78StopReachedRePutCollapses(true,true,29,13,13,1,true)` —
the base's identity-blind gate — is true for it. Plus the counterfactual: `w78RePutIdentityStands`
reverted to `return true;`, rebuilt, PARSETEST **4 failed / 6552 passed** (cost change, different
stop, moved board, and the empty-ledger MUST-NOT-MATCH).

**Fix.** The collapse identity is `w78RePutCollapseIdentity` = the hold-latch ACTION identity of
every acting row (`holdActionKeyRow`, so `w75CostGroupsKey` keeps the cost and the balanced-group
strip drops the board numbers) + the ask-cache board key (`declineBoardScope(serializeGameState())`,
the same value the pass-decline ledger keys on) + the stop statement (number, date, the count it was
stated against, and its SOURCE — the persisted store or the plan carry). Recorded in
`mListDeclineIdent` at the decline, compared at the re-put; any of the three different and the
window is asked.

**GREEN.** The pin walks the LIVE ledger (`optionSetKeyOf` → `listKeyHash` → identity → predicate),
writing its own declines exactly as the seam writes them on a `pass`: seqs 41→52 are still **1 ask
and 11 collapses**, the cost-changed re-put is ASKED, a different restated stop is ASKED, a moved
board is ASKED, and a moving `M=31 → M=34` does NOT mint a fresh identity. **Prediction:** the
wave-78 corpus shows `stop_reached_reputs_collapsed` > 0 with no seat ever passing a window whose
only acting row's `[cost:]` differs from the one it declined. Falsifier: a collapsed window whose
translog `options_text` carries a cost the previous decline's did not.

## F8 (HIGH, S13) — the hoisted shared card text entered the ask key and the async-slot key. **CONFIRM**

**Repro.** `w78HoistSharedCardText` erases a `{card text: "..."}` group from each row and emits the
blob as a PLAIN header line above the list. Those bytes used to be a balanced annotation group,
which `w77KeyTailOf` strips; as a header line nothing strips them, and `assemblePrompt` feeds
`keyTailStr` into both `askKey` and `mPromptTail` (the async-slot key). A changed shared gloss on
byte-identical action rows was a different question — the wave-74 CG re-ask shape.

**RED evidence (executed).** In the pin, on the JOINED menu built by the live `joinNumberedRows`:
`w76StripBalancedAnnotationGroups(stripDeclineReaskTags(joinedA)) != (...joinedB)` — the base key
tail, composed exactly as the base composed it, DIFFERS across the changed gloss.

**Fix.** The header carries a literal lead (`kSharedCardTextHeadLead`) and `w77KeyTailOf` erases
every LINE that begins with it, before the existing two strippers. Render untouched.

**GREEN.** `w77KeyTailOf(question + joinedA) == w77KeyTailOf(question + joinedB)` AND
`asyncSlotKeyOf(false,13,4,keyTailA,board) == asyncSlotKeyOf(false,13,4,keyTailB,board)`, with the
action name and `{1}` cost pips still IN the key, a changed ACTION row still a different key, and
the strip proven line-anchored (a row quoting the lead mid-line survives). **Prediction:** no
(turn, phase, seat) in the wave-78 corpus shows a repeated ask whose two prompts differ only in a
`Card text shared by option...` line.

## F4 (HIGH, S6) — "the loop closes" was rendered through an untargeted sweeper. **CONFIRM**

**Repro.** `w77OwnLoopStackState`'s threat half asks only `w77StackObjectTargets`. An opposing
`Destroy all(enchantment)` above the loop trigger targets nothing, so the walk returned RESOLVING —
and wave 78 RENDERS that verdict as "the loop closes without another action from you".

**RED evidence (executed).** `w77LoopVerdictFrom(true, true, false) == kW77LoopResolving`, and
`w78OwnLoopVerdictLine(kW77LoopResolving, ...)` contains "the loop closes without another action".

**Fix.** `w78LoopVerdictFrom` takes two more facts off the live stack: (c) an opposing object whose
script is mass removal of a component's TYPE (`w78MassRemovalOfTypeScript`: a removal verb plus an
`all(...)`/`each(...)` selector naming the type, the wildcard, or `permanent`; the type read off the
component with `w78ComponentTypeWord`) is THREATENED, named; (d) any OTHER opposing object that
resolves BEFORE the seat's trigger — `ActionStack::resolve` takes `getLatest`, so a HIGHER index
resolves first — makes the verdict the third face, `kW78LoopUnproven`, never RESOLVING. The new face
is rendered (`their <X> resolves before your trigger - the loop is NOT proven`) and has its own
marker-row key, so every transition re-opens a hold.

**GREEN.** Tranquility's and Wrath of God's repository scripts classify; a sweeper of another type,
an `all()` that does not remove, and a TARGETED destroy do not. The sweeper board renders the
THREATENED bytes; the unclassifiable board renders the third face and makes no closing claim; and
`w77ApplyOwnLoopFeed` leaves the row tag OFF on an unproven board. **Prediction:** no prompt in the
wave-78 corpus contains `own loop verdict: RESOLVING` on a window whose stack block lists an
opposing object above the seat's own loop trigger.

## F3 (HIGH, S5) — a conditional counter was called a guaranteed answer. **CONFIRM**

**Repro.** The join's whole test is `ait->source->magicText.find("fizzle")`. Mana Leak's primitive is
`transforms((,newability[pay[[{3}]] name(pay 3 mana) donothing?fizzle])) forever` — it contains
`fizzle`, so the second counter row printed "spent for nothing unless yours is itself countered or
removed". Paying {3} defeats it with neither exception.

**RED evidence (executed).** Mana Leak's verbatim script contains `fizzle`, and
`w78RedundantCounterTag("Mana Leak")` (the base one-argument form) contains the spent-for-nothing
promise.

**Fix.** `w78ConditionalCounterScript` classifies a pending counter as CONDITIONAL when its script
carries a payment or alternative (`?`, `pay`, `unless`) alongside `fizzle`;
`w78CounterEscapeCost` reads the price out of the script's own `pay[[...]]` group. The tag then
prints `your Mana Leak already targets this on the stack, but that counter is CONDITIONAL - they can
pay {3} to keep it, so this row is NOT redundant`. The unconditional wording is byte-identical.

**GREEN.** Mana Leak conditional with `{3}`; Counterspell and Essence Scatter unconditional and
byte-identical to wave 78 (the `125v146` seq-125 shape still covered); an unpriced escape says the
counter is conditional without inventing a number. **Prediction:** no wave-78 prompt pairs
`Mana Leak` (or any `pay…?fizzle` counter) with "spent for nothing".

## F2 (HIGH, S4) — the drain clause predicted future legality and counted spells. **CONFIRM**

**Repro.** `w78StackDrainNote` ended "the activation above stays available after" from a comparison
with the PREVIOUS asked menu; a pending opposing trigger that removes the activation's source
falsifies it. `w78TheirDrainingTriggerCount` counted every unresolved opposing object, spells
included.

**RED evidence (executed).** The removed literal is asserted absent at every link count 2..9 (on
base it is the clause's fixed tail), and `w78IsDrainLinkType(ACTION_SPELL)` is the predicate the
base walk did not have.

**Fix.** The clause states only what the engine established — "The rows above are what is legal NOW -
this says nothing about what will still be legal after their stack resolves" — and the count skips
anything that is not `ACTION_ABILITY`.

**GREEN.** No "stays available" at any link count; "covers every link" and "what is legal NOW" both
present; abilities counted, spells / damage / phase actions not. CV's own verbatim pin was updated
to the new bytes. **Prediction:** `stack_drain_windows_asked` still fires in the corpus, and no
drain clause names a number larger than the stack block's count of opposing TRIGGERS.

## F5 (MED, S9) — the note-reuse guard ignored `gone`, and the latch used a different key. **CONFIRM**

**Repro (a).** `holdNoteSameWindow(first, unseen, measuredSeq, nowSeq)` never read the newly computed
`gone`, so Astra's sequence — commit `{A,B}`, build an unchanged held window at seq N and suppress
it, rebuild `{A}` at the same N after B disappears — returned the CACHED "no row … is gone" note.
**(b)** ordinal normalisation (`w78StripHandleOrdinal` + occurrence index) lived only in the
bracket; `holdStillStands` compared ordinal-bearing keys, so `Goblin #1` → `#2` printed "unchanged"
while the latch re-opened.

**RED evidence (executed).** `holdNoteSameWindow(false, 0, 2, 2)` is true (the base guard calls the
rebuild the same window), and `!holdStillStands(heldFromG1, g2, &why, holdActionKeyRow)` — the base
latch re-opens on the two windows the bracket calls unchanged.

**Fix.** `gone` is the guard's fifth argument (defaulted 0, so every other caller is byte-identical).
`w78HoldLatchKeys` maps rows through `keyOf` and then through the BRACKET's `w78HoldRowKeys`
(ordinal stripped, occurrence indexed); `takeHold` records that list in a new `mHoldLatchRows`
beside the untouched set (the sibling-window rule still reads the set), and the hold check runs
`w78HoldStillStands` over it, falling back to the set predicate when no list exists.

**GREEN.** The suppressed-then-rebuilt sequence now prints a gone row through the live
`w76HoldReopenNote`; `Goblin #1` → `#2` is unchanged AND does not re-open; a SECOND instance of the
same action, a different card/cost/target, and a row leaving all still re-open with the two `why`
strings unchanged. **Prediction:** no wave-78 seat log shows a `hold check` bracket saying "every
row above was also on the menu" in the window immediately before a `hold re-opened` trace at the
same seam.

## F11 (MED, S11) — the deferred close's continuation checked only the parked arm. **CONFIRM**

**Repro.** The deferral site computes `!armAllowed(park) || mForceCloseArmed`; the continuation that
decides whether a deferred close may arm passed `mRetryPark.forceCloseArmed` ALONE. With a deferred
close, THIS arm still armed and an empty park, it armed on the very next tick, superseded the
outstanding close (`mForceCloseUnrecorded++`, `armed_after_defer_superseding`) and the same-arm
bound held for zero ticks — CV's reading of 1 was local simulation.

**RED evidence (executed).** Walked over the live decision functions in the live order:
`w76ForceCloseDecision(true, parkArmed /*false*/, deferTicks) == kW76CloseArm` on exactly that
state, and `gptForceCloseArm(thisArmArmed)` then returns 1 — one lost close.

**Fix.** One busy predicate for both sites (`w78ForceCloseBusy` / `w78ForceCloseSameArmBusy`); the
continuation and the `w78AtBound` flag both take it.

**GREEN.** On the same state the fixed continuation defers; the wait is still bounded at
`kW76CloseDeferMaxTicks` and the ONE supersede is paid at the bound, not on the next tick; the
identity `sameArmDeferred 1 + armsRefused 0 + unrecorded 1` holds; a busy PARK is still a park
refusal and not a same-arm bound. **Prediction:** the wave-78 corpus shows
`force_close_same_arm_deferred` > 0 with at least one `forced_close` record reading
`armed_at_defer_bound` and none reading `armed_after_defer_superseding` on an empty park.

## F6 (MED, S8) — the clause cutter cut inside quoted abilities and parentheticals. **CONFIRM**

**Repro.** `w78ClauseCutAtOrBefore` tracked neither quotes nor parentheses, so
`Enchanted creature has "Tap: Draw a card. Activate only if you control a Wizard."` cut after the
first sentence — emitting a granted ACTIVATION with its condition removed (a strictly stronger
ability than the card has) and an unfinished quotation.

**RED evidence (executed).** The base scan is re-derived inside the pin over that exact string and
its cut contains `Draw a card.` and not `Wizard`.

**Fix.** `w78UnitDepths` marks every byte of a `"..."` or `(...)` unit (its open and close
included); a boundary is accepted only at depth 0. `w78UnitSafeCut` backs the wave-48 word cut out
of any unit it lands inside, to the byte the unit opened at, and the caller marks the omission
`(...more)`.

**GREEN.** No boundary inside the quote; the render is `Enchanted creature has (...more)` with an
even number of quote characters; a budget wide enough keeps the condition; an unquoted two-sentence
gloss still cuts at its real boundary and renders exactly as wave 78 shipped it (S8 is not
fail-closed); a parenthetical is omitted whole. **Prediction:** no wave-78 prompt contains an odd
number of `"` characters inside a `{card text: ...}` or `{effect ...}` group.

## F7 (MED, S12) — GAINED/LOST were snapshot differences labelled as events. **CONFIRM**

**Repro.** `opponentLifeTrendLine` summed the rises and falls BETWEEN turn samples. Samples 20, 15,
18 over real events +7/−12/+3 print +3/−5, and an all-negative-net game prints no split at all —
the hidden-lifegain failure S12 was built to fix, inside S12's own fix.

**RED evidence (executed).** The base arithmetic is re-derived inside the pin: it reads
`gained 3 / taken 5` on Astra's samples, and `gained 0 / taken 15` on a falling sequence where the
opponent gained 21.

**Fix.** Two new members sum the opponent's life EVENTS at `WEventLife` as the seat receives them
(damage and non-damage alike), and each life SAMPLE snapshots those running totals — so the split
over the sampled span is a subtraction of two event totals. When no event source reaches the site
(either figure < 0) the NET prints alone and the split is DROPPED. The wording names the mechanism:
`life-gain EVENTS put +N on them and life-loss EVENTS took -M off`.

**GREEN.** +10/−12 on Astra's sequence with no `GAINED +3`/`LOST -5` anywhere and the net unchanged;
+21 shown on the all-falling game; the split dropped with no event source; a one-way trend still
byte-identical to wave 77. **Prediction:** in the wave-78 corpus, every `Opponent life trend:` line
carrying a split has `gain − loss` equal to the printed net.

## F9 (MED, S14) — "castable" ignored colours and alternative costs. **CONFIRM**

**Repro.** The keep-X candidate filter was `need = getConvertedCost(); need <= untappedSources`, so
Essence Scatter with only Mountains is a candidate and the clause promises a blue answer the seat
cannot cast; a card's alternative cost was ignored entirely.

**RED evidence (executed).** `w78KeepCandidateStands(2, 2)` — converted cost 2 against 2 sources —
is true, which is the base filter's whole test.

**Fix.** The candidate is priced by `w78PayableSourceCount` =
`ManaEngine::selectAutoTapProducers(this, hc, hc->getManaCost(), anyType, /*preserveOptions*/false)`
— the same colour-aware forecast the cast rows are priced by — and the figure printed is the number
of SOURCES that plan taps, the currency `{leaves N of your M}` already counts in. An empty plan is
not a candidate. A card with an engine-supported alternative cost says so: `w78KeepCostText` appends
`or its alternative cost` rather than inventing a price for a pitch cost no forecast can model.

**GREEN.** `w78KeepCandidateStands(-1, 2)` false, a fitting plan still a candidate, a plan wider
than the pool refused; the alternative-cost wording both ways; a card with no alternative
byte-identical to wave 78. **Prediction:** no wave-78 prompt names a keep-X instant the seat's own
mana line cannot produce the colours for.

## F10 (MED, S1) — the heuristic counter counted records outside its named class. **CONFIRM**

**Repro.** `if (w78LabelAbsent && handedToHeuristic(...))` tests the raw SHAPE flag. A reply that is
only `2 (Hold priority)` — no PLAN — classifies `unlabelled_plan` while `w78LabelAbsent` is true, so
`answer_label_absent_heuristic_played` counted records that could never be reconciled against the
`answer_label_absent` population.

**RED evidence (executed).** Through the live chain (`offProtocolBytes` →
`w78AnswerLabelAbsentShape` → `w75ProtocolDeviationClass`) that reply classifies `unlabelled_plan`
while the shape flag is TRUE and the window IS handed to the heuristic.

**Fix.** `w78LabelAbsentCounted(devClass, handed)` — increment only when the FINAL class is
`answer_label_absent`. **MEASURE ONLY: no parser change**, pinned (`answerSegmentStatic` still
returns no segment for the unlabelled line).

**GREEN.** The no-PLAN case counts 0; the five wave-77 records still count 5; no-heuristic, no
class, and every other class are uncounted. **Prediction:** in the wave-78 census,
`answer_label_absent_heuristic_played` ≤ the count of `protocol_deviation: answer_label_absent`
records that were handed to the heuristic, and the two reconcile exactly.

## F12 (MED, corpus-stats.py) — the census reconciliation omitted `forced_close`. **CONFIRM**

**Repro/RED (executed on the real tool).** The base `corpus-stats.py` run over a mocked read-only
corpus (one ask, one `forced_close`, gameend `protocol_replies: 1` per seat) prints
`CENSUS CHECK: ... (difference 2 = the kinds that carry no round trip: )` — an empty explanation —
because the explanatory list is the hard-coded triple `recovery/defer/wall_miss`.

**Fix.** `ROUNDTRIP_KINDS` names the decision seams that DO ask the model; the explanatory list is
every other decision kind in the census, computed generically (`no_roundtrip_kinds` /
`census_kind_list`), so a kind a future wave adds is explained with no edit. `census_kind_sum`
cross-checks the listed total against the difference and prints a loud MISMATCH line if a kind is on
the wrong side. `--selftest` covers the mocked input, a future-kind census, a round-trip-only census
and the wave-78 corpus's own numbers.

**GREEN.** Same mocked corpus now prints `... (difference 2 = ... forced_close 2)`.
On `matchups-20260911-010157-final`: `RECORD KINDS SUM 2746`,
`CENSUS CHECK: gameend protocol_replies 2740 vs record-kind sum 2746 (difference 6 = the kinds that
carry no round trip: defer 1, recovery 5)` — every non-protocol kind named, and the sums reconcile.
**Prediction:** the wave-78 census reconciles with `forced_close N` named in the difference line and
no MISMATCH line printed.

---

## Weakest evidence

1. **F1's board key.** The identity requires an unchanged `serializeGameState()`. The 12→1 pin is a
   synthetic walk over one board; if the deck123 seqs 41-52 windows actually differ in their
   serialisation (a draining stack, a phase line), the collapse will fire LESS often in the corpus
   than the pin suggests. That direction is safe (an extra ask, never a lost option), but the
   headline "12 → 1" is unproven on live boards.
2. **F9 has no executed board.** `w78KeepCandidateStands(-1, 2)` proves the RULE; the claim that the
   live forecast returns −1 for Essence Scatter over two Mountains is inference from
   `selectAutoTapProducers`' contract, not an executed game. A `ManaCost`-level pin was attempted and
   abandoned: constructing a `ManaCost` inside PARSETEST aborts (`vector<short>` out of range —
   the colour tables are not initialised at that point).
3. **F4's "unclassifiable" is everything.** Any opposing object above the trigger yields UNPROVEN,
   including ones that are provably harmless. This costs the RESOLVING clause on real loops and is a
   deliberate fail-safe, but nothing pins how often it silences a true verdict.
4. **F4 dropped a guard.** The walk's old `if (!observer->currentPlayer && !this->inPlay()) continue;`
   line is gone (it only ever skipped the target scan). Its removal makes detection strictly
   broader; no pin covers whatever board it was there for.
5. **F7's event span.** The split is exact only if `WEventLife` reaches this seat for EVERY opponent
   life change. A life change raised while the seat is not observing would be missed, and the clause
   would then under-report both halves with no way to tell.
6. **F6's degenerate case.** A gloss that OPENS with its quoted unit backs off to index 0 and keeps
   the wave-48 word cut — the half-quote can still be rendered there. No such gloss is known in the
   pool, and none is pinned.
7. **F5's two stores.** `mHoldRows` (set) and `mHoldLatchRows` (list) must stay in step; they are
   written and cleared together at all three sites, but a future writer that touches only one would
   silently split the latch from the bracket again.
8. **F11 is pinned over the decision functions, not through `pollCompletionRetry`.** Driving the
   real continuation needs an endpoint; the pin walks `w78ForceCloseBusy` →
   `w76ForceCloseDecision` → `gptForceCloseArm` in the live order with the live flags, which is one
   call frame short of Astra's demand.
9. **No corpus was run** (lane rule). Every prediction above is falsifiable only by wave 78's own
   corpus.
