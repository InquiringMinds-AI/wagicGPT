# Wave-34 thinking-trace audit — batch 3 (auditor 3 of 6)

Log-reading only. Seven translogs in `~/.Wagic/ai/gpt/logs/`, one seat each:

| tag | file | traces |
|---|---|---|
| G1 | `1787165713-ai_baka_deck152-0x56544b1b4b30-vs-ai_baka_deck36.jsonl` | 16 |
| G2 | `1787172913-ai_baka_deck105-0x56404f142cd0-vs-ai_baka_deck36.jsonl` | 11 |
| G3 | `1787177417-ai_baka_deck146-0x55fa86ae3040-vs-ai_baka_deck152.jsonl` | 21 |
| G4 | `1787184618-ai_baka_deck146-0x564f722c8f60-vs-ai_baka_deck158.jsonl` | 16 |
| G5 | `1787191818-ai_baka_deck105-0x5620dcdbe280-vs-ai_baka_deck139.jsonl` | 14 |
| G6 | `1787199017-ai_baka_deck139-0x55746a7aedc0-vs-ai_baka_deck152.jsonl` | 21 |
| G7 | `1787206218-ai_baka_deck105-0x5647b330f720-vs-ai_baka_deck152.jsonl` | 17 |

**116 reasoning-bearing traces** (123 records incl. 7 gamestart/gameend).
Kinds: ask 88, attackers 17, blockers 6, priority 5.
`reasoning_chars`: min 2,318 / p50 14,485 / p90 21,147 / max 33,370.
`reasoning_budget_hit`: **2/116 (1.7%)** — G1 seq13 (33,370) and G2 seq1 (23,497).
`post_answer_overrun` non-zero on 43/116; `post_plan_overrun` 0; `answer_replaced` 0; `commit_retracted` 0; `retry` 2 (both budget-hit traces).

---

## JOB 1 — Diagnostic-depth measurement

### Method

Stratified: full close-read of the 2 budget-hit traces, the 12 longest, and 22 keyword-flagged /
random traces (36 total). The remaining 80 were keyword-swept (`the prompt says`, contradiction,
confus, unclear, typo, `I cannot`, `wait,`, `actually`, `not listed`, `already in pool`,
`choose one mode`, `from exile`, …) and every flagged excerpt close-read in context.

Depth was then measured on all 116 by an instrument calibrated against the close reads:
walk the trace line by line; mark a line as a **novel development-diagnostic element** when it
matches a content-diagnostic signal (state reading / false belief / invented card fact / prompt
confusion / plan derivation / annotation misparse) **and** contributes ≥4 unseen content words
**and** its content-word shape has not been seen before. Reply-format/plan-line protocol lines are
excluded from the content class but their **first** occurrence is counted separately as a
category-(c) verbalized prompt confusion; depth = max(last novel content element, first protocol
verbalization). Everything after that offset is post-diagnostic churn.

Calibration note (direction of error): on hand-scored traces the instrument reads **deeper** than a
human scorer (e.g. G5 seq1: instrument 10,593, hand score ~3,760 — the oscillation quotes varied
instruction fragments that survive the novelty gate). Reported depths are therefore an **upper
bound**, and the derived budget recommendation is conservative (errs toward more budget).

### Results — absolute diagnostic depth (chars; /4 ≈ tokens)

| statistic | chars | ≈ tokens |
|---|---|---|
| p50 | **11,313** | **2,830** |
| p90 | **17,066** | **4,270** |
| max | **32,272** | **8,070** |
| mean | 11,284 | 2,820 |

By kind (p50 / p90, chars):

| kind | n | p50 | p90 |
|---|---|---|---|
| ask | 88 | 10,550 | 17,066 |
| attackers | 17 | 11,494 | 17,888 |
| blockers | 6 | 11,387 | 15,822 |
| **priority** | 5 | **16,687** | **19,898** |

`priority` is the deepest kind — see finding **D2** (non-main-phase priority menus give no reason
for the absence of casts/land drops, so the model re-derives the phase rules from scratch).

### Results — post-diagnostic churn

- **mean 0.23**, median 0.20, p90 0.52.
- Corpus-wide: 1,681,944 reasoning chars total, 1,309,020 pre-depth → **22% of all reasoning chars
  are post-diagnostic churn** (upper-bound instrument; true figure is higher).
- 23/116 traces (20%) are >40% churn.

### Reply-protocol churn (measured separately, subset of the above)

Lines deliberating the reply format / `PLAN:` line rule:

- **mean 15%** of every trace's chars; p50 12%, p90 29%, **max 61%**.
- 24/116 traces spend >20% of the reasoning channel on it.
- Median position of the *first* protocol mention is at **35% depth** — it is a tail phenomenon,
  so it sits squarely in the region a budget cut would remove.
- **89/116 traces** mention the plan-line rule ≥3 times.

### Supported budget

Current budget is **≈8,192 tokens** (the two hits truncate at 33,370 chars of English prose
≈ 4.0 chars/tok and 23,497 chars of degenerate repetition ≈ 2.9 chars/tok — both ≈8.1–8.3k tok).

- p90 diagnostic depth is **4,270 tokens**; max is **8,070 tokens**.
- **Keep 8,192 for development.** It is hit 1.7% of the time and the p90 sits at 52% of it.
- **Do not cut below ~6,000 tokens.** ~10% of traces carry novel diagnostic content past 17k chars,
  and the single richest finding in this batch (**F1**, the Katilda mana-source resolution) landed
  at char 33,050 of 33,370 — i.e. the current cap already truncated a diagnostic mid-sentence.
- A raise to **10,000 tokens** would recover both truncated tails for ~+2% corpus cost, and is
  worth it only if late-resolution findings of the F1 class are considered high-yield. Neutral
  recommendation: hold at 8,192.
- Churn is **not** a budget problem to solve by cutting the budget — 22% of it is protocol
  oscillation with a known cause (**P1**). Fixing P1 buys back more than any cut, without cost.

---

## JOB 2 — Development findings

### P — Prompt / reply-protocol defects (highest yield; cheapest fixes)

**P1. The `PLAN:` line rule is undecidable at the first decision, and costs whole traces.**
The protocol says *"LINE 2, ONLY IF your plan changed"* + *"If your plan is unchanged, OMIT the
PLAN line entirely — your last stated plan carries forward automatically."* At the pregame /
mulligan decision there is no prior plan, so neither branch applies, and the model cannot exit.

> `g5_001_ask` (G5 seq1, turn 0, 16,409 chars) —
> `[2770]` *"Plan: Not strictly necessary to write a PLAN line unless the plan *changed*. Since this is the pre-game decision, there is no prior plan."*
> `[7163]` *"If this is the first turn, there is no plan. So technically it's not 'unchanged'. It's 'new'."*
> `[11242]` *"**Wait, I'll stop oscillating.** The instruction is 'ONLY IF your plan changed'. Since there is no prior plan, I can't claim it changed."*
> …then oscillates for another **5,167 chars**, alternating `"Okay, I'll omit it."` / `"No, I'll write it."` twenty times, and terminates only at the token budget. **77–83% of the trace is this loop.** Final reply: `CHOICE: 1 (Keep this hand)` with no plan — so the whole first game is played with an empty carried-forward plan.

Prevalence: 62 hits of the rule text in that one trace; **89/116 traces** re-read the rule ≥3×;
protocol deliberation is **15% of all reasoning chars corpus-wide**, max 61%.
**Fix:** make the first/pregame decision *require* a `PLAN:` line, or restate the rule as
"state a PLAN line whenever you have no plan or your plan changed; otherwise omit it."

**P2. Degenerate repetition loop on the mulligan prompt (total budget loss).**

> `g2_001_ask` (G2 seq1, turn 0, 23,497 chars, budget_hit) — the entire reasoning field is
> `"[Analysis of the Study] (9),\n[Review of the Study] (9),"` repeated **~415 times**, opening
> with mojibake: `"Your Openings (1) [2�1) [\n2�6],\nMulligan (3),\n[Cast: Example Card] (4),"`.
> Diagnostic depth **0**; churn **100%**. The reply itself was well-formed
> (`CHOICE: 1 (Keep this hand)` + a sensible PLAN), so the failure is invisible to any
> answer-level metric.

Two of the seven seats' first decisions (G2 seq1, G5 seq1) are pathological — the mulligan prompt
shape is the weak point. Note the U+FFFD replacement chars: the reasoning field captured corrupted
bytes; worth checking the reasoning capture path for an encoding step.

**P3. Cast options carry no target slot, but list legal targets — the model never knows whether
picking the option also picks the target.** Three separate re-raises inside one trace:

> `g4_010_ask` `[13149]` — *"I need to check if I should target the Army or Bowmasters. The prompt doesn't ask me to choose the target *here*, it just lists the choice. **I will assume the game will ask for the target next**, or I just cast it. Given the Turn 5 log, there was a specific 'TARGET CHOICE' step."*
> `g3_017_ask` `[14873]` — *"I need to specify the target? … It doesn't explicitly ask me to select the target in the *format*, but the option description says 'legal targets right now: Wolf, Goblin'."*

**Fix:** one clause on cast options that list targets — "the target is asked separately after this
choice" — removes ~1.5k chars/trace on every targeted cast.

**P4. The `CHOICE:` example is cast-shaped on every decision kind.** Every prompt, including
`TARGET CHOICE` and mode menus, ends with `e.g. "CHOICE: 3 (Cast Example Card)"`. On a target
choice the instruction body says *"answer with the chosen TARGET's name (not 'Vanishing Verse')"*
while the example shows a cast — the model re-derives the resolution each time
(`g4_011_ask [19584]`: *"I should copy the name part. 'Orcish Bowmasters (1/1)' seems to be the name."*).

**P5. Option labels embed full `{card text: "…"}` — the "exactly as listed" echo rule then costs
thousands of chars.**

> `g5_009_ask` `[22561]`–`[25398]` and again `[27961]` — the model writes the 190-char option label
> out three times, then: *"That's long. I'll try to capture the essence but ensure it matches the
> start."* → **~3,000 chars** of echo agonizing on a single cast.

**Fix:** either drop the `(name)` echo requirement, or state that the number is authoritative and a
short name prefix suffices.

**P6. `Write your plan FROM the list` reads as a *ban* on naming future cards in the PLAN.**

> `g4_005_ask` `[10816]`–`[13283]` — *"This instruction is tricky… Does this mean I cannot write 'Cast Vanishing Verse' in my PLAN line if 'Cast Vanishing Verse' is not in the *current* choice list (which only has land drop options)?"* … *"If I interpret this strictly, I cannot name 'Vanishing Verse' in the plan."* — ~2,500 chars, unresolved; the plan is degraded as a result.

---

### D — Render / board-snapshot defects

**D1. `Mana available: N total` cannot be reconciled with the board because untapped permanents
carry NO annotation while tapped ones do.** The asymmetry makes absence-of-tag ambiguous.

> `g4_014_ask` `[1230]` — *"Looking at the board list: 'Shineshadow Snarl; Plains [tapped]; Swamp [tapped]'. **This contradicts 'Mana available: 1 total'.**"* (It does not — Snarl is untapped — but nothing on the line says so.)
> `g4_011_ask` `[219]` — *"I have 1 total untapped source available (actually, looking at the board: Shineshadow Snarl, Plains [tapped], Swamp [tapped]. **Wait**, the prompt says 'Mana available: 1 total (one untapped source…)' but lists Shineshadow Snarl, Plains [tapped], Swamp [tapped]."*
> `g3_011_ask` `[1578]` — *"Is it untapped? The list says 'Swamp #2 [tapped]', 'Plains [tapped]'. It doesn't explicitly mark Swamp #1 as tapped."*

Five traces spend chars trying to reconcile the mana line with the land list.
**Fix:** tag untapped lands explicitly, or name the untapped sources in the mana line.

**D2. Creature mana sources are invisible — they appear in the mana total but not in any land
list. This is what produced the deepest trace in the batch.**

> `g1_013_ask` (33,370 chars, **budget_hit**, the deepest trace). The model spends the whole trace
> unable to find a 4th mana source, inventing one at `[1323]` (*"So I have 4 lands total on
> battlefield: Barkchannel, Branchloft, Deserted Beach, Hengegate"* — Hengegate is in HAND), and only
> resolves it in the **last 320 chars before truncation**:
> `[32915]` *"Existing on battlefield: Barkchannel, Branchloft, Deserted Beach. (3 lands). Plus Katilda (Creature). Wait, 'Mana available: 4 total'. **Where does the 4th come from? Maybe Katilda taps for mana?** … Katilda is a Human (1/1). She is untapped. So she can produce {G} or {W}. So I have 3 Lands + 1 Katilda = 4 sources. So 'Mana available: 4 total' includes Katilda's ability`  ← *sentence truncated by the budget.*
>
> **Consequence — answer/derivation drift.** After 30k chars concluding *"removing Cathodion is
> strictly better for survival"* `[23904]`, the emitted reply says:
> `PLAN: Play Hengegate Pathway, cast Ranger Class and Fateful Absence **to remove Steel Wall**.`
> The plan contradicts the derivation. This is a budget-relevant pathology: at extreme depth the
> committed answer detaches from the reasoning.

The same question is answered in **340 chars** in `g1_012_ask [1267]` when the model happens to
recall Katilda's text early — so the fix is pure render: list creature mana sources on the mana line.

**D3. Poison counters — the infect deck's WIN CONDITION — are absent from the board snapshot.**
Verified: `poison` does not appear anywhere in the CURRENT SITUATION block or the system prompt of
any deck105 trace; the only hits are inside card text. The model rebuilds the count from narration
every single turn.

> `g7_014_priority` `[322]` — *"**The log doesn't explicitly show poison counters accumulated yet**, but my deck uses Infect. The strategy guide emphasizes counting poison. Let's trace the damage."*
> `g7_009_ask` `[150]` — *"Poison Counters: Not explicitly mentioned in the log so far, but my deck is Infect. **I need to track this.**"*
> `g2_007_ask` `[258]`, `[810]` — same reconstruction, again.

41/116 traces reason about poison. Per the trust doctrine, a silent omission is worse than wrong
text. **Fix: render `Poison: you N | opponent N` (and any other alt-win counter) in the situation
block.** Highest-value single render change in this batch.

**D4. `| Already in pool: {b}{w}` on a `TARGET CHOICE` decision is a true statement in the wrong
scope, and reads as a contradiction.** Confirmed in the raw prompt (G4 seq11):
`Mana available: 1 total (one untapped source…) | Already in pool: {b}{w}`.

> `g4_011_ask` `[3746]`–`[3862]` — *"The prompt says 'Mana available: 1 total (one untapped source…)' but then says 'Already in pool: {b}{w}'. **This is contradictory.** Usually 'Mana available' means untapped sources. 'Already in pool' means mana already generated."* … `[17375]` *"Where did the other mana come from? Maybe I tapped them earlier in the turn? Regardless, the prompt says I have the mana."*
> `g3_018_ask` `[1025]` — *"This is contradictory. If I just cast Vanishing Verse ({b}{w}), I would have tapped lands to pay for it."* … `[10139]` *"This is a contradiction in the prompt's provided text (Log says it resolved, Situation says it needs a target)."*

**Fix:** on TARGET CHOICE / on-the-stack decisions, suppress the mana line or replace it with
"cost already paid".

**D5. `Choose one mode for <card>:` is used for a *name-a-card* choice, and the list is bare names
with no context.** Confirmed in the raw prompt (G3 seq12): `Choose one mode for Silverquill
Silencer:` followed by 18 bare names (incl. `Clue`, a token).

> `g3_012_ask` `[11316]` — *"Let's consider the 'mode' phrasing. 'Choose one mode for Silverquill Silencer'. **This is slightly confusing wording. Silencer doesn't have modes.** It has an ability to name a card."*
> `g3_012_ask` `[4398]` — *"The list provided seems to be a selection of cards from the opponent's known deck list (**some might be typos or cards not actually in the provided list** like Pithing Needle, Sunset Revelry, Negate, Disdainful Stroke)."* ← invented "typo" belief from an unexplained list source.
> `g3_016_ask` `[739]` (next turn, reading its own narration) — *"the log says … 'Choose one mode for Silverquill Silencer: -> Fateful Absence'. **This seems like a simulation error in the log**"* — i.e. the emitter's own narration echo is read as an engine bug.
> `g4_013_ask` `[17956]` — misparse: *"The prompt asks to answer with the chosen **TARGET's** name for the **Vanishing Verse** decision"* — the mode menu got conflated with a target choice.

**Fix:** render the actual question ("As Silverquill Silencer enters, choose a nonland card name")
and say where the list comes from.

**D6. Target lists do not mark ownership — the model calls a legal own-creature target a typo.**

> `g1_009_ask` `[5091]` — *"Targets: Cathodion, Steel Wall, Scarecrone. (**Wolf is mine, why target Wolf? Probably typo in options list**, but I wouldn't target my Wolf)."*
> (Contrast `g3_016`/`g3_018`, where target options DO carry `[your battlefield]` / `[opponent's battlefield]` and the model reads them cleanly. The tag exists on the standalone TARGET CHOICE emitter but not on the inline `- legal targets right now: …` cast-option form.)

**D7. Land options don't state enters-tapped status; the model invents a rule.**

> `g1_016_ask` `[4572]` — *"the new one enters tapped? No, Hengegate Pathway is a basic dual land type usually entering untapped unless specified"* — an invented rule, later corrected at `[13203]` by re-reading the card text.
> `g6_006_ask` `[5345]` — shockland ordering: *"So the game is asking me to resolve the 'Shockland' decision for Sacred Foundry *after* it moved to battlefield? **That's odd mechanics-wise** (usually you decide before it enters)"* — the pay-2-life menu arrives after the land already renders as `[tapped]` on the battlefield. `[11667]`: *"the 'Sacred Foundry [tapped]' in the battlefield summary is a placeholder/error in the log display before the decision."*

**D8. Split / MDFC card costs render as one cost.**

> `g6_018_ask` `[3528]` — *"The prompt says 'Beanstalk Giant // Fertile Footsteps {6}{g}'. **This notation is slightly ambiguous** … (where each side lists its cost)"* … `[14999]` *"If the prompt only lists {6}{G}, **it might be a simplification or error in the prompt's representation of the card**."* Cost the model the Fertile Footsteps line entirely.

**D9. `[from exile]` cast tag is unexplained.**

> `g6_015_ask` `[3192]` — *"**Why is there an option to cast Pollywog Symbiote from exile?**"* (~1,500 chars to move past it). `[16229]`: it then declares a hand line *"likely an error in the prompt generation (copy-paste artifact)"*.

**D10. Infect combat damage narrates the number without the source of the pump.**

> `g7_014_priority` `[506]` — *"The log says 'Infect damage: 3 from Ichorclaw Myr to Intrepid Adversary'. **Wait, Ichorclaw Myr is a 1/1. Why did it deal 3 damage?**"* — resolved only by cross-reading the `-1/-1` counter events (Ichorclaw Myr's blocked +2/+2 trigger is never narrated).

**D11. Narration double-entry read as an engine bug.**

> `g3_019_attackers` `[4878]` — *"THEN 'TARGET CHOICE for Vanishing Verse… Wolf'. This implies a *second* Vanishing Verse was cast? **Or is this an error in the provided log simulation?**"* (the cast and the later on-stack target choice both narrate).

**Working well (do not regress):** `[tapped - cannot attack or block this turn]`,
`[summoning sick - cannot attack this turn, but CAN block]`, `(printed 0/0) [counters: 2x +1/+1]`,
`[deals 0 - this attack deals no damage to the opponent]` (`g6_016 [4789]` read it exactly right),
the MDFC `Flip Side [DISPLAY TOGGLE only …]` block, and the dungeon emitter with the full room path
(`g3_008` picked Lost Mine correctly and fast off the rendered path).

---

### G — Guide-vs-render mismatches (guide teaches a decision the engine never asks)

**G1. `Play a land if a 'Play <land>' option is listed` — the cast menu and the priority menu never
list one, and 11 traces stall on it.**

> `g7_007_ask` `[3293]`, `[5136]`, `[9316]`, `[9720]`, `[13612]` — five separate re-raises in one trace: *"There is no explicit 'Play Land' option in the numbered list… Since it's not listed, I assume the system handles the land drop separately"* → *"**Since I cannot play a land via the menu**, I will just cast the creature. The game likely handles the land drop implicitly or **I missed the option**."*
> `g7_011_priority` `[8365]` — *"'Play a land if a 'Play <land>' option is listed.' It is *not* listed in the 'Legal actions' section provided in the prompt text. **This is strange.** Usually the game engine lists 'Play Land' if available."* … `[9256]` *"**Or maybe the prompt generation missed it?**"* … `[15291]` *"The 'Legal Actions' do not list 'Play Forest'. **This is weird.**"* — ~4,000 chars in one trace.
> `g5_009_ask` `[2683]`, `[5312]`, `[22025]` — three re-raises.

The engine does issue a separate land-drop decision (`Play Forest / Hold Forest`), so this is purely
a **sequencing surprise**: the guide implies land and cast are the same menu. **Fix:** teach
"the land drop is asked as its own decision" in the guide, or render a `(land drop already
offered / already taken this turn)` marker on the cast menu.

**G2. Shockland "pay 2 life vs tap" is taught as an answer to give, but the land-drop menu only
offers `Play Breeding Pool`.**

> `g6_002_ask` — quotes the guide's *"ANSWER 'pay 2 life' ONLY when you will use that mana THIS TURN"* **thirteen times** across 21,857 chars. `[11128]` *"Since this is a 'Land drop' question, usually the game engine asks 'Play Breeding Pool (pay 2 life) / Play Breeding Pool (enters tapped)'."* `[11497]` *"**It does not give the 'Pay 2 life vs Enter Tapped' sub-choice here.** It assumes I will play it."* `[14157]` *"I will assume selecting this implies I will play it. The game will likely ask 'Pay 2 life?' next or I just select it."* `[20015]` — same assumption a fourth time.
> Churn on this trace: 32%. (The sub-choice IS asked separately — `g6_006_ask` — but the guide never says so.)

**G3. `Cast the biggest infect creature the Cast list offers` — "biggest" is undefined and drives a
~10,000-char oscillation.**

> `g5_009_ask` `[11277]` *"Which is 'biggest'? Power is usually the metric. 3 > 2."* → `[15885]` *"Does 'biggest' mean Power? 3 > 2. Does it mean Cost? 4 > 3."* → `[26571]`–`[27801]`, still unresolved; the model finally **overrides the guide** on its own Magic judgment (2/2 over 3/1). Guide teach effectively lost.
> **Fix:** define the tiebreak explicitly (highest power; toughness as tiebreak) or the guide is dead weight.

**G4. The guide's "trust the list" clause collides with the model's own (wrong) mana arithmetic and
costs 9,400 chars.**

> `g5_009_ask` `[2052]` — invented card fact: *"Cost: 5 mana (2 generic + 2 green). Mana Check: I have 4 lands. **I cannot pay 5 mana. Status: Illegal/Unpayable.**"* (`{2}{G}{G}` is 4.)
> `[9260]` *"**This is a contradiction in the prompt setup vs the explicit constraint.**"*
> `[8563]` *"If the game says it's payable, I must treat it as payable. Perhaps there is a mana ability I missed… **or perhaps the prompt generation has an inconsistency** but I must follow the rule."*
> Self-corrects only at `[14608]`–`[15071]`: *"{2}{G}{G} = 2 Generic + 1 Green + 1 Green = 4 Total Mana. **My mistake earlier was thinking {2} meant 3 or something.**"*
> The trust clause held (good — the model deferred to the list) but the model still burned a third
> of its budget re-litigating. **Consider rendering the numeric total alongside the symbols**
> (`{2}{g}{g} = 4 mana`) — the same class of error appears in `g4_011 [1605]`.

---

### T — Trace-level pathologies (budget-relevant structure)

| pathology | evidence | budget cost |
|---|---|---|
| **T1** Degenerate repetition loop | `g2_001` — 415× `[Analysis of the Study] (9),` + U+FFFD mojibake | 23,497 chars, 100% waste, budget hit |
| **T2** Protocol oscillation (P1) | `g5_001` — 20× `"Okay, I'll omit it."`, self-aware at `[11242]` | 13,639 chars, 83% of trace |
| **T3** Answer/derivation drift at extreme depth | `g1_013` — reasoning concludes Cathodion, PLAN says Steel Wall | correctness, not just budget |
| **T4** Budget truncation mid-diagnostic | `g1_013 [33315]` cut mid-sentence exactly at the resolution | loses the highest-value finding |
| **T5** Re-verification loops on the mana line | 5 traces, D1/D2/D4 | ~2–4k chars each |
| **T6** Echo-format loops on long option labels | `g5_009` P5 | ~3k chars |
| **T7** `post_answer_overrun` on 43/116 (max 211 chars) | model keeps thinking after the answer is committed | small, but confirms the answer is emitted before the reasoning terminates — relevant to T3 |

---

## NEW emitters / shapes observed (beyond the OLD register: zone arrows, question echoes,
## resolved-vs-countered ambiguity, counter-source gaps, dungeon room position)

1. `| Already in pool: {b}{w}` appended to the mana line — appears on TARGET CHOICE decisions (D4). 3 traces.
2. `Choose one mode for <card>:` used for a name-a-card ETB choice, bare-name list, no source stated (D5). 8 traces.
3. `TARGET CHOICE for X (this spell/ability is already on the stack and needs a target - it is NOT a cast or phase step)` paired with a cast-shaped `CHOICE:` example (P4).
4. `(note: the actions your plan names are no longer among the options available right now - the game state has advanced past that plan; re-derive your choice from the current board and the options below.)` — fires on mode menus where it is trivially true; adds noise.
5. `Flip Side [DISPLAY TOGGLE only - this is a modal double-faced land … in this engine only the currently-shown face can actually be played as a land.]` — long, but read correctly.
6. `[deals 0 - this attack deals no damage to the opponent]` on attacker options — read correctly.
7. `(printed 0/0) [counters: 2x +1/+1]` on token creatures — read correctly.
8. `[from exile]` cast-source tag with no explanation of why (D9).
9. `Cast X - legal targets right now: A, B {card text: "…"}` — the inline target list *without* `[your/opponent's battlefield]` tags (D6) and without a target slot in the reply (P3).
10. Shockland menu `pay 2 life [this permanent then enters the battlefield UNTAPPED - usable…]` / `tap [decline the payment; … enters TAPPED - unusable until your next untap step]` — good wording, wrong ordering vs the battlefield render (D7).
11. `Beanstalk Giant // Fertile Footsteps {6}{g}` — split card, one cost (D8).
12. `Your legal actions (Upkeep, YOUR turn): 1. Regenerate with Blight Mamba [cost: {1}{g}]` — priority menu at a non-main phase, no marker explaining why casts/land drops are absent (D2 / G1); drives the deepest-by-kind traces.

---

## Ranked fix list (development value per unit of work)

1. **D3** — render poison / alt-win counters in the situation block. *(win condition currently invisible)*
2. **P1** — make the pregame `PLAN:` line mandatory (kills T2 and a share of the corpus-wide 15% protocol churn).
3. **D1 + D2** — annotate untapped sources and list creature mana sources on the mana line. *(kills the deepest trace class)*
4. **D4** — suppress / rescope the mana line on TARGET CHOICE decisions.
5. **G1 + G2** — guide edits: state that the land drop and the shockland payment are their OWN decisions.
6. **P3 + P4** — say the target is asked separately; make the `CHOICE:` example kind-appropriate.
7. **D5** — render the real Silencer question and the list's provenance.
8. **G3** — define "biggest".
9. **D6** — put `[your battlefield]` / `[opponent's battlefield]` on inline cast-option target lists.
10. **P5** — relax the "exactly as listed" echo rule for long labels.
11. **P2** — investigate the mulligan-prompt shape and the U+FFFD bytes in the reasoning capture path.
12. **D7–D11** — enters-tapped status on land options; split-card costs; `[from exile]` reason; narrate the infect pump; de-duplicate the cast/target narration.
