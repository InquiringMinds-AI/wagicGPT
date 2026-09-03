# Wave-57 review carry-list (assembled 2026-09-03 at corpus prep)

Corpus: fourteenth fair-hand corpus. Binary = hermetic `make -B` of master **67b2bb38a** (wave-57
step-1: lanes A e67d3534c, B 002ae42c6, C 6a07a6571, D ba2afebd6, E a81f05695, F 59789dce2,
G b1e06895b, H 01fe39f1f, S c461135c7, T 9cfdd71d1, U b9feba4e0 on base 4d4ea0bdf = wave-56
close + hotfix lanes Y/Z/X/W; merged as b787310e1 A, 22e3117a6 B, 1c7ba720d C, b87db7d38 H,
52c60e114 E, 7f2f1679c G, 0a5668074 S, c74486020 T, cf59f3662 D, c2f6ff389 F, 67b2bb38a U).
Gate on 67b2bb38a: suite 1245 (2 failed = lifeline + merrow_reejerey, the known concurrency-only pair), 57 AI/0, 0 timeouts, PARSETEST 3350/0. Archived `archives/wagic-67b2bb38a-w57step1`. Model:
qwen36-35b-a3b on Spark :8084. Guides = wave-56 reviewer revisions (1e1dcd5ab) + wave-56 boundary
pass (a93aad889). Same pool/recipe as waves 54-56 (-j 21, -T 0, WAGIC_GPT_TIMEOUT=900,
MAXTOKENS=6000); `WAGIC_GPT_LOGWINDOW` UNSET (= `full`, byte-identical prompts) — the log-window
A/B is a SEPARATE run (lane H's design, §D). Vita: vpk16 (4366e8eec) is the owner's live build;
**vpk17 = 67b2bb38a** builds after this gate (carries both candidate softlock fixes F/U + the
dump-and-quit menu entry T + grouping G).

## Discipline (owner doctrine 2026-08-26)
Reviewers own guides; synthesis owns skill + general; the core loop is the purpose. Count from the
rendered `prompt`, never the `options` array. HOLD takes: `chosen_text` is now the rendered row
byte-for-byte at every priority seam (lane A D4) — an EXACT-match census must read 320/320-class
numbers; if it reads lower, D4 failed, do not switch to prefix. Baselines from wave 56 (21/21
natural, 0 SEGV, 2,762 decisions, 6 fallbacks 0.22%, 65 stale drops (44 land-drop arm),
HOLD 2,016/320 (15.9%), same-turn identical re-asks after a take 14/320, D2 145/145 correct + 4
lethal rows / 3 takes / 3 wins, D8 65 rows / 6 takes / 6 arrivals (44 illegal — FIXED by lane W),
`only display-toggle` 760 lines max run 4, INCOMING 25 of 260 combat windows, discard rows
357/426 bare, X rows 0/40 with a fit clause, narration 60.8% of the mean prompt).

## Set B - engine changes since the wave-56 corpus, each with its falsifiable prediction
- A (D5/D4/D16/D31): second async slot (`mAsyncLandState`; arm read off `mPromptTail`;
  `asyncBusy()` covers both). Predictions: stale drops **< 20** (was 65); drops whose next ask is
  `Land drop:` **0** (was 44); single-row takes with `chosen_text != options_text[choice-1]`
  **0 of N** (was 124/2,416); `choice: -1` records with no `chosen_text` **0** (was 6);
  `decision_reversed_in_prose` >= `plan_choice_conflict_narrowed` on every seat; per-stderr hold
  line count = sum of the two seats' `hold_windows_skipped`. NOT verified live: the second slot
  holding an answer across an arm switch; `mPromptTail` at all ~14 pollCompletionRetry sites.
- B (D6/D24/D22/D10): `INCOMING THIS COMBAT` in four forms (declared / settled / forecast
  `not declared yet - N of their creatures can attack, for up to M` / closed); windows during
  their combat with no line **0 of N** (was 209/218); NOTE the docket's cited `125v146` s65 is an
  OWN-turn Main 1 window — outside D6. D24 `- best case with every blocker assigned: you would be
  at K` (max-weight matching; tramplers/menace weight 0 -> `one legal assignment gets you to K`):
  lethal-header blockers windows requiring the seat to subtract **0**. D22 `BLOCKING THIS COMBAT:`
  header: declines-all-blocks-under-a-converter **< 1 of 1**. D10 B-lines nesting a gain **0**
  (was 10/36). Latch keyed on `observer->turn` — a two-combat turn mis-renders (unguarded).
- C (D7/D8/D11/D30/D21/D12): creature rows under THEIR converter priced x count
  (`{their converter: `): unpriced **0 of N**; discard rows with a computable dead verdict and none
  **0 of N** (was 357/426; legend says bare = UNPRICED, not dead); edict rows naming an
  already-targeted victim with no tail **0/3**; `{identical verdict right now to row ` renders
  >= 1 (D15's never did); cycle takes at opp life <= 6 **> 3 of 17**; all-dead menus with no
  menu-level line **0 of N** (stricter predicate than the docket's 85/197 — N will be smaller).
  Unobserved: `dynamicMagnitudes` on a hand card at cleanup; `ctx->spellTargetType` on the
  cast-mode menu; D12 firing live.
- D (D14/D26/D27/D28/D29): `Flip Side` rows for LAND-backed MDFCs on the AI seat **0** (108 of
  109 corpus rows; the 1 spell-backed row is KEPT — D33's `otherrestriction` gates make the toggle
  the only route to those casts); arrival clause (`{taking this row ` / `{it enters TAPPED `)
  three-valued — Pelakka Caverns is "enters tapped", not pay-3 (Oracle-verified); `[NAMED BY
  THEIR` per-cast clause; printed name leads (case-variant token not repeated); adjacent duplicate
  event lines folded as `(xN)` at the write seam (corpus had 1,467 runs of 2 / 150 of 3) — runs of
  identical adjacent events **0**. D13 MEASURED, not built: GAME LOG 60.8% of the mean prompt,
  17.9 of 30.8 MB; land-drop asks are the CHEAPEST prompt (5,588 chars) — the docket's land-drop
  scope reaches 0.37 MB, the 1,943 `ask` records hold 9.5 MB+ (lane-D.md table).
- E (D2/D9/D15/D19/D20/D23): pregame header counts MDFC back-face lands in BOTH tallies with the
  note `1 card is counted TWICE above, on purpose: ...`: headers omitting a `forcetype(land)` card
  **0 of N** (was 4/4) — detection reads the `autohand=` hand bucket, NOT `magicText` (lane W's
  note was wrong); X rows priced from the PRE-FLOATED pool (`potentialColorReach` is 0 there by
  construction — the old clause could never print): X rows with no fit clause **0 of N** (was
  40/40); `since turn N` opponent life trend on 100% of board frames (was 1/137); fetch colour
  positive form; `[<- ...]` marker on X cast rows, marked-row takes stay >= 6/9; `{feeds:}` counts
  hand converters. Pathway (land/land) deliberately untouched.
- F (D25/D38/D36/initMTGCI): dead-ability sweep before the garbage zone is freed (second UAF on
  `a->source` via `MTGGameZone::hasCard`; ASAN RED->GREEN; `WAGIC_NO_DEADREF_SWEEP=1` disable);
  orphan-chooser breaker in `userRequestNextGamePhase` (releases an AI-owned chooser with no
  decision in flight after 300 live refusals): prediction **0 SEGV, 0 phase-stuck games**; the
  replay loading stall floor no longer releases windows the record still uses (`0<name>`
  refusals 60 -> 2); six never-written `MTGCardInstance` members zeroed. NEW residuals: an
  Intruder Alarm + Thraben Doomsayer activation-loop livelock (146v123 stub exit 124 — NOT the
  phase-7 shape); 4th (stack-target click logged without its result digit) and 5th
  (engine-generated `choice` ordering) replay defects; D34's whose-board term not done.
- G (D42, human seat only): grouping key = name + every observable state; board pinned ungrouped
  while any chooser / stack item / declaration is live; targeted SPELLS now rim their board
  targets (`forcedBorderA/B` from `Spell::Render`); option "Stack identical permanents" default
  ON. No AI-seat prediction (render only); GuiPlay::Render −37..−50% at 39-55 cards, small loss
  below ~15. UNVERIFIED: console, touch, a human dividing damage across 3 members of a pile.
- H (D43 "measure"): `WAGIC_GPT_LOGWINDOW` unset/`full` byte-identical (352/352 assembled prompts);
  `kind:K`, `N` arms; records carry `log_window` + elided-turn count. THIS corpus runs unset —
  prediction: `log_window_kind` census on the control arm reproduces the wave-56 ask-kind shares
  (land-drop 10.1%) and `elided` is 0 on 100% of records.
- S/T/U (the vpk16 softlock, "screen frozen, selector immovable, indicator opponent on my turn,
  pause menu alive"): NOT reproduced (10 variations); two mechanisms match — a stranded AI-owned
  chooser/menu re-pinning `isInterrupting` every tick (F's breaker) and an unbounded in-flight
  model call (U's bound: request abandoned at deadline+grace, window at 2*deadline+grace;
  `abandoned_in_flight` fallback stamp). Predictions: `abandoned_in_flight` **0** in this corpus
  (the endpoint answers) and the `WAGIC_STALLPROBE` line never shows a held window > 1230 s; T's
  menu entry `Softlock: dump diagnostics and quit to menu` writes `softlock-<epoch>.txt` and stamps
  `#classification=softlock` — Vita: first softlock dump is the next play report's payload.

## Set C - flags in this binary (silent-instrument rule)
`WAGIC_GPT_LOGWINDOW` (H; unset = full) · `WAGIC_INFLIGHT_BOUND=0` (U, positive control) ·
`WAGIC_GPT_WEDGE=1` (U, dev emulator) · `WAGIC_NO_DEADREF_SWEEP=1` (F) · `WAGIC_HANG_GUARD=1`
(T, default off) · `WAGIC_STALLPROBE` (S, dev-gated) · `WAGIC_SINGLE_RNG=1` (E-56) ·
`WAGIC_GPT_BOARDINDEX=0` (usable) · `WAGIC_STALL_FLOOR=0` (R) · `WAGIC_GPT_NOTHREAD=1` ·
`aiseat <1|2>` / `cancelbutton` / `assertdisplaytoggle` / `softlockdump` / `hangspin` suite DSL.

## §D - the log-window A/B (owner ruling "measure"; lane H's design, wave57/lane-H.md)
AFTER this corpus: three arms, same pool, `-r 1`, **`-j 3`** identical across arms (at -j 6 the
median latency hits the timeout and 26% of decisions become `empty_reply` — saturation would be
read as a window effect), arms `unset` / `kind:3` / `4`. Metrics in the order 21 games can resolve
them: prompt composition (deterministic; also the arm-verification census), per-decision
inference, fallback rate, seat-reviewed misplays (~21 effective samples), win-rate BAND only
(SE ≈ 10.9 points — resolves a 30-point swing and nothing smaller). The harness pins NO seed
(`initRand(0)` -> `srand(time(0))`): arms cannot be paired game-for-game — a `WAGIC_SELFPLAY_SEED`
lever is a docket item. Rare-event A/B rule: no single favourable run is a result.

## Carried into the wave-58 docket
- Three suite runs terminated early with a SIGSEGV core in `GameStateDuel::End` ->
  `SAFE_DELETE(transcriptMenu)` while other lanes ran wagic concurrently (all lanes share one
  `~/.Wagic/`); clean on a quiet machine — a shared-state shutdown race worth its own item.
- Intruder Alarm / Thraben Doomsayer activation livelock (F); replay defects 4 and 5 (F); D34
  whose-board term + the chooser as a per-seat pointer (F/S); D33 spell back-face gates; the
  `can play <type>` legacy handler's actor bug for ~60 non-land entries (W); Runed Crown / Mantle
  dormancy (X); Pathway land/land coverage grammar (E); D6's two-combat-turn latch (B); D24 model
  audit for non-trample blocked-hits-face effects (B); `repeat_n` has no harvest consumer (A);
  `WAGIC_SELFPLAY_SEED` (H); T's hang guard unwinding through real engine frames (why it ships
  off); G's console/touch/divided-damage verification (first thing on vpk17); D13 narration
  budget = the A/B's outcome, owner's call after the measurement.
- Owner items answered this wave: D42 grouping (built, G), D43 log window ("measure", H + §D).
  Owner's open Vita reports: the vpk16 softlock (fixes F+U ship in vpk17 unverified against his
  hardware); "counters only once" (parked by him). vpk15/16 verdicts: "no apparent regressions ...
  subtly smoother even" (vpk15, c++14).
