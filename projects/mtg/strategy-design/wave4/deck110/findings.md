# Deck-110 (Etched Affinity) — Wave-4 game review

**Evidence base.** deck110's 12 seat translogs across the two 21-game validation runs
(`matchups-20260712-192832/` = run1, `matchups-20260712-230636/` = run2). 487 logged
decisions. Citations are `<epoch prefix>` + seq (+ turn).

**Record: 9–2, 1 timeout — the best deck in the pool.**

| file (epoch) | run | matchup | result | end turn | notes |
|---|---|---|---|---|---|
| 1783902515 | r1 | vs 109 Hellrider | **LOSS** my0/op8 | 8 | raced out; plating equipped T6 |
| 1783902517 | r1 | vs 131 | WIN my27/op-1 | 7 | clean |
| 1783903101 | r1 | vs 140 Lightmine control | WIN my14/op-4 | **27** | slow slog; huge re-equip waste |
| 1783904575 | r1 | vs 135 | WIN my20/op-2 | 9 | clean |
| 1783905283 | r1 | vs 44 | WIN my18/op-4 | 13 | clean; Blast clears a flier |
| 1783908830 | r1 | vs 133 Phyrexian | **TIMEOUT** my5/op22 | 12+ | stopped attacking after T4 |
| 1783915603 | r2 | vs 133 Phyrexian | WIN my18/op-3 | 10 | same matchup, won |
| 1783915940 | r2 | vs 135 | WIN my14/op-1 | 9 | clean, double Blast finish |
| 1783919558 | r2 | vs 140 | WIN my20/op-7 | 9 | clean (better draw than r1) |
| 1783920257 | r2 | vs 109 Hellrider | **LOSS** my0/op5 | 10 | rode a lone Signal Pest |
| 1783921985 | r2 | vs 131 | WIN my20/op-5 | 7 | textbook: deploy→equip→2x Blast face |
| 1783922880 | r2 | vs 44 | WIN my7/op-8 | 9 | clean |

Both losses are to **deck109 "Hellrider" (R aggro)**; the timeout and the 27-turn win are
both grindy/hostile boards (Phyrexian lifegain; RBW control with 4× Lightmine Field).

---

## TOP FINDINGS (revise the guide around these)

### 1. [FIXED — the wave-3 catastrophe is gone; propagate what caused it]
Wave-3 deck110 went **0/6 with five 40-minute timeouts**, driven by the "Mana in your pool:
(none)" delusion (44–88% cast-nothing rates) and 8% empty-reply fallbacks. Wave-4 is **9/2/1t,
games end by KILLING the opponent (opp life negative) around turn 7–13, and `fallback`=0 in
every game.** The representation fix shipped — the board now reads
`Mana available: {u}{r}{r}{w}{w} from 5 untapped sources (tapped automatically when you cast)`
— and the mana-delusion pass count fell from pervasive to **7 borderline cases in 487 decisions,
several of them CORRECT** (e.g. `1783903101` seq26 declines the `{B}{B}` attach line with no black
mana — exactly what the guide asks). **This is the headline: the guide's #1 mana rule + the
representation change together solved the corpus-killer. Do not weaken either.** Wave-3 findings
#1 (mana delusion), #3 (empty-reply fallback), #6 (blockers never firing — now 6 blocker
decisions logged) are all **FIXED**.

### 2. [REGRESSED — now the deck's #1 inefficiency] Re-equip loop is back and severe
The guide's explicit RULE 3 ("if the creature's power already shows the bonus … do NOT equip
again") is being **ignored**. 47 Cranial-Plating equips were taken across 12 games (correct play
needs ~1 per game, occasionally 2–3 to move carriers). Worst cases:
- `1783903101` (vs 140) **T19: FIVE consecutive equips on Etched Champion** (seq44–48). seq44 the
  Champion is `(2/2)` — first equip correct. seq46 and seq48 the board snapshot plainly reads
  `Etched Champion (6/2) (printed 2/2)` — plating is visibly attached — yet it equips twice more,
  draining `{r}{r}{w}` → `{w}` → tapped out. **T23 it does it again: five equips on Master of
  Etherium** (seq61–65).
- `1783902515` (vs 109 loss) T8 seq26–28: three equips on Ornithopter.
- `1783920257` (vs 109 loss) re-equips Signal Pest at T6 (seq22–23) and T8 (seq30–31).

Root cause is the engine **re-offering** the no-op equip line every priority window (wave-3
finding #5, still unfixed at the engine/menu layer) combined with the model not reading the
`(printed X/Y)` power tag as "already attached." This directly produces durdle (it is the main
reason the vs-140 game ran 27 turns) and wastes the mana that should deploy a second threat.
**PERSISTING/REGRESSED — the single highest-value fix.** The guide text alone did not hold;
consider both a stronger positive framing AND flagging the engine re-offer (see §6).

### 3. [LOSS PATTERN] The aggro mirror (vs 109 Hellrider) — deploys too narrow, too slow
Both losses are the same story: against a fast go-wide R-aggro deck, deck110 **rode a single
small attacker and never built a board.**
- `1783920257` (r2, my0/op5): its entire offense is ONE Signal Pest (0/1 → 5/1 equipped). It cast
  a 2nd Signal Pest (T4) and Steel Overseer (T8) but **only ever attacked with one creature.** It
  chump-blocked Ash Zealot with the spare Signal Pest (T5 seq19), re-equipped instead of
  developing (T6, T8), and at **T9 with the opponent at 5 declined ALL blocks** against a 6-power
  alpha strike (seq40), dropping 8→2 and dying T10. It out-*damaged* the clock (op 20→5) but lost
  the race because it deployed one threat at a time while Hellrider went wide.
- `1783902515` (r1, my0/op8): Cranial Plating cast **T2 but not equipped until T6** — a 4-turn
  dead-plating delay (the exact wave-3 sin), while it attacked with a 0-power Ornithopter for
  nothing. Equipped Steel Overseer (T6) then Ornithopter (T8, +3x re-equip), fired one Blast at
  face T8, and died on the crackback.

The guide says "you are the BEATDOWN in every matchup." Against Hellrider that framing is
actively misleading — it's the **aggro mirror**, where deck110 must (a) deploy the WHOLE hand
wide immediately rather than one-threat-then-equip, and (b) be willing to trade/chump to not die
on the crackback when it is the slower clock. **No guidance covers the mirror.** This is the
clearest place a guide revision could convert losses.

### 4. [DURDLE] Against grindy/hostile boards it stops attacking and plays one dork per turn
- `1783908830` (vs 133, the timeout): **it declares no attackers at all after T4.** It plays
  Signal Pest T0, durdles, and from T6–T12 just plays lands and re-deploys/​re-equips small
  creatures that keep dying to deck133's defense, bleeding 20→5 while the opponent gains to 22.
  Run2's game vs the SAME deck (`1783915603`) was won on turn 10 — so the matchup is winnable;
  the r1 game was a pure durdle spiral once its lone equipped threat kept getting answered.
- `1783903101` (vs 140, 27-turn win): partly forced — deck140 has 4× **Lightmine Field** (damages
  each attacker by the number of attackers), so mass attacks kill deck110's own 1-toughness
  creatures and the correct plan really is "send one fat protected Etched Champion." But the model
  padded that with the five-equip loops (§2) and long stretches of "Cast nothing," turning a
  should-be-tidy grind into 27 turns.

The through-line: when its equipped attacker is neutralized, the model **reverts to redeploying
one small creature per turn** instead of committing multiple bodies. The guide's "attack every
turn / go wide" intent is present but loses to the one-threat-at-a-time habit whenever the first
threat is contested.

### 5. [WORKED — propagate] The finisher and combat aggression are excellent
- **Galvanic Blast to the face at lethal range is reliably correct.** In the clean wins the model
  chains **two Blasts at the opponent's face then swings for lethal** (`1783921985` T7: op11→7→3
  then equipped Memnite kills; `1783915940` T5/T7 double-Blast). Across all games Blasts almost
  always target the opponent's face; the few at creatures are defensible (`1783905283` T7 clears a
  3/3 flying blocker). The guide's lethal-check + "point Blasts at face when opp ≤8" is landing.
- **Attack aggression and carrier selection are sound when a threat is live** — it equips Etched
  Champion / Memnite / a flier as the carrier and swings. Wave-3's judgment that "the strategic
  layer holds; the failures are mechanical" **still stands.**

---

## Supporting detail / secondary items

- **Menu noise persists (representation, general/engine layer).** `Cast Card Normally with Mox
  Opal` and `Play Land with Mox Opal` are the engine surfacing Mox Opal's mana ability as a
  priority option; the label reads like casting a spell and visibly confuses the model's PLAN
  (`1783920257` seq4: it picks it, then rambles about "activate Mox Opal to generate mana"). The
  `{B}{B}` attach line for Cranial Plating is still offered on a deck with no black sources (pure
  noise). Consistent with wave-3 #5 — the GPT pilot needs a cleaner menu; these clutter every
  decision and cost calls.
- **Multi-Plating greed (benign).** Several wins cast 2–3 Cranial Platings (`1783921985` T3/T5/T7)
  rather than creatures. Platings stack (+1/+0 each) and count for metalcraft, so it isn't wrong,
  but casting a 2nd/3rd Plating over a creature slightly narrows the board — a minor contributor
  to the "too narrow" pattern in §3.
- **0-power Ornithopter attacks (minor).** `1783902515` sends an unequipped Ornithopter (0/2) into
  combat for 0 damage. The `#HINT:dontattackwith(creature[power<=0])` should suppress this;
  Signal Pest at 0 power attacking is intended (battle cry) but the deck-list hint may not be
  reaching the attacker seam. Low impact.
- **Steel Overseer underused.** Only 3 `Counter with Steel Overseer [cost: Tap]` activations taken
  across all games (deck runs 2 copies, so partly draw). When it did hit the board it was often
  re-equipped rather than tapped for counters. The guide asks for the tap "EVERY turn"; it's being
  skipped.

## Wave-3 findings status
- #1 mana delusion — **FIXED** (representation change + guide rule).
- #2 pacing/durdle timeouts — **MOSTLY FIXED** (games now finish; 1 timeout remains, matchup-specific §4).
- #3 empty-reply fallback (8%) — **FIXED** (fallback=0 in all 12 games; likely the -j3 / timeout change).
- #4 plan/number mismatch — **FIXED** (no cast-a-plan-then-pass instances observed; replies execute their stated pick).
- #5 engine re-offers no-op equip + `{B}{B}` noise — **PERSISTING** at the engine/menu layer; drove finding §2.
- #6 blockers never reaching the GPT seam — **FIXED** (6 blocker decisions logged, blocks assigned correctly).

## Handoff for the guide revision
1. **Re-equip loop (§2)** is the top lever. The current negative-framed RULE 3 isn't holding.
   Reframe positively per the known "negative framing bleeds" lesson — e.g. tell it the equipped
   carrier's power tag `(printed X/Y)` means the Plating is DONE and this turn's job is now to
   ATTACK or deploy the next body, not touch the equip line again. Pair with an engine-side
   suppress/annotate of the already-attached equip line if feasible.
2. **Add an aggro-mirror clause (§3):** vs a faster go-wide aggro opponent, dump the hand WIDE
   turn 1–2 (multiple bodies before equipping), and once you are the slower clock it is correct to
   chump/trade to survive the crackback — don't die at 8 holding blockers.
3. **Anti-durdle vs contested boards (§4):** if your equipped threat keeps dying, commit MORE
   bodies at once rather than one-per-turn; keep declaring attackers every turn even after a
   threat is answered (the timeout came from ceasing attacks at T4).
4. **Do NOT weaken** the mana rule, the lethal/Blast-to-face directive, or the attack-aggression
   priors — all three are demonstrably working (§1, §5).
