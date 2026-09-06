# Deck 125 (Revelation Fracture, UW draw-go control) — wave 67 seat review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-102638/`. Six seat files `*deck125-*-vs-*`;
540 records, 522 model calls, median latency 12.1 s, p90 37.0 s, max 253 s (the corpus MAX of
1233 s is not on this seat). `post_answer_overrun` 351/522 (67%). **`unparsed_reply` 0** (27 last
wave); one fallback total (`index_name_conflict`, 125v162 seq 90, resolved correctly by name).
Record **4/6**, up from 1/6 on the wave-66 guide edit + the AW library render.

## 1. Game by game, with the deciding decisions

**125v130 — WON 23/0, T47.** Guide plan executed: land every turn, two Staffs, ping every upkeep.
The new cast-row `LIBRARY CEILING` clause DECLINED the Revelation twice (seq 84 at library 7, seq 85
at 6) — behaviour this seat had never shown. Then **seq 89/90 broke it**: library 4, the ANNOUNCE_X
header read `3 draws are already owed that you cannot decline ... X=1 is the largest value on this
menu the library pays for`, and the seat answered option 6, `X = 4`, emptying the library with three
undeclinable draws owed. It was dead on the T45 upkeep and survived only because the cleanup discard
at seq 92 included Emrakul, whose graveyard-shuffle refilled the library (0 -> 37 at seq 93). Won on
Staff pings four turns later. See HIGH-1: option 6's row clause reads a benign `0 left`.

**125v126 — WON 28/0, T31.** Clean. One deviation: seq 20/21 Pathed a **Perimeter Captain (0/4
defender)** with two more defenders as the only other candidates — the guide forbids this at any
life total. Free-rolled (the seat won).

**125v123 — WON 36/0, T46.** 40 turns of holding priority, Staff cast T43, X=6 at library 27 (under
every ceiling), Staff finished it. Nothing to fault.

**125v146 — WON 51/0, T46.** The corpus's `146v125 0/51` lopsided finish. Not lopsided play: a
mulligan to 6, two Elixirs recycled four times (seq 23/26/69/71), two Staffs online T24, three
sweepers taken at 4, 2 and 1 creatures. The 51 life is Elixir + two Revelations at ceiling-safe
X. Model text-book on the guide.

**125v162 — LOST 10/19, T23.** Decided at **seq 68**. Board: their Ob Nixilis (draw punisher),
Forced Fruition, 3 Howling Mines, Dictate of Kruphix; seat at 20 life, them at 19, library 26.
The seat cast **Path to Exile at a Shield Sphere (0/6 defender)** off a row printing
`[DRAW PRICE: casting this draws YOU 7 cards (their Forced Fruition), and their Ob Nixilis ...
deal you 7 - you would be at 13]`, with `4. Cast nothing right now` on the same menu. Life went
20 -> 13 -> 8 over the next ten windows. THREE separate guide rules forbade it (Path never at a
[defender]; Forced Fruition "cast nothing on your turn"; and "counter on theirs only off a
[DRAW PRICE ... at K] row reading K of 15 or more" — K was 13). Every surface was correct and
present. Pure STRATEGY. Credit where due: at seq 91, library 8 with 6 draws owed, the seat answered
**X = 2** — exactly the header's ceiling and BELOW the max — while the guide's own subtraction
(library - 2 - one per Staff = 5) would have decked it. The printed ceiling beat the guide's.

**125v152 — LOST -1/20, T21.** Decided at **seq 20** (T16, life 12 to their 20). The seat cast its
**LAST Supreme Verdict off a row reading `{right now: destroys 0 of their creatures (0 without a
restriction against attacking), 0 of yours}`** with `4. Cast nothing right now` available — and the
PLAN line in that same reply says "Use Supreme Verdict only when opponent has 2+ creatures able to
attack." Their board was literally empty (0 creatures, 0 cards in hand). Sigarda (grown to 6/6 by
two Ranger Classes) and two Brutal Cathars landed T17/T19 into no answer; life 12 -> 5 -> -1. The
guide's hardest line ("IF THAT SECOND NUMBER IS 0 ... NOT CASTABLE PLAYS THIS TURN ... the most
reliable line in the guide") broke for the first time in three corpora. Census: 5 sweeper takes
across 6 games, ~45 dead rows declined, this one taken.

## 2. Engine / interface / card items

**HIGH-1 — the ANNOUNCE_X ROW clause and the ANNOUNCE_X ASK header state different ceilings, and
the row is the one being answered.** Lane AW says the reserve, the row and the ask are "built from
the SAME reserve, so they cannot name different values". They are not. `; M more pending` renders
**0 times in the entire wave-67 corpus** (`/usr/bin/grep -o "more pending" *.jsonl | wc -l` = 0),
while the ask header names an owed count in 2 windows on this seat. The row clause counts the
library alone.
Repro: `1788708411-ai_baka_deck125-0x55912b4ba230-vs-ai_baka_deck130.jsonl` seq 90.
Header: `YOUR LIBRARY IS THE SECOND CEILING on this menu: it holds 4 cards ... 3 draws are already
owed that you cannot decline (your next draw step ... plus Staff of Nin's upkeep draw, plus Staff of
Nin's upkeep draw) ... X=1 is the largest value on this menu the library pays for.`
Option 1: `{library: this draws 9 of your 4 library cards - 0 left, which is 5 MORE than the library
holds: you would be asked to draw from an EMPTY library and LOSE the game as that draw is attempted}`
Option 6 (the one taken): `{library: this draws 4 of your 4 library cards - 0 left}` — **no warning,
no pending term**, though it is exactly as lethal as option 1. The loud clause fires on
`N > library` only; it must fire on `library - N < reserve`. The seat took the benign-looking row
and was saved by an Emrakul shuffle. Fix: give `xLibraryRowClause` the same `stackPendingDrawsFor +
drawStepExtrasScan` reserve the ask note already uses, so a row at or under the reserve reads
`- 0 left, and 3 draws you cannot decline are already owed: this loses the game`.

**HIGH-2 — the ANNOUNCE_X menu is the one menu where the row NAME and the row INDEX are both bare
integers, so a name/index conflict is structural.** `...deck125-...-vs-ai_baka_deck162.jsonl` seq 90:
reply `CHOICE: 2 (X = 2)` — index 2 is `X = 4`, the name `X = 2` is index 4. `parse_note` =
`echo_index_conflict;name_over_index;index_name_conflict`; the engine took the name (choice 4), which
was right, but recorded a fallback and burned a re-ask (seq 91, 11.9 s) on a window the model had
already answered correctly. This is the only fallback on the seat. Cheap fix: on an ANNOUNCE_X menu,
prefer the NAME silently and record no fallback — the header already says "Reply with the OPTION
number, not the X value", so the conflict is a known, self-resolving shape here, not an ambiguity.

**MED-1 — a dead sweeper row is priced but not verdicted.** `...vs-ai_baka_deck152` seq 20 renders
`{right now: destroys 0 of their creatures (0 without a restriction against attacking), 0 of yours}`
— true, complete, and taken anyway at 58.4 s of latency and a two-line reply. Lane AW's M1 gave the
ATTACK TOTAL line a closing verdict (`That is NOT lethal`); the sweeper row has the same shape and
no verdict. `{right now: destroys 0 of their creatures ...; this spell does nothing on the current
board}` is the same one-clause change on a line whose miss cost a whole game here.

**MED-2 — the DRAW PRICE bracket does not compare itself to declining.** `...vs-ai_baka_deck162`
seq 68 row 1 says `you would be at 13`; row 4 (`Cast nothing`) prices its own cleanup at `at worst
you would be at 8`. Two projections in different units, neither naming the other, on a menu whose
right answer is decided by exactly that comparison. A `{casting this costs you 7 life more than
declining}` tail on the priced row would make it a subtraction the model cannot skip.

**LOW-1 — `post_answer_overrun` 351/522 (67%) on this seat**, against the wave-66 figure of 60%
corpus-wide. The overruns are harmless here (0 `unparsed_reply`), but every deciding misplay above
came with a SHORT reply (seq 20: 2 lines / 58 s; seq 68: 2 lines / 24 s; seq 90: 2 lines / 3.3 s
class) while the 67% overrun sits on windows whose answer was a hold row. Reasoning length is
anti-correlated with the windows that decide games.

**Not checked**: I did not read the opponent seats or any stderr; I did not adjudicate lane
predictions (engine seat's job) beyond the two AW claims my own items falsify; I did not read
`AIPlayerGPT.cpp` — HIGH-1's mechanism is inferred from the two rendered surfaces plus lane AW's
own description, not from the code; no `repeat_past_stop`, `reveal_*`, `wall_miss` or `async_drop`
record appears on this seat; I did not verify any card against Scryfall (no card behaved wrongly).

## 3. Guide verdict: EDIT (five changes, all seq-paid)

The wave-66 edit worked and is kept: the deck went 1/6 -> 4/6, **zero deck-outs** (wave 66: two),
and the max-X take rate fell from 14/14 to 7/9 — both below-max answers were at ceiling-binding
windows. What the corpus now shows is that three of the guide's standing facts have gone stale
against the wave-67 render, and one of them is actively worse than the printed line.

Full revised guide: `wave67/deck125/strategy.txt` — **71,499 bytes** (`stat -c %s`), inside the
41,000-71,500 band; live file 71,199. Five edits; byte cost paid for by compressing superseded
prior-corpus anecdotes (no rule changed by a compression).

**E1 (X section) — the ceiling now comes from the ask, not from arithmetic.**
BEFORE: `READ ONE LINE BEFORE YOU ANSWER: "Your library: N cards" ... Subtract 2, and one more for
EACH Staff of Nin ... THAT NUMBER IS YOUR CEILING` / `No X row names your library, so the number
exists only if you write it`.
AFTER: `THE ASK NOW PRINTS YOUR CEILING - READ IT BEFORE YOU READ OPTION 1 ... "X=K is the largest
value on this menu the library pays for." K IS YOUR CEILING ... TAKE K FROM THAT SENTENCE, NOT FROM
ARITHMETIC OF YOUR OWN - it already subtracts the draws you owe ... Fall back to that subtraction
ONLY when no such sentence is printed. EACH ROW ALSO PRINTS "{library: ... - L-N left}". That clause
counts the library ALONE ... so a row reading "0 left" can be the row that kills you.`
Paid by **125v162 seq 91** (ask ceiling 2, guide's subtraction 5; the ask was right and 5 decks the
seat) and **125v130 seq 90** (the row said `0 left`, the ask said X=1, the seat answered the row).
"No X row names your library" is now false and told the model to ignore the better number.

**E2 (X section, WHAT THIS COST) — newest citations only.** The two wave-66 losses were replaced by
**125v130 seq 90** and **125v162 seq 91**; the block declares itself "newest only" and both prior
citations describe behaviour this corpus did not repeat.

**E3 (SWEEPERS) — the "most reliable line" claim is now false and is replaced by its cost.**
BEFORE: `Every dead row this corpus was declined, as in the two corpora before it: this is the most
reliable line in the guide.`
AFTER: `You broke it once and it cost you a game: 125v152 seq 20, at 12 life to their 20 with their
board empty, you cast your LAST Supreme Verdict off a row reading "destroys 0 of their creatures" -
and the PLAN in that same reply said "only when opponent has 2+ creatures". Sigarda and two Brutal
Cathars landed into no answer over the next two turns and you died at -1.`
Also added to the enumerated outranked reasons: **`being BEHIND`** and **`the last card in your
hand`** — the two facts true at seq 20 that the old list did not name. Paid by 125v152 seq 20.

**E4 (PATH TO EXILE) — decide the defender at the CAST row, not the target ask.**
Added after `A creature tagged [defender] still never gets the Path`: `DECIDE THAT AT THE CAST ROW,
NOT AT THE TARGET ASK. The cast row prints "{removes: A, B, C}" ... If EVERY name there is
[defender] or printed power 0 or 1, answer "Cast nothing right now" at THAT window and never open
the target ask.` Paid by **125v162 seq 68** and **125v126 seq 20** — both violations were committed
at the CAST window, which the section's rule (written about "candidates") did not reach.

**E5 (FORCED FRUITION) — a stale fact.** `it has not appeared in THREE corpora` -> `it landed this
corpus and you lost that game`. Paid by 125v162 seq 68.

Kept unchanged: #0 land drops (100% taken), Staff-of-Nin-as-win-condition (executed in all four
wins), the Elixir library-16 rule (the one binding window, 125v162 seq 88, was taken correctly),
the counter tier list, Lightmine Field, and cleanup discard.

## 4. Proposals
None. HIGH-1, HIGH-2, MED-1 and MED-2 are engine/render items, and the guide lessons above are all
deck-specific (this is the only deck with a library that runs out and the only one holding sweepers
against an empty board this often).
