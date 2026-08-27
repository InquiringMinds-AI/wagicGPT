# deck130 seat - core-prompt / render proposals (wave 47)

Engine items are in `seats/seat-123-130.md` (M1 ANNOUNCE_X carried plan, M2 missing
`[DRAW PRICE:]` row tag, L1 cycling row without a `[cost:]` bracket, L2 mana-line number word).
This file is prompt text and shape only.

---

## G47-4 - The X menu prints one axis and needs two

**Observation.** The announce rows carry, per row, either a kill list or the collapse
`{X pricing: same kills as X=N, for K less mana}`. Every comparative phrase on the screen is
about MANA SAVED; nothing marks the row that kills the most. At
`...deck130-...-vs-deck126.jsonl` seq 37 the pilot took X=3 with a strictly larger free row
above it, and at wave 46 the same seat took X=1 over a free X=2. The rule the guide states -
largest kill list whose `YOURS:` is `none` - is a property of the whole menu that no single row
announces.

**Proposal.** One marker, once per menu, on the row the guide's own procedure selects:
`2. X = 4 {X pricing: kills THEIRS: Pride Guardian x2, Perimeter Captain; YOURS: none}
   <- most kills at any affordable X that costs you nothing`
This is not a recommendation, it is a summary of the rows already printed - the same status as
`option 1 is the LARGEST X`, which the header already states. It gives the maximal fact the same
visibility the minimal fact has had since the cast row's
`smallest X that kills an OPPONENT creature` clause.

---

## G47-5 - Where a summary line exists, the ROW should carry the number too

**Observation.** The `DRAW PUNISHERS on the battlefield: ... Every card YOU draw costs you N life
to theirs.` line shipped and it works: 19 windows at this seat, 0 voluntary draws, and one
window where the pilot ran the check in the NEGATIVE direction and correctly cycled at 3 life
with no punisher out. But at the 5 windows where a `cycling with Forgotten Cave [cost: {r},
Cycle]` row shared the screen with the summary, that row printed a mana cost and nothing about
the life cost. Lane J's `[DRAW PRICE:]` tag is designed for exactly this and is not emitting
here (ENGINE M2).

**Proposal (design principle, beyond the one tag).** When the frame carries a summary of a hidden
cost, the ROWS that incur that cost should carry the number, not only the frame. The two
existing summary lines - `LIFE-TO-DAMAGE CONVERTER` and `DRAW PUNISHERS` - are both of this
shape, and both are read at the top of a 15-25 KB prompt and acted on at the bottom. The row is
where the decision is; the frame is where the fact is. Closing that gap is the same move that
made the land drop's consequence sentence work.

---

## G47-6 - Menu rows in one menu should carry the same annotation shape

**Observation.** `1. cycling {card text: "Cycling {3} (...)"} ` beside
`2. Cast Card Normally [cost: {r}{r}{x}] {card text: "Starstorm deals X damage to each
creature."}`. One row has a `[cost:]` bracket, the sibling does not, and the sibling's cost is
buried inside a parenthetical in its card text. No failure this corpus (0 of 4), so this is
parity, not a defect - but it is the last unannotated row shape I can find at this seat, and the
menu it lives in is the one that cost this deck a card in two previous corpora.

**Proposal.** Give the alternative-cost row its own bracket from the `autohand=__CYCLING__({3})`
value the parser already has: `1. cycling [cost: {3}, Cycle] {discard this card and draw 1}`.
