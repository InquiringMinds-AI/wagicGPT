# deck22 (Giants!, mono-R tribal ramp/midrange) - wave-27 FIRST-GUIDED validation

Corpus matchups-20260725-193608, binary ffec79fd8. Record **5/6** (up from 2/6 Step-0):
wins vs93 (20/-2 T12), vs136 (adj, 19/7 T13), vs134 (20/-2 T10), vs131 (12/-15 T13),
vs137 (17/-1 T11); loss vs18 (-7/17 T10). **146 decisions, 1 fallback (0.68%)** - the
single stale_echo decomposed below. Zero timeouts/draws/parser-contradictions.

## JOB 1 - d2 CHANGELING ANNOTATION: VALIDATED LIVE, misread DEAD

**Annotation renders verbatim** on hand/battlefield surfaces (cited):
  `Universal Automaton #1 {1} (1/1) [changeling: counts as Giant] [artifact] [changeling]`
(also carries d1 collision `#N` handles when two Automatons are in play - `#1`/`#2`.)

**The wave-26 misread died.** All 6 games KEPT the opening 7 (zero mulligans). The wave-26
2-game cost was mulliganing keepable hands / bottoming enablers citing "no other Giant" -
NONE of that recurred. Keep replies now recite the identity, e.g. vs93 s1 (verbatim):
  "Universal Automaton (a 1-mana Giant that enables Blind-Spot Giants...) ... Even without a
   Giant in hand, the Automaton counts as one, allowing future Giants to attack immediately."
No bottom decision arose this corpus (all keeps at 7) - the bottom surface was not exercised,
but the annotation IS present on the hand render that feeds a bottom prompt.

**Which lever moved it - BOTH, and the annotation is provably READ.** The guide (rule #1) and
the render annotation both ship. The model usually cites the guide's identity reasoning, but
the cleanest annotation signal is vs131 s3: the model copied `[changeling: counts as Giant]`
STRAIGHT out of the render into its CHOICE line - direct proof the annotation is consumed.
Double-teach converged; the fix is validated.

## JOB 3 - s3 turn-1 ask fallback decomposition (vs131 s3, the 1 non-defer)

- kind=ask, turn 1, Main phase 1 CAST decision; options: 1.Cast Universal Automaton  2.Cast
  Mogg Sentry  3.Cast nothing. latency 16.5s. choice=-1, fallback=**stale_echo**.
- The model chose CORRECTLY (option 1, Universal Automaton). Reply first line:
  `CHOICE: 1 (Cast Universal Automaton {1} (1/1) [changeling: counts as Giant])`
- **Why unparsed:** the model appended the render annotation `[changeling: counts as Giant]`
  to the echoed option NAME. The canonical option text is `Cast Universal Automaton {1} (1/1)`
  (no annotation). The c4 name-anchored answer check could not reconcile number(1)+name(with
  extra bracket) -> safe-direction fallback to heuristic (Baka), classified stale_echo.
- **Correlation is 1:1**: across all 146 decisions this is the ONLY reply whose CHOICE line
  echoed `[changeling`, and it is the ONLY fallback. Mechanism confirmed.
- **Harmful? NO.** Safe-direction: Baka cast the obvious turn-1 Automaton; deck22 won the game.
- This is a NEW d2<->c4 interaction (routed to notes.md ledger): an in-band representation
  annotation gets echoed by the model into its structured answer, tripping name-match parsing.

## JOB 2 - guide validation (2/6 -> 5/6)

1. **Attack-floor doctrine VALIDATED - the ~12-damage-left-home class is GONE.** Giants swung
   on curve; full alpha strikes closed every win: vs134 s20 T10 (Automaton+2xBlind-Spot+
   Skyraker, 20 life), vs93 s18/s20 (Calamity-doubled alpha), vs137 s19/s21, vs131 s22/s25
   (6-attacker lethal). No durdling 4/3+ bodies behind 1/1 chip.
2. **Mogg Sentry role - MOSTLY compliant, deviations all value-positive (no harmful attack).**
   Kept home as blocker: vs131 s7, vs137 s10, vs18 s8/s17, vs136 s9. Attacked only in
   value-positive spots: empty board (vs93 s10 into 0-creature Rats = free damage), ONLY legal
   attacker (vs137 s9 - offered list was "A1. Mogg Sentry" alone), lethal alpha (vs131 s25).
   The guide's flat prohibition is slightly over-strict vs the model's (correct) deviations;
   LOW-priority optional refinement noted, guide FROZEN this wave.
3. **Automaton identity fully internalized** - turn-1 casts every game; mulligan keeps cite it.

## JOB 4 - c1/c3 steady state (clean)

- c1 target enumeration: Inferno Titan 3-way face split fully enumerated + picked, no
  truncation (vs136 s12-14, s20-22, s25-27, s39-41 - all "The opponent (player, life N)").
- c3 pump renders: firebreathing delta+result clean AND stacks correctly across successive
  pumps: vs136 s28->s29 `+1/+0 until EOT (6/6 -> 7/6)` then `(7/6 -> 8/6)`; s35->s37 up to
  `(8/6 -> 9/6)`. No stat-conflation.

## The one loss (vs18) - matchup, not misplay

Kithkin mono-W anthem go-wide outraced Giants' ramp: Cenn's Heir grew 1/1->3/3->4/4->5/5,
Field Marshal (lord) + Goldmeadow Dodger pumped the team. The Blind-Spot Giant was used to
block/kill Field Marshal (s12) - killing the anthem lord is a defensible defensive line, not
passivity. Giants come online T5+ and Kithkin is faster; a legit aggro-race loss.

## Oracle cross-check (load-bearing, Scryfall)
Universal Automaton {1} 1/1 Changeling; Calamity Bearer doubles Giant-source damage; Sunrise
Sovereign +2/+2 trample - all match engine primitives. No card-script divergence.
