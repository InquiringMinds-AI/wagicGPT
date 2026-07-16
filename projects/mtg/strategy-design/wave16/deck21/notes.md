# deck21 seat - engine/harness notes ledger + rotation verdict (wave-16)

Layer-routing: engine/card-script/parser/construction items only; no guide prose here.

## Hybrid-pip annotation - EFFECT MEASURED (brief item, deck21 is a top-3 hybrid seat)
The wave-15 engine batch's "[hybrid: each {r/g} pays with R or ...]" cast-line annotation is
LIVE and WORKING at this seat. Before/after: w15 had 1 hybrid misread (declined a {R/G} card
for "lacking green mana", vs133 s3); w16 cast 7 hybrid cards with 0 declines. No wrongful
annotation, no double-counting of mana, no over-offer. Ship-confirmed at this seat.

## stale_echo - both instances GENUINE (superstring fix not mis-downgrading)
- vs62 s8: tapped out; reply looped on CHOICE-line FORMAT and never matched an offered option
  -> heuristic. Correct.
- vs133 s14: echoed "Play Mountain" after the land was already played and no Mountain remained
  in hand (menu = Volcanic Hammer only) -> absent-echo -> heuristic cast Volcanic Hammer to
  face (s15). Outcome-neutral, correct staleness route. NOT a downgrade of a named offered
  option. The wave-15 superstring false-positive fix behaves correctly here.

## Card-script divergences from real MTG Oracle (unchanged from w15 - maintainer verify, do NOT block guide)
Per wagicgpt-verify-oracle-text. The guide is written to ENGINE reality; flag for a maintainer
to check `Res/sets/primitives/*.txt` against Gatherer/Scryfall:
- Scuzzback Scrapper: engine = {R/G} 1/1 wither, NO haste, NO persist. Real card = {2}{R/G} 3/1
  haste persist. Cost AND body differ; missing haste is behaviorally load-bearing. (w15 ledger)
- Boartusk Liege: engine = 3/4 WITH trample (and first strike under Bloodmark, correctly). Real
  card = vanilla 4/4, no trample. Anthem effect matches. Body/keyword divergence only. (w15)
- Spark Elemental [treason] tag = its end-of-turn sacrifice (correct behavior, opaque label -
  w15 general-suggestions #2, rename to [sacrificed at end of turn]). Still opaque this wave.

## Opponent-side card behavior observed (informational, not a deck21 bug)
- deck62 Enchantress: a Birds of Paradise reached 25/26 flying trample via Fists of Ironwood +
  2x Ancestral Mask (vs62 s20). Ancestral Mask's "+2/+2 per other enchantment" and the aura
  stacking resolved as expected; noting because a 25/26 evasive body is a structural hard
  matchup for a no-removal aggro deck (drives the roster/difficulty ladder, not a defect).

## DECK-CONSTRUCTION item (route to roster owner, NOT the guide)
deck21 floods. vs133 was lost with creatures:0 from T5-T13 on an all-Mountain draw; the build
is 20 Mountain + a top-heavy anthem/lord count. Two of the four close losses have a curve/flood
component. If deck21 continues to under-race, the lever is CONSTRUCTION (trim a land or two /
smooth the curve toward more 1-2 drops), not the guide. Flagged for the construction ledger.

## No new engine defects at this seat
No segfaults, no target-cancel crash, no absent-echo wrongful downgrade, no attacker mixed-form
drop this wave. The 69793930a binary held clean across all 6 deck21 games. The w13/w15 parser
infrastructure is stable here.

## ROTATION VERDICT: KEEP IN ROTATION - guide REVISED, new-work signal present.
This is deck21's FIRST guided corpus and it produced a real, evidence-backed guide revision
(E1 / the RACE-MATH flyer exception, reconciling the exception with the matchup section) plus a
validated hybrid-annotation effect and a construction flag. It is NOT a rotation candidate:
- Guide was MODIFIED this wave (not byte-identical) = new-work signal by doctrine.
- The revision NEEDS a next corpus to validate: does the model now pick off small flyers early
  vs17/vs44 (the two by-4 losses), and does that convert either race without over-applying
  (blunting the clock vs non-flyer decks 110/135/133)? Re-run WITH the revised guide against the
  SAME flyer opponents (17, 44) is the validation target.
- Freeze-and-rotate is only appropriate once E1 is validated AND the record stabilizes; not yet.
