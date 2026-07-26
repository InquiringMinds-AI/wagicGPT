# deck93 (Relentless Rats) - wave-28 findings (FIRST-GUIDED VALIDATION, 4/6, up from 2/6)

Reviewer seat: deck93, mono-B Relentless Rats (24 Swamp / 36 Relentless Rats). Binary
ad8930961. Corpus matchups-20260726-050449. First validation of the wave-27 initial guide
(deployed byte-identical, 5124 bytes) plus the e4 LAND-IDENTITY-TAG batch fix.

Record decomposition (4/6): WIN vs134, WIN vs22, WIN vs146, WIN vs136; LOSS vs137, LOSS vs18.
Wave-27 was 2/6 (WIN vs134/vs136; LOSS vs22/vs131/vs137/vs18). deck131 rotated OUT; deck146 is
a new Step-0 opponent. Net flips: vs22 LOSS->WIN, vs146 new WIN, vs131 loss removed. The two
persistent losses are vs137 and vs18 - the two FAST go-wide aggro decks in the pool.

97 model decisions across 6 games, **0 fallbacks** (parser/binding held perfectly again at the
36-Rat stress seat - the two corpus-wide retracted_choice fallbacks are at deck134 seq25 and
deck146 seq40, NOT here).

================================================================================
JOB 1 - LAND-BLINDNESS KILL CHECK  ->  VERDICT: CLASS IS DEAD (6/6 kept)
================================================================================

The identical opening hand `Swamp; Swamp; Swamp; Relentless Rats x3` was dealt in ALL 6 games
(same as wave-27). Wave-27 mulliganed it 5/6 as a false "zero lands." **Wave-28 KEPT it 6/6.**
Zero mulligans corpus-wide at this seat; no multi-mulligan chains. Not one reply contains the
phrase "zero lands." The perception class that lost ~5 games last wave is gone.

### Tag presence (e4): CONFIRMED in every mulligan prompt
Each hand rendered the land tag: `Swamp (land: taps for {B}); Swamp (land: taps for {B});
Swamp (land: taps for {B}); Relentless Rats {1}{b}{b} (2/2); ...`. Present in all 6.

### Mulligan table (all verbatim; choice=1 "Keep this hand" in all 6)

| Opp | Kept | Attribution in reply |
|-----|------|----------------------|
| 134 | YES | "a hand with 3+ Swamps and at least one Rat is a 'SNAP KEEP'" (guide) |
| 22  | YES | "According to the strategy guide, 'Three Swamps + three Rats is a SNAP KEEP.'" |
| 146 | YES | "a hand with 3+ Swamps and at least one Rat is a 'SNAP KEEP'" (guide) |
| 137 | YES | "According to the strategy guide, 'Three Swamps + three Rats is a SNAP KEEP.'" |
| 18  | YES | "a hand with 3+ Swamps and at least one Relentless Rats is a 'SNAP KEEP.'" |
| 136 | YES | "'Three Swamps + three Rats is a SNAP KEEP.'" |

### LEVER ATTRIBUTION (NOT a clean annotation-lever signal)
All 6 replies attribute the keep to the **GUIDE's SNAP-KEEP rule**, verbatim. **NONE cites the
land tag `(land: taps for {B})`.** Every reply also correctly counts the mana math ("{1}{B}{B}
= three mana... cast on turn 3"). Unlike the deck22 d2 case (model cited the annotation tag
directly = clean annotation-lever signal), here the model verbalizes the GUIDE. The tag was
deployed simultaneously with the guide this wave, so the tag's INDEPENDENT contribution to the
mulligan keep is UNPROVEN at this seat - the guide's explicit categorical rule appears to be
carrying it. (The stronger annotation-lever evidence is the L2 contrast below, not the
mulligan.)

vs131's compounding wave-27 failure (mulled a 3-lander, kept a 1-lander) is structurally
impossible now - the deck never mulligans a keepable 3-Swamp hand. Loss driver eliminated.

Minor: deck18 seq1 plan said "Draw a card on turn 2" (you don't draw on turn 1 on the play);
harmless, kept correctly. Also the mulligan prompt renders the opening hand TWICE (once plain
"Swamp; Swamp; ..." then once tagged) - redundant but not harmful, no confusion observed.

================================================================================
JOB 2 - GUIDE VALIDATION FOR THE OTHER LEAKS
================================================================================

## L2 (illegal-slot references) -> VERDICT: **NOT FIXED - guide teach FAILED**

The guide's COMBAT section teaches "count your actual legal attackers from the A-lines, not
from how many Rats are on the board." **The model ignored it in 11 of 15 attack declarations
across all 6 games** - it still reads the board Rat count and pads the ATTACK line with phantom
slots:

- vs134 seq8: A-lines offered only `A1. Relentless Rats #1 (3/3)` (2nd Rat summoning-sick).
  Reply: "ATTACK: A1, A2 ... Attacking with both deals 6 damage" - only 3 will land.
- vs22 seq24: A-lines offered `A1..A4` (four 7/7s). Reply: "ATTACK: A1, A2, A3, A4, A5, A6 ...
  I have six 7/7 Relentless Rats ... 35 damage" - the model believes it has 6 attackers; only
  4 are legal. (Won anyway: opp at 3, 4x7 is lethal.)
- vs18 seq18: A-lines offered only `A1` (two Rats summoning-sick). Reply: "ATTACK: A1, A2, A3
  ... a wide board of three 4/4s (12 total power)" - only 1 Rat (4 power) could attack.

The engine safely drops every phantom slot (0 fallbacks, binding hits only intended-legal - the
wave-27 1c handle audit holds), so state never corrupts. BUT the model's PLAN and combat math
are built on attackers that do not exist. **In the vs18 LOSS this plausibly contributed to a
fatal misplay**: at seq18 (turn 11, 3 life) the model tapped its ONE legal Rat attacking for 4
while believing it was swinging "12 total power ... to lethal," then had no board to block with
and died turn 12.

ROOT CAUSE (see notes.md N-93a): the CURRENT-SITUATION board render shows summoning-sick Rats
IDENTICALLY to attack-ready ones - `Relentless Rats #2 {1}{b}{b} (4/4) (printed 2/2)` with NO
sickness marker (only `[tapped]` state is tagged). The model's board-count reasoning has no
inline signal that #2/#3 cannot attack, so it pattern-completes the ATTACK line from the board
and overrides the correct A-line list. **This is the exact inverse of the land-tag fix**: the
perception surface is un-annotated for the restriction, and guide PROSE alone does not override
a contradicting perception surface. The fix is an annotation, not more guide text (see
skill.md).

## L3 (over-blocking / lord-shrink) -> VERDICT: **FIXED (validated)**

The model now consistently REFUSES to chump or gang-block while not facing lethal, citing the
lord-shrink fact verbatim:
- vs18 seq6: "Losing my only Rat destroys my entire game plan, as every subsequent Rat relies
  on the count of Rats on the board for power" -> BLOCKS: none (correct, 4/4 attacker, safe).
- vs18 seq10: "trading a 2/2 for a 4/4 ... shrinks my entire board ... I will preserve my Rat"
  -> BLOCKS: none (correct).
- vs137 seq6: "sacrificing it to kill a 1/1 or 1/2 attacker reduces my board presence and
  weakens all future Rats" -> BLOCKS: none (correct).

Every block it DID declare (vs137 seq10/12/16, vs18 seq14/19) was a survival block against
near/actual lethal, correctly reasoned. **No instance** of the wave-27 L3 catastrophe
(gang-blocking the whole board into one attacker while the opponent was at low life). The
lord-shrink guide fact is being consumed and cited - clean guide-attribution.

## L4 (board-width / critical mass) -> VERDICT: guide sound; residual is MATCHUP speed

Peak Rat count tracks the result cleanly:
- WINS: vs22 peak 6 (opp -25), vs134 peak 4 (opp -5), vs146 peak 4 (opp -6), vs136 peak 4
  (opp -4).
- LOSSES: vs137 peak 2, vs18 peak 3.

In both losses the deck kept a good hand, curved, but never widened - vs137 shows the Rats
forced to trade/chump on defense vs a GW go-wide board it could not outrace, stalling at ~2
Rats. This is the STRUCTURAL slow-deck problem: mono-Rats casts its first threat on turn 3 and
needs several more turns to reach a lethal-wide board; vs the two fastest go-wide aggro decks
in the pool (deck137 GW convoke, deck18 Kithkin anthem) it is the SLOWER deck, so the guide's
"race when you are the faster, wider deck" correctly did NOT fire much (the model was behind on
board). The guide is not at fault; these are close-to-unwinnable speed mismatches for a
36-of-one-card swarm. No guide change extracts a win here.

================================================================================
JOB 3 - INSTANCE HANDLES AT THE 36-RAT SEAT (scale) -> VERDICT: CLEAN PASS (again)
================================================================================

0 fallbacks across 97 decisions in 6 games. Contiguous #N renders held (peak observed: 6
concurrent Rats at #1..#6, vs22 seq24). Handles carried into A-lines (A1..A4 = board #1..#4).
Every phantom over-list (11 attacker cases + 1 blocker case vs18 seq19 "B3" when only B1/B2
legal) was safely dropped by the binder - no mis-bind, no wrong-instance hit. The wave-27
R-DUPLICATE-NAME-INSTANCE audit conclusion is re-confirmed at scale: renders/handles are
correct; the residual is model ADHERENCE (L2), not a handle/binding defect.

================================================================================
JOB 4 - 2/6 -> 4/6 DECOMPOSITION
================================================================================

- **vs22 (LOSS->WIN): GUIDE.** Wave-27 lost this by mulliganing the 3-Swamp hand then keeping a
  2-Swamp/5-Rat hand that bled out (L1+L4). Wave-28 kept the 3-Swamp hand, reached 6 Rats,
  killed opp to -25. Directly attributable to the mulligan fix.
- **vs146 (new WIN): POOL.** New Step-0 opponent (Orzhov Dungeons), guideless debut, weak;
  deck93 curved out and won turn 10 (opp -6). Pool composition, not a deck93 improvement.
- **vs131 loss removed: ROTATION.** deck131 rotated out; the wave-27 loss to it is gone, not
  converted.
- **vs137, vs18 still LOSSES: MATCHUP (structural speed).** Unchanged; the mulligan fix does not
  help against decks that go wider faster than a turn-3-first-threat swarm.

Net: 1 real guide-driven flip (vs22), 1 pool-driven win (vs146), 1 rotation artifact (vs131), 2
structural losses persisting. The guide's headline lever (mulligan) is fully validated; the
record moved for the right reason on the game it should have moved.

================================================================================
JOB 5 - PARSER PRECEDENCE / e-BATCH STEADY STATE
================================================================================

- **Parser precedence contradictions at this seat: NONE.** 0 fallbacks; every choice bound
  within the legal option count. The phantom over-lists are model-side over-declaration, not
  parser contradictions - the binder's first-wins/drop-illegal precedence handled all 12
  cleanly.
- **e4 land tags - noise check: CLEAN.** Tags consumed without confusion; no prompt-noise, no
  mis-parse, no latency effect. Only cosmetic issue is the double-rendered opening hand in the
  mulligan prompt (harmless).
- **e5 annotation-echo tolerance: N/A at this seat** (the stale_echo shape was deck22's; 0
  fallbacks here means nothing to test). No `So, CHOICE:` or echo-class asks at deck93.
- **d/c-batch: steady** (0 fallbacks).

================================================================================
JOB 6 - ROTATION PROJECTION
================================================================================

See notes.md for the full verdict. Short form: the GUIDE is validated and frozen (byte-identical
redeploy) - all three guide-layer teaches resolved (mulligan FIXED, L3 FIXED, L4 sound). The ONE
residual (L2 phantom-attacker) is an ENGINE/REPRESENTATION work-signal (summoning-sick board
annotation), NOT a guide-teach failure. So deck93 is **NOT a clean rotation candidate this
wave** - it produced new engine work. It is NEAR-TERMINAL on the guide axis: once the
summoning-sick annotation lands and is validated, this seat is a clean exit (guide frozen, no
new guide work, handles proven at scale).
