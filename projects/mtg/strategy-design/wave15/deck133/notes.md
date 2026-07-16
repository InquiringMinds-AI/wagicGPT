# deck133 wave-15 — development notes (engine / harness / model / deck-construction) + rotation verdict

Corpus: PRIMARY `matchups-20260716-040138`, binary `/tmp/wagic-aad32bc7f` (target-cancel
use-after-free fix + absent-echo staleness route `d0f88326b` + attackers name reconcile + Galvanic
Blast metalcraft `3cb901295`), Spark GPU_MEM_UTIL=0.75 (latency med 11.4s / p90 93s). Real games:
135(W), 44(W), 140(L-adj), 17(L-adj), 21(L), 110(L) — 2W/4L. Layer-routing enforced: engine/harness/
model/construction items live HERE with seq repros; the guide is a ZERO-edit frozen diff.

## CRASH-FIX VALIDATION (my primary charge) — CONFIRMED, downgrade/close the item.
The wave-14 segfault (this seat's Fatal-Push target-decline use-after-free) is FIXED by aad32bc7f and
held live: **0 segfaults corpus-wide**; the same self-target / heuristic-resolved Fatal-Push family
was exercised 11x at this seat and every instance resolved cleanly (see findings 1a). No
`cancelCurrentAction` anomaly observed. The exact wave-14 trigger (explicit DECLINE at the target
sub-menu) did not recur — the model reaches "Cast nothing" at the top-level menu or casts onto the
available target — but the neighboring freed-object paths (vs21 s23 self-target auto-resolve; vs21
s27/s28 heuristic-driven FP cast + fizzle) are the same family and are stable. **Recommend closing
the wave-14 crash item as fixed-and-live-validated.**

## ABSENT-ECHO STALENESS ROUTE (d0f88326b) — CONFIRMED, close the wave-13 ledger #A.
The wave-13 game-losing Mechanism-B class (echo names an absent card -> index-wins -> arbitrary cast)
is FIXED: vs44 s25 is the identical shape (`CHOICE: 1 (Cast Gray Merchant)` at a `[Put in Play /
Decline]` menu) and routed harmlessly to the heuristic; vs140 s11/s12/s36 likewise; a full scan found
**0 Mechanism-B index leaks** among executed decisions. The wave-13 ledger #A (name-echo reconcile:
echo-names-no-option -> heuristic) is delivered and validated. **Close it.**

## HARNESS / REPLY-PROTOCOL

### B. [NEW, LOW, non-deciding] Absent-echo route FALSE POSITIVE via name-SUPERSTRING mismatch.
**vs140 seq37 T12 my13/opp18** (deck133-vs-deck140 file). Options `[1] Attack with Yawgmoth / [2] Play
Verdant Catacombs / [3] Cast Fatal Push / [4] Pass`; reply is a lucid plan closing `CHOICE: 1 (Attack
with Yawgmoth, Thran Physician)`. The echo names offered option 1 — but as a SUPERSTRING (full card
name `Attack with Yawgmoth, Thran Physician` ⊇ the option's short `Attack with Yawgmoth`), so an
equality/substring key match fails and the reply was downgraded to `stale_echo` -> heuristic. This is
the mirror-image of the fixed leak: the route now sometimes OVER-fires on a legal, present intent.
- **Candidate fix (harness reconcile, LOW):** in the name-echo / absent-echo match, treat an offered
  option name as a match when it is a PREFIX or substring of the echoed name (the model echoes the
  FULL card name; options often carry the SHORT name). Symmetric with the parseChoice name reconcile.
- Cost here was nil-to-negative (the heuristic's decline-to-attack Yawgmoth was arguably better).
  Single seat; carry as a WATCH ledger item, not urgent.

### C. Fallbacks: 15 (4 stale_echo + 11 unparsed_reply), all choice=-1, heuristic played, none deciding.
empty_reply = 0 (Spark-death class stays CLOSED, 4th clean corpus). Verified no self-Fatal-Push and
no self-destroy resolved at any fallback window (vs21 s27/s28 heuristic FP fizzled harmlessly). Two
unparsed windows in WON vs44 were rescued by an excellent heuristic (seq27 cast GM opp-6/my+6; seq30
alpha strike opp->4).

### E. `resolved:` field still not emitted (reviewer-facing, carried wave-9..13). Low priority.
The `events` narration carried the deltas used to verify FP resolution/fizzle, GM resolution (x2),
and the self-target destroy. An explicit `resolved: battlefield|graveyard|fizzled|countered` would
remove the manual narration-scrape. Also NOTE (reviewer-facing): the `options` field is intermittently
serialized as an INT COUNT rather than the option list (e.g. vs21 s7/s23/s24); the offered options had
to be recovered from the `prompt` CHOICES block. If cheap, always serialize `options` as the list.

## MODEL / REPRESENTATION

### A. [NEW mechanism, MED, non-deciding] self-Fatal-Push via card-PRESENCE hallucination (NOT the wave-13 stack-target). Route: ENGINE/REPRESENTATION (primary) + MODEL.
vs21 seq23 (T8, my12/opp18). Hand = two Fatal Push only; Bloodghast already on the battlefield. Reply
reasons entirely about "casting Bloodghast" (hallucinating it as a hand card) then writes `CHOICE: 1
(Cast Fatal Push)`; index 1 = the only-own-target Fatal Push -> destroyed its own Bloodghast
(returned via landfall). The name-echo protocol cannot catch this (echo and index AGREE on Fatal
Push; it is the reasoning that diverged). **This is DISTINCT from the wave-13 self-FP stack-targeting
recurrence (notes F), which did NOT recur this corpus** — vs21 s7, the only FP-in-response-to-a-spell,
correctly targeted a real battlefield creature (Scuzzback Scrapper).
- **Candidate fix (ENGINE/REPRESENTATION, MED — the durable lever):** a targeted-removal top-level
  CAST whose ONLY legal targets are the pilot's own permanents is essentially never correct.
  SUPPRESS it from the offered cast list (or hard-deprioritize it below "Cast nothing"), the way the
  601.2c filter already suppresses mandatory-target spells with no legal target. The "only legal
  targets are YOUR OWN" annotation is present and MOSTLY obeyed (10/11 own-target FP windows this
  corpus correctly declined) — but one hallucination punched through it, and suppression makes the
  failure structurally impossible rather than instruction-dependent. Do NOT lengthen the guide for it
  (present-and-disobeyed -> sanctioned-against escalation).
- Related MODEL surface: the hallucination that a battlefield/recurring creature (Bloodghast) is a
  hand card. Bloodghast's recursion (graveyard->battlefield on landfall) plausibly primes "Bloodghast
  is always available" confusion. Carry as a model note; no prompt lever proposed.

### F. Wave-13 self-FP STACK-TARGETING — did NOT recur. Keep the wave-11 representation item parked.
The carried representation candidate (annotate WHY an on-stack creature-spell is excluded from a
removal's legal targets) had no repro this wave. Keep parked; do not escalate.

## ENGINE / REPRESENTATION — inherited items CONFIRMED STEADY
- **GM bare-N shown==resolved** (2 real casts vs44: drains4 -> my+4/opp-4, drains6 -> my+6/opp-6). No
  N->N+2 regression. The "+2" workaround stays DELETED.
- **Own-targets Fatal Push annotation** obeyed 10/11 (see #A).
- **Yawgmoth / Liliana / fetch coexistence** clean; no re-ask inflation, no no-legal-target loop.

## DECK CONSTRUCTION (upstream decklist flags — RE-CONFIRMED; do NOT fix with guide length)
- **No early blocker; Bloodghast cannot block.** vs110 T7 blowout (Cranial-Plating affinity: 4/2
  flying Ornithopter took deck133 my13->my1 in one turn, no wall, Fatal Push never drawn); vs21
  Goblin race (my15->-1). Carried wave-10 #10 / wave-11 #11 / wave-13. Consider a cheap early blocker.
- **No reach / no flyer answer except Gray Merchant.** vs17 (Faeries): dealt ZERO damage to opp all
  game (opp stayed 20); GM never drawn (density 4/60). Carried wave-9..13.
- **Self-damage suite (Thoughtseize 2, Phyrexian Arena 1/turn, fetch 1 each) is disproportionately
  costly under life-adjudication.** vs140 lost the position on self-inflicted bleed (my18->13) while
  the opponent sat at 18; vs17 bled my14->7 on Yawgmoth activations with no closer. A 1-copy
  Thoughtseize->Inquisition shift and cutting Phyrexian Arena would reduce self-damage in life-capped
  games. Construction, not a guide gap. Carried wave-10 #11 / wave-11 #12 / wave-13.

## ROTATION VERDICT — KEEP deck133 in rotation one more cycle.
Guide is a ZERO-edit frozen diff (10th+ consecutive freeze) — that half of the rotation test is met.
But the seat is NOT a clean rotation-out candidate: it surfaced NEW work signal this cycle —
(1) a NEW engine/representation lever (suppress own-target-only removal casts, #A), (2) a NEW harness
reconcile nit (absent-echo false-positive via name-superstring, #B), and (3) it is the PRIMARY
validation seat for the aad32bc7f crash fix and the d0f88326b absent-echo route, both of which it
confirmed live. Per the rotation rule (rotate out only when BOTH the guide takes no modification AND
the seat surfaces no new work signal), deck133 fails the second condition -> KEEP. Re-evaluate for
rotation next cycle once #A/#B are worked or a second seat corroborates them.
