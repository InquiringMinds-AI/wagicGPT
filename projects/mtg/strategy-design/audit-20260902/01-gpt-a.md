# Audit 2026-09-02 - slice 1 (gpt-a): AIPlayerGPT.cpp lines 1-12000 + include/AIPlayerGPT.h

Master 859186536. Read in full: `include/AIPlayerGPT.h` (1137 lines) and `src/AIPlayerGPT.cpp` 1-12000
(all emitters, the async worker, pollCompletion/Retry, ctor, translog writer, narration register,
receiveEvent/describeEvent, serializeGameState head). Also read for call-frequency context only:
`askModel` (17295-17591), `decisionPending` (18806), `AIPlayerBaka::Act` (5030-5110). NOT read:
12000-37143 except those spans (describeAction, the seams, parseChoice, PARSETEST are slice 2+).

## Measurements taken (worktree audit-gpt-a, master binary, no rebuild)
* **perf run**: one headless selfplay game deck123 vs deck126, both seats `WAGIC_AI=gpt` against a
  fake endpoint (`CHOICE: 1` after 1.0 s), `perf record -F 499 -g`. 306 model calls, 17 turns,
  35,540 samples (~71 s CPU). Inclusive shares (children): `Act` 26.4% -> `computeActions` 26.1%,
  `FindCardToPlay` 13.3%, `serializeGameState` 9.8%, `describeZoneCards` 8.0%, `askModel` 6.7%,
  **`describeAttachments` 4.6%**, `assemblePrompt` 4.1%, `writeTransLog` 0.6%, `joinZoneEntries` 0.5%,
  `drawPunisher*` 0.7%, `instanceHandle` 0.2%, `dynamicMagnitudes` 0.15%. Engine `findType` (string-keyed
  type lookup) is 23.6% inclusive game-wide. CAVEAT: the desktop binary is effectively unoptimised
  (`std::string::size()` shows as its own 5% symbol), boards stayed small (no loop), so treat shares as
  relative signal, not Vita numbers. Data: `~/.gatelogs/audit-gpt-a.perf.data`, `.children.txt`.
* **corpus sizes** (40 most recent real translogs in `~/.Wagic/ai/gpt/logs`): per game, the sum of
  ask-cache keys (bytes from `--- CURRENT SITUATION ---` to prompt end, per `ask` record): median 99 KB,
  max 1.59 MB (471 asks, 85 turns); total narration delta (`events` fields): median 8 KB, max 41 KB.
* Unreferenced-function scan over the 277 free functions in the slice: none unreferenced (the
  `[already owned:` form is retired - it survives only as PARSETEST input text, not as an emitter).

## Findings (ranked)

### F1 MED PERF - describeAttachments is O(N^2) with a string-keyed type lookup per pair - VERIFIED
`src/AIPlayerGPT.cpp:2078-2105`, called per battlefield entry from `describeZoneCards` (4882), which runs
for BOTH battlefields on every `serializeGameState` (2-3 per decision: ask key 17392, assemblePrompt 9931,
priority boardKey 16766, stale-drop 8707).
Mechanism: for every host it walks both battlefields and, for every non-aura card, calls
`att->hasType(Subtypes::TYPE_EQUIPMENT) || att->hasType("fortification")` - the string overload resolves
through `MTGAllCards::findType(string)` (map lookup + string compare) on each (host, att) pair.
Evidence: perf 4.57% inclusive of the whole game for a 17-turn, small-board game (more than half of the
8.0% the zone render costs); scales as N^2 x findType, and the corpus boards this file documents run to
322 and 1,539 permanents (header 552-555, 4226-4231).
Fix: one pass per `describeZoneCards` call: build `std::map<MTGCardInstance*, vector<MTGCardInstance*>>`
host->attachments over both battlefields (aura: `auraParent`; equipment/fortification: `target`), with the
fortification type id resolved once (`static int kFort = MTGAllCards::findType("fortification")` then
`hasType(int)`), and print from the map. Same result string, order preserved (battlefield order).
Blast radius: board line only; every PARSETEST `{attached:` pin and the suite's board fixtures.
Verify: PARSETEST 2249/0, suite 1215/2-known, byte-identical prompts on a replayed corpus record.

### F2 MED PERF - instanceHandle is O(N) per call and called per entry - SUSPECTED at scale
`2123-2148` (42 call sites). Each call rescans the controller's whole battlefield counting same-named
cards; `describeZoneCards` calls it per entry (4754) and again per attachment (2099), per host (4902),
per blocked attacker (4910) -> O(N^2) name compares per render, x2-3 renders per decision. Measured only
0.2% on small boards; on the 322/1,539-token boards it is 10^5-10^6 string compares per render.
Fix: per-render `struct HandleIndex { map<string,int> total; map<MTGCardInstance*,int> rank; }` built in
one pass over a battlefield; `describeZoneCards`/`joinZoneEntries` take handles from it. Keep the
free function for the narration paths (1 call per event is fine).
Verify: PARSETEST "#N" pins; the `joinZoneEntries` x-range cases.

### F3 MED PERF - serializeGameState rebuilt 2-3x per decision and twice inside one askModel - VERIFIED
`askModel` computes `serializeGameState() + tailStr` for the cache key (17392, again at 17398 on a re-ask)
and `assemblePrompt` renders it AGAIN with `&tail` (9931) in the same call; `chooseOrderedAction` does the
same for `boardKey` (16766) before its assemblePrompt. perf: `serializeGameState` 9.8% inclusive, i.e.
the largest single GPT-seat cost. NOTE (corrects the header): the seams are NOT re-entered every tick
while a call is in flight - `AIPlayerBaka::Act` returns at `decisionPending` (AIPlayerBaka.cpp:5034), so
this is per decision, not per tick (see F9).
Fix: memoise the base render per engine tick: `string mBoardRenderCache; int mBoardRenderTick;` keyed on
a monotonic observer/ frame counter, returned by `serializeGameState(NULL)`; `assemblePrompt`'s
`optionText` variant only changes the OWN-battlefield line (`effectSkip`, 12232) - render that one line
separately and splice, so the key render and the prompt render share the other ~95%.
Blast radius: every seam; all corpus prompts must stay byte-identical. Verify: replay a corpus record
(`WAGIC_REPLAY`) and diff prompts; PARSETEST.

### F4 MED MEM-EFF - mAskCache is unbounded for the life of the seat - VERIFIED
`include/AIPlayerGPT.h:330`; writes at 17578, reads 17399; grep shows no `clear()`/`erase()` anywhere.
Every answered ask stores a key = full CURRENT SITUATION block + option block (measured 1.4-7.8 KB per
key). Corpus: median 99 KB per game, max 1.59 MB (85-turn game) held until the AIPlayerGPT is destroyed;
on the Vita's ~40 MB free heap that is 4% of the heap for cache entries that can never hit again once the
board has moved (the key embeds the phase and the board). The 1,200-game suite is unaffected (Baka seats).
Fix: (a) clear on turn change next to `mPassDeclineCount` (16375) - the cache's stated purpose is
re-polls within one state; or (b) key on a 64-bit hash of the string (`std::hash<string>`) and keep the
last K=64 entries. (a) is the smaller change and keeps the re-poll semantics.
Verify: 46 AI/0 suite games with `WAGIC_AI=gpt` fake endpoint; corpus replay shows identical choices.

### F5 LOW LEAK - mNarrationPending grows unbounded when the translog is off - VERIFIED
`writeTransLog` returns early when `mTransLogPath.empty()` (9483-9487) and `mNarrationPending` is only
cleared inside the record write (9582); `narrationAppend` deliberately never trims the delta (10099-10102);
9882/10845 append to it directly. With `translog=0` (the shipped default, GptConfig.cpp:37 - i.e. the
Vita player) every narration line of the game accumulates for the seat's lifetime: measured 8 KB median /
41 KB max per corpus game, more on loop games (the holder collapses lines, but each collapsed line still
lands here). Bounded by game length, so LOW - but it is a per-game leak on the platform the log is off.
Fix: in the early return, `mNarrationPending.clear();` (or skip the append when `mTransLogPath` is empty
- one branch in `narrationAppend`'s caller `writeNarration` 10246).
Verify: PARSETEST narration pins unchanged; translog-on corpus records byte-identical.

### F6 MED PERF - card scripts re-parsed per row per render; AutoLineMacro::Process per line - SUSPECTED
All the "pure over magicText" scanners in the slice lowercase and line-split a script on every call:
`drawPunisherClause` 6938, `discardPunisherClause` 7257, `castTriggerDrawCount` 7085,
`opponentExtraDrawPerTurn` 7203 (runs `AutoLineMacro::Process` per '_' line), `scriptAbilityDrawCount`
7568 and `scriptSelfDrawCount` 7621 (same), `amassCountersFromScript` 569 (same), `annihilatorTag` 2221,
`counterAddPlusFromScript` 692, `drawStepExtrasScan` 7461, `lifeToDamageConverterScript`/`lifeLossMirrorScript`
(11878-11880). `drawPunisherScan` alone has 8 call sites (per serializeGameState 12288/12297, per cast row
18464/18481, per ability row 14469, per X row 7830/8047); `converterSituationLine` + `drawPunisherSituationLine`
run on every serializeGameState. `dynamicMagnitudes` (1474) additionally builds an `AbilityFactory` and
`TargetChooserFactory`+chooser (`riderHasLegalTarget` 771) per row. Measured only 0.7% on a matchup
with no punishers; the decks these exist for (125/130/162) hit every path per row per render.
Fix: the scanners are functions of the script text only -> one `ScriptFacts` struct per distinct script,
memoised in a file-local `std::map<string, ScriptFacts>` (or computed once on `CardPrimitive` at load),
holding: drawPunisher {per, conditional}, discardPunisher, castTriggerDraw, extraDrawPerTurn {n, variable},
selfDraw, amassN, annihilatorN, counterAddPlus, converter/mirror flags, lowercased text. The board scans then
become a map lookup per card. Vita RAM cost: ~one struct per distinct card in the two decks (<100).
Verify: every scanner already has PARSETEST pins; run them through the memoised front door.

### F7 LOW PERF - collectTribalTypes rebuilds the whole creature-type vocabulary per changeling per render
`2280-2318` via `changelingAnnotation` (2325; call sites 4829 hand/board line, 9790 decklist, 24614 bottom
ask): lowercases and inserts the entire `getCreatureValuesById()` vocabulary into a map, then lowercases the
magicText of every card in 4 zones - per changeling card, per render. The result is a deck-level fact.
Fix: compute the tribal set once per player in `buildSystemPrompt` (member `std::set<string>` x2) and a
static lowercased vocab map; re-scan only if a card enters play from outside the deck (tokens have no
consumer scripts). SUSPECTED (no changeling deck in the profiled matchup).

### F8 LOW PERF - full-battlefield scans on EVERY engine event - SUSPECTED
`receiveEvent` ends with `noteDesignationChange()` (scans both battlefields, 10996) and `noteChosenNames()`
(scans both battlefields, an `ostringstream` key per chosen-name card, 11024) for every event of any type
(10988-10989). Events are the highest-frequency entry point in the seat (every zone change, counter, damage,
phase). Measured 0.11% on small boards; on a 1,539-permanent loop board every counter event pays 2 x N.
Fix: gate `noteChosenNames` to `WEventAbilityActivated`/`WEventZoneChange` of a card with `chooseaname`
scripting, and `noteDesignationChange` to `WEventCardTransforms` / zone changes of an Emblem.

### F9 LOW CRUD - stale/misleading comments and small dead guards (legibility for agents)
* `AIPlayerGPT.h:258-264` and 272-286 say the pregame/announce-X asks "are re-entered every tick while a
  model call is in flight"; since c5d `AIPlayerBaka::Act` returns at `decisionPending()` before
  `computeActions` (AIPlayerBaka.cpp:5034), so the Act-driven seams are entered once per answer. Only the
  reveal display / PreGamePhase-driven asks poll per tick. Reword - it steers an agent (it steered this
  audit) toward a per-tick cost model that is wrong for 90% of the seams.
* `AIPlayerGPT.cpp:1486-1489` `dynamicMagnitudes`: null-guards `card` then dereferences it unconditionally;
  both callers (14440, 17936) already guard. Drop the guard or make it `if (!card) return ""`.
* `8169-8197`: the WorkerCtx doc comment is split by the padlog `#if` block ("//mid-request." stranded at
  8197). Move the padlog block above the comment.
* `9162` `curl_global_init` per constructor, never `curl_global_cleanup` - refcounted inside libcurl so not
  a leak, but it is per-game in a long process and documented as not thread-safe; call once from `main`
  (or a function-local static) instead.
* `3902-3912` `optionCardText` runs `collectMutatePile` and then `pileAwareCardText` runs it again for the
  same card - fold into one call.

### F10 LOW PERF - translog opens/closes the file per record
`writeTransLog` 9658, `ensureGameStartRecord` 9314, `flushRecoveryRecord` 9465, `logGameEnd` 9710 each
`std::ofstream(path, app)`. The file itself notes (8788-8792) that per-event storage I/O on the Vita is a
known lag source. perf: 0.6% (json dump dominates). Fix: one `std::ofstream mTransLog` opened lazily in
`ensureGameStartRecord` (after the rename), `flush()` per record, closed in `logGameEnd`/dtor.

### F11 LOW MEM-SAFE - detached worker at process exit
`WorkerMain` (8209) runs on a detached thread holding only a `shared_ptr<AsyncState>`; a game that ends the
PROCESS mid-request (quit, suite exit, Vita power) leaves the thread inside curl / `gptLogLine` (static
`GptMutex`, static `lastErr` 8232-8238) while static destructors run. Not observed (the suite never spawns
a GPT worker). Fix: a process-lifetime `std::atomic<int> gLiveWorkers` + best-effort join/wait (bounded,
~timeout) in `AIPlayerGPT` shutdown, or `quick_exit`. The AsyncState/WorkerCtx ownership itself is sound:
mutex-guarded status/response, `started` written before spawn, `delete ctx` on the worker only.

## Checked and clean (no finding)
* Raw pointers `mCounteredSpell`, `mLoopAbility/mLoopClick`, `mRepeatAbility/mRepeatClick`,
  `mFlipDoneCount` keys, `mSelfActivationStamp` keys: compared, never dereferenced; `counterMarkerMatches`
  walks `moved->previous` (live at event time - MTGGameZones.cpp:936 deletes `previous` only on cleanup).
  Residual ABA on a reused address is theoretical (cleared on turn change / consumed on next stack move).
* Per-turn maps (`mPassDeclineCount`, `mFlipDoneCount`, `mListDeclineCount`, `mStuckCastLines`, `mHoldRows`)
  all have their clears (16375-16376, 16778/18569, 17905, 12996-13028). `mNarration` is trimmed at 24 KB.
* `mAsyncState` is touched only under `GptMutex` outside pollCompletion (grep); the refused-thread path
  resets the slot synchronously (8777-8783) - correct.
* Diagnostics: `GPTASYNCLOG`/`gptPadlogFile` compile out unless `_DEBUG || WAGIC_DEVLOGS` (8175-8195) -
  meets the owner rule. `DebugTrace` is the engine's existing macro.
* No dead emitters in the slice (277 free functions, all referenced); the retired `[already owned:` form
  exists only as PARSETEST input.

## Housekeeping
Worktree `worktrees/lanes/audit-gpt-a` removed; branch deleted. Test translogs from the fake-endpoint game
(`~/.Wagic/ai/gpt/logs/1788383531-*`) deleted so they cannot pollute a corpus review. perf data kept under
`~/.gatelogs/audit-gpt-a.*`.
