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
Owner correction (verbatim): "afaik, it doesnt, but since all tge cards maintain positioning, the
user remembers what they have targeted." -> two binding adjustments sent to lane G: expanded groups
FREEZE (order + slots) until every stack item referencing a member has resolved; and an explicit
board marker on every card that is a pending stack item's target (CardGui, before the art quad),
independent of the grouping option. The claim that a per-card target border already exists was
Claude's and unverified — the stack-entry target thumbnails exist (ActionStack.cpp), a board marker
was not found.

**Softlock escape (owner request, verbatim, 2026-09-03):** "We need a softlock exit to main menu
option, for capturing diagnostics from softlocks." -> lane T: (a) in-thread hang guard at the
engine's loop sites (per-tick time budget, throws a SoftlockAbort caught at the game-loop top ->
diagnostics dump -> main menu); (b) watchdog thread + pad chord for unguarded hangs (dump, then
forced exit — LiveArea on the console, the honest limit). Dump = turn/phase/active player,
interrupt holder, pending choice + chooser state, stack, action-layer abilities, last transcript
lines, last frame timing, the tripping loop site; file ux0:data/Wagic/softlock-<ts>.txt +
`#classification=softlock` on the transcript.
