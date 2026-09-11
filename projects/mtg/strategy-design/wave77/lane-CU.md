# Wave 77 — Lane CU: the Astra adversarial review, worked

Base = master `3752b3a68` (seeded from `90643013b`; gate 1: suite 1291/0 + 76 AI/0, PARSETEST 6314/0).
Final on this branch: **suite 1291 / 0 failed + 76 AI / 0 failed (THREADS=1), PARSETEST 6402 / 0 failed**
(+88 checks), `check-reply-instructions` OK (1 source, 37 guides), `bash -n` OK,
`selfplay-harness.sh --selftest` harvest 14/0 + regime 30/0 + pilot-stall 17/0 + window-loop 12/0,
`git diff | grep -c U+FFFD` = 0.

Ten findings: **ten CONFIRM, zero REFUTE.** Every one of Astra's source-traced claims held against the
code. Each fix is RED-on-the-base-behaviour first (the counterfactual is named per finding: the
function the live caller calls is reverted to its base body, rebuilt, and the pin fails), then GREEN.

---

## F1 (HIGH, R6/R11-row/Q5) — the ask key and the async slot drifted with board-derived numbers. **CONFIRM**

**Repro.** `AIPlayerGPT.cpp` ask-key sites (priority and `askModel`) both built
`keyTailStr = stripDeclineReaskTags(tailStr)`, which removes exactly ONE clause (the decline re-ask
tag) and leaves every other annotation in. The wave-77 CS pin asserted that fact as correct
(`stripDeclineReaskTags(joinNumberedRows(rowsA)) == joinNumberedRows(rowsA)`) and defended it by
precedent — the wave-76 CO Q5 pin, which says outright "a `{...}` price group's board numbers ride
them, and have since wave 54 … That is the key's intended semantics". CT's R11-row pin claimed
"outside the ask/hold keys" while testing only `optionSetKeyOf` and `holdActionKeyRow`.

**Why the wave-76 defence does not hold** (the reason the fix, not the pin, had to move): these
clauses are FORECASTS over the board — a crack-back arithmetic, a victim count, a life projection —
and a forecast can move while `serializeGameState` does not (a replacement effect expiring, a roster
fold, an ordering of equal bodies). When it moves the ask key moves, and an already-answered window
is asked again: the wave-74 CG freeze (384 calls on one window, 11 games lost over 8 h) on a new
clause.

**RED evidence (executed).** `w77KeyTailOf` wired into both live ask-key sites with the base body
(`return stripDeclineReaskTags(tail);`), then PARSETEST: **5 failed / 6317 passed**, all four pin
families — the R6 cover clause, the older sibling `crackBackKillRowTag`, the CQ roster+cover pair,
and CT's 63-vs-62 roster.

**Fix.** `w77KeyTailOf(tail) = w76StripBalancedAnnotationGroups(stripDeclineReaskTags(tail))` — the
SAME balanced-group rule the hold latch and the option-set key already take, applied to the ask key
and (through `assemblePrompt`) the async slot key. Cost pips survive (`isManaSymbolBody`), so the
ACTION and its cost are still the question; the board half of the key is what carries the board.

**Pins re-derived from the ruling, never adjusted to pass** — four sites, each now asserting
`w77KeyTailOf(A) == w77KeyTailOf(B)` AND `asyncSlotKeyOf(...) == asyncSlotKeyOf(...)` across changing
numbers, plus that the action + cost pips are still IN the key: `#W77-CU F1 KEY` (R6 cover),
`(older sibling crackBackKillRowTag)` — the precedent the wave-76 pin cited, fixed with it —
`(CQ roster + cover)`, `(CT roster)`. The wave-76 CQ pin's weaker form (the async key "tracks the
tail") was replaced outright.

**GREEN.** PARSETEST 6322/0 at this point. **Prediction:** next corpus, no seat shows a
`declined this exact list N times` clause with N rising on a board whose `serializeGameState` is
unchanged, and no (turn, phase, seat) shows >10 asks. Falsifier: a window loop whose two prompts
differ only inside `{...}`.

---

## F5 (HIGH, R2) — a timed-out asked window lost its history commit. **CONFIRM**

**Repro.** `writeTransLog` opened with `const int recordWindowSeq = mWindowSeq++;` for EVERY record.
`pollCompletionRetry` calls `flushWallMissRecord("wall_miss_no_retry")` from inside the poll, while
the window that staged sequence N is still in flight; that record consumed N, `askModel`'s own ask
record took N+1, and `w76HoldWindowAsked`'s guard (`pendingSeq[s] != windowSeq`) rejected the commit.
The next window at that seam then reported itself as the first ever asked. The same line also
consumed `mHoldCheckRefSeq`, so the wall-miss record carried the stamp intended for the ask.

**RED evidence (executed).** `w77RecordIsWindowRecord` reverted to `return true;` (the base rule that
every record takes a number), rebuilt: PARSETEST **3 failed / 6328 passed** —
`the asked window's record carries the sequence it staged under`,
`the next window at this seam does NOT say it is the first window asked`, and the two-wall-miss case.

**Fix.** A `wall_miss` is TELEMETRY ABOUT A WINDOW STILL IN FLIGHT, not a window of its own:
`w77ConsumeWindowSeq(mWindowSeq, kind)` returns the counter without advancing it for `wall_miss`, and
`mHoldCheckRefSeq` is likewise consumed only by a window record. Every other kind is unchanged, so a
window the heuristic answered (`defer`) still orphans its stage and never becomes the referent —
the wave-76 CN rule is untouched, pinned as a MUST-NOT-MATCH.

**Pin, through the live path.** The pin drives the LIVE functions in the LIVE order: stage with
`w76HoldReopenNote`, consume a `wall_miss` with `w77ConsumeWindowSeq`, consume the `ask` with the
same function, commit with `w76HoldWindowAsked` exactly as `writeTransLog` routes it, then read the
NEXT window's bracket. Plus the negative that the `pendingSeq` guard still rejects a foreign record.

**GREEN.** 6348/0 after F4 landed alongside. **Prediction:** next corpus, no seat log contains a
`wall_miss` record immediately followed by a window whose bracket claims to be the first at its seam;
`hold_check_ref_seq` appears on the ask record, not on the wall-miss one.

---

## F4 (HIGH, R1) — "your loop is resolving on its own" required no loop event. **CONFIRM**

**Repro.** `w77OwnLoopResolving()` = `stackLayer()->count(0, NOT_RESOLVED) > 0 && lifeLoopProvenWin(this)`.
Astra's board — Sanguine Bond + Exquisite Blood assembled, NO gain/loss trigger pending, the opponent
casting removal at Bond — satisfies both halves, so the seat's counterspell row was told
"your loop is resolving on its own - this row is not needed to win it". Holding there loses the loop.
The wave-77 CR five-link fixture passed `true` into the feed, so it could not see this.

**Fix, both halves off the live stack.** `w77OwnLoopStackState(theirSpell, component)` walks the
stack once and answers through one pure decision function, `w77LoopVerdictFrom(provenWin,
ownLoopEventPending, theirStackTargetsComponent)`:
* **(a)** RESOLVING needs an unresolved stack object whose SOURCE is one of this seat's own loop
  components (`w77IsLoopComponentCard`: on my battlefield, `lifeToDamageConverterScript` or
  `lifeLossMirrorScript`) — the loop actually executing, not a board that merely could;
* **(b)** THREATENED, which WINS over (a): an unresolved stack object of the opponent's that TARGETS
  a component (`w77StackObjectTargets` asks a `Spell`'s target list and a `StackAbility`'s
  `ability->target`, so removal reaches the test in either shape). The seat is then told the
  opposite, by name: `[their <spell> on the stack targets your <component>, a piece of your own life
  LOOP - the loop STOPS if it resolves, so a hold here does not cover this window]`.

**The hold had to be made honest too** (found while fixing, not in the review): both clauses are
`[...]` groups, which `holdActionKeyRow` strips — so a hold taken while the loop was "resolving"
would have kept standing after the removal appeared and the seat would never have been asked the one
window that matters. The verdict is therefore also a MARKER ROW, `[own loop verdict: …]`, added to
`kHoldVerdictMarkerHead` and pushed into both `takeHold`'s set and the hold check's
`rowsWithVerdict` — the #W68-BB (J9) / #W74-CH shape, recomputed off the live board at every check.
Three verdicts, three keys: any transition re-opens the window. Nothing is removed, collapsed or
auto-answered (pinned: two rows in, two answerable rows out, the hold row untouched).

**The CR five-link fixture no longer passes `true`.** Every link derives its flag from
`w77LoopVerdictFrom` over that link's board facts, and the verdict marker is built per link the way
`takeHold` builds it. Link six is Astra's board: the verdict flips to THREATENED, the false clause is
not printed, the opposite clause is, and `holdStillStands` **re-opens** — pinned both ways.

**RED evidence.** The verdict-table pins state the base behaviour directly and fail against it:
`w77LoopVerdictFrom(true, false, false)` is IDLE where the base predicate said resolving, and
`(true, false, true)` is THREATENED. The hold-reopen pin is RED on base by construction (no verdict
row existed, and both clauses strip to nothing). **GREEN** 6349/0.

**Prediction.** Next corpus: `own_loop_windows_asked` is non-zero only on seats whose own loop has a
link on the stack, and no seat holds through a window in which an opponent's stack object targets a
loop component. Falsifier: a translog showing the guidance clause on a window whose narration has no
loop trigger on the stack.

---

## F3 (HIGH, R6) — both new removal-cover paths resurrected wave-76's regeneration bug. **CONFIRM**

**Repro.** The wave-77 targeted-removal cover walk and the per-X cover walk each called
`crackBackBodyContribution` with no survival test, while the PARENT X path (`xSpellPricing`) had
called `w76CrackVictimTrulyRemoved(v.inst, 1)` since wave 76 CQ F5. A regenerating, ghostformed,
indestructible or persist/undying attacker was credited as removed, and the clause could promise
survival off a body that untaps and swings next turn.

**Fix.** One guarded figure, `w77CoverBodyContribution(c, destroyKind)` =
`w76CrackVictimTrulyRemoved ? crackBackBodyContribution : 0`, called by BOTH new walks — the two
answers bound together so a third path cannot make the same mistake by omission. Nothing is removed
from any roster: the creature is still named by the kill summary beside the clause; only the
survival promise stops being made on its behalf.

**RED evidence (executed).** `w77CoverBodyContribution` reverted to the unguarded body: PARSETEST
red on `the guard subtracts only the body that is gone` (part of the 7-failure F2/F3 counterfactual
run, 6353/7). **GREEN** 6360/0.

**Prediction.** Next corpus: no `{crack-back cover…}` clause claims removed power for a body the same
prompt's `{removes:}` / battlefield line marks as regenerating, ghostformed, indestructible or
persist/undying.

---

## F2 (HIGH, R6/R10) — ANNOUNCE_X treated single-target damage as a sweeper. **CONFIRM**

**Repro.** The per-X cover walk summed every killable victim with no `cbxSv.sweep` check; the parent
row's walk gates on `sv.sweep` (`xSpellPricing`). Astra's board — single-target `damage:X`, X=3, two
opposing 3/3 crack-back attackers, seat at 4 life, total 6 — produced "REMOVES 2 of those 2
attackers … takes 6 off the total, leaving 0" beside a kill gloss saying "one target only".

**Fix.** `w77CoverClauseFor(sweep, …)` answers WHICH clause a removal row's cover takes, for both
walks: a sweep keeps `crackBackKillRowTag` (summed, unchanged); a TARGETED row takes
`w77TargetedRemovalCoverTag`, which prices each victim separately, says "This row removes ONE body
and the target is YOUR choice", and does its survival arithmetic against the BEST SINGLE victim.
Nothing is capped — the classification, not the number, changed.

**RED evidence (executed).** `w77CoverClauseFor` reverted to "always `crackBackKillRowTag`":
PARSETEST **7 failed / 6353 passed**, including `the six-power claim is gone` and
`the honest figure is the BEST SINGLE victim`. **GREEN** 6360/0, with the byte-for-byte pin that the
per-X targeted clause IS the cast row's clause — one family, one wording.

**Prediction.** Next corpus: no X sub-menu row of a single-target spell claims more removed power
than its own best single victim; the sweep rows are unchanged.

---

## F7 (HIGH, R4) — a gain the board forbids was counted, twice. **CONFIRM**

**Repro (a).** The blocking projection accumulated `blockTriggerGain` (certain), `blockMayGain`
(the new optional half) and `blockLifelinkGain` with no life-gain prohibition test. Astra's board:
seat at 2 life with Perimeter Captain, opponent with Erebos; the optional half meters +2, the header
folds it in and WITHDRAWS "no block saves you". `Player::gainOrLoseLife` returns before the event.
**(b).** The edict row's wave-76 F6 gate suppressed the CONVERTER clause under a prohibition and left
`they gain 6 life (its toughness)` printed.

**Fix.** One predicate — `w76PlayerCanGainLife`, the one the suite's `assertcangainlife` asks — gates
every metered gain on the screen: `w77GateMeteredGains(canGain, sure, may, lifelink)` zeroes all
three before anything reads them, and `forcedSacrificeRowTag` takes a `gainReaches` flag. The edict
row is not silenced (the trust doctrine's no-silent-omission rule): it states the event AND the
prohibition — "they would gain 6 life (its toughness), but life gain is PREVENTED on that side right
now, so NO life is gained and nothing converts it".

**RED evidence (executed).** Gate made a no-op and the prohibition branch disabled: PARSETEST
**7 failed / 6367 passed**, including the base surface pin (`the +2 is folded in and the death
verdict is WITHDRAWN`) and both edict pins. **GREEN** 6374/0, with the MUST-NOT-MATCH that an
allowed gain is untouched — the fix gates, it never caps — and a key-stability pin on the edict row.

**Prediction.** Next corpus: on any board with `cantchangelife` / `nolifegain` / `nolifegainopponent`
in force, no INCOMING THIS COMBAT line meters a gain and no edict row states one.

---

## F6 (MED, R2c) — target-preview drift counted as an action change. **CONFIRM**

**Repro.** The live roster appends `targetPreviewFacts` = cost + type + CURRENT P/T
(`stackFactsCore` → `" {1}{r} (creature 1/1)"`). `stripRenderAnnotationsLc` drops a parenthetical
only when it begins with "land", so `(creature 1/1)` → `(creature 2/2)` made an unchanged action new,
the hold-check bracket measured 1 new row, and the hold re-opened over a board the seat cannot act on.

**Fix.** `w77StripTargetPreviewNumbers`, applied inside `holdActionKeyRow` ONLY (the option-set key,
the ask key and the echo matcher are untouched, the way #W76-CN Q1 scoped its own change): a
parenthetical whose body ends in `<digits>/<digits>` keeps its type words and loses the numbers, so
`(creature 1/1)` and `(creature 2/2)` both key as `(creature)`. Under the owner's ruling the identity
is the target INSTANCE — its name and handle, both of which survive.

**RED evidence (executed).** The call removed from `holdActionKeyRow`: PARSETEST **4 failed / 6398
passed** — the key equality, the handle+type survival, the hold-check bracket measuring zero, and
the latch standing. **GREEN** 6402/0. MUST-NOT-MATCH: a different target instance, a different type
word (artifact creature), a different cost, and any non-preview parenthetical all still key apart;
the wave-56 land rule is unchanged.

**R2c re-examined, as the brief asked.** R2c's refutation cases reorder the target NAMES. Names are
not preview facts — a roster whose names move is a real change and still re-opens (pinned). So this
fix does not convert R2c to CONFIRMED: **R2c's refutation stands**, and what Astra found is a
SEPARATE defect (preview drift) that the refutation's cases could not have exercised. That is the
honest reading of "say which".

---

## F8 (MED, R6) — the targeted cover annotation was an unbounded roster. **CONFIRM**

**Repro.** `w77TargetedRemovalCoverTag` printed one `name takes N off that total, leaving M` sentence
per victim. Measured on the base body: 100 same-named one-toughness attackers = **6,212 B** in one
clause, growing linearly with the board, bypassing the wave-76 Q2 roster fold the kill summary beside
it goes through.

**Fix.** Victims group by WHAT THEY TAKE (the only number the clause is about), each group's names go
through `joinVictimRoster` — the same count + shapes + handle-range + `and N more` fold — the total
is stated once above, groups print largest contribution first (the figure the decision turns on), and
past a 4-group cap the remainder is COUNTED and named, never trimmed in silence. The BEST figure and
the survival arithmetic are taken over EVERY victim, listed or not: the cap is a display bound, never
a change to the arithmetic (pinned).

**RED/GREEN (executed).** 6,212 B → **404 B** on the same board; RED run 6379/4. Pinned under a 500 B
bound, with the count leading, the group order, the counted residue, and the unchanged decision
figure. **GREEN** 6383/0.

---

## F9 (MED, R5) — CT's 37,121 B bound omitted bytes its own snippet builder emits. **CONFIRM**

**Repro.** `kBoardEffectClauseFactor * maxLen` was the head loop's BUDGET, not the function's output:
the protected last clause, the `" -- "` separators, the omitted-clause marker and `textSnippetCore`'s
visible ellipsis were all added on top. Executed counterexample at the 55-byte tier: **148 B returned
against an asserted 110 B** ceiling (Astra computed 163 B for her own three clauses; same class).
CT's per-name 266 B figure was therefore an assumption, not a bound.

**Fix.** The ceiling is now stated and ENFORCED:
`boardEffectSnippetCeiling(maxLen) = kBoardEffectClauseFactor * maxLen + kBoardEffectSnippetSlack`
(slack 64 B = the marker's worst case + two separators + the ellipsis). The head loop charges itself
the separator and the slack, and a give-back loop hands head clauses back one at a time — each
give-back RAISING the counted omission, never trimming in silence — until the render fits; the
protected last clause survives (pinned, and #W47-R6's "every earlier loyalty ability" pin is still
green at the 140 tier).

**Bound re-derived from the capped value**, as the brief required: CT's pin now uses
`boardEffectSnippetCeiling(len)`, so per-name goes 266 B → **330 B** and the whole-prompt worst case
6,746 B → **8,282 B**; on the corpus's largest prompt that is **30,375 + 8,282 = 38,657 B < 40,000 B**.
The bound still holds, and it is now a bound rather than an assumption.

**RED evidence (executed).** Slack set to 0 and the give-back loop disabled: **3 failed / 6386
passed**, the 55-tier case reporting 148 B against a 110 B ceiling.

---

## F10 (MED, R11) — harvest membership was not game identity. **CONFIRM**

**Repro (Astra executed it; re-executed here as selftest cases).** `harvest_belongs` matched
`deck in decks && abs(ep - gstart) <= 15`. With manifest `125v123` at epoch 1000 it **accepted** a
foreign `125v123` seat at 1005 and a foreign `125v162` seat at 1005, and **rejected** the legitimate
`125v123` seat initialised at 1016. Two concurrent games of the SAME pair are inseparable at any
tolerance — the shape that forced the wave-76 `-final` corpus to be rebuilt by hand.

**Fix — the game names its own logs.** The seat-log basename carries the player's own pointer and is
known only inside the engine, so `AIPlayerGPT` prints `WAGIC_GPT_TRANSLOG_FILE <basename>` on stderr
once per seat when the translog (an opt-in telemetry feature) is on — the harness already captures
that stream per game. `run_one_game` appends both names to `$OUTDIR/.seatlogs` as it reaps, and
`harvest_belongs` is exact-name membership when that file exists. The deck+tolerance predicate
remains ONLY as the fallback for an outdir with no `.seatlogs` (an old corpus, a probe outdir, a
binary that does not announce) and it now WARNS on stderr when it is used, so a silent degrade cannot
be mistaken for identity.

**RED/GREEN (executed).** With the exact-name branch removed, `--selftest` fails all four new cases
(Astra's three shapes plus the concurrent-same-pair case). With it, **harvest-selftest: 14 checks,
0 failed**, and the fallback case still harvests. `bash -n` OK; the other three harness selftests
(regime 30, pilot-stall 17, window-loop 12) are unchanged and green.

---

## Weakest evidence

1. **No fixture constructs a live stack or battlefield for the GPT render path.** F4's stack walk
   (`w77StackObjectTargets`, `w77IsLoopComponentCard`, the source-is-a-component test), F3's two call
   sites, F7's blocking-gain call site and F5's `writeTransLog` wiring are verified by SOURCE TRACE
   plus a pin on the exact function the live caller calls; the walk over engine objects itself is not
   executed by any test. This is the structural limit the review names and I did not beat it: PARSETEST
   has no game, and the suite cannot see a GPT render string (which is why `assertcangainlife` and
   `assertcrackremoved` exist at all). Where I could shrink the untested span I did — every fix routes
   through ONE named function that the pin drives — but the gap between "the function is right" and
   "the walk calls it on the right cards" is closed only by the next corpus.
2. **F4's definition of a "pending loop trigger" is mine, not the owner's.** I read the brief's
   "a life-gain/loss trigger of the loop's own cards on the stack" as "an unresolved stack object whose
   source is a loop component". A loop link whose stack object is sourced from a THIRD card (a copy
   effect, a granted trigger) would read as IDLE and simply print no guidance — fail-closed, which is
   the safe direction, but it is a narrowing of R1's population and the corpus will show it as
   `own_loop_windows_asked` falling.
3. **F4's threat clause changes the ask key** (it is a `[...]` group, which the ask key does not
   strip). I pinned CONSTANCY across polling ticks rather than equality across boards, and argued that
   a new spell of theirs on a loop piece SHOULD be a new question. That is a judgement, not a ruling;
   if the ruling is that `[...]` groups must also leave the ask key, F1's stripper is the place and
   this pin is the one to re-derive.
4. **F9's slack constant (64 B) is dimensioned, not measured over every possible marker.** It covers
   a 24-clause marker, two separators and the ellipsis; a future clause-count past 99 or a longer
   marker wording would need it re-checked. The give-back loop's final `textSnippetCore` guard means
   an overrun truncates visibly rather than escaping the bound, so the bound cannot silently break —
   but the pin proves the ceiling over three tiers and two texts, not over the corpus.
5. **F10's new engine line is a release-build stderr write** (guarded only by the translog being on,
   not by `_DEBUG`/`WAGIC_DEVLOGS`). I judged it as telemetry-config feedback on an opt-in feature
   rather than a diagnostic, but it is the one thing in this lane that ships a printf to a user path.
6. **F8's 4-group cap and F9's give-back both trade information for a bound.** Each states what it
   omitted, so neither is a silent surface — but neither was measured against a real corpus prompt,
   only against synthetic worst cases.
7. **No corpus was run** (the brief forbids it), so every "next corpus" prediction above is
   unfalsified. The three suite flakes did not appear in this run at all (1291/0 clean).
