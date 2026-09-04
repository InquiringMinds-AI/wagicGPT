# deck123 -> core-prompt / render proposals (layer-routed)

Engine BUGS are in the seat file's ENGINE list, not here. Below are prompt-text and render-shape
proposals only. Counted from the rendered `prompt` of my six deck123 seat logs.

## G57-1 (core prompt, reply protocol). THE PLAN LINE MUST DESCRIBE THE BOARD AFTER THE ROW RESOLVES
The protocol asks for `CHOICE:` then `PLAN:` and says the plan is split off before parsing. It
does not say WHEN the plan is standing. Two replies this corpus are self-contradictory inside one
message:
- `123v162` s31: `ATTACK: A1, A2, A3, A4, A5` / `PLAN: ... attack with all 5. Fire chain on
  opponent's turn.` - the five attackers include every creature that could fire the chain, and
  attacking taps them under the pilot's own Intruder Alarm.
- `123v125` s45-class windows generally: a plan naming a card that the CHOICE spends.
**Proposal:** one clause in the reply-protocol block - *"your PLAN describes the board as it will
be AFTER the row you just chose resolves; if a sentence in it names a permanent or a card that
row removes, taps or spends, rewrite that sentence before you send the reply."* Cheap (one
sentence), non-constraining (it constrains the plan text, never the choice), and it targets a
failure the choice-parser cannot see. Falsifiable: replies whose PLAN names a permanent the same
reply's choice taps or destroys, at my seats, 2 -> 0.

## G57-2 (render, scope). A MENU-LEVEL SUMMARY SHOULD BE COMPUTED FROM THE SAME PREDICATE AS THE ROWS
This is the prompt-shape half of ENGINE E1 (the bug half is in the seat file). Whatever fixes the
predicate, the SHAPE lesson generalises: when the render adds an aggregate line above a list, the
aggregate is the most emphatic sentence on the screen and the model acts on it in preference to
the rows. So an aggregate is worth its tokens only when it is derived from the row verdicts
themselves rather than computed independently - otherwise the two can disagree and the aggregate
wins. Same shape to watch on `INCOMING THIS COMBAT` (9 of 9 correct at my seats this corpus) and
on the pregame `counted TWICE above, on purpose` note (0 renders here, no MDFC in this sixty).

## G57-3 (render, cheap). THE HOLD ROW EARNED ITS TOKENS; THE DECLINE COUNTER MAY NOT HAVE
`Hold priority for the rest of this turn ... {taking this row skips the rest of this turn's
identical windows}` rendered on **350 of my deck123 prompts and was taken 140 times**, and
**0 of 289 hold takes across both my decks were followed by a byte-identical same-turn re-ask**
(wave 56: 8 of 119). That row is paying for itself.
`[you declined this exact list N times already this turn]` rendered on 105 deck123 prompts and
276 across both decks, reaching **N = 43** once (`130v126` t16). It produced 0 decline->cast
flips at my seats in two corpora. It is ~60 characters on a large fraction of windows for a
counter the guide explicitly tells the pilot to ignore. **Proposal:** measure it in the log-window
A/B rather than remove it blind - but if a budget cut is wanted, this is the cheapest line at my
seats to lose.

## G57-4 (render, keep). LINES THAT MEASURED CLEAN AT THIS SEAT, FOR THE RECORD
- `Opponent life trend:` on **1,025 of 1,025 board frames** across both my decks (D15 PASS), and
  it is load-bearing at deck130 (see that file's G57-6).
- `{taking this row skips ...}`, `{leaves N sources - no other row on this menu needs more than N}`
  (253 deck123 prompts / 638 rows / 45 takes), the discard verdicts (36 of 36 rows annotated,
  wave 56: 357 of 426 bare), and `Their untapped sources:` (435 of 435 prompts).
- Narration is **56.9%** of my mean prompt (10,939 chars, 12 seat logs, 1,067 records) against
  the corpus's 60.8% last wave.
