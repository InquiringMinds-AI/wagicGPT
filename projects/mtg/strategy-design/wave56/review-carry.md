# Wave-56 review carry-list (assembled 2026-09-03 at corpus prep)

Corpus: thirteenth fair-hand corpus. Binary = hermetic `make -B` of master **d0515fae3** (wave-56
step-1: lanes A 7a2e243c7, B 5c391be1e, C 7193e4bb5, D e60a7c15d, E 039af56c6 on base 41d1c15ef
= wave-55 step-1 b228b8648 + wave-55 review/synthesis; merged as 6aa8341d9 A, cc7acb04e B,
c7f76e299 E, 256ab5eb4 D, d0515fae3 C; f9a39205b = O7 Vita `-std=c++14`, CMakeLists only).
Gate on d0515fae3: suite 1234 (2 failed = lifeline + merrow_reejerey, the known concurrency-only
pair), 51 AI/0, 0 timeouts, PARSETEST 2736/0 (2636 + A 17 + B 49 + D 8 + C 26; lane E +0).
Archived `archives/wagic-d0515fae3-w56step1`. Model: qwen36-35b-a3b on Spark :8084. Guides =
wave-55 reviewer revisions (860528eb5) + wave-55 boundary pass (d948447ab, B1/B3/B4 edits on
deck125/146/152/130). Same pool/recipe as waves 54-55 (-j 21, -T 0, WAGIC_GPT_TIMEOUT=900,
MAXTOKENS=6000). Vita: vpk14 (860528eb5) is the owner's live build; vpk15 (6e8a7782c = vpk14 +
`-std=c++14` only) is built, archived, awaiting upload — the O7 A/B; wave 56 ships as vpk16.

## Discipline (owner doctrine 2026-08-26)
Reviewers own guides; synthesis owns skill + general; the core loop is the purpose. Adjudicate
render predictions by the emitter's actual string — COUNT FROM THE RENDERED PROMPT, never from
the translog `options` array. NEW (wave-55 boundary pass, silent instrument): the HOLD row's
`{taking this row ...}` tail is on 2,352/2,352 `options_text` rows but MISSING from 134 of 282
`chosen_text` takes — a HOLD take census must PREFIX-match the row, never exact-match (exact
read 76 at N>=2 where the true figure is 88). Baselines from wave 55 (21/21 natural, D1 of wave
55 DISCHARGED — 0 stall-floor releases): 3,171 decisions, 14 fallbacks, 0 timeouts, 43 stale
drops (turn-flip seam alternation), 0 heuristic handoffs, HOLD row 2,352 renders / 282 takes
(113 of 282 takes re-asked byte-identical same turn = the D1 re-open class, 12.4% of inference),
`hold_windows_skipped` 21/42 seats, ability rows `they would be at K` with K == 166 on 238/248.

## D1 discharge (lane A) — the wave's first question
Hold predicate = rendered rows byte-for-byte (`holdStillStands`), price included; the
declined-note key (`optionSetKeyOf`) is deliberately divergent. Predictions: byte-identical
same-turn re-asks after a HOLD take **< 20 of N** (was 113/282); decisions inside one mandatory
loop phase **< 5**; identical-declined runs **< 15%**, max run **< 10**; inference spent in
runs **< 6%** (was 12.4%). Stated, unmitigated risk: incoming lethal under an unchanged menu
stays held — count any game lost while a hold stood with a legal answer (expect 0; report N).
D18: `AsyncState::slotKey` = (forceClose, turn, phase, seam tail, board) — a drifted-prompt
answer on a matching slot is CONSUMED (new stderr line), a seam change still drops; stale drops
**< 20** (was 43). Lane A audited neither `mPromptTail` at all ~14 `pollCompletionRetry` sites
nor D18 firing live — a rescue that never fires shows as drops unchanged, not as an error.

## Set B - engine changes since the wave-55 corpus, each with its falsifiable prediction
- A (D1/D18/D16/D11/D4): above, plus D16 re-ask needs `proseNamesOtherMenuRow` — broad census
  kept (`decision_reversed_in_prose`), new `plan_choice_conflict_narrowed` stamp;
  `plan_choice_conflict` **<= 4** (was 9). D11 plan withdrawn at 40 windows, retraction served
  alone when the note fires on a plan > 4 windows old: plan echo max **< 60**, over-age served
  plans **0**. D4 `chosen_text` never empty while `choice >= 0`: **0** such records (was 134 of
  282 tail-less — see the prefix rule above; lane A's D4 and the boundary pass's finding are the
  SAME defect seen from two sides: adjudicate together).
- B (D2/D6/D9/D10/D13/D14/D15): D2 ROOT CAUSE = `AIAction`'s player-ability ctor never set
  `player` (indeterminate; the `dynamic_cast` fallback never ran; `->life` at the garbage offset
  read the constant 166). New `AIAction::targetedSeat()`; fixture `w56b_ability_row_seat.txt`
  (`assertabilityseat`) RED with only the initializer reverted. Predictions: ability rows with
  `K != opp_life - damage` **0 of N** (was 238/248); a lethal face row reachable from an ability
  menu >= 1. D6 `INCOMING THIS COMBAT` total on 100% of opponent-combat windows with attackers
  (was 0/50; trample carry-over past an assigned blocker deliberately under-claimed). D10 opponent
  open-mana count on 100% of prompts. D9 plans asserting an enumerating clause removes > 1 object
  **0**. D13 blockers B-lines with the gain nested inside the survival verdict **0** (attackers
  window still nests — not a D13 miss). D14 equipment casts taken at `of which 0 are creatures`
  **0** (was 1/3). D15 takes of the dearer of two same-card/same-verdict rows **0** (was 1/1).
- C (D3/D12/D7/D17): D3 `[RE-ASK]` notices whose quoted `CHOICE: n` differs from the row the
  sentence names **0 of N** (was 2/8); `plan_choice_conflict_recovered` where the recovered row is
  a pass while the latched row was live **0**. D12 `reveal_stall` on a reveal answered inside the
  poll-churn floor **0** (was 12/12); `reveal_wait_ticks/_secs` on 100% of reveals that waited.
  D7 X rows with no mana-fit clause **0** (was 11/11), monotone menus with no marker **0**; if an
  ANNOUNCE_X answer lands out of range the collapse is first suspect (`kMonotoneXCollapseFloor`).
  D17 REFRAMED: `WAGIC_GPT_BOARDINDEX=0` was never causal — the SEGV is a dangling
  `MTGAbility::target` (garbage zone `SAFE_DELETE`d two turns later while abilities naming its
  cards sit in `ActionLayer::mObjects`), base binary crashed 2/5 at BOARDINDEX=1 too. Fixed at
  the deref (`liveCardTarget`/`livePlayerTarget`, 5 sites + `abilityCanReactTo`), NOT at the
  ownership — `a->source` is read unguarded in the same loop. Prediction: **0 SEGV** in 21 games
  (wave 55: 0 observed, but the stub rate was 2/5 on one matchup); stub game at BOARDINDEX=0
  exits 0 (met 6/6) — the lever is USABLE for the first time.
- D (D8): CARD SCRIPT, not engine — 55 MDFC land-back primitives in borderline.txt gated the
  `{0}` hand entry on `isflipped == 1`, set only by the manual Flip Side toggle (two clicks;
  the AI's first click was eaten). Gate clause dropped. Predictions: back-face land rows **> 0**
  (was 0 of 3,171) and ARRIVAL (`resolved and entered` of a back face) in >= 1 record per
  affected deck (146: Emeria's Call, Agadeem's Awakening, Pelakka Predation; 152: Branchloft,
  Barkchannel, Hengegate; 145/147/151/34 out of pool); `Flip Side` takes on a modal-DFC land
  **0** (was 11, all deck146); fewer land-drop-less turns of the 146v130 seq 22/23/24 shape; no
  two-lands-in-a-turn via this route. Three fixtures RED on base primitives alone. Render:
  `mdfcLandPlayRowTag` feeds both emitters; two "only the currently-shown face" tails and the
  "gains you nothing playable" note corrected. REVIEWER ACTION (deck146 seat): the "NOT LANDS
  FOR YOU" block (skill #238's P10 prediction) must RETIRE on this corpus's evidence; deck152's
  MDFC prose likewise. The 30 `otherrestriction` SPELL back-face gates are untouched (working
  two-step, no repro). Console builds keep the gated script until `cardauto.{idx,dat}` are
  regenerated by a port build (vpk16).
- E (O8 RNG split + replay): `GameObserver::aiRandomGenerator` seeded `gameSeed ^ 0x9E3779B9`;
  `AIPlayer::getRandomGenerator()` returns it (the per-seat member it replaced was time+address
  seeded and never logged); three game-stream AI draws re-pointed; four libc `rand()` sites
  re-homed (Rules.cpp commander load also RESEEDED the process stream); `WRand` deleted;
  `Tournament::leaveOutAIvsAIMatches` got a file-static generator (no observer). NO fixture
  re-baselined (measured, not assumed). Replay: engine-generated auto-tap clicks parked in
  `mReplayEngineActions` (WAGIC_REPLAY only): lane AA g2 dump ABORT -> 147/147 complete;
  deck5-vs-deck95 ABORT -> 295/314; deck3-vs-deck164 94 -> 121; all three byte-identical under
  `WAGIC_SINGLE_RNG=1` (the gains are the replay fix). Prediction for THIS corpus: none that a
  translog can see (AI seats consume nothing from the game stream while loading); the corpus is
  the shakedown for unintended draw-order change — win-rate per matchup within the wave 53-55
  band, 0 new fixture-class anomalies. NOTE the standing "22-turn Baka-vs-Baka game replays to
  the identical end state" claim did NOT reproduce on the base binary (6/6 diverged; residual =
  a recorded mana click inside an interrupt window comes back `0<name>` refused) — a THIRD replay
  defect, docket it.

## Set C - flags in this binary (silent-instrument rule)
`WAGIC_SINGLE_RNG=1` (E) · `WAGIC_GPT_BOARDINDEX=0` NOW USABLE (C; was a crash, never causal) ·
`WAGIC_STALL_FLOOR=0` (R) · `WAGIC_OPT_SOURCES_CAP=14` / `WAGIC_OPT_WORK_BUDGET` (Q) · lane A-N
flags per wave54/review-carry.md §C · `WAGIC_GPT_DRIFT=1`, `WAGIC_GPT_NOTHREAD=1` (dev builds;
NOTHREAD emulates the Vita's thread refusal — the Vita seat runs the fallback path for EVERY
decision, verified 2026-09-03 against gpt-log.txt).

## Carried into the wave-57 docket
- Lane C's SECOND defect (base, unfixed): phase-7 LIVELOCK — both seats emit
  `AIPlayerGPT[ph7]: only display-toggle (Flip Side) options; auto-passing without a model call`
  forever (65 MB of one line, phase never advances). Repro = lane C's D17 stub command without a
  `timeout`. MDFC/Flip-Side neighbourhood of D8 — lane D's fix may or may not touch it: COUNT
  `only display-toggle` lines per game in this corpus (a run > 100 in one window = the livelock).
- `MTGAbility::target` ownership (C fixed the deref only; `a->source` unguarded; dead abilities
  stay in `ActionLayer::mObjects`). The third replay defect (E). The 30 spell back-face gates (D).
  Baka took the new back-face land row 0 times in a stacked game (heuristic-scorer item).
  Attackers-window nesting (B D13 residual). D18 rescue never observed firing (A).
- Wave-55 boundary obligation 2: deck130 :779 carries a second live dead quotation of the wave-54
  HOLD string (0 renders) — no finding cited it; next boundary pass, replacement string in
  wave55/reviewer-boundary-pass.md. Literal audits are a COUNT per file, not a first hit.
- Vita: vpk15 (c++14 flag-only A/B vs vpk14) awaiting the owner's upload window + play; O11
  SETTLED from the vpk13/14 memlog (avg_swp median 0.1 ms, max 0.5 — swap is not the cost;
  update phase is; 22/506 turns over the 16.7 ms budget) — the 30 fps cap expression is moot,
  A3-class update spikes are the residual. Owner's "counters only once" vpk13 report: card
  unnamed, Survivalist RULED OUT (grew under NOTHREAD on every Ally incl. Kor Ally tokens),
  parked by the owner. FTP listing of ux0:/data/Wagic/ai/gpt/ timed out — do not retry blind.
