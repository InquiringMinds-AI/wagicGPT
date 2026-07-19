# Deck-135 wave-20 — general suggestions (cross-deck / model / corpus items; NOT guide text)

Routed OFF the guide per layer-routing doctrine. deck135-observed but general in nature.

## #1 (MODEL / DECODE, DOMINANT residual, carried) — degenerate REPEAT-LOOP replies + salvage gap + a HIDDEN member.

Still the single most common failure shape at this seat: 7 fallbacks, all decode-time repetition spirals
(>12k chars), all `unparsed_reply`, ALL in the two LOST games (5 in vs27, 2 in vs49), NONE flipped a winnable
game. salvageLoopedChoice rescued 0/7 (keys on `CHOICE:`/`ATTACK:`; these spirals enumerate-and-reject every
option and never emit a clean trailing line). NEW this wave: at least one MORE loop is hidden from the fallback
tally — vs35 s10 parsed "cleanly" to a RETRACTED false-start CHOICE (see notes HARNESS-1), so the real
decode-loop tax is >=8. Two mitigations, both model/decode (NOT guide-fixable):
  (a) a decode-time repetition-penalty / max-token guard (the loops repeat a paragraph verbatim ~8x);
  (b) parser: when a reply contains a repetition signature, prefer the LAST well-formed CHOICE line and treat a
      pre-"Wait, I made a mistake" CHOICE as retracted — do not lock in an early false start.

## #2 (ENGINE representation, carried — mostly CLOSED at this seat, one residual) — eligibility surfacing on reveal/search choosers.

The wave-19 ask (offer only legal targets / annotate each card's eligibility on reveal-and-search choosers)
SHIPPED and fired: Glacial Revelation and Into the North reveals now carry `[eligible]` / `[does NOT qualify]`
marks + an `ELIGIBILITY:` header, which removed both wave-19 reveal fallbacks and all the reveal-comprehension
spirals at this seat (3/3 Into the North reveals now parse short and correct). CLOSE the eligibility-marking
half. Two residuals remain, both LOW priority:
  (a) The Into the North effect is still LABELLED as a whole-library "choose card / put back" menu rather than
      "search a snow land to battlefield" — cosmetic now that eligibility is marked (the model picks correctly
      off the marks; vs14 s12 even mislabeled the effect "Glacial Revelation" and still picked right). Optional
      polish: relabel the verbs + offer only the eligible snow-land targets.
  (b) The Azcanta ACTIVATION path (a `-land;-creature` to-hand restriction) had ZERO windows this corpus, so its
      eligibility marking is UNVERIFIED and the "is R1's all-sweep also in the activation optiontwo?" question is
      UNTESTED. Not vindicated by absence — keep open (notes ENGINE-R4).

## #3 (CORPUS, carried) — GPU-latency LIFE-adjudication as tiebreak; deck135 still adj-heavy.

deck135 sits in 2 of this corpus's 5 adjudicated games (vs14 adj-WIN ahead 15/3; vs49 adj-LOSS behind 2/14).
Adjudication itself is not a bug (brief owns) and it did not distort a result here — vs14 was clearly ahead,
vs49 clearly behind. It continues to CONFIRM the under-powered-clock construction flag (games that reach the cap
rather than closing). Noted for completeness; not a deck135-fixable item. Latency p90 78s is Spark queueing
under 3 concurrent games (brief), not a model-quality signal — the two reveal-heavy games still resolved fine.

## #4 (METHOD, cross-deck) — a fixed engine bug can be VALIDATED-CORRECT yet contribute ~zero to the win column.

Cross-deck reviewer caution surfaced by this seat: ENGINE-R1 (the wave's headline reveal fix) is validated
correct, but Glacial Revelation was drawn/cast only ONCE this corpus and that game was lost, so the fix moved
the record by ~0. Do not credit a validated engine fix with a win-rate jump without checking how many windows
actually exercised it (the rare-event-A/B-power discipline). Here the +2 wins attribute cleanly to a matchup
slate swap (deck110 out -> deck102 in) + one reveal-free variance flip (vs62), NOT to the engine batch. The
repeated-opponent control (5 of 6 opponents identical wave-over-wave) is what makes that attribution airtight —
use it whenever a rotation keeps most of the slate fixed.
