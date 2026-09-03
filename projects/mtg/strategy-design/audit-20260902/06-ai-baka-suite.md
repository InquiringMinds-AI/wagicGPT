# Audit 2026-09-02 — slice 6: heuristic AI (Baka), test harness, card DB / deck / options

Master 859186536. Measurements were taken in a throwaway worktree (`audit-ai-baka-suite`, removed
after the run) with the SDL debug build (`-O0 -g`, the gate binary) and the ASAN/LSan build; every
`wagic` run under `systemd-run --scope -p MemoryMax=4G` + `timeout`. Line numbers are master's.

Read in full: AIPlayerBaka.cpp/.h, AIPlayerBakaB.cpp/.h, AIPlayer.cpp/.h, AIHints.cpp/.h, AIStats.cpp/.h,
AIMomirPlayer.cpp/.h, TestSuiteAI.cpp/.h, MTGDeck.cpp/.h, DeckManager.cpp/.h, DeckDataWrapper.cpp/.h,
DeckMetaData.cpp/.h, DeckStats.cpp/.h, CardPrimitive.cpp/.h, MTGDefinitions.cpp/.h, MTGPack.cpp/.h,
PriceList.cpp/.h, PlayerData.cpp/.h, GameOptions.cpp/.h, Translate.cpp/.h, utils.cpp/.h.
Read only as far as the slice needed them (NOT audited): MTGAbility.cpp `abilityEfficiency` (6355-6490),
ManaEngine.cpp `potentialMana` (607-645), GameObserver.cpp dtor/loadPlayer, Player.cpp ctor/dtor,
GameStateDuel.cpp selfplay hook, MTGCardInstance.h member list.

## Measurements (what the numbers below come from)

| probe | how | result |
|---|---|---|
| Baka per-tick cost | worktree-only counters in `selectAbility` / `FindCardToPlay` / `chooseTarget`; headless Baka-vs-Baka selfplay, 200 s, deck5 vs deck12 | 737 `selectAbility` calls: 91,025 non-mana abilities scanned, **1,047,118 (ability,card) pairs, 2,094,236 `isReactingToClick` probes, 8,307 planner (`canPayMana`) calls, 314 ranked actions total** (0.43 per call), 3.20 s CPU = **4.3 ms per call** at -O0 on this i7. `FindCardToPlay`: 762 calls, 0.53 ms each. `chooseTarget`: 36 calls, 0.2 ms each. |
| suite per-game leak | single-thread suite, RSS sampled 1/s (688 tests before the 600 s cap) | RSS **flat at 161 MB for tests 1-147**, jumps to 500 MB inside test 148 (`generic/gift_of_the_gargantuan_i1085.txt`), flat at ~490-500 MB after. No per-game slope. |
| leak at exit | ASAN/LSan, 4 threads, 250 fixtures + all 46 [AI] fixtures (0 failures) | **82,092 B leaked in 5,322 allocations total** (~280 B/game). Top: `ManaCost::init` vectors leaked via `SnowCost::isPaymentSet` (ExtraCost.cpp:162, slice 4/5), and 320 B of `CardPrimitive` type vectors under leaked `MTGCardInstance`s. Nothing rooted in this slice's files. |
| the 330 MB fixture | run alone with RSS sampling; `perf record -e page-faults -g` | `gift_of_the_gargantuan_i1085.txt` alone: RSS 129 MB -> **483 MB**. 85% of the page faults are under `ALord::clone -> ListMaintainerAbility -> MTGAbility ctor -> CardDescriptor/MTGCardInstance/ManaCost` during `resolve`. Freed by exit (LSan clean) — heap retained by the allocator. Belongs to the abilities slice; handed off below. |
| primitive DB footprint | python over `Res/sets/primitives/*.txt`; `sizeof` via the build's include flags | 27,788 primitives / 74,377 printings / 336 sets. Strings on disk: `auto*` 5.42 MB (55,767 lines, avg 97 B), `text` 4.27 MB (avg 156 B), `name` 0.44 MB, `type` 0.26 MB. Desktop `sizeof(CardPrimitive)=640`, `MTGCard=32`, **`MTGCardInstance=2640`**, `ManaCost=176`, `std::string=32`. |
| release diagnostics | grep | one unconditional `printf` (AIPlayerBaka.cpp:4254), one `cout` (TestSuiteAI.cpp:1931, suite-only). |

Baka numbers in context: at 4.3 ms/tick on a desktop -O0 build the seat is invisible; the Vita is
~30-50x slower per core, so the same tick is ~150-200 ms — and `computeActions` runs it every AI
priority window (upkeep/main/combat/end/opponent-turn response windows), most of which rank nothing.

## Findings (ranked)

### HIGH

**H1. PERF — `AIPlayerBaka::selectAbility` scans abilities x cards and re-plans mana per pair**
`src/AIPlayerBaka.cpp:2339-2541` (and the near-copy `doAbility` 2624-2688). VERIFIED (counters above).
Mechanism: for every non-mana ability in the action layer (avg 123/tick) x every card in inPlay+
commandzone+hand+graveyard+exile (avg 11.5) the loop calls `a->isReactingToClick(card, totalPotentialMana)`
twice (2355 and 2359 — the second call repeats the first), and when the first probe fails and the
ability has a cost, `canPayMana` (the full `ManaEngine::planPayment`) runs for that pair — 8,307
planner calls for 314 ranked actions. `abilityPayment` is never cleared between pairs (the
N-116g comment at 2548 documents the correctness half of this), so once any pair set it every later
pair takes the `abilityPayment.size()` branch, builds a `ManaCost` (2366) and probes a third time.
Only the ability's own `source` can ever react to a click on `card` (the probe is source==card plus
zone/cost checks), so ~99.9% of the 1M pairs are dead by construction. Five identical ~38-line
zone rungs (2352-2540) are the CRUD half.
Fix sketch: iterate abilities once; for each ability take `card = a->source` (plus the
multi-source cases: `MTGAbility::isReactingToTargetClick` — collect the sources that pass a cheap
zone membership test instead of the 5 zone walks); probe once; reset `abilityPayment` per pair;
lift `getPotentialMana(card)` (2379) out of the pair loop — it only depends on `card` (issue #76
exclusion), so cache one `ManaCost` per distinct card. Collapse the five rungs into one loop over a
`MTGGameZone*[5]` like c5c did for `FindCardToPlay`. Same treatment for `doAbility`.
Blast radius: every Baka activation; AIPlayerGPT's fallback path; the 46 [AI] fixtures.
Verify: suite 1215/2-known + 46 AI/0; PARSETEST unchanged; re-run the counter probe — pairs should
drop to ~abilities, `ranked` unchanged for the same seed (`WAGIC_SELFPLAY_DECK0=5 DECK1=12`).

**H2. MEM-EFF — every `MTGCardInstance` carries a full private copy of its primitive (2,640 B + strings)**
`src/CardPrimitive.cpp:90-140` (copy ctor), `include/CardPrimitive.h:53-76`. VERIFIED sizes/bytes,
SUSPECTED on the Vita-side total (not measured there).
Mechanism: `MTGCardInstance` derives from `MTGCard` -> `CardPrimitive`, and the copy ctor deep-copies
`text`, `formattedText`, `name`+`lcname` (the same bytes twice), `magicText`, the `magicTexts` map
(re-looked-up twice per entry at 136-137), `types`, and clones `restrictions`. None of those change
for the life of an ordinary instance, and a new instance is built on every zone move (TestSuiteAI.cpp:959
notes "every zone move makes a new object"). Per instance that is ~97-200 B of magicText + ~156 B
text + ~32 B of names + map nodes on top of a 2,640-byte object; a 60-card deck x2 with typical
zone traffic is a few hundred instances per game. On the PSP/Vita the `cardtext` sidecar already
keeps `text` off the primitive, but `materializeMagicText` (505-542) makes `magicText` resident and
the copy ctor then duplicates it into every instance anyway.
Fix sketch: keep `const CardPrimitive * base` on the instance and read immutable fields through it
(`getName/getLCName/getFormattedText/magicText`) with copy-on-write only for the paths that mutate
(`setName` on flip, `addMagicText` on grants, `removeType`); or at minimum stop copying
`text/formattedText/nameOrig` into instances and use `magicTexts` by shared pointer.
Blast radius: everything that reads `card->name`/`magicText` directly — large; do it behind the
accessors first. Verify: suite + PARSETEST, then the Vita heap gauge (`WAGIC_VITAMEMLOG`).

**H3. LEAK/MEM (handoff to the abilities slice) — one fixture allocates ~330 MB through `ALord::clone`**
Evidence above (`gift_of_the_gargantuan_i1085.txt`, RSS 129 -> 483 MB, 85% of page faults under
`ALord::clone -> ListMaintainerAbility(MTGAbility ctor: CardDescriptor + MTGCardInstance + ManaCost)`
inside `resolve`). LSan is clean at exit, so it is churn, not a leak — but a 330 MB transient is a
guaranteed OOM on a 40 MB Vita heap the moment a reveal/lord interaction like this one happens in
play. Not read in this slice (AllAbilities.cpp); the suite fact is recorded here because the suite
RSS curve is otherwise flat. Verify after the fix: the single-fixture RSS run above.

### MED

**M1. CRUD — diagnostics shipping in release builds**
`src/AIPlayerBaka.cpp:4254` `printf("Choose attackers : ...")` — unconditional, fires every
`chooseAttackers` (every AI combat, every platform; on the Vita it goes to the console/log).
`src/AIPlayerBaka.cpp:3629-3640` — the same `DebugTrace("ai is doing a combo:")` twelve times.
VERIFIED (grep). Fix: delete the printf (the DebugTrace form exists) and eleven of the twelve lines.

**M2. MEM-SAFE — fixed arrays in the harness with no bounds check**
`src/TestSuiteAI.cpp:1214-1218` `TestSuiteActions::add` writes `actions[nbitems++]` into
`string actions[100]` (`MAX_TESTSUITE_ACTIONS`, TestSuiteAI.h:6); `TestSuiteGame::buildDeck`
2352-2374 fills `int list[100]` from every zone of the [INIT] block. VERIFIED latent: the largest
fixture today has 45 [DO] lines and 33 cards per player (`lexicon/target_player_and_zone_matrix.txt`),
so nothing overflows yet; the 101st [DO] line of a future fixture overwrites heap `std::string`
headers (silent corruption, not a red test). Fix: `vector<string>` / `vector<int>`; `MTGPlayerCards`
already takes `(int*, int)`, pass `data()`. Verify: a scratch fixture with 120 [DO] lines runs green
under ASAN.

**M3. CRUD + MEM-SAFE — `OrderedAIAction::getEfficiency` and `getRevealedEfficiency` are a 620-line
copy-paste that has drifted**
`src/AIPlayerBaka.cpp:208-922` vs `924-1548`. VERIFIED by reading. The copy already differs in five
places (STANDARD_BECOMES, PUMP threshold 20 vs 50, LIFER base 80 vs 100, the sacrifice `/10` rule,
the planeswalker/land tail) — fixes land in one and not the other. Inside the copy,
`MANA_PRODUCER` (1226-1227) dereferences `GAA->ability` without the NULL check the original has at
527: a nested mana ability reached through an `aicode=` reveal/scry scores against a NULL pointer
(SUSPECTED reachable; the original guards it, so treat the copy as wrong). Both functions also
`return 99` for `AACastCard` (256, 970) before the `SAFE_DELETE(transAbility)` — a leak on the
transformer path (SUSPECTED, needs a transforms(...) card whose new ability is a castcard).
Fix: one function with the ability passed in and the two knobs (`efficiency` cache, the sacrifice
rule) as parameters; keep the NULL guard. Verify: `ai/*.txt` fixtures (46 AI/0) + PARSETEST.

**M4. PERF (Vita) — `MTGAllCards::getCardByName` lower-cases 74,377 names per uncached lookup**
`src/MTGDeck.cpp:1138-1201`. VERIFIED by reading + counts. Each miss walks every printing and does
`string cardName = c->data->name; transform(tolower)` — one heap string per printing per lookup —
although `CardPrimitive::lcname` (CardPrimitive.cpp:660-665) already holds exactly that. A deck
file with 20 distinct names costs ~1.5 M string constructions on the Vita's first load (the suite
hides this with `prefetchCardNameCache`, which real play never calls). Fix: compare
`c->data->getLCName()`; better, build the name->printing index once (the prefetch function already
exists, with the token-vs-real rule) and drop the linear scan. Verify: time `MTGDeck` construction
of `ai/baka/deck1.txt` headless before/after; deck-load result identical (compare `cards` map).

**M5. PERF — `chooseTarget` re-parses the ability text per candidate card**
`src/AIPlayerBaka.cpp:2896-2909`: inside the per-card loop, `af.parseMagicLine(tc->belongsToAbility)`
+ a heap `OrderedAIAction` run once per card **per multiplier repetition** (the same card is pushed up
to 4 times for AIStats ranking, and each repetition re-parses). Also 2884-2894 calls
`getStats()->isInTop` three times per card, each a linear `list` walk. `AIHints::HintSaysDontBlock`
(AIHints.cpp:152-170) builds a `TargetChooser` from the hint string per hint per creature; chooseBlockers
calls it in five passes (4449, 4503, 4519, 4633, 4718, 4768). SUSPECTED impact (the selfplay decks
carried no hints and few multi-target spells; counters showed 0 re-parses there), but the shape is
O(cards x hints) parser calls per blocking tick on hinted decks. Fix: parse `belongsToAbility` once
before the loop; compute `isInTop` once per card; cache the hint TargetChoosers in `AIHint`
(they are strings today) or at least hoist the per-pass creation out of the creature loop.

**M6. PERF/IO — AIStats: a file read at the first event and a file write at every game end**
`src/AIPlayerBaka.cpp:4902-4911, 5190-5197`, `src/AIStats.cpp:147-193`. VERIFIED: the worktree's
`User/ai/baka/stats/testsuite.stats` was rewritten during the suite run (mtime inside the run,
content halved each save: 52565 -> 17237 ...). Every game — 1,215 per suite run, and one per match
on the Vita's flash — opens+writes `ai/baka/stats/<opponent>.stats` from the AI's destructor; every
damage event walks the whole action layer with a `dynamic_cast<ALord*>` per ability (AIStats.cpp:85-95)
and `list::sort`s the stat table (104). `save()` stores `value/2` (186), so the table decays toward 0
and `isInTop` mostly answers on `tooSmallCountsForTrue`; the feature is nearly inert while still
paying the I/O. Fix: skip load/save entirely when `observer->mSuiteGame` or headless; save only if
`stats` changed (dirty flag); replace the per-event lord scan with the `ALord` list the action
layer already maintains, or drop the aura/lord attribution.

**M7. CRUD — dead scaffolding in the AI tree**
`src/AIPlayerBakaB.cpp` (186 lines) + `include/AIPlayerBakaB.h`: every method is a one-line
pass-through to `AIPlayerBaka`, compiled only under `AI_CHANGE_TESTING`, which `include/config.h:34-36`
defines only for `_DEBUG && WIN32`; on every shipped target the file is dead and the `#ifndef
AI_CHANGE_TESTING` guards in `computeActions` (3727, 3770, 3941, 3948) are permanent. VERIFIED.
`src/GameOptions.cpp:293-305` `menuStr` formats a buffer it never uses; `src/DeckStats.cpp:397`
`if(!this) return;` (UB, optimised away); `src/DeckStats.cpp:339-360` "loop" over AI decks that
only ever checks `deck1`; `src/CardPrimitive.cpp:684` `if (types.size() > 400) return false; // Null pointer?`
guards nothing. `src/MTGPack.cpp:226` `getCardByName(pEntry->Value())` looks up the literal element
name `"card"`, so a pack `<card>` entry can never resolve — dormant today (no shipped
`Res/packs/*.txt` uses `<card`); note it, don't spend on it.

### LOW

**L1. MEM-SAFE — small buffer/NULL hazards** (all SUSPECTED, reading only)
- `src/utils.cpp:356-358, 372-374` `wordWrap`: `while (sentence[idx-1] != ' ') idx--;` with
  `size_t idx` walks past index 0 into `sentence[SIZE_MAX]` when the first word alone exceeds the
  width (DeckMenu descriptions, TextScroller). Fix: bound the walk at `breakIdx`.
- `src/PriceList.cpp:37-41` `char writer[20]` for `"%i\n%i\n"` — an 11-char negative token id plus
  a 7-digit price overflows; use the stream directly.
- `src/MTGDeck.cpp:1248` `meta_name.substr(4)` throws `out_of_range` for a deck basename shorter
  than 4 chars; `1331-1333` numeric `#CMD:` branch dereferences `newcard` without the NULL check the
  by-name branch has; `src/AIHints.cpp:323, 336` `hintTc->countValidTargets()` before the `hintTc &&`
  test that follows it.
- `src/GameOptions.cpp:231-268` `asColor` returns an `ARGB` built from an uninitialised `color[]`
  for 2-field input (the TODO at 266 says so).
- `src/MTGDeck.cpp:1817` `printDetailedDeckText`: `setInfo->id` with `setInfo` possibly NULL
  (`getInfo` returns NULL for `INTERNAL_SET` cards); also `1823` streams a `ManaCost*` — an
  `operator<<(ManaCost*)` exists, so fine.

**L2. PERF — DeckStats copies whole maps by value per query**
`src/DeckStats.cpp:51, 79, 91, 107, 187` `map<string,DeckStat*> stats = masterDeckStats[currentDeck];`
— a full map copy per `getDeckStat/nbGames/percentVictories/save`, called per opponent entry when the
deck menu builds. `getDeckStat` (61) matches by substring, so `ai_baka_deck2` resolves to `deck20`'s
stats when `deck2` is absent. Fix: `const map&` + exact `find`.

**L3. MEM-EFF — `CardPrimitive` still carries per-object members that could be side-tabled**
`include/CardPrimitive.h:53-58, 73-76`: `formattedText` (24 B vector, empty for every primitive
until rendered), `nameOrig` (empty on all 27,788), `backSide` (458 populated), `partner` (37),
`spellTargetType` (4,035), `magicTexts` map header (48 B desktop) — ~5 mostly-empty members per
primitive next to the five already moved to `RareStrings`. On a 32-bit target each empty string is
24 B: ~3 MB across the DB for fields populated on <15% of cards. Same side-table pattern as
CardPrimitive.cpp:30-75.

**L4. CRUD — trailing per-file items**
`src/AIPlayerBaka.cpp:1567-1584` `chooseCard` walks 7 zones x 2 players for a *cost* target
(`CanHandleCost`) on every potential-mana enumeration (comment at 110-116 knows it is hot);
`src/AIPlayer.cpp:218-244` `char deckFile[512]` sprintf of `opponent->deckFile` (a path chosen by
the user's profile); `src/MTGDeck.cpp:2053` `char myFilename[4096]` + `NEW MTGPack` (a file open)
for all 336 sets at load; `src/DeckManager.cpp:108-120` a `StatsWrapper` (stats file read) per deck
added to the opponent list.

## Not read / not measured
- `AIPlayerGPT.cpp` (slice 7), `AllAbilities.cpp`/`MTGAbility.cpp` internals beyond `abilityEfficiency`,
  `ManaEngine.cpp` beyond `potentialMana`, `LegalActions.cpp`, `MTGCardInstance.cpp`, `Player.cpp`.
- No Vita/PSP measurement; the 32-bit sizes are inferred (`string`=24 B) from the file's own comments.
- The Baka probe ran one matchup (deck5 vs deck12, ~2 games); hinted decks and multi-target-heavy
  decks (M5) were not exercised, which is why M5 is SUSPECTED.
- ASAN/LSan covered 296 of 1,215 fixtures (first 250 + the 46 [AI] ones); the full list does not
  finish inside the 600 s cap under the 4-agent build contention of this session.
