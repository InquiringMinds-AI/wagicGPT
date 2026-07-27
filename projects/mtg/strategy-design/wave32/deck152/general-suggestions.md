# deck152 -> general (core) prompt: wave-32

## VERDICT: **PASS** (27th consecutive). No core-prompt edit proposed.

Evidence from this seat: 0 fallbacks / 211 decisions (2nd consecutive zero-fallback corpus),
24/25 full attacker declarations, zero off-list answers, zero invented Cast numbers, zero
protocol violations in my own replies, zero paradox verbalizations of the new combat tags. The
answer-first protocol, the completeness guarantee, the plan carry-over and the decline-ordering
rules all held under a prompt that grew ~12% at p90 from the i13 card-text addition.

Two observations that LOOK like core-prompt candidates and are deliberately routed elsewhere.

---

## Observation 1 -- the completeness guarantee was CONTRADICTED twice, and the fix is NOT prose

The core prompt promises: *"The lists are also complete... if a spell is not in the cast list you
CANNOT cast it right now."* At **deck116 s18** and **deck105 s28** the model quoted that sentence
back and wrote *"This is a contradiction"* / *"Okay, there is a contradiction"*, because a card it
could demonstrably pay for was absent from the list (findings i1, residuals A and B).

**Do not weaken the guarantee.** It is load-bearing: it is what produces this seat's zero
off-list answers across two corpora, and a hedge ("the list may occasionally be incomplete")
would licence exactly the fabricated-option class it was written to kill, in exchange for
covering an engine defect that is being fixed. The sentence is TRUE whenever the oracle is
correct, and the oracle is the thing to correct. Routed to notes.md as N-152f / N-152g.

The pilot-side cost (kilobyte-scale re-derivation in the contradiction windows) is absorbed in
the DECK guide instead, where it can be phrased without touching the global invariant: *"When
that happens: DO NOT spend the decision arguing about it ... pick the best thing that IS offered,
in one short answer."* Deck-local, retires when the engine item lands.

---

## Observation 2 -- both corpus `unparsed_reply` fallbacks are one PARSER shape, not a prompt shape

Both fallbacks in this seat's games (deck139 s21, deck158 s35 -- opponent seats, `kind=blockers`)
are compliant `BLOCKS:` head lines written with CARD NAMES instead of `B#:A#` labels:

```
BLOCKS: Pollywog Symbiote blocks Elite Spellbinder
BLOCKS: Orc army: Sigarda, Champion of Light; Dunland Crebain: Moonrage Brute
```

The protocol was OBEYED -- correct anchor, first line, unambiguous assignments against the
offered lists. The core prompt already carries a correct positive example
(`e.g. "BLOCKS: B1:A2, B3:A1, B2:none"`) three lines above the failure, so the model is not
missing the instruction; it is substituting an equivalent notation. Adding a negative clause
("use the labels, never card names") is the kind of prompt churn the skill's rejected-ideas
appendix warns about: it lengthens a prompt that just grew 12% at p90, it cannot cover the next
equivalent notation, and it treats a parser gap as a compliance gap.

**Routed to notes.md as N-152j** (extend the blockers parser to resolve names against the offered
B/A lists by unique match, dropping unmatched names, first-wins as today). If two or more other
seats report the same shape this wave, the synthesis should still prefer the parser fix -- but at
that point the convergence is worth recording as a standing ledger priority rather than a prompt
edit.

---

## Item explicitly NOT proposed

A poison / alt-win progress line belongs in the CURRENT SITUATION block, which is core-prompt
adjacent -- but it is a RENDER change (new engine-computed state), not prompt prose, so it is
routed to notes.md as N-152i (HIGH) rather than proposed here. Flagging it so the synthesis does
not read this seat's PASS as "nothing needed at the board snapshot": the seat lost a game to a
number the snapshot does not contain.
