# deck162 - wave-47 edit (1), before -> after, with its finding

Baseline: the LIVE guide `bin/Res/ai/baka/deck162_strategy.txt` (27,086 bytes; byte-identical to
`wave46/deck162/strategy.txt` - the boundary pass rejected every candidate here). After: 28,033
bytes (**+947**). 0 deletions.

deck162 went **4-1** in its counted games this corpus (its sixth, vs deck123, is the orchestrator-
killed infinite-loop game and is excluded per the corpus note). One edit is all its record pays for.

---

## 162-A (HIGH) - RULE 1 has no tiebreak between two punishers, and the bodiless one lost the only game
**Finding:** `162 vs146` seq **6** (turn 7, **life 16** vs 20). Own battlefield line:
`(4 permanents listed, of which 0 are creatures, 0 of them able to attack right now)`. Their line:
`(7 permanents listed, of which 3 are creatures, 3 of them without a restriction against attacking)`.
The cast menu offered exactly two things, and both are punishers, so RULE 1 is satisfied either way:
```
1. Cast Underworld Dreams {b}{b}{b}    (type=Enchantment - verified)
2. Cast Fate Unraveler {3}{b} (3/4)    (type=Enchantment Creature, 3/4 - verified)
```
It took Underworld Dreams. The seat then **never produced a single `blockers` record in the whole
game** - it had nothing to block with - and went 16 -> 10 -> 7 -> 3 -> **-1** at turn 12. It cast
Fate Unraveler at seq 11, at **3 life**, four turns and thirteen points too late. This is deck162's
only loss this corpus, and RULE 1 - the deck's strongest rule, and the one at its best-ever
compliance - was obeyed at every step of it.
`Fate Unraveler`: `auto=@drawfoeof(player):damage:1 opponent`, `type=Enchantment Creature`,
`power=3 toughness=4`. `Master of the Feast`: `abilities=flying`, `type=Enchantment Creature`,
`power=5 toughness=5`, `auto=@each my upkeep:draw:1 opponent`. `Underworld Dreams`:
`auto=@drawfoeof(player):damage:1 opponent`, `type=Enchantment` - no body. All verified.
CLASSIFICATION: **STRATEGY** (both battlefield lines printed the creature counts the rule needs).
- **before:** RULE 1 ends at `ONE named exception: when your life is 8 or less AND the opponent's
  battlefield shows a creature, "Cast Fog Bank {1}{u}" goes ahead of the punisher for that one
  ask ...`
- **after:** + `WHICH PUNISHER, WHEN TWO ARE OFFERED: TAKE THE ONE THAT IS ALSO A BODY.` naming
  Fate Unraveler (3/4) and Master of the Feast (5/5 flying) as Enchantment Creatures, keying the
  test to the two printed counts (`of which N are creatures` on your line, `of which M are
  creatures, M of them without a restriction against attacking` on theirs), firing only when your
  N is 0 and their M is 1 or more, and carrying the WHAT THIS COST record.
- **Deliberately narrow.** It does not touch the punisher-vs-everything-else ordering (20/21 last
  corpus, and this corpus every punisher-vs-draw-engine ask went to the punisher). It is a
  tiebreak INSIDE the first rung, gated on two counts the prompt already prints.

---

## NOT edited, and why
- **The S6 send rung: 2 held of 2, and they are the wave-46 catastrophe repeating and being
  refused.** `162 vs126` seq **12** and **16**: `Fate Unraveler (3/4) [their untapped blockers:
  Perimeter Captain #1 (0/4) (neither dies (blocking trigger: they may gain 4, and if they do
  their converter takes 4 off you)); ...]`, with `LIFE-TO-DAMAGE CONVERTER on the battlefield:
  theirs - Sanguine Bond` printed. **`ATTACK: none` both times.** Wave 46's same matchup was a
  19-to-0 turn; this one was won **21 to 0** at turn 12. Wave-46 prediction **P10: PASS 2/2**, and
  wave-46 edit 162-B is doing exactly what it was written for. Nothing to add.
- **Rule 2 (draw engines behind a punisher): 39/42, scored on the rule's own text** (a FIRST
  Howling Mine with no punisher is licensed by the rule, so `vs125` seq 4, `vs152` seq 4,
  `vs123` seq 5 and `vs130` seq 6 are not breaks). The **3 real breaks** are exactly the shape the
  guide names as "THE EXACT SHAPE THAT LOSES GAMES" - a Mine already on the battlefield line, no
  punisher, and a second engine taken: `vs125` seq **6** (Master of the Feast), `vs152` seq **10**
  (a second Howling Mine), `vs123` seq **7** (Dictate of Kruphix). **All three games were won or
  are the excluded loop game.** Counter-evidence in the same corpus: `vs130` declined `Teferi's
  Puzzle Box` at seq 11/12/14/15 with no punisher out, cast `Fate Unraveler` at seq 17, and cast
  the Puzzle Box at seq 20 - the rule executed over five consecutive windows; `vs126` cast
  Underworld Dreams at seq 7 before any engine. Violated-and-unpunished at n=3 across two corpora:
  guide unchanged (amendments 45/64), carried as a wave-48 prediction.
- **Zero-power wall blocks: 5/5.** Every `[deals 0 - this block kills nothing, but it STOPS all N
  damage from reaching you THIS COMBAT]` row was used, and at `vs152` seq 13 the walls were chosen
  over the `both die` Fate Unraveler row. R12's `THIS COMBAT` scope is not being over-read.
- **Land drops 10/10** (was 35/39). Wave-46 edit 162-A holds.
- **The MULLIGAN section, the GANG BLOCK rung and the collapsed-tag rung remain UNTESTED**
  (1 mulligan, no chain; 0 live `GANG BLOCK:` clauses; 0 collapsed attacker tags). Third corpus for
  the last two. Not deleted (amendment 21).
