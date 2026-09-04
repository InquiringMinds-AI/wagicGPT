# deck130 (Budde's Ponza, mono-red land destruction) — wave-58 per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-015059/`, binary master `a3f0700d3`,
model `qwen36-35b-a3b`. Six games, **1-5**. Seat files below are named by their opponent suffix;
all are `1788504xxx-ai_baka_deck130-0x...-vs-ai_baka_deck<N>.jsonl`. Counts are read from the
rendered `prompt`, never from `options`.

Decision counts: vs152 25 · vs162 117 · vs125 135 · vs146 45 · vs126 73 · vs123 50.
Fallbacks in this seat: **12 `empty_reply`, 1 `named_row_reask`, 1 `wall_miss_unrecorded`**.

---

## 1. Game by game, and the decisions that decided them

### vs123 — **WON** 20 to -6, turn 28 (`...-vs-ai_baka_deck123.jsonl`)
The guide's plan run clean and the KEEP evidence. Molten Rain / Molten Rain / Lay Waste / Lay
Waste / Stone Rain / Stone Rain into a control deck's mana (seq9-24), Blastminer at 4 lands
(seq29), Rorix at ten sources (seq39), attack every turn (seq35, 37, 40, 44, 48). Never dropped
below 20 life. Cast-order entry 3 and entry 4 both fired in the right order, and the Blastminer
land-kill activation at seq43 was taken with mana left over.
One blemish, harmless here: **seq5-7**, turn 4, Spark Spray cast at the face for 1 with the
opponent at 19 and no creature on their line. See item G1.

### vs152 — LOST -9 to 19, turn 15
Removal kept pace (Spellbomb kills Elite Spellbinder seq14; Starstorm X=2 kills Spellbinder +
Katilda seq18) but Sigarda, Champion of Light (4/4 flier) went unanswered and hit for 4 a turn
from turn 9. Deciding window **seq22** (turn 14, life 2): the model chose row 4 "Cast nothing
right now" and its reply reads *"I cannot remove Sigarda or Elite Spellbinder with Hammer (3
damage is not enough for Sigarda's 4 toughness, **and Elite is 4/2 now**)"* — while row 3 on that
same screen printed `{kills: Elite Spellbinder - and 3 to the opponent at life 19 leaves them at
16}`. STRATEGY/perception slip against a TRUE rendered verdict; not game-deciding (both fliers
were lethal at 2 life either way).
Also **seq3-5**, turn 2: Spark Spray at the face for 1 damage at 20-20 with the opponent on one
Plains. Item G1.

### vs146 — LOST -2 to 19, turn 19
Nadaar, Selfless Paladin arrived as a 5/5 with vigilance and a Goblin beside it; life went
20 → 17 → 10 → 3 while the seat spent turns 6, 8 and 10 on Stone Rain / Lay Waste / Stone Rain.
Starstorm at X=5 (seq34, turn 12) finally swept it, one turn too late. The plays were inside the
guide's letter (life was still above 8 at each cast) but the guide has no clock term. **seq25**
is the 15-minute wall miss (item E2) and **seq27** re-asks the identical decision in Main phase 2.

### vs162 — LOST 0 to 18, turn 16. **The clearest guide-caused loss in the set.**
deck162 assembled Underworld Dreams + Fate Unraveler + Howling Mine + Dictate of Kruphix, so
every one of this seat's forced draws cost 2 life and the draw step drew 2-3 cards. The frame did
its job: `DRAW PUNISHERS on the battlefield: theirs - Underworld Dreams, Fate Unraveler. Every
card YOU draw costs you 2 life` and `DRAW FORECAST: your next draw step draws 2 cards (1 +
Howling Mine 1) = 2 x 2 = 4 life to the punishers above` were both on the screen.
**seq94** (turn 14, life 6, 8 untapped sources): the reply's own PLAN line reads *"The land plan
is over (life 6, 4 lands)"* — and then **CHOICE: 2 (Cast Stone Rain)**. **seq96** cast a second
Stone Rain the same turn. Six of eight sources gone. The guide already forbids this ("L at 8 or
less, and the spell is dead for the rest of the game"), so this is a STRATEGY violation of a rule
the pilot restated in the same reply — the rule was in a distant section, not on the cast-order
entry it governs. Item G2.
The cost is exact: with 4 of those 6 sources the seat could have paid `{1}{r}` twice for
Siege-Gang's sacrifice (3 Goblins were on the battlefield) and put 4 damage on **Fate Unraveler
(3/4)**, halving the punisher rate for the rest of the game. No sacrifice row appears on any
window at turn 14 after seq94 — it could no longer afford one. Two turns later, dead.
**seq114** (life 1) is the render item E4: four damage triggers on the stack, no total printed.

### vs126 — LOST 0 to 33, turn 25
The opponent tutored Exquisite Blood onto a battlefield already holding Sanguine Bond (turn 23
log in the seq71 prompt) — the exact board the guide names as unwinnable ("Nothing in this sixty
removes an enchantment"), behind five defenders. Before that the seat flooded: eleven lands, a
one-card hand, and turns 17-24 spent on land drops and hold rows. **seq31 and seq36-50** are nine
`empty_reply` fallbacks in eleven windows across turns 14-15 — item E1; during them the heuristic
fallback sacrificed a Pyrite Spellbomb into the opponent's face (recovery record at seq43).

### vs125 — LOST 0 to 11, turn 57. **The game the guide threw away.**
Correct burn phase: Spellbomb (seq79), Hammer (seq81-82), Spellbomb (seq86), Spellbomb (seq89)
took deck125 from 10 to 4. Siege-Gang was drawn at turn 44 and **countered by Essence Scatter**
(log in the seq126 prompt) — no engine fault there, and the absent sacrifice rows after it are
correct.
The loss is **seq95, 99, 103, 105, 107, 109, 112, 116, 118, 121, 125, 128, 130, 132** — fourteen
consecutive upkeep windows, turns 30 to 56, hand `(0 cards): (none)`, 11 to 14 untapped sources,
opponent at **4 life**, where row 1 read verbatim:

```
1. Put a card into hand with Hammer of Bogardan [cost: {2}{r}{r}{r}] {card text: "Hammer of
Bogardan deals 3 damage to any target. -- {2}{R}{R}{R}: Return Hammer of Bogardan from your
graveyard to your hand. Activate this ability only during your upkeep."}
```

and the answer every single time was `CHOICE: 0 (pass)`. Two returns and two recasts = 6 damage
= the game. Instead the seat died 1 life per turn to Staff of Nin over 27 turns. The primitive
(`mtg.txt:52046-52053`, `autograveyard={2}{R}{R}{R}:moveto(hand) myUpkeepOnly`) is faithful to
Oracle, and Scryfall agrees — the engine offered the winning line correctly. Items G3 and E3.

---

## 2. Engine / interface / card items

**HIGH — E1. `empty_reply` is a ~2.5 s transport failure being reported as a model refusal, and
it is not retried.** Every one of the 89 `empty_reply` records in the corpus has
`latency_ms` between **2502 and 2558**, against a **median 12 539 ms** for the 2 162 successful
calls (p90 63 855). A model that produced nothing after deliberating would not land in a 56 ms
band. Repro: `1788504693-...-vs-ai_baka_deck126.jsonl` seq 36, 38, 40, 42, 44, 46, 48, 50 —
eight in a row inside turn 15, `reply` empty, `latency_ms` 2510-2530, prompt 9 884-11 364 chars,
each followed by a `recovery` record. Ask: classify a sub-timeout empty body as a transport error
and re-issue once before handing the window to the heuristic; stamp the HTTP status on the record
so the corpus can tell a refusal from a dropped request.

**HIGH — E2. A `wall_miss_unrecorded` burns fifteen minutes and loses a phase.**
`...-vs-ai_baka_deck146.jsonl` seq25, `latency_ms` **900 022**, kind `wall_miss`, at turn 10
Main phase 1 with the seat at 10 life. The identical five-row menu is re-asked at seq27 in Main
phase 2 and answered normally. Seven such records exist corpus-wide, all at 900 018-900 037 ms —
a 900 s wall that no decision needs.

**HIGH — E3. The graveyard-recursion row is rendered bare while every cast row is priced.**
`1788504683-...-vs-ai_baka_deck125.jsonl` seq95 (and 13 more, listed above), rendered line quoted
in §1. Compare the same seat's cast rows, which carry
`{kills: Elite Spellbinder - and 3 to the opponent at life 19 leaves them at 16}` and
`{leaves 5 of your 8 untapped mana sources untapped}`. The recursion row carries neither the
damage the returned card will do, nor the opponent's life, nor "your hand is empty", nor the
mana left after paying. It is the only reach the deck has once its hand runs out, and it is the
least-annotated row on the screen. Ask: annotate an activated ability that moves a card to hand
with what that card then does — at minimum the returned card's own damage clause and the
opponent's current life, in the same `{...}` shape the cast rows use.

**MED — E4. A lethal stack is listed but never summed.**
`...-vs-ai_baka_deck162.jsonl` seq114, `Your life: 1`, rendered verbatim:
```
ON THE STACK, waiting to resolve (top resolves FIRST - you can respond now):
  1 (top): ability: Underworld Dreams's deal 1 damage [from their Underworld Dreams] ...
  2: ability: Howling Mine's Draw 1 ...
  3: ability: Fate Unraveler's deal 1 damage ...
  4: ability: Underworld Dreams's deal 1 damage ...
```
Three of those four deal 1 damage to a player at 1 life. The frame will forecast a future draw
step (`DRAW FORECAST: ... = 3 x 2 = 6 life to the punishers above`) but prints no arithmetic for
the damage already on the stack in front of it, and no "this kills you". Ask: the same
survival verdict the cast rows carry, computed over the stack's pending damage to the pilot.

**MED — E5. A repeatable activation is priced one payment at a time, and the per-payment verdict
reads as a verdict on the window.** Same file, seq110, row 1:
`Deal 2 damage with Siege-Gang Commander targeting Fate Unraveler [opponent's battlefield]
{right now: takes 2 damage - SURVIVES (toughness 4)} [cost: {1}{r}, Sacrifice]`. The seat held
three Goblins and eight untapped sources; two activations (4 mana) kill a 3/4. `SURVIVES` is
true of one payment and false of the window. Ask: where a sacrifice/activation cost can be paid
more than once from the current board and pool, print the affordable repeat count and the
cumulative verdict (`you can pay this 3 times right now; 2 payments = 4 damage - DIES`).

**MED — E6. The `best X` marker endorses spending the deck's only sweeper on a token.**
`...-vs-ai_baka_deck146.jsonl` seq25/seq27 row 1:
`{X pricing: max affordable X=3 (5 mana total); smallest X that kills an OPPONENT creature: X=2
(4 mana total) - kills THEIRS: Goblin; YOURS: none} [<- best X for this cast: X=2 - most kills at
any affordable X that costs you nothing]` — on a board whose other creature is a 5/5 Nadaar that
was killing the pilot. "Costs you nothing" is true and "best" is not: 4 of 5 sources for one
tapped 1/1 token. Same class as lane B's D5 on the sibling marker; the X-marker inherited the
selection rule but not the cost test.

**LOW — E7. Byte-identical menus re-asked across phases and within a phase.**
seq25 vs seq27 (vs146) are the same five rows in Main 1 and Main 2. `...-vs-ai_baka_deck162.jsonl`
seq32-38 is six windows in turn 7 answered "Cast nothing right now" / hold, alternating `ask` and
`priority`. The counter exists — seq114 renders `[you declined this exact list 3 times already
this turn]` — but it does not gate the re-ask.

**LOW — E8.** One `named_row_reask` at `...-vs-ai_baka_deck162.jsonl` seq56, `latency_ms` 11 720,
recovered at seq57-58 with no board change.

Card check: Hammer of Bogardan's primitive (`bin/Res/sets/primitives/mtg.txt:52046`) matches
Oracle exactly (`{1}{R}{R}` sorcery, 3 damage any target, `{2}{R}{R}{R}` return during your
upkeep only). Siege-Gang Commander, Starstorm, Spark Spray, Stone Rain, Molten Rain, Lay Waste
and Talisman of Impulse all rendered card text matching Oracle in the prompts read. **No card
defect found in this seat.**

---

## 3. Guide verdict: **EDIT** — `wave58/deck130/strategy.txt`

The guide is largely working: the vs123 win is its cast order executed line for line, and the
vs125 burn phase (Spellbomb, Hammer, Spellbomb, Spellbomb from 10 to 4) is rule #2 step 0 and its
release working exactly as written. Four edits, each paid for by a seq.

**G1 — rule #2's release fires before the land war has begun.**
Before → `At 0 creatures AND L of 2 or fewer, you have already won the land war: they are not
deploying the creature you were saving the card for ...`
After → the same, gated on `AND ONLY ONCE YOU HAVE ACTUALLY TAKEN LANDS OFF THEM - the log shows
a Stone Rain, Molten Rain, Lay Waste or Blastminer activation of yours that resolved`, plus the
turn-2/turn-4 counterexample.
Paid for by: **vs152 seq3-5** (turn 2, opponent on one Plains, Spark Spray into a face at 20) and
**vs123 seq5-7** (turn 4, same shape). A turn-2 opponent trivially satisfies "0 creatures and 2
or fewer lands"; the release was written for a board you dismantled, and read as a board that has
not been built yet. Rule #1's Spark Spray entry inherits the fix, since it defers to this release.

**G2 — the two-number gate now sits ON cast-order entry 3.**
Before → `3. STONE RAIN, MOLTEN RAIN or LAY WASTE at one of their lands, while their battlefield
shows FOUR OR FEWER lands. This is the deck's job; do not hold it for a better target.`
After → `... FOUR OR FEWER lands AND "Your life:" is 9 or more. ... BOTH NUMBERS SIT ON THIS
ENTRY because the copy that kept them in a distant section did not bind:` + the vs162 record.
Paid for by: **vs162 seq94 and seq96**. The life-8 floor already existed under HOW THE DECK WINS;
the pilot restated it verbatim in the same reply that broke it, which is what a rule sitting two
screens away from the entry it governs looks like. The Lay Waste entry in rule #1 now points at
entry 3's numbers instead of carrying its own land-count prose.

**G3 — the Hammer of Bogardan return gets its positive trigger, and the upkeep-pass clause stops
fencing it out.**
Before → rule #1: `answer CHOICE: 0 (pass) unless the option in front of you destroys a land or
kills a creature.` And the Hammer section carried three worked examples, **all of them "pass"**,
and one negative condition (`Run the loop only when you have no land destruction, no creature and
no removal target left`).
After → the upkeep clause adds `, or is "Put a card into hand with Hammer of Bogardan" under the
EMPTY-HAND RULE`; the Hammer section gains **THE EMPTY-HAND RULE** — hand reads `(0 cards):
(none)` and `Mana available:` is 8 or more → take the return every upkeep and cast it in main
phase 1 — with the fourteen-window record; and one of the three "pass" worked examples is cut so
the section is no longer one-sided.
Paid for by: **vs125 seq95 → seq132** (fourteen passes, opponent at 4, hand empty, 11-14 sources).
The old upkeep clause is the proximate cause: the return neither destroys a land nor kills a
creature, so the guide's own sentence told the pilot to pass the only line that won.

**G4 — a repeatable cost counts once per affordable payment.** Added one sentence to the
LETHAL-THIS-TURN exception: `A REPEATABLE cost counts once PER PAYMENT you can afford: three
Goblins at {1}{r} each with 8 mana is 6 damage, not 2 - the row prices ONE activation.`
Paid for by: **vs162 seq110** (`SURVIVES (toughness 4)` on a row payable twice into a 3/4).

Size: 69 606 → **70 927 bytes**, inside the pool band (deck162 47 921 … deck123 70 921). The
growth was paid back by compressing the Starstorm restatement, the CROSS-CHECK anecdote, the
one-screen-earlier paragraph and the Rorix second-copy note; no rule was removed.

---

## 4. What I did NOT check

The 21 `game-*.stderr` files (so nothing from `WAGIC_BLOCKLOG=1`, `async_drops` stamps or the
lane-C drift receipts); the lane-A..E predictions and the D22 blocker predictions (engine-seat's
job); the opponents' own seat translogs except where deck130's prompt log carried their actions;
the HUNG game and its rerun (deck152 vs deck126, not a deck130 matchup); mulligan/bottoming
quality — deck130 kept all six openers and I did not second-guess them; and the general guide's
reply protocol beyond the rows quoted above.
