# deck131 — wave 4 review (blue-red "spellslinger"; deck name is actually *Mind Control / counter deck*)

Corpus: two GPT-vs-GPT runs, qwen35 both seats:
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260712-192832/` (run1) and
`.../matchups-20260712-230636/` (run2). 12 deck131 games total.

**Record: 2 W / 8 L / 2 timeout.** Games (epoch = seat-jsonl epoch):

| result | vs | run | epoch | turns | end life (me/opp) | notes |
|---|---|---|---|---|---|---|
| WIN  | 109 | run1 | 1783910629 | 26 | 18 / 0  | curved payoffs, attacked every turn |
| WIN  | 44  | run2 | 1783922540 | 13 | 23 / -3 | curved payoffs, full-team alpha strike |
| LOSS | 110 | run1 | 1783902517 | 7  | -1 / 27 | fast durdle death, opp untouched |
| LOSS | 44  | run1 | 1783902519 | 20 | -4 / 16 | |
| LOSS | 133 | run1 | 1783906271 | 15 | -7 / 15 | |
| LOSS | 135 | run1 | 1783908913 | 14 | -10 / 13 | blocked with payoffs |
| LOSS | 109 | run2 | 1783915599 | 11 | -3 / 20 | 0 attacks, opp untouched |
| LOSS | 135 | run2 | 1783915602 | 26 | 0 / 14 | Guttersnipe used as blocker |
| LOSS | 110 | run2 | 1783921985 | 7  | -5 / 20 | pure durdle, no creature cast |
| LOSS | 133 | run2 | 1783922461 | 29 | -6 / 27 | Island-flood, 1st creature T18 |
| TIMEOUT | 140 | run1 | 1783903018 | 37 | — | lifegain deck outran the clock |
| TIMEOUT | 140 | run2 | 1783920139 | 36 | — | lifegain deck (opp -> 90 life) |

---

## The 3 findings that matter (lead with these for the guide rewrite)

### 1. The deck has TWO modes and the outcome is almost entirely which one it falls into. When it curves out its red payoffs it wins convincingly; when it doesn't it durdles on reactive cards and dies with the opponent near-untouched.

- **Both wins are textbook guide execution.** vs109 (1783910629): Young Pyromancer T6 -> attack T8 -> Guttersnipe T10 -> then **attacked with Guttersnipe every single turn T12-T26**, grinding opp 20->0. vs44 (1783922540): Mountain T1/T3, Young Pyro T3, Guttersnipe T5, **alpha-struck with the whole team (Pyro + Guttersnipe + 2 Elementals) every turn**, opp 20->-3 by T13. In both, the model followed the beatdown plan faithfully.
- **The 8 losses are the opposite picture: the opponent's life at death was 27, 16, 15, 13, 20, 14, 20, 27** — i.e. deck131 dealt almost no damage in games it lost. It reached for Prism Ring / Elixir / Cyclonic Rift / counters and never established a clock. Extreme cases: vs110 run2 (1783921985) cast Prism Ring + Elixir + Epiphany and **no creature at all**, dead T7 with opp at 20; vs109 run2 (1783915599) cast Cyclonic Rift/Downsize/Epiphany/Prism Ring/Aetherize, **zero attacks (atk 0/0)**, dead T11 opp at 20.

**Diagnosis: mostly DECK, partly MODEL.** The deck is a 60-card control shell (name literally "The Best Counter Deck Ever"): only **6 creatures** (3 Guttersnipe + 3 Young Pyromancer) and ~24 reactive/utility cards (4 Aetherize, 3 Essence Scatter, 2 Counterspell, 2 Dissipate, 2 Hydrolash, 3 Downsize, 3 Cyclonic Rift, 4 Prism Ring, 3 Elixir). The guide *reframes* it as "the beatdown in every matchup," but the reality is a durdle pile with two payoff cards. When the two payoffs + red mana show up early, the reframe works; when they don't, the model has nothing to be aggressive *with* and correctly-but-fatally reverts to the reactive cards the deck is actually built from. **No guide rewording fixes a hand with no threats in it.** This is the honest ceiling on deck131's win rate at its current build.

### 2. [MOST ACTIONABLE GUIDE FIX] The model repeatedly uses its beatdown creatures — Guttersnipe, Young Pyromancer, and Elemental tokens — as BLOCKERS, removing them from the race and sometimes trading the engine away. The guide talks about attacking but never forbids blocking.

Block decisions across the 12 games:
- vs135 run2 (1783915602) T16: **Guttersnipe blocks Ohran Viper.** Ohran Viper is a 1/1 whose combat damage destroys the creature it hit (`text=`: "destroy that creature at end of combat"). So Guttersnipe (the deck's entire kill condition) trades with a 1/1 utility snake. Catastrophic.
- vs109 run2 (1783915599) T5: **Elemental blocks Ash Zealot** (2/2 **first strike**, `text=` confirmed) — first strike kills the token before it deals damage; token dies for nothing, Ash Zealot untouched. This game then recorded 0 attacks and lost with opp at 20.
- vs133 run2 (1783922461) T19: Young Pyromancer blocks Bloodghast.
- vs135 run1 (1783908913) T6/T12/T14: Young Pyromancer blocks Boreal Druid (a 0/1 mana dork that deals no damage — a pointless block), then Elementals block Abominable Treefolk.

This is the "locally reasonable, globally blind" trap the project already knows: "there's an attacker, I have a creature, block it" is the control instinct, and it directly contradicts "you are the beatdown, attack every turn." **The guide's DO-NOT list covers land order and cast-nothing but has no line about blocking.** Add an explicit rule: *your Guttersnipe / Young Pyromancer / Elemental tokens ATTACK, they do not block — take the damage and race; only chump-block if you would otherwise die this turn.* This is the highest-leverage single change because obedience to concrete named rules is high (wave-3 finding 9) and this misplay recurs across four games and both runs.

### 3. Deck131 has no way to close against lifegain, and the model under-commits its attack — both vs140 games timed out.

- vs140 both games (1783903018, 1783920139): deck140 is a lifegain deck. In run2 the opponent's life climbed **19->23->30->37->45->53->69->90**. Deck131's clock is a single Guttersnipe hitting for 2 — it cannot outrace double-digit-per-turn lifegain. Even with perfect play these are unwinnable at this deck's power level (no reach, no burn-to-face, no evasion).
- Compounding it, the model **under-attacks**: at T31 (1783920139) it had 2 Young Pyromancers + Guttersnipe + Elemental tokens on board but the attackers record shows only "Guttersnipe" declared. The guide says attack with "Young Pyromancer, Guttersnipe, and all Elemental tokens"; the model swung with one creature. That won't matter vs a deck going to 90, but it's a real execution gap that costs damage in winnable games too.

**Diagnosis: DECK weakness (no reach/clock vs lifegain) + MODEL under-commitment.** The guide's LETHAL-CHECK math is good but doesn't force *maximum* board commitment each swing.

---

## Wave-3 findings — status

- **W3 #1 [CRITICAL] "GPT seam grants no opponent-turn priority windows" -> FIXED (engine).** Priority windows now appear in quantity (per-game `priority` kind counts: 14, 67, 37, 13, 52, 42, ...). The platform's phase auto-skip + bundled asks landed. Reactive decks are no longer structurally blank. (New nuance: windows now exist, but the model still passes most of them — that's now a decision-quality question, not an engine one.)
- **W3 #7 [MED] color-stranded / Mountain-first land rule -> FIXED and HOLDING.** In both wins the model played the Mountain the turn it drew it and cast the red payoff immediately. Verified the Island-flood losses were **draw variance, not rule violation**: in vs133 run2 (1783922461) the model played Island T2-T8 then Mountain **the very turn T10 it was first offered** — the Mountain simply wasn't in hand. The rule is being obeyed; the loss traces to the 14-Island/8-Mountain manabase starving an all-red threat suite (see mana-base note below).
- **W3 #3 [HIGH] "Cast nothing" passivity -> PARTIALLY FIXED, PERSISTING in the reactive-durdle losses.** The deck now casts freely in curve-out games, but in no-threat games it still strings "Cast nothing" across many windows (e.g. 1783922461). This is now inseparable from finding #1: with no threat drawn there's little *productive* to cast, so residual passivity is mostly a symptom of the deck, not the prompt.
- **W3 #9 model observations -> CONFIRMED.** Obedience to concrete named rules stays high (the win games quote the beatdown plan into action); principles/timing-heuristics stay weak. This is exactly why finding #2 (a concrete "don't block" rule) is the right lever.

## Draw-spell fix — HOLDING (not regressed)

The negatively-framed-clause rewrite is holding. **Draw casts (Artificer's Epiphany + Opportunity) = 12 across 12 games ~= 1.0/game**, vs the pre-rewrite baseline of ~1 across 6 games (0.17/game) and the post-rewrite A/B of 8 across 6 (1.33/game). Draw spells are being cast on-curve, not hoarded. No regression.

## Supporting detail / secondary notes

- **Mana base is suspect for an all-red payoff plan.** Every card that *wins* (Guttersnipe 2R, Young Pyromancer 1R) is red, yet the deck runs 14 Island / 8 Mountain. That skew is *why* the durdle-flood losses happen — the deck draws blue mana + blue reactive cards and starves the red threats. This is a **deck-construction** lever the guide cannot reach; worth flagging to whoever can edit the decklist (more Mountains, or fewer blue reactive cards). Guttersnipe reached the battlefield in only 9 casts across 12 games; Young Pyromancer 18.
- **The wins prove the guide is correct when the deck cooperates** — this is not a "bad guide" story. The rewrite's beatdown framing, cast-every-turn rule, Mountain-first rule, and draw-on-curve rule are all being executed in the games where the payoffs show up. The losing record traces primarily to (a) deck power level / threat density and (b) two concrete execution gaps the guide doesn't yet cover: blocking with payoffs (#2) and under-committed attacks (#3).
- **Guide vs reality mismatch to consider:** the guide asserts "you are the BEATDOWN in every matchup" and "you have no late game." Against aggro (vs110, vs109) the deck's reactive half is arguably its *better* plan, and against lifegain (vs140) it has neither a beatdown nor a control finish. The monolithic "always be the beatdown" is right against midrange/control but may be actively steering the model to cast its counters/bounce as durdle rather than to either commit fully to racing or use the reactive shell coherently. The next rewrite should decide whether to (a) double down on beatdown + add the anti-block/full-attack rules, accepting the deck simply loses when it doesn't draw threats, or (b) acknowledge the reactive half exists and give it one coherent job. Given high obedience to concrete rules and low deck threat density, (a) plus finding #2/#3 is the cleaner bet.

## What the losing record actually traces to (one line each)

- ~4 losses: **deck drew no early threat / flooded blue** -> durdled on reactive cards, dealt ~0 damage, died. (DECK)
- ~2 losses: **blocked with the beatdown creatures** instead of racing, incl. trading Guttersnipe for a 1/1. (MODEL + guide gap #2)
- 2 timeouts: **no clock vs lifegain** + under-committed attack. (DECK + model gap #3)
- 2 wins: **curved payoffs + attacked every turn** — the guide working as written. (executed correctly)
