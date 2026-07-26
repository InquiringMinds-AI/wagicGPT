# deck136 (Yarok of Pranks) - notes.md (engine/harness ledger items)
# Wave-28, FIRST-GUIDED. Binary ad8930961. Corpus matchups-20260726-050449.
# Layer-routing: engine/card-script/harness items. NONE become guide prose.

## RESOLVED: R-RESOLVED-SPELL-PHANTOM-CHOOSER (Legion's End) -- RETIRE from active ledger
Wave-27's #1-visibility ledger item is FIXED. Verified at three layers:
- CARD-SCRIPT: bin/Res/sets/primitives/borderline.txt Legion's End no longer carries the
  offending `auto=donothing notaTarget(*|targetcontrollerhand)` 4th line. Current block:
    target=creature[manacost<=2]|opponentbattlefield
    auto=moveto(exile)
    auto=all(*[share!name!]|targetcontrollerbattlefield) moveto(exile)
    auto=all(*[share!name!]|targetcontrollerhand) moveto(exile)
    auto=all(*[share!name!]|targetcontrollergraveyard) moveto(exile)
  The same-name exile from hand/graveyard is now a direct `all(...) moveto(exile)` with NO
  chooser-spawning reveal line -- so no `notaTarget(...)` selector is surfaced as a
  model-facing ask.
- LIVE (6 casts): 0 phantom hand-target asks; 0 fallbacks at the deck136 seat (wave-27:
  4 fallbacks, all this bug). The only Legion's End ask that still fires is a LEGITIMATE
  target choice when 2+ same-name MV<=2 creatures are on the OPPONENT battlefield, and it
  offers ONLY those creatures (no hand cards, no lands). Answered cleanly every time.
- PRIMARY EFFECT: same-name exile confirmed live (vs22 both Mogg Sentries battlefield->
  exile; vs137 both Edgewall Innkeepers; vs18 Wizened Cenn).
RESIDUAL (cosmetic, no action): when the two legal targets are same-name identical, the
"pick the ONE target" ask is redundant -- both are exiled by the same-name clause whichever
is picked. Harmless; model handles it; not worth an engine gate. If a future dedupe pass
wants to auto-resolve a target choice whose alternatives are provably outcome-equivalent
(same name + same effect), this is a candidate, but LOW priority.

## NEW LIGHT WATCH: Rankle combat-damage MAY-ASK surface unverified when Rankle connects
Rankle, Master of Pranks cast vs134 seq38 (t13), attacked t13 + t15 (flying + haste).
deck134 game ended t15 (deck136 won, opp -2). The "you may choose any number of: each
player discards / each loses 1 and draws / each sacrifices a creature" combat-damage
may-ask did NOT appear in the translog at the deck136 seat. Two benign explanations
(Rankle blocked; or the game ended before the trigger resolved) and one worth confirming
(the may-ask does not reach the interactive-AI seat when Rankle deals player damage).
Not a confirmed defect -- carry-forward WATCH. To confirm cleanly: a probe deck (deck199)
with Rankle + haste enablers vs an empty-board opponent, one pinned selfplay game, read
whether a CHOOSE_MENU/may-ask fires on the combat-damage trigger. Reuses the probe-deck
pattern; do not build until a second seat also shows Rankle-connects-without-may-ask.

## Unexercised decision classes (carry-forward)
- Liliana Dreadhorde General LOYALTY: +1 NOW EXERCISED (vs22 seq32/seq39, "create a 2/2
  Zombie", clean render + guide-aligned pick). The -4 (each player sacs two) anti-wide
  activation still has NO live trigger observed (Liliana not drawn in the go-wide games).
  Carry the -4 surface forward only.
- Ugin, the Ineffable: still never cast (offered vs146 seq41 t20; model chose Legion's
  End). Fully unexercised. Guide now nudges casting it on a 6-mana board stall; re-check
  next wave.
- Rankle may-ask: see WATCH above.

## Oracle-verification correction (routed per wagicgpt-verify-oracle-text)
Relentless Rats: engine `mana={1}{B}{B}` = MV 3 (real Oracle {2}{B} = MV 3). The wave-27
guide asserted MV 4 -- a fabrication that inverted the Rats matchup (see findings.md GUIDE
AUDIT). This is a GUIDE-layer fix (strategy.txt this wave), recorded here only as the
verification note: Ritual of Soot ("Destroy all creatures with mana value 3 or less",
borderline.txt) DOES kill Relentless Rats; Legion's End (target MV<=2) does NOT. The
Rats' +1/+1 self-pump does not change mana value, so Ritual hits them at any size.
No engine defect here -- the engine was correct; the guide draft was wrong.

## Corpus integrity (deck136 seat)
6 games, 0 fallbacks, parser/mana/render clean. Reply-length p50 ~1.2k; the top replies
(~13-14k) are all LEGITIMATE combat/cast/blocker decisions, NOT refusals -- the wave-27
15k-char phantom-refusal tax class is gone.

## ROTATION VERDICT: NOT a rotation candidate this wave
Rule: no-guide-mod AND no-new-work-signal = rotation candidate. deck136 got a REAL guide
modification this wave (the Rats MV inversion fix, go-wide matchup split, long-game
scoping, Ugin nudge) AND carries live new-work signal (Ugin fully unexercised; Rankle
may-ask unverified; Liliana -4 anti-wide untriggered; the Rats fix itself needs a live
re-validation vs deck93 next wave). KEEP in the pool. Projected exit: after a wave where
the Rats fix is validated live (Ritual actually swept the Rats, or the pilot correctly
benches Legion's End vs Rats) AND Ugin/Rankle-may-ask/-4 either fire clean or are ruled
seat-structural -- i.e. one more validation wave minimum before it looks like the
deck18/deck59 frozen-guide exit shape.
