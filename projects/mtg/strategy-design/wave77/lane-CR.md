# Wave-77 lane CR — the window/hold and meter family (R2, R1, R8, R9, R11)

Base: master `49c19f413`, worktree `worktrees/lanes/w77-CR`, branch `w77-lane-CR`.
Corpus for every repro: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-141526-final`
(+ the 19 `-rerun-NN-*` pool dirs for R11 b).

**Gates.** Seeded tree: suite 1291/0 + 76 AI/0, PARSETEST 6168/0.
After the lane: **PARSETEST 6222 passed, 0 failed** (+54), **suite 1291 tests (0 failed),
76 AI tests (0 failed)** single-threaded, detached unit `w77-CR-suite`, both failure
strings counted (`==Test Failed !==` 0, `==Test timed out` 0; none of the three known
flakes fired). `check-ctor-init` OK (118 files), `check-reply-instructions` OK
(1 source, 37 guides), `bash -n tools/selfplay-harness.sh` OK,
`--selftest` harvest **8 checks / 0 failed** (was 4), pilot-stall 17, window-loop 12,
regime-gate 30, all 0 failed. `git diff | grep -c U+FFFD` = **0**.
Files touched: `src/AIPlayerGPT.cpp`, `include/AIPlayerGPT.h` (both ASCII/LF),
`tools/selfplay-harness.sh`.

Tools written this lane take the run dir as an argument and echo it on their first
output line (LESSON OF WAVE 75 (2)): `~/.gatelogs/w77cr/holdcheck.py`,
`~/.gatelogs/w77cr/adjudicate.py`, `~/.gatelogs/w77cr/holdkey.py` (a faithful python port
of `holdActionKeyRow`, verified against the engine's own printed counts before use).

---

## R2 HIGH — the hold-check bracket, three residuals

### (a) the land-drop window shares the `"cast"` seam — CONFIRMED, and the mechanism is a NOTE LEAK as well as a seam collision

**Repro.** `holdcheck.py` over the corpus: 1,377 bracket renders, **338** land-drop
records, and **11 land-drop records CARRY A HOLD-CHECK BRACKET** — a bracket that window
never measured, because the land-drop builder never called `holdReopenNote` at all:

```
run dir: .../matchups-20260910-141526-final
{'brk': 1377, 'land_brk': 11, 'land': 338}
  125v130 deck130 seq 7 | 125v162 deck162 seq 20 | 146v125 deck146 seqs 59, 97
  146v126 deck126 seq 11 | 146v162 deck162 seq 12 | 152v125 deck125 seq 50
  162v130 deck130 seqs 8, 28, 46 | 126v146 deck146 seq 15
```

all eleven reading `every row above was also on the menu at the last window I asked you at
this seam` over `Play Mountain / Play no land right now`. Those are the same seqs
engine-seat HIGH-2 names, and they are HALF the 22: the other half are the casting windows
whose previous bracket-carrying record is one of these eleven.

Source-traced: `askModel` swaps `mNextAskPromptNote` out on every exit path IT reaches,
but the casting builder WRITES that note (`mNextAskPromptNote = declinedListNote(...)`,
`+= mCastHoldNote`, `+= loopChainingNote(...)`) and only then takes one of its three
no-ask returns (`holdHonoured`, `reserveDeclineHonoured`, `loopAutoPassWindow`). The note
survives to the next askModel call — which is very often the land drop.

**RED evidence (executed).** `#W77-CR R2a REPRO` drives `w76HoldReopenNote` /
`w76HoldWindowAsked` through the wave-76 routing (both windows at `"cast"`) and asserts the
casting menu then reads `2 rows above are new` with a land menu as its referent.

**Fix.** Three parts, all small:
* `writeTransLog` routes a `kind: ask` record's commit by the window's own class —
  `mLogWindowKind == kAskWindowLandDrop ? "land" : "cast"` — which askModel has already set.
* the land-drop builder calls `holdReopenNote("land", opts)` and appends it to the
  prompt-only channel, so the land drop gets its OWN correct bracket and its own run.
* `w77DropUnaskedCastNote(promptNote, castNote, refSeq)` (free function, member wrapper)
  clears all three at each of the casting builder's three no-ask returns.

**GREEN.** `#W77-CR R2a GREEN` ×4: the casting menu's referent is the last CASTING window;
nothing on it reads as new; the land seam keeps its own run; the drop helper clears all three.

### (b) the streak-1 comparison with nothing compared — FIXED by stating the true token

**Repro.** `146v126` deck146 seq 29 prints `every row above was also on the menu … (1 window
in a row now)` over `Cast Kaya / Cast Lolth / Cast Acererak`, while the previous record at
that seat's cast seam is seq 28, the LAND DROP (this is (a)'s other half — the leak
manufactures the streak-1 population). Separately, at the genuine first window at a seam the
note is the EMPTY STRING: the one menu with no history gets no hold guidance at all, and the
very next menu carries a paragraph about a "last window" the model has never seen.

**RED evidence (executed).** `#W77-CR R2b REPRO`: `holdReopenNoteText(0, 0).empty()` — the
base reading, executed on this tree.

**Fix.** `holdReopenNoteText(unseen, repeats, first = false)`. With `first` the note reads
`[hold check: this is the first window I have asked you at this seam - there is no earlier
menu here to compare it with yet; from the next one on this bracket says which rows have
moved since the last window I asked you at this seam]` — no comparison, no run. The default
argument keeps every existing caller and pin byte-identical. #W62-AC's rule applies: a
silent omission is worse than wrong text, because the model confabulates into gaps.

**GREEN.** 6 pins: the sentence; no comparison and no run inside it; it is a `\n[` bracket;
`stripNarrationDecoration` drops it; the LIVE memory prints exactly it at its first window;
the SECOND window is the first comparison at run 1.

### (c) the `N rows above are new` COUNT — **REFUTED**; the count was already correct

**Repro, and the correction.** `holdkey.py` is a byte-faithful port of `holdActionKeyRow`
(verdict markers, the combat-next erase, balanced-brace stripping with the mana-pip
carve-out, `stripRenderAnnotationsLc`'s bracket/`(land…)` rules, the life-projection
normaliser, `[cost: …]` lift). Run on the two cited cases it reproduces the ENGINE'S OWN
printed numbers exactly:

```
unseen 53->54: 1   (130v162 deck130 - the engine printed `1 row above is new`)
unseen 11->14: 3   (130v146 deck130 - the engine printed `3 rows above are new`)
```

and names the rows. They are not reorder/disappearance artefacts:
* `130v146` seqs 11 → 14 is the same five CARDS reordered, but three rows carry a
  `- legal targets right now: …` roster OUTSIDE any annotation group, and all three rosters
  changed (Molten Rain / Stone Rain gained `swamp`, Hammer's target list changed wholesale).
* `130v162` seqs 53 → 54: Rorix Bladewing left the menu AND became a legal Hammer target, so
  Hammer's row names a target it did not name before — `6 legal targets` → `7`.

Under the hold row's own printed contract ("only a row appearing, disappearing, or naming a
different card, cost or target" re-opens a hold) both counts are TRUE. `w74HoldUnseenRows`
is already "rows of the current menu whose action key is absent from the referent's key set",
order-independent. The wave-76 reading compared CARD NAMES, not rows.

**Fix.** None to the arithmetic. The four pins the brief requires are added so the property
cannot regress silently, plus the wave-74 key-stability pin.

**GREEN.** `#W77-CR R2c` ZERO / REORDER / ONE / DISAPPEARANCE through
`w74HoldUnseenRows(…, holdActionKeyRow)` — the live seam's own pair — plus the printed
sentence for each of the three non-trivial cases, plus `KEY PIN` that two rows differing
only in a projected life total key identically and count 0 new.

### (d) `hold_check_ref_seq` on the record

`W76HoldMemory` gains `lastSeq` (the window seq `last` was committed from) and `measuredRef`
(the referent the note at `measuredSeq` names, -1 = none). `AIPlayerGPT::holdReopenNote`
stamps `mHoldCheckRefSeq`; `writeTransLog` consumes it ABOVE the logging-off early return
and emits `hold_check_ref_seq` on the record that carries the bracket. One integer makes the
whole class decidable from the translog instead of re-implemented in Python — which is
exactly lane CN's weakest-evidence #1, and which this lane had to do again to write (c).

**GREEN.** 4 pins: -1 at the first window; a recorded window becomes the referent; the
bracket at seq 9 names seq 4; a BUILT-and-not-asked window is never the referent.

**Prediction (falsifiable, with a pin for the negative).** Next corpus: **0** land-drop
records carry a `[hold check: …]` bracket whose referent is a casting menu (wave 76: 11 of
338), and every bracket's `hold_check_ref_seq` names a record at the SAME seam class
(cast↔cast, land↔land, priority↔priority) — adjudicable with one `grep` and no key
re-implementation. NEGATIVE clause and its pin: the fix must not SILENCE the bracket — the
first window at every seam now prints one, so bracket renders should RISE, not fall, and
`#W77-CR R2b GREEN the live seam's first window prints exactly it` plus
`#W77-CR R2a GREEN the land seam keeps its own run` are the guards; if the next corpus shows
land-drop windows with no bracket at all, the land seam's note is being dropped before
askModel consumes it.

---

## R1 HIGH — 32 model calls to watch a won loop resolve itself

**Repro.** `126v123` deck126 seqs 30-61 (t17 Main 1), two alternating 4-row Sorin menus,
`chosen_text` `pass` on all 32, opponent 17 → 1 and seat 24 → 40.

**What the wave-76 seat got wrong, measured.** engine-seat HIGH-1 says "the hold latch
cannot close it (the rows' life-derived annotations move)". Run `holdkey.py` over seqs 30
and 32 (a link where BOTH lives moved): the four action keys are **byte-identical**
(`+1: create a 1/1 vampire with sorin, lord of innistrad [cost: counters]`, …). The latch
WOULD have held all 32 links. The loop was never a latch problem — the model simply never
took the hold row, because nothing on the screen said the hold answers all of them.

**RED evidence (executed).** `#W77-CR R1 NEGATIVE with no resolving own loop nothing is
added` is the base rendering of every one of those 32 windows, executed; the GREEN pins
below do not compile against the base tree (the helper does not exist there) — named in
Weakest evidence.

**Fix (annotation + the hold path; nothing collapsed, nothing auto-answered).**
* `w77OwnLoopResolving()` = the seat's OWN `lifeLoopProvenWin(this)` **and** something
  NOT_RESOLVED on the stack. Both halves: the proven half alone is a board the seat merely
  controls, and "your loop is resolving on its own" over an empty stack is a false surface.
* `w77ApplyOwnLoopFeed(rows, resolving)` appends, to every ACTING row (the
  `w72RowIsDeclineOrHold` filter the latch itself uses), ` [your loop is resolving on its
  own - this row is not needed to win it; the HOLD row on this menu covers every link of it
  until the loop ends or something else changes]`. Applied at BOTH seams in exactly the
  place and the manner `w75ApplyChainFeed` is (priority: `shownLines` + `renderRows`; cast:
  `menu`), so the clause and the printed list cannot disagree. Idempotent — the prompt is
  rebuilt on every polling tick.
* **DEVIATION FROM THE BRIEF, deliberate**: the clause names the hold row by its text, not
  as `HOLD (row N)`. The priority seam emits its numbered rows into `tail` BEFORE the hold
  row's index exists (`int holdRow = ++index;` runs after the repeat block), so a row number
  there would have to be PREDICTED, and a wrong row number is exactly the false surface the
  trust doctrine forbids. The clause therefore carries no number at all — which also makes
  the wave-74 key-stability requirement trivially true.
* `own_loop_windows_asked` on the gameend record, counted once per WINDOW
  (`mOwnLoopCountedSeq == mWindowSeq`), never once per polling tick.

**GREEN.** 11 pins. The feed reaches acting rows and only those; two rows in, two answerable
rows out (**nothing removed, collapsed or auto-answered**); `holdActionKeyRow` and
`optionSetKeyOf` are byte-identical with the clause and without it (it is a `[…]` group);
and **the hold path**: a fixture walks **five links** of the loop after the hold is taken,
with `they would be at N` / `you would be at N` ticking on the row at every link and both
verdict markers in the held set — `holdStillStands` stands at all five, **0 asks**. Plus the
MUST-NOT-MATCH that a newly available row still re-opens the window, so the latch is not a
blind cache.

**Prediction.** Next corpus: `own_loop_windows_asked` is non-zero on any seat that closes
its own life loop (wave 76: 35 at the deck126 seat, uncounted), and the longest own-loop run
at one seat in one phase falls well below 32 because the hold row is taken. NEGATIVE clause
and its pin: no window may be removed — `#W77-CR R1 KEY PIN NOTHING IS REMOVED, COLLAPSED OR
AUTO-ANSWERED` and the re-open MUST-NOT-MATCH are the guards; if a corpus shows a seat whose
own-loop windows DROP to zero asks without a `took the hold row` trace, something is
collapsing windows and must be reverted.

---

## R8 MED — Q13's board clause: not inert, but the negative sentence undid the bracket

**Repro (measured, not assumed).** 109 brackets; diffing the two prompts' SITUATION board
block for every pair: **5 board-identical, 104 genuinely moved**. The movers are real
(`Opponent hand size | Opponent library` 88, `Your hand (…)` 59+, `Opponent battlefield` 40,
`Your library` 32, `Your life | Opponent life` 28, the stack line 22). So `w76PhaseFreeBoardKey`
is NOT too strict: between two phases of one turn a draw step moves the board for real, and
widening the key to hide that would make the POSITIVE clause a lie. The defect is what the
seat was left with — told a list it already declined is being re-put AND that the board
moved, whose cheapest reading is "re-read everything", the opposite of the intent.

**RED evidence (executed).** `#W76-CQ F4 REPRO/GREEN` (revised) pins the exact bytes of the
negative branch; it FAILED on this tree the moment the wording changed, which is what makes
the pin the guard rather than a decoration.

**Fix.** The negative branch states the fact the cross-phase KEY guarantees, beside the fact
that the board is not: `; the board has moved since then, but this list has not - not one
row on it appeared, went away, or changed a card, cost or target`. The comparison itself is
untouched (the ask-cache board key below the phase line). New gameend field
`crossphase_board_unchanged`, counted on the same one-per-window gate as
`crossphase_identical_reputs` and never summed with it.

**GREEN.** 6 pins including the one the brief names: **the POSITIVE clause prints on the
152v125-class shape** (the same board serialised at two phases compares equal below the
phase line, and `w76CrossPhaseRePutNote(…, true)` prints `nothing on the board has
changed`), the MUST-NOT-MATCH that a board that really moved is not equal, that the two
sentences are exclusive, and the prompt-only-channel key pin.

**Prediction.** Next corpus: `crossphase_board_unchanged` is non-zero and small relative to
`crossphase_identical_reputs` (wave 76's offline split: 5 of 109), and no bracket prints
`nothing on the board has changed` over two asks whose serialised boards differ below the
phase line. NEGATIVE clause and its pin: if `crossphase_board_unchanged` is 0 across a
corpus that contains a same-turn multi-phase re-put over a frozen board, the positive clause
has gone dead — `#W77-CR R8 GREEN the POSITIVE clause prints on the 152v125-class shape` is
the guard.

---

## R9 MED — the third exemption and the no-PLAN skip (measure only)

**Repro.** `plan_names_uncastable_zone_card` 102 corpus-wide. deck126's 42 are all
protocol-legal TUTOR-TARGET plans ("cast Idyllic Tutor, find Sanguine Bond" — the named card
is what the seat will FETCH); deck146's ~15 are reanimation objects, spell targets, stack
objects and opponents' hand cards. The predicate was `planNamesStrandedCard`, a bare
substring of the plan.

**RED evidence (executed).** `#W77-CR R9 REPRO the base substring test fires on deck126's 42
protocol-legal TUTOR-TARGET plans` — `planNamesStrandedCard("cast Idyllic Tutor, find
Sanguine Bond", "Sanguine Bond")` is true on this tree.

**Fix (measure only; nothing is refused, re-asked or rendered).**
* `w77PlanNamesAsCastIntent` — the name counts only when a casting verb (`cast`, `casting`,
  `play`, `playing`, `recast`, `hardcast`, `flashback` — the list the cast rows and the reply
  protocol already use) sits within 24 bytes before it, so one short qualifier is allowed and
  a verb elsewhere in the line is not. `planNamesStrandedCard` is UNCHANGED: it is the right
  floor for the reserve-verdict measure, where the row already priced the strand.
* the no-PLAN skip: the measure runs only when THIS reply carried a plan line
  (`firstLineLeadingPlanPos(reply) != npos`). `mCurrentPlan` is the last plan the model
  stated, carried forward, so a record with `plan_line_missing` (91 corpus-wide) was being
  measured against an earlier window's sentence.

**GREEN.** 8 pins: the tutor target is not a cast intent; the tutor ITSELF still is; a
self-recursion plan naming the cast counts; `play`/`casting` count; a reanimation object and
a blocker do not; a verb 28 bytes away does not; empty plan and short name name nothing.

**Prediction.** Next corpus: `plan_names_uncastable_zone_card` falls by roughly deck126's
tutor-target population and deck146's non-cast population (wave 76: ~57 of 102), while
staying non-zero on the shape it exists for (a plan that says it will CAST a card sitting in
the seat's own graveyard). NEGATIVE clause and its pin: the exemptions must not empty the
counter — if the next corpus reports 0 across all seats, the verb window is too narrow;
`#W77-CR R9 GREEN ...and the card the plan says it will CAST still is` and the
self-recursion pin are the guards.

---

## R11 LOW — the same-arm bound, and the harvest's manifest

### (a) same-arm force-close re-arms

**Repro.** Wave 76's five `forced_close_unrecorded` are all same-arm re-arms with an EMPTY
park: `force_close_arms_refused` 0, `force_close_arms_deferred` 0,
`force_close_defer_bound_hits` 0 across 21 games — the Q8/F2 bound never fired once, because
it only ever looked at the OTHER arm.

**RED evidence (executed).** `#W77-CR R11a REPRO`:
`w76ForceCloseDecision(true, false, 0) == kW76CloseArm` — with an empty park the base
decision arms at once, however many closes THIS arm has outstanding.

**Fix.** The caller's busy test folds in `mForceCloseArmed`, so a re-arm while this arm's
close is still outstanding takes the SAME bounded deferral F2 built (no decode bought,
decision stays `kChoicePending`, armed anyway at `kW76CloseDeferMaxTicks`). Its own meter,
`force_close_same_arm_deferred`, never summed with the park's. Waste only, no loss — a close
that genuinely cannot resolve still arms and is still counted unrecorded.

**GREEN.** 5 pins: the REPRO; the deferral; the 64-tick loop showing exactly
`kW76CloseDeferMaxTicks` waits and the rest arms; neither-arm-busy arms at once; an unearned
close is still the retry ladder's.

**Prediction.** Next corpus: `forced_close_unrecorded` falls and
`force_close_same_arm_deferred` accounts for the difference, with the gameend identity
`recovered + missing + unrecorded == closes` still holding in every game. NEGATIVE clause
and its pin: the bound must not cost a decision — a non-zero
`force_close_same_arm_deferred` alongside any `reasoning_forced_close` fallback or a
non-zero `phase2_answer_missing` means the deferral is eating rescues and must be reverted;
`#W77-CR R11a NEGATIVE with neither arm busy the close arms at once` guards the common path.

### (b) the harness harvest keys on the run's own game manifest

**Repro (executed on the base harness).** The 19 `-rerun-NN-*` pool dirs each hold 17-38 seat
logs for ONE game. `harvest_translogs` is `comm -13 $BEFORE_LIST <(ls $LOGDIR/*.jsonl)` —
"anything that did not exist when I started" — and all 19 pools share one `$LOGDIR`, so each
swept up the others' in-flight, gameend-less logs. Driven directly against
`git show HEAD:…/selfplay-harness.sh`'s own function with one manifest game (125v123) and one
concurrent pool's log (deck162):

```
BASE harvest produced:
  1789074083-ai_baka_deck125-0xaa-vs-ai_baka_deck123.jsonl
  1789074098-ai_baka_deck162-0xbb-vs-ai_baka_deck125.jsonl   <- somebody else's game
```

**Fix.** `harvest_belongs` applies `tools/corpus-early-check.py`'s own manifest predicate
(#W75-CI P24), shared rather than re-derived: the outdir's live `game-<d0>v<d1>-<gstart>.stderr`
files ARE the manifest of which games this run started; a seat log belongs when its FILENAME
epoch (written once, never rewritten — mtime is destroyed by the harvest itself) is within
15 s of one of those `gstart`s AND its deck is one of that game's two decks. With no manifest
at all it degrades to the set difference, i.e. the old behaviour.

**GREEN.** `--selftest` harvest block grows 4 → **8 checks, 0 failed**: the four wave-72
sidecar checks, plus this run's own game log IS harvested under the manifest, a concurrent
pool's in-flight log is NOT, and the two FALLBACK checks (manifest removed → both harvested,
so a probe outdir cannot go empty). `bash -n` OK.

**Prediction.** Next multi-pool run: each one-game pool's harvest count equals exactly 2×
the games it started (2 for a one-game pool), and no `-final` dir has to be rebuilt by hand.
NEGATIVE clause and its pin: the manifest must not starve a harvest — the two fallback
checks are the guard; a pool reporting 0 harvested logs means the manifest matched nothing
and the fallback did not fire.

---

## Weakest evidence

1. **The engine's own hold-check referent is STILL not observable in the wave-76 corpus, and
   my adjudication disagrees with the wave-76 seat's.** Replaying the corpus with
   `adjudicate.py` (previous staging window at the seam, python `holdActionKeyRow`) yields
   **96 false SEEN claims + 29 wrong counts**, not 22. The two instruments differ in WHICH
   window they call the referent — the seat compared against the previous RECORD carrying a
   bracket, I compared against the previous window that would have STAGED — and a worked case
   (`123v125` deck123 seq 293) shows the engine's real referent was seq **282**, eleven
   records earlier, with four intervening bracket-carrying cast windows that apparently never
   committed. I could not establish why without a live game, and a lane may not run one this
   wave. `hold_check_ref_seq` is shipped precisely so the next seat does not have to guess;
   until a corpus carries it, the true false-bracket population is UNKNOWN and 22 should be
   read as a lower bound, not a measurement. The likeliest suspect, named for the next lane:
   `w76HoldWindowAsked`'s `pendingSeq[s] != windowSeq` guard silently refuses the commit
   whenever ANY other record is written between a window's last build and its own record,
   which leaves a stale referent and matches the shape exactly. I did not change it — a
   speculative loosening of a commit guard with no live evidence is how #W63-AD's defect was
   introduced.
2. **R2 (c) is a REFUTATION resting on my python port of `holdActionKeyRow`.** The port
   reproduces the engine's printed counts on both cited cases (3 and 1) before it is used to
   interpret them, which is the strongest available check, but it is still a second
   implementation of a function whose C++ original is the ground truth. If the port is wrong
   in a way that happens to reproduce two numbers, the refutation is wrong with it.
3. **R1's live path is unobserved.** The hold-path fixture proves `holdStillStands` over five
   synthetic links; whether the seat's crack-back and stack-death verdict WORDS stay
   categorical through a real 32-link drain is argued from `crackBackVerdictKey` /
   `stackDeathVerdictKey` returning three fixed strings, not measured. And whether the model
   TAKES the hold row once told is a model behaviour no fixture can predict — the corpus's
   `own_loop_windows_asked` is the only real gate.
4. **R1's clause deviates from the brief's wording** (no `(row N)`), for the reason given
   above. If the owner wants the number, the priority seam's row emission has to be
   restructured so the hold row's index exists before the acting rows are printed; that is a
   real change to a hot builder and is not this lane's.
5. **R8's 104-of-109 is a diff of the PROMPT's situation block, not of `boardStateKey`
   itself.** They should be the same bytes (the prefill is `serializeGameState()`), but the
   prompt block I diffed is what the model read, not what the key held; a divergence between
   the two would change the 5/104 split without changing the conclusion that the movers are
   real board changes.
6. **R11 (a)'s bound may cost nothing, or may defer a rescue this seat needed.** The five
   wave-76 re-arms are inferred from the counters reading 0, not from a stderr line naming
   the arm; if some of those five were the OTHER arm after all, `force_close_same_arm_deferred`
   will read 0 next corpus and the waste will be unchanged — which is itself the finding.
7. **What the translog cannot show, for any item in this lane**: whether a window was BUILT
   at all. Every suppressed window (hold latch, reserve decline, chain auto-pass, both replay
   caches) leaves no record by design, so the denominator of every hold-check claim, the true
   `own_loop_windows_asked` population before this wave, and the number of notes the leak
   discarded are all invisible. Three of this lane's five items are instruments added
   precisely because of that blind spot; none of them can be validated until a corpus runs.
