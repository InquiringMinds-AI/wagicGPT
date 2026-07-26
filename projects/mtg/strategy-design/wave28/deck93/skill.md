# deck93 skill proposals (wave 28) - for the synthesis agent

## PROPOSAL S-93a (PROMOTION CANDIDATE): Perception leaks die to ANNOTATION on the
## perception surface, not to guide prose. Distinguish PERCEPTION from STRATEGY leaks
## before choosing a layer.

This wave produced a clean natural experiment INSIDE one deck, one corpus, that isolates the
lever the deck22 d2 double-lever case could only suggest.

### The two leaks, same deck, same wave, opposite outcomes

- **Land-blindness (mulligan)**: a PERCEPTION leak (model misreads state - calls three Swamps
  "zero lands"). Treated with BOTH a guide teach (RULE #1) AND an annotation on the perception
  surface (e4 land tag `Swamp (land: taps for {B})`). Result: **DEAD, 6/6 kept.**
- **Phantom attackers (L2)**: also a PERCEPTION leak (model misreads state - counts board Rats
  as legal attackers, ignoring summoning-sickness). Treated with a guide teach ONLY ("count
  from the A-lines, not the board") and NO annotation (the board render tags `[tapped]` but not
  summoning-sickness). Result: **STILL PRESENT, 11/15 attack declarations over-list.**

Both leaks got the guide prose. Only one got the surface annotation. Only the annotated one
died. This is the isolation the mulligan case alone cannot give (there the model cites the
guide, not the tag, so guide-vs-tag is confounded). The WITHIN-wave L2 contrast supplies the
control: guide-alone demonstrably failed on an identical class of leak.

### The principle to promote

Classify a leak before routing its fix:
- **PERCEPTION leak** = the model reads the game state WRONG (mislabels a hand, miscounts
  legal actors, misreads a magnitude). The model does not lack the instruction - it fails to
  apply the instruction against a perception surface that contradicts or under-specifies it.
  Guide prose is the WRONG tool: adding more text does not override what the board render shows
  (L2 proves this - the guide already says exactly the right thing and is ignored). The RIGHT
  tool is to ANNOTATE THE FACT INLINE ON THE SURFACE the model reads (the land tag; the needed
  summoning-sick tag).
- **STRATEGY leak** = the model reads state correctly but DECIDES wrong (over-blocks a
  favorable board, feeds threats into bad trades). Here guide prose IS the right tool - L3
  proves it: the lord-shrink guide fact is cited verbatim and the behavior changed.

Routing rule for the skill: a leak whose failing reasoning MISREPORTS the board is a perception
leak -> route to an ENGINE/REPRESENTATION annotation (notes.md), and do NOT expect guide prose
to carry it. A leak whose reasoning reports the board correctly but weighs it wrong is a
strategy leak -> guide teach.

### Why this is a promotion candidate (second instance, now with isolation)

- deck22 d2 (prior wave): annotation-lever kill, model cited the tag directly = clean signal
  but single-lever, no negative control.
- deck93 wave-28 (this wave): the same annotation-lever kill (land-blindness) PLUS a negative
  control (L2, guide-only, same leak class, FAILED). The pairing upgrades "annotation kills
  perception classes" from an observation to a routing HEURISTIC with a discriminator
  (perception vs strategy) and evidence that the discriminator predicts which layer works.

Suggested skill text (condensed): "Before writing a guide teach for a leak, ask: is the model
MISREADING state or MISDECIDING with state read correctly? Misreading = perception leak;
guide prose will not fix it (the model already ignores correct A-lines / miscounts tagged
lands). Fix perception leaks by annotating the fact INLINE on the render surface the model
consumes (state tags: land identity, summoning-sickness, magnitudes). Reserve guide teaches
for genuine strategy leaks - decisions that are wrong even when the board is read right."

## PROPOSAL S-93b (minor): guide-attribution is a validation signal, but not a lever-isolation
Every validated teach this wave (SNAP KEEP, lord-shrink) is cited VERBATIM in the model's
reply. That verbatim citation is a reliable "the guide teach landed" signal for reviewers - but
it does NOT isolate the guide from a co-deployed annotation (mulligan: guide cited, tag also
present and possibly redundant). When a teach and an annotation ship together, look for a
same-wave negative control (a same-class leak that got only one lever) before claiming either
lever alone is sufficient.
