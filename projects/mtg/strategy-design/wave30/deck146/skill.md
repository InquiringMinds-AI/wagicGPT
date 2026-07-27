# deck146 -> strategy-writing-skill proposals (wave-30)

Context: deck146's SECOND guided corpus. The wave-29 fact-corrections all VALIDATED live this
wave (answer-by-name held -- Lost Mine was option 3 this corpus, the model answered by name;
room-effect-text lines used correctly; no stale gap-prose left). Two new proposals, both drawn
from surfaces that only became observable now (the g5 status-line landing + the first live walker
play).

## PROPOSAL 1 (MODERATE) -- Render-resolves-uncertainty: when an engine adds a STATUS LINE that
answers a model's prior second-guessing, CONFIRM the reasoning shifted from inference to
assertion, and do NOT add compensating guide prose. Wave-29 flagged N-146f (no "dungeons
completed" line -> the model second-guessed whether the anthem was on). The fix was an ENGINE
render addition ("Dungeons completed (you): N ... ACTIVE for you"), NOT a guide line. This wave
the model's language flipped from "the anthem applies *after* completion?" to "I have completed a
dungeon, so my creatures ARE +1/+1 (Nadaar 5/5)". RULE: a model-uncertainty finding whose true
fix is a render/status-line addition should be routed to the engine ledger, and the guide should
stay SILENT on the resolved fact -- adding a "trust the completed line" sentence would be
compensating prose for a gap that no longer exists (the twin of Proposal 3's shed-gap-prose, on
the additive side). The reviewer's job the following corpus is to VERIFY the reasoning shift in
the replies, not to re-teach the fact.

## PROPOSAL 2 (MODERATE) -- Validation transfers across a CARD CLASS's shared decision SURFACE;
do not treat every same-class card as its own unvalidated surface. deck146 carried two
planeswalkers (Lolth, Kaya) as "the unexercised surface." This corpus Lolth was finally cast and
her loyalty play validated cleanly (cast-when-affordable, -3-board-first, +0-value, use-every-turn
-- all guide-adherent, menu rendered with full {card text}). Kaya still never came down. But
Kaya's decision SURFACE -- the loyalty menu render, the +/- discipline, cast timing -- is the SAME
engine surface Lolth just exercised; her -3 is one more loyalty option presented identically. RULE:
once ANY card of a class exercises the shared engine decision surface with acceptable quality, that
SURFACE is validated; a specific same-class card that only differs in its ability TEXT (which the
render supplies verbatim) is a draw-variance residual, not a distinct unvalidated surface, and
should not by itself justify holding a rotation seat. Reserve "unvalidated surface" for a genuinely
different decision SHAPE (a new menu kind, a new cost type), not a new instance of a proven one.

## CARRY-FORWARD (wave-29 proposals now VALIDATED, keep in the skill)
- P2 (no option-number anchoring): confirmed -- Lost Mine drifted from option 2/3 (wave-29) to
  option 3 (wave-30); the answer-by-name guide worked. Ordinals remain the most fragile hard-code.
- P3 (shed gap-prose when a render gap closes): the guide had no stale gap-prose to re-flag this
  wave; the grep-for-"not shown/cannot see/truncated" rail stays worth running every review.
- P5 (teach the steady-state loop, not just entry): the post-completion re-entry loop the guide
  teaches was exercised heavily (up to 7 room branches/game in vs139) and navigated well.
