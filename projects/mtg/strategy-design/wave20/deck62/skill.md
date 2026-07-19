# deck62 wave-20 -- proposed additions to the strategy-writing skill (Step 0)

deck62 is now a FOUR-guided-corpus seat (revised round 4). All items grounded at this seat; flagged
single-seat where they are. Two are refinements of live wave-20 instrument questions (salvage-extension
scoring; the intra-guide-contradiction rung).

## 1. (STRONG, reviewer instrument) Score a combat SALVAGE extension on whether a COMMITTED, salvageable line EXISTS in the reply -- a "no-fire" is a CORRECT result when the reply truncated PRE-COMMITMENT, and it must not be logged as a salvage miss.
The wave-20 batch extended `salvageLoopedChoice` to BLOCKS/ATTACK/PUT. deck62's two blockers-class
fallbacks (vs14 s26, vs135 s19) are the live test, and the extension fired at NEITHER -- correctly. Both
replies truncated BEFORE emitting any coherent BLOCKS commitment: vs135 s19 has NO `BLOCKS:` line at all
(advancing block-math spiral, dies mid-sentence); vs14 s26 contains only `BLOCKS: none` (x2, as reasoning
asides) whose token is the OPPOSITE of the model's repeated stated intent ("So I block with Yavimaya"), so
salvaging it would have EXECUTED THE WRONG ANSWER. LESSON: a salvage extension targets a reply that CONTAINS
a mis-formatted-but-committed choice (a looped/duplicated coded line). A truncation-before-commitment is a
DIFFERENT class it cannot and must not touch. When validating a salvage extension, first classify each
fallback reply as (a) committed-but-mis-formatted (salvage's job) vs (b) truncated-pre-commit (below the
salvage layer -- a decode/length problem). Score the extension only against class (a); do not count a class-
(b) miss against it, and do not "improve" the salvage to grab a contradictory literal token from a class-(b)
reply. Sibling to the wave-11 "score block-side obedience by the reasoning AND the next record's combat
events, never chosen_text" rung: here the same seam shows the parse layer has THREE states, not two --
parsed / salvageable-mis-format / truncated-pre-commit -- and only the middle one is the extension's surface.

## 2. (STRONG, refines the intra-guide-contradiction rung) A newly-added off-case caution can become DECODE-LOOP FUEL when it introduces a near-contradiction the executor cannot resolve against an accurate PROMPT annotation -- the fix is to SHARPEN THE CAUTION'S SCOPE, not to delete it (it is obeyed at its real target).
Wave-19 added edit B to deck62 ("do NOT chump-block your 0/1 engine to absorb non-lethal damage while
comfortable"). Wave-20 shows it OBEYED at its clear target windows (the model took non-lethal damage while
ahead 3x rather than chumping the engine) BUT also fueling a verbatim decode loop at vs14 s26: offered a
plainly-good block (prompt tag "(you kill it, your blocker lives)" on a 5/5 into a 2/2), the model looped
to truncation litigating whether that favorable trade counted as the guide's forbidden chump-block, quoting
edit B's exact words -- an echo-test-positive GUIDE-CAUSED spiral -- then the heuristic sacrificed the engine
anyway. LESSON: when a #1-band caution is OBEYED at its target but CITED inside a spiral on an ADJACENT
decision, the diagnosis is SCOPE BLEED, not weakness or overshoot. Do not delete the obeyed caution and do
not shout it; add a decisive EXCEPTION that names the adjacent case the caution must NOT touch, deferring to
the accurate prompt annotation ("a favorable trade with a REAL creature is not a chump -- just make it").
This is the intra-guide-contradiction rung applied to a CAUTION-vs-ACCURATE-ANNOTATION tension (not two
guide rules): the executor resolves toward whichever clause is most concrete, and an accurate option-line
annotation loses to a loud guide caution unless the guide explicitly cedes to it. Bound the confidence: the
carrier is a decode loop (below the guide layer), so a scope-sharpen REMOVES THE FUEL but is not a claimed
fix -- frame it like a nudge (mirror of wave-19 edit C's mulligan-clause framing).

## 3. (single-seat, engine ledger discipline) When an unparse routes to a HEURISTIC that then misplays, attribute the misplay to the HEURISTIC, not the guide or the model -- and check whether the heuristic makes the exact mistake a guide clause was written to prevent (in which case the guide clause is structurally unable to reach it).
Both deck62 blocker fallbacks ended with the baka blocker heuristic GANG-BLOCKING a small attacker with the
0/1 shroud engine when a single real blocker sufficed -- needlessly sacrificing the draw engine. This is the
precise behavior edit B forbids, occurring BELOW the model's (never-emitted) answer, where guide prose cannot
reach. LESSON: a fallback path has its own decision quality, and when a guide clause targets a behavior that
ALSO occurs in the heuristic, the clause can only ever cover the model's half; the heuristic half is an
engine item. Separate the two ownership lanes explicitly before crediting or faulting the guide -- a guide
clause obeyed by the model AND violated by the heuristic on the same class is not a guide failure.

## 4. (confirmation, cross-wave) The computed-P/T distrust pocket stays DEAD at round 4 -- a retired watch verified by a NEGATIVE grep is worth one line, not a re-audit.
Fourth consecutive corpus with zero recount-of-already-printed-P/T (tight-signature grep = 0). Every
Blanchwood mention is prospective cast-value math (unbannable). LESSON: once a pocket is retired across
multiple waves, verify it with a cheap negative-signature grep and report the null in one line; do not
re-open the full audit each wave. (Reinforces the wave-18/19 verdict; no new method.)
