# Wave-27 reviewer brief (shared context for all 7 deck agents + synthesis)

Thirteenth cycle. Pool: 131, 137, 18, 22 (holdovers; 18+22 now FIRST-GUIDED — their wave-26
initial guides deployed) + **deck134 (Urza's Control, mono-U tron), deck93 (Relentless
Rats), deck136 (Yarok of Pranks) — GUIDELESS Step-0 debuts**. deck27/102/133 rotated out
last wave (canaries 15).

PROCESS RULE: NEVER kill any wagic process (pkill FORBIDDEN); kill only your own PIDs.

## The step-1 batch under live validation this corpus (binary ffec79fd8, commit ffec79fd8)

(d1) **R-DUPLICATE-NAME-INSTANCE fixed** — same-named battlefield permanents render stable
`#N` handles (battlefield order, collision-only) on board/A/B/target lines. deck93 (36
identical Rats) is the deliberate stressor: EVERY reviewer with duplicate names at their
seat cites renders + binding correctness; deck93 reviewer audits systematically.
(d2) **R-CHANGELING-TYPE-INFERENCE fixed** — changelings annotate live tribal types
(`[changeling: counts as Giant]`) on hand/decklist/bottom surfaces when a type-keyed
consumer exists. deck22 validates (Universal Automaton at mulligan/bottom — last wave's
2-game cost).
(d3) **Parser stash-leak general fix** (the REAL Tergrid poisoner — and!() payload leaking
across auto= lines; doubleside exonerated). Watch: any card whose ability unexpectedly
works/changed this corpus.
(d4) **R-DFC-BACKFACE fixed** (displayed-face cast resolves the displayed face). No
Kaldheim-god DFC in pool — steady-state only.
(d5) **Deck-load loud rejection** (set-hint silent drop now warns on stderr).
Carry: c-batch (wave-26) steady state — target enumeration, ETB annotations, pump renders,
parser hardening precedence, may-ask object naming.

## Corpus data

- **PRIMARY**: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260725-193608 — 21 games, binary
  ffec79fd8, -T 3000, REPPENALTY=1.05.
- Win table: **22:5/6 + 18:5/6 (both FIRST-GUIDED — the Step-0->guided pipeline again)**,
  136:4/6 (Step-0), 131:3/6, 93:2/6 (Step-0), 137:2/6, **134:0/6 (Step-0)**.
  0 timeouts/draws; 7 adjudicated at cap (28 gameend logs = 42-14, exactly those 7 —
  integrity clean).
- **Fallbacks: 30/1,281 decisions** — 23 defers (deck137 convoke class, UP from 15 — deck137
  reviewer: decompose the rise) + **7 non-defer: 5 ask + 2 priority**:
  - ⚠ **deck136 ASK CLASS (4 records, 3 games, ~seq7/turn3 deterministic): the model
    REFUSES the ask as premise-invalid** — sample reply: "Legion's End is currently in your
    GRAVEYARD, not on the stack... you cannot be asked to choose a target for a spell that
    has already resolved... Opponent's Battlefield: 0 creatures." → unparsed_reply fallback.
    ENGINE-SUSPECT: a targeting ask issued for a resolved/graveyard spell or with zero legal
    targets (violates BOTH the no-ask-without-legal-options invariant and ask freshness).
    deck136 reviewer: this is your TOP item — decompose all 4 records (what ability actually
    asked, engine state, was the ask stale or target-less), seq repros for notes.md.
  - deck22 vs? s3 turn 1 ask fallback (1 record) — deck22 reviewer decomposes.
  - deck131 vs134 s39/s40 turn 18 priority x2 (adjacent records, same game) — deck131
    reviewer decomposes (transient latency? render? parser?).
- Retry fired 0x.

## Per-seat focus

- **deck134 (Step-0, 0/6)**: FIRST READ + 0/6 decomposition (mono-U tron control in an
  aggro pool — construction? decisions? both?). Initial guide draft per the wave-26 skill.
  Counterspell windows + ETB-blink may-asks (c5/d-batch surfaces) — cite renders.
- **deck93 (Step-0, 2/6)**: FIRST READ + **d1 instance-handle SYSTEMATIC AUDIT** (36
  identical Rats: do board/A/B/target lines carry #N? does the model reference them? any
  binding error = top-severity finding). Initial guide draft.
- **deck136 (Step-0, 4/6)**: FIRST READ + **the ask-refusal class above (TOP)**. Rankle
  may-asks + Liliana/Ugin loyalty surfaces (unexercised class — cite renders). Initial
  guide draft.
- **deck18 (5/6, FIRST-GUIDED)**: guide validation (deck59 pattern — 4/6 Step-0 -> 5/6
  guided): did the Dodger evasion order fire? flyer-block RACE off-case? trust-line tax
  reduction (compare spike profile vs wave-26)? Rotation test applies from next wave.
- **deck22 (5/6, FIRST-GUIDED)**: guide validation + **d2 changeling annotation live check**
  (Automaton keeps/bottoms — did the misread die?); decompose your s3 turn-1 ask fallback.
- **deck131 (3/6, up from 2/6)**: favorable-block exception first corpus; decompose the two
  turn-18 priority fallbacks (vs134); PROJECTED ROTATION — wave-26 said rotate if the edit
  validates and nothing new appears; apply unsentimentally.
- **deck137 (2/6)**: convoke defers 15->23 — why the rise (new pool shapes? more convoke
  windows?); blockers floors round 3; closing-speed frontier reassessment; adventure
  steady state.
- **EVERYONE**: d1-d5 + c-batch steady state; flip-thrash livelock watch (none expected —
  no Kaldheim DFCs in pool); parser-hardening precedence (any answer-contradiction ->
  decompose verbatim).

## Layer-routing (unchanged)

Engine/harness -> notes.md with seq repros. Guide -> strategy.txt per
wave26/strategy-writing-skill.md (CURRENT skill). Skill -> skill.md. Core ->
general-suggestions.md (21 consecutive PASSes). Rotation test: no-guide-mod AND
no-new-work-signal = candidate; verdicts UNSENTIMENTAL (termination = new-work exhaustion;
guideless roster 141).

## Open ledger carried (cite if exercised, don't re-derive)

Flip-thrash LIVELOCK (HIGH — AIPlayerBaka::getEfficiency std::rand isFlipped toggle +
AATurnSide AI reset; blocks live Kaldheim-DFC play; wave-28 step-1 candidate); ETB header
card-name (engine seam); retracted-recode PLAN-corroboration discriminator (LOW); N-18c
Mobilization discoverability; N8 blocker-gang-chump; Azcanta probe; undying
trade-annotation; mulligan surface 4 corpora unexercised; commit-lock watch; prose-reversal
watch; untapped-tagging (below-bar); Guttersnipe all-tapped WATCH; I-4 wobbles;
reversal-spiral benign WATCH; deck27 bottom-phase clause (unvalidated, seat rotated).
