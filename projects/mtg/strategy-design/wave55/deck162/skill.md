# deck162 -> strategy-writing skill proposals (wave 55)

## S-1 (PROPOSED) - a BRAKE should ship with its RELEASE, read off the same screen

**Evidence.** deck162's K-of-0 brake ("an engine row printing `converters on your battlefield: 0`
with a Mine already out is not cast") is unconditional. Of its three violations this corpus, **two
were correct plays and one was the break**, and the discriminator was on the prompt the whole time:
a punisher NAME on the `Your hand (N cards):` line. `162v123` s18 (Underworld Dreams in hand) is
the cast that produced the `their next draw step draws 5 cards` forecast that won the game 20/0
five turns later; `162v125` s98 (no punisher in hand) is the break. An unconditional brake made the
seat's two best engine casts of the corpus into rule violations.

**Proposal.** When a guide adds a brake, the skill should require the author to state the condition
under which the brake RELEASES, in the same literal-keyed form as the brake itself - or to state
explicitly that no release exists. A brake without a release is not surgical, it is a category ban,
which is the exact failure the 162-A re-key was written to fix one wave earlier (a brake keyed to
"draw engines" firing on cards outside the category). The release is the second half of the same
lesson.

**Falsifier.** wave-56 deck162 takes of a `{feeds:}` row at K of 0 with a Mine already out and NO
punisher name on the hand line: **0/N**. Takes WITH a punisher name on the hand line are the
control and must stay non-zero.

## S-2 (PROPOSED) - re-key a plan-sentence prohibition to the sentence the corpus actually wrote,
and expect it to mutate

**Evidence.** deck162's guide bans "keep mana open" and the plan sentence "cast Underworld Dreams
first". The wave-55 residual (3 windows) is the same behaviour under a new sentence:
**"cast Underworld Dreams or Fate Unraveler as soon as possible"**. Wave 54's ban closed the two
phrasings it had seen and the model produced a third.

**Proposal.** A prohibition on a PLAN SENTENCE is a prohibition on a paraphrase class, and
enumerating the observed strings has now failed twice in this deck. The skill should push authors
to state the STRUCTURAL test instead ("your plan names a card that is not one of the numbered rows;
that card is not competing for this mana"), keep the observed strings only as the WHAT THIS COST
receipt, and expect a fresh paraphrase next corpus - i.e. an evocation with an enumeration attached
as evidence, not an enumeration doing the work. This is the strongest instance of the
explicit-vs-adaptable trade in this seat's corpus, and it is measured: 10 windows -> 3 under an
enumeration, with the residual being a paraphrase the enumeration did not list.

## S-3 (RECORDED) - a re-key that halves a window count with the record unchanged is still the
right evidence

deck162 went 2-4 -> 5-1 and its 162-A window count went 10 -> 3. Under the new-baseline rule the
record is not evidence about the edit; the window count is. Recording it because this is the
cleanest paired example in the pool of a guide lane where the two numbers move together and only
one of them is admissible.
