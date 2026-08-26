# Strategy-writing skill proposals from the deck125 seat (wave 46)

Only what this corpus's evidence supports. Each carries the observation that would falsify it.

## S1 (proposed amendment) - A RULE MAY ONLY ASK FOR A NUMBER THE PROMPT PRINTS
**Claim.** A guide rule whose input is a quantity the model must DERIVE by counting an unnumbered
rendered list will fail at a rate that no rewording fixes. Rules keyed to a printed integer, or to
a constant, hold.
**Evidence, one seat, one wave.**
| rule | input | conformance |
|---|---|---|
| RULE #0 land drop | a printed option row | **40/40** |
| sweeper 0-creature gate | `of which N are creatures` (printed integer) | **123/123** |
| counter `[defender]` clause | a printed tag on the stack line | **39/39** |
| Staff ping | a printed option row | **75/75** |
| Sphinx X sizing | *count the cards on `Your hand:`* | **4/17** |
| Staff of Nin tap-out | printed integer, but a four-way branch over it | **4/7** |
The X rule and the Staff rule are the two that ask for work on top of a field; the X rule asks for
a count that is not a field at all, and it is the worse of the two by a factor of three. The direct
proof that the model is not merely disobeying: 3 of the 4 replies that stated a hand size in words
stated it WRONG, once as "hand size 6" against an eleven-card list.
**Deployment.** When a rule's input is not rendered, the guide's job is to say so and pick a
constant, and to record a SUNSET TEST naming the render that would retire the constant (see this
wave's W3). The engine item goes to the seat file, not into the guide as an instruction to count
harder.
**Falsifier.** A rule keyed to an unrendered count that conforms above 80% in a later corpus.

## S2 (proposed amendment) - COLLAPSE A BRANCH OVER ONE FIELD INTO ONE CONSTANT
**Claim.** Where a rule branches to several thresholds off the same rendered field, the branch is
the failure, not the field. Collapsing to the single strictest constant that reproduces the
corpus's correct verdicts is worth more than the cases it over-refuses.
**Evidence.** deck125's Staff rule ("8 with an Essence Scatter, 9 with a Cancel or Dream Fracture,
11 with only a Fall of the Gavel, 6 with none") has sat at 6/11 -> 3-4/7 -> 4/7 across three waves
while every single-field rule on the same seat went to ceiling. The four conforming casts read
`Mana available:` 17, 10, 13, 22; the three violations read 6, 7, 6. A flat 9 separates them
perfectly and costs one turn on an Essence-Scatter-only hand.
This is amendment 61's WHY line pointed at a different target: the WHY exists so a later wave can
judge whether the number is still right; S2 says the NUMBER OF numbers is itself a variable.
**Falsifier.** A multi-threshold branch that reaches ceiling while a single-constant rule on the
same seat does not.

## S3 (proposed amendment) - A RULE THAT NAMES A CLASS MUST NAME A MEMBER
**Claim.** An abstract class ("anything printed (artifact) with no P/T") does not fire; the same
class with two named cards and one sentence of consequence does.
**Evidence, within one section, one wave.** deck125's counter section has two let-it-resolve
clauses. The `[defender]` clause names Pride Guardian, Wall of Omens, Fog Bank and Shield Sphere by
name with P/T, and went from 4/33 violations to **0/39**. The `(artifact) with no P/T` clause names
nothing, and took **2/39** counters this wave (Chromatic Lantern, Talisman of Impulse) - one of
them inside a loss. The two clauses are eight lines apart in the same guide, read by the same model,
in the same windows.
**Falsifier.** A named-member class that fails while an unnamed sibling in the same section holds.

## S4 (observation, not yet an amendment) - THE THIRD CONFIRMATION OF THE TWO-PLACE HOIST
The wave-44/45 finding that a rule absent from DECIDING SITUATIONS does not fire now has a third
instance and its first CLEAN one: the sweeper 0-creature gate went 3/14 -> 1/9 (hoisted) ->
**0/123**. Against it, the Emrakul absolute - present in the guide BODY in its most emphatic
possible form ("If 'Cast Emrakul, the Aeons Torn' is ever actually printed in your numbered list,
take it") and absent from the situations list - was declined **4 of 5 times**. I am not proposing
new wording; I am proposing the pattern be treated as established rather than as a live question,
so future reviewers stop re-testing it and spend the wave elsewhere.
