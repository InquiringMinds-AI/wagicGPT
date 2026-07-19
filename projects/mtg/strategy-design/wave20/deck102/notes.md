# deck102 dev notes - wave 20 (engine/harness/card-script ledger + rotation verdict)

## ENGINE / REPRESENTATION items

### E1 (HIGH VALUE, cross-deck) - the targeted-discard REVEAL prompt misrepresents a pick-ONE effect.
Thoughtseize's `reveal` record is framed as *"Reveal: you looked at the top N cards of your LIBRARY.
Decide ... which go to 'choose discards' ..."* with subset (`PUT: n, m`) semantics. TWO defects:
1. It labels the **opponent's revealed HAND** as "the top N cards of your library" - factually wrong;
   the cards are `revealzone(targetedpersonshand)`.
2. It uses choose-a-SUBSET framing on a pick-EXACTLY-ONE effect (`auto=... target(<1>*[-land]|reveal)`).
   This drives the model to OVER-SELECT (2 of 3 corpus records: picked both Boomerangs vs14, all 3
   nonlands vs135). The engine correctly discards only the FIRST selected eligible card (mechanical
   integrity CLEAN - ENGINE-R1 same-tick finalize confirmed on this seam), so over-selection is currently
   harmless, but it is a LATENT quality bug: if the pilot's true #1 is not the lowest eligible index,
   over-picking discards the wrong card.
Proposed fix (representation, verify against real Oracle first): render the reveal as a pick-ONE prompt
sourced from the opponent's hand - e.g. *"The opponent revealed their hand. Choose the ONE nonland card
they discard:"* with a single-choice reply. Generalizes to every targeted-discard (Duress, Distress,
Inquisition-style) that routes through the same reveal-partition path. This is the deck102 reviewer's
owned surface; first review of it.

### E2 (WATCH, low confidence) - repeated cast-offer of an already-cast SINGLETON.
vs135: "Cast Witch of the Moors" was CHOSEN 3x (s20/s21/s24) yet only ONE `Witch of the Moors: stack ->
battlefield` event occurs all game. vs62: "Cast Dreadhorde Invasion" chosen 3x (s5/s6/s16). In a singleton
deck a given card casts once, so either (a) the cast is not resolving and the option is re-offered, or (b)
a stale cast option persists after resolution and the model re-picks it. Wasted decisions either way.
Needs an engine probe (opponent-seat cross-read or a pinned probe deck with a marked singleton). Not a
guide-fixable item. Distinct from the known stale-plan class because the option itself appears to re-arm.

### Corpus-construction observation (not a bug) - commander decks under classic selfplay whiff their payoff.
deck102 runs as a 100-card singleton with its commander (Tergrid) shuffled into the library as a 1-of
(life 20, classic rules, library 93 = 100-7). A build-dependent ENGINE deck whose entire plan keys on a
single card will fail to draw it ~1/3 of games at 6 reps, so its corpus over-represents the headless mode.
Not an engine defect; a note for anyone reading deck102 (or other #CMD decks) win-rates - the engine-card
absence is variance, and the guide is written to function without it (see strategy.txt). If a future wave
wants to VALIDATE the Tergrid engine specifically, use a probe deck that stacks Tergrid + discard/sac
enablers (per the skill's probe-deck pattern) rather than waiting on the 1-of.

## CARD-SCRIPT / Oracle spot-checks (all confirmed engine == Oracle; no data fixes needed)
- Tergrid `text` matches Oracle (steal on opponent sac of nontoken permanent / discard of a PERMANENT
  card; auto excludes instants/sorceries). Pox/Smallpox symmetric (both `controller` and `opponent`
  legs) - matches Oracle. Archon of Cruelty ETB+attack quad-trigger, Syr Konrad death/discard ping,
  Phyrexian Obliterator sac-that-many, Gray Merchant devotion drain, Corrupt X=swamps any target - all
  match. No Oracle divergence found in the cards this deck actually played.

## ROTATION VERDICT
**NOT a rotation candidate.** deck102 is a GUIDELESS NEWCOMER on its first pass; per doctrine a newcomer is
never a rotation candidate on its first pass. It produced a full NEW initial guide (strategy.txt) = a
new-work signal by construction. Next wave: re-validate the initial guide (Step 0-ter) - focus on whether
(a) the symmetric-trap fire-gate stops the Pox/Smallpox-while-behind self-harm, (b) the deploy/attack floor
lifts the durdle (does opponent life actually move?), (c) the Thoughtseize pick-ONE line stops over-
selection, and (d) any game that DRAWS Tergrid exercises the steal engine. E1 is the priority engine item
to work between waves.
