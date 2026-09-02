# wave-53 seat-reviewer brief (common to all seats)

You are one reviewer in wagicGPT's dev loop (step 2). Repo: /home/magi/Projects/wagicGPT,
game dir projects/mtg. READ FIRST, in order:
1. projects/mtg/strategy-design/wave53/review-carry.md  (new-baseline discipline + the
   prediction sets you must adjudicate — PASS / FAIL / UNTESTED; UNTESTED is a real verdict
   when no window arose; never infer).
2. projects/mtg/strategy-design/wave52/strategy-writing-skill.md (the CURRENT skill edition, amendments 1-186:
   how guides for this model tier are written; the perception-vs-strategy routing doctrine;
   the trust doctrine — never teach doubt of the render).
3. Your seats' wave-52 file(s) in projects/mtg/strategy-design/wave52/seats/ (and wave52/general-strategy.md R1-R160, wave52/engine-ledger.md = the wave-53 docket D1-D28 whose lanes M-Q shipped D1-D14, plus wave53/lane-*.md for what each lane actually changed) (rung state,
   open predictions), and the live guides projects/mtg/bin/Res/ai/baka/deckN_strategy.txt.

CORPUS: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260902-023342/ — one JSONL per SEAT
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
- projects/mtg/strategy-design/wave53/seats/seat-<ids>.md — same shape as the wave-52 seat
  files: per-deck findings with counts (N/M windows), prediction adjudication (set A guide
  edits for your decks, set B engine lanes where your seats had windows), rotation evidence,
  engine/render items ranked HIGH/MED/LOW with a concrete repro (file, seq), a lategame
  specimen candidate (file + seq), and falsifiable predictions for wave 54.
- projects/mtg/strategy-design/wave53/deck<N>/strategy.txt — the FULL revised guide per deck
  (start from the live guide; every edit justified by a finding; numbers read off the engine
  render/primitives). Plus deck<N>/edit-texts.md: each edit as before->after with its finding.
- projects/mtg/strategy-design/wave53/deck<N>/skill.md — proposals for the strategy-writing
  skill (only what your evidence supports), and general-suggestions.md — core-prompt/render
  proposals, layer-routed (engine bugs go to the seat file's engine list, never prompt text).
DO NOT: edit files under bin/Res, src/, or run git; run wagic; deploy anything. Do not write
mechanisms as the owner's stated positions. Report what you did NOT check.

WAVE-47 SPECIFICS (owner doctrine 2026-08-26): YOU own your decks' guides — every change is
your decision from corpus evidence; no deck-content question goes to the owner. The core
loop's purpose is engine correctness + LLM UX (legality enforced structurally without
constraining choice; information/choice presented to maximize decision quality; efficient
inference) — your engine/render items feed it; guide edits are the cheap side quest, so be
surgical (every added line paid for by a cited seq; guides are already 27-38 KB). Adjudicate
render predictions by the emitter's actual string. This corpus is the FIRST carrying the
lanes M/N/O/P/Q (docket D1-D14: HOLD row, kill summaries, lethal tails, stack naming, timeout retry) plus the Vita lanes R/S/T (wave53/review-carry.md lists
each falsifiable prediction). Standing owner rulings: X menu largest-first; history lines
owner-prefixed.
CORPUS NOTE: 20/21 natural. Game deck152 vs deck125 (seat logs ...deck152-0x5583d9910230... /
...deck125-0x5583dac4bbe0..., stderr game-152v125-1788334434.stderr) HUNG: the stderr's last
engine line is `Action added to stack: StackAbility. (Source: Ranger Class)` right after the
deck152 seat answered `TARGET CHOICE for Ranger Class - its "put a +1/+1 counter" ability` with
Sigarda (attacking); the deck152 seat's last record is seq 28, kind `reveal`, Blockers, options 5,
`chosen_text` "none (no legal target)", latency -1, prompt 0; then NOTHING for ~13 h until the
harness adjudicated it (`adj0`, 20/9, turn 15). It has NO gameend record on either seat. Exclude
it from rates; the ENGINE seat owns it as HIGH #1 (an engine hang, not a model loop: no ticks, no
stderr) — pin the sequence (Ranger Class level-2 attack trigger targeting, then a reveal-kind
seam returning -1 at Blockers) from the two seat logs and the primitives.
Harvest: 42 seat logs, 3,572 decisions (wave 52: 3,384), 15 fallbacks (wave 52: 15 —
plan_choice_conflict 8, unparsed_reply 3, named_row_reask 3, retracted_choice 1; NO empty_reply
and NO timeout this corpus), max seat 607 records (146 vs 125, an 84-turn game), max prompt
32,068 (146 vs 123 seq 92, turn 33), 409 `- Paid` receipts / 0 no-source, HOLD row taken 29
times (count `hold_windows_skipped` yourself), 7 records of the NEW kind `recovery` (lane Q —
they follow a `choice: -1` fallback record; do not count them as decisions). Baseline files:
wave53/corpus-results.tsv, corpus-log.txt, lategame-specimen.txt (125 vs 146 seq 357, turn 84).
THREE-NUMBER LITERAL AUDIT (#168) is mandatory for every literal lanes O/P changed — the list is
in wave53/review-carry.md; report old-string renders (expect 0), new-string renders, takes.
