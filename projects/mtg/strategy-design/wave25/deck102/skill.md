# skill.md proposal — deck102 wave 25

ONE proposal, plus a RETIREMENT note on the wave-24 commit-lock sub-shape (now structurally dead).
No change proposed to the crutch-lifecycle, reveal pick-ONE, INDEX-WINS/echo, or attacker-scoring
rungs — all held clean (0 flip-thrash, correct Tergrid cast, 0 fallbacks, disciplined Smallpox).

## RETIREMENT — wave-24's method-3 THIRD commit-lock sub-shape (example-seeded out-of-range index) is DEAD; downgrade it to a historical footnote.
Last wave I added a third commit-lock sub-shape: an out-of-range line-1 `CHOICE:` index seeded by the
reply-protocol EXAMPLE + a stale plan. The wave-25 b1 de-fang ("CHOICE: 3 (Cast Example Card)")
removed that surface: 0 fallbacks corpus-wide at this seat, and the shape did not reproduce. The
diagnostic rung (when an unparsed reply carries an OUT-OF-RANGE coded line, check the example index +
stale-plan seeds) is still CORRECT and cheap to keep, but it should be tagged HISTORICAL/structurally-
mitigated, not an active watch. Do not spend reviewer attention hunting it unless a future core-prompt
change re-introduces a literal example index.

## PROPOSAL (single-seat, promotion-gated): a PREGAME MULLIGAN rung distinguishing HAND-READ FIDELITY from HAND-EVALUATION JUDGMENT.
The pregame mulligan is now this seat's dominant decision-leak surface — TWO consecutive waves, TWO
DIFFERENT failure classes:
- wave-24 (vs59): a FIDELITY failure — the model asserted "zero Swamps" on a 2-Swamp hand (a
  miscount). Not guide-fixable (attention slip). My wave-24 rung: diff the model's CLAIMED
  land/color/curve count against the LITERAL `Your hand:` line.
- wave-25 (vs109): a JUDGMENT failure — the model READ the hand correctly (3 Swamps, named Hymn) but
  mulliganed a keepable disruptive hand chasing a "turn-1 play." The claimed-vs-actual diff would MISS
  this (the read was accurate). This one IS guide-addressable (the guide's "early" was undefined ->
  the model resolved it to turn-1).

Proposed rung refinement — for every mulligan/bottom decision, run TWO distinct checks and ROUTE by
which fails:
1. **Hand-read FIDELITY** (route -> MODEL/notes, not guide): does the model's claimed land/color/curve
   count match the literal `Your hand:` line? A mismatch is an attention slip the guide cannot fix.
2. **Hand-EVALUATION judgment** (route -> GUIDE if the guide's keep criteria are ambiguous/silent on
   the case): did the model ship a hand that SATISFIES the guide's own keep criteria? If yes and it
   mulliganed anyway, the guide has an exploitable ambiguity — sharpen it. deck102 vs109: the hand met
   "2-4 Swamps + an early black play (Hymn listed)" verbatim; the model over-indexed on turn-1 speed
   the guide never required. Fix = define the fuzzy term ("early = by turn 2") in the guide, don't just
   note the model erred.

The GENERAL skill lesson (candidate for synthesis): when a decision fails and the guide's own criteria
were SATISFIED, the owner is the guide's AMBIGUITY, not the model — resolve the fuzzy term. When the
guide's criteria were VIOLATED by a misread of the inputs, the owner is the model — route to notes and
do NOT bloat the guide with text the model already had. The claimed-vs-actual diff is what
distinguishes them. Tag single-seat; promote to a standing pregame-review step if a 2nd seat shows
either a fidelity miscount OR a judgment over-mulligan of a hand that meets its guide's keep criteria.

(The one guide edit this wave — "early = by turn 2" — followed from check #2. No other guide edit
followed: the steal line got no test, and Tergrid/flip-thrash/Smallpox/painland all held.)
