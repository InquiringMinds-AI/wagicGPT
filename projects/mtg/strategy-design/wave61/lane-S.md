# Wave-61 lane S — C5, C6, C12, the best-X NET half of C10

Base: master `718e99d2a`. ONE file touched: `projects/mtg/src/AIPlayerGPT.cpp`. Every change is a
RENDER change (prompt text the LLM opponent reads): no engine behaviour, no rules resolution, no
legality gate, no option is added or removed. So the gate is PARSETEST plus a no-regression suite
run, and no suite fixture ships — there is no board assertion that could be RED on base. The
RED-on-base evidence is the corpus records cited per item, plus the PARSETEST cases, which do not
compile against base (`xNetLifeForX`, `oneShotDrawGrantScan`/`oneShotDrawGrantTag`,
`loopHalfAffordabilityClause` and the new `drawPriceRowTag` / `castDrawPriceRowTag` /
`xMonotoneMarker` / `pendingLoopWarningText` arities do not exist there).

Every code comment for this lane is tagged `#W61-S (Cn)`.

---

## C6 — two `DRAW PRICE` clauses on one row now fold SEQUENTIALLY into one resulting life

**Mechanism.** `drawPriceRowTag` (the card's own draw) and `castDrawPriceRowTag` (the draw an
opposing cast-trigger forces, Forced Fruition) were emitted from two independent blocks, each
subtracting its own cost from the SAME base life. `126v162` seq 16 (t13, life 23) therefore printed
`you would be at 22` and `you would be at 16` on ONE row whose true price was 8 — and that is the
row the seat took (§`wave60/deck126/review.md` HIGH-1). Corpus-wide, 2 rows of 99 draw-price rows
carry both clauses.

The two emitters are now called from ONE block. `drawPriceRowTag` takes `deferTotal`: it still
states its own cost in full and hands the resulting life to the clause below
(`(this row carries a SECOND draw price below; the two are added there and the resulting life is
stated once)`). `castDrawPriceRowTag` takes `priorCharge` and folds both charges once
(`; added to the 1 life the draw price above costs you, this ROW costs 8 life in total - you would
be at 15`), with the `this KILLS you` verdict computed on the SUM. Deferral happens only when the
second clause will actually price punishers; otherwise the first clause keeps its own total. Nothing
is deleted — both clauses print every magnitude they printed in wave 60.

**Byte-identity.** A single-clause row, and `priorCharge == 0`, are byte-identical to wave 60
(pinned by three NEGATIVE cases).

## C12 — a ONE-SHOT draw grant carries the converter count and the punisher price

**Mechanism.** Every draw row on a cast menu carries `converters on your battlefield: K` (via
`feedsRowTag`) — but `feedsRowTag` is fed by exactly two scans: `opponentExtraDrawPerTurn` (`@each`
per-turn grants) and `castTriggerDrawCount` (`@movedTo(...|opponentstack)` cast triggers). Peer into
the Abyss is neither: `choice name(Target opponent) draw:halfuptype:*:opponentlibrary opponent &&
life:-halfupopponentlifetotal opponent` (`mtg.txt:82571`, Oracle-checked; the branch labels are the
engine's own). It fell through both scans and carried no converter clause at all, on a menu whose
three OTHER draw rows each read `converters on your battlefield: 0` — `162v146` seq 23, t14, 3 life:
the seat cast it and wrote that Underworld Dreams would turn those 20 draws into 20 damage.
Underworld Dreams was on **nobody's** battlefield in that prompt (`wave60/deck162/review.md` HIGH-2).

New `oneShotDrawGrantScan` walks the card's own script for `draw:<expr> opponent|controller` on a
line that resolves when the SPELL does, and new `oneShotDrawGrantTag` renders
`[DRAW GRANT (one-shot, on resolution): ...]` — the pilot's converter count and names, what the
grant is worth against them (`so those 20 draws take 40 off them` / `nothing of yours punishes
their draws, so those 20 draws take 0 off them`), and, on a branch that draws for the PILOT, the
opposing draw-punisher price with the same `- you would be at K; this KILLS you` tail the cast rows
use. A MODAL card is scanned per BRANCH and each clause carries the engine's own branch label, so a
per-branch amount can never read as unconditional.

The amount is not a number in the script, so it is EVALUATED with the same `WParsedInt` the
`{right now: ...}` magnitudes use, on the same rails: never `rand`, never `x`/`xx` (the `{X pricing:}`
ladder prices those), never a resolution-time-only variable (`thatmuch`), never `power`/`toughness`
on a non-creature, never `manacost` on a targeted spell — and a branch whose amount will not
evaluate is NAMED as unclaimed rather than guessed.

**Blast radius, measured, not assumed.** I scanned all `Res/sets/primitives/*.txt` with the final
rails: **54 cards, 63 occurrences**, and every one is a genuine one-shot draw grant (Peer into the
Abyss, Timetwister, Time Spiral, Echo of Eons, Words of Wisdom, Sphinx of Enlightenment,
Rishkar's Expertise, Painful Truths, ...). The first draft matched 120 cards; each excluded class
was found by that scan and is skipped with the reason named in the code: a `newability[...]` payload
(an ability the card GRANTS, and `draw:1]))` is not a number), a `{cost}:` activated ability
(Bonder's Ornament), an `if ... then` conditional (Channeled Force), an `ability$!` sub-ability blob
(Browbeat), an `alternative` cast mode (Baleful Mastery), a macro-gated `_DIES_` line, `aslongas(`
(Depopulate — gated on their board), `foreach(` (Nature's Resurgence — per-object, not once),
`delayed` (Nissa's Revelation), a `rolld` branch, and `may draw:` (optional). A NUMERIC self-draw is
also skipped: `drawPriceRowTag` already prices exactly that shape on the same row, so every such row
stays byte-identical.

## C10 (NET half) — the best-X badge never endorses an X that kills the pilot in silence

**Mechanism.** `xMonotoneMarker` was computed from AFFORDABILITY alone. `125v162` seq 41/42 (t15,
my_life 2) rendered `1. X = 2 {X pricing: ... NET -2 life for this cast} [<- largest affordable X -
X=2 gains 2 life and draws 2 cards; no listed X does more]` — the badge endorsing an X the SAME ROW
had already priced at death, with `X = 1` at NET -1 surviving one line below. The seat took row 1
(`wave60/deck125/review.md` HIGH-2). Corpus: 2 `{X pricing:}` rows carry a negative NET; 1 of the 2
carries the badge.

The row's NET arithmetic is extracted into `xNetLifeForX`, and a new `xNetLadder` reads the SAME two
scans the row reads (`drawPunisherScan`, `forcedCleanupInputs`) to produce the NET at the cap and
the largest listed X whose NET still leaves the pilot alive. `xMonotoneMarker` appends — never
replaces — `- but NET -2 life for this cast puts you at 0; this KILLS you. X=1 is the largest listed
X whose NET (-1) leaves you alive, at 1`, or `No listed X leaves you alive` when none does. Both
surfaces carry it: the ANNOUNCE_X menu and, through `xCastRowMarkerFrom`, the cast row's
`[<- best X for this cast: ...]`.

The badge cannot drift from the row: PARSETEST pins `xNetLifeForX` against the NET
`xLifeDrawRowCore` itself prints, over a matrix of X. A NET the pilot survives, an unsupplied NET
and an unsupplied life each change **not one byte**.

## C5 — the loop pair

Two halves, and they landed differently.

**(a) LOOP HALF PENDING now states whether the other half is castable — SHIPPED.**
`126v146` seq 20 (t14) printed `the pair is one resolution from closing` with Sanguine Bond
`{3}{B}{B}` on the hand line, six untapped sources listed, exactly one able to make `{B}`, and no
Cast row for it anywhere on the menu with no reason given (`wave60/deck126/review.md` MED-3). Under
the trust doctrine a banner that says a resolution is available when it is not is a false surface,
and the guide's release condition is written against it.

`loopPendingSituationLine`, when the held half is in the SEAT'S OWN hand, now asks
`LegalActionsOracle::castableForDisplay(p)` — the same pure oracle the Cast rows are built from, so
the banner and the menu cannot disagree — and passes new `loopHalfAffordabilityClause` into
`pendingLoopWarningText`. It renders either `You CAN cast Sanguine Bond in this window ({3}{B}{B}) -
the closing resolution is on your menu right now.` or `You CANNOT cast Sanguine Bond in this window:
it is not among your legal casts right now (its cost is {3}{B}{B}; your untapped sources: 6, colours
you could make: {w}{u}{b}). The pair cannot close until that changes.` The verdict deliberately does
NOT guess whether cost or timing is the blocker — it states the oracle's answer and the mana facts,
both true either way. Every opponent-side and graveyard/exile pending half is byte-identical.

**(b) The closed-loop half of the finding is REFUTED — no change shipped.**
`wave60/engine-seat.md` HIGH-3 reports that when both halves stand on the seat's own battlefield the
render "names only the converter", "Exquisite Blood is not named", "the word loop does not appear",
5 of 5 closed-pair renders silent. I read the record it was written from. `126v130` seq 18 renders,
verbatim from the translog:

    LIFE-TO-DAMAGE CONVERTER on the battlefield: yours - Sanguine Bond. ... Both halves of a life
    LOOP are on YOUR battlefield (Sanguine Bond + Exquisite Blood): Exquisite Blood turns every life
    the other player loses back into life for you, which Sanguine Bond turns into life loss again,
    without limit. Any life you gain, or any life they lose, chains until they are at 0.

Corpus-wide: **84 prompts carry a LIFE-TO-DAMAGE CONVERTER block; 24 of them carry the closed-loop
clause.** The wave-49 `#W49-U D5` clause in `converterSummaryText` has been doing this job since
wave 49; the finding's quotation stops one sentence short of it. I did NOT rename the clause to
`LOOP CLOSED:` as the ask proposed — that would delete shipped wording the wave-49/wave-60 PARSETEST
cases pin, to no gain. Instead a PARSETEST case now pins the clause explicitly as this lane's, so a
later wave cannot re-report it as missing and remove or duplicate it.

---

## Gate

Detached unit `w61-S-gate`, `MemoryMax=4G`, `MemorySwapMax=0`, from
`worktrees/lanes/w61-S/projects/mtg/bin`, on a binary built with
`rm -f bin/wagic && make -f Makefile.sdl -j4` (clean link, no new sources, no qmake run).

- Suite, `WAGIC_TESTSUITE_THREADS=1`: **`Test suite finished: 1248 tests (0 failed), 65 AI tests
  (0 failed)`**; `==Test Failed !==` **0**, `==Test timed out` **0**. Equals the brief's baseline
  (1248 / 65 AI / 0 / 0).
- PARSETEST: **`=== self-test: 3742 passed, 0 failed ===`** — base 3699, **+43 cases**, all in one
  `[#W61-S]` block at the corpus end. No wave-60 case was deleted; none needed correcting.
- `git diff | /usr/bin/grep -c $'\357\277\275'` → **0**.

Each item ships a POSITIVE built on the cited corpus record, a MUST-NOT-MATCH NEGATIVE, and the ECHO
shape (the annotated row echoed in full still binds through `parseChoice`, and
`stripNarrationDecoration` leaves no residue in the append-only record).

## Predictions (wave-58 corpus, falsifiable)

- **C6.** No rendered option row will contain two `you would be at` figures. Every row carrying two
  `[DRAW PRICE:` clauses will carry exactly one resulting life, and it will equal
  `my_life - (cards x perDraw) - (perCast x perDraw)`. Wave 60: 2 of 2 such rows carried two
  contradictory figures (23 -> "22" and "16" for a true price of 8). Any two-clause row with two
  resulting lives, or with one that is not the sum, falsifies it.
- **C12.** Every cast row for a one-shot draw grant (Peer into the Abyss, Timetwister, Time Spiral,
  Echo of Eons, Words of Wisdom, ...) will carry `[DRAW GRANT (one-shot, on resolution):` naming the
  pilot's converter COUNT, and no such tag will name a permanent that is not on a battlefield line
  of the same prompt. Wave 60: 3 Peer rows, 0 with any converter clause, and the reply on the taken
  one asserted a punisher nobody controlled. A grant row with no tag, or a tag naming an absent
  permanent, falsifies it.
- **C10.** No `[<- largest affordable X` / `[<- best X for this cast` badge will be issued on a row
  whose own `NET` puts `my_life + NET <= 0` without the `but NET ... puts you at K; this KILLS you`
  clause, and where a lower listed X survives, that X will be named. Wave 60: 1 of 1 such badge was
  issued bare, and the seat took it. A bare badge over a lethal NET falsifies it.
- **C5.** Every `LOOP HALF PENDING` whose held half is in the SEAT'S OWN hand will carry either
  `You CAN cast <half> in this window` or `You CANNOT cast <half> in this window`, and the CANNOT
  form will never co-occur with a `Cast <half>` row on the same menu (they read the same oracle).
  Wave 60: 18 LOOP HALF PENDING renders, 0 with any availability statement, and `126v146` seq 20
  urged closing a pair whose half was uncastable. A pending banner with no verdict, or a CANNOT
  beside a live Cast row for that card, falsifies it.
- **C5 (b), the refutation.** The closed-pair clause will keep rendering: every prompt in which both
  loop halves stand on ONE battlefield will carry `Both halves of a life LOOP are on
  YOUR|THEIR battlefield (<converter> + <mirror>)`. Wave 60: 24 of 24 such renders already did.

## What I did NOT verify

- **No live probe, no model call.** No prompt was generated against a real board and no endpoint was
  contacted. The PURE emitters are pinned by PARSETEST; the BOARD-FACING wiring
  (`oneShotDrawGrantScan` against a live `MTGCardInstance`, the `castableForDisplay` /
  `potentialColorReach` call in `loopPendingSituationLine`, `xNetLadder`'s two scans) is exercised
  only by compilation and by the suite's non-GPT paths.
- **C12's evaluated amounts are unobserved on a live board.** The 54-card blast radius above is a
  scan of the primitives with the code's own rails re-implemented in python — it proves WHICH cards
  the scan will fire on, not what `WParsedInt` returns for each of their expressions at cast time. I
  did not confirm that `halfuptype:*:opponentlibrary` evaluates to the same 20 the `{right now:}`
  clause printed at `162v146` seq 23 (both go through `WParsedInt(expr, NULL, card)`, so they should
  agree by construction, but I did not observe it).
- **The DRAW GRANT tag's prompt-byte cost is unmeasured.** It is bounded (at most 4 branches, and
  only on the 54 cards) but I did not measure the growth against a real prompt.
- **C6's fold assumes the two clauses are the only draw prices on the row.** A third draw-price
  surface (the activated-ability row at the priority seam, the modal alt-cast row) still folds
  independently — those emitters were not in scope and are untouched; neither can co-occur with the
  cast row this lane changed.
- **C5's affordability verdict does not name WHICH constraint failed.** It states the oracle's
  answer plus the cost and the open-mana facts; it does not compute a per-colour source shortfall
  ("needs 2 black sources, you have 1"), and it does not distinguish a mana failure from a timing
  failure. The per-colour form is lane T's C7 territory.
- **I did not re-adjudicate any other lane's wave-60 prediction**, and I did not open
  `AllAbilities.cpp`, `MTGAbility.cpp` or any guide.
- **Only the cards named above were checked against the primitives** (Exquisite Blood
  `mtg.txt:37966-8`, Sanguine Bond `mtg.txt:99004-6`, Peer into the Abyss `borderline.txt:82571`,
  plus the whole-primitives blast-radius scan). Nothing was checked against Scryfall by me; the
  Peer Oracle text and its rounding were verified by the wave-60 deck162 reviewer, who states they
  checked Scryfall.
