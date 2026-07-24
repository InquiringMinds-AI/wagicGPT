# Wave-24 reviewer brief (shared context for all 7 deck agents + synthesis)

Tenth cycle. Pool: 27, 102, 109, 133, 131, 137 (continuing) + **59 WB Persistence (GUIDELESS
Step-0 — first deck of the guideless-remainder era)**. deck140 rotated out last cycle.

⚠ PROCESS RULE: NEVER kill any wagic process (pkill FORBIDDEN); kill only your own PIDs.

## Corpus history this wave (two runs — REVIEW ONLY THE SECOND)

- FIRST run (matchups-20260724-073032-N9B-OVERFIRE) is **QUARANTINED**: the wave-24 N9
  discriminator's branch (b) over-fired 131 retracted_choice (11% fallbacks) — phrase-set
  matched hypothetical rules prose. NEVER read it as decision evidence.
- Hotfix d7ff8b6c4 retired branch (b) outright (its sole justifying case — the unpayable
  Yawgmoth offer — is structurally impossible since SacrificeCost::canPay gained
  candidate-existence in the same wave). Retraction now = second-contradictory-coded-index
  ONLY.
- **PRIMARY (review this): `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260724-125739`** — binary
  /tmp/wagic-d7ff8b6c4, which ALSO carries the brand-new PRE-GAME MULLIGAN PHASE (below).

Binary d7ff8b6c4 = wave-24 step-1 batch (5f78e2934) + pregame phase (86df3b482) + N9 hotfix.
Step-1 batch items under validation (from 5f78e2934): (a) SacrificeCost candidate-existence
(Yawgmoth-class unpayable offers GONE — deck133 owns); (b) mulligan-window fix (superseded
hours later by the pregame phase — historical only); (c) R-ADV-FACE adventure options carry
spell text not creature P/T (deck137 owns); (d) N9 discriminator → as hotfixed (only
contradictory-coded-index retracts; deck133's 1 fire this corpus needs a genuine/FP verdict);
(e) ANNOUNCE_X cap header ("You can afford X up to N..."; any X-deck seat); (f) decode-collapse
ANSWER-LOCKED RETRY (0 fires this corpus — check: were there any garbage replies it SHOULD
have caught, or is 0 correct?).

## THE PRE-GAME MULLIGAN PHASE (86df3b482 — first fleet outing, EVERY reviewer validates)

Every game now opens with the CR-conformant London mulligan before turn 0 (spec:
strategy-design/pregame-mulligan/rules-spec.md): turn-ordered declarations (starting player
first), simultaneous execution, full-7 redraw per mulligan, bottom-N at keep (translog
kind="bottom", one bundled PUT: ask), loop until all keep, then the 103.6 leyline window.
AT YOUR SEAT verify: (1) the mulligan ask rendered and the model's keep/mull decisions were
sensible (hand quality vs decision); (2) any "bottom" record: did it bottom its N WORST cards?
(3) game start integrity — first draw lands on the non-starting player's turn 1; (4) any
leyline-keyword card in an opening hand: did the 103.6 window fire? (5) ANY anomaly in the
phase (double asks, wrong N, narration confusion about the redrawn hand — a known cosmetic:
the "opening hand" narration line may reflect the pre-mulligan deal).

## Corpus data (the PRIMARY run)

- 21 games, **2 timeout-draws (first timeouts in 9 corpora — 27vs137 and 102vs133, both ~turn
  10 at the 2400s cap with even life; DECOMPOSE: slow grind vs a phase/engine stall — check
  decision counts, latency, and whether the pregame phase consumed unusual time)**, 7 adj.
- Win table: 109:5/6, 133:4/6, 27:3/6, 59:3/6, 102:2/6, 137:2/6, 131:0/6.
- Fallbacks 22/1293 = 1.7% RAW, but 19 are deck137's by-design convoke defers
  (deferred_to_heuristic). TRUE model-fallbacks: **3 (0.2% — cleanest corpus yet)**:
  deck133 s21 ask retracted_choice (verdict needed: genuine (a) fire?), deck137 s4 ask
  stale_echo, deck102 s5 priority unparsed_reply (classify precisely).
- **HOTFIX VALIDATED: retracted_choice 131 → 1.** Latency median 17.3s p90 73s. Retry fired 0×.
- deck131 0/6 needs a decisions-based decomposition (it was 2/6 in the quarantined run — but
  that run is not comparable evidence; judge THIS corpus's decisions).

## Per-seat focus

- **deck133**: Yawgmoth unpayable offers GONE (verify none rendered)? Your s21
  retracted_choice — genuine second-contradictory-index or residual FP (this is the hotfix's
  live acceptance test)? Wave-23 guide adds (Bloodghast fodder, Liliana caution) round 2.
- **deck137**: R-ADV-FACE — adventure options now carry spell text (the Usher misfire class
  gone)? Initial-guide round 2. Your 19 convoke defers: any harm? s4 stale_echo classified.
- **deck102**: your s5 priority unparsed classified; DFC round 2 (Tergrid cast again? steal
  taken this time — the guide's new take-the-steal line); crutch-demotion holds.
- **deck109**: trimmed-guide round 3 (Pillar restoration validated?); attackers
  declared-vs-recorded round 3; 5/6 decisions-read.
- **deck131**: 0/6 — decompose honestly (draw/matchup vs play); Guttersnipe tapped-read guard
  round 2 (did it fire?); the wave-23 outage game is history (this is a fresh corpus).
- **deck27**: free-kills-first round 3; ahead-degeneration watch (retry never fired — did the
  collapse class simply not occur, or did answer-first absorb it?).
- **deck59 (GUIDELESS Step-0)**: baseline review per the skill's Step-0 doctrine; WB
  Persistence = wrath/recursion axes; write the INITIAL guide; persist mechanics are a fresh
  engine surface — note representation gaps precisely.
- **EVERYONE**: pregame-phase validation items above; REPPENALTY=1.05 second corpus (any
  degradation?).

## Layer-routing (unchanged)

Engine/harness -> notes.md with seq repros. Guide -> strategy.txt per
wave23/strategy-writing-skill.md (CURRENT skill). Skill proposals -> skill.md. Core ->
general-suggestions.md (18 consecutive PASSes). Win column context-only. Rotation test:
no-guide-mod AND no-new-work-signal = candidate.
