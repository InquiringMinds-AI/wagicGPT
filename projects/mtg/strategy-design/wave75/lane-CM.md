# Wave-75 lane CM — the fix lane for the Astra adversarial review

Base: master `301a30a37` (worktree `worktrees/lanes/w75-CM`, branch `w75-lane-CM`).
Source: `wave75/codex-review.md` (Codex `gpt-6-astra`, read-only, source-traced C++).

**Gates on the finished tree.** Suite THREADS=1 **1289 tests (0 failed), 76 AI tests
(0 failed)**; `==Test Failed !==` 0 and `==Test timed out` 0 (none of the three known
flakes fired). PARSETEST **5954 passed, 0 failed** (base 5901, +53 from this lane's
block). `selfplay-harness.sh --selftest`: harvest 4/0, **pilot-stall 17/0** (was 15),
window-loop 12/0, regime-gate 30/0; `bash -n` clean.
`check-reply-instructions.py` OK (1 source, 37 guides); constructor guard OK (118 files).
`git diff | grep -c U+FFFD` = **0**. Every wagic invocation ran under
`systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0`; nothing was killed
under the cap. One new suite fixture, registered in `_tests.txt` (1409 -> 1410 lines).

**RED-on-base evidence, collected in one run.** Before any fix landed, the seven
source-level findings were expressed as PARSETEST assertions against the SEEDED binary
and all seven failed:

```
[#W75-CM] RED PROBE
  FAIL  #W75-CM RED F2 two costs, two actions
  FAIL  #W75-CM RED F3 negated payload is not an answer
  FAIL  #W75-CM RED F3 bare-name payload is not an answer
  FAIL  #W75-CM RED F4 the P9 inline answer counts
  FAIL  #W75-CM RED F5 a parked close is not unrecorded
  FAIL  #W75-CM RED F6 a U+00A0 tail is trimmed
  FAIL  #W75-CM RED F6 a U+3000 tail is trimmed
  FAIL  #W75-CM RED F7 an infect self-ping does not feed a life chain
  FAIL  #W75-CM RED F8 a legendary token is a legend twin
=== self-test: 5901 passed, 9 failed ===
```

F1 got its RED from the suite (below) and F9 from the harness's own `--selftest`.
**All nine findings are CONFIRM. Astra was right nine times out of nine**; the only
correction this lane makes to the review is to F1's prescribed fix, which the target DSL
could not express as written (see F1).

---

## F1 HIGH (P4) — `other` excluded Sorin from the CREATURE alternative too  *(CONFIRM, fixed)*

**Verified against the code, not the claim.** `TargetChooser::other` is a single
chooser-level bool (`TargetChooser.h`), parsed off the WHOLE spec string
(`TargetChooser.cpp:117`, `s.find("other ")`), and `canTarget` refuses the source on it
(`TargetChooser.cpp:1616`) BEFORE any type or descriptor is evaluated. So on
`other *[creature;planeswalker]` the restriction reaches both alternatives, and Oracle
("Destroy up to three target creatures and/or **other** planeswalkers") restricts only
the planeswalker one. Astra's trigger is real.

**Can the DSL scope it? No — pinned.** In `createTargetChooser` the comma loop folds every
`typeName` into ONE `tc`, and a SECOND bracketed descriptor is not built at all
(`if (cd) { if (!tc) tc = NEW DescriptorTargetChooser(...); else { delete (cd); return NULL; } }`).
There is no union chooser in the class list. So two alternatives with different `other`
scoping cannot be written in card data as the engine stands, and both single-flag
readings are wrong: keeping `other` LOSES a legal target (an animated Sorin), dropping it
ADDS an illegal one (an unanimated self-target). Never removing a legal option is the
harder invariant, and the review asked for the reading that loses none — so the scope got
a name in the DSL instead.

**Fix (engine, additive).** New target-spec token `other!<type>[;<type>...] `
(`src/TargetChooser.cpp`, `include/TargetChooser.h`, `#W75-CM (F1)`): parsed before the
plain `other ` form so the longer token wins, the whole token erased so its type list can
never reach the type parser, and carried onto the chooser the spec builds
(`tc->otherUnless`). `canTarget`'s `other` block is now
`if (other && !otherAllowsSource(card))` — a candidate holding one of the named types is
exempt from the restriction; with an empty list the behaviour is byte-for-byte what it
was, so no existing script moves. `creature` asks the INSTANCE (`isCreature()`), because
an animated planeswalker is a creature only while the effect stands, which is the whole
case. Card data (`bin/Res/sets/primitives/planeswalkers.txt:3059`, byte-spliced):

    -target(<upto:3>other *[creature;planeswalker]|battlefield)
    +target(<upto:3>other!creature *[creature;planeswalker]|battlefield)

**RED / GREEN.** New fixture `w75cm_sorin_minus6_animated_self_is_legal.txt`: Sorin plus
Sarkhan the Masterless, Sarkhan's `+1` activated (`all(planeswalker|myBattlefield)
becomes(dragon creature,4/4,red,flying) ueot`), then `assertcantarget`. With the shipped
`other ` form restored on the same binary:

    TESTSUITE assertcantarget: 'sorin, lord of innistrad' expected 1 got 0 (choosers seen: 1)
    ==Test Failed !==   Test suite finished: 2 tests (1 failed)

With `other!creature`: 2 tests, 0 failed — and lane CK's
`w75ck_sorin_minus6_not_self_target.txt` (UNANIMATED Sorin, `expected 0`) passes in both
states, so the self-planeswalker case is still forbidden and Serra Angel is still legal in
every run. Nothing was removed from any legal set.

**Also done: lane-CK.md's "general re-entry latch hole" is rewritten as SUPERSEDED.**
Astra is right and the paragraph was misdiagnosed. CR 400.7 — an object that changes zones
is a NEW OBJECT with no memory of its previous existence — and CR 606.3's "no loyalty
ability of **that permanent** has been activated this turn" together say that a
planeswalker which died and returned (or was flickered, or bounced and replayed) MAY
activate a loyalty ability again. An identity latch keyed on the player or the oracle name
would remove legal activations. **No latch was built and none should be.** The wave-74
`126v152` narration is one defect, not two: the self-target, which this fix closes.

---

## F2 HIGH (P1b) — `holdActionKeyRow` erased the ACTIVATION COST  *(CONFIRM, fixed)*

**Repro.** `holdActionKeyRow` (`AIPlayerGPT.cpp`, #W74-CH) strips every `[...]` group
through `stripRenderAnnotationsLc`, and the renderer prints an activated ability's cost
inside one (`AIPlayerGPT.cpp:34251`, `o << " [cost: " << cost.str() << "]"`). Two
`name(...)` activations of one permanent at different costs — `Draw with Thought Vessel
[cost: {3}]` and `[cost: Tap]` — therefore key identically, and mana braces INSIDE a
square bracket do not survive the strip either. A newly legal second activation reads as a
row the model has already been offered, and the hold covers it.

**RED on base.** `holdActionKeyRow(a) != holdActionKeyRow(b)` — FAIL.

**Fix** (`#W75-CM (F2)`): `w75CostGroupsKey` lifts every `[cost: …]` group out of the row,
lower-cased and whitespace-collapsed, and appends it to the action key; every other
bracket and brace group is still stripped exactly as before. The cost is what the row
OFFERS ("this card, at this cost, against these objects"); the forecasts, prices, clocks
and counts around it remain the render's commentary on a board no hold claims is frozen.

**GREEN.** The two costs key apart; the hold-check census reads the new activation as **1
unseen row** (0 on base) and `holdStillStands` retires the hold instead of silently
extending it. **Key-stability pin set** (held set built the LIVE way, `holdActionKeyRow`
over `mLastMenuRows`): on the wave-74 `146v125` seq 65/66/70 three-row shape, whose only
delta is one row's `{leaves N of your M untapped mana sources}` count and its
`[declined this exact list N times this turn]` tag, **with a `[cost: {3}]` group now on the
row** — unseen rows 0, hold stands, hold-latch keys equal, option-set key equal, async
slot key a pure function of the unchanged ask tail. #W74-CH's carve-out survives the cost
being kept.

**Prediction.** In the next corpus, no seat is served a menu carrying two `[cost: …]`
variants of one `name(...)` activation without the hold-check bracket reporting at least
one new row; and the `146v125`-class oscillation (`1 row above is new` /
`every row above was also on the menu` over a byte-identical menu) does not reappear.

---

## F3 HIGH (P9) — the inline `CHOICE:` gate accepted an explicit NON-choice  *(CONFIRM, fixed)*

**Repro, verified in the code.** `codedHeadEnd` returns `npos` for a payload with no digit
head, and BOTH `choiceLineIsClean` (returns `true`, "not judged") and
`choiceLineIsRejection` (returns `false`) decline to judge it. So
`PLAN: Do not take CHOICE: not 2 (Cast Rorix Bladewing)` passed #W75-CJ (P9)'s inline gate
whole — one label, whitespace-anchored, on a PLAN-leading line — and the downstream parser
can bind the exact parenthetical name or rescue the embedded integer, executing the cast
the reply explicitly refused.

**RED on base.** Both the negated payload and a bare-name payload
(`PLAN: hold CHOICE: Rorix Bladewing`) parsed as answers — FAIL, twice.

**Fix** (`#W75-CM (F3)`): the inline form now requires the coded head the protocol asks
for — whitespace, then a digit run — or it is not an answer. On a LINE-LEADING label a
name-form payload is the legacy reply shape this project still accepts; inside the plan
line it is prose, and prose is what the P9 gate exists to refuse. Nothing else about the
gate moved, and `reply-protocol.txt` / `kReplyProtocol` are untouched.

**GREEN.** Both MUST-NOT-MATCH pins hold; #W75-CJ's own repro
(`PLAN: Play a land … CHOICE: 1 (Play Sunpetal Grove)`) still parses, `CHOICE:   3` (bare
index) still parses, and `CHOICE: maybe 2 later` does not.

**Prediction.** No translog record shows a consumed `choice` whose reply's only CHOICE
label sits inside a sentence that negates it; `plan_line`-carried answers keep parsing at
the wave-75 rate.

---

## F4 MED (P21) — two definitions of "coded answer"  *(CONFIRM, fixed)*

**Repro.** `gptForceCloseEarned(…, codedAnswerCount(content))` asked the strictly
LINE-LEADING scanner while the answer parser had just learned (P9) to read a CHOICE label
on the PLAN's own physical line. A complete cap-stopped reply of exactly that shape counted
ZERO answers and was discarded for a second decode that can choose differently or fail.

**RED on base.** `codedAnswerCount("PLAN: Play the land. CHOICE: 1 (Play Sunpetal Grove)")`
was 0 — FAIL.

**Fix** (`#W75-CM (F4)`): `codedAnswerCount` falls through to `gptInlineChoiceOnPlanLine`
when the line scan finds nothing. One reader, two callers.

**GREEN.** The count is 1 and `gptForceCloseEarned` no longer arms on it; a cap-stopped
reply with NO answer of either shape is still rescued; `ANSWER: CHOICE: 3 (Cast nothing)`
still counts 0 (the inline reader still requires the line-leading PLAN marker); and a
two-line multi-answer reply still counts 2, so the violation census is unchanged.

**Prediction.** `phase2_answer_recovered + phase2_answer_missing` falls relative to
`coded_answers >= 1` records, and no record carries `reasoning_forced_close` beside a
phase-1 reply whose text already contained an inline CHOICE answer.

---

## F5 MED (P2) — the armed flag was seat-global beside per-arm storage  *(CONFIRM, fixed)*

**Repro.** #W74-CF (F1) made the retry slot PER ARM (`mRetryPark` + `selectRetryArm`), but
`mForceCloseArmed` stayed one seat-global bool. Land arms a close; `selectRetryArm` parks
it; casting arms another — the second arming reads the FIRST arm's flag, increments
`mForceCloseUnrecorded` for a close that is safely parked, and recording either close
clears the flag for both. The accounting identity `recovered + missing + unrecorded ==
closes` then fails without a single request being lost.

**RED on base.** The shared-flag rule, simulated exactly (`if (shared) unrec++; shared =
true;` twice), yields `unrecorded == 1` — FAIL.

**Fix** (`#W75-CM (F5)`): `forceCloseArmed` joins `GptRetrySlot`;
`gptRetrySelectArmClose` carries the seat's live flag through the same swap
`gptRetrySelectArm` already performs; `gptForceCloseArm` / `gptForceCloseOutstanding` are
the arming and the record's accounting, both pure. The record now reads
`mForceCloseUnrecorded + gptForceCloseOutstanding(mForceCloseArmed,
mRetryPark.forceCloseArmed)`.

**GREEN.** Astra's interleaving, replayed through the PRODUCTION swap (land arms; casting
polls and land parks; casting arms; both complete and are recorded): **2 outstanding
closes, 0 unrecorded, identity 0 at the end**. MUST-NOT-MATCH: re-arming the SAME arm while
its close is outstanding still counts one unrecorded — the real "third path" #W75-CJ (P2c)
was built to see.

**Prediction.** In the next corpus `forced_close_unrecorded` is 0 in every game where
`phase2_answer_recovered + phase2_answer_missing` equals the stderr close count; ~~a non-zero
value now means a genuinely lost close, on one named arm.~~

> **SUPERSEDED (wave-76 lane CN, Q8; wave-75 engine-seat §2(c) + MED-2).** The second clause
> is WRONG and is retained only as history. The wave-75 corpus ran the identity
> `recovered + missing + unrecorded == closes` and it held in **21 of 21 games** *with*
> `forced_close_unrecorded` = 9 (15 + 0 + 9 = 24) — so the first clause PASSED and the
> non-zero value did **not** mean a lost close. All nine are closes that were armed,
> SUPERSEDED by a question that moved, and re-asked: every one is followed by
> `dropping stale async answer ... [outcome: re-asked]` or the seat's own re-serve, the
> corpus carries 0 fallbacks of any kind and 0 `phase2_answer_missing`. The correct reading:
> **a non-zero `forced_close_unrecorded` is a WASTED phase-2 decode (~80 s each, ≈12 min
> across the corpus), not a lost decision.** A lost decision would show as a fallback or as
> `phase2_answer_missing`, and both were 0. Wave-76 Q8 adds `force_close_arms_refused` as
> the bound on the waste; the identity itself is untouched.

---

## F6 MED (P2) — the trim modelled six ASCII characters, `str.strip()` is Unicode  *(CONFIRM, fixed)*

**Repro.** `gptForceClosePrefillBody` trimmed `" \t\r\n\f\v"`. Jinja's `|trim` is Python's
`str.strip()`, which removes every code point for which `str.isspace()` is true. A trace
ending in U+00A0 (or U+3000, U+2028, U+0085, U+205F …) survived our trim, the template
removed it, the rendered prompt no longer contained the final message byte for byte, and
`continue_final_message` 400s — the exact failure the helper exists to prevent. Astra also
noted the PARSETEST `QwenTpl::trim` REPEATED the ASCII-only implementation, so its
fixed-point assertion could not catch it.

**RED on base.** U+00A0 and U+3000 tails both survived into the built body — FAIL, twice.

**Fix** (`#W75-CM (F6)`): `gptPyWsLenAt` / `gptPyStripFrom` / `gptPyStripTo` implement
Python's whitespace set over UTF-8 (ASCII 0x09-0x0D, 0x1C-0x1F, space; U+0085, U+00A0,
U+1680, U+2000-U+200A, U+2028, U+2029, U+202F, U+205F, U+3000).

**GREEN.** The PARSETEST template model now implements the SAME set INDEPENDENTLY (its own
table, it does not call the helper) and the built body is a fixed point for seven tails
including U+00A0, U+3000, U+2028, U+0085, a both-ends case and the two wave-74 ASCII
shapes. A wholly-whitespace Unicode trace still sends the empty think block. MUST-NOT-MATCH:
an INTERIOR U+00A0 is content and is kept, because the template keeps it.

**Prediction.** No `http_error` record carries the body
`continue_final_message is set but the final message does not appear in the chat after
applying the chat template` (#W75-CJ P2a now logs that body).

---

## F7 HIGH (P5) — a rendered damage subtraction is not always life loss  *(CONFIRM, fixed at the source)*

**Repro, verified in the engine.** `Damage.cpp:192-199` REPLACES damage dealt to a PLAYER
by an INFECT / POISONDAMAGER source with poison counters; the life total never moves. The
project already knows this in two places — `sourceDealsPoisonInsteadOfDamage`, the
narration seam's `dealt as POISON COUNTERS, not life loss` line, and the blocker forecast's
poison partition — but `damagePlayerVerdict` subtracted from life anyway. A Prodigal
Pyromancer wearing Glistening Oil pinging its own controller at 20 life printed
`takes 1 damage - you would be at 19`, and #W75-CL (P5) then upgraded that false arithmetic,
under a proven opposing Sanguine Bond + Exquisite Blood loop, into the categorical
`you would be at 0 - this row feeds their chain`. Neither loop intake is fed by a poison
counter.

**RED on base.** `w75ChainFeedRow(damagePlayerVerdict(1, 20, true), true)` carried the
chain-feed tail — FAIL.

**Fix at the SOURCE, not the symptom** (`#W75-CM (F7)`): `damagePlayerVerdict` takes
`poisonInstead` and the target's poison count, and on an infect source prints the poison
forecast instead of a life subtraction — `takes 1 infect damage - dealt as POISON COUNTERS,
not life loss: your life total does not move and stays at 20. Your poison would be 1 of 10
- poison counters never reset, so this is permanent progress`. Both call sites pass
`sourceDealsPoisonInsteadOfDamage(...)` and `poisonCount`: the activated-ability row
(`adp->source`) and the spell target menu (`tc->source`). The P5 rewrite needs no gate at
all — its trigger phrase `you would be at ` is simply not printed on a poison row, so
there is no false subtraction left for it to find.

**GREEN.** The poison row carries no life subtraction, says what DOES happen, and
`w75ChainFeedRow(row, true) == row` byte for byte. MUST-NOT-MATCH: an ordinary damage
source still subtracts life and still feeds the chain under a live loop — #W75-CL (P5)
keeps every byte it earned, including the `125v126` seq 267 row that lost a 42-7 game. The
tenth counter is stated as the loss it is on both sides, and #W60-L (B1)'s survival guard
holds on the opponent-poison claim (`YOU LOSE BEFORE THIS RESOLVES`). An unsupplied poison
count claims no number at all.

**Key-stability pin set.** The poison row's board-derived numbers (the life total it says
does not move, the poison count) live INSIDE the `{right now: …}` group, which
`stripRenderAnnotationsLc` removes whole: two windows differing only in those numbers give
identical hold-latch keys, 0 unseen rows, a standing hold and an identical option-set key.
The ask key and the async slot key are the RENDERED tail, and a `{right now: …}` magnitude
has ridden them for every damage row since wave 54 — the poison form is byte-for-byte in
that pre-existing class and adds no new exposure; the pin states the slot key is a pure
function of the ask tail either way.

**Prediction.** No corpus row printed for an infect/poisondamager source contains
`you would be at`, and no `feeds their chain` clause appears on a row whose source has
infect. If a seat still takes a self-ping under a live loop, the row it took will say
`poison would be N of 10`, and that is a play decision, not a false surface.

---

## F8 HIGH (P7) — the legend-rule walk excluded tokens  *(CONFIRM, fixed)*

**Repro.** #W75-CK (P7)'s battlefield walk had `if (bc && !bc->isToken && bc != card &&
bc->name == card->name)`. A legendary Rorix TOKEN (a Cackling Counterpart copy kept through
an earlier legend choice) plus a cast Rorix therefore counted TWO bodies, and
`crackBackBlockerRowTag` printed `This adds 1 body … you cover 5 of 5 … which you SURVIVE`
at 3 life. CR 704.5j is about legendary permanents; a token permanent is one.

**RED on base.** The walk, extracted as the pure `w75LegendTwinControlled` over its own
inputs WITH the base's token exclusion, returned false for a token twin — FAIL. (The
review's own note that lane CK's pins "bypass the faulty battlefield walk by supplying
`legendTwinControlled` themselves" is why the walk itself is what got extracted.)

**Fix** (`#W75-CM (F8)`): the token exclusion is gone from `w75LegendTwinControlled`, and
production builds its input by walking `game->inPlay` once. The `[legendary: you already
control …]` MARK at the cast seam (`AIPlayerGPT.cpp:~40364`) had the same exclusion and now
counts a token twin too, so the bracket and the cover state the same board fact — the
non-legendary "second copy" mark keeps its own token exclusion, which is a different claim.

**GREEN.** A token twin is a twin; `castBodiesNetOfOwnText(1, true, twin) == 0`, so the row
prices nothing at all rather than promising a survival. MUST-NOT-MATCH: the card being cast
is not its own twin, and a token with a different name is not one.

**Prediction.** No `{crack-back cover: …}` clause claims a body for a cast whose name
already stands on the seat's own battlefield, token or not; the `[legendary: you already
control …]` bracket and the cover clause agree on every row that carries both.

---

## F9 HIGH (P24) — the pool could reach K from a subset of the live seats  *(CONFIRM, fixed)*

**Repro (Astra executed this against the extracted Python).** #W75-CI (P24) bounded each
seat's CONTRIBUTION to `ceil(K / live seats)`, which stops one wedged seat supplying all K
— but it does not make the pool COVER every seat. At K=6: four live seats (three carrying
two timeouts each, the fourth being answered normally) fills the pool from the three
failing seats and reads `STALL 6`; twenty-one live seats (six failing, fifteen answering)
likewise. The banner then claims "the last records of every live seat are timeout
fallbacks", which was false. `allHaveEvidence` checked the PRESENCE of records, never
whether the omitted seats were failing.

**Fix** (`selfplay-harness.sh`, `#W75-CM (F9)`): `allNewestSilent` — every live seat's
NEWEST record must be a silent-class failure. A seat whose newest record is a decision the
model answered is evidence AGAINST an endpoint stall, and one such seat is enough: the
endpoint is generating.

**...with the recency term that reconciles it with #W74-CF's own case (j).** That case
requires STALL when one live seat holds a single ARBITRARILY OLD success beside six newer
failure streaks. A newest record is counter-evidence only while the seat is still WRITING,
so a log whose mtime trails the newest live log by more than a grace window contributes
none. Grace is 600 s against a 120 s HTTP deadline — deliberately generous, because being
wrong here costs a corpus that keeps running (still bounded by the game watchdogs), while
being wrong the other way kills viable work.

**GREEN.** `pilot-stall-selftest: 17 checks, 0 failed` (was 15). Both of Astra's healthy
shapes are now cases (j) and (k) and read OK; the single-live-seat 20-timeout tail still
reads `STALL 6`; the wave-73 `wall_miss_no_retry` tail shape still reads `STALL 4`; the
P24 per-seat bound (case l) still reads `OK 1`; #W74-CF's stale-success case still reads
`STALL 6`. `bash -n` clean.

**Prediction.** No corpus is stopped with a `PILOT-STALL` marker while any live seat's
newest translog record is a model-answered decision written within ten minutes of the
newest live log.

---

## Weakest evidence

1. **F1's `other!` scope is honoured on ONE code path.** The list is attached at the
   factory's common `return tc;`, which is where descriptor and type choosers arrive; the
   ~50 `return NEW DamageableTargetChooser(...)` early returns above it do not carry it
   (they would silently behave as plain `other`). Sorin's spec reaches the common exit and
   is pinned by fixture; no other card uses `other!` today, so nothing else can be wrong
   yet — but a future author writing `other!` on a damageable spec would get the old
   behaviour with no diagnostic. Routed, not fixed: widening it means threading the list
   through every constructor.
2. **F7 is pinned pure, never observed live.** No corpus game in
   `matchups-20260909-104713-final` contains an infect source, so the false
   `you would be at` was never printed in anger — Astra's trigger is a constructed board
   (Glistening Oil on a Prodigal Pyromancer). The fix is proved on the emitters and on the
   two call sites by inspection; a probe deck would prove the wiring. The MUST-NOT-MATCH
   half (ordinary damage unchanged) is the half the corpus does exercise.
3. **F2's cost key assumes `[cost: …]` never carries a board-derived number.** It carries
   the PRINTED cost, and the key-stability pin covers annotations moving around it — but
   if a future render prices a cost against the board inside that bracket, the hold would
   re-open on it. The pin set would catch it only if that pin is re-run with the new shape.
4. **F5 is pinned on the pure swap, not on a live two-arm seat.** The interleaving is
   replayed through the production `gptRetrySelectArmClose`/`gptForceCloseArm`, but no
   endpoint stub ran; Astra's own note that "a serial stub cannot expose this interleaving"
   applies to any test short of a live corpus. The accounting identity on the next corpus
   is the real gate.
5. **F9's 600 s grace is chosen, not measured.** It separates Astra's shapes (mtimes within
   a second) from #W74-CF's stale-success case (a billion seconds apart) by an enormous
   margin, so no selftest discriminates the threshold. A real run where one seat legitimately
   goes 10+ minutes between records while every other seat wedges would still read STALL —
   which is the safe direction, but it is an assumption about decision latency.
6. **F8's second half (the `[legendary: …]` mark) has no fixture.** It is pinned only by the
   shared pure predicate and by inspection of the walk; the mark itself is rendered from a
   live board.
