# Audit 2026-09-02 — slice 02-gpt-b: `AIPlayerGPT.cpp` lines 12000–24000

Master commit 859186536. Read IN FULL (12000–24050, end of `chooseBlockers`' set-legality loop),
plus the callers/callees needed to reason about cadence and lifetime: `AIPlayerBaka::Act`
(AIPlayerBaka.cpp 5030–5190), `assemblePrompt` (9888–9931), the `mAskCache` header lines,
`DebugRoutines.h`, `Makefile.sdl` DEFINES, the Vita `CMakeLists.txt` defines (grep only),
`ManaEngine::selectAutoTapProducers` / `potentialColorReach` heads.

NOT read: `serializeGameState` body above 12000 (only its tail 12000–12316 is in-slice),
`consumePlan`, `pollCompletionRetry`, `writeTransLog`, `groupNumberedRows`/`joinNumberedRows`,
`DecisionManager`, the PARSETEST block (35k+), and everything below 24050.

No build or measurement was made for this slice — every PERF magnitude below is SUSPECTED;
the call counts and duplicate sites are VERIFIED by reading/grep. One important cadence fact
that bounds all the PERF items: `AIPlayerBaka::Act` returns at `decisionPending(dt)` BEFORE the
tick timer while a model call is in flight, so a prompt is NOT re-rendered every frame during
the wait. Each decision costs roughly two full renders (launch + consume) plus the cached-replay
ticks (`unchanged` path) when the same state is re-polled; the per-decision render is what the
findings below make cheaper.

Totals: 9 findings — PERF 4 (MED 3, LOW 1), MEM-EFF 2 (MED 1, LOW 1), MEM-SAFE 2 (LOW 2),
CRUD 1 (LOW, with 8 duplicate clusters). No LEAK found in the slice: every `NEW`/factory object
I traced (`potential` ManaCost 12023/12028, `TargetChooser` 18146/18419, `tc` 20885/20889) is
deleted on all paths.

---

## F1 — MED · MEM-EFF · `mAskCache` is never cleared inside a game; keys are whole situation blocks

**Where**: `src/AIPlayerGPT.cpp:17392–17401`, `:17578`; header `include/AIPlayerGPT.h:330`.
**VERIFIED** (grep: the only three references are `find`, `end`, `operator[]` — no `clear()`,
no erase anywhere in the file or header).

**Mechanism**: `askModel` keys its answer cache on `serializeGameState() + tailStr` (17392) —
the full CURRENT SITUATION render plus the whole rendered option list. The comments in this
slice record prompts of 10–17 KB and one priority window of 116 KB. Every distinct ask in a
duel inserts one such key and it stays until the `AIPlayerGPT` object dies with the game. A
40-turn game with a few hundred asks holds several MB of dead strings on the Vita's ~40 MB
heap; a wide-token-board game (the 432-option / 1539-creature games cited at 12718 and 16483)
holds far more. It is per-game, not cross-game (the object is per duel), so the suite does not
accumulate it — the Vita does, for the length of the longest game.

**Fix sketch**: the cache exists so re-polls of the same state and the earlier picks of a
multi-target selection replay for free — all within one turn. Clear it at the turn boundary
alongside the other per-turn maps (`mPassDeclineCount`, `mListDeclineCount`, `mStuckCastLines`
already do this in `chooseOrderedAction` 16373 / `FindCardToPlay` 17903). Cheapest form: store
`std::pair<int turn, int choice>` and drop entries whose turn < `observer->turn` on insert, or
just `mAskCache.clear()` when `mPassDeclineTurn != observer->turn`. Also consider hashing the
key (`std::hash<std::string>` → 8 bytes) instead of storing the 10–116 KB string; collisions are
harmless here (a wrong replay only reproduces an already-parsed option index on a byte-identical
question, and a 64-bit hash of a same-turn set is not a realistic collision).

**Blast radius**: `askModel` only. `mAskReaskKey` (17395) compares against `askKey0` and is
unaffected. **Verify**: PARSETEST (2249/0) unchanged; a live translog must show no new HTTP
call on a same-state re-poll within a turn (`latency_ms -1` records still present).

---

## F2 — MED · PERF · `describeAction` runs the mana planner and the colour-reach scan TWICE per option row

**Where**: `src/AIPlayerGPT.cpp:14316–14320` and `:14350–14358` (`selectAutoTapProducers`,
identical arguments `this, src, c, src->has(ANYTYPEOFMANAABILITY), false`);
`:14354` and `:14415` (`potentialColorReach(this, reachPolicy, NULL)`, board-wide, identical).
**VERIFIED** duplicate calls by reading; cost SUSPECTED (no timing).

**Mechanism**: `chooseOrderedAction` calls `describeAction` for EVERY candidate on every
render (16397–16398), before de-dup and decline filtering. For each row with a mana cost the
function runs `ManaEngine::selectAutoTapProducers` (which runs `planPayment` — ManaEngine.cpp
386–392 records a prior measurement that this planner was 54 % of a display refresh on the
owner's vpk11 board) once for the `{paying this taps:}` clause and once again for the
`{spends N of your M}` clause. `potentialColorReach` (walks every mana object in the action
layer, 1210+) is called once per row for the strand clause and once more per fetch row, when it
is a board-wide number that `FindCardToPlay` already hoists (17911 `untappedSources`). On the
432-row Staff-of-Nin window (16483) that is ~860 planner runs and ~430 reach scans per render.
The same shape recurs with board-wide scans inside per-row loops: `drawPunisherScan` per row
(14469, 18464, 18481, 19786), `castTriggerDrawScan`/`converterScan` per cast row (18476, 18499),
`exileCastNote`'s battlefield walk per cast row (13891–13907, guarded by the exile label).

**Fix sketch**: (a) in `describeAction`, compute `picks` once and reuse for both clauses;
(b) add an optional `int untappedSources` parameter (or a per-window member set by the caller)
so `chooseOrderedAction` computes `potentialColorReach` once per render, as `FindCardToPlay`
already does; (c) hoist `drawPunisherScan` / `castTriggerDrawScan` / `converterScan` results to
the window level (a small struct computed once per `chooseOrderedAction`/`FindCardToPlay` call).
Output must stay byte-identical — the render is the ask key.

**Blast radius**: `describeAction` (priority rows, narration at 17113/17201),
`FindCardToPlay`. **Verify**: translog `options_text` byte-diff on a fixed-seed replay before/after;
PARSETEST fixtures that exercise `describeAction`-fed strings.

---

## F3 — MED · PERF · Combat prompt render is O(attackers × blockers × permanents) with repeated per-pair battlefield/script scans

**Where**: `combatStatOf` 21419–21441 (calls `blockTriggeredLifeFor` 20900–20936, which
walks the controller's battlefield and re-parses every card's `magicText` line-by-line and
builds a `TargetChooser` via the factory per matching trigger line); `combatBlockOutcome`
21497–21505 / `combatAttackOutcome` 21511–21519 (each: `combatStatOf` ×2,
`combatPreventionKind` ×2, `combatPreventionKindToPlayer`, `playerHasLifeToDamageConverter`
which walks the battlefield and runs `lifeToDamageConverterScript` over every card's text);
`chooseAttackers` 22686–22733 (per pair: outcome + `combatPreventionKind(c, attacker)` twice
more at 22707/22726 + a THIRD `combatStatOf(c)` at 22723); `chooseBlockers` 23646–23701
(per pair, `combatStatOf(blockers[i])` recomputed for every attacker k);
`playerHasLifeLoop(opponent())` per attacker row at 22800 and 23551 (and again at 22898),
`converterSituationLine(this, opponent())` at 22890.
**VERIFIED** call structure; magnitude SUSPECTED.

**Mechanism**: none of the per-creature facts (`CombatTradeStat`, the blocking-trigger life
totals, the converter/loop booleans) depend on the pairing, yet all are recomputed per pair,
and `blockTriggeredLifeFor` is also computed for the ATTACKER side of every pair where its
result is never read (the code says so at 21436–21439). Token boards are the case that matters
(the corpus had 22-row blockers menus at 23588 and 1539-creature boards): A=20, B=20, P=40
gives ~1,200 outcome calls × (2 battlefield walks + 2 script parses + prevention probes).

**Fix sketch**: compute once per window — `std::map<MTGCardInstance*, CombatTradeStat>` (or
two parallel vectors indexed by row) for every attacker and every blocker, with
`blockTriggeredLifeFor` run only for the blocker side; `bool oppConverter`, `bool oppLoop`
once per window; pass the stats into `combatTradePreviewStats` directly (its signature already
takes the two structs — only the `combatBlockOutcome`/`combatAttackOutcome` wrappers need a
variant taking precomputed stats). Reuse `combatPreventionKind(c, attacker)` from the outcome
call in `chooseAttackers` instead of the two extra probes.

**Blast radius**: `chooseAttackers`, `chooseBlockers`, the four combat helpers; PARSETEST's
`combatTradePreviewStats` struct-only cases are untouched. **Verify**: translog
`options_text` byte-diff on a fixed-seed replay; PARSETEST 2249/0.

---

## F4 — MED · PERF · `serializeGameState` (the heavy situation render) runs 2–4 times per decision

**Where**: priority: `:16766` (boardKey) + `assemblePrompt` `:9931` → 2 per render (plus
`:16643` per repeat-plan tick). Cast: `:17912` (`boardNow`) + `askModel` `:17392` +
`assemblePrompt` `:9931` → 3, and `:17398` recomputes it a 4th time when `reasked` (the
expression `reasked ? serializeGameState() + tailStr : askKey0` re-renders instead of reusing).
Target/menu/cost asks: `askModel` + `assemblePrompt` → 2 per round.
**VERIFIED** counts; cost SUSPECTED. From its in-slice tail (12011–12316) the function runs
`potentialColorReach` (12025), `selfDamageManaSources`, `LegalActionsOracle::legalLandPlays`
(12099, an oracle pass), two `describeZoneCards` battlefield renders (12232/12238),
`converterSituationLine`, `drawPunisherSituationLine`, two `drawStepExtrasScan`s — the same
board-wide scans F2/F3 pay for again.

**Fix sketch**: two variants exist (`optionText == NULL` for keys, `&tail` for the prompt,
which differs only by `ownEffectSkip` 12219–12229). Memoise per call: pass the already-built
key string into `assemblePrompt` via an overload `assemblePrompt(tail, const string * situation)`
so the seams that computed it hand it over, and split the option-dependent skip out so ONE
render serves both (render once with the skip set computed from `tail`, and derive the key
variant from the same pass — or accept that the key uses the `&tail` render too, since `tail`
is already part of the key). Trivial half: `:17398` should be `askKey = reasked ? askKey0 +
"\n" + mAskReaskLine : askKey0` — same bytes, one render.

**Blast radius**: `assemblePrompt` callers (all seams), `askModel`. The ask key must keep its
exact bytes or every cached answer misses once. **Verify**: translog shows unchanged
`latency_ms -1` replay behaviour; prompt byte-diff on a replay.

---

## F5 — LOW · PERF · Repeated `tail.str()` copies of a prompt-sized buffer

**Where**: `:16760` (`tail.str().empty() || tail.str()[tail.str().size() - 1]` — three full
copies of the option list in one expression), `:16589`, `:16800`, `:16804`; `askModel`
`:17372` + `:17384`. **VERIFIED**.

**Mechanism**: `ostringstream::str()` copies the whole buffer; on the 116 KB window that is
~5 copies per render. **Fix**: `const string tailStr = tail.str();` once after the last `<<`,
and keep `optionsEnd` from an earlier single `str().size()` (or track it as the size of a
separate options ostringstream). **Verify**: byte-identical prompt.

---

## F6 — LOW · MEM-EFF · Per-turn maps keyed by the concatenated menu text

**Where**: `mListDeclineCount` keyed by `listKey` = every row joined (`:16781–16785`,
`:18572–18576`); `mStuckCastLines` holds whole option lines (`:17918`, `:18645`);
`mHoldRows` holds every row string (`:13032–13034`). All cleared per turn (VERIFIED), so this
is bounded — but one hold + one decline of a 116 KB list stores ~3 copies of it for the turn.
**Fix**: key `mListDeclineCount` by `std::hash<string>(listKey)`; store hashes in `mHoldRows`
(only equality is ever tested, 12928–12933). LOW; only matters on the wide-token boards.

---

## F7 — LOW · MEM-SAFE · Unbounded digit-run accumulators (signed overflow) in the combat/name parsers

**Where**: `parseBlockAssignments` `:23113–23114` (`b`), `:23127–23128` (`m`), `:23143–23144`
(`a`); `parseAttackerSet` `:22295–22296` (`n`), `:22321–22322` (`m`), `:22430–22431`;
`nameOrdinal` `:20719–20722`; `proseAttackerOrdinal` `:21685–21686`; `salvageProseAttackers`
`:21772–21773`; `salvageProsePutList` `:22100–22101`; `becomesBlockedSelfPump` `:21555`,
`:21560`. **VERIFIED** by reading. `scanRepeatCountInLine` (13216, 13233, 13243) already caps
with `v < 100000`; `parseChoice` uses `atoi`, which saturates (no wrap).

**Mechanism**: a reply carrying an 11+-digit run after `B`/`A`/`#` overflows `int` (UB; in
practice wraps), and the wrapped value is then range-checked as if it were the model's number:
`"B4294967297:A1"` reads as `B1:A1` — a blocker the model never named gets declared. A model
does not normally write such a token, but the parser is the seat that is supposed to make
illegal/unintended declarations structurally impossible, and the fix is one helper.
**Fix**: a `static int readDigits(const string&, size_t& pos, int cap = 100000)` used by all
of the above (cap → treat as "not a label"). **Verify**: add three PARSETEST cases
(`"B99999999999:A1"` → 0 pairs, `"ATTACK: A99999999999"` → -1/0, `"#99999999999"` ordinal → 0).

---

## F8 — LOW · MEM-SAFE · Raw `MTGAbility*` / `MTGCardInstance*` identities held across turns (ABA, no deref)

**Where**: `mLoopAbility`/`mLoopClick` (`noteLoopTake` 16256–16266, read at 12847, 17148;
deliberately survive turn boundaries, 16723), `mRepeatAbility`/`mRepeatClick` (16614, 17157),
`mFlipDoneCount` keyed by `MTGCardInstance*` (16432, 17143; per-turn clear).
**SUSPECTED**. The header already states they are compared, never dereferenced (h:575), and I
found no dereference — so no UAF. The residual is identity reuse: an ability/card freed by a
zone move whose address is reused by a new object makes `[repeat: … N times in a row]` (12847)
report a foreign loop, and, if both `ability` and `click` collide in the same turn, dispatches
a queued repeat-N plan onto the wrong row (16614). Both pointers must collide, so this is rare;
listed because the repeat plan executes engine actions without a model call.
**Fix**: clear `mLoop*`/`mRepeat*` when the source card leaves play (the seat already
receives zone events for narration — hook `endRepeatPlan("its source left play")` there), or
key on `(card->mtgid or instance id, ability->getMenuText())` instead of addresses.
**Verify**: a PARSETEST-free live check — sacrifice the token maker mid-plan and confirm the
plan ends with the receipt line (16273–16292).

---

## F9 — LOW · CRUD · One release-shipped stderr write, and eight duplicate-logic clusters

**(a) Release diagnostic — VERIFIED.** `:18773–18778` `fprintf(stderr, "AIPlayerGPT: menu on
%s could not be put to the model …")` is unconditional (the sibling MAYPROBE at 19529–19537 is
correctly gated). Owner rule: diagnostics compile out of release; the Vita build defines
neither `_DEBUG` nor a global `WAGIC_DEVLOGS` (CMakeLists 77–298), so this is the only
unguarded stderr write in the slice. The line exists for the #W41-1 "never silent" ruling,
which is a reviewer-facing requirement — the release-safe channel for that is the translog.
Fix: `writeTransLog("defer", …, "menu_not_askable")` (release path, already the corpus
reviewer's surface) + the same text under `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
Also `:18887`/`:18898` `sprintf` into `char[48]` — safe by arithmetic (max 26 and 24 bytes),
but `snprintf` costs nothing.

**(b) Duplicate logic — VERIFIED sites** (owner criterion: legible for LLM coding agents;
each copy is a place a fix lands on one seam and misses its twin, which this file's history
records repeatedly — e.g. the N-152e "sibling path" at 19742, the W37 capital-only matchers):
1. Echo word split: `:15153–15173` is byte-identical to `echoSignificantWords` (14787–14805;
   the comment at 14784 even says so). Near-copies with deliberate filter drift:
   `:15300–15313` (no `isRenderVocabWord`), `:15478–15492` (len≥3, +"the"/"for"),
   `:16019–16037`, and the combat variant `significantWords` 20651–20668. Refactor into one
   splitter taking `(minLen, extraStopWords, applyRenderVocab)` so the drift is a parameter,
   not a copy.
2. `CHOICE:` line walker (skip `*#-` decoration, case-insensitive label, substring after):
   `codedChoiceOrdinal` 13058–13085, `salvageLoopedChoice` 15820–15858, `firstCodedChoice`
   15869–15899, `choiceRetractedNoReplacement` 15956–16000, generic `collectLabeledLines`
   21585–21609. One `forEachLabelledLine(content, label, fn(line, endOffset))` serves all five.
3. `</think>` tail strip: 9 inline copies in-slice (12 in file) — `parseRepeatCount` 13283,
   `planSaysPassThisWindow` 13390, `replyHasPlanLine` 13501, `firstLabelledLine` 13514,
   `parseChoice` 14953, `choiceRetractedNoReplacement` 15946, `replyTerminatedNaturally` 21818,
   `chooseAttackers` 22939, `chooseBlockers` 23792. One `postThink(const string&)`.
4. Lower-casing loops: 51 inline `for … tolower` loops in-slice while `toLowerCopy` (18936,
   forward-declared 13573) exists; many lowercase a fresh copy of every option per pass
   (parseChoice 15127–15129, 15193–15197, 15206–15210, 15271–15272, 15380–15381; each pass
   re-lowercases all N options). Lower-case the option list ONCE at the top of `parseChoice`
   (a `vector<string> lowOpts`) and reuse — this also removes the 5×N string copies per parse
   that `salvageLoopedChoice`/`firstCodedChoice`/`codedChoiceOrdinal` multiply per CHOICE line.
5. Gang-block price: `:22773–22791` and `:23507–23548` compute the same `need`/`dmg`/`lowSum`
   from a sorted power vector (the blockers copy adds the `minBlk` raise). One helper taking
   `(powers, toughness, minBlockers)` returning `(need, dmg, anyOfThem)`.
6. `groupCombatCandidates` permutation block: `:22540–22553`, `:23301–23314`, `:23315–23333`.
7. Auto-tap creature/pain scan: `describeAction` 14319–14338 + 14357–14372 vs
   `FindCardToPlay` 18056–18081 (same `seen`/`tapped` set walk, same `producerSelfDamageOf`).
   Folds into F2's single-`picks` fix.
8. `drawPunisherScan` + join names + `drawPriceRowTag`: 14467–14473, 18462–18468,
   18479–18487, 19784–19789 — one `drawPriceTagFor(cards)` over a per-window scan (F2c).

**Verify** for all of (b): PARSETEST 2249/0 (most of these helpers are PARSETEST-covered) and a
translog prompt byte-diff on a replayed game; the parse-note signatures in the translog
(`echo_index_conflict`, `stale_echo_in_range`, …) must be identical before/after.

---

## Things checked and found sound (so the next auditor need not re-walk them)

- `TargetChooser` from `createTargetChooser(card)` in `FindCardToPlay` (18146) is deleted at
  18419 on every path (no early exit between); `blockingTriggerCovers` deletes its chooser
  (20889); `potential` ManaCost deleted (12028).
- `chooseOrderedAction` row bookkeeping: the hold row is pushed onto `shownLines` but not
  `shown` (16576–16577); every consumer that indexes `shown[choice-1]` (17067, 17201, 17235,
  16922) checks the hold case first or bounds `choice-1 < repeatBaseRow.size()` — no
  out-of-range read found.
- Raw `MTGCardInstance*` vectors across a model round-trip: `candidates` (FindCardToPlay),
  `picks`/`targets` (chooseTarget), `ordered`/`remaining` (orderBlockers), `attackers`/
  `blockers` (combat) are all rebuilt on every tick from the live oracle and consumed on the
  tick the cached answer lands; the cache stores an INDEX into the byte-identical menu, never a
  pointer. Sound.
- `atoi` on digit runs in `parseChoice` saturates on both 64- and 32-bit glibc/newlib → cannot
  wrap into range (contrast F7).
- `DebugTrace` is `(void 0)` on the Vita (no `_DEBUG`); desktop `Makefile.sdl` defines `_DEBUG`
  so the dev build prints — consistent with the owner's convention.
- Async worker / `AsyncState` lifetime: not in this slice (`aiPatiencePromptAnswer` 18837–18853
  only clears the endpoint and relies on the shared_ptr — consistent with the design note).
