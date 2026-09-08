# Wave-73 lane CA — instruments + render residue (N8, N12, N13, N14, N17)

Base `90f8d1d68`, branch `w73-lane-CA`. Corpus read: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-163626`
(wave 72, 1,910 model decisions over 42 seats), with `matchups-20260907-085638` (wave 71) as the
baseline where a ratio is quoted. No corpora launched. No guide edited. No protocol text touched.

Gates on the final binary: suite `WAGIC_TESTSUITE_THREADS=1` **1281 tests / 0 failed, 76 AI / 0
failed**, both failure strings counted at 0 (`==Test Failed !==`, `==Test timed out`), and the three
known single-thread flakes did not fire this run. PARSETEST **5487 -> 5513 passed, 0 failed (+26)**.
Both link guards OK (`check-ctor-init` 118 files, `check-reply-instructions` 1 source / 37 guides).
`git diff | grep -c U+FFFD` = 0.

---

## N8 MED — `replay_run` meant two things, and only one of the two re-serve paths was bounded
**CONFIRMED, both halves. FIXED.**

*What the field said.* `logAskReplay`'s `run` argument was passed `mAskReplayRuns[askKey]` (THIS
window's consecutive run) on the `cache_replay` path and `mRepeatAskAnswersReserved` (the GAME's
running total of re-served answers) on the `repeat_ask_reserved` path. Corpus maximum `replay_run`
90, at `123v125` seq 184, `why: repeat_ask_reserved`, against `kAskReplayRefuseMax` 64 — which reads
as a breached cap and is not one. True maximum per-window run in the corpus: 11.

*What the bound did.* `askReplayRefuseScoped` guarded the `mAskCache` path only.
`repeatAskAnswerStands` — **327 of the corpus's 521 replays (63%)**, up from 68 of 725 in wave 71 —
was bounded only by the turn boundary. The wave-71 drain loop's successor lives on the majority path
and the meter that would show it was the mislabelled one.

*Fix.* `replay_run` is now always the window's own run; the game total moved to its own record field
`replays_this_game`, written on every `ask_replay` record. The repeat latch runs through the SAME
predicate on its own map (`mRepeatAskRuns`), with the same cap, the same re-arm, the same lifetime
(cleared at the turn boundary beside `mAskReplayRuns`, erased for a window the model really
answers), and the same consequence: the latch is dropped and the window goes to the model. A refusal
writes `why: repeat_ask_refused` and counts onto `ask_replays_refused`.

*RED on base.* `#W73-CA N8 REPRO the repeat latch refuses at the SAME cap the ask cache does` —
fails on `90f8d1d68` because nothing called `askReplayRefuseScoped` with the repeat key. Plus a
long-but-finite negative (63 re-serves still served) and an interleaving MUST-NOT-MATCH.

*Prediction, next corpus.* Every `replay_run` in `askreplay/` is <= 64 and the maximum equals the
maximum per-window run computed from the sidecar itself; `replays_this_game` reproduces the old
`repeat_ask_reserved` numbers exactly (327-shaped, monotone within a game); `ask_replays_refused`
stays 0 unless a repeat-latch loop actually exists, in which case it is non-zero and visible.

---

## N12 MED — measured first, then fixed. The two halves have different answers.

### (a) `reserve_decline_windows_noted` 0 over 48 reserve rows — CONFIRMED, root cause is a DEAD PREDICATE. FIXED.
Not "the note is not wired to the path that renders" (the wave-72 seat's own hypothesis, correctly
left open). `menuHasReserveRow` matched the literal `"{reserve: this row is INSTANT SPEED"` — the
clause's **wave-67 opening words**. `#W72-BV (M9)` rewrote `sorceryReserveClause` to LEAD with the
consequence, so the live clause opens `{reserve: TAKE THIS ROW AND YOU CANNOT CAST <name> AT ALL
THIS TURN. This row is INSTANT SPEED...`.

Measured on the wave-72 corpus:
* 55 windows render a reserve clause; the 10 most common openings are all `TAKE THIS ROW AND...`.
* `{reserve: this row is INSTANT SPEED` appears **0 times corpus-wide**.
* Therefore the predicate returned false everywhere, and **both** counters it gates read 0 on all 42
  gameends: `reserve_decline_windows_skipped` 0 **and** `reserve_decline_windows_noted` 0.

This is a wave-72 regression, not a wave-73 discovery of dead code: the step latch worked in wave 71
under the old wording. It is also a textbook silent-instrument failure — the PARSETEST pins at
`#W67-AX I7` feed `menuHasReserveRow` the **wave-67 corpus rows**, which the renderer no longer
produces, so the pins stayed green through the whole regression.

*Fix.* Match the clause FAMILY (`"{reserve: "`, the same once-only literal the `#W69-BJ F1`
composed-row census uses), so a future reword of the clause body cannot silently disarm the latch.
New pins: POSITIVE built by calling `sorceryReserveClause` itself (so the pin can never again drift
from the renderer), a REGRESSION on the wave-67 wording, and a MUST-NOT-MATCH on a row whose card
text merely contains the word "reserve".

⚠ **This re-arms a window-SKIPPING path.** `reserveDeclineHonoured` was designed and reviewed in
`#W67-AX (I7)` and ran live in wave 71; nothing about it changed here except that its gate matches
again. Predicted next corpus: `reserve_decline_windows_skipped` non-zero on any seat that declines a
menu carrying a reserve row (deck162, deck126, deck123 are the seats that rendered them), and
`reserve_decline_windows_noted` non-zero wherever a same-turn later phase re-offers the same
castable set. If skipped rises but noted stays 0, the M9c turn-scoped half has a second defect and
this lane only found the first.

### (b) the declined-count clause's ~25% — MEASUREMENT REFUTES THE DEFECT. NO CHANGE MADE.
The brief required the cause be measured before anything changed. It was, and the class is not a
mis-answer.

**The measurement (two lines, corpus-wide).** 233 windows rendered `[you declined this exact list N
times already this turn]`; the answer was hold 81, an action 47, and the plain decline **105 (45%)** —
the per-seat 24% the deck125 seat measured is the low end, not the level. Of those 105, **101
explicitly weigh the hold row in the reasoning and reject it**, and **95 name an action they intend
to take LATER THIS TURN** (an activation at its own window, combat, a second main cast) — which is
exactly what the hold row surrenders ("the rows above include ACTIVATED abilities that are usable
RIGHT NOW"). Samples in the seat's own voice: *"Since I have nothing to do, holding is fine, but
'Cast nothing right now' is standard"*; *"Activating Staff is a separate window... The plan line
covers it."*

**And the cost is mostly zero.** Of the 105 plain declines under the clause, **70 (67%) were followed
by no further model call at all on that identical list in that turn**. 35 cost at least one, 79
extra calls in total = **4.1% of the corpus's 1,910 decisions**.

So the cause of the "loss" is that the choice is correct and the clause's implied preference is not.
The clause states a true fact and is silent about what to do, which is right; the wave-70/71/72
suggestion (drop or tag the plain decline row when the clause prints) would remove or discourage a
legal option that the pilot is choosing deliberately and, on this evidence, correctly. **I made no
change.** The real residual is the 79 repeat calls, which is N8's territory: those windows reached
the model because the ask cache keys on the BOARD and the board moved under an identical option
list. A cross-tab of `options_text`-identical, board-different same-turn windows is the measurement
the next wave needs before anyone touches this clause again.

---

## N13 MED — REFUTED as stated. The land-drop seam already has the check; the cited case is not reachable by any prompt-side check.
The claim was "the land-drop seam has no plan-vs-board name check; the attackers seam prints one —
reuse it". The block deck126's review quoted (`(note: your plan names "X", and no "X" is in your
hand, on either battlefield, in a graveyard, in exile or on the stack right now...)`) is
`planLibraryOnlyNote`, and it is emitted in `assemblePrompt`, which is seam-agnostic. Measured by
`log_window_kind` over the corpus:

| seam | prompts | carrying `your plan names` |
|---|---|---|
| land_drop | 325 | **31** |
| empty_stack_pass | 265 | 45 |
| unclassified | 1,154 | 219 |
| cleanup_discard | 26 | 13 |

The land-drop seam is not excluded. What it does NOT carry is the *menu-diff* clause (`"X" is no
longer on your menu`, 0 of 325), and that suppression is CORRECT: `planAbsentActionName` gates on the
menu being a cast menu, and a land-drop menu offers no casts, so "X is no longer on your menu" would
be a false claim about staleness.

The cited instance does not support the item either. `126v152` seq 16's *incoming* plan was the Sorin
plan (correctly silent: Sorin was in exile, a visible zone). The hallucinated premise — *"I can cast
Exquisite Blood right now! {4}{B} is available"* + Staff of Nin — was authored **in the same reply
that took `Play no land`**. No prompt-side note can contradict a claim the model has not made yet.
It was caught at the very next window (seq 18 renders the note naming Staff of Nin, as designed).

Residual worth a docket, not a fix this lane: `planNamesLibraryOnlyCard`'s vocabulary is the LIBRARY,
so a plan naming a card in **no zone at all** earns no note. Catching a false premise authored inside
the reply that acts on it would need a post-reply check at the land-drop seam, which is a re-ask
mechanism, not a render; it should be scoped deliberately, not smuggled in under a MED.

---

## N14 MED — four render items. All four CONFIRMED, four FIXED (one of them by gloss, not by forecast).

**(a) the animated land's death price.** `152v146` seq 30 -> 32: the seat animated Lair of the Hydra,
sent it into a listed blocker whose own tag said the attacker dies, and lost a LAND off a four-land
board. The animation row priced mana (`{rung ceiling: ...}`) and duration (`the animation costs ...`)
and never said the body and the land are one permanent. New `{death price: <name> is a LAND you
control - while it is animated it is a creature AND still that land, so if it dies in combat or to
removal you lose the land itself and the mana it makes, not just a body}`, on every self-animation
row of a LAND (broader than the rung-ceiling gate, which requires the land to be a mana source — a
creature-land that makes no mana is still a land you lose). It is a rules fact, so it is true at
every rung and on either board; added to `stripNarrationDecoration`'s decision-time list beside
`{rung ceiling: }` so it never enters history. Pins: positive, empty-name negative, an echo pin
proving the brace never survives into narration, and a pin that it states no board arithmetic.

**(b) the ghostform counter.** `152v146` seq 25 printed `[counters: 1x ghostform]` bare on Nadaar
while the trade tag read `(both die)`; the seat traded its Intrepid Adversary for a blocker that
returned to hand and left a 1/1 flier. The trade tag's claim is TRUE (the creature does die) — what
was missing is what the death DOES, which is the counter's business. `legibleCounterName` is the
table the `TeferiEffect` fix explicitly opened for "the next engine-named counter"; ghostform is now
in it, worded from Kaya the Inexorable's own `text=` line
(`Res/sets/primitives/planeswalkers.txt:1877`, checked against the `auto=` transform at :1874).
I did NOT touch the combat forecast.

Card note for the deck seats, not a lane item: the script targets `creature[-token]|mybattlefield`
while the Oracle reads "up to one target nontoken creature" (any). A script/Oracle divergence on
targeting scope, unrelated to the render defect and not what cost the game.

**(c) the MDFC back face with no link to the hand card.** `146v125` seq 35 printed
`Hand: Brightclimb Pathway` over rows `1. Brightclimb Pathway (menu text: Play Land)` and
`2. Grimclimb Pathway [PLAY THIS AS A LAND ...]` with nothing saying they are one card
(*"maybe I have a Grimclimb in hand?"*). Cause: the wave-59 fix for a real defect (the "can no longer
be cast" clause is nonsense about a LAND front face) passed an **empty front name**, which dropped the
identity link along with the clause. Measured: **58 of 295** rendered `PLAY THIS AS A LAND:` tags
carry no front-face clause at all. The name is now always printed; only the consequence branches —
land front face gets `and it is the OTHER FACE of "<front>" in your hand - one card with two land
faces, not two cards: playing either face spends the same card and the same land drop`. The spell
front face's wave-57 wording is pinned byte-identical.

**(d) "held back, it CANNOT block: X" beside "their untapped blockers: X".** `146v152` seq 31; the
model's own reading: *"This is contradictory. Usually, 'their untapped blockers' lists creatures that
CAN block."* The two tags run in opposite directions and the first named no subject. Fixed as a
SUBJECT problem, not a fact problem: `[held back, THIS creature could not block <names>]`, with the
`<label>: <name>` shape that mirrored the other tag gone from the short form. `"held back"` is kept
verbatim — the scope footnote and deck146's live guide (line 128) both key on it — and the footnote
now states the direction once: the two tags run opposite ways and the same name can honestly appear
in both. No affirmative `can block` substring anywhere, per the wave-29 rung.

*Predictions.* Zero reasoning bodies call the two block tags contradictory; zero call an MDFC row a
menu-generation typo; a creature-land animation taken into a listed killer is either not taken or is
taken with the land loss named in the reasoning; a ghostform-countered blocker is not traded with as
if killing it were removal.

---

## N17 LOW — CONFIRMED, and bigger than the deck reviews could see. FIXED IN THE RENDER.
Two deck seats each reported 2 replies echoing `YOUR PLAN:`. Corpus-wide it is **10 of 1,910
(0.52%)**, and they are **13.7% of the 73 `plan_line_missing` records** — every one also an
off-protocol charge.

`#W71-BR (L11)` took the COLON off the heading and the echoes did not stop, because what the model
copies is the **caps phrase**, not the punctuation: `YOUR PLAN` is the only all-caps label-shaped
token on the screen that names a plan, so it gets reused in the label slot. L11 kept the phrase for a
stated reason — "three live deck guides key on that string" — and that reason has expired: the
wave-72 guide pool has no key on the heading (`deck125` line 16 "YOUR PLAN LINE", `deck130` line 80
"YOUR PLAN'S VERB", both generic prose). So the phrase goes:
`THE PLAN YOU LAST STATED (as you stated it, ...) reads "..."`, and the two withdrawal headings
(`planContradictedBlock`, `planAssertedAbsentBlock`) lose it with it.

**The parser is untouched** — accepting a second label is prose-shaped tolerance under invariant 000.
The pin set proves it: the exact wave-72 reply shape (`123v152` seq 81) is still `plan_line_missing`
and still charged off-protocol bytes; what changed is that the prompt no longer supplies the phrase
to copy. The quotation shape and the no-`<label>: <text>`-split property L11 established are pinned
unchanged.

*Prediction.* `YOUR PLAN:` at the head of a reply: 10 -> 0. `plan_line_missing` falls by roughly the
same 10 (3.8% -> ~3.3%); if it does not, the residual is the attackers seam's dropped label (BW M15),
which this change does not address and never claimed to.

---

## Weakest evidence
1. **N12(a)'s behavioural half is a prediction, not a result.** I proved the predicate was dead and
   that the two counters it gates read 0; I did NOT run a game in which a reserve row is declined and
   a later window is skipped or noted. Re-arming a window-skipping path off a static repro is the
   riskiest thing in this lane. If the next corpus shows `reserve_decline_windows_skipped` rising
   with no matching `noted`, the M9c half is still broken and I only fixed its gate.
2. **N12(b) is a reasoning-body classification done with keyword heuristics** (later-action words,
   "hold" mentioned) over 105 records, hand-checked on 6. The 101/105 and 95/105 figures are
   therefore approximate; the 70/105 zero-cost figure and the 233/105/81/47 split are exact record
   arithmetic and do not depend on the classifier.
3. **N14(b) is a gloss, not a forecast fix.** The trade tag still prices a ghostform creature's death
   as a plain death; I asserted, but did not test, that a reader that sees the counter's gloss on the
   same screen will not trade into it. A forecast that reads death-triggered counter effects is the
   real fix and is out of this lane's scope.
4. **N14(c)'s 58-of-295 count is over rendered PROMPT TEXT**, so it counts a row once per prompt it
   appears in, not once per distinct decision. The proportion is right; the absolute number is
   inflated by re-renders.
5. **N13's refutation rests on `log_window_kind`** being an honest seam label. If `land_drop` is
   under-assigned (some land windows landing in `unclassified`), the 31/325 is a lower bound on
   coverage — which would only strengthen the refutation, but the table's denominators would move.
6. **No corpus was run**, so every prediction above is unadjudicated and the suite/PARSETEST green is
   the only evidence that nothing regressed. The suite is ~1 month old and covers almost none of
   these surfaces.
