# deck93 general-suggestions (core-prompt layer) - Step-0 debut

## CORE PROMPT: PASS (22nd consecutive)

deck93 routes core to PASS. No core-prompt body edit proposed. Reasoning:

- 99 decisions, 0 fallbacks - the reply protocol, parser, and combat/mulligan seams all
  held at the most duplicate-name-dense seat in the pool. The d1 fix works (see findings
  Part 1); nothing in the shared prompt needs changing to support it.
- The two combat leaks (L2 phantom slots, L3 over-blocking) are DECK-GUIDE issues, not
  core gaps. Specifically, L3's failure - trading the board away while ahead against a
  1-life opponent - happened DESPITE the core already rendering the correct line
  "Unblocked, these attackers deal up to 7 - you would be at 12 - NOT lethal: block only
  where the trade favors you; taking damage while ahead is often correct." The core line
  is present and correct; the model failed to adhere. That is a per-deck reinforcement
  lever (strategy.txt BLOCK section), not a symmetric core change. Adding more general
  block-restraint text would not have helped and risks over-tuning the symmetric prompt.
- The L1 mulligan land-blindness is a REPRESENTATION option (see notes.md N-A) plus a
  deck-guide fix (strategy.txt Rule #1), not a core-prompt change - the core prompt does
  not own basic-land rendering.

## Bar note
The 22nd-consecutive PASS bar is high; this seat clears it cleanly (0 fallbacks, no
core-attributable misplay). All deck93 misplays route to guide or ledger.
