# deck62 wave-19 -- general (cross-seat) suggestions

## 1. (VALIDATED at the filing seat) The wave-19 combat name-tolerance batch works -- confirm it at the other seats that bled block/attack fallbacks (110/14).
deck62 filed this fix last wave (wave-18 N2) and it SHIPPED as the wave-19 batch (parseBlockAssignments
name->label second pass; ineligible-only ATTACK->none). Three live saves at deck62 that would each
have unparsed last wave: vs14 s8 (mixed `ATTACK: A1, Saproling, Saproling` -> Canopy Spider, sick
Saprolings dropped), vs35 s33 (over-named `ATTACK: A1,A2,A3, Druid, Saproling, Saproling` -> the 3
eligible Argothians), vs27 s23 (NAME:NAME block `Yavimaya Enchantress: Walking Dead` -> resolved).
deck62 attacker + block-name fallbacks went 2 -> 0. The brief predicted this collapse at 110/14/62;
CONFIRMED at 62. The other filing seats (110, 14) should verify the same and credit the engine, not
the guide, for the fallback-class collapse.

## 2. (cross-seat, LOW) Redundant global enchantment in hand still amplifies decode loops -- an engine annotation would help where the guide cannot.
Re-filed from wave-18 N5. deck62 vs14 s21 and vs110 s1 both hit a decode-repetition loop that was
amplified by a redundant Primal Rage in hand while a copy was already on the battlefield (vs110 s1
had TWO in the opener). The deck62 guide already carries the "a 2nd copy does nothing" note and the
model QUOTES it ("redundant but harmless") -- yet still burns tokens on it inside the loop. The guide
has done all it can; the durable nudge is an engine representation tag on a hand card whose global
effect is already active, e.g. "Primal Rage (already in play - a 2nd copy does nothing)". Any deck
with duplicate global enchantments (Anthems, Rage effects) hits this. LOW priority.

## 3. (cross-seat, model/decode watch -- NOT a guide item) Decode-repetition-loop unparses are below the guide layer.
4 of deck62's 7 unparses this wave were verbatim-repetition decode loops (a phrase repeated to
truncation, no advancing reasoning), firing across all life states and even on a mulligan (vs35 s10,
vs135 s14, vs14 s21, vs110 s1). This is the deck135 REPEAT-LOOP signature at a second seat. It is a
model/decode pathology, not a strategy or representation defect -- a guide line cannot stop it (the
model quotes the correct guide line inside the loop). Route to the model/decode watch. Reviewers
should bucket truncation-unparses by whether the tail REPEATS (decode loop -> model watch) or
PROGRESSES (reasoning spiral -> guide/representation) before proposing any fix.

## 4. (observation, not a change) deck62's record is a matchup-shape floor, not a play problem.
2/6 again, with a different pool result (the wave-18 wins vs49/vs35 flipped to losses; vs27/vs135 came
in). One razor-adj loss (vs14 10/11, opponent stole the Yavimaya beater with Control Magic), one race
loss (vs49 at 2 life from T7 vs red aggro), two matchup-floor blowouts (Slivers go-wide, affinity).
It is a turn-8-13 ground/go-wide deck that floors against faster/wider clocks and grinds razor races
vs midrange. The guide cannot fix a clock deficit; do not chase the win-rate with construction-style
prose for a single-seat matchup floor (route to the roster owner, not the guide).
