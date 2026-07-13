# deck131 wave-5 -> proposed strategy-writing-skill revisions

Two proposals, both methodological (they change how a reviewer READS the corpus, not the
guide's content). Ranked by confidence.

## 1. [HIGH confidence] Attacker-participation denominator: count declared vs the engine's
OFFERED legal-attacker list, NEVER vs creatures on the battlefield.

Wave 4 adopted "partial answers to bundled asks" as a leak class (declares one attacker while
three are listed). Applying it this wave produced a FALSE POSITIVE until corrected: several
"declared 1, but 2-3 creatures on board" reads were summoning-sickness-correct - the engine only
offered ONE creature as a legal attacker (the others were summoning-sick, tapped, or just-made
tokens). Once I counted declared vs the engine's `A#.` "Your creatures that can attack:" list,
participation was 100% across all six games and the wave-4 concern was resolved, not reproduced.

Concretely: vs135 T11 the reply was literally "A1, A2" (the pilot INTENDED both creatures) but
only "A1. Elemental" was offered because Young Pyromancer had just been cast; the parser correctly
took A1. Scored against the battlefield that looks like under-commitment; scored against the
offered list it is full commitment plus one summoning-sick creature the pilot wrongly named.

Proposed edit to Step 2 (attackers/blockers bullet, the "Partial answers to bundled asks"
sub-bullet): add "The denominator is the engine's OFFERED legal-attacker list (the `A#.` lines),
not the creatures on your battlefield - summoning sickness, tapped status, and just-created tokens
legitimately shrink the legal set, and counting against the battlefield manufactures a phantom
subset-attack leak. A reply naming a creature the engine did NOT offer (an intended-but-illegal
attacker) is the pilot committing fully, not under-committing."

This also belongs in the digest-script guidance: extract the `A#.` option list for attackers
records, same as the numbered option lines for casts.

## 2. [MED confidence] Add a THIRD Step-0 revision mode: "OBEYED-BUT-LOSING".

The skill's turnaround-preservation mode keys on "the deck's record or key metric turned around."
It has no explicit mode for the case this deck presents: the record went DOWN (2/12 -> 0/6) yet
the guide was OBEYED at nearly every seam (100% attack participation, draw-on-curve holding,
Mountain-first mostly followed, blocks correct) and the losses attribute to deck construction and
variance, not to the guide. The danger here is symmetric to a turnaround: a bad n=6 record tempts a
loud rewrite, which would churn the obeyed lines and risk deck110's "faithful rewrite -> 0/6"
outcome in reverse.

Proposed addition to Step 0, after the TURNAROUND-PRESERVATION bullet:
"**OBEYED-BUT-LOSING mode** - when the record dropped but the obedience metrics are HIGH and the
losses attribute to deck construction / variance (opponent life at death near 20 across most
losses; wins/near-wins are the guide working as written), treat it like turnaround mode, NOT like
a failure to fix: freeze the obeyed lines, spend the wave only on the residual model-fixable lever
(here: the underused non-combat engine), and route the record itself to the deck-construction flag.
A falling record on n<=6 with a harder opponent slate is variance, not evidence the guide broke;
verify obedience before rewriting."

## Non-proposals (checked, no change needed)
- The "opponent life at death" fast-triage and the residual->deck-construction routing worked
  exactly as written; no edit.
- "Override the misleading generic prior BY NAME" covered the Guttersnipe reaction-hold override
  cleanly; no edit.
- The harness-defect sweep (desync/no-op/menu-offset) found nothing this corpus (head-first parser
  clean, 0 fallbacks) - the sweep is still worth running; its absence of hits is a clean result,
  not a reason to drop it.

## One reading gotcha worth a digest-guidance line (LOW confidence, optional)
When checking "was card X on the BATTLEFIELD at decision D," isolate the `Your battlefield:` line
specifically - splitting on `--- CURRENT SITUATION ---` and searching the remainder also matches
the card in `Your hand:` and in the carried PLAN, giving false "on board" reads. Cost me one
wrong Guttersnipe-presence pass before I narrowed the match.
