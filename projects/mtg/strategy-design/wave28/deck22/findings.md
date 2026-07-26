# deck22 (Giants!, mono-R tribal ramp/midrange) - wave-28 validation + ROTATION

Corpus matchups-20260726-050449, binary ad8930961, -T 3000, REPPENALTY=1.05. Guide FROZEN
byte-for-byte (cmp-verified identical to deployed bin/Res/ai/baka/deck22_strategy.txt AND to
wave-27 strategy.txt - frozen two consecutive corpora). Record **3/6** (down from 5/6):
W vs134, W vs136 (adj), W vs146; L vs18, L vs93, L vs137. **~124 decisions, ZERO fallbacks
(0.00%)**, zero timeouts/draws, zero parser-contradictions, zero parser anomalies (0 empty
chosen_text, 0 choice=null). No "Flip Side" pseudo-action sighting.

## JOB 1 - e5 ECHO-TOLERANCE: VERIFIED LIVE. The wave-27 stale_echo shape is STRUCTURALLY GONE.

The wave-27 R-ANNOTATION-ECHO-PARSE fallback (model echoes an in-band render annotation into
its CHOICE line -> name-match fails -> stale_echo -> heuristic) is **discharged by e5**.

- Annotation-echoing REPLIES this corpus: **5** (UP from 1 in wave-27 - the model echoes
  annotations MORE now, so the tolerance is genuinely exercised, not merely un-triggered):
  - vs134 s3: `CHOICE: 1 (Cast Universal Automaton {1} (1/1) [changeling: counts as Giant])`
  - vs18  s3: `CHOICE: 2 (Cast Universal Automaton {1} (1/1) [changeling: counts as Giant])`
  - vs93  s3: `CHOICE: 2 (Cast Universal Automaton {1} (1/1) [changeling: counts as Giant])`
  - vs136 s3: `CHOICE: 2 (Cast Universal Automaton {1} (1/1) [changeling: counts as Giant])`
  - vs136 s11: `CHOICE: 2 (Mountain [land] [your hand] - "R")` - a NEW annotation FORM
    (`[land] [your hand]`, not just `[changeling...]`) echoed into the answer.
- **All 5 parsed CLEAN** - `choice` equals the echoed option number in every case, ZERO
  fallbacks at the deck22 seat. The extra `[...]` tail no longer trips number+name
  reconciliation. The e5 tolerance holds across BOTH the `[changeling: counts as Giant]`
  shape that produced the wave-27 bug AND the previously-unseen `[land] [your hand]` shape.
- Verdict: R-ANNOTATION-ECHO-PARSE is CLOSED. The representation-lever <-> parser coupling
  that wave-27 surfaced is fixed and the fix generalizes past the single shape that motivated
  it. Zero engine-layer new-work remains at this seat.

## JOB 2 - guide steady state on the frozen guide: NO REGRESSION (2nd validation)

All three teaches held again, byte-identical guide:
1. **Automaton-is-a-Giant identity - fully internalized.** Cast turn 1-2 in ALL 6 games
   (vs134 s3, vs18 s3, vs93 s3, vs136 s3, vs137 s3; vs146 s5 - cast the turn it was drawn
   after a no-Automaton mulligan). Declared as an attacker every combat it was offered (the
   1/1 Giant runs the attack floor: vs134 s5/s8/s11/s14, vs93 s7/s11/s13/s16/s19/s22/s29,
   vs146 s10/s13/s17/s24). Mulligan keeps still recite the identity (vs93 s1 verbatim: "the
   critical combo piece: Universal Automaton ... is a Giant (due to Changeling)").
2. **Attack-floor doctrine held - no ~12-damage-left-home passivity.** Every win closed with
   full alpha strikes (vs134 s14 Automaton+Blind-Spot+Calamity T8; vs146 s24 T10 into opp-5;
   vs136 s23 Blind-Spot+Stinkdrinker). The "no attackers" calls this corpus were ALL correct
   floor-exceptions: vs137 s9/s12/s16 offered only Mogg Sentry / Stinkdrinker (1/3) blockers
   (guide EXC 1+2, held to block a losing race); vs18 s11/s16/s19/s22 offered only 1/1s /
   Stinkdrinker into a faster anthem race; vs146 s7 a lone summoning-relevant 1/1 pre-curve.
   No 4/3+ Giant was ever left home while chipping with a 1/1.
3. **Mogg Sentry role - compliant, one value-positive deviation (same class as wave-27).**
   Held home as a blocker: vs18 s20/s23, vs93 s20, vs137 s17, vs136. Attacked ONCE (vs93 s22,
   offered [Automaton, Mogg]): the model reasoned it explicitly - beatdown at 13-13 vs three
   4/4 Rats, opponent has no spell up so Mogg won't be needed to grow-block, a 4/4-for-1/1
   trade favors deck22, "I cannot afford to be passive." A correct race-positive deviation,
   NOT a misplay - identical to the wave-27 value-positive Mogg deviations. The guide's flat
   prohibition remains marginally over-strict but NON-BINDING (model deviates correctly
   without it).

## JOB 3 - 5/6 -> 3/6 DECOMPOSITION (honest, game by game)

Pool changed: deck131 (a wave-27 WIN) ROTATED OUT; deck146 debuted (a wave-28 WIN). That
swap is W->W = neutral. The two real drops are both W->L, both fully explained by
OPPONENT STRENGTHENING + deck22 draw variance - NOT deck22 decision regression:

- **vs93 W->L (deck22 -25 life, T15). Pool shift: 93 rose 2/6 -> 4/6.** deck22 KEPT a correct
  guide hand (3 Mountains + Automaton + 3 Mogg Sentry - 2+ lands, an early play) that drew
  the wrong half (four 1/1s, Giants arrived late). It aggressed correctly the whole game and
  fired Inferno Titan 3-at-face (s26-28, opp 12->9). Final board: lone Inferno Titan (6/6)
  vs SIX Relentless Rats each pumped to 7/7 (printed 2/2). A go-wide swarm that mono-R
  midrange cannot outscale once it hits 6 rats. Matchup + variance; deck22 still dragged opp
  to 9. No misplay.
- **vs137 W->L (deck22 dead T10, life 0). This is the e1 convoke fix landing on the
  opponent.** With convoke now resolving (wave-27 headline: 137's payoffs were dead behind a
  0/17-resolution defer), deck137 curved out and closed with EVASION: final board = 3x Faerie
  Guidemother (2/2 FLYING, +1/+1 countered) + Edgewall Innkeeper attacking. deck22's board was
  all-ground (Mogg, 3x Stinkdrinker 1/3, Sunrise Sovereign 5/5) with NO flyer/reach answer -
  6 flying damage over 6 life = dead. deck22 drew a flood of enablers/blockers + one beater.
  Its "no attackers" and block calls were all correct (nothing on the ground mattered; racing
  a 5/5 into opp-18 with 3 flyers inbound changes nothing). Pure matchup loss - the opponent
  got stronger from an ENGINE fix, deck22 did not get worse.

**Net: 5->3 is entirely pool-strengthening (93 +2/6; 137 convoke-enabled) + draw variance.
Both losses are correct-play losses. No decision class regressed.**

## JOB 4 - e4 land-tag noise: CLEAN

Land identity tags (`Mountain (land: taps for {R})`) render on hand/battlefield. Only 2
reply-mentions across the corpus (vs18 s25, vs136 s20) and both are benign - the model quoting
its own hand line while reasoning about contents. Zero confusion, zero misparse, zero decision
distortion. deck136 s11 even ECHOED the `[land] [your hand]` tag into a CHOICE line and it
parsed clean (see JOB 1). Tags are pure signal at this seat.

## JOB 5 - parser contradictions: NONE

Zero fallbacks, zero choice=-1, zero empty chosen_text, zero null-choice records across ~124
decisions. No render/answer contradiction. No phantom-chooser (that class is deck136's Legion's
End, not present here). No reasoning-tax sink from an illegal/contradictory ask.

## Oracle cross-check (load-bearing)
No card-script divergence exercised this corpus. Universal Automaton {1} 1/1 Changeling,
Relentless Rats (+other-Rats pump), Faerie Guidemother // Gift of the Fae adventure - all
render per Oracle. No suspected engine-divergence to route to verify-first.
