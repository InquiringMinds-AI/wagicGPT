# deck125 (Revelation Fracture - UW draw-go control) - wave-62 seat review

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-133058/` (binary 12be0b79c) for five
games; the 152v125 pairing is read from the RERUN
`/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-161801/` (binary 9230a94c3), the hung game's
partial translogs used only as evidence of the hang. Seat files are the `...deck125-<ptr>-vs-...` JSONL.
Record 2-4. No fallbacks, no `answer_replaced`, no retractions, max `deadline_pct` 21.7%.

## 1. Game by game, and the decisions that decided them

**125v162 - LOSS, turn 25, 0 vs 18.** The seat out-played the draw-punisher deck for twenty turns:
seven Staff pings at Ob Nixilis and Fate Unraveler (seq 54, 63, 70, 87, 88, 107, 109) killed the
punishers, both Paths went at real bodies (seq 83 Master of the Feast 5/5 flier, seq 86 Fate
Unraveler), and it reached turn 23 at 18 life with Ob Nixilis dead. **The deciding decision is
seq 89.** With `DISCARD PUNISHERS on the battlefield: theirs - Liliana's Caress` printed as its own
paragraph and the Revelation row carrying
`{... CLEANUP PRICE at X=7: that leaves 14 cards in hand against a maximum hand size of 7, so ...
the cleanup step forces up to 7 discards you cannot decline, and the opponent's Liliana's Caress
punishes every discard for 2 life each = up to 14 life - at worst you would be at 4}`, the seat cast
it and announced X=7 (seq 90). At seq 94 the cleanup ask read "your hand has 17 cards ... you must
discard exactly 10 cards now ... these 10 discards cost you 20 life - you would be at 5". Life 25 ->
5 in one step; dead two turns later. Rows 3 ("Cast nothing right now") and 4 (hold) were on that
menu. Secondary break at the same seq 94: four lands sat on the numbered list (#7, #12, #13, #17) and
the seat sent three of them **plus a Cancel** (#2), against the guide's absolute that a counterspell
is never part of the answer while a land is on the list.

**125v126 - WIN, turn 60, 52 vs 0.** Correct play against a wall deck: every wall spell let resolve
(seq 7, 8, 47, 48, 171, 214, 216, 232, 233, 246, 280, 298, 304, 315), Fall of the Gavel on Sanguine
Bond (seq 64) and Exquisite Blood (seq 79), Cancel on the second Exquisite Blood (seq 215) and on
their Staff of Nin (seq 275), Dream Fracture on Tribute to Hunger (seq 263) and Idyllic Tutor
(seq 286), Fall of the Gavel on Sorin (seq 288). The declines at seq 94 (Sorin), 116/118 (Idyllic
Tutor, Sanguine Bond) and 196 (their Staff) are NOT counter-on-sight breaks: the hand held only
Essence Scatter and Path, and Essence Scatter is correctly rendered
`[no cast row now: it must have a target and there is no legal target on the board]` against a
non-creature spell. The cost of this game is not the result but its LENGTH - see item MED-1.

**125v123 - WIN, turn 38, 45 vs -7.** Textbook. Counters spent on the real threats (Dream Fracture on
Devour Flesh seq 14 and Damnation seq 27, Cancel on Bloodline Keeper seq 17 and Vision Skeins
seq 19), Path on Thraben Doomsayer (seq 31), first Staff seq 25 and second seq 47, then Emrakul at
seq 58 off a menu with no `LIFE-TO-DAMAGE CONVERTER` line, attacking on the extra turn at seq 64.
Every clause of the guide's Emrakul carve-out fired as written.

**152v125 (rerun) - LOSS, turn 31, -2 vs 8.** Clean seat play: 15 of 15 land drops, six counters
spent on real creatures (seq 8, 12, 13, 26, 35, 38, 45), two Supreme Verdicts off a nonzero header
(seq 25, 34), Elixir cast+activated at 10 life (seq 21, 22), both Staffs pinging every upkeep. It
lost on cards: from seq 30 (turn 20) the hand was Supreme Verdict + Essence Scatter with the
opponent at zero creatures, and the last four draws (seq 48) were all lands. No guide break found.

**146v125 - LOSS, turn 17, -7 vs 20.** Seventeen decisions in sixteen turns; two 4/4 Angels off
Emeria's Call (turn 13, a sorcery Essence Scatter cannot answer) plus Nadaar ended it. One break:
**seq 6**, a Revelation cast at `max affordable X=1 (4 mana total)` at 17 life, against the guide's
max-affordable-3 floor. It did not cost the game (the tap-out gate was open - the only counter in
hand was a Fall of the Gavel the 4 sources could never pay for) but it is the same break as below.

**125v130 - LOSS, turn 30, 0 vs 20.** **Deciding decision seq 20/21**: turn 19, 20 life, a Revelation
row reading `max affordable X=2 (5 mana total)` taken for X=2, tapping five of five. Next turn
Molten Rain (2 damage + a land) and Hammer of Bogardan (3) resolved with no window offered, and the
Hammer recurred from the graveyard on turns 22, 26 and 28 for 3 apiece. Life 22 -> 0 with the seat
declining a dead Final Judgment / Supreme Verdict at every window (seq 24-37) - correctly, per the
guide's zero-header line, since deck130's board had **no creatures at all**. Note the engine was
right at every one of those windows: from turn 21 on the seat's only counter was an Essence Scatter,
which cannot target a sorcery, so no priority window was owed and none was given.

## 2. Engine / interface / card items

**HIGH-1. The `CLEANUP PRICE` tail undercounts, because it ignores draw triggers already on the
stack in the same prompt.** File
`matchups-20260905-133058/1788633063-ai_baka_deck125-0x561ff3408350-vs-ai_baka_deck162.jsonl`,
seq 89. That prompt's own `ON THE STACK` block lists four pending draw abilities (Howling Mine x2,
Dictate of Kruphix x2), yet the row prices only the current hand plus X:
`CLEANUP PRICE at X=7: that leaves 14 cards in hand ... up to 7 discards ... = up to 14 life - at
worst you would be at 4`. Actual, at seq 94: hand 17, ten forced discards, 20 life, 25 -> 5. Two
independent errors that happened to cancel: the card count omits the stacked triggers (14 vs 17),
and "you would be at 4" is computed off the PRE-cast life (18) instead of 18 + X. A seat told "at
worst you would be at 4" and then billed to 5 from 25 cannot calibrate on this number. Repro: read
seq 89's row 2 and seq 94's ask header in that file.

**MED-1. The hold row is offered constantly and is almost never the answer, and nothing in the
render escalates.** Across the six games the hold row was printed 459 times and taken 6; the
`[you declined this exact list N times already this turn]` tag was printed 204 times and the hold row
was taken on 4 of them. 147 of the misses are 125v126 alone, which ran 60 turns and 324 decisions
against a board that never threatened it (same file, seq 118 carries
`[you declined this exact list 4 times already this turn]` and is answered "Cast nothing right now").
The tag counts up but never changes the menu; a render that promoted the hold row to option 1, or
suppressed the identical re-ask outright once the count passes 2, would remove most of the corpus's
decision volume. This is an interface item, not a guide one - the guide already states the rule as
hard as it can be stated.

**MED-2. A land is only tagged `{spare:}` when the lands in play cover the most expensive card in
hand, so Emrakul in hand un-tags every land at the cleanup ask.** Same file, seq 94: eleven lands in
play, and all four land rows read `{you control 11 lands already; the most expensive card in your
hand costs 15}` with NO `{spare:` prefix, because Emrakul {15} is on the list. In 125v126 seq 39 the
same thing happens at six lands against a Staff of Nin {6}. The guide's cleanup ordering keys on the
literal string `{spare:`, and the one deck in the pool that always holds an uncastable {15} is the
one deck that never gets the tag. Suggested fix: exclude cards the engine has already priced as
uncastable-this-game (or simply cap the comparison at the seat's land count + 2) from the "most
expensive card in your hand" used by the spare test.

**LOW-1. Counterspells in hand are tagged `[no cast row now: it must have a target and there is no
legal target on the board]`.** e.g. same file seq 89 (Cancel), 146 file seq 14 (Essence Scatter,
Dream Fracture, Fall of the Gavel). The sentence is true of *now* and is the same string used for a
Path with no creatures out, but for a held counterspell it reads as a statement that the card is
inert, at exactly the windows where the tap-out gate needs the seat to price it as live. The cast
rows do carry the correct price (`Essence Scatter {1}{u} in your hand needs 2`), so this is cosmetic;
a variant string ("no legal target while the stack is empty - it answers a spell they cast later")
would remove the ambiguity.

**LOW-2 (not a defect; recorded so a later reader does not chase it).** `post_answer_overrun` is
nonzero on 376 of 601 seat decisions, but every one of them is the sanctioned `PLAN:` line: the
counter measures everything after the first coded-answer line, by design
(`src/AIPlayerGPT.cpp:13628`). Max seen 503 bytes. There are no reply spirals in this seat's corpus.

**Card facts checked against the primitive** (`bin/Res/sets/primitives/mtg.txt`): Liliana's Caress
`auto=@discarded(*|opponenthand):life:-2 opponent` (l.67351) - the 2-per-discard billing and the
cleanup-step firing are both correct, and Teferi's Puzzle Box (l.118332,
`bottomoflibrary all(*|opponenthand) && draw:countedamount`) correctly does NOT fire it. Sphinx's
Revelation (l.110851), Staff of Nin (l.112425), Howling Mine (l.55935), Dictate of Kruphix
(l.29399), Fate Unraveler (l.39182) and Hammer of Bogardan (l.52046, a sorcery - so Essence Scatter
genuinely could not answer it in 125v130) all match their Oracle text. No card defect found.

**What HELD, worth recording as a lane result.** All 129 `Play <land>` rows across the six seat
files classified enters-tapped correctly against the battlefield: 45 Glacial Fortress UNTAPPED with
a Plains or a Tundra out, 6 TAPPED with neither; 18 Seachrome Coast UNTAPPED at <= 2 other lands, 60
TAPPED above it. Zero false brackets - the contradiction the guide spent eleven lines teaching
around is gone. (85 land-drop windows, 85 lands played, 0 declined.)

## 3. Guide verdict: EDIT

Three of the six games were decided by the seat, and one of those - 125v162 - turned on a printed
fact the guide has **no line for at all**: the `DISCARD PUNISHERS` paragraph and the `CLEANUP PRICE`
tail. That is what pays for this edit. The other two guide breaks in the corpus (the
max-affordable-X floor, the cleanup counterspell-before-land absolute) are already stated at maximum
force; restating them would be noise, so they are only re-dated with this corpus's citations.

Revised guide: `wave62/deck125/strategy.txt`, 70,962 bytes (live guide 70,999; band 41-71 KB).

**Edit 1 - ADD the discard-is-damage stop** (new paragraph immediately above `THE DRAW-IS-DAMAGE
STOP` in the Sphinx's Revelation section) and one matching `DECIDING SITUATIONS` bullet.
before: nothing in the guide mentions `DISCARD PUNISHERS`, `Liliana's Caress` outside the cleanup
section, or the `CLEANUP PRICE` tail; the Revelation section's only stop keys on
`"Whenever an opponent draws"`, which was correctly absent at seq 89.
after: "THE DISCARD-IS-DAMAGE STOP ... WHILE THAT PARAGRAPH IS PRINTED, A REVELATION ROW CARRYING A
CLEANUP PRICE TAIL IS ANSWERED 'Cast nothing right now' - at any max affordable X, at any life
total, at any amount of open mana. AND THE TAIL UNDERSTATES: at 125v162 seq 89 it priced 7 discards
and 14 life; ... the cleanup at seq 94 asked for TEN discards and billed 20 life, 25 down to 5 ...
use an Elixir of Immortality ACTIVATION: 5 life, no card drawn, nothing discarded."
paid for by: 125v162 seq 89 -> seq 94 (25 -> 5, dead at seq 112). The stop is written to key on the
two printed strings, and it deliberately does NOT tell the seat to trust the tail's arithmetic,
because HIGH-1 shows the arithmetic is low.

**Edit 2 - REPLACE the stale land-bracket paragraph.**
before: "ONE THING THE LAND ROW GETS WRONG ... the bracket asserts TAPPED, the quoted sentence after
the colon states the condition, and the condition is what actually decides ... twenty-eight of the
thirty-four such rows this corpus were false that way. READ THE QUOTED SENTENCE AGAINST YOUR
BATTLEFIELD LINE, not the bracket." (11 lines)
after: "WHICH LAND: THE BRACKET IS NOW TRUE - READ IT AND TAKE IT. All 129 land rows this corpus
classified correctly against your own battlefield line; the bracket and the quoted sentence after it
no longer disagree, so nothing here needs re-deriving. When an UNTAPPED row and a TAPPED row are
both offered, take the UNTAPPED one ..." (8 lines, colour lesson and the 125v162 seq 4/7 citation
kept)
paid for by: 129 of 129 land rows correct across the six seat files (counts above). The old text taught
the seat to distrust a now-correct surface; leaving it in is a live risk of the seat second-guessing
a true bracket, and removing it funds the byte budget for Edit 1.

**Edit 3 - re-date three evidence passages that this corpus falsified** (no rule changed):
- ping section: "of fifty-nine ping windows, the five that printed a DRAW PUNISHERS line went at Ob
  Nixilis ... Both pings this corpus went at the face ... (125v162 seq 33, seq 36)" -> "sixty ping
  takes this corpus, and all seven offered under a DRAW PUNISHERS line went at the punisher (125v162
  seq 54, 63, 70, 87, 88 at Ob Nixilis, 107 and 109 at Fate Unraveler)". The old text told the seat
  it had just failed a rule it in fact executed 7 for 7.
- Revelation floor: "125v146 seq 15 ... seq 17 ... 125v123 seq 13" -> "125v146 seq 6 (X=1 at 17
  life), 125v130 seq 20 (X=2 at 20 life), 125v126 seq 34 (X=2 at 25 life)", plus the observation
  that all three were in the seat's own Main phase 1.
- hold row: "Fifty-seven such windows this corpus and you held on twenty-one" -> "204 windows
  carried that tag this corpus and you held on FOUR - 147 of the misses are one game, 125v126".
- cleanup lands: the three-example paragraph is replaced by 125v162 seq 94 plus one prior example.
- Emrakul "WHAT THIS COST": the 125v152 seq 57 citation (that pairing hung; its rerun printed no
  Emrakul row) is replaced by 125v123 seq 58/64, won 45 to -7.
- Forced Fruition section compressed from 20 lines to 16 (rule unchanged; it has now not appeared in
  two corpora).

## 4. Proposals

None. No `general-proposals.md` and no `skill-proposals.md`: HIGH-1 and MED-1/MED-2 are engine and
render items for the engine seat, and everything the seat got wrong that a guide could fix is either
the deck-specific discard stop above or a rule the general guide and the strategy-writing skill
already carry.

## 5. What I did NOT check

- The deck125 seat only. Opponent seats were read solely for context on stack contents; no opponent
  decision is adjudicated here.
- The hung 152v125 translogs beyond confirming the hang - none of its 39 records is cited as
  evidence.
- The lane-{W,X,Y,Z,AA} falsifiable predictions: not adjudicated (engine seat's deliverable). The
  land-classifier observation above is offered as input, not as a verdict on D1.
- Scryfall was not queried; every card fact was taken from the primitive, which matched the rendered
  `{card text:}` in every case examined, so no disagreement arose to report.
- No build, no game run, no git, nothing written outside `wave62/deck125/`.
- The mulligan/bottom branch: only two such asks in the six games (125v130 seq 1-3, a mulligan to
  six then bottoming Emrakul, which is the guide's first bottoming rule) - too small a sample to say
  anything about the bottoming order the guide learned last wave.
