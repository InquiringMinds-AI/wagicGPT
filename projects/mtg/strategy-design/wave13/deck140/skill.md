# deck140 wave-13b — strategy-writing-skill proposals

Guide disposition: **FROZEN, byte-identical** (`cmp` clean, 7364 bytes). No decision-level failure CLASS
is model-fixable by wording. The SWEEP TRIGGER validated a 5th corpus (read correctly at every window),
the #1 Rakdos-hold and WHICH-SWEEPER/X/Staff-ping lines were obeyed where the MODEL controlled the
outcome, and every residual routes below the guide — to the name-echo reconcile arm, the engine, or deck
construction. The two proposals below are METHOD/REVIEWER refinements grounded in this wave; neither
touches guide prose.

## Proposal 1 [METHOD — grounded in Findings 3+4, RE-CONFIRMS the wave-11 promotable] The name-echo protocol did NOT retire the UNLISTED/UNAFFORDABLE-CARD FIXATION; verify the reconciler on the "echo names NO option" case, because index-wins-by-design is silently wrong there
Wave-11 flagged the reconcile-named-action arm as promotable (absent named action -> intent-consistent
best-available option, never an arbitrary index). Wave-13's name-echo shipment ("(name)" on every CHOICE,
index-wins on conflict) was the natural place to fix it — and it did NOT. The skill's composed-verification
lens should record WHY: name-echo only helps when the echoed name MATCHES a listed option (then the
reconcile arm can remap/confirm). When the model fixates on a card that is not offered (unaffordable, or a
land already played this turn), the echo matches nothing, the reconciler has no target, and index-wins is
the ONLY behavior — which at deck140 vs44 s40-42 dumped a held Rakdos (X=8) and a Wrath into C=0. Authoring
point for the ledger #3 arm (unchanged from wave-11, now HIGHER-impact + 2nd witness): the promotable path
is exactly the "named action absent from options" branch. A reviewer must NOT read 100% name-echo
compliance as protocol success — compliance measures "carries a name", not "the name matched or the intent
survived." The kill-shot dumps are invisible to a compliance grep; they surface only by reading the PLAN
against the option list at every "Cast X into a spent-land-drop turn" seam.

## Proposal 2 [REVIEWER-METHOD — grounded in Finding 4] Distinguish an engine X-underpayment from a name-echo X-menu mis-map before routing a "wasted Rakdos"
Two DIFFERENT layers produced a wasted Rakdos this corpus with a shared symptom (a non-lethal Rakdos when
lethal was available): (a) **name-echo mis-map at an X-menu that WAS surfaced** (vs44 s41: echo "Play
Badlands" -> index 1 = X=8) = reply-protocol; vs (b) **engine auto-resolved X with NO menu surfaced**
(vs109 s43: intended X=9, engine paid X=3, 6 mana left in pool, no X decision logged) = auto-tap/X-payment
engine defect. The tell: does an X-menu record exist as its own `ask` between the Rakdos cast and the
resolution? Present -> reply-protocol; absent + leftover pool -> engine underpayment. A symptom grep
("Rakdos fired non-lethal") conflates them and would misroute the engine bug to the A/B, which cannot fix
an unsurfaced/under-maximized X. This mirrors the wave-11 twin (trailing-mismatch vs phantom-board) and
sharpens the harness-sweep taxonomy for X spells.

## Nothing else — no demotions
The pause-aware demotion terminus requires OBSERVED OBEYED with a replacing surface. Nothing qualifies:
the SWEEP TRIGGER, tapped/artifact/shroud belief-kills, and the #1 Rakdos-hold are core mechanics read
correctly (not demotable guardrails); BSZ-X, Staff face-ping, Lightmine, Mulligan, Lands all obeyed where
exercised. No single-seat evidence cuts any method or core line. Core stays PASS.
