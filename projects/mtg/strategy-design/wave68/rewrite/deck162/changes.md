# deck162 (Draw and Die) - guide rewrite under Amendment 332

## (a) Rules KEPT (one line each)
- Land drop: play a land every turn it is offered; never "Play no land right now"/"Hold"; pick the colour the hand needs, black first.
- Trust "Mana available"; generic is not a colour; a printed cast row is payable; no holding mana on your own main for your own sorcery-speed card.
- Rule 0c BODY TURN (M > N and life <= 10): every affordable creature row in one window, cheapest per body, no noncreature row.
- On a body turn with one paid body affordable, Fate Unraveler goes ahead of Master of the Feast.
- Rule 1: any punisher in the list is cast this ask, ahead of everything (Fog Bank exception at life <= 8 with a creature opposite).
- Rule 1a/1b: Fate Unraveler first when your line reads 0 creatures and theirs shows an unrestricted attacker.
- Full cast order 0-7 (body turn, punisher, Peer, Sphere, Fog Bank, Caress, engines, pass).
- K is read off the row and counts only the three punishers; the row prints "in your hand:" twice and the trailing one is the Caress/discard count.
- A Caress is never in K; it converts at their cleanup only, for cards held over seven.
- K >= 1: cast every engine every turn, punisher still first in the same ask.
- K = 0: first Howling Mine is still cast; the "[second copy:" tag identifies the braked one.
- First Mine vs a punisher on one unaffordable ask -> punisher.
- Brake release needs "draw converters in your hand: 1+" plus condition (i) colour and (ii) board (their attackers > your creatures and life <= 15).
- Last-ditch release: SYMMETRIC row + hand 0 + no wall/punisher row + crack-back <= 10.
- Master of the Feast at K of 0 only when their line reads 1+ creatures.
- Pre-send check on the chosen row; a bad board is not a release; plan-sentence check.
- Rows without "{feeds:" are exempt; "as soon as possible" is not a reason to decline a cheap permanent.
- {reserve:} clause naming a punisher -> decline, cast the punisher in main phase (outranks both K branches); run that subtraction yourself when no clause prints.
- The reservation survives the re-ask; the same row returning gets the same answer.
- Hold-priority row when "[you declined this exact list N times...]" prints, else "Cast nothing right now".
- A "[legendary:"/"[second copy:" duplicate is not a play.
- Shield Sphere every time it is listed; Fog Bank on sight of any enemy creature, even tapping out.
- Blocking overrides the combat prompt's generic hint.
- Tramplers are never fully blocked; leftover still hits; last blocker never goes on a trampler over an equal non-trampler.
- Lethal-header survival arithmetic (total, erase, highest-life tiebreak, hopeless case).
- Your only converter never blocks onto a line that kills it; lifelink/gain clauses are added to the line's cost.
- Block rungs 0-5 (8-life gate, free kill, neither-dies, survivor preference, dying line, BLOCKS: none).
- Attack: empty tag = send; all-friendly tag with no trigger/lifelink = send.
- One fatal name, any GANG BLOCK clause, or a collapsed tag with N >= 2 = ATTACK: none.
- Fate Unraveler never attacks into a dying or both-die tag.
- "LIFE-TO-DAMAGE CONVERTER: theirs" -> send only attackers with no trigger/lifelink clause.
- Last untapped body at life <= 8 stays home (usually Master of the Feast; Intruder Alarm note).
- Lethal check: Ob Nixilis -2 burst = 2 x P; then the slow count of engines x P plus unblocked power.
- Forced sacrifice order (Sphere, Fog Bank, Master, Fate Unraveler last); ignore the pay-the-least tie-break for a punisher.
- Ob Nixilis -2 targeting: token-maker first, never a token beside its maker, pass if only your own creature is offered; Peer's second question is always "target opponent".
- Sanguine Bond + Exquisite Blood: keep racing; LOOP SCOPE is correct; do not let your own life move.
- Mulligan branches (2+ lands keep, all/zero lands ship, one-land at 7/6/5), tiebreakers, colour is never a gate.
- Bottoming and cleanup-discard orders, "PUT: n, m" numbers-only format.
- Card facts for all 12 nonland cards (Forced Fruition verified as SEVEN cards against its primitive).

## (b) DROPPED, by category
- "WHAT THIS COST" / "WHAT THIS BOUGHT" narratives with game, seq and matchup citations: ~24,000 B.
- Per-window counts of past behaviour ("five takes", "42 windows", "six windows in one game"): ~3,500 B.
- Corpus/wave/census framing, "this bullet used to say the opposite", guide self-commentary: ~2,000 B.
- Repeated restatements of the same rule in three or four places (the K brake was stated ~6 times): ~14,000 B.
- Long anti-pattern essays reduced to the shortest clause that names the false sentence: ~7,500 B.
- Situations list cut from 24 bullets (mostly one-line restatements of rules above) to 5 worked situations: ~3,500 B.

## (c) Rules that could NOT be restated as condition -> action
None dropped for that reason. Two were compressed rather than dropped because of the byte ceiling: the
lethal-block arithmetic lost its explicit 5-step numbering (same content, prose), and the "sentences that
are always false" collection is folded into the rules it belongs to instead of standing as its own list.

## (d) Verification
- `stat -c %s` = **19993** bytes (ceiling 20000).
- `grep -ciE '...'` = **0**.
