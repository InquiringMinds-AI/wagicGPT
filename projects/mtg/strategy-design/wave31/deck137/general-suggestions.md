# deck137 wave-31 -- core (general) prompt proposals

## PASS (26th consecutive)

Nothing in this corpus routes to the general system prompt.

Layer-check of every candidate this seat produced:

| observation | correct layer | why not core |
|---|---|---|
| convoke offer/payment colour gap (F4 / N-137a) | ENGINE (`ExtraCost.cpp`) | the engine offers or does not offer a cast; no prompt wording changes what is legal |
| `maxAnnounceableX` not reached by the alternative-cost announce (F1 / N-137b) | ENGINE (`MTGRules.cpp`) | same |
| convoke branch silent no-op (F2 / N-137c) | ENGINE | same |
| divergence C defer (N-137d) | ENGINE (`AIPlayerGPT::FindCardToPlay`) | the named fix direction is a re-ask mechanism, not prompt text |
| whole-library search reveal (N-137e) | ENGINE/representation | the eligibility HEADER already does the prompt-side job and was read correctly 12/12 |
| pilot cast March at X=0 once (vs152 s33) | DECK GUIDE (already covered, lines 51-52) | a per-deck payoff rule; putting X-spell discipline in the core prompt would apply it to decks with no X spells |
| one reply whose `PLAN:` tail contradicted its own first-line `CHOICE:` (vs139 s30) | none | the answer-first protocol already took the right line; the protocol is working exactly as designed |
| the 11.5k-char blockers deliberation abandoned at vs158 s32 | none | the N-18e safety route handled it correctly and at zero cost |

Core-prompt surfaces exercised and observed CORRECT at this seat: the answer-first reply protocol
(0 unparsed, 0 empty, 0 retracted_choice, 0 stale_echo across 243 records), the decline-last
option ordering, the `--- CURRENT SITUATION ---` single-snapshot board, the stale-plan note
("the actions your plan names are no longer among the options available right now" -- fired at
vs139 s22 and was heeded), the bundled attackers/blockers protocol (32 + 15 records, zero illegal
assignments), and the `[tapped - cannot attack or block this turn]` / `[summoning sick - cannot
attack this turn]` tags (read correctly throughout, zero misreads).
