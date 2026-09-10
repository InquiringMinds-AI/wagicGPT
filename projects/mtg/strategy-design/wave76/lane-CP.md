# Wave-76 lane CP — row and prompt truth (Q2, Q10, Q11, Q12, Q14, Q15)

Base: master `77b920612`, worktree `worktrees/lanes/w76-CP`, branch `w76-lane-CP`.
Corpus for every repro: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-003556-final`.

**Gate.** Suite (THREADS=1, detached unit `w76-CP-suite`) **1289 tests / 0 failed + 76 AI / 0
failed** — `==Test Failed !==` 0 and `==Test timed out` 0, baseline exactly, no known flake fired.
PARSETEST **6016 passed / 0 failed** (baseline 5954/0; **+62** cases, all in one `#W76-CP` block at
the corpus end). `check-reply-instructions.py` OK (1 source, 37 guides). `git diff` U+FFFD count 0.

**RED-on-base method.** The fix hunks for Q2a, Q2b and Q15 were reverted in place (5 marked
`//RED-ON-BASE` edits), the tree rebuilt, and the new PARSETEST block run against it: **11 failed**,
and they are exactly the Q2 + Q15 pins plus the superseded wave-74 O9 pin. The fixed tree was then
restored byte-for-byte (`0` RED markers of mine remain) and rebuilt to 6016/0. Q10/Q11/Q12/Q14 add
new pure helpers, so their RED evidence is the corpus record, named per item below — stated again
under Weakest evidence.

---

## Q2 (HIGH) — the 80,805-byte prompt: an uncapped roster **and** a defeated copy-row fold

**Repro.** `125v123` deck125 **seq 394** (discard, t77), prompt 80,805 B: menu block 52,120 B, of
which **47,391 B is five rows** — `24.`/`25. Final Judgment (copy 1|2 of 2 in your hand)` at 9,450 B
and `26.`/`27.`/`28. Supreme Verdict (copy 1|2|3 of 3 in your hand)` at 9,497 B. Read off the record,
rows 26/27/28 are **byte-identical apart from the copy ordinal** (verified: strip `(copy N of 3 …)`
→ the three strings compare equal), and each carries its own copy of
`{right now: destroys 100 of their creatures … - THEIRS: Lord of Lineage (5/5) […], Vampire #1 …,
… Vampire #99 …}`.

**The mechanism, which the wave-75 seat could not see.** Two defects compose, and the second is not
"the fold does not reach the discard seam" — the discard seam *does* run `groupNumberedRows` +
`joinNumberedRows` (rows 3-6, 7-10, 11-13 of the same menu folded correctly). It is **which instance
grammar owns the row**: both collapses try `splitRowHandle` (the first ` #<digits>` token) FIRST and
`splitCopyRowHandle` only as a fallback, and these rows' own annotation names `Vampire #1`. So every
copy row keyed on that battlefield handle, the copy ordinal landed inside the *head*, no two heads
compared equal, and 28.5 KB of byte-identical text printed three times. Path to Exile folded because
its text contains no `#N` at all.

**RED evidence (on the seeded tree).** `#W76-CP Q2 REPRO/GREEN 125v123 seq 394: three copy rows whose
own annotation names a battlefield #N now fold to ONE range row` and its four siblings — all FAIL on
the reverted build, PASS on the fix.

**Fix.**
1. `sweeperRosterTail` prints the **count plus the distinct shapes**, through a new pure
   `joinVictimRoster` that reuses the battlefield line's own rule (group by head+tail in
   first-appearance order, ascending rank inside a group, a run of *consecutive* ranks at or above
   `kBattlefieldCollapseFloor` prints as one `#a-#b … xN`). 100 bodies: **9,250 B → 220 B**. Below the
   floor the string is byte-identical to the wave-60 wording. The one lossy edge — more distinct
   shapes than `kVictimRosterGroupCap` (12) — **names its residue** (`+ K more bodies in J further
   shapes not listed here (the count above covers them)`) rather than trimming silently.
2. `copyTagOwnsRow`: when a row's `(copy N of M in …)` tag comes **before** every `#N`, the `#N` is
   inside the row's commentary about the board and the copy grammar owns the row. Applied at both
   `groupNumberedRows` and `joinNumberedRows` so the gather and the print cannot disagree.

**The option set is untouched.** The fold is display: `shown`, `discardOrder` and the composed
answer→engine map still carry one slot per card. Pinned three ways — the gather is a permutation
*and* a bijection over 28 rows; the printed labels, ranges expanded, are exactly 1..28 with no gap and
no repeat; and `composeRowOrder(disp, order, composed)` satisfies `composed[k] == disp[order[k]]` for
every k with the folded rows included, which is the **wave-72 codex finding 6** hazard (a permutation
applied twice, or to an index already in engine space) stated as an assertion.

**GREEN.** The seq-394 shape (all 28 rows, the real capped roster) joins to **< 12,288 B** against
52,120 B on base. PARSETEST +26 cases here, including MUST-NOT-MATCHes for a non-consecutive rank run,
a handle-led row (unchanged), and a short roster (byte-identical).

**Key-stability pin set.** The roster count is a board-derived number inside a `{…}` group: two
windows differing only in 100-vs-99 bodies key **identically** at `holdActionKeyRow` and at
`optionSetKeyOf` (pinned both, with the "they really do differ in the printed bytes" pin first).

**Prediction (falsifiable).** Next corpus: **0 prompts above 40,000 B on any seat** (wave 75: 562
prompts over 20 KB, max 80,805 B), the largest `- THEIRS:` roster clause under 500 B, and the discard
seam's option COUNT unchanged at its wave-75 level (a 28-card hand still offers 28 answerable
numbers). Negative clause with its own pin: **0 answers resolve to the wrong card** — if the fold had
broken the map, `composed[k] == disp[order[k]]` would fail, and live it would show as a discard whose
`chosen_text` names a card other than the one the number printed.

---

## Q10 (MED) — a ghostform body counted as removed

**Repro.** `125v146` deck125 **seq 71** (t20): `{removes: Goblin #1, Silverquill Silencer, Nadaar,
Selfless Paladin, Goblin #2, Goblin #3}` on a Path to Exile row, while the same prompt's battlefield
line reads `Nadaar … [counters: 1x ghostform (when this creature dies or is exiled its owner returns
it to hand …)]`. 4 false clauses in 13 ghostform windows; the seat spent its only removal there.

**Fix.** `w76BodyReturnsInsteadOfDying` = a live `ghostform` counter **and** O1's own
`w73GhostformTriggerLive` (so the row and the counter gloss cannot disagree — a counter whose granting
trigger is gone is a marker only and gets nothing). Its tail
`(NOT removal: it returns to its owner's hand and leaves a 1/1 white flying Spirit)` rides the victim
name at both emitters: `sweeperVictimName` (the `- THEIRS:` roster) and the `removalVictimTag`
classification. **The body stays on the list** — it is a legal target and the spell resolves against
it; only the claim about what that buys is corrected.

**GREEN.** PARSETEST: the tail survives `removalVictimTag` intact, a board with no ghostform counter
renders byte-for-byte as before, and the key-stability pin holds (the tail is inside the `{removes: }`
brace group, so both keys are unchanged).

**Prediction.** Next corpus: **0 `{removes: …}` or sweeper roster entries name a body the same
prompt's battlefield line marks with a live ghostform counter, without the tail** — every such body
carries it. Negative clause, pinned: a ghostform counter whose trigger is gone gets **no** tail
(`w73GhostformTriggerLive` false → predicate false).

---

## Q11 (MED) — loyalty rows that name a target the ability does not have

**Repro.** `126v146` deck126 **seq 39/40**: `-6: destroy up to three and reanimate with Sorin, Lord of
Innistrad targeting Silverquill Silencer [opponent's battlefield]`, and the log at seq 40 shows the
ability destroying and reanimating *three other* permanents while the named Silencer survived. And
every Sorin menu at that seat (13 windows) rendered `-2: emblem: "creatures get +1/+0" with Sorin,
Lord of Innistrad targeting Sorin, Lord of Innistrad [your battlefield]` — the `-2` targets nothing.

**Fix.** `w76LoyaltyRowTargetForm(row, upTo)`, a pure test over **the row as rendered so far** (the
same text the reader gets, so the two cannot diverge): `2` = an `up to N (N>1)` chooser → the row
prints `w76UpToNTargetClause(N)` instead of a name; `1` = an emblem ability whose own text carries no
`target` → the row prints **no** target clause; `0` = everything else, unchanged. The arity clause is
a **bracket** group, so it rides the annotation channel.

**GREEN.** PARSETEST +11: both repro shapes positive; MUST-NOT-MATCH on a real single-target loyalty
ability (Kaya `+1`), on `up to one`, on an emblem ability that *does* target, and on any row with no
loyalty prefix. Key-stability: two arity clauses differing only in N key identically at
`optionSetKeyOf` and `holdActionKeyRow`, and `stripNarrationDecoration` removes the clause entirely.

**Prediction.** Next corpus: **0 loyalty rows print ` targeting ` on an `up to N>1` ability or on an
emblem ability**, and every such chooser row instead states its arity. Negative clause, pinned: a
loyalty ability with one real target still names it — if this regressed, `w76LoyaltyRowTargetForm`
would return non-zero for `+1: target creature gets ghostform with Kaya the Inexorable`.

---

## Q12 (MED) — the casting menu's closing rows had no stable order

**Repro.** deck126: **127 menus** print `Hold priority` then `Cast nothing right now`, **32 the
reverse** (e.g. `126v146` seq 4, rows `2. Cast nothing right now` / `3. Hold priority`), while every
one of them ends `CHOICE: … (the LAST row of this menu declines: it is a real answer, not a
fallback)`. On those 32 that sentence describes the **hold** row, which is not the plain decline and
whose own text says it gives up the turn's remaining casting windows.

**Fix.** The wave-66 gate was `LegalActionsOracle::hasInstantResponse(this)` — a board fact deciding a
*layout*. The order is now a fact about the menu: `w76ClosingRowInsertAt(declineRowIdx, menuSize)`
puts the hold **at** the decline row (decline last, hold immediately before it), and appends it when
the menu has no decline row. One pure function, called by the seam and asserted by the pin, so they
cannot drift. The option set is unchanged; every consumer below still reads the index assigned there.

**GREEN.** PARSETEST +4 (decline present, no decline, decline at row 0, an out-of-range index that
must never write past the end). The **priority** seam was checked and already has one order: the hold
is last among the numbered rows and the decline is row 0 (`#W73-BY N2 d`) — no change, and it is why
deck126's count is a casting-seam count only.

**Prediction.** Next corpus: **0 casting menus print `Cast nothing right now` above `Hold priority`**
(wave 75: 32 of 159 on deck126 alone), on every seat. Negative clause, pinned: a casting menu with no
decline row still carries a hold row (the append arm), so the count of menus carrying a hold does not
fall.

---

## Q14 (MED) — the 3-life land row said a plain land can attack

**Repro.** All **8 of 8** `pay 3 life` rows in the corpus, all deck146 (`123` seq 5, `152` seq 8,
`162` seq 22, `130` seqs 15/20/26, `125` seqs 17/22), print `[usable (tap for mana / attack) this
turn]` for Emeria, Shattered Skyclave and Agadeem, the Undercrypt — ordinary lands with no animation
ability. `146v162` seq 22 is the screen where the seat paid 3 life.

**Fix.** `etbUsableClause(canTapForMana, canAttackEver)` — four states, each true of the permanent it
describes. The caller reads the two board facts off the recovered land (`landTapMana(data->text)` for
mana, `isCreature() || permanentCanAnimate()` for attack — the same animate predicate `#W71-BR` and
`#W72-BX` already use). When the land cannot be recovered the wave-58 wording stands, the same
conservative fallback the NAME already uses. A permanent that can do neither gets the arrival fact and
no usability claim — never a silent deletion.

**GREEN.** PARSETEST +8, including the whole `146v162` seq-22 row as it renders now, the option short
name untouched (the echo anchor), and the key-stability pin: two windows differing only in the usable
bracket key identically at both `optionSetKeyOf` and `holdActionKeyRow`, and the bracket leaves no
trace in `stripNarrationDecoration`.

**Prediction.** Next corpus: **0 `pay N life` rows offer `attack` for a permanent that is neither a
creature nor animatable** (wave 75: 8 of 8 did). Negative clause, pinned: a creature-land keeps both
verbs — `etbUsableClause(true, true)` is byte-identical to the wave-58 string, so the fix cannot have
silently deleted the verb everywhere.

---

## Q15 (LOW) — cosmetics, the deviation class, and the Lightmine Field narration

**(a) The own-clock tag's plural for one source.** Repro `126v146` seq 27/28: `your Staff of Nin deal
1 damage a turn between them`, one Staff. Third wave (deck125 B-5, deck126 LOW-8). Fix: `ownClockTag`
says `deals N damage a turn` for one source and keeps the wave-63 `#1-#K deal … between them` wording
byte-for-byte for two or more. RED-on-base confirmed. The wave-74 `#W74-CE O9` pin asserted the plural
form for one copy; it is **rewritten in place and marked SUPERSEDED**, same assertion, corrected
wording. Key-stability pinned (the clause is the `#W74-CH` countdown that re-opened 240 holds — still
invisible to both keys).

**(b) `prose_outside_two_lines` counted the forced-close rescue.** Repro `126v146` deck126 **seq 22**,
a `reasoning_forced_close` record whose 823 off-protocol bytes are the phase-2 prefill's own trace
echoed back before a correctly-formed PLAN/ATTACK pair; the answer was consumed correctly. Fix:
`w75ProtocolDeviationClass` takes the forced-close flag (read from `rec` itself, the field the
reviewer will filter on) and names the bytes-only shape `forced_close_prefill_echo`. The record is not
dropped and the two underlying fields are untouched; a forced close that also lost its plan line is
still reported as the plan defect it is. Pinned in all four states plus the two negatives. **This is a
MEASURE field only — no protocol text, no parser tolerance, nothing that licenses prose (invariant
000).**

**(c) The six 723-727 B hold rows** (CI's stated 650 B ceiling, LOW-2). NOT changed. The overage is
the `{card text: …}` group and, at `125v126` seq 228, the `{answers the stack: …}` group the deck126
reviewer calls "the most important thing on the menu". Cutting a true group to meet a byte target
would be the trust-doctrine trade this wave has no evidence for. Reported, not fixed.

**(d) Two `CHOICE:` lines consumed first-wins** (`125v126` seq 345, deck125 B-7). Owner-ruled protocol
territory: **report only, confirmed covered**. The existing multi-answer pins are live and green
(`#W36-B2` group: `'Mox Jet, Pest #2, Pest #3' -> -1 note=multi_answer_unresolved`, and the
first-wins/last-coded-line walk pins). Nothing changed.

**(e) Lightmine Field — ENGINE CHECK, REPORTED, SCRIPT UNCHANGED.** Repro `125v123` seq 1210: 100
events of `Opponent's Lightmine Field dealt 1 damage to <body>` instead of one N-damage event. The
script (`bin/Res/sets/primitives/mtg.txt:67058`) is
`auto=@each blockers:foreach(creature[attacking]|Battlefield) damage:1 all(creature[attacking]|Battlefield)`
against `text=Whenever one or more creatures attack, Lightmine Field deals damage to each of those
creatures equal to the number of attacking creatures.` **The per-body events are exactly the script's
`foreach` by design** — N iterations of "1 damage to all attackers" — so per the lane brief this is
REPORTED and the script is NOT changed. Recorded for a card-script lane: the totals agree, the
*shapes* differ (Oracle is one event of N), and they diverge under prevention effects,
damage-triggered abilities and lifelink, all of which count events. Faithful re-expression would need
a dynamic amount on a single `damage:` rather than a `foreach` multiplier; that is a card-script
change with its own Oracle verification and does not belong in this lane.

---

## Weakest evidence

- **Q10, Q11, Q12, Q14 have no RED-on-base PARSETEST.** Each adds a new pure helper, so a build of the
  seeded tree cannot compile the pin that would fail on it. Their RED is the corpus record — named
  seq, quoted false string, and a count (4/13 ghostform windows; 13 Sorin `-2` windows and the seq
  39→40 resolution log; 32/159 menus; 8/8 pay-life rows). I did not build a per-item reverted binary
  for these four; the three that could be reverted in place (Q2a, Q2b, Q15a) were, and failed.
- **Nothing here was observed through a live render.** Every pin is over a pure builder or a pure
  order rule. Q2's fold is proven end-to-end over a *reconstructed* seq-394 menu, not over a menu the
  discard seam actually emitted, so what is unproven is that `buildCleanupDiscardAskText`'s live row
  strings match my reconstruction byte-for-byte (they were copied from the record, but the roster
  half is now the capped one and no live run has produced it).
- **Q2's group cap is untested against a real board.** `kVictimRosterGroupCap = 12` was never reached
  by anything in this corpus (the widest real roster is two distinct shapes over 100 bodies); the cap
  and its residue sentence are pinned synthetically only. If a board ever presents 13+ distinct
  shapes, the residue clause is the first thing a reviewer should read.
- **`sweeperSurvivorTail` was NOT capped.** The `- NOT DESTROYED (indestructible), stays on the
  battlefield: …` list has the same unbounded shape as the roster; it did not appear at length in this
  corpus, so I left it alone rather than change a string with no evidence behind it. It is the next
  place this class will surface.
- **Q11 reads the row text, not the engine's target spec.** `w76LoyaltyRowTargetForm` decides from the
  rendered label ("up to three", "emblem" without "target"), which is what the model reads, but it is
  a proxy for the ability's real arity. A loyalty ability whose printed text omits its own "up to N"
  would keep the single name; a card text that says "up to three" while the script targets one would
  now under-claim. I did not read `<upto:N>` off the target spec at that seam — the AIAction there
  exposes one target, and the plumbing to reach the chooser's arity is a larger change than Q11 asked
  for.
- **Q14's fallback is permissive.** When `etbPayOrTapLand` cannot recover the entering land, both
  facts default true and the row reads exactly as it did on base — so the defect can still print on a
  window where the recovery fails. The recovery is the same one the NAME already depends on, and the
  corpus shows it succeeding (all 8 rows named their land), but the failure mode is silent.
- **Q12 changed a layout that a live seam chooses.** The pin proves the rule; it does not prove that
  `declineRowIdx` is correct at the call site. If the decline row were mis-identified the hold would
  land in the wrong place — the same class of defect the wave-75 Q7 note warns about (a pure predicate
  proves nothing about its plumbing). The observable that would catch it is the prediction above.
