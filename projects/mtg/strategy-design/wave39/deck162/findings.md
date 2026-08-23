# deck162 "Draw and Die!" — Step-0 review (wave 39, GUIDELESS corpus)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-040744/`, 6 games, pilot `qwen36-35b-a3b`.
No `deck162_strategy.txt` exists in `Res/ai/baka/` — verified; this is a true Step-0 seat and the
corpus is a clean read of the qwen x core-prompt interaction with no guide confound.

**Corpus integrity**: 6 gameend records, 6 games, all reached a natural life-total kill (no draws, no
cap adjudications). 0 fallbacks across 121 decisions (0 `unparsed_reply`, 0 `empty_reply`, 0
`retracted_choice`, 0 `deferred_to_heuristic`). `commit_retracted` false on every record.

## 0. RECORD CORRECTION (read before anything else)

The reviewer brief states deck162 "went winless this corpus". **That is wrong.** From `results.tsv`
cross-checked against the six `gameend` records in deck162's own translogs:

| matchup | winner | deck162 final life | opp final life | turn |
|---|---|---|---|---|
| 146 v 162 | **162** | 4 | 0 | 12 |
| 126 v 162 | **162** | 16 | 0 | 13 |
| 125 v 162 | **162** | 19 | -2 | 15 |
| 36 v 162 | 36 | -9 | 17 | 8 |
| 139 v 162 | 139 | -9 | 2 | 12 |
| 152 v 162 | 152 | -5 | 13 | 10 |

**deck162 went 3/6.** Per the owner ruling the record is context only and is not the verdict — but a
false record in the brief would have driven this review toward "manufacture a posture rule to fill the
#1 slot" (wave-31 rung 0(b)). It did not; the #1 slot below is chosen off decision evidence.

## 1. PRESERVE LEDGER (unconditional at Step 0 — wave-32 HL9.1)

The corpus already does these correctly. The initial guide **must not legislate over them.**

| seam | measurement | verdict |
|---|---|---|
| Land drops | **36/36 windows, 0 skipped, 0 "Play no land right now"** | FROZEN. No land rule beyond a one-line dual-land tiebreaker. |
| Reply protocol | 121/121 decisions parsed; 0 fallbacks, 0 retractions | FROZEN. No format teaching. |
| Attacker declarations | **7/7 windows declared the full offered list** (no partial, no "no attackers") | FROZEN. Attack floor is a SHORT confirmatory line, never a loud #1-slot procedure (wave-31 rung 0(a)). |
| Ob Nixilis -2 target discipline | 5 windows. vs146 s20: 4 targets offered incl. its OWN Fate Unraveler, picked the opponent's Nadaar. vs125 s22/s23/s29: the ONLY offered target was its own Fate Unraveler — **passed all three times.** vs152 s13: picked Briarbridge Tracker over Sigarda (arguable, not wrong). | **4/4 correct declines / 1 correct pick.** FROZEN — do not write a targeting table. |
| Peer into the Abyss | 1/1 offered, cast, and the follow-up "target opponent / target controller" menu answered **target opponent** — the lethal branch. | FROZEN as a *decision*; the guide keeps only the never-target-yourself falsity fact, because n=1. |
| Mulligan | 6/6 kept a 7 with 3+ lands. Zero mulligans taken corpus-wide. | Behaviour correct; the mulligan SEAM is UNEXERCISED (see section 6). Guide gets a short floor, not a procedure. |

## 2. ABSENCE TABLE — offered / taken / arrived (wave-32 HL9.2 + HL11.3)

Counted over numbered `Cast <name>` option lines; "arrived" = a
`Your <name> resolved and entered the battlefield` line in `events`.

| card | offered | taken | arrived | routing of the gap |
|---|---|---|---|---|
| Howling Mine | 31 | 13 | 12 | offered > taken by design; the problem is *when* it was taken, not how often |
| **Fog Bank** | **21** | **1** | **1** | **offered > taken -> GUIDE LANE (deploy floor). The seat's second-largest leak.** |
| Liliana's Caress | 29 | 4 | 4 | offered > taken; low-impact, ranked below |
| Shield Sphere | 14 | 7 | 7 | offered > taken; a {0} permanent declined 7 times |
| **Underworld Dreams** | **6** | **3** | **2** | **offered > taken -> GUIDE LANE. The #1 leak (section 3).** 1 taken-but-not-arrived = countered (E-162a) |
| Fate Unraveler | 7 | 5 | 5 | healthy |
| Ob Nixilis | 5 | 4 | 3 | 1 taken-but-not-arrived = countered (E-162a) |
| Dictate of Kruphix | 5 | 2 | 2 | taken at the wrong time (section 3) |
| Master of the Feast | 3 | 2 | 2 | taken at the wrong time (section 3) |
| Teferi's Puzzle Box | 2 | 1 | 1 | the one cast produced the corpus's biggest single burst (section 3) |
| **Forced Fruition** | **1** | **0** | **0** | offered once, correctly passed for a lethal Peer. **UNEXERCISED** (section 6) |
| Peer into the Abyss | 1 | 1 | 1 | converted to a kill |

**The column of zeros that matters**: in the vs36 loss, the battlefield held **zero punishers for the
entire game** (Underworld Dreams was cast on turn 7 and the game ended on turn 8 before an opponent
draw step) while **two Howling Mines** had been feeding an artifact-aggro deck since turn 3. In the
vs152 loss the same shape: two Howling Mines by turn 5, first punisher (Ob Nixilis) turn 9, dead
turn 10. Two of the three losses are the same absence, and it appears in no per-decision metric.

## 3. FAILURE CLASS 1 (the #1 slot) — THE INVERTED INCENTIVE IS NOT SEQUENCED

deck162's win engine only works in one order: the punisher must be on the battlefield BEFORE the
symmetric draw engine, or the draw engine is a gift to the opponent. **The pilot understands the
synergy and reverses the order.** This is not a comprehension failure — it is a priority failure, and
that is exactly what a guide layer fixes.

### 3a. Punisher offered, symmetric engine taken instead — three windows in one game

`139v162`, translog `1787476087-ai_baka_deck162-...-vs-ai_baka_deck139.jsonl`. Underworld Dreams was
in hand and payable from turn 5 onward; it was OFFERED in **4 windows** and taken in the 4th, at 3 life.

- **s7, T5, life 20/18.** Options: `1. Liliana's Caress {1}{b}`, `2. Howling Mine`, **`3. Underworld
  Dreams {b}{b}{b}`**, `4. Master of the Feast`. Chose 1. Reasoning verbatim: *"the plan says 'Play
  Swamp, cast Liliana's Caress' ... I will follow the plan"* — it names option 3, quotes the core's own
  stale-plan caveat, and follows the stale plan anyway. **Verdict: PLAN INERTIA, not a belief miss**
  (wave-33 HL6). Guide lane anyway, because a hard priority order is what beats inertia.
- **s10, T7, life 19/16.** Options 2 = Underworld Dreams, 3 = Master of the Feast. Chose 3. Reasoning
  verbatim: *"Master of the Feast is a 5/5 flyer and draws cards for the opponent, which fuels my
  Underworld Dreams, Liliana's Caress, Fate Unraveler (**not in play**), Ob Nixilis (**not in play**)
  ... Wait, should I cast Underworld Dreams first? **No, Master of the Feast is the engine/threat.**"*
  It writes down, in its own words, that the punishers are not in play, and casts the card that hands
  the opponent a card every upkeep. **This is the cleanest single repro in the corpus.**
- **s13, T9, life 13/14.** It computes its mana exactly: *"Cost of Underworld Dreams: {B}{B}{B}. Cost
  of Dictate of Kruphix: {1}{U}{U}. Total: {1}{U}{U}{B}{B}{B}. I have exactly that!"* — then:
  *"I'll pick one ... **It doesn't strictly matter**, but I'll go with Dictate of Kruphix."* It cast
  Dictate first. At **s14, same turn**, its remaining mana was `{B}{B}`, Underworld Dreams was no
  longer castable, and it spent the two mana on **a second Howling Mine** instead. The order it called
  immaterial cost the punisher a full turn cycle AND converted the residual mana into a third free
  card per turn for the opponent. The opponent converted those cards into Brokkos and Illuna
  (two 6/6s) and killed it on turn 12.
- **s18, T11, life 3/7.** Finally cast Underworld Dreams and immediately computed the kill correctly:
  *"they draw 3, take 3 damage -> 0."* **The pilot can do the punisher arithmetic.** It just does it
  four turns late.

### 3b. The same shape as a positive control

`125v162` is the counterfactual. Howling Mine turn 3, **Underworld Dreams turn 5** — the shortest
engine-to-punisher gap in the corpus. From that point `events` shows an unbroken drain: Underworld
Dreams alone fired 9 times before Ob Nixilis and two Fate Unravelers joined it, and deck162 won at
**19 of 20 life**. It is the only game in which deck162 took essentially no damage.

### 3c. Gap table (context, n=6 — reported, not argued from)

Turn of the first symmetric draw engine CAST vs turn of the first punisher CAST.

| matchup | first symmetric | first punisher | gap | result |
|---|---|---|---|---|
| 125 | Mine T3 | Underworld Dreams T5 | 2 | WIN, 19 life |
| 146 | Mine T3 | Fate Unraveler T7 | 4 | WIN, 4 life |
| 126 | Mine T3, Mine T5 | Fate Unraveler T7 | 4 | WIN, 16 life |
| 36 | Mine T3, Mine T5 | Underworld Dreams T7 (never resolved a trigger) | 4 | LOSS T8 |
| 152 | Mine T3, Mine T5 | Ob Nixilis T9 | 6 | LOSS T10 |
| 139 | Mine T3 | Underworld Dreams T11 (offered T5/T7/T9) | 8 | LOSS T12 |

n=6 cannot support a rate claim and none is made. The load-bearing evidence is 3a: three windows in
one game where the punisher was on the table and the model wrote down why it should be cast, and
picked the gift instead.

### 3d. Why the false belief underneath is a DECK-FACT gap, not a perception gap

`36v162` s4 and s6, verbatim: *"In these engine decks, **drawing more cards is almost always the
priority to find the win con**. I'll stick with Howling Mine"* and *"Howling Mine is **strictly
better for my deck's strategy**."* The pilot reads Howling Mine as ITS OWN draw engine. The option
line prints the true symmetric text (`"...each player's draw step ... that player draws an additional
card"`) — the fact IS surfaced and the model reads past it, exactly the wave-20 deck102 FOURTH
idiosyncrasy face (a symmetric card fired as if one-sided), with the sign flipped: here the symmetry
is read as favouring the caster when it does not. **Corrective is a flat deck fact plus a fire-gate,
not a suppression** — the Mine is correct and strong once a punisher is out.

## 4. FAILURE CLASS 2 — THE BLOCK SEAM: A FALSE RULES BELIEF, 9 LIFE IN ONE GAME

7 block windows corpus-wide. Two declines in `146v162` rest on a flat rules error, stated twice.

- **s7, T4, life 17.** Attacker: Silverquill Silencer (3/2). Available: two Shield Spheres, both
  rendered `[deals 0 - this block kills nothing, it only absorbs damage] - may block A1 (neither
  dies)`. Chose `no blockers`. Reasoning verbatim: *"If I block with one or both, the opponent takes 0
  damage. **I take 3 damage (if one blocks) or 6 damage (if both block).** ... There is no benefit to
  blocking."* The pilot believes a block by a 0-power wall does not stop the damage, and that a second
  blocker DOUBLES the damage it takes. Both are false. Took 3.
- **s10, T6, life 14.** Two attackers (3/2 + 3/3 vigilance), two Shield Spheres, every line `(neither
  dies)`. Chose `no blockers`. Verbatim: *"Blocking with a defender **doesn't prevent damage to me
  unless the blocker prevents damage**"* — and it cites the core hint as licence, quoting the prompt
  line back: *"block only where the trade favors you; taking damage while ahead on LIFE is often
  correct"*. Took 6.
- It then blocked correctly at s13 and s17 once the damage was lethal-adjacent, and won the game **at
  4 life**. The 9 free life it declined was the entire margin.

Routing: the deciding fact IS rendered (`it only absorbs damage`, `(neither dies)`) and the pilot read
past it, so this is a **model rules belief**, not a state misread -> guide falsity-fact anchored to the
printed parenthetical, PLUS a render note (E-162c — the salient tokens on that line are `deals 0`
and `kills nothing`; the load-bearing clause trails and is vague).

The core's blockers hint explicitly says *"(your strategy guide's blocking rules override this general
hint)"* — the override slot exists and is empty for this deck. Same collision the wave-35 brief
flagged for deck152/deck105.

## 5. FAILURE CLASS 3 — THE WALLS ARE NEVER DEPLOYED

deck162 runs no removal except Ob Nixilis -2 and no lifegain. Its entire defence is 3 Shield Sphere
and 2 Fog Bank. **Fog Bank: offered in 21 windows, cast once.** Fog Bank prevents ALL combat damage
to and from itself (Oracle-verified) — against a repeating ground attacker it is a permanent, free,
unkillable wall.

- `36v162`: Fog Bank offered at s4, s6, s8 and declined all three, twice with the reasoning explicitly
  naming it (*"Hold Fog Bank for defense against Cathodions"* — s6) while Cathodion beat it from 19 to
  8. It never cast it. Died turn 8.
- `152v162`: Fog Bank offered in 6 windows, cast 0. Life 20 -> 18 -> 12 -> 3 -> dead on turn 10.
- Both losses to creature decks have `Fog Bank on the battlefield = 0` in the absence table.

This is a plain deploy-floor failure (offered > taken -> guide lane) and it is cheap to fix because
Fog Bank is a 2-mana permanent with no downside in this deck.

## 6. HONEST NULLS / UNEXERCISED SURFACES

| surface | corpus count | pool count | verdict |
|---|---|---|---|
| **Forced Fruition** (`@movedTo(*[-land]\|opponentstack):draw:7 opponent`) | offered 1, cast 0 | 2 copies in deck162 | **UNEXERCISED.** Its resolution, and the Underworld-Dreams-plus-Fruition interaction (7 damage per opponent spell), have never been observed. Guide teaches its RULE and both outcomes, no frequency claim. Re-check next corpus; a probe deck is the reliable way to exercise it. |
| **Mulligan seam** | 0 mulligans in 6 games | — | **UNEXERCISED.** Every hand was a 3+-land keep. The `having already taken N mulligan` string is confirmed present in the pool's prompts (grepped from a sibling seat's translog this corpus) but deck162 never saw it. Floor written against the verified string, no procedure. |
| **Teferi's Puzzle Box** | 1 cast (146v162 T9) | 2 copies | Fired correctly and hugely — after it landed, Fate Unraveler dealt **10 separate 1-damage triggers in one opponent turn cycle**. One instance only; the guide states the mechanism, not a rate. |
| **Liliana's Caress synergy** | 4 casts; 8 opponent discards corpus-wide | — | Confirmed live: in `139v162` Caress was out for 3 opponent discards and the opponent's life dropped 2 each time. The discards are cleanup-step overdraw caused by deck162's own Howling Mines — a real second-order payoff. Small sample; guide gives it a low-priority conditional line. |
| Attacker seam under pressure | 7 windows, all trivially small offered lists (1-2 creatures) | — | The seam is exercised but never hard. No evidence to legislate on. |

## 7. ENGINE / REPRESENTATION DEFECTS — for the wave-40 ledger

### E-162a (HIGH, cross-deck) — a COUNTERED spell narrates as "resolved and went to your graveyard"

The wave-35 lane-B narration change was supposed to make countered-vs-resolved distinct. It is not.

`125v162`, s12 events verbatim:
```
- You cast Ob Nixilis, the Hate-Twisted
- Opponent cast Cancel
- Your Ob Nixilis, the Hate-Twisted resolved and went to your graveyard
- Opponent's Cancel resolved and went to the opponent's graveyard
```
Same file, s27:
```
- You cast Howling Mine
- Opponent cast Cancel
- Your Howling Mine resolved and went to your graveyard
```
Both were countered. The narration asserts they RESOLVED. This is a TRUST-DOCTRINE violation of the
first kind: a false statement the pilot is instructed to believe, on the single most consequential
event class in a control matchup. It affects every deck that ever faces a counterspell, and it is
invisible to any per-decision metric. Repro: any game against deck125.
Ask: emit `- Your <card> was COUNTERED by <spell> and went to your graveyard`.

### E-162b (MEDIUM) — `{right now: draws 0}` on cards that draw

Dictate of Kruphix renders `Cast Dictate of Kruphix {1}{u}{u} {right now: draws 0}`; Teferi's Puzzle
Box renders `{right now: draws 0, draws 0}`. Both draw a nonzero number of cards for both players
every draw step. Per wave-33 HL7 (annotation truth), an emitted magnitude asserts the effect will
happen; `draws 0` on a card whose whole function is drawing is a falsity, and the annotation
programme's earned trust makes a wrong magnitude strictly worse than an absent one.
**Not yet observed causing a misplay** — at `139v162` s13 the pilot ignored the annotation and reasoned
from the card text (*"Dictate gives +1 draw per turn"*) — so this is docketed as a latent falsity, not
a loss. Ask: suppress the clause for triggered-on-a-future-step draws, or name the branch inline.

### E-162c (MEDIUM, wording) — the wall-block annotation buries its load-bearing clause

`B1. Shield Sphere #1 (0/6) [defender] [deals 0 - this block kills nothing, it only absorbs damage]
- may block A1 (neither dies)`

The two salient tokens are `deals 0` and `kills nothing`; the clause that actually decides the
question — that the block stops the damage — is the trailing, unquantified *"it only absorbs damage"*.
The pilot quoted the first two and concluded blocking was pointless (section 4, two windows, 9 life).
Per the wave-29 annotation-wording rung (restriction/verb first, no vague affirmative tail), ask:
`[deals 0 - this block kills nothing, but it STOPS all N damage from reaching you]` with N filled in.
This is the render half of section 4; the guide carries the interim falsity-fact.

### E-162d (LOW) — Peer into the Abyss magnitude flattens both modal branches

`Cast Peer into the Abyss {4}{b}{b}{b} {right now: life -4, life -8, draws 25}` at deck162 life 16,
opponent life 8. `life -4` is the target-opponent branch, `life -8` is the target-controller branch,
and the line does not say which is which. The pilot picked correctly (1/1) so no loss is attributable,
but a pilot reading `life -8` as its own cost could decline a lethal spell. Ask: label the branches, or
emit only the branch the option will take.

### N-162e (LOW, harness/roster) — `#HINT` lines are not enforced for the GPT seat

`deck162.txt` carries `#HINT:dontattackwith(Fate Unraveler)` and `#HINT:dontblockwith(Fate Unraveler)`.
The engine offered Fate Unraveler as an attacker in 4 windows and as a blocker in 1, and the pilot
took all of them. This is expected (AIHints is a Baka surface) and is noted only to record that the
deck author's instruction has to be carried by the GUIDE, per HL10. Two of the four attacks were
worth nothing: `126v162` s19 and s24 sent the 3/4 into a Wall of Omens (0/4) — `- Fate Unraveler dealt
3 damage to Wall of Omens`, twice, no damage through, punisher tapped out of blocking. One was
lethal-relevant (`146v162` s21, opponent at 3, Fate Unraveler swung for the last 3). So the hint is
carried into the guide as an ordering rule WITH the lethal exception, never as an absolute — a blanket
"never attack" would have cost the vs146 win.

## 8. PER-CLAIM CARD-FACT LEDGER (HL2) — every factual claim in `deck162_strategy.txt`

Adjudicated against BOTH Scryfall Oracle (`api.scryfall.com/cards/named?exact=`) and the engine
`text=`/`auto=`/`mana=` lines in `Res/sets/primitives/`. All twelve non-land cards agree between the
two sources; there is **no Oracle-vs-engine divergence in this deck**.

| # | claim as written in the guide | adjudicating source | verdict |
|---|---|---|---|
| 1 | Underworld Dreams costs {B}{B}{B}, enchantment, "whenever an opponent draws a card, deals 1 damage to that player" | Scryfall `{B}{B}{B}` / mtg.txt `mana={B}{B}{B}`, `auto=@drawfoeof(player):damage:1 opponent` | TRUE |
| 2 | Fate Unraveler costs {3}{B}, is a 3/4 creature, same on-draw trigger | Scryfall `{3}{B}` 3/4 / mtg.txt `mana={3}{B}` `power=3 toughness=4` `auto=@drawfoeof(player):damage:1 opponent` | TRUE |
| 3 | Ob Nixilis, the Hate-Twisted costs {3}{B}{B}, enters with 5 loyalty, same on-draw trigger, -2 destroys a creature and draws its controller two cards | Scryfall `{3}{B}{B}` loyalty 5 / planeswalkers.txt `counter(0/0,5,loyalty)`, `{C(0/0,-2,Loyalty)}:... destroy target(creature) && draw:2 targetcontroller` | TRUE |
| 4 | The three punishers each deal 1 damage per opponent draw, and they STACK | engine `events`, `125v162`: four separate sources logged 1 damage on the same draw (`Fate Unraveler dealt 1 ...` x2, `Ob Nixilis ... 1`, `Underworld Dreams ... 1`) | TRUE, live-verified |
| 5 | Howling Mine costs {2} and gives EACH player an extra card each draw step, only while untapped | Scryfall / mtg.txt `auto=@each my draw sourcenottap:draw:1 controller` + `@each opponent draw sourcenottap:draw:1 opponent` | TRUE |
| 6 | Dictate of Kruphix costs {1}{U}{U}, has flash, gives EACH player an extra card each draw step (no untapped clause) | Scryfall / mtg.txt `@each my draw:draw:1 controller` + `@each opponent draw:draw:1 opponent` | TRUE |
| 7 | Teferi's Puzzle Box costs {4} and makes EACH player bottom their hand and redraw that many each draw step | Scryfall / mtg.txt `count(type:*:opponenthand) && bottomoflibrary all(*\|opponenthand) && draw:countedamount opponent` | TRUE |
| 8 | With a punisher out, Puzzle Box turns the opponent's hand size into that much damage per turn | `146v162` events: **10 consecutive `Fate Unraveler dealt 1 damage to the opponent`** in one opponent turn cycle after Puzzle Box landed | TRUE, live-verified |
| 9 | Forced Fruition costs {4}{U}{U} and makes an opponent draw SEVEN cards whenever they cast a spell | Scryfall / mtg.txt `auto=@movedTo(*[-land]\|opponentstack):draw:7 opponent` | TRUE (card fact). **Never resolved in this corpus** — the guide states the rule and both outcomes, no rate claim |
| 10 | Master of the Feast costs {1}{B}{B}, is a 5/5 flier, and gives each opponent a card at YOUR upkeep | Scryfall `{1}{B}{B}` 5/5 flying / mtg.txt `power=5 toughness=5`, `auto=@each my upkeep:draw:1 opponent` | TRUE |
| 11 | Liliana's Caress costs {1}{B}; opponent loses 2 life per card THEY discard | Scryfall / mtg.txt `auto=@discarded(*\|opponenthand):life:-2 opponent` | TRUE |
| 12 | Caress fires off cleanup-step overdraw caused by your own Mines | `139v162` events: `Opponent discarded Forest` / `Opponent discarded Stomping Ground` x2, opponent life stepping down 2 each time with Caress on the battlefield | TRUE, live-verified (3 instances) |
| 13 | Shield Sphere costs {0}, is a 0/6 defender, and gets a -0/-1 counter each time it blocks | Scryfall 0/6 / mtg.txt `mana={0} power=0 toughness=6`, `auto=@combat(blocking) source(this):counter(0/-1) all(this)` | TRUE (observed 0/6 -> 0/5 -> 0/4 in `146v162`) |
| 14 | Fog Bank costs {1}{U}, is a 0/2 flying defender, and prevents ALL combat damage dealt to AND by it | Scryfall / mtg.txt `auto=preventAllCombatDamage to(this)` + `from(this)` | TRUE |
| 15 | Peer into the Abyss costs {4}{B}{B}{B}, is a sorcery, and makes the target draw half their library and lose half their life (round up each) | Scryfall / borderline.txt `draw:halfuptype:*:opponentlibrary opponent && life:-halfupopponentlifetotal opponent` | TRUE |
| 16 | Peer offers a second menu with "target opponent" and "target controller" and the second choice points it at YOURSELF | `126v162` s29 `options_text` = `["target opponent","target controller"]`; borderline.txt has two `choice name(...)` branches | TRUE, render-verified |
| 17 | The deck's cheapest card costs {0} (Shield Sphere) and it contains no one-mana card | deck162.txt full list vs primitives `mana=` for all 12 nonland cards: {0}, {1}{B}, {1}{U}, {1}{B}{B}, {2}, {1}{U}{U}, {3}{B}, {B}{B}{B}, {4}, {3}{B}{B}, {4}{U}{U}, {4}{B}{B}{B} | TRUE — no MV-1 card exists |
| 18 | The deck runs 26 lands, 18 of which make black and 18 of which make blue | deck162.txt: 8 Island + 8 Swamp + 4 Underground Sea + 4 Drowned Catacomb + 2 Dismal Backwater = 26; B = 8+4+4+2 = 18; U = 8+4+4+2 = 18 | TRUE |
| 19 | Every quoted render substring in the guide | grepped verbatim against this corpus's `prompt`/`options_text` fields: `"Cast nothing right now"`, `"Play no land right now"`, `"[deals 0 - this block kills nothing, it only absorbs damage]"`, `"(neither dies)"`, `"(you kill it, your blocker lives)"`, `"Your hand (7 cards), counted by the engine:"`, `"Playing every land in this hand would cover the cost of:"`, `"Cheapest spell in this hand:"`, `"Mana available:"`, `"having already taken 1 mulligan"` (present in the pool this corpus), `"target opponent"`, `"target controller"` | ALL non-zero hits |

## 9. WHAT THE GUIDE DOES (and what it deliberately does not)

**#1 slot**: the punisher-first cast order. Chosen because it traces to the decision evidence in 3a
(three windows, one game, the model's own words), not to the record — and because the posture, land
drops, attacks, targeting and reply protocol are already right (section 1), so there is no posture
rule to manufacture.

**#2**: the wall floor (Fog Bank + Shield Sphere), because it is the second-largest offered>taken gap
and it owns both fast losses.

**#3**: the block falsity-fact, because the guide has an explicit override slot in the core hint and
9 life in one game rides on it.

**Not written**: an attack floor beyond one confirmatory line; a land-drop rule; an Ob Nixilis
targeting table; a mulligan procedure; any frequency word about Forced Fruition, Puzzle Box or
Peer into the Abyss; any sentence hedging against a render.

## 10. STOPGAP REGISTER (line, defect + citation, removal condition)

| guide line | defect it patches | removal condition |
|---|---|---|
| "A block by Shield Sphere or Fog Bank STOPS that attacker's damage completely..." | E-162c, the wall-block annotation buries its load-bearing clause (146v162 s7, s10) | Delete when `/usr/bin/grep -c 'STOPS all' <corpus>/*.jsonl` is non-empty, i.e. when the render states the stopped damage itself. |
| (none written for E-162a) | Per doctrine a guide never teaches doubt about a render. No line is written about countered spells; the fix is engine-lane only. | n/a |

## 11. FALSIFIABLE PREDICTIONS for the next corpus (the real gate on this guide)

1. Windows where a punisher is in the cast list and a symmetric draw engine is chosen instead, with no
   punisher on the battlefield: **3 (this corpus, all in `139v162`) -> 0.**
2. Fog Bank offered>taken: **21/1 -> the guide's floor should put a Fog Bank on the battlefield in
   every game where the opponent controls a creature.** Measure "Fog Bank arrived" per game, not the
   take rate.
3. `no blockers` chosen when at least one offered block line reads `(neither dies)` and the header says
   NOT lethal: **2 -> 0.**
4. Regression watch (wave-31 pitfall: fixing this wave's leak causes next wave's): a punisher-first
   rule can become "never cast Howling Mine". The guide's release is countable and printed
   (a punisher named on YOUR battlefield line). Measure Howling Mine arrived-per-game: it must **not**
   fall below this corpus's 2.0/game.
