# deck125 (REVELATION FRACTURE, UW draw-go) — wave-68 review

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/`, six seat files
`*deck125-*-vs-*`. Record 3/6. Counts below are read from the rendered `prompt`; a window is one
record, and the distinct (game, turn) count is given wherever same-turn repeats inflate it.
Guide read: the rewritten live `bin/Res/ai/baka/deck125_strategy.txt` (19,978 B) plus
`wave68/rewrite/deck125/changes.md`.

## 1. Game by game, and the decisions that decided them

| opp | result | end | turn | the decision |
|---|---|---|---|---|
| 146 | WIN 80/-1 | 74 | Emrakul cast seq 115, again seq 143; Staff clock all game |
| 126 | **LOSS 65/6** | 41 | decked out at 65 life — Elixir declined seq 60/65/71, X=7 seq 110 |
| 152 | **LOSS 0/12** | 31 | seq 68: Cancel taken over Fall of the Gavel at 8 life |
| 123 | WIN 89/-10 | 67 | Emrakul seq 97/135, Elixir shuffle turn 67 |
| 130 | WIN 60/-10 | 48 | Emrakul seq 85 turn 47 |
| 162 | **LOSS 0/14** | 29 | Revelations under Underworld Dreams, seq 31 / 50 / 62 |

### The four lopsided finishes

**126, 65 life to 6 and LOST (deck-out).** This is the whole finding of the wave for this deck. From
turn 31 the seat held two Staffs, was at 44 life, the opponent at 12, and the library read 13. A
`Life with Elixir of Immortality` row was on the menu, with 12 untapped mana and a 25-card graveyard
(seq 60). It answered `CHOICE: 0 (pass)`, and again at seq 65 (library 12) and seq 71 (library 11).
Across that game an Elixir row was on the menu in 12 windows (4 distinct turns) where the guide's
own condition — life <= 15 **or** library <= 16 — was satisfied; it was taken in 1. Two more
Revelations followed with the opponent already at 8 and two Staffs out (seq 106 X=8, seq 109 X=7),
which the guide forbids outright ("Once their printed life is 10 or less and you control a Staff,
stop drawing"). At seq 110 the X menu's printed ceiling read *X=7 is the largest value on this menu
the library pays for (8 cards, holding 1 back for your next draw step)*; the seat obeyed it exactly
and announced X=7. The ceiling holds back ONE draw. Two Staffs draw two more at each upkeep — the
second Staff's trigger was still pending in that very window — so library 8 - 7 = 1, the pending
Staff trigger took it to 0 (seq 114), and the turn-41 draw step ended the game. The pilot's own
words at seq 110: *"Cast Sphinx's Revelation for X=7 to survive the library limit"*.

**130, 60/-10 (turn 48)** and **123, 89/-10 (turn 67)** and **146, -1/80 (turn 74)**: all three are
the intended kill — Staff pings grinding the opponent under 10, then an Emrakul row taken the first
window it printed (seq 85 / 97 / 115) and swung. No misplay decided any of them; they are long
because the Staff clock is 1-2 damage a turn. Emrakul: 5 rows offered, 5 taken, 100%.

## 2. Engine / interface / card items

**HIGH — the X-menu library ceiling under-counts owed draws when Staffs are out.**
`126` seq 110, the `Announce the value of X` ask: *"1 draw is already owed that you cannot decline
(your next draw step, which you cannot decline). X=7 is the largest value on this menu the library
pays for."* At that moment the seat controlled two Staff of Nin (both on its own battlefield line)
and the second Staff's upkeep draw trigger had not yet resolved. The true owed count was 2 (pending
trigger + draw step), and next upkeep owes 3. Obeying the printed ceiling lost the game at 65 life.
Repro: that record; the row for option 6 reads *"draws 7 of your 8 library cards - 1 left; 1 draw is
already owed that you cannot decline, and the 1 left still pays them"*. Fix: count unresolved
draw triggers on the stack plus one per `@each my upkeep:draw` permanent the player controls.
This is the only place in the corpus the ceiling sentence printed (2 records), so it is untested
elsewhere — the defect is structural, not rare.

**HIGH — the `Life with Elixir of Immortality` row prints no `{right now:}` verdict.**
`126` seq 60 and seq 111 (and 412 renders of that row corpus-wide). Every neighbouring row is
priced (*"takes 1 damage - they would be at 7"*), while the row that averts a deck-out prints only
its card text. At seq 111, with `Your library: 1 cards` and a 20-card graveyard, the menu gave the
pilot no reason to prefer option 9 over a ping. A `{right now: shuffles your 20-card graveyard into
your library - library becomes 21}` clause would make this decision readable. This is the
render half of the guide's biggest miss.

**HIGH — the no-op reversal predicate does not fire on a sweeper row that prints its own zero.**
The engine already re-asks this shape: `130` seq 20, `plan_contradicts_noop_row_reask`, on a
`Cast Supreme Verdict {right now: destroys 0 of their creatures}` take — and the re-ask worked (seq
21 declined). It fires only off the PLAN text. Corpus-wide the seat took a K=0 sweeper row 14 times
over 12 distinct (game, turn) out of 69 offer-windows: `146` seq 11, 20; `152` seq 39; `123` seq
10, 40, 41, 66, 104; `130` seq 28, 33, 56, 58; `162` seq 8, 61. Extending the existing predicate to
a taken row whose own `{right now: destroys/exiles 0 of their creatures}` reads 0 would remove all
14 without a guide byte. (Every one of them was unpunished in this corpus — see §3.)

**MED — an activation row is offered while `Mana available: 0 total`.** `126` seq 111 and 113 print
`9. Life with Elixir of Immortality [cost: {2}, Tap]` under *"Mana available: 0 total (no untapped
sources ...)"*. The protocol tells the pilot a printed row is payable now; here the two surfaces
disagree, and at library 1 that row was the game. Either suppress the row or annotate what pays it.

**MED — reply degeneration survives the cap.** `123` seq 64, `reply_truncated_reask`: the reply is
768 bytes of repeated CJK tokens and digits. `146` seq 133, `unparsed_reply` on a cleanup discard
where the model wrote prose and never emitted `PUT:`; the engine chose 6 cards for it including
Emrakul and three Fall of the Gavel. `126` seq 104, `distinct_index_reask` on `PUT: 3, 3`.

**LOW — card fact.** Path to Exile's primitive (`mtg.txt:84674`) gives the *target's controller* the
basic land; the live guide called it "its land-fetch cost ... so always pay it", which reads as a
cost the pilot pays. Corrected in the revision. All other card claims re-checked against the
primitives this wave and correct: Staff of Nin `{6}` / upkeep draw / `{T}` 1 damage; Elixir `{1}`,
`{2},{T}` gain 5 + shuffle; Revelation `{X}{W}{U}{U}`; Supreme Verdict `{1}{W}{W}{U}` nofizzle;
Final Judgment `{4}{W}{W}` exile; Lightmine Field `{2}{W}{W}`; Fall of the Gavel `{3}{W}{U}` counter
+ 5 life; Emrakul `{15}`, extra turn on cast, annihilator 6.

**Not checked**: the opponents' own guides and their misplays except where they set the board this
seat answered; blocker/attacker seams (this deck has 1 attackers record per game and no blocks);
latency and prompt-byte comparisons against wave 67 (engine seat's question); anything about decks
other than 125.

## 3. Guide verdict: EDIT

The rewrite did not lose a rule this corpus needed. Of the two rules `changes.md` (b) names as cut
for the ceiling: the **Emrakul edict-refund carve-out** is UNTESTED — no window arose, no
`LIFE-TO-DAMAGE CONVERTER` line printed at any of the 5 Emrakul rows, and the branch stays
reachable; the **Revelation full-hand hold** would have changed `162` seq 50 and seq 62, but the
KEPT discard-punisher rule already governed both rows verbatim (each printed a `CLEANUP PRICE at
X=N` tail under a printed `DISCARD PUNISHERS ... Liliana's Caress` paragraph) and was ignored, so
its absence is not what cost the game. `changes.md` (b) is confirmed. The compression itself is
clean: no rule the corpus exercised is missing.

What IS wrong is ordering: three rules the guide already contains sit below louder imperatives and
lost every contest with them. Each edit below is paid by a decision that lost or nearly lost a game.

**Edit 1 — the Elixir check moves inside the ping imperative and into the plan line.**
Before: the Staff section said *"Take 'Deal 1 Damage ...' in EVERY window offered, upkeep included,
with EVERY Staff"*, with the Elixir override as a following bullet; the Elixir section repeated it
("ahead of a Staff ping").
After: `READ "Your library: N cards" BEFORE EVERY PING. At 16 or less, with a "Life with Elixir of
Immortality" row anywhere on the same menu, TAKE THAT ROW and ping in the next window ...` and the
following bullet deleted; the dictated plan sentence gains `take an Elixir row before any ping while
my library reads 16 or less`.
Paid by: `126` seq 60, 65, 71 (each a due window declined, ending in the seq-114 deck-out);
`123` seq 130-144 (9 due windows, 1 take). Corpus: 30 due windows over 13 distinct (game, turn),
9 takes. The ping imperative ran 85/85 — the strongest-obeyed line in the guide — which is exactly
why the exception had to move inside its sentence rather than be restated a fourth time.
Prediction: in the next corpus, of Elixir-row windows where `Your library:` reads <= 16 and a ping
row is also on the menu, takes of the Elixir row reach >= 8 of 10; a hit is a break only if the
Elixir row and a ping row were on the SAME menu. Removal condition (belief-correcting): both the
Elixir row rendering a shuffle/library verdict AND zero breaks across two corpora with the shape.

**Edit 2 — the Revelation section opens with its three stops, ahead of the floors.**
Before: the section opened with the low-life floor; the "their life <= 10 with a Staff, stop
drawing" rule was the last bullet of a list, and the draw/discard-punisher stops lived in a
different section.
After: `THREE PRINTED STOPS COME FIRST` — (1) a `DRAW PUNISHERS on the battlefield: theirs`
paragraph, (2) a `{... CLEANUP PRICE at X=N ...}` tail, (3) their printed life <= 10 with a Staff on
your battlefield line — with the low-life floor stated as lifting (2) and (3) but never (1), which
matches the floor's own two printed strings. The two punisher bullets became pointers, so no rule is
in two places.
Paid by: stop (3) — `126` seq 106 (opp 8, two Staffs, X=8) and seq 109 (opp 8, X=7), the two casts
that emptied the library. Stops (1)/(2) — `162` seq 12, 25, 31, 50, 62: five takes of a Revelation
row across 33 windows (5 of 13 distinct turns) under a printed Underworld Dreams paragraph, three of
them also carrying a `CLEANUP PRICE` tail; seq 62's draws pushed the hand to 11 and the turn-27
cleanup billed 8 life to Liliana's Caress (11 -> 3), from which the seat died at turn 29.
Prediction: Revelation takes in windows printing any of the three literals fall to <= 1 in 20;
a hit is a break only if the low-life floor's two strings are both false. Removal condition: none —
this is a decision rule, not a fact the render can supply.

**Edit 3 — the X ceiling is read minus one per Staff.**
Before: *"THAT K IS YOUR CEILING ... TAKE IT FROM THAT SENTENCE, NOT FROM ARITHMETIC OF YOUR OWN,
since it already subtracts the draws you owe"*.
After: *"THAT K HOLDS BACK ONE DRAW ONLY - your draw step, as the sentence says. EACH Staff of Nin on
your battlefield line draws you one more card at your next upkeep, and that K does not count them,
so announce K MINUS ONE PER STAFF, never larger; at 0 or less take the 'Decline - do not cast this
after all' row."* This is a route-around off two true printed surfaces (the ceiling sentence's own
statement of what it holds back, and the battlefield Staff count); it teaches no doubt of a printed
number — the number is read for what it says it is. The old fallback formula is folded in.
Paid by: `126` seq 110 (K=7, two Staffs, announced 7, library 1, dead at seq 114; K minus one per
Staff = 5, which survives).
Prediction: no `126`-shaped deck-out recurs while the render is unfixed. Removal condition
(information-supplying): one corpus in which the ceiling sentence counts pending triggers and
per-upkeep draws — delete the Staff subtraction that wave and restore "take K from that sentence".

**Edit 4 — which counterspell, at low life.**
Added: *"WHICH ONE, when two or more rows can counter the same spell: the CHEAPEST - one exception,
and it outranks the price. Your printed life is 12 or less: take Fall of the Gavel."*
Paid by: `152` seq 68. At 8 life, with `INCOMING THIS COMBAT: not declared yet - 1 of their
creatures can attack, for up to 7 - you would be at 1` printed and both a Fall of the Gavel row and
a Cancel row able to counter the same Luminarch Aspirant, the seat took Cancel. The Wolf attacked
that turn at 8/8 (Ranger Class pumped it) for exactly lethal. Gavel's 5 life makes it 13 and
survives. The guide had no rule choosing between counter rows at all.
Prediction: at printed life <= 12 with two or more counter rows on one menu, the Gavel row is taken
>= 3 of 4 times. Removal condition: none.

**Trims that paid for the above** (guide was at 19,978 of 20,000, so every addition had to buy its
bytes): the Ob Nixilis ping situation (UNTESTED — the only `DRAW PUNISHERS` name in the corpus was
an enchantment, never a ping target, so the anecdote goes and the rule stays); the Lightmine
situation (its rule held, 12 of 14 N=0 windows declined); the K=0 sweeper situation (its rule is
being enforced by the engine item above, not by a fourth restatement); the two punisher bullets
folded to pointers at the Revelation stops; and prose compression in the deck paragraph, the
tap-out gate, the counterspell wall/power-0-1 pair, the sweeper header, Path branch 4, Lightmine,
the mulligan branch list, the hold paragraph, the cleanup tag paragraph, and the Emrakul converter
note. No rule was deleted.

**Rules deliberately NOT edited.**
*K = 0 sweepers*: 14 takes over 12 distinct (game, turn) of 69 offer-windows, and the loudest
absolute in the guide, already carrying its full exclusion list. Not one of the 14 is traceable to
a loss — three of the games were won lopsidedly and in the `162` game the opponent never had a
creature on the battlefield after turn 27. Violated-unpunished is not a reason to loosen it and a
fifth paraphrase is not a reason to spend bytes: the engine predicate above is the fix.
*"Play no land right now" is never your answer*: 25 takes over 13 distinct (game, turn), every one
at 10 or more lands already in play, cost zero. Same verdict.
*Lightmine Field at N=0*: cast twice at `{right now: they control 0 creatures able to attack}`
(`146` seq 25, `123` seq 49), both games won, the permanent later relevant in both. Reported, not
edited.

**Verification.** `stat -c %s wave68/deck125/strategy.txt` = **19987** (ceiling 20000).
`/usr/bin/grep -ciE 'corpus|wave[- ]?[0-9]|\bseq\b|previous game|last wave|this wave|census|paid
(for )?by|[0-9]{3}v[0-9]{3}|you (declined|took|cast|passed) [0-9]'` = **0**.
Frequency-adjective grep = **0** (the live guide had one, "a Revelation row rarely prints the
clause"; verified against the corpus that such a row prints `{no {leaves ...} count on this row}`
every time, and the sentence now states that). Literal audit: every string the revision quotes
renders in this seat's prompts — `Life with Elixir of Immortality` 412, `Your library:` 622,
`DRAW PUNISHERS on the battlefield: theirs` 61, `CLEANUP PRICE at X=` 25, `INCOMING THIS COMBAT` 14,
`Decline - do not cast this after all` 28, `is the largest value on this menu the library pays for`
2, `right now: they control` 28.

## 4. Proposals

None. No general-guide change and no skill change is supported by this seat's evidence; the skill's
existing amendments (A6 imperative-outvotes-stop, A26 exception-above-imperative, A287 route-around,
A319 supply-your-own-arithmetic-against-a-marker) already name every move made above.
