# deck140 wave-8 — development notes (engine / harness / model / deck-construction)

## FREEZE-CHECK (turnaround mode — constrained diff off the live wave-7 guide)
Every line the wave-8 corpus showed OBEYED was frozen; wording changed only where the shipped
representation made a paragraph redundant. Where each frozen item survives in strategy.txt:
- **Deck identity block** — verbatim.
- **Countable SWEEP TRIGGER (C from `(creatures: N)`; fire if C>=1 & life<=16-or-attacked; hold if
  C==0)** — verbatim, now explicitly anchored to the printed `(creatures: N)` string (0 overshoot,
  obeyed). FROZEN.
- **Tapped false-belief kill** — DEMOTED from a 4-line paragraph to one line anchored to
  `[tapped - untaps and can attack next turn]` (representation carries it; pilot swept tapped Ornithopter
  vs110 t6). Corrective FACT + marker string retained per demotion rule.
- **Artifact false-belief kill** — DEMOTED to one line ("artifacts don't lower C or shield a creature")
  (representation carries it; pilot swept the affinity board vs110 t10). Corrective FACT retained.
- **Shroud false-belief kill** — KEPT, lightly compressed (representation does NOT carry targeting;
  pilot cast Damnation/BSZ into a shroud creature vs44 t11/t13 — obeyed, so keep it as-is).
- **WHICH SWEEPER (color routing, first-payable)** — verbatim + ONE new guardrail line on BSZ-X (do not
  cast BSZ for X below the target's toughness). New text aimed only at the vs109-t4 wasted-BSZ miss.
- **#1 RULE Rakdos hold** — verbatim (held every non-lethal window; one desperation fire at L3). FROZEN.
- **X MENU** — verbatim (option 1 = biggest X, 4/4). FROZEN.
- **DEPLOY WHEN SAFE (Staff/Blightsteel/Lightmine/Venser/Elixir)** — verbatim (Staff cast every game;
  Lightmine deployed 3 games; Blightsteel cast the one game affordable). FROZEN.
- **STAFF OF NIN face-ping** — verbatim (100% of activations targeted the player face). FROZEN.
- **LANDS / MULLIGAN / OVERRIDE** — verbatim. FROZEN.
- **DECIDING SITUATIONS** — verbatim except the first two bullets now reference the printed
  `(creatures: 1)` / `(creatures: 0)` strings to match the representation anchor; the
  tapped/artifacts/shroud one-line recap kept.
No frozen line is missing from the revision.

## INFRA / HARNESS
1. **`-j` latency saturation persists and still inflates control-mirror timeouts.** deck140 games hit
   latency spikes to 68.4s (vs110), 59.6s (vs135), 57.3s (vs109), 52s (vs131); median 7-10s. A control
   deck makes 90-104 decisions/game, so at these latencies a game can burn the wall-clock cap while the
   pilot is playing correctly (vs135 timed out AHEAD 21-16). For any run whose DECISION QUALITY matters,
   use `-j 3` or raise `WAGIC_GPT_TIMEOUT`. IMPROVEMENT vs wave-7: the fallback cascade did NOT recur —
   0 empty-reply fallbacks this corpus (wave-7 vs135 was 19/34), so latency now costs wall-clock, not
   corrupted decisions.
2. **Control-mirror timeout adjudication still open (carried from wave-6/7).** vs135 timed out with
   deck140 AHEAD (21-16, t28); vs131 timed out with deck140 behind but only because its finisher was
   countered (below). A raw timeout row is not a deck140 loss/failure. The cap-by-decision-count /
   stalemate-detector / adjudicate-by-life-at-cap ask remains the right fix. results.tsv now fills
   life/turn on timeouts, which made this triage possible without opening the logs blind — good.

## ENGINE / REPRESENTATION
3. **Representation ask from wave-7 is CLOSED (shipped + fired).** `Opponent battlefield (creatures: N)`
   is on every board line; `[tapped - untaps and can attack next turn]` fired 86 times. Together they
   dissolved the tapped + artifact false-belief holds end-to-end (0 recurrences). This is a clean
   representation-beats-instruction close for the sweep decision. No further representation ask for
   deck140. (The `events` field also made the vs131 counter machine-readable: `Blightsteel: stack ->
   library` — resolved-vs-asserted works.)

## DECK-CONSTRUCTION (the user owns deck edits — flags only, restated not relitigated)
4. **Reach starvation + no closer is now the ENTIRE loss/timeout column.** With the overshoot fixed,
   deck140 executes its sweep-and-grind plan faithfully and still cannot close:
   - vs aggro (vs109 t27, vs133 t17, vs110 t15): swept correctly repeatedly, out-raced anyway — no
     early lifegain/blocker to survive turns 4-8, and a 1-damage/turn clock (Staff). Every game got
     LONGER than wave-7 (the fix bought turns) but the reach wall is unchanged.
   - vs counter-control (vs131 t50): the ONLY real closer, Blightsteel {12}, was COUNTERED by Essence
     Scatter and the deck had no second threat; Rakdos X=11 was short of opp-33. The deck folds to a
     single counterspell on its one castable win.
   Options, rough priority (unchanged from wave-7, now better-evidenced):
   - a cheaper or second recurring clock so the deck closes before the control-mirror cap and has a
     threat #2 after a counter;
   - a small amount of early defense/lifegain vs aggro (Lightmine Field x4 + Venser's Journal x2 arrive
     too late; a 1-2 mana speed bump would help);
   - a couple more UNTAPPED red sources so Pyroclasm {1}{r} is a reliable turn-2/3 cheap sweeper
     (repeatedly the pilot had W/B mana and no red and had to route to Wrath/Damnation/BSZ — the guide
     handles this correctly now, but red-light early turns still cost tempo).
   14 sweepers is the right density — the deck needs TEETH (a closer that survives a counter and beats
   an aggro rebuild), not more text. Do not spend guide length on reach starvation.

## MODEL (route to model-experiments residuals, not a guide line)
5. **Runtime board-parse under clutter — now handled by representation, so no longer a litmus.** The
   pilot correctly read C off cluttered 8-10-permanent boards this wave because the count is surfaced;
   the wave-7 model-capability question (does the pilot count creatures off a mixed board?) is moot
   while the representation ships. If the count is ever removed, this returns as a litmus.
6. **Snowball-single-threat WATCH (single seat).** vs44's Faerie Bladecrafter gained +1/+1 each turn
   past sweep range while the fire trigger correctly held at high life. A real opportunity miss, but
   legislating a high-life fire condition risks re-opening the wave-7 overshoot. Held to a WATCH (see
   findings.md / skill.md P4); promote only on a second seat.
