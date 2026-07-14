# deck140 wave-8 — proposals for the strategy-writing skill

Context: wave-7 re-anchored deck140's hold-everything overshoot to a countable SWEEP TRIGGER (fire if
C>=1 and under pressure; hold if C==0) with three false beliefs killed inline, and asked the engine
for a surfaced creature count + tapped-untap tag. Wave-8 is the first corpus with BOTH shipped. Result:
the overshoot class went 8 -> 0. The lessons below are about what a CONFIRMED-FIRED representation +
a VALIDATED re-anchor license you to do next.

## P1 (headline) — the RE-ANCHOR-TO-A-COUNT fix has the SAME terminus a representation fix has: when the count SHIPS as representation, DEMOTE the inline false-belief kills it now duplicates
Wave-7's method headline was "kill the false-belief bridge inline OR re-anchor to a hard count." This
wave closes the loop: the count SHIPPED as first-class representation (`Opponent battlefield
(creatures: N)`) AND the tapped bridge got its own marker (`[tapped - untaps and can attack next
turn]`, 86 firings). The pilot then swept the exact three board types it froze on last wave (a tapped
attacker, an artifact+creature affinity board, a shroud creature). So the guide's inline tapped-bridge
and artifact-bridge kills are now DOUBLE COVERAGE — the representation perceives what the prose was
standing in for — and demote per the confirmed-fired-representation rule (keep the corrective FACT +
the marker STRING + a one-line nod; drop the paragraph). **Method rule:** a re-anchor-to-a-count fix
and a surfaced-count representation are the SAME fix at two layers; once the count is BOTH the guide's
trigger AND a printed marker, the inline belief-kills the count subsumes are demotable — but only the
ones the count actually SUBSUMES.

## P2 — split the demotion by WHICH false belief the representation actually carries; a targeting misconception is NOT carried by a creature COUNT
The count `(creatures: N)` dissolves "they're tapped -> no creatures" (tapped creatures are IN N, and
the tag says they untap) and "it's an artifact board -> hold" (artifacts don't change N). It does NOT
dissolve "shroud stops my sweeper" — shroud is a TARGETING misconception, and a creature COUNT says
nothing about whether your removal targets. So the demotion is NOT uniform: DELETE/compress the two
bridges the representation carries; KEEP (compressed) the one it does not. **Method rule:** before
demoting a block of belief-kills against a newly-shipped representation, check EACH belief against what
the representation literally surfaces — a count carries "how many," a tag carries "state," but neither
carries "how your spell interacts." Demote only the beliefs the representation's surface actually
answers; the rest stay as guide-only facts. (This is the confirmed-fired demotion rule refined to
per-belief granularity.)

## P3 — validate a RE-ANCHOR the same way you validate a representation fire: the DEFECT CLASS goes to zero, joined to castability
Wave-7 predicted the re-anchored trigger would drop held-sweeper-while-C>=1 toward 0. Validation =
run the exact metric (`Cast nothing` while `(creatures: N>=1)` and under pressure) AND join each hit to
castability + intra-turn context, because the raw metric over-counts. This wave produced 7 raw
candidates and ALL 7 were non-overshoot on inspection: 2 mana-locked (no payable sweeper), 3 were a
first priority pass in a turn that DID sweep, 2 were a lost position with no sweeper that kills the
threat. Reported naively, "7 holds with creatures present" would have looked like the overshoot
persisting. **Method rule (extends wave-7 P4 confirm-before-accuse):** a re-anchor is validated by its
defect class going to zero AFTER the castability/intra-turn join — never by the raw held-with-creatures
tally, which conflates mana-locks and same-turn sweeps with real holds.

## P4 — a validated re-anchor puts the seat in TURNAROUND mode; the correct next move is a constrained diff (demote + freeze), NOT new fire conditions
The deck's key metric turned around (8 -> 0) on the FIRST corpus of the re-anchor. That is turnaround
mode: the temptation is to also fix the residual losses (reach starvation, a snowball edge case) by
ADDING fire conditions, but every loss this wave is deck construction, and a new high-life fire
condition is exactly the widening that produced the wave-7 overshoot. **Method rule:** after a
re-anchor validates on its first corpus, treat the seat as turnaround — demote the now-double-covered
lines, freeze the obeyed ones, and hold any new permissive/fire condition to a WATCH unless a second
seat forces it. The snowball edge (a single evasive threat gaining a counter/turn while the trigger
correctly holds at high life) is a real opportunity miss but a single seat; legislating it re-opens
the pendulum. WATCH, not widen.

## No-change confirmations (existing method validated)
- Timeout triage caught two non-guide timeouts: vs135 (deck140 AHEAD 21-16, winning-but-slow, 0
  fallbacks) and vs131 (Blightsteel COUNTERED by Essence Scatter t44, no lethal at opp-33 = no closer).
  The `events` field made the counter machine-readable (`stack -> library`) — the resolved-vs-asserted
  check (wave-7 133 P1) applied to a countered finisher.
- Harness-defect sweep first: 0 fallbacks, 0 empty/broken replies, 0 desyncs — the wave-7 vs135
  fallback cascade did not recur, so no game needed infra exclusion for parse reasons (only latency).
- Reach-starvation residual routed to deck construction, not louder prose (vs109 swept ~a dozen times
  correctly and still lost) — residual-attribution method held.
- Rakdos hold, Staff face-ping, X-menu, Lightmine deploy all obeyed; demote-the-winner discipline kept
  them compressed/frozen, not re-shouted.

## Routing note for the synthesis agent
P1 and P2 are the strong, generalizable contributions: (P1) a re-anchor-to-a-count and a surfaced-count
representation are the same fix at two layers and share the demotion terminus; (P2) demote a
belief-kill block per-belief against what the representation SURFACES — a count carries "how many," not
"how your spell interacts," so a targeting misconception (shroud) survives the demotion a count triggers.
P3 (validate a re-anchor by the defect class -> 0 after the castability join) and P4 (a validated
re-anchor = turnaround mode, constrained diff not new fire conditions) refine existing bullets
(confirm-before-accuse; turnaround-preservation). None require a core-prompt edit (general-suggestions.md
— PASS on core; the two wave-7 mechanics-fact WATCHES advance, see there).
