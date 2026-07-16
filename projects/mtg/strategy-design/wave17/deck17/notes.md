# notes.md — deck17 engine/harness ledger + rotation verdict (wave-17)

Layer-routed engine/representation/model items at the deck17 seat. Guide prose lives in
strategy.txt; this file is engine/harness/model only.

## Observed this corpus

1. STALE WITHIN-TURN MANA — LARGELY RESOLVED (was wave-16's main residual tax). The trust-the-menu
   line the revised guide added held: vs62 s15->s16 (2nd same-turn cast) was clean with no
   cast-list-distrust spiral; vs21 s15 showed one mild recount that SELF-CORRECTED (reconciled
   `Mana available: {u}` with the tapped-Island board and cast Gatewarden). None of wave-16's
   multi-thousand-token "why isn't Wasp Lancer listed?" spirals recurred. Representation lever
   still available (narrate remaining mana per cast) but the pressure is much reduced.

2. LONG-RAMBLE -> unparsed -> heuristic (model-incapacity, carried from wave-15/16). vs135 s13
   (already-lethal window), vs21 s10 (block-restriction re-derivation). Parse guard caught both
   correctly; neither flipped a game. Same class as prior waves. Route: model-experiments /
   long-ramble residuals. Representation cues proposed in general-suggestions.md (items 2, 3).

3. BLOCK-RESTRICTION RE-DERIVATION as a distinct ramble trigger (vs21 s10). The model re-proves
   "does creature X share Cloud Sprite's flyers-only restriction?" from card text on flyer-block
   decisions. Candidate for a blockers-menu per-creature annotation (`[can block: flyers only]` /
   `[can block: any]`). Representation item, benefits all flyer decks.

4. HYBRID-PIP ANNOTATION EFFECT (positive, no defect). Rendered on all 35 hybrid prompts at this
   seat (corpus-heaviest exposure); 0 mis-renders, 0 off-color misreads, 13/13 hybrid casts at the
   correct total. Strongest single confirmation for the synthesis annotation-effect tally.

5. NO stale_echo / wrongful downgrade at this seat (consistent with wave-16). The 3 choice=-1
   records are genuine long-ramble/unparsed, not superstring echoes.

6. PLAN-carry stale-intent caveat (new binary): observed firing harmlessly — e.g. vs110 s27 the
   model notes "My plan to cast Sentinels was incorrect because I am already in the Attackers
   phase" and self-corrects to attack. No misparse from the caveat at this seat.

## Deck-construction note
deck17 is 5/6 (up from 4/6). Its two wave-16 losses were both has-flyer/lifelink matchups; the
revised race/deployment guide reversed the one that recurred in this pool (vs110). The single loss
this corpus (vs21 goblin/burn) is a draw-and-matchup variance loss on a redundant land-light hand,
not a construction flaw and not a taught-class regression. No construction change proposed.

## ROTATION verdict
**deck17 IS a rotation candidate.** Its guide has now VALIDATED across two consecutive waves
(3/6 guideless -> 4/6 first guide -> 5/6 revised guide) and this wave produced NO deck-specific
guide work: every taught class DIED or PERSISTS-and-obeyed, the two wave-16 root fixes
(walls-aren't-flyers, race branch) fired and reversed their causal matchup, and the hybrid
annotation is fully load-bearing at the corpus's heaviest exposure. Guide is FROZEN
(byte-identical). All remaining residuals are corpus-wide model/representation items
(long-ramble, block-restriction ramble, mild mana-recount) already routed to notes/general-
suggestions and shared across decks — none is deck17-specific guide work. No-guide-mod AND
no-new-deck-specific-work-signal = rotation candidate. Recommend rotating deck17 out and letting
its guide stand as a converged exemplar; if kept, next-corpus watch is purely confirmatory.
