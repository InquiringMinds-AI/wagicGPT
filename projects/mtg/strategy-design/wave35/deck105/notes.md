# deck105 — Wave-35 engine/harness notes (wave-36 step-1 docket feed)

All repros arm C (`matchups-20260822-111102`) unless marked; arm A = `matchups-20260820-192210`.

## N-105a (RENDER, targeted fix, HIGH VALUE): annotate Ichorclaw Myr's blocker line

The tier-intrinsic 35B rules-prior "blocking Ichorclaw becomes 3/3" survived guide text AND a
contradicting printed outcome in both 35B quants (A vs152 seq9 t6, decision-corrupting; C vs152
seq26 t14, decision-neutral only because the block was mandated and all assignments equal). The
122B never produced it. The proven counter-pattern is annotate-at-the-option (Relic Putrescence's
"only legal targets are YOUR OWN" annotation: 5/5 correct declines arm A, 4/4 arm C). Proposal:
when a creature with a becomes-blocked trigger appears as a CANDIDATE BLOCKER, append the
blocker-side truth to its B-line, e.g.
  "B1. Ichorclaw Myr (1/1) [infect] (as a BLOCKER it stays 1/1 - its +2/+2 fires only when it
   attacks and is blocked) - may block A1 (...)"
Generalizes to any attack-trigger creature offered as a blocker. Guide-side hardening shipped
this wave (mechanism explanation + printed-outcome trust rule), but the class is prompt-resistant
at the guide level — the render seat is the fix.

## N-105b (CORE/RENDER, docket item carried): generic blockers-hint suffix vs deck calculus

The "NOT lethal: block only where the trade favors you; taking damage while ahead is often
correct" suffix is a standing counter-instruction to this deck's numeric rule. Evidence both
directions: 4.75-bit cited it as license to override the guide (A vs36 seq13 t7; leaned on it
A vs139 seq12 t8); FP8 explicitly refused it (C vs158 seq11 t8: "This is just the system
prompt's assessment. I will follow the strategy guide's explicit rule") — but spent deliberation
resolving the collision. Guide now scopes it deck-side ("steps 1-4 REPLACE that advice").
Core-side option routed in general-suggestions.md. This is the brief's docket item — flagged, not
re-litigated.

## N-105c (VERIFIED CLOSED): poison-line zero-state render serves the guide's arithmetic

Measured over all six arm-C deck105 seat files: "Poison counters (you): 0 of 10" renders 73x,
"(opponent): 0 of 10" 48x, full climb 1/3/4/6/9 of 10 present with the "N more end it" tail;
zero-state present in 6/6 games. "Poison counters (me)": 0 occurrences in prompts (10 in
reasoning = model paraphrase, not a defect). Behaviour rode it: C vs116 seq25 t12 "6/10, need 4,
attackers 6 — wins" — won t12 with opponent at 28 life, poison never reconciled against life.
No action needed; recorded per HL10.2 (measurement, not citation).

## N-105d (PILOT COST, informational): FP8 end-of-trace rumination loops

FP8 traces at this seat repeat commit-confirmation lines 5-10x before emitting ("Output matches."
/ re-quoting the BLOCKS line; specimens C vs152 seq26, C vs158 seq11 — visible in the verified
excerpts). Zero decision impact, all reasoning_degenerate <=0.011 (fp8-review harness-health) —
cost noise only. Watch, no action; belongs to the pilot/budget lane if token spend becomes a
constraint.

## N-105e (BY-DESIGN, no action): no-blockers-ask windows while all bodies tapped

C vs146: seat lost 7 life t7-t9 with no blockers ask offered — bodies tapped from attacking.
Same shape as the 122B's arm-B loss to 146. This is the deck's all-in risk profile, not a
harness gap; the new low-life hold-back exception (guide step 3) is the only mitigation that
belongs anywhere, and it is guide-lane.
