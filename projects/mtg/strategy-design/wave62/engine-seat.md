# Wave-62 engine-narration seat

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-133058/` (21 games on `12be0b79c`)
plus the single-game rerun `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-161801/`
(152v125 on `9230a94c3`). Every count below is from the rendered `prompt`, not the `options`
array; HOLD is prefix-matched. No wagic was run, no file outside `wave62/` was touched, no git.

---

## 1. Corpus census

| | main dir | rerun | total |
|---|---|---|---|
| games launched | 21 | 1 | 22 |
| natural end (`gameend` pair) | 20 | 1 | 21 |
| crashed | 0 | 0 | 0 |
| HUNG | 1 (152v125, `game-152v125-1788633074.stderr`) | 0 | 1 |
| seat translogs | 42 | 2 | 44 |
| records | 2,238 | 101 | 2,339 |
| decision windows (records carrying a `prompt`) | 2,154 | 97 | **2,251** |

Windows by kind: `ask` 1,752 · `priority` 332 · `attackers` 76 · `discard` 46 · `blockers` 21 ·
`reveal` 16 · `bottom` 7 · `recovery` 2 · `wall_miss` 1. Longest game 61 turns (125v126, 324
decisions on the deck125 seat); shortest seat file 8 decisions. Median window latency 16.0 s.

**Fallbacks — 2 in 2,251 windows (0.09 %), both recovered.**

| fallback | n | where |
|---|---|---|
| `wall_miss_unrecorded_wall` | 1 | `1788633069-…deck123-vs-deck162` seq 16, turn 10, latency 900,030 ms, `deadline_pct 100.0`, `transport curl=28,http=0,empty=1,connect_ms=20000,phase=wall`; recovered at seq 17 |
| `unparsed_reply` | 1 | `1788633091-…deck146-vs-deck126` seq 53, turn 25, `deadline_pct 58.5`, no transport stamp; recovered at seq 54 (Nadaar cast went through) |

**Stale drops (`async_drops`): 15**, every one of value 1, over 10 seat files. Reasons:
`casting/question (or turn/phase)/re-asked` 10 · `casting/board/re-asked` 3 ·
`casting/question and board/re-asked` 2. No drop cost a window — every one is a re-ask.

**Transport (lane H's new field): 2 records carry it**, both the same string
`curl=28,http=0,empty=1,connect_ms=20000,phase=wall`. One became the wall-miss fallback above; the
other (`1788633085-…deck123-vs-deck126` seq 12) carried `retry: 1`, answered on the retry, and is
the corpus's only `deadline_pct > 100` (**116.4**, 1,048 s total = a 900 s wall miss plus a 148 s
answered retry). `transport_error` retries: **0**. Timeouts classed as such: **0**.

**Wall misses**: `gameend` aggregate `wall_miss_events` 2, `wall_miss_unrecorded` 1.

**HOLD** (prefix-match on `Hold priority: pass now, and do not ask me again`): rendered on
**1,437** windows, **taken 227** times (15.8 %). `gameend` aggregates: `hold_windows_skipped`
1,191 · `mana_only_windows_skipped` 1,091 · `identical_ask_answers_reserved` 168 ·
`identical_option_asks_resolved` 3.

**Reveal**: 16 `reveal` windows, wait 6–111 s, 655 s total. `reveal_stall_forced`: **0**.

**Wave-58/59 signatures — recurrence check.** `empty_reply` **0** · `transport_error` **0** ·
`bad_reply` **0** · `reveal_stall_forced` **0** · `deadline_pct > 100` **1** (the answered retry
above, not a loss) · SEGV/abort/`terminate called` across all 22 stderr files **0** · translog gap
past the watchdog **1** — the hung game only.

**Regression checks the brief asked for.**
* *Stale drops*: 15, all benign re-asks (wave 61's shape recurs at the same low rate; no drop
  reached a fallback).
* *Wave-61's livelock fix (hold-check note)*: **holds.** Across all 44 seat files, **no prompt was
  ever re-asked byte-identically** (max repeat count of any identical prompt in a seat file = 1;
  longest run of identical consecutive prompts = 1). All 1,382 `[hold check: …]` notes render in
  the four intended shapes (538 "1 row above is new", 480 "N rows above are new", 215+149 "every
  row above was also on the menu … N window(s) in a row"), and all 427
  `[you declined this exact list N …]` notes carry D13's new scope clause, so the two no longer
  contradict. The *promise* the hold row makes is nevertheless still broken 15 times — see HIGH-2.

**The hang.** Not re-diagnosed; `wave62/corpus-hang.md` is taken as given, and the corpus
**corroborates** it: `game-152v125-1788633074.stderr` carries 88,232 `Action added to stack:
Day|Night` lines against 0–2 in every other game (146v152 2, 152v123 1, 152v126 2), and the
rerun on the fixed binary shows **2** — one `Day` (line 1127) and one `Night` (line 1231), i.e. an
opposite marker cast with one already in play produced exactly one add instead of a loop. The
hung pair's translogs stop at turn 27 with no `gameend`; they are used here as hang evidence only.

---

## 2. Prediction adjudication — 31 predictions: **18 PASS, 5 FAIL, 8 UNTESTED**

### Lane W (4 PASS, 1 FAIL)

* **D1 / prediction 1 (verdicts match the quoted condition) — PASS.** 104 decidable rows checked
  against the same prompt's own battlefield line: 31 checkland rows (subtype gate, resolved against
  the printed permanents, Urborg's `lord(land) transforms((swamp))` honoured) and 73 other-land
  count rows. **0** verdict mismatches, **0** rows whose printed "you control N other lands"
  disagreed with that prompt's own `… and N are lands` header. Both directions of the
  `compare(type:land)` family are Oracle-correct: Scryfall (fetched 2026-09-05) gives Hive of the
  Eye Tyrant and Lair of the Hydra as *"If you control two or more other lands, this land enters
  tapped"* — so `borderline.txt:53392` / `:64291` and the render that follows them are right, and
  the row reading TAPPED at 2 other lands is correct, not inverted.
* **D1 / prediction 2 (unconditional verdicts drop to 0 on conditional lands) — PASS.** 178 rows
  carry an enters-verdict; only **13** use the unconditional form, and all 13 belong to the
  genuinely ungated class (Arcane Sanctum 7, Forgotten Cave 5, Dismal Backwater 1). Wave 61's
  131-of-172 is **0**. The wave-61 hedge string appears 0 times because every conditional row in
  this corpus resolved.
* **D1 / prediction 3 (every UNTAPPED row names a real witness) — FAIL.** 61 witness clauses;
  **3 name a permanent that does not carry the subtype the clause claims**, and one of the three
  carries neither gate subtype. See HIGH-3.
* **D15 — PASS.** `never listed in a casting`: **0** prompts. All **606** prompts that carry a
  `PLAY THIS AS A LAND` row on an available land drop carry the new exception clause. All **90**
  MDFC hand entries (Pelakka Predation 55, Emeria's Call 18, Agadeem's Awakening 17) carry
  `[also a LAND: …]`; **0** missing.
* **D17 — PASS.** `Land drop: play ` **0**; a `Hold <land> - do not play it now` decline row
  **0**. All **371** land-drop asks read `Land drop: which land do you play now, if any?` with
  `Play no land right now` last (371 such rows). `plan_missing` fallbacks: **0** (wave 61: 1).
  The repeat row's new PLAN-rule clause renders 22 times.

### Lane X (3 PASS, 1 FAIL, 2 UNTESTED)

* **D2 — UNTESTED.** Only 2 windows in the corpus put an `ATTACK TOTAL` line on a screen carrying
  a life-loop banner (`1788633085-…deck123-vs-deck126` seq 55, `…deck126-vs-deck123` seq 23), and
  in both the opponent's untapped blockers covered every attacker, so the guaranteed-damage branch
  the kill claim lives on never rendered (`life back across their` 0, `so blocking can leave them
  as high as` 0). The D2 window did not arise. Falsifier count 0 of 0.
* **D6a — UNTESTED.** **0** crack-back windows in the corpus had a planeswalker on the opponent's
  battlefield, so the Lolth shape (17 wave-61 renders) had no chance to recur. The 45 clause
  entries that did print name Staff of Nin 36, Pyrite Spellbomb 7, Siege-Gang Commander 2 — all
  flat `damage:N` sources, so the expression-amount guard was never exercised either.
* **D6b — PASS.** The split gate now prints: **53** `noncreature permanents of theirs that can
  animate and attack are not in that count` clauses, against wave-61's 0 of 53. (The clause's
  *contents* are wrong — see HIGH-1.)
* **D8a — PASS, both halves.** **0** forecasts read `your next draw step` on a prompt whose own
  rendered header is `Phase: Draw | It is your turn` (30 such prompts, 28 `resolving NOW` lines).
  All **200** forecast lines carrying a `would be at N` figure carry `that KILLS` when N ≤ 0;
  **0** missing.
* **D8b — PASS.** `THIS WINS THE GAME` prints on 6 rows; **2** of them off the loop tail
  (`1788633081-…deck126-vs-deck130` seq 16/17, Tribute to Hunger, "both halves of YOUR life LOOP
  are in play, so that chains without limit until they are at 0"). **0** win tails on a row the
  seat could not survive.
* **D19 — FAIL.** `removes N from the CRACK-BACK total above`: **0 occurrences corpus-wide.** The
  window arose twice and both were missed — see HIGH-4.

### Lane Y (3 PASS, 1 FAIL)

* **D3 — PASS (primary), paired half UNTESTED.** **0** `DRAW FORECAST (theirs):` lines carry
  `LOOP CAUTION`; all **4** carry `LOOP SCOPE: they control BOTH halves` (e.g.
  `1788633102-…deck162-vs-deck126` seq 13). **0** own-side forecasts carry `LOOP CAUTION`. No
  screen carried a `theirs` forecast and an own forecast together, so the "the own line still
  carries LOOP CAUTION" half had no window. The four surviving `LOOP CAUTION` strings are all on
  the untouched `DRAW PUNISHERS` board paragraph — see MED-6.
* **D4 — PASS.** **3** distinct `becomes a K/K hydra` strings (1/1 ×7, 2/2 ×5, 3/3 ×2) against
  wave 61's exactly 1 across 387 rows; `1788633079-…deck152-vs-deck126` seq 15 offers three rungs
  at 5 untapped sources, top row `becomes a 3/3 hydra … [cost: {3}{g}]`. (Only 3 rungs at 5 mana,
  not 4 — LOW-8.)
* **D5 — FAIL.** 17 of 18 `ANNOUNCE_X` asks carry the Decline row; one does not — see HIGH-5.
* **D7 — PASS (main clause), taps half UNTESTED.** **100** `{repeat cost: N x {…} = M mana for all
  N; you have S spendable now, which pays for K of them …}` clauses; across the 5 menus with two
  or more `add N counters` rows (all 20-row Intrepid Adversary menus) **0** priced rows lack the
  clause. **0** repeat-cost rows carried `{paying this taps: …}` — no repeat payment plan in the
  corpus tapped a creature, so that half had no window.

### Lane Z (6 PASS, 1 FAIL, 2 UNTESTED)

* **D10 — PASS on the executed-row invariant.** `plan_prose_recode_ignored`: **0** (the shape did
  not recur). The corpus's single `latched_row_mismatch`
  (`1788633092-…deck126-vs-deck146` seq 66) executed *exactly* its line-1 answer: reply
  `CHOICE: 16 (Cast Sanguine Bond)`, row 16 is `Sanguine Bond (enchantment)`,
  `chosen_text` "Sanguine Bond", and seq 66's own `events` confirm
  `You used: choose card with Idyllic Tutor targeting Sanguine Bond`. The record's `choice` field
  reads 1 — a record defect, not a parse defect (HIGH-6), and the stamp is a false positive.
* **D9 — UNTESTED, and clause (c) is not adjudicable from this corpus.** `blocks_restated_prose_taken`
  / `attack_restated_prose_taken`: **0**. Of 97 combat records, **0** replies wrote a second
  line-leading `BLOCKS:`/`ATTACK:`, and the only mid-prose directive
  (`1788633094-…deck162-vs-deck146` seq 16) sits inside its `PLAN:` sentence and was correctly not
  read. No restatement window arose. Clause (c) — the `kReplyProtocol` sentence
  "The LAST answer line you write is the one that runs" — appears in **0** of the 2,251 logged
  prompts, along with every other reply-protocol sentence: see HIGH-7.
* **D11 — UNTESTED.** `named_row_reask`: **0**. No re-ask text of any shape appears in the corpus
  (`that name is not`, `not on this menu`, `was not one of the numbered rows` all 0), so the
  deck126 seq 44/65/82 out-of-range shape did not recur. The `laterStepRouteClause`'s trigger row
  (`(combat comes next this turn)`) rendered 519 times without ever being needed.
* **D12 — PASS.** 23 blockers-seam headers print `best case with every blocker assigned: you would
  be at N`. **20** have N > 0 and **all 20** print `one legal assignment that reaches it:` with a
  named blocker (e.g. `1788633061-…deck130-vs-deck152` seq 15,
  `… best case … you would be at 5 - one legal assignment that reaches it: Dwarven Blastminer
  blocks …`). The 3 without the clause all read `no block saves you` (N ≤ 0), which the prediction
  excludes.
* **D13 — FAIL.** The declined-note half is clean (**427** notes, **427** with the scope clause;
  old hold string **0**, new hold string **1,437**), but the row's promise is broken 15 times —
  see HIGH-2.
* **D14 — PASS.** **180** `{X pricing:}` rows; **0** carry neither a `{leaves …}` count nor the
  new scope tag (`no {leaves …} count on this row` renders 107 times).
* **D16(a) — PASS.** **221** plan-truncation notes, every one carrying
  `[…the rest of your plan was not carried: N further characters, of M you wrote …]`. **0** bare
  wave-60 markers.
* **D16(b) — PASS.** **159** windows rendered with a live designation marker
  (`It is currently Day|Night`), 156 of them offering a sacrifice/destroy/exile/target surface.
  **0** menu rows name `Day`, `Night` or an `[emblem]` object, and **0** battlefield listings
  carry a marker as a permanent. The rerun's ETB/annihilator paths behaved (2 marker adds).
* **D18 — PASS.** 1 of 1: the wall-miss record's `transport` stamp says `phase=wall` and its class
  is `wall_miss_unrecorded_wall`. **0** records classed `_connect`, so the "no `_connect` at
  `deadline_pct ≥ 95`" half holds vacuously. The deck-130 misreading is corrected on the record:
  `connect_ms=20000` sits beside `phase=wall` and the class agrees with the phase.

### Lane AA (2 PASS, 1 FAIL, 4 UNTESTED)

* **R1 — FAIL.** Same falsifier as D5 (HIGH-5): one ANNOUNCE_X ask rendered a 4-row menu with no
  Decline row and the `(this ask has no pass row)` format line.
* **R2 — UNTESTED.** No reply in the corpus wrote a combat directive after a negation token (or
  outside a PLAN block at all), so `combatDirectiveNegatedOnLine` never had a window. 0 of 0.
* **R3 — PASS.** **0** prompts contain two `DRAW FORECAST` lines of the same side (`(theirs)` count
  per prompt is 0 or 1 everywhere), and **0** prompts carry both `LOOP CAUTION` and `LOOP SCOPE`
  on the same side. The merge artefact is gone.
* **R4 — PASS (main claim), fallback wording UNTESTED.** **20** `{repeat cost: …}` rows claim
  `pays for K of them` with K ≥ 1; in **0** of them does the same prompt's `Mana available:`
  colour set fail to cover the per-counter cost's coloured pip (every one is `{1}{w}` on a
  `{u}{w}` or `{w}`-capable board). The `up to N of them if your colours fit` fallback rendered
  **0** times — the unparseable-cost path had no window.
* **R5 — UNTESTED.** Not adjudicable from the corpus surface: the prompt prints
  `Their untapped sources: N (colours they could make: …)` for **this** turn, while the gate
  prices **next** turn's reach, and no prompt prints that figure. 53 clauses render; none can be
  checked for colour fit or self-mana exclusion from the log. (The clause is separately wrong for
  a different reason — HIGH-1.)
* **R6 — UNTESTED.** **0** prompts in the corpus show a `cantchangelife` / `cantlifelose` /
  `cantlose` / prevention shape on either battlefield, so the new gate could not be seen to fire
  or to wrongly stay silent.
* **R7 / R8 — UNTESTED.** **0** decision records carry `fallback: timeout`; **0** carry
  `empty_reply`; both transport stamps in the corpus read `http=0` (no status ever arrived), so
  neither the preserved-503 path nor the answered-empty-200 path was exercised. The one retry
  that did happen (`deadline_pct 116.4`) is a curl-28 wall miss and was correctly retried once
  and answered, which is the pre-existing behaviour, not R7/R8's.

---

## 3. Engine / render / narration defects

### HIGH-1 — the crack-back animate clause lists one entry PER ABILITY, so ONE manland reads as five
Lane Y's D4 tie-break (one `RankingContainer` key per ability instead of per card) leaked into lane
X's `crackBackFloorSources` walk. Where the opponent controls **one** Lair of the Hydra, the clause
names it once per affordable rung.

Repro — `matchups-20260905-133058/1788633061-ai_baka_deck130-0x55df6f25e960-vs-ai_baka_deck152.jsonl`
seq 17 (also 18, 19), turn 12. The prompt's own opponent battlefield line lists exactly
`Lair of the Hydra [tapped]`, once. The rendered clause:

```
… and that number is a FLOOR, not a ceiling: noncreature permanents of theirs that can animate and
attack are not in that count - Lair of the Hydra, Lair of the Hydra, Lair of the Hydra, Lair of the
Hydra, Lair of the Hydra; triggers on their board add power before damage - Luminarch Aspirant
```

Same shape at `1788633079-ai_baka_deck126-0x55c22a483980-vs-ai_baka_deck152.jsonl` seq 6/7/8 (three
entries, one physical Lair). The count tracks the mana available, not the board. The control case
proves it is not a listing convention: `1788633092-ai_baka_deck126-…-vs-deck146` seq 56–68 lists
`Hive of the Eye Tyrant … , Hive of the Eye Tyrant …` twice against **two** physical Hives (`#1`,
`#2`) — Hive has one activated animate ability, Lair has twenty. A pilot pricing a crack-back reads
five extra attackers that do not exist. Secondary: Lair's entries carry no `(N power once animated)`
figure at all while Hive's do, so the same clause both overstates the count and omits the size.
Fix belongs where the walk enumerates abilities: dedupe by permanent, and take the best affordable
rung for the power figure (which is exactly what D4 made available).

### HIGH-2 — the hold row's rewritten promise is broken 15 times, all at the SAME seam
D13 rescoped the clause to `{taking this row skips every later window that asks THIS SAME question
with rows identical to these; a different question is still asked}` and predicted the surviving
leaks would all be cross-seam. They are not. Measured: after a hold was taken, **19** later windows
in the same turn re-asked a byte-identical `options_text` list; **all 19** are the same record
`kind`, and **15** of them also repeat the same question head verbatim.

Repro — `matchups-20260905-133058/1788633073-ai_baka_deck126-0x555d58890a00-vs-ai_baka_deck125.jsonl`
seq 51 → seq 54, turn unchanged, both `kind: ask`, both headed
`Casting decision (Main phase 1, YOUR turn): which card do you cast now, if any?`, identical row
set, seq 51's answer the Hold row. Others: `…deck130-vs-deck125` seq 11→14 (priority→priority,
`Which action do you take?`), `…deck123-vs-deck162` seq 30→33 and 54→57, `…deck130-vs-deck146`
seq 15→17, `…deck126-vs-deck125` seq 42→44, 43→46, 57→60, 93→95, 114→118. The row now makes a
narrower promise than wave 61's and still fails it; either the latch key must include what the row
text claims it includes, or the clause must stop promising and say what it actually does.

### HIGH-3 — the resolved land-drop verdict names a witness that does not have the named subtype
`landEntersTappedTagResolved`'s witness lookup and its printed subtype label disagree. Of 61
resolved-UNTAPPED clauses, 3 are false; in every mismatch the label is the gate's FIRST selector
while the permanent satisfies the second, or neither.

Repro (worst) — `matchups-20260905-133058/1788633073-ai_baka_deck126-0x555d58890a00-vs-ai_baka_deck125.jsonl`
seq 152:

```
[enters UNTAPPED - it makes mana this turn (you control Isolated Chapel, a Swamp): "Woodland
Cemetery enters tapped unless you control an Swamp or Forest."]
```

`mtg.txt:59410-59415` gives Isolated Chapel **no `subtype=` line at all** — it is neither a Swamp
nor a Forest, and there is no Urborg on that battlefield (checked: Bayou, Forest, Isolated Chapel
×2, Plains, Scrubland ×2, …). The verdict happens to be right (Bayou is `subtype=Swamp Forest`),
but the reason given is false, and a pilot that checks the reason will distrust the verdict.
Second shape — `…deck126-vs-deck146` seq 64: `(you control Savannah, a Swamp)`; Savannah is
`subtype=Forest Plains` (mtg.txt:99480-99482), so it is a legal witness for the *Forest* half and
is mislabelled. Third: the Isolated Chapel clause repeats at the same seat's seq 175. All other 58 clauses check
out against the primitives (Scrubland→Plains, Tundra→Plains, Underground Sea→Island/Swamp,
Bayou→Swamp/Forest, and Urborg→Swamp, which is correct under `lord(land) transforms((swamp))`).
Also cosmetic in the same family: the primitive's own `text=` reads "unless you control **an**
Swamp or Forest" (mtg.txt:135402) and is quoted verbatim.

### HIGH-4 — D19's crack-back relief clause never fires, including in the two windows it was built for
`removes N from the CRACK-BACK total above` occurs **0** times in 2,251 windows. Seven prompts
render both a `CRACK-BACK NEXT TURN` line and a `DIES` row; two of those seven have the exact
shape the lane wired (a DIES verdict on an OPPONENT-controlled body, on a damage-verdict row), and
neither carries the clause.

Repro — `matchups-20260905-133058/1788633087-ai_baka_deck125-0x558abfb7e100-vs-ai_baka_deck146.jsonl`
seq 15, turn 14, `Phase: Main phase 1 | It is your turn.`, life 15:

```
CRACK-BACK NEXT TURN: 3 of their creatures will be able to attack (tapped ones untap first), for up
to 9 from combat as their board stands - you would be at 6 or lower - and that number is a FLOOR,
not a ceiling: …
3. Deal 1 damage with Staff of Nin targeting Goblin [opponent's battlefield] {right now: takes 1
damage - DIES} [cost: Tap] …
```

Row 3 removes a body from the 9. Nothing on the screen says so; the seat took row 4 (the face).
Second instance: `…deck126-vs-deck146` seq 38, turn 20 (Staff of Nin #2 → Triumphant Adventurer,
DIES, crack-back 7). `crackBackNextTurnDue` is the obvious suspect — the line is demonstrably on
the screen in both records, so whatever the gate reads is not what the render used.

### HIGH-5 — the ANNOUNCE_X Decline row is state-dependent and vanishes once the mana is committed
D5 and R1 both predict a Decline row on every X menu. 17 of 18 have it; one does not.

Repro — `matchups-20260905-133058/1788633087-ai_baka_deck125-0x558abfb7e100-vs-ai_baka_deck146.jsonl`
seq 12, turn 12, Sphinx's Revelation:

```
1. X = 3 {X pricing: X=3 - you gain 3 life and draw 3 cards} {leaves 0 of your 6 floating mana …}
2. X = 2 …   3. X = 1 …   4. X = 0 {X pricing: X=0 - this cast does NOTHING …}
… write CHOICE: followed by the number of your choice (this ask has no pass row) …
```

The **same seat, same card**, seq 7 (turn 8) rendered the Decline row and the
`(the LAST row of this menu declines: …)` format line. The difference visible in the record is
that seq 12's `events` show the mana already spent when the menu armed
(`- Paid {u}{u}{w}{x} for Sphinx's Revelation with Plains #1; Glacial Fortress #1; …`), so
`object->checkCantCancel()` returned true and both `req.canDecline` and the format line correctly
followed it. The contract and the render agree; the *engine* withdrew the Cancel row. Note the
render is honest here — this is not a false surface — but the lanes' premise ("a human has always
been able to back out of the announcement") does not hold on this route, and the D5 repro shape
(a burned card with no way out) is still reachable.

### HIGH-6 — the `choice` field is a constant 1 on every `reveal` record, and it fires a false parse stamp
All **16** `reveal` records log `choice: 1` regardless of which row ran. Repro —
`matchups-20260905-133058/1788633092-ai_baka_deck126-0x55591d8b8040-vs-ai_baka_deck146.jsonl`
seq 66: 26 options, reply `CHOICE: 16 (Cast Sanguine Bond)`, `chosen_text` "Sanguine Bond" (row 16),
`events` confirm the search took Sanguine Bond — and `choice: 1`, plus a
`parse_note: latched_row_mismatch` that is a false positive. The other 15 show the same constant
(true indices 4, 13, 14, 16, 17, 18, 22, 23, 24, 26, 28, 29, 42, and two small menus), all with no
note. Two costs: the reveal seam raises a spurious mismatch stamp, and **any** prediction
adjudicated by the `choice` field is silently wrong on 16 records — D10's own falsifier is written
in exactly those terms.

### HIGH-7 — the reply protocol is absent from every logged `prompt`, so D9(c) cannot be adjudicated
Every ask's format line ends `… then a PLAN: line only if the reply rules call for one`, but the
reply rules themselves appear nowhere: `The LAST answer line you write is the one that runs` **0**
occurrences, `An answer written inside your PLAN sentence` **0**, `answer line` **0**, `change your
mind` **0**, in all 2,251 prompts. Nor is a system prompt logged (the `gamestart` record carries
only `game_id/model/deck names/seq`). The brief states `prompt` is "the full per-decision message
the model saw"; for the block D9(c) edited, and for the deck guide and general guide, it is not.
Until `kReplyProtocol` (or a hash of it) is written into the translog, every protocol-wording
change ships un-reviewable — this is the third wave in a row whose protocol claim is taken on
trust.

### MED-8 — the `DRAW PUNISHERS` board paragraph still carries D3's direction error
Lane Y fixed the forecast lines and said it left `drawPunisherSummaryText` alone; the error is
still there, on the same screen as the fixed line. Repro —
`matchups-20260905-133058/1788633102-ai_baka_deck162-0x562b0bc28020-vs-ai_baka_deck126.jsonl`
seq 13. The forecast line correctly reads `LOOP SCOPE: they control BOTH halves … and THIS number
does not enter it`. Eight lines up, the paragraph says of the same punishers:

```
LOOP CAUTION: they control BOTH halves of a life LOOP (Sanguine Bond + Exquisite Blood) … While the
pair stands, a number above that reads as life TO you is not a safe gain, and any life you pay is
fatal rather than merely expensive.
```

"a number above that reads as life TO you" points at the paragraph's own
`costs them 3 life to yours` and at the forecast's `3 life to you from your punishers above` — both
of which are life **they** lose, which (per lane Y's own probes) enters nothing. All 4 surviving
`LOOP CAUTION` strings in the corpus are this paragraph.

### MED-9 — "N life to you from your punishers above" reads as life gained, and no punisher gains any
Related to MED-8 but separable, and it is the wording D8 shipped. Underworld Dreams, Fate Unraveler
and Ob Nixilis, the Hate-Twisted are all `damage:N` to the drawing player; none gives the pilot a
point of life. The forecast renders `= 1 x 3 = 3 life to you from your punishers above` (15
occurrences of that phrasing, `…deck162-vs-deck126` seq 13–16) and the own-side mirror renders
`= N life to the punishers above`. Both should say the life is **lost by the drawer**, not "to you".

### MED-10 — the animate clause prices nothing
The 53 D6b clauses name the manland but print no activation cost, no colour requirement and no
statement of whether the opponent can pay it next turn. R5's whole fix is invisible on the page,
and a reviewer cannot adjudicate it (see R5 above). One extra number — the cost the gate already
computed — would make both the clause and its prediction checkable.

### LOW-11 — Lair of the Hydra's own rung list stops one short of affordable
`1788633079-ai_baka_deck152-0x55c22914cfb0-vs-ai_baka_deck126.jsonl` seq 15: `Mana available: 5
total (five untapped sources …)`, rows offered are `{1}{g}`, `{2}{g}`, `{3}{g}` — no `{4}{g}` 4/4,
which 5 sources pays for. Plausibly a deliberate self-mana exclusion (animating the Lair while
tapping it for mana would leave it tapped), in which case it is correct and only wants a word on
the row; flagged so the next lane decides which.

### LOW-12 — the repeat-row grind is a real window cost
`1788633085-ai_baka_deck123-0x557b7b88b130-vs-ai_baka_deck126.jsonl` turn 14 spends **29** decision
windows on one seat, ending at
`[repeat: you control 59 creatures; activated this turn 57 times already]` (seq 58). The counted
repeat row exists to collapse exactly this; it was offered and not taken. Guide territory, noted
here only because it is the corpus's largest single-turn window spend.

---

## 4. Lategame specimen

`wave62/lategame-specimen.txt` — `1788633073-ai_baka_deck125-0x555d57578430-vs-ai_baka_deck126.jsonl`
seq 324, turn 61, the last decision of the corpus's longest game; the seat took row 8 (Staff of Nin
→ opponent, `{THIS WINS THE GAME}`) and won 52–0. 31,506 bytes, 11 rows, 27 permanents against 31.

---

## 5. What I did NOT check

* **No wagic was run and no source was read for causation.** Every mechanism sentence above is
  inferred from the rendered prompt plus the primitives; HIGH-1's "one entry per ability" and
  HIGH-5's "the engine withdrew the Cancel row" are the corpus's shape, not a traced call path.
* **The hang was not re-diagnosed.** `corpus-hang.md` is taken as given; I only corroborated its
  stderr signature and the rerun's 2-add outcome.
* **Per-deck play quality, guides, and misplay attribution** — the deck seats' job, not touched.
  Where a seat's decision is named above it is only as evidence that a surface was missing.
* **Oracle verification was done for the two cards a verdict turned on** (Hive of the Eye Tyrant,
  Lair of the Hydra — Scryfall, 2026-09-05, both matching the primitive). Every other card fact
  above is checked against the primitive `text=`/`subtype=`/`auto=` only, not against Scryfall.
* **The `options`/`options_text` arrays were used only to compare two windows' row sets** (HIGH-2)
  and to recover a true row index (HIGH-6); every count is from the rendered `prompt`.
* **Latency, cost and model behaviour** beyond the fallback/deadline census. I did not read replies
  except where a prediction is about the reply (D9, D10).
* **R5, R6, R7/R8 and D6a are UNTESTED because no window arose**, not because they were skipped;
  none of the four can be closed from this corpus and each needs a constructed board or a driven
  transport failure.
