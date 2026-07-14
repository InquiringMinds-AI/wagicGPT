# deck133 wave-8 — development notes (engine / harness / model / deck-construction)

Corpus: PRIMARY control arm `matchups-20260714-044131`. 135v133 = KNOWN ENGINE WEDGE (turn-2
livelock, 6 records) — EXCLUDED. Real games: 140(W), 131(W), 109(W), 110(L), 44(L).

## ENGINE / REPRESENTATION — inherited items now RESOLVED
1. **Gray Merchant drain display — FIX SHIPPED AND VERIFIED (was wave-7 open item #1).** The option
   now shows the RESOLVED total including GM's own +2. Repro: vs109 seq36, `{right now: drains 8}`
   resolved `opp -8 / my +8` (devotion 6 + GM's own 2 = 8). Consistent with the brief's probe (drains
   6 on devotion-4). ACTION TAKEN: deleted the guide's "+2" workaround; changed all `N+2` thresholds
   to `N`. No further engine work.

2. **Liliana no-legal-target loop — FIX CONFIRMED (was a wave-7 concern).** 0 `fails validation` in all
   6 stderr files; 0 defer records in JSONL. Both Lilianas coexist-cast in different games (Last Hope
   vs110 seq16, of the Veil vs44 seq34); neither looped. Closed.

## HARNESS
3. **Priority re-ask multiplier still present** (vs44 seq37-40: repeated single-option "Put in Play
   with Bloodstained Mire" re-asks, mana unspent — re-offers, not real fetches; vs110/109 also
   inflated). Known infra item, not a leak; noting so a future wave doesn't read the count as durdle.

4. **`resolved:` field still not emitted (reviewer-facing).** The new `events` field DOES carry the
   life/zone deltas that let a reviewer verify resolution (used here to confirm the GM +8/-8), but an
   explicit `resolved:` field would remove the manual narration-scrape. Harness/evidence ledger.

## MODEL (route to model-experiments residuals, NOT guide lines)
5. **Liliana-the-Last-Hope +1 (-2/-1) not recognized as removal.** vs110 seq12 (T5, my8, being raced):
   the pilot had `Cast Liliana, the Last Hope` offered and chose Bloodghast (a can't-block body); the
   +1 would have killed the 6/1 Memnite (6/1 -> 4/0) a turn earlier. Narrow (only kills X/1) and in an
   already-losing race; a litmus case for "does the pilot see a loyalty +1 as removal?" — NOT worth a
   guide line (it would risk mis-firing the +1). Single instance; watch.

6. **Thoughtseize past-T3 empty/small-hand discipline (watch, not a change).** 2/2 past-T3 Thoughtseizes
   were into <=1-card hands, both in losses (vs110 seq23 EMPTY hand at my2 while dying; vs44 seq21
   1-card hand at my14->12). Rule is present, correct, and obeyed in the early game of all 3 wins.
   Both disobediences in already-lost/construction spots -> frozen, not louder. Repro anchors above.
   Escalation IF it recurs in a deciding spot: representation (surface opp hand size at the cast).

## DECK CONSTRUCTION (upstream decklist flags — RE-CONFIRMED from wave-7; do NOT fix with guide length)
7. **No early blocker; Bloodghast (T2 body) cannot block.** Both losses were races lost with no wall
   drawn early (vs110 affinity Memnite+Plating to T8; vs44 faerie flyers to T14). Only ~1 real
   `blockers` decision all corpus. Consider a cheap early blocker.

8. **No reach / no flyer answer except Gray Merchant; GM density 4/60, no selection.** GM reached the
   list in 1 game this wave and WON it (vs109 stabilizer); in both losses it never came, and deck133
   has no way to touch flyers (vs44 died to unblockable Faerie/Archmage beats). Consider +1 GM or cheap
   selection, and/or a reach/flyer-defense source.

9. **Discard-suite self-damage = the close-race margin.** Thoughtseize 2 + fetch 1s supplied the exact
   life deck133 died by (vs44 died at 0; vs110's Thoughtseize at 2 life). The wave-6 Thoughtseize-split
   mitigation still holds in the EARLY game; the residual is construction (a 1-copy Thoughtseize ->
   Inquisition shift would cut self-damage), not a guide gap.

## WHAT WORKED (freeze — do not churn)
- Impact-ranked act-floor + attack floor: OBEYED in all 3 wins (140/131/109 attacked with everything
  each turn once they had a board). Defender/Obliterator-wall + GM <=10 stabilizer line: WON vs109
  (flipped L->W). Thoughtseize/Inquisition split: held in the early game. GM two-uses-two-triggers:
  the stabilizer use finally exercised and won. Only change this wave = the mechanical GM display
  `N+2 -> N` correction.
