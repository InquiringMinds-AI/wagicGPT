# deck137 wave-31 -- proposals for the strategy-writing skill (next edition)

Two proposals, both promotions/sharpenings of existing rungs rather than new machinery. Layer
check: neither is a guide teach and neither is core-prompt text.

--------------------------------------------------------------------------------
## P1 (PROMOTION-GRADE) -- arrival-tracing needs a SAME-SEAM CONTROL, not just a traced failure
--------------------------------------------------------------------------------
**Where:** the resolution-verification rung / Step 2's REALIZED-MAGNITUDE column, right after
wave-30 method headline 1 ("verify a residual->0 exit gate by arrival-tracing, never by counting
the fallback field").

**Proposal.** Extend the rung: once arrival-tracing shows a class failing, the reviewer's next
obligation is to find a **CONTROL in the same corpus that rides the SAME SEAM and DOES resolve**,
and to report the pair. A traced failure alone localizes the defect to "somewhere in this
decision"; the control pair localizes it to a specific BRANCH and is what makes the note
actionable by a fix agent without re-deriving the seam.

**Evidence (wave-31 deck137).** Four March convoke picks silently no-op'd after the mode menu
`Choose an option for March of the Multitudes: 1. Cast Card Normally / 2. convoke` (vs139
s22-24 / s29-30 / s36-38 / s41-42). On its own that says "the convoke cast is broken somewhere".
The control -- vs148 s53-54, the byte-identical menu shape for Venerated Loxodon answered
`Cast Card Normally`, which narrates `hand -> stack` / `stack -> battlefield` -- proves the mode
menu, the click choreography and the normal branch are all sound, and pins the fault to the
alternative(convoke) branch. That one comparison converted a four-wave-old "convoke is flaky"
note into a one-line fix target. The pair is also the fix's own negative control (a fix must not
break the branch that works).

**Cost:** near zero -- the control is usually already in the same six logs.

**Falsifiable form for the DoD:** a residual/silent-no-op note is not DONE until it names
(a) >=1 traced failing attempt with seqs, and (b) >=1 same-seam attempt that resolved, or an
explicit statement that no control exists in the corpus.

--------------------------------------------------------------------------------
## P2 (SINGLE-SEAT, tag it) -- a "PROMISE THE ENGINE OWES" is a THIRD guide-prose state, distinct from a correct teach and a stale-render stopgap
--------------------------------------------------------------------------------
**Where:** as a face on the STOPGAP-RETIREMENT lifecycle rung (wave-30 method headline 3), and in
the Step-0 freeze/revise decision.

**Proposal.** Name the state where a guide line is (i) rules-correct, (ii) describes what the
engine INTENDS and its fixture claims, and (iii) has never once been observed to happen live. It
is NOT a misteach (nothing about it is wrong) and NOT a stale-render stopgap (no render
contradicts it) -- so neither existing rung fires, and the default "freeze" is correct. But the
reviewer must **record it by name with the corpus count**, so a later reviewer can convert it to
a stopgap-retirement if the engine work never lands. Without the name, the line just silently
persists across freezes and nobody owns the question.

**Evidence (wave-31 deck137).** Guide line 55: *"Convoke lets tapped creatures pay part of X too,
so a wide board makes a BIG March."* That is CR 702.51, it matches what `maxAnnounceableX` was
just changed to do, and the gold fixture `march_convoke_x_credit` asserts it. Live it has
delivered **0 Soldiers from 5 March picks over 6 games this wave**, and 1 resolution in 9 picks
the wave before. Editing the line would be wrong (it would teach the pilot to avoid a card the
deck is built around, on an engine bug's authority); leaving it unnamed loses the thread.

**Suggested reviewer phrasing:** `PROMISE OWED: <guide line> -- rules-correct, engine-intended,
observed 0/N live this corpus; revisit as a stopgap-retirement candidate if <ledger item> does
not land.`

**Tagging:** single-seat. Promote only if a second seat reports a guide line in the same state.

--------------------------------------------------------------------------------
## Not proposed (considered and dropped)
--------------------------------------------------------------------------------
- A guide teach for Giant Killer's `{1}{W},{T}: Tap target creature`. 29 priority windows, 28
  passes, and the passes are CORRECT (the ability taps Giant Killer itself, costing an attacker
  in a beatdown deck; the model's own reasoning at vs148 s51 gets this exactly right). Teaching
  restraint that already exists is the anti-pattern the skill's "cut what already works" rule
  forbids.
- Any rung about the X=0 March cast at vs152 s33. The guide already forbids it in explicit words
  (lines 51-52) and the pilot violated it once in six games. One lapse against optimal prose is
  variance, not a teach defect -- covered by the existing evidence-reading discipline.
