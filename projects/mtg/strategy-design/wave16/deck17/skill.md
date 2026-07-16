# skill.md - deck17 first-guide-validation method learnings (wave-16)

Proposals for the strategy-writing skill's FIRST-GUIDE-VALIDATION doctrine (the "did it die /
persist / mutate?" pass). Single-seat evidence is a WATCH, not an adopted line; flagged as such.

## WATCH-1 - When validating a guide, score every "under-action" claim against the ENGINE'S
eligibility list before calling it a play failure.
The tempting wave-16 read was "deck17 chronically under-attacks (1 attacker/turn while behind)."
It is FALSE. Matching each `attackers` choice to the engine's `A1..An` eligibility list showed
every single-attacker turn had exactly ONE eligible creature (rest summoning-sick or defenders);
the model attacked with all it was offered, every turn, in all 6 games. The `chosen_text` alone
looks like passivity; the eligibility list in the prompt is the disproof. GENERAL METHOD: a
first-guide validation must reconcile the choice against the OFFERED set - a taught floor that
"looks disobeyed" is often obeyed against a set you didn't read. This is the attacker-seat cousin
of the wave-15 defender-drop lesson (score `ATTACK:` against Defender tags).

## WATCH-2 - A corrective #1-rule can KILL its target belief and reveal a DIFFERENT belief
underneath in the same domain. Name the successor, do not re-teach the corpse.
deck17's hybrid off-color belief (the wave-15 #1 rule + the new annotation) is DEAD - zero
recurrence. But the reasoning-tax it drove did not vanish; it relocated to STALE WITHIN-TURN MANA
tracking (the model recounts mana from its Island total on a second same-turn cast and distrusts
the correct cast list). The validation move: when a taught belief dies but the TAX SIGNATURE
(giant replies at the same seam) persists, look for the successor belief rather than concluding
the fix failed - and route the successor correctly (here: representation/trust-line, not another
hybrid paragraph). Reply-length at the seam is the tax gauge that survives the belief swap.

## WATCH-3 - A positive deployment heuristic ("cast the BIGGEST X") will over-trigger on any
member of X that is a trap; scope the superlative to the PROPERTY you actually want.
"Cast the biggest flyer you can afford" mis-fired because the biggest flyer on the menu was a
DEFENDER (Plumeveil 4/4). The model even reasoned "a 4/4 wall does not advance the clock" and
cast it anyway - the guide's literal superlative beat the model's correct instinct. This was the
ROOT CAUSE of both losses. LESSON for guide-writing: when you give a "cast the biggest/best Y"
rule, and the card pool contains a Y that satisfies the surface property (a 4/4 flyer) but not
the intent (an attacker), scope the superlative to the INTENT ("biggest flyer THAT CAN ATTACK"),
or the model will pick the trap and quote your rule while doing it. Applies to any deck mixing
big walls with the real threats.

## WATCH-4 - "Almost always true" preconditions leave the OFF-CASE unguided, and the off-case is
where a validated guide loses.
The guide's spine is "the opponent has no flyer, which is almost always." True for 4/6 opponents
- and the deck won all 4 cleanly. Both LOSSES were the 2 opponents that DO fly / gain life, where
the precondition silently fails and the guide gave no racing instruction. Validation lesson: when
a first guide is built on an "almost always" board fact, WRITE the off-case branch in the first
revision even if the winning corpus never needed it - the losing corpus lives entirely in the
off-case. (Reconfirms the skill's "don't assume the common case" rung on a new archetype.)

## Reconfirmed (no change)
- Core's "every listed option is legal AND payable" rule remains load-bearing; the annotation now
  reinforces it. Do not weaken.
- First-revision TRIM of core-restating sections works: the DEFENDER-attack, Cloud-Sprite-block,
  and mulligan sections were validated DEAD/clean and compressed with no risk (the behavior is
  fixed, so the prose was pure length). Trim what the corpus proves solved; spend the bytes on
  the classes still failing (deployment/steal/race). Guide held at 80 lines / 5.9KB (down from 85
  / 6.3KB) while adding two new sections.
