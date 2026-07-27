# deck158 — core (general) prompt: **PASS** (27th consecutive)

Corpus: `matchups-20260727-115847`, binary `7fabd9bd0`, 6 games, 120 decisions.

## Verdict

**PASS.** No core-prompt edit is proposed. Every leak traced at this seat routes to a layer below
or beside the core prompt, and two core rules were observed doing exactly their job.

### Core rules that FIRED correctly at this seat

- **The act/cast floor.** 33 casting windows with a real cast offered, **32 casts taken, 0
  declines** (the 33rd is a fallback, not a decline). Wave-31's four false-affordability declines
  at this same seat are gone — but note the credit belongs to the i12 RENDER, not to core prose:
  core already carried the corrective ("Never decline a listed action because you believe you
  cannot pay for it"; "any color can pay a generic cost") through the wave-31 corpus and it lost
  to a bad surface anyway. This is the cleanest confirmation yet of the perception-vs-strategy
  routing: **a correct core rule loses to a contradicting render and wins the moment the render is
  fixed.** Core needed no change in either direction.
- **"If a spell is not in the cast list you CANNOT cast it right now."** Quoted verbatim by the
  pilot at vs139 s16 to overrule its own (wrong) arithmetic that Grishnakh `{2}{R}` was payable
  off two sources. The engine was right; core's rule carried the pilot to the right conclusion
  against its own miscalculation. Load-bearing, keep as is.
- **Ping arithmetic** ("N damage targets a creature only when N >= its toughness, else face").
  Correct at vs152 s11 (1 damage, only a 2/2 available -> face). The one miss (vs105 s7, a 1/1
  infect creature ignored) is a DECK-GUIDE wording failure — the deck guide's own exception was
  buried in a trailing "unless" clause — and is fixed in strategy.txt, not here.
- **Plan-staleness notice.** Fired correctly at vsk36 s16 and vs139 s16
  (`"the actions your plan names are no longer among the options available right now"`), and the
  pilot re-derived from the board in both cases. See the flagged item below for its one side
  effect.

## One item FLAGGED, deliberately NOT proposed (needs cross-seat convergence)

**A COMMIT terminator on the reply protocol: "your answer ends at the PLAN line."**

Evidence, single seat, single instance: vs139 s16 produced a complete and CORRECT answer —
`CHOICE: 1 (Cast Mordor Muster {1}{b})`, reasoning, `PLAN:` — and then continued for ~10,000
further characters (`"Wait, looking at the opponent's board..."`, `"Actually, let's
re-evaluate..."`), never re-committed, and was correctly rejected as `retracted_choice`. A right
answer was thrown away and the heuristic answered at 2 life.

Why it is flagged rather than proposed:
1. **n = 1 at this seat**, and the core prompt is symmetric across seven decks — the bar for a
   core edit is convergence, not a single fatal instance.
2. The trigger is a **hopeless board state** (life 2, lethal incoming, two options). A terminator
   that stops the model re-examining a genuinely bad position may cost more elsewhere than it
   saves here.
3. There is a cheaper, reversible probe: the reply protocol already anchors the ANSWER first, so
   the question is only whether an explicit stop instruction changes retraction rate. That is
   measurable from existing translogs (count post-PLAN continuations, corpus-wide) before any
   prompt text is written.

**Recommended action for synthesis:** ask the other six seats to report, as a counted field,
`replies whose text continues past their own PLAN: line` and, of those, how many retracted. If two
or more seats show fatal post-PLAN retractions, this becomes a core proposal in wave 33 with a
measurement already in hand. See deck158 skill.md P6 for the classification rung that produces the
field.

## Items explicitly NOT routed to core

- **Poison/alt-win invisibility** (N-158j) — a render/engine omission. Core prose cannot describe
  a state variable that is not in the prompt.
- **Amass preview macro path-scope** (N-158m), **Feed the Swarm target-menu cost gap** (N-158k),
  **empty `[from ]` counter source** (N-158l) — all render-lane, all in notes.md.
- **Blocker label-form compliance** (the vs152 s35 `BLOCKS: Orc army: Sigarda...` unparse) — the
  protocol already specifies `Bn:An` and the render already listed the legal pairs; the failure was
  the model asserting a tapped creature as a blocker. Handled with a seam procedure in
  strategy.txt, plus a tolerance proposal at the harness layer (N-158n). Making the core protocol
  louder is the wrong lever for a perception error.
- **Cost-notation misread** (`{2}{R}` read as 2 mana) — deck-local, one line in strategy.txt. If a
  second seat reports it, it becomes a core candidate; this seat's instance did not change the
  outcome.
