# deck146 - wave-53 edits (before -> after, each with its finding)

Live guide under test: `bin/Res/ai/baka/deck146_strategy.txt` (54,731 B). Revised: 58,288 B (+3,557).
Corpus `matchups-20260902-023342`. Card facts verified with `/usr/bin/grep` against
`bin/Res/sets/primitives/*.txt`. Three edits, plus one deletion that pays for part of them.

---

## E1 - CAST ORDER entry 5: the Silverquill Command condition was keyed to a literal the
## emitter never prints (146-A, HIGH)

**Finding.** `146v125` s240 (T47, 22/40, 16 untapped sources), s528 (T79, 20/34, **20** untapped
sources) and ~18 sibling windows across turns 29-83: the seat answered `Cast nothing right now`
on a menu holding a live `2. Cast Silverquill Command {2}{b}{w} {leaves 12 of your 16 untapped
mana sources untapped} {card text: "Choose two -- ... Return target creature card with mana value
2 or less from your graveyard to the battlefield. ..."}`. The s240 reply's stated reason is FALSE
on the screen it was written from: *"my graveyard has no creature cards with mana value 2 or less
(return mode dead)"* - the narration directly above that menu carries
`- Your Triumphant Adventurer was COUNTERED by Essence Scatter and went to your graveyard` and
`- Your Silverquill Silencer was COUNTERED by Dream Fracture and went to your graveyard`.
Primitives: `Triumphant Adventurer ... mana={B}{W}` and `Silverquill Silencer ... mana={B}{W}`
(mana value 2 each); `Silverquill Command auto=choice name(Return creature and you draw)
target(creature[manacost<=2]|mygraveyard) moveto(mybattlefield) && draw:1 controller && life:-1
controller`. The return mode was live and it is a free body plus a card.

**Why the pilot could not see it (the #168 audit result).** The guide's third condition read
*"the Command row's own `{right now:` text names a card it would return"*. THREE-NUMBER AUDIT for
that literal on this row family: **28 `Cast Silverquill Command` rows corpus-wide, 0 carry
`{right now:`, 0 takes under it.** The literal itself is alive (1,586 occurrences corpus-wide on
other cards) - it is simply never emitted for this card. A condition keyed to a string that
renders 0/28 can never be satisfied, so entry 5 collapsed to entry 8b/9 every window.

**BEFORE**
```
 5. Silverquill Command - when at least ONE of these three is true on the screen: their
    battlefield line shows a creature (the sacrifice mode is live); your battlefield line shows a
    creature (the pump mode is live); the Command row's own "{right now:" text names a card it
    would return. Two live modes is a two-for-one and it is not a card to hold. When NONE of the
    three is true, its only live mode is a card for 1 life and it drops to entry 8b.
```
**AFTER** - the third condition re-keyed to text that is actually on the screen (the log lines
plus the two card names this deck owns at mana value 2), with the row's silence stated outright:
```
 5. Silverquill Command - when at least ONE of these three is true on the screen: their
    battlefield line shows a creature (the sacrifice mode is live); your battlefield line shows a
    creature (the pump mode is live); THE RETURN MODE IS LIVE - and that one is decided by your
    GRAVEYARD, which the Command row does not annotate. The row prints its card text and its
    "{leaves N of your M" clause and NOTHING ELSE: there is no "{right now:" clause on this card
    and there never will be, so do not wait for one. The return mode reads "Return target creature
    card with mana value 2 or less from your graveyard to the battlefield", and this deck owns
    exactly TWO creatures at mana value 2 - TRIUMPHANT ADVENTURER {b}{w} and SILVERQUILL SILENCER
    {b}{w}. So the test is a name search in the log lines above the menu: if the log has printed
    "Triumphant Adventurer" or "Silverquill Silencer" going to YOUR graveyard (died, was
    countered, was destroyed, was sacrificed) and that name is not on your battlefield line now,
    THE RETURN MODE IS LIVE and the Command is a body plus a card for four mana. Two live modes
    is a two-for-one and it is not a card to hold. When NONE of the three is true, its only live
    mode is a card for 1 life and it drops to entry 8b.
    WHAT THIS COST: about twenty "Cast nothing right now" answers to a live Command row across
    turns 29-83 of one game, at eight to twenty untapped sources and a board of two animated
    lands - one reply writing "my graveyard has no creature cards with mana value 2 or less"
    while the log above it showed both an Adventurer and a Silencer countered into that
    graveyard. Never call the return mode dead without searching the log for the two names.
```

**E1b - the matching DECIDING SITUATIONS bullet (#177: every condition in the prose appears in
its bullet, or the bullet wins).**

**BEFORE**
```
- Silverquill Command is on the list, no removal-with-a-target and no planeswalker is, and their
  line shows a creature OR your line shows a creature OR its row names a card it returns -> cast
  it. None of the three true -> it sits below Acererak (entry 8b), still above "nothing".
```
**AFTER**
```
- Silverquill Command is on the list, no removal-with-a-target and no planeswalker is, and their
  line shows a creature OR your line shows a creature OR the log shows "Triumphant Adventurer" or
  "Silverquill Silencer" in YOUR graveyard and not on your battlefield line -> cast it. The
  Command row carries no mode annotation at all, so the graveyard branch is a name search in the
  log, never a clause you wait for on the row. None of the three true -> it sits below Acererak
  (entry 8b), still above "nothing".
```

---

## E2 - one matchup paragraph for the archetype that has beaten this deck twice running
## (146-B, #180)

**Finding.** deck125 (counters + lifegain + recursion) took `146v125` in wave 52 (-10/53, T52)
and again this wave (14/39, **T85, 607 records, 88.2 min of inference**). Verified from the
primitives, deck146 has NO answer to their two colourless artifacts: `Elixir of Immortality
mana={1}`, `auto={2}{T}:life:5 && moveTo(mylibrary) all(this) && moveTo(mylibrary) all(*|mygraveyard)
&& shuffle` - used at T24, T29, T70, T78 for +20 life and four library refills; `Vanishing Verse
target=*[-multicolor;-colorless]|battlefield` cannot target it; `Soul Shatter auto=... sacrifice
notaTarget(*[creature;planeswalker;...])` cannot touch an artifact; only `Kaya the Inexorable
auto={C(0/0,-3,Loyalty)}:name(-3: Exile non-land permanent) target(*[-land]|battlefield)
moveto(exile)` can. Both Kayas were countered on the turn they were cast (T41 Fall of the Gavel,
T49 Cancel), as were two Nadaars, a Barrowin, a Lolth, three Silverquill Commands, two Acereraks
and a Silencer. The Silencer's naming ask at s10 named `Essence Scatter`; the log then shows
`Cancel` and `Dream Fracture` doing most of the countering.

**AFTER** - inserted immediately above `DECIDING SITUATIONS (recognize -> do):`, new text only:
```
AGAINST A DECK THAT COUNTERS AND GAINS LIFE (the deck whose log shows Cancel, Essence Scatter,
Dream Fracture, Fall of the Gavel, Supreme Verdict, Sphinx's Revelation, Elixir of Immortality,
Staff of Nin, Emrakul). This matchup has taken the last two games and both ran past turn 50.
- READ THE LOG FOR "was COUNTERED by <name>" AND NAME THAT CARD WITH SILVERQUILL SILENCER. The
  Silencer's naming ask is a list of their cards; the counterspell that has already eaten one of
  your spells is the card they will cast again, and every recast then costs them 3 life and draws
  you one. Name the counter, not their biggest creature - their creatures are the one thing your
  removal already answers.
- THEIR ELIXIR OF IMMORTALITY AND THEIR STAFF OF NIN ARE COLORLESS ARTIFACTS, AND KAYA IS YOUR
  ONLY ANSWER TO EITHER. Vanishing Verse exiles a MONOCOLORED permanent and its target list will
  simply not contain them; Soul Shatter takes a creature or a planeswalker and does nothing to an
  artifact. Kaya's -3 exiles ANY nonland permanent. So when their battlefield line prints "Elixir
  of Immortality" or "Staff of Nin", Kaya is not a value engine in this game - she is the removal,
  and the -3 goes at the ARTIFACT, not at a creature you could have blocked. An Elixir left alone
  gains them 5 and shuffles their graveyard back into their library every few turns; there is no
  number of Hive attacks that beats that.
- ONE SPELL A TURN INTO OPEN BLUE MANA LOSES. Three Kayas and two Nadaars were countered one at
  a time in one game. Prefer the turn where you can cast TWO things, so the counter they hold
  answers only half of it - and remember their empty board makes Silverquill Command's return
  mode LIVE (entry 5), not dead.
```

---

## E3 - DELETION: the show-the-work ritual (146-T, #181)

**Finding.** The wave-51 ritual fired **15 of 165** own-turn `Cast nothing right now` answers that
held another live `Cast ...` row (9%; it was 20/84 = 24% in wave 52, and #181 already ruled that
24% is its natural rate, not decay). What it protects did not break: of those 165 windows, **139
held only rows the engine had already marked dead** (`at 0 this does nothing` renders 396 times at
this seat and is taken 0 times; `[legendary:` 12 rows, 0 takes; `the only legal targets are YOUR
OWN` 0 takes), and the 26 with a genuinely live row are the Command lane (E1), two guide-legal
Acererak declines and four guide-forbidden Agadeem's Awakening declines. Cast-order misroutes
attributable to the missing sentence: **0**. A ritual that costs tokens on 165 windows, fires on
15 and changes no answer is deleted rather than restated.

**BEFORE**
```
 SHOW THE WORK ON THE
MENUS WHERE IT DECIDES ANYTHING: when you answer "Cast nothing right now" AND the menu holds at
least one other "Cast ..." row, your reply names the highest entry that was on
this menu and the reason it did not match, in the shape "nothing right now: entry <number>
<card name> was on this menu; it did not match because <a fact printed on this screen>". Fill
the three blanks from THIS menu and THIS board - a sentence about a card that is not on the menu
is not the work, it is the list going unread. If you cannot fill the blanks, the answer is not
"nothing" - go back and take the first line that matched. When "Cast nothing right now" is the
ONLY other row, or every other row is one the engine has already marked dead ("at 0 this does
nothing", "the only legal targets are YOUR OWN right now"), answer with the bare CHOICE line and
no sentence: there is nothing on that menu to show.
```
**AFTER**
```
 When every other row on the menu is one the engine has
already marked dead ("at 0 this does nothing", "the only legal targets are YOUR OWN right now",
"[legendary: you already control"), "Cast nothing right now" is the whole answer and needs no
sentence. When a row is NOT marked dead, the reason it does not match must be a fact printed on
THIS screen - if you cannot point at one, the answer is not "nothing": go back and take the
first line that matched.
```

---

## E4 - the cleanup discard had no recognise->do bullet (146-C, #177)

**Finding.** `146v130` (L 0/2, T18). Land-locked at two lands by Molten Rain (T7) and Stone Rain
(T9), the seat discarded at three straight cleanups - s15 (T13) `PUT: 8` Silverquill Command,
s17 (T15) `PUT: 5` Pelakka Predation, s18 (T17) `PUT: 3` Soul Shatter - with **Emeria's Call
{4}{w}{w}{w} on all three numbered lists** and never sent. The guide's cleanup order names
Emeria's Call FIRST, but only inside a bullet in the MULLIGAN section 500 lines above; the
DECIDING SITUATIONS list, which is the surface the executor pattern-matches, had no cleanup
bullet at all. **3 of 3 cleanup discards broke the order.** The seat died at 0 with the opponent
on 2 and a Nadaar it never found a third land for.

**AFTER** - one new bullet in DECIDING SITUATIONS, immediately above the Emeria's/Agadeem's/
Pelakka bullet:
```
- The ask says "your hand has N cards and your maximum hand size is 7" -> this is the cleanup
  discard, and the FIRST name you send is EMERIA'S CALL whenever it is on the numbered list -
  seven mana is a card you are not casting this turn or next. Then Agadeem's Awakening, then a
  second planeswalker, then a land once your battlefield line already prints four or more lands,
  then the priciest remaining spell. WHAT THIS COST: land-locked at two lands against land
  destruction, this seat discarded at three straight cleanups (Silverquill Command, then Pelakka
  Predation, then a Soul Shatter) with Emeria's Call sitting on all three lists, and died at 0
  with the opponent on 2 life and a Nadaar it never had the third land to cast.
```

---

## Considered and NOT made

- **Mulligan.** No edit. P1 passed clean (0 ships at 2+ lands; both mulligans were one-landers
  under `would not cover any spell in it`, the guide's own ship). The 595 s at `146v125` s1 is
  deliberation cost, not error, and #138 forbids a fourth restatement of a rule that is working.
- **The Hive `[this land is TAPPED: animated, it still cannot attack this turn]` take** at
  `146v125` s443 (1 of 47 such rows). First instance, unpunished; CHECK 3 already names the tag
  verbatim. Recorded under #118, not restated.
- **The dungeon choice.** `146v125` s14/s31 and `146v123` s7/s51/s72/s84 all answered LOST MINE
  OF PHANDELVER by name, which is what the guide dictates; the Acererak bounce is the guide's own
  stated consequence (`auto=if type(Tomb of Annihilation[dungeoncompleted]|myzones)~equalto~0 then
  ... moveTo(hand)` - verified). `146v123` completed the dungeon four times and won. No change.
