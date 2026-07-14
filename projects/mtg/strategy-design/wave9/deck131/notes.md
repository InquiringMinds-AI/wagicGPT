# deck131 wave-9 — working notes (catch-all, self-contained for the pause)

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260714-094942/`, 6 deck131 games, 0W/5L/1TO.
Live guide during this corpus = the WAVE-8 revision. Seat files (from `gamestart` record):
vs109-L 1784041212 | vs140-TO 1784042388 | vs110-L 1784044085 | vs44-L 1784046898 |
vs133-L 1784047264 | vs135-L 1784047950.

## Reproducible evidence (each item = full repro)

### R1 — vs44 mulligan REPLY-PROTOCOL INTENT-COLLAPSE (the headline)
File `1784046898-ai_baka_deck131-0x559d9770f980.jsonl`, seq 1, turn 1.
- opening hand (from prompt): `Island; Island; Downsize {u}; Island; Essence Scatter {1}{u};
  Cyclonic Rift {1}{u}; Young Pyromancer {1}{r}` — 0 Mountain, no Artificer's Epiphany/Opportunity
  → correct = MULLIGAN.
- `options_text` = `['Keep this hand','Mulligan']`; `choice` = 1; `chosen_text` = "Keep this hand".
- `reply` = `"1\nPLAN: Keep this hand. It contains a Mountain? No, it has three Islands. Wait, the
  strategy guide says: 'If the word Mountain does NOT appear ... AND the hand has no Artificer's
  Epiphany and no Opportunity, MULLIGAN'. My hand has no Mountain, no Artificer's Epiphany, and no
  Opportunity. Therefore, I must Mulligan. The hand has Young Pyromancer and counters, but without a
  red source or a way to dig for one ... I cannot cast my win condition. I will shuffle this hand back
  and draw a new one."`
- DIAGNOSIS: the head `1` (=Keep) was emitted before the PLAN; the PLAN correctly reasons to
  Mulligan and explicitly rejects the Young Pyromancer bait; the head was never revised. The wave-8
  strict-gate guide fix WORKED (reasoning correct — this is what wave-8's false-KEEP-by-hijack looked
  like at the reasoning level, now dead). Residual = reply-protocol head-first commit. Repro to
  confirm: `python3 -c "import json; r=[json.loads(l) for l in open(F) if l.strip()]; x=[y for y in r
  if y.get('seq')==1][0]; print(x['choice'], x['chosen_text']); print(x['reply'])"` with F = the file.

### R2 — Mountain-first 9/9 (land fix steady)
Script: for each deck131 jsonl, count decisions whose `options_text` list contains BOTH a
"Play Mountain" and a "Play Island" entry → 10 co-offers (vs109 1, vs140 2, vs110 1, vs133 2,
vs135 4, vs44 0). Of those, chose a "Mountain" option in 9; the 1 non-Mountain = vs135 seq5 T3
(`['Play Island','Play Mountain','Play no land right now']` → "Play no land right now"), a CORRECT
decline of a 2nd land drop that turn (battlefield already had the turn's Mountain; reply: "I have
already played a Mountain this turn, so I cannot play another land"). vs44 = 0 co-offers (drew no
Mountain until mid-game; the kept 0-red hand from R1).

### R3 — Guttersnipe cast-every-turn / NEVER-block
Guttersnipe genuinely on `Your battlefield (creatures: N):` line: vs44 (3 decisions), vs135 (3),
others 0 — 2 games testable (wave-8 was 1). Fuel-declined leaks (Guttersnipe out + a castable
instant/sorcery that is NOT Prism Ring/Elixir + chose nothing) = 0. Guttersnipe ATTACKED (vs135 seq21
T9, chose "Elemental, Guttersnipe") and NEVER blocked. All blocker decisions at 17-20 life = "no
blockers" (vs109 seq7 T3, vs135 seq10 T4 + seq19 T8). Lone chump: vs133 seq56 T23 "Young Pyromancer
blocks Geralf's Messenger" at 8/5 — a lost late game (ended -1/5); marginal, low-confidence, not
scored (opts:None; not a guide defect).

### R4 — counter seam played well (no sweeper-vs-Guttersnipe window)
vs140 file `1784042388-ai_baka_deck131-0x557961ae69d0.jsonl`: Guttersnipe cast T32 stayed (game timed
out AHEAD 29/25); NO Pyroclasm resolved into it with a counter up → the wave-8 board-wipe counter
clause had no test window. Counter picks all sound: seq28 T22 Dissipate on Staff of Nin (kept
Counterspell), seq33 T32 Counterspell on Venser's Journal, seq41 T44 Essence Scatter on Blightsteel
Colossus. Also vs110 seq30 Counterspell on Cranial Plating; vs44 seq11 Essence Scatter on Scion of
Oona, seq24 Counterspell on Archmage of Echoes.

### R5 — damage>=toughness watch: 0 recurrence
Grep all 6 deck131 replies for "toughness" co-occurring with survive/kill/destroy/dies/"2 damage" →
0 hits. Precondition (sweeper on stack vs live Guttersnipe with counter up) did not occur. Stays
single-seat (wave-8 vs140 only). Do NOT promote.

## Mulligan tally (both directions — the brief's explicit ask)
- (no-Mountain openers, REASONING correct)/(no-Mountain openers) = 1/1 (vs44 reasoned to Mulligan).
  Wave-8 was 1/2 (a reasoning-level false-KEEP). IMPROVED — the strict gate fixed the reasoning.
- (no-Mountain openers, OUTCOME correct)/(no-Mountain openers) = 0/1 (vs44 head-committed Keep). The
  gap between reasoning and outcome = the reply-protocol intent-collapse (R1). NOT a guide defect.
- (keepable Mountain-hands kept)/(keepable) = 5/5 (vs109/140/110/133/135). No false-MULLIGAN recurred.

## Freeze-check (obeyed-but-losing discipline — every kept line accounted for; guide FREEZES)
- Two-gear identity, Guttersnipe cast-every-turn #1, deploy-payoffs/cast-creature-first, YP-swarm
  second win path, Mountain-first pick rule, attack-every-creature/no-block-while-racing,
  NEVER-block-Guttersnipe, reactive-spells + board-wipe counter clause, draw-spells,
  Prism/Elixir-lifegain, lethal-check, strict two-step mulligan gate, DECIDING SITUATIONS — ALL
  obeyed/held/untested-precondition-rare → KEPT VERBATIM.
- ONE freeze-safe edit: the winning-line self-citation "that is exactly how the last win closed"
  (corpus-pinned; no win this corpus, the cited token-swarm win is from wave-8) → de-specified to
  "that is how this deck's fastest wins close." Removes a corpus-stale self-citation per the skill's
  staleness rule; the described win PATTERN is real and unchanged; changes no pick. No other edit.

## Harness sweep
- 0 desyncs, 0 engine no-ops, 0 defers (`deferred_to_heuristic`), 0 `fails validation`, across all 6
  games + 6 stderr sets. Transport clean (waves 5-6-7-8-9).
- vs140 timeout still writes no per-seat `gameend` in the jsonl (results.tsv adjudicates). Minor.

## For a future deck131 reviewer (if the project resumes) — gotchas
- n=6, single run: win-rate is noise. The guide is at its terminus (5 waves obeyed-but-losing);
  further wins come from CONSTRUCTION (threat density 6/60, reach vs lifegain) and the HARNESS
  (reply-protocol A/B), not guide prose. Resist churning frozen lines off a bad win column.
- SPLIT mulligan scoring reasoning-correct vs outcome-correct — a guide fix that corrects the
  reasoning is validated even if the OUTCOME stays wrong (the wave-9 vs44 lesson); the outcome gap is
  the reply-protocol layer's, not the guide's.
- The reply-protocol intent-collapse is now 2 seats (deck135 s48, deck131 vs44 seq1) — if the A/B has
  not run, it is the highest-leverage open item touching this deck.
- Isolate `Your battlefield (creatures: N):` for on-board checks (a loose "Guttersnipe" substring
  match catches hand/cast-list/guide text — false positives); the opening-hand line for mulligan; the
  `A#.`/attackers records for combat.
- vs109 mono-red aggro, vs110 affinity, vs133 zombies, vs135 snow tempo, vs140 lifegain-grind are
  hard-to-unwinnable for a 6/60-threat 2-damage engine; don't read those as guide/pilot failures.
