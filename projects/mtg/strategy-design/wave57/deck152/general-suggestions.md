# deck152 -> core-prompt / render proposals (layer-routed)

Engine bugs are in the seat file (E-1..E-6). These are prompt/render asks.

## R-3 (MED, restated from deck146's list because this deck is where it bites) - the identical-declined note needs a second number

At deck152's seats the note fired on 73 windows at N>=2 and **0 of them repeated their situation
block**. Corpus-wide: 801 vs 163. Ask:
`[you declined this exact list N times already this turn; the board has not changed since M of them]`.
With M in hand the guide rule becomes decidable and the pilot stops being told "nothing is
happening" while its own stack drains under it. Repro `152v123` seqs 69-82.

## R-4 (LOW) - the pregame mana-source line should count both faces of a land/land MDFC

`152v125` seq 1: `{W} 1, {G} 3` on a hand of three Pathways whose other faces make {W} and {U}; the
deck played those faces 7 times. D2 already reads the `autohand=` bucket for spell-backed MDFCs;
the Pathway class needs the same treatment with different wording, because both faces are lands and
the card must stay counted ONCE in the land tally while contributing TWO colours to the source line.
Suggested form: `{G} 3 (2 of them are Pathways that can instead make {W} or {U} - one face each,
never both)`.

## R-6 (LOW) - an option list that is identical while the stack drains is worth one line of context

Not a fix, an observation with a number: at `152v123` turn 15 the same 3-row menu was asked 12 times
while the stack went from 4 items to 1. Every one of those was a real decision and the seat answered
each in ~27 s. The prompts are 11-15 KB each. If the window economy is ever revisited, the cheap
lever here is not suppressing the window (the decision is real) but telling the pilot **what
changed** since the last identical list - one line, from the diff the engine can already compute for
R-3. That would be the same emitter serving two purposes.
