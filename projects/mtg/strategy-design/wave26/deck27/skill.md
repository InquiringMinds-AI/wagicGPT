# skill.md - deck27 reviewer, wave 26 (proposals for the synthesis agent)

Two proposals, both CLOSE/PROMOTE witnesses (not new rungs). This seat produced NO new
guide work this corpus - the strategy.txt is FROZEN byte-identical (cmp-verified).

## #1 (CLOSE/PROMOTE) - b2 NATURAL-STOP now has the CLEAN ASK-SEAM ISOLATION wave-25 asked for.

wave-25 validated b2 only at the BLOCKERS seam (deck27 vs109 seq11) and flagged the honest
caveat: the blocker parser's own "none-is-not-an-assignment / last-real-line-wins" behavior
could co-produce the correct outcome, so that seat could NOT isolate b2 from the parser. It
called for "a natural-stop reversal at the ASK/CHOICE seam, where answer-first is the only
prior floor" as the cleaner isolation.

**This corpus supplies THREE such cases, all correct, all last-line-applied:**
- vs22 seq26: `CHOICE: 1 (Cast Lord of Atlantis)` -> `CHOICE: 2 (Cast nothing right now)` (ch=2). Cast-nothing let the board attack for lethal; deck27 won 2/-4 same turn.
- vs102 seq14: `CHOICE: 2 (Play Island)` -> `CHOICE: 1 (Play Swamp)` (ch=1). Swamp secured the 2nd black for {1}{B}{B}.
- vs18 seq4: `CHOICE: 2 (Cast nothing right now)` -> `CHOICE: 1 (Cast Metathran Zombie)` (ch=1). Deploy-every-turn.

All three are natural-stop (each ends with a PLAN line), at the ASK seam where the blocker
parser has no role, and the engine applied the FINAL coded line in every case.
**Recommendation:** mark the wave-25 caveat RESOLVED - b2 natural-stop precedence is now
isolated-and-confirmed at the ASK/CHOICE seam, not only the blockers seam. The standing
`choice`-field scoring check (read WHICH line the engine applied on a first-vs-last
divergence) held clean here and should stay standing.

## #2 (CLOSE-BY-FACT, re-confirm no re-open) - c4 parser hardening: ZERO misfires at a
187-decision seat that exercised natural-stop, truncated, and ATTACK answer-first all in
the same corpus.

The c4 corpus gate (the standing N9 "only a corpus proves not-firing-where-it-shouldn't"
lesson) is CLEAN at this seat: 3 natural-stop rescues (last-line, correct), 2 truncated
replies (first-line, correct - both in the lost vs18 game), 1 ATTACK answer-first (first-line
A1, the sole legal attacker), 1 spurious "(A)"-annotation normalization that was NOT a
reversal. 0 fallbacks, 0 retries. The hardening dropped/mis-picked nothing. Tag the c4 gate
PASS at the deck27 seat; no mechanism concern surfaced.

## Rotation-relevant note for synthesis (see notes.md for the full verdict)

deck27 produced NO guide edit, NO engine/parser bug, and NO new decision-class this corpus.
The ONE item under first-corpus validation (the bottom-phase rule-#1 clause) was UNEXERCISED
(zero mulligans all 6 games). By the standing rotation test (no-guide-mod AND
no-new-work-signal) deck27 is a ROTATION CANDIDATE this wave. The unexercised bottom clause
is a low-frequency edit that carries forward as unvalidated - per the reframed termination
doctrine (gate on NEW-WORK exhaustion, not on validating every low-frequency edit), it does
NOT justify holding the seat.

## No core-prompt change from this seat (see general-suggestions.md - 21st consecutive PASS).
