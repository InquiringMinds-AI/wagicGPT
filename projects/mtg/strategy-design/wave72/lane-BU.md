# Wave-72 lane BU — the opponent-turn response window and the window model (M4, M5)

Base `0e4e6b9b8`. Worktree `worktrees/lanes/w72-BU`, branch `w72-lane-BU`.
Base binary kept at `~/.gatelogs/w72-BU-base-wagic` for every counterfactual below.

**Verdicts: M4 CONFIRMED and fixed. M5 REFUTED as an engine gap** — the windows the
item says are missing are issued, and three new suite fixtures pin them; the deck130
symptom has a different cause, root-caused below and docketed for wave 73.

---

## THE WINDOW MODEL (the decision this lane was asked to make)

1. **A window is one (turn, phase, board).** The model gets ONE model call for it. The
   casting ask (`FindCardToPlay` -> "Casting decision") is issued first and is the only
   seam that can offer a cast; the priority ask (`chooseOrderedAction` -> "Your legal
   actions") is the window's ask when no cast is legal.
2. **The model's own HOLD row closes the WINDOW, not the seam.** Its text is "Hold
   priority: pass now, and do not ask me again - this turn or later - until one of the
   rows above changes"; it names no seam, so a hold taken at either seam now suppresses
   the sibling seam's ask over the same turn, phase and byte-identical board. That is a
   window whose answer is already given, not a removed option.
3. **"Cast nothing right now" closes the casting question only**, so the priority ask
   still follows it — the two menus are disjoint by construction (`chooseOrderedAction`
   drops every hand cast as a dead end), and collapsing that pair would take cycling and
   activation rows the model has never been shown. Nothing else about either seam moves.

---

## M4 — the priority window and the sibling cast ask are both issued — CONFIRMED, fixed

**Repro (read-only, corpus `matchups-20260907-085638`).**
`1788789427-ai_baka_deck130-...-vs-ai_baka_deck125.jsonl` seq 24 is a `Casting decision`
ask at turn 14 UPKEEP offering `Cast Starstorm {r}{r}{x} {X pricing: max affordable X=4}`;
seq 25 is a `priority` ask at the SAME turn and phase whose rows are `cycling with Spark
Spray` / `cycling with Starstorm` / hold. Two model calls, one board.

**Measurement (the RED-on-base script,
`strategy-design/wave72/lane-BU-double-ask-red.py`).** Over the whole corpus, comparing
the `--- CURRENT SITUATION ---` block byte for byte (minus the carried-plan heading,
which moves *because* the first ask was answered):

```
ask/priority windows: 1982
same-window cast/priority DOUBLE ASKS: 44 (2.2% of windows)
     25  cast nothing (still two asks, by design)
     19  HOLD (collapses after the fix)
by seat: {deck130: 24, deck146: 13, deck123: 6, deck152: 1}
```

**Root cause (one line).** `AIPlayerGPT::holdHonoured` looks the hold up by SEAM
(`mHoldRows.find(seam)`) and returns "this question is owed" for the other seam of the
same window, so a row that promises "do not ask me again" was answered again inside the
same tick, over the same board — a wasted call AND a false surface (the doctrine's own
rule: the surface owes the model truth).

**Fix.** One pure predicate and one latch.
* `gptHoldCoversSiblingWindow(heldSeam, heldTurn, heldPhase, heldBoard, nowSeam,
  nowTurn, nowPhase, nowBoard)` — true only for the OTHER seam of the SAME window: same
  turn, same phase, byte-identical `serializeGameState()`. Same seam, moved board, moved
  phase, moved turn and "no hold taken at all" are all false, so every other window
  falls through to the existing per-seam row predicate UNCHANGED.
* `takeHold` records that window (`mHoldWindowSeam/Turn/Phase/Board`); `holdHonoured`
  checks the sibling arm first and, when it fires, counts
  `sibling_window_asks_skipped` (new gameend field, a subset of
  `hold_windows_skipped`, which is still incremented and still split cast/priority) and
  writes a DebugTrace naming both seams.

**RED on base.** The script above replays the base rule over the corpus: 44 same-board
double asks stand, 19 of them opened by a hold — those 19 make one call after the fix and
two before it. PARSETEST adds **9** checks (2 positives incl. the symmetric direction,
5 must-not-matches — same seam, moved board, moved phase, moved turn, no hold — and 2
echoes proving no row text moved and the plain cast decline is untouched):
base `~/.gatelogs/w72-BU-base-wagic` **5330 passed / 0 failed**, after **5339 / 0**,
delta exactly this lane's additions.

**Prediction (next corpus, falsifiable).** `sibling_window_asks_skipped` is non-zero and
roughly 45% of the same-window cast/priority pairs the script counts (19 of 44 here);
re-running `lane-BU-double-ask-red.py` on the next corpus shows the HOLD class at or
near **0** while the "cast nothing" class survives at its present rate; no window is
lost — `hold_windows_skipped` rises by exactly the same number
`sibling_window_asks_skipped` reports, decisions/turn falls by ~1% (19 calls out of
2,159), and hangs stay at 0.

---

## M5 — the seat gets almost no OPPONENT-TURN windows — REFUTED as an engine gap

**What the item asked for.** "Fixture RED on base: opponent attacks, seat holds
Starstorm-like instant with mana, the seat's model must be asked after attackers."
That fixture is **GREEN on base.** So are the two other seams the item names.

**Three new fixtures, all GREEN on base, registered in `_tests.txt` as regression pins:**

| fixture | how it is discriminating | base |
|---|---|---|
| `ai_opp_turn_after_attackers_w72bu.txt` | P2 holds **Kill Shot** ({2}{W}, *destroy target attacking creature*) over three Plains. It has NO legal target until P1 declares attackers, so no earlier window can pass it. | **PASS** |
| `ai_opp_turn_after_blockers_w72bu.txt` | P2 holds a synthetic probe carrying `restriction=opponentblockersonly` — castable in exactly ONE step of the whole game, the opponent's declare-blockers step. | **PASS** |
| `ai_opp_turn_end_step_w72bu.txt` | the same technique with `restriction=opponentendonly` — castable only in the opponent's end step; the assert is taken one step later, in their cleanup. | **PASS** |

(The two probe cards are `BU Blockers Window Probe` id 2100000990 and `BU End Step Window
Probe` id 2100000991 in `Res/test/lexicon/test_primitives.txt`, the default synthetic set.
A fourth variant, P2 holding a Wall of Wood so a blocker DECLARATION is actually issued
before the cast — the `chooseBlockers(); selectAbility();` path in `computeActions` — also
passes; it is not registered because the blockers pin above already covers the step.)

**Direct engine evidence (`WAGIC_ADVANCEPROBE=1` + a temporary probe inside
`LegalActionsOracle::hasInstantResponse`, both removed before the commit).** The window
is issued at every opponent-turn phase boundary at which `hasInstantResponse` is true;
where it is false the phase advances directly, and the predicate is honest about it.

**So why did deck130 see 4 opponent-turn windows in 353 decisions?** Two causes, neither
of them a missing window:

1. **Its mana is gone by the time the windows exist — an abandoned {X} announcement
   destroys the payment.** Reproduced on the base binary with a probe fixture (P2 holds
   **Starstorm** `{X}{R}{R}` over four untapped Mountains, P1 attacks): at the opponent's
   Main 1 window the seat pays — `AIPlayerBaka: AI attempting to pay a mana cost. Cost:
   {r}{r}{x}`, four Mountains tapped — and the cast then dies at
   `MTGPutInPlayRule: cast of Starstorm dropped after its X was announced (no longer
   castable - pool ); announcement cleared, card stays in hand` (the #W43-6 branch).
   The mana is not refunded and the lands are not untapped. The instrumented predicate
   reads `pMana=4` at Main 1 and `pMana=0, untapped=0, tapped=4` at combat begins,
   attackers, blockers, combat damage, combat ends, second main and end — so
   `hasInstantResponse` is FALSE for the whole rest of the turn and **every** later
   window, theirs and its own, is correctly never issued. That is exactly the seat's
   census (its one Main-1 window, then nothing).
   The same shape is in the LIVE corpus and was already half-seen: wave-70 #W68-BB (J5)
   records `130v126` s21-s31 writing **three `Paid {r}{r}{x} for Starstorm` lines for a
   spell that never went on the stack** — wave 70 fixed the NARRATION of that, not the
   lost payment.
   *Harness caveat, stated up front:* in the fixture the announcement is dropped by the
   suite's menu-default answering the ANNOUNCE_X menu, which is a known script-pump
   artifact; the live drop has a different trigger (a post-announcement decline). The
   DEFECT under test — a dropped announcement destroys the payment and taps the seat out
   — is the same on both paths, and the live witness above is independent of the harness.
   **Not fixed here**: a refund without a same-phase re-offer guard risks a
   pay/drop/retry loop, and this lane is not spending the wave's 0-hang result on it.
   Docketed below.
2. **`hold_windows_skipped` is invisible to a translog census.** deck130 held **255**
   windows across its six games against 361 asked decisions; a held window writes no
   record, so "4 opponent-turn windows in 353 decisions" counts asks, not windows. The
   corpus-wide opponent-turn ASK share is 298 of 1,982 (15.0%) and is a deck property,
   not an engine one: deck123 46.5%, deck125 15.7%, deck146 6.0%, deck126 5.0%,
   deck162 2.2%, deck130 1.1%, deck152 0.6%.

**Prediction (next corpus).** The three fixtures stay green. The opponent-turn ask share
does NOT rise on this wave's changes (nothing here creates a window), and any reviewer
re-opening M5 must first read `sibling_window_asks_skipped` + `hold_windows_skipped` and
the seat's untapped-source line — an opponent-turn window the seat cannot pay for is a
mana fact, not an engine fact.

---

## Docketed to wave 73 (new, HIGH)

**A dropped {X} announcement destroys the payment.** `MTGPutInPlayRule::reactToClick`'s
`setX > -1` branch clears the announcement and leaves the card in hand, but the mana that
was floated for it is gone and the sources stay tapped — the seat is mana-dead for the
rest of the turn and receives no further response window at all. Repro above (four
Mountains, Starstorm, `WAGIC_ADVANCEPROBE=1`); live witness `130v126` s21-s31. A fix must
refund the payment AND guard against the pay/drop/retry loop a refund enables (the
`mStuckCastLines` idiom is the model on the GPT side; the heuristic side has none).

## Gates

* Build: clean link; both link guards print OK (`check-ctor-init: OK (118 file(s))`,
  `check-reply-instructions: OK`).
* PARSETEST: base **5330 / 0 failed** -> after **5339 / 0 failed** (+9, exactly this
  lane's checks).
* Suite (`WAGIC_TESTSUITE_THREADS=1`, detached unit `w72-BU-suite`, log
  `~/.gatelogs/w72-BU-suite.log`): **1280 tests (0 failed), 76 AI tests (0 failed)** — 0 `==Test Failed !==`, 0 `==Test timed out`. AI tests 73 -> 76, exactly the three fixtures this lane adds; the BP pins `ai_mdfc_decline_livelock_w71bp` and `ai_mdfc_decline_sticky_w71bp` and the BQ pin `ai_instant_own_end_step_w71bq` are green, and the three known single-thread flakes (lifeline, intrepid_adversary_repeated_payment, merrow_reejerey) did not fire this run.
* `git diff | /usr/bin/grep -c $'\357\277\275'` = 0.

## Weakest evidence

The M4 fix's benefit is measured on the wave-71 corpus by REPLAYING the rule over
recorded prompts, not by running the changed engine: 19 collapses is a projection, and a
model that answers a slightly different menu next corpus can move it. The fix's SAFETY is
better evidenced than its size — the predicate is pure, pinned in seven directions, and
its false branch is the pre-fix code path byte for byte. On M5, the harness caveat on the
Starstorm repro is stated above and is the one place a reviewer should push: the live
witness (`130v126` s21-s31) is a wave-70 note read second-hand, not a record I re-read
end to end this lane.
