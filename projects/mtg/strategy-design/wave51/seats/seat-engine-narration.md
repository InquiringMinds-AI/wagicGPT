# Wave-51 seat review — ENGINE / NARRATION

Corpus `matchups-20260827-155545`, binary master c010ddb47 (lanes A 42f2eff2b, B df8ff7641, C 4cf3745a7,
D ac7c5f5be, E 9c772cd3d, F 7c0a04d1d + Vita fixes ca8a71e14 / dd8663146). 42 seat logs, **2,213
records**, **2,129 asks** (ask 1,493 / priority 445 / attackers 110 / blockers 31 / discard 31 / reveal 13 /
bottom 6) + 42 gamestart + 42 gameend. **21/21 natural** — 42/42 seats carry `gameend`. Max seat **301**
records (`deck146-vs-deck125`, an **80-turn** game; wave 50 max was 166 / 54 turns). Max prompt **33,536**
(`deck146-vs-deck125` seq 279, priority, turn 78) — **3 prompts over lane R/X's 30,000 bar**, all in that
one game, all 71-78% log window (diagnosis below).

⚠ The brief's CORPUS NOTE (123v162 dead loop, pointers `0x55e867ceaa40` / `0x55e8669ce290`) does NOT
describe this corpus (third wave running). This run's 123v162 seats are `...deck123-0x55db5c1994a0...` /
`...deck162-0x55db5ae57fd0...`, both carry `gameend`. Every rate below uses 21/21.

Every string quoted is the emitter's ACTUAL output, read with python over the JSONL (`events`,
`options_text`, `prompt`, `reply`) and `/usr/bin/grep` over the stderr files. `seq` is the record's own
field. Narration counts are DEDUPED from each record's `events` field; the prompt-side count of `- Paid `
is 16,779 (carries) vs **452** actual receipts.

**Health.** Fallbacks by the `fallback` field **5 / 2,129 = 0.23%**: `named_row_reask` 3 (all three
`named_row_reask_recovered` on the next record — `[RE-ASK]` rendered 3 times, first live firings),
`unparsed_reply` 1 (D-2 below — same root cause as an `echo_index_conflict_ambiguous` that executed a
dead card), `stale_livelock` 1 (**NEW class**, D-1 below — cost a 5/5 lord and 10 tokens). Baka-executed
decisions: 2 (the livelock and the unparsed reply). `answer_replaced` 18 (all audited — table at the end;
all last-clean-line with reasoning between, one `deck123-vs-deck125` pattern repeated 5×: `CHOICE: 5
(Cast Idyllic Tutor)` → `CHOICE: 6 (Cast nothing right now)`). `commit_retracted` 0. `parse_note` 12:
`named_row_reask_recovered` 3, `attack_last_line_taken` 2, `named_row_not_offered` 2 (+1 compound),
`stale_echo_in_range` 2 (+1 compound), `echo_index_conflict` 1, `echo_index_conflict_ambiguous` 1.
`mana_only_windows_skipped` max **161**. Max priority windows in one turn **10** (`deck123-vs-deck126`
turn 14 — the repeat/equip turn). stderr: `with no source tapped` **0**, `all actions pass-declined`
488, `auto-passing without a model call` 1,387, `repeat plan iteration` 44, `dropping stale async
answer` 39 (9 games; only one game reached the 6-streak), `RE-ASK` 0 in stderr (it is prompt-side).

**Mulligans (real hands).** 36 seats kept 7; 6 seats mulliganed: four to 6, `deck152-vs-deck125` to 5,
and **`deck146-vs-deck125` mulliganed FIVE times and kept 2 cards** (seq 1-6 `Mulligan (a keep after this
one would keep N)`, seq 7 `PUT: 4, 2, 6, 1, 7`). That seat then played the 80-turn game. **OWNER
QUESTION** (per the brief's new-baseline rule): a mull-to-2 with a guide floor is the deck seat's to
report; recorded here because it is the 301-record game's opening.

---

## VERDICT TABLE — Set B, by the emitter's string

| Lane | Prediction | Verdict | Count / citation |
|---|---|---|---|
| A 42f2eff2b | edict resolutions with no sacrifice 0/N (was 1/13) | **PASS (0/15)** | 15 `- You cast <Soul Shatter/Tribute to Hunger/Devour Flesh>` blocks, every one followed by a victim line in the same or next `events`: `deck146-vs-deck162` seq 41 `- Opponent used: sacrifice a creature or planeswalker with Soul Shatter targeting Ob Nixilis, the Hate-Twisted`; `deck146-vs-deck125` seq 171 `… targeting Emrakul, the Aeons Torn / - Opponent's Emrakul, the Aeons Torn died` (Soul Shatter does not target — protection irrelevant; correct); `deck123-vs-deck125` seq 66 Tribute to Hunger → `Opponent's Emrakul, the Aeons Torn died`. No Flare of Malice / Riveteers Charm cast arose |
| A | Ob Nixilis-class walkers sacrificed when highest | **PASS (2/2)** | `deck146-vs-deck162` seq 40 row `{right now: they sacrifice Ob Nixilis, the Hate-Twisted (MV 5, their highest)}` → seq 41 victim Ob Nixilis; `deck146-vs-deck126` seq 43 `… Sorin, Lord of Innistrad (MV 4, their highest)` → seq 44 `… targeting Sorin, Lord of Innistrad` |
| B df8ff7641 | `- Paid` spending a multi-colour source while a mono source of an unneeded colour sat untapped: 0/N on plannable costs (was 87/362 lane, 69/386 seat) | **PASS (1/421 plannable) — residual classes as predicted** | Method: for each of the 443 `- Paid <cost> for <card> with <s1>; <s2>…` receipts, the untapped set = the preceding prompt's `Those sources, one per untapped card:` line minus sources spent earlier in the same block; minimum multi-colour count by exhaustive subset + pip matching. Residuals: 14 `{X}`/other costs (`- Paid {3}{r}{r}{x}…`), 8 receipts naming a source not on the sources line (creature / foreach), **0** over-taps (every non-X receipt names exactly pips-many sources — the carried Hive over-tap did not recur: 0/N). Suboptimal **5**, all deck152: 4 are the creature-source class (`deck152-vs-deck126` seq 20 `- Paid {2}{w} for Elite Spellbinder with Elite Spellbinder; Overgrown Farmland #1; Overgrown Farmland #2` while `Intrepid Adversary` (Katilda-granted mana) sat untapped — the planner correctly prefers lands over creatures); the ONE genuine land case: `deck152-vs-deck125` seq 39 `- Paid {g}{w} for Katilda, Dawnhart Prime with Overgrown Farmland #1; Plains #2` with `Lair of the Hydra {g}; Hengegate Pathway {w}` untapped (seq 38 sources line) |
| C 4cf3745a7 | in-range index + off-menu name → `named_row_reask`, never `stale_echo` to Baka (was 3/3) | **PASS (2/2 re-asked, 2/2 recovered)** | `deck123-vs-deck130` seq 25 `CHOICE: 2 (Cast Thraben Doomsayer)` on rows `Cast Intruder Alarm / Cast Tribute to Hunger / Cast Idyllic Tutor / Cast nothing` → `fallback: named_row_reask`, `parse_note: stale_echo_in_range` → seq 26 prompt `[RE-ASK] "Cast Thraben Doomsayer" is not on this list. Answer with a number from 1 to 4.` → `CHOICE: 2 (Cast Tribute to Hunger)` `named_row_reask_recovered`. `deck126-vs-deck146` seq 22 `CHOICE: 1 (Cast Exquisite Blood)` vs row 1 `Cast Wall of Omens` → seq 23 `[RE-ASK] "Cast Exquisite Blood" is not on this list…` → `CHOICE: 1 (Cast Wall of Omens)`. Third re-ask (`deck146-vs-deck126` seq 52, damage-assignment `CHOICE: 3 (Wall of Omens)` over two `Perimeter Captain (0/4) [defender]` rows) is the index-PAST-menu class, also recovered. `stale_echo` fallback **0** |
| C | `x0` on the repeat row = pass (`repeat_count_zero_pass`); `ran 1 time (you named 0)` 0 | **UNTESTED / PASS(0)** | 0 replies named `x0`; repeat row offered **6**, taken **3** (`deck123-vs-deck162` seq 15 x17, `deck123-vs-deck126` seq 27 x23, seq 47 x17), receipts `- Your repeated activation ran 17 of the 17 times you named` / `23 of the 23` / **`7 of the 17 times you named (the cost could no longer be paid)`** (D-1). `ran 1 time (you named 0)` 0 |
| C | repeat takes with a pass-PLAN and no `plan_choice_conflict` record: 0 | **UNTESTED** | `plan_choice_conflict` 0 renders; no take carried a clean `this window: pass` / `stop reached` PLAN (seq 27's PLAN reads `stop at M = 26; M is 3 now; this window: x23 / pass` — not a conflict) |
| C (carry) | genuine echo of the previous window exempt (`mPrevWindowRows`) | **observed, noisy** | `deck146-vs-deck125` seq 160 `CHOICE: 0 (pass)` stamped `stale_echo_in_range;named_row_not_offered` and `deck123-vs-deck146` seq 15 `CHOICE: 0 (pass)` stamped `named_row_not_offered` — a pass carries no name; both executed pass correctly. Note noise, LOW (D-8) |
| D ac7c5f5be | `- Paid` on every mana-cost activation N/N (was 6/9) | **PASS (53/53 observable)** | 55 priority takes whose `chosen_text` has `[cost: {…`; 53 followed by a `- Paid … for <source>` in the next `events`; the 2 others (`deck130-vs-deck125` seq 88, `deck152-vs-deck126` seq 49) are followed directly by `gameend` (no window to carry it). Receipts on `becomes beholder with Hive of the Eye Tyrant` read `- Paid {3}{b} for Hive of the Eye Tyrant with …` with exactly 4 sources |
| D | `with no source tapped` count | **0** | 0 in events, prompts, and stderr — the under-pay never surfaced (or never happened) |
| D | `turn` == last `=== Turn N` in the prompt N/N | **PASS (2,070/2,070)** | every record with a turn header; wave-50 D-7 (`turn` = log − 1) closed. `discard` records now read `turn: 10` under `=== Turn 10` |
| D | edict clause victim == narrated sacrifice N/N | **PASS (6/6)** | 9 `{right now: they sacrifice <name> (MV N, their highest)}` rows, 6 taken (`deck146-vs-deck162` 40 Ob Nixilis; `deck146-vs-deck123` 34 Thraben Doomsayer; `deck146-vs-deck130` 18 Rorix Bladewing; `deck146-vs-deck125` 170 Emrakul, the Aeons Torn (MV 15); `deck146-vs-deck152` 19 Katilda, Dawnhart Prime; `deck146-vs-deck126` 43 Sorin) — 6/6 next-window victim == clause name. Tie-count and 0-board clause forms never rendered (`their highest` 10 renders, `tie` 0): UNTESTED |
| D | receipt sources joined by `; ` (wave-50 D-5) | **PASS** | `- Paid {1}{b} for Devour Flesh with Scrubland; Tundra`; comma-named cards no longer ambiguous |
| E 9c772cd3d | ability-damage takes at a target with toughness/loyalty > N: 0 (was 2) | **FAIL (4/12 takes) — clause rendered on all 4** | 428 rows carry `{right now: takes N damage - DIES}` (209) / `- SURVIVES (toughness N)` (196) / `- SURVIVES (loyalty N, K left)` (9) / `- SURVIVES (toughness N, K more kills it)` (9) / `- DIES (loyalty N)` (5). Takes on a SURVIVES row: `deck130-vs-deck126` seq 37 `Deal 2 damage with Siege-Gang Commander targeting Wall of Omens #3 [opponent's battlefield] {right now: takes 2 damage - SURVIVES (toughness 4)} [cost: {1}{r}, Sacrifice]` (sacrificed a Goblin to ping a 0/4), seq 49 `… Pyrite Spellbomb targeting Overgrown Battlement … SURVIVES (toughness 4)`; the other 2 are TARGET CHOICE sub-menus for Spark Spray where the opponent's face was row 1 (`deck130-vs-deck126` 84, `deck130-vs-deck146` 68). Render correct → deck130 seat (STRATEGY) |
| E | Upkeep animations followed by `ATTACK: none`: 0 (was 2/11) | **PASS (0/0) — by avoidance** | `[Upkeep offer: this animation lasts only until end of turn, and the same row is offered again in your main phase - declining here counts toward this turn's two declines]` rendered **35** (all deck146 Hive rows; it is appended AFTER `{card text:}`, so it lives in `prompt` only — `options_text`/`chosen_text` never carry it). Upkeep animations taken **0**. Main-phase Hive animations taken 9 (`deck146-vs-deck125` 89/91/110/112/150/196/212/260/262); 2 (seq 110, 112, turn 51) were followed by `ATTACK: none` (seq 115) — the row moved, the misplay followed it once (deck146 seat) |
| E | dead Tutor / second-copy casts with `[already owned:` rendered: 0 (was 2+2) | **FAIL (21 takes / 139 rows) — but the tag conflates stackable with dead** | Takes: Idyllic Tutor with `[already owned: Intruder Alarm on your battlefield - this finds only an enchantment card]` ×4 (`deck123-vs-deck125` 48 (D-2's parse defect, not a model take), 55, 105; `deck126-vs-deck125` 21 `… Exquisite Blood …`); **legendary** second copies: `deck152-vs-deck146` 41 `Cast Teferi, Who Slows the Sunset {2}{u}{w} [already owned: Teferi, Who Slows the Sunset on your battlefield]` → seq 43 `- Your Teferi, Who Slows the Sunset was put into your graveyard from the battlefield (that Teferi … was 1 of 2 copies …)`; `deck146-vs-deck126` 20 Lolth likewise (5 mana to bin one) — legend rule enforced by the engine, but the tag never says "the legend rule bins one". The other 15 takes are STACKABLE duplicates (Howling Mine ×4, Talisman of Impulse ×5, Underworld Dreams ×3, Staff of Nin ×2, Elixir ×1) where a second copy is a real play — the tag marks same-name, not dead (D-5) |
| E | Hammer-class returns over a castable body with `{spends K of your M …; <card> in your hand needs K}` rendered: 0 | **PASS (0/141 by class) — 7 takes, none Hammer-class** | 141 rows; takes: Elixir `Life` ×2 (`deck125-vs-deck130` 8 `{spends 2 of your 2 untapped mana sources this turn; Essence Scatter {1}{u} in your hand needs 2}`, `deck125-vs-deck162` 20), Clue draws ×2, Blastminer ×1, Siege-Gang ping ×1 (the seq 37 take above, `Starstorm … needs 2`), Hive ×1. No `Put a card into hand with Hammer of Bogardan` take over a castable body |
| F 7c0a04d1d | every Howling Mine / Master of the Feast / Dictate / Font / Puzzle Box / Forced Fruition cast row carries `{feeds:` | **PASS (78/78)** | shapes: `{feeds: the opponent draws N extra card per turn; converters on your battlefield: K (nothing of yours punishes their draws or discards yet)…}` 32, `… converters on your battlefield: K - <names>}` 28, `{feeds: the opponent draws an amount that is not fixed (read the card); …}` 15 (Puzzle Box / Fruition), `{feeds: the opponent draws N cards per spell they cast; …}` 2 |
| F | deck162 engine casts at `converters … 0` with opp 0 creatures: 0 (was 5/9) | **FAIL (2/42 offered; 3 incl. opp 1 creature)** | `deck162-vs-deck146` seq 4 `Cast Howling Mine {2} … {feeds: … converters on your battlefield: 0 (nothing of your…` opp creatures 0, seq 6 (opp 1), `deck162-vs-deck125` seq 4 (opp 0) — all turn-2 Mines. 42 such rows offered, 3 taken (was 5/9) — rendered correctly; deck162 seat |
| F | bare `// <Name>"}` endings 0 corpus-wide (was 34 lane / 15 seat) | **PASS (0)** | 0 in options_text and 0 in prompts by regex `// [^"{}]+"\}` excluding `(text omitted)`; `(text omitted)` rendered 278 (`{target text: "… // Mistgate Pathway (text omitted)"}` on the wave-50 D-6 land rows) |
| carry | max priority windows per turn ≤ 10 | **PASS (10)** | `deck123-vs-deck126` turn 14 |
| carry | Baka fallbacks ≤ 0.31% | **PASS (0.23%)** | 5/2,129; Baka-executed 2 |
| carry | no prompt > 30,000 | **FAIL (3)** | all `deck146-vs-deck125` (seq 279 33,536 / 237 30,898 / 235 30,533), 71-78% log window at turns 72-78 of an 80-turn game; the largest non-log-driven prompt is `deck123-vs-deck126` seq 44 **29,469** at **23% log** — 35 rows, 26 of them `Equip with Lightning Greaves targeting Vampire #N [your battlefield] (Lightning Greaves is ALREADY attached to Lord of Lineage - this MOVES it to Vampire #N, and Lord of Lineage loses what it grants) [repeat: activated this turn 1 times already. …] {card text: …}` (D-4) |
| Vita fixes | no corpus prediction | **not checked** | |

Set A is not this seat's.

---

## The 301-record game (`deck146-vs-deck125`, 80 turns, deck146 lost −14 to 45)
What kept it going: a **creatureless stalemate**. deck125 (control) wrathed/countered everything and gained
20 → 46 (`Sphinx's Revelation`, `Elixir of Immortality` seq 206); deck146 sat on planeswalkers with 4×
Vanishing Verse + 2× Soul Shatter in hand and NO targets (seq 279 hand line). Per-turn model traffic was
a `Casting decision` + 1-2 `Your legal actions` windows (Kaya/Lolth loyalty rows) + an emblem-driven
`TARGET CHOICE for this effect - its "cast legendary spell"` ask each upkeep (Kaya's −7 emblem recast Kaya
from the graveyard 7 times: `- You cast Kaya the Inexorable from your graveyard` seq 172/181/192/208/220/
232/275), then Kaya `-3: exile non-land permanent … targeting Kaya the Inexorable [your battlefield]` on
itself — 74 such self-exile rows rendered, 1 taken (seq 284) — **no `{this hits YOUR permanent}` on
exile/equip/+1 rows** (D-3). Damage: Emrakul twice (seq 169 `dealt 15 damage to you (now 5)`; the second
Emrakul at seq 291 ended it), Staff of Nin pings, Lolth `0:` self-drain. Turn 76 carried 7 priority
windows (corpus max 10). Latency sum for the seat 4,093 s (~68 min) vs 846 s for the opponent seat.
Opening: the mull-to-2 above. Nothing engine-side held the game open; no loop, no livelock (15 single
stale drops in this game, none consecutive).

## Prompt chars per decision kind — wave 50 → wave 51

| kind | n50 | mean50 | max50 | n51 | mean51 | max51 | p90 latency 50 → 51 (s) | max lat 51 |
|---|---|---|---|---|---|---|---|---|
| ask | 1,214 | 9,102 | 25,018 | 1,493 | 10,418 | 29,355 | 56.2 → 56.8 | 849.5 |
| priority | 376 | 12,038 | 25,793 | 445 | 14,302 | 33,536 | 54.8 → 59.3 | 602.0 |
| attackers | 102 | 11,227 | 25,087 | 110 | 11,324 | 26,911 | 52.4 → 62.9 | 308.7 |
| blockers | 37 | 11,268 | 25,450 | 31 | 10,374 | 20,779 | 67.5 → 79.1 | 171.4 |
| discard | 28 | 9,534 | 25,402 | 31 | 12,506 | 27,450 | 54.4 → 123.0 | 477.6 |
| reveal | 15 | 14,049 | 20,689 | 13 | 14,544 | 19,789 | 42.5 → 43.2 | 59.7 |
| bottom | 8 | 2,200 | 2,542 | 6 | 2,266 | 2,469 | 78.3 → 70.2 | 81.2 |

Means up 2-19% (priority +19%): the 80-turn game alone contributes 291 asks at a 57-82% log share; 102
prompts exceed wave 50's max of 25,793, 96 of them in `deck146-vs-deck125`. Mean log share: ask 57% /
priority 61% / attackers 61% / discard 61% (wave 50: 54/58/62). Lane F's D30 "no change" decision on the
log window is now the whole overrun. Discard p90 latency 54 → 123 s (31 asks; the 477 s max is seq 276-
class 29 KB prompts in the long game).

---

## NEW DEFECTS (record + emitter string)

**D-1 HIGH — `stale_livelock`: the repeat-N loop launches a casting ask per iteration, each drop counts
toward the 6-streak breaker, and Baka gets the decision mid-loop.** `deck123-vs-deck126` seq 47 `CHOICE:
29 (Create vampire with Lord of Lineage x17)` on the row `…, repeated N times, then stop [you control 28
creatures right now; …; N is at most 200]`. stderr `game-126v123-1787864185.stderr` lines 2145-2231: each
`AIPlayerGPT[ph4]: repeat plan iteration k/17 (no model call)` is preceded by `AIPlayerGPT: land-drop ask
NOT issued …` and `AIPlayerGPT: dropping stale async answer` — an async **Casting decision** request
(Devour Flesh castable with 2 open mana) was launched between iterations, the next iteration changed the
board, the answer was dropped, ×6 → line 2231 `AIPlayerGPT: 6 consecutive stale drops - giving this
decision to the heuristic` → `Casting decision (Main phase 1, YOUR turn) … -> chose -1 of 2` → `ai is doing
a combo:Devour Flesh`. JSONL seq 48 `kind: ask, fallback: stale_livelock, latency_ms: -1, reply: "",
choice: -1` (no model call at all). Consequences: seq 49 TARGET CHOICE `CHOICE: 2 (Yourself (player,
life 17))` (the model, asked to target a spell it never cast, targeted itself), seq 50 `CHOICE: 1 (Lord of
Lineage (5/5) …)`, seq 51 events `- Your Lord of Lineage died / - You gained 5 life (now 22) / - Your
repeated activation ran 7 of the 17 times you named (the cost could no longer be paid)`. The breaker
(`kStaleLivelockLimit = 6`, AIPlayerGPT.cpp ~8117) was built for an unstable prompt on an UNCHANGED state;
here the state changes legitimately every iteration. Fix: do not launch (or do not count drops of) model
requests while `repeat plan iteration` is in progress — the row's contract says "then returns priority to
you here". Precondition that wave 50's four ≥17 repeats lacked: a castable spell in hand during the loop.

**D-2 HIGH — `Cast nothing right now` echo is ambiguous against edict rows carrying `at 0 this does
nothing`.** `deck123-vs-deck125` seq 48 reply `CHOICE: 5 (Cast nothing right now)` on rows `1. Cast
Tribute to Hunger {2}{b} {right now: they control 0 creatures - at 0 this does nothing} … 2. Cast Devour
Flesh … 5. Cast Idyllic Tutor {2}{w} [already owned: Intruder Alarm on your battlefield - this finds only
an enchantment card] … 6. Cast nothing right now` → `parse_note: echo_index_conflict_ambiguous`, **executed
5 = Idyllic Tutor** (the dead tutor; `- Your Idyllic Tutor was COUNTERED by Cancel` seq 50). Then seq 49
`CHOICE: 4 (Cast nothing right now)` on a 3-row menu (Tribute / Devour / Cast nothing) → `fallback:
unparsed_reply`, Baka. Mechanism (AIPlayerGPT.cpp ~13620-13660): the echo's significant words
(`nothing`, `right`) are matched against the FULL option text including `{…}` annotations; `{right now:
they control 0 creatures - at 0 this does nothing}` contains both, so every 0-creature edict row matches →
`echoConflict` → index-wins (48) / no remap for an out-of-range index (49). Fix: match the echo against
the row's short name (text before the first ` {` / ` [`), or treat the pass-row label as a reserved echo.
Same-shape risk: any annotation vocabulary that overlaps a row label.

**D-3 MED — `{this hits YOUR permanent}` is on damage/destroy rows only; exile / equip / +1 rows targeting
your own permanent carry nothing.** Rendered: `Deal N damage … [your battlefield] {this hits YOUR
permanent}` 229, `Destroy …` 30; untagged: `Equip with Lightning Greaves targeting <own> [your battlefield]`
118, `-3: exile non-land permanent with Kaya the Inexorable targeting Kaya the Inexorable [your
battlefield] [cost: Counters]` 74, `+1: target creature gets ghostform …` 47 (benign), `-3: emblem …` 9,
`-3: destroy target creature …` 2. Takes on untagged self-exile: `deck146-vs-deck125` seq 284 (Kaya exiled
herself). The equip rows are legitimate self-targets; the exile ones are the sweeper-class the wave-49 fix
was for.

**D-4 MED — per-token row fan-out: 26-34 `Equip with Lightning Greaves targeting Vampire #N` rows, ~600
chars each.** `deck123-vs-deck126` seq 44 (29,469 chars at 23% log, 35 rows), 42, 47, 51 (34 equip rows).
Each row repeats the 200-char `(Lightning Greaves is ALREADY attached to …)` clause, the `[repeat:
activated this turn 1 times already …]` tag and the card text. The target clause already collapses
`Vampire #1-#27 x27`; the option menu should offer one row per identical-token group (or the target as a
sub-ask), as X-menu rows do.

**D-5 MED — `[already owned: <name> on your battlefield]` marks same-name, not dead.** 139 rows, 21 takes;
15 of the takes are stackable duplicates (a second Howling Mine is the plan). The two legendary takes
(`deck152-vs-deck146` 41 Teferi, `deck146-vs-deck126` 20 Lolth) resolved, then the engine asked the seat
to bin one (`- You used: Put a card into the graveyard with Teferi … targeting Teferi, Who Slows the
Sunset #2`) — 4-5 mana for nothing, with no clause saying so. Add `- legendary: the legend rule will put
one into your graveyard` on legendary rows; leave stackables untagged or say `(a second copy stacks)`.

**D-6 LOW — receipts without a payee.** 7 receipts read `- Paid {1}{w} with Overgrown Farmland #2;
Deserted Beach` — no `for <card>` (`deck152-vs-deck123` 35, `deck152-vs-deck125` 36 ×2, 48, `deck152-vs-
deck162` 10, `deck152-vs-deck146` 20; all `{1}{w}`, the Intrepid Adversary ETB pay-any-number-of-times
cost). 3 read `- Paid {3} for Chromatic Lantern from mana already floating` (fine).

**D-7 LOW — duplicate identical rows without `#N` on damage-assignment / opponent-hand asks.** 28 ask
menus: `deck146-vs-deck126` seq 52 two rows `Perimeter Captain (0/4) [defender]` (the model's `CHOICE: 3
(Wall of Omens)` re-ask above), `deck152-vs-deck126` 25-28 `Overgrown Battlement (0/4) [defender]` ×2,
`deck152-vs-deck126` 10 `Idyllic Tutor [sorcery] [opponent's hand]` ×2, `deck146-vs-deck125` 61 `Plains
[land] [your library] - "W"` ×4. Cosmetic, but a name-echo cannot disambiguate them.

**D-8 LOW — pass replies stamped with name notes.** `deck146-vs-deck125` seq 160 `CHOICE: 0 (pass)` →
`stale_echo_in_range;named_row_not_offered`; `deck123-vs-deck146` seq 15 `CHOICE: 0 (pass)` →
`named_row_not_offered`. Executed pass; the notes are noise for the corpus counts.

**Observed, not defects:** `answer_replaced` 18 — `deck130-vs-deck125` 25 (`CHOICE: 3 (Dwarven
Blastminer)` → `CHOICE: 1 (The opponent …)` ×2, exec 1), 85; `deck162-vs-deck146` 17 (discard `PUT: 3`
then a stray `CHOICE: 2` line; exec 1 = Forced Fruition); `deck130-vs-deck123` 36, 42; `deck146-vs-
deck125` 82, 136, 217, 260; `deck130-vs-deck162` 33; `deck126-vs-deck162` 22; `deck123-vs-deck125` 46,
50, 52, 54, 104 (Tutor → Cast nothing, exec 6 each); `deck146-vs-deck152` 39, 40 (`ATTACK: A1` →
`ATTACK: none.` ×2, exec none) — all last-clean-line reads. `echo_index_conflict` `deck146-vs-deck125`
295 `CHOICE: 3 (Plains #3 - "W")` on rows `1. Plains #2 / 2. Plains #3 / 3. Plains #4` → executed 2
(name wins) — correct. `attack_last_line_taken` 2 — reasoning bodies, correct reads. Legend rule: enforced
(above).

---

## Lategame specimen candidate (invariant 0)
`1787864173-ai_baka_deck146-0x55930e561360-vs-ai_baka_deck125.jsonl` seq **279** (33,536-char priority
window at turn 78 of 80: 23.7 KB log, 11 loyalty rows incl. three self-exile Kaya rows, the stack with
opponent's Path to Exile over the seat's Silverquill Command, hand of 4 Vanishing Verse + 2 Soul Shatter
with no targets) paired with seq **235-240** (30.5-30.9 KB, 77-82% log). Alternative for the repeat
machinery + livelock: `1787864189-ai_baka_deck123-0x5593ba17b190-vs-ai_baka_deck126.jsonl` seq **44-51**
(the 35-row equip fan-out, the x17 take, the `stale_livelock` record, the self-Devour).

## WAVE-52 DOCKET PROPOSAL (engine / UX only), ranked
1. **D-1** repeat-loop livelock: suppress model requests (or exempt their drops from the streak) while
   `repeat plan iteration` runs; the loop's own contract returns priority after N. Repro: game 126v123
   stderr 2145-2231; JSONL seq 47-51. Cost: a 5/5 lord, 10 tokens, and a Baka-cast self-edict.
2. **D-2** echo matcher: match names against the row's short label, not the annotated text; `nothing`/
   `right` from `at 0 this does nothing` currently make the pass row ambiguous (1 dead cast + 1 Baka).
3. **D-4** collapse per-token equip/target rows (`Vampire #1-#27 x27` style) — the only non-log prompt
   near 30 KB, and 34 near-identical rows for one decision.
4. **D-3** `{this hits YOUR permanent}` on exile / destroy-class loyalty rows (74 untagged Kaya self-exile
   rows, 1 taken).
5. **D-5** `[already owned:` → legend-rule clause on legendaries; drop or soften on stackables.
6. **D-6** payee on the Adversary-class receipt (`- Paid {1}{w} for <ability> with …`).
7. **D-7** `#N` on duplicate rows in assignment / hand-reveal asks.
8. **D-8** don't stamp name notes on `CHOICE: 0 (pass)`.
9. Log window: 102 prompts over wave 50's max, 3 over 30 KB, all one 80-turn game at 71-82% log — the D30
   "no change" is now the sole cause of the bar breach; a turn-bounded or size-bounded log window is the
   lever (decision still the owner's).
10. Watch: lane-E SURVIVES takes (4, deck130), Upkeep-row moved but main-phase animation then `ATTACK:
    none` 2 (deck146), deck162 turn-2 Mines 2 — all rendered correctly; deck seats.

## Falsifiable predictions for wave 52 (if the docket lands in order)
- `stale_livelock` 0/N; every repeat take runs `N of the N times you named` unless the cost fails for a
  board reason narrated in the same block.
- `echo_index_conflict_ambiguous` / `unparsed_reply` on a reply whose parenthetical equals a row's short
  label: 0/N.
- No priority window with > 12 rows of the same verb+source over `#N` targets; no non-log-driven prompt
  > 20 KB.
- Self-exile / self-destroy loyalty rows carry `{this hits YOUR permanent}` N/N; takes 0.
- Legendary second-copy casts with the legend clause rendered: 0/N.
- 21/21 natural; Baka-executed fallbacks ≤ 0.31%; max priority windows/turn ≤ 10; `turn` == narration
  turn N/N; `with no source tapped` 0.

## What I did NOT check
- Any Set A guide prediction; the mull-to-2 is reported, not adjudicated.
- The Vita/human-seat oracle fixes (not LLM-visible).
- D-1's fix site beyond reading `pollCompletion` (~8095-8125) and the stderr trace — did not run wagic.
- Lane B on the 8 creature/foreach receipts (classified, not solved) and on `{X}` costs (14).
- Whether the 15 stale drops in the 80-turn game were the same cast-ask-during-upkeep shape (single
  drops, never consecutive; not traced).
- Card facts: no primitive or Scryfall lookups were needed for this seat's verdicts (Soul Shatter's
  no-target clause vs Emrakul's protection is CR, not card text).
