# Wave-30 reviewer brief (shared context for all 7 deck agents + synthesis)

Sixteenth cycle. Pool: 137, 136, 146, 139, 148, 122 (122/139/148 now FIRST-GUIDED) +
**deck152 (Bant Midrange) — Step-0 guideless debut: werewolf day/night transform (first
live outing since the flip-thrash fix), Class level-up, GWU 3-color mana**. deck93 rotated
out (canaries 19).

PROCESS RULE: NEVER kill any wagic process (pkill FORBIDDEN); kill only your own PIDs.

## The step-1 batch under live validation this corpus (binary 6e168aa05)

(g1) **Convoke payability-before-offer** — unaffordable convoke variants are no longer
OFFERED (offer-true implies pay-completes; X-convoke only at payable X>=1). deck137: the
FINAL exit gate — defers should be ~0 and every offered convoke should resolve; decompose
your 2 residual defers (the cast-mode-menu cascade is the known out-of-scope seam — check
if they're that shape).
(g2) **PUT-predicate filter** — tutor reveals offer only predicate-legal cards; empty ->
no ask. deck136: tutor windows should be clean; the phantom-tutor class dead.
(g3) **Mutate render batch** — verb-labeled remove-from-hand asks (DISCARD/SACRIFICE/...),
mutate steps role-named with CR 725 context, merged piles as ONE line, unified cost label.
deck139: the render gaps N-139a-d should be closed; loot decisions should stop misreading
as targeting. EVERYONE: any loot/discard ask at your seat — cite the new framing.
(g4) **Tapped-tag reword** `[tapped - cannot attack or block this turn]` — the wave-29
residual misread class should die. (g5) **Dungeons-completed line** at 146.

## Corpus data

- **PRIMARY**: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260726-181642 — 21 games, binary
  6e168aa05, -T 3000, REPPENALTY=1.05.
- Win table: **122:6/6 (FIRST-GUIDED PERFECTA — 4th pipeline validation)**, 136:4/6,
  146:3/6, 137:3/6, 148:2/6, 152:2/6 (Step-0), 139:1/6. 0 timeouts/draws; **12 adj at cap
  (HIGH — grindiest pool yet; 18 gameend logs = 42-24, integrity clean)**. Synthesis
  should note the adjudication rate as a pool-composition property.
- **Fallbacks: 4/1,474 (0.27% — campaign record)** — 2 defers (deck137 vs? game — the g1
  residual), 1 priority unparsed (deck152), 1 priority unparsed (deck146). Retry 0.

## Per-seat focus

- **deck137 (3/6)**: g1 FINAL exit gate — resolution table for every convoke pick
  (offered/announced/paid/resolved); decompose the 2 defers (cast-mode cascade shape? or
  new?); if defers are the known out-of-scope seam AND all offered convokes resolved, the
  convoke arc is CLOSED at this seat -> ROTATE OUT per the sharpened gate (longest-serving
  seat, in pool since wave-22 — apply finality; the menu-cascade residual is engine-lane,
  not a seat hold).
- **deck136 (4/6)**: g2 verification (tutor windows clean); near-terminal check (MV3
  validated last wave; if tutors clean + guide frozen + Ugin still the only unexercised
  surface, weigh rotation honestly — Ugin alone may not hold a seat under new-work
  exhaustion).
- **deck122 (6/6, FIRST-GUIDED)**: perfecta validation (deck59/18 shape): all guide
  teaches validated? 6/6-hides-taxes audit; threshold discipline held (no more racing own
  life down)? Felidar/40-life line pursued or still unexercised (N-122a probe stands)?
  If teaches validated + frozen: NEAR-TERMINAL -> wave-31 exit projection.
- **deck139 (1/6, FIRST-GUIDED)**: g3 verification (mutate renders — merged pile ONE line,
  role-named steps, verb-labeled loots; cite before/after at your seat); guide validation
  (Apex-mutate-only teach, fresh-body-under-aggro); 2/6->1/6 decomposition (drop with a
  guide = harshest audit: did a teach mislead, or pool/variance?); latency tail vs
  wave-29's 195-199s spikes.
- **deck148 (2/6, FIRST-GUIDED)**: guide validation (attack floor fired? gear-AM-first?
  Duelist double-strike?); 1/6->2/6 read; construction-ceilinged reassessment.
- **deck146 (3/6)**: g5 completed-line check; Kaya/Lolth exercised at last (3rd corpus —
  if still unexercised, weigh whether that alone holds the seat); decompose your 1
  unparsed priority; steady state.
- **deck152 (Step-0, 2/6)**: FIRST READ — werewolf day/night transform LIVE (the
  flip-thrash fix's transform steady state: do day/night flips render coherently? any
  isFlipped weirdness?); Class level-up surfaces (render + decisions); GWU mana (the
  dual-land oracle fix under 3-color stress). Decompose your 1 unparsed priority. Initial
  guide draft per the wave-29 skill.
- **EVERYONE**: g-batch steady state; verb-labeled asks wherever loots fire; parser
  contradictions verbatim.

## Layer-routing (unchanged)

Engine/harness -> notes.md with seq repros. Guide -> strategy.txt per
wave29/strategy-writing-skill.md (CURRENT skill — S-93a/b established; verify-oracle rail
covers render numbers + no-ordinals; construction categories: terminal / ceilinged).
Skill -> skill.md. Core -> general-suggestions.md (24 consecutive PASSes). Rotation:
UNSENTIMENTAL (new-work exhaustion; guideless roster 136 after 152's guide).

## Open ledger carried (cite if exercised)

Cast-mode-menu convoke cascade (the g1 out-of-scope seam); N-122a Felidar probe; N-139g
under-card statics probe; Rankle mode-bleed watch; L-18-1 planning tax (2 witnesses);
N-134b/d; L2 cross-zone dup-name; ETB header card-name; N8; Azcanta; undying; commit-lock;
prose-reversal; untapped-tagging; I-4.
