# wave-52 seat-reviewer brief (common to all seats)

You are one reviewer in wagicGPT's dev loop (step 2). Repo: /home/magi/Projects/wagicGPT,
game dir projects/mtg. READ FIRST, in order:
1. projects/mtg/strategy-design/wave52/review-carry.md  (new-baseline discipline + the
   prediction sets you must adjudicate — PASS / FAIL / UNTESTED; UNTESTED is a real verdict
   when no window arose; never infer).
2. projects/mtg/strategy-design/wave51/strategy-writing-skill.md (the CURRENT skill edition, amendments 1-167:
   how guides for this model tier are written; the perception-vs-strategy routing doctrine;
   the trust doctrine — never teach doubt of the render).
3. Your seats' wave-51 file(s) in projects/mtg/strategy-design/wave51/seats/ (and wave51/general-strategy.md R1-R135, wave51/engine-ledger.md = the wave-52 docket D1-D27 whose lanes G-L shipped) (rung state,
   open predictions), and the live guides projects/mtg/bin/Res/ai/baka/deckN_strategy.txt.

CORPUS: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260901-223430/ — one JSONL per SEAT
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
- projects/mtg/strategy-design/wave52/seats/seat-<ids>.md — same shape as the wave-51 seat
  files: per-deck findings with counts (N/M windows), prediction adjudication (set A guide
  edits for your decks, set B engine lanes where your seats had windows), rotation evidence,
  engine/render items ranked HIGH/MED/LOW with a concrete repro (file, seq), a lategame
  specimen candidate (file + seq), and falsifiable predictions for wave 53.
- projects/mtg/strategy-design/wave52/deck<N>/strategy.txt — the FULL revised guide per deck
  (start from the live guide; every edit justified by a finding; numbers read off the engine
  render/primitives). Plus deck<N>/edit-texts.md: each edit as before->after with its finding.
- projects/mtg/strategy-design/wave52/deck<N>/skill.md — proposals for the strategy-writing
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
lanes G/H/I/J/K/L plus the owner batches, the option-preserving autotap and the Baka pump-targeting fix (wave52/review-carry.md lists
each falsifiable prediction). Standing owner rulings: X menu largest-first; history lines
owner-prefixed.
CORPUS NOTE: 21/21 natural (0 timeouts, 0 crashes, 0 cap-adjudicated). Harvest: 42 seat logs,
3,384 decisions (wave 51: 2,171), 15 fallbacks (wave 51: 5 — classes: plan_choice_conflict 5,
empty_reply 3, unparsed_reply 3, multiblock_reask 3, named_row_reask 1; the engine seat owns
the jump), max seat 353 records, max prompt 31,551 chars (146 vs 152 seq 107, turn 27),
464 `- Paid` receipts / 0 no-source (3 read `from mana already floating`, a legitimate form).
Longest games: 125 vs 130 (69 turns), 152 vs 125 (53), 146 vs 125 (51). Baseline files:
wave52/corpus-results.tsv, wave52/corpus-log.txt, wave52/lategame-specimen.txt.
GUIDE-LITERAL DRIFT (do first, see review-carry.md): deck123/deck126 still cite retired
`[already owned:` / `[from exile]` literals — re-key to the lane-K forms and count meetings.
