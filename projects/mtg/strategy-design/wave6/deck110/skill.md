# Skill proposals from deck-110 wave-6

Additive method refinements for the strategy-writing skill. deck110 this wave is the
CLEAN case study for two existing skill lines; it also surfaces one new one.

## 1. NEW: verify a shipped REPRESENTATION fix actually FIRES in the corpus before crediting it.
Wave 5 correctly diagnosed the re-equip no-op as a representation defect and routed it to an
engine marker (the option-line "(ALREADY attached ... this would change NOTHING)" label).
The marker SHIPPED — and fired **zero times** for deck110, because it checks `auraParent`
while equipment stores its host in `->target` (auras and equipment are different fields; the
engine explicitly excludes equipment from auraParent). The no-op rate was unchanged (9/18,
50%). **Lesson for the method:** when a prior wave routed a leak to a representation/engine
fix and that fix has since "shipped," the reviewer's FIRST step is to grep the new corpus for
the marker string and confirm it appears on the decisions it targets. A shipped-but-inert fix
looks identical to a shipped-and-obeyed-but-weak fix in the win column, but the remedy is
opposite (fix the code vs. abandon the approach). Add a one-line check to Step 0: "For every
leak a prior wave routed to representation, confirm the representation actually appears in
this corpus before re-diagnosing." This is the composition-seam rule applied across waves:
the fix lived in a different file than the diagnosis, and only end-to-end observation catches
the mismatch.

## 2. REINFORCE: OBEYED-BUT-LOSING is the correct frame for a variance-driven record drop.
deck110 went 5/6 -> 1/6 raw (3/6 adjudicated) with EVERY obedience metric healthy (cast-
nothing ~10%, mana-delusion gone, go-wide intact, Lightmine rewrite working) and all three
losses attributable to draw variance (flood, close race, weakest-body draw) against a slate
that gained a working counter deck. This is the deck110-in-reverse of its own wave-4 warning:
a bad short record tempts a rewrite; the rewrite churns obeyed lines and manufactures the
0/6. The skill already carries OBEYED-BUT-LOSING mode — this wave is a textbook confirmation.
Sharpen the entry test: **if the WIN and the adjudicated-timeout-wins show the deck executing
its plan, and the losses each lack a precondition the guide can't create (a threat curve),
the record drop is variance — freeze and route to deck construction, do not rewrite.**

## 3. REINFORCE: the block seam can be STRUCTURALLY ABSENT; a "block to survive" rule needs
its upstream ATTACK-seam partner. deck110 had ZERO blockers windows in 6 games — every
would-be blocker was tapped attacking, so THE RACE's "chump at <=8" could never fire. This is
the mirror of the wave-5 deck135 finding (a defender role that only changes BLOCKING does
nothing when stabilization happens at the CAST seam). Generalize: **when a survival rule lives
at the block seam, check whether that seam even ARRIVES; if the pilot taps out attacking, the
survival instruction must be written at the ATTACK seam ("don't attack with a creature you
need to block") instead.** Add to Step 2's attackers/blockers bullet.

## 4. REINFORCE: separate the ecosystem/meta shift from a guide failure in attribution.
deck110's wave-4/5 dominance was partly an artifact of a blind counter seam (the stack wasn't
surfaced, so opponents' counters were dead); wave-6 closed that seam, and the counter deck
now answers affinity's discrete threats. This is a fourth attribution axis alongside bad-play
/ variance / harness-defect: **META SHIFT — a rule that was winning because the opponent
couldn't punish it, now losing because a platform fix made the counter work.** The guide
didn't change; the world did. Record it as a deck-construction/meta flag, not a guide bug,
and do not "fix" the guide against it (the correct response is deck-list or go-wide
sequencing, not louder threat prose).
