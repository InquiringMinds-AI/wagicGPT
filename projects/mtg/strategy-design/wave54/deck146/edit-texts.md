# deck146 - wave-54 edits (before -> after), each with its finding

Live guide 58,288 B -> 60,762 B (+2,474; pool band 39.6-74.7 KB). Five edits: three additions,
two re-keys that also shorten. Every literal below was read off this corpus's rows.

---

## 146-A (NEW) - the DRAW PRICE spell tax is a turn-level close, not a card-level hesitation
**Finding.** `146v162` s46 (T25, 8 life), s49 (T27, 7), s53 (T27, 8), s55 (T29, 6): four windows,
4-7 untapped sources, menus of 5-7 live creature/sorcery rows, all answered `Cast nothing right
now`. The opponent line carried `Forced Fruition {4}{u}{u} {effect: "Whenever an opponent casts a
spell, that player draws seven cards."}` plus one and then two `Underworld Dreams`. Five of the
six rows at s46 carry `[DRAW PRICE: casting this draws YOU 7 cards (their Forced Fruition), and
their Underworld Dreams deals you 7 - you would be at 1]`. The pilot answered correctly all four
times and won the game 4/-1 on attacks alone - but from the render, with **no sentence in this
guide about it**, and against a guide whose standing imperative is "Do NOT sit on removal".
20 such rows corpus-wide, 26 at this deck, **0 casts from any of them.**
**Why the edit.** An unaided derivation that survived four windows is not a rule; the next
Forced Fruition seat gets it or does not. The edit also states the half the pilot never wrote:
the tax is on SPELLS, so the land drop, the attack, the dungeon room, the Hive animation and a
Kaya loyalty activation are all still free - which is exactly how that game was won.

BEFORE (DECIDING SITUATIONS, first entry):
```
DECIDING SITUATIONS (recognize -> do):
- Their battlefield line shows BOTH "Sanguine Bond" and "Exquisite Blood" -> ...
```
AFTER (new first entry inserted):
```
DECIDING SITUATIONS (recognize -> do):
- A cast row carries "[DRAW PRICE: casting this draws YOU N cards (their <card>), and their
  <punisher> deals you N - you would be at K]" -> that bracket is a PRICE ON CASTING AT ALL, not
  on that card, and when K is at or below 1 the whole cast menu is closed for the turn: every
  other row on the same menu is paying the same N whether the engine wrote the tail on it or not.
  Answer "Cast nothing right now" and WIN THE TURN ANOTHER WAY - the tax is on SPELLS, so your
  land drop, your attack, your dungeon room, your Hive animation and your Kaya loyalty ability
  are all still free. Do not read this as "sit on removal": you are not holding the card for a
  better moment, you are declining to pay seven life for it. WHAT THIS BOUGHT: four windows at
  8, 7, 8 and 6 life against Forced Fruition plus one and then two Underworld Dreams, a full
  six-row menu of Barrowin, Acererak, Nadaar, Silencer, Command and Emeria's Call declined every
  time, and the game won at 4 life on attacks alone with the opponent taken to -1.
```

---

## 146-B (RE-KEY) - "most expensive first" means the mana cost printed on the row
**Finding.** 7 cleanup discards this corpus. The Emeria's-Call half **PASSES 1/1**: `146v125` s20
had Emeria's Call at row 6 and answered `PUT: 6` (wave 53: 0 of 3) - that sentence is left alone,
not restated. The residual is the general order, **2 of 7 off it**: `146v130` s13 sent Pelakka
Predation `{2}{b}` while `Lolth, Spider Queen {3}{b}{b}` sat at row 4; `146v162` s39 sent Acererak
`{2}{b}` while two `Kaya the Inexorable {3}{b}{w}` and a Lolth sat on the list. The discard ask
prints every card's mana cost on its own numbered row; nothing had to be remembered.

BEFORE:
```
  the numbers of the cards you SEND, most expensive first: Emeria's Call, Agadeem's Awakening, a
  second planeswalker,
```
AFTER:
```
  the numbers of the cards you SEND, most expensive first - and "most expensive" is the MANA
  COST PRINTED ON EACH NUMBERED ROW, read off the list in front of you, not a card you remember
  being dear: a {3}{b}{b} Lolth or a {3}{b}{w} Kaya on the list goes under before a {2}{b}
  Pelakka Predation or a {2}{b} Acererak, every time (twice in one corpus the three-mana card
  went and the five-mana walker stayed). In order: Emeria's Call, Agadeem's Awakening, a
  second planeswalker,
```

---

## 146-C (NEW) - a discard spell is not an answer to a board
**Finding.** `146v123` s13, T9, 20 life vs 17. Own line `2 creatures`; opponent line
`28 permanents listed, of which 23 are creatures, 23 of them without a restriction against
attacking` behind Intruder Alarm + Thraben Doomsayer. Menu: `1. Cast Acererak the Archlich
{2}{b} (5/5)`, `2. Cast Pelakka Predation {2}{b}`, `3. Cast Soul Shatter {2}{b} {right now: 23
tied at MV 0 (their highest) ...}`, `4. Cast nothing`. Answered **row 2**, the hand attack, whose
own plan sentence hoped to strip "likely another Thraben Doomsayer". Dead at -3 on turn 10. The
guide's existing anti-aggro posture ("deploy blockers early") had no recognise-surface keyed to
the opponent header's own creature count.

BEFORE (immediately above the "Emeria's Call ... plus lands" entry):
```
- Your hand is Emeria's Call, Agadeem's Awakening or Pelakka Predation plus lands -> those are
```
AFTER (new entry inserted above it):
```
- Their battlefield line reads "of which N are creatures, N of them without a restriction
  against attacking" with N at least three more than your own line's creature count -> a
  DISCARD spell is not a play in that window. Pelakka Predation takes one card out of a hand
  that is no longer where the game is; the same mana buys a BODY that blocks this turn and every
  turn after. Cast the biggest creature row you can afford, or the removal that kills their
  engine permanent - never the hand attack. WHAT THIS COST: at 20 life against 23 attacking
  creatures behind Intruder Alarm and Thraben Doomsayer, with an Acererak 5/5 and a Soul Shatter
  both on the same menu, this seat cast Pelakka Predation and was dead at -3 one turn later.
- Your hand is Emeria's Call, Agadeem's Awakening or Pelakka Predation plus lands -> those are
```

---

## 146-D (RE-KEY, B1 REVERSE audit) - the coverage line's new bracketed reason
**Finding.** Lane E/C changed the literal this corpus. Wave 53 rendered
`Playing every land in this hand would not cover any spell in it.` (10 renders, bare, period).
Wave 54 renders **0** of that string and **11** of a new one carrying a cause clause in two
families: COUNT (`(1 land is not enough for your cheapest spell at mana value 2)` x3,
`(2 lands are not enough for your cheapest spell at mana value 3)` x1) and COLOUR
(`(you have 3 lands; no spell in it is castable off {W}{B} alone)` and six siblings). This
guide's STEP 2 sentence quotes the OLD bare literal, so it is now stale in the reverse-audit
sense: the engine has begun printing, in its own voice, the exact colour argument the section's
COLOUR IS NEVER A REASON TO SHIP paragraph exists to refute. deck146 itself passed (0 of 6
mulligans; two 2-land keeps under the coverage line) - this is a re-key that keeps the rung.

BEFORE:
```
  At ONE land, that coverage line is
  the engine's own arithmetic and it is the whole one-land test:
```
AFTER:
```
  THAT LINE NOW PRINTS ITS REASON IN BRACKETS, AND ONLY ONE OF THE TWO REASONS IS A TEST.
  "(1 land is not enough for your cheapest spell at mana value 2)" is the engine's arithmetic on
  the land COUNT. "(you have 2 lands; no spell in it is castable off {B} alone)" is a COLOUR
  statement about turn one - it is the sentence COLOUR IS NEVER A REASON TO SHIP forbids, printed
  by the engine, and it ships nothing at any land count.
  At ONE land, the count-family line is
  the engine's own arithmetic and it is the whole one-land test:
```

---

## 146-E (TRIM, pays for the above) - the three-incident recital collapses to one sentence
**Finding.** deck146 mulligans at two or more lands: **0 of 6** this corpus (wave 53: 0 of N;
wave 52: 3 of 4). The rung is holding across two corpora; the recital of all three wave-52
incidents is no longer buying anything the rung's own sentence does not.

BEFORE:
```
however the coverage line reads. WHAT THIS COST, three times in one corpus: a 2-land seven
  under "would not cover" was shipped; a 3-land SIX under "would not cover" was shipped on top
  of another mulligan, down to (keeping 5); and a second game shipped a 2-land seven the same
  way. None of the three was on the ship list, and each paid a card for the same land odds.
```
AFTER:
```
however the coverage line reads. WHAT THIS COST, three shipped hands in one corpus (a 2-land
  seven, a 3-land six on top of another mulligan down to (keeping 5), and a second 2-land seven):
  none was on the ship list, and each paid a card for the same land odds.
```

---

## NOT EDITED, and why
- **The wave-53 146-A entry (Silverquill Command's return mode).** Lane C's D5 shipped
  `{modes live right now: ...; dead (no legal object right now): ...}`: **11 of 11** Silverquill
  Command rows carry it (was 0 of 28) and the card was **cast twice** (`146v126` s47 T23,
  `146v152` s41 T15) against 0 casts in 28 windows last wave. The entry now has a live key and
  needs nothing.
- **The show-the-work deletion (wave-53 P3 control).** Own-turn `Cast nothing` on a menu with a
  live, non-dead row: **14** this corpus, and **13 of the 14 are correct** - 4 DRAW-PRICE closes
  (146-A), 4 Acererak-only two-row menus at 6 life where Acererak bounces itself, 2 lethal-attack
  turns, 2 legend-rule duplicates, 1 tapped-out end step. The deletion cost nothing. PASS.
