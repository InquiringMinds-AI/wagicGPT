# Wave-73 lane BY — the HOLD family and the window key (N1, N2, N5, N6, N9, N16)

Base `90f8d1d68`, worktree `worktrees/lanes/w73-BY`, branch `w73-lane-BY`.
Gates: suite THREADS=1 **1281 tests / 0 failed, 76 AI tests / 0 failed** (none of the three
known flakes fired); PARSETEST **5516 passed / 0 failed** against base **5487 / 0** — delta
**+29**, exactly this lane's additions. Both link guards OK (`check-ctor-init` 118 files,
`check-reply-instructions` 1 source + 37 guides). `git diff | grep -c U+FFFD` = 0. No corpus run.

Nothing in this lane removes, caps or auto-answers a legal option. The one collapse it widens
(N6) fires only where `LegalActionsOracle::hasAnyLegalAction` says the seat has NO legal action
at all, and the one it repairs (N1) can only fire on a menu whose ACTING rows are a subset of a
menu the model already answered.

---

## N2 HIGH — THE HOLD FAMILY. CONFIRMED (four seats, one mechanism); FIXED in four parts.

**The mechanism.** A hold's only release was "a printed row changed", and a ONCE-PER-TURN row
renders the same string on turn 13 and turn 25. So on a menu whose acting rows are loyalty
abilities, a `{T}:` maker or a repeat activation, the release condition cannot occur while the
permanent sits there, and "pass this window" silently became "concede this permanent".

**(a) The untap release.** `w73HoldExpiresAtUntap(heldTurn, nowTurn, ownTurnNow)` — a hold ends
at the start of the holder's next turn, whatever the rows do. `holdHonoured` checks it before
every other arm, drops every seam's latch together and counts `hold_released_turn` on the
gameend record. The release direction is PERMISSIVE: the model is asked again and may take the
row again over the same board.
*RED on base:* base has no release path at all; reverting the predicate to `return false` fails
all three POSITIVE pins. `152v162` seq 34 (Teferi silenced for 6 turns, died at 0), `123v130`
seq 31 (Create row held t7→t22), `125v130` seq 46 (t27→t31 at 18→6 life) each become a one-turn
hold instead of a game-long one.

**(b) The pass ROW on every priority window.** `AIPlayerGPT.cpp` printed `0. Pass priority (take
no action this window)` only under `baseIndex == 1`, while the answer sentence has always said
"0 = pass priority" unconditionally — 36 of deck146's 66 priority windows advertised a pass with
no row for it, and HOLD was taken as a pass 12 times with the reply's own PLAN saying "pass"
(the model names the gap itself at `146v125` seq 180). The guard is deleted. Row 0 is an index
the parser has always accepted, so the ANSWER GRAMMAR does not move; the row stays out of
`shownLines` exactly as it always was, so no hold row set and no option-set key moves either.

**(c) The row says so.** All three hold spellings now read
`Hold priority - pass now, and do not ask me again - YOU CANNOT COME BACK AND TAKE ONE OF THE
ROWS ABOVE LATER THIS TURN - it stands until your next turn begins, or until one of the rows
above changes (...)`. The shared `kHoldPriorityRowHead` prefix is preserved byte-compatibly with
`holdRowIndexOf` / the reserved-echo binder, and the two decline rows still lead with different
words (wave-55 D11b).

**(d) N9 — the cost LEADS.** The "you cannot take one of these later this turn" clause used to
sit ~600 chars into the benefit brace (`125` seq 36/101 held over their own +1). It is now the
row's headline, ahead of `any change re-opens this window`, and the brace OPENS with the untap
release so its scope sentence cannot read as forever.

**Prediction (falsifiable, next corpus).** `hold_released_turn` > 0 in any game where a hold is
taken at least one turn before the holder's next untap; no seat has a suppressed-window run
spanning more than one of its own turns; `hold_windows_skipped_priority` falls; every priority
prompt contains a `0. Pass priority` row (0 prompts with `0 = pass priority` and no row), and
the share of priority HOLD takes whose own PLAN line says "pass" falls from deck146's 12/20.

## N1 HIGH — `sibling_window_asks_skipped` = 0. CONFIRMED; key fixed, covering rule stated.

**Cause (as the engine seat read it, re-verified in code).** `gptHoldCoversSiblingWindow`
compares two `serializeGameState()` strings; BT M13 / BX F3 put the CASTING-DECISION STATE
inside that string (a hand card reads `[castable now]` while the cast window is open and carries
the answered tag once it closes), and `takeHold` records the board from INSIDE the cast loop.
So the sibling priority window's board differed from the recorded one on every castable hand
card — 19 collapses in wave 71 became 0 in wave 72. Two fixes cancelled each other.

**Fix.** `w73SiblingBoardKey()` normalises the one annotation the cast window's own state writes
(answered tag → `[castable now]`) and is applied at BOTH ends (`takeHold`'s record and
`holdHonoured`'s comparison). The two tag strings are now single constants that
`handCastabilityTag` returns, so a second copy cannot break the key silently. The rendered
prompt is untouched.

**The covering rule, stated (and now printed).** *The collapse stands only for the OTHER seam of
the SAME window — same turn, same phase, the same board once the casting seam's own answered-tag
is normalised away — AND only when every ACTING row of the new menu was already printed on the
held menu. Declines and hold rows are not acting rows.* Every skip's DebugTrace now carries the
rule plus BOTH option sets (`held rows:` / `rows now:`), so a corpus reader can adjudicate a
skip without re-deriving the boards.

**Honest consequence.** With the key repaired, BX F1's subset rule is still the second bar, and
the engine seat is right that the cast and priority menus are largely disjoint by construction —
so the collapse will fire mainly on siblings whose ACTING set is EMPTY (a menu that offers
nothing but declines). That is a real, safe saving and it is the only one the doctrine permits;
this lane does not widen the rule to a union, because a union would close a window carrying a
row the model has not seen.

**Prediction.** `sibling_window_asks_skipped` > 0 next corpus; every skip's stderr line shows a
`rows now:` set whose acting entries all appear in `held rows:`; re-running
`lane-BU-double-ask-red.py` shows the HOLD class of same-board double asks at or near 0 while
the "cast nothing" class survives.

## N5 HIGH — the LOOP COMPLETE header contradicted the LOOP SCOPE clause. CONFIRMED; FIXED.

The header's tail ("a number that reads as life to them is lethal to you") was FALSE for the
dominant number on deck162's screen and the `LOOP SCOPE:` clause in the same prompt said so;
`162v126` seq 29 obeyed the header, held its only removal and lost 0-28. New shared emitter
`loopChainEntrySentence(theirs)` composes ONE sentence that both surfaces now carry byte for
byte (`Their chain starts only from life YOU lose or life THEY gain - life THEY lose does not
enter it.`); the false half is deleted, the true half ("fatal rather than expensive") still
leads. Pinned by substring identity on both faces plus a MUST-NOT-MATCH on the deleted clause.

**Second half — the `[LOOP RUNNING ...]` bracket.** Its one actionable sentence pointed at the
HOLD row and was printed over a menu whose rows 1-4 were Ob Nixilis loyalty activations that do
not enter the chain. `loopChainingNote` now takes `actingRowsPresent`, read off the same rows the
menu is about to print with the hold latch's own decline/hold filter, and at both seams. With an
acting row present the hold-answers-all sentence is REPLACED by "the rows above are NOT part of
the chain and are not answered by it: each one is a play that stands on its own, and the HOLD row
gives every one of them up." Nothing is deleted and no row is removed; the wave-66 sentence
still stands on a menu that carries only declines, which is the window class it was written for.

**Prediction.** 0 prompts contain both `the HOLD row answers all of them at once` and a numbered
activation/cast row; the header and the scope clause never state opposite rules in one prompt
(grep: every prompt containing `LOOP COMPLETE` and `LOOP SCOPE` carries the same entry sentence
twice); a deck162-shaped hold over live removal does not recur.

## N6 HIGH — a resolving chain put to the seat once per link. CONFIRMED; the collapse generalised.

`130v162` turn 10 is 20 consecutive windows, one per link of an unstoppable trigger chain, each
re-opened because the countdown ("13 damage … 11 … 6") is a figure the hold's same-row carve-out
does not forgive, and deck130's only instant was not castable in any of them. `chainAutoPassApplies
(chainOnStack, oppLoopProven, anyLegalAction)` generalises the wave-66 life-loop arm to ANY
resolving chain: a NOT_RESOLVED object on the stack AND the oracle's own "this seat has no legal
action at all". No legal option is lost because there is none — the only rows such a window can
print are the pass/hold/decline family. Fail-closed as before (any legal action, and the ask goes
out). The old arm is a strict subset, pinned as such. Counted as `chain_windows_collapsed`, and
the narration receipt no longer claims a life LOOP it may not have been.

**Prediction.** `chain_windows_collapsed` > 0 in any game with a multi-link trigger chain; no
turn shows 20 windows for one chain; total decisions/turn falls slightly with no game losing a
window in which it had a legal play (cross-check: no collapse stderr line on a turn whose next
record offers a cast row over the same board).

## N16 MED — a main-1 casting window that did not fire. INVESTIGATED; instrumented, not changed.

**Measured.** Corpus-wide, 2 of 102 own turns carrying an attackers ask have NO `Main phase 1`
record: `152v146` t18 (the review's case — Teferi ×2 and Katilda all `[castable now]` at main 2
off the same 4 untapped sources) and `146v162` t9 (which had spent its mana on a draw-step Soul
Shatter and had 1 source left, so cost there is likely nil). The `askreplay/` sidecar has NO file
for the deck152 seat, so this is not a re-served answer; `async_drops` = 0 on that seat's gameend,
so it is not a dropped answer either. The stderr shows main 1 entered and a phase advance
requested with no land-drop trace and no casting-decision trace at all.

**Mechanism (code reading, NOT yet reproduced).** `AIPlayerBaka::computeActions` has two reach
branches. Only the second ("standard actions", guarded by an empty stack) offers a SORCERY-SPEED
window; the first is instant-speed only and its reach test is
`isInterrupting == this && topController != this` — and a NextGamePhase object has NO controller,
so that arm can be true DURING the seat's own main phase. When it is, `FindCardToPlay` is called
with the instant-only filter, every non-instant is dropped, and the whole main phase passes with
no cast ask. `own_turn_windows_skipped` cannot see this: it is scoped to the non-main
instant-speed phases (`ownInstantSpeedWindow`), which is why the class was invisible.

**Shipped: the instrument, not the behaviour change.** New report-only hook
`noteMainPhaseWindowSkipped(why)` (virtual on Baka, counted on GPT) fires on BOTH ways a seat's
own main phase can reach no casting window — the instant-speed response arm answering it, and an
unresolved stack object keeping the standard branch out of reach — with the reason on stderr and
`main_phase_windows_skipped` on the gameend record. I did NOT change the reach rule: I could not
produce a RED fixture for it, and re-routing a main phase out of the response arm is a change to
the hottest path in the AI on 2-of-102 evidence. The next corpus will say which arm swallows the
phase and how often, and that is the gate for fixing it.

**Prediction.** `main_phase_windows_skipped` is non-zero and equals the number of own turns with
no `Main phase 1` record; the stderr reason on the deck152-shaped case names the instant-speed
response arm.

---

## Weakest evidence (declared)

1. **N16's mechanism is a code-reading hypothesis, not a reproduction.** Neither the suite nor a
   probe produced the missing window; the instrument exists precisely because I could not.
2. **N1's benefit is smaller than the counter name suggests.** The key is repaired and provably
   comparable, but BX F1's subset rule still bars every sibling that offers a row the cast menu
   did not print — so the expected firing set is menus whose acting-row set is empty. If the next
   corpus shows `sibling_window_asks_skipped` still 0, the remaining bar is F1, not the key, and
   the decision then is whether to price a union rule (which this lane declines to take).
3. **N2(a)'s release partially re-instates a turn boundary that wave-61 C14 deliberately removed.**
   C14's evidence (`146v125` seqs 92-101, ten asks over one unchanged menu) spanned a turn
   boundary; the untap rule will re-open one window per own turn in that shape. That is the
   deliberate trade — one extra ask per turn against a game-long concession — but it is a real
   cost and the next corpus's `hold_released_turn` against decisions/turn is how to adjudicate it.
4. **N6's collapse is gated on `hasAnyLegalAction`, which is documented as permissive.** If it
   over-reports "yes" the collapse under-fires (safe); if a future change makes it strict it
   could under-report and collapse a window with a play. The pin covers the predicate, not the
   oracle.
5. **No corpus and no live game was run**, so every render change here is verified only by
   PARSETEST literals and the suite — not by a model reading it.
