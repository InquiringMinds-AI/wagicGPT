# Wave-57 lane H — the game-log window MECHANISM (D43 "measure"; the build half of D13)

Branch `w57-lane-H`, worktree `worktrees/lanes/w57-H`. Base: master at `4d4ea0bdf`
(wave-57 step-1 lane brief; >= `7ee3a51e8`).

Owner ruling D43 is one word — **measure**. So this lane ships a **mechanism behind a flag
that defaults OFF**, plus the A/B design the orchestrator runs. It does **not** change what
the AI seat is served today, and it does not decide anything.

---

## 1. What shipped

`WAGIC_GPT_LOGWINDOW` — an env lever read once per process:

| value | arm |
|---|---|
| unset / `full` | **default.** No window. The GAME LOG is today's, byte for byte, header included. |
| `kind` | the per-ask-kind budget. **Land drop**, **cleanup discard**, **empty-stack pass** and **display/menu** asks get the last **K** turns (K = 3); **casting, combat, targeting, blocking, reveal, pregame and anything unclassified keep the whole log.** |
| `kind:K` | the same with an explicit K |
| `N` (a bare positive integer) | the last N turns for **every** ask kind |
| anything else | **refused**, traced, and the window stays OFF — a typo in an A/B launch must not mint a silent third arm |

Files: `projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/include/AIPlayerGPT.h`. Every line
tagged `#W57-H (D43)`. The change is confined to (a) one pure block sitting immediately above
`assemblePrompt`, (b) four small member functions beside it, (c) the two lines inside
`assemblePrompt` that emit the GAME LOG, (d) three translog fields, (e) one line at each of the
seven ask seams naming that seam's ask class.

### The doctrine problem, and how the mechanism answers it

The append-only GAME NARRATION is the **only** memory the model has — there is no chat
transcript — so shortening it is a **deletion**, and the trust doctrine's rule is that a silent
omission is worse than a wrong line (the model confabulates rules into gaps). Hence:

* **Nothing is elided silently.** Every elision renders **one new line**, the first line of the
  log, carrying what the dropped turns held that no later line can restate:
  * both **graveyards** and both **exiles**, live at prompt time — the log's existing *zone
    duty* (the owner's 2026-08-19 doctrine: graveyard and exile are never in CURRENT
    SITUATION, the historical log is where they live). This is the brief's "cards each side has
    cast that are no longer on the battlefield but matter": recursion targets and
    exiled-with cards;
  * the **pregame record** lifted verbatim out of the elided prefix — opening-hand size and
    every mulligan outcome (the card list is cut at its colon; the SIZE is the carried fact);
  * the **exact count of turns elided**;
  * and, when the 24k trim had already eaten events before those turns, that fact too.
* **The header stops lying.** `GAME LOG (everything that has happened so far):` becomes
  `GAME LOG (the last N turns in full; everything before them is condensed into the first line):`
  the moment anything is actually elided. A true statement in the wrong scope is a lie.
* **The cut is on a turn boundary.** `=== Turn N - ...` at the start of a line is the only stable
  boundary; the kept text always begins at a header, so no turn is ever half-shown.
* **The summary line is deliberately NOT the 24k trim marker's literal** (`(...earlier events
  trimmed`) — the two coexist in one log and a census must be able to tell an elision from a
  trim.

Live sample (arm `4`, stub game, turn 35):

```
GAME LOG (the last 4 turns in full; everything before them is condensed into the first line):
(...the first 30 turns of this game are condensed into this line - graveyards now: you - empty;
 opponent - Lay Waste, Talisman of Impulse, Starstorm x2, Dwarven Blastminer, Stone Rain x3,
 Siege-Gang Commander x3, Molten Rain x2, Rorix Bladewing, Hammer of Bogardan, Spark Spray;
 exiled now: you - none; opponent - none)
=== Turn 31 - YOUR turn ===
...
```
(one line in the render; wrapped here.)

### Ask classification

Set by the seam, one line each, then refined by a **pure** classifier where the seam serves more
than one class:

| seam | class |
|---|---|
| `askModel`, question starts `Land drop: ` | `land_drop` — **budgeted** |
| `askModel`, every row inert (decline / hold / `Flip Side` / `Done` / `Pass`) | `display_menu` — **budgeted** |
| `askModel`, anything else | `unclassified` — full log |
| `chooseOrderedAction`, any real `Cast ` row | `cast` — full log |
| `chooseOrderedAction`, something respondable on the stack | `unclassified` — full log |
| `chooseOrderedAction`, empty stack and no cast row | `empty_stack_pass` — **budgeted** |
| `cleanupDiscard` | `cleanup_discard` — **budgeted** |
| `chooseAttackers`, `chooseBlockers` | `combat` — full log |
| `decideReveal` | `target_or_reveal` — full log |
| pregame bottoming | `pregame` — never windowed (the pregame frame is hand-only and renders no GAME LOG at all) |

**Deliberate narrowing vs D13, flagged.** D13's own ask is "`ask`/`priority` with an EMPTY
`ON THE STACK` -> last 3 turns", which would budget cast-bearing priority windows too. The lane
brief says casting asks keep the full log, so `kind` implements the **narrower** rule: a window
offering a cast is a casting ask whatever the stack holds. The stronger form of D13 is reachable
as the `N` arm, which budgets everything. Both are on the table for the A/B; nobody has to
re-derive them.

The respondable-stack test calls **`stackObjectIsRespondable`**, the same predicate
`serializeGameState`'s stack block calls — only the walk is separate, so a change to what counts
as respondable moves both.

### Keys are untouched — confirmed, not assumed

The ask cache, the priority dedupe/deadlock key and the async slot key are **board state +
question**, never the prompt:

* `askKey = serializeGameState() + tailStr` (priority) / `serializeGameState()|situationPrefill + tailStr` (`askModel`);
* `asyncSlotKeyOf(forceClose, turn, phase, mPromptTail, serializeGameState())`.

The window rewrites the **GAME LOG section of the user message only**. It does not touch
`tail`/`mPromptTail`, the option rows, `serializeGameState()`, the carried `PLAN:` block, the
plan-withdrawal/menu-diff clauses, or the consumed-decision echoes inside the kept window (those
are ordinary narration lines and ride the window unchanged). **No key input changes in any arm.**

### Translog

Every record now carries:

* `log_window` — the configured arm (`full` / `kind:3` / `4`), so the harvest verifies the arm
  **from the data** instead of trusting the launch environment;
* `log_window_kind` — this ask's class (`land_drop`, `cast`, `unclassified`, ...), so a record
  whose class was never budgeted is distinguishable from one that was;
* `log_window_turns_elided` — present only when > 0.

### Diagnostic

`renderProbeDump` (already compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`)
now also dumps the **assembled** prompt with its arm/class/elided stamp, so the byte-identity
claim is checkable by diffing a run's dumps. Nothing new is added to a release build.

---

## 2. Gates

Hermetic `make -B` baseline re-confirmed on this worktree **before** any edit
(`~/.gatelogs/w57-H-base.log`), then the post-change gate (`~/.gatelogs/w57-H-v4.log`):

| gate | baseline (this worktree, pre-change) | after |
|---|---|---|
| PARSETEST | **2736 / 0** | **2783 / 0** (+47 new cases) |
| suite, `WAGIC_TESTSUITE_THREADS=1` | **1241 (0 failed)**, **55 AI (0 failed)**, 0 timeouts | **1241 (0 failed)**, **55 AI (0 failed)**, 0 timeouts |
| build | exit 0 | exit 0 |
| `git diff \| grep -c U+FFFD` | — | **0** |

The two known concurrency-only failures (`lifeline`, `merrow_reejerey`) do not appear at
THREADS=1, as expected — solo-green both before and after.

Stub selfplay games (deck125 vs deck130, `WAGIC_AI=gpt` with no reachable endpoint, so every
decision falls to Baka and the prompt-assembly path still runs), memory-capped, one arm each:

| arm | exit | render-probe records | windowed records |
|---|---|---|---|
| unset | 0 | 40 | 0 |
| `full` | 0 | 120 | 0 |
| `kind:3` | 0 | 102 | 0 (see caveat) |
| `4` | 0 | 124 | 116 |
| `kindly` (a typo) | 0 | 90 | 0 — refused, window OFF |

### The byte-identity proof for `full`

Two independent legs:

1. **PARSETEST.** `logWindowLogHeader(false, K) == "GAME LOG (everything that has happened so
   far):"` for every K — and that function is now the **only** producer of that string.
   `logWindowSplit` returns the narration **byte-identical** (`pre + kept == log`, `elided == 0`)
   for a log at the budget, under the budget, at a budget of 0, and with no turn header at all.
   `logWindowKindBudgeted(kLogWindowFull, ...)` is false for every class, so `logWindowApply`
   returns `narration` unchanged on every path under `full`.
2. **On a stub game.** Across the `unset`, `full`, `kindly` and `kind:3` runs — **352 assembled
   prompts** — the assembled user message contains
   `"GAME LOG (everything that has happened so far):\n" + mNarration + "\n"` verbatim, and
   `log_window_turns_elided` is 0. **352 of 352, zero mismatches.** The GAME LOG section under
   the default arm is the wave-56 bytes.

---

## 3. Falsifiable predictions

* **P1 (identity).** In an A/B `full` arm, `log_window_turns_elided` is present on **0 of N**
  records and every prompt's GAME LOG header is the wave-56 literal — **N of N**.
* **P2 (arm engages, and only where it should).** Under `kind:3`, `log_window_turns_elided` is
  present on **0** records whose `log_window_kind` is `cast`, `combat`, `target_or_reveal`,
  `pregame` or `unclassified`, and present on the majority of `land_drop` records past turn 3.
* **P3 (the thing D13 is about).** Under an `N` arm, the GAME LOG's share of the mean prompt
  falls well below the wave-56 corpus figure of **54.0%** (63.2% at deck125), and mean prompt
  chars fall with it. This is deterministic, not statistical — it needs measuring, not testing.
* **P4 (keys).** Under `full`, the `answer_replaced` / stale-answer rate, the `hold re-opened`
  count and the run-share statistics stay inside their wave-56 bands. A move here would mean a
  key input changed and the claim in §1 is wrong.

---

## 4. The A/B design (for the orchestrator)

**Arms** — one corpus per arm, identical in everything else:

| arm | `WAGIC_GPT_LOGWINDOW` | role |
|---|---|---|
| **A (control)** | unset | today's engine, today's prompts |
| **B** | `kind:3` | D13's shape, narrowed per the lane brief |
| **C** | `3` (or `4`) | the aggressive arm — every ask kind windowed; this is what tells a small effect from a large one |

Arm A is the control **and** the identity check: if arm A's records differ from the wave-56
corpus in prompt composition, the instrument is wrong and the run is void.

**Pool and size.** Same deck pool as wave 56, `-r 1` (round-robin over the 7-deck pool = **21
games per arm**, the owner's stated minimum). Model: `qwen36-35b-a3b` at
`http://100.116.136.74:8084`, the same endpoint and the same `WAGIC_GPT_TIMEOUT` for all arms —
a per-arm latency difference is one of the things being measured, so it must not also be an
input.

> ⚠ **`-j 21` is a hazard, not a setting.** The 21 comes from `-r 1` over the pool, not from
> concurrency. At `-j 6` against one Spark, median decision latency has already gone 4-5 s ->
> 17 s with p90 **at** the HTTP timeout, 26% of decisions becoming `empty_reply` fallbacks and
> half the games timing out. Run the corpora at **`-j 3`** (the decision-quality setting). Two
> of this A/B's four metrics — fallback rate and per-decision inference — are exactly what
> saturation destroys, so a concurrency-driven fallback storm would be read as a log-window
> effect. If wall time forces a higher `-j`, it must be **identical across arms** and the
> latency distribution reported alongside every metric.

**Seeds — the harness does NOT pin them.** Checked: the `WAGIC_SELFPLAY_*` env surface is
`WAGIC_SELFPLAY`, `_ACTIVE`, `_DECK0/1`, `_FAIRAI`, `_FAIRHAND`, `_ONESHOT`, `_RESULT` — there is
**no seed variable**, and `GameStateDuel::initRand(0)` falls through to `srand(time(0))`. Only
the *test suite* path pins a seed (`testSuite->seed`). So the arms **cannot be paired game for
game**: deal variance is not removed, and every game-level comparison pays for it. Two
consequences: (i) do not report a matched-pairs statistic; (ii) if pairing is wanted later, the
cheap route is the shipped transcript/replay facility (`WAGIC_TRANSCRIPT` + `WAGIC_REPLAY`),
which restores the seed and rand values — but replay re-issues *recorded* clicks, so it
re-plays a game rather than re-deciding it, and is therefore **not** usable as an A/B arm. A
`WAGIC_SELFPLAY_SEED` is the real fix and is a separate docket item.

**Metrics**, in descending order of what 21 games can actually say:

1. **Prompt composition** (deterministic; no test needed): mean/median prompt chars, GAME LOG
   share of the prompt, `log_window_turns_elided` distribution, per-`log_window_kind` counts.
   Verify the arm here first — an arm whose `log_window` field disagrees with its launch is void.
2. **Per-decision inference**: `latency_ms`, `reasoning_chars` / `reasoning_tokens`,
   `reasoning_budget_hit`, `long_reply`, `deadline_pct`. n is in the thousands of decisions.
3. **Fallback rate**: `fallback` (`empty_reply` / `unparsed_reply`) per 1,000 decisions, plus
   `choice: -1` and recovery pairing. Also thousands of decisions — but read it **against the
   latency distribution**, never alone.
4. **Seat-reviewed misplay counts**: the per-deck reviewers' rung censuses on each arm, same
   predicates, same reviewer brief. This is the metric the ruling actually cares about and the
   most expensive to trust.
5. **Win-rate band**: a **guardrail**, not a measurement (below).

**Power — what 21 games per arm CAN and CANNOT resolve.**

* **It cannot resolve win rate.** At p ≈ 0.5, n = 21, SE ≈ 10.9 points; a two-arm comparison at
  conventional power needs roughly a **30-percentage-point** swing before it is distinguishable
  from noise. So win rate answers exactly one question: *did an arm fall off a cliff?* Report it
  as a band and never as evidence that an arm is better.
* **It resolves prompt composition and inference cost outright** — those are per-decision (≈2,700
  decisions per arm in wave 56) and, for composition, deterministic given the arm.
* **Fallback rate**: per-decision n is large, but failures cluster by game and by server state.
  Treat a change under ~1.5× as unresolved unless the latency distributions match.
* **Misplay counts**: nominally per-decision, but the honest denominator is closer to the **21
  games**, because a misplay class recurs within a game. A rung going 24/24 -> 8/24 is a result;
  a rung going 24/24 -> 22/24 is not.
* **The rare-event rule applies.** A single favourable run is **not** a result. If arm C looks
  better on misplays, the follow-up is a second corpus at the same size, not a decision.

**Stopping rule.** If any arm's games stop completing (invariant 00), stop the run and say so —
do not reframe truncated games as a calibration corpus.

---

## 5. What I did NOT verify

* **No live model run.** Every number above comes from PARSETEST, the suite, and stub selfplay
  games whose GPT seats had **no reachable endpoint** (`AIPlayerGPT: no LLM endpoint reachable,
  falling back to Baka heuristics for every decision`). Nothing here says how the model *plays*
  under any arm — that is the A/B's job.
* **The ask classifiers are PARSETEST-proven, not corpus-observed.** Because the stub seats never
  reached a model, no ask-seam prompt was ever assembled: every render-probe record carries
  `kind=unclassified`. So `land_drop` / `cleanup_discard` / `empty_stack_pass` / `display_menu`
  have never been *counted* on a real corpus, and the `kind:3` arm has never been seen to elide
  anything live. **The first job of the A/B harvest is a `log_window_kind` census on the control
  arm** — if `land_drop` is not ≈10% of records (the wave-56 figure), the classifier is wrong,
  not the window.
* **The pregame clause of the summary line is PARSETEST-proven only.** The stub games produced
  **no** `- Your opening hand (...)` narration line at all (0 occurrences in 352 dumps), so the
  clause never rendered live. This looks like a property of the `WAGIC_SELFPLAY` deal path rather
  than of this change — the wave-56 lategame specimen carries the line normally — but it is
  unverified either way.
* **The opponent's mulligans are not in this seat's log** and nothing invents them. Only
  `mulliganNarration` (own-seat) is written today, so the brief's "each side's mulligan/opening
  hand size" is carried for **this seat only**. The opponent-side literals are recognised by the
  digest so a future emitter is carried automatically, but they render nowhere today.
* **`kind` mode's live prompt effect is unmeasured** for the same reason as above.
* **`logWindowSetting`'s cache is a function-static read once per process** and is not guarded by
  a mutex. Only the game thread assembles prompts (the async worker never does), so this is safe
  as written, but it is an assumption, not a proof.
* **A reply of the literal shape `CHOICE: 1 <the whole summary line>` does not bind** — the
  summary opens with `(` like the CHOICE parenthetical and the tail-stripper only strips `[...]`
  tails on already-anchored candidates. Pinned as a PARSETEST negative and recorded here. It is
  the same shape the 24k trim marker has carried for many waves with no observed echo, and the
  reply protocol asks for the row's short name in that position; a reply quoting the summary in
  its **PLAN** parses normally (pinned positive).
* **No console/port build** and no Vita run: the lever is desktop-side and untested on PSP/Vita.
