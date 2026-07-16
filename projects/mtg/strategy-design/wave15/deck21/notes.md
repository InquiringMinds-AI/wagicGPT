# deck21 seat - engine/harness notes ledger + rotation verdict (wave-15)

Layer-routing: engine/card-script/parser items only; no guide prose here.

## Card-script divergences from real MTG Oracle (verify, do NOT block the guide)
Per the wagicgpt-verify-oracle-text discipline: the engine's deck21 cards differ from real
cards. The guide is written to ENGINE reality; these are flagged for a maintainer to check
the `Res/sets/primitives/*.txt` definitions against Gatherer/Scryfall.
- **Scuzzback Scrapper**: engine = `{R/G}` 1/1, wither, no haste, no persist. Real card =
  `{2}{R/G}` 3/1, haste, persist. Both cost and body differ; the missing haste is
  behaviorally load-bearing (it cannot swing the turn it lands - confirmed vs135 s28->s29,
  named as attacker but engine dropped it as ineligible). If the primitive is meant to be the
  real card, it is badly wrong; if this is an intentional custom card, fine - but the guide
  assumes the engine version.
- **Boartusk Liege**: engine = 3/4 WITH trample (and first strike when Bloodmark Mentor is
  out, correctly). Real card = vanilla 4/4, no trample. The anthem effect (other red + other
  green get +1/+1) matches. Body/keyword divergence only.
- **Spark Elemental** `[treason]` tag = its end-of-turn sacrifice (correct behavior, opaque
  label - see general-suggestions #2).

## Fallbacks at this seat (all correct, no wrongful downgrade)
- 5 unparsed_reply (vs135 s5 deferred, s24; vs133 s8; vs140 s35; and one more) - all
  giant reasoning loops correctly routed to heuristic; 0 scavenged casts. Loop-guard working.
- 2 stale_echo, both CORRECT catches (spot-checked per brief):
  - vs135 s10: echo named "Play Mountain", which is NOT in the current cast menu (Volcanic
    Hammer / Spark / nothing) - absent-echo -> heuristic. Correct.
  - vs133 s14: target sub-menu (opponent / Bloodghast / yourself); echo named the PARENT
    action "Cast Volcanic Hammer" (no literal option match) -> heuristic. Index-1 = "The
    opponent" was the intended target, so outcome-neutral (next window cast Hammer at face).
    Correct staleness route, NOT a downgrade of a named offered option.
- 1 deferred_to_heuristic (vs135 s5, empty reply after the s4 2,773-word Spark loop).

## Recurrences corroborating existing wave-13 ledger items
- MIXED-FORM attacker drop: vs135 s29 `ATTACK: A1, A2, Scuzzback Scrapper` - trailing name
  dropped after A-indices. Outcome-neutral (Scuzzback ineligible). Another witness for the
  attackers name->index reconcile arm.
- No new segfaults, no target-cancel crash, no absent-echo wrongful downgrade - the
  aad32bc7f / d0f88326b / 3cb901295 infrastructure held at this seat.

## Latency / adjudication
Two of deck21's three "non-wins" are latency-adjudicated POSITIONS: vs135 adjudicated AHEAD
(17 vs 8 = a win) and vs140 adjudicated BEHIND (13 vs 31 = a loss earned by the hold-burn
durdle, not by the clock). Scored on position merits per the brief, not on the adjudication.

## ROTATION VERDICT
N/A - deck21 is a NEWCOMER this cycle (guideless first pass), receiving its INITIAL guide.
It is not a rotation candidate. It should re-run WITH the wave-15 guide next corpus to
validate F1 (burn-as-clock / burn-can't-kill-X-X), F2 (Spark timing), and F7 (no durdle)
against the same opponents that exposed them (vs140 lifegain, vs135/vs17 walls).
