# deck126 -> strategy-writing skill proposals (wave 56)

Only what this seat's evidence supports. Amendment numbers continue from #261 (and from
deck125/skill.md's #262-#265, which are proposed alongside these).

## P5 - THE ZERO-HOLD-TEXT CONTROL (#233) HAS A SECOND CORPUS AND A HOLDING RATIO

**Evidence.** Same binary, same model, same corpus, same emitter string on 1,087 rows across my
two seats. deck125's guide carries a seven-line block naming the hold row and telling the pilot to
take it at the FIRST declined window; deck126's contains the string "Hold priority" **zero times**.

| | HOLD renders | takes | rate | opponent-turn rate |
|---|---|---|---|---|
| deck125 (taught) | 685 | 127 | 18.5% | 23.6% |
| deck126 (control) | 402 | 11 | 2.7% | 1.9% |

Wave 55 read 10.1% vs 1.5% on the same split. Both rose; **the ratio held at ~7x** through a
corpus in which deck126's decisions rose 51% and deck125's fell 14%.

**Proposal (#266).** Keep #233's control in force for one more wave and record the ratio, not just
the rates: rates move with the window economy, the ratio has not. Add the finding as it stands -
**guide text is worth roughly 7x on the uptake of a render affordance at this model tier** - and
note the control's cost is measurably nil here (deck126 lost no game to a declined window; its
wasted windows are absorbed by `mana_only_windows_skipped`, 1,482 vs deck125's 0).

## P7 - AN ABSOLUTE ("X IS NEVER YOUR ANSWER") NEEDS ITS EXCLUSION LIST TO BE A LIST, AND THE LIST
## MUST BE CLOSED BY A "WHEN ALL OF THEM" SENTENCE

**Evidence.** deck126's Rule #2 says "while both names are on your battlefield line, 'Cast nothing
right now' is NEVER your answer to a menu that prints any Cast row at all", with two named
exclusions (a zero-Tribute, a `[second copy:` row). In one game (`126v125`) it produced **six** bad
spends: three dead Idyllic Tutors (seqs 15, 30, 32), a second Sanguine Bond (seq 46), two second
Chromatic Lanterns (seqs 167, 215) - every one off a menu whose only other Cast row was already on
the exclusion list. The third dead-row kind existed in the guide (entry 2's dead-Tutor stop) but
not in Rule #2's list, and the absolute outranked it. Control case: `126v152` seq 28/29, a lone
dead Tutor, correctly declined.

**Proposal (#267).** When a guide states an absolute of the form "answer A is NEVER correct while
condition C", the exclusions must be (a) an explicit enumerated list, (b) drawn from every other
rule in the guide that already forbids a row, and (c) closed by a sentence of the form "when EVERY
row on the menu is one of these, the menu is EMPTY and A IS your answer". Absolutes without the
closing sentence convert every other stop in the guide into a break.

## P8 - A ROW'S VERDICT IS ABOUT THE BOARD, NOT ABOUT THE STACK: GUIDES MUST SEND THE PILOT TO THE
## STACK SECTION BEFORE ANY REMOVAL ROW

**Evidence.** deck126 spent 3 of its 10 Tribute to Hunger casts on a creature its own first copy
was already sacrificing (`126v146` seqs 18->19, `126v130` seqs 28->29, `126v125` seqs 261->262).
In each case the prompt's `ON THE STACK` section named the seat's own answer
(`ability: Tribute to Hunger's gain life equal to its toughness (aimed at Silverquill Silencer)
[from your Tribute to Hunger]`) and the row's `{right now:}` verdict, correctly describing the
board at that instant, still read `<name> is sacrificed, you gain 2`. The deck runs two Tributes;
in the game where it burned both on one creature it had none for a nine-attacker board and lost
by 10.

**Proposal (#268).** Any guide rule that fires on a `{right now:}` verdict naming a victim must be
written with a preceding read of `ON THE STACK`: "your own spell on the stack is the answer already
given". Generalise it in the skill as a written rule - **a per-row verdict describes the board, and
the stack is not the board** - because the same shape is available to every removal, counter and
edict in the pool. The render half (a note on the row) is filed as a MED engine item; the guide
half is cheap and does not wait on it.

## P9 - WHEN A NEW LITERAL SHIPS THAT PRICES A DECK'S CORE ENGINE, THE GUIDE MUST NAME IT IN THE
## RULE THAT OWNS IT, IN THE SAME WAVE

**Evidence.** Lane B shipped
`{blocking trigger, this combat: you may gain N, and if you do your converter takes N off them}` -
45 rows at this seat - which is the exact sentence deck126's whole win condition is made of.
The guide's blocking rules were still written around the OLD parenthesis
(`your blocker dies, attacker lives`), so at `126v146` seq 37 the pilot read "your blocker dies",
benched its only blocker, forfeited 2 damage stopped + 2 life + 2 off their total, and lost that
game at -10 against **10**. The same wave's `; this KILLS you` header (3 renders) went 0-for-1 on
maximising damage stopped for the same reason.

**Proposal (#269).** Add to the reviewer's checklist, above the prediction adjudication: **for each
NEW literal in the wave's lane briefs, ask whether it prices this deck's win condition or its
death condition; if it does, the rule that owns that condition must quote the literal this wave,
not next.** A shipped affordance the guide does not name is, for this model tier, a shipped
affordance the pilot does not use - which is the same finding #233's hold-row control measures
from the other side (P5).
