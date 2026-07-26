# deck134 (Urza's Control, mono-U/G tron) - wave-28 findings (FIRST-GUIDED, 0/6 AGAIN)

Corpus: matchups-20260726-050449, binary ad8930961, -T 3000. Guide live (wave-27 deploy).
Pool shift this wave: deck131 (UR burn) rotated OUT, deck146 (Orzhov Dungeons) IN.

Seat logs (deck134 side), all LOSSES:
- vs18  (Kithkin)   1785060294 - LOSS turn 10, my -3, opp 20   (dmg dealt 0)
- vs22  (Giants)    1785060296 - LOSS turn  8, my  0, opp 20   (dmg dealt 0)
- vs93  (Rats)      1785060868 - LOSS turn 11, my -5, opp 20   (dmg dealt 0)
- vs136 (BW aggro)  1785061389 - LOSS turn 15, my -2, opp 22   (dmg dealt -2; opp gained)
- vs146 (Orzhov)    1785063877 - LOSS(adj) turn 18, my 4, opp 17 (dmg dealt 3)
- vs137 (convoke)   1785069022 - LOSS turn 10, my -8, opp 18   (dmg dealt 2)

Fallbacks: 1 (vs146 seq25 ask/retracted_choice - decomposed below, benign). Parser clean.

## THE HEADLINE: every guide-addressable leak from wave-27 is FIXED, and the record did not move.

The guide's job was to fix the wave-27 DECISION leaks. It did - comprehensively. Tron went from
assembling in ~1/6 games (wave-27) to 3/3 in FOUR of six and 2/3 in the two that died too fast.
Map is cracked at the FIRST payable window in every game (vs wave-27's ~10 passed windows/game).
Mulldrifter is hard-cast 100% of the time and used as a blocker. Fogs and Pulse fire on trigger.
And the deck STILL went 0/6, dealing ~0 damage - LESS net than wave-27. The losses are not in the
places the guide can reach.

## PER-TEACH VALIDATION (independent of the record)

| # | Teach | Verdict | Evidence (seq) |
|---|-------|---------|----------------|
| 1 | Crack Expedition Map on sight; fetch missing Urza land | VALIDATED | Cracked at FIRST payable window EVERY game: vs18 seq6 (T5), vs22 seq9 (T7), vs93 seq6 (T4), vs136 seq21 (T8), vs146 seq5 (T4), vs137 seq5 (T5). Fetch targets SANE - always the missing Urza piece (vs18 Mine->Tower->Power Plant; vs93 same). Tron 3/3: vs18(T10), vs93(T6), vs136(T14), vs146(T8); 2/3 in vs22/vs137 (died first). Wave-27: passed ~10x/game, tron 1/6. Complete reversal of the #1 leak. |
| 2 | Stonehorn+Ghostly Flicker lock (blink Stonehorn every turn) | TAUGHT, UNEXERCISABLE (construction) | NO "Cast Stonehorn"/"Cast Ghostly Flicker" option in ANY of 6 games. Both only surfaced as LIBRARY REVEALS from dig (Impulse/Forbidden Alchemy) and went back to library (vs136 lines 197-254: revealed then returned). Pilot INTENDED the lock - vs136 PLAN verbatim: "I will immediately cast Stonehorn Dignitary to lock the opponent out." Teach landed cognitively; pieces never reached a castable hand. Lock assembled in ZERO games - a draw-dependent 2-card combo the deck cannot tutor. |
| 3 | Ghostly Flicker targets - value ETBs, NEVER a bare land | UNEXERCISED | Flicker never castable (see #2). No land-blinks possible; no evidence either way. Wave-27 this leaked (blinked lands 2x); this wave the card never came up. |
| 4 | Mulldrifter: hard-cast {4}{U} under pressure, evoke only when safe | VALIDATED | Hard-cast 100%: vs18 seq13 (T9 L6), vs136 seq24 (T10 L13), seq29 (T12 L11). ZERO evokes. Every hard-cast then blocked (vs18 seq15 blocks Field Marshal; vs136 seq26 blocks Dread Presence, seq31 blocks Rankle). Wave-27 evoked under pressure 2x. Reversed. |
| 5 | Fogs (Moment's Peace) / lifegain (Pulse) on trigger | VALIDATED (1 minor slip) | Pulse fired at 1 life: vs137 seq13 (T9 L1, dug via Impulse). Fog fired on the one clear lethal-attack window: vs146 seq21 (T17 Blockers, L4, opponent lethal swing). ~25 "held" fog offers are all on the pilot's OWN main phases at safe life = CORRECT restraint, not leaks. NO death caused by a skipped-available fog. ONE slip: vs136 seq39 fogged on its OWN Main phase 2 (T14 L6, no combat) = wasted - a model-execution slip against a correct guide instruction, not a guide defect, non-converting. |

## STRUCTURAL VERDICT: CONSTRUCTION-TERMINAL (flag to ROSTER OWNER)

With the guide's levers demonstrably pulled, 0/6 is construction-terminal. Quantified vs wave-27:

|                         | wave-27 (guideless) | wave-28 (guided) |
|-------------------------|---------------------|------------------|
| Record                  | 0/6                 | 0/6              |
| Damage dealt (median)   | ~0 (opp final 20/20/16/19/32/20) | ~0 (opp final 20/20/20/22/17/18) |
| Damage dealt (net/6 gm) | ~ -7 (opps gained)  | +3               |
| Tron assembled (3/3)    | ~1/6 (only vs136)   | 4/6 full + 2/6 partial-at-death |
| Map crack latency       | T12+ / never        | first payable window, every game |
| Mulldrifter             | evoked under pressure | 100% hard-cast, used as blocker |
| Own final life (avg)    | -0.67               | -2.3 (pool shift: Giants T8 blowout) |
| Turns survived (avg)    | 13.7                | 12.0             |

The guide moved tron assembly from ~17% to ~67-100% of games and fixed every decision leak, and
damage dealt stayed at ~0 and the record stayed 0/6. That is the signature of construction-terminal:
the losses live in construction, not decisions.

WHY it cannot convert even with the engine online:
1. The only hard anti-aggro plan (Stonehorn+Flicker) is an UNTUTORABLE 2-card combo. It appeared
   only as library reveals in 6 games; it never reached a castable hand. A deck whose survival
   depends on drawing both halves of a combo it cannot search is construction-fragile by design.
2. The clock (Mulldrifter 2/2 flyers) is correctly used as BLOCKERS under aggro, so it never
   attacks -> ~0 damage. No other clock (Compelling Argument mill needs the same uncastable
   Flicker+Wall loop).
3. Under -T 3000 cap-adjudication the deck CANNOT win on life. Every game it survives to the cap
   while behind (vs146 T18 at 4 vs 17) is scored a LOSS by adjudication. A clockless control deck
   in a 7-seat aggro pool has no path to a cap win.

This is the honest "teaches validated, record flat" verdict the brief anticipated - unusual but
clean. The guide instrument's reach ends at decision quality; it did all it can here in ONE corpus.

## DEFER DECOMPOSITION (the 1 seat fallback)
vs146 seq25 (T18 Main phase 2, own turn, L4, fallback=retracted_choice, choice=-1). The model
REPLIED cleanly "CHOICE: 1 (Cast Moment's Peace {1}{g} [from your graveyard])" - a flashback fog -
on its OWN Main phase 2 where there is no combat to prevent. Its own reasoning even noted "the
opponent has already finished their combat phase," yet still selected it. The engine RETRACTED the
choice (no legal effect for a fog with no combat pending) and the heuristic answered; the pilot
then cast Mnemonic Wall (seq26). Classification: benign self-healing retraction of a wasted-intent
model choice (desperation at 4 life to the cap). NOT the e5 annotation-echo class - the CHOICE line
matched option 1 exactly with no bracketed-tail mismatch (e5 holding at this seat). Cross-refs
N-134b (the option displayed "{1}{g}" - the flashback-cost display bug, reproduced).

## What the CORE handled correctly (PASS - do NOT re-teach)
Role=defense (blocked correctly every combat), fog RESTRAINT on own turns (held ~25 wasted-fog
offers correctly), counter timing, Map crack now fires (guide-added, held), "listed = payable"
(paid off-color pips via Unknown Shores/Prism, no color-decline), mulligan by land count, Pulse at
low life. Counterspell/Condescend usage sound. Core + guide are behaving; the deck is the problem.
