# deck21 (Goblin Gang swarm aggro) — REVISED-GUIDE VALIDATION findings (wave-17)

Corpus: `matchups-20260716-151559`, binary `/tmp/wagic-7cdcf9c73`. deck21 plays its FIRST
corpus under the WAVE-16 REVISED guide (commit adding the RACE-MATH EXCEPTION / no-blocker race
math). Card rules text below is derived from translog option/board annotation strings (engine
reality — see notes.md), not real-MTG memory.

## Record: 2/6 (WON vs 135, 17 · LOST vs 35, 62, 49, 110). Verdict: STRUCTURAL FLOOR + variance, CLEAN core execution, ONE costly revision MISFIRE (E1').

| opp | result | how it ended | class |
|-----|--------|--------------|-------|
| 135 (snow ramp)    | **WON** my20 op-5 T15  | 20–0 blowout, never took damage | clean win |
| 17 (UB Faeries)    | **WON** my14 op-1 T11  | fast race; flyers came late + had shroud | clean win (w16 by-4 loss FLIPPED) |
| 35 (Slivers)       | **LOST** my-2 op2 T11  | raced to my14/op5 then no board to finish; slivers went wide + gained flying, alpha'd | variance (burn-flood draw) |
| 62 (Enchantress)   | **LOST** my-1 op7 T15  | Yavimaya Enchantress 5/9 voltron; **wasted a Volcanic Hammer on a 0/1 BoP** | STRUCTURAL voltron + **E1' misfire** |
| 49 (Dragons/RG)    | **LOST** my-3 op4 T11  | close race; declined to kill Dragonmaster Outcast (→3/3 via War Paint, the lethal clock) | close + kill-on-sight-engine miss |
| 110 (Affinity)     | **LOST** my-9 op14 T6  | Cranial Plating → 6/1 then 13/1 Memnite by T5; killed the first, Affinity replayed | STRUCTURAL (artifacts wall) |

**Headline:** the pool ROTATED toward decks structurally hostile to a no-removal / no-evasion
swarm — Affinity's Cranial-Plating army (110), Enchantress voltron (62), and a Sliver deck that
grants its whole board flying+pump late (35). deck21's veteran w16 flyer nemesis (44, UB Faeries)
rotated OUT and its remaining flyer matchup (17) FLIPPED to a WIN. The record floor is DOMINANTLY
STRUCTURAL + draw variance with CLEAN core execution — **no taught class regressed**. The one
non-structural, decision-driven finding is that the WAVE-16 REVISION itself FIRED and MISFIRED
(below): it earns a surgical tightening, not a rollback.

## STEP 0-ter: what DIED / PERSISTED / MUTATED (per taught class, with seq repros)

### PERSISTED (working as taught — no regression)
- **burn-as-clock / face-by-default (w15 F1a — the w15 game-loser): STILL DEAD.** Zero holds of
  burn "for interaction." Every burn fired on deck21's own turn: vs135 s6/s11/s13/s18 (all face),
  vs17 s11/s15/s25 (face), vs35 s7/s10/s12/s15/s19 (face), vs49 s7/s10/s14/s16 (face). The single
  most important w15 teaching point remains resolved a full wave later.
- **burn-at-toughness>3 (w15 F1b): STILL DEAD.** No burn thrown at an uncastable-toughness body.
  Note the ONE apparent exception is CORRECT: vs110 s7 Lightning-Bolted "Memnite (6/1)" — that is
  6 POWER / 1 TOUGHNESS (Cranial Plating pumps power only), killable by 3 damage; removing the
  plated attacker was the right kill, not a toughness-blunder.
- **Spark Elemental MP1 timing (w15 F2): STILL DEAD.** Every Spark cast in Main phase 1 and swung:
  vs135 s9, vs17 s9, vs35 s3, vs110 (Spark cast+swung T3). No MP2 agonizing.
- **haste roster + summoning-sick tokens (w15 F3): STILL DEAD.** Boggart Ram-Gang cast-and-attacked
  same turn (haste) vs17 s13, vs49 s17; Goblin Gang Leader + tokens correctly deployed a turn
  ahead of the alpha. No alpha-strike recompute errors.
- **anthem / first-strike math (w15 F4): PERSISTED.** vs17 s6 Bloodmark Mentor deployed early; its
  first strike let a 1/1 Raging Goblin KILL Oona's Gatewarden and live (s8), and carried the
  whole race. vs135 Orcish Oriflamme + Goblin King deployed onto a board (never into empty).
- **hybrid {R/G} affordability (w15 F5): STILL DEAD.** 8 hybrid-annotated prompts, 0 declines
  (Scuzzback x3, Boggart x3, Boartusk). The "[hybrid: each {r/g} pays with R or G]" annotation
  continues to carry it (notes.md). w15's misread class stays gone a wave later.
- **don't durdle (w15 F7): PERSISTED.** A body deployed whenever one was castable. Empty-board
  turns (vs35 mid-late) were a burn-flood DRAW (no creature ever offered after s3), not a durdle.
- **blocking = default-none (guide's BLOCKING section): VALIDATED.** Combat is engine-auto-resolved
  (see "1 blocker decision" below); the single escalated block (vs62 s25, my5 vs a 5-power
  trampler) was a CORRECT lethal-survival gang-block. The default-no-block posture held all corpus.

### DIED: nothing regressed to a prior failure class. (No class died back.)

### MUTATED → the one real SIGNAL (E1'): the WAVE-16 RACE-MATH REVISION FIRED, and OVER-APPLIED on a 0-POWER flyer.

The w16 revision added: *"spend ONE burn EARLY to kill a small flyer (toughness 3 or less) — even
a 1/1 … an unanswered 1/1 flyer costs you MORE life than the 3 a burn spell deals to the face."*
It scopes the target on **toughness** (killability) but never on **POWER** (the thing that
actually generates the per-turn race cost).

- **vs62 s5→s6 (T2, my20 op20) — the misfire, verbatim:** the model QUOTED the revision — *"my
  strategy guide explicitly states to spend one burn spell early to kill a small flyer (toughness
  3 or less) when racing a flying deck I cannot block, as unblocked flyers deal dama[ge]…"* — and
  Volcanic-Hammered a **Birds of Paradise (0/1) [flying]**, a 0-POWER mana dork. A 0-power flyer
  deals **0** combat damage per turn; the race-math premise (power × turns) evaluates to ZERO, so
  the exception should NOT have fired. This spent a 3-damage burn (the deck's clock) to buy zero
  tempo. **deck21 lost vs62 by exactly 1 life (my-1), racing the opponent only to op7** — had that
  Volcanic Hammer gone face, opp reaches 4, plausibly within reach of the T12 Goblin Gang Leader
  alpha before deck21 died. Costly, plausibly game-deciding.
- **Self-correction confirms the model CAN reason it right:** on later turns vs62 s14/s16 the model
  correctly noted the BoP "deals 0 damage to me this turn" and sent its Lightning Bolts to the
  FACE. So the misfire is a ONE-TIME early-reflex error the guide can pre-empt — the model just
  needs the POWER floor stated so it doesn't fire on turn 2.
- **The over-application GUARD works elsewhere:** vs17 s20/s21 — the model correctly DECLINED to
  bolt two 1/1 flyers (Cloud Sprite, Briarberry Cohort) because *"I win next turn regardless"* —
  the revision's "in a race you can win in the next turn or two, ignore the flyers and go face"
  clause fired correctly. And vs17's opposing flyers had SHROUD (untargetable) — so the flipped
  vs17 WIN was NOT driven by the revision; it was a clean fast race the revision merely didn't
  botch.

**Net on the w16 revision: PARTIAL PASS.** The "don't-burn-a-flyer-when-you-can-win-soon" guard
holds; the "don't-burn-a-flyer-that-isn't-a-clock" guard is MISSING and cost a game. The earned
fix is the classic first-revision TIGHTENING (w16 method-headline-1(d)): add a POWER floor — the
race cost is the flyer's POWER per turn, so a 0-power flyer (a mana dork like Birds of Paradise)
is NOT a clock; ignore it and go face. See strategy.txt.

### SECONDARY signal (WATCH, one confounded instance): kill-on-sight ENGINE not recognized.
- **vs49 s13/s15 (T6, my13 op13):** deck21 held TWO Lightning Bolts and chose FACE both times,
  leaving a bare **Dragonmaster Outcast (1/1)** — whose option text legibly reads *"…put a 5/5 red
  Dragon creature token…"* (a token-making ENGINE the guide's kill-on-sight clause names). The
  model's reply: *"killing it yields no immediate advantage over hitting face."* It later took
  Goblin War Paint (+2/+2) → a **3/3** that anchored the lethal clock (3/turn ×3 turns = 9). **deck21
  lost by 3.** Killing the 1/1 Outcast (toughness 1) at s13 plausibly flips the game. CONFOUND: the
  War Paint pump wasn't foreseeable at s13, and the "6+ lands" dragon condition wasn't yet live, so
  the model's "no immediate advantage" was semi-reasonable. This is the SAME shape as w16's E1 (the
  model under-values killing a small opposing body that becomes an out-of-burn-range threat) and it
  reinforces a structural fact about this seat (see below), but it is ONE confounded instance — it
  earns a light sharpening of the kill-on-sight ENGINE example, not an over-broad new rule.

## "Only 1 blocker decision all corpus" — ANSWERED: engine auto-combat, not a data gap.
The engine AUTO-RESOLVES combat for this seat. Across 6 games: 101 GPT `ask` decisions, ALL in
Upkeep(3)/Main-1(95)/Main-2(3) — **zero attacker asks and zero routine blocker asks**. Attackers
are auto-declared (25 `kind:attackers` log records, e.g. "You declared attackers: …" with no GPT
prompt) consistent with the deck's all-out-aggro posture, and blocks default to NONE automatically.
The GPT is escalated a BLOCK decision ONLY when the auto-heuristic detects a would-be-LETHAL swing
— which happened exactly ONCE: **vs62 s25 (my5, facing a 5/9 trample Yavimaya Enchantress = lethal
if unblocked)**, and the model made the correct call (choice 3: gang-block all three goblins → 1
tramples through to my4, survives). So "1 blocker decision" is STRUCTURAL to the engine's
auto-combat + the deck's default-no-block posture, and it exactly matches the guide's BLOCKING
section ("default: declare NO blockers; block only to survive lethal"). Not a data gap, not a bug.

## STRUCTURAL vs DECISIONS: the 2/6 floor is STRUCTURAL with clean execution.
- **vs110 (LOST T6): STRUCTURAL.** Affinity's Cranial Plating turned Memnite into a 6/1 (T3) then
  13/1 (T5) attacker. deck21 correctly bolted the first plated Memnite (s7, toughness 1) and raced,
  but a mono-Mountain no-removal swarm has no answer to a turn-4 13/1 that Affinity simply replays.
  Turn-6 blowout, clean decisions. This is the pool's "artifacts wall."
- **vs62 (LOST -1): STRUCTURAL voltron + the E1' misfire.** Yavimaya Enchantress 5/9 trampler is a
  wall deck21 cannot break; racing was the right line (opp 20→7) and the correct lethal-block bought
  a turn. But the T2 BoP misfire (above) threw away 3 clock in a game lost by 1 — the one place a
  decision, not structure, shaped the margin.
- **vs35 (LOST -2): VARIANCE.** A burn-heavy/creature-light draw (NO creature offered after s3);
  deck21 raced correctly to my14/op5 but had no board to finish and got alpha'd once the Slivers
  went wide and gained flying. Same class as w16's vs133 flood. Corroborates the construction flag.
- **vs49 (LOST -3): close race + the one debatable miss** (Outcast, above). Semi-structural (Dragons
  pump/fly) with a single kill-priority signal.
- **vs135, vs17 (WON): clean.**

3 of 4 losses are structural or draw-variance with clean core play; the remaining decision content
is entirely the kill-priority / race-math JUDGMENT family (E1' + the Outcast watch), which is the
guide's own frontier, not a core-class regression.

## Reasoning-tax / fallbacks
2 fallbacks corpus-wide (~2% of 101 asks — matches the corpus best-ever 3.3%), both benign
unparsed_reply: vs35 s17 (a LAND decision — heuristic played the Mountain, outcome-neutral) and
vs110 s5 (a CAST decision — model rambled about an unaffordable Boggart; next seq it bolted the
Memnite; wasted round trip only, game already structurally lost). 0 stale_echo, 0 defer, 0 empty.
No scavenged casts. The guide continues to short-circuit the w15 reasoning loop.
