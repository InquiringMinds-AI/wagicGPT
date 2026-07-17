# Deck-135 wave-18 — general suggestions (cross-deck / model / corpus items; NOT guide text)

Routed OFF the guide per layer-routing doctrine. deck135-observed but general in nature.

## #1 (MODEL, carried + MUTATED 4th corpus) — reflexive counter (Force of Negation) on a low-impact noncreature at a 2-for-1.

The weak pilot spends a free counter + a pitched card (2-for-1) to stop a noncreature spell that is
NOT worth two cards. Face-burn (the guide's verbatim NEVER line) did NOT recur this corpus, but the
class mutated to two NEW shapes, both clear violations of the guide's general Force whitelist ("spend
it ONLY on a wipe / mass-bounce that clears your board / a spell that would KILL your only threat"):
- vs62 s12: Forced **Primal Rage** (a trample-granting enchantment) — and the free "exile a blue
  card" pitch silently ate **Abominable Treefolk, the deck's only finisher** (the only blue card),
  while the model believed it was pitching a green Into the North.
- vs14 s14: Forced **Unsummon** (a bounce) on its own Icehide Golem — a bounce is not a kill (the
  Golem returns to hand, recastable for {1}); a 2-for-1 to prevent a temporary tempo loss on a 2/2.

Both were NON-FATAL (won both games). This is MODEL over-application of an already-maximal rule, NOT
a guide gap — prior waves correctly declined further Force restatements. Best fix is the incoming
OPTION-LINE WARNING ANNOTATION (owner ruling), which should (a) flag off-whitelist counter targets
(pump/anthem/trample enchantment, single-target bounce, face burn, discard = "let it resolve") and
(b) name/warn which card the free pitch will exile when it is the caster's only pitch-color card or
its finisher. See notes ENGINE #1d. Model-watch otherwise; do not add a 4th Force line to the guide.

## #2 (MODEL / DECODE, DOMINANT residual) — degenerate REPEAT-LOOP replies (decode-time repetition spirals).

Now the single most common failure shape at this seat: the model repeats one or two sentences
dozens-to-hundreds of times until the token cap truncates the reply, yielding `unparsed_reply`
(vs49 s22 ~100x, vs110 s19 ~60x, vs62 s31; plus vs35 s19 ~15x which parsed). Trigger: LOW-LIFE or
ambiguous-board spots on phase-restricted / binary / trigger menus where the model recognizes a
hopeless or confusing situation. NONE flipped a winnable game (2 in lost games, 1 post-adjudication).
NOT the PLAN-caveat stale-plan class (confirmed absent). Route to a model/decode-time mitigation
(repetition penalty and/or a max-token guard that emits the best-parsed CHOICE on truncation), not
the guide. A truncation-time fallback that scans the (looping) reply for the last well-formed
`CHOICE:`/`ATTACK:`/`BLOCKS:` line would salvage most of these — the model usually states a
tentative choice early in the spiral before looping.

## #3 (MODEL, carried) — card-KNOWLEDGE errors (color / fetch-target).

- **Into the North / pitch-color mis-ID:** vs62 s12 the model called Into the North "a blue card"
  (it is {1}{g}, GREEN) — the mistake that let the Force pitch eat Treefolk. Recurs from the wave-16
  family ("Into the North blue"; "Flooded Strand fetches Forest").
- **Windswept Heath cannot fetch Island:** vs49 the model repeatedly planned to fetch a Snow-Covered
  Island with Windswept Heath (Forest/Plains only), stranding its blue half for turns (long ramble
  at s7 before self-correcting; contributed to the blue-screw loss). A KEY-CARDS engine-text line on
  which fetch gets which colors could help, but this is model card-knowledge; watch, do not yet
  add. Route to model card-knowledge watch / few-shot.

## #4 (CORPUS, carried) — GPU-latency LIFE-adjudication as tiebreak.

vs110 was decided by adjudication (14 v 13, T8) — deck135 was AHEAD, so adjudication confirmed a win
it had earned (the control sequence had it ahead on board and life). Corpus-wide only 2
life-adjudicated (per brief). Not a deck135 item; brief owns. Noted for completeness.
