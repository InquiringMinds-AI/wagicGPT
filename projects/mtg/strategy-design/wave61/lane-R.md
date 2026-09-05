# Wave-61 lane R — C1, C2, C3, C4 (combat and stack aggregates that lie)

Branch `w61-lane-R`, base `718e99d2a` (wave-60 close). Base binary for every counterfactual:
`archives/wagic-614f8452e-w60step1` (718e99d2a adds only review documents on top of it).
Evidence read before designing: `wave60/lane-L.md` (the B2 scan and the B11 ATTACK TOTAL I
extend), `wave60/lane-Q.md` (R4's infect/doublestrike/CANTLOSE split, which I leave intact),
`wave60/engine-seat.md` HIGH-1/HIGH-2, `wave60/deck152/review.md` H1, `wave60/deck126/review.md`
HIGH-2 / MED-1 / MED-2, `wave60/deck125/review.md` HIGH-1.

## C1 — `ATTACK TOTAL` stops naming a resulting life the same prompt's own tags falsify

**What the corpus proves.** Two independent classes, both visible on the page the line sits on.

*(a) Blocking-trigger life gain.* `152v126` s48/s58/s66: floors of 43/57/71 against post-combat
actuals of 66/80/94 — **+23 on 3 of 3 swings**, and it decided deck152's only loss (the opponent's
life went 20 → 95 on the seat's own attacks). The prompt already prints the per-attacker tag
(`blocking trigger: they gain 3 and may gain 4 more`), but Perimeter Captain fires **per blocking
defender**, so each tag is a ONE-blocker figure and even their sum is not the price.

*(b) Attack-declaration punishers.* `126`'s `125` s121: Lightmine Field on the rendered opponent
battlefield, `ATTACK TOTAL` promising 2 damage, and the declaration trigger killed both 1/1
attackers for 0 damage dealt.

**Mechanism.** `attackTotalLine` takes two new defaulted parameters.

* `blockGain` — the life their blocking triggers can hand them, summed by the caller over the SAME
  `canBlock()` set the blocker-count line one row above states, via the existing
  `blockTriggeredLifeFor` (both the certain and the `may` half: a `may` is theirs to take and this
  is a number the seat must survive). It is a proven ceiling, because with at least one attacker
  every blocker they own may block (a gang block puts them all in front of one). With it > 0 the
  floor no longer names a bare resulting life: it names the damage floor, the life that damage
  ALONE would leave, and the gain that can put them back up — and the kill claim survives only when
  the gain cannot save them (`that KILLS them whatever they block, gain included`). The CEILING
  clause is deliberately untouched: it is conditional on nothing blocking, and a trigger that rides
  a BLOCK cannot fire in that branch.
* `attackPunishers` — names, from `attackDeclarationPunishers()`. Lightmine Field's engine line is
  a PHASE trigger (`@each blockers:foreach(creature[attacking]|Battlefield) damage:1
  all(creature[attacking]|Battlefield)`, mtg.txt:67059), not an `@combat(attacking)` one, so the
  predicate is the SHAPE of the effect — a triggered line that damages a SET of attacking
  creatures — and not the trigger word. Nothing is computed: how many attackers die depends on how
  many are DECLARED, which is the choice being made. The permanent is named, both figures are
  re-scoped (`Both figures are BEFORE their attack triggers: …`), the reason they are not folded is
  stated (a silent omission is the shape the model confabulates into), and **every kill claim is
  withheld** while a punisher is unpriced.

Fails closed: an unreadable script line is not evidence a punisher exists.

## C2 — the blockers ask gets the blocker-count-respecting verdict B11 gave the attackers ask

**What the corpus proves.** `162v123` s18: `INCOMING THIS COMBAT: 47 attackers, 140 unblocked
damage … (your creatures may legally block every attacker in that total)` over **one** blocker row
— and that same record is the one that DROPPED the `best case with every blocker assigned` clause
that 31 of the other 32 renders carry. `126`'s `146` s9/s17/s22 are the same lie at 1-vs-2,
1-vs-3, 1-vs-6, one of them in the same sentence as `no block saves you`.

**Mechanism.** Two changes to the same solver the clause below it already runs.

* `assignableMatchedAttackers(attackerCount, can)` — the CARDINALITY of a maximum legal matching
  over the engine's own pairwise map, from the same greedy augmenting search
  (`assignableAugment`) the remainder uses, with every attacker weighted equally. The aggregate
  promise now prints only where `matched >= assignable`, i.e. where a full assignment provably
  exists. Where it does not, the line states the count-respecting fact instead: `your creatures can
  legally block at most M of those N attackers at once, so at least N-M of them go unblocked`. The
  unblockable split still wins the parenthetical when it applies — it is the sharper fact and the
  two never both print. `matched < 0` (not computed) keeps the wave-60 wording byte for byte:
  nothing is claimed from a number that was never derived.
* The `assignableRemainderDamage` guard was `nb > 32 || na > 32`, which is exactly why s18 printed
  no best-case figure — **the cap deleted the clause on the one window where the arithmetic decided
  the game.** Raised to 256/256. The matching is O(V·E) over a `char` map; 256×256 is 64 KB and a
  few hundred thousand steps worst case, beside a model call. The bound stays only so a
  pathological board cannot run unbounded.

## C3 — `CRACK-BACK NEXT TURN` stops calling a floor a ceiling, and nets evasion

**What the corpus proves.** 6 of 26 turns took MORE than the printed `up to N` at deck125 — a seat
with zero creatures for which this line is the only tap-out instrument — and the two worst are the
deciding turns of two losses. Plus `123v126` s20: 20 attackers priced at 41, **19 of them fliers**
against an all-ground defence, no netting at all (51/51 renders at that seat).

**Mechanism.** `crackBackFloorSources(opp)` scans the opponent's battlefield scripts for the three
classes the reviews proved, and names each rather than folding a number the seat cannot re-derive:

* **animators** — an ACTIVATED `becomes(Creature …^P/T^…)` on a permanent that is not currently a
  creature. Hive of the Eye Tyrant (borderline.txt:53394) is the repro; the printed power is read
  off the `^3/3^` segment when it parses, and omitted when it does not.
* **activated direct damage** — a non-triggered line with a numeric `damage:N` whose target can be
  a player (`anytarget` / `target(player…` / ` opponent`). Siege-Gang Commander
  (`{1}{R}{S(goblin|myBattlefield)}:Damage:2 target(anytarget)`, mtg.txt:105073) turned `up to 5`
  into 9. The per-activation figure is stated; the NUMBER of activations is not guessed (it turns
  on their mana and their sacrifice fodder).
* **pump triggers** — a triggered line timed at their combat (`my combatbegins` / `my attackers` /
  `my blockers` / `@combat(attacking)`) that adds a positive `counter(…)`. Luminarch Aspirant and
  Ranger Class are the repro (a 4/4 attacked as a 6/6). Named only: which creature they target is
  their choice and is made after this window.

When any of the three is present the head clause becomes `for up to N from combat as their board
stands - you would be at K **or lower**`, and the sources ride `- and that number is a FLOOR, not a
ceiling: …`. The kill clause SURVIVES that change: a floor at or past the seat's life is still a
proven death. With none of the three present the line is byte-identical to the wave-59 wording.

Evasion is a separate sub-total in the register the blockers ask uses: `- of that, X from F
attackers nothing you control can legally block`, computed with the engine's own
`canBlockPairwise` (flying/reach, shadow, protection, landwalk, `cantBeBlockedBy`, …) and
deliberately NOT `canBlock(attacker)`, which additionally tests the tapped state and the attacker
flag — neither of which holds for a combat that has not happened. Menace folds through
`minBlockersRequired()`: too few legal bodies is unblockable. Printed only where the seat HAS
bodies, for the reason the INCOMING split uses.

## C4 — `ON THE STACK` prices a `target(anytarget)` ability aimed at the pilot

**What the corpus proves.** 4 renders (`deck130` s118/s139, `deck125` s66, `deck152` s77), Staff of
Nin every time, each showing the row and NO total, each followed by the log line recording the
damage landing on the seat. 38 of 42 damaging stack blocks priced, 4 silently unpriced — and Staff
of Nin is a repeat-every-turn clock, so the miss is systematic.

**Mechanism — and it is not the reason lane L gave.** The B2 scan read `getTarget()` on the payload
ability, and for a CHOSEN target that field is empty until the ability RESOLVES:
`TargetAbility::resolve` (MTGAbility.cpp:8469) is where `ability->target = t` happens, and until
then the pick lives in the WRAPPER's own `TargetChooser`. That is the same place
`ActivatedAbility::activateAbility` reads its activation-event targets from (#W43-10), which also
records why `ability->target` is not a usable fallback: the MTGAbility ctor seeds it with the
SOURCE card. So `scanStackAbilityLife` now threads the innermost chooser's `getTargetsFrom()` down
through the NestedAbility walk and consults it **only** where the payload's own `who` is
`TargetChooser::UNSET` — exactly the branch whose `getTarget()` returns `target`. An ability whose
`who` names CONTROLLER/OPPONENT/OWNER resolves its player itself, and a sibling `target(creature)`
pick must not be read as that player. Every target in the list is priced, because that is what
`TargetAbility::resolve`'s oneShot loop does with them. `ActionElement` gains one public
const accessor (`chosenTargets()`) because `tc` is protected.

## Files

* `projects/mtg/src/AIPlayerGPT.cpp` — all four items + 41 PARSETEST cases, tagged
  `#W61-R (C1|C2|C3|C4)`.
* `projects/mtg/include/ActionElement.h` — `chosenTargets()` accessor (C4).
* `projects/mtg/bin/Res/test/w61R_stack_anytarget_total.txt` — new suite fixture (C4).
* `projects/mtg/bin/Res/test/_tests.txt` — registry line for it.

## Gate

Hermetic build (`rm -f bin/wagic && make -f Makefile.sdl -j4`), detached unit `w61-R-gate`,
every run under `MemoryMax=4G MemorySwapMax=0`. No kill under the cap.

| | base `614f8452e` (brief's baseline) | this lane |
|---|---|---|
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1248 tests, 0 failed, 0 timed out | **1249 tests, 0 failed, 0 timed out** |
| AI tests | 65, 0 failed | **65, 0 failed** |
| PARSETEST | 3699 passed, 0 failed | **3740 passed, 0 failed** (+41) |

Both `==Test Failed !==` and `==Test timed out` were counted in the log; both are **0**.
Suite delta +1 is exactly the new fixture. `git diff | /usr/bin/grep -c $'\357\277\275'` = 0.

**Fixture RED on base, GREEN after — how verified.** `w61R_stack_anytarget_total.txt` was run
scoped (`WAGIC_TESTSUITE_FILE=test/_probe.txt`, probe file deleted afterwards) against the archived
base binary `archives/wagic-614f8452e-w60step1` with the identical Res tree:

```
==narration assertion failed for player 1: expected "on the stack: 1 damage to you - you would be
at 19", log was "... | on the stack: ability: staff of nin's deal 1 damage [from their staff of
nin] | "  →  ==Test Failed !==
```

which is the corpus shape verbatim: the row renders, the total does not. The same fixture on the
new binary is `==Test Successful !==`. P1 is the NEGATIVE (`nonarration:` — the same stack, the
other seat, no total claimed for a player the damage is not aimed at) and passes on BOTH binaries.
The fixture drives `TestSuiteAI::Act`'s `stackAbilityRegister`, which runs the production
`stackObjectLifeLossToSeat` → `pendingStackDamageLine` pair, so the red is a red RENDER, not a red
copy of one. It needs P2 to hold an instant response (Mountain + Shock) or the ability resolves
inside the activating tick and no window exists at all — that is stated here because a fixture that
silently loses its window would go falsely green.

**PARSETEST cases shipped (41, `[#W61-R]`).** Per item: a positive on the real corpus shape, a
must-NOT-match negative (the sentence the seat obeyed is asserted GONE, not merely joined), a
byte-identical regression proving every new argument at its default reproduces the wave-60/59 line,
and an echo-shape check — all four items are prompt LINES and introduce no braced or bracketed
annotation, and a reply that quotes them still binds its attackers/blockers despite the digits.
No wave-60 case was deleted or rewritten: every one of the 3699 still passes unchanged, because
each change is gated on a board fact the old cases do not carry.

## Predictions for the next corpus (falsifiable)

1. **C1a.** **0** `ATTACK TOTAL` floors read `they would be at N` on a window whose A-rows carry a
   `blocking trigger:` tag. Falsifier: any record with both substrings. Where the tag is present
   the floor reads `that damage alone puts them at K, but … up to G life back across their B
   blockers, so blocking can leave them as high as K+G`, and the post-combat opponent life in the
   next record for that seat lies in `[K, K+G]`.
2. **C1b.** **0** `KILLS them` claims on any window whose opponent battlefield carries a permanent
   matching `attackDeclarationPunishers`; every such window carries the `Both figures are BEFORE
   their attack triggers:` sentence naming it.
3. **C2.** **0** renders of `may legally block every attacker in that total` on a record whose
   blocker rows number fewer than its unblocked attackers. **100%** of `blockers` records with at
   least one legal pairing carry a `best case with every blocker assigned` / `at least N of that
   lands` clause — the 47-attacker drop cannot recur below 256 attackers.
4. **C3.** **0** `CRACK-BACK NEXT TURN` lines read `for up to N - you would be at K` on a board
   carrying an animator, an activated player-damage source, or a combat-timed pump trigger; those
   read `or lower` and carry the FLOOR clause. And the wave-60 overrun rate (6 of 26 turns took
   more than the printed number) falls on the (a)/(b)/(c) classes specifically — an overrun that
   remains should be a class none of the three names.
5. **C4.** **0** stack blocks containing a `target(anytarget)`-style activated ability aimed at the
   seat render without an `ON THE STACK: N damage to you` line. Falsifier: a Staff of Nin row (or
   any chosen-player damage row) with no total. The four wave-60 renders are the exact repro set.

## What I did NOT verify

* **No live model probe and no corpus run.** Every render change here is PARSETEST-proven and
  code-verified; **not one of the four new emitters has been observed in a real prompt.** C4 alone
  is additionally proven end-to-end through live ability objects by the suite fixture.
* **C1's `blockGain` was never observed against a live Perimeter Captain board.** The arithmetic
  reuses `blockTriggeredLifeFor`, which wave-45 shipped and the per-row tags already print, so the
  inputs are proven; the AGGREGATE over their whole blocker set is not, and I did not construct a
  fixture for it.
* **C1's punisher scan is a script-text predicate, not an ability-graph walk.** It can miss a
  declaration punisher written in a shape other than "triggered line, `damage:`,
  `creature[attacking]`, `all(`/`foreach(`" — Lightmine Field is the only card I checked the bytes
  of. It can in principle fire on a triggered line that damages attacking creatures for a reason
  other than the declaration.
* **C1's `blockGain` ceiling assumes every blocker they own may block.** True whenever at least one
  attacker is declared and gang-blocking is legal; a board where a blocking RESTRICTION (a `may
  block only …` clause) forbids some of them would make the ceiling loose in the safe direction —
  it over-states the gain, never under-states it.
* **C2's `assignableMatchedAttackers` has no live observation.** It is PARSETEST-proven over the
  same map the remainder already solved (including the 47×1 shape), but no corpus record has been
  rendered through it. The 256 cap is also untested at scale — no board in any corpus has come
  near it, and above it the clause still drops (silently, as before).
* **C3's three scans are script-text predicates and each can under-count.** `target(creature,player)`
  is not matched by the `target(player` test; a granted animation or a pump granted by another
  permanent is invisible; a damage source whose amount is an expression rather than a literal digit
  is skipped. Every miss keeps the OLD (ceiling) wording, which is the wrong direction — I state it
  rather than claim coverage. I verified the predicate by hand against the primitive bytes of
  Lightmine Field, Perimeter Captain, Siege-Gang Commander, Staff of Nin, Hive of the Eye Tyrant,
  Luminarch Aspirant and Ranger Class only.
* **C3's evasion sub-total is a NEXT-TURN forecast** and takes no account of what either side plays
  between now and then; `canBlockPairwise` is evaluated on today's bodies.
* **C4's AALifer half is untested end-to-end.** The chosen-target branch is symmetric with the
  AADamager one and covered at the phrase level in PARSETEST, but no fixture and no probe puts a
  chosen-target AALifer on the stack.
* **C4 on SPELLS is still not priced** — lane L's scope note stands unchanged; only StackAbility
  objects are scanned, so the total remains a FLOOR on pending damage in the general case and the
  line does not say so.
* **No card fact was taken from model memory.** Every primitive quoted above was read out of
  `bin/Res/sets/primitives/*.txt` in this worktree. I did NOT re-check any of them against
  Scryfall; the wave-60 reviews state Perimeter Captain, Lightmine Field, Siege-Gang Commander,
  Staff of Nin, Hive of the Eye Tyrant, Luminarch Aspirant and Ranger Class were Oracle-verified
  there.
* **No corpus re-count.** Every "N of M" figure is quoted from the cited review files; I re-read no
  seq record end-to-end myself.
* **No GUI, no console, no merge, no guide or prompt asset touched.**
