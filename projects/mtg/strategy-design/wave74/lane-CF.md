# Wave-74 lane CF — fix lane for the Astra adversarial review

Base: master `49a132f03` (= step-one merge `92843101e` + the review), gate 1 baselines
1285/0 suite, 76 AI/0, PARSETEST 5708/0.

Gate on this lane's tree: **suite 1285 tests (0 failed), 76 AI tests (0 failed)**,
single-threaded, both failure strings counted (`==Test Failed !==` 0, `==Test timed out` 0);
**PARSETEST 5748 passed, 0 failed** (+40); `selfplay-harness.sh --selftest`
harvest 4/0, **pilot-stall 13/0** (was 7), regime-gate 30/0; `bash -n` clean;
`check-reply-instructions.py` OK (1 source, 37 guides); U+FFFD in the diff: 0.

## RED evidence — the counterfactual build

Every C++ finding is pinned by a PARSETEST case that FAILS when the fixed function
body is replaced by the base's own semantics. One build carried all six reversions
(`gptRetrySelectArm` a no-op; `lifeToDamageConverterRate` returning the old boolean;
`keepX = maxX - keepNeed`; `w74ClockSourceRecurs` always true; `w74ScriptGrantsHasteToMine`
always false; `planStateThousandsComma` always false) with the new pins in place:

```
=== self-test: 5732 passed, 16 failed ===
FAIL #W74-CF F1 (5 checks)  FAIL #W74-CF F5 (1)  FAIL #W74-CF F6 (4)
FAIL #W74-CF F7 (2)         FAIL #W74-CF F8 (1)  FAIL #W74-CF F9 (3)
```

The harness findings were replayed against the BASE predicate lifted out of
`git show HEAD:` into a standalone script, over the exact fixtures the new selftests
build. It reproduced Astra's four numbers verbatim:

```
case h (slow healthy run, want OK):        STALL 6
case i (stalled tail, want STALL 6):       OK 3
case j (stale success vetoes, want STALL): OK 6
case k (banner count, want 3):             0
```

After the fix: `OK 6` / `STALL 6` / `STALL 6` / `3`.

Card facts used below were read off the repository primitives, not from model memory:
Cliffhaven Vampire `auto=@lifeof(player) from(*[-lifefaker]|*):life:-1 opponent`
(mtg.txt:20374, text "each opponent loses 1 life"); Sanguine Bond `:life:-thatmuch opponent`
(99005); Fervor `auto=lord(creature|myBattlefield) haste` (40019); Drain Life
`mana={1}{B}{X:black}` (31444); Staff of Nin `mana={6}`, `{T}:damage:1 target(anytarget)`
(112427); Sorin, Lord of Innistrad's +1 makes a `token(Vampire,...,lifelink)` with no
haste anywhere in its own text (planeswalkers.txt:3057).

---

## F1 — HIGH, O2: one arm overwrites another arm's armed retry — **CONFIRMED**

**RED.** Source-traced and pinned. `retryArmMatches` (AIPlayerGPT.cpp, wave-74 lane CD)
guards only the CONSUME and ABANDON branches. All four arming branches below it write
`mRetryBase` / `mRetryActivePrompt` / `mForceClosePrefill` unconditionally into the single
shared slot. Astra's interleaving therefore stands: land arms a forced close; casting's
poll skips the guarded block (ownership mismatch), runs its own primary, and its forced-close
arming overwrites land's record while land's second leg is outstanding. Land then polls its
ORDINARY prompt, whose async slot key differs from a forced-close request, so the completed
leg is discarded. Neither the ownership boolean, the delivery-ratio experiment nor the
two-second pass hold prevents it. Replayed as five PARSETEST checks, all RED with the select
disabled.

**Fix.** Per-arm retry STORAGE, not a flag. `struct GptRetrySlot` (AIPlayerGPT.h) holds every
field the arming branches write — activePrompt, base, doneBase, ceilingDoneBase, prefill,
firstLatencyMs, budgetMs, armLand, phase1Length. `mRetryPark` holds the other arm's slot;
`gptRetrySelectArm(live, park, landArm)` (pure, PARSETEST-pinned) swaps at the top of every
`pollCompletionRetry`, so after it returns the live slot is this arm's or empty and the other
arm's leg is parked untouched. `decisionArmed()` now reads live-or-park, so the pass hold still
sees an armed leg that is currently parked.

**Prediction (falsifiable).** In the next corpus, forced closes ARMED and phase-2 requests SENT
converge: the wave-73 stub-endpoint ratio was 409 armed / 38 sent and the live corpus 33 / 2.
If `phase2_answer_recovered + phase2_answer_missing` stays far below the arming count on a
run where both arms are active, this fix did not take.

## F2 — MED, O2: the decode-garbage retry set no ownership — **CONFIRMED**

**RED.** The fourth arming branch (`isDecodeGarbage(content)`) sets `mRetryActivePrompt`,
`mRetryBase` and `mRetryFirstLatencyMs` and never sets `mRetryArmLand`; the constructor
leaves it false, so a land-drop decode-garbage retry on a fresh seat is owned by "casting"
and the land arm's next poll fails `retryArmMatches` and never launches it. Confirmed by
reading the branch — there is no executable pin for it (see Weakest evidence).

**Fix.** Ownership is stamped ONCE, in `selectRetryArm()`, from the arm whose window is
running — not by each arming branch. The three per-branch `mRetryArmLand = asyncLandArm(...)`
assignments are removed with it, so the omission class cannot recur: a branch that forgets
to stamp ownership no longer exists to forget.

**Prediction.** A decode-garbage retry armed from a land-drop window launches on the land
arm's next poll (translog: a `retry=1` record on an `ask`/`priority` land-drop seam).

## F3 — HIGH, O26: the stall predicate kills healthy runs and misses stalled tails — **CONFIRMED**

**RED.** All three of Astra's counterexamples reproduce on the base predicate, verbatim
(numbers above). "Last three records per unfinished log" is neither the newest K decisions
nor evidence that every active seat has stopped being answered.

**Fix.** The pool is the newest K decision records ACROSS seats — live logs ordered by mtime,
newest first, newest record first within a log — and STALL additionally requires that EVERY
active seat has produced at least one decision record. That clause is the whole difference
between case (h), a young run that must live (fifteen seats still on `gamestart` carry no
evidence either way), and case (j), where a stale record must not veto six newer failures
(it is simply not in the newest-K window). Case (i) is caught because one seat's twenty
timeouts now fill K on their own. Three new selftests, one per case.

Mtime is the ordering key because translog records carry no timestamp field (checked against
the wave-73 corpus: `seq`, `turn`, `phase`, `latency_ms`, no clock). During a live sweep a
log's mtime IS the time of its last record; the harvest that rewrites every mtime happens
after the run, and this predicate never reads a harvested directory — noted in the code.

**Prediction.** A run whose seats are all answering never trips the banner, and a run where
one live seat goes silent for K consecutive decisions trips it within K decisions of the wedge.

## F4 — MED, O26: the banner certifies fallback-contaminated games — **CONFIRMED**

**RED.** `timeout_fallbacks_in` counted `fallback == "timeout"` exactly; a log of
`wall_miss_no_retry` / `empty_reply` / `http_error` returned 0 and the banner printed
"none — every decision in every seat log was answered by the model". Measured: 0 on a
three-record fixture of exactly those classes.

**Fix.** ONE exported class list, `PILOT_SILENT_CLASSES`, backs the tripwire, the per-game
helper (renamed `silent_fallbacks_in`) and the results banner. The banner names the classes
it counted and breaks the total down by class, so the two surfaces can be checked against
each other by reading them. `unparsed_reply` stays out of the list in both: that is the model
answering badly, a play problem, not a wedge. Selftest asserts the agreement in both
directions (the banner counts 3 on the log the tripwire calls STALL 3; both count 0 on
`unparsed_reply`).

## F5 — HIGH, O5: fixed-one-life triggers priced as proportional converters — **CONFIRMED**

**RED.** `lifeToDamageConverterScript` is a predicate; it accepts Cliffhaven Vampire's
`:life:-1 opponent` exactly as it accepts Sanguine Bond's `:life:-thatmuch opponent`, and the
O5 caller used it as proof that every lifelink point converts. On Astra's board (one 4-power
lifelink attacker, opponent at 8) the base computes `selfConvLifelink = 4`, rendering the
ATTACK TOTAL as 0 / lethal; the true unblocked result is 3. Two Sanguine Bonds collapsed to
one boolean and undercounted.

**Fix.** `lifeToDamageConverterRate(script, proportional)` returns the RATE: `proportional`
for the `thatmuch` form, otherwise the fixed number lost per gain EVENT. The caller sums the
battlefield — `propConverters * gainPoints + fixedPerEvent * gainEvents`, where a double
striker contributes two events and twice the points. Cliffhaven + a 4-power lifelinker = 1;
two Bonds = 8. Pinned both ways plus the mirror half and a plain lifegain trigger as
must-not-matches.

**Prediction.** An ATTACK TOTAL on a board with a fixed converter no longer claims a kill it
cannot deliver: the kill claim and the post-combat life in the narration agree.

## F6 — MED, O8: a colour-limited X budget minus a generic cost — **CONFIRMED**

**RED.** `maxX - keepNeed` on Astra's board (Drain Life `{1}{B}{X:black}`, four Swamps and
seven colourless one-mana sources, Staff of Nin `{6}` in hand): `maxAnnounceableX` = 3,
`keepNeed` = 6, so the row printed the categorical "no X on this row leaves it payable this
turn, not even X=0" — while X=3 pays `{1}{B}`, three black for X, and leaves six for the
Staff. Reversed on ordinary one-mana sources, not an off-by-one.

**Fix.** One currency: `keepX = min(maxX, totalMana - baseCMC - keepNeed)`, with `totalMana`
the row's own `untappedSources` (the same figure the keep-candidate filter already uses, so
the two clauses on the row cannot name different numbers). The colour ceiling caps X and
nothing else. A `totalMana < 0` default reproduces the previous arithmetic byte for byte,
which is why lane CE's own pin (`maxX 11, base 3, keep 6 -> X=5`) is unchanged.

**Prediction.** X rows that previously refused categorically now name a keep-X, and the
`125v126` seq-308 shape (a plan that casts for max X and strands the card it named) stops
being licensed by the row.

## F7 — MED, O9: a clock promised off an unusable ability — **CONFIRMED**

**RED.** `ownClockTagFor` summed `w74TapOnlyDamagePerActivation` over every battlefield card
with no board gate at all, so a tapped `DOESNOTUNTAP` pinger earned "the opponent reaches 0
in 2 more turns". Pinned as `w74ClockSourceRecurs(cannotUntap=true, ...)`, RED with the gate
disabled.

**Fix.** Two gates. (a) A source counts only if it RECURS: the engine's own cannot-untap pair
(`DOESNOTUNTAP` or `frozen >= 1`) excludes it, and a creature that entered this turn without
haste ticks a turn later than the sentence claims, so it is excluded too. Both directions
understate the clock and neither can invent one. (b) The lethal-incoming gate the O9 lane
skipped: `ownClockTagFor` takes the seat's own life and the incoming combat damage already
latched for this turn (`mIncomingCombatTurn == observer->turn`), and suppresses the whole
sentence when the seat does not survive to spend the rate. Both callers (the pass-priority
row and the decline row) pass them.

**Prediction.** The tag appears only beside a source that will untap, and never on a window
where the seat is facing latched lethal — a clock tag whose game ends before its stated turn
count is a falsification.

## F8 — MED, O15: haste-granted tokens declared unable to attack — **CONFIRMED**

**RED.** The clause read "haste" out of the planeswalker's own truncated card text
(`lowTxt`), so Sorin's +1 under a Fervor said "the body this makes arrives summoning sick -
it cannot attack until your next turn". Fervor's script is `lord(creature|myBattlefield)
haste`; Sorin's own text contains no "haste". Pinned; RED with the battlefield scan disabled.

**Fix.** `w74ScriptGrantsHasteToMine` (pure over one permanent's script) plus a battlefield
walk — the grant is read off the board because the token does not exist yet and its
`basicAbilities` cannot be asked. Scoped: a lord line must name `creature` AND `mybattlefield`,
so an opponent-scoped lord grants this token nothing, and an activated `{2}{R}:haste` is a
payment, not a static. All four shapes pinned.

**Prediction.** A seat with a haste granter prints no summoning-sick clause on its token
rows, and the token attacks the turn it arrives.

## F9 — MED, O7: a thousands separator eaten as a clause delimiter — **CONFIRMED**

**RED.** `M=1,000; cast Staff; then attack.` -> accepted prefix `M=1,` -> after one step the
carry reads `M=1,cast Staff; then attack.` The plan asserts one creature where the model
stated a thousand. Reproduced exactly by the new pins, RED with the fix disabled.

**Fix.** `planStateThousandsComma` accepts `\d{1,3}(,\d{3})*` inside a state clause: a comma
counts as part of the number only when a digit precedes it and exactly three digits follow
that are not themselves followed by a digit. Both the clause-separator scan and the
number-consumption loop consult it. Every other comma still ends the clause, so
`stop=3, M=2; swing` still splits and a plan opening with prose is untouched (both pinned).

**Prediction.** A carried plan's stated number is byte-identical across windows regardless of
formatting; the O7c "your stated stop was set when their life was N" clause can no longer
name a number the model never wrote.

## O19 — the verification gap, not a defect — **coverage added**

Astra found no defensible answer-mapping defect and neither did this lane; the objection was
that the O19 pins are hand-built permutations beside the seam, so a display permutation and a
map-back that disagreed would both have passed. The permutation and its inverse are now the
seam's own code, lifted out unchanged as `w74XPermuteToClimbing` / `w74XPickToContractIndex`,
and one PARSETEST case runs a capX=3 ANNOUNCE_X menu through RENDER (the seam's permutation)
-> ANSWER (the real `parseChoice`, on the reply shape the pilot writes) -> APPLY (the seam's
map-back) for every rung, asserting the applied contract index equals the X the answered row
printed. The descending path's map-back is pinned byte-identical as a must-not-match.

---

## Weakest evidence

1. **F2 has no executable RED.** The decode-garbage arming branch is not reachable from
   PARSETEST (it needs a live poll), so the confirmation is a source read of one branch and
   the fix is structural — ownership moved to a place no branch can skip. If the branch is
   in fact unreachable for land-drop seams for some reason I did not find, the finding is
   real but harmless and the fix is still correct.
2. **F1's RED is a counterfactual, not the base binary.** `gptRetrySelectArm` did not exist
   on base, so the pin is RED against a build where it is a no-op — which IS base semantics
   (one shared slot, no arm selection), but it is a reconstruction, not the original code
   path. The live interleaving itself was not reproduced in a game; the delivery-ratio
   experiment named in lane CD is the instrument that would show it, and it was not re-run.
3. **F3's ordering key is file mtime.** Translog records carry no timestamp, so "newest K by
   record time" is approximated by log mtime plus within-log order. This is exact for a live
   sweep and MEANINGLESS for a harvested directory (the harvest resets every mtime — the
   known wave-4 gotcha). The predicate is only ever called on the live LOGDIR, but nothing
   in the code enforces that; a future caller pointing it at an outdir would get noise.
4. **F3's "every active seat represented" rule is my reading of the brief**, chosen because it
   is the only formulation that makes all three of Astra's cases come out the way he says
   they should. It trades one conservatism for another: a run where a single seat has not yet
   made one decision can never be called stalled, however long the others have been silent.
5. **F5's gain-event count assumes one lifelink trigger per attacker per combat** (two for
   double strike). First strike creating a separate damage step, or a converter that triggers
   on a gain the attack does not cause, is not modelled — the number is a floor for fixed
   converters, which is the safe direction for a kill claim.
6. **F7's lethal-incoming gate uses only the latched combat damage for the current turn.** A
   seat facing lethal from a source that is not declared combat (burn in hand, an activated
   drain) still gets the clock sentence. The gate closes Astra's class, not the general one.
7. **F8's haste scan is script-shaped, not engine-shaped.** It matches `lord(...creature...
   mybattlefield...) haste`; a haste grant expressed some other way (a token-granting ability
   that also confers it, a `transforms(...newability[...haste])` payload) is not seen, so the
   false clause can still print on an exotic board. The engine has no pre-token query to ask.
8. **No corpus was run.** Every prediction above is untested against play; the next corpus's
   seat review is the real gate, per the wave method.
