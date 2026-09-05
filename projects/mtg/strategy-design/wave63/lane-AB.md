# Wave-63 lane AB — E1, E3, E4 (combat renders: the block side of the life loop, the block suggestion, gang blocks)

Base: master `41fbad879`. Branch `w63-lane-AB`. Worktree `worktrees/lanes/w63-AB`.
One file changed: `projects/mtg/src/AIPlayerGPT.cpp`. Every wagic run memory-capped at
4 G with swap off; the gate ran detached under `w63-AB-gate`. No cap kill.

---

## What shipped

### E1 — the life LOOP on the BLOCK side (deck123 HIGH-1; D2 was attack-side only)

`123v126` seq 76: the blockers header promised *"best case with every blocker assigned:
you would be at 16"* and the B rows priced *"they gain 1 from this block only"* beside
*"(you kill it, your blocker lives)"*, with Sanguine Bond + Exquisite Blood on THEIR
battlefield. The block ran the seat 16 -> 0 in eleven lose-1/gain-1 pairs off a won board.

Two emitters, one rule, D2's shape exactly — no survival number, the reason NAMED rather
than left as a gap to confabulate into.

**The header.** `incomingCombatLine` takes `oppLifeLoopClosed`, supplied by
`lifeLoopProvenWin(opp)` — the #W62-AA R6 gate, so the clause never prints over a board
where the chain provably cannot run (Teferi's Protection, `cantchangelife`, a seat that
cannot gain). With it true and the seat holding bodies, BOTH best-case branches are
withdrawn (the exact one and the trample/menace floor) and replaced by the mechanism:
any life a block gains them AND any life the combat costs this seat both enter the chain,
so no assignment of blockers bounds it and no resulting-life figure is a survival claim.

The chain is entered from both ends because the mirror fires on damage-based life loss —
which is not an assumption: #W62-Y (D3) measured it on this engine with two suite fixtures
last wave. That is why **the decline line is stated only where it is provably outside the
chain**: with unblocked damage 0, declining costs no life and gains them none and is named
as the one line that enters the chain nowhere; with unblocked damage > 0 the render says
so, in the same sentence, rather than implying `BLOCKS: none` is safe. This is stricter
than the ledger's "declining is the survivable line when it is" reads at first glance, and
it is the reason the qualifier is in the ledger: on seq 76's own board declining was not
survivable either, and a header that said it was would be the same class of false surface
the item is about.

**The rows.** `combatTradePreviewStats` takes `foeLifeLoop`, and one tail rides exactly
the clauses whose gainer is the READING seat's opponent: the attacker's lifelink on a B row
(`123v126` seq 76's own clause), and — on the attackers window, where `b` is the blocking
side — their blocker's lifelink and their blocking trigger. It states the mechanism
(*"ANY life they gain chains without limit until you are at 0: this is not a priced trade
and the number above is not a ceiling"*), never a prediction. A gain to the READER never
carries it. Both windows pass the same `lifeLoopProvenWin(opponent())` board fact, computed
once per window beside the existing `oppLifeLoop`, so the row tails, the A-row loop tag and
the attackers-window scope paragraph cannot disagree about the same board.

### E3 — the survivable-block suggestion: material, and the lethal screen (deck152 HIGH-1/2)

*(a) it was life-maximising only.* `assignableRemainderDamage`'s matching maximises
PREVENTED DAMAGE and nothing else, so among the many assignments reaching the same number
it printed whichever the augmenting walk happened to build — `146v152` seq 25 named
`Wolf blocks Barrowin`, a chump, while the same Wolf line offered two
`(you kill it, your blocker lives)` results; the seat copied the header verbatim.

`improveAssignmentMaterial` (pure) now post-processes the matching with two
**prevention-preserving** moves only: SWAP two matched blockers' attackers when both
pairings are legal and the ranks improve, and REPLACE a matched blocker with an idle one
that can block the same attacker at a better rank. Neither move changes WHICH attackers are
blocked, so `bestCase` — the life figure the header prints — is untouched by it and the two
cannot drift. The rank is `blockPairMaterialRank`, 4 = kill and live > 3 = neither dies >
2 = trade > 1 = chump, taken from `combatTradePreviewStats`'s own lethality via a new
defaulted `outAttackerDies` out-param, so the header's ranking and the row's printed verdict
are the same computation. The rank matrix costs one forecast per legal pairing and is
bounded at 2048 pairings; past the bound no material is claimed (`blockersDying = -1`) and
the clause says nothing about it. `blockAssignmentClause` states the material fact it was
chosen on — *"chosen for your blockers' material as well as for the life"* plus
*"every blocker in it survives"* / *"N of those blockers die"* — so the ranking is checkable
rather than implicit.

*(b) on a lethal screen it withdrew entirely* and the unguided seat picked the
damage-MAXIMISING block (seq 28). The line is now printed on the lethal sub-branch too,
labelled as what it is: *"no assignment of your blockers survives this; the assignment that
lets in the LEAST damage is: ..."*, beside the unchanged `no block saves you` badge. No
survival is claimed in either wording. The FLOOR branch (trample/menace in the total) still
names nothing — D12's reason holds: no single assignment reaches that figure.

### E4 — the gang verdict, and prices that SUM (deck162 HIGH; deck146 HIGH-2)

*(a) ordering was the whole defect.* `- GANG BLOCK:` sat at the END of the attacker tag,
behind three friendly per-blocker parentheses on an UNCOLLAPSED tag (`162v146` seq 16 — the
reply reasoned the gang block out loud and attacked into it anyway, opponent at 1). The
model reads the favourable outcomes first and the verdict that revokes them arrives after
the decision is framed. The verdict now LEADS `potentialBlockersTag`, ahead of the
enumerated or collapsed results, and `gangBlockPriceTag` gains `resultsBelow` so its own
pointer at those results inverts with the move — a pointer naming the wrong direction would
be its own false surface. The blockers-window voicing (`mine = true`, its own bracket)
is untouched.

*(b) the prices never said they add.* A-lines price `(blocking trigger:` and lifelink per
LONE block and nothing said several blockers pay several times: `146v126` seq 32, worst
single tag 5, three walls gang-blocked, opponent gained 9. `blockPriceSumTag` (pure) states
it from exactly the candidates the tag lists, with the `(N if all block)` figure, and rides
the tag beside the gang verdict in both the enumerated and the collapsed form. The certain
and the "may" halves are kept apart and never summed into one number — a gain the script
only MAY take is not a figure this seat can rely on — and the clause is silent when fewer
than two of the listed candidates carry a price at all, because then the sum IS the single
price the tag already prints.

### Re-pinned PARSETEST expectations (deliberate, not drift)

Five wave-45/48/62 cases pinned surfaces these items move. Each was replaced in place, with
its own `#W63-AB` comment saying why, and none was deleted:

* `#W45-2` and `#W48-D4` order cases — the gang verdict now LEADS the bracket, so both
  re-pin the new order (verdict, bodies/price, note).
* two `#W62-Z D12` head-string equalities — the clause head now states why this assignment
  and not another of the same life figure.
* `#W62-Z D12 MUST-NOT-MATCH a lethal best case names no saving assignment` — **deliberately
  reversed**: it now pins that the lethal screen DOES name the least-damage line, labelled,
  and still carries `no block saves you` and no `one legal assignment` wording.

---

## Gate

Clean rebuild (`rm -f bin/wagic && make -f Makefile.sdl -j4`), detached unit `w63-AB-gate`,
`MemoryMax=4G` / `MemorySwapMax=0` on the unit and on every interactive run. No cap kill.

| | base (brief, `caa5d6ea3`) | this lane |
|---|---|---|
| PARSETEST | 4111 passed, 0 failed | **4146 passed, 0 failed** (+35) |
| suite, default threads | 1254, 2 known concurrency failures | **1254 tests, 2 failed**, `==Test Failed !==` **2**, `==Test timed out` **0**, 66 AI / 0 failed |
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1254, 0 failed | **1254 tests, 0 failed**, `==Test Failed !==` **0**, `==Test timed out` **0**, 66 AI / 0 failed |

The two default-threads failures are the brief's own known concurrency-only pair, named from
the log: `lifeline.txt` and `merrow_reejerey.txt`. Both are green at THREADS=1, which is
ground truth. **0 new failures.** (The AI count reads 66 here against the brief's 65; the
brief's baseline is `caa5d6ea3` and lane Z added `w62Z_designation_marker_not_a_permanent`
before the wave-62 close this lane branches from. 0 failed either way.)

`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.

### RED-on-base evidence

Nothing here changes what the ENGINE does with a card, an option or an answer — all three
items are render/classification changes — so no suite fixture can be RED and none is
claimed (the same standing as wave-62 lane X). RED was produced instead by reverting every
mechanism to its wave-62 behaviour with the new cases already in place and rebuilding:
`oppLifeLoopClosed` forced false, `foeLifeLoop` forced false, `improveAssignmentMaterial`
made a no-op, `blockAssignmentClause` returned its wave-62 head, the header's lethal branch
restored to suppressing the assignment, `resultsBelow` ignored, the hoist reverted to the
wave-62 tail order, and `blockPriceSumTag` emptied.

Result: **4125 passed, 21 failed** — 6 E1, 5 E3 (+2 of the re-pinned D12 head cases), 6 E4
(+2 of the re-pinned order cases). The other 14 new cases are NEGATIVE / MUST-NOT-MATCH /
regression / echo-shape pins that pass on base too, which is what they are for. The tree was
restored from a byte copy taken before the revert and rebuilt clean before the gate.

---

## Predictions (falsifiable, for the wave-63 corpus)

1. **E1 header** — 0 `INCOMING THIS COMBAT` lines contain both
   `best case with every blocker assigned` and `both halves of their life LOOP are in play`
   in the same render. Every such line on a board where the opponent holds a CLOSED loop
   reads `NO best-case life figure is given`, and 0 of them print a
   `you would be at N` best-case or `AT BEST` figure. Falsifier: one co-occurrence, or one
   loop board that still prints a best-case life.
2. **E1 rows** — every B-row `{their attacker's lifelink, this block: ...}` in a record whose
   opponent holds a closed loop carries `chains without limit until you are at 0`; 0 clauses
   naming life the READING seat gains carry that tail. Falsifier: one of either.
3. **E1 decline** — 0 renders read `Declining every block costs you no life` on a record
   whose same line prints a nonzero `unblocked damage`. Falsifier: one.
4. **E3a** — every `one legal assignment that reaches it` clause carries
   `chosen for your blockers' material as well as for the life` and one of
   `every blocker in it survives` / `N of those blockers die`; and in records where the
   same window's B rows offer a `(you kill it, your blocker lives)` pairing for a blocker
   the clause names, the clause does not name that blocker in a chump pairing instead
   (seq 25's exact shape). Falsifier: one named chump beside an available kill-and-live for
   the SAME blocker against an attacker the header also blocks.
5. **E3b** — every header carrying `no block saves you` on the exact branch also carries
   `the assignment that lets in the LEAST damage is:`; 0 of those also read
   `one legal assignment that reaches it`. Falsifier: one lethal exact-branch header with
   no least-damage line.
6. **E4a** — 0 `their untapped blockers` tags place `GANG BLOCK:` after any per-blocker
   result; every tag carrying it opens `[their untapped blockers: GANG BLOCK:`, and 0 of
   those read `each result above is a LONE blocker only`. Falsifier: one of either.
7. **E4b** — every A-line tag listing two or more priced candidates carries
   `(N if all block)`, and that N is at or above the largest single price printed in the
   same tag; 0 such clauses appear on a tag with fewer than two priced candidates.
   Falsifier: one sum below its own worst single price, or one on a single-priced tag.

---

## What I did NOT verify

- **No live model probe was run.** Fixtures and PARSETEST are the whole gate; nothing here
  has been seen by a model, so every effect on behaviour is a prediction, not a measurement.
- **No corpus render was captured.** The three emitters were exercised only through their
  pure builders. In particular `lifeLoopProvenWin(opp)` — the gate that decides whether the
  E1 clauses print at all — needs a live board and is proven here only by inspection and by
  the wave-62 R6 cases that already pin its truth table; how many wave-63 windows it fires on
  is unmeasured.
- **The material pass is proven pure, not on a board.** `improveAssignmentMaterial` is pinned
  over synthetic `can`/`rank` matrices; `blockPairMaterialRank`'s 4/3/2/1 mapping onto real
  `MTGCardInstance` pairs is argued from `combatTradePreviewStats`'s own kill flags and is not
  exercised by a fixture (it needs live cards). If those flags are wrong for some pairing, the
  rank inherits the error rather than adding one.
- **The 2048-pairing rank bound is a guess at a safe cost, not a measurement.** Past it,
  material is simply not claimed; no board in the corpus was checked for crossing it.
- **E1's decline clause is strictly stricter than the ledger's wording.** It claims the
  decline is safe only at zero unblocked damage. If a future finding shows the mirror does
  NOT read some class of combat life loss, that clause under-claims — deliberately, and in
  the fail-closed direction.
- **E4b's sum counts the candidates the TAG lists, not every creature they control.** A body
  that could not legally block this attacker is out of both the list and the sum; a body that
  arrives before their block is in neither. The figure is a price for the listed set and says
  so ("if all block"), not a board maximum.
- **The collapsed form's sum is over all listed candidates while only the biggest and the
  priciest are SHOWN.** That is intended (it is the aggregate the collapse hides) but it means
  the reader cannot add the printed rows up to the printed sum on a collapsed tag.
- **No cross-lane interaction was tested.** Lanes AC, AD and AE touch adjacent emitters
  (AC owns E5's crack-back walk and E15's mana clarifier, AD the record and the hold row);
  the combined diff has not been built here.
- **`combatTradePreviewStats`'s attacker-seat blocking-trigger tail was added for symmetry**
  with E1's B-row half. E1's evidence is the blockers window; the attackers-window voicing is
  the same mechanism and the same board fact, but it has no corpus record of its own.
