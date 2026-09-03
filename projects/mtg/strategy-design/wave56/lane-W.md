# Wave-56 lane W (HOTFIX) — E-2: `restriction{can play land}` did not stop a land drop on the opponent's turn

Branch `w56-lane-W`, worktree `worktrees/lanes/w56-W`, base master `d94e4aa4d`.
Blocking a Vita release; scope was the legality defect first, render items after.

## CORRECTION TO THE SEAT FINDING — the token was NOT a no-op

Seat 146/152/162's E-2 says `restriction{can play land}` "is a NO-OP string" because
`AbilityFactory::parseRestriction` (`src/MTGAbility.cpp:1076`) matches only
myturnonly / opponentturnonly / assorcery / `<my|opponent><phase>only`. That read stops one
function too early. `parseCastRestrictions` does not consist of that switch: below it is a
chain of `restriction[i].find(...)` string handlers, and one of them
(**`src/MTGAbility.cpp:1029`, `check = restriction[i].find("can play")`**) has handled
`can play <type>` since long before this wave. It builds a clone of the card, strips its types,
adds the named type, and then:

```
if (cardDummy->isLand() && ...playRestrictions->canPutIntoZone(cardDummy, ...inPlay) == CANT_PLAY)
    canplay = false;
...
if ((cardDummy->owner == observer->currentActionPlayer) && !cardDummy->hasType(TYPE_INSTANT)
    && !cardDummy->StackIsEmptyandSorcerySpeed())
    canplay = false;
```

So the land-drop quota and the sorcery-speed test were both already there. Measured on the
seeded base binary `archives/wagic-d94e4aa4d-w56hotfix`, before any change of mine:

* own main phase, drop already spent -> the back-face row does **not** react (Plains first,
  then Emeria's Call: one reacting ability, no menu, nothing enters). GREEN on base.
* own main phase, drop unspent -> the row reacts and the land enters. GREEN on base
  (this is lane D's fixture).
* **opponent's turn, AI seat at its own priority window -> the land ENTERS.** RED on base.

**The actual defect is the actor, not the absence of a check.** Two things in that handler read
`observer->currentActionPlayer` instead of the player the restriction is being evaluated FOR
(`player`, already passed in, and available as `card->controller()`):

1. the sorcery-speed test is guarded by `cardDummy->owner == observer->currentActionPlayer`,
   so when a seat acts at a priority window on the OTHER player's turn the timing test is
   **skipped entirely** — 18 of the corpus's 62 windows, and the one that resolved
   (`152v162` seq 3, Boulderloft Pathway in the opponent's upkeep of turn 2, then a Plains on
   its own turn 3);
2. the quota is read off `observer->currentActionPlayer->game->playRestrictions` — i.e. in that
   same situation, **the opponent's land drop**, not the acting seat's. That is the most likely
   mechanism behind the 6 `Land drop: ALREADY USED this turn` windows too (they overlap the
   opponent-turn set; the census rows sum to 54 against 41 non-legal windows, so they do
   overlap), and it is why "already used" and "row still offered" could coexist.

The seat's `Ask` — "change the 55 entries to `restriction{assorcery}` and add the land-drop
check separately" — would have been a **regression**: `assorcery` is evaluated against
`observer->currentPlayer`, which is right, but it drops the quota that the existing handler
already enforced, and the brief's own instruction not to retreat to `assorcery` alone stands.

## THE FIX — make the token a real restriction, evaluated for the card's controller

Engine, general, three files.

**1. `ActivatedAbility::CAN_PLAY_LAND = 61`** (`include/MTGAbility.h`).
`AbilityFactory::parseRestriction` now recognises `can play land` (and the alias
`canplayland`), checked FIRST so the substring scans below cannot mis-claim it. That gives the
token a life it did not have: `getAbility` calls `parseRestriction` on the whole ability line
(`MTGAbility.cpp:2706`) and stores the result as the ability's `restrictions` int, so the
gate now runs in `ActivatedAbility::isReactingToClick` — which is what builds the OPTION LIST.
The corpus failure is an *offered* illegal row, so refusing the click is not enough; the row
must not exist. Both `isReactingToClick`'s switch and `parseCastRestrictions`' switch got the
new case, and they call the same predicate.

**2. `LegalActionsOracle::canPlayLandNow(MTGCardInstance*, Player* actor)`**
(`src/LegalActions.cpp`) — the predicate, pure:

* `actor`, when given, must be the card's **controller** (the acting player at the seam);
* `card->StackIsEmptyandSorcerySpeed()` — own turn, FIRSTMAIN or SECONDMAIN, empty stack, no
  foreign interrupt. This is **the same call** `MTGPutInPlayRule::isReactingToClick` makes for
  a normal land (`src/MTGRules.cpp:342`), on the same object, so the two land routes cannot
  disagree about timing;
* `controller()->game->playRestrictions->landDropAvailable(...)` — the drop.

Note what is NOT reused: the legacy handler's `observer->currentActionPlayer`. Everything here
is `card->controller()`. The legacy string handler still runs afterwards and can only ADD
refusals (parseCastRestrictions returns 0 if any check fails), so the two are ANDed and the
strictly stronger one wins; I left it alone rather than re-point `currentActionPlayer` in a
handler that also serves ~60 `can play creature` / `can play sorcery` entries (foretell,
disturb, commander) in a blocking hotfix. **That is a docketed residual, below.**

**3. `PlayRestrictions::landDropAvailable(GameObserver*, MTGCardInstance* ctx)`** +
`MaxPerTurnRestriction::quotaLeft()` (`src/PlayRestrictions.cpp`).
The lands-per-turn counter asked **by type**, not by card. `canPutIntoZone(card, inPlay)`
returns `NO_OPINION` for any card the land restriction's TargetChooser cannot target — and an
Emeria's Call in hand is a **Sorcery** — so asking it about the MDFC itself is meaningless.
`landDropAvailable` builds a probe chooser from the same `"land"` spec the rules card's
`maxPlay(land)1` registered (`bin/Res/rules/*.txt`), finds that exact restriction through the
existing `getMaxPerTurnRestrictionByTargetChooser`, and reads `quotaLeft()`. `quotaLeft()` is
the count `canPutIntoZone` itself now calls — **one definition**, so the flip route and the
normal drop read the identical number. TRUE when no such restriction is registered (a rules set
without `maxPlay(land)`).

**Is the drop CONSUMED?** Yes, and it always was: `AAFlip::resolve` adds the back face to the
battlefield zone, `MTGGameZone::addCard` records it in `cardsSeenThisTurn`, and
`MaxPerTurnRestriction` counts that. Lane D's `mdfc_land_drop_consumed_w56d.txt` pins
back-face-then-Plains; my `mdfc_land_row_consumes_drop_w56w.txt` pins the other two directions
(back face then a SECOND back face, and back face then Plains), and
`mdfc_land_row_after_land_drop_neg_w56w.txt` pins Plains-then-back-face. Nothing in the new
check loosened that.

**Card data untouched.** All 57 `restriction{can play land}` entries in `borderline.txt` keep
their text (55 `autohand=` MDFC entries, plus `auto={0}:restriction{can play land}` on the
Omenkeel-style "play a land from the opponent's exile" ability and one
`autocommandzone=... restriction{can play land}` sideboard-basic entry). Those last two now
also get the CR-305.1 gate, which is correct for them. No byte-splice was needed anywhere.

## Fixtures (`Res/test/`, registered in `_tests.txt`)

RED/GREEN measured against `archives/wagic-d94e4aa4d-w56hotfix` (base) and the lane binary.

| fixture | base | lane |
|---|---|---|
| `mdfc_land_row_opponent_turn_neg_w56w.txt` — AI seat holds Emeria's Call on the OPPONENT's turn (`FORCEABILITY`, `AICALLS 60`, both seats `ai`) | **RED**: back face entered, life 20 -> 17, card gone from hand | **GREEN**: battlefield unchanged, life 20, card still in hand |
| `mdfc_land_row_after_land_drop_neg_w56w.txt` — Plains first, then the back face | GREEN | GREEN |
| `mdfc_land_row_consumes_drop_w56w.txt` — back face, then a SECOND MDFC back face and a Plains, both refused | GREEN | GREEN |
| lane D's four `mdfc_*_w56d.txt` (NEGATIVE control) | — | **all GREEN** |

Only the first is RED->GREEN, and that is the honest picture: the already-used and
wrong-phase cases were already enforced by the legacy handler for a seat acting on its own
turn. Fixtures 2 and 3 are **regression pins** on the new quota path, not repros — they exist
because `landDropAvailable` is a new way to answer a question that already had an answer, and a
pin is the only thing that keeps them equal.

**Why the opponent-turn case needs an AI seat**: a scripted click routes through
`GameObserver::currentlyActing()`, and `ActivatedAbility::isReactingToClick` requires
`source->controller() == player`, so a human/script click can never reach this ability while
another player is active. The corpus's illegal play was an AI seat at its own priority window
on the opponent's turn, and that is the only seat that can get there. A scripted opponent-turn
fixture I wrote first came back green on BOTH binaries for exactly that reason and was
replaced.

## E-3 (MED) — DONE

`Land drop: you have no land you could play right now (none playable from your hand or any
other zone).` printed on 17 windows that carried a live `PLAY THIS AS A LAND` row three lines
below it, one of which the seat took (`146v125` seq 13). `LegalActionsOracle::legalLandPlays`
is right to ignore an Emeria's Call, so the summary was derived from a set that structurally
cannot contain these plays. New file-local `mdfcBackFaceLandStatus(Player*, bool& playable,
bool& haveLand)` in `src/AIPlayerGPT.cpp` ORs the back-face plays into both flags, using
`asMdfcLandPlay` + `isReactingToClick` — **the same test that puts the row in the option
list**, so the sentence and the rows now come from one union. `haveLand` counts a HELD back
face even when it is not playable, which is what keeps "the drop is already spent" distinct
from "there is nothing to play". No rendered string changed, so PARSETEST needed no new cases.

## E-1 (HIGH, render) — NOT DONE, deliberately, with the design note

The pregame hand header counts printed types (`pregameHandHeader`,
`src/AIPlayerGPT.cpp:~2204`), so a hand whose only lands are MDFC back faces reads
`counted by the engine: 0 lands, 7 spells`, and that decided both of deck146's mulligans and
its one bottom. It is contained (detection is data-only — `c->magicText` carries
`forcetype(land)`, so it works before any ability is registered) but it is **not cheap**, and
it lands on the mulligan surface, which the trust doctrine makes hand-only and which the owner
reads. The unresolved part is not the plumbing, it is the TALLY SEMANTICS, and I am not
guessing it inside a blocking hotfix:

* the seat's proposed wording counts the card as a land ONLY
  (`2 lands (1 of them a land on the back of a spell: Agadeem's Awakening), 5 spells`), which
  keeps `lands + spells == handSize` — but then a Pelakka Predation ({B} removal) drops out of
  the `Cheapest spell in this hand:` scan and out of `Playing every land in this hand would
  cover the cost of:`. That trades one false number for another;
* counting it in BOTH tallies is truthful about the card but breaks the sum, so it needs the
  note to carry the arithmetic (`2 lands, 6 spells — 1 card is both: Agadeem's Awakening plays
  as a spell OR as the land Agadeem, the Undercrypt`);
* either way `sources[]` must gain the back face's colours (look the back face up in
  `MTGCollection()` by the `flip(...)` name and reuse `landTapMana`, as the row emitter at
  `:17117` already does), and `pregameHandHeaderText` gains a parameter and needs new PARSETEST
  cases (positive, negative, and the both-tallies note shape).

Docketed for the next wave with the above as the starting point. **The guide half is already
shipped** (146-A retired the "NOT LANDS FOR YOU" block), so the pilot is no longer being taught
the false rule; what remains is the false NUMBER.

## Gate (lane binary, full)

| | baseline (brief) | lane W |
|---|---|---|
| suite `WAGIC_TESTSUITE_THREADS=1` | 1239 tests / 0 failed | **1241 tests / 0 failed** |
| AI tests | 54 / 0 | **55 / 0** |
| suite timeouts (`timed out`) | 0 | **0** |
| PARSETEST | 2736 / 0 | **2736 / 0** |

+2 scripted tests and +1 AI test = the three new fixtures. PARSETEST is unchanged because no
rendered string changed. Logs: `~/.gatelogs/w56W-{suite,parsetest}.log`.

## Residuals / what I did NOT verify

* **The legacy `can play <type>` handler still uses `observer->currentActionPlayer`**
  (`src/MTGAbility.cpp:1042-1046`). For `can play land` it is now shadowed by a correct,
  stricter check, so it cannot cause a false ALLOW there. It is still wrong in the same way for
  the ~60 `can play creature` / `can play sorcery` entries (foretell, disturb, the commander
  cost ladder): a seat evaluating one of those at a priority window on the opponent's turn
  skips its own sorcery-speed test and reads the opponent's stack restriction. Not repro'd, not
  touched — a hotfix is the wrong place to move a predicate under 60 cards. **Docket it.**
* **No live model run and no corpus.** E-3's effect is reasoned + suite-green only; nothing
  rendered a prompt in this lane. The falsifiable prediction for wave 57 is below.
* **No Vita/PSP build, no GUI drive.** The human seat rides the same
  `ActivatedAbility::isReactingToClick`, so the row is gated for it too by construction, but I
  did not click it. The console also needs its `cardauto.{idx,dat}` sidecars regenerated if a
  port build is made — unchanged from lane D's caveat (card data was not touched this lane, so
  a port carrying lane D's data already carries mine).
* **The 30 `otherrestriction=...,compare(isflipped)~equalto~1` spell-back-face gates** —
  untouched, same as lane D.
* **`make -B`** — not run. No header ABI change beyond added declarations and one enum value,
  and every dependent object recompiled in this worktree (MTGAbility.h, PlayRestrictions.h and
  LegalActions.h edits pulled the tree through). A shipping build should still `make -B`.

## Falsifiable predictions for wave 57

1. **`PLAY THIS AS A LAND` rows on a prompt whose header is not the seat's own main phase: 0.**
   Wave 56 measured 41 of 62 (18 opponent-turn, 13 Blockers, 6 drop-spent). This is E-2's whole
   claim; a single non-zero is a fix failure, not noise.
2. **Land plays resolved on any turn half that is not the player's own main phase: 0**, by any
   route. Wave 56 had one (`152v162` seq 3).
3. **Lands entering per seat per turn never exceeds 1**, counting back faces — unchanged from
   lane D's prediction 5, now pinned by three fixtures instead of one.
4. **E-3**: prompts carrying BOTH `Land drop: you have no land you could play right now` and a
   `PLAY THIS AS A LAND` row: **0** (was 17). And prompts carrying `Land drop: NOT yet used` on
   a turn whose only playable land is a back face: **> 0** — the positive half, which catches a
   `mdfcBackFaceLandStatus` that silently never fires.
5. **E-1 is still open**: deck146 pregame prompts whose `counted by the engine: N lands`
   disagrees with the hand's true land-backed count remain non-zero. This prediction exists to
   keep the item visible, not to pass.
