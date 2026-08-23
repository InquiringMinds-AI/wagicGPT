# deck152 — Wave-37 guide revision (three edits from validation-152-105.md)

**Mode**: turnaround-preservation — everything validated kept byte-verbatim (plan-memo
paragraph, FA-sequencing/removal text, deploy floor, mana trust, day/night, Flip-Side land
recipe, mulligan floor: all untouched; diff confirms only the three edit sites changed).
Base = deployed `bin/Res/ai/baka/deck152_strategy.txt`; output here, NOT deployed.

## Edit 1 — trade-drift: restate-then-override named as the failure shape

Rule 4 gains one bullet after the "template error" falsity-fact: "THE DRIFT TO CATCH:
restating the parenthesis, then overriding it" — quotes the failure's own arc (reads
"(your blocker dies, attacker lives)" correctly, concludes "blocking it kills the bigger
threat" anyway), names the tell-phrases verbatim ("but actually it survives", "but the
block still stops it"), and states the precedence flatly: a conclusion contradicting a
parenthesis you just quoted is wrong, THE PARENTHESIS WINS, the BLOCKS: answer must agree
with the parentheses of the picked assignments. Evidence: vs158 s8 (t5) — B1 Wolf onto the
3/3 Orc army through a restated dies-line; Wolf died, army lived, Bowmasters unblocked.

**Deliberate deviation**: the validation's alternative ("requiring the echo of the
parenthetical in the block answer") was NOT adopted — the blockers ask ends "Write nothing
else", so mandating extra echo text in the answer collides with the reply protocol (the
owner's binding reply-protocol ruling makes that lane untouchable from the guide). The
consistency form ("must agree with") gets the same self-check without adding output lines.

## Edit 2 — RETURN-SWING check: promoted to the decision point + growth term

The rule-2 bullet is rewritten to open "BEFORE YOU WRITE \"ATTACK:\", RUN THE RETURN-SWING
CHECK - every attackers decision, first." — anchored to the literal token the pilot is about
to emit (the ask's own `ATTACK:` protocol line), which is the closest guide-lane equivalent
of moving it to where the pilot reads it. (True perception-routing — rendering the fact in
the attackers ask when armed — is engine/render-lane; docketed below, not attempted here.)
Two content additions:
- **GROWTH term** (mirrors deck105 step 2): a creature whose line shows +1/+1 counters is
  GROWING (the Orc army class) and counts at its power PLUS ONE for the arming test.
  Calibration: vs158 s36 (t14, life 6, army 5/5 [5x +1/+1]) — old letter 6>5 unarmed, died
  t15 at exactly -1; new letter 5+1=6 ≥ 6 arms it.
- **Sole-attacker clause**: "even when it is your ONLY listed attacker ('ATTACK: none' is
  the right answer then), even when their board 'cannot block this turn'" — names both
  rationalizations from the 0/1 miss verbatim-adjacent (vs36 s22, t8, life 1 vs Cathodion
  3/3: trace checked "no other options" + "they can't block" and never ran the swing-back;
  dead at s23/t9 when Wolf home + Aspirant would have absorbed all 4).
Arming condition, tapped-does-not-protect, lethal-now release, and above-the-number no-op
all preserved from the validated wave-35 text.

## Edit 3 — Katilda-vs-infect precedence closed

One sentence in the infect posture + a cross-reference clause in rule 4's Katilda line:
KATILDA is the ONE exception to block-always, keyed to the string the blocker window
prints — stays out of infect chump-blocks while "you would be at N of 10 poison" reads
7 or less; blocks like everything else at 8+. Precedence direction follows the corpus:
vs105 s10 (poison 2/10, Katilda sole listed blocker on a dies-line vs Ichorclaw) declined
and the validation graded it defensible — the sentence makes that ruling text, with a
countable flip point so the block-always rule still owns the endgame. Both surfaces state
the same threshold so the rule-4/infect collision cannot re-open.

## Docket (not guide-lane, carried forward)

- Perception-routing for the RETURN-SWING check (render an armed-state fact at the
  attackers ask) — validation item 4(b)'s stronger form; engine-lane.
- payRepeatModeNote casing/seam defect (validation item 1) — engine-lane, not touched here.
- Trade-trust crutch 2-corpora retirement clock: corpus 1 read class 3→1 (bar not met);
  this revision restarts the count for the strengthened wording.

## Validation asks for the next corpus

Class counts: blocks into a dies-line contradicting a quoted parenthesis (target 0);
RETURN-SWING armed instances incl. growth-term armings (target: check run in-trace at each);
Katilda infect-block decisions vs the N-of-10 threshold (target: matches the printed
number both directions); confirm no new deliberation loops at the attackers seam from the
promoted check (it now fronts every attackers decision).
