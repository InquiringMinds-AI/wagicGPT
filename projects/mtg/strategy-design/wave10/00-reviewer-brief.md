# Wave-10 reviewer brief (shared context for all 7 deck agents + synthesis)

This is the RESUME wave after the 2026-07-14 pause: the wave-9 parked ledger items 1-4 all
shipped between corpora (commits c8c054be8 + fd64c6fc6). The corpus this wave reviews is the
FIRST under the answer-after-plan reply protocol, so protocol-shift effects are a first-class
review dimension, not noise.

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-031825` — 21 games, round-robin
  over decks 44,109,110,131,133,135,140. Binary /tmp/wagic-c8c054be8, wave-9 guides, ANSWER-LAST
  reply protocol (scratch reasoning -> PLAN: -> CHOICE:/ATTACK:/BLOCKS: final line),
  max_tokens 2048.
- Control baseline: wave-9 `matchups-20260714-094942` (head-first protocol; the intent-collapse
  control arm — 120/2978 detector flags, see wave10/reply-protocol-ab.md).
- Translog novelties this corpus: filenames carry `-vs-<oppdeck>`; capped games appear in
  results.tsv as winner=adj0/adj1 (life-adjudicated at cap).

## USER EVALUATION DOCTRINE (BINDING, unchanged)

Win-rate is luck-of-draw + deck design, NOT play quality; cite it as context only. The unit of
analysis is the DECISION: audit opportunity-conversion (lethal taken/missed, correct holds,
removal timing, racing/blocking arithmetic) with seq repros. A guide revision is justified by
a decision-level failure class; validated by that class disappearing.

## Engine changes whose firing MUST be verified (confirmed-fired-representation doctrine)

1. **Answer-after-plan protocol**: label compliance rate per seat (CHOICE:/ATTACK:/BLOCKS: in
   reply); fallback rate vs control (~0); run tools/intent-collapse-metric.py on the corpus
   and compare against the 120-flag control baseline. Positives to hunt: decisions where the
   plan's conclusion now MATCHES the recorded choice at windows that would have collapsed
   (mulligan asks, cast-nothing reversals, attacker declines).
2. **Fetch consume-on-choose**: fetch-crack windows/seat (control: deck135 avg 21.3, corpus
   216 total, 196 fetch-only). Expect a large drop. Verify no strategic damage: fetches still
   crack when the guide wants them (deck135 crack-for-missing-color).
3. **Legal-target names on targeted casts** ("- legal targets right now: ..."): rendered? Did
   the deck44 fabricated-no-legal-target class (wave-9 s54) die? Reasoning-tax check at those
   windows.
4. **Thin-hand discard annotation** (deck133 Thoughtseize at opp hand <= 2): fired? obeyed?
5. **[deals 0] blocker marker**: any 0-power blocker windows? (wave-8 Archmage class)
6. **Artifact tag completeness** (deck110): does a re-derived per-line count now AGREE with
   the `Artifacts in play: N` summary everywhere?
7. **adj0/adj1 rows**: sanity-check one against the seat translogs' final life.
8. **max_tokens 2048 + truncation guard**: any unparsed_reply from repetition loops (expected
   rare, self-healing)? Any reply STILL cut before its answer line?

## Wave-9 guide-revision verification list (per-deck reviewers)

1. **deck44**: counter category #2 WHY-as-imperative + value-floor face-burn carve-out — did
   the counter spine hold? Own-targets + legal-target-names interplay at GFTT windows.
2. **deck135**: Rule #1 point 3 develop-branch rewrite (option-list gate, fetch-crack for
   missing color, offered=payable/green-pays-generic) — fetch-crack develop drop toward 0?
   Watch the parked core candidate: any decline of an OFFERED cast on a "colored can't pay
   generic" belief? (2nd seat = promotion trigger.)
3. **deck140**: sweep-trigger boundary "16 or LOWER" + tapped invented-phrase clause + BSZ
   partial-sweep qualification + Lightmine second-copy line — all obeyed? Grind timeouts now
   adjudicated: read adj rows before any timeout claim.
4. **deck109/110/131/133**: FROZEN guides — verify the frozen guardrails' classes stayed dead
   (chump-on-LETHAL-only 109; artifacts-count reading 110; mulligan gate + Mountain-first 131;
   GM bare-N + coexist-casts 133).
5. ALL: carried watches (phantom-lethal 109/110; shroud 140; free-alt-cost 135; multi-copy
   equip churn 110; damage>=toughness 131; within-turn double-sweep 140).

## Output contract (per deck agent, sequential, in strategy-design/wave10/deckN/)

findings.md -> strategy.txt (full revised guide per the CURRENT skill =
strategy-design/wave9/strategy-writing-skill.md) -> skill.md -> general-suggestions.md ->
notes.md. Freeze-check every kept line; displacement audit; layer-routing enforced (engine
items -> notes.md ledger with repros, NEVER prompt text). Convergence gate: single-seat items
cannot cut core lines. The pause-aware demotion doctrine still applies to guides frozen in
wave-9: demote a guardrail only once its class was OBSERVED OBEYED this corpus.
