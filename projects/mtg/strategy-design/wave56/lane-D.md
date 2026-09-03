# Wave-56 lane D — D8: the MDFC land back face is playable from hand again

Branch `w56-lane-D`, worktree `worktrees/lanes/w56-D`, base master 41d1c15ef (src = b228b8648).

## Mechanism — CARD SCRIPT, not the engine

Every modal double-faced card whose back face is a LAND carries, in
`bin/Res/sets/primitives/borderline.txt`, a hand entry of exactly this shape:

```
autohand={0}:restriction{can play land,compare(isflipped)~equalto~1} name(<back>) name(<back>) flip(<back>) forcetype(land)
```

`compare(isflipped)~equalto~1` is the whole defect. `isFlipped` is set in exactly one
place for a card in hand — `AATurnSide::resolve`, i.e. the manual **"Flip Side" display
toggle** (`anyzone={0}:doubleside(<back>)`). So on a fresh hand card the entry's
restriction is false, the toggle is the ONLY ability reacting to the click, the engine
fires it with no menu at all, and the land face is unreachable in one click. It was
reachable in TWO (toggle, then click again and take the now-legal entry) — measured on the
base binary before any change — which is why nothing in the engine ever looked broken, and
why 0 of the wave-55 corpus's 3,171 decisions ever saw a land row for either face.

The engine is NOT at fault and is not touched. `LegalActionsOracle::legalLandPlays` is
correct to ignore these cards: an Emeria's Call in hand is a *Sorcery*, and the back face
reaches the battlefield through `AAFlip`'s `forcetype(land)` branch, never through
`MTGPutInPlayRule`. **The fix is data**: drop the `isflipped` clause, leaving
`restriction{can play land}` — which is what the Oracle text says ("You may play
<back face> as a land"; no prerequisite).

Applied to **all 55** such entries in borderline.txt (byte-spliced, `rb`/`wb`; U+FFFD count
in the diff = 0). Audited: every primitive whose `text=` declares an other face that is a
LAND now has the ungated entry. The 11 remaining hits of that audit are the Pathway BACK
faces themselves (separate primitives, reached from the front face's entry — correct) plus
Creeping Inn // Hostile Hostel, a transform DFC whose back face is not played from hand.

**Cards changed (55)**, of which these are in the pool decks: Agadeem's Awakening,
Pelakka Predation (deck146), Emeria's Call (deck146), Brightclimb Pathway (deck145,
deck146), Branchloft Pathway (deck147, deck151, deck152), Barkchannel Pathway and
Hengegate Pathway (deck152), Valakut Awakening (deck34). The other 47 are the rest of the
ZNR/KHM/DSK/BLB modal lands, shipped together because they are one shape.

### NOT changed, deliberately
`borderline.txt` still holds 30 `otherrestriction=...,compare(isflipped)~equalto~1` gates
(and 3 sacrifice-cost `autohand=` ones). Those are the modal-DFC **spell** back faces —
a different route (the alternative-cost cast) with a working two-step and no repro in the
docket. Left for a future item; noted here so the next lane does not have to re-derive it.

## Render: three claims that the fix made false

Per the trust doctrine the surface owes the model truth, and D8's fix turns three shipped
strings into lies. All in `src/AIPlayerGPT.cpp`, all tagged `#W56-D (D8)`:

1. describeAction's modal-DFC LAND branch said *"in this engine only the currently-shown
   face can actually be played as a land. Just play the current face."* — now says both
   faces have their own row in the same window and either is one land drop.
2. The CHOOSE_MENU "Flip Side" tail carried the same claim; same correction.
3. `mdfcSpellLandBackNote` (spell // land, the Emeria's Call class) ended *"gains you
   nothing playable. Use the face that is showing."* — now points at the row that plays
   the land face.

And the newly-existing row needed a surface of its own: the engine labels it with nothing
but the back face's **name** (lowercased script token), which reads as a card reference,
not a land drop. New pure helper `mdfcLandPlayRowTag(backName, backMana, frontName)` feeds
BOTH emitters (priority-seam ` -> ` form, CHOOSE_MENU `[...]` form) so they cannot drift —
the failure mode that made three Flip-Side texts wrong one wave at a time. The printed
name is re-read from the collection (proper casing), and a Pathway (front face also a
land) gets the short form with no "that face can no longer be cast" clause.

## Gate (worktree, fresh full build; `~/.gatelogs/w56-D-{suite,parsetest}3.log`)

| | base (brief) | lane D |
|---|---|---|
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1229, 2 known concurrency failures (solo-green) | **1233 tests, 0 failed** |
| AI tests | 51 / 0 | **51 / 0** |
| suite timeouts | 0 | **0** |
| PARSETEST | 2636 / 0 | **2644 / 0** (+8: 6 new corpus-end CHECKs + 2 new W31-A echo binds) |

Suite deltas are the four new fixtures (registered in `Res/test/_tests.txt`):

* `mdfc_land_back_face_w56d.txt` — Emeria's Call in hand, land drop available, one click →
  the back face enters, life 20 → 17 (its own pay-3-life ETB), and `assertmtgid 491634`
  pins that the instance carries the BACK face's multiverse id. **RED on base**
  (verified by reverting borderline.txt only: life 20, card still in hand, id not found).
* `mdfc_land_drop_consumed_w56d.txt` — the same play, then a Plains that must STAY IN HAND.
  `AAFlip` adds straight to the battlefield zone; `MTGGameZone::addCard` records it in
  `cardsSeenThisTurn`, so `MaxPerTurnRestriction` counts it. Nothing else pinned that, and
  losing it would be a free extra land every turn. **RED on base.**
* `mdfc_land_back_face_pathway_w56d.txt` — the Pathway class (both faces lands): option 0
  still plays the front face, option 2 is the back face's new row. **RED on base**
  (index 2 was the menu's Cancel).
* `mdfc_land_back_face_upkeep_neg_w56d.txt` — NEGATIVE: the ungated entry is still
  sorcery-speed. In UPKEEP the click finds only the display toggle and nothing enters.
  Confirmed the same at END OF TURN (probe, not registered).

## Live probe (arrival-traced, not counted)

Two probe decks (198/199, 10× Emeria's Call + 10× Agadeem's Awakening + 10× Hengegate
Pathway) driven GPT-vs-GPT against qwen36-35b-a3b on Spark :8084, one-shot, three games.
Artifacts in `wave56/probe-mdfc-land/`; the probe decks are DELETED from
`bin/Res/ai/baka/` (roster back to 164).

* Game 3 (final binary): **41 windows offered a `PLAY THIS AS A LAND` row, 6 taken**, and
  **42 records show an MDFC back face on the seat's OWN battlefield line** — the downstream
  game event, not a counter. 3 windows carried the CHOOSE_MENU bracket form. 0 Pathway rows
  claimed a lost cast.
* The two earlier games caught two real render defects that the suite could not: the row
  printed the lowercased script token as the land's name, and the Pathway form claimed a
  castable face was lost. Both fixed before the final gate.
* A Baka-vs-Baka run over the same decks played **zero** back faces: the heuristic seat does
  not value a `{0}` flip-into-play ability. That is a heuristic-scorer item, not a D8 item.

## Guide obligation (orchestrator, not edited here)

**deck146's "NOT LANDS FOR YOU" block must retire.** It taught the pilot that Emeria's Call
and Agadeem's Awakening can never be lands; that is now false, and skill #238's P10
prediction exists to detect exactly this. deck145, deck147, deck151, deck152 and deck34 also
run affected cards and may want a line about the choice. Lane D edited no guide.

## Falsifiable predictions for wave 57

1. `Land drop`-class rows naming an MDFC back face: **> 0** — specifically, records whose
   rendered option list carries `PLAY THIS AS A LAND` in every corpus game whose seat drew
   one of the eight pool cards, where wave 55 measured 0 of 3,171 decisions.
2. **Arrival**, not offers: at least one corpus record per affected deck shows the back face
   on the seat's own battlefield line (deck146, deck152 most likely).
3. `Flip Side` takes on a modal-DFC LAND: **0** — the toggle's remaining reason to exist is
   display, and every tail now says so.
4. Turns on which a seat holding an affected card takes NO land drop while at <= 3 lands
   with an untaken drop: **strictly fewer than wave 55's** (the 146v130 seq 22/23/24 shape).
5. No seat plays two lands in a turn via this route (fixture-pinned, but worth a corpus
   cross-check: land count per turn per seat).

## What I did NOT verify

* **The human/console seat.** Everything here is the AI seat and the scripted harness. The
  hand-click menu a human sees now has an extra row on 55 cards; I did not drive the GUI,
  and I did not check the PSP/Vita `cardauto.{idx,dat}` sidecars (they are regenerated from
  primitives by `make-*-cards.sh` at deploy time — a port build must be re-made or the
  console will keep the gated script). Port builds are owner-initiated.
* **The heuristic AI's use of the new row.** Baka took it zero times in a stacked-deck game.
  I did not investigate `AIPlayerBaka`'s scoring of a `{0}` flip ability, and I did not check
  whether it can now thrash between the toggle and the land row (the game completed in 11
  turns and the 51 AI suite tests are green, so no livelock was observed).
* **The 30 `otherrestriction` spell-back-face gates** — same isflipped shape, not touched,
  not repro'd.
* **`make -B`.** I touched no header; the build is effectively full (every live object was
  recompiled in this worktree, the 13 objects with seed mtimes are orphans of an older
  makefile and are not in the link line), and the binary was rebuilt three times.
* **Corpus-scale numbers.** Three probe games on a stacked deck are not a corpus; the
  predictions above are for the wave-56 corpus to settle.
