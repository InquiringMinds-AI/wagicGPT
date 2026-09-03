# Owner rulings, 2026-09-03 (put to him directly; verbatim)

**D42 — Arena-style board grouping.** "i like this, but it should seperate out any unique copies,
for instance if they have summoning sickness, or if an equipment is attached, or if some are
tapped. only items with identical names and states should stack." -> lane G (frontend, human seat,
desktop + console): stack key = printed name + full observable state (tapped, summoning sick,
attachments/auras/equipment, counters, damage marked, face/flip state, +N/+N until-EOT effects,
blocking/attacking assignment, control changes) — ANY difference splits the group. Expand on focus;
d-pad walks groups, then members. Measure CardGui::Render per-frame before/after on a wide board.

**D43 — the game-log window in the model's prompt.** "measure." -> lane H builds the mechanism
behind a flag DEFAULT OFF (`WAGIC_GPT_LOGWINDOW`: `full` (today) / `kind` (D13 per-ask-kind budget:
land drop, discard, empty-stack pass see the last 2-3 turns; combat + casting keep everything) /
`N` (last N turns + one summary line)); then an A/B corpus at equal power (same pool, same seeds
where the harness allows, 21 games per arm minimum) scored on the seat reviewers' misplay counts,
fallback rate, per-decision inference, and win-rate band — the rare-event A/B power rule applies:
no single favourable run is a result.

**D42 follow-up — targeting under grouping.** Owner's concern (verbatim): "i am unsure of how this
interacts with targeting, for instance in a spell like fireball, where you may want to spread its
damage across several grouped targets. arena, and older mtg games show this visually, with the stack
having a visual representation with lines drawn from effects to targets. wagic doesnt easily have an
equivalent for communicating with the user." Mechanism (Claude's proposal, given to lane G, not the
owner's position): targeting mode EXPANDS — any group with a legal target for the live chooser fans
out into clickable members and re-collapses on resolve/cancel; a click on a collapsed group during
targeting expands, never targets; reuse the existing target border + the stack's per-entry target
thumbnails (one thumbnail x N when all targets come from one group). Same rule for attacker/blocker
declaration and cost-target prompts. Owner has not ruled on the mechanism.
