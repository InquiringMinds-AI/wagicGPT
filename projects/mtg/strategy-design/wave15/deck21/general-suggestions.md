# deck21 seat - general (cross-deck) suggestions (wave-15)

Non-deck-specific observations; route to the harness/engine owner, not any guide.

1. REASONING-TASK LOOP GUARD (highest value). deck21 is the tax epicenter: multiple replies
   of 2,000-3,000 words that are the SAME sentence repeated dozens of times (vs135 s24:
   "maybe the Flooded Strand is a snow land? No." ~40x; vs133 s8; vs140 s35). The
   unparsed->heuristic guard catches them AFTER the full generation cost (latency + a lost
   decision to the heuristic). A cheap n-gram/line repetition detector that early-stops
   generation when the same ~15-token span repeats N times would recover latency AND let a
   truncated-but-early reply still carry a usable CHOICE. This is archetype-independent but
   worst on aggro decks whose "should I hold / trade / attack" branches invite oscillation.

2. `[treason]` KEYWORD ANNOTATION IS MISLEADING. Spark Elemental's end-of-turn sacrifice is
   tagged `[treason]` in the option/board annotator. "Treason" normally means Act-of-Treason
   (steal a creature); the model only recovered the real meaning by reading the full rules
   text, at real token cost. Rename the tag to something like `[sacrificed at end of turn]`.

3. THE CORE "hold instant-speed interaction" LINE MIS-FIRES FOR BURN-AGGRO. It caused a
   game-losing hold at deck21 vs140 (see skill.md D2). Two decks now need their guide to
   override it (deck21, deck109). Consider whether the core line should be conditioned
   ("...unless your instants are burn you are using as a clock") rather than repeatedly
   countermanded per-deck. This is a candidate for the core body, but it is single-corpus at
   this seat - flag, do not cut/add a core line on deck21 alone (doctrine: single-seat items
   cannot touch core lines). Cross-seat trigger: a 2nd deck's reviewer finding an offered
   burn/instant HELD as interaction against a deck that wants it on a clock.

4. MIXED-FORM ATTACKER DROP recurred (corroborates wave-13 ledger). deck21 vs135 s29:
   `ATTACK: A1, A2, Scuzzback Scrapper` - the parser consumed A1,A2 and the trailing name
   "Scuzzback Scrapper" was dropped. Outcome-NEUTRAL here (Scuzzback was summoning-sick and
   ineligible anyway), but it is another witness for the attackers name->index reconcile arm.
