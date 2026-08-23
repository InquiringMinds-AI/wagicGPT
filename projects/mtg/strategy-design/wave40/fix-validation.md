# Wave-40 cross-cutting fix-validation — coverage fractions

**Seat**: cross-cutting fix-validation (completeness critic).
**Corpus**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-110407` — 42 seat files, 21 games,
binary `465422fcf`, pilot `qwen36-35b-a3b`. 2073 records (1433 `ask`, 348 `priority`,
135 `attackers`, 57 `blockers`, 42 `gamestart`, 42 `gameend`, 16 `reveal`), 7 fallbacks.
**Method**: every denominator found INDEPENDENTLY of the fixed string (see per-item "occasions
found by"). Narration counted over the per-record `events` DELTA stream, never over `prompt` —
the prompt carries the *cumulative* GAME LOG, so a raw `grep` over the .jsonl overcounts
narration by ~5-25x (`resolved and went to` raw-greps 7731 times; the true delta count is a
fraction of that). Render lines counted per emission over `prompt` / `options_text`.
**Seat-views**: both POVs of each game counted (each game contributes two seat files).

---

## Fraction table

| # | Fix | Occasions (denominator, found independently) | Emissions with the new string | Fraction | Verdict |
|---|-----|---|---|---|---|
| **#3a** | `was COUNTERED by <spell>` | 40 counterspell RESOLUTIONS (Essence Scatter 12, Dream Fracture 10, Fall of the Gavel 10, Cancel 8) | 40 | **40/40 = 100%** | **SHIPPED** |
| **#3b** | KILL metric: `resolved and went to` on a countered spell | 40 counter events | 0 leaks | **0** | **SHIPPED** |
| **#3c** | KILL metric (independent angle): permanent spell narrated `resolved and went to … graveyard` | 40 pool permanents scanned corpus-wide | 0 | **0** | **SHIPPED** |
| **#4a** | Stack SPELL entries carrying cost + type + (P/T) | 109 spell entry lines (55 distinct) in 266 `ON THE STACK` prompt blocks | 108 | **108/109 = 99.1%** | **SHIPPED** (1 miss, see F1) |
| **#4b** | `can target on the stack:` clauses carrying cost + type + (P/T) | 39 clauses (36 distinct) | 39 | **39/39 = 100%** | **SHIPPED** |
| **#5a** | Wall-block `deals 0` annotation carrying a STOPS clause | 89 blocker-side `[deals 0 …]` annotations | 89 (32 numeric-N + 44 multi-attacker alternate + 13 trampler alternate) | **89/89 = 100%** | **SHIPPED** |
| **#5b** | N is TRUE (attacker power vs printed N) | 6-sample audit of numeric-N emissions | 6 correct | **6/6** | **TRUE** |
| **#5c** | `no blockers` chosen with a `(neither dies)` line offered | 35 blocker windows offering a free block | 0 declined | **0/35** (baseline 2) | **HARM GONE** |
| **#6a** | Unreachable-attacker reason tag | 12 attacker lines with no B-line able to block them | 12 | **12/12 = 100%** | **SHIPPED** |
| **#6b** | `all_assignments_illegal` | corpus-wide | 0 | **0** (was 1) | **CONFIRMED** |
| **#8** | Back-to-back duplicate cast ask, second showing POST-payment mana | 697 cast-decision asks; 12 consecutive-seq identical-menu pairs examined | 0 fit the defect | **0** (baseline 4 pairs/game on X-decks) | **SHIPPED** |
| **#9** | `Add N green mana with Overgrown Battlement`, N = defender count | 57 Battlement activation labels | 57 with N == defenders on the controller's battlefield at emission | **57/57 = 100%** | **SHIPPED** |
| **#10** | `{right now: draws 0}` | corpus-wide | 0 occurrences (prompt AND narration) | **0** | **CONFIRMED GONE** |
| **#16** | Bogus winner rows in `results.tsv` | 21 rows | 21 natural (every loser at ≤0 life, no `adj`/crash row) | **0 bogus** | **CONFIRMED** |
| **#19** | `put a card into their library` masked lines — **BASELINE for wave-41** | — | **47** masked lines, 6 seat files / 6 games | see F2 | **UNFIXED (expected)** |
| **#7** | Mutated-pile quoted rules text — **BASELINE for wave-41** | 20 mutate-pile battlefield option lines carrying quoted text | 14 quote the WRONG card's text | **14/20 = 70% wrong** | **UNFIXED (expected)** |

### Sample seqs (for re-check)

- **#3** end-to-end, both POVs, one counter:
  `1787501050-…deck125-0x56538c749520-vs-deck139` **seq 16** chose
  `Cast Fall of the Gavel {3}{u}{w} - can target on the stack: Beanstalk Giant // Fertile Footsteps {6}{g} (creature 7/7)`
  → **seq 17** events: `- You cast Fall of the Gavel` / `- Opponent's Beanstalk Giant // Fertile Footsteps was COUNTERED by Fall of the Gavel and went to the opponent's graveyard` / `- You gained 5 life (now 30)` / `- Your Fall of the Gavel resolved and went to your graveyard`.
  Victim seat `…deck139-0x56538b427e80-vs-deck125` **seq 20**:
  `- Your Beanstalk Giant // Fertile Footsteps was COUNTERED by Fall of the Gavel and went to your graveyard`.
  The counterer is NAMED on both sides — #3's decision-relevant ask satisfied.
- **#4**: `…deck125-0x56538c749520…` seq 16/17 stack block —
  `1 (top): opponent's Beanstalk Giant // Fertile Footsteps {6}{g} (creature 7/7) [spell]`.
- **#5**: `…deck126-0x55fef64cab40…` **seq 32** — `A1. Fate Unraveler (3/4) deals 3`, five B-lines each
  `[deals 0 - this block kills nothing, but it STOPS all 3 damage from reaching you]`. N == A1's power.
  Multi-attacker alternate at `…deck126-0x55ee54004c60…` seq 22 (two blockable attackers → the
  "each A-line above says how much" wording, correctly declining to invent one N).
- **#6**: `…deck126-0x55ee54004c60…` seq 15 —
  `A2. Sigarda, Champion of Light (4/4) deals 4 [trample, flying] [NONE of your available blockers can block this attacker - it has flying and none of your available blockers has flying or reach]`.
  All 12 occasions carry this one reason string (flying-vs-no-flier/reacher); no other evasion
  class occurred in the corpus, so the register is **unexercised** for menace / protection /
  "can't be blocked by walls" — an untested branch, not a passing one.
- **#9**: `…deck126-0x55fef64cab40…` seq 17 `Add 6 green mana …` with 6 defenders; seq 45 `Add 7 …`
  with 7; `…deck126-0x562623199ca0…` seq 8 `Add 1 …` with 1 defender. Ramp tracks the count
  monotonically across three games.
- **#8**: the 12 consecutive-seq identical-menu cast pairs all resolve to legitimate windows —
  8 have intervening `- You cast <X>` + resolution events (a genuine second window for a second
  copy; mana correctly re-read POST-payment for that new window), and 4 are response windows
  opened by opponent action with **identical** `Mana available:` before and after
  (`…deck123-0x55bfdbdae220…` 18/19 at 6/6; `…deck125-0x55c3ff4e5050…` 17/18 at 1/1;
  `…deck123-0x5634600b4c10…` 20/21 at 2/2 and 86/87 at 10/10). Zero unanswered-window re-asks.
  X-announce flow is clean: `…deck125-0x56377c85eb60…` seq 23 cast ask → seq 24 `X = 3` menu →
  seq 25 next window; no duplicate.

---

## Findings

### F1 — [LOW, NEW] A daybound "Day" marker renders as a `[spell]` on the stack with no card facts
`…deck152-0x55ee52cc5ca0-vs-deck126` **seq 16**:
`ON THE STACK … 1 (top): your Day [spell]` — emitted right after `- You cast Brutal Cathar`
(daybound). This is the **only** one of 109 stack spell entries missing cost/type/P/T, and it is
missing them because it is not a card: the day/night state marker is being pushed onto the stack
and typed `[spell]`. It is a small trust-doctrine violation (the pilot is told a spell it can
respond to is on the stack) and it is the reason #4 reads 99.1% rather than 100%. n=1 this
corpus; deck152 is the only daybound deck in the pool.

### F2 — [MEDIUM-HIGH, NEW — sharpens #19] The mask is **observer-scoped**, not origin-scoped, and NO opponent activation is ever narrated
#19 was written as "the zone-change narrator masks card identity regardless of ORIGIN zone." The
corpus says the origin logic is already right on the **actor's own seat** and wrong only on the
**observer's**. Same game, same events, two seat files:

| seat `…deck125-0x56345ed9bfa0-vs-deck123` (actor) | seat `…deck123-0x5634600b4c10-vs-deck125` (observer) |
|---|---|
| `- You used: Life with Elixir of Immortality` | *(no line)* |
| `- You gained 5 life (now 25)` | `- Opponent gained 5 life (now 25)` |
| `- Your Elixir of Immortality was put into your library` | `- Opponent put a card into their library` |
| `- Your Dream Fracture moved from your graveyard to your library` (×N, each named) | `- Opponent put a card into their library` (×N) |

**The generalisation is much bigger than Elixir**: `- You used: …` appears **165 times**
corpus-wide; `- Opponent used: …` appears **0 times**. Every activated ability, every
planeswalker loyalty ability, is invisible on the opposing seat — the observer sees only the
effect (`- Opponent's Kaya the Inexorable got a loyalty counter`, 50 such lines;
`- Opponent's Lolth, Spider Queen …`, 57) with no cause line. Verified side-by-side:
`…deck146-0x55ff68c7f060…` seq 30 `- You used: +1: don't target any creature with Kaya the
Inexorable` vs its pair `…deck126-0x55ff69fb4510…` seq 25 `- Opponent's Kaya the Inexorable got a
loyalty counter`.

**Wave-41 baselines**: 47 masked `put a card into their library` lines (+1 `…into their hand`,
also a public graveyard→hand move); 165 actor-side activations with 0 observer-side echoes.
This corpus's masked count is much lower than wave-39's 93-in-one-game because deck125 activated
Elixir far less; the **structural** number to fix against is the 165/0 activation asymmetry, not
the 47.

### F3 — [LOW, NEW] 25 no-op `moved from the opponent's zone to the opponent's zone` lines
All 25 are `Lost Mine of Phandelver` (deck146 dungeon venturing), e.g.
`…deck125-0x55b11e871510…` seqs 7/15/18/26. A zone change that begins and ends in the same zone
is narrated as a move; it is pure noise in a log the pilot is instructed to trust, and it sits
next to the (correct) `- Opponent ventured into Lost Mine of Phandelver: venture step 1 of that
run` line that already says the real thing. Cheap suppression: skip the narration when
origin == destination.

### F4 — [INFO, checked-clean] `Mana available: 0` at the X-announce ask is NOT a lie
All 10 X-announce asks render `Mana available: 0 total`. Checked before reporting: the same line
carries `| Already in pool: {u}{u}{w}{w}{w}{w} (6 mana ALREADY produced and floating right now -
this is SEPARATE from the 0 untapped sources counted above…)`. The zero is literally true (every
source is tapped) and the reconciliation clause is present. **No defect.** Noted because it is
the shape a future seat will mis-flag, and because it does mean a guide rule keyed to the bare
string `Mana available:` still reads 0 at that window.

### F5 — [expected] #7 mutate-pile wrong-text is still live, now quantified
14 of 20 mutate-pile battlefield option lines quote another card's rules text (70%). Fresh
repros beyond the ledger's two: `Everquill Phoenix (4/4) [flying, mutate] [your battlefield] -
"You may play an additional land…"` (Dryad's), `Gemrazer #1 (4/4) [trample, reach, mutate] -
"Each creature spell you cast costs {1} less…"` (Pollywog's), `Snapdax, Apex of the Hunt (3/5)
[double strike, reach, mutate] - "Reach -- When Arboreal Grazer enters…"` (Grazer's). It now
also reaches the OPPONENT's board (`…deck152-0x557fe9f525a0…` seq 35 renders two of deck139's
piles with swapped texts), so it mis-informs targeting decisions on both sides, not just #139's
own. Battlefield-summary lines are clean (they quote no text); the defect is confined to
option/target lines.

### F6 — [HOUSEKEEPING] Both registered guide stopgaps have already been re-keyed
- deck162 Rule 4: the retirement condition in `wave39/deck162/findings.md` §10
  (`grep -c 'STOPS all'` non-empty) **FIRES** — 89 emissions. The guide has already been
  rewritten to quote the new engine string (`deck162_strategy.txt` L74-75), so the interim is now
  a *dependency* on the register rather than a stopgap. Synthesis seat's call whether to delete
  L67/L74-75 or keep them as reinforcement; not acted on here.
- deck126 V1: `deck126_strategy.txt` no longer contains the false `Add 1 green mana` anchor —
  grep returns nothing. The #9 guide/engine sequencing hazard is discharged.

### F7 — What no seat looked at
- **Silverquill Command is a latent counter and was never used as one.** 8 resolutions this
  corpus, every one choosing non-counter modes (`return creature and you draw`,
  `creature gains 3/3 and you draw`). It is deck146's only counter-capable card; the #3 register
  is therefore **unvalidated against a modal counter** — if the counter mode is even reachable
  (cf. ledger #21 alias-hardcode counters), it may not raise `WEventSpellCountered`. Worth one
  targeted check in wave-41 before declaring #3 closed for the whole pool.
- **#6's register is single-branch.** All 12 occasions were flying-vs-no-reach. Menace,
  protection, shroud/hexproof-from-blockers and "can't be blocked by walls" never occurred.
- **Counters-spent-on-non-threats ratio (the #4 payoff metric, baseline 9/25) is not computed
  here** — deck125's seat owns that judgment call. The 40 counters this corpus are enumerated
  above by spell; the raw material is in the four deck125 seat files.
