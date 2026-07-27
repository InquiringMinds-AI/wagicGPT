# deck139 -> strategy-writing skill proposals (wave 32)

Four proposals. P1 is promotion-grade and cross-seat by construction; P2 and P3 are single-seat
but each earned by a witness this wave; P4 is an instrument, measurement-only.

---

## P1 (PROMOTION-GRADE, 2-seat by construction) -- A FALSE BELIEF THAT SURVIVES BOTH THE GUIDE AND THE RENDER IS A **COUNT** PROBLEM, AND THE THIRD LAYER IS A COUNT, NOT A THIRD SENTENCE.

The skill already routes a misread state to the render (perception-vs-strategy) and forbids a
third copy of an instruction (representation-beats-instruction). It does not yet say what to do
when the render ALREADY carries the fact and the pilot reasons past it anyway. Wave 32 produced a
clean two-cycle instance:

- **Cycle 1 (wave 31).** Belief: "this hand has zero lands" with three dual lands in it. Fix
  applied at the GUIDE layer per wave-31 HL10 (re-aim the exemplar): the guide was rewritten to
  enumerate the five green lands by printed name and to quote the render substring
  (`A card printed "(land: taps for {R}{G})" IS a land AND IS a green source; read the braces`).
- **Cycle 2 (wave 32).** The belief did not die; it MUTATED to "this hand has zero GREEN mana
  sources", and the pilot now RECITES THE GUIDE'S OWN LIST while holding a member of it
  (deck139 vs105 s1: *"3 lands but zero green mana sources (Forest, Stomping Ground, Temple
  Garden, Overgrown Tomb, Breeding Pool)"* -- with a literal **Forest** in the rendered hand). Six
  false mulligans out of nine, each against a hand line that printed `(land: taps for {R}{G})`.

The generalisation: **a categorical belief about a card CLASS ("these lands do not really make
X") is not reachable by naming the class again, in either layer. Both the guide sentence and the
per-card tag are the SAME move -- they assert membership. The pilot's counter-move is to invent a
disqualifier ("Stomping Ground is red/green but you have no way to pay the life"). What it cannot
argue with is an AGGREGATE the engine computed: a COUNT.** This is exactly the shape that worked
for the i12/N-158g mana line (`Mana available: 5 total (...)`), and the shape the i11 creature
markers did NOT have -- and correspondingly, i11's own axis (creatures) produced ZERO misreads at
this seat this wave while the land/colour axis produced six.

THE RULE, for the Step-0 ladder and the fix-layer checklist:
> When a false belief about a card CLASS survives (a) an explicit guide sentence naming the class
> and (b) a per-item render tag asserting membership, do NOT write a third assertion and do NOT
> re-aim the exemplar again. Escalate to an **engine-computed COUNT over the class**, printed as a
> header above the items (`Your hand (7 cards): lands 3 (green sources: 1) | creatures 4`). A
> per-item tag answers "is this one an X"; only a count answers "how many X do I have", which is
> the question the decision actually turns on -- and a count is the one form the pilot cannot
> reason its way around, because there is nothing per-card left to disqualify.
> Diagnostic that you are in this case, not in ordinary disobedience: **the reply REPRODUCES your
> list and then excludes a member of it.** That is class-membership denial, not a missing fact.

Corollary for the exemplar rung (wave-31 HL10): re-aiming an exemplar is a ONE-SHOT remedy. If the
re-aimed exemplar fires and the belief survives in a NEW phrasing, the exemplar lever is spent;
record it as spent and route upward, rather than re-aiming a third time.

Cross-seat by construction: any seat whose keep/hold decision turns on "how many of my cards are
of type X" hits it -- deck146 ran 19 mulligans in this same corpus, and every land-count deck in
the roster is a candidate witness.

---

## P2 (single-seat) -- A NUMERIC FLOOR MUST BE KEYED TO A NUMBER THE PROMPT PRINTS, NOT TO A COUNT THE PILOT MAINTAINS. AN INTERNAL COUNTER IS DEFERRED, NOT REJECTED.

Wave-31 HL10's corollary said a mulligan teach needs a NUMERIC FLOOR, and gave the wording *"at
most two mulligans, then KEEP the best hand you have"*. That floor was written into deck139's
guide and it was DISOBEYED once -- and the failure mode is instructive, because the pilot never
argued with it. It DEFERRED it, at every step, by one:

- vs36 s3 (prompt: *"having already taken 2 mulligans"*): *"Since I have already taken 2
  mulligans, I must take this one ... **and I will keep whatever I draw next regardless of
  quality**."*
- vs36 s5 (prompt: *"having already taken 4 mulligans"*): *"**With four mulligans already taken, I
  must keep the next hand**, but I cannot keep this one."*
- Kept at 5 mulligans, 2 cards, dead on turn 7.

The floor as written is a rule about a COUNT THE PILOT HOLDS ("you may take at most TWO"), and a
weak executor converts a held count into a promise about the next window -- the stale-plan /
deferred-plan loop, at a seam that gets exactly one chance and cannot self-correct. Meanwhile the
prompt was printing the authoritative count in front of it the whole time.

THE RULE:
> A numeric floor or cap on a repeatable decision is written as a TEST OVER A STRING THE PROMPT
> PRINTS, never as a budget the pilot tracks: *"the prompt says 'having already taken N
> mulligans'. If that sentence appears at all, this is your LAST look: answer Keep."* Same
> discipline as the menu rung's value-anchored-not-position-anchored pick -- anchor the terminator
> to something the executor READS, not to something it must REMEMBER.
> And add the anti-pattern by name, since the executor generates it verbatim: *"'I will keep
> whatever I draw next' is how you end up with a 2-card hand -- keep THIS one instead."*

This also supplies the missing half of "a pursuit needs a countable completion condition": the
condition must be countable **from the prompt**, not merely countable.

---

## P3 (single-seat, but the second instance of the wave-31 HL3 shape at a NEW seam) -- A VALUE TEACH EVAPORATES AT THE **DEPLOY** SEAM, NOT ONLY AT COMBAT; AND "WHICH BODY" IS A SEPARATE RULE FROM "A BODY".

Wave-31 HL3 located the role/value evaporation at the COMBAT seam (deck122's attack, deck148's
block). deck139 supplies the same shape at the CAST seam, and it decided a game.

vs152 s20, T10: `Your life: 8 | Opponent life: 26`, `Your battlefield (creatures: 0)`,
`Opponent battlefield (creatures: 5)`, `Mana available: 4 total`. Offered
`1. Cast Pollywog Symbiote {1}{u} (1/3)` / `2. Cast Gemrazer {3}{g} (4/4)` / `3. Cast nothing`.
It cast the **1/3 engine**: *"I need to stabilize and set up my engine ... it reduces the cost of
my future mutate spells by {1}, making my big Apex bombs much more affordable."* Next decision it
had one blocker, a 1/3 with no reach, against a 3/1 flier; the reply named an illegal block, went
unparsed, and it died at -3 the following turn. Gemrazer (4/4, **reach**) was exactly payable.

The guide had a strong CAST FLOOR ("if a Cast line is offered, TAKE ONE") and it was OBEYED. The
guide had a #2 rule ("under aggro, a fresh body beats a mutate") and it did not apply, because
both offers were fresh bodies. The guide raised Pollywog's value in three separate places and gave
the deploy seam no tiebreak. The floor answers "cast something"; nothing answered "cast WHICH".

THE RULE:
> A CAST/DEPLOY FLOOR is not finished until it also ranks the OFFERS. Wherever a guide names an
> ENGINE permanent (a discounter, a lord, a value creature) it must state, in the deploy block,
> the board condition under which the engine LOSES to a plain body, keyed to countables the pilot
> reads: *"when your life is <= N and the opponent shows more creatures than you, cast the offered
> creature with the HIGHEST TOUGHNESS -- not the engine, not the cheapest."* Add the evasion
> qualifier when the deck's blockers differ in keywords (*"prefer [reach]/[flying] if any opposing
> attacker shows [flying]"*), and give the rule an OFF-RAMP so it does not invert (*"above N life,
> or even/ahead on creatures, cast the engine first -- that is when it is right"*).
> Generalised: **HL3 says a ROLE owes a mechanical cost at the seam where the role is cheapest to
> abandon. Extend it: a VALUE teach about a specific permanent owes a DEPLOY-ORDER rule, because
> the seam where "this card is my engine" is cheapest to over-apply is the seam where the engine
> is one of the offers.**

---

## P4 (instrument, measurement-only) -- WHEN A CORPUS BINARY IS NEWER THAN THE PROBE BINARY, THE TRIGGER-PREDICATE COUNT CAN COME BACK **GREEN**, AND THAT IS A RESULT, NOT A NULL. RECORD THE BINARY ON BOTH SIDES.

Wave-31 HL4.2 established the trigger-predicate discipline for a crash/abort/silent-no-op fix:
write the trigger condition as a board state, count it in the corpus, and if the count is 0 the
verdict is UNEXERCISED and the deliverable is a probe spec. deck139 ran that discipline this wave
and hit the case the rung does not cover: **the count was 6 and all six PASSED**, against a
fixture-proven defect (N-139m) that the probe had reproduced 3/3.

The resolution is a version fact the reviewer nearly missed: the probe ran on `6ca1f8411`; the
corpus ran on `7fabd9bd0`, which lands a `doPay`/`isPaymentSet` **stale payment target** fix from
the convoke arc -- and N-139m's mechanism ("pays, fires its cast trigger, then leaves the stack
into no zone") is in that family. The correct next action is therefore NOT the scheduled fix; it
is a **fixture RE-RUN on the corpus binary**, which is minutes of work and can retire the docket
item outright.

THE RULE (append to the arrival-tracing / trigger-predicate rung):
> Record the BINARY HASH on both sides of every open-defect claim -- the one the fixture failed on
> and the one the corpus ran on -- and check their order before treating a KNOWN-OPEN item as
> live. When the corpus binary is NEWER and the trigger predicate fires N>0 times with 0 failures,
> the verdict is not "the defect is live and we got lucky" and not "the defect is fixed": it is
> **"fixture and corpus disagree; RE-RUN THE FIXTURE ON THE CORPUS BINARY FIRST."** A green
> trigger-predicate count on a newer binary is EVIDENCE, and it changes the shape of the docket
> item either way -- if the fixture goes green the item retires to a regression test; if it stays
> red on a binary the live path passes, the defect is localised to the fixture harness path rather
> than the production path, which is a different and cheaper fix.
> Corollary, mirroring the rung's own asymmetry argument: the reason this matters is the same
> reason the rung exists -- a symptom-shaped read (here "the probe says it is broken") is not a
> validation of the CURRENT tree.
