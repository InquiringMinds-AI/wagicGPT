# Wave-60 engine-narration seat

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-171140/` (binary = master `614f8452e`).
Method: every count below is taken from the rendered `prompt` text with python, never from `options`.
HOLD takes are prefix-matched against the rendered row. Card facts checked against
`projects/mtg/bin/Res/sets/primitives/*.txt`.

---

## 1. Corpus census

| | |
|---|---|
| games | 21 (42 seat files); **21 natural, 0 crashed, 0 hung** (results.tsv has 21 winners) |
| records | 2,426; **2,330 prompt-bearing decisions** |
| by kind | ask 1,654 · priority 459 · attackers 105 · discard 45 · blockers 40 · reveal 21 · recovery 12 · bottom 4 · wall_miss 2 · gamestart/gameend 42 each |
| game length | turns 11 - 66 (median 15) |
| latency (excl. wall misses) | median 18.5 s · mean 32.9 s · p95 95.1 s · max 744.8 s · 15 decisions over 300 s |
| **fallbacks: 12** | `named_row_reask` 8 · `named_row_not_offered` 2 · `wall_miss_unrecorded` 2. All 12 carry a following `recovery` record. |
| stale drops (`async_drops`) | **64**, all `[outcome: re-asked]` — 40 land-drop arm, 24 casting arm (18 "question moved", 5 "question and board moved", 1 "board moved"). 0 lost decisions. |
| transport failures (`transport` field, lane H) | **2 records**, both `curl=28,http=0,empty=1` at 900,024 / 900,027 ms |
| `transport_error` retries | **0** (the class did not fire; see L/M adjudication B4) |
| timeouts / wall misses | 2 (`wall_miss_events` 2, `wall_miss_unrecorded` 2), both turn 14, both recovered on the next window (39.7 s / 30.9 s) |
| HOLD | **2,736 rows rendered** (1,512 "Hold priority for the rest of this turn", 558 "Cast nothing right now", 494 "Cast nothing right now (combat comes next this turn)", 172 "Hold <land>"); **431 takes** by prefix-match |
| windows suppressed | `hold_windows_skipped` 2,013 · `mana_only_windows_skipped` 533 · `identical_ask_answers_reserved` 136 · `identical_option_asks_resolved` 8 · `dropped_assignments` 0 |
| prompt bytes | 26.6 MB over 2,280 log-bearing prompts; median 9,954 · p95 25,206 · **max 31,096**; GAME LOG = **62.9%** of prompt bytes (wave 59: 76%) |
| **wave-58/59 crash signatures** | `empty_reply` **0** · `bad_reply` **0** · `transport_error` **0** · `deadline_pct` **field absent from the translog this wave** · `reveal_stall_forced` **0** · no translog gap past the watchdog (every seq is contiguous per file) · 21 stderr logs: **0** `Segmentation`, 0 `abort`, 0 `assert`, 0 signal lines |

---

## 2. Prediction adjudication

`N of M` counts are from the rendered prompts of this corpus. UNTESTED means the window
never arose; it is not a soft PASS.

### Lane L

| # | Verdict | Evidence |
|---|---|---|
| **1 (B1)** | **PASS (negative arm) / UNTESTED (positive arm)** | 6 win-token renders in the corpus; **0 of 6** sit on a row that also prints a `this KILLS you` cast price, and the seat's life on all six is 33/83/10/10/8/8 — comfortably above the row's own price. All six are followed by a `gameend` the seat won or by a legal continuation. `WOULD win, but you lose N life first`: **0 renders** — no window held a lethal cast price beside a win, so the fold itself is still unobserved (exactly as lane L predicted of its own probes). The near-miss that proves the *cast-price* half works: `1788559915-...deck130` s62/s63 (t20, life 2) rendered `[NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life ... you would be at -1; this KILLS you]` with **no win token**, and the model declined twice on that exact reading. That is wave-59 HIGH-1's board, correctly rendered. |
| **2 (B2)** | **PARTIAL FAIL** | Label half **PASS**: **0 of 408** stack renders carry a bare `'s triggered ability]` / `'s activated ability]` with an empty parenthetical; the Ob Nixilis rows now read `triggered ability (deals 1 damage to you)` (6 renders). Total half **FAIL**: of 44 stack blocks containing an ability that damages the seat, **38 carry a correct `ON THE STACK: N damage to you - you would be at K` (N = the row sum, K = my_life - N in all 38)** and **4 print no total at all** — see HIGH-1 below. 43 total lines rendered overall; the 5 my sum-checker flagged were the Ob Nixilis decorated rows and all reconcile. |
| **3 (B11)** | **PASS** | `ATTACK TOTAL:` on **105 of 105** `attackers` records (100%). `can be blocked at all`: **0**. Kill claims: **13** records say `that KILLS them whatever they block`; in **13 of 13** the seat won that game, and 4 of them are the last record before `gameend`. The header reads `N total combat damage to a player` on 105/105 (R4's rewording). |

### Lane M

| # | Verdict | Evidence |
|---|---|---|
| **B4** | **FAIL as written; mechanism not falsified** | The literal falsifier lane M named fired: **2 of 2** records carrying a transport stamp read `curl=28,http=0,empty=1` at `latency_ms` 900,024 / 900,027 — i.e. the full 900 s deadline (`1788559927-...deck130` s27; `1788559934-...deck162` s16). Both are classed **`wall_miss_unrecorded`**, not `transport_error`; `transport_error` fired **0 of 2** again. But curl 28 at 900 s is the *overall* timeout, not the connect timeout a 20 s bound would cap — the seat connected and the server never answered. So the bound is not shown broken; the *prediction* is. Two derived facts: `deadline_pct` **is no longer emitted at all**, so the "never exceeds 100" half is **UNTESTED and unmeasurable**; and no retry was attempted on either record (900 s total, not 1,800), so D10's fresh-deadline arm did not run. |
| **B3** | **PASS (outcome) / UNTESTED (recovery arms)** | **0 of 45** discard replies contained a repeated index and **0** were scored `unparsed_reply` (wave 59: 2 of 66). The ask half is clean: **21 of 21** multi-card discard asks print `; the numbers must all be DIFFERENT (a number written twice counts once)` and **24 of 24** single-card asks correctly omit it. `duplicate_index_deduped` 0, `distinct_index_reask` 0, `distinct_index_reask_exhausted` 0 — no duplicate arrived, so the dedupe and re-ask paths are UNTESTED. Negative ("no two re-asks per discard") holds vacuously. |
| **B13a** | **PASS** | 2,200 `YOUR PLAN` echoes; **234 carry the truncation marker**; longest plan body **478 chars** (marker included), longest *core* 401. Excluding the appended `(note: ...)` / `(your plan's target is not on this window)` caveat paragraphs, **1 of 2,200** cores exceeds 400 chars, by one character (401, marker present) — a boundary rounding, not the wave-59 shape (148 of 410 over 400, longest 1,599). `YOUR PLAN was withdrawn:` **0 renders** — the contradiction detector had no hit; I did **not** measure its false-negative rate. |
| **B13b** | **PASS** | **0 consecutive duplicate event lines** across all 42 seats' final rendered GAME LOGs. |
| **B13c** | **PASS** | 51 (file, turn) pairs render `[you declined this exact list N times already this turn]`; **0 violations** — max N in any turn is 10 and every turn's N is ≤ that turn's `ask`+`priority` record count (wave 59: 23 against 9 asks). |

### Lane N

| # | Verdict | Evidence |
|---|---|---|
| **B5-1** | **PASS** | 52 `{feeds: ...}` rows. **0** name a `@discarded(...)` permanent inside `converters on your battlefield: K`; where Liliana's Caress is present it appears only in the separate `discard punishers (a different class ...)` clause (6 rows). Wave 59: 18 mispaired rows. |
| **B5-2** | **UNTESTED** | 312 `{X pricing:` records exist, but **0** co-occur with an opposing discard punisher; `CLEANUP PRICE` rendered **0** times; `NET 0 life for this cast` **0** times. The window never arose. |
| **B5-3** | **UNTESTED** | **0 of 45** cleanup-discard asks were taken with a discard punisher on the opponent's battlefield. |
| **B6-1** | **UNTESTED (predicted window) — but see MED-1** | `LOOP CAUTION` **0 renders**. The predicted window (both loop halves on the **opponent's** battlefield) never arose: **0** prompts show Sanguine Bond + Exquisite Blood together on the opponent's board. **5 prompts show both on the SEAT's own board** and none carries any closed-loop text (MED-1). |
| **B6-2** | **PASS** | `LOOP HALF PENDING` renders in 18 records across 4 seat files. In every file it fires on the **first** decision at which one half is on a battlefield and the other is in a hand the seat can see — `1788559926-...deck126` s16 (t13), `1788559945-...deck126` s34 (t20), `1788559953-...deck126` s19 (t14), `1788559953-...deck146` s23 (t13). Wave 59: 25 silent decisions first. |

### Lane O

| # | Verdict | Evidence |
|---|---|---|
| **B7 (render)** | **PASS** | 155 `{right now: destroys/exiles ...}` clauses; **56 carry a roster**, and **0** print a nonzero destroyed/exiled count without one. Wave 59: 180 counts, 0 names. |
| **B7 (outcome)** | **UNTESTED** | Deck125's six games are not in this corpus's sweeper windows in a comparable shape; I did not attempt the one-for-one-sweep rate comparison and make no claim. |
| **B8a** | **PASS** | 6 renders of `1. +1: tap or untap permanents with Teferi, Who Slows the Sunset`; **6 of 6** carry `Untap the chosen permanents you control. Tap the chosen permanents you don't control. You gain 2 life.` in full; **0** end in `and...`. The `-2:` row and the cast rows still show the +1 clause elided as `Tap the...` with a visible ellipsis, which is the intended focus behaviour. |
| **B8b** | **UNTESTED** | The +1 was **never activated** in the corpus (`chosen_text` never starts `+1: tap or untap`), so **0** tap/untap sub-menus were rendered. `TAPS their` 0, `normally CANNOT BLOCK` 0 — R5's scoped rewording is likewise UNTESTED. |
| **B10** | **PASS (1 of 1)** | `1788559945-...deck152` s36 renders `FORCED SACRIFICE OF ONE OF YOUR OWN CREATURES: the opponent's effect (Tribute to Hunger) forces YOU to sacrifice a creature YOU control...` and **every** row carries `[you SACRIFICE this; they gain N life (its toughness)]`. **0** renders of the wave-59 generic `TARGET CHOICE for <X> - its "gain life equal to its toughness" ability` header. Only one such ask arose. |
| **B12** | **PASS (render) / FAIL (outcome)** | **14 of 14** venture asks name their source (`VENTURE with Nadaar, Selfless Paladin` / `Triumphant Adventurer` / `Acererak the Archlich`); wave 59 was 0 of 11. The Acererak-sourced ask (`1788559941-...deck146` s33) carries the `own text names THIS dungeon` clause on the Tomb of Annihilation row — 1 of 1. But the seat still chose **Lost Mine of Phandelver 14 of 14, including the Acererak ask**, so the behavioural half ("the Lost Mine default share on Acererak-sourced ventures falls below 2/2") is falsified at 1/1. |

### Lane P

| # | Verdict | Evidence |
|---|---|---|
| **B14a** | **UNTESTED** | All **21** reveals were answered (`PUT: n`); **0 declines**, so the decline path never ran. `reveal_stall_forced` **0** across the corpus and 0 force-close lines in 21 stderr logs; reveal waits ranged 495 - 11,557 ticks (3 - 74 s), all completing. |
| **B14b** | **UNTESTED** | 0 declined reveals; the seat-consumption shape had no window. Lane P's own instruction applies: untested, not confirmed. |
| **B9 (render)** | **PASS (1 of 1)** | `1788559942-...deck152` s14 (blockers): `B1. Brutal Cathar (2/2) [daybound] - may block A1 ... {if this leaves the battlefield: their Nadaar, Selfless Paladin (3/3) comes back from exile}`. The crack-back clause follows on s15/s16/s17: `- and 1 more of theirs (3 power) comes back from exile as soon as the creature holding it leaves the battlefield`, and the unconditional total (`for up to 3`) excludes it, as designed. Only one hostage board existed. |
| **B9 (behavioural)** | **UNTESTED** | One board, one game; lane P already called this unseparable from variance. |

### Lane Q

| # | Verdict | Evidence |
|---|---|---|
| **R1** | **UNTESTED** | 7 windows print `casting this costs you N life`, but **every one has exactly ONE naming permanent** (a single Silverquill Silencer). No two-punisher board arose, so the summing fix has no observation. |
| **R3** | **UNTESTED** | 0 `CLEANUP PRICE` renders and 0 `NET at worst` renders; also 0 `NET -N` rows with a cleanup clause. Nothing to falsify. |
| **R4** | **PASS (partial) / UNTESTED (infect, CANTLOSE)** | Header reworded on **105 of 105** ATTACK TOTAL lines (`N total combat damage to a player`). **0 INFECT creatures and 0 `CANTLOSE`/`CANTLIFELOSE` permanents in the corpus**, so those two arms are UNTESTED. No ATTACK TOTAL line names an infect attacker's power (vacuously true). |
| **R6** | **UNTESTED** | **0 indestructible creatures** on any sweeper board; `NOT DESTROYED (indestructible)` 0 renders, `may survive: it can regenerate` 0 renders. |
| **R7** | **PASS (1 of 1)** | The one hostage descriptor reads `their Nadaar, Selfless Paladin (3/3)` and the crack-back clause `(3 power)`. Nadaar's printed body is 3/3 (`borderline.txt:75383-75390`, Scryfall agrees); wave 59 rendered the same card as 4/4 off its live counters. |
| **R8** | **PASS** | See B13a. 234 markers present, marker at the end of the plan body in every case (the 23 apparent exceptions are the caveat paragraph appended *after* the plan block, not a trimmed marker). No plan longer than ~400 lacks the marker except the single 401-char core above. |
| **R9** | **UNTESTED** | `empty_reply` **0**, `bad_reply` **0**, and no `http_status` field appears on any record — no schema fault occurred. Lane Q's own condition applies: untested, not confirmed. |

**D22**: skipped per brief.

---

## 3. Engine / render / narration defects

### HIGH-1 — the INCOMING blocker verdict is false at scale: 1 blocker told it "may legally block every attacker" out of 47
`1788559908-ai_baka_deck162-0x55a47158b570-vs-ai_baka_deck123.jsonl` seq 18 (blockers, t14, life 20):

```
INCOMING THIS COMBAT: 47 attackers, 140 unblocked damage - you would be at -120; this KILLS you (your creatures may legally block every attacker in that total)
B1. Fate Unraveler (3/4) [doesn't untap during its controller's untap step] - may block A1-A47 (all: you kill it, your blocker lives)
```

There is exactly **one** blocker row. The parenthetical is a legality statement about each attacker
individually being blockable, but it reads — and in the model's own words did read — as an
aggregate promise; and the row's `(all: you kill it, your blocker lives)` says a 3/4 survives and
kills whichever of the 47 it meets, which is true per-attacker and impossible in aggregate. This is
wave-59 MED `deck126` MED-1 ("1 blocker vs 5") recurring at 9× the scale, and it is the exact
counterpart of what B11 just fixed on the attackers side: the attackers ask now prints a FLOOR that
respects the blocker count (`At most N can be blocked`), the blockers ask still prints none. 32
`may legally block every attacker` renders corpus-wide. Same record also **omits** the
`best case with every blocker assigned:` clause that 31 of the other 32 carry, so the one window
where the arithmetic mattered most is the one that dropped it.

### HIGH-2 — an activated ability aimed at the seat contributes 0 to the ON THE STACK total (B2's scan misses `target(anytarget)`)
`1788559942-ai_baka_deck130-0x557da07e64d0-vs-ai_baka_deck125.jsonl` seq 118 (t35, life 18):

```
ON THE STACK, waiting to resolve (top first):
  1 (top): ability: Staff of Nin's deal 1 damage [from their Staff of Nin] [triggered/activated ability]
```

No `ON THE STACK:` total line. The very next record's log reads
`- Opponent's Staff of Nin dealt 1 damage to you (now 16)` — the damage was aimed at the seat and
resolved. Staff of Nin's primitive is `auto={T}:damage:1 target(anytarget)` (`mtg.txt:112427`), so
its target is CHOSEN, not bound to CONTROLLER/OPPONENT — precisely the branch lane L's
`scanStackAbilityLife` skips ("source-less abilities skipped because `ActivatedAbilityTP::getTarget()`
resolves CONTROLLER/OPPONENT through `source->controller()`"). The stack object already holds the
chosen target; the scan does not read it. **4 renders** (`deck130` s118 and s139, `deck125` s66,
`deck152` s77 — Staff of Nin every time). Two further no-total blocks are Lightmine Field, which
damages creatures, not players (`mtg.txt:67059`) and is **correctly** excluded. Net: 38 of 42
damaging stack blocks priced, 4 silently unpriced — and Staff of Nin is a repeat-every-upkeep clock,
so the miss is systematic, not incidental.

### HIGH-3 — the loop narration goes silent at the moment the loop CLOSES
`1788559926-ai_baka_deck126-0x564462829dc0-vs-ai_baka_deck130.jsonl`: at seq 16 (t13) the seat holds
Sanguine Bond on its battlefield and Exquisite Blood in hand, and lane N's new line fires:

```
LOOP HALF PENDING: Exquisite Blood is on YOUR battlefield and the other half of the pair, Sanguine Bond, is in your hand. Nothing has chained yet - the pair is one resolution from closing...
```

Two records later (seq 18, same turn, after `You cast Exquisite Blood`) **both halves are on the
seat's own battlefield**, and the entire loop narration is:

```
LIFE-TO-DAMAGE CONVERTER on the battlefield: yours - Sanguine Bond. While it is in play, life ITS CONTROLLER gains also makes the other player lose that much life.
```

Exquisite Blood is not named, the word "loop" does not appear, and the LOOP HALF PENDING line is
gone. The prompt is strictly *less* informative about the combo the instant the combo exists. 5
closed-pair renders across 2 files (`...deck126` s18 ×4, `1788559945-...deck126` s35), **0** with any
closed-loop text. B6-1's clause only fires for a pair on the OPPONENT's board; nothing covers the
seat's own. This is the shape wave-59 `deck162` HIGH-1 and `deck152` #4 were about, seen from the
other side of the table.

### MED-1 — `WINS THE GAME` is asserted on an unresolved spell with the opponent's counter-mana rendered on the same screen
`1788559942-ai_baka_deck130-0x557da07e64d0-vs-ai_baka_deck125.jsonl` s151 (t42):
`{no creature target - and 3 to the opponent at life 3 WINS THE GAME}`, with
`Their untapped sources: 7 (colours they could make: {u}{w})` eleven lines above and *two prior
counterspells in the same game's log*. The Hammer was countered by Dream Fracture; the seat re-ran
the identical line at s155 (t44) against the same open mana, and only then did it resolve. The win
token is a claim about the END of the game made about a spell that has not resolved; B1 taught it to
subtract the pilot's own life price but not to hedge on resolution. 6 of 6 win-token renders in the
corpus are cast/activation rows. The defender's mirror (`this KILLS you`) is about damage already on
the stack and does not have this problem.

### MED-2 — `wall_miss` records still carry a wrong phase, `options: 0`, and now no prompt at all
Both wall misses (`...deck130` s27, `...deck162` s16) record `phase: "Main phase 1"`, `options: 0`,
`choice: -1` and **no `prompt` field**, so nothing in the translog says which question was lost.
Wave-59 flagged the phase/options half; the missing prompt makes the record unauditable. Related and
worth stating plainly: **`deadline_pct` is no longer written to the translog at all** this wave, which
silently removed the instrument lane M's B4 prediction was written against.

### MED-3 — roster and hand lists are comma-joined, and card names contain commas
`{feeds: ... in your hand: 3 - Fate Unraveler, Ob Nixilis, the Hate-Twisted, Underworld Dreams}` —
the count is right (3) but the list reads as four items. Same separator on the new B7 sweeper rosters
(`- THEIRS: Perimeter Captain (0/4) [defender], Pride Guardian (0/3) [defender], ...`), which happens
to be safe only because no comma-named creature was on a sweeper board this corpus. 52 `{feeds:}` rows in the corpus, **38** of
which list a comma-named card (`Ob Nixilis, the Hate-Twisted`). A separator the payload can contain is not a separator;
`; ` or a numbered form would fix both surfaces at once.

### LOW-1 — singular/plural on the sweeper count
`{right now: destroys 1 of their creature (1 without a restriction against attacking), ...}` —
"1 of their creature". 9 renders. Carried over from wave 59, untouched by B7.

### LOW-2 — the `named_row_*` fallback cluster is a plan-vs-menu mismatch, not a parse fault
8 `named_row_reask` + 2 `named_row_not_offered`, all recovered, all in the first ~25 seq of their
games, and all in seats whose plan echo carried the appended
`(note: this decision's list does not contain the actions your plan names...)` caveat. The caveat and
the fallback are two renderings of the same event; only one of them is counted.

---

## 4. Lategame specimen

`wave60/lategame-specimen.txt` — `1788559917-ai_baka_deck125-...-vs-ai_baka_deck126.jsonl` seq 185,
turn 39 (Upkeep), the decision that won the game (row 14, Staff of Nin #2, opp 1 -> 0; the next
record is `gameend won=true`). It is also the corpus's largest prompt at 31,096 characters.

---

## 5. What I did NOT check

* **No guide reading, no strategy judgement.** I did not open any `deck<N>_strategy.txt` or
  `general-strategy.md`, and every "the seat chose X" note above is offered as an engine/render
  observation, not a play verdict.
* **No source reading beyond primitives.** I did not open `AIPlayerGPT.cpp`, `AllAbilities.cpp` or
  any other engine source; HIGH-2's attribution to `scanStackAbilityLife`'s CONTROLLER/OPPONENT
  branch is inferred from lane L's own written scope plus the primitive, and is an inference.
* **No build, no run, no git.** Nothing was compiled and no game was driven; the binary's identity is
  taken from the brief.
* **Only the four cards named above were checked against primitives** (Staff of Nin, Lightmine Field,
  Nadaar, Underworld Dreams via lane L). Nothing was checked against Scryfall by me — Nadaar's 3/3
  and Tribute to Hunger's text are quoted from lanes O/P/Q, which state they were Scryfall-checked.
* **B7's outcome prediction, B9's behavioural prediction and the crack-back `for up to N` overrun
  check (wave-59 MED) were not measured** — each needs a per-turn combat replay I did not build.
* **B13a's contradiction detector false-negative rate is unmeasured.** It fired 0 times over 2,200
  plan echoes; I did not read those echoes for contradictions it should have caught.
* **The 2 wall misses were not attributed to a cause.** Both are turn 14, both `curl=28` at the full
  deadline, in two different games; I did not check whether they are simultaneous in wall-clock time
  (the translog carries no timestamps) or correlate with the endpoint.
* **I did not re-count any wave-58 or wave-59 figure.** Every comparison to wave 59 above is quoted
  from `wave60/known-bugs.md` or the lane files.
* **HOLD takes** are prefix-matched to 60 characters against the rendered row; a HOLD row whose
  chosen text was reformatted beyond that prefix would be missed.
