# deck123 - core-prompt / render proposals (wave 50), layer-routed

Engine defects are in `wave50/seats/seat-123-130.md` (H1-H2, M1-M3, L1-L2). These are the
prompt/render proposals; each names its evidence and its layer.

**G1 (render, repeat row) - print the stop-side numbers beside the count.** The row now prints
`[you control M creatures right now; ...]` and the overshoot fell from 300 to 28. The two numbers
the pilot still has to fetch from elsewhere are `Opponent life:` and the opponent's creature count.
Ask: the repeat row's bracket also carries `opponent life L, they control C creatures` so the
subtraction's three inputs sit on one line (123v162 seq 34: the plan read C as 1 with the line
saying 2; 123v130 seq 26: stop written as 24 with C 4 on the screen). Layer: render (lane Z's row).

**G2 (core prompt, reply protocol) - a "this window:" conflict between CHOICE and PLAN is a
re-ask.** Seat M1: four replies whose CHOICE named x<N> while the PLAN line beneath said `this
window: pass`. The protocol executes line one. Ask: when a reply's PLAN line contains `this window:
pass` / `stop reached` and the CHOICE line carries a count > 0 on a repeat row, one `[RE-ASK]`
quoting both lines (`plan_choice_conflict`). Layer: lane Y's CHOICE grammar.

**G3 (render, cast rows) - ownership tags on the Tutor and on any permanent already controlled.**
Seat M2: `Cast Idyllic Tutor` with the Alarm on the line (123v125 seq 118, 139; wave 49 x3);
`Cast Intruder Alarm` with one out (seq 156). Ask: `{Intruder Alarm is already on your battlefield
- this finds only an enchantment}` on the Tutor row when any Alarm is owned; `{a copy is already on
your battlefield}` on a permanent's cast row (R67 / #125). Layer: render.

**G4 (render, edict rows) - name the class the sacrifice would remove.** The Devour Flesh row now
says who gains (lane X, correct at 123v152 seq 6 and 123v126 seq 21). The decision that remained
was "is this 1/1 worth a card": Katilda (a mana lord) was declined, Perimeter Captain (a defender)
was declined, one right, one wrong. Ask: on `they control 1 creature - <name> is sacrificed` rows,
append the creature's keyword tags as the battlefield line prints them (`[defender]`, `[lord]`/its
first card-text clause) so the guide's "defender or power 0" key is on the row. Layer: render;
small.

**G5 (render, land-drop ask) - mark enters-tapped lands on the Play rows.** 123v162 seq 6: `Play
Arcane Sanctum` chosen over `Play Underground Sea` with a three-mana spell castable off the
untapped one. The hand line prints `(land: taps for {W}{U}{B})` but not "enters tapped". Ask:
`Play Arcane Sanctum {enters tapped}` / `Play Isolated Chapel {enters tapped - you control no
Plains or Swamp}` on the Land drop rows. Layer: render.

**G6 (render, attackers) - price Lightmine Field on the attacker tag.** 123v125 seq 39, 63: two
2/2s sent into `Lightmine Field` twice, both dying each time; the tag priced blockers ("no creature
they control can block this attacker") and not the Field. Ask: when an opposing permanent damages
attackers by their count, the attackers header carries `{Lightmine Field: each attacker takes N
where N is the number you send - your 2/2s die at 2 or more}`. Layer: render.
