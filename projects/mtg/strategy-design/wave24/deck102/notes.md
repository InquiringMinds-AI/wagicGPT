# deck102 dev notes — wave 24 (engine/harness/model ledger + rotation verdict)

Corpus `matchups-20260724-125739`, binary `d7ff8b6c4`. Record 2/6 (W vs27, W-adj vs131, L vs59, L
vs109, L-adj vs137, DRAW-timeout vs133). 1 fallback at seat (s5). Quiet, correct corpus.

## VALIDATED / HELD this corpus

### Tergrid cast RULES-CORRECT (contrast wave-23's backface bug).
vs133: Tergrid drawn -> discarded -> recurred via Mortuary Mire -> redrawn -> cast the {3}{B}{B} GOD
FRONT FACE directly via "Cast Card Normally" (5 lands available: 4 Swamp + Mortuary Mire), resolved
as the 4/5 God, equipped Lightning Greaves. This is the correct path. The wave-23
R-DFC-BACKFACE-RESOLVES-FRONT bug (casting the {3}{B} Lantern back-face deploys the God front-face)
did NOT recur — but ONLY because the Lantern back-face was never cast this corpus (the model reached
the God's full mana). The bug is UN-RE-EXERCISED, not resolved. Keep it OPEN in the master ledger.
Probe path unchanged: a deck199 stacked to reliably reach a Lantern-affordable / God-unaffordable
turn and inspect the resolved permanent.

### Flip-thrash CLOSED / demotion HELD round 2.
0 Flip Side pseudo-actions consumed across all 6 games (wave-22: 11, wave-23: 1, wave-24: 0). The
DISPLAY-TOGGLE annotation renders in full. Even the s5 confusion (below), where the toggle was the
ONLY listed legal action, did not produce a consume. Demotion durable.

### Painland demotion HELD; Smallpox discipline HELD.
Ancient Tomb drawn (vs59 hand, vs109) — no self-death, priced correctly. One symmetric cast:
vs109 s4 Smallpox {b}{b} turn 2 L20/20 empty board — WELL-REASONED (forces opp to sac their only
creature + their only land Mountain while 102 loses one of two Swamps + a card). Disciplined, not a
leak; the gated-exception / case-(c) framing held.

## HARNESS / CORE item — s5 out-of-range hallucination (benign, precisely mechanized)

Repro: vs133 seq 5, kind=priority, turn 0, Main phase 1, L20/20. Only legal actions: 0 (pass), 1
(Flip Side DISPLAY TOGGLE on Tergrid). Nothing castable ({b} available; land already played). Model
line 1: `CHOICE: 2 (Cast Commander's Sphere)` -> OUT OF RANGE -> unparsed_reply -> heuristic PASSED
(correct). Reply completed (3556 chars); in prose the model reasons correctly to "choose 0 (pass)"
but never re-emits a coded line. NO game harm.
Two seeds (both in-record):
1. STALE PLAN NAMING AN UNAFFORDABLE ACTION. The carried `YOUR PLAN` said "cast Commander's Sphere"
   ({3}) with only {b} available — impossible when written, fed back as authoritative intent.
2. REPLY-PROTOCOL EXAMPLE INDEX. The protocol's `e.g. "CHOICE: 2 (Cast Fatal Push)"` was
   pattern-filled literally: `CHOICE: 2 (Cast <planned card>)`, index committed before legality
   check.
Candidate fixes (route to the engine/harness/core ledger, NOT the guide):
- (core, low-risk, only-if-a-2nd-seat-repeats) make the example index-agnostic — `CHOICE: <the
  number> (<action name>)` — and/or append "choose only a listed number; if you can afford nothing,
  CHOICE: 0 (pass)". See general-suggestions WATCH (19th PASS; not escalated on 1 benign instance).
- (harness plan-hygiene) do not carry a PLAN whose named action is not in the current legal/affordable
  set as authoritative intent — it seeds phantom casts.
- (representation) a priority window whose ONLY listed action is a display-toggle is confusing (the
  model asked "is this a partial list?"); consider an explicit "0. Pass priority (cast nothing)"
  LINE when the legal set contains no castable action, so pass is a listed choice rather than an
  implicit 0.
Classification: commit-lock family, THIRD sub-shape (example/stale-plan-seeded out-of-range, prose-
corrected, never re-coded). Isolated this corpus (1/205 seat, 1/corpus at this class).

## MODEL item — pregame mulligan hand-misread (vs59)

vs59 seq2: model mulliganed a KEEPABLE hand (4 lands incl. 2 Swamps + Sol-tier ramp rock Worn
Powerstone + Archon bomb + Necrogen Mists) while its reply claimed "zero basic Swamps." The prompt's
`Your hand:` line rendered both Swamps clearly (representation CORRECT) -> this is a MODEL
attention/miscount error, anchored to a stale PLAN ("mulligan to find at least one basic Swamp") that
the hand already satisfied. Consequence: over-mulliganed to a weak flooded 5 (kept 4 lands + a lone
1/1 after bottoming Sangromancer, the only payoff); lost at t10. Not guide-fixable (a model miscount,
not a knowledge gap). Reviewer-method response is in skill.md (the mulligan hand-read diff, extending
method-2 to the pregame hand). Watch for recurrence now that every game runs the pregame phase.

## HARNESS/THROUGHPUT — 102vs133 timeout (coordinate finding, half mine)

Genuine SLOW GRIND, not an engine/phase stall. Seat: 34 latency decisions, sum 706s, median 16.4s,
max 107.8s, 18 PRIORITY + 14 ask. Symmetric opp ~700s => ~1400s inference + pregame + re-asks => cap
at t10, DEAD EVEN 18/18 (neither ahead -> correct draw). Driver: mono-B attrition (102) vs mono-B
devotion (133) board stall -> many pass-priority windows, each a full ~16s model call (18 vs 6 in the
aggressive vs27 win). No single decision consumed the cap. Remedy = throughput (harness -j /
WAGIC_GPT_TIMEOUT), not play/engine. The 27vs137 timeout (also ~t10, even life) is very likely the
same class — coordinate with the deck27/deck137 seats for a unified "stalled-board priority-window
latency" throughput note.

## Pregame phase mechanics — SOUND at this seat
Bottom-N math correct (vs59 mulled 2 -> bottom 2; vs133 mulled 1 -> bottom 1; keeps bottom 0). No
double-asks, no wrong-N, no leyline cards (deck102 runs none). CURRENT-SITUATION hand line correct at
each mulligan ask (the "opening-hand narration = pre-mull deal" cosmetic caused no harm here). Game
start proceeded normally.

## STRATEGY EDITS APPLIED THIS WAVE: NONE.
strategy.txt is a byte-identical copy of the deployed guide (cmp-verified). Rationale: every tested
guide line HELD (flip-thrash gone, Tergrid cast correctly, Smallpox disciplined, painland priced),
the take-the-steal line got no test (0 steal windows), and the s5/mulligan items are model/harness
faults the guide cannot reliably fix. No corpus-evidenced guide gap.

## ROTATION VERDICT — NOT a candidate this wave (keystone validation debt), but the GUIDE has converged.

Honest application of the test (no-guide-mod AND no-new-work-signal):
- no-guide-mod: TRUE this wave (byte-copy; no edit warranted).
- no-new-work-signal: NOT cleanly met. Wave-23 set three conditions to rotate ("re-validate once
  more"): (a) DFC crutch demotion holds -> YES (0 flip-thrash); (b) R-DFC-BACKFACE-RESOLVES-FRONT
  resolved/benign -> NO, un-re-exercised (Lantern back-face never cast); (c) steal-take line lands
  cleanly -> NO, ZERO steal windows this corpus. Two of three are UNVALIDATED (not failed — the
  random pool never produced a steal window, even though Tergrid hit the board, because it landed at
  the timeout cap). The deck's CORE PAYOFF line (take-the-steal) is now unproven across two
  consecutive Tergrid-on-board waves. That is a legitimate new-work signal (validation debt), just
  not a guide EDIT.

Recommendation to force convergence: run a **deck199 PROBE** (per the seat's own method-5: a probe is
the guaranteed path) stacking Tergrid + an edict/discard package vs an opponent with sacrificeable
permanents, on a mana curve that reliably reaches a Lantern-affordable/God-unaffordable turn. ONE
controlled game validates BOTH open items at once: (1) the take-the-steal line firing on a real "put
on battlefield / don't" window, and (2) R-DFC-BACKFACE-RESOLVES-FRONT (does casting the Lantern
back-face still deploy the God?). If the steal-take line fires cleanly and the backface question
resolves, deck102 becomes a STRONG rotation candidate next wave — guide converged, keystone fully
validated. Until then: hold, run the probe.
