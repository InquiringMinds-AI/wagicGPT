# deck133 wave-7 — development notes (engine / harness / model / deck-construction)

## ENGINE / REPRESENTATION
1. **Gray Merchant drain option under-displays by 2** (RESOLVED 4/4 across waves). See
   general-suggestions.md #1. Fix = show the resolved drain (include GM's own +2); enables
   deleting the guide workaround. A/B, GM-scoped, verify no double-count on other lifeleech
   cards. Repro: run `matchups-*/*deck133*`, vs131 seq72 (`shown 5`, narration `opp -7 / my +7`).

2. **[REVIEWER-FACING] No resolution outcome in the translog.** A consumed cast's
   countered/fizzled/resolved status must be scraped from the next record's narration tail.
   vs44 seq27 GM was Counterspelled (`stack -> graveyard`) but `chosen_text` + the seq28 plan
   read as a successful drain. Proposal: emit a `resolved:` field. Harness/evidence ledger.

## HARNESS
3. **Priority re-ask multiplier still inflates decision counts** (vs110: 57 priority records;
   vs131: 40) — single-option re-asks. Known infra item, not a leak; noting for continuity so a
   future wave doesn't read the count as durdle.

4. **One 120s latency timeout -> empty_reply** (vs135, the only >=60s call in deck133's corpus).
   Isolated; the game was already lost on board. No action beyond the standing latency budget
   (-j 3 was respected; median 6.9-8.4s this run — healthy).

## MODEL (route to model-experiments residuals, NOT guide lines)
5. **Pilot recomputes an engine-surfaced magnitude by hand and errs** (vs131 seq72: ignored
   `{right now: drains 5}`, manually counted devotion, got the wrong number). Cannot be fixed
   with more guide wording — it's a display-trust failure. Litmus record for the "does the pilot
   trust surfaced numbers?" experiment. The representation fix (#1) is the real answer.

6. **Possible cast re-pick loop at low life (LOW PRIORITY — did not decide a game).** vs135
   seq47/50/52: Liliana of the Veil chosen 3x across consecutive `ask` records at my3 while
   dying. Could be an engine no-op (cast not taken) OR legitimate separate priority windows /
   two-Liliana confusion. Not investigated to ground truth because deck133 was dead to Icehide
   beats regardless. Flag for a check IF a re-pick loop shows up in a game it could decide;
   repro anchor: vs135 seq47-52.

## DECK CONSTRUCTION (upstream decklist flags — set expectations, do NOT fix with guide length)
7. **No early blocker; Bloodghast (the T2 body) cannot block.** Both losses (vs109 Goblins,
   vs135 Icehide Golems) were races lost at -1 because deck133 could not WALL early — its walls
   (Geralf's Messenger undying, Phyrexian Obliterator) are 3-4 drops and draw-dependent, and
   went undrawn. Only 1 `blockers` decision in the entire 6-game corpus. Consider a cheap
   early blocker (a 1-2 drop that can actually block) to firm up the race matchups.

8. **No reach except Gray Merchant; GM density 4/60, no selection.** GM reached the cast list in
   2/6 games (unchanged from wave-6), never during a low-life crunch, so its stabilize-at-<=10
   trigger is STILL unexercised across two waves. Both losses had no reach. Consider +1 GM or
   cheap card selection to find it.

9. **Discard suite self-damage is the -1 margin in both losses.** Thoughtseize 2 life + fetch
   1s supplied exactly the 2 life deck133 died by (both losses ended at -1). A 1-copy
   Thoughtseize -> Inquisition shift would cut self-damage without losing disruption (Inquisition
   is free). The wave-6 guide mitigation (Thoughtseize-split) held and is well-calibrated, so
   this is a construction lever, not a guide gap.

10. **[META] Discard answers depth, not width.** vs109's go-wide Goblin engine floods faster
    than 1-for-1 discard can strip; the anti-burn line is calibrated to a card-bound (burn)
    clock. deck133 has no sweeper. Guide now notes "Obliterator wall vs go-wide, not more
    discard" (per-deck); the structural answer (a sweeper or a mass -X/-X) is a construction
    question. The vs109 W(wave6)->L(wave7) swing is DRAW VARIANCE (walls not drawn), not a
    guide regression — do not chase it.

## WHAT WORKED (freeze — do not churn)
- Thoughtseize/Inquisition self-cost split: held + well-calibrated (fired on Damnation/counters,
  avoided empty hands). GM +2 read: offset confirmed again. Impact-ranked act-floor: obeyed in
  all wins. Discard-strips-counters vs the #1 deck: won it (with Obliterator beats as the closer).
