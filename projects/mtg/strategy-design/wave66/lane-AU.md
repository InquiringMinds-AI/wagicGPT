# Wave-66 lane AU — the five findings of the codex adversarial review

Base: `03afa7065` (master after the AQ-AT merge + the review commit). Every finding was VERIFIED
against the code first, then fixed in its general form. Gate on this tree: **PARSETEST 4906 passed,
0 failed** (baseline 4879/0, +27 cases); **suite THREADS=1 1273 tests, 0 failed, 67 AI tests, 0
failed, 0 timeouts** — the baselines exactly.

## Verdicts

| # | Sev | Finding | Verdict | Where it was proved |
|---|-----|---------|---------|---------------------|
| 1 | HIGH | `holdKeyRow` normalises EVERY digit inside a `[...]`/`{...}`, so a row whose verdict moved `kills 0 of` -> `kills 1 of` keeps the hold | **CONFIRMED** | `holdKeyNumbersNormalised` (old `:24107`) replaced any digit run at `depth > 0` with `#`; lane AS's own report names the risk ("a magnitude genuinely changed while every word stayed the same is now forgiven") |
| 2 | HIGH | `receiveEvent` counts every library->hand move in the draw phase; `drawsStillAhead` subtracts it from a forecast built from the turn-based draw + static extras | **CONFIRMED** | the old counter tested only `from == library && to == hand && phase == DRAW && currentPlayer == drawer` — a cantrip, a cracked Clue or an ETB draw all satisfy it, and `WEventZoneChange` carries no source |
| 3 | MED | `engineKindForScript` calls any `:` before `token(` a repeatable engine | **CONFIRMED** | `low.rfind(':', tk) != string::npos` matches `@movedto(this\|mybattlefield):token(...)`, a one-shot ETB; the negative pin used a colon-free stand-in |
| 4 | MED | `attackersHeldSickLine` claims "entered this turn" and "attacks from your NEXT turn on" for every omitted sick body; the caller tests neither | **CONFIRMED** | the caller's walk filtered on `isCreature() && hasSummoningSickness()` only — no CANTATTACK / DEFENSER / attack-cost test, and sickness does not imply entry (a creature GAINED this turn is sick) |
| 5 | MED | `askExemplar` falls back to row 1 when every row is a no-op | **CONFIRMED** | the `pick = 0` initialiser survived the all-dead loop; the PARSETEST at `:68970` pinned `CHOICE: 1 (Cast A)` |

No finding was refuted.

## Mechanisms

**R1 — the hold key forgives a life total BY NAME.** `holdKeyNumbersNormalised` /
`holdKeyManaCostBraceAt` are gone. `holdKeyLifeProjectionsNormalised` normalises a number only when
it is introduced by one of six enumerated clauses — `you would be at `, `they would be at `,
`at life `, `leaves them at `, `so taking it costs you `, and `(your N life` — and leaves every
other digit in the key. A kill count, a damage figure, a survivor count, a mana cost, an instance
ordinal and every verdict word therefore re-open the window. `holdRowBenefitClause` says exactly
that ("differs only by a LIFE TOTAL it projects ... Any OTHER change re-opens this window,
including a kill count, a damage figure, a survivor count, a price"); its three literal pins were
updated in place. Both corpus runs lane AS shipped this for (`152v162` s42->s43, `130v126`
s37->s38) are re-verified GREEN under the narrowed rule, end to end through `holdStillStands`.

**R2 — a draw retires a forecast draw only if the forecast counted its source.** `WEventcardDraw`
now carries the drawing ability's `source` card (`WEvent.h`, `WEvent.cpp`, `AllAbilities.cpp` —
one construction site, defaulted argument, no other caller changes). `AIPlayerGPT::receiveEvent`
counts on that event instead of the zone change and routes each draw through the new pure
`drawFeedsStepForecast(script, onBattlefield, isTurnBasedRule)`: a permanent with an
`@each my draw` / `@each opponent draw` line that draws (the exact hook `drawStepExtrasScan`
reads) or the turn-based rule counts; a cantrip resolving from the stack, a cracked Clue and an ETB
draw do not. Where provenance cannot be known (no source card at all) the MINIMUM is taken — the
turn-based draw, once, and no more (`sourceless && counted == 0`). Draws that were NOT counted go
to `mDrawStepOther{Mine,Theirs}`, are never subtracted, and are stated on the forecast line by
`drawsUnattributedClause` ("you also drew N cards this step from a spell or an ability, which this
forecast never counted, so nothing above is subtracted for them"). With nothing unattributed the
line is byte-identical to lane AQ's.

**R3 — repeatable is what the line's HEAD says.** New pure `repeatableTokenEngineLine`: the head is
everything before the line's first `:`; a cost head (it carries a `{`) is an activated ability and
is repeatable, an `@` head is repeatable only when it recurs (`each` / `upkeep` / `attack` /
`combat`). `@movedto(...)`, `@targeted`, `@damaged`, `@discarded` are one-shot. The negative
PARSETEST now uses Wagic's real grammar `@movedto(this|mybattlefield):token(Soldier,...)` and keeps
the colon-free form as a second negative.

**R4 — say only what is known.** `attackersHeldSickLine` drops "entered this turn without haste"
and drops the blanket next-turn promise; it now reads "NOT offered above and NOT able to attack
this turn (summoning sick) - ... A plan that names one of them as an attacker THIS turn cannot be
executed." The next-turn sentence is separate and names only the bodies for which the caller tested
that `MTGCardInstance::canAttack`'s other gates would pass once the sickness lifts: no CANTATTACK,
no flyers-only bar, no DEFENSER without CANATTACK, not a Battle, `attackCost <= 0`.

**R5 — the all-dead menu exemplifies no row.** `askExemplar` prefers the decline row when every row
is a no-op and the menu has one (passing is the least harmful answer and is a real row); with no
decline it returns the template `CHOICE: <row number> (<that row's short name>)` and sets
`*usedRow = 0`. The new pure `exemplarSentence` then reads "(a worked example of the FORMAT only -
every row on this list does nothing right now, so no row is exemplified: pick the least harmful)".
An ordinary menu's sentence is byte-identical to the shipped one.

## RED evidence

All five mechanisms were reverted **simultaneously** in one build (the hold key back to
digit-blind; `drawFeedsStepForecast` back to unconditional true — which is exactly the old
"every library->hand move counts" rule; `repeatableTokenEngineLine` back to "any `:` before
`token(`"; the sick line back to its two unsupported claims; `askExemplar` back to `pick = 0`) with
every new case in place. Result: **4883 passed, 23 failed**. The failures, by finding:

* **R1 (7)**: `MUST-RE-OPEN kills 0 of -> kills 1 of ...`, its end-to-end `holdStillStands` twin,
  `MUST-RE-OPEN a damage figure, a survivor count and a price all stay in the key`, `NEGATIVE an
  anchor's words alone ...`, `MUST-RE-OPEN a source count moving is not a life projection`, `a
  {right now:} survivor count moving DOES re-open the window`, `MUST-NOT-MATCH the hold key is NOT
  stripAnnotations and NOT digit-blind`. (Two pre-existing `#W64-AJ` cases and two `#W66-AS H7`
  cost/ordinal cases also went red under the crude revert — the revert is not byte-exact to wave
  65's code, which is why it re-broke them.)
* **R2 (3)**: the cantrip, the cracked-Clue/ETB pair, and the off-battlefield script all reported
  as feeding the forecast.
* **R3 (1)**: `@movedto(this|mybattlefield):token(Soldier,...)` classified as a TOKEN ENGINE.
* **R4 (4)**: the "entered this turn" assertion, the unconditional NEXT-turn promise, the
  Defender/CANTATTACK case, and the no-verdicts-supplied default. (`#W66-AT H6`'s own header case
  also went red on the reverted string.)
* **R5 (3)**: the all-dead menu exemplified `CHOICE: 1 (Cast A)` and the sentence still said
  "written out from row 1".

Restored, rebuilt, re-gated: 4906/0.

## Predictions for the wave-66 corpus

1. No hold survives a window in which a row's `kills N of` / damage / survivor count changed —
   falsifier: two consecutive same-seam records where the earlier reply took the hold row and the
   later window's `options_text` differ in a bracketed count that is not a life total.
2. No `DRAW FORECAST` line reports `0 life LOST BY YOU ... (the whole step has already resolved)`
   in a draw step where a punisher-counted extra draw is still on the stack; where a cantrip or a
   cracked Clue resolves in the draw step the line carries the `you also drew N card(s) this
   step ...` clause.
3. No sweep or sacrifice bracket names a body whose only `token(` line is an `@movedto(...)` ETB as
   a TOKEN ENGINE.
4. No attackers window promises a NEXT-turn attack for a Defender, a CANTATTACK body or a body
   with an unpaid attack cost; no window says "entered this turn" of a held-out sick body.
5. No `named_row_reask` or mandatory menu exemplifies a row that `rowSaysNoOp` flags; on an
   all-dead menu the ask carries the FORMAT-only sentence.

## What I did NOT verify

* **R2's attribution is verified at the predicate and at the renderer, not end to end in a game.**
  There is no suite fixture that observes the DRAW FORECAST text, and the engine emits no draw
  event a fixture can assert on, so the seam evidence is: the new `WEventcardDraw::source` value
  (one construction site, `AADrawer::resolve`'s own `source`), the pure predicate's PARSETEST
  cases, and the reverted-rule RED. **This is the weakest evidence in the lane.** A live draw step
  with a Howling Mine and a cantrip is the falsifier; prediction 2 states it.
* **The `sourceless && counted == 0` minimum assumes the turn-based draw's ability has no
  battlefield card behind it.** I read `Rules.cpp:228` (`parseMagicLine(..., &(g->ExtraRules[i]))`)
  and `GameObserver.cpp:119` (`ExtraRules = new MTGCardInstance[2]()`, magicText empty, in no
  zone) and matched both pointers explicitly, but I did not observe a live draw step in a debugger
  to confirm the pointer identity at runtime. If it does not hold, the draw falls into the
  `sourceless` arm anyway (source not on a battlefield, empty script) and is counted once — the
  same minimum.
* **R1's six anchors are the clauses I found by grepping the emitters** (`you would be at`,
  `they would be at`, `at life`, `leaves them at`, `so taking it costs you`, `(your N life`). I did
  not sweep every emitter for a further life-total phrasing. A missed one costs a re-opened window
  (a question asked), never a suppressed row — the miss direction is the safe one.
* **R5's decline-preferred branch is unreachable on today's menus**: a decline row is not flagged
  by `rowSaysNoOp`, so an all-dead menu carrying one does not exist in the corpus. It is written
  for the case where one is annotated dead, and pinned by a synthetic case.
* **R4's `attackCost` gate** is read straight off the field; I did not trace which abilities set it.
* No live model probe was run (fixtures and PARSETEST are the gate, per the brief).
