# deck21 (Goblin Gang) — FIRST-GUIDE VALIDATION findings (wave-16)

Corpus: `matchups-20260716-084532`, binary `/tmp/wagic-69793930a`. deck21 played its FIRST
GUIDED corpus (initial guide shipped wave-15, commit 04bf8f316). Card rules text below is
derived from translog option/board annotation strings (engine reality — see notes.md), not
real-MTG memory.

## Record: 3/6 guideless (wave-15) -> 2/6 GUIDED (wave-16). Verdict: MOSTLY VARIANCE + POOL SWAP, one real signal.

| opp | w15 | w16 | how it ended (w16) | class |
|-----|-----|-----|--------------------|-------|
| 44 (UB Faeries) | WON | **LOST** my-4 op4 T12 | close flyer race, lost by 4 | MUTATED — signal E1 |
| 110 (Affinity)  | LOST | **WON** my16 op-5 T9 | killed lifelink flyer, raced | flipped positive |
| 133 (Bloodghast)| WON | **LOST** my-2 op2 T14 | mana FLOOD, empty board T5-13 | variance |
| 135 (snow ramp) | WON(pos)| **WON** my20 op-4 T9 | 20-0 blowout | persisted (improved) |
| 17 (UB Faeries) | LOST | **LOST** my-4 op4 T8 | flyer race, lost by 4 | persisted loss — signal E1 |
| 62 (Enchantress)| — (new)| **LOST** my-12 op4 T13 | 25/26 flying voltron | hard matchup |

Note 140 (a lifegain deck deck21 LOST to in w15 by durdling burn) ROTATED OUT; 62 (Enchantress
voltron) rotated IN. The pool lost an opponent deck21 would now likely BEAT (the F1a hold-burn
bug that lost vs140 is fixed) and gained a near-unwinnable one. **3 of the 4 losses are within 4
life** — deck21 is now racing everyone to single digits; the record is a coin-flip surface, not
a collapse. Decision counts fell to 18-24/game (from w15's tax-heavy 40+) with only 5 fallbacks
corpus-wide — the guide did its second job (short-circuiting the reasoning loop).

## The guide's wave-15 charge: what DIED, PERSISTED, MUTATED (per taught class, with seqs)

### DIED (fixed — no recurrence): the guide's core wins
- **F1a — hold burn as "interaction" (the w15 game-loser): DEAD.** Zero instances of holding
  burn for the opponent's turn. Every burn fired on deck21's own turn at the face or a valued
  creature: vs44 s6/s10/s12/s15, vs135 s15/s17/s20, vs17 s5/s7/s11/s13, vs133 s16, vs62 s3/s14.
  The single most important w15 teaching point is resolved.
- **F1b — burn thrown at toughness>3: DEAD.** vs44 s10: model quotes the guide, correctly
  declines to Volcanic-Hammer a 4/4 Faerie Bladecrafter, sends it to face. No "3 damage shrinks
  the 6/6" loop anywhere.
- **F2 — Spark Elemental MP1 timing: DEAD.** vs135 s3 and vs44 s3: cast Spark in Main phase 1
  and swung the same turn. No MP2 agonizing (w15 burned 2,773 words on this).
- **F3 — haste roster + summoning-sick tokens: DEAD.** vs135 s9-s10 Boggart Ram-Gang cast and
  attacked same turn (haste); s12 cast Goblin Gang Leader, s13 attacked ONLY Boggart (correctly
  excluded the just-cast Leader + tokens); s22 swung Leader + both tokens the NEXT turn. Roster
  internalized; no alpha-strike recompute.
- **F5 — hybrid {R/G} affordability: DEAD.** 7 hybrid cards cast (Scuzzback x4, Boggart,
  Boartusk x2), ZERO declines corpus-wide. Both the guide's HYBRID section AND the new engine
  annotation ("[hybrid: each {r/g} pays with R or ...]") carry it now (see notes.md — annotation
  effect). The wave-15 F5 misread class is gone.

### PERSISTED (working as taught)
- **F4 — anthem / first-strike math:** vs62 s16/s23 — Bloodmark Mentor's first strike shows on
  Scuzzback + Bloodmark and they attack into blockers profitably; Boartusk's +1/+1 pumps the
  team (Scuzzback 1/1->3/3). Named and folded into combat.
- **Exception (kill lifegain/engine on sight, toughness <=3):** vs110 s11-s12 — Volcanic Hammer
  targeted the Vault Skirge (1/1 flying lifelink) instead of the face, killing the lifegain
  engine, then raced. This WON a game deck21 LOST guideless in w15. The exception, when the
  model recognizes the target as "worth it," fires correctly.
- **F7 — don't durdle:** held wherever creatures existed. The empty-board turns (vs133, vs17
  late) were mana FLOOD (no creatures drawn), not a durdle by choice.

### MUTATED -> the one real SIGNAL (E1): "all burn to face vs a flyer swarm" backfires on SMALL, KILLABLE flyers

Both flyer losses (vs44, vs17) died by **exactly 4 life** running the guide's own instruction.
The guide's MATCHUPS-YOU-CANNOT-SOLVE section lumps "a faerie flyer swarm" together with "walls
of big or flying blockers (Plumeveil 4/4)" and says for ALL of them: *"Do NOT waste turns or
burn trying to break the wall... send EVERY burn spell at the face, and win the LIFE race."* The
model obeyed the more-specific matchup line over the exception clause:

- **vs17 s13 plan (verbatim):** *"I will not waste burn on their flyers since I cannot block
  them effectively and need to maximize my clock; keeping the Lightning Bolts in hand is correct."*
  The Cloud Sprite and Briarberry Cohort entered as **1/1 flyers** (both killable by any burn),
  then grew via liege lords to 2/2 and 3/3 (s16). deck21 sent 4 burns to the face, could never
  block, took 9 in one swing (my16->7, s15->s16), lost by 4.
- **vs44 s10 plan:** quotes the guide, keeps burn for the face while a **Faerie Miscreant 1/1
  flyer** connects every turn and a Faerie Bladecrafter grows 4/4->5/5->6/6 (s10/s15/s19) out of
  burn range. Lost by 4.

The math the guide gets wrong for THIS deck: **deck21 has no blockers, so every point of
opponent evasive power is unblockable damage EVERY remaining turn.** In a race you cannot end in
1-2 turns, an unanswered 1/1 flyer over a 6-turn game costs you 6 life — more than the 3 the
burn would deal to the face. The guide's exception ("worth more than 3 face damage... an evasive
attacker that is outracing you") never fires because (a) "outracing you" reads as needing a
big/scary flyer, and (b) "worth more than 3 face" makes a 1/1 look beneath a burn spell. And the
MATCHUP section actively countermands the exception. The window is also narrow: small flyers get
pumped out of burn range fast (Bladecrafter 2/2->6/6; Cloud Sprite/Briarberry 1/1->3/3 under
lords), so it must be an EARLY pick or none.

This is the earned first-revision: reconcile the exception and the matchup section, and give the
model the no-blocker race math so it picks off small persistent flyers EARLY.

## Games that are NOT signal (variance / hard matchup — no guide change earns from these)
- **vs133 (lost 2-2): mana FLOOD.** creatures:0 from T5 through T13 (s11/s13/s14/s18 all show an
  empty board); deck21 drew Mountains and had no body to deploy (Goblin Gang Leader stranded at
  2 mana s11). Chucked burn to face, ground out by recurring Bloodghast (2/1 cantblock). The T13
  Orcish Oriflamme cast into an empty board (s18) is a wasted card, but the game was already
  lost. Pure draw variance; F7 already covers "deploy a body every turn — when you draw one."
- **vs62 (lost -12): Enchantress voltron, near-unwinnable.** Opponent turned a Birds of Paradise
  into a **25/26 flying trampler** via Fists of Ironwood + 2x Ancestral Mask (s20). deck21
  played correctly — deployed, first-struck, attacked every turn, raced opp 20->4 — but nothing
  in the deck answers a 25/26 flyer it can't block. This is the "wall you cannot solve" matchup
  the guide already names; racing was the right (losing) line. Faint corroboration of E1: the
  BoP was a killable 0/1 engine creature early (s10) before it grew, but a 25/26 voltron was not
  foreseeable — I do NOT build a rule on this instance.

## Reasoning-tax note
Tax collapsed vs wave-15. Fallbacks: 2 unparsed (vs44 s8/s20 — format-loop, correctly routed),
1 unparsed (vs17 s19), 2 stale_echo (vs62 s8, vs133 s14 — both GENUINE staleness catches,
verified below), 0 scavenged casts. The guide pre-answered the top w15 tax spikes.

## stale_echo audit (brief item — flag wrongful downgrades)
Both are CORRECT catches, no wrongful downgrade of a named offered option:
- **vs62 s8:** tapped out ({r} floating only); reply looped on the CHOICE-line FORMAT (~"does
  CHOICE need a target?") and never emitted a parseable option match -> heuristic. Genuine.
- **vs133 s14:** reply echoed "Play Mountain" (CHOICE: 1) but the land was already played that
  turn and no Mountain remained in hand — "Play Mountain" is NOT in the current menu (hand =
  Volcanic Hammer only). Absent-echo -> heuristic, which cast Volcanic Hammer to face (s15 target
  = the opponent). Outcome-neutral, correct staleness route. The superstring fix is not
  mis-downgrading at this seat.
