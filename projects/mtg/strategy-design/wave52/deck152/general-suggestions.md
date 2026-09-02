# deck152 -> core-prompt / render proposals (layer-routed)

## G5 (prompt, HIGH) - the same declined option set should not be re-asked at every phase stop
This is the seat file's E-2 stated as a prompt-layer ask rather than an engine one, because there
is a cheap prompt-side version: the ask already knows the option set and the board. **375 of my
1,351 records** are the identical set, asked 3+ times inside one turn, declined every time -
64.9 min of the 579.8 min my seats spent. `152v125` alone contributes 11-13 asks per turn on
T16/T18/T20/T22/T24/T26. Proposal, in order of preference: (a) skip the re-ask while the option
set and the board hash are unchanged since the last decline in this turn; (b) failing that, ask
once per turn-half and once more after any stack resolution; (c) failing that, carry the count on
the row - `[you declined this exact list N times already this turn]` - which at least lets the
guide say "PASS" without re-deriving. Note that the offers themselves are correct and wanted:
lane I is what made the flash Dictate visible on the opponent's turn at all.

## G6 (render) - the `{paying this taps: ...}` clause should also appear on ACTIVATION rows in the
phase where it is a mistake, and say which phase is safe
The clause is excellent and it is what 152-I is now keyed to. One addition, paid for by
`152v162` s27: the clause appeared on a Katilda **activation** in Upkeep exactly as it appears on
a cast row, but nothing on the row says the same activation will be offered again after combat.
The Lair of the Hydra row already carries that form (`[Upkeep offer: this animation lasts only
until end of turn, and the same row is offered again in your main phase ...]`). Extending the
"offered again later this turn" note to any row whose `{paying this taps:` clause names a
creature would make the pass self-evident: `{paying this taps: Luminarch Aspirant x3 - they
cannot attack this turn; this row is offered again in your main phases}`.

## G7 (render) - the converter block should state which branch the board is in
The `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond` block is long, accurate
and correctly worded. What it does not say is the thing every guide branches on: whether the
OTHER half is present. `152v126` s24/s45 are two windows where the pilot read the block, wrote a
both-names conclusion, and held five unblockable attackers. Proposal: append the count and the
missing name to the block - `... theirs - Sanguine Bond (1 of the 2 loop halves; Exquisite Blood
is NOT on the battlefield, so damage you deal them gains them nothing)`. That is a fact the
engine already has, it is the fact the branch turns on, and it would have answered both windows.

## G8 (prompt, small) - the BLOCKS format line could carry the one-per-blocker constraint
Two of my seats' five fallbacks were `multiblock_reask` from a repeated B-number (`B1:A1, B2:A4,
B1:A5` and `B2:A1, B2:A2`). The re-ask worked both times, so this is an efficiency item, not a
correctness one. The blocker ask's format line names the syntax; adding *"each B-number at most
once; several B-numbers may share one A-number"* to it costs one clause and removes a whole
fallback class. The A-line that invites the mistake already exists and is well worded:
`no 1-on-1 result exists - only a block by TWO OR MORE of your creatures counts`.
