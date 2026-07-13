# deck131 — wave 5 review (blue-red "spellslinger"; deck name is literally "Mind Control / The Best Counter Deck Ever")

Corpus: one GPT-vs-GPT run, qwen35 both seats,
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260713-145947/`. **6 deck131 games.**
Live guide in play = the wave-4 revision (`bin/Res/ai/baka/deck131_strategy.txt`:
two-gear identity, "YOUR CREATURES ATTACK. THEY DO THEIR BLOCKING BY RACING",
count-and-match attackers, Mountain-first, draw-on-curve).

**Record: 0 W / 5 L / 1 TIMEOUT.** (Wave-4 was 2 W / 8 L / 2 TO over 12 games.)

Seat-file -> game map (seat epoch = results start_epoch + 3..4; confirmed by opponent card names):

| result | vs | file (epoch) | turns | end life me/opp | one-line cause |
|---|---|---|---|---|---|
| LOSS | 110 | 1783974685 | 7  | -1 / 18 | no red source in opener; never cast a creature; affinity aggro |
| LOSS | 133 | 1783976205 | 17 | -1 / 29 | flooded on Islands; devotion/Liliana opp drained past it |
| LOSS | 140 | 1783976995 | 34 | 13 / 56 | no red until T19; unwinnable lifegain matchup; never attacked |
| TIMEOUT | 135 | 1783977943 | — | 23 / 8 | winning-but-slow: attacked every turn, 2-3 clock hit decision cap |
| LOSS | 44  | 1783981248 | 20 | -6 / 20 | curved out, but threats removed; opp untouched at 20 |
| LOSS | 109 | 1783981630 | 11 | -3 / 20 | durdled T0-3, Young Pyro delayed to T4; mono-red raced it |

**Opponent life at death: 18, 29, 56, (TO opp 8), 20, 20.** In four of six games deck131 dealt
essentially zero damage -- the wave-4 signature (opp near 20 = the deck never had a plan running)
recurs almost unchanged.

---

## Headline: this is the SAME deck-construction ceiling wave 4 named, hitting a harder opponent slate.

The two wave-4 wins (vs109, vs44) were both games where the payoffs curved out and connected.
This corpus deck131 drew those same matchups (vs109, vs44) **and lost both** -- to worse mana and
to the threats being removed, not to a guide regression. There is no evidence the wave-4 guide
caused the drop from 2/12 to 0/6; n=6 with a brutal draw (affinity aggro, lifegain, mono-red,
fliers, devotion) explains it. The guide is being **executed faithfully** in almost every seam
(see "What the pilot did well"). The losing record traces to the decklist, plus a small, real,
model-fixable residue on the Guttersnipe engine.

---

## Per-game loss attribution (with translog evidence: file + seq)

### vs110 -- LOSS, DECK (manabase). file 1783974685
- Opening hand kept (seq 0): `Island; Island; Island; Essence Scatter; Young Pyromancer;
  Prism Ring; Elixir of Immortality` -- **three Islands, zero red sources, and the only threat
  (Young Pyromancer {1}{R}) is red.** The threat was uncastable from turn 1.
- Pilot did the only castable things: Prism Ring T0, Elixir T2, then "Cast nothing" T5/T7 while
  affinity aggro (Ornithopter 6/3, Master of Etherium 5/5, Vault Skirge lifelink flyer, Cranial
  Plating) killed it by T7. A Mountain and Young Pyromancer were both in hand by the end -- drawn
  too late.
- **Attribution: DECK / mulligan.** 14-Island/8-Mountain over all-red payoffs put a hand on the
  table that could not deploy. No guide wording casts a creature with no red mana.

### vs133 -- LOSS, DECK (flood + strong opponent) + minor durdle. file 1783976205
- Cast Young Pyromancer T2 and attacked with it T4. Young Pyromancer was then removed and
  Guttersnipe was never drawn. Final board (last decision): 6 Islands + 1 Mountain, hand a single
  Island -- **flooded.** Opponent = mono-black devotion (Gray Merchant of Asphodel, Liliana of
  the Veil, Bloodghast) whose drain climbed opp to 29 while draining deck131 to -1.
- **Attribution: DECK (flood / low threat density) vs. a strong drain deck.** Not a guide bug.

### vs140 -- LOSS, DECK (manabase + unwinnable matchup) + PILOT (no attacks). file 1783976995
- Opening had Young Pyromancer but **no Mountain**; the Mountain-first rule cannot fire without a
  Mountain, and one did not arrive until T19. Threats stranded ~18 turns.
- deck140 is the lifegain deck; opp life climbed 20->27->36->45->56. Per wave 4 this is unwinnable
  at deck131's 2-damage clock.
- **Compounding pilot error: ZERO attackers records across 34 turns**, though Young Pyromancer was
  out T19-20 and Guttersnipe T21 -- they were summoning-sick then removed before an attack step
  surfaced. And one clear missed engine cast: **T22 (seq 22), Guttersnipe on the battlefield,
  declined the only cast (Counterspell) for "Cast nothing"** -- with Guttersnipe out that cast was
  2 to the face.
- **Attribution: DECK (mana + matchup) primary; PILOT (missed Guttersnipe damage) secondary.**

### vs135 -- TIMEOUT, INFRA + deck low-power (NOT a guide bug). file 1783977943
- This was the pilot's BEST game: Young Pyromancer T3, attacked essentially every turn, ground opp
  19->8, deck131 at 23. It was **ahead at the cap.** Guttersnipe was never drawn, so the clock was
  a lone 2/1 + tokens (2-3/turn).
- 86 decision records (53 priority windows), median latency 9.5 s. A slow 2-3 clock plus high
  decision density hit the 2400 s / decision cap before it could close.
- **Attribution: winning-but-slow timeout -- deck power (no Guttersnipe, small clock) + wall-clock
  economics.** Per the skill, adding anti-passivity pressure here would regress the guide against a
  problem it does not have. Report as infra/deck; write nothing.

### vs44 -- LOSS, DECK/matchup + PILOT (missed Guttersnipe casts). file 1783981248
- Curved perfectly: Prism Ring T1, Young Pyromancer T3, Guttersnipe T5; attacked T5 (Young Pyro)
  and T7 (Guttersnipe). But opponent = a fliers/tempo deck (Faerie Miscreant 3/3 flying shroud x2,
  Archmage of Echoes 6/6); the ground 2/1 and 2/2 never connected and both were removed -- **opp
  sat at 20 the entire game** while deck131 fell to -6 by T20.
- Missed engine casts while Guttersnipe was on the battlefield: **T7 (seq 10) declined the only
  cast, Cyclonic Rift {1}{U}** (bounce a flier + 2 Guttersnipe damage), and **T20 (seq 24)
  declined Dissipate** with a re-cast Guttersnipe out. Each was 2 to a face stuck at 20.
- **Attribution: DECK/matchup primary (threats can't push through, get removed); PILOT
  (Guttersnipe-engine underuse) secondary.**

### vs109 -- LOSS, PILOT + GUIDE (durdle/sequencing) + fast matchup. file 1783981630
- The most guide-relevant loss. Opening had `Island; Island; Mountain; ...; Young Pyromancer`.
  **T0 (seq 1): the PLAN text says "Play Mountain first to enable Young Pyromancer's red cost" --
  then the pilot chose "Play Island."** A direct Mountain-first violation (and a plan/choice
  contradiction; no desync -- reply head "1" == choice 1, and option 1 was "Play Island").
- Consequence: playing Island T0 meant no red source until Mountain came down T4, so **Young
  Pyromancer slipped from a possible T2 cast to T4.** Turns T0-T3 went to Prism Ring x2 + "Cast
  nothing" (declining Cyclonic Rift). Against mono-red aggro (Ash Zealot 2/2 first-strike haste,
  Goblins, Stromkirk Noble, Rakdos Cackler) those two lost tempo turns were the game -- dead T11,
  opp untouched at 20.
- **Attribution: PILOT (Mountain-first violated; early durdle) + GUIDE (Prism Ring emphasis
  legitimizes the durdle) against a fast clock.**

---

## OVERSHOOT AUDIT (mandatory)

Wave 4 added the loud #1 rule "YOUR CREATURES ATTACK. THEY DO THEIR BLOCKING BY RACING", the
"declare BLOCKERS -> 'no blockers' unless the swing is lethal" rule, and the count-and-match
attacker procedure. Flagged wave-4 risks: a Lightmine-Field-style solo-rule misapplication and
attack-floor suicide attacks.

**Finding: the #1 attack/no-block rule did NOT overshoot in this corpus.**
- **No suicide attacks.** Every `attackers` record declared exactly the creatures the engine
  listed as legal (verified against the `A#.` option list): single-creature declarations were
  summoning-sickness-correct, not under-commitment. Example: vs135 T11 the reply was "A1, A2"
  (intending both) but only A1 (Elemental) was offered because Young Pyromancer was cast that turn
  -- the engine correctly excluded it; the pilot's intent was full commitment.
- **No Lightmine-style solo-rule misapplication observed.** No card exercising that shape appeared
  in these six matchups; the risk did not manifest. Keep watching -- unrefuted, not disproven.
- **Blocking rule did not cost a game.** The `blockers` seam fired only once (vs109 T5, "no
  blockers" at 21 life -- correct, non-lethal). No game was lost to declining a chump.
- **The count-and-match / partial-declaration concern from wave 4 did NOT reproduce.** Attacker
  participation was 100% of legal attackers across all six games.

**The over-application that DID occur is subtler and lives on the CAST seam, not combat.** The
guide's DIG-gear line ("you still cast every turn ... Prism Ring or Elixir with spare mana") plus
"Prism Ring ({1}): cast it turn 1" plus the general act-floor combine so the pilot **satisfies
"cast every turn" with cards that do not threaten** (Prism Ring, Elixir, held reactions), while
the actual win engine (Guttersnipe damage) sits idle. This is the locally-obedient / globally-bad
shape: the act-floor is met and the deck still deals zero. vs109 (Prism Ring x2 in the first four
turns of a race it lost) and vs44/vs140 (Guttersnipe out, "Cast nothing" chosen) are the evidence.
The fix is not a brake -- it is re-pointing the "cast every turn" energy at the Guttersnipe engine
and demoting Prism Ring/Elixir (see strategy.txt).

---

## The one model-fixable lever: the Guttersnipe engine is the win condition, and it is underused.

Guttersnipe (`auto=@movedTo(instant,sorcery|mystack):damage:2 opponent`) deals 2 to the opponent
**every time you cast an instant or sorcery** -- no combat required. This, not attacking with
2/2s, is how the deck actually kills a defended opponent (four losses ended with opp at >=20
because the *combat* plan was blocked/removed). Yet with Guttersnipe on the battlefield the pilot
chose "Cast nothing" at castable-spell windows at least three times (vs44 T7 seq 10; vs44 T20 seq
24; vs140 T22 seq 22), because the general prior "never fire a reaction at nothing" and the guide's
own "these are support cards, never your plan" suppress the cast. With Guttersnipe out, the cast
**is** the plan. The guide has this fact (line ~45) but buried and un-prioritized, and it never
overrides the reaction-holding prior by name. Promoting and sharpening it is the single
highest-value change a guide can make here.

---

## What the pilot did well (do not "fix" these)

- **100% attacker participation** -- declared every legal attacker every combat; the wave-4 subset
  worry is resolved.
- **Draw spells on curve** -- Artificer's Epiphany / Opportunity cast when affordable (wave-4
  draw-spell rewrite still holding).
- **Mountain-first mostly obeyed** -- played the Mountain the turn it mattered in vs44, vs133,
  vs135; violated only at vs109 T0.
- **Reactions correctly held with no target** in most windows (Counterspell/Aetherize/Dissipate
  declined when nothing was worth answering) -- correct *except* when Guttersnipe was out.
- **No parser desyncs** (head-first parser clean across all 6 games); **no fallbacks** in any game;
  **no reflexive bad blocks**; **no self-targeting** (Cyclonic Rift's single-target mode is
  opponent-only by `auto=`, so the wave-4-suspected self-bounce is structurally impossible and did
  not occur).

## What the losing record actually traces to (one line each)
- 2 losses (vs110, vs140): **no red source when it mattered** -> threats stranded. DECK (manabase).
- 1 loss (vs133): **flooded on Islands** vs a strong drain deck. DECK (threat density / flood).
- 2 losses (vs44, vs109): **threats deployed but couldn't push damage** (removed/blocked; opp at
  20) + **Guttersnipe engine underused** + (vs109) a Mountain-first/sequencing error. MODEL/GUIDE
  fixable at the margin; DECK-limited overall.
- 1 timeout (vs135): **winning-but-slow**, no Guttersnipe drawn. INFRA + deck low-power.
