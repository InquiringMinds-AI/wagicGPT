# Dev notes — deck109 wave-24 (engine/harness ledger with repros) + ROTATION VERDICT

## Record: 5/6 (context only) — beat 27/131/59/102/137, lost 133. The one loss = the documented
drain/lifegain (Yawgmoth BG devotion) matchup ceiling, no misplay. 0 fallbacks at seat.

## Corpus / provenance
- Run `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260724-125739` (the un-quarantined PRIMARY; NOT
  the -073032-N9B-OVERFIRE quarantined run). Binary `/tmp/wagic-d7ff8b6c4` (step-1 batch
  5f78e2934 + pregame phase 86df3b482 + N9 hotfix d7ff8b6c4).
- deck109 acting-seat files (opp): 1784915864 (27), 1784917940 (131), 1784918761 (59),
  1784920774 (133), 1784922282 (102), 1784923736 (137).

## ENGINE/HARNESS: NOTHING TO OPEN this wave. All validated surfaces held.
- **Ledger A2 / fix (d) (first-parseable ATTACK authoritative) — STAYS CLOSED.** 27/27 attacker
  windows honored the first coded line; recorded == first-line in-eligible tokens; 0 eligible
  dropped. 3 over-declarations (vs131 s15, vs133 s18, vs137 s7) all named INELIGIBLE tokens,
  correctly dropped. 2 spiral windows (vs27 s19 x3-lines @79s; vs131 s22 x2-lines @87s) took the
  first line correctly. No prose "Attack: Deal N" hijack line appeared in any window — the class
  the fix targets had no opportunity to fire and was not reproduced. No deck109 attention needed.
- **Pregame mulligan phase (86df3b482) — VALIDATED at seat.** Exactly 1 mulligan ask/game
  (options Keep/Mulligan), no double-asks, no wrong-N. No bottom events (nobody mulliganed ->
  no library bottoming path exercised; translog `bottom` kind never appeared this seat — the
  bottom-at-keep path is UNEXERCISED here, not confirmed). Game-start integrity correct: on-the-
  play games (131/59/133/137) open "Turn 1 - YOUR turn" with no T1 draw; on-the-draw games
  (27/102) open "Turn 1 - opponent's turn". No narration anomaly (the known cosmetic opening-hand
  line did not misfire visibly). REPRO for the bottom path (for a seat that DOES mulligan): none
  from deck109 — flag to synthesis that the bottom-N-at-keep translog record is unverified here.
- **N9 discriminator hotfix / retracted_choice — N/A at seat** (0 retractions, 0 fallbacks).
- **Decode-collapse ANSWER-LOCKED RETRY — fired 0x, correct.** Grep of all 6 files: no garbage/
  empty/truncated reply that a retry SHOULD have caught. 0 is correct here (answer-first held
  111/111 ask + 27/27 attack; longest replies are coherent long spirals, not decode collapse).

## NOT an engine item: Pillar cost is MODEL-SIDE and now CORRECT -> GUIDE layer, no ticket.
The wave-23 vs137 s12 mis-costing (declined affordable Pillar believing {1}{R}) is a MODEL prior-
belief error, not an engine render bug — the option line always showed `Cast Pillar of Flame {r}`
correctly. This corpus confirms the engine render was never the problem: 14 windows offered Pillar
at 1 red mana, all read correctly by the model. Do NOT open an engine ticket for Pillar cost
rendering. The restored guide cost line (deployed lines 37-39) is the correct and now-validated fix.

## Transform (b) / echo (c) / forced-loss (a) / DFC: N/A at this seat
Mono-red, 20 Mountains: no DFC/modal (b N/A), no reveal windows, no forced self-discard/sac of own
cards (a N/A). Echo INDEX-WINS (c): no out-of-range/echo event occurred this corpus (last wave's
vs27 s13 unparsed shape did NOT recur — 0 unparsed at seat).

## ROTATION VERDICT — CONVERGENCE CANDIDATE; recommend KEEP ONE MORE CYCLE to close the Pillar 2-corpora clock, THEN rotate.
Applying the test honestly (no-guide-mod AND no-new-work-signal = candidate):
- **No guide mod this wave:** strategy.txt byte-copied from deployed, cmp-verified identical
  (md5 a8f163f7f2bf1ecce5c9f4f13997ccf1). No revision warranted — the restored Pillar line works,
  burn/attackers/deploy all clean, no new decision-error class emerged.
- **New-work signals: NONE actionable.** Engine (A2) closed; skill rung already refined wave-23;
  core PASS streak intact. 0 fallbacks. The seat is BEHAVIORALLY CONVERGED (5/6, disciplined).
- **The one open item is a VALIDATION-CONTINUATION, not new work:** the wave-23 skill refinement
  set a 2-corpora bar for a cost/comprehension crutch (one clean corpus is insufficient evidence
  of the restored line's durability). THIS corpus is #1 of the two, and it passes cleanly WITH the
  trigger present in the exact regressing matchup (vs137) — strong evidence, but the doctrine that
  drove the restoration explicitly demands a second confirmation before declaring the crutch
  settled. Rotating out now would repeat wave-22's mistake (declaring redundancy on a single
  corpus). ALSO: the aggressive-one-land mulligan judgment (a NEW surface from the pregame phase)
  was UNTESTED this corpus (all 6 hands were trivial 3-land keeps) — a second cycle gives it a
  chance to be exercised.
- **Recommendation: KEEP deck109 in the pool ONE more cycle as a NON-canary** (frozen revised
  guide, no mods) to (a) complete corpus #2 of the Pillar clock, and (b) get any marginal-hand
  mulligan read. **If corpus #2 again shows 0 Pillar mis-costings with the 1-red-mana trigger
  present AND the mulligan phase produces no anomaly, THEN rotate deck109 out as a fully converged
  canary.** The engine/harness side is already closed and needs no further deck109 attention;
  this final cycle is purely to satisfy the crutch-validation doctrine the seat itself established.
