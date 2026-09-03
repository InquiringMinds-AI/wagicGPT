# deck130 -> core-prompt / render proposals (wave 55)

Layer-routed, and short: lane C solved this seat's two standing render asks (the X marker and the
kill-list ownership split), so most of what wave 54 filed here is closed. Engine defects are in
`wave55/seats/seat-123-130.md` (E1-E7); the one that matters most for this deck is **E2**, the
ability row's false opponent life, and it is a bug rather than a design ask so it is not repeated
here.

## G7. CLOSED FROM WAVE 54, RECORDED SO NOBODY RE-ASKS.
- **G6/G7 (always mark exactly one X row; put the mark on the BOTTOM of a collapsed run)** -
  **BOTH SHIPPED AND BOTH WORK.** 3 of 3 X menus carried a marker; on both menus with a collapsed
  run the mark was on the bottom row (`130v152` s46 `X = 4 {same kills as X=7, for 3 less mana}`,
  `130v146` s40 `X = 1 {same kills as X=3, for 2 less mana}`), and both takes of a marked row were
  correct. A second marker string was added for the no-free-row case
  (`[<- best trade: the most of THEIRS at the smallest cost to YOURS ...]`) which wave 54 did not
  ask for and which is the right shape.
- **G8 (single-target `{kills:}` naming the caster's own permanents unmarked)** - **SHIPPED**:
  71 rows now read `{kills whichever you target: THEIRS - <names>; YOURS - <names>}`. `130v146`
  s23's wave-54 repro (`{kills: Plains #1, ... Mountain #5}` with five of the seven the pilot's own)
  renders correctly this corpus.
- **G4 (fetch rows naming no colour)** - **SHIPPED at the other seat and better than asked**:
  `and it adds {W} or {B} (you cannot make {W} right now)`.
- **G9 (ability rows with no life arithmetic)** - **shipped, with the wrong number.** See E2.
- **G3 (library-reveal menus not collapsing duplicates)** - **SHIPPED**: ranged rows with
  `(copies 1-3 of 3 in this list) ... x3`, and the same collapse now runs on the Cleanup discard
  menu.

## G10. RUN THE CAST-ROW DEAD-VERDICT FAMILY OVER THE CLEANUP DISCARD MENU.
Filed as E4 in the seat file; repeating the ask here because it is a render-design choice rather
than a defect. Of **146** rendered discard rows across my 19 windows, **21** carry an annotation.
The predicate that fires is narrow (a creature-targeting instant with 0 legal targets ->
`{dead right now: 0 legal targets on the board for it}`; a land beyond the third ->
`{spare: you control N lands}`), and on the same menu an edict at `they control 0 creatures`, an
Idyllic Tutor with its only target already owned, and a Damnation on an empty board all print
nothing. `{you already control one}` rendered 0 times at my seats.
**Ask.** Reuse the verdicts the CAST rows already compute for the same cards, so that the absence
of a verdict on a discard row means "this card is live". Until then no guide can teach
"the unannotated row is the keep" - which is the rule a pilot most wants at that ask, and which
would be free.

## G11. TAG THE DEARER OF TWO ROWS THAT NAME THE SAME CARD WITH THE SAME VERDICT.
Filed as E5 (deck123's `123v152` s104 is the repro: `Cast Damnation {2}{b}{b}` and `Cast Damnation
{4}{b}{b} [from exile]` with byte-identical `{right now: ...}` clauses, and the pilot answered the
dearer one). Recording it on deck130's list too because this deck will meet the same shape from the
other direction: it runs 2 Rorix and 3 Siege-Gang, and a flashback/exile row from any pool deck
produces it. The X menu already solves exactly this problem with
`{X pricing: same kills as X=N, for K less mana}`, and the asked string is its twin:
`{same effect as row N, for K more mana}`.

## G12. THE MARKER IS A RANKING, AND THE PROMPT COULD SAY THAT IT IS NOT A RECOMMENDATION TO CAST.
**Evidence.** `130v146` s34 -> s36: the cast row's `{X pricing: ...}` and then the X menu's
`[<- best trade: the most of THEIRS at the smallest cost to YOURS - at X=2 that is THEIRS:
Silverquill Silencer; YOURS: Goblin x2]`, taken, with the PLAN quoting the marker. One of theirs
for two of the pilot's own is a trade this deck's own strategy forbids, and the marker is correct
about what it says - it is the BEST of the rows on that menu - while the menu itself had no
"cast nothing" row, because that decision was one screen earlier.
**Ask (small, and optional).** On a marked row in a no-pass-row menu where the marked X's YOURS
list is longer than its THEIRS list, extend the marker by four words: `... (this is the best row
on this menu, not a reason to cast)`. Or, cheaper and arguably better: put the marker's own
`THEIRS n / YOURS m` counts on the CAST row one screen earlier, where `Cast nothing right now`
still exists.
**Guide half shipped regardless** as D55-1 (the WHICH-vs-WHETHER split) - I am not waiting on this.

## G13. NOTHING ELSE. TWO NEGATIVES WORTH RECORDING.
- The `{X pricing:` cast-row annotation is doing its job: 3 Starstorm casts, 0 casts off a row
  reading `even at X=N NOTHING on the board dies` or `affords only X=0`, and `X = 0 {X pricing:
  X=0 - this cast does NOTHING: it deals 0 damage and the spell is spent}` rendered 3 times and was
  taken 0 times. The wave-53 X=0 rung is closed at this seat.
- The wall/converter block (`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond
  ...`, 106 renders across both my seats, plus `their life LOOP is in play` on attacker rows) is
  the surface that flipped `130v126` from a 0-to-44 loss to a 20-to-0 win. No change asked.
