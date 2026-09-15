# Engine-audit fix lane 2 — the review's eight rework items

Branch `fix-engine-audit-2`, base = master `583ae53c8` + the review commit `5e618f379`.
Source: `audit-2026-09/fix-review-astra.md` (verdict REWORK, eight ranked items).
Worked in rank order. Every CR line below was grepped out of the official CR on disk
(`~/.gatelogs/engine-audit/cr.txt`); every card fact was read off the card's own
`text=`/`auto=` line in `bin/Res/sets/primitives/`.

**On RED.** Where a fixture needs only pre-existing suite commands, RED is measured on the
base binary built from `583ae53c8` (kept at `projects/mtg/bin/wagic-base`). Where the witness
needs a suite command this lane adds, the base binary cannot run it — an unknown command is
silently ignored — so RED is measured with the ENGINE HUNK ALONE reverted and everything else
in place. Item 1 needed a third form and says so.

---

## 1. The edict re-choice no longer leaves a targeting-protection bypass

**Rules.** CR 702.18a: *"Shroud is a static ability. 'Shroud' means 'This permanent or player
can't be the target of spells or abilities.'"* CR 702.11b: *"'Hexproof' on a permanent means
'This permanent can't be the target of spells or abilities your opponents control.'"*
CR 701.21a: *"To sacrifice a permanent, its controller moves it from the battlefield directly
to its owner's graveyard. A player can't sacrifice something that isn't a permanent, or
something that's a permanent they don't control."*

**Defect.** The wave-82 re-choice built its chooser with a SURVIVING CREATURE as the chooser's
card. `TargetChooser`'s constructor sets `targetter = card`, so the `targetter->bypassTC = true`
used to express "this chooser does not target" wrote to that creature, and clearing
`tc->targetter` afterwards does not undo it. `TargetChooser::canTarget` then returns
`targetter->bypassTC` for shroud, protection and opposing hexproof — a permanent bypass on a
permanent the player never sacrificed, outside the sacrifice effect entirely.

**Fix.** New `EdictSacrificeChooser` (`include/TargetChooser.h:348`,
`src/TargetChooser.cpp:2679`). It has NO `targetter` at all — a sacrifice is not targeting —
so no card is ever written to and the protection block is never consulted; its `source` stays
the EDICT's own card (a spell, never a candidate creature). `canTarget` answers eligibility
live from the sacrificing player's battlefield, and `targetsZone` answers the one zone a
sacrifice can come from directly instead of resolving "whose battlefield" from a source card
this chooser deliberately does not use. Wired in at `src/AllAbilities.cpp:6930`.

**RED — measured, not inferred.** On the heuristic seat the mutated card (`cands[0]`) is also
the card the heuristic sacrifices, so a scripted game cannot show a SURVIVING bypass; the
mutation is only observable at the moment the chooser is built. Measured on a build carrying
the wave-82 construction plus one trace line, on this fixture's exact board:

```
W83-FA-PROBE: after building the wave-82 re-choice chooser, cands[0]='Hill Giant' bypassTC=1
(it is the chooser's targetter and a card the player may keep)
```

**GREEN / standing pin** `bin/Res/test/w83fa_edict_no_protection_bypass.txt`: after a real
re-choice, `asserttargetbypass 0` for every one of P2's three creatures, plus
`assertcantarget 0 lexicon tough shroud witness via Prodigal Sorcerer` (an opposing targeted
ability) and `assertedictchoice p2 1`. It fails the moment any re-choice leaves a bypass on a
card that is still around. New suite command `asserttargetbypass` reads `MTGCardInstance::bypassTC`
directly — the field `TargetChooser` consults.

---

## 2. Only an actual SEARCH may fail to find

**Rules.** CR 701.23a: *"To search for a card in a zone, look at all cards in that zone (even
if it's a hidden zone) and find a card that matches the given description."* CR 701.23b gives
the fail-to-find permission to such a search *for cards with a stated quality*. CR 701.23d:
*"If a player is searching a hidden zone simply for a quantity of cards ... that player must
find that many cards."* CR 608.2c: *"The controller of the spell or ability follows its
instructions in the order written."*

**Defect.** The wave-82 predicate was "library origin + a qualified chooser". Mulch satisfies
it and is not a search — `mtg.txt:77098` *"Reveal the top four cards of your library. Put all
land cards revealed this way into your hand and the rest into your graveyard"*, scripted
`reveal:4 optionone target(<4>land|reveal) moveto(hand) ... optiontwo all(*|reveal)
moveto(mygraveyard)`. A seat could finalize with zero lands selected and send its own lands to
the graveyard. Sensation Gorger (`reveal:1`) is the same class.

**Fix.** `src/AllAbilities.cpp:807` (`searchMayFailToFind`) now requires three things: the
reveal LOOKED AT THE WHOLE ZONE, that zone is the player's own library, and the chooser names a
stated quality (a bare `*` builds a plain `TargetZoneChooser`, which is 701.23d's
quantity-only search and must find). The whole-zone test reads the script: `Reveal:type:<spec>:<zone>`
parks every matching card of the zone while `reveal:N` takes the top N, and the parser leaves
that form as the ability's first token, so the test is `number` starting with `type:`
(`src/AllAbilities.cpp:820`).

**RED fixture** `bin/Res/test/w83fb_mulch_is_not_a_search.txt` — on `wagic-base`
(583ae53c8): the empty answer is accepted, so nothing reaches the hand and the reveal is voided
back to the library (`graveyard expected 4, got 1`; `library expected 1, got 5`;
`hand expected 1, got 0`). **GREEN**: the mandatory floor commits a legal land and Forest
reaches the hand. `w82ec_search_may_fail_to_find.txt` (Idyllic Tutor) stays green, so the
permission the rules DO give is not lost.

---

## 3. A draw never closes a live reveal; the partial-library case is fixed

**Rules.** CR 701.20b: *"Revealing a card doesn't cause it to leave the zone it's in."*
CR 117.2e: *"Resolving spells and abilities may instruct players to make choices or take
actions ... Even if a player is doing so, no player has priority while a spell or ability is
resolving."* CR 704.5b: the empty-library loss.

**Defect.** The wave-82 helper force-closed EVERY reveal sharing the drawing player's reveal
zone, with no test of stalledness, pending choice, or ownership. Its premise — no priority
during resolution, therefore nothing should draw mid-reveal — misreads 117.2e, which forbids
PRIORITY, not instructed draws: Sensation Gorger (`mtg.txt:102263`) runs `discard:20 && draw:4`
INSIDE reveal option one. And it only looked when the physical library was COMPLETELY empty,
so a partial library still skipped its own logical top.

**Fix — which of the two options the brief offered.** *Logical-zone semantics.*
`src/MTGGameZones.cpp:435`: a draw takes the library's logical top, which is the first card
parked out of THIS library (a reveal takes the top first), whether or not the physical library
is empty. Nothing is closed from the draw path; the helper is deleted
(`src/AllAbilities.cpp`, the `#W83-FC` note where it stood). The only remaining close path is
the reveal driver's own stall guard, `forceCloseStalledReveal` — the only code that can prove a
reveal made no progress.

**RED fixtures.**
* `bin/Res/test/w83fc_partial_library_logical_top.txt` (new test primitive `Lexicon Top Card
  Deadend Peek`, `reveal:1` with a dead-end option one): on `wagic-base`, the library is not
  empty so the parked card is ignored and the draw takes the physically top card —
  `Card ID not the same. Didn't find 205` (Hill Giant, the logical top), `library expected 4,
  got 3`. **GREEN**: Hill Giant is drawn.
* `bin/Res/test/w69bg_reveal_parked_library_no_deckout.txt` (rewritten): the two cards NOT
  drawn are asserted to be **still parked in the reveal zone** (`reveal:*,*`), which is only
  true if the reveal was left alone. On `wagic-base` the force-close returns all three and
  kills the reveal.

**Not delivered:** a Sensation Gorger fixture. Its Kinship `@each my upkeep` trigger did not
fire in any scripted arrangement I could build (three attempts from `untap` with `next` and
filler ticks), so the "an instructed draw inside option one must not cancel the effect"
scenario is covered only by the two fixtures above plus the deletion of the close path. Called
out again under weakest evidence.

---

## 4. A multiple-choice answer survives compaction

**Defect.** Every row of a multiple-choice menu is added by `setCustomMenuObject` with the SAME
id — `mObjects.size()-1`, captured at arm time — and the layer compacts under an armed menu.
The wave-82 repair bounds-checked that id and REFUSED the row, so `doReactTo` returned without
ever reaching `ButtonPressedOnMultipleChoice` and every legal mode of a live `MenuAbility`
became unreachable to the AI and to a script, while the human's `ButtonPressed` (which
dispatches multiple choice BEFORE slot resolution) kept working.

**Fix.** `src/ActionLayer.cpp:772` and the contract comment above it separate the four things
that rode one integer: ROW IDENTITY (`menuRowElements`, the only safe identity),
LAYER SLOT (the only value a caller may index with), MODE INDEX (what a multiple-choice row
means — its own position, which no compaction can invalidate) and the SENTINELS. A
multiple-choice row now answers `true` with `slot = kMenuRowIsMode` (`ActionLayer.h`), a value
no reader can mistake for an index; `closeUnanswerableMandatoryMenu` leaves such menus alone
because their rows are always answerable. The comment also records, honestly, that
`ButtonPressed`'s ordinary branch does accept slot 0 as a layer index and always has — the
wave-82 claim that every caller forbids it was false.

**RED fixture** `bin/Res/test/w82ef_multichoice_stale_row_id.txt`, with the
`getMenuControlId` hunk reverted to the wave-82 form:

```
TESTSUITE assertmultichoicestaleid: getMenuControlId REFUSED a live multiple-choice row after
the layer compacted - doReactTo returns without ever reaching ButtonPressedOnMultipleChoice,
so the menu's legal modes are unreachable
```

and with the hunk reverted further, to pre-wave-82: `getMenuControlId accepted a stale
multiple-choice row id as slot 105 with mObjects.size() 104 - indexing it is the F1 abort`.
**GREEN**: the row is answerable AND carries no usable index — both halves asserted.

**Not delivered as asked:** the review wanted the fixture to *select a surviving real mode
after compaction*. I built that fixture (Heartless Act's modal menu + a new
`shrinkactionlayer` command) and could not make it discriminate: the scripted `choice 0` did
not reach `doReactTo` at all on either binary once the layer had been shrunk, so it failed on
the fixed build and passed on base — the opposite of the property under test, and evidence
that something else in the scripted modal path was being disturbed rather than the row
lookup. Rather than ship a fixture whose signal I could not explain, the pin is the direct
lookup assertion above. `shrinkactionlayer` is kept (it is the only way to compact a layer
under an armed menu from a script) and is used by the fixture that remains.

---

## 5. The edict is a valid resolution-time choice, not a stamp

**Rules.** CR 608.2d (the choice is announced by the player while the effect is applied, and
*"The player can't choose an option that's illegal or impossible"*); CR 701.21a (what may be
sacrificed); CR 702.26b (a phased-out permanent *"is treated as though it does not exist"*).

**Five defects, five fixes** (`src/AllAbilities.cpp:6930`):
* *The gate and the offered set disagreed.* `EdictSacrificeChooser::canTarget` is now the ONLY
  eligibility predicate — it answers the stale-victim gate (`cands` is built by calling it),
  it filters the candidate scan, and it is the chooser the player answers. The
  `creature|mybattlefield` chooser that admitted CANTBESACRIFIED permanents and mutated-down
  components is gone.
* *The single-victim predicate omitted the mutation exclusion.* Same fix: there is one
  predicate now, and it carries the mutation test.
* *`activateStored()` ignored the sacrifice result.* `src/AllAbilities.cpp:7279`:
  `activateStored` returns what the stored ability did, and the lifegain is skipped when the
  sacrifice took nothing. Scoped by `AADynamic::isEdictShape()`
  (`src/AllAbilities.cpp:7266`) — one place that says what an edict is, so the guard and this
  rail cannot drift apart.
* *The autopick fallback.* Deleted. If the chooser cannot be built the effect does nothing
  rather than choosing for the player.
* *A new response window.* `choose->canBeInterrupted = false` (and the same on the `MayAbility`
  wrapper), so `MTGAbility::fireAbility` resolves the answer in place instead of adding
  another stack object.
* *The witness proved an intention.* `gEdictRechoiceArmed` is now stamped from the FAR SIDE of
  `addToGame()`, after checking the ability is registered in the action layer and the chooser's
  `Owner` is the sacrificing seat; `assertedictchoice` requires it.

**Fixtures.** `w83fa_edict_no_protection_bypass.txt` carries `assertedictchoice p2 1` (armed in
layer). `w83fa_edict_unsacrificeable_never_pays.txt` is a standing INVARIANT pin: P2's only
creature is `Lexicon Unsacrificeable Witness` and no life may be paid. **It is not RED on base**
— the heuristic seat declines to activate the granted ability at all when its only candidate is
unsacrificeable, so the wave-82 life-for-nothing path is not reachable from a scripted game;
it is kept because it fails immediately if the eligibility or the sacrifice-result rail
regresses. Said again under weakest evidence.

---

## 6. The expired decision is closed, never rebuilt

**Rules.** CR 117.3d: *"If a player has priority and chooses not to take any actions, that
player passes."*

**Defect.** `setMenuObject` gathers every ability reacting to the subject NOW and keeps the
triggered/free rows if there are any, otherwise the ordinary cost-bearing ones. So the wave-82
rebuild could re-arm an expired mandatory TRIGGER as "activate this other ability", WITH NO
CANCEL ROW — a voluntary action turned into one the player cannot decline. "Same card" is not
"same decision", and 117.3d does not justify it.

**Fix.** `src/ActionLayer.cpp:843`, `closeUnanswerableMandatoryMenu` (renamed from
`rebuildExpiredMandatoryMenu`; called at `src/ActionLayer.cpp:356`). A mandatory menu whose rows
have all left the game is CLOSED and nothing is put in its place. That removes nothing — the
rows were already gone — and it releases the layer, which is the completion path Astra F07
asked for. A row counts as an answer only if it resolves to an ability the layer still holds
(`slot > 0`), so the wave-82 conflation of "lookup succeeded" with "answerable" is gone too.

**RED fixture** `bin/Res/test/w82eg_expired_mandatory_menu.txt`, extended with the review's
own scenario: after the menu's rows expire the card is GRANTED a fresh voluntary `{0}:`
activation (the shape a permanent has when an expired trigger leaves it holding an ordinary
ability). With the rebuild restored:

```
TESTSUITE assertexpiredmenu: 'w58 stale menu witness' REBUILT a mandatory menu after its own
rows expired (1 answerable row(s), cantCancel=1) - a voluntary activation must not become a
decision the player cannot decline
```

**GREEN**: the menu closes. With the whole close reverted the fixture HANGS (measured in lane
1: killed at 200 s), which is the defect the close exists for — so both directions are pinned.

---

## 7. Destructor-driven deregistration is bounded and validated

**Defect (a) — retry without progress.** `purgeDeadReferencesForCard` set `again = true` whether
or not `moveToGarbage` succeeded, and `removeFromGame` returns false for an element already in
`mDestroying`: a re-entrant sweep meeting such an element retried it for ever.
**Fix** `src/ActionLayer.cpp:179`: the walk restarts only when an element actually LEFT; one
that refuses is traced and skipped.

**Defect (b) — quadratic cleanup.** `cleanGarbage()` deletes G entries and every `~MTGAbility`
re-walked the whole `garbage` vector: Θ(G²) on a shared path, on a 444 MHz Vita.
**Fix** `src/ActionLayer.cpp:244`: a `mSweepingGarbage` flag (initialised in the ctor;
`check-ctor-init` clean) makes `forgetElement` a no-op for the duration of the sweep — nothing
that survives a wholesale emptying can name an entry of it — and an early return covers the
common case of an ability that was never registered at all (every parse-time template and every
clone deleted without being added).

**Not fixed.** The review's third observation — that erasing from `mObjects` inside a destructor
changes the behaviour of active indexed traversals — stands. The forward `Update` loop can skip
one entry for one tick if an element earlier in the vector is deleted during it. I did not
change that: the alternative (nulling the slot) needs every `mObjects[i]` reader in the engine
to be null-safe, and `setMenuObject` for one dereferences without a check. Recorded as a known
residual, not repaired.

---

## 8. The pass fingerprint reads the fields it claimed to

**Defect.** Three fields were misread. `Counters::mCount` counts counter RECORDS, not
QUANTITY — `Counters::addCounter` increments `Counter::nb` on an existing record — so a second
counter of an existing kind was invisible. `MTGCard::getId()` is the PRINTING's mtgid, so two
copies of a card were indistinguishable. And the legal-action set covered casts and land plays
but not ACTIVATED abilities.

**Fix** `src/AIPlayerBaka.cpp` (`menuPassProbe`, the `#W83-FG` notes at :3224 and below): each
counter record's name, power/toughness and `nb`; the instance POINTER instead of the printing
id; and the set of abilities that answer `isReactingToClick` for this seat right now, read off
the action layer. Built only on ticks already at the hold floor, so the ordinary tick pays
nothing.

**RED fixture** `bin/Res/test/w82ed_pass_probe_sees_mana.txt`, extended with a counter step
(new suite command `addcounter`, which adds a NAMED 0/0 `charge` counter so no power or
toughness moves). With the wave-82 `menuPassProbe` body restored:

```
TESTSUITE addcounter: Grizzly Bears now carries 1 counter record(s), charge x1
TESTSUITE addcounter: Grizzly Bears now carries 1 counter record(s), charge x2
TESTSUITE assertprobemoved: expected 1 got 0
```

— the record count is unchanged, the quantity doubled, and the fingerprint did not move.
**GREEN**: it moves, on the mana step and on the counter step.

**Not claimed.** This is a fingerprint, not a proof of total state equality, and the honest
claim is bounded by that. It now moves on every input the review named; the floor remains a
last resort behind a 24-tick hold and a 200-tick identical run; and repeated equality still
overrides `hasAnyLegalAction`, which is the design the original brief asked for.

---

## Gates

* **Suite, THREADS=1, detached unit, `make -B` binary**: `1304 tests (0 failed), 79 AI tests
  (0 failed)`; `==Test Failed !==` 0, `==Test timed out` 0. Base in this worktree is 1302 + 77;
  this lane adds two scripted fixtures (`w83fb`, `w83fc`) and two AI fixtures
  (`w83fa_edict_no_protection_bypass`, `w83fa_edict_unsacrificeable_never_pays`).
* **PARSETEST**: `=== self-test: 7275 passed, 0 failed ===` (baseline 7275 — engine work, no
  parse-relevant string changed).
* **`check-ctor-init.py`**: OK (118 files); the 55 pre-existing uninitialised scalars are
  unchanged. The one header scalar this lane adds (`ActionLayer::mSweepingGarbage`) is
  initialised in the constructor.
* **Encoding**: `git diff | grep -c U+FFFD` = 0. `GameObserver.cpp` and `MTGCardInstance.cpp`
  were not touched by this lane; `MTGGameZones.cpp` was byte-spliced.
* **Heurisic A/B gate**, `tools/baka-ab.sh <bin> ~/.gatelogs/fa2-baka 30 4` (630 games) vs
  `archives/baka-ab/4a4f17a25`:

```
accepted: 1050 games, 0 unfinished, turns med 17.0 max 94
candidate:  630 games, 0 unfinished, turns med 17.0 max 87
deck  acc  cand   z
 123  160/300    97/180  +0.12
 125  119/300    86/180  +1.74
 126  181/300   103/180  -0.67
 130   75/300    38/180  -0.97
 146  124/300    77/180  +0.31
 152  185/300   107/180  -0.48
 162  206/300   122/180  -0.20
PASS
```

---

## Weakest evidence

  **PASS, exit 0**: 630/630 games finished — no crash, no hang, no unfinished game — game
  lengths equal, and no per-deck shift reaches |z| >= 2, so there is no FLAG to explain. Deck
  125 at +1.74 is the largest and is the deck whose win rate also drifted up in lane 1's runs;
  it is under the threshold and no item here predicts a play change on it. Two items in this
  lane DO change heuristic play by design and are worth naming against that table: item 3 (a
  draw now takes a parked revealed card, which can change which card a seat draws in any game
  with a library reveal) and item 5 (an edict pays no life when its sacrifice did not happen).
  Neither moved any deck to the flag threshold over 630 games.

---

## Weakest evidence

Three places where what I shipped is stronger than what I proved.

**Item 1's RED is an instrumented build, not a fixture, and the reason matters.** On the
heuristic seat the mutated card (`cands[0]`) is always also the card the heuristic sacrifices,
so no scripted game can show a SURVIVING shroud bypass — I tried seven board arrangements and
the heuristic's sacrifice preference is not a stable function of power, toughness or zone
order (it took the highest toughness on one board, the lowest power on another, and on a third
it declined to activate at all), so I could not steer it into keeping the mutated creature. The
mutation is real and measured (`bypassTC=1` on Hill Giant, printed from the wave-82
construction), and the rules consequence follows from `TargetChooser::canTarget` returning
`targetter->bypassTC` — but the consequence itself was never observed in a game, and the human
seat where the review says it bites is exactly the seat the suite cannot drive. What ships as a
standing pin is an invariant sweep, which would catch a re-introduction but did not catch this
one on its own.

**Item 4's fixture does not select a mode, which is what the review asked for.** I built the
Heartless Act fixture with a new `shrinkactionlayer` command and it produced the WRONG signal:
after the layer was compacted the scripted `choice 0` never reached `doReactTo` on either
binary, so the fixture failed on the fixed build and passed on base. That is the opposite of
the property under test, and I could not explain it — something else in the scripted modal path
is disturbed by compaction, and it may be a real defect this lane has not found. I dropped the
fixture rather than ship a green I could not account for, and pinned item 4 with a direct
lookup assertion instead. So the claim "an AI's legal answer is reachable again" rests on the
lookup contract plus a code reading of `doReactTo`, not on an answered mode.

**Two fixtures are invariant pins, not REDs.** `w83fa_edict_unsacrificeable_never_pays.txt`
passes on base too, because the heuristic declines to activate the granted ability when its
only candidate cannot be sacrificed — so the wave-82 "life for a sacrifice that never happened"
path is unreachable from a scripted game and the sacrifice-result rail (item 5) has no measured
RED at all, only a code-level one. And item 3's Sensation Gorger case — the review's own
counterexample, an instructed draw INSIDE a live reveal — has no fixture: its Kinship upkeep
trigger did not fire in any scripted arrangement I could build, so what is pinned is the
logical-top draw and the fact that the untouched cards stay parked, not the specific
`discard:20 && draw:4` sequence the review named.

Smaller residuals, recorded rather than repaired: item 7's third observation (erasing from
`mObjects` inside a destructor can make the forward `Update` loop skip one entry for one tick)
is unfixed, because the alternative needs every `mObjects[i]` reader to be null-safe and
`setMenuObject` is not; and item 8 remains a fingerprint, so equality is still not a proof of
identical state — it now moves on every input the review named, and nothing more than that is
claimed.
