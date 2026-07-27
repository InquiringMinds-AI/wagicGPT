# Wave-32 probe results — N-122a (Felidar Sovereign alt-win)

Probe run 2026-07-27, binary 6ca1f8411. Stacked probe pair `deck199.txt` (4x Felidar
Sovereign + Soul Warden/Suture Priest/Lone Missionary/Wall of Omens + 20 Plains, no other
win route), GPT mirror one-shot, deleted after the run.

## Verdict: **PASSED — the engine is CORRECT. N-122a retires with no change.**

Evidence (translogs 1785164837-ai_baka_deck199-*, stderr /tmp archive):
- Seat 1 landed TWO Felidar Sovereigns (~turn 11) at life 32-36 — below the threshold,
  so intermediate upkeeps correctly did not fire.
- Life crossed 40 during turns 15-16 (Soul Warden pings + lifelink); at seat 1's TURN-16
  UPKEEP (life 44) the stderr shows two `StackAbility (Source: Felidar Sovereign)`
  added, the first resolving, and the game ending `winner=0 life0=44 life1=47 turn=16`
  with the opponent alive at 47 — the wingame check fired exactly per
  `@each my upkeep:this(controllerlife > 39) wingame`.
- Confirms the ledger's hypothesis: two corpora of non-firing were co-presentation
  failure (card + threshold + upkeep never aligned in random draws), not an engine
  defect. Monitor-grep caveat for future probes: the engine logs the win as a Felidar
  StackAbility resolution, not any "wingame"/"wins the game" string.

## Side products of the probe pair run

- The deck198 convoke probe game CRASHED (SIGSEGV, `__dynamic_cast` at
  `ExtraCost.cpp:1231`) — root-caused to the step-1 rework's stale-payment lifecycle:
  `Convoke::doPay` still guarded on the transient `target` field while `isPaymentSet`
  was re-keyed to the chosen-targets list, so a float-then-pay cast completed without
  ever clearing the TargetChooser; a chosen convoker later died and was freed; the next
  offer probe dynamic_cast a dangling pointer. FIXED in 7fabd9bd0 (doPay re-keyed +
  targets cleared however payment ends + the ancient `ExtraCosts::reset` "set payments
  unset" TODO discharged). Gates: suite 1031+20/0 ST, PARSETEST 220/0, all step-1
  fixtures green. Probe re-run pending at write time.
- Pre-crash, the same game showed the FIRST OBSERVED March of the Multitudes cast
  reaching resolution (turn 14) — a positive early signal for the convoke fixes,
  superseded by the re-run's arrival-traced verdict.
