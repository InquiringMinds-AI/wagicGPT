# Priority, Mana & Castability UX Redesign

**Status**: BUILDING (2026-07-10) — user mandate: full mechanism replacement in
one campaign, NO interim patches or light fixes on the old system ("we need to
fix the mechanism, or when we do, all of that other work will need to be
revised"). Everything downstream — PC frontend with PC-native UX, the PSP
frontend, the LLM UX, the strategy guides — consumes the revised mechanism.
The DecisionRequest→Action contract is therefore the FRONTEND-AGNOSTIC gameplay
API: PC UI, PSP UI, and AI policies are peer consumers of one decision stream.
**Origin**: wave-3 strategy revision revealed that guide content was being spent
compensating for platform flaws. User direction: fix priority passing / phase
stepping, mana selection, and castability indication at the engine+UX layer, for
the benefit of BOTH the human player and the LLM pilot. Baka is no longer a
mandatory always-on fallback — it should survive as an *optional* opponent and
can be rewritten against the new structure.

**Reference model**: the official-client pattern (Arena/MTGO): auto-pass with
stops, castable-card highlighting, auto-tap with sensible color selection.

---

## 1. What the code does today (ground truth, from the 2026-07-10 investigation)

### Priority / phases
- Every actor funnels phase advancement through
  `GameObserver::userRequestNextGamePhase` (`GameObserver.cpp:320-366`). The
  stop-vs-skip decision (352-361) creates an interrupt window ONLY if: combat
  ORDER/TRIGGERS/damage step, `opponent()->isAI()`, the per-phase human option
  flag (`Options::optionInterrupt(phase)`, `GameOptions.h:75-88`), or the
  per-player `offerInterruptOnPhase`. Otherwise the phase advances with NO
  window.
- Stack pushes pre-seed who may respond: `ActionStack::addSpell`/`addAbility`
  set `interruptDecision = DONT_INTERRUPT` for most cases
  (`ActionStack.cpp:737-750, 837-855`), and `AddNextGamePhase` pre-declines the
  acting player (776-786). The offer state machine (`ActionStack::Update`,
  1062-1195) then only asks whoever is still `NOT_DECIDED`.
- **Consequence**: MTG's actual rule — every player gets priority at every
  point — does not exist. Windows are opt-in per phase-flag and per-ability
  type. Corpus evidence: a control deck received 4 opponent-turn windows in 6
  games; Cruel Edict resolved twice through open UU + Counterspell.
- Humans click through every phase (`MTGGamePhase.cpp:37-60`); the AI passes by
  interpreting an empty clickstream as "nothing to do"
  (`AIPlayerBaka.cpp:4536-4613`, mirrored `AIPlayerGPT.cpp:1702-1787`).

### Combat seams
- `chooseBlockers` is reached only from `computeActions`' standard branch:
  currently acting + NOT interrupting + **stack has zero NOT_RESOLVED items** +
  blockers phase + opponent's turn (`AIPlayerBaka.cpp:3761, 3961-3966`). Any
  attack trigger on the stack silently skips the entire block decision.
  Corpus evidence: 3 blocker asks in 21 games; a lethal 5-attacker alpha strike
  produced no ask.

### Mana / castability
- Humans must tap lands manually FIRST: a hand-click is a no-op unless the mana
  POOL already covers the cost (`MTGRules.cpp:302-389`, `canAfford` at 355;
  payment at 531). No auto-tap, no partial payment, no potential-mana view.
- The AI has what humans lack: `getPotentialMana`
  (`AIPlayerBaka.cpp:1537-1575`), color/land selection (`canPayMana`,
  1576+), auto-payment (`payTheManaCost`, 1380-1535).
- Legal-action computation is scattered and duplicated 3+ ways: the
  `MTG*Rule::isReactingToClick` family (pool-based, human), Baka's
  `FindCardToPlay` + `canAfford`/`canPayMana` (potential-mana), and GPT's
  `roughlyPayable` + 601.2c `TargetChooser` validity filter
  (`AIPlayerGPT.cpp:1344-1476`) — the only place target-validity exists.
- **Consequence for the LLM**: the `Mana in your pool: (none)` line plus no
  affordability contract produced the corpus's dominant failure (44-90%
  cast-nothing paralysis). The LLM UX bug and the human UX bug are the same bug.

---

## 2. Design

Four pieces, each independently valuable, ordered by dependency.

### 2.1 Legal-action oracle (the foundation)
One authoritative computation:

```
LegalActions computeLegalActions(Player* p)   // at the current game point
```

returning typed entries: `PlayLand(card)`, `CastSpell(card, costVariant)`,
`ActivateAbility(source, ability)`, `DeclareAttackers(candidates)`,
`DeclareBlockers(legal block map)`, `PayExtraCost(...)`, `AnswerChoice(...)`,
`PassPriority`. Built from the existing primitives — potential mana
(generalized to ALL players, not just AI), `ManaCost::canAfford`
(`ManaCost.cpp:1067`), timing/zone/restriction checks (extracted from
`MTGPutInPlayRule` and siblings), the 601.2c target-validity filter (promoted
from `AIPlayerGPT.cpp:1459-1472`), and `canBlock`. Existing consumers migrate:
the `MTG*Rule` click handlers, both AI `FindCardToPlay` paths, and the GPT
prompt builder all become readers of the oracle instead of owning private
copies.

### 2.2 Priority engine: auto-pass with stops
Replace opt-in interrupt seeding with a real priority loop:

- After every stack push and at every phase/step boundary, each player in
  APNAP order holds priority.
- The engine AUTO-PASSES silently for a player when the oracle returns only
  `PassPriority`, or when the player's stops configuration says skip this
  point and nothing new was added to the stack since they last passed.
- Otherwise the player gets a DECISION (see 2.3): for a human, the game pauses
  (Arena-style); for an AI policy, a request is issued.
- The existing per-phase interruption options (`GameOptions.h:75-88`) become
  the human stops config surface — they finally gate something principled.
  Add "full control" (stop at every non-empty window) and sensible defaults
  (stop when you can act on the opponent's turn; skip empty own-phase
  boundaries).
- `SPLIT_SECOND` stays a no-window special case. `NextGamePhase`-as-stack-item
  and the `interruptDecision[]` seeding are replaced by the loop.
- **This one mechanism fixes**: human click-through (auto-pass), the missing
  opponent-turn windows (universal priority), the silent blockers skip (block
  decisions become engine-generated at the blockers step, not discovered by an
  AI tick against fragile branch conditions), and the LLM's wasted calls on
  empty windows (auto-pass = no model call — the skip-call-when-only-pass item
  falls out for free).

### 2.3 Decision contract: DecisionRequest → Action
The interface everything answers through:

```
DecisionRequest { kind, player, context (typed), options (from the oracle) }
Action          { the chosen option + parameters }
```

Consumers/producers:
- **Human UI**: renders the request (highlighted cards, combat arrows, menus).
- **AIPlayerGPT**: serializes request → prompt tail; answer → Action. The
  existing seams (cast menu, targets, blockers/attackers bundles, modal
  choices, mulligan) map 1:1 onto request kinds; the translog records the
  request/answer pair natively (fixes the option-count-only logging).
- **Baka rewritten as a policy**: a heuristic scorer over the same options —
  no clickstream choreography, no 4,600-line Act loop, no mirrored-Act
  duplication. Ships as the offline/instant opponent OPTION.
- **Per-decision fallback**: if the GPT policy fails (timeout, parse), the
  heuristic policy answers THAT request; the game never stalls and the
  fallback is logged per decision (fixes the silent choice:-1 class).

### 2.4 Mana & castability UX
- **Auto-tap for humans**: hand-click on a castable card pays automatically
  using the AI's existing color-selection logic (`canPayMana`) promoted to a
  player-agnostic service; manual land-tapping remains available (full
  control), and pre-tapped pool mana is consumed first.
- **Castability display**: hand cards visually marked castable/not (oracle
  output), Arena-glow equivalent in Wagic's renderer; the same marking rides
  the LLM's hand representation.
- **Representation fixes that fall out**: `Mana available: N untapped lands`
  replaces `Mana in your pool: (none)`; the prompt states the affordability
  contract once, truthfully, because the oracle enforces it.

---

## 3. Sequencing (each wave lands suite-green and independently shippable)

- **W1 — Oracle extraction**: build `computeLegalActions` from the existing
  primitives; migrate the GPT path onto it (kills the roughlyPayable/601.2c
  duplication); fix the mana representation line. No behavior change for
  humans/Baka. Low risk, immediate LLM payoff.
- **W2 — Human mana UX**: auto-tap + castable highlighting via the oracle
  (touches `MTGPutInPlayRule` consumption + `GuiHand`/renderer). No priority
  changes yet.
- **W3 — Priority engine**: the stops loop replacing window seeding;
  engine-generated combat decisions; DecisionRequest→Action contract;
  Baka-as-policy rewrite (may start as a thin adapter over existing heuristics,
  then shrink). The big wave; needs its own probe-deck validation plan
  (counterspell windows, triggers-during-blockers, split second, stops config).
- **W4 — Cleanup + validation**: delete interface-workaround text from guides
  and general prompt (the wave-3 skill's promotion mechanism defines what goes);
  run a validation corpus; compare cast-nothing rate, offered-vs-taken,
  window counts, timeout rate against matchups-20260709-220729.

### 2.3.1 W3c implementation plan (concretized 2026-07-11, post-W3b)

W3b landed the issuance half for combat: `GameObserver::pendingCombatDecision`
decides WHEN a declaration is due; the AI consults it instead of branch luck.
W3c builds the answer half — typed requests and actions — in increments, each
suite-green:

- **c1 — contract types + combat through the contract.** `DecisionRequest`
  (kind, player, typed options) and `Action` structs; an engine-side
  `DecisionManager` that issues requests from the existing pendency
  computations and APPLIES answers itself (the click choreography moves
  INSIDE the manager — consumers stop knowing about defenser-cycling).
  Baka's combat heuristics become option scorers returning an attacker set /
  block map; GPT's bundled combat asks map 1:1 onto the request.
- **c2 — per-decision fallback + native logging.** A failed policy answer
  (timeout, parse, illegal) reroutes THAT request to the heuristic policy;
  the translog records the request/answer pair natively, with a fallback
  reason field (kills the silent choice:-1 class).
- **c3 — modal choices / X announcement / menus** (`selectMenuOption` seam)
  through the contract; MenuAbility lifetime owned by the manager (the
  proper fix for the menu-zombie class the suite currently defaults around).
  Investigation findings (2026-07-11, full map in the session record):
  - **The no-answerer zombie was FALSIFIED empirically** (2026-07-11): a
    cross-seat menu (AI casts Fade Away, the pay menu arms for the
    NON-acting AI) is answered fine on the pre-change binary — because
    every menu-arming path already hands the seat over via
    `setIsInterrupting(source->controller())` (May/MenuAbility::Update),
    and `currentlyActing()` returns the interrupter. A speculative
    seat-swap fix was built, proven redundant by counterfactual, and
    REVERTED. The property is pinned by `ai/menu_cross_seat_answer.txt`
    (guards the isInterrupting handoff through the W3c migration).
    The REAL residual zombie faces: an unpayable `mExtraPayment`
    commitment pins `testDestroy` at 0 forever (the pendrell/fade_away
    wedge class — suite-defaulted today), and `menuObject` /
    `currentActionCard` dangling on zone change or game end.
  - **Greedy X auto-complete** is `MenuAbility::Update`'s ExtraManaCost
    poll (`AllAbilities.cpp:7609-7631`): fires at minimal affordability,
    reading X from the pool at that tick — affects humans too, worked
    around fixture-side today. Real fix: explicit confirm (answer-time X)
    instead of affordability polling.
  - **Lifecycle**: `removeMenu` deferred-destroy + `testDestroy` sweep is
    the codebase's own delete-in-callback guard — the manager must keep
    that discipline; `menuObject`/`currentActionCard` are raw pointers
    with no zone-change invalidation (dangle class, queued).
  - Request kinds map: may yes/no (Cancel = no), choose-one mode
    (`MenuAbility::abilities[]`, text via `getMenuText()`), X announcement
    (`AAWhatsX` per affordable X), pay[[/kicker (single-slot
    `mExtraPayment`). Option sets must be SNAPSHOTTED at request build -
    the destructor frees unchosen options and the pool mutates while the
    hold keeps the game paused.
- **c4 — casts and targets** (the big one: `FindCardToPlay`/`chooseTarget`
  seams become request kinds; the oracle's legalCasts is already the options
  enumeration).
- **c5 — Baka-as-policy shrink**: delete the mirrored Act duplication, the
  4,600-line loop reduces to scorers over contract options; ships as the
  offline/instant opponent option per the user's decision.
  Concretization (2026-07-11, from reading the commit machinery):
  - **The cast COMMIT is a repeated five-line stanza** in Baka's
    computeActions (~3486-3525, once per cost variant):
    `payTheManaCost(<variant cost>) -> clickstream.push(AIAction(this, card))
    -> gotPayments.clear()`. The card click routes through the rules'
    isReactingToClick walk; each RULE sets card->paymenttype itself, and
    when several play methods exist the engine arms the play-method MENU -
    which is ALREADY contract-routed (CHOOSE_MENU). So applyCastSpell does
    NOT need variant-dispatch click logic: payment clicks + one card click,
    and the variant choice rides the menu request that arms.
  - **payTheManaCost embeds two POLICY decisions in mechanics**: extra-cost
    targets via chooseCard (what to sacrifice/discard for a producer or
    cost), and X = dump-the-whole-pool. These must become request kinds /
    policy callbacks before the mechanics can be shared.
  - **Increments**:
    - **c5a — applyCastSpell for the plain case**: re-validate via a fresh
      buildCastSpell (the (card, viaAlternative) pair must still be
      offered); payment = ManaEngine::planPayment with the caller's policy,
      plan -> producer clicks (g->cardClick(source, producerAbility),
      mirroring payTheManaCost's AManaProducer/GenericActivatedAbility
      handling); then the card click. Casts whose selected cost carries
      extraCosts or X return false - the consumer keeps the legacy path
      (GPT: aiForcedCandidate; Baka: payTheManaCost) for those. GPT
      consumes first, Baka after.
    - **c5b — extra-cost targets as decisions**: the ExtraCost tc rides
      CHOOSE_TARGET (build from the cost's tc instead of the game's
      current chooser); kills the chooseCard policy embed and the
      remaining aiForcedCandidate dependency.
    - **c5c — ladder to scorer**: Baka's FindCardToPlay per-type rungs
      (~700 lines, 4-5 near-duplicates) become one scorer over req.casts
      with type-priority as weights; the hint/combo machinery stays as a
      pre-pass that can short-circuit the scorer.
    - **c5d — Act de-duplication endgame**: with casts, menus, targets and
      combat all contract-routed, AIPlayerGPT::Act's mirrored base body
      collapses to the async-gate + contract consumption; AIPlayerBaka
      becomes one policy implementation of the same consumer shape.

## 4. Risks / open questions

- **Latent ability-GC double-destroy (observed 2026-07-11, SIGSEGV core
  on file):** `ActionLayer::Update`'s garbage sweep can recurse -
  `ListMaintainerAbility::destroy -> ALord::removed -> removeObserver ->
  moveToGarbage -> ATransformer::destroy` - and crash when a lord and its
  transformer co-die in one sweep. Exposed by c5a's first (synchronous
  click burst) shape; nondeterministic. The AI clickstream's
  one-click-per-tick cadence keeps it dormant, which is why planCastSpell
  deliberately returns a plan instead of clicking. A future human-facing
  synchronous applier (or any batching of zone-changing clicks) needs this
  fixed first: the sweep must defer nested removeFromGame calls the way
  removeMenu defers menu destruction.

- `nextGamePhase` recursion + `PhaseRing` are old and central; the suite
  (1002+4) plus probe decks are the safety net. Expect test fixtures that
  encode current phase semantics to need review rather than blind preservation.
- `GuiCombat` owns combat UX and calls `userRequestNextGamePhase` directly —
  W3 must rewire it as a decision consumer.
- Auto-pass changes game pacing for the selfplay harness (fewer, meatier
  decisions — strictly good for inference cost, but re-measure game length).
- The `opponent()->isAI()` special case in today's stop gate exists to keep
  AI games moving; the stops model must preserve AI-vs-AI throughput (policies
  auto-answer, so windows cost one request, not wall-clock waiting).
- Priority timer (`INTERRUPT_SECONDS`) semantics under async LLM thinking —
  the extendInterruptOffer keep-alive concept carries over per decision.
- Mulligan/serum window, X announcement, kicker/alternative costs: fold into
  the oracle's costVariant enumeration (they're already enumerated piecemeal
  in the rule variants).

## 5. What this buys back

Strategy purity: wave-3 guides carry paragraphs of interface apologetics
("offered = payable", "you will rarely act on the opponent's turn", tap-out
permissions). W1/W3 make those TRUE at the platform level; W4 deletes them.
The next corpus then measures Magic play, not interface survival — and the
human client gets the modern-client feel the engine always deserved.
