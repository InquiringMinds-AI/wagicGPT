# General suggestions — deck109, wave 13

Two items for synthesis. Neither is a core prompt-body edit (deck109 votes core PASS, 8th wave).

## 1. PROMOTE the attackers name->index reconcile arm to cover BOTH the full-name AND the mixed forms (engine/harness, top new item this seat)

The brief already carries `ATTACK: Hellrider` (full-name) as a ledger candidate ("attackers name->index
reconcile arm mirroring parseChoice"). deck109 this corpus supplies the DATA to scope it correctly:
there are TWO distinct name shapes, and the mixed one is worse than the flagged full-name one.

- **Full-name (`ATTACK: Hellrider`)**: UNPARSED -> `choice=-1` -> baka heuristic attacks with all
  eligible creatures. For a pure-aggro deck this is OUTCOME-NEUTRAL (verified in combat: vs131 s19,
  vs135 s13, vs140 s33 all attacked with the intended creatures). Low urgency AT THIS SEAT, but a
  control/midrange seat where "attack with everything" is WRONG would be mis-served by the heuristic.
- **Mixed (`ATTACK: A1, Rakdos Cackler`)**: PARSES PARTIALLY — takes the A-indices, drops the trailing
  NAME token(s), records a NON-FALLBACK `choice=N` with fewer attackers than intended. 5 windows / 3
  games / 6 dropped declarations this seat (vs140 s7/s40, vs110 s10/s14, vs135 s8), confirmed in combat
  at vs140 s7. This one is SILENT — no fallback, no marker — so it evades every current metric.

**Recommended fix (route to the attackers parser, NEVER the guide):** in the attackers parse arm, after
consuming A-indices, ALSO resolve any residual NAME tokens on the `ATTACK:` line against the eligible-
attacker roster by unique-name match (the same reconcile logic the name-echo protocol uses for CHOICE),
and UNION them into the declared set. This fixes the mixed-form silent drop AND upgrades the full-name
form from "heuristic guess" to "the exact creatures the model named." Owner: the attackers-record parse
site in AIPlayerGPT.cpp (the one that already 1-based-parses `A<n>`).

## 2. Option-line HYBRID-cost annotation (Ledger Item B) — STILL UNSHIPPED, keep the guide card-notes

Verified 0 `- hybrid:` strings in any deck109 prompt this corpus; the option line still renders bare
`Cast Rakdos Cackler {b/r}` / `Cast Boros Reckoner {r/w}{r/w}{r/w}`. The guide card-notes carried the
load flawlessly again (all 7 hybrid casts taken AND engine-executed), so this is not urgent — but until
the durable annotation ships, the deck109 Rakdos Cackler and Boros Reckoner card-notes remain the SOLE
carriers and must NOT be demoted (thin-sample + unshipped-backstop, pause-aware demotion doctrine).
Proposed annotation unchanged: append `- hybrid: pay each with {R} or {B}` (resp. `{R} or {W}`) to any
option whose cost carries a `{X/Y}` symbol.

## Not suggesting (explicitly held)

- No guide edit for deck109 — the guide is at terminus, frozen byte-identical; the wave-10 hybrid line
  is now verified at the engine-execution level.
- No core prompt-body edit — deck109 votes PASS (8th consecutive wave).
- No guide line for the mixed-form attacker drop — the model already NAMES every attacker (the guide is
  obeyed); this is a parser bug and belongs in the notes ledger, not prose.
- No demotion of the Obliterator / LETHAL-branch guardrails — untested again this corpus (vs133 raced
  Obliterator out 20-0, 0 block windows); pause-aware demotion doctrine keeps them at full prominence.
- No deck-construction change at 5/6 — the vs140 loss is lifegain-control matchup + mana flood variance
  (a known RDW ceiling), surfaced by draw speed and made a loss by the latency adjudication.
