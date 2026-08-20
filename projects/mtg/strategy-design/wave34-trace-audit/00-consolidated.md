# Wave-34 trace audit — consolidated (6 batches, ~740 traces, 2026-08-20)

Purpose (owner): calibrate the reasoning budget FOR DEVELOPMENT ("we don't care how much
the model needs for optimal decision making, at this time") + extract dev-purposed
findings. Detail + verbatim repros: batch1.md..batch6.md.

## Diagnostic-depth distribution (last novel dev-diagnostic element, ~tokens)

| batch | p50 | p90 | max | churn mean |
|---|---|---|---|---|
| 1 | 3.2k | 5.0k | 6.2k | 17% (interleaved) |
| 2 | 2.9k | 4.9k | 6.0k | 29% |
| 3 | 2.8k | 4.3k | 8.1k* | 23% |
| 4 | 3.9k | 5.1k | 5.7k | 25% |
| 5 | 3.3k | 4.9k | 6.1k | 14% |
| 6 | 1.8k | 3.8k | 4.6k | 41% |

Pooled: **p50 ~3k, p90 ~4.8k, max ≤6.2k in 5 of 6 batches** (*the 8.1k is one trace,
upper-bound instrument). Budget recs: 5.0/7.0/hold-8/8.0/6.5/4.5-5.0k.

## RECOMMENDED WAVE-35 BUDGET: 6,000 tokens

Covers every batch's p90 with margin and 5/6 batches' max; batch-3's floor warning
("below ~6k truncates ~10% mid-diagnostic") is satisfied at exactly 6k. UNANIMOUS
6-batch caveat: **churn is CAUSED and INTERLEAVED, not trailing — a cap cannot recover
it** (b1: ~4 "final check" closers per trace woven through). Fix the churn drivers
below, re-measure on the wave-35 corpus, cut again on evidence.

## The churn drivers (protocol/wording tier — fix before any further cut)

1. **THE PLAN-LINE RULE** (#1 in all six batches; 7–15% of ALL reasoning chars,
   ~100% of traces): "ONLY if your plan changed" is undecidable at the first decision
   (no prior plan), when executing an unchanged plan, and when half-stale; "write your
   complete game plan" + "from here on" misread as validate-against-current-options
   (one trace: 14k chars, 52%). Rewrite: define the null case (first decision: always
   write one), the executing case (omit), kill the "from the list" reading.
2. **Stale-plan annotation false-fires** (fires on 24% of prompts, near-100% false on
   pay-life/attacker/blocker asks; read as a LEGALITY ruling — "This confirms I cannot
   activate Amulet"; b5: it false-fires even when the planned action IS listed).
3. **System prompt template still carries the OLD protocol** ("two or three sentences
   of reason") — contradiction litigated in 9% of traces. One-line sync.
4. **The echo contract vs long option labels**: "action name exactly as listed" is
   unsatisfiable when labels carry truncated {card text} blobs — the mechanism behind
   ALL post_answer_overrun events at two batches (b2 D2, b6 F14). Echo = number +
   short name; shorten labels.
5. Combat/priority asks never state MP2-follows / instants-remain-castable (deepest
   churn driver at b5); ATTACK/BLOCKS worked example contradicts the parenthesised-
   name mandate (b5 #10, 70/102 traces reason about format).

## Render tier (dev findings, new since the docketed narration ruling)

- **Hand lines lack duplicate copy-numbering** (battlefield has it): model DISCARDED a
  real second copy as "a typo in the prompt" — 4 instances, 3 decks; 45/146 traces
  litigate (b5/b6). Cheapest high-value fix.
- `Your battlefield (creatures: N)` header over an ALL-PERMANENTS list (b1/b4/b5;
  84/84 mismatch at b4).
- Mana-line contradictions: `Already in pool: {..}` phantom floating mana incl. on
  TARGET CHOICE asks; `Mana available: N` vs board [tapped] flags; no source
  attribution ("colours you can make" → model confabulates an Urborg); Tolarian
  Academy variable output ignored (b1-b5).
- **Land drop never offered/stated on cast asks** → model simulates an imaginary land
  drop and does mana math on a board it never had (b4 F3 39/91, b6 P2 34/146, b3
  guide-stall 11 traces).
- Mutate label prints a WRONG total ("total 3 mana" for {2}{BR}{W}{W}) (b1); amass
  preview omits Mauhur's +1 while the GUIDE asserts it's included (b6 F6 — guide and
  render must agree on which is authoritative).
- Stack renders `StackAbility. (Source: )` empty (b2 R6); ability tags leak raw
  tokens (`nofizzle`, `protectionfromcoloredspells`); Emrakul's Annihilator 6 absent
  from tags (b6 F11).
- Log arithmetic residuals: first counter of an infect batch prints the PRE-batch
  value then next prints lower (b6 F4) — ⚠ this corpus RAN the N-105f fix; this is a
  RESIDUAL on another path; life lines print post-batch totals on each partial line
  (F5); doubled amass prints one +1/+1 line.
- Poison line suppressed at 0 — design question: infect seats may warrant a standing
  `0 of 10` (b3's 41-trace litigation + b6 F7 9 decisions; NOT a regression — code
  verified intact, renders on first nonzero).
- `Your library: 0 cards` printed beside a 47-card reveal (b1/b5); tutor reveal
  registered as a "top 48" look (b5).
- `[cantblock]` co-rendered with "...but CAN block" (b1/b2); summoning-sick tag on
  OPPONENT permanents during your turn (b2 R4); untapped permanents untagged while
  tapped are tagged + creature mana sources absent from land lists (b3 — one 33k
  trace hunted a 4th source, found Katilda in the last 320 chars).

## Engine tier (needs verification then fixtures)

- **Scarecrone's sacrifice cost is never paid** — activated 12+ times, stays on the
  battlefield, free repeatable draw (b6 F1). Also script "Sacrifice a Scarecrow" vs
  Oracle "an artifact creature" (b2 C1).
- **Fateful Absence offers the caster's OWN creatures as targets** (b5 #6) + its cast
  missing from the log before the target sub-prompt.
- Artifact-count off-by-one vs Master of Etherium P/T (b2 R5).
- b6 F2 Vanishing Verse "excludes opponent lands" — ⚠ VERIFY ORACLE FIRST (monocolored
  vs colorless lands; the trace's rules claim may itself be wrong).
- Glaze Fiend read as static +2/+2-per-artifact in 16/17 traces (real: triggered,
  until-EOT) — card-text/annotation surface question (b4 F9, b5 #9).

## Instrumentation anomalies (verify before wave-35 relies on them)

- b6: budget_hit fired at 12,058 chars while 29,027-char traces did not — the cap
  counts TOKENS (chars/token varies) but VERIFY the detection isn't misfiring.
- Degenerate decode collapses exist and are invisible to answer metrics: 415-repeat
  mojibake trace with a WELL-FORMED reply (b3), 13.8k-char repetition loop (b5 #2),
  "Amulet"→"Amunt" degradation past 12k chars (b5 #13). Consider a trace-degeneracy
  counter (repetition ratio) in the translog.
- Priority asks are the worst value-per-token (18 near-identical × 14.7k chars in one
  game, b6) — a per-kind budget or cheaper priority ask shape is a candidate, on
  evidence, after the wording fixes.
