# Deck-140 wave-5 proposals for the strategy-writing skill

Two genuinely additive method points, both forced by this corpus. Everything else in the wave-4
skill held up and needs no change (the harness-defect sweep, the wins-as-mirrors-of-losses lens,
the brake/pursuit overshoot audit, and the stopgap-lifecycle triple all did exactly their jobs).

## P1 — A fixed-defect stopgap written against a SURFACE can INVERT, not just go inert. Re-read it against the NEW surface; the pilot ignoring it is the tell.

The wave-4 skill's stopgap lifecycle says a stopgap whose defect is engine-fixed is DELETED, and it
already names the X-menu case. What it does NOT warn: when the fix is a REVERSAL or RELABEL of the
same surface the stopgap was written for (menu order, option text, a board label), an un-deleted
stopgap does not merely waste attention — it can point at the **worst** option.

Concretely this wave: the engine reversed the X menu to largest-first, and the live guide's loud #1
rule still said "pick the LAST line" — which now names X = 0. The deck won 6/7 X decisions at max
**only because the pilot IGNORED the guide** and followed the engine's new ask text. A guide line the
pilot succeeds by disobeying is not harmless; it is one bad day from a blanked kill spell.

Proposed additions:
- In the Step-0 stopgap re-check ("how a wave-N+1 author knows which stopgaps are still needed"): for
  any stopgap written against a MENU ORDER, OPTION LABEL, or BOARD REPRESENTATION surface, don't just
  ask "is the defect fixed?" — pull one fresh prompt and confirm the stopgap's INSTRUCTION still
  matches the current surface. An engine fix that reverses/relabels the surface inverts a
  positionally-phrased stopgap ("pick the last line", "option 1 does nothing").
- Add a TELL to Step 0: **a guide line the new corpus shows the pilot SUCCEEDING WHILE DISOBEYING is
  a delete/flip signal, not a validation.** The wave-4 skill teaches "grep replies for echoes of the
  guide's phrasing when MISPLAYING" (a guide bug); the mirror is also true — a rule contradicted by
  the surface, where success correlates with ignoring it, is stale-or-inverted and must be flipped or
  cut. This is cheap to check with the offered-vs-taken table already required.
- Corollary for positional teaching: prefer stopgaps phrased against a STABLE property of the option
  ("pick the line with the biggest number"), not a POSITION ("last line") — a value-anchored verify
  survives a menu reversal; a position-anchored pick does not. (The wave-4 X-teaching pattern's
  step 4 "verify: the line reads X = <biggest number>" survived the reversal; its step 3 "reply with
  the LAST line" did not.)

## P2 — Add "menu-adjacency index carryover" as a distinct shape under the numeric-offset sweep.

The wave-4 numeric-offset sweep looks for chosen = intended - 1, or collapse-to-1. This corpus
produced a THIRD shape that looks identical in a chosen_text column but has a different cause and fix:
the model carries a **bare number from the immediately-preceding menu** onto a different menu that
arms right after it. (deck140 vs110 seq74->seq75: fixated on "option 2 = Wrath of God" on the cast
menu, then replied "2" onto the very next X menu -> X=5 instead of max 6.)

Proposed addition to Step 2's numeric-offset check:
- When a numeric menu's chosen index does NOT match the value/index trap pattern, test whether it
  equals the PRIOR decision's chosen index (a carryover), especially when the two menus armed
  back-to-back in the same turn/phase. The fix differs: value/index trap -> menu-space teaching or
  reversal; carryover -> a per-ask reset / distinct labeling on the second menu (engine/representation
  layer), and it is usually a low-frequency, often-immaterial artifact — do not over-weight a single
  instance or write a guide line for it (route to the engine ledger).

## Not proposed (checked and rejected)
- No change to the reach/deck-construction routing — the skill's "attribute the residual, route it
  upstream" section already covers this deck's dominant loss precondition exactly.
- No new negative construct — the wave-4 positive-default ruling held; this guide's revision uses one
  falsity-fact interception ("I have no mana is ALWAYS FALSE when a cast is offered", frozen from the
  obeyed wave-4 line) and a positive DECIDING SITUATIONS recap, no mid-guide DO-NOTs.
- No relaxing the 40-80 line budget: the revision came IN at 77 lines by compressing the two
  now-obeyed rules (X-menu, Staff targeting) to pay for the finisher-discipline rule — the exact
  Step-0 demotion the skill prescribes.
