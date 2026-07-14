# Deck-135 (Modern Snow) — Wave-7 game review

Corpus: one GPT-vs-GPT round-robin, run `matchups-20260713-222831`. All six deck135
seat games read decision-by-decision from the translogs. Game→file map at the bottom.
Median seat latency ~7-9s; the wave-6 latency excuse is gone, so durdle attributions
are clean.

## RECORD — 1W / 4L / 1TO (down from wave-6's effective 2W)

results.tsv rows for deck135 (winner=N means deckN-slot won, NOT deck135):

| game    | deck0 | deck1 | outcome (deck135)              | my_life | opp_life | turn |
|---------|-------|-------|--------------------------------|---------|----------|------|
| 135v110 | 135   | 110   | **LOSS**                       | -1      | **20**   | 9    |
| 135v131 | 135   | 131   | **LOSS**                       | -3      | 5        | 17   |
| 135v140 | 135   | 140   | **TIMEOUT** (behind 15v26@T25) | 15      | 26       | 25   |
| 135v109 | 135   | 109   | **LOSS**                       | -1      | 12       | 9    |
| 135v133 | 135   | 133   | **WIN**                        | 16      | -1       | 12   |
| 44v135  | 44    | 135   | **LOSS**                       | -10     | **18**   | 18   |

**Two of the four losses end with the opponent AT OR NEAR 20 (vs110 opp20, vs44
opp18) — the skill's "deck never had a plan running" discriminator.** vs109 opp12 and
vs131 opp5 are closer, but both are clock losses, not durdle. This is an
OBEYED-BUT-LOSING wave: the guide's #1 rule is followed, and the record is dominated by
two causes a guide line cannot touch — a manabase that strands the deck's own 2-color
creatures, and a slow clock. Reply parser clean (0 desyncs; 1 spurious in vs140). One
stderr validation-drop in 135v133 was the OPPONENT's Liliana of the Veil (deck133 seat),
not a deck135 decision.

## CENTRAL QUESTION 1 — did the anti-phantom-lethal board-read anchor fire? (model-quality watch)

**YES — the wave-6 hallucinated-on-board class did NOT recur. Zero instances this corpus.**

The wave-6 loss (vs131 s52-60) was: the PLAN asserted "Abominable Treefolk 5/5 on my
battlefield → lethal," declined Ice-Fang Coatl twice, and passed 9 decisions — while
Treefolk was never cast/offered. Wave-6 added to RULE #1 the board-read anchor: "read
your OWN battlefield line first — if a creature is not printed there, it is NOT in play."

This wave, in the same vs131 matchup, the pilot tracked Treefolk **correctly as in-hand**
and actually CAST it:
- s46 (T15, priority): "My plan is to untap, draw, and then in my main phase cast
  Abominable Treefolk. With 14 snow permanents … Treefolk will be a 14/14" — framed as a
  future cast of a hand card, NOT a present board assertion.
- s53 (T16, ask): **cast Abominable Treefolk (15/15)** — the plan was executed.
- s54 attacks all-out with the resolved Treefolk.

No window in the corpus shows the pilot declining a castable creature ("Cast nothing")
because it believed an un-cast creature was already on its battlefield computing a lethal.
Every "creature not taken" this wave is value-over-creature (Q3 below), not phantom-board.
**The anchor holds. Precondition caveat (rare-event rule): only vs131 and vs44 carried
Treefolk-centric plans, so the non-recurrence is off a modest precondition count — keep
the anchor at full prominence, do not demote on this zero.**

### A DISTINCT residual to NOT confuse with phantom-lethal: stale-plan around an uncastable Treefolk (vs44)

vs44 shows a different failure that is easy to mis-file as the phantom class. The pilot
repeatedly plans "cast Abominable Treefolk immediately, 6/6 Trample" — s19 (T7), s30
(T9), s66 (T15) — but **Treefolk was NEVER offered as a castable option all game (0
offers, 0 casts in vs44).** The pilot correctly says Treefolk is "in hand," never "on the
battlefield," so this is NOT on-board hallucination. It is plan-diverges-from-legality /
stale-plan poisoning driven by a **manabase strand**: Treefolk needs {2}{G}{U} and the
deck could not assemble U. The plan re-affirmed a cast the engine never offered, across
many turns, while the pilot got ground 20→-10. Route: manabase construction flag +
stale-plan (model), NOT the board-read anchor (which fires on a false CURRENT board,
absent here).

## CENTRAL QUESTION 2 — Force of Negation calibration (wave-6 said 5/5 free over-fire)

**LARGELY RESOLVED. Force fired 10× this corpus; 9/10 targets correct or defensible — a
clean reversal of wave-6's 5/5-all-wrong.** Force counters NONCREATURE spells (all 10
observed targets were noncreature — consistent with the engine text). The guide's wave-6
sharpening ("spend it ONLY on a board wipe … or a spell that would kill your ONLY threat")
is now shipped in the live guide and is being OBEYED where sweepers exist:

| game  | seq  | target                                  | verdict |
|-------|------|-----------------------------------------|---------|
| vs140 | s60  | Black Sun's Zenith                      | CORRECT (sweeper) |
| vs140 | s72  | Pyroclasm                               | CORRECT (sweeper) |
| vs140 | s81  | Damnation                               | CORRECT (sweeper) |
| vs140 | s90  | Lightmine Field                         | CORRECT (damages all attackers ≈ sweep) |
| vs140 | s101 | Staff of Nin                            | marginal (value engine, defensible vs control mirror) |
| vs140 | s113 | Pyroclasm                               | CORRECT (sweeper) |
| vs140 | s114 | Black Sun's Zenith                      | CORRECT (sweeper) |
| vs140 | s127 | Venser's Journal                        | marginal (value/lifegain permanent) |
| vs44  | s74  | Go for the Throat targeting Ohran Viper | DEFENSIBLE (protects a threat) |
| vs110 | s45  | Galvanic Blast targeting me             | **OVER-FIRE** (small burn to face while mana-screwed) |

- The 8 vs140 fires are the correct matchup for Force (deck140 is a Damnation/Pyroclasm/
  Black Sun's Zenith control deck); 6 are literal board wipes, 2 are marginal value
  permanents. This is Force working as intended, and it is a major reason vs140 went to a
  timeout stalemate (see Q3 clock note).
- The ONE clear over-fire is vs110 s45: exiling a blue card to Force a Galvanic Blast (to
  face, ~1-4 damage) at my9 with no untapped sources, in a game already lost to color
  screw. "Free" still occasionally reads as "always fire" against small burn — the guide's
  anti-fast-aggro caveat is present but did not bite here. Minor; tie tighter to the named
  case (see strategy.txt). NOT a promote-to-core item.

## CENTRAL QUESTION 3 — Rule #1 (deploy-before-value) obeyed? residual = construction?

**OBEYED, still healthy, no overshoot. Residual IS construction (color screw) + slow clock.**

- **Creature offered → taken: 20/23 (87%)** across the 6 games (vs110 2/2, vs140 6/7,
  vs131 6/8, vs133 3/3, vs109 1/1, vs44 2/2). Icehide Golem taken on sight; Ohran Viper,
  Ice-Fang Coatl, and (vs131) Treefolk all deployed when offered.
- **Direct conflict windows (a creature AND a value/fetch/dig BOTH castable same
  decision): creature chosen 9/12.** The 3 misses are value-over-creature "dig for more"
  rationalizations, NOT overshoot:
  - vs140 s2 (T0): cast Arcum's Astrolabe with Icehide Golem also in list — marginal
    (Astrolabe enables the whole curve turn 0).
  - vs131 s23 (T8) and s28 (T10): cast **Glacial Revelation** while **Diamond Faerie (3/3
    flyer)** was a castable creature — the pilot dug "to find more threats / fix mana,"
    then cast Diamond Faerie at s29. Two turns of dig delayed a clock in a game it lost at
    opp 5. Mild residual edge of the durdle: a castable creature still occasionally loses
    to a "dig to find a BIGGER threat" excuse.

**Residual loss attribution — construction/clock, not durdle, not seam-passivity:**
- **vs109 LOSS (opp12):** only 1 creature offered ALL game (Icehide Golem). Ice-Fang Coatl
  / Ohran Viper stranded — could not assemble {G}{U} / {1}{G}{G} vs a fast RB clock. Color
  screw.
- **vs110 LOSS (opp20):** 2 creatures offered; opp never fell below 20. Color/tempo screw
  vs affinity+burn. Deck never got a plan running.
- **vs44 LOSS (opp18):** Treefolk never castable (0 offers, manabase strand); only Golem +
  Viper deployed; slow chip clock while ground 20→-10.
- **vs131 LOSS (opp5):** actually DEPLOYED a 15/15 Treefolk (T16) — but too late; already
  at 1 life after durdling to 15 snow permanents. Slow clock, worsened by the 2
  Glacial-Revelation-over-creature misses.
- **vs140 TIMEOUT (behind 15v26@T25):** control mirror; Force countered every sweeper but
  the tiny board could not close through opp lifegain. Slow clock + Force stalemate; 144
  decisions inflated by the fetch re-offer tax (Q4).

Diamond Faerie is **no longer zero-offer**: offered 3× / cast 1× in vs131 (the T17 grind,
once the deck reached 15 snow permanents + Astrolabe fixing). It remains rare (1 of 6
games, longest game only) and is never a plan-around card — keep the demoted one-line
treatment, but the wave-6 "dead weight / offered 0 two waves running" claim is now
inaccurate.

## CENTRAL QUESTION 4 — fetch re-offer loop (engine fix still pending; fresh repro + calls-per-crack)

Two distinct costs, both from the engine re-offering a fetch-crack activation at every
priority window a fetch land is in play. options_text greps make both trivial to spot:
verbatim strings `"Put in Play with Misty Rainforest targeting Snow-Covered …"` and
`"search basic land with Prismatic Vista targeting Snow-Covered Forest [cost: …]"`.

**(a) Calls-per-actual-crack — IMPROVED to ~1-2.** Consecutive identical-crack re-picks
are now at most x2 (vs110 s2, vs140 s87, vs131 s2). The wave-6 3-4-deep re-pick chains
(vs44 s111→s113) did NOT recur. So when the pilot actively cracks a fetch it now costs
~1-2 model calls, down from wave-6's 3-4.

**(b) Held-fetch re-offer TAX — dominant cost, unchanged and large.** Every priority
window with an uncracked fetch in play re-offers its crack, and the pilot must process it
even when it correctly declines. Windows where a fetch-crack was offered and NOT taken:

| game | held-fetch re-offer windows |
|------|-----------------------------|
| vs140 (TIMEOUT) | **97** |
| vs44  | 71 |
| vs133 | 51 |
| vs109 | 49 |
| vs110 | 44 |
| vs131 | 5  |

vs140's 97 re-offer windows are a leading contributor to its 144-decision timeout. This
is an **ENGINE-LEDGER item** (not guide-fixable): the crack activation should be consumed/
withdrawn once chosen, and a held fetch's crack should not re-prompt every priority window.
See notes.md.

## WAVE-6 LEDGER STATUS
FIXED / CONFIRMED:
- **Phantom-lethal board-read anchor** — held; 0 recurrences; the vs131 offender now casts
  Treefolk correctly.
- **Force scoping** — shipped guide line obeyed; 9/10 fires correct/defensible.
- **Rule #1 deploy-before-value** — obeyed 87% (20/23), 9/12 conflicts, no overshoot.
- **Fetch consecutive re-pick chains** — down to ≤2 calls-per-crack.

PERSISTING / NEW:
- **Manabase strands 2-color creatures** (vs109, vs44, vs110) — standing construction flag;
  the deck's win creatures are gold/2-color off a green-heavy, blue-thin base.
- **Slow clock** — even the loss/draw games reach T17-T25; tiny bodies chip, Treefolk lands
  too late. Feeds every timeout.
- **Held-fetch re-offer tax** (44-97 windows/game) — engine ledger, unchanged.
- **Value-over-creature dig misses** (vs131 Glacial Revelation ×2) — mild Rule #1 edge.
- **Force over-fire residual** (vs110 Galvanic Blast) — 1/10, minor.

## MODEL-QUALITY RESIDUALS (route to model-experiments track)
- **Stale-plan around an uncastable card** (vs44): the pilot re-affirmed "cast Treefolk
  immediately" across many turns while Treefolk was never offered (manabase strand). The
  plan diverges from legality every window; the board/hand distinction is correct so it is
  NOT the phantom-board class. Litmus case for stale-plan-vs-menu reconciliation.
- No on-board hallucination, no desyncs, no numeric-offset menu traps, no degenerate
  blocker replies.

## GAME → FILE MAP (run = matchups-20260713-222831)
| jsonl epoch | opp | outcome | turns | final (me v opp) |
|-------------|-----|---------|-------|------------------|
| 1784000115 | vs110 | LOSS    | 9  | -1 v 20 |
| 1784001280 | vs140 | TIMEOUT | 25 | 15 v 26 |
| 1784001601 | vs131 | LOSS    | 17 | -3 v 5  |
| 1784003384 | vs133 | WIN     | 12 | 16 v -1 |
| 1784003680 | vs109 | LOSS    | 9  | -1 v 12 |
| 1784005195 | vs44  | LOSS    | 18 | -10 v 18 |
