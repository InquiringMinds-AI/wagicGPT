# deck36 -> core (general) prompt: **PASS**

**Verdict: PASS.** No core-prompt edit is proposed from this seat. It is the 27th consecutive PASS if
the wave-31 count (26) carries.

## Why each candidate did NOT route to core

**1. "Coloured mana cannot pay a generic cost" — PRESENT in core and LOSING at distance -> representation, not core.**
deck152 s4/s5, verbatim: *"'Mana costs are written like {2}{R} (two generic plus one red).' '{1}' is
one generic. If I have {W} and {B}, I cannot pay {1}."* The pilot **quotes the core prompt's own
sentence** and reaches the opposite conclusion, then answers `Cast nothing right now` with three
payable creatures offered (recovered in Main 2). By the layer-routing rule, a core line that is
present and still bypassed routes to REPRESENTATION — repetition is not a mechanism and a second
paragraph would be a pure tax on every deck. Routed to notes.md **N-36f** with a concrete surface
proposal (state what the mana BUYS on the mana line: *"any of them pays a generic {1}"*), and the
deck guide carries RULE #2 as the per-deck interim, which is the sanctioned placement for a
belief-that-VETOES that only bites decks with generic-heavy curves.

**2. A {0} mana cost is invisible — REPRESENTATION, and a code fix exists.**
No core sentence can make the pilot see a token the renderer deletes. The guard
`if (cost && cost->getConvertedCost())` at three emitters is the whole defect (notes N-36b). Adding
"some cards cost {0}" to core would tax 7 decks to patch 1 renderer branch.

**3. Poison counters are invisible and the blockers header computes false life math against infect — REPRESENTATION, HIGH.**
This cost deck36 the vs105 game (notes N-36a). It is a missing board field plus a wrong arithmetic
branch, not a doctrine gap; the core's blocking guidance is fine and was followed correctly against
non-infect attackers at the same header in the same corpus (deck158 s19). Elevating poison to a core
sentence would not help a pilot who cannot see the counter.

**4. "Main phase 1 | It is the opponent's turn" read as a contradiction — flagged for CONVERGENCE, not proposed.**
3 verbalizations at 2 opponent-turn priority windows at this seat, e.g. *"the prompt says 'Phase:
Main phase 1 | It is the opponent's turn.' This is a direct contradiction. Let's assume the game is
actually in **my** Main Phase 1"* (deck152 s15/s16). The pilot does not accept holding priority in
the opponent's main phase. My sample is 2 windows — far too thin to cut or add core text under the
single-seat rule. **Recommendation to synthesis: check convergence across seats.** If two or more
seats verbalize it, the cheapest fix is a COMPLETION of the existing core priority line rather than
new doctrine — or better, a representation change to the phase header itself (name the turn owner
before the phase: `Opponent's turn - their Main phase 1; you hold priority`), which is the
non-taxing option and stays consistent with "surface the fact, don't repeat the instruction".

**5. Cross-zone duplicate names — existing open ledger item (L2), 1 instance, not core.**
With one Master of Etherium on the battlefield and a second in hand the pilot called the prompt
self-contradictory. Instance handles are battlefield-scoped by design; the hand line carries none.
One occurrence, recoverable, already on the ledger.

## What core got RIGHT at this seat, worth recording against future removal experiments

- The **interface-contract anchor** ("if a Cast option is listed you can pay for it") is what pulled
  the pilot back every time the generic-mana belief fired — it recovered inside the same turn in
  every instance. Do not thin it.
- **Zero fallbacks in 144 decisions**, zero defers, zero truncations, across six games and five
  opponents, at a seat with a 108-line guide-free prompt. The reply protocol and answer-first
  contract are not under strain here even at 14,000-character replies.
- The **attacker/blocker floors** in core were sufficient without any per-deck reinforcement: 20/21
  attacker windows perfect against the eligible set, 14/14 defensible blocks. This is direct evidence
  for the "do no harm at a strong Step-0 seat" posture (wave-31 HL8) and against adding more combat
  doctrine to core.
