# deck162 - wave-51 edits (corpus `matchups-20260827-155545`, deck162 2-4)

Baseline = live `bin/Res/ai/baka/deck162_strategy.txt` (33,483 bytes, wave-50 revision + boundary
B1). After = `wave51/deck162/strategy.txt` (34,979 bytes, +1,496). One re-key (RULE 2 -> the lane-F
`{feeds:` string, in five places) and one discard clause. Card facts: Liliana's Caress
`@discarded(*|opponenthand):life:-2 opponent`; Howling Mine `@each opponent draw sourcenottap:
draw:1 opponent`; Master of the Feast `@each my upkeep:draw:1 opponent`. The `{feeds:` literal is
quoted from the corpus rows (`162 vs146` s6: `{feeds: the opponent draws 1 extra card per turn;
converters on your battlefield: 0 (nothing of yours punishes their draws or discards yet - the
cards you hand them are free until a converter is out)}`; `162 vs125` s17: `converters on your
battlefield: 2 - Ob Nixilis, the Hate-Twisted, Liliana's Caress`).

| # | section | before -> after (abridged) | finding (file, seq) |
|---|---|---|---|
| 162-G | RULE 2, the count paragraph | *"Look at YOUR battlefield line and count the names Underworld Dreams, Fate Unraveler and Ob Nixilis. LILIANA'S CARESS ... COUNTS AS ONE OF THEM once ..."* -> *"EVERY draw-engine row prints its own count: '{feeds: ...; converters on your battlefield: K - <names>}' ... Read K off the row you are about to take. It counts Underworld Dreams, Fate Unraveler, Ob Nixilis AND Liliana's Caress for you ... ONE qualifier is yours to add: when the only name after K is Liliana's Caress, K counts only while the 'Opponent hand size:' line reads 7 or more ... at a hand of 6 or less ... the row's K is read as 0."* | R39/lane F shipped: every Mine / Dictate / Puzzle Box / Forced Fruition / Master of the Feast row at my six seats carried `{feeds:` (78 rows, 0 misses). The engine's K counts Caress unconditionally (`162 vs125` s8: `converters: 1 - Liliana's Caress` at hand 7 - compliant by luck of the hand size); the guide keeps its hand-size qualifier as the one thing the string does not price (boundary B4's note). |
| 162-G (b) | RULE 2, the two branches | *"- ONE OR MORE of them on your battlefield"* -> *"- K OF 1 OR MORE (the Caress qualifier passed)"*; *"- ZERO of them ... one Howling Mine is enough. With a Howling Mine already ... take a wall ..."* -> *"- K OF 0: one Howling Mine is enough, and the FIRST Howling Mine is cast at K of 0 - its row reads 'converters on your battlefield: 0' too, and that is fine for that one card ... a second row reading 'converters on your battlefield: 0' is the brake ... WHAT THIS COST, both ways: ..."* | The break: `162 vs146` s6 (T6): `Cast Howling Mine {2} [already owned: Howling Mine on your battlefield] ... {feeds: ...; converters on your battlefield: 0 (nothing of yours punishes ...)}` taken; the plan says "stack Howling Mine and Dictate once a punisher is on the battlefield" while doing it. deck146 then held 11-13 cards (s24 `146 vs162`: hand 11; s37: 13), Versed Underworld Dreams twice (s25, s47/s49) and Soul-Shattered Ob Nixilis (s40); deck162 dead T15 from 20. The over-brake: `162 vs130` s5, s6, s9, s10 (T5-T7): `Cast Howling Mine` at K of 0 with NO Mine on the line, passed four windows, plan "stack draw engines once Fate Unraveler is out" - the first-Mine exemption lived only in cast-order entry 5 and was never read. |
| 162-G (c) | RULE 2 tail, CHECK-YOUR-PLAN sentence, cast order entry 5, three DECIDING lines | *"Count the punisher names on YOUR battlefield line"* -> *"Read K off the row"*; plan check keyed to *"the row you are taking prints 'converters on your battlefield: 0'"*; entry 5: *"when the row's 'converters on your battlefield:' number is 1 or more, or it is your first Howling Mine (K of 0 and no Howling Mine on your line yet)"*; DECIDING: the no-punisher, the punisher-out and the plan-says-stack lines all keyed to the printed K. | Same windows. Every sentence that counted battlefield names now reads the row's own number (#128: key to the emit string; #149: the converter is in the count). |
| 162-H | MULLIGAN, cleanup-discard order | *"you answer 'PUT: n, m' - the numbers of the cards you SEND"* -> *"the numbers ONLY, nothing in brackets after them - of the cards you SEND"* | `162 vs146` s17 (T12 cleanup, 477 s): reply `PUT: 3 (Forced Fruition)`; the engine discarded **Liliana's Caress** (row 2) - narration `Cleanup discard (hand 8, limit 7): you chose Liliana's Caress`. The order itself was right (Forced Fruition first); the bracketed name changed the executed card. Engine item E-1 (HIGH) in the seat file; this clause is the emit-form of the ask's own instruction ("write PUT: followed by the 1 card number"), not a description of the defect. |

## Read but NOT edited
- **The four losses**: `vs123` (-69 T12): Intruder Alarm + Lord of Lineage + 20 Vampires by T11
  (s13 their line: 21 creatures); the seat's only body was a Shield Sphere, Master of the Feast
  gated correctly at "0 creatures" (s9/s10), Underworld Dreams T9 (RULE 1, first punisher drawn),
  Puzzle Box T11 at K = 1 - no window had a better legal answer. `vs130` (0/8 T16): Fate Unraveler
  T9, Starstorm killed it T14, Rorix twice; the first-Mine over-brake above is the only text
  window. `vs152` (-5/10 T13): 2-land keep (guide: keep; coverage named Shield Sphere), lands T2/
  T4/T6/T8, first spell T8; Fate Unraveler chumped Moonrage Brute at 16 life on rung 0's "8 OR
  LESS" arithmetic (16 - 8 = 8, `vs152` s8) - inside the rule, and the alternative (take 8, keep the
  punisher) faced the same 8 next turn with no wall; no edit. `vs146` (-2/4 T15): the 162-G break
  plus the engine mis-discard.
- **RULE 1 punisher-first**: every window with a punisher on the menu cast it - `vs125` s13/s16,
  `vs126` s7/s9/s13/s16, `vs130` s12, `vs146` s8/s12/s15/s19/s20, `vs152` s7/s10, `vs123` s12 (15/15).
- **Ob Nixilis -2**: `vs146` s16 on Nadaar (the dungeon's token maker) ✓, `vs152` s11 on Intrepid
  Adversary (the lord) and s14 on Katilda ✓; no 2 x P >= life window arose (162-D untested).
- **RULE 3**: Shield Sphere cast 6/6 offers; Fog Bank 3/3 at the first opposing creature.
- **`[already owned:` casts**: Underworld Dreams #2/#3 (`vs126` s9/s13, `vs146` s20) and Mine #2/#3
  at K >= 1 (`vs125` s17, `vs146` s13) are all live stacks; the tag's only "dead" take is the 162-G
  break, which is a brake question, not a redundancy one.
- **Master of the Feast**: cast `vs125` s8 (K = 1 via Caress, opp hand 7) and `vs126` s21 (K = 4);
  never at K = 0. `vs130` s19/s20 (T15, K = 0, opp creatures on their line) it was passed for Sphere
  + Fog Bank - the exception permits it, the walls were the better cast at 6 life.
