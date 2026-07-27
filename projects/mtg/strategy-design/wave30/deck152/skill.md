# deck152 wave-30 -- skill proposals (for the wave-30 synthesis / skill maintenance)

Step-0 guideless debut. Two proposals, both tagged single-seat pending a second witness, plus a
maintenance note on the transform-surface method.

## S-152a (PROPOSAL, single-seat) -- "UNEXERCISED SURFACE" is a first-class review verdict; report the null, do not manufacture a read

deck152 was rotated in explicitly to give the flip-thrash fix its first LIVE transform outing, yet
the day->night FLIP never fired in any of the 6 games (daybound triggers only on a whole spell-less
turn, which never occurred in a spell-dense pool). The honest verdict is UNEXERCISED, not "clean"
and not "buggy": the day-SIDE steady render is coherent and the exile/Class/mana surfaces are fine,
but the specific mechanic the seat was seeded to test produced ZERO events. Rung: when a seat is
rotated in to validate a specific engine behavior and the corpus never triggers it, say so plainly
and state the TRIGGER CONDITION that was missing (here: a turn with no spells cast) -- do not
back-fill a verdict from the adjacent-but-different surfaces that DID fire, and flag that a
controlled PROBE (a slow/durdle matchup, or a scripted spell-less turn) is the only way to exercise
it. This is the transform-surface analogue of the wave-20 ABSENT-ENGINE-CARD rung (write for the
mode you actually observed; a rarely-triggered surface over-reports its untested state). Promote to
a named rung if a second "seeded-but-unexercised" seat recurs.

## S-152b (PROPOSAL, single-seat) -- the COSMETIC-ONLY OPTION is a fabrication attractor; a bare-priority window that offers only a no-op toggle invites a phantom play

The one hard fallback (vs136 seq27) fired at a bare kind=priority window whose ONLY non-pass
"legal action" was the Flip-Side DISPLAY TOGGLE (casts nothing). Seeing no real play but a listed
"action", the weak executor fabricated a play it believed it had (a nonexistent "Cast Briarbridge
Tracker") and over-derived its mana to justify it. This is distinct from the two-imperatives decode
loop and from a normal perception leak: the CARRIER is an option list that presents a cosmetic
no-op as if it were a play. Rung candidate: an option that changes nothing (a display toggle, a
re-sort, a view switch) should not be surfaced as a numbered legal ACTION at a priority pass --
either suppress it or label the window "nothing to do -- pass". Until the render changes, a guide
line ("if your only listed action is a cosmetic Flip Side toggle, answer 0 (pass); do not invent a
Cast number") is the interim (shipped as deck152 guide Rule #1's tail). Sibling to the
TRUST-THE-RENDERED-NUMBER family (the model overrode the accurate mana line). Promote if a second
deck fabricates a play at a cosmetic-only-option window.

## Method note (no change) -- Step-0-bis rules applied cleanly

- Rule 1 (read from the ENGINE): the verify-oracle card table was built entirely from primitives;
  engine matched Scryfall for all 20+ cards, no divergence. Rule 3 dominant-idiosyncrasy: the
  dominant COST here was reasoning tax (Coven) + one mana-perception fabrication, so Rule #1 is the
  TRUST-THE-MANA-LINE corrective (the established rule-1/2 trust-line for a DERIVED-mana deck),
  above the win engine. Rule 5 (attack floor on the first pass): written despite the wins looking
  like the deck already attacks, because the losing games under-developed; the floor is keyed to
  board-readable facts and names the flyers (Elite Spellbinder, Sigarda). Rule 2 trust-the-rendered
  P/T: Briarbridge's +2/+0 and the [counters] all render live, so the guide says "trust the printed
  [X/Y]".
