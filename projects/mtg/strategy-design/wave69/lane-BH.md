# Wave-69 lane BH — K4, K6 (the numbers on the two surfaces the seats trust)

Base: master `169ee41f3`. Branch `w69-lane-BH`, worktree `worktrees/lanes/w69-BH`.
Files changed: `projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/src/TestSuiteAI.cpp`,
`projects/mtg/bin/Res/test/_tests.txt`, new `projects/mtg/bin/Res/test/w69bh_upkeep_draw_target_is_a_card.txt`.

Every wagic run memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`); no kill under the cap on any run.
The gate ran detached (`w69-BH-gate`) with a foreground until-loop wait; no Monitor, no background task.
Every edit was a python `rb`/`wb` byte splice; `git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.
The binary was freshness-checked (`ls -la bin/wagic`) after every make, and no build was ever chained
behind a counting grep. Every comment tagged `#W69-BH (<item>)`.

Nothing volatile entered `mPromptTail` or an ask key. The one new key-affecting term is the extra
re-ask K4c can open; it is latched per board / per ask key exactly like the shipped no-op re-ask, so
it can only ADD one window, never remove one.

---

## THE K4 COUNT (done before any code was touched)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/`, 42 seat logs, 2,282 rendered prompts.

* **33 ANNOUNCE_X windows** in the whole corpus; **31** carry a library-reserve clause; **2** print the
  ceiling sentence.
* **15** of the 33 stand on a board with at least one `@each my upkeep ... draw` permanent
  (Staff of Nin) on the SEAT'S OWN battlefield.
* **2** of those 15 under-count, and both are the same window shape — the seat's OWN upkeep with an
  own Staff trigger still unresolved on the stack:

| window | phase | library | printed reserve | true reserve | printed ceiling | true ceiling | taken |
|---|---|---|---|---|---|---|---|
| `125v126` s91 (T37) | own Upkeep | 33 | 1 | **2** | (none printed) | — | X = 11 |
| `125v126` s110 (T41) | own Upkeep | 8 | 1 | **2** | **X = 7** | **X = 6** | X = 7 → decked at 65 life |

  0 windows over-count. The 13 remaining Staff windows are CORRECT: at every phase other than the
  seat's own upkeep the battlefield walk runs and folds one per Staff (`125v130` s119/s128 read 3 with
  two Staffs and a 2-card draw step, checked by hand against each prompt's own DRAW FORECAST line).
* **`plus the N draws already on the stack` — the reserve's stack term — rendered 0 times in all
  2,282 prompts.** So did `; M more pending`. The stack term was dead, not rare.

### THE MISSING TERM

Not `@each` permanents, not draw-step extras, not double-counted Staffs: **the stack scan resolved the
draw's RECIPIENT differently from the way the engine resolves it.**

`scanStackAbilityDraws` counted a pending `AADrawer` only when
`dynamic_cast<Player *>(target) == seat`. `AADrawer::resolve` instead calls
`getPlayerFromTarget(getTarget())`, which maps a **card** target to that card's **controller**.

* `Staff of Nin` is `auto=@each my upkeep:draw:1` — no target keyword, so `who == UNSET` and the
  ability's `target` is the SOURCE CARD. `dynamic_cast<Player *>` of a card is NULL → **0 draws for
  anybody**.
* `Howling Mine` is `auto=@each my draw sourcenottap:draw:1 controller` — `who == CONTROLLER`, so
  `ActivatedAbilityTP::getTarget()` hands back a real `Player *`. That is why the wave-63 E2 evidence
  (Howling Mine / Dictate of Kruphix triggers) looked like the walk worked, and why wave-68 lane BB's
  fold — which was correct in every other respect — still read 1.

Compounding it (and only there): `xReserveUpkeepAhead` (#W64-AK R9) deliberately skips the battlefield
`@each my upkeep` walk at the seat's own upkeep, on the ground that the trigger "has already gone on
the stack". It has — and the stack term could not see it. Both terms were blind in the same window.
That gate is left **unchanged** and is correct: at the seat's own upkeep the pending triggers are the
stack's to count, and everywhere else an own upkeep trigger cannot be pending, so the two terms are
mutually exclusive by phase and the fix cannot double-count.

---

## What shipped

### K4a — the pending-draw scan resolves the recipient the way the resolution does
`stackDrawRecipient(Targetable *)` maps Player → itself, MTGCardInstance → its controller,
Interruptible → its source's controller — `MTGAbility::getPlayerFromTarget`'s own mapping — and
`scanStackAbilityDraws` uses it. Nothing else changed: the reserve arithmetic, the ceiling and the
row clause are wave-68's, now fed the term they were always meant to have. At `125v126` s110 the
reserve becomes 2 and the ceiling X=6, and `xLibraryReserveWhy` prints
`plus the 1 draw already on the stack, which you cannot decline either` for the first time in the
project's history. No X is removed and no row is capped — the rows above the ceiling still print,
now carrying the deck warning that decides them.

### K4b — the row that averts the deck-out prices the library it rebuilds
`Life with Elixir of Immortality [cost: {2}, Tap]` rendered 412 times with no `{right now:}` verdict
while every neighbouring activation row carried one; at `125v126` s111 it stood beside a ping with
`Your library: 1 cards` and a 19-card graveyard and the seat pinged. It now carries
`{right now: shuffles your 19-card graveyard and this card back into your library - your library goes
from 1 to 21 cards}`. Gated on TWO independent facts so the count is exact and never a guess: the
ability chain the ROW is built from contains an `AAShuffle`, AND the source's script carries both move
literals (`moveto(mylibrary)` with `all(*|mygraveyard)`). Anything else gets no clause.

### K4c — the no-op re-ask fires off the ROW'S OWN zero
`noopRowEarnsReask(row)` = `rowSaysNoOp(row) && !rowIsDeclineRow(row)`, called at both seams in place
of the `rowSaysNoOp AND planArguesAgainstRow` conjunction. Corpus: 14 K=0 sweeper takes over 12
distinct (game, turn) — `146` s11/s20, `152` s39, `123` s10/s40/s41/s66/s104, `130` s28/s33/s56/s58,
`162` s8/s61 — none of which the conjunction could fire on, against ONE fire of the plan form
(`130` s20, where the re-ask worked and s21 declined). The reply's agreement is now only what the
corrected question quotes back (`", and your reply says so too"` appears only when it is true). The
decline rows stay exempt. Budget, latches and the exempt one-shot are wave-68's; a take with no plan
agreement stamps `noop_row_zero_reask` so the two populations stay separable in the next corpus.

### K6a — the crack-back cover counts the bodies already standing
`162v152` s15 printed `Counting only the checked bodies you cover 5 of 13 ... you would be at 4` on a
board holding an untapped Master of the Feast (5/5) and an untapped Fate Unraveler (3/4); s19 read
`at 0` the same way. `crackBackCoverFacts` now also walks the SEAT's battlefield for creatures that
are untapped NOW (a body tapped now stays tapped through the whole of their turn — the seat's
permanents untap in the seat's own untap step) and can block, asking `canBlockPairwise` — the same
engine call the row's own body is asked with — per attacker. `crackBackBlockerRowTag` spends that pool
FIRST (they are certain; no cast is required to have them), each body assigned to at most one
attacker, then the row's own bodies cover what is left. With no standing body every byte is wave 68's.

### K6b — the reserve clause ends in a verdict word
`{reserve: ...}` ended in its two operands (`leaves 1 source, and ... needs 5`). Its own guard is
`need <= left -> ""`, so wherever it prints the subtraction is already decided; it now ends
`VERDICT: taking this row STRANDS <card> this turn - N source(s) cannot pay M, and it has no window
after your main phase}`.

### K6c — the positive twin of the stack verdict
`{answers the stack: NO}` rendered 4 times in the corpus and the seat took the declining row **4 of 4
times**. `stackAnswerYesRowClause(gain, stackLoss, myLife)` prints
`{answers the stack: YES - ...}` on an ACTION row whose life gain outlasts the pending lethal stack
(the seat has priority, so anything taken now resolves first). The gain is read from the ability the
row is built from — `abilitySelfLifeGainNow`, a plain positive integer `life:N` whose recipient
resolves to this seat through the same `stackDrawRecipient` mapping. A removal of the SOURCE is
deliberately NOT claimed: destroying the source does nothing to an ability already on the stack.

### K6d — the header names the material line beside the life line
`152v146` s15/s19 both printed `chosen for the LIFE ONLY - it does not preserve your material` and
the seat answered exactly the named assignment 2 of 2. The disclaimer is true and it is not an
alternative. The blockers header now runs the OTHER search — the same matching over a `can` matrix
with every pairing whose material rank is below the survival rank struck out — and prints
`blockKeepAlternativeClause`: ` ALTERNATIVE, chosen for MATERIAL instead of life: <pairings> - every
blocker in it survives, and it lets in K combat damage instead of N (K-N more damage, and it spends no
creature). Both lines are legal: the one above is the lowest life, this one is the lowest material
cost.` Printed only when the life line actually spends a body (`blockersDying > 0`) and the material
line costs more life; otherwise the life line already IS the material line and nothing is added.

---

## Gate numbers

| | base `169ee41f3` | this lane |
|---|---|---|
| PARSETEST | 5272 passed, 0 failed | **5307 passed, 0 failed** (+35) |
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1277 / 0 failed | **1278 tests, 0 failed** (+1 fixture) |
| AI tests | 67 / 0 | **67, 0 failed** |
| `==Test timed out` | 0 | **0** |

Build: `rm -f bin/wagic && make -f Makefile.sdl -j4`, binary re-stamped and mtime-checked each time.
Gate logs: `~/.gatelogs/w69-BH-parsetest.log`, `~/.gatelogs/w69-BH-suite.log`.

### RED on base — 11 PARSETEST cases + 1 suite fixture

RED produced the way lanes AL/AQ/AW/BB produce it: the seven mechanisms stubbed back to their wave-68
behaviour with the new cases already in place, rebuilt (`5296 passed, 11 failed`; the fixture
`==Test Failed !==`), then restored and rebuilt green.

| item | RED |
|---|---|
| K4a (recipient mapping) | the suite fixture `w69bh_upkeep_draw_target_is_a_card.txt` (the pure cases pin the arithmetic, which did not change — see "what I did NOT verify") |
| K4b (library refill verdict) | 1 |
| K4c (row-only re-ask predicate) | 1 |
| K6a (standing bodies in the cover) | 4 |
| K6b (reserve verdict word) | 2 |
| K6c (positive stack answer) | 2 |
| K6d (material alternative) | 1 |

The fixture is the K4a proof: `assertpendingdraws <n> card <name>` (new third form) stages a REAL
`AADrawer` over an in-play card, targeting the card, with `who` left UNSET — exactly what
`AbilityFactory` builds for a bare `draw:N` — and asks the shipped scan. 0 on the wave-68 tree, 1 now.

### Strings the ~20 KB guides key on

* `{reserve:` opener: **unchanged**. `deck162_strategy.txt` lines 82-87 quote the clause with an
  ellipsis and key on the presence/absence of the `{reserve:` opener; the phrase
  `SORCERY SPEED, so your main phase this turn is its last window` is still present verbatim — only
  the closing `}` moved to the end of the new VERDICT sentence. The wave-66 PARSETEST case that pinned
  `... its last window}` was updated to pin `... its last window.` plus the verdict; that is the one
  deliberate pin change in this lane.
* `{right now:` opener, `{crack-back cover:` opener, `{answers the stack: ` opener, `Counting only the
  checked bodies` (only replaced when standing bodies exist), and the whole
  `one legal assignment that reaches it, chosen for the LIFE ONLY ...` sentence: **all unchanged**;
  every K6 change appends.
* No guide in `bin/Res/ai/baka/` contains `answers the stack`, `crack-back cover`, `already owed`,
  `largest listed X` or `one legal assignment` (checked with `/usr/bin/grep -l`).

---

## Falsifiable predictions for the wave-69 corpus

1. **K4a.** Every ANNOUNCE_X window at a seat's OWN upkeep with an unresolved own upkeep-draw trigger
   prints `plus the N draw(s) already on the stack` and a reserve of at least `1 + N`. FALSIFIER: one
   such window whose printed reserve is 1.
2. **K4a.** No seat empties its library from an X answer it took off a printed ceiling. FALSIFIER: a
   `gameend` recording a deck-out within 3 turns of an ANNOUNCE_X answer at or below that window's
   printed ceiling.
3. **K4b.** Every `Life with Elixir of Immortality` row carries a `{right now: shuffles your N-card
   graveyard ...}` clause; where `Your library:` reads <= 5 and an Elixir row is offered, the Elixir
   row is taken at least half the time. FALSIFIER: an Elixir row with no clause, or 0 takes across
   >= 6 such windows.
4. **K4c.** `noop_row_zero_reask` appears in the corpus (the shape it fires on occurred 14 times in
   wave 68), and the count of TAKEN rows whose own `{right now: ...}` reads zero falls below 14.
   FALSIFIER: 14 or more such takes with no `noop_row_zero_reask` record among them.
5. **K6a.** No `{crack-back cover:}` clause projects a life total on a board where the seat controls
   an untapped creature without a `You ALSO already control N untapped creature(s)` term.
   FALSIFIER: one such clause.
6. **K6b.** Every `{reserve:}` clause ends in `has no window after your main phase}`. FALSIFIER: one
   that does not.
7. **K6c.** No seat takes a `{answers the stack: NO}` row in a window where another row on the same
   menu carries `{answers the stack: YES}`. FALSIFIER: one such pair (wave 68: 4 of 4 NO rows taken,
   with no YES ever offered).
8. **K6d.** Where the header prints both lines, the seat's declaration matches the ALTERNATIVE at
   least once. FALSIFIER: >= 4 two-line headers with 0 material-line answers (wave 68: 2 of 2 copies
   of the life-only line).

---

## What I did NOT verify

* **No live model probe was run.** Every claim above is from the wave-68 corpus, PARSETEST and the
  suite. Whether the model USES any of these clauses is the wave-69 corpus's question, not mine.
* **K4a's pure PARSETEST cases are not RED-able.** `xLibraryReserveCount`, `xLibraryCeilingX`,
  `xLibraryRowClause` and `xLibraryReserveWhy` are wave-68's code and were not changed; the cases I
  added pin s110's numbers through them so the corpus arithmetic is checkable, but they pass on base
  too. The RED evidence for K4a is the suite fixture, and only the suite fixture.
* **The fixture stages the shape, it does not schedule a real upkeep.** The scripted pump takes one
  command per ~40 ticks and a Staff trigger resolves inside a priority window a scripted fixture
  cannot reliably land a command in, so the fixture constructs the AADrawer the way `AbilityFactory`
  does and pushes it. I did NOT observe the fixed scan against a trigger the ENGINE put there; the
  corpus is where that gets observed.
* **I did not re-derive the 13 correct Staff windows' draw-step extras from the engine** — I read each
  one's own printed DRAW FORECAST line and its `why` string. If a prompt's forecast were itself wrong,
  my "0 over-counts" would inherit that error.
* **K6c covers ABILITY rows only.** A CAST row's self life gain has no instantiated `MTGAbility` to
  walk before the cast, and reading it off the card script would risk the false claim N-158h forbids.
  A life-gain INSTANT that answers a lethal stack therefore still gets no YES verdict; `125v162`
  s68/s69 would not have earned one either (the seat was dead at every X).
* **K6a assigns greedily by attacker power, not optimally.** The pairing is the same greedy shape the
  shipped clause already used, restricted so no body is spent twice; it can under-claim cover on an
  awkward legality graph (which reads as "you would be at less than you would be"), never over-claim
  a body that cannot block.
* **K6d's ALTERNATIVE line's damage figure excludes blocking triggers and lifelink.** The header's own
  life figure folds those for the NAMED assignment; the alternative states the combat damage it lets
  in and the material it keeps, and deliberately projects no life total, so the two numbers are not
  presented as comparable life totals. That is a real asymmetry a later wave may want to close.
* **The cost of K4c's widened re-ask is unmeasured.** It can only add asks — an upper bound of one per
  board / per ask key, and wave 68's shape count says roughly 14 more asks per corpus — but I did not
  measure latency.
