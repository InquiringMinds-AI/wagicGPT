# deck134 -> strategy-writing-skill proposals (wave 27)

deck134 is the FIRST control / counterspell / tron / ETB-BLINK deck to come through Step-0-bis.
All three proposals are TAGGED SINGLE-SEAT (one seat, this deck) per skill discipline; each is a
new-shape observation, framed as evidence, for the synthesis agent to accept/tag/reject. Note a
likely convergence signal this same wave: deck136 (Yarok of Pranks) is also an ETB/blink deck in
the pool, so proposal 2 may reproduce at that seat.

## Proposal 1 (Step 0-bis, rule 5 family / core-transfer gap): a NON-LAND fetch/sac engine
needs its own NAMED crack line - the core's fetchland rule does NOT transfer to an artifact.
Evidence: Expedition Map ({2},T,Sac: search a LAND to hand) is the deck's tempo lifeline, and the
pilot PASSED the "search card with Expedition Map" activation at a bare priority window ~10x in
one game (vs18 seq5,7,8,9,10,13,14,15,16,19 - never cracked it), and delayed it ~6 turns in two
others (vs93 crack T12; vs131 crack T17). The core prompt DOES carry "sacrifice a fetchland the
first time its activation is offered" and "activate your permanents' abilities" - but the pilot
did not map an ARTIFACT-that-sacrifices-for-a-land onto "fetchland," so the core line never fired.
Proposed rung: when a deck runs a sac-for-value / crack-to-search ARTIFACT (Expedition Map,
Armillary Sphere, Ichor Wellspring class), write a card-named "take the crack, here is what to
fetch" rule #1 even though core covers fetchlands; a bare priority window whose only action is a
self-sac activation reads to the executor as "pass by default." The corrective is option-anchored
(name the exact "search card with X" string) + a fetch-target order + a closed pass-exception.

## Proposal 2 (Step 0-bis, NEW engine shape): the ETB-REUSE / BLINK engine - teach (a) blink
targets by ETB value, (b) the repeated per-turn sequence, (c) NEVER blink a bare land.
Evidence: deck134's real anti-aggro win is Stonehorn Dignitary (ETB: opponent skips their next
combat) + Ghostly Flicker (blink two of your permanents, re-firing ETBs) = re-blink Stonehorn
every turn -> opponent never attacks. The pilot NEVER assembled it (vs136 cast Stonehorn seq25
then ATTACKED with the 1/4 for 1 and held Flicker seq27/29). And when it did cast Flicker it
mis-targeted onto LANDS (vs93 seq46/47 blinked Urza's Tower + Unknown Shores = dead; vs136 seq16
blinked Wall + a land) - it only blinked value once (vs131 seq32/33, two Prophetic Prisms = draw
2). A static "Ghostly Flicker: blink two permanents" capability line does NOT produce this - the
weak executor needs (a) an explicit blink-TARGET priority list (ETB value permanents named,
"never a land"), and (b) the engine written as a REPEATED per-turn sequence ("blink Stonehorn
EVERY turn"), the same way rule 5 (wave 16, deck62) generalized "write the floor on the first
pass" from ATTACKING to SEQUENCING. Extends that rung from a one-time sequence to a REPEATED
per-turn loop. Watch deck136 (Yarok) for convergence: if a second blink deck mis-targets or
whiffs the loop, promote to a named rung.

## Proposal 3 (Step 0-bis, rule-3-adjacent / alt-cost face): an EVOKE / alt-cost option that
buys a discount by DISCARDING THE BODY is a STOP-GATE under aggro - hard-cast for the blocker.
Evidence: Mulldrifter offers hard-cast {4}{U} (2/2 flyer + draw 2) AND evoke {2}{U} (draw 2 then
sacrifice, NO body). At 7 life vs convoke aggro the pilot chose evoke (vs137 seq15), throwing
away the flying blocker it needed; evoked again at 13 life vs burn (vs131 seq12/13). This is
adjacent to the FIFTH idiosyncrasy face (deck137 adventure alt-cost) but distinct: both options
deliver the SAME draw, and the alt-cost simply removes the body, so it is not a mislabel - it is
a value-vs-tempo mis-weight the executor resolves toward the cheaper number. Guide corrective is a
board-state gate ("hard-cast when you need a body / are under pressure; evoke only when safe or
short on mana"). Likely guide-only (card-specific), but flag: if a second evoke/alt-cost-drops-
the-body card recurs, a general "an alt-cost that discards the permanent is a body-vs-cards gate,
not a free discount" rung is warranted.

## Everything else: the skill worked as written.
Step-0-bis rules 1-6 all applied cleanly (engine-derived card facts; reasoning-tax spikes ranked
the teaching points - the top tax windows were exactly the Map crack, the Flicker targets, and the
low-life survival casts; rule 3 "dominant idiosyncrasy = rule #1" put the Map crack above the win
engine; rule 4 countermand-a-core-reflex was NOT needed (no core line was actively anti-teaching
here - the deck's leaks are missing deck-knowledge, not a wrong core reflex); rule 5 attack/deploy
floor was inverted into "do NOT attack with the lock piece"). Density landed at the top of the band
(76 lines) - justified by the capability table + lock + survival sections, per rule 6.
