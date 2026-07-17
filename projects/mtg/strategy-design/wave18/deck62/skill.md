# deck62 wave-18 — proposed additions to the strategy-writing skill (Step 0-ter)

deck62 is the first REVISED-GUIDE-ROUND-2 seat whose corpus flipped from mostly-winning (4/6) to
mostly-losing (2/6) via a pool swap and variance, with ZERO taught-class regression. That shift
exposed several generalizable review lessons:

## 1. (Step 0-ter rule-6, STRONG worked example) When the corpus difficulty rises, the TAX MOVES to the off-case — and a first guide's unguided off-case births a NEW tax shape there.
Wave-17 validated deck62 on a mostly-winning corpus; the off-case (behind, low life) rarely
engaged, so it was left unguided. Wave-18 put deck62 behind in 4 of 6 games, and a brand-new
dominant tax appeared ONLY in the off-case: a "losing-position lethal-hunt spiral" — at 2 life vs
an opponent at 20/32, the model re-derives its whole board over and over hunting for lethal that
does not exist ("10 is not 32, I must have made a mistake"), blows the full token budget, and
truncates -> unparse. This is the literal payoff of rule 6 ("the LOSING corpus lives entirely in
the off-case"): the fix is a behavioral off-case branch (make the best play and pass when you
cannot win this turn), authored even though the winning corpus never needed it. LESSON FOR
REVIEWERS: when a guided seat's win-rate drops because the POOL got harder, do not read the drop as
regression — read it as the off-case finally ENGAGING, and expect a new tax shape localized there.

## 2. A validated tightening can PERSIST-and-DISOBEY under a specific TRIGGER — name the trigger, not the rule.
The wave-17 A-list-only tightening ("NEVER name a creature that is absent from the list") did not
die; it recurred twice (vs14 s21, vs21 s12), BOTH times when the model was behind and in
"I MUST attack to win" mode and overrode the list with its own-board wish-read. The rule was
present and correct; a stronger drive beat it. Restating the rule harder is weak; the effective
edit binds eligibility to the LIST explicitly for the trigger case ("this holds EVEN when you feel
you must attack to win; wanting the damage does not add a summoning-sick creature to combat").
Sibling to the wave-17 "biggest flyer THAT CAN ATTACK" superlative-scoping lesson: the model reads
the surface it WANTS, so the rule must pre-empt the specific want.

## 3. When corpus difficulty shifts, read the tax at GIANT-REPLY count, not fallback rate.
deck62's fallback rate barely moved (3/143 -> 5/182, 2.1% -> 2.7%) but giant replies (>8k) went
1 -> 8. The fallback rate was flat because most spirals still emitted a parseable CHOICE before
truncating; the giant count tracked the real signal (number of losing games). When a seat's
win-rate falls, the fallback rate can hide a tax explosion — count giant replies and bucket them by
board-state (winning vs behind) to see it.

## 4. Distinguish "recount" FLAVORS before crediting or blaming a trust-line ban.
A crude "reply mentions Forests" grep conflates two different behaviors: (a) recounting the printed
P/T of a creature ALREADY wearing Blanchwood (the ban's target — GONE this wave), and (b)
PROSPECTIVE cast-value math (computing what Blanchwood WILL do to a creature not yet wearing it —
legitimate and unbannable). The wave-17 Blanchwood recount-ban WORKED; the surviving Forest-counts
are all flavor (b). Reviewers validating a "trust the printed number" rule must separate the two by
reading WHETHER the target already shows the boosted "(printed X/Y)" line in the prompt.

## 5. Truncation-unparse is a behavior lever, not a token-budget lever.
The 3 spiral unparses truncated at the full token budget with no CHOICE/ATTACK line. Raising
max-tokens does NOT help (the model spirals longer and still truncates); the fix is the off-case
behavioral line that stops the spiral. When an unparse is a truncated ramble (not a formatting
miss), route it to guide behavior (or a representation nudge), never to a bigger budget.
