# deck93 dev notes (wave 29) - engine/harness ledger + ROTATION VERDICT (FINAL)

## N-93a (CLOSED - FIX VALIDATED): summoning-sick board annotation kills the L2 phantom class
The wave-28 proposal - add `[summoning sick - cannot attack this turn]` inline on the
own-battlefield render, mirroring the e4 land tag - shipped in binary 50eeed8be (f3) and is
VALIDATED at this seat. Phantom-attacker over-list rate collapsed 11/15 (73%, wave 28,
guide-only) -> 1/21 (5%, wave 29), and the single residual is a DIFFERENT class (see N-93c),
so the summoning-sick over-list class is 0/21. 18/21 attack replies cite the restriction
directly. The guide was frozen byte-identical across the transition (cmp-verified 5124 bytes),
so the annotation is the isolated cause. Tag present in every game (9-15 occurrences/game).
CLOSED - no further work at this leak.

## N-93b (CLOSED - DE-DUP APPLIED): mulligan prompt no longer double-renders the hand
The wave-28 cosmetic (mulligan prompt listed the opening hand twice - plain then tagged) is
resolved. The mulligan decision prompt now renders the hand ONCE (the tagged `Your hand:`
line) plus the descriptive "Pre-game mulligan decision (London mulligan)" sentence. Verified
across all 6 games. The plain "opening hand (6 cards): Swamp; Swamp;..." string survives only
in the event-history replay of later prompts (expected/harmless). CLOSED.

## N-93c (NEW, LOW severity, CROSS-DECK - not a deck93 blocker): tapped-tag wording is an
## attractive-nuisance; leads to a present-turn attack misread
**Symptom.** The single residual phantom over-list this wave (137 seq18, turn 11) was NOT a
summoning-sick failure. Board (verbatim): `Relentless Rats #2 {1}{b}{b} (5/5) (printed 2/2)
[tapped - untaps and can attack next turn]`, with #3/#4 correctly excluded by their sick tags.
A-lines offered only `A1. Relentless Rats #1 (5/5)`. Reply: "ATTACK: A1, A2 ... both untapped
Relentless Rats (#1 and #2)". The model read the tapped tag's substring "can attack" and
mis-bound it to the present turn, ignoring "next turn," and called a [tapped] creature
"untapped."
**Root cause.** The tapped-state annotation `[tapped - untaps and can attack next turn]`
embeds an action-AFFIRMATIVE phrase ("can attack") about the very action it restricts. The
summoning-sick tag works cleanly precisely because it is restriction-first and
affirmative-free ("summoning sick - cannot attack this turn").
**Proposed fix (render layer).** Reword the tapped tag restriction-first, dropping the
attack-affirmative clause: e.g. `[tapped - cannot attack this turn]` (optionally
`[tapped - cannot attack or block this turn]` if it also gates blocking). Do NOT state the
permitted future action in the same clause.
**Repro.** Binary 50eeed8be, corpus matchups-20260726-115018, file
`1785091303-ai_baka_deck93-*vs-ai_baka_deck137.jsonl`, seq18.
**Layer note.** Engine/representation render-wording, cross-deck (affects any deck reading a
tapped creature). Low severity - 1/21 here, caused no game harm (the one legal attacker was
part of a winning swing). Logged as a general render-polish ledger item; explicitly NOT a
deck93-specific work-signal that should hold this seat for a fourth arc (see rotation verdict
and skill.md S-93b for the wording principle).

## Steady-state confirmations (no action)
- Instance handles / R-DUPLICATE-NAME-INSTANCE: CLEAN at scale, round 3 (0 fallbacks, 109
  decisions, peak #6 concurrent Rats). Handle/binding defect class remains closed.
- f3 sick tag: consumed cleanly, no prompt-noise, no latency, READ not ignored (18/21 cite).
- Land-blindness: DEAD (6/6 keep, 0 "zero lands" phrase corpus-wide).
- Blocker over-lists: NONE (wave-28 had 1). Parser contradictions: NONE.
- All corpus fallbacks (11/1,316 per brief) are at OTHER seats; deck93 = 0.

================================================================================
ROTATION VERDICT: ROTATE OUT - WITH FINALITY (third complete pipeline arc)
================================================================================

Per the wave-29 brief's named exit path: "if L2 is dead and the guide stays frozen, ROTATE
OUT with finality." Both conditions are met:

1. **L2 phantom-attacker class is DEAD.** 11/15 -> 0/21 for the summoning-sick class; the
   exit gate (f3 sick-tag validation) is PASSED, with a completed within-deck controlled
   experiment (guide frozen, annotation the isolated variable) and 18/21 direct-tag-citation
   confirmation. This is the annotation-lever's cleanest demonstration in the corpus.
2. **Guide is FROZEN and fully validated.** Byte-identical redeploy (cmp-verified 5124 bytes),
   frozen since wave-27. All guide-layer teaches resolved across the arc: mulligan
   land-blindness FIXED, L3 over-blocking FIXED, L4 critical-mass sound (residual losses are
   structural matchup speed, not a guide defect - re-confirmed by the vs146 loss to a faster
   now-guided deck). No guide work remains and none is projected.
3. **No new deck93-specific work-signal.** N-93a and N-93b are CLOSED. The only new item
   (N-93c tapped-tag wording) is a CROSS-DECK render-polish ledger entry, not a deck93 leak -
   it does not qualify as a seat-specific work-signal, and holding deck93 for a fourth arc to
   chase a general wording tweak would be sentimental, contrary to the UNSENTIMENTAL rotation
   rule. The rotation test (guide frozen AND no new SEAT-specific work) is satisfied.

deck93 completes its THIRD full pipeline arc (wave-27 initial guide -> wave-28 first-guided
validation + perception-vs-strategy natural experiment opened -> wave-29 exit-gate passed +
experiment closed). It leaves behind a promoted skill heuristic (S-93a, the
perception-vs-strategy discriminator with a completed controlled experiment) and one open
cross-deck ledger item (N-93c). Guide frozen, handles proven at scale three times, no residual
seat work. Clean terminal exit. ROTATE OUT.
