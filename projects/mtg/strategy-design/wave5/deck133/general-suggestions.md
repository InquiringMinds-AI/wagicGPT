# Wave-5 deck133 — proposed changes to the shared core prompt / general strategy

Base: strategy-design/wave4/general-strategy.txt (as absorbed into
bin/Res/ai/gpt/system_prompt.txt). Burden of proof is ON core — a line is
proposed here only where the failure showed up deck-AGNOSTIC. From this single
seat, that burden is mostly UNMET; most items route per-deck or to representation.
Kept separate from the deck-specific asks (those are in strategy.txt / notes.md).

## No new core DOCTRINE proposed. Two routing observations:

### 1. Plan/list disconnect persists despite the existing core line — route to REPRESENTATION, not more prose.
The core already carries "write your plan FROM the list… a missing card is never a
reason to choose Cast nothing." It STILL fired: vs110 seq26 (1783980403), the pilot
planned "Cast Gray Merchant immediately," Gray Merchant was not in the list (only 4
lands), and it chose "Cast nothing" over an available Fatal Push. Per the skill's
layer-routing (a core line present but losing routes to representation, never a
second paragraph of the same instruction), the fix is NOT louder core text — it is
a **structured pre-choice step that forces the reply to name which LISTED option
number its plan uses** before free-text planning. Recommend the model-experiments /
reply-protocol track evaluate a pre-choice extraction; do not add core wording.

### 2. Impact-ranked act-floor is already in core — the miss is deck-specific selection, keep it per-deck.
The vs109 discard flood (casting the cheapest spell to satisfy "act every turn"
while being raced) is an application failure of core's existing "choose by IMPACT"
+ "cast something, starting with your win condition or engine." What counts as
low-impact (discard vs a near-empty hand) is deck-specific, so the fix is the
per-deck impact-ranked cast list in strategy.txt, NOT a core change. Flagging only
so the synthesis agent sees the same "cheapest-cast satisfies the floor" shape and
can decide if it recurs across decks (if 3+ decks show it, a core line "the
act-floor is satisfied by your HIGHEST-impact legal play, not the cheapest" would
be justified — from one seat it is not).

## Representation ask that would remove a guide stopgap (see notes.md E2).
The Gray Merchant option displays `{right now: drains N}` where N under-counts the
card's own devotion by 2. This is a general property of any devotion/board-scaling
payoff shown before it resolves. If the engine renders the drain as-if-resolved
(including the card's own contribution), the guide's "real drain = N + 2" stopgap
(and any similar per-deck correction on other devotion cards) can be deleted at
once. Engine/representation layer, not the prompt body.

## Threshold reuse (per the skill's reconcile-by-reuse rule).
The deck-guide's Gray-Merchant stabilize trigger reuses the core's existing
**8/10-life burn-at-face threshold family** rather than minting a new number: it
fires GM-to-stabilize at "your life 10 or less," matching core's "once the
opponent is at 8 life or less always fire damage at their face" register and the
blocking "8 or less" survival threshold. No new numeric constant introduced.
