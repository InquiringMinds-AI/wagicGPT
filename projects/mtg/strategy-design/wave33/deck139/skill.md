# deck139 -> proposals for the strategy-writing skill, wave 33

Five proposals. **P1 and P2 are the wave's whole story at this seat and I believe they are
promotion-grade**: P1 is the counter-lesson to a rung the skill already carries and which this
seat obeyed to its own cost; P2 is the boundary condition on wave-32 HL6, discovered by the fix
HL6 prescribed. P3-P5 are smaller and one of them (P5) is a correction to my own wave-32 rung.

---

## P1 (PROMOTION-GRADE) -- A ZERO-OCCURRENCE COUNT ON A CLASS THE RENDER DOES NOT COVER IS EVIDENCE THE TEACH IS WORKING, NOT EVIDENCE THE CLASS IS DEAD. THE DEMOTION/DELETION VERDICT REQUIRES NAMING THE SURFACE THAT WILL HOLD THE LINE AFTER THE SENTENCE IS GONE.

**The witness, and it is this seat's own error.** The wave-31 deck139 guide carried
*"Every creature in this deck is non-Human, so any of your creatures [can be the host]"*. The
wave-32 corpus produced **zero** replies asserting a deck creature was Human, and a **6/7** mutate
take-rate. The wave-32 revision -- mine -- ran the HL1 card-fact audit, recorded that sentence
**TRUE**, and then DELETED it under the three-part demotion of validated teaches, keeping only the
membership CONDITION (*"when a non-Human creature is on your battlefield, take the mutate line"*).
The wave-33 corpus: **5 replies asserting a deck creature is Human, mutate take-rate 1/6**, the
deck's #1 rule off in five of six games, and it is the largest single decision defect behind a
0/6.

**The mechanism, stated so it generalises.** Wave-32 HL3 rung 3 introduced the SUPERSEDED verdict:
*a teach whose CLASS is dead and whose work the RENDER now does -- delete it in the same revision
that records it, and quote the superseding render string.* That rung is correct and its safeguard
is the quote requirement. What went wrong here is that the same reviewer applied the SPIRIT of the
rung (this teach's class shows zero, the teach is proven, compress it) **without the quote**,
because there is no render string to quote: `subtype=` is a declared primitive field that the
corpus renders **zero** times in 190 records. The teach was not superseded. It was the ONLY thing
carrying the fact, and its zero-count was the measurement of its own success.

The two cases are indistinguishable from the counted audit alone -- both read "class dead, 0
occurrences" -- and they have opposite correct actions.

**THE RULE (an addition to HL3 rung 3's verdict column, and a hard gate on any deletion):**
> Before deleting or compressing ANY guide sentence, name the SURFACE that will carry its fact
> once the sentence is gone, and paste the render string. Exactly three answers are admissible:
> (a) **a render string, quoted** -> SUPERSEDED, delete (HL3 as written);
> (b) **another sentence in this same guide, quoted** -> compress into it, and re-read the survivor
>     to confirm it states the FACT and not merely the CONDITION;
> (c) **nothing** -> **the sentence is LOAD-BEARING and stays, whatever its occurrence count.**
> A zero occurrence count is not an answer to this question; it is the QUESTION.
> **Cheap mechanical tell for (c), and it is the one that fires here: the guide asks the pilot to
> evaluate a card property (creature type, subtype, colour identity, legendary-ness, whether a
> permanent is a token) and a grep of the corpus prompts for that property returns ZERO hits.**
> Run that grep on every property any surviving guide CONDITION names, at revision time.

**And the composition defect underneath it, which is the cheaper thing to teach:** the wave-32
revision kept a CONDITION (*"when a non-Human creature is on your battlefield"*) whose FACT it had
just deleted. A condition the pilot cannot evaluate is worse than no rule -- it does not fail
silently, it invites the pilot to evaluate it and it will invent an answer. **Whenever a guide
rule is phrased "when <property> then <action>", the guide owes, in the same paragraph, either the
property's VALUE as a flat fact or the render string that prints it.** Pairs with HL1 rung 4's
"do not ship a more careful counting procedure, ship a decision rule that does not DEPEND on the
count" -- the same move one level up: this guide now ships
*"the engine prints 'with its mutate cost' ONLY when a legal host already exists; if the line is
there, take it"*, which removes the property evaluation from the pilot entirely.

---

## P2 (PROMOTION-GRADE) -- HL6's COUNT ESCALATION WORKS, AND IT IS SCOPED TO THE CLASS IT COUNTS. THE BELIEF MIGRATES TO THE NEAREST CLASS WITH NO AGGREGATE. A COUNT RETIRES ONE AXIS, NOT THE MECHANISM.

Wave-32 HL6 said: when a false belief about a card CLASS survives both a guide sentence and a
per-item render tag, escalate to an ENGINE-COMPUTED COUNT, and gave the diagnostic *"the reply
REPRODUCES your list and then EXCLUDES a member of it."* It was written from this seat and it was
**right**. Measured here:

| axis | guide sentence | per-item tag | engine COUNT | misreads |
|---|---|---|---|---|
| green mana sources | present | `(land: taps for {R}{G})` | **`{G} 3` (i9, NEW)** | **0** (was 6) |
| non-Human creature type | **deleted w32** | **absent** (`subtype=` renders 0x) | absent | **5** |
| summoning sickness on an OFFERED attacker | absent | absent on the A-line | absent | **1** |

Nine mulligans became zero. And in the same corpus the identical mechanism -- recite the class,
exclude a member, invent a disqualifier -- produced five refusals of an option the ENGINE had
already ruled legal, plus one refusal of an attacker the engine had already listed as able to
attack. **The pilot did not stop denying class membership. It stopped denying it about lands.**

**THE RULE (extends HL6, and it is what makes the escalation ladder honest):**
> A COUNT retires ONE AXIS. It does not retire the membership-denial MECHANISM, and the mechanism
> re-lands on whichever class the current decision turns on that has no aggregate. So when you
> escalate a class to a count, **enumerate the OTHER card properties that any guide rule or any
> engine legality check makes decision-relevant in this deck, and check each for an aggregate.**
> Concretely, at revision time: (1) list every property a guide CONDITION names; (2) list every
> `otherrestriction=` / legality predicate on the deck's key cards -- those are exactly the
> properties the engine will silently decide and the pilot will loudly re-litigate; (3) for each,
> record surface / tag / count. A property with none of the three is the next migration target,
> and it is cheaper to write a falsity-fact for it now than to find it in the next corpus's loss
> column.
> **Ranking, from this wave's evidence:** an ENGINE-COMPUTED AGGREGATE beats a per-item tag beats
> a guide sentence -- but a fourth form beat all of them here and costs nothing:
> **make the ENGINE'S OWN OFFER the evidence.** *"The engine only prints this line when the
> condition holds"* removes the property from the pilot's reasoning entirely, needs no render
> change, and is available wherever the fix is gated by an `otherrestriction`.
> Adopt in this order: (a) offer-as-evidence, (b) engine-computed count, (c) per-item tag,
> (d) guide assertion.

**Corollary for the wave-32 corollary (the exemplar lever):** wave-32 recorded the exemplar lever
as SPENT for this seat and routed upward to a count. That was correct and the count **paid**. The
count is now also spent on that axis -- there is nothing left to escalate for green sources, which
is what "closed" should mean. **Record a closed axis as CLOSED and stop looking at it**; this
seat's whole wave-33 review would have missed the Human class if it had spent its budget
re-verifying the mulligan.

---

## P3 -- A SYMMETRIC PERMISSION IN A PROMPT SENTENCE IS AN INSTRUCTION IN BOTH DIRECTIONS. WHEN A RULE'S PROHIBITION AND ITS PERMISSION SHARE A SENTENCE, THE GUIDE OWES THE PROHIBITION ITS OWN LINE WITH THE FORBIDDEN OUTPUT NAMED.

The blocker prompt says, in one sentence:
> *"Assign each blocker to AT MOST ONE attacker (a creature cannot block two attackers), but
> several DIFFERENT blockers may gang-block the same attacker."*

vs105 s24 answered `BLOCKS: Illuna blocks Cystbearer, Illuna blocks Hand of the Praetors, Illuna
blocks Ichorclaw Myr #1, Illuna blocks Ichorclaw Myr #2` and argued *"she can legally block all
four"*. The parser kept the first, three infect attackers connected, poison hit exactly 10, the
game ended at 14 life.

The pilot mirrored the sentence's own symmetry: *many blockers -> one attacker* generalised to
*one blocker -> many attackers*. This is not a missing fact (the prohibition is present, in
parentheses, in the same sentence) and it is not a false surface. It is the executor pattern-
matching on the vivid half of a two-sided sentence.

**THE RULE (Step 5, wording, and it pairs with wave-32 HL6's closing clause on forbidding a false
SENTENCE by name):** where a rendered sentence carries a PERMISSION and a PROHIBITION together,
the guide restates the PROHIBITION alone, on its own line, with the forbidden OUTPUT SHAPE named
in the pilot's own syntax -- *"never write the same creature's name twice in a BLOCKS line; only
the first assignment happens and every other attacker you named hits you in full"* -- plus the
positive substitute for the situation that generates it (*"one blocker and four attackers means
you are choosing WHICH ONE to stop"*). Naming the syntax, not the rule, is the load-bearing part:
the executor's error is in the line it writes, not in its understanding.

**Also, cheap and general:** the drop was SILENT. `unparsed_reply` did not fire (one assignment
survived) and `all_assignments_illegal` did not fire (not all were illegal). A partial drop is
invisible to both. -> instrument item in notes.md (`dropped_assignments`), not a skill rung.

---

## P4 -- ADD A SELF-INFLICTED-LIFE ROW TO THE SEAT'S STANDARD MEASUREMENTS, AND GIVE ANY REPEATED PAY-OR-DECLINE SEAM A DEFAULT PLUS A NAMED TRIGGER.

deck139 answered `pay 2 life` at **20 of 20** shockland windows across six games = **40 life**, and
19/20 the wave before. In the game it lost by **3**, it had paid **8**. In the game it lost to
poison at 14 life, the **only** life it lost all game was the 6 it paid its own lands. **Neither
wave's review measured it**, because every existing rung looks at casts, attacks, blocks, mulligans
and fallbacks -- and this seam is none of those. It is a repeated, cheap-looking, individually
defensible decision that only shows up in aggregate.

Two rungs:
1. **MEASUREMENT (HL10, one regex, every seat that runs pain lands, shocklands, fetches, Phyrexian
   costs, painful fixing, or any "pay N to do X now" prompt): report `payments taken / windows`
   and `total life spent`, beside the cast-nothing split.** Cross-tab against each loss's final
   life. The tell that it needs a rule: `total life spent` is a material fraction of the life the
   seat actually lost. Cost: one grep on `chosen_text`.
2. **AUTHORING: a repeated pay-or-decline seam gets a DEFAULT plus a NAMED TRIGGER for the
   exception, never a cost/benefit description.** The wave-32 guide described the choice
   accurately (*"the 2 life is an enters-the-battlefield choice about tapped-or-untapped"*) and
   left the decision open; the pilot took the tempo every time, because every individual window
   makes tempo look free. The shipped form is *"ANSWER 'tap' unless the untapped land pays for
   something you will cast THIS turn"* + a life-bracket override. This is HL8 rung 1's closed
   exception list applied to a resource seam rather than a combat seam.

Note the interaction worth flagging to synthesis: the wave-32 guide's shockland falsity-fact was
written to cure the MULLIGAN misread. The count header cured that misread, and the clause's only
remaining live effect was to normalise the payment. **A stopgap whose original class dies can keep
teaching in a second seam** -- HL3 rung 2's "what does the residual tax rehearse NOW?" applied to
a retired stopgap's SIDE EFFECT rather than to its target belief.

---

## P5 -- CORRECTION TO MY OWN WAVE-32 RUNG: A GREEN TRIGGER-PREDICATE COUNT SHOULD BE REPORTED WITH ITS DENOMINATOR'S CAUSE, BECAUSE A FIRING COUNT CAN BE SUPPRESSED BY A GUIDE DEFECT.

Wave-32 HL10 (deck139 P4) added: *"when the corpus binary is NEWER and the trigger predicate fires
N>0 times with 0 failures, the verdict is 'fixture and corpus disagree -- re-run the fixture'."*
That rung worked and N-139m was correctly re-scoped to the harness lane.

This wave the same predicate fired **once** (down from six), and passed. Reported bare, "1/1 pass"
looks like weakening evidence for the harness scoping. It is not: the predicate fired once because
the pilot **declined five of six mutate windows on a false belief**, so the engine never got the
chance. **The denominator moved for a reason that has nothing to do with the defect under test.**

**THE RULE (one clause on the trigger-predicate rung):** report the predicate's firing count with
the count of OPPORTUNITIES that existed for it -- offers, windows, or legal chances -- and, when
they differ, name what consumed the gap. *"1 firing / 6 opportunities, 5 consumed by a guide-layer
decision defect"* is a usable number; *"1 firing, 1 pass"* invites a reviewer to read a shrinking
denominator as fading risk. This is the arrival-tracing doctrine's "a counter that only records
failures cannot see a non-event" applied to the counter's own denominator.

---

## Not proposed (recorded so synthesis can see they were considered)

- **A rung about the 0/6 and pool drift.** The pool moved (158: 3/6 -> 6/6; 146: 2/6 -> 4/6, and
  those are exactly the two seats deck139 beat in wave 32). I considered proposing a
  "beat-only-the-bottom" fragility instrument. **Not proposed:** the existing rungs already require
  per-game attribution and this seat's decomposition worked without a new instrument. Recorded for
  synthesis's cross-seat read, since deck105's -4 and deck158's +3 are the same corpus event.
- **Anything about reply length or the reasoning block.** Owner-ruled; routed to
  general-suggestions.md as evidence only, per the directive.
