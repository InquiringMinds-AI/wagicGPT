# Wave-8 reviewer brief (shared context for all 7 deck agents + synthesis)

## Corpus data

- **PRIMARY (control arm)**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260714-044131` — 21 games,
  round-robin over decks 44,109,110,131,133,135,140, binary /tmp/wagic-a00a99f48 (full wave-7
  engine ledger), wave-7 guides, UNCHANGED core prompt. 3 timeouts.
- **SECONDARY (A/B removal arm)**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260714-064838` —
  same stack, core prompt with the `hand -> stack` cast-happened anchor sentence REMOVED (full
  sentence). 2 timeouts. Use for A/B-relevant checks only; per-deck findings cite the PRIMARY arm.
- Baseline for wave-over-wave comparison: wave-7 corpus `matchups-20260713-222831`.
- results.tsv now fills life0/life1/turn on timeout rows (adjudicated from seat translogs).
- NEW translog fields this wave: `gamestart` header record (game_id shared by both seats,
  my_deck/opp_deck), `defer` records (`deferred_to_heuristic` when a validation-failed pick fell
  to the heuristic), `events` (narration delta since the previous record — a consumed cast's
  outcome is machine-readable), options_text as before.

## A/B verdict (already computed — synthesis folds it in, reviewers need not recompute)

Anchor removal is ADOPTED: cast-nothing 41.5% vs 41.9%, consecutive same-pick 12 vs 12,
same-name double-casts 13 vs 14. No regression; the anchor's defect class is fixed at source
(353f1a150 + e11b97a77). The live prompt now carries the removal. User concern that motivated
the full-sentence removal: the anti-repick clause risked reading a legitimate second copy of a
just-cast card as "the cast did not happen" (name-level ambiguity).

## USER EVALUATION DOCTRINE (2026-07-14 — BINDING on every review)

Win-rate is luck-of-draw + deck design, NOT play quality. At the intermediate band the project
targets, most board states have objectively correct answers. A good strategy = the pilot plays
in a way that WINS WHEN IT HAS THE OPPORTUNITY or can create it. Win tables need hundreds of
games per deck to separate from noise — cite them as context only, NEVER as a verdict on a
guide. The unit of analysis is the DECISION: audit opportunity-conversion (lethal taken/missed,
correct holds, removal timing, racing/blocking arithmetic) with seq-number repros. A guide
revision is justified by a decision-level failure class; it is validated by that class
disappearing next wave.

## Engine changes to VERIFY FIRED (wave-7 ledger, all shipped in this corpus's binary)

1. Land-drop enumeration: distinct lands co-offered ("Play Mountain" + "Play Island" in one
   options_text). CONFIRMED 96 multi-land offers corpus-wide. Per-deck reviewers of 2-color
   decks: verify the guide's color-priority rule now executes, then DROP the interim
   hold-to-reach-your-color stopgap from the guide (tracked removal condition met).
2. No-legal-target loop: `fails validation` ≈ 0 (corpus total: 1, Glimmervoid's deliberate
   residual gate). Defer records make this visible per-deck.
3. Creature counts on battlefield headers + tapped-creature "untaps and can attack next turn"
   tag. deck140 reviewer: check whether the false-belief classes (tapped=no-threat,
   artifact-board=sweeper-proof, shroud-stops-wipes) died; if so the guide's false-belief kill
   lines become deletable.
4. Decline suppression: priority windows 17.9 -> 11.0 per seat. deck135 reviewer: held-fetch
   re-offer tax should be gone; verify no strategy damage (a wanted crack made impossible —
   look for a fetch the pilot declined twice then needed).
5. Blocker-seam lethal arithmetic + punisher text riders. All reviewers: reflexive high-life
   blocking and missed must-blocks are the watch (3 guises / 5 seats historically); check
   whether the surfaced arithmetic killed them.
6. Gray Merchant resolved-drain display (adds own pips). deck133: the guide's read-shown-add-2
   workaround becomes deletable if the display is right (verified drains 6 on devotion-4 in
   probe).
7. KNOWN ENGINE WEDGE in the PRIMARY arm: game 135v133 (timeout, turn 2, 20/20) is an ENGINE
   LIVELOCK, not a play failure — Arcum's Astrolabe was offered in its never-castable normal
   mode, the pick no-op'd, and the cached ask replayed 903k times. Fixed post-corpus (oracle
   honors cast restrictions + cast-seam livelock breaker). deck135/deck133 reviewers: exclude
   that game from play-quality claims; the other timeouts (135v140, 140v131 in both arms) are
   genuine grinds — check who was ahead via the adjudicated life/turn columns.

## Inherited convergence watches (promote at 2nd seat)

- Phantom-lethal fixation -> decline-a-listed-body (deck109, 1 seat).
- Mechanics facts tapped-still-threat + shroud-vs-non-targeted (deck140; prefer representation
  #3 over core prose — check if #3 already dissolved it).
- Free-reaction-card-cost (deck135; need a 2nd free-alt-cost deck).
- Multi-copy equip churn (deck110; suppression shipped only for pass-declines, not
  all-copies-attached — re-measure the churn).
- Plan/choice-mismatch on attractive labeled counter options (deck44, low-frequency residual).

## Deck-construction flags (user owns decklists — restate, don't relitigate)

deck131 manabase 14I/8M + 6/60 threat density (weigh against the now-landed land-menu fix);
deck135 blue-thin manabase strands its win creatures; deck140 reach starvation; deck131 & 140
cannot beat lifegain without a non-incremental finisher; deck109 evasion-matchup class.

## Output contract (per deck agent, sequential, in strategy-design/wave8/deckN/)

findings.md -> strategy.txt (full revised guide per the CURRENT skill =
strategy-design/wave7/strategy-writing-skill.md) -> skill.md (proposals) ->
general-suggestions.md (core-prompt proposals, layer-routed) -> notes.md (dev notes:
engine/harness/model items with seq repros). Freeze-check every kept line; displacement audit
per skill P1; layer-routing enforced (engine items to notes.md, never prompt text).
