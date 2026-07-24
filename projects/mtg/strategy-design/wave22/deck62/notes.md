# deck62 notes — engine/harness items + rotation verdict (wave 22)

Seat: deck62 (Enchantresses), SIXTH guided corpus (guide byte-frozen, round 6). 149 decisions (ask 114, attackers 25,
blockers 10; 0 reveal) across 6 games. Record 4/6 (W vs102,27,109,140[adj]; L vs44,135[adj]). **Fallbacks = 0** (was 4
in wave-21: 3 retracted_choice N9 over-fires + 1 stale_echo). Binary 1d8ed8194.

## The brief's three explicit checks

1. **N9 validation — PASS, strong-evidence form.** 0 fallbacks. The wave-21 FP shape (single first-line CHOICE +
   downstream "Wait,"/"Actually," self-correction in PLAN prose) recurs 27x this corpus and ALL parsed clean; 10 are
   the EXACT wave-21 FP context (`Play Forest`/`Hold Forest` land-drop menu): vs102 s19, vs140 s22/s32, vs27 s5/s11,
   vs44 s2/s3, vs135 s15, vs109 s5/s19. Verbatim witness vs27 s11 (`CHOICE: 1 (Play Forest)` first, multiple "Wait,"
   in PLAN body re-deriving mana/protection mechanics, parsed to `Play Forest`). This is shape-PRESENT-and-passing,
   the strong proof — distinguished from the weak shape-absent. The 2 remaining corpus retracted_choice fires are at
   deck102's seat, not here. **The seat that owned 3 of wave-21's 5 retracted_choice over-fires now produces the
   triggering shape 27x with zero fires — HARNESS-N9 (item b, reference-scoping) is VALIDATED at its witness seat.**
2. **Edit D round 2 + snap-then-revise — both confirmed.** 10 blocks, 0 fallbacks, 0 favorable-block spirals (edit D's
   spiral class dead across a 2nd corpus). Snap-then-revise still present and still rescued by last-wins, this corpus
   on a BLOCKS ask with real stakes: vs27 s16 snap `BLOCKS: B1:A2` (chump the 0/1 dork) -> reasoned `BLOCKS: none`
   (correct, not lethal), parser took `none`. No truncation-between-snap-and-correction (0 truncations).
3. **N8 gang-chump — still UNEXERCISED.** 0 block fallbacks; the heuristic block path never ran. Report null.

## Wave-22 engine-batch live validation (this seat's mandate)
- **(b) HARNESS-N9 retraction detector reference-scoped: VALIDATED at witness seat.** See check 1 above. The fix
  correctly no-fired on 27 same-shape replies (incl. 10 exact land-drop-shape); 0 FPs; deck62's 3 wave-21 over-fires
  did not recur. This closes deck62's harness-witness mandate.
- **answer-first + parser hardening: still holding.** 0 unparsed, 0 empty, 0 missing coded line across 149 decisions.
  Longest reply vs135 s24 (13,511 chars / 198.5s) parsed clean (CAST ask, led with CHOICE:1).
- **(h) baka blocker fallback (0-power skip / no 2nd-blocker pile): UNEXERCISED.** 0 block fallbacks. See N8.
- **Combat trade-outcome annotations: present and accurate** across all 10 blocks ("you kill it, your blocker lives"
  on vs102 s16; "(both die)" on vs135 s9; "your blocker dies, attacker lives" flagged on the 0-power chumps at vs135
  s5, vs109 s4/s18, vs27 s16).
- Items (a) fetchland, (c) echo-prefix/label-hijack, (d) transform-annotate, (e) painland — not applicable at this
  seat (mono-green, no fetches/transforms/painlands/echo). Nothing to validate here; owned by 135/140/102.

## Engine / harness / representation items (NOT guide prose)

- **N9 (HARNESS) — RESOLVED at this seat.** The wave-21 retracted_choice over-fire (which deck62 owned 3 of 5 of) is
  FIXED by the reference-scoping in binary 1d8ed8194. Validation: 27 same-shape replies (single CHOICE + downstream
  "Wait,"/"Actually,"), 10 in the exact land-drop FP context, ALL parsed clean; 0 retracted_choice at deck62 (down
  from 3). The genuine revise-and-replace case is preserved (vs27 s16 emits a SECOND coded line and last-wins took it).
  Cross-seat closure check belongs to deck102 (owns the 2 remaining corpus fires — verify they are real disavowals,
  not residual FPs). deck62's witness role is DISCHARGED.
- **N8 (ENGINE, MEDIUM, CARRIED — UNEXERCISED, 2nd corpus running) — baka blocker-fallback gang-chump.** Item-(h) fix
  code-shipped; 0 block fallbacks at deck62 again (all 10 blocks committed a first-line BLOCKS and parsed).
  Structurally, answer-first prevents block fallbacks at this seat, so N8 will likely NEVER validate at deck62. Keep
  OPEN; validate at a seat that actually produces a block fallback with a valuable 0-power body. If deck62 rotates,
  this is one more reason its N8 role cannot progress here.
- **N7-stale (HARNESS, LOW, wave-21) — did NOT recur.** 0 stale_echo, 0 reveals at deck62 this corpus (the wave-21
  Web-target spell-name-prefix leak had no occasion). Untested this corpus; cross-seat item, no deck62 signal.
- **N6 (model/decode watch) — persists as LATENCY, not parse harm.** Over-deliberation is undiminished (vs135 s24
  13.5k chars / 198.5s; vs27 s8/s17-19 5-8k CoT spirals) but answer-first keeps it parse-safe. One soft
  recount-of-printed-P/T instance (vs27 s19, combat-survival math, decision-harmless). Route to decode sampler
  (repetition_penalty is shipped-but-OFF); not guide-addressable. Latency p90 74.4s, max 198.5s.
- **snap-then-revise (cross-seat WATCH, from this seat) — reinforced.** vs27 s16 is a fresh BLOCKS-ask witness (snap
  worse chump -> reasoned `none`, rescued by last-wins). The truncation-between-snap-and-correction exposure did not
  bite (0 truncations). Cross-seat; closes only with a decode-time length/repetition guard. See general-suggestions #1.
- **Model rules-slip (carried, not owned) — illegal double-block declaration.** vs135 s22 declared one blocker against
  two attackers (`B2:A1, B2:A2` + `B3:A3, B3:A5`); engine resolved first-legal. Same class as wave-21 vs14 s21. Model
  error, engine-handled, not a fallback or guide issue. Low.

## Guide edit shipped this wave (round 6): NONE — BYTE-FREEZE
strategy.txt is a byte-identical copy of the deployed `Res/ai/baka/deck62_strategy.txt` (8893 bytes, `cmp` IDENTICAL).
Every taught class held; the N9 witness mandate discharged; all open items are HARNESS/ENGINE lane. No guide
contradiction, #1-band leak, or new model-fixable pathology surfaced to justify an edit.

## ROTATION VERDICT — ROTATE OUT (converged; named keep-reason DISCHARGED)

Apply the standing test honestly (rotation candidate = no-guide-mod AND no-new-work-signal):

- **Guide-mod this wave = NO** (byte-freeze, round 6; the guide has been frozen two consecutive rounds and edit D is
  validated across two corpora).
- **New-work-signal seat-specific = NO.** deck62's wave-21 hold was explicit and NAMED: "KEEP one more cycle
  explicitly as the N9-over-fire fix validator." Per the wave-21 skill rung — *"candidacy is triggered by the DISCHARGE
  of the specific NAMED keep-reason... when a NAMED keep-reason discharges and nothing seat-specific replaces it,
  rotate — do NOT manufacture a fresh reason to hold a converged seat."* **That named keep-reason has DISCHARGED this
  corpus:** the HARNESS-N9 reference-scoping fix shipped in 1d8ed8194 and this corpus validates it clean at deck62's
  seat (0 FPs on 27 same-shape replies, incl. 10 in the exact land-drop FP context; the seat's 3 wave-21 over-fires
  did not recur).
- **The residual open items are NOT deck62-specific and do NOT replace the keep-reason:**
  - N8 (unexercised) needs ANY block-fallback seat — deck62 structurally never produces one under answer-first, so
    holding deck62 cannot advance N8.
  - snap-then-revise truncation exposure is a cross-seat WATCH — deck62 gave it a fresh witness (vs27 s16), which is
    the point of a watch, not a reason to hold this seat.
  - N6 latency is decode-lane, cross-seat.

**Verdict: deck62 ROTATES OUT as a converged exemplar.** Its guide has converged (byte-frozen two rounds; every taught
class held) and its sole remaining hold — the N9 witness role — is discharged with the fix validated at this seat. The
guide stays DEPLOYED as a canary (like decks 14/35/49). No seat-specific new work remains that requires deck62 in the
active pool. Recommend rotating deck62 to the canary set next cycle and freeing its pool slot for a deck with live
guide-refinement signal.

## Engine items count: N9 (RESOLVED at seat, witness mandate discharged); N8 (blocker-fallback gang-chump, ENGINE
MEDIUM, CARRIED/unexercised — likely unvalidatable at this seat); N6 model/decode LATENCY watch (persists);
snap-then-revise cross-seat watch (reinforced, vs27 s16); double-block rules-slip (LOW, carried). Retired/no-signal:
N7-stale (no reveals/stale_echo this corpus). Closed: N9 over-fire at deck62.
