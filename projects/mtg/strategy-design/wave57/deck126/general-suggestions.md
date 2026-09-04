# wave-57 core-prompt / render proposals — deck126 seat

Layer routing: engine DEFECTS live in `wave57/seats/seat-125-126.md`'s engine list (HIGH #1 the
`rowSaysNoOp` verb whitelist, HIGH #2 the trample-clamped best-case, MED #5 the retracted reply,
MED #6 the land-drop async arm). Below is prompt COMPOSITION and answer-channel UX only.

## #G6 — THE ANSWER CHANNEL: a naturally-terminated reply's LAST `CHOICE:` is its considered one

`126v146` seq 34 (turn 20, 2 life) is the corpus's only `retracted_choice` and it is the seat's
last live window. Line 1: `CHOICE: 3 (Cast Staff of Nin)`. Then ~900 words that correctly reach row
1 (`Cast Exquisite Blood`, the half that completes the pair, affordable at 7 sources, and NOT the
one the opponent's Silencer had named for 3 life). Last line: `So CHOICE: 1.` The engine refused,
stamped `choice: -1`, recovered, and the seat died.

The refusal is a defensible policy and I am not calling it a bug. The observation is about what it
costs and what it discards: the model's FIRST line is its snap answer and its LAST is its worked
one, and on this reply the worked one was right. Proposal for the core loop: on a reply that
terminates naturally and carries exactly two `CHOICE:` lines both naming legal rows, take the
LAST and stamp it (`choice_revised`), reserving `retracted_choice` for replies that are truncated,
contradictory across more than two answers, or name an illegal row. Rate is 1 in 3,317; cost per
occurrence is a whole decision. Falsifier: D125 in the seat file.

Second-order note for whoever owns the protocol: this failure is manufactured by the answer-first
contract, which asks a prose reasoner to commit before it reasons. The guide can only mitigate it
(edit D57-2) and the skill proposal is #S9. If the protocol ever gains a `THINK:` line before
`CHOICE:`, this whole class disappears — but that trades prompt discipline for tokens and is the
owner's call, not a reviewer's.

## #G7 — THE PLAN ECHO IS BEING ANSWERED AS IF IT WERE THE MENU

All THREE `[RE-ASK]` notices in this corpus are mine, and all three read
`[RE-ASK] "Cast Sanguine Bond" is not on this list. Answer with a number from 1 to N.`
(`126v130` seq 10, `126v146` seq 31, `126v162` seq 13). In each, the seat's echoed
`YOUR PLAN (as you last stated it ...)` header said to cast Sanguine Bond, and in each the Bond was
on the HAND line with no castable row. The notice works — 2 of 3 recovered to a printed row — but
the source of the error is the same every time: the plan names a card, the menu does not, and the
pilot answers the plan.

The re-ask emitter is correct and I am not asking for a change to it. Two cheap composition ideas,
in preference order:
1. **Add one clause to the notice saying WHY**: `... is not on this list (it is on your hand line;
   nothing in this window can cast it)`. That is the fact the pilot is missing, it is already
   computable from the hand line the same prompt prints, and it is one clause on a string that
   renders 3 times per corpus.
2. The plan echo could carry its own staleness marker when a card it names has had no castable row
   for N consecutive windows. This is more speculative and I do not have a denominator for it —
   3 re-asks is not enough to justify a new latch. Recorded as an idea, not a request.

## #G8 — THE ALL-DEAD TOKEN IS THE STRONGEST RENDER RESULT AT EITHER OF MY SEATS, AND IT IS UNDERCOUNTED

`NO LIVE CAST ROW ON THIS MENU` rendered on **405 prompts** across my two seats and **0 casts were
made off any of them** — the largest clean denominator in this seat file. The wave-57 docket's own
grep read 0 corpus-wide, so the literal is worth restating for the next census: it is a header
token under the question, not a row annotation.

The undercount is structural and lane C flagged it in advance: the predicate requires EVERY cast
row to carry a `{right now:}` verdict, so deck126's bracketed dead markers (`[second copy: ...]`,
`[finds only an enchantment card - every enchantment left in your library is a copy ...]`) never
let the token fire — and those are exactly the menus this seat spent cards on (1 dead tutor take,
2 second-copy takes, all three in games this seat lost or nearly lost). HIGH #1 in the seat file
widens the same gap from the other side, since a zero-magnitude Final Judgment reads as live.

Ask, in the lane's own terms: the bracketed markers are already computed from the board with the
same certainty as `{right now:}` and could feed `everyCastRowDead` without changing what any row
prints. That is an append-only extension of a shipped, verified feature to the menus where this
seat actually loses cards. **Core-loop decision quality, no row removed, no window closed.**

## #G9 — WHAT THE CONTROL SAYS ABOUT EVERY RENDER PROPOSAL WE SCORE

Three corpora of the zero-hold-text control put a number on something the core loop should carry:
a correct, well-priced, prominently placed row that the guide does not name is taken at **0.6%**
(0 of 336 opponent-turn windows this corpus) where the taught version is taken at **19.8%**.

This is not an argument against render work — every ceiling in this seat file is a render the guide
DOES name, and lane C's D8/D12 and lane B's D6 all moved real behaviour. It is a calibration for
how we score proposals: **"the pilot will see it" is worth roughly a twentieth of "the pilot is
told to read it".** A render that no guide will reference should be justified by legality,
correctness or cost — not by expected uptake.
