# deck137 -- development notes (engine/harness ledger) -- wave 29

Binary 50eeed8be, corpus matchups-20260726-115018.

## N-137-MARCH-XCONVOKE [PARTIALLY-CLOSED -- HIGH] f1 fix landed for affordable convoke; residual pinned
The f1 fix ({X} propagated onto the convoke shell + X-announce + X re-entry guard) WORKS for the
clean-affordable-direct-convoke case. FIRST-EVER March resolution in corpus history:
  1785098855-...-vs-deck139.jsonl seq27 t8 -- model picks "Cast March with its convoke cost {x}",
  seq28 "X = 1" (X-announce FIRES), narration `March: library->hand->stack`,
  `Soldier (1/1): created -> battlefield`, `March: stack -> graveyard`. 1 token. (w27: 0/17,
  w28: 0/16 tokens.)
RESIDUAL, now pinned (8 defers this corpus, all at deck137 seat: 7 vs148 + 1 vs93; same shape as
w28 -- deferred_to_heuristic, opts=0). The X-affordability annotation `You can afford X up to N` is
a perfect predictor: present ONLY in vs139 (resolved), absent in every defer game. So all defers =
convoke cannot afford X>=1. Two sub-shapes:
  (i) UNAFFORDABLE convoke offered anyway then defers (6/8): base {G}{W}{W} unpayable from untapped
      lands+creatures (vs148 seq10 t2 = 1 land + 1 creature = 2 mana vs 3 needed; seq34/39 t8 = no
      untapped mana sources). buildCastSpell (c5) offered an option not actually convoke-payable.
  (ii) PLAIN X-cast cascades into the convoke sub-path after mana committed (vs93 seq48-51 t12):
      plain `Cast March {g}{w}{w}{x}` with 7 untapped sources -> `Cast Card Normally / convoke`
      sub-menu -> convoke -> by then mana tapped (`no untapped sources`) -> defer. The w28 plain-
      route cascade is UNCHANGED.
FIX DIRECTION for the engine owner: either (a) don't OFFER a convoke cast whose convoke payment
can't complete the base (compute convoke-payability in buildCastSpell before offering), or (b) make
the deferred convoke payment resolve at the affordable X (incl. X=0 -> decline cleanly, not defer),
and stop the plain-route "Cast Card Normally -> convoke" detour from re-entering an unpayable path
after mana is spent. deck137 is the SOLE seat that exercises convoke -- keep it in pool as the only
observation post until defers -> 0.
IMPACT: March is deck137's go-wide + lifelink STABILIZE payoff. When affordable it now works
(vs139, fastest kill t10). When not, the model churns on it (vs148: 7 defers t2-t10, adj loss).
This is the single highest-value remaining engine item for deck137's win-rate.

## N-137-LOXCONVOKE-SILENT-NOOP [WATCH -> characterized -- LOW] recurs; unified with March root
1785084621-...-vs-deck148.jsonl seq27 t6: model picks "Cast Venerated Loxodon with its convoke cost
(4/4)", NO defer, NO arrival. Affordability: 2 untapped sources + 2 creatures = 4 mana vs {4}{W} =
5 needed -> UNAFFORDABLE. Same root as N-137-MARCH-XCONVOKE (engine offers a convoke cast whose
payment can't complete). Divergence is only in failure MODE: fixed-cost convoke -> silent no-op;
X-spell convoke -> logged defer. So this is NOT a distinct defect -- it is the fixed-cost face of
the offer-then-can't-pay-convoke gap. Fixing buildCastSpell convoke-payability (option (a) above)
would close both. LOW severity (a silent decline of an unaffordable option is nearly harmless);
recorded to unify the ledger, not as a separate work item.

## Corpus health at deck137 seat (clean)
0 unparsed_reply, 0 empty_reply, 0 retracted_choice, 0 stale_echo, 0 priority fallback across all
6 logs. All 8 fallbacks = deferred_to_heuristic, all = March convoke (N-137-MARCH-XCONVOKE).
Blockers: 7 records, all short+legal, 0 illegal gang-piles (floor round-5 HELD). The new X-announce
seam parsed cleanly when it fired (vs139 `X = 1`). c4 parser gate PASS. e4 land-identity tags,
adventure bodies, shockland ETB menus, instance #N handles all render clean -- steady state.

## GUIDE CHANGE THIS WAVE (applied to strategy.txt)
Sharpened the March bullet with an X>=2-3 affordability threshold, grounded in the card's own
`auto=this(X=0) donothing` (verified in bin/Res/sets/primitives/borderline.txt): X=0 March makes
ZERO Soldiers = wasted card; only cast when X>=2-3 is payable; otherwise deploy a creature. This is
variant-agnostic strategic advice (does NOT name the engine bug) that also steers the model off the
exact unaffordable picks that trigger the residual defer (the vs148 t2/t8 churn). Loxodon's "either
cast works reliably" softening from w28 is now VALIDATED (2 arrivals, 0 defers, both routes) and
kept unchanged -- no misleading residual there.

## ROTATION (honest read, with finality)
deck137 does NOT rotate THIS wave. The wave-28 NAMED EXIT PATH gate ("N-137-MARCH-XCONVOKE FIXED
AND validated -- March creates tokens, closing-speed losses resolve") is NOT met: March creates a
token now but only 1/7 episodes, 8 defers remain STILL CONVOKE-CLASS (fix INCOMPLETE), and
closing-speed is unchanged at 3/6 with the vs148 loss March-residual-driven. It remains the SOLE
convoke observation post. NAMED EXIT PATH (carried, sharpened): once the convoke offer-affordability
gap is closed AND validated here (defers -> 0, affordable convoke March consistently makes tokens
across a corpus, Loxodon silent-noops -> 0), deck137's new-work well is exhausted (comprehension
long converged + closing-speed resolves to CONSTRUCTION -- non-March go-wide/lifelink redundancy is
a deck-build lever). At THAT point it is a clean rotation candidate on the next convoke-clean
corpus -- apply with finality, do not re-hold for sentiment. deck137 has been in-pool since wave-22
(longest-serving current seat); the ONLY thing holding it is this one HIGH engine item observable
nowhere else.
