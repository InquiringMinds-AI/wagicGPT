# Wave-65 lane AN — G6, G10 and the forced-sacrifice tail (verdicts that contradict the same screen)

Base: master `537070ac2`. Branch `w65-lane-AN`. Worktree `worktrees/lanes/w65-AN`.
One file changed: `projects/mtg/src/AIPlayerGPT.cpp`. Every wagic run memory-capped at 4 G with
swap off; the gate ran detached as `w65-AN-gate`. No cap kill. Every item is a render /
classification change — nothing here changes what the engine does with a card, an option or an
answer — so RED was produced the way wave-64 lane AG and wave-63 lane AB produced it (each
mechanism reverted to its wave-64 behaviour with the new cases already in place); see
**RED-on-base evidence**.

---

## What shipped

### G6a — the life-LOOP clause yields to the verdict printed on the same screen

`123v126` seq 48: 51 creatures, `ATTACK TOTAL ... KILLS them whatever they block` AND the
Sanguine Bond / Exquisite Blood `fatal to you, not a trade` clause on every unblockable row.
`ATTACK: none`, lost 43-0. Two authorities in one window, and the one that stopped the action was
the wrong one: a life loop is a reason to fear a long game, never a reason to decline a kill —
**the chain cannot fire after they are dead**.

The two clauses are now composed from ONE computation. `attackTotalLine` gained an out-param
(`outKillClaim`) set inside the exact branch that prints `; that KILLS them whatever they block`,
so a second "is this attack lethal" derivation that could drift from the printed one does not
exist. Both of the total's existing withholding branches keep withholding and therefore keep the
caution: an unpriced attack punisher (`#W61-R` C1b) and the loop's own no-claim branch
(`#W62-X` D2) each report `false`.

`lifeLoopAttackerRowTag(reachable, attackProvenLethal)` (pure) is the composer. On a proven-lethal
screen it prints `(their life LOOP is in play, but LETHAL COMES FIRST: ... the loop cannot fire
after they are dead - the chain is a reason to FINISH this combat, not a reason to hold back)`;
otherwise it prints the wave-64 caution byte for byte.

**Ordering, and why the totals block moved.** The clause has to read a line composed BELOW it, so
the attackers window now builds its totals into their own buffer (`totalsTail`) before the rows
are joined, and each row reserves the byte offset where its clause goes (`aRowLoopAt`). The
rendered order is unchanged — rows, then the blocker-count line, then ATTACK TOTAL — and the
TRANSLOG row is built after the insertion, so the logged option is still byte-identical to the
rendered one.

### G6b — the clause prints only where a branch of the chain can be entered

`146 vs126` seq 29: the clause on all 3 attacker rows against a **creatureless** opponent, where
the same prompt says every attacker is unblockable (neither loop branch reachable:
mtg.txt:37967/:99004). Two gates, both from predicates this window already holds:

* **per row** — with nothing of theirs able to block THIS attacker (`noLegalBlockForThisRow`, the
  same predicate the guaranteed-damage floor and the blocker-count line are built from),
  declaring it gains them no life off a block and the row's own decision enters the chain
  nowhere. No clause on that row.
* **per board** — the gate is now `lifeLoopProvenWin` (the `#W62-AA` R6 rule), not
  `playerHasLifeLoop`. A clause that says "chains without limit" must not print over a board where
  the chain provably cannot run, and this is the gate this same window's E1 price tails and the
  LOOP SCOPE paragraph already use, so the three can no longer disagree. Applied to the blockers
  window's copy of the clause too, for that reason; that window has no ATTACK TOTAL (the attackers
  there are theirs) and its loss branch is reachable by construction, so it prints the caution face
  only.

Nothing else is withdrawn: the window's LOOP SCOPE paragraph, the loop banner and the converter
situation line are untouched, so the board fact is still on the screen in every case.

### G6c — the per-row converter arithmetic consults the loop detector

`162 vs126` seq 13: `{their converter:}` priced a body `life 21 -> 17` six lines under the LOOP
SCOPE paragraph that says ANY nonzero payment on a tag above is fatal. The seat cast it and died
from 21. A converter plus a mirror is not a SIZED price at all — the first point they gain
re-enters the chain — so `theirConverterBodyTag` takes `theirLoopClosed` (`lifeLoopProvenWin` of
the opponent, the paragraph's own predicate) and on a closed loop prints the loop verdict and **no
resulting-life figure**: `... and BOTH halves of their life LOOP are in play, so NO figure is given
for what this body costs you ... ANY nonzero payment here is fatal, whatever your life total is`.
The facts the row is built from (toughness, converter names, count) are kept; only the arithmetic
that assumed a bounded board is withdrawn. With the loop open the row is byte-identical to wave 64.

### G10a — the material claim names the search it is the maximum of

`152v146` seq 13/27 printed `no better material reaches that life figure` one line above a
`GANG BLOCK:` clause reaching the same life with a group that also kills the attacker; 0 of 10
suggestions in that corpus ever named two blockers on one attacker. The search behind the claim is
`assignableRemainderDamage`'s bipartite matching — one blocker per attacker — so the maximum is
real but scoped, and the clause now says so: `... among LONE blocks (one blocker per attacker is
all this search covers; a GANG BLOCK of two or more of yours on one attacker was not searched and
can do better - any "GANG BLOCK:" clause below prices one)`. The other two heads make no optimality
claim and are unchanged byte for byte.

### G10b — a committed blocker is not a free one

`126v152` seq 9, after blockers were declared: `AIPlayerGPT.cpp:21480` filtered candidates with
`canBlock()`, which is the SOLO gate (untapped, no can't-block) and says nothing about whether the
body is already standing in front of something. The committed Perimeter Captain was handed to the
matching a second time, its damage was subtracted twice, and the assignment the header NAMED could
not be declared at all. A creature blocks one attacker (CR 509.1a), so a body with a `defenser` is
out of the search — and what it is already doing is FOLDED rather than dropped: its certain
blocking-trigger life and its lifelink both enter the same projection, since the attacker it
blocks is already out of `declared` and out of `inDamage`.

### G10c — the best case folds the lifelink the same block gifts

`152` s12: the best-case life ignored the lifelink of the blockers in the very assignment it
names — the identical omission F8b fixed for blocking triggers one wave earlier; both resolve
inside this combat and both move the figure this line is about. `blockerLifelinkGain` (pure) is
the term, and it exists only where the damage is actually dealt: a blocker with no first strike
facing a first-striker that can kill it outright (deathtouch makes 1 lethal) deals none and gains
none. Every other way a block can be stopped — prevention, a pump, removal in response — can only
make it smaller, so the term under-states, which is the safe direction for a survival number.
`incomingCombatLine` keeps it as its own argument so the wording names both halves (`before your
blocking triggers and your blockers' lifelink, N after the G those blocks gain you`; the floor
branch in its own register), and the same sum feeds the `no block saves you` badge and the lethal
screen — a lifelink gift that lifts the best case above 0 withdraws a death claim that would
otherwise be false. With no lifelink every string is byte-identical to wave 64.

### MED — the forced-sacrifice tie-break faces whoever is paid

`buildForcedSacrificeAsk` appended `the one that pays the least` unconditionally, including on the
`gain == 2` branch where the toughness is life **YOU** gain (`126 vs123` seq 10) — the ask told the
seat to give itself the least. The sentence now branches with the beneficiary the header already
names: `pays the least` (they are paid), `GAINS YOU THE MOST` (you are paid), and nothing at all
where the script named no beneficiary, because then no row differs in a way this ask can rank. The
`gain == 1` string is byte-identical to wave 64.

### Re-pinned PARSETEST expectation (deliberate, not drift)

One `#W64-AG F8a` head-string equality pinned the UNQUALIFIED `no better material reaches that
life figure` — which is deck152 HIGH-1's defect exactly. It was replaced in place with a
`#W65-AN` comment saying why; F8a's own claim (no material head over a chump) is unchanged and
still pinned. Nothing was deleted.

---

## Gate

Clean rebuild (`rm -f bin/wagic && make -f Makefile.sdl -j4`), detached unit `w65-AN-gate`,
`MemoryMax=4G` / `MemorySwapMax=0` on the unit and on every interactive run. No cap kill.

| | base (brief, `537070ac2` / `dab6a6cba`) | this lane |
|---|---|---|
| PARSETEST | 4536 passed, 0 failed | **4573 passed, 0 failed** (+37) |
| suite, default threads | 1269, 2 known concurrency | **1269 tests, 2 failed**, `==Test Failed !==` **2**, `==Test timed out` **0**, 67 AI / 0 failed |
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1269, 0 failed | **1269 tests, 0 failed**, `==Test Failed !==` **0**, `==Test timed out` **0**, 67 AI / 0 failed |

The two default-threads failures are the brief's own known concurrency set, named from the log:
`lifeline` and `merrow_reejerey`. Both green at THREADS=1, which is ground truth.
**0 new failures.** `git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.

### RED-on-base evidence

Nothing here changes engine BEHAVIOUR, so no suite fixture can be RED and none is claimed (the
standing of wave-64 lane AG and wave-63 lane AB). RED was produced by reverting every mechanism to
its wave-64 behaviour with the new cases already in place and rebuilding: `lifeLoopAttackerRowTag`
returning the one wave-64 face at entry, `attackTotalLine` never reporting its kill claim,
`theirConverterBodyTag` ignoring `theirLoopClosed`, `blockAssignmentClause` restored to the
unqualified material head, `incomingCombatLine`'s lifelink term forced to 0, `blockerLifelinkGain`
without its damage-dealt test, and `buildForcedSacrificeAsk`'s tie-break unconditional.

Result: **4555 passed, 18 failed** — 7 G6 (3 attacker-row / total, 2 converter, plus the
reachability and the loop-verdict pins), 8 G10, 2 MED, plus the deliberately re-pinned
`#W64-AG F8a` head case. The other 20 new cases are NEGATIVE / MUST-NOT-MATCH / echo-shape pins
that pass on base too, which is what they are for. The tree was restored from a byte copy taken
before the revert and rebuilt clean before the gate.

---

## Predictions (falsifiable, for the wave-65 corpus)

1. **G6a** — 0 attacker windows print `fatal to you, not a trade` in the same render as an
   `ATTACK TOTAL ... KILLS them whatever they block`. Every such co-occurrence instead reads
   `LETHAL COMES FIRST`. Falsifier: one window carrying both the wave-64 caution and a printed
   kill claim (the `123v126` seq 48 shape), or one `LETHAL COMES FIRST` on a screen whose ATTACK
   TOTAL makes no kill claim.
2. **G6b** — 0 attacker rows whose own tag says no untapped creature of theirs may legally block
   it carry any life-LOOP clause, and 0 windows print the clause where `lifeLoopProvenWin` is
   false. Falsifier: one clause on an unblockable row (the `146 vs126` seq 29 shape), or one on a
   board where the chain is provably blocked (Teferi's Protection / `cantchangelife`).
3. **G6c** — every `{their converter:` row rendered on a board where both halves of the
   opponent's loop are in play carries `ANY nonzero payment here is fatal` and **no** `life N -> M`
   figure; every one on an open board is byte-identical to wave 64. Falsifier: one closed-loop row
   printing a resulting life (the `162 vs126` seq 13 shape).
4. **G10a** — every `no better material reaches that life figure` in the corpus is followed by
   `among LONE blocks`; 0 appear unqualified. Falsifier: one unqualified instance. Secondary (the
   behaviour, not a render fact): a reply that names two blockers on one attacker appears at least
   once where the window carries both this clause and a `GANG BLOCK:` verdict.
5. **G10b** — 0 named assignments contain a blocker the same window shows as already blocking, and
   0 `best case` figures in an after-blockers-declared window subtract a committed blocker's
   attacker twice. Falsifier: one named assignment whose blocker is already committed (the
   `126v152` seq 9 shape).
6. **G10c** — every best-case figure in a window whose named assignment includes a lifelink
   blocker that deals damage carries the `and your blockers' lifelink` pair, and 0 such windows
   carry `no block saves you` when the after-figure is above 0. Falsifier: one of either.
7. **MED** — 0 `FORCED SACRIFICE` asks whose price is life YOU gain say `pays the least`; every one
   says `GAINS YOU THE MOST`. Falsifier: one inverted ask (the `126 vs123` seq 10 shape).

---

## What I did NOT verify

- **No live model probe was run and no corpus render was captured.** Fixtures and PARSETEST are
  the whole gate; nothing here has been seen by a model, so every effect on behaviour is a
  prediction, not a measurement. How many wave-65 windows each emitter fires on is unmeasured.
- **G10b (the committed-blocker exclusion) is the one item with no pure surface.** It is a
  two-line guard on the engine's own `defenser` field inside the blockers-header caller, so it
  cannot be pinned by PARSETEST and no suite fixture can read a prompt. It is proven by reading
  (`canBlock()` at `MTGCardInstance.cpp:1316` tests tapped / can't-block / in-play only) and by
  CR 509.1a; the FOLD it ships (committed triggers and lifelink) reuses the same two pure helpers
  the assignment path uses, which ARE pinned. No rendered window was captured showing the
  before/after, and that is the weakest evidence in this lane.
- **The `lethalProven` yield is only as good as the ATTACK TOTAL's own kill claim.** It reports
  what that line printed, so where the total over-claims (a proven floor that a trick or an
  untapped effect overturns) the row clause inherits that error rather than adding one. The two
  withholding branches the total already has were checked; a third source of over-claim was not
  looked for.
- **The per-row reachability gate is about THIS row.** A row nothing of theirs can block prints no
  clause even though another attacker in the same window may be blockable and may feed the chain.
  That is deliberate — the tag is a price on the row's own decision — and the window-level LOOP
  SCOPE paragraph still carries the board fact, but a seat reading only the rows on such a board
  sees the loop named once fewer than in wave 64.
- **`blockerLifelinkGain` models the one way a blocker is stopped from dealing damage that this
  window can see** (first strike, with deathtouch as the 1-damage case). Prevention shields, a
  pump on either side, protection, and removal in response are not modelled; all of them can only
  make the real gain smaller, so the term over-states the seat's life in none of them — but a
  board where the blocker is removed at instant speed will read a best case it never reaches.
- **The lifelink term is over the blockers the assignment NAMES plus the ones already committed.**
  A lifelink body the seat could add but the matching did not pick contributes nothing, so the
  figure is not the maximum reachable life on a lifelink-heavy board.
- **G6c reads only `lifeLoopProvenWin`.** A converter of theirs on a board where the mirror is in
  a zone the seat has merely SEEN (the `#W60-N` pending-loop banner's case) still prints the
  arithmetic; the row moves only once the pair is actually on the battlefield.
- **The MED tie-break was changed, not the row tags.** `forcedSacrificeRowTag` still states the
  gain per row exactly as before, and nothing here folds a life-to-damage converter into that
  gain — the known-bugs note that Sanguine Bond can turn the seat's own gain into damage is not
  addressed and should stay on the ledger.
- **No cross-lane interaction was tested.** Lane AL owns the crack-back and pass-row pricing and
  lane AM the attackers-window walker path; the combined diff has not been built here. The totals
  block in `buildAttackersAsk` MOVED (above the row join, into `totalsTail`) — a lane touching the
  same region will see that as a conflict, and the merge must keep the buffer, not the old
  in-place emission.
