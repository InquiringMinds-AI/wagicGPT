# wave-55 seat-reviewer brief (common to all seats)

You are one reviewer in wagicGPT's dev loop (step 2). Repo: /home/magi/Projects/wagicGPT,
game dir projects/mtg. READ FIRST, in order:
1. projects/mtg/strategy-design/wave55/review-carry.md  (new-baseline discipline + the
   prediction sets you must adjudicate — PASS / FAIL / UNTESTED; UNTESTED is a real verdict
   when no window arose; never infer).
2. projects/mtg/strategy-design/wave54/strategy-writing-skill.md (the CURRENT skill edition, amendments 1-232:
   how guides for this model tier are written; the perception-vs-strategy routing doctrine;
   the trust doctrine — never teach doubt of the render).
3. Your seats' wave-54 file(s) in projects/mtg/strategy-design/wave54/seats/ (and wave54/general-strategy.md R1-R224, wave54/engine-ledger.md = the wave-55 docket D1-D35 whose lanes M-Q shipped D1-D14, plus wave53/lane-*.md for what each lane actually changed) (rung state,
   open predictions), and the live guides projects/mtg/bin/Res/ai/baka/deckN_strategy.txt.

CORPUS: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-074354/ — one JSONL per SEAT
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
- projects/mtg/strategy-design/wave55/seats/seat-<ids>.md — same shape as the wave-54 seat
  files: per-deck findings with counts (N/M windows), prediction adjudication (set A guide
  edits for your decks, set B engine lanes where your seats had windows), rotation evidence,
  engine/render items ranked HIGH/MED/LOW with a concrete repro (file, seq), a lategame
  specimen candidate (file + seq), and falsifiable predictions for wave 56.
- projects/mtg/strategy-design/wave55/deck<N>/strategy.txt — the FULL revised guide per deck
  (start from the live guide; every edit justified by a finding; numbers read off the engine
  render/primitives). Plus deck<N>/edit-texts.md: each edit as before->after with its finding.
- projects/mtg/strategy-design/wave55/deck<N>/skill.md — proposals for the strategy-writing
  skill (only what your evidence supports), and general-suggestions.md — core-prompt/render
  proposals, layer-routed (engine bugs go to the seat file's engine list, never prompt text).
DO NOT: edit files under bin/Res, src/, or run git; run wagic; deploy anything. Do not write
mechanisms as the owner's stated positions. Report what you did NOT check.

WAVE-47 SPECIFICS (owner doctrine 2026-08-26): YOU own your decks' guides — every change is
your decision from corpus evidence; no deck-content question goes to the owner. The core
loop's purpose is engine correctness + LLM UX (legality enforced structurally without
constraining choice; information/choice presented to maximize decision quality; efficient
inference) — your engine/render items feed it; guide edits are the cheap side quest, so be
surgical (every added line paid for by a cited seq; pool band 41-71 KB, measure yours).
Adjudicate render predictions by the emitter's actual string, and COUNT FROM THE RENDERED
`prompt`, never from the `options` array (three wave-54 docket numbers were array counts that
did not render). This corpus is the FIRST carrying wave-55 step-1 (lanes A-E: option-set keys +
own-turn HOLD row, blockers re-ask + menace header, X markers + both-player edict verdicts +
NAMED-BY price, reveal/discard collapse + chooseaname + discard verdicts, reveal_stall +
wall_miss records) AND the first UNCONTAMINATED window economy since wave 53 (lane R's
stall-floor fix) — wave55/review-carry.md lists every falsifiable prediction and flag, and lane
A's wave-54 HOLD predictions are re-adjudicated HERE. Standing owner rulings: X menu
largest-first; history lines owner-prefixed; hard caps on legal choices rejected on sight.
CORPUS NOTE: **21/21 natural** (0 timeouts, 0 crashes, 0 hung); wall time 2 h 03 min.
**D1 DISCHARGED on the first read: stall-floor releases 0 (was 468), `dropping stale async
answer` 43 (was 488; wave 53: 63), `giving this decision to the heuristic` 0.** Opponent-turn
windows are BACK: 3,171 decisions (wave 54: 2,212; wave 53: 3,483; priority 555 / ask 2,416 /
attackers 90 / discard 68 / blockers 21 / reveal 12 / bottom 9), inference 22.98 h = 26.1 s per
decision (wave 54: 23.9), max prompt 30,031 (`152v125` seq 314), prompts > 30 K: 1.
Harvest: 14 fallbacks (0.44%: plan_choice_conflict 9, named_row_reask 3, stale_echo 1,
unparsed_reply 1; NO all_assignments_illegal, NO stale_livelock, NO empty/timeout), 7
`recovery`, 2 `chose -1 of N`, 2 NEW `kind: wall_miss` records + `wall_miss_events` 3 on
gameends (lane E's D23 — adjudicate), `reveal_stall` present on all 12 reveal records (check
whether it is a stamp or a zero-valued field before calling it a park), 1 `latched_row_mismatch`,
1 `long_reply`, `hold_row_named` 5, **`pass_hold_ambiguous` 0** (was 16), `echo_index_conflict`
10 / `name_over_index` 5. **HOLD ROW (lane A, own turn now included): 2,352 renders / 282 takes
(12.0%)**, `hold_windows_skipped` non-zero on **21 of 42** gameends (was 7/42), sum 735 (was
72); in declined-note N>=3 windows **67 of 753 = 8.9%** (target >= 20%: FAIL on first read —
reviewers re-derive); `identical_option_asks_resolved` 9. Identical-declined runs (harvest
predicate, (turn, options_text) + declined): **21.5% of decisions, MAX RUN 16** (was 16.1% /
50; the max-run target < 20 is MET, the < 12% share is NOT — but the denominator now includes
own-turn windows the row is offered on; measure both ways per #208 and say which). Max seat 318
(`152v125`, 64 turns). Baseline files: wave55/corpus-results.tsv, corpus-log.txt,
lategame-specimen.txt (`152v125` seq 309, turn 65, Blockers, 29,337 chars, 17 rows).
THREE-NUMBER LITERAL AUDIT (#188) is mandatory for every literal lanes A-E changed — the list
is in wave55/review-carry.md §B; report old-string renders (expect 0), new-string renders, takes.
