# deck123 seat - core-prompt / render proposals (wave 47)

Layer routing: engine BUGS and missing affordances are in `seats/seat-123-130.md` under
ENGINE H1/H2/M1/M2/M3/L1/L2/L3 and are NOT repeated here. This file is only for changes to the
prompt's own text and shape.

---

## G47-1 - A repeated action deserves a repeated-action reply contract

**Observation.** The reply-format sentence is constant across every priority window:
`Which action do you take? On the FIRST line write CHOICE: followed by the number (0 = pass
priority) ... then a PLAN: line only if the reply rules call for one.` On the 1,894 windows of
the dead-looped game, that sentence was the only place the exit existed, and the pilot re-wrote
a full PLAN line on a large fraction of them - at seq 600, 1000, 1500, 1900 the plan is a fresh
paraphrase of the same intention.

**Proposal (prompt text, not engine behaviour).** On a priority window whose option set is
IDENTICAL to the previous window's at the same seat and phase, replace the generic reply sentence
with one that names the two live choices in the pilot's own terms:
`You have taken this same action N times already this turn and the screen has not otherwise
changed. Continue (CHOICE: 1) or stop and pass (CHOICE: 0)?`
This costs ~30 characters, adds no information the engine does not already have (the repeat
counter exists), constrains no choice, and puts the stop in the sentence the pilot is answering
rather than in the format boilerplate.
It is a prompt-text change; the numbered `0. Pass priority` ROW I asked for in ENGINE H1 is the
engine-side version and the two are independent - either would help, both would be better.

---

## G47-2 - Suppress the PLAN request on a decision that cannot change the plan

**Observation.** Wave 46 shipped exactly this for two window classes and both worked: pregame
mulligan asks now carry no `YOUR PLAN` block (0 of 16 at this seat pair, chains 9 -> 0), and land
drops now end `this decision needs no PLAN line` (0 of 19 declines, max latency 453 s -> 18 s).
The generalisation is doing real work and is not stated anywhere as a principle.

**Proposal.** Make it a rule of the prompt builder rather than two special cases: a decision
whose outcome cannot invalidate the standing plan should neither request a PLAN line nor re-show
one. The two shipped cases are the proof; the third candidate visible from this seat is the
ANNOUNCE_X screen (ENGINE M1), where the re-shown plan already contains an X chosen before the
kill lists existed and was answered verbatim.

---

## G47-3 - The blockers screen should say what it costs to re-derive

**Observation.** `...deck123-...-vs-deck152` seq 27: a 5-attacker blockers screen produced a
~1,800-word reply at **522 s** in which the pilot re-computed every 1-on-1 trade the rows already
printed, and then built its answer on a spell it could not pay for. The screen already carries
*"Trust it over your own arithmetic; never re-derive it."* The instruction is present and was not
followed on the one window where the header said LETHAL.

**Proposal.** On LETHAL-flagged blockers windows specifically, put the affordability fact next to
the priority promise, since the promise is what invited the essay:
`You keep priority through the rest of this combat: instants and activated abilities you hold
stay castable after blockers are declared - of the cards in your hand, N are castable with your
current Mana available: <list or "none">.`
The engine already computes castability for every `ask` window; this is that same computation
surfaced on the screen where the pilot plans a post-block instant. It hides nothing and forbids
nothing - it removes a whole branch of reasoning that was fiction.

---

## Reported but explicitly NOT proposed
- Anything about the LOOP's stopping behaviour beyond G47-1: that is ENGINE H1 and belongs to
  the lane, and my guide now carries the pilot-side stop, so wave 48 can separate the halves.
- Anything about attacker-row collapse: ENGINE H2.
