# deck125 (Revelation Fracture) - wave-56 guide edits, before -> after

Base: live guide `projects/mtg/bin/Res/ai/baka/deck125_strategy.txt` (67,098 B).
Result: `wave56/deck125/strategy.txt` (**69,761 B**, inside the 41-71 KB pool band).
Five additions, four compressions of rules now at their ceiling; net +2,663 B.
Corpus `matchups-20260903-120952`, deck125 seat logs (6 games, 813 decisions).

---

## W-E1 - the Staff ping goes at the DRAW PUNISHER, not the face (STAFF OF NIN section)

**Finding.** `125v162` (lost 0/6, t24-25). `Ob Nixilis, the Hate-Twisted [planeswalker]
[counters: 5x loyalty]` entered on t18 and the prompt's own line read
`DRAW PUNISHERS on the battlefield: theirs - Ob Nixilis, the Hate-Twisted. Every card YOU draw
costs you 1 life to theirs` with `DRAW FORECAST: your next draw step draws 3 cards
(1 + Dictate of Kruphix 1 + Howling Mine 1) = 3 x 1 = 3 life to the punishers above` (seq 137).
Every ping window from seq 87 (t19) to seq 166 (t25) printed
`1. Deal 1 damage with Staff of Nin #1 targeting Ob Nixilis, the Hate-Twisted ...
{right now: takes 1 damage - SURVIVES (loyalty 5, 4 left)}` as **option 1**. The seat took the
FACE row **8 times of 8** (seqs 87, 88, 108, 110, 137, 139, 164, 166) and went 16 -> 0 while
taking them 13 -> 6. Five of those eight pings kill the planeswalker outright. Its two Staffs
also DRAW two extra cards a turn, so the Staffs were feeding the punisher.
Primitive check: `planeswalkers.txt:2684-2691` Ob Nixilis, the Hate-Twisted -
`auto=counter(0/0,5,loyalty)`, `auto=@drawfoeof(player):damage:1 opponent`, 1 damage per draw.
`mtg.txt:39182-39190` Fate Unraveler - same `@drawfoeof` trigger, 3/4 Enchantment Creature.
STRATEGY (the render printed the punisher line, the forecast, the loyalty and the survive verdict;
nothing was missing).

**Before**
```
- Point it at a creature or a planeswalker only when 1 damage finishes it: printed toughness 1, or a
  planeswalker showing exactly 1 loyalty counter. Otherwise it goes at the player.
```

**After** - a 12-line block headed
`- THE PING GOES AT THE FACE, WITH ONE EXCEPTION, AND THE EXCEPTION IS PRINTED AS ITS OWN LINE.`
naming the DRAW PUNISHERS / DRAW FORECAST lines, the `SURVIVES (loyalty N, N-1 left)` /
`SURVIVES (toughness N)` verdicts as the kill counter, the two-pings-a-turn arithmetic, and the
eight-of-eight cite; the old sentence is kept as the no-punisher branch. A mirror line was added
to DECIDING SITUATIONS.

## W-E2 - the Emrakul CONVERTER stop, re-cited and compressed (#1)

**Finding.** `125v126` **seq 254** (t55, **37 life vs their 2**, 18 sources, two Staffs on the
battlefield that would have won on the next upkeep). The prompt carried
`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond #1, Sanguine Bond #2` and the
menu's row 3 was `Cast Emrakul, the Aeons Torn {15} (15/15) {leaves 3 of your 18 ...}`. Reply:
`CHOICE: 3 (Cast Emrakul, the Aeons Torn)`. Their Tribute to Hunger ate it (deck126 seqs 261-262,
`Emrakul, the Aeons Torn (15/15) ... is sacrificed, you gain 15`, taken twice), both Bonds
converted, and the seat went 37 -> 22 -> 7 -> **-8** in that one turn. This is the third game in
two corpora lost to this row. The guide's block was long and story-shaped; it is now shorter and
absolute, with the "the closer they are to dead the worse this row is" case written in - because
that is the shape that broke it.

**Before** - the six-line "THE CONVERTER LINE WAS PRINTED IN ALL 353 OF YOUR PROMPTS ..." paragraph.
**After** - a nine-line paragraph headed
`THIS IS THE MOST EXPENSIVE ROW IN THE DECK AND IT HAS NOW COST THREE GAMES`, carrying the 37/2
cite and the new rule sentence `THE CLOSER THEY ARE TO DEAD, THE WORSE THIS ROW IS`.

## W-E3 - the tap-out gate's SECOND-Staff clause gets its break (STAFF OF NIN, shape (d))

**Finding.** `125v146` **seq 65** (t26, 13 life, six untapped sources). Row 1 read
`Cast Staff of Nin {6} [second copy: you already control Staff of Nin; ...] {leaves 0 of your 6
untapped mana sources untapped - casting this taps you out} {spends 6 of your 6 ...; Essence
Scatter {1}{u} in your hand needs 2}` with `Cancel`, `Fall of the Gavel`, `Dream Fracture` and TWO
`Essence Scatter` on the hand line, and the opponent header reading `3 are creatures, 3 of them
without a restriction against attacking`. Taken. A `Kaya the Inexorable` -3 exiled that Staff two
turns later (seq 68's battlefield line shows one Staff); the five counterspells never left the
hand and the seat died -10 on t29. Staff casts at `casting this taps you out` this corpus: **5 of
6 rows** - four FIRST Staffs (sanctioned by shape (d)) and this one SECOND. STRATEGY.

**Before** - shape (d) ended `...the gate binds that row exactly as written.`
**After** - the same sentence plus a six-line cite of seq 65.

## W-E4 - a mana rock is a decline at EVERY life total (SPEND YOUR ANSWERS)

**Finding.** 29 counterspell casts this corpus. Three went at a stack line whose
`{target text:}` reads only "Add": `126v125`-side `125v126` **seq 167** (Cancel at Chromatic
Lantern, t42, 33 life) and **seq 213** (Fall of the Gavel at Chromatic Lantern, t50, 33 life), and
`125v130` **seq 109** (Fall of the Gavel at Talisman of Impulse, t58, 32 life). Counters at a
`[defender]` or printed-power-0 stack line: **0** (last corpus's two breaks are closed).
Every one of the three was at 32-33 life; "far ahead" is the excuse and it is now named.

**Before** - the paragraph ended `An (artifact) stack line with no P/T is a decline unless its
{target text:} damages, draws or makes tokens.`
**After** - the same, plus `BEING FAR AHEAD IS THE EXCUSE THIS LINE KEEPS LOSING TO, AND IT IS
NOT ONE.` with the three cites and their life totals.

## W-E5 - a rule for land destruction, which the guide did not have (SPEND YOUR ANSWERS)

**Finding.** `125v130` (won 32/0, t60) is a land-destruction matchup and the guide had no line for
it, so four of the seat's counters were unruled: `seq 35` Dream Fracture at
`Lay Waste {3}{r} (sorcery) [cycling] {target text: "Destroy target land."}` at **2 lands**,
`seq 67` Fall of the Gavel at `Molten Rain {1}{r}{r}` , `seq 46`/`seq 82` Dream Fractures at
`Dwarven Blastminer {1}{r} (creature 1/1) {target text: "{2}{R}, {T}: Destroy target nonbasic
land."}`. All four were right and the game was won on fourteen lands at turn 60; the rule that
sanctioned them did not exist. The Blastminer is already covered by the repeating-engine line.

**Before** - the bullet began `- anything printed (land), (artifact) with no P/T, or
(instant)/(sorcery) that only draws or searches: LET IT RESOLVE.`
**After** - a new bullet ahead of it: counter a `"Destroy target land"` spell at 5 lands or fewer
on your own battlefield line, let it resolve at 6 or more, with the four cites.

---

## Compressions (rules at their ceiling; no rule weakened)

- **X = 3 section** (-1,100 B): the three paragraphs of argument collapse to the rule, the
  library-floor lower bound, and the new COLLAPSED-MENU instruction. Evidence: 3 X menus this
  corpus, 3 answers of `X = 3`, off menus topping out at 3, 4 and 6 - one of them the collapsed
  form `2-5. X = 5 down to X = 2` at `125v130` seq 72, where the seat answered option 4 (= X = 3)
  and decoded the range correctly. The new lines TEACH that decode.
- **low-X Revelation** (-700 B): 86 `max affordable X=1/2` rows, **0 taken** (was 2 of 92). The two
  anecdotes go; the "an empty menu is still an empty menu" sentence stays.
- **sweeper at header 0** (-400 B): 124 dead sweeper rows, **0 cast**. Two corpora at zero.
- **Elixir activation gate** (-400 B): 9 casts, 9 activations, **0 gate breaks** (was 5).
