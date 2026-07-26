# R-DFC-BACKFACE-RESOLVES-FRONT - engine fix (wave-27 step-1)

Fix on HEAD 774d3a039 (worktree agent-a7039c6029d03e08e). One file: src/MTGRules.cpp
(MTGPutInPlayRule::reactToClick, +26 lines). Uncommitted per wave protocol.

## Bug
Casting the flipped BACK face of a Kaldheim modal-DFC god (Tergrid, God of Fright //
Tergrid's Lantern) via "Cast Card Normally" paid the back's {3}{B} but resolved the
{3}{B}{B} FRONT God - pay less, get strictly more.

Root chain (ground-truth captured this session via WAGIC_DFC_PROBE instrumentation):
1. The AI flip machinery DESYNCS card->isFlipped from the displayed face. The flip-thrash
   trace shows AATurnSide::resolve entered repeatedly with the flag and the shown face out
   of agreement (AIPlayerBaka OrderedAIAction::getEfficiency std::rand toggle of
   source->isFlipped + the AATurnSide AI-reset at AllAbilities.cpp:5421 that clears the flag
   without reverting the shown face).
2. With the display flipped to the Lantern (name/cost {3}{B}) but the flag reading front
   (isFlipped==0), the front cast restriction compare(isflipped)~equalto~0 wrongly PASSES,
   so MTGPutInPlayRule offers "Cast Card Normally" for the reduced cost. A normal cast never
   sets paid(alternative), so the autostack "if paid(alternative) then flip(...)" never fires
   and the FRONT base face resolves.

Cast-time state proven identical in both counterfactual runs (see counterfactual_*.txt):
  MTGPutInPlayRule::reactToClick name='Tergrid's Lantern' modelName='Tergrid, God of Fright'
  isFlipped=0 nameOrig='Tergrid, God of Fright' altHasAlt=1 altPaid=0

## Fix (candidate a - cast-path reroute, in scope)
In MTGPutInPlayRule::reactToClick, after payment and before the spell reaches the stack:
when the card's FRONT primitive carries an other= alternative cost AND a `stack` autostack
containing flip(, the card is DISPLAYING a non-front face (card->name != model->data->name),
and the paid cost is the reduced back cost (CMC <= the front's alternative CMC) - stamp
alternateCostPaid[MANA_PAID_WITH_ALTERNATIVE]=1 and payResult=MANA_PAID_WITH_ALTERNATIVE.
The autostack flip then fires and the DISPLAYED (back) face resolves; paid cost always
matches the resolved face. Generalizes to the whole 11-card Kaldheim-god modal-DFC class
(borderline.txt: Birgi/Cosima/Egon/Esika/Halvar/Jorn/Kolvori/Reidane/Toralf/Valki/Tergrid).

Desync-proof: keys on the displayed NAME/COST (set by the display flip), never on the
corruptible isFlipped flag. Chosen over candidate (c) (gating AllAbilities.cpp:5421) because
that line is shared by all DFC/transform/werewolf flips and the desync's primary source is
the AI scorer (AIPlayerBaka) - both outside the cast-path scope and higher regression risk.

## Does NOT change
Front-face normal casts (name==front primitive name -> predicate false); the alternative-cost
cast path (untouched); transform/flip triggers on the battlefield (AAFlip path; PutInPlay only
fires off-battlefield); non-modal-DFC cards (predicate requires the other=+autostack-flip shape).

## Validation
- Gates (fixed binary, WAGIC_AI=baka, single-thread): PARSETEST 99/99; main 1027 tests + 11 AI
  (0 failed); lexicon 819 (0 failed).
- Witnesses (clean final binary): tergrid_lantern_altcost_wave24 PASS (milestone preserved) +
  tergrid_steals_sacrificed_creature / tergrid_steals_discarded_permanent /
  tergrid_ignores_sacrificed_token PASS. (effect_doubleside_turns_back_face fails only in a
  SCOPED run on BOTH baseline and fixed binaries - a synthetic-primitive registration artifact
  of isolated runs; it passes in the full suite.)
- Direct suite fixture for the BUG: structurally blocked (confirmed) - the trigger state
  (display flipped to back while isFlipped==0) is an AI-runtime desync a scripted sequence
  cannot create, and MODE_TEST_SUITE keeps the front MTGId (AATurnSide/AAFlip setMTGId gate).
- DETERMINISTIC counterfactual (temporary env-gated WAGIC_DFC_DESYNC hook + WAGIC_DFC_NOFIX
  gate, both REMOVED before finish; same binary, same fixture, same desync state):
    FIX OFF -> "Card ID not the same. Didn't find 503722" (Lantern absent; God 503721 resolved) = BUG
    FIX ON  -> Test Successful (Tergrid's Lantern resolves)
  Files: counterfactual_FIX_OFF.txt / counterfactual_FIX_ON.txt.
- LIVE probe: 1 GPT-vs-GPT game (deck199 Tergrid vs deck198), fixed binary. UNOBSERVED for a
  completed back-face cast: the game LIVELOCKED at the flip-thrash (AI oscillates isFlipped via
  the std::rand hack + AATurnSide reset, never completing a cast; reached turn 9 in ~50 min then
  stalled on stale async answers). This livelock is upstream of casting and independent of this
  fix. Trace: live_g1_flipthrash_livelock.txt. The deterministic counterfactual is the primary
  proof; the specific reroute firing in a completed live game is UNOBSERVED.

## Ledger note (out of scope, flagged)
Kaldheim-god DFCs FLIP-THRASH-LIVELOCK live GPT games: AIPlayerBaka::getEfficiency (line ~642)
toggles source->isFlipped via std::rand as a scorer side effect, and AATurnSide::resolve's AI
branch (AllAbilities.cpp:5421) clears the flag without reverting the display. Together they let
the AI oscillate isFlipped indefinitely without ever flipping the display or casting. This is the
desync SOURCE and blocks live validation of the Tergrid deck; belongs to the AI-scorer / shared
AATurnSide owners.
