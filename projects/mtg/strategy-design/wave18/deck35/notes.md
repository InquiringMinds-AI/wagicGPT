# deck35 — development notes (engine / harness / model / construction) + rotation verdict

## Engine / representation ledger items
1. **[REPRESENTATION, CROSS-SEAT — now 2-wave convergent; reinforces wave-17 ledger #1] Block-seam
   combat-outcome annotation.** Wave-17 flagged the block-math tax and proposed annotating each
   candidate block with a computed OUTCOME. Wave-18 gives it teeth: at 62 s16 the tax produced the
   corpus's ONLY fallback — an 11,169-char loop that distrusted BOTH the rendered opponent [X/Y] and
   the existing "unblocked, these attackers deal up to N" annotation and re-derived a 10/12 aura
   creature's power from its enchantments until it emitted an unparseable reply (Baka answered).
   The annotation the engine ALREADY provides was CORRECT and would have closed the decision in one
   line; the model would not trust it. Proposed fix (unchanged, now stronger): at the BLOCK seam,
   annotate each candidate block ASSIGNMENT with the computed result — post-block life delta, "you
   survive / you die," blocker dies Y/N, trample-through amount, first-strike ordering — so there is
   nothing left to re-derive. Layer: engine/representation, NOT prose (a guide line can only ask the
   model to trust a number it is choosing to distrust — this wave's revision does that as a stopgap,
   but the durable fix is to remove the re-derivation surface). **Convergence: wave-17 flagged it,
   wave-18 shows it causing a fallback — flag to synthesis as ready-to-build if a second seat's
   block-tax converges.**

2. **[MODEL/ENGINE WATCH — verify, do NOT assert a misplay] Did deck35 get a block decision at 110
   t8?** vs110 ended turn 8 (deck35 -1) with NO `blockers` record on the opponent's lethal turn,
   despite deck35 holding 3/3 flyers at 3 life (some freshly cast = untapped and LEGAL blockers;
   summoning sickness does not stop blocking). Either the model never had the choice (the engine
   auto-resolved, cousin of the Azcanta optional-reveal auto-decline) or the engine correctly found
   no block that changes the result. Probe: confirm a `blockers` decision is ISSUED to the AI seat
   whenever it has >=1 untapped eligible blocker and the incoming attack is lethal. No blockers
   record = the model may never have had the choice; do not score it as a durdle or a misplay.

3. **[MODEL WATCH — carried from wave-17 #2, still unconfirmed] Gemhide auto-tap suppressing
   attackers.** vs135 won at opp -7 but issued only ONE `attackers` decision across a 5-body board
   (t5-t9). No Heart Sliver was drawn (everything summoning-sick the turn it landed) AND two Gemhide
   Slivers were cast/tapped for mana — both suppress attack eligibility, so the low count reconciles
   against the eligible set (Step 0-ter rule 2) and did NOT cost the game. But the pattern "few
   attack steps in Gemhide-heavy games" keeps the wave-17 hypothesis (autoTapForCost prefers a
   Gemhide any-color producer over a land, tapping a would-be attacker) alive. Still worth the
   one-game pinned probe: watch whether autoTap selects Gemhide slivers over lands for colored costs
   and whether that later removes an attacker from the eligible set.

## Deck-construction observations (recorded; not my call to change)
- **Shifting Sliver — the premier win-con — has now gone undeployed for TWO full corpora.** It needs
  {3}{u} and the blue base is only ~5 Islands + Gemhide; whenever blue is scarce it is stranded in
  hand (directly cost the 62 game, where BOTH blue evasion lords sat uncastable while a 10/12 aura
  Spider ran deck35 over). If construction is ever tuned: raise blue sources (or move the
  unblockable/flying evasion off a hard blue pip), and trim the redundant situational lords (3x
  Striking, 3x Spinneret) for more evasion/pump. The guide steers priority meanwhile but cannot draw
  an Island.
- **Both losses were to the pool's two fastest/biggest decks** (110 = Affinity/Cranial-Plating
  lifelink flyer; 62 = Blanchwood/Ancestral-Mask aura giant). A base-1/1 ground swarm is structurally
  soft to a turbo-flyer clock and to a single oversized trampler — not a guide-fixable axis. deck35
  attacked every turn in both; the durdle is gone, these are honest speed/size losses.

## Rotation verdict
**NOT a rotation candidate.** This seat is a FIRST-GUIDED VALIDATION that (a) validated its headline
class DEAD (FC1 durdle: 2/6 zero-attacker games -> 0/6; the instrument's clean win) and the wave-17
multi-anthem-stacking caveat RESOLVED, AND (b) produced a REVISION (surgical two-sided-trust + forced-
lethal-escape clause targeting FC3, now the dominant residual). Per Step 0-ter, a validated guide
that produces a revision carries a new-work signal by construction and is not a rotation candidate.
New work for next corpus is concrete: re-validate the FC3 clause (does the block-seam reply length
collapse? does the unparsed fallback recur?), focused on the has-giant / has-fast-flyer matchups
(62, 110) that exposed the off-case; and the block-outcome annotation (ledger #1) is now a 2-wave
convergent engine build waiting on a second seat's confirmation.
