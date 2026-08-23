# Wave-37 crash: 152v36 game, SIGSEGV->abort, turn 6 (2026-08-22 20:33)

First crash in ~5 corpora (last crash-free streak began wave-33). Binary 5104a2433.
SIGSEGV (SEGV_MAPERR), MAIN thread, real stack (core 350456; my earlier Damage-path
read was a DIFFERENT, older core from a lane worktree - disregard it):
  Player::opponent()                <- segfault: garbage/NULL Player*
  MTGGameZone::intToZone(int, Player*, Player*)
  MTGGameZone::stringToZone
  AAMover::destinationZone
  AAMover::resolve
  ALord::_added(Damageable*)        <- lord "added" hook running a MOVER
  ALord::added(Player*)
  ListMaintainerAbility::updateTargets
  ALord::resolve
  IfThenAbility::resolve
  AbilityFactory::magicText         <- resolve-time ability parsing
  AbilityFactory::addAbilities
  Spell::resolve
Old engine code (ALord/AAMover/MTGGameZone) - none of the wave-35/36/37 lanes touched
these files; likely a latent card-script interaction (152v36 board, turn 6) surfaced by
new play patterns. NEXT STEP-1 lane: identify the deck152/36 card whose script wires an
if-then lord with a move-on-player-added; reproduce (fixture from the game log through
turn 6), run under Makefile.sdl-asan, fix at the origin. Core preserved locally at
crash/w37-152v36.core (gitignored).

## RESOLVED (wave-38 crash lane, 2026-08-22)

Root cause: deck152's Elite Spellbinder "reveal hand" clause used `all(opponenthand)`.
TargetChooserFactory::createTargetChooser prefix-matched `s.find("opponent") == 0`, so the
zone string parsed as a PLAYER target chooser -> ALord over the opponent player ->
ALord::_added(Player*) handed the Player* to the one-shot AAMover ("opponentreveal") as its
target -> AAMover::resolve blind-cast it to MTGCardInstance* -> garbage controller() ->
Player::opponent() SEGV. Fires whenever the opponent has zero nonland cards in hand at ETB.
Same broken clause on Check for Traps, Grief, Humiliate (all borderline.txt).

Fix (commit in wave-38): exact-token guard on the "opponent" branch (zone names no longer
parse as player targets); dynamic_cast type guards in AAMover::resolve + AARandomMover::resolve
(movers move cards; player targets are skipped, not dereferenced); card scripts corrected to
`all(*|opponenthand)` (x4). Fixture: test/elite_spellbinder_empty_hand.txt — segfaults the
5104a2433 binary (red), green on the fixed build; a synthetic probe with the old broken
script shape also runs clean on the new engine (guards hold for the general case).
