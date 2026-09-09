# Wave-74 lane CC — render truth

Base: master `c0551632e` (seeded worktree `worktrees/lanes/w74-CC`, branch `w74-lane-CC`).
Corpus for every repro: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-225637-final`.

**Gate.** PARSETEST 5630 passed / 0 failed (base 5593/0 — 3 of those pins were
SUPERSEDED by this lane and rewritten, +35 new cases, net +37). Suite THREADS=1:
**1285 tests (0 failed) + 76 AI tests (0 failed)**, `==Test Failed !==` 0 and
`==Test timed out` 0 — no flakes fired on this run (`lifeline`,
`merrow_reejerey`, `intrepid_adversary_repeated_payment` all passed). Two new
fixtures (base 1283). Diff U+FFFD count 0.

Two new suite instruments, both because the defect under test is a GPT RENDER
string and the suite asserts zones:
- `assertghostformlive <0|1> <card>` — asks the board render's own liveness
  predicate (`gptGhostformGrantLive`, exported out of the file's anonymous
  namespace for this).
- `assertchoosertargeted <0|1>` — reads `tc->targetter` on the chooser the
  engine is waiting on, i.e. edict-vs-target as the parser actually built it.

---

## O1 (HIGH) — the ghostform gloss was FALSE on 56/56 renders

**Repro.** `1788839837-…deck126-…-vs-…deck146.jsonl` seq 16, one prompt, two
lines apart:

- event line: `- Opponent's Silverquill Silencer got a ghostform (when this creature dies or is exiled its owner returns it to hand and creates a 1/1 white flying Spirit - killing it is not removal) counter (now 3/2)`
- board line: `Silverquill Silencer {b}{w} (3/2) [counters: 1x ghostform (a marker only - the ability that returned it is gone)] [named: Idyllic Tutor]`

**Mechanism (verified against the primitive, not the review).**
`planeswalkers.txt:1874` — Kaya the Inexorable's +1 grants
`newability[@movedto(this|graveyard) … :name(Put back to hand) all(creature[fresh;counter{0/0.1.Ghostform}]|mygraveyard) moveto(myhand) and!( token(Spirit Cai) )!]`.
The wave-73 predicate matched the menu text `"Put back to hand"`. `all(` is a
LORD keyword (`kLordKeywords`, AIPlayerGPT.cpp:21), so the object the factory
builds is `GenericTriggeredAbility{ t = TrCardAddedToZone, ability = ALord }`;
`GenericTriggeredAbility::getMenuText` forwards to the nested ability
(MTGAbility.cpp:9173) and `ALord` has no `getMenuText` override, so it returns
`ActionElement`'s base `"Ability"`. The script's own `name()` never reaches the
scan — so the predicate answered 0 on every live grant, and the gloss printed
the DANGEROUS direction (it told the seat the creature was safe to trade with).

**Fix.** Judge the grant STRUCTURALLY: a live `TriggeredAbility` sourced on this
creature whose nested payload (`NestedAbility` / `MultiAbility` / `AAMover::andAbility`,
depth-capped 8) is an `AAMover` into a HAND. That is exactly what the gloss
promises, it survives every wrapper the factory may add, and it disappears with
the ability objects when `ALoseAbilities` removes them — that removal keys on
`currentAction->source == _target` (AllAbilities.cpp:10389), the same pointer
this scan keys on. The menu-text test is kept as a first cheap path.

**RED on base.** `Res/test/w74cc_ghostform_grant_live.txt` — Kaya + Grizzly
Bears, `assertghostformlive 0` before the +1 and `1` after. With the structural
branch disabled (`if (false && w74GrantMovesToHand(...))`, rebuilt):
`TESTSUITE assertghostformlive: 'grizzly bears' expected 1 got 0` →
`==Test Failed !==`, `1 tests (1 failed)`. Restored: `==Test Successful !==`.
Both states are in the one fixture (no grant → 0, green on both trees; grant
live → 1, red then green).

**Prediction (next corpus).** Every `[counters: … 1x ghostform …]` line on a
creature whose grant has not been stripped renders the FULL gloss and 0 render
the `(a marker only …)` form; the marker form appears only after an abilities
wipe. Falsifiable by one grep of the corpus prompts for `a marker only`.

---

## O3 (HIGH) — the forced-sacrifice header on a pump target picker

**Repro.** `146v125` deck146 seat seq 59 chose the mode
`creature gains 3/3 and you draw` (no sacrifice half — the row's own annotation
says so). Seq 60 then rendered the pump's target picker as:

> `FORCED SACRIFICE OF ONE OF YOUR OWN CREATURES: your own effect (Silverquill Command) makes you sacrifice a creature YOU control. Every row below is a creature you will LOSE off the battlefield … Pick the creature you can best AFFORD TO LOSE (usually your least useful body)`

over four of the seat's own bodies, each tagged `[you SACRIFICE this]`. The seat
answered `CHOICE: 2 (Hive of the Eye Tyrant)` — its worst body — and the pump
made a 4/4 into a 7/7 by accident. Contrast `146v126` seq 27, the sibling mode
that DOES sacrifice: the header there is the correct pump-target one, and it
escaped only because the opponent's three defenders were on the pump's legal
list, i.e. by luck, not by a guard.

**Mechanism.** `borderline.txt:102740-102747`: Silverquill Command writes eight
`choice name(...)` lines into one `magicText`, four of them carrying
`ability$!… notaTarget(creature|mybattlefield) sacrifice!$`. The wave-60
detection reads that string off the CARD, so it fires whichever mode is acting.
The chosen mode's own pick is `target(creature)`.

**Fix.** New pure predicate `forcedSacrificeShape(script, allMineBattlefieldCreatures,
chooserIsTargeted)` — the header is refused when the pick is a real target. The
structural basis: `notaTarget(...)` clears `tc->targetter` at parse time
(MTGAbility.cpp, the notatarget branch) and `target(...)` keeps it.

**RED on base.** Two halves.
1. PARSETEST contrast pin: the wave-73 rule written out (`sacrifice` +
   `notatarget(creature|mybattlefield)` present) is asserted to FIRE on the
   seq-60 script while `forcedSacrificeShape(sqc, true, true)` is false — the
   red and the green in one CHECK. The seq-27 shape and the real Tribute-to-Hunger
   edict are pinned as MUST-NOT-MATCH (untouched).
2. `Res/test/w74cc_edict_chooser_untargeted.txt` — the empirical half, so the
   gate is not resting on my reading of the parser: Giant Growth's chooser
   `assertchoosertargeted 1`, Fleshbag Marauder's edict chooser
   `assertchoosertargeted 0`, both green on the engine's own objects.

**Prediction.** In the next corpus, `FORCED SACRIFICE OF ONE OF YOUR OWN
CREATURES` appears only on records whose acting effect is an edict; every
Silverquill Command mode whose target pick is the pump renders the
`TARGET CHOICE for Silverquill Command` header instead. Falsifiable by
cross-tabbing the header string against the preceding `CHOOSE_MODE` record.

---

## O5 (HIGH) — ATTACK TOTAL ignored the seat's own life-to-damage converter

**Repro.** `126v130` deck126 seat seq 43, opponent at 5, one attacker
`A1. Vampire #1 (2/1) [lifelink]`, `LIFE-TO-DAMAGE CONVERTER on the
battlefield: yours - Sanguine Bond` in the same prompt:

> `ATTACK TOTAL: 1 attacker listed, 2 total combat damage to a player - declaring all of them with none blocked puts them at 3. That is NOT lethal: they survive at 3 even with none of them blocked.`

The lifelink gain of 2 is life the opponent ALSO loses through the seat's own
Sanguine Bond, so the true unblocked figure is 1. The prompt contradicted its
own converter paragraph, and the categorical verdict was computed from the
wrong number.

**Fix.** `attackTotalLine` takes `selfConverterLifelink` (last param, default 0).
The caller sums the power of LIFELINK attackers among the OFFERED rows (×2 for
double strike, the same way the blockers' lifelink ceiling counts it) when the
ACTING seat's own battlefield carries a `lifeToDamageConverterScript` permanent.
The line then prints the converted figure beside the combat-only one and the
verdict is computed from it. The `guaranteed`-floor branch is deliberately NOT
adjusted: which attackers are in the floor is not known here, and under-claiming
a kill is the safe direction.

**RED on base.** PARSETEST: `attackTotalLine(1, 2, 5, 0, 2, …, 2)` must contain
`they are at 1, not 3` and `That is NOT lethal: they survive at 1`, and must NOT
contain `survive at 3` — the exact wave-73 output. `…, 0)` is pinned
byte-identical to the 5-argument wave-73 call, so no non-converter window moves.

**Prediction.** Next corpus: every `ATTACK TOTAL` record on a seat controlling
its own converter with a lifelink attacker carries the
`of that damage is LIFELINK` clause, and no seat is told "NOT lethal" over a
swing that its own converter makes lethal. Cross-tab: converter present in
CURRENT SITUATION as `yours` × attacker row carrying `[lifelink]` × clause
present.

---

## O12 (MED) — "without a restriction against attacking" counted permanently tapped bodies

**Repro.** `125v123` deck125 seat seq 399:

> `Opponent battlefield (125 permanents listed, of which 103 are creatures, 103 of them without a restriction against attacking, and 17 are lands): … Human #1-#102 (1/1) [doesn't untap during its controller's untap step] [tapped - cannot attack or block this turn] x102 …`

Two Intruder Alarms; 102 of the 103 are tapped and have no untap step to come
back from. The same line tags them and then counts them.

**Fix.** `attackRestrictionFree` takes a fifth restriction —
`tappedAndWillNotUntap` — and `boardCreatureCanAttackNow`'s static branch passes
`c->isTapped() && (basicAbilities[DOESNOTUNTAP] || c->frozen >= 1)`, the same
pair line 23112 already uses for this question. Tappedness ALONE stays out (it
clears at their untap, which is why the static branch ignores it).

**RED on base.** PARSETEST: `!attackRestrictionFree(false,false,false,false,true)`
fails on the seeded tree, where the 4-argument function has no fifth term. Two
MUST-NOT-MATCH cases pin every wave-73 call site byte-identical.

**Prediction.** Next corpus: on any board carrying an Intruder Alarm plus tapped
tokens, the header's able-to-attack count is strictly less than the creature
count, and the deck125 guide's compensating sentence becomes redundant rather
than contradicted.

---

## O15 (MED) — loyalty rows carried no consequence

**Repro.** `126v130` deck126 seat seq 42 (the deciding decision of that loss).
Both Sorin rows render as label + `[cost: Counters]` + the full card text and
nothing else: `1. +1: create a 1/1 vampire with Sorin, Lord of Innistrad …`,
`2. -2: emblem: "creatures get +1/+0" with Sorin, Lord of Innistrad …`. The
token's summoning sickness is stated by the ATTACKERS seam one window later;
the emblem's cumulativity with the live one is stated nowhere.

**Fix.** `loyaltyRowConsequenceTag(label, hasteText, emblemsLive)` — pure over
the row's own label plus a board count — appended in `describeAction` for
planeswalker rows only, guarded against a second `{right now: ` on the composed
row. `dynamicMagnitudes` returns "" for walkers by design (N-146g: a walker's
magicText bundles every loyalty, so the MAGNITUDE is unattributable), so this is
the only such bracket the row can hold, and the magnitude is deliberately still
not claimed.

Card check: `Sorin, Lord of Innistrad` `-2` is
`emblem transforms((,newability[lord(creature|mybattlefield) 1/0])) forever dontremove`
with `text=… -2: You get an emblem with "Creatures you control get +1/+0."` —
each emblem is its own object granting its own lord, so a second −2 stacks.

**RED on base.** PARSETEST: seven cases, including the two seq-42 rows
(REPRO) and four MUST-NOT-MATCH (a haste token says nothing; a `-3: exile`
loyalty says nothing; a non-loyalty row is not a loyalty row; the clause opens
with ` {right now: ` so the composed-row duplicate census can see it). The
function does not exist on base, so all seven fail there.

**Prediction.** Next corpus: every `+N:` row that creates a body carries
`summoning sick`, every emblem row carries the cumulative/permanent clause, and
no walker row carries two `{right now: ` brackets (the DEVLOGS census would
print a `W69-BJ F1` line if it did).

---

## O16 (MED) — no `(keeping N)` on the first mulligan window

**Repro.** deck130 seq 2 (identical in all games): the fresh-7 header carries no
size clause at all, so the deck guides' mulligan bullets — which key on the
`(keeping N)` spelling the LATER windows use — are unreachable on the one window
every game has.

**Fix.** `mulliganKeepClause(mullsTaken, keepSize)`: ` (keeping 7)` at
mullsTaken 0, and the wave-47 sentence byte-identical after that.

**RED on base.** PARSETEST pins `mulliganKeepClause(0, 7) == " (keeping 7)"`
(absent on base) and the mullsTaken>0 string byte-for-byte.

**Prediction.** `(keeping ` appears on 100% of pre-game mulligan prompts next
corpus (wave 73: only the post-mulligan ones).

---

## O19 (MED) — the X ladder now CLIMBS and the option number IS the X

**Repro.** `125v126` seq 100 — `1. X = 7 … 7. X = 1 … 8. X = 0`, reply
`CHOICE: 5 (X = 5)`; index 5 is X = 3, the name says X = 5, and the translog's
`chosen_text` shows the NAME match rescued it to option 3. `125v123` seq 624 is
the same shape. Two waves running, the corpus's only index/name conflict class.

**Fix.** The rows are permuted as the LAST act of assembly (after every
annotation has been placed at its `capX - X` index, so nothing upstream moves)
into `X = 1, 2, … capX, then X = 0`. Option N announces X = N for every rung
above zero; X = 0 keeps its row as the last rung. Consequences handled:
- the header states the identity and where X = 0 is (and a `capX == 0` menu gets
  its own sentence);
- ` Reply with the OPTION number, not the X value:` → ` Reply with the OPTION number:`
  (the second half stopped being true);
- the pick map back into the contract's `index==X` space is
  `(pick < capX) ? pick + 1 : 0`, tracked by an `xClimbing` flag so the
  descending map survives any shape where the permutation does not run;
- the ask NARRATION is registered AFTER the permutation (it is indexed in shown
  space — leaving it where it was would have narrated the wrong X for every row);
- the monotone-X range collapse reads its step off the run's own first pair
  instead of hard-coding −1, and `monotoneXRangeRow` renders
  `X = a up to X = b … smallest X first` when the run climbs. The descending
  wording is byte-identical, so wave 56's pins hold.

This deliberately supersedes the wave-23 largest-first order. That order existed
because the model replies its intended VALUE as the option number; with X = N at
option N that slip is no longer a slip, which is strictly better than either
previous order.

**RED on base.** Three wave-73 pins FAILED on the changed tree and were rewritten
as SUPERSEDED (`W23-X header ties option 1 to the cap value`,
`#W63-AD E7 the answer instruction stays LAST on the header`, plus the O21 one
below) — that failure IS the red: the base strings are pinned gone by
MUST-NOT-MATCH cases. New cases pin the header, the permutation, the
permutation↔pick round trip, and both directions of the range row.

**Prediction.** Next corpus: `index_name_conflict` on ANNOUNCE_X records drops to
0, and any `CHOICE: N (X = N)` reply resolves to the row it names without a name
rescue (`chosen_text` X equals the replied number).

---

## O20 (LOW) — the casting hold's headline was false at the very next window

**Repro.** 15 of 21 upkeep holds were followed by a PRIORITY record in the same
phase. The row's headline is `do not ask me again`, and the latch is kept PER
SEAM by design (#W62-Z), so that is the contract working — and the row never
said so.

**Fix.** The CASTING spelling (`kHoldPriorityRowTextCast`) now ends
`… and NOT the priority window that follows on this same step - that is a
different question at a different seam and you will still be asked it)`. The two
PRIORITY spellings are untouched (the priority seam is the one being described,
not a sibling).

**RED on base.** PARSETEST: the clause is absent from `holdRowLine(true)` on base;
`holdRowLine(false)` and `holdRowLine(false,true)` are pinned free of it.

**Prediction.** Next corpus: no reasoning body on a casting-seam hold treats the
following priority window as an engine bug or a contradiction.

---

## O21 (LOW) — three of the four sub-items

**(b) the worked example copied an annotation the same sentence forbids.**
Repro `…deck146-…-vs-…deck130.jsonl` seq 14:
`e.g. "CHOICE: 1 (Brightclimb Pathway (menu text: Play Land))"` under
`copy nothing from the {...} annotations`; the model wrote "This is slightly
contradictory but I'll follow the exact example string." `askExemplar` now cuts a
trailing ` (menu text: …)` the same way it cuts a `{…}` tail. ONLY that spelling
— a row whose short name genuinely ends in parentheses (`mulliganRowLabel`) is
pinned untouched.

**(c) the `[second copy: …]` stacking gloss read as encouragement** (55 renders,
`126v146` seq 30). Its first half was permission and its second an argument in
favour, with no cost side. The cost is the same on every one of those rows and is
a fact, not a recommendation, so the tag now ends
`… the price is a card and this window's cast spent DOUBLING an effect you
already have on the battlefield, not adding one you do not`. The wave-52 HEAD is
still byte-identical (guides key on it); the wave-69 K8 pin was rewritten as
SUPERSEDED.

**(a) hold text ~450 bytes — NOT DONE, deliberately.** Every sentence in
`holdRowBenefitClause` is a fact the latch's predicate actually honours (the
turn-start release, the two forgiven differences, the "any other change re-opens"
list), and the trust doctrine forbids trimming a true token off a row the pilot
prices. O20 ADDS to the casting spelling rather than shrinking it. Shrinking this
row is a scoping decision about what the latch is allowed to leave unsaid, not a
byte-count edit; it wants its own item.

**(d) the hold row is still taken on menus with a live activated row** — a
BEHAVIOUR observation, not a render defect. Wave 72 already put the activation
sentence on that spelling (`kHoldPriorityRowTextActivation`) and wave 73 measured
4 takes in 248 decisions on this seat. No change; reported.

**Prediction.** Next corpus: 0 worked examples containing `(menu text:`; 0
reasoning bodies calling the example contradictory; the `[second copy:` clause
appears with its price half on 100% of stacking renders.

---

## Weakest evidence — what I did NOT test

- **No corpus, no live game.** Every fix here is gated by PARSETEST + the suite.
  Nine of the nine items are RENDER strings; only O1 and O3 have a runtime
  fixture. The rest are proven pure and proven to be CALLED (the call sites
  compile and the suite exercises the seams), but the composed prompt was not
  re-rendered end to end for any of them.
- **O1 is proven on the GRANT, not on the WIPE.** The fixture covers "no grant →
  0" and "grant live → 1". I did NOT build the third state the gloss is written
  for — abilities REMOVED by a Sudden Spoiling-class effect with the counter
  still on the creature — so the `(a marker only …)` branch is now reachable only
  in theory. If `ALoseAbilities` misses a granted trigger for some reason my scan
  would keep saying "live", which is the DANGEROUS direction. Untested.
- **O3's `tc->targetter` reading is proven on two cards** (Giant Growth,
  Fleshbag Marauder), not on the whole `notaTarget(` family. In particular I did
  NOT run Silverquill Command itself through a fixture — the repro is corpus
  text plus the primitive, and the discriminator is the fixture's two cards.
- **O5's arithmetic is not the whole truth.** It converts LIFELINK only. A
  converter also fires on blocking triggers, drains and any other gain the swing
  causes, and the `guaranteed` floor branch is deliberately left uncorrected. The
  figure is now closer, not complete. I also did not verify against a live
  Sanguine Bond board that the engine actually applies the drain in the order the
  clause implies (the wave-73 review's `−2 emblem` arithmetic came from the
  reviewer, not from me).
- **O12 has no fixture.** The lord-granted path IS confirmed
  (`mtg.txt:58851` `auto=lord(creature) doesnotuntap`, and `keywordList` — which
  printed `[doesn't untap during its controller's untap step]` on those 102
  Humans in the seq-399 render — reads `card->basicAbilities[j]` directly, so the
  flag is on the affected creature), but the COUNT itself is asserted nowhere: no
  suite command reads the board header, so the only evidence that the number
  moves is the pure predicate plus that reasoning. `frozen >= 1` was taken from
  the sibling site at line 23112 and is not exercised at all.
- **O15's token detection is a LABEL test** (`token`/`creat` plus an `N/M` pair),
  not a read of the created token's card. A loyalty row that creates a
  non-creature token whose label happens to carry a P/T would be mis-tagged. The
  emblem count is every emblem the seat controls, not emblems from THIS walker —
  the claim is scoped to "a new emblem is added to them" precisely so that stays
  true, but it is a weaker statement than the review asked for.
- **O19 is the highest-risk change in the lane.** It reverses a validated wave-23
  ordering and touches the pick mapping, the narration indexing and the range
  collapse. The permutation and its inverse are pinned in PARSETEST as pure
  arithmetic, but no fixture and no game has run an ANNOUNCE_X window end to end
  on this tree. If the map is wrong, the seat announces the wrong X silently.
  This is the item to check first in the next corpus.
- **O21(a) and O21(d) are not fixed** (reasons above).
- Suite flakes: none fired this run, so I did not exercise the solo-rerun path.
