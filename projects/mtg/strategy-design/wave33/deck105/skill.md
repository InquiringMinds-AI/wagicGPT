# deck105 — strategy-writing-skill rung proposals, wave 33

Against the CURRENT edition, `wave32/strategy-writing-skill.md`. Every proposal carries a
wave-33 witness from this seat; where a proposal sharpens an existing headline it says which.

---

## P1 (sharpens HL3 rung 1 + rung 2) — A STOPGAP CAN OUTLIVE ITS OWN FALSITY FACT. THE REMOVAL
## CONDITION IS A STRING GREP, AND AN INVERTED FALSITY FACT IS A P0 GUIDE DEFECT.

HL3 rung 1 says grep every quoted string and repair the zero-hit ones. This seat shows the shape
that rung does not name: the deployed guide's stopgap section was built on a NEGATIVE claim about
the render —

> *"The board does not print the poison count. Work it out ONCE, in ONE clause..."*
> *"Neither counter is announced in the log - add both by hand."*

Neither sentence is a quotation, so the quoted-string sweep does not see them; both were TRUE when
written and both are now FLATLY FALSE (the board prints the count in **63/63** of this seat's
prompts; both Ichor Rats counters narrate). And the procedure they introduce keys on
`"Damage: N dealt by <one of YOUR creatures> to the opponent"`, which has **ZERO occurrences** in
the entire corpus — the poison half of that emitter was replaced.

So a stopgap can fail in three independent ways at once, and only one of them is currently swept.
**THE RULE, three parts:**
1. **A stopgap's REMOVAL CONDITION is executable or it is not a removal condition.** Write it as
   a grep at the time you write the stopgap ("delete this section when
   `grep 'Poison counters (' <corpus>` is non-empty"), and run it as the FIRST act of the next
   revision, before the log read.
2. **A guide sentence asserting what the render does NOT do is a FALSITY FACT and inherits the
   same versioned-dependency status as a quoted string.** Extract them alongside the quotations
   (they have a recognisable form: "the board does not", "is not printed", "is not announced",
   "you will not see") and check each one against the corpus. An INVERTED falsity fact is worse
   than a dead quotation: it actively instructs the pilot to ignore a surface that now exists.
3. **When a stopgap's procedure and its falsity fact both die, DELETE THE SECTION - do not edit
   it.** Editing preserves the framing ("you must work this out"), which is precisely the belief
   the render has retired.

---

## P2 (NEW RUNG) — A THRESHOLD RULE MUST NAME ITS NUMBER BY THE RENDER SUBSTRING THAT PRECEDES
## IT. A BARE VARIABLE LETTER IS A DEFECT AT THE SEVERITY OF A FALSE CARD FACT.

The deployed guide's #1 rule read: *"read the engine's own line - 'Your life: 20. Unblocked, these
attackers deal up to 5 - you would be at 15 - NOT lethal'. That 'you would be at N' number is
accurate... N is 9 or more -> answer 'BLOCKS: none'. N is 8 or less, or the line says LETHAL ->
block now."*

The rendered line contains TWO numbers. The pilot bound `N` to the damage, not to the resulting
life, then discovered that its own guide's worked example disagreed with its binding and spent
7,788 chars on a LAND DROP litigating it (`105v146 s11`, verbatim: *"5 is not 9 or more. So
according to the rule, we should block. But the example says we don't. This is a contradiction."*).
Two decisions later, at 10 life against a 9-power swing, it wrote *"re-reading the guide's specific
math: 'N is 9 or more -> BLOCKS: none' ... Therefore, I block with none"*, went to 1 life and lost
the game.

This is not the same defect as HL2's self-contradiction (two sentences that disagree). Here ONE
sentence is ambiguous and the example is what exposes it — the guide is internally consistent
under the intended binding and internally contradictory under the other. **A reviewer proof-reading
their own rule cannot see it, because they already know which number they meant.**

**THE RULE, three parts, all cheap:**
1. **Never introduce a bare variable letter for a rendered quantity.** Give it a CONTENT NAME that
   states which quantity it is - "YOUR LIFE AFTER", "their poison total", "the attacker's printed
   power".
2. **Anchor the name to the render substring it follows**, and say which number it is NOT:
   *"Call the number that comes after 'you would be at' YOUR LIFE AFTER. In that example YOUR LIFE
   AFTER is 13, not 4. It is never the damage number."*
3. **Every worked example in the guide must be re-derived from the RULE, in the rule's own
   vocabulary.** A pre-ship check: for each threshold rule, take each of its examples and evaluate
   the rule against it explicitly. If any example needs a different binding to come out right, the
   rule is ambiguous - not the example.

---

## P3 (NEW RUNG) — A GATE KEYED ON A CURRENT-STATE NUMBER MUST CARRY A GROWTH CLAUSE WHENEVER THE
## RENDER PRINTS ACCUMULATION.

All four wave-33 losses at this seat are against boards that GREW, and the growth is rendered
completely and honestly: `Counter added to Orc army: +1/+1 (now 3/3)` -> `(now 6/6)` ->
`(now 9/9)`, plus `[counters: 9x +1/+1]` and `(printed 0/0)` on the board line. The guide's gate
priced only the CURRENT swing, so it read "safe" every turn until the turn it read "dead":

> `105v158 s9`, at 17 life against a 6/6 that had been a 3/3 two turns earlier:
> *"According to your strategy guide, you only block if the incoming damage would reduce your life
> to 8 or fewer. Since 11 is safely above that threshold, blocking is unnecessary."*
> Two turns later a 9/9 plus a stolen creature killed it from 11.

**THE RULE:** before writing any threshold on a board quantity, grep the corpus for whether that
quantity ACCUMULATES in the render (`[counters: Nx +1/+1]`, a `(printed X/Y)` delta, an amass or
level counter, a growing token). If it does, the threshold is evaluated against the NEXT value,
not this one, and the guide says how to get the next value from a printed string - e.g.
*"if any attacker shows '[counters: Nx +1/+1]', or its printed power is bigger than it was the
last time it attacked, subtract its power from YOUR LIFE AFTER one more time."* This is a
render-anchored forward look and it carries no frequency claim, so it is HL6-clean.

**Why this is not covered by "block when the line says LETHAL":** the LETHAL line is a
one-turn-horizon statement by construction, and a linear deck that must be alive in three turns
cannot be steered by a one-turn horizon.

---

## P4 (sharpens HL2) — THE CARD-FACT LEDGER NEEDS A SECOND CLAIM TYPE: RENDER FACT, ADJUDICATED
## BY A CORPUS GREP.

HL2's ledger has one row shape: claim / primitive line / TRUE-FALSE. Two of the 17 claims in this
seat's audit are not adjudicable that way:

- *"Ichor Rats ... Neither counter is announced in the log - add both by hand"* sits inside a CARD
  entry, passes any primitive check (the card does give each player a counter), and is FALSE about
  the prompt.
- *"The board does not print the poison count"* is the same shape at section scope.

**THE RULE:** the ledger gets a TYPE column with exactly two values. **CARD FACT** is adjudicated
by the shipping primitive AND real Oracle text (both, per the existing hard gate). **RENDER FACT**
- any claim about what the pilot will or will not SEE - is adjudicated by a verbatim grep of the
corpus prompts, and its evidence cell holds the hit count. A guide sentence that mixes the two
(*"it does X, and the log does not say so"*) is split into two rows. The cost is one extra grep
per render claim; the miss it catches is a guide instructing the pilot to hand-compute a number
the engine now prints in every prompt.

---

## P5 (completes wave-32 HL1 rung 3) — A RE-RUN SEAT REPORTS THE OPPONENT-BEHAVIOUR DELTA AT THE
## FIXED SEAM AS ITS PRIMARY EVIDENCE, BEFORE THE WIN TABLE.

Wave-32 HL1 rung 3 tells a winning seat to audit the opponents' surface and report CONFOUNDED. It
does not say what the seat that RE-RUNS the confound must report. The win table is the weakest
available statement of it: 6/6 -> 2/6 is 6 games of a noisy variable. The strong statement is one
counted line at the seam that was fixed:

> infect blocker windows ending in a declared block: **8/12 (wave-32) -> 10/10 (wave-33)**

That number is n=22, it is measured at the OPPONENTS' seats where the fix landed, and it is what
makes the record delta interpretable rather than merely observed. **THE RULE:** when a corpus
re-runs a seat whose prior record was declared CONFOUNDED, the seat's headline is
`<behaviour at the fixed seam>, before and after`, and the win table is reported as its
consequence. Both halves are required: a behaviour delta with no record delta is a fix that did
not matter, and a record delta with no behaviour delta is not attributable to the fix at all.

---

## P6 (instrument) — A COMMIT/RETRACTION COUNTER MUST BE DEFINED ON THE ANSWER LABEL, NOT ON THE
## PLAN LINE.

`commit_retracted` fired **0 / 240** times across both seats of this seat's six games, while
**7 / 240** replies (5/123 at deck105) end on a different answer label than they begin with. The
field misses them because the re-commit happens BEFORE the `PLAN:` line, leaving
`post_plan_overrun` at 0. The two decision-relevant misses at this seat were both HARMFUL and one
was fatal (`105v146 s15`: `BLOCKS: B1:A2, B2:A1` -> scratch text -> `BLOCKS: none` -> 1 life ->
loss; `post_plan_overrun` 0, `commit_retracted` False).

**THE RULE for any future measurement of this class:** compute it as
`first answer label != last answer label`, over the whole reply, independent of where `PLAN:`
falls. `post_plan_overrun` remains useful as a VERBOSITY meter; it is not a retraction meter.
(Routed to general-suggestions.md as evidence for the owner's protocol ruling, not as a proposal.)

---

## Method note — HL8 do-no-harm was applied and one candidate rule was DECLINED

The vs158 loss also has an attack-side reading: deck105 attacked with both untapped creatures on
turn 7 and therefore had no blocker available on turn 8. A "hold a blocker back" rule would sit
directly in front of the seat's strongest preserved behaviour (**19/19 full attacker
declarations, 0/29 cast-nothing**), on **n=1**. It was not written. The block gate rewrite reaches
the same game one decision earlier (`s9`, where a chump block costs one body and zero life) without
touching the attack floor. Recording the decline so a later wave does not read the omission as an
oversight; if a second seat produces the shape, the attack-side rule is the next candidate.
