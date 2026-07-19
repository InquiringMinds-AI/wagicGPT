# skill proposals from deck102 (Tergrid) - wave 20, Step 0-bis (GUIDELESS newcomer)

All single-seat; tagged pending a second instance per skill convention. Each EXTENDS a Step-0-bis rung,
none replaces one. Routed to the synthesis agent for the merge into `strategy-writing-skill.md`.

## PROPOSAL 1 (Step 0-bis rule 1/3, NEW face) - the ABSENT-ENGINE-CARD newcomer: write the guide for the payoff-ABSENT majority case, with a prioritize-when-drawn clause on top.

When a guideless deck is built around a SINGLE payoff card (here Tergrid, a 1-of in a 100-card singleton)
and that card never got drawn across the whole corpus (~38% expected whiff at 6 games), the corpus is a
pure sample of the deck's HEADLESS mode. Two rungs:
- **Do NOT build the guide's spine around the engine card the pilot rarely sees.** The guide must make the
  deck FUNCTION in the majority of games where the payoff never shows (deck102: disrupt + deploy a real
  threat + attack), then carry the engine as a prioritize+protect-WHEN-DRAWN clause above it - not as the
  central plan. A guide whose #1 rule is "assemble the Tergrid loop" is dead text in ~2/3 of games.
- **Verify LIBRARY vs stranded-zone before concluding the engine is "unreachable."** A payoff appearing 0
  times can be an engine bug (a command-zone card with no cast path) OR ordinary variance. Disambiguate
  with the library-size arithmetic (deck102: library 93 at turn 1 = 100-7, and the list has 99 non-
  commander cards, so the 100th IS shuffled in -> variance, not a gap). Do not log a variance whiff as an
  engine defect, and do not "fix" the guide against a card the pilot simply never drew.

## PROPOSAL 2 (Step 0-bis rule 3, a FOURTH idiosyncrasy face) - the SELF-HARM card the pilot reads as one-sided.

Rule 3 names three idiosyncrasy faces: belief-that-VETOES (17/21), card-text FABRICATION (62), and the
target-SUB-MENU interface seam (49). deck102 is a fourth: a SYMMETRIC-cost card (Pox/Smallpox: each player
loses life, discards, sacrifices) that the pilot fires as if it were one-sided disruption, paying life and
resources it cannot afford while behind (vs35 s18 Pox at L8; vs49 s11 Pox at L6 vs aggro). The tell: the
option line RENDERS the self-cost (`{right now: life -7, life -3}`) and the model casts anyway - so it is
a DECK-FACT gap, not a representation gap (the fact IS surfaced; the model does not know the card is a
payoff-card without its payoff). Corrective is a scoped FIRE-GATE keyed to board/life state ("cast a
symmetric mass-effect ONLY when the payoff is out, OR you are ahead on board, OR your own hand/board is
empty; NEVER while behind or being raced"), not "you CAN'T cast it." Sibling to rule 2's TRUST-THE-
RENDERED-NUMBER (there the number is right and the model re-derives; here the number is right and the model
IGNORES its sign).

## PROPOSAL 3 (reveal-seam instrument, NEW sub-class) - the DISCARD-PICK / choose-from-opponent-hand reveal is a pick-CARDINALITY surface; score over-selection + verify against the auto's <N> token.

deck135's wave-19 reveal rung covered LIBRARY-partition reveals (Glacial Revelation: choose-subset-to-a-
zone). deck102 adds the TARGETED-DISCARD face (Thoughtseize/Duress-style: choose from the OPPONENT's
revealed hand). It looks identical in the prompt (same "choose discards / look" partition framing) but is
a pick-EXACTLY-N effect, and the cardinality is the thing to check:
- **Read the `<N>` token in the card's `auto=`** to learn the true cardinality (Thoughtseize:
  `target(<1>*[-land]|reveal)` = pick ONE). A pick-ONE effect rendered as choose-a-SUBSET drives the model
  to OVER-SELECT (deck102: picked 2 of 2 Boomerangs, 3 of 3 nonlands) - harmless ONLY when the true best
  card is the lowest eligible index (all 3 records happened to land that way), a LATENT quality bug when it
  is not (over-picking discards the wrong card and forfeits the choice the card exists to give).
- **Score mechanical integrity by "which card actually left the hand" in the NEXT record's events**, same
  as the deck135 rung - here the lowest-index selected eligible card went to graveyard exactly once (the
  ENGINE-R1 same-tick finalize is confirmed working on this seam too). The DEFECT is representation (the
  prompt mislabels the opponent's hand as "top N of your library" and uses subset framing on a pick-ONE) ->
  ledger, not guide; the guide teaches the pick-ONE fact interim.

## PROPOSAL 4 (Step 0-ter / wave-18 spiral rung, AMPLIFIER note) - the losing-position spiral scales with the REVIEWED deck's own board complexity.

All 6 deck102 fallbacks are the wave-18 behind/stalled giant-reply spiral (>13k truncations). New
observation: a PRISON/ATTRITION deck AMPLIFIES the spiral because its OWN win-more pieces (No Mercy,
Painful Quandary, Oppression, Megrim, Necrogen Mists) build LARGE stalled boards -> more objects to
re-derive -> longer replies -> more truncations. When scoring the spiral for a deck that intentionally
grinds to a big board, expect the tax to be worse than for an aggro deck and weight the off-case branch
accordingly - the off-case is not a rare corner for these archetypes, it is where most of their turns live.
(Single-seat; the fix is still the wave-18 GUIDE off-case behavior branch, not a token-budget raise.)
