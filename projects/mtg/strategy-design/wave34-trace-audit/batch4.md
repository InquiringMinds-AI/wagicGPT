# Wave-34 thinking-trace audit — batch 4

Auditor 4 of 6. Log-reading only. Corpus = the 7 assigned files in `~/.Wagic/ai/gpt/logs/`.

## 0. Corpus shape (measured, not assumed)

- **91 `kind:"ask"` records** across the 7 assigned files (+7 `gamestart`). Each file holds
  ONE game seat; sibling files with the same timestamp prefix are the *opponent's* seat and
  were excluded (assignment was by exact filename).
- `reasoning_chars`: **p50 15,366 · p90 21,085 · max 25,622 · min 5,097 · mean 15,301**
  (÷4 ≈ p50 3.8k tok, p90 5.3k tok, max 6.4k tok).
- **⚠ `reasoning_budget_hit` is NOT EMITTED AT ALL in this corpus.** The field is absent from
  every record (key set verified). The stratification step "FULL-READ all budget_hit:true"
  therefore had zero members, and **no truncation evidence exists in these logs**. Either the
  emitter was not shipped in this build or the cap was never reached. This is itself a
  logging finding: without the flag we cannot distinguish "model finished" from "model was cut".
- `answer_replaced`, `commit_retracted`, `post_answer_overrun`, `post_plan_overrun`,
  `dropped_assignments` are all present and all zero across the batch — the NEW reply protocol
  parsed cleanly 91/91. No protocol-level failures to report.

**Method actually run**: full close-read of **15 traces** (8 of the 10 longest by
`reasoning_chars`, plus 7 stratified-random), with per-line char offsets, plus a
regex sweep of all 91 traces for the diagnostic keyword set and for structural emitters.

---

## JOB 1 — Diagnostic-depth measurement

Scored per close-read trace: char offset of the **last novel development-diagnostic element**
(a false belief in the model's own words, an invented card fact, a verbalized prompt
confusion, the completion of first plan derivation, or annotation-misparse evidence).
Everything after that offset is churn (re-verification, recounting, no-new-belief oscillation,
board restatement, reply-format re-litigation).

| file | seq | chars | depth (chars) | churn |
|---|---|---|---|---|
| 1787199018 | 15 | 23,724 | 22,941 | 3% |
| 1787184618 | 14 | 23,987 | 20,493 | 15% |
| 1787206218 | 14 | 21,085 | 19,465 | 8% |
| 1787172913 | 04 | 23,245 | 16,593 | 29% |
| 1787191818 | 17 | 23,340 | 16,428 | 30% |
| 1787206218 | 09 | 20,215 | 16,359 | 19% |
| 1787165713 | 05 | 24,103 | 15,913 | 34% |
| 1787191818 | 02 | 19,531 | 15,481 | 21% |
| 1787199018 | 16 | 25,622 | 15,146 | 41% |
| 1787172913 | 21 | 22,098 | 14,921 | 32% |
| 1787184618 | 07 | 21,408 | 13,831 | 35% |
| 1787184618 | 11 | 8,626 | 8,317 | 4% |
| 1787191818 | 07 | 10,658 | 6,286 | 41% |
| 1787177418 | 16 | 10,156 | 6,190 | 39% |
| 1787172913 | 01 | 5,610 | 4,353 | 22% |

**Measured depth (close-read set, n=15, over-weighted toward the long tail):**
- p50 **15,481 chars ≈ 3,870 tok**
- p90 **20,493 chars ≈ 5,120 tok**
- max **22,941 chars ≈ 5,735 tok**

**Mean post-diagnostic churn fraction: 0.249 (24.9%).** Range 3%–41%.

**Corpus-projected depth** (mean retention 0.751 applied to the full-corpus
`reasoning_chars` distribution, since the close-read set is long-biased):
- p50 ≈ **11,540 chars ≈ 2,885 tok**
- p90 ≈ **15,835 chars ≈ 3,960 tok**
- max = the directly measured 22,941 chars ≈ **5,735 tok**

**Supported budget: ~6,000 reasoning tokens covers the deepest observed diagnostic in this
batch; ~4,000 covers p90.** Recommend **8,000** as the development setting (generous headroom
over the measured max, per the no-arbitrary-chokehold rule). Nothing in this corpus justifies
more: the longest trace in the batch (25,622 chars) reached its last novel diagnostic at
15,146 chars and spent the remaining 10.5k re-litigating the PLAN line and re-running the same
combat arithmetic three times.

**Caveat on the number.** Because `reasoning_budget_hit` is absent, this measures where the
model *chose* to stop being novel, not where a cap bit. The cap currently in force is not
binding and cannot be inferred from these logs. Ship the flag before the next wave.

---

## JOB 1b — Trace-level pathologies (budget-relevant structure)

Measured across all 91 traces:

| pathology | measure |
|---|---|
| **Reply-protocol tail** — chars after the FIRST mention of the PLAN-line conditional | **p50 44% · p90 67% · max 96% · mean 45%** of the trace |
| **Answer-format tail** — chars after the FIRST mention of `exactly as listed` / `CHOICE:` format | p50 43% · p90 85% · mean 43% |
| **Oscillation** — `Wait`/`wait,` pivots per trace | **p50 18 · p90 29 · max 38 · mean 18.7** |
| Traces that litigate the PLAN-line conditional at all | **88/91 (96%)** |

**This is the single largest budget consumer in the batch.** Roughly **half of every trace,
by character count, sits after the model first raises "ONLY IF your plan changed."** Most of
that tail is churn, but not all of it — see finding F1: a real, repeatedly-verbalized ambiguity
lives in that instruction, so trimming it needs a prompt fix, not a budget cut.

Sub-pathologies:
- **Terminal re-verification loop.** 1787199018 s16 re-derives the identical Grazer-block
  damage table three times at offsets 5,613 / 8,109 / 22,042 with zero new conclusions.
- **Decision flip-flop.** 1787191818 s02 flips Forest→Swamp→Forest→Swamp across 13k chars on a
  turn-2 land drop, closing at 19,531 chars for a two-option choice.
- **Guide-rule oscillation.** 1787191818 s17 re-parses one ambiguous guide sentence five times
  (offsets 4,956 / 8,357 / 12,749 / 16,176 / 22,800), reaching the same answer each time.

---

## JOB 2 — Development findings

### F1 · The PLAN-line conditional is genuinely ambiguous, and it costs ~45% of every trace
**Severity: high (budget) · 88/91 traces**

The instruction pair `LINE 2, ONLY IF your plan has changed` + `If your plan is unchanged,
OMIT the PLAN line entirely` has no defined behaviour for two states the game reaches
constantly. The model verbalizes both:

1. **No prior plan (every mulligan / first decision).**
   > `1787165713 seq=1 @11547`: *"Since this is the very first decision, there is no 'last stated plan'. ... The instruction 'ONLY IF your plan changed' is ambiguous on initialization."*
   > `1787177418 seq=1 @7996`: *"If I interpret 'unchanged' as 'same as previous', and there is no previous, it's ambiguous."*
   > `1787184618 seq=1 @4777`: *"Since there is no prior plan, it's ambiguous if I should write one."*
   > `1787172913 seq=1 @4120`: *"If I am setting the plan for the first time, is it considered 'changed'? Yes, from null to something."*
   Three different traces resolve this three different ways (omit / include / include-to-be-safe).

2. **Plan executed but now factually stale.** The model repeatedly concludes it *must* write a
   PLAN line to correct the record, in direct tension with the OMIT rule:
   > `1787165713 seq=5 @20537`: *"If I don't [write one], the old one persists. The old one says 'cast Mox Jet'. If I don't write a new plan, the next turn might expect me to cast Mox Jet again (which is impossible). So I **must** write a new plan to correct the record."*
   > `1787199018 seq=9 @15129`: *"The old plan says 'In Main Phase 2, cast Elite Spellbinder'. Next turn, that won't make sense. So I should probably update the plan to avoid confusion for the next turn. But strict reading: 'If your plan is unchanged, OMIT'."*
   > `1787206218 seq=9 @18612`: *"If I don't write a new plan, the system might assume I attack with Mamba (which is summoning sick). So writing the new plan is crucial."*

**Fix direction**: define the null-plan case explicitly, and replace "changed" with an
observable test (e.g. "write PLAN whenever your last stated plan names an action you have
already taken or can no longer take"). This is the highest-leverage budget lever in the batch.

### F2 · The "PLAN: name only cards in the numbered choices" rule collides with land-drop-only nodes
**Severity: medium · verbatim in 1787184618 s14**

At a land-drop decision node the numbered options are lands only, but the model's plan
necessarily names the spell it intends to cast next:
> `1787184618 seq=14 @19999`: *"Does this mean I can't mention Mordor Muster in the PLAN line because it's not in the numbered choices? ... The restriction 'check that it appears in the numbered choices' applies to the action I am choosing right now, not necessarily the PLAN description of future turns."*

The model resolves it correctly but only after ~1.5k chars. The rule needs to be scoped to the
CHOICE line explicitly.

### F3 · Casting-decision menus offer no land-drop option, and the model invents an unmodelled action
**Severity: high (correctness risk) · 39/91 traces mention "Play Land"; ≥5 traces reason at length about its absence**

When a Main-Phase-1 node presents only `Cast X / Cast nothing`, the model believes it still
has a land drop available and *assumes it happens implicitly*:
> `1787165713 seq=5 @18436`: *"In these text interfaces, usually you assume you play a land if you have one and haven't played one this turn, unless instructed otherwise. I will assume I play Great Furnace (Artifact Land) before casting Cathodion."*
> `1787172913 seq=4 @13658`: *"I should play Tolarian Academy. But I can't select 'Play Tolarian Academy' from the numbered list. ... So I will cast Mox Sapphire. Then I can play a land afterwards **if the system allows**."*
> `1787184618 seq=11 @8247`: *"I should play that Swamp first (implicit action), then cast Swarming."*

The model's mana math for the *chosen* spell is then computed against a land count that
includes a land it never actually played (1787165713 s5 @9473: *"Then I have mana to cast
Cathodion"*). The decision happened to be legal, but the reasoning is built on a false board.
**The prompt should state at each casting node whether the land drop is still available and
whether it will be offered separately.**

### F4 · `Mana available: N total` and `| Already in pool: {…}` contradict each other
**Severity: high · the `Already in pool` clause appears in only 2 of 91 prompts and caused deep confusion in BOTH (2/2)**

> `1787177418 seq=16 @8988`: *"'Mana available: 2 total (two untapped sources … colours you can make: {u}{w}) | Already in pool: {g}{g}{w}'. This is contradictory. 'Available: 2 total' usually means total mana you can spend. 'Already in pool: {g}{g}{w}' means 3 mana. If I have 3 mana in the pool, I can cast {2}{W}. If I only have 2 total available, I cannot."*
> `1787177418 seq=16 @2238`: *"The 'Already in pool: {g}{g}{w}' line is very strange if I haven't cast anything yet this turn. Maybe it's a glitch in the prompt generation."*
> `1787206218 seq=9 @1315`: *"This looks contradictory or implies I have mana in the pool already. Usually, mana empties at end of turn."*

Compounding it: the colours listed by the two clauses **disagree** (`{u}{w}` available vs
`{g}{g}{w}` in pool). The confusion consumed ~65% of 1787177418 s16. Whatever the emitter
means, the two clauses must be reconciled into one statement of spendable mana.

### F5 · The mana line does not account for variable-output lands (Tolarian Academy)
**Severity: high (the model deliberately overrides an authoritative line)**

The prompt's mana line reports a *source count*; Tolarian Academy produces {U} per artifact.
The guide says `DO NOT RE-DERIVE YOUR MANA`, yet the model must, and does:
> `1787172913 seq=21 @9341`: *"The prompt says 'Mana available: 3 total'. This is confusing. Usually Tapping Academy generates mana. If I tap Academy, I get 9 mana. Then I can pay {7}. … I can cast it."*

It then committed a PLAN to cast a `{7}` spell on a turn the engine reported 3 mana. This is a
direct conflict between an engine-authoritative line and a card in the deck. Either the mana
line must resolve Academy, or the guide's do-not-re-derive rule must carve it out.

### F6 · Battlefield/count-line contradictions the model must reconcile by hand
**Severity: medium · 7/91 traces contain an explicit "contradicts / why does it say" reconciliation**

- **Artifact count** (`1787172913` s7, s21): battlefield lists 10 permanents, `Artifacts in
  play: you 9`.
  > `seq=21 @13938`: *"That's 10 items. Why does it say 9? Maybe Mox Jet or Mox Sapphire doesn't count?"* — resolved only by recalling a guide sentence that Tolarian Academy is not an artifact.
- **Hand vs log** (`1787199018` s9, s16; `1787191818` s22): a land the log says was played is
  still listed in hand.
  > `1787199018 seq=9 @2828`: *"This contradicts the Game Log which said 'Your Barkchannel Pathway: hand -> battlefield'."*
  > `1787191818 seq=22 @14725`: *"Log: 'Your Nethroi, Apex of Death: stack -> battlefield'. Current Situation: 'Your hand: … Nethroi, Apex of Death …'. **This is definitely a bug in the provided text state vs log.**"*
- **Opponent lands rendered tapped on the player's own turn** (`1787206218` s14):
  > `@3036`: *"Wait, if it's my Turn 9, the opponent's lands should have untapped at the start of my turn (during my Untap step). However, the 'Current Situation' explicitly lists them as '[tapped]'."*
  The model then reasoned about opponent instant-speed interaction from this (probably wrong)
  state. Worth checking whether the renderer snapshots opponent tap state at end-of-their-turn.

### F7 · `Your battlefield (creatures: N)` labels a list of ALL permanents — 84/84 mismatch
**Severity: medium (latent trap; only 1/91 traces flagged it, but it feeds the recount loops)**

Every single battlefield line in the batch has `(creatures: N)` where N ≠ the number of
semicolon-separated items that follow (lands, enchantments and artifacts are in the list).
> `1787206218 seq=17 @7427`: *"The board state lists 'Branchloft Pathway' and 'Barkchannel Pathway' under 'Your battlefield (creatures: 1)'. That seems like a formatting quirk in the log (listing lands under creatures?)."*

Rename the label or split the list.

### F8 · Stale-plan annotation fires when the plan is NOT stale
**Severity: medium · the annotation appears and is echoed in 19/91 traces**

> `1787206218 seq=3 @11425`: *"'(note: the actions your plan names are no longer among the options available right now …)'. This note is a bit confusing. It says 'actions your plan names are no longer among the options'. My plan said 'Play land'. **Option 2 is 'Play Land'. So it IS among the options.** Maybe the note is generic boilerplate."*

A false-positive on this annotation trains the model to discount it. The matcher should be
checked against option text, not just card names.

### F9 · Strategy-guide defects the traces expose

- **Internally ambiguous conditional** (`1787191818` s17, five re-parses):
  > `@8357`: *"'When two or more Cast lines are offered AND your life is 10 or less AND the opponent shows more creatures than you: [Rule A]. Above 10 life, or when you are even or ahead on creatures, cast Pollywog first'. This phrasing is slightly ambiguous."*
  The `AND`-guarded exception followed by an `OR`-joined default does not compose. The model
  ultimately followed the letter (cast the 1/3 Pollywog) over a 5/5 deathtouch blocker while
  facing three infect creatures — a plausible mis-play caused purely by guide grammar.
- **Rule conflict the guide never resolves** (`1787172913` s4, four re-parses): "cast {0}
  artifacts always first" vs Glaze Fiend's *whenever another artifact enters* trigger.
  > `@6729`: *"Therefore, casting Glaze Fiend before Mox Sapphire is better for Glaze Fiend's stats this turn. **BUT**, the Strategy Guide says 'cast artifacts cheapest first'."*
  The model's game analysis is correct and the guide's blanket rule is wrong for this pair;
  it followed the guide and gave up the pump.
- **Threat-vs-value conflict** (`1787184618` s7): "Mordor Muster — cast on sight" vs "when your
  main phase offers BOTH a creature or threat AND a lower-impact value spell, cast the THREAT".
  Both name the same decision; the model oscillates five times.

### F10 · Invented / assumed card facts (6/91)
- `1787199018 seq=16 @1280`: *"Elite Spellbinder (Human? Text doesn't explicitly say Human, but usually yes in this set context … Let's assume yes)"* — creature type invented to justify Katilda mana.
- `1787199018 seq=15 @4798`: Luminarch Aspirant described as *"1/1 Flying, Lifelink"* — neither
  keyword is on the card as rendered; the model then reasoned about lifelink value.
- `1787177418 seq=1 @4183`: *"'Lair of the Hydra … If you control two or more other lands, Lair of the Hydra enters tapped.' This looks like a typo in the prompt's provided card text compared to standard MTG rules"* — **check this card's script; the model is right that real Lair of the Hydra has no such clause, and the printed text may be an inverted `unless`.**
- `1787172913 seq=21 @14968`: *"Tolarian Academy does NOT generate mana based on itself, but based on other artifacts"* — reaches the right number by the wrong rule.

### F11 · False belief about legal hand size (6/91)
> `1787191818 seq=2 @1564`: *"I drew 1 card on Turn 2. That makes 8 cards in hand. **This is unusual for standard MTG (usually 7 max), but I must follow the game state provided.** Perhaps it's a specific variant or just a data quirk."*

The render is correct (no cleanup discard until end of turn); the *model* is wrong. Gold
example of a false belief that costs budget but not correctness. No prompt fix needed — but if
budget matters, one clause ("hand size is only enforced at end of turn") would kill it.

### F12 · A pre-resolved ETB is presented as an open choice
> `1787191818 seq=4 @4422`: *"The log says 'Your Arboreal Grazer: stack -> battlefield'. This suggests it already happened. Why is there a choice?"*
> `@11658`: *"This sequence is weird. It lists the stack -> battlefield transition **before** asking me to choose. This suggests the game engine might be showing me what will happen if I choose, or it's a glitch in the log display."*

Also `1787177418 seq=17 @8092`: *"'1 (top): your Day [spell]'. This is weird. It seems the prompt thinks Brutal Cathar is a 'Day [spell]'."* — the stack renderer appears to be naming a day/night state token as a spell.

### F13 · Negative power renders (5 prompts) — NOT a defect
`Arboreal Grazer {g} (-2/1) (printed 0/3) [reach] [counters: 2x -1/-1]`. Rules-correct and the
`(printed X/Y)` + `[counters: …]` annotation carried it: **0/91 traces expressed confusion.**
Recording it as a *working* emitter, since the OLD-register rewrite may be tempted to clamp it.

---

## Register notes for the docketed rewrite

New emitters/shapes observed beyond the known list (zone arrows, question echoes,
resolved-vs-countered ambiguity, counter-source gaps, dungeon room position):

- `| Already in pool: {…}` appended to the `Mana available:` line — **new, and 2/2 confusing** (F4).
- `(printed X/Y)` + `[counters: Nx -1/-1]` P/T annotation — **new, and working** (F13).
- `{right now: Army 1/1 -> 2/2}` inline option annotation on amass casts — **working**; parsed
  correctly and used in every trace that saw it, no confusion.
- `[summoning sick - cannot attack this turn, but CAN block]` — **working**, and it is the cue
  the model uses to confirm whose turn it is.
- `(note: the actions your plan names are no longer among the options available right now …)` —
  fires false-positive (F8).
- `Your battlefield (creatures: N):` label — 100% count mismatch (F7).
- No dungeon-room, no counter-source-gap, no resolved-vs-countered instances appeared in this
  batch's 91 traces.
