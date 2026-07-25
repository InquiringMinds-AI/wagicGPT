# deck59 — engine / representation / data notes (wave 25, Step-0-ter)

## ROTATION VERDICT: ROTATE OUT (converged; unsentimental)
deck59's initial guide LANDED (3/6 guideless -> 6/6 guided). Both idiosyncrasy levers are
VALIDATED at the seat: the hybrid-mana veto/mulligan-anxiety COLLAPSED (rule #1), and the
persist + one-sided-sweeper GO-gate fired at the exact wave-24 loss spot with the recursion
WHY and correctly held while ahead (rule #2, the loss lever, now closed both directions).
The guide is BYTE-COPIED unchanged (cmp-clean vs both the deployed guide and wave24) — NO
guide-mod this wave. There is NO new GUIDE-lane work signal: the two new items are (a) an
engine/representation ledger item (Restless pump-option rendering, below) and (b) an
outcome-neutral watch (the all-Swamp over-count, below), neither a seat that needs
re-piloting. Per the brief's endgame instruction: deck59 is a converged seat and rotates to
canary. Its single open item is ENGINE-lane, not a pilot problem. If the loop wants a next
first-guided rotate-in, deck59 is done; if the guided roster is exhausted, this is one more
seat confirming the termination condition (validated guide, no residual guide work).

## ENGINE / REPRESENTATION LEDGER

### R-RESTLESS-PUMP-OPTION-AMBIGUOUS (NEW, deck59 — largest residual spiral at this seat)
The Restless Apparition activated-pump option renders:
  `3/3 with Restless Apparition [cost: {w/b}{w/b}{w/b}] {card text: "{W/B}{W/B}{W/B}:
   Restless Apparition gets +3/+3 until end of turn. -- Persist (...)"}`
The `3/3` PREFIX contradicts the `+3/+3` card text and is not the resulting P/T (a 2/2 or a
persisted 1/1 pumped +3/+3 is 5/5 or 4/4, never 3/3). The model cannot reconcile the prefix
with the card text and spirals: "Why does the option say 3/3? ... the card text says +3/+3
... maybe the option text is just poorly phrased." This drives 4 of the corpus's 5 largest
PRIORITY spirals: deck133 s16 (6.8k), deck133 s19 (13.0k), deck27 s18 (7.5k), deck27 s21
(7.6k). Non-deciding (every pump resolved correctly; 6/6), so a latency/verbosity tax, not
a misplay. Suggested engine fix: render the activated-pump option with an unambiguous
effect descriptor, e.g. `Activate Restless Apparition: +3/+3 until EOT (2/2 -> 5/5) [cost:
{w/b}{w/b}{w/b}]`, dropping the misleading standalone `3/3`. Repro: any priority window
where Restless Apparition is on the battlefield with 3+ open mana (deck133 s16/s19, deck27
s18/s21). Sibling class to the wave-24 persist-narration gaps — a representation defect the
option-annotation surface can eliminate.

### b6 CONFIRMED LIVE (wave-24 gaps CLOSED-BY-FACT) — no action
- Persist RETURN now narrated: `- Your <Card> returns to the battlefield with a -1/-1
  counter (persist)` (deck131 s6, s31). Wave-24 notes #1 CLOSED.
- Block trade-annotations now carry the return clause: `(both die (yours returns with a
  -1/-1 counter (persist)))`, `(you kill it, your blocker lives)`, `(your blocker dies,
  attacker lives (yours returns with a -1/-1 counter (persist)))`. Wave-24 notes #2 CLOSED.
- Decision effect verified: 11/12 block decisions cite the return; blocks made because the
  return is visible (vs109 s13 "trade and trigger its Persist"; vs27 s33 chumps a
  first-striker for the return). Do not re-diagnose either as a gap in a future wave.

### First-strike + persist sub-confabulation (minor, deck27 s33) — flag, not diagnose
At the annotated block `Heartmender ... (your blocker dies, attacker lives (yours returns
with a -1/-1 counter (persist)))` the reply floats a false aside: "if Heartmender dies and
returns before the normal damage step, it can deal damage." A returned persist creature
does NOT deal the combat damage it was assigned before dying; against a first striker it is
dead before the normal damage step regardless. The annotation itself is CORRECT and the
block choice was correct — this is a model rules-confusion aside that did not change the
decision. No engine action; noting for the b5 (wither/first-strike-ordering) watch that a
first-strike-vs-persist annotation can seed a "returns and still deals damage" mis-belief.

## DATA-INTEGRITY (positive vs wave 24)
All 6 deck59 games have a `gameend` record in the deck59 log this corpus (wave-24 had 3
missing). Win table 6/6 fully corroborated from the logs: vs131 24/-8 T15, vs102 15/0 T13,
vs109 13/-5 T13, vs133 13/-4 T13, vs27 4/0 T23, vs137 18/-4 T11. The wave-24 missing-gameend
observation does not recur at this seat.

## CARD-SCRIPT / ORACLE (unchanged from wave 24; no action)
All ten cards remain engine-faithful to Oracle as rendered (persist, wither, fear, Kitchen
Finks ETB lifegain, Heartmender per-upkeep -1/-1 removal from EACH of your creatures,
verified via persisted-body P/T recovery across upkeeps; the three sweepers). Costs diverge
from paper only in the expected hybrid-notation way; all castable off W/B. No card-script
bug this seat.

## RULES-SHIFT WATCH (batch A) — no impact at deck59
No planeswalkers (a4 redirect-menu removal: no records), no battles (a5), no day/night or
city's-blessing markers (a6). Turn-1 upkeep (a1): deck59's only upkeep trigger is Heartmender
(no-op turn 1 with no creatures). No anomaly, no stale expectation of the removed redirect
menu observed corpus-wide.

## MODEL / LATENCY
Zero fallbacks at deck59 (0 unparsed / empty / retracted / stale_echo). Reply lengths at
this seat: 5 games p50 ~1.0-1.1k, the vs27 grind (T23) is the long tail (max 12.4k). Giants
>6k = 10 across all decision kinds (5 priority = the Restless pump ambiguity, 3 blockers +
2 ask = tight-game combat/sweep math in vs27 at myL4). Latency max 191s (vs27 s30, the 12.4k
sweep spiral). No decode-collapse, retry fired 0x — spot-checked the longest replies, all
coherent coded lines, no garbage. The mana-derivation share of the tax is gone; the residual
is the pump-option ambiguity (engine-lane) + genuine hard combat math in the one long game.
