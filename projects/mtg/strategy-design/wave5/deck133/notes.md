# Wave-5 deck133 — working notes (engine / harness / deck-builder)

## For the ENGINE owner

### E1 [FIXED — confirm] Negative-life state-based loss now applies.
Wave-4's P0 (opponent stuck at -10, game continued) is resolved for this deck's
drain kills: vs44 ended via a `gameend` record at opp **-13** (seat 1783972792,
seq35→gameend), vs135 at **-3** (1783979342), vs131 at **-1** (1783976205). The
lifeleech drain now triggers the loss check. No repro of the wave-4 bug this run.

### E2 [NEW, representation] Gray Merchant option under-displays its own drain by 2.
The option renders `Cast Gray Merchant of Asphodel {3}{b}{b} (2/4) {right now:
drains N}`, but N excludes Gray Merchant's own {B}{B} = 2 devotion (he is still
in hand when the option is built; `auto=lifeleech:...:mybattlefield` counts him
only once he resolves). Verified: displayed 4→actual 6 (1783976205 seq40),
6→8 (seq44), 8→10 (1783979342 seq43). **Ask:** compute the displayed drain as if
Gray Merchant were already on the battlefield (+ his own devotion), so the number
the pilot reads is the number that lands. Until then the guide teaches "real drain
= shown N + 2" as a stopgap (removal condition: this display fix ships).

### E3 [rendering] Collective Brutality escalate menu emits malformed / opaque option lists.
Seat 1783980403 (vs110): seq37 shows `1: | 1:Cast Card Normally | 2:escalate |
3:Decline`; seq39 shows a duplicated-index list `... 7: | 1:Put in Play ...`.
The `Cast Card Normally` label is opaque (wave-4 interface-defusal class). Repro:
1783980403 seq36-39. Not guide-fixable; flagging for the modal-menu assembler.

## For the HARNESS owner

- **results.tsv still lacks final life on `timeout` rows** (carried from wave-4).
  vs140 (1783972794) scored `timeout` but the seat log's last record is a lethal
  attack declaration at opp 2 — a near-won game. A final-life snapshot at cap (or
  a `gameend` on timeouts, still absent) would stop every reviewer re-opening the
  translog to learn a timeout was winning-but-slow.
- **Single-option priority re-ask multiplier** still inflates decision count and
  wall clock (vs140 68 records, vs131 46; many are the same single fetch/priority
  offer re-asked across phases). A within-turn "declined this exact single-option
  offer" memo would cut grind games materially with zero decision-quality cost.
- **Latency:** deck133-seat calls spiked to 64-74s six times in the vs140 game
  (seq0/19/28/44/55/61) — consistent with `-j` concurrency saturating Spark. On
  grindy lifegain pools (140, 131 run Elixir of Immortality) games cap on wall
  clock; either raise the cap, lower -j, or score capped games by final life.

## For the NEXT deck133 reviewer (process record)

- Timeout attribution held again: print final `my_life`/`opp_life` FIRST. vs140
  ended my6/opp2 with a lethal attack on the last record — winning-but-slow, do
  not read as passivity. The 24/33 cast-nothing in that game was ~all correct
  targetless Fatal Push (empty opponent board), not a durdle; exclude it and the
  corpus cast-nothing rate is ~16%.
- The wins ARE the guide material: vs44 (WON vs the Faerie deck that beat it twice
  at opp-20 in wave-4) is the DEFENDER template working — Obliterator wall +
  Fatal Push on their board. Watch vs109/vs110 next wave for whether the impact-
  ranked act-floor stops the discard flood, and whether Gray Merchant gets cast
  at low life (the vs110 seq34 miss).
- Grep signatures: discard-flood = `Cast Thoughtseize`/`Cast Inquisition` chosen
  on turn >=4 while my_life is falling and opp_life flat; GM-stabilize miss =
  `Cast Gray Merchant` present in the option list at my_life <= 10 while
  chosen_text is a different cast.

## For the MODEL-EXPERIMENTS track (qwen35 pilot profile)

Executes reliably: affordability, turn-numbered deploy, multi-attacker lethal
math, impact-ranked discard/removal TARGET picks, Yawgmoth loop, Gray Merchant
recursion via Liliana, fetch-cracking, reading the `drains N` display.
Remaining failure signatures (ranked by cost this corpus):
1. **Emergency-stabilize blindness** — at 1 life it reached for removal on a 0/2
   over a castable Gray Merchant that gains life = drain (vs110 seq34). It knows
   GM "is the win condition" (says so seq38) but doesn't fire it as a survival
   button; the trigger has to be pre-chewed to a life threshold in the guide.
2. **Posture inertia** — holds "strip their hand" across a losing race; the
   DEFENDER trigger facts are all visible but it re-derives role too late (vs109,
   only at life 5). Same as wave-4; needs the trigger pre-chewed, which the guide
   now does.
3. **Plan/list disconnect** — plans a cast the menu doesn't offer, then picks
   "Cast nothing" over an available option (vs110 seq26). The plan register is
   still not legality-checked; candidate is a structured pre-choice extraction
   (name which LISTED option the plan uses) — representation, not guide prose.

## For the DECK BUILDER (standing flag: Gray Merchant density)

- **Gray Merchant density vs role — reaffirm.** 4 copies / 60, no tutors or
  selection: it reached castability in only 3 of 6 games (cast 3). It is the
  deck's ONLY blocker-independent damage AND its stabilizer, and half the losses
  are games it never arrived. Options: +1 Merchant, or cheap selection (a 2nd
  Phyrexian Arena raises both draw and devotion). Or accept ~half the games are
  won by beats — the guide now covers both.
- **Discard suite cost in this pool.** 8 one-mana discard spells (4 Thoughtseize
  at 2 life each) in a pool where >=3 opponents race (44 Faeries, 109 red aggro,
  110 affinity). Both losses (vs109, vs110) had self-inflicted life (Thoughtseize
  + fetches) as a real damage source. A 1-2 copy shift from Thoughtseize toward
  Inquisition or cheap removal would fit the pool; guide-level mitigation (rank
  discard last when racing) is now in place either way.
- **No reach anywhere except Gray Merchant.** Bloodghast `cantblock` + no fliers
  makes evasion (Faeries) structurally hard on the ground; vs44 was only won
  because Obliterator + removal arrived. A sideboard swap (Plague Engineer is in
  the SB) would help if selfplay ever runs SB logic.
