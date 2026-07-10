# AI Opponent Redesign — What We Need To Solve
Working notes, 2026-07-06. Design anchor for the LLM-opponent-feature work.

**STATUS 2026-07-09: the P0-P4 queue is SHIPPED or dispositioned** (each section carries its
own status line). Remaining tail: sibling-cost canPay + engine-wide 601.2c (engine track),
skip-call-when-only-pass (measure first), decision pre-fetch + the DecisionRequest->Action
contract (the architectural end-state). NEXT: fresh hints-free selfplay corpus over the locked
7-deck pool with the new stack -> cold-check decision quality -> wave2 guide review +
revalidation (section at bottom).

## Core diagnosis (the unifying frame)
`AIPlayerGPT extends AIPlayerBaka` — the LLM was **grafted** onto the heuristic AI's decision plumbing, which was built for **deterministic logic operating on internal state it never had to externalize**. There is no interface designed for a *reasoning* AI. Every "representation" finding below is a symptom of that graft, not an isolated bug.

## The target: an AI decision interface (a DecisionRequest -> Action contract)
Mirror image of the **BoardState contract** we built for the `web/` frontend (which decoupled render from game logic). The AI side needs its analog: the engine emits a self-contained **DecisionRequest**; the agent returns an **Action**. This decouples the AI's decision interface from Baka's heuristic internals; any AI (heuristic OR LLM) consumes the same contract.

The contract must provide:
- **Self-contained decision context** — the deciding fact is ON each option (effect, current magnitude, benefit-not-just-cost). No recall from a far-away deck blob.
- **Rules-valid action sets only** — no illegal casts / targetless targeted spells in the set.
- **Free choice, not veto** — the agent picks the action; the heuristic isn't pre-choosing it.
- **No heuristic-internal leakage** — Baka's efficiency scores are not the agent's business.
- **Compact, correct history** — current state sent fresh once + what *happened* + the agent's own prior decisions/plan; NOT a stack of stale full board snapshots.

## Work list (prioritized)

### P0 — History / prompt representation  [SHIPPED 2026-07-09, commits 9192b0c30+]
Implemented as head/tail: static system prompt + one user message = append-only game
NARRATION (turn headers always; lazy phase markers; events + own decisions; opening hand
collapsed to one line; tokens narrate as "created ->"), then ONE fresh board, the model's
last PLAN line, and the legal choices. Reply protocol: choice then PLAN:, model told all
else drops from context (complete-plan restatement required). Ask/dedupe caches keyed by
state+question so narration growth can't thrash them. Gotcha for posterity:
WEventPhaseChange->to->player is INVERTED from turn 2 on (ring builds next turn against an
already-flipped id) - turn ownership must read observer->currentPlayer.
Original finding (kept for context):
Current impl (`AIPlayerGPT.cpp`): a **windowed conversation transcript**. `buildRequestBody` (728-732) sends system + the running `(decision-prompt, reply)` pairs + the current prompt. After each reply the prompt+reply are appended (889-890). Window (896-897): `while (mMessages.size() > 41) erase(begin+1, begin+3)` = keep system + the most recent **20 exchanges**. Each historical "user" turn is a **full board serialization**; the code's own comment (895) admits "old states are superseded by the snapshot we resend." So up to ~20 STALE full snapshots stack ahead of the current one -> attention dilution for a weak model, token bloat, and an unstable prefix that defeats prefix caching. **FIX:** send current state fresh once + a compact history (events + own decisions/plan), not full stale re-dumps. This is a first-class contract decision: separate CURRENT decision context from a COMPACT running history (they are currently conflated).

### P1 — Decision presentation  [MOSTLY SHIPPED 2026-07-09; state-computed magnitudes remain]
Shipped: rules-text snippet on target options AND on activated-ability action lines (the
benefit rides the option - fetch shows its search text next to [cost]); type tags on
non-creature targets/board lines; LIVE keyword set (granted/lost) on board lines, combat
declaration lines, and damage-order options; artifact counts in the state block; identical
option lines DE-DUPED (fetch: 13 -> 1); mana single-brace fix ({{1}{u}} was a wrapper bug);
empty pool renders "(none)"; truncated plans trimmed to the last complete sentence.
MAGNITUDES SHIPPED TOO (2026-07-09, later): dynamic (non-numeric) amount expressions after
known effect verbs (lifeleech/damage/life/draw/prevent) in the card's auto= script are
evaluated at menu time with the engine's own WParsedInt and annotated on cast/action lines
as "{right now: drains N}". Guards: static numbers skipped (already in rules text), "x"
skipped (unknowable pre-announcement), "rand" skipped (rendering would draw from the game
RNG). Live-verified: Gray Merchant's cast option tracked devotion 0 -> 3 -> 5 across a
probe game. X-spells stay covered by the X-announcement menu.
Original finding (kept for context):
Option lines hide the deciding fact -> the weak model picks near-arbitrarily. FIX in `serializeGameState` / `describeAction`:
- Target options carry the card's **effect / role tag** (threat/removal/counter/ramp/draw/land). Bare names -> arbitrary discard/removal picks (deck133: stripped a mana rock + mana dork over a card-advantage engine).
- State-computed payoffs carry **current magnitude** (Gray Merchant "drains 2 now"; X-spells; "for each" effects; overrun).
- Sacrifice-for-value actions show **BENEFIT not just cost** (fetch crack shows only `[cost: Life,Tap,Sacrifice]` -> qwen reads pure downside, never cracks; deck135).
- **Equipment attachment** on the board line (which creature it's on / equipped flag) — currently invisible, can't tell a working Cranial Plating from a spare (deck110).
- **Artifact-count / metalcraft** status line (deck110).
- **Static/keyword abilities** on the board line (Bloodghast "can't block"; deck133) — lower priority.

### P2 — Rules-valid action sets  [PARTIAL 2026-07-09: GPT-seam 601.2c filter + blank-template fallback shipped]
Shipped at the GPT contract seam (strangler step): FindCardToPlay's cast menu now drops
spells that REQUIRE a target when none exists (mandatory single target with zero candidates;
exactly-N with fewer than N) - "up to N", untargeted, and any-target spells untouched.
Validated with probe decks: removal offered 16/16 with creatures present, never offered
creatureless (and no model call fires when the only castable was filtered). Blank
"Choose the target for " prompt: tc->source is a NAMELESS fake card for granted/inner
abilities (Liliana +1 discard); prompt now falls back to the waiting action element's menu
text ("discard"), else "this effect" (fallback path not yet observed live - watch corpora).
REMAINING (engine-level, non-LLM track candidates): 601.2c in MTGPutInPlayRule itself
(humans + Baka still get offered illegal casts - verified: the rule checks mana/timing only);
sibling-requirement canPay (Crackleburr, CS-002 residual); ishuman/genrand card-data gates
(CS-017) + emerge/offering ishuman gate (CS-003) -> route to the interactive path for GPT.
Original finding (kept for context):
- **ILLEGAL CASTS OFFERED (bug).** Targeted spells with no legal target offered as legal (Go for the Throat vs all-artifact board; Essence Scatter on empty stack; Downsize into creatureless board). MTG rule 601.2c: a spell that REQUIRES a target can't be cast with no legal target; spells with NO target or "up to N" CAN. ⚠ Do NOT over-filter — casting a legally-castable spell for ANOTHER CARD'S TRIGGER (Guttersnipe/Young Pyromancer/prowess/storm) is legitimate; never block a spell just because its own effect looks null. Fix the rules engine's legal-move set (affects Baka too, not just GPT).
- **BLANK effect-name target prompt (bug).** `"Choose the target for "` with the `{name}` never substituted, offering only the caster's own creatures (game 1783314190 rec 17). Template-substitution failure; trace the emitting seam.

### P3 — Free-choice card play  [RE-SCOPED 2026-07-09: spell side ALREADY free-choice in code]
Reality check against the code: FindCardToPlay's SPELL side already offers one free choice
across every castable card (hand/graveyard/exile/commandzone, alt-costs listed as separate
modes), validated via aiForcedCandidate. Remaining veto/graft surface: (a) LANDS stay
propose-then-veto (deliberate - dropping a land is nearly always right; Play listed first
since P4); (b) validation-failure fallback hands the whole cast to Baka (rarer now that the
601.2c filter removes the targetless-pick cause); (c) the full DecisionRequest->Action
contract (decoupling decisions from clickstream / the duplicated Act body) = the
architectural end-state, a dedicated refactor, not a queue item.
Original finding (kept for context):
`FindCardToPlay` is **propose-then-veto**: Baka chooses the card, the LLM only approves/holds. Card **selection** is Baka's -> on the decks Baka most fumbles, the LLM is capped no matter how good the guide is. The full free-choice card-play rewrite removes this. (NOTE: in the pool7 corpus the finishers WERE offered — Rakdos's Return offered 76x, qwen passed — so THAT passivity was real qwen behavior, not Baka hiding cards; but veto-only remains the structural ceiling.)

### P4 — Prompt hygiene / cleanups  [SHIPPED 2026-07-09 except skip-when-only-pass + verb normalization]
Shipped: hints leak removed at the ROOT (member, env var, config key, GUI row all gone -
the confound no longer exists in any configuration); option ordering fixed (land veto lists
Play FIRST, cast menu lists "Cast nothing" LAST, multi-target "Done" goes LAST); de-dup,
mana normalization, phase-collapse landed earlier with P0/P1. Live-verified vs local
llama.cpp: 0 hint leaks, Play-first picked 6/6. NOT done: skip-model-call-when-only-pass
(single-candidate priority asks still call; measure first), verb normalization (the P1
card-text snippet largely dissolves it).
Original list (kept for context):
- **Drop the `(heuristic score N)` leak** (`mShowHints`). Baka's currency in the reasoning model's prompt is a crutch/anchor; removing it dissolves the hints confound at the ROOT (not just via a `HINTS=0` flag).
- **De-dup identical option lines** (fetch -> 9 byte-identical basics; deck133/135).
- **Normalize mana-cost rendering**: options show `{{1}{u}}` (double-brace lowercase) vs the taught `{2}{R}` (deck131).
- **Collapse event-log phase-spam** (`Phase: ---` placeholders, untap->draw runs; deck131/133/135).
- **Option ordering**: put the usually-correct option FIRST and traps LAST (self-burn target listed second), and do NOT list "pass/Hold" first — the model favors option 1 (positional anchoring: Prism Ring always picked option 3; "Hold" first likely drives the 82% pass; deck131/140/109).
- **Skip the model call when only "pass" is legal** (wasted inference; games are inference-bound; deck135/109).
- **Normalize inconsistent verbs** for the same action (fetch: "search basic land" vs "Put in Play"; deck135).

## Pacing principle for GUI/UX work  [user directive, 2026-07-09 — AUDITED same day]
AUDIT RESULT: the act throttle is timer=0.1s (0.07 fast mode) per decision cycle, so the
throttle-then-ask serialization costs <=0.1s against multi-second inference - negligible,
and the engine warns going lower skips phases. The async worker already runs rendering/
animation CONCURRENT with the in-flight request ("opponent is thinking..."), which is the
directive's core shape. No throttle surgery warranted. The REAL future lever is decision
PRE-FETCH - fire the ask for a decision that is already knowable before the engine's tick
reaches the AI (e.g. during the player's action resolution or transition animations) -
which belongs with the DecisionRequest contract work (P3 end-state).
The engine paces itself with real-time padding (the AI act throttle in
`AIPlayerBaka::Act` — max ~14 actions/sec — plus animations and transitions).
When the opponent is a model, every padding must work to **hide inference
latency, not extend it**: fire the model call at the earliest moment the
decision is knowable and let the pacing/animation run CONCURRENT with the
in-flight request, so the padding absorbs latency the player would otherwise
feel. Never sequence padding after the reply. (The "opponent is thinking..."
notice is the existing example of the right shape; audit the act-throttle and
any transition timers for the wrong shape — throttle-then-ask serializes
padding + inference.) For testing, `WAGIC_FASTCLOCK` strips this padding
entirely — headless corpora are bound by real work only.

## The ishuman lens  [user directive, 2026-07-09 — MECHANISM SHIPPED same day]
SHIPPED: `Player::isInteractiveAI()` (default false; AIPlayerGPT returns true while an
endpoint is live) + the single card-data evaluation site (WParsedInt "ishuman") now treats
an interactive AI as human. ALL ~151 ishuman gates in the primitives route the GPT player
onto the interactive lines in one move: emerge/offering/entwine alternative casts (CS-003)
AND the genrand dice-roll trigger lines (CS-017). Validated live: Aquastrand Spider's graft
may-menu (previously ishuman==0 + genrand2 dice) reached the model as a real choice.
Baka fallback keeps the dice lines (gate requires a live endpoint); suite unaffected.
Any engine path gated "for the human" (`ishuman` restrictions, human-only menus) must be
re-evaluated for the LLM opponent: does it have access, and should it? Those gates encode
Baka's limitations, not the game's rules. First instance: emerge/offering casts (CS-003).
Second instance (CS-017, 2026-07-09): the gates also live in CARD DATA — e.g. Cytoplast
Manipulator's graft lines carry `compare(ishuman)~equalto~0,compare(genrandN)~equalto~0`
restrictions: for any non-human player (including AIPlayerGPT) the graft counter-move is
decided by a DICE ROLL baked into the script, and the LLM never sees the choice. Sweep the
primitives for `ishuman`/`genrand` gates when building the P2/P3 action surface.

## Methodology
- Future test corpora: **`WAGIC_GPT_HINTS=0`** — strip Baka's scores so we measure qwen+guide, not qwen-following-Baka.
- Migration = **strangler pattern**: move ONE decision seam onto the clean contract at a time; suite 915 as the net. NOT a big-bang rewrite. Same approach as the parked engine/render-decoupling note, aimed at the AI interface.
- The guides are real and worth keeping, but a guide **cannot repair an interface that hides the deciding fact** — P1 (and P0) come before pouring more effort into guide wording.

## What the strategy-design process produced (staged, NOT promoted to live Res/)
Under `projects/mtg/strategy-design/`:
- `wave2/general-strategy.txt` — the NEW general guide (passivity fix; instant-vs-non-instant timing with 2nd-main default; bluffing paragraph cut; choose-by-impact; complete-synergies; answers-vs-reach; lethal-check; reworded cost paragraph). ~31 lines.
- `wave2/strategy-writing-skill.md` — the unified strategy-writing skill (incl. "instantiate, don't restate").
- `wave1/deckN/strategy.txt` (7) — first-draft per-deck guides (44/135/140/131/110/109/133).
- `archive/wave1-skills/` (7) — the divergent per-deck skills, wave-one designs.
- `wave1/deckN/{general-suggestions,findings}.md` — inputs + the findings backlog.
Promote to `bin/Res/ai/gpt/system_prompt.txt` + `deckN_strategy.txt` only AFTER user review (`git add -f` for Res; NO AI-attribution). ⚠ These were generated with hints ON / veto-only card-play, so re-validate under `HINTS=0` once P-items land.
