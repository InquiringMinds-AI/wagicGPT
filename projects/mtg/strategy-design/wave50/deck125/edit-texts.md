# deck125 - wave-50 edits (each: finding -> before -> after)

Numbering continues from wave 49 (W13-W19). Live guide = `bin/Res/ai/baka/deck125_strategy.txt`
at ce3c7a90c; the full revised guide is `wave50/deck125/strategy.txt`. Guide size 40.0 KB -> 44.6
KB (W22 rewrites the gate in place; W21/W24/W25 are the additions; two history sentences cut).

## W20 - EMRAKUL-OFF re-keyed to the printed `{effect:}` text (W46 FAIL, `vs126` seq 42)
Finding: `Cast Emrakul` taken with `Exquisite Blood {4}{b} [enchantment] {effect: "Whenever an
opponent loses life, you gain that much life."}` on the opponent line, the reply's own plan
carrying the prohibition. #102 (rule restated and broken in one reply) + #118 (re-key to a printed
token, no restatement).
BEFORE: "THE ONE BOARD WHERE THAT LINE IS OFF, and it outranks it: the opponent battlefield line
shows Sanguine Bond or Exquisite Blood, or the game log shows the opponent has cast Tribute to
Hunger or Devour Flesh ("sacrifices a creature ... gains life equal to its toughness")."
AFTER: "THE ONE BOARD WHERE THAT LINE IS OFF, and it outranks it. Read the OPPONENT battlefield
line before you take the Emrakul row, and read the game log. If that line prints an [enchantment]
whose {effect:} reads "Whenever an opponent loses life, you gain that much life" (Exquisite Blood)
or "Whenever you gain life, ... loses that much life" (Sanguine Bond), or the log prints "Opponent
used: Gain life equal to its toughness with Tribute to Hunger" or a Devour Flesh, ..." + the seq 42
instance in one sentence ("You cast it again at 33 life with "Exquisite Blood {4}{b}
[enchantment]" printed on their line and your own plan line saying not to."). DECIDING SITUATIONS
bullet re-keyed the same way. Also: the extra-turn sentence now cites the log string
("Turn 37 - YOUR turn" after "Turn 36") instead of asserting it.

## W21 - NEW: a sweeper with Emrakul out reads its own `1 of yours` tail (W49 FAIL, `vs126` seq 44)
Finding: Supreme Verdict off `{right now: destroys 10 of their creatures (all of them carry a
restriction against attacking), 1 of yours}` on Emrakul's extra turn - killed the Emrakul, ten
walls, nothing that could attack. The guide's sweeper opener said a wipe "only ever costs the
opponent" - false once Emrakul is out; STRATEGY (surface read, decision wrong).
BEFORE (#1 tail): "Once an Emrakul of yours IS on the battlefield ... Nothing else in this guide
changes."
AFTER: "... And READ THE SWEEPER ROW'S LAST NUMBER: "Cast Supreme Verdict ... {right now: destroys
10 of their creatures (...), 1 of yours}" - that "1 of yours" is your Emrakul. Supreme Verdict and
Final Judgment kill every creature, yours included; with an Emrakul out, any sweeper row whose tail
reads "1 of yours" is answered "Cast nothing right now". You cast a Verdict off exactly that row
on Emrakul's extra turn ..."
BEFORE (sweeper opener): "Your battlefield has no creatures, so a board wipe only ever costs the
opponent."
AFTER: "Your battlefield has no creatures while no Emrakul of yours is out, so a board wipe then
only costs the opponent (with an Emrakul out, read #1: the row's "1 of yours" tail is the
Emrakul)." + a DECIDING SITUATIONS bullet ("An Emrakul of yours is on the battlefield: attack with
it every window; and a Supreme Verdict or Final Judgment row whose tail reads "1 of yours" is
answered "Cast nothing right now"").

## W22 - THE TAP-OUT GATE re-derived: N is the printed cost of the cheapest counter that counts
Finding (W47): seventh corpus of the fixed "leaves 5" gate failing (2/5 casts) and the second in
which the breaks cost nothing: `vs162` seq 20 held an Essence Scatter against a deck that cast no
creature (Dreams x2, Dictate, Howling Mine, then Peer into the Abyss) - the Scatter could stop
nothing; `vs123` seq 29 cast at `leaves 2` holding Scatter x2 + Gavel - 2 pays a Scatter. The gate
was over-strict, not under-obeyed (#102: a rule failing seven corpora is re-derived, not restated
an eighth time). The re-derived gate keeps the wave-49 loss illegal (taps-out with a Scatter
against Tovolar's creature deck) and makes both this corpus's casts legal. Elixir activation
(W51: seq 8 `vs123` 2 -> 0 with Scatter + Gavel at 20 life; seq 27 `vs126` 6 -> 4 with Gavel)
priced the same way - both stay breaks under it.
BEFORE: "Look at your hand for ANY of Essence Scatter, Cancel, Dream Fracture or Fall of the
Gavel - all four, not the three cheap ones. ... If any counterspell at all is in your hand, take
the row only when its N is 5 or more. Five, because Fall of the Gavel costs {3}{U}{W} and it is
the counter you are most often holding alone." + the "leaves 4 or less ... six corpora running"
bullet.
AFTER: "THE GATE IS A PRICE, AND THE PRICE IS PRINTED IN YOUR HAND. Your hand line prints every
counterspell with its cost: Essence Scatter {1}{u} is 2 mana, Cancel {1}{u}{u} and Dream Fracture
{1}{u}{u} are 3, Fall of the Gavel {3}{u}{w} is 5. ... Take the row only when N is AT LEAST the
cost of the cheapest counter in your hand THAT CAN HIT WHAT THIS OPPONENT CASTS: - Cancel, Dream
Fracture and Fall of the Gavel counter anything ... N must cover the cheapest of them - 3, or 5
when the Gavel is the only one. - Essence Scatter counters CREATURE SPELLS ONLY. It counts ... when
the opponent's battlefield line or the game log shows they cast creatures. Against an opponent
whose line and log show only enchantments, artifacts and sorceries (...) a Scatter in hand is not
a reason to hold mana ... - No counter in your hand that counts: the gate is open at any N." The
bullet re-worded to "A STAFF ROW WHOSE N IS BELOW THE PRICE OF A COUNTER THAT COUNTS IS ANSWERED
"Cast nothing right now"". Plan line (three places) -> "Keep the counter in my hand payable;
Staff of Nin only from a row whose leaves N covers that counter's cost; ping their face every
turn." Revelation and Elixir gate sentences re-pointed at "the price of a counter that counts";
Elixir section cites the turn-2 activation. DECIDING SITUATIONS: the Staff bullet, the Elixir
bullet and the generic "leaves 4 or less" bullet re-keyed; the duplicate Staff bullet removed.
#131-style self-check: a hand of {Scatter} vs a creature deck at leaves 2 -> cast; the same hand
vs an enchantment deck at leaves 0 -> cast; {Gavel} at leaves 4 -> hold; {Cancel, Gavel} at
leaves 3 -> cast; no counter -> cast. No two sentences disagree on any of them.

## W23 - the exactly-1 sweeper branch for a 0/1-power creature (W49, `vs146` seq 46; `vs152` seq 13)
Finding: Verdict at a lone Spirit 1/1 token with two Paths in hand (33 life) - the sweeper section
said "exactly 1 and Path in list: take the Path" while the Path section forbids a 1-power token: a
contradiction the executor resolved by casting the Verdict. Separately, Verdict at a lone 3/1
flier without a Path (seq 13, 17 life) - the existing power-4 branch already covers it; cited.
BEFORE: "- exactly 1 creature, and Path to Exile is in your list: take the Path and keep the
sweeper. ... - exactly 1 creature, and that creature is tagged [defender] or shows printed power 0:
cast NOTHING."
AFTER: the nothing-branch first and widened - "exactly 1 creature, and that creature is tagged
[defender], or shows printed power 0 or 1 with none of the engine text the Path section names:
cast NOTHING - no sweeper, no Path" (+ the Spirit instance); the Path branch and the no-Path branch
now read "exactly 1 creature with printed power 2 or more". Sweeper row strings updated to the
emitter's actual wording (`(3 without a restriction against attacking)`, `(all of them carry a
restriction against attacking)`, `(it carries a restriction against attacking)`); the old
`(0 able to attack)` quote removed (it no longer prints - trust doctrine: cite what IS printed).
DECIDING SITUATIONS exactly-1 bullet re-worded to match.

## W24 - Lightmine: `[tapped]` creatures are counted (W49, `vs146` seq 61)
Finding: `they control 3 creatures able to attack - deals 3 to each if all 3 attack` declined at
25 life because "Barrowin is tapped, Goblin is tapped, Spider is tapped". The row and header had
counted them. One sentence each in the sweeper header paragraph and the Lightmine section:
AFTER: "A creature tagged [tapped] is still counted: tapped is not a restriction, and it untaps."
/ "And when the row reads 3, CAST IT: "[tapped]" on their creatures is not a reason to wait - the
row already counted them, and they untap." Lightmine row strings quoted as printed.

## W25 - NEW section: CLEANUP DISCARD (lane W's `discard` ask; 9 asks this corpus)
Finding: no guide text for the new ask; `vs123` seq 76 sent a Cancel from a hand holding a
Lightmine Field at 44 life vs an empty board; seq 52 sent two Scatters keeping Emrakul at 34 life
on 16 lands (fine - Emrakul castable next turn). Order written to match the bottoming order's
logic (lands + answers kept), with the 13-land Emrakul exception the seq 52 case shows.
AFTER: the section as written in strategy.txt + a one-line DECIDING SITUATIONS bullet.

## Recorded, NOT edited
- Path: 0 breaks (W50 PASS both halves - two Path-at-<=12 windows, both right). Sentence updated
  to cite this corpus's correct takes instead of the old count.
- Revelation: 0/16 at X=1/2, X=3 6/6 - counts updated in place.
- Counter-on-sight list: Peer into the Abyss added to the draw-engine line with the `vs162`
  instance (it is a sorcery that killed the seat from 14 under two Dreams; Katilda added to the
  `{T}:` examples from `vs152` seq 6).
- Cut: "You have broken it in six corpora running. This one: three of six Staff casts ..." (W22
  supersedes) and the "IT ALSO OUTRANKS THE TAP-OUT GATE ... a gate asking for 5" wording (now
  "a gate could never pass it").
