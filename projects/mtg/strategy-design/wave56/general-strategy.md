# Wave-56 general strategy — RENDER / CORE-PROMPT proposals, layer-routed (R1-R258 revised, R259-R292 new)

Corpus `matchups-20260903-120952` (THIRTEENTH fair-hand corpus), binary master **d0515fae3**
(wave-56 step-1 lanes A `7a2e243c7` / B `5c391be1e` / C `7193e4bb5` / D `e60a7c15d` / E `039af56c6`
on base `41d1c15ef`; merged 6aa8341d9 / cc7acb04e / c7f76e299 / 256ab5eb4 / d0515fae3. Gate: suite
1234 with the 2 known concurrency-only failures, 51 AI/0, PARSETEST 2736/0). Pilot
**qwen36-35b-a3b** on Spark :8084, `-j 21`, `WAGIC_GPT_TIMEOUT=900`.

42 seat logs, **2,854 records** = 42 `gamestart` + **42 `gameend`** + 6 `recovery` + 2 `wall_miss` +
**2,762 decisions** (ask 2,127 / priority 456 / attackers 88 / discard 55 / blockers 25 / reveal 9 /
bottom 2). **21/21 GAMES NATURAL — invariant 00 SATISFIED**: 0 timeouts, 0 crashes, 0 hangs,
0 adjudications, **0 `Segmentation`**. **6 decision `fallback` fields = 0.22%** (`named_row_reask` 3,
`stale_echo` 1, `wall_miss_unrecorded` 2 — **no `plan_choice_conflict`, no `all_assignments_illegal`,
no `stale_livelock`, no `unparsed_reply`, no `empty_reply`, no `timeout`**). **Baka executed 0
decisions**; `giving this decision to the heuristic` **0** in all 42 stderr. `choice: -1` **6**, and
**6 `recovery` records, 0 unpaired**. Max prompt **26,298** (`125v130` seq 113); **prompts > 30 K: 0**
(was 1); mean **11,185** (was 12,174, −8.1%). Total inference **21.96 h over all positive latencies /
21.46 h over decisions only** = **27.97 s per decision** (two units, both stated, #208).

Sources: the four seat files, seven `deckN/general-suggestions.md`, seven `deckN/skill.md`, seven
`deckN/edit-texts.md`, `wave56/lane-{A,B,C,D,E,Y,Z,X}.md`, `wave56/review-carry.md`, and the corpus —
**every load-bearing number below was re-derived on disk by this step** with python over the JSONL,
**counting rows from the rendered `prompt`** (#240) and PREFIX-matching HOLD takes (#294's sibling
rule from the wave-55 boundary pass), plus `/usr/bin/grep` over the 21 `.stderr`.

**Routing rule (owner doctrine 2026-08-26).** The core loop — engine mechanics, LLM UX, bugs — is the
purpose. Everything below is a change to what the ENGINE RENDERS, what the PARSER executes, or the
CORE PROMPT's own text. Engine BUGS with call sites are in `engine-ledger.md` (the wave-57 docket);
this file is the ranked routing view with the evidence and the wave-57 falsifier for each item.

**Governing constraint.** Legality is enforced structurally; the model's CHOICE is never constrained.
Every proposal ADDS a true token, a true row, a receipt, a stamp or one re-ask, and DELETES nothing
the model can use — **with one deliberate exception this wave, R272**, which deletes a row the model
took 0 times in 109 renders and which exists only for a human display. **Hard caps on legal choices
are rejected on sight**; three are rejected below. Standing owner rulings honoured throughout:
X menus largest-first (**9 of 9 this corpus**), history lines owner-prefixed, declines LAST.

**THE INSTRUMENT NOTICE.** Wave 55's window economy was uncontaminated and wave 56's is comparable:
`giving this decision to the heuristic` 0, stall-floor releases 0, `interrupt window held` 0. The
denominator moved for a different reason — **2,762 decisions against wave 55's 3,171 (−12.9%)**,
because games were shorter (max seat 61 turns) and because `hold_windows_skipped` rose **735 → 1,236**
and `mana_only_windows_skipped` stands at **1,482**. Rates are comparable; absolute counts are not.
The one metric that is NOT an engine number is per-decision inference: **R257, now confirmed twice.**

---

## STATUS OF R1-R258, by the emitter's actual string

Verdicts carry from `wave55/general-strategy.md` except where a wave-56 lane, or this corpus, touched
them. The table below covers every item a wave-56 lane addressed plus every item a seat re-filed.

| item | wave-56 verdict | evidence (re-derived on disk) |
|---|---|---|
| **R225** the HOLD latch's re-open predicate | **CLOSED — PASS, and it is the wave's cleanest lane result** | Lane A re-keyed `holdStillStands` to the rendered rows. Byte-identical same-turn re-asks after a HOLD take: **14 of 320 (4.4%)**, was 113 of 282 (40.1%). `hold re-opened` fires 144 times in stderr and **every one names a row**: 116 `at the cast seam - a printed row changed or is newly available`, 28 at the priority seam. **0 re-opens on a life-only or stack-only change.** Inference inside identical-declined runs **1.18 h = 5.5%** (was 12.4%), target < 6% **PASS** |
| **R225(b)** the run-length half | **SPLIT — share PASS, max run FAIL (#300)** | share **14.99%** on the wave-53 predicate (target < 15%), **16.8%** on rendered rows at runs ≥ 3, **22.3%** at runs ≥ 2. **Max run 13 under every predicate tried** (target < 10) — **FAIL**. Split by side: own-turn 2.4% / max 9; opponent-turn **44.1% / max 13** (wave 55: 57.8%). The residual is entirely an opponent-turn phenomenon → **R270**, **R289** |
| **R226** the ability-row life falsehood | **CLOSED — PASS 0 of 145** | Lane B found the root cause (`AIAction`'s player-ability ctor never wrote `player`; `->life` at the garbage offset read the constant 166). `they would be at K` renders **145 rows** and `K == opp_life − damage` on **145/145**. `at 164` / `at 165` render **0**. And the fix produced a surface that could not exist before: **4 rows read `they would be at 0; THIS WINS THE GAME`, the model took the row in 3 of 3 games and won each** (`125v123` s166, `126v130` s95, `125v130` s115) |
| **R227** the `[RE-ASK]` wrong-line quote | **UNTESTED (N = 0), second wave for the arm** | 3 `[RE-ASK]` notices, **all three the OTHER form** (`"Cast Thraben Doomsayer" is not on this list. Answer with a number from 1 to 5.`). The CHOICE-line shape D3 rewrote **never rendered**. `plan_choice_conflict_recovered` **0**. 0/0 mismatches is vacuous; the fix is unverified → ledger D9 |
| **R228** the bare discard rows | **FAIL — 357 of 426 rows (83.8%) carry no verdict; RE-FILED HIGH** | Tag family counted explicitly (#298): `{dead right now:` 37 rows, `{spare:` 32, everything else nothing. Per deck: deck125 220 of 272 bare, deck123 100 of 115, deck146 28 of 29. **My own first predicate (`row contains a '{'`) read 0 bare and would have discharged this item** — see #298 → **R266** |
| **R229** the missing incoming-combat total | **HALF SHIPPED, HALF OPEN** | Lane B's `INCOMING THIS COMBAT` is on **25 of 25 `blockers`-kind windows** and its arithmetic checks 25/25 against `my_life`, with `; this KILLS you` matching `K <= 0`. But it is emitted from the **blockers ask only**: of 260 combat-phase windows corpus-wide, **25 carry it (9.6%)**, and a creature-less seat gets none — **deck125: 0 of 20** → **R264** |
| **R230** the empty `chosen_text` | **SPLIT — PASS on emptiness, FAIL on fidelity** | Records with `choice >= 0` and empty `chosen_text`: **0 of 2,758** (was 9). But **112 of 320 HOLD takes** record `chosen_text` truncated at `…you give up no cast)`, dropping the `{taking this row skips…}` tail present on **2,016 / 2,016** rendered rows; **140 of 2,758 takes overall are tail-less**. Exact-matching reads 208 HOLD takes where the truth is 320 — a **35% undercount**, and it is why the orchestrator's D8 harvest was wrong → **R262** |
| **R231** the monotone X family | **SPLIT — marker PASS 9/9, fit clause FAIL 0/40** | 9 X menus, 40 `X = n` rows. Every menu carries exactly one `[<- …]` marker (`largest affordable X` 3 renders, `most kills` 6) and the header `Every listed value is affordable; option 1 is the LARGEST X` on 9/9, largest-first 9/9. **Not one of the 40 rows carries `{leaves N sources}` or `{taps you out}`**, while 3,498 `{leaves` occurrences render on cast rows corpus-wide. Marked row taken 7 of 9 → **R267** |
| **R232** MDFC land backs | **SHIPPED — PASS on every arm lane D predicted, and it opened a new legality defect** | **65 `PLAY THIS AS A LAND` rows in 62 records, 6 takes, 6 arrivals**, all at deck146/deck152 seats. `Flip Side` takes **0 of 109** (was 11). 0 turns with two land drops via this route. The three retired claims (`only the currently-shown face`, `gains you nothing playable`, `Use the face that is showing`) render **0**. Residuals: **R259** (legality), **R260** (pregame count), **R261** (contradicting summary line), **R272** (the toggle row) |
| **R233** the edict tie clause's quantifier | **PASS (1 of 1) — thin** | `they sacrifice ONE of these 2, their choice - all tied at MV 3 (their highest)` renders once (`146v152` s30) and was taken; the reply's PLAN reads "either … or …" — singular, which is the belief the clause was written to produce |
| **R234** the opponent's open mana | **CLOSED — PASS** | `Their untapped sources: N (colours they could make: …)` on **2,716 of 2,716** prompts carrying a `CURRENT SITUATION` block (98.3% of all 2,762 decisions; the 46 misses are the 44 pregame asks + 2 `bottom`, which are HAND-ONLY by owner directive and must NOT carry a board line). Tapped-out form `Their untapped sources: 0` renders 576. deck152's wave-55 disaster matchup inverted on this line alone: a 65-turn loss became a **T15 win, every creature cast at a printed count of 0-3** |
| **R235** plan staleness | **UNTESTED (no window) — and the mechanism is unverified** | D11's withdrawal sentence `is withdrawn (you stated it,` renders **0 / 0 / —**. Plan echo max **20** (target < 60), p90 3, over-40 served plans 0 — but there were **0 records with a plan older than 40** and **0 where the menu-diff note (`"<card>" is no longer on your menu`, 473 renders) fired on a plan more than 4 windows old**. The improvement is shorter games, not D11 → **R282** |
| **R236** `reveal_stall` | **CLOSED — PASS 0 of 9, wait fields 9 of 9** | `reveal_stall` is gone from the field set; `reveal_wait_ticks` + `reveal_wait_secs` on 9/9 reveals, `_secs` within 1 s of `latency_ms/1000` on 9/9, ticks uncorrelated with seconds (the structural signature working) |
| **R237** the residual seam alternation | **FAIL — 65 drops (was 43; target < 20), and the class is now named** | See **R263**. The D18 rescue **does fire** — `consuming an in-flight answer whose prompt text drifted (same seam, turn, phase and board)` × **8**, in 8 distinct games. All 65 drops are the turn-flip alternation: **44 followed immediately by a `Land drop:` ask, 21 by a `Casting decision`**; 32 of 65 are preceded by `AIPlayerBaka: Mana cost is NULL.` (the Baka opponent tapping a land), 10 by a resolving `10DrawAction`, 5 by a `NextGamePhase`. **0 are phase-only on an unchanged board** — D18's second prediction PASSES exactly while its headline fails |
| **R238** `{feeds:`'s release | **SHIPPED guide-side; render half OPEN** | deck162's brake now names its release and its near-miss. The missing render half is the in-hand converter count → **R280** |
| **R239** `{MV n}` on discard rows | **NOT SHIPPED — 0 renders** | 426 discard rows, **0 carry `{MV`**. Subsumed by **R266**, which asks for the whole verdict family rather than one tag |
| **R240** the `[doesn't untap]` consequence | **PASS by carve-out, not by render** | deck146's P1 fired 2 of 2 and **both flagged plays were correct** (a vigilance attacker into `They have 0 untapped creatures able to block`). The rule over-braked; the edit is a carve-out (#288). No render change asked |
| **R241** the plan-vs-row-clause note | **PASS on the stamp, INSTRUMENT DEFECT beside it** | `plan_choice_conflict` **0** (was 9); `plan_choice_conflict_narrowed` **14**, and 6 sampled replies show the prose naming only the row the CHOICE took — 0 genuine rivals suppressed. **But the promised BROAD census `decision_reversed_in_prose` renders 0** (wave 55: 14): the narrowed stamp replaced it rather than sitting beside it → **R281** |
| **R242** the nested blocking-trigger clause | **FAIL — 10 of 36 B-lines** | The new un-nested clauses ship and are correct (`{blocking trigger, this combat: you may gain 2}` 22 occurrences / 6 records; `{after this combat: you control N fewer blocker…}` 26 / 18). 9 of the 10 residuals are the ATTACKER's lifelink; **1 is squarely the predicted class** (`126v123` s37, an OWN gain nested with no un-nested tag) → **R268** |
| **R243** the menu-level dead verdict | **NOT SHIPPED — re-filed by two seats independently** | deck123: **85 of its 197 casting menus had no live cast row**; the pilot read them correctly 84 times and failed once. deck126: 296 rows `{right now: they control 0 creatures - at 0 this does nothing}`, 27 `[second copy:`, 17 dead-Tutor brackets — **each row says it is dead; nothing says the MENU is** → **R270** |
| **R244** the `plan_choice_conflict` predicate | **CLOSED — PASS 0** | 0 firings corpus-wide; the field is absent from the record set entirely |
| **R245** what-changed on a repeat | **UNTESTED** | no lane shipped it; the declined-note count is the only repeat signal and it renders on 877 prompts (max N = **19**) |
| **R246** the unpriced equipment row | **CLOSED — the render told the truth** | `{right now: you control 0 creatures - this equips nothing}` on **62 rows / 62 records**; **1 take** (`123v125` s45, at 7 untapped sources). Rate 33% → 1.6%. This is now a deck123 guide item, not a render item |
| **R247** the duplicate cast row | **UNTESTED (N = 0), and a SIBLING is open** | `{same effect as row N, for K more mana}` renders **0 / 0 / 0** and the window never arose — 0 menus carry two `Cast <same card>` rows with an identical verdict at different cost. The sibling that DID arise is two DIFFERENT cards with byte-identical verdicts (`123v125` s45, Devour Flesh {1}{b} and Tribute to Hunger {2}{b}) → **R288** |
| **R248** the marker as a ranking | **CLOSED — PASS, and it is the most followed annotation in the render** | 9 of 9 menus marked, marked row taken **7 of 9**; at the deck130 seats **6 of 6** (wave 55: 2 of 3; wave 54: 0 renders). Extension asked → **R277** |
| **R249** the declined count inside the HOLD row | **NOT SHIPPED** | the declined note remains its own line above the menu (877 renders). `optionSetKeyOf` still strips it, so the key collision lane A named in writing is unresolved; the ask stands unchanged |
| **R250** the `(xN)` log collapse | **OPEN — 1,767 adjacent duplicate event lines in 670 records** | dominated by `- Opponent drew a card` ×1,348 (Howling Mine games — two draws in one turn is TRUE and is never collapsed). Wave-55's `- Phase: Draw` duplicate class is **GONE (0)** → **R285** |
| **R253** resource 404s | **OPEN — 1,774 in the 21 stderr** | unchanged in character; no correctness effect observed |
| **R254** the DRAW FORECAST product | **PASS/KEEP** | deck162's `DRAW FORECAST` + `DRAW PUNISHERS` pair is intact and doing work; **109 renders of the punishers line at deck130's seats, 0 draws or cycles taken under one — sixth corpus** |
| **R255** the `WAGIC_GPT_BOARDINDEX=0` segfault | **CLOSED — PASS** | Lane C reframed it: never causal; the SEGV was a dangling `MTGAbility::target`, fixed at 5 deref sites + `abilityCanReactTo`. **0 `Segmentation` / `ASAN` / `assert` in all 42 stderr, 21/21 natural**, and the flag is usable for the first time. Ownership residual carried (ledger D25) |
| **R256** `cache_hits` on `gameend` | **OPEN — fourth wave** | `cache_hits` present on **0 of 42** gameends. Lane M's counters still reach no log |
| **R257** per-decision inference reported with its concurrency, never carried as a target | **CONFIRMED A SECOND TIME — and it retires the prompt-trim latency lever** | mean prompt **fell 8.1%** (12,174 → 11,185) while per-decision inference **rose 7%** (26.1 → 27.97 s) at the same `-j 21`. The rival hypothesis (prompt size drives latency) predicts the opposite sign. A prompt-byte trim may not be justified by a latency argument at this concurrency; byte trims are context-headroom items only |
| **R258** the log window | **OWNER, standing** | unchanged |

**Everything not listed carries its wave-55 verdict unchanged.** No wave-56 lane touched R1-R192,
R199-R224, R251 or R252, and no seat re-filed any of them.

---

## NEW — ENGINE, HIGH

### R259. The MDFC land-back row is offered where a land play is not legal, and one such play resolved on the opponent's turn.
**44 of the 65 `PLAY THIS AS A LAND` rows are rendered at a window where the play is not
sorcery-speed-legal**: 17 at the seat's own **Upkeep**, 15 at **Blockers**, 1 at **Draw**, 8 with
`Land drop: ALREADY USED this turn` on the same prompt; only **21** are on the seat's own main phase
with a drop available. **`152v162` seq 3 is the take**: `phase: Upkeep`, `It is the opponent's turn.`,
`Mana available: 0`; the seat answered `CHOICE: 1 (boulderloft pathway with Branchloft Pathway)` and
the next prompt's log reads `- You played Boulderloft Pathway` under `=== Turn 2 - opponent's turn ===`,
then `- You played Plains` on turn 3. **CR 305.1** — two land drops in consecutive turn halves.
**Mechanism, source-verified by the deck146/152/162 seat and not re-derived here:**
`restriction{can play land}` is a **no-op string** — `AbilityFactory::parseRestriction`
(`src/MTGAbility.cpp:1069-1099`) recognises only `myturnonly` / `opponentturnonly` / `assorcery` /
`<my|opponent><phase>only` and returns `NO_RESTRICTION` otherwise, so `parseCastRestrictions`
(`:106-136`) gates nothing; the `compare(isflipped)~equalto~1` clause lane D removed had been the
only thing keeping the row off illegal windows (#296). **Layer: CARD SCRIPT + engine.** Ask:
`restriction{assorcery}` on the 55 `autohand={0}` entries in `borderline.txt`, **plus** a separate
land-drop-remaining check (`assorcery` does not cover the 8 ALREADY-USED rows), **plus** a fixture
that asserts the ROW IS ABSENT on the opponent's turn — lane D's existing negative fixture asserts
only that a CLICK finds nothing, which is why it passes today.
**⚠ Lane W (MDFC land-drop legality gate) is IN FLIGHT on master and is not in this corpus** — if it
lands, this item is discharged by it and wave 57 re-measures rather than re-files.
**Wave-57 falsifier:** `PLAY THIS AS A LAND` rows at a non-main phase, on the opponent's turn, or
under `Land drop: ALREADY USED` — **0 of N** (this corpus 44 of 65).

### R260. The pregame hand header does not count MDFC back-face lands, and it decided 2 of 2 mulligans and the corpus's one bottom.
`Your hand (7 cards), counted by the engine: N lands, M spells` counts printed card types.
**`146v152` seq 1** reads `counted by the engine: 0 lands, 7 spells` and
`Playing every land in this hand would not cover any spell in it` on a seven holding **both**
`Agadeem's Awakening` and `Emeria's Call` — a **two-land seven** in this binary — and the seat
**mulliganed**. **`146v130` seq 1** reads `1 land, 6 spells` with `Pelakka Predation` in hand: also a
two-land seven, also **mulliganed**; seq 2/3 then read `1 land` again with `Agadeem's Awakening` and
the seat **bottomed Agadeem's Awakening — its own second land**. Both games lost.
**This corpus produced exactly 2 mulligans and 1 bottom, and all three are this defect.**
The same undercount feeds `Mana sources among those lands` and the coverage clause.
**Layer: render (pregame serializer).** Ask: count a hand card as a land when it carries a
`forcetype(land)` hand entry, and print the split —
`counted by the engine: 2 lands (1 of them a land on the back of a spell: Agadeem's Awakening), 5 spells`.
Stays HAND-ONLY; no board frame. **Ranked as correctness, not render polish, per skill #297**: the
pregame prompt has no later window to correct it. **Possibly absorbed by lane W — mark and re-measure.**
**Wave-57 falsifier:** hands containing a `forcetype(land)` card whose header land count omits it —
**0 of N** (this corpus 4 of 4 pregame prompts at deck146 seats).

### R261. The `Land drop:` summary line contradicts a land row on the same prompt.
`Land drop: you have no land you could play right now (none playable from your hand or any other
zone).` renders on **1,109 windows**, and **18 of the 65 back-face land rows sit on a prompt carrying
it**. `146v125` seq 13 is the decisive one: the line says no land is playable, option 1 plays a land,
**the seat took it and the land entered**. Under the trust doctrine the model believes the surface it
reads first, and here that is the summary. `LegalActionsOracle::legalLandPlays` is right to ignore an
Emeria's Call (it is a Sorcery; the back face arrives via `AAFlip`) — **the sentence it feeds is what
is now false.** **Layer: render.** Ask: derive the summary line from the same union that produces the
option rows. Two rendered statements about one question must not disagree.
**Possibly absorbed by lane W — mark and re-measure.**
**Wave-57 falsifier:** prompts carrying both `Land drop: you have no land you could play right now`
and a numbered land-play row — **0 of N** (this corpus 18 rows / 17 windows).

### R262. `chosen_text` fidelity: 112 of 320 HOLD takes are tail-less, and no `choice: -1` record carries a refusal value.
Two faces of one field. **(a)** The `{taking this row skips the rest of this turn's identical
windows}` tail is on **2,016 of 2,016** rendered HOLD rows and missing from **112 of 320** takes
(35%); corpus-wide **140 of 2,758** takes record a `chosen_text` that is a strict prefix of the
rendered row. Exact-matching reads **208** HOLD takes where the truth is 320 — **and this is why the
orchestrator's harvest read D8 as "0 takes, 0 arrivals" when the disk says 6 and 6.** A field that
silently truncates has now produced a wrong docket number in two consecutive waves.
**(b)** All **6** `choice: -1` records (3 `named_row_reask`, 1 `stale_echo`, 2 `wall_miss_unrecorded`)
carry **no `chosen_text` at all**, and the promised `<refused: …>` value renders **0 times** — so a
harvest still cannot distinguish a refusal from a missing field. **Layer: engine (translog writer).**
Ask: write `chosen_text` byte-equal to the rendered row, and write `<refused: …>` on every `-1`.
**Wave-57 falsifier:** takes whose `chosen_text` is not byte-equal to the rendered row — **0 of N**
(this corpus 140 of 2,758); `choice: -1` records with no `<refused:` value — **0 of N** (6 of 6).

### R263. The async slot is shared by the land-drop and casting arms: 65 drops, 0.51 h, and the rescue is not where the volume is.
`dropping stale async answer` **65** (wave 55: 43; lane A's target < 20) — **FAIL**. The new rescue
line **fires 8 times** in 8 distinct games, so the mechanism works. Classified on disk: **44 of 65
drops are followed immediately by a `Land drop:` ask** and 21 by a `Casting decision`; **32 of 65 are
preceded by `AIPlayerBaka: Mana cost is NULL.`** — the Baka opponent auto-tapping a land, a real board
mutation that legitimately misses the slot key while the in-flight answer was for a menu the land
drop does not touch. **0 drops are phase-only on an unchanged board.** The rise 43 → 65 tracks the
window mix: HOLD takes rose 282 → 320, and a hold taken at the end of a turn is exactly what leaves an
in-flight answer for the next turn's land-drop ask to displace. Cost **65 × 27.97 s ≈ 0.51 h (2.4%)**.
Repro: `game-125v123-1788455392.stderr:932`, `game-123v130-1788455402.stderr:475`.
**Layer: engine.** Ask (cheap, and it is a **second slot**, not a key change): give the land-drop ask
its own async slot so the two arms do not share one.
**Wave-57 falsifier:** stale drops **< 20**, and **0** drops whose next ask is a `Land drop:`
(this corpus 44 of 65).

---

## NEW — RENDER, HIGH

### R264. `INCOMING THIS COMBAT` is emitted on the `blockers` ask only, so a creature-less deck never sees the number that ends its games.
The line shipped and it is correct: `126v146` seq 41 renders `INCOMING THIS COMBAT: 9 attackers,
26 unblocked damage - you would be at -18; this KILLS you (your creatures may legally block every
attacker in that total)`, on **25 of 25** `blockers`-kind windows, arithmetic 25/25.
**Corpus-wide it reaches 25 of 260 combat-phase windows (9.6%)**, and the gap is structural: the
engine generates no `blockers` ask for a seat with no creature. **deck125 took 20 combat-phase
windows across six games and not one carries the line** (its own seat counts 68 opponent-combat
windows on a wider phase predicate — `Combat begins` 28, `Attackers` 20, `Combat ends` 20 — and 0
carry it either way). Its opponent's attackers, their `[attacking]` tags and their (P/T) are all
printed; **the SUM never is.** Per deck, combat-phase windows without the line: deck146 66/67,
deck152 53/57, deck126 30/39, deck130 21/26, **deck125 20/20**, deck162 8/12, deck123 6/14.
**Layer: render.** Ask: emit the same device on **any** `ask`/`priority` window during the opponent's
combat whenever the opponent's header shows declared attackers, gated on THEIR attackers rather than
on MY blockers. It is one line, and it is the `[DRAW PRICE:` pattern pointed at the number that ends
games. **Wave-57 falsifier:** opponent-turn combat-phase windows with a declared attacker and no
incoming total — **0 of N** (this corpus 204 of 260 by the all-kinds predicate).

### R265. The Emrakul row is the only cast row in the pool that can hand the opponent 15 life, and it is the only one that does not price itself.
`125v126` seq 254 (t55, 37 life against 2): row 3 reads `Cast Emrakul, the Aeons Torn {15} (15/15)
{leaves 3 of your 18 untapped mana sources untapped} {card text: "…"}` — a mana clause and card text,
nothing else. **On the same menu** row 1 prints `{removes: …}` with eleven enumerated legal targets
and row 4 prints `{X pricing: max affordable X=15 (18 mana total)}`; the prompt carries
`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond #1, Sanguine Bond #2` **forty
lines above the menu**, and the opponent's line lists four Tribute-capable bodies. The seat took row 3,
they Tributed it, and **37 became −8 in one turn.** Fourth corpus in which this row has been taken
under a printed converter line; three games lost to it, and the guide rule has grown every wave
since 51 (skill #273). **Layer: render.** Ask: when a converter of THEIRS is on the battlefield, a
creature cast row whose creature's toughness is T carries
`{their converter: if this is sacrificed or destroyed they gain up to T and you lose that much - you
would be at K}` — the shape the engine already computes twice on that same menu.
**Wave-57 falsifier:** creature cast rows rendered under a printed opponent converter line with no
converter price — **0 of N** (this corpus: every one).

### R266. 84% of discard rows carry no verdict, and the cards the decks actually hold get nothing.
**357 of 426 discard rows (83.8%) carry no dead/spare verdict** — the whole family is `{dead right
now:` (37 rows) and `{spare:` (32). Per deck: **deck125 220 of 272**, deck123 100 of 115, deck146 28
of 29. Bare on rows the engine **already evaluates for the CAST menu on the same turn**: a Supreme
Verdict or Final Judgment while the opponent header's second number reads 0 (`125v126` seqs 98, 111,
148, 163 — four cleanups in one game, every one sending a sweeper the engine could have marked dead),
a Lightmine Field at the same 0 (`125v126` s197, `125v162` s92), an Essence Scatter against a wall
deck (`125v126` s120). And the asymmetry is what blocks a guide rule: `123v130` s22 is a 14-row
cleanup on a creature-less board where the `Tragic Slip` rows carry `{dead right now: 0 legal targets
on the board for it}` while `Tribute to Hunger`, `Devour Flesh` and `Damnation` carry nothing —
**an edict at N=0 and a Damnation at `destroys 0` are exactly as dead.** Until the predicate is one
family, no guide can teach "the unannotated row is the keep".
**Layer: render.** Ask: run the cast-row `{right now:}` evaluator over the discard menu.
Ranked HIGH for the second wave running because the LAND half of this shipped and took deck125's send
order from 3-of-4 wrong to **0 breaks in two corpora**; the sweeper half is the same size.
**Wave-57 falsifier:** discard rows whose card has a computable dead-verdict and carries none —
**0 of N** (this corpus 357 of 426 bare).

---

## NEW — MED

**R267. D7(a)'s mana-fit clause is unreachable at ANNOUNCE_X: 0 of 40 X rows.** `125v130` seq 72
renders `X = 6 {X pricing: X=6 - you gain 6 life and draw 6 cards} [<- largest affordable X …]` with
no `{leaves N sources}` anywhere on the menu, while 3,498 `{leaves` occurrences render on cast rows.
Lane C shipped the per-row `selectAutoTapProducers` call and flagged in writing that it had never been
observed rendering; **this corpus confirms the path is dead.** The marker landed, so the pilot sees
WHICH X is largest but not what any X leaves up — and it answered X = 3 on 2 of the 3 menus where more
was affordable (both deck125, where the guide mandates X = 3 verbatim; per skill #261 the anchor is the
guide, not the render). *Falsifier: X rows with no fit clause 0 of N (40/40) — and if still 0 renders,
the item is a wiring bug, not a builder bug.*

**R268. D13 residual: 10 of 36 blockers B-lines nest a gain inside the survival verdict.** 9 are the
ATTACKER's lifelink (`(you kill it, your blocker lives (lifelink: they gain 1 from this block only…))`
— `126v152` s7/s11, `123v126` s57/s65/s92, `162v152` s8, `146v126` s20); **1 is squarely the predicted
class** — `126v123` seq 37, `B7. Vampire (1/1) [lifelink…] - may block A1 (your blocker dies, attacker
lives (lifelink: you gain 1))`, an OWN gain, nested, with no un-nested tag. Ask: route both lifelink
flavours through the same out-params. *Falsifier: 0 of N (10/36).*

**R269. A `{right now:}` verdict does not know the seat's own answer is already on the stack.**
`126v146` seq 19: the stack section reads `1 (top): ability: Tribute to Hunger's gain life equal to
its toughness (aimed at Silverquill Silencer) [from your Tribute to Hunger]` and row 1 still reads
`{right now: they control 1 creature - Silverquill Silencer (3/2) is sacrificed, you gain 2}`. Both
true at that instant; together misleading. Same shape at `126v130` s29 and `126v125` s262 — **3 of
that seat's 10 Tribute casts**, and in the game where it burned both copies on one creature it had
none for a nine-attacker board and lost by 10. Ask: append `- but an answer of yours on the stack is
already removing it`. *Falsifier: cast rows naming a victim already targeted by the seat's own stack
object, with no such tail — 0 of N (3 of 3 observed).*

**R270. A menu-level dead verdict.** deck123: **85 of 197 casting menus had no live cast row**; the
pilot performed the six-row scan correctly 84 times and failed once. deck126: 296 `{right now: they
control 0 creatures - at 0 this does nothing}` rows, 27 `[second copy:`, 17 dead-Tutor brackets — and
**six bad spends in one game off menus whose only other Cast row was already dead**. Ask: one line
under the question — `every cast row on this menu is priced dead right now` / `{no row on this menu
changes a number on the board}` — computed from clauses the emitter already produces. It is the mirror
of `{leaves N sources - no other row on this menu needs more than N}`, which works. **This is the
single highest-frequency inference the prompt currently asks the model to perform.** *Falsifier: menus
where every cast row carries a dead marker and no menu-level line prints — 0 of N.*

**R271. Budget the narration by ask kind.** Measured corpus-wide: **82.7% of the mean prompt (median
85.6%) sits above the option rows**, and **54.0% is the GAME LOG above `--- CURRENT SITUATION ---`**;
at deck125's seats the log share is **63.2%** of a 12,618-char mean. The wave's specimen
(`wave56/lategame-specimen.txt`, `125v130` seq 112) splits **narration 20,953 | current situation
2,910 | choices + protocol 436 = 86% narration** for a **three-row land drop** at 32 life against an
opponent on 2 with an empty hand and no creature, **where all three answers win** and rows 1 and 2 are
interchangeable off 14 sources. **133 of that seat's 1,314 decisions (10.1%) are land-drop asks.**
Ask (a cap on a section, never on a menu): `Land drop:` asks whose menu is only `Play <land> … /
Play no land right now` → last **2 turns** of log; `discard` (cleanup) → last 2 turns; `ask`/`priority`
with an EMPTY `ON THE STACK` → last 3 turns; non-empty stack, `blockers`, `attackers`, `reveal` →
unchanged. **Justified as context headroom, not as a latency lever (R257).** *Falsifier: a land-drop
ask carrying more than 3 turns of log — 0 of N.*

**R272. The `Flip Side` display toggle reaches the AI seat, and no LLM seat has a display.**
`Flip Side` rendered **109 rows across 90 windows** (all deck146/deck152 seats) and was taken
**0 times** — **71,557 characters of option text** whose entire content is "you do not need this row".
Below the prompt, the same toggle is the **only** option in **760** priority windows, each built and
discarded (`AIPlayerGPT[phN]: only display-toggle (Flip Side) options; auto-passing without a model
call`). With D8 shipped, the row's remaining reason to exist is a spell back face (30 `otherrestriction`
gates, untouched and unexercised: 0 alternative-cost back-face casts in the corpus). Ask: suppress the
`doubleside` ability from the AI seat's option set — **the `ishuman` lens applied in the opposite
direction from usual.** This is the one proposal in this file that removes a row, and it removes a row
with a corpus take count of zero. *Falsifier: `Flip Side` rows rendered to an AI seat — 0 of N (109);
`only display-toggle` lines < 200 (760).*

**R273. The opponent LIFE TREND line.** `Their untapped sources: N` (R234) is the model: a fact the
pilot would otherwise reconstruct across windows, printed once. deck130's whole face-damage decision
and its CROSS-CHECK latch turn on **whether the opponent's life is rising and by how much per turn**,
and the prompt gives only the scalar `Opponent life: N`. `130v125` is 137 records long and **the
latch's entire input is invisible on 136 of them**; it fired on one Elixir of Immortality activation
and governed thirty turns. Ask: one line — `Opponent life trend: +5 over their last turn (Elixir of
Immortality)` or `unchanged for 6 turns`. Skill #281 makes this the render half of every latch rule.

**R274. A positive form for the fetch row's colour parenthesis.** `… and it adds {W} or {B} (you
cannot make {W} right now)` prints only when a colour is missing, so on a window where both are
available **no row carries it and any rule keyed to the tag is silent** — `123v126` s6 took a Swamp
over a Scrubland there. 5 fetch windows, 4 carrying the negative tag. Ask: a symmetric positive tag,
or mark the row adding the most colours you are short of in the `[<- …]` idiom. Skill #269 is the
guide-side obligation; this is the render side.

**R275. The back-face land row should say it will ask for 3 life.** Every Emeria, Shattered Skyclave /
Agadeem, the Undercrypt / Pelakka Caverns take answers a follow-up menu (`1. pay 3 life - enters
UNTAPPED [usable this turn] / 2. tap - enters TAPPED`); the row says only `puts "<name>" onto the
battlefield as a land (taps for {B})`. At `146v126` s19 the reply's plan asserted "the land enters
untapped" before that menu was shown. Ask: append `{it will ask you to pay 3 life to enter untapped}`
for the pay-life class, nothing for the Pathways (which have no such menu) — the clause is conditional.

**R276. `INCOMING THIS COMBAT` should print the assignable remainder.** The line already splits the
total (`16 unblocked damage - you would be at -8; this KILLS you (of that, 10 from 2 attackers none of
your creatures can block)`). The decision it feeds is "can any assignment get me above 0", and the
seat has to do the subtraction (`123v152` s23: 16 − 3 = 13 > 8, decline — correct, and the guide's
LETHAL rule ordered a block). Printing `best case with every blocker assigned: you would be at -5`
answers the question the header raises, from numbers already computed for the B-lines' trade previews.
Skill #271 is the guide-side gate; this removes the arithmetic entirely.

**R277. Mark ONE row on every priced menu, not only on X menus.** The `[<- …]` marker is the most
reliably followed annotation the render produces — **9 of 9 menus marked, 7 of 9 taken, 6 of 6 at
deck130's seats, 2 of 3 last wave, 0 of 2 before it existed.** The two places it would pay next:
(a) the Starstorm CAST row one screen earlier, where WHETHER is decided and the pilot still reads two
lists by hand; (b) any menu where two rows produce the same `{right now:}` verdict at different prices
(`123v125` s45). Cheapest decision-quality win in the ledger.

**R278. Cycling rows should price the cast they replace.** `cycling with Lay Waste [cost: {2}, Cycle]`
says nothing about the land kill it spends, and the cast row says nothing about the draw it spends. At
`Opponent life: 2` with 14 untapped sources against four of their lands (`130v125` s52-s73, the
opponent sat at 2 life for seven turns) the cast is worth nothing and the dig is the only line, and
**neither row says so**. 137 cycle windows at that seat, 16 taken; at `Opponent life:` ≤ 6, 17 offered
and 3 taken. Ask: `{the cast this replaces: destroys 1 of their 4 lands}`.

**R279. Lift the blocking-trigger total onto the BLOCKS header.** `{blocking trigger, this combat: you
may gain 2, and if you do your converter takes 2 off them}` (45 rows at deck126) renders **beside** the
1-on-1 verdict `(your blocker dies, attacker lives)`, whose plain meaning is "don't". At `126v146`
seq 37 the seat read the death half, benched its only blocker and lost the game at −10 against 10.
Ask: when the seat controls a converter, emit a header the way `INCOMING THIS COMBAT` is emitted —
`BLOCKING THIS COMBAT: each of your N blockers that blocks gains you 2 and takes 2 off them - up to
K off their K+M`. The per-row clause stays; **the total is the number the decision turns on, and totals
are what this render does well everywhere else.**

**R280. `{feeds:}` should count converters in hand as well as on the battlefield.** Every K-of-0
judgement deck162 makes needs two numbers: converters on the battlefield (printed) and converters IN
HAND (not printed — the pilot scans its own hand line for three card names, and at `162v125` s15 it got
that scan wrong by counting Forced Fruition). Ask: `{feeds: … converters on your battlefield: 0; in
your hand: 1}`. It would let the guide drop its card-name list entirely (skill #293's near-miss stays).

**R281. The `decision_reversed_in_prose` broad census disappeared.** Lane A promised it would be kept
**beside** `plan_choice_conflict_narrowed`. It renders **0** (wave 55: 14) and is absent from every
record. Without it, *"0 conflicts"* cannot be distinguished from *"0 counted"* — a silent instrument in
exactly the class skill #263 named. Ask: restore the broad stamp.

**R282. D11's two withdrawal rules never fired, and no window could have fired them.** `is withdrawn
(you stated it,` renders 0/0/—; there were 0 records with a plan older than 40 windows and 0 where the
menu-diff note (473 renders) fired on a plan more than 4 windows old. Ask: nothing to change; recorded
so the PASS on plan-echo max (20, target < 60) is **not** credited to the mechanism (#283 cause (a)).

**R283. `[NAMED BY THEIR <src>: …]` should say whether the price is per-cast or one-off.** `123v146`
s7: `[NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life and draws them a card - you
would be at 14]` at 17 life; the model took it and the log reads `- You lost 3 life (now 14) /
- Opponent drew a card`, primitive verified (`borderline.txt:102764`). **The price claim is now TESTED
and TRUE, 4 renders / 1 take.** What the row does not say is whether declining makes the surcharge go
away — the difference between "pay it now" and "pay it when you must".

---

## NEW — LOW

**R284.** The MDFC land row leads with the lowercased script token before the printed name
(`boulderloft pathway [PLAY THIS AS A LAND: puts "Boulderloft Pathway" …]`). The model echoes the row
it picks, so the token lands in `chosen_text` and in **every subsequent narration line** — `152v162`
carries 54 renders of one such line. Cosmetic for parsing (lane D added the PARSETEST binds); the
narration is re-rendered per prompt, so it is paid for once per prompt for the rest of the game.

**R285.** **1,767 adjacent duplicate event-log lines in 670 records**, dominated by `- Opponent drew a
card` ×1,348 (Howling Mine games, where two draws in one turn is TRUE and is never collapsed to
`(xN)`). Wave-55's `- Phase: Draw` duplicate class is **GONE (0)**. Context-headroom item only (R257).

**R286.** The `only display-toggle (Flip Side) options; auto-passing without a model call` census, for
the record and to close the docket question: **760 lines, all 11 affected games at deck146/deck152
seats** (146v126 263, 146v130 158, 146v152 156, 146v125 56, 152v162 40, 146v162 34, 146v123 20,
152v123 16, 152v130 13, 152v125 2, 152v126 2). **Longest consecutive run in any game: 4.** Phase
spread ph2 93 / ph3 35 / ph4 160 / ph5 42 / ph6 34 / ph7 83 / ph8 51 / ph9 32 / ph10 94 / ph11 103 /
ph12 33 — not phase-7-specific either. **This is NOT lane C's livelock** (whose signature is a run > 100
in one window); it is bounded, inference-free per-window churn. Subsumed by **R272**.

**R287.** `hold_windows_skipped` should have a stderr companion. The hold line already fires 282 times
in one reviewer's games with the right shape; the SAVING is counted only on the `gameend` field
(sum **1,236**, non-zero on 30 of 42 seats, max 153), invisible to the pilot and to a reviewer
cross-tabbing holds against savings. No prompt change asked.

**R288.** Two DIFFERENT cards with byte-identical `{right now:}` verdicts at different prices, unmarked.
`123v125` s45 lists `Cast Devour Flesh {1}{b}` and `Cast Tribute to Hunger {2}{b}`, both reading
`they control 0 creatures - at 0 this does nothing`. Both declined, so nothing was lost. D15's
`{same effect as row N, for K more mana}` keys on the card NAME; keying it on the rendered verdict
clause would cover this. LOW because the pilot has declined every instance.

---

## PROCESS / MEASUREMENT

**R289.** **Literal audits normalise whitespace before counting** (skill #294). Three consecutive
waves' greps reported the deployed guides free of the wave-54 HOLD string; the string is live in
`deck130_strategy.txt` at line 785, wrapped across a newline. Any dead-string sweep, #188 old-renders
column or #268 fourth column runs `re.sub(r'\s+',' ',text)` on both sides first.

**R290.** **A log-line census quotes the line from the lane report or from one observed instance,
never from a paraphrase** (skill #295). The D18 rescue was reported this wave as 0, 3, 4 and 8
firings; the true line contains neither "slot" nor "rescue", and the 0 was a seat measuring its own
predicate. Disk: **8**.

**R291.** **The denominator is printed inside the verdict cell** (skill #300). D1's run prediction is
PASS at 14.99%, FAIL at 16.8% and FAIL at 22.3% on three defensible predicates, and **max run 13 under
all three**. A lane that meets a target on one unit and misses on another is recorded SPLIT, and the
wave-57 prediction is restated in the predicate the wave-56 ledger used.

**R292.** **A verdict-tag census enumerates the tags it counts** (skill #298). This step's own first
discard census read *"426 of 426 annotated, 0 bare"* on the predicate "the row contains a `{`" and
would have discharged R228; with the family named it is **357 of 426 bare**. Reported here as a
process rule because the failure was synthesis's, not a seat's.

---

## PASS / KEEP — recorded so no later lane trims them

The corrected `they would be at K` ability tail and its `THIS WINS THE GAME` flag (145/145, 4 lethal
rows, 3 takes, 3 wins) · `Their untapped sources: N (colours they could make: …)` (2,716/2,716, and it
inverted a 65-turn loss into a T15 win) · `INCOMING THIS COMBAT` with its `; this KILLS you` flag on the
blockers ask (25/25, arithmetic 25/25) · the menace header `need TWO or more blockers each` (3/3, and
**0 false positives** — the positive arm is tested for the first time) · the X marker family and the
`Every listed value is affordable; option 1 is the LARGEST X` header (9/9, largest-first 9/9) ·
`reveal_wait_ticks` / `_secs` (9/9) · the HOLD row and its `{taking this row skips …}` tail
(2,016/2,016 rendered; the tail is the *field's* problem, not the row's — R262) ·
`{blocking trigger, this combat: …}` and `{after this combat: you control N fewer blocker …}` ·
`(their life LOOP is in play: any life they gain or you lose in this combat chains without limit -
fatal to you, …)` — **5 renders, all at deck152's seats, and six correct all-out-attack refusals in
the game it lost anyway**; deck152 asks it be extended off combat rows to the CURRENT SITUATION board
line whenever both halves of a life loop are on the opponent's battlefield · the `PLAY THIS AS A LAND`
row itself (65 rows, 6 takes, 6 arrivals, and the corpus's best colour-screw fix at `146v125` s13) ·
`[NAMED BY THEIR …]` (4/1, now traced end to end) · `[you declined this exact list N times already this
turn]` (877 renders) · the `{leaves N sources - no other row on this menu needs more than N}` clause ·
deck162's `DRAW FORECAST` + `DRAW PUNISHERS` pair (109 renders at deck130's seats, 0 breaks, sixth
corpus) · `mana_only_windows_skipped` (1,482, no cost this corpus can find).

---

## REJECTED, and why

- **"Skip the opponent-turn window whose `ON THE STACK` is empty at a phase this seat has never cast in"**
  (deck125 G4, deck126 G8, seat-125-126 MED #4 — **FIFTH wave running**, now with the strongest
  denominator it has ever had: deck125 **210 of 302** opponent-turn windows at nine phases with zero
  casts, deck126 **226 of 230**, and `mana_only_windows_skipped` at 1,482 as a working precedent in the
  same binary). **REJECTED again.** The denominator objection has been gone for two waves; the doctrine
  objection has not — **it removes a legal window from the model**. The sanctioned levers are unchanged
  and both are in this file: **R270** (tell the pilot the menu is dead instead of hiding it) and the
  HOLD row, which is now at **22.9% adoption on the opponent-turn arm** and which the model takes
  *itself*. The gap between 22.9% and 100% is a guide problem (#233/#276), not a licence.
- **"Stop asking the model at all when the land-drop menu's only non-`Play` row is `Play no land right
  now` and a guide floor forces the play"** (seat-125-126 MED #5, second arm). **REJECTED on sight** —
  it is a hard cap on a legal choice, and the fact that the seat answered `Play <land>` 76 of 76 times
  is an argument that the model is doing it right, not an argument for taking the choice away. The
  first arm of the same item (**a second async slot**) is adopted in full as **R263**.
- **"When the declined note reaches a threshold, hoist the HOLD row above `Cast nothing right now`."**
  **NOT ADOPTED, and recorded so it is not re-filed as new.** deck152's evidence is the best yet —
  **32 of its 33 N ≥ 2 misses answered `Cast nothing right now`, which sits two positions above HOLD** —
  but decline-ordering is a **standing owner ruling** and reversing it to exploit the same position bias
  is the owner's call, not synthesis's. The part that needs no ordering change is adopted guide-side as
  skill **#290** (the anti-anchor clause names the losing row), and deck146 — whose block does name it —
  converts at 25.0% against deck152's 5.7% at N ≥ 2.
- **A guide compensation for R259's illegal land-drop window.** **REJECTED, and deck152's reviewer
  reached the same conclusion unprompted and wrote it down**: teaching the pilot to exploit an
  illegal-but-working play is a rule that dies with the fix, and the guide teaches the rules-correct
  main-phase line instead. Trust doctrine unchanged: a guide describes cards and printed strings, never
  the engine's defects, and **no guide sentence may be written against R259, R260 or R261** — the
  surfaces are fixed. deck146's `ADD ONE FOR EACH …` arithmetic is the one sanctioned exception and it
  is sanctioned only because both its inputs are true surfaces (skill #287) and it ships with its own
  retirement trigger (skill #285).
- **Any win-rate attribution.** deck152 5-1, deck162 5-1, deck126 3-3, deck146 3-3, deck130 2-4,
  deck125 2-4, deck123 1-5; n = 6 per deck on real hands, in the corpus that is also the O8 RNG-split
  shakedown, so draw order is not comparable to wave 55 either. Counts only (#87, #167, #217, #292).
  All seven seats are correctly recorded **NO EXIT**.

---

## MEASUREMENT — what wave 57 must report

1. **HOLD**: renders / takes / rate, PREFIX-matched, corpus and per deck, plus the N ≥ 3 arm
   (this corpus **320 / 2,016 = 15.9%**; N ≥ 3 **60 / 501 = 12.0%**; own-turn 11.8%, opponent-turn
   **22.9%**) — and **the taught-vs-control ratio** (#276: 18.5% vs 2.7%, ~7×, second wave).
2. **Identical-declined runs** in the wave-53 predicate **with the predicate restated** (#300):
   14.99%, max 13, 5.5% of inference.
3. **Stale drops** with the next-ask class attached (65: 44 land-drop / 21 cast) and the rescue count
   from the **quoted** line (8).
4. **`chosen_text`** byte-equality on takes (140 of 2,758 tail-less) and `<refused:` on `-1` (0 of 6).
5. **MDFC**: `PLAY THIS AS A LAND` rows / takes / arrivals, **split by window legality** (65 / 6 / 6;
   44 of 65 illegal), and the pregame land count (4 of 4 wrong at deck146).
6. **Verdict censuses with their tag families named** (#298): discard 357/426 bare; X fit clause 0/40;
   B-line nesting 10/36.
7. **Prompt economics with the concurrency stated** (#257): mean 11,185, > 30 K 0, > 25 K 6,
   narration share 82.7% above the rows / 54.0% game log, 27.97 s per decision at `-j 21`.
