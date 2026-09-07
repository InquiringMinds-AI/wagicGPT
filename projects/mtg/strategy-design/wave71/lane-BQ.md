# Wave-71 lane BQ — decision-seam defects (known-bugs L3, L4, L5, L12)

Base `77d82bbe6`. Worktree `worktrees/lanes/w71-BQ`, branch `w71-lane-BQ`.
All four items CONFIRMED. Gates: suite 1279 + 68 AI, **0 failed, 0 timed out**
(`WAGIC_TESTSUITE_THREADS=1`, detached unit `w71-BQ-suite`, log
`~/.gatelogs/w71-BQ-suite.log`); PARSETEST **5302 → 5319 passed, 0 failed**
(+17 checks, exactly this lane's additions). Both link-time guards print OK.
Base binary kept at `~/.gatelogs/w71-BQ-base-wagic` for the counterfactuals below.

---

## L3 — `rowSaysNoOp` reads a Morbid qualifier as a verdict — CONFIRMED

**Repro (read-only, corpus `matchups-20260906-224849`).**
`1788752940-ai_baka_deck123-...-vs-ai_baka_deck152.jsonl` seq 13, rendered row:

```
Cast Tragic Slip {b} {right now: -1/-1 (no creature has died this turn, so Morbid does NOT apply)}
  {leaves 2 of your 3 untapped mana sources untapped} {kills: Elite Spellbinder, Intrepid Adversary}
```
Reply `CHOICE: 1 (Cast Tragic Slip)` → `fallback: noop_row_zero_reask`,
`chosen_text: <refused: noop_row_zero_reask>`. Same shape seq 11 and
`...vs-ai_baka_deck146.jsonl` seq 4.

**Root cause (one line).** `AIPlayerGPT::rowSaysNoOp` lowercased the WHOLE row and
returned true on the substring `does not apply`, whose only emitter
(`morbidMagnitudeClause`) puts it INSIDE the parenthetical qualifier of a real
magnitude — a statement about WHICH of two magnitudes applies — while the row's
own `{kills: ...}` clause, the engine's live verdict, was never consulted.

**Fix.** Two pure predicates in front of the phrase test:
* `rowNamesALiveKill` — a `{kills: <non-empty>}` clause defeats any no-op read.
  The zero forms carry different tags (`{kills 0 of the N ...}`, `{kills nothing: ...}`)
  and an `- INDESTRUCTIBLE, destroy does nothing: <names>` note inside the tag is
  about the bodies the list does NOT name, so it cannot cancel the list.
* `noOpPhraseIsAVerdict` — `does nothing` / `does not apply` count only at
  parenthesis depth 0 and outside double quotes, so a Morbid qualifier and a
  quoted `{card text: "..."}` blob are no longer verdicts. Same unit rule
  `verdictScopeOperative` / `verdictReadsZero` already apply one layer down;
  `rowSaysNoOp` is still the single zero-predicate and nothing else changed.

**RED on base.** `strategy-design/wave71/lane-BQ-noop-row-red.py` replays the base
predicate and the fixed one over every rendered cast row in the corpus:
**2,914 cast rows scanned, 41 verdict changes, every one base=TRUE → fixed=FALSE**,
every one a Tragic Slip row (6 distinct rendered shapes). Pins:
4 negatives (the seq-13 row; the Morbid clause alone; a quoted card text;
a kills-list carrying an indestructible note), 3 positives/regressions
(`{right now: does nothing this turn}`, the Tribute and Devour Flesh scope forms,
the computed-magnitude grammar), 1 echo (the row binds `CHOICE: 1 (Cast Tragic Slip)`
unchanged — no annotation was added or removed).

**Prediction (next corpus).** `noop_row_zero_reask` fires 0 times on a row carrying
`{kills: <name>}`; the Tragic Slip re-ask wording ("says it does nothing") does not
appear against a printed kill.

---

## L4 — an instant `[castable now]` gets no cast row at `priority` windows — CONFIRMED

**Repro.** `1788752934-ai_baka_deck130-...-vs-ai_baka_deck123.jsonl` seq 25 is the
seat's own **turn 12 UPKEEP**, `kind: priority`. Hand prints
`Starstorm ... {r}{r}{x} [instant] [castable now]`; the seven rows are four
Blastminer activations, two cycling rows and the hold row — no cast row. 23 of 23
such windows in that seat; all 25 "Casting decision" asks offered one.

**Root cause (one line).** `AIPlayerBaka::computeActions` has two reach branches —
the W52-I **response-window** branch, which runs `FindCardToPlay(mana, "")` and can
cast an instant, and the **standard-actions** branch, which on the seat's own turn
answers every non-main phase with `selectAbility()` alone; so on its own upkeep,
after blockers, at combat damage and in its end step the seat could not cast an
instant at all and the GPT seat's `FindCardToPlay` override (the cast ask) was
never reached.

**Fix.** The own-turn instant-speed windows (`UPKEEP`, `COMBATBLOCKERS`,
`COMBATDAMAGE`, `ENDOFTURN`, stack empty) now enter the SAME branch as a response
window. Those are exactly the phases the standard branch already answered with
`selectAbility()`, and that branch falls back to `selectAbility()` itself when no
castable card is found, so a window with no instant is byte-identical to before.
`COMBATATTACKERS` is deliberately excluded: it currently asks nothing after the
engine-issued declaration, so a window there would be a NEW ask, not a missing cast.

**RED on base.** New fixture `bin/Res/test/ai_instant_own_end_step_w71bq.txt`
(registered in `_tests.txt`): P1 (AI) starts at its own UPKEEP holding Lightning
Bolt over three Mountains, P2 has a Grizzly Bears; the assert is taken at P1's
`firstmain`, i.e. after upkeep and draw and BEFORE any main phase, so only an
upkeep cast can produce it. On `~/.gatelogs/w71-BQ-base-wagic` (deterministic,
3 runs):
```
==Card number not the same in player 0's graveyard==, expected 1, got 0     (Bolt not cast)
==Card number not the same in player 0's hand==, expected 1, got 2          (Bolt still held)
==Card number not the same in player 1's battlefield==, expected 0, got 1   (Bears alive)
==Test Failed !==
```
Post-fix: `==Test Successful !==`, and green again in the full single-threaded suite.

**Prediction (next corpus).** In any game where an instant prints `[castable now]`
in hand, at least one own-turn `priority` window outside a main phase renders a
`Cast <that instant>` row; the deck130-vs-123 shape (23 of 23 windows with no cast
row) does not recur. Watch also for the new ask volume: upkeep/end-step cast asks
are windows the seat did not previously get.

---

## L5 — the two-step cast reverses itself at "Choose an option for X" — CONFIRMED

**Repro.** deck130 seat, 7 reversals; canonical `deck126 seq 35 → 36`:
seq 35 `PLAN: Starstorm at X=1 to kill Vampire ... / CHOICE: 1 (Cast Starstorm)`;
seq 36 menu `Choose an option for Starstorm: 1. Cast Card Normally … 2. cycling …
3. Decline`, answered `CHOICE: 2 (cycling)`. 21 modal cycling menus in the seat,
4 answered "Cast Card Normally".

**Root cause (one line).** The modal header carries no trace of the cast the model
already answered, so a menu that is the SECOND HALF of one decision renders as a
fresh choice between two equal uses of the card.

**Fix.** `AIPlayerGPT` records the committed cast (`mCommittedCastName` +
`mCommittedCastTurn`) at the one place the model's pick is validated, and the
cast-mode menu's header gains one pure fact
(`castModeCommitmentNote`, file-static, four arguments, provable without a game):

```
 (you answered "Cast Starstorm" at the previous window - this menu completes THAT
  cast, it is not a new offer: the "Cast Card Normally" row casts it, and every
  other row abandons the cast you just chose)
```
**Nothing is removed and nothing is auto-answered**: cycling stays offered, in its
own place, with its own annotations; `req.optionTexts` (the staleness key), the
option order and the answer index are untouched, and the addition is header text
only. Gated on the engine's own `Cast Card Normally` label, on the menu subject
matching the committed card, and on the SAME turn — so a menu armed by the
heuristic's own pick, or by the same card a turn later, carries no claim about what
the model said.

**RED on base.** The helper does not exist on base, so the pins are the discriminator:
1 positive (the wording), 1 echo (one line, leading space, no `\n`, no reply label —
it can be neither a row nor a label), 4 must-not-matches (not a cast-mode menu;
a different card; an earlier turn; no commitment at all). The base behaviour is the
absence of any such header, which the corpus records above document directly.

**Prediction (next corpus).** Of the modal cast-mode menus that follow a model-answered
`Cast X`, the `Cast Card Normally` row is taken in a clear majority (base: 4 of 21
across all modal cycling menus); any remaining reversal is a deliberate one and its
record carries the header fact in the prompt, so it is separable from a misread.

---

## L12 — a reveal answered `BLOCKS:` then `PUT:` — CONFIRMED, narrower than it looks

**Repro.** `1788752940-ai_baka_deck152-...-vs-ai_baka_deck123.jsonl` seq 25,
`kind: reveal`, **phase Blockers, own turn** (the seat had just declared attackers
at seq 24). Reply: `BLOCKS: Sigarda, Champion of Light blocks Vampire.` then
`PUT: 1`. The parser latched the right line and the answer was correct; the defect
is that the PLAN slot was filled with a combat declaration.

**Diagnosis (checked, not assumed).** The seq-25 prompt contains **no** `BLOCKS`
string anywhere — the cue is the board header `Phase: Blockers | It is your turn.`
plus the log's own `Phase: Attackers / You declared attackers: ...` two lines above
the reveal. The reveal is the only seam whose window routinely arms inside a step
named after a different decision.

**Fix (render, not protocol).** The reveal ask states its own scope, once, above the
existing instruction line:
```
This window decides only where the cards listed above go: no attack and no block is
being declared in it, and PUT: is the only answer label it reads.
```
`kReplyProtocol`, `kPlanFirstLead` and the `PUT:` instruction itself are untouched —
nothing about the reply FORM is added or relaxed, and no parser tolerance is added
(a `BLOCKS:` line in a reveal reply is still not an answer). The link-time
reply-instruction guard passes.

**RED on base.** Pins: 1 positive (the scope sentence is present and says both
halves), 1 must-not-match (the line names no OTHER answer label, so it cannot itself
cue one), 1 echo (one line, terminated, saying nothing about writing). On base the
string does not exist.

**Prediction (next corpus).** 0 reveal records whose reply carries `BLOCKS:` or
`ATTACK:`; base rate 1 of 20.

---

## Weakest evidence (in order)

1. **L5 and L12 have no live evidence at all.** Both are pure-render changes pinned
   only by PARSETEST; neither was observed steering a real decision, and the L5
   prediction is a rate claim on 21 base events — a single next-corpus sample cannot
   separate a moderate effect from noise. Treat both as hypotheses until the seat
   review reads them.
2. **L3's blast radius is wider than the 3 seat fires.** 41 rendered rows change
   verdict, but only 6 were ever taken by the model. **35 of the 41 are
   `{kills 0 of the N CREATURE targets at -1/-1}` rows** — rows that are saved by the
   parenthetical rule rather than by the kill list, and which the base guard was
   re-asking for the wrong reason. They are no longer re-asked, and the engine has
   no correct zero-verdict for "a -1/-1 that kills nobody" (it is not a no-op — the
   creature still shrinks). If a wave wants those windows re-asked, that needs a
   real verdict, not the Morbid phrase.
3. **L4 is the only behaviour change to the shared heuristic path.** The suite is
   green single-threaded and the new fixture discriminates the exact window, but the
   suite has no coverage of upkeep/end-step casting for most cards, and the change
   gives BOTH seats (Baka included) new windows — the live cost is extra decisions
   per turn and extra inference at the GPT seat, which only a corpus can price.
   The fixture proves one card (Lightning Bolt) in one phase (upkeep); the other
   three admitted phases are reasoned from the same branch, not observed.
4. **The L4 fixture depends on Baka's own pick.** It asserts that the seat casts the
   only castable card in hand; a future change to `FindCardToPlay`'s scoring could
   turn it red for a reason unrelated to the branch under test.
