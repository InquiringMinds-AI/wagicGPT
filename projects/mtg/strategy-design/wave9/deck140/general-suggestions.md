# deck140 wave-9 — general (cross-deck / core / infra) suggestions

Routing note: nothing here is a deck140 guide edit. The single guide diff (boundary disambiguation +
invented-anti-pattern clause) is in strategy.txt. These are for the general/core/infra layers.

## 1. [INFRA — highest priority] Adjudicate long games by life-at-cap; the timeout column is fake
All 3 deck140 timeouts are latency-starved games deck140 was AHEAD (36-13, 18-3) or even (25-29) in,
with 60-103 decisions and ~21-23 min cumulative model latency. They are NOT losses and NOT play
failures. Two independent fixes, either resolves it:
  (a) a decision-count / stalemate cap that ADJUDICATES BY LIFE at the cap instead of scoring a
      timeout as a loss (results.tsv already fills life/turn at the cap — the data is there);
  (b) run quality-sensitive matchups at `-j 3` or a raised `WAGIC_GPT_TIMEOUT` so wall-clock does not
      truncate a correctly-played grind. Median latency ~10 s x 100 decisions = ~17 min of pure
      inference before spikes; the cap is decision-density x latency, nothing else.

## 2. [REPLY-PROTOCOL A/B — now double-witnessed] Schedule the answer-before-reasoning fix
deck140 joins deck135 as a witnessing seat for intent-collapse (head token emitted before the PLAN
reasons; PLAN concludes a different same-window action; head never revised). In deck140 it self-corrects
on the next priority window, but it wasted a sweeper into an empty board (vs131 t40) and briefly emitted
"Cast nothing" at 1 life before recovering (vs109 t10). Two seats clear the bar the wave-8 note set for
the durable fix. Recommend the reply-protocol A/B: accept the LAST bare number in the reply, OR move the
CHOICE token to AFTER the PLAN. Judge by decision-quality (symmetric core), not win-rate. Route to the
code-appended reply-protocol layer, never the guide.

## 3. [DECK CONSTRUCTION — user owns] deck140 reach starvation + no closer (restated, not relitigated)
The entire loss column is one fact: deck140 sweeps faithfully and cannot close before it dies. Options,
rough priority (unchanged from wave-7/8, now better-evidenced across 3 losses + the vs131 grind):
  - a cheaper or second recurring clock so the deck closes before the control-mirror cap AND has a
    threat #2 after a counter (Blightsteel {12} folds to a single Essence Scatter / countermagic);
  - a 1-2 mana early speed bump / lifegain vs aggro (Lightmine x4 + Venser's x2 arrive too late to
    survive turns 4-8 against mono-R/mono-B/faeries);
  - a couple more UNTAPPED red sources so Pyroclasm {1}{r} is a reliable turn-2/3 cheap sweeper
    (repeatedly the pilot had W/B and no red and had to route to Wrath/Damnation/BSZ).
14 sweepers is the right density — the deck needs TEETH, not text. Do NOT answer reach starvation with
guide prose.

## 4. [CORE — no change] Shroud candidate cannot advance; no data
The wave-8 shroud-vs-non-targeted core candidate needed a 2nd seat / shroud board to advance. This
corpus produced ZERO shroud creatures for deck140, so no new signal. Keep the deck140 shroud guide line;
no core promotion. (Do not read "no data" as "resolved.")
