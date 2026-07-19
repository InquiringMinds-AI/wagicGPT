# deck35 -- general Magic-reasoning / cross-deck suggestions (single-seat, non-core)

Layer-routed: these are general reasoning/representation observations, NOT deck35-specific guide lines
and NOT engine bugs (engine items are in notes.md). None can add/cut a skill core line.

## G1 [DECODE-TIME GUARD, cross-deck -- the residual reasoning tax is a REPETITION loop, route it to decode config, not prose]
The two longest replies this wave (vs14 seq19 14,078 chars on a land drop; vs62 seq7 12,809 chars on a
cast) are decode-repetition spirals: the tail degenerates into short repeated lines ("So I should play
the Island now. And cast Striking Sliver. But wait...") that still terminate in a valid CHOICE. They
did NOT fall back, but they are one unlucky decode from truncating unparsed. This is a MODEL/DECODE
pathology, not a guide gap -- a guide off-case cannot reach a decode loop (cross-seat evidence: the
model quotes correct guide lines mid-loop). Recommend a **decode-time repetition penalty and/or a
per-reply max-token guard** at the sampler, applied corpus-wide. This is the durable fix for the
tax-relocation dynamic (deck35 S2): suppressing indecision at one prompt seam moves it to another, but
a decode-level guard catches it at every seam at once.

## G2 [REPRESENTATION, positive -- the per-blocker trade tags continue to drive correct SELECTIVE blocking]
The block prompt's "you kill it, your blocker lives" / "you would be at M -- LETHAL/NOT lethal" tags
produced clean, cheap blocking decisions again: deck35 took the ONE free profitable block offered all
corpus (vs14 seq22, tagged "you kill it, your blocker lives") and correctly declined every non-lethal
non-profitable block ("no blockers" x6). Keep these tags; they are the durable cure for the block-math
re-derivation tax and they generalize to every combat-heavy seat. (Reconfirms wave-19 S3 end-to-end.)

## G3 [REPRESENTATION, watch -- summoning-sickness eligibility should be surfaced at the attack step, not just inferred]
The non-issuance bug (notes.md #1) is hard to reason about partly because the model can only INFER which
of its creatures are attack-eligible (unsick + untapped) from scattered `[tapped - untaps and can attack
next turn]` tags and cast-turn memory. When the attack step DOES fire, the options_text lists the
eligible attackers explicitly (good). Suggestion for the combat-prompt builder corpus-wide: when the
engine SKIPS the attack step, that is invisible to the seat; but when it fires, keep listing eligibility
plainly (it already does). No model-side change; flagged so the engine fix (notes #1) and the prompt
representation stay aligned once non-issuance is addressed.

## G4 [CONSTRUCTION-ADJACENT reasoning, cross-deck note for aggro seats]
For a creature-mana aggro deck, a HASTE enabler is doing double duty this corpus: it wins tempo AND it
sidesteps the attackers-non-issuance bug (Heart-present games attacked every turn; no-Heart games
skipped windows). Any future aggro seat that curves through summoning-sick bodies with no haste is
disproportionately exposed to the engine bug. This is not a guide line (single-seat, and the deck can't
choose to draw Heart), but it is a useful lens for the synthesis pass: "did this aggro seat have haste?"
partially predicts whether its 0/low-attacker games are engine-caused vs model-caused.
