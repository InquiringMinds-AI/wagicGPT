# Core-prompt / render proposals from seat deck130 (wave 44)

LAYER ROUTING: engine defects are in the seat file's ENGINE list (H1, H2, M3, M4, L5) and are
NOT restated as prompt text here.

## G4 (HIGH, core-prompt half of engine item H1) - the X-announce prompt should state the
mapping in the imperative, not as a fact about option 1.
Current text: `Every listed value is affordable; option 1 is the LARGEST X (X = 5). Reply with
the OPTION number, not the X value:` followed by `1. X = 5 / 2. X = 4 / ...`.
The sentence is TRUE and the pilot still answered `CHOICE: 1` intending X=1 (deck130 vs126 seq
46, decisive; 2 of 18 X-announce windows corpus-wide show a plan/choice divergence, both toward
option 1). A statement about which option holds the largest value is a fact the reader has to
apply; an instruction is a step the reader executes.
Proposed replacement for that one sentence: `The option number is NOT the value of X - find the
row whose text reads "X = " followed by the number you want, and reply with THAT row's number.`
This costs one sentence and no render work. It is independent of, and inferior to, the real fix
(H1: order ascending, or carry the per-X kill preview onto each row) - ship both.

## G5 (MED) - the "Opponent life:" cross-check the guides rely on has no prompt-side support.
Several guides (deck130 rule #2 CROSS-CHECK) ask the pilot to compare the opponent's life with
what it was "when you last looked". The prompt prints only the current value; the previous value
exists only in the cumulative GAME LOG, tens of lines up, interleaved. deck130 fired 9 face
damage spells across a game in which the opponent's life went 25 -> 22 -> 18 -> 20 -> 17 -> 19
-> 24 -> 27 -> 24, and never once noticed the reversals.
Proposal: one optional clause on the life line when it has moved the wrong way since this
seat's previous decision - e.g. `Opponent life: 24 (was 19 at your last decision, +5)`. It is a
delta the harness already has and it is the only cheap way to make a trend-based teach
executable. If this is judged out of scope for the core prompt, the alternative is to retire
trend-based teaches from the guides entirely; they cannot be executed against a scalar.

## G6 (LOW) - the `may block` line should be named as the legality list in the standing
blockers paragraph.
Current standing text explains the parenthetical trade results and says "Trust it over your own
arithmetic; never re-derive it." It does not say that the list of attackers on a blocker's row
IS the list of attackers that blocker may legally block. deck130 vs146 seq 32 replied "Nadaar
is unblockable by Blastminer" about an attacker printed on its own blocker's `may block` line,
and died as a result.
Proposed addition: `Every attacker named on a blocker's "may block" line is an attacker that
blocker CAN legally block; the parenthetical says how the fight ends, not whether it is legal.`
