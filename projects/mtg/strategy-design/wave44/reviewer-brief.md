# Wave-44 seat-reviewer brief (common to all seats)

You are one reviewer in wagicGPT's dev loop (step 2). Repo: /home/magi/Projects/wagicGPT,
game dir projects/mtg. READ FIRST, in order:
1. projects/mtg/strategy-design/wave44/review-carry.md  (new-baseline discipline + the
   prediction sets you must adjudicate — PASS / FAIL / UNTESTED; UNTESTED is a real verdict
   when no window arose; never infer).
2. projects/mtg/strategy-design/wave43/strategy-writing-skill.md (the CURRENT skill edition:
   how guides for this model tier are written; the perception-vs-strategy routing doctrine;
   the trust doctrine — never teach doubt of the render).
3. Your seats' wave-43 file(s) in projects/mtg/strategy-design/wave43/seats/ (rung state,
   open predictions), and the live guides projects/mtg/bin/Res/ai/baka/deckN_strategy.txt.

CORPUS: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-111937/ — one JSONL per SEAT
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
- projects/mtg/strategy-design/wave44/seats/seat-<ids>.md — same shape as the wave-43 seat
  files: per-deck findings with counts (N/M windows), prediction adjudication (set A guide
  edits for your decks, set B engine lanes where your seats had windows), rotation evidence,
  engine/render items ranked HIGH/MED/LOW with a concrete repro (file, seq), a lategame
  specimen candidate (file + seq), and falsifiable predictions for wave 45.
- projects/mtg/strategy-design/wave44/deck<N>/strategy.txt — the FULL revised guide per deck
  (start from the live guide; every edit justified by a finding; numbers read off the engine
  render/primitives). Plus deck<N>/edit-texts.md: each edit as before->after with its finding.
- projects/mtg/strategy-design/wave44/deck<N>/skill.md — proposals for the strategy-writing
  skill (only what your evidence supports), and general-suggestions.md — core-prompt/render
  proposals, layer-routed (engine bugs go to the seat file's engine list, never prompt text).
DO NOT: edit files under bin/Res, src/, or run git; run wagic; deploy anything. Do not write
mechanisms as the owner's stated positions. Report what you did NOT check.
