# Wave-49 seat review — ENGINE / NARRATION

Corpus `matchups-20260827-094106`, binary master 7e94544f1 (Vita fixes + lanes R/U/T/S).
42 seat logs, **1,999 records**, **1,915 asks** (ask 1422 / priority 364 / attackers 91 /
blockers 21 / reveal 9 (8 with a prompt) / bottom 8) + 42 gamestart + 42 gameend. **21/21
natural** — 42/42 seats carry `gameend`; max seat **129** records (`deck130-vs-deck125`); longest
games turn-field 60 (`deck125-vs-deck126`), 54, 50. Wave 48 was 1,729 asks, max seat 133.

⚠ The brief's CORPUS NOTE (deck123-vs-deck162 dead-looped, no gameend) does NOT describe this
corpus: both `...deck123-0x563fceb01d00-vs-...deck162` and its opposite carry a `gameend`
(deck162 won 20-0 at turn-field 16); the pointers quoted (`0x55e867ceaa40`) are not in this run.
The note is carried from an earlier corpus. Every rate below uses 21/21.

Every string quoted is the emitter's ACTUAL output, read with python over the JSONL
(`/usr/bin/grep` + `python3`). Seat names `deckA-vs-deckB` = the deckA seat's JSONL; `seq` is the
record's own field; `turn` is the JSONL field (still the log's turn minus one — D-8 carried).

**Health.** Fallbacks **6 / 1,915 = 0.31%** by the `fallback` field — but two of the six are the
NEW one-shot re-asks (`repeat_count_reask` 1, `named_row_reask` 1), both **recovered** on the
next window; Baka-executed fallbacks are **4 / 1,915 = 0.21%** (`unparsed_reply` 3, `stale_echo`
1) — in band. `commit_retracted` 0. `answer_replaced` **10** (all audited, table below).
`parse_note`: `repeat_count_under_two` 3, `attack_last_line_taken` 3, `named_row_not_offered` 3,
`echo_index_conflict` 2, `multi_answer_first_taken` 2, `blocks_last_line_taken` 1,
`repeat_count_reask_recovered` 1, `echo_index_conflict_ambiguous` 1, `named_row_reask_recovered`
1. `repeat_count_missing` **0** (was 4). Explicit `CHOICE: 0` passes on priority: **205**.
`mana_only_windows_skipped` max **265** (`deck126-vs-deck125`; was 835).

Corrections to the numbers I was handed: fallbacks are 6 by field (5 + `repeat_count_reask`);
`named_row_not_offered` is **3**, not 1 — two of them are informational notes on a reply whose
FIRST clean line was `CHOICE: 0 (pass)` and whose second `CHOICE:` named an off-menu card
(`deck146-vs-deck130` seq 39 `CHOICE: 6 (Cast Triumphant Adventurer)`, `deck146-vs-deck162` seq
26 `CHOICE: 4 (Cast Kaya the Inexorable)`); the pass executed, nothing was replaced.

---

## VERDICT TABLE — Set B, by the emitter's string

| Lane | Prediction | Verdict | Count / citation |
|---|---|---|---|
| R fa1fddf02 | no blockers prompt > 15,000 chars | **FAIL (4/21, by ≤ 2.6 KB)** | max **17,616** `deck146-vs-deck152` seq 55 (turn-field 23); 16,708 seq 49; `deck152-vs-deck146` seq 33 (15,368), 39. NOT combat rows: seq 55 has 2 B-rows + 3 A-rows and **14,547 of its 17,616 chars are the game log** (250 `- ` lines, turns 1-23). The combat block is 1,770 chars. The bar was set for combat enumeration, which is fixed; the residual is log growth in 20+-turn games |
| R | no prompt > 30,000 | **FAIL (1/1,915)** | **34,333** `deck125-vs-deck123` seq 13 (ask, Main phase 1, turn-field 10). Filled by ONE row: `3. Cast Path to Exile {w} ... - legal targets right now: Lord of Lineage (creature 5/5) [...], Vampire #1 (creature 4/4) [flying, doesn't untap during its controller's untap step], Vampire #2 (...), ... Vampire #322 (...)` = **28,755 chars, 323 handles**, while the battlefield line two sections up reads `Vampire #1-#322 (4/4) (printed 2/2) [flying, doesn't untap ...] x322`. The `legal targets right now:` clause is the last uncollapsed enumeration → **D-1 (HIGH)**. Next-largest prompts are 28,487 / 28,406 / 28,153 (turn-fields 31, 53, 40) and are 78-84% game log |
| R | no B-row repeats a parenthetical > 2x | **PASS** | 0/21 blockers prompts; 52 `may block` clauses, 8 carry `(all: ...)`, 3 carry an `A<n>-A<m>` range |
| R | 0 same-name A/B rows split by another name | **PASS** | 0/112 combat prompts (was 2/71 + the 61 KB case) |
| R | header attacking + "more able" ≤ creature count | **PASS** | 39/39 headers of the form `N are creatures, K of them are attacking right now, M more able` satisfy K+M ≤ N |
| U 2d6b16d8f | every Bond+Blood board prompt says `Both halves of a life LOOP` | **PASS** | 223 prompts carry it (`...on THEIR battlefield` 123, `...on YOUR battlefield` 100); 0 prompts with both names on one battlefield line lack it. Direction sentence verbatim: `Any life they gain, or any life you lose, chains until YOU are at 0 - so ANY nonzero payment on a tag above is fatal, not merely expensive.` A-row tag rendered 9x: `(their life LOOP is in play: any life they gain or you lose in this combat chains without limit - fatal to you, not a trade)` |
| U | 0 non-`none` ATTACK lines into THEIR loop | **PASS (2/2)** | attackers asks under `on THEIR battlefield`: `deck152-vs-deck126` seq 31 and 40, both `ATTACK: none`. The third LOOP attackers ask (`deck126-vs-deck130` seq 39, `ATTACK: A1, A2, A3`) is the loop on YOUR side — correct |
| U | 0 "net 0" replies | **PASS** | 0/1,915 (`net 0`/`net zero`, case-insensitive) |
| U | every Sphinx's Revelation / Clue row under a punisher priced | **UNTESTED** | 0 `Cast Sphinx's Revelation` rows rendered under `DRAW PUNISHERS` this corpus; 0 Clue rows. `[DRAW PRICE:]` rendered **48** times, e.g. `deck125-vs-deck162` seq 27 `[DRAW PRICE: casting this draws YOU 7 cards (their Forced Fruition), and their Underworld Dreams #1, Underworld Dreams #2, Ob Nixilis, the Hate-Twisted deal you 21]` — the opposing-cast-trigger row class works |
| U | DRAW FORECAST present wherever DRAW PUNISHERS is | **FAIL by the string (98/161), PASS by intent** | `DRAW FORECAST: your next draw step draws 1 card = 1 x 1 = 1 life to the punishers above.` is on 98/98 prompts whose header reads `DRAW PUNISHERS on the battlefield: theirs`; on 0/63 whose header reads `...: yours - Underworld Dreams. Every card the OPPONENT draws costs them 1 life to yours.` (e.g. `deck162-vs-deck125` seq 13). The forecast is only emitted for the punished seat. The mirror line (`their next draw step costs them N`) would be useful to the punisher's seat — LOW, D-6 |
| U | no PLAN echoed verbatim > 5 windows | **FAIL by the string; mechanism partly working** | longest identical `YOUR PLAN (as you last stated it):` run **28** windows (`deck125-vs-deck126` seq 69-96, `Answer their threats, resolve Staff of Nin from a row reading leaves 5 or more, ping their face every turn.`). In that run the model RE-STATED the same PLAN line in only 5/27 replies (seq 79, 85, 88, 91, 96); the other 22 replies carried no `PLAN:` line at all, so the carried plan persisted through 10 consecutive windows without a restatement (seq 69-78) before the first restatement. Same seat seq 97 carries NO plan (expired at last). The 5-echo counter appears to count only windows since the last restatement but the restatement resets it — so a plan the model re-says every ~6th window never expires. 39 runs > 5 identical prompt plans pool-wide (most are this deck125 guide-sourced plan) |
| U | 0 "the game is lost" carries | **FAIL (5 windows)** | `deck123-vs-deck125` seq 53, 54, 55, 56 and `deck130-vs-deck125` seq 125 carry `YOUR PLAN (as you last stated it): The game is lost. ...`. The model stated it fresh at seq 52, 53, 54 (so 53-55 are one-window carries of a just-said plan) and the guard "names no action" cannot fire: the text goes on to name cards (`...Thraben Doomsayer/Bloodline Keeper to build an army...`). A carry rule keyed on the opening clause (`The game is lost`) would catch all 5 |
| U | bare back-face endings 0/N | **FAIL (6/183)** | `// <Name> (text omitted)` rendered **177** times; **6** blocks still end bare: `... That player discards that card. // Pelakka Caverns` (`deck146-vs-deck125` seq 6, 88; `deck146-vs-deck152` seq 39; `deck146-vs-deck126` seq 11; `deck146-vs-deck130` seq 10) and `... indestructible until your next turn. // Emeria, Shattered Skyclave` (`deck146-vs-deck152` seq 52). All 6 sit on the `Choose an option for <MDFC>:` menu's `Cast Card Normally [cost: ...] {card text: ...}` row — a second emitter path that the marker did not reach (the same card's hand/cast row on the same seat DOES carry the marker: seq 5 vs seq 6). D-7 LOW |
| T a2fd9684a | 0 casts tapping a colour-C source while a non-C source could pay the generic | **UNTESTED by string; 1 counter-sample** | no narration line names which sources a payment tapped; adjudicated by diffing `Those sources, one per untapped card:` across consecutive windows. Sample `deck162-vs-deck125` seq 26 → 27: `Cast Fog Bank {1}{u}` with sources `Drowned Catacomb #1 {u} or {b}; Island #1 {u}; Island #2 {u}; Drowned Catacomb #2 {u} or {b}`; after: `Drowned Catacomb #1 {u} or {b}; Island #1 {u}` — the generic pip was paid by a two-colour dual while a mono `Island` sat untapped. Not the prediction's exact wording (every source here makes {u}), but against "colour-count ASC". Needs a `- Paid {1}{u} with Island #2, Drowned Catacomb #2` receipt to be adjudicable (D-9 LOW) |
| T | creature-tapping casts in a lethal-on-board pre-combat window 0 | **PASS (0/2)** | `{paying this taps: ...}` rendered on **10** rows; taken **2** (`deck152-vs-deck162` seq 11, 16: `4. Cast Sigarda, Champion of Light {1}{g}{w}{w} ... {paying this taps: Luminarch Aspirant, Katilda, Dawnhart Prime - they cannot attack this turn}`), neither lethal-on-board (opp 19 / 18 life, boards of 2-3 small creatures). Model-side note: both replies then planned `Attack with Katilda ... and Sigarda` — the tag was read past, not misrendered |
| T | edict casts at N=0: 0/N | **PASS** | edict rows `{right now: they control N creatures - at N this does nothing}` **123** rendered, chosen **0**; `- they choose which one` 45 rendered, 3 chosen (N=7, 2, 2); named-sacrifice form (`- Emrakul, the Aeons Torn is sacrificed, you gain 15`) 9 chosen at N=1. Attack-punisher form `they control 0 creatures able to attack - deals 0 until they have an attacker` chosen once (`deck125-vs-deck130` seq 50, Lightmine Field — a permanent, correctly not an edict) |
| T | sweeper casts on `destroys 0` rows: 0/N | **PASS** | `{right now: destroys N of their creatures (K able to attack), M of yours}` rendered 177 (+66 `exiles`); 87 read `destroys 0`; chosen **0/87**. 5 sweeper takes, N = 4, 1, 323, 3, 1 (`deck125-vs-deck123` seq 13: `1. Cast Supreme Verdict {1}{u}{w}{w} {right now: destroys 323 of their creatures (0 able to attack), 0 of yours}` → the 322-Vampire board was wiped) |
| S 63e456fdf | off-menu-name replacements executed 0 | **PASS** | `named_row_not_offered` 3: one → `[RE-ASK]`, two → first clean line (`CHOICE: 0`) executed, off-menu line ignored |
| S | false `answer_replaced` 0 | **PASS (10/10 audited)** | in all 10 the executed index is the reply's LAST coded line and differs from the first: e.g. `deck146-vs-deck125` seq 40 `CHOICE: 1 (Cast Soul Shatter)` → `CHOICE: 3 (Cast nothing right now)` (reasoning: "Soul Shatter is a sorcery"); `deck152-vs-deck146` seq 33 `BLOCKS: B1:A1, B2:A1, B3:A2` → `BLOCKS: B1:A1, B2:A1, B3:none`. But see **D-2 (HIGH)**: `deck123-vs-deck125` seq 94 the last coded line was a NEGATION and it executed |
| S | `repeat_count_missing` without a preceding `repeat_count_reask` 0 | **PASS** | `repeat_count_missing` 0/1,915; `repeat_count_reask` 1 (`deck123-vs-deck125` seq 11, reply `CHOICE: 2 (Create vampire with Bloodline Keeper, repeated N times, then stop)`) → seq 12 prompt `[RE-ASK] You chose the repeat row but named no count. Answer again with the number of repeats on the CHOICE line, in the row's own format.` → reply `CHOICE: 2 (Create vampire with Bloodline Keeper x19)` → seq 13 receipt `- Your repeated activation ran 19 of the 19 times you named`. 1/1 recovered |
| S | index-past-menu Baka fallbacks without a `named_row_reask` first 0; ≥ 50% recovered | **PASS (0/1; 1/1 recovered)** | `deck146-vs-deck126` seq 9 `CHOICE: 4 (Cast Barrowin of Clan Undurr)` on a 3-row menu → seq 10 `[RE-ASK] "Cast Barrowin of Clan Undurr" is not on this list. Answer with a number from 1 to 3.` → `CHOICE: 1 (Cast Pelakka Predation)`. Was 4/5 of the wave-48 fallbacks; now 0 Baka fallbacks of this class |
| S | exit row `Cast nothing right now (combat comes next this turn)` on own first main | **PASS** | rendered **525** times; plain `Cast nothing right now` 281 (Main 2 / opponent's turn) |
| S | `kCastAnsweredFact` on the priority ask after a cast ask | **PASS** | `You have already answered this phase's Casting decision (a card you did not cast there is not re-offered below). The rows below are the OTHER actions available now.` rendered **125** times (e.g. `deck162-vs-deck125` seq 19) |
| carry (wave-48 predictions) | max priority windows per turn ≤ 10 | **FAIL (16)** | `deck123-vs-deck125` turn-field 9 (Upkeep): 16 windows built 322 Vampires (singles at seq 9, 10, 13-15; repeat takes x19, x1, x1, x2, x25, x50, x120, x100; pass at 23). Next: 7, 6, 5 |
| carry | fallback rate ≤ 0.31% | **PASS** | 0.21% Baka-executed (0.31% counting the two recovered re-asks) |
| carry | `mana_only_windows_skipped` max ≤ 1,000 | **PASS** | 265 |

Set A is not this seat's.

---

## Repeat row, `[RE-ASK]`, receipts — the numbers
- Repeat row (`..., repeated N times, then stop [you name N on the CHOICE line, e.g. "CHOICE: 2 (Create vampire with Bloodline Keeper x50)"; ... N is at most 200]`) **offered 16, taken 12, pass chosen 4** (`deck123-vs-deck130` seq 24, 25; `deck123-vs-deck125` seq 23, 26).
- Counts named on the 12 takes: x17, x5, x1, (countless → re-ask), x19, x1, x1, x2, x25, x50, x120, x100. The three `x1` takes are `repeat_count_under_two` (ran once; the single row was right there — cosmetic).
- Receipts, distinct: **8** — `ran 17 of the 17`, `5 of the 5` (`deck123-vs-deck130` seq 22, 23), `19 of the 19`, `2 of the 2`, `25 of the 25`, `50 of the 50`, `120 of the 120`, `100 of the 100` (`deck123-vs-deck125` seq 13, 19-23). All read `ran N of the N times you named` — no early stop occurred. The 75 receipt lines in prompts are these 8 carried through the log window.
- `[RE-ASK]` prompts: **2**, both recovered (above). `named_row_reask_exhausted` 0.
- `[repeat:]` tag verbatim: `[repeat: activated this turn 1 times already; you control 2 creatures. This turn will not advance while you keep taking this option; you have taken it 3 times in a row with no other action in between.]` (29 renders).
- The 322-token turn: 16 priority windows, 5,044-7,585-char prompts, then `deck125-vs-deck123` seq 13 (the 34 KB Path to Exile row) → `CHOICE: 1 (Cast Supreme Verdict)` and the board was gone. The loop machinery closed a 322-body loop in one turn; the cost moved to the OPPONENT's screen (D-1).

## Prompt chars per decision kind — wave 48 → wave 49

| kind | n48 | mean48 | max48 | n49 | mean49 | max49 | p90 latency 48 → 49 (s) |
|---|---|---|---|---|---|---|---|
| ask | 1,240 | 8,162 | 27,987 | 1,422 | 10,485 | **34,333** | 55.7 → 49.2 |
| priority | 364 | 11,331 | 27,721 | 364 | 12,204 | 28,487 | 47.7 → 52.4 |
| attackers | 71 | 9,588 | 24,153 | 91 | 10,690 | 25,701 | 50.6 → 46.6 |
| blockers | 26 | 12,733 | 61,240 | 21 | 9,782 | 17,616 | 56.1 → 55.1 |
| reveal | 19 | 12,006 | 15,555 | 8 | 11,883 | 16,303 | 49.7 → 56.0 |
| bottom | 8 | 2,107 | 2,370 | 8 | 2,170 | 2,660 | 82.3 → 79.3 |

Blockers max 61,240 → 17,616 (lane R did its job; the residual is log). Ask mean rose 28% — this
corpus has three 50-60-turn games (turn-fields 60, 54, 50) whose late prompts are 22-24 KB of log
each; the log window, not any emitter, is now the size driver in every kind. Latency max 727 s
(`deck126-vs-deck123` seq 16, 6,024-char prompt, 7,582-char reply — reasoning, not prompt size).

**Fallbacks (4 Baka-executed):** `deck123-vs-deck130` seq 31 (see D-3), `deck146-vs-deck125` seq
82 and `deck125-vs-deck130` seq 101 (`unparsed_reply`: the reply is byte garbage — `__1� ... «2 «':
_::` / `hot一1 曾Hot曾微 ...` — model-side decode failure, 25 s / 48 s, nothing to render),
`deck146-vs-deck152` seq 54 (`stale_echo`: `CHOICE: 1 (Cast Vanishing Verse)` on a 2-row target
menu `Wolf (2/2) / Wolf (2/2)` — the model echoed the previous window's cast row as the name).

---

## NEW DEFECTS (record + emitter string)

**D-1 HIGH — `legal targets right now:` enumerates every token; 34 KB row.** `deck125-vs-deck123`
seq 13: `3. Cast Path to Exile {w} {leaves 5 of your 6 untapped mana sources untapped} - legal
targets right now: Lord of Lineage (creature 5/5) [...] {target text: "..."}, Vampire #1 (creature
4/4) [flying, doesn't untap during its controller's untap step], Vampire #2 (...), ..., Vampire
#322 (...) {card text: ...}` — 323 handles, ONE distinct body, 28,755 chars, while the battlefield
line already renders `Vampire #1-#322 (4/4) (printed 2/2) [...] x322`. 25 rows pool-wide carry ≥
10 targets (`Stone Rain`/`Lay Waste` against 30 lands: `deck130-vs-deck125` seq 115-125, 1.2 KB
each). Fix: reuse the battlefield's `#a-#b ... xN` collapse for consecutive same-name same-body
handles in the target list (one clause, ~150 chars), and cap distinct entries with `(+N more like
these)`.

**D-2 HIGH — the "last clean coded line" rule executes a line the model wrote to REJECT.**
`deck123-vs-deck125` seq 94: first line `CHOICE: 3 (Cast Thraben Doomsayer)`; last coded line
`CHOICE: 1 (Cast Lightning Greaves) is wrong because we need to cast Do[omsayer first]`; reasoning
ends `So Choice 3 is correct for this window.`; `chosen_text` = `Cast Lightning Greaves {2} ...`,
`answer_replaced: true`. Next window's log: `- You cast Lightning Greaves - Opponent cast Cancel -
Your Lightning Greaves was COUNTERED`; Doomsayer stayed in hand at 2 life vs 57. A coded line is
CLEAN only if nothing follows its closing parenthesis (or only whitespace); `... is wrong because`,
`... would`, `... instead of` must disqualify it. 1/10 replacements this corpus; the other 9 were
genuine re-answers.

**D-3 HIGH — the prompt's own worked example is unparseable when the option name contains
parentheses.** `deck123-vs-deck130` seq 31, damage-order ask: rows `1. Goblin (1/1) [...]`, the
prompt's format line reads `e.g. "CHOICE: 1 (Goblin (1/1))" (a worked example of the format using
the first option ...)`; the model replied exactly `CHOICE: 1 (Goblin (1/1))` → `unparsed_reply`,
and its trailing `CHOICE: 0 (Pass)` was executed (`answer_replaced: true`, choice 0 on a
3-option order ask with no pass row). Either strip the `(P/T)` from the example's short name, or
parse a nested parenthetical (`CHOICE: N (` … balanced `)`).

**D-4 HIGH — casts that get RESPONDED TO tap twice.** Diffing `Mana available: N total` across
consecutive windows for 234 non-X casts: **12 casts tapped exactly 2x their pips and left the
difference floating** — `deck146-vs-deck125` seq 32 `Cast Silverquill Silencer {b}{w}` with 6
sources → next window `Mana available: 2 total ... Those sources: Swamp #1 {b}; Plains #1 {w} |
Already in pool: {b}{w} (2 mana ALREADY produced and floating right now ...)` and the battlefield
shows `Plains #2 [tapped]; Brightclimb Pathway [tapped]; Hive of the Eye Tyrant [tapped]; Swamp
#2 [tapped]` (4 for a 2-pip spell); `deck162-vs-deck125` seq 33 `Cast Howling Mine {2}`, 7
sources → 3 untapped + `{u}{u}` floating (all four blue sources gone; Cancel on the stack, no blue
to respond with); also seq 39/49/76 of `deck146-vs-deck125` (`{b}{w}` casts, 4 tapped),
`deck123-vs-deck125` seq 46/69 `Idyllic Tutor {2}{w}` (6 tapped), `deck125-vs-deck126` seq 118
`Elixir {1}` (2 tapped). **11 of the 12 are casts the opponent answered with a counterspell on
the stack** (`- You cast X` / `- Opponent cast Cancel|Essence Scatter|Fall of the Gavel`); 13
other answered casts did not double-tap. Wave 48 had 7/7 the same shape (`deck146-vs-deck125`
seq 47 `Silverquill Command {2}{b}{w}` 8 tapped, seq 53 `Kaya {3}{b}{w}` 10 tapped) and the
wave-47 corpus (`matchups-20260826-182155`) has **0** — this began with the wave-48 binary
(E1 + lanes P/O/N/Q), not lane T. The floating mana drains at end of phase, so each such cast
costs its price twice and empties the seat's response mana in the exact window a counterspell is
on the stack. Repro shape: GPT seat casts, Baka/GPT opponent responds before the cast's priority
window; look at whether the cast's `AIAction` (or `ManaEngine::autoTapForCost` via the cast
click) is applied a second time when the interrupted priority round re-enters the seat.

**D-5 MED — carried plan never expires while the model re-says it every ~6 windows; and
"The game is lost" carried 5 times.** Table rows above. `deck125-vs-deck126` seq 69-78: ten
windows with NO `PLAN:` in any reply and the same carried line. Expiry should count windows since
the plan was last stated, not reset on a restatement that changes nothing; a plan opening `The
game is lost` (or naming no legal action on the current menu) should not be carried at all.

**D-6 LOW — `DRAW FORECAST` only for the punished seat.** 0/63 `DRAW PUNISHERS on the
battlefield: yours` prompts carry a forecast; the punisher's seat would use `their next draw step
draws 1 card = 1 life to you`. Also: the `...: theirs` header already prices the draw step; the
seat opposite deck162 (deck125, Sphinx's Revelation deck) never cast Revelation under a punisher
this corpus, so the `draw:X` pricing (wave-48 D-6) stays untested.

**D-7 LOW — `(text omitted)` marker missing on the `Choose an option for <MDFC>:` menu's `Cast
Card Normally` row.** 6/183 back-face endings (all Pelakka Predation / Emeria's Call); the hand
row for the same card carries it.

**D-8 LOW (carried, still open) — JSONL `turn` = log turn − 1.** `deck125-vs-deck123` seq 13
`turn: 10` while its log header reads `=== Turn 11 - YOUR turn ===`.

**D-9 LOW — no payment receipt.** Nothing in the log names which sources a cast tapped; lane T's
colour-source prediction cannot be adjudicated by string. One line `- Paid {1}{u} with Island #2,
Drowned Catacomb #2` per cast (already computed by `selectAutoTapProducers`) would make D-4 and
the lane-T ordering visible in every future corpus — and `deck162-vs-deck125` seq 26 (Fog Bank
paid its generic from a dual while an Island sat untapped) suggests the ordering has a case to
answer.

**Observed, not a defect:** `multi_answer_first_taken` 2 (`deck125-vs-deck130` seq 68, 96: two
clean `CHOICE:` lines naming Staff of Nin #1 then #2 — the model listed a sequence; first
executed, the second was offered again next window). `echo_index_conflict` 2 +
`_ambiguous` 1 — in band. The three `x1` repeat takes — the model used the repeat row as a
single (harmless, 1 activation each).

---

## Lategame specimen candidate (invariant 0)
`1787841678-ai_baka_deck125-0x561c77675570-vs-ai_baka_deck123.jsonl` seq **13** (the 34,333-char
ask: `Vampire #1-#322 ... x322` collapsed on the battlefield line, 323 handles enumerated on the
Path to Exile row, Supreme Verdict row `destroys 323 of their creatures (0 able to attack)`) paired
with `...deck123-0x561c789917c0-vs-ai_baka_deck125.jsonl` seq **11-23** (the repeat-row re-ask,
the x19/x25/x50/x120/x100 takes and their receipts). One turn, both seats, every lane-S string
and the one enumeration lane R missed.

---

## WAVE-50 DOCKET PROPOSAL (engine / UX only), ranked
1. **D-4** double-tap on answered casts (mana paid twice, response mana gone with a counter on
   the stack; 12 casts this corpus, 7 last corpus, 0 before the wave-48 binary — a regression).
2. **D-2** clean-line rule: a coded line followed by prose is not an answer (executed a rejected
   option; the model's stated choice was thrown away).
3. **D-3** the worked example `CHOICE: 1 (Goblin (1/1))` must parse (nested parens) or not be
   emitted with `(P/T)` inside.
4. **D-1** collapse `legal targets right now:` over same-name same-body handles (34 KB → ~6 KB;
   the last uncollapsed enumeration).
5. **D-5** plan-carry expiry keyed on windows-since-stated + never carry `The game is lost`.
6. **D-9** `- Paid {cost} with <sources>` receipt (makes lane T adjudicable; cheap).
7. **D-7** `(text omitted)` on the MDFC option menu's cast row.
8. **D-6** DRAW FORECAST mirror line for the punisher's seat.
9. **D-8** 1-based `turn` in the JSONL (third corpus quoting off-by-one turns).
10. Watch only: log window is now the size driver (22-24 KB of a 28 KB prompt at turn 40+); a
    per-turn cap or older-turn compaction is the next 10 KB, but no prompt exceeded 35 KB.
Carried, not re-derived: D14-D20 from wave48/engine-ledger.md.

## Falsifiable predictions for wave 50 (if the docket lands in order)
- Casts with `tapped == 2 x pips` (source-count diff across windows) 0/N (was 12/234).
- `answer_replaced` where the executed line has trailing prose after `)`: 0/N.
- `unparsed_reply` on a reply that equals the prompt's own worked example: 0/N.
- No prompt > 30,000 chars; no single option row > 3,000 chars (was 28,755).
- No carried plan identical for > 5 consecutive windows without a restatement; `The game is lost`
  carried 0/N.
- `{card text:}` blocks ending on a bare `// <name>`: 0/N (was 6).
- 21/21 natural; Baka-executed fallbacks ≤ 0.31%; every `[RE-ASK]` recovered.

## What I did NOT check
- Any Set A guide prediction (deck seats' work).
- Lane T's source-ORDER rule beyond the one Fog Bank sample — no receipt string exists to
  adjudicate it; the D-4 double-tap dominates any diff-based measurement.
- Whether D-4's second payment comes from the cast click being re-applied or from the response
  window's own autotap — correlation with "opponent responded" is 11/12 + 7/7, mechanism not
  traced in source (this seat does not edit src/).
- The Vita/human-seat fixes (not LLM-visible).
- `NarrationCycleHolder` on a period > 1 cycle; none arose.
- Card-fact claims: none made in this file.
