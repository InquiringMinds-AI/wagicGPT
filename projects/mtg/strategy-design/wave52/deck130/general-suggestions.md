# deck130 -> core-prompt / render proposals (wave 52)

Layer-routed; engine defects are in `wave52/seats/seat-123-130.md`.

## G8 (HIGH, parser scope). `decision_reversed_in_prose` fired 3 times and was right 0 times.
Lane J's prose-verdict check produced all three of this seat's fallbacks: v125 s48, s66, s82
(`fallback: plan_choice_conflict`, `parse_note: decision_reversed_in_prose`), each recovered on
the re-ask (`plan_choice_conflict_recovered`). In all three the re-asked answer was the SAME row
the first coded line had named (s48/s49 pass, s66/s67 pass, s82/s83 the Spellbomb at the face).
What tripped it was deliberation, not reversal - "Wait, re-evaluating.", "Actually, looking at the
board...", "However, the guide says" - inside a PLAN line that then names no row at all.
Ask: require the prose verdict to NAME a row (a number, or a short name that matches one) before
it counts as a reversal. A prose paragraph that reaches no named row is thinking, and the coded
line stands. As written the check costs a full extra inference per occurrence on this model, and
3 of my seats' 5 fallbacks are it.
(Contrast: `answer_replaced` - a SECOND coded CHOICE line - fired 5 times at my two seats and was
correct 5 times, including deck130 v152 s17 where the revision from "Cast nothing" to "Cast
Talisman of Impulse" was the right play. That mechanism should not change.)

## G9 (MED, render). The battlefield line's collapsed handles make a counting gate hard to read.
`Island #1-#4 x4; Plains #1-#3 x3; Seachrome Coast` is nine lands in three printed rows. Eight of
this deck's nineteen land-destruction casts went at boards of five to twelve lands (v125 s61,
s70, s75, s95, s97, s100; v126 s18; v146 s38). Their battlefield header already prints
`(N permanents listed, of which K are creatures)`.
Ask: extend that header to `(N permanents listed, of which K are creatures and L are lands)`. The
guide's central gate ("FOUR OR FEWER lands") then reads off a printed number instead of a manual
sum over collapsed handles, exactly as the creature gates already do.

## G10 (MED, render). Nothing on a damage row says the target is a draw punisher.
v162 was lost 0 to 20 to FORCED draws (two Howling Mines) under Fate Unraveler + Underworld
Dreams. The frame prints `DRAW PUNISHERS on the battlefield: theirs - Fate Unraveler. Every card
YOU draw costs you 1 life to theirs.` in 59 windows, and rule #0 held 59/59 - zero voluntary
draws. But the damage rows that could have killed the Unraveler carry no back-reference: the
Starstorm cast row says `At X=4 - kills THEIRS: Fog Bank, Fate Unraveler`, with nothing marking
which of those two names is the one charging the pilot life every turn.
Ask: tag the punisher wherever it appears as a damage/removal target -
`Fate Unraveler (3/4) [DRAW PUNISHER: costs you 1 per card drawn]`. Same construction as the
existing `[defender]` and `[planeswalker]` tags, and it makes the frame's summary line
actionable instead of only prohibitive.

## G11 (MED, render). The cleanup discard ask prints duplicate markers in the hand line but not
## in its own numbered list.
v162 s16: the CURRENT SITUATION hand line reads
`Talisman of Impulse (copy 1 of 2 in your hand) {2} [artifact]; ... (copy 2 of 2 in your hand)`,
but the numbered discard list beneath it prints `1. Hammer of Bogardan {1}{r}{r} (sorcery)
{card text: ...}` with no copy marker and no castability note. The pilot sent a castable Hammer
and the {1} Spellbomb while four duplicates were on the screen.
Ask: carry the `(copy N of M in your hand)` marker onto the numbered rows, and append
`{castable this turn}` where the current mana covers it - both facts are already computed for the
casting menu.

## G12 (LOW, render). `Deal 2 damage with Siege-Gang Commander targeting the opponent` does not
## price itself against the guide's own floor.
Face rows read `The opponent (player, life N)` on target menus but
`... targeting the opponent` on ability rows, with the life total only in the frame. Six casts
above the floor this corpus.
Ask: make the ability row carry the number too - `targeting the opponent (player, life 21)` -
so the same string decides both shapes.

## G13 (LOW, cost). Latency and reply overrun at this seat.
381 decisions, p50 21 s, p90 55 s, 36 over 60 s, one at the 900 s timeout wall (v162 s18, empty
reply, `fallback: empty_reply`). `post_answer_overrun` non-zero on 312/381, max 6,901 characters.
The three slowest (v152 s17 577 s, v125 s18 225 s, v125 s33 199 s) are all windows where the
model recomputed mana projections by hand. See G7 for the shared ask.
