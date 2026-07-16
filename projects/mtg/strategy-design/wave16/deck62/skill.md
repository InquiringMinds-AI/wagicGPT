# deck62 skill proposals — Step 0-bis refinements from an ENGINE-DECK guideless first pass

deck62 is the first ENGINE deck (draw-engine + auras) to run Step 0-bis; decks 17/21 were an
evasion-aggro and a burn-aggro deck. The doctrine held; three refinements are engine-deck-shaped.
All are proposals for the synthesis agent — single-seat this wave, flagged as such.

## S1 (PROPOSED, engine-deck extension of Step 0-bis rule 3) — for an ENGINE deck, rule #1 is
"what your cards DO", and the sharpest tell is qwen HALLUCINATING an effect the card lacks, not a
false BELIEF that vetoes a real option.
Step 0-bis rule 3 says "when one idiosyncrasy dominates, make its corrective FACT rule #1." decks
17/21 fit the belief-that-VETOES shape (hybrid "I can't pay"; "hold burn"). deck62 is a different
shape: qwen invented effects the cards do not have — Fists of Ironwood "kills it (1 damage to a
0/2)", "gives +1/+1" (vs110 s3-s5, 13k tax across three records). The corrective is not "you CAN do
X" but "your card does NOT do X; here is what it actually does." Proposed refinement: on an engine/
combo deck, the #1 rule is a CAPABILITY TABLE — one line per key card stating its real effect in
plain terms — because the pilot's failure mode is fabricating card text, and a capability table
pre-empts the whole fabrication class. Rank the table by tax (the cards qwen wrote the most words
mis-describing go first). This is the engine-deck instantiation of rule 3, not a replacement.

## S2 (PROPOSED, new, 1-seat) — teach ENGINE SEQUENCING as an explicit ordering rule, because a
weak pilot will not infer a play-order from a static ability description.
The Argothian Enchantress draw ("whenever you cast an enchantment, draw a card") was present in 47
prompt records and qwen NEVER sequenced around it — it cast cheap enchantments before deploying the
enchantress every game. A capability line alone ("she draws off enchantments") is insufficient; the
guide needs an ORDER imperative ("cast her FIRST, THEN dump enchantments"). Proposed: when a deck
has a trigger whose value depends on PLAY ORDER (an enchantress, a prowess/magecraft body, a
sacrifice payoff), write the sequence as an explicit rule, not just the card's ability text. This
generalizes Step 0-bis rule 5's "write the floor on the first pass" from ATTACKING to SEQUENCING:
any value that only exists if actions happen in a certain order must be stated as an order.

## S3 (PROPOSED, sharpens Step 0-bis rule 5's eligibility note) — on a token/go-wide deck, the
attack-floor must explicitly say the "creatures that can attack" LIST is the eligible set and
summoning-sick bodies are already excluded; and it must name which of the deck's OWN 0-power
creatures to hold back.
deck62's worst combat tax (vs135 s20, 11.6k -> unparsed alpha strike) came from qwen counting 4
summoning-sick Saprolings as attackers (they were absent from the offered list, correctly) AND
agonizing over whether to send a 0/1 Argothian. Both are pre-answerable: "attack from the list;
it already excludes the ineligible" + "never attack with your 0-power engine/dork creatures (name
them)". deck17's guide named its Defenders as the exclusion; deck62 shows the same exclusion rule
applies to 0-POWER ENGINE PIECES and SUMMONING-SICK TOKENS, not just Defenders. Proposed: broaden
the Step 0-bis rule-5 exclusion clause from "name the walls" to "name every creature the model
should NOT declare — walls, 0-power engine/mana creatures — and state that the eligible-attacker
list already drops summoning-sick bodies so it should not re-derive eligibility."

## S4 (PROPOSED, reinforces Step 0-bis rule 2) — an aura/stat-stacking deck generates a distinct
tax class (recomputing the pumped creature's P/T from its aura stack) that persists EVEN IN WINS;
pre-answer it with "trust the printed [X/Y], do not re-derive."
Even in the won game (vs21 s19, 10.4k tax) qwen re-derived the 9/10 Birds from "+2/+2 per
enchantment... Gaea's +1/+1... Web +0/+2..." instead of reading the battlefield line's printed
[9/10]. The board serialization ALREADY shows the final stats; the guide should tell the pilot to
use them. Convergent with the deck135 board-representation rung — same fix (read the rendered
number, don't recompute from parts). 1-seat here; note for convergence if a second stat-stacking
deck (auras/counters/anthems) shows the recompute tax.

## Convergence note
S1/S2/S4 are the engine-deck face of Step 0-bis and mutually reinforce (capability table + ordering
rule + trust-the-rendered-number all attack the "qwen has no card model" root). S3 is a clean
2-seat-adjacent extension of an existing rung (deck17 exclusion -> generalized). None warrants a
core-prompt edit; all live at the guide-authoring-method layer. Recommend the synthesis agent fold
S1-S4 as engine-deck refinements to Step 0-bis, tagged single-seat pending a second engine deck.
