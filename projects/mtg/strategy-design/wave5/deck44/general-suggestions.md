# General-layer suggestions (core prompt / general-strategy.txt) - deck44 wave-5

Clearly separated: CORE-PROMPT / REPRESENTATION asks are cross-deck; DECK-SPECIFIC asks stay in
strategy.txt and are listed here only so the synthesis agent sees the split. Per the epistemic rule,
these are single-SEAT observations (deck44 only) - I propose them for cross-deck corroboration, not
as unilateral core edits.

## CORE PROMPT (deck-agnostic, but burden-of-proof is on core)

1. **Extend the existing counter falsity-fact with the second observed wrong phrasing.** The core
   already carries *"the thought 'there is no spell on the stack' is always false while the option is
   listed."* Deck44's pilot bypassed it using a DIFFERENT sentence - *"the game log shows the spell
   moved to the battlefield"* / *"it already resolved"* (`e1248 s3`, `e5169 s30`). The interception
   must match the executor's own phrasing to fire, so add that variant to the same core clause:
   *"...and 'the spell already resolved / moved to the battlefield' is equally false - the option is
   offered only while the spell is still on the stack."* This is deck-agnostic (every instant-holding
   deck hits it) - but confirm it appears in other seats' logs before spending the core line.

2. **The plan/choice-mismatch line is present and losing - route the real fix to representation, not
   more core prose.** Core already says *"never describe an action and then choose Cast nothing."*
   Deck44 violated it with a fully-correct plan naming the exact card (`e8528 s26/s49`). A second core
   sentence will not help; the durable fix is option LABELING (below). I do NOT propose new core text
   here - I flag it for the synthesis agent as evidence that this core line needs a representation
   partner, not reinforcement.

## REPRESENTATION / ENGINE (the highest-value cross-deck fix - wave-4 F1 ask, still unshipped)

These are the real fix for #1 and #2 above and for the #25 stack-invisibility item. They are not
prompt prose - they change what the executor SEES.

3. **Surface the stack as a first-class CURRENT SITUATION line** whenever the player has priority
   with a spell pending: `On the stack (you may respond): Opponent's Wrath of God {2}{w}{w}`. Today
   the fact lives only in the log tail and the model fabricates over it (see skill.md #2). Repro:
   `e1248 s3` (Young Pyromancer), `e5169 s30` (Cranial Plating), `e8528 s26` (Wrath of God).

4. **Label reaction options with their target:** `Cast Counterspell - counter Wrath of God` instead
   of bare `Cast Counterspell {u}{u}`. This directly attacks the plan/choice mismatch: a labeled
   counter line out-competes "Cast nothing," which a bare cost-only line does not. Cross-deck: helps
   every deck with counters, targeted removal, and combat tricks.

Note on #25 (counterspell reaction TIMING): this corpus suggests the timing half is improving - all
27 counter windows arrived while the spell was genuinely still pending (27/27 live), unlike wave-4's
vs110 case where the window arrived post-resolution. The REPRESENTATION half (stack not surfaced,
option unlabeled) persists and is now the whole of the defect. Update the #25 ledger accordingly.

## DECK-SPECIFIC (stays in deck44's guide, listed here only for the split)
- Sweeper-first counter priority, the falsity-fact tied to deck44's counters, the plan/number tie,
  the compressed Sleep-Cursed untap line, the defenders-count mulligan line - all in strategy.txt.
