# wave-56 seat-reviewer brief (common to all seats)

You are one reviewer in wagicGPT's dev loop (step 2). Repo: /home/magi/Projects/wagicGPT,
game dir projects/mtg. READ FIRST, in order:
1. projects/mtg/strategy-design/wave56/review-carry.md  (new-baseline discipline + the
   prediction sets you must adjudicate — PASS / FAIL / UNTESTED; UNTESTED is a real verdict
   when no window arose; never infer).
2. projects/mtg/strategy-design/wave55/strategy-writing-skill.md (the CURRENT skill edition, amendments 1-261:
   how guides for this model tier are written; the perception-vs-strategy routing doctrine;
   the trust doctrine — never teach doubt of the render).
3. Your seats' wave-55 file(s) in projects/mtg/strategy-design/wave55/seats/ (and wave55/general-strategy.md, wave55/engine-ledger.md = the wave-56 docket D1-D35 whose lanes A-E shipped D1-D18 + O8, plus wave56/lane-*.md for what each lane actually changed; wave55/reviewer-boundary-pass.md for the B1-B9 guide edits now live) (rung state,
   open predictions), and the live guides projects/mtg/bin/Res/ai/baka/deckN_strategy.txt.

CORPUS: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-120952/ — one JSONL per SEAT
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
- projects/mtg/strategy-design/wave56/seats/seat-<ids>.md — same shape as the wave-55 seat
  files: per-deck findings with counts (N/M windows), prediction adjudication (set A guide
  edits for your decks, set B engine lanes where your seats had windows), rotation evidence,
  engine/render items ranked HIGH/MED/LOW with a concrete repro (file, seq), a lategame
  specimen candidate (file + seq), and falsifiable predictions for wave 56.
- projects/mtg/strategy-design/wave56/deck<N>/strategy.txt — the FULL revised guide per deck
  (start from the live guide; every edit justified by a finding; numbers read off the engine
  render/primitives). Plus deck<N>/edit-texts.md: each edit as before->after with its finding.
- projects/mtg/strategy-design/wave56/deck<N>/skill.md — proposals for the strategy-writing
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
stall-floor fix) — wave56/review-carry.md lists every falsifiable prediction and flag, and lane
A's wave-54 HOLD predictions are re-adjudicated HERE. Standing owner rulings: X menu
largest-first; history lines owner-prefixed; hard caps on legal choices rejected on sight.
CORPUS NOTE (wave 56, FIRST corpus carrying wave-56 step-1 lanes A-E; see wave56/review-carry.md
for every prediction): **21/21 natural** (0 timeouts, 0 crashes, 0 hung, **0 SEGV** — D17's
UAF fix; the base binary crashed 2/5 stub games); wall time ~2 h 10 min. 2,762 decisions
(wave 55: 3,171; ask 2,127 / priority 456 / attackers 88 / discard 55 / blockers 25 / reveal 9
/ bottom 2), inference 21.96 h = 28.6 s per decision (wave 55: 26.1), max prompt 26,298
(`125v130` seq 113), prompts > 30 K: 0. Harvest (orchestrator first read — RE-DERIVE, my
predicates are named where uncalibrated): 6 fallbacks (0.22%: named_row_reask 3, stale_echo 1,
wall_miss_unrecorded 2; NO plan_choice_conflict — was 9 — but check whether lane A's narrowing
moved it to `plan_choice_conflict_narrowed`, which my harvest read as 0: count the stamp
yourself), 6 `recovery`, 2 `wall_miss` + `wall_miss_events` 3, `latched_row_mismatch` 0,
`long_reply` 1, `identical_option_asks_resolved` 3, **`giving this decision to the heuristic` 0**,
**`dropping stale async answer` 65 (was 43; lane A's D18 target < 20: FAIL on first read** —
find lane A's slot-rescue stderr line (8 lines matched slot|rescue|consum) and say whether the
rescue ever fired and what class the 65 drops are; the turn-flip seam alternation is expected
to survive). **HOLD ROW: 2,016 renders / 320 takes (15.9%; prefix-matched, exact == prefix this
corpus)**, `hold_windows_skipped` non-zero on **30 of 42** gameends (was 21/42), sum 1,236 (was
735); **D1: byte-identical same-turn re-asks after a take 10 (was 113; target < 20: PASS on
first read)**; identical-declined runs 14.3% of decisions, max run 12 (targets < 15% / < 10:
share PASS, max FAIL by 2 — measure both denominators per #208). **D2: 147 ability rows carry
`they would be at K`, 0 mismatches vs opp_life - damage (was 238/248): PASS** — confirm a lethal
face row appeared at least once. D6: `INCOMING THIS COMBAT` present on 38 of 40 opponent-combat
windows by my loose predicate (the 2 misses were Upkeep/Main windows my regex mis-tagged —
re-count on Blockers-phase windows only). D10 (opponent open-mana count): my predicate did NOT
match lane B's emitter string — read lane-B.md for the literal and count it. D7: 9 X menus, all
with `{X pricing:` clauses and a `[<- largest affordable X ...]`/`[<- most kills ...]` marker
(0 without a fit clause) — check the monotone-collapse rows for out-of-range answers. **D8: 21
back-face land rows rendered (`PLAY THIS AS A LAND` / Pathway `-> PLAY THIS AS A LAND`), 0
takes, 0 arrivals, 0 `Flip Side` takes (was 11): the row exists, the model never took it —
FAIL on arrival, PASS on Flip-Side; deck146/152 seats: was a back-face land ever the right play
when offered, and does the guide's "NOT LANDS FOR YOU" block (skill #238 P10) now contradict the
render? It must retire on this evidence.** D3: 3 `[RE-ASK]` notices, all named_row_reask
(no latched-line shape arose — UNTESTED for the D3 fix). D12: 9 reveals, `reveal_stall` 0,
wait fields on 9/9: PASS. D4: 0 empty `chosen_text`: PASS. D14 equipment-at-0-creatures takes
0. Lane C's phase-7 `only display-toggle (Flip Side) options; auto-passing` line: 760 total,
**263 in one game (146v126), 158 (146v130), 156 (146v152), 56 (146v125) — all deck146 seats,
bounded (games ended) but a run this long is inference-free churn: count per window and say
whether it is the livelock's bounded cousin.** Max seat 61 turns (`125v130`). Win rates: 152
5/6, 162 5/6, 126 3/6, 146 3/6, 130 2/6, 125 2/6, 123 1/6 (123 = 1/6 for the third wave
running — its seat must say whether the guide or the deck is the ceiling). Baseline files:
wave56/corpus-results.tsv, corpus-log.txt, lategame-specimen.txt (`125v130`, see header).
THREE-NUMBER LITERAL AUDIT (#188) for every literal lanes A-D changed (list in
wave56/review-carry.md §B): old-string renders (expect 0), new-string renders, takes. HOLD
take census: PREFIX-match (wave-55 boundary pass finding).
