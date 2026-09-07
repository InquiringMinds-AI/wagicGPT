# Wave 71 - lane BR (card/render truth: L6, L7, L8, L11, L13-L18, L19)

Base `77d82bbe6`. Gates on the merged lane tree: suite **1279 tests, 0 failed / 68 AI tests,
0 failed** (`WAGIC_TESTSUITE_THREADS=1`, both failure strings counted, 0 timeouts);
PARSETEST **5361 passed, 0 failed** (base 5302 -> +59 cases). `git diff | grep -c U+FFFD` = 0.
Files: `src/AIPlayerGPT.cpp`, `src/AllAbilities.cpp`, `src/ActionStack.cpp`,
`include/AllAbilities.h`, `bin/Res/test/tribute_to_hunger_double_w71.txt` (+ `_tests.txt`).

---

## L6 Tribute to Hunger double-pays - CONFIRMED, fixed at the ENGINE layer

**Oracle first (per the standing memory).** Gatherer/Scryfall: `{2}{B}` Instant, "Target opponent
sacrifices a creature of their choice. You gain life equal to that creature's toughness."
`mtg.txt:124073`'s `mana=` and `text=` are byte-faithful to it; the card script is INNOCENT.

**Mechanism, read off the corpus (not theorised).** `126v125` seq 131-134 prompts show the whole
shape: the SPELL resolves and goes to the graveyard at once, the victim is chosen THEN
("You used: Gain life equal to its toughness with Tribute to Hunger targeting Emrakul"), and a
separate stack object - "ability: Tribute to Hunger's gain life equal to its toughness (aimed at
Emrakul)" - carries the lifegain AND the sacrifice to a later resolution. Cast a second copy in
that window and seq 133 shows TWO such abilities stacked, both aimed at the one Emrakul. Each pays
the full toughness off its stored pointer; only one sacrifice happens. `125v126` seq 120: opponent
10 -> 25 -> 40 and ~15 further turns; `126v125` seq 130 -> 132: 10 -> 25 -> 38.
The RENDER was already honest at both seats ("an effect already on the stack is aimed at that same
creature; if it is gone when this resolves ... this does nothing", plus a `noop_row_zero_reask`) -
this is purely a resolution defect.

**RED-on-base evidence.** `bin/Res/test/tribute_to_hunger_double_w71.txt` (registered): two copies,
one Grizzly Bears, `ai` seat. On `77d82bbe6`: `==life problem for player 0. Expected 22, got 24==`.
After: green, with the Bears in the graveyard and both Tributes spent.

**Fix, and the false start that shaped it.** First attempt - "no life if the victim is not in play
at `AADynamic::resolve`" - turned the full suite RED on all seven `condemn/*` fixtures: Condemn is
`auto=bottomoflibrary` + `auto=dynamicability<!mytgt toughnesslifegain targetcontroller!>`, i.e. the
SPELL removes the body itself and then pays for it, which is correct. The separator is WHO is
resolving. `StackAbility::resolve` now stamps `AADynamic::resolvingFromStackAbility` (the nest is
walked the usual way), and `AADynamic::resolve` refuses the payment only when (a) it is resolving as
its own stack object - a granted or triggered ability put there earlier - and (b) the family is
exactly `mytgt toughnesslifegain`, and (c) the victim is no longer on a battlefield. A spell running
its own `auto=` lines is never stamped, so Condemn and Angelic Chorus are untouched (all eight
re-run green beside the new fixture).

**Prediction (falsifiable, next corpus):** zero games in which two edicts of this family credit one
death twice; `feeds`/edict rows that print "if it is gone when this resolves ... this does nothing"
are now TRUE of the outcome as well as the row.

## L7 animating lands invisible to every count - CONFIRMED, fixed at the render
Header, sweeper row and Lightmine's N all counted current creatures (`125v152` seq 35/38: "0 are
creatures ... 10 are lands" over a Lair of the Hydra that had dealt 14). NOT folded into K - a land
that has not animated is not a creature and a sweeper does not kill it; folding it in would be the
second false surface. Stated BESIDE the count, one wording shared by the three surfaces
(`animatableNotCountedTail`), off an activated `becomes(creature ...)` rung
(`scriptHasActivatedAnimate`, the same script shape the crack-back walk reads). With no animator
every one of the three renders byte-identical to before (pinned).
**Prediction:** a control seat facing a creature-land no longer reads `destroys 0` as "their board is
empty"; the header's animatable count is non-zero in every window that lists one.

## L8 `{feeds:}`/`{reserve:}` against a hand a pending trigger is about to bottom - CONFIRMED
Nothing is folded, because nothing about the replacing hand is knowable (Puzzle Box bottoms the hand
and redraws). One line under the stack block that causes it, naming the source and the counts it
voids (`stackHandReplacerFor` + `handAboutToBeReplacedLine`). It claims no number.
**Prediction:** `162v152` seq 21's shape (a `{reserve: ... STRANDS <punisher>}` verdict taken while
its own Puzzle Box trigger is on the stack) does not recur without the seat having been told.

## L11 `YOUR PLAN:` transcribed as the reply label - CONFIRMED, fixed at the render
The heading was `YOUR PLAN (...): <sentence>` - byte for byte the protocol's own label shape, one
line above the menu. It is now a QUOTATION: `YOUR PLAN (as you last stated it, 1 window ago on turn
10) reads "..."`. The words "YOUR PLAN" are kept exactly (three LIVE deck guides key on that
string - 125, 130, 146). **The parser was not touched and still accepts no second label.**
On the 92 unlabelled plan lines (class A): the render half is the same one - the most recent thing
the model saw called "the plan" was a bare sentence after a colon, and 20 of the 92 open with
"then"/"Continue", i.e. a CARRY. The remaining half lives in the reply-protocol text, which is an
owner-ruled surface (invariant 000c): REPORTED, not changed.
**Prediction:** `YOUR PLAN:` openings go to 0; class A falls but does not vanish.

## L13 modal spell narrated as resolved before its mode ask - CONFIRMED, fixed at the render
`146v130` seq 20. Both statements are true of the engine and read as a contradiction to a reader
instructed to believe the screen. The mode-ask header now states the order
(`modalResolutionOrderNote`), scoped to the seam where the subject is a payload the game can no
longer point at (`ctx == NULL`). It never tells the reader to distrust the log.
**Prediction:** no reasoning trace spends itself on "the log says it already resolved?".

## L14 the "asked at its own window" promise, and the rung ceiling - CONFIRMED, fixed at the render
(a) The dead-cast-menu scope sentence now ends "Those windows offer a row only where the cost can
actually be paid, so this sentence is not a promise that any particular ability of yours is
affordable this turn." (b) The rung ceiling rode the OPTION ROW, i.e. it printed only once the
animation was already affordable - the model could never read it in the window where it would change
the plan (`146v126` s20, `146v125` s44/s45). A creature-land now carries it on the PERMANENT, at
every window (`manlandBoardTag`); it states the RULE, never a rung or an affordability.
**Prediction:** no animation plan written at a window whose sources cannot pay for it.

## L15 Tomb of Annihilation modal rows with no annotation - CONFIRMED, two causes
(i) The seam matched row labels against `ctx->magicText` only, and a dungeon's rooms live in
`autocommandzone=`, i.e. in `magicTexts` - so `162v146` seq 21 rendered "1. lose 2 life / 2.
sacrifice cards" bare while the same seat's seq 16 (an `auto=` card) priced the identical mode. The
menu script is now every zone bucket joined (`scriptAllZones`), the rail the cast-mode menu already
used. (ii) A mode that prices NOTHING rendered SILENT, and the model filled the gap ("Sacrificing
cards costs life. I'd die." - Sandfall Cell sacrifices an artifact, a creature and a land, and costs
no life). The negative is what the script implies and is now stated, but only when it is exact: a
segment naming `life:`/`draw:` at all - including inside a granted payload this tag deliberately
drops - still returns nothing rather than being declared free. Three wave-65 pins that asserted
SILENCE on grant-only rows were rewritten to assert the negative, with new negatives proving no
magnitude and no life total is claimed.
**ALSO FOUND, not fixed:** a `dungeonRoomHeader` path exists (N-146n) and did NOT fire for this
window - the header named the dungeon, not the room. Docketed for the wave.
**Prediction:** zero bare mode rows; no invented mode content in reasoning.

## L16 false `[DRAW PRICE]` on a cast row - CONFIRMED, fixed
`130v162` seq 17 priced Pyrite Spellbomb's cast at "this draws 1 card ... you would be at 15".
`scriptSelfDrawCount` counted `auto={1}{S}:draw:1` - an ACTIVATED line - as a cast-time draw.
It now skips a line whose payload sits behind a cost, detected depth-aware
(`lineIsActivatedCost`: the first depth-0 colon, preceded by a top-level `{...}` group), so
`foreach(creature[attacking]):draw:1` and `life:x && draw:x` still count and
`this(variable{type:*:myhand}=0) {3}{T}:draw:3` does not.
**Prediction:** no `[DRAW PRICE]` on a cast row for a card that draws only from an activation; the
live deck130 guide's hand-written counter-rule becomes dead weight (a guide item for its reviewer).

## L17 hold row on cast menus, and the repeat-row lag - CONFIRMED, both fixed
(a) The row is NOT removed (withholding a legal decline is the wrong lever, and the latch is what
ends the identical-run loops). On a CASTING menu it now says what it costs there - "on THIS menu
that means you also give up this turn's remaining CASTING windows for as long as these rows stand" -
replacing "you give up no cast", which is exactly the claim that is false there
(`hold_windows_skipped_cast` 343/76/60/44 vs priority 30/0/21/19; `123v146` seq 17 took it over a
live Idyllic Tutor). Both spellings share a HEAD, so `holdRowIndexOf` and `isReservedHoldEcho` bind
either (pinned). The priority seam's row is byte-identical to before.
(b) `kRepeatRowFloor` was 2 while `kRepeatActivationFloor` is 1, so `123v130` seq 18 printed
"activated this turn 1 times already. This turn will not advance while you keep taking this option."
on a menu with no batch row - five windows (~9 min) of one-at-a-time taps before seq 23/24 offered
it. The floor is now literally `kRepeatActivationFloor`: the window that states the fact offers the
row. (The old comment already claimed they were the same evidence.)
**Prediction:** `hold_windows_skipped_cast` falls sharply; a token loop costs ~2 decisions, not 7.

## L18 crack-back floor / decline-vs-hold / two parser refusals - split verdict
- **(a) CONFIRMED, fixed.** The head printed a life total and the FLOOR clause printed NAMES CARRYING
  NUMBERS that nothing added up; `125v152` seq 38's reasoning produced "2 + 1 + 6 = 9. I would be at
  3" against an actual 14, and lost the game. The line now does the addition the engine owns both
  halves of ("ADD THOSE UP: ... you would be at N"), keeps the FLOOR warning, follows the kill
  verdict to the summed number, and refuses to call the sum complete while anything listed has no
  number ("and lower still - something listed above adds power this render cannot size"). With
  nothing sizeable the line is byte-identical to before.
- **(b) PARTLY ADDRESSED by L17(a).** The two decline rows were near-synonyms on cast menus (15 of 75
  misses); the hold row's cast-seam text now differs from "Cast nothing right now" in substance, not
  only in an annotation on the other row. Whether that closes the 20% is the next corpus's question.
- **(c) NOT DONE, deliberately - REPORT.** deck125 E-5 asks for two parser widenings and I ship
  neither. The bare-name half (`CHOICE: Cast Dream Fracture`, seq 93) would REVERT wave 70: lane BL's
  T5-T8 rewrite deliberately made the numberless form unparsed and rewrote every ask to demand a row
  number. The label half (`HOLD PRIORITY: 2 (Hold priority)`, seq 110) has a RENDER cause worth
  recording: the hold row's own head is `Hold priority: ...`, i.e. label-shaped, and this is the same
  failure as L11 one row lower - the model copied a rendered heading into its label slot. The fix is
  to de-colon that head; I did not take it this late in the lane because the string is compared in
  four places (`holdRowIndexOf`, `isReservedHoldEcho`, two `compare(0, 14, "Hold priority:")` sites at
  `AIPlayerGPT.cpp:~26548` and `~32246`) and I had already changed that row once this wave. Docketed.

## L19 the F1 diagnostic on disjoint tap groups - CONFIRMED, fixed
29 false fires over two games. One literal is not always one fact: `{paying this taps: ` names a tap
GROUP and one activation legitimately has two disjoint ones. The duplicate test for that literal is
now over the clause BODY (`repeatedClauseBody`); the wave-69 merge shape (the SAME clause twice)
still fires. The other four literals keep the strict count.

---

## Weakest evidence
1. **The L6 scope.** The stamp separates "resolving as a stack object" from "a spell running its own
   `auto=` lines", which is the separator the two card families actually differ on - but it is a
   PROXY for the rules question (did the sacrifice happen?). A granted edict whose victim is still in
   play but which cannot be sacrificed for some other reason would still pay. The fix also
   under-delivers where a second creature exists: real Magic sacrifices THAT one and pays its
   toughness; this pays 0. That is strictly better than paying for a body that never died, and it is
   the shape the corpus produced, but it is not complete rules coverage. The complete fix is to make
   the victim choice happen at RESOLUTION rather than when the ability is stacked - a much larger
   change, and one I did not attempt.
2. **The fixture reaches the defect through the `ai` command**, not through explicit scripted casts -
   the two copies are cast by the AI seat. It is deterministic and it was RED on base for exactly the
   right reason (life 24 vs 22, one death), but it pins the OUTCOME, not the stacking order.
3. **Every render item is unvalidated against a model.** All eleven are pinned as strings; none has
   been read by the pilot. The L15 negative in particular adds a clause to EVERY unpriced mode row of
   every modal card, and its value ("the model stops inventing") is a prediction, not a measurement.
4. **L7's animate predicate is script-shaped** (`becomes(creature` on a line with a cost head). A
   creature-land written some other way is invisible to it, exactly as it is to the crack-back walk
   this borrows from - so the header's count is a floor, not a census. It never over-claims.
5. **L11's second half is unaddressed** by anything I shipped: if the 92 unlabelled plan lines are
   driven by the protocol text rather than by the heading, the count will barely move.
