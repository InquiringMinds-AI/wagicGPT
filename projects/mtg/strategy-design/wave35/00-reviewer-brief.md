# Wave-35 reviewer brief (guide-review cycle; shared context for all 7 deck agents + synthesis)

Twentieth cycle. Pool UNCHANGED: 146, 139, 152, 158, 105, 36, 116. This is the FIRST
guide-review wave since wave-33 — wave-34 was a calibration corpus (protocol/budget), and
wave-35's corpora were run as a MODEL-TIER A/B. OWNER DIRECTIVE for this cycle: **all
three corpora are reviewable material.**

PROCESS RULE: NEVER kill any wagic process (pkill FORBIDDEN); kill only your own PIDs.
Everything here is READ-ONLY except your own output files.

## The three corpora (same 21 matchups, same guides, same budget 6000, thinking ON)

| arm | run dir(s) | pilot | notes |
|---|---|---|---|
| A | `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260820-192210` + `matchups-20260821-022400` (146v36 rerun) | Qwen3.6-35B-A3B PrismaQuant 4.75-bit | RETIRED quant (proven quant damage) |
| B | `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260821-033000` | Qwen3.5-122B-A10B | zero comprehension failures; plan-inertia + budget-default classes |
| C | `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260822-111102` | **Qwen3.6-35B-A3B-FP8 — THE STANDING OPPONENT from now on** | guides you write are FOR THIS PILOT |

All 63 games ran to NATURAL completion (no caps — caps are abolished, owner ruling).
Translog fields: seq, turn, kind, prompt, reply, reasoning (native thinking trace, the
audit surface), chosen_text, my_life, opp_life, fallback fields.

**Weight the arms accordingly**: arm C is the pilot your guide must teach — its failure
shapes are the ones guide text can and must catch. Arm B shows what a stronger pilot does
with the same guide (its justified audibles mark places the guide UNDER-SPECIFIES). Arm A
is background evidence only — do not tune a guide to quant-damage artifacts.

## OWNER RULINGS in force (binding)

1. **Review criterion (2026-08-22, verbatim)**: "mtg has alot of random chance to it. win
   rate is irrelevant. the question is, how well do the models follow the provided
   strategies, and if they diverge from the provided strategies, is it a good choice to
   do so at that time." Win tables are context, never a verdict on a guide.
2. Reply protocol: ANSWER + concise changed-only PLAN, native thinking ON in the dev
   loop. Do not propose reply-format changes.
3. Guides teach THE DECK'S SPECIFIC ENGINE, not general Magic; jank engineering — make
   the bad card reliably win.
4. Per-wave owner prompt review: DONE for this wave (turn-30 specimen delivered).

## Pre-digested findings — READ YOUR SEAT'S FILES FIRST (cite, don't re-derive)

`projects/mtg/strategy-design/wave35-review/`: `deckN-tier-review.md` (arms A+B, all 7
seats), `deckN-fp8-review.md` (arm C, seats 105/116/158/36 only), `00-synthesis.md`
(+FP8 addendum: cross-seat verdicts, failure fingerprints, consolidated docket).
GUIDE BUGS already identified there — your revision MUST resolve your seat's:
- deck158: "their deaths feed the Army" is FALSE (engine-verified; only Vanguard amasses
  on death). Arm C REFUTED it from card text; arm A obeyed it into two misplays.
- deck139: "ALWAYS mutate over" is wrong when the cast card is smaller than the host;
  Dryad-on-curve vs cheap-mutate priority conflict.
- deck152/105: the generic blockers hint ("block only where the trade favors you")
  contradicts deck105's numeric ≤13→BLOCK rule; guide-side scoping where applicable.

## ⚠ INTERIM CHANGES since the wave-33 binary (bdb30d8e7 → 9833cf39e/f0689f56f) — attribute deltas correctly

The prompts these corpora carry are HEAVILY revised vs wave-33. A behavior delta vs the
wave-33 corpus is NOT a pilot property until checked against:
- **Wave-34 protocol batch**: native thinking (budget 6000), reply = answer + changed-only
  PLAN, two-phase budget rescue, hidden-trace handling.
- **Wave-35 lane A (protocol)**: three-case PLAN rule; stale-plan caveat rebuilt (action-menu
  gate, pre-comma name match); echo = number + short name; MP2/instants-castable facts on
  combat/priority asks.
- **Wave-35 lane B (narration register)**: past-tense declarative GAME LOG ("You drew X",
  "Opponent cast X targeting Y", countered-vs-resolved distinct); graveyard/exile digests
  ride the trim marker; cast lines carry targets.
- **Wave-35 lane C (render truth)**: hand copy-numbering "(copy R of N)"; honest battlefield
  header; per-source mana line; land-drop statement; alt-cost divergence note; amass
  plus(N) preview; poison zero-state when poison live.
- **Wave-35 lane D (engine)**: selfAsCostPayment (Scarecrone class fixed); budget_hit gated
  on finish_reason; reasoning_degenerate metric.
- **Livelock fixes** (mid-corpus B/C relevant): deterministic priority-menu order; 6-drop
  stale-streak breaker (class `stale_livelock`).

## Per-seat focus (beyond your seat's standing items)

- **deck105**: the numeric block rule executed well everywhere except arm A's prior-override
  pair — is the rule's TEXT hardenable ("believe the printed outcome line") guide-side?
- **deck139**: mutate over/under REWRITE (the G-bug); arm B's mutate-under audible is the
  correct play your guide currently forbids. Dryad/curve conflict resolution.
- **deck146**: arm B lost a won game to Agadeem-on-a-planeswalker despite your named do-not
  — is the do-not's placement/wording at fault, or was that pilot-owned? The Kaya -3
  decision-tree text validated verbatim at arm A — keep what works.
- **deck152**: plan-inertia (arm B cited stale PLAN text as reason twice) — can the guide
  say "re-derive from the board; the plan is a note, not a rule"? FA-sequencing text held.
- **deck158**: resolve the FALSE amass line (mandatory); best-blocker rule under pressure
  (arm A declined Vanguard at 4 life); toughness/threshold arithmetic support.
- **deck36**: recursion-target discipline (arm C recursed Cathodion; restriction never
  engaged — wording/placement?); sacrifice keep-list held at arm C — keep; the one-per-ask
  sacrifice paragraph exists but overruns persist (note for core, not guide).
- **deck116**: Rule 4 (Piper) QUOTED VERBATIM by arm C — that's the wording standard to
  replicate; the illegal-multi-block loss is harness-lane (re-ask docket), not guide.

## Layer routing (unchanged)

Engine/harness → `deckN/notes.md` with seq repros (the wave-36 step-1 docket feeds from
these + the tier reviews' consolidated docket). Guide → `deckN/deckN_strategy.txt` FULL
REVISED TEXT per **wave33/strategy-writing-skill.md** (current edition — HL1 hard gate,
HL5 input-side, HL6 count escalation, HL7 mulligan rung, HL10 instruments). Skill deltas
→ `deckN/skill.md`. Core/general-prompt → `deckN/general-suggestions.md` (28 consecutive
PASSes; bar unchanged). Rotation: UNSENTIMENTAL, but under ruling #1 rotation arguments
must be adherence/maturity-shaped, not record-shaped.

## Output location

`projects/mtg/strategy-design/wave35/deckN/` — findings.md (the review), the revised
guide, notes.md, optional skill.md/general-suggestions.md.
