# deck130 -> core-prompt / render proposals (layer-routed)

Engine bugs are in the seat file's ENGINE list. Prompt-text and render-shape proposals only.

## G57-5 (render). A DECISION MENU WITH NO DECLINING ROW SHOULD SAY SO, AND SAY WHERE THE DECLINE WAS
Three windows at this seat are scored as misplays by any letter-reading of the guide and are not
decisions at all, because the menu had no row that declines:
- `130v125` s92 and s94 - target menus with exactly `1. The opponent (player, life 26)` and
  `2. Yourself (player, life 20)`;
- `130v162` s8 - an X menu with exactly `1. X = 0 {X pricing: X=0 - this cast does NOTHING ...}`.
The engine is right to offer them (the cast is already committed; legality is not the issue).
But the model is being asked a question with one honest answer, at full inference cost, and the
guide cannot teach anything at that seam. **Proposal, two parts, both cheap:**
(a) when a decision menu contains no decline/hold/pass row, say it once in the menu header -
    *"every row on this menu spends the card; the decline was the previous window"* - so the
    pilot's carried rule is routed to the right seam instead of being violated at the wrong one;
(b) consider not asking at all when the menu has exactly ONE row: `130v162` s8 cost a full model
    round trip to answer a question with one option. (The seat's `identical_option_asks_resolved`
    is 1 in each of two games, so a collapse path exists; this is the N=1 case of it.)
This is UX, not legality: nothing is withheld and no choice is constrained.

## G57-6 (render, keep and extend). `Opponent life trend:` IS THE BEST NEW LINE AT THIS SEAT
It rendered on **1,025 of 1,025 board frames** (D15 PASS) and it is the only line that makes the
CROSS-CHECK latch checkable without memory: `Opponent life trend: turn 34: 28, turn 36: 28,
turn 38: 28, now 31 (+3 since turn 34)`. My D57-1 guide edit is keyed to it. **Extension worth
its tokens:** the same treatment for YOUR life on the same line. `130v126` t16 ran the pilot from
18 to 0 in one phase, one point per window, across 72 decisions - the frame printed the current
number each time and never the slope, so no window said "you are in a loop".

## G57-7 (render). THE `[<- best trade:` MARKER SHOULD CARRY ITS NET, NOT ONLY ITS ARGMAX
`130v152` s31/s33: `[<- best trade: the most of THEIRS at the smallest cost to YOURS - at X=4
that is THEIRS: Sigarda, Champion of Light; YOURS: Dwarven Blastminer, Siege-Gang Commander,
Goblin x3]`. The marker is correct by its own definition and points at a 1-for-5. The pilot
followed it, which is what the trust doctrine says it should do. **Proposal:** print the two
counts in the marker itself (`THEIRS 1 / YOURS 5`) so the argmax and the net are both on the row,
and/or suppress the `best trade` form when YOURS exceeds THEIRS (the `most kills ... that costs
you nothing` form already handles the free case). This is the render half of guide edit D57-5;
the guide half ships either way.

## G57-8 (core prompt). THE X-ANNOUNCE MENU'S HEADER COULD NAME THE CAST ROW IT CAME FROM
Related to G57-5(a) and to my S57-6 skill proposal. On the X menu the pilot has already spent the
card, but the guide - and the model's carried plan - are still reasoning about whether to cast.
One clause in the announce header (*"the cast is committed; this menu only sets X"*) closes that
gap for ~10 tokens. Denominator this corpus: 5 X menus at this seat, 1 of which (`130v162` s8)
had a single row.

## G57-9 (measurement, for the log-window A/B). MY SEATS' NUMBERS
12 seat logs, 1,067 records, 1,043 decisions, 2 fallbacks (0.19%; wave 56 at these seats 0.38%).
Mean prompt 10,939 chars; **narration 56.9%** of it; max prompt 25,772 (`130v125` s133, turn 58).
`log_window` = `full` on 1,041 of 1,041 records that carry the field, `log_window_elided` absent
on all 1,067 - lane H's control arm reproduces clean at my seats.
