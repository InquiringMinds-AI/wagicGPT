# deck27 skill.md — proposals for the strategy-writing skill (wave-19)

Seat: Zombies (UB tribal), GUIDELESS FIRST PASS (Step 0-bis). One reviewer = single-seat by
default; each item below is tagged with its promotion status.

## Step 0-bis confirmations (this newcomer re-fired existing rungs — no new headline)

- **Trust-the-rendered-number, mana face (CONFIRMED, Nth seat).** A hybrid-heavy UB deck: every
  {U/B} pip pays with either color and the "[hybrid: each {u/b} pays with U or B - total N]" tag
  is present. No mana false-belief fallback fired at this seat (unlike deck49/deck135 mono-color
  seats) - suggesting the hybrid tag + two-color base is EASIER for the pilot than mono-color, not
  harder. Kept as guide rule #1 anyway (load-bearing, cheap). No change to the skill; just a data
  point that the false-belief risk scales with color-COMMITMENT, not just derived mana.

- **Losing-position / off-case-spiral doctrine (CONFIRMED).** 5 of 6 giants (>8k) sat in a BEHIND
  board state; the sole ahead giant was an over-deliberation in a WON game. Authored the off-case
  branch in the initial guide ("make the best play and pass, do not hunt impossible lethal") even
  though the WINS never engaged it, per the wave-18 promoted rung. No skill change.

- **0-attacker durdle instrument (CONFIRMED, applied).** No adj/cap games here, so the instrument
  had no turn-cap game to read - but the durdle showed up in the LOSS TEMPO instead (lords deployed
  t13, piecemeal attacks). Records the sibling observation: for a deck with no cap game, tabulate
  TIME-TO-FIRST-LORD and time-to-board-of-N as the durdle proxy, not just per-game attacker count.
  MEASUREMENT-only, single-seat, biases no guide content - adopt at reviewer discretion.

## WATCHES (single-seat, not promoted — promote on a second clean seat)

- **WATCH: the FREE-BLOCK DECLINE class (deck49 s9).** The trade annotation correctly printed
  "(you kill it, your blocker lives)" - a strictly favorable block that kills the opponent's
  team-wide first-strike lord - and the model DECLINED it (no blockers). This is distinct from the
  gang-pile trap (that was coordination failure across sequential asks; this is a SINGLE clean
  bundled block the model read correctly in its own reply and then refused). Candidate guide line
  for any deck with real blockers: "a '(you kill it, your blocker lives)' block is FREE - take it;
  declining it is a misplay, especially against an opponent lord/anthem-giver." The durable fix is
  arguably engine-side (the annotation is already correct; the model just disobeys it) - so this may
  be a MODEL-obedience watch, not a representation gap. Promote on a second seat where a correct
  favorable-block annotation is declined.

- **WATCH: combat STALE-BOARD HALLUCINATION, distinct from name-tolerance (deck14 s9).** The block
  prompt offered ONE attacker (A1); the model invented an A2 ("Gravelgill Axeshark 4/4") remembered
  from an earlier turn and answered "BLOCKS: B1:A2" - an out-of-range index -> illegal -> unparsed.
  This is NOT the old code/name-format discard the wave-19 name-tolerance batch targeted (rendering
  was clean and correct here), and salvageLoopedChoice cannot rescue an illegal-index assignment
  (well-formed but references a nonexistent attacker). Candidate GUIDE line (interim): "only the
  attackers listed as A1, A2, ... exist right now; do not block an attacker from an earlier turn."
  Candidate ENGINE/parser item (durable): when a bundled-block reply references an out-of-range
  attacker index, drop that pairing but SALVAGE the rest of the assignment rather than discarding
  the whole reply (here the reply had no other valid pairing, but a mixed reply would lose good
  pairings too). Routed to notes.md ledger. Promote the guide line on a second combat-hallucination
  seat.

- **WATCH: the AHEAD over-deliberation giant (deck110 s20).** One giant sat in a WON, ahead board
  state - the model re-derived the whole board over "what if their card is removal" on a trivial
  Main-2 cast. The off-case doctrine is framed around BEING BEHIND; this is the mirror (ahead and
  still rambling). Added a "when ahead, keep it simple, deploy-attack-pass" line to the initial
  guide. Single-seat and rare (1 of 6 giants) - WATCH whether the ahead-giant recurs; if it stays a
  singleton it is model noise, not a guide-worthy class.

## No new core-prompt candidate from this seat
Every item is single-seat and/or owns a layer below core (guide off-case, engine parser). See
general-suggestions.md for the layer routing.
