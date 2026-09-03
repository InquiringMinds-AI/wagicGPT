# Core-prompt / render proposals from the deck146 seat (wave 54)
Layer-routed: engine DEFECTS are in the seat file's engine list, not here. These are render and
core-prompt asks.

## R-1 (MED) - the HOLD row and row 0 now share a short name, and three replies could not tell them apart
Lane A's reword made row 0 `0. Pass priority (take no action this window)` and the HOLD row
`N. Pass priority, and do not ask me again this turn unless the board changes (...)`. The reply
format asks for "its SHORT NAME in parentheses". At `146v126` **s54, s56, s57** the reply was
`CHOICE: 3 (Pass priority)` and executed row 3 (the HOLD row); at **s55** it was
`CHOICE: 0 (pass)` and executed row 0. All four are stamped `pass_hold_ambiguous`.
**This corrects the corpus note in the reviewer brief**, which says all 16 stamps are a plain
`CHOICE: 0 (pass)` false positive: 13 are, **3 are not** - they are a genuine name collision the
index happened to resolve. Ask: make the two rows' first two words differ (e.g. keep row 0 as
`Pass priority` and open the HOLD row `Stop asking this turn - pass priority, and ...`), or have
the name-matcher key the HOLD row on its tail clause. Either kills the ambiguity at the source
rather than relying on the index.

## R-2 (MED) - the coverage line's colour clause reads as a verdict; it should carry its own scope
New this corpus: `Playing every land in this hand would not cover any spell in it (you have 3
lands; no spell in it is castable off {W}{U} alone)`. Every word is true, and the trust doctrine
says the pilot must believe it. But it is a TURN-ONE statement rendered as a hand-level
conclusion, and it is the single sentence three guides in this seat spend paragraphs refuting.
7 of the 11 renders corpus-wide are this colour family and one of them shipped a three-land seven.
Ask: append the scope the clause already implies - `... castable off {W}{U} alone **on turn one**`
- or split the render into the two families explicitly (`land count short` vs `colours not yet
paired`). The count family needs no change.

## R-3 (LOW, positive - no change asked) - the modal clause and the DRAW PRICE tail coexist
Lane C's D5 and D10 land on the same row without displacing each other:
`146v162` s46 row 5 carries `{modes live right now: ...; dead (no legal object right now): ...}`
AND `[DRAW PRICE: ... - you would be at 1]` AND the menu-fit clause. 11 of 11 Silverquill Command
rows carry the modal clause (was 0 of 28) and the card was cast twice corpus-wide against 0 last
wave. Recorded so a future lane does not "simplify" one of the three off the row.

## R-4 (LOW) - the reveal chooser's qualify tags are the clearest annotation in the corpus
`146v123` s15 rendered `[does NOT qualify - goes to "put back"]` and `[eligible for "choose a
card"]` per row on a Pelakka Predation reveal. Nothing to fix; naming it so the pattern is
reused when the two-dimensional ability menu (lane D's carried item) gets its collapse.
