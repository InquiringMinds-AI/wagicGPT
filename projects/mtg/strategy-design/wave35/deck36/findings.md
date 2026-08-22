# deck36 (Master of Ether — UB Artifact Aggro) — Wave-35 Guide Review

Seat: deck36 · Deployed guide: `Res/ai/baka/deck36_strategy.txt` (wave-33 revision)
Corpora, arm-weighted per the brief: **C** (FP8 35B, `matchups-20260822-111102` — the standing
opponent, the pilot this revision must teach) > **B** (122B, `matchups-20260821-033000` —
under-specification detector) > **A** (retired 4.75-bit, `matchups-20260820-192210` + 022400 rerun —
background only, no tuning to quant damage).
Pre-digested evidence: `wave35-review/deck36-tier-review.md` (arms A+B) and
`wave35-review/deck36-fp8-review.md` (arm C). Cited below, not re-derived; fresh trace pulls were
made only where a wording rework needed the exact render string.

Ruling #1 governs: win rate irrelevant; adherence per decision + divergence quality.

---

## 1. What the corpus says about the deployed guide, per section

### 1.1 Sacrifice ladder + keep-list — HELD at arm C. FROZEN.
Two full annihilator gauntlets at arm C (F2 vs deck116, T13 + T15): keep-list intact, every
intended pick ladder-rule-1 Pest-first; no creature, no Academy Ruins, no Nuisance Engine ever
chosen (fp8-review F2). Arm B: ladder-clean throughout. Arm A's one keep-list break (A3 seq30,
Academy Ruins over Great Furnace) did **not** recur under FP8 -> quant damage, not a wording defect.
Per Step-0 turnaround-preservation, this section's wording is frozen verbatim in the revision.
The persisting one-per-ask OVERRUN (F2 s29/s39/s40) is reply-protocol/harness, not this text —
the model's *intent* was ladder-correct in all three events; routed to notes.md, not guide-patched.

### 1.2 Academy Ruins recursion — the restriction NEVER ENGAGED at the decision point. REWORKED.
The focus item. Arm C F2 s28 T12 (verified in-trace this review): the option rendered
`1. Put in Library with Academy Ruins targeting Cathodion [cost: {1}{u}, Tap]` and the reasoning
planned lethal math around the topdecked Cathodion, committed `CHOICE: 1`, and **never once
mentioned the restriction** — not argued with, not seen. Same shape at arm A (A3 seq23 T10,
Scarecrone target). Diagnosis per Step 0's attribution ladder: not a representation gap (the target
name is printed on the option line the pilot reads) and not a belief the pilot rejects — a
**placement/keying defect**. The rule lived as the tail clause of a Key Cards bullet
("...costs your next draw, so use it only on Master of Etherium or Akroma's Memorial"), keyed to
the card name "Academy Ruins", while the decision point renders as **"Put in Library with ...
targeting <Name>"** — the lexical surface the pilot pattern-matches never contains the guide's
anchor phrase in a rule-shaped sentence. Fix (Step 5: procedure over the visible option text, closed
exception list): a dedicated block keyed to the literal option string, one test — read the name
after "targeting"; Master of Etherium or Akroma's Memorial -> take; ANY other name -> 0 — with the
WHY (replaces your next draw) and the observed excuse named ("it helps my lethal math" does not
override; only the two names do). Recap line added to DECIDING SITUATIONS (tail re-fire). Old Key
Cards clause compressed into a pointer (HL3 gate answer (b): the fact's carrying surface is the new
block; survivor re-read confirms it states the fact, not just the condition).

### 1.3 Tolarian Academy turn discipline — UNTESTED at arm C; reworked decision-point-first, flagged unvalidated.
Arm C never drew Academy (0 prompt mentions across 174 decisions — fp8-review class-2 row), so per
HL10.1 arm C is a **disqualified witness** and this rework ships unvalidated. Evidence base: arm A
misuse A4 seq20 T6 (tapped Academy on the opponent's blockers phase — "free mana to use later this
turn or next turn") versus arm B's reference game (B4: seq8 own-MP1-while-casting, seq28->29 MP2 to
power Memorial, seq16 passed at opp upkeep). The deployed text enumerates windows
(upkeep / opponent's turn / your MP1) — the pilot must classify the current window before any rule
fires, and arm A's failure mode was exactly a misclassified "later use" rationalization. Rework:
lead with ONE gate question at the decision point ("is this YOUR Main Phase 1 AND am I casting
something this turn? unless BOTH are yes, answer 0 — the offer comes back"), name the anti-pattern
verbatim ("tap now for later" is always wrong — mana dies at end of step), and keep the three
load-bearing obeyed tokens verbatim: mana-disappears fact, Pest-before-Academy ordering (B4
seq28-29), "a listed option is already payable". Note: arm B's justified audible (B4 seq17, Pest on
opp's MP1 with Academy explicitly declined) is Nuisance-Engine-seam, not Academy-seam; the gate
does not forbid it.

### 1.4 Blocking — one real FP8 failure + one false guide premise. AMENDED.
- **F4 s19 T8** (verified in-trace): menu printed `B1. Yotian Soldier (1/4) [vigilance] - may block
  A1 (you kill it, your blocker lives)` (Luminarch Aspirant); trace asserted "If I block, I take 4
  damage and lose 2 creatures" — both halves false against the printed line — and declined the free
  kill; the unkilled Aspirant fed the Wolf to 10/10 and the game was lost. Arm C's ONE outright
  misread of a correct annotation. Note the same prompt's generic header ("block only where the
  trade favors you; taking damage while ahead is often correct") plausibly *licensed* the decline —
  the brief's deck152/105 generic-hint conflict has a deck36 face (notes.md).
- **F2 s23 T11**: guide premise "put them in front of the biggest attacker every turn, they survive
  it" is FALSE vs 5+ power (Jin-Gitaxias 5/4); the pilot correctly refuted it from the printed
  outcome (divergent-neutral). A guide sentence a sound pilot must refute is a guide bug (Step-0
  echo-inverse).
Fix: an outcome-line trust rule — "the parenthetical is the computed outcome; believe it over your
own math, both ways: a '(you kill it, your blocker lives)' line is a FREE KILL, take it every time,
even when the attack is small and NOT lethal" — plus the survive-it premise replaced with an
outcome-keyed default and the existing <=12-life chump off-ramp (which arm C executed correctly at
F2 s33, Steel Wall on Kozilek 12/12). Recap line in DECIDING SITUATIONS.

### 1.5 Everything else — obeyed at arm C; FROZEN.
Cheapest-first/artifact-land sequencing (F1 s2-s6, F4 s2/s6), Master first-listed (F1 s12, F6 s15),
Memorial first-listed (F3 s19, F6 s18), deals-0 exclusion (F5: nine of nine attack asks), <=12-life
hold (F2 s36, F4 s29 correctly *sent* at 2 life on correct arithmetic), infect blocking (F3
s9/s13/s21/s26), Scarecrone only-when-behind (F6 s13 passed while ahead — a discipline point arm A
failed), mulligan by engine count (F1 s1). Arm B under-specification scan: its audibles
(second-Memorial redundancy B6 seq22; lethal-assured holds B1 seq26/27) are lethal-endgame
judgment calls — legislating them risks the brake-absolutism trap on the weaker pilot for near-zero
gain (the FP8 arm showed the same faculty unprompted at F5 s31, the lethal-assured land hold). No
text added. All frozen lines survive verbatim; freeze-check passed against the revision.

## 2. Adherence summary (context, per ruling #1)

Arm C: 174 decisions, 2 comprehension failures (F2 s28 recursion; F4 s19 outcome-line), 3
prompt-assisted overruns, ~97% adherent, both audibles argued from true board facts. Arm B: 0
failures, ~95%, audible quality exemplary. Arm A: 8 failures, ~86%, mostly proven quant damage
(fp8-review class table). Both arm-C failures are addressed by this revision's two reworks; the
overruns are harness-lane.

## 3. Rotation-relevant observation

Adherence/maturity-shaped (ruling #1): the guide's core loop is CONVERGED on the standing pilot —
every scripted section executed at 97%+ across 174 decisions, and the two model-caused failures
both live at rules the revision re-keys rather than at missing strategy. If wave-36 validates the
two reworked blocks (recursion discipline; outcome-line trust) and the harness closes docket-#1
phrasing, this seat approaches byte-freeze; the one hole is that Academy turn-discipline remains
UNEXERCISED at the standing pilot (0 draws in arm C) — a freeze before it samples would ship an
unvalidated section, so per HL1's hard gate the seat should stay one more wave until Academy play
is witnessed under FP8.
