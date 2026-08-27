# deck130 (Budde's Ponza) - wave-51 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (wave-50 revision + boundary
pass, 49,602 chars). Eight edits by exact-string replacement on a copy; 49,602 -> 53,744 (+4,142 net:
+5,380 added, -1,238 deleted). Corpus: `matchups-20260827-155545`; seqs by short form.

## D51-1 - WALL CHECK re-keyed to the converter tag, with the arithmetic in emit form
**Finding.** 130v126 seq 20 (19 life): all four attacker tags read `(neither dies (blocking trigger:
they may gain 2, and if they do their converter takes 2 off you))` (lane F's clause); the PLAN
reasoned through the life gain and sent all four -> 19 to 11, opponent 16 to 24. seq 28 (10 life):
three Goblins into the same tag -> 10 to 4, opponent 24 to 30. Same matchup as wave 50's 21 -> 30
(D50-7's specimen): the paired-edit replay (#132) FAILED. Second corpus -> #102's second-failure
check: the contradiction was the ATTACK FLOOR's "send EVERY creature listed" one paragraph below,
which the pilot's plan followed. Fix: the wall check becomes the floor's FIRST case, keyed to the
tag literal, with the price written in the reply.
**After:** `Count the attackers you were about to send and double it ... Write it: "Wall: <n>
attackers x 2 = <2n> life; ATTACK: none"` + the two specimens; the floor's FIRST case now reads
"the WALL CHECK above"; DECIDING line added.

## D51-2 - rule #2 point 2: ability rows now print DIES/SURVIVES (lane E shipped) - the "print NO
result" paragraph was FALSE and is replaced
**Finding.** #128: `{right now: takes N damage - DIES/SURVIVES (toughness T)}` on 428 ability rows
across the six games (130v126 156, 130v123 76 ...), including the post-combat form `SURVIVES
(toughness 4, 3 more kills it)`. The pilot still fired at a SURVIVES row: 130v126 seq 49 (Spellbomb
at Overgrown Battlement `SURVIVES (toughness 4)` at 3 life, paid with a Talisman -> 2). And read it
right once: seq 22 Spellbomb at Perimeter Captain `DIES` after 2 combat damage.
**Before:** `An ABILITY'S rows ... print NO result: there you do the read yourself ...` + the wave-50
Fate Unraveler anecdote. **After:** `EVERY damage row prints its result ... "SURVIVES" is the answer
"not this row"` + the seq 49 and seq 22 specimens. DECIDING line replaced.

## D51-3 - the Talisman can be the last point (life floor re-keyed to the payment)
**Finding.** 130v126 seq 49 (3 -> 2), seq 82-83 (2 -> 1, Spark Spray at a 0/4 defender), seq 85 (1 ->
0: `Cast Spark Spray {leaves 0 of your 1 untapped mana source}`, receipt `Paid {r} for Spark Spray
with Talisman of Impulse` - the game ended 0 to 24 with the opponent never attacking). D50-12's
"CAUTION ... life 3 or less" line existed and failed three times in one game -> re-keyed to the row
strings (`{leaves 0 of your 1 ...}`, `Those sources, one per untapped card`) with a dictated
sentence, "Talisman pays: life <L> -> <L-1>". Render half is seat item H3.

## D51-4 - Starstorm (a)/(b): the dictated two-list sentence; one anecdote cut (#102 third failure)
**Finding.** 130v126 seq 40-42: at 3 life, `X = 4 {kills THEIRS: Wall of Omens x2, Overgrown
Battlement x2, Perimeter Captain; YOURS: Siege-Gang Commander, Goblin}` taken - (a) and (b) in one
cast (five defenders, own Commander); seq 65-67: X=4 at Pride Guardian + Battlement alone (b); seq
69-70 `Decline - do nothing` used correctly once. 130v123 seq 38-40: `Cast Card Normally` on a row
reading `even at X=4 NOTHING the OPPONENT controls dies`, then `X = 0` (the D50-4 exit was on the
previous screen). (b) is at its third corpus in the deck126 matchup; (a) its second.
**Before:** the (a) anecdote ("at 1 life against an opponent on 9 ... X=5 ... died with an empty
board"). **After:** `PLAN: Starstorm: THEIRS non-defenders <n>; YOURS names Rorix/Commander
<yes/no>; <cast / not this window>` + this corpus's two specimens. DECIDING line added.

## D51-5 - Hammer / Spellbomb / Spark Spray never touch a land
**Finding.** 130v123 seq 28-29: Hammer cast under "targeting Underground Sea #2 (destroy it)"; the
target menu (players + creatures) got `CHOICE: 3 (Siege-Gang Commander #1 [your battlefield])` -
the pilot's own Commander died (P14 FAIL 1). 130v125 seq 24-25: "Kill Dwarven Blastminer with Hammer
to destroy Plains" - the first coded line named the Blastminer, a later line named the opponent
(answer_replaced), 652 s. Wave 49's Spellbomb-into-own-Blastminer was the same confusion. Third
corpus of "damage spell named as land destruction".
**After:** paragraph before "A ROW THAT NAMES YOUR OWN PERMANENT" + DECIDING line.

## D51-6 - ATTACK FLOOR: Blastminer's "ATTACK: none - always" becomes an odds condition (#135)
**Finding.** 130v125 seq 34, 37, 41, 45, 48, 52, 55 (turns 14-26): `A1. Dwarven Blastminer (1/1) [no
creature they control can block this attacker]` -> `ATTACK: none` every time, opponent at 18 on 0-1
BASIC lands (his ability had no nonbasic target most turns) with no creature ever; the game ran to
turn 36. Seven windows x 1 = the seven turns a Commander later needed.
**Before:** `FIRST: if Dwarven Blastminer is the ONLY creature on that list, answer "ATTACK: none" -
always`. **After:** stays home while his {2}{R} has a nonbasic `[opponent's battlefield]` target this
turn or their line shows an untapped creature; with neither, send him. DECIDING line added.

## D51-7 - #1 RULE table: Lay Waste cycling below three lands with a cheaper land-kill in hand
**Finding.** 130v146 seq 6: `cycling with Lay Waste` at 2 Mountains with Molten Rain + Stone Rain in
hand (opponent 1 land); drew the third Mountain; Molten Rain on turn 6, Stone Rain turn 8, won.
Wave 50's break (v152 s16) was at four lands with 2 mana - the rule's case. The 8-land gate now
carries the one exception the corpus proved.

## D51-8 - DECIDING: Perimeter Captain line gains the Sanguine Bond clause
Ties D51-1 to the existing wall line so the two never disagree (#131 construction: Captain alone;
Captain + Bond; Bond alone -> same verdict at the attack step).

## Not edited, recorded
- 130v152 seq 32: Commander sent into a two-blocker tag (`Briarbridge Tracker (your attacker dies)
  ; Katilda (you kill it)`) and died to the Tracker; rule exists ("SIEGE-GANG COMMANDER never goes on
  such a tag"); one window, won. seq 29: a Goblin chumped a 2/3 at 16 life (rule: N above 5 -> none).
- 130v146 seq 55: Spellbomb at the face at 10 with Nadaar (3/3) on their line - the 6-or-less floor
  (P15 FAIL 1); seq 63: Goblin sacrificed at the face at 9. One game, won. Recorded.
- 130v146 seq 19: Spellbomb at the face at 18 with 0 creatures on their line (wave 50 v146 s7 the
  same shape). Second corpus; unpunished; recorded.
- 130v126 seq 37-38: two Goblins and 4 mana (one Talisman point) spent to kill Wall of Omens #3 at
  4 life; seq 74: Lay Waste at 8 opposing lands. Rule (b)-adjacent and the life floor; both won
  nothing; the game was decided at seq 20/28 (D51-1).
- Hammer return with a body castable (P16): 0 taken / 2 windows with lane E's `{spends 5 of your 5;
  Siege-Gang Commander needs 5}` clause rendered (130v152 seq 25, seq 30). The D50-1 paragraph
  stands untouched at its one restatement (#138).
- Mulligans: 130v146 seq 1 a 0-land seven shipped, the six (5 Mountains + Molten Rain + Stone Rain)
  kept, a Mountain bottomed (order followed); 130v126 seq 1 a ONE-land seven (Forgotten Cave, two
  Talismans, Spark Spray) kept under a coverage line naming Spark Spray - the guide's own rule, the
  hand acted on turn 4 (Talisman) and cast the Commander on turn 8; the loss came at seq 20/28, not
  from the keep. 130v162 seq 1: six Mountains + Molten Rain kept - not "all lands"; won turn 16. No
  floor text, no chain, no owner question at this seat.
- Cleanup `discard` asks at deck130: 0 for the second corpus running -> the boundary-pass B1 order is
  UNTESTABLE-AT-THIS-SEAT (#132); #143's metric has no denominator here.
