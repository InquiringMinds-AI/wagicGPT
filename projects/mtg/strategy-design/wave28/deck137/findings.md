# deck137 -- Selesnya (GW) Adventures -- wave 28 (guided ROUND 6)

Binary ad8930961, corpus matchups-20260726-050449, -T 3000, REPPENALTY=1.05.
My 6 pilot logs: 1785*-ai_baka_deck137-0x*-vs-{22,18,93,146,134,136}.jsonl.

Outcomes: **3/6, 0 timeouts, 0 draws** (UP from wave-27's 2/6).
- WIN vs22  (KILL 18/0  t10) -- mono-R Giants; killed on the board (wave-27 LOST this race).
- WIN vs93  (KILL 20/-12 t12) -- Relentless Rats; Trostani Soldiers + Loxodon + fliers closed it.
- WIN vs134 (KILL 18/-8 t10) -- mono-U tron durdler; adventure bodies, killed clean.
- LOSS vs18 (KILLED -7/19 t9) -- Kithkin anthem-aggro; own life 18->2->dead, opp held 19-20.
- LOSS vs136 (KILLED 0/26 t11) -- mono-B Rankle/Knight; own life 18->9->dead, opp held ~18-20.
- LOSS vs146 (ADJ-behind 4/8 t14) -- Orzhov Dungeons; no gameend, cut at harvest while behind.

Corpus health at my seat: **0 unparsed, 0 empty, 0 retracted_choice, 0 stale_echo, 0 priority
fallback.** All 16 fallbacks are deferred_to_heuristic -- and ALL 16 are **March of the
Multitudes convoke** defers. ZERO Venerated Loxodon defers (wave-27 had 23). See section 1.

================================================================================
## (1) e1 CONVOKE-NONRESOLUTION: **PARTIALLY CLOSED.** Loxodon CLOSED + proven live; March
## remains open under the SEPARATELY-LEDGERED March-via-convoke X-announcement gap.
================================================================================

THE headline. Applied the RESOLUTION-VERIFICATION rung to every defer and every Loxodon/March
cast pick, tracing each to its downstream effect (Loxodon battlefield arrival / March token).

### 1a. Defer decomposition -- EVERY defer, shape and card
16 defers (vs134 x10, vs136 x4, vs18 x2). Decomposed:

  game   | defer seqs (t)                                | card  | shape
  -------|-----------------------------------------------|-------|---------------------------
  vs18   | 24(t6), 31(t9)                                | March | convoke tap-payment, opts=0
  vs134  | 22,23,28(t4) 31,33,36(t6) 38(t7) 41,43,44(t8) | March | convoke tap-payment, opts=0
  vs136  | 25,27(t6) 33(t8) 39(t10)                      | March | convoke tap-payment, opts=0

- (a) Shape IDENTICAL to wave-27's convoke sub-step defer: each follows an ASK chosen_text
  `Cast March of the Multitudes with its convoke cost`; the defer is the empty (opts=0,
  latency -1, no reply) tap-payment sub-step. NOT a new shape.
- Decisive change: **100% of defers are MARCH; ZERO are Loxodon.** Every Loxodon convoke pick
  either RESOLVED (vs93) or silently no-op'd with NO defer (vs146 t4). The deferred-tap-payment
  failure mode is GONE for Loxodon.

### 1b. RESOLUTION TABLE (this corpus vs wave-27)

  card                    | convoke picks | plain picks | ARRIVALS/TOKENS | wave-27
  ------------------------|---------------|-------------|-----------------|--------
  Venerated Loxodon       | 2             | 2           | **2**           | 2/23
  March of the Multitudes | 13            | 4           | **0**           | 0/17

- Loxodon: 2 arrivals / 4 attempts, 0 defers. One via CONVOKE (vs93), one via plain (vs146).
  Convoke-specific: wave-27 was 2/23 all-defer; now 1 resolved / 2 convoke picks / 0 defers.
- March: 0 tokens / 17 attempts -- unchanged from wave-27's 0/17. The 2 Soldiers in vs93 came
  from TROSTANI DISCORDANT's ETB (narration: `Trostani ... stack -> battlefield | Soldier:
  created`), NOT March. Known/ledgered March-via-convoke X-announcement gap; not re-derived.

### 1c. THE END-TO-END CONVOKE-VARIANT CASE (brief-required) -- vs93 seq35 t10, LIFE 20/11
The model is offered the CONVOKE variant and CHOOSES it (not steered to plain); it RESOLVES:
- seq34 t10: plays Plains, PLAN "cast Venerated Loxodon".
- seq35 t10: `CHOICE: 1 (Cast Venerated Loxodon with its convoke cost (4/4))` -- CONVOKE chosen.
  NO defer follows (seq36 is attackers).
- narration: `Your Venerated Loxodon: hand -> stack -> battlefield` -- it ARRIVED.
- seq37 t11: `BLOCKS: B1:A1 ... Venerated Loxodon blocks Relentless Rats` -- live body next turn.
Trace complete: convoke pick -> stack -> battlefield -> functions as a real creature.
**This proves the ENGINE FIX is doing the work, NOT the guide's prefer-plain steering** -- the
model freely picked convoke and the previously-dead path resolved.

### 1d. March: guide steering CANNOT rescue it (both variants engine-bound) -- vs18/vs136
The model DID prefer plain per the guide -- vs18 seq21, vs136 seq30/seq36 all picked
`Cast March of the Multitudes {g}{w}{w}{x}` (plain X-cast) FIRST. But plain March does not
resolve either: it cascades into a `Cast Card Normally` menu, the engine RE-OFFERS convoke, the
model flips to `with its convoke cost`, and THAT defers (vs18 21->22->23->24-defer; vs136
30->31->32->33-defer; 36->37->38->39-defer). 0 tokens every time. March's X-announcement seam is
broken independent of the convoke choice -- the prefer-plain lever that rescues Loxodon has
nothing working to steer March toward. March is fully engine-bound.
(vs93 seq28->29 took a different `convoke` sub-menu, ch=2, NO defer -- still 0 March tokens.)

### 1e. VERDICT: **CONVOKE-NONRESOLUTION PARTIALLY CLOSED.**
- Fixed-cost convoke (Venerated Loxodon): **CLOSED** -- 0 defers, resolves via convoke (vs93
  end-to-end) and plain (vs146); wave-27 deferred-tap-payment failure gone.
- X-spell convoke (March of the Multitudes): **STILL OPEN**, but this is the separately-ledgered
  March-via-convoke X-announcement gap (deeper X-modeling), NOT the general convoke-nonresolution
  the e1 fix targeted. 16 sightings; 0/17 tokens, unchanged.
- New WATCH (LOW): vs146 seq14 t4 -- Loxodon CONVOKE chosen, no defer, no arrival (board t6 =
  creatures:2, no Loxodon); re-cast plain seq27 t8 and resolved. Silent convoke no-op distinct
  from the wave-27 defer shape; may be a legitimate insufficient-convoke decline. Routed to notes.

================================================================================
## (2) BLOCKERS FLOOR ROUND 4: HELD. 4 replies short + legal + zero fallback.
================================================================================
Four blockers records (vs18 s12, vs93 s25, vs93 s37, vs146 s31); vs22/vs134/vs136 x0. All single
legal B:A assignments, all fb='', all parsed:
- vs18 s12 t3: `B1:A1` Flaxen 1/2 chumps a 2/2 (correct chump, not lethal).
- vs93 s25 t7: `B2:A1` Lovestruck 5/5 block-kills a Rat 3/3, survives.
- vs93 s37 t11: `B1:A1` convoke-Loxodon 4/4 blocks a Rat (the 1c body).
- vs146 s31 t9: `B1:A2` Loxodon 4/4 block-kills the 3/2 Silverquill, survives; takes the 6/6.
**ZERO illegal gang-piles** (wave-27 had two). Floor: HELD, clean.

================================================================================
## (3) CLOSING-SPEED RE-VERDICT (with Loxodon now a WORKING payoff)
================================================================================
3/6, THREE kills this corpus (vs22 t10, vs93 t12, vs134 t10) -- up from ONE in wave-27; the vs22
race it LOST last wave it WON here. With Loxodon resolving (pump + 4/4 body) the attack doctrine
converts more boards. Attack doctrine remains SOUND (full swings unblocked, lethal recognition,
correct holds under lethal -- section 2).

The two losses (vs18, vs136) are the SAME race-loss shape, root UNCHANGED: **March -- the go-wide
+ lifelink STABILIZE payoff -- is still dead.** vs18 own life 18->14->2->dead, opp held 19-20;
vs136 18->15->9->dead, opp held ~18-20. Could neither race nor stabilize because the lifelink-
Soldier flood the STABILIZE role depends on never materializes (0 March tokens). Half the payoff
suite now works (Loxodon), half is dead (March).

**Re-verdict:** closing speed IMPROVED and is now PARTIALLY resolved by the engine fix (Loxodon).
The residual is NOT a guide lever -- it is the March X-cast engine defect (the stabilize plan's
engine). No attack/stabilize-doctrine tweak fixes a payoff that makes zero tokens. Closing speed
at deck137 is now **ENGINE-BOUND ON MARCH ONLY** (Loxodon half closed); re-evaluate a guide
closing-lever only after March's X-cast resolves. Non-March go-wide/lifelink redundancy
(construction) is the fallback lever if the March fix is deferred -- flagged, not actioned.

================================================================================
## (4) e4 LAND-IDENTITY TAGS at the GW two-color seat: CLEAN, no noise.
================================================================================
Tags render and cause no confusion: `Plains (land: taps for {W})`, `Temple Garden (land: ...)`,
`Forest (...taps for {G})`. Model mulligans/sequences mana correctly (green+white reached every
game; no color-blindness), no prompt-noise, no misread, no tag-induced agonizing. e4: PASS.

================================================================================
## (5) PARSER PRECEDENCE / c4 hardening: no contradictions this corpus.
================================================================================
0 unparsed_reply, 0 first!=final coded-line reversals, 0 retracted_choice, 0 stale_echo (grep-
confirmed all 6 logs). All 16 fallbacks are the March convoke defer (a resolution failure, not a
parser drop). c4 gate: PASS. No verbatim contradiction to report.

================================================================================
## (6) c/d BATCH + adventure/shockland steady state
================================================================================
- d1 instance handles: steady -- `#N` renders on same-named permanents; no misbind.
- c2 ETB shockland (Temple Garden): pay-2-life menus render self-describing; correct choices
  (vs146 seq25 `lose 2 life` to enter untapped). Steady.
- Adventure bodies: Lovestruck 5/5, Faerie 1/1, Giant Killer attack/block as real bodies; zero
  "not a real body" declines. Steady.
- d4 DFC / flip-thrash: no DFC at this seat; no sighting.

================================================================================
## (7) ROTATION VERDICT: does NOT rotate (new-work now NARROW). See notes.md.
================================================================================
Comprehension converged, d1 validated, e1 Loxodon CLOSED at this seat. Remaining new-work: (a)
the March X-cast engine gap (ledgered; deck137 is the SOLE convoke seat = only observation post
for any future March fix), (b) a modest guide revision this wave (soften the now-obsolete blanket
prefer-plain steering for Loxodon). Both live, so KEEP -- but FLAG to ROSTER OWNER: once March's
X-cast resolves and is validated here, deck137's new-work well is likely exhausted (comprehension
converged + closing-speed resolved to construction) and it becomes a clean rotation candidate on
the next convoke-clean corpus.

repetition_penalty=1.05 -- sixth-corpus verdict: SAME. Coherent across 12k-13k replies, no prose
harm, no spiral. Unchanged.
