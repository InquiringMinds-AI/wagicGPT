# deck105 — core (general prompt) proposals, wave 33

## VERDICT: PASS (28th consecutive). No core-prompt change proposed from this seat.

And, specifically: **G1 — the HELD alt-win clause — is DISCHARGED. Recommend DROP, not adopt.**

---

## G1 RE-CHECK (the brief's adopt-only-if-it-persists item)

**The claim under test.** `AIPlayerGPT.cpp:72` (was :63 at the wave-32 binary) still reads,
unchanged and present in every prompt of every duel:

> `"You are playing a duel of Magic: The Gathering. You win by reducing the opponent's life from
> 20 to 0."`

The sentence is still flatly false for this deck. The question the brief asked is not whether it
is false but whether the CONFLATION it caused survives now that poison is rendered.

**It does not.** Across all 123 deck105 decisions in this corpus:
- **ZERO** occurrences of the wave-32 conflation shape. The exemplar was `105v146 s23`:
  *"the opponent has 21 life. I need 20 poison. I have 11. I need 9 more."* Grepping this seat's
  replies for `20 poison | reduce (their|the opponent's) life to 0 | win by reducing`: **0 hits**
  at deck105's seat.
- **ZERO** poison/life reconciliation spirals. The wave-32 diagnostic
  (*"If they have 11 poison counters, they have already lost... then my count is wrong."*) does not
  recur; see findings.md §1.
- The pilot instead quotes the RENDERED threshold back correctly in 5 replies
  (*"bringing the opponent to 6/10"*, *"totaling 5 poison ... to reach 11/10 and win"*).

**The three "reduce the opponent's life to 0" statements that DO appear in these six games are all
at OPPONENT seats** (`146v105 s15/s17/s21/s22/s23`, `36v105 s29`) and are all TRUE for those
decks, which win by damage. Not one of them shows an opponent ignoring its own poison total: the
same opponents blocked 10/10 infect swings and quoted their poison lines while doing it.

**Conclusion.** N-105a was the carrier, exactly as the wave-32 self-hold predicted. The false core
sentence is inert once the alternate win condition has a rendered state variable. Widening the
core sentence would spend a core edit on a defect the render has already retired, and would add a
clause every non-alt-win duel pays for.

**Disposition: DROP G1 from the held list.** Replace it with a one-line entry in the intake check
that wave-32 HL1 rung 1 already installs: *"a pool win condition with no rendered state variable
is a P0 render item"* — the render is the fix, and the core sentence is a cosmetic residue that
should be corrected only if a core edit is being made for another reason. If it ever is, the
minimal correct form is *"You win by reducing the opponent's life to 0, or by any alternate win
condition your cards provide - the board prints the state of any that are live."*

---

## OWNER DIRECTIVE 1 — post-choice reasoning block eliminated. EVIDENCE ONLY, no counter-proposal.

Per the correction: no reorderings, brevity clauses or terminators are proposed. Three evidence
contributions from this seat.

### (a) Counted overrun / retraction fields

| field | deck105 seat (123 dec.) | both seats (240 dec.) |
|---|---|---|
| `post_plan_overrun` > 0 | 15 (12.2%) | 26 (10.8%) |
| mean `post_plan_overrun` (all decisions) | 225 chars | 156 chars |
| max `post_plan_overrun` | 7,651 (`105v146 s16`, 134 s) | 7,651 |
| `commit_retracted` == True | **0** | **0** |
| fatal retractions by the field's own definition | 0 | 0 |
| replies with no `PLAN:` line (compliant since f46dd58ee) | 1 | 3 |
| **post-answer text, mean chars per decision** (all text after the first answer label) | **1,804** | 1,650 |
| **post-answer text, total across the 6 games** | **221,905 chars** (~37k chars / ~10k tokens per game, this seat alone) | 394,864 chars |

The seat-level post-answer figure (1,804 chars/decision) reproduces the owner's corpus-wide
~1,900 independently.

### (b) Does the post-answer self-correction carry decision VALUE?

**At this seat, NO — it is net negative.** The measured rate is **5 / 123 (4.1%)** at deck105 and
**7 / 240 (2.9%)** overall, matching the corpus 4.2%. Broken down by value:

| repro | flip | value |
|---|---|---|
| `105v146 s15` blockers t8 | `BLOCKS: B1:A2, B2:A1` -> `BLOCKS: none` | **HARMFUL, and the game.** The first answer blocked with two 1/1s at 10 life against a 9-power swing. The scratch text then re-read the guide, mis-bound its threshold, and retracted to no-blocks. deck105 went to 1 life and lost on turn 10. |
| `105v146 s16` priority t9 | `CHOICE: 0 (pass)` -> `CHOICE: 1 (Regenerate with Blight Mamba)` | **HARMFUL and rules-wrong.** The FIRST answer carried the correct reasoning (*"Regenerating Blight Mamba now is unnecessary because it is not currently being destroyed"*). The 7,651-char scratch block talked itself into paying {1}{G} on its OWN upkeep to survive blocks on the opponent's next turn - a regeneration shield lasts until end of turn (CR 701.15), so the mana was wasted at 1 life. 134 s. |
| `105v36 s24`, `105v116 s5`, `105v152 s16` | see findings §5 | neutral (outcome-identical) |
| `116v105 s9`, `158v105 s11` | see findings §5 | mildly positive (a land choice, a removal target) |

So of 7 flips: **2 decision-relevant, both harmful, one fatal; 3 neutral; 2 mildly positive on
low-stakes choices.** Nothing in this seat's data suggests native pre-answer reasoning would need
to REPLACE a benefit — the post-answer block's only two consequential acts here were to overturn a
better first answer. That is consistent with the owner's diagnosis that the spirals are the model
executing the instructed pattern.

**One instrument caveat the wave-34 A/B will need:** `commit_retracted` did not detect ANY of the
7 (0/7), because all seven re-commit BEFORE the `PLAN:` line. If the A/B measures decision quality
by that field it will report no change by construction. Measure `first answer label != last answer
label` instead (skill.md P6).

### (c) Was the discarded scratch text load-bearing for THIS REVIEW?

**Yes, twice, and both times as evidence of a defect rather than of quality.** The review's two
sharpest findings are only legible because the scratch survived into the translog:

1. `105v146 s11/s12` — the verbatim discovery that the guide's own #1 rule is ambiguous
   (*"5 is not 9 or more. So according to the rule, we should block. But the example says we
   don't. This is a contradiction."*). Without that text the guide defect would have been a
   silent bad block, indistinguishable from a judgement call, and skill.md P2 would not exist.
2. `139v105 s18` — the verbatim board/log contradiction that root-caused N-105g (unstable instance
   suffixes). The pilot did the diagnosis; the review only had to confirm the mechanism in
   `instanceHandle()`.

This is exactly the split the owner's design already makes: the DEV LOOP needs this text and the
USER does not. The only thing this seat adds is that it must reach the TRANSLOG under the dev-loop
setting — capturing native reasoning into `reasoning_content` preserves both findings above; a
setting that discards reasoning entirely would have cost this review two of its five headline
items.

---

## OWNER DIRECTIVE 2 — dev-vs-user verbosity split

Supporting measurement only, no proposal. At deck105's seat the shipped-terse target would remove
**221,905 characters across 6 games** (~37k chars / ~10k output tokens per game at ONE seat) with
no observed decision cost, since the seat's `post_plan_overrun` text produced zero beneficial
consequential changes (see (b)). Latency corroborates: the seat's four longest replies
(11,912 / 11,414 / 11,089 / 8,424 chars) took 174 s / 187 s / 115 s / 130 s, and three of the four
reached the same answer their first line already carried.

---

## OWNER DIRECTIVE 3 (related docket) — consumed-decision narration decoration

Confirmed present at this seat and measured. Consumed decisions narrate with full decoration, e.g.
`chosen_text` values such as
`Cast Ichorclaw Myr {2} (1/1) {card text: "Infect -- Whenever Ichorclaw Myr becomes blocked, it
gets +2/+2 until end of turn."}` and
`Regenerate with Blight Mamba [cost: {1}{g}] {card text: "Infect -- {1}{G}: Regenerate Blight
Mamba."}`. Magnitude is small and matches the owner's ~320-char estimate; recorded as corroboration
for the wave-34 step-1 candidate, not proposed here.
