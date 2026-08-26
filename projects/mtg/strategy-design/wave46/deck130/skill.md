# Strategy-writing skill proposals from deck130, wave 46
Only what this seat's evidence supports.

## PROPOSAL S46-E - A RULE THAT FAILS TWICE IN THE SAME THREE-WINDOW SEQUENCE HAS EXHAUSTED THE
## GUIDE LANE. Say so in the artefacts, and name the render item that replaces it.
EVIDENCE. deck130's Starstorm cycling rung failed identically in wave 45 and wave 46: cast row ->
"Choose an option for Starstorm:" -> "Cast Card Normally" -> announce -> X=0. Between the two
corpora the guide gained `CASTING IT AT X=0 IS NOT CYCLING`, a description of the menu by name,
and `NEVER announce X=0`. In wave 46 the pilot's own plan said "to cycle it ... per Strategy
Guide Rule #1" and it took the wrong row anyway. The menu's two rows are bare - `1. Cast Card
Normally` / `2. cycling` - in a loop where the row before and the row after both carry full
annotations.
THE RULE: on the SECOND identical failure of a rung, the reviewer writes the re-form AND records
a stated limit - what evidence would show the guide lane is exhausted, and which engine item is
the remedy - so wave N+1 does not spend its edit budget on a fourth restatement. A guide can only
change what the pilot decides; it cannot change what the pilot can see on the row.
(deck130's wave-46 edit does exactly this: EDIT D46-3 re-forms once more, keyed to the model's
own plan text plus a self-detecting tripwire, and its edit-texts entry says in writing that a
third failure hands the problem to ENGINE H2.)

## PROPOSAL S46-F - WHEN A GUIDE HEURISTIC POINTS THE SAME WAY AS A RENDER'S LEAD CLAUSE, THE
## REVIEWER OWNS THE COLLISION.
EVIDENCE. The X-pricing cast row leads with `smallest X that kills an OPPONENT creature: X=1`.
The guide said "Pick the smallest X that reaches the toughness you must kill". Both are true and
neither is wrong on its own, but together they made "smallest" the whole decision: the pilot took
X=1 where X=2 killed the token-maker as well for `YOURS: none`, and lost 19 to -31.
THE RULE: when a rung fails, check whether the guide sentence and the render's OWN leading clause
push the same direction. If they do, the guide edit must state the OTHER direction explicitly and
name the render clause it is qualifying - and the render item should be filed too, because a
guide qualifying its own prompt is a weaker fix than a prompt that states both facts in the same
clause shape.

## PROPOSAL S46-G - A PER-CARD RULE TABLE CANNOT CARRY A PER-BOARD COST. Lift it to its own rule.
EVIDENCE. deck130's `#1 RULE` is a table with one row per cycling card. The cost that killed the
seat - three permanents on the OPPONENT'S board that each deal 1 damage per card drawn - is not a
property of any of those four cards; expressing it in the table would have meant writing it four
times and would still have missed Pyrite Spellbomb's draw mode and the draw step. It went in as a
new `#0 RULE` above the table, keyed to the rendered `{effect:}` text rather than to a card list.
THE RULE: before adding a clause to a per-card table, ask which side of the board owns the fact.
An opponent-board fact that applies to a whole ACTION CLASS (drawing, gaining life, attacking)
belongs above the table as its own rule, keyed to the render string that names it, so it covers
the actions the table does not list.
