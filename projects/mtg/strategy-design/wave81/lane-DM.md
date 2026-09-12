# Wave-81 lane DM — engine rules, echo, intel and bytes (V6, V11, V12, V13, V14, V16, V17, V2-residual)

Base `4df71b19b`. Worktree `worktrees/lanes/w81-DM`, branch `w81-lane-DM`.
Evidence corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final` (2,135 prompts, 21 games).

**GATE.** Suite THREADS=1 as the detached unit `w81-DM-suite`: **1,298 tests (0 failed), 76 AI tests (0 failed)** —
baseline 1,296/0 + 76/0, grown by this lane's two V6 fixtures, `==Test Failed !==` 0 and `==Test timed out` 0.
PARSETEST **7,186 passed, 0 failed** (baseline 7,102/0, +84). `check-ctor-init` OK, `check-reply-instructions`
OK (1 source, 37 guides), `selfplay-harness.sh --selftest` 0 failed across its four blocks, `corpus-stats.py
--selftest` OK, `prompt-byte-census.py --selftest` OK, `regime-gate.py --selftest` 34/0,
`runmanifest.py --selftest` OK (new this wave). `git diff --stat -- projects/mtg/bin/Res/ai/gpt/reply-protocol.txt`
is EMPTY; the only `kReplyProtocol` string in the diff is a comment saying it is untouched.
`git diff | grep -c $'\357\277\275'` = **0**.

---

## V6 — Exquisite Blood and life lost to DAMAGE: **REFUTE** (engine and render both correct)

**The Oracle, fetched 2026-09-12 from Scryfall** (`/cards/named?exact=Exquisite+Blood`,
oracle_id `8f933fae-6c0c-42d7-a817-14760d8285cd`):

> **Exquisite Blood** — `{4}{B}` Enchantment — **"Whenever an opponent loses life, you gain that much life."**

and its partner (`/cards/named?exact=Sanguine+Bond`):

> **Sanguine Bond** — `{3}{B}{B}` Enchantment — **"Whenever you gain life, target opponent loses that much life."**

Both card scripts are faithful. `mtg.txt:37966` is
`auto=@lifelostfoeof(player):life:thatmuch controller` with the modern `text=` verbatim; `mtg.txt:99004` is
`auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent`, also verbatim. **Nothing is owed on the
card side.**

**The engine finding.** `Damage::resolve` (Damage.cpp:299) raises `WEventLife((Player*)target, -damage, source,
/*fromDamage=*/true)` for **every** player-target damage — the `typeOfDamage == 1` branch above it only does prowl
and commander bookkeeping and does not gate the event. `TrLifeGained::triggerOnEventImpl` (AllAbilities.h:1626)
filters on the event's **sign** and the two side flags (`thiscontroller` / `thisopponent`) and **never reads
`fromDamage` or the damage type at all**. So combat damage and spell damage take the identical path: damage to a
player IS life loss to this engine, and `@lifelostfoeof(player)` fires on it. #W62-Y already pinned the
SPELL-damage half (`w62y_lifelost_damage_fires.txt`); the COMBAT half V6 named had no fixture, and now does.

**The corpus record is the wrong-SIDE case, not a missing trigger.** In `130v126` **both** loop halves are on the
**opponent's** battlefield (deck130's own prompt at seq 42: `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs
- Sanguine Bond`). deck130 dealt 11 damage **to** deck126. Their Exquisite Blood reads "whenever an **opponent**
loses life" — its opponent is deck130, who lost nothing — so it must not fire, their Sanguine Bond never gets a
gain event, and **zero triggers on 11 damage is rules-correct**. The wave-80 deck130 review's premise
("Exquisite Blood should have triggered three times") is the side error, not the engine's.

**And the render was already right.** `130v126` seq 42 carries, in full:

> `LOOP COMPLETE: BOTH halves of a life LOOP (Sanguine Bond + Exquisite Blood) are on THEIR battlefield right
> now. Any life YOU lose, and any life THEY gain, chains until you are at 0 - so every life payment on this
> screen is fatal rather than expensive. **Their chain starts only from life YOU lose or life THEY gain - life
> THEY lose does not enter it.**`

The seat's reply on that same window was `PLAN: Cast Hammer of Bogardan targeting the opponent to trigger the
Sanguine Bond/Exquisite Blood infinite loop and win the game.` — it read the last sentence and answered against
it. The seat's EARLIER reasoning (declining the Talisman, turns 14-20) was **correct** Magic and the guide's
TALISMAN rule firing: self-damage from the Talisman IS life the seat loses, which is one of the two entries that
chain that clause names. So the six turns of paralysis were right, and the seq-42 cast was wrong for a reason
the screen had already stated. **No engine change and no render change ships for V6.** The residue is a
guide/strategy surface (deck130), not an engine one.

**GREEN (new, RED-free by design — the item is a REFUTE).** Two suite fixtures, registered in `_tests.txt`
immediately after the #W62-Y pair:
* `w81dm_lifelost_combat_damage_fires.txt` — P1 holds Exquisite Blood and attacks with a 3/3 into an empty
  board; P2 loses 3 to **COMBAT** damage; P1 must be at 23 and P2 at 17. **PASSES on base**, which is the
  finding: the combat path was never broken.
* `w81dm_lifelost_combat_damage_wrong_side.txt` — the `130v126` board verbatim (Exquisite Blood **and** Sanguine
  Bond on P2, P1 attacks P2 with a 3/3); P1's life must be **unchanged** at 20 and P2 at 17. **PASSES on base.**

Both are **bounded structurally, not by a step cap**: in the first there is no Sanguine Bond at all, so no
payload can re-enter; in the second the only Exquisite Blood is on the side that never gains, so the chain has no
entry. Neither fixture can loop, and the pre-existing `exquisite_blood_i953.txt.disabled` (a REAL loop, disabled
in 2026-07 for a game-end timing gap) is untouched.

**Prediction (falsifiable).** Next corpus: on any game where a seat controls Exquisite Blood and that seat's
opponent takes combat damage, the seat's life rises by exactly that damage — measured as: for every narration
line `- Your <X> dealt N damage to the opponent` on a seat whose own battlefield line prints Exquisite Blood, a
life-gain of N on the same seat in the same turn's events. Zero such gains on a game that has the pair on the
CONTROLLING side is a FAIL. (The wave-80 population is the mirror of this and is now pinned as correct.)

---

## V16 — the echo heading stops naming a plan in caps: **CONFIRM**, fixed

**RED.** deck125 MED B-1: all four of that seat's protocol deviations are the collision between the render
heading `THE PLAN YOU LAST STATED` and the protocol's own label `PLAN:` — `125v126` seqs 164/165 and `125v152`
seq 63 answer with a label slot reading `THE PLAN: ...`, and `152` seq 111's entire reply is the render's own
sentence echoed back (`THE PLAN YOU LAST STATED reads "End turn."` + `CHOICE: 6`). #W73-CA had already
established the mechanism and applied it once: what the model copies into the label slot is the **caps phrase
that names a plan**, not the punctuation — it removed `YOUR PLAN` for exactly this and the carry heading kept a
second one.

**Fix (RENDER TEXT ONLY).** One constant, `kEchoHeadWord = "WHAT YOU LAST WROTE"`, now feeds all three faces
that carried the phrase — the carry heading and both withdrawal blocks (`planContradictedBlock`,
`planAssertedAbsentBlock`) — so no face can drift back. The carry reads
`WHAT YOU LAST WROTE (as you wrote it, 1 window ago on turn 10) reads "<plan>"`. The #W73-CA quotation shape (a
verb, then the plan in quotes, never `<CAPS>: <text>`) is preserved on every face. The golden protocol
(`reply-protocol.txt` / `kReplyProtocol`) and the parser are untouched; no option is removed, capped or
auto-answered.

**GREEN.** `#W81-DM V16` — the three existing header pins re-asserted on the new bytes, plus MUST-NOT-MATCH that
the token `PLAN` appears on **none** of the three faces (the item's own criterion; `PLAN:` was already absent
since #W71-BR L11, and it is the bare word that produced `THE PLAN: Hold priority...`), a REPRO pin that
`152` seq 111's echoed sentence is no longer on the screen to copy, a POSITIVE that the quotation shape survives,
and a pin that all three faces render the one constant.

**Prediction (falsifiable, NON-ZERO on the negative side).** Next corpus: `THE PLAN:` as a reply's label slot
= **0**, and replies whose whole plan line is the render's heading sentence = **0** — measured over every reply
carrying `protocol_deviation` or `unlabelled_plan`. Any non-zero is a FAIL. The positive half: `plan_line_missing`
at deck125 falls from 4/4 deviations-are-this-collision toward the corpus rate. A corpus in which deck125's
deviations are still this shape is a FAIL, not "untested".

---

## V11 — the plan echo drops a step the current window is executing: **CONFIRM**, fixed

**RED, byte-exact, through the live entry `w80CarriedPlanSteps`:**
* `152v125` seqs 15 → 16. Plan `Cast Intrepid Adversary and add 1 valor counter. Attack with Elite Spellbinder
  only to survive Lightmine Field. Tap Katilda for mana in main 2.` The valor-counter menu — **the window that
  adds the counter** — echoes `"Attack with Elite Spellbinder only to survive Lightmine Field. Tap Katilda for
  mana in main 2."`: the cast clause AND this window's own step, both gone. Pinned byte-exact as
  `#W81-DM V11 RED-ON-BASE`.
* `152v126` seqs 19 → 20. `Cast Brutal Cathar to exile a Perimeter Captain, then level Ranger Class to 2 with the
  remaining {G}.` echoes at the ETB **target** window as `"then level Ranger Class to 2 with the remaining {G}."`
  — the clause naming the target, dropped at the window that picks the target. Pinned byte-exact.

**Diagnosis.** #W80-DG's step-back is gated on the cast latch, and #W80-DH F5 closes that latch when the spell
**reaches the stack**. Both of these windows arrive AFTER that close (the valor menu and the ETB target window
are resolution-time), so the latch is 0 and the pointer trims. The two wave-80 pairs that PASSED (`152v123`
20 → 21, `152v125` 21 → 22) differ only in **punctuation**: they wrote the cast and the counter as comma-joined
clauses of one sentence, which `planStepEnds` never splits, so nothing could be trimmed. **A sentence boundary is
not a fact about the game.**

**Fix — the latch keys on the OBJECT.** Before it trims, the carry asks one question: does the step the pointer
has just passed name an object **this window is about**? The object is matched by the name the two surfaces
share (`w81StepNameCandidates` takes capitalised runs of ≥2 words, or one capitalised word of ≥4 characters,
never the step's leading verb; `w81StepNamedByWindow` looks each up verbatim in the window's own tail), never by
the sentence, the punctuation or the latch state. **Bounded at ONE step**, so the carry can never walk backwards
through a plan: the window that executes a step is the one right after it.

**GREEN.** `#W81-DM V11` — both corpus pairs GREEN through the live entry with their own window tails, plus:
MUST-NOT-MATCH an unrelated window carries the wave-80 bytes; MUST-NOT-MATCH an empty tail reclaims nothing;
BOUNDED-AT-ONE (a window naming a card two steps back does not walk back twice); MUST-NOT-MATCH the step's
opening verb is never an object name; the two wave-80 PASS pairs are untouched; and the extractor takes both
objects of `Cast Brutal Cathar to exile a Perimeter Captain` and never `Cast`.

**Prediction (falsifiable, NON-ZERO).** Next corpus: **0** echoes at a window whose own subject is named by the
step immediately preceding the pointer and is missing from the echo — measured as: for every record whose prompt
names a card in its menu head or rows AND whose immediately-preceding record's plan named that same card, the
`WHAT YOU LAST WROTE` text must contain that card's name. Non-zero is a FAIL. Positive half:
`plan_cast_steps_closed` stays > 0 and the completing-menu echo population (18 prompts in wave 80) grows a
reclaim count > 0 on the valor/ETB shapes.

---

## V12 — a multi-row answer is silently reduced to its first row: **CONFIRM**, both halves fixed

**RED.** `152` seqs 217 and 237 (Emrakul's Annihilator, sacrifice 6): the replies are
`CHOICE: 1, 2, 7, 8, 9, 11 (Boulderloft Pathway, Forest, Plains #1, Plains #2, Plains #3, Hengegate Pathway)` and
`CHOICE: 5, 6, 7, 8, 9, 10 (...)`, both `parse_note: multi_answer_first_taken`. The parser keeps row 1, the model
plainly believes it chose all six, and the window's own sentence read only `Choose card 1 of exactly 6 from the
list below, and answer with the chosen card's row number.` — a sentence a reader can take **either** way (as "pick
the first of the six you are naming now", or as "this is the first of six windows"). It is the second. And
`multi_answer_first_taken` was **not in the gameend census**, so the corpus reader had to grep 2,135 records to
find the two.

**Fix, half 1 (render).** `buildHandRemovalAsk`'s **multi** branch now closes with
`THIS WINDOW TAKES ONE ROW NUMBER, NOT A LIST: you are asked once per card and this is window <i> of <N>, so the
rest of your picks are made at the windows that follow this one. If you write several numbers here, the FIRST is
the one that happens and the others are discarded.` The last clause states what the parser does, because a model
told only "one number" that still writes six loses five picks silently. **Nothing is removed, capped or
auto-answered**: every row stays, the answer is still the model's, and a multi-number reply is still read — it is
just described honestly. The single-pick branch and the entire gain builder are byte-identical to wave 80.

**Fix, half 2 (census).** `multi_answer_first_taken` is now a gameend field, incremented **at the site where the
note is stamped onto the record**, so the game total and its per-record trace (`parse_note` on that seq) are the
same event seen twice — the wave-79 LESSON applied to a counter that did not have it.

**GREEN.** `#W81-DM V12` — the REPRO ask built with the corpus's own arguments (Emrakul, sacrifice, exactly 6,
pick 0) pinned byte-exact; seq 237's window number follows the pick index; MUST-NOT-MATCH on the single-pick ask
and on the gain builder; an unlimited ask claims no window count it cannot compute; an ECHO pin that seq 217's
reply **still** parses to row 1 and **still** stamps the note (the parser is untouched, which is what makes the
new sentence true); and a KEY pin that the ask is pure over its arguments.

**Prediction (falsifiable, NON-ZERO).** Next corpus: `multi_answer_first_taken` appears as a **gameend field on
all 42 seats** (so a reader can divide by record count), and the per-record note count equals the sum of that
field across seats — a mismatch is a FAIL. Behavioural half: multi-row answers at the forced-loss seam fall from
2 toward 0; a corpus with 2 or more is a FAIL on the render half, not "untested".

---

## V13 — intel missing from the rows: **CONFIRM** on all three, fixed

### (a) the `TOKEN ENGINE` marker on the edict cast row and the forced player-target ask

**RED.** deck123 HIGH-2, a near-A/B inside one seat's six games. PRESENT at `162v123` seq 79 (the victim ask,
where the pick is a real choice): `Thraben Doomsayer #1 (2/2) ... THIS IS NOT JUST A BODY: TOKEN ENGINE (it makes
more permanents, one per activation)`. **ABSENT** at `125v123` seq 238, the **cast row**, which names the victim
outright — `YOU control 1 creature - targeting yourself sacrifices Bloodline Keeper, and you gain 3` — and
**ABSENT** at seq 239, the forced player-target ask, the last window before the seat's engine left the board for
good. deck123 Devour-Fleshed its only Keeper and spent 13 turns on an empty board.

**Fix.** `w81EngineBodyTail` runs the engine's **own** detector (`engineKindForScript`, the one the victim ask
and the sweep rows already use) over the name the row **already resolves**, and emits the **same literal**
(`; THIS IS NOT JUST A BODY: <kind>`) the pick surface prints, so the two can never disagree about one card. It
reaches three faces: `edictSelfClause` (the cast row's own half), `w77EdictPlayerTargetTag` (both player-target
rows), and the THEIRS half of the cast row — appended after the verdict, never inside the name. It fires only
where the victim is a single DETERMINED body; at N > 1 no row names a victim and there is nothing to mark.

**GREEN.** `#W81-DM V13` — both corpus windows pinned byte-exact; the marker proven identical to the victim
ask's literal; ECHO (the ask marker is inside the `{right now: ...}` group and strips out of the narrated
record); MUST-NOT-MATCH on a vanilla body, on N > 1, on the zero branch of both surfaces, and on a NULL card;
a symmetry pin that the opponent half of the ask takes the same marker; and a KEY pin that the marker bytes are
the script's, unmixed with any board number.

### (b) a `COUNTERSPELLS THEY HAVE USED` line

**RED.** deck123 HIGH-3, two makers dead in one game. `125v123` seq 143 (t22 main 1) carries
`Their untapped sources: 11 (colours they could make: {u}{w})` **and**
`Their graveyard (8 cards): Essence Scatter {1}{u} x2; Dream Fracture {1}{u}{u}; ...; Cancel {1}{u}{u}; ...`, and
**no rendered clause anywhere** said a counter was live. The seat cast Thraben Doomsayer into it; the Bloodline
Keeper went the same way at seq 190. The prompt already held both facts and already classifies cards by function
in four other places (`a DRAW PUNISHER`, `a TOKEN ENGINE`, `LIFE-TO-DAMAGE CONVERTER`, `a life-loss mirror`).

**Fix.** One line, emitted directly under the open-mana line whose number it is read against:

> `COUNTERSPELLS THEY HAVE USED: they have countered 2 of your spells this game, and their graveyard holds 4
> counterspells - Essence Scatter {1}{u}, Essence Scatter {1}{u}, Dream Fracture {1}{u}{u}, Cancel {1}{u}{u}.
> Read that against their 11 untapped sources on the line above. This is a record of what they have DONE and what
> is in a zone you can see; it is not a claim that they hold one now - their hand is not shown to you.`

The count is an **EVENT** count (`mOppCounteredSpells`, taken at the zone-change observer where the engine itself
decides a departure was a counter), never inferred from the graveyard — a counterspell can reach a graveyard
without countering anything. The roster is built with the engine's own `w79CounterspellScript` detector over a
zone the prompt already prints in full. The closing clause is the trust-doctrine boundary: "they can counter
this" is not derivable and is not printed.

**GREEN.** `#W81-DM V13` — the repro line pinned byte-exact; MUST-NOT-MATCH that an empty roster prints
**nothing** at any count; the zero-event face and both singular/plural agreements; MUST-NOT-MATCH that the line
never asserts a counter is held or live; ECHO (a plain narration line, no channel to strip); and the **key
pin**: the line is emitted into the BOARD stream (`serializeGameState`'s `out`), never into the option-list
stream `tail` that the ask key, the async slot key and the hold latch are all built from — pinned as "the option
list the key is built from cannot contain the clause or its number", which is the wave-74 LESSON's requirement
for a new board-derived number.

Two gameend fields ship with it: `opp_countered_spells` (the events) and `counter_intel_rendered` (the windows
the line actually reached, **incremented at the render**, wave-79 LESSON 1).

### (c) `NO LIVE CAST ROW` on dead-only menus

**RED.** deck126 MED-3, `126v130` seqs 15/21/22/32/38. Seq 32's whole menu is `1. Cast Idyllic Tutor {2}{w}
[finds only an enchantment card - every enchantment left in your library is a copy of one you already control or
hold: Sanguine Bond]`, a hold row and a decline row; seqs 22/38 add a `[second copy:` Exquisite Blood. The marker
printed on **none** of the five, while the same marker printed 25 times at that seat on menus dead in exactly the
same way. The seat had to classify them itself and got 3 of 5 right.

**Fix.** `everyCastRowDead` now also accepts a row this engine already marked dead with a BRACKET TAG, and only
the two faces whose text is a **dead claim**: `secondCopyTag`'s verdict-1 tail (`a second copy changes nothing`)
and `tutorFindsTag`'s two exhausted searches. The second-copy **stacking** face and the "adds only its own
abilities" face are both live rows and are deliberately excluded — a false header is the one failure this fold
has already had (#W58-C D2, 8 of 497 renders) and the miss direction is silence. `anyTagDead` widens only the
header's **opening clause** and only on a menu that carries such a row, so the 25 renders that already worked are
byte-identical to wave 80. The nothing-withheld clause and the #W68-BD scope sentence are untouched.

**GREEN.** `#W81-DM V13` — seq 32 and seqs 22/38 both pinned; the widened opening pinned byte-exact;
MUST-NOT-MATCH that a verdict-only dead menu is byte-identical to wave 80, that both live tag faces and a fresh
tutor never read dead, and that one live row still silences the header; plus purity and ECHO pins.

**Prediction for V13 (falsifiable, NON-ZERO, per-record traced).** Next corpus: (a) **0** edict cast rows or
forced player-target asks that name a single victim whose script `engineKindForScript` classifies and that carry
no `THIS IS NOT JUST A BODY:` marker — measured by joining each such rendered row against the named card's
script; (b) `counter_intel_rendered` > 0 on **every** seat whose opponent's graveyard held a counterspell, and
`opp_countered_spells` equal to that seat's count of `was COUNTERED` narration lines about its own spells — a
`counter_intel_rendered` of 0 on a game where the roster was non-empty is a **FAIL**, not "untested"; (c) **0**
menus whose every cast row carries a verdict reading zero or one of the two dead tags and whose prompt lacks
`NO LIVE CAST ROW ON THIS MENU` (deck126 MED-3's own falsification clause).

---

## V14 — bytes: **CONFIRM**, both halves fixed

### (a) the cover paragraph prints once per prompt

**RED.** `162v146` seq 30 is a 25,717-byte ask carrying the `{crack-back cover: ...}` paragraph **twice, byte for
byte identical, at 2,012 B each** — both rows add one body to the same board, so both paragraphs compute the same
cover against the same 17. That is 7.8% of the prompt for zero information.

**Fix.** `w81FoldDuplicateCoverParagraphs` runs on the settled menu copy, beside `applyDuplicateEffectTags` and
`applyBoardSweepMark`. A cover whose bytes are **exactly** those of one already printed on this menu is replaced
by `{crack-back cover: identical to the one printed in full on row N above - ...}`. Two covers that differ by
even one digit **both** print in full, because the difference IS the per-row delta. Nothing is removed, capped or
auto-answered; the fold is inside the `{...}` channel, so it strips out of the narrated record and stays outside
every key exactly as the paragraph it replaces did.

### (b) the narration budget is a function of the whole prompt

**RED.** `152v125` seq 215: 15,298 B of narration under a 6.8 KB `--- CURRENT SITUATION ---` block = 22,143 B,
and the growth trim is satisfied because 15,298 < 16,000. It never looks at the board. 83 of deck152's 85
over-20-KB prompts are that one 49-turn game.

**Fix.** The board block is built **first** in `assemblePrompt` (the emission order — log, then situation — is
unchanged; only the order in which the two strings are computed moves), and the log is then cut to
`w81NarrationBudget(everything else on this window)`: aim the assembled prompt at 20,000 B, give the log what is
left, never below a 6,000 B floor. The cut is the growth trim's cut — line boundary, oldest first, behind the
**same** marker built from the same four zone digests. A prompt already under the target trims nothing.
`WAGIC_GPT_TRIM_V1=1` disables it with the rest of the wave-79 pair, so "was it me?" is one env var.

**GREEN.** `#W81-DM V14` — 19 pins including both RED repros (the seq-215 log pinned as UNDER the growth
trigger, so the growth trim provably never fires on it; the seq-30 duplicate folded and the fold naming its
source row), MUST-NOT-MATCH that a prompt under the target trims nothing and that two DIFFERENT covers both print
in full, the floor behaviour, the disable flag, a no-line-boundary no-op, a KEY pin that the fold cannot move an
option-set key, and ECHO pins on both.

### The before/after census

`prompt-byte-census.py` **BEFORE** (its corpus-total line, which is new — see V17):

```
CORPUS: .../matchups-20260912-074153-final
  TOTAL   n=2135  mean  13130 -> 13130   >20KB  338 ->  338   hoist 0 B (0 prompts)   log trim 0 B
```

**AFTER**, the same 2,135 prompts with this lane's two byte fixes applied — arithmetic over bytes already on
disk, not a prediction about the pilot:

| deck | n | mean now | mean after | >20 KB now | >20 KB after | cover fold | log budget |
|---|---|---|---|---|---|---|---|
| 123 | 358 | 12,405 | 12,357 | 14 | **0** | 0 B (0) | 17,166 B (14) |
| 125 | 529 | 15,192 | 14,505 | 157 | **0** | 900 B (3) | 362,658 B (157) |
| 126 | 168 | 10,348 | 10,299 | 3 | **0** | 3,144 B (2) | 5,000 B (3) |
| 130 | 301 | 11,908 | 11,760 | 14 | **0** | 1,766 B (3) | 42,782 B (14) |
| 146 | 332 | 13,860 | 13,601 | 55 | **0** | 4,697 B (3) | 81,056 B (55) |
| 152 | 279 | 14,194 | 13,803 | 85 | **0** | 0 B (0) | 109,046 B (85) |
| 162 | 168 | 9,943 | 9,813 | 10 | **0** | 7,078 B (3) | 14,865 B (9) |
| **corpus** | **2,135** | **13,130** | **12,825** | **338** | **0** | **17,585 B (14)** | **632,573 B (337)** |

Corpus total 28,033,436 → 27,383,278 B (**−2.3%**, −305 B/prompt mean). MED-2 asked for the next byte item to be
scoped against the **+1,095 B** the wave added, not against the hold apparatus: this recovers 28% of that, and it
takes the share over 20 KB from **13.4% to 0%**, which is the figure MED-2 is actually about. Per U17 this buys
no latency and is not claimed to.

**Prediction (falsifiable, NON-ZERO).** Next corpus: prompts over 20,000 B **fall below 100** (from 338, 13.4%),
mean prompt bytes fall below 13,000, `{crack-back cover: identical to the one printed in full on row` renders
**> 0**, and the narration-budget marker appears on prompts whose log is under 16,000 B (a cut the growth trim
cannot make). A corpus where the fold clause renders 0 times is a FAIL, not "untested".

---

## V17 — three low items: **CONFIRM** on all three, fixed

**(a) `prompt-byte-census.py` crashed at line 297.** RED reproduced on the corpus: exit **1**,
`TypeError: 'int' object is not subscriptable` — the HOLD APPARATUS loop named its per-deck subtotal `tot` and
**shadowed the corpus-total dict** built at the top of the same function, so the per-deck tables printed and the
corpus line never did. Renamed to `holdTot`. GREEN: exit **0** and the corpus total prints (the BEFORE row
above). The tool already takes the run dir as an argument and echoes it on its first line, and `--selftest`
passes.

**(b) the `x<N>` exemplar goes neutral.** RED: 5 wave-80 prompts read
`e.g. "CHOICE: 19 (Create human with Thraben Doomsayer x<N>)"` — this row's own index and name, written as the
thing to copy — the seam #W80-DG U5 missed when it converted the ask seam and the ANNOUNCE_X template. The
example is now U5's own FORMAT literal, `"CHOICE: <row number> (<that row's short name> x<N>)"`, with a face
saying the two slots are placeholders and not a recommendation of any row. What the bracket is FOR (N is a digit;
the bare name is refused and re-asked) is untouched, and the row is still offered and still answerable exactly as
before. GREEN: the two wave-48/50 pins that asserted the live quotation are **inverted into MUST-NOT-MATCH**, the
"index follows the row" pin becomes "no index at all, at any index", plus a key-stability pin that the example
text is now the same bytes on every repeat row and a POSITIVE that the rule still prints in full.

**(c) a cycling row says it discards.** RED: all 9 priced cycling rows at `162v130` seqs 76-103 read
`[DRAW PRICE: this draws 1 card ...]` with **no discard term**, and `a CYCLING cost is a discard` rendered **0
times corpus-wide**, because #W80-DF U6 gated the whole discard clause on a discard **punisher** existing. A
punisher changes what the discard COSTS; it does not change whether the row discards. The mechanism clause now
rides every cycling row that **already** prints this tag, carries **no life figure of its own**, and says
outright that nothing on the board charges life for it (#W76 R3's rule). A cycling row WITH a punisher takes
#W80-DF's branch and is byte-identical to wave 80; a row that prints no tag at all still prints none, so this
adds no new brackets anywhere. GREEN: 8 pins including both MUST-NOT-MATCH directions and an ECHO pin that the
narrated row is the same with the clause and without it.

**Prediction (falsifiable, NON-ZERO).** Next corpus: `a CYCLING cost is a discard` renders **> 0** (wave 80: 0)
and `which nothing on this board charges you life for` renders > 0 on the unpunished boards; `x<N>` exemplars
quoting a live row = **0** and `CHOICE: <row number> (<that row's short name> x<N>)` renders > 0; and any harness
step gating on `prompt-byte-census.py`'s exit status sees 0.

---

## V2 residual — the degraded manifest path announces itself: **CONFIRM**, fixed

**RED.** On base `runmanifest.py` contains no `MANIFEST ABSENT` string at all
(`git show 4df71b19b:projects/mtg/tools/runmanifest.py | grep -c 'MANIFEST ABSENT'` = 0): an outdir with no
announcement channel fell back to scanning the shared `~/.Wagic/ai/gpt/logs` by time, and every consumer's output
looked identical to a manifest-keyed one. That is the exact silence the 2026-09-12 false regime gate cost a full
rerun attempt for.

**Fix.** `own_logs`'s degraded branch now calls `announce_degrade()`, which prints
`MANIFEST ABSENT - scanning by time` **before it returns anything** — on the **first line** of the degraded
read, on stderr so no consumer's parsed stdout changes, once per process (the six harness sites each run in their
own python). The EMPTY set is still "nothing announced yet" and is NOT a degrade.

**GREEN.** `runmanifest.py --selftest` is new this wave, 8 checks, 0 failed: the manifest-less outdir returns
None and its read prints the banner on its first line and returns every jsonl; the banner is once per process;
an announcing outdir keys by name, returns only the run's own log and prints **nothing**; and the empty manifest
selects nothing and does not announce a degrade. `regime-gate.py --selftest` (34/0) and
`selfplay-harness.sh --selftest` both still pass with the tool changed under them.

**Prediction (falsifiable).** Next corpus run: `MANIFEST ABSENT - scanning by time` appears **0 times** in the
harness's own stderr for a live run (every live outdir has its per-game `game-*.stderr` before the game starts),
and appears exactly once per process in any probe or selftest directory that has no announcement channel. A live
run that prints it is a real finding — it means a sweep read the shared directory — and is a FAIL.

---

## Weakest evidence

The weakest thing in this lane is that **every behavioural prediction above rests on pins and arithmetic, and not
one of these renders has been driven through a live game** — no lane sends requests to the pilot this wave, so
the fixes are proven at the helper and, for V13's dead-cast header and V14's cover fold, at the settled menu copy
one call short of the emitter. That is precisely the failure LESSON OF WAVE 78 names (a pin can pass while the
live path renders nothing, S6: pinned bytes, 0 renders in 42 logs), and three of this lane's items are exposed to
it in different degrees. V13(b) is the most exposed: its `counter_intel_rendered` counter exists *because* I
cannot demonstrate the line reaches a window, the emission site is inside `serializeGameState`'s opponent block
which I have not driven, and the counter's own value is the only thing that can adjudicate it — a 0 there is a
FAIL by the wave-78 rule and I am predicting a number I have not observed once. V14(b) is the next most exposed
and in a second way: it **reorders computation** in `assemblePrompt` (the board block is now built before the log
rather than after), and although the emission order and every pin are unchanged, I have not proven that
`serializeGameState` has no ordering-sensitive side effect on state the log block reads — the suite's 1,298 tests
and 7,186 PARSETEST pins exercise almost none of the GPT prompt-assembly path, so "green" here is much weaker
evidence than its size suggests. V11's reclaim rule is the weakest in a third way: its match is a **string
match** between a plan step's capitalised runs and the window's tail, which will over-keep on any window that
happens to mention a card the previous step named, and the only bound on that cost is that it can reclaim at most
one step — I have no corpus measurement of how often that over-keep will fire, so the byte figures in V14's
table are, strictly, computed without V11's cost in them. Finally, V6's REFUTE is the strongest item here
evidentially (Oracle text, the two code paths read, two passing fixtures, and the render's own contradicting
sentence quoted from the corpus) and yet it closes nothing the pilot will feel: the residual is a deck130 guide
surface I did not touch and no lane this wave owns.
