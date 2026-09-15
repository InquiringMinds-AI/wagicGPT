# Engine-audit fix lane 4 — the third review's three items

Branch `fix-engine-audit-4`, base = master `fc449bee6` + the review commit `d6d084b69`.
Source: `audit-2026-09/fix-review-3-fable.md` (verdict REWORK; items E14, E6(b), E6(a)).
Scope is exactly those three; nothing else was touched. Item 1 was done first, as instructed.

Every CR line was grepped out of `~/.gatelogs/engine-audit/cr.txt`; every card fact was read off
the card's own `text=`/`auto=` line.

**On RED.** Where the witness needs only pre-existing suite commands, RED is measured on the
base binary (`projects/mtg/bin/wagic-base`). Where it needs a command this lane adds, the base
binary silently ignores that command, so RED is measured with the ENGINE HUNK ALONE reverted and
everything else in place — the convention of the previous three lanes, stated per item.

---

## 1. The forced-pass floor can still fire when the offered set is empty (hang risk, done first)

**Rule.** CR 117.3d: *"If a player has priority and chooses not to take any actions, that player
passes."*

**Defect (introduced by lane 3).** `stalled = sameState && sameRefusal`, and `sameRefusal` is
false by construction whenever `menuPassOfferedSet()` is empty. The hold arm below it keeps the
phase for as long as `LegalActionsOracle::hasAnyLegalAction` is true and the tick is not
stalled — and those two predicates disagree by construction. `hasAnyLegalAction` answers TRUE
for a cleanup discard, a declare-attackers window with a legal attacker, a declare-blockers
window with a legal block, the blocker-ORDER step, and casts priced on POTENTIAL mana;
`menuPassOfferedSet` prices casts on the current mana POOL and carries no combat or cleanup rows
at all. In any of those windows the wave-58/71 shape — a menu that re-arms and is answered every
tick — held the phase FOREVER: wave 83 broke it after 24 + 200 identical ticks, lane 3 never
did. It sits on the shared `AIPlayerBaka::Act` path, which `AIPlayerGPT` does not override, so
the model seat inherited it too.

**Fix.** The reviewer's one-liner, expressed as a pure predicate so it can be pinned without a
224-tick game: `AIPlayerBaka::menuPassStalled` (`include/AIPlayerBaka.h:290`) returns
`sameState && (offeredEmpty || sameRefusal)`, and the floor calls it
(`src/AIPlayerBaka.cpp:6055`). An identical state is required always; the REFUSAL evidence is
required only where there was something to refuse. The lane-3 comment calling the offered set a
"recorded refusal" is corrected in place — it is a filter on where refusal evidence is needed.

**RED fixtures**, both new, one per window the reviewer named:
* `bin/Res/test/w85ha_pass_floor_cleanup_window.txt` — cleanup, eight uncastable cards in hand,
  no mana.
* `bin/Res/test/w85ha_pass_floor_blockers_window.txt` — P1 attacks with Rootbreaker Wurm, P2 has
  an untapped Alpha Myr that could block; the assertion is taken on P2's seat in its blockers
  window.

No fixture can pump the 24 + 200 ticks the arm needs, so the new command
`assertpassfloorreachable` (`src/TestSuiteAI.cpp:972`) asserts the arm's own predicate against a
REAL board in exactly such a window — the technique `assertloopautopass` already uses for the
auto-pass gate — and it fails LOUDLY if the window does not actually exercise the disagreement,
so it cannot pass vacuously. With the predicate reverted to the lane-3 form:

```
TESTSUITE assertpassfloorreachable: player 1 expected 1 got 0 (hasAnyLegalAction=1,
offered rows=none) - with a legal action and no offered row, a floor that cannot fire holds
the phase for ever          [w85ha_pass_floor_cleanup_window.txt]
TESTSUITE assertpassfloorreachable: player 2 expected 1 got 0 (hasAnyLegalAction=1,
offered rows=none) ...       [w85ha_pass_floor_blockers_window.txt]
```

**GREEN**: reachable in both windows, with the disagreement confirmed present in both.

---

## 2. Parked-card eligibility is derived from the CURRENT library

**Rules.** CR 121.1: *"A player draws a card by putting the top card of their library into their
hand."* CR 701.20b: *"Revealing a card doesn't cause it to leave the zone it's in."* CR 704.5b:
*"If a player attempted to draw a card from a library with no cards in it since the last time
state-based actions were checked, that player loses the game."*

**Defect (introduced by lane 3).** The depth was a COUNT that only a physical draw decremented.
Any other removal of the cards above — a mill, a tutor, a `bottomoflibrary`, a shuffle — left it
stuck above zero; once the physical library was empty the draw returned having drawn NOTHING,
every draw step, and never decked. The library is logically non-empty in that state, so the
correct outcome is the draw, not a skipped draw and not a loss.

**Fix** `src/MTGGameZones.cpp:427` and `src/AllAbilities.cpp:321`: the depth is not stored at
all. Each parked card carries the IDENTITIES of the library cards the reveal left above it
(`MTGCardInstance::mRevealAbove`, `include/MTGCardInstance.h:214`), recorded by the selective
walk as the cards it skips, and empty for the top-N and reveal-until walks which leave nothing
above. Eligibility is re-derived on every draw: a parked card is drawable when none of those is
still in the library zone — pointer identity only, never a dereference. That is self-correcting
under every removal route, and it collapses the two predicates lane 3 had (the logical-top test
and the deck-out suppression) into ONE: if anything is parked out of this library when the
physical zone is empty, it is drawable by construction and has already been drawn, so the
deck-out branch needs no special case and is back to its original form.

**RED fixture** `bin/Res/test/w85hb_mill_above_parked_card.txt` (new command `millcard`, which
takes cards off the top of a library without drawing them — the removal route a count cannot
see): the selective peek parks the Dwarf under Grizzly Bears, the Bears are milled, then a draw.
With the staleness restored (anything recorded above blocks for ever, whatever became of it):

```
==Card number not the same in player 0's library==, expected 1, got 0
==Card ID not the same. Didn't find 146==   (Craw Wurm - drawn from UNDER the parked Dwarf)
==Card ID not the same. Didn't find 29760== (Dwarven Grunt - never eligible, never drawn)
```

**GREEN**: Dwarven Grunt is drawn and Craw Wurm stays in the library. `w84gd`, `w83fc` and
`w69bg` stay green, so the cases lane 3 fixed are unchanged.

---

## 3. Unstamped park sites — discharged by the item-2 derivation

The review's minimal fix was "stamp 0 at `MTGScryCards::initDisplay` and treat `< 0` as eligible
in the draw". The brief preferred the derivation from item 2 if it made the field unnecessary,
and it does: **there is no stamp to miss.** A card parked by a route that records nothing — the
scry display, a `moveto(myreveal)` mover — has an EMPTY `mRevealAbove`, which means "nothing
known to be above it", so it is eligible. That is the wave-83 behaviour the review asked to be
restored, it is the right default rather than a special case, and it removes the asymmetry the
review flagged as E6(c) (a card that could never be drawn yet still suppressed the deck-out),
because the suppression no longer exists.

No fixture. I tried to build one — a test-only sorcery moving a library card to the reveal zone
by the mover route — and could not get the card to move (`library 1, reveal 0`; the script
resolved without doing anything), so I removed it rather than ship a witness that passes for the
wrong reason. The review itself found the shipped exposure to be `aicode`-only lines plus every
future script, and found no human-reachable one. Called out under weakest evidence.

---

## Gates

* **Suite, THREADS=1, detached unit, `make -B` binary**: `1309 tests (0 failed), 80 AI tests
  (0 failed)`; `==Test Failed !==` 0, `==Test timed out` 0. Baseline 1306 + 80; this lane adds
  three scripted fixtures.
* **PARSETEST**: `=== self-test: 7275 passed, 0 failed ===` (baseline 7275).
* **`check-ctor-init.py`**: OK (118 files); pre-existing debt unchanged at 55. The scalar this
  lane removes (`mRevealAboveCount`) was initialised; its replacement is a vector, which the
  checker does not track and which is cleared at every park site and in `initMTGCI`.
* **Encoding**: `git diff | grep -c U+FFFD` = 0.
* **Heuristic A/B gate**, `tools/baka-ab.sh <bin> ~/.gatelogs/fa4-baka 30 4` (630 games) vs
  `archives/baka-ab/4a4f17a25`:

```
accepted: 1050 games, 0 unfinished, turns med 17.0 max 94
candidate:  630 games, 0 unfinished, turns med 17.0 max 84
deck  acc  cand   z
 123  160/300    89/180  -0.83
 125  119/300    69/180  -0.29
 126  181/300   116/180  +0.90
 130   75/300    40/180  -0.69
 146  124/300    70/180  -0.53
 152  185/300   115/180  +0.49
 162  206/300   131/180  +0.95
PASS
```

---

## Weakest evidence

  **PASS, exit 0**: 630/630 games finished — no crash, no hang, no unfinished game — game
  lengths equal, and no per-deck shift reaches |z| >= 2, so there is no FLAG to explain. The
  largest is deck 162 at +0.95, the tightest spread of the four lanes. Item 1 makes the floor
  EASIER to fire than lane 3 had it (back to the wave-83 condition in windows with no offered
  row), and item 2 can change which card a draw takes while a reveal is parked; neither moved a
  deck to the threshold, and the unchanged max turn count is the relevant signal for a
  livelock fix.

---

## Weakest evidence

**Item 1's fixtures assert the arm's predicate, not the arm firing.** The floor needs 24 hold
ticks and then 200 identical ticks, which no scripted fixture can pump — the same limit lane 3
reported. So what is pinned is: in a cleanup-discard window and in a declare-blockers window on
a real board, `hasAnyLegalAction` is TRUE, the non-mana offered set is EMPTY, and the arm's own
predicate now answers "can fire" where the lane-3 form answered "cannot". That is the whole of
the reviewer's mechanism, measured in both windows he named, and the command refuses to pass
vacuously — but it is not a game in which a seat that would have hung is observed to recover.
Nobody in four lanes has produced the re-arming-menu shape on demand, and the 630-game A/B saw
none, so the hang this removes remains a mechanism, not an observation.

**Item 2's witness uses a synthetic mill.** `millcard` takes cards off the library top through
the engine's own `putInZone`, which is what a mill, a tutor or a `bottomoflibrary` does to the
zone, but it is not a shipped card resolving. The review could not name a concrete shipped
sequence either (a parked reveal plus a non-draw library removal plus an empty physical
library), so the defect and its fix are both established at the mechanism level. The derivation
itself has a residual I should name: `mRevealAbove` holds raw card pointers compared against the
live library, so a freed instance whose address is later reused by a card that IS in the library
would read as "still above" and hold the parked card down for one draw. The reveal's lifetime is
short and the comparison never dereferences, but it is an identity assumption, not a proof.

**Item 3 has no witness at all.** I could not get a library card into the reveal zone by a
non-reveal route from a script — a test-only `moveto(myreveal)` sorcery resolved without moving
anything — so the claim that an unstamped park is now eligible rests on reading the code: the
empty-set case is the same branch the recorded-set case takes when nothing above it survives.
The review's own search found the shipped exposure to be `aicode` lines only, and every shipped
scry draw to be `delayed`, so there is nothing in the current pool that would show the
difference either way.

**Two carry-overs the review listed as items 4 and 5 are untouched, by scope.** The human-seat
edict prompt is still a mechanism argument rather than a Vita observation, and the
re-added-after-garbaged element caveat in `forgetElement` (a second garbage slot for the swept
entry would not be nulled) is neither shown reachable nor guarded. Both were explicitly outside
this lane's three items.
