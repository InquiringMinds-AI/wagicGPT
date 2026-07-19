# deck62 wave-19 -- proposed additions to the strategy-writing skill (Step 0-ter)

deck62 is now a THREE-guided-corpus seat (revised round 3). This wave it produced a clean
before/after on a fix it FILED itself, plus two generalizable review lessons about what a guide can
and cannot reach. All grounded at this seat; flagged single-seat where they are.

## 1. (STRONG, 2-wave arc) A reviewer-filed ENGINE item that SHIPS and CLOSES its class is the highest-value review outcome -- name the arc across waves and CREDIT the collapse to the engine, not the guide.
Wave-18 deck62 filed N2: "when an ATTACK line names ineligible creatures, drop them and attack with
the eligible subset instead of unparsing the whole line." That fix shipped as the wave-19 combat
name-tolerance batch, and deck62's own corpus is the cleanest confirmation: three saves (vs14 s8
mixed-index+name attack -> Canopy Spider; vs35 s33 over-named attack -> the 3 eligible Argothians;
vs27 s23 NAME:NAME block -> resolved) that would EACH have unparsed last wave, and ZERO attacker or
block-name fallbacks this wave (down from 2). LESSON: when a prior wave's reviewer-filed harness item
lands, the current reviewer must (a) verify it at the FILING seat with seq repros, and (b) attribute
the fallback-class collapse to the ENGINE, then check whether the guide line written as the interim
mitigation is now redundant (it is -- the A-list sharpen -- but keep it frozen; the engine is the
durable layer, the guide line is harmless belt-and-suspenders). This is the concurrent-engine-fix
apportionment rule (wave-18 deck49) applied ACROSS waves: credit the engine for the class it closed.

## 2. The off-case behavioral branch must be scoped to the CONDITION (cannot win this turn), not the SYMPTOM (being behind) -- or it misses the ahead-can't-close spiral.
Wave-18's anti-spiral line was authored from behind-at-2-life spirals and scoped "WHEN YOU ARE
BEHIND." Wave-19 shows the SAME reasoning pathology (re-deriving the board hunting exact
lethal-this-turn, to truncation) firing while the model is AHEAD or even (vs135 s32 at 18/13 with a
winning board -> unparse; vs14 s40 close). Meanwhile the behind-case the line was written for now
resolves correctly (vs49 at 2 life parses to the right survival play). LESSON: an off-case branch
keyed on a board-STATE proxy ("behind", "low life") leaks the moment the same behavior appears in
the opposite state. Key it on the DECISION CONDITION the behavior actually tracks ("you cannot reach
lethal THIS turn -- ahead or behind"). Sibling to the wave-18 name-the-trigger lesson: there the fix
was to bind a rule to the specific WANT; here it is to bind a rule to the specific CONDITION, not the
mood-proxy that happened to co-occur in the first corpus.

## 3. (single-seat, reviewer instrument) Separate DECODE-REPETITION-LOOP unparses from REASONING-SPIRAL unparses before proposing a guide fix -- only the latter is guide-addressable.
This wave 4 of deck62's 7 unparses were verbatim-repetition decode loops (a short phrase repeated to
truncation, no advancing reasoning -- the deck135 REPEAT-LOOP signature), occurring at EVEN life
(vs35 s10 block-math), ahead-ish (vs135 s14), behind (vs14 s21), and on a MULLIGAN (vs110 s1). These
are a model/decode pathology: the guide cannot stop a decode loop (the model even QUOTES the correct
guide line inside the loop and loops anyway). Only the genuine reasoning-spiral (verbose but
advancing, e.g. vs135 s32's real lethal-math) responds to a guide behavioral branch. LESSON: bucket
truncation-unparses by mechanism (repetition-loop vs advancing-spiral) by reading whether the tail
REPEATS or PROGRESSES; route repetition-loops to the model/decode watch (NOT the guide), and only
write guide prose against advancing spirals. Extends wave-18 deck62 #5 (truncation is a behavior
lever not a budget lever) with a second fork: some truncations are neither -- they are decode
pathology below the guide layer entirely.

## 4. (single-seat, confirmation) Trade-outcome annotations should be audited for ACCURACY and for whether the model OBEYS them -- a correct annotation that is ignored is a guide/behavior gap, not an annotation defect.
The wave-19 "(both die)/(neither dies)/(you kill it)/[deals 0 - only absorbs]" block annotations were
accurate at all 19 deck62 block decisions (no mislead) and enabled the value blocks -- but the model
still chump-blocked its 0/1 shroud ENGINE to absorb non-lethal damage while ahead, against the
explicit "[deals 0 - only absorbs damage]" tag (vs135 s25, vs14 s36). LESSON: score a new
representation annotation on TWO axes -- is it correct, and did the model act on it -- and when it is
correct-but-ignored, route the residual to the GUIDE (a protect-the-engine-on-defense line here), not
to a change in the annotation. Mirror of the wave-15 "count and impact are different measures."
