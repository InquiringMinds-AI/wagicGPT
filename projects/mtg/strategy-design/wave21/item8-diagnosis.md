# Wave-21 ledger item 8 — repeated cast-offer of an already-cast singleton: DIAGNOSED

Status: root-caused from wave-20 translogs + code read (2026-07-23). Fix layers (a) and (b) below
SHIPPED in the wave-21 engine batch; layer (c) remains OPEN (needs a discriminating probe). The
"repeated offer" framing was wrong — **the cast never happens**. The option re-arms because the
spell is still in hand and still "castable" by the oracle's (miscounted) math.

## The signature (both corpus instances)

deck102 vs deck135, `1784495813-ai_baka_deck102-...jsonl` s20/s21 (same main phase!) + s24:
- s20: hand = Witch of the Moors {3}{b}{b}; "Mana available: {b}{b}{b}{b}{b} from 5 untapped sources".
  Model picks Cast Witch.
- s21 (same phase, re-ask): **Witch still in hand, NO `hand -> stack` event, `{b}{b}{b}{b}` floating
  in pool, one "untapped source" left = Leaden Myr `{attached: Gelid Shackles}` rendered
  `[noactivatedability]`.** The plan tapped the 4 real sources, then the Myr click was refused, the
  cast click failed on affordability, and 4 mana floated. Pool(4) + Myr(1) still "affords" 5 → the
  cast is re-offered → the model re-picks it. Wasted decisions + floated mana + no cast.

deck102 vs deck62, `1784494237-...jsonl` s5/s6: same shape at 2 mana — Dreadhorde Invasion {1}{b},
Swamp tapped + `{b}` floating, Leaden Myr (turn-2 cast, NOT sick, NOT disabled) left untapped, no cast
event. The refusing condition here is NOT NOACTIVATED — see "game-2 residual" below.

## Root-cause chain (code)

0. The click path is `AIAction::Act` → `GameObserver::cardClick(card, ability)` (GameObserver.cpp:1576)
   → `AManaProducer::reactToClick` (MTGAbility.cpp:8825), which requires BOTH
   `AManaProducer::isReactingToClick` AND the full base `ActivatedAbility::isReactingToClick`
   (MTGAbility.cpp:8829 — added guard). The base check is where the refusals live.
1. `producerUsable` (src/ManaEngine.cpp:38) gates what `potentialMana`/`planPayment` count. It checks
   tapped/sickness/in-play/phased/castRestriction/cost — but NOT the click layer's refusal conditions
   in `ActivatedAbility::isReactingToClick` (src/MTGAbility.cpp:7747-7768): `Constants::NOACTIVATED`
   (line 7747 — THIS is game 1's confirmed refusal for the Shackled Myr), mutation-parent, `NOMANA`
   (for mana producers), `NOACTIVATEDTAP`+TapCost. A disabled producer is therefore counted for
   castability (legalCasts), rendered in "Mana available", and planned as a payment source — but its
   click is refused at activation time. Additional count-time hole: `TapCost::isPaymentSet`
   (ExtraCost.cpp:744) passes when its `source` is unset, so `extraCosts->canPay()` in the counting
   path can vacuously succeed.
2. `DecisionManager::planCastSpell` (src/DecisionContract.cpp:387) validates plan SHAPE (producer
   types) but not clickability; the consumer (src/AIPlayerBaka.cpp:3304-3314) enqueues
   producer-clicks + cast-click on the one-per-tick clickstream with **no verification that each
   click landed**. A refused producer click silently degrades into: tap the others, fail the cast,
   float the mana.
3. Nothing marks the failed attempt, so the (still miscounted) oracle re-offers the same cast on the
   next poll — the model re-picks it, loops.

## Fix layers

- (a) **Counting fix (small, do first):** `producerUsable` must mirror the click layer's refusals:
  refuse when `source->has(Constants::NOACTIVATED)`, when the producer is a mana producer and
  `source->has(Constants::NOMANA)`, when `source->has(Constants::NOACTIVATEDTAP)` and the ability
  cost carries a TapCost, and when `source->mutation && source->parentCards.size()`. Fixes game 1's
  class at the root: disabled producers stop being counted → Witch stops being offered at 4-real
  sources → no stall, no float, no repeat-offer.
- (b) **Clickstream robustness (structural):** producer AIActions should verify the click landed
  (probe `isReactingToClick` at click time); on refusal, abort the REMAINING plan including the cast
  click instead of blindly continuing (prevents the float for any future counted-but-unclickable
  condition).
- (c) **Game-2 residual (open):** the vs62 Myr was untapped/unsick/undisabled yet never got clicked.
  Two candidate mechanisms, both plausible from code:
  (i) `ActivatedAbility::isReactingToClick` line 7719 requires `source->controller() ==
  game->currentlyActing()` — if an interrupt/response window shifted the acting player between
  clickstream ticks, the producer click is refused even though the producer itself is fine.
  (ii) Stored `MTGAbility*` producer pointers going STALE between the planning tick and the click
  tick (the ability-GC latent the one-click-per-tick cadence was built around — see planCastSpell
  header note); hardening = re-resolve the producer ability from `manaObjects` BY SOURCE CARD at
  click time. Needs a deterministic probe (stub endpoint + pinned deck with a mana creature) to
  discriminate before shipping a fix. Note the landed-verification in (b) converts EITHER mechanism
  from "float mana + loop" into a clean abort, which is why (b) is worth shipping even before (c)
  is discriminated.
