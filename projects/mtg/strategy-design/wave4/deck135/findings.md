# Deck-135 (Modern Snow) — Wave-4 game review

Corpus: two 21-game GPT-vs-GPT runs, deck135 seat files:
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260712-192832/` (R1) and
`.../matchups-20260712-230636/` (R2). Deck135 record: R1 3W-2L-1TO, R2 2W-3L-1TO
(5/12). Citations use RUN + opponent + `seq`/turn. Game→file map at the bottom.

Deck plan (what it is built to do): base-green snow tempo-value. Cheap self-replacing
snow creatures (Ice-Fang Coatl, Ohran Viper) grind cards; Abominable Treefolk (power =
snow-permanent count, trample) or Diamond Faerie (flyer + team pump) close. It must
DEPLOY A THREAT AND ATTACK EVERY TURN; when it durdles it stalls at ~20 opp life and
loses or times out. Blue mana (Islands + Astrolabe) gates half the deck.

The single biggest wave-4 problem is unchanged in shape from wave-3 but has MOVED: the
deck still fails to close, but the cause is no longer blue-mana screw (wave-3 fixed
that). It is now **passivity that the current guide actively feeds** — Island-fetch
obsession, attack-avoidance around any punisher, and deferred-plan loops on its own win
conditions. Aggression correlates cleanly with winning (wins declare 3-8 real attacks;
losses/timeouts declare 0-1).

---

## TOP FINDINGS (ranked by frequency x cost-to-winning)

### 1. Chronic under-attacking — the deck stops attacking whenever the board isn't wide open — SEV: GAME-LOSING, RECURS IN EVERY LOSS/TIMEOUT

The guide says "attack every turn," but in all 5 losses and both timeouts the model
stops attacking as soon as the opponent presents ANY friction, and the opponent's life
barely moves. Two triggers:

- **Lightmine-Field over-caution → total attack freeze.** R2 vs140 (TIMEOUT): from T10
  on, the model declares "no attackers" turn after turn (seq 44, 59, 74) and passes,
  reasoning it will not take *any* Lightmine damage — even though its Ohran Viper is a
  **1/3 that survives the 1 damage from attacking alone** and draws a card on hit. It
  plans "attack next turn with Viper alone" from ~T12 to the T32 timeout and never once
  does. Opponent climbs 21→31 while deck135 sits frozen at 13-15 life. The guide's
  Lightmine clause ("attack with ONE creature whose toughness is 2+") is correct but the
  model reads it as "do not attack"; it needs the POSITIVE form ("attack with Ohran Viper
  ALONE every turn — 1/3 survives 1 damage and draws you a card").
- **Fear of any bad trade → no clock at all.** R2 vs44 (LOSS, opponent still at 20 when
  deck135 died at -1, T12): in 56 decisions the model declared attackers with a real
  attacker exactly **once**. It held Icehide Golem (2/2) home turn after turn rather than
  trade into Oona's Gatewarden, dealt ~0 damage all game, and was chipped out by evasive
  faeries. Same shape R1 vs44 and R2 vs110.

Aggression tally (real attacker declared / "no attackers"): wins 3, 3, 5, 8, 2, 3;
losses/timeouts 0, 3, 1, 4, and R2vs140 = 1 real vs 4 "none". The wins are simply the
games where it kept swinging.

**Fix direction:** an act-floor for COMBAT, not just casting. The guide already has "cast
every turn"; it needs the mirror "declare at least one attacker every turn unless a
listed attacker dies for free," and the Lightmine warning rewritten as a positive
single-attacker instruction, not a prohibition.

### 2. Win condition offered and refused — Diamond Faerie offered 22x, cast 0x (R2 vs140) — SEV: GAME-LOSING (caused a timeout)

In R2 vs140, "Cast Diamond Faerie" appeared as a numbered legal option in **22 of 100
records and was chosen 0 times**, while the model named Diamond Faerie in its PLAN in
**55 records**. Its own finisher — a 3/3 flyer that dodges Lightmine (Lightmine only hits
attackers on the ground; it can't stop a flyer being declared) and pumps the snow team
`{1}{snow}: all your snow creatures +1/+1` — was castable and wanted, and never cast.
Three compounding reasons, all visible in the replies:

- **Deferral loop:** seq 32, 33, 35 all plan "cast Diamond Faerie NEXT turn"; it re-reads
  and re-affirms that intention for ~20 turns (wave-3 F3 stale-plan loop, PERSISTING).
- **"No mana" false belief on its own main** (seq 35, 36, 39: "I have no mana to cast
  spells" while it has untapped lands) — the wave-3 / general-prompt "empty pool is
  normal" fact still losing locally here.
- **Turn-ownership confusion:** it declines Diamond Faerie as "wrong phase; it is the
  opponent's turn" while being offered a sorcery-speed Cast option (which only appears on
  its OWN main). PERSISTING wave-3 F3 second shape.

### 3. Island-fetch obsession — Rule #1 over-corrected into durdle — SEV: GAME-LOSING (durdle/timeout fuel)

Wave-3's Rule #1 ("GET AN ISLAND BY TURN 3") cured the blue-screw, but the model now
spends an outsized share of its decisions HUNTING Islands and planning future blue spells
instead of deploying and attacking. Fetch-activation decision counts: R1 vs44 15/65,
R2 vs109 16/59, R2 vs140 10/100, R2 vs44 8/56 — versus deploying a threat and swinging.
In R2 vs44 nearly every plan is "fetch a Snow-Covered Island to enable my blue spells"
for blue spells it never draws or casts, while Icehide Golem and Boreal Druid sit on the
board not attacking. This is the wave-3 diagnosis moving downstream exactly as the method
doc warns: the blocking action (get blue) is fixed, and the next bottleneck (turn that
blue into a board and a clock) is now the leak. The guide should demote Rule #1 from the
loud #1 slot to a supporting line ("one Island is enough — after that, fetch to develop
and ATTACK, do not keep hunting Islands") and promote the act/attack floor to #1.

### 4. Fetch re-offer loop PERSISTS (wave-3 F2) — SEV: MODERATE (inference budget = timeout fuel)

The same fetch crack is re-offered and re-answered across Main1→Main2→Upkeep before it
executes: R2 vs140 seq 2-4 (Prismatic Vista across three phases), seq 8-9 (Windswept
Heath); R2 vs44 seq 2-3, 23-24, 31-32, 36-37 (Flooded Strand, four separate 2x-3x runs).
Each re-ask is a full model call and each grows the narration. With games hitting the
40-min cap at 100-143 decisions, this remains direct timeout fuel and it re-seeds the
stale-plan pollution in Finding 2. This is an engine/harness seam, not a guide item —
carried forward for the platform track.

### 5. Survival-check at low life still imperfect (wave-3 S4) — SEV: GAME-LOSING in instances, MIXED

R2 vs110 (LOSS): at **3 life** (seq 21, T8) the model attacked with BOTH Icehide Golems
("A1, A2"), emptying its board of blockers against an aggro deck, and died the next turn.
This is the wave-3 S4 "attack with everything at low life" failure, PERSISTING. It is
mixed, though: R2 vs109 seq 35 shows the survival math working — at exactly-lethal 7 life
vs three attackers (1+3+3) it correctly assigned all three blockers to survive. So the
model CAN run the defensive sum; it just doesn't reliably decide to hold blockers on its
own attack step. Needs the "keep back enough to survive the swing-back" instruction tied
to a life threshold the model checks before declaring attackers.

---

## WAVE-3 STATUS LEDGER

**FIXED / IMPROVED:**
- **Blockers seam now fires (wave-3 F1, the #1 wave-3 engine item).** 9 blocker decisions
  across these 12 games vs. 3 in 21 games in wave-3. Defensive blocks are mostly correct:
  double-block a lethal attacker to survive (R1 vs110 seq 16), block-all-to-survive at
  exactly-lethal life (R2 vs109 seq 35), sensible chump/trade blocks (R2 vs131 seq 12
  Boreal Druid on Young Pyromancer). The game-deciding "no blockers ask on an alpha
  strike" failure did not recur. This is the single biggest platform improvement.
- **Fetch cost-cracking still works** (wave-2 headline). No cost-refusal failures; fetches
  crack promptly. Residual is F2's mechanical re-offer, not cost aversion.
- **Force of Negation now used correctly on board wipes.** R2 vs140 seq 61 countered
  Damnation (exiling Diamond Faerie), seq 75 countered Pyroclasm — exactly the guide's
  "save it for a board wipe" line. The reflexive-counter failure is gone.

**PERSISTING:**
- Stale-plan / deferred-plan loops (F3): verbatim plan echo carried across turns — e.g.
  R2 vs140 seq 67-73 repeat "Cast Ohran Viper... attack with Viper alone" identically
  T16→T20 while never executed. Turn-ownership confusion ("it is the opponent's turn" on
  its own main) also persists (Finding 2). The plan text visibly pollutes later replies
  (many replies begin with a stray fragment of the previous plan, e.g. ". Cast Glacial
  Revelation {2}{g} PLAN: ...").
- "No mana" false belief on the model's own main with untapped lands (Finding 2).
- S4 low-life over-attacking (Finding 5).
- Force-of-Negation holding still drives passivity: of 43 "Cast nothing" decisions across
  the 12 games, 15 cite holding Force of Negation and 19 cite waiting on the opponent.
  Less dominant than wave-3 but still the largest single durdle rationale.

**REGRESSED / NEW:**
- Island-fetch durdle (Finding 3) is a NEW downstream failure created by the wave-3 Rule
  #1 over-emphasis. Not present as such in wave-3 (which had the opposite problem).

---

## MATCHUP-SPECIFIC

- **vs110 (artifact/affinity aggro: Ornithopter, Memnite, Mox Opal) — 0-2, both ~T9
  deaths.** The guide's defense section is scoped to "VS RED AGGRO (Stromkirk Noble,
  goblin tokens, burn)" — deck110 is NOT red, so the model does not apply the "deploy
  blockers before ramp" rule. In both games it spent T0-5 cracking fetches for Islands
  while being beaten 20→7 (R1 vs110), then deployed too late. The aggro-defense trigger
  should be generalized from "red aggro" to "any deck that has attacked you twice by T3 /
  dropped you below ~14 early," keyed on the observable, not on card names.
- **vs44 (UB faeries) — 0-2.** Pure failure-to-close: evasive 1/1s chip deck135 out while
  it fails to build a clock (Finding 1/3). In R2 vs44 seq 12 the model even wrote "Cast
  nothing this turn... but this is likely a mistake given the strategy guide" and passed
  anyway — it OBEYED its passive default against its own stated knowledge.
- **vs109 (RB aggro: Rakdos Cackler, Gore-House Chainwalker, Boros Reckoner, goblins) —
  1-1.** Here the model defended better (blocked the red attackers, Finding "FIXED"), so
  the blocker-seam fix directly helped this matchup. The loss (R2) was a slow bleed to 0
  while the opponent stayed near 17-19 — again failure to close, not bad blocking.
- **vs131 / vs133 — the wins.** Same deck, aggression flipped on: 5-8 real attacks,
  Treefolk/Coatl/Viper deployed and swinging. Confirms wave-3's mirror-lever reading (wins
  = attack every turn). Note even wins can be 26-turn grinds (R2 vs131, 124 decisions) —
  the deck is slow to actually kill even when ahead.

## MODEL-QUALITY RESIDUALS (for the model-experiment / OpenRouter track — not guide-fixable)

- **Degenerate output at a blockers seam:** R1 vs109 seq 17 — the reply collapsed to
  "** ** ** ** ..." repeated, empty choice, no valid block declared. Rare but real; a
  blocker decision was silently lost. Good litmus for candidate models.
- **Illegal blocker assignment:** R1 vs44 seq 35 — "B1:A1, B1:A2" assigns the SAME blocker
  to two attackers (the prompt explicitly forbids this). The tolerant parser recovered
  (resolved to the single legal block), but the model produced an illegal plan.
- **Lightmine arithmetic:** the model treats ANY Lightmine damage as unacceptable even
  when its attacker survives (Ohran Viper 1/3 vs 1 damage) — it performs the retrieval and
  fails the "does my creature survive?" comparison (same class as wave-3's Lightmine
  math failure, different direction: wave-3 over-attacked, wave-4 under-attacks).

---

## GAME → FILE MAP

R1 = matchups-20260712-192832, R2 = matchups-20260712-230636. `<epoch>-ai_baka_deck135-*.jsonl`.

| run | epoch | opp | outcome | turns | final life (me v opp) |
|-----|-------|-----|---------|-------|-----|
| R1 | 1783904575 | vs110 | LOSS | 9 | -2 v 20 |
| R1 | 1783905418 | vs109 | WIN | 14 | 1 v -14 |
| R1 | 1783906513 | vs140 | TIMEOUT | 27 | 5 v 25 |
| R1 | 1783907132 | vs133 | WIN | 12 | 3 v -4 |
| R1 | 1783908913 | vs131 | WIN | 14 | 13 v -10 |
| R1 | 1783910753 | vs44 | LOSS | 16 | 0 v 11 |
| R2 | 1783915602 | vs131 | WIN | 26 | 14 v 0 |
| R2 | 1783915940 | vs110 | LOSS | 9 | -1 v 14 |
| R2 | 1783916692 | vs44 | LOSS | 12 | -1 v 20 |
| R2 | 1783917446 | vs109 | LOSS | 15 | 0 v 17 |
| R2 | 1783918305 | vs133 | WIN | 12 | 12 v -9 |
| R2 | 1783919025 | vs140 | TIMEOUT | 32 | 13 v 30 |
