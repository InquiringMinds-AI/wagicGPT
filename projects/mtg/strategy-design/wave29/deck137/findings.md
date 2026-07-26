# deck137 -- Selesnya (GW) Adventures -- wave 29 (guided ROUND 7) -- THE f1 EXIT GATE

Binary 50eeed8be, corpus matchups-20260726-115018, -T 3000, REPPENALTY=1.05.
My 6 pilot logs: 1785*-ai_baka_deck137-0x*-vs-{148,136,93,122,146,139}.jsonl.

Outcomes: **3/6, 0 timeouts, 0 draws** (SAME rate as wave-28's 3/6; different opponents).
- WIN  vs146 (KILL 22/-9  t14) -- Orzhov Dungeons; ground them out, board held.
- WIN  vs122 (KILL 4/-9   t14) -- Eternal Life lifegain; raced it, won at 4 life.
- WIN  vs139 (KILL 18/0   t10) -- Mutants/mutate; clean kill AND the ONE game March resolved.
- LOSS vs136 (KILLED 0/30 t15) -- mono-B Rankle/drain; ground out, board too small (0 March picks).
- LOSS vs93  (KILLED -24/4 t13)-- Relentless Rats; CLOSE (opp died to 4) but 137 died -24.
- LOSS vs148 (ADJ-behind 7/15 t10)-- Kor Army equipment; burned t2-t10 in a March re-pick loop.

Corpus health at my seat: **0 unparsed, 0 empty, 0 retracted_choice, 0 stale_echo, 0 priority
fallback.** All 8 corpus defers are AT THIS SEAT (7 vs148 + 1 vs93) and ALL 8 are **March of the
Multitudes convoke** defers. The corpus's entire 8-defer budget is deck137's March residual.

================================================================================
## (0) HEADLINE -- f1 VERDICT: **PARTIALLY VALIDATED. First-ever March resolution, but 8
## residual defers remain -- STILL CONVOKE-CLASS. The fix is REAL but INCOMPLETE.**
================================================================================
Applied the resolution-verification rung + the wave-28 trace-ALL-variants refinement: every
March pick traced to its downstream game-state effect (token creation), BOTH the convoke variant
AND the plain X-cast variant.

- **March made a token for the FIRST TIME in corpus history** (0/17 w27, 0/16 w28 -> 1 token
  w29, vs139). End-to-end proven (section 2). The {X}-propagation-onto-the-convoke-shell +
  X-announce fix WORKS for the clean-affordable-direct-convoke case.
- **But March still deferred 8 times (7 vs148 + 1 vs93), same shape as wave-28** (deferred_to_
  heuristic, opts=0, chosen_text="March of the Multitudes"). NOT a new decision class. So per the
  brief's test -- "if any defers are still convoke-class, the fix is incomplete" -- the fix is
  INCOMPLETE. deck137 does NOT rotate (section 7).

================================================================================
## (1) THE MARCH RESOLUTION TABLE (every cast episode) -- vs the 0/17 + 0/16 history
================================================================================

  game  | seq(t)         | ROUTE picked            | X announced? | payment? | tokens | board/mana at pick
  ------|----------------|-------------------------|--------------|----------|--------|-------------------------------
  vs148 | 10 (t2)        | convoke variant direct  | NO           | NO       | 0      | 1 land, 1 creature -- UNAFFORDABLE base
  vs148 | 30 (t6)        | convoke variant direct  | NO           | NO       | 0      | 2 untapped sources
  vs148 | 34 (t8)        | convoke variant direct  | NO           | NO       | 0      | NO untapped mana sources
  vs148 | 39 (t8)        | convoke variant direct  | NO           | NO       | 0      | NO untapped mana sources
  vs148 | 41-44 (t10)    | convoke (re-pick loop x4)| NO          | NO       | 0      | 5 sources -> tapped by loop end
  vs93  | 48-50 (t12)    | PLAIN X-cast -> convoke  | NO           | NO       | 0      | 7 sources on plain; 0 by convoke sub-step
  vs139 | 27-28 (t8)     | convoke variant direct  | **YES (X=1)**| **YES**  | **1**  | {g}{w}+2 sources+{w}{w} pool, 4 creatures
  ------|----------------|-------------------------|--------------|----------|--------|-------------------------------
  TOTAL: 7 cast episodes, 1 resolved, **1 token** (w27: 0/17 tokens; w28: 0/16 tokens)

Loxodon side (fixed-cost convoke -- steady):
  4 picks (3 convoke, 1 plain), **2 arrivals** (vs93 convoke, vs122 convoke), 0 defers,
  1 silent-noop (vs148 -- UNAFFORDABLE, see section 3). Fixed-cost convoke remains CLOSED.

================================================================================
## (2) THE END-TO-END RESOLVED CAST (brief-required) -- vs139 seq27-29 t8, LIFE 16/9
================================================================================
The model is offered the CONVOKE variant, chooses it, ANNOUNCES X, pays, and MAKES A TOKEN:
- seq27 t8: `CHOICE: 1 (Cast March of the Multitudes with its convoke cost {x})` -- convoke chosen
  directly. Board: 4 creatures, {g}{w} from 2 sources + {w}{w} already in pool. The option line
  carried the NEW X-affordability annotation (`You can afford X up to 1 ... higher values NOT ...`).
- seq28 t8: `CHOICE: X = 1` -- **the X-ANNOUNCE step fires** (this is the fix: X now propagates
  onto the convoke shell and the announcement is asked). Pool after: `{w}{w}{w}{w}`.
- narration seq29: `March of the Multitudes: library -> hand` ... `hand -> stack` ...
  `Soldier (1/1): created -> battlefield` ... `March of the Multitudes: stack -> graveyard`.
Trace complete: convoke pick -> X announced -> payment completed -> 1 Soldier token -> March to
graveyard. **This is the first time in three corpora that a March cast produced a token.**

================================================================================
## (3) DEFER DECOMPOSITION -- my share of the corpus's 8 defers (ALL 8 are mine)
================================================================================
Shape: IDENTICAL to wave-27/28 (deferred_to_heuristic, opts=0, latency -1, empty reply, chosen_
text "March of the Multitudes"). STILL CONVOKE-CLASS. But strictly NARROWER than wave-28, and the
mechanism is now pinned:

### 3a. THE PREDICTOR: the X-affordability annotation.
The option-line X-affordability annotation (`You can afford X up to N`) appears in EXACTLY ONE
game -- vs139 (10 occurrences) -- and that is the ONE game March resolved. It is ABSENT in every
defer game (vs148, vs93 = 0 occurrences each). Perfect correlation: **annotation present ->
March resolves via X-announce; annotation absent -> March defers.** The annotation is emitted only
when the convoke payment can actually reach X>=1. So all 8 defers correspond to cases where the
convoke could NOT afford X>=1.

### 3b. Two sub-shapes of the residual (both engine-bound, both convoke-class):
- (i) **Unaffordable convoke, offered anyway then defers** (vs148 t2/t6/t8 -- 6 of 8). The base
  {G}{W}{W} can't be paid from the untapped lands+creatures (t2: 1 land+1 creature = 2 mana vs 3
  needed; t8: no untapped mana sources). The engine OFFERS the convoke March, the model picks it,
  the payment can't complete, it defers. buildCastSpell (c5) offered an option not actually
  payable via convoke.
- (ii) **Plain X-cast cascades into the convoke sub-path after mana is committed** (vs93 t12 --
  1 of 8; also the tail of the vs148 t10 loop). Trace: model picks `Cast March {g}{w}{w}{x}`
  plain (seq48, 7 untapped sources -- plenty for a big X) -> engine offers `Cast Card Normally /
  convoke` sub-menu (seq49) -> model picks convoke -> by seq50 mana is already tapped
  (`no untapped sources`) -> defer (seq51). The plain route's own "would you like convoke?"
  detour lands back in the unaffordable-convoke defer. This is the wave-28 N-137-MARCH-XCONVOKE
  cascade, UNCHANGED for the plain route.

### 3c. Verdict on the shape question (brief): STILL CONVOKE-CLASS -> fix INCOMPLETE.
The 8 defers are not a different shape; they are the same convoke defer, now confined to the
can't-afford-X>=1 case (either genuinely unaffordable, or mana already committed by a plain
attempt). The affordable-direct-convoke case that used to defer now RESOLVES. So: real progress,
but the residual is the same class the e1/f1 fix targets. Not closed.

================================================================================
## (4) UNIFYING ENGINE CHARACTERIZATION (Loxodon silent-noop + March defer = one root)
================================================================================
The Loxodon silent-noop RECURS this corpus: vs148 seq27 t6, model picks `Cast Venerated Loxodon
with its convoke cost (4/4)`, NO defer, NO arrival. Affordability: 2 untapped sources + 2
creatures = 4 mana vs Loxodon's {4}{W} = 5 needed. UNAFFORDABLE. Same root as the March defers:
**the engine offers a convoke cast whose convoke payment cannot actually complete.** The two
mechanics diverge only in HOW they fail:
  - fixed-cost convoke unaffordable (Loxodon) -> SILENT no-op, no defer logged.
  - X-spell convoke unaffordable (March)      -> DEFER logged (deferred_to_heuristic, opts=0).
This is a refined, unified characterization of the residual -- NOT a new decision class. It is the
single engine item deck137 exists to observe. Routed to notes.md (N-137-MARCH-XCONVOKE updated to
PARTIALLY-CLOSED; N-137-LOXCONVOKE-SILENT-NOOP recurred + unified).

================================================================================
## (5) BLOCKERS FLOOR ROUND 5: HELD. 7 records, all legal, zero fallback, zero gang-pile.
================================================================================
7 blockers records (vs136 x3, vs93 x1, vs122 x1, vs146 x2; vs148/vs139 x0). All fb='', all
parsed, ZERO illegal gang-piles:
- vs93 s33 t9: `Venerated Loxodon blocks Relentless Rats` -- convoke-Loxodon body blocks (legal).
- vs122 s22 t7: `Lovestruck Beast blocks Soul War` -- 5/5 body blocks (legal, this game a WIN).
- vs136 s32/s37/s47, vs146 s23/s31: `no blockers` declines. vs146 declines are fine (a WIN, board
  ahead). vs136's 3 declines happen while being ground out 21->14->8->0 by mono-B drain with only
  2-3 creatures (mostly against a bigger/evasive board); no obviously-profitable block was passed
  up that the truncated prompt reveals -- this is a too-small-board loss, not a blocker misplay.
Floor: HELD, clean. Round-5 floor intact.

================================================================================
## (6) CLOSING-SPEED RE-VERDICT (with March now a PARTIALLY-working payoff)
================================================================================
3/6, THREE kills (vs146 t14, vs122 t14, vs139 t10). The ONE game March resolved (vs139) was also
the FASTEST kill (t10) -- suggestive but a single data point, not causal proof.

The losses' shapes:
- vs148 (adj-behind): the March re-pick loop is the mechanistic driver. The model spent t2->t10
  repeatedly picking an unaffordable convoke March (7 defers), burning tempo while Kor equipment
  aggro built. Own life 18->11->7 while stuck. This IS a March-residual-influenced loss.
- vs93 (close loss, opp to 4): 1 March defer at t12 (already losing); primarily a fast Rats race,
  not defer-driven. 137 nearly won (opp at 4).
- vs136 (blowout 0/30): NOT a March loss (0 March picks all game). Board too small vs mono-B
  drain; construction/matchup + draw variance.

**Re-verdict:** closing speed is UNCHANGED at 3/6 and remains ENGINE-BOUND on March -- but now
PARTIALLY. When March is affordable it resolves and contributes (vs139). When it is not, the model
still churns on it (vs148) -- a loss the residual defect + the model's rational re-picking produce
together. The guide lever this wave (an X>=2-3 affordability threshold for casting March, see
strategy.txt) attacks the model side of that churn without naming the bug; the engine side
(offer-then-can't-pay) stays ledgered. No attack/stabilize-doctrine tweak fixes an X=0 payoff that
makes 0 tokens.

================================================================================
## (7) ROTATION VERDICT: **DOES NOT ROTATE. Fix incomplete; sole observation post retained.**
================================================================================
The wave-28 NAMED EXIT PATH requires: "N-137-MARCH-XCONVOKE FIXED AND validated at this seat
(March creates tokens, closing-speed losses resolve)." Status against that gate:
  - March creates tokens: YES -- but ONCE (1/7 episodes). Not "validated."
  - Residual defers: 8, still convoke-class. Fix INCOMPLETE.
  - Closing-speed losses resolve: NO -- still 3/6; vs148 loss is March-residual-driven.
The finality condition is NOT met. deck137 KEEPS -- it is the SOLE seat exercising convoke and the
ONLY observation post for the (now pinned) offer-then-can't-pay-convoke defect. Rotating now would
blind the loop to whether the remaining fix lands.
NAMED EXIT PATH (carried, sharpened): once the convoke offer-affordability gap is closed AND
validated here (defers -> 0, affordable convoke March consistently makes tokens across a corpus,
Loxodon silent-noops -> 0), deck137's new-work well is exhausted (comprehension long converged +
closing-speed resolves to CONSTRUCTION) and it is a clean rotation candidate on the next
convoke-clean corpus. Apply that with finality when it hits -- do not re-hold for sentiment.

repetition_penalty=1.05 -- seventh-corpus verdict: SAME. Coherent across 12k-13k replies, no prose
harm, no spiral. Unchanged.
