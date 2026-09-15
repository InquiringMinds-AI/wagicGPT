# Engine-audit fix lane 3 — the second review's five ranked items

Branch `fix-engine-audit-3`, base = master `f4f10a8d0` + the review commit `fdd6a134c`.
Source: `audit-2026-09/fix-review-2-astra.md` (verdict REWORK, five ranked items, two of
them defects the previous rework introduced). Scope is exactly those five; nothing else was
touched. Done in the order the brief gave: item 5 first, then 1, 2, 3, 4.

Every CR line was grepped out of `~/.gatelogs/engine-audit/cr.txt`; every card fact was read
off the card's own `text=`/`auto=` line.

**On RED.** Where the witness needs only pre-existing suite commands, RED is measured on the
base binary built from the base commit (kept at `projects/mtg/bin/wagic-base`). Where it needs
a command this lane adds, the base binary silently ignores that command, so RED is measured
with the ENGINE HUNK ALONE reverted and everything else in place — the convention the previous
two lanes used, stated again per item.

---

## 5. The abandoned lifegain is deleted (done first — trivial)

`AADynamic::resolve` allocates `mainAbility = NEW AALifer(...)` and hands it to
`activateMainAbility`. The `!sacrificed && isEdictShape()` branch that lane 2 introduced
returned before the ordinary `SAFE_DELETE(toActivate)`, and `~AADynamic` frees only
`storedAbility`, so the lifegain leaked on exactly the failure path that branch created.

**Fix** `src/AllAbilities.cpp:7355`: the branch clears the `mainAbility` member if it names the
same object and then deletes it. No behaviour change — the effect still pays no life.

**RED**: none. A leak has no runtime witness in this harness, and the review did not ask for
one ("Trivial; do it first"). The evidence is the code path itself: one allocation, one owner,
one exit that now frees it.

---

## 1. `mSweepingGarbage` no longer suppresses deregistration for other objects

**Defect (introduced by lane 2).** `GameObserver::Update` calls `cleanGarbage()` during
ordinary play, and a garbage entry's destructor recursively deletes abilities it OWNS —
`MayAbility::~MayAbility` does `SAFE_DELETE(ability)`, `GenericTargetAbility` the same. The
wave-83 flag was held across the WHOLE sweep, so `forgetElement` returned immediately for those
children too. A child still in `mObjects` left its freed address in the LIVE layer; a child
also occupying a LATER garbage slot was left there for the same sweep to delete a second time;
menu rows, mana producers and the waiting action were no longer cleared. That is precisely the
set of ownership/registration combinations the facility exists to close.

**Fix.**
* `src/ActionLayer.cpp:297` (`cleanGarbage`) records `mGarbageEntryBeingDeleted` around each
  `SAFE_DELETE`, and `src/ActionLayer.cpp:231` (`forgetElement`) exempts ONLY that object, and
  only from the `garbage` walk — its own slot is nulled by that very `SAFE_DELETE` and the
  vector is cleared straight after. That single walk was the whole Θ(G²) term. Every other
  container, and every other object including a recursively deleted child, is bookkept in full.
* The "unregistered temporary" short-circuit is now an O(1) per-element flag,
  `ActionElement::mEverRegistered` (`include/ActionElement.h:50`), set once by `GuiLayer::Add`
  (`src/GuiLayers.cpp:35`) and false on every clone. The review was right that the old
  all-containers-empty test is never true during play and so recognised nothing.

**RED fixture** `bin/Res/test/w84gb_garbage_child_deregistered.txt` (new command
`assertgarbagechildderegistered`): a child ability registered in the layer, a `MayAbility` that
owns it also registered, the parent moved to garbage, then the sweep — the engine's own
destructors, checked by pointer identity only, never a dereference. With the blanket flag
restored:

```
TESTSUITE assertgarbagechildderegistered: a garbage entry's recursively deleted child is
STILL registered at mObjects[106] - the layer is holding freed storage
```

**GREEN**: the child is gone from the layer before its storage can be reused. Also 630 heuristic
games with 0 unfinished (below).

---

## 2. The edict binds to the TARGETED PLAYER

**Rule.** CR 701.21a: *"To sacrifice a permanent, its controller moves it from the battlefield
directly to its owner's graveyard. A player can't sacrifice something that isn't a permanent,
or something that's a permanent they don't control."*

**Defect.** The gate took `owner = victim->controller()` and then checked that the victim was
controlled by that same player — circular, so "the targeted player no longer controls this
creature" could not be detected. A battlefield-to-battlefield control change keeps the same
object and moves only `lastController`, which is what `controller()` returns, so the effect
noticed nothing: the creature was sacrificed off the WRONG battlefield, and if it had become
unusable the replacement scan ran over the wrong player's creatures. Tribute's lifegain
recipient was derived from the victim's controller too.

**Fix** `src/AllAbilities.cpp:6970`: the seat is the granted ability's own controller.
`ability$!...!$ targetedplayer` is resolved by `ATargetedAbilityCreator`, which builds the
granted ability on a dummy card whose owner and `lastController` ARE the targeted player and
whose `storedSourceCard` is the granter; this ability's `source` is that dummy. The gate is now
a real question and answers false after a control change. `src/AllAbilities.cpp:7077` derives
the lifegain recipient from the ability (its dummy's `storedSourceCard`, else the dummy's
controller's opponent) instead of the victim's controller — scoped to the edict shape, so every
other `targetopponent` payload is untouched.

**RED fixture** `bin/Res/test/w84gc_edict_seat_is_the_targeted_player.txt` (new command
`givecontrol`, which does what the engine's own control change does): P1 casts Tribute at P2,
P2's seat pre-selects Craw Wurm, P1 then takes control of it before the ability resolves. On
`wagic-base`: `Card number not the same in player 0's battlefield, expected 1, got 0` — the
stolen Craw Wurm is sacrificed off P1's battlefield and P2 keeps Grizzly Bears.
**GREEN**: P2 sacrifices from its own battlefield, the stolen creature is untouched, and P1 —
whose spell it is — gains the life.

---

## 3. The forced pass needs a recorded refusal, not fingerprint equality

**Rule.** CR 117.3d: *"If a player has priority and chooses not to take any actions, that
player passes."*

**Three fixes.**
* **The floor now needs two independent facts** (`src/AIPlayerBaka.cpp:6012`): the state is
  identical (the fingerprint) AND the same set of NON-MANA rows has been on the table, untaken,
  for the whole run. The new `menuPassOfferedSet()` (`src/AIPlayerBaka.cpp:3330`) builds that
  set from the oracle — castable spells, land drops, and permanents with a usable non-mana
  activated ability. Mana abilities are excluded deliberately: a tappable land is available on
  every tick of every window and is not a row a seat "declines". The refusal is not inferred
  from the fingerprint: this code runs inside Act's empty-clickstream branch, after
  `computeActions` has had its turn, so by construction the seat queued nothing that tick, and
  a run of such ticks over an unchanged offered set is a recorded refusal of every row in it.
  An EMPTY offered set is never a refusal and never satisfies the arm.
* **Energy, experience and poison are in the digest** (`src/AIPlayerBaka.cpp:3208`).
  `AAAlterEnergy::resolve` moves `energyCount` without touching a zone, a card or a mana pool,
  which is exactly why a second energy gain read as "no progress".
* **The eligibility scan no longer calls a non-pure reader**
  (`src/AIPlayerBaka.cpp:3287`). `GenericTargetAbility::isReactingToClick` can allocate and
  replace the ability's chooser, so the wave-83 scan mutated the layer it was measuring on every
  floor tick. It now asks `LegalActionsOracle::usableAbilityCards`, the engine's own legality
  query, and records each usable ability's TARGET-SET SIZE, so an otherwise-unchanged ability
  whose legal targets grew or shrank counts as progress.

**RED fixture** `bin/Res/test/w82ed_pass_probe_sees_mana.txt`, extended with an energy step
(new command `addenergy`) and a purity step (`probesnap` then `assertprobemoved 0` — taking the
probe twice with nothing in between must give the same answer). With the energy/experience/
poison line removed:

```
TESTSUITE assertprobemoved: expected 1 got 0
```

**GREEN**: the fingerprint moves on mana, on a counter quantity, and on energy — and does not
move on itself.

**Not delivered:** a fixture that drives the FLOOR itself. The arm needs 24 hold ticks plus 200
identical ticks, which no scripted fixture can pump, so what is pinned is the two properties the
arm is built out of, not the arm firing. Called out again under weakest evidence.

---

## 4. The logical top of a selectively revealed library

**Rules.** CR 121.1: *"A player draws a card by putting the top card of their library into
their hand."* CR 701.20b: *"Revealing a card doesn't cause it to leave the zone it's in."*

**Defect.** "First parked out of this library" is the top only when the reveal parked a
contiguous run from the top. A selective reveal does not: `revealtype(dwarf|mylibrary)`
(Dwarven Recruiter, `mtg.txt:33376`) walks the library and parks only the Dwarves, leaving every
non-Dwarf where it was — including above the first parked Dwarf.

**Fix.** Each parked card records how many library cards were still above it at the moment it
was parked — `MTGCardInstance::mRevealAboveCount` (`include/MTGCardInstance.h:205`), stamped at
all three reveal move sites (`src/AllAbilities.cpp:317` for the selective walk, which runs
downward and removes as it goes so everything still above index `i` is exactly what stayed
behind; the top-N and reveal-until branches stamp 0). `MTGPlayerCards::drawFromLibrary`
(`src/MTGGameZones.cpp:426`) takes a parked card only when nothing is above it, and decrements
the others when a physical draw removes one of the cards above them — so a card parked under two
others does become the logical top after those two are drawn.

**The count-based reader is fixed too**: the empty-library branch now counts the cards parked
out of THIS library before declaring the CR 704.5b loss. A library that is full by the rules no
longer decks its owner out, which is the defect this whole thread started from.

**RED fixture** `bin/Res/test/w84gd_selective_reveal_logical_top.txt` (new test primitive
`Lexicon Selective Deadend Peek`, the Dwarven Recruiter shape with a dead-end option one): the
library is Craw Wurm / Dwarven Grunt / Grizzly Bears on top; the peek parks only the Grunt. On
`wagic-base` the parked Dwarf is drawn — `Card ID not the same. Didn't find 155` (Grizzly Bears)
and `library expected 1, got 2`. **GREEN**: Grizzly Bears is drawn and the Dwarf stays parked.
`w83fc_partial_library_logical_top.txt` and `w69bg_reveal_parked_library_no_deckout.txt` stay
green, so the contiguous-top behaviour is unchanged.

---

## Gates

* **Suite, THREADS=1, detached unit, `make -B` binary**: `1306 tests (0 failed), 80 AI tests
  (0 failed)`; `==Test Failed !==` 0, `==Test timed out` 0. Baseline 1304 + 79; this lane adds
  two scripted fixtures (`w84gb`, `w84gd`) and one AI fixture (`w84gc`).
* **PARSETEST**: `=== self-test: 7275 passed, 0 failed ===` (baseline 7275).
* **`check-ctor-init.py`**: OK (118 files); pre-existing debt unchanged at 55 — the one new
  scalar (`MTGCardInstance::mRevealAboveCount`) is assigned in both constructors and in
  `initMTGCI`. `ActionElement::mEverRegistered` is set in both of that class's constructors.
* **Encoding**: `git diff | grep -c U+FFFD` = 0.
* **Heuristic A/B gate**, `tools/baka-ab.sh <bin> ~/.gatelogs/fa3-baka 30 4` (630 games) vs
  `archives/baka-ab/4a4f17a25`:

```
accepted: 1050 games, 0 unfinished, turns med 17.0 max 94
candidate:  630 games, 0 unfinished, turns med 17.0 max 87
deck  acc  cand   z
 123  160/300    99/180  +0.35
 125  119/300    80/180  +1.03
 126  181/300   118/180  +1.14
 130   75/300    40/180  -0.69
 146  124/300    72/180  -0.29
 152  185/300   107/180  -0.48
 162  206/300   114/180  -1.20
PASS
```

---

## Weakest evidence

  **PASS, exit 0**: 630/630 games finished — no crash, no hang, no unfinished game — game
  lengths equal, and no per-deck shift reaches |z| >= 2, so there is no FLAG to explain. The
  largest is deck 126 at +1.14. Two items here can move heuristic play: item 4 changes which
  card a draw takes during a selective reveal, and item 3 makes the forced-pass floor STRICTER
  (it now needs a recorded refusal as well as an identical state), so the seat holds a window in
  cases where wave 83 would have passed. Neither moved a deck to the threshold, and the
  unchanged game-length distribution is the relevant signal for the stricter floor: making that
  arm harder to fire did not produce longer games.

---

## Weakest evidence

**Item 1's fixture proves the registration, not the lifetime.** `assertgarbagechildderegistered`
builds the exact ownership shape out of the engine's own destructors and shows the child's
address surviving in `mObjects` on the broken path — which is a real, deterministic
demonstration of the contract failure. It is not a demonstration that this shape occurs in a
real game: I did not find a card sequence that puts an owned child in the layer while its parent
sits in garbage, and the review did not claim one either ("I did not reproduce a concrete card
sequence or prove a new deterministic double-free"). So the argument for the fix is the contract
plus the 630 clean games, not an observed crash that stopped.

**Item 3's floor is still not exercised end to end.** The arm needs 24 hold ticks and then 200
identical ticks; no scripted fixture can pump that, so what is pinned is the two properties the
arm is built out of — the fingerprint moves on energy (and on mana, and on counter quantity) and
does not move on itself — not the arm declining to fire on a productive energy repeat. The
decline record itself is also an inference from position rather than an explicit per-row memo: I
rely on being inside Act's empty-clickstream branch, which means `computeActions` queued nothing
this tick, rather than on a recorded "declined row X" event per row. That is a weaker statement
than "a recorded decline of every non-mana row" reads, and it is the honest one.

**Item 4 is correct for the reveal paths, and only those.** `mRevealAboveCount` is stamped at
the three places `MTGRevealingCards::Update` parks cards, and decremented by physical draws. Any
OTHER operation that changes the library while cards are parked — a shuffle, a bottom-of-library
move, a library-order effect, a draw replacement — invalidates the recorded depth and nothing
re-derives it. The review named that class ("Full logical-library correctness during selective
reveals, reordering, shuffling and draw replacements") and this lane does not close it; it closes
the ordering error it demonstrated (a non-Dwarf above a parked Dwarf) and makes the deck-out
check read the logical library. Non-draw library readers other than the deck-out check still see
the physical zone.

**Item 2's fixture uses a synthetic control change.** `givecontrol` performs the same
battlefield-to-battlefield move with the same `lastController` update that the engine's own
control-change effects perform, and the RED is unambiguous, but it is not a real Act of Treason
resolving inside the granted ability's window — the timing of a scripted control change relative
to a stack resolution is the fixture's doing, not a card's. And the human half of item 2 remains
where the previous review left it: the routing is a mechanism argument, not a GUI observation.

**Item 5 has no runtime witness at all** — a leak is invisible to this harness. The evidence is
that the branch now has exactly one exit and it frees what the caller allocated.
