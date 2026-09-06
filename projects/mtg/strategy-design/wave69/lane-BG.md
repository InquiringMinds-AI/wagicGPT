# Wave-69 lane BG — K1, K5, K8

Base: master `169ee41f3` (worktree `worktrees/lanes/w69-BG`, branch `w69-lane-BG`).
Evidence: the wave-68 reviews cited in `known-bugs.md`, the corpus
`matchups-20260906-134120` (read-only), the primitives, and the fixture / PARSETEST
instruments below. **No live model probe and no corpus run.**

---

## K1 — the game that ended with no loss condition met. ROOT CAUSE.

**The loss check that fired: `MTGPlayerCards::drawFromLibrary`'s empty-library branch
(`src/MTGGameZones.cpp:398`, `setLoser(library->owner)`).** Not `DeadLifeState` (it needs
life <= 0 or 10 poison; the seats were at 19 and 11), not `AAWinGame`, not the commander
21-damage setter, not the network concede, and not a `CANTLOSE` inversion from BE R7 — R7
changed only the RENDERED countdown sentence, it never touched this function.

**Why it fired on a library of 42 cards.** A library reveal does not read the library: it
**moves** it. `MTGRevealingCards::Update` (`src/AllAbilities.cpp:348-361`) loops
`playerForZone->game->putInZone(toMove, RevealFromZone, RevealZone)` over `nbCard` cards,
and Idyllic Tutor's count expression is `type:*:mylibrary` — the WHOLE library. So at
stderr:1929 (`reveal put 1 of 42 to option one in one reply`) all 42 of deck123's cards
left `library` and entered `p->game->reveal`. Option one's payload — the ability that puts
the other 41 back (`transforms((,newability[all(other *|reveal) moveto(mylibrary)
and!(shuffle)!],newability[moveto(hand)]))`) — went on the stack as
`Action added to stack: StackAbility. (Source: Idyllic Tutor)` (stderr:1930) and **has no
matching `Resolving Action on stack: StackAbility` anywhere in the file**. The cards stayed
parked. Twenty-two lines later deck146 cast Vision Skeins (`all(player) draw:2`,
mtg.txt:129547); its two `10DrawAction`s called `drawFromLibrary` on a zone that read 0,
and the empty-library branch declared deck123 the loser:
`WAGIC_SELFPLAY_RESULT winner=0 life0=19 life1=11 turn=17`.

By the rules those 42 cards never left the library — revealing does not change a card's
zone — so the draw was owed one of them and there was no empty library to lose to.

**Fix (general form), `src/MTGGameZones.cpp`.** Before the empty-library branch,
`drawFromLibrary` looks for a card parked in **this player's own `reveal` zone whose
`previousZone` is this library** (`MTGGameZone::removeCard` stamps `previousZone` on every
move, so that predicate is exact) and, if it finds one, returns it with the same
`putInZone` call the reveal's own force-close and option-two paths use, then draws
normally. The reveal takes the library's TOP first, so the first parked card in zone order
is the card this draw is owed. Nothing else changes: a genuinely empty library still decks
out, the three `CANTLOSE`/`CANTMILLLOSE`/`CANTWIN` exceptions are untouched, and a reveal
of a DIFFERENT zone (`revealzone(myhand)`) leaves `previousZone == hand` and still decks
out. `CardDisplay::Update` re-inits a display whose zone changed, so an open display
cannot be stranded by the return.

**Fixture: `bin/Res/test/w69bg_reveal_parked_library_no_deckout.txt`** (registered in
`_tests.txt`). New test-only primitive `Lexicon Whole Library Deadend Peek`
(`test/lexicon/test_primitives.txt`, id 2100000988) is Idyllic Tutor's whole-library reveal
with option one predicate-gated to a land (the fixture's library holds none) and no option
two, so no chooser ever arms and all three cards stay parked — the same parked-library
state, reached by a route a script can hold open. New driver command
**`drawcard <1|2> <count>`** (`src/TestSuiteAI.cpp`) calls `MTGPlayerCards::drawFromLibrary`
directly, the exact call `10DrawAction` and the draw step make; no scripted click can reach
it while a reveal display is open, because an open display holds every phase advance (the
same justification lane AM gave `aideclareattack`). The command is registered in all three
of the harness's non-click allowlists.

**RED on base, measured on this tree**: `src/MTGGameZones.cpp` reverted to `169ee41f3`,
everything else in place, clean rebuild, fixture run alone via `WAGIC_TESTSUITE_FILE`:
**1 of 1 failed** —
`==Card number not the same in player 0's hand==, expected 1, got 0` +
`==Card ID not the same. Didn't find 205`, with the log reading
`TESTSUITE drawcard: player 1 draws 1 (library 0, reveal 3)` and then `leaveduelphase`
(the duel ended on the spot). GREEN after the fix, same fixture, same binary otherwise:
`==Test Successful !==`, `gameover:none`, Hill Giant (the library's top, revealed first)
in hand.

---

## K5 — rows that do nothing are offered and taken

**(a) the tapped animation is priced, not only restricted** (`src/AIPlayerGPT.cpp`).
`[this land is TAPPED: animated, it still cannot attack this turn]` states the restriction
and stops; every OTHER row on that menu carries a `{right now: ...}` verdict, so the
animation was the only unpriced row and read as the live one — taken 2/2 (`146: 162 s22` at
1 life, `125 s243`). `describeAction` now appends `{right now: does nothing this turn}` at
the END of the row (so any earlier verdict is already present and cannot be doubled), gated
by the pure `tappedAnimateNeedsVerdict(row)` — the TAPPED bracket present and no
`{right now: ` of the row's own. "does nothing" is the grammar `rowSaysNoOp` folds, so
`verdictReadsZero` reads the row as zero and it joins the all-dead census
(`everyCastRowDead`) instead of being the exception that keeps the census silent. The row,
its cost and the bracket all stay; the activation is still legal and still offered.

**(b) one wording at both seams.** `discardAlreadyControlClause` printed
`{you already control one: X}` and was ignored 4/4 while the cast menu's
`[legendary: you already control ...]` mark was obeyed. Same words now, in the mark shape
that is obeyed: `[you already control one: X]`. **Scope stated honestly:** the
`{dead right now: ...}` verdict was NOT re-shaped — it is one cross-seam verdict literal
that the guides key on and that `discardDisposabilityClass` sorts on, and this lane's
evidence is about the already-control clause only.

**(c) the unpayable activation says so on the row.** `125: 126 s111/113` offered an
activation at `Mana available: 0 total` with only its `[cost: ...]` printed.
`cannotPayNowClause(reach)` adds `{you cannot pay this right now: 0 mana available}` — and
it fires only on the conjunction of three engine facts already computed on that row: the
mana pool cannot afford the cost, `ManaEngine::selectAutoTapProducers` returns no producer,
and `windowReach()` (`ManaEngine::potentialColorReach`) is 0. The row is NOT removed (a cost
can be paid by something this gate does not model, and the doctrine forbids retiring a legal
row); the clause is deliberately not a `{right now: ...}` verdict, so it never enters the
dead census.

---

## K8 — the second-copy tag reads the script

`secondCopyVerdict` already classifies a duplicate off the card's own `auto=` lines, but
verdict 0 (it stacks) printed only the legality fact — `both stay on the battlefield - no
legend rule` — and answered nothing about usefulness. So `125 s42` (a second Sanguine Bond,
`mtg.txt:99004` `@lifeof(player) ... :life:-thatmuch opponent`, which doubles the drain per
life gained) and `125 s35` (a second Staff of Nin, `mtg.txt:112425` `@each my upkeep:draw:1`
+ `{T}:damage:1`, a second draw and a second ping) printed the same neutral head a redundant
copy would print. Verdict 0 now appends `, and this copy is one more of the same effect -
each line it repeats happens again]`. The wave-52 HEAD is byte-identical (PARSETEST pins
that explicitly) — the clause is appended exactly as verdicts 1 and 2 already are.
Second change: an EMPTY `magicText` no longer defaults to verdict 0. With no script to read
the tag states the legality fact only; claiming a second use of an effect this engine never
saw is the wrong-scope lie the trust doctrine forbids.

---

## Gate numbers (worktree, hermetic-enough: `rm -f bin/wagic` + full make before each run)

| gate | base (brief) | this lane |
|---|---|---|
| PARSETEST | 5272 / 0 failed | **5294 / 0 failed** |
| Suite, `WAGIC_TESTSUITE_THREADS=1` | 1277 / 0 failed | **1278 / 0 failed** |
| AI tests | 67 / 0 | **67 / 0** |
| timeouts | 0 | **0** |

PARSETEST +22 net: 25 new cases, 3 old ones replaced (two `#W53-P D11` cases that pinned
the pre-K8 stacking wording and the byte-equality of the empty-script default; one
`#W55-D D9` case that pinned the brace form of the already-control clause). Suite +1 = the
K1 fixture. `git diff | /usr/bin/grep -c $'\357\277\275'` = 0. All edits byte-spliced with
python rb/wb.

---

## Predictions for the wave-69 corpus (falsifiable)

1. **K1**: zero games end with `WAGIC_SELFPLAY_RESULT` while both `life0` and `life1` are
   above 0 and neither seat's last-printed library is 0. Falsifier: any such result line.
   Weaker sibling prediction: on any turn a seat's own reveal parks its library and a draw
   follows, that seat draws a card and the game continues.
2. **K5(a)**: every `becomes ...` row carrying `[this land is TAPPED:` also carries
   `{right now: does nothing this turn}`, and the take rate on those rows falls below 2/2.
   Falsifier: a take of a row printing both clauses.
3. **K5(b)**: `{you already control one:` renders 0 times; `[you already control one:`
   renders instead, and the discard/cleanup takes of a row carrying it fall below 4/4.
4. **K5(c)**: no activation row is offered at `Mana available: 0 total` without
   `{you cannot pay this right now: 0 mana available}` on it.
5. **K8**: every `[second copy:` row whose card's script has a `}:`-cost or countable line
   carries `one more of the same effect`; no row carrying that clause also carries
   `changes nothing`.

---

## What I did NOT verify

- **The corpus game was not replayed to the defect.** The transcript
  `bin/User/transcripts/1788720099-ai_baka_deck146-vs-ai_baka_deck123.txt` IS that game
  (`#result winner=p1 turn=17 life=19/11`), but the replay **diverged at action 69, turn 4**
  — `expected 'p1.zone[16] 1lost mine of phandelver' got 'p2.choice 0' ... chooser=triumphant
  adventurer` — twelve turns before the loss. That divergence is itself an unexplained
  finding and is NOT this lane's fix; the root cause above is from the stderr, the
  primitives and the reveal/draw code, not from a replay reaching turn 17. Nobody has
  reproduced the loss end to end in a real game on either binary.
- **Why the Idyllic Tutor option-one `StackAbility` never resolved is unexplained and
  unfixed.** This lane fixes the loss the stranding CAUSED, not the stranding. If the payload
  still fails to resolve, the seat now keeps a library that reads 0 while its cards sit in
  the reveal zone and draws them back one at a time — no loss, but the printed library count
  is still wrong. That is the `deck123 MED` "Idyllic Tutor never decrements the printed
  library" item, which I did not touch at all.
- **The fixture reaches the parked-library state by a STALLED DISPLAY, the corpus reached it
  by an unresolved payload after the display closed.** Both leave the same zone state, which
  is the state the fix is about; they are not the same route, and no fixture pins the
  corpus's route.
- **No gdb session.** The loss setter was identified by enumerating every `setLoser` call
  site and eliminating each against the recorded state (lives 19/11, no poison, no commander
  damage, not a network game), then confirmed by the fixture's RED/GREEN pair. I did not put
  a symbol breakpoint on `drawFromLibrary` in a live game.
- **No live model probe.** Every K5/K8 change is verified at the pure helper AND at a
  corpus-shaped row through `parseChoice` (the echo case), but no model has read the new
  wordings, and I have no evidence about how the pilot responds to them — the predictions
  above are the test.
- **`{dead right now: ...}` was left alone** (see K5(b)); the brief's "ONE wording at both
  seams" was applied to the already-control clause only.
- **`putInZone(reveal -> library)` fires a zone-change event.** It is the same call the
  reveal's own force-close makes, and the suite is green, but I did not audit which
  `@movedTo(...|mylibrary)` triggers could now fire one draw earlier than before.
- **Guide-keyed literals**: `[this land is TAPPED: ...]` and the wave-52 `[second copy:
  you already control ...; both stay on the battlefield - no legend rule` head are both
  byte-stable (PARSETEST pins the second explicitly). `{you already control one: X}` is NOT
  stable — it became `[you already control one: X]`; any guide keyed on the brace form
  needs re-keying.
