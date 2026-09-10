# Wave-76 lane CO — combat and loop truth (Q3, Q4, Q5, Q6)

Base: master 77b920612 (worktree `worktrees/lanes/w76-CO`, branch `w76-lane-CO`).
Corpus for every repro and census: `matchups-20260910-003556-final` (2,919 records,
21 games, single base binary `archives/wagic-7a04cd6da-w75step1`).
Gate: **PARSETEST 5954 -> 5996, 0 failed** (+42 cases). **Suite 1289 tests (0 failed)
+ 76 AI tests (0 failed)**, THREADS=1, detached unit, both failure strings counted, 0
known flakes hit. `check-reply-instructions.py`: OK (1 source, 37 guides). U+FFFD 0.
Commit `775b2ac4a`.

One census script served all four items; it takes the run directory as its argument
and echoes it (wave-75 lesson 2). It is a scratch tool under `~/.gatelogs/`, not a
repo artifact.

---

## Q3 HIGH — the loop-complete false-verdict family, outside P5's `{right now:}` scope

### Repro

**(a) the combat header.** `146v126` deck146 **seq 70** (T22 Blockers, 20 life vs 32).
Six lines apart, on one screen:

```
INCOMING THIS COMBAT: 1 attacker, 1 unblocked damage - you would be at 19 ... - NO
best-case life figure is given: both halves of their life LOOP are in play, so any
life a block gains them AND any life this combat costs you both enter a chain that
does not stop until you are at 0 ... no survival claim is made from these figures.
...
Your life: 20. Unblocked, these attackers deal up to 1 - you would be at 19 - NOT
lethal: block only where the trade favors you.
```

The attacker is a 1/1 **lifelink** Vampire under their Sanguine Bond + Exquisite
Blood. Every line of the prompt that could refuse a survival claim did; the header
made one anyway. deck146 lost that game.

**(b) the own-clock tag.** `125v123` deck125 **seqs 381-397** (t77). The screen prints
`CRACK-BACK NEXT TURN: ... you would be at -84; that would KILL you` and, six lines
below, on the decline row, `{the clock you already control: ... the opponent reaches 0
in N more turns, with no card spent}`. #W75-CL (P10) took the stack and the combat
declaration; the crack-back line was the one stated lethal it did not read.

### RED on base

* **Census (a)**: `NOT lethal: block only where the trade favors you` renders **25**
  times corpus-wide; **3** of them sit on a prompt whose INCOMING block prints
  `NO best-case life figure is given` — `146v126` seqs **64** and **70**, `162v126`
  seq **13**. (known-bugs' "8 renders" counts the neighbouring seqs of the same
  windows; the measurable render-level figure is 3.)
* **Census (b)**: **193** own-clock renders; **8** are on a prompt whose
  `CRACK-BACK NEXT TURN:` line carries `that would KILL you` — all eight on
  `125v123` deck125 t77 (seqs 381, 382, 384, 385, 387, 392, 393, 395).
* **PARSETEST, on the seeded tree's own encoded behaviour** (both shipped as standing
  `RED-ON-BASE` assertions rather than as prose):
  `combatDamageForecast(20, 0, 1, 0, 32)` — the base call, and the exact seq-70
  numbers — contains `you would be at 19 - NOT lethal: block only where the trade
  favors you`; and the base's own-clock gate has **no crack-back argument at all**, so
  `w76StatedLethalOnScreen(97, 0, 0, 181, false)` is not expressible on base.

### Fix

`w76StatedLethalOnScreen(myLife, incomingCombat, stackDamage, crackBackDamage,
oppLoopLive)` — **one** predicate for "this screen already states a lethal", pure over
its inputs. Every source is a **stated** one, so the predicate can never disagree with
the screen the reader has:

* the loop arm is #W75-CL (P10)'s, kept verbatim — under a proven opponent loop a
  stated loss of ANY size (stack or combat declaration) is the lethal, and **the loop
  alone suppresses nothing**;
* the arithmetic arm is `myLife - worst <= 0`;
* the **new** crack-back arm keys on exactly the arithmetic the CRACK-BACK NEXT TURN
  line prints its own `that would KILL you` from — `statedCrackBackLossToSeat` returns
  `crackBackScreenTotal` plus the floor's ADD-THOSE-UP figure, which is both branches
  the line prints that clause from, so the two cannot drift. A crack-back is a whole
  turn cycle away and the seat untaps first, so it does **not** arm the loop arm.

`w75OwnClockSuppressed` is now that predicate with no crack-back stated (its wave-75
pins are unmoved, pinned as an identity). `ownClockTagFor` takes the crack-back at both
call sites (priority row 0, the casting decline row).
`combatDamageForecast` takes `oppLoopLive` = `lifeLoopProvenWin(opponent())` — the same
predicate the INCOMING block is rendered from — and, on a nonzero life swing under it,
**withdraws the survival verdict and says so**, in the same register #W63-AB (E1) uses
on the INCOMING line itself. Nothing is deleted: the subtraction still prints. The
take-the-damage-while-ahead hint goes with the verdict (against a chain it is the
inverse of correct play).

### GREEN

+13 cases. Repro pins for both halves; MUST-NOT-MATCH for: a no-loop window byte-
identical to the wave-75 five-argument call; an ordinary lethal swing still saying
LETHAL under a loop; a pure POISON swing untouched (no life is lost, nothing enters the
chain); a **survivable** crack-back stating no lethal; the loop with **no** stated loss
suppressing nothing; an unknown life total claiming nothing; and the four-argument
wave-75 gate as an identity.

### Prediction

Next corpus: **0** prompts carry `NOT lethal: block only where the trade favors you`
while also carrying `NO best-case life figure is given` (wave 75: 3), and **0**
own-clock renders sit on a prompt whose `CRACK-BACK NEXT TURN:` line says
`that would KILL you` (wave 75: 8).
**Pin for the negative clause** (the clock must not be over-deleted):
`{the clock you already control:` still renders on windows with no stated loss — the
census to run is own-clock renders under a live `LOOP COMPLETE ... THEIR battlefield`
with no nonzero loss on the screen; wave 75 had 57 such renders at 28-38 life and all
57 must survive. Falsifier: that count falling, or a seat losing a game whose clock was
true and deleted.

---

## Q4 HIGH — ATTACK TOTAL's aggregate cover and floor are false on two boards

### Repro

**(a) menace.** `126v146` deck126 **seq 45** (t20, attackers seam, under a live LOOP
COMPLETE). Four offered attackers — Barrowin (3/3), Nadaar (3/3), Spider #1 (2/1)
**menace**, Spider #2 (2/1) **menace** — against four untapped blockers. Verbatim:

```
A3. Spider #1 (2/1) [reach, menace ...] [menace - cannot be blocked by fewer than two
creatures; they have 4 untapped creatures that could join such a block ...]
...
ATTACK TOTAL: 4 attackers listed, 10 total combat damage ... Their 4 untapped blockers
can cover every attacker you could send, so none of that damage is guaranteed.
```

Covering all four costs 1+1+2+2 = **6** bodies and they have 4. The per-attacker tags
in the same prompt model menace correctly; only the aggregate — the line the model
reads first — did not.

**(b) the floor over an attack trigger.** `125v123` deck123 **seq 1208**:

```
ATTACK TOTAL: 100 attackers listed, 401 total combat damage ... At least 401 damage
lands whatever they block - they would be at -304. Both figures are BEFORE their attack
triggers: Lightmine Field, Lightmine Field fires on the declaration ...
```

`ATTACK: all` was taken; the declaration trigger destroyed the board before damage and
the opponent's life did not move from 97. The requalification was a **footnote under an
already-stated claim** — the shape #W64-AK settled ("a true footnote cannot repair a
false verdict").

Both Oracle checks done against the engine's own script, not a card list: the trigger
class is `boardTurnOnClause`'s `attackPunisher` predicate (`@each blockers` +
`creature[attacking]` + `damage:`), which is what names Lightmine Field; the blockers
requirement is the engine's `MTGCardInstance::minBlockersRequired()`, the same call the
per-attacker menace tag is rendered from.

### RED on base

* **Census (a)**: **4** attackers-seam renders assert `can cover every attacker you
  could send` with at least one **menace** attacker among the A-rows — `126v146` seqs
  45 and 59, `146v126` seqs 57 and 69. seq 45 is hand-verified above.
* **Census (b)**: `At least N damage lands whatever they block` renders with a named
  attack punisher on the same line — seq 1208 is the corpus specimen and the whole
  100-attacker swing rode it.
* **PARSETEST RED-ON-BASE**: `attackTotalLine(100, 401, 97, 0, 401)` — the base call —
  contains `At least 401 damage lands whatever they block - they would be at -304`,
  which is the sentence deck123 read.

### Fix

**(a)** `w76BlockableCoveredPower(powers, blockersNeeded, blockerCount)`: the most power
their bodies can legally block, as an exact knapsack (capacity = their untapped blocker
count; each attacker's weight = `minBlockersRequired()`, CR 509.1c; value = its power).
`guaranteed` becomes `blockableTotal - covered`, plus the unblockable attackers as
before. The direction is the safe one — maximising what they block **minimises** what is
claimed guaranteed. With every weight 1 the DP picks the `blockerCount` largest powers,
which is byte-for-byte the wave-60 sum-of-the-smallest-remaining, so no non-menace
window moves. `rowBlockersNeeded` is filled in the same pass as `rowPower`, off the same
`attackers[j]`.

**(b)** the floor sentence is requalified **in place** when `attackPunishers` is
non-empty, naming the punisher in the same clause as the number: `At least N damage
lands whatever they BLOCK, but that floor is over their BLOCKS ONLY: <punisher> fires on
your declaration, before any combat damage, and can remove attackers from that total -
so it is NOT a floor on what lands`. Every number still prints; #W61-R (C1b)'s kill-claim
gate is untouched.

### GREEN

+13 cases. Q4a: the seq-45 board gives covered 8 of 10 -> **2 guaranteed** (base: 0);
all-ones identity to the wave-60 arithmetic; a three-body requirement against two
blockers covered at all; zero-blocker and empty-board identities. Q4b: the categorical
sentence gone under a punisher, the punisher named in the clause, no kill claim, and the
no-punisher wording byte-identical.

### Prediction

Next corpus: **0** `ATTACK TOTAL` lines say `can cover every attacker you could send`
on a window whose A-rows include a menace attacker while the offered attacker count
exceeds their blocker count in bodies (wave 75: 4); and **0** lines carry
`damage lands whatever they block` on a prompt that also names an attack punisher
(wave 75: every such render did).
**Pin for the negative clause** (no over-suppression): windows with no menace attacker
must render the wave-60 floor sentence unchanged — the cross-tab is
`At least N damage lands whatever they block -` present × no `menace` string in the
A-rows × no punisher named; wave 75's count of that shape must not fall.

---

## Q5 HIGH — crack-back arithmetic is asymmetric

### Repro

`130v152` deck130 **seq 23** (t10, 9 life, casting menu). The screen prints
`CRACK-BACK NEXT TURN: 2 of their creatures ... for up to 6 from combat ... you would be
at 4 or lower - of that, 5 from 1 attacker nothing you control can legally block - and
that number is a FLOOR ...` (Luminarch Aspirant). The menu:

* row 2 `Cast Starstorm {r}{r}{x}` — its own X pricing names
  `At X=3 - kills THEIRS: Elite Spellbinder, Luminarch Aspirant`, **both** crack-back
  attackers — and carries **no** crack-back clause;
* row 3 `Cast Siege-Gang Commander` — adds bodies — carries the full
  `{crack-back cover: ... you cover 1 of 6, leaving 5 ...}` paragraph;
* row 4 `Cast Hammer of Bogardan` — `kills whichever you target: THEIRS - Elite
  Spellbinder, Luminarch Aspirant` — **no** clause.

The seat took row 3 and lost. Removing an attacker and blocking one are the same
arithmetic on the same line; only one of them was priced.

### RED on base

* **Census**: **529** prompts carry a `CRACK-BACK NEXT TURN:` line; **38** carry a
  `{crack-back cover:` clause, and every one of those 38 is the blocker (bodies-added)
  or edict form. **13** crack-back prompts carry a row that names a kill of THEIRS
  (`kills THEIRS:` / a `destroys` sweep) — none of them got a cover clause.
* **PARSETEST**: `crackBackKillRowTag` does not exist on the seeded tree; every case in
  the block is red by construction there.

### Fix

`crackBackKillRowTag(total, myLife, totalIsFloor, removedPower, removedBodies,
attackerBodies)` — pure, and a member of the existing cover family (same opening
`{crack-back cover: `, same FLOOR wording, same closing disclaimer). It says what the row
takes OFF the total and where that leaves the seat, and it obeys #W65-AL (G4) without
exception: **no survival verdict against a total the line above calls a FLOOR**. The
clause carries no CHECKED/unchecked split — a dead attacker does not need a legal block —
and returns "" whenever the row removes nothing, no line is printed, or the two walks
disagree (`removedBodies > attackerBodies`).

The input comes from the row's own kill walk, not a second one: `CastRowBoardAnswer`
gained `crackRemoved` / `crackRemovedBodies`, summed with **`crackBackBodyContribution`**
— the per-body half of the walk the CRACK-BACK line itself is built from — at the two
places that already enumerate the victims. `boardCreatureCounts` accumulates it for the
sweeper roster; `xSpellPricing` accumulates it over `sv.victims` at the X its **own**
marker names, so the clause and the marker can never name different casts. `XDamVictim`
now carries its instance so the match is by identity, never by rendered name. The wiring
is gated through the same `crackBackScreenTotal` as the two sibling blocks, so this row
can never point at a line that is not above it; the edict form keeps its own clause.

### GREEN

+11 cases. The seq-23 shape (`This row REMOVES 2 of those 2 attackers ... takes 6 off
the total, leaving 0`) with no survival verdict on the floor; the CEILING branch
completing the subtraction and printing SURVIVE; a removal that falls short printing
`which still KILLS you`; four silent cases; and the full **key-stability pin set** —
hold-latch (`holdActionKeyRow`), hold-check (the held set built as the live seam builds
it, `mLastMenuRows` via `holdActionKeyRow`), option-set (`optionSetKeyOf`), and ask/async
slot key. The last of these is stated as the truth rather than as a wish: the ask key and
the slot key ARE the rendered tail (`stripDeclineReaskTags` touches only the decline-reask
tag), so a `{...}` price group's board numbers ride them and have since wave 54 — that is
the key's intended BOARD-STATE + QUESTION semantics, and the pin is that this clause
enters no key path its sibling `crackBackBlockerRowTag` does not already enter. A first
draft of that pin asserted key-equality and **failed** on the seeded tree; the code was
right and the pin was wrong.

### Prediction

Next corpus: every cast row that names a kill of one of THEIRS on a prompt carrying a
`CRACK-BACK NEXT TURN:` line also carries a `{crack-back cover:` clause (wave 75: 0 of
13), and the clause's `REMOVES K of those N attackers` count never exceeds the line's own
`N of their creatures`.
**Pin for the negative clause** (no clause where nothing is removed): the count of
`{crack-back cover:` clauses containing `This row REMOVES` must be **0** on rows whose
victims are all outside the crack-back total — the cross-tab is the clause present ×
the row's named victims × the line's attacker count. A clause on a row that removes no
attacker falsifies the fix.

---

## Q6 HIGH — the forced-sacrifice row states half the price under their converter

### Repro

`162v126` deck162 **seq 10** (t11, 18 life vs 21). Header: `... THE PRICE: the OPPONENT
gains life equal to the sacrificed creature's TOUGHNESS ... the one that pays the least`.
Rows: `Shield Sphere #1 (0/6) ... [you SACRIFICE this; they gain 6 life (its toughness)]`
×2 and `Fog Bank (0/2) ... they gain 2`. The same prompt's frame carries
`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond`. Their gain of 6 is
6 off the **seat's** total (Sanguine Bond, Oracle: "Whenever you gain life, target
opponent loses that much life"; engine script
`@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent` — verified through
`lifeToDamageConverterTake`, which reads that exact line). The larger half of the price
the tie-break ranks was never printed.

### RED on base

* PARSETEST: `forcedSacrificeRowTag(1, 6)` on the seeded tree is exactly
  `" [you SACRIFICE this; they gain 6 life (its toughness)]"` — the seq-10 row, verbatim
  — and the function has no converter parameter, so every new case is red there.

### Fix

`forcedSacrificeRowTag` takes `convName` / `convTakes` and, on the gain-1 branch only,
appends `- and with their <converter> that is N off YOUR life`. The call site scans the
opponent's battlefield with **`lifeToDamageConverterTake`**, the same helper and the same
scan shape #W53-O (D13) already uses on the edict row, so the two surfaces cannot
disagree about one board. A converter whose amount is not knowable (take 0) prints
nothing rather than a guess.

**Key stability**: the only number added is a function of the toughness already printed
on the row. This clause lives inside a `[...]` bracket, which the ask key does **not**
strip — a life-derived number there would mint a fresh key every time the same forced
choice is re-put at a different life total, which is the wave-49 CG shape. A draft
carrying `so you would be at N` was removed for exactly that reason; the consequence for
the seat's life total is what the LIFE-TO-DAMAGE CONVERTER paragraph on the same screen
is for. Pinned as a MUST-NOT-MATCH (`you would be at` absent from the bracket).

### GREEN

+6 cases: the helper reading Sanguine Bond's real primitive line; the full seq-10 row
byte-exact; three MUST-NOT-MATCH (no converter -> the wave-59 wording byte for byte; the
gain-2 branch untouched, since a converter of THEIRS does not convert the seat's gains;
take 0 -> nothing); and the key pin.

### Prediction

Next corpus: every `[you SACRIFICE this; they gain N life (its toughness)]` row rendered
while a converter of theirs is on the battlefield also carries
`and with their <name> that is N off YOUR life` (wave 75: 0 of 3), and no such row
carries `you would be at`.
**Pin for the negative clause**: rows rendered with **no** converter of theirs must stay
byte-identical to wave 75's — the cross-tab is the row present × no
`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs` in the prompt × the clause absent.

---

## Weakest evidence

1. **Q4a's census is an upper bound, not four proven falsehoods.** The 4 renders are
   "the aggregate claimed full cover and a menace attacker was among the A-rows"; only
   `126v146` seq 45 was hand-checked against the blocker count (4 bodies, needs 6). The
   other three may be boards where their blocker count covers the menace requirement
   anyway, in which case the line was true and the fix is a no-op there. The A-rows are
   also collapsed on wide boards, so a menace attacker inside a collapsed run could be
   invisible to the census in either direction.
2. **Q5's live coverage is untested end to end.** Every PARSETEST case exercises
   `crackBackKillRowTag` and the two `CastRowBoardAnswer` fill sites are compile-verified,
   but no game was run: whether `xSpellPricing`'s `sv.sweep` gate and the sweeper roster
   actually fire on the deck130 seq-23 board (Starstorm's X survey, Hammer's targeted
   form) is a hypothesis until a corpus renders one. **The Hammer of Bogardan shape is
   deliberately NOT covered**: a targeted removal's victim is the seat's own next choice,
   so no row-level claim about what it removes is honest yet — 2 of the seq-23 menu's 3
   unpriced rows are covered, the third is not. That is a real residual, not a closure.
3. **Q3a's fix is scoped to the header alone.** The corpus has **9** `which you SURVIVE`
   renders and **0** of them sit under a live opponent loop, so the crack-back cover
   family's survival verdicts got no loop gate — there is no RED to justify one, and
   adding an unevidenced gate would risk deleting true tokens. If a loop-live board ever
   produces one, the cover clauses will state a survival the chain overturns.
4. **The engine's own reference for "the screen states a lethal" is not logged.**
   `w76StatedLethalOnScreen` reads the crack-back off a live walk; the record does not
   carry what the line printed, so the next wave's census must re-derive it from the
   prompt text exactly as this one did (grep the `CRACK-BACK NEXT TURN:` line for
   `that would KILL you`). Logging the predicate's own verdict per record would make the
   Q3 prediction directly checkable instead of re-derived.
5. **Q3's 3-vs-8 discrepancy with known-bugs.** known-bugs Q3(a) says "8 renders
   corpus-wide"; the render-level measurement is 3 (and Q3(b)'s "9 of 191" measures as 8
   of 193). The difference is a counting convention, not a disagreement about the boards
   — but the numbers in this file are the ones the next wave should compare against, and
   the earlier ones should not be re-cited.
