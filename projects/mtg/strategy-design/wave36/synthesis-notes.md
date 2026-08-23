# Wave-36 synthesis — TWENTY-FIRST cycle (validation wave)

Corpus matchups-20260822-155508: FP8 pilot, revised guides, binary bb3cc9b12, 21/21
natural, **0 fallbacks / 1,390 decisions — the campaign's first ZERO-fallback corpus**
(prior record 1/1,277, wave-33). 1 budget hit, 0 degenerate, p50 110s, no caps.

## Rotation adjudication (adherence/maturity per the standing rubric)

- **FROZEN: deck105** (≤13→BLOCK 17/17 exact incl. boundaries; 0 hint overrides;
  Ichorclaw class dead), **deck158** (G1 beliefs 0/149; kill tests 12/12; 0
  transpositions), **deck116** (R7 2/2; R5 residue 0; multi-block 0/10 legal).
- **FROZEN WITH WATCH: deck36** — one clause applied this commit ("Tolarian Academy
  taps for casting SPELLS - playing a land is NOT a cast"; its 1/28 Academy miss);
  Academy HL10.1 discharged (27/28 on first exercise).
- **HOLD deck152**: trade-trust note reduced printed-death blocks 3→1 but the survivor
  RESTATED the parenthetical then asserted the opposite (silent drift — wording watch);
  RETURN-SWING 0/1 where armed (the exact death it targets) + needs a growth term;
  Katilda-vs-infect precedence. One revision + validation.
- **HOLD deck146**: frozen doctrine held; NEW combat-value teaches not binding where
  armed (hold-back computed 7-vs-6 then attacked "because tapped"; false damage model
  chump). Command timing 3/4 (one MAIN-2 pump recurrence). One revision + validation.
- **HOLD deck139**: no text changes — mutate over/under 7/8 incl. the exact wave-35
  failure shape corrected with the guide quoted; gate = engine shockland fix landing
  (put-land asks contaminate its ledger) + one clean corpus.

## Step-1 fix validation (falsifiable predictions, counted)

VALIDATED: compound-mode attribution 2/2 (zero enemy pumps); returns-NOTHING magnitude
4/4 steering; BOTH-mains fact (0 pre-main hallucinated casts, ×40); combat-stall 0
recurrences (tracer silent); blockers-skip 0 recurrences (the one missing-ask candidate
was a tapped blocker — instrument fired correctly); trade-trust note 12/12 renders,
class 3→1; multi-block/echo-index/livelock machinery: 0 occasions corpus-wide (the
failure shapes themselves did not occur); Finks hybrid cast live-confirmed.
REFUTED (both = LIVE-PATH INERT, the tested-the-helper-not-the-caller class):
1. **Shockland already-tapped truth** — ≥7 tapped-shock asks still said "enters
   UNTAPPED"; 2 no-op pays; lying narration recurred. Lane A's own caveat named this
   exact link (`ctx->isTapped()` unreadable at the menu-arming site). FIX MOVES TO THE
   MENU-ARMING FLOW (wave-37 step-1 #1).
2. **damageTargetVerdict** — 0 renders corpus-wide (no qualifying ask reached the
   hook). Needs an arrival trace + live-path verification (wave-37 #2).
Plus two small live-path bugs found: `payRepeatModeNote` casing ("Add " vs live
"add ") — never fires (its PARSETEST fixture tested the wrong casing — fixture-lies
class); search asks promise an "[eligible]" marker that never renders (10/10).

## The lesson (method, recorded)

An annotation fix is not shipped until an ARRIVAL TRACE shows it rendering in a live
prompt. PARSETEST pins the helper's strings; the caller's arming context is where both
refuted fixes died. Wave-37 step-1 briefs carry this requirement explicitly.
