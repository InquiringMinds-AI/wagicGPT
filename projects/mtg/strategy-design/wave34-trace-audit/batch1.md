# Wave-34 thinking-trace audit — Batch 1

Auditor 1 of 6. Log-reading only; no engine runs.

**Corpus**: 8 seat logs, 125 traces carrying a `reasoning` field.

| file (prefix) | matchup | traces |
|---|---|---|
| 1787162603 | deck109 vs deck133 | 13 |
| 1787165717 | deck139 vs deck158 | 24 |
| 1787172917 | deck139 vs deck36 | 13 |
| 1787180113 | deck116 vs deck158 | 17 |
| 1787187313 | deck116 vs deck152 | 16 |
| 1787194513 | deck139 vs deck146 | 19 |
| 1787201714 | deck146 vs deck36 | 10 |
| 1787208914 | deck105 vs deck116 | 15 |

Decision kinds: `ask` 101, `attackers` 16, `reveal` 4, `blockers` 3, `priority` 1.

---

## 0. Methodology note

The brief budgeted for a keyword sweep over most of the batch. The batch turned out to be **125 traces, not ~125 traces × unreadable bulk** — total ~1.8 M chars — so coverage went deeper than planned:

1. **Full close-read, 18 traces** (both `reasoning_budget_hit` traces, the 8 longest, and 8 spanning the short/mid range down to 1,187 chars). These are the ground-truth set for depth.
2. **Tail close-read** was the primary depth instrument. The quantity being measured — offset of the *last* novel diagnostic — lives in the tail, so reading the final 3–6 k chars of a trace answers it at a fraction of the cost of a full read. Applied to all 18 ground-truth traces.
3. **Mechanical estimator over all 125** (`depth.py`): last match of a diagnostic-marker regex, *suppressing* matches that sit inside a reply-format/PLAN-line deliberation window (≥2 format markers within ±250 chars). Calibrated against the 18 ground truths: **mean bias −45 chars, mean absolute error 2,326 chars** — unbiased, adequate for a distribution, not for any single trace.
4. **Targeted regex sweeps** over `reasoning` and `prompt` for defect classes, with verbatim excerpt extraction (`pgrep.py`).

**Instrument caveat, recorded per fleet practice.** A first sweep pass silently ran over **248** traces instead of 125: the 10-char timestamp prefix used to select files collides with the sibling *seat* log of the same game (`1787162603-…deck109-…` and `1787162603-…deck133-…`). Every early number was contaminated by the opposing seat. All figures below are from full-filename selection. Anyone reusing these scripts: select by full filename, never by timestamp prefix.

A second caveat on the churn metric: a naive "last marker" sweep put the mean post-marker fraction at 5.5%, because the dominant terminal marker is `wait,` / `actually,` — which in this corpus is overwhelmingly *oscillation*, not new belief. Treating those as diagnostic inflates depth to near-100% of every trace. The suppression rule above is what separates the two, and it is the single judgment call the numbers rest on.

---

## JOB 1 — Diagnostic-depth measurement

### Trace length (context)

| | chars | ~tokens (/4) |
|---|---|---|
| p10 | 6,780 | 1,695 |
| p50 | 14,517 | 3,629 |
| p90 | 21,870 | 5,467 |
| max | 27,131 | 6,783 |

`reasoning_budget_hit` fired on only **2 / 125** (1.6%) — 1787180113 seq8 (22,020 ch) and 1787194513 seq16 (14,517 ch). Both still emitted a well-formed answer + PLAN; the flag is not producing truncated replies. Note the two hit at very different lengths, so the flag is not a fixed char ceiling.

### Absolute diagnostic depth (offset of LAST novel development-diagnostic element)

Estimator over all 125:

| | chars | ~tokens |
|---|---|---|
| **p50** | **12,919** | **3,230** |
| **p90** | **19,839** | **4,960** |
| **max** | **24,932** | **6,233** |

Cumulative — fraction of traces whose last novel diagnostic lands at or before a given budget:

| budget | ~tokens | traces covered |
|---|---|---|
| 4,000 ch | 1,000 | 12% |
| 6,000 ch | 1,500 | 17% |
| 8,000 ch | 2,000 | 22% |
| 10,000 ch | 2,500 | 30% |
| 12,000 ch | 3,000 | 44% |
| 16,000 ch | 4,000 | 77% |
| 20,000 ch | 5,000 | 91% |

### Post-diagnostic churn fraction

- **Mean 16.8%** (estimator, all 125) · **median 8.6%**
- **Mean 11.9% · median 11.6%** on the 18 close-read ground truths (which skew long by construction)

By trace length:

| length bucket | n | depth p50 | mean churn |
|---|---|---|---|
| < 5 k | 9 | 1,456 | 43% |
| 5–10 k | 17 | 5,910 | 27% |
| 10–18 k | 68 | 12,545 | 14% |
| 18 k+ | 31 | 18,256 | 10% |

### The headline finding of Job 1

**Churn is not a tail. It is interleaved.** The naive model — model does its diagnostic work, then pads — is wrong for this corpus. Trailing churn is only ~12–17%; the reason traces run to 14 k chars median is that the two biggest churn generators (PLAN-line deliberation, option-name copying) fire *repeatedly throughout* the trace, between diagnostic passes.

Two structural measures make this concrete across all 125:

- **"final check" / "double-check" / "one more thing" / "ready to write" closers: mean 3.9 per trace, max 9, ≥3 in 74% of traces.** The model announces it is finished ~4 times per decision.
- **≥3 separate PLAN-line deliberations in 112 / 125 traces (90%).**

Consequence for budget-setting: a pure token cap does **not** recover the wasted 40–60%. It truncates diagnostics, because the last novel diagnostic is frequently a late-discovered render contradiction (see F-05, F-07, F-09 below — all found past @18,000). The compression lever is the churn *drivers* (§ Job 2, F-12 through F-16), not the ceiling.

---

## JOB 2 — Development findings

Each entry: file prefix + seq, verbatim quote, repro.

### A. Render / engine defects the model detected

**F-01 — `Mana available:` line contradicts the `[tapped]` tags on the battlefield list.** *(explicit contradiction in 5/125 = 4%; the line is re-quoted ≥2× in 63% of traces, mean 3.4 quotes/trace — the single most re-verified render element in the corpus)*

> `1787165717 seq13 @3285`: "If all lands are tapped, I have 0 mana. But the prompt says 'Mana available: 1 total (one untapped source...)'. This contradicts the 'tapped' tags in the battlefield list. However, I must trust the 'Mana available' line provided by the engine"

> `1787187313 seq7 @13625`: "So I *did* play a land. But 'Mana available: 3 total' contradicts this. Perhaps the 'Mana available' text is static from the start of the turn before the land drop?"

Repro: any turn where a land entered this turn, or where a pending choice has not yet resolved. The two emitters (mana summary, per-permanent tap tags) are computed at different moments.

**F-02 — `Your library: N cards` reports 0 while a reveal/search simultaneously offers a 47–49 card list.** *(4/125; concentrated in 1787187313 and 1787180113 seq17)*

> `1787187313 seq11 @9844`: "the log says 'Your library: 0 cards'. But the prompt says 'Reveal: you looked at the top 47 cards of your library.' This is contradictory. ... Usually, if library is 0, you lose the game on draw step."

> `1787187313 seq11 @20906`: "The '0 cards' in the summary is **definitely an inconsistency/error in the prompt generation**. I must follow the explicit instruction to choose from the list."

> `1787187313 seq8 @8038`: "The 'Your library: 0 cards' line in the Current Situation might be a display error in the prompt's summary"

Repro: any `reveal` / library-search decision. Cost: ~2,000 chars per occurrence spent reconciling, plus a real risk the model declines a search spell it believes will fizzle — `1787187313 seq11 @19906` reads "if I have 0 library, Farseek fails" and nearly drops Farseek from the plan on that basis.

**F-03 — A permanent is listed in BOTH `Your hand` and `Your battlefield`.** *(7/125)*

> `1787165717 seq14 @8184`: "Current Situation: 'Your hand: Pollywog Symbiote...'. Current Situation: 'Your battlefield (creatures: 3)... Pollywog Symbiote...'. This is a **duplicate entry error** in the prompt text."

The model burns @3015→@11013 (≈8 k chars, 66% of the trace) on this one ambiguity, and ends unresolved: "Even if Pollywog is in my hand, I can't cast it (wrong phase). So regardless of where Pollywog is, I cannot use it to block if it's in hand."

**F-04 — During a shockland `pay 2 life / tap` choice, the pending land is rendered on the battlefield *already tapped*, and/or still in hand.** *(1787172917 seq3, seq6; 1787165717 seq6)*

> `1787172917 seq6 @7241`: "'Stomping Ground #2 [tapped]'. This suggests the system might have already decided it enters tapped, OR it's showing the state *before* the choice is made (which is contradictory)."

> `1787172917 seq3 @1079`: "the log says 'Your Stomping Ground: hand -> battlefield' but then the 'CURRENT SITUATION' lists 'Your hand: Plains; Stomping Ground...'. This is contradictory."

The option text also never names the permanent — it says only "**this permanent**" — so the model must infer which land the choice is about from the log line above it.

**F-05 — `Phase:` line reports Upkeep after Draw and Main Phase 1 are already in the log.** *(1787180113 seq11, a `priority` node)*

> `@21153`: "Current Situation says 'Phase: Upkeep'. If it's Turn 10 Upkeep, then Draw hasn't happened yet. But the Log shows Turn 10 Draw happened ('Your Hinterland Harbor: library -> hand'). Contradiction again."

Same trace, two further state mismatches in the same prompt:

> `@10844`: "The Log says Thragtusk was played in Turn 10. The Current Situation says only Beast is there. This is a contradiction in the prompt data."

> `@14938`: "So at end of Turn 9, I am at 12. Current Situation says: 'Your life: 11'. This implies I took 1 damage? Or the log is inconsistent."

**F-06 — `[cantblock]` and "but CAN block" printed on the same creature.** *(NEW emitter shape — the summoning-sickness annotation hard-codes the blocking clause without consulting the can't-block flag)*

> `1787162603 seq13 @7972`: "'Bloodghast {b}{b} (2/1) [cantblock] [summoning sick - cannot attack this turn, but CAN block]' ... This is contradictory. 'cantblock' tag vs 'CAN block' text."

Repro: any summoning-sick creature that also has can't-block (Bloodghast, walls with the flag, etc.). Fix is local: make the summoning-sick suffix conditional on the block flag.

**F-07 — The mutate option label drops the generic component of the mutate cost.** *(1787194513 seq16 — one of the two budget-hit traces)*

Option label: `mutate cost {w}{w}{b/r} ... [hybrid: each {b/r} pays with B or R - total 3 mana]`
Card text in the same option: `Mutate {2}{BR}{W}{W}`

> `@10000`: "The card text in the deck list says {2}{BR}{W}{W}. The choice text says 'mutate cost {w}{w}{b/r} ... total 3 mana'. **This is contradictory.** However, the instruction says 'Every listed choice is legal AND PAYABLE right now'. Since I have 5 mana, I can afford either interpretation."

The mutate-cost renderer omits `{2}` and then asserts a wrong "total 3 mana" gloss. Only luck (5 available mana) kept this from producing a bad decision. **Highest-severity render bug in the batch** — it is a *wrong number stated as engine ground truth*, not merely an ambiguity, and it is exactly the class the guide's "trust the numbers the game prints" rail tells the model to believe.

**F-08 — Mutate over/under choice is offered *after* the log shows the spell resolved.** *(1787165717 seq24)*

> `@4735`: "'Your Everquill Phoenix: stack -> battlefield'. This sequence usually happens *after* the spell resolves. But the 'CURRENT SITUATION' block presents a choice *for* Everquill Phoenix. This is contradictory. If it went to battlefield, the choice is done."

**F-09 — `Your battlefield (creatures: N):` header sits atop a list of ALL permanents, so the model reads lands as creatures.** *(5/125)*

> `1787162603 seq13 @2147`: "This list is confusing. It lists Mountains as 'creatures'? No, it says 'Your battlefield (creatures: 3): ...'. Then it lists Mountains. **This looks like a parsing error in the prompt's representation of the board.**"

Repro: universal — every board render has this shape. Costs a re-parse pass in a fifth of traces. Cheap fix: label the count as "creatures among these", or segment the list.

**F-10 — The `colours you can make:` line has no source attribution, so the model fabricates a cause.** *(NEW emitter gap, analogous to the docketed counter-source gap)*

> `1787162603 seq13 @8844`: "why {r}{b}? I have 4 Mountains. Mountains make {R}. Where does {B} come from? Ah, the opponent has Urborg (Tomb of Yawgmoth) which makes all lands Swamps."

The model invents an explanation (an Urborg that may or may not be on the board) rather than reporting a defect. When a summary number is unexplained the model will *confabulate a mechanism*, and that confabulation then feeds later reasoning as fact.

### B. Prompt-caused confusion (protocol layer)

**F-11 — The system prompt still carries the OLD reply protocol, in direct conflict with the new one.** *(explicitly litigated in 11/125 = 9%; the conflicting text is quoted verbatim by the model, so this is confirmed present in the system prompt / strategy guide, which is not captured in the `prompt` field)*

Stale text: `"Write your CHOICE / ATTACK / BLOCKS / PUT line, two or three sentences of reason, and STOP."`
New text: `"Your reply is ONE line, or TWO when your plan changed... Write no reasoning, no commentary, no restatement of the board and no working in the reply itself"`

> `1787165717 seq21 @21324`: "**It contradicts** the 'Write your CHOICE... two or three sentences of reason' instruction in the middle. The final instruction block ('HOW TO REPLY') is the most specific and recent."

> `1787194513 seq3 @4901`: "The 'two or three sentences of reason' instruction seems to conflict with the 'Write nothing else' instruction in the 'HOW TO REPLY' section."

The model resolves it correctly every time observed (recency + specificity), so no bad answers resulted — but it pays 300–900 chars each time, and it is one edit to delete. **Highest value-per-effort fix in the batch.**

**F-12 — The model believes the `PLAN:` line is an ACTION/state channel the engine consumes, not a note to its future self.** *(the corpus's single largest churn generator: ≥3 PLAN deliberations in 112/125 = 90%; explicit action-channel belief in 5/125)*

> `1787180113 seq14 @24131` (longest trace in the batch, 27,131 ch): "If I don't write a PLAN line, **the system might not register the Amulet activation**. In these text games, **the PLAN line is the primary way to communicate actions that aren't direct selections in the provided list** (since the list is limited to Attacks/Blocks/Casts). So I must write the PLAN line."

> `1787172917 seq8 @23082`: "If I don't, the system might assume I still want to 'Play Plains' next turn. That would be bad. So I must write it."

> `1787180113 seq15 @22777`: "Plan Change: Yes, because I can't execute the old plan"

The "ONLY if your plan changed" gate is unanswerable as written: the model cannot decide whether *advancing through* a multi-step plan counts as changing it, and re-litigates that question 3–9 times per decision. Verbatim, `1787172917 seq8 @22082`: "If I interpret 'unchanged' as 'consistent with previous intent', I can omit it. But 'unchanged' usually means 'I still intend to do X, Y, Z as previously stated'. If I've done X, then Y, Z is the new plan."

**F-13 — The stale-plan annotation false-fires, and the model then reads it as a statement about legality.** *(fires on 30/125 = 24% of prompts; false on essentially every non-cast decision type)*

Annotation: `(note: the actions your plan names are no longer among the options available right now - the game state has advanced past that plan; re-derive your choice from the current board and the options below.)`

Clear false positives:

| file/seq | plan text | options offered |
|---|---|---|
| 1787172917 seq3, seq6 | "Play Stomping Ground untapped **(pay 2 life)**" | `pay 2 life` / `tap` |
| 1787165717 seq21 | "Play Temple Garden untapped **by paying 2 life**" | `pay 2 life` / `tap` |
| 1787162603 seq11, seq13 | "**attack with all creatures** to trigger Battalion" | `A1./A2./A3.` attacker list |
| 1787180113 seq8 | "Next turn **play a land**" | `Play Island` / `Play Hinterland Harbor` |

It appears to fire whenever the plan text does not literally string-match an option label — which is structurally impossible for `pay 2 life`, `A1/A2/A3`, and `B1:A1` decision types. Worse, the model treats it as authoritative rules information:

> `1787180113 seq15 @21777`: "The note says 'actions your plan names are no longer among the options available'. **This confirms I cannot Activate Amulet.**"

> `1787165717 seq21 @7507`: "This is a bit contradictory. If I select Choice 1, I am executing 'Play Temple Garden untapped'."

A false annotation is being converted into a false belief about what is legal. This is a NEW shape beyond the docketed old-register list and should be docketed separately from the log-register rewrite.

**F-14 — "its name in parentheses" is unanswerable when option labels carry `{card text: "..."}` and bracketed glosses.** *(20/125 = 16% deliberate it explicitly; drives 42/125 nonzero `post_answer_overrun`, mean 59, max 347)*

> `1787187313 seq12 @19923`: "Name in parentheses: 'Cast Explore {1}{g} {card text: "You may play an additional land this turn. -- Draw a card."}' ... Wait, the instruction says 'its name in parentheses'. Example: 'CHOICE: 3 (Cast Example Card)'. ... I will use 'Cast Explore {1}{g}' to be precise, or 'Cast Explore'."

> `1787165717 seq21 @22032`: "'CHOICE: 2 (tap [decline the payment; this permanent instead enters the battlefield TAPPED - unusable until your next untap step])' ... usually I should shorten it to the core name if possible, but the instruction says 'its name in parentheses'"

The `chosen_text` for 1787194513 seq16 shows what the model actually emits when it decides to copy faithfully — a 300-char option label including the full card text. That is where the answer-overrun bytes come from.

**F-15 — The `priority`-node legal-actions list is read as an exhaustive legality statement.**

> `1787180113 seq11 @21698`: "The list provided is: '1. Put in Play with Quicksilver Amulet targeting Kitchen Finks'. **It does NOT list 'Cast Time of Need'. This might imply I cannot cast Time of Need right now. Why? Maybe I don't have enough mana?** ... Maybe the system only lists interactions with permanents on the battlefield?"

The prompt never states that further decision nodes are coming this turn, so absence from *this* list reads as illegality. The model recovers by guessing correctly, but it guesses.

**F-16 — Activated abilities are never surfaced as options, so the model routes them through PLAN.** *(5/125; all in 1787180113, the Quicksilver Amulet deck)*

> `1787180113 seq14 @22131`: "the 'Activate' action isn't in the 'Attackers' list, I need to ensure the system tracks it."

The whole win condition of that deck (Amulet activation) has no decision seam. The model spends the deck's entire game trying to execute it through the plan channel. Combined with F-13, the model at `seq15` concludes it *cannot* activate — and it never does.

### C. Rules and card facts the model invented

**F-17 — The model fabricates its own library contents from training priors.** *(10/125)*

> `1787180113 seq8 @2900`: "If I cast Time of Need now, I search for a legendary creature. Which one? - Emrakul (15/15, Annihilator 6, Protection from Colored Spells). - Kozilek (12/12, Annihilator 4). - Ulamog (10/10, Indestructible, Annihilator 4). - Sheoldred (6/6...). - Jin-Gitaxias (5/4, Draw 7)."

At that point it had not seen its library. The `seq11` reveal later confirmed several of these are genuinely in the deck, so the priors were partly right — which is worse for the dev loop, because it means the behaviour will look correct in aggregate while being unfounded. Every downstream plan in that game ("cheat Emrakul, win via Annihilator") rests on it.

**F-18 — Oracle text recited verbatim from training for cards whose render carries no text.**

> `1787180113 seq8 @18400`: "Orcish Bowmasters: 'Whenever an opponent draws a card except the first one they draw in each of their draw steps, Orcish Bowmasters deals 1 damage to any target. Then amass Orcs 1.'"

> `1787180113 seq8 @1700`: "Hinterland Harbor: Dual land {U}{G}. **Enters tapped unless I control a Forest or Island.**" — the render says only `(land: taps for {U}{G})`.

> `1787180113 seq14 @23131`: "Quicksilver Amulet: '{4}, {T}: You may put a creature card from your hand onto the battlefield.'" — derived from the render's `Quicksilver Amulet {4} [artifact]`, where `{4}` is the *mana value*, not the activation cost. Correct here by coincidence; the render gives no way to tell them apart.

**F-19 — Genuine rules questions the render leaves unanswered.**

> `1787194513 seq16 @13100` (mutate + summoning sickness): "Does the merged creature have summoning sickness? ... Actually, usually, the merged creature is treated as the base creature for timing purposes." — reached by guess, marked "usually".

> `1787180113 seq8 @18900`: "If I cast Time of Need, I search my library. **Does searching count as drawing?** No, searching is not drawing." — re: whether Orcish Bowmasters triggers.

> `1787201714 seq5 @13302`: "confirm if I need to name the card for Silencer. The option text includes the card text which says 'As Silverquill Silencer enters, choose a nonland card name.' Usually, the game would ask me to name the card *after* I select the option." — a real question about whether a sub-decision seam exists. It does not know.

**F-20 — A hard numeric rail in the strategy guide gets litigated at its boundary.**

> `1787172917 seq10 @17800`: "the damage would otherwise put you below 6 life ... Life would be 6. Condition 'below 6' is false (6 is not below 6). So I am not forced to block."

Correct reading, but the same trace carries a **false belief about its own life total** — it states "I am at 8 life" at `@15823` while the prompt says `Your life: 16` and "you would be at 6". The block-floor arithmetic and the life-total belief disagree inside one trace.

---

## Trace-level pathology shapes (budget-relevant structure)

**P-1 — Multi-close ("I'm done" ×4).** Mean 3.9 closers per trace (`final check` / `double-check` / `one more thing` / `ready to write`), max 9, ≥3 in 74% of traces. The model reaches a decision, announces completion, then reopens. Each reopen costs 400–1,200 chars and typically restates a settled fact.

**P-2 — Binary A/B oscillation with no new belief.** The cleanest instance is `1787194513 seq2 @18842–@22000`: ~3,500 chars cycling "If I fetch Temple Garden, I keep Island in hand / If I fetch Island, I keep Temple Garden in hand" five times verbatim, ending where it started. No marker word fires — the mechanical `wait,`/`actually` sweep misses this shape entirely.

**P-3 — Silent thrash (no markers, degraded prose).** `1787172917 seq5`, a short trace (2,844 ch), flips Stomping Ground → Plains → Stomping Ground inside 1,000 chars with the prose degrading into repetitive filler ("The key is balancing immediate mana needs with long-term strategic positioning"). Distinct from P-2: no explicit comparison structure, just drift. Undetectable by keyword.

**P-4 — Annotation distrust → full re-enumeration.** `1787187313 seq11 @21000–@23000`: the reveal prompt had already tagged every one of 47 cards `[ELIGIBLE]` / `[does NOT qualify]`, and the model re-enumerated all 47 by hand to re-derive "only Quicksilver Amulet is an artifact". 2,000 chars, ~9% of the trace, to reconstruct information already printed. The engine's helper annotations are not being trusted — plausibly *because* of F-01/F-02/F-03 teaching the model that engine summaries are unreliable. **The render defects and the churn are causally linked.**

**P-5 — Format-anxiety tail.** In 8 of the 18 close-read traces, the final 1,500–3,500 chars contain zero game content — purely PLAN-line necessity (F-12) and option-name copying (F-14). This is the one genuinely *trailing* churn block, and it is the ~12% measured above.

---

## Budget recommendation from this batch's data

The data does **not** support setting a low cap and expecting the diagnostics to survive.

- To retain the last novel diagnostic in **77%** of decisions: **16,000 chars ≈ 4,000 tokens**.
- To retain it in **91%**: **20,000 chars ≈ 5,000 tokens**.
- Cutting to 3,000 tokens loses the last novel diagnostic in **56%** of decisions — and the losses are biased toward exactly the traces that matter most, because the deepest diagnostics (F-05, F-07, F-09, F-13) are late-discovered render contradictions found past @18,000.

**Recommended for this batch: 5,000 tokens (~20 k chars).** Current p90 is 5,467 and only 2 traces hit the budget, so this is close to a no-op on the ceiling — which is the point. The ceiling is not where the waste is.

**The actual compression path, in order of value per unit of effort:**

1. **Delete the stale "two or three sentences of reason" line from the system prompt** (F-11). One edit; removes a self-contradiction the model detects in 9% of traces.
2. **Replace the "ONLY if your plan changed" gate** (F-12) with an unconditional, cheap instruction — always emit `PLAN:`, or never. The current gate is unanswerable mid-plan and is deliberated 3+ times in **90%** of traces. This alone is the largest single lever in the corpus. State explicitly that PLAN is a note to your future self and is **not** an action channel — the model currently believes the opposite and has routed an entire deck's win condition through it.
3. **Fix the stale-plan annotation's firing condition** (F-13) — 24% fire rate, near-100% false on `pay 2 life` / attacker / blocker prompts, and it is being converted into false legality beliefs.
4. **Define what to copy in `CHOICE: n (...)`** (F-14) — give the option a short stable name distinct from its gloss, or drop the name requirement and take the number. Removes 16% of traces' format deliberation and most of the answer-overrun bytes.
5. **Fix F-07 (mutate cost) and F-02 (library 0) as correctness bugs**, not ergonomics — both print wrong numbers under a prompt rail that instructs the model to trust printed numbers.
6. **Surface activated abilities as decision options** (F-16).

Expected effect: items 1–4 are all *interleaved* churn, not tail churn, so removing them should compress median trace length substantially further than the 12–17% trailing-churn figure suggests — plausibly toward the 8–10 k char range — while leaving diagnostic depth intact.

---

## Artifacts

Scripts used, in `/tmp/claude-1000/-home-magi/af45c871-41e2-4fdf-b823-118307d55316/scratchpad/w34/`: `dump.py` (single-trace dump with @offset markers), `tail.py` (batch tail-read), `pgrep.py` (regex + context over `prompt`/`reasoning`), `sweep.py`, `depth.py` (depth estimator + calibration), `depth.json`, `sweep.json`. All select files by full filename — see the § 0 caveat.
