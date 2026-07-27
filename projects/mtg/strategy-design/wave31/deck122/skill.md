# deck122 -- wave 31 skill proposals

Two proposals, both single-seat-evidenced this wave, both offered as SHARPENINGS of
existing rungs rather than new rungs. The seat is exiting; these are what the next
control-role guide and the next near-terminal reviewer should inherit.

---

## Proposal 1 (Step 4 / matchup-posture authoring) -- when a guide assigns a deck the
## CONTROL role, name the TAP COST explicitly; an abstract posture does not survive a
## locally-"free" attack

**Evidence.** deck122's frozen guide carries an explicit control posture:
"vs AGGRO ... you are the CONTROL. Do not race. Deploy blockers ... take the good blocks."
The model honored it repeatedly (five separate `ATTACK: none` declarations at healthy life
totals). It failed exactly once, and the failure mode is instructive:
`1785152439-...deck122-vs-deck158.jsonl` **seq30 t10**, at 24 life against an amass board,
it attacked with all three of its blockers -- Soul Warden, Suture Priest, Kitchen Finks --
reasoning "The opponent's massive threats ... are all tapped ... They cannot block. ...
Risk: None. I am not exposing myself to combat damage this turn." The attack was indeed
free IN THAT COMBAT. It was not free on the crackback: at seq35 t11 the only untapped
bodies were two 0/1 Blood Artists and the swing took 28 -> 7 (a single chump on the 15/15
would have absorbed 15 of the 21).

The model demonstrably KNOWS the rule in the abstract -- three seqs later at seq37 it
writes "attacking with my small creatures now leaves me with fewer blockers next turn."
This is not a knowledge gap; it is a **local re-derivation gap**: the posture teach lives
in the guide's matchup section, and the attack seam re-derives combat from scratch with a
"can they block me?" heuristic that never prices the tapped-blocker cost.

**Rung sharpening.** A posture teach ("you are the control, do not race") is a STANCE and
is applied at the level the model reasons at. The COROLLARY that makes the stance operative
at the attack seam must be stated as a concrete, checkable sentence in the same teach, e.g.
"Your blockers stay home even when the opponent is tapped out -- an attack that cannot be
blocked still taps your blockers for their next swing. Only attack with a body you are
willing to have unavailable on their turn." Generalize: **when a guide assigns a role, name
the ONE mechanical cost that the role's decisions pay at the seam where that role is
easiest to abandon** (control -> tap cost at the attack seam; aggro -> the block that
forfeits a turn of damage; combo -> the mana spent on interaction).

Applies to any deck whose guide assigns CONTROL/stabilize posture. Single-seat -> tag it;
promote if a second control-role seat shows the same locally-free-attack fallacy.

---

## Proposal 2 (Step 0-ter / exit-gate discipline) -- when reading a WIN-RATE DROP at a
## near-terminal seat, decompose the record against the POOL's adjudication rate before
## reading it as a teach regression

**Evidence.** deck122 went 6/6 -> 3/6 across two guided corpora with NO teach failing.
The wave-30 taxes audit had already flagged the perfecta as partly structural (2 kills +
4 adjudication-ahead grinds, in a pool with 12 cap-adjudications). Wave 31's pool had 3
cap-adjudications total. The decomposition:

| | kills | adj-ahead | losses | pool adjudications |
|---|---|---|---|---|
| wave 30 | 2 | 4 | 0 | 12 / 21 |
| wave 31 | 2 | 1 | 3 | 3 / 21 |

**The kill count is identical.** The entire delta sits in the adjudication column, i.e. in
the pool property, not the deck. Had the reviewer read 6/6 -> 3/6 as evidence of a teach
regression, the seat would have been held for a phantom problem -- the mirror image of the
wave-30 g1 false-clean, and the same lesson from the opposite direction.

**Rung.** Method headline 6's adjudication caveat is currently phrased one-way ("do not
over-credit a high win rate when the deck's win metric is the tiebreaker"). It is
BIDIRECTIONAL and should be stated so: **a deck whose win metric is the adjudication
tiebreaker has a win rate that tracks the POOL's adjudication rate; both a rise and a fall
must be decomposed into kills vs adj-ahead before either is attributed to the guide.**
The kill count is the pool-invariant statistic; make it the headline number for such seats,
and state the pool's adjudication rate alongside every record at these seats.

---

## Not proposed

- The Blood Artist and Felidar-bottom absolutes were both relaxed by the model with sound
  local reasoning and zero realized cost. The deck131 never-block precedent says the
  absolute-with-escape-clause form is working as intended -- the model kept the absolute
  in every binding case and broke it only where its rationale did not apply. No change.
