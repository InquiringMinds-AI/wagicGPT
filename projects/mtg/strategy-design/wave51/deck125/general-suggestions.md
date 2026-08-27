# deck125 - general (core-prompt / render) suggestions, layer-routed (wave 51)

G1 (render, cast rows): extend lane E's `{spends K of your M untapped mana sources this turn;
<card> in your hand needs K}` clause from ability rows to own-turn CAST rows whose `leaves N` is
below the cheapest instant in the hand: `Cast Staff of Nin {6} {leaves 1 of your 7 untapped mana
sources untapped; Dream Fracture {1}{u}{u} in your hand needs 3}`. Evidence: `deck125 vs152` seq
24 and `vs162` seq 24 (the gate sentence in the plan line, the row taken anyway - #102 exhausted on
the guide side); the ability-row clause at `vs130` seq 8 is the emitter path to copy. Guide
counterpart: W27.

G2 (render, X announce): the X menu's header could carry the seat's life when it is at or below
the biggest attacking power on the opponent line: `Your life: 1 - the largest X here gains 7`.
Evidence: `vs152` seq 39 (X=7 offered at 1 life, not cast). Guide counterpart: W30. Low priority -
the guide edit may suffice.

G3 (narration): the chooser-attributed `You used: Gain life equal to its toughness with Tribute to
Hunger targeting Emrakul` (`vs123` seq 53) and `Opponent used: Sacrifice with Emrakul, the Aeons
Torn targeting ...` (seq 51) - see seat engine #4. Cosmetic.

G4 (core prompt, not deck): the opponent's deck NAME is not in the prompt (no line matches
"deck"); every opponent-keyed rule must key to printed cards or the log, which is what the guides
do. No change requested - recorded so nobody writes "against Sanguine Blood" into a guide.
