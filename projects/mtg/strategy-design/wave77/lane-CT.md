# Wave-77 lane CT — the battlefield gloss and row size (R5, R11-row)

Base: master `49c19f413`, worktree `worktrees/lanes/w77-CT`, branch `w77-lane-CT`.
Corpus for every repro: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-141526-final`
(2,314 records, 2,133 of them carrying a `--- CURRENT SITUATION ---` render).

**Gate.** Suite (THREADS=1, detached unit `w77-CT-suite`) **1291 tests / 0 failed + 76 AI / 0
failed** — `==Test Failed !==` 0 and `==Test timed out` 0, baseline exactly, no known flake fired.
PARSETEST **6194 passed / 0 failed** (baseline 6168/0; **+26** cases in one `#W77-CT` block at the
corpus end). `check-reply-instructions.py` OK (1 source, 37 guides), run by the makefile on every
build. `git diff` U+FFFD count **0**. One file changed: `projects/mtg/src/AIPlayerGPT.cpp`.

**RED-on-base method.** The R11-row fix hunks were reverted in place (5 `//RED-ON-BASE` edits
restoring the comma joins), the tree rebuilt, and the block re-run: **4 failed**, and they are
exactly the four R11-row GREEN pins — the NEGATIVE/byte-identical pins and the key pins passed on
both trees, which is what they assert. The fixed tree was then restored byte-for-byte (`0` markers
of mine remain) and rebuilt to 6194/0. R5's fix adds new functions rather than changing existing
ones, so its RED is not a revert: it is (a) the corpus, (b) a PARSETEST pin that asserts the BASE
class test's answer directly, and (c) a live counterfactual on the archived base binary — all three
below.

---

## R5 (HIGH) — the battlefield line printed no ability text for any creature

**Repro.** `125v130` deck125 **seq 7**, prompt 7,260 B. The opponent battlefield line reads:

```
Dwarven Blastminer {1}{r} (1/1) [tapped - cannot attack or block this turn]; Mountain #1-#3 [tapped] x3;
Pyrite Spellbomb {1} [artifact] {effect: "{R}, Sacrifice Pyrite Spellbomb: Pyrite Spellbomb deals 2 damage
to any target. -- {1}, Sacrifice Pyrite Spellbomb: Draw a card."}
```

The artifact beside it is glossed in full; the creature that then land-destroyed the seat to 0
permanents by t17 says only that it is a tapped 1/1. Corpus-wide: **3,892 creature entries on the
two battlefield lines, 0 of them carrying an `{effect:}` clause**, against 24,083 non-creature
entries of which 1,518 do. (R5's docket said "2,734 non-creature entries carry `{effect:}`"; my
count is of ENTRIES whose rendered text contains the substring, 1,518 — the population and the
0-of-3,892 are the load-bearing halves and both agree.)

**Source verified against the engine.** `Res/sets/primitives/mtg.txt`:
`auto={2}{R}{T}:destroy target(land[-basic])` and
`text={2}{R}, {T}: Destroy target nonbasic land. -- Morph {R} (…)`. The `text=` line names exactly
the ability the `auto=` script runs, so the line the model should read and the script the engine
runs **agree** here; the gloss is sourced from `text=` (`card->text`), as every other `{effect:}` is.

**Mechanism.** `boardEffectTextEligible` (wave-46 R3) returns false for `isCreature`, with the stated
reason "their line already carries live keywords, P/T, combat state and summoning-sickness, and that
is the budget". That is true of the KEYWORDS and silent about the ACTIVATED and TRIGGERED abilities,
which is what a creature's `text=` holds and what four of deck125's guide rules key on.

**Fix.**
1. `boardCreatureEffectEligible` — the same class test with the sides swapped: a creature (never a
   land), with an `auto=` script, with printed rules text that is not engine token bookkeeping.
2. `boardCreatureEffectText(text, keywordsPrinted)` — drops every clause the entry has **already**
   printed in its `[...]` bracket, compared against `keywordList()`'s own rendered output so the two
   surfaces cannot disagree. Perimeter Captain's `Defender` goes, its
   `Whenever a creature you control with defender blocks…` stays. A clause whose keyword was NOT
   printed is **kept** — fail-open, because silence is the failure this family exists to stop. The
   match is whole-entry (`flying` does not swallow `Flying Men can block anything`).
3. Budget: the wave-46 width tier **capped at 120 B** (`kBoardCreatureEffectLen`), counted in its
   OWN map so that adding creatures shrinks no existing non-creature clause — every wave-46..76 byte
   on this line has a repro behind it and none of them moved.
4. Width cap: at most `kBoardCreatureEffectNameCap` (12, the `kVictimRosterGroupCap` value, for the
   same reason) distinct creature names per zone line carry a gloss, in board order, and the residue
   is **counted and named** (`boardCreatureEffectResidueTail`). Without it the per-name budget floors
   at 55 B and the line grows without bound in the width of the board — the Q2 defect one surface
   over. Names, cost, P/T, keywords and status stay complete for every creature at any width.

**Byte measurements.**
| where | before | after |
|---|---|---|
| `125v130` deck125 **seq 7** prompt | 7,260 B | **7,538 B** (+278: one distinct creature name, opponent card, voice frame included) |
| corpus **largest** prompt (`125v146` seq 179) | 30,375 B | **< 37,121 B** by the analytic bound below |
| worst case per creature name at the cap | — | 266 B analytic (`2 × 55` budget + `{effect: ""}` + each-copy note + voice frame); widest measured tag in the corpus's texts 196 B |
| worst case whole prompt | — | 30,375 + 2 zones × 12 names × 266 + 2 residue tails = **37,121 B** |

The corpus's max-prompt board could not be re-rendered (the prompt record holds the rendered text,
not the game state, and the fair-hand corpus is not replayable from it), so the 30,375 B figure is
bounded **arithmetically** and the pin is on the bound, not on a re-render — stated again under
Weakest evidence. Note also: in THIS corpus the 30 KB game is `125v146` (30,375 B at seq 179), not
`125v123` (max 24,895 B); the brief's `125v123` label is from the wave-76 corpus.

**Live RED/GREEN (the render probe, not a helper).** `WAGIC_GPT_RENDERPROBE` under
`WAGIC_SELFPLAY_DECK0=152 DECK1=126`, GPT seats with a dead endpoint so no request leaves the box:

* archived **base** binary `archives/wagic-d372a1153-w76step1`: **258 creature entries rendered,
  0 glossed**.
* this tree: **450 creature entries, 224 glossed**, e.g.
  `Perimeter Captain {w} (0/4) [defender] {effect: "Whenever a creature you control with defender blocks, you may gain 2 life."}`
  — the `Defender` clause dropped because the bracket beside it already says it. Max creature-gloss
  bytes in one render **2,586 B**; max situation render 29,302 B; **0** residue tails (no board in
  the game reached 12 distinct creature names). Different hands (fair-hand deal), same decks.

**Prediction (falsifiable).** Next corpus: **every creature battlefield entry whose primitive has a
non-empty `auto=` and a `text=` line that says something beyond the keywords its own `[...]` bracket
prints carries an `{effect:}` clause** — the wave-76 rate was 0 of 3,892. And **no prompt on any seat
exceeds 40,000 B**. Negative clause, with its own pin: **no entry ever renders `{effect: ""}`, and no
creature whose whole text is an already-printed keyword is glossed at all** — pinned by
`#W77-CT R5 NEGATIVE a creature whose whole text is the keyword the line already printed is
INELIGIBLE` plus the tag-builder pin beside it; live it would show as an empty brace group on a
vanilla-with-keyword body.

---

## R11-row (LOW) — the uncapped `kills whichever you target:` roster

**Repro.** `123v125` deck123 **seq 420**: a 735 B clause naming
`Human #1, Human #2, … Human #63` one body at a time. The docketed 2,374 B option row is
`152v126` deck152 **seq 63** (measured 2,377 B), `1. Cast Fateful Absence {1}{w} …`. The sibling
`{removes: …}` branch of the same emitter carried the same shape at **712 B** (Thraben Doomsayer +
Human #1..#40).

**Fix.** Both emitters — `castKillSummaryTag`'s `{kills whichever you target: }` branch and
`removalVictimTag`'s four rosters (`{verb} whichever you target:` THEIRS/YOURS, the INDESTRUCTIBLE
list, and the plain `{verb}: }` branch) — now join through **`joinVictimRoster`**, the wave-76 Q2
call: the COUNT leads, distinct shapes group in first-appearance order, a consecutive rank run at or
above the collapse floor prints as one handle range with its count, and the one lossy edge (more
distinct shapes than the group cap) **names its residue**. The `{removes: }` branch is capped in the
same edit rather than left to come back as a third wave of the identical defect.

**GREEN.** 63-body roster **735 B → 81 B**; 41-body `{removes: }` **→ 58 B**. Below the collapse
floor the strings are byte-identical to the wave-55 wording (pinned on a 2-name roster and on the
single-name `{removes: Goblin #1}` the wave-76 Q10 pin depends on), and `THEIRS - none` is unchanged.

**The option set is untouched.** This is one annotation clause inside a brace group on one row; the
`- legal targets right now:` list beside it, and every answerable number, are unchanged. Nothing is
removed, capped or auto-answered.

**Key-stability pin set.** The roster count is board-derived, so (wave-74 lesson) two rows differing
only in 63-vs-62 bodies were pinned: they really do differ in the printed bytes (instrument pin
first), and `optionSetKeyOf` and `holdActionKeyRow` are **identical** for both — the clause rides a
`{…}` brace group.

**Prediction (falsifiable).** Next corpus: **the largest `{kills whichever you target:}` /
`{removes:}` clause is under 300 B** (wave-76: 735 B and 712 B), and **no option row containing one
exceeds 1,600 B** (wave-76 max 2,377 B). Negative clause with its own pin: **no body disappears from
a roster** — below the collapse floor the string is byte-identical (pinned), and past the group cap
the residue is stated as a count (`#W77-CT R11-row GREEN past the group cap the residue is COUNTED
and named`); live, a lost body would show as a `chosen_text` naming a creature the row never listed.

---

## Weakest evidence

1. **The 40 KB pin is arithmetic, not a re-render.** The corpus record stores the rendered prompt,
   not the game state that produced it, and the fair-hand corpus is not replayable from a translog —
   so the max-prompt board could not be rebuilt with the fix in place. The pin is an **upper bound**
   (two zone lines, both at the 12-name cap, every name an opponent copy charged the voice frame and
   the each-copy note, at twice the 55 B floor budget) added to the measured 30,375 B. It is
   conservative in every term, but it is a bound, not a measurement.
2. **The live render probe ran GPT seats with a dead endpoint**, so every decision was played by the
   heuristic. The RENDER is the thing under test and it is produced before any request, so this is
   the right instrument — but the two games (base 258 entries, fixed 450) are DIFFERENT games with
   different hands, not a counterfactual on one board. The one-board counterfactual is the seq-7
   PARSETEST pin, which is on the class test, not on a rendered line.
3. **The keyword-drop is only as good as `keywordList`'s wording.** The gloss drops a clause when its
   head matches a whole entry of the rendered bracket. Where the engine's legible keyword name and
   the printed card's clause head differ (`cannot block` vs `Defender`, a granted keyword with no
   clause, a keyword the primitive spells differently), the clause is KEPT and the reader sees the
   fact twice. That is the deliberate direction, but it means the duplication rate is unmeasured
   until the next corpus.
4. **`joinVictimRoster` says "bodies" about non-creature permanents.** `{removes whichever you
   target: THEIRS - Liliana's Caress, Underworld Dreams, Dictate of Kruphix}` now leads with
   `3 bodies:`. The word is inherited from the sweeper roster this call was written for; it is
   consistent across the family and it names nothing falsely, but it is the wrong noun for an
   enchantment and no lane has ruled on it.
5. **Three-entry rosters changed wording** (they now lead with `3 bodies:`), which is a visible
   change to many small rows, not only the runaway ones. The collapse floor is 3 and I did not move
   it; a lane that wants the small rows untouched would raise it.
6. **The residue tail has never fired live.** No board in the probe games reached 12 distinct
   creature names, and the wave-76 corpus's maximum was 10 across BOTH lines. The tail is pinned on
   zero, one and many in PARSETEST and rendered on no real board.
