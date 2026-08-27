# deck130 - proposals for the strategy-writing skill (wave 49)

**S1 - A survival carve-out (#71/#112) needs its own falsifier: does the action remove the lethal?**
D48-3's SURVIVAL FIRST ("their power >= your life -> take the X whose THEIRS list is longest,
WHATEVER the YOURS list names") was obeyed at `vs152` seq 17-19 and killed the pilot's only blocker
while the 6/6 that was the lethal survived every affordable X. The carve-out compared the wrong two
numbers: their total power vs life, when the question is their power AFTER the sweep vs life.
Proposed rule: every survival override states the post-action quantity it is betting on ("the
creatures the sweep does NOT kill total less than your life") and the guide walks the subtraction on
the row's own lists. Generalises to any "do X to survive" rule: X buys survival only if the printed
threat after X is below the printed life.

**S2 - A one-row window is a decision, not a prompt to act, and the guide names the pass as its
answer.** Two self-hits this corpus (`vs146` seq 16 Spellbomb into own Blastminer; `vs126` seq 38
Blastminer into own Forgotten Cave) came from windows whose only action row named the pilot's own
permanent, with a plan that named a target the window did not list. The executor took the nearest
row. The old "never a row tagged [your battlefield]" line existed for the Blastminer and was not
generalised to the deck's other four target-taking abilities (#81 coverage). Proposed rule: for every
ability that can target its controller's permanents, the guide states (a) the owner tag that marks
a self-target and (b) that a window of only such rows is answered 0 (pass); reviewer metric: rows
taken whose target carries `[your battlefield]`.

**S3 - The upkeep asks that spend the turn's mana are a section of their own.** The Hammer return
(`vs146` seq 22, 28) is asked in upkeep, before the main-phase menu shows the body; #112 put the
lethal count first in the turn for the same reason (mana spent in upkeep decides main). Proposed:
guides for decks with upkeep mana sinks (Hammer's return, Katilda's pump, the Hive) carry a "YOUR
UPKEEP" block that reads the hand and the land count and names what the upkeep must leave untapped.

**S4 - A mulligan section's rules must not contradict each other; when they do, the pilot picks one
and the reviewer cannot score it (#97, #119).** "Mulligan only zero lands or all lands" and "the
coverage line decides the close ones ... 'would not cover any spell' is the one shape where seven
can be worse than six" both stood; at `vs123` seq 1 they disagreed and the pilot shipped. The
rewrite (D49-1) states one odds paragraph and one preference order, no floors. Proposed reviewer
check: for each mulligan section, construct the four hands (0 lands / 1 land no coverage / 1 land
with coverage / all lands) at (keeping 7), (keeping 6) and (keeping 4) and confirm every sentence
in the section gives the same verdict.

**S5 - Cast-order rank must survive within a turn, not only within a window.** `vs123` seq 22-29:
entry 1 (Starstorm) and entry 3 (Molten Rain) were both castable; the pilot cast entry 3 first and
then entry 1 at the X the leftover mana allowed. The list said "take the FIRST line that matches";
it did not say the first line is also first in TIME when two are castable and one is X-costed.
Proposed: any ordered cast list whose top entry is an X spell or a mana-hungry spell states the
sequencing consequence ("it goes first in the turn - the X shrinks with every spell cast before it").
