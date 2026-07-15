# Wave-10: reply-protocol intent-collapse — design record & control baseline

## The defect (ledger #1, 4 seats across waves 8-9)

The head-first reply protocol committed the answer token BEFORE the model reasoned in its
PLAN. When the plan concluded a DIFFERENT option for the SAME window, the recorded choice
stayed locked to the stale head. Labeled positives:

- deck131 vs44 seq1 (1784046898): head `1`=Keep, PLAN reasons "...Therefore, I must
  Mulligan" — one-shot decision, cost the game.
- deck135 vs131 s14 (1784047950): head `4`=Cast nothing, PLAN reverses "wait, I *can*
  cast Coatl!".
- deck109 vs44 seq7 (1784040589): attackers head literal `none`, plan wanted the attack.

## Offline study (2026-07-15, corpora matchups-20260714-094942 + -044131, 2978 records)

- **Arm A (accept LAST bare integer) is a no-op**: only 4/2978 replies contain a trailing
  bare integer differing from the head — the model does not re-emit a corrected number.
  The labeled mulligan positive has NO second integer anywhere. Arm A rejected.
- **Arm C (non-numeric head repair) is marginal**: 3 non-numeric heads, 2 of them
  garbled-reply corruption. Rejected as a standalone; the attackers "none" case is fixed
  by the same answer-after-plan move.
- **Arm B selected**: reversal-language + concluded-option-mismatch detector flags ~120
  records (~4.0% raw; detector has false positives from future-tense planning — treat as
  an UPPER BOUND and compare with the SAME detector on the wave-10 corpus).

## The shipped design (arm B)

- Reply contract reordered: PLAN first ("the plan is where you reason"), answer LAST on a
  labeled line: `CHOICE: N` (priority/menus/asks), `ATTACK: A1, A3|none`, `BLOCKS:
  B1:A2, ...`. Protocol text + all four per-ask tails updated.
- `consumePlan` recognizes the LAST answer label at a line start (markdown decoration
  tolerated), returns its line remainder as the decision part (wrap-to-next-line
  tolerated), and trims the carried plan at the label. Downstream parsers unchanged.
- Backward compatible: a legacy head-first reply parses exactly as before (replayed all
  2977 fresh-call ask/priority records: 2977/2977 identical choices).
- Truncation guard: a reply that leads with `PLAN:` but lost its answer line parses as
  FAIL (heuristic answers) — plan-prose integers can never hijack the choice.

## Control-arm baseline (wave-9 corpus, head-first protocol) — compare wave-10 against

| metric | control (wave-9) |
|---|---|
| ask/priority records | 2978 |
| reversal+mismatch detector flags | 120 (4.0%, upper bound) |
| non-numeric heads | 3 |
| unparsed_reply fallbacks | ~0 (1 empty_reply) |

Wave-10 success criteria: detector flags materially down (the labeled-positive CLASS gone
— no head-vs-plan disagreement where the plan names its own concluded option); answer-label
compliance ~100%; unparsed_reply/fallback rate not worse than control; no defer/desync
regression. Study scripts: scratchpad intent_collapse_study.py / replay_parser.py
(session-local; re-derivable from this record).
