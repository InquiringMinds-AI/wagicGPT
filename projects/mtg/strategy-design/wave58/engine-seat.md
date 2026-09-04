# Wave-58 engine-narration seat

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-015059/` (binary
`archives/wagic-a3f0700d3-w58step1b` = master a3f0700d3). Rerun of the hung matchup:
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-042235/`. All counts below are from the rendered
`prompt` text of the 42 seat translogs (brace-balanced extraction of each `{right now: ...}` /
`{dead right now: ...}` group, scopes split on `;` at depth 0, parenthetical qualifiers stripped),
not from the `options` array. Script: session scratchpad `w58/{lib,verd}.py`.

---

## 1. Corpus census

| | |
|---|---|
| games | 21 attempted — **20 natural**, **1 HUNG** (deck152 vs deck126, F2); rerun of that matchup finished naturally (turn 18, deck126 won 25-0) |
| SEGV / SIGABRT / assertion / core dump | **0** in all 21 stderr files (F1 did not recur on master) |
| seat translogs | 42 (2 per game); 40 carry a `gameend` record (the 2 hung seats do not) |
| records | 2457 total; **2270 decision records** carrying a `prompt` |
| kinds | ask 1757, priority 347, recovery 105, attackers 72, discard 48, gamestart 42, gameend 40, reveal 19, blockers 16, wall_miss 7, bottom 4 |
| latency | n=2270, median 11.5 s, p90 63.7 s, max 963.8 s |
| **fallbacks** | **108** (4.8% of asks): `empty_reply` **89**, `named_row_reask` 10, `wall_miss_unrecorded` 7, `unparsed_reply` 2 |
| fallbacks by kind | ask 71, priority 20, wall_miss 7, attackers 5, reveal 2, discard 3, blockers 1 |
| recovery records | 105 (every fallback except 3 got a paired recovery record) |
| wall misses | 8 events, 7 unrecorded; 5 of the 6 longest latencies are wall_miss at 900 020-900 037 ms (the 900 s GPT deadline) |
| timeouts | 0 records classed `timeout`; 0 `stale_livelock`; 0 `reasoning_only`; 0 `abandoned_in_flight` |
| **stale async drops** | **60**, all stamped (lane C): `land-drop/question (or turn/phase)/re-asked` 28, `casting/question (or turn/phase)/re-asked` 22, `casting/question and board/re-asked` 9, `casting/board/re-asked` 1. Outcome is `re-asked` on **60 of 60**; `gave-up-to-heuristic` never occurred. stderr carries 60 `dropping stale async answer` lines, **60 of 60 with `[outcome: ...]`** |
| HOLD | rendered on **1524** windows; **taken 271** times (prefix-match of `chosen_text` against the row body: ask 172, priority 99); `hold_windows_skipped` sums to **1053** across the 40 gameends |
| parse notes | `stale_echo_in_range` 11 → `named_row_reask_recovered` 11 (paired), `echo_index_conflict;name_over_index` 1 |
| answer replaced / retried | `answer_replaced` 8, `retry` 1, `dropped_assignments` 1, `latched_coded_line` 25 |
| `post_answer_overrun` | > 0 chars on **1114 of 2270** windows (49%); 18 windows over 3000 chars of post-answer prose |
| `land-drop ask NOT issued` | 861 stderr lines (windows correctly suppressed) |

### What the translogs add about F2 (the hang)

The deck152 seat's last three records are **consecutive `empty_reply` fallbacks**: seq 26
(`attackers`, turn 11), seq 28 (`reveal`, Blockers), seq 30 (`ask`, Blockers) — latencies 2503 /
2502 / 2542 ms, replies literally `""`. The seat had just resolved Sigarda's Coven attack trigger
chain (seq 25 declares Sigarda as an attacker; the reveal at seq 28 is the trigger's chooser).
So the hang did not begin with the `-1` menu answer — it began with the **endpoint returning empty
bodies**, and the `-1` on "Choose an option for Sigarda, Champion of Light" is the third
consecutive heuristic hand-off, on the trigger's own follow-up chooser. Every `empty_reply` in the
corpus has this same 2.5 s signature (below), so F2's precondition is reproducible independently
of Sigarda: **a run of transport failures landing on a may-trigger's chooser**. The deck126 seat
stops at seq 12 (turn 10, its own main) with no fallback and no error — it was simply never asked
again. No `only display-toggle` livelock signature and no watchdog dump (WAGIC_HANG_GUARD was off).

---

## 2. Prediction adjudication

### Lane A — D3 (`rowSaysNoOp` magnitude parser)

**PASS, 42 of 42.** Every `{right now: exiles 0 of their creatures (0 without a restriction
against attacking), 0 of yours}` render (42) is classified dead: 37 sit under the
`NO LIVE CAST ROW ON THIS MENU` header, and the remaining 5 sit on menus that also carry a cast
row with **no** `{right now:}` verdict at all (Elixir of Immortality, Sphinx's Revelation), where
the header is correctly silent — e.g. `1788504701-...deck125-...-vs-deck123.jsonl` seq 70.
The **negative holds**: `exiles 1/2/3 of their creatures` (6 renders) and
`exiles 0 of their creatures ..., 1 of yours` (3 renders, `...deck125-0x55ac9159eb90...` seq 116 /
118) are all **live** — never badged dead, never under the header.
Cleanup-discard rebadging fires: **12** `{dead right now: exiles 0 ...}` renders. (But see HIGH-1 —
the discard channel applies the predicate to the whole verdict string, not per scope.)

### Lane B — D1 (life-payment subtraction)

**PASS, 3 of 3.** Three true `pay N life` option rows rendered in the corpus, each carrying
`{this payment puts you at N}`: `...deck146-0x5654d30b6220-vs-deck162` seq 21 (life 21 → 18,
**taken**), `...deck146-0x558a81c2d360-vs-deck125` seq 28 (20 → 17, declined),
`...deck146-0x5595223952f0-vs-deck130` seq 56 (19 → 16, declined). **Lethal takes: 0 of 0** — the
`{you are at L life: paying C puts you at N and you LOSE the game}` form **never rendered**, so
that half is **UNTESTED** (no window arose at `my_life <= cost`).

### Lane B — D5 (trade marker uses both sides)

**PASS on the priced form, 3 of 3; the endorsed form is UNTESTED.**
`[<- best trade: ...]` rendered **0** times, so "renders where YOURS > THEIRS: 0 of N" is vacuously
true and untested. The lopsided **priced** form rendered 3 times, all inside the D5 sibling
`[<- best X for this cast: X=5 - the only X that kills anything of THEIRS costs you more of YOURS
than it takes - THEIRS 3 / YOURS 4]` (`...deck130-0x5569cb670ad0-vs-deck162` seq 111/112/114,
Starstorm, turn 16) — `YOURS 4 > THEIRS 3` as required, and **not taken** in any of the three
(the seat chose "Cast nothing right now"). Other X markers: 207 `best X for this cast` (largest
affordable X), 32 `no X on this menu kills anything of THEIRS`, 8 free-kill forms.

### Lane B — D7 (assignable bound is a true floor)

**PASS on the observable half.** `- best case with every blocker assigned:` rendered 10 times;
the floor form `- at least D of that lands ... you would be at N or better` 6 times; the legacy
`one legal assignment gets you to` **0** times (superseded, as designed). `; no block saves you`
rendered **once**: `1788504699-ai_baka_deck123-0x562a1171ba80-vs-deck162.jsonl` seq 24, turn 13,
Blockers, at 1 life vs `INCOMING THIS COMBAT: 2 attackers, 8 unblocked damage - you would be at
-7; this KILLS you ... - best case with every blocker assigned: you would be at -2; no block saves
you`. The seat's `gameend` two records later: `my_life -7, won false, turn 13`. **The badge sat on
a window the seat did die to — 1 of 1.** I did **not** independently re-derive each of the 10
`best case` numbers from the board frame, so "optimistic against the true floor: 0 of N" is
adjudicated only by the absence of a badge that the outcome contradicted (N=1).

### Lane C — D2 (false `NO LIVE CAST ROW` header)

**PASS, 0 of 316, both directions.** 316 windows render the header. Under **not one** of them does
any cast row's `{right now:}` name a live magnitude in **any** scope (0 of 316) — the wave-57
`123v130` s45 / Tragic Slip classes are gone from this channel. The converse also holds: of 371
cast menus where every cast row carries a verdict, **0** are all-dead-but-headerless. Header volume
is 316 renders / 371 fully-priced menus.

### Lane C — D4 (drop outcome + `async_drops` record)

**PASS, 60 of 60, both channels.** Every stderr `dropping stale async answer` line carries
`[outcome: ...]` (60/60) and every drop is stamped on the next written record
(`async_drops` totals 60 across 55 records; `async_drop_events` present on all 55). The
byte-identical wave-57 prefix and arm parenthetical are preserved.
**The count fell: 60 drops here vs 108 in wave 57** (a 44% drop that lane C explicitly did not
predict — 21 games vs wave 57's 32 explains most of it; per game 2.9 vs 3.4). A **new drift kind
appeared that the wave-57 census did not have**: `casting/board/re-asked` (1) — board-only drift,
which lane C's D4(a) argument had assumed was never the sole cause. n=1, so it is a shape
observation, not a rate.

### Lane C — D9 (`INCOMING THIS COMBAT` gated on the opponent's turn)

**PASS, 0 of 10 — thin.** Opponent-turn windows from upkeep through end with ≥ 1 creature able to
attack: **10**, and **all 10** carry an `INCOMING THIS COMBAT` line (0 missing, against wave 57's
20 missing). The forecast form rendered 10 times, the declared form 30, `they declared no
attackers` 24 — 64 renders total. **Negative holds:** 0 `not declared yet` lines at or past the
opponent's second main. Caveat: the `able to attack` gate is read from the board frame's own
`N of them able to attack right now` string, so this adjudicates the render against the render.

### Lane D — D45 (lower-cased script token in narration)

**PASS, 0 occurrences.** Over all 2270 prompts, `- You/Opponent used: <label> with <Card>` lines
whose `<label>` is a `name=` entry in `bin/Res/sets/primitives/*.txt` **and** starts lower case:
**0** (wave 57: 708). Printed-case card-name labels: **2013** across the Pathway/MDFC decks —
`Mistgate Pathway` 564, `Boulderloft Pathway` 432, `Tidechannel Pathway` 371, `Grimclimb Pathway`
334, `Pelakka Caverns` 156, `Agadeem, the Undercrypt` 115, `Emeria, Shattered Skyclave` 9.
Lower-case **non**-card labels unchanged in character: **38 distinct / 7183 occurrences**
(`cycling` 1007, `choose a land` 698, `choose a new dungeon` 619, `goblin lair` 402, …) — up from
wave 57's 30/9261 in distinct-label count, none of them a card name.

### Lane E — G1 / G2 / the three-state setting

**UNTESTED, all three.** This corpus is `WAGIC_HEADLESS=1` AI-vs-AI: `GuiPlay`, `CardGui::Render`,
`DuelLayers::Render` and the option menu never run, `WAGIC_BOARDGROUP_CHURN` was not set, and no
translog carries a render field. No window arose. (Lane E's own "What I did NOT verify" already
says nobody has seen the badge; nothing in this corpus changes that.)

### wave-57 D22 — lane V's four blocker predictions

**UNTESTED, all four — and the instrument produced nothing.** See HIGH-2. `W57V_BLOCK` appears
**0** times across all 21 stderr files, although the literal *is* present in the archived binary
(`strings archives/wagic-a3f0700d3-w58step1b | grep -c W57V_BLOCK` = 1), i.e. the block is
compiled in and was gated only by `getenv("WAGIC_BLOCKLOG")`. `tools/selfplay-harness.sh` line 191
builds the child environment with `env <explicit vars> ./wagic` and **never sets or exports
`WAGIC_BLOCKLOG`** — there is no line anywhere in the repo that does. So either the flag was not in
the operator's shell, or the path was never reached; the two are indistinguishable from the
artefacts. Separately, D22 was **not reachable in this corpus by construction anyway**: all 16
`blockers` windows were decided by the GPT seat's bundled-blockers override (which D22 excludes),
and the single fallback to `AIPlayerBaka::chooseBlockers`
(`...deck123-0x562a1171ba80-vs-deck162` seq 24) was the "no block saves you" window. **D22 needs a
Baka-vs-Baka run with the flag set in the harness, not a GPT corpus.**

---

## 3. Engine / render / narration defects

### HIGH-1 — the cleanup-discard `{dead right now: ...}` badge still uses the whole-string predicate: 11 FALSE "dead" verdicts on cards that kill a real creature

Lane C moved the *header* to the per-scope `verdictReadsZero` and explicitly left "D2's
discard-menu half" to lane A; lane A widened what `rowSaysNoOp` *recognises* but not what it is
*applied to*. Result: the exact two shapes lane C's D2 write-up names as the defect are still live
in the discard channel. **139 rows carry a dead badge; 11 of them name a live magnitude** in some
scope, with no `0 legal targets on the board for it` escape on the row.

Repro (`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-015059/`):
```
1788504668-ai_baka_deck123-0x557bd4563d70-vs-ai_baka_deck126.jsonl  seq 12  turn 6  (discard)
6. Devour Flesh {1}{b} (instant) {card text: Target player sacrifices a creature, then gains life
equal to that creature's toughness.} {dead right now: they control 1 creature - Overgrown
Battlement (0/4) [defender] is sacrificed, they gain 4 - the sacrificing player gains, not you;
YOU control 0 creatures - targeting yourself does nothing}
```
```
1788504697-ai_baka_deck123-0x560907371270-vs-ai_baka_deck130.jsonl  seq 17  turn 19  (discard)
5. Tragic Slip {b} (instant) {card text: Target creature gets -1/-1 until end of turn. -- Morbid -
that creature gets -13/-13 instead if a creature died this turn.}
{dead right now: -1/-1 (no creature has died this turn, so Morbid does NOT apply)}
```
Distribution of the 11: Devour Flesh naming a real opponent creature 8 (Overgrown Battlement x2,
Dwarven Blastminer x2, Silverquill Silencer x1, "they control 2 creatures - they choose which one"
x3), Tragic Slip's live `-1/-1` behind a Morbid qualifier 3
(`...deck123-...-vs-deck126` seq 25; `...deck123-...-vs-deck130` seq 17, 18).
Both cards verified against their primitives — `Devour Flesh` / `Tragic Slip` in
`bin/Res/sets/primitives/*.txt` match the printed Oracle text, so the card scripts are right and
the **verdict renderer is the liar**. This is a false surface at a discard decision: the seat is
told a removal spell it is about to pitch does nothing.
**Fix shape:** call `AIPlayerGPT::verdictReadsZero` (lane C's public pure helper) at the
cleanup-discard rebadge site instead of `rowSaysNoOp`, and pin the two shapes above in PARSETEST.

### HIGH-2 — 89 `empty_reply` fallbacks are a deterministic transport fault with a 2.5 s signature, and the engine writes NOTHING to stderr about them

All **89** `empty_reply` records have `reply == ""` and a latency in **2502–2558 ms** (median 2510;
non-fallback median 12 692 ms) — a near-constant cost that cannot be model variance. They arrive in
**runs**: 14 singletons, but also runs of 3, 4, 5, 6, 8×3, 9 and **10 consecutive**
(`1788504678-ai_baka_deck152-0x55dea0e980f0-vs-ai_baka_deck162.jsonl` seq 32-50, ten straight
decisions surrendered to the heuristic). Repro of the run head:
```
1788504678-ai_baka_deck152-...-vs-ai_baka_deck162.jsonl seq 32  latency_ms 2508
  fallback "empty_reply"  reply ""  chosen_text "<refused: empty_reply>"
```
stderr for those games contains **no HTTP status, no URL retry, no body-length line, nothing** —
`grep -i 'http\|curl\|refused\|reset\|empty'` over all 21 stderr files returns only the 42
`using http://... (model ...)` banners. `noAnswerClassFor`'s audit-L status table exists but the
0/200 fall-through swallows this case, so a reviewer reading a translog cannot tell "endpoint
refused the connection" from "the model returned an empty body". **This is the corpus's single
biggest silent loss: 3.9% of all decisions, clustered, and it is the mechanism that precedes F2.**
**Ask:** log the HTTP status / transport errno and the elapsed time at the empty-body seam, and
give the ~2.5 s constant-cost class its own name so it stops sharing a word with a live model that
answered nothing.

### HIGH-3 — F2's precondition is a fallback run, not a card: 3 consecutive `empty_reply` refusals immediately precede the hang

Covered in §1. The engine-side consequence is the one to fix: a may-trigger's follow-up chooser
(`Choose an option for Sigarda, Champion of Light`) received `-1` from the heuristic path **with no
translog record written at all** — the seat's record stream simply ends at seq 30. Every other
heuristic hand-off in the corpus writes a record. **Ask:** the `-1` menu answer must write a
record (kind + `fallback` + the options it declined) so the last decision before a hang is
visible, and `chooseMenuAction` returning `-1` on an AI-owned chooser must be a state the
orphan-chooser breaker can see. The matchup reran clean, so this is a rare interleaving, not a
deterministic softlock.

### MED-4 — the X-pricing block enumerates X values the marker does not, so its own recommendation is unpriced

`...deck130-0x5569cb670ad0-vs-ai_baka_deck162.jsonl` seq 111 (turn 16), Starstorm:
```
{X pricing: max affordable X=6 (8 mana total); smallest X that kills an OPPONENT creature: X=4
(6 mana total) - kills THEIRS: Fate Unraveler; YOURS: Siege-Gang Commander, Goblin x3. A smaller
X=1 (3 mana total) kills only YOURS: Goblin x3. At X=6 - kills THEIRS: Shield Sphere x2, Fate
Unraveler; YOURS: Siege-Gang Commander, Goblin x3}
[<- best X for this cast: X=5 - the only X that kills anything of THEIRS costs you more of YOURS
than it takes - THEIRS 3 / YOURS 4]
```
The block prices X=1, X=4, X=6. The marker names **X=5**, which appears nowhere in the block — the
model is told which X is best and shown three other X's. Cheap fix: always emit the marker's own X
into the pricing block.

### MED-5 — `stale_echo_in_range`: 11 stale echoes landed on a VALID index and were only caught by the name check

11 windows carry `parse_note: stale_echo_in_range`, each followed on the next record by
`named_row_reask_recovered` and a `named_row_reask` fallback — i.e. the model echoed a *previous*
window's row number, that number was still in range on the new menu, and only the SHORT-NAME half
of the reply protocol prevented the engine from committing the wrong row. 6 of the 11 are in one
game (`1788504668-ai_baka_deck123-0x557bd4563d70-vs-ai_baka_deck126.jsonl` seq 8/14/19/22/26/29).
Each costs a full extra round trip. The name check is doing real safety work here and should be
recorded as such; the residual risk is any window where the stale name *also* matches.

### MED-6 — the MDFC land-drop row forecasts a life payment without pricing it

7 renders of the form
`{taking this row then ASKS you to pay 3 life: pay and it enters UNTAPPED..., decline and it
enters TAPPED}` (e.g. `1788504670-ai_baka_deck146-0x5654d30b6220-vs-ai_baka_deck162.jsonl` seq 20,
`my_life` 21). D1's subtraction is applied only at the follow-up ask. The amount and `my_life` are
both known at the land-drop window, so the same `{this payment puts you at N}` clause could ride
the forecast — and at low life the seat would see the lethal half one decision earlier, where it
still has the option not to play the land at all.

### MED-7 — 49% of replies write prose past the answer

`post_answer_overrun > 0` on 1114 of 2270 windows; 869 of those are under 200 chars, but 18 exceed
3000. Not a correctness defect (the parse is unaffected) but it is paid inference on every window,
and it is measurable: the biggest offenders cluster with the 963.8 s outlier
(`1788504701-ai_baka_deck123-0x55a05fcbc480...` seq 47).

### LOW-8 — `dropped_assignments` / unusable blocker reply, 1 occurrence

`AIPlayerGPT: declared blocks from N assignment(s) - unusable reply, heuristic declares` appears
once in stderr with a matching `dropped_assignments` field. Named for the ledger; n=1.

---

## 4. Lategame specimen

`wave58/lategame-specimen.txt` — `1788504664-ai_baka_deck126-0x55f8e7326c20-vs-ai_baka_deck146.jsonl`
seq 48, turn 30, Main phase 1 (its own turn), `my_life` 1 vs 20, 18 832 chars. The seat's last real
cast window before dying on turn 31; it tutored Sanguine Bond one decision earlier and the menu now
badges that row **self-lethal** because the opponent's Silverquill Silencer named it
(`casting this costs you 3 life and draws them a card - you would be at -2; this KILLS you`). Both
`Silverquill Silencer` and `Perimeter Captain` were verified line-for-line against
`bin/Res/sets/primitives/` and match their Oracle text. The seat took the 0/4 defender.

---

## What I did NOT check

- **I did not re-derive the 10 `best case with every blocker assigned` numbers from each board
  frame.** D7's "optimistic against the true floor: 0 of N" is adjudicated only by the single
  `no block saves you` badge whose outcome I could check (1 of 1), not by arithmetic on 10 boards.
- **D9's able-to-attack gate is read from the prompt's own board frame**, so the adjudication
  compares one render against another; it does not prove the engine's attacker count is right.
- **I did not run anything.** No build, no game, no probe, no live model call; nothing under
  `src/`, `bin/Res/` or git was touched.
- **Lane E is unadjudicable from this corpus and I made no attempt to run its probes** (they need a
  display and a dev build).
- **I did not determine whether `WAGIC_BLOCKLOG` was actually exported** for the corpus run — only
  that no repo file sets it and that 0 lines were produced.
- **The 11 false dead-badges: I did not check whether any of them changed a discard.** The census
  is of the rendered surface, not of the decision it produced.
- **I read only the corpus and the lane briefs**, not the per-deck seats' reviews (they run in
  parallel), so anything a single-deck reader found is not de-duplicated here.
- **Card-fact verification was limited to the four cards I cite by name** (Devour Flesh, Tragic
  Slip, Silverquill Silencer, Perimeter Captain), against the primitives only — no Scryfall call
  was made, so no disagreement could be reported.

## Orchestrator addendum — D22 adjudicated on a heuristic-vs-heuristic run (2026-09-04 ~06:30)
The GPT-vs-GPT corpus cannot test lane V (every blocker window used the bundled GPT override), so
lane V's own instrument was re-run on the CURRENT binary (`archives/wagic-a3f0700d3-w58step1b`):
lane V's ten fixed Baka-vs-Baka matchups (`~/.gatelogs/w58V-ab/run.sh`, derived from
`~/.gatelogs/w57V/ab/run.sh`; `WAGIC_SELFPLAY_ONESHOT`, `WAGIC_AI=baka`, `WAGIC_BLOCKLOG=1`), 10
games, 66 declared blocks (53 `stage=value`, 13 survival sweep, all gang=1).
- Prediction 1 (value block loses the blocker, no attacker death, NOT facing lethal): **PASS, 0 of 53**
  — the 6 value-pass chumps all read `life=1 incoming=16` (lethal; allowed by the prediction).
- Prediction 2 (value declaration `spent > worth`): **PASS, 0 of 53** — the 7 overspends are all
  survival-sweep blocks at lethal, which the prediction exempts.
- Prediction 3 (no value-pass gangs): **PASS, 0 of 53** (every block gang=1).
- Prediction 4 (first-strike attacker blocked only by a body that survives/kills): **UNTESTED** — no
  first-strike attacker was declared against in these 10 games (not counted; say so).
Caveat: 10 games, categorical claims only; the counter (`count.py`) does NOT apply the not-facing-
lethal condition itself — the six lines were read by hand. Raw lines in `~/.gatelogs/w58V-ab/W58-*.err`.
