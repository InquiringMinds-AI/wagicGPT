# General suggestions — deck44, wave 13b

Core prompt: **PASS** (single-seat items cannot add a core line per the convergence gate). But this seat
surfaced a genuinely new, HIGH-severity, DECK-AGNOSTIC failure family that synthesis should weigh — it is
NOT a guide-prose issue and NOT deck44-specific, so it belongs to the cross-deck / engine surface.

## 1. (TOP, cross-deck) Reply-protocol reconcile: an ABSENT echo must not fall back to the raw index

The shipped name-echo protocol is 100%-compliant (152/152 CHOICE lines carry `(name)` at this seat) but
its "index wins on conflict" tie-break is the wrong default for the **absent-echo** case — when the
echoed name matches NO current option. That signals a STALE index (parent-action echo at a target
sub-menu, or a just-cast card after the option list shifted mid-turn), and index-wins then fires an
unwanted cast. At deck44 it caused a **self-destroy of the pilot's own creature via its own Go for the
Throat** (vs135 s24) and a **removal wasted against the model's own stated refusal** (vs133 s14→s15).
Any deck that (a) runs targeted removal with a target sub-menu, or (b) casts more than one spell per
turn (shifting indices), is exposed. Recommendation for synthesis: on an absent-echo, prefer the
Cast-nothing option / re-prompt, never the raw index (extends wave-11 skill headline 1 to the name-echo
layer). Cross-seat grep trigger: target sub-menus with a `[your battlefield]` `chosen_text`.

## 2. (cross-deck, representation) Forced-choice sub-menus need committing-effect framing + own-target danger annotation

Targeted-spell and forced-sacrifice sub-menus are bare creature lists with no header naming the spell/
effect that forces the choice and no cancel. The model repeatedly calls them "glitches" and re-litigates
(deck44 vs135 s24 self-destroy, vs133 s15 Bloodghast waste, vs133 s19 Liliana sacrifice misread). A
header ("Choose the target for Go for the Throat you are casting" / "Liliana forces a sacrifice — not
optional"), an own-vs-opponent annotation ("a `[your battlefield]` target destroys YOUR creature"), and
listing opponent targets first would defuse both the confusion (F2) and, partly, the self-destroy (F1).
Deck-agnostic; synthesis should grep other seats' sub-menu records.

## 3. (DEFENSIVE guide-line candidate — WATCH, do NOT adopt on single-seat)
If the engine/reconcile fix is slow, a pilot-reachable mitigation would be a guide line such as: "If a
bare list of creatures appears asking you to choose a TARGET or a SACRIFICE, it is a FORCED choice from a
spell/effect already committed — it is NOT a glitch and you cannot pass. For a removal target pick the
`[opponent's battlefield]` creature and NEVER a `[your battlefield]` one; for a forced sacrifice pick your
LEAST valuable body (a Surveilling Sprite replaces itself with a card)." Offered ONLY as a WATCH: it is
single-seat, and the surer fix is the reconcile arm + sub-menu representation above. Adopt only if a 2nd
seat witnesses the class.
