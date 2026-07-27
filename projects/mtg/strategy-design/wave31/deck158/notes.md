# deck158 — wave-31 development notes (engine / harness / representation)

Corpus `matchups-20260727-025026`, binary cf7cb2e53. Zero crash signatures at this seat
(N-139h corpus gate PASSES here). All citations are `<opponent> seq<N> t<N>` from the
deck158 seat translogs; "game Turn N" quotes the narration's own turn header.

---

## N-158g — HIGHEST PRIORITY. `Mana available:` line leads with the COLOUR SET, which the pilot reads as the POOL SIZE. Cost a game.

Current render: `Mana available: {r}{b} from 5 untapped sources (tapped automatically when you
cast)`. The pilot repeatedly binds `{r}{b}` to "I have two mana".

Repros (all: a cast the pilot then declined as unaffordable was ON the option list):
- **vs148 seq19, t9, my_life 1 — GAME-LOSING.** `Mana available: {r}{b} from 5 untapped
  sources`; options `1. Cast Snarling Warg {3}{b} (3/4)` / `2. Cast nothing right now`.
  Reply: *"I have {R}{B} available, but Snarling Warg costs {3}{B} (four mana total). I cannot
  afford to cast it."* Chose Cast nothing. Died at -4 the following turn to 1+3 = 4 damage the
  Warg (a 4/4 with an Orc out) could have blocked. This is deck158's only loss.
- **vs148 seq17, t9** — same board, same offered Warg, `I only have {R}{B}`, Cast nothing.
- **vs146 seq31, t17, my_life 5 vs opp 7** — `from 6 untapped sources`, both
  `Cast Gothmog, Morgul Lieutenant {3}{b}` and `Cast Snarling Warg {3}{b}` offered; reply
  "cannot pay {3}{B}"; took the 2-mana March instead.
- **vs152 seq11, t7** — `{r}{b} from 2 untapped sources`, `Cast Feed the Swarm {1}{b}` offered;
  reply *"I only have one {B} and one {R} (which cannot pay for the generic {1})"*; Cast
  nothing. (Second, distinct false belief: off-colour mana cannot pay generic.)

Tax cost as well as decision cost: **8 of this seat's 10 largest replies are mana arithmetic**,
three of them on a LAND-DROP decision (vs137 seq18 13,319 chars / 208s; vs148 seq2 12,875 /
164s; vs152 seq22 12,198 / 150s). At ~12s/decision affordable under the 3000s cap, a 200s
land-drop is a direct adjudication risk.

Note this is NOT an instruction gap: the core prompt already carries "Never decline a listed
action because you believe... you cannot pay for it", "any color can pay a generic cost", and
the same item in its common-errors list. Instruction is present and losing at distance ->
REPRESENTATION per the standing routing rule. Per wave-29 HL2 (annotation wording is
load-bearing, and an attractive-nuisance substring wins over the intended reading), the fix is
WORDING, count-first:
  `Mana available: 5 (five untapped sources; colours you can make: {r}{b})`
or minimally reorder to `Mana available: 5 untapped sources, producing {r}{b}`. The digit the
pilot needs must precede the colour set, and the colour set must be explicitly labelled as
colours, not as a pool. A guide-layer stopgap ships this wave (strategy.txt Rule #1) and should
be DEMOTED the corpus after this render lands — verify by grepping deck158 replies for
"I only have {" and "cannot afford" against offered casts; the target is zero.

---

## N-158c — Foray of Orcs renders `{right now: damage 0}` in 100% of cases; the true magnitude is (Army power after amass).

`borderline.txt:41425-41427`: amass 2, *then* `damage:power target(creature|opponentbattlefield)`
where power is the amassed Army's. The evaluated magnitude is built before the amass (and
apparently off a source with power 0), so it prints 0 always.
- **vs152 seq17**: rendered `Cast Foray of Orcs {3}{r} {right now: damage 0}` with NO Army on
  the battlefield. Resolved (seq18/19 narration): `Your Orc army (0/0): created -> battlefield`,
  two `Counter added to Orc army`, `Damage: 2 dealt by Orc army to Briarbridge Tracker`.
  True magnitude **2**, rendered **0**.
- **vs137 seq19**: rendered `{right now: damage 0}` with a 7/7 Army out (true value would have
  been 9 against a legal target).
The pilot reasoned past it both times but paid 13.1k and 9.8k chars doing so, and explicitly
argued with the annotation ("The 'damage 0' note in the option refers to the secondary
effect..."). This is the Gray-Merchant `{drains N}` offset shape, except the offset is the
entire value. Fix: evaluate `damage:power` against the post-amass Army (or suppress the
`{right now: ...}` clause when the magnitude cannot be evaluated, rather than printing 0 —
a printed 0 is worse than no annotation). Guide stopgap in strategy.txt (Foray entry); removal
condition = the option line shows the post-amass number.

---

## N-158h — Feed the Swarm renders a CONSTANT `{right now: life -2}` regardless of target.

`borderline.txt:38779`: `newability[life:-manacost opponent]` — you lose life equal to the
DESTROYED permanent's mana value. All ~44 option renders across the corpus print `life -2`.
Resolved values observed:
| target | mana cost | rendered | actual |
|---|---|---|---|
| Nadaar, Selfless Paladin (vs146 seq20/21) | `{2}{W}` | -2 | **-3** (my_life 8 -> 5) |
| Stone Haven Pilgrim (vs148 seq11/12) | `{1}{W}` | -2 | -2 (7 -> 5) |
| Serra Ascendant (vs122 seq12/13) | `{W}` | -2 | **-1** |
| Kor ally token (vs148 seq13/14) | token, MV 0 | -2 | **0** (5 -> 5) |
The magnitude evaluator cannot know the target at option-build time. Options: evaluate per
legal target and fold it into the existing `- legal targets right now: A, B, C` list
(`... Nadaar, Selfless Paladin (costs you 3 life)`), or drop the clause for target-dependent
self-costs. This deck finishes its wins at 3-15 life, so a 1-life error is live. Guide stopgap
in strategy.txt (Feed the Swarm entry) with the same removal condition.

---

## N-158f — Assault on Osgiliath's double-strike / haste emblem appears not to reach the amassed Orc Army. TWO witnesses.

`borderline.txt:5890`: `auto=emblem transforms((,newability[lord(*[orc;goblin]|mybattlefield)
double strike],newability[lord(*[orc;goblin]|mybattlefield) haste])) ueot` — an unconditional
line, so it should fire on both the base and the alternative cast.
1. **HASTE — vs146 seq28/29, game Turn 16.** Base face, X=3 announced and paid. Narration:
   `Your Assault on Osgiliath: hand -> stack` / `Your Orc army (0/0): created -> battlefield` /
   three `Counter added to Orc army` / **`=== Turn 17 - opponent's turn ===`**. No `Phase:
   Attackers`, no attackers window — the engine found no eligible attacker, i.e. the fresh
   3/3 Army was summoning-sick. The pilot had cast it precisely for the haste swing (plan:
   "Attack immediately with the Army to deal 6 damage") at 5 life vs opp 11 and got nothing.
2. **DOUBLE STRIKE — vs152 game Turn 12.** Alternative face, X=3; the 7/7 Army attacks and is
   blocked by a 3-toughness Elite Spellbinder. Log shows a single
   `Damage: 7 dealt by Orc army to Elite Spellbinder` **and** `Damage: 3 dealt by Elite
   Spellbinder to Orc army`. With double strike the first-strike 7 would have killed the
   blocker before it dealt damage back.
Likely axis to check: whether `lord(*[orc;goblin]|mybattlefield)` matches the `Orc Army` token
(created as `Creature Orc Army`, then `transforms((Orc,...))` by the amass macro) at the time
the emblem is evaluated, and whether an emblem created in the same resolution as the token
applies to it. Deliberately NOT written into strategy.txt: the guide teaches Assault as
"X counters at once" and does not promise the same-turn swing (per the guides-describe-play,
engine-bugs-get-fixed-in-the-engine rule).

---

## N-158i — X-COST ZERO-SLACK silent defer (NEW SEAM; sibling of, not identical to, convoke divergence C)

**vs146 seq25, t13, `kind=defer`, `fallback=deferred_to_heuristic`, `prompt=""`, `reply=""`,
`chosen_text="Assault on Osgiliath"`, `choice=-1`.** The model was never asked.

- seq24 (Main phase 2, same turn): `Mana available: {r} from 3 untapped sources`, battlefield
  has exactly three untapped Mountains, **creatures: 0**. Options
  `1. Cast Assault on Osgiliath {r}{r}{r}{x}` (base face — legal, `restriction=type(army|
  mybattlefield)~equalto~0`) / `2. Cast nothing`. Pilot chose 1.
- Cost `{X}{R}{R}{R}`: the three colour pips consume all three sources, so the only affordable
  X is **0**.
- seq25 defers. **Arrival-traced: the cast never resolved.** seq26's narration goes
  `Your Orc army: graveyard -> zone` -> `=== Turn 15 - opponent's turn ===` with no
  `Assault on Osgiliath: hand -> stack`, and seq26's hand still lists the card.
- **Counterfactual in the same game**: seq27 (6 untapped sources, X-slack 3) — identical cast,
  ANNOUNCE_X fires ("You can afford X up to 3"), cast resolves, three counters land.

Classification: the *symptom* is the divergence-C exit (`FindCardToPlay` `aiForcedCandidate`
validation rejecting a cast `offerable()` approved -> silent defer -> silent no-op), but the
*trigger* is not convoke — deck158 runs no convoke and no creature-tapping alternative cost.
The discriminator here is **X affordability slack == 0**. Suggested fix directions:
(i) extend the h2 `maxAnnounceableX` credit path to plain `{X}` costs so an X=0-only cast
either announces X=0 or is filtered out by the same mechanism that filters 601.2c-illegal
casts; (ii) make the validation reject LOUD (re-ask the model) instead of deferring.
Severity is LOW in outcome (X=0 on this card produces a 0/0 Army that dies to SBAs, so the
deferred cast was a null play) but it is a clean, cheap repro of the silent-abort class.

---

## N-158e — flash creature casts are absent from priority-window legal-action lists

**vs152 seq27, t13, own Upkeep.** `Your hand: Feed the Swarm {1}{b}; Swamp; Orcish Bowmasters
{1}{b} (1/1)`. `Mana available: {r}{b} from 4 untapped sources`. Entire legal-action list:
`1. Draw 1 with Clue [cost: {2}, Sacrifice]`. Orcish Bowmasters has `abilities=flash`
(`borderline.txt:80464`) and `{1}{B}` was payable. The pilot noticed the absence and reasoned
around it ("the options provided only show Draw 1 with Clue... I must draw"). Whether flash
casts *should* be offered in the controller's own upkeep is a design call, but the current
behaviour makes every flash creature in the game a strictly-main-phase card, which silently
deletes a real capability. Related executability datum: this seat saw **6 priority windows in
6 games, only 1 of them on the opponent's turn** (vs152 seq14) — worth checking whether the
h5 display-toggle auto-pass is over-suppressing response windows for a seat with a flash
creature and instant-speed-relevant mana up.

---

## N-158a — amass cast options carry no magnitude (LATENT, low priority)

`1. Cast Mordor Muster {1}{b}`, `Cast March from the Black Gate {1}{b}`,
`Cast Dunland Crebain {2}{b} (1/1)`, `Cast Swarming of Moria {2}{r}` and the Bowmasters/Gothmog
lines carry no amass amount and no resulting Army size, so the pilot re-derives it every time
(the top-2 tax spikes at this seat, vs137 seq18/19, are exactly this arithmetic). A
`{right now: Army 7/7 -> 9/9}` clause on any amass cast option would collapse it. Grouped with
N-158c: both are the same evaluator, and Foray needs its magnitude fixed anyway.

## N-158b — `- Counter added to Orc army` narration has no count and hides the Mauhur bonus (LATENT)

One narration line per amass EVENT, no magnitude, no source. Arithmetic proof of the ambiguity:
vs122 game Turn 12, four such lines took the Army 7/7 -> 15/15 (+8, Mauhur out); vs137 game
Turn 12, two lines took it 7/7 -> 9/9 (+2, no Mauhur). No misplay traced to it — the pilot used
the battlefield `(N/N)` line, which is correct — so this is latent. Suggested:
`- Orc army gets 2x +1/+1 counters (now 9/9) [Foray of Orcs]`.

## N-158d — Foray's damage target menu names the ARMY as the ability source (LATENT)

**vs152 seq18**: `TARGET CHOICE for Orc army (this spell/ability is already on the stack and
needs a target ...). Pick the ONE target it will affect ... answer with the chosen TARGET's
name (not "Orc army")`. The damage ability is transformed onto the Army token, so the header
names the player's own creature as the thing asking for a target. The pilot decoded it
correctly, but a header naming your own creature is an invitation to self-target and is a
degenerate-label sibling of the wave-16 WATCH. Suggested: name the originating spell
(`TARGET CHOICE for Foray of Orcs`) when the ability was transformed from a spell.

---

## Non-findings, recorded so the next reviewer does not re-open them

- **Amass has no target decision by design.** `_AMASSORC1_/2_` use `notatarget(army|
  myBattlefield)`; zero amass-target asks in 165 decisions. Nothing to fix.
- **Army state rendering is correct and complete**: `(N/N) (printed 0/0) [deathtouch]
  [counters: Nx +1/+1]`, attacker lines, blocker trade annotations, `created -> battlefield`.
- **Assault's two faces are correctly restriction-gated** (base offered only with 0 Armies,
  alternative only with 1+) and the ANNOUNCE_X menu (affordability cap + largest-first +
  "reply with the OPTION number") produced max-X **2/2**.
- **The `[summoning sick - cannot attack this turn]` tag wording is fine** (restriction-first,
  no action-affirmative substring). The vs146 seq23 "sick creatures cannot block" misread is a
  model knowledge gap, routed to the GUIDE, not to the tag.
- **Bowmasters ping targeting is core-handled**: face chosen 5/5, all correct. No guide or
  render work needed.
- **Never drawn in 6 games**: Swarming of Moria (3-of), Ugluk of the White Hand, Grishnakh,
  Brash Instigator. Grishnakh's amass-gated steal is the only genuinely unexercised decision
  SHAPE left in the deck; its cousin (Sauron's ETB steal) exercised correctly twice.
