# deck146 - wave-56 guide edits (before -> after, each with its finding)

Base: live `bin/Res/ai/baka/deck146_strategy.txt` (64,394 B, wave-55 reviewer revision + the
wave-55 boundary pass). Result: `wave56/deck146/strategy.txt`, 67,328 B (+2,934, pool band 41-71 KB).
Nine edits, all from one root cause: **lane D's D8 made the three land-backed spells in this deck
actually playable as lands, and every rung of the guide that said otherwise is now false.**

---

## 146-A - RETIRE "NOT LANDS FOR YOU" (skill #238's P10 prediction fires)

FINDING. 62 windows in this corpus rendered a numbered `PLAY THIS AS A LAND` option row; **44 of
them at deck146 seats, and the seat took it 4 times, with the back face on its own battlefield line
in 4 of 4** (`146v125` s13 Agadeem, the Undercrypt; `146v126` s19 Pelakka Caverns; `146v152` s17
Agadeem; `146v162` s12 Emeria, Shattered Skyclave). Wave-55 P10 said this stays 0 "unless the
engine changes" and that a non-zero reading retires the block in the same wave. It is non-zero.

BEFORE
```
- EMERIA'S CALL AND AGADEEM'S AWAKENING ARE NOT LANDS FOR YOU. They are printed with a land on the
  back, but the game NEVER offers you their land face - it has not appeared on a single land-drop
  menu. Count them as SPELLS ONLY: a hand with two lands and an Emeria's Call is a TWO-land hand.
  The same is true of Pelakka Predation - it is a SORCERY only, never a land.
```
AFTER - the block is replaced by a THEY-ARE-LANDS block that also carries the hand-count
correction (see 146-H for why the count needs stating twice) and a removal condition. Full text in
`strategy.txt`; its two cited specimens are the corpus's two deck146 losses:
`146v152` s1 (header "0 lands" on a seven holding BOTH Agadeem's Awakening and Emeria's Call -
a two-land seven - mulliganed) and `146v130` s1/s2/s3 (header "1 land" twice, mulliganed the
first seven, then bottomed Agadeem's Awakening off the keep - its own second land - and died -3/8).

Card facts verified in `bin/Res/sets/primitives/borderline.txt`: `:1177` Agadeem's Awakening
`autohand={0}:restriction{can play land} ... flip(Agadeem, the Undercrypt) forcetype(land)`;
`:33968` Emeria's Call -> Emeria, Shattered Skyclave; `:82646` Pelakka Predation -> Pelakka
Caverns. The `compare(isflipped)~equalto~1` gate is gone from all three.

## 146-B - mulligan BOTTOM ask: the land-backed cards leave the "bottom first" rung
FINDING: `146v130` s3, the PUT line sent Agadeem's Awakening off a keep the header priced at one
land. The old rung named exactly that card as the first thing to send.
BEFORE `... bottom the rest starting with the most expensive (Emeria's Call, Agadeem's Awakening, a second planeswalker, then the priciest card left).`
AFTER  `... (a second planeswalker, then the priciest card left). EMERIA'S CALL, AGADEEM'S AWAKENING AND PELAKKA PREDATION ARE LANDS FOR THIS BUILD - never bottom one while your keep is at two counted lands or fewer, and never bottom one ahead of a spell you cannot cast.`

## 146-C - the bottom-build land tally
BEFORE `they cover. Hive of the Eye Tyrant IS a land for this count.`
AFTER  `they cover. Hive of the Eye Tyrant IS a land for this count, and so are Emeria's Call, Agadeem's Awakening and Pelakka Predation.`

## 146-D - cleanup-discard prose order
BEFORE `second planeswalker, then a land in hand once your battlefield line already prints four or`
AFTER  inserts `(Emeria's Call, Agadeem's Awakening and Pelakka Predation are NOT on this rung any more - they are lands, and go only under the land rung below)`.
FINDING: `146v126` s6, PUT: 8 = Agadeem's Awakening at T7 on a two-land battlefield - a land drop
thrown away, and the highest-cost card left on that list was a {3}{b}{b} Lolth at row 1.

## 146-E - the numbered cleanup SCAN's first two rungs
FINDING: same window. Rungs (1) and (2) named the two land-backed cards as automatic sends.
BEFORE
```
    (1) EMERIA'S CALL - send it. Seven mana is a card you are not casting this turn or next.
    (2) AGADEEM'S AWAKENING - send it; it is never cast.
```
AFTER
```
    (1) A CARD THAT IS NEITHER A LAND NOR A LAND-BACKED SPELL, at the highest printed mana value
        on the list - Emeria's Call, Agadeem's Awakening and Pelakka Predation are LAND-BACKED and
        do not go here. Send them only under rung (4), and only once your battlefield line prints
        four or more lands.
    (2) A SECOND COPY of a card already on this list whose first copy you are keeping.
```
Discard record this corpus: 4 windows. `146v125` s9 PUT: 3 (correct), `146v126` s6 PUT: 8
(**break, now re-keyed**), `146v126` s7 PUT: 1 (a duplicate Lolth - correct, and it is what new
rung 2 names), `146v130` s21 PUT: 8 (Silverquill Command, the highest cost left - correct).

## 146-F - the WHAT THIS BROKE line under that scan
Rewritten so it no longer cites the retired ordering as the lesson.

## 146-G - the DECIDING SITUATIONS entry for a hand of land-backed cards
BEFORE `-> those are SPELLS, not lands. Play a real land, hold Emeria's Call for seven mana, and leave Agadeem's Awakening alone - it is never cast`
AFTER  positive rule: play a real land first; take the `PLAY THIS AS A LAND` row on your OWN main
phase when the alternative is no land drop at all, or when the spell face is dead (the row prints
`{right now: returns NOTHING - your graveyard has no creature cards ...}` when Agadeem's is).
Cites the four takes; `146v125` s13 is the worked positive - five white sources, a hand of
`{2}{b}` / `{b}{w}` black spells, Agadeem, the Undercrypt taken and paid 3 life to enter untapped.

## 146-H - the hand-header rung
FINDING: the pregame hand header under-counts. Three deck146 pregame asks this corpus named a land
count that excluded a land-backed card in the same hand (`146v130` s1 "1 land" with Pelakka
Predation; `146v130` s2/s3 "1 land" with Agadeem's Awakening; `146v152` s1 "0 lands" with two).
Per the trust doctrine this guide does NOT teach doubt of the render: it teaches an ADDITION whose
inputs are two true surfaces (the header's number and the hand line's card names), and it carries
its own removal condition.
BEFORE `- The engine's hand header says "2 lands" -> it is a two-land hand. Use its count, not your own.`
AFTER  `- The engine's hand header says "2 lands" -> your land count is that number PLUS one for each Emeria's Call, Agadeem's Awakening and Pelakka Predation on the hand line. Everything else in that header is exact; add nothing else to it.`
The ENGINE half is the seat file's **E-1 (HIGH)** and is the fix that retires this edit.

## 146-I - CHECK 0 ("doesn't untap") over-brakes; two exemptions added
FINDING (wave-55 P1 re-adjudicated): 2 of 24 deck146 attack windows had every declared A-line
carrying `[doesn't untap during its controller's untap step]` with power below the opponent's
printed life, so P1 reads FAIL 2/2 - **but both were correct plays and the rule is what is wrong.**
`146v123` s12 (T7, 20 vs 14): the one A-line is Nadaar 3/3 **vigilance**, tagged, with
`[no creature they control can block this attacker]` and a header reading
`They have 0 untapped creatures able to block`. Vigilance means attacking never taps it, so the
tag costs nothing. `146v123` s21 (T9, 20 vs 11): three tagged A-lines, only Nadaar sent, Silencer
and Goblin correctly held under Bloodline Keeper. The game was won 21/-2 on T11.
BEFORE `Under that tag you declare an attacker ONLY when the power you are sending is at or above their printed life THIS turn. Otherwise ATTACK: none ...`
AFTER  adds, ahead of that bar: `(i)` an A-line whose tags also contain `vigilance` is never tapped
by attacking - send it free, always; `(ii)` when the header reads `They have 0 untapped creatures
able to block` AND an untapped creature is still left on your line after the declaration, send the
ones that get through. The bar then applies to everything else, unchanged.

---
## What did NOT change and why
- The HOLD block (wave-55 146-D) stays byte-identical: deck146's HOLD take rate went
  **7 of 80 (8.8%) -> 35 of 161 (21.7%)** and at N>=2 **0 of 12 -> 5 of 20**. It is working.
- The Soul Shatter one-card rule (wave-55 146-C) stays: 0 plans this corpus asserted it removes
  more than one, and lane C's `{right now: they sacrifice ONE of these 2, their choice ...}`
  clause rendered and was cast correctly at `146v152` s30.
- The mulligan colour-family floor stays: 2 colour-clause renders, 2 keeps.
