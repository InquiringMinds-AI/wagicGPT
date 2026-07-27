# deck122 -- wave 31 core (general prompt) verdict

**PASS (26th consecutive).**

The general prompt handled a lifegain engine, conditional-P/T renders (Serra `(6/6)
(printed 1/1)`, Divinity `(8/8)`), London mulligan + bottoming, may-triggers with a
Life / Life Loss modal, multi-attacker block enumerations, and gang-block syntax without
a single core-layer failure across 176 decisions at this seat. Answer-first compliance was
100%: even the two truncated replies opened with a complete, legal `BLOCKS:` line.

## One item considered and DELIBERATELY NOT routed here

The vs158 seq30 misplay -- attacking with all three blockers into a tapped-out board and
then having nothing untapped for the 21-damage crackback -- would superficially motivate a
core-prompt line at the attack seam ("a creature that attacks is tapped and cannot block
until it untaps, unless it has vigilance").

Rejected, because the evidence says the model already knows it: three decisions later, at
seq37 of the same game, it writes unprompted "attacking with my small creatures now leaves
me with fewer blockers next turn." A general-prompt restatement of a fact the executor
demonstrably holds buys nothing and taxes every decision in the corpus. The failure is
application under a locally-persuasive "they cannot block me" frame, which is a
guide/posture-layer problem -> routed to skill.md Proposal 1.

## Items routed elsewhere

- Attacker tapped-tag self-contradiction (`[tapped - cannot attack or block this turn]
  [attacking]`, 331 corpus occurrences, 4 seats verbalized it, cost deck122 the vs158
  game) -> **notes.md N-122c** (render/engine layer, not core prompt text).
- Truncation discarding a valid answer-first `BLOCKS:` line and defaulting to no-blocks
  -> **notes.md N-122d** (harness parse/fallback policy).
- Missing casting-decision asks with payable spells in hand -> **notes.md N-122b**.
