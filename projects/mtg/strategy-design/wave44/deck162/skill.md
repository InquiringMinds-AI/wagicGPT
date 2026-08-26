# deck162 — proposals for the strategy-writing skill (wave-44 evidence only)

## S-K. A DELETION IS A CHANGE AND NEEDS A PREDICTION, NOT A COST ESTIMATE
Wave 43 deleted deck162's ATTACKING section down to stops (S6) on the ground that 21 lines governed
4 decisions. This corpus falsified it twice in six games: the deleted D3 Master-of-the-Feast gate
was exactly the rule needed at vs deck123 seq 13 (Master swung into a 6-blocker board, died to a
3-Vampire gang block, game lost at -21), and the surviving section - now all prohibitions - left
the model with no rule saying a "(you kill it, your attacker lives)" tag means SEND, so it declined
a strictly free attack at vs deck152 seq 19.
PROPOSAL: the skill's decision-value-per-token criterion is measuring the wrong thing when the
prose it cuts is a LOW-FREQUENCY, HIGH-COST gate. Add a second test before any deletion: what does
the section look like with this removed - are the remaining rules jointly SUFFICIENT to answer the
decision, or only to forbid answers? A section that is 100% prohibitions after a cut is a defect
regardless of its token count. And record deletions as PREDICTIONS ("removing X costs nothing"),
adjudicated like any other edit - wave 43 did exactly this, which is why the falsification is
clean and cheap to see. Keep that practice; tighten the criterion behind it.

## S-L. PROHIBITION-ONLY SECTIONS PRODUCE PARALYSIS, NOT SAFETY
deck162 attacked in 2 of 6 attack windows this corpus. Its ATTACKING section, post-S6, contained
three sentences and every one of them said when NOT to attack. The model then generalised the stop
onto a board where the render had explicitly said its attacker survives, and wrote 300 words of
reasoning inventing a trade the tag had already ruled out.
PROPOSAL: the skill should require every combat section to carry at least one AFFIRMATIVE rung
naming the rendered string that means GO. Stops are cheap to write and cheap to over-apply; the
positive branch is what bounds them. This generalises the wave-43 finding on S3 (a wrong teach
corrected) into a structural rule: a rendered outcome vocabulary needs full coverage, not just
its dangerous half.

## S-M. TEACH THE COLLAPSED FORM OF A RENDER FIELD, NOT ONLY ITS ENUMERATED FORM
Every guide in the pool teaches the "their untapped blockers" tag in its enumerated form
("Perimeter Captain (0/4) (neither dies); Vampire (1/1) (both die)"). None mentions its COLLAPSED
form ("they have 6 untapped creatures that could block this one, biggest Bloodline Keeper #1 (3/3)
(you kill it, your attacker lives)"), which is what the render emits above a threshold - and which
carries a very different truth value, since it prices exactly one blocker on a board where a gang
block is available. The model treated the two forms as the same sentence.
PROPOSAL: when a render field has more than one rendering (collapsed / summarised / truncated),
the guide must name every form the seat can actually see, and the skill should ask reviewers to
enumerate a field's renderings from the corpus before writing a rule about it.

## S-N. A RULE THAT ENDS AT ONE ASK LEAVES THE NEXT ASK UNGOVERNED
deck162's Rule 3 says "Cast Shield Sphere {0} ... take it every single time it is listed" (5/5 this
corpus) and its MULLIGAN section ends at the keep. At the bottoming ask - a different ask, in the
same window - it bottomed the Shield Sphere. Neither rule was violated; they simply never met.
PROPOSAL: the skill should treat a multi-ask sequence (mulligan -> keep -> bottom; cast -> target ->
mode) as ONE governed window and require each guide section to say what happens at every ask in the
sequence, or explicitly to hand it to another section by name.
