# Dev notes from deck-110 wave-6 (engine / harness / deck-construction ledger)

## ENGINE

### #E1 (HIGH — the wave's decisive engine finding). Re-attach no-op marker is INERT for equipment.
The re-attach no-op marker (`AIPlayerGPT::describeAction`, `src/AIPlayerGPT.cpp:1155-1156`)
NEVER fires for Cranial Plating (or any equipment). It gates on:
```
MTGCardInstance * moved = action.click ? action.click : (action.ability ? action.ability->source : NULL);
if (moved && moved->auraParent == action.target)
    out << " (ALREADY attached to it - this would change NOTHING)";
```
But **equipment does not use `auraParent`.** Confirmed in code:
- `AEquip::equip` (`src/AllAbilities.cpp:10431`) sets the host via `source->target = equipped;`
  and `equipped->equipment += 1;` — the host lives in `source->target`.
- `GameObserver::enchantmentStatus` (`src/GameObserver.cpp:1248`) sets
  `card->auraParent = card->target;` ONLY for cards that are AURAs and
  `!card->hasType(Subtypes::TYPE_EQUIPMENT)`. Equipment is explicitly excluded, so a Cranial
  Plating's `auraParent` is permanently NULL.
Result: the marker condition is structurally unsatisfiable for equipment; string
`"change NOTHING"` = 0 occurrences across all 6 deck110 prompts this corpus.

**FIX (one line):** also test the equipment host field, e.g.
```
if (moved && (moved->auraParent == action.target || moved->target == action.target))
```
(Optionally guard `moved->target` on `moved->hasType(TYPE_EQUIPMENT)` for clarity — but
`->target` on a non-attached, non-equipment permanent is generally NULL, so the OR is safe.)
Verify: a re-equip line to the current host must then render
"... targeting Steel Overseer (ALREADY attached to it - this would change NOTHING)".

**Impact / why it matters:** no-op re-equips this corpus were 9/18 (50%), unchanged from
wave-5's 15/35 (43%). The pilot READS the board's two-power done-signal, verbalizes "already
attached ... waste of mana," and re-equips anyway (`1783991383` s15; `1783995813` s43, s25)
— instruction has failed at full strength for two waves. The OPTION-LINE marker is the proven
correct lever (wave-5 diagnosis) and was simply shipped to the wrong field. Fixing #E1 is the
single highest-leverage change for deck110 (stops wasted {1}s AND wasted priority windows that
inflate decision count -> latency -> timeouts). Repro binary counterfactual is cheap via
archived `/tmp/wagic-<hash>`.

## HARNESS / INFRA

### #H1 (MED). Grind games generate huge priority-pass counts -> latency-bound timeouts.
vs140 (66 records, T30 cap) and vs133 (30 priority windows) spend dozens of decisions on
`pass` in unwinnable-by-combat stalls (double Lightmine; opp grind). Each pass is an inference
round trip. The no-op re-equips (#E1) add more. Two of deck110's six games are timeouts, both
adjudicated wins on life but both wall-clock-bound. Fixing #E1 reduces the count; a deeper
lever is auto-passing priority windows where the pilot has no instant/ability that changes
anything (an engine auto-pass heuristic for AI when the only legal actions are "pass" + a
provably-no-op equip). Route to the priority/auto-pass ledger.

## DECK CONSTRUCTION (route to the deck-list, NOT the guide)

### #D1. Threat-density / flood risk. deck110's 25 "creatures" include 8 near-zero-power
bodies (4 Signal Pest 0/1, Ornithopter 0/2) plus 3 Mox Opal + 4 Cranial Plating + 20 lands.
Draws heavy on Mox/land (vs44 flood: only one creature in 15 turns) or all-Signal-Pest/
Ornithopter (vs133: opp only to 10 by T11) produce NO clock and lose to variance. The guide
cannot manufacture a threat curve. If the deck is meant to be robust, consider trimming a
land or a Mox for a fourth 2-3-drop threat, or a 3rd/4th real beater.

### #D2. Counter-vulnerability is now LIVE (the ecosystem shift). With the stack surfaced and
counter options labeled (shipped this corpus), the counter deck (deck44) answers affinity's
discrete high-value threats 1-for-1 (countered Etched Champion via Arcane Denial in the
deck110 game; corpus-wide 13/14 counter windows per the deck44 reviewer). deck110's wave-4/5
dominance was PARTLY an artifact of the blind-counter seam that this platform fix closed. The
guide's answer (go wide / bait) is in strategy.txt, but the structural exposure is a
deck-list/meta reality: a low-to-the-ground, go-wide build resists counters better than one
leaning on Master of Etherium / Etched Champion as its only path. Not a guide bug — a
deck-construction + meta flag for the synthesis/deck-gen track.

### #D3. Galvanic Blast scarcity vs stall/Lightmine. Only 4 Galvanic Blast = the deck's sole
reach through Lightmine Field / a stalled board. vs140 the pilot correctly refused to attack
into double Lightmine (Lightmine rewrite working) and got opp to 3 — then couldn't close
because no Blast was drawn. Structural, not a piloting leak. If Lightmine/stall decks persist
in the pool, more reach (a 5th Blast, or another burn/evasion finisher) would matter.

## WHAT WORKED (freeze — do not touch)
- The wave-5 Lightmine rewrite: LANDED and IMPROVED the vs140 result (adj WIN 13-7 vs wave-5
  loss my7/op15); no suicide attacks into double Lightmine.
- Go-wide + Blast-to-face + lethal check: the vs131 win (opp -13) and both adjudicated
  timeout-wins show the plan executes when bodies are drawn.
- Mana falsity-fact: no "I have no mana" delusion recurred.
- Equip brake re-key to unattached platings: no observed suppression of legit 2nd attaches.
