# Wave-67 lane AZ — the seven findings of the codex adversarial review

Base: `5afffc21b` (master after the AV-AY merge + the review commit + this lane's brief). Every finding
was VERIFIED against the code first, then fixed in its general form. Gate on this tree:
**PARSETEST 5107 passed, 0 failed** (baseline 5071/0, +36 cases); **suite THREADS=1 1274 tests, 0 failed,
67 AI tests, 0 failed, 0 timeouts** — the baselines exactly.

## Verdicts

| # | Sev | Finding | Verdict | Where it was proved |
|---|-----|---------|---------|---------------------|
| 1 | HIGH | the label-missing salvage matches a NEGATED sentence to the row it names | **CONFIRMED** | `salvageLabelMissingChoice` (old `:18407`) tested only `low.find(name)` after any `kActs` verb hit; no polarity test exists anywhere in the function or its callers |
| 2 | HIGH | a stop leaving exactly one repetition executes zero while narrating one | **CONFIRMED** | the receipt was written before the branch and always said `ran <allowed> of them this window`; the branch was `if (allowed >= 2) namedCount = allowed; else { choice = 0; ... }` — `allowed == 1` took the else |
| 3 | HIGH | the reservation-decline latch auto-passes on phase + candidate names + source count alone | **CONFIRMED** | `reserveDeclineStillStands` compared only `castSetKey` and `untappedSources`; `reserveDeclineHonoured` added only turn and phase. Nothing in it can see the stack, either life total, either battlefield, the hand or the library |
| 4 | HIGH | the repeat-pay renderer REMOVES legal rows instead of shortening them | **CONFIRMED** | the `repeatHidden` vector fed the `shownOpts` filter, so rows 3..21 were absent from the printed list; the PARSETEST at `:71452` pinned "18 identical rungs are hidden" as correct |
| 5 | MED | an unresolved `MayAbility` draw is counted as a draw that cannot be declined | **CONFIRMED** | `scanStackAbilityDraws` recursed through `NestedAbility`, and `MayAbility : MTGAbility, NestedAbility` (`AllAbilities.h:2056`) — the may-draw landed in `cards`, which `xLibraryReserveWhy` describes as "which you cannot decline either" |
| 6 | MED | a loop half in a graveyard is treated as returnable | **CONFIRMED** | the zone table's `back` column was a literal `true` for `graveyard`; no board fact was consulted, and `:71763` pinned "a graveyard half is recurrable" |
| 7 | HIGH | an unusable mandatory reveal is answered with the first eligible card | **CONFIRMED** | `firstEligibleRevealIndex` returned the first `true` in the eligibility vector; the driver floor in `AllAbilities.cpp:1341` took the first `canTarget` card in zone order and recorded nothing |

No finding was refuted.

## Mechanisms

**R1 — polarity before names.** New pure `sentenceNegatesBefore(lowSentence, at)`: a word-bounded
negation cue (`not`, `never`, `cannot`, `no longer`, `rather than`, `instead of`, `avoid`,
`avoiding`, `refrain`, `without`, and the two contracted `n't` forms, ASCII and U+2019) standing
BEFORE the position where a row's short name appears disqualifies that match. With no surviving
candidate the salvage returns -1 and the caller re-asks once — never Baka, never the row. Word
bounding is what keeps `Cast nothing right now` from negating itself on the `not` inside `nothing`,
and a cue AFTER the name (`I cast Doom Blade, not Shock`) is not a negation of it. The direction of
the error is deliberate: a false negative costs one re-ask, a false positive casts the spell the
reply said it was keeping.

**R2 — one repetition left is one repetition run.** New pure `repeatStopExecutesNothing(allowed)`
(`allowed == 0`) is the branch's whole predicate, and the receipt is now the pure
`repeatStopClampReceipt(named, statedCurrent, statedStop, allowed)` — one function for the number
narrated and the number executed. `allowed == 1` sets `namedCount = 1`, which the existing
`repeat_count_under_two` path performs as the base row's single activation with its own receipt
(`- ran 1 time (you named 1)`). Nothing is withheld and no ceiling of the engine's invention is
added; only a stop with no room left (`allowed == 0`) passes the window, and then the receipt says
`ran 0 of them this window`.

**R3 — the board is the key.** `reserveDeclineStillStands` takes the held and current BOARD KEYS
first (`serializeGameState()` — the same string the priority ask key and the cast livelock breaker
compare, carrying the stack, both life totals, both battlefields, hand and library counts). The
castable-set key and the untapped-source count stay as the finer tests they were, and turn/phase
still bound the latch. `mReserveDeclineBoard` is cleared wherever the key is. Nothing volatile
enters it: the board key is the render the window already built (it is not the prompt, which carries
the notes and the plan), and it never touches `mPromptTail`, the ask key or the async slot key.

**R4 — the render shortens descriptions, it does not delete rows.** `payRepeatCollapse` still finds
the identical band, but its flag vector now marks rows to SHORTEN. Each band row is truncated back
to the engine's own label (`repeatBase[i]`, captured before the price tag is appended) and given
`payRepeatBandRowTag(bandPaid)` — ` {identical in effect right now: adds 1 counter}`, one short clause — and
the band's largest row carries the arithmetic in a reworded `payRepeatCollapseTag` that says the
rows are identical and that every one of them is still a legal answer. `repeatHidden` /
`repeatCollapsed` are gone from the `shownToFull` path, which is now the Flip-Side filter's alone,
so the printed list and the engine's option vector are index-identical again for these menus. The
152v146 s11 menu goes from ~2.7 kB to roughly 1.2 kB with all 21 rungs answerable.

**R5 — a may-draw is optional.** `scanStackAbilityDraws` carries an `underMay` flag set by
`dynamic_cast<MayAbility *>` and accumulates anything beneath one into a separate `mayCards`;
`stackPendingDrawsFor` gains an optional `mayOut` and still RETURNS the undeclinable count, so every
existing caller keeps exactly what it had. `xLibraryReserve` passes the may count to
`xLibraryReserveWhy`, which names it as `(up to N further draws on the stack you MAY decline are not
reserved for)` and leaves it out of the reserve — an under-count keeps the ceiling generous, the
only direction that cannot cost a legal line.

**R6 — a graveyard returns only if something returns it.** New pure
`graveyardRecursionScript(magicText, targetSpec)` — the script must mention a graveyard and move a
card to a hand or a battlefield (`target(...|myGraveyard) ... moveto(hand)`, `moveTo(myBattlefield)`,
the `may moveto(hand) target(*|mygraveyard)` ETB shape) — and `seatHasGraveyardRecursion(pl)` scans
that player's battlefield and hand for one. The graveyard row's `back` verdict is that scan.
`pendingLoopWarningText` gains a `blockedByExile` flag so the withheld-claim face names the right
reason: exile keeps its wording, a graveyard reads "does not come back without a recursion effect,
and nothing that could return it is on that battlefield or in that hand". The consequence of a close
is still stated, conditionally, and the zone is still named.

**R7 — the heuristic picks, not the vector.** `heuristicRevealIndex(cmc, eligible)` takes the highest
converted cost among the rows the engine's `canTarget` accepted, ties to the earliest row.
`AIPlayerBaka::chooseCard` cannot serve here and would not help if it could: it walks hand, library,
battlefield, graveyard, stack, exile and command zone and never the reveal zone
(`AIPlayerBaka.cpp:1636`), and it is itself first-in-zone-order — routing through it would return the
same positional answer with a heuristic's name on it. The preference the heuristic actually has over
cards it may take is cost-dominant (`FindCardToPlay`'s `"*"` rank is pure max cost), so that is the
preference the degrade uses. Two smaller fixes rode along: the `reveal_fallback_pick` record now
stores the PRINTED row number (it stored the reveal-vector index, which differs whenever the printed
list was collapsed), and the driver floor in `AllAbilities.cpp` calls the new extern
`gptNoteEngineRevealFloor(seat, card)` so a floor pick is counted and traced instead of silent —
`engine_reveal_floor_picks` is a new gameend report field, zero on a healthy game.

## RED evidence

All seven mechanisms were reverted **simultaneously** in one build (`sentenceNegatesBefore` -> false;
`repeatStopExecutesNothing` -> `allowed < 2`; the board test dropped from `reserveDeclineStillStands`;
both repeat-pay tags back to the wave-67 wording with no short clause; `MayAbility` no longer starting
a may-scope; `graveyardRecursionScript` -> true; `heuristicRevealIndex` -> first eligible), signatures
kept, with every new case in place. Result: **5084 passed, 23 failed** (log
`~/.gatelogs/w67-AZ-red.log`). By finding: R1 5, R2 1, R3 3, R4 5, R5 3, R6 3, R7 3. Restoring the
mechanisms returns 5107/0.

Corpus RED for R3, measured directly on the run the latch was written for
(`matchups-20260906-064038`, `162v130`, the `--- CURRENT SITUATION ---` block of each record):

* s16 -> s17 (same turn 11, same Draw step, life 10/16 both): the seat's hand went **5 -> 6 cards**,
  its library **46 -> 45**, and a Howling Mine draw trigger **left the stack**.
* s18 -> s19 (same turn 13, same Draw step, life 1/8 both): hand **7 -> 9**, library **43 -> 41**,
  stack **3 objects -> 1**.

So the brief's expectation ("nothing changed there") does not hold: under the full board key NEITHER
corpus pair latches, and both windows are re-asked. That is the correct reading — the wave-67 latch
held those windows only by not looking at what moved, and its two supporting facts (the same one
castable spell, the same 4 untapped sources) are true of a board that had changed in three ways. The
lane AX loss at 162v130 s18/s19 is therefore a decision-quality event, not a window the engine should
have auto-passed. The latch is kept, correctly keyed, and will now fire only on a genuinely
unchanged board.

## Predictions for the wave-67 corpus

1. `reserve_decline_windows_skipped` drops sharply (to 0 on most seats); no game shows a
   reservation-held window across a turn in which the opponent resolved a spell or either life total
   moved.
2. No `label_missing_salvaged` record whose reply's decisive sentence carries a negation cue before
   the row name; `label_missing_reask` rises by roughly the count of such replies.
3. Every `repeat_clamped_to_own_stop(...,executed=1)` record is followed by an executed activation
   (a `repeat_count_under_two` receipt), never by a pass.
4. Every Intrepid Adversary-class menu prints all its rungs; the ask's byte size for those menus
   falls by roughly half against wave 66, and any answer naming a mid-band rung resolves to that rung.
5. No `xLibraryReserveWhy` string says "cannot decline either" about a stack draw whose ability is a
   `MayAbility`; where one is pending the row instead carries the "MAY decline ... not reserved for"
   clause.
6. No `LOOP HALF PENDING` banner claims "one resolution from closing" for a graveyard half on a board
   with no recursion card in that player's hand or battlefield.
7. Every `reveal_fallback_pick` names the highest-cost eligible revealed card, and its `choice` field
   indexes the printed options list.

## What I did NOT verify

* **No live corpus run.** Every claim above about model behaviour is a prediction, not an observation;
  the gates are PARSETEST and the suite.
* **R4's row-preservation is not pinned by a pure function.** The tags are (5 cases), but "the printed
  list still contains every rung" is structural — it is proved by the removal of `repeatHidden` from
  the `shownToFull` path, not by a test. A fixture would need a live Intrepid Adversary ETB menu with
  a model seat.
* **R7's seam path is not exercised by the suite.** `w67AX_reveal_refusal_takes_legal_card.txt` passes,
  but the trace shows it rides the AllAbilities DRIVER FLOOR, not `AIPlayerGPT::decideReveal`'s
  degrade, so `heuristicRevealIndex` is covered by PARSETEST only. The floor's own pick is unchanged
  (first legal in zone order) — it is the last resort under paths with no seat to ask — and it now
  records that it fired.
* **R7's cost-dominant preference is a claim about the heuristic, not about Magic.** Highest mana value
  is the preference this codebase's heuristic carries (`FindCardToPlay` `"*"`); it is not always the
  best tutor target. It is a fallback for a decision the model refused.
* **R5's row clause was left alone.** `xLibraryRowClause`'s `{... N more draws pending on the stack,
  which you cannot decline}` now counts only undeclinable draws, which is true, but it does not name
  the optional ones; only the reserve's `why` does. Threading the may count to that call site was out
  of scope for this lane.
* **R6's recursion scan reads hand and battlefield only**, and reads scripts, not the rules: a
  graveyard-castable half (flashback, escape, "you may cast it from your graveyard") is not detected
  and will read as needing recursion. That is the safe direction (it withholds a claim rather than
  making one), but it is an under-claim.
* **R1's cue list is not exhaustive** and is deliberately conservative: an unusual negation
  ("I decline to cast Doom Blade") is not caught, and an unrelated cue standing before the row name
  ("I cannot find a better line, so I cast Doom Blade") costs a re-ask.
