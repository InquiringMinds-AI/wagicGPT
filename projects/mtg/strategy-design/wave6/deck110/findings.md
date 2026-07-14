# Deck-110 (Etched Affinity) — Wave-6 game review

**Evidence base.** 6 seat translogs from `matchups-20260713-190011/` (one run),
239 logged decisions for this seat. Card facts verified against
`bin/Res/sets/primitives/mtg.txt` `auto=`/`text=` and against the engine
(`src/AllAbilities.cpp` AEquip, `src/AIPlayerGPT.cpp describeAction`,
`src/GameObserver.cpp enchantmentStatus`). Citations are `<epoch prefix>` + seq (+ turn).

## Record: 1 WIN / 3 LOSS / 2 TIMEOUT — adjudicated 3/6. A COLLAPSE from wave-5's 5/6.

| file (epoch) | matchup | raw result | end T | end life | one-line cause |
|---|---|---|---|---|---|
| 1783996823 | 131v110 | **WIN**    | 13 | my18 / op-13 | drew bodies, went wide, closed |
| 1783991383 | 135v110 | TIMEOUT (**adj WIN 19-16**) | 7 (cap) | my19 / op16 | latency-starved early cap |
| 1783992016 | 140v110 | TIMEOUT (**adj WIN 13-7**)  | 30 (cap) | my13 / op7 | double Lightmine, no Blast to close |
| 1783988143 | 44v110  | **LOSS** | 15 | my0 / op12 | threat-starved (Mox flood) + Etched Champion COUNTERED |
| 1783993782 | 110v109 | **LOSS** | 9  | my-6 / op5 | close aggro race lost by ~1 turn |
| 1783995813 | 110v133 | **LOSS** | 11 | my0 / op10 | drew the 3 weakest bodies, too slow, ground out |

**Harness sweep (required, clean):** `fallback` absent in all 239 records; head-first
parser desync 0 across parseable ask+priority decisions. Every misplay below is a GENUINE
decision, not a parser/no-op-parse artifact.

---

## CENTRAL QUESTION — what killed it? VERDICT: VARIANCE + deck-construction on n=6, NOT guide, NOT pilot decay.

The wave-5 skill names this exact situation: *"A falling record on n<=6 against a harder
opponent slate is variance, not evidence the guide broke."* The three new losses were all
wins last wave; all three share a precondition **the guide cannot create** — a playable
threat curve. This is **OBEYED-BUT-LOSING mode**. Discriminators that rule out guide/pilot:

- **The one win and both adjudicated timeout-wins prove the deck still executes when it
  draws bodies.** vs131 (WIN): opp 20->19->6->2->-13 as the pilot deployed Memnite/Steel
  Overseer/Signal Pest and attacked with all of them (s16/s23/s33/s38). Same pilot, same
  guide, one variable flipped (a real board).
- **Cast-nothing is healthy (~12/117 ask ~= 10%)** — no paralysis, no durdle regression.
- **The obeyed wave-5 lines still fire**: mana falsity-fact held (no "I have no mana"
  delusion recurred), Galvanic-Blast-to-face reliable, go-wide deployment when bodies exist.

### Per-loss attribution

**1) vs44 — LOSS (opp 12). VARIANCE (threat starvation) + ECOSYSTEM (counter) sealing it.**
- The draw was a Mox-Opal/land FLOOD: Memnite T1, then nothing but Mox Opal / Glimmervoid /
  land / Cranial Plating for ten turns; the only real creatures were Etched Champion (T13)
  and Vault Skirge (T15). The pilot correctly kept firing Galvanic Blast to face (opp
  20->16->12) but had no board.
- **The ecosystem/counter hypothesis is CONFIRMED but SECONDARY here.** Narration
  (`1783988143`, last prompt): `Your Etched Champion: stack -> graveyard` immediately
  followed by `Opponent's Arcane Denial: stack -> graveyard` — deck44 **countered the one
  real threat the deck managed to cast**. Affinity's discrete high-value threats are prime
  counter bait, and with the stack now surfaced + counter options labeled, deck44 uses them
  (corpus-wide it countered 13/14 windows per the deck44 reviewer). But the game was already
  lost to no-board: opp never dropped below 12. The counter removed the last-ditch threat; it
  did not swing a live game.
- Minor combat cost: T3 the pilot attacked its equipped **6/1 Memnite into a 1/1
  Faerie Miscreant block** (`Damage: 1 dealt by Faerie Miscreant to Memnite` ->
  `Your Memnite: battlefield -> graveyard`). Cranial Plating adds power, not toughness, so
  the carrier trades DOWN to any chump. Losing the only carrier to a 1/1 was a real (small)
  misplay, but with the flood behind it, immaterial to the result.
- Attribution: **VARIANCE-primary, ECOSYSTEM-secondary.** Not guide, not pilot decay.

**2) vs109 — LOSS (opp 5). MATCHUP/VARIANCE (close race) + one attack-discipline leak.**
- A genuine aggro-mirror race, lost by roughly one turn: the pilot got opp to 5 with two
  creatures (Memnite, Signal Pest) + four face Galvanic Blasts (opp 20->19->17->13->11->7->5).
  Its own life fell 20->17->8->-6.
- **The one model-fixable leak:** at s19/s20 T8, life **8**, it attacked all-out with both
  creatures (tapping them) when the swing wasn't lethal (opp 5, 2 damage available) — then
  died to the crackback on T9. **There is NO blockers record in the entire corpus** (see
  Convergence Watch): the pilot tapped out its would-be blockers attacking, so no block
  window ever opened. THE RACE's "chump at <=8" instruction cannot fire against a creature you
  already tapped attacking. The gap is at the ATTACK seam, not the block seam.
- Attribution: **MATCHUP/VARIANCE-primary** (close race vs faster aggro), **minor PILOT**
  (attacked with a needed blocker at 8 life). n=1.

**3) vs133 — LOSS (opp 10). VARIANCE (weakest-creature draw) + too-slow clock.**
- Drew the three WEAKEST bodies in the deck: 2x Signal Pest (0/1 base) and Ornithopter
  (0/2). Chipped opp 20->10 over eleven turns — the guide's "kill by turn 5-6" missed by
  double — and got ground out by Phyrexian Obliterator (5/5 trample) + Gray Merchant drain.
- Etched Champion finally arrived T10 (s41) but was summoning-sick that turn; the pilot's
  s45 plan named it as an attacker while it was NOT in the offered `A#.` list — **full
  commitment to a creature the engine didn't offer, NOT a partial-answer leak** (per the
  wave-5 denominator rule). Correctly excluded.
- The game generated 30 priority windows of mostly `pass`, plus no-op re-equips (below) —
  grind that also feeds latency.
- Attribution: **VARIANCE-primary** (no clock in the draw). Not guide, not pilot.

**Bottom line on the collapse:** three independent variance-driven losses (flood, close
race, weak-body draw) against a slate that now includes a working counter deck. The guide is
not the cause. Freeze it; route the record to a deck-construction flag (below).

---

## RE-ATTACH NO-OP MARKER — SHIPPED BUT INERT (engine bug found). Task audit.

**Count this corpus: 9 of 18 equip/attach selections (50%) were no-op re-equips**
(target already showed two power numbers, e.g. `(8/1) (printed 1/1)`, plating already
attached). Wave-5 was 15/35 (43%). Fewer equips overall (18 vs 35), same-or-worse waste
fraction. Every carrier that was legitimately equipped got re-equipped as a no-op on the
next window: Vault Skirge s14 LEGIT->s15,s19 NOOP; s17 LEGIT->s18,s21 NOOP; Steel Overseer
s37 LEGIT->s38,s39 NOOP; s17 LEGIT->s18 NOOP; Signal Pest s8->s25; Etched Champion s42->s43.

**The shipped marker fired ZERO times.** The string
`(ALREADY attached to it - this would change NOTHING)` — `AIPlayerGPT.cpp:1156` — appears
**0x** in all 6 deck110 prompts (`/usr/bin/grep -c "change NOTHING"` = 0 everywhere).

**ROOT CAUSE (engine bug, confirmed in code):** `describeAction` gates the marker on
`if (moved && moved->auraParent == action.target)`
but **equipment does not use `auraParent`.** `AEquip::equip` stores the host in
`source->target` (`src/AllAbilities.cpp:10431`: `source->target = equipped`), and
`GameObserver::enchantmentStatus` (`src/GameObserver.cpp:1248`) sets `auraParent` **only for
cards that are AURAs and explicitly NOT `TYPE_EQUIPMENT`**. Cranial Plating is Equipment, so
`moved->auraParent` is **permanently NULL** — the marker condition is structurally
unsatisfiable for the exact card it was built to fix. **FIX:** also test
`moved->target == action.target` (see notes.md #E1). This is the wave-5 representation fix;
it was designed correctly and shipped to the wrong field.

**Pilot obedience to the BOARD cue: still absent (read-then-disobey, 2 waves running).** The
pilot reads the two-power done-signal and verbalizes it correctly, then re-equips anyway:
- `1783991383` s15: PLAN "...Vault Skirge is listed as (17/1)... the Plating is **already
  attached**... Equipping it again (Option 8) costs {1} and does nothing." -> chose the equip.
- `1783995813` s43 (Etched Champion), s25 (Signal Pest): same shape.
This reconfirms the wave-5 ceiling: the perception cue on the BOARD line is read and
disobeyed; the marker on the OPTION line is the real lever — and it was never actually
delivered. **Do not escalate guide wording; fix the marker (notes #E1).**

---

## DISPLACEMENT / OVERSHOOT AUDIT on the wave-5 rewrites (mandatory)

**Lightmine exception rewrite — LANDED, IMPROVED, no overshoot, no displacement.** Wave-5
replaced "attack with your single equipped creature" (a suicide order — Plating gives no
toughness) with "do NOT attack; win by Galvanic Blast; attack only with toughness >
Lightmine damage." vs140 has **double** Lightmine Field. The pilot declared attackers only
**once all game** (s23 T11, before the fields mattered) and correctly declared **no
attackers for the remaining ~19 turns** into the double field. Result: adjudicated **WIN
13-7**, versus wave-5's **loss my7/op15** in the same matchup. The fix moved the outcome the
right way. The residual — couldn't close from opp-at-3 (s38 T21) — is **deck construction**:
no Galvanic Blast drawn in the window, and the deck runs only 4. Not an overshoot.

**Equip-brake re-key (flat per-turn cap -> "once per Cranial Plating you control") — no
observed harm.** No case of a legit second-plating attach being suppressed; legit 2-power
jumps happened where warranted. The re-key did NOT cause over-*casting*. The no-op RE-equip
loop is the SAME representation defect (above), not a brake overshoot.

**No other overshoot/displacement.** Cast-nothing ~10% (healthy). Go-wide produced no
suicide attacks outside the (correctly-avoided) Lightmine case.

---

## CONVERGENCE WATCH — reflexive high-life blocking: N/A this corpus.

**ZERO `blockers` decisions across all 6 games.** The block seam never arrived (the pilot's
creatures were tapped from attacking, or games ended on the crackback without a window). So
reflexive high-life blocking cannot be occurring, and it is NOT a lever this wave. The
related real leak is upstream at the ATTACK seam (vs109: attacking with a needed blocker at
8 life) — addressed as a tight clause, not a block rule.

---

## DECK-CONSTRUCTION / META FLAG (obeyed-but-losing — route upstream, not a guide failure)

deck110 is now structurally exposed on three axes the guide cannot fix:
1. **Flood / dead-draw risk.** 25 "creatures" but 8 of them are ~0-power bodies (4 Signal
   Pest 0/1, and Ornithopter 0/2 among the 0-drops), plus 3 Mox Opal + 4 Cranial Plating +
   20 lands. Threat-light or all-Signal-Pest draws (vs44 flood, vs133) produce no clock.
2. **Counter-vulnerability (the ecosystem shift).** Wins route through a few discrete
   high-value threats (Master of Etherium, Etched Champion, Cranial Plating) — exactly the
   cards a now-effective counter deck (deck44, stack surfaced + labeled) answers 1-for-1.
   Wave-4/5 dominance was partly the blind-counter seam; that seam is closed.
3. **Only 4 Galvanic Blast** = the deck's sole reach through Lightmine/stall. Not drawing it
   in the closing window = can't finish (vs140).

Per the skill: **freeze the obeyed lines, spend the wave on the one model-fixable lever, and
route the record itself to this flag.** The one model-fixable lever is the no-op re-equip,
whose durable fix is the engine marker (notes #E1), not guide text.

---

## Handoff for the guide revision (OBEYED-BUT-LOSING / constrained diff)

Losses are variance/construction; the guide worked where the draw allowed. Revise as a
FROZEN-LINE diff off the wave-5 guide. New text only at two tiny, specific, model-fixable
leaks; everything else verbatim:
1. **THE RACE, at <=8 life:** don't attack with a creature you need to block with — a tapped
   attacker can't block (vs109 s19/s20). One clause inside the existing RACE block.
2. **Counter-aware go-wide (vs44):** against a blue/counter opponent, lead with your
   cheapest bodies and go wide so one counter can't stop the clock; don't jam your lone
   Master of Etherium into open blue mana as your first play. Value-anchored, brief — NOT a
   "hold your threats" rule (that would durdle a fast deck).
3. **DO NOT** weaken: the mana falsity-fact, THE RACE core, ATTACK EVERY TURN, LETHAL CHECK,
   Blast-to-face, carrier order, the Lightmine rewrite (working), the equip brake, or the
   mulligan.

The no-op re-equip residual routes to engine notes #E1 (the marker is broken, not the guide).

## FREEZE-CHECK (obeyed-but-losing mode)
Every wave-5 line survives verbatim in the wave-6 strategy.txt EXCEPT the two additive
clauses above (RACE <=8 no-attack-with-blocker; counter-aware go-wide). Verified:
- Identity block / kill-by-5-6 / cast-nothing=lost-turn / beatdown-by-default — VERBATIM.
- Rule #1 equip brake (two-power done-signal + once-per-plating + mana->creatures/Blasts) — VERBATIM.
- CASTING mana falsity-fact + Memnite/Ornithopter {0} — VERBATIM.
- THE ENGINE (cast-Plating-T1 / equip-first-{1} / carrier order / power-only) — VERBATIM.
- THE RACE (go-wider / chump-at-8) — VERBATIM + the <=8 no-attack-with-needed-blocker clause appended.
- ATTACK EVERY TURN / Signal Pest / Etched Champion / Lightmine rewrite — VERBATIM.
- LETHAL CHECK / Blast-to-face — VERBATIM.
- OTHER KEY CARDS (Steel Overseer / Master / Thoughtcast / Mox Opal / Glimmervoid) — VERBATIM.
- MULLIGAN / DECIDING SITUATIONS recap — VERBATIM (+ one counter-aware situation line).
