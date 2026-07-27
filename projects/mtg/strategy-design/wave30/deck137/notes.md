# deck137 -- development notes (engine/harness ledger) -- wave 30

Binary 6e168aa05, corpus matchups-20260726-181642.

## N-137-MARCH-XCONVOKE [STILL OPEN -- HIGH] payability-before-offer fix narrowed but did NOT close
The wave-30 fix (offer gated on payability + X-affordability annotation + X-announce) delivered
real progress:
  - FIRST multi-token March in corpus history: vs152 seq35-37 t12, model picks convoke March, the
    option carries `You can afford X up to 3 ... higher values are NOT offered`, seq36 `X = 3`
    fires, narration `March: hand->stack`, `Soldier (1/1): created -> battlefield` x3,
    `March: stack -> graveyard`. THREE soldiers. (w27 0 tokens, w28 0, w29 1 @X=1, w30 3 @X=3.)
  - Logged defers 8 -> 2. Re-pick churn 4-7x -> at most 2 consecutive picks.
RESIDUAL, UNCHANGED ROOT: the engine still OFFERS a convoke cast whose convoke payment it will not
complete when the X-affordability annotation is ABSENT. Same annotation-predictor as w29 (PRESENT
-> resolves; ABSENT -> fails). This wave the failure appears in BOTH forms:
  - DEFER (logged): vs152 t10 seq24 model picks convoke direct (only `[convoke, cast nothing]`
    offered; NO plain option, NO "Cast Card Normally" mode menu), correctly plans X=3, then the
    engine hands the X-announce/payment sub-step to the heuristic with opts=0/prompt="" -> defer
    x2 -> declined -> March back to hand (recast fine at t12). NOT the plain-cast mode-menu cascade.
  - SILENT NO-OP (UNLOGGED): vs139 t6 seq20/21 model picks convoke March (annotation absent),
    4 lands auto-tap into pool ({g}{w}{w}{w}) but cast never completes, picks again, still no-op;
    March stranded in hand all game. NO fallback record emitted.
NEW SUB-FINDING (methodological, HIGH value to the loop): the fallback/defer counter UNDERCOUNTS
convoke-resolution failure. A failed convoke can end as a logged defer OR an unlogged silent no-op;
only ARRIVAL-tracing (did a token/body appear in narration for each offered convoke?) detects the
silent ones. This corpus: 2 logged defers but 5 offered-but-unresolved convoke picks (~55%
offered-convoke resolution rate). Any future "defers -> 0" gate MUST be read as "offered convokes
that produce no arrival -> 0", verified by arrival-tracing, not by the fallback field.
FIX DIRECTION for the engine owner (unchanged, now with a tighter test):
  (a) gate the convoke OFFER on the SAME payability the annotation/resolution use -- do not offer
      a convoke variant whose convoke-X cannot reach X>=1 (March) or whose fixed cost cannot be
      paid (Loxodon); OR
  (b) make the deferred/no-op convoke resolve at the affordable X (incl. X=0 -> decline CLEANLY,
      not silent-tap-and-abandon that wastes the floated mana), and route the X-announce/payment
      sub-step back to the model (it already resolves cleanly when reached -- vs152 t12).
  Acceptance test for CLOSURE: across a corpus, EVERY offered Loxodon convoke arrives and EVERY
  offered March convoke either announces X>=1 and makes tokens OR is not offered -- zero silent
  no-ops (arrival-traced), zero defers. deck137 is the SOLE seat that exercises convoke -- keep it
  in pool as the only observation post until that test passes.
IMPACT: March is deck137's go-wide + lifelink STABILIZE payoff. When the annotation fires it now
works and contributes to a win (vs152). When it does not, the deck's biggest swing is silently
forfeited (vs139 March stranded). Highest-value remaining engine item for deck137's win-rate.

## N-137-LOXCONVOKE-SILENT-NOOP [STILL OPEN -- LOW/unified] recurs on an AFFORDABLE Loxodon
vs122 seq20 t6: model picks "Cast Venerated Loxodon with its convoke cost (4/4)", NO defer, NO
arrival; Loxodon recast PLAIN at t8 (seq27) and arrives. Affordability at t6: 3 untapped mana
sources + 4 convokable creatures >= {4}{W}=5 -- AFFORDABLE, yet silently declined. This is the
fixed-cost face of N-137-MARCH-XCONVOKE (offer-then-can't/won't-complete convoke). NOTE the shift
from w29: w29 characterized this as UNAFFORDABLE-offered-anyway; w30's instance looks AFFORDABLE by
hand-count and still no-op'd, which strengthens the case that the OFFER gate and the RESOLUTION
gate use different payability logic. Divergence from March is only the failure mode (fixed-cost ->
silent; X-spell -> silent OR logged defer). Option (a) above closes both. LOW severity (a silent
decline is nearly harmless; the model recovers via plain), recorded to unify the ledger.

## Corpus health at deck137 seat (clean, model side)
0 unparsed_reply, 0 empty_reply, 0 retracted_choice, 0 stale_echo, 0 priority fallback across all
6 logs. All logged fallbacks (2) = deferred_to_heuristic, both March convoke (vs152 t10). Blockers:
11 records, all short+legal, 0 illegal gang-piles (floor round-6 HELD). X-announce parsed cleanly
when it fired (vs152 `X = 3`). Tapped-tag reword `[tapped - cannot attack or block this turn]`
rendered 131x, read correctly every time (g4 steady). No loot/discard ask at this seat (g3 N/A).
Adventure bodies, land-identity tags, shockland ETB "pay 2 life" menus, instance #N handles,
convoke +1/+1 counter narration ("Counter added to Trostani", vs136) all render clean -- steady.

## HARNESS/CORPUS note (pool composition, not a deck137 defect)
deck137 3 kills (w29) -> 0 kills / 6-of-6 adjudicated (w30). Pool-driven: brief reports 12 adj at
cap pool-wide (grindiest corpus yet). deck137's wins were all life-ahead adjudications; not a
closing-speed regression at this seat. No latency tail issue at my seat (max single-decision
latency ~161s at vs152 seq35, within the -T 3000 budget; games ran to natural adjudication).

## GUIDE CHANGE THIS WAVE: NONE. strategy.txt is a BYTE-COPY of wave-29 (cmp-verified identical to
the live bin/Res/ai/baka/deck137_strategy.txt). The evidence does not demand a revision:
  - The X>=2-3 March affordability threshold is WORKING (model computed X=3 correctly, vs152 t12).
  - The one-attempt-per-card anti-re-pick clause is WORKING (churn tamed to <=2 picks).
  - The Loxodon "prefer PLAIN when you have the mana" nudge already steers off the convoke no-op
    (model recovered via plain in vs122). Softening "either cast works reliably" further would
    either encode the engine bug (forbidden) or over-steer; the existing prose is correct.
The remaining failures are ONE (partially-fixed) engine defect and ZERO guide/prompt defects.

## ROTATION (honest read, unsentimental both ways) -- DOES NOT ROTATE
The g1 FINAL exit gate is NOT met. The wave-29 NAMED EXIT PATH required ALL THREE of: defers -> 0;
affordable convoke March consistently makes tokens across a corpus; Loxodon silent-noops -> 0.
Wave-30 status: defers = 2 (not 0); March resolves 1/5 (not consistent -- 2 defers + 2 silent
no-ops); Loxodon silent-noop RECURRED (vs122). ZERO of three conditions met. The convoke arc is
NOT closed.
Rotation is UNSENTIMENTAL -- and the unsentimental read here is KEEP, for two evidence reasons:
  1. deck137 is the SOLE seat exercising convoke; rotating blinds the loop to whether the last
     fix lands. The residual is a HIGH engine item observable nowhere else.
  2. This wave CORRECTS the premise the rotation was staged on. The orchestrator brief expected
     "2 defers" as the whole residual; arrival-tracing revealed 2 defers + 3 UNLOGGED silent
     no-ops. Rotating on the fallback-count alone would rotate on a false-clean signal. The
     honest gate is arrival-traced offered-convoke resolution, and it sits at ~55%.
NAMED EXIT PATH (carried, sharpened): rotate with finality only once, across a corpus, EVERY
offered convoke produces an arrival (arrival-traced, not fallback-counted) -- every Loxodon
convoke arrives, every March convoke either announces X>=1 + makes tokens or is not offered,
ZERO silent no-ops, ZERO defers. At that point deck137's new-work well is exhausted (comprehension
long converged, closing-speed resolves to CONSTRUCTION + pool, the sole engine item closed) and it
is a clean rotation candidate on the next convoke-clean corpus -- apply with finality, do not
re-hold for sentiment. deck137 has been in-pool since wave-22 (longest-serving current seat); the
ONLY thing holding it is this one HIGH engine item, and it is closer than ever but not yet closed.
