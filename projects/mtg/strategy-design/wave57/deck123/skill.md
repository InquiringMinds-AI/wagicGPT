# deck123 -> strategy-writing skill: proposals from the wave-57 corpus

Only what my own evidence supports. Every claim is counted from the rendered `prompt` of the six
`deck123` seat logs in `matchups-20260903-174505/`.

## S57-1. A RULE MUST NAME THE UNIT IT READS, OR AN AGGREGATE LINE WILL EAT IT
**Evidence.** deck123's RULE 5 keys on the edict row's own `they control N creature(s)` clause
and is correct. On `123v130` s45/s49/s50 that row printed `they control 1 creature - Rorix
Bladewing (6/5) [flying, haste] is sacrificed` and the pilot declined it three times, because a
NEW menu-level line above the rows read `NO LIVE CAST ROW ON THIS MENU: all N cast rows below
carry a verdict computed from the board that reads zero`. It lost that game. The guide's rule
did not contradict the header - it simply never said which unit it reads, and the aggregate
sentence is the more emphatic one on the screen.
**Proposal.** Add to the skill's rung-writing section: *every teach names its unit - "this ROW's
{right now:} clause", "this LINE of the board frame", "the header of this menu" - and when the
render carries both a per-row verdict and a menu-level summary of those verdicts, a rung over
the rows says so explicitly.* This is not a hedge against the render (forbidden) and it is not
doubt: it is a scope statement, of exactly the kind the trust doctrine's rule (2) already
demands of the render itself. Give it a removal condition so it retires when the surfaces agree.
**Falsifiable at the next corpus:** casts/declines that contradict a row verdict while a
menu-level summary is on the screen, at my seats: 3 -> 0.

## S57-2. WORKED EXAMPLES ARE THE TRIM CURRENCY; THE RULE SENTENCE IS NOT
**Evidence.** My predecessor's P4 (a ~2.5 KB trim of worked examples off passing rungs) is
**PASSED on every clause it named**: Damnation casts at `destroys 0` 0 of 54 rows; edict casts at
N != 1 0 of 267 rows; `plan_choice_conflict` 0; casts off an all-dead menu 1, and that one was
correct play against a false header. The rungs held with the examples gone. In the SAME file the
one paragraph whose example I could have cut and did not - the Alarm second-copy bracket - is the
rung that broke (`123v130` s55, 1 of 6 rows).
**Proposal.** State the trim rule as an arithmetic one, not a judgement: *a rung whose
denominator this corpus is >= 20 rows with 0 breaks may lose its worked examples and keep its
test sentence; a rung with any break this corpus keeps or gains an example, and its example is
THIS corpus's repro, never an inherited one.* Two corpora of evidence now support the first half.

## S57-3. "PASS" ON A DEAD DENOMINATOR IS "UNTESTED", AND THE SKILL SHOULD FORCE THE WORD
**Evidence.** Wave 56 recorded RULE 3 as "25 rows, all 25 `destroys 0`, 0 casts - PASS" and my
predecessor flagged it vacuous by hand. This corpus the same rung had a LIVE denominator - 54
rows, 9 of them live, 3 casts, all at M = 0 - and it is the first real test the rung has had.
Conversely deck123's `DRAW PUNISHERS` rung fell from 109 renders to 47 and deck130's from 109 to
2; a "seventh corpus at zero" on 2 renders is not a seventh corpus of anything.
**Proposal.** The skill's adjudication table already has PASS / FAIL / UNTESTED. Add: *a PASS
whose denominator contains no window where the forbidden action was AVAILABLE and non-trivial is
written UNTESTED (vacuous), and the reviewer prints both the row count and the LIVE row count.*

## S57-4. THE ORDERING BETWEEN TWO RULES IS ITSELF A RUNG, AND IT IS THE ONE NOBODY WRITES
**Evidence.** `123v162` s31. deck123's guide contains "fire the chain on the OPPONENT'S turn"
(RULE 1) and "send every creature on that list" (CHECK 5). Both are correct in isolation. Taken
together on one turn they instruct the pilot to attack with its token-makers and then fire the
chain with the makers it just tapped - and under its own Intruder Alarm nothing untaps again.
26 power stood tapped for seven turns and the deck died at 0 without another attack. The reply
executed both rules and the plan line names both.
**Proposal.** Add a check to the skill's final pass: *for each pair of rules that can be
satisfied in the same turn, write the turn that satisfies both and check that it is a turn you
want. Where it is not, the exception belongs in BOTH places, in the words each rule already
uses.* I applied this to A57-2a/A57-2b (the same exception in the RULE 1 timing paragraph and in
CHECK 5). This is cheap - it is a read of the file against itself, no corpus needed.

## S57-5. A RUNG THAT SURVIVES A RENDER FIX IS A DIFFERENT RUNG AND SHOULD BE RE-DATED
**Evidence.** The Greaves rung (`{right now: you control 0 creatures - this equips nothing}`) was
carried for six corpora as a strategy break while the row was unpriced, then re-keyed to the
literal in wave 56, and this corpus it is **45 rows / 0 takes**. It was never one rung: it was a
render gap, then a rung. The file still carried eleven lines of history from the render-gap era.
**Proposal.** *When a render change supplies the number a rung was asking the pilot to compute,
the rung's corpus count RESTARTS at that corpus and its pre-fix anecdotes are deleted, not
retained as emphasis.* Applied here as A57-3.
