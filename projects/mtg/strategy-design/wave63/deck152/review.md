# Wave-63 deck152 (Bant Human midrange) — per-deck review

Seat record 4-2. Corpus `matchups-20260905-191148` (binary 98d13050f) for 152v123, 152v146,
152v126, 152v125; reruns `matchups-20260905-214946` (152v162) and `matchups-20260905-215345`
(152v130) on 0026a9d87. The two 191148 files for 152v162 / 152v130 are the hung games and are
used below only as evidence of the hang. Zero `fallback` records in all six games; zero
`transport`/`empty_reply` markers; no `deadline_pct` above 100.

## 1. Game by game

| game | file (seat) | result | decided by |
|---|---|---|---|
| 152v123 | 1788653513-…-vs-deck123 | WON 21/-3 t13 | Spellbinder + Sigarda curve, opp had no answer |
| 152v126 | 1788653550-…-vs-deck126 | WON 20/-5 t11 | two Luminarch Aspirants stacking counters |
| 146v152 | 1788653548-…-vs-deck146 | WON 10/0 t23 | Sigarda + Intrepid Adversary lifelink race |
| 152v125 | 1788653552-…-vs-deck125 | LOST -2/25 t37 | Emrakul (annihilator 6); board wiped from t23 |
| 152v162 | 214946/1788662989-…-vs-deck162 | LOST 0/3 t15 | punisher package; **HIGH-1 and HIGH-2 below both bit here** |
| 152v130 | 215345/1788663228-…-vs-deck130 | WON 13/-4 t25 | Spellbinder beats + Sigarda |

Deciding decisions with citations:

- **152v162 seq 8/9 (t7, Draw step) — STRATEGY.** With TWO Brutal Cathars in hand the seat spent
  its only Fateful Absence on Fog Bank (0/2 defender). The same turn it cast a Brutal Cathar
  (seq 11) whose exile ETB then had **no target**. Ob Nixilis, the Hate-Twisted resolved on t10
  with no answer left, -2'd both Cathars and pinged 6 of the damage that killed the seat in a
  game the opponent finished at 3 life. Surface was correct and complete (the option line printed
  Fog Bank's full target text and Underworld Dreams' effect); the model's stated reason — "stop
  Underworld Dreams triggers from drawing" — is not a fact about Fog Bank. Guide item, see §3.
- **152v162 seq 17 (t11 Attackers) — see HIGH-1.** The main-1 casting window never opened, so
  Intrepid Adversary + valor counter were cast at seq 18/19 in **main 2**. Precombat they would
  have made Katilda/Cathar/Wolf 2/2, 3/3, 3/3 (8 damage instead of 5). The game ended with the
  opponent at 3.
- **152v125 seq 44/45 (t22 Upkeep) — STRATEGY, against an existing guide rule.** The cast row read
  "the only legal targets are YOUR OWN right now" — the guide's verbatim STOP — and the seat cast
  Fateful Absence anyway, on the theory that destroying Sigarda would "save her from Path to
  Exile". At the target ask it recognised the error ("I must accept this loss") and, with no
  decline row, had to destroy its own Sigarda. See HIGH-3 for the render half of this.
  The mirror case at 152v126 seq 15 shows the rule IS followed elsewhere (declined correctly), so
  this is adherence, not a missing rule — no edit proposed for it.
- **152v125 seq 74/75 (t35) — see HIGH-1.** Front-face land drop; the Lair of the Hydra animation
  landed in **main 2** (seq 75) and the turn produced **no attackers window at all**. The model's
  own plan at seq 76 still says "Attack with the 9/9 Hydra … bringing opponent to 16": 9 damage
  lost off a board that had nothing else, against an opponent at 25.
- **146v152 seq 18 (t11 Blockers) — good play, bad header.** See MED-1.
- 152v125 seq 76-81 (t37): Emrakul annihilator 6, six forced-sacrifice asks; `INCOMING THIS
  COMBAT … this KILLS you` rendered correctly. Nothing to fix — the seat was dead on board.

## 2. Engine / interface / card items

### HIGH-1 — a pathway's FRONT face consumes the rest of main phase 1
The seat's main-phase-1 casting window is never asked again after it takes the front-face row of a
modal land menu (the row rendered `<Name> (menu text: Play Land)`), while the back-face row leaves
it intact. Corpus-wide over all 42 seat files of `matchups-20260905-191148`:

| face taken | next window is Main phase 1, same turn | next window is Attackers / Combat begins / Main phase 2 |
|---|---|---|
| FRONT `(menu text: Play Land)` | **0** | 15 |
| back face | 5 | 1 (deck146 seq 15, Agadeem's Awakening — a tap-payment ask, different shape) |

Repro (cleanest A/B, same deck, same turn number, adjacent games):
- `215345/1788663228-…-vs-deck130` seq 6 (t3) takes the **back** face → seq 7 is
  `Main phase 1` "Cast Luminarch Aspirant" and the Aspirant's beginning-of-combat trigger fires.
- `191148/1788653550-…-vs-deck126` seq 6 (t3) takes the **front** face → seq 7 is
  `Main phase 2` "Cast Luminarch Aspirant {leaves 0 of your 2 untapped mana sources untapped}".
  Same card, same mana, one turn later in effect.

Rendered line at the seam (`214946/1788662989-…-vs-deck162` seq 16):
`1. Hengegate Pathway (menu text: Play Land) [PLAY THIS AS A LAND: puts "Hengegate Pathway" onto the battlefield as a land (taps for {W}). It costs no mana and uses no stack, and it USES YOUR LAND DROP for this turn.]`
— that text promises the drop costs no cast; the seat then gets no cast window this main phase.

Cost in these six games: 152v162 seq 16 (t11, Adversary+counter pushed to main 2, game lost with
opp at 3), 152v125 seq 74 (t35, no attack at all), 152v126 seq 14 (t7, Brutal Cathar to main 2),
152v146 seq 20 (t12, Augur + Aspirant to main 2), 152v130-rerun seq 16/20/27 (t9/t11/t15).
Deck152 runs ten pathways, so it is hit hardest, but the table above is all seven decks.
I did NOT determine the mechanism in the engine — I only read the prompt renderer around
`src/AIPlayerGPT.cpp:32283-32430` (the modal-DFC land-menu detection) and did not trace the
action-execution path; the observable is the record, not a code claim.

### HIGH-2 — no decision surface anywhere lets the seat attack a planeswalker
Across the whole corpus, **0** `attackers` prompts mention a planeswalker in the combat block, and
no prompt in any seat file contains "attack the planeswalker", "which planeswalker",
"planeswalker instead" or "redirect". Every ATTACK TOTAL line reads "… total combat damage to a
player". The engine itself supports it (`attackPlaneswalkerCost` in `MTGCardInstance.cpp:469`,
`src/GuiPlay.cpp:728` "draw line when attacking planeswalker"), so this is a GPT-surface gap.

Repro: `214946/1788662989-…-vs-deck162` seq 17 (t11). Opponent battlefield line:
`Ob Nixilis, the Hate-Twisted {3}{b}{b} [planeswalker] [counters: 3x loyalty]`, opponent has
**0 creatures**, and the combat block offers only
`ATTACK TOTAL: 3 attackers listed, 5 total combat damage to a player`. Wolf (2) + Katilda (1)
into a 3-loyalty walker kills it outright with Cathar's 2 still going to the face. Ob Nixilis
instead survived the game and dealt 6 of the damage that killed the seat (counted from the final
game log: 6 Ob Nixilis pings, 8 Underworld Dreams pings), which ended 0 to 3.

### HIGH-3 — the stack-trap NOTE names a decline row that does not exist, and calls a destroy a bounce
`src/AIPlayerGPT.cpp:33893-33898` emits, unconditionally under `stackTrapNote`:
`… pick a battlefield permanent that is worth bouncing, or decline.`
Repro `191148/1788653552-…-vs-deck125` seq 45: the target ask that follows has exactly two rows
(`options: 2`, both the seat's own creatures) and **no decline row**, and the spell is Fateful
Absence, whose own card text on the same screen reads "Destroy target creature or planeswalker" —
nothing is bounced. The model had just written "I must accept this loss", i.e. it wanted the
decline the note advertised, and destroyed its own 6/6 Sigarda instead. One occurrence corpus-wide
and it is this seat's; the string is unconditional, so the window is rare, not the bug.

### MED-1 — the best-case blockers header claims material was weighed on a pure chump
`src/AIPlayerGPT.cpp:19488-19497`: the non-`leastDamageOnly` branch always says
"one legal assignment that reaches it, chosen for your blockers' material as well as for the life".
Repro `191148/1788653548-…-vs-deck146` seq 18 (t11), rendered:
`best case with every blocker assigned: you would be at 20 - one legal assignment that reaches it, chosen for your blockers' material as well as for the life: Katilda, Dawnhart Prime blocks Barrowin of Clan Undurr; 1 of those blockers die`
The only B-line on the same screen is `B1. Katilda, Dawnhart Prime (2/2) - may block A1 (your blocker dies, attacker lives)` — a pure chump, at 20 life against 3 damage. Material was not weighed; there was nothing to weigh. This is the wave-62 E3 shape surviving the lane-AB rewrite in the *no-survivable-line* case: the header should say so rather than assert a ranking it did not perform. The seat played correctly (BLOCKS: none) and paid nothing here, so this is a narration defect, not a lost game.

### MED-2 — `attack_last_line_taken` with the deciding tail trimmed and no line recorded
`191148/1788653548-…-vs-deck146` seq 39: `parse_note = attack_last_line_taken`,
`reply_trimmed_bytes = 4137`, `coded_answers = 2`, and the record carries **no** `latched_line`
field — unlike `ask` records, where the E6 fix does record it verbatim (e.g. same file seq 24
`latched_line = "CHOICE: 2 (Cast Tovolar's Huntmaster)"` beside `reply_trimmed_bytes = 9105`).
The chosen attackers happen to match the reply's first `ATTACK:` line, but the record cannot prove
which line the parser took. Corpus-wide there are 2 `attack_last_line_taken` records; one carries
`latched_line`, this one does not. Extend the verbatim-line record to the attackers parser.

### LOW-3 — combat-begins window grind
`191148/1788653550-…-vs-deck126` t7 spends eight consecutive windows at `Combat begins`
(seq 13-20: land, face, cast-nothing, Aspirant counter, cast-nothing, hold, Aspirant counter,
cast-nothing) before the attackers ask. t11 repeats it (seq 33-40). Each is a full prompt render.
Nothing was decided wrongly; it is latency and context spend.

### LOW-4 — model re-derives facts the prompt already states, at length
`214946/…-vs-deck162` seq 23 (4,781 B trimmed): at 3 life the reply argues "Drawing Augur costs 3
life. We would go to 0" before correcting itself — casting is not drawing, and the DRAW PUNISHERS
paragraph on the same screen says "Every card YOU draw". `191148/…-vs-deck146` seq 39 (4,137 B)
re-derives whether blocked attackers damage the player, which the ATTACK TOTAL line answers. In
all three trimmed-reply cases in this seat's games the first coded line was correct and
`answer_replaced` was `False`, so nothing was lost — but a "casting a card is not drawing a card"
clause beside the DRAW FORECAST would buy back kilobytes of reasoning at the exact life totals
where it matters.

### Hang (context only)
The two 191148 files for 152v162 / 152v130 end mid-game with no `gameend`; 152v162's last record
is seq 46 at t13, 152v130's seq 37 at t15, and both carry `async_drops` at the seam
(`152v162` seq 25 `async_drop_events: ['casting/question and board/re-asked', 'casting/board/re-asked']`, seq 35 likewise).
Consistent with `wave63/corpus-hang.md`'s daybound marker loop; both reruns finished naturally.
No corpus game of this seat showed the `softlock-blockers-1788650768.md` shape (an AI seat stuck at
Blockers with a calm stack): the seat's seven blockers windows all resolved in one ask.

## 3. Guide verdict: EDIT (one change)

One change, to the FATEFUL ABSENCE bullet in the REMOVAL section. Everything else in
`deck152_strategy.txt` KEEPS: the mulligan rules, #1 mana rule, #1a land drop, #2/#2a/#3 deploy and
lethal-count rules and #4 blocking all held up in these six games, and the STOP clause that this
same bullet already carries was obeyed correctly at 152v126 seq 15. I deliberately did NOT add
another synonym for the "fire it at your own board" wrong-reasons list even though 152v125 seq 44
violated it — the rule is present and explicit; that was an adherence miss, and restating it is
noise.

Full revised guide: `wave63/deck152/strategy.txt` (67,114 B → 68,040 B; pool band 41-71 KB).

**Edit 1 — Fateful Absence: name the answer ORDER against a creature.**

BEFORE (lines 403-404):
```
- FATEFUL ABSENCE ({1}{W}, instant): destroys a creature or planeswalker (they get a Clue). Save
  it for their best creature or a planeswalker. This is the ONE card worth holding with mana up.
```
AFTER:
```
- FATEFUL ABSENCE ({1}{W}, instant): destroys a creature or planeswalker (they get a Clue). It is
  the ONLY card in this deck that answers a PLANESWALKER, and it is the ONE card worth holding
  with mana up. Against a CREATURE it is your LAST answer, not your first: Brutal Cathar exiles a
  creature and leaves a 2/2 behind, and you often hold two of them. So before you point Absence at
  a creature, look at your hand - if a Brutal Cathar is in it, cast the CATHAR at that creature
  and keep the Absence. A wall (Fog Bank, any 0/x defender) is never worth the Absence while a
  Cathar is in hand; it is worth the Cathar. Point Absence at a creature only when it is a
  planeswalker-free board AND no Cathar can handle the thing.
  WHAT THIS COST: on turn 7 against deck162, holding TWO Brutal Cathars, this seat fired Absence
  at a 0/2 Fog Bank in its own draw step - then cast a Cathar the same turn whose exile ETB had
  nothing left to exile. Ob Nixilis, the Hate-Twisted landed on turn 10 with the answer already
  spent, killed both Cathars with its -2, and pinged 6 of the damage that ended the game 0 to 3.
```
Paid by `214946/1788662989-ai_baka_deck152-…-vs-ai_baka_deck162.jsonl` seq 8 (the cast), seq 9
(the target), seq 11 (the Cathar cast with no exile target), and the final game log's Ob Nixilis
ping count.

## 4. Proposals
None. Nothing here generalises past deck152's own card list (the Cathar/Absence ordering) or is
already a general-guide/skill topic; the three HIGH items are engine work, not guide work.

## What I did NOT check
- The opponent seats' translogs beyond the game log embedded in this seat's prompts; I did not
  read deck123/125/126/130/146/162 seat files except for the corpus-wide census tables above.
- Card scripts: I verified no primitive by `name=` grep and consulted no Scryfall text this pass —
  every card fact above is quoted from the rendered prompt, not from `primitives/*.txt`. The
  Fog Bank / Ob Nixilis / Fateful Absence / Intrepid Adversary texts should be re-checked against
  the primitives before anyone acts on the arithmetic.
- The engine mechanism behind HIGH-1 (no execution-path trace, no fixture, no build — the brief
  bars running wagic).
- HOLD render/take prefix census, `async_drops` totals and lane-AB…AF prediction adjudication:
  engine-seat scope, not attempted here.
- I did not verify that the alternative lines I price (attacking Ob Nixilis at 152v162 seq 17;
  Adversary precombat at t11) actually win — the opponent's replies to a different board are not
  in the record. The claim is only that the surface never offered them.
