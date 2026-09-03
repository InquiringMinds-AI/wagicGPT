# Wave-56 lane C — parser, records, X rows, the broken lever

Branch `w56-lane-C`, worktree `worktrees/lanes/w56-C`, seeded from master 41d1c15ef
(src = gated b228b8648). Docket items D3, D12, D7, D17 from `wave55/engine-ledger.md`.

---

## D17 (PROCESS) — `WAGIC_GPT_BOARDINDEX=0` segfaults. IT IS NOT THE BOARD INDEX.

### The repro

`tools/gpt-stub-server.py --answer "CHOICE: 1"`, one `WAGIC_SELFPLAY_ONESHOT` game,
`WAGIC_SELFPLAY_DECK0=146 WAGIC_SELFPLAY_DECK1=152`, `WAGIC_AI=gpt`,
`WAGIC_GPT_BOARDINDEX=0`, on the **base** binary: `EXIT=139` at turn 12. Reproduced.

### The backtrace (gdb, base binary, same setup — crashed at turn 8 on that run)

```
Thread 1 "wagic" received signal SIGSEGV, Segmentation fault.
0x00007ffff78b3f55 in __dynamic_cast () from /usr/lib/libstdc++.so.6
#0  __dynamic_cast ()
#1  AIPlayerBaka::rankActivations (this=..., ranking=std::map with 0 elements,
       totalPotentialMana=..., Specific=0x0, withCard=0x0, costPlans=0x0)
       at src/AIPlayerBaka.cpp:2649
#2  AIPlayerBaka::selectAbility ()          at src/AIPlayerBaka.cpp:2354
#3  AIPlayerBaka::computeActions ()         at src/AIPlayerBaka.cpp:4069
#4  AIPlayerGPT::computeActions ()          at src/AIPlayerGPT.cpp:21678
#5  AIPlayerBaka::Act (dt=0.1)              at src/AIPlayerBaka.cpp:5136
#6  DuelLayers::Update / GameObserver::Update / GameStateDuel::Update / JGE::Update / main
```

Line 2649 was
`MTGCardInstance * cands[2] = { a->source, dynamic_cast<MTGCardInstance*>(a->target) };`.

### The root cause

`MTGAbility::target` is a raw `Targetable *` with **no clearing contract** — nothing
nulls it when the object it names is freed. Dead tokens and copies reach
`MTGPlayerCards::garbage`, and the zone that was `garbage` two turns ago is
`SAFE_DELETE`d (`MTGGameZones.cpp:230-241`), which deletes its cards — while abilities
that named them are still in `ActionLayer::mObjects`. `AIPlayerBaka::rankActivations`
walks **every** one of those objects on every ranking pass, and `dynamic_cast` reads a
vtable pointer out of freed memory. `__dynamic_cast` faults.

**The disable flag is not causal.** Five stub games per arm on the BASE binary,
same matchup:

| arm | exit 139 | exits 0 |
|---|---|---|
| `WAGIC_GPT_BOARDINDEX=0` | 4 of 5 (turns 10, 14, 12, 15) | 1 (turn 18) |
| `WAGIC_GPT_BOARDINDEX=1` (the shipped default) | **2 of 5** (turns 15, 10) | 3 (turns 21, 17, 12) |

So the **shipped** path crashes this matchup too, and lane L's lever correlates with
nothing but how many decisions get taken before the dead pointer is met. The wave-55
ledger's suspect ("the legacy per-host scan path") is refuted: the fault frame is in
`AIPlayerBaka`, which the board index does not touch. Two waves of "no reviewer could
A/B the board index" were a *latent engine crash* misfiled as a broken lever.

### The fix

The dynamic **type** is never the question at any of these sites: each asks "is this
pointer one of the cards that exist right now". That is answerable by **pointer
identity** against the live zones, which dereferences nothing. `(Targetable *) card`
is a compile-time upcast (`MTGCardInstance` reaches `Targetable` once, through
`Damageable`, non-virtually), so the comparison is exact: identical to the
`dynamic_cast`'s answer for every live pointer, and `NULL` — which every call site
already handles — instead of a fault for a dead one.

`AIPlayerBaka.cpp` gains `liveCardTarget(GameObserver *, Targetable *)` (scans both
players' inPlay/hand/graveyard/library/exile/commandzone/stack/reveal/temp/sideboard/
garbage/garbageLastTurn — the same set a `dynamic_cast` could resolve) and
`livePlayerTarget`. Five `dynamic_cast<MTGCardInstance*>(x->target)` sites are routed
through them: `regenerationPendingFor` (:203), both `OrderedAIAction::getEfficiency`
overloads (:248, :965 — reached from `rankPair` even after the scan is safe),
`AIPlayerBaka::doAbility`'s OrderedAIAction factory (:2896), and `rankActivations`
(:2649). `abilityCanReactTo` (:2542) becomes a direct pointer comparison.

### The A/B (`timeout -k 10 240` per game, so a hang is `EXIT=124` and not a hang)

Six stub games per arm, deck146 vs deck152, archived base `wagic-b228b8648-w55step1`
versus this lane's binary:

| binary | flag | exit 139 | exit 124 | exit 0 |
|---|---|---|---|---|
| base b228b8648 | `BOARDINDEX=0` | **3** (t14, t12, t14) | 1 (t11) | 2 (t17, t16) |
| w56-lane-C | `BOARDINDEX=0` | **0** | 0 | **6** (t17, 20, 23, 23, 25, 18) |
| w56-lane-C | `BOARDINDEX=1` (default) | **0** | 0 | **6** (t17, 17, 19, 16, 38, 16) |

**D17's gate is met**: a full stub selfplay game with `WAGIC_GPT_BOARDINDEX=0` completes
exit 0, 6 of 6 — and the shipped default path, which was also crashing, is fixed by the
same change.

### A SECOND, SEPARATE defect found on the way (not fixed here)

The base's `EXIT=124` at turn 11 is a **livelock**, and the fixed binary reaches it too
when run uncapped: both seats sit in phase 7 emitting
`AIPlayerGPT[ph7]: only display-toggle (Flip Side) options; auto-passing without a model
call` forever — one run wrote **65 MB** of that one line and never advanced the phase.
It is present on the BASE binary (that is where the 124 came from), so it is not this
lane's, and it is not in this lane's docket. It belongs on the wave-56 ledger: an
MDFC/"Flip Side"-only window where every seat auto-passes without a model call and
nothing requests the phase advance. Lane D is in the neighbouring code (D8, the MDFC
land back face) and should be told; the reproducer is exactly the command in "The repro"
above, run without a `timeout`.

**Gate:** see the table above and the suite table below.

---

## D3 (HIGH, R227) — the `[RE-ASK]` notice quotes the LATCHED line and reads its region

`130v123` seq 109: first coded line `CHOICE: 0 (pass)` with "I will pass" beside it,
400 words, then `CHOICE: 5 (cycling with Starstorm)`. The engine latched and executed
row 5 — **correctly** — then read the first line's pass prose as a contradiction of it,
quoted the discarded line under the latched line's row number, re-asked, and the second
reply passed. Recorded as `plan_choice_conflict_recovered`.

Two changes, both in `AIPlayerGPT.cpp`:

* `AIPlayerGPT::latchedCodedChoiceLine()` — the same walk as `codedChoiceOrdinal`, but
  reporting the latched line's **text** and its **offset** in the post-think reply.
* `planSaysPassThisWindow()` takes a `fromStripped` offset. The priority seam passes the
  latched line's offset, so both the `plan:` scan and the post-CHOICE sentence scan start
  at the line the engine actually ran. A verdict written *before* the answer the engine
  took is evidence about a line the engine did not run — it no longer fires the detector,
  and it no longer supplies the quote.

Fallback is exact wave-55 behaviour: when no coded line re-parses to the executed choice
(a salvaged or name-resolved answer), the quote is `firstLabelledLine` and the scan is the
whole reply. A one-answer reply is byte-identical (the latched line *is* the first line,
offset 0). The same `quotedChoiceLine` now feeds the repeat-count and `plan_missing` arms,
which had the same first-line-quote defect on a two-answer reply.

**Prediction.** `[RE-ASK]` notices whose quoted `CHOICE: n` index differs from the row
number the same sentence names: **0 of N** (wave 55: 2 of 8).
`plan_choice_conflict_recovered` records whose recovered row is a pass while the latched
row was live: **0** (wave 55: 1). Corollary: `decision_reversed_in_prose` should FALL,
because the two-coded-answer shape no longer scores it from the discarded region.

---

## D12 (MED, R236) — the reveal wait is a wait; `reveal_stall` is the guarded case

Wave 55 stamped the triple on **12 of 12** reveal records, with `reveal_stall_secs`
equal to `latency_ms / 1000` on all twelve and `reveal_stall_phase` a constant 0. The
condition was `mAIStallStructTicks > 0` — true the moment the model starts thinking, so
it selected *every* reveal, never a park.

* `AllAbilities.cpp` gains `revealStructParked(ticks, secs, fixtureTicks, deadlineMs)` —
  the structural threshold as **one** predicate, and `driveInteractiveReveal`'s
  force-close now calls it too, so the figure the seat stamps and the figure the guard
  acts on cannot be two different tests.
* `Player::noteRevealStall` takes a fourth argument, `parked` (the driver's own verdict).
* The record now carries **`reveal_wait_ticks` / `reveal_wait_secs`** — named for what
  they are — whenever the driver waited at all, and `reveal_stall` only as a **boolean
  `true`**, present only when the threshold was crossed, with `reveal_stall_phase`
  riding that case alone (the one case where the phase can vary and is diagnostic).

**Prediction.** `reveal_stall` present on a reveal answered inside the poll-churn floor:
**0** (wave 55: 12 of 12). `reveal_wait_ticks`/`reveal_wait_secs` present on **100% of
reveal records that waited for a model answer**, and still equal to `latency_ms/1000` —
that equality was never the defect; the *name* and the *claim* were.

**Fraction reported (as the brief asks):** no reveal fired in this lane's own stub games
(the stub takes `CHOICE: 1` and deck146/deck152 produced none), so the fraction is pinned
structurally in PARSETEST rather than measured live — the wave-56 corpus is the first
real read. See "what I did NOT verify".

---

## D7 (MED, R231) — the monotone X family

Three parts, all in the ANNOUNCE_X seam.

**(a) the mana-fit clause.** The cast row deliberately prints none — its own comment
says why: *"an {X} cost has no remainder yet: X is announced AFTER this pick"* — and the
ANNOUNCE_X menu never printed one either, so the single real trade-off on a monotone X
menu (what taking the big X leaves up) was on **no screen at all**. It is now computed
per row exactly as the cast row computes its own: resolve the cost for THIS row's X the
way `MTGPutInPlayRule` does (copy, add the announced total as generic — or as the X
colour for a coloured X — then clear the X flag), ask
`ManaEngine::selectAutoTapProducers` which sources it would spend, and emit the cast
row's own `leavesUntappedTag`. Counted per SOURCE CARD, so it reads against the same
total the mana line prints.

**(b) the fourth marker family.** The other three rank a KILL dimension. 11 of wave 55's
14 X menus were Sphinx's Revelation (`auto=life:X && draw:X`) — strictly increasing,
no kill dimension — and carried no marker of any form. `xMonotoneMarker` states a FACT
about the whole menu on the largest affordable X:
`[<- largest affordable X - X=12 gains 12 life and draws 12 cards; no listed X does more]`.
It rides the existing one-row-marked plumbing, is bracketed (so it never enters history),
and deliberately does **not** instruct — holding mana up is exactly the trade (a) now
prices, and the taken X is deck125's guide rung, not this render's business.

**(c) the monotone run collapse.** A 13-row Sphinx's Revelation menu spent eleven rows
restating one sentence. `splitMonotoneXRow` recognises the row by two anchors (a leading
`X = <n> ` and a `{X pricing: X=<n>` naming the SAME n — nothing else in the corpus can
match) and reduces it to its X plus the row with every digit run replaced by `#`; a run
is rows whose X falls by exactly one and whose digit-erased text is identical. Four or
more such rows print as ONE range row that quotes **both ends verbatim** and states the
decode, and `kOptionRangeNote` gains the sentence that decodes it. Nothing is deleted:
every number in the range is still a separately choosable option and its X is arithmetic
the row states. The marker (b) breaks the run at the top and the `X = 0` special row
breaks it at the bottom, so what collapses is exactly the identical middle.

**Prediction.** X rows carrying no mana-fit clause: **0** (wave 55: 11 menus of 11).
Monotone X menus carrying no marker: **0** (wave 55: 11 of 11). An X menu with more than
4 identical-middle rows renders as `1.` + one range row + the tail rows, and the pilot's
answers on such a menu stay inside the offered range (no out-of-range answer, no
`unparsed_reply` on an ANNOUNCE_X record). **No prediction is made about the taken X** —
that is deck125's rung, and the corpus's 11/11 CORRECT stands.

---

## Gate (this worktree, hermetic `make -B -j4`)

| | base b228b8648 | w56-lane-C |
|---|---|---|
| build | clean | clean (`make -B`, rc 0) |
| suite (`WAGIC_TESTSUITE_THREADS=1`) | 1229 / 0 failed | **1229 / 0 failed** |
| AI fixtures | 51 / 0 | **51 / 0** |
| suite timeouts | 0 | **0** |
| PARSETEST | 2636 / 0 | **2662 / 0** (+26: D3 6, D7 15, D12 5) |
| stub selfplay, `WAGIC_GPT_BOARDINDEX=0` (6 games, 240 s cap) | 3 x exit 139, 1 x exit 124 | **6 of 6 exit 0** |
| stub selfplay, `WAGIC_GPT_BOARDINDEX=1` (6 games, 240 s cap) | 2 of 5 exit 139 (uncapped 5-game arm) | **6 of 6 exit 0** |

`git diff | grep -c $'\357\277\275'` = **0** (no U+FFFD; all six files byte-spliced).

---

## What I did NOT verify

* **No live model ran.** Every game in this lane used the stub server
  (`--answer "CHOICE: 1"`), so no D3 `[RE-ASK]` notice, no D7 X menu and no D12 reveal
  record was produced by a real reply. All three are pinned in PARSETEST and by
  construction; the wave-56 corpus is their first live read.
* **D12's fraction is unmeasured.** No reveal fired in this lane's games, so the
  "fraction of reveal records carrying the new fields" the brief asks for could not be
  taken. The predicted values are structural.
* **D7(a) has never been observed rendering.** The per-X `selectAutoTapProducers` call is
  the cast row's own emitter on a resolved cost, but no game in this lane reached an
  ANNOUNCE_X menu, so the clause has not been seen on a real row. The reviewer instrument
  is free: annotation-present-vs-outcome cross-tabs on ANNOUNCE_X records.
* **D7(c)'s decode has not been tested on a model.** The collapse is provably lossless
  and the note explains it, but whether the pilot answers correctly *inside* a monotone
  range is a corpus question. If an ANNOUNCE_X record in wave 56 answers out of range or
  unparsed, this is the first suspect — the floor is one constant
  (`kMonotoneXCollapseFloor`) and raising it disables the collapse without touching
  anything else.
* **D17 is fixed at the deref, not at the ownership.** `MTGAbility::target` still
  dangles; what changed is that the AI no longer dereferences it to learn a type. Any
  OTHER site that dereferences a dead `target` (or a dead `source`) will still fault —
  I did not audit the rest of the engine for them, and `a->source` in particular is read
  unguarded in the same loop. A real ownership fix (clearing or refcounting the
  Targetable back-pointers, or evicting dead abilities from `ActionLayer::mObjects`)
  remains open and is worth a docket item.
* **The crash rate is a 5-to-6-game sample per arm** on ONE matchup, and the stub's fixed
  `CHOICE: 1` is not corpus play. The base's 4/5 (BI=0) vs 2/5 (BI=1) split is enough to
  refute "the flag causes it" — the default path crashes too — but it is NOT a rate
  estimate. The post-fix 12 of 12 exit 0 is a strong signal, not a proof of absence.
* **The Flip-Side phase-7 livelock is reported, not diagnosed.** I confirmed it on the
  base binary and did not investigate it; no fix, no fixture.
