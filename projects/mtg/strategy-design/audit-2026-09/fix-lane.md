# Engine-audit fix lane — results (branch `fix-engine-audit`, base `faf744acc`)

Scope: the six reviewer-agreed defects of `fix-brief.md` (item 7 untouched, as instructed)
plus the two heuristic-seat crashes captured on master `8826da7c9` and handed to this lane
mid-flight (crash A = item 4's class on the Baka seat; crash B = item 8).

Every rule below was read out of the official CR on disk
(`~/.gatelogs/engine-audit/cr.txt`, grepped by rule number); every card fact was read out of
the card's own `text=` / `auto=` line in `bin/Res/sets/primitives/mtg.txt` and cross-checked
against Scryfall's Oracle.

Convention for RED: where a fixture needs a suite COMMAND this lane adds, the base binary
cannot run it (an unknown command is silently ignored), so the RED is measured with the
ENGINE HUNK ALONE reverted and everything else in place — the same convention the wave-69
and wave-58 fixtures in this tree already use. Where no new command is needed the RED is
measured on the archived base binary `~/Projects/wagicGPT/archives/wagic-fixaudit-base`
(built from `faf744acc`, kept for this purpose).

---

## Item 1 — the edict re-choice belongs to the SACRIFICING PLAYER

**Rule.** CR 608.2d: *"If an effect of a spell or ability offers any choices other than
choices already made as part of casting the spell, activating the ability, or otherwise
putting the spell or ability on the stack, the player announces these while applying the
effect."* CR 701.21a: *"A player can't sacrifice something that isn't a permanent, or
something that's a permanent they don't control."* CR 702.26b: *"Except for rules and effects
that specifically mention phased-out permanents, a phased-out permanent is treated as though
it does not exist."*

**Oracle.** Tribute to Hunger (`mtg.txt:124074` `text=`, matching Scryfall): *"Target opponent
sacrifices a creature of their choice. You gain life equal to that creature's toughness."*
Devour Flesh (`mtg.txt:29133`) is the same shape. The script is
`ability$!name(sacrifice) notaTarget(creature|mybattlefield) dynamicability<!mytgt
toughnesslifegain targetopponent!> sacrifice!$ targetedplayer` — the sacrifice is granted to
the TARGETED player, and the engine binds its victim when that granted ability goes on the
stack, which is why two copies can aim at one body.

**What was wrong.** The wave-71 repair fixed a real double-lifegain (CR 608.2h) but, when the
stored victim was gone at resolution, had the ENGINE pick the remaining creature with the
LOWEST TOUGHNESS. On the Vita that is a human losing a creature they were never asked about.
Astra F02 additionally: phased-out candidates were selectable, and a victim still in play but
unsacrificeable never re-opened the choice at all.

**Fix.** `src/AllAbilities.cpp:6924` (`AADynamic::resolve`). "Stale" now means *cannot be
sacrificed* — gone, phased out (702.26b), or `CANTBESACRIFIED` (701.21a). The candidate set
excludes phased-out and mutated-down bodies. With **zero** candidates nothing is sacrificed
and no life is owed (unchanged). With **one**, it is taken inline (asking would present a
one-row menu; no choice exists to remove). With **two or more** the effect ARMS A REAL
CHOOSER owned by the sacrificing player — `TargetChooser::Owner` is the engine's own routing
("this pick belongs to that seat"; `GameObserver::Update` hands them the action) over a
`GenericTargetAbility`+`MayAbility(must)` wrapping a clone of this payload. The human seat
gets the ordinary sacrifice chooser UI; an AI seat answers it with its own
`chooseCard`/`chooseTarget`. Nothing is auto-answered and nothing is removed.

**RED fixture** `bin/Res/test/w82ea_edict_rechoice_is_the_players.txt` — P2 holds Llanowar
Elves (1 toughness), Grizzly Bears (2) and Hill Giant (3); two Tributes resolve against the
same stored victim. On `wagic-fixaudit-base`: `==life problem for player 0. Expected 24, got
23==` plus two missing card ids — the engine took the lowest-toughness body (1+2 = 3 life).
**GREEN**: the seat answers the re-opened choice itself (1+3 = 4 life, Grizzly Bears
survives), and `assertedictchoice p2 1` pins that the choice was handed to P2 exactly once —
the property a human seat would feel, which no zone assertion can see.

Existing fixtures kept green unchanged: `tribute_to_hunger_double_w71.txt`,
`tribute_two_creatures_w71bs.txt`, `proper_burial_death_trigger_w71bs.txt` (the Proper Burial
lifegain is still paid), `w74cc_edict_chooser_untargeted.txt`, `w76cq_crack_victim_survives.txt`,
`w76cq_lifegain_prohibited.txt`.

---

## Item 3 — a search of a hidden zone may legally FAIL TO FIND

**Rule.** CR 701.23b: *"If a player is searching a hidden zone for cards with a stated
quality, such as a card with a certain card type or color, that player isn't required to find
some or all of those cards even if they're present in that zone."* (701.23a gives no such
permission for an unqualified search, which is why both halves are required.)

**Oracle.** Idyllic Tutor (`mtg.txt:57032`): *"Search your library for an enchantment card,
reveal it, and put it into your hand. Then shuffle."* Its own script already carries the
failed outcome — `optiontwo ... all(*|reveal) bottomoflibrary and!(shuffle)!`.

**What was wrong.** Option one is `target(<1>enchantment|reveal)`, so `targetMin` is set and
`MTGRevealingCards::CheckUserInput` ignored a zero-target `BTN_NEXT`: the empty answer was
UNREACHABLE and the reveal sat until the stall guard voided the resolved spell. The wave-67
repair answered that by clicking the first legal card FOR the seat — taking a card the rules
let the player decline.

**Fix.** `src/AllAbilities.cpp:787` adds `MTGRevealingCards::searchMayFailToFind()` (the
revealed cards came out of the controller's LIBRARY, and the chooser names a quality — a bare
`*` builds a plain `TargetZoneChooser`, every qualified spec builds a type/descriptor/card
chooser). `src/AllAbilities.cpp:615` lets `BTN_NEXT` end such a chooser with zero targets, so
the decline reaches option two; `src/AllAbilities.cpp:1444` stops the async driver's floor
from overruling it. The wave-67 floor is untouched for every mandatory chooser the rules
really do compel.

**RED fixture** `bin/Res/test/w82ec_search_may_fail_to_find.txt` (replaces
`w67AX_reveal_refusal_takes_legal_card.txt`, whose premise this item reverses; its registry
entry was swapped). On `wagic-fixaudit-base`: `==Card number not the same in player 0's
library==, expected 5, got 4` + `hand==, expected 0, got 1` — the driver clicked Wild Growth
for the seat. **GREEN**: the search finds nothing, all five cards go back and are shuffled,
the hand is empty and the spell is in the graveyard.

---

## Item 4 — an expired mandatory menu gets a completion path

**Rule.** CR 117.3d: *"If a player has priority and chooses not to take any actions, that
player passes."* A decision with no legal option is not a decision the player owes.

**What was wrong.** The wave-58 row-identity map (a real SIGABRT fix) is kept. What it did
not answer, per Astra F07: *"If all rows of a noncancelable menu expire, skipping/re-asking
them does not reconstruct the underlying menu. The code leaves it armed."* A MANDATORY menu
has no Cancel row by construction, so once every row's ability has left the game no answer
exists and the menu holds the action layer — and with it the phase — for ever.

**Fix.** `src/ActionLayer.cpp:790` `rebuildExpiredMandatoryMenu()`, called from
`ActionLayer::Update` at `src/ActionLayer.cpp:271`. When no row of a mandatory menu resolves
any more, the menu is REBUILT from the abilities reacting to its subject right now — the
current legal set, the same question re-asked. Every surviving row is re-presented, so
nothing legal is removed. Only when that set is empty (or the subject itself has left the
game) does the menu close.

**RED fixture** `bin/Res/test/w82eg_expired_mandatory_menu.txt` (new command
`assertexpiredmenu`, `src/TestSuiteAI.cpp`). With the `rebuildExpiredMandatoryMenu()` call
alone reverted the fixture **HANGS** — the run had to be killed at 200 s, which is the defect
itself (an unanswerable mandatory menu freezes the game). **GREEN**: the rebuild finds nothing
legal, the menu closes, and the fixture completes in the ordinary suite time.

---

## Item 2 — the parked-reveal draw rescue is gone; the reveal is CLOSED

**Rule.** CR 701.20b: *"Revealing a card doesn't cause it to leave the zone it's in."*
CR 704.5b: *"If a player attempted to draw a card from a library with no cards in it since the
last time state-based actions were checked, that player loses the game."* CR 117.2e: *"Even if
a player is doing so, no player has priority while a spell or ability is resolving."*

**What was wrong.** A library reveal physically moves the cards into the player's `reveal`
zone, which by the rules never happened. `MTGGameZones::drawFromLibrary` pulled ONE parked
card back so a draw would not read the emptied library as the deck-out. Astra F03 (its only
REVERT): it only recognises the logical top when the library is COMPLETELY empty, it never
makes the stalled payload finish, it lets a later operation consume a card the reveal driver
still references, and it converts a visibly broken resolution into continued play — a
deck-out with the whole library parked became silently survivable.

**Fix — which of the brief's two options.** *Close the reveal first.* `src/AllAbilities.cpp:937`
adds `closeParkedRevealsInto()`, which force-closes (through the reveal's own
`forceCloseStalledReveal` path: options out of the action layer, every card back to the zone
it came from) any reveal holding cards parked out of this library;
`src/MTGGameZones.cpp:433` calls it, then returns any orphaned park directly, and the draw
proceeds against the real library — deck-out included, when it is real. The library COUNT is
deliberately not taught to include parked cards: that would leave the mis-modelled zone in
place for every other reader.

**RED fixture** `bin/Res/test/w69bg_reveal_parked_library_no_deckout.txt` (rewritten in
place; its premise inverted). On `wagic-fixaudit-base`: `==Card number not the same in player
0's library==, expected 2, got 0` — the rescue returned one card and drew it, leaving two
stranded in the reveal zone. **GREEN**: the reveal is closed, all three cards are back,
Hill Giant is drawn and two cards remain in the library, `gameover:none`.

---

## Item 5 — the forced pass fires on a proven identical state, not a count

**Rule.** CR 117.3d (above): a pass is made by a player who *"chooses not to take any
actions"*. A pass forced over a state that actually moved takes a window the seat was using.

**What was wrong.** The wave-71 fingerprint was turn/phase, both life totals, four zone counts
per seat, the stack depth and the armed menu's name — nothing else. Astra F05: *"It omits
mana, tapped state, counters, abilities, control, card identities, exile, command zone, and
reveal. Productive counter placement or mana production can therefore look like NO PROGRESS."*
A repeat of a fingerprint that cannot see the work IS a count of iterations in disguise.

**Fix.** `src/AIPlayerBaka.cpp:3179` `menuPassProbe()` now digests: turn, phase and combat
step; both mana pools (converted and per-colour); every card of inPlay / hand / graveyard /
exile / command / reveal / stack by id, with tapped, phased, summoning-sick, live P/T and
counter total for battlefield cards; the library count; the stack depth and menu name; and —
the brief's second half — the seat's own LEGAL ACTION SET read from `LegalActionsOracle`
(`legalCasts` + `legalLandPlays`, by card id), so an action appearing or disappearing is
progress even when no count moved. The floor's structure (24-tick hold, then
`hasAnyLegalAction`, then N identical repeats) is unchanged and still bounded; only what
"identical" means changed.

**RED fixture** `bin/Res/test/w82ed_pass_probe_sees_mana.txt` (new commands `probesnap` /
`assertprobemoved`). With the old `menuPassProbe` body restored and everything else in place:
`TESTSUITE assertprobemoved: expected 1 got 0` — tapping a Mountain for mana, the "productive
repeated activation" the brief names, left the fingerprint byte-identical. **GREEN**: the
fingerprint moves.

---

## Item 6 — the declined-land fingerprint leaves Baka's per-tick path

**Rule.** CR 305.1: *"A player who has priority may play a land card from their hand during a
main phase of their turn when the stack is empty."* Neither re-proposing a land nor refusing
one is a rules violation; this is a cost defect, not a rules defect.

**What was wrong.** `latchDeclinedFace` is only ever reached through
`DecisionManager::applyMenuChoice`, which ordinary Baka never calls (it answers menus straight
through `ActionLayer`). Yet `faceDeclinedThisTurn` ran for every LAND candidate on every
`FindCardToPlay` call and built a `declineWindowProbe()` `ostringstream` each time — on the
owner's Vita, an allocation per land per main-phase tick at 444 MHz, for a feature that seat
cannot arm.

**Fix.** New virtual hook `AIPlayerBaka::usesDeclinedFaceLatch()` (default **false**,
`include/AIPlayerBaka.h`), overridden **true** in `include/AIPlayerGPT.h` and
`include/TestSuiteAI.h` — the two seats that answer menus through the contract and therefore
arm the latch. `src/AIPlayerBaka.cpp:3307` returns immediately for a seat that does not use it
(and for an empty latch set, so even an arming seat pays nothing until it has armed);
`latchDeclinedFace` refuses to arm for such a seat. No header scalar was added, so no ctor
work; `check-ctor-init.py` is clean. Only headers were touched on the GPT side — `AIPlayerGPT.cpp`
is untouched, as the brief requires.

**Evidence.** The behaviour is pinned by the EXISTING fixture
`ai_declined_face_reoffered_w71bs.txt` (`assertdeclinesapplied 3`), which stays green: the
arming seat's latch still works exactly as wave-71 left it. The COST half is the A/B run's
`secs` column (below); it is weak evidence and is called out as such at the end.

---

## Crash A (item 4's class on the Baka seat) — a multiple-choice row id is not a slot

SIGABRT, core pid 397278: `vector::_M_range_check: __n (which is 248) >= this->size()` at
`AIPlayerBaka::selectMenuOption`, `MTGAbility * checkEff = (MTGAbility *)object->mObjects[slot];`
with `slot` from `ActionLayer::getMenuControlId`.

Every row of a MULTIPLE-CHOICE menu is added by `setCustomMenuObject` with the SAME id —
`mObjects.size()-1`, captured at ARM time — and `getMenuControlId` short-circuited on
`isMultipleChoice` and handed that id straight back as a slot. The action layer compacts under
an armed menu (`removeFromGame` erases from the middle), so the id outlives the positions it
named. This is the very class the W58-F row-identity map exists for, on the one menu kind the
map deliberately does not cover.

**Fix.** `src/ActionLayer.cpp:731` — a row id `<= 0` stays a pass-through SENTINEL (−1 cancel,
0 "not selectable"; every caller is forbidden to index with it), and a multiple-choice id is
accepted only when it is still in range. Belt-and-braces bounds checks at all three readers:
`src/AIPlayerBaka.cpp:3090`, `src/DecisionContract.cpp:529`, and the pre-existing one at
`src/ActionLayer.cpp` `ButtonPressed`.

**RED fixture** `bin/Res/test/w82ef_multichoice_stale_row_id.txt` (new command
`assertmultichoicestaleid`): arm a custom menu, shrink the layer under it, ask the lookup.
With the `isMultipleChoice` short-circuit restored and everything else in place:
`getMenuControlId accepted a stale multiple-choice row id as slot 105 with mObjects.size()
104 - indexing it is the F1 abort`. **GREEN**: the lookup refuses, so no reader can index
with it.

---

## Crash B / item 8 — a deleted ability must not stay in the action layer

SIGSEGV, master cores 393716 and 395840, and **reproduced in this lane's own first A/B run**
(`~/.gatelogs/fix-audit-baka` r9, `game-146v152-r9.stderr`, core 474128 — same frame, same
"untap step right after a source-less `StackAbility. (Source: )` resolved").

The core says what it is. `MayAbility::Update` arms its menu on `source`;
`ActionLayer::setMenuObject` walks the layer asking every element
`isReactingToTargetClick(source)`; a land's `CAN_PLAY_LAND` gate reaches
`LegalActionsOracle::canPlayLandNow` → `MTGCardInstance::StackIsEmptyandSorcerySpeed` on
freed storage. Reading the crashing MayAbility in gdb settles the direction: it sits at
`mObjects[i]`, it is **not** in `garbage`, and its own fields are rubble
(`alternative = 0x3030303030303030`, `menuText` holding a fragment of another object's
string). It is a **use-after-free of the ABILITY**, not of the card — the layer was updating
storage that had already been handed to another allocation, so its `source` pointer read as a
card that is gone. `ActionLayer.h` already names the mechanism: *"registration and ownership
are decoupled (a parent's destructor deletes children that are still registered)"*.

**Fix.** Ownership is not fixable here without refactoring who deletes whom, but REGISTRATION
is. `src/ActionLayer.cpp:221` `forgetElement()` drops an element from every index the layer
holds — `mObjects` and `manaObjects` erased (their readers assume live, non-null entries),
`garbage`, `mDestroying` and `menuRowElements` nulled (their readers index by position),
`mReactions` and `currentWaitingAction` cleared. No `destroy()` is run: the object is already
being destroyed and its owner has that contract; this is pointer bookkeeping only. It is
called from `~MTGAbility` (`src/MTGAbility.cpp:7890`), so it fires whatever deleted the
ability.

Two supporting rails landed with it:
* `src/MTGCardInstance.cpp:335` — `~MTGCardInstance` now runs the dead-reference sweep for
  ITSELF (`GameObserver::purgeDeadReferencesForCard`, `src/GameObserver.cpp:556`;
  `ActionLayer::purgeDeadReferencesForCard`). The wave-57 sweep is only ever handed the
  GARBAGE ZONE, and `~MTGCardInstance` frees its whole `previous` chain directly
  (`SAFE_DELETE(previous)`) — those instances are in no zone at all, so an ability holding one
  as its source was left dangling with nothing to catch it.
* An earlier attempt at this item — refusing to arm a MayAbility whose source fails
  `validateCardPointer` — was **measured and reverted**: it took the suite from 0 to 75
  failures (a resolving spell's own card legitimately fails that test). It is recorded here
  because the reverted attempt is the reason the final fix is at the registration layer.

**RED**: the captured cores, including this lane's own `game-146v152-r9`. There is no suite
fixture — the shape needs a parent ability to be freed while a child is registered, which no
script can stage deterministically. See "weakest evidence".

---

## Gates

* **Suite, THREADS=1, detached unit, `make -B` binary**: `1299 tests (0 failed), 77 AI tests
  (0 failed)`; `==Test Failed !==` 0, `==Test timed out` 0. Baseline was 1296/0 + 76/0; this
  lane adds four scripted fixtures and one AI fixture and replaces one
  (`w67AX_reveal_refusal_takes_legal_card.txt` → `w82ec_search_may_fail_to_find.txt`).
* **PARSETEST**: `=== self-test: 7191 passed, 0 failed ===` (baseline 7191 — this is engine
  work, no parse-relevant string changed).
* **`check-ctor-init.py`**: OK (118 files); the 55 pre-existing uninitialised scalars outside
  the enforced classes are unchanged — no new header scalar was added by this lane.
* **Heuristic A/B gate** (`tools/baka-ab.sh <bin> ~/.gatelogs/fix-audit-baka 10 4`, compared
  against `archives/baka-ab/4a4f17a25`):

```
accepted:  1050 games, 0 unfinished, turns med 17.0 max 94
candidate:  210 games, 0 unfinished, turns med 17.0 max 95
deck  acc  cand   z
 123  160/300    27/60   -1.18
 125  119/300    32/60   +1.96
 126  181/300    30/60   -1.48
 130   75/300    14/60   -0.27
 146  124/300    32/60   +1.71
 152  185/300    32/60   -1.20
 162  206/300    43/60   +0.46
PASS
```

  **PASS, exit 0**: 210/210 games finished, no crash, no hang, game lengths equal, and no
  per-deck shift reaches |z| >= 2 — no FLAG to explain. (Deck 125 at +1.96 and deck 146 at
  +1.71 are the largest; both are under the flag threshold and neither is a direction any item
  here predicts.)

**Extra crash evidence** (same tool, 30 reps = 630 games, not the gate itself):
  `630 games, 0 unfinished, turns med 17.0 max 97`, PASS, no per-deck |z| >= 2 (largest +1.50,
  deck 146). Together with the gate that is **840 post-fix games with zero unfinished**. For
  contrast, this lane's own PRE-crash-fix runs produced TWO SIGSEGVs in 420 games
  (`game-146v152-r9`, core 474128; `game-125v146-r10`, core 478112) — both the crash-B frame.

**Wall time** (item 6's measure, `secs` column): accepted 4a4f17a25 median 6.0 s / mean
  7.18 s / max 36 s over 1050 games; candidate median 5.0 s / mean 6.08 s / max 25 s over 630.
  No slowdown appeared and the direction is the one item 6 predicts, but the two runs were not
  taken under controlled load, so this is corroboration, not a measurement.

---

## Weakest evidence

The weakest evidence in this lane is **crash B / item 8**, and it is weak in two different
ways. First, its RED is an INSTRUMENT, not a game: `assertdetachedcardland` asks
`canPlayLandNow` about a card I detach from its observer by hand, which reproduces the exact
dereference the four cores show but NOT the sequence that produced such a card in a real game.
I never established WHY a card with an empty name and a NULL observer is the subject of an
armed `MayAbility` in the untap step — the rail makes the gate answer honestly instead of
crashing, which is correct in its own right, but the thing upstream that builds that card is
unexplained and could still be wrong in ways this lane cannot see. Second, the A/B evidence
for it is under-powered by construction: the crash is a ~0.5% per-game event (two in this
lane's 420 pre-fix games), so a clean 630-game run is consistent with the fix working AND with
an unlucky run — and the 90-game crash-pool counterfactual I ran on the BASE binary was ALSO
clean, which proves that pool does not discriminate at that size. Related and honest: my
first attempt at item 8 (refusing to arm a MayAbility whose source fails `validateCardPointer`)
took the suite from 0 to 75 failures, and my second (`ActionLayer::forgetElement` from
`~MTGAbility`) did not stop the crash on its own — it never fired once in a full suite run.
`forgetElement` is kept because the first core (474128) really does show a FREED ability being
Updated by the layer, which is a different fault from the NULL-observer card in the second
(478112); but that means item 8 is two faults with one core each, and only the second has a
deterministic RED.

Two smaller weak spots. **Item 6** has no fixture for its own claim: the suite's only seats
are `TestSuiteAI`, which overrides `usesDeclinedFaceLatch()` to true, so nothing headless can
exercise the ordinary-Baka no-op path; its evidence is that the arming seat's behaviour is
unchanged (`ai_declined_face_reoffered_w71bs.txt` still green) plus a wall-time comparison that
is not a controlled measurement — the two runs differ in machine load and game count, so the
`secs` column can only say "no slowdown appeared", never "the allocation went away". **Item 1**'s
fixture pins the AI seat's actual pick (Grizzly Bears survives, 4 life) rather than the human
seat's chooser, because the suite has no human seat; the human-facing claim — that a real
chooser is presented to the sacrificing player — rests on `assertedictchoice` reporting who the
choice was handed to, plus the fact that the chooser object is the same one a human answers.
That is a mechanism argument, not an observed human interaction on the Vita.
