# Wave-60 lane L — B1, B2, B11 (lethality the render withholds or lies about)

Worktree `worktrees/lanes/w60-L`, branch `w60-lane-L`, base `8b922abf0`.
Evidence read before designing: `wave59/engine-seat.md` (HIGH-1, MED-3), `wave59/deck130/review.md`
(HIGH-1), `wave59/deck146/review.md` (HIGH-1), `wave59/deck123/review.md` (I1), and the seq records
themselves in `matchups-20260904-130044/` (130v146 s43/s44 read end-to-end).

## B1 — a win fold that subtracts what kills the pilot first

**What the corpus proves.** `130v146` s43 (turn 26, life 1 vs 2) rendered, on ONE row, both
`[NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life ... you would be at -2; this
KILLS you]` and `- and 3 to the opponent at life 2 WINS THE GAME`. s44's target menu then printed
`1. The opponent (player, life 2) {right now: takes 3 damage - they would be at -1; THIS WINS THE
GAME}` with no death clause at all. I re-read s44's stored prompt: it carries **no `ON THE STACK`
section** — the spell is not on the stack yet at the target ask, so the target menu's lie is not a
missing stack render, it is an unsubtracted cast price. `gameend` two records later: `life -2, lost`.

**Mechanism.** A win fold is a claim about the END of the game, so it is false whenever the pilot
stops being alive first. New pure `winFoldBlockedTail(myLife, lifeLossFirst)` returns
`WOULD win, but you lose N life first - you would be at K: YOU LOSE BEFORE THIS RESOLVES` when the
committed loss reaches 0, and `""` otherwise. Both win emitters take two new defaulted parameters
and consult it:

* `castPlayerDamageTail` (the cast row). The row now accumulates `rowSelfLifeCost` in the same pass
  that prints the prices — the naming permanent's surcharge (`namedCastPriceTag`, `nLife`) plus the
  auto-tap plan's pain (`paymentLifeCostClause`, `painDamage`) — and hands it to the fold.
* `damagePlayerVerdict` (the target menu's player rows). `AIPlayerGPT::chooseTarget` computes
  `perilBeforeResolve` once per ask from the object's POSITION: `stackLifeLossBefore()` sums the
  life loss from stack objects **above** the object being targeted (they resolve first) and returns
  `-1` when the source is not on the stack at all, in which case the unpaid cast surcharge
  (`namedCastLifeSurcharge`, the same `namedCastPenaltyScan` the cast row prints) is used instead.

**Rules basis, stated because it decides the scope.** Objects already on the stack do NOT precede a
spell being cast now — the new spell goes on top and the opponent dies to SBA before they resolve —
so pending stack damage is deliberately NOT subtracted from a cast row's win fold. What does precede
it: costs paid on announcement, and triggers the cast itself puts on the stack above the spell
(the Silencer shape). That is exactly what the two inputs above are.

## B2 — pending damage on the stack, totalled, and rows that name their effect

**What the corpus proves.** `146v162` s26 (T15 Draw, `Your life: 5`) listed four unresolved
triggers for four damage with no total; rows 1/3 read a bare
`Ob Nixilis, the Hate-Twisted's triggered ability` while rows 2/4 read
`Underworld Dreams's deal 1 damage`, so even reading row-by-row does not give the number. 13 of 77
stack renders in that deck's corpus carry damage at the seat, 3 at life 1.

**Mechanism.** `scanStackAbilityLife` walks a stack object's live ability graph (AADamager, AALifer,
through NestedAbility/MultiAbility, `rand` magnitudes never evaluated, source-less abilities skipped
because `ActivatedAbilityTP::getTarget()` resolves CONTROLLER/OPPONENT through `source->controller()`)
and answers two things at once: the life it takes off this seat, and an effect phrase
(`stackDamagePhrase` / `stackLifePhrase`). The stack block runs it per row: the phrase is passed
into `stackAbilityBody` → `stackAbilityLine`, which appends it **only** where the label has already
degenerated to the bare kind (`triggered ability (deals 1 damage to you)`), so a row that already
names its effect is byte-identical; the total is printed under the rows as
`ON THE STACK: N damage to you - you would be at K` (+ `; that would KILL you`). The total counts
only the rows the block actually renders, so the sum and its addends cannot disagree.

**Scope, honestly.** SPELLS on the stack are not priced — only StackAbility objects are. Every
repro in the evidence is a triggered ability; a spell's damage rider is not instantiated until it
resolves and I did not build a second derivation for it.

**On "TARGET CHOICE windows render the stack (now 11/89)":** the stack block lives in
`serializeGameStateImpl`, which every window renders, so target-choice windows already render the
stack **whenever there is one**. s44 is the proof that 11/89 is largely the truth and not a
suppression: at cast-time targeting the spell is not yet on the stack. Nothing was changed for this;
what the s44 window actually needed was B1's fold, which it now gets.

## B11 — the ATTACKERS ask gets the aggregate the blockers ask has

**What the corpus proves.** `123v126` s71: 31 attackers into a 22-life opponent with
`They have 5 untapped creatures able to block; declaring more than 5 attackers leaves at least
(your attackers - 5) of them unblocked.` as the entire arithmetic; the neighbouring cast window s69
spent 431,938 ms and 4,049 characters deriving it by hand. The defender's ask has
`INCOMING THIS COMBAT: N attackers, X unblocked damage - you would be at Y; this KILLS you`.

**Mechanism.** `attackTotalLine(attackers, totalPower, oppLife, blockers, guaranteed)`, emitted
directly under `attackerBlockerCountLine`, computed from the same pass that builds the A-rows
(`rowPower`, `rowNoLegalBlock`) so the totals cannot disagree with the rows. Two numbers of
deliberately different kind:

* CEILING — `declaring all of them with none blocked puts them at K`. Conditional, no kill claim:
  they choose whether to block.
* FLOOR — every attacker no untapped creature of theirs may legally block (the same predicate
  `noPotentialBlockersTag` / the menace clearing print), plus the SMALLEST powers among the
  blockable ones that their blocker count cannot cover. At most `blockers` attackers can be blocked
  at all, so this is a proven minimum; menace only raises the requirement, trample and first strike
  do not lower it. `; that KILLS them whatever they block` when it reaches their life. When
  `guaranteed == 0` the line says the blockers can cover every attacker and promises nothing.

No win token is claimed here: attacker and blocker damage is simultaneous and a converter can kill
the pilot in the same step, so the line mirrors the defender's `KILLS you` register rather than
asserting a game end.

**The crack-back is not re-derived.** `CRACK-BACK NEXT TURN` already renders on Attackers windows
(wave-59 K8 PASS: 30 renders in the Attackers phase). Nothing was added for it.

**A live probe corrected the wording before commit.** The first build printed, over ONE attacker no
untapped creature could legally block, `At most 3 of them can be blocked at all, so at least 3
damage lands ...` — a true count offered as the cause of a floor it did not cause. The floor now
states the number and no cause; the blocker-count line directly above already states the cap. The
PARSETEST negative pins it.

## Files

* `projects/mtg/src/AIPlayerGPT.cpp` — all three items, tagged `#W60-L (B1|B2|B11)`.
* `projects/mtg/bin/Res/test/w60_stack_pending_damage_total.txt` — new suite fixture (B2).
* `projects/mtg/bin/Res/test/_tests.txt` — registry line for it.

## Gate

Hermetic build (`rm -f bin/wagic && make -f Makefile.sdl -j4`), every run under
`MemoryMax=4G MemorySwapMax=0`, no kill under the cap.

| | base `5039427da` (re-measured here) | after |
|---|---|---|
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1247 tests, **0 failed**, **0 timed out** | 1248 tests, **0 failed**, **0 timed out** |
| AI tests | 62, 0 failed | 62, 0 failed |
| PARSETEST | 3529 passed, 0 failed | **3554 passed, 0 failed** (+25) |

Both `==Test Failed !==` and `==Test timed out` were counted; both are 0. The brief's baseline said
61 AI tests — the base binary re-measured here reports **62**, so that figure was stale by one, not
moved by this lane. Suite delta +1 is exactly the new fixture.

**Fixture RED on base, GREEN after — how verified.** The fixture was run scoped
(`WAGIC_TESTSUITE_FILE=test/_probe.txt`, probe file deleted afterwards) against the archived base
binary `archives/wagic-5039427da-w59step1` with the identical Res tree: it fails with
`narration assertion failed for player 1: expected "on the stack: 1 damage to you - you would be at
19", log was "... | on the stack: ability: underworld dreams's deal 1 damage [from their underworld
dreams] | "` → `==Test Failed !==`. The same fixture on the new binary is `==Test Successful !==`.
Exactly one assertion moved (the total); the effect-label assertion passes on BOTH binaries, which
is the intended negative — Underworld Dreams already names its effect and must not be decorated.
The fixture drives `TestSuiteAI::Act`'s `stackAbilityRegister`, which was extended to run the same
two production calls the prompt's stack block runs, so the red is a red render, not a red copy of one.

**PARSETEST cases shipped** (25, `[W60-L]`): per item a positive, a must-NOT-match negative, a
byte-identical regression for the defaulted/undecorated form, and an echo-shape check
(B1's fold stays inside the row's existing single `{...}` pair and adds no bracket; B2's total and
B11's aggregate are prompt LINES and introduce no braced or bracketed annotation at all).

## Predictions for the wave-60 corpus (falsifiable)

1. **B1.** Every `WINS THE GAME` / `THIS WINS THE GAME` render in the corpus sits on a window where
   the seat's life minus the row's own printed life price is > 0. Equivalently: **0** prompts contain
   both a `this KILLS you` cast price and a win token on the SAME row. Where the price is lethal the
   row reads `WOULD win, but you lose N life first ... YOU LOSE BEFORE THIS RESOLVES` instead, and
   no such row is followed by a `gameend` in which the seat won.
2. **B2.** Every rendered `ON THE STACK, waiting to resolve` block whose rows include an ability that
   damages or drains the seat carries exactly one `ON THE STACK: N damage to you - you would be at K`
   line, and N equals the sum over those rows. **0** stack rows read `'s triggered ability]` or
   `'s activated ability]` with an empty parenthetical where the underlying ability is an
   AADamager/AALifer aimed at a player.
3. **B11.** **100%** of `attackers` records render an `ATTACK TOTAL:` line; **0** of them contain the
   substring `can be blocked at all`; and on every record whose floor clause says
   `that KILLS them whatever they block`, the opponent's life in the following `gameend` (or in the
   next record) is <= 0 unless the seat declared fewer attackers than the line assumed.

## What I did NOT verify

* **B1's fold never fired live.** Two GPT probe games (deck123 vs deck126, deck146 vs deck162, Spark
  35B at :8084, ~41 prompt-bearing decisions each) produced **0** `WOULD win, but` renders — no
  window in either game held a lethal cast price beside a win. The strings are PARSETEST-proven and
  the two input paths are code-verified, but the composed row has not been observed.
* **B2's total never fired live either.** 0 `ON THE STACK:` renders in those two probes (the wave-59
  base rate is 13/77 for one deck, so two short games missing it is unsurprising). It IS exercised by
  the new suite fixture through the production calls — that is the whole reason the register was
  extended — but not yet in a real prompt.
* **B2 on SPELLS.** Only StackAbility objects are priced; a spell on the stack that will damage the
  seat contributes 0 to the total and prints no effect phrase. The total is therefore a FLOOR on
  pending damage in the general case, and the line does not say so.
* **`stackLifeLossBefore`'s above-the-object branch** has no observation at all: every target ask in
  both probes was a cast-time ask, so the branch that prices an ability already on the stack against
  the objects above it was never taken outside the code path's own construction.
* **`AALifer` handling is untested end-to-end.** The `life:-N` shape is reachable in the scan and
  covered in PARSETEST at the phrase level, but no fixture and no probe put an AALifer on the stack.
* **No wave-58 or wave-59 corpus re-count.** Every "N of M" figure above is quoted from the cited
  review files; I re-read only 130v146 s43/s44 records myself.
* **The probe games' play quality was not reviewed** and their translogs are not preserved with the
  wave — they were arrival traces for the new emitters, nothing more. The deck123 probe's ATTACK
  TOTAL render was the only new line observed live (4 more in the deck146 probe).
* **No card fact beyond Underworld Dreams' primitive** (`auto=@drawfoeof(player):damage:1 opponent`,
  read off `mtg.txt:125823`) was re-verified by me; Silverquill Silencer and Acererak are taken from
  the wave-59 reviews, which state they were Scryfall-checked.
* **No GUI, no console, no merge.** I did not run the wave-58 corpus, did not touch any guide or
  prompt asset, did not modify `endpoints.txt` (the probes used `WAGIC_GPT_URL/MODEL/KEY` env
  overrides), and did not merge this branch anywhere.
