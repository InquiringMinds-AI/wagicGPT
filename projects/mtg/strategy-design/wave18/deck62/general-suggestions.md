# deck62 wave-18 — general (cross-seat) suggestions

## 1. (VALIDATED, verify-only) The "legal targets are YOUR OWN / legal targets right now: <names>" engine labels are load-bearing and perfect at this seat.
20 of 20 aura target sub-menus that OFFERED an opponent creature were answered with an OWN creature
(zero enchant-opponent gifts), and every target sub-menu parsed to a target name (the wave-17 N1
sub-menu-as-phase-menu misread did NOT recur). This is the single strongest signal at the seat. Do
not remove or dilute these labels; the TARGET CHOICE framing (wave-17 engine batch) is confirmed
dead-on for a deck whose whole game is "enchant the right creature."

## 2. (cross-seat harness) Filter named-but-ineligible attackers instead of rejecting the whole declaration.
Both A-list unparses (vs14 s21 "ATTACK: Birds of Paradise" with only Argothian eligible; vs21 s12
"ATTACK: Birds, Saproling, Saproling" same) named creatures ABSENT from the A-list and the parser
rejected the entire line -> heuristic fallback. In both the correct answer was ATTACK: none. A
cheap, convergent-value harness fix: when an ATTACK line names creatures, DROP the ones not on the
eligible A-list rather than unparsing the whole declaration (attack with the eligible subset, which
here is the empty set = ATTACK: none). This turns a whole unparsed class into a correct auto-answer
and helps any attack-seat that over-names. (Guide-side mitigation shipped this wave; the harness
fix is the durable one.)

## 3. (cross-seat, LOW) Redundant global enchantments in hand generate wasted reasoning.
deck21 s14/s17: Primal Rage sat in hand while a copy was already on the battlefield (a redundant
global). The model burned hundreds of tokens on "why is it in my hand?" mid-spiral. Any deck with
duplicate global enchantments (Anthems, Rage effects) hits this. Cheap representation nudge:
annotate a hand card whose global effect is already active, e.g. "Primal Rage (already in play -
a 2nd copy does nothing)". LOW priority; folded into the deck62 guide as a card-note as a stopgap.

## 4. (observation, not a change) deck62's record is a matchup-shape floor, not a play problem.
2/6 with two razor losses (-2 v 3; 11 v 13 adj) and two matchup-floor blowouts (Goblin aggro,
affinity+lifegain). It is a turn-8-13 ground/go-wide deck; it beats the midrange/go-wide mirrors
(49, 35) and floors against faster clocks. The guide cannot fix a clock deficit; do not chase the
win-rate with construction-style guide prose for a single-seat matchup floor.
