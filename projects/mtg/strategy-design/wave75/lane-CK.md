# Wave-75 lane CK — engine truth (P3, P4, P7, P22)

Base: master 207a7f410 (worktree `worktrees/lanes/w75-CK`, branch `w75-lane-CK`).
Gates on the finished tree: suite THREADS=1 **1288 tests (0 failed), 76 AI tests (0 failed)**,
`==Test Failed !==` 0 and `==Test timed out` 0 (the three known flakes — lifeline,
merrow_reejerey, intrepid_adversary_repeated_payment — all passed this run).
PARSETEST **5795 passed, 0 failed** (+7 from this lane's block; base 5788).
Three fixtures added and registered in `_tests.txt` (1406 -> 1409 lines).
Every wagic invocation ran under `systemd-run --user --scope -p MemoryMax=4G
-p MemorySwapMax=0`; nothing was killed under the cap. `git diff | grep -c U+FFFD` = 0.

---

## P3 HIGH — the payability oracle under board-dependent producers  *(fixed)*

**Repro.** `126v162` deck126 seq 22, t15. Board: Chromatic Lantern, Overgrown Battlement #1,
Overgrown Battlement #2, Isolated Chapel — four untapped sources, six real mana (each
Battlement taps for `{G}` per defender, and two defenders are out). Hand carried
`Exquisite Blood {4}{b} [enchantment] [cannot pay now: needs 5 mana, you have 4 untapped
sources]` with **no cast row**, while `Sanguine Bond {3}{b}{b}` — the same converted cost,
strictly harder colours — was row 1 and resolved. Same shape at seqs 17/21/26.

**RED on base.** New fixture `w75ck_payable_foreach_battlement_blood.txt` reproduces the
board exactly (the LAYER ORDER is load-bearing — see below) and asserts both verdicts:

    TESTSUITE assertcastable: 'exquisite blood' expected 1 got 0

with `assertcastable 1 sanguine bond` passing in the same run — the asymmetry itself is
what fails, not a blanket "everything is castable".

**Mechanism.** `ManaEngine::planPayment` is `LegalActionsOracle::payable`'s last resort, and
its pass-0 walk has one exit for a producer whose coloured pips are already covered:

```
if((coloredSatisfied || fullColor == needColorConverted) && result short of cost)
{
    if((cost->hasColor(0) || cost->hasColor(7)) && amp) { ...generic fill... }
    continue;                       // <- unconditional
}
GenericActivatedAbility * gmp = ...  // the foreach branch, never reached
```

A foreach-wrapped VARIABLE producer is a `GenericActivatedAbility`, not an `AManaProducer`,
so `amp` is NULL for it: the generic-fill block does nothing and the `continue` skips the
foreach branch entirely. Passes 1 and 2 cannot rescue it either — `genericFillOrder`
filters to `AManaProducer` too. So a variable producer could contribute to a cost's
GENERIC portion in **no pass at all**, and payability became a function of layer order:
the Lantern (played before the Battlements) paid Blood's single `{b}` pip in pass 0,
`coloredSatisfied` went true at the very next entry, and the four mana standing in two
Walls were never reached. Sanguine Bond survived only because its SECOND `{b}` pip kept
`coloredSatisfied` false long enough for the foreach branch to run twice.

**Fix** (`src/ManaEngine.cpp`, `#W75-CK (P3)`): a foreach-wrapped producer falls THROUGH the
generic-fill gate to its own branch, which already carries the right test (`helps`: take it
only while the total is still short or a needed colour is). Everything else keeps the old
control flow. `potentialMana` still SKIPS foreach producers by design (strict, one ability
per card) — untouched; the planner is where the truth lives, exactly as the wave-52 fixtures
`aipay_foreach_one_battlement_2G/2B` already assume (both still pass: with three defenders
the first Wall covers `{2}{G}` and the other two are not tapped).

**GREEN.** Fixture passes; the two wave-52 foreach fixtures and the whole suite unchanged.

**Falsifiable prediction for the next corpus.** On a deck126 seat with an Overgrown
Battlement untapped, `cannot pay now` tags naming a converted cost the board's VARIABLE
producers cover will drop to zero, and Exquisite Blood will appear as a Cast row in every
window where Sanguine Bond does. Cross-tab: any prompt that still carries a `cannot pay now`
tag AND lists a variable producer in `Those sources` is a residual — file it with the
`Mana available:` line and the hand tag verbatim.

---

## P4 HIGH — Sorin's `-6`  *(root cause is NOT the loyalty cost; fixed in card data)*

**The report's first hypothesis is refuted.** P4 read the board deltas (6 loyalty at seq 38,
3 at seq 39) as "the `-6` charged THREE loyalty (a two-digit/negative clamp?)". The seat's
own narration, which is ground truth, says the opposite:

```
- You used: -6: destroy up to three and reanimate with Sorin, Lord of Innistrad
             targeting Sigarda, Champion of Light
- Your Sorin, Lord of Innistrad lost 6 loyalty counters (now 0)
- Your Sorin, Lord of Innistrad was put into your graveyard from the battlefield
- Your Sorin, Lord of Innistrad entered the battlefield from your graveyard
- Your Sorin, Lord of Innistrad got 3 loyalty counters (now 3)
- Your Pride Guardian died ... entered the battlefield from your graveyard
- Your Vampire died ... entered the battlefield from your zone
- You used: -2: emblem ... Your Sorin, Lord of Innistrad lost 2 loyalty counters
```

Six were charged and the walker died as a state-based action. Independently confirmed on the
engine: a scripted Sorin taken to 6 loyalty by three `+1`s and then given the `-6` goes
6 -> 0 -> graveyard. **No loyalty-cost fixture is warranted, and the "clamp" line of
investigation should not be re-walked.**

**What actually happened.** Targets are chosen BEFORE the cost is paid
(`ActivatedAbility::reactToTargetClick` pays after `targetsReadyCheck`), and the script's
target spec did not exclude the source:

    target(<upto:3>*[creature;planeswalker]|battlefield)

Sorin was legally one of its own `<upto:3>` targets. The cost then killed it, and
`destroy and!(moveto(mybattlefield))!` returned the dead walker — a FRESH card instance with
its printed 3 loyalty. The "6 -> 3" reading is that return, not a mis-parsed cost.

Oracle (Scryfall + the card's own `text=` line): *"-6: Destroy up to three target creatures
and/or **other** planeswalkers."* The engine already supports the word: `other ` in a target
spec sets `TargetChooser::other`, and `canTarget` refuses the source.

**Fix** (`bin/Res/sets/primitives/planeswalkers.txt`, card data — no engine change):

    -target(<upto:3>*[creature;planeswalker]|battlefield) ...
    +target(<upto:3>other *[creature;planeswalker]|battlefield) ...

**Instrument added.** The option SET a target spec produces is invisible to every zone
assertion, so the suite gained `assertcantarget <0|1> <target> [via <source card>]`
(`src/TestSuiteAI.cpp`, `#W75-CK (P4)`). Without `via` it asks the live chooser; with `via`
it asks the target choosers the source's own abilities carry on the action layer, which is
what a script can reach deterministically (a live chooser exists only inside the tick window
between the menu answer and the pick). It compares `ab->source` by POINTER only — walking
`previous` there reads freed memory and SIGSEGVs (measured, gdb backtrace at
`TestSuiteAI.cpp:1514`).

**RED / GREEN.** `w75ck_sorin_minus6_not_self_target.txt`. On the same binary with the word
removed again:

    TESTSUITE assertcantarget: 'sorin, lord of innistrad' expected 0 got 1 (choosers seen: 1)

with `assertcantarget 1 serra angel` passing in both states — the fix must not shrink the
legal set. With `other ` restored: 1 test, 0 failed.

**SUPERSEDED (wave-75 lane CM, Astra review finding 1).** This paragraph read the
once-per-turn loyalty latch (`MTGAbility::isReactingToClick`: refuse when an
`ActivatedAbility` whose `source == card` already has `counters`) as a "general hole"
because a planeswalker that leaves and re-enters the battlefield gets fresh ability
objects and can activate again. **That is not a hole - it is the rule.** CR 400.7: an
object that changes zones becomes a NEW OBJECT with no memory of its previous existence,
and CR 606.3 restricts a loyalty ability to "any time he or she has priority ... and no
loyalty ability of that permanent has been activated this turn" - *that permanent*, i.e.
that object. A Sorin that died and returned is a different permanent and may activate a
loyalty ability again; so may one flickered by Conjurer's Closet, or one bounced and
replayed. An identity latch keyed on the player or on the card's oracle name would
REMOVE LEGAL ACTIVATIONS, which this project does not do. **Nothing is to be built here.**
What the wave-74 `126v152` narration actually shows is one defect, not two: Sorin was a
legal target of his own `-6`, died to the cost, and `and!(moveto(mybattlefield))!`
returned him - the second loyalty ability that turn is the CORRECT behaviour of the new
object, and it disappears when the self-target does. The `other` fix closes it; see
wave75/lane-CM.md F1 for the scoping correction that fix itself needed.

**Also not done, with the reason.** P4 asked for a `{right now:}` clause on the `-6` row
(the O15 gap). The `-6`'s effect has no evaluable magnitude — it is `destroy` +
`moveto(mybattlefield)` over a chosen set, not an expression `WParsedInt` can price — so any
clause would be a NEW bracket carrying board-derived counts, which under the wave-74 lesson
must ship with the full key-stability pin set and belongs with the render lane (CL), not
here. Nothing was invented in its place.

**Falsifiable prediction.** In the next corpus no `-6: destroy up to three and reanimate`
consumption is followed, in the same turn, by a narration pair
`Your Sorin, Lord of Innistrad was put into your graveyard` ->
`Your Sorin, Lord of Innistrad entered the battlefield from your graveyard`, and no seat
takes two Sorin loyalty abilities in one turn. If a second loyalty ability still appears,
the narration will NOT contain that return pair — and that is the general latch hole above,
not this fix failing.

---

## P7 HIGH — the crack-back cover counted a legend-rule duplicate as a body  *(fixed)*

**Repro.** `130v162` deck130 seqs 44/45, t14, at 3 life. Row 1, verbatim:

```
Cast Rorix Bladewing {3}{r}{r}{r} (6/5) [legendary: you already control Rorix Bladewing -
legend rule: casting this sends one copy to your graveyard (you choose which)] ...
{crack-back cover: the CRACK-BACK NEXT TURN line above is 5 from 1 of their creatures and
puts you at -2. This adds 1 body ... you cover 5 of 5, leaving 0 -> you would be at 3,
which you SURVIVE - and more blockers can only lower that, so nothing uncounted here
overturns it.}
```

Two brackets apart, the row states the legend rule and then prices a body that rule takes
back: after the cast the seat controls ONE Rorix, exactly as before it. Under the trust
doctrine the survival verdict is an instruction, so this is a false instruction at 3 life.

**Fix** (`src/AIPlayerGPT.cpp`, `#W75-CK (P7)`): a pure helper
`castBodiesNetOfOwnText(bodies, cardIsCreature, legendTwinControlled)` subtracts one body
when a CREATURE card's legendary twin is already on the seat's own battlefield, and the
cast row computes `legendTwinControlled` from the SAME board fact the legend-rule bracket
above it is emitted from (a non-token same-name copy in `game->inPlay`), so the two cannot
disagree. Only a creature loses a body to CR 704.5j — a legendary artifact's tokens are not
the legend. At 0 net bodies `crackBackBlockerRowTag` returns "", so the row prices nothing
and no verdict rests on the phantom blocker; the legend-rule bracket still states the whole
fact, so nothing is deleted from the model's view.

**RED / GREEN — PARSETEST pin** (7 CHECKs, corpus 5788 -> 5795, 0 failed). The block
reproduces the shipped clause on the exact Rorix board and then shows the netted row:

```
CHECK(withBody.find("This adds 1 body") && withBody.find("which you SURVIVE"))   // REPRO
CHECK(crackBackBlockerRowTag(5, 3, 0, 0, rorix, false, 0).empty())               // after
```

plus the table: `(1,creature,twin)->0`, `(1,creature,no twin)->1` (MUST-NOT-MATCH: the first
copy still adds its body), `(3,creature,twin)->2` (tokens survive the rule),
`(2,non-creature,twin)->2` (MUST-NOT-MATCH), and the `<=0` identities.
No key-stability pin was needed: `{crack-back cover: ` is already on the history/ask-key
strip list (`#W64-AH F11`) and this change adds no bracket.

**Falsifiable prediction.** In the next corpus no row carrying
`[legendary: you already control <name> - legend rule: ...]` also carries a
`{crack-back cover: ...}` clause; the same cards' FIRST copies still carry it.

---

## P22 LOW — the pathway `anyzone` parse errors  *(fixed; both messages were false)*

**Repro** (scoped run, Brightclimb Pathway on the battlefield, Branchloft in hand):

```
ABILITYFACTORY Error parsing: {0}:doubleside(grimclimb pathway)
 no matching ability found. doubleside(grimclimb pathway)
ABILITYFACTORY ERROR: Parser returned NULL {t}:add{w}
```

Three lines, two of them ERROR. Both diagnoses are wrong, and the second indicts an
innocent line.

1. `anyzone={0}:doubleside(<back face>)` is parsed in EVERY zone, and the doubleside parser
   refuses on the battlefield **by design** ("It's not allowed to turn side on battlefield")
   — a permanent's face is not turned there. A by-design NULL, exactly like the existing
   unpaid-alternative-cost branch, wearing an ERROR costume.
2. `AbilityFactory::getAbilities` CONSUMES `magicText` as it walks the card's lines, so the
   failure report printed the REMAINING text — the line AFTER the one that failed. That is
   how ONE by-design refusal per pathway came to be filed as six broken `{t}:add{g}` /
   `{t}:add{w}` mana abilities. Those lines parse and work.

**Fix** (`src/MTGAbility.cpp` + `include/MTGAbility.h`, `#W75-CK (P22)`): a sibling of the
existing by-design flag, `gAbilityParseZoneGatedNull`, set by the doubleside zone gate,
reset per top-level line beside `gAbilityParseAltCostUnpaid`, consumed at all three report
sites (INFO instead of ERROR, and the low-level tail says "not available in this zone"), and
passed to the `WAGIC_VALIDATE` sink's `byDesign` argument so the validator counts it as a
skip. The generic failure report now prints `line`, not the consumed `magicText`.

**Measured, same fixture, same binary:** before 3 lines / 2 ERROR; after 2 lines / 0 ERROR
(`INFO ABILITYFACTORY: line not available in this zone (by design): {0}:doubleside(...)`).
`WAGIC_VALIDATE=1` over the whole collection: 67,587 cards, 141,431 lines, 22 failures,
0 warnings — unchanged by this lane, and no pathway among them.

**Pin.** `w75ck_pathway_anyzone_parse_quiet.txt` pins the innocence of the accused line:
the pathway on the battlefield pays a `{W}` cost through the human auto-tap
(`TESTSUITE autotap savannah lions picks: Brightclimb Pathway`), leaving 1 tapped permanent
and `{W}` in pool.

**Falsifiable prediction.** No corpus stderr from wave 75 on contains
`ABILITYFACTORY ERROR: Parser returned NULL {t}:add{` — and if any `Parser returned NULL`
survives, the string it now prints is the line that actually failed, so the next reader is
not sent after the wrong card line.

---

## Weakest evidence

1. **P3's fixture is not the corpus game.** It reproduces the corpus BOARD and the corpus
   verdict, and the mechanism is read off the source, but the seat's own policy
   (`GptManaPolicy`) is not what `assertcastable` runs (`castableForDisplay` uses the
   free-producer default). Both reach the same `payable` -> `planPayment`, and the first
   version of the fixture — same cards, Battlements listed BEFORE the Lantern — went
   falsely GREEN, which is exactly how order-dependence hides. The pin is honest about the
   order it needs; a future edit that changes layer order in the walk could make it pass for
   the wrong reason. The real gate is the next corpus's `cannot pay now` census.
2. **P4's resolution path is argued, not executed.** I could not drive the `<upto:3>` chooser
   to completion from a script (it auto-closes within a tick window), so the fixture pins the
   TARGET SET, not the reanimation. The chain "Sorin was targetable -> Sorin was targeted ->
   the moveto returned it" rests on the narration's own ordering, which is strong but is one
   game. If the next corpus shows the return WITHOUT a self-target being possible, the
   mechanism is something else (the `and!()!` binding rather than the target list) and this
   fix will have been necessary but not sufficient.
3. **P7 is a pure-function pin, not a rendered row.** The PARSETEST block proves the helper
   and proves the clause disappears at 0 bodies; it does not prove that the callsite computes
   `legendTwinControlled` correctly against a live battlefield (the twin walk skips tokens
   and the card itself, but no fixture renders it). Cross-tab in the next corpus is cheap and
   is the prediction above.
4. **P22's harm remains unquantified.** The claim is only that both messages were false and
   the mana lines work; nothing here shows the noise ever cost a decision. The `anyzone`
   parse also still runs (and still fails) once per pathway per parse — the fix changes what
   is SAID about it, not that it happens.
5. The suite run reported 0 failed including the three known flakes, so this run gives no
   information about whether the flake rate moved.
