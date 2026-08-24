# Wave-41 probe results (harvested 2026-08-24)

Runner: one agent, sequential, both probes; artifacts in probe-mutate/ and probe-152edits/
(instruments included; no serve cycling; scratch HOME; roster restored, verified to deck164).

## Probe 1 — mutate-pile render (#W41-4): DISCHARGED

Stub-driven (extended tools/gpt-stub-server.py into a prompt-reading option picker), 99
model-free games, both seats AIPlayerGPT — real prompt assembly/HTTP/parsing/translog.

- Coverage fraction: **35/35** option/target lines naming a board-marked mutate pile carry
  the per-card attributed concatenation (baseline was 6/20). Zero W41-4 defect shape
  (one member's text under another member's name).
- Attribution correctness: 70/70 `|| <name>: <text>` pairs match the primitives' own
  `text=` lines; member sets agree with the battlefield tag's (top, underneath) set 35/35.
- Instrument does NOT string-match the fix: it parses the prompt's own battlefield render
  for `[mutated pile - ...]` tags, then classifies option lines naming those permanents.
- Honest scope limits: all 35 on the opponent-targeting surface (self-side host-ask line
  never generated in 99 games — same emitter, covered by construction, not observed);
  every observed pile depth 2 (depth>2 is a loop, not a branch, in mutatedPileTextCore).

## Probe 2 — deck152 guide edits (real serve :8084, corpus-mirrored settings, 5 games)

G1-G4 natural; G5 killed by the harness at t18 (not an engine failure; records used,
flagged incomplete). Instrument analyze-p2.py validated by reproducing wave-40's published
deck152 numbers exactly before use.

- **(a) armed sole-attacker: STILL NULL on its precondition** — 0 armed sole-attacker
  windows in 5 games (17 attacker windows, 9 sole but unarmed, 2 armed-but-two-offered;
  one of those was lethal, licensed by the guide's lethal exception). "free damage": 0
  anywhere. Structural resistance: arming needs low life AND exactly one offered attacker,
  which pull opposite ways in this build. Three deck-iteration attempts failed to force it.
  → Adjudication carries to the wave-41 corpus (2-corpus bar logic applies).
- **(b) low-N blocking: ARMED for the first time — NO paralysis.** 5 blocker windows,
  4 at forecast ≤9 (4, 6, -1, 4), **4/4 blocks taken**; the N=12 favourable trade also
  taken. Zero blocker-dies (rung-3) assignments. BUT: no rung-3 parenthesis was ever
  OFFERED, so both "zero blocker-dies at N≥10" and the ≤9 chump case remain 0/0 —
  uninformative. Carries to corpus review.
- **(c) trade-drift: 0.** No parenthesis-quote-then-override in any blockers reply.
  Protocol clean, no fallbacks.

## New ledger seeds (from probe 2)

- **#W42 seed — false arm on restricted bodies**: G1 s32 arming creature was Goblin Goon
  rendered `[can't attack, can't block, cantpwattack]`; guide step (1) reads POWER only,
  so a permanently-restricted body arms a return-swing check against a swing that cannot
  happen. Guide fix candidate: "biggest opposing power among creatures that can attack".
