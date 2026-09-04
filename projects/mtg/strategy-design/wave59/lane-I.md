# Wave-59 lane I — K4, K5, K7 (three render verdicts that lie)

Base: master `35c9b85bf`. Branch `w59-lane-I`. ONE file touched:
`projects/mtg/src/AIPlayerGPT.cpp`. All three items are render/verdict surfaces — no engine
behaviour, no legal set, no row removed, no window closed, no cap. Every change adds a true
token or corrects a false one; nothing the model relies on is deleted.

## K4 — the cleanup-discard `{dead right now: ...}` badge folds PER SCOPE

**Site:** `discardBoardVerdictTag` (the #W57-C D8 rebadge). It folded `rowSaysNoOp` over the
WHOLE verdict string, but a verdict is a LIST of per-scope verdicts joined by `;`, each carrying
its own parenthetical qualifier. 11 of the corpus's 139 dead badges named a live kill
(wave-58 engine-seat HIGH-1): `123v126` s12 badged **Devour Flesh** dead because the SELF scope
read "targeting yourself does nothing" while the OPPONENT scope named Overgrown Battlement
dying; `123v130` s17/s18 badged **Tragic Slip** dead on its Morbid *qualifier* while the
operative `-1/-1` stood outside the parentheses. Both cards re-verified against their
primitives — the scripts are right, the badge was the liar, and the lie sits at a discard where
the seat is told the removal it is about to pitch does nothing.

**Shipped:** the rebadge calls lane C's public pure `AIPlayerGPT::verdictReadsZero` (the same
per-scope fold the cast-menu header uses) instead of `rowSaysNoOp`. The discard clause carries a
leading space that the helper's anchor does not, so the space is dropped before the fold.
`rowSaysNoOp` is unchanged — this only decides what it is applied to, exactly as lane C framed
it. A verdict that is not all-zero now prints as `{right now: ...}` instead of
`{dead right now: ...}`; no row and no tag disappears.

## K5 — the D7 floor form, and the trample cross-reference

**(a) `incomingCombatLine`'s non-exact branch (`AIPlayerGPT.cpp` ~15122).** `bestCaseDamage` is
a FLOOR on the damage that lands, so `myLife - bestCaseDamage` is a **CEILING on life** — and it
was printed with "or better", which asserts the opposite of what it proves. `162v152` seq 19,
turn 13, 18 life vs 5 attackers for 20: *"at least 15 of that lands ... you would be at 3 or
better"*. The seat's actual legal block landed it at **0** and it died that combat, on a line
that positively told it the floor was survivable. Two corrections, both pessimistic:
- the value is worded as the ceiling it is —
  `you would be at N AT BEST (no assignment of your blockers does better)`;
- a non-positive ceiling is a PROVEN death (a floor of D at life ≤ D means no assignment
  survives), so the branch now carries the same `; no block saves you` badge the exact branch
  has. The old branch claimed no death at all.

**(b) `zeroPowerBlockerTag`'s two TRAMPLE rungs.** They ended `- each A-line above says how
much`, but the trample carry-over is **not** on the A-line: `162v152` seq 19's A2 read only
`Sigarda, Champion of Light (4/4) deals 4`, while `2 tramples to your face` lives in **B2's own
per-attacker may-block note**. The seat summed the A-lines, missed the 2, computed "I survive
with 2 life" and died at 0 — a pointer to a fact that is not where it says it is, at the seam
that decided the game. Both trample rungs now point at this row's own `"may block"` note. The
non-trample rungs keep `each A-line above`: there the number IS the attacker's printed damage
and the A-line does carry it.

## K7 — both land-drop rows of the MDFC menu are named

**Site:** the CHOOSE_MENU decoration pass, beside the #W56-D (D8) back-face branch. The two land
drops on one menu were rendered by different emitters, so the BACK face carried the full wave-57
treatment and the FRONT face rendered as a bare `Play Land` (`146v123` seq 3: row 1
`Grimclimb Pathway [PLAY THIS AS A LAND: ... (taps for {B}) ... USES YOUR LAND DROP ...]`,
row 2 `Play Land`). Row 2 IS Brightclimb Pathway, which taps for `{W}` — no name, no colour, no
land-drop note. **32 renders corpus-wide, taken 13 times.** A rendered row that names nothing is
the silent-omission half of the trust doctrine; the gap here is *which land it is playing*.

**Shipped:** a new pure `mdfcLandFaceRow(menuLabel, faceName, faceMana, arrivalTag)` composes the
row from the SAME emitters the sibling row uses (`mdfcRowHead` + `mdfcLandPlayRowTag` + the D26
`mdfcLandArrivalTag`), and the front-face branch calls it. The row now reads
`Brightclimb Pathway (menu text: Play Land) [PLAY THIS AS A LAND: puts "Brightclimb Pathway"
onto the battlefield as a land (taps for {W}). It costs no mana and uses no stack, and it USES
YOUR LAND DROP for this turn.]`. Facts are read off the card the row actually plays (the shown
face by name via `MTGCollection()`, falling back to this hand card's own script). The engine's
`"Play Land"` token stays in the head so a bare-token echo still binds. Append-only: the option
vector, the answer INDEX and `req.optionTexts` (the staleness key) are untouched, and anything an
earlier pass appended to the row is preserved. Gate: `curFace` is non-empty only on the land-face
menu (`landFaceMenu && ctx->isLand()`), which is this shape and no other; with no face name to
state the emitter returns the engine's row untouched.

## RED verification (measured, not asserted)

The final tree was rebuilt with **only the four implementation edits reverted** (the K4
predicate, the K5(a) branch, the two K5(b) strings, the K7 branch) and the full new PARSETEST
corpus kept. Result: `3466 passed, 13 failed` — the 3 superseded wave-58 CHECKs plus 10 of the
new ones (log `~/.gatelogs/w59-I-RED.log`). The tree was then restored and rebuilt.

⚠ **K7's CHECKs were GREEN in that RED run** and are green on base, because they exercise the
pure emitters, not the branch. That is why the composition was factored into
`mdfcLandFaceRow`: the tests now bind to the exact string the shipped branch emits. They still
do **not** prove the branch FIRES on a live menu — see "not verified".

Three pre-existing expectations were UPDATED, never deleted (each pinned the defect):
`#W57-B D24`'s floor case and `#W58-B D7`'s `126v152` case carried `-5 or better` / `-1 or
better`, and `#W58-B D7`'s NEGATIVE asserted the floor form never claims a death — at life 4
against a floor of 4 that death is provable, and withholding it was the deck162 defect.

## Gate (hermetic `rm -f bin/wagic && make -f Makefile.sdl -j4`, clean link; both legs detached under `MemoryMax=4G MemorySwapMax=0`)

| | base (brief) | lane I |
|---|---|---|
| PARSETEST | 3455 passed / 0 failed | **3480 passed / 0 failed** (+25) |
| suite `==Test Failed !==` @ THREADS=1 | 0 | **0** |
| suite `==Test timed out` | 0 | **0** |
| suite total | 1245 | **1245 (0 failed)** |
| AI tests | 61 / 0 | **61 (0 failed)** |
| `git diff \| grep -c $'\357\277\275'` | — | **0** |

Logs: `~/.gatelogs/w59-I-parsetest.log`, `~/.gatelogs/w59-I-suite.log`, `~/.gatelogs/w59-I-RED.log`.

## Predictions (falsifiable, for the wave-59 corpus)

- **K4** — `{dead right now: ...}` badges whose verdict names a live magnitude in ANY scope:
  **0 of N** (wave 58: 11 of 139). The count of dead badges should fall by roughly those 11 and
  no more; a badge that disappears from an all-zero verdict (a Damnation at `destroys 0`, an
  edict at N=0) is a lane break. Devour Flesh and Tragic Slip rows naming a real body now render
  under `{right now: ...}`.
- **K5** — `... or better` on any `INCOMING THIS COMBAT` line: **0 of N**. Every rendered
  `at least D of that lands ... you would be at N AT BEST` with `N <= 0` carries
  `; no block saves you`: **N of N**; and every such badge sits on a window the seat did in fact
  die to unless it acted outside blocking. Zero renders of `each A-line above says how much` on a
  rung that also says `except what a trampler pushes through`.
- **K7** — bare `Play Land` rows (a row whose whole text is the token): **0 of N**
  (wave 58: 32 renders, 13 taken). Every `Play Land` row carries a printed face name and a
  `PLAY THIS AS A LAND` tag, and the sibling back-face row's tag is byte-identical in shape.

## What I did NOT verify

- **No live run of any kind.** No corpus game, no probe deck, no Spark call, no translog. All
  three items are verified by PARSETEST over pure functions plus the composed row strings, and by
  the RED/GREEN pair above. The suite is a no-regression gate here, not a proof: the harness has
  no GPT seat, so none of these strings is rendered in a suite game and **no suite fixture is
  reachable for any of the three** (same reachability limit lane B recorded).
- **K7's branch is not proven to FIRE.** `mdfcLandFaceRow` is pinned, but the gate
  (`curFace` non-empty, `req.optionTexts[i] == "Play Land"`) and the `MTGCollection()` lookup of
  the shown face are exercised only by a live MDFC land menu. If the front face's own card record
  is not found or is not a land, the arrival clause falls back to this hand card's `magicText`,
  which I did not check against a flipped-face instance.
- **K7's scope.** The corpus's 32 bare `Play Land` renders were not individually classified; I
  fixed the shape the deck146 repro shows (the modal-DFC land play menu). If any of those 32 came
  from a menu where `ctx->isLand()` is false, that render is untouched.
- **K4's blast radius is measured only by the seat's census, not re-derived.** I did not replay
  the wave-58 discard prompts through both predicates the way lane C did for its header, so
  "11 badges flip" is the reviewer's count, not mine; and I did not check whether any of the 11
  actually changed a discard.
- **K5's `AT BEST` wording is untested against a model.** Whether the pilot reads a life ceiling
  correctly is a seat-review question this lane cannot answer; the claim shipped here is only
  that the sentence is now true.
- **The trample pointer assumes the B-row's may-block note is present** on the row that carries
  the tag. That is how the corpus renders it (`162v152` seq 19), but I did not prove the two are
  emitted together in every code path.
- **Nothing outside `AIPlayerGPT.cpp` was touched** — no Res asset, no primitive, no header, no
  fixture, and no other lane's item.
