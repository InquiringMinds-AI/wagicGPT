# Wave 72 — deck 146 (Orzhov dungeon midrange) per-deck review

Seat files: `matchups-20260907-163626/*ai_baka_deck146-0x*-vs-*.jsonl` (6 games). Seat census:
**404 model decisions** (ask 279, priority 66, attackers 41, blockers 4, reveal 2, gamestart/system
12), **0 fallbacks**, 0 `reply_truncated`, 0 `action_before_plan`, 0 `menu_pass_no_progress`,
0 `ask_replays_refused` / 46 reserved, 6 `async_drops` (all in the vs-125 game). Reasoning present
on every decision. `plan_line_missing` 31 (7.7 %, wave 71: 9.4 %), `off_protocol_bytes>0` 29.
Latency p50 69-120 s per game, max 305 s. Gameend counters for this seat:
`own_turn_windows_skipped` 93, `hold_windows_skipped` 216, `sibling_window_asks_skipped` 0,
`stop_reached_windows_skipped` 0 (engine-seat items — no seat of mine ever rendered either path).

| # | Opponent | Result | Turn | Final life | Wave-71 result vs same deck |
|---|---|---|---|---|---|
| 1 | 125 | WIN | 29 | 22 / -5 | WIN |
| 2 | 126 | **LOSS** | 17 | -2 / 29 | WIN |
| 3 | 123 | WIN | 23 | 23 / -9 | WIN |
| 4 | 152 | **WIN** | 19 | 7 / -11 | LOSS |
| 5 | 162 | WIN | 15 | 10 / -2 | WIN |
| 6 | 130 | WIN | 17 | 22 / 0 | WIN |

5-1 for the third wave running; the loss rotated from 152 to 126.

## 0. Did the wave-71 edits pay off? YES on three of four, and two engine fixes landed

- **Edit 1 (Silencer name procedure) — PAID, and the engine fixed the surface under it.** All six
  name choices this corpus named a real, castable card with a high remaining-copy count
  (deck123 seq 9 Damnation 4, deck130 seq 22 Lay Waste 4, deck152 seq 6 Elite Spellbinder 4,
  seq 17 Luminarch Aspirant 4, deck123 seq 52 Vision Skeins 4, deck126 seq 17 Sanguine Bond 2).
  **Wave-71 HIGH 1 and HIGH 2 are both FIXED in the engine**: token names are gone from the list
  (CR 201.4), every row now prints `{copies not yet in a public zone: N of theirs}`, and the list
  is sorted by that N. Consequence for the guide: the wave-71 rule's premise ("`{visible now}`
  rows print FIRST") is now FALSE — visible rows sit at 4-5 in the vs-126 list — so the rule is
  rewritten around the new tag (Edit 1 below).
- **Edit 4 (legend rule) — PAID and the engine header is FIXED.** deck152 seq 51 rendered a proper
  `LEGEND RULE CHOICE ... this is the STATE-BASED ACTION of the legend rule, not a cast ... Pick the
  copy you want to LOSE`, and the model picked row 2, the `[summoning sick]` Nadaar — exactly what
  the guide says, and the opposite of wave 71's error. Wave-71 MED 3 closed.
- **Wave-71 MED 4 FIXED**: 72 casting-already-answered windows this seat, **0** still carrying
  `[castable now]` on a hand card (wave 71: 27 of 47).
- **Edit 2 (Hive affordability shrink) — held.** Six animations taken (deck125 seq 41/107/113/120/130/165,
  deck123 seq 30/50/59), every one payable; no unpayable-animation plan.
- **Edit 3 (scoping Sanguine Bond to removal lists) — held**: the block was not quoted at a name ask.

## 1. Game by game — the deciding decisions

**vs 126 (the LOSS, T17).** Deck 126 assembled Sanguine Bond + Exquisite Blood by T15 and the render
called it correctly (`LOOP COMPLETE`, the converter block, per-tag "this is not a priced trade").
The game was lost far earlier, at **seq 6 (T3)** and **seq 7 (T5)**: both Vanishing Verses were
fired at **Pride Guardian (0/3 defender)** and **Wall of Omens (0/4 defender)** — two bodies that
can never attack. Verse is this deck's only maindeck answer to a monocoloured *enchantment*, and
both combo halves are mono-black. The reasoning at seq 7 shows the model *seeing* the problem and
overruling itself on the guide: "Wall of Omens is a 0/4 defender. Is it a threat? ... But the guide
says: '1. Removal ... when their line shows a creature or planeswalker you can hit'". This is a
STRATEGY item and the guide caused it — Edit 2 below. By seq 41 (T17) the model was reduced to
Emeria's Call for two Angels; at **seq 43** it attacked into a lifelink Vampire knowing the loop
would kill it, and its own reasoning shows why that was not the misplay it looks like ("if I don't
block next turn I take 1 from the lifelink Vampire and the loop kills me anyway") — dead either way.
I am not treating seq 43 as a guide failure.

**vs 125 (WIN T29).** Creatureless permission again, and again the Hive was the clock. One clear
misplay: **seq 14 (T7)**, the Pelakka Predation reveal — hand was `Supreme Verdict`, `Fall of the
Gavel`, `Emrakul, the Aeons Torn`, 2 Islands, and the model took **Emrakul**. Supreme Verdict was
cast at **seq 39 (T15)** and swept the Goblin and both Spiders. A 15-mana creature in a 24-land
permission deck is the card they never cast; the sweeper is the card that actually resolves.
Guide Edit 3. (Engine contribution: the reveal rows print no mana cost — see HIGH 2.)

**vs 152 (WIN T19; wave 71's loss).** Straight race won on the back of a wide dungeon board
(seq 47 five attackers, seq 55 seven for 17). **seq 23 (T12)** declined a `(both die)` block of
Intrepid Adversary with Nadaar at 10 life; the guide's rung-2 exception ("unless the attacker it
kills is worth less than your blocker") is exactly what the reasoning invoked, and the seat won,
so I am recording it as a judgment call, not a misplay.

**vs 123 (WIN T23), vs 162 (WIN T15), vs 130 (WIN T17).** Clean curve-outs; every lethal window was
taken on the turn the `ATTACK TOTAL` line called it lethal (seq 63 / 48 / 34). At **deck123 seq 43**
the model cast Soul Shatter against an explicit `VERDICT: taking this row STRANDS Barrowin of Clan
Undurr this turn` and then wrote a plan naming Barrowin the same turn — it overrode the STRANDS
verdict deliberately for the Bloodline Keeper, which was right, but the plan line contradicts the
verdict it just read (see LOW 6).

## 2. Engine / interface / card items

### HIGH 1 — the priority menu's pass ROW exists only on single-option windows, while every window advertises "0 = pass priority"
`src/AIPlayerGPT.cpp:35146` prints `0. Pass priority (take no action this window)` under
`if (baseIndex == 1)` — i.e. only when the menu has exactly one real option. The answer sentence
(`:35328`) is unconditional: `CHOICE: followed by the number (0 = pass priority) ... or "CHOICE: 0
(pass)"`. **36 of this seat's 66 priority windows advertise the pass and print no row for it.**
Repro: `...vs-ai_baka_deck125.jsonl` **seq 180** (T29), a 12-row Kaya/Lolth priority menu whose
tail reads `...12. Hold priority - pass now...` then `CHOICE: ... (0 = pass priority)`. The
reasoning: *"the list of legal actions only includes Kaya abilities and the hold row. There is no
'Pass priority' option except the hold row. This is a common UI quirk ... But if it covers Main 2,
that's bad."* It then answered `CHOICE: 12 (Hold priority - pass now)`.
Measured harm shape: **HOLD was taken at 20 priority windows and in 12 of them the reply's own plan
says "pass"** (deck125 seq 61, 66, 82, 99, 103, 117, 180, ...). The model wants a one-window pass
and buys a durable hold, and the HOLD row on a priority menu explicitly surrenders every live
activated ability ("the rows above include ACTIVATED abilities that are usable RIGHT NOW") — for
this deck that is Kaya, Lolth and the Hive. This is the same absent-decline shape wave 48 fixed for
single-option windows, still live for multi-option ones. Fix: drop the `baseIndex == 1` guard, or
say in the sentence that the pass is unlisted here.

### HIGH 2 — the reveal (Pelakka Predation) list gates eligibility on mana VALUE and never prints a mana cost
`...vs-ai_baka_deck125.jsonl` **seq 14** (T7). Header: `ELIGIBILITY: only a manacost>=3 card may go
to "choose a card"`. Rows:
```
3. Supreme Verdict (sorcery) [can't be countered] {text: ... Destroy all creatures.} [eligible ...]
4. Fall of the Gavel (instant) {text: Counter target spell. You gain 5 life.} [eligible ...]
5. Emrakul, the Aeons Torn (15/15 creature) [flying, can't be countered, ...] [eligible ...]
```
No row carries `{4}{W}{W}` or `{15}`. The one number the header makes decisive is the one number
withheld, and the "biggest body" reads as the biggest threat. The model took Emrakul; Supreme
Verdict wiped its board eight turns later (seq 39). Same shape at deck162 seq 19. Fix: print the
printed mana cost on every reveal row (the cast menu already does).

### MED 3 — the worked example truncates mid-token and reads to the model as a typo in the prompt
`src/AIPlayerGPT.cpp:36038` does `core = core.substr(0, 48)` after stripping only the ` {`
annotation tail, so any row whose name is followed by ` - "card text` or ` (room N of 7 in ...)`
is chopped mid-quote or mid-parenthesis. **79 of this seat's 279 exemplified asks emit a malformed
example**, and in **20** of them the reasoning stops to call the prompt broken:
- deck152 seq 51: `e.g. "CHOICE: 1 (Nadaar, Selfless Paladin #1 (4/4) - "Vigilance -)"`
- deck125 seq 111: `e.g. "CHOICE: 1 (goblin lair (room 2 of 7 in Lost Mine of Phandel)"` →
  *"this seems to be a typo in the prompt's example"*
- deck125 seq 28/31/39, deck162 seq 30: `CHOICE: 2 (Hold priority - pass now, and do not ask me agai)`
  → *"note the typo in the prompt's example 'agai'"*
The example also contradicts its own sentence, which says *"its SHORT NAME in parentheses (the name
only - copy nothing from the {...} annotations)"* while the example copies room paths and card text.
No decision was lost, but under the trust doctrine this is the surface teaching the model to
distrust it. Fix: cut at the first ` - ` / ` (` separator and never emit an unbalanced quote or
paren; the priority menu's own placeholder form (`"CHOICE: 3 (Cast Example Card)"`) is the model.

### MED 4 — an MDFC's back face is offered under a name that appears nowhere else on the screen
`...vs-ai_baka_deck125.jsonl` **seq 35** (T13). Hand line: `Brightclimb Pathway (land: taps for
{W})`. Menu: `1. Brightclimb Pathway (menu text: Play Land) ...` / `2. Grimclimb Pathway [PLAY THIS
AS A LAND ... taps for {B}]`. Nothing says the two rows are one card. Reasoning: *"That's a bit
confusing. It might be a typo in the prompt's menu generation, or maybe I have a Grimclimb in
hand?"* Fix: tag the back-face row `[the other face of <hand card> - one card, either face uses the
same card and the same land drop]`.

### MED 5 — "held back, it CANNOT block: X" sits next to "their untapped blockers: X"
`...vs-ai_baka_deck152.jsonl` **seq 31** (T13):
`A1. Nadaar ... [held back, it CANNOT block: Elite Spellbinder (flying)] [their untapped blockers:
Elite Spellbinder (3/1) (both die)]`. The two brackets name the same creature and read as opposite
claims about who can block whom (the first is about MY blocking, the second about THEIRS). The
reasoning: *"This is contradictory. Usually, 'their untapped blockers' lists creatures that CAN
block."* Fix: word the first clause from the held-back creature's side ("keeping Nadaar home does
not stop Elite Spellbinder - it flies").

### LOW 6 — the STRANDS verdict is read, then contradicted inside the same reply
`...vs-ai_baka_deck123.jsonl` **seq 43** (T19). The row's verdict is arithmetically true
(6 sources, Soul Shatter leaves 3, Barrowin needs 4). The model's first pass misread it
(*"Wait, 6 total - 2 = 4 ... That's a bit confusing"*), recovered, then wrote
`PLAN: ... then cast Barrowin in main phase to build pressure` — a plan the row it just took has
made impossible. Not a false surface; a note that the consequence-first wording is being parsed on
the second read, and that nothing checks a PLAN against a STRANDS verdict it was shown.

### LOW 7 — reply shape (31 `plan_line_missing`)
27 = the plan sentence is present but the literal `PLAN:` label is dropped; 2 = the model uses the
recap header's wording `YOUR PLAN:` (deck125 seq 61, 180); 2 = action line only (deck125 seq 148,
deck152 seq 16). All parsed, no fallback. Wave 71's `YOUR PLAN (as you last stated it ...)` header
collision is down from 4 to 2 but is the same collision.

## 3. Guide verdict: EDIT

Live guide 19,979 bytes -> revised **19,859 bytes** (`wave72/deck146/strategy.txt`). Five edits;
two add rules, three pay the bytes back.

**Edit 1 — REWRITE the SILVERQUILL SILENCER row-reading rule (the surface changed under it).**
- before: `"{visible now: ...}" IS A MINUS, NOT A PLUS - it counts copies already spent, and the list prints those rows FIRST. Take the first row that fits: (1) an UNMARKED row ... for the card their decklist holds most copies of ... (2) a "{visible now: N in their graveyard}" row whose decklist count is above N. NEVER a name their decklist does not hold - that is a TOKEN ("Goblin") ...`
- after: `Every row prints "{copies not yet in a public zone: N of theirs}" and the list is SORTED BY THAT N - that N is the decision. Take the highest N, breaking ties toward a counterspell or a sweeper, then the missing half of a pair on their line ... a body last, an N of 0 never. "{visible now: ...}" only says where the SPENT copies sit - it never raises a row.`
- paid for by: **deck126 seq 17** (the wave-72 render prints the remaining-copy count on every row
  and sorts by it; the `{visible now}` rows were at 4 and 5, not first, so the old rule's ordering
  claim is now a false statement about the screen) and by the engine's token-name drop, which makes
  the whole TOKEN clause dead text. Net: **-260 bytes** and one stale teach removed.

**Edit 2 — VANISHING VERSE MAY NOT BE SPENT ON A 0-POWER OR [defender] BODY (the rule that earned its bytes).**
- added to the VERSE bullet: `A body printing "[defender]" or a power of 0 never attacks you: NEVER
  SPEND VERSE ON ONE. Hold the card instead - Verse is this deck's only answer to a monocoloured
  ENCHANTMENT or ARTIFACT, and those are what beat you.` and to cast-order entry 1: `for Verse, only
  when its target list names one of THEIRS with power above 0 and no "[defender]"`.
- paid for by: **deck126 seq 6** (Pride Guardian 0/3 defender) and **seq 7** (Wall of Omens 0/4
  defender) — both Verses gone by turn 5, both combo halves mono-black and unanswerable from turn 15
  on, and the seq-7 reasoning shows the cast-order line overruling the model's own correct doubt.
  The guide already states this exact test for Kaya's -3 ("A permanent printing '[defender]' or a
  power of 0 never attacks you"); this makes the deck's other exile obey the rule its sibling has.
  It does not weaken "never sit on removal" — it names the one target class that is not removal-worthy.

**Edit 3 — PELAKKA PREDATION'S REVEAL LIST: take the card that gets CAST (the rule that earned its bytes).**
- added: `ON ITS REVEAL LIST take the card that will actually be CAST: a sweeper ("Destroy all",
  "Each ... sacrifices") or a counterspell ahead of ANY creature, however big - the huge body is the
  card they never reach.`
- paid for by: **deck125 seq 14 -> seq 39** (Emrakul taken, Supreme Verdict left in hand and cast
  eight turns later into a board wipe). The guide's existing Predation line ("a bomb, never a
  curve") is what pointed at the 15/15; nothing in the guide covered the discard *choice*.

**Edit 4 — REWRITE THE HOLD-PRIORITY ROW around the surface that now exists.**
- before: the one-literal test on `[you declined this exact list N times already this turn]`.
- after: HOLD is a DURABLE pass; read the `[hold check: ...]` line ("every row above was also on
  the menu at the last window" -> HOLD; "N rows above are new" -> decline this window only); never
  hold a list offering a Kaya/Lolth loyalty ability or the Hive animation; with no numbered pass row
  on a priority list the one-window pass is `CHOICE: 0 (pass)`.
- paid for by: the old literal renders 53 times but `[hold check: ...]` renders **212** times, so the
  guide was testing the rarer surface; and by the **12 priority HOLDs whose own reply said "pass"**
  (HIGH 1). This is the route-around form: a decision rule that does not depend on the missing pass
  row, and it retires when the row is printed.

**Edit 5 — the bytes: CUT the SHIELD SPHERE bullet, the `[repeat: ...]` Hive line, and compress the LEGEND and DRAW-PRICE blocks.**
- Shield Sphere: rendered **0 times** in this seat across wave 71 and wave 72 (deck162 runs exactly
  one copy in 60); its rule is a three-line exception to CHECK B, and CHECK B's own "(neither dies"
  reading survives without it. `[repeat: activated this turn N times already]`: **0 renders** in two
  waves and CHECK 2 already carries the same test (wave 71 compressed it; this wave removes it).
  LEGEND: the engine header is now correct and self-explaining, so only the pick rule is kept.
  DRAW-PRICE: same rule, three lines instead of four.
- These four cuts free **~600 bytes**; Edits 2, 3 and 4 spend ~480 of them.

## 4. What I did NOT check
- Opponent seat files beyond board context; no audit of any opponent deck's play.
- The 6 `async_drops` in the vs-125 game — not traced to a seam.
- BOTTOM and CLEANUP DISCARD: **zero windows again** (all six hands were untouched-7 keeps, and no
  hand ever exceeded 7 at cleanup), so those two blocks are untested for a third wave. Same for
  CHECK 0b, the DRAW-PRICE bracket (0 renders), and Shield Sphere.
- BLOCK LADDER: only **4** blocker decisions in six games (all vs 152); still effectively untested.
- `sibling_window_asks_skipped` / `stop_reached_windows_skipped` are 0 for all six of my games; I
  did not read the code to say which of the three causes the brief lists applies — engine seat.
- Corpus-wide census, lane BT/BU/BV/BW/BX adjudication, and the corpus's 3 fallbacks (none in this
  seat) — engine seat.
- Whether HIGH 1's 12 pass-vs-hold mismatches ever cost a game: they did not in these six, and I
  did not design a test for it.

No `general-proposals.md` and no `skill-proposals.md`. Every item above is an engine/render item for
the core loop or a deck-specific guide teach; nothing I found is a gap in the method that
wave68/strategy-writing-skill-v2.md teaches — Edits 1 and 4 are that skill's "the guide must name
the surface that is actually on the screen" discipline applied to two surfaces the engine changed
under the wave-71 guide.
