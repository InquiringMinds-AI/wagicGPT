# Lane X — the 13-hour engine HANG (wave-53 corpus, game 152v125)

## The sequence, in plain words

Turn 15. deck152 (GPT seat, Bant Midrange) is at 20, deck125 at 9. deck152 levels
Ranger Class to 2 and then to 3 (seq 24, 25), then declares all four creatures as
attackers (seq 26: Elite Spellbinder, Moonrage Brute, Sigarda, Wolf).

Two attack triggers go on the stack together: Ranger Class's level-2 "whenever you
attack, put a +1/+1 counter on target attacking creature", and Sigarda's Coven
trigger ("look at the top five cards of your library; you may put a Human creature
card from among them into your hand; put the rest on the bottom").

Ranger Class's trigger resolves first and is answered normally (seq 27, kind `ask`,
4 options, target Sigarda). stderr shows both StackAbilities added, Ranger Class
resolving, then Sigarda's resolving. That is the last real line in the file.

Sigarda's reveal opens on the top five: Branchloft Pathway x2, Barkchannel Pathway,
Fateful Absence, Ranger Class. **No Human among them.** Option one of the reveal is
`target(<upto:1>human|reveal) moveto(hand)` — zero legal targets.

From there the engine never ticks again: ~13 hours at the Blockers step, no stderr,
no further seat records. The seat log's last line is seq 28 — kind `reveal`, 5
options, `choice` -1, `chosen_text` "none (no legal target)", `latency_ms` -1,
`prompt` 0: an ask answered with no model call.

Path to Exile (the deck125 seat's last record, seq 17, turn 12) is not part of this;
it is simply where that seat's log stops because the game stopped.

## The seam

`MTGRevealingCards` (src/AllAbilities.cpp), the interactive-AI reveal driver.

`toResolve()` picks the reveal's branch **up front**, using option one's own
predicate: `countValidTargets() == 0` leaves `abilityFirst` NULL and constructs
**option TWO** instead (`target(*|reveal) bottomoflibrary and!( all(*|reveal)
bottomoflibrary )!`). So on a no-legal-target reveal the chooser standing in the
action layer belongs to option two.

`driveInteractiveReveal()` phase 0 did not know that. It asked the seat
(`AIPlayerGPT::decideReveal`, which self-declines through the N-136a empty-eligible
shortcut — the seq-28 record, no model call, return -1), then issued its finalize
`CheckUserInput(JGE_BTN_NEXT)` against the chooser it assumed was option one's.
`CheckUserInput`'s decline branch — the one that builds option two — is gated on
`!abilitySecond`, so with option two already built the press could not take it, and
option two's window was spent on zero targets. Nothing left the reveal zone.

Phase 3 then reached `if (!tc) return;` and waited for an option-two chooser that
had already come and gone. The reveal display stays open, and an open display holds
every phase-advance path — including `userRequestNextGamePhase`. No tick, and no
stderr because the driver's own tracing is env-gated (`WAGIC_REVEAL_DEBUG`).

Not card-specific: any reveal whose option one has no legal target among the
revealed cards hangs the same way for an interactive-AI seat. Four of the five Coven
firings in the corpus contained a Human and completed normally, which is why it
reads as intermittent.

## Reproduction

`bin/Res/test/w53_reveal_no_eligible_option_one.txt`, GPT-free. Sigarda's own name
carries commas, which suite zone lists split on, so the fixture drives Lexicon
Hurkyl Wizard (test/lexicon/test_primitives.txt) — it already carries the same
reveal shape: a gated option one plus `optiontwo target(*|reveal) bottomoflibrary
and!( all(*|reveal) bottomoflibrary )!`. Its option one takes noncreature nonland
cards; the fixture's top five are all creatures, so option one has zero legal
targets and the engine arms option two up front — the live shape exactly.
`revealasync` names a card matching nothing, so the test stub decides with an empty
pick set: the same downstream state as the live -1, with no model in the loop.

- RED on base (master 859186536 binary): `phase problem. Expected [Cleanup](12), got
  [End](11)` and the five revealed cards stranded in the reveal zone (library 1 of
  6). The engine stopped ticking; only the fixture script running out ends the run.
  Under a real game there is no script, which is the 13 hours.
- GREEN after: the reveal completes, all six cards are in the library, the turn
  reaches Cleanup.

## The fix

Both changes are in the driver, not in any card.

1. **Phase 0 — the root cause.** When `!abilityFirst && abilitySecond` (the engine
   already ruled option one has no legal target and armed option two), there is no
   option-one decision to commit: take no picks, press nothing, and hand straight to
   phase 3, which finds option two armed and clicks the remainder into it. The ask
   is kept — it self-declines with "none (no legal target)" and narrates, at no
   model call, which is the truth of that board. Picks are explicitly dropped here:
   a click would land on option two and bottom the card the model asked to keep.
   A `DebugTrace` line now prints when this path is taken.
2. **Phase 3 — a floor under every other path.** If option two has been reaped from
   the action layer while the reveal zone is still full, its window was consumed and
   the wait can never end. Rebuild it once (latched by `mAISecondRebuilt`) with a
   stderr line naming what happened, so no reveal can leave the game without a tick
   that advances it, and so any remaining early-consuming path shows up in a log
   instead of as a silent hang.

Files: `projects/mtg/src/AllAbilities.cpp`, `projects/mtg/include/AllAbilities.h`,
fixture + `_tests.txt`.

## Gate

Suite 1216 tests / 2 failed / 0 timed out — the two are the known concurrency-only
pair (lifeline.txt, merrow_reejerey.txt); 46 AI tests / 0 failed. PARSETEST
2249 passed / 0 failed. (Baseline 1215/2-known + 46/0 and 2249/0; the count grows by
this lane's one new fixture.)

## What I did NOT verify

- No live-model run. The repro and the fix are exercised through the fixture's
  test-async stub, which produces the same driver state as the live -1 but does not
  re-drive the real `decideReveal` against the 35B endpoint.
- Not verified against the real Sigarda, Champion of Light card in a fixture (comma
  in the name); the lexicon clone matches the reveal construct, not Sigarda's coven
  restriction or its combat trigger.
- The phase-3 rebuild guard is not covered by a test — nothing in the suite now
  reaches it, because phase 0 removes the one known path that did. It is a floor,
  and its stderr line is how a future path that hits it gets found.
- Not measured: whether other seams (interrupt windows, `extendInterruptOffer`) can
  produce a comparable no-tick park. The corpus hang was fully explained by this
  seam, so those were not swept.
- The other reveal-driver phases (a genuine option-one decline, surveil binning) are
  unchanged and were checked only through the existing suite fixtures passing.
