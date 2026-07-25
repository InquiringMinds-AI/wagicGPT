# notes.md - deck27 reviewer, wave 26 (engine/harness ledger + ROTATION VERDICT)

Layer-routing: engine/harness items only. Guide items -> strategy.txt (FROZEN this wave);
skill items -> skill.md. **No new engine or parser BUG found this corpus at this seat.** The
entries below are live-VALIDATION confirmations of already-shipped items (informational -
close, do not re-open), plus the rotation verdict.

## #1 (VALIDATED - close) - c4 parser hardening + b2 natural-stop: CLEAN, with the clean
## ASK-seam isolation wave-25 requested.

187 decisions, 0 fallbacks, 0 retries, 0 misfires. Three natural-stop CHOICE reversals at
the ASK/CHOICE seam (vs22 seq26, vs102 seq14, vs18 seq4) each led with a WRONG coded line,
reasoned to completion, reversed, ended with a PLAN line, and the engine applied the FINAL
line - all three correct. Because these are at the ASK seam (not blockers), the blocker
parser cannot co-produce the outcome, so this ISOLATES b2 from the parser (the exact caveat
wave-25 notes #1 left open). Two truncated replies (vs18 seq35 blockers, vs18 seq37
attackers - both in the already-lost vs18 game) correctly took the FIRST coded line
(truncated-first / ATTACK answer-first). No legitimate reply was dropped or mis-picked. See
findings FOCUS 2 and skill.md #1/#2.

## #2 (UNEXERCISED - carry) - wither-vs-first-strike annotation: no block seam re-tested it.

Oona's Gatewarden was cast once (vs102 seq3) but never offered as a blocker; no block option
this corpus contained a "wither"/"shrinks" annotation string. The wave-24 defect is already
CLOSED-BY-FACT (wave-25); this corpus adds no new data. Carry as unexercised at round 5;
nothing for the engine ledger.

## #3 - carried-PLAN staleness aggravator re-observed (informational, known ledger item).

vs18 seq35/seq37 both spiraled to truncation because a stale carried "YOUR PLAN" named
main-phase casts (Lord of the Undead, Marauding Knight) no longer legal in the attackers
phase, and the model thrashed reconciling the plan against the phase. This is the wave-24
plan-hygiene aggravator (a carried PLAN whose named action is not in the current legal set
seeds confusion). At this seat it cost LATENCY only - the first coded line was applied and
correct/forced both times. No new action; the item is already ledgered. If a between-wave
plan-hygiene pass is ever done, "drop/annotate carried-plan actions illegal in the current
phase" would reduce this specific spiral class.

## #4 - rules surfaces N/A at this seat (informational).

c1 (no targeted removal/no Oona targeted ability - zero target-preview lines), c2 (only
nonbasic is Underground Sea, an original dual - no ETB pay/tap), c3 (static anthem lords, no
activated pumps), c5 (no may-triggers of the Tergrid render class). Nothing for the engine
ledger from this seat re: batch C.

## ROTATION VERDICT - ROTATE OUT deck27 (CONVERGED; no new-work signal)

Rotation test = no-guide-mod AND no-new-work-signal. **deck27 satisfies BOTH this wave:**

- **No guide-mod:** strategy.txt is FROZEN byte-identical (cmp-verified against the deployed
  guide and the wave-25 revision). The guide covers every observed failure class; nothing
  this corpus was guide-addressable.
- **No new-work signal:** both losses (133 devotion, 18 go-wide anthem) are matchup /
  dead-anyway with no misplay; no engine or parser bug; no new decision-class; core PASS
  streak now 21. The only items are CLOSE/validated confirmations (c4/b2 clean, wither
  closed-by-fact, plan-staleness already ledgered).

The wave-25 notes pre-committed to exactly this: "If wave-26 shows the vs102 hybrid-bottom
fix held and surfaces no new misplay, ROTATE deck27." The ONE caveat is that the bottom-phase
rule-#1 clause was UNEXERCISED (zero mulligans all 6 games) - it neither validated nor failed.
Per the reframed termination doctrine (deck supply unbounded; termination gates on NEW-WORK
EXHAUSTION, not on validating every low-frequency edit), an unexercised low-frequency clause
does NOT justify holding the seat - it carries forward as an unvalidated guide line the way
the durdle off-case (now 7 corpora unexercised) does.

**Recommendation: ROTATE deck27 OUT.** The seat has been guided since early waves; both
wave-24 harmful classes (bottom commit-lock, first-strike lord-chump) are structurally gone;
b2/b5/c4 all validated positive; this corpus surfaced zero new work. If the synthesis agent
prefers to hold one more corpus purely to exercise the bottom clause, that is a defensible
minority call - but by the standing test the seat is a clean rotation/termination candidate,
and I score it CONVERGED.
