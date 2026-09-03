# Wave-55 lane D — menus + collapse + discard (D8, D18, D22, D9)

Base: master `8a1768d1c`. Gates on this worktree, all under
`MemoryMax=4G / MemorySwapMax=0` (no kill):

| gate | base (brief) | this branch |
|---|---|---|
| PARSETEST | 2489 / 0 failed | **2535 / 0 failed** (+46) |
| suite (`WAGIC_TESTSUITE_THREADS=1`, ground truth) | 1228, 2 concurrency-only failures multi-threaded | **1228 tests (0 failed) + 51 AI (0 failed), 0 timeouts** |

`git diff | /usr/bin/grep -c $'\357\277\275'` = 0. Build incremental over the seeded
`objs-sdl` (no `make -B` — see "what I did NOT verify").

---

## FIRST: three docket numbers are artifacts of the wrong field, and one item is bigger than the docket says

Before writing any code I re-derived D8/D9/D18 from the corpus **prompts**
(`matchups-20260903-034355`, python over every `.jsonl`). The ledger's counts for
these three were computed from the record's **`options`** array — the raw,
UNCOLLAPSED option list the engine hands the seat — not from the `prompt` the model
actually read. On the prompt:

* **D8 — "46 records, 403 redundant rows, ~160,800 chars" is not what rendered.**
  The corpus's own repro (`123v162` seq 26) renders as **ONE line**:
  `1-23. Equip with Lightning Greaves targeting Vampire #1-#23 [your battlefield] (… this MOVES it to Vampire #1-#23 …) x23`.
  Wave-53 lane P's 1-D collapse was already firing on every `#N` family. Counting
  redundant rows in the PROMPTS of all 2,212 decisions: **12**, in three records
  (`Destroy with Dwarven Blastminer #N targeting Underground Sea #N` ×2 records,
  `Deal 2 damage with Pyrite Spellbomb targeting Goblin #N`). This is the same
  correction wave-54 lane D flagged for its own "fourteen byte-identical Mountains"
  bullet; it recurred at ledger scale. The genuine 2-D residual is real but small,
  and it is exactly the shape lane D declined to ship.
* **D9 — "the nine options are bare card names … no annotation on any row" is the
  `options` array.** `125v162` seq 47's rendered rows carry copy tag, mana cost,
  type and `{card text: …}`. What is genuinely missing is the **verdicts** — which
  is what D9 asks for, so the item stands; only its "no annotation of any kind"
  framing does not.
* **D18 — bigger than filed, and it is this lane's largest win.** Reveal rows are
  not byte-identical: `copyOfTag` gives them `(copy 2 of 4 in this list)`, a SECOND
  instance grammar `splitRowHandle` cannot see (it only knows `" #N"`). Rows
  collapsible under that grammar, corpus-wide, in the PROMPTS: **254 rows /
  36,274 chars** (89 reveal records, 15 discard records) — 20× D8's residual.

Everything below was built against those re-derived numbers.

---

## What shipped

### D18 (= R210) — the copy-tag instance grammar, and the collapse that reaches reveal + discard
`src/AIPlayerGPT.cpp`, `include/AIPlayerGPT.h`

* `splitCopyRowHandle(row, head, tail, rank, total, scope)` — the second instance
  grammar, decomposed exactly as `splitRowHandle` does the first. Refuses an
  out-of-range or single-copy tag (`copy 0 of 4`, `copy 5 of 4`, `copy 1 of 1`) and
  anything that is not the full `(copy K of N in <scope>)` shape.
* `copyRangeTag(first, last, total, scope)` — the printed range,
  `(copies 1-4 of 4 in this list)`; a run of one keeps the singular tag. **No
  invented notation**: the range is the copies the run covers, out of the same
  total and in the same scope the uncollapsed rows named.
* `groupNumberedRows` tries the copy grammar after the `#N` grammar fails (its own
  `\x05` key namespace, keyed on head+tail+total+scope), so scattered copies gather
  the way `#N` families do. `joinNumberedRows` prints a consecutive-rank run of them
  as one row.
* **The two seams that were building their own numbered lists now go through the
  collapse.** `buildRevealAskText` and `AIPlayerGPT::buildCleanupDiscardAskText`
  build a `vector<string>` of rows, group + join it, and hand the permutation back
  through a new `outOrder` out-param.
* **Index-safety** is one function, `unpermuteSelection(order, n, send)`: the reply
  is parsed in PRINTED positions (the `names` vector is permuted to match) and
  mapped back before anything reads `revealed[j]` / `hand[j]`. An identity
  permutation is a no-op; a size mismatch is refused rather than half-applied.
* Nothing is deleted and nothing is capped: every card keeps its own option number,
  and `kOptionRangeNote` gains the decode clause for the copy form.

### D8 (= R200, engine seat D-3) — the two-dimensional (source × target) collapse
`src/AIPlayerGPT.cpp`

Wave-54 lane D's objection was correct and is answered rather than overridden: a
cross-product printed as a single `#a-#b × #c-#d` range **cannot** be decoded
truthfully, so this does not print one. Instead:

* `splitSourceOrdinal(row, masked, rank, sourceName)` finds the **source's own**
  ordinal — the last `#K` token BEFORE `" targeting "` — and masks **only that one
  token**. That is the honest mask: `Blastminer #1 targeting Sea #1` and
  `Blastminer #2 targeting Sea #1` are the same option from two sources, while a
  blanket mask would also equate them with `#2 targeting Sea #2`, which is a
  different option (pinned as a PARSETEST negative).
* `joinNumberedRows` detects that rows come off the engine **source-major** — every
  option of source #1, then the same options of source #2 — and marks the second and
  later blocks. No row is moved.
* Each later block prints as ONE line, `sourceBlockLine(...)`:
  `12-15. The same 4 options as 8-11, with Dwarven Blastminer #2 as the source instead of #1 (12 is the same choice as 8, 13 as 9, and so on to 15 as 11) x4`.
  **The decode is exact and stated on the line**, in option NUMBERS, so it composes
  with the 1-D collapse inside the reference block (2 Staff of Nin × 14 identical
  Vampires = 28 options → 2 lines, and 15↔1 … 28↔14 still resolves).
* Guards: both blocks must be the same length, the block length ≥ 2, the collapsed
  rows ≥ the collapse floor (3), the repeated block must be exactly that long, and
  **every** row must match after masking. One differing tail anywhere defeats the
  whole collapse (PARSETEST negative). `kOptionRangeNote` gains the decode clause.

### D22 (= R214) — the `chooseaname` menu is a card-NAME menu
`include/DecisionContract.h`, `src/DecisionContract.cpp`, `src/AIPlayerGPT.cpp`

* Root cause, on disk: `chooseaname` reaches the seat as `CHOOSE_MODE` because
  `GenericChooseTypeColorName::resolve` builds the name list as a `MenuAbility` of
  `AASetNameChosen` options — indistinguishable from a modal "choose one" at
  `buildMenuChoice`. The header was true of the menu OBJECT and false of the
  decision.
* `DecisionRequest::nameChoiceMenu` is set from the engine's own option objects
  (`dynamic_cast<AASetNameChosen*>(menu->abilities[0])`), never guessed from the
  strings. The KIND stays `CHOOSE_MODE` — the apply path is byte-identical, so
  `applyMenuChoice`, the staleness key (`req.optionTexts`) and the option order are
  untouched.
* `chooseANameHeaderText(source, cardText)`:
  `Choose a card NAME for Silverquill Silencer - this is a card-NAME choice, NOT a mode: every row below is the NAME of a card, and the one you pick becomes the name Silverquill Silencer is set to. What naming it does: <the card's own text>. A row carrying a {visible now: ...} tag names a card that can be seen in a public zone right now; an unmarked row names a card that is not visible in any public zone.`
* `namedCardVisibilityTag(theirBf, theirGy, myBf, myGy)` on each row — **public zones
  only**. The engine builds the name list from every zone of the other player,
  hidden ones included; annotating a row with "still in their hand" would put
  information on the prompt the pilot has no right to, so the tag counts only what
  the render already shows. Counts, no verdict: what a name is worth against this
  deck is the pilot's call.
* **Both halves speak one vocabulary.** The observing seat's `[named: <card>]` tag
  is correct and is unchanged; the actor's header and its narration now use the same
  verb — the ask narration for a name menu reads
  `You named "Essence Scatter" with Silverquill Silencer` instead of
  `You chose Silverquill Silencer's mode "Essence Scatter"`.

### D9 (= R201) — the cleanup discard row's three verdicts
`src/AIPlayerGPT.cpp`

Three pure clauses over facts the emitter already computes elsewhere, plus one
board read per ask (not per row):

* `discardSpareLandClause(myLands)` → ` {spare: you control 9 lands already}` on a
  land row.
* `discardAlreadyControlClause(name)` → ` {you already control one: Howling Mine}`
  when a non-token permanent of that name is on the seat's battlefield.
* `discardDeadTargetClause(legalTargets)` → ` {dead right now: 0 legal targets on
  the board for it}`, driven by the engine's OWN chooser
  (`modalSpecObjectCount(observer, card, card->spellTargetType)`) — never a guess
  about what the card is for.
  **The clause is deliberately withheld from any card whose target spec names the
  STACK.** A counterspell has no legal target during a cleanup step and always
  would, so "dead" would be true of the moment and false of the card — the trust
  doctrine's wrong-scope lie, and precisely the card class (`Fall of the Gavel`)
  the seat wrongly discarded in the repro. Rendering it there would have made D9
  worse, not better.
* **Wording note:** the docket's `{dead right now: they control 0 creatures}` is
  narrowed to the edict family; the shipped clause states the count the engine's own
  chooser sees, which is true for every card shape and needed no card-class guess.
* All four new braced annotations (`{spare:`, `{dead right now`,
  `{you already control one`, `{visible now:`) are added to
  `stripNarrationDecoration` — they are decision-time surfaces and must not enter
  the append-only log. **PARSETEST caught this**: the first run failed exactly the
  two echo-residue cases.

---

## PARSETEST (+46 cases; every collapse shape, every new annotation)

Positive, must-NOT-match negative, and echo shape for each:
* D18: the copy grammar's decomposition; four out-of-range/malformed negatives; the
  printed range (plural and singular); the whole path (four scattered copies gather
  and print as one row) with a pair below the floor and a unique row left alone; the
  permutation is total and a bijection; the map-back names the row printed at that
  position; identity is a no-op; a size mismatch is refused.
* D8: the source ordinal is the pre-`targeting` one and the target's survives; three
  negatives (no source ordinal, no `targeting` clause, the mask-collision case);
  the block line's exact decode string; the live 2×4 Blastminer shape rendering as
  five lines; an incomplete second block and a one-row-different block both refusing
  to collapse; the range note carrying both new decode clauses; **two echo cases —
  a bare number inside a collapsed block, and a collapsed member's full text.**
* D9: each clause positive + negative; the echo-residue case.
* D22: header positives, the "no mode wording" negative, the nameless-source
  negative, the visibility tag both-sides positive and the nothing-public negative,
  and the echo case.

---

## Falsifiable predictions (wave-55 corpus)

* **D18** — `reveal` and `discard` records whose rendered option block prints the
  same card name on ≥ 3 rows that differ only in their copy ordinal: **0** (was 254
  rows / 36,274 chars over 104 records). Every such render carries a
  `(copies a-b of N in <scope>)` row and the range note. **No reveal/discard record
  in which the chosen card differs from the card printed at the answered number**
  (the map-back's falsifier — check `chosen_text` against the row).
* **D8** — ask/priority records printing two or more blocks of rows that differ only
  in a source ordinal before `" targeting "`: **0** (was 3 records / 12 rows). Each
  such render carries exactly one `The same N options as A-B, with <source> #K`
  line per repeated source, and **0 out-of-range answers** on those records.
* **D22** — records whose header reads `Choose one mode for <X>:` over rows that are
  bare card names: **0** (was 9/9). Every `chooseaname` record's header starts
  `Choose a card NAME for`, and its narration line reads `You named "<card>" with
  <source>`, matching the observing seat's `[named: <card>]`.
* **D9** — `discard` rows carrying no verdict clause at all where one applies (a
  land row, or a row naming a permanent already on the battlefield): **0**. Cleanup
  discards that send a counterspell while a land is on the numbered list and the
  seat controls 6+ lands: **0** (was 3 of 4). **No `{dead right now:` clause on a
  row whose card targets the stack** — a single instance refutes the gate.

---

## What I did NOT verify

* **No live model probe was run.** Every prediction is about the next corpus.
  Nothing in this lane was measured against a model.
* **No suite fixture ships with this lane, and the D22 change touches
  `DecisionContract.cpp`.** Nothing here changes a game action, a legality gate, a
  choice space or an option order — `nameChoiceMenu` is a read-only classification
  of the engine's own option objects and every string is on the AI seat's prompt,
  which the test suite does not render. The suite was run as a regression gate only.
  A fixture that pinned it would need an `[AI]` game resolving a `chooseaname`
  permanent plus a new `TestSuiteAI` register for menu asks (the wave-54 lane-D
  `ownCardChooserRegister` pattern); I judged that out of budget against three other
  items and am flagging it rather than claiming coverage.
* **The D8 source-block collapse has NOT been observed on live engine output.** Its
  premise — that a multi-source ability menu is emitted source-major and contiguous
  — is read off the corpus's own Dwarven Blastminer records (`130v123` s53/s59,
  rows 8-15) and is what the PARSETEST fixture reproduces; I did not read the
  emitter loop that orders those rows to prove it holds for every ability class. If
  a menu ever interleaves sources, the collapse simply does not fire (the block
  test fails) — the failure mode is the status quo, not a wrong render.
* **The D8/D18 collapses interact with `groupNumberedRows`'s permutation only where
  a caller permutes.** In `askModel` the parallel action list is permuted (wave-48
  machinery, unchanged). In the two new call sites the permutation is inverted with
  `unpermuteSelection`. I did **not** audit every other consumer of
  `joinNumberedRows` for a source-block assumption; the source-block detection is
  render-local and moves no row, so no caller's index space changes.
* **`discardDeadTargetClause`'s gate was not exercised against a real counterspell
  at runtime** — the stack-spec exclusion is pinned by construction (`spellTargetType`
  containing `stack`, verified against `Fall of the Gavel`'s `target=*|stack` in
  `primitives/mtg.txt`) and by PARSETEST on the pure clause, not by a live render.
* **`modalSpecObjectCount` is now called once per non-land hand card at each cleanup
  discard.** It builds and destroys a `TargetChooser` per call. Precedent exists
  (the cast menu calls it per modal spec per row) and a cleanup hand is ≤ ~12 cards,
  but I did not measure it.
* **No hermetic `make -B`.** The gate ran incrementally over the seeded `objs-sdl`.
  The seeded binary's PARSETEST baseline matched the brief's 2489 exactly before any
  edit, which is the evidence the seed is not stale; a shipping gate should still
  rebuild.
* **The D22 visibility tag counts battlefield and graveyard only** — not exile, the
  stack, or the command zone. Those are public too; they were left out to keep the
  tag short, so a name visible only in exile renders unmarked. That is an omission
  of a true fact, not a false statement, but it is an omission.
* **I did not verify Silverquill Silencer's Oracle wording against Scryfall.** The
  header's "what naming it does" sentence is the card's own `text=` line rendered by
  the existing `cardTextSnippet`, so it says whatever the primitive says; if the
  primitive mis-models the card, this header repeats it faithfully.
