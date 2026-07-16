# deck131 skill contribution — wave 13

The CURRENT skill is `strategy-design/wave11/strategy-writing-skill.md`. Per the epistemics rule, NO
single-seat evidence proposes a new method line; everything below CONFIRMS an existing rung or is a
reviewer-method note. deck131's guide is FROZEN byte-identical (8th consecutive freeze). Two method
notes this wave, both sharpening existing rungs — no new method line.

## Confirmed (not new): the confirmed-fired doctrine caught BOTH engine wins at this seat, and index-wins-on-conflict works.

- **The wave-11 H1 fix is field-CONFIRMED, not just shipped.** Wave-11 routed "bare `BLOCKS: none`
  unparsed → baka blocks Guttersnipe into a fatal trade" as a HARNESS/PARSE fix (explicitly NOT a
  guide edit that emits `B1:none`). This wave the fix landed and the seat confirms it: `BLOCKS: none`
  / `BLOCKS: None` both parse clean (deck109 s7/s12). This is a positive instance of the layer-routing
  doctrine paying off — the parser was fixed, the guide never touched, and the natural decline reply
  now works. It vindicates "never re-word the guide to paper a parser bug."
- **Name-echo's index-wins-on-conflict is the right design and is doing real work.** 163/163 echo
  compliance, 0 wrong-remaps. Crucially, the wave-11 H3 class ("plan fixates on an unlisted card,
  emits a mis-mapped index") did NOT disappear — the model still mis-echoes unlisted cards (F3) — but
  the protocol converted it from an OUT-OF-RANGE unparse into a benign index-wins outcome. Method
  point (confirms, doesn't extend): a containment mechanism at the reply layer can absorb a
  persistent model-comprehension defect without the defect being "fixed" — so a reviewer must not
  read "0 unparsed conflicts" as "the model stopped hallucinating options." Score the ECHO content,
  not just the parse result, to keep the underlying class visible.

## Method SHARPENING (extends the wave-10/11 headline — obeyed-in-reasoning, leaks at a lower layer): the leak can also be a MENU-COMPLETENESS defect, and it can drive the model onto the forbidden play.

Wave-11 established that a guardrail can fire perfectly in reasoning yet leak at the parse layer
(bare `BLOCKS: none`). deck131 wave-13 adds the mirror at the REPRESENTATION layer, on the same
blocker seam:
- At vs109 s29 the NEVER-block-Guttersnipe absolute fired perfectly in reasoning, yet the engine
  blocked Guttersnipe — this time because the blocker LEGEND omitted a legal untapped blocker (the
  Elemental), leaving the model only `B1. Guttersnipe`. With the creature it WANTED to block with
  absent from the menu, the model forced B1 onto "the Elemental" and the engine executed Guttersnipe.
- **Reviewer-method sharpening: when a block-side (or attack-side) guardrail appears violated,
  cross-check the OPTION/BLOCKER LEGEND against the battlefield line before attributing the violation
  to the model.** A missing-legal-option menu defect can manifest as an apparent guardrail failure. In
  wave-11 the leak layer was the parser; here it is the menu itself. Both are below the guide; both
  require reading the reply's reasoning AND the engine's offered options AND the executed combat,
  never `chosen_text` alone.

## Method note (adherence, not method-new): a maximally-explicit guide line can still be MISAPPLIED by the model — and adding louder text is the sanctioned-against escalation.

The vs135 s1 mulligan misfire (F1) is the clearest case yet at this seat of a guide line that is
already maximally explicit ("full stop", names the YP counter-example, names the exact "hope to draw
a Mountain" trap) being MISCITED by the model ("the guide allows keeping hands with... payoffs even
without red" — it does not). This confirms an existing skill rung rather than adding one: when a
model misreads an already-explicit rule, the fix is NOT more guide prose (that is churn on a frozen
line and the sanctioned-against escalation); it routes to model-adherence WATCH. The reviewer signal
worth carrying: a single variance-REWARDED misfire (this hand won) is the easiest kind to over-react
to — the win must not launder the decision as correct, and one instance in 7 waves must not reopen a
frozen gate. Hold the freeze; watch for a 2nd instance.

## No core, no guide edit this wave.
Core stays PASS at this seat (no 2+-seat core candidate). Guide frozen byte-identical (8th). Every
finding routes below/around the guide (engine parse, engine representation, reply-protocol,
model-adherence) or to construction.
