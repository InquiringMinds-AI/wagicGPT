# General suggestions — deck44, wave 11

No general (cross-deck / core-prompt) suggestion from this seat. **PASS.**

Rationale: this is a verification wave. The deck44 guide is FROZEN byte-identical (8th wave), the core
prompt is untouched (single-seat items cannot add a core line per the convergence gate), and every
wave-10 engine fix verified clean at this seat (stack "NO legal target" contradiction gone; ramble
guard holds with 0 scavenged casts; own-targets 29/29 obeyed). No new decision-level failure class
emerged. The one persistent model-relevant item (auto-tap color-stranding) is engine-layer and already
carried on the ledger; it routes to notes.md, not to any general/core surface.

Cross-deck items that MIGHT interest synthesis (both already-known, offered only for corroboration):
1. **Auto-tap greedy-blue-for-generic (engine ledger #2)** is a DECK-AGNOSTIC engine defect that will
   bite any deck holding an instant of a color while casting a generic-heavy spell of a different color.
   deck44 is the canonical victim (holds {U}{U} for counters), but synthesis may want to grep other
   seats' mana lines for the same tap pattern to confirm the class is engine-wide, not deck44-specific.
2. **Strict free-form block/attacker parser** (notes.md #3) is also deck-agnostic — any seat whose model
   declares blocks/attacks by NAME or with an out-of-range index will drop to the heuristic. Worth a
   corpus-wide grep for `unparsed_reply` on blockers/attackers records to size whether the strict parse
   ever flips a CORRECT stated declaration to a game-costing heuristic at some other seat.
