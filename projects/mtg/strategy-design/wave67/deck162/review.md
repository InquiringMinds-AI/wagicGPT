# Wave 67 - deck 162 (Draw and Die, UB forced-draw punish) - per-deck review

Corpus: /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-102638/
Seat files: `1788708401-...deck162-...-vs-...deck123`, `...403-...vs-deck146`, `...407-...vs-deck126`,
`...413-...vs-deck152`, `...421-...vs-deck130`, `...425-...vs-deck125` (all under the corpus dir).
Record 4/6 (W vs 123, 126, 130, 125; L vs 146, 152). 144 decisions, 0 fallbacks, 0 `unparsed_reply`,
0 `engine_reveal_floor_picks`, 0 `wall_miss_unrecorded`, `post_answer_overrun` on 93/144 (65%).

## 1. Game by game

**W vs 123 (turn 12, 20 to 0).** Clean curve: land every turn (seq 3,5,6,8,10,14), Underworld Dreams
seq 7 (T5), UWD #2 seq 9 (T7), Ob Nixilis seq 11 (T9), then engines. At seq 15 (T11, K=3 printed on
the row) it took Master of the Feast over Forced Fruition and left 3 sources - correct: engines are
free once converters are out. Rule 1 and Rule 0 both fired every window.

**L vs 146 (turn 15, -9 to 19).** Deciding stretch is seq 15 / 19 / 20 (turns 12-15). Both punishers
were answered by removal (Fate Unraveler exiled by Vanishing Verse T9; Shield Sphere exiled by Kaya
T11), leaving a hand of Dictate of Kruphix + Forced Fruition + a land and six then seven untapped
sources. The K-of-0 brake ("draw converters in your hand: 0" -> the brake stands, every time,
however much mana is idle) forbade the only card on the menu, and the release's condition (ii)
(their M > your N and life <= 15) forbade it a second way; there was no wall row on either menu.
The seat answered "Hold priority" at seq 15, 19 and 20 and wrote the SAME plan all three times -
"cast Dictate of Kruphix to draw into a punisher" - while CRACK-BACK went from "you would be at 5 or
lower" to "you would be at -3 ... that would KILL you". Three turns, seven mana, nothing cast.
GUIDE item, not perception: the brake was read correctly and it was the wrong instruction for a
board with no punisher left and no wall on offer. (Removal-driven variance is real here; the
actionable half is the three idle turns, not the two exiles.)

**W vs 126 (turn 16, 19 to 0).** Punisher-first held throughout: seq 7 declined a SECOND Howling Mine
at K of 0 with five converters in hand and 2 mana ("I must not cast more engines ... as K=0"), then
Underworld Dreams seq 10, Fate Unraveler seq 12, Ob Nixilis seq 15. Rule 2's brake paid here.
Contains the corpus's one deadline overrun (see HIGH-1).

**L vs 152 (turn 15, -31 to 26).** Matchup, not guide. Play was guide-compliant: seq 10/11 declined a
second Howling Mine and a second Caress at K of 0 (converters in hand: 0), seq 16 cracked a Clue to
dig, seq 18 took Underworld Dreams over Master of the Feast at 4 mana (Rule 1). Turn 13 the opponent
went from 6 power to a 20-to-2 swing in one combat (Sigarda pumped to 7/7 by two Luminarch Aspirants,
two Intrepid Adversaries with lifelink) while the seat had zero creatures - Fog Bank was exiled by
Brutal Cathar. At seq 20 (life 2) it took Fate Unraveler over Master of the Feast and then cast
Master in the same window (seq 21) - both wave-66 edits firing, too late to matter. INCOMING read
"37 unblocked damage ... no block saves you"; nothing on the block menu was a misplay.

**W vs 130 (turn 20, 6 to 0).** The brake was the whole game: seq 8 (T5) and seq 16 (T11) both
declined Master of the Feast at K of 0 with converters-in-hand 1 then 2, on menus whose only other
row was "Cast nothing right now". Underworld Dreams landed seq 20 (T13), Master seq 22 as a body at
6 life, Ob Nixilis seq 25, UWD #2 + Mine #2 seq 29/30. Opponent 20 -> 0 in four draw steps.

**W vs 125 (turn 23, opponent DECKED at 10 life).** The long game and the guide's best showing:
Underworld Dreams countered twice and Fate Unraveler countered once (T6, T10, T12) and the seat kept
re-deploying (seq 16, 21, 22 Fate Unraveler; 25, 28 Ob Nixilis; 29, 30 Mine #2/#3; 35 Forced
Fruition; 36 Dictate). The win came from the opponent's library, not their life total: deck125's own
seat at turn 23 was told "YOUR LIBRARY IS THE SECOND CEILING ... it holds 8 cards ... 6 draws are
already owed that you cannot decline ... X=2 is the largest value on this menu the library pays for"
(game-125v162-1788708422.stderr:2271) and died drawing from empty at 10 life. One flaw, seq 19 -
see HIGH-2.

## 2. Engine / interface / card items

**HIGH-1 - the 900 s wall issues an unbudgeted retry, so a decision can bill 1233 s.**
`1788708407-ai_baka_deck162-0x56442565e5c0-vs-ai_baka_deck126.jsonl` seq 13 (turn 13, blockers):
`latency_ms 1233411`, `deadline_pct 137.0`, `retry 1`, `wall_miss 1`,
`transport: curl=28,http=0,empty=1,connect_ms=20000,phase=wall`. The stderr says it plainly:
`game-126v162-1788708404.stderr:1277` "AIPlayerGPT: transport outcome curl=28 http=0 empty=1" then
:1278 "AIPlayerGPT: no reply after 900s - one retry". Lane AX measured the wall as having "zero
retry budget by arithmetic"; the code nevertheless retries AFTER the wall, and the retry's own
~333 s lands on top of the 900 s. This is the corpus MAX latency the brief asked to be explained,
and it is a wall + retry, not a slow model. Either the retry has to be inside the 900 s budget or
the deadline has to be documented as 900 s + one retry.

**MED-1 - the blockers seam re-derives trades the row already computed, and can emit a second
answer.** Same record (vs126 seq 13): `post_answer_overrun 2711`, `reply_trimmed_bytes 872`,
`coded_answers 2`, `ignored_line: "BLOCKS: B2:A1"`, `parse_note: later_answer_ignored;long_reply`.
The rendered rows already carried "B3. Fate Unraveler (3/4) - may block A1 (you kill it, your
blocker lives)" and the reply still argued Sanguine Bond / lifelink arithmetic for ~2.7 KB and then
wrote a DIFFERENT BLOCKS line. AV's first-wins kept the correct B3:A1, so nothing was lost here -
but the second coded line is the failure mode first-wins exists for, and it appeared twice in this
deck's 144 decisions (also `...vs-deck146` seq 19, `ignored_line: CHOICE: 3 (Hold priority...)`,
same choice number, harmless). Both were long replies; both were the two slowest non-wall records.

**MED-2 - the reservation-decline latch (lane AX I7b / AZ R3) was UNTESTED at this seat.**
All six gameend records read `reserve_decline_windows_skipped: 0` and
`identical_ask_answers_reserved: 0`. Exactly ONE `{reserve:}` row was rendered in the whole deck-162
corpus - `...vs-deck125` seq 19 - and it was a FIRST ask that the seat TOOK, so no decline was ever
made and the latch had no window to hold or to hold wrongly. It never held; it also never held
across a changed board. The engine seat should not read this deck's zeros as a pass.

**MED-3 - `hold_windows_skipped` 56 in the game the seat lost, 0-2 everywhere else.**
`...vs-deck146` gameend: `hold_windows_skipped: 56` off four "Hold priority" takes (seq 12, 15, 16,
19, 20), against 0 in four games and 2 in vs125. The hold row is doing what it says, but the seat
that skipped 56 windows is the seat that spent three turns with seven mana idle. Worth a cross-deck
look at whether a hold should re-open when the CRACK-BACK line crosses into lethal - the row text
explicitly says a projected LIFE TOTAL change does NOT re-open a hold, which is exactly the change
that mattered at vs146 seq 19/20.

**LOW-1 - decking the opponent is a real win route for this deck and no rendered line says so.**
vs125 was won on the opponent's empty library at 10 life. The seat's prompt prints "Opponent
library: 41 cards" and a DRAW FORECAST in life, but never the same forecast in CARDS ("at the
current forecast they draw D per turn -> empty in T turns"). Their seat gets the full ceiling text
(the AW I4 library reserve); ours gets nothing. For a deck whose engines are the mill, that is a
missing fact at decision time, and it would change engine-vs-punisher ordering in the late game.

**LOW-2 - nothing card-side.** Verified against the primitives: Fate Unraveler
(mtg.txt:39182) `auto=@drawfoeof(player):damage:1 opponent`, 3/4 Enchantment Creature, {3}{B} -
matches Oracle and matches the guide's "punisher that is also a body"; Dictate of Kruphix
(mtg.txt:29399) has BOTH `@each my draw:draw:1 controller` and `@each opponent draw:draw:1
opponent` - genuinely symmetric, which is what the new guide clause leans on.

## 3. Guide verdict: EDIT

The guide is largely working: 4/6, Rule 0 (land every turn) fired in 100% of land windows, Rule 1
put a punisher ahead of an engine at every ask where both were offered, the K-of-0 brake declined a
second engine correctly six times (vs126 seq 7; vs152 seq 10, 11; vs130 seq 8, 16; vs125 seq 11/15),
and BOTH wave-66 edits fired: Fate-Unraveler-as-a-body at `vs152` seq 20 (took Fate Unraveler {3}{b}
over Master of the Feast {1}{b}{b} at 2 life on a lethal board, "a second punisher and a blocker",
then cast Master in the same window at seq 21) and again at `vs125` seq 22 before Master at seq 23.
Two edits are paid for; nothing else is.

**Edit 1 - the {reserve:} clause was scoped to the wrong branch.**
Paid for by `...vs-deck125` seq 19 (turn 14, DRAW step, 6 sources). The row read "1. Cast Dictate of
Kruphix {1}{u}{u} {leaves 3 of your 6 untapped mana sources untapped} {reserve: this row is INSTANT
SPEED ... Taking it HERE, before your main phase, leaves 3 sources, and Ob Nixilis, the Hate-Twisted
{3}{b}{b} in your hand needs 5 - it is SORCERY SPEED, so your main phase this turn is its last
window}" and the same row printed "draw converters ... on your battlefield: 0". The seat took it and
wrote "PLAN: ... In Main Phase 1, cast Ob Nixilis, the Hate-Twisted (using 5 mana)" - the row had
already said 3 were left. Ob Nixilis was not cast that turn (seq 21 cast Fate Unraveler for 4
instead) nor the next. The live guide DOES carry the right sentence ("A PLAN THAT SAYS 'cast Dictate
now, then cast Underworld Dreams in Main Phase' IS THE ROW TELLING YOU IT IS FALSE") - but the whole
subtraction/reservation block sits INDENTED UNDER the "- K OF 1 OR MORE" bullet, and this row printed
K of 0. A model reading its own branch never reaches it.
BEFORE: the reservation appears only inside the K-of-1-or-more bullet (live guide lines 140-176).
AFTER: a new paragraph at the head of the "- K OF 0" branch, "THE {reserve:} CLAUSE OUTRANKS THIS
WHOLE BRANCH, AT K OF 0 AS WELL", keyed on the printed clause itself (self-contained, so it does not
depend on scrolling back), with the vs125 seq 19 cost. The K-of-1-or-more block is untouched.

**Edit 2 - the K-of-0 brake has no exit when the menu offers nothing else.**
Paid for by `...vs-deck146` seq 15 and seq 19 (turns 12 and 14, life 10 then 8, six then seven
untapped sources). Menus: Dictate of Kruphix + Forced Fruition + hold + decline, no wall row, no
punisher row, "draw converters in your hand: 0", CRACK-BACK "you would be at 5 or lower" then "you
would be at -3 or lower; that would KILL you". Guide-compliant answer = hold, three times; result
-9. The Dictate row itself prints the distinction the fix needs: "and so do YOU: 1 extra card per
turn (this engine is SYMMETRIC - it feeds both players)". Master of the Feast and Forced Fruition
print no such clause.
BEFORE: "'draw converters in your hand: 0' -> the brake stands, 'Cast nothing right now', every
time, however much mana is idle", with the only exits being condition (i) colour and (ii) board.
AFTER: a bounded last-ditch release added after the (ii) paragraph - cast the engine only when ALL
of: the row says SYMMETRIC, converters-in-hand is 0, the menu has NO wall row and NO punisher row at
any price, and CRACK-BACK puts you at 10 or lower. ASYMMETRIC rows never release this way.

To stay inside the pool band, two superseded corpus-census paragraphs were dropped: "THIS IS WHAT
THE THREE TAKES IN THIS CORPUS LOOKED LIKE" (its lesson is fully carried by the release plus
conditions (i)/(ii) plus the vs146 seq-11 cost that follows it) and "WHAT THIS COST, both ways"
(both halves - a second Mine at K of 0, and the first Mine being exempt - are stated as rules
immediately above it, keyed on the "[second copy:" tag).

Guide written to wave67/deck162/strategy.txt, 71,490 bytes (band 41,000-71,500, verified with
`stat -c %s`). No general-proposals.md or skill-proposals.md: nothing here generalises past this
deck's own K arithmetic.

## What I did NOT check
Only the six deck-162 seat files were read end to end; opponent seats were consulted only for the
vs125 deck-out (their stderr) and the vs152 combat log as rendered into my seat's prompt. I did not
adjudicate any lane prediction outside the ones my seat could see (AX I7b, AV first-wins, the wave
deadline question), did not census the corpus-wide counters, and verified primitives for only two
cards (Fate Unraveler, Dictate of Kruphix). I did not build, run wagic, or touch bin/Res or src/.
