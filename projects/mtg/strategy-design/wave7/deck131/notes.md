# deck131 wave-7 - working notes (catch-all)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260713-222831/`, 6 deck131 games, 1W/4L/1TO.
Seat files (epoch): vs44-L 1783999714 | vs135-W 1784001601 | vs109-L 1784004822 |
vs110-L 1784006250 | vs140-TO 1784006779 | vs133-L 1784008428.
Pairing: seat_epoch = results start_epoch + 2..3; confirmed by opponent card names in prompts.

## The headline evidence (reproducible)
- One-land-at-a-time menu: `python3` over each jsonl, no decision has both "Play Mountain" and
  "Play Island" in options_text -> printed nothing. Land opts are always a single land's Play/Hold.
- First Mountain PLAYED per game: vs110 T0 (Mtns hand-first), vs140 T3, vs109 T4, vs44 T7,
  vs135 T9 (mull-6), vs133 T10 (false-mulled). vs110 = the control (curved out YP T2, Gut T4).
- vs133 false mulligan: opener raw = "Essence Scatter {1}{u}; Mountain; Mountain; Island; Elixir
  of Immortality {1}; Prism Ring {1}; Young Pyromancer {1}{r}" -> mulliganed as "zero Mountains".

## Mulligan tally (recheck metrics)
- (no-Mountain openers mulliganed)/(no-Mountain openers) = 1/1 (vs135). Wave 6 was 1/3. Improved.
- (keepable Mountain-hands kept)/(keepable Mountain-hands) = 5/6. The miss: vs133 (false mull).
- BOTH n small; do not over-read. Guardrail added to strategy.txt (literal-string trigger + keep-side
  "a Mountain + a payoff is always a keep").

## Freeze-check (turnaround discipline - every frozen line accounted for)
- Guttersnipe cast-every-turn #1: KEPT verbatim (top block). Obeyed; 0 cast-nothing-while-out.
- Two-gear identity (race/dig): KEPT verbatim.
- Deploy-payoffs / cast-creature-first: KEPT verbatim.
- Mountain-first: KEPT (wording) + ADDED the interface stopgap beneath it (new, tagged for removal
  when the harness fix lands).
- Attack-every-creature / no-block-while-racing: KEPT verbatim; ADDED "NEVER block with Guttersnipe"
  as a new explicit line (vs110 violation).
- Reactive-spells / draw-spells / Prism-Ring-lifegain / lethal-check: KEPT verbatim.
- Mulligan block: REWRITTEN in place (literal-string trigger + false-positive guardrail); the
  no-Mountain override and the "cast-once-I-draw-a-Mountain is a TRAP" tokens KEPT verbatim.
- Winning-line block: UPDATED to the vs135 win (mulligan a 0-Mtn hand; survive on counters; grind a
  SECOND Guttersnipe to close) - the wave-6 line's exact turn-numbers no longer matched a real game.

## Harness sweep
- 0 desyncs, 0 engine no-ops, no numeric menus, across all 6 games. Transport clean (waves 5-6-7).
- Timeout (vs140) writes no gameend - still open ask.

## For the next deck131 reviewer (gotchas)
- n=6, single run: win-rate is noise. Stable metrics: attack participation (100%), mulligan
  discipline (both directions now), Guttersnipe-out cast rate (precondition rare - only 1/6 games
  had Guttersnipe live with a cast window), block quality, and the NEW land-menu offer check.
- CHECK THE OPTION LIST, not just the board, when a guide rule names an option to pick. The whole
  wave-7 finding was invisible from the board/reply - it lived in options_text.
- Isolate `Your battlefield:` for on-board checks; `Your hand:` for the Mountain-count and mulligan
  reads; the raw prompt for the `A#.` attacker list.
- vs110 (affinity, Master of Etherium 10/5 on curve) and vs140 (lifegain) are near-unwinnable;
  don't read those losses as guide/pilot failures.
