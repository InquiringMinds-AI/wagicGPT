# Wave-74 lane CH — the hold re-opened at every link of a trigger chain

Base `1d1d3744d` (gate 3: 1285/0, 76 AI/0, PARSETEST 5760/0). Worktree
`worktrees/lanes/w74-CH`, branch `w74-lane-CH`.

Gate on this branch: `make -f Makefile.sdl -j8` clean, both link guards OK
(`check-ctor-init: OK (118 files)`, `check-reply-instructions: OK (1 source, 37 guides)`).
**Suite THREADS=1, detached unit `w74-CH-suite`: 1285 tests (0 failed), 76 AI tests
(0 failed)** — `==Test Failed !==` 0 and `==Test timed out` 0; none of the three known
flakes fired, so no solo rerun was needed. **PARSETEST 5,788 passed / 0 failed**
(base 5,788 − 5,760 = **+28 cases**). `bash -n tools/selfplay-harness.sh` OK;
`--selftest` 4 + 13 + **12** + 30 checks, 0 failed. `git diff | grep -c $'\357\277\275'` = **0**.
Every wagic invocation ran under `systemd-run --user --scope -q -p MemoryMax=4G -p
MemorySwapMax=0 --`; nothing was killed under the cap.

---

## Root cause

`takeHold` records the model's own hold as a SET OF RENDERED ROWS and `holdStillStands`
re-checks that set at every later window through `holdKeyRow`. `holdKeyRow` is a
RENDER key: it erases three named clauses and normalises a life total introduced by one
of six ENUMERATED anchor phrases (`#W66-AS` H7, `#W66-AU` R1). Everything else in a
`{...}` or `[...]` annotation is compared byte for byte.

That enumeration is a losing race. Every wave adds annotations that price the board
against a life total, and each new spelling re-opens every hold again until somebody
adds it to the anchor list. Wave 74 added two, and both landed on the row a holding seat
is looking at.

### Instance 1 — the seat's OWN chain (`125v126`, deck126 seat, turn 32 Upkeep)

41 casting-decision asks in ONE phase, every one a full model call, every one answered
`Hold priority`, while that seat's own **Sanguine Bond + Exquisite Blood** chain drained
the opponent **21 → 1** (one life per link). The chain was one link from winning when the
harness killed the run. In that game's stderr:
`took the hold row at the cast seam on turn 31` **33 times**, and
`hold re-opened at the cast seam - a printed row changed or is newly available`
**240 times**.

Diffed byte for byte off the seat's own `options_text`, the menu was three rows at every
one of the 41 windows (`Cast Tribute to Hunger`, `Hold priority`, `Cast nothing right
now`) and only the third row ever moved:

| pair | my_life/opp_life | the ONLY delta |
|---|---|---|
| seq 266 → 267 | 19/21 → 19/21 | the O11 re-ask tag ARRIVES: `…declined this exact list 1 time this turn` |
| seq 307 → 308 | 38/2 → 38/1 | O9's own-clock tag `reaches 0 in 2 more turns` → `1 more turn`, and the O11 count `39` → `40` |

So the hold was re-opened at **link one** by the O11 count appearing, and at every link
after that by O9's clock counting the opponent's life down. Neither is an action.

### Instance 2 — the OPPONENT's chain (`125v162`, deck125 seat, turn 27 Draw)

38 asks in one draw step under Ob Nixilis + Underworld Dreams, `Hold priority` every
time. The row that moved is **not** a decline row and **not** an O6 `would be at N`
spelling — it is the X-cast row's O8 bracket restating the damage already on the stack:

```
[<- best X for this cast: X=9 … counted from the 8 life the 18 damage ALREADY ON THE
 STACK leaves you on, puts you at -1; this KILLS you …]      (seq 138)
…                                                the 17 damage …                (seq 139)
```

A **third** annotation class, in a different bracket, on a different seam — which is the
evidence that this is not a missing spelling but a wrong KEY SHAPE.

Deck152 (17 asks, turn 39) and deck146 (15, turn 21) are legitimate state changes and
are not this.

## Fix

**The hold's row-identity key is the ACTION, not the render.** `holdActionKeyRow()`
(new) erases the `(combat comes next this turn)` clause by name — it is a plain
parenthetical, not an annotation group, so `#W56-A` D1's main-1/main-2 equivalence must
still be done explicitly — and then runs the row through **`stripRenderAnnotationsLc`**,
the same stripper `optionSetKeyOf` already uses (which is exactly why the O11 count never
disturbed the option-set key, and why `#W74-CE` could believe the clause was safe
everywhere). `holdKeyLifeProjectionsNormalised` is kept after the strip as belt and
braces for a life total printed outside any annotation.

`holdStillStands` gained a defaulted key-function parameter. Only `holdHonoured` passes
`holdActionKeyRow`; `holdReopenNote` / `w74HoldUnseenRows` (the re-open BRACKET, `#W74-CD`
O6) keep `holdKeyRow`, so the note still tells the model truthfully how many rendered rows
moved. `takeHold` records action keys.

**What still re-opens a hold**: a row appearing, a row disappearing, a row naming a
different card, cost or target — every ACTION difference; the holder's own untap
(`releaseHoldIfUntapPassed`, `#W73-BY` N2a — unchanged, and it is what bounds the whole
forgiveness); and the two LETHALITY verdicts, which are deliberately not row annotations
but synthetic marker rows joined to the held set and recomputed off the live board every
window. `#W68-BB` (J9) already did this for the crack-back verdict; because the action key
strips J9's `{answers the stack: NO …}` clause off the decline rows, this lane adds its
twin, `stackDeathVerdictKey` / `stackDeathVerdictNow`, so a hold taken over a survivable
stack is still re-opened the window that stack turns lethal.

`w72RowIsDeclineOrHold` is now case-insensitive on its heads: the sibling rule (`#W72-BX`
F1) reads the same keys, and those are lower-cased by the stripper. Left as-is it would
have called every decline row an ACTING row and inverted F1.

Nothing is removed, capped or auto-answered; no rendered byte changed anywhere.

## Deliverables

**(a) The hold key is the action identity** — as above.

**(b) PARSETEST pins, +28, all on the two seats' verbatim `options_text`.**
- seq 266 → 267 and seq 307 → 308 key identically and the hold **stands**; the same pairs
  key differently under `holdKeyRow` — the **RED-on-base assertion is in the corpus**, so
  the base behaviour is pinned rather than described.
- link 1 and link 40 of the chain are the SAME key.
- Instance 2's rows key identically under the action key and differently under the render
  key.
- MUST-NOT-MATCH: a genuinely new action row (`Cast Path to Exile …`) re-opens the hold
  with `a printed row changed or is newly available`; a row disappearing re-opens it with
  `a printed row it was held over is gone`; the same card aimed at a different object is
  a different key (the target name is outside the annotations).
- The two verdict markers are returned byte for byte (stripping would erase them —
  they are all brackets), survive → LETHAL still re-opens on both, and an unchanged
  verdict holds.
- The decline heads still classify once keyed, a real cast still does not, and
  `Pass priority (combat comes next this turn)` still keys to `Pass priority`.

**(c) Fixture: the chain under a taken hold — `tools/hold-chain-probe.sh` (new).**
**Not a suite fixture, and cannot be one**: the shape needs a seat that asks a MODEL and
then TAKES the hold row; the suite has no endpoint, so its GPT seats fall back to the
heuristic and `takeHold` is never called (the same reason lane CG's window-loop fixture is
a probe). The model is `tools/gpt-stub-server.py`, preferring the chain pieces and
otherwise `Hold priority`. Probe decks live in `tools/fixtures/w74-CH/` and are copied
into `Res/ai/baka` only for the run, then deleted: deck198 = Sanguine Bond ×4 + Exquisite
Blood ×4 + Nyx-Fleece Ram ×4 (1 life every upkeep — the chain starter) + **Tribute to
Hunger ×4**, the no-op acting row that keeps the cast window open, which is the corpus
seat's own shape; deck199 = lands only.

Five runs per binary (real hands, so the games differ):

| binary | holds TAKEN | holds RE-OPENED | windows the hold **HELD** | model calls |
|---|---|---|---|---|
| base `wagic-60e9fe2ad-w74step1b` | 0, 12, 13, 59, 64 | 0, 11, 12, 57, 59 | **0, 0, 0, 0, 0** | 24–105 |
| this branch | 0, 0, 0, 4, 5 | 0, 0, 0, 1, 3 | **0, 0, 0, 63, 65** | 26–34 |

The discriminator is the third column, and it is categorical: on the base binary a hold
the model TOOK closed **no window at all** in 5 runs of 5 — every link re-opened it. On
this branch, whenever a hold was taken it held **63–65** windows and was re-opened 1–3
times (by real action changes). Model calls fall from a 24–105 spread to 26–34.

**(d) Harness: the WINDOW LOOP tripwire now requires an UNCHANGED BOARD.** Lane CG's
predicate is a count only, and it stopped run `20260909-104713` at 18/21 games on a
FINITE CHAIN that was making progress. `window_loop_verdict` now takes K
(`WAGIC_WINDOW_LOOP_K`, default 8) and fires only when the count is ≥ N **and** the
newest K decisions of that seat sit on ONE board — identical `my_life`, identical
`opp_life`, identical stack-top line — **or** carry no `events` at all. Banner names both
terms and routes the reader to CG (unstable ask key) or CH (hold key reading the render).
Selftest grew 7 → **12 checks, 0 failed**: the moving-life chain reads OK, the seat's-own-
life variant reads OK, a chain that moves only the stack top reads OK, the real dead loop
still reads LOOP, and a frozen board with narration still reads LOOP.

Against the real directories:

| run | base harness | this branch |
|---|---|---|
| `matchups-20260909-015553-INVALID-windowloop` (the dead loop) | `LOOP 382 …deck123…` | `LOOP 382 …deck123…` |
| `matchups-20260909-104713` (the chain, falsely stopped) | **`LOOP 40 …deck126…`** | **`OK 0`** |
| `matchups-20260907-225637-final` (healthy wave 73) | `OK 0` | `OK 0` |

**(e) O13 chain cost — REPORT ONLY, no collapse.** Over the 36 gameends of
`matchups-20260909-104713`: `chain_windows_collapsed` **0**, `chain_windows_only_selfharm`
**6**, `chain_selfharm_rows` 558, `chain_acting_rows` 989. The 41 windows of the deck126
chain contributed **nothing** to those counters, and the reason is structural: the O13
instrument runs at the PRIORITY seam only (it is measured over `shownLines` in the
priority arm), while the whole chain was paid at the CASTING seam. Measured directly off
the corpus's own `options_text` instead: of **2,428** ask windows carrying at least one
acting row, **416 (17%)** offered nothing but acting rows whose own annotation says
`this does nothing` — 210 on deck126 seats, 175 on deck123, 31 on deck146. The deck126
chain's only acting row was
`Cast Tribute to Hunger {right now: they control 0 creatures - at 0 this does nothing}`,
i.e. all 41 windows were of that class. **Recommendation for O13's owner (not done here):
the instrument needs the casting seam before its number means anything.**

## Prediction (falsifiable, next corpus)

On the same deck pool, summed over all gameends: `hold_windows_skipped_cast` rises well
above the 141 of `matchups-20260909-104713` and **no seat log shows more than ~5 asks in
one (turn, phase) while its own or its opponent's life total is moving monotonically** —
the 41-ask and 38-ask phases do not recur. `hold_released_turn` stays non-zero (the untap
release is untouched). The WINDOW LOOP tripwire does not fire on a chain; if it fires at
all, the named seat's newest 8 decisions will share one board, and that is a real wedge.
If instead a seat still walks a chain phase past ~10 asks, the re-open is coming from an
ACTION row genuinely appearing and disappearing per link, not from the key — check the
`held rows` / `rows now` stderr pair before touching this key again.

## Weakest evidence

* **The forgiveness is now broad, and `#W66-AU` (R1)'s case is inside it.** A verdict
  WORD or a magnitude inside an annotation — `kills 0 of their 1 creatures` →
  `kills 1 of their 1 creatures`, the case R1 called "the blind cache this latch must
  never become" — no longer re-opens a hold. Within one turn, under a hold the model
  itself took over that row, the seat now waits for its untap. I judged that acceptable
  because the untap release (`#W73-BY` N2a) bounds it to one turn and the two lethality
  classes are carved out as marker rows, but it IS a real behavioural loss and it is not
  measured: no corpus has been run since.
* **The two lethality carve-outs are the only ones.** If a third class of annotation
  carries a fact the model must be re-asked over (a "your only blocker just died" style
  verdict), it needs its own marker row and will not be caught by the key.
* **`stripRenderAnnotationsLc` does not handle NESTED braces.** Instance 2's row carries
  `{no {leaves ...} count on this row: … X=9, 12 mana total … is X=8}`; the stripper
  closes on the FIRST `}`, so the tail of that clause survives into the key, digits and
  all. That is conservative (it can only cause extra re-opens, never a stale hold) and it
  did not affect either instance's pin, but it means the "every annotation is stripped"
  claim is true of the stripper's contract and not quite of that one row. I did not fix
  the nesting: it would change `optionSetKeyOf` and both ask keys corpus-wide, which is
  not this lane's scope.
* **The probe is stochastic**, like lane CG's — real hands, no seed control. The
  categorical column (0 held on base, 5 runs of 5) is the honest discriminator; the
  finish/turn-count columns are not evidence. Two of five fixed runs never took a hold at
  all and prove nothing.
* **The stub answers instantly.** It reproduces the control flow faithfully and says
  nothing about the wall-clock arithmetic that actually froze the corpus.
* **No live-endpoint validation.** Nothing here ran against Spark.
* **`holdReopenNote` deliberately keeps the render key**, so the bracket can now say
  "1 row above is new" about a window the latch would have held. It is only rendered on
  windows that ARE asked, so the model never reads a note contradicting a hold in force —
  but the two keys are no longer the single key `#W74-CD` (O6) unified them into.
* **The O13 number is a lower bound and a substitute measurement**: `this does nothing` is
  the annotation I could count off the corpus, not the classifier's own predicate.
