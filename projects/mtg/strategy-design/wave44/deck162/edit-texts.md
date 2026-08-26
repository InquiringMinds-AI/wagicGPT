# deck162 — wave-44 edit texts (before -> after, each with its finding)

Baseline = the LIVE guide `projects/mtg/bin/Res/ai/baka/deck162_strategy.txt` (wave-43 S5/S6
already applied: the ATTACKING section was cut to the Fate Unraveler stop + S5 + the lethal
pointer, and the D3 Master-of-the-Feast gate was DELETED). 4 edits, 0 deletions.
Corpus: 6 games (4-2), 141 decisions, 2 fallbacks.

**Headline: S6's deletion is FALSIFIED. It cost a card and a game.**

---

## EDIT 162-A (HIGH) — restore the attack-side POSITIVE branch, and gate the collapsed tag

FINDING 1 (S6 removed the positive branch). deck162 vs deck152, attackers seq 19, turn 11,
opponent at 14. Both attackers were priced identically:
```
A1. Fate Unraveler #1 (3/4) [their untapped blockers: Intrepid Adversary (3/1)
    (you kill it, your attacker lives (lifelink: they gain 3))]
A2. Fate Unraveler #2 (3/4) [same]
```
Answer: `ATTACK: none`, with 300 words of reasoning containing "If I attack with Fate Unravelers,
I might trade them for Intrepid Adversary... If I lose both, I have 0 punishers." The tag had
already said the attacker LIVES (3 power vs 4 toughness); a trade was not on the table. Either
they decline and take 6, or they block and hand deck162 a free kill on their lifelink lord.
After S6 the ATTACKING section contains ONLY prohibitions - there is no line anywhere in the guide
that says a "(you kill it, your attacker lives)" attack tag means SEND. The deck attacked 2 times
in 6 attack windows this corpus.
CLASSIFICATION: STRATEGY (the surface was true and complete; the guide had deleted its own
positive rung). No cost this game - deck162 won it on the Puzzle Box line - but the pattern is
free damage declined.

FINDING 2 (S6 deleted the D3 Master-of-the-Feast gate; the deletion cost the vs123 game).
deck162 vs deck123, attackers seq 13, turn 12. Sole attacker:
```
A1. Master of the Feast (5/5) [flying, doesn't untap during its controller's untap step]
    [their untapped blockers: they have 6 untapped creatures that could block this one,
     biggest Bloodline Keeper #1 (3/3) (you kill it, your attacker lives)]
```
Answer: `ATTACK: A1`. Game log: "Master of the Feast dealt 1 damage to Vampire / 2 / 2 -
Your Master of the Feast died" - a three-Vampire gang block (6 damage vs 5 toughness). deck162
lost the game at turn 13 on life -21.
The parenthesis is TRUE for the biggest blocker in isolation and the footnote does say the numbers
are "before gang-blocks", but the collapsed six-blocker summary hands the model a
"you kill it, your attacker lives" verdict on a board where the attack is suicide. This is both a
render item (seat file E-2, HIGH) and the exact hole S6 opened by deleting the D3 gate.
The "[doesn't untap during its controller's untap step]" tag was verified NOT to be a false
surface: deck123 controls Intruder Alarm ("Creatures don't untap during their controllers' untap
steps"), so the tag is correct. Master of the Feast's own primitive (mtg.txt) is
`abilities=flying`, `auto=@each my upkeep:draw:1 opponent`, 5/5 - no non-untap clause of its own.

BEFORE (the whole ATTACKING section, 13 lines):
```
=== ATTACKING ===
Never send Fate Unraveler into a "their untapped blockers" tag that reads "your attacker dies,
their blocker lives". That tag is the engine's own computed result - do not argue with it. Fate
Unraveler is a punisher first: dead, it stops the draw damage that wins you the game. If the
lethal check has not cleared and that tag is on its line, the answer is ATTACK: none.
The same answer covers "(both die)". Fate Unraveler trading itself for one creature is still
a punisher that stops dealing draw damage, and the creature it kills is one they replace. The
tag also does not tell you what the BLOCKER gains: a lifelink blocker hands them life on top
of the trade. Below the lethal check, "(both die)" on Fate Unraveler's line reads exactly like
"your attacker dies": ATTACK: none.
The one time Fate Unraveler goes: the printed opponent life is at or below the total power of
every attacker you are sending including it - then run the lethal check below, swing with
everything and end it.
```
AFTER:
```
=== ATTACKING ===
Read each attacker's own "their untapped blockers" tag and answer from it. The tag is the
engine's computed result - do not argue with it, and do not answer ATTACK: none out of a
general wish to keep your board.
SEND IT when every outcome on that creature's tag reads "(you kill it, your attacker lives)".
That line says your creature SURVIVES and theirs does not: there is no trade to fear and no
punisher to lose. Either they decline the block and take the damage, or they block and hand
you a free kill. "I might trade them away" is FALSE against that tag - the tag already told
you the attacker lives. This is the commonest way this deck throws away free damage, and it
costs you the turns your punishers are buying.
DO NOT SEND Fate Unraveler into a tag that reads "your attacker dies, their blocker lives",
and do not send it into "(both die)" either: Fate Unraveler is a punisher first, and dead it
stops the draw damage that wins you the game, for a creature they simply replace. The tag also
does not tell you what the BLOCKER gains - a lifelink blocker hands them life on top of the
trade. Below the lethal check, both of those lines mean ATTACK: none for Fate Unraveler.
MASTER OF THE FEAST AND THE COLLAPSED TAG. When a creature's tag does NOT list their blockers
one by one but instead reads "they have N untapped creatures that could block this one, biggest
X (you kill it, your attacker lives)", that parenthesis prices ONE blocker only - the biggest -
and the footnote under it says the numbers are computed "before gang-blocks". N is how many
creatures can block that attacker, and any two of them may block it TOGETHER. So read N, not
the parenthesis:
  N = 1  -> the parenthesis is the whole story; use it.
  N >= 2 -> a gang block is available and your attacker's toughness is measured against the
            SUM of their bodies, not against the biggest one. Unless the lethal check has
            cleared, the answer for that creature is ATTACK: none.
Master of the Feast is a 5/5 flier and your fastest clock, but it does not untap while Intruder
Alarm or a similar effect is out (its battlefield line will say so), and three 2/2 tokens gang
it to death for nothing. Do not swing it into an N >= 2 board.
The one time everything goes: the printed opponent life is at or below the total power of every
attacker you are sending - then run the lethal check below, swing with everything, and end it.
```

---

## EDIT 162-B (MED) — name the exact shape that breaks the Rule-2 brake

FINDING. Rule-2 compliance measured 10/13 across the corpus. All three violations were the same
shape - a Howling Mine already on the battlefield, NO punisher, and a second draw engine taken:
  deck162 vs deck152 seq 8  -> Cast Dictate of Kruphix (no punisher out)
  deck162 vs deck123 seq 9  -> Cast Howling Mine #2 (Mine already out, no punisher)
  deck162 vs deck123 seq 18 -> Cast Dictate of Kruphix (Mine already out, no punisher)
The two vs123 breaks are in the game deck162 lost at -21 to a Bloodline Keeper / Thraben Doomsayer
token engine - i.e. deck162 spent two turns handing extra cards to the deck that was assembling
a board, with nothing on its own battlefield converting those cards into damage.
CLASSIFICATION: STRATEGY.

BEFORE:
```
The thought "drawing more cards is the priority, I need to find my win condition" is ALWAYS FALSE
for this deck - a card you hand the opponent with no punisher out is a card that kills you.
```
AFTER — the same two lines, then:
```
THE EXACT SHAPE THAT LOSES GAMES: a Howling Mine is already on your battlefield line, no
punisher is, and the ask offers a SECOND Mine, a Dictate of Kruphix or a Puzzle Box. Every one
of those is an extra card per turn for a deck that is assembling something. Take the wall, the
Liliana's Caress or "Cast nothing right now" instead, every single time, until a punisher name
appears on YOUR battlefield line. Count the names on the line - do not count the ones in hand.
```

---

## EDIT 162-C (MED) — the MULLIGAN section had NO bottoming rule

FINDING. deck162 took the corpus's second mulligan (vs deck125, seq 1: 0 lands, 7 spells -> Mulligan;
seq 2: 3 lands, 4 spells -> Keep) and was then asked to bottom one card (`bottom` record, seq 3).
It bottomed SHIELD SPHERE, out of a hand of Shield Sphere / 2x Underground Sea / Master of the
Feast / Ob Nixilis / Liliana's Caress / Dismal Backwater. Shield Sphere costs {0} and Rule 3 says
"Take it every single time it is listed, whatever else you are doing". The guide's MULLIGAN
section stops at the keep and says nothing about the bottoming ask, so the two rules never met.
CLASSIFICATION: STRATEGY (missing teach). This is a bottoming rule, NOT a keep-floor change -
the floor is untouched (see the seat file's fair-hand section).

BEFORE (end of the MULLIGAN section):
```
If the prompt says "having already taken 1 mulligan", this is your LAST look: answer "Keep this
hand". "I will keep whatever I draw next" is how you end up with a 4-card hand - keep THIS one.
```
AFTER — the same two lines, then:
```
WHEN YOU KEEP AFTER A MULLIGAN THE GAME ASKS WHICH CARDS TO BOTTOM. Never bottom a land, and
never bottom SHIELD SPHERE - it costs ZERO mana, so it is free defence you can deploy on any
turn no matter what your lands are doing, and it is the cheapest card in the deck to keep.
Never bottom your only punisher either. Bottom your most EXPENSIVE spell first: Forced Fruition,
Peer into the Abyss, Teferi's Puzzle Box, a second copy of a draw engine - in that order.
```

---

## EDIT 162-D (LOW) — two new DECIDING SITUATIONS rows plus the Puzzle-Box-vs-punisher row

FINDING. Rule-1 (punisher first) measured 13/17. Two of the four misses took Teferi's Puzzle Box
over a punisher in the same list (vs deck152 seq 18, vs deck126 seq 12); one is the seq-37
`retracted_choice` fallback (the engine decided, not the model); one is Peer into the Abyss with a
punisher already out, which cast-order line 2 permits. So the live residue is the Puzzle Box.

BEFORE:
```
- Two spells are both payable and one is a punisher: the punisher goes first, then spend the rest.
```
AFTER:
```
- Two spells are both payable and one is a punisher: the punisher goes first, then spend the rest.
- Teferi's Puzzle Box, Forced Fruition or a Dictate is in the SAME list as a punisher: the
  punisher is number 1 in the cast order and the Box is number 5. Cast the punisher this ask.
  A Puzzle Box with nothing to convert its cards is a gift, however big the number looks.
- An attacker's tag reads only "(you kill it, your attacker lives)": send that attacker.
- An attacker's tag reads "they have N untapped creatures that could block this one" with N of
  2 or more: ATTACK: none for that creature unless the lethal check has cleared.
```

---

## NOT EDITED, and why
- RULE 3 walls / "Cast Shield Sphere {0}" is free: 5/5 CONFIRMED.
- RULE 4 blocking: 10/10 block windows answered correctly, including the two judgement calls -
  vs deck130 seq 27 spent Master of the Feast on a "(both die)" against Rorix Bladewing (6/5
  flying, their best creature and 6 of the 10 incoming) rather than on a free kill against a 1/1,
  which is right; vs deck152 seq 17 assigned one wall per attacker with no double-stacking, which
  is rule 4.2 exactly. Wall-blocks 10/10 - the wave-43 D5 result holds under fair hands.
- The keep floor ("KEEP any 7-card hand with two or more lands") is NOT touched.
- S5 (the "(both die)" half of the Fate Unraveler stop) is KEPT IN SUBSTANCE - it had no window
  this corpus and nothing falsifies it. Note it is not kept VERBATIM: EDIT 162-A rewrites the
  section it lives in, condensing S5's two prohibitions ("your attacker dies" and "(both die)")
  into one sentence pair so the new positive branch can sit above them. Every clause of S5 is
  still present, including the lifelink-gain note and the below-the-lethal-check scope.
