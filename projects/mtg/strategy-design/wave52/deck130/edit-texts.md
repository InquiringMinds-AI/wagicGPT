# deck130 (Budde's Ponza) - wave-52 edit texts

Base: live guide `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (53,744 bytes). Revision:
`wave52/deck130/strategy.txt` (58,875 bytes; +5.1 KB net after three trims).
Corpus: `matchups-20260901-223430`, deck130 seat logs (6 games, 381 decisions, 3 fallbacks -
all three `plan_choice_conflict`, all three recovered). 3 wins / 3 losses.
Every edit names the seq that paid for it.

---

## D52-1 - the face-damage floor, six breaks, five of them in the game it lost
**Finding.** Damage aimed at "The opponent (player, life N)" this corpus: 10 casts.
Legal by the guide (N <= 6, or the LETHAL-THIS-TURN chain): 4 - v123 s54/s56/s57 (8 -> 5 -> 3 ->
1 then attack for the win) and v126 s27 (at 2 life, lethal).
**Breaks: 6** - v123 s11 (t4, opp 19), v125 s16 (t12, opp 20), v125 s36 (t20, opp **23**),
v125 s47 (t24, opp 19), v125 s57 (t32, opp 16), v125 s83 (t50, opp 21).
The CROSS-CHECK latch fired in that same game at turn 14 (opponent 20 -> 23 between two windows)
and was then broken four times over thirty-six turns. Final: 25 to -5 on turn 69.
EDIT (rule #2 point 4): the row's own printed number made the gate, plus the count and the
verdict "an empty opposing board is a reason to HOLD the damage".
EDIT (CROSS-CHECK): re-titled "AND IT IS A ONE-WAY LATCH", with the four post-latch casts named.

## D52-2 - land destruction past four opposing lands: 8 breaks / 19 casts
**Finding.** Casts against a board of five or more lands: v126 s18 (6), v146 s38 (5, and at
**5 life** - also a land-plan life-floor break), v125 s61 (9), s70 (9), s75 (10), s95 (12+),
s97 (12+), s100 (12+). Every one of the six deck125 casts is in the 69-turn loss. The gate text
existed; nothing told the pilot HOW to count a battlefield line whose lands are printed as
collapsed handles (`Island #1-#4 x4; Plains #1-#3 x3; Seachrome Coast` = nine).
EDIT (HOW THE DECK WINS): "COUNT THE LAND ROWS ON THEIR BATTLEFIELD LINE BEFORE EVERY ONE OF
THESE CASTS. Collapsed handles count in full ... At FIVE or more the spell is dead for the rest
of the game - it does not become live again because you have nothing else to do with the mana."

## D52-3 - the GANG BLOCK clause, scoped too narrowly, lost v146
**Finding (the deciding misplay of that game).** v146 s35, blockers, 8 life:
`Unblocked, these attackers deal up to 3 - you would be at 5 - NOT lethal`;
`A1. Silverquill Silencer (3/2) deals 3 [GANG BLOCK: any 2 of yours together deal 2, enough to
kill this attacker; each B-line result below is a LONE blocker only]`;
`B1-B3. Goblin #1-#3 (1/1) - may block A1 (your blocker dies, attacker lives) x3`.
Reply: `BLOCKS: none`. 8 -> 5; the Silencer hit again on turn 13 (5 -> ... ), Rorix came down at
2 life on turn 14 and the game ended -1 to 17.
The COMBAT prose already said "a DRAW PUNISHER ... **or the only creature hitting you**"; the
DECIDING SITUATIONS bullet the pilot actually reads said DRAW PUNISHERS only. Seam.
BEFORE:
```
- The blockers header prints "GANG BLOCK: your N biggest together deal X, enough to kill this
  attacker" on a creature named on the "DRAW PUNISHERS" line, and your life minus its "deals
  N" is 5 or less -> all N blockers on it.
```
AFTER: adds "OR it is the only creature attacking you", quotes the `any N of yours` form the
emitter actually printed, and carries the s35 numbers.

## D52-4 - Dwarven Blastminer's attack floor should read his tag, not their creature count
**Finding.** v162 s11 (t6), s22 (t10), s68 (t14): Blastminer alone on the list, and his row read
`[their untapped blockers: Shield Sphere (0/6) (neither dies); Fog Bank (0/2) (neither dies (no
combat damage is dealt either way - prevented))]`. He was SENT all three times - correct in fact
(1 free damage a turn, no risk), but the guide's letter said hold, because "their line shows an
untapped creature". Primitives verified: Shield Sphere `abilities=defender` 0/6; Fog Bank
`abilities=flying,defender` 0/2 with all combat damage prevented.
EDIT (ATTACK FLOOR, SECOND): "READ HIS ROW'S OWN TAG, do not count their creatures: if every
blocker in `[their untapped blockers: ...]` is priced `(neither dies)` ... SEND HIM. He stays
home only when a blocker's parenthesis says he dies."

## D52-5 - Starstorm rule (b): one defender-only sweep, and the carve-out that makes it right
**Finding.** v126 s13-s15 (t10): `X = 4 {X pricing: kills THEIRS: Overgrown Battlement x2, Pride
Guardian; YOURS: none} [<- most kills at any affordable X that costs you nothing]` - all three
are `[defender]` (primitives verified). Rule (b) says do not cast. It was cast. It cost nothing
in that game only because **Rorix Bladewing flies** and was already getting through for 6 a turn
(s11 20 -> 14, s16 14 -> 8, s22 8 -> 2; game won 19 to -1 on turn 14).
EDIT: adds "THE ONE CARVE-OUT, and it is about YOUR attacker, not theirs" - a GROUNDED attacker
that those defenders are actually stopping makes the sweep a tempo play; Rorix never does.

## D52-6 - cycling Starstorm with creatures on their line (1 break)
**Finding.** v162 s20 (t10): `cycling with Starstorm` taken with two creatures on the opponent's
battlefield line (Shield Sphere, Fog Bank). Other Starstorm cycles were correct 4/4 (v125 s19,
s28, s45, s90 - all at "of which 0 are creatures").
EDIT: the release is the line's own count and nothing else - "not 'they are only defenders', not
'I need a land'."

## D52-7 - cycling Lay Waste below eight lands (1 break)
**Finding.** v146 s45 (t12, **5 life**, six lands, from a blockers-step priority window).
EDIT: "Count YOUR lands on your own battlefield line before you answer", with the s45 numbers.

## D52-8 - cleanup discard: two castable cards sent while four duplicates sat in hand
**Finding.** 4 cleanup discard asks (wave 51: 0 - this rung was UNTESTABLE-AT-THIS-SEAT for two
corpora). v162 s16 (hand 10, discard 3, four lands): sent Hammer of Bogardan `{1}{r}{r}`
(castable that turn), Rorix Bladewing (sanctioned by the order), and Pyrite Spellbomb `{1}` - the
cheapest card in the hand - while holding `Talisman of Impulse (copy 1 of 2)`,
`(copy 2 of 2)`, `Starstorm (copy 1 of 2)` and `(copy 2 of 2)`. One turn later (s23) it sent both
Talismans anyway.
EDIT: "A CARD YOU COULD CAST THIS TURN OR NEXT IS NEVER IN THAT LIST WHILE A DUPLICATE IS ON THE
SCREEN", naming the `(copy N of M in your hand)` marker the ask prints.

## D52-9 - NEW: a counter-deck paragraph (deck125), the 70-turn loss
**Finding.** v125: Siege-Gang Commander cast t8 and t20, Rorix cast t10 and t38 - the seat log
holds **zero `attackers` records in 117 decisions across 70 turns**, and the battlefield line
read "of which 0 are creatures" at every window from t8 to t20 and again after. One threat per
turn into open blue mana. Final 25 to -5 with seventeen sources untapped.
deck123's guide has had a counter-deck paragraph since wave 50; deck130's had none.
EDIT (added above PYRITE SPELLBOMB): cast the CHEAP spell FIRST and the threat SECOND in the same
main phase (Talisman {2}, Spellbomb {1}, a second Talisman, a Stone Rain); "never lead a turn
with Rorix or Siege-Gang while they have two or more untapped lands and a card in hand."

## D52-10 - a DRAW PUNISHER is a kill target, not only a draw gate
**Finding.** Rule #0 held perfectly: **59 windows with the `DRAW PUNISHERS on the battlefield:`
line, 0 voluntary draws taken** (wave 51: 0/14). v162 was lost anyway, 0 to 20 on turn 16, to
FORCED draws: two Howling Mines on their side plus Fate Unraveler and Underworld Dreams. Nothing
in the guide made the punisher a target. Primitives verified: Fate Unraveler is an
`Enchantment Creature`, power 3 toughness **4** - Hammer of Bogardan (3) and Pyrite Spellbomb (2)
cannot kill it alone; the Starstorm cast row at s66 reads
`At X=4 - kills THEIRS: Fog Bank, Fate Unra...` and the pilot did not reach that X until turn 14
at 8 life. Underworld Dreams is an Enchantment (unanswerable by this deck); Howling Mine an
Artifact (also unanswerable) - so the Unraveler was the whole answer.
EDIT (rule #2 point 1): a creature named on the DRAW PUNISHERS line joins the planeswalker at the
top of the target order, with the 4-damage arithmetic spelled out.

---

## Trims paid for the additions (net +5.1 KB, not +5.9)
- **D52-T1** the "Cast Starstorm at X=2 to kill Shield Sphere" story compressed - the
  `{X pricing: ... even at X=N NOTHING ... dies}` clause is now answered "Cast nothing right now"
  in 60+ of 60+ windows this corpus, so the paragraph has done its job.
- **D52-T2** the X-menu STEP 3 anecdote deleted (the Pride Guardian / Overgrown Battlement story
  is now carried by D52-5 with a live example).
- **D52-T3** the land-plan life-floor anecdote compressed and re-pointed at v146 s38, with the
  lane-K life clause quoted in place of the old "Paid" line.
