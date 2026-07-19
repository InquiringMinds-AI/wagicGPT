# deck35 -- core-prompt (general-strategy) proposals -- layer-routed

Single-seat; for the synthesis agent to weigh against convergence. Both are deck-agnostic
reasoning-discipline items surfaced by deck35 but NOT sliver-specific.

## G1 [CORE-PROMPT candidate -- commit instantly on TRIVIAL / OUTCOME-IRRELEVANT choices; do not oscillate]

Both of deck35's fallbacks this corpus were **trivial basic-LAND-DROP `ask` decisions** where the
options are outcome-equivalent this turn (Play Mountain vs Play Forest vs Play no land, when the
model has no blue source either way). The model oscillated -- vs135 seq3 ran 14,958 chars of
"I'll play a Forest. Actually a Mountain ... It doesn't matter. I'll play a Forest ..." and NEVER
emitted a committed CHOICE, timing out into an unparsed Baka fallback; seq6 stale-echoed on the same
land drop. This is the same self-poisoning shape wave-18's G2 flagged at the block seam (forced /
equivalent outcomes -> keeps re-deriving instead of answering), now at a different seam. A single
flat core rung -- "When your legal options lead to the same result this turn (e.g. a basic-land drop
that does not change what you can cast, or a block where you take the same damage no matter what),
pick ANY one and emit your choice immediately; do NOT keep re-checking a decision whose outcomes are
equivalent" -- is deck-agnostic and directly prevents both the block-lethal spiral AND the land-drop
spiral. Layer = core prompt (reasoning discipline). **Convergence flag: this now generalizes wave-18
G2 from one seam (forced-lethal blocks) to a CLASS (equivalent-outcome choices at any seam). If a
second seat shows an equivalent-outcome oscillation at ANY seam (land drop, mulligan, cast-nothing,
forced block), this is a strong core add -- the two seams at deck35 alone already argue for the
general form over the block-specific one.**

## G2 [CORE-PROMPT candidate -- mana/color reasoning should not paralyze an unrelated decision]

The seed of both deck35 fallbacks was a correct-but-unbounded worry: "I have three blue slivers and
no Island." That worry is legitimate, but it hijacked a decision (which basic land to play) that could
not resolve it, and the model would not stop restating it. A brief core rung -- "Noting a mana problem
is fine, but resolve the DECISION IN FRONT OF YOU; if the current choice cannot fix the mana problem,
make the best available move and move on rather than re-litigating the shortage" -- would keep a real
constraint from becoming an infinite loop. Lower priority than G1 (G1 subsumes most of its benefit),
but flagged because the color-screw-anxiety trigger is specific and recognizable, and any three-color
or splash deck in the pool can hit it. Layer = core prompt.
