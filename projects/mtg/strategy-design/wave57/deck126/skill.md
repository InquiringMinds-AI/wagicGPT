# wave-57 proposals for the strategy-writing skill — from the deck126 seat

Evidence base: `matchups-20260903-174505`, deck126's 588 decisions across 6 seat logs, and the
three-corpus zero-hold-text control. Each proposal names the corpus fact that pays for it.

## #S7 (STRONG) — THE HOLD-ROW CONTROL (#233) IS SETTLED. RECORD THE RESULT AND STOP SPENDING A DECK ON IT

Three corpora, same emitter string, same model, same binary generation, two guides:

| corpus | deck125 (taught) | deck126 (control, zero text) | ratio |
|---|---|---|---|
| wave 55 | 10.1% | 1.5% | 7× |
| wave 56 | 18.5% | 2.7% | 7× |
| **wave 57** | **15.9%** (opp-turn 19.8%) | **0.6%** (opp-turn **0 of 336**) | **26×** |

The answer to "how much of the hold row's uptake is the ROW and how much is the GUIDE" is: almost
all of it is the guide, at every uptake level the row has reached. The control cost deck126
nothing measurable in any of the three corpora (its wasted windows are absorbed by
`mana_only_windows_skipped`, 385 this wave; no game at this seat was lost to a declined window).

Proposed skill amendment: **#233 is answered — an emitter row that is not taught is taken at
roughly a twentieth of the rate of one that is. Record it as a finding about GUIDE UPTAKE
generally, not about the hold row, and retire the control.** deck126 stays text-free in wave 58
only as a cheap fourth data point, and the seat file predicts the ratio holds above 5×; if the
synthesis wants the deck's opponent-turn windows back, it can teach the row in wave 58 and read
the delta directly.

The transferable claim, which is the reason to keep this in the skill at all: **the engine can put
a correct, well-priced row in front of this model tier for a thousand renders and it will be taken
at under one percent until the guide names it.** Every render proposal we score as "the pilot will
see it" should be discounted by that.

## #S8 (STRONG) — AN ENTRY-ORDERED LIST NEEDS ITS TOP ENTRY RE-READ FIRST, EVERY WINDOW

deck126's Rule #7 is a strict priority list. It broke four times in eleven windows, and three of
the four are one game and three consecutive casts: at 13 life it took entry 4 over entry 1, at 10
life entry 7 over entry 1 (and outside entry 7's own carve-out), and then a dead entry 2. Its
zero-value ceilings in the SAME game were perfect (0 of 445 zero-Tribute rows, 0 spends off 405
all-dead menus). So the failure is not recognition of dead rows; it is that the list is walked from
wherever the pilot's attention landed rather than from the top.

Proposed skill rule: **a guide that governs by an ordered list must state the top entry's test as
a per-window READ instruction, not as a ranking** — "read the menu for these two names first, every
window, before you read anything else on it". Ranking language ("take the highest entry that
appears") describes the answer; reading language describes the procedure, and this model tier
executes procedures more reliably than it executes comparisons.

## #S9 (STRONG) — REPLY FORMAT: ANSWER-FIRST CONVERTS A CHANGE OF MIND INTO A LOST WINDOW

`126v146` seq 34, at 2 life on turn 20: the reply opened `CHOICE: 3 (Cast Staff of Nin)`, then ran
~900 words that correctly worked out that row 1 (Cast Exquisite Blood) was the play, and closed
`So CHOICE: 1.` The engine stamped `retracted_choice` and the seat's last live window was spent on
nothing. **The reasoning was right and the game was lost to the format.**

This is a skill-level fact about the protocol we write guides against, not a deck fact: the
answer-first contract asks a model that thinks in prose to commit before it has thought. Guides can
only mitigate it ("do the deciding before you write line 1" — deck126 edit D57-2). The seat file
carries the engine-side ask (MED #5: take the LAST `CHOICE:` line on a naturally-terminated reply).

Proposed skill rule: **every guide whose deck has ever produced a fallback should carry one reply-
discipline line, and the skill should say what it is** — one CHOICE line, decided before written,
never a second. Frequency is low (1 in 3,317 corpus-wide) and cost is total (a whole decision, and
in this case a game), which is exactly the shape that a one-line rule is for.

## #S10 (MEDIUM) — A GUIDE MUST NAME THE SUBSTITUTE WHEN A PRICE TURNS ITS TOP RULE OFF

At `126v146` seq 34 the guide's entry 1 said "both names missing, both rows printed: TAKE SANGUINE
BOND" and the Bond's row said `[NAMED BY THEIR Silverquill Silencer #2: casting this costs you 3
life ... - you would be at -1; this KILLS you]`. The pilot had to resolve a collision between a
guide absolute and a printed lethal price with no instruction covering the pair — and spent the
window doing it.

Proposed skill rule: **wherever a guide states an absolute, and the engine can print a price that
makes obeying it lethal, the guide must state the exception AND name what to take instead.** "Go
down the list" is not a substitute when the list's next entry is also priced. Cheap to check: grep
the guide for its own absolutes, then grep the corpus for a lethal-price string on the same card.

## #S11 (MEDIUM) — COUNT A GUIDE'S CARVE-OUTS AGAINST THE BOARD THAT BROKE IT

deck126's Chromatic Lantern entry has a carve-out that moves it above the walls only at "Mana
available: 4 or less". The seat took a Lantern over entry 1 at **six** sources. The rule was not
ambiguous and it was not close; the carve-out simply was not read.

Combined with #S2 from the deck125 seat (one rule, one place) this is the same underlying claim
from the other direction: **carve-outs are where this model tier loses the thread, and a seat file
should report, for every break, how many clauses of the guide were live at that board.** Wave 57's
two decided losses both score >= 2. That number is a better regression signal than the win rate.

## What this seat does NOT propose

- No hold-row text for deck126 in wave 58 (see #S7).
- No change to the blocker doctrine: the wave-56 blocker edits went 9 of 9 this corpus (every
  available blocker assigned in every window, 0 `BLOCKS: none`), so that section is at rest.
