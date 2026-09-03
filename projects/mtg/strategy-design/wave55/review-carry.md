# Wave-55 review carry-list (assembled 2026-09-03 at corpus prep)

Corpus: twelfth fair-hand corpus. Binary = hermetic `make -B` of master **b228b8648** (wave-55
step-1: lanes A 8fe5c6682, B f7d13d99d, C 38ccaae55, D 8f9ee5736, E d56efcdaf on base 8a1768d1c =
wave-54 step-1 + P purge + Q autotap search + R stall-floor hotfix). Gate on b228b8648: suite 1229 (2 failed = lifeline + merrow_reejerey, the known concurrency-only pair), 51 AI/0, 0 timeouts, PARSETEST 2636/0.
Archived `archives/wagic-b228b8648-w55step1`. Model: qwen36-35b-a3b on Spark :8084. Guides =
wave-54 reviewer revisions (5160eb1cb) + boundary pass (dd0cd2d74). Same pool/recipe as wave 54
(-j 21, -T 0, WAGIC_GPT_TIMEOUT=900, MAXTOKENS=6000, REPPENALTY=1.05). Vita: vpk13 (f29684e59)
is the owner's live build; a vpk14 CHECK build of 6d6fe7cef compiled clean (not uploaded).

## Discipline (owner doctrine 2026-08-26)
Reviewers own guides; synthesis owns skill + general; the core loop is the purpose. Adjudicate
render predictions by the emitter's actual string — and COUNT FROM THE RENDERED PROMPT, never
from the translog `options` array (wave-55 lane D found three docket numbers derived from the
array that did not render). Baselines from wave 54 (21/21 natural; window economy CONTAMINATED by
the stall floor): 2,212 decisions, 23 fallbacks, 0 timeouts, `dropping stale async answer` 488
(468 = stall-floor releases at 12,002 ticks; wave 53: 63), HOLD row 426 renders / 43 takes,
identical-declined runs 16.1% (max run 50), opponent-turn share of decisions 19.9% (wave 53:
30.2%), prompts > 30 K: 2, inference 14.67 h / 2,212 = 23.9 s per decision.

## D1 discharge (lane R) — the wave's first question
`WAGIC_STALL_FLOOR=0` is the A/B. Predictions: stall-floor release lines on GPT seats **0**
(was 468; the compile-gated line now prints wall seconds); `dropping stale async answer` back
near the wave-53 level (**< 100**; lane E's stub reproduction shows a RESIDUAL class — the
land-drop ask and the casting ask alternating in the single async slot on an unchanged board,
24 of 30 stub drops — which lane R does not address and which belongs to the priority seam:
count it separately and name it for lane A of wave 56); `giving this decision to the heuristic`
**0**; opponent-turn share of decisions back **>= 28%**; per-decision inference **<= 22 s**.
Lane A's wave-54 HOLD predictions (ruled UNTESTED) are re-adjudicated THIS corpus on the
uncontaminated denominator.

## Set B - engine changes since the wave-54 corpus, each with its falsifiable prediction
- A (D2/D4/D11/D19/D21): declined-list + hold keys are the option SET (`optionSetKeyOf`), the
  hold key drops `Your life:`; HOLD row offered on OWN-turn windows too; row text now `Hold
  priority for the rest of this turn: pass now, and do not ask me again unless the board changes
  (...) {taking this row skips the rest of this turn's identical windows}`; exact-label match
  beats prefix. Predictions: decisions inside one mandatory life-loss loop phase **< 5** (was 40 /
  29); identical-declined runs **< 12%**, max run **< 20** (was 16.1% / 50); own-turn runs >= 20
  **0**; `pass_hold_ambiguous` **<= 4**, `CHOICE: 0 (pass)` never stamps; HOLD takes in
  declined-note N>=3 windows **>= 20%** (was 10.4%); `hold_windows_skipped` sum **> 72**;
  the declined-note reaches the own-turn Main 1 / Main 2 pair N/N. #188 three-number audit on
  the new row string (old strings 0).
- B (D3/D13/D14): `all_assignments_illegal` records with no follower re-ask `[RE-ASK]` **0/N**
  (was 1/1); second all-illegal reply stamped `_exhausted` (heuristic handoff kept — a blanket
  no-blocks is the worst combat default); menace header `N of the M attackers need TWO or more
  blockers each; you have K blockers, enough to complete at most J` on 100% of blockers windows
  with a menace attacker, 0% otherwise; the fallback field carries `: <pairs + reason>`.
- C (D6/D7/D10/D15/D16): X menus with no marker **0** (was 2/2; three ranked markers, the mark
  on the BOTTOM row of a collapsed run with both kill lists restated); takes of a `kills THEIRS:
  none` row while another affordable row names an opponent creature **0**; both-player
  `target=player` rows with a one-sided verdict **0/N** (was 178/180; `edictSelfClause` +
  `{this row does not pick a target ...}` commit clause at N>=2 with a player row); casts of a
  card named in an opponent `[named:` tag with no `[NAMED BY THEIR <src>: ... you would be at K]`
  **0** (was 2/2; the tag itself byte-identical); single-target `{kills:` lists without the
  THEIRS/YOURS split **0**; ability rows pointing damage at a player without a life total **0**
  (was 73). **D17 STRUCK** (lane C: 24/24 fetch rows carried the colour clause in wave 54; the
  seat restated wave 53's count) — a regression case now pins it.
- D (D8/D18/D22/D9): library-reveal / cleanup-discard menus collapse `(copy k of n)` duplicates
  (**254 rows / 36,274 chars** in wave 54 — the real item); multi-source ability menus print one
  decode line per later source block (`12-15. The same 4 options as 8-11, with <source> #2 ...`)
  — residual `#N` rows in rendered prompts **<= 12** (was 12, NOT 403: that number came from the
  options array); `chooseaname` header names a CARD NAME menu + `{visible now:}` per row
  (public zones only) + narration `You named X with ...` **9/9** (was 0/9); `discard` rows carry
  `{spare: you control N lands}` / `{dead right now: ...}` (withheld on stack-targeting cards —
  a counterspell is never dead at cleanup) / `{you already control one}` — bare discard rows
  **0**; cleanup discards sending a counterspell past a listed spare land **0** (was 3 of 4).
- E (D5/D23/D12): `reveal_stall` / `reveal_stall_secs` / `reveal_stall_phase` on any parked
  reveal (structural signature ignores poll ticks; poll-churn floor = max(1800 s, 3 x deadline));
  `stale_livelock` on a `reveal` record **0** (was 1); every 900 s wall miss leaves `wall_miss: 1`
  on the consuming record or a `kind: wall_miss` record; `gameend` carries `wall_miss_events` +
  `wall_miss_unrecorded`; the mulligan colour clause says `on turn one` (1 land) / `even with
  every land in this hand in play` (2+): mulligans under a colour-family clause at 2+ lands **0**.
  A/B verdict on D5(b): NONE of the lane-M/L flags moves the per-tick prompt — the mover is the
  seam alternation (see D1 above).

## Set C - flags in this binary (silent-instrument rule)
`WAGIC_STALL_FLOOR=0` (R) · `WAGIC_OPT_SOURCES_CAP=14` / `WAGIC_OPT_WORK_BUDGET` (Q) · lane
A-N flags per wave54/review-carry.md §C · `WAGIC_GPT_DRIFT=1` (E, dev builds only) ·
**KNOWN BROKEN LEVER: `WAGIC_GPT_BOARDINDEX=0` SEGFAULTS (also on the wave-54 base binary)** —
lane L's disable flag cannot be used until fixed; docket it.

## Carried into the wave-56 docket
- The seam-alternation stale-drop class (lane E's finding; priority seam, lane A's file).
- `WAGIC_GPT_BOARDINDEX=0` segfault. Lane D's `nameChoiceMenu` has no suite pin (needs a
  `chooseaname` register). Lane B's exhausted branch has never executed. D20 `{feeds:` conditional
  converters; D24-D27 LOW; D28 owner item; D29 replay + O8 RNG split (owner YES; its own lane
  after this corpus); D31-D35; O7 gnu++14 Vita build (own build, after vpk13 play data); A15(b)
  / A32 projects.
- Vita open reports: 8 lag games (vpk13 `frames` lines + `avg_swp` decide O11); `bug` game
  1788398189; Dominating Vampire (D32).
