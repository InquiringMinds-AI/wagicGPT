# deck158 (Orcs of Mordor) — wave-33 seat findings (3rd corpus, 2nd guided)

Corpus `matchups-20260819-070739`, binary `bdb30d8e7`, 6 games, guide
`bin/Res/ai/baka/deck158_strategy.txt` unchanged since the wave-32 revision.
All seq/turn citations are the translog record's own `seq`/`turn`. Card facts verified
against `bin/Res/sets/primitives/borderline.txt` + `_macros.txt` (`/usr/bin/grep`) and,
where load-bearing, against Scryfall Oracle text (Foray of Orcs, Brutal Cathar //
Moonrage Brute, Tavern Ruffian).

## HEADLINE — THE PERFECTA IS ~HALF TAILWIND, AND THE KILL CLOCK NEVER MOVED

**6/6, six kills, zero adjudications, zero fallbacks.** Per HL1's winning-seat rule this
is a corpus-integrity question first. The honest decomposition is below; the short form
is that **two of the six wins are worth very little, two are worth a discount, and two
are clean** — while the *seat-level* improvements (fallbacks, cast floor, blocker
compliance) are real and measurable and are the part that should carry forward.

| opponent | result | end | turn | decisions | opponent's corpus-wide record | discount |
|---|---|---|---|---|---|---|
| 116 | WIN — kill | opp -2 | 8 | 13 | 3-3 | **none — clean** |
| 36 | WIN — kill | opp -14 | 10 | 24 | 3-3 | **none — clean** |
| 105 | WIN — kill | opp 0 | 8 | 18 | 2-4 (was 6/6 in w32) | poison now rendered; see §3 |
| 152 | WIN — kill | opp -3 | 13 | 28 | 3-3 | **opponent mulliganed to 5** |
| 146 | WIN — kill | opp -5 | 15 | 29 | 4-2 (pool #2) | **opponent mulliganed to 5** |
| 139 | WIN — kill | opp -19 | 13 | 26 | **0-6** | **near-zero information** |

Pool records this corpus (derived from `results.tsv`, 21 games):
**158 6-0 · 146 4-2 · 36 3-3 · 116 3-3 · 152 3-3 · 105 2-4 · 139 0-6.**
Adjudication rate rose 1/21 → **4/21**; my seat took 0 in all three waves.

### The kill clock did NOT collapse and did NOT slow — the wave-32 statistic was win-conditioned

Wave-32's headline "median-9 kill clock" was computed over the three games that were
won. Over **all six games** the medians are:

| wave | all-games median turn | wins-only median | kills | adjudications (mine) |
|---|---|---|---|---|
| 31 | — | 13 | 5 | 0 |
| 32 | **11** (9, 8, 10, 12, 13, 16) | 9 | 3 | 0 |
| 33 | **11.5** (8, 8, 10, 13, 13, 15) | 11.5 | **6** | 0 |

The all-games median moved 11 → 11.5, i.e. not at all. **The wave-32 "the clock
collapsed to 9" reading was a survivorship artifact**: fast games are the ones you win,
so conditioning the clock on wins reports the deck's best games as its typical game.
Routed to skill.md as a rung (S2). The pool-invariant statistic that *did* move is the
one HL7 already names: **kills 3 → 6** against a field whose adjudication rate rose.

### What is actually attributable to what

**A. Decision quality — REAL, and it is the loss-tracing residue the wave-32 revision
was written for.**
- **Fallbacks 2/120 (1.7%) → 0/138 (0%).** Both wave-32 fallbacks were mine; both are
  gone. The fatal one (`unparsed_reply`, name-form `BLOCKS:` at 6 life, §4 R1 of
  wave-32) is answered by the guide's new blocker-seam procedure: **8/8 blocker windows
  this corpus answered in `Bn:An` label form, every assignment legal, zero unparsed**,
  including the one infect window (105 s13). `all_assignments_illegal`: **0 occurrences**
  (no fallback of any class fired).
- **Cast floor 32/33 → 42/42.** Zero declines of an offered cast in 42 casting windows.
- **The cost-notation falsity fact added in the wave-32 revision ("{2}{R} is THREE
  mana") VALIDATED: zero cost-notation misreads in 138 decisions.** Every affordability
  sentence in the corpus computes `{1}{B}` as 2 mana correctly (36 s10, 116 s2, 152 s11,
  146 s22). The wave-32 belief that replaced the wave-31 belief is dead.
- **Grishnakh's new guide line fired on its first live exercise** (105 s9 → s16 → s18:
  cast, stole Cystbearer with Army power 5 ≥ its 2, attacked with it under `[haste]`).

**B. Render tailwind — LARGE, and concentrated on this deck's core annotation.**
- **i6 / N-158m: amass preview coverage 3/52 (5.8%) → 59/63 (93.7%), which is 59/59 =
  100% of non-trigger-gated offers** (§2). The annotation designed for this deck went
  from blind to complete in one wave. Every cast decision in the deck now carries its
  own magnitude.
- **i1/i2 poison**: 11/11 infect-exposed prompts carry the count; the blocker forecast
  priced against 10 and **changed my block** (§3).
- **N-158k closed** (the brief lists it KNOWN-OPEN — it is not; §2b).

**C. Pool weakness — must be discounted, and it is not small.**
deck139 went **0-6**; beating it is close to uninformative. deck146 and deck152 each
took **two mulligans to five cards** in my game and still took me to turn 15 and 13.
deck105 fell 6/6 → 2/6 the moment poison was rendered — my win over it is a clean
measurement, but it is a measurement of a seat that just lost its render advantage, not
of a stronger deck158.

**Verdict I would defend: the seat is now fallback-free with both wave-32 loss-tracing
residues closed; the 6/6 itself overstates it. Two clean wins (36, 116), two discounted
(152, 146), one re-measurement (105), one near-void (139).**

---

## 1. Corpus shape at this seat

138 decisions ex gamestart/gameend (120 in wave-32). Mix: 102 ask (92 in Main phase 1,
6 Untap, 3 Blockers, 1 Draw), 26 attackers, 8 blockers, 2 priority.
**Zero Main-phase-2 decisions again** (see T2, §4).

Reply length p50 1,049 / p95 **11,906** / max 14,570 (wave-32: p50 1,060 / p95 5,287 /
max 12,180 — **p95 more than doubled**). Latency p50 17.4s / p90 80.6s / p95 186.3s /
max **219.0s**. **Crash gate: 0 signatures in all 6 stderr files** (`game-158v116`,
`game-158v36`, `game-158v105`, `game-139v158`, `game-152v158`, `game-146v158`).

The length regression is not distributed — it is two mechanisms, both in notes.md:
**N-158r** (the pregame prompt latching onto the on-the-play first land drop: 116 s2 =
14,570 chars / 214.5s on a three-option land drop) and the deck146 mid-game self-audit
spirals (146 s19/s22/s27 at 192-208s each).

## 2. The step-1 batch at this seat

### 2a. (i6) N-158m amass preview — **VALIDATED. 59/63 offers = 100% of non-trigger-gated.**

Measured over every `N. Cast <amass card>` option line in 6 games:

| card | offers | with `{right now: Army X/X -> Y/Y}` |
|---|---|---|
| Orcish Bowmasters | 25 | **25** |
| March from the Black Gate | 14 | **14** |
| Mordor Muster | 13 | **13** |
| Easterling Vanguard | 4 | 0 — **correctly suppressed** |
| Swarming of Moria | 2 | **2** |
| Grishnakh, Brash Instigator | 2 | **2** |
| Foray of Orcs | 1 | **1** |
| Dunland Crebain | 1 | **1** |
| Gothmog, Morgul Lieutenant | 1 | **1** |
| **total** | **63** | **59 (93.7%)** |

**The four misses are all Easterling Vanguard, whose amass is `_DIES_`-gated** — the
raw-line trigger filter drops it before the macro expansion runs, which is correct: a
cast-time preview of a death trigger would be a confident lie. Against the brief's
prediction ("amass previews on ALL non-trigger-gated offers") this is **59/59, exact.**

Both branch forms render, and both are correct against the narration:
- `Cast Mordor Muster {1}{b} {right now: Army 0/0 -> 1/1 (a new 0/0 Army token is
  created first)}` (36 s4) — the no-Army branch, token clause spelled out.
- `Cast Swarming of Moria {2}{r} {right now: Army 6/6 -> 8/8}` (36 s16) — amass 2.
- `Cast Grishnakh, Brash Instigator {2}{r} (1/1) {right now: Army 3/3 -> 5/5}` (105 s9).
- `Cast Dunland Crebain {2}{b} (1/1) {right now: Army 7/7 -> 9/9}` (146 s17).
- `Cast Gothmog, Morgul Lieutenant {3}{b} (3/3) {right now: Army 6/6 -> 7/7}` (116 s12).

Assault on Osgiliath — the negative control the fix had to preserve — **was never
offered in any of 6 games for the third consecutive corpus**, so its multi-branch
suppression remains a code read, not a live witness. **N-158f and N-158i stay
UNEXERCISED.**

### 2b. N-158k — **CLOSED by the step-1 batch. The brief's KNOWN-OPEN entry is STALE.**

The brief lists N-158k as "costs present on the cast line, still ABSENT on the target
menu". It shipped at `948578887` (`AIPlayerGPT.cpp:6363-6376`, with a `[W33-N158k]`
PARSETEST case at `:10825`). Live: **3/3 Feed the Swarm target menus, 11/11 target
lines carry `(costs you N life)`**, every number correct:

- 139 s11 t7 — `Arboreal Grazer #1 (0/3) [reach] ... (costs you 1 life)` ×4 (`{G}`, MV 1 ✓)
- 146 s24 t13 — `Spider #1 (2/1) [reach, menace] ... (costs you 0 life)` ×2 (token, MV 0 ✓)
- 146 s28 t15 — `Triumphant Adventurer (1/1) [deathtouch] ... (costs you 2 life)`
  (`{1}{B}`, MV 2 ✓) + both Spiders at 0

**It is load-bearing at the commit seat.** 146 s28: the pilot paid **2 life** for
Triumphant Adventurer over two free Spiders because the Adventurer (1/1 deathtouch,
first strike on its own turn) was the card blanking the Army — T5 exactly, executed at
the seat where the price is now visible. Wave-32's complaint ("deck152 answered it at
11 life and again at 6 with the number invisible") is answered.

**Consequence for the guide:** the deployed guide says *"CHOOSE YOUR TARGET FROM THAT
LINE, before you commit, because the target menu that comes next does not repeat the
prices."* **That sentence is now FALSE.** HL10 reconciliation — it goes in the revision.

### 2c. N-158l (empty `[from ]`) — **CLOSED. 0 occurrences** (wave-32: 38 distinct
prompts). 21 distinct counter-narration lines this corpus, all with a name or no
bracket. Attribution: interim `0280a08b4` (counter events from every engine path), not
the step-1 batch.

### 2d. (i7) `(printed X/Y)` — **VALIDATED at this seat: zero false tags.**
46 tagged renders, every one a true delta: `Snarling Warg {3}{b} (4/4) (printed 3/4)`
(28), `Orc army (N/N) (printed 0/0)` (94 across sizes), `Briarbridge Tracker (4/3)
(printed 2/3)` (9), `(printed 1/1)`, `(printed 3/3)`. **Moonrage Brute carries NO
printed tag** (wave-32: 22 false `(printed 2/2)`). Confirmed against Scryfall: Brutal
Cathar // Moonrage Brute, back face 3/3 — so `(3/3)` untagged is correct.

### 2e. (i5) `{0}` guards — **PASS on real costs, RESIDUAL on costless objects.**
Real `{0}` costs render correctly (Mox Jet, Mox Sapphire, Ornithopter). But
**`Moonrage Brute {0} (3/3)`** (4 renders) confabulates a cost on a transform back face
whose Oracle `mana_cost` is the empty string, and **`Day {0}` / `Night {0}`** (16
renders) render the game-wide day/night *designation* as a costless permanent — 12 of
them on **my own battlefield line**. Routed **N-158v**, LOW (no misplay traced).

### 2f. (i8) name-form `BLOCKS:` — **UNEXERCISED.** All 8 blocker replies used
`Bn:An` labels; the name-form path never triggered. `all_assignments_illegal`: 0.

### 2g. (i9) hand-only pregame — **VALIDATED for its own scope, but the scope LEAKS.**
All 6 mulligan asks are hand-only with the engine-computed header:
```
Your hand (7 cards), counted by the engine: 3 lands, 4 spells.
Mana sources among those lands, counted by the engine: {B} 2, {R} 1. ...
Cheapest spell in this hand: Orcish Bowmasters {1}{b} (mana value 2).
Playing every land in this hand would cover the cost of: Orcish Bowmasters {1}{b}, ...
```
Zero battlefield facts, **zero `Mana available: 0 total` echoes in mulligan replies**,
6/6 first-hand keeps. But the same serializer is still latched at the **on-the-play
first land drop** — see N-158r (notes.md), the seat's worst latency event.

## 3. The poison pair at my seat (i1 / i2)

**i1 — 11/11 infect-exposed prompts (100%) carry the count.** The first 7 decisions of
the vs105 game precede any poison event; from s8 (t6) to s18 (t8) every prompt carries
the status line and the narration:
```
Poison counters (you): 1 of 10 - you LOSE the game at 10 poison counters, whatever the
life total is; 9 more end it. Poison is not life: it does not reset between turns, and
gaining life or preventing damage does not remove a poison counter.
- Poison: you take 2 poison counters - now 3 of 10 (you LOSE at 10 poison counters; 7 more end it)
- Infect damage: 2 from Cystbearer to you - dealt as POISON COUNTERS, not life loss: no
  life was lost (see the Poison line)
```
The wave-32 four-silent-events class (an infect damage line with no consequence) is
dead at this seat.

**i2 — the forecast partitions by damage type and prices against 10. Zero "NOT lethal"
emissions on the infect window.** Verbatim, 105 s13 t7:
```
Your life: 18. Your poison counters: 1 of 10.
Unblocked, these attackers put up to 3 POISON COUNTERS on you - you would be at 4 of 10
poison - that is not yet 10, but poison counters never reset and nothing here removes
them, so every counter you take is permanent progress toward losing. Life total is not
the resource under attack; price the block against the poison count.
```
**Negative control holds:** all 7 non-infect blocker windows are unchanged
(`Your life: 20. Unblocked, these attackers deal up to 1 - you would be at 19 - NOT
lethal: block only where the trade favors you`, 36 s5 / 36 s14 / 36 s18 / 139 s23 /
152 s13 / 152 s19 / 152 s23).

**It changed my blocking.** At 105 s13 the pilot blocked with its only body
(`BLOCKS: B1:A1`) and reasoned entirely in poison — *"Total poison taken this turn would
be 3 ... Blocking the Myr reduces the incoming poison to 2, which is better than taking
3."* At the equivalent wave-32 seam the pilot could not see the resource at all. It won
on turn 8 at 3 poison of 10 and 18 life.

**One defect inside that same window** — the B-line's naive trade said
`may block A1 (both die)`; Ichorclaw Myr's rendered text on the same screen says
*"Whenever Ichorclaw Myr becomes blocked, it gets +2/+2 until end of turn"*, so the
truth is *your blocker dies, attacker lives* (narration: Bowmasters dealt 1, took 3
infect as -1/-1 counters, went to graveyard; the Myr survived). Routed **N-158t**. No
harm — the block was correct on poison grounds either way.

## 4. Teach-by-teach firing record

| # | teach | fired? | evidence |
|---|---|---|---|
| T1 | attack floor — declare every creature offered | **YES, 26/26 windows, 53/53 creatures** | zero `ATTACK: none`, zero subsets. Cumulative guided+unguided **77/77**. Includes both borrowed bodies (Briarbridge Tracker 152 s17, Cystbearer 105 s18). |
| T2 | amass in MAIN PHASE 1 then attack | **outcome YES 42/42, mechanism UNEXERCISED (2nd wave)** | all 42 casts in Main phase 1; **zero Main-phase-2 decisions of any kind** in 6 games. Per HL4 rule 2 the choice the teach exists to settle was never offered. |
| T3 | cast floor — take the cast | **YES, 42/42** | 42 windows with a real cast offered, 42 taken, 0 declines (w32: 32/33). |
| T4 | Easterling Vanguard is a trade/chump | **YES** | cast on sight 4/4; attacked 152 s22/s28; the death-amass paid off. |
| T5 | Feed the Swarm — spend it on what is killing you | **YES, 4/4** | 146 s28 paid 2 life for Triumphant Adventurer over two 0-life Spiders; 146 s24 took the cheaper of two identical Spiders; 139 s11, 36 s21 fine. |
| T6 | Sauron/Grishnakh — steal the biggest, swing with it | **YES, 2/2** | 152 s16 took Briarbridge Tracker (4/3) over Brutal Cathar and Katilda, attacked with it at s17 under `[haste, vigilance]`; 105 s16→s18 same shape with Grishnakh. **No repeat of the wave-32 "she is tapped, I cannot attack" misread** — the render now prints `[haste]` on the stolen body. |
| T7 | Bowmasters — cast in main phase, never hold for flash | **YES, non-harmful** | 2 priority windows in 6 games, both Upkeep, both offering only Sauron's pump; Bowmasters never offerable at instant speed. **N-158e steady state, not counted against the guide.** |
| T8 | Bowmasters ping — shoot toughness 1, else face | **NO — 3/5, MISSED TWICE** | ✓ 36 s11 (Salvage Slasher 1/1), ✓ 146 s7 and ✓ 146 s19 (Triumphant Adventurer 1/1). ✗ **152 s18 t10**: `Katilda, Dawnhart Prime (1/1)` on the menu, took the face at 15 life, rationalised *"we have no mana to cast removal spells right now"* — irrelevant to a free ping. ✗ **105 s11 t6**: menu had `Ichorclaw Myr (1/1) [infect]` and `Cystbearer (2/3) [infect]`; **shot the 2/3 for 1**, which kills nothing and wastes the trigger entirely. §5 R1. |
| T9 | mulligan — count black sources, ≤2 mulligans | **UNEXERCISED for its discriminating half** | 6/6 first-hand keeps; every opener had 3 lands including ≥2 Swamps. The pilot cited the rule by name at 146 s1 (*"satisfies the mulligan criteria of having at least..."*), so it is being read — but no Swamp-less or 1-land hand ever appeared. |
| T10 | cost notation — `{2}{R}` is THREE mana | **YES, VALIDATED** | zero cost-notation misreads in 138 decisions; §"A" above. |
| T11 | blocker-seam procedure (`Bn:An`, read the `- may block An` list) | **YES, 8/8** | every blocker reply label-form, every assignment on an offered pair, zero fallbacks. **The wave-32 fatal class is dead.** |

**Did any teach MISLEAD?** Two guide sentences are now factually wrong and one is
dangerously incomplete — all three go in the revision:
1. *"the target menu that comes next does not repeat the prices"* — **FALSE since
   N-158k closed** (§2b).
2. *"one card removes their creature and grows your attacker in the same breath"* +
   *"With no creature to shoot, cast it anyway"* — true but **incomplete in the exact
   direction the engine now pushes**: see R2.
3. T8's toughness-1 test survives as a *name-matching* rule and lost twice to a
   "bigger threat" heuristic. It needs the lethality fact, not just the test.

## 5. THE RESIDUE — what the revision is for

**R1 (#1 slot). T8 leaks in a new shape: the pilot picks the biggest threat, not the
one it can kill.** 105 s11 is the clean specimen and it is worse than a missed
exception — the reply's **first** answer was `CHOICE: 5 (Ichorclaw Myr (1/1))`, which is
correct, and ~1,800 characters of post-answer scratch talked it into
`CHOICE: 6 (Cystbearer (2/3))`, which the engine took. One damage to a 2/3 does nothing
at all; the reply never once asks whether the ping kills its target. 152 s18 is the same
substitution without the retraction. The fix is a lethality fact, exception-first: 1
damage kills a creature whose printed toughness is 1 and nothing else.

**R2. Foray of Orcs' `damage N` is rendered even with ZERO legal targets, and the pilot
read it as face damage — citing the annotation as its authority.** 116 s12 t8, opponent
battlefield `(creatures: 0)`, option line `Cast Foray of Orcs {3}{r} {right now: Army
6/6 -> 8/8, damage 8}`. Reply, verbatim: *"Since there are no opponent creatures to
target, the spell deals 8 damage directly to the opponent (as per the 'right now' text:
Army 6/6 -> 8/8, damage 8). This reduces the opponent's life to 1."* Scryfall Oracle:
*"...deals X damage to **target creature an opponent controls**"* — the option line's own
`{card text: ...}` says so too. The opponent stayed at 9; the pilot won anyway on a
9-power swing. **No harm this game, but this is a lethal-arithmetic falsity planted by
the render.** Engine fix routed **N-158s**; the guide gets the matching sentence,
because a guide line is the cheap half and the annotation is the authority the pilot
already trusts.

**R3. The mulligan rule now duplicates work the engine does.** The pregame header counts
lands, counts `{B}`/`{R}` sources, names the cheapest spell and lists which spells the
hand's lands would cover. The guide still tells the pilot to count Swamps itself. HL10:
retarget the rule at the header (which is authoritative and free) and keep only the
judgement the header cannot make — the ≤2-mulligan floor and the Swamp-less exception.

**R4. Reconciliation sweep**: delete the Feed-the-Swarm "the menu does not repeat the
prices" clause; the prices are on both surfaces now and the guide should say so.

Everything else in the guide is **FROZEN**: engine identity, attack floor, cast floor,
blocker-seam procedure, cost-notation fact. All five validated at 100% this corpus and
the revision must not re-legislate them.

## 6. Commit-failure counter (deck158 = the counter's natural seat)

Fields as counted, 138 decisions:

| measure | count |
|---|---|
| replies carrying a `PLAN:` line | 131 |
| replies with **no** `PLAN:` line (protocol-compliant per `f46dd58ee`) | **7** |
| replies continuing past their own first `PLAN:` line (`post_plan_overrun` > 0) | **11 / 131 (8.4%)** |
| of those, `commit_retracted` | **0** |
| of those, fatal (fallback / lost decision) | **0** |
| mean `post_plan_overrun` over replies with a PLAN line | 335 chars |
| largest overruns | 146 s19 **11,519** · 146 s22 **11,075** · 116 s2 **6,591** · 146 s27 5,560 · 146 s26 4,801 |

**PROMOTION TRIGGER NOT MET AT THIS SEAT: zero retractions, zero fatal overruns.**
Wave-32's `retracted_choice` (139 s16, 12,180 chars / 206s) did not recur.

**The harm the counter does not measure: PLAN-line pollution.** 146 s19's `PLAN:` line
*is* where the spiral starts, so the corrupted text became the persisted plan and was
replayed into the next prompt verbatim:
```
YOUR PLAN (as you last stated it): Resolve Orcish Bowmasters targeting Triumphant
Adventurer to destroy it and grow the Army to 10/10. Then attack with the Orc Army
(10/10) and Snarling Warg (4/4) for 14 damage, winning the game (16 - 14 = 2, wait,
16-14=2? No, 16-14=2. Wait, 16-14=2. I need 16 damage to win. Let me re-calculate.
```
(146 s20 prompt.) 146 s23's plan is a *quotation of its own previous plan*
(`YOUR PLAN: "Cast Feed the Swarm... to clear a potential blocker". This is correct.`).
The plan is the only durable state the pilot carries between decisions, and the
scratch register is writing into it. Reported for the owner directive, not
counter-proposed against.

## 7. Carried open items at this seat

- **N-158e** (flash windows) — KNOWN-OPEN, steady state, 3rd corpus: 2 priority windows
  in 6 games, both Upkeep, Bowmasters never offered at instant speed. Not counted
  against the guide; the de-flash teach stays.
- **N-158d** (Foray's target menu names the Army) — **UNEXERCISED**: Foray was cast once
  with no legal target, so no target menu was raised.
- **N-158f / N-158i** (Assault on Osgiliath) — **UNEXERCISED, 3rd consecutive corpus.**
  Assault has never been offered under three binaries. A probe deck is now the only
  instrument that will close these; recorded in skill.md.
- **N-158n** (name-form BLOCKS parser) — the fix shipped; **UNEXERCISED** (no name-form
  reply occurred).
- **L2 cross-zone dup-name** — witnessed: 36 s12, the pilot flagged
  *"'Your battlefield ... Orcish Bowmasters'. But the 'Your hand' list ALSO lists
  'Orcish Bowmasters'. This is a contradiction in the provided text."* deck158 runs 2×
  Bowmasters, so both statements were true. Cite, don't re-diagnose.
- **N-158j** (poison unrendered) — **CLOSED** by i1/i2 (§3).
- **N-158k, N-158l, N-158m** — **CLOSED** (§2).

## 8. New items opened this corpus

All detailed with seq repros in notes.md.

| id | severity | one line |
|---|---|---|
| **N-158r** | **HIGH (cost)** | pregame hand-only serialization latches through the on-the-play player's first land drop — 3/3 on-the-play games, one 214.5s / 14,570-char spiral on a 3-option choice. |
| **N-158s** | **HIGH (correctness)** | Foray of Orcs renders `damage N` with zero legal targets; the pilot cited it as authority for face damage (116 s12). |
| **N-158t** | MEDIUM | blocker forecast's naive 1-on-1 trade ignores "whenever this becomes blocked, it gets +X/+X" — `(both die)` was false at 105 s13. |
| **N-158u** | LOW | batched `-1/-1` counter narration prints the FINAL total on every line instead of a running one (105 s14: three lines all reading `(now -2/-2)`). |
| **N-158v** | LOW | `Day {0}` / `Night {0}` rendered as permanents on **my** battlefield line; `Moonrage Brute {0}` confabulates a cost on a costless back face (i5 residual). |
| **L-158-2** | watch | consumed-ask instruction boilerplate replayed in the game log is **15-19% of the log** at this seat — owner-directive-3 evidence, larger than the docket's estimate. |
