# General suggestions (wave 15) — deck110 seat

No new CORE prompt-body candidate from the deck110 seat (core PASS, 11th consecutive wave). Guide
FROZEN (8th freeze). The wave-15 items route to ENGINE/CARD-SCRIPT (now RESOLVED), the
attackers-parser (RESOLVED), representation, and MODEL layers. Full repros in `notes.md`.

## Shipped-fix validations (cross-deck, close the ledger items)

1. **[RESOLVED / HIGH] Galvanic Blast metalcraft now delivers 4 at 3+ artifacts, 2 at <=2.** The
   wave-13 flat-2 bug (`3cb901295` fix) is confirmed CORRECT on both arms at this seat and it
   improved play: deck133 flipped loss->win and the model fired the Blast at the FACE for the guide's
   intended 4-reach at the exact spot where wave-13 committed its Obliterator creature-burn blunder.
   Cross-deck to any Galvanic Blast / metalcraft-oneshot card — those should be re-audited too, but
   deck110's finisher is CLOSED. NOT a guide edit (guide was Oracle-correct throughout).

2. **[RESOLVED] Attackers name->index reconcile (`d0f88326b`) works.** No eligible attacker dropped
   across 20 windows; MIXED and FULL-NAME forms resolve to the correct eligible set; ineligible
   (summoning-sick) names correctly excluded. The wave-13 MIXED-form silent-drop is closed at this
   seat. NOT a guide edit (the model NAMES every attacker; the parser now unions them).

## Engine / representation ledger (cross-deck relevance, still open)

3. **[MEDIUM] Equip re-offer no-op marker should be computed from OUTCOME DELTA, not target identity
   (E2, unfixed).** RECURRED this wave as a CLEANER, fully-executed A->B->A Plating bounce
   (deck135 s20->s21, 2 wasted mana, non-outcome): the model moved its only Plating OFF the best
   carrier (7/2 protection Etched Champion) onto a 0-power Signal Pest, then moved it back. One
   predicate — mark/suppress "Equip ... targeting X" whenever the move would not raise X's resulting
   power above its current carrier's — suppresses both the equivalent-carrier bounce and the
   downgrade-move. Owner: `AIPlayerGPT.cpp` equip-option serialization.

4. **[LOW-MED / representation] The "Cast Card Normally with Mox Opal" option label is a confusion
   trap — it shows Mox Opal's OWN ability text, not the card/action it represents.** Two witnesses in
   one game (deck44 s5 -> 5.4k-token confusion spiral -> stale_echo fallback; deck44 s30 -> "This is
   confusing. Let me assume it's a mistake and pass" -> unparsed_reply). The model correctly deduces
   the intent ("play Glimmervoid" / tap for mana) but burns huge reasoning doing it, and under the
   latency cap that risks a timeout. The guide already tells the model these lines "just make mana
   with Mox Opal," so this is a REPRESENTATION fix, not a guide one: the option should name the CARD
   being cast (or the action), not echo Mox Opal's `{Metalcraft - {T}: Add one mana...}` text. Owner:
   `AIPlayerGPT.cpp` option serialization for cast-with-mana-source lines. Cross-deck (any deck with
   Mox Opal / mana-rock cast-with options).

## Model-layer items (not guide-fixable; cross-deck)

5. **[MODEL WATCH / NEW] Summoning-sickness is not tracked — the model names freshly-cast creatures
   as attackers and counts them in lethal/damage math.** 5 witnesses (deck140 s9, deck135 s11,
   deck133 s17, deck17 s24/s32). Harmless this corpus (engine roster + reconcile eligibility filter
   exclude the ineligible names; every window was non-lethal), but the model's OWN reach arithmetic
   is inflated — latent risk that it mis-judges a lethal or fails to hold a blocker on the belief a
   summoning-sick body attacks. Cheap harness mitigation if it ever bites: annotate freshly-entered
   creatures in the board line as `[can't attack yet — summoning sick]`. Synthesis question: do other
   aggro seats inflate lethal math with summoning-sick creatures?

6. **Blocker over-block while ahead (deck21 s6, s23) — cross-seat SYNTHESIS question, LOW stakes this
   wave.** The "must remove their creature" instinct recurs (traded an evasive lifelink flier for a
   ground 1/1 at 18; took 2 permanent wither counters off a 12/6 to kill a 2/1 at 9) but both were
   minor/defensible and non-outcome — nothing like wave-13's costly 10-power-clock throwaway. The
   prompt annotated the correct line in both. Keep the cross-seat question open (do aggro seats
   over-block on defense); deck110's contribution is "recurs, low stakes." If it ever hardens
   cross-seat, sharpen the block-value prompt annotation, NOT a per-deck guide line.

## Deck construction (not a guide task)
7. **deck110's loss column is still owned by draw variance / matchup, not obedience.** The single loss
   (vs140) was Lightmine Field + board-wipes with ZERO Galvanic Blast drawn = no legal path to
   damage; the model played the Lightmine line correctly and got ground out. Standing DC1; if ever
   tuned, trim ~0-power filler + Mox/land toward mid bodies + reach/evasion redundancy. Not a wave
   task.
