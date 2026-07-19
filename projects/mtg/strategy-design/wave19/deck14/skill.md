# deck14 (Deep Blue) — skill.md — proposals for the strategy-writing skill

Second appearance (first GUIDED corpus). All items are guide-authoring METHOD; none touches core.
The value here is a clean before/after on a FIRST-GUIDE that lets us characterize HOW a taught class
moves under an initial guide.

## PROPOSAL 1 (NEW rung, STRONG — promote the wave-18 single-seat WATCH to a validated method) —
## the COMBAT-ANSWER FORMAT-DRIFT rung is CONFIRMED: pairing "you rarely block / trust the rendered
## [X/Y]" with an explicit CODE-FORMAT discipline line COLLAPSES the combat-fallback class on a
## math-heavy deck.
Wave-18 deck14 flagged this as a single-seat watch (3 of 5 fallbacks were 8.6-13.1k-char combat
rambles that drifted out of the coded format). Wave-19, with the guide's Rule #2 in place (and the
engine's combat name-tolerance batch alongside), the class DIED: combat replies collapsed to
300-800 chars, and the two residual tax spikes (vs35 s26 8,307 / vs49 s28 4,492) both PARSED to
clean coded lines instead of falling back. This is a real, measurable guide effect. METHOD for the
skill: for ANY derivation-heavy-combat deck (flyers + lords/anthems + recursion), ship the
short-answer + code-format discipline line as a standard rung; expect it to convert the tax from a
FALLBACK source into a merely-verbose-but-parsed decision. (The engine parser tolerance is the
belt; the guide line is the suspenders — they compound.) No longer single-seat; validated across
this seat's before/after.

## PROPOSAL 2 (NEW rung, STRONG) — an attack-FLOOR rule reliably kills the 0-attacker durdle but
## the durdle MUTATES into a SLOW-CLOCK soft-durdle; a first guide's aggression rule must be written
## in TWO tiers (declare attackers AND deploy the real threat / swing the whole board), or tier-2
## failures replace tier-1 failures.
This is the headline method finding. deck14's wave-18 loss signature was ZERO attackers. The
wave-19 guide's Rule #1 ("a turn with a live untapped creature and no attack is a misplay") FIRED
hard — the model literally quotes it verbatim in a reply (vs49 s28) and every game now declares
attackers. But the deck still lost the two games where it attacked with a single small creature
while its FINISHERS (flyers) sat in hand and the opponent's life barely moved. The durdle did not
die; it relocated one level down — from "don't attack" to "attack trivially." LESSON: an aggression
countermand should not stop at "declare attackers." It must also specify WHAT a real clock is for
that deck (deploy the finisher; race the mana to it; swing the whole board), because a model that
learns to satisfy the literal floor will satisfy ONLY the literal floor. Generalizes the wave-17/18
"be aggressive" family: state the aggression rule at the altitude of the WIN CONDITION, not the
altitude of the combat step. Applies to any deck with a distinct finisher behind a cheap opening
(aggro-with-a-top-end, evasive-finisher tempo, reanimator-with-a-bomb).

## PROPOSAL 3 (reinforcement, no new text) — the INVENTED-OPTION-NUMBER / list-authority failure
## has a stable sub-shape worth naming: MENU/INTENT MISMATCH (narrate action X, emit an index that
## points at option Y).
Wave-18 saw one invented number (out-of-range). Wave-19 gives three instances that share a tighter
signature: the model builds a correct plan in prose, then appends a CHOICE index pointing at a
DIFFERENT listed option (vs49 s26 "Boomerang the Liege" -> index was Island; vs135 s27 "cast
Counsel" -> index was Play Island; vs27 s15 "cast nothing" -> index was Lord of Atlantis). Naming
it "menu/intent mismatch" (distinct from out-of-range over-reach and from unpayable-decline
over-restriction) helps the guide off-ramp ("re-read the option your number points to and confirm
it matches your intent") and correctly routes the DURABLE fix to representation/engine, not core.
Corpus-wide list-authority family member; stays below core.

## PROPOSAL 4 (method note, no new text) — an OBSERVABILITY gap can silently defeat a per-seat
## validation assignment; the skill should tell reviewers to CHECK their seat's log has the field
## the brief's focus item needs BEFORE reporting on it.
This seat was asked to validate TRADE-OUTCOME block annotations, but every `blockers`/`attackers`
record here logs `options_text` EMPTY — the annotation surface is not present in deck14's logs, so
the focus item was unverifiable from this seat (reported honestly as a gap, not silently skipped).
Method: when a brief assigns a decision-surface validation, first confirm the surface is LOGGED at
your seat; if not, say so with the field name so synthesis knows the coverage hole is a logging
issue, not a clean pass.

## Convergence summary
- Confirmed general (already in skill): mana-is-trivial rule (mono-color); attack-floor for
  aggro/tempo first guides; countermand-a-core-reflex; the 0-attacker durdle instrument.
- PROMOTED this wave (validated off single-seat): the combat-answer FORMAT-DRIFT rung (Proposal 1)
  — confirmed to convert combat fallbacks to parsed decisions.
- NEW general method (Proposal 2): write aggression countermands in TWO tiers (attack AND be a real
  clock) — a first guide's tier-1 fix reveals a tier-2 slow-clock failure.
- NEW sub-shape named (Proposal 3): menu/intent MISMATCH within the list-authority family.
- NEW reviewer-discipline note (Proposal 4): verify the focus surface is logged at your seat.
