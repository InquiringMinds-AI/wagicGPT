# deck21 seat — engine/harness notes ledger + ROTATION verdict (wave-18)

Layer-routing: engine/card-script/parser/construction items only; no guide prose here.

## Binary f664539a3 held CLEAN at this seat.
0 segfaults, 0 target-cancel crashes, 0 absent-echo wrongful downgrades, 0 attacker mixed-form
drops. 4 fallbacks / 119 asks (3.4%): vs110 s30 unparsed_reply + s31 stale_echo (both T15, opp
already at 2, game won that turn); vs135 s16 + s18 unparsed_reply (T7/T9, opp 7→4, won 20-0). All
four benign, all in already-won games, heuristic covered. The wave-17 engine batch (TARGET CHOICE
sub-menu framing, hand-card PUT_INTO_PLAY dead-end filter, opponent-only-GOOD suppression, BLOCKS
paraphrase catch, "(player, life N)" strip) did not misbehave here. TARGET CHOICE framing worked
cleanly across this seat's many target decisions (every burn target correctly resolved to face or
to the intended engine/lifelink body — see findings). No stale_echo except the one benign s31.

## ENGINE ITEM (cross-seat): the "giant-reply / computed-P/T-distrust" pocket — all 4 fallbacks.
All four deck21 unparsed/stale fallbacks are the SAME shape and it is NOT a stale-plan spiral: the
model over-narrates when it is analyzing a high-toughness opposing body it cannot kill (the vs135
6/6→8/8 Abominable Treefolk; its own mana/hand state at vs110 T15) and runs past the "CHOICE:"
line. The REASONING in each is correct ("can't kill the wall, race the face") — only the emission
fails. This matches the deck62 seat's 5-unparsed tax (computed-P/T distrust). Because the content
is right and only the format breaks, this is an engine/parser opportunity, not a guide fix:
options (a) a soft reply-length nudge / second-pass CHOICE re-prompt when a reply parses no CHOICE
line but names a legal option; (b) tolerate a trailing "attack with X for lethal"-style plan
without a CHOICE line when only one non-pass option exists. Routed to engine owner; spans ≥2 seats.

## VALIDATION CONFIRMATIONS this corpus (engine-side observations, informational).
- **Power-floor firing** relied on the option line correctly rendering "Birds of Paradise" (0/1)
  and "Argothian Enchantress" (0/1) as legal burn targets WITH their P/T — the model read power=0
  off the rendered body and declined. Target-menu P/T rendering is load-bearing to the guide and
  worked (vs62 s6/s10/s17). No wrongful power display.
- **Kill-on-sight firing** relied on the opponent-battlefield option text carrying the engine line
  un-truncated: Dragonmaster Outcast's "put a 5/5 red Dragon creature token…" (vs49 s7), Vault
  Skirge's "Flying -- Lifelink" (vs110 s13), Signal Pest's "Battle cry (…)" (vs110 s4) all rendered
  legibly and the model acted on them. Option-text engine rendering is correct and load-bearing.

## Hybrid-pip annotation — STILL WORKING (4th corpus of confirmation at a top hybrid seat).
~12 hybrid-annotated cast prompts this wave, 0 declines. "[hybrid: each {r/g} pays with R or G -
total N mana]" continues to carry affordability; no wrongful annotation, no double-count. Ship-confirmed.

## Card-script divergences from real MTG Oracle (unchanged from w15/w16/w17 — maintainer verify, do NOT block guide).
Per wagicgpt-verify-oracle-text. Guide is written to ENGINE reality; flag for a maintainer to check
`Res/sets/primitives/*.txt` against Gatherer/Scryfall:
- Scuzzback Scrapper: engine = {R/G} 1/1 wither, NO haste, NO persist. Real card = {2}{R/G} 3/1
  haste persist. Cost AND body differ; missing haste is behaviorally load-bearing. (w15 ledger)
- Boartusk Liege: engine = 3/4 WITH trample. Real card = vanilla 4/4, no trample. (w15 ledger)
- Spark Elemental "[treason]"/EOT-sacrifice label still opaque (behavior correct).
- NEW opponent-side observations (informational, not deck21 bugs): deck35 Horned Sliver rendered
  "(printed 2/2)" pumped to 3/3 then 5/3 with [trample, first strike] via lord-stack — behaved as
  expected; a tribal deck granting first-strike+trample+width late is a structural swing for a
  no-block swarm (same class flagged w17). deck14 Deep Blue: Lord of Atlantis "(printed 2/2)"
  correctly shrunk to 1/1 by deck21's WITHER combat damage (-1/-1 counters persisted) — wither
  interaction rendered and applied correctly on blocks (vs14 s28/s32).

## DECK-CONSTRUCTION item (route to roster owner). See general-suggestions #1.
THREE-wave flood pattern (w16 vs133, w17 vs35, w18 vs35 + vs49): both losses this wave were 0-creature
back-halves. With the guide now converged, construction (trim land / add cheap redundancy) is the
last lever on this seat's record. Guide already deploys optimally when a body exists.

## ROTATION VERDICT: ROTATE OUT — CONVERGED. Guide FROZEN byte-identical; both open revisions VALIDATED; no new-work signal.
deck21 is a freeze-and-rotate candidate this wave — the SECOND converged exemplar after deck17
(rotated w18). The wave-17 notes set the exact bar: *"Once E1' validates AND the record stabilizes,
deck21 is a freeze-and-rotate candidate."* Both conditions are now met:
- **Guide FROZEN byte-identical** to wave-17 (diff-confirmed vs both the wave-17 file and the live
  Res guide) = no new-work signal by doctrine.
- **Both open revisions VALIDATED on their motivating matchups:** the POWER FLOOR closed the
  0-power-flyer misfire on the SAME vs62/Birds-of-Paradise pairing (flip: LOST −1 → WON +12, model
  quoting the clause), and the kill-on-sight-ENGINE sharpening made the model kill the Dragonmaster
  Outcast on sight vs49 (the exact w17 miss) plus two correct engine/lifelink kills vs110 (flip:
  structural LOSS → WIN). No taught class regressed; no class mutated.
- **Record floor is STRUCTURAL** (Slivers-scaling variance, creature-light flood, control-stalemate
  draw) with clean, now-fully-validated execution — no decision-driven loss remains.
Remaining levers are NOT guide levers: (a) construction (the flood pattern, general-suggestions #1),
(b) the cross-seat giant-reply engine pocket (above). Both routed off the guide layer. deck21 has
no open guide work; rotate it out and, if a length trim is desired, schedule it as a DEDICATED,
separately-validated pass (general-suggestions #2 / skill.md #4), not as a reopening of this seat.
