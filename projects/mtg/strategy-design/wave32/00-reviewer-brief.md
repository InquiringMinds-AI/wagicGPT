# Wave-32 reviewer brief (shared context for all 7 deck agents + synthesis)

Eighteenth cycle. Pool: holdovers 146, 139, 152, 158 + THREE Step-0 guideless debuts:
**deck105 (Infected Wounds — BG infect: poison alt-win, damage that doesn't reduce
life; engine poison support confirmed pre-corpus)**, **deck36 (Master of Ether — UB
artifacts: dynamic artifact-count lord, artifact lands, {0}-cost casts)**, **deck116
(Time of Need — GU cheat-into-play via Piper/Amulet, tutors, 1-of Eldrazi)**. Rotated
out wave-31: 122 (near-terminal confirmed; N-122a since probe-PASSED — engine correct),
148 (construction hand-off), 137 (guide-lane exhausted; convoke arc continues via probe).

PROCESS RULE: NEVER kill any wagic process (pkill FORBIDDEN); kill only your own PIDs.

## The step-1 batch under live validation this corpus (binary 7fabd9bd0)

Wave-31's dominant defect shape was PATH-SCOPED FIXES (three recurrences). Every (i)
item below names its emitter sweep; cite the post-fix string from YOUR seat's paths.

**Mana/engine lane:**
(i1) **N-152c/N-122b FIXED** — the generic-fill branch skipped colour assignment, so
multi-pip spells payable off two-ability duals were silently not offered. PREDICTION:
zero castable-but-not-offered instances; any seat whose mana line shows >=2 colours and
holds a 2-pip spell should show its Cast line. deck152: Sigarda-class windows now offer.
(i2) **N-137a convoke colour scan + N-137b announce routing + crash-fix 7fabd9bd0**
(convoke stale-payment lifecycle). No pool deck plays convoke — validated by probe
(see probe-results-*.md); cite only if a convoke card appears via steal/generate.
(i3) **N-146h PARTIAL** — Agadeem's silent no-op was a NEGATIVE MENU INDEX silently
dropped (ledger hypothesis falsified: ANNOUNCE_X was reached); index now priced off
maxAnnounceableX + clamped. deck146: Agadeem casts must not burn mana silently; a
SECOND layer may remain — if the cast still fails it must now fail LOUDLY; trace it.
(i4) **N-148c lifelink-ueot class FIXED** (grant had no resolve()). PREDICTION: every
"X deals N" by a lifelink-ueot-granted creature is followed by "life +N". 21+29
primitive constructs — any seat may witness.

**Decision-flow lane:**
(i5) **DIVERGENCE-C EXIT: LOUD RE-ASK.** Validation rejects now re-ask with the option
removed (`[RE-ASK n]` header, cap 2, then Baka). PREDICTION: zero silent window burns;
translog markers `validation_reject_reask` / `_exhausted` replace bare defers. EVERYONE:
report every marker with its trigger shape.
(i6) **N-122d truncation salvage** — compliant head `BLOCKS:` lines survive truncation;
label-scoped declines no longer read as global; irrecoverable truncations now get the
HEURISTIC's blocks (`truncated_abandoned_heuristic`), not blanket no-blocks.
(i7) **N-139i London bottoming FIXED at root** (target re-clamped against shrinking
hand). PREDICTION: every "KEEPS after N mulligan(s)" is followed by exactly N bottoms
with constant labels; a 3-card-hand-after-7-mulls cannot recur. deck139 focus.

**Render lane (PARSETEST 220/0):**
(i8) **N-122c + N-139k combat tags** — attackers render `[tapped - attacking]`;
summoning-sick adds ", but CAN block". PREDICTION: zero paradox verbalizations, zero
"no untapped creatures to block" misreads. EVERYONE.
(i9) **N-139j** zero-power attackers annotated `[deals 0 ...]`. deck139: no 0/3 Grazer
attacks (or if one occurs, the reply must not claim damage).
(i10) **N-148b/a instance handles + forward equipment lists** — `[attached to: X #N]`;
`{attached:` forward renders exist. deck36 (artifact deck!) is the natural witness seat.
(i11) **N-148d creature type markers on hand lines** — zero "zero creatures" mulligans
of hands containing one.
(i12) **N-158g count-first mana line** (`Mana available: 5 total (...)`). PREDICTION:
zero "I cannot afford" declines of offered payable casts; deck158's tax spikes on land
drops should collapse.
(i13) **N-146i BROAD card text on Cast lines.** The Acererak belief class dies; prompt
grows ~0.4-1.1k/ask — flag any length-related degradation (this is the batch's main
cost risk).
(i14) **magnitude cluster** — amass previews `{right now: Army 7/7 -> 9/9}` (only on
fixed-count on-resolution amass; multi-branch/trigger-gated correctly suppressed);
Foray damage post-amass; Feed-the-Swarm per-target life costs in the targets list;
counter narration with count+source. deck158 focus.
(i15) **N-152d/e transform DFC honesty** — `(printed X/Y)` reads the displayed face
(no tag when delta 0); transform Flip-Side text truthful on BOTH emitter paths, no
literal "backside". deck152 focus.
(i16) AIStats missing-file no longer logs FATAL (crash-grep hygiene for the 3 debuts).

## KNOWN-OPEN (cite, don't re-diagnose)

- **N-139m (NEW, HIGH)**: a creature spell WITH MUTATE cast while Pollywog Symbiote is
  on the battlefield pays, loots, and the card VANISHES (fixture-proven; discount path
  exonerated). deck139: count occurrences; do NOT teach around it (engine-lane).
- **N-158e flash windows**: Baka's computeActions builds no casts outside main phases
  on its own turn (mechanism located, fix deferred — shared-path). Bowmasters
  hold-for-flash teaches CANNOT fire; don't count that against the guide.
- N-146h second layer (see i3); Agadeem may still not resolve — loudly.
- Convoke divergence-C triggers now re-ask (i5) — the deferred-cast class is EXPECTED
  to appear as re-ask markers, not defers.

## Corpus data

- **PRIMARY**: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260727-115847 — 21 games, binary
  7fabd9bd0, -T 3000, REPPENALTY=1.05.
- Win table: **105:6/6 (STEP-0 GUIDELESS PERFECTA — first ever by a Step-0 seat)**,
  36:4/6, 158:3/6, 152:3/6, 146:2/6, 139:2/6, 116:1/6. **0 timeouts/draws, only 1 adj
  at cap** (campaign low; 20 decisive gameends, integrity clean).
- **ZERO crash signatures across all 21 stderr files** — with THREE new decks and i16
  making the scan clean.
- **Fallbacks: 3/1,117 (0.27%, ties campaign record) — ZERO defers, ZERO
  truncated_abandoned**: 1 retracted_choice (158 vs139), 2 unparsed_reply (158 vs152,
  139 vs152). The i5/i6 machinery produced zero silent burns; note: **zero
  `validation_reject_reask` markers fired — no validation rejects even occurred in this
  pool** (no convoke/X-stress decks), so i5 remains live-validated only by the probe.
- Decision mix: 703 ask, 144 attackers, 96 priority, 59 blockers, 18 reveal, 15 bottom.

## Per-seat focus

- **deck146 (HOLD, exit projected wave-33)**: revised guide's first corpus (three false
  teaches corrected — Acererak bounce, Hive cost, MDFC removal); i3 Agadeem trace; i13
  card-text-on-cast validation (the belief class that cost the vs148 game); Verse
  take-rate recovery (16 offered/4 taken -> should rise); zero removal-declined-for-
  Acererak windows.
- **deck139 (HOLD)**: i7 bottoming; i8/i9 tags; revised guide (mulligan floor, dual-land
  counting — the 2.50 mull/game spiral should end); N-139m occurrence count; mutate
  steady state; latency tail.
- **deck152 (Step-0 seat, 2nd guided corpus)**: i1 Sigarda-class offers; i15 DFC
  honesty; deploy-floor rule (the two fatal holds); werewolf-rewrite validation (flips
  are COMMON now teach); Class/lords steady state.
- **deck158 (2nd corpus, 1st with revisions)**: i12 mana-count belief repair (the
  vs148 death); i14 magnitude cluster; amass steady state; Bowmasters (with N-158e
  known-open); guide first-outing validation.
- **deck105 (Step-0)**: FIRST READ — poison/infect: how do poison counters render, does
  the alt-win fire, does -1/-1 counter damage narrate distinctly from damage? Baseline
  decision quality for the initial guide.
- **deck36 (Step-0)**: FIRST READ — artifact-count lord dynamics, {0}-cost casts,
  artifact lands; i10 attachment handles at the natural witness seat.
- **deck116 (Step-0)**: FIRST READ — cheat-into-play (Elvish Piper-style activations),
  tutor/search renders (eligible-count rule), 1-of Eldrazi as the absent-engine-card
  risk shape.
- **EVERYONE**: crash gate (0 signatures — 3 new decks rotate in, i16 makes FATAL greps
  clean); re-ask markers with trigger shapes; i8 tag renders; parser contradictions
  verbatim.

## Layer-routing (unchanged)

Engine/harness -> notes.md with seq repros. Guide -> strategy.txt per
wave31/strategy-writing-skill.md (CURRENT edition — HL1 retroactive card-fact audit is
now a HARD GATE on freeze/exit; HL5 path-scoped sweep; HL6 no manufactured frequency
claims). Skill -> skill.md. Core -> general-suggestions.md (26 consecutive PASSes).
Rotation: UNSENTIMENTAL.

## Open ledger carried (cite if exercised)

N-139m (top); N-158e; N-146h layer 2; N-152a MDFC back-face land placement (engine-R;
Teferi NOT blocked); N-139g under-card statics; Rankle mode-bleed; L-18-1 planning tax
(length-only watch); N-134b/d; L2 cross-zone dup-name; ETB header card-name; N8;
Azcanta; undying; commit-lock; prose-reversal; untapped-tagging; I-4.
