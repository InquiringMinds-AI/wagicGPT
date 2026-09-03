# Wave-56 engine ledger — THE WAVE-57 DOCKET (engine / UX only)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-120952/` — THIRTEENTH fair-hand corpus.
Binary = hermetic `make -B` of master **d0515fae3** (wave-56 step-1: lanes A `7a2e243c7`,
B `5c391be1e`, C `7193e4bb5`, D `e60a7c15d`, E `039af56c6` on base `41d1c15ef`; merged 6aa8341d9 A,
cc7acb04e B, c7f76e299 E, 256ab5eb4 D, d0515fae3 C; `f9a39205b` = O7 Vita `-std=c++14`).
Gate on d0515fae3: suite **1234** (2 failed = `lifeline` + `merrow_reejerey`, the known
concurrency-only pair), **51 AI / 0**, 0 timeouts, PARSETEST **2736 / 0**. Archived
`archives/wagic-d0515fae3-w56step1`. Pilot **qwen36-35b-a3b** on Spark :8084, `-j 21`,
`WAGIC_GPT_TIMEOUT=900`, `MAXTOKENS=6000`. Guides = the wave-55 reviewer revisions (`860528eb5`) +
the wave-55 boundary pass (`d948447ab`).

**Health.** 42 seat logs, 2,854 records = 42 `gamestart` + **42 `gameend`** + 6 `recovery` +
2 `wall_miss` + **2,762 decisions** (ask 2,127 / priority 456 / attackers 88 / discard 55 /
blockers 25 / reveal 9 / bottom 2). **21/21 natural — invariant 00 satisfied**: 0 timeouts,
0 crashes, 0 hangs, 0 adjudications, **0 `Segmentation` / `ASAN` / `assert` in all 42 stderr**.
Wall time ≈ 2 h 10 min. Inference **21.96 h all-latencies / 21.46 h decisions-only = 27.97 s per
decision**. Max prompt **26,298** (`125v130` seq 113); **> 30 K: 0** (was 1); **> 25 K: 6** (was 252).
`giving this decision to the heuristic` **0**; **Baka executed 0 decisions**.

**Every number in this file was re-derived on disk by this step** with python over the JSONL,
counting rows from the rendered `prompt` (#240), PREFIX-matching HOLD takes, naming tag families
(#298), normalising whitespace on guide literals (#294), and `/usr/bin/grep` over the 21 `.stderr`
with the emitter's own quoted line (#295). Where the four seats disagreed, the disk decided and the
disagreement is recorded rather than averaged (#300).

**⚠ NOT IN THIS CORPUS.** Lanes **Y** (ProliferateChooser AI targeting), **Z** (cancel inside an
interrupt window) and **X** (Light-Paws attach) are today's Vita-report hotfixes, merged on master
**after** the corpus binary — their fixes are not exercised here and their residuals are carried
below (D34, D35, D36). A lane **W** for the MDFC land-drop legality gate (**D1**, and probably
**D2**/**D3**) is **IN FLIGHT** and likewise not in this corpus; the three items are marked so a
wave-57 reviewer re-measures rather than re-files.

---

## FALLBACK CLASSES — 6 of 2,762 = **0.22%** (wave 55: 14 of 3,171 = 0.44%; a new floor)

| class | n | records | note |
|---|---|---|---|
| `named_row_reask` | 3 | `123v146` s13, `126v130` s12, `126v130` s58 | all 3 `named_row_reask_recovered`; `_exhausted` **0** |
| `wall_miss_unrecorded` | 2 | `146v123` s19 (`latency_ms` 900042), `130v162` s7 | both paired with a `recovery`; `wall_miss_events` sum 3 |
| `stale_echo` | 1 | `123v126` s68 | recovered |
| **absent entirely** | 0 | — | `plan_choice_conflict`, `all_assignments_illegal`, `stale_livelock`, `unparsed_reply`, `empty_reply`, `timeout`, `engine_answered`, `pass_hold_ambiguous`, `answer_replaced`, `commit_retracted` |

`parse_note` totals: `plan_choice_conflict_narrowed` 14 · `echo_index_conflict` 10 ·
`hold_row_named` 6 · `stale_echo_in_range` 3 · `named_row_reask_recovered` 3 · `name_over_index` 3 ·
`latched_row_mismatch` 2 · `plan_contradicts_noop_row` 1 · `named_row_not_offered` 1 ·
`blocks_last_line_taken` 1 · `attack_last_line_taken` 1 · `long_reply` 1 ·
`repeat_count_under_two` 1 · **`decision_reversed_in_prose` 0 (was 14 — D16)**.
`gameend` sums: `hold_windows_skipped` **1,236** (non-zero on **30 of 42** seats, max 153; was 735
on 21/42) · `mana_only_windows_skipped` **1,482** (all deck126) · `identical_option_asks_resolved` 6 ·
`wall_miss_events` 3 · `wall_miss_unrecorded` 2 · **`cache_hits` present on 0 of 42 (D37)**.
stderr: `auto-passing without a model call` 2,242 · `only display-toggle` **760** ·
`all actions pass-declined` 158 · **`hold re-opened` 144** (116 cast seam / 28 priority seam, every
one naming a changed row) · `dropping stale async answer` **65** ·
`consuming an in-flight answer whose prompt text drifted` **8** · `no reply after 900s` 3 ·
resource 404s 1,774 · `http_error` 0 · `force-close` 0 · `chose -1 of N` 0.

## PER-KIND PROMPT CHARS

| kind | n | mean | p50 | p90 | max | lat p50 | lat p90 | lat max |
|---|---|---|---|---|---|---|---|---|
| ask | 2,127 | 10,854 | 10,106 | 19,079 | 24,732 | 14.0 s | 56.7 s | 935.5 s |
| priority | 456 | 12,653 | 12,406 | 20,900 | 25,947 | 16.9 s | 59.4 s | 745.0 s |
| discard | 55 | 12,842 | 12,354 | 20,222 | **26,298** | 11.1 s | 52.7 s | 460.2 s |
| attackers | 88 | 10,910 | 10,253 | 18,586 | 24,969 | 26.6 s | 58.0 s | 257.0 s |
| blockers | 25 | 10,706 | 9,448 | 17,612 | 25,023 | 40.3 s | 72.2 s | 491.8 s |
| reveal | 9 | 10,681 | 10,731 | 14,528 | 14,528 | 38.0 s | 58.3 s | 58.3 s |
| bottom | 2 | 2,422 | — | — | 2,527 | 78.1 s | — | 78.1 s |
| wall_miss | 2 | 6,902 | — | — | 8,646 | 900 s | — | 900 s |

Total prompt volume **30.89 M** chars (wave 55: 38.60 M over 3,171 decisions); mean **11,185**
(12,174, **−8.1%**). **Narration share: 82.7% of the mean prompt sits above the option rows
(median 85.6%); 54.0% is the GAME LOG above `--- CURRENT SITUATION ---`** (wave 55: 58.1%).
Where the 21.46 h went: opponent-turn windows 5.78 h (26.9%) · windows at declined-note ≥ 3
**2.02 h (9.4%)**, was 11.9% · identical-declined runs **1.18 h (5.5%)**, was 12.4% · the 65 dropped
calls ≈ 0.51 h · the three 900 s wall misses 0.75 h.
**Prompt bytes fell 8.1% and per-decision latency ROSE 7% at the same `-j 21`** — the wave-55
contention ruling confirmed a second time (D40 / R257). **A byte trim is a context-headroom item, not
a latency lever, and this docket does not carry per-decision inference as a target.**

---

# THE DOCKET, RANKED

## D1 — HIGH — the MDFC land-back row is offered where a land play is not legal, and one such play RESOLVED on the opponent's turn. 44 of 65 rows; `restriction{can play land}` is a no-op string
**Record.** 65 `PLAY THIS AS A LAND` rows in 62 records, all at deck146/deck152 seats. Legality of
the window they render in, counted from the rendered `prompt`'s own `phase` and `Land drop:` line:
**21 legal** (`ask` / Main phase 1 / `Land drop: NOT yet used this turn`) and **44 not** — **17
`priority`/Upkeep**, **15 `priority`/Blockers** (7 under `you have no land you could play right now`,
8 under `ALREADY USED this turn`), **1 `priority`/Draw**, **11 `priority`/Main phase 1 under a false
`Land drop:` line** (that last group is D3's overlap).
**The take.** `1788455417-ai_baka_deck152-…-vs-ai_baka_deck162.jsonl` **seq 3** — `phase: Upkeep`,
`Phase: Upkeep | It is the opponent's turn.`, `Mana available: 0`. Reply
`CHOICE: 1 (boulderloft pathway with Branchloft Pathway)`; the next prompt's log reads
```
=== Turn 2 - opponent's turn ===
- Phase: Upkeep
- You used: boulderloft pathway with Branchloft Pathway -> PLAY THIS AS A LAND: …
- You played Boulderloft Pathway
```
and `=== Turn 3 - YOUR turn === … - You played Plains` follows. **CR 305.1** — a land played outside
a main phase, on the opponent's turn, plus a second drop in the next turn half. deck152 won 10/−4.
**Mechanism** (read by the deck146/152/162 seat in `src/MTGAbility.cpp`, quoted as a read, not a
trace): `AbilityFactory::parseRestriction` (`:1069-1099`) recognises only `myturnonly`,
`opponentturnonly`, `assorcery` and the `<my|opponent><phase>only` family and returns
`ActivatedAbility::NO_RESTRICTION` for anything else; `parseCastRestrictions` (`:106-136`) then gates
nothing. The `compare(isflipped)~equalto~1` clause lane D removed from 55 `autohand={0}` entries in
`borderline.txt` had been the only thing keeping the row off illegal windows (**skill #296**).
**Ask.** `restriction{assorcery}` on the 55 entries — it enforces both `player ==
observer->currentPlayer` and `cPhase == MTG_PHASE_FIRSTMAIN || MTG_PHASE_SECONDMAIN` — **plus** a
separate land-drop-remaining check (`assorcery` does not cover the 8 ALREADY-USED rows), **plus** a
fixture that asserts **the ROW IS ABSENT** on the opponent's turn. Lane D's existing negative fixture
`mdfc_land_back_face_upkeep_neg_w56d.txt` passes today only because a CLICK finds nothing in upkeep;
the corpus shows one such click that DID work.
**⚠ Lane W is in flight and may discharge this.** **Prediction:** `PLAY THIS AS A LAND` rows at a
non-main phase, on the opponent's turn, or under `Land drop: ALREADY USED` — **0 of N** (65 → 44).

## D2 — HIGH — the pregame hand header does not count MDFC back-face lands, and it decided 2 of 2 mulligans and the corpus's only bottom
**Record.** `1788455409-ai_baka_deck146-…-vs-ai_baka_deck152.jsonl` **seq 1**:
`Your hand (7 cards), counted by the engine: 0 lands, 7 spells.` /
`Mana sources among those lands, counted by the engine: none - this hand holds no lands at all.` /
`Playing every land in this hand would not cover any spell in it` — on a hand line carrying **both
`Agadeem's Awakening` and `Emeria's Call`**, which this binary plays as untapped lands for 3 life.
The seat **mulliganed**, kept a three-land six, lost −2/18.
`1788455421-ai_baka_deck146-…-vs-ai_baka_deck130.jsonl` **seq 1**: `1 land, 6 spells` with
`Pelakka Predation` in hand — **mulliganed**; **seq 2/3**: `1 land` again with `Agadeem's Awakening`,
kept, then `PUT: 2` = **Agadeem's Awakening — the seat bottomed its own second land**; lost −3/8.
**This corpus produced exactly 2 mulligans and 1 bottom and all three are this defect.** Same seat
then played the back face as a land four times later in the same corpus, including the corpus's best
MDFC positive (`146v125` s13: five lands, three white mana, a `{b}{w}` and a `{2}{b}` hand; took
`agadeem, the undercrypt`, paid 3 life to enter untapped, and its next `Mana available` line read
`{b}{w}` — the exact colour screw that lost a wave-55 game).
Primitives: `borderline.txt:1177 / :33968 / :82646`, each
`autohand={0}:restriction{can play land} name(<back>) flip(<back>) forcetype(land)`.
**Ask.** Count a hand card as a land when it carries a `forcetype(land)` hand entry, and print the
split: `counted by the engine: 2 lands (1 of them a land on the back of a spell: Agadeem's
Awakening), 5 spells`. The colour line and the coverage clause follow the same count. **Stays
HAND-ONLY** — no board frame on a pregame prompt (owner directive).
**Ranked correctness, not render polish (skill #297): the pregame prompt has no later window to
correct it, and the trust doctrine makes the header load-bearing by construction.**
**⚠ Possibly absorbed by lane W.** **Prediction:** hands containing a `forcetype(land)` card whose
header land count omits it — **0 of N** (this corpus 4 of 4 at deck146 seats); **and P10 flips a
second time** — deck146's guide arithmetic route-around (skill #287) is deleted in the same wave.

## D3 — HIGH — the `Land drop:` summary line and a land row on the same prompt contradict each other, 18 rows / 17 windows, and the model believed the row
**Record.** `1788455397-ai_baka_deck146-…-vs-ai_baka_deck125.jsonl` **seq 13**:
`Land drop: you have no land you could play right now (none playable from your hand or any other
zone).` and, three lines later, a numbered option that plays a land — **which the seat took and which
entered**. That line renders on **1,109 windows** corpus-wide; **18 of the 65 back-face land rows sit
on a prompt carrying it** (11 at Main phase 1, 7 at Blockers), and **4 of the 6 takes** were made in
such a window. `LegalActionsOracle::legalLandPlays` is right to ignore an Emeria's Call — it is a
Sorcery and the back face arrives via `AAFlip` — **the sentence it feeds is what is now false.**
**Ask.** Derive the summary line from the same union that produces the option rows. Two rendered
statements about one question must not disagree; under the trust doctrine the model believes the one
it reads first, and here that is the summary. **⚠ Possibly absorbed by lane W.**
**Prediction:** prompts carrying both `Land drop: you have no land you could play right now` and a
numbered land-play row — **0 of N** (18 rows / 17 windows).

## D4 — HIGH — `chosen_text` is not the rendered row: 112 of 320 HOLD takes are tail-less, and it has now produced a wrong docket number in two consecutive waves
**Record.** The `{taking this row skips the rest of this turn's identical windows}` tail is on
**2,016 of 2,016** rendered HOLD rows and missing from **112 of 320** takes (35%); corpus-wide
**140 of 2,758** takes record a `chosen_text` that is a strict prefix of the row it names. Repro
`130v162` seq 82. **Exact-matching reads 208 HOLD takes where the truth is 320** — and it is why the
wave-56 harvest read D8 (below, discharged) as *"21 rows, 0 takes, 0 arrivals"* when the disk says
65 / 6 / 6. Wave 55's boundary pass found the same defect at 134 of 282 and lane A's D4 fixed a
different property (emptiness): **records with `choice >= 0` and empty `chosen_text` are 0 of 2,758**,
a clean PASS on the wrong half.
Second face: all **6** `choice: -1` records (3 `named_row_reask`, 1 `stale_echo`, 2
`wall_miss_unrecorded`) carry **no `chosen_text` at all**, and the promised `<refused: …>` value
renders **0 times**, so a harvest cannot distinguish a refusal from a missing field.
**Ask.** Write `chosen_text` byte-equal to the rendered row on every take; write `<refused: …>` on
every `-1`. **Prediction:** takes whose `chosen_text` is not byte-equal to the rendered row —
**0 of N** (140 of 2,758); `choice: -1` records with no `<refused:` value — **0 of N** (6 of 6).

## D5 — HIGH — the async slot is shared by the land-drop and casting arms: 65 stale drops, 44 of them displaced by a `Land drop:` ask, 0.51 h. The D18 rescue works and is not where the volume is
**Record.** `dropping stale async answer` **65** (wave 55: 43; lane A's target **< 20**) — FAIL.
The rescue line `AIPlayerGPT: consuming an in-flight answer whose prompt text drifted (same seam,
turn, phase and board)` **fires 8 times**, in 8 distinct games (`123v130`, `125v126`, `125v162`,
`146v123`, `146v125`, `146v126`, `146v152`, `162v130`) — so the mechanism is live.
Classification of all 65, on disk: **next ask after the drop = `Land drop:` 44, `Casting decision`
21**; **line immediately before the drop = `AIPlayerBaka: Mana cost is NULL.` 32** (the Baka
opponent auto-tapping a land — a real board mutation that legitimately misses the slot key while the
in-flight answer was for a menu the land drop does not touch), resolving `10DrawAction` 10,
`NextGamePhase (Current phase is: Draw)` 5, an Ob Nixilis `StackAbility` 4, the seat's own hold-seam
line 2, singletons 12. **0 drops are phase-only on an unchanged board** — D18's second prediction
passes exactly while its headline fails. Repro `game-125v123-1788455392.stderr:932`,
`game-123v130-1788455402.stderr:475`; sample context in `game-125v126-1788455432.stderr:530-533`.
The rise 43 → 65 tracks the window mix, not a regression in the slot: HOLD takes went 282 → 320, and
a hold taken at the end of a turn is exactly what leaves an in-flight answer for the next turn's
land-drop ask to displace. Cost **65 × 27.97 s ≈ 0.51 h (2.4% of 21.46 h)**.
**Ask.** A **second async slot** for the land-drop ask, so the two arms do not share one. This is
cheaper than widening the board half of `slotKey` and does not risk consuming a genuinely stale
answer. (The alternative lane A may prefer: exclude permanents that entered on the OPPONENT's side
outside combat from the board half.) **Rejected on sight:** the seat proposal to stop asking the
model at all on a forced land drop — it is a hard cap on a legal choice.
**Prediction:** stale drops **< 20**, and **0** drops whose next ask is a `Land drop:` (44 of 65).

## D6 — HIGH — `INCOMING THIS COMBAT` is emitted from the `blockers` ask only, so a creature-less deck never sees the number that ends its games: 25 of 260 combat-phase windows
**Record.** The line shipped and it is correct — `1788455432-ai_baka_deck126-…-vs-ai_baka_deck146.jsonl`
**seq 41**: `INCOMING THIS COMBAT: 9 attackers, 26 unblocked damage - you would be at -18; this KILLS
you (your creatures may legally block every attacker in that total)`, on **25 of 25** `blockers`-kind
windows, arithmetic verified 25/25 against `my_life`, `; this KILLS you` matching `K <= 0` on 25/25.
**Corpus-wide it reaches 25 of 260 combat-phase windows (9.6%).** Per deck, combat-phase windows
carrying **no** total: deck146 66/67, deck152 53/57, deck126 30/39, deck130 21/26, **deck125 20/20**,
deck162 8/12, deck123 6/14. The gap is structural: the engine generates no `blockers` ask for a seat
with no creature, and **deck125 is creature-less by design** (its own seat counts 68 opponent-combat
windows on a wider phase predicate — `Combat begins` 28, `Attackers` 20, `Combat ends` 20 — with 0
carrying the line either way). `125v146` seq 65 prints `3 are creatures, 3 of them without a
restriction against attacking` beside `Nadaar, Selfless Paladin (4/4) [vigilance]`, two 2/2 Goblins
and 13 life: **ten power on the screen and no total.**
**Ask.** Emit the same device on any `ask`/`priority` window during the opponent's combat, gated on
**THEIR declared attackers** rather than on MY blockers. One line; the `[DRAW PRICE:` pattern pointed
at the number that ends games. **Prediction:** opponent-turn combat-phase windows with a declared
attacker and no incoming total — **0 of N** (204 of 260 by the all-kinds predicate).

## D7 — HIGH — a creature cast row rendered while a converter of THEIRS is on the battlefield carries no price. Fourth corpus, three games
**Record.** `1788455435-ai_baka_deck125-…-vs-ai_baka_deck126.jsonl` **seq 254** (turn 55, 37 life
against 2). Row 3: `Cast Emrakul, the Aeons Torn {15} (15/15) {leaves 3 of your 18 untapped mana
sources untapped} {card text: "…"}` — a mana clause and card text, nothing else. **On the same
menu**, row 1 prints `{removes: Wall of Omens #1, …}` with eleven enumerated legal targets and row 4
prints `{X pricing: max affordable X=15 (18 mana total)}`. The prompt carries
`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond #1, Sanguine Bond #2` **forty
lines above the menu**, and the opponent's line lists four Tribute-capable bodies. The seat took
row 3, they Tributed it, **37 became −8 in one turn**.
**Ask.** `{their converter: if this is sacrificed or destroyed they gain up to <toughness> and you
lose that much - you would be at K}` — the shape the engine already computes twice on that same menu.
**The general finding this item carries, and it is the wave's governing render principle
(skill #273):** every deck125 rule enforced by a **number on the row** went to zero and stayed there
(low-X Revelation 0/86, sweeper-at-a-dead-header 0/124, Lightmine 0/91, the cleanup land rule 0 in
three corpora); the rule enforced by **a paragraph elsewhere in the prompt** has now broken in four.
**Prediction:** creature cast rows under a printed opponent converter line with no converter
price — **0 of N**.

## D8 — HIGH — 357 of 426 discard rows (83.8%) carry no verdict, and the asymmetry is what blocks the guide rule. Second wave at HIGH
**Record.** Tag family counted explicitly (skill #298): the whole family is `{dead right now:`
(37 rows) and `{spare:` (32); nothing else. Per deck: **deck125 220 of 272 bare**, deck123 100 of
115, deck146 28 of 29, deck162 4 of 5, deck130 5 of 5. `{MV` renders **0 times** on a discard row.
Bare on rows the engine **already evaluates for the CAST menu in the same turn**: a Supreme Verdict
or Final Judgment while the opponent header's second number reads 0 (`125v126` seqs 98, 111, 148,
163 — four cleanups in one game, each sending a sweeper the engine could have marked dead), a
Lightmine Field at the same 0 (`125v126` s197, `125v162` s92), an Essence Scatter into a wall deck
(`125v126` s120).
**The asymmetry is the blocker.** `123v130` **seq 22** is a 14-row cleanup on a board where the
opponent controls no creature: the `Tragic Slip` rows carry `{dead right now: 0 legal targets on the
board for it}` and `Tribute to Hunger`, `Devour Flesh` and `Damnation` on the same menu carry
nothing — **an edict at N=0 and a Damnation at `destroys 0` are exactly as dead.** Until the
predicate is one family no guide can teach "the unannotated row is the keep", and every deck's
discard rung stays at "scan for `copy 2 of 2`".
**Ask.** Run the cast-row `{right now:}` evaluator over the discard menu. HIGH for the second wave
because the LAND half shipped and took deck125's send order from 3-of-4 wrong to **0 breaks in two
corpora**; the sweeper half is the same size. **Prediction:** discard rows whose card has a
computable dead-verdict and carries none — **0 of N** (357 of 426 bare).

---

## D9 — MED — D7(a)'s mana-fit clause is unreachable at ANNOUNCE_X: 0 of 40 X rows, against 3,498 `{leaves` occurrences on cast rows
**Record.** 9 X menus, 40 `X = n` rows, **not one** carrying `{leaves N sources}` or `{taps you out}`.
Repro `125v130` **seq 72**: `X = 6 {X pricing: X=6 - you gain 6 life and draw 6 cards} [<- largest
affordable X - X=6 gains 6 life and draws 6 cards; no listed X does more]` with no fit clause
anywhere on the menu. Lane C shipped the per-row `selectAutoTapProducers` call and flagged in writing
that it had never been observed rendering; **this corpus confirms the code path does not reach the
emitted row.** The marker half landed (D10 below is PASS), so the pilot sees WHICH X is largest but
not what any X leaves up. **Prediction:** X rows with no fit clause **0 of N** (40/40). *Falsifier
branch: still 0 renders → the item is a wiring bug at that seam, not a builder bug.*

## D10 — MED — D13 residual: 10 of 36 blockers B-lines still nest a gain inside the survival verdict
**Record.** The new un-nested clauses ship and are correct (`{blocking trigger, this combat: you may
gain 2, …}` 22 occurrences / 6 records; `{after this combat: you control N fewer blocker - M
available now, …}` 26 / 18). Nine residuals are the ATTACKER's lifelink (`126v152` s7/s11,
`123v126` s57/s65/s92, `162v152` s8, `146v126` s20) and are arguably outside D13's out-params;
**one is squarely the predicted class** — `126v123` **seq 37**: `B7. Vampire (1/1) [lifelink…] - may
block A1 (your blocker dies, attacker lives (lifelink: you gain 1))`, an OWN gain, nested, with no
un-nested tag. The attackers window still nests by design. **Ask:** route both lifelink flavours
through the same out-params. **Prediction:** B-lines nesting any gain inside the survival verdict —
**0 of N** (10 of 36).

## D11 — MED — a `{right now:}` verdict does not know the seat's own answer is already on the stack: 3 of 10 Tribute casts, and it cost a game
**Record.** `1788455432-ai_baka_deck126-…-vs-ai_baka_deck146.jsonl` **seq 19** — the prompt's stack
section reads `1 (top): ability: Tribute to Hunger's gain life equal to its toughness (aimed at
Silverquill Silencer) [from your Tribute to Hunger]` and row 1 still reads `Cast Tribute to Hunger
{2}{b} {right now: they control 1 creature - Silverquill Silencer (3/2) is sacrificed, you gain 2}`.
Both true at that instant; together misleading — the second copy will sacrifice nothing. Same shape
at `126v130` **seq 29** and `126v125` **seq 262**. The deck runs two Tributes; in the game where it
burned both on one creature it had none for a nine-attacker board and lost by 10.
**Ask.** Append `- but an answer of yours on the stack is already removing it` when a stack object
targets or removes the victim the row names. Both facts are on screen already; **the row is where the
decision is made.** Guide half shipped this wave (skill #278). **Prediction:** cast rows naming a
victim already targeted by the seat's own stack object with no such tail — **0 of N** (3 of 3).

## D12 — MED — a menu-level dead verdict. 85 of deck123's 197 casting menus had no live cast row, and it is the highest-frequency inference the prompt asks for
**Record.** deck123: **85 of 197 casting menus all-dead**; the pilot performed the multi-row scan
correctly 84 times and failed once (`123v125` s45). deck126: 296 rows `{right now: they control 0
creatures - at 0 this does nothing}`, 27 `[second copy: you already control <name>]`, 17
`[finds only an enchantment card …]` — and **six bad spends in one game** (`126v125` seqs 15, 30, 32,
46, 167, 215), every one off a menu whose only other Cast row was already dead by the engine's own
marker.
**Ask.** One line under the question, computed from clauses the emitter already produces:
`every cast row on this menu is priced dead right now` (or `{no row on this menu changes a number on
the board}`). It adds no choice and removes none; it is the mirror of `{leaves N sources - no other
row on this menu needs more than N}`, which works. **Prediction:** menus where every cast row carries
a dead marker and no menu-level line prints — **0 of N**.

## D13 — MED — the narration budget should be a function of the ask kind: 82.7% of the mean prompt is above the rows, and the wave's specimen is 86% narration for a decision every answer wins
**Record.** Corpus-wide **82.7% mean / 85.6% median** of the prompt sits above the option rows;
**54.0%** is the GAME LOG above `--- CURRENT SITUATION ---` (63.2% at deck125's 1,288 board-bearing
prompts). `wave56/lategame-specimen.txt` = `125v130` **seq 112** splits **narration 20,953 |
current situation 2,910 | choices + protocol 436**: a **three-row land drop** (`Play Seachrome Coast
/ Play Island / Play no land right now`) at **32 life against an opponent on 2 with an empty hand and
no creature**, off 14 untapped sources making {W} 7 / {U} 12, holding three Path to Exile, two Fall
of the Gavel, a Dream Fracture and a Cancel — **rows 1 and 2 are interchangeable and row 3 is wrong
for a reason that fits in nine words.** The information that could change the answer is ~700 of the
2,910 situation chars and **none of the 20,953 narration chars**, which include twenty-two
`Opponent played Mountain` lines. **133 of that seat's 1,314 decisions (10.1%) are land-drop asks.**
**Ask (a cap on a SECTION, never on a menu — the menu is untouched):** `Land drop:` asks whose menu
is only `Play <land> … / Play no land right now` → last **2 turns** of log; `discard` (cleanup) →
last 2 turns; `ask`/`priority` with an EMPTY `ON THE STACK` → last 3 turns; non-empty stack,
`blockers`, `attackers`, `reveal` → unchanged. **Justified as context headroom, not latency (D40).**
**Prediction:** a land-drop ask carrying more than 3 turns of log — **0 of N**.

## D14 — MED — the `Flip Side` display toggle reaches the AI seat: 109 rows in 90 windows, 0 takes, ~71.6 KB of prompt, plus 760 option sets built and discarded
**Record.** `Flip Side` rendered **109 rows across 90 windows** (all deck146/deck152 seats),
**taken 0 times** (wave 55: 11), **71,557 characters** of option text whose entire content is "you do
not need this row" — repro `146v125` **seq 26**, a two-row menu whose rows are (1) the land row and
(2) a ~650-character Flip Side row ending by telling the model it does not need it. Below the prompt,
the toggle is the ONLY option in **760** priority windows, each built and auto-passed
(`AIPlayerGPT[phN]: only display-toggle (Flip Side) options; auto-passing without a model call`).
**The census that answers the carried docket question: 760 lines, longest CONSECUTIVE run 4 in any
game** (146v126 263 lines/187 runs, 146v130 158/111, 146v152 156/121, 146v125 56/42, 152v162 40/33,
146v162 34/27, 146v123 20/17, 152v123 16/12, 152v130 13/10, 152v125 2, 152v126 2); phase spread ph2
93 / ph3 35 / ph4 160 / ph5 42 / ph6 34 / ph7 83 / ph8 51 / ph9 32 / ph10 94 / ph11 103 / ph12 33 —
**not phase-7-specific and NOT lane C's livelock** (whose signature is a run > 100 in one window).
Bounded, inference-free, and every game ended naturally.
**Ask.** Suppress the `doubleside` ability from the AI seat's option set — the `ishuman` lens applied
in the opposite direction from usual. With D8's land row shipped, the toggle's remaining reason to
exist is a **spell** back face (D33), and the corpus contains **0** alternative-cost back-face casts.
**This is the one docket item that removes a row, and it removes a row with a corpus take count of
zero.** **Prediction:** `Flip Side` rows rendered to an AI seat **0 of N** (109);
`only display-toggle` lines **< 200** (760), no run > 4.

## D15 — MED — the board frame carries the opponent's mana but not their LIFE TREND, and a latch built on the trend is invisible on 136 of 137 windows
**Record.** `Their untapped sources: N` (D24, PASS) is the model: a fact the pilot would otherwise
reconstruct across windows, printed once. deck130's entire face-damage decision and its CROSS-CHECK
latch turn on **whether the opponent's life is rising and by how much per turn**, and the prompt
gives only the scalar `Opponent life: N`. `130v125` is **137 records long** and the latch's whole
input is invisible on 136 of them; it fired on a single Elixir of Immortality activation and governed
the next thirty turns (correctly there — 5 a turn against a recurring Hammer's 3 — and it would be
wrong after one lifelink block). **Ask.** One line: `Opponent life trend: +5 over their last turn
(Elixir of Immortality)` or `unchanged for 6 turns`. **Skill #281 makes this the render half of every
latch rule in the pool.** **Prediction:** with the line rendering, hold-rule breaks split by
#280's two columns show **0** in the "no better target could exist" column being scored as misplays.

## D16 — MED — the `decision_reversed_in_prose` broad census disappeared, so "0 conflicts" cannot be distinguished from "0 counted"
**Record.** Lane A promised the broad census would be kept **beside** the new narrowed stamp.
`plan_choice_conflict` **0** (was 9) and `plan_choice_conflict_narrowed` **14** — six of the 14
replies were read and in every one the prose names only the row the CHOICE took, so **0 genuine
rivals were suppressed** and the narrowing is correct. But `decision_reversed_in_prose` renders
**0** (wave 55: 14) and is absent from every record. **Ask:** restore the broad stamp beside the
narrowed one. Silent-instrument class (skill #263). **Prediction:** `decision_reversed_in_prose`
non-zero on a corpus in which `plan_choice_conflict_narrowed` is non-zero.

## D17 — MED — three shipped recovery arms are now TWO WAVES un-executed, and one of them was rewritten this wave on no live evidence
**Record.** (a) `[RE-ASK]`: **3 notices, all three the "not on this list" form**
(`123v146` s15, `126v130` s14, `126v130` s60); the `Your CHOICE line takes row N ("CHOICE: n …")`
shape that lane C rewrote for D3 **never rendered**, so its 0/0 mismatch count is vacuous.
(b) `all_assignments_illegal` + its re-ask arm and `_exhausted` branch: **never fired**; all 25
`blockers` records carry `dropped_assignments: 0`. (c) `plan_choice_conflict_recovered`: **0**.
**Ask:** a deterministic fixture or a `WAGIC_GPT_*` forcing flag for each arm, so a rewrite is not
shipped again against zero live evidence. **Prediction:** at least one live firing of one of the
three, or a fixture that executes each.

## D18 — MED — D11's two plan-withdrawal rules never fired, and the PASS they appear to have earned belongs to shorter games
**Record.** `is withdrawn (you stated it,` renders **0 / 0 / —**. Plan echo max **20** (target < 60),
p90 3, over-40 served plans 0 — but there were **0 records with a plan older than 40 windows** and
**0 where the menu-diff note (`"<card>" is no longer on your menu`, 473 renders) fired on a plan more
than 4 windows old.** Both rules are **UNTESTED under skill #283 cause (a)**, and the plan-echo
improvement is a consequence of the window mix. **Prediction:** if a plan reaches 40 windows or the
menu-diff note fires on a plan > 4 windows old, the withdrawal sentence renders on **100%** of those
windows (this corpus: 0 windows, 0 renders).

## D19 — MED — the fetch row's colour parenthesis has only a negative form, so a rule keyed to it is silent exactly where both colours are available
**Record.** `… and it adds {W} or {B} (you cannot make {W} right now)` prints only for a colour the
seat cannot currently make. `123v126` **seq 6**: turn 4, `{W} 1, {B} 1` on the mana line, a `{b}` and
a `{1}{b}` spell in hand, against a deck with no land destruction — **no row carried the tag** and
the seat cracked Marsh Flats for a Swamp (`and it adds {B}`) over a Scrubland (`and it adds {W} or
{B}`). 5 fetch windows at that seat, 4 carrying the negative tag. **Ask:** a symmetric positive form
(`(this is your only source of {U})`), or mark the row adding the most colours you are short of in
the `[<- …]` idiom. **Prediction:** fetch windows where no row carries a colour tag of either
polarity — **0 of N** (1 of 5).

## D20 — MED — promote the `[<- …]` marker off X menus: it is the most reliably followed annotation the render produces
**Record.** 9 of 9 X menus marked, **marked row taken 7 of 9** corpus-wide and **6 of 6** at deck130's
seats (wave 55: 2 of 3; wave 54: 0 renders, marker did not exist). The two places it pays next:
(a) the Starstorm CAST row one screen earlier, where WHETHER is decided and the pilot still reads two
lists by hand; (b) any menu where two rows produce the same `{right now:}` verdict at different
prices (`123v125` s45 — see D30). **Ask:** mark exactly one row on every priced menu. Cheapest
decision-quality win in the docket. **Prediction:** marked-row takes stay at ≥ 6 of 9 on the extended
family, and takes of a dearer row with a byte-identical verdict — **0 of N**.

## D21 — MED — a cycling row does not price the cast it replaces, and the cast row does not price the draw
**Record.** `cycling with Lay Waste [cost: {2}, Cycle]` says nothing about the land kill it spends.
`130v125` **seqs 52-73**: the opponent sat at **2 life for seven turns** with four lands while the
seat held 12-14 untapped sources; its whole menu was `Cast Lay Waste` / `cycling with Lay Waste` /
`Cast nothing` / hold. It took the cycle **once** (s66). **137 cycle windows at that seat, 16 taken;
at `Opponent life:` ≤ 6, 17 offered and 3 taken.** **Ask:** `{the cast this replaces: destroys 1 of
their 4 lands}` on the cycle row, and the symmetric clause on the cast row.
**Prediction:** cycle takes at `Opponent life:` ≤ 6 rise above 3 of 17.

## D22 — MED — lift the blocking-trigger TOTAL onto the BLOCKS header, the way `INCOMING THIS COMBAT` is lifted
**Record.** `{blocking trigger, this combat: you may gain 2, and if you do your converter takes 2 off
them}` (45 rows at deck126) renders **beside** the 1-on-1 verdict `(your blocker dies, attacker
lives)`, whose plain meaning is "don't". `126v146` **seq 37**: the seat read the death half, answered
`no blockers`, forfeited 2 damage stopped + 2 life + 2 off their total, and lost that game at **−10
against 10**. The same wave's `; this KILLS you` header went 0-for-1 on maximising damage stopped for
the same reason. **Ask:** `BLOCKING THIS COMBAT: each of your N blockers that blocks gains you 2 and
takes 2 off them - up to K off their K+M`. The per-row clause stays; **the total is the number the
decision turns on, and totals are what this render does well everywhere else.**
**Prediction:** blockers windows where the seat controls a converter and declines all blocks under a
non-lethal header — falls below 1 of 1.

## D23 — MED — `{feeds:}` counts converters on the battlefield but not in hand, which is the discriminator its own release condition is written against
**Record.** Every K-of-0 judgement deck162 makes needs both numbers; only the battlefield count is
printed, so the pilot scans its own hand line for three card names. `162v125` **seq 15**: cast
Dictate of Kruphix on a row reading `converters on your battlefield: 0` with only **Forced Fruition**
and two Islands in hand — a FEEDER read as a converter — handing the opponent an extra card a turn
for four turns before a converter arrived. **Ask:** `{feeds: … converters on your battlefield: 0; in
your hand: 1}`. It would let the guide drop its card-name list entirely (the named near-miss stays,
skill #293). **Prediction:** `{feeds:}` takes at `converters: 0` with no converter in hand — 0 of N
(1 of 3 this corpus).

## D24 — MED — `INCOMING THIS COMBAT` should print the ASSIGNABLE REMAINDER, not only the unblockable half
**Record.** `123v152` **seq 23**: 8 life, `INCOMING THIS COMBAT: 4 attackers, 16 unblocked damage -
you would be at -8; this KILLS you (of that, 10 from 2 attackers none of your creatures can block)`,
one blocker whose only legal assignment removes 3. The seat answered `BLOCKS: none` and was **right**
— 16 − 3 = 13 > 8 — while its guide's LETHAL rule ordered a block. The decision the header raises is
"can any assignment get me above 0", and the seat must do the subtraction. **Ask:** `best case with
every blocker assigned: you would be at -5`, from numbers already computed for the B-lines' trade
previews. **Prediction:** lethal-header blockers windows where the seat must subtract to reach the
right answer — **0 of N**.

## D25 — MED — `MTGAbility::target` ownership: lane C fixed the DEREF, not the ownership, and `a->source` is read unguarded in the same loop
**Record.** D17 discharged: **0 SEGV in 21 games**, `WAGIC_GPT_BOARDINDEX=0` usable for the first
time (stub exits 0, 6/6). But the fix is `liveCardTarget` / `livePlayerTarget` at 5 deref sites plus
`abilityCanReactTo`; **dead abilities still sit in `ActionLayer::mObjects` with a dangling `source`**.
No new evidence either way this corpus — the fix held and the ownership item stands.
**Prediction:** none a translog can see. Ask: an owning handle or a sweep of `mObjects` on zone
destruction, with an ASAN run of the stub at `BOARDINDEX=0` now that the flag works.

## D26 — MED — the back-face land row does not say the land will ask for 3 life
**Record.** Every Emeria, Shattered Skyclave / Agadeem, the Undercrypt / Pelakka Caverns take answers
a follow-up menu (`1. pay 3 life - enters UNTAPPED [usable this turn] / 2. tap - enters TAPPED`); the
row says only `puts "<name>" onto the battlefield as a land (taps for {B})`. At `146v126` **seq 19**
the reply's plan asserted "the land enters untapped" before that menu was shown. **Ask:** append
`{it will ask you to pay 3 life to enter untapped}` for the pay-life class and nothing for the
Pathways, which have no such menu — the clause is conditional. **Prediction:** plans asserting an
untapped arrival before the follow-up menu — 0 of N.

## D27 — MED — `[NAMED BY THEIR <src>: …]` does not say whether the price is per-cast or one-off
**Record.** `123v146` **seq 7**: `Cast Intruder Alarm {2}{u} [NAMED BY THEIR Silverquill Silencer:
casting this costs you 3 life and draws them a card - you would be at 14]` at 17 life; the model took
it and the log reads `- You cast Intruder Alarm / - You lost 3 life (now 14) / - Opponent drew a
card`. Primitive verified (`borderline.txt:102764`, `auto=chooseanameopp … life:-3 opponent &&
draw:1 controller`). **The price claim is now TESTED and TRUE — 4 renders / 1 take.** What the row
does not say is whether declining makes the surcharge go away, which is the difference between "pay
it now" and "pay it when you must". **Prediction:** none; a one-clause addition.

---

## D28 — LOW — the MDFC row leads with the lowercased script token before the printed name
`boulderloft pathway [PLAY THIS AS A LAND: puts "Boulderloft Pathway" …]`, and in the priority-seam
form `- You used: boulderloft pathway with Branchloft Pathway -> …`. The model echoes the row it
picks, so the token lands in `chosen_text` and in **every subsequent narration line** — `152v162`
carries **54 renders of one such line** from a single take. Cosmetic for parsing (lane D added the
PARSETEST binds); the narration is re-rendered per prompt.

## D29 — LOW — 1,767 adjacent duplicate event-log lines in 670 records
Dominated by `- Opponent drew a card` **×1,348** (Howling Mine games, where two draws in one turn is
TRUE and is never collapsed to `(xN)`). Wave-55's `- Phase: Draw` duplicate class is **GONE (0)**.
With the GAME LOG at 54% of the mean prompt this is the cheapest remaining byte trim — and, per D40,
worth **nothing** in latency. Context-headroom item only.

## D30 — LOW — two DIFFERENT cards with byte-identical `{right now:}` verdicts at two prices, unmarked
`123v125` **seq 45** lists `Cast Devour Flesh {1}{b}` and `Cast Tribute to Hunger {2}{b}`, both
reading `they control 0 creatures - at 0 this does nothing`. Both declined, so nothing was lost.
D15's `{same effect as row N, for K more mana}` keys on the card NAME and rendered **0 times** all
corpus (the same-card window never arose); keying it on the rendered verdict clause would cover this
family too. Subsumed by D20 if the marker is promoted.

## D31 — LOW — `hold_windows_skipped` has no stderr companion
The hold line already fires with the right shape (`AIPlayerGPT: holding priority at the priority
seam`, 282 firings in one reviewer's games); the SAVING is counted only on `gameend`
(sum **1,236**, non-zero on 30 of 42 seats, max 153) — invisible to the pilot and to a reviewer
cross-tabbing holds against savings without reading gameends. No prompt change asked.

## D32 — LOW — 1,774 resource 404s across the 21 stderr
Unchanged in character (audio plus `thumbnail`); no correctness effect observed. `In GetCost Seems
ManaCost was not properly initialized` likewise unchanged.

---

# CARRIED ENGINE ITEMS WITH NO CORPUS SURFACE

## D33 — MED — the 30 `otherrestriction` SPELL back-face gates (lane D)
Untouched by D8 and **unexercised**: `Flip Side` takes 0, and no alternative-cost back-face cast
appears anywhere in the corpus. They are the only remaining reason the toggle row exists (D14).
**Ask:** decide whether the two-step toggle is the intended route for a spell back face or whether it
gets a direct row like the land face did — and if the former, keep the row for spell backs only.

## D34 — MED — the per-seat target-chooser owner (lane Z residual, and lane Y's generalisation)
Lane Z fixed the cancel path inside an interrupt window (`ActionStack::CheckUserInput` consuming
`JGE_BTN_SEC` before `CardSelector` could run `cancelCurrentAction()`, while
`MTGPutInPlayRule::reactToClick` had armed `targetChooser` + `cardWaitingForTargets` with **nothing
paid**). Lane Y fixed `ProliferateChooser::canTarget`, whose AI guidance existed **in the PLAYER
branch only** while its CARD branch accepted any in-play card with a non-empty counter list, either
side's — the AI proliferated the human's +1/+1 counters five times in one turn on the owner's Vita.
**Neither fix is in this corpus.** The carried item is the general one: `GameObserver::targetChooser`
is process-global while decisions are per-seat, and `AbilityFactory::abilityEfficiency` scores a
proliferate target without asking whose board it is on. **Ask:** a per-seat chooser and a
whose-board term in the scorer. **Prediction:** none a self-play translog can see; the evidence is
Vita transcripts and the fixtures the two lanes shipped.

## D35 — MED — the Runed Crown / Mantle of the Ancients dormancy class (lane X residual)
Lane X fixed Light-Paws, Emperor's Voice: Oracle puts the tutored Aura onto the battlefield **attached
to Light-Paws**, and the engine offered the attach as a choice (owner's Vita report, transcript
`~line 146`, the Aura landing on Starfield Mystic). The carried class is every other card whose
attach target is **fixed by Oracle** and which the engine may be treating as a choice, or which is
dormant because no fixture covers it — Runed Crown and Mantle of the Ancients are the two named
candidates. **Ask:** a sweep of `attach`/`moveto(battlefield)` scripts for a fixed attachment that
the engine renders as a prompt. **Prediction:** a fixture per candidate, RED on the current binary or
explicitly GREEN.

## D36 — MED — the THIRD replay defect (lane E)
The standing *"a 22-turn Baka-vs-Baka game replays to the identical end state"* claim did **not**
reproduce on the base binary (6/6 diverged); the residual is that a recorded mana click inside an
interrupt window comes back `0<name>` refused. Lane E's parked auto-tap clicks
(`mReplayEngineActions`, `WAGIC_REPLAY` only) took lane AA g2 to 147/147, deck5-vs-deck95 to 295/314
and deck3-vs-deck164 from 94 to 121, all byte-identical under `WAGIC_SINGLE_RNG=1`. **This corpus
sees nothing of it** — AI seats consume nothing from the game stream while loading — and the O8 RNG
split's own shakedown prediction PASSED (win rate per matchup inside the wave 53-55 band; **0
non-monotonic library counts** that are not Elixir shuffles; opening hands 42/42 at 7 cards).
**Ask:** fix the interrupt-window mana click, then re-run the three dumps.

## D37 — MED — lane M's cache-hit counters reach no log, FOURTH corpus
`cache_hits` present on **0 of 42** gameends. Unchanged.

## D38 — MED — the phase-7 LIVELOCK (lane C's second, unfixed defect) — and it is NOT what this corpus shows
Lane C's repro is its D17 stub command **without** a `timeout`: both seats emit
`AIPlayerGPT[ph7]: only display-toggle (Flip Side) options; auto-passing without a model call`
forever, 65 MB of one line, the phase never advancing. **The corpus does NOT reproduce it**: 760
lines, **max consecutive run 4**, spread across 11 phases, every game natural (D14). So lane D's fix
neither caused nor cured it, and the stub condition is a different one. **Kept as a docket item for
the stub repro only; re-scoped from "corpus-visible risk" to "unreproduced stub condition".**

## D39 — MED — `kMaxOptSources` 14, the `&&`-wrapped mana producer, and Baka's blindness to the new row
`kMaxOptSources` is 14 and the owner's option-preserving autotap is a no-op above it;
`planPayment` / `potentialMana` / `selectAutoTapProducers` cannot see a `&&`-wrapped producer (this
is very likely the same wiring D9 falls through). **New this wave:** `Baka took the new back-face
land row 0 times in a stacked game` — a heuristic-scorer item, with **0 Baka executions in this
corpus** so no new evidence either way.

## D40 — MEASUREMENT / RULING — per-decision inference at a fixed `-j` is not an engine metric, CONFIRMED A SECOND TIME
Mean prompt **fell 8.1%** (12,174 → 11,185) while per-decision inference **rose 7%** (26.1 → 27.97 s)
at the same 21 concurrent games — the rival hypothesis (prompt size drives latency) predicts the
opposite sign, for the second consecutive wave. **A prompt-byte trim may not be justified by a
latency argument at this concurrency**; D13 and D29 are context-headroom items and are ranked as
such. **Prediction:** a third confirmation retires the trim-as-latency-lever permanently.

## D41 — MED — the audit lanes' central watch items remain unobservable from a corpus
Lanes G / H / I / K / N: no RSS telemetry, no ASAN, no per-tick verdict trace, 0 Baka executions, no
Vita exposure. Confirmed only that **no crash, hang, segfault or new error signature appears in the
42 stderr**.

## D42 — MED (frontend / human seat) — Arena-style grouping + the `CardGui::Render` per-card cost — **OWNER ITEM**
Unchanged. Carried.

## D43 — MEASUREMENT / DECISION — the log window — **OWNER ITEM**
Unchanged. Carried, and it is now the item D13 is a partial, ask-kind-scoped answer to: D13 proposes
a per-ask-kind budget the owner has not ruled on, and it is filed as a proposal, not a change.

## D44 — CARRIED — Vita
`vpk15` (`6e8a7782c` = vpk14 + `-std=c++14` only) is built and archived, **awaiting the owner's
upload window** — the O7 A/B. Wave 56 ships as **vpk16**, which must also regenerate
`cardauto.{idx,dat}` for the console builds (they keep the gated MDFC script until a port build
does). O11 is **SETTLED** from the vpk13/14 memlog (avg_swp median 0.1 ms, max 0.5 — swap is not the
cost; the update phase is; 22/506 turns over the 16.7 ms budget), so the 30 fps cap expression is
moot and A3-class update spikes are the residual. The owner's "counters only once" vpk13 report is
parked by him (card unnamed; Survivalist RULED OUT). **FTP listing of `ux0:/data/Wagic/ai/gpt/` timed
out — do not retry blind.** Lanes Y / Z / X are all vpk15 reports and all three are fixed on master.

---

# DISCHARGED — SET B: the wave-56 docket D1-D35 and lanes A-E, by the emitter's actual string

Reconciled across the four seats; where they disagreed, the **disk** decided and the disagreement is
named. Audits are **old renders / new renders / takes** (#188), with #268's fourth column where a
guide sentence was built on the old string.

| item | verdict | evidence (re-derived) | seat disagreement resolved |
|---|---|---|---|
| **D1** hold re-open predicate | **PASS** | same-turn byte-identical re-asks after a HOLD take **14 of 320 (4.4%)**, was 113/282; `hold re-opened` 144, **every one naming a changed row** (116 cast seam / 28 priority seam); inference in runs **5.5%** (target < 6%), was 12.4% | orchestrator harvest 10, engine seat 14 — same class, stricter adjacency rule. Both PASS |
| **D1(b)** run statistics | **SPLIT (#300)** | share **14.99%** (wave-53 predicate) / 16.8% (rows, runs ≥ 3) / 22.3% (rows, runs ≥ 2); **max run 13 under all three** (target < 10). Own-turn 2.4% max 9; **opponent-turn 44.1% max 13** (was 57.8%) | harvest 14.3% / max 12 **NOT REPRODUCIBLE** by this step under four predicates; the engine seat's 14.99% is |
| **D1(c)** life-loss loop < 5 | **UNTESTED — cause (a)** | no mandatory life-loss loop arose; largest run inside one `(turn, phase)` anywhere is 11 (`125v126`), an ordinary priority seam | — |
| **D1(d)** stated risk: games lost while a hold stood | **N = 0** | 21 lost seats, 9 took a HOLD in their last two turns; only `130v162` had lethal-INCOMING windows after a hold (s90/s91) **and the seat was still asked at both, with the full menu** | — |
| **D2** ability-row life | **PASS 0 of 145** | `at 164`/`at 165` render **0**; and 4 rows read `they would be at 0; THIS WINS THE GAME`, taken in 3 of 3 games, **won each** (`125v123` s166, `126v130` s95, `125v130` s115). Audit **0 / 145 / 99** | deck146/152/162 seat UNTESTED at its own seats (0 renders) — cause (c), no player-damage activated ability in those decks (#283) |
| **D3** `[RE-ASK]` quoted line | **UNTESTED — cause (a), second wave** | 3 notices, all the "not on this list" form; the CHOICE-line shape never rendered; `plan_choice_conflict_recovered` 0 → **D17** | — |
| **D4** `chosen_text` never empty | **SPLIT — PASS 0 of 2,758 empty; FAIL on fidelity 112 of 320 / 140 of 2,758 tail-less; `<refused:` 0 of 6** | → **D4 (new docket)** | the boundary-pass finding and lane A's D4 are the SAME field seen from two sides, adjudicated together as asked |
| **D5** discard verdicts | **FAIL 357 of 426 bare (83.8%)** | tag family named (#298); `{dead right now:` 37, `{spare:` 32, `{MV` **0** → **D8 (new docket)** | **this step's own first predicate ("row contains a `{`") read 0 bare and was wrong**; the seat's 229/282 at its own seats is right |
| **D6** incoming-combat total | **PASS 25 of 25 on the blockers ask; the GAP is structural** | arithmetic 25/25, `; this KILLS you` matches `K <= 0` 25/25; **25 of 260 combat-phase windows corpus-wide** → **D6 (new docket)** | harvest's "38 of 40" was a loose regex (Upkeep/Main windows mis-tagged); engine seat 29/29 and 38/38 on two narrower predicates; all three describe the same emitter |
| **D7(a)** X mana-fit clause | **FAIL 0 of 40 rows / 0 of 9 menus** | lane C's own "never observed rendering" confirmed live → **D9** | — |
| **D7(b)** X marker | **PASS 0 of 9 unmarked** | `largest affordable X` 3 renders / 1 take, `most kills` 6 / 6, `best trade` 0; header + largest-first **9/9** (owner ruling) | — |
| **D7(c)** monotone collapse | **UNTESTED on the collapse — cause (a); PASS on range** | largest X menu 7 rows, largest Sphinx 5 — the 4-row run threshold never crossed. **0 out-of-range answers, 0 `unparsed_reply` on any ANNOUNCE_X record.** 2 of 3 menus where a larger X was affordable still answered X = 3 — **deck125's guide mandates it verbatim** (#261), so the anchor is the guide | — |
| **D8** MDFC land back | **PASS on ALL FOUR ARMS — and the harvest was wrong** | **65 rows / 62 records / 6 takes / 6 arrivals** (Agadeem ×2, Emeria, Pelakka Caverns, Boulderloft ×2), every arrival traced to a `- You played <back face>` line and then to later `- Paid …` lines; `Flip Side` takes **0 of 109** (was 11); **0 turns with two land drops via this route**; the wave-55 `146v130` s22/23/24 land-drop-less shape does not recur. Retired claims render 0/0/0 | **orchestrator harvest "21 rows, 0 takes, 0 arrivals" is WRONG** — its predicate under-counted rows and its take census exact-matched a tail-less `chosen_text` (D4). Cause of the 21: the same literal also appears in the append-only GAME LOG. **Residuals D1/D2/D3 are NEW defects the fix uncovered (#296)** |
| **D9** enumerating clause | **PASS 1 of 1 — thin** | `they sacrifice ONE of these 2, their choice - all tied at MV 3 (their highest)` renders once (`146v152` s30), taken, plan reads "either … or …". The 127 renders of `- they choose which one` are a different emitter | — |
| **D10** opponent open mana | **PASS 2,716 of 2,716** | 98.3% of all 2,762 decisions; the 46 misses are 44 pregame asks + 2 `bottom`, HAND-ONLY by owner directive. Tapped-out form 576 renders | harvest's predicate did not match lane B's literal; the literal is `Their untapped sources: N (colours they could make: …)` |
| **D11** plan withdrawal | **UNTESTED — cause (a), both rules** | 0 renders; 0 windows could have fired either → **D18** | engine seat's PASS on plan-echo max is correctly re-attributed to the window mix |
| **D12** `reveal_stall` | **PASS 0 of 9, wait fields 9 of 9** | `_secs` within 1 s of `latency_ms/1000` on 9/9; ticks uncorrelated with seconds (3,797↔21 s vs 15,475↔48 s) — the structural signature working. Max `_secs` 58 against a 2,700 s floor | three seats agree |
| **D13** B-line nesting | **FAIL 10 of 36** | 9 attacker-lifelink, **1 the predicted own-gain class** (`126v123` s37) → **D10** | deck146/152/162 UNTESTED at its 9 blockers windows — cause (a) |
| **D14** equipment at 0 creatures | **FAIL 1 of 62 — rate 33% → 1.6%, and it is now a GUIDE item** | `{right now: you control 0 creatures - this equips nothing}` on **62 rows / 62 records**; the one take is `123v125` s45 at 7 untapped sources. **The render told the truth** | — |
| **D15** duplicate cast row | **UNTESTED — cause (a)** | `{same effect as row N, for K more mana}` 0/0/0, and 0 menus carry two `Cast <same card>` rows with an identical verdict at different cost. The sibling that DID arise is two different cards → **D30** | — |
| **D16** `plan_choice_conflict` narrowing | **PASS 0 (was 9) — with an INSTRUMENT DEFECT beside it** | `plan_choice_conflict_narrowed` 14, 6 replies read, 0 genuine rivals suppressed; **`decision_reversed_in_prose` 0 (was 14)** → **D16** | — |
| **D17** `WAGIC_GPT_BOARDINDEX=0` SEGV | **PASS 0** | 0 `Segmentation` / `ASAN` / `assert` in 42 stderr; 21/21 natural; base binary's 2/5 stub crash rate does not recur; the flag is usable for the first time. Ownership residual → **D25** | — |
| **D18** stale-drop slot rescue | **FAIL on the headline (65, target < 20); PASS on the second arm (0 phase-only-on-unchanged-board); the RESCUE FIRES** | rescue line **8 firings in 8 games**; 44 land-drop arm / 21 cast arm → **D5** | **four different counts were filed: 0, 3, 4 and 8.** The **0** (`grep 'slot\|rescue'`) is an instrument failure — **the line contains neither word** (skill #295). Disk: **8** |
| **D19** contention ruling | **CONFIRMED A SECOND TIME** | prompt −8.1%, latency +7% at the same `-j 21` → **D40** | — |
| **D20-D24** LOW carries | **D20 partly closed** (`- Phase: Draw` duplicates 0; `- Opponent drew a card` 1,348 remains → **D29**) · **D22** conditional `{kills N of the …}` untouched · **D23** 404s 1,774 → **D32** · **D24** DRAW FORECAST **PASS/KEEP** (109 renders at deck130's seats, 0 breaks, sixth corpus) | | |
| **D25-D35** carried | **D25** (cache counters) still 0 of 42 → **D37** · **D26** (`all_assignments_illegal` arm) **UNTESTED, second wave** → **D17** · **D27** `{feeds:` release **shipped guide-side**, render half → **D23** · **D28** replay → **D36** · **D29/D30** → **D39** · **D31/D32** unchanged · **D33** → **D42** · **D34** → **D41** · **D35** → **D43** | | |
| **lane E (O8 RNG split)** | **PASS** | win rate per matchup inside the wave 53-55 band (152 5/6, 162 5/6, 126 3/6, 146 3/6, 130 2/6, 125 2/6, 123 1/6); **0 non-monotonic library counts** that are not Elixir of Immortality shuffles (13 increases, all deck125 seats, all with Elixir in play); opening hands **42/42 at 7 cards**; 0 fixture-class anomalies | — |
| **carry: Baka executions** | **PASS 0 of 2,762 (0%)** | `chose -1 of N` 0; `giving this decision to the heuristic` 0 | — |
| **carry: mis-executions** | **PASS 0 genuine** | 2,583 replies carry a `CHOICE: n (label)` parenthetical; **13 labels disagree with the executed row (5.03 / 1,000) and all 13 are cosmetic short-forms**; the one that looked substantive (`130v125` s43, labelled `Cycle Spark Spray`) took the numbered CAST row it coded, which is the doctrine | — |
| **carry: recovery pairing** | **PASS 6 of 6** | 6 `recovery` for 6 `choice: -1` | — |
| **carry: menace header** | **PASS 3 / 3, positive arm TESTED for the first time** | `need TWO or more blockers` 3 renders (all `126v146` s32/s37/s41), **0 elsewhere**; both counts verified against the A-lines | — |
| **carry: phase-7 livelock** | **DOES NOT REPRODUCE** | 760 lines, max run 4, 11 phases → **D14** (waste) and **D38** (the stub condition stands) | three seats agree |

# DISCHARGED — SET A: wave-55 guide edits, by deck (reviewer verdicts, summarised — this step re-derived only what a docket item rests on)

**deck123** — the seat's verdict is **the ceiling is the deck** (skill #267): in five losses it
controlled a creature in **2 of 194 windows**, in three of them 0 of 156. Rungs held at rates the
guide has never had: land drops 24/24, Damnation 25 rows / 0 casts, edicts 238 rows / 0 casts,
`[second copy:` 20/0, 85 all-dead menus with ONE cast off them. **P3 FAILS 1 of 2** (the Greaves,
now a pure guide rung — D14 discharged). Guide **−189 bytes**, second consecutive shrink.
**deck130** — X marker **6 of 6** (P9/P10 PASS N/N), draw-punishers 109/0 sixth corpus, Talisman life
PASS sixth corpus, land drops 24/24, cycling **P11 PASSES** (was 4 wrong). **P12 FAILS 5** by the
letter and **0 of 5 by the revised release** (skill #280); **P14 FAILS 1**, second corpus, same
plan-verb shape (skill #282). `[legendary:` 0 renders — UNTESTED, cause (a).
**deck125** — land drop 76/76, sweeper-at-a-dead-header **0/124**, Lightmine **0/91**, X = 3 3/3
including a collapsed menu (skill #274 now shipped in the guide), Staff face 78/78 (skill #272's
scoping case). The Emrakul stop **broke a fourth time** → D7.
**deck126** — the **zero-hold-text control** held its ~7× ratio (18.5% vs 2.7%); Rule #2's absolute
produced **six bad spends in one game** (skill #277); the blocking-trigger literal was not named in
the rule that owns the win condition (skill #279); zero-Tribute rung 0/218, dead-tutor 0/18,
reveal picks 5/5.
**deck146** — **P10 fired and did its job**: 44 windows / 4 takes / 4 arrivals, and the three-wave
"NOT LANDS FOR YOU" block was retired **with its eight downstream citations in one pass**
(skill #285/#286). **P1 FAILS 2 by the key and both plays are correct** — rule over-brakes
(skill #288). P2 PASS 0/4 + 0/4 but its key is obsolete; P3 PASS.
**deck152** — **P5 PASSED and the wave-55 disaster matchup inverted** (a 65-turn loss became a T15
win) on the strength of `Their untapped sources:`, a RENDER line; P4 PASS 0/N. HOLD take rate
4.5% → 16.8% overall but **2 of 35 at N ≥ 2, with 32 of the 33 misses answering `Cast nothing right
now`** (skill #290).
**deck162** — **the lane closes**: declined-note N ≥ 2 windows **65 → 0**, `hold_windows_skipped`
6 → 1 of 6 seats, and the record (5-1 both corpora) says nothing (skill #292). P6 FAILS 3 by the key,
all 3 correct (over-brakes); **P7 FAILS 1 of 3** — `162v125` s15, Forced Fruition counted as a
converter (skill #293); P8 FAILS 3 by the key, all 3 correct.

**Mulligans (new-baseline record).** 42 opening asks + 2 mulligan re-asks = 44 pregame prompts;
**40 of 42 seats kept 7; 2 mulliganed once and both then kept 6**; 2 `bottom` records, both the
model's. **Zero chains, zero double-mulligans, zero mull-to-zero, no mulligan decided by the
heuristic.** **The OWNER-QUESTION trigger (a repeated mulligan against an explicit floor) did NOT
fire** — and both mulligans and the one bottom are **D2**, a render defect with an engine fix, not a
floor break. The wave-55 three-land-seven oddity did not recur; deck125 and deck126 shipped 12 offers
with 0 mulligans and not one `would not cover any spell in it` line.

---

# SPECIMENS

**SHIP `wave56/lategame-specimen.txt` = `125v130` seq 112** (turn 59, `ask`, **24,299 chars, 3 rows**,
32 life vs 2) — already staged, nothing to regenerate, and it poses the owner's invariant-0 question
in its sharpest available form: **a three-row land drop where all three answers win, billed at 86%
narration** (`wave56/lategame-specimen-logcost.txt` splits it **20,953 | 2,910 | 436**). Rows 1 and 2
are interchangeable off 14 sources; the opponent is at 2 with an empty hand and no creature and two
Staffs of Nin win on the next upkeep whichever row is taken. The 125/126 seat's read is carried
verbatim into **D13**, and it is the one exhibit that makes the narration-budget ask concrete.

**Beside it, and the reason to keep two:** `126v146` **seq 41** (turn 27, **Blockers, 25,023 chars**,
log share 59%) is the wave's whole NEW render surface on one page — the menace header (`2 of the 9
attackers need TWO or more blockers each; you have 2 blockers, enough to complete at most 1 of those
blocks`), 9 A-lines with per-attacker menace annotations, `INCOMING THIS COMBAT`,
`{blocking trigger, this combat: …}`, `{after this combat: you control 1 fewer blocker …}` and
`Their untapped sources:` — **and the model blocked the two Goblins with its two Walls, which is the
right read of that header.** If the owner wants one file for invariant 0, ship the primary; if he
wants to see what the wave BUILT, this is the page.

**Recorded and DECLINED as substitutes, so they are not re-proposed:** `125v123` seq 166 (the D2
exhibit — two `they would be at 0; THIS WINS THE GAME` rows, taken, game over; 16,328 chars) is the
best *single-defect-closed* page but poses no question; `146v152` seq 41-58 and `130v125` seq 84-113
are both multi-record READING assignments, promoted for the lanes and declined for invariant 0, which
asks its question of ONE screen; `146v130` seqs 1-3 (three pregame prompts under 6 KB whose land tally
is wrong by one, ending in a PUT that bottoms a land) is **the shortest complete exhibit for D2** and
is promoted for the wave-57 lane brief.
