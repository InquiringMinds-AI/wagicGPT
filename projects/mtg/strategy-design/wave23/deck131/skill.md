# deck131 wave-23 — skill.md proposal

One reviewer-method proposal for the wave-23 strategy-writing skill. Everything else at this seat is
covered by existing skill doctrine (rotation new-leak hunt, demotion-debt audit, crutch lifecycle,
answer-first scoring).

## PROPOSAL — SCORE A "SAFE SWING" RATIONALE AGAINST THE ACTUAL TAP TAGS, NOT THE MODEL'S CLAIM

Method-headline candidate (single-seat now; deck131 vs27, GAME-DECIDING): when a pilot declares an
attacker with a rationale of the form "it is safe, the opponent has no untapped blockers," the reviewer
must DIFF the model's claimed tapped-set against the prompt's literal `[tapped]` tags on the opponent
battlefield line — NOT accept the rationale, and NOT infer safety from the fact that the swing was
declared. deck131 vs27 s15: the model wrote *"Vodalian Zombie and Lord of the Undead are both tapped"*
and swung its only Guttersnipe; the prompt showed the [tapped] tag on Vodalian ONLY — Lord of the
Undead was untapped, blocked, and traded away the deck's only reach. A `chosen_text` glance ("Guttersnipe
attacks") and a swing-was-declared check both MISS this; only the claimed-vs-actual tap diff catches it.

Why this is a distinct rung from wave-22 headline 4 (declared-set vs recorded-set for attackers): that
rung catches UNDER-recording (a valid coded line dropped to fewer attackers). This catches a different
failure — the DECLARED set is faithfully recorded, but the DECISION rested on a false board read. The
attacker was correctly parsed and executed; the harm is upstream, in the model's comprehension of the
representation. So the attacker-scoring procedure needs TWO diffs now: (1) declared-set == recorded-set
(execution fidelity), and (2) claimed-board-state == actual-board-state (decision-input fidelity) for
any "this swing is safe because X" rationale.

Guide-lane corollary (already applied at deck131): when a deck has a single irreplaceable
combat-relevant permanent (a lone reach source, a lone finisher), the guide's protection for it must
cover BOTH sides — never-block AND never-attack-into-a-possible-trade — and, because the trigger is a
tap-tag misread, the attack-side guard should instruct a PER-CREATURE [tapped]-tag read ("a creature
printed without [tapped] can block, even if another creature on the same line is tapped"). deck131's
NEVER-block-Guttersnipe rule had no offensive twin; that gap cost a game the moment a partly-tapped
board appeared. General rung: audit every "protect the keystone permanent" guide rule for one-sided
coverage.

Promotion threshold: a 2nd seat showing a tapped-state per-creature misread promotes the reviewer-method
diff to a standing attacker-scoring step and the tap-tag-read to a cross-seat combat-safety guide
pattern. Single-seat now — proposing the reviewer diff (cheap, catches a class chosen_text hides), not
a cross-seat guide mandate.
