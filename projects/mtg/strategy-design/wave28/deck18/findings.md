# deck18 (Kithkin, mono-W tribal anthem aggro) - wave-28 6/6 + ROTATION

Corpus: matchups-20260726-050449, binary ad8930961, -T 3000, REPPENALTY=1.05.
deck18 record 6/6 (UP from 5/6 wave-27 first-guided; 4/6 Step-0 wave-26). Guide under audit
= wave-26 initial deploy, BYTE-FROZEN (deck18_strategy.txt cmp-identical to wave26 == wave27
== deployed; re-verified this wave).

- WINS (all 6): vs134 (t10, 20v-3), vs22 (t18, 16v0), vs136 (t12, 19v-5), vs146 (t8, 20v-7),
  vs137 (t9, 19v-7), vs93 (t12, 17v0). No adj at deck18's seat; 6/6 clean.
- Fallbacks at this seat: ZERO markers in own-seat files (kind census: 75 ask + 32 attackers
  + 7 priority + 7 blockers, 0 defer/retract/unparsed/retry). Consistent with the corpus's
  18 fallbacks living at 137/134/146.
- This IS the deck59 exit shape: 6/6 validation corpus on a frozen guide, all targeted leaks
  holding, new signal routes OFF the deck guide. ROTATION applied with finality (notes.md).

A 6/6 record on a frozen guide still gets audited. The wins are correct, but they HIDE a
worsened latency tail and one genuine decision/parse slippage. Decomposed below.

## JOB 1 - THE 6/6-HIDES-TAXES AUDIT (the deck59 exit shape, honestly)

### Reply-length profile (ask-only, comparable to wave-27's Leak-3 A/B table)

  metric      wave-27 (guided)   wave-28 (this)     delta
  N (ask)     127                75                 (fewer/shorter games - 6 fast wins)
  p50         943                943                flat
  p95         7,112              7,411              +4%
  max         12,915             14,600             +13%  NEW HIGH
  mean        1,909              1,892              flat
  >7,000      9 (7.1%)           6 (8.0%)           ~flat rate
  >10,000     3 (2.4%)           4 (5.3%)           rate ~2.2x

All decision kinds (incl. combat): N=121, p50=943, p90=3,935, p95=11,678, max=14,600,
mean=1,959; >7k=9, >10k=7, >12k=3.

VERDICT: the body of the distribution is stable (p50/mean flat - the guide's stat-re-derivation
fix HOLDS: no "recompute Field Marshal from base 2/2 + each lord" P/T spiral dominates the top
this corpus). But the TAIL WORSENED - a new high (14,600) and the >10k rate roughly doubled.
Six decisions ran 174,687-218,599 ms (2.9-3.6 MINUTES) of wall-clock each. This is the tax
the perfect record conceals.

### What the wins hide - three findings

1. LATENCY TAIL, escalated. The top-6 spikes are all near the -T 3000 token ceiling
   (~11.7k-14.6k chars = ~3000 tokens at 4-5 char/tok) and each cost >170s:
     14,600  vs146 seq13 [ask]      lat 203s  Ballynock-vs-Mobilization at opp-6 (way ahead)
     12,523  vs93  seq21 [priority] lat 196s
     12,363  vs93  seq20 [blockers] lat 201s  (the slippage - see #2)
     11,879  vs93  seq17 [ask]      lat 175s  Mobilization-vs-nothing at opp-6
     11,751  vs22  seq22 [priority] lat 186s
     11,734  vs137 seq14 [ask]      lat 188s  Armored Ascension target (genuinely complex)
     11,678  vs134 seq11 [ask]      lat 219s  Field Marshal First-Strike interaction reasoning
   The re-derivation class the trust-line killed is NOT back at the top. Two of these are
   defensible (vs137 seq14 lethal/evasion target math; vs134 seq11 FS-granting interaction).
   The other four are LOW-STAKES-WHILE-AHEAD planning loops - the model writes a full
   multi-turn lethal plan on a 2-option "cast X or nothing" prompt while the opponent is at
   3-6 life and the game is already decided.

2. ONE GENUINE SLIPPAGE, masked by the win: vs93 seq20 blockers (12,363 chars, 201s).
   At 17 life vs opp 3, a single attacker (Relentless Rats 4/4; "your blocker dies, attacker
   lives"). The reply emits ONE early coded line `BLOCKS: B1:A1` (Soldier blocks Rats), then
   loops "So I should NOT block ... blocking is strictly worse (I lose a card, no benefit) ...
   So I should NOT block" and RUNS OUT of tokens mid-loop - the reply ends
   "...If I don't block, I have 13 life." with NO closing CHOICE:/BLOCKS: terminator. The
   parser fell back to the stale early `BLOCKS: B1:A1` line -> the model BLOCKED and lost the
   Soldier for nothing, which CONTRADICTS its own repeated conclusion. Immaterial to the
   result (lethal was locked next turn either way, and it won t12), but a real
   decision-quality-AND-parse defect that the 6/6 record hides. This is L-18-1 escalated from
   "verbose" to "non-terminating -> token-ceiling truncation -> stale-line fallback." Routed to
   notes.md as N-18e (HARNESS) + L-18-1 update (MODEL).

3. L-18-1 BROADENED. Wave-27 it was trivial LAND DROPS (vs137 seq12 "Play Plains" 7,112;
   vs136 seq17 5,844). Wave-28 the same tax now fires on low-stakes CAST and BLOCK decisions
   while ahead, at 11-14.6k chars, bumping the token ceiling. The shape (plan the whole
   game-winning sequence on a forced/near-forced choice) is unchanged; the magnitude and the
   decision surface both grew. Still MODEL-layer, latency-not-correctness, cross-seat - NOT a
   deck18 guide-mod.

## JOB 2 - STEADY-STATE CHECKS (all still holding on the frozen guide)

### Dodger evasion (LEAK 1): VALIDATED again - 9/9
Every attacker-phase decision where Goldmeadow Dodger was on board and untapped, it attacked:
vs22 seq6/15/18, vs136 seq6/9, vs146 seq6/9/12/14. Never present-but-held. The "SWING IT
EVERY TURN - a big blocker cannot block it" order fires on the frozen guide. The attackers
kind is well-behaved overall (N=32, p50=912, p95=2,272, max=2,769) - NO impossible-block
spiral anywhere (wave-26's 14k signature leak remains gone).

### Flyer-block off-case (LEAK 2): VALIDATED - off-case exercised, clean
Only one flyer/reach attacker faced all corpus: vs22 seq24 (Skyraker Giant 4/3 [reach]).
Model chose "no blockers" (5,557 chars, but correct - reasoned "It can block flyers. I have
no flyers ... no benefit to blocking ... opponent at 2 ... do not block"). ZERO illegal
ground-block-of-flyer attempts. The guide's ground-only RACE line holds. (The 5.5k tax on a
clearly-declined block while at 20 life vs opp 2 is the same L-18-1 planning tax, not a
mechanics error.)

### Trust-line (LEAK 3): HOLDING - no P/T re-derivation spiral at the top
p50/mean flat; the "recompute the lord stack from base each time" class that dominated
wave-26's top spikes is not present. The surviving tail spikes are planning loops, not stat
re-derivation. The "(printed X/Y) - trust it" line still does its job.

## JOB 3 - e4 LAND-TAG NOISE at a mono-white seat: ZERO cost
`Plains (land: taps for {W})` rendered 116x across the 6 games (the ONLY land tag - mono-W,
never any other color/ambiguity). Model NEVER echoed or reasoned about "taps for" in ANY of
121 replies (0/121). No verbosity, no confusion, no prompt-noise from the tag at this seat.
Clean pass for e4. (The one land-counting wobble - vs136 seq11 "I have 3 Plains, so I'm
tapped out?" - is the model confusing land-drop-count vs mana-tapped, NOT tag-induced.)

## JOB 4 - PARSER CONTRADICTIONS (verbatim) + e-batch steady state
- No reply had multiple DISTINCT `BLOCKS:` lines this corpus (the wave-27 vs137 seq11
  self-correction/precedence class did not recur). The ONLY parser event is vs93 seq20
  (finding #2): a SINGLE stale `BLOCKS: B1:A1` line surviving an unterminated reply -
  truncation, not precedence. Verbatim: reply contains one `BLOCKS: B1:A1`, then the prose
  loop ".../ So I should NOT block. / ... / blocking is strictly worse ... / So I should NOT
  block. / Wait, if I don't block, the Rats deals 4 damage. / ... / If I don't block, I have
  13 life.\n" [END - no terminator]. Parsed choice=1 "Soldier blocks Relentless Rats".
- e1 (convoke): N/A (no convoke in mono-W Kithkin). e2 (phantom chooser): N/A (no
  Legion's End). e3 (flip-thrash): N/A (no DFCs; adventure cards present but no flip menu).
  e5 (annotation-echo): no stale_echo shape at this seat (0 fallback markers). e4 covered
  above. e-batch steady state at deck18: CLEAN.

## SUMMARY
Frozen guide, 6/6, all three targeted leaks holding, e4 clean, 0 fallback markers. The audit
finds NO guide-fixable defect - the perfect record is legitimate at the decision-CORRECTNESS
level (5 of 6 tail spikes are correct decisions; the 6th, vs93 seq20, is immaterial to the
result). What the wins hide is (a) a worsened latency tail bumping the token ceiling and (b)
one truncation->stale-line slippage. BOTH route OFF the deck guide (MODEL/HARNESS layers).
This is exactly the deck59 exit shape -> ROTATE (notes.md).
