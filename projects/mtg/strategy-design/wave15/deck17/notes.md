# notes.md - deck17 engine/harness ledger + rotation verdict (wave-15)

Layer-routed engine/representation items observed at the deck17 seat. Guide prose items live
in strategy.txt; this file is engine/harness only.

## Observed this corpus

1. DEGENERATE REPETITION -> unparsed -> heuristic (model-incapacity/latency). vs135 s6: the
   reply looped "It gets +1/+1 from Glen Elendra Liege (if I had one out, which I don't)" ~100
   times, then "It will also be a good blocker against the opponent's other creatures"
   unbounded, until truncation -> choice -1 heuristic fallback. Pure model failure on a
   low-stakes cast; the parse guard caught it correctly. Route: model-experiments residuals /
   the long-ramble class, NOT guide. Notable because it directly feeds the adjudication-by-
   latency loss mode.

2. STALE / CONTRADICTORY BOARD-STATE ANNOTATION (representation; matches wave-13 deck135 s19
   shape). vs21 s23: an attacker serialized `[tapped - untaps and can attack next turn]
   [attacking]` - a tapped-vs-attacking contradiction the model spent a giant reply
   reconciling. Also vs135: Oona's Gatewarden appeared on the battlefield line (`creatures:1`)
   after the game log said it "battlefield -> graveyard" (it had been RE-CAST; the log tail
   read as a contradiction). Both are reasoning-tax multipliers on the board serialization,
   not the option annotations - the wave-13 board-locus item, witnessed again at a new seat.
   Route: representation (board-state serialization).

3. choice=-1 LONG-RAMBLE / TRUNCATION fallbacks (guard working). vs110 s15 (unparsed ->
   heuristic); vs44 s26 (reply truncated mid-sentence at "If I cast Plumeveil, I have" - Main
   phase 2, lethal already assured, harmless). Consistent with the corpus-wide benign
   unparsed class. No action.

4. MIXED-FORM ATTACK LINE - NOT a clean bug witness here. vs21 s22 `ATTACK: A1, Plumeveil
   (first), Plumeveil (second)` recorded only Scion (A1). The trailing NAME tokens were
   DEFENDERS (Plumeveil cannot attack), so dropping them was CORRECT, not the mixed-form
   silent under-declaration the ledger tracks. Flagged so synthesis does NOT count this as a
   mixed-form drop. (The deck-side fix is the guide's "never name a defender as an attacker.")

5. Gelid Shackles interaction rendered correctly (vs135): enchanted Gatewarden showed
   `[cantblock, noactivatedability]` and the model respected it. No defect - positive note
   that the aura/keyword-suppression serialization is working.

## Fallback / staleness spot-check (per brief)
No wrongful stale_echo downgrade found at this seat: the choice=-1 records here are genuine
long-ramble/degenerate/truncation cases (no `CHOICE:` reached), not echoes that named an
offered option and got mis-routed. The absent-echo staleness route is not implicated at deck17.

## ROTATION verdict
deck17 is a NEWCOMER (entered guideless this wave) - NOT a rotation candidate. It generated
substantial new work signal (the hybrid #1-rule, defender facts, attack floor, steal
priority) and now carries its FIRST guide (strategy.txt), which must be validated next corpus.
Keep deck17 in the pool; re-review its shipped guide next wave.
