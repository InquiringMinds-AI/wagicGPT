# deck123 (Intruders Of Thraben, WBU token combo) — wave-61 per-deck review

Corpus: `matchups-20260905-092408/`, six seat translogs
`1788618{260,272,274,282,284,287}-ai_baka_deck123-0x…-vs-ai_baka_deck<opp>.jsonl`.
Record **0-6**. 2 fallbacks in 297 seat records (`plan_missing`, `named_row_reask`), 0 hangs,
0 crashes. Guide read: `bin/Res/ai/baka/deck123_strategy.txt` (70,982 B), `wave59/general-strategy.md`.
All card facts below verified against `bin/Res/sets/primitives/mtg.txt` (line numbers cited).

---

## 1. Game by game — outcome and the decisions that decided it

### vs deck152 (loss, turn 9, -2 to 20) — `…287-…-vs-ai_baka_deck152.jsonl`
Kept a 3-land 7 with two Bloodline Keepers. Cast **one spell all game** (Vision Skeins, s20).

* **s4 / s5 (turn 4, MP1 then MP2), the deciding pair.** Row 1 read
  `Cast Devour Flesh {1}{b} {right now: they control 1 creature - Luminarch Aspirant (2/2) is
  sacrificed, they gain 2 …}` — castable, N=1, the body a 2-power counter-engine, Tribute to Hunger
  not on the list (needed 3 mana, it had 2). RULE 5's N=1 branch is the cast. It answered
  `2 (Cast nothing right now)` **twice**. Aspirant went 2/2 → 3/3 → 4/4; life 20 → 17 → 9 → dead.
  By s11/s13 N was 2 and the window was gone (declining there was correct).
* **s10 (turn 6, MP1).** `Play no land right now` on a live `Land drop: NOT yet used this turn`
  ask with two Play rows — RULE 0 violation. Recovered at s12 (played Isolated Chapel in MP2), so
  the drop was not lost, but MP1 was spent at 2 mana instead of 3.
* **s13.** Its own PLAN one window earlier said *"cast Devour Flesh on the opponent to remove Elite
  Spellbinder"*; the row was on the menu, castable; it answered `4 (Cast nothing right now)`.
  (N was 2 by then, so declining was right — but the plan and the answer contradicted.)

### vs deck146 (loss, turn 15, 2 to 22) — `…282-…`
* **s6 (turn 4, Cleanup).** Discarded **Damnation** — its only copy — off a menu that printed
  `6. Devour Flesh (copy 1 of 2 …)` / `7. Devour Flesh (copy 2 of 2 …)`. The guide's Vision Skeins
  entry names this exact ask and this exact answer: *"name the SPARE — a second copy ('copy 2 of 2')
  … never your only maker, only Alarm or only Damnation."*
* s8 Intruder Alarm resolved turn 6; s17 Bloodline Keeper turn 10; Keeper died turn 11; s25 second
  Keeper turn 12, blocked and died turn 13 (s26); s27 Thraben Doomsayer turn 14, blocked and died
  turn 15. **No token line was ever offered** — every maker died before it lost summoning sickness.
  That is correct engine behaviour (Keeper cast on your turn T is sick through T+1), not a bug.
* Its own reply at s26 contains the self-correction *"Wait, I don't have Intruder Alarm"* — the
  Alarm cast at s8 was gone from the battlefield line by s17 and the model tracked it late.

### vs deck162 (loss, turn 15, 0 to 20) — `…284-…`
* **s31/s34/s37 (turn 14, YOUR UPKEEP).** Fired the whole chain in its own upkeep — 22 Vampires,
  every one `[summoning sick - cannot attack this turn, but CAN block]`. RULE 1's
  *"YOUR OWN UPKEEP IS THE WORST WINDOW"* / *"only the opponent's turn grows the attack list"*.
  The attack step (s45) then offered exactly one attacker.
* **s43 (turn 14, MP1).** `Hold Isolated Chapel - do not play it now` on a live land drop.
  See MED-2 — this ask's wording is a variant RULE 0 does not quote. Recovered at s47.
* Lost to Peer into the Abyss forcing 21 draws into `Underworld Dreams + 2x Fate Unraveler`
  (3 life per draw, the prompt's own DRAW PUNISHERS block). No decision preceded it.

### vs deck126 (loss, turn 13, 0 to 39) — `…274-…` — **the game with the engine defect**
Assembled the combo (Alarm turn 6, Doomsayer turn 8, Greaves turn 10) and built **205 Humans**.
Attacked **zero** times.
* **s52 (turn 12, Attackers) — the deciding decision.** 97 attackers, 98 power, two 0/4 defenders
  untapped. The window printed **two verdicts that contradict each other** (see HIGH-1). The seat
  answered `ATTACK: none`, citing the loop.
  Its read was RIGHT and the guide's BRANCH B2 says exactly this. Confirmed by what followed:
  s73 the opponent's edict took one 1/1, they gained 1, and the Sanguine Bond ↔ Exquisite Blood
  chain took the seat from 19 to 0 and them from 20 to 39 (19 iterations of 1) in one event.
* The real loss point is upstream and outside the guide's reach: nothing in the sixty removes an
  enchantment, and at turn 10 (s23) only 1 of its 25 creatures could attack.

### vs deck125 (loss, turn 33, 0 to 61) — `…272-…`
* **s36 (turn 22, MP1).** Cast Damnation off a row reading
  `{right now: destroys 0 of their creatures (0 without a restriction against attacking), 0 of yours}`
  on a menu headed `NO LIVE CAST ROW ON THIS MENU`. RULE 3: *"'DESTROYS 0 OF THEIR CREATURES' IS A
  CARD THROWN AWAY. At N = 0 the Damnation stays in hand, whatever the plan says."*
  Its PLAN line read *"Cast Damnation (clears board, no creatures to trade)"*.
* **s21–s34 (turns 14–19).** Five straight turns at 5 mana answering `Cast nothing` / `Hold`,
  ending in **four Cleanup discards** (s23 Devour Flesh, s27 Damnation, s33 Intruder Alarm,
  s37 Devour Flesh). Two 2x Staff of Nin pinged 2/turn the whole time.
* **s42 / s46 / s51 (turns 24, 26, 28).** Three Thraben Doomsayers cast into an empty opponent
  board; each died to Staff of Nin before its own turn came round. Correct casts, dead deck.
* **s30.** `<refused: named_row_reask>` — answered `CHOICE: 2 (Cast Thraben Doomsayer)` where row 2
  was Damnation and Doomsayer was not on the menu at all. 1 re-ask + 1 recovery record.

### vs deck130 (loss, turn 20, -1 to 17) — `…260-…`
Mono-red burn with **0 creatures on the board from turn 13 to the end**. Every removal spell in the
sixty targets a creature. From s33 to s39 (turns 15-20) the seat correctly answered `Cast nothing`
against menus the engine itself headed `NO LIVE CAST ROW ON THIS MENU`, held the redundant second
Intruder Alarm (`a second copy changes nothing`) and the dead Idyllic Tutor
(`every enchantment left in your library is a copy of one you already control`). Life 20 → -1.
**No misplay found.** This is a deck-vs-matchup loss and the zero-verdict rows worked.

---

## 2. Engine / interface / card items

### HIGH-1 — `ATTACK TOTAL`'s kill claim ignores the life LOOP the same window narrates
File `1788618274-ai_baka_deck123-0x55afa8eea030-vs-ai_baka_deck126.jsonl`, **seq 52**, turn 12.
One window, two verdicts, opposite answers:

```
ATTACK TOTAL: 97 attackers listed, 98 total combat damage to a player - declaring all of them
with none blocked puts them at -78. At least 95 damage lands whatever they block … up to 4 life
back across their 2 blockers, so blocking can leave them as high as -71; that KILLS them
whatever they block, gain included.
```
```
Both halves of their life LOOP are in play (see CURRENT SITUATION): any life they gain or you
lose in combat chains until you are at 0, so an attack that lets them gain ANY life, or costs
you ANY life, is fatal.
```

Lane R's C1 `blockGain` is doing its job — it summed the 4 life and re-scoped the floor. What it
cannot do is notice that with `Sanguine Bond` (mtg.txt:99004, `@lifeof(player) … life:-thatmuch
opponent`) and `Exquisite Blood` (mtg.txt:37966, `@lifelostfoeof(player):life:thatmuch controller`)
both on the same battlefield, the gain is **not a bounded 4** — it is unbounded and it resolves in
declare-blockers, before any of the 98 damage. `Perimeter Captain` (mtg.txt:85398) fires per
blocking defender and both their bodies are defenders, so any block starts it.
**The corpus proves the loop is real and fatal, in this very game:** seq 73 → gameend, a single
1-life gain took the seat 19 → 0 and the opponent 20 → 39.
So `that KILLS them whatever they block, gain included` is a **false verdict on a decision that
decided a game**, and the guide currently has to spend a whole paragraph
(`YOUR ATTACKER TAGS CANNOT SEE THIS`) telling the model to disbelieve the engine.
**Fix shape:** withhold every kill claim (and name the reason) whenever a life LOOP is detected on
the opponent's side and `blockGain > 0` — the same fail-closed discipline C1 already applies to
an unpriced `attackPunisher`.

### MED-1 — Isolated Chapel's enters-tapped condition is never resolved, and the seat declines the drop
Rendered every time, e.g. `…287-…-vs-deck152` seq 10 / `…284-…-vs-deck162` seq 43:
`Play Isolated Chapel [enters tapped UNLESS its own condition holds, so it may make no mana this
turn: "Isolated Chapel enters tapped unless you control a Plains or Swamp."]`
The condition is **decidable from the rendered battlefield line** and in this deck is essentially
always TRUE: `Underground Sea` is `subtype=Island Swamp` (mtg.txt:125732), `Tundra` is
`Plains Island` (124691), `Scrubland` is `Plains Swamp` (100809) — twelve of its twenty-four lands
satisfy it, and the engine agrees (deck152 seq 12 → seq 13, Chapel enters untapped, mana 2 → 3 with
no Plains or Swamp *named* on the battlefield line). The V-lane fix deliberately reads the land's own
ETB line only; the cost of that is a permanently-hedged row on a deck whose lands are all duals.
Both land-drop declines in this corpus are on a Chapel row. **Fix shape:** evaluate
`aslongas(plains,swamp|myBattlefield)` against the same battlefield the prompt just printed and
render `enters UNTAPPED (you control Underground Sea, a Swamp)` / `enters TAPPED` — not "may".

### MED-2 — the one-land land-drop ask uses different wording from the many-land one
`…284-…-vs-deck162` seq 43: `Land drop: play Isolated Chapel now?` with rows
`1. Play Isolated Chapel …` / `2. Hold Isolated Chapel - do not play it now`.
Every other land drop in the corpus reads `Land drop: which land do you play now, if any?` with a
final `Play no land right now`. The seat took row 2. Any instruction surface that quotes the
decline row verbatim (RULE 0 does) misses this variant. **Fix shape:** one wording for both arities.

### MED-3 — `plan_missing` is enforced inconsistently on plan-less priority windows
`…274-…-vs-deck126`: the `YOUR PLAN` block is **absent** from seq 18, 19, 21 and 22 (present at 17,
20, 24-27). seq 18 answered with no `PLAN:` line and was accepted; **seq 21** answered with no
`PLAN:` line and was refused `<refused: plan_missing>` — same game, same chain, three windows apart.
The difference appears to be that seq 18 chose `0 (pass)` and seq 21 chose a repeat-N row, but that
rule is nowhere on the page. Cost: 1 of this deck's 2 fallbacks. Also worth asking why the plan
block is dropped at all inside a repeat chain — the seat is required to re-derive a plan it stated
one window earlier, on a window that will be re-offered dozens of times.

### LOW-1 — draw-punisher damage is not narrated
`…284-…-vs-deck162`, last prompt: the log prints 21 consecutive `- You drew X` lines and one
`- You lost 8 life (now 7)`; the 7 → 0 that ended the game (Underworld Dreams + 2x Fate Unraveler,
the block above it prices at 3/draw) is never narrated. No decision followed, so this is cosmetic,
but a post-hoc reader cannot reconstruct the kill from the log.

---

## 3. Guide verdict: **KEEP as is**

No file written to `wave61/deck123/strategy.txt`.

Five of the six games turn on a decision the guide **already** governs explicitly, by name, with a
quoted row and a worked failure:

| seq | what it did | the rule it already had |
|---|---|---|
| `152` s4/s5 | declined Devour Flesh at N=1 | RULE 5: *"use Devour Flesh on the opponent when Tribute to Hunger is not on the list and they have exactly ONE creature"* |
| `152` s10 | `Play no land right now` on a live drop | RULE 0: *"If ANY 'Play' line is listed, take one. Every turn."* |
| `146` s6 | discarded its only Damnation next to a `copy 2 of 2` | Vision Skeins entry: *"name the SPARE … never your only maker, only Alarm or only Damnation"* |
| `162` s31-37 | fired the chain in its own upkeep | RULE 1: *"YOUR OWN UPKEEP IS THE WORST WINDOW"* |
| `125` s36 | Damnation at `destroys 0 … 0 of yours` | RULE 3: *"'DESTROYS 0 OF THEIR CREATURES' IS A CARD THROWN AWAY"* |

And where the guide *was* obeyed it was **right**: BRANCH B2 (`126` s52) called an engine kill claim
false and the next two records proved the guide correct against the engine; the deck130 game is six
turns of correct passes off zero-verdict rows.

There is nothing here to add. The failure mode is **compliance, not content** — the text of every
violated rule is already imperative, already quotes the row verbatim, and already carries the
"WHAT THIS COST" receipt. A sixth restatement is the wave-58 busywork pattern, and the file is at
70,982 B against a 71 KB ceiling, so any addition must evict text that is currently working.
The two remaining losses (deck130 burn with no creatures to remove; deck126's enchantment loop with
no enchantment removal in the sixty) are **deck-construction** outcomes that no guide edit reaches.

## 4. Proposals
None. No `general-proposals.md`, no `skill-proposals.md`.
Everything decisive here is either an engine item (§2) or an existing deck-guide rule that was not
followed; neither is evidence that the general guide or the strategy-writing skill is missing a rule.

---

## What I did NOT check
* The **opponents' seat logs** — I read only deck123's six files. Opponent lines were taken from
  deck123's own rendered GAME LOG and battlefield blocks.
* Any **stderr** file, and the run's `async_drops` / `transport` / timing fields — census and
  prediction adjudication belong to the engine seat, and I made no PASS/FAIL call on any
  lane-{R,S,T,U,V} prediction. HIGH-1 is offered as a repro for C1's residual, not as a verdict.
* Whether Wagic's engine actually iterates the Sanguine Bond ↔ Exquisite Blood loop to zero **in
  general**; I verified it did so in this game (seq 73 → gameend, 19 → 0 / 20 → 39) and did not
  read the C++ that implements it.
* Scryfall — every card fact above came from the primitive; I did not cross-check any of them
  against `api.scryfall.com`, so a primitive that mis-models its card would not have been caught.
* The mulligan asks beyond confirming they happened (6 looks, 1 mulligan, 1 bottom, no chain).
* Guide rules no window here exercised — CHECK 2 (Lightmine Field), the Lord of Lineage / transform
  lines beyond `162` s40-42, the `{spare: …}` and best-X badges. (RULE 2 Marsh Flats and RULE 6
  Greaves WERE exercised and held: 4 cracks on the first window listed, and one equip at `126` s16
  with every later re-offer declined.)
