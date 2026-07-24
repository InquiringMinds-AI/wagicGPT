# deck27 skill.md — proposals for the strategy-writing skill (wave-23)

Seat: Zombies (UB tribal), guided round 4. Single-seat by default; layer-routing enforced (single-seat
items CANNOT add/cut a skill core line). No new headline rung. This corpus CONFIRMS existing rungs and
surfaces ONE candidate classification note for synthesis (decode-collapse vs commit-lock).

## Confirmations (existing rungs, strong worked examples)

- **"A record swing is not evidence a change worked" (read the tax, not the record) — CONFIRMED in the
  UP direction now.** Record rose 1/6 -> 4/6 while the guide stayed byte-frozen and the revision touched
  exactly ONE decision. The swing is pool-swap + variance (three clean earned wins on the win-path; the
  two losses are the two known worst archetypes). Symmetric to wave-22's DOWN-direction confirmation.
  A reviewer must attribute per game and resist crediting the revision for the up-swing. No text change.

- **"Fix the residual at the layer the parser actually commits" — CONFIRMED: the free-kills-first GUIDE
  fix behaved correctly.** The wave-22 residual (a snap lord-chump misplay) was routed to the GUIDE (the
  snap-line content), not to a decode campaign. This corpus the near-identical scenario (vs109 s13:
  goblins + a bigger attacker, lone lord blocker) resolved CORRECTLY at the snap layer. Reinforces
  layer-first routing: front-loading the correct snap heuristic in the guide is the right lever for a
  snap-committed decision kind. (Thin: N=1 multi-attacker case — hold for another corpus.)

- **"Write the off-case even if the winning corpus never needs it" — off-case STILL unexercised, now 4
  corpora.** The durdle late-stall revision has gone four corpora without its trigger firing (deck27
  attacks every turn; the walled-stall-with-held-attackers state never occurs). Per wave-21's own note,
  this is the expected life of a low-frequency off-case; hold and re-flag, do not cut. Same nuance as
  wave-22 stands: when an off-case's prescription IS followed and the loss persists (vs140), the real
  gap may be CONSTRUCTION, not guidance — re-check guide-fixability before re-authoring.

## WATCHES (unpromoted) + candidate classification note for synthesis

- **NEW candidate note — separate DECODE-COLLAPSE from COMMIT-LOCK when classifying unparsed/truncated
  fallbacks.** Two distinct failure classes share the "answer-first didn't save it" surface but need
  different owners and fixes:
    - **Commit-lock** (wave-22): a WRONG coded answer is emitted on line 1, the model self-corrects in
      later prose, truncation freezes the wrong snap. Owner: GUIDE (make the snap right) / possibly
      PARSER (prefer last coded line on non-truncated replies). There IS a coded answer.
    - **Decode-collapse** (wave-23, vs137 s12-14): the reply is pure degenerate token-repetition with
      NO coded line at all -> unparsed_reply. Answer-first cannot save it (nothing was committed); the
      parser is correct to reject it (nothing to recover). Owner: MODEL/DECODE-sampling (rep_penalty,
      max-tokens fast-fail), NOT parser, NOT guide.
  The reviewer test: does the failed reply contain a coded line (CHOICE/ATTACK/BLOCKS)? If yes ->
  commit-lock family (guide/parser). If no -> decode-collapse (sampling/harness). Mis-routing a collapse
  to a parser change wastes engine work on a non-parser fault. Candidate note for synthesis; single-seat
  origin but a general classification aid — promote only if a 2nd reviewer independently hits it.

- **Ahead-position degeneration: now HARMFUL, and the "cheap fix" (rep_penalty) did not hold.** The
  ahead-loop watch (formerly benign verbatim-line loops, always answer-first-saved) produced 3
  clustered unparsed decode-collapses this corpus — under the rep_penalty=1.05 pilot that was its
  theorized fix. Data point for the synthesis pilot verdict: rep_penalty=1.05 is insufficient for this
  collapse. Not a skill-core change; feeds the pilot verdict + the notes.md sampling ledger.

## No new core-prompt candidate from this seat
Every item is single-seat and/or owns a layer below core (wither = closed engine annotation;
free-kills-first = per-deck guide, validated thin; decode-collapse/ahead-degeneration = sampling/harness;
durdle off-case = unexercised guide; lifegain gap = construction). See general-suggestions.md for routing.
