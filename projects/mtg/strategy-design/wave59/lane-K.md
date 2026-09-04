# Wave-59 lane K — K6: Intrepid Adversary's repeated `{1}{W}` payment stops early

Base: master `35c9b85bf`. Item: wave58/deck152/review.md HIGH-1.

## Verification first (card data is INNOCENT)

Scryfall, `Intrepid Adversary` {1}{W} Creature — Human Scout 3/1: "Lifelink / When this creature
enters, you may pay {1}{W} any number of times. When you pay this cost one or more times, put
that many valor counters on this creature. / Creatures you control get +1/+1 for each valor
counter on this creature." The primitive (`borderline.txt:57861`) matches clause for clause:
20 `choice name(Add N counters) thisforeach(variable{N}) ability$!may ... pay({1}{W}) ...
counter(0/0.1.Valor) oneshot!$ controller` lines plus
`auto=thisforeach(counter{0/0.1.Valor}>0) lord(creature|myBattlefield) 1/1` (the anthem is
"Creatures you control", self included — correct).

The second card in the repro is also faithful: `Katilda, Dawnhart Prime` (`borderline.txt:61432`),
"Human creatures you control have '{T}: Add one mana of any of this creature's colors'", scripted
`auto=lord(creature[human&<colour>]|myBattlefield) {T}:add{<colour>}` — the engine's standard
grant idiom, used by ~60 primitives (`lord(sliver) {T}:`, `lord(land|myBattlefield) {T}:`,
`lord(eldrazi|mybattlefield) {T}:`, `lord(Food|myBattlefield) {T}:` ...).

**No card data was changed.** (Two cut-off Codex runs on this lane had left an uncommitted
rewrite of Katilda's five grant lines into `transforms((,newability[{T}:add{X}]))` and a fixture
comment asserting "Katilda's grants vanish when she taps". Both were reverted/corrected: the
grants do NOT vanish — instrumenting `ActionLayer::manaObjects` at the failure point showed all
nine granted producers still registered, on untapped grantees, with `tapflag=1 tapped=0 sick=0
inplay=1 out=1`.)

## Mechanism (engine, general)

`ALord::_added` (and `ATeach`) grant an ability by cloning it and calling
`MTGAbility::propagateSource(a, grantee)`. That function repointed `a->source` and every nested /
multi child's source — but **not the ability's cost**. The clone's `ManaCost` IS deep-copied
(`ManaCost` copy ctor -> `ExtraCosts::clone`), so each grantee owns its own `TapCost`; that
`TapCost`'s `ExtraCost::source` kept naming the **granting** card.

Two consequences, both live:

1. `TapCost::isPaymentSet()` returns 0 when `source->isTapped() || source->hasSummoningSickness()`.
   `ManaEngine::producerUsable` ends in `cost->extraCosts->canPay()`. So **the moment the granter
   tapped, every grantee's granted ability became unpayable at once** — for the payment planner
   (`AIPlayerBaka::canPayMana`), for the human cast gate, and for the render count
   (`ManaEngine::potentialColorReach`).
2. `TapCost::doPay()` taps `ExtraCost::source` — i.e. activating a grantee's ability tapped the
   **granter**, not the grantee.

That is exactly the corpus shape. deck152 seq 70-79: 12 sources, 7 W-capable, 6 payments
affordable; the engine made 4 and the mana line then read `0 total (no untapped sources)` with
four untapped W-capable permanents standing — because the four that remained (Elite Spellbinder,
Briarbridge Tracker, Intrepid Adversary #1, Augur of Autumn) held ONLY Katilda-granted abilities,
and Katilda was by then tapped. Second instance same game (seq 59-60): 7 sources -> 2 counters.

Cost of the bug in that game: 2 valor counters = +2/+2 on eight creatures on the turn the seat
needed to close.

## What shipped

- `projects/mtg/src/MTGAbility.cpp` — `MTGAbility::propagateSource` now also repoints the
  ability's `getCost()->extraCosts` (the `ExtraCosts::source` and every `ExtraCost::setSource`)
  at the new source. One place, every grant path, every extra-cost kind. Rules basis: a granted
  ability's costs are paid by the permanent that HAS the ability.
- `projects/mtg/src/TestSuiteAI.cpp` — two harness commands (no engine behaviour):
  `aipaycost` drives the AI's real optional-mana payment (`canPayMana` + `payTheManaCost` +
  drain of the queued producer clicks) on the pending `mExtraPayment`; `assertmanasources N`
  pins `ManaEngine::potentialColorReach` — the SAME call that renders the `Mana available:`
  line — so a lying mana line is a test failure.
- `projects/mtg/bin/Res/test/intrepid_adversary_repeated_payment.txt` (+ `_tests.txt` entry) —
  5 Plains + Katilda + 2 Luminarch Aspirant + Augur of Autumn + 2 Elite Spellbinder + a resident
  Intrepid Adversary; cast a second Intrepid Adversary, ask for 6 counters, pay six times.
  Asserts `tappedinplay:12`, `assertmanasources 6` mid-payment, and the payment narration.

No parse/render STRING changed (the mana line's shape is untouched — only its number is now
true), so no PARSETEST cases were added; the count is unchanged at 3455.

## RED on base / GREEN after

RED verified on the base engine (`35c9b85bf`) built with the harness commands only and the card
data untouched: `~/.gatelogs/w59-K-red1.log`
— `==tapped battlefield cards problem for player 0. Expected 12, got 6==` and
`TESTSUITE assertmanasources: expected 6 sources got 0`. Only the five Plains and Katilda ever
tapped; three of six payments landed. GREEN with the `propagateSource` change and nothing else
(`~/.gatelogs/w59-K-green1.log`).

## Gate (hermetic `make -B`, detached unit `w59-K-gate`)

- build: 0 errors (`~/.gatelogs/w59-K-make.log`)
- suite, `WAGIC_TESTSUITE_THREADS=1`: **1246 tests, 0 failed, 0 timed out**, **61 AI tests, 0
  failed** (baseline 1245 + this lane's 1 fixture; the two known concurrency-only failures do not
  appear single-threaded)
- PARSETEST: **3455 passed, 0 failed**

## Predictions (falsifiable, wave-58 corpus shapes)

1. deck152 vs deck126 rerun, seq 70-79: with 12 sources / 7 W-capable, Intrepid Adversary
   receives **6** valor counters, not 4; the seq 59-60 instance (7 sources) receives **3**, not 2.
2. In any decision window where Katilda is tapped and untapped Human creatures remain, the
   `Mana available:` line is **non-zero** and lists those Humans — `0 total (no untapped sources)`
   with untapped grantees on board occurs **0** times.
3. Across a new corpus, **0** occurrences of the granter-taps-instead-of-grantee shape: a payment
   narration naming a grantee while the granting permanent is the card that becomes tapped.
4. Generalization, no regression: the ~60 other `lord(...)`/`all(...)`-granted activated abilities
   with tap or sacrifice costs (Slivers, `lord(land|myBattlefield) {T}:`, Eldrazi, Food) keep
   working — suite 0 failed already holds; expect no new fallback/defer spike attributable to
   mana-source counting in the next corpus.

## What I did NOT verify

- **No live run.** Fixtures + gate only; no selfplay game, no probe deck, no model call. The
  corpus predictions above are unverified.
- **The rendered string.** `assertmanasources` pins `potentialColorReach`, the engine call behind
  the line; the assembled `Mana available: ...` text itself was not exercised in this lane.
- **Non-tap extra costs on granted abilities** (`SacrificeCost`, `TapTargetCost`, snow, energy)
  ride the same repointing by construction, but have no fixture here.
- **`ATeach`** (the aura/enchantment grant twin, `AllAbilities.h:4099`) calls the same
  `propagateSource` and therefore gets the same fix; I did not find or build a card case for it.
- **The harness command's fidelity to the live path**: `aipaycost` calls `canPayMana` +
  `payTheManaCost` directly, which is what `AIPlayerBaka::selectAbility`/`doAbility` call on a
  pending `mExtraPayment`; the live path adds the menu plumbing around them, which the fixture
  drives with `choice 0` but does not otherwise assert.
- **Whether the payment loop has any OTHER early-stop cause.** The one proven here fully accounts
  for the two repros in deck152; a different board (no granted producers) was not tested.
