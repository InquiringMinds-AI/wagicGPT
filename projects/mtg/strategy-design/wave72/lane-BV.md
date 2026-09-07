# Wave-72 lane BV — render truth A (M6, M7, M8, M9, M12, M16, M17, M18)

Base `0e4e6b9b8`, branch `w72-lane-BV`. Gates on the merged lane tree:
suite **1280 tests, 0 failed / 73 AI tests, 0 failed** (THREADS=1, detached unit
`w72-BV-suite`); PARSETEST **5385 passed, 0 failed** (baseline on base binary
5330 → **+55**, all of them this lane's). One natural Baka game (146 vs 130,
`WAGIC_SELFPLAY_ONESHOT`) completed to a winner at turn 14 — the changed
`GenericChooseTypeColorName` and reveal paths are live-clean. `git diff | grep -c
U+FFFD` = 0. No corpus launched.

Exposure counts below are measured over the two live corpora
(`matchups-20260907-085638` = wave 71, `matchups-20260906-224849` = wave 70) by
scanning the `prompt` field of every record; they are the RED-on-base evidence
for the render items, alongside the confirmation that **none of the nine new
strings exists in `0e4e6b9b8`** (`git show 0e4e6b9b8:...AIPlayerGPT.cpp | grep -c`
= 0 for each).

---

## M6 HIGH — the PRIORITY hold row prices the activations beside it — CONFIRMED, FIXED

**Repro.** `126v146` seq 50 (read in full from the corpus): rows 1/2/3 are Sorin
`+1`, `-2`, `-6`, row 4 an Overgrown Battlement mana row, row 5
`Hold priority: ... (any change re-opens this window; you give up no cast)`. The
reply's PLAN names an attack and takes row 5. Same shape at `126v162` seq 21/27
and `126v152` seq 26 — four of four priority holds in that seat.

**Fix.** A third spelling `kHoldPriorityRowTextActivation` sharing the head every
consumer binds on (`holdRowIndexOf`, `isReservedHoldEcho`), selected by
`holdRowLine(castSeam, activationLive)`. The priority seam computes
`activationLive` from the actions the rows above were printed from —
`asActivatedForCount(...) && !isManaOnlyAction(...)` over the base rows — so the
sentence and the list cannot disagree. A menu with no activation keeps the
wave-53 row byte for byte; the casting seam's spelling still wins where both
could apply.

**Exposure.** 51 windows in the wave-71 corpus and 50 in wave-70 printed
`you give up no cast` on a menu carrying a numbered `+N:`/`-N:` row.

**Pins.** positive (the new sentence), negative (`you give up no cast` absent;
the no-activation row byte-identical to `kHoldPriorityRowText + benefit`; the
cast spelling unaffected), echo (`holdRowIndexOf` / `isReservedHoldEcho` bind it).

**Prediction (falsifiable).** Next corpus: on priority menus carrying a loyalty
or other activated row, the hold row is taken at a materially lower rate than
4/4, and no gameend shows a `hold` taken on a menu whose reply PLAN names one of
its own activated rows.

## M7 HIGH — the card-NAME list — CONFIRMED, FIXED in three parts

**(a) Token names.** `146v130` seq 30 named `Goblin` — a Siege-Gang Commander
TOKEN; deck130 holds no card of that name, so the Silencer's
`@movedto(*[chosenname]|opponentstack)` trigger could never fire again. Fixed at
the **engine**, in `GenericChooseTypeColorName::resolve` (`AllAbilities.cpp`):
tokens are skipped when building the name list. This is not an option removal
under the standing rule — CR 201.3 lets a "choose a card name" effect name only a
*card*, and a token that is not a copy of a card has no card name, so the row was
never a legal answer. The fix is seat-agnostic: the human menu loses it too.

**(b) The deciding number.** `namedCardRemainingTag` renders
`{copies not yet in a public zone: N of theirs, M of yours - only a copy that is
not already public can still be cast}`, computed over the same nine zones the
engine builds the list from, minus the public ones (inPlay, graveyard, stack,
exile, commandzone, reveal). **This is not new information**: both decklists are
in the system prompt and the public counts are already on the row, so it is a
subtraction the reader could already do and provably did not (`146v126` seq 17's
reasoning: "Opponent has Sanguine Bond on the battlefield ... I should choose
Sanguine Bond", while four live Tributes sat unmarked at row 8).

**(c) Order.** The engine walks `inPlay` first, so the rows a *cast* trigger can
never fire on were rendered at the top of a menu whose own convention is
"usually-correct option first" — and the format example, written out from row 1,
then named the wrong card in the model's own words. Rows are now `stable_sort`ed
by the remaining-copy count, highest first. Nothing is removed and the answer
index still means what `applyMenuChoice` thinks it means: a permutation vector is
inverted on the pick and the ask narration is permuted with the rows. The header
sentence now says what the two counts are worth and states the ordering.

**Exposure.** 9 name menus in wave 71, 6 in wave 70 (small, but each one is a
permanent, game-shaping choice).

**Pins.** positive/negative on the tag, the header's two new sentences, echo
(strip leaves no residue; a reply echoing the new tag still binds its row).

**Prediction.** Next corpus: zero token names on any `Choose a card NAME` list,
and the chosen row's `{copies not yet in a public zone: ...}` is > 0 in every
name choice.

**Weakest point of this item:** the reorder is live-path code that PARSETEST
cannot reach (it needs a game with a `chooseanameopp` permanent). The
permutation and its inverse are pinned only by inspection; the smoke game did not
produce a name menu.

## M8 HIGH — an opponent permanent's `{effect:}` is in THEIR voice — CONFIRMED, FIXED

`152v126` seq 35 read `Exquisite Blood ... {effect: "Whenever an opponent loses
life, you gain that much life."}` from the wrong seat, concluded its own damage
was refunded, alpha-struck into a printed "blocking can leave them as high as 12"
and lost the game. Primitive and engine both verified right by the reviewer (seq
40: opponent 12 → 4, no gain).

**Fix.** `boardEffectTag` gains an `opponentsCard` flag, set only by the opponent
battlefield's `describeZoneCards` call, and emits
`(THEIR card, written from THEIR seat: "you"/"your" in this text means THEM, and
"opponent" in it means YOU)` inside the existing `{effect ...}` group — but only
when `textCarriesSeatVoice` finds an actual seat pronoun (word-boundary matched,
so "Young Wolf" is not a hit).

**Design choice, stated deliberately:** the frame STATES THE MAPPING rather than
rewriting the sentence. The reviewer proposed a rewrite (`whenever YOU lose life,
THEY gain that much`); a regex rewrite of arbitrary rules text is a surface that
can be false, and a false surface is the defect this fixes. The mapping is
provable for every card.

**Exposure.** 766 such tags across 598 windows (wave 71); 1,256 across 799
(wave 70) — every opponent non-creature non-land permanent whose text names a
seat: Underworld Dreams, Howling Mine, Staff of Nin, Intruder Alarm.

**Prediction.** Next corpus: no reasoning trace attributes an opponent
permanent's "you" to the reading seat. (Measured by grepping reasoning for a
card's own text quoted with an inverted subject; a single occurrence refutes.)

## M9 HIGH — the `{reserve:}` STRANDS verdict — CONFIRMED, FIXED in three parts

**(a) The land drop.** `162v146` seq 10 printed
`VERDICT: taking this row STRANDS Howling Mine this turn - 1 source cannot pay 2`
in the same prompt as `Land drop: NOT yet used this turn` and an Island in hand;
seq 11 played a Swamp, seq 12 cast the Mine. `sorceryReserveClause` now takes
`landDropSources`, folded into the remainder before the guard, so the false
verdict is **not printed at all** in that case. The count is `1` iff a legal land
play exists that does not enter tapped (`castBodyEntersTapped`, the engine's own
`tap(noevent)` idiom, deliberately unqualified so a mis-read under-counts).

**(b) Consequence first.** The clause now opens
`{reserve: TAKE THIS ROW AND YOU CANNOT CAST <X> AT ALL THIS TURN.` before any
window clause — `162v152` seq 17 quoted the old trailing "your main phase this
turn is its last window" as a *promise* ("That means I will cast it in Main Phase
1"), took the row and lost the game.

**(c) The decline carried across the step.** REFUTED AS SPECIFIED, replaced.
Widening `reserveDeclineHonoured`'s phase test to span upkeep→draw would fire on
nothing: the draw itself moves the hand and library counts, so the full board key
(`serializeGameState`, required since #W67-AZ R3) differs by construction and the
window is correctly re-asked — which is exactly what the corpus shows. Shipping
the widening would have been a fix that fires on nothing, and loosening the board
key is the one thing the doctrine forbids. Instead the seat now keeps a
TURN-scoped memory of the reserve decline (cast-set key + turn) and RE-SERVES the
window with a prompt-only note (the reviewer's own first option):
`[reserve row declined earlier THIS TURN: ... Nothing is withheld - the row is on
the list below and taking it is still legal.]`, counted as
`reserve_decline_windows_noted` on the gameend census. No window is suppressed.

**Exposure.** 39 STRANDS renders in wave 71, of which **20 were on a prompt
printing an unused land drop**; 13 / 4 in wave 70.

**Prediction.** Next corpus: `reserve_decline_windows_noted` > 0 in any game with
a reserve row, and no STRANDS verdict is overridden-and-vindicated by the
reasoning (the `162v146` s10 shape). Zero false verdicts is the falsifier.

## M12 MED — the legend-rule choice's false header — CONFIRMED, FIXED

`146v162` seq 19/26 rendered `TARGET CHOICE for Nadaar, Selfless Paladin - its
"put a card into the graveyard" ability (this spell/ability is already on the
stack ...)`. Nadaar has no such ability and the legend rule is a state-based
action. `legendRuleHeaderText` replaces the generic header when
`legendRuleSelect` is set: it names the rule, denies the three things the pick is
not, says the picked copy goes to the graveyard and the others STAY, and asks for
the copy being GIVEN UP. `legendRuleTargetClause` (the no-merge fact) is still
appended, unchanged. 2 renders in each corpus.

**Prediction.** Next corpus: no reasoning trace at a legend-rule window
speculates about a prompt bug or reads the list as "which copy to keep".

## M16 MED — a loyalty row truncating a SIBLING loyalty clause — CONFIRMED, FIXED

The reviewer placed the truncation on the `-6` row; the corpus places it on the
`+1` and `-2` rows, and the defect is real and larger than reported.
`boardEffectSnippetFocus` gave the FOCUSED clause `maxLen*2` and the last clause
`maxLen` — Sorin's `-6` clause is 153 bytes against 140, so on every row that is
not the `-6` it printed `...to the battlefield under...` and stopped: the clause
that says WHOSE board the reanimated creatures arrive on, which is the entire
reason a wall deck takes the `-6`. The sibling clauses ARE the other rows of that
menu. Every clause whose lead is a loyalty token now gets the focus allowance;
non-loyalty clauses are budgeted exactly as before (pinned).

**Exposure.** 119 renders carried `under...` in wave 71, 72 in wave 70.

**Prediction.** Next corpus: `to the battlefield under your control.` appears in
full on every Sorin row, and the `-6` is taken at least once on a board with 3+
opposing creatures.

## M17 MED — the reveal/search window — CONFIRMED, FIXED in three parts

- **Destination zone.** `revealDestinationZone` reads `moveto(<zone>)` out of the
  same option-one script `describeRevealFilter` already parses, and the ask now
  ends `The card you send to "get a human" goes to YOUR HAND.` An unparseable
  script says nothing rather than guessing. 9 reveal asks in wave 71 / 19 in
  wave 70 named only the script label.
- **Duplicates.** Rows gain `{you already have this card: N on your battlefield,
  M in your hand ...}` counted off the reader's own zones, with the legend-rule
  consequence stated only for a legendary already on the battlefield
  (`152v162` seq 38 took a third Katilda holding one and controlling one).
- **The stale plan.** `kRevealPlanScopeFact` states that the carried PLAN was
  stated at an EARLIER window and that this window's answer is a card number.
  Nothing about the reply FORM changes and no label is added the parser reads —
  `126v146` seq 8's plan was "Next turn, cast Sanguine Bond and Exquisite Blood"
  served over a 50-card search.

**Prediction.** Next corpus: zero reveal windows answered with an `ATTACK:`/
`BLOCKS:` line, and no duplicate legendary taken off a reveal list while a copy
is on the seat's own battlefield.

## M18 MED — the crack-back header and the tap-you-out row — CONFIRMED, FIXED

**(a)** The evasive split is gated on the seat HAVING bodies, on the wave-61
reasoning that with none "nothing you control can block it" says nothing. It says
the whole thing. `126v146` seq 8 (wave 70) printed `for up to 6 - you would be at
5` to a WALL deck holding no creature, with nothing anywhere saying the total was
unblocked in full — and the doctrine's own rule is that the model confabulates a
defence into the gap. The line now adds
`- you control NO creature, so every point of that is unblocked: nothing on your
battlefield can stop any of it`, read off the same flag the split is gated on.
The wave-61 NEGATIVE pin that asserted silence here is SUPERSEDED and rewritten
in place with the reason.
**Exposure.** 108 crack-back windows in wave 71 and 122 in wave 70 were rendered
to a seat whose own battlefield line reads 0 creatures.

**(b)** `tapOutCrackBackClause` gives a row that leaves 0 sources the arithmetic
its siblings have: what it taps stays tapped until the seat's OWN untap step,
which comes AFTER the printed crack-back, so the row means holding 0 sources
through all of it. Gated on exactly the render's own conditions
(`crackBackNextTurnDue` over the same phase and seat, `crackBackTotalOver` for the
number) so a row can never point at a line that is not above it. No instant is
named and no answer is claimed to exist — it is a rule about untap timing.

**Prediction.** Next corpus: on casting menus printing a crack-back line, the
tap-you-out row is taken less often than the leave-N row when the crack-back is
lethal; and no gameend shows a seat tapping out into a printed lethal crack-back
with an instant still in hand.

---

## What was NOT done, and the weakest evidence

- **Every item in this lane is a RENDER change with no live GPT window behind
  it.** The gates are PARSETEST (pure functions, all three shapes per change) and
  the suite; the corpus counts above prove the DEFECTIVE string was printed N
  times on the base, not that the NEW string changes a decision. The next
  corpus's seat reviews are the real gate, which is what the predictions are for.
- **Weakest single item: M7's reorder.** It is the only change in this lane that
  moves an answer index. The inverse permutation and the narration permutation
  are correct by inspection and are exercised by no test — `chooseanameopp` needs
  a Silverquill Silencer on the battlefield, which neither the suite nor the
  smoke game produced. If a merge-tree gate is cheap, a probe deck with the
  Silencer is the honest check.
- **M8's frame is a mapping, not a rewrite** — a model that ignores the frame and
  reads the quoted sentence literally is still wrong. This is the conservative
  half of the reviewer's proposal, chosen because the aggressive half can print a
  false sentence.
- **M9(c) is a NOTE, not a suppression.** If the next corpus still shows the
  upkeep→draw reversal with the note printed, the defect is strategy, not render,
  and belongs in the guide.
- The M16 defect was reported on the wrong row by the reviewer; the fix covers
  the row it actually occurred on (the siblings) and the reported row (already
  whole, pinned as a regression).
