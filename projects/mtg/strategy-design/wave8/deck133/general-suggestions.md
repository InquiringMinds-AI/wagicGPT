# deck133 wave-8 — core-prompt / representation proposals (layer-routed)

The burden of proof is ON core: every deck pays for each line. **deck133 produced NO core-prompt
proposal this wave.** The corpus was clean (0 desyncs, 0 fails-validation, 0 defer, no no-op decided
a game), and the one guide change was a DELETION driven by a shipped engine fix, not a new leak. Two
routing notes below (both ENGINE/REPRESENTATION, not the prompt body) and the standing construction
flags.

## 1. [REPRESENTATION -> ENGINE — SHIPPED, now verified at the seat] Gray Merchant drain display fix CONFIRMED; guide stopgap deleted.
The wave-7 ask (make the GM drain option show the RESOLVED total, including GM's own +2) has SHIPPED
and is confirmed at this seat: vs109 seq36 showed `{right now: drains 8}` and resolved as exactly 8
(devotion 6 from other pips + GM's own 2, display included the 2). The per-deck guide's "read N, add
2" workaround is now DELETED (it would over-count by 2 and mis-evaluate lethal / the <=10 stabilizer).
No further core/engine action — recording the close of the loop so a future wave does not re-open it.
If any OTHER deck's guide carries a Gray-Merchant "+2" note, it should be deleted in the same sweep
(this is the display-fix propagation the stopgap-lifecycle triple anticipated).

## 2. [EVIDENCE-TOOLING — reviewer-facing, still open] Surface the RESOLUTION outcome of a consumed cast in the translog.
Re-flagging the wave-7 harness/evidence ask (NOT player-facing, NOT the prompt): a reviewer still must
scrape the next record's narration tail to tell whether a cast RESOLVED vs was countered/fizzled. The
new `events` field helps (it carried the GM resolution `Your life +8 / Opponent's life -8` this wave,
which is how the +2-fix was verified), but an explicit per-record
`resolved: battlefield|countered|fizzled|graveyard` would remove the manual scrape and prevent
win-mechanism mis-attribution (the wave-7 vs44-seq27 countered-GM trap). Harness/evidence ledger.

## 3. [NON-ITEM — explicitly NOT core] Thoughtseize empty-hand discipline stays PER-DECK.
The two past-T3 Thoughtseize-into-<=1-card-hand disobediences are per-deck posture (a control deck
sometimes WANTS to strip the last card), and both were in already-lost construction spots. NOT a core
edit. If it recurs in a deciding spot, the fix is REPRESENTATION (surface opp hand size at the cast
decision), not a core sentence — recording only to argue the routing.

## Standing DECK-CONSTRUCTION flags (RE-CONFIRMED from wave-7; user owns the decklist)
Both losses re-confirm wave-7's structural residuals — set as expectations, do NOT fix with guide length:
- No early blocker; Bloodghast (the T2 body) CANNOT block -> both losses were races lost with no wall
  drawn early (vs110 affinity Memnite, vs44 faerie flyers). Consider a cheap early blocker.
- No reach / no flyer answer except Gray Merchant (density 4/60, no selection); GM reached the list in
  only 1 game this wave (won it). Consider +1 GM or cheap card selection.
- Discard suite self-damage (Thoughtseize 2 + fetch 1s) is the -1/-0 margin in the close races.
