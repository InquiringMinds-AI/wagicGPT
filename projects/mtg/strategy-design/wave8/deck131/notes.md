# deck131 wave-8 - working notes (catch-all)

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260714-044131/`, 6 deck131 games, 1W/4L/1TO.
Seat files (from `gamestart` record - no epoch arithmetic needed this wave):
vs135-W 1784022098 | vs140-TO 1784024389 | vs109-L 1784025145 | vs110-L 1784026638 |
vs133-L 1784027267 | vs44-L 1784027271.

## Reproducible evidence
- Land-menu fix fired: python over each jsonl, count decisions whose options_text has both a
  "Play Mountain" and a "Play Island" entry -> 12 total (vs135 2, vs140 5, vs109 2, vs110 2,
  vs133 1, vs44 0). All 12 chose "Play Mountain" (checked chosen_text). vs44 had 0 because it drew
  no Mountain until T7 (nothing to co-offer). Wave-7 count was 0.
- First Mountain PLAYED per game: vs135 T1, vs140 T1, vs109 T0, vs110 T0, vs133 T0, vs44 T7.
- vs44 false KEEP: opener = "Island; Young Pyromancer; Cyclonic Rift; Island; Island; Prism Ring;
  Cyclonic Rift" (0 Mountain, no Artificer's Epiphany/Opportunity) -> guide says MULLIGAN; pilot
  KEPT (seq1 reply quotes the keep-side guardrail, drops "Mountain and", keeps on "Young Pyromancer").
- vs140 protect-payoff miss: seq51 T34, option "Cast Dissipate {1}{u}{u} - can target on the stack:
  Pyroclasm", chose "Cast nothing right now"; reply asserts the 2/2 Guttersnipe survives 2 damage.
  Guttersnipe events: cast+swept T11, T17 (dealt 2 first), T35 - Pyroclasm each time.

## Mulligan tally (recheck metrics, both directions)
- (no-Mountain openers mulliganed)/(no-Mountain openers) = 1/2 (vs140 mull ok; vs44 false KEEP).
  Wave 7 was 1/1. New failure mode: false KEEP (was false MULLIGAN in wave 7).
- (keepable Mountain-hands kept)/(keepable) = 5/6 (all Mountain-hands kept; wave-7's false-mulligan
  did NOT recur). The keep-side guardrail held for its intended direction.
- Both n small; the fix this wave is a STRUCTURAL gate ordering, not a louder rule.

## Freeze-check (obeyed-but-losing discipline - every frozen line accounted for)
- Guttersnipe cast-every-turn #1: KEPT verbatim. Obeyed where testable (vs140); 0 cast-nothing-out.
- Two-gear identity: KEPT verbatim.
- Deploy-payoffs / cast-creature-first: KEPT; ADDED a sentence naming the Young Pyromancer token
  swarm as a standalone win path (this wave's WIN was YP-swarm, no Guttersnipe) - additive, ties to
  an existing payoff, does not change the pick order.
- Mountain-first: pick rule KEPT (re-stated for the co-offer that now exists); DROPPED the interim
  "hold non-Mountain lands to reach the Mountain" stopgap AND the stale "one-land-at-a-time menu"
  explanation (removal condition MET - fix fired 12/12).
- Attack-every-creature / no-block-while-racing / NEVER-block-Guttersnipe: KEPT verbatim (obeyed).
- Reactive-spells: KEPT; ADDED a board-wipe-recognition sub-bullet to the counter line (Guttersnipe
  2/2 dies to "deals 2 to all creatures"; counter it) - a clause on a frozen line.
- Draw-spells / Prism-Ring-lifegain / lethal-check: KEPT verbatim (obeyed).
- Mulligan block: REWRITTEN as a strict two-step gate (STEP 1 red-source gate decides alone,
  un-overridable; STEP 2 keep-side only for hands that pass). The load-bearing tokens KEPT verbatim:
  the literal-string "Mountain" read, the "cast-once-I-draw-a-Mountain is a TRAP" sentence, the
  no-Mountain override of the general 2-5-lands rule. The keep-side guardrail was RE-SUBJECTED so
  "Young Pyromancer" can no longer stand alone as a keep trigger (Mountain is the subject).
- Winning-line block: UPDATED to the vs135 YP-swarm win; removed the stale double-Guttersnipe
  turn-by-turn self-citation (no game this wave matched it). Guttersnipe grind kept as the alternative.
- DECIDING SITUATIONS: land line updated to the co-offer; ADDED the board-wipe-counter line.

## Harness sweep
- 0 desyncs, 0 engine no-ops, 0 defers (`deferred_to_heuristic`), 0 `fails validation`, across all 6
  games and both stderr sets. Transport clean (waves 5-6-7-8).
- Timeout (vs140) still writes no per-seat gameend in the jsonl; results.tsv now adjudicates it.

## For the next deck131 reviewer (gotchas)
- n=6, single run: win-rate is noise. Stable metrics: multi-land offer + Mountain-first rate (now
  12/12 - if this regresses, the land fix regressed); mulligan BOTH directions (a guardrail can invert
  - re-check no-red-mulligan AND keepable-kept every wave); Guttersnipe-out cast rate (precondition
  rare - only 1/6 games had Guttersnipe live); block quality; attack participation (100%).
- CHECK THE OPTION LIST when a guide rule names an option to pick - the wave-7 finding lived there;
  this wave the FIX also lives there (multi-land co-offer).
- Isolate `Your battlefield (creatures: N):` for on-board checks; the opening-hand line for mulligan
  and Mountain reads; the `A#.` list / attackers records for combat.
- vs140 (lifegain + repeated Pyroclasm), vs110 (affinity, Master of Etherium), vs109 (mono-red
  aggro), vs133 (zombies) are hard-to-unwinnable; don't read those losses as guide/pilot failures.
- Watch: `damage >= toughness = dead` false belief (vs140 seq51) - if a second seat shows it, it is a
  core rules-fact.
