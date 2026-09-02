# deck123 -> strategy-writing skill proposals (wave 52)

Only proposals my own seat evidence supports. Each names the amendment it would extend and the
seq that pays for it. The synthesis seat owns the skill; these are candidates, not edits.

## S1 (NEW). A guide sentence that describes a RENDER STRING is a dated citation - date it, or
## the guide teaches a surface that no longer exists.
Three separate deck123 sentences went false in one engine wave: "the Slip's target rows print no
DIES / SURVIVES" (lane L now prints it - v152 s10 `-1/-1 - DIES`, v146 s19 three
`- SURVIVES (toughness N, -1/-1 more kills it)` rows), `[already owned:` (0 renders; three new
brackets), and the bare `[from exile]` (now carries a 20-word clause). The wave-51 carry caught
two of these by hand and missed the Slip one entirely, and the Slip one is the sentence that
cost a card at 6 life.
**Proposal.** The skill should require every quoted emitter string in a guide to be written in a
form a script can check - i.e. exact, on its own line or inside backticks - and should make
"re-grep every quoted literal against the new corpus before writing anything else" the FIRST
step of a reviewer's pass, not a carry-list item. The literal audit is mechanical; the reviewer's
judgement is not, and spending judgement on it is waste.

## S2 (extends the trust doctrine). When the render's clause answers a DIFFERENT question than the
## guide's clause did, the guide must be re-keyed even though nothing is "wrong".
`[already owned: Intruder Alarm on your battlefield - this finds only an enchantment card]` told
the pilot the card was DEAD. Its replacement,
`[second copy: you already control Intruder Alarm; both stay on the battlefield - no legend rule]`,
tells the pilot the cast is LEGAL. Both are true; only the first carried the verdict. The pilot
cast a second Alarm four times under the new clause (v126 s74, v162 s39, v125 s41, s59) having
cast zero dead second Alarms in the window the old clause covered.
**Proposal.** Add to the trust doctrine a companion clause: never teach doubt of the render, AND
never assume a replaced clause carries the old clause's verdict. When an engine lane rewords a
tag, the reviewer's job is to ask *what question does the new string answer* and put the missing
verdict in the guide.

## S3 (extends the perception-vs-strategy routing doctrine). A decision that has NO pass row is a
## different class from one that does, and guides must move the rule to the window that can
## still refuse.
v146 s18/s19: the CAST row was refusable; the TARGET ask that followed printed "this ask has no
pass row" and offered three `SURVIVES` rows. A rule written at the target window cannot fire.
**Proposal.** The skill should carry a "where does this rule live" test: if the losing choice is
made on a forced ask, the rule belongs on the row that OPENED it, and the guide sentence should
say so in the words the prompt uses ("the target ask that follows has no pass row").

## S4 (extends the amendment on floors and qualifiers). A floor with a qualifier attached is two
## rules, and the corpus will find the hand where they disagree.
Second corpus running (wave 51 deck146, wave 52 deck123 v130 s1). "TWO OR MORE LANDS: keep"
followed immediately by "read the coverage line" produced a ship on a two-land seven whose
coverage line read "would not cover any spell in it".
**Proposal.** In a numbered decision ladder, a rung that ends in a floor may not be followed by a
qualifier in the same rung. Put the qualifier on the rung it actually governs (here: one-land
hands) or state explicitly that it does not apply above the floor. Constructing the twelve hands
(#131) catches this; the skill should say to construct them at every rung whose text CHANGED,
not only for a new section.

## S5. Plan-shape templates should name each input variable separately, or the model fills the
## template and still does the arithmetic wrong.
`PLAN: stop at M = <L + C + 3>; M is <M> now; this window: <x<N> / pass>` was filled at v162 s33
as "Stop creating humans at M=40 (L+C+3 = 20+2+3=25)" - the template's slot and the template's
own formula, both present, disagreeing by 15, in one line.
**Proposal.** Templates that carry a computed number must have a slot for each INPUT too
(`L <>, C <>, stop <L+C+3>`), so the contradiction is visible on the line and checkable by a
parser. Filed also as a general-suggestions parser item.
