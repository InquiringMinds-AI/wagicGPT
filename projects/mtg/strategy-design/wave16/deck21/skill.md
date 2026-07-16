# deck21 FIRST-GUIDE VALIDATION — skill deltas (wave-16)

Current skill = `strategy-design/wave15/strategy-writing-skill.md`. These are METHOD deltas
from validating deck21's initial guide against its first guided corpus. Each is method, not
deck prose.

## V1 - A guideless-pass guide's biggest first-corpus RISK is an OVER-BROAD countermand, and it hides in a MATCHUP section.
deck21's initial guide correctly countermanded the core "hold instants" reflex (w15 D2) and
fixed 5 taught classes cleanly. Its ONE first-corpus failure was the opposite shape: a
matchup rule that was written TOO BROADLY. "Walls of big or flying blockers (... a faerie
flyer swarm ...): send EVERY burn spell at the face" collapsed two distinct situations - a
HIGH-TOUGHNESS wall you cannot kill (correct: go face) and a SMALL-flyer swarm you CAN kill
but cannot block (wrong: an early kill is the winning line). The model obeyed the specific
matchup line over the general EXCEPTION clause and lost two races by exactly 4 life. Method:
when a guide has both a general exception ("kill X on sight") and a matchup section, CHECK
THEM FOR CONTRADICTION - the more-specific matchup line wins in the model's reasoning and
will silently override the exception. A first-revision often TIGHTENS an over-broad
countermand, not just adds facts.

## V2 - "Worth more than 3 face damage" is a BAD evocation for a small recurring threat; give the MECHANICAL race math instead.
The w15 exception said kill "an evasive attacker (a flyer) that is outracing you... worth
more than 3 face damage." It never fired for a 1/1 flyer: "worth more than 3 face" makes a
1/1 look beneath a burn spell (naive 1 < 3), and "outracing you" reads as needing a big/scary
flyer. The fix that works is the concrete mechanism the deck's own structure supplies: this
deck has NO blockers, so an unblockable attacker deals its power to you EVERY remaining turn;
over an N-turn race a 1/1 flyer costs N life > 3. Replace value-judgment evocations ("worth
it", "outracing") with the countable rule the board state proves. This is the
adaptable-cognitive-guidance principle: word choice selects the implicit procedure - "worth
more than 3" invokes a wrong one-shot comparison; "costs you 1 per turn for the rest of the
race" invokes the right cumulative one.

## V3 - VALIDATE THE FIX-WINDOW, not just the fix direction. A kill-the-threat rule needs a "do it EARLY" clause when the threat grows.
deck21's flyers were killable ONLY briefly: Faerie Bladecrafter 2/2->6/6, Cloud Sprite and
Briarberry 1/1->3/3 under liege lords/counters within a few turns. A bare "kill the small
flyer" rule fired-too-late is worthless (burn bounces off a 4-toughness body). When a
teaching point says "remove threat X with resource Y (toughness-limited)," and the corpus
shows X GROWS, the rule MUST carry the timing: early or never. Read the board deltas across a
game (the counters/printed-vs-current stats in the annotation) to detect a growing target
before writing the removal rule.

## V4 - The guideless->guided TAX COLLAPSE is the primary evidence the guide worked; read it before the win-rate.
deck21 dropped 3/6 -> 2/6 but decision counts fell from w15's tax-heavy 40+/game to 18-24 and
fallbacks fell to 5 corpus-wide with 0 scavenged casts. Every w15 top tax spike (Spark timing
2,773 words; sick-token alpha-strike 1,266; hold-burn quote) VANISHED. The win-rate is a
coin-flip surface at this seat (3 of 4 losses within 4 life; one pure mana-flood, one pool
swap 140->62). When validating a first guide, separate the guide's JOB (did the taught classes
die? did the tax collapse?) from the RECORD (which the pool change and variance move more than
the guide does). Report died/persisted/mutated per class before touching the record.

No changes to the wave-13/15 method headlines (absent-echo staleness, attackers name->index
reconcile, game-log-resolution scoring, teach-engine-reality-not-oracle, reasoning-tax-as-
saliency) - all held at this seat this wave.
