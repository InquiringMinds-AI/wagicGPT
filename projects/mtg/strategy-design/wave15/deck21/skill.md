# deck21 guideless-first-pass — skill deltas (wave-15)

Current skill = `strategy-design/wave13/strategy-writing-skill.md`. These are ADDITIONS to
the guideless-first-pass doctrine, drawn from writing deck21's initial guide. Each is
method, not deck prose.

## D1 - The reasoning-tax spikes ARE the saliency signal for a guideless deck.
A guided deck hides which facts are load-bearing (the guide already supplied them). A
GUIDELESS corpus surfaces them for free: every place qwen burned 800-3,000 words (or looped
into unparsed_reply) re-deriving something was a deck fact the core prompt does not carry.
Rank candidate teaching points by BOTH (a) did it cause a wrong/near-wrong action, and (b)
reasoning-tax magnitude. At deck21 the top tax spikes mapped 1:1 to the top teaching points:
Spark-Elemental end-step timing (2,773 words), summoning-sick token alpha-strike math (1,266),
burn-can't-kill-an-X/X snow loop (2,099 -> unparsed). Write the guide to PRE-ANSWER the
highest-tax derivations; a first guide's second job (after correctness) is short-circuiting
the loop, and the tax map tells you exactly where.

## D2 - A newcomer aggro guide's first job is often to COUNTERMAND a generic core line,
not just to add deck facts. The core prompt's "hold instant-speed interaction for the
opponent's turn" is CORRECT for control and ACTIVELY GAME-LOSING for a burn-as-clock deck -
qwen quoted it verbatim ("My strategy guide explicitly states...") and held its Lightning
Bolts into an empty board while a lifegain deck stabilized (deck21 vs140, the adjudicated
loss). deck109's frozen guide already encodes the same override ("the hold-instants prior
does not apply" to Searing Spear). Generalize: when a new archetype's core resource collides
with a generic core-prompt reflex, the guide must name and reverse the reflex explicitly for
this deck's cards - a bare deck-fact list will not stop the model from obeying the core line.

## D3 - Teach ENGINE reality, not Oracle memory. deck21's cards diverge from real MTG:
Scuzzback Scrapper is a `{R/G}` 1/1 wither with NO haste/persist here (real: {2}{R/G} 3/1
haste persist); Boartusk Liege is a 3/4 WITH trample (real: vanilla 4/4). A guide written
from MTG knowledge would misstate stats, costs, and the haste roster - and the haste roster
is load-bearing (which creatures swing the turn they land). Derive every stat/keyword from
the option/board annotation strings and the observed combat log, and cross-check the
haste/eligibility claim against a real attack window (Scuzzback named-but-dropped as an
attacker the turn it was cast = confirmation it has no haste, despite the real card).
Card-script divergences route to notes.md for Oracle verification; the guide uses what the
engine actually does.

No changes to the wave-13 method headlines (absent-echo staleness, attackers name->index
reconcile, game-log-resolution scoring, board-contradiction tax) - all held at this seat.
