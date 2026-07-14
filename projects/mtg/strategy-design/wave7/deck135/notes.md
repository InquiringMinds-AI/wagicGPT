# Deck-135 wave-7 — development notes

Run: `matchups-20260713-222831`. deck135 jsonl epochs and opponents in findings.md.

## ENGINE LEDGER

- **Fetch re-offer tax (UNFIXED, carried from wave-6; fresh repro + sharper metric).**
  The engine re-offers a fetch-crack activation at EVERY priority window a fetch land is
  in play. Two cost surfaces:
  - Consecutive re-picks of the SAME crack: now ≤2 (improved from wave-6's 3-4 chains).
    Repro: vs110 s2, vs140 s87, vs131 s2 (`"search basic land with Prismatic Vista
    targeting Snow-Covered Forest [cost: Life, Tap, Sacrifice]"` chosen twice in a row).
  - Held-fetch re-offer windows (the dominant cost): the crack is offered and correctly
    declined, but each is still a model call. Per game: vs140 **97**, vs44 71, vs133 51,
    vs109 49, vs110 44, vs131 5. vs140's 97 fed its 144-decision timeout.
  Fix (engine): once a crack activation is chosen, consume/withdraw it; and a held fetch's
  crack should not re-prompt every priority window (offer it at most once per main-phase
  window, or suppress on opponent turns / while tapped out). options_text greps make this
  trivially detectable: the identical `Put in Play with <fetch> targeting <land>` /
  `search basic land with <fetch> targeting <land>` string recurs verbatim across
  consecutive seqs. This is the #1 engine item for this deck's timeouts.

## DECK-CONSTRUCTION FLAGS (route upstream; not guide-length-fixable)

- **Manabase strands the deck's own 2-color win creatures.** The creatures that win
  (Ice-Fang Coatl {G}{U}, Ohran Viper {1}{G}{G}, Abominable Treefolk {2}{G}{U}) are gold /
  double-pip off a green-heavy, blue-thin base: blue comes from only 2 Snow-Covered Island
  + fetches that can reach Island (Misty Rainforest, Flooded Strand, Prismatic Vista) +
  Arcum's Astrolabe. Against a fast clock the fixing is too slow.
  - vs109 (LOSS, opp 12): 1 creature offered ALL game; Ice-Fang Coatl + Ohran Viper never
    castable.
  - vs44 (LOSS, opp 18): Abominable Treefolk NEVER offered (0 casts) — short blue all game;
    the pilot's stale "cast Treefolk immediately" plan chased a card the base could not pay
    for.
  - vs110 (LOSS, opp 20): only 2 creatures offered; opp never dropped below 20.
  Suggested construction direction (user's call): raise the effective blue count (more
  Islands / more Island-capable fetches / lean harder on Astrolabe as the blue enabler), or
  cut a pip off the creature suite. This is the deck's ceiling, not a piloting bug.

- **Slow clock is structural.** Even the WIN (vs133) took T12; losses/draw reached
  T17-T25. The bodies are 1/1-2/2; Abominable Treefolk is the only real closer and lands
  T16+ when it lands at all (vs131 cast a 15/15 at T16 but had already been ground to 1
  life). Feeds every timeout. Related to construction (threat size/count), not guide text.

- **Diamond Faerie is NO LONGER zero-offer** (correcting the wave-6 "dead weight, offered
  0 two waves" claim): offered 3× / cast 1× in vs131's T17 grind, once the deck reached 15
  snow permanents + Astrolabe fixing. Still rare (1 of 6 games), still not a plan-around
  card — the guide keeps it demoted but now names it in Rule #1's creature list so the
  pilot doesn't dig past it (see the vs131 Glacial-Revelation-over-Diamond-Faerie misses).

## MODEL-QUALITY RESIDUALS (route to model-experiments track)

- **Stale-plan around an uncastable card (vs44 s19/s30/s66).** The pilot re-affirmed "cast
  Abominable Treefolk immediately, 6/6" across many turns while Treefolk was never a
  castable option (manabase strand). Correct hand/board tracking (never claims it's on the
  battlefield) — so this is plan-diverges-from-legality sustained across turns, NOT the
  phantom-board class. Litmus case for stale-plan-vs-menu reconciliation.

- **No on-board hallucination this corpus** (the wave-6 phantom-lethal class did not
  recur; the board-read anchor held). No desyncs (0/6 games), no numeric-offset menu
  traps, no degenerate blocker replies.

## STOPGAP / DEFUSAL TRIPLES (line, defect + citation, removal condition)

1. RULE #1 board-read anchor ("if a creature is not printed on your battlefield line it is
   NOT in play") — patches model on-board hallucination (wave-6 vs131 s52-60). Removal:
   when the phantom-lethal class is resolved model-side and stays at 0 across multiple
   waves with adequate precondition count. KEEP (held this wave, but off a modest
   precondition count).
2. RULE #1 / DECIDING-SITUATIONS uncastable-Treefolk anchor ("if Treefolk is NOT in your
   cast list you cannot pay for it yet; deploy what IS listed") — patches the vs44 stale-
   plan-around-uncastable pattern (manabase strand). Removal: when the manabase is fixed
   OR the model reliably reconciles a stale plan against the current legal list.
3. Force face-burn redirect ("NEVER Force a burn spell pointed at your own face") — patches
   the vs110 s45 over-fire (Galvanic Blast to face while screwed). Removal: when Force
   over-fire on face-burn stays at 0 across waves.
4. Fetch re-offer guide line ("do not re-pick a fetch-crack you already took; the land
   resolved") — INTERIM stopgap for the engine re-offer defect above. Removal: when the
   engine consumes/withdraws the crack activation (engine ledger item #1).

## FREEZE-CHECK (turnaround-preservation / OBEYED-BUT-LOSING mode)

The guide was revised as a constrained diff off the live deployed guide. Frozen lines and
where they survived:
- **RULE #1 block** — FROZEN verbatim, except point 1 now also names Diamond Faerie in the
  creature list and adds a one-clause "dig for a bigger threat is the losing thought"
  (closes the vs131 s23/s28 value-over-creature misses). The board-read anchor sentence is
  verbatim.
- **RULE #2 block** — FROZEN verbatim (attack seam healthy; no "no attackers" freezes; the
  Boreal-Druid carve-out did not misfire — Druid offered/attacked cleanly).
- **WHO IS THE BEATDOWN / MANA / MULLIGAN / LETHAL CHECK** — FROZEN verbatim.
- **Force of Negation KEY-CARD line** — kept its board-wipe scoping (obeyed 9/10); ADDED
  named sweepers (Black Sun's Zenith, Lightmine Field, from observed vs140 targets) and the
  face-burn redirect. The obeyed core ("spend it ONLY on a board wipe / a spell that kills
  your only threat") is verbatim.
- **Abominable Treefolk / Diamond Faerie KEY-CARD lines** — kept; added the uncastable-
  Treefolk anchor and the "Diamond Faerie is a creature under Rule #1" clause.
- **DECIDING SITUATIONS** — kept all prior lines; added the uncastable-Treefolk situation
  and a Force board-wipe-vs-face-burn situation. All positive situation→play form.
No frozen line was dropped; new text targets only the value-over-creature edge and the two
new stopgaps.

## VALIDATION TODO (before this guide is credited)
Re-run self-play with this guide in place and confirm: (a) Rule #1 conflict rate holds or
improves (Diamond Faerie now taken over Glacial Revelation); (b) Force over-fire on
face-burn stays at 0; (c) no phantom-board recurrence; (d) the engine fetch-re-offer fix,
if it ships, drops the held-offer window counts and the timeout rate. The record itself is
construction-bound — do NOT read a flat/low win-rate as a guide regression until the
manabase flag is addressed.
