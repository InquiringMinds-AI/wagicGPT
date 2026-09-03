# deck146 -> core-prompt / render proposals (wave 55)

Layer-routed. Engine BUGS are in the seat file's engine list (E-1..E-5), never here.

## R-1 (MED, render) - order the cleanup-discard menu by descending printed mana value

The discard ask numbers the hand in an order the pilot cannot predict, and every guide in this pool
ranks discards by cost. Two of seven deck146 discards this corpus sent a cheaper card while a
dearer one sat lower on the list (`146v130` s23: `PUT: 1` = Agadeem's Awakening `{b}{b}{b}{x}` with
Emeria's Call `{4}{w}{w}{w}` at row 5; `146v162` s20: `PUT: 7, 1` = a spare Plains + Acererak
`{2}{b}` with two Lolths `{3}{b}{b}` and two Silverquill Commands `{2}{b}{w}` left). Sorting the
rows by descending mana value would make row 1 the answer the guides already ask for, and would
cost nothing to any other consumer of the menu. Cheaper alternative if ordering is unwanted: print
`{MV n}` on each discard row so the comparison is a scan and not arithmetic over `{..}` symbols.

## R-2 (MED, render) - the edict tie clause needs a quantifier

`{right now: 24 tied at MV 0 (their highest): Human #1; Human #2; ... }` reads as an effect list.
Suggested: `{right now: they sacrifice ONE of these 24, their choice - all tied at MV 0 (their
highest): ...}`. Same family as lane C's both-player edict verdicts (D15/D16), which added the
"this row does not pick a target" commit clause but not a count. Repro `146v123` seq 28; two
consecutive plans at that seat asserted a 24-card sacrifice.

## R-3 (LOW, render) - `[doesn't untap during its controller's untap step]` deserves a consequence
clause on the ATTACKERS menu

The tag is printed on the A-line and on the battlefield line, and its consequence for a declared
attacker (it is tapped for the rest of the game, not for one turn) is exactly the kind of derived
fact the render already computes elsewhere. Suggested tail on the A-line only:
`(declaring this attacker taps it permanently - nothing untaps it)`. Repro `146v123` seq 24: both
A-lines carried the bare tag, both were sent, and the seat had no blocker for the 24-creature
crack-back that killed it. This is the attack-side twin of the block-side `(you kill it, your
attacker lives)` computation the pilot already trusts.

## R-4 (LOW, prompt) - the plan block should not carry an arithmetic assertion forward unchallenged

`146v123` seq 28's served plan opened with a sentence the seat had written one window earlier and
that the current row contradicted (`sacrifice all 24 Human tokens` vs a row naming a one-card
edict). The prompt already carries the excellent
`(note: this decision's list does not contain the actions your plan names ...)` note for a NAME
mismatch; there is no equivalent for a plan whose stated EFFECT is contradicted by an annotation on
a row the plan names. Not proposed as a general checker - proposed as a narrow case: when the plan
text names a card that is a row on THIS menu and that row carries a `{right now:` clause, append
`(your plan names this row; read its {right now:} clause again before you act on the plan)`.
