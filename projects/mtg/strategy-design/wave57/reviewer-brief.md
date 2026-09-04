# wave-57 seat-reviewer brief (common to all seats)

You are one reviewer in wagicGPT's dev loop (step 2). Repo: /home/magi/Projects/wagicGPT,
game dir projects/mtg. READ FIRST, in order:
1. projects/mtg/strategy-design/wave57/review-carry.md  (new-baseline discipline + the
   prediction sets you must adjudicate — PASS / FAIL / UNTESTED; UNTESTED is a real verdict
   when no window arose; never infer).
2. projects/mtg/strategy-design/wave56/strategy-writing-skill.md (the CURRENT skill edition, amendments 1-300:
   how guides for this model tier are written; the perception-vs-strategy routing doctrine;
   the trust doctrine — never teach doubt of the render).
3. Your seats' wave-56 file(s) in projects/mtg/strategy-design/wave56/seats/ (and wave56/general-strategy.md, wave56/engine-ledger.md = the wave-57 docket D1-D44 whose lanes A-H + S/T/U/V shipped, plus wave57/lane-*.md for what each lane actually changed and wave57/owner-decisions.md; wave56/reviewer-boundary-pass.md for the B1-B10 guide edits now live) (rung state,
   open predictions), and the live guides projects/mtg/bin/Res/ai/baka/deckN_strategy.txt.

CORPUS: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-174505/ — one JSONL per SEAT
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
- projects/mtg/strategy-design/wave57/seats/seat-<ids>.md — same shape as the wave-56 seat
  files: per-deck findings with counts (N/M windows), prediction adjudication (set A guide
  edits for your decks, set B engine lanes where your seats had windows), rotation evidence,
  engine/render items ranked HIGH/MED/LOW with a concrete repro (file, seq), a lategame
  specimen candidate (file + seq), and falsifiable predictions for wave 56.
- projects/mtg/strategy-design/wave57/deck<N>/strategy.txt — the FULL revised guide per deck
  (start from the live guide; every edit justified by a finding; numbers read off the engine
  render/primitives). Plus deck<N>/edit-texts.md: each edit as before->after with its finding.
- projects/mtg/strategy-design/wave57/deck<N>/skill.md — proposals for the strategy-writing
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
stall-floor fix) — wave57/review-carry.md lists every falsifiable prediction and flag, and lane
A's wave-54 HOLD predictions are re-adjudicated HERE. Standing owner rulings: X menu
largest-first; history lines owner-prefixed; hard caps on legal choices rejected on sight.
CORPUS NOTE (wave 57, FIRST corpus carrying wave-57 step-1 lanes A-H + S/T/U; see
wave57/review-carry.md for every prediction and §D for the separate log-window A/B): **21/21
natural** (0 timeouts, 0 crashes, 0 hung, 0 SEGV); wall ~2 h 10 min. **3,317 decisions** (wave 56:
2,762; ask 2,642 / priority 481 / attackers 76 / discard 66 / blockers 35 / reveal 13 / bottom 4),
inference 25.38 h = 27.5 s per decision (wave 56: 28.6), max prompt 30,228 (`125v130` seq 175),
prompts > 30 K: 4. Harvest (orchestrator first read — RE-DERIVE; uncalibrated predicates are
named): 8 fallbacks (0.24%: named_row_reask 6, retracted_choice 1, unparsed_reply 1), 7
`recovery`, `identical_option_asks_resolved` 5, **`abandoned_in_flight` 0 (lane U: PASS)**,
`giving this decision to the heuristic` 0, `log_window` = full on 3,317/3,317 with 0 elided (lane
H control: PASS). **HOLD 2,490 renders / 450 takes (18.1%)**, same-turn byte-identical re-asks
after a take **14 of 450 (D1: PASS)**; identical-declined runs 17.1% / max 12. **D4 PASS: 0 of
2,960 single-row takes have `chosen_text != options_text[choice-1]`, 0 `choice: -1` records lack
`chosen_text`** — exact-match censuses are trustworthy again. **D5 FAIL on first read: `dropping
stale async answer` 108 (was 65; target < 20), 45 lines mention the land arm, slot rescues 11**
— the new drop line carries the arm and `asyncSlotDriftKind`: classify all 108 (which arm, which
key half moved) before calling the second slot ineffective; engine seat owns this. D6 (B): by a
LOOSE phase predicate 437 opponent-combat-phase windows, 112 with an `INCOMING THIS COMBAT` line
(108 declared / 4 forecast) — the docket predicted 0 without a line: adjudicate on the emitter's
actual window set (declared / settled / forecast / closed forms, lane-B.md); `best case with every
blocker assigned` 34 lines; `BLOCKING THIS COMBAT` header 6. D8 (C): my predicate read 569/569
discard rows as bare (`{` absent) — CALIBRATE against lane C's `discardBoardVerdictTag` literal
and the legend before adjudicating. D7 (C): 80 of 82 creature cast rows in prompts mentioning
Sanguine Bond lack `{their converter:` — my scope was the whole prompt, not "converter on THEIR
battlefield": re-derive. D12 dead-menu header: 0 by my grep — verify the literal. D14 (D):
`Flip Side` rows **0** (PASS; was 109); stderr `only display-toggle` lines 1,309 (up from 760 —
say whether these are now candidate-level drops with no prompt cost). D9 (E): 55 X rows, **0
without a fit clause (PASS; was 40/40)**. D15 (E): life trend on **3,267/3,267 board frames
(PASS)**. D2 (E): the `counted TWICE` note on 5 of 88 pregame prompts (only MDFC hands carry it —
check every MDFC-holding pregame has it). D13 measurement stands (60.8% wave 56). Lane V (Baka
blocker policy) is NOT in this corpus's binary (merged after launch) — its predictions belong to
wave 58. Win rates: 152 4/6, 123 **3/6 (was 1/6 for three waves)**, 126 3/6, 162 3/6, 125 3/6,
130 3/6, 146 2/6 — the flattest band on record; no rate is attributable to any edit. Baseline
files: wave57/corpus-results.tsv, corpus-log.txt, lategame-specimen.txt (`125v?` seq 176, turn
45, 15,319 chars, 78% narration). THREE-NUMBER LITERAL AUDIT (#188) for every literal lanes A-E
changed (wave57/review-carry.md §B). Owner context (verbatim, wave57/owner-decisions.md +
psp-work/NOTES.md): "on the vita, ive been playing only against the heuristic ai" — his play
reports are Baka-seat games; do not route Vita findings to guides.
