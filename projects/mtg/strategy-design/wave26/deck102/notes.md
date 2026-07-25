# deck102 dev notes -- wave 26 (engine/harness/model ledger + ROTATION VERDICT)

Corpus matchups-20260725-115216, binary 77e91ef4e. Record 0/6 (L adj vs22, L vs131, L vs133, L vs137,
L vs18, L vs27). 0 fallbacks at seat, 0 corpus-position fallbacks here. The 0/6 is a POOL-RISE +
DRAW-VARIANCE collapse with sound decisions (full decomposition in findings.md).

## VALIDATED / HELD this corpus

### Mulligan edit ("early = by turn 2") FIRST CORPUS -- CLEAN.
All 6 hands KEPT, all 6 keeps correct by the guide's own criteria. The wave-25 over-mulligan class
(turn-1-chase) and the wave-24 fidelity miscount did NOT recur. The edit is validated non-harmful; no
incorrect keep attributable to it; no new mulligan ambiguity. This is the single confirmation wave-25
said the edit needed. Mulligan surface: closed, no new work signal.

### c4 parser hardening -- VALIDATED with 3 live positive cases.
Three replies opened one CHOICE, reversed mid-reply, and the parser correctly took the FINAL/last-
coded answer (deck137 s8 CHOICE 4->1 Animate Dead; deck18 s5 self-negated "CHOICE 1 Block" -> BLOCKS:
none; deck27 s9 CHOICE 3->1 Barren Moor). Zero misfires, zero dropped/mis-picked answers, zero
fallbacks. The N9 not-firing-where-it-shouldn't caution is satisfied at this seat -- the hardening
survived three multi-reversal spirals. Details in findings.md Section 4.

### c1 target-preview (opponent-first, no cap) -- VALIDATED.
Thoughtseize renders "legal targets right now: the opponent, you" opponent-FIRST (vs22 s3, vs18 s26,
vs137 s24). No truncation, no prompt-length side effect.

### Flip-thrash demotion -- HELD round 4.
Tergrid in hand once (vs22 s57/s58, "Flip Side" present); NOT consumed (chose pass / Equip). 0/6
across the corpus (w22:11 -> w23:1 -> w24:0 -> w25:0 -> w26:0). Durable.

### Damnation / reanimation / attack-with-everything -- correct where exercised.
vs22 Damnation t12 removed a lethal Inferno Titan 6/6 at L11 (correct, worth the 2/2 token). vs137
Animate Dead t4 reanimated the opponent's Giant Killer (poor-man's-Tergrid line working). Attacker
decisions declared all available bodies in every combat (vs22 s46/s55) -- no withheld-attacker leak.

## ENGINE (OPEN, master ledger -- untouched this corpus, Tergrid never cast)
- **R-DFC-BACKFACE-RESOLVES-FRONT** -- UN-RE-EXERCISED 4th consecutive wave (Lantern back-face never
  cast; model never reached a Lantern-affordable/God-unaffordable turn, and Tergrid was in hand only
  once, uncastable before game end). Fix map on ledger (AllAbilities.cpp:5422 + autostack
  paid(alternative), no suite coverage). The deck199 probe already reproduced it; nothing new here.
- **primitives-only card + real set hint = silent drop** (MTGDeck.cpp:1043, loud-rejection candidate,
  low) -- carried from the probe report; not exercised at this seat (deck102's (KHM) works only in the
  #CMD commander slot). No new evidence.
- **Tergrid steal chain** -- CLOSED end-to-end by the probe pipeline (parse -> fire -> legible ask ->
  correct model play, per probe report FINAL PROBE). No live corpus reproduction possible (0 windows,
  as the wave-25 mechanistic finding predicted). Discharged.

## NEW ledger item (benign, WATCH -- notes-layer, below core bar)
- **Reversal-spiral tolerated by c4.** The model increasingly emits an opening CHOICE line then
  reverses it 2-3 times before landing (135-224s latency singletons; deck18 s5, deck27 s9, deck137
  s8). BENEFICIAL this corpus (each reversal reached the strictly better answer) and fully absorbed by
  c4's last-coded-line selection. WATCH: if a future spiral ever reverses PAST the correct answer, or
  lands an out-of-range index that the hardening then has to fall back on, promote. Do NOT propose
  suppressing the reconsideration (see general-suggestions.md) -- it is producing better decisions.

## ROTATION VERDICT -- ROTATE deck102 OUT. Guide FROZEN (byte-copy, cmp-identical to deployed).

The rotation test, applied unsentimentally:
- **no-guide-mod: TRUE.** Zero guide edits this wave -- the guide is byte-frozen (9545 bytes, cmp-
  identical, 0 non-ASCII). The wave-25 mulligan edit is validated clean, so the guide has converged
  and needs no further calibration.
- **no-new-work-signal: TRUE.** The 0/6 collapse decomposes entirely to pool-rise + draw-variance
  with sound decisions and no repeated guide-addressable decision leak (findings.md Section 1). The
  only remaining open items are ENGINE (R-DFC-BACKFACE, primitives-set-hint drop) already on the
  master ledger and already characterized by the probe -- NOT per-wave guide-review work. The steal
  chain is discharged by the probe. The mulligan edit's confirmation is delivered. The c4 hardening is
  validated. There is NO live new decision-class at this seat.

Both legs of the test pass -> ROTATE OUT. wave-25 said "guide CONVERGED, steal validation is
structurally undischargeable in the loop, hand to the probe"; the probe DID discharge it; this wave
confirms the guide is frozen-clean and the 0/6 carries no new work. deck102 leaves the guided per-wave
review rotation. Any future engine fix (backface, set-hint drop) is ledger/engine work, not a reason
to re-seat deck102 in the guide loop. Deck supply is unbounded (146 guideless remain); this seat has
reached NEW-WORK EXHAUSTION for the guide layer.
