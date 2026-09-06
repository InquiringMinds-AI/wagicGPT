# deck130 (Budde's Ponza) guide rewrite — Amendment 332

## (a) Rules KEPT (condition -> action; every rule the live guide stated survives)
1. Deck identity: LD + removal, not burn; 11 total burn damage; every turn has a job.
2. Cast order entries 1-7 (Starstorm / single-target damage / land-kill / body / Blastminer-Talisman
   / cycle / cast nothing), first match wins, ask again after.
3. Land-count curve 2/3/4/5/6+.
4. Land drop before anything, incl. before any cycle; Forgotten Cave enters tapped.
5. Upkeep = pass unless the row destroys a land, kills a creature, or is the Hammer return.
6. Draw-punisher N test: N=1 & life>5 may cycle; N>=2 or N >= life-5 -> no optional draw.
7. Draw step is mandatory; no row skips it; "DRAW FORECAST" is not an offer.
8. Punishers punish draws only; "[castable now]" bodies are free; kill the punisher (Fate Unraveler
   3/4 needs Starstorm X=4).
9. Cycling table: Forgotten Cave, Spark Spray (DIES-row test), Lay Waste (mana-line gate, never
   twice a turn), Starstorm (0-creatures release, Cave first).
10. X=0 is not cycling; "Decline - do nothing" is the exit from a mis-entered option menu.
11. Land plan over at 10+ mana vs 5+ of their lands -> cycle.
12. Targeting order 0-4 (lethal / planeswalker or punisher / reachable toughness / recurring
    attacker / face at 6 or less), kill-count counts creatures only, repeatable-cost arithmetic.
13. Row verdict outranks the plan's verb; SURVIVES(loyalty) is a take; post-combat flip; Spellbomb
    stays loaded.
14. 6-life floor release: 0 creatures AND <=2 lands AND a resolved land-kill of yours.
15. Lifegain trend latch; Sanguine Bond + Exquisite Blood = do not deploy at all.
16. Damage spells never target lands; never a [your battlefield] row; never "Yourself".
17. Starstorm: decide WHETHER at the cast row (the X menu's Decline strands the paid mana); (a)
    YOURS names Rorix/Siege-Gang; (b) defenders-only THEIRS + grounded-attacker carve-out; X=0 and
    "nothing dies" annotations; survival override; PLAN template; marked-row rule; walk down
    "same kills" runs; never a "kills THEIRS: none" row.
18. Land plan: two numbers before every cast; collapsed handles count in full; dead at N>=5 or
    L<=8; ends when they are in damage range; prefer nonbasics.
19. Card facts: Blastminer (nonbasic only, untapped, gates, punisher-body override), Talisman life
    cost + 3-life rule, Siege-Gang four bodies + sacrifice use, Rorix legend rule, Hammer loop cost
    + EMPTY-HAND rule + pass-with-a-body-in-hand, Spellbomb, play-around-counters ordering.
20. Combat: race check + carve-out; wall check; attack floor incl. Blastminer case and send-all;
    blocker tags both forms + gang block; crack-back exception; post-sweep one-row list.
21. Blocking: kill-and-live, Goblin (both die), never Commander/Rorix except vs a token-maker; one
    blocker per attacker; "you would be at N" bands; LETHAL narrowing + block the largest deals N;
    gang block on a punisher/only attacker.
22. Mulligan keeps/ships by land+spell count and the coverage line; bottoming preference order;
    cleanup discard order and prohibitions.
23. Plan line must name a next action; hold row at the first ask of the turn.

## (b) DROPPED, by category (approx. bytes of the 71,227-byte original)
- Game/record narration and counts of past behaviour ("this deck cast/declined/answered ...",
  window counts, turn-by-turn stories used as proof): ~26,000 B.
- Citations of runs, matchups, deck-vs-deck identifiers, and "last corpus"-style framing: ~4,500 B.
- Repetition of the same rule in three or four places (rule body + card section + situations list
  + cast-order entry), collapsed to one statement each: ~13,000 B.
- Rhetorical/forbidden-thought commentary and meta-commentary about the guide itself: ~4,000 B.
- Over-quoted prompt strings trimmed to the fragment the rule keys on: ~3,000 B.
- Situations list cut from ~35 entries to 5, each restating a rule already stated above: ~2,700 B.

## (c) Rules that could NOT be restated as condition -> action
- None dropped for that reason. Two were rewritten to route rather than repeat: cast-order entry 1
  now points at STARSTORM'S X, and entry 3 at THE LAND PLAN, because both carried a full copy of
  the test they gate.

## (d) Verification
- `stat -c %s strategy.txt` = **19996** (<= 20000).
- `/usr/bin/grep -ciE 'corpus|wave[- ]?[0-9]|\bseq\b|previous game|last wave|this wave|census|paid (for )?by|[0-9]{3}v[0-9]{3}|you (declined|took|cast|passed) [0-9]'` = **0**.
