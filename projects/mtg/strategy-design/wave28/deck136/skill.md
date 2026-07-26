# deck136 -> strategy-writing-skill proposals (wave-28, FIRST-GUIDED validation)
# Layer: skill.md = proposals to the guide-WRITING skill. Tag single-seat + threshold.

## PROPOSAL 1 (NOVEL - the record-drop-under-first-guide lesson the brief flagged) --
## A FIRST-GUIDED RECORD DELTA IS NOT A CLEAN GUIDE SIGNAL. Attribute per-matchup and
## per-decision; never read a guide's quality off the aggregate W-L in a round-robin.
deck136's record DROPPED 4/6 -> 2/6 the wave its guide was deployed. The naive read
("the guide hurt") is FALSE. Decomposed, the drop is entirely EXOGENOUS to deck136's
guide:
  - SWING 1 was POOL: a wave-27 opponent that deck136 beat (deck131) rotated OUT and a
    new opponent (deck146 Orzhov Dungeons) rotated IN and won. The roster changed under
    the deck; no play of deck136's caused it.
  - SWING 2 was the OPPONENT'S guide: deck18 went 35-1-loss -> 6/6-perfecta because
    deck18 got its OWN first guide this wave. In a round-robin where MULTIPLE decks are
    simultaneously first-guided, every deck's record moves with every OTHER deck's
    improvement. The win column is a SHARED-fate signal, not a per-guide one.
The method rule: when a first-guided deck's record moves, split the matchups into STABLE
(same result both waves - the guide's true baseline) and SWUNG, and for each swing ask
"did MY deck's guide change a decision, or did the OPPONENT/pool change?" Only swings
traceable to a decision this guide steered count as guide signal. deck136's stable
matchups (wins vs134/vs137, losses vs22/vs93) show the guide's floor held; the two swings
were pool + opponent-guide. Promotion: this is the SECOND first-guided deck this wave to
drop or flatline under a guide (deck134 0/6-again) - promote to a named skill rung
("first-guided record attribution: decompose swings before judging the guide") now, not
single-seat.

## PROPOSAL 2 (NOVEL - the harshest-audit corollary) -- THE WORST GUIDE DEFECT IS FOUND BY
## DECISION AUDIT, NOT BY THE RECORD. A fabricated card FACT can survive a winning record
## and can be invisible in the game it poisons.
deck136's single worst guide defect was a fabricated mana value (Relentless Rats asserted
MV4; engine + Oracle = MV3) that INVERTED both removal facts (told the pilot Ritual was
dead vs Rats when it is the answer; told it Legion's End hits Rats when it cannot). This
defect:
  - did NOT show in the record (the game it hit, vs93, was a loss both waves and the
    pilot never drew Ritual to be mis-benched);
  - WAS visible only in the PLAN prose - the pilot spent five turns planning an
    impossible "Legion's End the Rats" line the engine never offered, and self-corrected
    against its own guide mid-game ("Legion's End requires MV<=2, Rats are MV 3").
Method rule (reinforces wagicgpt-verify-oracle-text at the guide-writing layer): every
numeric card fact a guide states (mana value, power/toughness thresholds, "hits MV<=N")
MUST be read off the card-script `mana=`/`text=` line, never from model memory, at
DRAFT time - and the reviewer's decision audit must read PLAN prose for plays the pilot
attempted-and-abandoned, not just plays it executed. An abandoned-then-relapsed plan is
the fingerprint of a guide FACT error (the pilot trusts the guide over the board until
the board forces a correction). Promotion: adopt immediately as a rule-N face - it is a
verification tightening congruent with the existing verify-oracle doctrine, witnessed by
concrete live damage.

## PROPOSAL 3 (reaffirm P2-style, single-seat) -- the "you almost never lose the long
## game" identity line is a LIABILITY vs a value MIRROR. Scope archetype-supremacy claims.
The wave-27 guide opened with "You almost never lose the LONG game." deck146 (Orzhov
Dungeons, its own card engine + Vanishing Verse removal) out-ground deck136 to a 22-turn
adjudication loss (8-16). A blanket "you win the long game" claim makes a value deck
COAST into a grind it can lose on cards. Refinement: an identity line that asserts a
deck wins a phase/axis (long game, the race, the air) must SCOPE it to the opponent class
it is true against ("vs decks with no engine of their own") and name the exception class
(another value/attrition deck). Tag single-seat; promote if a second value-midrange deck
loses a long-game mirror under an unscoped supremacy line.

## Boundary re-checks (nothing to merge into core from this seat)
- No core-prompt defect. Parser held (0 fallbacks at seat). Land tags clean. PASS #23.
- e2 phantom-chooser fix is an ENGINE win, not a method change - recorded in notes.md.
