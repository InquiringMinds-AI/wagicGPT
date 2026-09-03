# deck162 - wave-56 guide edits (before -> after)

Base: live `bin/Res/ai/baka/deck162_strategy.txt` (45,365 B). Result:
`wave56/deck162/strategy.txt`, 46,607 B (+1,242). Two edits. deck162 went 5-1 for a second corpus
and is the pool's best-diagnosed deck; both edits are carve-outs that stop an existing rule from
flagging correct play, not new rules.

## 162-A - the K-of-0 brake gets a duplicate-permanent exemption

FINDING (wave-55 P6 re-adjudicated). P6's key - "own-turn `Cast nothing right now` on a menu whose
only non-pass row prints no `{feeds:}` clause" - fired 3 times, and **all three declines were
correct**, so the key is over-braking rather than the pilot mis-declining:
- `162v125` s38 and s39, T24: the only non-pass row is
  `Cast Ob Nixilis, the Hate-Twisted {3}{b}{b} [legendary: you already control one ...]` with an
  Ob Nixilis already on the battlefield line. Casting it kills one of the two.
- `162v146` s4, T4: rows are `Cast Fog Bank {1}{u} (0/2)` and `Cast Liliana's Caress {1}{b}` with
  no converter out; both declined, and the game was won 6/0.
`[legendary:` rendered 21 rows at my seats with 0 takes and `[second copy:` 18 rows with 9 takes -
the discrimination is live in the render; the guide just needed to say the brake is not what
decides those windows.

AFTER - a new paragraph immediately above THE TEST IS ONE LITERAL naming the two exempt row tags
(`[legendary: you already control one ...]`, `[second copy: you already control ...]` at
`converters on your battlefield: 0`) and recording the three windows as correct.

## 162-B - Forced Fruition is a FEEDER, not a converter

FINDING (wave-55 P7). 3 takes of a `{feeds:}` row at `converters on your battlefield: 0` this
corpus. Two are sanctioned (`162v123` s4 the first Howling Mine with two Underworld Dreams on the
hand line - P8's carve-out, still 1/1; `162v130` s6 Master of the Feast with Fate Unraveler AND Ob
Nixilis in hand - the control, and it must stay non-zero). **One is a real break:** `162v125` s15,
T14, `Cast Dictate of Kruphix {1}{u}{u}` at K of 0 with a hand of Forced Fruition + two Islands +
the Dictate - no Underworld Dreams, no Fate Unraveler, no Ob Nixilis. The reply treated Forced
Fruition as if it discharged the brake.

AFTER - inserted above "The row costs you nothing the brake would have let you take":
```
FORCED FRUITION IS A FEEDER, NOT A CONVERTER. It makes THEM draw four per spell; it deals no
damage by itself. Holding it in hand does not release the brake - only Underworld Dreams, Fate
Unraveler, Ob Nixilis or a Liliana's Caress on your battlefield line converts a card into life
loss. WHAT THIS BROKE: at `vs125` seq 15 ...
```

---
## What did NOT change and why
- **The HOLD block is working and needs nothing.** deck162's HOLD renders went 174 -> 65 and its
  declined-note windows at N>=2 went **65 -> 0**: the wave-55 headline-1 concentration
  (`162v125` answering the same two-row Dictate menu eight to ten times a turn for eleven turns)
  is GONE. Takes 4 of 65.
- **P8 (162-C, the Caress fit clause) stays PASS.** Three windows offered a `{feeds:}` row beside
  `Cast Liliana's Caress` with no room for both (`162v123` s14/s18/s19); the seat took Forced
  Fruition at 2 converters and Howling Mine at 3, which is the guide's own ordering.
- The mulligan floor stays: 6 keeps, 0 mulligans (wave 55 had one at three lands). One colour-family
  clause rendered at 5 lands and was kept.
