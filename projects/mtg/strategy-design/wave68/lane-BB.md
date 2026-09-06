# Wave-68 lane BB — J4, J5, J9 (numbers the two surfaces disagree on)

Base: master `4837e76a1`. Branch `w68-lane-BB`, worktree `worktrees/lanes/w68-BB`.
Files changed: `projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/include/AIPlayerGPT.h`.
Every wagic run memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`); no kill under the cap on any
run. The gate ran detached (`w68-BB-gate`) with a foreground until-loop wait. Every edit was a
python `rb`/`wb` byte splice; `git diff | /usr/bin/grep -c $'\357\277\275'` = **0**. The binary was
freshness-checked (`ls -la bin/wagic`) after every make, and no build was ever chained behind a
counting grep. Every comment tagged `#W68-BB (<item>)`.

Nothing volatile entered `mPromptTail` or an ask key: no counter, clock or running total was added.
The two new key-affecting terms are both functions of the BOARD (a lethal-stack verdict printed on
the decline rows; a crack-back verdict WORD in the hold latch), so they can only change when the
board changes — and both can only ADD re-opens, never remove a window.

---

## What shipped

### J4a — one ceiling function, both surfaces (deck125 HIGH-1/2; lane AW's claim falsified)

Lane AW wrote that its three surfaces "cannot name different values". The corpus falsified it.
`125v130` s90 (t43), verbatim: the ANNOUNCE_X header said

> `3 draws are already owed that you cannot decline (your next draw step ... plus Staff of Nin's
> upkeep draw, plus Staff of Nin's upkeep draw) ... X=1 is the largest listed X the library pays for`

and option 6 said

> `X = 4 {X pricing: X=4 - you gain 4 life and draw 4 cards} {library: this draws 4 of your 4
> library cards - 0 left}`

Both statements are TRUE and they answer different questions. `xLibraryRowClause`'s third argument
was `stackPendingDrawsFor` — the STACK term alone — while the header and `xLibraryCeilingX` are
built from the whole reserve (draw step + stack + `@each my upkeep ... draw:N` permanents). The two
Staff of Nin draws live on the battlefield, not the stack, so the row's term was 0 and the clause
said nothing. That is also why `; M more pending` rendered **0 times in the whole corpus**: the
only term that could print it was the one that is almost never nonzero. The seat took option 6 and
emptied its library with 3 undeclinable draws owed (it survived on an Emrakul reshuffle).

`xLibraryRowClause(cards, library, owedDraws)` now takes the SAME reserve, and states whether the
remainder pays it:

* `{... - 0 left; 3 draws are already owed that you cannot decline and only 0 would be left: you
  would be asked to draw from an EMPTY library and LOSE the game as that draw is attempted}`
* `{... - 3 left; 3 draws are already owed that you cannot decline, and the 3 left still pays them}`

The caller (`xLifeDrawRowAnnotations` via the X-menu annotation builder) passes `rowReserve`, not
`rowStack`. Nothing is capped and no X is removed: every row still prints, with the number that
decides it. With nothing owed (`owedDraws == 0`) the clause is wave 67's byte for byte.

**The one-function claim is now an identity, pinned in PARSETEST from the s90 numbers**
(library 4, reserve 3, capX 9, drawPerX 1): one call builds the header
(`xAnnounceLibraryNote(..., xLibraryCeilingX(9,1,4,3))`), one call builds the rows
(`xLifeDrawRowAnnotations(..., 4, 3)`), and the test asserts that **the largest drawing row that
carries no deck warning equals the header's ceiling**. A header and a row cannot name different
values without that case going red.

### J4b — on the X menu a row NAME is the X value, never an index conflict

`125v162` s90: `CHOICE: 2 (X = 2)` — index 2 is X=4, the name is row 4 — scored as a number/name
disagreement, `index_name_conflict`, one of the corpus's 7 fallbacks; s91 recovered with
`CHOICE: 4 (X = 2)`. One full round trip to be told again what the reply had already said
unambiguously. `menuIsBareXAnnounce` recognises the menu from its own rows (≥2 `X = <digits>` rows,
every other row a decline/hold), and the name-over-index branch then resolves to the named row with
`x_value_named` + `index_name_unique_name` and **no** `index_name_conflict` — the same reading
`#W66-AS` already gives the hold row's unique label. Off that menu the generic rule is untouched
(negative case pinned).

### J5a — the cannot-pay tag counts the mana already floating (deck130 HIGH-2)

`130v126` s29, verbatim from one prompt: `Already in pool: {r}{r}{r}{r}{r}{r}{c} (7 mana ALREADY
produced and floating right now)`, `Your hand (4 cards): Spark Spray {r} [instant] [cannot pay now:
needs 1 mana, you have 0 untapped sources]; ...`, and option `1. Cast Spark Spray`. **My own census
over the corpus** (all 2304 rendered prompts): **110** carry a floating pool, **74** of those carry
at least one cannot-pay tag (**264** tags), and **9** of those tags name a card the SAME prompt
offers a Cast row for. (The ledger's figures were 55 of 82 and 6; mine are counted with the hand
line split on `"; "`, which the ledger's may not have been.)

Mechanism: the scan built its pool from `ManaEngine::potentialMana` (untapped producers only) while
the affordability test it feeds, `LegalActionsOracle::payable`, falls through to
`ManaEngine::planPayment`, which adds `getManaPool()` before it plans. The scan asked the narrower
question and printed its answer as a verdict about the cast. Now the scan's pool folds the floating
mana (`castPool->add(getManaPool())`), the `kHandNeedsMana`/`kHandNeedsColours` split is decided
against `sources + floating`, and the tag names it: `[cannot pay now: needs 1 mana, you have 0
untapped sources and 7 mana already floating]`. With an empty pool every byte is wave 67's.

### J5b — a post-announcement decline says what it costs (deck130 HIGH-1)

`130v126` s21-s24 (T10): six sources and a Talisman were tapped into the pool for Starstorm, and the
X menu's decline row read `Decline - do not cast this after all (the announcement is cancelled and
the card stays in your hand)` — true about the CARD and silent about the mana. The seat declined;
the pool emptied at end of step. `130v162` s70-s73 is the same shape and lost the deck's own lethal
line. The decline row now carries `{declining now FORFEITS the N mana already paid: the card comes
back to your hand but the mana does not - your sources stay tapped, and any floating mana you do not
spend before this step ends is lost}` — rendered only when mana is actually floating, and stripped
from history like every other decision-time clause.

### J5c — the vanished cast says it vanished

The narration wrote `- Paid {r}{r}{x} for Starstorm with Mountain #1; ...` three times for a spell
that never went on the stack (`130v126` s25-s31, including a `- Paid {r}{r}{x} for Starstorm from
mana already floating` receipt at s30). `notePaymentQueued` now remembers the card and the step key
of the receipt it wrote, and a decline at the ANNOUNCE_X menu appends `Starstorm was NOT cast: you
declined after the payment above, so it is back in your hand and the 6 mana that paid for it is
still floating, unspent`. A real X answer retires the marker.

### J9a — the stack's death verdict rides the rows that let it resolve (deck123 HIGH-1)

`123v162` s32 (t12, 2 life): the board frame printed `ON THE STACK: 3 damage to you - you would be
at -1; that would KILL you`, and ~40 lines later a three-row menu. The seat wrote "Stack resolves
(damage to 2)", took the Hold row, and died. `stackDeathRowClause` repeats the verdict on the rows
that DECLINE — taking one IS taking the damage — the way the blockers seam repeats its lethal verdict
on the screen the declaration is made on:

> `{answers the stack: NO - 3 damage is ALREADY ON THE STACK, and taking this row lets it resolve:
> that puts you at -1 and KILLS you}`

Attached at the priority seam and the cast seam, on the `Hold priority:` and `Cast nothing` rows,
BEFORE the hold latch reads them, so a hold taken on a lethal screen is held over a row that says
so. Rendered only when the pending stack is lethal; a survivable stack leaves every row
byte-identical to wave 67.

A POSITIVE `{answers the stack: yes}` on a row that would PREVENT the death is deliberately **not**
claimed — see "What I did not verify". What the s32 row was missing instead is now printed: the
self-edict clause (`targeting yourself sacrifices one of them, your choice, and you gain its
toughness`) names the ceiling of a gain the seat itself picks — `- you pick which, so up to N
(your largest toughness)`.

### J9b — the hold re-opens when the CRACK-BACK verdict WORD changes (deck162 MED-3)

`162v146` s19/s20; `hold_windows_skipped` 56 in the game that was lost. The hold latch reads ROWS,
and the CRACK-BACK line lives in the board frame — so a hold taken while the crack-back was
survivable stayed held after it crossed into lethal, and AU R1's six enumerated clauses cannot help:
they deliberately normalise the life NUMBER out of the key, which is the only thing that moves in
`you would be at 5` -> `you would be at -3`. `crackBackVerdictKey` turns the crack-back into three
distinct terms (`none` / `you survive` / `LETHAL`) and both `takeHold` and `holdHonoured` put that
marker in the held set. It is recomputed at hold-check time off the live board (the same
`crackBackTotalOver` walk the printed line uses), never read from the last prompt — a latched copy
would notice the change one window late, which is exactly the window the seat dies in. The marker
carries no digits, so no AU R1 anchor can normalise it away, and it adds a term without removing any
of AU R1's.

---

## Gate numbers

| | base `4837e76a1` | this lane |
|---|---|---|
| PARSETEST | 5107 passed, 0 failed | **5148 passed, 0 failed** (+41) |
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1274 / 0 failed | **1274 tests, 0 failed** |
| AI tests | 67 / 0 | **67, 0 failed** |
| `==Test timed out` | 0 | **0** |

Build: `rm -f bin/wagic && make -f Makefile.sdl -j4`, binary re-stamped each time. Gate log:
`~/.gatelogs/w68-BB-parsetest.log`, `~/.gatelogs/w68-BB-suite.log`.

### RED on base — 21 cases

Every item here is a RENDER, PARSE or KEY change: none changes what the engine does with a card, an
option or an answer, so no suite fixture can be red on base and none is claimed. RED was produced
the way lanes AL/AQ/AW produce it — the eight mechanisms stubbed back to their wave-67 behaviour
with the new cases already in place, binary rebuilt (`5127 passed, 21 failed`), then restored and
rebuilt green.

| item | RED cases |
|---|---|
| J4a (row clause takes the whole reserve; the header/row identity) | 5 (incl. the 2 wave-67 cases whose wording this replaces) |
| J4b (bare-integer X name) | 2 |
| J5a (floating pool in the tag) | 2 |
| J5b (decline forfeiture) | 2 |
| J5c (abandonment narration) | 2 |
| J9a (stack death verdict + self-edict ceiling) | 4 |
| J9b (crack-back verdict key) | 4 |

---

## Falsifiable predictions for the wave-68 corpus

1. **J4a.** Every ANNOUNCE_X row of a drawing X spell whose remainder falls below the reserve carries
   `draws are already owed that you cannot decline and only N would be left`, and **no seat answers
   such a row**. FALSIFIER: one ANNOUNCE_X answer whose chosen row carries that warning.
2. **J4a (identity).** For every ANNOUNCE_X window in the corpus, the largest X whose row carries no
   `LOSE the game` clause equals the X named by that window's `largest value on this menu the
   library pays for`. FALSIFIER: one window where they differ.
3. **J4b.** `index_name_conflict` is 0 on ANNOUNCE_X windows, and every `x_value_named` record's
   `chosen_text` begins `X = ` with the value the reply's parenthetical named. FALSIFIER: one
   `index_name_conflict` on a menu whose rows are `X = N`.
4. **J5a.** No prompt contains both `[cannot pay now: ...]` for a card and an offered `Cast <that
   card>` row (my census: 9 such prompts in the wave-67 corpus). Every cannot-pay tag in a prompt
   that also prints `Already in pool:` names the floating mana. FALSIFIER: one contradicting pair.
5. **J5b/c.** Every ANNOUNCE_X decline taken with a nonzero pool is followed in the narration by a
   `was NOT cast: you declined after the payment above` line, and no seat declines an announcement
   with ≥4 mana floating more than once per turn. FALSIFIER: a `Paid ... for X` receipt with no
   matching abandonment line where the following record shows the card back in hand.
6. **J9a.** No seat takes a `Hold priority:` or `Cast nothing` row carrying
   `{answers the stack: NO ...}` and loses the game to that stack's damage in the next record.
   FALSIFIER: one such pair.
7. **J9b.** No hold survives a window in which the crack-back verdict word changed:
   `hold_windows_skipped` at deck162's seats falls below wave 67's 56 in a lost game, and no two
   consecutive same-seam records show the earlier taking a hold and the later printing a CRACK-BACK
   line whose `KILL you` presence differs from the earlier's. FALSIFIER: one such pair.

---

## What I did NOT verify

* **No live model probe was run.** Every claim above is from the wave-67 corpus, PARSETEST and the
  suite. Whether the model USES the new clauses is the wave-68 corpus's question, not mine.
* **The positive half of J9a is not implemented.** `{answers the stack: yes}` on a row that would
  prevent the death (a counterspell, a life-gain instant) is not claimed, because no per-row
  life-gain magnitude exists at the priority/cast seams that could carry it without risking a FALSE
  claim on a row whose effect the render cannot evaluate. What ships is the negative verdict on the
  rows that decline, plus the self-edict's own gain ceiling. If the wave-68 corpus shows a seat
  reading the negative verdict and still holding, the positive half is the next move and it needs a
  row-level life-delta the render does not have today.
* **J5c covers the ANNOUNCE_X decline only.** The `130v126` s28 shape — a decline on the
  ALTERNATIVE-COST menu (`Decline - do nothing`) after a receipt was written — still produces a
  receipt with no abandonment line. Hooking it needs a way to know that a CHOOSE_MENU is an
  alternative-cost menu for the card just paid for; I did not build one.
* **The abandonment marker can in principle mis-fire** on a same-named second copy declined in the
  same step after a successful cast of the first, if no ANNOUNCE_X answer intervened to retire the
  marker. No such window exists in the wave-67 corpus; I did not construct one.
* **The crack-back marker's cost is unmeasured.** It can only add re-opens, and I have not measured
  how many: the verdict word changes rarely, but `crackBackTotalOver` now runs once per hold check
  as well as once per prompt. No latency effect is claimed.
* **`stackPendingDrawsFor` is no longer read by the row clause** (the reserve subsumes it). The
  `rowStack` out-parameter is still computed at that call site and now goes unused there; I left it
  in place rather than change `xLibraryReserve`'s signature.
