# deck102 (Tergrid, discard/sacrifice) — findings (wave 21, FIRST GUIDED corpus / Step 0-ter)

Corpus: `matchups-20260723-084938`, binary `3be7f5a67`, 6 games round-robin (vs 27/14/49/35/62/135).
Record **3/6** (up from 1/6 guideless) — wins vs49, vs35, and vs135(life-adjudicated at cap);
losses vs27, vs14(life-adjudicated at cap), vs62. **0 fallbacks** (down from 6 in wave-20 — the biggest
single-metric win at this seat). This is the initial guide's first live validation. Headline: the guide
WORKS on its three primary axes (deploy/attack floor, symmetric-trap gate, pick-ONE) and the wave-21
engine batch (answer-first + reveal-framing + mana-count integrity) fired cleanly at this seat.

Outcomes decomposed (opp life is the deploy/attack-floor signal):

| vs  | result       | end turn | my/opp life | opp life trajectory        | read |
|-----|--------------|----------|-------------|----------------------------|------|
| 49  | WIN          | 21       | 31 / -3     | 20->18->16->12->10->6->-3  | attacked every turn t11+, killed them |
| 35  | WIN          | 13       | 11 / -5     | 20->16->12->8->4->-5       | Tourach clock every turn t5+, closed |
| 135 | WIN (adj)    | cap 13   | 18 / 5      | 20->18->13->5              | ahead at cap; steady clock, no durdle |
| 14  | loss (adj)   | cap ~14  | 4 / 14      | 20->18->16->14             | attacked but only with a 2/2 Myr; raced faster than it clocked |
| 27  | loss         | 13       | -1 / 19     | stuck at 19                | the one residual durdle — never kept a creature alive; painland self-death |
| 62  | loss         | 8        | 0 / 20      | stuck at 20                | fast race; sole 2/1 attacker suicided into a 3/3; developed too slow |

The wave-20 durdle pattern (opp parked at 19-20 in nearly every loss) is BROKEN: opp life reached lethal
in two games and single digits in a third. The two "opp stuck" games (27, 62) are the residuals, and
neither is a clean attack-FLOOR failure (see section 3).

---

## Section 1 — REVEAL PICK-ONE (owned engine validation, item e): CLEAN PASS, both records.

Two reveal records, both Thoughtseize resolving. The wave-21 reveal-framing fix (item e) is VALIDATED at
this seat, and the wave-20 over-selection regression is GONE.

**vs27 s10 (t5, L16/20):** revealed hand = Glen Elendra Liege, Oona's Gatewarden (2 cards). Prompt now
renders: "The opponent revealed their hand (2 cards). Choose the ONE card to send to 'choose discards' -
that is the card they discard; every other card stays in their hand." ... "On the FIRST line write PUT:
followed by the ONE card number you choose". Model: `PUT: 1 (Glen Elendra Liege)` — exactly ONE pick.
Optimal strip (anthem-lord flyer buffing their two creatures, over a 2/1 defender). Events confirm
`Opponent's Glen Elendra Liege: hand -> graveyard`, Gatewarden returned to hand. Named card left the hand
exactly once. Mechanically clean.

**vs14 s7 (t3, L18/20):** revealed hand = Island, Boomerang, Unsummon, Unsummon, Island (5 cards). Same
new framing plus the ELIGIBILITY block marking both Islands `[does NOT qualify - goes to "look"]`. Model:
`PUT: 2 (Boomerang)` — exactly ONE pick. Optimal strip: Boomerang bounces ANY permanent (can hit my mana
rocks/lands) vs Unsummon which only returns a creature and I had none. Events confirm `Opponent's
Boomerang: hand -> graveyard`, the two Unsummons + Islands returned to hand. Clean.

Direct answers to the wave-20 exit question and the E1 ledger item:
- Framing renders "The opponent revealed their hand ... choose the ONE card": YES (both).
- Whose-hand mislabel ("top N cards of your library") gone: YES — no longer present.
- Model picked exactly one (no over-selection; wave-20 was 2/3 over-picks): YES, 0/2 over-picks.
  vs14 is the SAME hand shape that over-picked BOTH Boomerangs in wave-20 — now a single clean pick.
- Best strip: YES (both optimal).
- Named card left the opponent's hand (events): YES (both, exactly once).

Consequence for the GUIDE: the strategy.txt Thoughtseize paragraph describes a prompt that no longer
exists (it explains the "top N cards of your library" mislabel and warns "only the FIRST card you name is
discarded ... do NOT list several"). Representation is fixed -> the interim crutch is now STALE and should
be retired (see strategy.txt edit 1).

---

## Section 2 — CAST-STALL LOOP RESIDUAL (item i / wave-20 E2, residual (c)): ABSENT. Fix confirmed.

Scanned all ask/priority chosen casts across all 6 games for any repeat-pick of the same cast across
consecutive asks with mana left floating. NONE found. Every "Cast X" is distinct and progresses; there is
no consecutive duplicate cast anywhere in the corpus.

The two wave-20 stall instances are directly refuted:
- vs62 (the wave-20 "Cast Dreadhorde Invasion x3" game): this corpus casts Dreadhorde Invasion ONCE
  (s6, t5), then moves to Tinybones (s10). No repeat, no float.
- No "Cast Witch of the Moors"-class loop anywhere (Witch not drawn this corpus, but the mechanism is the
  same — no cast re-offered after a completed/attempted cast).

`"Already in pool"` appears 0-3x per game but never coincides with a repeat cast — it is the benign
multi-source-tap render within a single cast sequence, not the stall signature. The item-8 counting fix
(a) + plan-abort (b) shipped in `3be7f5a67` are confirmed working at this seat; residual (c) did not fire.

---

## Section 3 — GUIDE VALIDATION (Step 0-ter, the wave-20 exit questions).

### (a) Symmetric-trap fire-gate stopped the Pox/Smallpox self-harm: LARGELY YES (1 marginal cast).
Wave-20 fired symmetric mass-effects while behind in 3+ games (Pox at 6 life into aggro, etc.). This
corpus has exactly ONE symmetric cast: vs27 s14 Smallpox at L10/20. Judgment: literally behind on life
(10 vs 20) AND being raced, which the guide's hard "NEVER while behind/raced" forbids — BUT the model's
board had 0 creatures, which satisfies the guide's clause-(c) exception ("your OWN board is near-empty so
you lose little"), and the model cited exactly that. So the gate did its job of suppressing the flagrant
self-immolation; this one cast landed in the genuinely-ambiguous near-empty case.

The residual flaw is Magic-nuance the guide doesn't yet carry: a symmetric sac-a-creature is the
OPPONENT's choice — vs27 the opp would sacrifice one of two Oona's Gatewarden defenders (their worst
creature) and keep both evasive attackers, so the Smallpox did NOT slow the clock the model expected it
to. Net it traded my land + 1 life + a card to make them pitch a non-attacker. Small leak, one game, but
it points to a precise clause-(c) sharpening (strategy.txt edit 2). Overall the gate is a clear win.

### (b) Deploy/attack floor moved opponent life: STRONGLY YES.
The record's 1/6->3/6 swing is this rule working. Opp life reached lethal in vs49 (->-3) and vs35 (->-5)
and 5 in vs135 — in wave-20 opp life sat at 19-20 in every loss. Every `attackers` decision in the corpus
DECLARED attackers; there is no wave-20-style "no attackers" at parity with creatures up. Threat
deployment was real: Ravenous Chupacabra, Crypt Ghast, Syr Konrad (5/4), Korlash (5/5), Sangromancer,
Hypnotic Specter, Whip-reanimated bodies, Dreadhorde zombie army. The two "opp stuck" losses are NOT
attack-floor failures:
- vs27: the model had `creatures: 0` on board for most of the game (its t1 Elvish Doomsayer died early; it
  durdled t5-9 on Thoughtseize/Smallpox/Commander's Sphere/Liliana's Triumph with no creature in hand —
  hand was Smallpox/Sphere/Memory Jar/Grave Pact) and only deployed Korlash at t13. A DEPLOYMENT-timing /
  draw problem (no early threat to keep alive), not a failure to declare attacks. Also self-killed via
  painland (section 4 / notes).
- vs62: an 8-turn fast loss. Its ONE attack (Tourach 2/1, t5) went into an untapped 3/3 Yavimaya
  Enchantress that blocked and killed Tourach for zero damage. Over-aggression edge case (see WATCH-1),
  then raced 20->0 in three turns. Developed too slowly (Tourach + Dreadhorde + Tinybones = no real clock)
  with non-Swamp utility lands (Geier Reach Sanitarium, Strip Mine) slowing black mana; partly a draw.

### (c) Thoughtseize pick-ONE stopped over-selection: YES — see section 1 (0/2 over-picks).

### (d) Any game DREW Tergrid and exercised the steal engine: DREW but NOT exercised.
vs27 turn 14: `Your Tergrid, God of Fright: library -> hand` — Tergrid was finally drawn. But the game had
already ended at turn 13 (my_life -1, adjudged loss) — Tergrid arrived one draw step too late and was
never cast. So the steal engine remains unexercised across two corpora (variance: 1-of in a 100-card
singleton). The `TERGRID IS THE ENGINE` guide block is still un-live-tested; keep it (it costs nothing and
is correct), and note a probe-deck run is the only reliable way to validate the steal (notes.md).

---

## Section 4 — ANSWER-FIRST + parser (every-reviewer validation): CLEAN PASS.
Sampled the p95+ replies. All of the longest replies lead with the coded line:
- s22 priority 14,310 chars -> `CHOICE: 0 (pass)` first
- s12 ask 14,243 chars -> `CHOICE: 4 (Cast nothing right now)` first
- s11 ask 13,867 chars -> `CHOICE: 1 (Cast Blackblade Reforged)` first
The wave-20 fallback class at this seat was 6/6 "behind/stalled >13k-char truncations" that never emitted
a parseable line. This corpus has the SAME giant deliberations at the SAME behind/stalled spots (the tax
persists — long replies still occur) but 0 fallbacks, because the answer now comes first and the
truncation lands after a complete coded line. Answer-first is the mechanism that closed the fallback class
here. No answer-QUALITY regression observed from committing before reasoning (the reasoning that follows
matches the choice in every sampled record). No `retracted_choice` at this seat (0 fallbacks), so nothing
to judge on that route.

---

## Section 5 — WATCH items (not guide edits, or low-confidence).

- WATCH-1 (n=1): over-aggression — sole clock into a fatal profitable block. vs62 s7: Tourach (2/1, the
  game's only real threat) attacked into an untapped Yavimaya Enchantress (3-power) which blocked and
  killed it for zero opponent damage. The attack-floor's "declare ALL of them" is correct and winning
  overall; this is the opposite-failure-mode edge (reckless attack) the two-tier method warns about. n=1
  in a game lost regardless — I did NOT edit the floor for it (softening "declare all" risks re-opening
  the durdle litigation the floor exists to suppress). Flagged for the next corpus: if a lone growing/key
  attacker suiciding into a bigger untapped blocker recurs, add a tightly-scoped carve-out.
- WATCH-2: Tergrid draw variance. 0/12 games across two corpora have exercised the steal engine (drawn
  once, too late). Not guide-fixable; a Tergrid+enablers probe deck is the validation path.
