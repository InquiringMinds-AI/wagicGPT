# deck27 skill.md — proposals for the strategy-writing skill (wave-22)

Seat: Zombies (UB tribal), guided round 3. Single-seat by default; layer-routing enforced (single-seat
items CANNOT add/cut a skill core line). No new headline rung. This corpus CONFIRMS existing rungs and
surfaces ONE candidate note for synthesis (a NEW instance of the answer-first commit interaction).

## Confirmations (existing rungs, strong worked examples)

- **"Read the tax, not the record" (rule 1) CONFIRMED in the DOWN direction across a pool swap.** The
  record fell 5/6 → 1/6 while the tax stayed at ZERO (0 fallbacks, unchanged from wave-21). Per-game
  attribution shows the drop is entirely pool-swap difficulty + draw/opponent variance (3 continuing
  opponents flipped W→L, each with a clean variance cause; 2 new opponents are deck27's worst
  archetypes). NOT one shipped change touched deck27's decisions (0 exposure to fetch/N9/echo/transform/
  painland at this seat). Lesson reinforced: a record SWING — up OR down — is not evidence a guide/engine
  change worked or broke; you must attribute per-game, and a frozen guide + 0 fallbacks + 0 annotation
  exposure is dispositive proof the swing is exogenous. No text change.

- **"Write the off-case even if the winning corpus never needs it" (rule 6) — off-case STILL
  unexercised, now 3 corpora.** The late-stall durdle revision has gone THREE corpora without its
  trigger firing (its authoring corpus, wave-21, wave-22). vs140 was the best grind window yet
  (lifegain-control, cap game) and STILL didn't trigger it — deck27 attacked every turn, never held
  back. This is the expected life of a low-frequency off-case, NOT evidence to cut it. Reinforces
  wave-21's candidate note: **an off-case revision may require MANY corpora to draw its trigger; hold
  and re-flag, do not delete or "revise for lack of validation."** NEW nuance worth synthesis: vs140
  showed the off-case's PREMISE may be insufficient even when followed — deck27 obeyed "attack every
  turn" and still lost the clock, because the real gap (no answer to lifegain) is CONSTRUCTION, not
  guidance. When an off-case's prescription is followed and the loss persists, re-examine whether the
  target state is guide-fixable at all, or is masking a construction problem. Candidate note; single-seat.

- **Model-vs-representation-vs-PARSER doctrine CONFIRMED, with a new wrinkle.** Wither (representation)
  fixed at the engine → clean, closed. But this corpus adds a case where the PARSER-layer fix
  (answer-first) has a DOWNSIDE, not just an upside: commit-first is a truncation-safe floor ONLY when
  the snap answer is correct (vs109 s15 — a snap misplay was frozen by truncation before the model's
  own correction could land). Sharpens the doctrine: **a parser/harness change that defuses one failure
  mode can introduce a new one at the same seam; validate both directions, and prefer to fix the
  RESIDUAL at the layer the parser actually commits (here: the guide's snap-line content).** Supports
  layer-first routing; no core change.

## WATCHES (unpromoted)

- **Answer-first commit-lock (snap misplay frozen by truncation): 1st HARMFUL instance (vs109 s15).**
  Prior instance (wave-21 vs35 s18) was benign. Candidate skill note for synthesis: **when a decision
  is committed on the first coded line and the model may self-correct in later reasoning, a TRUNCATED
  reply locks the snap — so for decision kinds where the snap is easy to get wrong (multi-attacker
  blocker allocation especially), the GUIDE must front-load the correct heuristic so the snap is right;
  do NOT rely on last-wins to rescue it (truncation may pre-empt the corrected line).** Cross-seat:
  synthesis should check whether other seats show harmful commit-locks. Promote to a rung only on a 2nd
  seat.

- **Ahead-position degeneration loop: NO recurrence this corpus.** Stays at 3 instances, unpromoted
  (decode-layer; answer-first defuses harm when it does fire). No action.

## No new core-prompt candidate from this seat
Every item is single-seat and/or owns a layer below core (wither = closed engine annotation; answer-first
commit-lock = parser/guide seam; durdle off-case = unexercised guide; lifegain gap = construction). See
general-suggestions.md for routing.
