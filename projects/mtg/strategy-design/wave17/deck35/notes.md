# deck35 — development notes (engine / harness / model / construction) + rotation verdict

## Engine / representation ledger items
1. **[REPRESENTATION, cross-seat candidate] Blocking-math comprehension tax.** The block seam
   produces the largest reasoning-tax replies in this corpus for deck35, repeatedly re-deriving
   first-strike / trample / reach damage from scratch, sometimes wrongly:
   - deck110 s5 — 8717 chars (reach/flying vs Signal Pest).
   - deck62 s14 — 5720 chars; s5 — 2531 chars, and MIS-computed a 1/3 Canopy Spider (1 power, no
     trample) as dealing trample overflow ("the remaining 2 damage goes to me").
   - deck49 s12 — 2271 chars; s16 — 2254 chars ("Wait, let me re-read the combat rules: 'a creature
     it kills deals NO damage back'").
   Proposed fix (if it converges with other seats): the option/target annotation already carries
   rules-text and evaluated magnitudes (P1/P2/P4); extend it at the BLOCK/ATTACK seam with a
   computed combat OUTCOME per candidate assignment (blocker dies Y/N, damage to defender, damage
   trampled through, first-strike ordering). This would cut the tax at its source. Layer: engine/
   representation, NOT core prose. Needs ≥2-seat convergence before building — flag for synthesis.

2. **[MODEL WATCH, low priority — NOT confirmed] Does Gemhide auto-tap eat attackers?** Gemhide
   Sliver gives every sliver "{T}: add any color"; `ManaEngine::autoTapForCost` may tap a sliver
   (any-color producer) to pay a cast, leaving it tapped at the attack step. In deck35's zero-attack
   losses the empty board was demonstrably caused by CHUMP-BLOCKING (deck49 t9 prompt: creatures:0
   after 5 casts + 3 blocks), not by mana-tapping — so this is NOT the cause of the durdle. But it
   is worth a one-game probe (pin deck35, watch whether auto-tap selects Gemhide slivers over lands
   for colored costs and whether that later suppresses an attack). Not asserting a bug; recording
   the hypothesis so it is not re-derived cold.

## Deck-construction observations (not my call to change; recorded)
- deck35 is 62 cards: 35 slivers + 27 lands (12 Forest / 5 Island / 10 Mountain). Slightly land-
  light for the curve top (Might {4}{g}, Fury {5}{r}) but Gemhide ramp compensates.
- The list is heavy on SITUATIONAL lords relative to race-closers: 3x Striking Sliver (first
  strike, and note the model cast it redundantly 3x in one game), 3x Spinneret (reach, defensive),
  only 2x Shifting (the flat-unblockable win-con vs this pool) and 3x Winged. If construction is
  ever tuned, more evasion/pump and fewer redundant Striking/Spinneret would raise the race
  ceiling. Guide steers priority meanwhile.
- Oracle cross-check (engine script vs real MTG): all sliver `auto=lord(sliver) …` lines match
  real Oracle text EXCEPT the engine convention that `lord(sliver)` buffs ALL slivers on the
  battlefield including an opponent's (real Slivers do too, historically), while Striking Sliver is
  correctly scoped `lord(sliver|mybattlefield)` = yours only (matches M14 Oracle). No divergences
  requiring a card-script fix; the guide uses engine reality (Shifting = unblockable vs non-slivers).

## Rotation verdict
**deck35 is a GUIDELESS NEWCOMER that just entered the pool; this pass WROTE its initial guide.**
Per doctrine, newcomers effectively never rotate on entry. This seat produced a substantial new
guide (a rule-#1 countermand + capability table + attack floor) and a clear new-work signal (the
guide is untested; the durdle-signature instrument wants validation). **NOT a rotation candidate.**
Next wave should run deck35's FIRST GUIDED corpus and validate, per Step 0-ter, whether the
defensive-spiral class (FC1) DIED, and whether the 0-attacker-game signature disappears.
