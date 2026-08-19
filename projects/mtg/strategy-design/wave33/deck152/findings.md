# deck152 (Bant Midrange GWU) -- wave-33 findings

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260819-070739`, binary `bdb30d8e7`, -T 3000,
REPPENALTY=1.05, -j 3. Comparison corpus: wave-32 `matchups-20260727-115847` (binary `7fabd9bd0`).
Third guided corpus; guide revised at the end of wave-32 (werewolf frequency claim removed, Flip
Side prohibition generalized, deploy-floor exceptions closed, infect stopgap added).

**Record 3/6** (FLAT for the third consecutive corpus), read off `results.tsv` + `gameend`:

| opponent | result | my_life | opp_life | turn | shape |
|---|---|---|---|---|---|
| deck105 | **WIN** | 20 | 0 | 12 | kill -- **the wave-32 poison loss inverted** |
| deck139 | **WIN** | 14 | -16 | 13 | kill |
| deck36 | **WIN** | 12 | 6 | 14 | cap adjudication, ahead |
| deck146 | LOSS | -1 | 12 | 16 | killed (two 4/4 fliers, no reach) |
| deck158 | LOSS | -3 | 14 | 13 | killed -- **a MULLIGAN loss, see R6** |
| deck116 | LOSS | 15 | 21 | 15 | cap adjudication, behind on life |

**HL7 decomposition**: KILLS 3 -> 3 -> **2** (wave-31 3, wave-32 3, wave-33 2), with the third
win an adjudication-ahead. The pool-invariant statistic slipped by one; the record did not.
Two cap adjudications this corpus at this seat (wave-32: one) -- both latency-driven, see LATENCY.

**Fallbacks at this seat: 1 / 211 decisions (0.47%)** -- breaking a two-corpus zero streak, and
the one is the NEW `all_assignments_illegal` class firing exactly as i8 designed (see i8). No
`unparsed_reply`, no `empty_reply`, no `retracted_choice`, no defers, no truncations.
Decision mix: 150 ask, 23 attackers, 26 priority, 9 blockers, 2 bottom, 1 reveal.

**Crash gate: 0 signatures** in all six of my `.stderr` files.

**LATENCY**: median 17.0s / p90 82s / p95 130s / max 194s; **27 decisions over 60s, 18 over
100s**. Reply p50 1049 / p95 7774 / max 15082 (wave-32: p50 1123 / p95 7562 / max 14458 -- flat).
Prompt p50 7989 / p90 13678 / max 25008 (wave-32 p50 7610 / p90 15769 / max 23775). The two cap
adjudications are latency, not decision count: vs36 reached 38 decisions and vs116 45 decisions
inside 3000s while the games were at turn 14/15 and still live.

---

## HL1 HARD GATE -- RETROACTIVE FULL-GUIDE CARD-FACT LEDGER (run on the guide I am SHIPPING)

Per HL2 this is a per-claim ledger, run as a terminal pass over the revised text, and every
sentence written THIS wave is in scope.

| # | claim as written (revised guide) | adjudicating primitive line | verdict |
|---|---|---|---|
| 1 | Sigarda `{1}{G}{W}{W}` 4/4 flying trample, Humans +1/+1 | `mana={1}{G}{W}{W}`, `power=4`, `abilities=flying,trample`, `auto=lord(human|mybattlefield) 1/1` | TRUE |
| 2 | Sigarda's coven attack dig | `auto=@combat(attacking) source(this) restriction{coven}: ... reveal:5 ... target(<upto:1>human|reveal) moveto(hand)` | TRUE |
| 3 | Intrepid Adversary `{1}{W}` 3/1 lifelink, ETB pay `{1}{W}` any number of times, +1/+1 per valor | `mana={1}{W}` `power=3` `toughness=1` `abilities=lifelink`; `auto=choice name(Add 1..5 counters) ... pay({1}{W}) ... counter(0/0.1.Valor)` | TRUE |
| 4 | Katilda `{G}{W}` 1/1, your HUMANS tap for one mana of their own colour, pro-Werewolves | `mana={G}{W}` `power=1` `toughness=1`; `auto=lord(creature[human&green]|myBattlefield) {T}:add{G}` (+ four colour siblings); `auto=protection from(*[werewolf])` | TRUE |
| 5 | **NEW THIS WAVE**: Katilda is herself a Human, so she taps for `{G}` or `{W}` | `subtype=Human Warlock` | TRUE (live witness: deck116 s29, 7 lands + Katilda = `Mana available: 8 total`) |
| 6 | Katilda `{4}{G}{W},{T}` = a +1/+1 counter on EACH of your creatures | `auto={4}{G}{W}{T}:name(Put 1/1 counters) all(creature|mybattlefield) counter(1/1)` | TRUE |
| 7 | Luminarch Aspirant `{1}{W}` 1/1, +1/+1 counter each of your combats | `mana={1}{W}` `power=1`; `auto=@each my combatbegins:counter(1/1) target(creature|myBattlefield)` | TRUE |
| 8 | Ranger Class `{1}{G}`, Wolf on ETB, L2 `{1}{G}`, L3 `{3}{G}` | `mana={1}{G}`; `auto=name(Create Wolf) _WOLFTOKEN_`; `this(variable{hascntlevel}=1) {1}{G}`; `=2 {3}{G}` | TRUE |
| 9 | Fateful Absence `{1}{W}` instant, destroy creature or planeswalker, they investigate | `mana={1}{W}`, `type=Instant`, text verbatim | TRUE |
| 10 | Brutal Cathar `{2}{W}` 2/2, ETB/transform exiles an opponent creature until Cathar leaves | `mana={2}{W}` `power=2`; `auto=name(Exile a creature) target(creature|opponentbattlefield) (blink)forsrc` | TRUE |
| 11 | Moonrage Brute 3/3 first strike, Ward-Pay 3 life | `power=3 toughness=3`, `abilities=first strike,nightbound`, ward auto present | TRUE |
| 12 | Tovolar's Huntmaster `{4}{G}{G}` 6/6, ETB two 2/2 Wolves | `mana={4}{G}{G}`, `auto=_WOLFTOKEN_*2` | TRUE |
| 13 | Tovolar's Packleader 7/7, enters OR attacks -> two Wolves, `{2}{G}{G}` fight | `power=7 toughness=7`, text verbatim | TRUE |
| 14 | Elite Spellbinder `{2}{W}` 3/1 flying | `mana={2}{W}` `power=3 toughness=1` `abilities=flying` | TRUE |
| 15 | Teferi `{2}{W}{U}`, starts at 4 loyalty, +1 untap yours/tap theirs + gain 2 life, -2 digs three | `auto=counter(0/0,4,loyalty)`; `+1 ... counter(0/0.1.TeferiEffect)` + `@counteradded ... life:2 controller`; `-2: Reveal top 3 cards` | TRUE |
| 16 | Deserted Beach taps `{W}` or `{U}`; Overgrown Farmland taps `{G}` or `{W}` | `auto={T}:add{W}` / `add{U}`; `add{G}` / `add{W}` | TRUE |
| 17 | **NEW THIS WAVE**: both duals enter TAPPED unless you already control two other lands | `auto=if type(other land|mybattlefield)~lessthan~2 then tap(noevent)` | TRUE |
| 18 | Augur of Autumn `{1}{G}{G}` 2/3, coven -> cast creatures off the top | `mana={1}{G}{G}` `power=2 toughness=3`; `this(variable{pcoven}>0) canplaycreaturelibrarytop` | TRUE |
| 19 | Briarbridge Tracker `{2}{G}`, vigilance, +2/+0 while you control a token | `mana={2}{G}` `abilities=vigilance`; `auto=aslongas(*[token]|mybattlefield) 2/0 >0` | TRUE |
| 20 | **NEW THIS WAVE**: 25 lands; 14 make green, 15 make white, and **only the 4 Overgrown Farmlands make BOTH** | `deck152.txt` counted: Forest 1 + Branchloft 4 + Barkchannel 4 + Lair 1 + Farmland 4 = 14 G; Plains 3 + Hengegate 4 + Beach 4 + Farmland 4 = 15 W | TRUE |
| 21 | **NEW THIS WAVE**: your cheapest cards cost TWO mana; Katilda `{G}{W}` is the only 2-drop needing both colours | decklist MVs: min 2 | TRUE |

**No false card fact found. The audit PASSES for the second consecutive corpus.** Class sweep per
HL2.3: the one class corrected this wave is the (printed X/Y) werewolf claim (see i7); grepped the
whole guide for `printed` -- one occurrence, fixed in place.

**HL3.1 QUOTED-STRING SWEEP** (run BEFORE reading logs; deployed guide's quotations vs the corpus
`prompt` fields at this seat):

| deployed quoted string | hits | disposition |
|---|---|---|
| `colours you can make` | 212 | LIVE, kept |
| `Cast nothing right now` | 99 | LIVE, kept |
| `Flip Side` | 76 | LIVE, kept |
| `Ignore this option` | 18 | LIVE, kept |
| `no untapped sources` | 33 | LIVE, kept |
| `(printed 2/2)` (generic) | 38 | LIVE, general teach kept |
| **`Moonrage Brute (3/3) (printed 2/2)`** | **0** | **DEAD -- i7 fixed it; teach SUPERSEDED, deleted** |
| **`Tovolar's Packleader (7/7) (printed 6/6)`** | **0** | **DEAD -- same, deleted** |
| **`Damage: N dealt by <their infect creature> to you` with life unchanged** | **0** | **DEAD -- i1 replaced the emitter; the infect STOPGAP AUTO-DEMOTES** |

Three zero-hit strings in a 126-line guide, all three because a wave-33 fix landed. Two of the
three sit inside paragraphs whose *claims* are now false about the prompt, not merely stale.

---

## (i4) N-152f `deferFlexibleSource` PER-ABILITY: **PASS, ARRIVAL-TRACED, with a negative-control ladder**

Arrival traces, never a fallback count. Two independent arrivals on the BROKEN branch, plus a
three-window negative control and an isolated one-source transition.

### ARRIVAL A -- the tight one (exact mana, every coloured pip on a flexible source)

**deck36 seq26 t12, `kind=ask`.** Four untapped sources, all lands: `Barkchannel Pathway`
(`{G}` only), `Deserted Beach` (`{T}:add{W}` THEN `{T}:add{U}`), `Lair of the Hydra` (`{G}` only),
`Overgrown Farmland` (`{T}:add{G}` THEN `{T}:add{W}`).
`Mana available: 4 total (four untapped sources ... colours you can make: {g}{u}{w})`.
Teferi is `{2}{U}{W}` = **exactly four**, no slack.

- The ONLY blue is Deserted Beach's SECOND ability.
- The ONLY two white sources are Deserted Beach and Overgrown Farmland -- **both flexible**.
- The unique legal payment is Beach->`{U}`, Farmland->`{W}`, Barkchannel+Lair->`{2}`.

That is the wave-32 mutual-deferral deadlock verbatim: Beach considering `{W}` sees Farmland's
`add{W}` ability and judges it "dedicated to W"; Farmland considering `{W}` sees Beach's `add{W}`
and does the same. Post-fix render:

```
3. Cast Teferi, Who Slows the Sunset {2}{u}{w} {card text: "+1: Choose up to one target artifact, ...
```

**Offered, chosen, RESOLVED** -- seq27 events: `Your Teferi, Who Slows the Sunset: hand -> stack`,
`Your Teferi, Who Slows the Sunset: stack -> battlefield`, four
`Counter added to Teferi, Who Slows the Sunset: loyalty`, then the pilot activated
`+1: tap or untap permanents`. Arrival on the battlefield, not an offer count.
The same board offered Teferi at **seq13 t8** and **seq19 t10** too (declined for a creature both
times) -- three offers on the broken branch in one game.

### ARRIVAL B -- the three-pip one (all-flexible white, mono green)

**deck116 seq29 t12, `kind=ask`.** Eight untapped sources: `Forest`, `Branchloft Pathway`,
`Barkchannel Pathway #1/#2/#3` (all `{G}` only), `Deserted Beach` (W/U), `Overgrown Farmland`
(G/W), and **`Katilda, Dawnhart Prime`** -- a Human who taps for `{G}` or `{W}` under her own lord
ability (ledger row 5; `Mana available: 8 total` with only 7 lands proves the engine counts her).
Sigarda is `{1}{G}{W}{W}`: **every white source on the board is flexible** (Beach, Farmland,
Katilda), two must simultaneously commit to `{W}`, green comes off a mono Barkchannel.

```
2. Cast Sigarda, Champion of Light {1}{g}{w}{w} (4/4) {card text: "Flying, trample -- Humans you control get +1/+1. ...
```

**Offered, chosen, RESOLVED** -- seq30 events: `Your Sigarda, Champion of Light: hand -> stack`,
`stack -> battlefield`. On the battlefield line at s34/s36/s41, blocked and killed a Beast at
s34/s35, attacked at s42. Teferi (`{2}{U}{W}`, also all-flexible) is offered in the same window
and again at s36, where it is cast and resolved.

### NEGATIVE-CONTROL LADDER (same game, same seat -- the fix is isolated, not blanket)

deck116 seqs **12 / 19 / 24** all carry `colours you can make: {g}{u}{w}` and hold Teferi, and
Teferi is **correctly NOT offered** at all three: the Deserted Beach is the board's ONLY white AND
only blue source, so `{U}` and `{W}` cannot both be paid. s24 additionally holds Sigarda (two
white pips, one white source) and correctly suppresses it too. **The offer appears at s29 -- the
exact window the Overgrown Farmland arrived** (`- Your Overgrown Farmland: hand -> battlefield` in
s29's own events), i.e. the turn a second flexible white source existed. The fix engaging,
isolated, on a one-source delta.

### FALSE-SUPPRESSION SWEEP: **ZERO**

All 44 casting windows cross-checked: for each hand card, cost vs `Mana available: N total` and
the printed colour set. **Five apparent misses, all five correct** -- deck36 s14 and deck116 s5
are colour-impossible (`colours you can make: {g}` vs a `{W}` cost), deck116 s12/s19/s24 are the
negative controls above. **No castable-but-not-offered residual survives at this seat. N-152f is
CLOSED.**

### N-152g (legend suppression, KNOWN-OPEN, fix staged wave-34): **UNEXERCISED, 0 windows**

Trigger predicate = a legendary card in hand while a same-named legendary is on your own
battlefield. deck36 held **two** Teferi and cast one at s26 -- but s26 was the game's LAST casting
window (cap fell at t14), so the second copy never met a cast seam. Sigarda reached the
battlefield at deck116 s29 with no second copy in hand. **Suppressed windows counted: 0.** Not
re-diagnosed, not falsified, simply not sampled; the wave-34 fix should ship on the code reading.

---

## (i7) N-152d layer 2 -- `(printed X/Y)` ON THE BOARD-SNAPSHOT PATH: **PASS, zero false tags**

Counted over **all twelve logs of my six games** (my seat and every opponent seat), as fractions:

| string | wave-32 | wave-33 |
|---|---|---|
| `Moonrage Brute (3/3) (printed 2/2)` | **22** | **0** |
| `Moonrage Brute (3/3)` clean | 12 | **9 / 9 = 100%** |
| `Tovolar's Packleader (7/7) (printed 6/6)` | 0 (coverage) | **0** |
| `Tovolar's Packleader (7/7)` clean | 3 | **24 / 24 = 100%** |
| `Brutal Cathar (2/2)` clean (front face, no anthem) | -- | **110 / 110 = 100%** |
| `Tovolar's Huntmaster (6/6)` clean (front face, no anthem) | -- | **49 / 49 = 100%** |

**The previously-broken emitter is verified by name, on both sides.** My board snapshot, deck152
s27/s28/s29 verbatim:

```
Your battlefield (creatures: 1): Overgrown Farmland; Hengegate Pathway; Barkchannel Pathway; Branchloft Pathway; Plains; Moonrage Brute {0} (3/3) [first strike, nightbound]; Deserted Beach; Night {0}
```

and the opponent's mirror of the same permanent, deck158 s20/s21/s22/s23:

```
Opponent battlefield (creatures: 1): ... Moonrage Brute {0} (3/3) [first strike, nightbound] [tapped - cannot attack or block this turn]
```

Delta is 0 (Moonrage Brute is `power=3 toughness=3`, `subtype=Werewolf`, not Human, so Sigarda's
`lord(human)` cannot reach it; no counters, no other anthem) and **no tag is emitted**. Board
snapshot 7/7 clean, attacker/target enumerators 2/2 clean.

**POSITIVE CONTROL, same corpus, so the zero is not "the annotator stopped firing":** 51 distinct
`(printed X/Y)` renders fired at true deltas, including
`Tovolar's Huntmaster {4}{g}{g} (7/7) (printed 6/6)` **six times** in the vs146 game -- Huntmaster
is `subtype=Human Werewolf`, an Intrepid Adversary with one valor counter was on my battlefield,
and every other creature moved with it (`Wolf #1 (3/3) (printed 2/2)`,
`Intrepid Adversary (4/2) (printed 3/1)`). Also `Katilda, Dawnhart Prime (2/2) (printed 1/1)` x20,
`Sigarda ... (5/5) (printed 4/4)`, `Luminarch Aspirant (2/2) (printed 1/1)` x6,
`Briarbridge Tracker (4/3) (printed 2/3)` x17 (its own Clue token). The annotator is alive and
correct; only the transformed-face selection was wrong and it is fixed. **N-152d is CLOSED.**

### NEW, and caused by i5: a `{0}` cost printed on faces that have NO mana cost

Attribution per INTERIM CHANGES: this is **i5 / N-36b** (the six falsy-zero print guards routed
through `manaCostToken()`), not the step-1 transform work. Wave-32 corpus: `Moonrage Brute {0}` =
0 occurrences, `Day {0}`/`Night {0}` = 0 (they rendered bare). Wave-33, my six games:

| render | count | truth |
|---|---|---|
| `Mox Jet {0}` / `Mox Sapphire {0}` | 222 | **CORRECT** -- the intended fix, real `{0}` artifacts |
| `Day {0}` / `Night {0}` | 142 | **FALSE** -- a designation marker is not a card and has no cost |
| `Moonrage Brute {0}` | 11 | **FALSE** -- a transformed back face has NO mana cost, not a zero one |
| `Tovolar's Packleader {0}` | 13 | **FALSE** -- same |

The guard flipped from "print nothing when the cost is falsy" to "print `{0}` when the cost is
falsy", which is right for a real zero and wrong for an ABSENT cost. 166 false renders at this
seat, and it contradicts the Flip Side annotation's own closing promise (*"this card is NEVER cast
as its other face"*) sitting in the same prompt: `{0}` reads as free. **Not yet acted on** -- 0
replies reason about it (the Flip Side prohibition holds). Routed as **N-152k (MEDIUM)** with a
discriminator, not a diagnosis.

---

## (i8) `all_assignments_illegal` -- **ONE OCCURRENCE, WITH ITS TRIGGER SHAPE**

The corpus's only fallback at this seat, and it is the new class working.

**deck36 seq8 t5, `kind=blockers`**, `fallback: all_assignments_illegal`,
`pruned_pairs: "Intrepid Adversary -> Glaze Fiend"`, `choice_source: None`, `chosen_text: None`.

Offer verbatim:
```
A1. Silver Myr (1/1) deals 1
A2. Glaze Fiend (2/3) deals 2 [flying]
Your available blockers (with, for each attacker it may block, the naive 1-on-1 trade ...):
B1. Intrepid Adversary (3/1) [lifelink] - may block A1 (both die)
```
Reply line 1: `BLOCKS: B1:A2`. Glaze Fiend has **flying**; B1's own line says `may block A1` and
nothing else. The single assignment in the set was illegal, so the whole set pruned to empty.

**TRIGGER SHAPE for the ledger: a single ground blocker, a two-attacker set one of which is
evasive, and a reply that reasons the trade math ("3/1 trades with their 2/3, I take 1 instead of
2") without ever consulting the per-blocker `may block A#` list.** The reasoning is otherwise
sound and the protocol was obeyed (correct anchor, first line) -- a legality read, not a
compliance failure. Under wave-32's code this would have exited as a silent `unparsed_reply`
(N-139s); it is now named and the pruned pair recorded. **i8 PASS on this leg**, and the class is
guide-addressable (a blocking rule is added to the revision).

Related, and evidence for the owner directive: at **deck146 s29** the pilot wrote the illegal
`BLOCKS: B1:A1, B1:A4` on its first line, caught it in its own post-answer text (*"Wait, I cannot
assign the same blocker (B1) to two attackers"*), and re-committed `BLOCKS: B1:A1`. The engine
took the LAST label. **Post-answer scratch text is currently the only mechanism that caught an
illegal block at this seat.**

---

## (i1) N-105a POISON RENDERED: **PASS, 100% coverage at both seats**

Game `152v105`, log `1787147368-...-vs-ai_baka_deck105.jsonl`. `gameend`:
`{'won': True, 'my_life': 20, 'opp_life': 0, 'turn': 12}`.

**Coverage FRACTION, my seat: 25 / 25 = 100%.** The first prompt carrying a poison state line is
seq12; **every one of the 25 prompts from seq12 to the end carries `Poison counters (you): N of
10 ...`**, zero misses. Opponent seat: 19 of 20 prompts, the gap being pre-first-counter.

Verbatim, my seat:
```
Poison counters (you): 4 of 10 - you LOSE the game at 10 poison counters, whatever the life total is; 6 more end it. Poison is not life: it does not reset ...
Poison counters (opponent): 1 of 10 - the opponent LOSES the game at 10 poison counters ...
```

Narration is DISTINCT from ordinary damage, and both halves of the wave-32 silence are dead:
```
- Infect damage: 2 from Ichor Rats to you - dealt as POISON COUNTERS, not life loss: no life was lost (see the Poison line)
- Poison: you take 1 poison counter - now 1 of 10 (you LOSE at 10 poison counters; 9 more end it)
```
The second line is the **non-combat** case (Ichor Rats' ETB, `alterpoison`) -- the wave-32
four-silent-events class is confirmed dead at this seat. The per-attacker tag also reaches the
combat enumerator:
`A1. Ichor Rats (2/1) deals 2 [infect] [its combat damage to YOU is dealt as POISON COUNTERS, not life loss - your life total will not move and blocking is the only thing ...]`.

**The wave-32 gate is discharged**: N-152i predicted "a loss with `won:false` and `my_life > 10`
must not recur without the number having been visible". The loss did not recur at all -- the seat
won 20-0 on turn 12 having taken 4 poison, against the deck that beat it at 23 life last wave.

## (i2) N-105b HONEST BLOCKER FORECAST: **PASS, 1/1 infect window, and it CHANGED the block**

**Coverage: 1 of 1 infect blocker windows partitioned by damage type; 0 emissions of
`you would be at N - NOT lethal` on an infect window.** My seat, seq32 t11 verbatim:

```
Unblocked, these attackers put up to 2 POISON COUNTERS on you - you would be at 6 of 10 poison - that is not yet 10, but poison counters never reset and nothing here removes them, so every counter you take is permanent progress toward losing.
```

**The pilot obeyed it and quoted its arithmetic back** (seq32 reply, verbatim):

> *"Since it has **Infect**, this damage translates directly to **2 poison counters**. You
> currently have 4 poison counters; taking 2 more would bring you to 6/10. ... Taking the hit
> would leave you at 6/10 poison with no way to remove them ... Not blocking would be a
> significant risk toward losing via poison."*

It then chump-blocked a 2/1 with **Katilda, a legendary lord**, at 20 life -- the exact inversion
of the wave-32 reference repro (deck36 s25 t12: *"the total damage (5) is not lethal (I would drop
to 15) ... I declare no blocks"* -> dead at 20 life). The trade was correct and the game was won
two turns later.

**NEGATIVE CONTROL intact.** The same seam on non-infect windows is unchanged: eight
`Your life: N. Unblocked, these attackers deal up to X - you would be at Y - NOT lethal` lines
across my other five games, plus the LETHAL variant firing correctly twice
(`you would be at -2 - LETHAL if it all connects: block enough to survive`, deck146 s29 at 8 life;
and at deck105's own seat at 6 life). i1 and i2 do not leak into ordinary combat.

---

## (i9) PREGAME HAND-ONLY: **PASS on the render, FAILED at the DECISION -- two false mulligans**

**Render: clean.** All eight pregame prompts (6 keeps/mulligans + 2 bottoms) contain **zero**
battlefield facts -- no `Mana available:`, no `Your battlefield`, no `Opponent battlefield`. Zero
`Mana available: 0 total` echoes in any mulligan reply. The engine-computed header fires on 16/16
hand asks:

```
Your hand (7 cards), counted by the engine: 3 lands, 4 spells.
Mana sources among those lands, counted by the engine: {W} 3, {G} 1. ...
Cheapest spell in this hand: Intrepid Adversary {1}{w} (mana value 2).
Playing every land in this hand would cover the cost of: Intrepid Adversary {1}{w}, Fateful Absence {1}{w}, Ranger Class {1}{g}, Ranger Class {1}{g}.
```

**And the seat mulliganed against it twice.** Three mulligans in six games; **two of the three are
FALSE**, both on the COLOUR axis, and both cite a criterion the DEPLOYED GUIDE gave them.

**vs158 seq1** -- hand `Branchloft Pathway; Ranger Class; Forest; Katilda; Barkchannel Pathway;
Fateful Absence; Ranger Class`. Header: `3 lands, 4 spells`, `{G} 3`, `Playing every land in this
hand would cover the cost of: Ranger Class {1}{g}, Ranger Class {1}{g}`. Reply verbatim:

> *"This hand has three green mana sources and two copies of Ranger Class, but critically lacks a
> source of White mana. ... Without a White source, the hand is effectively dead for the first few
> turns"*

**vs105 seq1** -- hand `Plains; Deserted Beach; Luminarch Aspirant; Plains; Fateful Absence;
Fateful Absence; Ranger Class`. Header: `{W} 2, {U} 1`, `Playing every land in this hand would
cover the cost of: Luminarch Aspirant {1}{w}, Fateful Absence {1}{w}, Fateful Absence {1}{w}`.
Reply verbatim:

> *"Having no Green mana sources in a 7-card hand with 4 spells is a fatal flaw for this specific
> deck"*

**The diagnostic is HL6's, one rung over.** This is not membership denial (the pilot never disputes
that a Forest makes green); it is **COVERAGE denial** -- the reply RECITES the engine's coverage
list and then declares the hand "effectively dead" / "a fatal flaw" anyway. The count escalation
reached it and did not bind, because the pilot's disqualifier is about the deck's PLAN, not about
the hand's contents.

**And the guide authored the disqualifier.** Deployed line 105:
*"Mulligan only hands with no creatures at all, or one that cannot make GW mana."* A mono-green or
mono-white hand literally "cannot make GW mana". Both mulligans are the guide being obeyed.

**HL7.3 satisfiability check, which the wave-32 revision never ran.** 25 lands: 14 make green, 15
make white, **and only the 4 Overgrown Farmlands make both**. A three-land opener is mono-coloured
roughly two times in five. **The guide shipped a keep criterion that fails on ~40% of its own
three-land openers.** That is the deck105-wave-32 shape (*"mulliganing for a hand with at least one
1-drop"* in a deck with no 1-drops) with the impossible criterion supplied by the guide instead of
invented by the pilot -- which is worse, because the pilot is behaving correctly.

**COST: the vs158 loss.** Two mulligans -> keep 7 and bottom 2 -> a **five-card** hand, with
`Sigarda, Champion of Light` (the guide's own "best card in the deck") and a Brutal Cathar
bottomed at seq4. The game: one creature on the battlefield through turn 5, six casts total, three
attacker asks in 36 decisions, **zero creatures on my battlefield from seq30 (t12) onward**, dead
at -3 on turn 13. The first decisively guide-addressable loss component this seat has produced in
three corpora, and the #1 item of the revision.

---

## REVISED-GUIDE VALIDATION (wave-32 revision, first corpus)

### R1 -- DEPLOY FLOOR + THE SAFETY CLAUSE: **FIRED. Both wave-32 survivors are dead.**

HL9's cast-nothing SPLIT, with the deck's instant count stated first (HL10): deck152 runs **two
instants** (2x Fateful Absence) and two enchantments, so the split is informative, not the raw
rate.

| corpus | casting decisions | `Cast nothing` | (a) instant-hold (sanctioned) | (b) AGAINST the floor | lowest life at a (b) |
|---|---|---|---|---|---|
| wave-31 | 49 | 7 | 5 | **2** | **1 life** |
| wave-32 | 56 | 15 | 13 | **2** | 11 life |
| wave-33 | **44** | **4** | **3** | **1** | **18 life** |

Raw rate 26.8% -> **9.1%**, against-floor 2 -> **1**, and the survivor is not a floor failure at
all (below). Specifically:

- **The SAFETY argument the wave-32 revision was written against did not recur once.** Zero
  declines reasoning *"they cannot punish me this turn"*. The inverted clause (*"a turn where they
  cannot punish you is the CHEAPEST turn you will ever get to add a body"*) is FIRED.
- **The lethal-on-board exception (b) was never needed** -- 0 uses, 0 abuses.
- The three sanctioned holds are deck116 s13/s15/s19, all `Fateful Absence`-only windows at 20
  life with no targetable opponent creature. Exactly the hold Rule #3(a) licenses.

**The one against-floor decline is NOT a floor failure -- it is a Rule #1 failure.**
**deck36 seq14 t8, life 18**, `Mana available: 2 total (two untapped sources ... colours you can
make: {g})`, offer `1. Cast Ranger Class {1}{g}`. Reply verbatim:

> *"I have only {G} mana available (from the untapped Barkchannel Pathway). Ranger Class costs
> {1}{G} (one generic and one green) ... Neither can be cast with the current mana."*

Two untapped G sources; `{1}{G}` is trivially payable. The pilot declined an OFFERED, LISTED,
payable cast. The next decision (**seq15, priority**) spends its whole reply on the same question
and names it exactly:

> *"Ranger Class is `{1}{G}`, so I need 1 generic and 1 Green. **Do I have generic mana?**"*
> ... *"looking closely at the 'Mana available' line: '2 total (two untapped sources, colours you
> can make: {g})'. This implies I have two sources but they only make Green?"*

**THE SUCCESSOR BELIEF, NAMED: the pilot treats GENERIC as a colour that the `colours you can
make:` set does not contain.** HL3.2's prediction firing on schedule -- the wave-32 false-decline
class is dead (i12's count line works) and the residual tax rehearses a NEW belief one seam over.
It is also a **second, cross-seat witness**: deck105's wave-32 review recorded the identical belief
(*"generic-cost payability off an off-colour land -- re-derived from scratch ELEVEN times inside
ONE reply"*). Two seats, two waves, one sentence. The revision installs the falsity-fact.

Cost: two burned windows at t8 (the deploy AND the level-up), in a game won on adjudication. seq15
was rescued by the reply's own post-answer self-correction (`0 (pass)` -> `2 (level Counter with
Ranger Class)`).

### R2 -- WEREWOLF SECTION (frequency claim removed in wave-32): **PASS, and half of it is now SUPERSEDED**

Pre-ship frequency grep over the deployed guide
(`rarely|often|usually|regularly|typically|expect|most of the time|by default|almost always|seldom`):
two hits, **neither inside a mechanic description** -- line 47 *"Pay as often as you can afford"*
(an instruction about a cost) and line 108 *"an empty board loses these far more often than walking
into removal does"* (a comparison between two lines of play). HL4 clean.

The mechanics teach fired. `Day`/`Night` designations render on the battlefield line (`Day {0}`
100x, `Night {0}` 42x in my games) and are correctly excluded from the creature count. A flip
happened and was piloted well: `Moonrage Brute (3/3) [first strike, nightbound]` attacked at
deck158 s29 and traded into `Snarling Warg (4/4)`, first strike read correctly on both sides. Flip
rate this corpus: 1 of 4 games with a werewolf on my battlefield; combined across three corpora
**6 of 13**. Per HL4 that number stays in findings.md and never enters the guide.

**SUPERSEDED (HL3.3), deleted in the same revision that records it**: the paragraph *"READ THE
CURRENT NUMBERS, NOT ANY '(printed X/Y)' TAG ... On a transformed werewolf the printed pair is its
OTHER FACE's number"*. Its two quoted strings are now zero-hit and its central claim is FALSE ABOUT
THE PROMPT -- a transformed werewolf now carries **no tag at all**. The general half (first pair is
the real size) is kept and re-aimed at the tags that do fire.

### R3 -- FLIP SIDE, MENU-LABEL-SCOPED PROHIBITION: **PASS, 33/33**

The wave-32 generalization (from *"A Pathway's blue back face"* to *"NEVER SPEND A DECISION ON
'FLIP SIDE' - ON ANY CARD, EVER"*) is the guide-side twin of HL5, validated at the surface level
the failure lived on. **33 `Flip Side` option-lines offered across the six games; 0 taken.**
Wave-32 took two (deck105 s28 `choice 3`, s31 `choice 2`, both `Flip Side with Tovolar's
Huntmaster`) against a Pathway-scoped rule. The option rode four card classes again this corpus
(Brutal Cathar, Tovolar's Huntmaster, the Pathways, MDFC lands) and the menu-label-scoped rule
covered all of them. **KEEP BYTE-FOR-BYTE.** The wave's cleanest guide-side result.

### R4 -- ATTACK FLOOR: **HOLDS, 22/23 full declarations**

23 attacker asks, **zero mixed-form drops, zero parse failures, zero fallbacks**, and in 22 of 23
the seat declared **every** creature on the eligibility list -- including an 11-of-11 alpha strike
(deck139 s29) and a 5-of-5 (s24). Escalation intact: deck105 s10 (1/1) -> s15 (2/2) -> s21 (3/3)
-> s28 (4/4) -> s35 (4/4).

The one decline, **deck139 seq8 t5**: one eligible attacker, `ATTACK: no attackers`. Same matchup
and near-identical shape to wave-32's single decline (deck139 s9, a 1/1 into `Arboreal Grazer (0/3)
[reach]`). Two corpora, one deck, one shape, both games WON -- the wording corner wave-32 flagged,
still one instance in 23. Not revised; a third witness would promote it.

### R5 -- REMOVAL DISCIPLINE / Rule #1 removal half: **PASS**

Fateful Absence held at instant speed through three windows where it was the only offer, and spent
on impact when spent: deck116 s24/s25 on `Thragtusk (5/3)` (a re-commit from `Cast nothing`),
deck139 s12/s13 on `Dryad of the Ilysian Grove`, deck36 s20/s21 on the flier `Glaze Fiend`. Zero
off-list answers, zero invented Cast numbers.

### R6 -- MULLIGAN: **FAILED, 2 false mulligans of 3, one of them the vs158 loss.** See (i9).

### R7 -- LORDS / CLASS / KATILDA STEADY STATE: **CLEAN, plus one new capability proven**

Ranger Class cast in 5 of 6 games, two copies coexisting and levelling independently with no
cross-copy bleed; Level 2 reached in 4 games (deck36 s15, deck105 s24, deck116 s32/s43, deck139),
L3 not reached. Intrepid Adversary's valor menu answered sanely both directions (`add 1 counter`
with spare mana at deck146 s13/s28, `don't add any counter` on a bare board at deck158 s11, deck36
s7). **Katilda was proven as a MANA SOURCE for the first time** -- deck116 s29's `Mana available: 8
total` over 7 lands, and the Sigarda cast that followed used her. The guide already teaches this
("she lets you double-spell"); it is now witnessed.

---

## THE MISGIVING FAMILY (HL10: distinct BELIEFS, not instances)

One regex (`cannot cast|cannot afford|not enough mana|can't cast|do not have enough|insufficient
mana`) over all 211 replies: **85 hits across 37 replies (17.5% of replies)**. They collapse to
exactly **THREE distinct beliefs**:

1. **"Generic mana is a colour I may not have"** (deck36 s14/s15, deck116 s10/s25/s32) -- the
   successor belief, R1. **Cost: one false decline of an offered payable cast, one burned
   activation window.** The only one of the three that produced a wrong action.
2. **"A mono-coloured hand cannot execute my deck"** (deck158 s1/s2, deck105 s1, deck139 s2) -- the
   mulligan belief, i9/R6. **Cost: a game.**
3. **Correct unpayability reasoning** (deck116 s13/s15/s19/s20/s24, deck146 s4) -- the pilot
   correctly concluding it cannot pay. Not a defect; the instrument working.

Free cross-check: those 85 re-derivations produced **exactly ONE** false decline of an offered Cast
(belief 1), against wave-32's zero. The instance count is noise; the belief count sizes the
revision, and it puts two clauses in the guide.

**CONTRADICTION VERBALIZATIONS: 2, both benign.** deck116 s20 (*"Teferi is not in the list"*) is
the pilot **correctly obeying the completeness guarantee** on a correct suppression -- it quotes
the core prompt back and passes, in 1,300 characters, with no spiral. Compare wave-32's
kilobyte-scale *"This is a contradiction"* spirals at the same seam. deck36 s15 is belief 1. **The
N-152c/N-152f contradiction class is dead at this seat.**

---

## THE LOSSES

### deck158 (LOSS, -3 vs 14, t13) -- **THE FINDING OF THE CORPUS: a mulligan loss.**

Fully decomposed in i9/R6. Chain: guide criterion *"cannot make GW mana"* -> two false mulligans ->
keep 5 -> Sigarda bottomed -> 6 casts in 13 turns -> board empty from t12 -> dead.
**Guide-addressable, and the guide is the cause.** The last five decisions (s32-s36) are Teferi
sub-menus resolved on an empty board -- the pilot playing correctly with nothing to play.

### deck146 (LOSS, -1 vs 12, t16) -- **STRUCTURAL, correctly forecast, no guide component.**

Nine casts across 16 turns, an empty hand at the end, and **one attacker ask in 29 decisions** --
the identical ceiling signature this matchup produced in wave-32 (`Triumphant Adventurer (1/1)
[first strike, deathtouch]` plus exile removal keeps `hasLegalAttacker` false). The killing window,
s29 t16 at 8 life: `A2. Angel #1 (4/4) [flying]`, `A3. Angel #2 (4/4) [flying]` against
`B1. Intrepid Adversary (4/2)` -- one blocker, 10 damage, no reach in the maindeck. The forecast
said `you would be at -2 - LETHAL` and it was right; no legal set of blocks survives. Matchup, not
pilot.

### deck116 (LOSS by cap adjudication, 15 vs 21, t15) -- **CLOCK, not decisions.**

45 decisions in 3000s, 10 casts, 6 attacker asks all fully declared, Sigarda deployed and
attacking, Teferi deployed and used both ways. The opponent gained above its starting total
(Kitchen Finks persist + Glimmerpost) and the cap fell with the board alive. The seat's own latency
is the proximate cause: **18 decisions over 100s in this corpus**, and this game carried the two
largest replies at the seat (s39 reveal, 15,082 chars / 187s; s38 priority, 11,573 chars). Routed
to the cap/latency docket, not to the guide.

**Summary: 1 of 3 losses has a decisive guide-addressable component (wave-32: 0), and it is a
guide-authored criterion, not a pilot error.** That is what the revision fixes.

---

## COMMIT-FAILURE COUNTER (the cross-seat instrument)

Counted fields, all 211 decisions at this seat, quoted verbatim from the translog:

- **replies that continue past their own `PLAN:` line (`post_plan_overrun > 0`): 21 / 211 = 10.0%.**
  Mean overrun **2,173** chars, median 1,081, max **13,243** (deck116 s39, a `reveal`).
- **of those, RETRACTED (`commit_retracted: true`): 0.**
- **of those, FATAL: 0.**
- `pruned_pairs` non-empty: 1 (the `all_assignments_illegal` at deck36 s8, see i8).
- Replies carrying no `PLAN:` line at all: **5 / 211** -- protocol-compliant under the interim
  `f46dd58ee`, and correctly excluded from `post_plan_overrun`.

**This seat contributes ZERO fatal post-PLAN retractions to the promotion trigger.** Per HL10's
spec the trigger needs two seats with a fatal overrun+retraction; deck152 is not one of them.

**Owner-directive evidence (b) -- does post-answer self-correction carry decision VALUE?**
Measured as last-label-differs re-commits: **11 / 211 = 5.2%** (campaign-wide 4.2%). The engine
took the LAST label in all eleven. Decomposed:

| seat window | first -> last | value |
|---|---|---|
| deck146 s29 blockers | `B1:A1, B1:A4` (ILLEGAL - one blocker on two attackers) -> `B1:A1` | **legality rescue** |
| deck116 s24 ask | `Cast nothing` -> `Cast Fateful Absence` (killed `Thragtusk (5/3)`) | high |
| deck36 s26 ask | `Cast nothing` -> `Cast Teferi` (resolved, piloted) | high |
| deck36 s15 priority | `pass` -> `level Ranger Class` | rescued the belief-1 false decline |
| deck105 s24, deck116 s32 priority | `pass` -> `level Ranger Class` | free value, guide-sanctioned |
| deck116 s38 priority | `pass` -> `-2: reveal top 3` | mild positive |
| deck146 s28 ask | `don't add any counter` -> `add 1 counter` | positive (spare mana, guide rule) |
| deck146 s21 blockers | `B1:A1` -> `none` (10 life, 1 damage) | neutral/defensible |
| deck139 s3, s10 ask | land swaps (Barkchannel over Hengegate; Beach over Plains) | mild positive |

**9 of 11 improved the decision, 1 converted an ILLEGAL answer into a legal one, 0 were harmful, 0
retracted.** At this seat the post-answer block is net-positive and materially so. That is not an
argument against the owner's ruling -- it is the size of what native pre-answer reasoning must
replace. See general-suggestions.md for the two conditions I can evidence.

---

## OTHER LEDGER ITEMS WITNESSED

- **N-152a (MDFC back-face land placement)**: still unofferable, as expected. Carried.
- **N-152h (MDFC-spell Flip Side wording, third path)**: **UNEXERCISED at this seat** -- the string
  `the other face appears there as an alternative-cost cast` has 0 hits in my six games. deck152
  draws no MDFC spells; cite only from a seat that does.
- **N-158k (Feed the Swarm cost absent on the target menu)**: exercised AGAINST me and correct on
  the cast line -- deck158 s21 reads
  `1. Cast Feed the Swarm {1}{b} - legal targets right now: Moonrage Brute (costs you 0 life)`.
  I did not observe the COMMIT-seat menu. No new occurrence to add.
- **Phase-header contradiction WATCH** (deck36's wave-32 flag, witnessed at deck152 s15/s16 last
  wave): **ZERO occurrences at this seat this corpus.** No second witness from me; the watch does
  not promote on my evidence.
- **h5 auto-pass**: 26 priority asks, **0 toggle-ONLY windows.** Third consecutive clean corpus.
- **Plan-field pollution (HL10)**: 34 of 211 prompts (16.1%) carry a `YOUR PLAN` block over 400
  chars, none over ~800. Well below deck36's wave-32 39%. Not a driver here.
- **i13 prompt cost**: p90 15,769 -> 13,678 (-13%), max 23,775 -> 25,008 (+5%). Tail flat, body
  down. No degradation.

---

## ROTATION RECOMMENDATION

**HOLD for wave-34, exit still projected wave-34 -- but the projection is now CONDITIONAL on the
mulligan section, not on the render lane.**

- Both wave-33 engine gates this seat owned **CLOSED**: N-152f arrival-traced on the broken branch
  with a negative-control ladder and zero false suppressions; N-152d layer 2 at zero false tags
  with live positive controls. Neither needs another corpus.
- The HL1 card-fact audit **PASSES for the second consecutive corpus**, so the exit's hard gate is
  discharged.
- The infect STOPGAP retires this wave on its own removal condition (quoted string dead, number
  rendered), so the guide no longer carries a stopgap. That was wave-32's stated blocker to
  freezing and it is gone.
- **But the revision this wave touches the MULLIGAN section for the first time in the seat's
  history, and that section just cost a game.** A guide revised this wave cannot be frozen this
  wave. Wave-34 must show: zero false mulligans on the colour axis, and zero false declines of an
  offered payable cast (the generic-pip clause).
- Teach record across three guided corpora is stable and improving: attack floor 27/27 -> 24/25 ->
  22/23; removal discipline clean x3; lords/Class clean x3; deploy-floor against-floor count
  2 -> 2 -> 1 with the safety pathology eliminated; Flip Side 0/33 after the menu-label rescope.
  **Projected exit: wave-34** if the mulligan floor and the generic-pip clause validate.
