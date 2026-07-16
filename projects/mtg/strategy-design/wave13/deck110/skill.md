# Skill delta (wave 13) — deck110 seat contribution

The CURRENT skill is `strategy-design/wave11/strategy-writing-skill.md`. Wave 13 is a
VERIFICATION wave (guides at terminus, adopt bar HIGH). The deck110 seat proposes **NO new
method headline** and **NO cut/edit to any existing method line** — the guide freezes for the
7th wave. But this corpus produced ONE genuine verification-method sharpening that the existing
skill does not state and that would have caught a corpus-wide error two waves earlier.

## PROPOSED SHARPENING (offered to synthesis) — verify RESOLVED EFFECTS against game-log events, not the model's asserted effect.

**The gap it closes.** The skill already teaches "confirmed-fired-representation" (verify a
representation lever actually rendered) and "join the board state before scoring a tally." It
does NOT yet say: when the MODEL asserts an effect magnitude in its reasoning (e.g. "Galvanic
Blast deals 4 with metalcraft"), the review must confirm that magnitude against the ENGINE's
own resolution record (the `Damage:`/`sacrifice`/`dies` game-log events), because the model's
asserted effect and the engine's delivered effect can DIVERGE — and when they do, the divergence
is an engine/card-script bug hiding behind fluent-but-wrong model prose.

**The evidence.** Wave-11 deck110 reported all Galvanic Blast casts as "genuine metalcraft,
correct 4 dmg" — read straight off the model's "4 damage" reasoning. Wave-13 checked the
`Damage:` events and found EVERY Blast in all 6 games dealt 2, never 4: the card's metalcraft
`+2` line (`aslongas(...) damage:2 >2`, missing `oneshot`) never fires. A HIGH, outcome-bearing
bug on the deck's core wincon sat undetected for ≥2 waves precisely because the review trusted
the model's claimed effect instead of the engine's delivered effect. The model's prose is a
statement of INTENT; only the game log is a statement of RESULT.

**How to apply.** For any decision whose value depends on an effect MAGNITUDE the model computes
(burn damage, pump size, life swing, sacrifice count, counters added), locate the resolution in
the game log and confirm the number. Where the deck has a guide-stated magnitude ("4 with
metalcraft," "protection from all colors at 3+"), spot-check that the engine actually delivers
it at least once — a magnitude that NEVER appears in the log across a whole corpus is a bug, not
a coincidence. This extends confirmed-fired-representation from "did the surface render" to "did
the mechanic resolve as rendered."

## Confirmations of existing method (no change)
- **Confirmed-fired-representation / count-agreement (wave-10 headline 3)** held again: surface
  `Artifacts in play: you N` correct in every window; the fold-in reasoning artifact persists
  yet flipped no metalcraft-gated pick (representation-beats-instruction wall, now on this seat
  through 5 waves). Do NOT re-open with louder READ wording.
- **Outcome-delta no-op predicate (wave-10 headline 5)** re-validated by a new E2 variant
  (deck131 s29 net-zero Plating relocation + s30 parse-guarded bounce). Engine ledger, never a
  guide line.
- **Reply-protocol index-vs-plan (wave-10 headline 2)** — the NEW name-echo protocol is the
  shipped realization of this A/B, and it WORKED: 177/177 echoes present, and the one echo-index
  conflict (deck131 s7) resolved correctly to index by design, absorbing a model confusion.
  Index-wins-on-conflict validated.

## Verification-method sharpening (additive, from the blocker seam)
- **When a guardrail's seam becomes newly EXERCISED (here: the seat went from 0 defensive blocks
  to 5), re-audit it against the PROMPT'S OWN annotation before attributing to the guide.**
  deck135 s17 over-blocked despite the prompt spelling out the correct line verbatim ("block
  only where the trade favors you"). A newly-active seam can expose a model failure that the
  representation already guards — score it MODEL/representation-wall, not a guide gap, and don't
  reach for a guide line the prompt already states.
