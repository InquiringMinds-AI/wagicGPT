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
