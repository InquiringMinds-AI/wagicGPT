# wave-54 seat-reviewer brief (common to all seats)

You are one reviewer in wagicGPT's dev loop (step 2). Repo: /home/magi/Projects/wagicGPT,
game dir projects/mtg. READ FIRST, in order:
1. projects/mtg/strategy-design/wave54/review-carry.md  (new-baseline discipline + the
   prediction sets you must adjudicate — PASS / FAIL / UNTESTED; UNTESTED is a real verdict
   when no window arose; never infer).
2. projects/mtg/strategy-design/wave53/strategy-writing-skill.md (the CURRENT skill edition, amendments 1-208:
   how guides for this model tier are written; the perception-vs-strategy routing doctrine;
   the trust doctrine — never teach doubt of the render).
3. Your seats' wave-53 file(s) in projects/mtg/strategy-design/wave53/seats/ (and wave53/general-strategy.md R1-R192, wave53/engine-ledger.md = the wave-54 docket D1-D38 whose lanes M-Q shipped D1-D14, plus wave53/lane-*.md for what each lane actually changed) (rung state,
   open predictions), and the live guides projects/mtg/bin/Res/ai/baka/deckN_strategy.txt.

CORPUS: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-034355/ — one JSONL per SEAT
(filename ...deckA-<ptr>-vs-...deckB = the deckA seat). Records: {seq, kind, prompt, reply,
choice, chosen_text, options, turn, phase, my_life, opp_life, latency_ms, fallback?}. The
`prompt` is the full per-decision message (narration + CURRENT SITUATION + plan + legal
choices) — what the model SAW; `choice`/`chosen_text` what it did. A closing `gameend`
record per seat carries the outcome. `bottom` = mulligan bottoming (first corpus ever with
real hands). results.tsv has winners. Read translogs with python, not by eye.

METHOD: for each of your decks, per game, trace the decisions that decided the game (both
seats' narration is complete). Classify each misplay: PERCEPTION (model misread a true
surface, or the surface was false/missing -> ENGINE/RENDER item) vs STRATEGY (read
correctly, decided wrong -> guide teach). Verify EVERY card fact you rely on against the
primitive (`/usr/bin/grep -A6 "^\[card\]" ... ` or `/usr/bin/grep -n "name=CardName" -A8
projects/mtg/bin/Res/sets/primitives/*.txt`; text=/auto= lines are what the engine and model
use) — agent card-fact claims have been wrong 4 times; if Scryfall (WebFetch
https://api.scryfall.com/cards/named?exact=NAME) and the primitive disagree, report both.
Use /usr/bin/grep (bare grep is a wrapper that can drop matches).

NEW-BASELINE RULES (binding): hands are real now — do NOT attribute win-rate/curve variance
vs wave 43 to guide edits. Record per seat: mulligan offered/taken, kept land counts, any
mull-to-zero or repeated mull WITH a guide floor -> flag as OWNER QUESTION, do not patch.

OUTPUT (write files; final message = a 10-line summary only):
- projects/mtg/strategy-design/wave54/seats/seat-<ids>.md — same shape as the wave-53 seat
  files: per-deck findings with counts (N/M windows), prediction adjudication (set A guide
  edits for your decks, set B engine lanes where your seats had windows), rotation evidence,
  engine/render items ranked HIGH/MED/LOW with a concrete repro (file, seq), a lategame
  specimen candidate (file + seq), and falsifiable predictions for wave 55.
- projects/mtg/strategy-design/wave54/deck<N>/strategy.txt — the FULL revised guide per deck
  (start from the live guide; every edit justified by a finding; numbers read off the engine
  render/primitives). Plus deck<N>/edit-texts.md: each edit as before->after with its finding.
- projects/mtg/strategy-design/wave54/deck<N>/skill.md — proposals for the strategy-writing
  skill (only what your evidence supports), and general-suggestions.md — core-prompt/render
  proposals, layer-routed (engine bugs go to the seat file's engine list, never prompt text).
DO NOT: edit files under bin/Res, src/, or run git; run wagic; deploy anything. Do not write
mechanisms as the owner's stated positions. Report what you did NOT check.

WAVE-47 SPECIFICS (owner doctrine 2026-08-26): YOU own your decks' guides — every change is
your decision from corpus evidence; no deck-content question goes to the owner. The core
loop's purpose is engine correctness + LLM UX (legality enforced structurally without
constraining choice; information/choice presented to maximize decision quality; efficient
inference) — your engine/render items feed it; guide edits are the cheap side quest, so be
surgical (every added line paid for by a cited seq; pool guides are 39.6-70.2 KB — measure
yours). Adjudicate render predictions by the emitter's actual string. This corpus is the
FIRST carrying wave-54 step-1 (engine lanes A-F: HOLD row reword + keys, parser stamps,
cast-row pricing with player targets, gain-framed searches, forced-sac ordering, render truths,
reveal stall guard) AND eight audit perf/memory lanes (G-N: ability ownership, per-tick ability
epoch gate, ManaCost/garbage/instance memory, render caches, Baka selectAbility rewrite, GPT
transport/narration/cache) — wave54/review-carry.md lists every falsifiable prediction and
every DISABLE FLAG. Any behaviour drift with no engine-lane explanation is a candidate audit-lane
regression: name the flag that would isolate it. Standing owner rulings: X menu largest-first;
history lines owner-prefixed; hard caps on legal choices rejected on sight.
CORPUS NOTE: **21/21 natural — D1 DISCHARGED** (0 timeouts, 0 crashes, 0 hung; the new
no-progress supervisor never fired). Wall time 1 h 45 min for the whole corpus (wave 53: ~21 h)
— games are SHORTER (max turn 63, `125v123`; wave 53 max 84) and decisions FEWER, and the
audit lanes made every tick cheaper; do NOT read the speed as a model change.
Harvest: 42 seat logs, **2,212 decisions** (wave 53: 3,483; ask 1,683 / priority 332 /
attackers 99 / blockers 36 / discard 34 / reveal 20 / bottom 8), **23 fallbacks** (wave 53: 15 —
plan_choice_conflict 12, named_row_reask 7, named_row_not_offered 2, stale_livelock 1,
all_assignments_illegal 1; NO empty_reply, NO timeout, `choice: -1` with no fallback 0), 10
`recovery` records (not decisions), `chose -1 of N` (Baka executed) 2, `giving this decision to
the heuristic` **1** (wave 53: 0 — a 6-consecutive-stale-drop handoff), max seat 312
(`123v125`), max prompt 32,215 (`146v126` seq 86), `- Paid` renders 15,170 / 0 no-source,
inference 14.67 h. Parse notes: decision_reversed_in_prose 18, **pass_hold_ambiguous 16**
(lane A's new stamp — all 16 are a plain `CHOICE: 0 (pass)` executing row 0 correctly: the
stamp is a FALSE POSITIVE of the reword, `(pass)` now heads both rows — engine seat: docket it),
latched_row_mismatch 2 (lane B's D13 stamp — first firings, adjudicate both), long_reply 1
(`123v125` seq 1, the opening mulligan at 1,488 s under 21-game launch contention).
**HOLD ROW (lane A):** 426 renders (wave 53: 1,173), **43 takes (10.1%; wave 53: 29 = 2.5%)**,
`hold_windows_skipped` non-zero on **7 of 42** gameends (was 2/40), sum 72; `hold re-opened` 21;
identical-declined runs **12.2%** of decisions (was 26.2%; target < 15% MET);
`identical_option_asks_resolved` 5 on 2 gameends (lane D's D8(b)). Adjudicate every lane-A
prediction from the rows (opponent-turn windows/casts per seat: count them yourself).
**ENGINE-SEAT HIGH CANDIDATE — `dropping stale async answer` 488 (wave 53: 63, +675%)**, up to
64 in one game (`126v123`), one `6 consecutive stale drops` handoff. Every drop follows an
`Action added to stack: NextGamePhase` line in stderr — at EVERY phase (Main 69, Draw 37,
Attackers 36, Combat begins 35, Combat ends 30, Cleanup 29, End 28) — i.e. the phase advanced
UNDER an in-flight ask and the answer was thrown away and re-asked; in wave 53 the preceding
line was a resolving stack action, never a phase step. Each drop is a wasted model call. This is
NEW this corpus and has no engine-lane prediction that explains it: pin the mechanism from the
seat logs + stderr (which seam, whose turn, was the window the seat's own or the opponent's) and
name the wave-54 lane/flag most likely responsible (candidates: lane A hold keys, lane AA's
AI-window stall floor, lane K's Baka `Act` window change, lane M's ask-cache/prefill, lane L's
board index; the disable flags in review-carry §C are the A/B — reviewers do NOT run wagic;
say which flag a lane should flip first).
Baseline files: wave54/corpus-results.tsv, corpus-log.txt, lategame-specimen.txt (`125v123`
seq 188, turn 63, Cleanup, 25,767 chars, 9 rows).
THREE-NUMBER LITERAL AUDIT (#188) is mandatory for every literal lanes A-E changed — the list
is in wave54/review-carry.md §B; report old-string renders (expect 0), new-string renders, takes.
