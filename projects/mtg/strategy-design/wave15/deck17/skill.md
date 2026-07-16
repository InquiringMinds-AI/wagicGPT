# skill.md - deck17 guideless-first-pass method learnings (wave-15)

Proposals for the strategy-writing skill's guideless-first-pass doctrine. Single-seat
evidence per the epistemics rule is a WATCH, not an adopted method line; flagged as such.

## WATCH-1 - On a guideless first pass, a deck whose ENTIRE mana cost profile is one
idiosyncrasy makes that idiosyncrasy the #1 rule, ABOVE the win engine.
deck17 is ~all hybrid ({u/b}/{w/u}) off mono-Island mana. The single largest source of both
lost decisions AND reasoning-tax (which drives adjudication losses) was the model's stable
false belief that hybrid pips need the off-color, re-derived from scratch at nearly every
cast in every game (and once mis-counting {u/b}{u/b}{u/b} as 6 mana). The skill already lists
"hybrid {U/B} read as needing a Swamp" in its false-world-model examples; the first-pass
learning is that when a deck's cost profile is DOMINATED by one such belief, the corrective
FACT earns the #1-RULE slot over the engine, because the belief taxes/vetoes the engine's own
plays. Tie it to the visible list ("if a Cast option is listed, it is payable") AND give the
positive follow-through ("cast the biggest flyer offered"), because the belief's downstream
cost is under-deployment, not just tax. (2 decision-level witnesses vs44 s7, vs135 s11.)

## WATCH-2 - Derive the DEFENDER set explicitly before writing the attack floor; a
guideless pilot treats every creature as a potential attacker.
deck17 runs 8 defenders (Gatewarden x4, Plumeveil x4) it must never swing with, yet the model
named them as attackers and called a defender "a future threat." The engine silently drops
ineligible attackers, so the mistake is invisible in `choice` (it looks clean) - you catch it
only by reading the `ATTACK:` line against the card's Defender tag. First-pass move: when a
deck mixes attackers and walls, name the walls as an explicit exclusion INSIDE the attack
floor, and score attacker declarations against creature eligibility, not the recorded choice.

## WATCH-3 - The attack-floor-does-not-transfer lesson (skill's deck135 wave-5 rung) recurs
on a fresh deck's FIRST corpus and is confirmed for a flyers deck.
Even with the cast floor effectively supplied by core, deck17 went passive at the ATTACK seam
vs140 (3 attack windows in 33 turns; opponent to 64 life). A guideless flyers deck needs its
own per-turn attack floor keyed on a board-readable fact ("opponent has no flyer/reach ->
all your flyers connect"), written the first pass, not deferred to a revision. Reconfirms the
existing rung on a new archetype; no new method needed - just: on a first pass for any
evasion deck, WRITE the attack floor immediately, do not wait for the passivity to appear.

## Reconfirmed (no change)
- Core's "every listed option is legal AND payable" rule is load-bearing here - it rescued
  the final cast under maximal hybrid confusion. The guide augments it (kills the tax); it
  does not replace it.
- Positive framing + a closed exception for the empty choice (the "no attackers only when..."
  form) transfers directly to a new deck; used verbatim in strategy.txt's attack floor.
- Match deck44's density (~one screen). deck17's guide landed at 85 lines / 6.3KB - at the
  top of the target band because the deck needs a full steal section + defender facts + the
  hybrid rule; trim on the first revision if any section shows as pure tax.
