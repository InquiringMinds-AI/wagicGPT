# deck18 skill proposals (wave-26 Step-0 read) - what this guideless debut teaches about guide-writing

Bar per brief: proposals ride evidence; PASS is a success verdict. deck18 is the 8th+
guideless Step-0 seat. Nothing here overturns the Step-0-bis method; two CONFIRMATIONS
and one small SHARPENING, all tagged single-seat.

## CONFIRMATION 1 - TRUST-THE-RENDERED-NUMBER is the #1 Step-0-bis teaching point for ANY
   lord/anthem/counter stat-stacking deck (now a 5th seat: 17 mana / 62 Blanchwood /
   35 slivers / 59 hybrid + deck18 Kithkin).

deck18 reproduces the deck62/deck35 signature exactly: the board renders "Field Marshal
(3/3) (printed 2/2)" and the model recomputes from base + each lord anyway, spending
7k-14k chars per spike (vs102 s14, vs131 s18/s22, vs133 s12, vs27 s11/s13/s19/s23). It
is the dominant tax on the deck and the top saliency signal (Step-0-bis rule 2). This is
one more confirmation, NOT a new rung - the existing PROMOTED rule ("trust the printed
[X/Y] for BOTH boards; do not re-derive from base + each lord") already covers it. Note
for future Step-0 authors: on a PURE anthem/lord aggro deck (no activated pump, no
hybrid color confound), TRUST-THE-RENDERED-NUMBER is the ONLY high-tax class - there is
no fabrication (62) or color-veto (17/59) face to compete with it, so it earns rule #1's
"trust" line with the capability table riding beneath it, not above.

## CONFIRMATION 2 - the ATTACK-FLOOR-on-first-pass rung's cousin: teach a deck's EVASION
   as an ATTACK DIRECTIVE, because the model knows the evasion fact but does not apply it
   at the attack seam (cross-phase fact-transfer failure).

New witness for Step-0-bis rule 5 (write the attack/deploy floor on the first pass).
deck18 shows a sharper variant: the model KNEW Goldmeadow Dodger "evades large blockers"
at the vs133 MULLIGAN (s1, verbatim) and then, at the vs133 ATTACK seam (s17), forgot it
entirely - 14k chars agonizing that a 5/5 Obliterator would block and kill the Dodger,
when power>=4 cannot block it at all. The abstract capability does not transfer to the
combat decision on its own. AUTHORING RULE (extends rule 5 / the deck17 "biggest flyer
THAT CAN ATTACK" superlative-scoping family): for any deck with a printed evasion
keyword (can't-be-blocked-by, fear, flying, unblockable), state it TWICE - once as a
capability line AND once as an explicit ATTACK ORDER keyed to the board fact ("swing it
EVERY turn; their big creature cannot block it"). A capability line alone is read at
mulligan and dropped at combat. Tag single-seat; converges with the cross-phase
mana-fact rung (wave-25 Method headline 3) - the general shape is "a deck FACT stated
for one phase does not reach the phase where it decides; restate it at the deciding seam."

## SHARPENING - the "no flyers/reach -> can't block flyers" off-case belongs in the FIRST
   guide for any ground-only deck, as a RACE directive, not just an exclusion.

deck17's Step-0-bis rung named DEFENDERS as an attack-floor exclusion ("do not declare
your walls"). deck18 adds the DEFENSIVE mirror: a deck with NO flyers/reach cannot BLOCK
the opponent's flyers, and the weak pilot does not infer this - it burns 9.9k chars
planning an illegal ground-blocks-flyer block (vs137 s10) and emits a phantom answer the
engine drops. The first guide should carry, for any ground-only deck facing a pool with
flyers: "you cannot block flyers - do not try; take it and RACE." This is the
Step-0-bis rule-6 OFF-CASE (the losing corpus lives in the has-flyer matchup) written on
the FIRST pass rather than waiting for the revision to expose it. Tag single-seat;
promote to a general ground-only-deck first-pass line if a second no-evasion aggro deck
reproduces the illegal-flyer-block tax.

## PASS - no change to the mulligan (FIDELITY-vs-JUDGMENT) rung, the sixth-face template,
   or the density band.

- Mulligan: all 6 keeps correct, no FIDELITY miscount and no JUDGMENT over-mulligan; the
  surface is UNEXERCISED at this seat (mono-white, every hand a trivial Plains+threats
  keep). Carry forward as unexercised per the wave-25 surface-unexercised carry; the
  guide gets only a one-line mono-white "never mulligan for color" note (the cross-phase
  mana fact reaching the mulligan, cheap insurance though no color-anxiety appeared).
- Sixth face (symmetric-effect-asymmetric-in-your-favor via recursion): N/A - deck18 has
  no sweepers/persist/undying. Correctly not applied (brief: "if applicable").
- Density: draft landed ~78 lines / ~4.3 KB, mid-band, justified by the capability table
  + three leak sections. On revision, trim whichever leak the next corpus proves dead.
