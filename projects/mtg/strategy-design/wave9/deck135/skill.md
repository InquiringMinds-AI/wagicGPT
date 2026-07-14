# Deck-135 wave-9 — proposals for the strategy-writing skill

Mode this wave: TURNAROUND / OBEYED-BUT-LOSING, improving (record 1W/4L/1TO, up from wave-8's
0W/4L/2TO; vs131 flipped to a win, vs44 to a near-win at opp 2). The wave-8 develop-imperative
WORKED for the direct-fix case; the wave's job was to verify that terminus and sharpen the ONE
residual sub-shape it did not cover. Guide revised as a constrained diff off the live guide;
frozen lines preserved (see notes.md freeze-check). Proposals below, ranked by evidence.

## P1 (validates + extends a wave-8 method line) — a DISPLACEMENT fix (else-branch imperative) validates the way you validate a re-anchor: the FROZEN-obeyed direct case AND a residual HARDER sub-case, and the sub-case is where the executor's list-vs-hand misread lives.

Wave-8 turned deck135's Rule #1 else-PERMISSION into an else-ORDER (develop when no creature is
castable). This wave VALIDATES it exactly as the skill says validate a re-anchor — by the defect
class going to zero AFTER joining the deciding board state — and the result splits cleanly:
- **DIRECT-fix case: defect -> 0.** Every wave-8 freeze window (a *directly castable* Into the
  North / Glacial Revelation declined to "hold mana") is GONE. vs44, the matchup that froze on 4
  windows and lost at opp 18, now casts Into the North on curve and takes the opponent to 2.
- **INTERMEDIATE-fix case: defect persists (vs133 4 turns, vs140 T6).** When the enabling play is
  not a directly-castable spell but **cracking an uncracked fetch** (because Into the North itself
  is uncastable for lack of mana/color), the imperative did not transfer.

The generalizable method point: **an else-branch develop-ORDER must gate on the OPTION LIST, not
the executor's HAND, and it must name every SHAPE the develop action can take.** deck135's pilot
read a creature in its HAND as "castable" and let Rule #1's own "creature-before-fetch" ordering
VETO the fetch that would enable it — quoting the guide while misplaying ("Do not fetch lands if I
have a creature to cast... adhering to Rule #1 which prioritizes creatures over land development,"
verbatim vs133 s24), the classic guide-caused bug. The authoring move: (a) re-gate the else-branch
on "no creature among your numbered Cast OPTIONS" and state flatly that a creature in HAND absent
from the options is not castable, so the fetch that fixes its color IS the creature play (not
"fetching instead of a creature"); (b) enumerate the develop shapes symmetrically — a directly
castable fix AND a fetch-crack for the missing color, de-biased from one color (the guide's
"almost always the ISLAND / short BLUE" blinded it when the missing color was GREEN). This is the
same "state BOTH branches as countable conditions" discipline, one level deeper: state both SHAPES
of the develop action, keyed to the option list.

## P2 (completes the wave-8 Force-scoping line) — a 2-for-1 reactive card's target list must EXCLUDE every 1-for-1 target class BY THE CARD-ECONOMY RULE, not enumerate only the ones already seen.

Wave-8 froze Force's scoping with a face-burn redirect. This wave the pilot Forced an **Inquisition
of Kozilek** (targeted discard) — a NEW non-qualifying target the guide's "spend it ONLY on [board
wipe / mass-bounce / kill-only-threat]" list did not explicitly exclude. The underlying rule is
card economy: Force costs TWO cards (Force + the exiled blue), so it only breaks even against a
target that costs YOU more than two cards' worth (a board wipe, a board-clearing bounce, removal of
your only threat). A 1-for-1 discard is a LOSING exchange, identical in shape to the face-burn ban.
**Method point: when a reactive spell carries a fixed card COST, write its exclusion as the
economy rule ("never spend a 2-card answer on a 1-card problem") and name each 1-for-1 class the
executor will otherwise treat as "a spell I can counter, so I should" (face burn, discard, a
single small creature/token spell) — an ONLY-on-[list] positive is under-specified because the
executor reads any counterable spell as in-scope.** This extends the wave-6 "a FREE reaction still
costs a CARD — scope by card cost" note from the alt-cost side to the target side.

## P3 (recurrence promotes a watch's URGENCY, not its layer) — reply-protocol intent-collapse now has a SECOND same-deck instance across two waves, and the 2nd is a self-documenting repro; the layer routing is unchanged but the A/B is now due.

vs131 s14: reply head `4` (Cast nothing) while Coatl is castable; the PLAN self-corrects IN TEXT
("...wait, I *can* cast Ice-Fang Coatl! Let me re-evaluate..."), proving the head token committed
before the model finished reasoning. Same class as wave-8 vs131 s48; self-corrected same turn (cast
Coatl MP2), no game cost. **Method point: a WATCH that recurs across waves at the same seat with a
cleaner repro does not change LAYER (still reply-protocol, never the guide) but it does discharge
the "single datum, don't act" hold — a second confirmed instance is the trigger to actually RUN the
armed A/B (accept the LAST bare number / move CHOICE after the PLAN / reconcile a trailing named
index against the head), judged by decision-quality on a symmetric core.** Distinct from
PLAN/CHOICE MISMATCH (there the head matches a passive option and the reasoning is genuinely for a
different action); here head and reasoning are the SAME window, head emitted first and revised in
text but not in the token. Do NOT re-word the guide over it; do NOT score it against a guide rule.

## Non-proposals (freeze / hold, recorded for continuity)
- Rule #1 point 1/2, Rule #2, WHO-IS-BEATDOWN, MULLIGAN, LETHAL CHECK, the board-read anchor, the
  uncastable-key-card anchor, and the core of the Force scoping line all held and are FROZEN.
- 135v133 engine livelock fix CONFIRMED (game completed; Astrolabe casts via snow cost every game)
  — no guide action, retire the wedge caveat.
- Fetch re-offer tax reduced further (~17/game comparable) — ENGINE ledger, not guide.
- Free-reaction-card-cost (Force "free = always fire") — still SINGLE-SEAT (deck135). No promotion.
- vs110 marginal-keep (blue-clog one-creature hand) — construction-adjacent, one datum, WATCH only.
