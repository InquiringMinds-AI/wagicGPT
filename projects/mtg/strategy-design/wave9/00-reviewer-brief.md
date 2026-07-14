# Wave-9 reviewer brief (shared context for all 7 deck agents + synthesis)

**THIS IS THE FINAL WAVE BEFORE A PROJECT PAUSE** (user decision 2026-07-14: shifting to other
work after this cycle). Reviews must leave each guide in a clean, shippable state and route
open items to durable notes — nothing depends on "next wave" landing soon.

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260714-094942` — 21 games, round-robin
  over decks 44,109,110,131,133,135,140. Binary /tmp/wagic-a40671057 (adds: cast-restriction
  gating in the oracle + cast-seam livelock breaker + broadened punisher rider + own-targets
  annotation), wave-8 guides, post-A/B core prompt (hand->stack anchor GONE). 3 timeouts.
- Baselines: wave-8 control `matchups-20260714-044131`, wave-8 removal arm
  `matchups-20260714-064838`, wave-7 `matchups-20260713-222831`.
- Translog fields: gamestart header (game_id/opp_deck), defer records (ZERO this corpus),
  events (narration delta), options_text.

## USER EVALUATION DOCTRINE (BINDING, unchanged from wave-8)

Win-rate is luck-of-draw + deck design, NOT play quality; cite it as context only. The unit of
analysis is the DECISION: audit opportunity-conversion (lethal taken/missed, correct holds,
removal timing, racing/blocking arithmetic) with seq repros. A guide revision is justified by
a decision-level failure class; validated by that class disappearing.

## Already verified main-side (do not re-derive; per-deck confirmation welcome)

- 135v133 LIVELOCK FIXED: the matchup completed decisively (T11). The Astrolabe class
  (restriction-gated cast modes) is dead; Astrolabe now casts via its snow cost.
- Own-targets annotation rendered 166x ("the only legal targets are YOUR OWN right now").
- Punisher rider now fires on damage-trigger punishers (Boros Reckoner confirmed; Obliterator
  didn't meet a block window this corpus — deck133's reviewer should look for one).
- Timeout rows adjudicated: 135v140 (behind), 140v131 (close), 140v110 (AHEAD 18-3) — all the
  known deck140 grind class, triage latency vs play before claims.
- Zero defer records; zero validation failures corpus-wide.

## Wave-9 verification list (per-deck reviewers)

1. **deck109**: the re-anchored chump rule (block only on surfaced LETHAL) — did the two
   wave-8 misplay classes (Obliterator block at 10 life, high-life gang block) die?
2. **deck131**: the strict-ordered mulligan gate — false-keep class gone? Both directions of
   the tally. Land-menu execution steady (Mountain-first)?
3. **deck135**: the develop-imperative (Rule #1 point 3) — did development paralysis die?
   Fetch-tax trend (was ~27/game post-suppression).
4. **deck140**: sweeper timing on the count-anchored rule (2nd corpus); BSZ-X guardrail; the
   demoted belief-kill lines — any resurrection of the false beliefs? Timeout triage per above.
5. **deck133**: bare-N Gray Merchant thresholds working? Obliterator punisher-rider sighting?
   Liliana coexist-casts steady?
6. **deck110**: reading `Artifacts in play: N` (the wave-8 taught rule) — did the pilot stop
   hand-counting? Re-attach/no-op steady?
7. **deck44**: counter spine steady? Own-targets annotation at GFTT windows — did the
   reasoning tax drop (shorter/cleaner plans at those windows)?
8. ALL: watches carried from wave-8 (phantom-lethal deck109; shroud-vs-non-targeted deck140;
   free-reaction-cost deck135; multi-copy churn deck110; intent-collapse reply protocol
   deck135; `damage >= toughness = dead` deck131).

## Output contract (per deck agent, sequential, in strategy-design/wave9/deckN/)

findings.md -> strategy.txt (full revised guide per the CURRENT skill =
strategy-design/wave8/strategy-writing-skill.md) -> skill.md -> general-suggestions.md ->
notes.md. Freeze-check every kept line; displacement audit; layer-routing enforced. Because of
the pause: notes.md items must be self-contained (full repro + context), and strategy.txt must
stand alone without expecting a wave-10 follow-up.
