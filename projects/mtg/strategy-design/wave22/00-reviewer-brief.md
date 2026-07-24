# Wave-22 reviewer brief (shared context for all 7 deck agents + synthesis)

Eighth per-deck-rotation cycle. Pool: **135, 62, 27, 102 (continuing) + 109 Mono-R Aggro, 44
Faerie Tempo, 140 RBW Control (ROTATED IN — older guided decks re-validating under the current
engine; find each one's most recent prior review under strategy-design/wave*/deckN/ for baseline
expectations; their guides were written many engine-generations ago).** decks 14/35/49 rotated
OUT last cycle as converged (guides stay deployed as canaries).

⚠ PROCESS RULE for every agent: NEVER kill any wagic process (`pkill`/`pkill -x wagic` is
FORBIDDEN — a corpus may be running; kill only your own PIDs). The wave-22 corpus #1 was
destroyed this way and had to be re-run.

Binary 1d8ed8194 — delta since wave-21's 3be7f5a67 (the wave-22 step-1 batch; live-UNVALIDATED,
THIS CORPUS is the validation):
(a) **ENGINE-F1 fetchland fizzle FIXED** — a 2016 guard in payTheManaCost's zero-cost branch
    refused ANY land with the land-drop spent, including an IN-PLAY fetchland being activated:
    every AI fetch crack after a same-turn land drop silently no-oped. Now scoped to lands
    played from hand. Detector for validation: a chosen "Put in Play with <fetchland>" followed
    in events by `<fetchland>: battlefield -> graveyard` + `<land>: library -> battlefield`.
(b) **HARNESS-N9 retraction detector reference-scoped** (phrase must precede line-leading
    PLAN: AND reference the chosen option). Wave-21's 4 false positives are suppressed-class.
(c) **Echo family**: "<spell> targeting <target>" prefix strip (anchored to pending source);
    consumePlan now label-filters on CHOICE paths (a combat-math "Attack:" CoT line can no
    longer hijack a valid CHOICE).
(d) ENGINE-R6: Transform options annotate "[available NOW - condition already met...]".
(e) R-PAINLAND: self-damage mana sources tagged on battlefield + CAUTION on the mana line
    (Ancient Tomb 2 damage, painlands 1).
(f) Carried from wave-21, still live: answer-first protocol, partial attacker-sparing, reveal
    pick-ONE framing, wither annotations, bounce-on-stack header, 0-power blocker heuristic.

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260723-173843` — 21 games round-robin,
  binary /tmp/wagic-1d8ed8194. (An earlier same-day run `...135741-CONTAMINATED-pkill` is
  QUARANTINED — never read it as evidence.)
- Win table: 44:4/6, 135:4/6, 62:4/6, 140:3/6, 109:3/6, 102:2/6, 27:1/6.
  Continuity deltas (vs wave-21, same-engine baseline): 135 1/6→4/6, 62 4/6→4/6, 27 5/6→1/6,
  102 3/6→2/6. Newcomers have no same-pool baseline (pool changed — treat cross-wave record
  comparison as WEAK evidence this cycle; the pool swap changes matchup difficulty for everyone).
- 0 timeouts/draws; **8 life-adjudicated at cap** — audited legitimate (turns 10-35, 31-67
  decisions/seat, deck140-the-grinder in 5 of 8; latency median 16.9s p90 77.6s at j3 — check
  who-was-ahead per the timeout-economics rule when reading adj outcomes).
- **Fallbacks 8/1368 = 0.6%: 2 retracted_choice (was 5 — the N9 fix's first live corpus;
  judge whether BOTH remaining fires are correct), 6 stale_echo, 0 unparsed, 0 empty.**
  Per-seat: 140:4, 102:3, 135:1, everyone else 0. All 8 records:
  135vs27 s27 ask stale_echo; 102vs62 s21 ask retracted_choice; 140vs62 s27 ask stale_echo;
  140vs27 s26 ask stale_echo; 102vs140 s29 ask retracted_choice; 140vs102 s9 priority
  stale_echo + s12 ask stale_echo; 102vs109 s42 priority stale_echo.
- Decision kinds: ask 827, priority 270, attackers 143, blockers 38, reveal 22 (135:20, 102:2).
  0 prose salvages fired.

## VALIDATION FOCUS (per owner)

- **deck135 OWNS (a) fetch resolution**: heavy fetch activity all corpus — run the detector on
  EVERY chosen crack; any fizzle (crack chosen, no graveyard+fetch events) = seq + verbatim.
  Also: R1 Glacial partition on 20 reveal records (regression detector), Azcanta transform —
  did the new (d) annotation flip the wave-21 declines into accepts? 1/6→4/6 on a frozen guide:
  attribute honestly (fetch fix vs pool change).
- **deck102 OWNS (b) N9 residual**: both remaining retracted_choice fires are at YOUR seat
  (s21 vs62, s29 vs140) — read the replies: correct fires (real disavowals) or residual FPs?
  Also painland CAUTION (e): did it render, and did the Ancient Tomb self-death class stop?
- **deck140 OWNS the stale_echo cluster**: 4 of 6 stale_echos are at your seat (a NEW seat) —
  classify each against the shipped (c) fixes: source-prefix shape? label-hijack shape? or a
  genuinely new shape the fixes don't cover? This decides whether (c) closed the family.
- **deck109 / deck44 / deck140 (rotated-in)**: re-validate your OLD guide under the modern
  engine per the current strategy-writing skill (wave21/strategy-writing-skill.md): stale
  crutches for since-fixed engine defects should be RETIRED (the skill's crutch-lifecycle
  rung); teach what the modern representation actually shows (trade annotations, castability,
  answer-first reply protocol postdate these guides).
- **deck27**: 5/6→1/6 across the pool swap — decompose (new-pool matchups vs any regression;
  the wither fix and late-stall revision were validated last wave; did anything shipped since
  hurt?).
- **deck62**: 0 fallbacks (had 4). Confirm the N9-FP class is gone at the seat that owned it;
  edit D round 2; N8 gang-chump still unexercised?
- **Everyone**: answer-first still holding (sample longest replies); reveal/wither/bounce
  surfaces spot-checks where they occur at your seat.

## Layer-routing (unchanged)

Engine/harness items -> notes.md with seq repros, never prompt text. Guide changes ->
strategy.txt per wave21/strategy-writing-skill.md. Skill proposals -> skill.md. Core-prompt
proposals -> general-suggestions.md. Deck-construction -> notes.md. Win column = context-only.
Rotation test: no-guide-mod AND no-new-work-signal = candidate.
