# Deck 125 (Revelation Fracture, UW draw-go control) - wave 66 seat review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-064038/`. Six seat files
`*deck125-*-vs-*`; 592 records, 547 model calls, median latency 6.1 s, p90 47.9 s, max 461 s.
27 of the corpus's 83 `unparsed_reply` fallbacks (33%) landed on this seat.
Record 1/6 (won 125v130 only), after 4/6 last wave on the same guide.

## 1. Game by game, with the deciding decisions

**125v130 - WON 67/0, turn 37** (`1788694869-...deck125-0x55a5b8a611e0-vs-...deck130`).
The guide's plan executed exactly: land every turn, Elixir cast turn 1, two Staff of Nin, ping
every upkeep. Three Sphinx's Revelations at seq 18 (X=3), 65 (X=5), 73 (X=8) - the mana was small,
so option 1 was always under the library ceiling. Elixir activated at seq 62 and 86; library never
dropped below 30. This is the control case for section 3.

**125v146 - LOST 0/20, turn 19** (`...deck125-0x558da9ed83e0-vs-...deck146`).
Decided by a PARSER REFUSAL, not a decision. seq 6 (turn 7, life 17): the opponent's Silverquill
Silencer had named Essence Scatter. The model reasoned correctly - "If I cast Essence Scatter to
counter it, I trigger Silencer's ability: I lose 3 life ... Crucially, I must not cast Essence
Scatter into the Silencer" - and answered `ANSWER: CHOICE: 2 (Hold priority)`. The parser refused
it (`fallback: unparsed_reply`), Baka answered, and the seq 7 `recovery` record's `recovered_by`
narration reads: "You cast Essence Scatter / You lost 3 life (now 14) / Opponent drew a card".
The fallback executed the exact play the model had just ruled out. The window spanned turns 7->10
and 17->7 life; the deck never recovered. See HIGH-1.

**125v152 - LOST -3/25, turn 19** (`...deck125-0x55ff4b9d6530-vs-...deck152`).
19 model decisions, no fallbacks, no misplay I can cite: land every turn, Path, Essence Scatter,
two Supreme Verdicts (seq 9 "destroys 1", seq 14 "destroys 3"), Lightmine Field at seq 19. Staff of
Nin was never drawn, so the deck had no clock. Read this as matchup/variance, not guide effect.

**125v162 - LOST 0/17, turn 14** (`...deck125-0x55c907fcc2a0-vs-...deck162`).
Three refusals (seq 10, 14, 18) in the first 8 turns, one of them 387 s (seq 18, `[+1238 bytes
written past the PLAN line trimmed from this record]`). Staff landed on turn 11 with the deck
already at 14. Life went 20->0 while the seat spent 3 of its first 18 windows on Baka. Variance +
HIGH-1, not a guide item.

**125v123 - LOST 44/3, turn 59** (`...deck125-0x55a1ef3af260-vs-...deck123`). DECKED OUT.
The deciding decision is seq 132: "Your library: 14 cards", two Staff of Nin on the battlefield
(the guide's ceiling is library - 2 - one per Staff = 10), and the seat announced **X = 13** on the
Sphinx's Revelation X menu. Library 14 -> 1; the two Staff upkeep draws at seq 133/134 took it to 0
and the game ended on the next draw with the opponent on 3 life and the seat on 44. The same seam
had already taken 27 -> 14 at seq 126 (X=13, ceiling 23 - legal). Nine refusals in this game
(seq 6, 51, 62, 66, 78, 100, 111, 130); seq 51's refused reply said "I should activate an Elixir
... I will choose option 1" - the Elixir did get activated later (seq 80, 107), so that one did not
decide anything.

**125v126 - LOST 60/5, turn 77** (`...deck125-0x5583a84a5320-vs-...deck126`). DECKED OUT.
Two decisions, both citable:
- **seq 273** (turn 76, 60 life to their 6, one Staff, "Your library: 22 cards", ceiling 19): the X
  menu was answered `CHOICE: 1 (X = 21)` in 3.3 s. Library 22 -> 1. This after seq 272 (47.9 s of
  reasoning on the cast row) had explicitly said "I have 22 cards left in library. Drawing 20
  leaves me with 2. That is dangerous ... I don't need to draw" and then cast it anyway. The full
  deliberation happened at the cast window; the X window got none.
- **seq 277/279** (turn 78, library 0, Elixir of Immortality untapped on the battlefield, row 1 of
  the menu reading "Life with Elixir of Immortality ... Shuffle Elixir of Immortality and your
  graveyard into library"): the seat took the hold row, then passed, writing into its PLAN
  "Do not activate Elixir while Sanguine Bond is on the battlefield (it closes the loop with
  Exquisite Blood in graveyard)." That is false and the guide says so in section #1; the same reply
  chain had it RIGHT at seq 272 ("Sanguine Bond triggers when *they* gain life. My life gain does
  not trigger it"). Verified: `mtg.txt:99005 auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch
  opponent` - controller's lifegain only; Exquisite Blood was in their graveyard, not in play.
  The activation was a guaranteed save from a won board.

## 2. Engine / interface / card items

**HIGH-1 - the parser refuses `ANSWER: CHOICE: N (...)`, and lane AR H2a's own section names are
what produce it.** 10 of this seat's 27 refusals are a reply whose answer line reads
`ANSWER: CHOICE: 2 (Hold priority)` (also `CORRECTION: CHOICE: 3 (...)` at 125v130 seq 52, and
`So PUT: 1, 8.` at 125v126 seq 258 - same shape, a short prefix on the coded line). The reply
protocol (`reply_protocol` in the `system` record) instructs "Write your REASONING first, then your
ANSWER, then your PLAN" and heads its middle section "THE ANSWER LINE"; the model mirrors all three
headers as literal labels and writes `REASONING:` / `ANSWER: CHOICE: ...` / `PLAN:`. The protocol
also promises "the FIRST line carrying that label is the one that runs" - which is not what the
implementation does: the label must start the line.
Repro: `1788694855-ai_baka_deck125-0x558da9ed83e0-vs-ai_baka_deck146.jsonl` seq 6, reply tail
`"ANSWER: CHOICE: 2 (Hold priority)"`, `fallback: unparsed_reply`, `choice: -1`; then seq 7
`recovery.recovered_by` = "You cast Essence Scatter / You lost 3 life (now 14) / Opponent drew a
card". Cost: the fallback played the line the model had explicitly rejected, in the game the seat
lost 0-20. Wave 65 had zero heuristic fallbacks; this seam is new with H2a. The cheap fix is to
accept a short same-line prefix before the label (`^\s*[A-Za-z ]{0,20}:?\s*(CHOICE|ATTACK|BLOCKS|PUT):`)
rather than to re-word the protocol - the reply already contains an unambiguous, correctly-formed
answer.

**HIGH-2 - no X row prices the draw against library size, and the seat took the maximum X in 14 of
14 X windows.** Census over the six seat files: every `X = N` answer was the largest offered
(13 of 14 were literally option 1; 125v123 seq 132 took option 2). The X menu's rows carry
`{X pricing: X=21 - you gain 21 life and draw 21 cards}` and `{leaves K of your 24 floating mana
unspent}` - mana, life and cards, but never "of your L library cards, L-N left". "Your library: N
cards" is printed, but far up in the board block and not on the row that spends it. Two games were
lost this way from won boards: 125v126 seq 273 (library 22, X=21, lost at 60-5) and 125v123 seq 132
(library 14, X=13, lost at 44-3); the game the seat WON is the one where max-X happened to be 3/5/8.
Repro line, `...deck125-0x5583a84a5320-vs-...deck126` seq 273:
`1. X = 21 {X pricing: X=21 - you gain 21 life and draw 21 cards} {leaves 3 of your 24 floating mana unspent}`
with `Your library: 22 cards` in the board block. Ask: put the library remainder on every X row that
draws (`{draws 21 of your 22 library cards - 1 left}`), and flag a row that would leave fewer cards
than the seat's per-turn draw count. The deck guide has carried this rule in emphatic prose for at
least two corpora (it already cites a prior-corpus 125v126 seq 528 loss of the same shape) and the
model still takes option 1 - prose has lost this argument; the fact has to ride the option.

**HIGH-3 - the X-menu answer is made with no deliberation because the cast window consumed it.**
seq 272 (cast row) = 47,976 ms and 2,700 characters of correct analysis including the library
count; seq 273 (X announcement, immediately after, same board) = 3,335 ms and one line. The engine
splits one decision across two windows and the second one is where the irreversible number is
chosen. Either fold the X choice into the cast row (the cast row already computes "best X for this
cast"), or carry the cast window's own reasoning constraint onto the X menu.

**MED-1 - the cast row's "max affordable X" disagreed with the X menu by one, once.**
`...vs-...deck126` seq 272 cast row: `{X pricing: max affordable X=20 (23 mana total)}` and
`[<- best X for this cast: X=20 - largest affordable X ...]`, while the same prompt's board block
reads `Mana available: 24 total` and the X menu one window later (seq 273) offers X = 21 with
`24 floating mana`. 16 of the 17 cast-row/X-menu pairs on this seat agree exactly; this is the
singleton. I did not identify the code path. It matters because the model did its library
arithmetic against the cast row's 20 ("drawing 20 leaves me with 2") and was then handed 21.

**MED-2 - a PLAN clause that contradicts a printed prompt paragraph is re-served verbatim and acted
on.** 125v126 seq 279's PLAN carries "Do not activate Elixir while Sanguine Bond is on the
battlefield (it closes the loop with Exquisite Blood in graveyard)" while the same prompt's
converter paragraph says the opposite ("a converter of THEIRS turns every life THEY gain") and the
`LOOP HALF PENDING` line says the pair has NOT closed (Exquisite Blood is in their graveyard). Lane
AR already has `planLibraryOnlyNote` and `plan_contradicts_noop_row`; the sibling check
(`plan_contradicts_converter_paragraph`, or more generally: a carried plan naming a permanent whose
printed paragraph asserts the opposite) would have caught this one. The seat had already stated the
correct rule at seq 272 and lost it into the plan.

**MED-3 - the discard seam is where the "no coded line at all" refusals cluster.** 5 of this seat's
15 label-free refusals are discard windows (125v123 seq 62, 66, 74; 125v126 seq 194, 258): the
model enumerates its picks in prose ("So: 1, 2, 3, 4, 11, 8", "Discard #8 and #3") and never emits
`PUT:`. The seat's discard load is extreme - 22 discard windows in 125v126, one of them (seq 276)
asking for 18 discards out of a 25-card hand after the X=21 Revelation. Baka then chose, and its
picks differed materially from the model's stated intent every time (seq 66: model said Lightmine
Field + Supreme Verdict, Baka discarded Fall of the Gavel twice). `DEFAULT ANSWER: discard the spare
land` (H5) rendered 13 times on this seat but only covers the spare-land case; a default that
answers a large forced discard by the printed disposability order would remove most of this class.

**LOW-1 - latency.** Seat max 461 s (125v146 seq 6, the refused Silencer window), 387 s
(125v162 seq 18, which also overran past PLAN by 1,238 trimmed bytes). p90 47.9 s. The two games
with the worst p90 (189 s and 139 s) are two of the three fast losses. The reasoning-first order is
producing long replies at windows whose answer is a hold row.

**Not checked**: I did not read the opponent seats except for the 125v126 stderr close-out; I did
not adjudicate any lane prediction (engine seat's job); I did not verify the parser code itself -
HIGH-1's mechanism is inferred from the reply shapes plus the protocol text, not from reading
`AIPlayerGPT.cpp`; no `repeat_past_stop`, `reveal_stall_forced` or `wall_miss` records appear on
this seat, and `DRAW FORECAST` rendered only in 125v162 (22 windows), so I have nothing to say
about AQ H1.

## 3. Guide verdict: EDIT (one change)

The guide was followed almost everywhere it speaks: 100% of land drops taken, Staff-of-Nin-as-win-
condition plan written and executed in every game, Elixir cast on sight, no Emrakul cast on a
converter board. The two losses that mattered were both the same single answer, and the guide's
library-ceiling rule already existed - but it was written as a check to apply AFTER reading the
menu, immediately followed by "YOUR X IS THE LARGEST NUMBER ON THE MENU ... option 1 whenever the
ceiling is not binding". At a 3.3-second window the second sentence is what got executed.

Full revised guide: `wave66/deck125/strategy.txt` (71,199 bytes; live file 70,901; pool band top is
deck130's 71,227). One edit:

BEFORE (deck125_strategy.txt lines 502-508):
```
announce THAT number and nothing larger. At 125v126 seq 528 you were at 70 life, ahead 70 to 23,
with "Your library: 12 cards" printed above the menu, and you answered option 1 for X = 20. You
gained 20 life to 90 and lost the game on the draw. That is the only game this deck has ever lost
from ahead, and no life total protects you from it.
WITH THAT CEILING SATISFIED, YOUR X IS THE LARGEST NUMBER ON THE MENU. Answer the largest option
the ceiling allows - option 1 whenever the ceiling is not binding - with no other arithmetic in
front of it.
```
AFTER:
```
announce THAT number and nothing larger.
WORK THE CEILING BEFORE YOU READ OPTION 1, NOT AFTER. No X row names your library, so the number
exists only if you write it: library, minus 2, minus one per Staff. Option 1 is your answer ONLY
when option 1 is at or under it.
WHAT THIS COST (newest only). Both losses from a won board were this one answer. 125v126 seq 273:
60 life to their 6, "Your library: 22 cards", one Staff - ceiling 19 - you answered option 1 for
X = 21, drew to 1 card and lost on the second upkeep draw at 60 to 5. 125v123 seq 132: library 14,
two Staffs - ceiling 10 - you answered X = 13 and lost at 44 to 3. In the game you WON the mana was
small and option 1 was X = 3, 5, 8 - under the ceiling every time. Being ahead makes this loss
more likely, not less.
WITH THAT CEILING SATISFIED, YOUR X IS THE LARGEST NUMBER THE CEILING ALLOWS, with no other
arithmetic in front of it.
```
Paid for by 125v126 seq 273 and 125v123 seq 132. It replaces a stale prior-corpus citation with the
two current ones, removes the standing "option 1 whenever" nudge, and orders the ceiling ahead of
the menu read. A trailing sentence in the following paragraph ("A smaller X saves nothing ... life
is never discarded") was compressed to keep the file inside the pool band; its content (do not shave
X below the ceiling for no reason) is preserved.

Everything else is KEPT unchanged, including the converter-runs-one-way teach that seq 279 violated:
the guide states it correctly and the model restated it correctly at seq 272 before losing it into a
carried plan. That is MED-2's job, not more guide prose.

No general-guide or skill proposals: HIGH-1, HIGH-2 and HIGH-3 are engine/render items, and the one
guide-shaped lesson (put the ceiling before the menu read) is deck-specific.
