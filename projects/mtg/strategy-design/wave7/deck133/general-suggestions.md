# deck133 wave-7 — core-prompt / representation proposals (layer-routed)

The burden of proof is ON core: every deck pays for each line. deck133 produced NO core-prompt
proposal this wave (no deck-agnostic BAD-PLAY leak — the corpus was clean: 0 desyncs, 1
fallback, no no-op decided a game). Two REPRESENTATION items (route to engine, not the prompt
body) and one non-item.

## 1. [REPRESENTATION -> ENGINE, A/B not a wave edit] Make the Gray Merchant drain option display the RESOLVED drain (include GM's own +2).
- **Where:** the option text builder that emits `{right now: drains N}` for lifeleech/devotion
  magnitudes (the `WParsedInt(expr, NULL, card)` evaluation path that already computes the P1/P4
  dynamic magnitude).
- **Defect:** N is computed BEFORE Gray Merchant himself is on the battlefield, so it
  under-counts his own {B}{B} = 2 devotion. Real drain = N + 2. Confirmed 4/4 across waves
  (wave-5 3/3, wave-6 1/1, wave-7 vs131 seq72: shown 5, resolved `opp -7 / my +7`).
- **Why it routes to representation, not the guide:** every deck133 guide since wave-5 carries a
  "read N and ADD 2" workaround. This wave (vs131 seq72) the pilot IGNORED it and manually
  recounted devotion, erring. Instruction cannot substitute for a correct display. If the option
  showed the RESOLVED value, the workaround is DELETABLE from the guide.
- **Scope check before shipping:** confirm the +2 is GM-specific (his own devotion counted at
  resolution) and not a general lifeleech-eval offset that would double-count on other cards —
  the fix must add the caster's own contribution ONLY where the magnitude is
  `mybattlefield`-scoped devotion the source itself will join. Verify against
  `auto=lifeleech: -type:manab:mybattlefield`.
- **Ship as an A/B, not a blind wave edit** (single-seat evidence; GM reached the list in only
  2/6 games).

## 2. [REPRESENTATION / EVIDENCE-TOOLING — reviewer-facing, not player-facing] Surface the RESOLUTION outcome of a consumed cast in the translog.
- The translog records `chosen_text` (what was picked) but a reviewer must reconstruct whether
  the spell RESOLVED or was countered/fizzled from the next record's narration tail (vs44 seq27:
  a "won by GM drain" reading is wrong — it was countered; won by beats). A per-record
  `resolved: battlefield|countered|fizzled|graveyard` (or an `effect_events` echo) on the NEXT
  gameend/decision would remove the manual narration-scrape and prevent mis-attribution of win
  MECHANISMS. Filed to the harness/evidence ledger, not the prompt.

## 3. [NON-ITEM — explicitly NOT core] Discard threat-axis blind spot stays PER-DECK.
The vs109 "discard can't answer go-wide" observation is posture-/matchup-dependent (a control
deck WANTS discard vs width to strip the payoff). It goes in the deck133 guide (added: "if they
go wide, Obliterator wall + Collective Brutality, not more discard"), NEVER core. Recording here
only to argue the routing: promoting it would misplay every disruption deck that faces a
card-bound clock.
