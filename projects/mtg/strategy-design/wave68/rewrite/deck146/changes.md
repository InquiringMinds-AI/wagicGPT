# deck146 (Orzhov Dungeons) guide rewrite — Amendment 332

## (a) Rules KEPT (restated as condition -> action)
1. Cast order entries 1-9 incl. 8b, first-match-and-stop, "cast nothing" is last not a tiebreaker.
2. Row not engine-marked dead -> the reason it does not match must be printed on this screen; take the ask again (more than one spell a turn); a venture creature never outranks a planeswalker.
5. Verse: monocolored only, COLOR-not-type test, target list is the only truth, "YOUR OWN" -> do not cast, exile is not a bounce.
6. Verse dead vs all-colorless boards -> win with bodies/Soul Shatter/Kaya.
7. Soul Shatter: highest MV, no target, takes EXACTLY ONE of the tied list; dead with no creature/walker.
8. Soul Shatter leaves cast-order entry 1 at >~3 tied names.
9. Kaya -3 exiles any nonland permanent; target = biggest body that will connect; all-"[your battlefield]" -> +1 or pass; never -3 your own.
10. Command: cast row's live/dead clause governs; mode ask re-lists dead pairs unmarked.
11. Command pump only in MAIN 1; no main-2 pump, no "pump now attack next turn".
12. Command target = your own creature.
13. Command draw direction = yourself, opponent only for lethal.
14. Silencer naming order (1)(2)(3) incl. counterspell from "was COUNTERED by".
15. Pelakka Predation: MV>=3 only; not a play when their creature count is 3+ above yours.
16. Emeria's Call indestructible rider; never cast Agadeem's Awakening; colour never blocks a listed cast; land every turn; Snarl/Hive tapped conditions.
19. Three land-backed cards are LANDS; play-as-land conditions.
20. Header counts land-backed cards twice; land number true, spell number high by one; back-face colour.
21. 3-life menu: answer "tap" by default, pay only on (a)+(b); never at 3 life or less; Caverns exempt.
22. Hive CHECK 0 (write sum + their life), 0b (only clock), 1 (upkeep), 2 (already animated/tapped), 3 (sources), repeat-tag PASS.
23. No-blocker A-line -> always send (except CHECK A).
24. Attackers tap; Nadaar vigilance; never block with an attacker.
25. CHECK 0 doesn't-untap tag + two exemptions; CHECK A last body at life<=8; CHECK B lone all-"(neither dies)".
26. Outnumbering rule for all-"(neither dies)" tags; they choose the block.
27. Collapsed tag / GANG BLOCK -> card stays home, token goes.
28. "your attacker dies" per-creature STOP; price the worst name.
29. Shield Sphere exception and its two limits.
30. Blocking-trigger / lifelink clause -> swing pays them; prices add.
31. Life-to-damage converter -> price comes off your life; banned plan phrases.
32. "most expensive to attack into" is the swing's price.
33. Lethal overrides every stop; render's printed P/T is the math.
34. Block ladder: survival-first at lethal, rungs 0-3, forecast floor 10, trample exception, one blocker stops a non-trampler, "BLOCKS: none" test, Adventurer as blocker.
35. Acererak bounces, is never on the battlefield, is a repeatable venture only.
36. Always LOST MINE by name; Tomb/Mad Mage conditions; completion effects; room branch decided on effect text.
37. Lolth -3 first; Lolth's 0 life floor of 6.
38. Planeswalkers act every turn; hold-priority row literal test.
40. Draw-price bracket: prices casting at all; K<=1 -> cast nothing, non-spell actions still free.
41. Mulligan: engine counts, "(keeping N)" is position, keep at 5-or-lower with a land, the three ship hands, coverage line is a one-land COUNT test, colour never ships, carried plan is not evidence, keep-descriptions are tiebreakers.
42. Bottom ask: build the best N, land-backed cards count as lands, land goes under only from 5+ lands, plan-vs-PUT check.
43. Cleanup discard scan rungs (1)-(5) incl. the land-backed recognition warning and never-your-only-removal.
44. Counter/lifegain matchup: name the counterspell, Kaya is the only artifact answer, cast two spells a turn.
45. Sanguine Bond / Exquisite Blood priority and the both-out ATTACK: none.

## (b) DROPPED, by category (live guide 71,482 bytes -> 19,981)
- Past-game narration / "WHAT THIS COST" and "WHAT THIS PAID" ledgers, per-game citations, matchup records: ~18,000 bytes.
- Named-game references (`vsNNN` / NNNvNNN / seq numbers) and counts of past windows: ~3,500 bytes.
- Repetition of the same rule in DECIDING SITUATIONS after it was already stated in its own section (the whole trailing recognize->do list, ~9,000 bytes; its unique content was folded back into the rule sections).
- Argumentative restatement (the same rule re-asserted in three phrasings, "the sentence this seat wrote", "the plan is already wrong" essays): ~13,000 bytes.
- Card text the prompt already prints (full mode lists, loyalty ability text, dungeon room inventories, Emeria/Lolth/Command wording): ~4,000 bytes.
- Explanatory rationale after each rule (odds arithmetic, why-this-works prose): ~4,000 bytes.

## (c) Rules that could NOT be restated as condition -> action
None dropped for this reason. Two are judgement calls with no printed trigger and are kept as stated priorities rather than tests: "cast Acererak only with mana you have nothing better to do with", and "Tomb of Annihilation only when racing and clearly ahead on life".

## (d) Verification
- `stat -c %s strategy.txt` = **19981** (ceiling 20000).
- forbidden-token grep -ciE = **0**.
