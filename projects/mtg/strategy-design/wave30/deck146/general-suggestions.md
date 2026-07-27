# deck146 -> general (core-prompt) suggestions (wave-30)

## VERDICT: PASS (25th consecutive)

The core prompt did its job cleanly at this seat this corpus:
- The deciding-fact-rides-the-option architecture handled the first live PLANESWALKER play
  without a seat-specific patch: Lolth's loyalty menu rendered each ability with full {card text}
  and a [cost: Counters] tag, and the model read all three and picked correctly (-3 board first,
  +0 value after). No core change warranted from the walker surface.
- The g5 "Dungeons completed (you): N ... ACTIVE for you" status line (an ENGINE render addition,
  not a prompt change) landed and the model's reasoning flipped from second-guessing to confident
  assertion. Correctly an engine-ledger item, not a core-prompt text change.
- Dual-land mana line still renders true color reach (0 collapse lines / 6 games) a second corpus.

### The one fallback is NOT a core-prompt text defect
deck146's single unparsed_reply (vs152 s17) was a 210s over-reasoning spiral: the model's PLAN
said "play a land", the offered priority list held only a Flip-Side display toggle + pass, and the
model invented an off-menu "CHOICE: 2 (Play Plains)" then looped for ~3,000 tokens rather than
accept "lists are complete." The core prompt ALREADY carries that rule verbatim ("lists are
complete: if a spell is not in the cast list you CANNOT cast it") -- the model QUOTED it and
spiralled anyway. This is a model-behavior/latency phenomenon (worsened by the corpus's server
contention: median 16.8s, p90 73s, max 216s), not a gap in the prompt's text. No wording change
would beat a reasoning spiral; strengthening the rule risks over-constraining every other seat.
Routed to notes.md as a cross-deck model watch (the "invent an off-menu option matching my PLAN"
shape). If this shape recurs at multiple seats next corpus, revisit as a possible core item then.

No core-prompt change is warranted from this seat.
