# deck17 (Faeries) — FIRST-GUIDE VALIDATION findings (wave-16)

Corpus: `matchups-20260716-084532`, binary `/tmp/wagic-69793930a`. deck17 played all 6 games
with its FIRST shipped guide (wave-15 commit 04bf8f316) + the new hybrid-pip annotation.
**Record 4/6, up from 3/6 guideless.**

Acting-seat files (`ai_baka_deck17-*` seat):
- vs133 (mono-B recursion): **WON**, opp -1, t11. Cloud Sprite/Briarberry beatdown; even stole a
  Bloodghast (Control Magic) and attacked with it t9/t11.
- vs62 (Enchantresses): **WON**, opp -2, t10. Full flyer board every turn.
- vs21 (Goblin aggro): **WON**, opp -4, t8. Both anthem lords + wide air, fast kill.
- vs135 (snow midrange): **WON**, opp -9, t11. Two Cloud Sprites + Scion + Liege + Faerie Swarm.
- vs44 (UB Faerie mirror): **LOST**, me -4, t20. Over-walled + late/weak steal; out-raced.
- vs110 (affinity, Vault Skirge lifelink): **LOST** (adjudicated, no clean gameend at this seat).
  Over-walled: cast two Plumeveils, clocked with a lone 1/1 Cloud Sprite while opp gained to 23.

The 4 wins are the guide working as intended: attack every turn, defenders excluded, hybrid
casts on curve, fast air kills (t8-t11). Both LOSSES are the two matchups where the opponent HAS
FLYERS / gains life — exactly where the guide's core assumption ("the opponent has no flyer,
which is almost always") breaks and the deck must actually race.

## TAUGHT-CLASS VERDICTS

### CLASS 1 — HYBRID mana (rule #1): DIED as taught, MUTATED.
The off-color false belief is GONE. No game re-derived "{u/b} needs a Swamp" or miscounted
`{u/b}{u/b}{u/b}` as 6 mana (the wave-15 signature leaks). The guide's #1 rule + the new
annotation `[hybrid: each {u/b} pays with U or B - total N mana]` (on every hybrid cast line this
seat) reinforce each other: clean low-tax hybrid casts now dominate — vs21 s13 Liege rl623,
vs135 s6 Gatewarden rl604 / s14 Liege rl597, vs62 s8 Wasp Lancer rl685, vs44 s9 rl828, vs110 s11
rl753. Zero under-deployments from hybrid confusion (the wave-15 "Scion over Wasp Lancer" leak
did not recur). Guide-effect and annotation-effect both positive and mutually reinforcing.

MUTATION: the residual tax relocated to **stale within-turn mana tracking**. On a SECOND cast in
the same main phase the model re-derives mana from the battlefield Island count instead of the
explicit `Mana available:` line, then distrusts the (correct) cast list and oscillates thousands
of tokens. Clearest witness — **vs62 s13** (rl6732): prompt shows `Mana available: {u} from 1
untapped source`, cast list correctly offers only Gatewarden (1 mana; Scion already cast that
turn for 3), yet the model insists "I have 4 mana available (4 untapped Islands)... Why is Wasp
Lancer not listed?" and burns 6000+ chars fighting the correct list. Same shape vs110 s5
(rl9648). NOT the hybrid belief — mana-remaining bookkeeping. Route: representation + small guide
trust-line. Feeds latency/adjudication risk.

### CLASS 2 — DEFENDERS never attack: DIED (clean).
Grep of every final `ATTACK:` line across all 6 games: **zero** defenders (Gatewarden/Plumeveil)
ever declared as an attacker. The wave-15 leak is fully fixed. The verbose defender explanation
can be TRIMMED to a flat statement (pre-flagged). BUT a new adjacent defect lives at the
DEPLOYMENT seam (Class 6): the exclusion is in the attack rule, not the deployment rule.

### CLASS 3 — ATTACK FLOOR: FIRED where its precondition held; GAP in has-flyer matchups.
In all 4 wins the model attacked with EVERY eligible attacker, every turn — verified by matching
each `attackers` choice against the engine's `A1..An` eligibility list. Where it declared a
single attacker (vs44 s12/s20/s24, vs110 s08/s12/s14) the engine had offered only ONE eligible
creature (rest summoning-sick or defenders). **The "chronic under-attacking" hypothesis is
FALSIFIED at this seat — the model attacked optimally given eligibility.** The floor is sound; do
not touch its core. The gap: the floor is keyed on "opponent has no flyer," and in the two losses
the opponent DID have flyers/lifelink, so the floor's precondition never engaged and nothing else
told the model how to race.

### CLASS 4 — STEAL early, don't hoard: PERSISTS (in the mirror), present-and-disobeyed.
vs44: Control Magic was OFFERED at t11 (s19) and t13 (s23) with the opponent's **Scion of Oona
(anthem lord)** among the legal targets — the ideal steal (removes their anthem AND gives you a
body). The model DECLINED both, casting Plumeveil (a wall) at t11 and Mahamoti at t13, then
finally cast Control Magic at t15 (s26) on a 1/1 Scion — too late, already me7/opp16. The guide's
"do NOT hoard, steal their best creature NOW" line is present and was disobeyed. Needs sharpening
+ tie to the wall-deployment fix. (vs110: steal spells never drawn/offered — not implicated.)

### CLASS 5 — Cloud Sprite block restriction: DIED (quiet).
No recurrence of the wave-15 "does Scion share Cloud Sprite's restriction?" agonizing. Blocking
clean (vs135 s8 Gatewarden blocks ground Icehide Golem; vs44 s31 Plumeveil + Briarberry
double-block). Trim block-notes to one line.

### CLASS 6 — NEW: "cast the BIGGEST flyer you can afford" mis-counts DEFENDERS -> over-walling. ROOT CAUSE OF BOTH LOSSES.
Plumeveil is a 4/4 flyer WITH Defender. The deployment rule tells the model to cast the biggest
flyer; the model correctly reads Plumeveil as the biggest flyer on the menu and casts the wall
over a real attacker. **vs110 s7** is the smoking gun: the model reasons verbatim "A 4/4 wall
that cannot attack does not advance the clock... We need to attack," then casts Plumeveil anyway
because "it is the biggest flyer" and the guide says cast the biggest. Consequence: in vs110 its
board became Cloud Sprite + two Plumeveils, so the only eligible attacker every turn was a lone
1/1 (opp gained to 23 on Vault Skirge lifelink). In vs44 it cast Plumeveil at t11 instead of
stealing the opp's Scion, and its board attrited to a lone Mahamoti by t17. **The defender
exclusion must extend from the ATTACK rule to the DEPLOYMENT rule:** "biggest flyer" must mean
biggest flyer THAT CAN ATTACK; walls are cast only when you need a blocker this turn, never as
your development when racing. This single fix addresses both losses' deployment failure.

## Loss reframe (epistemic note)
Neither loss is an attack-floor failure or an under-attack-by-choice failure (both falsified
against eligibility lists). Both are DEPLOYMENT failures: the model spent mana on walls (Class 6)
and, in the mirror, on a wall instead of the anthem-steal (Class 4), so its clock was too thin to
win a race. The guide taught the deck to fly over a defenceless opponent (the 4-win case) but did
not teach it to RACE an opponent that flies back or gains life.
