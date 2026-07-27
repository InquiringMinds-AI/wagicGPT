# Wave-31 reviewer brief (shared context for all 7 deck agents + synthesis)

Seventeenth cycle. Pool: 137, 146, 139, 148, 122, 152 + **deck158 (Orcs of Mordor) —
Step-0 guideless debut: RB amass**. deck136 rotated out (tutors clean, guide frozen,
Ugin-only residual didn't hold the seat).

PROCESS RULE: NEVER kill any wagic process (pkill FORBIDDEN); kill only your own PIDs.

## The step-1 batch under live validation this corpus (binary cf7cb2e53)

(h1) **N-139h ManaCost OOB crash fix** — the ManaCost::remove crash was an out-of-bounds
heap read (under-sized cost vector, unchecked indexing; the assert's own min-guard made
the old subtraction unfireable). Bounds guards on both remove() overloads + release clamp.
EVERYONE: the corpus itself is the validation — any crash/abort at your seat is a
regression finding of the first order. deck139: Pollywog-mutate casts should now RESOLVE.
(h2) **Convoke divergence A+B FIXED, C KNOWN-OPEN** — (A) maxAnnounceableX now credits
untapped creatures per CR 702.51 (the missing X-cap was the perfect-predictor mechanism:
it suppressed ANNOUNCE_X → silent no-op); (B) convoke-X payment residual negative pool
floored (gold fixture march_convoke_x_credit: {G}{W}{W}+3 convokers → X=2, pool 0).
(C) NOT fixed, wave-32 ledger: AIPlayerGPT::FindCardToPlay ~line 3991 —
aiForcedCandidate validation (Baka FindCardToPlay pMana '*') rejects some convoke casts
offerable() approved → silent defer. deck137: arrival-traced convoke table
(offered/announced/paid/resolved per pick); **defers should now be ONLY the divergence-C
shape**; A+B observable as ANNOUNCE_X firing on mana-short convoke.
(h3) **N-136a FULLY CLOSED (reveal chooser)** — wave-30's filter read the WRONG chooser
(multi-option reveals pre-armed option two's unrestricted shuffle chooser when option one
was impossible → everything "eligible"); eligibility now built from abilityOne's own
target() predicate. Live: 3 empty-graveyard reveals = NO model call (vs 42-option phantom
asks). Fixture mausoleum_multi_empty rides the true gap. ANY tutor/reveal seat: asks must
be eligible-count options or no-ask at all.
(h4) **N-152a HONEST PARTIAL (MDFC lands)** — the engine structurally cannot place an
MDFC land's back face via AI seams (flip works, play offered, put-into-play defers on
front-face isflipped==0) = NEW ENGINE-R LEDGER ITEM. The misleading annotation is
replaced with truthful both-faces+colors text. deck152: cite the new MDFC land text;
back-face land placement remaining blocked is EXPECTED (engine-lane, not a seat finding —
Teferi still blocked).
(h5) **N-152b DONE** — display-toggle-only priority windows auto-pass without a model
call (verified live; confirm the ask-count drop at 152).
(h6) **N-146g DONE** — magnitude eval skips planeswalkers. deck146: no more bogus
{right now: ...} magnitudes on walker lines.

## Corpus data

- **PRIMARY**: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260727-025026 — 21 games, binary
  cf7cb2e53, -T 3000, REPPENALTY=1.05.
- Win table: **158:5/6 (STEP-0 GUIDELESS DEBUT — strongest Step-0 of the campaign)**,
  148:4/6, 122:3/6, 152:3/6, 137:3/6, 146:2/6, 139:1/6. 0 timeouts/draws; only **3 adj
  at cap** (vs wave-30's 12 — pool grindiness dropped with 136→158). 36 gameend logs,
  integrity clean.
- **ZERO crash signatures in all 21 game stderr files — N-139h corpus gate PASSED.**
- **Fallbacks: 6/1,432 (0.42%)** — 2 defers (deck137 vs152 seq34 t12 — divergence-C
  candidate, VERIFY the shape; deck158 vs146 seq25 t13 — NEW seat, decompose), 3
  truncated_abandoned (122 vs137; 137 vs158; 122 vs158), 1 unparsed (146 vs122).
- Decision mix: 897 ask, 204 attackers, 118 priority, 95 blockers, 24 bottom, 14
  reveal, 2 defer.

## Per-seat focus

- **deck137**: h2 arrival-traced convoke table — every offered convoke:
  offered/announced/paid/resolved; classify every defer (divergence-C shape or NEW);
  ANNOUNCE_X on mana-short convoke = A working. If defers are exclusively divergence-C
  and offered convokes resolve, the convoke arc at this seat is engine-lane only —
  longest-serving seat, weigh rotation per the sharpened gate.
- **deck122**: NEAR-TERMINAL EXIT DUE THIS WAVE (wave-30 projection after the 6/6
  first-guided perfecta). Confirm teaches still validated + frozen; N-122a Felidar probe
  disposition; if steady, ROTATE OUT with the exit write-up.
- **deck148**: near-terminal — confirm construction-ceilinged state, then ROTATE OUT +
  construction hand-off notes.
- **deck146**: near-terminal projection wave-32. h6 walker-magnitude check; Kaya/Lolth
  exercised at last (4th corpus — if still unexercised, that alone doesn't hold the
  seat).
- **deck139**: h1 validation seat — crash-free corpus + Pollywog-mutate casts RESOLVING
  (arrival-traced, not defer-counted). Rotation gated on this validation. Guide teaches
  steady-state read.
- **deck152**: h4 truthful MDFC text cited + h5 auto-pass confirmed (ask-count vs
  wave-30); werewolf day/night + Class level-up steady state; initial guide validation
  (first guided outing — did the wave-30 draft's teaches land?).
- **deck158 (Step-0, guideless)**: FIRST READ — RB amass: army-token creation/growth
  renders, amass target choices, sac/aristocrat lines if present. Baseline decision
  quality for the initial guide draft per the CURRENT skill.
- **EVERYONE**: h-batch steady state; any crash = top-priority finding; reveal asks
  eligible-count-or-silent; parser contradictions verbatim.

## Layer-routing (unchanged)

Engine/harness -> notes.md with seq repros. Guide -> strategy.txt per
wave30/strategy-writing-skill.md (CURRENT skill edition). Skill -> skill.md. Core ->
general-suggestions.md (25 consecutive PASSes). Rotation: UNSENTIMENTAL (new-work
exhaustion; three near-terminal exits projected this wave — 122, 148, and 137 if the
convoke arc closes).

## Open ledger carried (cite if exercised)

Convoke divergence C (FindCardToPlay aiForcedCandidate reject → silent defer; fix
direction: accept convoke on offerable()'s payability OR loud re-ask); N-152a MDFC
back-face land placement (engine-R); N-122a Felidar probe; N-139g under-card statics
probe; Rankle mode-bleed watch; L-18-1 planning tax (2 witnesses); N-134b/d; L2
cross-zone dup-name; ETB header card-name; N8; Azcanta; undying; commit-lock;
prose-reversal; untapped-tagging; I-4.
