# deck126 - core-prompt / render proposals (wave 48), layer-routed

Engine BUGS with repro are in `wave48/seats/seat-125-126.md` (HIGH #1 is the E1 name-mismatch).

G1 (core prompt / parser, HIGH - the bug itself is in the seat file) - **A replacement answer whose
parenthetical name does not match the option at that index must not be executed.** `vs130` seq
25: first line `CHOICE: 2 (Cast nothing right now)`, re-answer `CHOICE: 1 (Cast Sanguine Bond)`
with Sanguine Bond nowhere on the 2-row menu; index 1 = Tribute to Hunger was cast at a
0-creature header. The short name is asked for precisely so this check is possible. The same
check would classify the corpus's four `unparsed_reply` fallbacks (all out-of-range indices whose
names are cards NOT on the menu: Acererak, Rorix, Fate Unraveler, "Attack with all creatures")
as "named a card not offered" - a distinct, countable failure (a hallucinated row) rather than a
parse miss.

G2 (core prompt, MED) - **Post-answer overrun is now the reasoning channel, and it costs 4-9 minutes
a decision.** 9/174 replies on this seat carried two or more `CHOICE:` lines with 2.5-4.9 KB of
reasoning between them (`post_answer_overrun` 4,648 / 4,889 / 4,779 / 2,563 chars); latencies 235
s (`vs146` seq 22) and 515 s (`vs130` seq 11). The prompt's "Write nothing else" is not holding,
and E1 has made the overrun load-bearing. Proposal: either legitimise it (a bounded `THINK:` block
BEFORE the CHOICE line, so the coded answer is written after the reasoning and there is one of
them) or measure it as a first-class counter (`multi_choice_replies`) so the next corpus can tell
whether the E1 replacement is rescuing answers or amplifying drift.

G3 (render, MED) - **The Tribute row should say what "their choice" would be.** `Cast Tribute to
Hunger ... - legal targets right now: the opponent` carries no forecast; the header's creature
count is on another line. Proposal, same family as the converter/lifelink previews: `{right now:
they control 1 creature - Thraben Doomsayer (2/2) is the one they must sacrifice; you would gain
2}` / `{right now: they control 0 creatures - this sacrifices nothing and gains you 0}`. The
0-branch is the zero stop the guide keeps having to restate; the 1-branch is F1/F5 written by the
engine.

G4 (render, LOW) - **`[repeat:]`-tagged Sorin loyalty rows and the priority menu** (`vs146` seq 46):
a 12-row priority menu of 9 Staff-target rows + 3 Sorin rows; the ping-the-opponent row sits at
index 11. Fine at this size; noted only because the target-menu collapse (R3) has no window on
this seat this corpus (max 11 Staff rows).

G5 (measurement) - `mana_only_windows_skipped` per game this seat: 0, 0, 0, 103 (`vs152`), 0, 835
(`vs123`); all-mana menus reaching the model: 0 (the one Battlement-bearing menu, `vs152` seq 20,
also carried Staff pings). The 835 is the Doomsayer/Alarm game, the same class as wave 47's
8,610 - lane N cut it by 10x but the window count is still the opponent's loop length.
