# Wave-81 lane DL — verdict truth (V5, V7, V8, V10, V13-spare, V17-gap)

Base `4df71b19b`, branch `w81-lane-DL`. Corpus of record:
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final`.

Gate: suite THREADS=1 **1296 tests (0 failed), 76 AI tests (0 failed)** (both failure
strings counted, 0 each); PARSETEST **7161 passed, 0 failed** (base 7102, +59);
`check-ctor-init` OK; `check-reply-instructions` OK (37 guides); `git diff | grep -c
U+FFFD` = 0; `reply-protocol.txt` / `kReplyProtocol` untouched (diff --stat empty for
that path). Final gate ran after `make -B`.

---

## V5 — one total for the crack-back verdict — **CONFIRM**

**RED (live record, rebuilt through the live composition).**
`1789216918-ai_baka_deck130-0x559161469ed0` (`152v130`) seq 17, life 20. The prompt's
own CRACK-BACK NEXT TURN line ends

```
... ADD THOSE UP: the numbers named there come to 5 more damage, so the total to
subtract from your life is 20 and you would be at 0; that would KILL you, and lower still
```

and 15 lines below it, in the same prompt:

```
[crack-back verdict: you survive - ... the crack-back alone does not end the game]
```

The verdict is `w80CrackBackVerdictLine(crackBackVerdictNow(), ...)`
(src/AIPlayerGPT.cpp:36754 on base) and `crackBackVerdictNow()` is a threshold over
`crackBackTotalOver` — the RAW combat sum. `crackBackFloorSources`' addenda (the
animated Lair of the Hydra and Luminarch Aspirant's trigger power) reach the header at
src/AIPlayerGPT.cpp:31975 and never reach the verdict.

Second half, the compulsory draw: `1789241587-ai_baka_deck123-0x55c06425fcf0`
(`123v162`) seq 97, **life 1**, prompt carries
`DRAW FORECAST: ... = 9 life LOST BY YOU ... you would be at -8; that KILLS you. This
draw step is COMPULSORY - no row on any menu declines it.` and, beside the rows,
`[crack-back verdict: their UNBLOCKED total reaches your life total, but a legal block
prevents it - with your best block: 0 damage still gets through, leaving you at 1. So
passing this window does NOT hand them the game ...]`. DH F9's optional/forced split
reached the BLOCKERS verdict (src/AIPlayerGPT.cpp:58999 on base) and nothing else.

**FIX.** `w81CrackBackVerdictLine(face, rawCombat, addUp, compulsoryDraw, bestBlockFloor,
myLife)` recomputes the verdict from the figure the SCREEN publishes — raw combat + the
header's own ADD THOSE UP addenda + the compulsory draw-step charge — and NAMES the three
parts inside the bracket ("ONE TOTAL: this verdict uses N - the R from combat plus ...").
The best-block floor is raised by the same addenda (an animated land and a ping are not in
the DP's attacker set; a compulsory draw is not blockable at all), so "blocked away" is now
a claim about the published total. The draw walk that the blockers header used inline was
extracted to `w81CompulsoryDrawStepLoss` and BOTH callers now read it, so the two screens
cannot compute two numbers. `w81CrackBackVerdictLineNow()` supplies the addenda from the
same `crackBackFloorSources(opponent(), &extra, &unsized)` call the header makes; both
render sites (ask seam, cast seam) switched to it. With no addenda the wave-80 line is
returned byte for byte. The hold latch's MARKER (`crackBackVerdictKey`) is untouched.

**GREEN.** 15 pins in `[#W81-DL V5]` + `[#W81-DL V5] KEY`:
`152v130` seq 17's header really does publish a KILL; the wave-80 verdict says `you
survive` on it (RED); the new line does not; it names the total; `123v162` seq 97's
compulsory-draw board flips from `a legal block prevents it` (RED, pinned on the wave-80
function) to `LETHAL EVEN THROUGH YOUR BEST BLOCK` with the 12 named; the blocked-away face
survives where it is true of the published total; ZERO (no addenda -> byte-identical to
wave 80), ONE (a single point prints its own total), purity, and MUST-NOT-MATCH (a `none`
face manufactures no verdict). KEY: the pair differing only in the addendum keys EQUAL on
`optionSetKeyOf`, on `holdActionKeyRow` per row of the menu (the way `mLastMenuRows` feeds
the live seam), on `w77KeyTailOf` and on `asyncSlotKeyOf`; the verdict line is not a
numbered row (`w79NumberedRowBody` false), and the live seam builds `keyTailStr`
(src/AIPlayerGPT.cpp:46715) BEFORE any seam note is appended.

**PREDICTION.** `crackback_verdict_folded_total` > 0 in the next corpus's gameend census,
with per-record field `w81_render_events` containing `crackback_one_total_folded` on every
window it counted (incremented inside `w81ApplyRenderEventsAtSend`, called from
`w80ApplyVerdictFacesAtSend` — the same send boundary wave 80's faces use). Falsifier:
ANY prompt where `[crack-back verdict: you survive` and a CRACK-BACK NEXT TURN line
reading `that would KILL you` appear together — predicted 0.

---

## V7 — the own-loop starter — **CONFIRM**

**RED.** Corpus census over all 42 seat logs: 18 `It starts from <X>:` lines —
**Perimeter Captain 16, Pride Guardian 1, Exquisite Blood 1**. Ten of the sixteen are
`1789216933-ai_baka_deck126-0x55559c0ca2d0` (`126v130`) seqs 13/15/21/22/23/29/32/38/39,
the game deck126 lost at 0 life on turn 22. Oracle/script check
(bin/Res/sets/primitives/mtg.txt): Perimeter Captain `auto=@combat(blocking)
source(creature[defender]|mybattlefield):may life:2 controller`, text "Whenever a creature
you control with defender blocks, you may gain 2 life"; Pride Guardian
`auto=@combat(blocking) source(this):life:3 controller`. Both fire only if the OPPONENT
attacks into them. `126v130` seq 39's own prompt says
`CRACK-BACK NEXT TURN: ... 6 from 1 attacker nothing you control can legally block`
beside `It starts from Perimeter Captain`. On base `w80StarterLineKind` classified both as
`kW80StarterCycleTrigger` and `w80StarterIsLive` returned true unconditionally
(src/AIPlayerGPT.cpp:36214/36222 on base).
The Exquisite Blood line (`126v123` seq 16) is the HAND branch: it excluded nothing, while
the battlefield branch already excludes `w77IsLoopComponentCard` — and Exquisite Blood
(`@lifelostfoeof(player):life:thatmuch controller`) is one of the loop's own two halves.

**FIX.** Two new starter kinds. `kW81StarterOpponentTrigger` — an event whose text carries
`blocking` / `blocked` / `blocks` / `attacked` (`w81StarterEventNeedsOpponent`) — is NEVER
live. `kW81StarterAttackTrigger` (`attacking` / `attacks`) is live only while that body can
actually be declared an attacker now (`c->canAttack()`, the engine's own verdict). The hand
branch now skips a card whose script is a loop half (`lifeToDamageConverterScript` /
`lifeLossMirrorScript`, the two predicates `w77IsLoopComponentCard` uses — it asks the
battlefield question and returns false for a hand card).

**GREEN.** 8 pins in `[#W81-DL V7]`: Perimeter Captain's and Pride Guardian's verbatim
script lines classify as opponent-dependent and are not live whatever else the board
affords; the wave-80 classifier's verdict on them is pinned as the RED; the three
opponent-dependent spellings match and a turn-cycle event, an ETB and `@combat(attacking)`
do NOT; an attack trigger is live only with a body that can attack; the wave-80 kinds keep
their wave-80 verdicts.

**PREDICTION.** `It starts from Perimeter Captain` and `It starts from Pride Guardian`:
**0** occurrences in the next corpus (falsifier: any occurrence). `[own loop verdict:
proven but no live starter]` faces **> 0**, counted by the existing
`own_loop_verdict_lines_rendered` and traced per record by `own_loop_verdict`.

---

## V17-gap — the gap branch — **CONFIRM (untested branch, now driven)**

**RED.** `proven but no live starter` rendered **0** times in the wave-80 corpus (engine
seat §2(f)); DH F6 shipped the branch and nothing ever entered it.

**FIX.** V7's two gates are what put `126v130` seq 39's board into it. That board
(prompt line 175): Perimeter Captain #1/#2, Overgrown Battlement #1/#2, Chromatic Lantern,
Pride Guardian, Sanguine Bond, Exquisite Blood; hand: Idyllic Tutor x2, Exquisite Blood,
Wall of Omens. Sanguine Bond + Exquisite Blood are the chain's own halves (excluded on the
battlefield by construction, and now in hand too); the Captains and the Guardian are
opponent-dependent; the Battlements, the Lantern and the Tutors carry no life term at all.

**GREEN.** 5 pins in `[#W81-DL V7/V17]`: the script test rejects the battlements/lantern/
tutors; it does NOT reject the second Exquisite Blood (the RED the hand-branch exclusion
answers); the gap line prints, says `no live way to start it` / `NOT yet a win`, contains
neither `proven win` nor `It starts from`; and `w80LoopFaceOfLine` stamps
`[own loop verdict: proven but no live starter]` so the branch is countable.

**PREDICTION.** See V7 — the gap face > 0, `It starts from Perimeter Captain` = 0.

---

## V8 — the X markers — **CONFIRM (both halves)**

**RED.** `1789216937-ai_baka_deck130-0x562b68836ca0` (`162v130`) seq 75, row 1:

```
1. Cast Starstorm {r}{r}{x} {X pricing: max affordable X=6 ...} [<- best X for this cast:
X=6 - the only X that kills anything of THEIRS costs you more of YOURS than it takes -
THEIRS 4 / YOURS 5] ... [<- board sweep: THEIRS 4 / YOURS 5 - the only row on this menu
that prices a board sweep (it takes more of YOURS than of THEIRS)]
```

Both halves in one row. (a) `xCastRowBestXMarker` gated the `best X for this cast:` prefix
on `mk != kXNoKillsMarker` only, so the X menu's LOPSIDED verdict — a refusal — was
reopened as a recommendation. (b) `xSpellPricing` fills `ans->theirs/mine` and left
`mineNames`/`mineEngines` empty, so U7's roster never reached the X-variable path (the same
shape at `130v152` seqs 49-75).

**FIX.** (a) the prefix is additionally gated off any marker carrying `costs you more of
YOURS than it takes` — a refusal is carried across bare, exactly as the no-kill verdict
already was. (b) `xSpellPricing` now builds `mineNames`/`mineEngines` from this row's own
victim list at this row's own marked X, using U7's two predicates (`sweeperVictimName`,
`engineKindForScript`), so both sweeper paths word the same fact identically.

**GREEN.** 11 pins in the two `[#W81-DL V8]` blocks. The refusal half is driven through the
LIVE `xCastRowBestXMarker(sv, ...)` on an `XVictimSurvey` rebuilt from seq 75's board (4 of
theirs, 5 of mine, maxX 6): RED is the wave-80 composition (`xCastRowMarkerFrom(mk, mx, mk
!= kXNoKillsMarker)` -> ` [<- best X for this cast: X=`), GREEN carries the refusal first
with both counts; a cast the ladder does NOT refuse still names its best X (the wave-76
lesson: pin that the positive clause still prints); the no-kill marker is unchanged. The
roster half pins the bare wave-80 marker (RED, byte-exact), the rostered one, and the
send-time predicate `w81XSweepMarkerNamesOwnLosses` positive/negative/not-an-X-row, plus
ZERO (nothing of YOURS -> no roster, wave-66 bytes).

**PREDICTION.** `x_cast_row_refusal_markers` > 0 AND `x_sweep_roster_markers` > 0 in the
gameend census, per-record trace `w81_render_events` = `x_cast_row_refusal_marker` /
`x_sweep_roster_marker`, both counted at the send. Falsifier: any prompt where
`best X for this cast` and `costs you more of YOURS than it takes` occur in the same row —
predicted 0.

---

## V10 — render placement and the cover clause — **CONFIRM (both halves)**

**RED.** `1789216925-ai_baka_deck146-0x55be71935390` (`146v162`) seq 52, prompt lines
268-269:

```
A1. Silverquill Silencer #1 (3/2) [indestructible] [held back, THIS creature could not
block ANY of their 1 creatures: Fog Bank (flying)] [their untapped blockers: Fog Bank
(0/2) (neither dies ...)]
```

245 bytes per row, most of it a fact about NOT attacking, inside the line that offers the
attack (`ln << hb;`, src/AIPlayerGPT.cpp:57621 on base). Second half: the
`CRACK-BACK COST OF ATTACKING` paragraph's gate was
`crackBackScreenTotal(...) && life - cbTotal <= 0 && !attackers.empty()`
(src/AIPlayerGPT.cpp:58335 on base) — it fired only where the crack-back had already won,
so 1 of deck152's 8 crack-back attack prompts carried it and `152v146` seq 19 did not.

**FIX.** The hold-back tag is captured into `heldBackLine` and emitted after every in-row
annotation as `"\n   " + tag` — a line of its own under the row, nothing deleted, and the
reserved `aRowLoopAt` byte position (taken earlier in the row) is unaffected. The cover
gate becomes `w81AttackCoverDue(crackBackScreenTotal(...), !attackers.empty())` — exactly
the line's own gate; the STAY HOME tag beneath already prints its own survival verdict from
the same arithmetic, so the non-lethal face is a number, not a scare.

**GREEN.** 9 pins in the two `[#W81-DL V10]` blocks: the wave-80 row is one line with the
tag inside it (RED); the rendered row driven through the LIVE `joinBlockerRows` emitter has
no `[held back` on its offer line and carries `\n   [held back, THIS creature could not
block` beneath; the tag itself is byte-identical; ZERO (nothing restricted -> no line).
The gate pins both negatives and the positive, and pin the stay-home paragraph's
non-lethal arithmetic (`covers 6 of that 6, leaving 0 -> you would be at 20, which you
SURVIVE`) so printing it on a non-lethal board claims nothing new.

**PREDICTION.** `attackers_crackback_cover_clauses` > 0 and EQUAL to the number of
`attackers` records whose prompt carries `CRACK-BACK NEXT TURN:`; per-record trace
`w81_render_events` = `attackers_crackback_cover_clause`, counted at the send. Falsifier:
any `A<n>.` line in any prompt containing `[held back` — predicted 0.

---

## V13-spare — the cleanup `{spare:}` verdict is colour-aware — **CONFIRM**

**RED.** `1789216937-ai_baka_deck162-0x562b6751c5a0` (`162v130`) seq 47, cleanup, discard 2
of 9. Rows 1 and 2:

```
1. Dismal Backwater (land) ... {spare: you control 5 lands already; the most expensive
card in your hand you could still reach costs 3}
2. Swamp (land) {card text: B} {spare: you control 5 lands already; ...costs 3}
```

The hand holds **three Underworld Dreams at {B}{B}{B}**; the battlefield's black sources
are Drowned Catacomb and one tapped Swamp — **two**. Both rows that could fix that are
marked spare, and the header above them says "BY DEFAULT, discard the spare land."
`discardSpareLandClause` (src/AIPlayerGPT.cpp:60896 on base) compares only COUNT against
the top converted cost.

**FIX.** The clause takes three more facts — the colour this land makes that the hand is
short of, how many of that colour a single reachable card demands, and how many LAND
sources of it the battlefield holds. When the demand exceeds the sources the verdict WORD
is withheld and the two numbers print as a bare fact ("COLOUR: a card in your hand you
could still reach needs 3 {B} and discarding this row leaves you 2 LAND sources of it
(lands only - any nonland producer you control is not in this count)"). Nothing is hidden,
no row loses its count, only the word is conditioned — the #W61-T (C11) shape. The two
tallies are built with `landColorFlags` (the same engine-read the hand header and the
per-card land tag print from) and `ManaCost::getCost`, over cards inside the existing reach
cap.

**GREEN.** 6 pins in `[#W81-DL V13]`: the wave-80 clause on seq 47's numbers, byte-exact
(RED); the verdict word withheld when the colour binds; the count and the two colour
numbers both still printing; ZERO (enough sources -> wave-80 clause byte for byte);
MUST-NOT-MATCH (no colour demand, or a land making no needed colour, changes nothing);
ONE (singular spelling); and the count gate still refusing the word on its own.

**PREDICTION.** `spare_colour_verdict_withheld` > 0 in the next corpus's gameend census,
per-record trace `w81_render_events` = `spare_colour_withheld`, counted at the send.
Falsifier: any cleanup land row marked `{spare:` whose prompt shows a reachable hand card
needing more of that land's colour than the battlefield supplies — predicted 0.

---

## Weakest evidence

The one thing I could NOT drive through the live path is a real BOARD. Every RED here was
read out of the corpus prompt the seat actually received, and every GREEN is pinned on the
rendered bytes — but PARSETEST has no `GameObserver`, so five of the six fixes are pinned at
the last pure layer before the board, not at the board itself. Concretely: V5's pins drive
`w81CrackBackVerdictLine` with the three numbers taken off the record, and the composition
`w81CrackBackVerdictLineNow()` performs — `crackBackTotalOver`, `crackBackFloorSources`
(and its `floorSrc.empty()` guard), `w81CompulsoryDrawLossNow`, `w80CrackBackBestBlockFloorNow`
— is verified only by reading it, so a wrong reading of the header's addenda by
`crackBackFloorSources` on some other board shape would not be caught here. V7's pins drive
the classifier on the two verbatim primitive lines but not `w80LiveLoopStarterName`'s walk
over a real battlefield, so the `canAttack()` call and the hand-branch loop-half skip are
unexercised on a live zone; the gap line is pinned as a STRING, not as the output of that
walk. V10's held-back move is pinned through the live `joinBlockerRows` with a hand-built
rest, not through `chooseAttackers` itself, and the cover gate is pinned as the extracted
predicate `w81AttackCoverDue` — the `crackBackScreenTotal` half it is fed is unchanged but
unexercised. V13's clause is pinned with the colour numbers supplied by hand; the two
tallies that compute them (`landColorFlags` over the battlefield, `getCost` over the hand)
are not driven by any pin, so "the Swamp at `162v130` seq 47 loses its spare word" is an
argument from the code plus the prompt, not a measurement. V8's refusal half IS driven
through the live `xCastRowBestXMarker` on a rebuilt survey, which is the strongest of the
six; its roster half is pinned at `boardSweepMarker` and at the send-time predicate, not
through `xSpellPricing` over real `MTGCardInstance` victims (the pin cannot build one), so
the `v.inst` walk I added is unverified except by inspection. Finally, none of the five new
counters has been observed non-zero: they are predictions, and the wave-79 lesson says a
counter that reads 0 after the fix is a FAIL, not "untested" — I have wired each to a named
per-record field (`w81_render_events`) so the next census can adjudicate them, but I have
run no corpus (no lane sends requests to the pilot this wave) and therefore have no evidence
that the staging flags reach `askModel` on the attackers and discard seams, which is the
single most likely place for this lane's work to be silently dead.
