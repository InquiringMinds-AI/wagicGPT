# deck116 — wave-35 dev notes (engine/harness lane; repros by seq)

All items arm C (`matchups-20260822-111102`, deck116 seat files) unless marked.

1. **HIGH (harness, re-ask docket — the brief's named item): illegal multi-block silently
   truncated to the first pair.** vs105 seq25 t12: reply `BLOCKS: B1:A1, B1:A2, B1:A3` with
   sole blocker Emrakul vs three infect attackers; prompt printed "Assign each blocker to AT
   MOST ONE attacker" in full; engine applied only B1:A1 (chosen_text "Emrakul blocks Blight
   Mamba"), A2+A3 connected for 5 poison, game lost at 28 life. The legal survival line
   (B1:A3, Skithiryx) was printed and trivial. FIX SHAPE: on a multi-assignment of one blocker,
   REJECT and re-ask with the constraint echoed (or drop only the excess pairs and re-ask),
   never silently keep pair #1. This failure was fully recoverable at the harness level; guide
   now also states the constraint (Rule #9), but a constraint-override at the reply layer needs
   the structural catch.

2. **MEDIUM (harness/API): empty-reply anomaly.** vs152 seq12 t7 (Farseek PUT reveal): reply
   AND reasoning both 0 chars; engine re-asked at seq13 and play continued (seq13's stale PLAN
   text "Pick Sheoldred" during a land search; choice itself fine). Looks like a transport
   drop, not a model decision — excluded from scoring; flag for the runner.

3. **Carried from the tier review (arm A, wave-36 docket, unchanged status):**
   - Damage-assignment-order render defect: identical wrong ability dump on every blocker line
     (A vs36 seq20-22 t11 — "Ornithopter (0/2) [trample, flying, first strike, haste,
     vigilance, protection from red, protection from black]"; real keyword: flying only).
   - Same asks lack a reply-format contract (model guessed between BLOCKS:/CHOICE: syntax —
     guessed right; reply-protocol lane, ships from code next to the parser).
   - Suspected castability omission: A vs105 seq17 t11 — "Mana available: 4", hand Kitchen
     Finks {1}{g/w}{g/w}, cast list offered only Farseek. Needs engine-side verification of
     hybrid-mana castability with the Glimmerpost/Tropical mix; low confidence.
   - Legibility (minor): B vs139 — Amulet destruction surfaced only as the bare log line "Your
     Quicksilver Amulet was put into your graveyard" with no cause; the 122B re-planned
     correctly, a weaker pilot may not.

4. **Observation, no action:** R5 "cast nothing" residue is tier-intrinsic (5 arm-C letter
   violations, all harmless, mostly hold-then-spend-same-turn or lethal-on-board). The guide
   now sanctions main-2 casting and names the observed wrong phrase; if the residue survives
   the next corpus at similar magnitude, accept it as noise — do not escalate wording.
