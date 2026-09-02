# Wave-53 lane R — Vita human-play reports (build vpk11, engine dddb6f62b)

Two owner-reported defects from the 2026-09-02 Vita session. Transcripts read from
`psp-work/logs/20260902-vpk11/` (read-only, unmodified).

---

## Report 2 — "it didn't allow me an opportunity to tap my creature for mana" — FIXED

**Owner, verbatim:** "i tried to pay for the legend shrine's ability to put counters on
shrines, but it didn't allow me an opportunity to tap my creature for mana after it asked
me if i wanted to use it."

Card: **Go-Shintai of Boundless Vigor**
`auto=@each my end:may name(Pay 1 and put counters) pay({1}) ... target(shrine|myBattlefield) counter(1/1,type:shrine:myBattlefield)`
The mana source he wanted: **Twitching Doll**, `auto={T}:add{G} && counter(0/0,1,nest)` (five
colour abilities, each carrying a side effect), with every land already tapped.

### Mechanism, in plain words

Answering "pay" arms `game->mExtraPayment` and `MenuAbility::Update` holds the window open
until the mana pool covers the cost. That hold has an escape hatch — the **unpayable
commitment release**: if *no* sequence of clicks could ever cover the mana leg, the engine
declines for the player rather than pinning the phase ring forever. It answered that
question with `ManaEngine::potentialMana`.

`potentialMana` walks `ActionLayer::manaObjects` with a plain
`dynamic_cast<AManaProducer*>`. But that index is **built** through
`AbilityFactory::getCoreAbility` (`GuiLayer::Add`) — so a producer *wrapped* by a `&&` side
effect (exactly Twitching Doll's shape) is in the index and invisible to the reader. The
index and its reader disagreed. With only such a creature untapped, the player's whole mana
capacity read **zero**, the release fired on the *same tick the window opened*, and the
prompt vanished before any click could reach the Doll. A land worked because a land's
producer is unwrapped.

A second, smaller gap sat next to it: the ordinary (no-X) pay branch of
`MenuAbility::reactToChoiceClick` never called `ManaEngine::autoTapForCost`, although the
X-announcement branch right below it does. A human who had not *already* floated the mana
had to know to click producers before answering the prompt — which is not how casting pays.

### Fix

`projects/mtg/src/AllAbilities.cpp`, two places:

1. `MenuAbility::Update`, unpayable-commitment release — replaced the `potentialMana` call
   with a **local, deliberately permissive** walk that reads the index the way it was
   written (`getCoreAbility`), counting every distinct untapped source on the payer's
   battlefield whose core ability makes mana. Any doubt now keeps the window OPEN.
   `potentialMana` itself is left alone on purpose: its one-ability-per-card, cost-checked
   semantics are pinned by the oracle/gate fixtures, and widening it there **did** regress
   them (`oracle_selftap_westvale_5_lands`, `oracle_ability_dual_not_double_4_lands`,
   `bestow_no_envoy_human_gate_lands`, plus 2 AI tests — measured, then reverted).
   Sources are matched by POINTER against the battlefield array, not `hasCard()`:
   `manaObjects` also holds parse-time template producers whose source card has id 0, and
   `hasCard()` answers those by id — counting one of them kept
   `generic/fade_away_unpayable_pay` hanging (measured, then fixed).
2. `MenuAbility::reactToChoiceClick`, non-X pay branch — auto-tap the human's free untapped
   producers on the "pay" commitment, mirroring the X branch. AI seats keep their own planner.

### Suite pin

`projects/mtg/bin/Res/test/w53_pay_interrupt_creature_mana.txt` (registered in `_tests.txt`).
Human seat holds Twitching Doll (untapped, the ONLY mana source) + Go-Shintai; at the end
step it answers the may, clicks the Doll, picks a mana ability, targets the shrine.
- **RED on base** (`archives/wagic-dddb6f62b-w52corpus`): `assertpt 2/2` got `1/1` — the pay
  window was already gone when the click landed and the mana floated unspent.
- **GREEN after**.

---

## Report 1 — "the heuristic ai got stuck targeting a spell, it cycled through the targets
endlessly" — NOT FIXED (investigation only)

Transcript `transcript-1788327409-player_deck7-vs-ai_baka_deck154.txt`, human p1 deck7 vs AI
p2 deck154, abandoned on turn 11 at 13/2 with no result line.

### What the evidence says

- The stall is on the **human's** turn 11. After `p1.next 1 6` the AI clicks
  `p2.battlefield[3] 1dominating vampire` and `p2.battlefield[8] 1odric, blood-cursed`
  alternately, ten times, all with click result **1** (something reacted each time). It then
  stops answering interrupt windows entirely, and `p1.next 1 9/10/11/12` are logged as
  *effective* phase requests that never roll the turn. That signature — phase ring pinned,
  the human's clicks landing on a red target-selection border over his own creatures — is an
  armed menu/target chooser that its owner never answers.
- `psp-work/logs/20260902-vpk11/fizzlelog.txt` contains **no** Dominating Vampire resolve
  line anywhere in the session: the ETB never resolved, not even when it was cast on turn 7.
- Dominating Vampire was cast turn 7 (`p2.hand[2]`), Odric, Blood-Cursed turn 10
  (`p2.hand[2]`). Odric's arrival is what raises the Vampire count from 1 to 2 and therefore
  what first makes the human's Mossborn Hydra (MV 2) a legal target for
  `target(creature[manacost<=type:vampire:mybattlefield])`.

### Ruled out, with evidence

- **Replay is unusable here.** `WAGIC_REPLAY` on this transcript diverges at action 49,
  turn 4 phase 4 (`expected 'p1.hand[6] 1forest' got 'p1.next 1 3'`, the hand is one card
  short of the Vita's) — long before the stall, so the live board cannot be rebuilt from it.
  (This is itself a real replay defect on a human-vs-Baka console dump; it is the first such
  dump, and verifying that path was already flagged as unverified.)
- **The ETB "may" is one-shot and drops itself cleanly when no target is legal.** A bare
  `auto=` carrying a `target(...)` on a permanent parses to
  `MayAbility(must=true){GenericTargetAbility}` (`MTGAbility.cpp`, the `!activated && tc`
  fallback). Fixtures run on the base binary: (a) AI casts Dominating Vampire with legal
  targets — `MAYPROBE ... valid=1`, ability resolves, creature stolen; (b) same with every
  opponent creature above the Vampire count — `MAYPROBE ... valid=0`, the may returns
  early and is destroyed, **no stall**; (c) DV + Odric seeded together against a
  reconstructed turn-11 board — resolves, **no stall**; (d) DV with no legal target followed
  by a later Vampire making one legal — **no stall**. Four fixtures, no livelock.
- `getIndexOf(NULL)` returning a live index (which would strand the may forever) — checked:
  `GuiLayer` never stores NULL in `mObjects`, so `testDestroy` does destroy it.

### The candidate I could NOT arm in a fixture (stated as a hypothesis, not a finding)

`MayAbility::testDestroy` returns 0 while **any** `menuObject` is armed, and
`MayAbility::Update` only ever fires once (`triggered`). A may that trips its early
"no valid targets" return on a tick where some *other* card's menu is open therefore stays
registered forever with `triggered == 1` — never updating again, but still answering
`isReactingToTargetClick` as soon as targets become legal later. Combined with
`AIPlayerBaka::computeActions` / `chooseTarget` both returning 0 outright when the seat is
not `currentlyActing()`, a chooser armed for the AI during the human's turn can never be
answered — and `ActionLayer::Update` early-returns on `menuObject`, which pins the phase
ring exactly as observed. `MayAbility::reactToTargetClick` also overwrites `mClone` on every
click without destroying the previous one, which would orphan a chooser per click.

I did not ship a change for this. It is a code-reading hypothesis that I could not
reproduce on the current binary, and the project's own rule is to reproduce before fixing
(a speculative menu-zombie fix has already been shown redundant once). The next step that
would settle it is a Baka-vs-Baka or human-seat run instrumented to print, per tick,
`menuObject` / `currentWaitingAction` / `currentlyActing` and to flag any menu armed for a
seat that is not the acting player — cheap to add, and it converts the hypothesis into
either a repro or a refutation.

**Also noted while reading the transcript (not investigated):** on turn 10 the AI clicked
`p2.battlefield[8] 0chandra, dressed to kill` 13 times in one turn and once more after the
phase change, every one with click result **0** — a second, non-fatal AI re-click loop on a
planeswalker whose loyalty ability was already spent.

---

## Gate (worktree `worktrees/lanes/w53-R`, branch `w53-lane-R`)

Baseline re-confirmed on this worktree before any change:
`1210 tests (1 failed: lifeline.txt), 44 AI tests (0 failed)`; PARSETEST `2121 passed, 0 failed`.

Final, after the change:
- Suite: **1211 tests (2 failed), 44 AI tests (0 failed), 0 timeouts** — the 2 are exactly
  `lifeline.txt` + `merrow_reejerey.txt`, the known concurrency-only pair named in the lane
  brief. +1 test = the new fixture.
- PARSETEST: **2121 passed, 0 failed** (no parse/render strings touched).

## What I did NOT verify

- **The Vita is not rebuilt by me.** No VPK was produced; the fix is on the desktop build
  only. Port builds are owner-initiated.
- The report-2 fix was not observed in the real GUI or on hardware — only headless fixtures.
- The `autoTapForCost` addition at the pay commitment is gated on `!isAI()`, and BOTH suite
  seats are AIPlayer-derived, so no fixture can exercise it. I verified the call site with a
  throwaway positive control instead: with the gate temporarily forced true and a Forest as
  the only source, the payment completed with no producer click at all (`assertpt 2/2`
  passed); the gate was then restored and rebuilt. The same control with Twitching Doll did
  NOT complete — `ManaEngine::planPayment`/`selectAutoTapProducers` share the wrapped-producer
  blindness, so auto-tap still cannot see a `&&`-wrapped producer. That is a known residual:
  such a source must be clicked by hand (which now works). Fixing the planner walks was
  deliberately left out of this lane — it changes AI payment behaviour and needs its own pin.
- Report 1 is unfixed and its mechanism unproven; see above.
