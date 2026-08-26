# Strategy-writing skill proposals — from the deck125 seat (wave 45)

Only what this seat's evidence supports. Amendment numbering continues from #47.

## Proposal S-A — A RULE THAT NEEDS TWO SUBTRACTIONS WILL BE DONE WRONG; COLLAPSE IT TO ONE.

deck125's X-sizing rule asked for `7 - (handline - 1)`: read a number, subtract 1, subtract the
result from 7. It was obeyed 6/12. The identical rule written as `8 - handline` — one subtraction
off one rendered number — reproduces every one of those twelve verdicts with no intermediate value
to lose. The wave-44 edit that introduced the intermediate (E3, "subtract the Revelation itself
from the hand count") was correct about the game and wrong about the executor: it made the
correction VISIBLE as a step instead of folding it into the constant.

Proposed text: *When a rule computes a number from the render, write it as ONE operation on ONE
rendered field. Fold every correction term into the constant rather than making the pilot apply it
("X = 8 minus your hand line", never "count your hand, subtract 1 for the spell you are casting,
then subtract that from 7"). If the arithmetic cannot be collapsed, it is two rules and the second
one will not fire. Keep the reason for the constant in one clause after the rule, where it explains
but is not a step.*

## Proposal S-B — A PROHIBITION AND A FORMULA IN THE SAME SECTION MUST SHARE ONE THRESHOLD.

The live guide forbade casting Sphinx's Revelation "at hand 7 or more" and, four paragraphs later,
gave a ceiling formula that permits X=1 at a hand line of 7. Two numbers for one boundary; the
model cast at hand line 7 and at hand line 8. Rewriting both at 8 (hand line 8 => ceiling 0 => do
not cast) removed the seam without changing any verdict.

Proposed text: *Where a section carries both a gate ("do not do this when ...") and a formula that
sizes the same action, DERIVE the gate from the formula and state it in the formula's own units.
Two independently-stated thresholds for one boundary is a contradiction the pilot resolves by
picking whichever it read last.*

## Proposal S-C — A CATEGORY BAN NEEDS ITS OWN RENDERED TAG, NOT A NUMERIC PROXY.

The counter and sweeper sections both gated on printed power ("power 0 or 1: let it resolve";
"cast the sweeper at printed power 4 or more"). Four counters and two sweepers went to bodies whose
printed power satisfied the guide's own let-it-resolve side but that the numeric framing did not
make salient: Pride Guardian 0/3, Wall of Omens 0/4, Fog Bank 0/2, Shield Sphere 0/6. The Path
section, which names the `[defender]` TAG instead of a number ("a 0/4 wall is never a Path target";
"a creature tagged [defender] still never gets the Path, at any life total"), was obeyed — 0
violations across the corpus.

Proposed text: *When the class you want excluded has its own tag on the render, gate on the TAG,
not on a number that happens to correlate with it. `[defender]` is a fact about what the creature
can ever do; `printed power 0` is a fact about this instant. Where both a tag rule and a numeric
rule cover the same card, state the tag rule first and mark it as having no exception — the numeric
rule then only has to handle the cards the tag does not cover.*

## Proposal S-D (evidence-limited) — THE LAND DROP IS A RULE, NOT A PREAMBLE.

Both guides on this seat described the land drop only inside an opening paragraph ("Every turn is
the same three actions: play a land, answer ..., ping ..."). Neither had it as a numbered rule or
in DECIDING SITUATIONS. deck125 declined 14 of 81 turn-level drops, deck126 2 of 41, and in three
different games the decline chain is inside the causal path of a loss. By contrast the wave-44 E1
edit, which hoisted the sweeper gate INTO the situations list, held at 41/42 this corpus.

Proposed text: *Every action the deck takes on essentially every turn belongs in the guide as its
own imperative rule AND as a line in the situations list, however obvious it is. "Obvious" is a
property of the reader who already knows the game; a costless, always-correct action stated only as
scene-setting is an action the pilot will skip when a plan sentence points elsewhere.*
