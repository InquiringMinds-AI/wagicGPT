# deck133 wave-7 proposals for the strategy-writing skill

Mode this wave: OBEYED-BUT-LOSING / TURNAROUND (record rose 3/6 -> 4/6, best ever; beat the
#1 deck). All four wave-6 fixes validated. The proposals below are METHOD refinements the
deck133 seat is well-placed to contribute; each is flagged single-seat vs cross-cutting.

## 1. [METHOD — strengthens an existing rung] "Read the NARRATION, not the plan prose" — a resolved-vs-asserted check the harness-sweep should make explicit.
The skill already teaches "cast happened is not effect happened" and "a log-tail fact gets a
confident false reading." This wave produced the sharpest instance yet at the RESOLUTION stage,
not the cast stage: vs44 seq27 the pilot cast Gray Merchant, its seq28 PLAN asserted "opponent
goes to 3, I gain 8" — but the narration shows `Gray Merchant: hand -> stack` -> `Counterspell:
hand -> stack` -> `Gray Merchant: stack -> GRAVEYARD`. The spell was COUNTERED and never
resolved; a reviewer who trusted `chosen_text` + the plan prose would have recorded a
"+2-offset-confirmed GM finisher win" that never happened, and mis-attributed the WIN mechanism
(it was Obliterator beats). PROPOSAL: add a one-line check to the harness-defect sweep / Step 2:
"For any cast whose EFFECT you are about to credit (a drain, a counter, a kill), confirm
`stack -> battlefield` (or the effect's own life/zone events) in the narration — `stack ->
graveyard` after an opponent spell = countered, and the pilot's PLAN will assert the intended
effect regardless." This is the resolution-stage twin of the existing cast-happened anchor.
Cross-cutting (any deck with a finisher an opponent can answer). CONFIDENCE: high — it changed
my own attribution mid-review.

## 2. [METHOD — sharpens turnaround/obeyed-but-losing] When an engine-SURFACED number exists but the pilot recomputes it by hand and errs, that is a REPRESENTATION signal, not a guide-wording one — even in a WIN.
The skill's wave-5 line "prefer an engine-surfaced number, verify its offset" assumed the pilot
would READ the surfaced number. vs131 seq72 shows the pilot IGNORING `{right now: drains 5}` and
manually counting devotion (got the wrong number, cast anyway, happened to win). PROPOSAL:
extend that rung — "a pilot that manually recomputes a value the option already displays is a
representation defect (the display isn't trusted / isn't the RESOLVED value), not a
guide-arithmetic failure; the fix is to make the option show the FINAL value so no runtime
math is invited, and the guide's 'add N' workaround becomes deletable." Note the trap: this
surfaced in a WIN, so a win-only review would miss it. Single-seat but mechanically general
(GM's +2 under-display is the concrete case; any option showing a pre-modifier magnitude
qualifies). CONFIDENCE: medium-high.

## 3. [WATCH — do not promote] Discard as interaction has a THREAT-AXIS blind spot: it answers reach/counters/sweepers (depth), never a go-wide token engine (width).
vs109: the anti-burn strip fired (took Searing Spear) and was IRRELEVANT because deck109 kills
by Goblin width, not burn depth. The skill already has "META SHIFT" and "residual -> deck
construction"; this is a narrower, reusable observation for disruption-based decks: a discard
line's value is bounded by whether the opponent's clock is CARD-limited (few key cards discard
can strip) or BOARD-limited (a token/recursion engine that floods faster than 1-for-1 discard).
PROPOSAL: a one-line watch in Step 1's redundancy/role analysis — "before crediting a discard
plan vs an aggro matchup, check whether that opponent's kill is card-bound (discard helps) or
width-bound (discard barely helps; the answer is a wall/sweeper)." Single-seat; keep as watch,
promote only if a second discard deck reproduces the width blind spot.

## 4. [CONFIRM — existing method held] Turnaround-preservation freeze mode produced a 5-clause diff on a 4/6 wave; nothing needed rewriting.
No new method, but a positive data point: applying the freeze-check discipline to a rising deck
yielded exactly the right output size (near-zero change, residuals routed upstream). The
temptation on a 4/6 "best wave" is to add ambition; the corpus said freeze. The skill's
OBEYED-BUT-LOSING mode + "size the residual lever's ceiling" gate both fired correctly — the
losses' ceiling (both -1 construction races) does NOT justify a #1-slot lever, so no top-slot
churn. Keep the mode as written.
