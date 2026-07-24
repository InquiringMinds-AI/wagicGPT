# deck62 wave-22 — skill (method) contribution

One reusable instrument rung this wave; the rest is application of existing rungs (see PASS note at end).

## PROPOSED RUNG — Validate a FALSE-POSITIVE-SUPPRESSION fix on the SHAPE STILL BEING PRODUCED, not on its absence; shape-present-and-passing is STRONG evidence, shape-absent is WEAK.

When a prior wave logs a safety-route OVER-FIRE (a detector that discarded a correct answer) and the next batch ships
a scoping fix, the naive validation is "the fallback didn't recur — fixed." That is the WEAK form: a zero-fire count is
consistent with two very different worlds — (i) the fix works and correctly no-fires on the triggering shape, or (ii)
the triggering SHAPE simply didn't occur this corpus (the model happened not to produce it), in which case the detector
was never tested and you have learned nothing about the fix.

**The discriminator: find the TRIGGERING SHAPE still being produced this corpus, and show it now PARSES.** The rung has
two obligations:
1. **Characterize the shape mechanically** (not "the bug") so you can grep for it independently of any fallback flag.
   For HARNESS-N9 the shape was: exactly one first-line `CHOICE` + a self-correction phrase ("Wait,"/"Actually,"/
   "Correction:") appearing AFTER the CHOICE in the reasoning body, with no contradictory second coded line.
2. **Confirm the shape is PRESENT and PASSING.** deck62 produced it 27x this corpus (10 in the exact original FP
   context — a `Play Forest`/`Hold Forest` land-drop menu), and every one parsed clean (stated option == executed
   `chosen_text`, 0 fallbacks). That is the strong proof: the detector was RE-EXPOSED to its own trigger and correctly
   no-fired. Had the shape been absent entirely, the correct write-up is "fix UNTESTED at this seat (shape did not
   recur)" — explicitly the weaker verdict, never "validated."

**Corollary — distinguish the phrase from the token.** A suppression fix is validated by the CLASS of trigger, not one
literal string. deck62's wave-21 FPs used both "Wait," and "Correction:"; this corpus "Correction:" never appeared but
"Wait," recurred 27x. Validating only on the exact token that happened to appear in the prior corpus would have
under-tested the fix (or falsely reported the shape absent). Grep the shape's PHRASE FAMILY, not its historical token.

**Corollary — the WITNESS seat is the seat whose behavioral signature MATCHES the trigger.** deck62's documented
over-deliberation-of-a-settled-decision signature IS "self-correction-after-the-answer," which is exactly what the N9
route keys on — so deck62 is the predictable exposing seat and the ideal fix-validator (it reliably re-produces the
shape). When a suppression fix ships, route its validation to the seat whose signature reproduces the trigger, and hold
that seat ONLY until the fix is confirmed on the live shape — then rotate (see the discharge-of-named-keep-reason rung).

This EXTENDS wave-21's method-headline-1 ("score every new safety route for false positives, pick its witness by
matching the route's trigger to a seat's signature"). Headline-1 established finding the over-fire; this rung is the
back-half — how to VALIDATE the over-fire's FIX without being fooled by a coincidental shape-absence.

## Existing rungs re-confirmed (no new skill text needed)
- **Answer-first validation discipline (wave-21 headline 2):** re-point parse-risk to LATENCY once answer-first
  removes the parse-failure mode. Confirmed: vs135 s24 (13.5k chars / 198.5s) parsed clean; the >12k spiral is
  undiminished as latency (p90 74.4s), zero parse harm. CHECK-FIRST==FINAL confirmed (vs27 s16 snap `BLOCKS: B1:A2`
  worse -> reasoned `none` better, last-wins took it).
- **Crutch lifecycle / discharge-of-named-keep-reason (wave-21):** applied directly in the rotation verdict — a NAMED
  keep-reason (N9 witness) discharged, nothing seat-specific replaces it, so ROTATE; do not manufacture a fresh hold.
- **Score a chosen action by state delta, not by "the model answered" (wave-21 headline 4):** applied to the
  enchant-opponent-gift check — vs135 s25 confirmed via the actual target-record (own `Druid of the Cowl` picked from a
  mostly-opponent menu), not via prose intent.

## Core (general prompt): PASS
Every wave-22 finding at this seat routes BELOW core (guide is frozen) or to the harness/decode ledger (N9 resolved,
N8 unexercised, N6 latency, snap-then-revise watch). No general-prompt line is implicated. 17th consecutive PASS from
this seat's vantage.
