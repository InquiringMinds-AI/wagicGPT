# wave-55 deck126 - proposals for the strategy-writing skill

Only what this seat's corpus supports. deck126's guide is close to its ceiling - every wave-54
prediction with a window passed - so these are about the SHAPE of rules that are already right,
plus one new device the corpus forced.

## S7 - A new render string that lands INSIDE an existing one changes what the existing rule reads
## like, and the guide must name the new string the wave it appears.

Blockers STEP 1 keys on the parenthesis `(your blocker dies, attacker lives)`. This corpus that
parenthesis grew a second clause inside it:
`may block A1 (your blocker dies, attacker lives (blocking trigger: you gain 3 and may gain 2
more))`. Both halves are true; the new half prices the block at 5 life and the old half prices it
at one wall. At `126v146` seq 6 the seat blocked with both walls at 20 life against a lone 1/1
[first strike, deathtouch], banked 7, lost Pride Guardian, and five turns later met five attackers
with one blocker.

Proposal: an amendment saying **when a lane adds a clause INSIDE a string an existing rule already
keys on, the rule must be re-read as the composed string, and the edit quotes the composed string
in full.** A rule that quotes only its own half is a rule the pilot will apply to a different
sentence than the one printed. This is the composition-seam failure at the guide/render boundary
rather than at a fragment boundary, and it is cheap to check: grep the corpus for the rule's quoted
literal and confirm it still occurs as a whole line.

## S8 - Rank ties inside a top-priority entry, or the entry decides by list order.

Rule #7 entry 1 is "Exquisite Blood or Sanguine Bond - whichever NAME is missing". It ran 9 takes of
9 windows this corpus - a rule at its ceiling - and still produced a wrong answer, because at
`126v130` seq 14 BOTH were missing and both rows were printed. The pilot took Exquisite Blood; the
guide's own Rule #3 says the Bond alone is a clock and the Blood alone does nothing you can start.
The entry had a perfect hit rate and no tiebreak.

Proposal: **an entry that can match two rows at once needs its tiebreak written into the entry, not
left to a section elsewhere that explains the difference.** The general check for a guide edition:
for every priority entry, ask "can two rows match this at the same time?" and, if yes, name the
winner in the entry. Cheap, and it caught the only entry-1 error in two corpora.

## S9 - Universals and their exceptions must live in the same paragraph.

(Stated at length as S4 in `deck125/skill.md`; deck126 is where it fired.) Rule #2's "'Cast nothing
right now' is NEVER your answer to a menu that prints any Cast row at all" versus Rule #4's zero
stop and entry 7's second-copy stop. Three windows apart the pilot resolved the same contradiction
in both directions (`126v125` seq 169/185 correct, seq 200 the corpus's only second-copy take).

## S10 - A carve-out that fires 5 of 12 is not necessarily failing - count the windows an entry
## ABOVE it legitimately took.

Entry 7's Lantern carve-out ("moves above the walls when a combo piece is on the hand line and
`Mana available:` reads 4 or less") was taken on 5 of 12 matching menus. Five of the seven declines
took a HIGHER entry - four live Idyllic Tutors (entry 2) and one Rule-7b Tribute - and are correct
by the same guide. The real break rate is 2 of 12, and one of those two was at 2 life where a wall
is defensible.

Proposal: add to the adjudication procedure that **for any rule inside a priority list, the
denominator is windows where no higher entry matched**, and the seat file must report both numbers.
A raw hit rate on a list entry is a fiction of the same family as wave-54's D92 "raw 73, live 5".

## S11 - Do not spend guide bytes on a rule the deck cannot execute.

deck126 got 169 opponent-turn windows and made **zero** casts in them across six games; its only
instant is Tribute to Hunger, and 218 of the 228 Tribute rows read "at 0 this does nothing". Any
future edit that adds opponent-turn instruction to this guide is spending bytes on a seam that has
produced nothing in three corpora. The right place for that waste is the render layer
(`deck126/general-suggestions.md` G3), not the guide.

Proposal: an amendment making **"can this deck ever act here?" a gate on adding guide text for a
window class**, answered from the seat's own cast counts by phase, before the text is written.
