# deck105 "Infected Wounds" (BG Infect) — wave-33 seat review — THE RE-RUN SEAT

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260819-070739`, binary `bdb30d8e7`, -T 3000,
REPPENALTY=1.05, -j 3. Seat logs: 6 games. **123 model decisions at deck105's seat**
(240 across both seats of those 6 games), **0 fallbacks, 0 crash/FATAL/assert signatures in all
6 stderr files.**

**Record: 2/6 — down from the wave-32 ASTERISKED 6/6.**

---

## 0. HEADLINE — THE RE-RUN VERDICT, ARRIVAL-TRACED

**The wave-32 perfecta was SUBSTANTIALLY A RENDER ARTIFACT. Confirmed, with the mechanism
measured at both seats.**

The confound wave-32 named was N-105b: at every infect blocker window the defender was told the
swing was "NOT lethal" and that "taking damage while ahead is often correct". N-105b shipped.
The behavioural consequence is not subtle:

| | wave-32 (poison-blind opponents) | wave-33 (honest forecast) |
|---|---|---|
| infect blocker windows at opponent seats | 12 | 10 |
| windows in which the opponent DECLARED A BLOCK | 8 / 12 (4 ended `no blockers`) | **10 / 10** |
| deck105 record | 6/6, all poison alt-wins | **2/6**, both poison alt-wins |
| deck105's own life at gameend | 20, 6, 3, 20, 2, 8 (never below 2) | **0, -5, 0, 0**, 20, 17 |
| poison delivered in the games it LOST | n/a (no losses) | **1, 1, 4, 1** |

Arrival trace of the reversal, worked in full at the cleanest specimen (105 v 158, the fastest
loss, turn 8):

1. `1787152102 deck158 seq13 t7` — deck158's blocker window against Ichorclaw Myr + Cystbearer.
   The prompt now reads, verbatim:
   `Unblocked, these attackers put up to 3 POISON COUNTERS on you - you would be at 4 of 10
   poison - that is not yet 10, but poison counters never reset and nothing here removes them, so
   every counter you take is permanent progress toward losing.`
   deck158 blocked (`Orcish Bowmasters blocks Ichorclaw Myr`). In wave-32 this window's line said
   the attack was harmless and the same shape ended in `no blockers`.
2. deck158 then removed the other half of the clock at the SOURCE — `seq11 t6` targeted Cystbearer
   with Orcish Bowmasters, and at `seq16 t8` **stole Cystbearer outright**.
3. deck105's poison total at gameend: **1 of 10**. deck158's Orc army grew, rendered in the log at
   every step (`Counter added to Orc army: +1/+1 (now 3/3)` -> `(now 6/6)` -> `(now 9/9)`), and
   killed deck105 on turn 8 at **0 life**.

The same shape holds in the other three losses: deck105 delivered 1, 1 and 4 poison and died at
0 / -5 / 0 life on turns 10, 11 and 12. **It never once got within 5 counters of the win in a
game it lost.** The wave-32 clock (10-11 poison by turn 12-17, every game) does not exist against
opponents who block.

**Both wave-33 wins are still poison alt-wins**, and both came against seats whose boards could
not block enough:
- `105 v 116` (won t10, deck116 at **23 life**): poison rendered at `8 of 10` before the t10
  swing; deck116 got exactly ONE blocker window in the game (`seq8`) and blocked correctly there.
- `139 v 105` (won t11, deck139 at **14 life**): the win is carried by **Hand of the Praetors**,
  the card wave-32 recorded as never drawn. Cast `seq12 t7`; its cast-trigger poison ability
  targeted the opponent at `seq19`, `seq21`, `seq26` (3/3 correct target picks); its lord made the
  final swing 3+3+2+2 = 10 poison off a base of 3. deck139's blocker window (`seq24 t11`) rendered
  `you would be at 13 of 10 poison - LETHAL if it all connects` and it blocked — with the ONLY
  blocker it had (Illuna). Its loss was correctly informed and unavoidable, not a render victim.

**Verdict for the roster lane:** the 6/6 was worth roughly four games of opponent misinformation.
2/6 against the same pool is the first clean measurement. deck105 is not a pool-breaking
construction; it is a fragile linear deck that the pool answers by blocking and by removing or
stealing its two-power bodies. No difficulty-ladder pricing should use the wave-32 number.

---

## 1. THE STEP-1 POISON PAIR — coverage FRACTIONS at both seats (i1, i2)

### (i1) N-105a — poison is RENDERED. VALIDATED, 100%, both seats.

| surface | wave-32 | wave-33 |
|---|---|---|
| status line | absent | `Poison counters (opponent): 4 of 10 - the opponent LOSES the game at 10 poison counters, whatever the life total is; 6 more end it.` |
| combat poison narration | `- Damage: 2 dealt by Contagious Nim to the opponent` (identical to real damage) | `- Infect damage: 2 from Contagious Nim to the opponent - dealt as POISON COUNTERS, not life loss: no life was lost (see the Poison line)` + `- Poison: the opponent takes 2 poison counters - now 6 of 10 (...)` |
| creature half of infect | `- Counter added to X: -1/-1` | `- Infect damage: 3 from Cystbearer to Arboreal Grazer - dealt as 3 -1/-1 counters, a permanent shrink that does NOT wear off at end of turn` |
| non-combat poison (`alterpoison`, Ichor Rats) | **silent** | `- Poison: you take 1 poison counter - now 1 of 10 (...)` + the opponent's line |

**COVERAGE FRACTION (per HL5, not a sample):** prompts issued after the first `- Poison:` event
in the log that carry the `Poison counters (` status line —
**deck105 seat 63/63; opponent seats 57/57; TOTAL 120/120 = 100%.** Zero misses at any decision
kind (`ask` / `attackers` / `blockers` / `priority`). The line is correctly suppressed at 0/0, so
non-infect duels pay nothing. **The wave-32 four-silent-events class is DEAD** (4/4 Ichor Rats
ETBs in wave-32 were silent; every `alterpoison` and every infect combat hit narrates here).

N-105a's own three validation predicates, run as written:
- (i) **ZERO** hand-count re-derivations. Grep of all 123 deck105 replies for
  `my count is wrong|Let's recount|based on the cumulative|counters (based on`: **0 hits.**
  The wave-32 8,827-char *"if the game is still going, then my count is wrong"* spiral class does
  not recur once.
- (ii) **No LAND DROP in the seat's top-5 tax rows.** Top 5 by reply length are now
  blockers (11,912 / 11,414 / 8,424), a cast decision (11,089) and a priority window (8,286); the
  first land drop is rank 6. Wave-32 had 4 of the top 8 as land drops; wave-33 has 2 of 8.
- (iii) **At least one reply quotes the rendered number back.** 5 replies do
  (`105v36 s24`, `105v116 s14`, `105v152 s15/s19`, `105v158 s11`).

### Arithmetic ON THE RENDERED COUNT — the wave-32 diagnostic is CURED at the base, one residual

Every one of the 5 poison-arithmetic replies reads the BASE count correctly off the render. The
arithmetic itself is 4/5 correct:
- `105v116 s14` — prompt `Poison counters (opponent): 4 of 10`; attacked with Ichor Rats(2) +
  Plague Stinger(1) + Ichorclaw Myr(1); reply *"...to 8 of 10"*. **CORRECT.**
- `105v152 s19` — prompt `4 of 10`; Ichor Rats 2 power; reply *"bringing the opponent to 6/10"*,
  PLAN *"...and Tel-Jilad Fallen (totaling 5 poison) to reach 11/10 and win"*. **CORRECT twice.**
- `105v158 s11` — prompt `1 of 10`; Myr(1)+Cystbearer(2); reply *"to 4/10"*. **CORRECT.**
- `105v152 s15` — prompt `2 of 10`; Ichor Rats is a **2/1**; reply *"Attacking deals 1 poison
  counter, bringing the opponent to 3/10"*. **WRONG by 1** — but the error is a per-creature
  POWER misread, not a cumulative-count re-derivation, and it changed nothing (it attacked with
  the whole board anyway).

**So: the wave-32 diagnostic ("correct strategy, wrong count every time it mattered") is
discharged.** The remaining error class is reading a creature's power off its own option line,
which is a different and much cheaper failure.

### (i2) N-105b — the blocker forecast partitions by damage type. VALIDATED, 100%, both directions.

- **Infect windows (opponent seats): 10 / 10 carry the poison forecast. 0 / 10 carry a life
  forecast.** The predicted emission `you would be at <life-N> — NOT lethal` on an infect window:
  **ZERO occurrences.** The lethal case fires correctly: `139 v 105 s24` renders
  `you would be at 13 of 10 poison - LETHAL if it all connects: at 10 poison counters you lose the
  game no matter what your life total is. Your life total does not answer this; block the...`
- **The advice clause is gone from the infect branch.** The wave-32 misteach *"taking damage while
  ahead is often correct"* has **0 occurrences** on any infect window; it is replaced by
  *"poison counters never reset and nothing here removes them, so every counter you take is
  permanent progress toward losing"*. This is the restriction-first wording the wave-32 note asked
  for, and it is doing the work.
- **NEGATIVE CONTROL CLEAN.** At deck105's own seat (which never faces infect) all **17/17**
  blocker windows still render the unchanged life form
  (`Your life: 17. Unblocked, these attackers deal up to 4 - you would be at 13 - NOT lethal:
  block only where the trade favors you; taking damage while ahead is often correct.`), 14 of them
  with the `- NOT lethal` tail and 4 with `LETHAL if it all connects`. Same-seam non-infect
  behaviour is untouched.
- **Per-attacker annotation also shipped and reaches 100% of infect attacker lines:**
  `A1. Cystbearer (3/4) deals 3 [infect] [its combat damage to YOU is dealt as POISON COUNTERS,
  not life loss - your life total will not move and blocking is the only thing that stops it]
  [infect also damages CREATURES as -1/-1 counters, which do not wear off at end of turn - a
  blocker that survives stays shrunk]`.

**Behavioural payoff, counted:** infect blocker windows ending in a declared block went
**8/12 -> 10/10**. This is the single measured cause of the record collapse and it is the fix
working, not a regression.

---

## 2. WHAT THE GUIDE DID — the decision audit (this is the seat's own failure lane)

The pilot's mechanical floors held perfectly; every loss traces to the guide's ONE gate rule.

### PRESERVE LEDGER — all four wave-32 load-bearing behaviours held

| seam | wave-33 metric | verdict |
|---|---|---|
| **Attackers** | **19 / 19 windows declared EVERY offered creature.** Zero `ATTACK: none`, zero partials. | PRESERVED |
| **Casting** | **0 `Cast nothing right now` in 29 cast windows.** | PRESERVED |
| **Mulligan** | **0 mulligans in 6 games**, 6/6 keeps. The wave-32 NM3 class (mulliganing for a 1-drop the deck does not contain) is **DEAD** — the guide's flat statement is quoted back correctly at `105v158 s1`. | FIXED, but see M1 |
| **Ichor Rats symmetric pricing** | 3/3 casts, all priced correctly WITH the now-visible self-counter: *"applies poison pressure (1 to opponent, 1 to me) ... Even though I take a poison counter, getting the opponent to 1 counter now accelerates my win condition"* (`105v116 s6`). The render did NOT flip it into a false one-sided decline. | STEADY STATE |
| **Corpse Cur / Necropede / Hand targeting** | 100% correct: Corpse Cur returned Blight Mamba (`139v105`), Necropede's death trigger pointed at an opponent creature (`105v36 s19/s20` -> Master of Etherium), Hand of the Praetors' cast-trigger targeted **the opponent 3/3 times**. | PRESERVED |

### L1 — THE #1 RULE IS AMBIGUOUS, THE PILOT BOUND IT WRONG, AND IT COST THE vs146 GAME

The guide's block gate reads *"That 'you would be at N' number ... N is 9 or more -> answer 'BLOCKS:
none'. N is 8 or less, or the line says LETHAL -> block now"*, and then the SITUATIONS section
gives *"At 20 life, they swing for 5 with one big creature -> 'BLOCKS: none'"*. The pilot bound
`N` to the **damage**, not to the resulting life, and then found its own guide self-contradictory.
Verbatim, `105v146 s11 t7` (a LAND DROP, 7,788 chars / 109 s):

> *"5 is not 9 or more. So according to the rule, we should block. But the example says we don't.
> This is a contradiction. Let's assume the example is correct and the rule is misstated."*

and again at `s12`: *"There is a contradiction in the guide text provided."*

**The fatal application, `105v146 s15 t8` (kind=blockers, at 10 life vs a 9-power swing):**
prompt line `Your life: 10. Unblocked, these attackers deal up to 9 - you would be at 1 - NOT
lethal`. Reply, verbatim:

> *"Wait, re-reading the guide's specific math: 'N is 9 or more -> answer BLOCKS: none'. The prompt
> says 'Unblocked... you would be at 1 - NOT lethal'. Therefore, I block with **none**."*

-> `BLOCKS: none` -> 10 life -> **1 life** -> dead on turn 10 at 0. Two 1/1 blockers were offered
(`B1. Necropede`, `B2. Blight Mamba`) and the guide's own resulting-life reading (1 <= 8) says
BLOCK. **This is a guide-caused loss with a verbatim self-citation.** It is the wave-32 HL2
self-contradiction class (deck146's signature) firing inside deck105's own guide.

### L2 — THE GATE IS BLIND TO A GROWING CLOCK (the vs158 loss, and the shape of the other two)

`105v158 s9 t6`, at 17 life, single attacker `A1. Orc army (6/6)`, one blocker offered
(`B1. Cystbearer`). Reply, verbatim:

> *"According to your strategy guide, you only block if the incoming damage would reduce your life
> to 8 or fewer. Since 11 is safely above that threshold, blocking is unnecessary... Taking the
> damage now to keep your clock ticking is the correct play."*

The army was rendered in the log as growing every single turn — `(now 1/1)` -> `(now 3/3)` ->
`(now 6/6)` -> `(now 9/9)`, plus `[counters: 9x +1/+1]` on the board line. At 17 life a chump block
costs one body and zero life; the guide forbade it. Two turns later the 9/9 plus a **stolen
Cystbearer** killed deck105 from 11. Same shape in `105v152` (Wolf 2/2->3/3->4/4->5/5 under Ranger
Class) and `105v36` (Master of Etherium growing with every artifact).

**A static threshold on the CURRENT swing cannot price an accelerating board, and every loss in
this corpus was against one.** The gate must key on a number the render already prints
(`[counters: Nx +1/+1]`, `(printed X/Y)` deltas, the attacker's power vs last turn).

### M1 — THE MULLIGAN KEEP RULE HAS NO COLOUR CLAUSE, AND THE ENGINE ALREADY SHIPS THE ANSWER

`105v158 s1` opening hand: `Forest; Plague Stinger {1}{b}; Forest; Forest; Plague Stinger {1}{b};
Contagious Nim {2}{b}; Contagious Nim {2}{b}` — three Forests and four BLACK spells. i9's header
said so in one line, verbatim: **`Playing every land in this hand would not cover any spell in it.`**
The pilot kept it anyway and wrote *"The presence of 3 lands ensures we can cast a 2-mana creature
on turn 1 or 2"* — a direct contradiction of a rendered engine-computed line — quoting the guide's
own *"'no one-drop' is never a reason to mulligan"* as licence. It was still stuck on colour at
turn 7 (*"I have no Black mana to cast my hand's creatures this turn"*) and lost on turn 8 having
cast only what it drew. **The guide's KEEP rule counts lands and creatures and never mentions
colour; the render now provides exactly the sentence to key on.** (This is the i9 header working
and a GUIDE rung failing, not an i9 miss.)

### L3 — one false card fact, with a live witness (HL1 ledger below)

The guide asserts of Ichorclaw Myr: *"it kills the blocker instead of dying."* `105v116` refutes
it in one combat: Ichorclaw Myr attacked, was blocked by Kitchen Finks (3/2), pumped to 3/3, put
3 -1/-1 counters on Finks **and took 3 damage and died** —
`- Your Ichorclaw Myr: battlefield -> graveyard` / `- Opponent's Kitchen Finks: battlefield ->
graveyard`. Both died. FALSE as written.

---

## 3. HL1 RETROACTIVE CARD-FACT AUDIT — the per-claim LEDGER (hard gate)

Every checkable claim in the DEPLOYED guide, each adjudicated against BOTH the shipping primitive
(`Res/sets/primitives/mtg.txt`, read this session) and real Oracle text (scryfall API, fetched
this session — reviewer Oracle memory not trusted).

| # | claim as written in the guide | adjudicating source | verdict |
|---|---|---|---|
| 1 | "10 poison counters and they LOSE, at any life total" | CR 104.3c / render `LOSES the game at 10 poison counters` | TRUE |
| 2 | Plague Stinger {1}{B} 1/1 flying, "unblockable unless they have a flyer or reach" | Oracle: Flying, Infect; 1/1 {1}{B} | TRUE |
| 3 | Blight Mamba {1}{G} 1/1, "{1}{G}: Regenerate" | Oracle: `{1}{G}: Regenerate this creature.` | TRUE |
| 4 | Necropede {2} 1/1 artifact, "when it dies you MAY put a -1/-1 counter on target creature" | Oracle: `When this creature dies, you may put a -1/-1 counter on target creature.` | TRUE |
| 5 | Ichorclaw Myr {2} 1/1 artifact, "+2/+2 whenever it becomes BLOCKED" | Oracle: `Whenever this creature becomes blocked, it gets +2/+2 until end of turn.` | TRUE |
| 5b | ..."so it attacks as a 1/1 and becomes a 3/3 the moment they block it — **it kills the blocker instead of dying**" | Oracle P/T 1/1 -> 3/3; witness `105v116` (Kitchen Finks 3/2: BOTH died) | **FALSE — fixed this wave** |
| 6 | Contagious Nim {2}{B} 2/2 | Oracle | TRUE |
| 7 | Cystbearer {2}{G} 2/3 | Oracle | TRUE |
| 8 | Ichor Rats {1}{B}{B} 2/1, "on entry EACH player gets a poison counter, you included" | Oracle: `When this creature enters, each player gets a poison counter.` | TRUE |
| 8b | ..."**Neither counter is announced in the log — add both by hand**" | render now emits `- Poison: you take 1 poison counter - now 1 of 10` AND the opponent's line | **FALSE (superseded by i1) — deleted this wave** |
| 9 | Tel-Jilad Fallen {2}{G}{G} 3/1, protection from artifacts, "no artifact creature can block it" | Oracle: `Protection from artifacts`; CR 702.16e | TRUE |
| 10 | Blackcleave Goblin {3}{B} 2/1 haste | Oracle | TRUE |
| 11 | Corpse Cur {4} 2/2, "on entry you MAY return an infect creature from your graveyard to hand" | Oracle: `you may return target creature card with infect from your graveyard to your hand` | TRUE |
| 12 | Skithiryx {3}{B}{B} 4/4 flying; "{B}: gains haste until end of turn"; "{B}{B}: regenerate" | Oracle, verbatim match | TRUE |
| 13 | Putrefax {3}{G}{G} 5/3 trample haste, "SACRIFICED at the beginning of the end step, so it exists for exactly one attack" | Oracle: `At the beginning of the end step, sacrifice this creature.` | TRUE |
| 14 | Hand of the Praetors {3}{B} 3/2; "your other infect creatures get +1/+1"; "whenever you CAST an infect creature spell target player gets a poison counter" | Oracle, verbatim match; EXERCISED and correct in `139v105` | TRUE |
| 15 | Relic Putrescence {2}{B}, "enchant an ARTIFACT; when that artifact becomes tapped its controller gets a poison counter" | Oracle, verbatim match | TRUE (see §6 for the unproven engine half) |
| 16 | "Your cheapest card costs TWO mana; this deck has no 1-drops at all" | decklist + primitives: cheapest MV is 2 (Blight Mamba, Plague Stinger, Necropede, Ichorclaw Myr) | TRUE |
| 17 | "there are 35 [lands] in the deck" | `deck105.txt`: 21 Forest + 14 Swamp = 35 of 65 | TRUE |

**Ledger result: 17 claims checked, 2 FALSE (5b, 8b), both fixed in the shipped revision.** Per
HL2 rung 3 the FALSE CLASSES were swept, not point-fixed: (5b) every "your creature wins the
combat" assertion in the guide was re-read — Blight Mamba's regenerate line and Tel-Jilad's
"no artifact creature can block it" survive the sweep; (8b) every "the log does not print X"
assertion was re-read — all of them were poison-render claims and all are now false, which is why
§4 deletes the whole counting section rather than editing it.

---

## 4. HL3 QUOTED-STRING SWEEP — run BEFORE the log read, per the rung

Every substring the deployed guide presents as something the pilot will SEE, grepped verbatim
against the 123 prompts of this seat:

| quoted string | hits | verdict |
|---|---|---|
| `Unblocked, these attackers deal up to ... - you would be at ... - NOT lethal` | 17 | LIVE (deck105 never faces infect, so it always gets this branch) |
| `LETHAL` (as "the line says LETHAL") | 4 (`LETHAL if it all connects: block enough to survive`) | LIVE |
| `Play <land>` option | 46 | LIVE |
| `Your creatures that can attack` | 36 | LIVE |
| **`Damage: N dealt by <one of YOUR creatures> to the opponent`** (the poison-counting procedure's key) | **0** | **DEAD STRING — the poison half of this emitter was replaced by `- Infect damage: N from X to the opponent ...`. Zero `- Damage: N dealt by X to the opponent` lines exist in the seat's entire corpus.** |
| `A "Damage" line with no "Opponent's life -N" beside it is poison, not life` (the inference rule) | n/a | SUPERSEDED — the render now says it outright |
| `The board does not print the poison count` (falsity fact) | n/a | **NOW FALSE — the board prints it in 63/63 prompts** |

**STOPGAP RETIREMENT — the two-step verdict (HL3 rung 2).**
(a) **Is the original class dead?** YES: 0/123 hand-count re-derivations, 0 poison/life
conflations, no land drop in the top-5 tax rows.
(b) **What does the residual tax rehearse NOW?** The tax MOVED, it did not vanish: the seat's four
largest replies are all **blocker-seam or survival decisions** (11,912 / 11,414 / 8,424 / 8,286
chars, 174 s / 187 s / 130 s / 134 s), and their content is the pilot litigating the guide's own
block gate against a board that is about to kill it (`105v146 s11/s12` is the purest specimen:
7,788 chars spent on a LAND DROP arguing about the block rule). **The successor belief is "the
guide forbids me to block and I am about to die."** The stopgap is therefore not merely retired —
its slot is reallocated to the gate rewrite in §L1/§L2, which is what the residual tax is
actually about.

Latency moved with it: median 17.1 s -> **15.8 s**, but p90 61.8 s -> **75.6 s** and the top rows
relocated from poison-counting to the combat seam. Max fell 219 s -> 187 s.

---

## 5. COMMIT-FAILURE COUNTER — counted fields, and the field's blind spot

Reported as counted fields per the brief, for the 123 decisions at deck105's seat and, in
parentheses, for all 240 decisions across both seats of these 6 games.

| field | deck105 seat | both seats |
|---|---|---|
| replies that continue past their own `PLAN:` line (`post_plan_overrun` > 0) | **15 / 123 (12.2%)** | 26 / 240 (10.8%) |
| mean `post_plan_overrun` over all decisions | **225 chars** | 156 chars |
| largest `post_plan_overrun` | 7,651 (`105v146 s16`, priority, 134 s) | 7,651 (same) |
| `commit_retracted` == True | **0 / 123** | **0 / 240** |
| of those, FATAL | 0 | 0 |
| `pruned_pairs` | not emitted at this seat | — |
| replies with NO `PLAN:` line (protocol-compliant since f46dd58ee) | 1 / 123 | 3 / 240 |
| `unparsed_reply` / `empty_reply` / `retracted_choice` / `truncated_abandoned` / `validation_reject_*` / `deferred_to_heuristic` | **0** | **0** |
| `all_assignments_illegal` | 0 | 0 (corpus-wide: 1, in `152 v 36`, not my seat) |

**PROMOTION TRIGGER (>=2 seats with a fatal overrun+retraction): NOT MET AT THIS SEAT — 0
retractions recorded. But the field is under-counting, and this is the load-bearing observation.**

`commit_retracted` fired 0 times, yet **7 of 240 replies (2.9%; 5 of 123 = 4.1% at deck105's
seat) end on a DIFFERENT answer label than they began with** — the exact class the field exists to
measure. It misses them because the re-commit happens BEFORE the `PLAN:` line, so
`post_plan_overrun` is 0 and no retraction is recorded:

| repro | first label | last label (taken) | value of the change |
|---|---|---|---|
| `105v146 s15` blockers t8 | `BLOCKS: B1:A2, B2:A1` | `BLOCKS: none` | **NEGATIVE — this is the guide-caused loss in §L1. The scratch text converted a survivable block into the losing no-block.** overrun 0, retracted False |
| `105v146 s16` priority t9 | `CHOICE: 0 (pass)` — with the CORRECT reason (*"Regenerating Blight Mamba now is unnecessary because it is not currently being destroyed"*) | `CHOICE: 1 (Regenerate with Blight Mamba)` | **NEGATIVE — and rules-wrong.** It regenerated on its OWN upkeep planning to block on the opponent's next turn; a regeneration shield lasts until end of turn (CR 701.15), so it spent {1}{G} at 1 life for nothing. overrun 7,651 / 134 s |
| `105v36 s24` blockers t11 | `Plague Stinger: Glaze Fiend` | `Plague Stinger: Master of Etherium` | neutral (dead either way at 3 life vs 16) |
| `105v116 s5` ask t4 | `2 (Play Swamp)` | `1 (Play Forest)` | neutral |
| `105v152 s16` blockers t10 | `B1:A4` | `B1:A1` | neutral |
| `116v105 s9` ask t7 | `3 (Play no land right now)` | `2 (Play Glimmerpost)` | positive (small) |
| `158v105 s11` ask t6 | target Ichorclaw Myr | target Cystbearer | positive (small) |

**At this seat the post-answer self-correction is net NEGATIVE: the two decision-relevant flips
were both harmful, one of them fatally.**

---

## 6. Card/engine items EXERCISED for the first time at this seat

- **Hand of the Praetors — EXERCISED, correct end-to-end** (wave-32: drawn 0 times). Lord bonus
  visible in the rendered P/T (`Cystbearer {2}{g} (3/4) (printed 2/3) [infect]`), cast-trigger
  fired per infect creature spell, 3/3 targets pointed at the opponent, and it carried the vs139
  win. The wave-32 probe debt for this card is DISCHARGED by natural play.
- **Relic Putrescence — STILL UNPROVEN.** Cast once (`105v36 s16`) and correctly attached to an
  OPPONENT artifact (`Mox Jet`, chosen from 11 targets — the guide's targeting teach worked). It
  sat attached for three turns and deck36's poison total never moved off `1 of 10`. Whether the
  enchanted Mox Jet ever became tapped is not rendered, so the trigger is **neither validated nor
  falsified**. The wave-32 probe spec (does `@tapped(mytgt):damage:1 targetcontroller` on an
  `abilities=infect` Aura produce a POISON counter or 1 life loss?) still stands.
- **The LLM damage-assignment ORDER seam (`d083a0629`) — EXERCISED, 3 windows, correct, expensive.**
  `139v105 s14/s15/s16`: Cystbearer (3/4) blocked by 4 creatures, three sequential position asks
  (`Combat damage order: your attacker Cystbearer (3/4) is blocked by 4 creatures. Damage is
  assigned in order, up to each blocker's toughness. Choose the blocker dealt damage in position
  N.`). Picks were legal and sensible (Arboreal Grazer 0/3 first, killing it exactly with 3). Cost:
  **140 s and 11,915 chars across three asks** for a decision worth one creature. Reported as a
  cost observation, not a defect.
- **Putrefax / Skithiryx — UNEXERCISED this corpus** (neither drawn). Guide lines retained in the
  HL6 positive form (rule + both outcomes + explicit no-op clause), no frequency claim.

---

## 7. Parser / render contradictions, verbatim (-> notes.md)

1. **Batched -1/-1 counter lines print the POST-BATCH P/T on every line.**
   `- Counter added to Arboreal Grazer #1: -1/-1 (now -3/0)` printed three times identically for a
   0/3 taking three counters (should read `(now -1/2)`, `(now -2/1)`, `(now -3/0)`). Same at
   `Kitchen Finks: -1/-1 (now 0/-1)` x3 and `Pollywog Symbiote #1: -1/-1 (now -2/0)` x3.
   -> **N-105f**.
2. **Instance suffixes are re-indexed after a death, so the append-only log contradicts the board
   snapshot.** `139v105 s18` cost an 11,089-char / 115 s spiral; verbatim:
   *"'Opponent's Arboreal Grazer: battlefield -> graveyard'. It died. ... But the 'CURRENT SITUATION'
   block explicitly lists them as 'Opponent battlefield (creatures: 3): ... Arboreal Grazer ...
   Pollywog Symbiote #1 ...'. This is a contradiction."* The counter narration says `#2` took the
   counter; the board snapshot shows the counter on `#1`. -> **N-105g** (a same-zone face of the
   carried ledger item L2 cross-zone dup-name).
3. **Zone-move narration drops the instance suffix** that the counter lines carry
   (`- Opponent's Arboreal Grazer: battlefield -> graveyard` vs
   `- Counter added to Arboreal Grazer #1:`) — the proximate cause of (2). -> N-105g.
4. **N-105d (the `(creatures: N)` battlefield header)** — the wave-32 misread did NOT recur at
   this seat in 123 decisions. Steady state; do not re-report.
5. **N-105e (bottom-line card text truncating mid-word)** — UNEXERCISED (0 mulligans, 0 bottoming
   asks). Carried, not observed.

---

## 8. Guide disposition

**REVISED — `strategy.txt` in this directory.** Four changes, each traced to a repro above:
(1) the whole "COUNT THE POISON" stopgap DELETED (its removal condition fired; its key string has
zero hits) and replaced with a two-line read-the-render instruction quoting the live status line;
(2) the #1 block rule rewritten — the gate number is NAMED instead of called "N" (the s11/s12
contradiction and the s15 loss), the threshold raised, the SITUATIONS examples rewritten to match
the rule, and a growth clause added keyed to `[counters: Nx +1/+1]` / `(printed X/Y)`;
(3) the Ichorclaw Myr and Ichor Rats false facts fixed; (4) a colour clause added to the mulligan
keep rule, keyed to the engine's own `Playing every land in this hand would not cover any spell in
it.` No frequency adjective appears anywhere in the revision (pre-ship grep for
`rarely|often|usually|regularly|typically|expect|most of the time|by default|almost always|seldom`
run and clean, apart from the two occurrences inside strings the guide QUOTES from the engine).

Preserve ledger untouched: the attack floor, the deploy floor and the Ichor Rats pricing keep
their one confirmatory clause each and no new gate is placed in front of them.
