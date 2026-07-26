# deck122 -> development notes (engine / harness / representation) -- wave 29

Step-0 read of the FIRST alt-win-condition deck. Corpus 50eeed8be, matchups-20260726-115018.
deck122 seats: ZERO fallbacks, no parser contradictions, no crashes, no new decision-class
defects. The lifegain deck, Sorin loyalty, may-triggers, and forced-exile all ran clean.

## N-122a (NEW, ENGINE-CONFIRMED / LIVE-UNVALIDATED) -- Felidar Sovereign 40-life alt-win.

The engine IMPLEMENTS the upkeep 40+-life win check correctly, confirmed by code:
- primitives/mtg.txt:39654 `auto=@each my upkeep:this(controllerlife > 39) wingame`
  (oracle-faithful: own upkeep, 40+).
- `wingame` -> AAWinGame (MTGAbility.cpp:4915). AAWinGame::resolve (AllAbilities.cpp:7590)
  sets the opponent as loser, honoring CANTLOSE (opp board) and CANTWIN (own board).
- `wingame` is reused by 14 primitive cards (Coalition Victory / Test of Endurance class),
  so the keyword path is battle-tested; GameStateDuel.cpp:2227/2233 consume the win.

BUT this alt-win CLASS has never been validated in live play in the campaign, and was not
exercised this corpus: max deck122 life = 31; Felidar never resolved to the battlefield
(drawn vs93/vs137, game ended before 6 mana + a safe window; BOTTOMED on the mulligan
vs148). RECOMMEND a probe-deck live validation (the proven pattern): write a temporary
Res/ai/baka/deck199.txt stacking Felidar Sovereign + Soul Warden x4 + Lone Missionary +
Kabira Crossroads x4 + Kitchen Finks + Serra Ascendant (fast, redundant lifegain), pin one
selfplay game (WAGIC_SELFPLAY_DECK0/1), and confirm the upkeep trigger fires wingame at
40+ life and the game ends with the Felidar controller as winner. Delete the probe deck
after. This gives the alt-win-condition class its first live confirmation and is a between-
wave engine task, not guide work.

## POSITIVE representation finding (RECORD, no work) -- conditional-static P/T renders live.

The board/hand render shows the LIVE effective P/T and conferred keywords for conditional-
static boosts, with a `(printed X/Y)` annotation:
- Serra Ascendant vs93 seq14 @31 life: `(6/6) (printed 1/1) [flying, lifelink]`; renders
  `(1/1)` below 30.
- Divinity of Pride vs148 @26 life: `(8/8) (printed 4/4) [flying, lifelink]`.
This closes the perception question for the life-threshold-anthem class: the model CAN see
the boosted stats, so threshold play is a GUIDE strategy teach (routed to strategy.txt), not
a representation annotation request. Same annotation family as Kitchen Finks
`(2/1) (printed 3/2)` under a persist counter. No engine change needed; recorded so a future
wave does not re-open it as a suspected perception leak.

## Fallback ownership (per brief item 5)

deck122 owns NONE of the 3 unassigned non-defer fallbacks. All 6 deck122 seats had zero
fallbacks (no priority retracted_choice, no priority unparsed_reply, no blockers
truncated_abandoned). The 2 priority fallbacks + 1 truncated_abandoned belong to other
seats.

## f-batch steady state (deck122 view)

Nothing in the f1-f6 batch is deck122-relevant (no convoke, no duals-collapse cast window
observed as a problem, no summoning-sick attack over-listing seen in the clean attacker
bundles, no dungeon, no DFC). deck122 ran without exercising any f-batch surface -- reported
as a null observation, not a validation.
