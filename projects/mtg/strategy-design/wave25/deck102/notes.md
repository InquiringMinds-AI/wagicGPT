# deck102 dev notes — wave 25 (engine/harness/model ledger + rotation verdict)

Corpus `matchups-20260725-035605`, binary `62d8783a5`. Record 4/6 (W vs131, L vs59, W vs133, W-adj
vs137, W vs27, L vs109). 0 fallbacks at seat, 0 corpus-wide. Strong, clean corpus.

## VALIDATED / HELD this corpus

### Tergrid cast RULES-CORRECT (3rd consecutive correct cast).
vs131 seq27 t8: `Cast Tergrid, God of Fright {3}{b}{b} (4/5)` -> seq28 `Cast Card Normally` -> onto
battlefield. Cast the {3}{B}{B} GOD FRONT FACE directly (5 lands available: Urborg + Swamps + Mortuary
Mire + Thran Dynamo ramp). Won the game with Tergrid + Sheoldred beatdown. Correct path.
R-DFC-BACKFACE-RESOLVES-FRONT did NOT recur — but again ONLY because the Lantern back-face was never
cast (model reached full God mana). Bug UN-RE-EXERCISED for a THIRD wave. Keep OPEN in the master ledger.

### Flip-thrash CLOSED / demotion HELD round 3.
0 Flip Side pseudo-actions consumed across all 6 games (wave-22: 11, wave-23: 1, wave-24: 0, wave-25:
0). Demotion durable.

### Smallpox discipline HELD round 4.
vs59 s14 Smallpox {b}{b} turn 6 on developed boards vs a Persist deck — WELL-REASONED (sac their
Persist creature + a land + discard; 102 loses one land + 1 life; sets up Gray Merchant next turn).
Fired on a developed board with a plan, not while raced low. Guide case-(b)/(c). Not a leak.

### pw-redirect removal (a4) VERIFIED at this seat.
0 redirect-menu records corpus-wide (chosen_text + choice blocks scanned; guide has no "redirect"
text). Thoughtseize aimed at the opponent resolves as a single target step (vs137 s8-9, vs109 s5-6),
no redirect menu. Net effect: simplification, no new confusion. No decision-shift observed. NOTE: the
exact Corrupt/damage-to-player case was not exercised this corpus (no targeted damage spell cast at a
player) — a4's effect on Corrupt specifically is inferred from the general menu absence, not directly
observed. Low priority to exercise; fine to leave to natural pool play.

## ENGINE (OPEN) — R-DFC-BACKFACE-RESOLVES-FRONT, un-re-exercised 3rd wave.
Casting the {3}{B} Tergrid's Lantern back-face may deploy the God front face (wave-23 observation).
Never reproduced since because the model keeps reaching full God mana and casting the God directly.
The random pool will not reliably produce a Lantern-affordable/God-unaffordable turn. Repro path
(unchanged): a deck199 stacked to reach that turn; inspect the resolved permanent. Bundle with the
steal-probe below (ONE probe game validates both).

## THE KEYSTONE — steal-window validation debt is now 3 WAVES deep and DISCHARGEABLE ONLY BY PROBE.

New mechanistic finding this wave (see findings.md Section 1): every opponent permanent that left play
while Tergrid was on board (vs131, the sole window) was either a TOKEN (Elemental — correctly excluded),
a COMBAT death (Young Pyromancer — not a sac/discard), or an EFFECT-move (Elixir of Immortality left
via its `{2},{T}` shuffle-into-library, NOT a sacrifice cost). Sheoldred's forced opponent-upkeep
sacrifices DID fire — but the opponent fed TOKENS to them. **The deck's own random-pool steal enablers
(edicts, symmetric Pox/Smallpox, Sheoldred) let the OPPONENT choose the sacrifice, and token-making
opponents always sac a token.** A genuine steal needs a FORCED nontoken discard (Hymn/Thoughtseize with
Tergrid live) or a targeted single-permanent edict vs a token-less board — an intersection the random
pool has not produced in 3 Tergrid-on-board waves and likely never will at meaningful frequency.

The steal is IMPLEMENTED (verified in `Res/sets/primitives/borderline.txt`): the front face's
`auto=@sacrificed(*[-token]|opponentbattlefield)` and `@discarded(*[-instant;-sorcery]|opponenthand)`
lines transform the card into a "Put on battlefield / Don't put on battlefield" modal, with correct
token/instant/sorcery exclusions. It is un-fired, not broken.

**Recommendation — DECOUPLE the steal validation from the per-wave review loop.** Run ONE deck199
PROBE (between-wave engine task, not a corpus): stack Tergrid + a targeted Hymn/Thoughtseize/single-
edict package vs an opponent whose board is nontoken permanents, on a curve that (a) reliably lands
Tergrid then forces a nontoken discard/sacrifice while it lives, and (b) also hits a Lantern-affordable/
God-unaffordable turn. That ONE game validates BOTH open items at once: the take-the-steal line firing
on a real "Put on battlefield / don't" window, AND R-DFC-BACKFACE-RESOLVES-FRONT. Holding deck102 in
the per-wave GUIDE-review loop for a 4th steal-watch wave will not discharge this — the pool can't.

## MODEL/GUIDE item — pregame over-mulligan (vs109), turn-1-chase class.
vs109 seq1: model mulliganed a KEEPABLE {3 Swamp, Arcane Signet, Hymn to Tourach, Smallpox, Tinybones}
hand (reads it correctly — 3 Swamps, names Hymn) because it had "no Turn 1 play." This is a JUDGMENT
error, not a miscount (contrast wave-24's "zero Swamps" on a 2-Swamp hand). The hand satisfies the
guide's keep criteria verbatim (Hymn to Tourach is listed as an acceptable early black play). Root
cause: the guide's "early black play" left "early" undefined; the model resolved it to TURN 1. This is
a GUIDE AMBIGUITY, so — unlike the wave-24 miscount — it IS guide-addressable. Applied THIS wave as the
one strategy.txt edit: define "early" as by turn 2 and forbid mulliganing a functional 3-land Hymn/
disruption hand to chase a turn-1 play. Watch for recurrence; if a 2nd seat over-mulligans a keepable
turn-2-disruption hand, promote to a standing pregame-review rung (skill.md).

## STRATEGY EDITS APPLIED THIS WAVE: ONE (mulligan-line sharpening).
strategy.txt = deployed guide + the mulligan paragraph sharpened ('"EARLY" MEANS BY TURN 2, NOT TURN
1... a turn-2 Hymn/Thoughtseize/Tinybones hand with 2-4 lands is a CLEAR KEEP; do NOT mulligan a
functional 3-land disruptive hand to chase a turn-1 play'). diff-verified surgical (only the mulligan
paragraph changed), 0 non-ASCII bytes, 9545 bytes. Every OTHER tested guide line HELD (Tergrid cast,
flip-thrash, Smallpox, painland, threat-deployment). The steal line got no genuine test (0 windows).

## ROTATION VERDICT — ROTATE deck102 OUT after this wave; hand the steal/backface validation to the between-wave PROBE, decoupled from the guide-review loop.

Unsentimental application of the test (the brief demands it — campaign endgame is real, converged
seats rotate):
- no-guide-mod: FALSE this wave (one edit — the mulligan sharpening). By the literal test, not a
  zero-mod candidate THIS wave.
- no-new-work-signal: the ONLY outstanding signals are (a) the steal-line + R-DFC-BACKFACE validation,
  which THREE waves of corpora have proven the random pool CANNOT produce, and (b) the mulligan edit's
  first-corpus confirmation. Neither is per-wave GUIDE-review work: (a) is a one-shot deck199 PROBE
  (engine layer), and (b) is a single confirmation that does not need a dedicated review seat.

The honest call: **deck102's guide is CONVERGED.** wave-24 said "convergence imminent pending the steal
validation"; this wave establishes that the steal validation is STRUCTURALLY undischargeable in the
selfplay loop and belongs in the probe queue, not in a 4th, 5th, ... steal-watch wave. The guide edit
this wave is the last calibration. Recommendation: **run the deck199 probe once (validates steal +
backface together), and ROTATE deck102 out of the guided per-wave review loop.** Do not hold the seat
in the loop waiting for a random steal window that the deck's own mechanics prevent. If the probe
surfaces a backface bug or a broken steal modal, that is ENGINE work (notes/ledger), not a reason to
keep deck102 in the guide-review rotation.
