# Dev notes — deck109 wave-22 (engine/harness ledger with repros) + ROTATION VERDICT

## Record: 3/6 (context only) — beat 27/140/102, lost 135/44/62. All 3 losses = evasive/race
matchup ceilings (bigger midrange, faerie tempo, green voltron-flier). 0 fallbacks at seat.

## Corpus / provenance
- Run `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260723-173843` (21 games round-robin), binary
  `/tmp/wagic-1d8ed8194` (wave-22 step-1 batch, live-unvalidated -> THIS corpus validates it).
- deck109 seat files (opp): 1784850348 (27), 1784850718 (140), 1784852536 (135), 1784856343 (44),
  1784856508 (102), 1784858809 (62).

## LEDGER ITEM B (engine / representation) — CLOSED this wave: hybrid option-line annotation SHIPPED
Wave-13 flagged this UNSHIPPED. The engine now renders the hybrid cost on the option line:
- `Cast Rakdos Cackler {b/r} (1/1) [hybrid: each {b/r} pays with B or R - total 1 mana]` (vs27 s24)
- `Cast Boros Reckoner {r/w}{r/w}{r/w} (3/3) [hybrid: each {r/w} pays with R or W - total 3 mana] {right now: damage 0}` (vs44 s23)
Validation at seat: 0 hybrid-confusion markers ("lacking black"/"no black"/"need black"/"two mana"
grep = 0); Rakdos Cackler cast 2/2, Boros Reckoner 1/1, all off Mountains; Unleash 1/1-counter 8/8.
**Consequence for the guide (already applied in this wave's strategy.txt):** the interim card-note
crutch is RETIRED/demoted per the skill's crutch-lifecycle CLOSED-criteria. No further engine work
owed on Ledger B; mark CLOSED.

## LEDGER ITEM A (engine / harness — attacker parse) — RECURS in a NEW trigger: SPIRAL/TRUNCATION under-count
Wave-13 Ledger A was the MIXED index+name attack form silently dropping the named attacker. Two
updates this wave:

**(A1) Mixed-form drops are OUTCOME-NEUTRAL this corpus (the dropped names were all ineligible).**
Four mixed/full-name attack lines occurred; in every case the named-but-non-index creature was NOT in
the engine's eligible-attacker set (summoning sick), so dropping it changed nothing:
| seq | line | eligible opts | recorded (next combat log) |
|---|---|---|---|
| vs27 s10  | `A1, Goblin, Goblin`        | opts=1 (Gore-House only)     | Gore-House Chainwalker |
| vs44 s14  | `A1, A2`                    | opts=1 (Ash Zealot only)     | Ash Zealot |
| vs44 s25  | `A1, Boros Reckoner`        | opts=1 (Ash Zealot only)     | Ash Zealot |
| vs102 s16 | `A1, Gore-House Chainwalker`| opts=1 (Goblin only)         | Goblin |
No harm — the named creatures could not have attacked. (Contrast wave-13, where dropped names WERE
eligible.) The proposed name->index reconcile arm would still make these declare-intent-exact, but
priority is low absent an eligible-name drop this corpus.

**(A2) NEW and consequential: a PURE-INDEX line under-recorded when the reply spiraled to max_tokens.**
- **vs62 s21 (T9, attackers):** reply's answer-first line = `ATTACK: A1, A2, A3, A4` (all 4 eligible:
  A1 Goblin, A2 Goblin, A3 Ash Zealot, A4 Legion Loyalist). Recorded `choice=2`, chosen `Goblin,
  Legion Loyalist` (A1+A4); next record's combat log: `You declared attackers: Goblin, Legion
  Loyalist`. **A2 (Goblin) + A3 (Ash Zealot) silently dropped — ~3 power.** No `fallback` field.
- Mechanism: the reply then looped "I will declare no attackers … but the guide says attack … wait"
  ~15 times (Briar Patch -1/-0 shrank the team; opp's 5/8 flier unkillable) and hit max_tokens
  (truncated mid-sentence, 12457 chars). There is NO later well-formed `ATTACK:` line in the reply,
  so last-well-formed-ATTACK-wins should have kept A1-A4. The recorded 2-subset (A1+A4, not "none",
  not "A3 only" as the prose waffled toward) looks like a **combat prose-intent salvage arm partially
  overriding the clean answer-first line** — i.e. the salvage fired on truncated waffle prose and
  produced a wrong 2-attacker subset instead of honoring the answer-first line.
- **Repro is cheap** off `/tmp/wagic-1d8ed8194`: the record is self-contained (seq 21 of the vs62
  seat file). Contrast s23 (same game, T11): `ATTACK: A1..A6`, long reply but NOT a repetition
  spiral, recorded all 6 correctly — so the trigger is specifically the max_tokens truncation /
  waffle-prose salvage, not reply length per se.
- **Proposed engine change (route here, NEVER guide/core):** when a well-formed answer-first
  `ATTACK:` line is present, the combat prose-intent salvage must NOT override it with a
  differently-counted subset — the answer-first coded line is authoritative; salvage is a FALLBACK
  for when no coded line parsed. Audit the attackers salvage precedence at the AIPlayerGPT combat
  parse site. Secondary: cap/guard the pathological repetition (see general-suggestions — the
  model-behavior half).
- **Outcome coupling:** dropped ~3 power in a game deck109 lost to an unraceable 5/8 flier
  regardless — NON-DECISIVE, but a clean loss-decoupled-enough regression fixture (the game was
  lost to matchup; the drop did not create the loss). Do not inflate; do not downgrade — it is a
  live silent under-count invisible to a window count.

## Answer-first / last-CHOICE-wins — WORKING (informational, no action)
- vs62 s16 (T7 ask): first line `CHOICE: 2 (Cast nothing)`, model self-corrected mid-reply to
  `CHOICE: 1 (Cast Pillar of Flame)`; parser took the last well-formed CHOICE = choice=1 (correct
  aggressive play). last-well-formed-CHOICE-wins working as designed. NOT a bug — recorded here only
  to distinguish it from the s21 attacker anomaly (CHOICE path honored the late line; ATTACK path did
  not honor the early answer-first line).

## Burn / blocking / priority verification (this seat)
- **0 self-target burn** (vs102 s12 own-Goblin target = FORCED edict, only own creatures legal). Burn
  face 17/19; exception fired once (vs44 s13 -> Obyra drainer, correct).
- **Blockers: 2 windows** (vs27, vs135), both correct all-decline NOT-lethal via parseable `Bn:none`.
- **Priority: 8 windows** (vs44 T10-13), all Boros Reckoner `{RW}: first strike` with `damage 0`
  (not in combat) — model PASSED 8/8. Correct (guide: pay only in combat vs 3-power).
- **Transform (d) / painland (e) / bounce-on-stack / wither / reveal**: N/A at this seat (mono-red,
  20 Mountains, no painlands, no transform, no reveal; Boros bounce never triggered — it took 0
  combat damage all corpus).

## Deck-construction observations (upstream, unchanged — do NOT relitigate at a re-validation)
No card advantage / no reach vs a bigger board or a lifegain/voltron shell; no answer to evasion
(fliers/shroud/pro-red). Beat the grindy 140 by flooding-then-topdecking a haste Hellrider; lost the
three races to structural-edge boards. Inherent RDW ceilings surfaced by draw speed.

## ROTATION VERDICT
**deck109 = ROTATION CANDIDATE (converged / re-validated).** Guide change this wave is
crutch-RETIREMENT only (engine now carries the hybrid + cost info the crutches taught) — no new
decision-level failure class routes to the guide; the spine is byte-frozen in behavior. The only two
live items are ENGINE/HARNESS (Ledger A2 attacker-salvage precedence; the model-behavior spiral ->
general-suggestions), neither a guide signal. Under the rotation test (no-guide-mod-of-substance AND
no-new-work-signal-at-guide-layer = candidate), deck109 qualifies: the revision is a maintenance
trim, not new play-quality guidance. Recommend rotating deck109 OUT next cycle, guide kept deployed
as a canary, after the trimmed guide gets one confirming corpus (to verify the retired crutches were
truly redundant, not load-bearing under a different draw).
