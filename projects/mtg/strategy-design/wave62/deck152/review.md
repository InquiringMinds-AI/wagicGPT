# wave-62 deck152 review (Bant Human midrange)

Seat files, corpus `matchups-20260905-133058` unless noted; the 152v125 pairing is read from the
RERUN `matchups-20260905-161801` (binary 9230a94c3), the hung file being used only as hang evidence.

| # | file (deck152 seat) | opp | result | turn |
|---|---|---|---|---|
| G1 | 1788633061-...-vs-ai_baka_deck130 | 130 | WIN 20 / -5 | 13 |
| G2 | 1788633079-...-vs-ai_baka_deck126 | 126 | WIN 20 / -1 | 11 |
| G3 | 1788633075-...-vs-ai_baka_deck123 | 123 | WIN 20 / -5 | 17 |
| G4 | 1788633089-...-vs-ai_baka_deck146 | 146 | LOSS -4 / 22 | 17 |
| G5 | 1788633100-...-vs-ai_baka_deck162 | 162 | WIN 29 / -8 | 13 |
| G6 | 161801/1788643083-...-vs-ai_baka_deck125 (rerun) | 125 | WIN 8 / -2 | 31 |

5-1. No fallbacks, no wall misses, no timeouts, no re-asks, no illegal answers in any of the six
(`fallback` absent on all 213 seat records; every `deadline_pct` < 50; `wall_miss_events` 0).

## 1. Game by game — the deciding decisions

**G1 (vs130), win.** Curve-out: Aspirant t3/t5, Spellbinder t7, escalating swings t7/t9/t11/t13.
Spellbinder's hand look took Lay Waste (seq 11) — no enchantment on the list, so no rule bit.
seq 17 answered **"Play no land right now"** on a one-land drop ask (Lair of the Hydra, enters
tapped) — a flat #1a RULE violation. It cost nothing only because the identical drop ask was
re-offered at seq 19 after Katilda resolved and taken there. seq 20 and 23 correctly passed
Lair rows tagged `[this land is TAPPED: ...]` / `{paying this taps: Luminarch Aspirant, Elite
Spellbinder}`.

**G2 (vs126), win.** Cathar t5 → Sigarda t7 → swing every turn. seq 17 Spellbinder look listed
**Sanguine Bond** and the seat exiled it (the guide's named priority), which is why the converter
never assembled. Won on t11 before the 126 seat could rebuild.

**G3 (vs123), win.** seq 22 Spellbinder look → **Idyllic Tutor** exiled, exactly the fallback the
guide names when neither Blood nor Bond is on the list. Two Spellbinders closed it at seq 40.

**G4 (vs146), LOSS — the one game to read.** Deck146 ground the seat out with Barrowin recursion,
Silverquill Silencer and a dungeon engine. The seat declared **zero attackers in the whole game**;
that is not a missing window — every combat it had either no untapped non-sick creature (seq 20:
"2 are creatures, 0 of them able to attack right now") or an empty board.
- **seq 25 (t15, life 9) is the decision that lost it.** Header: "Unblocked, these attackers deal
  up to 6 - you would be at 3 - NOT lethal". Its one blocker, `B1. Wolf (2/2)`, was offered
  `A2 (you kill it, your blocker lives)`, `A3 (you kill it, your blocker lives)` and
  `A4 (your blocker dies, attacker lives)`. The seat answered `BLOCKS: B1:A4` — the rung-3 chump —
  against the ladder's own top-down order (#4 RULE, rung 1: "TAKE THAT BLOCK, always, at every life
  total and every forecast number"). It bought 2 life and spent the Wolf. Two turns later at
  seq 28 it had two Wolves against five attackers and needed three blockers to live; a surviving
  Wolf makes "best case ... you would be at 0" into a survivable 2.
- **seq 28 (t17, life 6), lethal screen.** Header: "best case with every blocker assigned: you
  would be at 0; no block saves you". The seat answered `B1:A2, B2:A3` (the two 2-power Goblins),
  taking 10 instead of the 6 that blocking A4/A5 would have taken — against the guide's
  "SURVIVAL COMES BEFORE THE LADDER ... taking the BIGGEST 'deals N' attackers first". Moot on
  this screen (0 is still a loss) but the wrong habit.
- seq 21 (t13) reply prose: "L1 dies, L2 lives" while the assignment it picked, `B2:A3`, is
  printed `(both die)` — the #4 RULE "drift to catch" (restate-then-override) in a window where
  the assignment itself was still right.
- seq 19/20: both Luminarch Aspirant combat triggers put on the SAME body (Aspirant #2 → 3/3)
  against "DO NOT PUT EVERY COUNTER ON ONE BODY". Tovolar's Huntmaster (t16) was answered by
  Kaya the Inexorable's -3 exile before it could ever attack — variance, not a guide effect.

**G5 (vs162), win — and the corpus's best protocol event.** At seq 34 (t13, opp at 3, Fog Bank
their only body) the reply's first line was `CHOICE: 2 (Cast nothing right now)`; the model then
reasoned past it ("So I MUST cast Fateful Absence") and emitted a second `CHOICE: 1 (Cast Fateful
Absence)`. The engine latched the LATER coded line (`answer_replaced: true`,
`latched_coded_line: 2`, `coded_answers: 2`) and cast it. seq 35 targeted Fog Bank; seq 43
attacked with four for the win. Without the latch this seat passes its own kill turn. This is
D9's "latch the LAST coded CHOICE" paying, live, on a game-deciding window.

**G6 (vs125 rerun), win at t31.** Deck125 answered every board with Supreme Verdict and drained
1/turn off Staff of Nin; the seat's "0 are creatures" turns (seq 42-45) are wipes, not passivity.
It kept re-deploying under #3 RULE and closed with Aspirant swings at seq 46/49.

**Hang corroboration.** The main-corpus 152v125 file's last record is seq 39, t27 Main phase 2,
`Cast Brutal Cathar` — the exact cast wave62/corpus-hang.md names as the trigger; its stderr
carries 88,232 `Action added to stack: Day|Night` lines. Nothing in the seat's decisions
contributed; no further decision data exists in that file.

## 2. Engine / interface / card items

**HIGH-1 — the blocker header's suggested assignment maximises LIFE ONLY and the model copies it
verbatim.** `INCOMING THIS COMBAT: ... best case with every blocker assigned: you would be at 6 -
one legal assignment that reaches it: Wolf blocks Barrowin of Clan Undurr` (G4 seq 25). That named
assignment is a rung-3 chump that loses the blocker, while the SAME blocker's line offers two
`(you kill it, your blocker lives)` results on the same screen. The seat answered the engine's
suggestion character-for-character (`BLOCKS: B1:A4`), and it is the decision that lost G4. Same
copy behaviour at G4 seq 21 (suggested "Luminarch Aspirant #1 blocks Barrowin; Luminarch Aspirant
#2 blocks Silverquill Silencer" → answered `B1:A4, B2:A3`). Fix: either price the suggestion on
blockers-surviving as well as life, or label it plainly as "highest life total, ignoring which of
your creatures die" so it stops reading as a recommendation.

**HIGH-2 — on a lethal screen the header withdraws the assignment entirely.** G4 seq 28:
"best case with every blocker assigned: you would be at 0; no block saves you" — and no
assignment is printed, because none reaches survival. The model, left with no suggestion, then
chose the damage-MAXIMISING block (10 through instead of 6). When no block saves you, still print
the least-damage assignment; a losing screen is exactly where the seat has the least to go on.

**MED-1 — Intrepid Adversary's counter menu prints 21 rows when 1 or 0 are payable.** G5 seq 24
(t11, 1 spendable): rows 2-21 all read `{repeat cost: N x {1}{w} = 2N mana for all N; you have 1
spendable now, which pays for 0 of them and stops}` — twenty rows that provably do nothing, and
the free "don't add any counter" row already covers that outcome. Rerun G6 seq 37 (3 spendable):
rows 3-21 all "pays for 1 of them and stops", i.e. nineteen aliases of row 2. Cap the list at the
affordable rung plus one, the way the Lair rows now are. (Same family as D5's X=0 dead end.)

**MED-2 — the Adversary row's mana half of D7 has landed; the tapping half is untested here.**
The modal ask now carries the per-counter cost and the affordability clause (G6 seq 37 above),
which the wave-61 deck152 HIGH-3 said was missing. I saw no window for this deck where a Human
was among the payable sources for that row, so whether it now prints
`{paying this taps: <names>}` is UNTESTED in this seat's six games.

**LOW-1 — the land drop is re-asked after each cast in the same main phase.** G1 seq 17 and seq 19
are the same one-land ask (`1. Play Lair of the Hydra [enters TAPPED ...] / 2. Play no land right
now`), the second issued after Katilda resolved. Harmless here — it recovered a #1a violation —
but it is a duplicate decision paid for at ~25 s of wall clock each.

**Fixes confirmed working in this seat (no defect found):**
- **D1 enters-tapped classifier.** All 12 land rows across the six games print the resolved form
  with the count that decides it: `Play Overgrown Farmland [enters TAPPED - it makes no mana this
  turn (you control 1 other land): "Overgrown Farmland enters tapped unless you control two or
  more other lands."]` and, at G2 seq 2, `Play Lair of the Hydra [enters UNTAPPED - it makes mana
  this turn (you control 0 other lands)]`. No unconditional-TAPPED misread observed.
- **D4 Lair of the Hydra X rungs.** Now rendered as separate affordable rungs — G2 seq 15:
  `1. becomes a 1/1 hydra ... [cost: {1}{g}]`, `2. becomes a 2/2 hydra ... [cost: {2}{g}]`,
  `3. becomes a 3/3 hydra ... [cost: {3}{g}]`. Verified faithful against the primitive:
  borderline.txt:64293-64296 define `{1}{G}` 1/1, `{2}{G}` 2/2, `{3}{G}` 3/3, `{4}{G}` 4/4 as
  twenty discrete abilities (not a real X cost), and the enters-tapped condition at 64291 is
  `if compare(type:land:myBattlefield)~morethan~2 then tap(noevent)`.
- Fog Bank's render ("Prevent all combat damage that would be dealt to and dealt by Fog Bank",
  G5 seq 35) matches mtg.txt:42416-42420 (`preventAllCombatDamage to(this)` / `from(this)`);
  the model reasoned correctly off it at G5 seq 33.

## 3. Guide verdict: **KEEP as is**

The live `deck152_strategy.txt` (67,114 bytes, inside the 41-71 KB band) is unedited. Every
misplay this corpus produced is a rule the guide ALREADY states in the words the seat needed:
- G4 seq 25 rung-3-over-rung-1 → BLOCK LADDER, "answer from this ladder, top down" and rung 1
  "TAKE THAT BLOCK, always ... No number on this screen can make this block wrong."
- G4 seq 28 small-attackers-first on a lethal screen → "SURVIVAL COMES BEFORE THE LADDER ...
  taking the BIGGEST 'deals N' attackers first".
- G4 seq 21 prose contradicting `(both die)` → "THE DRIFT TO CATCH ... THE PARENTHESIS WINS".
- G4 seq 19/20 both counters on one body → "DO NOT PUT EVERY COUNTER ON ONE BODY".
- G1 seq 17 "Play no land right now" → #1a RULE, "The ONLY answer that is ever right is
  'Play <land>'".
The guide's own named plays are what won the other five: the Spellbinder priority list fired
correctly twice (G2 seq 17 Sanguine Bond, G3 seq 22 Idyllic Tutor), the Lair PASS tags were
obeyed (G1 seq 20/23), and the deploy floor carried G6 through three board wipes. Restating rules
the seat broke while looking at them buys nothing; the failure mode is compliance, not coverage,
and the two HIGH items above are the surfaces that actually steered the losing decision.

One observation recorded without an edit: the seat took the HOLD row in 7 of 7 opportunities and
in NONE of them did the prompt carry `[you declined this exact list N times already this turn]`,
the single literal the guide's HOLD rule makes the test (5 of the 7 carried a `[hold check: N rows
above are new ...]` line instead, i.e. the board had just moved). It cost nothing measurable
(G2 skipped 25 windows and won on t11; G1 skipped 1), so the deviation is not paid for and the
rule stays as written.

No general-guide or skill proposals: nothing in these six games is evidence about a rule that
crosses decks, and the two interface items belong to the engine seat's surface, not the guides'.

## 4. What I did NOT check
- The five other decks' seats except as opponent context for these six pairings.
- The lane-{W,X,Y,Z,AA} prediction ledgers — not adjudicated here (engine seat's job); the D1/D4/D7
  notes above are observations from this seat's windows only, not verdicts on those predictions.
- The hung 152v125 file's 39 decision records were NOT read as play evidence, per the brief.
- `Scryfall` cross-checks: card facts were verified against the primitives only (no disagreement
  arose that needed a second source).
- Any engine source; no build, no run, no git.
