# Wave 71 — lane BP: the hang (known-bugs L1, L2)

Base `77d82bbe6` (source-identical to `14c82df37`, the wave-70 corpus binary, over
`projects/mtg/src` and `include`). Evidence: wave70/engine-seat.md §0, wave70/known-bugs.md
L1/L2, and the corpus at `matchups-20260906-224849`
(`game-152v126-1788752967.stderr`, 1.23 GB / 31,012,527 lines; seat logs
`1788752973-ai_baka_deck152-…jsonl` ending at seq 55 and `…deck126-…jsonl` at seq 17;
`results.tsv` row `152 126 hang 20 1 15`).

## L1 — CONFIRMED, in all four links

Re-verified against the corpus bytes before touching code:

| link | claim | measured |
|---|---|---|
| 1 the decline is not sticky | menu cancel changes nothing; Baka re-proposes | `Should I play Hengegate Pathway? shouldPlayPercentage = 90 → AI wants to play card` repeats on every cycle |
| 2 the re-ask is silent | `mAskCache` replays with no call and no record | `Choose an option for Hengegate Pathway: -> chose 3 of 3` appears exactly **1×** in 1.23 GB; the seat log stops at seq 55 |
| 3 the floor cannot fire | `hasAnyLegalAction` arm is inert in a livelock | `menu pass floor reached … not passing` **2,584,166**, `menu pass floor fired` **0** |
| 4 no watchdog covers it | translog-quiet clock only, 3,600 s | game ran 3,641 s and was killed by the harness, not the watchdog |

### Fix (a) — the decline is latched, at the one choke point every decliner passes through
`DecisionManager::applyMenuChoice` (`src/DecisionContract.cpp`), CHOOSE_MENU decline branch:
when the menu's `contextCard` is in the deciding player's **hand**, call
`AIPlayerBaka::latchDeclinedFace(card)` before clicking cancel. `AIPlayerBaka::FindCardToPlay`
then skips any latched card among its **LAND** candidates (`firstLandIdx` marks where the
`legalLandPlays` block begins), and `AIPlayerGPT::FindCardToPlay`'s land seam drops latched
cards from the offered rows so the ask and the validation pass agree by construction. Latch is
per-card, per-turn (a land drop is a once-per-turn window); it is the `held the land drop`
shape the seam one level up already had. Nothing is removed from the seat: every other land is
still playable, and the card returns next turn.
Chosen over latching *all* candidate kinds so a declined CAST menu still exercises (b).

### Fix (b) — a NO-PROGRESS arm on the W64-AK pass floor
`AIPlayerBaka::Act`. The `hasAnyLegalAction` arm was written to keep a window the seat still
wants; a livelock is *defined* by a legal action that never executes, so that arm is inert
exactly where the floor is needed. New: `menuPassProbe()` builds a cheap fingerprint (turn,
phase, both lives, all five zone counts on both sides, stack depth, armed menu name) and the
pure static `menuPassNoProgress()` says when it has repeated `kMenuPassNoProgressMax = 200`
consecutive ticks *while already at the floor*; then the pass is forced and counted separately
(`mMenuPassNoProgress`, and the DebugTrace names which arm fired — the two are different
findings). The predicate is evaluated once per tick, before the arms, so the branch that fires
and the reason recorded are the same evaluation.

### Fix (c) — the ask cache refuses a runaway replay
`AIPlayerGPT::askModel`. A cache hit now runs `askReplayRefuse()`; at
`kAskReplayRefuseMax = 64` consecutive replays of the same key the entry is **erased** and the
window goes back to the model, so a loop either breaks or becomes visible traffic instead of a
1.2 GB stderr. Counted on gameend as `ask_replays_refused`. A real model answer clears the run.

## L2 — the silent replay path now writes records

`logAskReplay()` writes one compact JSONL record per re-served answer on **both** silent paths
(`cache_replay` and `repeat_ask_reserved`), plus `cache_replay_refused` at the refusal.
Record shape:

```
{"seq":N,"kind":"ask_replay","why":"cache_replay|repeat_ask_reserved|cache_replay_refused",
 "replayed_from":<seq the answer was consumed at>,"replay_run":<consecutive replays>,
 "choice":<1-based>,"options":<count>,"turn":T,"phase":P,"question":"<decision line>"}
```

Provenance comes from a new `mAskCacheSeq` shadow map (cleared with the cache on the turn
boundary) and `mRepeatAskSeq`. Volume is bounded by the refusal in (c). Four new gameend
fields: `ask_replays_reserved`, `ask_replays_refused`, `menu_pass_no_progress`,
`declined_face_latches` — all expected 0 in a healthy game.

Harness half (`tools/selfplay-harness.sh`): `no_progress_sweep` gains a FAST arm. The existing
3,600 s translog-quiet clock is right for a slow decision and wrong for a spin. The new arm
requires BOTH — the translog silent for `FAST_HANG_QUIET_S` (300 s) **and** the game's stderr
grown by `FAST_HANG_MB` (64 MB) *within that same silence*, the byte count anchored to the
moment the translog last moved. That keeps the file's own warning intact (the 13-hour 404 park
grew stderr slowly and must not trip) while catching a ~340 KB/s spin in minutes, bounding the
evidence file near 100 MB instead of 1.23 GB. Also fixed: the sweep's glob `.inflight-*` was
matching its own sidecars — `*.hung|*.sz` are now skipped explicitly.

## RED-on-base evidence

The fixtures need an answer no suite could previously produce: `AIPlayerBaka::selectMenuOption`
always takes a real row, and the decline came from `AIPlayerGPT`'s menu seam, which needs a live
endpoint. New suite commands (`src/TestSuiteAI.cpp`, test-only file):

- `aideclineface <card>[ <budget>]` — a STANDING decline for one card's own face/mode menu,
  applied through `DecisionManager::applyMenuChoice(choice = -1)`, the exact call
  `AIPlayerGPT::chooseMenuAction` makes for "Decline - do nothing". Budget 0 = for ever.
  Applied before the heuristic's tick, the order the live path has.
- `assertdeclinesapplied <n>` — how many declines the arm actually made.

Counterfactual method: the *instrument* stays, the *fix* is disabled (the latch call and the
land-candidate skip each `&& false`), rebuilt, and the fixtures run on that binary — so the RED
is the defect, not a missing command.

| fixture | base (instrument, fix disabled) | after |
|---|---|---|
| `ai_mdfc_decline_sticky_w71bp` (budget 4) | **RED** — `assertdeclinesapplied: expected 1, got 4`; hand 1→0 (`Didn't find 491910`), battlefield 2→3: the land the seat REFUSED is in play | **GREEN** — 1 decline, pathway in hand, Goblin cast, P2 19 |
| `ai_mdfc_decline_livelock_w71bp` (budget 0 = the corpus shape) | **RED** — 117 declines and still in `Main phase 1` against an `End` expectation; hand 2 (nothing played), battlefield 1, P2 20: the seat never leaves the window | **GREEN** — 1 decline, End phase, P2 19 |

PARSETEST (pure predicates for (b) and (c), whose live paths need a game and a model):
7 new cases — no-progress POSITIVE (fires at tick 200, re-arms 3× in 601), NEGATIVE (a moving
state never fires over 1,000 ticks), NEGATIVE (one tick of progress resets the run), NEGATIVE
(an empty fingerprint never forces a pass); refusal POSITIVE (63rd index = the 64th replay,
re-arms), NEGATIVE (alternating windows in one tick are not a run), REGRESSION (a 63-long
legitimate re-serve run is still served).

## Gate numbers

- Build: clean link; both link-time guards printed OK (`check-ctor-init: OK (118 file(s))`,
  `check-reply-instructions: OK (1 source file(s), 37 guide(s))`).
- Suite, `WAGIC_TESTSUITE_THREADS=1`, memory-capped detached unit: **1279 tests (0 failed),
  69 AI tests (0 failed)**; `==Test Failed !==` 0, `==Test timed out` 0. (+2 AI fixtures.)
- PARSETEST: base `wagic-14c82df37-w70step1` **5302 passed / 0 failed** → after **5309 / 0**
  (+7, exactly the additions).
- `git diff | grep -c U+FFFD` = 0.

## Predictions for the next corpus (falsifiable)

1. **Zero HUNG games.** `results.tsv` carries no `hang` row and no game's stderr exceeds
   ~100 MB. If a hang still occurs, its stderr is now bounded and the seat log is no longer
   silent — the `ask_replay` records name the window.
2. `identical_ask_answers_reserved` is visible per window: every seat's gameend carries
   `ask_replays_reserved` (expected non-zero — the legitimate drain-loop re-serves — and
   `ask_replays_refused` expected **0** in healthy games; any non-zero is a loop report).
3. `declined_face_latches` is small and non-zero across the corpus (declines happen and now
   stick); `menu_pass_no_progress` is **0** — if it is not, a livelock class outside the land
   drop exists and (b) caught it, which is itself the finding.

## Weakest evidence

**Fix (b) has no live fixture.** Its predicate is pinned in PARSETEST and its wiring is
compile-verified and suite-green, but no test drives the actual `Act` branch: producing a
non-land click-time menu that re-arms for ever needs a card whose click offers two rules and a
decline, and the only family I could reach in a fixture is the MDFC land, which fix (a) now
latches first. So (b)'s *live* behaviour — that a stalled cast menu is broken by the forced
pass — is untested; the corpus counter in prediction 3 is the first real evidence.

Second-weakest: `kMenuPassNoProgressMax = 200` and `kAskReplayRefuseMax = 64` are chosen, not
measured. 200 ticks past a 24-tick floor is ~8× the widest self-resolve the suite's own
mandatory-chooser guard measured (8 driver ticks), and 64 is comfortably above the largest
legitimate re-serve run in the wave-70 corpus (the K10 note's 32 identical upkeep windows), but
neither has a distribution behind it.

Third: the latch is keyed on the `MTGCardInstance *` pointer plus the turn. A card that leaves
and re-enters hand within one turn would come back as a different instance and be proposable
again — correct behaviour in my reading (it is a new object the seat has not refused), but it
is a reading, not a measurement.
