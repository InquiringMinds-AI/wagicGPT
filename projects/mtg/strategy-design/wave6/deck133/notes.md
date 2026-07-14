# Wave-6 deck133 — working notes (engine / harness / deck-builder / model)

## For the ENGINE owner
### E1 [STILL LIVE — re-confirmed] Gray Merchant option under-displays its own drain by 2.
Option renders `Cast Gray Merchant of Asphodel {3}{b}{b} (2/4) {right now: drains N}`; N excludes
Gray Merchant's own {B}{B}=2 devotion (`auto=lifeleech:-type:manab:mybattlefield` counts only
resolved permanents; he is in hand when the option is built). Re-verified this corpus on the one
RESOLVED cast: **vs44 (1783991689) seq55 shown `drains 10` → my 4→16 (+12), opp 10→-2 (−12).
Actual = shown + 2.** (vs131 seq69's cast was countered, no resolution to measure.) Ask: compute
the displayed drain as if he were already on the battlefield. Guide stopgap "shown N + 2" until then.

### E2 [confirm — FIXED, held] Negative-life state-based loss fires cleanly.
All three kill games ended via a `gameend` record: vs44 opp -2 (seq56), vs110 opp 0 (seq34),
vs109 opp -2. No repro of the wave-4 "stuck at negative life" bug. Resolved for this deck.

## For the HARNESS owner
- **Single-option priority re-ask multiplier** still inflating: vs140 (1783989616) = 81 priority
  records of 105; vs131 = 20 of 89. Many are the same single fetch/targetless-Fatal-Push offer
  re-asked across phases. A within-turn "declined this exact single-option offer" memo would cut
  grind/timeout games materially with zero decision-quality cost.
- **results.tsv timeout rows still lack final life** (`131 133 timeout - - -`, `140 133 timeout
  - - -`). vs131 was a dead-even draw (17-17 T31); vs140 was a behind-loss (15-25 T20). Neither is
  legible from results.tsv alone. Add a final-life snapshot or a `gameend` on cap.
- **Latency:** 3 calls ≥60s in each of vs131/vs140, max ~70s, consistent with `-j` concurrency
  saturating Spark. Grindy lifegain pools (140 Elixir/Venser's Journal, 131 tokens/Liliana) hit
  the wall-clock cap; raise the cap, lower -j, or score capped games by final life.

## For the NEXT deck133 reviewer (process record)
- Parser clean AGAIN (0 desyncs, 6 games). No engine no-op. The corpus is clean enough that the
  only real loss (vs135) and the two timeouts are attributable to life-discipline + construction
  + infra, not the pilot mis-executing.
- **Fix the load() glob to match `-ai_baka_deck133-` explicitly** — both seats of a game share the
  `start_epoch`, so a bare `EPOCH-*.jsonl` glob silently grabs the OPPONENT's seat file (cost me
  one wrong summary pass on the 131v133 game). Game→seat map this run:
  131v133=1783987216(89rec,draw17-17), 135v133=1783987218(LOSS my-1), 140v133=1783989616(TO loss),
  44v133=1783991689(WIN opp-2), 110v133=1783995814(WIN opp0), 109v133=1783996045(WIN opp-2).
- Grep signatures that mattered: Thoughtseize-into-empty-hand = `Cast Thoughtseize` chosen while
  the board shows opponent 0 cards (vs135 seq24, pilot's own seq26 reply admits "0 cards, so
  Thoughtseize is redundant"); premature GM = `Cast Gray Merchant` chosen with `drains N` where
  N+2 < opp_life and my_life > 10 (vs131 seq69, countered).

## For the MODEL-EXPERIMENTS track (qwen35 pilot profile)
Executes reliably: affordability, turn-numbered deploy, multi-attacker lethal math, impact-ranked
discard/removal TARGET picks, Collective Brutality -2/-2 removal mode (NEW this wave — used vs109
seq29, vs135 seq19; the wave-5 "missed" leak is closed), Bloodghast recursion + go-wide, Fatal
Push at the real attacker, reading the `drains N` display, survival blocking with undying Messenger.
Remaining failure signatures (ranked by cost this corpus):
1. **Life-cost blindness on Thoughtseize** — pays 2 life to strip an EMPTY hand while racing
   (vs135 seq24), decisive in the only loss (race lost by 2). Guide now separates Thoughtseize
   (2 life) from Inquisition (free) and gates it on a visible empty/irrelevant hand.
2. **Payoff-position override** — takes a conditional #1-ranked payoff (Gray Merchant) even when
   its gating condition is false, because it is listed first (vs131 seq69, countered). Guide now
   states the ELSE branch inside the entry.
3. **Plan/list disconnect (stale plan)** — PLAN repeats "cast Yawgmoth now" for 5 decisions while
   Yawgmoth is unaffordable and unlisted (vs131 seq40-45); the pilot correctly picks the LISTED
   Fatal Push/Inquisition instead, so no bad choice, just a hallucinated plan. General-prompt
   territory (stale-plan grounding); candidate for structured pre-choice extraction (representation).

## For the DECK BUILDER (standing flags — reaffirm from wave-5)
- **Gray Merchant density vs role.** 4/60, no tutors/selection: castable in only 2 of 6 games,
  and NEVER during a low-life crunch (my6 vs109, my10 vs110, my-1 vs135 all had no GM in the list).
  It is the deck's ONLY blocker-independent damage AND its stabilizer. +1 Merchant, or cheap
  selection (a 2nd Phyrexian Arena raises draw AND devotion), would let the wave-5 stabilize rule
  actually fire. Or accept ~half the games are won by beats — the guide covers both.
- **Discard self-damage in a race-heavy pool.** 4 Thoughtseize at 2 life each. Both non-wins where
  life mattered (vs135 loss, vs140 grind) had Thoughtseize + fetch life as a real damage source;
  vs135 was lost by exactly the 2× Thoughtseize margin (4 life). A 1-2 copy Thoughtseize→Inquisition
  shift fits the pool. Guide mitigation (rank Thoughtseize below Inquisition, gate on empty hand)
  is in place regardless.
- **No reach except Gray Merchant.** Bloodghast `cantblock` + no fliers makes lifegain/reset decks
  (vs140: Elixir of Immortality, Venser's Journal, Wrath of God — opp climbed to 25) structurally
  hard: with no reach and no GM drawn, deck133 cannot outrun repeated lifegain + a board wipe.
  Plague Engineer / more reach out of the SB would help if selfplay ever runs SB logic.
