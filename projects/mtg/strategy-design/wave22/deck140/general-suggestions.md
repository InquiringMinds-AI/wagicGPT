# General-strategy (core-prompt) suggestions — deck140 wave 22

Routing: core (general prompt) proposals only. Deck-specific -> strategy.txt; engine/harness ->
notes.md; method -> skill.md.

## PROPOSAL (NEW, one game, but a clean loss-cause + generalizes across the pool) — a DETRIMENTAL-
SELF-CHOICE framing header for forced discard / sacrifice, analogous to the shipped reveal-framing
header.

Problem: the forced-self-discard seam renders as the GENERIC target template:
`TARGET CHOICE for discard a card (this spell/ability is already on the stack and needs a target -
it is NOT a cast or phase step). Pick the ONE target it will affect from the list below, and answer
with the chosen TARGET's name`. That template is calibrated for choosing what a spell AFFECTS —
normally an OPPONENT'S permanent you want removed, where "the one it will affect" = "the most
valuable target to hit." For a FORCED SELF-DISCARD/SACRIFICE the value logic INVERTS: the "target" is
YOUR OWN card that you LOSE, so you want the LEAST valuable, not the most. The template gives no cue to
invert. deck140 (vs102, Archon of Cruelty) discarded Damnation then Pyroclasm — its two live sweepers —
into a go-wide board and died two turns later.

Why core, not just per-deck: any deck facing a discard/sacrifice/edict-on-self effect hits the same
inverted-value trap; it is not deck140-specific (it is only ACUTE for deck140 because its whole game
plan lives in a few high-impact cards). This is the same class the wave-21 reveal-framing header
solved for a different seam (whose-hand + pick-EXACTLY-ONE); the fix shape is identical: a short header
that names the CHOICE'S DIRECTION.

Suggested header shape (core prompt, fired whenever the pending effect is a detrimental self-choice —
discard, sacrifice, opponent chooses the loss):
> This effect makes YOU LOSE one of your own cards/permanents. You are choosing which of YOUR things
> to GIVE UP, not which to use - so pick the LEAST useful one; the rest you keep.

Verification if adopted (per the skill's option-line-annotation terminus): (i) mis-pitch count -> 0
(did the pilot ever discard/sacrifice its most valuable card when a clearly-worse one was offered),
and (ii) a reasoning-tax drop at the annotated windows vs un-annotated self-choice windows. Confirm the
header fires on the seam (grep the prompt for the header string on discard/sacrifice decisions) before
crediting it — this seam currently reuses the target template, so a naive add could miss it.

Convergence note: this only fired at one seat (deck102's Tergrid is the pool's discard engine), so it
is a SINGLE-SEAT proposal — hold for corroboration from any other reviewer whose seat faced discard/
sacrifice, or adopt as a low-risk header (it cannot harm a seat that never sees the seam). I flag it
because it was directly LOSS-CAUSING, not a cosmetic tax.

## PASS on everything else
The general prompt otherwise held at this seat: answer-first parsed all long replies (0 unparsed, 0
empty at my seat), the SWEEP/hold/finisher behavior needs no core change (it lives correctly in the
per-deck guide), and no core line was cited while misplaying. No other core proposal.
