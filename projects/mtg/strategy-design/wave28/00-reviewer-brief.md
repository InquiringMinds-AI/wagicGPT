# Wave-28 reviewer brief (shared context for all 7 deck agents + synthesis)

Fourteenth cycle. Pool: 137, 18, 22, 134, 93, 136 (134/93/136 now FIRST-GUIDED — wave-27
initial guides deployed) + **deck146 (Orzhov Dungeons) — Step-0 guideless debut; loyalty
(Kaya x2) + venture/dungeon = CAMPAIGN-FIRST surfaces**. deck131 rotated out last wave
(canaries 16).

PROCESS RULE: NEVER kill any wagic process (pkill FORBIDDEN); kill only your own PIDs.

## The step-1 batch under live validation this corpus (binary ad8930961)

(e1) **CONVOKE-NONRESOLUTION fixed** — forced convoke picks keep alternative pricing,
remainder floats from lands, isPaymentSet re-passes. The fix agent's live check showed 0
defers in 2 games and a fixture proves Loxodon resolves off 1 Plains + 4 taps. ⚠ BUT this
corpus still logs 16 defers, ~14 at deck137 seats — deck137 reviewer: THE headline —
decompose every defer (is it still convoke? did the deferred cast RESOLVE downstream —
apply the RESOLUTION-VERIFICATION rung, count Loxodon/March battlefield arrivals vs
attempts, compare wave-27's 2/23 + 0/17).
(e2) **Phantom chooser removed** (Legion's End card-script; engine gate deliberately NOT
shipped — regressed Black Vise). deck136: verify 0 phantom asks + Legion's End still
destroys/exiles correctly live.
(e3) **Flip-thrash livelock fixed** (scorer no longer mutates isFlipped; AATurnSide
coherent). No Kaldheim DFCs in pool — steady-state watch only; 'Flip Side' menu
pseudo-action is a ledgered suppression candidate, note any sighting.
(e4) **Land identity tags** — hand/mulligan/bottom/decklist now render
`Swamp (land: taps for {B})` etc. deck93: did mulligan land-blindness die (3-Swamp hands
kept)? EVERYONE: any prompt-noise or confusion from the tags at your seat.
(e5) **Annotation-echo tolerance** (+4 PARSETEST) — echoed [bracketed] tails no longer trip
name-matching. deck22: your wave-27 stale_echo shape should be structurally gone.
Carry: d-batch + c-batch steady state.

## Corpus data

- **PRIMARY**: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260726-050449 — 21 games, binary
  ad8930961, -T 3000, REPPENALTY=1.05.
- Win table: **18:6/6 (first-guided validation perfecta — deck59-arc repeat)**, 93:4/6,
  22:3/6, 146:3/6 (Step-0), 137:3/6, 136:2/6, **134:0/6 AGAIN (first-guided — the guide
  did not move the record; reviewer must decompose construction-vs-decisions honestly)**.
  0 timeouts/draws, 4 adj (34 gameend logs = 42-8 — integrity clean).
- **Fallbacks: 18/1,072** — 16 defers (10+4+2 across three deck137 games = 14 at 137;
  1 deck134; 1 deck146) + 2 ask/retracted_choice (find + decompose: which seats, which
  shape — the wave-26 'So, CHOICE:' class? the annotation-echo class supposedly fixed?).
  Retry 0.

## Per-seat focus

- **deck146 (Step-0, 3/6)**: FIRST READ — campaign-first surfaces: venture/dungeon room
  choices (how do they RENDER? does the model navigate rooms sanely?) + Kaya loyalty asks
  (render + decision quality). Decompose your 1 defer. Initial guide draft per the
  wave-27 skill.
- **deck137 (3/6)**: e1 CONVOKE RESOLUTION — the headline (see above). Also blockers
  floors round 4; closing speed re-verdict WITH working payoffs (last wave named it
  engine-ceiling BECAUSE convoke was dead — with Loxodon live, is the guide's attack
  doctrine now sufficient?).
- **deck93 (4/6, FIRST-GUIDED)**: e4 land tags + guide validation (land-blindness dead?
  3-Swamp hands kept? cite mulligan replies verbatim); instance-handle steady state;
  2/6->4/6 decomposition.
- **deck134 (0/6 AGAIN, FIRST-GUIDED)**: the guide's teaches — Map cracked now? Stonehorn
  lock assembled? — validate each teach even where games were lost; then the honest call:
  is 0/6 construction-terminal (clockless control in aggro pool) with the guide having
  done all it can? Flag to ROSTER OWNER if so; decompose your 1 defer.
- **deck136 (2/6, FIRST-GUIDED)**: e2 phantom-chooser verification; guide validation
  (attack floor, removal priority); 4/6->2/6 decomposition (pool shift? variance?).
- **deck22 (3/6)**: e5 echo-tolerance verification; guide steady state; 5/6->3/6
  decomposition; rotation test (wave-27 projected exit — apply it).
- **deck18 (6/6)**: 6/6-hides-taxes audit; rotation test with finality (wave-27 projected
  exit; a 6/6 validation corpus on a frozen guide = the deck59 exit shape exactly).
- **EVERYONE**: e-batch + d/c steady state; parser precedence contradictions decomposed
  verbatim; land-tag noise check.

## Layer-routing (unchanged)

Engine/harness -> notes.md with seq repros. Guide -> strategy.txt per
wave27/strategy-writing-skill.md (CURRENT skill). Skill -> skill.md. Core ->
general-suggestions.md (22 consecutive PASSes). Rotation: no-guide-mod AND
no-new-work-signal = candidate; UNSENTIMENTAL (termination = new-work exhaustion;
guideless roster ~140).

## Open ledger carried (cite if exercised)

'Flip Side' menu no-op suppression candidate (AIPlayerGPT, sanctioned class);
March-via-convoke X-announcement gap (deeper modeling, plain X-cast works); ETB header
card-name; N-134b flashback-cost display; N-134d Mulldrifter double-ask; N-B
summoning-sick marker; L2 cross-zone dup-name; retracted-recode discriminator (LOW); N8;
Azcanta; undying; commit-lock; prose-reversal; untapped-tagging; Guttersnipe WATCH
(seat rotated — dormant); I-4 wobbles; L-18-1 planning tax; favorable-block inconsistency
WATCH (seat rotated — dormant); illegal gang-block WATCH.
