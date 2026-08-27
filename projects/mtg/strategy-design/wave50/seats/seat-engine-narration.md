# Wave-50 seat review — ENGINE / NARRATION

Corpus `matchups-20260827-115759`, binary master 67aeffe45 (lanes V d9144d62c, Z 5b6b44c44,
X c37805358, Y 7858a403f, W b39b80375). 42 seat logs, **1,864 records**, **1,780 asks** (ask
1,214 / priority 376 / attackers 102 / blockers 37 / discard 28 (NEW kind) / reveal 15 / bottom 8)
+ 42 gamestart + 42 gameend. **21/21 natural** — 42/42 seats carry `gameend`; max seat **166**
records (`deck123-vs-deck125`, the 54-turn game; turn-fields up to 53). Wave 49 was 1,915 asks,
max seat 129, max 16 priority windows/turn.

⚠ The brief's CORPUS NOTE (deck123-vs-deck162 dead-looped, no gameend, pointers `0x55e867ceaa40`
/ `0x55e8669ce290`) does NOT describe this corpus — same finding as wave 49. This run's 123v162
seats are `...deck123-0x55de8e828b00...` / `...deck162-0x55de8d50c1f0...`, both carry `gameend`
(deck162 won 21-0 at turn 13; results.tsv row `162 123 0`). Every rate below uses 21/21.

Every string quoted is the emitter's ACTUAL output, read with python over the JSONL and
`/usr/bin/grep` over the stderr files. Seat names `deckA-vs-deckB` = the deckA seat's JSONL; `seq`
is the record's own field; `turn` is the JSONL field (still log turn − 1 — D-8 carried, see the
discard rows below). `- Paid` receipts and all narration counts are DEDUPED from each record's
`events` field (the new lines since the previous window), not from the carried log window — the
prompt-side count of `- Paid ` is 10,339 (carries) vs **396** actual receipts.

**Health.** Fallbacks **3 / 1,780 = 0.17%** by the `fallback` field, all `stale_echo`, all
Baka-executed (`choice: -1`), all three on deck126 seats and all one shape (D-4 below).
`answer_replaced` **5** (all audited, all genuine re-answers — table below). `commit_retracted` 0.
`parse_note` **10** (not 0 as briefed): `attack_last_line_taken` 5, `repeat_count_under_two` 3,
`blocks_last_line_taken` 2. None of lane Y's new notes fired (`rejected_line_skipped`,
`no_pass_sibling_taken`, `no_pass_reask*`) and no `degenerate_decode` / `unparsed_reply` /
`repeat_count_reask` / `named_row_reask` — `[RE-ASK]` rendered **0** times. `mana_only_windows_skipped`
max **203** (`deck126-vs-deck146`; was 265). Max priority windows in one turn **10**
(`deck123-vs-deck130` turn-field 16; was 16). Mulligans (real hands): 34 seats kept 7, 8 mulliganed
to 6 (7 kept 6, 1 went to 5 and kept 5); 8 `bottom` records.

Corrections to the numbers I was handed: `parse_note` is 10, not 0; the **overall** max prompt is
**25,793** (a PRIORITY record, `deck126-vs-deck146` seq 52, 21 rows, 63% log) — 25,018
(`deck123-vs-deck125` seq 163, Main 2) is the max ASK; `- Paid` receipts are 396 by the events
field (357 tied to a `- You cast` in the same block, 39 to activations / casts from exile).

---

## VERDICT TABLE — Set B, by the emitter's string

| Lane | Prediction | Verdict | Count / citation |
|---|---|---|---|
| V d9144d62c | casts with tapped == 2x pips 0/N (was 12/234) | **PASS (0/236)** | `Mana available: N total` diffed across consecutive same-turn windows for 236 non-X casts: 0 at 2x. The only over-tap is `deck152-vs-deck126` seq 23 Sigarda `{1}{g}{w}{w}` 6→1 (5 for 4 pips) — receipt seq 24 `- Paid {1}{g}{w}{w} for Sigarda, Champion of Light with Deserted Beach #1, Branchloft Pathway, Katilda, Dawnhart Prime, Elite Spellbinder` names 4 sources; Katilda's own tap took the fifth |
| V | counter on the stack, then a payment attempt for the countered spell in stderr: 0 | **PASS (0/26)** | 26 `Action added to stack: Cancel|Essence Scatter|Dream Fracture|Fall of the Gavel`; between each and its `Resolving Action on stack: <counter>` there is no `-  Target: <countered card>` / `- Next card to play: <countered card>` (e.g. `game-125v123-1787849914.stderr` line 915: Cancel on Thraben Doomsayer → `AIPlayerGPT: chose to cast nothing` ×2 → `Resolving Action on stack: Cancel`) |
| Z 5b6b44c44 | one `- Paid` per cast/activation (two = a D1 recurrence) | **PASS (0/396)** | per seat, no two `- Paid ... for X` without a `- You cast X` / `- You used: ... with X` between them |
| Z | repeat takes of exactly 50 with no arithmetic: 0 | **PASS (0/9 takes)** | repeat row (`..., repeated N times, then stop [you control M creatures right now; you name N on the CHOICE line, e.g. "CHOICE: 2 (Create human with Thraben Doomsayer x<N>)"; ...]`) offered **18**, taken **9**: x21, x0, x17, x1, x1, x3, x3, x28, x25 (seats/seqs below). No x50; every take with N ≥ 2 came with a `Stop at M = …` line in the reply |
| Z | verbatim `x<N>` echo → `repeat_count_reask` | **UNTESTED** | 0 replies contain the literal `x<N>` |
| Z | all-mana single-row windows with an empty stack reaching the model: 0 (was 3) | **PASS (0)** | the only mana rows rendered are `Add N green mana with Overgrown Battlement [#N] [cost: Tap]` (34 renders, never alone); 0 priority windows whose action rows are all `Add …`. stderr `only mana production and no pending cost; auto-passing without a model call` **379** |
| Z (T) | generic pips paid from a multi-colour source while a mono source could pay: 0/N by string | **FAIL (69/386)** | `deck162-vs-deck123` seq 5 sources `Drowned Catacomb #1 {u} or {b}; Drowned Catacomb #2 {u} or {b}; Swamp {b}` → seq 6 `- Paid {2} for Howling Mine with Drowned Catacomb #1, Drowned Catacomb #2` (Swamp untapped); seq 7→8 the same with `Swamp #1`, `Swamp #2` both untapped; `deck125-vs-deck152` seq 34→35 `- Paid {w} for Path to Exile with Seachrome Coast` with `Plains #1 {w}; Plains #2 {w}` untapped; `deck152-vs-deck123` seq 15→16 `- Paid {1}{w} for Intrepid Adversary with Plains #2, Overgrown Farmland #2` with `Plains #1 {w}` untapped. Method: for each receipt, sources available = the nearest preceding prompt's `Those sources, one per untapped card:` line (creature sources and sources spent by earlier receipts in the same block removed); minimum multi-colour sources needed computed by exact matching; 78/386 receipts used more multi-colour sources than necessary, 69 of them on costs with generic pips. By deck: 125 22, 152 17, 146 14, 123 12, 162 10, 126 3 → **D-2** |
| Z | `[repeat:]` tag leads with `you control M creatures;` | **PASS** | `[repeat: you control 4 creatures; activated this turn 1 times already. This turn will not advance while you keep taking this option.]` (`deck123-vs-deck162` seq 33); 72 tag renders |
| X c37805358 | gain-recipient disagreeing with the primitive 0/N | **PASS (0/7)** | Tribute to Hunger rows `they control 1 creature - Shield Sphere is sacrificed, you gain 6` (×2), `… Fate Unraveler …, you gain 4`, `… Thraben Doomsayer …, you gain 2`, `… Bloodline Keeper …, you gain 3`, `… Triumphant Adventurer …, you gain 1` ↔ primitive `mtg.txt:124074 text=Target opponent sacrifices a creature of their choice. You gain life …` (`auto=… toughnesslifegain targetopponent …`). Devour Flesh rows `they control 1 creature - Katilda, Dawnhart Prime is sacrificed, they gain 1 - the sacrificing player gains, not you`, `… Perimeter Captain …, they gain 4 - the sacrificing player gains, not you` ↔ `mtg.txt:29136 text=Target player sacrifices a creature, then gains life …` (`… targetcontroller …`) |
| X | no option row > 3,000 chars; no prompt > 30,000 from the target clause | **PASS** | max row **1,715** (`deck152-vs-deck126` seq 31 Fateful Absence); max prompt 25,793. `legal targets right now:` clauses: 373 rows, max clause **288** chars (`deck130-vs-deck123` seq 55: `Human #1-#18 (creature 1/1) [doesn't untap during its controller's untap step] x18, Vampire #1-#10 (creature 2/2) [flying, …] x10, the opponent, Siege-Gang …`); 8 clauses carry an `xN` collapse. Wave 49's 28,755-char row class is gone |
| X | `(0 able to attack)` 0 occurrences | **PASS (0)** | sweeper rows read `{right now: destroys N of their creatures (N without a restriction against attacking), N of yours}` — 81 renders (Main 1 55, Main 2 26), all on the caster's own turn; the opponent-turn form (`able to attack right now`) never arose on a sweeper row — that half UNTESTED |
| X | casts of a named card by the punished seat 0 (was 3) | **PASS (0/76)** | `[named:` rendered on the opponent-battlefield line **76** prompts, e.g. `deck126-vs-deck146` seq 35 `Silverquill Silencer {b}{w} (4/3) (printed 3/2) [named: Vampire]`; 0 of those windows cast the named card. Narration `- Opponent's Silverquill Silencer named Bloodline Keeper` etc. 62+53+23 lines. The one cast of a formerly-named card (`deck123-vs-deck146` seq 26 Bloodline Keeper) came with the opponent's battlefield at `Swamp [tapped]; Plains [tapped]; Brightclimb Pathway [tapped]` — Silencer gone, cast legal |
| X | `DRAW FORECAST (theirs)` on 100% of `DRAW PUNISHERS … yours` prompts (was 0/63) | **PASS (50/50)** | `DRAW FORECAST (theirs): their next draw step draws N cards (1 + Howling Mine 1) = N x N = N life to you from your punishers above.` (39 renders across 3 shapes); the punished-seat `DRAW FORECAST: your next draw step …` on 81 |
| X | bare `// <name>"}` endings 0 (was 6) | **FAIL (15) — a third emitter path** | all 15 are the `{target text:}` of a Pathway LAND inside a `legal targets right now:` clause on land-destruction rows: `Hengegate Pathway (land) {target text: "{T}: Add {W}. // Mistgate Pathway"}` (`deck130-vs-deck152` seq 15, 19; Mistgate 11, Grimclimb 4 — all deck130 seats, Molten Rain / Stone Rain / Lay Waste rows). The wave-49 path is fixed: `Cast Card Normally … // Pelakka Caverns (text omitted)"}` 4/4, `… // Emeria, Shattered Skyclave (text omitted)"}` 1/1; `(text omitted)` 145 renders → **D-6** |
| Y 7858a403f | executed lines with a verdict tail 0/N | **PASS (0/1)** | the one candidate: `deck126-vs-deck125` seq 19 coded lines `CHOICE: 3 (Cast nothing right now)` / `CHOICE: 4 (Cast Exquisite Blood) -- Wait, the list only goes to 3. Let me re-read the options.` / `CHOICE: 3 (Cast nothing right now)`; executed **3**, `answer_replaced: false`. No `rejected_line_skipped` note was stamped (the line is also out of range on a 3-row menu, so which guard caught it is not observable — D-8 LOW) |
| Y | exemplar-equal replies failing 0/N | **PASS (0/507)** | 507 replies whose first line equals the prompt's `e.g. "CHOICE: …"` exemplar; 0 fallbacks |
| Y | `CHOICE: 0` reaching Baka without sibling/re-ask 0/N | **UNTESTED** | 0 replies wrote `CHOICE: 0` on a `this ask has no pass row` ask |
| Y | "The game is lost" carries 0 (was 5) | **PASS (0) — thin** | 0 carried plans open with `The game is lost`. The one reply containing the phrase (`deck130-vs-deck126` seq 71, priority) put it in prose (`I must pass. The game is lost unless …`) and its PLAN line opened `The opponent controls Sanguine Bond and Exquisite Blood, creating a lethal life-gain/loss loop …`, which was then carried **5** windows — a plan whose action is "pass and hope" survived the names-no-action test (observation, LOW) |
| Y | garbage decodes reaching Baka as `unparsed_reply` 0/N | **UNTESTED** | 0 replies with > 15% non-ASCII; 0 `unparsed_reply`, 0 `degenerate_decode` |
| Y | `plan_echo_count` report field; content-keyed expiry | **observed** | field on 413 records, max **26** (`deck125-vs-deck146` seq 50-51). Longest identical carried `YOUR PLAN (as you last stated it):` run **50** windows (`deck125-vs-deck123` seq 49-98, `Keep a counterspell's mana open; Staff of Nin only from a row reading leaves 5 or more; ping their face every turn.`) with the model restating it in 6/50 replies — by design now (a valid guide-sourced plan), so not a defect; wave 49's 28-run is superseded by this 50-run |
| Y | `(your plan's target is not on this window)` on TARGET CHOICE | **PASS (rendered 6)** | `deck146-vs-deck126` seq 43, `deck125-vs-deck152` seq 35, `deck152-vs-deck125` seq 82, `deck152-vs-deck162` seq 9, `deck125-vs-deck146` seq 54 |
| W b39b80375 | `You revealed N cards but none was a legal target` at a foreign chooser 0 | **PASS (0)** | 15 reveal records; `- You revealed N cards and took Katilda, Dawnhart Prime (put in hand)` class 64 lines; the string never rendered |
| W | edict resolutions with no sacrifice 0/N | **FAIL (1/13)** | `deck146-vs-deck162` seq 25 `Cast Soul Shatter {2}{b} …` with `Opponent battlefield (11 permanents listed, of which 1 is a creature, …)` → seq 26 events `- Paid {2}{b} for Soul Shatter with Shineshadow Snarl #2, Swamp #1, Swamp #2 / - You cast Soul Shatter / - Your Soul Shatter resolved and went to your graveyard` — no sacrifice line; opponent seat `deck162-vs-deck146` seq 15 `- Your Master of the Feast resolved and entered the battlefield`, seq 16 `- Opponent cast Soul Shatter / - Opponent's Soul Shatter resolved …`, seq 17 `- You declared blockers: Master of the Feast blocks Goblin`. The other 12 edict takes all show the victim (`- Opponent's Goblin died`, `- Opponent used: sacrifice a creature or planeswalker with Soul Shatter targeting Intrepid Adversary`) → **D-1** |
| W | every cleanup `You discarded` preceded by a `discard` record N/N | **PASS (26/26)** | 28 `discard` records; 26 `- Cleanup discard (hand N, limit 7): you chose …` lines, each in the events of the record right after its `discard` record. The 2 unnarrated records (`deck125-vs-deck130` seq 12, `deck146-vs-deck162` seq 28) are followed directly by `gameend` — no later window to carry the line. The 17 other `- You discarded` lines are cycling (`Forgotten Cave`) and opponent effects |
| W | self-target damage/destroy rows taken 0 (was 2) | **PASS (0/124)** | `{this hits YOUR permanent}` on 124 rows (`Deal 2 damage with Siege-Gang Commander targeting Siege-Gang Commander [your battlefield] {this hits YOUR permanent} [cost: {1}{r}, Sacrifice]`, `Deal 3 damage with Staff of Nin targeting Wall of Omens #N [your battlefield] {this hits YOUR permanent}`); 0 taken. `Every action here targets your own permanent; 0 (pass) is the usual answer.` rendered **0** — no window had only self-target rows: UNTESTED |
| W | rows retired only after 2 pass-declines | **observed via stderr only** | `AIPlayerGPT[phN]: all actions pass-declined this turn; passing` **801**; not adjudicated per row |
| carry | max priority windows per turn ≤ 10 | **PASS (10)** | `deck123-vs-deck130` turn-field 16 |
| carry | Baka fallbacks ≤ 0.31% | **PASS (0.17%)** | 3/1,780 |
| carry | `mana_only_windows_skipped` max ≤ 1,000 | **PASS (203)** | |
| carry D-8 | 1-based `turn` | **still open** | every `discard` record: `turn` = last log header − 1 (e.g. `deck123-vs-deck162` seq 23 `turn: 9`, prompt `=== Turn 10 - YOUR turn ===`) |

Set A is not this seat's.

---

## Repeat row, receipts, discards — the numbers
- Repeat row offered **18**, taken **9**, pass chosen 9. Takes (seat, seq, M shown → N named, stop
  stated): `deck123-vs-deck162` 34 (M=5 → x21, stop 25), 35 (M=26 → **x0**, stop 25);
  `deck123-vs-deck130` 26 (M=3 → x17, 24), 30 (M=21 → x1, 23), 31 (M=22 → x1, 27), 42 (M=24 →
  x3, 27), 46 (M=29 → x3, 20); `deck123-vs-deck126` 28 (M=3 → x28, 30), 32 (M=33 → x25, 25).
- Receipts `- Your repeated activation ran N of the N times you named`: 21, 17, 3, 3, 28, 25 — no
  early stop; `- You used: Create human with Thraben Doomsayer - ran 1 time (you named 0)` ×1 and
  `(you named 1)` ×2 for the three `repeat_count_under_two` takes.
- `deck123-vs-deck126` seq 32: M=33, the reply's own stop was 25, it named x25 → 58 creatures. The
  `[you control M creatures right now; …]` surface was correct; the arithmetic is the model's
  (deck-seat matter).
- `- Paid` receipts 396; 10 carry a comma-named source (`with Katilda, Dawnhart Prime, Elite
  Spellbinder`; `with Urborg, Tomb of Yawgmoth, Scrubland`) — ambiguous to a reader (D-5).
- Discard asks (28): prompt shape `Cleanup step (CR 514.1): your hand has 9 cards and your maximum
  hand size is 7, so you must discard exactly 2 cards now. Name EXACTLY 2 card numbers … keep the
  cards your plan needs and discard what you can spare.` then numbered rows with cost / type /
  card text, answer `PUT: 1, 4`. Choices: duplicate copies (Damnation ×2, Essence Scatter ×2,
  Tribute to Hunger 1 of 3) or excess lands (Tundra ×3, Island ×2, Island/Seachrome/Island) in 19;
  deck125 discarded Supreme Verdict, Sphinx's Revelation, Emrakul, Final Judgment, Lightmine Field
  across its 9 (strategy, its seat's call). Highest-MV is Baka's policy only; the GPT ask is free.
  All 28 executed (`chosen_text` = the named cards, count = choice).

## Prompt chars per decision kind — wave 49 → wave 50

| kind | n49 | mean49 | max49 | n50 | mean50 | max50 | p90 latency 49 → 50 (s) |
|---|---|---|---|---|---|---|---|
| ask | 1,422 | 10,485 | 34,333 | 1,214 | 9,102 | 25,018 | 49.2 → 56.3 |
| priority | 364 | 12,204 | 28,487 | 376 | 12,038 | 25,793 | 53.2 → 55.9 |
| attackers | 91 | 10,690 | 25,701 | 102 | 11,227 | 25,087 | 49.5 → 57.7 |
| blockers | 21 | 9,782 | 17,616 | 37 | 11,267 | 25,450 | 56.8 → 76.2 |
| reveal | 8 | 11,883 | 16,303 | 15 | 14,048 | 20,689 | 102.5 → 52.8 |
| bottom | 8 | 2,170 | 2,660 | 8 | 2,200 | 2,542 | 90.8 → 98.8 |
| discard | — | — | — | 28 | 9,534 | 25,402 | — → 57.1 |

Ask mean −13%, max −27% (the 34 KB target-clause row is gone). The log window is the size driver
in every kind: mean log share ask 54% / priority 58% / attackers 62%; the six largest prompts are
63-83% log (`deck125-vs-deck123` seq 96-98: 20.5-21.0 KB of 25.3-25.4 KB at turn-fields 52-54).
Latency max 587 s (an ask); blockers p90 rose to 76 s with 37 (vs 21) blockers asks.

---

## NEW DEFECTS (record + emitter string)

**D-1 HIGH — Soul Shatter resolved with no sacrifice.** `deck146-vs-deck162` seq 25 →
26 (above). stderr `game-146v162-1787849900.stderr` line 1363 `Resolving Action on stack: Soul
Shatter` is followed directly by the seat's next land-drop/casting lines — no `ActionLayer::
doReactTo`, no `AIPlayerGPT: targeting with  -> 1 target(s), first: …`, no `StackAbility`. The
working case `game-146v152-1787849904.stderr` (`deck146-vs-deck152` seq 24) reads `Resolving Action
on stack: Soul Shatter / ActionLayer::doReactTo 140 / CHECKING Spell / AIPlayerGPT: targeting with
-> 1 target(s), first: Intrepid Adversary (3/1) … [your battlefield] / Action added to stack:
StackAbility.  (Source: ) / Resolving …`. Primitive `borderline.txt:106093 auto=ability$!name(Sacrifice
a creature or planeswalker) … sacrifice notaTarget(creature[manacost=convertedcost:highest:*:
myBattlefield]|myBattlefield)!$ opponent`. The victim would have been Master of the Feast (MV 3,
the opponent's only creature). Candidates: lane W's own-chooser / hold-while-unresolved logic
(the sacrifice chooser belongs to the FOREIGN seat) or the `convertedcost:highest` filter with a
single candidate; not traced (this seat does not run wagic). 1/13 edict takes; the 12 others
narrate the victim.

**D-2 HIGH (by count) — generic pips paid from duals while mono lands sat untapped.** 69/386
receipts; citations in the table. The lane-T ordering ("colour-count ASC") is not what the
receipts show; the wave-49 Fog Bank sample (D-9 there) was the same shape and is now adjudicable.
Cost: strands colours in exactly the response windows counters live in (`deck125` is the
counterspell deck and the worst offender at 22).

**D-3 MED — `x0` on the repeat row executed once.** `deck123-vs-deck162` seq 35 reply `CHOICE: 2
(Create human with Thraben Doomsayer x0) / PLAN: Stop at M = 25. M is 26 now. Pass.` → seq 36
events `- You used: Create human with Thraben Doomsayer - ran 1 time (you named 0)`, M 26 → 27
(`repeat_count_under_two`). A named 0 is a decline; treat as pass (or `[RE-ASK]`), never floor to 1.
The two `x1` takes (`deck123-vs-deck130` seq 30, 31) are harmless singles.

**D-4 MED — in-range index + off-menu name → `stale_echo` Baka fallback, not a `[RE-ASK]`.** All
3 fallbacks: `deck126-vs-deck125` seq 14 `CHOICE: 1 (Cast Sanguine Bond)` on a menu whose row 1 is
`Cast Overgrown Battlement {1}{g} (0/4) …`; `deck126-vs-deck162` seq 16 `CHOICE: 1 (Cast Sanguine
Bond)` vs row 1 `Cast Chromatic Lantern {3} …`; `deck126-vs-deck130` seq 13 `CHOICE: 1 (Cast
Exquisite Blood)` vs row 1 `Cast Tribute to Hunger {2}{b} {right now: they control 0 creatures - at 0
this does nothing} …`. Sanguine Bond / Exquisite Blood were in hand but not on the menu (one black
source). `parseChoice` sets `staleEcho` via `echoNoMatch && choice > 0 && echoStaleForIndex(choice)`
(AIPlayerGPT.cpp ~13565) and returns −1 → Baka, which then cast row 1 in two of the three (`- Paid
{1}{g} for Overgrown Battlement …`, `- Paid {3} for Chromatic Lantern …`). The `named_row_reask`
path (index PAST the menu, wave 49: 1/1 recovered) should also own this shape: `[RE-ASK] "Cast
Sanguine Bond" is not on this list. Answer with a number from 1 to 2.`

**D-5 LOW — receipt source separator collides with card names.** `- Paid {1}{w} for Intrepid
Adversary with Katilda, Dawnhart Prime, Elite Spellbinder` (`deck152-vs-deck123` seq 13) reads as
three sources for two pips; `with Urborg, Tomb of Yawgmoth, Scrubland` likewise (10 receipts).
Use `; ` (the `Those sources` line already does).

**D-6 LOW — bare back face on `{target text:}` of MDFC lands in target clauses.** 15 renders
(table). `markBareBackFaces` reaches the hand row and the option-menu row but not the target-clause
text.

**D-7 LOW (carried D-8) — JSONL `turn` = log turn − 1.** Fourth corpus; the new `discard` records
show it plainly (28/28).

**D-8 LOW — rejection not stamped when the rejected line is also out of range.** `deck126-vs-deck125`
seq 19 (table): the right thing executed, but `parse_note` is empty, so the D7 guard's first live
firing is invisible.

**Observed, not defects:** `answer_replaced` 5 — `deck126-vs-deck146` seq 37 `CHOICE: 2 (Cast
nothing right now)` → `CHOICE: 1 (Cast Chromatic Lantern)` (1 executed); `deck146-vs-deck126` seq 18
`ATTACK: A1, A2` → `ATTACK: none` ×3 (none); `deck152-vs-deck126` seq 40 `ATTACK: A1, A5` →
`ATTACK: A1, A2, A3, A4, A5` (all five); `deck123-vs-deck146` seq 35 `CHOICE: 1 (Create vampire …)` →
`CHOICE: 0 (pass)` (pass); `deck130-vs-deck126` seq 60 `CHOICE: 6 (X = 1)` → `CHOICE: 4 (X = 3)`
(X=3). All last-clean-line, all with reasoning between. `attack_last_line_taken` 5 /
`blocks_last_line_taken` 2 — reasoning bodies with a `Final decision:` line; correct reads.

---

## Lategame specimen candidate (invariant 0)
`1787849917-ai_baka_deck123-0x55f93524f640-vs-ai_baka_deck125.jsonl` seq **163** (the 25,018-char
Main-2 ask at turn-field 53 of the 54-turn game, 166-record seat) paired with
`...deck125-0x55f933f33530-vs-ai_baka_deck123.jsonl` seq **96-98** (a `discard` ask and two priority
windows at 25.3-25.4 KB, 81-83% log, the carried 50-window plan). Alternative for the repeat
machinery: `...deck123-0x556aeb0dee00-vs-ai_baka_deck126.jsonl` seq **28-33** (x28 then x25 → 58
creatures, both receipts, the `[you control M creatures right now` row at M=3 and M=33).

## WAVE-51 DOCKET PROPOSAL (engine / UX only), ranked
1. **D-1** edict no-op: Soul Shatter resolved, foreign seat never sacrificed (repro: game 146v162
   stderr line 1363 vs 146v152's working trace; suspect the lane-W chooser gate or the
   `convertedcost:highest` filter at one candidate). A spell that resolves without effect is the
   costliest class of engine bug — silent to both seats.
2. **D-2** autotap source order: pay generic from mono sources first, coloured pips from the
   scarcest colour (69/386 receipts; the receipt lane Z shipped makes this a one-line test now).
3. **D-4** route index-in-range + off-menu-name to the named-row `[RE-ASK]` (3/3 fallbacks this
   corpus are this shape; Baka cast the wrong card in 2 of them).
4. **D-3** `x0` on the repeat row = decline.
5. **D-6** `(text omitted)` on `{target text:}` back faces.
6. **D-5** `; ` separator in `- Paid … with …`.
7. **D-7** 1-based `turn` in the JSONL.
8. **D-8** stamp `rejected_line_skipped` before the range check.
9. Watch only: log share 63-83% of the largest prompts; no prompt > 26 KB; blockers p90 76 s.
Carried, not re-derived: D15-D17, D20-D26 from wave49/engine-ledger.md (D16 FORECAST mirror and
D17 MDFC marker VERIFIED shipped above).

## Falsifiable predictions for wave 51 (if the docket lands in order)
- Edict/sacrifice takes whose next events carry no victim line: 0/N (was 1/13).
- `- Paid` receipts using more multi-colour sources than the minimum over the untapped set: 0/N
  (was 78/386; 69/386 on generic pips).
- Baka-executed `stale_echo` on a reply whose index is in range: 0/N; every off-menu name → one
  `[RE-ASK]`, ≥ 50% recovered.
- `ran 1 time (you named 0)`: 0/N.
- `// <name>"}` bare endings anywhere (hand row, option menu, target clause): 0/N (was 15).
- 21/21 natural; Baka-executed fallbacks ≤ 0.31%; max priority windows/turn ≤ 10; no prompt > 30,000.

## What I did NOT check
- Any Set A guide prediction (deck seats' work).
- D-1's mechanism in source (did not run wagic, did not trace `ownChooser()` / the sacrifice
  filter); the two stderr traces are the whole evidence.
- Whether Baka's autotap order is deterministic per land order (D-2 is adjudicated by outcome only).
- Per-row adjudication of "rows retired after 2 pass-declines" (stderr count only).
- The Vita/human-seat fixes; `NarrationCycleHolder` on a period > 1 cycle (none arose).
- Card facts: only the three primitives quoted (Tribute to Hunger, Devour Flesh, Soul Shatter) were
  read; no Scryfall cross-check was made.
