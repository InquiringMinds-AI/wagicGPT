# deck14 (Deep Blue) — skill.md — proposals for the strategy-writing skill (wave-21)

Fourth appearance (revised-guide round 3). All items are guide-authoring METHOD; none touches core.
The value this wave: a two-round arc CLOSES — a leak that was routed to representation in wave-20 got its
engine fix shipped in the wave-21 batch, and this corpus is the confirmed-fired-and-obeyed demotion trigger.
It exercises the skill's demotion terminus on a REAL representation fix, and sharpens the displacement rung.

## PROPOSAL 1 (VALIDATES the demotion terminus on an OPTION-LINE-ANNOTATION shape end-to-end) — a
## present-and-disobeyed misconception that was HOISTED to an interim guide trap in wave-20 AND routed to
## representation is CLOSED in one round when the representation ships, and the corpus is the demotion signal.
Wave-20 (deck14 Proposal 2) established the "hoist a buried-passive-#1-leak into a named trap" move as a
legitimate turnaround diff, WHILE flagging the durable fix as representation. Wave-21 completes the arc: the
engine shipped BOTH surfaces — a CAST-option annotation (`[this cannot target the spell on the stack]`) and
a TARGET-menu header — and this corpus confirms them FIRED (26 cast windows) and OBEYED (25 declines; the 1
cast was a correct aura-fizzle bounce; the 1 menu picked the opponent's biggest threat, 0 self-bounces).
Per the skill's own terminus ("confirmed fired AND obeyed -> the interim GUIDE defense is now DOUBLE
COVERAGE and must be demoted"), the wave-21 diff is a DEMOTION of the hoisted trap, keeping the corrective
FACT + the shipped marker STRING + the never-self-bounce anchor, dropping the (1)/(2) procedure the engine
now carries at the decision point. METHOD CONFIRMED across the full lifecycle at one seat in two waves:
buried-passive line (w19) -> hoisted named trap (w20, interim) -> representation ships (w21 batch) ->
demote the trap (w21). The OPTION-LINE-ANNOTATION shape (skill's fourth representation shape) behaves like
the marker-string shape: verify by (i) misplay-count -> 0 (0 self-target casts, 0 self-bounces) and (ii) a
DECLINE-rate signal on the annotated cast (25/26 "Cast nothing" — the annotation converts an
answer-a-stack-spell impulse into a decline). Both signals present -> demote.

## PROPOSAL 2 (SHARPENS the displacement rung with a DRAW-DENOMINATOR score) — when a clock-quality rule's
## deployment RATE drops between corpora, score deployment against the DRAW denominator, not the game count,
## before reading the drop as a rule regression.
Wave-20 validated Rule #1B at "finisher deployed 4/6 games." Wave-21 shows 3/6 — a naive read is "the rule
weakened." But splitting by whether the finisher was DRAWN: it was in-hand in exactly 3 games, and deployed
in 3/3 of them (100%), each the turn it became castable. The 3 non-deployments never drew a finisher (pure
starvation). METHOD: a deployment-RATE metric for a named threat must be reported as deployed / DRAWN, not
deployed / games — a falling deployed/games with a steady-or-perfect deployed/drawn is DRAW VARIANCE, the
displacement distinction, not the rule failing. Corollary that makes it airtight: also confirm timing
against the MANA line (deck14 vs35 Mahamoti sat in hand from t1 but 6 mana wasn't up until t11 — a
"sat-on-the-flyer" false positive that the mana-availability check dissolves). This is the quantitative form
of the wave-20 displacement rung: split "not deployed because not drawn" from "not deployed while castable"
by an explicit denominator, and gate any "sat on it" claim on castability.

## PROPOSAL 3 (NEW rung, LOW) — a stale_echo can be a FORMAT-echo downgrade distinct from the three known
## causes (leak-direction / superstring / degenerate-label): the model echoes the SPELL name on a TARGET
## menu, where the header asked for the target name.
Wave-16 catalogued three stale_echo downgrade causes. deck14 vs27 s29 adds a fourth witness shape: on a
TARGET-CHOICE menu (no stack spell), the model wrote `CHOICE: 1 (Unsummon targeting Inkfathom Infiltrator)`
— echoing the SPELL ("Unsummon", in no option) ahead of the correct target (Inkfathom Infiltrator, which IS
option 1 and appears as a substring of the echo). Parser Mechanism-B'd it to stale_echo. This is neither a
hallucinated-absent card (the target IS present) nor a superstring nor a degenerate label — it is a
FORMAT-echo where the parenthetical leads with the spell verb. Routing: this is a PARSER-robustness item
(match the "targeting <OPTION>" tail, or honor the in-range index which also pointed at option 1), NOT a
guide or matcher-loosening fix — the guide/header already say "answer with the chosen TARGET's name (not the
spell)". Single-seat, 1 non-deciding firing -> WATCH; adopt into the scoring rung only if a second bounce/
steal/removal seat shows the "<spell> targeting <target>" echo on a target menu.

## PROPOSAL 4 (reinforces the closing-loop discipline) — when a prior wave routed TWO coupled leaks to the
## engine and BOTH fixes ship in one batch, verify EACH by its own shape-specific signal, and confirm the
## COUPLING dissolved.
deck14 routed E1 (bounce-on-stack) and E2 (truncation-before-line) to the engine in wave-20, noting they
were PARTIALLY COUPLED (E1's impossible-target confusion was one of E2's two truncation triggers). Wave-21
both fixes shipped (item f header/annotation; item b answer-first). Verified independently: E1 by the
marker-string-fired + decline-rate; E2 by coded-line-first on every p95 reply. AND the coupling dissolved —
the bounce-menu thrash that produced 3 of wave-20's truncation fallbacks now produces neither a fallback nor
a truncation (the model declines the cast or picks a clean target). METHOD: when coupled leaks get a joint
batch, score each fix by its own signal AND explicitly re-check the SHARED failure surface (here: bounce
menus, which were the intersection) to confirm the coupling term went to zero — a per-fix pass that skips
the intersection can miss a residual that only lives where both used to compound.

## Convergence summary
- VALIDATED (completes wave-20 P2): the hoist->representation->demote lifecycle closes in two waves; the
  option-line-annotation shape hits the demotion terminus via misplay->0 + decline-rate.
- SHARPENED (wave-20 displacement rung): score named-threat deployment as deployed/DRAWN with a mana-line
  castability gate, not deployed/games.
- NEW rung (P3, LOW/WATCH): FORMAT-echo (spell name on a target menu) is a fourth stale_echo cause,
  parser-owned.
- METHOD (P4): coupled-leak joint batches must be scored per-fix AND at the shared surface to confirm the
  coupling term dissolved.
