# Wave-57 lane D — prompt economy (D14, D26, D27, D28, D29) + the D13 measurement

Branch `w57-lane-D`, worktree `worktrees/lanes/w57-D`, base master `4d4ea0bdf` (wave-57 lane brief;
src = wave-56 close `7ee3a51e8` + hotfix lanes). Docket = `wave56/engine-ledger.md`. Corpus read
(read-only, python over JSONL): `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-120952/`.

Everything here is `src/AIPlayerGPT.cpp` + `include/AIPlayerGPT.h` + one `src/TestSuiteAI.cpp`
assert command and one fixture. **No card script and no engine rule was touched**; the engine's
option vector, `req.optionTexts` (the staleness key) and every answer INDEX are unchanged on both
seams, which is what lets D14 shrink a shown list without moving a click.

---

## D14 — the `Flip Side` display toggle leaves the AI seat's option list

**Record re-derived (not taken on faith).** 109 rows in 90 windows, 0 takes; by kind **87 priority
rows + 22 `ask` rows**. New census this lane: **108 of the 109 are LAND-backed** (the row says
"modal double-faced land" / "other face is a LAND"); exactly **1** is the plain spell-face form.

**Shipped, and the scope is the item.** The toggle is dropped where — and only where — the card's
OTHER face is a LAND, i.e. exactly where #W56-D (D8) put a real one-click row for that face in the
same window:

* **priority seam** (`chooseOrderedAction`) — dropped at the CANDIDATE level, before
  `describeAction`, so the ~650-character row is never BUILT. This is where the docket's *"760
  option sets built and discarded"* lives. When the toggle was the window's only candidate the
  window auto-passes with `return NULL`, byte-for-byte the behaviour the N-152b gate already gave
  it (no model call, and deliberately **not** a fall-through to Baka).
* **`ask` seam** (`chooseMenuAction`) — a filtered SHOWN list plus a `shownToFull` index map, applied
  only when the D8 back-face land row is on that same menu (`mdfcLandRowShown`), and only while at
  least two rows survive. The engine's `opts` / `menuIndices` / `optionTexts` are untouched and the
  model's pick is mapped back before anything is clicked; the decline still resolves against the
  FULL count.

**Why not all toggles.** `borderline.txt` still carries 30
`otherrestriction=...,compare(isflipped)~equalto~1` gates on the modal-DFC **SPELL** back faces
(D33, not fixed), and `isFlipped` is set in exactly one place: this toggle. For a spell back face the
toggle is therefore the **only route to a legal cast**, so suppressing it would delete a play — the
one thing the doctrine forbids. The docket's ask ("suppress the `doubleside` ability") is therefore
implemented as *"suppress it where the play it duplicates is already offered"*: it removes 108 of the
109 rows and strands nothing. **This is a deliberate deviation from the docket's literal ask and from
its `0 of N` prediction; the residue is the spell-face class, and it closes when D33 lands.**

**A false claim found by the live probe and fixed (seq 10).** A modal-DFC SPELL whose other face is a
LAND (Pelakka Predation // Pelakka Caverns) reaches the CAST-MODE menu with no `Play Land` row and no
`backSide` field, so its toggle fell through to the generic tail — which told the model *"the Cast
menu ... offers the other face as an alternative-cost cast"*. A land is never cast, at any cost. That
menu carries no land row to duplicate, so the toggle is still OFFERED there; its tail now says what is
true and points at the row that really plays that face. Same species as the three Flip-Side texts
#W56-D had to correct one wave at a time.

## D26 — the back-face land row says what the land will ASK on arrival

`mdfcLandArrivalClass` reads the BACK FACE's own script and is three-valued, so the clause is only
made where it is true; `mdfcLandArrivalTag` renders it in the braced channel (registered in
`stripNarrationDecoration`, so it never enters history). Both emitters take it through the existing
`mdfcLandPlayRowTag`, so they cannot drift.

* pay-or-tap (`choice name(Pay N life) ... choice name(Tap) tap(noevent)`) →
  `{taking this row then ASKS you to pay 3 life: pay and it enters UNTAPPED and makes mana this turn,
  decline and it enters TAPPED}`
* unconditional (`tap(noevent)`, no choice) →
  `{it enters TAPPED - it makes no mana this turn unless something untaps it, and no menu follows}`
* neither (the Pathways) → nothing.

**Docket correction.** D26's record lists **Pelakka Caverns** with Emeria and Agadeem as a card whose
take "answers a follow-up menu". It does not: its script is a bare `tap(noevent)` and its Oracle text
(Scryfall, verified 2026-09-03) is *"This land enters tapped"* — no menu, no payment. Agadeem, the
Undercrypt / Emeria, Shattered Skyclave / Sea Gate, Reborn / Shatterskull read *"As this land enters,
you may pay 3 life. If you don't, it enters tapped."* The unconditional clause exists because leaving
that class silent would leave exactly the wrong inference (an untapped arrival) unanswered — the same
defect D26 is about.

## D27 — `[NAMED BY THEIR <src>: …]` says per-cast, not one-off

The price is read off `@movedto(*[chosenname]|opponentstack)`, a standing triggered ability on THEIR
permanent, so it is charged on every cast of a card with that name for as long as that permanent is on
the battlefield. Oracle-verified (Scryfall, 2026-09-03) — Silverquill Silencer: *"Whenever an opponent
casts a spell with the chosen name, they lose 3 life and you draw a card."* The tag now closes with
`. This price is PER CAST, not one-off: it is charged again every time you cast a card with that name
while their <src> is on the battlefield, so declining now does not make it go away`. One clause,
inside the same bracket, so it stays out of history.

## D28 — the row leads with the printed name, the token second

`mdfcRowHead(printedName, menuToken)` feeds BOTH emitters. The engine's menu text for this row is the
back face's lowercased script token, and it led the row and then rode `chosen_text` into every
re-rendered narration line (`152v162`: 54 renders from one take). The head is now the collection's
printed name; a genuinely different token (e.g. the `backside` placeholder) is kept second as
`(menu text: <token>)` so a token-only echo still binds. When the token is only a **case variant** of
the printed name — the live case — it is not printed twice: `parseChoice` binds case-blind, and the
PARSETEST echo cases prove both the printed name and the lowercased token still answer the row.

## D29 — adjacent duplicate log lines fold into one counted line

Census reproduced exactly: **1,767 excess lines in 670 records**, run lengths **1,467 × 2 and
150 × 3**, top class `- Opponent drew a card` **× 1,348**. That is precisely what falls through the
#W48-D11 cycle holder: its collapse floor is 3 repetitions AND the run must survive un-flushed.
The fold is therefore done at the WRITE seam (`AIPlayerGPT::writeNarration` →
`collapseAdjacentDuplicate` → the pure `foldDuplicateTail`), which catches the run however it was
broken upstream, and it reuses **#W43-11's own renderer** (`collapsedRunNarration`) so there is one
idiom for "this happened N times": `- Opponent drew 2 cards`, not a second invented shape.
(The docket's own wording is `(xN)`, which is that renderer's fallback for a sentence it cannot
re-conjugate — hence the choice over the lane brief's literal `repeated N times`.) Nothing is
dropped: the count is exact, two draws in one turn stay TRUE and stay visible. The record delta
(`events`) folds with the log when the earlier occurrence is still in this delta, and otherwise
carries the single new occurrence verbatim, so a record never over- or under-states what happened
since the last one. A direct writer (turn header, trim marker) landing in between makes the fold a
no-op rather than a corruption — that is what `foldDuplicateTail`'s tail check is for.

---

## D13 — MEASUREMENT ONLY (proposal awaiting the owner, D43; nothing implemented)

Per-ask-kind narration share, wave-56 corpus, 2,716 records that carry both a prompt and an option
list. `log%` = the GAME LOG block (`GAME LOG (...)` up to `--- CURRENT SITUATION ---`) as a share of
the whole prompt. `>3 turns` counts records whose log carries more than three `=== Turn N ===`
headers. `log% if 2 turns` is what the log share would be if the log were cut to the last two turn
headers — i.e. the D13 proposal's ceiling, measured, not estimated.

| ask class | n | mean prompt | log% | mean turns of log | >3 turns | log% if 2 turns |
|---|---:|---:|---:|---:|---:|---:|
| `ask`, stack empty | 1521 | 10834 | 60.3 | 20.5 | 1475 | 6.1 |
| `ask`, stack live | 422 | 13653 | 63.4 | 25.3 | 422 | 5.6 |
| `priority`, stack empty | 352 | 11721 | 60.6 | 19.4 | 350 | 5.9 |
| **land-drop ask (pure menu)** | **140** | **5588** | **49.2** | **9.0** | **93** | **8.1** |
| `priority`, stack live | 104 | 15810 | 65.4 | 25.6 | 104 | 5.3 |
| attackers | 88 | 10910 | 58.9 | 14.8 | 88 | 8.4 |
| discard | 55 | 12843 | 62.9 | 27.0 | 55 | 5.3 |
| blockers | 25 | 10707 | 52.7 | 13.4 | 25 | 10.2 |
| reveal | 9 | 10681 | 35.3 | 11.4 | 9 | 8.1 |
| **ALL** | **2716** | **11349** | **60.8** | **20.5** | **2621** | **6.1** |

Readings, offered as findings and not as a recommendation:

1. The GAME LOG is **60.8%** of the mean prompt corpus-wide (the docket's 54.0% figure is close; the
   difference is that this count includes the log's own header and runs to the situation marker).
   **17.9 MB of the 30.8 MB of prompt text in this corpus is game log.**
2. The docket's prediction *"a land-drop ask carrying more than 3 turns of log — 0 of N"* is today
   **93 of 140**, and **2,621 of 2,716** records across all kinds carry more than 3 turns.
3. A land-drop ask is the CHEAPEST prompt in the corpus already (5,588 chars mean, half the corpus
   mean) and its log share is the second-lowest. **The narration mass is not in the land-drop asks;
   it is in the 1,943 `ask` records (71.5% of the corpus) at 10.8-13.7 KB each.** A cap scoped to
   land drops as the docket describes would reach 140 records and about 0.37 MB — 2% of the log
   bytes. The same cap applied to the empty-stack `ask` class reaches 9.5 MB.
4. A two-turn cut is worth roughly **10x** on every class: log share falls from ~60% to ~6%.
   Whether that is the right trade is a decision about what the pilot may forget, not a measurement,
   and it is the owner's (D43).

---

## Gate (worktree, hermetic `make -B` for the baseline, incremental for the final)

| | base (own worktree, `4d4ea0bdf`) | lane D |
|---|---|---|
| PARSETEST | 2736 / 0 | **2772 / 0** (+36) |
| suite, `WAGIC_TESTSUITE_THREADS=1` | 1241 tests, 0 failed | **1242 tests, 0 failed** (+1 fixture) |
| AI tests | 55 / 0 | **55 / 0** |
| suite timeouts | 0 | **0** |
| `git diff \| grep -c U+FFFD` | — | **0** |

Logs: `~/.gatelogs/w57D-{build,suite,parsetest}-{base,final8}.log`.

The brief's baseline names "1241/2-known or 1241/0 at THREADS=1"; this worktree's own base gate was
**1241 / 0** at THREADS=1 (the lifeline / merrow_reejerey pair is concurrency-only and did not fire).

### Suite pin

`bin/Res/test/w57d_display_toggle_seat_scope.txt` + a new `assertdisplaytoggle <1|0> <card>` command
(`src/TestSuiteAI.cpp`), the same shape lane B's `assertabilityseat` took when a GPT-seat predicate
needed a live card the PARSETEST corpus cannot build. It pins D14's discriminator in BOTH directions
against real card data: **Hengegate Pathway → 1 (suppressed)**, **Tergrid, God of Fright → 0
(offered)**. Instrument proven to fire: an inverted copy of the fixture reports
`expected 0 got 1` / `expected 1 got 0` and fails the test (`~/.gatelogs/w57D-probe3.log`), so a
green is a green and not a silent no-op.

**What the suite CANNOT pin, stated plainly:** the AI-seat half of D14 is not fixture-expressible.
The suite's seats are TestSuiteAI/Baka; `AIPlayerGPT::chooseOrderedAction` and `chooseMenuAction`
only run with an endpoint configured, so there is no fixture that can be RED-on-base by *rendering*
a Flip Side row. The predicate pin above plus PARSETEST plus the live probe below are the three
instruments that do cover it.

### Live probe (arrival-traced, GPT-vs-GPT, qwen36-35b-a3b on Spark :8084)

Two probe decks (198/199: 10× Emeria's Call + 10× Agadeem's Awakening + 10× Hengegate Pathway /
10× Pelakka Predation + filler), one-shot, one full game (22 turns, natural end). Records:
`~/.Wagic/ai/gpt/logs/1788470902-*.jsonl`. **The probe decks were DELETED from `bin/Res/ai/baka/`
after the run.** 56 records carrying an option list:

* `Flip Side` rows: **1** — and it is the spell-face cast-mode menu described under D14 (no land row
  to duplicate, so correctly still offered). **0 land-backed toggle rows reached the seat.**
* `PLAY THIS AS A LAND` rows: **15, taken 7.**
* D26 clause by class, all correct against the scripts: **10 pay-or-tap** (Agadeem 5, Emeria 5),
  **2 enters-tapped** (Pelakka Caverns), **3 no clause** (Mistgate Pathway, the Pathway class).
* D28: every row head is the printed name — `Agadeem, the Undercrypt with Agadeem's Awakening`,
  `Emeria, Shattered Skyclave`, `Mistgate Pathway`, `Pelakka Caverns`. The pre-lane form was
  `agadeem, the undercrypt with ...`.
* The probe is what caught the false cast-mode toggle tail (fixed above, re-gated after).

---

## Falsifiable predictions for wave 58

1. **D14.** `Flip Side` rows rendered to an AI seat whose card's other face is a LAND: **0 of N**
   (this corpus: 108). Spell-face toggle rows may still render and are not a miss — their count is
   the size of the D33 residue (this corpus: 1). `only display-toggle ... auto-passing` lines stay
   bounded and no run exceeds 4 (this corpus: 760 lines, longest run 4).
2. **D14 (b).** Option sets built and discarded for a land-backed toggle-only priority window: **0**
   — the drop is at the candidate level, so `describeAction` is never entered for that row.
3. **D26.** Reply plans asserting an UNTAPPED arrival for a pay-3-life back face before its follow-up
   menu is shown: **0 of N** (this corpus: the `146v126` seq 19 class). And plans asserting a usable
   land this turn from an unconditional-tapped back face (Pelakka Caverns, Malakir Mire):
   **0 of N** — a prediction the docket's own record could not make, because it had that card in the
   wrong class.
4. **D27.** Replies that decline a `[NAMED BY THEIR ...]` row while reasoning that the price will go
   away, or that the surcharge is one-off: **0 of N** (this corpus: unmeasured, 4 renders / 1 take).
5. **D28.** Lowercased script tokens (`agadeem, the undercrypt`, `boulderloft pathway`) appearing in
   `chosen_text` or in a narration line: **0 of N** (this corpus: 54 narration renders from one take
   at `152v162`).
6. **D29.** Adjacent byte-identical GAME LOG lines in any record: **0 of N** (this corpus: 1,767 in
   670 records). Records whose log carries a collapsed count line (`drew N cards`, `(xN)`): **> 0**.
7. **D13 (measurement only, no code).** If the owner rules on D43, the land-drop-scoped cap reaches
   ~140 records / ~0.37 MB; the `ask` classes are where the 17.9 MB is.

---

## What I did NOT verify

* **The human / console seat.** D14 changes only the GPT seat's shown list, and the fixture pins that
  the engine's menu is untouched — but I did not drive the GUI, and I did not check the PSP/Vita
  `cardauto.{idx,dat}` sidecars. No card script changed this lane, so a port build needs nothing new.
* **D14 at the `ask` seam under a DECLINE-only remainder.** The guard (`shownOpts.size() >= 2`) means
  such a menu keeps its full list; I did not construct a live menu that hits it, and no corpus record
  shows one.
* **D27's clause against a SECOND naming permanent.** The clause names one source (the one whose
  script was scanned). A board with two different naming permanents taxing the same cast would print
  one clause per row per source; I did not build that board, and the corpus has one such permanent.
* **D29 under the 24 KB narration trim.** The trim rewrites the log from the FRONT and the fold only
  ever touches the LAST line, so they cannot collide; I reasoned this rather than driving a game long
  enough to trim while a run was open.
* **D29's effect on the ask cache / prompt prefix.** The fold rewrites the last log line in place,
  which mutates a prefix that was previously append-only. Every prompt is assembled fresh from
  `mNarration` and the staleness key is the full prompt text, so a rewritten tail behaves exactly
  like a new line — but I did not measure cache-hit behaviour, and lane M's cache counters still
  reach no log (D37).
* **Corpus-scale numbers for any of the five.** One probe game is not a corpus; the predictions above
  are for the wave-57 corpus to settle.
* **The suite's non-determinism seen during this lane.** Three suite runs terminated EARLY (at
  5k/16k/116 lines, each at a different `generic/*_i1085.txt` fixture) while other wave-57 lanes were
  building and running wagic concurrently, and one left a SIGSEGV core in
  `GameStateDuel::End` → `SAFE_DELETE(transcriptMenu)` (the shutdown path, after the run). Under a
  quiet machine the same binary completed 1242/0 twice. I did NOT root-cause it; it is not in this
  lane's diff (none of this lane's code runs in a TestSuiteAI seat), all fleet lanes share one
  `~/.Wagic/`, and it is worth a docket line for wave 58.
