# Wave-34 thinking-trace audit — BATCH 2

Auditor 2 of 6. Log-reading only. 8 files, **90 `kind:"ask"` records** (136 lines incl. gamestart/draw).
Register: OLD log register (rewrite already docketed). Reply protocol: NEW (answer + optional PLAN;
thinking captured in the `"reasoning"` field).

## Corpus shape

| metric | value |
|---|---|
| ask records | 90 |
| `reasoning_chars` p50 / p90 / max / mean | 13,535 / 20,738 / 28,344 / 13,683 |
| `reasoning_budget_hit: true` | **3 / 90** (3.3%) |
| `post_answer_overrun > 0` | 42 / 90 (median non-zero 145 ch, max 428) |
| `answer_replaced` / `commit_retracted` / `coded_answers>1` / `dropped_assignments` | 0 / 0 / 0 / 0 |

Budget-hit traces: `1787162603` seq3 (11,567 ch), `1787201714` seq15 (16,982), `1787208914` seq6 (17,298).
Note the budget hits fire *well below* the corpus max (28,344) — the cap is not a fixed char ceiling.

## Method

Stratified. Full close-read of **19 traces** hand-measured for diagnostic depth: all 3 budget-hit,
the 10 longest by `reasoning_chars`, plus randoms (seed 34). Corpus-wide keyword sweep (python over
jsonl) on `contradict|inconsisten|confus|unclear|ambiguous|typo|prompt is wrong|cantblock|not listed|
the prompt says|wait,|actually|I cannot`, with close-read of every flagged excerpt across all 90.

An automated novelty proxy for depth was built and **rejected**: it tracked short traces well
(±1% ) but over-read the long ones by up to 42% of trace length, because the churn segments restate
the same belief in *fresh wording* — lexical novelty is not semantic novelty, which is precisely the
pathology under measurement. Stats below are hand-measured only.

---

# JOB 1 — Diagnostic-depth measurement

"Depth" = character offset of the **last novel development-diagnostic element**: a stated reading of a
rendered line, a false belief in the model's own words, an invented card/rules claim, verbalized
prompt confusion, first derivation of the plan, or an annotation/echo misparse. Not counted:
re-verified arithmetic, recounting, option oscillation adding no new belief, board restatement.

| file | seq | total ch | depth ch | churn |
|---|---|---|---|---|
| 1787162603 | 1 | 12,204 | 6,800 | 0.44 |
| 1787162603 | 3 * | 11,567 | 6,000 | 0.48 |
| 1787162603 | 6 | 24,246 | 18,700 | 0.23 |
| 1787162603 | 7 | 3,666 | 3,666 | 0.00 |
| 1787162603 | 10 | 22,647 | 13,100 | 0.42 |
| 1787162603 | 13 | 4,656 | 4,656 | 0.00 |
| 1787162603 | 15 | 20,911 | 13,000 | 0.38 |
| 1787162603 | 16 | 17,283 | 11,600 | 0.33 |
| 1787162603 | 23 | 3,333 | 3,333 | 0.00 |
| 1787165717 | 4 | 12,968 | 6,500 | 0.50 |
| 1787165717 | 7 | 15,437 | 10,600 | 0.31 |
| 1787165717 | 9 | 22,222 | 13,000 | 0.41 |
| 1787165717 | 12 | 21,796 | 14,700 | 0.33 |
| 1787172917 | 1 | 5,165 | 2,800 | 0.46 |
| 1787172917 | 3 | 21,963 | 16,100 | 0.27 |
| 1787172917 | 10 | 19,887 | 19,500 | 0.02 |
| 1787180113 | 11 | 9,651 | 3,500 | 0.64 |
| 1787187313 | 13 | 18,952 | 17,200 | 0.09 |
| 1787194513 | 9 | 26,829 | 24,000 | 0.11 |

`*` = `reasoning_budget_hit: true`

**Absolute diagnostic depth (n=19, sample deliberately biased long):**

| | chars | ≈ tokens (/4) |
|---|---|---|
| p50 | **11,600** | 2,900 |
| p90 | **19,500** | 4,875 |
| max | **24,000** | 6,000 |
| mean | 10,987 | 2,747 |

**Mean post-diagnostic churn fraction: 0.285** (median 0.33; range 0.00–0.64).

Because the sample over-weights the longest traces (10 of 19 are top-10 by length), the true
corpus-wide p50 depth is lower than 11,600; these figures are a conservative *upper* estimate.

### Depth structure

Three regimes, cleanly separated:

1. **Clean traces (churn ≈ 0)** — 3.3k–4.7k ch, no format loop, correct render reading. Example:
   `1787162603` seq7 (3,666 ch) makes the same decision as seq6 (24,246 ch) on a nearly identical
   board — the difference is entirely whether the model reached a false belief about the mana line.
2. **Format-loop traces (churn 0.30–0.64)** — the decision is settled early; the tail is the
   PLAN-line / option-echo loop (see D1, D2). This is the modal shape.
3. **Genuinely deep traces (churn < 0.15)** — the model is *still producing new diagnostic content*
   at the last character, because a render defect is unresolvable from the prompt
   (`1787172917` seq10, `1787194513` seq9). These are the traces that justify a large budget.

### Trace-level pathologies (budget-relevant structure)

- **`"Wait,"` count: 1,456 across 89/90 traces (~16 per trace).** This is the churn signature.
- **`plan changed` / `OMIT the PLAN` re-reads: 661 across 77/90 traces.**
- **Verbatim repetition loop.** `1787162603` seq6: chars @22000–24246 are a *literal duplicate* of
  @21500–23000 — an identical 9-step block ("Wait, I need to check if I should play a fetchland… /
  cast Thoughtseize… / cast Bloodghast… / Final decision: Option 1.") emitted twice with no variation.
  Pure degenerate repetition; ~2,200 chars of zero-information output.
- **Oscillation without new belief.** `1787162603` seq10 spends @13000–22647 (9.6k ch, 42%) cycling
  four fixed sub-questions with no new content after the contradiction is stated once.
- **Overrun.** 42/90 replies carry `post_answer_overrun > 0` (median 145 ch). Cause is D2: the model
  copies the *entire* option string — annotations and embedded `{card text: "…"}` blob included —
  into the CHOICE parentheses. See `1787165717` seq7's reply, which echoes a 250-char option label.

---

# JOB 2 — Development findings

No seat review will run on this corpus, so these are the harvest. Verbatim quotes are from the
`"reasoning"` field unless marked PROMPT.

## Render / engine defects

### R1 — `Mana available: N total` is unattributable to a permanent, and contradicts the tapped-flags
**Severity: highest. The single largest churn source in the batch.** Appears in 5+ traces.
The mana line states a count but never names which permanent produces it, so whenever the model's own
count of untapped lands disagrees, it cannot resolve the disagreement and loops.

- `1787162603` seq10 @13000: *"Board: `Snow-Covered Swamp [tapped]`. If it's tapped, I have no mana.
  But the prompt says `Mana available: 1 total`. **This is a contradiction in the prompt's state
  description vs the available choices.**"* — then loops for the remaining 9,600 chars.
- `1787162603` seq6 @15000–20000: four successive false hypotheses about what the line means —
  *"Maybe it means 'You have 1 land in hand that can produce mana'?"* … *"might mean 'You can play a
  land and get 1 mana'"* … *"This might mean I have 1 mana already."*
- `1787201714` seq15 @2133: *"This seems to contradict the board state which has Ancient Den, Great
  Furnace, Swamp (in hand), Island (in hand)."*

**Fix:** name the sources — `Mana available: 2 ({B} Swamp #1, {B} Urborg)`.

### R2 — `Already in pool:` shows floating mana with zero untapped sources
Same family as R1, distinct emitter. Mana in pool should not survive to a fresh decision point.

- `1787162603` seq16 @914 (PROMPT): `Mana available: 0 total (no untapped sources) | Already in pool: {b}`
  → *"**This is strange.** … Bloodghast costs {B}{B}. I need to have paid for it."*
- `1787165717` seq11 @720 (PROMPT): `Mana available: 2 total (… colours you can make: {b}) | Already in pool: {r}{b}`
  → *"**This is contradictory.** … If I have {R}{B} in the pool, I have 2 mana. But the text says
  'colours you can make: {b}'."* and @7237: *"**Maybe the prompt is testing if I notice the
  inconsistency? Or maybe the 'Already in pool: {r}{b}' is a mistake in the prompt generation.**"*
  Note the pool shows `{r}` while the board's only untapped sources are Swamps.

### R3 — Self-contradictory creature annotation: `[cantblock]` + `but CAN block`
Raw internal token `cantblock` is emitted un-prettified, immediately followed by an annotation
asserting the opposite.

- `1787162603` seq15 (PROMPT): `Bloodghast {b}{b} (2/1) [cantblock] [summoning sick - cannot attack
  this turn, but CAN block]`
- The mirror side of the same game quotes it as a defect verbatim: *"There is a contradiction in the
  prompt's 'Current Situation' block … '[cantblock] [summoning sick - cannot attack this turn, but CAN
  block]'. **This is contradictory. 'cantblock' tag vs 'CAN block' text.**"*

**Fix:** suppress the "but CAN block" clause when the permanent has a can't-block restriction, and
render the token as `can't block`.

### R4 — Summoning-sickness annotation applied to OPPONENT permanents during the player's own turn
**Systematic across the whole corpus, not just this batch.** Blocking is never gated by summoning
sickness, and an opponent's creature cannot attack on your turn for reasons unrelated to sickness, so
the annotation is meaningless *and* misleading in this position. Corpus-wide sweep found it in every
game file checked, e.g.:

- `1787208914` seq6 @2649: *"'Hand of the Praetors … [summoning sick - cannot attack this turn, but
  CAN block]'. **This is contradictory to standard rules (it entered on Turn 7, it's Turn 8)**, but I
  must trust the 'CURRENT SITUATION' description provided by the game engine."*
- Same shape in `1787162603` seq6/7 (opponent Goblins), `1787165717` seq4/7 (opponent Dryad, Pollywog),
  `1787172917` seq3 (opponent Arboreal Grazer).

### R5 — `Artifacts in play: you N` disagrees with the rendered CDA power/toughness by exactly 1
`1787172917` seq10 (19,887 ch, **churn 0.02** — the model never resolves it and is still working on
it at the final character). Prompt states `Artifacts in play: you 7` while rendering
`Master of Etherium #1 {2}{u} (8/8) (printed 0/0)`.

- @18500: *"Total = 2 + 2 + 1 + 2 = 7. So Master should be 7/7. **Why does the log say 8/8? … This
  discrepancy is weird.**"*
- @19000: *"Strategy Guide says: 'The prompt prints "Artifacts in play: you N"… Never recount… if your
  count and N disagree, N is right'. So N=7. But Master is listed as (8/8)."*

The strategy guide independently documents the same off-by-one (*"Master is itself an artifact, so it
LANDS ONE BIGGER than the line says — cast at (8/8) and a 9/9 arrives"*), which is guide text papering
over an engine inconsistency. **Worth a direct engine check**: either the count line excludes a
permanent it should include, or the CDA is computed over a different set.

### R6 — Stack render emits the class name and an empty source
`1787180113` seq11 @991 (PROMPT): `1 (top): StackAbility. (Source: ) [triggered/activated ability]`
The model must reconstruct which ability is resolving from the game log:
*"There is an ability on the stack: '1 (top): StackAbility. (Source: ) …' Context: I just cast
'Orcish Bowmasters'."* Trivial fix, high value — this trace's churn is 0.64.

### R7 — Mutate pile: component mana cost shown against the pile's P/T
`1787194513` seq10 @7200: *"The log says 'Migratory Greathorn {1}{u} (3/4)'. … If Greathorn is on top,
it should be 3/4. **But why {1}{u}? Maybe it's a typo in the log** or I'm misreading the mutate
mechanic."* The top card's name and P/T are shown with a *different* component's mana cost.
Consequential: the model then derives colour from the pile to decide Vanishing Verse legality
(*"Mutated creatures are multicolored … Therefore, Vanishing Verse cannot target"*) — a targeting
decision made off a render the model itself distrusts.

### R8 — Mutate pile death emits two unexplained `battlefield -> graveyard` lines
`1787165717` seq12 @14000: Feed the Swarm destroys the Snapdax pile; the log emits a separate
`Opponent's Dryad of the Ilysian Grove: battlefield -> graveyard` with no indication it was the
merged component. *"Why? Feed the Swarm destroys a creature or enchantment. Snapdax was the target.
Dryad was removed too? … **Maybe it was a mistake in the log provided in the prompt?**"*

### R9 — Duplicate cards in hand are not instance-numbered, so duplicates read as log contradictions
Lands on the battlefield get `Swamp #1 / #2`; hand cards and non-land permanents do not. With one copy
cast and one held, the game log and the hand list appear to conflict.
`1787165717` seq12 @1841: *"**There is a contradiction between the Game Log (March was cast on Turn 4)
and the Current Situation (March is in hand).**"* — 2,000 chars to resolve to "I have two copies."
Same shape at `1787180113` seq14 @23969: *"It lists 'Mountain (land: taps for {R})'. This must be a
different Mountain **or the one I just played is not removed from the list in the prompt text
(sometimes prompts are inconsistent)**."*

### R10 — Lowercase hybrid `{g/w}` does not read as hybrid → false "I cannot cast" belief
`1787208914` seq3 @5357: *"Kitchen Finks is {1}{G/W}{G/W} (requires White? Wait, the text says
{1}{g/w}{g/w}. **That looks like a typo in the provided decklist text** … Standard Kitchen Finks is
{1}{G}{W}. … Let's assume it needs Green and White. I have no White sources. **I cannot cast Kitchen
Finks currently.**"*
The render is *correct* ({G/W} hybrid is real Oracle); the model's "standard is {1}{G}{W}" is an
invented card fact. But the outcome is a false unavailability belief that changed play. **Fix on the
render side** — annotate hybrids, e.g. `{1}{G/W}{G/W} (hybrid: G or W)`.

## Card-script defect (Oracle mismatch)

### C1 — Scarecrone sacrifice clause is wrong
`1787201714` seq15 @3044: card text supplied as *"{1}, Sacrifice a Scarecrow: Draw a card."*
Real Oracle: **"{1}, Sacrifice an artifact creature: Draw a card."** The model flags it and cannot
reconcile it with the guide: @15654 *"'Sacrifice a Scarecrow' implies *another* Scarecrow? Strategy
Guide says: 'sacrifices Scarecrone itself'. **There is a contradiction between the Strategy Guide and
the Card Text provided in the deck list.**"*
Should go through `/wagic-cardscript` (verify Oracle → express faithfully in card script).

## Invented rules claims (model-side, no engine defect)

### M1 — "Urborg doesn't tap for mana"
`1787162603` seq6, repeated ~6× across the trace: *"Does Urborg tap for mana? No, Urborg is a land
that makes other lands Swamps, **it doesn't tap for mana itself** unless specified (it doesn't have
{T}: Add {B})."* False — Urborg makes *itself* a Swamp. This false belief is what turned a routine
land drop into a 24,246-char trace; the same model states it *correctly* one decision later
(seq7: *"Urborg makes all lands Swamps, Snow-Covered Swamp taps for B"*, 3,666 ch, churn 0).
Interacts with R1: had the mana line named its source, the false belief would have been unreachable.

### M2 — "Two copies of March from the Black Gate don't stack"
`1787165717` seq7 @3900: *"Having two copies doesn't stack the 'whenever' trigger (it's a static
ability on the enchantment). Casting another one does nothing extra mechanically … **Bad play.**"*
False (two enchantments = two triggers), and it directly contradicts the strategy guide the model is
holding (*"cast BOTH copies"*). The model reverses itself at seq12 and reasons correctly.

### M3 — Enchantment mislabelled as sorcery
`1787165717` seq9 @4000: *"March from the Black Gate {1}{B} is a sorcery"* despite the prompt's
`[enchantment]` tag on the same line. Low impact, but evidence the type tag is being skimmed.

## Prompt-caused confusions (old register — NEW shapes only)

### D1 — "ONLY if your plan changed" is unresolvable and is the corpus-wide #1 churn driver
Present in **77 / 90 traces** (661 marker hits). Two irresolvable sub-cases:

**(a) No prior plan exists** (turn 0). `1787172917` seq1 @2000–4600, `1787187313` seq1 @2433,
`1787162603` seq1 @5400–12204 (**7,000 chars — 57% of the trace**) cycling:
*"Since there is no previous plan, I can't say it's 'unchanged'. I am creating one."* …
*"to be safe on the 'ONE line' constraint if the system interprets 'unchanged' as 'no previous plan
to compare', I might skip it."* … *"Actually, to be most helpful … I will write a plan. Wait,
re-reading: 'If your plan is unchanged, OMIT the PLAN line entirely'. Okay, I'll just provide the
Choice."* The model flips its answer at least six times.

**(b) Executing an existing plan** — is that "changed"? `1787165717` seq4 @4000–12500,
`1787162603` seq16 (17,283 ch on a **two-option target pick**), `1787180113` seq5 @8033:
*"Playing the land is executing the plan. The fact that March is already out is a state change, but
my *intent* … is the same."*

**Fix:** state the rule as a decidable test, not a comparison against a possibly-absent prior — e.g.
"Write a PLAN line whenever your plan is absent, stale, or now names an action you have already taken;
otherwise omit it."

### D2 — "action name exactly as listed" vs option labels carrying embedded annotation blobs
Options render as
`3. Cast Mordor Muster {1}{b} {right now: Army 1/1 -> 2/2} {card text: "You draw a card and you lose
1 life. -- Amass Orcs 1. (Put a +1/+1 counter…)"}` — and the card-text blob is often *truncated
mid-sentence with a trailing `...`*. "Exactly as listed" is then impossible to satisfy.

`1787165717` seq4 @5500–12500 and seq7 @9500–15400 (each ~6,000 chars) agonize over scope:
*"I should use the short name or the full string?"* … *"I'll use the full option text minus the
leading 'Cast '."* … *"copying the full text might be safer to match 'exactly as listed'. **But it
cuts off.**"* seq7's reply then echoes the entire 250-char blob. **This is the mechanism behind the
42/90 `post_answer_overrun`.**

**Fix:** separate the echo-able label from its annotations — put the label first and make the
matched span explicit, e.g. `3. Cast Mordor Muster {1}{b}  — right now: Army 1/1 -> 2/2 — card text: …`
with the instruction naming only the pre-dash span. Never truncate card text mid-word inside a
string the model is told to copy verbatim.

### D3 — "Write your plan FROM the list" is read as validating the PLAN line against the current options
**The single most expensive prompt defect in the batch.** `1787194513` seq9 (26,829 ch, depth 24,000)
spends chars **@9891–23900 — roughly 14,000 characters, 52% of the trace** — on nothing else. Excerpts:

- @15090: *"The numbered choices *right now* are about the Land Drop. 'Nadaar' is NOT in the numbered
  choices for the Land Drop. Does this mean I cannot mention casting Nadaar in the Plan line?"*
- @18764: *"if the system parses the Plan line against the current list of choices (1, 2, 3), it will
  fail if it expects the card to be in the list."*
- @22750: *"So I cannot name 'Nadaar' in the Plan line according to this strict reading. I must plan
  using the cards/actions available in the list? **No, that makes no sense for a Plan line which
  covers the whole game.**"*
- @23888: *"Okay, I'll follow the instruction literally. If 'Nadaar' isn't in the numbered choices, I
  shouldn't name it in the plan? **That seems wrong for a Plan line.**"*

Recurs at `1787194513` seq11 (twice) and `1787187313` seq13. The rule is written for the *CHOICE*
line but sits adjacent to the PLAN instruction, and "not available right now" is being applied to a
line explicitly scoped "from here on."

**Fix:** scope it explicitly — "This applies to your CHOICE line only. Your PLAN may name any card in
your hand or deck."

### D4 — Casting prompts never state whether the land drop is still available
The model repeatedly cannot tell whether it has already played a land, and has to reconstruct it from
the game log. `1787165717` seq12 @12500: *"Can I play a land AND cast a spell in the same turn? …
**The interface provided 'Casting decision' options. It doesn't explicitly offer 'Play Land'.**"*
Also `1787172917` seq3 @3000, `1787187313` seq16 @4409: *"Can I play a land? … the prompt asks 'which
card do you cast now, if any?'. It does not explicitly ask about land drops."*

**Fix:** one clause on casting prompts — `Land drop: already used this turn.` / `still available`.

### D5 — Cast-then-target flow is undeclared
When an option is `Cast Thoughtseize - legal targets right now: the opponent, you`, the prompt does
not say a separate TARGET CHOICE step follows. `1787162603` seq10 @20500 and seq15 @16500:
*"It implies I target the opponent by default or the game handles it."* … *"It seems the game engine
will handle the target selection after I choose to cast it."* ~1,500 chars per occurrence.

### D6 — The stale-plan note is read as an instruction to rewrite the PLAN, compounding D1
`1787162603` seq16 @10500–14000 (3,500 chars): *"the note says 'the actions your plan names are no
longer among the options available right now — the game state has advanced past that plan; re-derive
your choice'. **This note suggests I should re-derive the choice.** … It doesn't necessarily mean I
need to write a new PLAN line in the output."* The model litigates this four separate times.

---

# Supported reasoning budget

**Recommend ~7,000 tokens (≈28,000 chars).**

Evidence:
- Hand-measured diagnostic depth: p50 2,900 tok, **p90 4,875 tok**, max 6,000 tok — on a sample
  deliberately biased toward the longest traces, so these are conservative-high.
- The genuinely deep traces (churn < 0.15) are still producing new diagnostic content at their final
  character (`1787172917` seq10 at 19,887 ch; `1787194513` seq9 at 26,829 ch; `1787180113` seq14 at
  28,344 ch). Cutting below ~7,000 tok would clip real development signal from exactly the traces
  where a render defect is unresolvable — the highest-value traces in the harvest.
- Only 3/90 hit the current cap, and none of the three shows evidence of truncated diagnostic content.

**Do not tighten the budget to buy back the 28.5% mean churn.** The churn is not a budget problem —
it is D1/D2/D3 (three prompt defects) plus R1/R2 (two render defects). Fixing those five is worth far
more than a cap, and a cap would truncate depth before it truncates churn, since churn lives at the
*tail* of long traces but so does the deep content of the R5/D3 class. Re-measure depth after the
five fixes land; the p90 should fall well below 4,000 tok, at which point tightening is evidence-based.

## Ranked fix list (by measured churn cost)

| # | Defect | Cost |
|---|---|---|
| 1 | D3 "plan FROM the list" scope | up to 52% of a single trace (14,000 ch) |
| 2 | D1 "ONLY if your plan changed" | 77/90 traces; up to 57% of a trace |
| 3 | R1 unattributable `Mana available` | 5+ traces; up to 42% of a trace |
| 4 | D2 option-label echo scope | ~6,000 ch/occurrence; causes 42/90 overruns |
| 5 | R2 `Already in pool` phantom mana | 2 traces, high confusion per hit |
| 6 | R6 `StackAbility. (Source: )` | 1 trace at churn 0.64; trivial fix |
| 7 | R3 `[cantblock]` + "CAN block" | trivial fix, corpus-wide |
| 8 | R4 opponent summoning-sick tag | trivial fix, corpus-wide |
| 9 | R5 artifact-count vs CDA off-by-one | needs an engine check, not a render fix |
| 10 | R7/R8 mutate pile render | 2 traces; affects a targeting decision |
| 11 | D4 land-drop availability | 3+ traces |
| 12 | R9 hand-card instance numbering | 2 traces, ~2,000 ch each |
| 13 | D5 cast-then-target flow | 2 traces, ~1,500 ch each |
| 14 | R10 lowercase hybrid `{g/w}` | 1 trace, changed play |
| 15 | C1 Scarecrone card script | Oracle mismatch — route to `/wagic-cardscript` |
