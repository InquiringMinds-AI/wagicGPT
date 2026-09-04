# wave-59 deck152 review (Bant Human midrange, seat record 3-3)

Corpus: `matchups-20260904-130044`, my six seat files `*deck152-0x*`.
236 decisions, **0 fallbacks, 0 `transport` records, 0 re-asks, 0 timeouts** in my six games.

## 1. Game by game

| opp | result | turn | my/their life | the decision that decided it |
|---|---|---|---|---|
| 123 | WIN | 11 | 22 / 0 | clean beatdown; Katilda t3, two Brutes, alpha at s28. No contested window. |
| 162 | WIN | 13 | 27 / -6 | curve-out (Adversary t3, Ranger t5, Tracker t7, Sigarda+Aspirant t11); s27 four-attacker alpha ends it. |
| 125 | WIN | 27 | 18 / -4 | mull to 6 (s1-s3, bottomed Sigarda) then a long grind; the Moonrage Brute attacks at s33/43/55/68 did all the work. 28 HOLD skips, 9 identical-ask answers reserved - the D-lane machinery carried a 70-decision game with no re-ask. |
| 146 | LOSS | 17 | -4 / 21 | **s35** chump-block with Brutal Cathar (the block returned the Nadaar that Cathar had EXILED - never rendered) and **s38-s42**, the whole pre-lethal turn spent on a Teferi +1 that taps permanents which untap before the attack. |
| 126 | LOSS | 14 | -1 / 34 | **s27** cast Tovolar's Huntmaster off a row printed `{paying this taps: Brutal Cathar, Katilda, Dawnhart Prime - they cannot attack this turn}` - two attackers sold in Main 1 for a summoning-sick body, one turn before the opponent assembled Sanguine Bond + Exquisite Blood. **s34** then had no non-fatal answer. |
| 130 | LOSS | 29 | 0 / 5 | mana denial + burn (Molten Rain on my third land t24, Hammer of Bogardan loop t26/t28 killing Katilda and Intrepid Adversary). Two lands on turn 17 with a seven-spell hand: variance plus removal, no seat decision I can fault. |

## 2. Engine / interface / card items

### HIGH-1 - Teferi's +1 never prints its two verbs, and its sub-menus never say tap vs untap (cost the 146 game)
Every render of the card cuts the text at `+1: Choose up to one target artifact, up to one target
creature, and...` - **27 renders in my six games, 60 corpus-wide** - i.e. `Untap the chosen
permanents you control. Tap the chosen permanents you don't control.` is never shown. The
activation then fans into a chain of bare menus (`choose your land` / `choose opponent land`,
`choose a creature` / `choose opponent creature`) that also never state which branch taps and
which untaps, nor that a tap is undone by the target's own untap step.
Repro: `1788544866-ai_baka_deck152-0x5591f86fcba0-vs-ai_baka_deck146.jsonl`, seq 38-42 (five asks
to place two targets). Rendered line, seq 38 row 1:
`1. +1: tap or untap permanents with Teferi, Who Slows the Sunset [cost: Counters] {card text: "+1: Choose up to one target artifact, up to one target creature, and... -- -2: ..."}`
Cost: at 10 life under `CRACK-BACK NEXT TURN: 4 of their creatures will be able to attack ... for
up to 14 - you would be at -4; that would KILL you`, the seat wrote in four consecutive plans that
tapping now "stops one of the 4 attackers" (s38, s40, s41, s42), then declined all three casting
windows (s43, s44) and died to all four. A second no-op: `...vs-ai_baka_deck123.jsonl` seq 24 took
`Tundra [land] [opponent's battlefield] [tapped]` - tapping an already-tapped permanent.
Primitive verified (`planeswalkers.txt:3339`) and matches Scryfall exactly; the script is right,
the render is the liar. This is wave-58's deck152 MED ("Teferi +1 = 7 round trips, text truncated
before its verbs") recurring and now paying for a loss - escalate.
Fix: print the two verbs (untruncated for a row whose whole meaning is its verbs), label the
sub-menu rows (`choose opponent land [TAPS it - it untaps in their untap step, before they
attack]`), and badge an already-tapped target as a no-op.

### HIGH-2 - a creature exiled "until this leaves the battlefield" is invisible at the block that kills the exiler
`...vs-ai_baka_deck146.jsonl` seq 35. Brutal Cathar had Nadaar, Selfless Paladin (4/4, vigilance)
exiled. The block window renders:
`B1. Brutal Cathar (2/2) [daybound] - may block A1 (your blocker dies, attacker lives) {after this combat: you control 1 fewer blocker - 1 available now, 0 if this one dies here}`
and an opponent battlefield line reading `of which 3 are creatures`. Nothing anywhere states that
a 4/4 comes back when this blocker dies. The header said `you would be at 6`, so the guide's
chump rule (correctly) said block; the block returned Nadaar, and seq 43's crack-back line counts
**4 attackers for 14** - Nadaar among them. The `{after this combat: you control 1 fewer blocker}`
tag is exactly the right place for `+ their Nadaar, Selfless Paladin (4/4) returns from exile`.
Same shape earlier in the file (log at seq 10: `Nadaar was exiled ... Nadaar entered the
battlefield from exile` when the first Cathar died) - so it happened twice in one game.

### HIGH-3 - "you must sacrifice a creature" is rendered as a target choice, with no price on the rows
`...vs-ai_baka_deck126.jsonl` seq 34. Rendered header:
`TARGET CHOICE for Tribute to Hunger - its "gain life equal to its toughness" ability (this spell/ability is already on the stack and needs a target - it is NOT a cast or phase step). Pick the ONE target it will affect from the list below`
The primitive (`mtg.txt:124073`, `notaTarget(creature|mybattlefield) ... toughnesslifegain
targetopponent ... sacrifice`) and Scryfall agree: this is *my* sacrifice, and *their* life gain
equals the sacrificed creature's TOUGHNESS. The ask says neither. Seven rows are printed with P/T
but no `[you SACRIFICE this - they gain N life]`, and the seat picked its 7/7 while writing
"denying them life gain". With Sanguine Bond + Exquisite Blood out every pick was lethal, so this
did not change *this* result - but the render defect is generic to every edict/sacrifice ask.

### HIGH-4 - the loop warning arrives one decision after the loop closes
Same file. `Exquisite Blood` sat on the opponent's battlefield and `Cards you have seen in the
opponent's hand: Sanguine Bond` sat in my frame for **seq 9 through seq 33 - 25 consecutive
decisions, turns 7-13 - with no warning of any kind**. The `LIFE-TO-DAMAGE CONVERTER on the
battlefield` block (which correctly says "ANY nonzero payment on a tag above is fatal") appears
for the first time at seq 34, after Sanguine Bond resolved, on the ask that killed the seat.
A warning is only worth its render if it precedes the decision it governs: when one half of a
named loop is on the battlefield and the other half is a card the seat has SEEN in their hand,
say so on every frame from the moment both facts are known.

### MED-1 - the valor-counter menu prints 21 rows when 0 of them can do anything
`...vs-ai_baka_deck130.jsonl` seq 17: `Mana available: 0 total (no untapped sources)`, then a
21-row menu whose own note reads `With no spendable mana left, every option adds 0 counters.`
**2 of the corpus's 3 Intrepid Adversary counter asks are in this state** (the other is
`...vs-ai_baka_deck162.jsonl` seq 6). A decision with one reachable outcome should collapse to one
row (or auto-answer) rather than spend a round trip on 21. Note the wave-58 HIGH-1 signature
(payment stopping early with sources untapped) did NOT recur: seq 29 asked 1 and paid 1.

### MED-2 - the counter-gain log line prints the pre-static P/T and contradicts the board frame
The log writes `Your Intrepid Adversary got a valor counter (now 3/1)` while the battlefield frame
for the same permanent reads `Intrepid Adversary {1}{w} (4/2) (printed 3/1) [lifelink] [counters:
1x valor]`. The `(now N/N)` parenthetical is the base P/T, not the current one, so the narration
contradicts the frame for any card whose counters feed a static lord. Repro of the cost, in my
opponent's seat for my own game: `1788544884-ai_baka_deck130-0x5570b56f0570-vs-ai_baka_deck152.jsonl`
seq 62, whose reply spends a long trace on it - *"Got 1 valor counter -> 4/2? No ... Log says
(now 3/1) ... Wait, the board frame says (4/2). Why is"*. The frame is right; the log line is wrong.

### LOW-1 - "Cast nothing right now" re-opens the same window; the HOLD row is right there
`...vs-ai_baka_deck125.jsonl` seq 64/66/67: three asks in one turn whose only cast row is
Fateful Absence with `- the only legal targets are YOUR OWN right now` and a self-harm warning.
Rows 3 (`Hold priority for the rest of this turn`) would have closed the turn; the seat answered
`Cast nothing` each time and got `[you declined this exact list 2 times already this turn]`. The
counter and the HOLD row both work - this is the seat under-using them, so a LOW note only.

### Lane observations from my six files (not adjudication - that is the engine seat's)
- **K7 holds here**: 0 bare `Play Land` rows in my 236 decisions (and 0 corpus-wide, vs 32 in wave
  58); every land-drop row I took printed a face name and a `PLAY THIS AS A LAND` tag (e.g.
  `...vs-deck130` seq 15 `Boulderloft Pathway [PLAY THIS AS A LAND: ...]`).
- **K8 own-turn crack-back renders**: `...vs-deck146` seq 43/44 carry `CRACK-BACK NEXT TURN: 4 of
  their creatures will be able to attack (tapped ones untap first), for up to 14 - you would be at
  -4; that would KILL you` on MY main phase, with tapped creatures counted. The number was correct.
- No `{dead right now:}` badge in my files named a live magnitude (K4): the only one that fired
  (`...vs-deck130` seq 13, Fateful Absence `{dead right now: 0 legal targets on the board for it}`)
  was true - the opponent controlled 0 creatures.

## 3. Guide verdict: EDIT (one edit)

Full revised guide at `wave59/deck152/strategy.txt` (started from the live
`bin/Res/ai/baka/deck152_strategy.txt`; 65,395 -> 67,114 bytes, inside the 41-71 KB band).

**Edit 1 - the TEFERI paragraph (live guide line 464-468).**
- BEFORE: `Do not contort your mana for Teferi - keep the GW curve on line. Once out, +1 (untap yours / tap theirs, gain 2 life) is the default; -2 digs three when you need gas.`
- AFTER: the mana sentences unchanged, then a block that (a) states the two verbs the render never
  prints and which menu branch carries each, (b) states that a tap does not survive the target's
  own untap step - on your turn the tap half only removes a BLOCKER, to stop an ATTACKER you must
  tap on THEIR turn before attackers are declared, (c) says a target printed `[tapped]` is a
  wasted pick, (d) makes -2 the default when your battlefield line reads `0 are creatures`, with
  the WHAT THIS COST paragraph naming the two takes below.
- PAID BY: `...vs-ai_baka_deck146.jsonl` seq 38-42 + seq 43/44 (the losing turn: five asks and
  three declines spent on an effect the untap step undoes, under a printed lethal crack-back), and
  `...vs-ai_baka_deck123.jsonl` seq 24 (the +1's land pick spent on an already-`[tapped]` Tundra).
  The old line named "tap theirs" as *the default* with no timing law attached, so the seat was
  following the guide when it lost the turn.

**Deliberately NOT edited** (both are rule VIOLATIONS against correct renders and correct text -
more guide prose would not have helped):
- #2a (`a row whose clause names TWO OR MORE creatures is PASS, always, in Upkeep or Main 1`) was
  violated at `...vs-ai_baka_deck126.jsonl` seq 27, on a row that printed the clause verbatim
  (`{paying this taps: Brutal Cathar, Katilda, Dawnhart Prime - they cannot attack this turn}`)
  and whose own reply then listed both tapped creatures as attackers. The rule, its WHAT THIS COST
  and the clause-count test are already on the page.
- #4 blocking: the seq-35 chump at `you would be at 6` was the guide's ladder answering correctly
  on the facts it was given; what was missing was a fact (HIGH-2), not a rule.

No general-strategy proposal and no skill proposal. The Teferi timing law is general in principle,
but deck152 holds the pool's only permanent-tapper (deck125/126/130/162's "Teferi" hits are
Teferi's Puzzle Box), so the general guide is the wrong home for it and a general amendment here
would be untriggered text for six of seven decks.

## 4. What I did NOT check
- I did not read the other decks' seat files except deck126's and deck130's for context on my own
  games, and deck130's seq 62 for the MED-2 quote; corpus-wide counts I report (60 Teferi
  truncations, 0 bare `Play Land`, 3 valor asks) are mechanical greps over all 42 files, not reads.
- I did not adjudicate any lane prediction - HIGH/K notes above are observations from my six files.
- I did not rebuild or run the engine, and did not verify any proposed fix compiles or fires.
- I did not verify Nadaar/Brutal Cathar/Vanishing Verse/Silverquill Command scripts against
  Scryfall; the cards I did verify both ways are Tribute to Hunger, Teferi Who Slows the Sunset,
  Intrepid Adversary, Exquisite Blood and Sanguine Bond (primitives faithful in all five).
- I did not measure whether the deck130 loss was avoidable with a different mulligan; the hand was
  a real keep with lands, and I read the rest as land destruction plus burn, not seat error.
