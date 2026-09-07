# Wave-72 lane BW — render truth B + the Puzzle Box (M3, M11, M15 report, M19, M20, M21, M23)

Base `0e4e6b9b8`. Worktree `worktrees/lanes/w72-BW`, branch `w72-lane-BW`.
Gates on the merged lane tree: suite **1281 tests / 0 failed, 73 AI / 0 failed**
(THREADS=1, both failure strings counted, 0 and 0); PARSETEST **5361 / 0**
(base 5330, **+31**); both link guards OK; `git diff | grep -c U+FFFD` = **0**.
Corpus read for the two measurements: `matchups-20260907-085638` (2,159 replies).

---

## M3 HIGH — the hand-replacement warning — CONFIRMED, ROOT-CAUSED, FIXED, PINNED

**The wave-71 diagnosis was one layer too deep.** Engine-seat HIGH-3 and lane-BS F7
both reasoned about `abilityEmptiesHandOf`'s payload walk. The walk was never
reached: `stackHandReplacerFor` took its source card from
`dynamic_cast<MTGCardInstance *>(it->source)` — `Interruptible::source` — and
**`StackAbility` never assigns it**. The base ctor
(`ActionStack.h:44`, `source(NULL)`) leaves it NULL and nothing on the push path
(`ActionStack::addAbility` → `StackAbility::StackAbility`, `ActionStack.cpp:1012`,
`:567`) fills it in. So for EVERY triggered and activated ability the scan hit
`if (!src) continue;` before the shape gate and before the payload walk, and
returned `""` unconditionally — which is exactly the FAIL at `123v162` seq 43-45.
`StackAbility::getDisplayName` reads `ability->source` for this same reason, which
is why the stack block above the missing line could name the Puzzle Box correctly
while the line below it could not.

**Fix** (`AIPlayerGPT.cpp` `stackHandReplacerFor`): the `StackAbility` cast moves
FIRST and `src` falls back to `sa->ability->source`. Second, smaller fix in
`abilityEmptiesHandOf`: `TargetZoneChooser::targetsZone(zone, source)` **assigns**
`tc->source` (`TargetChooser.cpp:2181`), so reading a live chooser mutated it —
the value is saved and restored around the call.

**RED on base, GREEN after.** New suite command `asserthandreplacer <expected>
<host card>` + fixture `bin/Res/test/w72bw_puzzle_box_hand_replacer.txt`, with a
Teferi's Puzzle Box **on each side** as the brief asks. It stages the host's
registered `GenericTriggeredAbility`s one at a time (the object `fireAbility`
pushes), asks the new door `gptStackHandReplacerFor`, and retires each staged
object `RESOLVED_NOK` so nothing draws or bottoms. The Box carries both halves
(`mtg.txt:118333-4`, `all(*|myhand)` and `all(*|opponenthand)`), so of the four
registered triggers exactly TWO empty any given seat's hand. The assertion is
discriminating in three directions: **0** = the scan reaches no payload,
**4** = the whole script is being read instead of the payload, **2** = correct.

    base 0e4e6b9b8:  staged 4 trigger(s), expected 2 ... got 0   ==Test Failed !==
    after:           1 test (0 failed)

**Prediction (falsifiable, next corpus):** every window whose stack carries an
unresolved Puzzle Box / hand-bottoming trigger renders `EVERY COUNT TAKEN FROM
YOUR HAND ON THIS SCREEN IS ABOUT TO BE VOID`; the `123v162` seq 43-45 shape
(stack block naming the Box, no line under it) does not recur. Because the NULL
source was unconditional, the same line should now be reachable for any other
hand-replacing stack ability, not only the Box.

## M11 HIGH — the repeatable row against the life trend — FIXED, PINNED

`130v125` seq 138 printed `Opponent life trend: ... now 44 (+15 since turn 50)` and
`- and 3 to the opponent at life 44 leaves them at 41` on the same screen for 25
consecutive turns. New pure emitter `oppLifeRaceClause(dmg, gain, turns)`, appended
inside `castPlayerDamageTail`'s non-lethal branch, fed by a new member
`AIPlayerGPT::oppLifeRise` reading the SAME samples and the same turn labels the
`Opponent life trend:` line is built from — so the row and the frame cannot
disagree. Threaded through the two live call sites (`castKillVerdictNow` gains two
defaulted ints; `FindCardToPlay`'s site reads the member directly).

    - and 3 to the opponent at life 44 leaves them at 41 - and their life has RISEN
    15 over the last 4 turns (+3.75 per turn), so 3 a turn does not close that gap

Deliberately narrow: it does **not** call the row repeatable (the engine does not
know that here), it does not tell the pilot to decline, it is silent unless the
rise is measured over ≥1 turn AND at least as fast as this row's damage, and a
LETHAL row returns on the WINS THE GAME branch before it. An untrended row is
byte-identical to wave 71. 6 pins (repro, three rate formats, two negatives,
must-not-match, echo).

**Prediction:** on any seat running a repeatable damage engine against a rising
life total, the clause renders on the row and the consecutive-take run shortens;
zero renders on a falling or flat trend, and zero on a lethal row.

## M15 MED — REPORT ONLY (no protocol change, no parser change)

Corpus `matchups-20260907-085638`, 2,159 replies, `plan_line_missing` **90 (4.2%)**.

**What precedes an unlabelled plan line: nothing, and the instruction is not the
variable.** 90 of 90 replies open with the identical `\n\n` — byte-identical to
the 2,069 labelled ones, so no reply-side prefix distinguishes them. **89 of the
90 sit under an instruction whose FINAL LINE begins `Write your PLAN: line
first`**; the one exception is the only instruction form in the corpus that omits
the label entirely — the `[RE-ASK]` line — which drops it 1 of 4. deck152's
hypothesis (*9 of 14 at seams whose instruction caps the ACTION label only*) is
**REFUTED**: every seam names the PLAN label first, in every window.

Shape of the 90: **67** an unlabelled plan SENTENCE (the label alone is missing),
**15** the wrong label `YOUR PLAN:` (the render heading copied into the label
slot — the L11 shape, still alive after the wave-71 render fix), **7** no plan
line at all, **1** another label.

Per seam (missing / windows):

| seam | rate |
|---|---|
| `discard` cleanup_discard | 7/50 (14.0%) |
| `attackers` combat | 11/81 (13.6%) |
| `reveal` target_or_reveal | 1/12 (8.3%) |
| `blockers` combat | 2/30 (6.7%) |
| `priority` unclassified | 3/63 (4.8%) |
| `ask` unclassified | 51/1256 (4.1%) |
| `ask` land_drop | 8/350 (2.3%) |
| `priority` empty_stack_pass | 7/313 (2.2%) |

Per instruction FORM the ordering is the same and sharper — the rate tracks the
ACTION clause's own complexity, not the PLAN clause (which is constant):
attackers-with-the-planeswalker-variant **4/12 (33%)**, `[RE-ASK]` **1/4 (25%)**,
one-card PUT with the "or exactly X to choose no card" branch **1/4**, N-card
discard **4/24 (17%)**, plain attackers **7/69 (10%)**, blocks **2/30**, the
ordinary CHOICE menu **47/1380 (3.4%)**, the priority menu **10/376 (2.7%)**, and
the four shortest single-clause PUT/CHOICE forms **0 of 25 combined**.

Reading, offered as measurement not as a proposal: the label is dropped where the
ACTION half of the same sentence is longest and most branched. Both surfaces the
lane could act on (the protocol text; parser tolerance) are owner-ruled under
invariant 000 and neither was touched. The 15 `YOUR PLAN:` records are a RENDER
question and are the one part of this that a future lane can act on without the
owner.

## M19 MED — loop-half salience — FIXED, PINNED

Two changes in `loopPendingSituationLine`:
1. **A completed pair now has a header.** `(!conv.empty() && !mir.empty())` used to
   `continue` — the state that wins the game announced itself with nothing but a
   `LIFE-TO-DAMAGE CONVERTER` line naming ONE half (deck123 seq 21). New pure
   emitter `closedLoopHeaderText(converter, mirror, theirs)` states the closure and
   the two entries into the chain, per side.
2. **An unclosable pair is silent.** `!halfCanReturn` (the half is in exile, or in
   a graveyard with no recursion on that board) now `continue`s instead of printing
   the five-line block — 158 renders in one `vs125` game, at full length, in a game
   where it could never close. The half that IS in play keeps its own
   converter/mirror lines in the board frame, so nothing about the live board goes
   unsaid, and the absence of a header no longer reads as "no loop" now that the
   winning state has one.

**Residual, flagged not fixed:** `pendingLoopWarningText`'s `!halfCanReturn` branch
is now unreachable from the live caller and survives only in its PARSETEST pins. It
is a deletion candidate for a revert lane; I did not delete it in a seven-item lane.

**Prediction:** `LOOP HALF PENDING` render count drops to the closable cases only;
at least one `LOOP COMPLETE:` render appears in any game where a seat assembles
both halves, and the deck126 guide's "NO HEADER ANNOUNCES A COMPLETE PAIR" line
becomes false (guide-owner's call, not edited here).

## M20 MED — the animatable tail steered a decision wrong — FIXED, PINNED

`125v152` seq 64, at 2 life: `PLAN: Cast Supreme Verdict to remove the impending
Lair of the Hydra threat` over a row whose own verdict read `destroys 0 ... 0 of
yours`. The wave-71 tail said what the count does NOT cover and stopped there.
Two additions, both facts about the board and neither advice about the row:

- `animatableNotCountedTail` gains the TENSE: `- it is not a creature as the board
  stands, so nothing that only affects creatures reaches it right now` (plural
  agreeing). Shared by the header, the sweeper row and the Lightmine row, so the
  three surfaces still cannot disagree.
- `manlandBoardTag` gains the animation COST, read off the same script line the tag
  is derived from (new `scriptActivatedAnimateCost`): `[the animation costs
  {2}{g}{g}, and until it is paid this permanent is NOT a creature - it cannot
  attack or block and nothing that only affects creatures reaches it]`. Empty cost
  keeps every wave-71 string byte-identical.

Two wave-71 `#W71-BR L7` pins assert the old wording and were rewritten to the new
one (renamed `#W71-BR L7 + #W72-BW M20`) — they are the same assertions, extended.

**Prediction:** zero replies that justify a sweeper by an un-animated land; the
cost bracket renders on every creature-land on either board.

## M21 MED — declined-count windows, 26% mis-answered — FIXED (render), PINNED

deck125 A-4: 65 declined-count windows, `Cast nothing right now` in 17 (26%),
unchanged across two waves of guide capitals. Cause, at the render: the two decline
rows are not equivalent and **only one of them says so** — the hold row carries
`holdRowBenefitClause` ("this same question is not put to you again…") and the
plain decline carries nothing, so a pilot reading the pair top-down has no printed
difference to price. New `plainDeclineScopeClause` / `appendPlainDeclineScope`,
applied at both seams right after `appendStackDeathToDeclineRows`:

    {closes ONLY this window - the same list can be put to you again this turn, at
    this seam or another; the hold row is the row that closes the run}

Three deliberate constraints: **no row is removed** (the plain decline stays a legal
answer, invariant); the clause is **unconditional wherever a hold row exists** — one
that appeared only once the declined count rose would change the row text mid-turn
and mint a fresh `askKey`, the exact trap `declinedListNote` was built to avoid; and
it carries **no number** for the same reason. It is a `{...}` clause, so
`optionSetKeyLine` strips it and the declined count still reaches the same list
(pinned), and it is registered in `stripNarrationDecoration` so it never enters
history (pinned).

**Prediction:** the plain-decline share of declined-count windows falls below 26%
with no fall in total declines (the hold should absorb them), and
`hold_windows_skipped` rises.

## M23 LOW — three parts

**(a) Silverquill life tax — clause added.** The review's own verdict on the render
was "exemplary"; the gap is that every ORDERING surface the pilot has (the mana
line, `{leaves N of your M untapped mana sources untapped}`, the row's cost head)
is denominated in MANA, so a cross-row comparison cannot see a 3-life-per-cast tax
at all (`123v146` seq 86, at 7 life). The P-list itself is deck123's GUIDE and is
the deck reviewer's property — not edited. `namedCastPriceTag` now names the
currency it is NOT in, only when life is actually charged: `This is NOT part of the
mana cost and is not counted in the mana line or in any "leaves N untapped" clause
on this menu: paying it is a LIFE payment on top of the mana, at every cast`.
Draw-only naming permanents make no such claim (pinned negative).

**(b) `HOLD PRIORITY:` row head — de-coloned (the BR L18c docket).** `125v152` seq
110 answered `HOLD PRIORITY: 2 (Hold priority)` and was refused: the row's own head
was label-shaped and the model copied a rendered heading into its label slot — the
L11 shape one row lower. `Hold priority:` → `Hold priority -` on all three
spellings (`kHoldPriorityRowHead`, `kHoldPriorityRowText`,
`kHoldPriorityRowTextCast`); the two head comparisons that spelled the colon out
(`appendStackDeathToDeclineRows`, the decline-row scan at the cast seam) are cut
back to the colon-free 13-char prefix; `holdRowIndexOf` and `isReservedHoldEcho`
bind through the shared head and are untouched. The short-name echo
`CHOICE: 2 (Hold priority)` still binds the row (pinned). Three deck guides quoted
the old literal verbatim (`deck125:208`, `deck130:248`, `deck146:182`) and would
have become false surfaces; the QUOTED STRING ONLY was updated in each — no guide
content was authored or re-edited.

**(c) Dungeon room header — REFUTED on the corpus.** Engine-seat recorded it as
"not visible in any prompt". It is visible: **20 prompts carry `DUNGEON ROOM
CHOICE - you are venturing in <dungeon>` with the full printed room list**, and
20 of 20 room-branch windows have it (e.g.
`1788789405-ai_baka_deck146-...-vs-ai_baka_deck126.jsonl` seq 15, Lost Mine of
Phandelver, all 7 rooms listed). The 3,312/266 counts that looked like a gap are
narration echoes of the chosen option text in LATER prompts, not room-branch
windows; a census on the `options_text` field returns 0 because those rows are not
in that field. No change made. **One real residual measured:** the header's venture-
step sentence renders **0 of 20 times** — `explores` reads the dungeon's Explore
counter and it is 0 at every room-branch window, so `This is venture step N of your
current run` never prints. Docketed rather than fixed: I have no repro that
separates "the counter lands after the room choice" from "the counter is named
something else", and a fix without a RED fixture is exactly what this lane's rules
forbid.

**(d) Idyllic Tutor one-window lag** — carried, not in this lane's item list, still
UNTESTED.

---

## Files

- `projects/mtg/src/AIPlayerGPT.cpp` — M3 source fix + `gptStackHandReplacerFor`
  door; M11 `oppLifeRaceClause` + `oppLifeRise` + `castPlayerDamageTail` /
  `castKillVerdictNow` threading; M19 `closedLoopHeaderText` + the two
  `loopPendingSituationLine` branches; M20 `animatableNotCountedTail` +
  `scriptActivatedAnimateCost` + `manlandBoardTag`; M21 `plainDeclineScopeClause` /
  `appendPlainDeclineScope` + the `stripNarrationDecoration` entry; M23a the
  `namedCastPriceTag` clause; M23b the three hold-row constants and two head
  comparisons; the `[#W72-BW]` PARSETEST block (+31 cases, at the corpus end).
- `projects/mtg/include/AIPlayerGPT.h` — the door declaration and `oppLifeRise`.
- `projects/mtg/src/TestSuiteAI.cpp` — the `asserthandreplacer` command.
- `projects/mtg/bin/Res/test/w72bw_puzzle_box_hand_replacer.txt` + `_tests.txt`.
- `projects/mtg/bin/Res/ai/baka/deck{125,130,146}_strategy.txt` — the quoted hold-row
  literal only.

## Weakest evidence, in order

1. **Every render item (M11, M19, M20, M21, M23a) is pinned as a STRING and has
   never been read by the pilot.** Their value is a prediction, not a measurement —
   the next corpus's seat review is the gate. M21 in particular changes a row that
   appears on nearly every window; if the clause is read as a reason to take the
   HOLD where a live row exists, it will show up as a rise in dead holds, and the
   deck123 guide's "THE HOLD ROW IS A P8 ANSWER AND NOTHING ELSE" is the rule it
   would be fighting.
2. **M3's fixture pins the PREDICATE, not the live path.** It stages the trigger
   rather than waiting for one to fire in a real draw step, so it proves that the
   scan now reaches the payload and reads it relative to its source; it does not
   prove that the call site at `AIPlayerGPT.cpp:~24300` runs in the window the
   corpus failed in. The NULL-source root cause is certain (it is a missing
   assignment, provable by reading two constructors) and the RED/GREEN is exactly
   that fault, but "the line will now appear at seq 43-45's shape" is inference.
3. **M11's trend inputs are only ever 3 samples** (`mOppLifeByTurn[3]`), taken at
   this seat's first look each turn, so the rate is over at most the last three
   sampled turns and a turn with no window of ours is absent rather than
   interpolated. `+3.75 per turn` in the fixture is arithmetic over that window,
   not over the game.
4. **M19's suppression is a deletion of text.** The trust doctrine says silent
   omissions are worse than wrong text; the argument that this one is safe rests on
   the broken pair changing no decision and on the completed pair now having its
   own header. If a future review finds a seat reasoning about a pair whose half is
   in exile, this is where it came from.
5. **M23c is a refutation from the corpus, not from the code.** I did not re-read
   the header path to prove it always fires — I counted renders. A room-branch
   window that produced no header would not be visible to that count if it also
   produced no record.
