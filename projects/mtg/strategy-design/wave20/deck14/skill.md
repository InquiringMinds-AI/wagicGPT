# deck14 (Deep Blue) — skill.md — proposals for the strategy-writing skill

Third appearance (revised-guide round 2). All items are guide-authoring METHOD; none touches core.
The value this wave is a clean SECOND-round validation: a wave-19 revision that was written as a
two-tier aggression rule can now be scored for whether tier-2 actually landed.

## PROPOSAL 1 (VALIDATES the wave-19 "two-tier aggression countermand" method headline — promote from
## "written" to "landed on the next corpus") — a TIER-2 clock-quality rule (deploy the finisher / swing
## the whole board) DOES cure the slow-clock soft-durdle when the deck draws its material.
Wave-19's headline method was: write an aggression countermand in TWO tiers, because a model that learns
to satisfy the literal attack-floor satisfies ONLY the floor (durdle mutates from "don't attack" to
"attack trivially"). deck14's wave-19 revision added Rule #1B ("deploy your flyers; swing the whole
board; race the mana to the finisher"). Wave-20 measures it: **finisher deployment 1/6 -> 4/6, all three
wins are finisher games, and the finisher is now cast the turn it becomes available** (no sit-on-the-flyer
behavior). METHOD CONFIRMED: the tier-2 rule is not just theoretically right, it moves the behavior on the
next corpus — a first-guide tier-1 fix that reveals a tier-2 failure is correctly closed by a tier-2 rule
authored at the altitude of the WIN CONDITION. The residual non-deployment (2/6) attributes to DRAW/mana
STARVATION (vs135 never drew a flyer), not to the rule — which is the DISPLACEMENT distinction, not a rule
failure. Generalizes: after shipping a tier-2 clock-quality rule, score deployment RATE of the named
threat and split "not deployed because not drawn" (displacement) from "not deployed while castable"
(the rule failing).

## PROPOSAL 2 (NEW rung, STRONG) — a PRESENT-AND-DISOBEYED rules-misconception that recurs and ESCALATES
## across waves is the signal to HOIST + CONCRETIZE the existing buried line into a named TRAP, even in
## turnaround mode — because the passive one-liner is demonstrably not landing and it is the seat's #1 leak.
deck14 carries the line "Bounce can only return a PERMANENT on the battlefield; it cannot answer a spell on
the stack" — buried at the end of a bullet, phrased passively. It went from 1 instance (wave-19) to 1
costly misplay + 3 fallbacks (wave-20), the seat's single biggest model-fixable error cluster. The standard
doctrine ("present-and-disobeyed -> representation/engine, do not restate louder") is right about the
DURABLE fix, but this wave sharpens the INTERIM: when the disobeyed line is (a) buried, (b) passive, and (c)
the deck's #1 leak, a turnaround-mode constrained diff that HOISTS it to a named trap ("BOUNCE-ON-THE-STACK
IS YOUR #1 MISFIRE") with the UPSTREAM behavior ("do not cast bounce to answer a stack spell") AND the
DOWNSTREAM menu fact ("the target menu is battlefield-only; do not thrash; never self-bounce to reach the
stack") is warranted — it is not a "third copy of a well-placed tell," it is the FIRST prominent, actionable
placement. The test for hoist-vs-leave-to-representation: is the current line prominent+concrete+obeyed-
sometimes (leave it, fix representation) or buried+passive+never-landing (hoist it once, still flag the
representation fix as durable)?

## PROPOSAL 3 (NEW rung, method) — a truncation-unparse that NEVER reaches a coded line is a DISTINCT
## failure mode from format-DRIFT, and a "combat format-drift class DIED" verdict must be RE-TESTED under
## the two triggers that produce it, or the death claim will not hold next wave.
Wave-19 declared deck14's combat format-drift class dead (replies collapsed to 300-800 chars). Wave-20 it
partially returned — but as a DIFFERENT mechanism: not card-names-instead-of-codes (format drift, which
terminates), but TOKEN-CAP TRUNCATION (the reply exhausts the cap mid-derivation and emits NO final line,
coded or otherwise). It fires under exactly two triggers: (a) an IMPOSSIBLE-TARGET confusion (the model
thrashes hunting for something not in the list — the bounce-on-stack menu), and (b) a HOPELESS/complex board
(the model exhaustively enumerates every block scenario). METHOD: when declaring a combat-reply-length class
"dead," do not score it only on the median; check the TAIL of the distribution under those two triggers, and
bucket any residual by TERMINATION (drifted-but-terminated = format class; truncated-before-any-line =
decode/token-cap class, a different owner). The salvage extension (BLOCKS/ATTACK/PUT) cannot touch the
truncated-before-line class — salvage needs a coded line to salvage — so a "salvage shipped" fix does not
close it. Prose-intent salvage (recover the decision stated in the reasoning prose) is the only reply-level
net for it.

## PROPOSAL 4 (reinforcement of the wave-19 reviewer-discipline note — the observability item RESOLVED, so
## record the resolution) — verifying the focus surface is logged is a repeated-across-waves check, and this
## wave the gap it flagged CLOSED, which is itself worth recording so the next reviewer does not re-flag it.
Wave-19 (deck14 Proposal 4) flagged that every combat record at this seat logged EMPTY `options_text`,
defeating the TRADE-OUTCOME-annotation validation. Wave-20: 40/40 combat records carry full options_text WITH
the trade annotations. METHOD note: an observability gap flagged as a coverage HOLE (not a clean pass) gives
the harness a concrete field to fix, and the NEXT wave should confirm-and-retire it — closing the loop
prevents the perennial re-flagging that the skill warns about elsewhere. Recorded closed.

## Convergence summary
- VALIDATED on second corpus (was NEW method wave-19): two-tier aggression countermand — tier-2 clock-quality
  rule lands (finisher deployment 1/6->4/6, all wins are finisher games). Split non-deployment into
  displacement (not drawn) vs rule-failure (castable, not cast).
- NEW rung (Proposal 2): hoist-a-buried-passive-#1-leak-line into a named trap is a legitimate turnaround-mode
  diff, distinct from the forbidden "third copy of a well-placed tell."
- NEW rung (Proposal 3): truncated-before-line != format-drift; re-test a "dead" combat-length class under the
  impossible-target and hopeless-board triggers; salvage cannot reach truncated-before-line.
- CLOSED (Proposal 4): the wave-19 combat-options_text observability gap is fixed; recorded so it is not
  re-flagged.
