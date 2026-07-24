# deck27 general-suggestions.md — core-prompt proposals (wave-24)

Layer-routing enforced: single-seat items CANNOT add or cut a core line; engine/representation, decode
and parser items live in the notes.md ledger, never in core prompt text. **This seat proposes NO
core-prompt change (continuing the consecutive-PASS streak).**

## Core prompt: PASS (no change proposed)

Nothing this seat found is both (a) 2+-seat convergent AND (b) properly core. Detail:

1. **Bottom-phase guidance — PER-DECK GUIDE, not core.** The vs137 commit-lock is fixed at deck27's
   guide layer (keep color-fixing, never bottom your only color source, decide-once-on-line-1). If
   synthesis sees bottom-phase misplays across MULTIPLE decks, a core "on the bottom decision, keep your
   mana base and cheapest plays; put your final list on the first line" could be warranted — flag as a
   cross-seat watch, not core text on N=1.

2. **First-strike block caution — PER-DECK GUIDE, not core.** Added to deck27's blocking section (trust
   the annotation; a same-power first striker chumps your blocker; never chump a lord into it). It is a
   real-blocker-deck concern. Only if synthesis sees first-strike misreads across multiple real-blocker
   decks would a core blocking line be warranted — cross-seat watch, not core on N=1.

3. **Commit-lock parser (prefer last coded line, non-truncated) — ENGINE/PARSER, not core.** notes.md #1.
   A live harmful instance validating the wave-23 proposal. Sampling/parser owner. No core text.

4. **Wither-vs-first-strike annotation clause — ENGINE/representation, not core.** notes.md #2. The
   "(wither shrinks it to 0/0)" clause is wrong under first strike. Not core.

5. **Verbosity/latency + max-tokens fast-cap — MODEL/SAMPLING + HARNESS, not core.** notes.md #3/#4. The
   decode-collapse class did NOT recur; verbosity did (parseable over-reasoning, 60-180s). No core prose
   prevents it. Recorded as cross-seat data for the synthesis pilot/latency verdict.

6. **Late-stall durdle revision — GUIDE off-case, unexercised (5th corpus).** Owns the guide layer;
   trigger did not occur. Not core.

7. **Matchup losses (vs102 Tourach, vs109 mono-R) — DECK CONSTRUCTION, not prompt.** notes.md #7/#8. No
   removal/reach; roster owner.

## Confirmations for the synthesis agent (no text change)

- **Trust-the-rendered-number held again with ZERO mana fallbacks** on the hybrid {U/B} base — Nth data
  point. Reinforces the existing rule; no new prose. (Note the ONE place the model overrode a rendered
  annotation this corpus — vs109 seq11 first-strike block — is now guide-addressed, and reinforces
  "trust the annotation" rather than contradicting it.)

- **CAUTION for synthesis:** do NOT read deck27's 4/6 -> 3/6 dip as any batch/guide regression — the
  guide was byte-frozen this corpus and the dip is variance/pool (vs109 draw-flip, vs102 tightening).
  Symmetric to wave-23's caution about not reading its UP-swing as the revision working.

- **DECODE input for the synthesis pilot verdict:** the wave-23 ahead-degeneration decode-COLLAPSE did
  NOT recur at this seat (0 fallbacks; no collapse signature). The answer-locked retry's 0 corpus-wide
  fires is consistent with "the class did not occur," not a missed detector — at least at deck27's seat.
  What replaced it is benign verbosity (parseable long reasoning) whose only real harm was enabling ONE
  commit-lock. If the retry is to earn its keep, the more common residual is now the commit-lock
  (wrong-but-parseable snap), which a retry does NOT catch — a last-line parser preference does.

## Engine/representation/decode/parser items produced (routed to notes.md, NOT core)
- Commit-lock in bottom phase: harmful, non-truncated; parser last-line proposal validated (notes #1).
- Wither-vs-first-strike annotation clause wrong (notes #2).
- Decode-collapse did NOT recur; verbosity->latency; max-tokens fast-cap candidate (notes #3/#4).
- Bottom-phase + first-strike GUIDE additions (notes #5/#6, shipped in strategy.txt).
- vs102/vs109 matchup construction notes (notes #7/#8, roster owner).
