# Deck-135 (Modern Snow) — Wave-9 game review

Corpus: PRIMARY `matchups-20260714-094942` (wave-8 guide live = the current
`deck135_strategy.txt`; post-A/B core prompt; cast-restriction gating + cast-seam livelock
breaker in the oracle). All six deck135 seat games read decision-by-decision from the translogs.
Doctrine applied: decision-level opportunity-conversion audit; the win column is CONTEXT ONLY.
Game->file map at bottom.

## RECORD (context only — not a verdict) — 1W / 4L / 1 TO

| game    | opp (name)              | outcome (deck135)              | my_life | opp_life | turn |
|---------|-------------------------|--------------------------------|---------|----------|------|
| 135v131 | 131 Mind Control        | **WIN**                        | 17      | -4       | 10   |
| 135v44  | 44 Faerie Archmage      | LOSS (near-win — opp at **2**) | -4      | 2        | 16   |
| 135v133 | 133 Phyrexian Asphodel  | LOSS (COMPLETED — was wedge)   | -1      | 27       | 11   |
| 135v110 | 110 Etched Affinity     | LOSS                           | 0       | 20       | 7    |
| 135v109 | 109 Hellrider           | LOSS                           | 0       | 20       | 9    |
| 135v140 | 140 Wipe Them Out!      | TIMEOUT (behind 13v36)         | 13      | 36       | 23   |

**Improved from wave-8 (0W/4L/2TO).** vs131 FLIPPED loss->win; vs44 flipped a frozen-durdle
loss (wave-8 opp 18) to an aggressive near-win (opp 2). Both improvements are creditable to the
wave-8 develop-imperative (Rule #1 point 3) — see Q1. The remaining losses are construction +
slow-clock + two narrow, guide-addressable residuals (Q2, Q3). This stays an OBEYED-BUT-LOSING /
turnaround-mode wave: freeze the obeyed spine, spend the wave on two constrained sharpenings.

## Q1 — DEVELOP-IMPERATIVE (Rule #1 point 3): did development paralysis die? MOSTLY YES.

**The wave-8 paralysis class DIED for the DIRECT-fix case, and it is the wave's headline.**
Wave-8 vs44 froze on 4 own-turn windows, declining a *directly castable* `Into the North` /
`Glacial Revelation` to "hold mana for a creature." This wave that shape is GONE:

- **vs44 (near-win, opp 2):** the SAME matchup that froze last wave now develops every turn —
  Golem T1/T3, Astrolabe, Icehide Golem + Coatl T7, `Into the North` cast T9 (the exact spell
  declined 4x last wave), Treefolk (9/9) T11 and T13, Diamond Faerie T15. It took the opponent
  from 20 to 2. The develop-branch is OBEYED and it flipped the game.
- **vs131 (WIN):** continuous curve — Golem T0, Coatl T4, Treefolk (6/6) T6, Coatl T8. No freeze
  window with a directly-castable fix passed.
- **No value-over-creature overshoot.** Every window a creature appeared in the Cast options it
  was taken on sight (Golem, Coatl, Viper, Treefolk, Druid, Diamond Faerie). Rule #1 point 1 is
  frozen-obeyed. The lone "Cast nothing while a creature was castable" (vs131 s14) is the
  reply-protocol intent-collapse (Q4), not a durdle — self-corrected same turn.

## Q2 — RESIDUAL PARALYSIS SUB-SHAPE (NEW, guide-addressable): fetch-crack-as-development is not recognized when the missing color is GREEN or the fix needs an intermediate step.

The develop-imperative works when the fix is a card the pilot can cast *directly*. It FAILS in the
harder sub-case where the enabling play is **cracking an uncracked fetch** (because `Into the
North` itself is uncastable for lack of mana/color) — and the failure is reinforced by a
**list-vs-hand** misread and a **blue-biased** develop-branch.

**vs133 — 4 turns of dead board (T6->T10) with a creature in hand.** At s23/s24 (T6, life 19,
creatures: 0): hand = Gelid Shackles {w}, `Into the North {1}{g}` x2, **Boreal Druid {g}**,
Search. Mana available = **{u} only** (no green source). Options = only fetch-cracks (a
Misty->Snow-Covered-Forest crack was offered). The green source needed for Druid/Into the North
was one fetch-crack away. The pilot **PASSED at s23 AND s24**, and its own PLAN reveals the
mechanism verbatim:
> s23: "cast Boreal Druid to deploy a creature, following Rule #1. **Do not fetch lands if I have
> a creature to cast**; use leftover mana to fetch if needed."
> s24: "Cast Boreal Druid immediately... **adhering to Rule #1 which prioritizes creatures over
> land development.**"

The pilot reads a creature in its **HAND** as "castable," and lets Rule #1's "creature-before-fetch"
ordering VETO the fetch — but Druid is **not in the option list** (no green), and the fetch is
exactly what would make it castable. It sat creatureless from T6 to T10, finally cracking a fetch
(s33) then casting Druid (s34) — ~4 wasted turns vs a drain deck that ran to 27 life.

**vs140 T6 (milder, same mechanism, two flavors):** creatureless, mana = {g} (1 source). s22/s24
Cast nothing; s23 passed the fetch-cracks. The PLANs show (a) "cast Into the North to fetch an
Island" while Into the North needs {1}{g}=2 and only 1 mana is up — a stale plan to cast an
uncastable fix, refusing the fetch-crack that would ramp to it ("I will not sacrifice my fetch
lands yet as I can cast Into the North first"); and (b) s24: "the only legal action is Arcum's
Astrolabe for {1}, but **I have no generic mana** (my only land is tapped for {G})" — a FALSE
belief: {G} pays a {1} generic cost, and the engine OFFERED Astrolabe (offered = payable). So it
Cast nothing rather than dig.

Three reinforcing causes, all guide-addressable as a constrained sharpening of point 3:
1. **List-vs-hand.** The develop-gate must key on the numbered Cast OPTIONS, not the hand. A
   creature in hand but NOT in the options is not castable this turn; the fetch that fixes its
   color IS the creature play (the setup step), not "fetching instead of a creature."
2. **Blue-blindness.** Point 3a leads "almost always the Snow-Covered ISLAND; you are short BLUE
   far more often than green." vs133/vs109 were short GREEN. De-bias: crack for the color your
   creatures need — Forest if short green, Island if short blue.
3. **Offered = payable / colored pays generic.** Name it at the seam: anything in your Cast
   options is payable now, and Astrolabe's {1} is paid by any mana including green.

## Q3 — FORCE OF NEGATION: new misfire shape (discard) + face-burn watch clean.

Four fires this wave. Face-burn over-fire: **0** (watch holds). But a NEW non-qualifying target
appeared:

| game | seq | target on stack | verdict |
|------|-----|-----------------|---------|
| vs44 | s49 | Go for the Throat -> Abominable Treefolk (the 9/9 closer, race at 9v6) | **CORRECT** (kills your only real finisher) |
| vs44 | s26 | Go for the Throat -> Ice-Fang Coatl (race, protecting the engine creature) | DEFENSIBLE (free on their turn, tight race won to opp 2) |
| vs140| s77 | Rakdos's Return (big-X burn-to-face + discard your whole hand), behind 13v36 | DEFENSIBLE (not a "single small burn"; potential-lethal + hand strip) |
| vs133| s13 | **Inquisition of Kozilek (targeted discard)** | **MISFIRE** — Force costs 2 cards (Force + exiled Astrolabe) to stop a 1-card discard = card-negative; not a board-wipe / mass-bounce / kill-only-threat |

The vs133 misfire is the SAME card-economy error as the face-burn ban: Force's 2-for-1 cost is
only worth paying against a board wipe, a board-clearing bounce, or removal of your only threat —
NOT against 1-for-1 hand disruption. The guide's "spend it ONLY on [list]" did not explicitly
exclude discard, and the pilot burned Force+Astrolabe on a T3 Inquisition in a grind it then lost.
**Guide fix: add discard (Inquisition/Thoughtseize) to the NEVER-Force list, with the 2-for-1
rationale (identical to face-burn).** Free-reaction-card-cost watch CONTINUES — still single-seat
(deck135 only); no 2nd free-alt-cost deck in this corpus, so no promotion.

## Q4 — REPLY-PROTOCOL INTENT-COLLAPSE: recurred (2nd instance, cleanest repro yet). LAYER-ROUTED.

**vs131 s14 (T8):** mana {g}{u}{w}{w} (4 sources), Coatl {g}{u} in the Cast options. Reply head =
`4` (Cast nothing). The PLAN then reasons and self-corrects IN TEXT, verbatim:
> "a creature I can't cast yet (Ice-Fang Coatl needs {g}{u} but I have {g}{u}{w}{w} available,
> **wait, I *can* cast Ice-Fang Coatl! Let me re-evaluate.**..."

The head token committed `4` before the PLAN finished reasoning; the model corrected itself in the
same reply but was already locked. Cast Coatl one phase later (s18, MP2). ~1 phase tempo, no game
cost (won 17 to -4). This is the wave-8 vs131 s48 class — now a SECOND instance at this deck across
two waves, and a textbook demonstration ("wait, I *can* cast..."). Still a property of the
code-appended reply-protocol (number-first), NOT a guide or judgment defect. Route to the
reply-protocol layer; do NOT re-word the guide over it, do NOT score it against a guide rule. The
recurrence STRENGTHENS the case for the reply-protocol A/B (accept LAST bare number / move CHOICE
after the PLAN) — see notes.md + general-suggestions.md.

## Q5 — 135v133 ENGINE FIX: CONFIRMED. Astrolabe casts normally via its snow cost.

The previously-wedged matchup **completed to T11** (was a T2 livelock in wave-8). Arcum's
Astrolabe cast via its `alternative cost {1}` in EVERY game (vs133 s18; also vs140 x4, vs110 x2,
vs44 x2, vs109, vs131). No restriction-gated no-op, no cached-ask replay, no livelock. The
Astrolabe class is dead as promised in the brief.

## Q6 — FETCH RE-OFFER TAX: trend continues DOWN. Engine ledger continues.

Declined fetch-offer windows per game (a fetch-crack offered, not taken): vs140 **45** (grind),
vs44 24, vs133 22, vs109 16, vs110 15, vs131 6. Avg **21.3/game** all-in; **16.6/game** excluding
the vs140 grind — down from wave-8's ~27/game (and wave-7's ~62). Decline-suppression holds, no
regression, no wanted crack made impossible. The tax still inflates decision counts in the vs140
grind (45 windows) -> ENGINE LEDGER continues (notes.md); NOT guide-fixable.

## Q7 — combat seams / mulligan / phantom-lethal.

- **Combat clean.** No reflexive high-life chump, no missed must-block. vs44 attacked with the 9/9
  Treefolk to push opp to 2 (correct aggression). Boreal-Druid mana-carve-out held.
- **No phantom-lethal / board hallucination** (board-read anchor held, 0 recurrences).
- **Mulligan watch (vs110):** the pilot KEPT a blue-clog one-creature hand (Coatl the only
  creature; 3 fetches, 2 Astrolabe, Search) and died to Etched Affinity T7 without deploying a
  body. It is a KEEP by the current rule (2-4 lands incl. fetches + one creature). Marginal vs fast
  aggro; low-priority watch (construction-adjacent, one datum) — see notes.md.

## WAVE-8 LEDGER STATUS
CONFIRMED / HELD:
- Develop-imperative (Rule #1 point 3) — WORKED for the direct-fix case; wave-8 vs44 freeze class
  gone (vs44 flipped to near-win; vs131 to win). Freeze point 1/2.
- Force scoping — face-burn over-fire 0; board-wipe/mass-bounce/kill-only-threat fires correct.
- Fetch decline-suppression — held; tax ~27 -> ~17/game (comparable games).
- 135v133 livelock — FIXED, game completed; Astrolabe casts normally.

NEW / PERSISTING:
- **Fetch-crack-as-development not recognized (green-short / intermediate-step)** — vs133 (4 turns),
  vs140 T6 — sharpen Rule #1 point 3 (Q2).
- **Force on discard (Inquisition)** — vs133 s13 misfire — add discard to the NEVER list (Q3).
- Reply-protocol intent-collapse — vs131 s14, 2nd instance — reply-protocol layer (Q4).
- Manabase strands 2-color creatures / slow clock — standing construction flags.
- Held-fetch re-offer tax (vs140 45) — engine ledger.

## GAME -> FILE MAP (run = matchups-20260714-094942)
| jsonl epoch | opp | outcome | turns | final (me v opp) |
|-------------|-----|---------|-------|------------------|
| 1784042098 | vs140 | TIMEOUT | 23 | 13 v 36 |
| 1784042117 | vs110 | LOSS    | 7  | 0 v 20  |
| 1784045654 | vs109 | LOSS    | 9  | 0 v 20  |
| 1784046413 | vs133 | LOSS    | 11 | -1 v 27 (engine wedge FIXED — completed) |
| 1784046476 | vs44  | LOSS    | 16 | -4 v 2  (near-win) |
| 1784047950 | vs131 | WIN     | 10 | 17 v -4 |
