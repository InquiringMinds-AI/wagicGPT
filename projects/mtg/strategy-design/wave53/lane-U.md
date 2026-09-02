# Wave-53 lane U — Vita vpk12 human-play reports: the AI's double block, and the
# transcripts that lost their `#result` line

Base: master `859186536` (wave-53 lanes M-T merged). Branch `w53-lane-U`.
Baseline re-measured on this worktree before any change: suite **1215 tests (2 failed)
+ 46 AI (0 failed), 0 timeouts**; PARSETEST **2249 passed / 0 failed**.

---

## REPORT A — "opponent double blocked for no good reason" (tag `bad blocking`)

Transcript `psp-work/logs/20260902-vpk12/transcript-1788381069-player_deck1-vs-ai_baka_deck132.txt`,
`#result winner=p1 turn=24 life=34/13`. Human p1 deck1 "Fangbearer Grind" — Hapatra,
Gifted Aetherborn, Vampire Nighthawk, Fynn the Fangbearer, Mire Triton, Bloodforged
Battle-Axe. **Every attacking creature in that deck has deathtouch**; p2 (Baka, deck132,
mono-W lifegain) blocked with 1/3 Daybreak Chaplains. p2 did not die to damage — it died
to Fynn's poison at 13 life, which is why the result line reads a live life total.

### Where the second blocker comes from

I scanned every p2 blocker-click sequence in the game. There are five combats; the two
the owner would have seen last (turns 23 and 24) are the multi-attacker ones at actions
391-399 and 437-439, where two DIFFERENT Daybreak Chaplains (battlefield[8] and
battlefield[10]) are clicked in the same blocker window, one of them six times. A click
record is not a declaration record — a click *cycles* a blocker one step through its
legal attackers and a 4-click blocker with 3 legal attackers ends unassigned — so the
clicks locate the combat but cannot by themselves prove which attacker each body ended
on. What they do show unambiguously is **multi-click cycling on one body**, and that is
the signature of pass 1 of `AIPlayerBaka::chooseBlockers`: pass 1 is the only pass that
clicks a blocker repeatedly until it accepts or runs out; pass 3 clicks once.

`chooseBlockers` has four stages. The wave-53 **survival sweep** (lane T) is not the
source: it skips any attacker that already has a blocker, so it can only ever add a
FIRST block, and here it would not have fired at all (the AI was at 13 facing ~7 power —
`unblockedDamageTo` was well under `life`). Pass 3 is not the source either: it refuses
any attacker whose `currentBlockers >= maxBlockers`, and `maxBlockers` is 1 unless the
attacker has menace or "three or more blockers". **Pass 1 is the only stage that can put
two blockers on an ordinary attacker** — it keeps assigning while the attacker's tracked
remaining toughness is still positive, with no per-attacker cap. (Its own source comments
its intent: `AIStats::isInTop` carries the note *"uncomment the above return to make Ai
always multiblock your creatures."*)

### The hole: the blocking arithmetic does not know what deathtouch is

Two 1/3 Chaplains onto a 2/2 deathtouch attacker reads, in pure power-vs-toughness terms,
as a free kill: 1+1 >= 2 kills it, and each blocker's toughness 3 beats its power 2, so
both "survive". Deathtouch (CR 702.2b) makes that reading false — any nonzero damage from
the attacker destroys the creature, so the attacker splits 1 and 1 and **both blockers
die**. The gang is a 2-for-1 against the defender. That is the "no good reason".

The same blind spot sits in pass 3 on its own, and it fires far more often than the gang:

```
bool survives = (card->toughness > attacker->power);
...
else if (survives && attacker->power < life)   // "block, we live and reduce damage"
```

Against a deathtouch attacker `survives` is simply wrong, so the seat feeds one creature
per combat to a deathtouch attacker it cannot kill, for nothing.

### The fix (`src/AIPlayerBaka.cpp`)

New file-static `blockerDiesToAttackerRegardless(attacker, blocker)` — true when the
attacker has DEATHTOUCH or PERPETUALDEATHTOUCH, has power >= 1, and the blocker has
neither indestructible nor protection from the attacker (the two outs the rules give it).

1. **Pass 1** refuses a blocker that would be the SECOND-or-later body on such an
   attacker. The lone blocker that remains is then handed to pass 2, which already
   unassigns a block that does not kill — so the seat ends up taking the damage instead
   of paying two creatures, which is the correct line.
2. **Pass 3** computes `survives` through the same helper, so the "we survive and reduce
   damage" branch can no longer be entered against a deathtouch attacker. The lethal-chump
   branch (`!survives && attacker->power >= life`) is unaffected and now correctly
   reachable in that case.

Nothing else is touched: no cap on a first block, no change to menace/three-blocker sets,
lane T's survival sweep untouched.

### Fixture — `bin/Res/test/ai_block_deathtouch_gang_w53u.txt` (registered in `_tests.txt`)

A 2/2 deathtouch attacker (Pitiless Gorgon, `abilities=deathtouch`, no `auto=`) swings
into a defender at 20 holding two 1/3 vanilla bodies (Maritime Guard).

- **RED on base** (`859186536`, measured by reverting `src/AIPlayerBaka.cpp`, rebuilding
  and running scoped): the defender blocks, takes 0, and ends with **one guard in the
  graveyard and one on the battlefield** — a creature spent for nothing, the Gorgon alive.
  Assert diff: `life expected 18, got 20`, `graveyard expected 0, got 1`,
  `battlefield expected 2, got 1`.
- **GREEN after**: no block is declared, the defender ends at **18** with both guards and
  the Gorgon alive.

### What this fixture does NOT cover, stated plainly

It pins the **pass 3** half. It does **not** pin the pass 1 gang, and I could not build a
fixture that does: pass 1's accept branch is gated on `getStats()->isInTop(attacker, 3,
false)`, and `AIStats::isInTop` returns `tooSmallCountsForTrue` (here **false**) whenever
the stat list holds fewer than `max` entries. The stat list is built from damage the AI
has actually taken, so pass 1 is **unreachable in a fresh fixture board** and only wakes
up in a long game where three or more distinct opposing cards have connected — which is
exactly the owner's turn-23 board and not something a static `[INIT]` can manufacture. I
tried: a 3-distinct-attacker priming board over one combat only ever primes two stats, and
the long version (160 pad commands) segfaults on the **base** binary too, so it is not a
usable harness. The pass 1 guard therefore ships on the reading above plus the rules
argument, not on a measured red.

---

## REPORT B — 9 of 16 vpk12 transcripts carry `#classification` but no `#result`

### Who writes the two lines

Both go through `GameObserver::appendTranscriptNote` (`src/GameObserver.cpp`), which
appends to `mTranscriptNotes` and rewrites the file. The callers are both in
`GameStateDuel::Update`:

- `#result …` — inside `if (game->didWin())` in `case DUEL_STATE_PLAY` (line ~1205).
- `#classification=…` — in `ButtonPressed` for `DUEL_MENU_TRANSCRIPT_TAG` (line ~1843),
  after the "How was this match?" menu is answered in `case DUEL_STATE_END`.

### Why the result line went missing — proved, not guessed

On the Vita the seat is human-vs-CPU with one game per match, so `tournamentRoute` is
false and the **only** route into `DUEL_STATE_END` (where the classification menu is
built) is the `else` branch *inside* `if (game->didWin())`. So for all nine files the
`didWin()` block certainly ran — the classification proves it. Inside that block the
result note sat behind

```cpp
static GameObserver * transcribed = NULL;
if (transcribed != game) { transcribed = game; ...write #result... }
```

A `GameObserver` is deleted in `GameStateDuel::End()` and a fresh one allocated in
`Start()`. **A recycled heap address makes that guard read "already done" for a brand new
game.** Desktop glibc does not recycle it (see below); the Vita's allocator does.

The corroboration is decisive and comes from a **second, independent** one-shot in the
same block — the Vita memlog's `vitaMemProbe("gameend", …)`, guarded by its own
`static GameObserver * memlogged`. Those two statics share exactly one operand: the `game`
pointer. Over the whole vpk12 session:

```
memlog "gameend" present per game: 1 1 1 0 0 0 1 0 0 1 1 0 0 1 0 0
transcript "#result"  present per game: 1 1 1 0 0 0 1 0 0 1 1 0 0 1 0 0
```

**16 of 16.** No other shared cause exists: the code between them is unconditional, and
everything downstream (the menu, the classification write) ran normally in all sixteen.

The 9 do **not** correlate with the winner (p2 won two of them, p1 seven), and every one
of the sixteen carries the `#transcript note …` header the note-write emits, so the
transcript file itself was healthy throughout.

### The fix

The one-shot moves onto the object, where identity actually lives:

- `include/GameObserver.h` — new `bool mGameEndNoted;` and `bool mGameEndMemlogged;`.
- `src/GameObserver.cpp` (CP1252 + CRLF, byte-spliced) — both cleared in the constructor
  **and** in `resetStartupGame()` next to `mTranscriptNotes = ""`, so a re-baselined game
  is treated as new.
- `src/GameStateDuel.cpp` — both `static GameObserver *` guards replaced by the flags.

The fix is allocator-independent: no address comparison remains.

### Reproduction — what I got and what I did NOT get

I could not reproduce the miss on desktop, and I say so rather than claim I did.

- 42 consecutive games in one process through the CPU-vs-CPU tournament route (which does
  the same `End(); Start();` cycle) on the **base** binary: **42/42 wrote `#result`**.
- Instrumented the game-end block with a temporary `WAGIC_OBSPTR_TRACE` fprintf printing
  the observer pointer and whether it equalled the previous game's: **0 repeats in 47
  games**. glibc simply never handed the address back; newlib on the Vita evidently does
  (its `heap_arena` grows in fixed steps and the pool is tight).
- `WAGIC_TRANSCRIPT_MENU_SELFTEST=1` forces `tournamentRoute = false`, i.e. the console's
  own shape (victory screen + classification menu) — but that route returns to the main
  menu and the desktop harness then plays **one** game per process, so it cannot exercise
  a second allocation at all. It did confirm the human-shaped path writes `#result` **and**
  `#classification` for the game it does play.
- The temporary trace was removed before the gate and is **not** in the commit.

---

## Files
- `projects/mtg/src/AIPlayerBaka.cpp` — deathtouch helper; pass 1 gang refusal; pass 3
  `survives`.
- `projects/mtg/include/GameObserver.h`, `projects/mtg/src/GameObserver.cpp`,
  `projects/mtg/src/GameStateDuel.cpp` — per-observer game-end one-shots.
- `projects/mtg/bin/Res/test/ai_block_deathtouch_gang_w53u.txt` + `_tests.txt`.

## Gate (this worktree, memory-capped, `-p MemoryMax=4G -p MemorySwapMax=0`)

| | baseline (base source) | after |
|---|---|---|
| suite | 1215 tests, 2 failed; 46 AI, 0 failed; 0 timeouts | **1215 tests, 2 failed; 47 AI, 0 failed; 0 timeouts** |
| PARSETEST | 2249 passed / 0 failed | **2249 passed / 0 failed** |

The two failures are the known concurrency-only pair `lifeline.txt` + `merrow_reejerey.txt`.
AI count 46 -> 47 is this lane's fixture. Every existing blocking pin re-checked green
individually: `ai_block_lethal_set_wave53` (lane T), `blocker_one_attacker_only_w53m`
(lane M), `menace_defender_{one_blocker,two_blockers,ai_control_vanilla}`,
`plague_wight_blockers_shrink`, `trample_vs_{ordinary,protection}_blocker`,
`nontrample_vs_protection_blocker`, `argothian_enchantress`,
`combat_block_after_attack_trigger`.
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**; GameObserver.cpp CRLF preserved.
No PARSETEST cases added: nothing in this lane touches a parsed or rendered string.

## Falsifiable predictions
1. In the next Vita session, **every** transcript that receives a `#classification` also
   carries a `#result` line, and the memlog carries one `gameend` per `gamestart` — any
   missing pair falsifies the recycled-pointer reading.
2. Baka never declares a second blocker on a non-menace **deathtouch** attacker, and never
   declares a block on a deathtouch attacker it cannot kill unless it is otherwise dead on
   board that combat.
3. If the owner still sees a "double block for no good reason" after this, the attacker
   was NOT a deathtouch creature and the residue is pass 1's missing per-attacker cap in
   general (a cost-blind gang) — a separate docket item, not this fix.

## What I did NOT verify
- **No Vita rebuild and no device run by me.** Everything is desktop, headless, suite-level.
- **Replay of the report-A transcript is impossible on this dump.** `WAGIC_REPLAY` diverges
  at **action 83, turn 6**: `p1.zone[0] 1bloodforged battle-axe` expected, `p1.choice 0`
  got, with `chooser=read the bones` — the Read the Bones scry/surveil window resolves in a
  different order under replay. That is a *different* divergence from lane T's RNG-stream
  drift (it fires before the game's only shuffle) and is a new replay item. The board at
  the reported combat is therefore read from the transcript and the card definitions, not
  replayed.
- **Which pass produced the second blocker in the owner's game is inferred, not observed.**
  The clicks show pass-1-shaped cycling and pass 1 is the only stage that CAN do it, but I
  did not instrument the console build, and `chooseBlockers` does not run during a replay
  (the AI's clicks are in the record), so no run of any kind observed that decision.
- The pass 1 gang guard has **no red-on-base fixture** (see Report A above).
- I did not measure whether the deathtouch guard changes Baka's win rate; no corpus or
  selfplay statistics were taken. The only live runs were the throwaway 42- and 47-game
  selfplay batches used to test allocator behaviour, and their transcripts were discarded.
- Nothing here touches `AIPlayerGPT`, `MTGRules.cpp`, the render, or any card script; the
  LLM seat is unchanged except that its heuristic fallback (Baka) now blocks differently.
- `heuristicDamageOrder` already models deathtouch correctly (`cost = deathtouch ? 1 : …`);
  I did not change it, and I did not check whether the ATTACK side (`chooseAttackers`) has
  the same blind spot on the defender's deathtouch blockers.
