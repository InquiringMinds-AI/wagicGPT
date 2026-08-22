# deck36 — general/core suggestions (wave 35; core is 28-PASS, bar unchanged — these are NOT core edits)

1. **Outcome-line trust is now a two-seat pattern.** deck36 F4 s19 (declined a printed
   "(you kill it, your blocker lives)" free kill) and deck105's arm-A prior-override pair (brief
   per-seat item: "believe the printed outcome line") are the same defect: the pilot re-derives
   combat math over a computed annotation and loses to its own arithmetic. Wave-35 guide-side
   fixes land per-deck (deck36's revision adds the trust rule). If a third seat shows it under
   FP8, the sentence "the parenthetical after 'may block' is the computed outcome — believe it
   over your own math" is a candidate for the shared combat preamble rather than 7 guide copies.
   Not proposing a core edit now; flagging the convergence trigger.

2. **Instrument gap: silent engine fall-through.** F2 s29 shows a reply the parser could not use
   resolving to an engine-side pick with `fallback: None` — `chosen_text` matches nothing in the
   reply and no field says so. Any no-parse resolution should stamp a fallback/parse-status field;
   otherwise adherence audits under-count harness-caused divergences (HL11: the boundary the field
   measures is not the boundary the behaviour has).
