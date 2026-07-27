# deck105 -> core (general) prompt proposals, wave 32

Layer-routing applied first. Of the five defects this seat found, four are RENDER/engine and go
to notes.md (N-105a/b/d/e). One is genuinely core-prompt text, and it is the first non-PASS this
reviewer would file — but it is filed as a **BOUNDED, ONE-CLAUSE** change, not a rewrite.

## G1 — the core prompt states a WIN CONDITION that is FALSE for any alt-win deck. (PROPOSE)

`AIPlayerGPT.cpp:63`, first sentence of the static system prompt, every duel, every seat:

> "You are playing a duel of Magic: The Gathering. You win by reducing the opponent's life from
> 20 to 0."

For deck105 this is simply wrong: **all six of its wins came at opponent life totals of 20, 21,
15, 23, 23 and 14** — none of them ever went below 14 — and the win in each case was 10 poison
counters. The sentence is not a harmless simplification at this seat; it is the ONLY
win-condition statement the pilot ever receives, it is asserted flatly, and it survives every
prompt of every game. The observed cost is the pilot repeatedly re-deriving which number it is
racing and getting it wrong (vs146 s23 t15, verbatim: *"the opponent has 21 life. I need 20
poison. I have 11. I need 9 more."* — the poison threshold conflated with the life total; vs146
s22 t14, 8,827 chars, ending *"If the game is still going, then my count is wrong."*).

The deck guide countermands it BY NAME (Step-5's override-the-generic-prior move), which is the
sanctioned per-deck route and is cheaper than a core edit. But this particular line is not a
prior that is "correct in general and anti-teaching for one archetype" — it is a factual claim
that is FALSE whenever an alt-win is in the pool, and the roster now contains at least one such
deck permanently.

Proposed minimal edit (one clause, no new paragraph, no new concept):

> "You are playing a duel of Magic: The Gathering. You win by reducing the opponent's life from
> 20 to 0 — **or by any alternative win or loss condition a card in play states; when a card
> says a player loses the game, that is as real as life reaching 0.**"

Why this form and not more: it removes the falsity without teaching poison, proliferate,
Thassa's Oracle or any other specific mechanic (which belong on card text, and i13 already
delivers that — the infect reminder text is the ONLY reason a guideless deck105 pilot played
infect correctly from turn 0). It also does not add a rule the pilot must apply; it withdraws an
absolute it was obeying. Cost: ~25 tokens on every prompt of every duel.

Recommended handling: **route to the synthesis agent as a PROPOSAL, not an adopted edit.** The
core has now passed 26 consecutive waves and a single-seat finding is a weak basis for touching
it; the honest test is whether a second alt-win deck (or the same deck after N-105a ships) still
shows the conflation. Two acceptable dispositions, in order of preference:
  (a) HOLD — carry the deck-guide countermand only, re-check at deck105's next corpus. If the
      guide's explicit "ignore that line" clause fires and the conflation goes to zero, the core
      line is doing no residual damage and the core stays at PASS.
  (b) ADOPT the clause above if N-105a ships and the conflation PERSISTS — that would prove the
      false core sentence, not the missing render, is the carrier.
This reviewer's own read: (a). The render fix (N-105a) is very likely to dissolve the whole
symptom, and adopting a core edit in the same wave as the render fix would make the two
uncorrelatable.

## G2 — no other core change. (PASS)

Everything else this seat could have wanted is already in core and demonstrably working:
- "If a spell is not in the cast list you CANNOT cast it right now" is quoted back verbatim and
  obeyed (deck139 s22 t13).
- The plan/stale-plan machinery fired correctly, including the drift notice
  (*"the actions your plan names are no longer among the options available right now"*, deck36
  s25, deck146 s18) — no stale-plan misplays at this seat.
- Anti-deferral is holding: 89 of 90 ask windows took an action; the single decline was correct.
- The attacker seam needs nothing: 29/29 full declarations with no guide at all.

So: **core PASS on the substance, with G1 raised as the one bounded proposal.** If the synthesis
takes disposition (a), this seat counts as a PASS and the streak is unbroken.
