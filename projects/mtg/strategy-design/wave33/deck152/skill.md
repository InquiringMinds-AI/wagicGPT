# deck152 -> proposals for the strategy-writing skill (wave 33)

Four proposals. P1 and P2 are promotion-grade by my reading (each has two independent witnesses
across waves or seats); P3 and P4 are single-seat and tagged as such. All four are corrections or
sharpenings of rungs this seat itself helped write, which is the pattern worth noting: **every
one of my wave-33 defects is a wave-32 rung applied to the wrong layer, not a rung that was
missing.**

---

## P1 (PROMOTION-GRADE, and it belongs inside HL7) -- A KEEP CRITERION IS A DISQUALIFIER THE PILOT WILL USE. Run it against the DECKLIST *as a disqualifier*, not as a description, and prefer a criterion the engine already PRINTS as a positive.

**Witnesses: deck152 wave-33 (two false mulligans, one of them the vs158 loss) + deck105 wave-32
(the "no 1-drop" mulligan in a deck with no 1-drops).** Two seats, two waves, one shape.

HL7.3 already says: *"run the keep criterion against the DECKLIST and confirm a hand satisfying it
EXISTS at a reasonable rate."* deck152's guide passed that test as written and failed in play,
because HL7.3 checks the criterion in its POSITIVE direction (can a keepable hand exist?) and the
pilot uses it in its NEGATIVE direction (is this hand disqualified?). The deployed line was:

> *"Mulligan only hands with no creatures at all, or one that cannot make GW mana."*

Keepable GW hands obviously exist -- the criterion passes HL7.3 trivially. But read as a
disqualifier it fires on **every mono-coloured opener**, and deck152's 25 lands split 14 green /
15 white with **only 4 lands making both**, so a three-land opener is mono-coloured roughly two
times in five. The pilot obeyed and mulliganed a hand the engine had just told it casts two Ranger
Classes.

**THE RULE, three parts:**
1. **State every keep criterion in its NEGATIVE form and count how often it FIRES against the
   decklist**, not how often it can be satisfied. "Mulligan hands that cannot make GW" is the same
   sentence as "keep only hands that make GW"; only the first form makes the ~40% obvious.
2. **A colour criterion in a mulligan section is a disqualifier by construction, and a two-colour
   deck whose duals are a minority of its lands cannot carry one.** Where the deck lacks fixing,
   HL7's cross-check (deck158 R2, "state the colour requirement first") is correct for a deck with
   ZERO relevant sources; it is actively harmful for a deck with 14 of one and 15 of the other.
   The distinguishing question is not "does the deck have fixing" but **"can the deck cast
   anything at all off one colour?"** If yes, the colour clause must be a tiebreaker, never a gate.
3. **Prefer a criterion the prompt PRINTS as a positive.** deck152's pregame prompt already emits
   `Playing every land in this hand would cover the cost of: <list>` -- an engine-computed
   aggregate, in exactly HL6's escalation form, and non-empty in BOTH false mulligans. The
   revision's floor is *"if that line NAMES EVEN ONE CARD, KEEP"*. This is HL7.2's
   test-over-a-printed-string applied to the keep side rather than the cap side, and it is
   strictly better than any hand-authored count because it already reflects the actual hand.

**And the diagnostic that distinguishes this from ordinary disobedience** (a companion to HL6's
recite-then-exclude tell): **the reply RECITES the engine's coverage list and then declares the
hand dead anyway.** HL6's tell is membership denial (*"zero green sources"* holding a Forest);
this is **COVERAGE denial** -- the pilot concedes every fact and rejects the conclusion, because
its disqualifier is about the DECK'S PLAN, not about the hand. Verbatim, deck152 vs158 s1:
*"This hand has three green mana sources and two copies of Ranger Class, but critically lacks a
source of White mana ... the hand is effectively dead for the first few turns."* No count can
answer that. Only removing the licence can.

---

## P2 (PROMOTION-GRADE) -- HL3.2's SUCCESSOR-BELIEF RUNG NEEDS A NAMED FIRST ENTRY: "generic is not a colour". Whenever a guide teaches the pilot to trust an ENGINE-COMPUTED COLOUR SET, it owes one sentence saying what that set does NOT constrain.

**Witnesses: deck152 wave-33 (deck36 s14/s15 -- a false decline of an offered payable cast, plus
the whole next decision spent on it) + deck105 wave-32 (the same belief re-derived ELEVEN times
inside one reply at deck116 s9).** Two seats, two waves.

HL3.2 predicted the shape and asked reviewers to name the successor when a stopgap retires. Here
is the first one worth carrying forward as a standing item, because it is not deck-specific -- it
attaches to the i12 count line itself, which every deck in the pool now reads:

> `Mana available: 2 total (two untapped sources, tapped automatically when you cast; colours you can make: {g})`

The pilot reads `colours you can make: {g}` as an exhaustive list of what it can PAY, and a cost
of `{1}{G}` as demanding two things from that list. deck152 vs36 s15 verbatim:
*"Ranger Class is {1}{G}, so I need 1 generic and 1 Green. **Do I have generic mana?**"* and, in
the same reply, *"This implies I have two sources but they only make Green?"*

The line is not wrong; it is INCOMPLETE in the one way the pilot notices. **The rule: a rendered
set that answers "which X can I do" invites the inference "everything not in this set is
impossible". Where that inference is false for a whole category (generic pips, colourless costs,
any-colour effects), the guide owes an explicit negative -- and it should be stated as a FALSITY
FACT about the SURFACE, not about the card** (HL9.4's authoring form):

> *"GENERIC IS NOT A COLOR. The {1} in {1}{G} is paid by ANY untapped source; it does not need to
> appear in the colour list, and there is no such thing as 'not having generic mana'."*

**Cheap authoring check, generalisable past mana:** for every engine-computed SET the guide points
at, write the sentence "everything not in this set is impossible" and ask whether it is true. If
it is not, that is a required clause. Applies equally to the target list, the blocker list, and
the attacker eligibility list.

---

## P3 (single-seat, tagged) -- HL3.3's SUPERSEDED verdict needs a fourth trigger: a teach whose subject was a BUG. When an engine fix lands, a guide sentence that DESCRIBED the bug is not stale, it is FALSE, and it must be deleted in the same revision -- but its GENERAL half usually survives and must be re-aimed, not dropped.

HL3.3 defines SUPERSEDED as "a teach whose CLASS is dead and whose work the RENDER now does".
deck152 hit a variant it does not cover: a teach whose class is dead because the render used to be
**wrong** and now is **right**. The deployed paragraph read:

> *"READ THE CURRENT NUMBERS, NOT ANY '(printed X/Y)' TAG: on a battlefield line reading
> 'Moonrage Brute (3/3) (printed 2/2)' ... On a transformed werewolf the printed pair is its OTHER
> FACE's number."*

After i7 both quoted strings have **zero** hits and a transformed werewolf carries **no tag at
all**, so the second sentence is a false statement about the prompt -- the pilot is being told to
expect and reinterpret a tag it will never see. Reported as FIRED it would have survived a freeze
forever (no misplay ever traces to it); reported as FAILED it invites a rewrite of a correct idea.

**The refinement, two clauses:**
- **A guide sentence whose subject is a rendering DEFECT carries the defect's lifetime.** When the
  reviewer's own findings close the defect (N-152d here), the sentence is deleted in the same
  revision, and the deletion is auditable by the zero-hit quoted string -- which HL3.1's sweep
  already produces for free. **Concretely: the HL3.1 sweep's zero-hit list and the wave's CLOSED
  engine items should be cross-referenced, because a string that died because YOUR fix landed is a
  different verdict from a string that died because the emitter changed under you.**
- **The general half survives.** `(printed X/Y)` still fires 51 times at true deltas in the same
  corpus. Deleting the paragraph wholesale would have removed a correct and useful teach. The
  revision keeps *"the FIRST pair is always the creature's real current size"* and re-aims the
  explanation at the tags that DO fire (counters and lords), which is what the render now means.

---

## P4 (single-seat, tagged; a small addition to HL9.4's absent-field rung) -- AN ABSENT-FIELD FIX MUST DISTINGUISH "the value is zero" FROM "the field does not exist", AND THE FIXTURE MUST RIDE THE SECOND CASE.

HL9.4 diagnosed `if (value) print(value)` deleting every legitimate ZERO and prescribed routing
cost emitters through one token function. That shipped as i5 and it works for its target: 222
correct `Mox Jet {0}` / `Mox Sapphire {0}` renders at my seat, the deck36 confabulation class
addressed. It also introduced **166 false renders at my seat alone** -- `Day {0}`, `Night {0}`,
`Moonrage Brute {0}`, `Tovolar's Packleader {0}` -- because a designation marker and a transformed
back face have **no mana cost object at all**, and the new guard cannot tell that from a cost of
zero. (Verified against the wave-32 corpus: all four render bare there, so this is i5's delta.)

The failure is the exact mirror of the one HL9.4 was written against, and it is predictable at
design time: **`if (value) print(value)` deletes real zeros; `print(value ?? 0)` invents zeros
where the field is absent.** Both are one-line bugs and both produce a confident wrong card fact
that no audit rung can see, because the render still parses and resolves correctly.

**THE RULE (one clause appended to HL9.4):** when fixing a suppressed field, enumerate the
populations that reach the emitter with the field ABSENT, and state which token each gets. Then
**write the fixture on the absent population, not the zero one** (HL5.4). The i5 fixture, if it
exists, rides Moxen -- the branch that already produces the right answer once the guard is
inverted. The branch that needed a test is `Moonrage Brute` on the battlefield.

**Free discriminator when the two populations are hard to tell apart in code:** they are usually
separable in the DATA. Here, Moxen carry `mana={0}` in `borderline.txt`; Moonrage Brute,
Tovolar's Packleader, Day and Night carry **no `mana=` line at all**. That is a one-grep design-time
check, in the same family as HL5.1's "state which representation the scan sees".

---

## NOT PROPOSED (recorded so a later wave does not mistake silence for absence)

- **The attacker-decline wording corner** (deck139 s8 this wave, deck139 s9 last wave: one eligible
  attacker declined into a wall). Same matchup, same shape, two corpora, both games WON, one
  instance in 23 and one in 25. Still below the bar; a third witness from a DIFFERENT opponent
  would promote it.
- **A front-loaded blocker-eligibility render** (`B1. [may block: A1] ...`). One witness
  (deck36 s8). The guide-side clause is cheaper and is in this wave's revision; if the illegal
  assignment recurs at any seat with the clause deployed, the render change earns its promotion.
- **The phase-header contradiction** deck36 flagged in wave-32 with deck152 as its only witness:
  **zero occurrences at this seat this corpus.** I am the seat that produced last wave's evidence
  and I cannot reproduce it. The watch should not promote on my data.
