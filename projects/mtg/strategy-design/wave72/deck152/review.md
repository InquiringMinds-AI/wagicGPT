# Wave-72 deck 152 review — BANT HUMAN MIDRANGE

Corpus `matchups-20260907-163626`, seat files `...ai_baka_deck152-<ptr>-vs-...` (6 games, all natural ends).
**232 decisions, 232 with native reasoning** (`reasoning_chars` min 1,934 / p50 5,187 / max 18,969),
**1 fallback** (`noop_row_zero_reask`, genuine — see MED-2), 0 `reply_truncated`, 0 `action_before_plan`.
`plan_line_missing` 12 (5.2%), `off_protocol_bytes>0` 11 (4.7%). Latency p50 93.5 s / p95 167.3 s.
Live guide read = the wave-71 edit (19,992 B, byte-identical to `wave71/deck152/strategy.txt`). Seat **3-3**.

## 1 — Game by game

| # | Opp | Result | Turn | Life | Deciding decisions |
|---|-----|--------|------|------|--------------------|
| 1 | 123 | **win** | 13 | 28 / -17 | seq 24 lethal count clears → cast nothing; seq 27 the 5-attacker "KILLS them whatever they block" alpha |
| 2 | 130 | **win** | 11 | 20 / 0 | seq 11/17/23 three upkeep Lair offers all PASSED (wave-71 edit 2); clean curve, seq 28 alpha |
| 3 | 126 | **win** | 15 | 20 / -8 | **seq 18 + seq 22 — the wave-71 edits 1 and 4 working together against Sanguine Bond; see §3** |
| 4 | 162 | loss | 19 | 0 / 14 | **seq 34 — see HIGH-1.** Teferi at 6 loyalty was never activated again; seat died at 0 to drain |
| 5 | 146 | loss | 19 | -11 / 7 | **seq 30 + seq 32 — see HIGH-2**; the animated Lair died to a tag that named its killer |
| 6 | 125 | loss | 64 | 0 / 68 | UW control: 4x counter, 2x Supreme Verdict, 2x Staff of Nin, Sphinx's Revelation + Elixir of Immortality. **Not a guide loss** — 17 creatures in the graveyard, opponent's life engine unanswerable by this deck. seq 57 is a real but non-deciding violation (edit 3 below) |

**Game 6 is matchup, not guide.** From turn 34 the opponent gained more per Sphinx's Revelation than this deck
can deal, and Elixir of Immortality reset their library twice. The seat kept deploying (it double-spelled at
seq 69/70 when the rows allowed) and was answered every time. Nothing in a deck guide reaches that.

## 2 — Engine / interface / card items

### HIGH-1 — one HOLD row silences a planeswalker's once-a-turn ability for the whole game
`1788817025-ai_baka_deck152-0x55a9e9cf3f00-vs-ai_baka_deck162.jsonl` seq 34 (turn 13, main 1, 22 vs 14).
Menu rows: `1. +1: tap or untap permanents with Teferi...`, `2. -2: reveal top 3 cards with Teferi...`,
`3. Hold priority - ... the rows above include ACTIVATED abilities that are usable RIGHT NOW, and taking this
row gives every one of them up for as long as these rows stand`. The header also read
`[hold check: every row above was also on the menu at the last window at this seam (6 windows in a row now)]`.
The reply was `CHOICE: 3 (Hold priority)` with a PLAN about *attacking* — the `reasoning` never mentions
Teferi; it is entirely about the combat step it thought it was in.

After that hold, **no Teferi window is ever offered again** (seq 35-45 contain none;
`hold_windows_skipped_priority` = 10 for this game). Teferi's +1 gains 2 life
(`bin/Res/sets/primitives/planeswalkers.txt:3346`, `auto=@counteradded(0/0.1.TeferiEffect) from(this):name(Gain
2 life) life:2 controller` — Scryfall agrees). Six turns of +1 = 12 life in a game the seat lost at exactly 0,
having been at 7 with three Fate Unravelers + Underworld Dreams pinging it down.

The model's read of the row is not unreasonable: a loyalty ability is once per turn, so its row is
byte-identical every turn and the hold's own "until one of the rows above changes" condition can never fire.
Engine half (this is the part worth the core loop's attention): **a hold should be released at the start of
each of the holding seat's turns whenever a suppressed row is a once-per-turn activation** (loyalty
especially) — otherwise the hold is not "pass this window", it is "concede this permanent". Guide half is
edit 1 below. Classification: STRATEGY on a true surface, with an engine design defect behind it.

### HIGH-2 — nothing on the animation menu binds the animated land to the attack rules, and the seat kept animating into a listed killer
`...vs-ai_baka_deck146.jsonl` seq 30 (T16 main 1) → seq 32 (T16 attackers), 17 vs 7.
seq 30 rendered exactly one rung, `1. becomes a 1/1 hydra {rung ceiling: Lair of the Hydra's OWN mana is not
counted toward this activation ...}` — the ceiling is **correct** (2 non-Lair untapped sources ⇒ X=1), but the
`reasoning` fought it: *"I have 3 mana. I can pay {3}{G} to make it a 3/3. The option says 1/1 because it's a
menu, but I can choose X=3 if the game allows, or I just pick 1."* and the PLAN line it wrote says "Animate
Lair into a **3/3** Hydra". It then took the 1/1, added the Aspirant counter to it (seq 31), and at seq 32 sent
it into `A1. Lair of the Hydra (2/2) [their untapped blockers: Nadaar, Selfless Paladin (3/3) (your attacker
dies, their blocker lives); Goblin #2 (2/2) (both die)]` under `ATTACK TOTAL: ... That is NOT lethal`.
Nadaar blocked; the Lair is in the graveyard by seq 36 and the seat had one creature left against a 7-attacker
lethal swing two turns later.

Both halves are guide-fixable (edit 2), but the render could help: the animation row prices mana and duration
and says nothing about the fact that **losing this creature loses a LAND**. A clause on the animation row —
`{if this dies while animated you lose the land}` — would put the real price where the decision is made.
Classification: STRATEGY (surface true and complete). Rung ceiling verified correct against
`borderline.txt:64290-64303` ({X}{G} rungs, `ueot`).

### MED-1 — a counter that changes a creature's death is printed as a bare counter name, and the trade tag prices the fight as if it did not exist
`...vs-ai_baka_deck146.jsonl` seq 25 (T14). Opponent line: `Nadaar, Selfless Paladin {2}{w} (3/3) [vigilance]
[counters: 1x ghostform]`; the attack tag reads `Nadaar, Selfless Paladin (3/3) (both die ...)`. The counter
means the creature **returns to its owner's hand and makes them a 1/1 flier** when it dies
(`planeswalkers.txt:1874`, Kaya's +1: `@movedto(this|graveyard) ... moveto(myhand) and!( token(Spirit Cai) )!`
— Scryfall agrees). The log confirms it: *"Opponent's Nadaar died ... was returned to the opponent's hand ...
Opponent created a 1/1 Spirit Cai token"*. The seat traded its Intrepid Adversary for a blocker that came
straight back plus a bonus body, on a tag that said "both die". Fix: gloss non-standard counters on the
battlefield line the way the `{effect: ...}` frame is glossed, and let the trade tag say
`(both die - theirs returns to their hand)`.

### MED-2 — the `noop_row_zero_reask` fire in this seat is GENUINE (rows quoted)
`...vs-ai_baka_deck125.jsonl` seq 62 (T49). Every one of the 14 animation rows carried
`[this land is TAPPED: animated, it still cannot attack this turn]` **and** `{right now: does nothing this
turn}`; the reply was `CHOICE: 14 (becomes a 14/14 hydra)`. The rescue refused it and the re-ask (seq 63) got
`CHOICE: 15 (Hold priority)` with the reasoning *"animating Lair of the Hydra does nothing this turn"* — the
re-ask that names the refused row worked exactly as designed. **True fire, correct outcome.** (This is the
seat's only fallback in 232 decisions.)

### MED-3 — a main-phase-1 casting window did not fire while two castable cards sat in hand
`...vs-ai_baka_deck146.jsonl` turn 18: the records go seq 33 (T17 blockers) → seq 34 (T18 **Attackers**) →
seq 35 (T18 **Main phase 2**). No main-1 record exists, yet at seq 35 the hand printed
`Teferi, Who Slows the Sunset ... [castable now]` (x2) and `Katilda, Dawnhart Prime {g}{w} ... [castable now]`
off 4 untapped sources — the same rows were available in main 1. `own_turn_windows_skipped` = 10 in this game
(corpus-wide 718), so this is presumably the new BT-M14 skip, but the skipped window had live casting rows.
Cost here was nil (the seat was dead on board either way) — flagged because the skip rule looks wider than
"windows with nothing to do". Engine seat owns the counter; this is a concrete instance.

### MED-4 — the carried-plan header label still collides with the required reply label (wave-71 MED-3, unfixed)
`...vs-ai_baka_deck146.jsonl` seq 20: prompt carries `YOUR PLAN (as you last stated it, ...) reads "..."`, the
reply is `YOUR PLAN: Exile Lolth, Spider Queen ...` + `CHOICE: 1 (...)` — 75 off-protocol bytes and one
`plan_line_missing`, caused by the model copying the label the prompt supplied. Renaming the carry header
(e.g. `PLAN CARRIED FORWARD (from turn 12):`) removes it at no cost.

### LOW-1 — the whole off-protocol charge is again one shape (wave-70 L11, unchanged)
All 11 records with `off_protocol_bytes>0` are a correct plan sentence on line 1 with the `PLAN:` label
missing or replaced (`Attack with both flyers to pressure.` / `CAST ELITE SPELLBINDER IN MAIN 2 ...` /
`I will pass on the land drop.`). None is prose after the action line, none reverses the action, none exceeds
one line. The action label is in caps in the instruction; the plan label is not.

### Verified NOT defects this wave
- **The opponent `{effect:}` perspective frame is live and correct** — `{effect (THEIR card, written from THEIR
  seat: "you"/"your" in this text means THEM, and "opponent" in it means YOU): ...}` on Lolth, Liliana's Caress,
  Underworld Dreams, Ob Nixilis, Staff of Nin. This is the fix for wave-71 HIGH-1, which lost that wave's
  deck126 game; **the same matchup was won this wave** (§3). A card with no "you"/"opponent" in its text
  (Lightmine Field, `...vs-deck125` seq 75) correctly gets the plain `{effect: ...}` — not an inconsistency.
- **The legend-rule annotation now reaches reveal rows** — `...vs-deck123` seq 28:
  `Sigarda ... {you already have this card: 1 on your battlefield - it is LEGENDARY ... (CR 704.5j)} [does NOT
  qualify - goes to "put on bottom"]`. This retires wave-70 MED-1 and pays for a guide deletion (edit 5).
- **The reveal window now names the destination zone** (wave-70/71 MED-2 fixed): `The card you send to "get a
  human" goes to YOUR HAND` (seq 21, seq 28).
- **Katilda's mana bill** rendered once (`...vs-deck130` seq 17, `{paying this taps: Katilda, Dawnhart Prime -
  it cannot attack this turn}` on a Lair activation) and the guide's COUNT THE NAMES rule was obeyed.

## 3 — Did wave 71's edits pay off?

- **Edit 1 (the "That IS lethal - but only if none of them is blocked" lure) + Edit 4 (the converter race) —
  PASS, and together they won game 3.** `...vs-deck126` seq 18: `ATTACK TOTAL: ... That IS lethal - but only
  if none of them is blocked. At least 8 damage lands ... up to 15 life back across their 3 blockers, so
  blocking can leave them as high as 17`, with A1/A2 tagged `[no creature they control can block this
  attacker]` and A3 (Intrepid Adversary, lifelink) carrying three blocking-trigger prices. Answer:
  `ATTACK: A1, A2` — the unblockable pair only, PLAN "Attack with unblockable creatures to race Sanguine Bond".
  Same again at seq 22. Opponent went 21 → 18 → 10 → 2 → -8. **This is the exact window shape that lost the
  wave-71 deck126 game; both wave-71 edits fired and the matchup flipped.**
- **Edit 2 (Lair leads with the pass) — 6 PASS / 2 FAIL, up from 0/3.** PASS: every `[Upkeep offer: ...]` row
  in games 2 and 5 and the first in game 6 (`deck130` seq 11/17/23, `deck146` seq 23/26, `deck125` seq 64,
  several quoting the rule back: *"Pass on the Upkeep offer ... it only lasts until end of turn"*). FAIL:
  `deck125` seq 68 (took a 16/16 **upkeep** rung on T53) and `deck125` seq 62 (a `[this land is TAPPED]` rung,
  caught by the no-op rescue, MED-2). Both fails are inside the already-lost 64-turn game.
- **Edit 3 (land drop never declined) — FAIL, 2 of 2 windows.** `...vs-deck125` seq 57 and seq 58 (T45 main 1
  and main 2) both answered `Play no land right now` with the only card in hand being **Lair of the Hydra**.
  The stated reason is a new one the edit did not name: *"With 16 lands already untapped and no creatures to
  attack with, playing a 17th land that enters tapped provides no immediate advantage."* Widened in edit 3
  below.
- **Mulligan / bottoming — PASS, and tested this wave.** `...vs-deck162` seq 2: M of 7, `1 land`,
  `Playing every land in this hand would not cover any spell in it` → `Mulligan`, exactly the guide's floor.
  seq 3: M of 6, 3 lands, coverage line names two cards → `Keep`. seq 4 bottomed the duplicate Teferi.
- **The Sigarda Coven reveal — PASS, 2 of 2** (`deck123` seq 21 took Katilda, seq 28 took Elite Spellbinder
  over Tovolar's Huntmaster — the cheapest castable Human both times, and the legendary Sigarda row was
  correctly left alone).

## 4 — Guide verdict: **EDIT**

Revised guide at `wave72/deck152/strategy.txt`, **19,995 B** (live 19,992; ceiling 20,000). Amendment-332
grep clean (no wave/seq/corpus/game citation, no counts, no history).

**Edit 1 — the HOLD row is barred while an unused activated ability is on the menu.** Paid by `deck162` seq 34.
before: `THE HOLD-PRIORITY ROW (...) is a pass with one extra effect: when you have ALREADY decided ...`
after: `... is a pass with one extra effect. NEVER take it while any row above it is an activated or loyalty
ability you have not used this turn: the hold gives up every row above it until one of them CHANGES, and a
once-a-turn ability prints the same row every turn, so one hold can silence it for the rest of the game -
answer such a menu with the ability itself or with the plain pass row. Otherwise: when you have ALREADY
decided ...`
Why this shape: the old paragraph told the model *when to prefer* HOLD over a decline and never told it what
HOLD costs. The row's own text states the cost and was not read; the rule now leads with the bar and gives the
alternative answer (the plain pass row) so a bar does not become a forced action.

**Edit 2 — Lair of the Hydra: the animated land is an attacker the attack rules bind, and take the biggest
rung.** Paid by `deck146` seq 30 + seq 32, and `deck125` seq 62.
before: `... Animate in MAIN 1 only, with mana left after your creature, and only when its A-line would read
"[no creature they control can block this attacker]".`
after: adds `"{right now: does nothing this turn}"` to the list of PASS brackets, and ends
`... at the largest rung your OTHER sources pay for, and only when its A-line would read "[no creature they
control can block this attacker]" - once animated it is an attacker like any other and the ATTACKING rules
bind it, so an A-line naming a blocker that kills it means you lose a LAND for nothing.`
Why this shape: the condition existed and was skipped because the guide never connected the animation to the
ATTACKING section's own STOP rule; naming the consequence in the currency the model was counting (a LAND, not
a creature) is what the seq-32 reasoning was missing. The rung sentence answers the seq-30 reasoning that
argued with the menu's ceiling.

**Edit 3 — the land-drop decline loses its two remaining excuses, and Lair is named.** Paid by `deck125`
seq 57 + seq 58.
before: `... never "Play no land right now" - not even on a turn your untapped sources already cover every card
you mean to cast - and never hold a land for a colour you want later;`
after: `... never "Play no land right now" - not on a turn your untapped sources already cover every card you
mean to cast, not because you hold more lands than you can spend, not because the row says the land enters
TAPPED - and never hold a land for a colour you want later; ... Lair of the Hydra in hand is a future attacker
as well as a land: play it like any other.`
Why: the wave-71 wording covered "enough mana"; the reply named "more lands than I can spend" and "enters
tapped" instead. Both are now closed by name, and the last clause makes the specific card that was left in
hand a positive reason rather than a neutral one.

**Edit 4 — Teferi is used every turn once he is out.** Paid by `deck162` seq 34-45 (never activated after T13,
game lost at 0 life).
before: `- TEFERI ({2}{W}{U}) ... you never contort your mana for him. His +1 is a three-stage cascade ...`
after: inserts `Once he is on the battlefield take a loyalty ability EVERY turn - the +1's 2 life is free,
nothing else you run gains life, and a Teferi you never activate is a blank.`
Why: the bullet described how to navigate the +1 *when the seat chooses it* and never said to choose it. It
pairs with edit 1: edit 1 stops the row being silenced, edit 4 says what to do with it.

**Edit 5 — the deploy floor covers the game the seat thinks is lost.** Paid by `deck146` seq 35 (main 2, 5
life, `Katilda ... [castable now]`, answer `Hold priority`, PLAN "Pass turn, accept loss" — the third window
in a row whose plan was a concession).
before: `... and creatures are never held to dodge removal.`
after: `... and creatures are never held to dodge removal - not even in a game you think is lost.`
Why, and honestly: at seq 35 the render itself priced the cast (`you cover 5 of 17, leaving 12 -> you would be
at -7, which still KILLS you`), so this decision cost nothing and is NOT why the game was lost. The clause is
worth its 40 bytes only because the same "accept loss" plan appeared at seq 34 and seq 36 and would have
suppressed a cast that mattered one turn earlier.

**Bytes.** The five edits add ~1,050 B, paid by: the SIGARDA "these rows carry NO legendary warning" workaround
(now false — the engine prints the legend-rule tag, see §2 verified list); the CLEANUP-DISCARD walk-through
compressed to its ordering rule (no cleanup discard window has fired in two waves); the both-halves converter
bullet compressed to the two facts that are load-bearing (it runs on life gained/lost, race with unblockable
only — no both-halves board has appeared in two waves); the mulligan carried-plan and pathway-face bullets and
the Katilda B-row paragraph tightened; and reflow. No rule was removed.

## 5 — Proposals

**None.** HIGH-1's transferable lesson (a hold that suppresses a once-per-turn activation is a concession, not
a pass) is an ENGINE change, not a general-guide or skill change — `wave62/general-strategy.md` is not loaded
at runtime, and `wave68/strategy-writing-skill-v2.md` already carries the primitive-verification and
decision-time-only rules this review leaned on. A proposal restating either would be noise.

## 6 — What I did NOT check

- The other six decks' seats beyond opponent-seat context; I opened no `.stderr` file and did not count the
  `{X}`-announcement payment loss.
- `own_turn_windows_skipped` (10 / 36 / 40 / 35 / 148 / 0 across the six games) beyond the single instance in
  MED-3; the engine seat owns that instrument. Likewise `async_drops` (7 in the deck125 game, 1 in deck126).
- `sibling_window_asks_skipped` and `stop_reached_windows_skipped` are 0 in all six of this seat's gameends; I
  did not read the code to say which of the three explanations applies — that is the engine seat's item.
- `blocker_forecast_multi` (10/16/6) and `blocker_forecast_gang` (2, `...vs-deck162`): I read the two block
  windows that decided games and did not audit every collapsed forecast against the board.
- Card primitives: I verified Teferi (`planeswalkers.txt:3339-3349`), Kaya's ghostform
  (`planeswalkers.txt:1874`) and Lair of the Hydra (`borderline.txt:64290-64303`). I did not re-verify the
  rest of the deck this wave.
- The MULLIGAN section's M-of-5 branch and the BOTTOMING order below 6 cards: only one mulligan and one
  1-card bottom fired, so those bullets carry no evidence this wave and were left unchanged.
