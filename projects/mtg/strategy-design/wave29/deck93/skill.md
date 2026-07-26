# deck93 skill proposals (wave 29) - THE NATURAL EXPERIMENT'S CLOSING CHAPTER

## PROPOSAL S-93a (CONFIRMED - promote from candidate to established heuristic):
## Perception leaks die to ANNOTATION on the perception surface, not to guide prose.
## The perception-vs-strategy discriminator now has a completed controlled experiment.

Wave-28 opened this natural experiment INSIDE one deck: two same-class perception leaks
(land-blindness at mulligan, phantom-attackers at combat), both given the guide prose, only
one given a surface annotation. The annotated one (land-blindness) died; the guide-only one
(L2 phantom attackers) failed 11/15. But wave-28 could not fully isolate the lever, because
the KILLED leak (mulligan) had BOTH treatments co-deployed and the model cited the GUIDE, not
the tag. Wave-29 closes the loop: the guide-only failure (L2) got its missing treatment - the
`[summoning sick - cannot attack this turn]` annotation, with the GUIDE FROZEN BYTE-IDENTICAL.

### The completed experiment (one deck, three waves, guide constant)

| leak class          | guide teach | surface annotation | phantom/miss rate     |
|---------------------|-------------|--------------------|-----------------------|
| L2 phantom attacker | yes (w27-29)| NO  (wave 28)      | 11/15 over-listed     |
| L2 phantom attacker | yes (frozen)| YES (wave 29)      | 0/21 (sick class)     |

The guide text for L2 never changed across the transition. The ONLY variable between the
73% failure and the ~0% failure was the addition of the inline annotation on the render
surface. This is the clean single-variable isolation the mulligan case alone could not give.
And unlike the mulligan (model cited the guide), here **18/21 replies cite the tag/restriction
DIRECTLY** ("Relentless Rats #2 entered the battlefield this turn ... summoning sickness and
cannot attack") - the model is demonstrably reading the annotation off the surface, not the
guide. Annotation-lever, confirmed with a negative control AND direct-citation evidence.

### The established principle (was a candidate, now promoted)

Classify a leak BEFORE routing its fix:
- PERCEPTION leak = the model reads game state WRONG (mislabels a hand, miscounts legal
  actors, misreads a magnitude/state). It does not lack the instruction - it fails to apply
  the instruction against a perception surface that contradicts or under-specifies it. Guide
  prose is the WRONG tool: L2 proves guide-alone fails even when the guide says exactly the
  right thing. The RIGHT tool is to ANNOTATE THE FACT INLINE on the surface the model reads.
- STRATEGY leak = the model reads state correctly but DECIDES wrong. Guide prose IS the right
  tool - L3 (over-blocking / lord-shrink, wave 28) proves it: the fact is cited verbatim and
  behavior changed.

Routing rule (skill text, condensed): "Before writing a guide teach for a leak, ask: is the
model MISREADING state or MISDECIDING with state read correctly? Misreading = perception leak;
guide prose will NOT fix it (the model already ignores correct A-lines / miscounts tagged
lands). Fix perception leaks by annotating the fact INLINE on the render surface the model
consumes (state tags: land identity, summoning-sickness, tapped-state, magnitudes). Reserve
guide teaches for genuine strategy leaks - decisions wrong even when the board is read right.
Verification that the annotation landed = look for the model CITING THE TAG'S FACT in its
reasoning (18/21 here), not merely the corrected action."

## PROPOSAL S-93b (REFINED - annotation WORDING is itself load-bearing)

New evidence this wave: the ONE residual phantom (137 seq18) was NOT a summoning-sick failure
- it was a `[tapped - untaps and can attack next turn]` MISREAD. The model quoted "can attack
next turn," dropped "next turn," and declared the tapped creature as an attacker now. The
annotation worked as a lever (the sick tags on the same board were obeyed perfectly), but the
tapped-tag's wording embeds an attack-affirmative phrase ("can attack") that a fast reader
mis-binds to the present turn.

Skill implication: an annotation is only as good as its WORDING. A restriction tag that
contains an affirmative substring about the very action it restricts is an attractive-nuisance.
Prefer restriction-first phrasing that leads with the negative and never states the permitted
action in the same clause (e.g. `[tapped - CANNOT attack this turn]` rather than
`[tapped - untaps and can attack next turn]`). This is the mirror of why the sick tag works so
cleanly: it leads with "summoning sick - cannot attack this turn," negative-first, no
attack-affirmative substring. The perception-vs-strategy heuristic gets a corollary: when you
route a perception leak to an annotation, AUDIT THE ANNOTATION'S WORDING for
restriction-first, action-affirmative-free phrasing.

## PROPOSAL S-93c (closing meta-note on the deck93 arc for the skill's record)

deck93 completes a THIRD full pipeline arc and is the cleanest single-deck demonstration in
the corpus of the guide-layer / engine-layer split. Its lasting contribution to the skill is
not a strategy fact but a ROUTING METHODOLOGY validated end-to-end: (1) a leak was
mis-diagnosed as guide-fixable and given prose (wave 27-28); (2) the prose demonstrably failed
under a within-deck negative control (wave 28); (3) re-routed to a surface annotation, the
class died with direct-citation confirmation (wave 29). Reviewers should treat "guide prose
was added and the leak persisted" as a POSITIVE diagnostic that the leak is perceptual and
belongs at the annotation layer - not as a reason to write MORE prose.
