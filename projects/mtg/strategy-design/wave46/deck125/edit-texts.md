# deck125 (Revelation Fracture) - wave-46 edits, before -> after

Corpus `matchups-20260826-155842`, seat files `*ai_baka_deck125-0x*`. 502 decisions / 6 games,
3W-3L. Every card fact below is read off `projects/mtg/bin/Res/sets/primitives/mtg.txt` with
`/usr/bin/grep`; every render string is quoted from the corpus, not paraphrased.

Nine edits. Two wave-45 rules are UNTOUCHED because they passed outright: RULE #0 (the land drop,
40/40 windows taken, 0 declines) and the `[defender]` counter clause (0/39 counters on a defender
body, was 4/33). Skill #35 - do not edit a rule that just held.

---

## W1 - COUNTERS: mana artifacts named into the let-it-resolve class
FINDING: 39 of 43 counter windows taken; 2 of the 39 counters went to an artifact whose text only
makes mana - `Cast Dream Fracture` on `opponent's Chromatic Lantern {3} (artifact) [spell]`
(`...deck125-...-vs-ai_baka_deck126.jsonl` seq 10, turn 7) and on `opponent's Talisman of Impulse
{2} (artifact) [spell]` (`...vs-ai_baka_deck130.jsonl` seq 8, turn 7). The Lantern one is inside
the vs126 loss: it was one of only three counters that game, and the Sanguine Bond it was fixing
mana for resolved on turn 12 uncountered. The guide's existing line ("anything printed (land),
(artifact) with no P/T ... LET IT RESOLVE") already covers it and did not fire - the class had no
NAME and no example.

BEFORE:
```
  - anything printed (land), (artifact) with no P/T, or (instant)/(sorcery) that only draws or
    searches: LET IT RESOLVE.
```
AFTER: the same line plus a named MANA ARTIFACT paragraph (Chromatic Lantern {3}, Talisman of
Impulse {2}, "any artifact whose {target text:} says 'Add' and nothing else") with the turn-7 /
turn-12 chain written in. Plus a DECIDING SITUATIONS line naming both cards.
Card facts: Chromatic Lantern primitive `auto=lord(land|mybattlefield) transforms((...add{B}...))`
+ `{T}:Add{G}/{R}/{U}/{B}/{W}`, `mana={3}` - it makes mana and does nothing else.

## W2 - THE DRAW-IS-DAMAGE STOP (new hard rule at the top of the Revelation section)
FINDING, and it is the vs162 loss (`...vs-ai_baka_deck162.jsonl`, lost turn 18 from 25 life).
Their battlefield line at seq 39: `Howling Mine #1 {2} [artifact] {effect (each copy of this card
does this): "At the beginning of each player's draw step, if Howling Mine is untapped, that player
draws an additional card."}`, `Howling Mine #2`, `Howling Mine #3`, `Underworld Dreams #1 {b}{b}{b}
[enchantment] {effect (each copy of this card does this): "Whenever an opponent draws a card,
Underworld Dreams deals 1 damage to him or her."}`, `Underworld Dreams #2`, `Ob Nixilis, the
Hate-Twisted ... {effect: "Whenever an opponent draws a card, Ob Nixilis ... deals 1 damage to that
player."}`, `Fate Unraveler {3}{b} (3/4)`. The narration for turn 17 is thirteen consecutive
`dealt 1 damage to you` lines: life 20 -> 5 inside a single draw step. At seq 36, at 6 life, the
model then cast Sphinx's Revelation for X = 1 and drew itself to 4.
PERCEPTION vs STRATEGY: **STRATEGY** - the `{effect:}` text was printed on every one of those
permanents (lane G, #W46-3, renders here).
The guide named all three cards on its COUNTER ON SIGHT list and said nothing about what to do once
they RESOLVE. Its only stop-drawing rule was keyed to the opponent's life being 10 or less.

BEFORE: (the Revelation section opened straight into the mana threshold, with only)
```
Once the opponent's printed life is 10 or less and you control a Staff of Nin, stop drawing
entirely: ...
```
AFTER: a `THE DRAW-IS-DAMAGE STOP, AND IT OUTRANKS EVERY OTHER LINE IN THIS SECTION` paragraph
keyed to the rendered `{effect:}` substring "Whenever an opponent draws" (not to a name list - the
substring is what the render prints and it generalises past this pool), plus a DECIDING SITUATIONS
line. Old line retained beneath it.

## W3 - SPHINX'S REVELATION: X = 3, flat. The hand-count subtraction is deleted.
FINDING: 17 X windows. The wave-45 rule was `X = 8 - (cards on your hand line)` with a library
ceiling. **Conformed 4/17.** 7 of the 17 casts were at a hand line of 8 or more (wave 45: 6/12).
The mechanism is now visible: the model cannot count the hand line, because the ask does not print
a count of it. Of 4 replies on this seat that stated a hand size in words, **3 were wrong** -
`...vs-ai_baka_deck162.jsonl` seq 30 says *"hand size 6"* against a `Your hand:` line listing
**eleven** cards; seq 29 the same; `...vs-ai_baka_deck152.jsonl` seq 57 says 7 against 4.
PERCEPTION vs STRATEGY: **PERCEPTION** at the root (the number the rule needs is not rendered -
see the seat file's HIGH engine item #1), so the guide must stop asking for it.

BEFORE: `ONE SUBTRACTION, off one rendered number: COUNT the cards listed on your "Your hand:"
line. Call that number N. YOUR X IS 8 MINUS N.` plus two checks and two worked examples.
AFTER: the section header becomes `X IS ALWAYS 3` and the body `YOUR X IS 3. NOT THE LARGEST
NUMBER ON THE MENU.` - one constant, no arithmetic in front of it, with the library comparison
(`Your library: N cards`, minus 2, minus one per Staff) as the ONLY thing that can lower it. The
reason is stated as the ABSENCE of a field ("the ask prints your library, it prints their hand
size, and it does NOT print a count of your own hand"), not as distrust of the hand list itself -
the trust doctrine forbids teaching doubt of the render, and the hand list is not what is wrong
here.
WHY 3: 8-N was 3 or less in 14 of the 17 windows and 4 in the other three, so a flat 3 costs at
most one card in the best case and prevents every disaster in the corpus (the model announced 7
once and 8 three times). The wave-45 gate `Mana available: 6 or more` already means "X = 3 or
better", so the guide's own entry condition and its X now agree on one number.
SUNSET TEST (wave 47): if the engine ships a rendered own-hand count, replace this constant with a
subtraction off that field and re-measure; the constant is a workaround for a missing render, not
a strategy claim.

## W4 - THE TAP-OUT GATE: one gate for every own-turn spend, and Fall of the Gavel inside it
FINDING, and it is the vs126 loss (`...vs-ai_baka_deck126.jsonl`, lost turn 27 from 27 life).
Turn 11, seq 15-17: hand `Fall of the Gavel {3}{u}{w}; Path to Exile {w}; Emrakul; Supreme Verdict;
Elixir of Immortality {1}; Island`, `Mana available: 5 total`. The model cast the Elixir ({1}) and
then activated it ({2}), leaving 2 untapped. Turn 12 narration: `Opponent cast Sanguine Bond` /
`Opponent's Sanguine Bond resolved and entered the battlefield` - **no counter window was offered**,
because Fall of the Gavel costs 5 and 2 were open. Sanguine Bond is the FIRST name on this guide's
COUNTER ON SIGHT list. Two Bonds and an Exquisite Blood later, deck125 died on turn 27.
Two defects, both the guide's:
 (a) the counterspell subtraction named only Cancel, Dream Fracture and Essence Scatter - **Fall of
     the Gavel was not in it**, and it was the only counter in hand;
 (b) the Elixir section had NO payability gate at all ("Activate it in your OWN main phase with
     whatever mana is left after the spell you wanted this turn").

BEFORE (appeared twice, verbatim, in two sections):
```
Before you spend mana on Sphinx's Revelation, Staff of Nin or Elixir of Immortality on your own
turn, look at your hand for Cancel, Dream Fracture or Essence Scatter. If one is there, subtract
3 from your "Mana available:" line (2 for Essence Scatter) and use the remainder as the number
the rules below ask for.
```
AFTER (both places): `THE TAP-OUT GATE` - all FOUR counters named, one threshold ("the spell must
leave your Mana available: line reading 5 or more afterwards"), scoped to every own-turn spend
including an Elixir ACTIVATION. Five because Fall of the Gavel is {3}{U}{W}.
Plus the Elixir section's activation line rewritten (below) and two DECIDING SITUATIONS lines.

## W5 - ELIXIR OF IMMORTALITY: activation gated
BEFORE: `Cast it the turn it is listed when you have a spare {1}. Activate it in your OWN main
phase with whatever mana is left after the spell you wanted this turn, or at any point your life
is 15 or less.`
AFTER: the tap-out gate applies to the activation; outside it, activate only at life 15 or less or
library 16 or less. The turn-11/turn-12 chain is written in.
COUNT: 35 Elixir-activation windows, 15 taken. Library never fell below 20 in any offered window
(wave 45: never below 23), so the guide's `library <= 16` trigger is still UNTESTED for a second
wave - the eager activation is why it never gets there. No decking loss (0/6).

## W6 - STAFF OF NIN: one constant, 9
FINDING: 7 Staff casts. Under the wave-45 branch (8 with an Essence Scatter / 9 with a Cancel or a
Dream Fracture / 11 with only a Fall of the Gavel / 6 with none), **4 of 7 conformed** - the same
rate as wave 44's 6/11 and wave 45's 3-4/7, three waves flat. Violations:
`...vs-deck146.jsonl` seq 23 (`Mana available: 6`, Cancel in hand, needs 9),
`...vs-deck162.jsonl` seq 21 (7, Essence Scatter, needs 8),
`...vs-deck152.jsonl` seq 25 (6, Cancel, needs 9).
The rule is right and the four-way branch is what fails.
BEFORE: the four-way subtraction quoted above.
AFTER: `CAST STAFF OF NIN ONLY WHEN YOUR "Mana available:" LINE READS 9 OR MORE.` One number, with
the no-counter-in-hand exception at 6 kept as the single carve-out.
WHY 9: 9 - 6 = 3, which is Cancel and Dream Fracture. It reproduces every one of the seven
verdicts in this corpus (the four conforming casts read 17, 10, 13, 22; the three violations read
6, 7, 6). An Essence-Scatter-only hand loses one turn under it; that is the price of one number.
n = 7 - reported directionally per amendment #45. Do not close the lane below n = 10.

## W7 - SWEEPERS: subtract the walls before you read the header
FINDING: 165 sweeper-offering windows, 10 casts. **0 casts at a header of 0 creatures across 123
zero-creature windows** (wave 45: 1 of 9 casts; wave 44: 3 of 14) - the hoisted gate is now clean
and W19's first half PASSES. The NEW failure is one branch down: 2 of the 10 casts were into boards
whose entire creature count was 0-power defenders.
 - `...vs-ai_baka_deck126.jsonl` seq 13, turn 8, life 20, Path to Exile also in the menu:
   `Opponent battlefield (6 permanents listed, of which 2 are creatures): ... Pride Guardian {w}
   (0/3) [defender]; ... Overgrown Battlement {1}{g} (0/4) [defender]` - Supreme Verdict cast.
 - `...vs-ai_baka_deck126.jsonl` seq 43, turn 24, life 24: header reads 3 creatures, and they are
   Pride Guardian (0/3) [defender] and two Overgrown Battlement (0/4) [defender].
The guide's printed-power/defender clause exists only in the `exactly 1 creature` branch; the
`2 or more` branch reads the header number raw.
BEFORE: `IF THAT NUMBER IS 0, ...` immediately after `READ THAT NUMBER FIRST`.
AFTER: a `BEFORE YOU USE THAT NUMBER, SUBTRACT THE WALLS` paragraph (every `[defender]` and every
printed power 0 comes off the count) above the branches, plus the DECIDING SITUATIONS entry
restated as "2 or more creatures AFTER you subtract every [defender] and every printed-power-0
body". Same rule the Path and 1-creature branches already run, applied to the count itself.

## W8 - EMRAKUL: into DECIDING SITUATIONS
FINDING: `Cast Emrakul, the Aeons Torn` was printed in the numbered list **5 times** and taken
**once**. The four declines are `...vs-ai_baka_deck123.jsonl` seq 85, 86 (turn 38) and seq 90, 91
(turn 40); the take is `...vs-ai_baka_deck152.jsonl` seq 179 (turn 63). The rule is already
absolute in the guide body ("If 'Cast Emrakul, the Aeons Torn' is ever actually printed in your
numbered list, take it") and it was NOT in the situations list - the wave-44 lesson that a rule
absent from that list does not fire (the sweeper gate went 3/14 -> 1/9 -> 0/123 once hoisted).
AFTER: first entry in DECIDING SITUATIONS.

## W9 - BOTTOMING: walk the numbered list
FINDING: one bottoming ask this corpus (`...vs-ai_baka_deck152.jsonl` seq 3). Hand: `Plains;
Elixir of Immortality {1}; Staff of Nin {6}; Island (copy 1 of 2 in your hand); Sphinx's
Revelation; Island (copy 2 of 2 in your hand); Glacial Fortress`. The guide's order is "Emrakul
first, then a second copy of a card you already hold, then your lands beyond the fifth" - entry 2
matches (two Islands, and the render marks them `(copy 2 of 2 in your hand)`). The model bottomed
the Sphinx's Revelation. VIOLATION, unpunished (that game was won on turn 64).
BEFORE: one sentence, three clauses, no anchor to what the ask prints.
AFTER: a four-step first-match walk keyed to the ask's own numbered list and to the render's
`(copy 2 of 2 in your hand)` string, with a terminal "most expensive spell" step so the order can
never run out (it did run out on this hand: no Emrakul, and only 4 lands).

---

## THRESHOLD WHYS (skill amendment 61) - updated for this wave
- X = 3 because 8-minus-hand-count was 3 or less in 14 of 17 windows and the input it needs
  (a count of your own hand) is not rendered; above 3 the end-of-turn discard starts.
- Staff of Nin at 9 because 9 - 6 (Staff's cost) = 3, which pays Cancel and Dream Fracture.
- The tap-out gate leaves 5 because Fall of the Gavel is {3}{U}{W} and it is the counter most
  often held alone.
- Elixir at library 16 (unchanged, still UNTESTED) because with two Staffs you draw three a turn
  and the offering window comes round every other turn.
- Sweeper at 2 creatures (unchanged) - but the count is now creatures that can ATTACK, because a
  0-power defender cannot take a point off you and you have no creature for it to block.
- Revelation entry at `Mana available: 6` (unchanged) because {X}{W}{U}{U} at X = 3 is 6.
