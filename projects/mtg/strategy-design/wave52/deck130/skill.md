# deck130 -> strategy-writing skill proposals (wave 52)

Only what this seat's evidence supports.

## S6 (NEW, strongest of my two decks). A rule stated once in prose and once in the
## DECIDING SITUATIONS list must be stated with the SAME scope, or the list wins and the prose
## is dead text.
v146 s35 lost that game. The COMBAT prose read "When that attacker is a DRAW PUNISHER named on
the 'DRAW PUNISHERS' line, **or the only creature hitting you**, and your life minus its 'deals
N' is 5 or less, put every blocker the clause counts on it." The DECIDING SITUATIONS bullet - the
recognise->do line the model actually pattern-matches - dropped the second branch. The window
that arrived was the second branch: a lone Silverquill Silencer (3/2), `GANG BLOCK: any 2 of
yours together deal 2, enough to kill this attacker`, three Goblins listed, 8 life, "you would be
at 5". Answer: `BLOCKS: none`.
**Proposal.** The skill should make the DECIDING SITUATIONS list a mechanical DERIVATION of the
prose rules, and require a reviewer check that every condition in the prose appears in the
matching bullet. A one-line diff between the two surfaces is cheap; this cost a game.

## S7. A gate whose input is a COUNT needs to say how to count the render's collapsed form.
"while the opponent's battlefield shows FOUR OR FEWER lands" was broken 8 times in 19 casts. The
lines it is read against print `Island #1-#4 x4; Plains #1-#3 x3; Seachrome Coast` - nine lands
in three visual rows. Guides that gate on counts should quote the collapsed form and say it
counts in full (v125 s61, s70, s75, s95, s97, s100).
**Proposal.** Add a skill amendment: whenever a rule's input is a count read off a battlefield or
option line, the guide must show one collapsed-handle example and state the arithmetic. Handles
are now on cast rows, ability rows, attacker rows, blocker rows and sacrifice rows.

## S8. A "do not do X" rule with no "so do Y instead" is answered by doing X when nothing else
## is on the menu.
Every one of the six face-damage breaks and six land-destruction breaks happened in windows where
the pilot's alternative was "Cast nothing right now" with 9-17 sources untapped (v125 s47, s57,
s61, s70, s75, s83, s95, s97, s100). The guide's prohibition was correct and was read; the
sentence the model needed was that HOLDING the card IS the play.
**Proposal.** The skill should require a prohibition on a card to name what the card is being
held FOR, in the same sentence - not as motivation, as the alternative action. "It is dead for
the rest of the game" and "hold it for the creature that arrives later" are different
instructions to a model that is looking for something to do with its mana.

## S9. Guides for decks facing a known archetype should carry the archetype paragraph, and the
## skill should say which archetypes are load-bearing in this pool.
deck123's guide has had an "AGAINST A DECK THAT COUNTERS (deck125)" paragraph since wave 50;
deck130's had none, and lost 25 to -5 across seventy turns with zero `attackers` records in 117
decisions. The pool's archetypes are small and known (counters = deck125; lifegain walls +
converter = deck126; draw punishers = deck162; land destruction = deck130).
**Proposal.** Add a checklist item: for each guide, one paragraph per pool archetype that has
beaten it, or an explicit note that the matchup has not produced a loss yet. This is cheap
(4-8 lines) and it is the highest-leverage text in deck123's guide by this corpus's evidence.

## S10 (small). "Never draw" rules against punishers should be paired with "kill the punisher".
Rule #0 held 59/59 windows at this seat and the deck still died to forced draws (v162, two
Howling Mines). A defensive gate that cannot be satisfied needs the offensive half beside it.
