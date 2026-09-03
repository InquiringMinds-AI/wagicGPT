# Wave-54 audit lane K = synthesis "Lane E" (heuristic AI, deck DB, harness)

Branch `w54-lane-K`, seeded from master dab9f72f4. Items: A3, A4, A12 (Baka sites), A33
(Baka printf/DebugTrace), A36, A37, A38, A39 (I/O only - keep/drop is owner item O9), A51,
L19, L20, L22. Files touched: `AIPlayerBaka.cpp/.h`, `AIStats.cpp/.h`, `GuiHand.cpp/.h`,
`MTGDeck.cpp`, `DeckStats.cpp`, `AIHints.cpp`, `AIPlayer.cpp`, `utils.cpp`, `PriceList.cpp`,
`GameOptions.cpp`, `TestSuiteAI.cpp/.h`. Every code comment is tagged `#W54-K (item)`;
header additions sit in `// audit-K` blocks.

## Gate (this worktree, probe-free binary built 22:16)

- Suite (base-gate recipe, default threads): **1219 tests, 2 failed** = `lifeline.txt` +
  `merrow_reejerey.txt` (the known concurrency-only pair; re-run solo single-threaded:
  **2 tests, 0 failed**), **49 AI / 0 failed**, 0 timeouts. Same numbers from the base
  binary (`archives/wagic-ef133ea2a-w54base`) on this worktree's Res, run under perf for the
  before-profile: 1219/2, 49/0.
- PARSETEST: **2258 passed, 0 failed** (= baseline; no parse-relevant string changed).
- `git diff | grep -c U+FFFD` = 0.

## A3 - `selectAbility` / `doAbility` activation scan (HIGH, PERF)

Shipped: `AIPlayerBaka::rankActivations` + `rankPair` + `abilityCanReactTo` replace the five
identical zone rungs in `selectAbility` and the near-copy in `doAbility`. Per ability the
candidate cards are its `source` and (for the overrides that react to their target, e.g.
`AUnBlocker`) its `target`, filtered by zone membership (`MTGGameZone::hasCard`, an O(1)
`currentZone` compare); abilities with no source (the rules-layer objects) keep the full
five-zone scan because they react to any card. `isReactingToClick` runs once per pair (was
twice), the specific-mana plan vector is cleared before every pair (the N-116g stale-plan
carry is gone from the scan itself; the payment step already recomputed for the chosen
action), and `getPotentialMana(card)` is memoised per distinct card for the call. The
attack/block-cost plan `doAbility` builds is keyed by ability (`costPlans`) instead of riding
the shared vector. **Disable flag: `WAGIC_BAKA_LEGACY_SCAN=1`** restores the every-pair scan
with the stale carry, on the same binary.

How every override was checked: all `isReactingToClick` overrides in `MTGAbility.h`,
`AllAbilities.h/.cpp`, `MTGRules` were read - `ActivatedAbility::isReactingToClick` (and
everything delegating to it: Generic*, AUpkeep, AAMorph, AAMulligan, ActivatedAbilityTP)
requires `card == source`; `ASpellCastLife`, the cumulative-upkeep/`AADamagePrevent`-style
overrides at AllAbilities.h:6462/6558/6872/7042 require `card == source`; `AUnBlocker`
(:2848) requires `card == target`; `MTGAbility`'s base returns 0; the MTGRules classes are
`PermanentAbility(observer, id)` with a NULL source. Nothing reacts to a card that is
neither its source nor its target, so the candidate filter cannot drop a reacting pair.

**Falsifiable measure - the RANKED SET.** A worktree-only in-process A/B (removed before
commit): on every `selectAbility` call the legacy scan and the new scan ran back to back on
the SAME game state and the two `RankingContainer`s were compared as multisets of
(ability, click, target, playerTarget, player). This is stronger than "same seed" - the RNG
is time-seeded in self-play and there is no seed env, so the comparison was made per call
instead. Headless Baka-vs-Baka:

| game | selectAbility calls | ranked-set mismatches | pairs legacy -> new | probes legacy -> new | planner calls legacy -> new | ranked (both) |
|---|---|---|---|---|---|---|
| deck5 vs deck12 (the docket's pairing) | 211 | **0** | 263,885 -> 74,656 | 277,861 -> 74,766 | 2,769 -> 78 | 110 = 110 |
| deck164 vs deck164 (Nazgul mirror, wide boards) | 128 | **0** | 163,134 -> 45,850 | 163,141 -> 45,857 | 1,431 -> 66 | 7 = 7 |

(The legacy "probes" column counts one probe per pair because the double call was removed
for both modes; the original code's count was 2x pairs, i.e. the ledger's 2,094,236 for
1,047,118 pairs.) Pairs do not fall all the way to "= abilities" because the ~30 rules-layer
objects have no source and legitimately keep the full scan: ~354 pairs per call remain, of
which ~330 are rules x 11 cards, each a cheap early-exit probe. The expensive parts are
where the ratio is: planner calls 2,769 -> 78 (target < 1,000), per-pair potential mana
gone.

**Timing** (temporary `clock_gettime` around `computeActions` on decision ticks, bucketed
by total permanents on both battlefields; each mode ran its own game, so boards differ
slightly). Parallel `-O2` build (`Makefile.sdlO2` + `objs-o2`, deleted afterwards), 30 fps
`dt` (`WAGIC_FASTCLOCK=0.0333`), avg microseconds per decision tick:

| permanents | legacy 5v12 | new 5v12 | legacy 164 | new 164 |
|---|---|---|---|---|
| 8-11 | 888 | 226 | 1,877 | 2,557 (n=13, one 29 ms outlier) |
| 12-15 | 1,750 | 631 | 4,256 | 280 |
| 16-19 | 13,546 | 308 | 7,905 | 552 |
| 20-23 | 26,436 | 628 | 12,736 | 332 |
| 24-27 | **39,215** (max 158 ms) | **1,644** (max 10 ms) | - | - |
| 28-31 | 71,796 | 1,658 | - | - |

Target was `Act` at 24-27 permanents 10 ms -> <= 2 ms: **1.64 ms avg**. (The legacy
numbers here exceed lane W's 10 ms because this pairing carries many costed abilities -
12,216 planner calls in the legacy game.) At -O0 the same shape: 16-19 permanents 11.1 ms ->
1.2 ms on the Nazgul mirror.

## A4 - `GuiHandSelf::Update` change-driven refresh (HIGH, PERF)

Shipped: (1) an AI-owned hand skips the oracles entirely - the flags it writes are read
only by `CardGui::Render` (grep: no engine/AI/suite reader; TestSuiteAI's `assertcastable`
asks the oracle directly), so self-play, the demo and every suite seat stop paying for a
display nobody reads; (2) for a human hand the two oracle calls run only when
`mDisplayDirty` (set by ANY event reaching `receiveEventPlus`) or a state signature moved
(phase, turn, currentPlayer, currentlyActing, stack NOT_RESOLVED count, action-layer object
count, pool string, hand pointers, battlefield pointers + tap state). The tap-preview memo
now keys on that signature + the focused card (its old signature was a subset). The 0.25 s
timer stays as the poll cadence for the signature. **Disable flag:
`WAGIC_HAND_REFRESH_ALWAYS=1`** restores the unconditional refresh (human hand).

Measure (perf, headless suite, children share): before (base binary)
`usableAbilityCards` 8.31% + `castableForDisplay` 5.01% = **13.3%**; after **0.01% +
0.01%**. Target was 15.1% -> < 2%.

## A39 - AIStats I/O (MED) - feature kept, owner item O9 untouched

Shipped: `AIStats(player, file, persistent)`; a suite (`observer->mSuiteGame`), headless,
`WAGIC_TESTSUITE` or PARSETEST process builds a non-persistent table (no file read on the
first damage event, no rewrite at game end); `save()` also skips when nothing changed
(`mDirty`). Decisions are unchanged: `testsuite.stats` in this worktree held ONE entry (3
lines), so `isInTop` already answered on `tooSmallCountsForTrue` - exactly what an empty
in-memory table answers. Evidence: `User/ai/baka/stats/testsuite.stats` mtime stayed at
21:56 (the base binary's perf run) through the 22:17 gate and the perf-after suite, and
`ai_baka_deck5/12.stats` were untouched by the probe games. NOT done: the per-damage-event
`ALord` scan + `list::sort` - the action layer maintains no lord list (checked), so that
half needs either a new index or the owner's O9 verdict.

## A36 - `getCardByName` (MED, load time)

Shipped: the three printing scans (`getCardByNameFaceSibling`, the forced-set loop, the
name loop) compare `CardPrimitive::getLCName()` instead of lower-casing a fresh copy of
every printing's name per lookup. Not done: the one-time name index (the prefetch path) -
the lcname compare removes the allocation storm with zero cache-semantics risk. Deck load
timing (temporary timer in the `MTGDeck(config_file)` ctor, headless self-play start, three
runs each, same binary, legacy path behind a temporary env): first 60-card deck 42.7 / 23.9
/ 51.8 ms -> 10.1 / 21.6 / 14.2 ms; second deck 133 / 117 / 143 ms -> 64.6 / 69.0 / 62.3 ms.
The residual is the ctor's other work (set lookups, the per-card copies), not the name scan.

## A37 - the getEfficiency copies (MED) - PARTIAL, deliberately

Shipped: the NULL guard on `GAA->ability` in `getRevealedEfficiency` (the reachable crash)
and `SAFE_DELETE(transAbility)` before the `return 99` castcard exit in BOTH functions (the
leak). NOT shipped: the merge into one function. A normalised diff of the two bodies shows
the drift is far larger than the ledger's "five places": the revealed copy also lacks the
`STANDARD_BECOMES` else-branch, `AATurnSide`, the GenericReveal/Scry recursion (which is
WHY it is a separate function), the sacrifice-cost `/10` rule, the planeswalker/land tail,
and the `efficiency` cache. A merge that preserves both outputs needs ~10 knobs, and a merge
that does not changes the ranked set for every `aicode=` reveal/scry card - outside this
lane's identity measure and with no fixture pinning those scores. Recommend a next-wave
item with a scoring pin before merging.

## A38 - `chooseTarget` re-parse (MED, SUSPECTED)

Shipped: `tc->belongsToAbility` is parsed ONCE per `chooseTarget` (was once per candidate
card per multiplier repetition) and the per-card `OrderedAIAction` is a stack object. NOT
done: caching `AIHints` TargetChoosers - `createTargetChooser(hint, card)` takes the creature
as the chooser's SOURCE, so the chooser is card-dependent and cannot be cached per hint
(the "other"/"another" semantics ride the source); the hoist would need a source-agnostic
chooser, which the factory does not offer. The `isInTop` triple walk is left (the stat list
is tens of entries).

## A12 (Baka sites) - four `NEW ManaCost(ManaCost::parseManaCost("{0}"...))` in `doAbility`
are now stack `ManaCost` objects (identical cost, no leaked parse temporary).

## A33 - the unconditional `printf("Choose attackers ...")` is a `DebugTrace`; the twelve
identical `ai is doing a combo` traces are one.

## A51 - `TestSuiteActions::actions` is a `vector<string>` (grown on `add`),
`buildDeck`'s `int list[100]` a `vector<int>`; `MAX_TESTSUITE_ACTIONS` is now unused.

## L19 / L20 / L22

- `wordWrap`: the back-walk to the previous space is bounded at the line start (a first
  word wider than the line walked `idx` to 0 and read `sentence[SIZE_MAX]`).
- `PriceList::save`: streams the ints (was `char writer[20]`).
- `MTGDeck`: `meta_name.substr(4)` guarded; numeric `#CMD:` branch NULL-checks `newcard`;
  `printDetailedDeckText` tolerates a NULL `setInfo`.
- `AIHints`: `countValidTargets()` after the NULL test (two sites).
- `GameOption::asColor`: `color[]` initialised `{0,0,0,255}`.
- `DeckStats`: no whole-map copies (references); `getDeckStat` tries the exact key first and
  the substring fallback only accepts a match ending at a token boundary (`deck2` no longer
  resolves to `deck20`).
- `AIPlayerFactory::createAIPlayer`: `std::string` deck paths (was `char[512]` sprintf of a
  profile path).
- NOT done from L22: `chooseCard`'s 7x2 zone walk (it already returns at the first hit; the
  cost is the no-target case and needs a different probe), `MTGSetInfo`'s `NEW MTGPack` per
  set at load (its `mPack` member is read directly by `GameStateShop.cpp`, outside this
  lane's files - a lazy accessor is a two-file change), `DeckManager`'s stats read per
  opponent deck.

## What I did NOT verify

- **The hand-glow / tap-preview manual check was NOT done.** It needs a window on the
  owner's display (the display is his; a lane agent cannot share the pointer). The A4
  change is exercised headless only through the suite (`assertcastable`/`assertusable`
  fixtures ask the oracle directly, not the display flags) and through the compile. The
  falsifiable prediction for his next desktop or VPK session: the hand glow and the blue
  tap-preview border still update on every phase change, cast, tap and hand move, and a
  static board no longer costs the 4 Hz refresh. If a glow ever looks stale, the
  one-flag check is `WAGIC_HAND_REFRESH_ALWAYS=1` on the same binary.
- Nothing measured on the Vita; the -O2 desktop numbers are the proxy.
- The -O2 timing games were separate games per mode (no seed pinning exists), so the
  per-bucket averages compare different boards of the same width; the ranked-set identity
  was measured in-process instead and is exact.
- A39's `ALord` scan half, A37's merge, A38's hint-chooser cache, L22's three trailing
  items: not shipped, reasons above.
- `WAGIC_TESTSUITE_THREADS=1` full run not repeated (the two failures were re-run solo).
