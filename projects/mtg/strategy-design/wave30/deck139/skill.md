# What the MUTATE surface teaches the strategy-writing skill (deck139, wave-30 FIRST-GUIDED)

Wave-29 opened four render gaps on this deck and routed them to the ledger; wave-30 shipped the
fixes (binary 6e168aa05) and this seat VALIDATED them. The durable lessons:

## LESSON 1 (PROMOTE TO ESTABLISHED) -- the STOPGAP-RETIREMENT lifecycle for perception fixes
The wave-28 perception-vs-strategy rung says a perception leak dies to a SURFACE ANNOTATION, and
the guide carries a STOPGAP trap-warning "while the render fix is routed to notes." Wave-30
closes the loop: **once the render fix SHIPS, the stopgap prose must be RETIRED, and the guide
re-aligned to the live render strings.** Leaving stale stopgaps is a NEW mislead -- deck139's
wave-29 guide told the model to expect "TARGET CHOICE for Pollywog listing your HAND" and a
"TWO adjacent lines" pile; both renders no longer exist (now "DISCARD ONE OF YOUR OWN CARDS" and
a single "[mutated pile - ONE merged creature]" line), so a byte-copy would have shipped prose
that contradicts what the model actually sees. The skill rule: **a render-stopgap has a
lifecycle -- write it -> fix the render -> retire/rewrite the stopgap to match the shipped
render.** The reviewer who owns the seat the wave the fix ships is responsible for the retirement
(this is a principled revision even when "no teach misled," because a stale render description IS
a latent misteach).

## LESSON 2 -- validate a render fix with a BEFORE/AFTER quote at the MOTIVATING seat
The clean way to verify a shipped representation fix is to quote the wave-N render and the
wave-N+1 render of the SAME decision class side by side, at the seat that motivated it. Four
one-line before/afters (N-139a/b/c/d in findings.md) settled "did the fix land?" in minutes and
proved the misplay class (accidental discard) dead. Skill rule: **a representation-ledger item
is CLOSED only by a before/after render quote plus a decision that used the fixed render
correctly -- not by "the code compiled" or "win-rate went up."**

## LESSON 3 -- fixing PERCEPTION latency does NOT fix STRATEGY latency; the tail MOVES
Wave-29's 195-199k ms tail was mutate-render rules-lawyering. The render fix made the mutate FLOW
fast (11-17k ms) -- but the >195k tail is UNCHANGED, because it MOVED onto combat math and
big-board deliberation (double-strike/trample/reanimation). Skill rule: **a compound-mechanic
deck carries two distinct latency sources -- PERCEPTION (render ambiguity, curable at the surface)
and STRATEGY (intrinsic decision complexity, curable only by a guide "decide-and-commit" teach or
a core reasoning-length bound). Do not credit a render fix with the whole tail; re-locate the
tail after the fix and route the residual to the correct layer.** Corollary observed here: on a
wall-clock-capped harness, a slow seat's 200s decisions cause EARLY adjudication losses, so the
"decide combat fast" teach is not just a truncation hedge -- it directly defends games.

## LESSON 4 -- a static-ability discount that survives a merge is BOTH a feature and a crash risk
N-139g answered POSITIVE: an under-card static (Pollywog's mutate-cost reduction) keeps
functioning while buried (correct CR 725). The SAME code path (applying a discount to a mutate
cost) underflowed and crashed when the discounted cost drove a mana component negative
(N-139h). Skill rule: **when the guide teaches "keep card X for its passive discount," verify the
discount's INTERACTION with the exact costs it will modify -- a cost-reduction that is correct in
isolation can underflow a specific multi-symbol/hybrid cost. Flag such interactions as an
engine-verify item, not an assumed given.** (This does not change the guide teach -- keeping
Pollywog is still right -- but it is why the crash is on the ledger.)

## VERIFY-ORACLE rail (held again)
Every card fact re-read from primitives + Scryfall. N-139e reconfirmed: Migratory Greathorn's
land-search is mutate-ONLY (primitive + Oracle agree); a hardcast ramps nothing. Recorded so no
future reviewer re-opens it. Mutate costs still must be read from the BASE `other=` line, not the
Pollywog-reduced number in the option (the reduced {1}{g}{b}{w} on Nethroi is the discount at
work, not the printed cost).
