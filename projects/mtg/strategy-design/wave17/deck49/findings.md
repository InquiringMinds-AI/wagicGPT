# deck49 (Dragons) — GUIDELESS FIRST PASS findings (wave 17)

Mono-red Dragons aggro-midrange. Corpus: 6 games, 3/6 (W vs deck35/deck21/deck110, L vs
deck135/deck17/deck62). 0 timeouts. ~145 decisions. Played with NO guide (core prompt only),
so this corpus is the clean qwen×core interaction — the tax spikes and stale_echoes below are
the deck-knowledge gaps the core does NOT carry.

## Deck read (from the ENGINE, not real-MTG memory)

Win path: curve out flying Dragons + burn the face. Everything casts off Mountains (mono-red).
Engine card facts (verified against Res/sets/primitives — divergences from real MTG noted, guide
uses ENGINE values):
- Dragonmaster Outcast {R} 1/1 — upkeep with 6+ lands: make a 5/5 flying Dragon token. THE grind engine.
- Bloodmark Mentor {1}{R} **1/1** (real 2/1) — all your red creatures (incl. itself) have first strike.
- Firespitter Whelp {2}{R} 2/2 flying Dragon — deals 1 to opp whenever you cast a noncreature OR Dragon spell.
- Thunderbreak Regent {2}{R}{R} 4/4 flying Dragon — if opp targets a Dragon you control, deal 3 to them.
- Ashenmoor Liege {1}{BR}{BR}{BR} 4/4 — other red (and black) creatures +1/+1; if an opp targets it, they lose 4. **The {B/R} pips pay with RED — it is an all-red anthem, NOT a black card.** (deck49's ONE hybrid exposure.)
- Crucible of Fire {3}{R} (real {2}{R}{R}) enchantment — Dragon creatures get +3/+3. Does NOTHING with no Dragon in play.
- Shivan Dragon {4}{R}{R} 5/5 flying, {R}:+1/+0. Rorix Bladewing {3}{R}{R}{R} 6/? flying HASTE legendary Dragon.
- Goblin War Paint {1}{R} (real {R}) aura — enchanted creature +2/+2 and HASTE. Targets ANY creature (incl. opp's).
- Lightning Bolt {R} 3 to any target. Lava Spike {R} 3 to a PLAYER only. Beacon of Destruction {3}{R}{R} 5 to any target, shuffles back (recurring).

## Failure classes (ranked by tax + impact)

### C1 — MANA FALSE-BELIEF ("no untapped sources / I have 0 mana"). #1 tax source.
The core's recurring "Mana in pool: none → I can't cast" misread, and it is the single biggest
reasoning-tax epicenter in the corpus for a deck whose mana is TRIVIAL (mono-red off Mountains).
- deck135 s4 (tax 11,695 → stale_echo): "the game log says Your Mountain [tapped]... Therefore I
  have 0 mana. I cannot cast Lightning Bolt" — while the SITUATION line read "Mana available:
  {r} from 1 untapped source". Pure fabricated mana-lock.
- deck35 s8 (2,138), s14 (7,252), deck135 s3 (11,695): all the same re-derivation of whether a
  {R}/{1}{R} spell is payable off Mountains that are right there.
This is the deck17/deck62 "trust the rendered number / cast list is authoritative" convergence.

### C2 — TARGET SUB-MENU confusion. Root cause of ALL 7 stale_echoes + top tax.
deck49 casts targeted spells constantly (Bolt, Lava Spike, War Paint). Each opens a SEPARATE
follow-up menu listing the targets. The weak model does not model this second menu: it echoes the
SPELL name, invents a "Cast nothing" option that isn't there, or its index contradicts its own
plan. See the stale_echo audit below — every one lives here.

### C3 — BURN mis-targeting (spends the clock on non-threats / wrong side).
Burn is deck49's clock and reach. It went FACE correctly most of the time (deck35 s17, deck135
s10, deck21 s11/s13, deck17 s7, deck110 s16) — the core already races reasonably. But leaks:
- deck110 s4: Lightning Bolt → Ornithopter (0/2, a non-attacking wall) turn 1 at 18 life. A wasted
  bolt on a 0-power blocker; should go face. (Won anyway.)
- deck62 s9-s12 (the loss): spent BOTH Bolts on the opponent's board (Birds of Paradise 0/3 dork,
  a 4/4 Saproling) at T5, deployed no threat, then durdled. Killing a non-attacking mana dork while
  you are the aggressor is tempo lost, not gained.

### C4 — Goblin War Paint offered on OPPONENT-only targets → loops. (Fatal to two decisions.)
War Paint is a +2/+2/haste BUFF; casting it on an enemy creature helps THEM. When the model held
War Paint and its only legal targets were opponent creatures, it could not find a use and looped:
- deck17 s18 (tax 12,406 → unparsed_reply): the "I have Rorix? No. I have Bolt? No…" infinite
  roster loop — a whole decision burned because no good War-Paint target existed.
- deck135 s22 (tax 13,831 → unparsed_reply): the "maybe the prompt is a trick… maybe I can Steal?"
  loop, same root — War Paint offered only Icehide Golem / Ice-Fang Coatl (opp creatures).

### C5 — Crucible of Fire cast with NO Dragon in play = dead card.
Crucible pumps Dragons +3/+3; with zero Dragons it does nothing.
- deck135 s17 (T9, 0 creatures, being raced): cast Crucible into an empty board.
- deck17 s15 (T7, 1 non-Dragon Outcast): cast Crucible, no Dragon to buff.
Both were tempo wasted while under pressure. (In both games the deeper problem was mana-screwed
6-drop dragons — see C6 — but a dead enchantment made it worse.)

### C6 — DURDLE: "Cast nothing" spam + under-deployment (present in all 3 losses).
The losses all show long "Cast nothing" runs (deck135 s11/s12/s14/s20/s21; deck62 s14/s15/s17/s18;
deck17 s10/s19). IMPORTANT — reconciled against the offered set: much of this is FORCED, not a
passivity bug. In those windows the only offered casts were War Paint on enemy creatures (bad),
Crucible with no Dragon (dead), or a 6-drop Dragon the model could not afford (Shivan/Rorix on 5
lands). The genuine, model-fixable slice is small; the losses are mostly deck-construction/draw:
- deck135 loss: mana-screwed at 5 lands the whole game holding two {6} Dragons (Shivan, Rorix);
  only early creature (Bloodmark Mentor) died. Never reached 6 lands to fire Dragonmaster/Dragons.
- deck62 loss: after trading Mentor in a block T4 and spending both bolts on the enemy board T5,
  drew only 6-drops + build-around enchantments; capped at 5 lands; no clock.
Route the record component to deck-construction (top-heavy Dragon curve, only two {6} finishers,
few 3-cost bodies), not to a guide rule. The guide's leverage is C1-C5 tax + the deploy/attack floor.

## STALE_ECHO AUDIT — deck49 owns 7 (special charge). Verdict: 1 WRONGFUL, 6 correct catches.

ALL 7 occur at the C2 seam (a target sub-menu, or a cast menu where the model wanted an unofferable
card). Bucketed per the wave-13/16 echo taxonomy:

1. **deck21 s4 — WRONGFUL DOWNGRADE.** Options: ["The opponent (player, life 20)", "Raging Goblin…",
   "Yourself…"] (Bolt target sub-menu). Reply: `CHOICE: 1 (Cast Lightning Bolt targeting The
   opponent)`. Index 1 = "The opponent" = CORRECT, and the echo literally NAMES the offered option
   "The opponent" — yet it routed to stale_echo→heuristic. Cause: the option's canonical label
   carries a "(player, life 20)" suffix, so neither an exact match nor a clean substring match of the
   bare token "The opponent" (embedded in a longer phrase) fires. This is the superstring/embedded-name
   false-positive recurring in the player-target shape. Non-fatal (heuristic also bolted face; a win),
   but a genuine wrongful downgrade of a correct index. ENGINE item (see notes.md).

2. **deck35 s11 & 3. deck17 s13 — DEGENERATE CARD-NAME-ON-TARGET-SUBMENU (correct catch, correct
   index discarded).** War Paint target sub-menu offering creatures. Reply names the intended target
   correctly in the PLAN ("buff Dragonmaster Outcast") and the INDEX is correct (s11 idx1=Dragonmaster
   Outcast; s13 idx3=Dragonmaster Outcast), but the parenthetical echoes the SPELL "(Goblin War Paint)"
   — which is not an offered target option — so the matcher correctly finds no match and downgrades. Not
   "wrongful" by the strict definition (no offered option named), but a systematic model confusion: on a
   target sub-menu it re-states the spell instead of the target. Heuristic chose the same/valid target,
   so harmless here — but the correct index was thrown away.

4. **deck135 s4 — correct catch.** Bolt target menu [opponent / Icehide Golem / Yourself]. Model,
   drowning in the C1 mana false-belief, tried `CHOICE: 3 (Cast nothing right now)` — an option NOT
   present in a mandatory target menu (idx3 = "Yourself"). Echo names no option → heuristic. Correct.

5. **deck135 s18 — correct catch (Mechanism B).** Cast menu [War Paint / Lightning Bolt / nothing].
   Reply `CHOICE: 2 (Cast Crucible of Fire)` — Crucible is NOT in the list (idx2 = Lightning Bolt,
   unintended). Hallucinated-absent card; staleness route prevented an unrelated Bolt cast.

6. **deck110 s9 — correct catch (Mechanism B + plan/index mismatch).** War Paint target menu
   [opponent / Vault Skirge / Yourself / Bloodmark Mentor]. Plan says target Bloodmark Mentor (idx4)
   but the emitted `CHOICE: 1` = the OPPONENT. Echo "(Cast Goblin War Paint)" names no target option
   → heuristic. The route PREVENTED buffing the opponent with War Paint. Good catch.

7. **deck62 s3 — correct catch (Mechanism B).** Cast menu [Lightning Bolt / nothing] on turn 1 (1
   land). Reply `CHOICE: 1 (Cast Bloodmark Mentor)` — Mentor unaffordable/unoffered (idx1 = Bolt,
   unintended). Hallucinated-absent card; staleness route saved an unwanted turn-1 Bolt.

Audit verdict: staleness routing is doing its job (6/7 genuine saves, several from Mechanism-B
hallucinated-absent echoes and one opponent-buff prevention). The lone WRONGFUL downgrade is
deck21 s4 (matcher can't reconcile "The opponent" against the "(player, life 20)" suffix). The
2 degenerate catches (s11/s13) discarded a correct index harmlessly. The dominant driver of the
whole cluster is C2: deck49 is a spell-heavy deck and its every targeted cast opens a sub-menu the
weak model mishandles — addressed in the guide (teach the flow) AND notes.md (representation/matcher).
