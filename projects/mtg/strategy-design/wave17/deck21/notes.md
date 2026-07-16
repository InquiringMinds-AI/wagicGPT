# deck21 seat — engine/harness notes ledger + ROTATION verdict (wave-17)

Layer-routing: engine/card-script/parser/construction items only; no guide prose here.

## Combat is ENGINE-AUTO-RESOLVED at this seat (characterization, not a defect).
101 GPT `ask` decisions across 6 games are ALL in Upkeep/Main-1/Main-2. The engine AUTO-declares
attackers (25 `kind:attackers` log records with no GPT prompt, matching the deck's all-out posture)
and AUTO-declines blocks. The GPT is escalated a BLOCK decision ONLY on a would-be-LETHAL swing —
exactly ONCE all corpus (vs62 s25, my5 vs a 5/9 trampler; correct gang-block). This is why "only 1
blocker decision" — it is structural to auto-combat + the default-no-block posture, not a data gap
and not a bug. Consequence worth flagging to the engine owner: because this seat never blocks, EVERY
opposing attacker (ground or flying) connects its full power every turn — the "no-blocker race math"
is a real structural property of the seat, not a flyer-only concern (see findings E1' + the vs49
Outcast ground-engine clock).

## Hybrid-pip annotation — STILL WORKING (3rd corpus of confirmation at a top hybrid seat).
8 hybrid-annotated cast prompts this wave, 0 declines. "[hybrid: each {r/g} pays with R or G - total
N mana]" continues to carry the affordability fact; no wrongful annotation, no double-count, no
over-offer. Ship-confirmed again.

## Binary 7cdcf9c73 held CLEAN at this seat.
0 segfaults, 0 target-cancel crashes, 0 absent-echo wrongful downgrades, 0 stale_echo, 0 attacker
mixed-form drops. 2 fallbacks total (~2% of asks), both benign unparsed_reply (vs35 s17 land drop →
heuristic played the land; vs110 s5 cast ramble → next seq bolted correctly). The wave-16 PLAN-carry
stale-intent caveat and legalCasts/own-target-BAD-effect suppression fixes did not misbehave here
(this seat saw no unpayable-hardcast or own-only-bad-target menus to exercise them). Parser
infrastructure stable.

## Card-script divergences from real MTG Oracle (unchanged from w15/w16 — maintainer verify, do NOT block guide).
Per wagicgpt-verify-oracle-text. Guide is written to ENGINE reality; flag for a maintainer to check
`Res/sets/primitives/*.txt` against Gatherer/Scryfall:
- Scuzzback Scrapper: engine = {R/G} 1/1 wither, NO haste, NO persist. Real card = {2}{R/G} 3/1
  haste persist. Cost AND body differ; missing haste is behaviorally load-bearing. (w15 ledger)
- Boartusk Liege: engine = 3/4 WITH trample. Real card = vanilla 4/4, no trample. Anthem matches;
  body/keyword divergence only. (w15 ledger)
- Spark Elemental "[treason]" tag = its end-of-turn sacrifice (correct behavior, opaque label — see
  general-suggestions #2, still opaque this wave).

## Opponent-side card behavior observed (informational, not a deck21 bug).
- deck110 Affinity: Cranial Plating turned Memnite into a 6/1 (T3) then 13/1 (T5) — power-only pump,
  toughness stays 1 (so burn kills it; deck21 correctly bolted the 6/1 at s7). The structural
  problem is Affinity's redundancy (replays Plating on a fresh Memnite), not a defect.
- deck49 Dragons/RG: Dragonmaster Outcast (1/1) took Goblin War Paint (+2/+2) → 3/3 and anchored the
  lethal clock; option text legibly carried its dragon-token engine line (not truncated).
- deck35 Slivers: a Winged Sliver granted the whole team FLYING and the lords pumped 1/1 → 2/2, so a
  ground race deck21 was winning (my14/op5) flipped to an evasive alpha. Lord-stack behaved as
  expected; noting because a tribal deck that grants evasion+pump late is a structural swing for a
  no-block swarm.

## DECK-CONSTRUCTION item (route to roster owner). See general-suggestions #1.
Two-wave flood pattern (w16 vs133, w17 vs35): close/winnable games lost on creature-light draws.
Lever is construction (trim land / smooth curve), not the guide.

## ROTATION VERDICT: KEEP IN ROTATION — guide REVISED, new-work signal present.
This corpus produced a real, evidence-backed guide revision (E1' — the POWER FLOOR on the race-math
flyer exception, closing the w16 revision's over-application that quoted the guide verbatim and cost
the vs62 loss by 1 life) plus a light kill-on-sight-engine sharpening (vs49). NOT a rotation
candidate:
- Guide was MODIFIED this wave (not byte-identical) = new-work signal by doctrine.
- The revision NEEDS a next corpus to VALIDATE: does the POWER floor stop the 0-power-flyer misfire
  (re-run vs a mana-dork/voltron deck like 62) WITHOUT blunting a legitimate power-1+ flyer pick or
  the fast-race guard (which already works, vs17)? And does the kill-on-sight-engine sharpening make
  the model kill a legibly-rendered token-maker (re-run vs 49-type Dragons)?
- The record floor is STRUCTURAL (110 artifacts wall, 62 voltron, 35 flood variance) with clean core
  execution — NO taught class regressed. Once E1' validates AND the record stabilizes, deck21 is a
  freeze-and-rotate candidate; not yet (the just-added exception has one open over-application fix in
  flight).
