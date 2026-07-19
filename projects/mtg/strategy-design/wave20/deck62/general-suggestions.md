# deck62 wave-20 -- general (cross-seat) suggestions

## 1. (cross-seat, ENGINE, MEDIUM) The baka blocker-fallback should not gang-block or chump with a 0/1 shroud/engine creature when a single real blocker makes the block lethal-safe.
Both deck62 blocker fallbacks (vs14 s26, vs135 s19) truncated pre-commit -> heuristic -> the heuristic
GANG-BLOCKED a small attacker (a 2/2) with the 0/1 Argothian engine alongside a real blocker (Yavimaya)
that already killed the attacker alone -- sacrificing the draw engine for nothing. Any deck with a
high-value 0-power creature (Argothian here; other decks' mana dorks, planeswalker-esque 0/1s, engine
pieces) hits this whenever a block reply unparses. The blocker heuristic should (a) prefer non-0/1,
non-shroud, non-engine blockers, and (b) not add a second blocker to an attacker that is already dead to
the first. This is where the model's answers fail MOST expensively (a block fallback loses a card, not just
a tempo beat). MEDIUM: it recurred 2/2 at deck62 and both times cost the engine; the fatal-vs-harmless split
is trajectory-dependent (vs14 won anyway, vs135 was a floor), but the pattern is deterministic.

## 2. (cross-seat, HARNESS, LOW) A reply that emits the answer-format TEMPLATE literally ("CHOICE: [Number] ([Name])") alongside a real committed choice should not unparse.
deck62 vs49 s18: the model reached a valid "CHOICE: 1 (Dragonmaster Outcast)" but the giant reply ALSO
carried the literal template placeholder "CHOICE: [Number] ([Name])" (x2) plus a contradictory illegal
"CHOICE: Argothian Enchantress" -> parser could not disambiguate -> unparse. A cheap tolerance: DROP
CHOICE lines whose bracketed content is a literal template placeholder (`[Number]`, `[Name]`) before
disambiguation, and among the remainder prefer the last well-formed `CHOICE: N (Name)` whose N is in range
AND whose name matches option N. Cross-seat by construction (the template appears in the shared system
prompt, so any model can parrot it). LOW: single instance, floor game.

## 3. (cross-seat, model/decode watch -- NOT a guide item) The dominant deck62 unparse driver shifted from decode-REPETITION loops (wave-19) to advancing-spiral / over-deliberation truncations (wave-20).
5 of deck62's 7 fallbacks are ADVANCING reasoning that runs past the token budget: survival math while
behind (vs135 s22), a settled cast decision the model keeps re-deliberating past ("Next turn...") while
AHEAD (vs35 s17), a trivial land-drop spiraled into board reconstruction (vs14 s34), and one block-math
enumeration (vs135 s19). Only vs14 s26's tail is a true verbatim repetition loop -- and it was fueled by a
guide near-contradiction (see skill #2). LESSON for reviewers: bucket truncation-unparses by whether the
tail REPEATS (decode loop -> model/decode watch) or PROGRESSES (advancing spiral -> length/budget, and
guide-reachable ONLY when the spiral is a genuine reasoning branch the off-case can shortcut). The
over-deliberation-of-a-SETTLED-decision variant (model reaches the right answer, then keeps going and
truncates before the CHOICE line) is a specific advancing-spiral shape a guide "decide and answer, do not
keep deliberating past your conclusion" nudge MIGHT reach -- but it is low-confidence and cross-seat; watch
for it before writing prose.

## 4. (observation, not a change) deck62's record remains a matchup-shape floor.
3/6 this wave (up from 2/6), on pool luck (drew guideless deck102; beat 14/27). All three losses are
matchup floors: deck49 red aggro never let a board form (all attack decisions opts=1), deck35 Slivers
out-tempo a go-wide mirror, deck135 out-sizes with an 8/8 by T8. The three wins all fired the taught
attack-every-turn/go-wide beatdown line. It is a turn-8-13 ground deck that floors against faster/wider
clocks; the guide cannot fix a clock deficit -- route the record to the roster owner, not the guide.
