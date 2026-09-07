# Wave-71 lane BS — the fix lane for the Astra step-one review

Worktree `worktrees/lanes/w71-BS`, branch `w71-lane-BS`, base `586a3f69c`.
Every finding was reproduced RED on the base binary (archived at
`~/.gatelogs/w71-BS-base-wagic`, built from 586a3f69c) before it was fixed, or is
recorded below as refuted/limited with the counter-evidence.

Gates on the merged lane tree:
- build: clean link; `check-ctor-init: OK (118 files)`, `check-reply-instructions: OK (1 source, 37 guides)`
- suite (`WAGIC_TESTSUITE_THREADS=1`, memory-capped, detached unit): **1280 tests 0 failed, 73 AI tests 0 failed** (`==Test Failed !==` 0, `==Test timed out` 0)
- PARSETEST: **5330 passed, 0 failed** (base 5314 -> +16 checks)
- `git diff | grep -c $'\357\277\275'` = 0

Three new fixtures, all registered in `Res/test/_tests.txt`:
`proper_burial_death_trigger_w71bs.txt`, `tribute_two_creatures_w71bs.txt`,
`ai_declined_face_reoffered_w71bs.txt`.

RED on 586a3f69c, GREEN on this branch (one run of the three, same binary, same Res):

| fixture | base 586a3f69c | this branch |
|---|---|---|
| proper_burial_death_trigger_w71bs | `life problem for player 0. Expected 22, got 20` | Successful |
| tribute_two_creatures_w71bs | `Expected 23, got 22` + `player 1's graveyard expected 2, got 1` + `player 1's battlefield expected 0, got 1` | Successful |
| ai_declined_face_reoffered_w71bs | `assertdeclinesapplied: expected 3, got 1` | Successful |

---

## F1 HIGH — the stack stamp disabled legitimate death-trigger lifegain — CONFIRMED, FIXED

**Reproduction.** Proper Burial's primitive (`mtg.txt:89137`) is
`auto=@movedTo(creature|graveyard) from(mybattlefield):all(trigger[to]) dynamicability<!mytgt toughnesslifegain targetcontroller!>`
— the same primitive family as Tribute to Hunger, arriving as its own stack object with the
victim in the graveyard by definition. Grim Feast (`50348`) and Death Watch (`27072`) are
identical in shape. Fixture: P1 controls Proper Burial + Grizzly Bears and Doom Blades its own
Bears. Oracle (mtg.wtf/DIS 16): "you gain life equal to that creature's toughness". Base pays
nothing — `Expected 22, got 20`.

**Fix** (`src/AllAbilities.cpp`, `AADynamic::resolve`). The guard is armed only for the EDICT
shape: `dynamic_cast<AASacrificeCard *>(storedAbility)`. `dynamicability<!...!> sacrifice`
parses the trailing words into this ability's own `storedAbility` (`MTGAbility.cpp:3566`), so
"the life is the price of a sacrifice this same object is about to make" is a property of the
payload and is decidable here. A death trigger has no sacrifice in its payload and is never
touched. The lane-BR machinery is kept: `StackAbility::resolve`'s stamp
(`resolvingFromStackAbility`) still distinguishes a stack object from a spell's own `auto=`
lines, so Condemn's seven fixtures are untouched (all green in the suite).

**Prediction for the next corpus.** A game with any of these three enchantments shows the life
total move on each death; `deck` reviews should no longer see "lifegain enchantment did
nothing" narration.

## F2 HIGH — Tribute's second-creature case — CONFIRMED, FIXED (re-target implemented)

**Reproduction.** Oracle (mtg.wtf/FDN 614): "Target opponent sacrifices a creature of their
choice." The choice is made ON RESOLUTION and does not target. The engine picks the victim when
the granted ability goes on the stack, so two copies queued against one board store the same
victim; wave 71 suppressed the second copy entirely. Fixture: P2 controls Grizzly Bears (2) and
Llanowar Elves (1), two Tributes resolve. Rules-correct: both die, P1 gains 3 (life 23). Base:
`Expected 23, got 22`, one creature alive, one card in the graveyard.

**Fix.** Same site. When the stored victim is gone, the payload RE-CHOOSES from that player's
remaining creatures (lowest toughness — the conservative reading of "a creature of their
choice", and the least life this effect can pay) and re-points `target`; `AADynamic::
activateStored` copies `target` into the cloned sacrifice, so the lifegain and the sacrifice
cannot disagree by construction. Suppression survives only for the no-creature-left case,
which keeps the wave-71 fixture `tribute_to_hunger_double_w71.txt` green (life 22).

**One correction to the review's code site.** The re-target CANNOT live at
`ActionStack.cpp:449`/`StackAbility::resolve`: at that moment the granted ability's payload
still carries the parser's placeholder target (traced: "stored victim <empty name>"), and the
real creature is only in `target` at `AADynamic::resolve`. An earlier attempt at the stack site
retargeted the template and produced 24 life; the working fix is at the dynamic's own resolve.

**Prediction.** Two edicts in one turn kill two bodies and pay both toughnesses; a corpus
`{right now: ... is sacrificed, you gain N}` row that resolves second no longer pays 0.

## F3 HIGH — a face-menu decline removed a legal land for the whole turn — CONFIRMED, FIXED

**Reproduction.** `AIPlayerBaka::faceDeclinedThisTurn` keyed the latch on the TURN, and
`DecisionManager::applyMenuChoice` armed it for ANY declined hand-card menu. Fixture: the
decline arm is unbounded and the seat has a Goblin to play; on base exactly ONE decline is ever
applied (`assertdeclinesapplied: expected 3, got 1`) — the card is erased for the rest of the
turn however far the board moves.

**Fix.** Two scopes, both narrowed.
- The latch now carries the serialized BOARD FINGERPRINT it was given in
  (`AIPlayerBaka::declineWindowProbe` — turn, phase, both seats' life and zone counts; no menu
  name and no stack size, because the menu is open when the decline is given and closed when the
  proposer asks again). The refusal holds only while that fingerprint holds. In a livelock
  nothing moves, so the fingerprint is identical and the livelock fix is intact.
- It is armed only when the declined proposal WAS a land drop, proved against
  `LegalActionsOracle::legalLandPlays` — the same legal set the seam filters, so the arm and its
  consumer cannot disagree.

Post-fix the fixture sees three windows (main 1 before the Goblin, main 1 after it, main 2).
The two lane-BP fixtures asserted the old turn-scoped count of 1 and now assert 3; their end
states are unchanged and their DESC blocks record why (both green).

**Prediction.** `declined_face_latches` per game rises; zero HUNG games still holds; a corpus
land-drop window after a board change offers the declined pathway again.

## F4 HIGH — cache refusal fell straight into another cache — CONFIRMED, FIXED

**Reproduction (source-traced, no fixture).** `AIPlayerGPT.cpp` ask cache: the refusal erased
`mAskCache[askKey]` and fell through to `repeatAskAnswerStands(mRepeatAskKey, ...)`, which is
keyed on turn+phase+question+rows — byte-identical in the livelock — and returned the same
answer with no request. Every later tick then missed the first cache and never reached the
refusal check again.

**Fix.** The refusal invalidates BOTH: `mRepeatAskKey.clear(); mRepeatAskTurn = -1;
mRepeatAskSeq = -1` alongside the `mAskCache`/`mAskCacheSeq` erase. A refused window reaches
the model or it is not refused.

**Second half — the watchdog.** `tools/selfplay-harness.sh no_progress_sweep` times both
silence arms off `max(getmtime)` over the seat translogs, so writing `ask_replay` records into
the translog made a spinning seat look alive. The records are kept but now go to a SIDECAR:
`<logdir>/askreplay/<same basename>`. The sweep's glob is non-recursive over LOGDIR, so the
sidecar is invisible to the watchdog and visible to a reviewer, and its own mtime is the spin's
timeline. No harness change was needed.

**Prediction.** A wave-70-shaped spin now trips the fast arm at ~64 MB of stderr; the sidecar
holds the replay run.

## F5 HIGH — fixed answer caps cannot represent large legal declarations — CONFIRMED, FIXED

**Reproduction (arithmetic, pinned in PARSETEST).** 512 legal one-to-one blocks need
`BLOCKS: B1:A1, ... B512:A512` — ~6 KB — and the fitted blockers ceiling is 256 tokens, which
buys ~806 B at the worst-case 3.15 B/token. A cut after a complete pair still parses
(`pairs > 0`), so the surviving prefix executes and the omitted blockers are silently
unassigned.

**Fix.** The answer ceiling is now a function of the legal answer's cardinality.
`gptDeclarationAnswerFloorTokens(items, bytesPerItem)` = `16 B` (label + margin) `+ items *
bytesPerItem + 256 B` (the PLAN line) converted at the worst-case 3.15 B/token and rounded up to
a multiple of 32; `kBlockPairAnswerBytes = 12` (`B512:A512, `), `kPutSlotAnswerBytes = 6`. At 512
pairs: `16 + 6144 + 256 = 6416 B / 3.15 = 2037 -> 2048 tokens`. `gptResolveMaxTokens` takes the
MAX of the fitted ceiling and this floor, so it lowers nothing and it beats even a configured
operator ceiling (a configured 400 may shorten replies; it may not delete a legal answer). It is
an ANSWER floor only — the reasoning budget is still ADDED (invariant 000(d), pinned:
`thinking.reasoning == 6000 && thinking.total == 2048 + 6000`). Armed at all five declaration
seams (blockers, attackers, discard, reveal, bottom) from the board's own counts.

**The truncation re-ask is restored, narrowly.** On `finish_reason=length` at a DECLARATION
seam, the seat buys exactly ONE re-ask: the SAME question with the floor doubled, carried on the
existing retry-tag idiom so the bytes are identical and only the slot key and the allowance
differ. No added text, no correction line — nothing that could license prose (invariant 000).

**Prediction.** Wide-board blockers windows stop producing partial declarations; a
`ceiling re-ask` line appears in stderr only on genuinely truncated declarations.

## F6 MED — deleting the distinct-index re-ask changed an irreversible live path — CONFIRMED, RESTORED

The wave-71 deletion left `PUT: 1, 2, 2` on a discard-of-three falling into the fill loop, which
discards the highest-mana-value unselected card — the finisher the PLAN was written around. Zero
occurrences in one corpus is not evidence that removing an irreversible-path guard is
behaviour-neutral. Restored verbatim in shape: one labelled-line re-ask that states the
arithmetic ("your PUT line named N different cards and this discard needs M"), fired ONCE per ask
text, reading nothing but the `PUT:` line, with `distinct_index_reask` /
`distinct_index_reask_recovered` / `distinct_index_reask_exhausted` stamps back on the record.
The fill loop remains the floor behind it.

## F7 MED — Puzzle Box's warning named the wrong player's hand — CONFIRMED, FIXED

**Reproduction (source-traced).** `stackHandReplacerFor` took the affected hand from
`src->controller() != seat` plus a scan of the source's WHOLE script. Teferi's Puzzle Box
(`mtg.txt:118333-4`) carries both halves — `all(*|myhand)` for its controller's draw step and
`all(*|opponenthand)` for the opponent's — so the controller's hand was announced however the
trigger had actually fired, and the seat whose hand really was being replaced was skipped
because it did not control the source.

**Fix.** The affected hand is derived from the PENDING STACK ABILITY'S PAYLOAD. `all(*|myhand)`
parses to a lord whose TargetChooser carries the hand zone, and
`TargetZoneChooser::targetsZone(zone, source)` resolves `myhand`/`opponenthand` against the
source card — the same relative reading the script has. The walk follows NestedAbility and
MultiAbility (which is what `&&` and `all(...)` build, `MTGAbility.cpp:3197`). The
`controller() != seat` gate is gone; the script read survives only as a SHAPE gate
(bottomoflibrary / moveTo).

**Weakest evidence in this lane** — this one has no fixture and no PARSETEST pin: the predicate
needs a live stack object, and the existing L8 pins are wording-only. It is source-reasoned and
suite-green, nothing more. A probe deck with Puzzle Box on both sides would settle it.

## F8 HIGH — damage modes were declared incapable of changing life — CONFIRMED, FIXED

**Reproduction (pinned).** Jeskai Charm (`mtg.txt:60165`):
`choice name(4 damage) damage:4 target(opponent,planeswalker)` contains neither `life:` nor
`draw:`, so the tag printed "it changes no life total and draws no cards" over a mode that is
lethal to an opponent at four. That is a false surface contradicted by the mode's own primitive.

**Fix.** The negative may be stated only when the mode's segment names NO life-affecting
primitive at all — `life`, `draw:`, `damage`, `drain`, `poison`, `dynamicability`/`dynamic`.
Anything else says nothing, which is what an uncertain forecast owes the model. Pinned positive
(a `moveTo(ownerLibrary)` mode still gets the negative), negative (the damage mode returns "")
and must-not-match (the literal never reaches a damage mode).

## F9 MED — the new PARSETEST pin endorsed rejecting a useful Devour Flesh cast — CONFIRMED, PIN FLIPPED

**Reproduction (pinned).** The wave-71 pin REQUIRED
`{right now: they control 1 creature - Rorix is sacrificed; YOU control 0 creatures - targeting
yourself does nothing}` to read as a no-op, so a live removal spell earned the no-op re-ask
because its self-targeting branch is useless.

**Fix.** `rowSaysNoOp` is branch-aware: the verdict is split at top-level semicolons (outside
parens and quotes) and the row is a no-op only if EVERY OPERATIVE branch is. A conditional branch
("if they gain one before this resolves...") is not operative — the same unit rule
`phraseScopeIsConditional` already applies inside a branch — so it neither rescues nor condemns a
row. The old pin is replaced by the negative the review asks for, plus a positive (every
operative branch dead is still a no-op) and a regression (a conditional sibling never rescues a
dead row). The single-branch and computed-magnitude grammars are untouched.

---

## Weakest evidence, in order

1. **F7** has no test of any kind — no fixture, no pin. Source-reasoned only.
2. **F4** is pinned by nothing executable either: the fall-through is stateful across ticks and
   the sidecar path needs a live endpoint. Both halves are source-traced and the suite is green,
   which proves only that nothing else broke.
3. **F5's re-ask** (as opposed to its floor, which is pinned) never runs in the suite — no
   `finish_reason=length` is producible without a model. The floor arithmetic is pinned; the
   round trip is not.
4. **F2's replacement choice** (lowest toughness) is a defensible proxy for "a creature of their
   choice", not the opponent seat actually choosing. The rules-correct engine would ask the
   sacrificing player at resolution; that is a larger change than this finding.
5. **F6** restores a path with zero occurrences in the last corpus, so its own effect is
   unobservable until the shape recurs.
