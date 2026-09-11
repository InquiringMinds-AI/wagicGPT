# Wave-77 lane CS — combat and sacrifice truth (R3, R4, R6, R7, R10)

Base: master `49c19f413` (worktree `worktrees/lanes/w77-CS`, branch `w77-lane-CS`).
Corpus for every repro: `matchups-20260910-141526-final` (21 games, single base
binary `archives/wagic-d372a1153-w76step1`).

Gate: **PARSETEST 6168 -> 6234, 0 failed** (+66 cases). **Suite 1291 tests
(0 failed) + 76 AI tests (0 failed)**, THREADS=1, detached unit `w77-CS-suite`,
BOTH failure strings counted (`==Test Failed !==` 0, `==Test timed out` 0), 0
known flakes hit. `check-reply-instructions.py`: OK (1 source, 37 guides).
`git diff | grep -c U+FFFD` = **0**. Every wagic run under the 4G memory cap;
no run was killed under it.

Oracle checks are against the engine's own script, never a card list: Sanguine
Bond `@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent` (read
through `lifeToDamageConverterTake`, whose PARSETEST case quotes the primitive
line verbatim); Devour Flesh `target=player` /
`auto=ability$!name(sacrifice) notaTarget(creature|mybattlefield)
dynamicability<!mytgt toughnesslifegain targetcontroller!> sacrifice!$
targetedplayer` / `text=Target player sacrifices a creature, then gains life
equal to that creature's toughness.` (mtg.txt:29133-29136) — the TARGETED
player both sacrifices and gains, which is what makes the converter that prices
the event the one on the TARGET's battlefield; Shield Sphere `{0}` (0/6)
`Defender -- Whenever Shield Sphere blocks, put a -0/-1 counter on it.`

---

## R3 HIGH — one clause shape, and it names the EVENT

### Repro

**(a) the loop branch read as a MANA-COST statement.** `162v126` deck162
**seq 20** (t14, Main 1). Row 3, verbatim:

```
3. Cast Shield Sphere {0} (0/6) ... {their converter: this body has toughness 6
and they control 1 life-to-damage converter (Sanguine Bond) - and BOTH halves of
their life LOOP are in play, so NO figure is given for what this body costs you:
the first life any effect of theirs gains off it re-enters the chain, which does
not stop until you are at 0. ANY nonzero payment here is fatal, whatever your
life total is}
```

The model's own reasoning channel, verbatim:

```
One detail: "LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine
Bond... ANY nonzero payment here is fatal". Shield Sphere costs 0, so it's fine.
...
- Life Loop: Costs 0, safe.
```

It cast the body. The clause never named WHAT is paid, so "payment" resolved to
the only number on the row — `{0}`.

**(b) the seat's OWN converter is absent.** `126v123` deck126 **seq 23**: a
forced sacrifice under the seat's own Sanguine Bond, rows reading
`[you SACRIFICE this; you gain 4 life (its toughness)]` and
`[... you gain 1 life (its toughness)]`. 4 life gained is 4 off THEM; no row,
and no header, said so.

**(c) the edict's OPPONENT branch is unpriced, on BOTH surfaces.**
`123v126` deck123 **seq 70** (cast row): `Cast Devour Flesh {1}{b} {right now:
they control 2 creatures - they choose which one; YOU control 0 creatures -
targeting yourself does nothing}` — the SELF branch priced in full, the
OPPONENT branch stopped at "they choose which one", with their Sanguine Bond in
the frame. **seq 71** (the target ask one record later) offered
`1. The opponent (player, life 20)` / `2. Yourself (player, life 20)` and
nothing else — two bare names for a spell whose whole content is a sacrifice
and a life gain. The seat took the opponent, handed a Bond opponent 4 life and
took 4 off itself.

### RED on base

* `theirConverterBodyTag(6, {"Sanguine Bond"}, 18, true)` on the seeded tree
  contains `ANY nonzero payment here is fatal` — the seq-20 string, and the
  wave-65 PARSETEST case **asserted** it.
* `forcedSacrificeRowTag(2, 4, NULL, "Sanguine Bond", 4)` on base is
  `" [you SACRIFICE this; you gain 4 life (its toughness)]"` — the gain-2 branch
  had no converter arm at all, and the wave-76 case asserted that silence
  (`#W76-CO Q6 MUST-NOT-MATCH the gain-2 branch ... is untouched`).
* `edictClause` had no `maxToughness` parameter, so the N>1 gain branch is not
  expressible on base; `w77EdictPlayerTargetTag` does not exist there, and every
  case in that block is red by construction.

### Fix

`w77ConverterTakeClause(theyGain, convName, convTakes, upTo)` — **one** clause,
used by every life-gain-on-sacrifice surface. It states a GAIN and a LOSS and
never the words "cost" or "pay" (pinned): `- and with their <conv> that is N off
YOUR life` / `- and with your <conv> that is N off THEIRS`.

* (a) the loop branch keeps #W65-AN (G6)'s rule (loop verdict, no resulting-life
  figure) and replaces the cost-shaped sentence with the EVENT: `THE EVENT BEING
  PRICED IS NOT THIS ROW'S MANA COST: it is the life THEY gain if any effect of
  theirs sacrifices or destroys this body and gains them life equal to its
  toughness - they gain 6, and the converter named above turns that into 6 off
  YOUR life ... A {0} body is exactly as fatal here as a {6} one; what is paid is
  LIFE, later, off this toughness`. The figure is the NON-loop branch's own
  arithmetic (`toughness * converters`), so the two faces cannot disagree.
* (b) `forcedSacrificeRowTag`'s gain-2 branch takes the same clause, and the CALL
  SITE scans the **gainer's** battlefield (`sacGainer = sacGain==1 ? opponent()
  : this`) through the same `lifeToDamageConverterTake` + `w76PlayerCanGainLife`
  gate the gain-1 branch already used.
* (c) `edictFloorScan` gains a `maxOut` ceiling (the same walk the floor comes
  from). `edictClause`'s N>1 gain branch prices the range: `they gain that
  creature's toughness: THEIR pick, so between 1 and 4 across their creatures`
  + the clause at `up to`. `edictSelfClause` takes the seat's own converter on
  both its arms. `w77EdictPlayerTargetTag` gives the TARGET rows the same shape
  at 0 / 1 / N bodies, wired at the target seam through
  `w77EdictPlayerTargetTagFor`, which recognises the shape off the script
  (`toughnesslifegain` + `sacrifice` + `targetedplayer`) and scans the TARGET's
  own battlefield for the converter — because the targeted player is the one who
  gains.

### GREEN

+27 cases. The seq-20 row with the misread string gone and the event named; the
two-converter plural; gain-2 with the seat's own Bond, byte-exact; the seq-70
range clause; both seq-71 target rows byte-exact; six MUST-NOT-MATCH silences
(no converter, unknowable take, gain 0, 0 bodies, N==1 determined victim, the
seat-gains direction); and the full **key-stability pin set** (hold-latch,
hold-check built as `mLastMenuRows` builds it, option-set, ask/async-slot),
plus the MUST-NOT-MATCH that no LIFE-derived byte enters the `[...]` bracket
(the wave-49 CG shape).

Two wave-65/76 assertions are **SUPERSEDED in place**, each marked with why:
the `ANY nonzero payment` wording pin, and the gain-2-is-bare pin.

### Prediction

Next corpus: **0** `{their converter:` clauses contain `ANY nonzero payment`
(wave 76: every loop-branch render), and **0** reasoning bodies on a
`{their converter:` window contain a sentence matching `cost[s]? 0.*safe`
(wave 76: 1, and it lost the game). Every
`[you SACRIFICE this; you gain N life (its toughness)]` row rendered while a
converter of the SEAT'S is on the battlefield carries `and with your <name>
that is N off THEIRS` (wave 76: 0 of 2). Every `Cast <edict>` row at 2+ of
their bodies carries `THEIR pick, so`, and both player rows of a
toughness-lifegain edict target ask carry `{right now: ` (wave 76: 0 of 2).
**Pin for the negative clauses**: rows rendered with NO converter stay
byte-identical to wave 76 — the cross-tab is the row present x no
`LIFE-TO-DAMAGE CONVERTER on the battlefield` in the prompt x the clause absent;
and `you would be at` must stay absent from every `[you SACRIFICE this` bracket.
Falsifier for (c): a `{right now: ` target row whose range does not bracket the
toughness the opponent actually hands over.

---

## R4 HIGH — the AT BEST figure folds the metered lifegain

### Repro

`126v146` deck126 **seq 53** (4 life, 6 attackers). Two lines, six apart:

```
INCOMING THIS COMBAT: 6 attackers, 14 unblocked damage - you would be at -10;
this KILLS you (your creatures can legally block at most 2 of those 6 attackers
at once ...) - at least 9 of that lands whatever you block ...: you would be at
-5 AT BEST (no assignment of your blockers does better); no block saves you
BLOCKING THIS COMBAT: each of your 2 blockers that blocks gains you 0 and may
gain 2 more - up to 4 life for you
```

True best is **-1**. The seat answered `BLOCKS: none`.

### RED on base

`incomingCombatLine(6, 14, 4, true, 0, 0, 9, false, 2, 6, "", false, 0, 0)` —
the exact seq-53 numbers — prints `you would be at -5 AT BEST (no assignment of
your blockers does better)` and contains no `OPTIONAL gain` string; the function
has no may-gain parameter on base, so every new case is red there. The cause is
in the accumulator: both walks add only the CERTAIN half (`if (bs > 0)
blockTriggerGain += bs;`) and drop `bm`.

### Fix

`blockMayGain` is accumulated over the same two walks (`match` and the
already-declared blockers), passed to `incomingCombatLine`, and stated as its
own second figure on BOTH branches: `-1 AT BEST if you also take every OPTIONAL
gain the BLOCKING THIS COMBAT line below meters (4 more; those are "may" gains
and yours to take)`. The CERTAIN figure still prints first and is never
displaced. The `no block saves you` badge moves to the fully-folded value —
that is the fail-closed direction (a death claim is withdrawn when it cannot be
proven), while `blockMayAvert` / `you SURVIVE` stay on the CERTAIN figure, so no
survival is ever founded on an optional gain.

### GREEN

+8 cases: the seq-53 shape with both numbers; the badge still true at -1; the
badge withdrawn when the metered gain carries past 0, with `you SURVIVE` still
absent there; byte-identity at may-gain 0 and at a negative; the exact branch;
and the MUST-NOT-MATCH that under a PROVEN opponent loop no best-case figure
exists at all, so none is folded (#W63-AB E1 unmoved).

### Prediction

Next corpus: **0** prompts carry `AT BEST (no assignment of your blockers does
better)` while a `BLOCKING THIS COMBAT:` line on the same prompt says `may gain`
and the AT BEST figure excludes it (wave 76: 1 measured by hand at deck126 seq
53; the machine cross-tab is `may gain` present x `OPTIONAL gain` absent, which
must be 0). **Pin for the negative clause**: windows whose blockers meter only
CERTAIN gain must render the wave-65 line byte for byte — the cross-tab is
`AT BEST once the` present x `OPTIONAL gain` absent, and that count must NOT
fall to zero (a fall would mean the certain half was replaced rather than joined).

---

## R6 MED — the cover family reaches three more surfaces

### Repro

**(a) targeted removal, `130v146` deck130 seq 14** — `Cast Hammer of Bogardan
{kills whichever you target: THEIRS - Barrowin, Goblin}` under an 8-point
crack-back, no cover clause, while sweeps and edicts on the same menus carry
one. This is #W76-CO (Q5)'s own named residual ("the Hammer of Bogardan shape is
deliberately NOT covered ... That is a real residual, not a closure"). deck130
lost that game.
**(b) X sub-menu, engine-seat MED-2 (`123v130` seq 22)** — the parent cast row
carries `{crack-back cover: ...}`; the ANNOUNCE_X rows price their kills and
carry none.
**(c) the attackers menu, `162v152` deck162 seq 14** — the full
`CRACK-BACK COST OF ATTACKING:` paragraph (17 damage, puts you at -12) is prose
end to end; cast rows elsewhere print `you cover N of M`. The model invented the
number.

### RED on base

`w77TargetedRemovalCoverTag`, `w77StayHomeCoveredPower` and
`w77StayHomeCoverTag` do not exist on the seeded tree — every case in those
blocks is red by construction there; (b)'s red is that the ANNOUNCE_X assembly
contains no `crackBackKillRowTag` call at all.

### Fix

* (a) a CONDITIONAL clause, which is what a target the seat has not picked yet
  allows: each named victim priced separately off the same
  `crackBackBodyContribution` walk the CRACK-BACK line is built from, then `The
  most any ONE of them takes off is K, leaving L`. Q5 left the shape out because
  no row-level claim was honest; the answer is to make the claim conditional,
  not to leave the row silent (the trust doctrine's no-silent-omission rule).
  Gated through the same `crackBackScreenTotal` as every sibling.
* (b) the SAME helper the parent row uses (`crackBackKillRowTag`), once per X,
  fed from the same `XVictimSurvey` the kill lists are built from and keyed on
  `XDamVictim::inst` (the identity handle Q5 added), so no third number exists.
* (c) `w77StayHomeCoveredPower` — the cover family's own greedy (attackers by
  power, each spending as many DISTINCT bodies as `minBlockersRequired()`
  demands, no body counted twice) over the seat's untapped bodies — and
  `w77StayHomeCoverTag` appended to the CRACK-BACK COST OF ATTACKING paragraph.
  Both branches obey #W65-AL (G4).

### GREEN

+20 cases: the seq-14 two-victim clause byte-checked; the FLOOR branch with no
survival verdict; four silences (no victim in the total, no line, no bodies,
unknown life) and the walks-disagree silence; (b)'s identity to the parent
helper plus its zero-removal silence; (c)'s one-body/menace/zero arithmetic and
the seq-14 clause byte-exact; and the **key-stability pin set** for the
row-borne clause (hold-latch, hold-check, option-set, ask/async slot), plus the
pin that the stay-home clause is prompt prose and enters no option key at all.

### Prediction

Next corpus: every cast row carrying `{kills whichever you target: THEIRS -` on
a prompt with a `CRACK-BACK NEXT TURN:` line also carries `{crack-back cover:`
(wave 76: 0 of 13 such rows); every ANNOUNCE_X menu rendered under a crack-back
line has at least one row carrying `{crack-back cover:` (wave 76: 0); and every
attackers window printing `CRACK-BACK COST OF ATTACKING` also prints
`{crack-back cover, STAY HOME:` (wave 76: 0).
**Pin for the negative clauses**: (a) the count of `{crack-back cover:` clauses
containing `This row removes ONE body` on rows whose named victims are all
OUTSIDE the crack-back total must be **0**; (b) X rows whose X removes no
crack-back body must carry no clause; (c) `STAY HOME` must never appear on a
window with no `CRACK-BACK NEXT TURN:` line. A clause on a row that removes
nothing falsifies the fix.

---

## R7 MED — the attack aggregate counts LEGAL blockers

### Repro

`123v152` deck123 **seq 104**: `Their 3 untapped blockers ...` where only Elite
Spellbinder can block 11 fliers, and the guaranteed floor reads 16 where it is
20. `152v123` deck152 **seq 31**: `You ALSO already control 2 untapped creatures
that can block on their turn ... on their own they cover 2 of 18` — priced at
ONE body's worth in the same sentence, and seq 32's blockers window offers
exactly ONE B-row (Briarbridge Tracker cannot block a flier).

### RED on base

The count walk is `if (c && c->isCreature() && c->canBlock()) blockerCount++` —
the parameterless SOLO gate (untapped, no can't-block), which says nothing about
whether the body can block anything ON OFFER; `crackBackCoverFacts` sets
`*existingOut = standing.size()` before any legality is consulted, while the
per-attacker `existingBlockers` lists it builds ARE filtered. The wave-60
PARSETEST case asserted the unscoped wording (`Their 3 untapped blockers can
cover ...`) and is superseded in place.

### Fix

The same distinction #W61-V (R8) already drew one line below for the blocking-
life ceiling, on the same pairwise map (`couldBlockIfItAttacked`, because nothing
is an attacker yet at declare-attackers): a body with no legal block among the
listed attackers is not a blocker of this attack. `blockerCount` — which feeds
BOTH the header line and `w76BlockableCoveredPower`'s knapsack capacity — counts
only bodies that can legally block at least one listed attacker, and the
sentence names that scope. `crackBackCoverFacts` derives `*existingOut` from the
union of the `existingBlockers` indices it just built, so the count and the
arithmetic beside it come from one walk.

### GREEN

+4 cases: the scoped wording on both nouns; and the MECHANISM pinned as
arithmetic — three bodies cover 20 of the seq-104 board, one legal blocker
covers 9, and filtering the capacity can only RAISE the guaranteed floor. Two
wave-60 cases updated for the new scope clause.

### Prediction

Next corpus: **0** `ATTACK TOTAL` lines assert full cover on a window whose
A-rows carry `[no creature they control can block this attacker]`; and **0**
`{crack-back cover:` clauses say `You ALSO already control N untapped creatures`
with an N larger than the number of distinct bodies its own per-attacker lists
name. **Pin for the negative clause** (no over-filtering): windows where every
untapped body of theirs can block something must render the same count as wave
76 — the cross-tab is `untapped blocker` present x no evasion keyword
(flying/shadow/protection/menace) among the A-rows x the count unchanged; that
population must not shrink. Falsifier: a floor that rises on a board with no
evasion at all.

---

## R10 MED — the X window echoes the plan, and its example is neutral

### Repro

`130v162` deck130 **seq 60**: an ANNOUNCE_X window answered `CHOICE: 1 (X = 1)`
on a row reading `kills THEIRS: none; YOURS: Dwarven Blastminer` — the format
example, copied. 3 of 3 deck130 X windows carried NO plan echo, against 128 of
155 asks elsewhere.

### RED on base

Both plan-render gates read `!pregame && !mInAnnounceXAsk && !mCurrentPlan.empty()`
(the #W48 D9 suppression), so the echo is structurally impossible on this seam;
`askExemplar` is called unconditionally and writes the example out from row 1,
and `exemplarSentence` has only a row-0 and a row-N face.

### Fix

The echo renders on ANNOUNCE_X like every seam — invariant 000 (e) makes the
PLAN a carried SEQUENCE each later window executes the next step of, and a seam
that hides it is a seam with no conditioning at all. The wave-48 hazard (the
plan's X was fixed one screen earlier, before the per-X kill lists existed) is
real and is answered by NAMING the scope beside the echo rather than deleting a
true token: `kAnnounceXPlanScopeNote`. The worked example on this one seam
becomes the neutral template `CHOICE: <n> (X = <n>)` with its own
`exemplarRow = -1` face — a FORMAT example that makes no claim about any row of
the ladder. Every other seam keeps the wave-66 wording byte for byte, and the
wave-66 dead-menu face (row 0) is untouched.

### GREEN

+5 cases: the scope note's two load-bearing clauses; the neutral template and
its "not a recommendation" face; and two MUST-NOT-MATCH — the row-based example
on every other seam, and the row-0 dead-menu face.

### Prediction

Next corpus: `plan_echo` present on ANNOUNCE_X records at the same rate as the
rest of the seat (wave 76: 0 of 3 vs 128 of 155), and **0** ANNOUNCE_X replies
whose chosen X equals 1 while the prompt's example named row 1 (the example no
longer names a row). **Pin for the negative clause**: non-X asks must keep
`written out from row N of this list` — its corpus-wide count must NOT fall
(a fall means the neutral branch leaked off its seam).

---

## Weakest evidence

1. **Nothing in this lane was run against a live board.** Every case is
   PARSETEST over pure helpers plus a compile-verified call site. The walks that
   feed them — the gainer-side converter scan, `edictFloorScan`'s new ceiling,
   the `couldBlockIfItAttacked` filter, the per-X `XDamVictim::inst` match, the
   stay-home body walk — are hypotheses until a corpus renders one. The R6(b)
   wiring is the weakest of these: its cited record (`123v130` seq 22) could
   not be located in the harvested corpus at that seq, so the shape is taken
   from engine-seat MED-2's description and from the parent row's own clause,
   not from a prompt I read.
2. **R3(c)'s range is a claim about a choice neither player has made.** "THEIR
   pick, so between MIN and MAX" is true of the board, but the opponent picks
   for reasons this engine cannot see (an engine body, a tutor target), so the
   MAX is an exposure ceiling, not a forecast. If they routinely hand over the
   floor, the clause will read as alarmist and the next wave should measure
   actual-vs-range before widening it further.
3. **R7's filter may be too aggressive on a board that CHANGES before their
   turn.** `couldBlockIfItAttacked` is evaluated now; a body that cannot block a
   flier today can block it after a Giant Growth, a removal, or their own
   creature losing flying. The count is honestly scoped ("that can legally block
   at least one of these attackers"), but the knapsack capacity it feeds is now
   a present-tense number used for a future combat. The direction is safe (it
   raises the floor, never lowers it), and that is the only defence offered.
4. **R4's `no block saves you` now depends on an OPTIONAL gain.** The badge is
   withdrawn whenever the metered "may" gains could carry the seat past 0 — but
   a "may" is only reachable if the seat actually takes it, and nothing in this
   change makes it take it. A seat that declines its own may-gain will die on a
   window whose death badge was withdrawn. The alternative (keeping a death
   claim that the seat's own choice can falsify) is worse, and the certain
   figure still prints first, but this is a real trade and not a strict
   improvement.
5. **The R10 measure is not on the record.** Nothing logs whether a given record
   was an ANNOUNCE_X ask, so the next wave's census must re-derive it from the
   prompt text (`option N announces X = N` / the `X = ` row anchors) exactly as
   this one did. Logging the seam kind per record would make the prediction
   directly checkable instead of re-derived — the same gap #W76-CO's weakest
   evidence #4 flagged for the crack-back predicate.
6. **Two superseded wave-65/76 assertions were rewritten, not deleted.** If
   either rewrite is wrong, the pin that would have caught it is gone. Both are
   marked SUPERSEDED in place with the reason, so the next reader can see what
   was traded.
