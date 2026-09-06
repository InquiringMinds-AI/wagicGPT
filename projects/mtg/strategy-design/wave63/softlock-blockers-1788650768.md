# Vita softlock 1788650768 (vpk26, 2026-09-05): declare-blockers advance refused — ROOT CAUSE FOUND

Owner: "a bizarre softlock where i could assign and cancel blockers but not go to the next step" and, asked whether it
refused with zero blockers: "yes, it refused regardless of configuration".

## Verdict

The advance was refused by the W43-1 gate in `GameObserver::userRequestNextGamePhase` (CR 509.1c: a menace attacker
blocked by exactly one creature is not a legal declaration) and vpk26 gives NO feedback for that refusal. The
transcript's final blocker set is Renata, Called to the Hunt alone on Nullpriest of Oblivion (MENACE) plus Nyx-Fleece
Ram on Soul Warden — "one blocker per attacker", which reads as a complete configuration and is illegal. Every
configuration that keeps one creature alone on Nullpriest is refused; the set is accepted the moment Renata is removed
(or both blockers go on Nullpriest).

**Fix already on master (not in any VPK): `349a4d5b4`** — `GameObserver::setAdvanceRefusal` + a 4 s banner in
`GameStateDuel::Render`: "Nullpriest of Oblivion can't be blocked by fewer than 2 creatures - add another blocker or
remove it". The refusal itself is rules-correct and stays.

## Evidence (verified 2026-09-05, headless replay of the Vita transcript, binary = master 98d13050f + probes)

- `WAGIC_ADVANCEPROBE=1` with the ENTRY/branch lines added in this commit: every retry of a synthetic `p2.next 1 7`
  at turn 13 / Blockers prints `refused: illegal block declaration`; no other gate fires. (scratch logs replay-e)
- Same record + `p1.battlefield[5] 1renata, called to the hunt` (remove Renata) + `p2.next 1 7` → the advance passes
  (`nextGamePhase direct`), replay ends at turn 13 phase 8. (replay-f)
- Same + remove both blockers + next → passes likewise. (replay-g)
- So the engine's blocker set tracks the clicks exactly (a removal click is `toggleDefenser(NULL)`, and the record shows
  each removal logged `1<name>`); there is no display/engine desync in the record.

## What the earlier (pre-fix) analysis got wrong — for the next investigator

- "NO `p1.next` logged in turn 13" was a seat error: the request logs under `currentPlayer`, so a human press on the
  AI's turn is `p2.next`. There was no `p2.next 1 7` either — same conclusion (presses never passed the gates), wrong key.
- The first synthetic line (`p1.next 1 7`) was mis-seated AND was "consumed" by a STALE engine action from an earlier
  turn: `mReplayEngineActions` is never pruned and `transcriptActionKey` ignores the turn, so a stale `p1.next 1 7`
  from turn 11 matched. Consumption performs nothing, so that run tested nothing. `WAGIC_REPLAY_NOCONSUME=1` (dev, this
  commit) disables the match for synthetic-line probes.
- The probe printing nothing was therefore NOT "no gate refused" — the request was never issued.
- W43-1 was ruled out on the owner's "zero blockers" recollection; the record's final state has one. Owner reports are
  ground truth for what he SAW; the record is ground truth for what the engine HELD, and here they reconcile: he
  never held a legal set at a moment he pressed advance (the only legal sets are Renata off Nullpriest, or both on it).

## Residual

- The Vita UI: nothing tells the player that an attacker has menace at declare time except the card text; the banner
  now names the count. Whether the card view shows the keyword prominently on the Vita is unverified.
- Replay hygiene: a refused advance leaves no trace in the transcript, which is what made this a two-session hunt.
  Option (not done): a dev-only `#refused next: <why>` comment line in the transcript — only if the loader skips
  mid-file `#` lines (unverified).
- `mReplayEngineActions` cross-turn matching is a real replay-loader defect (a stale engine action can satisfy a later
  recorded intent); left as-is with the dev switch, flagged for wave-64 known-bugs.
