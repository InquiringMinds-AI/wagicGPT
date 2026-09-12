# Wave-79 lane DA — combat truth (T5, T6, T7, T9, T16)

Base: `06fad1449` (master after the wave-78 known-bugs commit).
PARSETEST baseline 6642 passed / 0 failed -> **6708 passed / 0 failed** (+66).
Suite baseline "1291 tests (0 failed), 76 AI tests (0 failed)" -> see GATE below (+2 fixtures).

Every number quoted below was read off the wave-78 corpus
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-125420-final`, from the `prompt`
field of the cited record — i.e. the exact bytes the model saw.

---

## T5 HIGH — the `AT BEST` figure was neither a bound nor an optimum

**RED (three corpus boards, reproduced byte-for-byte in PARSETEST):**

* `146v152` seq 21 (t12, 20 life). A1 Sigarda 6/6 [trample, flying], A2 Luminarch
  Aspirant 2/2; B1 Spider 2/1 [reach] (may block both), B2 Goblin 1/1 (A2 only).
  Rendered: `at least 6 of that lands whatever you block (trample/menace counted
  as unblocked): you would be at 14 AT BEST (no assignment of your blockers does
  better)`. The seat played `B1:A1, B2:A2` and **ended at 15**.
  `assignableRemainderDamage` returns 6 on those inputs — pinned as RED.
* `162v152` seq 19 (t11, 18 life). 16 unblocked, floor 6 -> `12 AT BEST`; Fog Bank
  0/2 soaks 2 of Sigarda's 6, true best 4 -> **14**. Pinned RED at 6, GREEN at 4.
* `146v123` seq 19 (t?, 18 life). Header: `no assignment of your blockers survives
  this` (best case -6) **and** `26 KILLS you at 18 life: this line LOSES THE GAME
  and the line above does not` — two verdicts, one screen, disagreeing.
* `146v152` seq 26. `GANG BLOCK: any 3 of yours together deal 6, enough to kill
  this attacker` on a 6/6 **trampler** whose own B-row says `5 tramples to your
  face` for a lone block. Model reasoning: "0 trample". It took 3.

**Fix (two sentences):** `w79BestBlockDamage` is an exact DP over (attacker index x
free-blocker bitmask) that models the two things the wave-58 matching could not —
trample spill (`power - sum of the blockers' lethal cuts`, CR 702.19b, deathtouch
cutting each to 1 per CR 702.2b) and a menace attacker being fully blocked by two
bodies (CR 509.1c) — and it is run exactly where `exactAssignment` is already
false, i.e. only on boards carrying a trample/menace attacker, bounded at 12x12.
Where it runs the figure becomes a proven maximum with a named assignment; where
it cannot (bigger boards) the wording drops both the optimality claim and the
death badge, the gang clause states its group's spill from the same group it
names, and `blockKeepAlternativeClause` reads the life line's own lethality off
the two numbers it already holds so a header can no longer print two verdicts
that disagree.

**GREEN:**
* PARSETEST: `w79BestBlockDamage` = 5 on the seq-21 inputs (life 15) with
  `match == {Spider->Sigarda, Goblin->Luminarch}` — the seat's own line; = 4 on
  seq 19 (life 14); menace 4 -> 0 with two bodies, 4 with one; deathtouch trampler
  6 into a 5-toughness body -> 5.
* **Suite, the engine's own damage code** (the live combat path, not the
  reviewer's arithmetic): `w79da_t5_trample_soak_decline.txt` (2/1 in front of the
  2/2, trampler unblocked) ends at **14** — the wave-78 "ceiling" — and
  `w79da_t5_trample_soak_block.txt` (2/1 in front of the trampler, 1/1 on the 2/2)
  ends at **15**. 15 > 14 from GuiCombat/Damage, so the claim was false, not
  merely different.
* Gang clause now appends `this attacker has TRAMPLE ... it assigns its lethal
  damage to that group (3 of toughness between them) and 3 still tramples to your
  face`, byte-prefixed by the unchanged wave-78 kill claim.
* `blockKeepAlternativeClause(26, 24, 18)` now reads `... this line LOSES THE GAME
  - and so does the line above (24 at 18 life). Neither line on this screen
  survives; the one above simply loses by less.`

**Prediction (falsifiable, wave-79 engine seat):** 0 records whose blockers header
prints an `AT BEST` / `best case` life figure that a legal assignment on the same
board beats. Concretely: on every `blockers` record carrying a trample or menace
attacker, the header's stated figure must be `>=` the life the seat actually holds
at the end of that combat; `at_best_beaten_by_legal_block` must be **0**, and
`exact_block_search_ran` must be **> 0** (a 0 there is a FAIL, not "untested" —
it means the DP's gate never admitted a real board). And 0 headers containing
both `no assignment of your blockers survives` and `and the line above does not`.

---

## T6 HIGH — the crack-back cover subtracted from the wrong total

**RED:** `162v146` seq 15. The CRACK-BACK line's own ADD-THOSE-UP sentence reads
`the total to subtract from your life is 10 and you would be at -2; that would
KILL you`; the Master of the Feast row's `{crack-back cover: ...}` reads `the
CRACK-BACK NEXT TURN line above is 7 from 3 of their creatures ... you cover 3 of
7, leaving 4 -> you would be at 4`. The model answered "Safe enough"; the game
ended at -2. Pinned: `crackBackBlockerRowTag(7, 8, 1, 0, atk, true, 0)` reproduces
`you cover 3 of 7, leaving 4 -> you would be at 4`.

**Fix:** `crackBackBlockerRowTag` takes the SAME `floorExtra` figure the line above
added (read from `crackBackFloorSources`, not recomputed) and subtracts its cover
from `total + floorExtra`, naming both halves; the cover itself is still capped at
the creature total, because bodies block creatures.

**GREEN:** the clause now prints `... is 7 from 3 of their creatures PLUS the 3
that line's own ADD THOSE UP sentence adds ... - 10 in total ... and puts you at
-2 ... you cover 3 of 10, leaving 7 -> you would be at 1`. With no ADD-THOSE-UP
figure on the screen every byte is wave 78's (pinned both directions).

**Prediction:** 0 windows in the wave-79 corpus where a `{crack-back cover:` clause
and the `CRACK-BACK NEXT TURN` line on the same prompt state different totals;
`crackback_cover_total_mismatch` = 0 and `crackback_cover_with_floor_extra` > 0.

---

## T7 HIGH — `they cannot attack this turn` is false under an untap engine

**RED:** `152v123` seqs 17/18. `{paying this taps: Katilda, Dawnhart Prime,
Briarbridge Tracker - they cannot attack this turn}` across the table from a live
**Intruder Alarm** (`@movedTo(creature|...):untap all(creature)`). Both named
bodies attacked at seq 21. Pinned: `paymentTapsClause` reproduces the seq-17 bytes
exactly.

**Fix:** `w79UntapEngineSources` scans BOTH battlefields for a script line that
untaps a SET of creatures (predicate is the effect shape, not a card name; the
standing `doesnotuntap` half of the same card is deleted before the scan so no
no-untap lord reads as an untap engine, and an unreadable script proves nothing).
Where one exists the clause keeps its restriction and names the exception after it
(`- UNLESS Intruder Alarm (THEIRS) untaps them first: ...`), on all three live
emitters (cast row, activation row, pay-repeat row).

**GREEN:** pinned on the seq-17 names; the no-restriction row and the no-engine
board both render the wave-78 bytes unchanged.

**Prediction:** in the wave-79 corpus, every `{paying this taps:` clause rendered
on a board carrying an untap-all permanent contains `UNLESS`; `tap_bill_untap_
exception_rendered` > 0 and `tap_bill_false_under_untap_engine` = 0 (a clause with
no `UNLESS` on such a board).

---

## T9 HIGH — an unpriced spent blocker, and a named-but-unfolded punisher

**RED (a):** `130v146` seq 53, 1 life: `Deal 2 damage with Siege-Gang Commander
targeting Goblin #1 ... {removes 2 from the CRACK-BACK total above: 7 -> 5 - you
would be at -4; that still KILLS you} [cost: {1}{r}, Sacrifice]`. Two windows
later (seq 55) the blockers header computes exactly the lost blocker (`best case
with every blocker assigned: you would be at -1`). Pinned byte-for-byte.

**RED (b):** `126v125` seq 83 (and 95/116/120/125): `ATTACK TOTAL: 1 attacker
listed, 1 total combat damage ... At least 1 damage lands whatever they BLOCK`
with Lightmine Field named on the same line and explicitly NOT folded. The 1/1
died to the declaration trigger five times for 0 damage dealt.

**Fix:** (a) `w79SacrificeBlockerGiveBack` asks the cost's own `SacrificeCost`
TargetChooser whether it can take one of the seat's untapped blockers, and prices
the loss with the cover clause's own model (`w79CoveredByBodies`: attackers by
power desc, each costing `minBlockersRequired()` bodies); the relief clause states
the give-back and computes its kill verdict off the corrected figure. (b) the
punisher's amount is exactly computable for the ONE declaration `ATTACK TOTAL`
prices — all of them — so `attackDeclarationPunisherSize` reads it
(`foreach(creature[attacking]` scales by the declaration, a non-numeric magnitude
claims nothing) and the line folds it: which attackers die, how much of the total
that removes, and the resulting life.

**GREEN:** relief clause -> `you would be at -4 - but the Sacrifice in this row's
own cost takes one of your 4 untapped blockers off the board before that attack,
and that body was covering 2 of the same total: count it back and you would be at
-6; that still KILLS you`. ATTACK TOTAL -> `FOLDED, for the all-in declaration
this line prices: Lightmine Field deals 1 to EACH of the 1 attacker you would
declare ... That kills 1 of them (Vampire #1), so 1 of the 1 above never gets
dealt: 0 reaches them and they would be at 41 - not 40.` The wave-61 scoping
sentence is kept beside it for every other declaration; an unsized punisher leaves
every wave-78 byte alone.

**Prediction:** `sacrifice_row_blocker_priced` > 0 and 0 `[cost: ..., Sacrifice]`
rows carrying a `{removes ... CRACK-BACK ...}` clause with no give-back sentence
on a board where the seat has an untapped blocker the cost can take;
`attack_total_punisher_folded` > 0 and 0 declarations of a creature whose toughness
is at or below the folded punisher figure.

---

## T16 MED — the stack's own consequences, and an untap keyword with no state

**RED (a):** `130v162` seq 49, 1 life. Stack: 2 queued draw abilities + 3 damage
abilities. `ON THE STACK: 3 damage to you - you would be at -2; that would KILL
you` while the SAME screen's `DRAW FORECAST` says the 2 remaining draws cost 6
more and put the seat at -5. Pinned byte-for-byte.

**RED (b):** `146v123` seq 15 (~1,300 reasoning chars): `Human #1-#102 (1/1)
[doesn't untap during its controller's untap step]` on UNTAPPED bodies. The board
line prints `[tapped ...]` when tapped and NOTHING when not, so absence means
untapped everywhere else and the keyword filled the silence with the opposite.

**Fix:** (a) `w79QueuedDrawPunisherLoss` = `stackPendingDrawsFor` x the opponent's
per-draw punisher rate; it is added inside `pendingStackLifeLossToSeat` (so the
stack-death verdict key the hold latch reads, the X ladder and the row clause all
move with it) and printed as its own named term in the line. (b) a permanent
carrying `DOESNOTUNTAP` and not tapped renders `[untapped]`.

**GREEN:** `ON THE STACK: 3 damage to you from the damage abilities listed above,
plus 6 more that the 2 queued draws on this same stack cost you (2 x 3 per draw to
their Underworld Dreams #1, Underworld Dreams #2, Ob Nixilis, the Hate-Twisted - a
queued draw is not declinable) = 9 in total - you would be at -8; that would KILL
you` — the same -8 the DRAW FORECAST already implied. A draw-only stack is now
priced where wave 78 printed nothing. `[untapped]` pinned as a `[...]` group that
changes no action key.

**Prediction:** 0 prompts where `ON THE STACK:` and `DRAW FORECAST` disagree about
the seat's post-stack life; `stack_total_with_queued_draws` > 0. And 0 reasoning
bodies in the corpus that call an untapped `doesnotuntap` body tapped;
`untapped_face_rendered` > 0.

---

## Key stability (LESSON OF WAVE 74/78)

`[#W79-DA] KEY` builds the JOINED menu (one acting cast row + the hold row beside
it) twice, differing ONLY in this wave's numbers — the T6 cover totals, the T7
untap exception, the T9 give-back — and asserts, through the LIVE builders:

* `optionSetKeyOf(menu)` identical;
* `holdActionKeyRow(row)` identical, and the whole latched row SET (built the way
  the live seam builds it, `holdActionKeyRow` over `mLastMenuRows`) identical;
* `w77KeyTailOf(tail)` identical — this is the ask key's AND the async slot's half
  of the rendered list;
* MUST-NOT-MATCH: none of the three clause bodies survives into the key, while the
  action and its cost pips do.

Board-frame lines (INCOMING THIS COMBAT, ON THE STACK, ATTACK TOTAL, `[untapped]`)
are not menu rows; they live in `serializeGameState`, the board half of the ask and
async keys, which already moves with every life total and tap state on the board.
`[untapped]` specifically flips only when `[tapped ...]` flips, so it introduces no
key motion that the tapped face did not already introduce (pinned).

---

## Weakest evidence (read this before trusting the above)

1. **No corpus, no live seat.** Lane rule: no corpus and no pilot requests from a
   lane. Every T5/T6/T7/T9/T16 claim about the RENDERED prompt is proven at the
   emitter (PARSETEST, corpus-derived inputs) and, for T5 only, at the ENGINE
   (two suite fixtures). The path from the live board walk into those emitters is
   unexercised for T6, T7, T9 and T16 — this is exactly the S6/S3 shape the wave-78
   lesson names, so each prediction above carries a **non-zero** counter whose 0 is
   a FAIL.
2. **T5 DP bound.** `w79BestBlockDamage` declines past 12 attackers x 12 blockers.
   `146v123` seq 19 (27 attackers) is past it. On such boards the header now prints
   the honest unsearched wording rather than a false optimum, but it gives the seat
   no better line. Widening the bound needs a different algorithm.
3. **T5 fidelity gaps in the DP's combat model.** It prices trample spill off
   remaining toughness and deathtouch, and nothing else: a first-strike blocker
   that kills the trampler before it deals damage, damage prevention on the face
   (Fog Bank's own `preventAllCombatDamage`), protection, and becomes-blocked
   pumps are all outside it. Each of those makes the printed figure PESSIMISTIC
   (more damage than reality), which is the safe direction for a survival claim
   but can still understate a reachable life.
4. **T5 material pass suppressed.** When the DP's answer blocks a trampler/menace
   attacker or stacks two bodies on one attacker, `improveAssignmentMaterial` is
   skipped and the assignment clause makes no material claim (it would change the
   damage). Those windows lose the wave-63 material ranking.
5. **`trampleOverflow` (already-declared blocks) still uses printed toughness**,
   not remaining — inherited from #W58-B and untouched here. It UNDER-states the
   overflow, i.e. over-states the seat's life, on a pre-damaged blocker.
6. **T9(a) picks no specific body.** The give-back is the cover delta of losing ONE
   body under the capacity model (bodies are interchangeable there), so it is
   correct for that model and does not name which creature the seat will sacrifice.
   A board where the candidate bodies differ in block legality is not modelled.
7. **T9(b) kill test is toughness-only**: indestructible is excluded, protection /
   prevention / damage-replacement are not. It can therefore over-claim a punisher
   kill on a protected attacker. The `ATTACK TOTAL` positive kill claim is still
   gated off whenever a punisher is named, so no lethality claim rides it.
8. **T7 scan breadth.** The predicate matches `untap` + `all(`/`foreach(` +
   `creature` on one script line. Untap effects that name a single target, or that
   reach the seat's bodies through a granted ability, are missed (fails closed).
   It does not check whether the untap's own trigger can actually fire this turn —
   the clause says "can be standing again", not "will be".
9. **T16(a) scope.** `stackPendingDrawsFor` counts the undeclinable draws the stack
   will hand the seat; a draw that is itself conditional on a resolution the seat
   can still change is counted. The punisher rate is read from the battlefield as
   it stands.
10. **The two new suite fixtures prove the RULES claim, not the render.** They
    show the engine leaves the seat at 15 where the header swore 14 was the
    ceiling. They do not exercise `incomingCombatLine`'s caller.
