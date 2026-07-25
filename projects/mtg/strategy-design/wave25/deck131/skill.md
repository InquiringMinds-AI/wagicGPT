# deck131 wave-25 — skill.md proposal

One reviewer-method proposal for the strategy-writing skill. The wave-23 (claimed-vs-actual-tap diff) and
wave-24 (rung-3: answer-line-vs-conclusion diff) rungs already cover the WRONG-play failure modes. This
corpus surfaced a distinct discipline: a CORRECT play that VIOLATES a guide line, which flags the GUIDE as
the defect — the inverse of everything the prior rungs catch.

## PROPOSAL — "CORRECT DEVIATION FLAGS A GUIDE DEFECT": when the model's play is right but breaks a guide rule, audit the RULE, don't score the play

Method-headline candidate (deck131 vs137 s3 + vs133 s3 + vs59 s27 THIS corpus — 3 instances, 2 distinct
guide lines, all GAME-legal correct plays): the standing reviewer reflex is to score a decision that
DEVIATES from the deployed guide as either a pilot error (model didn't follow the guide) or, if the guide
already covers it, to reach for "shout louder" (the sanctioned-against move). This corpus produced a THIRD
case the existing rungs don't name: the model's play was Magic-CORRECT and the guide line it violated was
Magic-WRONG (a too-absolute rule with an unhandled corner). The correct move is neither "score a pilot
error" nor "shout louder" — it is to CORRECT THE GUIDE RULE (soften the absolute / add the missing
exception), because a guide that is obeyed literally in a DIFFERENT instance of that corner would force a
WORSE play.

Two concrete instances this corpus, same shape:
1. **Bottom line, "never bottom a draw spell":** the model correctly bottomed an over-costed draw
   (Opportunity, 6 mana, cheaper dig in hand) and a duplicate draw (2nd Artificer's Epiphany). The absolute
   is Magic-wrong for loaded/duplicate hands. Fix = impact/redundancy ranking; only "never bottom your ONLY
   draw/payoff/red-source."
2. **Never-block line, "never Guttersnipe, even to die":** the model correctly blocked-to-survive with a
   lone Guttersnipe facing lethal (no other blocker). The absolute assumes a chump exists. Fix = add the
   lethal-AND-only-blocker exception.

So the decision-scoring procedure gains a discipline (not a 4th mechanical diff, but a routing rule):

**When a decision DEVIATES from the deployed guide, FIRST judge the play against Oracle/engine rules on its
own merits. If the DEVIATION is CORRECT, the guide line is the suspect — audit it for a miscalibrated
ABSOLUTE (a "never/always" with an unhandled corner) and CORRECT the rule (soften / add the exception),
grounded in the guide's existing valuations. Do NOT score a pilot error, and do NOT confuse this with
"shout louder" — louder re-emphasizes a line that is RIGHT but ignored; this REPLACES a line that is WRONG.
The tell that distinguishes them: a correct model deviation means the line is wrong (fix it); a wrong model
deviation from a right line means the model slipped (WATCH / harness, never louder).**

Why this is a distinct rung and why it matters for verdicts: it prevents two opposite mis-routes. Mis-route
A — scoring the model's correct play as a pilot error (deflates the pilot, hides a real guide bug).
Mis-route B — invoking "shout louder" on the wrong line (the sanctioned-against churn) when the line should
instead be REPLACED. Both were live temptations this corpus: the two bottom decisions "looked like" the
model ignoring the guide, and the Guttersnipe block "looked like" the wave-24 non-lethal-block slip — but
all three were CORRECT plays revealing WRONG absolutes.

Boundary against the sanctioned-against rule (kept intact): "shout louder" remains forbidden for a line
that is CORRECT but stochastically ignored (deck131's tapped-read guard is exactly that — it held 8/9, the
prose is right, the miss is model comprehension -> WATCH, guide untouched). The new rung fires ONLY when the
deviation is CORRECT and the violated line is WRONG. The two are separated by the same claimed-vs-actual
check the prior rungs use: is the deviation game-legal and rules-correct? If yes -> fix the guide rule; if
no -> the model slipped, leave the (correct) guide alone.

Promotion threshold: this corpus alone gives 3 instances across 2 guide lines at deck131. It generalizes
trivially (any guide with an absolute has this failure mode), so I propose it as a STANDING routing
discipline, not a single-seat curiosity — but flag it for the synthesis agent to confirm against other
reviewers' corrections this wave (convergence = strongest merge signal).
