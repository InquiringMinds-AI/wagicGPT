# Deck-140 review — wave 6 findings

Deck: **"Wipe Them Out!"** — RBW control, 14 board wipes, ~2 creatures (Blightsteel Colossus x2),
finishers = Rakdos's Return / Staff of Nin / Blightsteel Colossus. Plan: sweep, deploy a finisher,
grind their face to 0 on reach. It out-survives the pool and under-damages it — its win rate lives
on closing.

Corpus: `matchups-20260713-190011` (round-robin, `-j` concurrent), 6 deck140 games.
Record from results.tsv + adjudication:
- vs131 **W** (t34, my143/opp6) — Blightsteel infect kill, the 143-life grind
- vs44  **L** (t20, my-2/opp20) — never dealt 1 damage; tempo/counter deck out-raced the sweeps
- vs109 **L** (t27, my0/opp12) — out-attritioned; swept forever, no clock
- vs133 **T** (t20) — adjudicated **W** ahead 25-15; latency-starved control mirror
- vs110 **T** (t30) — adjudicated **L / behind 7-13**; **piloting loss** (Findings 1+2), not the
  "winning-but-slow reach-starve" wave 5 called it
- vs135 **T** (t17) — adjudicated **draw 16-16**; latency-starved + one wasted X=1 burst

Net ~ 1 raw W, 2 L, 1 adj-W, 1 adj-L, 1 draw. **Both model-fixable levers this wave live in vs110,
the most winnable of the non-wins.**

Severity legend: [FIXED] — [GUIDE] — [STRUCTURAL] (deck/infra, not a guide/model bug).

---

## Harness / executability sweep (run FIRST — all clean)
- **Fallbacks: 0.** Every decision was the model's own.
- **Desyncs: 0 / 295** parseable-head records across all 6 games (reply's first integer == `choice`).
- **Menu numeric-offset:** resolved — the X reversal holds (Finding 3). One deliberate X=0 (vs110
  seq35, cycling Black Sun's Zenith into an empty board), not an offset bug.
- **Latency:** heavy `-j` saturation — spikes to **60.9s / 61.7s** (vs110, vs109), median 8.6-10.3s
  (solo baseline ~4-5s). Two timeouts are latency-bound (Finding 7).

The corpus is clean at the harness layer. Every leak below is bad play or deck construction.

---

## TOP FINDINGS

### 1. [GUIDE — the #1 rule FAILED] Rakdos's Return hold-for-kill was disobeyed 6 of 7 casts
Wave 5 made *"RAKDOS'S RETURN IS YOUR KILL SHOT, HELD FOR LETHAL"* THE #1 RULE. Wave 6 shows it did
**not** take. Of 7 Rakdos's Return casts, **only 1 (vs131 t34, X=11) was the held kill**; the other 6
were early chips for tiny X:

| game | turn | X | opp life at cast | my life | note |
|---|---|---|---|---|---|
| vs44  | 17 | 3 | 20 | 9  | desperate, dying to aggro; likely countered (opp stayed 20) |
| vs133 | 8  | 3 | 20 | 21 | full life both sides — pure "establish pressure" misplay |
| vs110 | 8  | 3 | 18 | 20 | **both copies burned** — forfeited the winnable game |
| vs110 | 10 | 3 | 13 | 20 | second copy, opp not pressuring |
| vs109 | 18 | 4 | 20 | 11 | opp hand=0, discard worthless; opp at full 20 |
| vs135 | 5  | 1 | 20 | 20 | **X=1 on turn 5** — one damage; intent collapse (below) |

**This is not simple hoard-vs-fire noise: the model HOLDS Rakdos correctly on most turns.** In vs133 it
picked "Cast nothing" over an offered Rakdos 4 times (seq7/9/17/19) before firing at t8; in vs110 it
held at t2/t4. The capability is there — it lacks a STABLE rule, and leaks the burst on ~6 turns.
Three mechanisms drive the leaks, and the guide is complicit in two:

- **(a) The guide's own cast-nothing rule FORBIDS the correct hold.** Guide lines 57-59:
  *"'Cast nothing' is correct ONLY when every offered spell is a sweeper or Cruel Edict AND the
  opponent has no creatures — otherwise cast something."* Rakdos's Return is **not** a sweeper, so on a
  turn where the board is empty and the only castable spell is a below-lethal Rakdos's Return, the
  guide says "cast something" → the model fires the burst. **vs110 t8 seq11 is the proof:** the options
  were literally only `1. Cast Rakdos's Return` / `2. Cast nothing`; the PLAN says *"Cast Staff
  immediately… Keep both copies of Rakdos's Return for a lethal finish"* — but Staff {6} was
  unaffordable (5 mana), and with "hold" ruled out by the guide it took option 1. The #1 rule (hold)
  and the cast-nothing rule (don't hold non-sweepers) **directly contradict each other**, and the
  cast-nothing rule wins because it names the actual option in front of the pilot.
- **(b) The general prompt's burn-prior overrides the hold.** vs133/vs109 plans rationalize the early
  fire as *"establish immediate pressure / disrupt their hand"* — the core's *"direct damage and burn
  are part of your CLOCK… send them at the opponent's face rather than saving them"* pattern-matches
  Rakdos's Return (it deals X to the face). The guide never overrides this prior **by name**.
- **(c) Intent collapse / first-option pick.** vs135 t5 seq4: options `1. Cast Rakdos's Return /
  2. Cast Pyroclasm / 3. Cast nothing`; the PLAN says *"Cast Pyroclasm to destroy the Icehide Golem…
  hold Rakdos's Return for lethal"* — and it picked **option 1** (Rakdos, X=1). Correct plan, betrayed
  at the number. Deck44-class PLAN/CHOICE MISMATCH; Rakdos's Return sits at option 1 in ~11 of the
  windows it was offered, so first-option bias lands on it.

**Cost:** decisive in vs110 (both kill shots spent at X=3 while opp sat at 18/13 and deck140 at full
20 — had one been held, Staff pings ground opp to ~14 by t22, an X=8-12 kill was on the table; instead
timed out behind). Elsewhere it wasted the burst in already-losing/drawn games. **This is the #1
model-fixable lever.** Fix = (i) expand the hold/cast-nothing exception to explicitly include a
below-lethal Rakdos's Return, resolving the contradiction; (ii) override the burn-prior by name;
(iii) a targeted anti-first-option hold line.

### 2. [GUIDE — the un-veto's first corpus] Sweepers wasted into creatureless boards, driven by "sweepers kill artifacts"
The hint-veto fix means the model's sweeper picks now resolve (21 sweeper/edict picks, ~0 validation
failures — the fix works). But with the deck-file's `restriction{type(creature)~morethan~1}` guard
GONE, **timing discipline collapsed in the artifact matchup.** ~7 of 22 sweeper/edict casts hit a
board with **0 creatures**:

- **vs110 (5 wastes):** Pyroclasm t4, Pyroclasm t20, Wrath t20, Pyroclasm t22, Black Sun's Zenith t22
  — all into an affinity board of **Mox Opal / Cranial Plating / Glimmervoid** (artifacts, no
  creatures). The PLANs are explicit and **wrong**: seq28 *"Cast Pyroclasm to destroy all opponent
  artifacts (Mox Opal, Cranial Platings, Glimmervoids)"*; seq29 *"Cast Wrath of God to clear the
  opponent's board of artifacts."* The model believes board wipes destroy artifacts.
- **vs109 (2 wastes):** Black Sun's Zenith X=3 into 0 creatures at t14 and t22 (board = only
  Mountains). Card-neutral (BSZ shuffles back) but burns 5 mana + a turn while deck140 is dying.

**Verified against the engine `auto=`** — creature-only, no exceptions:
- Pyroclasm `auto=damage:2 all(creature)`; Wrath/Damnation `auto=bury all(creature)`;
  Black Sun's Zenith `auto=counter(-1/-1,X) all(creature)`. **None touch artifacts or enchantments.**

The guide says *"WHICH SWEEPER (only when the opponent HAS creatures…)"* and gates cast-nothing on "no
creatures" — but never states the corrective FACT the model's false belief needs. Fix = a flat fact
anchored to the named cards: **your sweepers hit CREATURES ONLY; against an artifact/land board with
no creatures they do nothing — hold them.** (Also cost vs110: 4+ turns of mana and finite
Wrath/Damnation/Pyroclasm dumped on artifacts instead of held for the real creatures — Steel Overseer,
equipped Vault Skirge — that came later.) One intent-collapse lives inside this leak (vs110 seq33:
PLAN *"casting Pyroclasm now is a waste… I must not cast it"* then chose Pyroclasm) — representation,
handoff-only, not a guide line.

### 3. [FIXED — keep compressed] X-menu discipline is solved
7 X announcements, realized X at the maximum in **7/7** by intent (values 1, 1, 3, 3, 3, 4, 11; every
pick was option 1 = the largest offered). The lone X=0 (vs110 seq35) was a **deliberate** choice to
cycle Black Sun's Zenith into an empty board — the model reasoned it, not an offset bug, immaterial
(0 creatures either way). The live guide's flipped rule ("Pick OPTION 1 — the largest X; confirm it
reads the biggest number shown") is correct and followed. **Keep the one-line confirm.** The wave-4
index/value trap is dead two waves running.

### 4. [WIN ANALYSIS] vs131 143-life game — Blightsteel infect won it; Venser's Journal was survival, not a win path
What won: **Blightsteel Colossus attacking t34 (11 power, infect = 11 poison ≥ 10 lethal)**, with
Rakdos's Return X=11 fired the same turn (opp 17→6) as insurance. The opponent ran Prism Ring lifegain
and sat at 18-24 all game, so **normal damage could not close** — Blightsteel's infect **ignores their
life total and lifegain entirely** (poison is a separate track). Venser's Journal (cast t8/t10,
`auto=@each my upkeep:life:phandcount` = gain 1 life per card in hand each upkeep) plus Staff's card
draw ballooned deck140 to **143 life** — that kept it alive through the grind but **did not itself
win**. Teaching value: (a) **Blightsteel is the answer to lifegain decks** — its poison clock is
unaffected by their life gain — worth one keyed line; (b) Venser's Journal is a **survival tool vs
aggro/lifegain**, already correctly on the deploy list; do NOT promote it to a win condition. The 143
life is a symptom of the slow clock, not a strategy to pursue.

### 5. [FIXED — demote] Staff-of-Nin targeting still solved
Staff pings went to the opponent's face essentially every time; the few creature-targeted pings were
toughness-1 kills or immaterial 0/2s. The wave-4 waste (pinging 2/2 Guttersnipe) does not recur.
Demote to the one compressed line already in the guide (face by default; a creature only when its
toughness is shown as 1).

### 6. [CONVERGENCE WATCH — clean] Reflexive high-life blocking not observed
deck140 runs ~no creatures (only Blightsteel), so `blockers` decisions barely arise and the one combat
record (vs131, Blightsteel attacking) was correct. The reflexive-block convergence watch is **N/A for
this deck** — no evidence either way.

### 7. [INFRA + one revision] Timeout triage ×2 (plus the third)
- **vs133 (t20, 94 records, adj W 25-15):** latency-starved control mirror. Spikes 41.6s / 30.8s /
  21.8s, median 8.6s; 94 decisions at the 2400s cap ≈ the `-j` artifact. Game was ahead and live.
  **Infra, write nothing** (one wasted Rakdos X=3 at t8, not the timeout cause).
- **vs110 (t30, 54 records, adj L behind 7-13):** **REVISE wave-5's attribution.** Wave 5 called this
  "winning-but-slow, reach-starved." It is substantially a **piloting loss**: both Rakdos's Returns
  burned early for X=3 (Finding 1) AND 5 sweepers dumped on the artifact board (Finding 2). Latency
  spikes (60.9s / 49.9s / 45.6s) contributed to the clock, but deck140 had put itself behind. Findings
  1+2 target exactly this game.
- **vs135 (t17, 19 records, adj draw 16-16):** latency-starved (one 63.7s spike, rest normal, only 19
  decisions) + the wasted X=1 burst (Finding 1c). Mostly infra; the intent-collapse is representation.

Control mirrors still generate 2-4× the decisions of aggro games, so at a fixed wall-clock cap they eat
the timeout column regardless of play — the stalemate-detector / cap-by-decisions ask (notes.md) is
still open.

---

## OVERSHOOT / DISPLACEMENT AUDIT (mandatory)
1. **THE #1 RULE — hold-for-kill (guide lines 7-16).** Did NOT overshoot into hoarding; it **UNDER-
   fired** — disobeyed 6/7 (Finding 1). DISPLACEMENT-adjacent: the hold rule is correct but an
   UPSTREAM rule (cast-nothing scope) forbids executing it. Fix the upstream rule, don't shout the hold
   rule louder.
2. **Sweeper table (lines 44-53).** Correct for real creature boards. The waste (Finding 2) is a
   MISSING corrective fact, not an overshoot of the table.
3. **Staff targeting (lines 39-42).** No overshoot; obeyed. Demote.
4. **Cast-nothing rule (lines 57-59).** Where the damage is: simultaneously too NARROW (forbids holding
   a below-lethal Rakdos's Return, 1a) and missing the artifact-board case (Finding 2). Rewrite its
   exception list.
5. **No attack-floor suicide** (deck has ~no creatures; Blightsteel correctly attacked vs131 to win).

## STRUCTURAL residual (unchanged, still real) — reach starvation
vs44 and vs109 losses share a precondition the guide cannot create: **no castable clock.** vs44 dealt
**0 damage all game** (opp at 20 for 20 turns) against a flyer-tempo deck with Counterspell (2 fired;
the t17 Rakdos's Return likely countered) — swept 5 times, still died at -2. vs109 swept ~8 times, opp
at 20 until t14, deck140 bled to 0. Faithful execution, no reach. The 14-sweeper density is right; the
deck needs **teeth, not text** (notes.md). Do not answer reach starvation with louder guide prose.

---

## Wave-5 findings status
- **W5 #1 X rule flipped to value-anchored** — **HELD** (7/7 max-X; Finding 3). Keep compressed.
- **W5 #2 hold Rakdos's Return for the kill** — **FAILED in practice** (6/7 early fires; Finding 1).
  The rule exists but the cast-nothing contradiction + burn-prior defeat it. This wave's #1 fix.
- **W5 Staff targeting** — **HELD** (Finding 5). Demote.
- **Sweeper waste into empty/artifact boards** — **NEW leak** exposed by the hint-veto removal
  (Finding 2). Corrective fact needed.
- **Reach starvation** — **PERSISTS as STRUCTURAL** (vs44, vs109). Deck-construction flag, not guide.

## Priority for the guide reviser (next task)
1. **Resolve the hold-vs-cast-nothing contradiction** (Finding 1a): expand the cast-nothing/hold
   exception so a **below-lethal Rakdos's Return is a legal HOLD**. Highest-leverage edit.
2. **Override the burn-prior by name** for Rakdos's Return (1b) + the anti-first-option hold line (1c).
3. **Add the sweeper corrective FACT** (Finding 2): sweepers hit CREATURES ONLY, not artifacts/
   enchantments; against a 0-creature board, hold them.
4. **Add one Blightsteel-vs-lifegain line** (Finding 4): infect poison ignores their life/lifegain.
5. **Demote** Staff targeting and the X rule to their compressed cores; set expectations that the
   residual is deck reach (notes.md).
