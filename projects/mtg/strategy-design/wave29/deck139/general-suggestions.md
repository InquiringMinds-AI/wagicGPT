# General (core / general-prompt) suggestions -- deck139 seat, wave-29

## VERDICT: PASS (24th consecutive)

The core/general prompt held cleanly at the mutate seat across 155 decisions in 6 games:
- 0 fallbacks, 0 retries, 0 unparsed replies, 0 truncation-abandons at this seat.
- The CHOICE-number contract parsed correctly through the entire multi-ask mutate flow
  (cost-choice, normal/mutate, over/under, mutate-target, loot-discard, land-fetch) with no
  menu-drift or off-by-one. 1-based CHOICE numbering mapped to options correctly every time,
  including the two-word pseudo-options ("mutate over", "mutate under").
- The plan/PLAN-line carry worked; the "the actions your plan names are no longer among the
  options" advisory fired correctly when the mutate flow advanced past a stated plan
  (deck93 s22) and the model re-derived from the current board.
- Nothing in the mutate render defeated the parser -- the merged-creature two-line battlefield
  render and the mislabeled loot are COMPREHENSION problems (routed to notes/guide), not
  parse/contract problems. The core prompt is not implicated.

## One core-adjacent observation (NOT a fail; routed, noted here for the synthesis)

The core prompt does not bound reply length, and the mutate/combat surface at this seat
produced the campaign's steepest latency tail: six decisions at 195,000-199,000 ms and a
14,181-char reply one class short of the truncation cliff that fired deck148's f6 safety.
This is DECK-SPECIFIC stimulus (mutate stacking + double-strike/trample math invite deep
rules-lawyering), addressable at the RENDER layer (remove the ambiguity the model is resolving)
and the GUIDE layer ("decide combat fast, commit"), so it does NOT argue for a core change now.
BUT if future mutate/combat-heavy decks reproduce the >195k tail, a core-level "state your
CHOICE line first, then keep reasoning brief" nudge (or a soft reasoning-length ceiling) becomes
worth considering -- flagging for the synthesis owner as a WATCH, not a proposed edit. Consistent
with deck18's "6/6 hides taxes" audit-harder finding: the seat is parse-clean yet carries a real
latency/truncation tax the win column hides.
