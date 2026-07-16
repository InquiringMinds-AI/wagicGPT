# notes.md - deck17 engine/harness ledger + rotation verdict (wave-16)

Layer-routed engine/representation items at the deck17 seat. Guide prose lives in strategy.txt;
this file is engine/harness/model only.

## Observed this corpus

1. STALE WITHIN-TURN MANA / DISTRUST-THE-CAST-LIST (representation + model-tax). vs62 s13
   (rl6732), vs110 s5 (rl9648): on a second same-turn cast the model recounts mana from its
   battlefield Island total, ignores the explicit `Mana available: {u} from 1 untapped source`
   line, and spends thousands of tokens asking "why isn't Wasp Lancer listed?" when the cast list
   is correct (mana already spent on an earlier cast this turn). The prompt is correct - the model
   overrides it. Representation lever proposals in general-suggestions.md. This is the successor
   to the (now-dead) hybrid off-color tax and is the main remaining latency source at hybrid-cast
   windows. Route: representation (cast-list / mana narration), NOT guide-only.

2. DEGENERATE-REPETITION / LONG-RAMBLE -> unparsed -> heuristic (model-incapacity, carried from
   wave-15). vs133 s14 (rl14224 unparsed), vs62 s17/s18 (rl12509/13448 unparsed at a no-blockers
   and a low-stakes cast), vs44 s34 (rl12472 unparsed at t19). The parse guard caught all of them
   correctly and the heuristic answered; none flipped a game (mostly no-blocker or trivial-cast
   windows). Same class as wave-15 note 1. Route: model-experiments residuals / long-ramble class.

3. HYBRID-PIP ANNOTATION EFFECT (positive, no defect). The wave-15 annotation `[hybrid: each
   {u/b} pays with U or B - total N mana]` rendered on every hybrid cast line at this seat and
   correlates with the collapse of the hybrid off-color belief (findings Class 1). No mis-render
   observed. Positive confirmation for the synthesis annotation-effect tally.

4. NO stale_echo / wrongful downgrade at this seat. The choice=-1 records here are genuine
   long-ramble/unparsed (item 2), not superstring echoes that named an offered option and got
   mis-routed. The stale_echo superstring fix is not implicated at deck17.

5. NO board-annotation contradiction of the wave-15 shape recurred (the `[tapped]...[attacking]`
   / re-cast-Gatewarden "died" contradictions). Board serialization read clean this corpus.

## Deck-construction note
deck17 is 4/6 and its two losses are both against decks that fly / gain life (vs44 mirror, vs110
affinity-lifelink). The deck's plan is sound vs the ground-bound field; its weakness is the race.
No construction change proposed - the guide's new race/deployment sections are the lever.

## ROTATION verdict
deck17 is NOT a rotation candidate. Its first guide VALIDATED with a measurable improvement (3/6
-> 4/6) AND produced substantive new guide work this wave: the guide is REVISED (not frozen) -
Class 6 (biggest-flyer-excludes-walls) is the root-cause fix for both losses, Class 4 (steal the
anthem early) sharpened, a race branch added, dead sections trimmed. Keep deck17 in the pool;
re-validate the revised guide next corpus, focusing on the two has-flyer matchups (mirror,
lifelink) and whether the stale-mana representation tax was addressed.
