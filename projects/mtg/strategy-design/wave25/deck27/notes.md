# notes.md — deck27 reviewer, wave 25 (engine/harness/deck-construction ledger + ROTATION VERDICT)

Layer-routing: engine/harness items only. Guide items are in strategy.txt; skill items in skill.md.
**No new engine or parser BUG found this corpus at this seat.** The two entries below are
live-VALIDATION confirmations of already-shipped batch items (informational — close, do not re-open),
plus a rotation verdict.

## #1 (VALIDATED — close) — b2 natural-stop precedence works and is game-positive (vs109 seq11)

The wave-25 b2 change (naturally-terminated reply takes the model's FINAL answer) was observed
RESCUING a lethal misread at the blockers seam: first line `BLOCKS: none` (death at 6 life) reversed
to `BLOCKS: B1:A2` (survival) in a natural-stop reply, and the engine applied the FINAL line
(choice=1); deck27 survived turn 7. This is the first live-positive of the wave-24 headline-1/headline-3
parser candidate. See findings FOCUS 2 and skill.md #1. Caveat: at the blockers seam the pre-existing
"`none`-is-not-an-assignment / last real line wins" parser behavior can co-produce this outcome, so
this seat cannot fully isolate b2 from the blocker parser. A cleaner isolation (a natural-stop reversal
at the ASK/CHOICE seam, where answer-first is the only prior floor) would confirm the mechanism
unambiguously — flag for whoever audits b2 corpus-wide.

## #2 (VALIDATED — close) — b5 wither-vs-first-strike annotation correct in both directions

The wave-24 false "(wither shrinks it to 0/0)" clause (rendered even when a first striker kills the
withering wall before it bites) is FIXED. This corpus: first-strike case omits the shrink note (vs109
seq9/seq11); non-first-strike case renders it correctly (vs137 seq12: Oona shrinks a 5/5 to 3/3). The
wither watch (reopened narrowly in wave-24) is CLOSED-BY-FACT. See findings FOCUS 2b.

## #3 — rules-fix batch A: no engine surface exercised at deck27's seat (informational)

deck27 has no upkeep-trigger cards (a1 N/A), no burn/damage spells (a4 redirect-menu N/A — confirmed
no redirect record exists at this seat), no creature-Battles / day-night / priority-at-untap-cleanup
(a2/a5/a6 N/A). a3 (combat roles clear at end of combat) produced no stale-attacker artifact — the
one near-miss (vs109 seq6, wondering about a present-but-not-attacking creature) is board-reading, not
a role-clearing bug. Nothing for the engine ledger from this seat re: batch A.

## #4 — latency regime (informational, not an engine item)

vs137 shifted from a wave-24 timeout-DRAW (even 12/12) to a wave-25 adj-LOSS (behind 10/16 at the cap),
and vs59 ran to the -T 3000 cap (LOSS 0/4). Same GW-Adventures/attrition decision-density latency sink
the wave-24 ledger already documented; the -T 3000 bump did not change the outcome class for these two
decision-dense matchups at this seat. No new engine/harness action — the known timeout-economics rule
(raise WAGIC_GPT_TIMEOUT or drop -j for decision-dense pools) already covers it.

## ROTATION VERDICT — DO NOT ROTATE THIS ROUND (marginal; converging; rotate NEXT round if the bottom
## fix validates and nothing new appears)

Rotation test = no-guide-mod AND no-new-work-signal. deck27 has a guide-mod this round (the vs102
bottom-phase rule-#1 clause + the lethal-block line), so by the standing test it is NOT a rotation
candidate this wave. But the signal is genuinely small and the seat is CONVERGING:

- Both wave-24 harmful classes (bottom commit-lock, first-strike lord-chump) are STRUCTURALLY GONE.
- b2 (natural-stop) and b5 (wither-FS) both VALIDATED POSITIVE at this seat — closed.
- The only new work is one guide clause carrying rule #1 into the bottom phase (a cross-phase mana
  fact, single-seat, low-harm — the game was a matchup loss regardless).
- No engine/parser/core work from this seat; core PASS streak at 20.

Honest call for the campaign endgame: after this bottom-phase clause validates, deck27's guide will
cover every observed failure class and the seat becomes a clean rotation/termination candidate. If
wave-26 shows the vs102 hybrid-bottom fix held and surfaces no new misplay, ROTATE deck27 (or, if the
guided roster is exhausted and no new-work signal exists anywhere, this seat is done — say so plainly).
This round: keep it in, ship the small guide revision, expect it to be the last substantive edit.
