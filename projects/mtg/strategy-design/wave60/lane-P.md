# Wave-60 lane P — B9, B14 (engine state the render cannot see)

Branch `w60-lane-P`, base `8b922abf0`.

## B14 — the F2 residuals (wave-59 known-bugs 12/13)

### What lane G left, and what was actually wrong

Lane G (wave 58) closed the EXIT (the stall guard force-closes a reveal nothing can finish)
and listed four failed attempts at the CAUSE, concluding "the correct cause fix needs the
identity of the nested waiting element, which `MTGRevealingCards` does not hold."

It does hold it. Measured on the w58G fixture with a throwaway `typeid`/pointer probe
(removed before the gate):

```
[PROBE] wait=0x..756c0 type=GenericTargetAbility onLayer=108
        f1c=0x..756c0  f2c=0  trig2=1  menuObj=0
        second=0x..84170 type=MayAbility  first=type MayAbility
```

* `abilityFirst` and `abilitySecond` are **MayAbility** wrappers. The element that arms the
  reveal's target chooser is `MayAbility::mClone` — `reactToTargetClick` does
  `mClone = ability->clone(); mClone->addToGame()`. That clone is a PUBLIC member, so the
  identity lane G wanted is `dynamic_cast<MayAbility*>(abilityFirst)->mClone`.
* `wait == f1c`: the layer's waiting action after the decline is **option one's clone**, and it
  is ALIVE on the action layer (index 108), not a corpse and not an address collision.
* `f2c == 0` with `trig2 == 1`: option two's MayAbility triggered and armed its menu, the AI's
  menu answer landed on option ONE's clone (which reacts to clicks on the same source), and
  `MayAbility::testDestroy` then reaped option two as an unanswered may. That is why option
  two "never arms".
* A HUMAN never sees this: the decline branch runs `if (!controller()->isAI()) game->Update(0);`
  before building option two, and that one tick is what lets the clone leave. The AI path
  skips it.

### B14a — shipped

`AllAbilities.cpp`: `MTGRevealingCards::optionOneCloneArmed()` (is option one's clone still in
the action layer) and `buildOptionTwo()` (build, or DEFER for a tick while it is). Both the
DECLINE branch and the BTN_OK branch of `CheckUserInput` now route through `buildOptionTwo`;
driver phase 3 retries the deferred build directly (re-entering `CheckUserInput` would click
the source again and re-fire option one). The deferral is taken ONLY on the async-drive path
(`isInteractiveAI()` or the fixture's forced-async seam), so human and heuristic timing is
byte-identical to base. Nothing is capped, no window is removed, and a clone that never leaves
is still answerable to lane G's stall guard.

### B14b — shipped

`AllAbilities.cpp`: `MTGRevealingCards::drivingFor(GameObserver*, MTGCardInstance*)` — a live
scan of the action layer for an un-finished async reveal from that card. Deliberately a scan,
not an ownership stamp: `MTGRevealingCards` IS an ActionElement, so its presence in the layer
is its lifetime and no flag can go stale behind it.
`AIPlayerBaka.cpp`, `computeActions`: the target branch returns without answering when
`drivingFor(currentTc->source)` is true. The driver clicks its own picks through
`observer->cardClick`, so it is not gated by this. Lane G rejected Codex's version of this for
two reasons that do not apply here: this is not an unconditional early return in `Act` (it is
one branch, conditioned on a live driver), and nothing is auto-clicked on the model's behalf.
The MENU answer is deliberately NOT gated — the seat's menu answer is what ARMS option one in
the first place (`doReactTo 108` precedes `phase0 decided` in every trace); gating it would
stall every reveal.

### Fixtures (both RED on base, GREEN after)

Base binary for the RED: `archives/wagic-5039427da-w59step1` (engine-identical to `8b922abf0`,
which adds only strategy-design docs and Res guides). Both run under the 4G cap at THREADS=1.

* `bin/Res/test/w60P_reveal_decline_arms_option_two.txt` — w58G's board with **no
  `revealstallbudget`**, so the shipped guard (20000 ticks + a wall floor) cannot fire inside a
  script and the only question left is whether the reveal finishes BY ITSELF.
  **RED on base**: `==Card number not the same in player 0's library==, expected 7, got 2` —
  five cards stranded in an open reveal zone. **GREEN after**: library 7, reached through
  option two (`option two DEFERRED` -> `phase3: retrying the deferred option-two build`, and
  no force-close line).
* `bin/Res/test/w60P_reveal_decline_not_consumed_by_seat.txt` — the same board at
  `revealasyncticks 2`. **RED on base**: library 6 / hand 1 — the seat took "Get a human" after
  the model DECLINED, deterministic, exactly wave-59 known-bugs 13. **GREEN after**: hand 0,
  library 7.
* `w58G_reveal_decline_advances.txt` is untouched and still green (it pins the GUARD; this lane
  removes the guard's *need* on this path, not the guard).

Consequence for lane G's noted regression: on this path the revealed cards now go to the BOTTOM
of the library through option two, which is what Sigarda's Oracle says, instead of back to the
library by the force-close's least-harm contract.

## B9 — a creature exiled "until this leaves the battlefield"

`152v146` s35: Brutal Cathar chump-blocked while holding Nadaar, Selfless Paladin (4/4) in
exile; the block returned Nadaar and s43's crack-back counted it among 4 attackers for 14.

Mechanism (verified live, temporary probe in `ABlink::Update`, removed before the gate — a
Brutal Cathar fixture printed
`forsrc=1 src=Brutal Cathar onLayer=109 held=Gray Ogre blinkedFlag=1 inExile=1 pt=2/2`):
`(blink)forsrc` leaves a live `ABlink` **on the action layer** whose `source` is the exiler and
whose `Blinked` is the hostage, sitting in its owner's exile zone. Nothing in the render read it.

Shipped in `AIPlayerGPT.cpp`:

* `exileHostagesOf(card, out)` — the layer scan (blinkForSource, source match, `Blinked->blinked`,
  still in the owner's exile).
* `exileHostageDescriptor(held, me)` — `their Nadaar, Selfless Paladin (4/4)`; possession follows
  the OWNER, because that is the side `ABlink::returnCardIntoPlay` puts it back on.
* `exileHostageRowTag(descriptors)` — ` {if this leaves the battlefield: <list> comes back from
  exile}`. Condition FIRST (the annotation-wording rule), and it claims a RETURN on a condition,
  never a death: a bounced or sacrificed exiler hands the hostage back the same way, so
  "if this dies" would have been the narrower — and sometimes false — claim.
* Rendered on the BLOCKER rows (row-level, not per may-block entry: the return does not depend
  on which attacker it blocks) and on the ATTACKER rows in BOTH attacker-row builders.
* `crackBackExileReturnClause(count, power)` + two defaulted parameters on
  `crackBackNextTurnLine`. The hostage is **not** folded into the unconditional total — it is
  not on the battlefield, and inflating that number would be a claim about a board that does not
  exist. It rides its own clause: ` - and 1 more of theirs (4 power) comes back from exile as
  soon as the creature holding it leaves the battlefield`. The counts are gathered off the
  SEAT's own battlefield in the existing crack-back walk (the exilers are ours; the hostages are
  in exile and appear in no board walk at all).

PARSETEST: 15 new cases — the exact clause text, condition-first, the empty-list negative, the
never-claims-a-death negative, the plural form, the unconditional-total-untouched check, a
byte-identical regression against the wave-59 line when there is no hostage, two zero-value
negatives, and the echo shapes (an `ATTACK:` and a `BLOCK:` reply trailing the clause still
bind, a `B1:none` echo stays declined, and the clause parses exactly as the wave-56 block-cost
clause it renders beside).

## Gate

Detached unit `w60-P-gate`, 4G cap, worktree binary rebuilt with `rm -f bin/wagic && make -f
Makefile.sdl -j4`.

| | base (`wagic-5039427da-w59step1`, base registry) | this lane |
|---|---|---|
| suite, THREADS=1 | 1247 tests, **0 failed**, 62 AI, 0 failed | 1247 tests, **0 failed**, **64 AI, 0 failed** |
| `==Test timed out` | 0 | **0** |
| PARSETEST | 3529 / 0 failed | **3544 / 0 failed** (+15) |

AI count 62 -> 64 is exactly this lane's two fixtures. (The brief's "61 AI" figure was measured
on `5039427da`'s registry; on our base `8b922abf0` the same binary reads 62 — measured, above.)
`git diff | /usr/bin/grep -c $'\357\277\275'` = 0.

## Predictions (falsifiable, next corpus)

1. **B14a**: zero `reveal_stall_forced` records whose driver phase is 5 or 6 on a DECLINED
   interactive reveal. Force-closes may still appear for other shapes (a foreign consumer, a
   card with no option two); one that names phase 5/6 after a decline means the deferral did not
   reach that path. Corollary: on Sigarda-shaped reveals the revealed remainder now reaches the
   BOTTOM of the library, so a seat that declines no longer redraws the same five cards.
2. **B14b**: zero decisions in which a reveal record classes the model's answer as a decline
   while the narration in the same seat's next frame reports the option-one card arriving
   (`-> your hand` / `-> the battlefield`) from that reveal. Wave 59 had no window on this path;
   if the next corpus also has none, the prediction is untested, not confirmed — check the
   count of interactive-reveal asks first.
3. **B9**: every blockers/attackers ask whose seat controls a live `(blink)forsrc` exiler
   carries `if this leaves the battlefield:` on that creature's row, and every own-turn
   `CRACK-BACK NEXT TURN` line on such a board carries the ` - and N more of theirs (M power)`
   clause. Falsifier: a corpus game in which a creature returns from exile (the narration says
   so) and no ask before it printed the clause.
4. **B9 (behavioural, weaker)**: a chump-block that hands back a body the seat could not survive
   should become rarer, but a single corpus cannot separate that from variance — the render
   claim above is the testable half.

## What I did NOT verify

* **No live model probe and no corpus run.** B9's wiring is verified in two halves — the pure
  text functions by PARSETEST, and the engine state they read by the `ABlink::Update` probe run
  above — but I never saw the composed clause in a real prompt, because a GPT seat with no
  endpoint falls back to Baka and renders nothing. The first corpus is its real gate.
* **B9 ships no suite fixture.** It is a render change (PARSETEST is its gate per the brief);
  the engine state behind it was already correct on base, so there is nothing to make RED.
* **The two attacker-row builders were patched identically without establishing which is which.**
  Both push into `aRowRest`; I did not trace which window reaches each.
* **The B14b guard's blast radius beyond reveals is unmeasured.** `drivingFor` is false whenever
  no async reveal is live, so the branch is inert elsewhere by construction, but the suite has
  no fixture in which a seat legitimately needs to answer a chooser during someone else's live
  reveal.
* **Multi-hostage and self-owned-hostage rendering is PARSETEST-only.** No board in the suite or
  corpus has two `(blink)forsrc` exilers at once, and none has a seat exiling its own creature
  that way.
* **The build is incremental on a seeded `objs-sdl`, not `make -B`.** The seed came from
  master's build of this exact base commit and the `include/AllAbilities.h` edit forced a wide
  recompile, but I did not run the hermetic rebuild the wave-52 lane-J note asks for before a
  SHIPPING gate.
* I did not re-run the wave-58 corpus games (152v126, 152v146) under the fixed binary.
