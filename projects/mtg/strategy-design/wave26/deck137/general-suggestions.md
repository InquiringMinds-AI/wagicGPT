# deck137 general-suggestions (wave 26) -- CORE-layer candidates only (bar high: 20 consecutive PASSes)

## NO new core PROPOSAL this wave. Two items kept BELOW-BAR as accumulating evidence.

### (below-bar, accumulating) "Trust the engine-offered legal set, do not re-derive its condition" -- now witnessed at a SECOND SEAM, still ONE DECK.
The model overrides engine-computed legal sets by re-deriving the underlying condition itself:
- wave-25 deck137 s21 (ATTACK list): overrode a PRESENT Lovestruck (offered as legal attacker)
  by re-deriving the "control a 1/1" rule, wrongly concluded ineligible, passed a 5/5 swing.
- wave-26 deck137 s36 (BLOCKERS list): overrode a PRESENT Lovestruck (offered as a legal
  untapped blocker with "you kill it, your blocker lives") by re-deriving its tapped state from
  the game log, declined free removal.
Same failure family, two seams, but still a SINGLE DECK. The natural core form would be one
representation/prompt-level directive: "any option the engine OFFERS is legal right now -- its
preconditions are already checked; act on it, do not re-derive whether it is allowed." The
promotion bar per the wave-25 ceiling doctrine is a SECOND DECK exhibiting the same override, not
a second seam on the same deck. HOLD below-bar; deck reviewers this wave should flag any
"model re-derived a condition the engine already computed and overrode a legal offer" at THEIR
seat -- if a second deck shows it, this graduates to a core proposal.

### (below-bar, single-witness) Same-named-permanent instance disambiguation (R-DUPLICATE-NAME-INSTANCE).
Corpus-general BY CONSTRUCTION (any deck running 2+ creatures of the same name, or tokens sharing
a name, in mixed tap/state) but only ONE witness so far (deck137 vs18 s36: two Lovestruck Beasts,
one tapped one untapped, no instance handle; model could not bind the offered B-line to the
untapped copy and declined a free block). The candidate core fix -- give same-named permanents a
stable per-instance handle in the battlefield list AND in every A/B/target/activation option line
-- is a representation change, not a per-deck guide. Do NOT act on one seat. Watch for recurrence
at duplicate-heavy or token-heavy decks (deck18 Kithkin tokens, deck22 Giants, wide go-wide decks
are natural candidates). Tracked in notes.md as the representation ledger item; surfaced here so
core reviewers know it is corpus-general the moment a second deck hits it.

## Standing core items unchanged at this seat (no new evidence for/against):
- c4 parser hardening (example-echo exclusion + line-anchored selection): PASS at this seat --
  11 reversals, all parsed to the model's final coded line, zero drops/mis-picks. No misfire to
  report; contributes one clean corpus toward the c4 firing-where-it-should-not confidence.
- b2 natural-stop precedence: continues to work as a spiral safety net (see skill.md), with the
  honest limit that it ships a considered-wrong final answer (s36) -- not a core defect, a
  representation gap routed above.
