# Wave-62 lane Y — D3, D4, D5, D7

Branch `w62-lane-Y` off master `0d9b2d52e`. Build: `rm -f bin/wagic && make -f Makefile.sdl -j4`
(qmake untouched, no new sources). Every wagic run under
`systemd-run --user --scope -p MemoryMax=4G -p MemorySwapMax=0`; the gate ran detached as
`w62-Y-gate2` with a foreground until-loop wait. No kill under the cap on any run.

## Gate

| run | result |
|---|---|
| suite, default threads (`w62-Y-gate2`) | `1252 tests (2 failed), 65 AI tests (0 failed)`, `==Test timed out` count **0** |
| the 2 failures | `lifeline.txt` and one more — the known concurrency-only pair (baseline 1249/2) |
| suite, `WAGIC_TESTSUITE_THREADS=1` (`w62-Y-t1`) | `1252 tests (0 failed), 65 AI tests (0 failed)`, 0 timeouts — **no new failure from this lane** |
| PARSETEST | `=== self-test: 3940 passed, 0 failed ===` (baseline 3915/0; +25 cases) |

Test count went 1249 -> 1252: the three fixtures below. `git diff | grep -c $'\357\277\275'` = 0
after every edit (all sources byte-spliced with python rb/wb).

## What shipped

### D3 — the LOOP CAUTION direction (the ledger's engine claim is REFUTED; the narration was the bug)

`src/AIPlayerGPT.cpp`: new `loopNonChainingClause()` + `loopCautionForLine()`; the two draw-step
forecast emitters now take the clause that is true for the number THEY carry.

**Oracle** (Scryfall, fetched 2026-09-05, `/cards/named?exact=Exquisite%20Blood`): `{4}{B}` —
"Whenever an opponent loses life, you gain that much life."

**Engine binding, verified before touching anything.** `Damage::resolve` raises
`WEventLife(-damage, fromDamage=true)` for a player target (`src/Damage.cpp:299`), and
`TrLifeGained::triggerOnEventImpl` (`include/AllAbilities.h:1628`) filters on the SIGN of the
amount and the two side flags only — it never reads `fromDamage`. Two live probes on base
confirmed both directions:

- my Exquisite Blood + my Lightning Bolt at the opponent → I gain exactly 3 (**the trigger DOES
  fire on damage-based life loss**);
- their Exquisite Blood + Sanguine Bond, my Lightning Bolt at them → my life untouched, theirs
  −3.

The second is rules-correct and is exactly what `162v126` seq 52–55 recorded (opp 20 → 0,
`my_life` pinned): THEIR Exquisite Blood reads "whenever an OPPONENT loses life", and its opponent
is the pilot, who lost nothing. **So no engine change ships for D3, and no RED-on-base engine
fixture is owed for it** — the mechanism the finding asserted does not exist. (Instrument check:
the first probe was re-run with a deliberately wrong `life:` assert and went RED, so the fixture
harness really was reading the life total.)

What WAS false is the caution's **binding**. A loop held by side H is entered from exactly two
places: H gains life, or H's OPPONENT loses life. `loopCautionForBoards` handed one sentence to
every line, so the fatal-chain caution ("any life you pay is fatal", "a number above that reads as
life TO you is not a safe gain") stood on `DRAW FORECAST (theirs): ... = 24 life to you from your
punishers above` — a number that is life THEY lose and enters their loop nowhere. A seat that
believed it concludes its own win condition is suicide; the review records that this seat only won
by disbelieving the sentence.

Now:
- `drawStepForecastText` (YOUR draw step under THEIR punishers — life YOU lose) keeps the chaining
  caution for a THEIRS pair, and gets the scope sentence for a YOURS pair;
- `theirDrawStepForecastText` (THEIR draw step under YOUR punishers — life THEY lose) gets the
  scope sentence for a THEIRS pair, and the chaining caution for a YOURS pair;
- `drawPunisherSummaryText` (the board paragraph, no single number) is untouched.

Nothing is deleted: the new sentence is a true clause that names both entry points the chain
actually has. Fixtures `w62y_lifelost_damage_fires.txt` and
`w62y_lifelost_damage_wrong_side.txt` pin the binding in both directions (GREEN on base by
construction — they are regression pins for the half the ledger accused, not a fix's proof).
PARSETEST: positive for both faces, must-NOT-match negatives (one half only; no pair at all; the
scope sentence makes no "fatal" claim), and both forecast lines asserted to carry the right one and
not the other.

### D4 — sibling abilities of one card collapsed into ONE ranking key

`include/AIPlayerBaka.h` (`CmpAbilities`), `include/AIPlayer.h` + `src/AIPlayer.cpp` (`AIAction::id`).

Mechanism: Lair of the Hydra defines twenty animate rungs (`borderline.txt:64293-64312`). All
twenty score the same `STANDARD_BECOMES` efficiency, carry no target, and share one click card, so
in `RankingContainer` (`std::map<OrderedAIAction,int,CmpAbilities>`) every field the comparator
walked — efficiency, `id`, `target`, `playerAbilityTarget`, `player`, `click` — compared equal and
`ranking[aiAction] = 1` kept only the FIRST rung inserted. That is why the corpus renders the
animate row 387 times and every one of them reads `becomes a 1/1 hydra [cost: {1}{g}]` at every
mana total (`/usr/bin/grep` over the corpus: 354 + 65 + 12 + … , zero `2/2`..`20/20`).
Worse, `AIAction::id` — the comparator's FIRST tie-break after the efficiency — was never
initialised by any constructor, so the ordering was reading an indeterminate value.

Fix: tie-break on the ABILITY POINTER (the identity that actually separates two abilities of one
card), and zero-initialise `id` in every `AIAction` constructor. Strictly more rows, no row
duplicated, no row removed. Baka's own pick is unchanged by construction — the collapse only ever
merged EQUAL-efficiency siblings, and `chooseOrderedAction` still takes `ranking.begin()`.

RED on base, verified: with the one tie-break line removed and the binary rebuilt, PARSETEST read
`3938 passed, 2 failed` with `FAIL #W62-Y D4 two equally-rated abilities of ONE card are two
ranking keys` and its NEGATIVE; restored and rebuilt it reads `3940 passed, 0 failed`. The case
pins `id` equal on both actions so it cannot pass on garbage.

### D5 — the X menu now has the decline row the engine already had

`src/DecisionContract.cpp` (both ANNOUNCE_X branches + `applyMenuChoice`), `src/AIPlayerGPT.cpp`
(the ANNOUNCE_X ask, and `askModel`'s format line).

`MTGRules.cpp:581` arms the X announcement with `setCustomMenuObject(card, false, selection)`, so
`ActionLayer` appends a `Cancel` row and leaves `cantCancel` at 0 — a human has always been able to
back out of the announcement. The decision contract never carried that row: `req.canDecline` was
hard `false` for ANNOUNCE_X and `applyMenuChoice` dropped any negative choice. `130 s21/s22`
therefore offered `Cast Starstorm {X pricing: your mana affords only X=0 right now, which deals 0
damage and kills nothing}` and then a ONE-row menu headed `this ask has no pass row`; the card was
unavoidably spent (offered 7 windows, taken 1).

Now: `req.canDecline = !object->checkCantCancel()` on both ANNOUNCE_X branches; the GPT ask appends
`Decline - do not cast this after all (the announcement is cancelled and the card stays in your
hand)` and maps that pick to `act.choice = -1`; `applyMenuChoice` clicks the menu's own last row for
it — the same index the CHOOSE_MENU decline already used. No cast row is suppressed and no legal X
is capped: one more TRUE row.

Side-fix in the same change, because the new row made an existing line false: `askModel`'s format
line printed `(this ask has no pass row)` on EVERY ask, including the CHOOSE_MENU asks that already
appended `Decline - do nothing`. It now prints `kDeclineRowFact` — `(the LAST row of this menu
declines: it is a real answer, not a fallback)` — when the caller says it gave a decline row, and
`kNoPassRowFact` otherwise.

Fixture `w62y_announce_x_cancel_keeps_card.txt` pins the engine claim the change rests on: Ivy
Elemental `{X}{G}` over `{3}{G}` (menu X=0..3, Cancel at index 4), `choice 4` → the card stays in
HAND and the floated `{3}{G}` stays floating. GREEN on base by construction — the row is the
engine's; what was missing was the contract carrying it.

### D7 — the pay-repeat modal ask now prints its bill

`src/AIPlayerGPT.cpp`: `payRepeatPerCounterCost()`, `manaCostTextCmc()`, `payRepeatRowCostTag()`,
and the annotation block at the CHOOSE_MENU seam (the Intrepid Adversary menu arms as CHOOSE_MENU,
not CHOOSE_MODE).

`G2 seq 23/24` rendered `Choose an option for Intrepid Adversary:` with "add 1 counter" … "add 20
counters", the partial-pay NOTE, and no mana line, no total and no `{paying this taps: ...}`
clause; "add 3 counters" spent 6 more mana through Katilda, Dawnhart Prime and Elite Spellbinder —
both creatures — and t11's Attackers step vanished. The guide's #2a stop is built entirely on that
clause, so with the clause absent the stop cannot fire.

The per-counter price is on the card's own script line that names the option
(`choice name(Add 3 counters) ... pay({1}{W}) ...`, `borderline.txt:57864-57883`), read from there
and never guessed — a row whose line carries no `pay(` keeps the engine's text untouched. Each
priced row now gains
`{repeat cost: 3 x {1}{w} = 6 mana for all 3; you have 5 spendable now, which pays for 2 of them
and stops}` (spendable = floating pool + `potentialColorReach`), and, where the affordable share
cannot come out of the floating pool, the same `{paying this taps: ...}` clause every priced cast
row carries — same helper, same per-source attack/block restriction. `{repeat cost: ` is added to
`stripNarrationDecoration`'s drop list (decision-time pricing, not history).
`req.optionTexts`, the option ORDER and the answer index are untouched.

PARSETEST: positive for both label capitalisations, must-NOT-match negatives (the
`Don't add any counter` row, a row with no matching script line, an empty script), the converted-cost
helper, all three tag shapes (partial / full / no-mana-figure), and the echo shape — bare label
binds, whole annotated row binds, and the two tags leave no residue in the narrated record.

## Predictions (falsifiable, for the wave-62 corpus)

1. **D3** — zero prompts will carry the substring `LOOP CAUTION` on a `DRAW FORECAST (theirs):`
   line. Where a completed pair sits on the opponent's battlefield and the seat holds draw
   punishers, that line will instead carry `LOOP SCOPE: they control BOTH halves`, and the
   `DRAW FORECAST:` (your own draw step) line on the same screen will still carry
   `LOOP CAUTION: they control BOTH halves`.
2. **D4** — in any window where Lair of the Hydra (or another multi-rung animate manland) is in
   play untapped with N ≥ 2 affordable rungs, the option list will contain more than one
   `becomes a K/K hydra` row, and at ≥ 4 mana at least one row with K ≥ 2. Corpus-wide the count of
   distinct `becomes a K/K hydra` strings will be > 1 (it was exactly 1 across 387 rows).
3. **D5** — every ANNOUNCE_X ask record will carry a final option beginning
   `Decline - do not cast this after all`, its format line will read
   `(the LAST row of this menu declines...)` and not `(this ask has no pass row)`, and no record
   will show a one-row X menu. Where `X pricing: your mana affords only X=0` appears on a cast row
   that is then taken, the X menu that follows will offer 2 rows, not 1.
4. **D7** — every `Choose an option for <card>:` ask with two or more `add N counters` rows will
   carry a `{repeat cost: N x {…} = M mana` clause on each such row, and where the payment plan
   taps a creature, a `{paying this taps: ` clause naming it on the same row.

## What I did NOT verify

- **No live model probe was run.** Every claim above is from PARSETEST, the suite, the corpus
  greps and source reading. None of the four predictions has been observed on a real prompt.
- **D3**: the two probes cover Lightning Bolt (`damage:N target`). I did NOT run the
  `@drawfoeof(player):damage:1 opponent` punisher shape (Underworld Dreams / Fate Unraveler /
  Ob Nixilis) end-to-end through a draw step, nor combat damage, nor lifelink; I read that they all
  route through `Damage::resolve`'s player branch, and did not prove it per card. I also did NOT
  re-verify the OTHER `lifelost*` bindings (`lifelostof`, `lifeloss`) against a board — they build
  the same `TrLifeGained` with `type=1` from the same factory lines, which is a source-level
  argument, not a test.
- **D3**: I did not touch `converterSummaryText`'s board paragraph or `pendingLoopWarningText`.
  If either carries the same direction error on a numbered line elsewhere, this lane did not find
  it.
- **D4**: no board-level suite fixture discriminates this change, and I did not write one that
  claims to. Baka picks `ranking.begin()` and the collapse only merged EQUAL-efficiency siblings,
  so Baka's action is unchanged by construction; the only consumer whose output changes is the GPT
  option list, which the suite cannot render. The RED/GREEN evidence is the PARSETEST comparator
  case (verified RED with the tie-break removed and the binary rebuilt). I did NOT measure the
  prompt-size cost of the extra rows — a 20-rung manland at high mana will add rows to the menu.
  I also did not audit whether any other consumer of `RankingContainer` (AIHints::findActions,
  `doAbility`) depends on the old collapse.
- **D5**: the cancel path is pinned for Ivy Elemental only. I did NOT test the `pay[[{X}]]`
  announcement round (`MenuAbility::announcing`), where the same `canDecline` now fires and where
  the decline runs a DIFFERENT engine branch (the explicit `announcing` out-of-range branch in
  `MenuAbility::reactToChoiceClick`). I read that branch; I did not exercise it.
  I also did not check whether an X spell with extra costs already paid (targets chosen, additional
  costs committed) leaves anything behind when cancelled beyond the floating mana the fixture pins.
- **D5**: changing `askModel`'s format line changes the ask KEY for every CHOOSE_MENU ask that
  offers a decline. It is stable for a given state (nothing counted, nothing time-varying), so it
  cannot livelock, but the ask CACHE from a pre-change record will not hit — untested against a
  live run.
- **D7**: the `{repeat cost:}` numbers were proven pure (PARSETEST) but never rendered against a
  real Intrepid Adversary board. `spendable = pool + potentialColorReach` assumes the two do not
  overlap (`potentialColorReach` walks untapped producers only); I read the function, I did not
  measure it against a board with floating mana. The clause also assumes every counter costs the
  same — true for the Intrepid Adversary class, unverified for any other card that arms this menu.
- **D7**: I read `intrepid_adversary_repeated_payment.txt` (lane T, registered in `_tests.txt`)
  and it passed in every run of this lane's gate, including the single-threaded one. I did NOT
  reproduce or diagnose its reported intermittency, and nothing in this lane addresses it.
- The 2 default-thread suite failures are asserted to be the known concurrency-only pair on the
  strength of `lifeline.txt` being named in the log and the single-threaded run reading 0 failed.
  I did not name the second file.
