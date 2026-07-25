# deck131 wave-26 -- skill.md proposal

One reviewer-method refinement for the strategy-writing skill, EXTENDING (not replacing) the wave-25
"CORRECT DEVIATION FLAGS A GUIDE DEFECT" rung that this seat introduced.

## PROPOSAL -- REPEATED CORRECT-DEVIATION ON THE SAME RULE = a RECALIBRATION SERIES; edit the WHOLE corner, not one point at a time

Context: the wave-25 deck131 rung ("when the model's play is right but breaks a guide rule, audit the RULE,
don't score the play") fired again this corpus -- a THIRD time on the SAME too-absolute rule (the
"NEVER block with Guttersnipe" line). The recalibration history of that one line:
- wave-24: added the LETHAL-and-only-blocker exception (chump to survive).
- wave-26 (this corpus): added the FAVORABLE-block exception (Guttersnipe survives a power<2 attacker).

Two waves, two separate edits, SAME absolute -- each time the model surfaced a NEW corner the absolute got
wrong, and each edit patched only the corner that happened to be exercised that corpus. This is a signal the
prior rung under-specifies: when a guide absolute is found miscalibrated, the reviewer should not patch ONLY
the observed corner but ENUMERATE the rule's full decision space and state the general principle, so the
next corner does not require a whole new wave to discover.

**The refinement (a method discipline, not a new mechanical diff):**

**When a CORRECT DEVIATION reveals a miscalibrated absolute, do not merely add the one exception you
observed. Restate the rule around its GOVERNING PRINCIPLE and enumerate the decision axis, so every corner is
covered at once. For a "never X" rule, ask: what is the ACTUAL cost the rule is guarding against, and across
the full range of the relevant variable, WHERE does that cost apply? Patch the whole range.**

Worked example (this seat, the Guttersnipe-block rule): the real cost being guarded is "losing your only
reach in a TRADE." The relevant variable is (attacker power vs Guttersnipe toughness) x (am I dying?). The
full space:
- attacker power < toughness -> Guttersnipe SURVIVES -> not a trade -> ALLOW (favorable block).
- attacker power >= toughness, NOT lethal -> a TRADE you don't need -> FORBID.
- attacker power >= toughness, LETHAL and only blocker -> a trade that beats dying -> ALLOW (chump).
Stating that 3-way space once would have shipped BOTH exceptions in wave-24; instead each arrived a wave
apart as its corner happened to appear. The lesson: the governing principle ("does Guttersnipe survive / am
I dying?") generates the full rule; the observed instance is just the corner that made you look.

Why this matters for verdicts and churn: a seat that produces "one more exception to the same absolute" every
wave LOOKS like live new-work, but it is really the SAME finding (a miscalibrated absolute) discovered
piecemeal. Enumerating the full corner at first contact (a) ships the correct rule in one edit, (b) prevents
a false "new decision class every wave" reading that defers a rotation verdict indefinitely, and (c) makes
the eventual rotation call honest -- once the rule's full space is stated, the corner-supply is EXHAUSTED and
the seat's guide-work is genuinely done (deck131 is exactly here after this wave).

Boundary (kept intact): this does NOT license speculative exceptions for corners the model has not exercised
-- state the general PRINCIPLE and the enumerated axis (which is rules-derivable), but ground each concrete
clause in Oracle/engine rules, and do not invent play the model never made. The discipline is "cover the
whole rules-space of the corner you were shown," not "imagine new corners." It remains separated from the
sanctioned-against "shout louder" by the same tell as the wave-25 rung: a CORRECT model deviation means the
line is wrong (fix its whole space); a WRONG model deviation from a right line means the model slipped
(WATCH, never louder).

Promotion: this corpus gives the confirming SECOND data point for the wave-25 rung (the rung fired again,
cleanly) PLUS the piecemeal-recalibration observation. I propose it to the synthesis agent as an EXTENSION
clause on the existing wave-25 rung, not a standalone rung -- flag for convergence against any other
reviewer this wave who patched a too-absolute rule and left an adjacent corner unstated.
