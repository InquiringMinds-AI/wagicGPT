# deck140 wave-7 — proposals for the strategy-writing skill

Context: wave-6 fixed deck140's early-Rakdos-waste by WIDENING the hold (cast-nothing case (a)
+ a "sweepers hit creatures only, hold vs artifact boards" corrective fact). Wave-7 the deck
collapsed 0-4+2T: the pilot now holds sweepers against boards that HAVE creatures. The mechanism
is a new, generalizable method lesson.

## P1 (headline) — OVERSHOOT VIA FALSE-BELIEF BRIDGE: a widened HOLD doesn't over-fire directly, the executor SATISFIES it through a pre-existing false belief
When you widen a permissive/hold condition ("hold when the opponent has NO creatures"), the weak
executor does not apply it only in the case you meant. It reaches the hold whenever it can make the
condition *read* as satisfied — and it has stable false beliefs that do exactly that. deck140's
"hold when NO creatures" was reached on boards with 2-3 creatures via THREE bridges, each quoted
from the pilot's PLAN and contradicted by the same record's board:
- "their creatures are TAPPED" -> "no (untapped) creatures" -> hold (fatal; tapped attackers untap);
- "their board is mostly ARTIFACTS" -> "artifact board, sweeper useless" -> hold (creatures present);
- "their creatures have SHROUD" -> "I can't remove them" -> hold (sweeps do not target; `auto=`
  confirms `all(creature)`, and Cruel Edict targets the PLAYER).
**Method rule:** after widening any hold/brake condition, enumerate the false beliefs the executor
already holds (mine the corpus replies) and ask which of them can make the executor read the widened
condition as MET when it is not. Kill each bridge inline as a flat FALSITY FACT, OR re-anchor the
condition to a HARD board-readable COUNT the belief cannot reinterpret ("count the (X/Y) tokens;
that number, tapped or not, IS the trigger"). This is the sibling of representation-beats-instruction
at the *condition* level: a permissive condition phrased in words the executor can reinterpret is a
condition it will reinterpret toward its dominant failure mode (here, passivity).

## P2 — a CORRECTIVE FACT over-generalizes exactly like a brake; scope it to a COUNT, not a board ARCHETYPE
Wave-6 shipped the true fact "sweepers do nothing to artifacts; hold vs an artifact/no-creature
board." The executor keyed on the ARCHETYPE ("artifact board") and dropped the "no-creature"
qualifier, so an affinity board (artifacts + creatures) read as "hold." **Method rule:** a corrective
fact scoped to a recognizable board archetype ("artifact board", "aggro board") will be applied to
every board that matches the archetype, ignoring the qualifier. Scope corrective facts to a COUNTABLE
the executor reads directly (C = number of (X/Y) tokens), never to an archetype it pattern-matches.
Same shape as "brake keyed to a visible count, not a judgment" — extended to corrective facts.

## P3 — middle-calibration after an overshoot is RE-ANCHOR, not swing-back
When wave N widened a hold and wave N+1 shows hold-everything, the correction is NOT to narrow the
hold back toward always-cast (that recreates wave N-1's waste — the pendulum). A HOLD decision has
two branches (hold vs fire) and EACH needs its own countable condition stated together: fire when
C>=1 and (life<=16 or attacked); hold only when C==0. Re-expressing the same decision as a
board-readable count with a numeric fire trigger excludes BOTH failure modes at once. (This is the
task's "calibrate to the middle" and the skill's conditional-payoff-needs-its-ELSE, applied to a
hold whose ELSE is a fire trigger, not a passive fallback.)

## P4 — do NOT read a raw HELD-count as overshoot without joining the board state
deck140 vs131 showed ~118 "sweepers offered / held" — the apparent smoking gun. Joined to the
opponent battlefield line, ~116 of those holds were CORRECT (the opponent was creatureless nearly
all game); the real overshoot was 8 decisions across two OTHER games where the opponent had
creatures. **Method rule (digest discipline):** when auditing a HOLD/pass tally, filter each held
decision by the deciding board fact (here: opponent creature count) before scoring — a raw
held-total conflates correct holds against an empty board with the fatal holds against a real
threat, and points the reviewer at the wrong game. Confirm-before-accuse, applied to a hold tally.

## No-change confirmations (existing method validated)
- Timeout triage caught two non-guide timeouts (vs131 ahead-but-slow + 112s latency; vs135 19/34
  fallbacks at 32s median = latency-starved) — both correctly excluded from play analysis.
- Harness-defect sweep first: the fallback cascade (vs135) and one malformed reply (vs133 t6) were
  separated from play before scoring.
- Reach-starvation residual routed to deck construction, not louder prose (vs109 swept 4x correctly
  and still lost) — the residual-attribution method held.
- Rakdos hold (wave-6 #1) is now obeyed; demote-the-winner discipline applied (kept compressed, not
  re-shouted).

## Routing note for the synthesis agent
P1 and P2 are the strong, generalizable contributions (the false-belief-bridge and archetype-vs-count
lessons apply to any widened permissive condition or board-scoped corrective fact). P3 and P4 are
refinements of existing bullets (middle-calibration of overshoot; digest discipline). None require a
core-prompt edit (see general-suggestions.md — PASS on core this wave).
