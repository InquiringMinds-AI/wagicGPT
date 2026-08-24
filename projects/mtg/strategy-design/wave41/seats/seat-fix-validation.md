# Wave-41 cross-cutting fix-validation seat (harvested 2026-08-24)

[Seat-returned report, orchestrator-filed. All counts from the per-record events delta
stream (12,872 event lines / 2,165 records) or options_text — never cumulative prompt greps.]

## Verdict table
- 1a anonymous public-origin moves: 0/12,872 (baseline 47) — CONFIRMED.
- 1b hidden-origin masking: 0 opportunities — 0/0 UNINFORMATIVE (only hand->library moves
  were own-seat mulligan bottomings, peer correctly never sees them).
- 1c bulk-shuffle collapse: 28/28 collapsed attributed lines, 0 spam — CONFIRMED.
- 1d same-zone no-op: 0/12,872 (baseline 25) — CONFIRMED.
- 2 effect-as-source: old "with Put in Play" shape 0 — BUT 42 residual (13 sourceless
  observer lines + 29 anonymous "with this effect's X ability" fallbacks) — PARTIALLY
  REFUTED -> D3.
- 3 used-lines symmetry: 236 You / 419 Opponent (was 165/0) — CONFIRMED, with a NEW
  inverse gap -> D2.
- 4 Hydroblast/Pyroblast/Blast/Lace offers: 0 of 7 pool decks contain them — HONEST NULL.
- 5 prevention block outcomes: Fog Bank 5/5 render "(neither dies ... prevented)", 0
  violations; all 25 dies-lines on non-prevention bodies — CONFIRMED.
- 6 may-batching: 6 batch asks / 27 triggers (pre-fix 1,189 re-asks); arrival-traced
  exact accounting, 0 silent gains — CONFIRMED.
- 6b turn-scoped verdict edge: no turn held two identical windows — 0/0 UNINFORMATIVE.
- 7 X-announce: 11 records = 11 narration lines = 11 X casts; 0 silent commits; X=0
  present 11/11 ladders — CONFIRMED.
- 8 repeat countable: 47/49 = 96% at/above floor 2; 0 unstripped [repeat: echo leaks —
  CONFIRMED; 2 misses -> D6.
- 9a morbid: 33/33 — CONFIRMED. 9b equip: MOVES-note 209/209, done-signal 0/0 NULL
  (no-op form never offered). 9c may labels: 1/1 (low N). 9d marker filter: 0/0 NULL,
  0 leaks. 9e flip-toggle: 9/9 + stderr auto-pass suppressions — CONFIRMED.
- 9f target-preview: 168/218 typed; 50-line shortfall is one branch -> D5 — PARTIALLY
  REFUTED.
- 10 fallback autopsy: 15/2165 = 0.69%. All 11 empty_reply at latency 2503-2506ms (NOT
  timeout — client floor 5000ms; serve returned nothing fast), clustered 2 games/3 seats
  turns 1-6 = ONE transient serve event at the -j 21 startup herd. Model-attributable
  classes FLAT wave-over-wave (0.18% vs 0.19%). NOISE for AI layer; endpoint-startup note.
- 11a stacked lord grants: pool has no keyword lord — HONEST NULL. 11b chooser
  auto-resolve: no notaTarget scripts in pool — NULL; global 21/21 natural, 0 hangs.

Protocol health: answer_replaced 1, commit_retracted 1, dropped_assignments 0/29,
parse_note 4 (echo_index_conflict), reasoning_budget_hit 12, degenerate max 0.013.

## NEW defects -> wave-42 docket
- D1 (HIGH) Tutor search reveals WHOLE LIBRARY card-by-card to BOTH seats — 1,546 event
  lines (12%), 2.53M chars = 15.2% of all assembled prompt text; also a hidden-info LEAK
  (observer names opponent's library, 377 lines; rules say reveal only the found card).
  Fix shape: lane-B bulk-collapse applied to the search/reveal path. Cites: ...891-deck126
  seq 9 (162-line delta), ...863-deck130 seq 12, ...898-deck162 seq 6.
- D2 (MED) Observer channel now a SUPERSET of actor's own narration — asymmetry inverted
  (236/419), 17 of 20 pairs disagree; actor writes only consumed decisions, observer event
  fires for everything reaching activateAbility() (heuristic/auto-resolved/trigger-internal
  invisible to their own actor).
- D3 (MED) Sourceless/raw-identifier activation lines on observer path (13 no-source, 29
  anonymous-fallback) — new event needs lane B's waiting->source ladder.
- D4 (MED) Engine lexicon leaking into narration: ToughLife(9), Put in Play/Hand/Library
  (43), "1/1 Counter"(57, double-space), level Counter(12), Deal 1 Damage(168); malformed
  reveal-summary grammar ("You revealed 51 and put Exquisite Blood to choose card").
- D5 (MED, trust-doctrine) "the only legal targets are YOUR OWN right now" branch names
  NOTHING (59 lines, all Tragic Slip cites) — silent omission invites confabulation;
  render the owned targets with facts. (= the 9f shortfall.)
- D6 (LOW) [repeat:] counter not incremented for land-animation abilities (both misses =
  "becomes beholder with Hive of the Eye Tyrant"; ActivatedAbility::counters not advancing
  on this class).
- D7 (LOW) Token-cleanup moves render anonymous destination "your zone" (84 lines;
  ceases-to-exist moves survived the same-zone suppression).
- D8 (LOW) events delta sliced by char offset not line boundary (1/1808, ...863-deck130
  seq 62 begins mid-word).
- D9 (LOW) Mulligan bottoming narrates card-by-card, never states the bottoming; 7-line
  shuffle-back spam foldable by the same bulk-collapse.

## Owner lategame specimen (standing invariant)
Staged: scratchpad/lategame-specimen-w41.txt — ...893 deck130 seq 79, t41, 17,010 chars
(corpus median 9,759; t>=25 median 17,140; max 27,715). ~15% of it is D1 reveal spam —
pair them when he reads it.
