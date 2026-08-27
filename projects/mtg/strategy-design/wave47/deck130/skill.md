# Strategy-writing skill proposals from the deck130 seat, wave 47

One proposal. deck130 had a single decision-losing violation in 400 decisions, and its cause was
structural rather than doctrinal, so this is the only thing the evidence pays for.

---

## S47-3 - A CARD SECTION THAT RESTATES A PROCEDURE IS A SECOND SOURCE OF TRUTH, AND IT WINS

**Evidence.** deck130's rule #2 STEP 1 says: *"Find the row with the LARGEST 'kills THEIRS' list
whose 'YOURS:' reads 'none'."* Its KEY CARDS entry for the same card, 100 lines later, said:
*"Pick the smallest X that reaches the toughness you must kill."* At
`...deck130-0x562ec751eb60-vs-...deck126.jsonl` seq 37 the pilot answered X=3 - exactly the
smallest X that reached the toughness it named - with an X=4 row one line above reading
`kills THEIRS: Pride Guardian x2, Perimeter Captain; YOURS: none`. It followed the card section.
This is the SECOND consecutive corpus in which a deck130 failure traced to a card-section
sentence contradicting a rule-section procedure (wave 46: Dwarven Blastminer, gated in HOW THE
DECK WINS and ungated in his KEY CARDS entry, 2 of 2 violations, one of them lethal).

**Proposed amendment text.**
> These guides carry a rules section and a card section, and the card section is read at the
> moment of decision because the card's name is on the row. Any procedure that appears in both
> is therefore a fork, and the corpus says the CARD entry wins. So: a card's entry must never
> restate a procedure - it may only NAME the rule that owns it ("the X is chosen by rule #2
> STEP 1"). Before shipping a guide, take every card that owns a rule and diff its two
> appearances; a paraphrase is a contradiction waiting for a window. Two waves running, the
> highest-yield edit at this seat has been DELETING a card-section sentence, not adding one.

**Falsifiable form.** After this wave, a guide passes the check if, for every card named in both
a rules section and a card section, the card section contains no imperative about WHICH option
to pick - only a pointer. deck130's wave-47 guide now satisfies this for Starstorm and for
Dwarven Blastminer; the other card entries were not audited this wave and should be.

---

## Adjacent, recorded but NOT proposed as an amendment
The same window shows the pilot answering its CARRIED PLAN verbatim on a menu the plan predates
(`Cast Starstorm at X=3 ...` written at the cast row, replayed at the announce row). This is the
same shape as the wave-46 pre-game mulligan trap, which was fixed in the RENDER (the plan block
is now suppressed on pregame asks) rather than in the skill. I have put the guide-side tripwire
in deck130's rule #2 as STEP 0 and routed the render half to ENGINE M1, and I am deliberately NOT
proposing a skill amendment for it: one seat, one window, and the fix that worked last time was
an engine fix.
