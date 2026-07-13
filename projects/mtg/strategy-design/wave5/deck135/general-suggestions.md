# Deck-135 wave-5 — proposals for the shared core prompt / general strategy

Layer-routed per the skill's handoff discipline. Burden of proof is on CORE: ship core
text only where the failure is deck-agnostic. My deck's evidence is single-seat, so I
propose NO core cuts and only one small COMPLETION; the rest routes PER-DECK or ENGINE.

## G1 (CORE — completion of an existing line, low burden). Order value-vs-threat at the cast seam.

The core prompt (system_prompt.txt) already says: "deploy a threat, use an engine, or
remove a creature pressuring you … Deploying outranks saving." It does NOT rank a
THREAT against a same-turn VALUE/RAMP/DIG spell — and a weak pilot reads "advance your
plan" as satisfied by casting Astrolabe/Search/a fetch.

PROPOSAL (append to the "Your own turn is for ACTING" bullet, deck-agnostic form):
"When your main phase offers BOTH a creature/threat and a card-draw, ramp, or search
spell, cast the THREAT this turn and spend leftover mana on the value spell — a body on
the board advances your plan; a dig only helps you find one."

WHY IT'S DECK-AGNOSTIC: every creature deck in the pool has cantrips/ramp/fetches, and
the durdle-on-value-over-body shape is the deck135 headline (opp ended 18–23 in every
loss). It is a COMPLETION of the existing deploy-outranks-saving clause, not new
doctrine. Flagged as single-seat-observed — synthesis should only ship it if another
reviewer's deck shows the same value-over-threat pattern (convergence gate).

## G2 (PER-DECK, NOT core). Utility-creature carve-out in the attack floor.

The core combat block's free case ("if the opponent has NO untapped creatures, attack
with every creature you control") is correct for real bodies but taps mana-dorks for a
1-point chip. This is DECK-DEPENDENT (only decks with utility creatures care) — it
belongs in each such deck's guide, NOT core. Do not add a core exception; a control
deck's every-creature-attacks floor is fine. Recorded here only to argue the routing.

## G3 (ENGINE/REPRESENTATION, not prompt). Fetch re-offer loop.

The single biggest deck-agnostic defect I saw is the fetch re-offer loop (see notes.md
E1): one crack consuming 3–5 model calls across phases. Instruction cannot fix a
re-offered legal action — this routes to the engine ledger, never to prompt text. Noted
here so synthesis does not attempt a prompt-layer mitigation.

## Nothing else proposed for core.
The interface contract, act-floor, plan/number binding, anti-deferral, and combat block
are already carrying their weight (13/13 attack-seam compliance, 0 desyncs, correct
blocks). No core line was observed LOSING at distance for this deck this wave that a
guide line can't cover, so no core reinforcement is warranted from my single seat.
