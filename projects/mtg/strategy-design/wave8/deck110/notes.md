# Deck-110 wave-8 — development notes (engine / harness / model / deck-construction)

## E1 — CLOSED & RE-VALIDATED: the re-equip "(ALREADY attached... change NOTHING)" marker.
Wave-6 inert -> wave-7 fired-and-obeyed -> wave-8 STABLE. Wave-8 corpus: marker string renders
(`(ALREADY attached to it - this would change NOTHING)`), 16 marked-offer decisions, marker taken 0,
no-op re-equip rate 0/9 = 0% (wave-7 6%, wave-6 50%). No further engine or guide work; the guide's
compressed Rule #1 (wave-7) holds. Keep the fix in `src/AIPlayerGPT.cpp:1163-1173`.

## E2 — ENGINE/REPRESENTATION (watch, unchanged from wave-7): per-instance marker blind to multi-copy churn.
The marker fires only on "already attached to THIS target." With two Cranial Platings out, a pilot can
still move Plating-A off a carrier holding Plating-B (a genuine, unmarked state change). This wave the
churn did NOT recur as a costly loop — 9 equips, all justified (see findings). Candidate engine fix
stands: suppress/demote the equip re-offer once EVERY copy of the equipment is attached to SOME
carrier. Low priority; promote only if a second multiples-of-an-equipment deck reproduces a costly
instance. NO guide text.

## E3 — ENGINE/REPRESENTATION (NEW, informational, no action): the `Artifacts in play: you N` line is accurate; the pilot ignores it.
The prompt now surfaces `Artifacts in play: you N | opponent N` (and `Your battlefield (creatures: N)`).
The artifact count is 100% accurate: recomputed 180/180 records with ZERO offset (it counts Mox Opal +
Darksteel Citadel + artifact creatures, and correctly excludes Glimmervoid). No engine bug. The pilot
nonetheless hand-counts and over-counts (folds Glimmervoid + basic lands in; vs140 s33 "6 artifacts (2
Glimmervoids, Island...)" vs surfaced `you 3`; vs131 s5-12 "3" vs `you 2`). This is fixed at the GUIDE
layer this wave (teach reading the line), not the engine. If a future reviewer wants a representation
nudge, bolding/relabeling the count is the lever; the number itself needs no change.

## M1 — MODEL / REPLY-PROTOCOL: degenerate unparsed attackers reply.
`1784026638` (vs131) seq18 T5 attackers: `fallback:unparsed_reply`, choice=-1, reply =
`Answer:\n###\n###...` (a repeating degenerate token stream). Only one attacker offered (Signal Pest
1/2); the engine fell back to the heuristic. Non-fatal (blowout win, opp -8). This is the known
model-capability degenerate-output class (skill: "** **" degenerate blockers reply -> residuals).
Route to the model-experiments residuals track as a litmus case; NOT guide-fixable. n=1 this seat.

## H1 — HARNESS: the new `defer` record makes a validation-drop machine-readable (works as designed).
`1784028007` (vs140) seq3 T1: `kind:defer`, `fallback:deferred_to_heuristic`, chosen `Glimmervoid`,
options 0, empty reply, latency -1. The model's land pick (seq2 "Play Glimmervoid") failed validation
(`game-140v110-*.stderr`: "model chose land Glimmervoid but it fails validation; deferring to
heuristic") and the heuristic played the SAME land. Zero strategic harm; recorded only to confirm the
new `defer` record type surfaces validation-drops that the wave-6 stderr-scrape used to require. No
action; a land-play validation edge worth an engine glance if it recurs across seats (why did a normal
first-land Glimmervoid fail validation?).

## DC1 — DECK-CONSTRUCTION / META FLAG (standing, reaffirmed — owns the loss column).
deck110's single wave-8 loss (vs44) is a STACK of the standing axes, and this wave it manifested as a
counter+evasion RACE-LOSS (not the wave-7 flood):
1. **Counter-vulnerability (meta).** Wins route through a few discrete high-value threats (Master,
   Etched Champion, Cranial Plating) = ideal 1-for-1 counter targets now that the stack is surfaced and
   counters are labeled. deck44 Counterspelled Plating + Champion (+ Master) — stderr-confirmed.
2. **Evasion exposure.** deck110 can only block fliers with Vault Skirge / Ornithopter; against an
   evasive clock (Archmage of Echoes flier) it gets run over once those flyers are spent/countered.
3. **Reach starvation.** Only 4 Galvanic Blast is the sole non-creature reach; vs44 the pilot held 2
   Cranial Platings at the end with no creature to equip and no Blast drawn, 4 short from opp 7.
Not a guide task. If the deck list is ever tuned: trim ~0-power filler (4x Signal Pest 0/1, Ornithopter
0/2) and/or the Mox/land count toward more mid bodies, and add reach/evasion-block redundancy, to cut
flood + single-threat-counter + evasion exposure at once.

## Positive validations (no action, recorded for continuity)
- Harness CLEAN: 0 desyncs across 206 decisions; the only fallbacks are the 1 defer + 1 degenerate above.
- Land-drop enumeration (engine ledger #1) FIRED: 16 multi-land co-offers for deck110. No color-priority
  land stopgap in this guide to drop.
- Blocker seam: 6 windows, all correct (free protection-block of an 8/8; favorable trade; appropriate
  chumps). Wave-6/7 attack-seam survival clause continues to behave; no reflexive high-life block.
- Core `hand -> stack` cast-happened anchor REMOVAL (adopted by synthesis): 0 engine no-ops this seat —
  corroborates no-regression.
