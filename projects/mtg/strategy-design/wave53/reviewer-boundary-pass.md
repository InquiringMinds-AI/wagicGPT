# Wave-53 reviewer boundary pass (deck reviewer lane)

Scope: the ten boundary findings B1-B10 in `wave53/synthesis-notes.md`, adjudicated by the
reviewer of each deck (reviewers own guides; synthesis only reports) and applied to the LIVE
guides in `projects/mtg/bin/Res/ai/baka/` (the deployed wave-53 revisions, commit `baf36b936`;
`wave53/deckN/strategy.txt` re-verified byte-identical to `deckN_strategy.txt` for all seven
before this pass began). Governing amendments: **187** (a derivation a shipped literal now
answers is CUT in the wave the literal first renders), **188** (the three-number audit is
counted from the ROWS and scoped per (literal, card family), stating the field), **189** (a
sentence keyed to a counting literal names what is outside the count), **192** (a threshold is
a constant against a printed number), **199** (the step that wins the game is step 0 and
overrides the rest of the procedure), **202/203** (list cardinality; the #171 scoping sentence
as a standing port), **205/206/207** (the survival gate at the ask that spends the resource;
imperatives on BOTH branches of a fix; the carried plan is not a render), **208** (re-derive
every N-of-M from the field the claim is about), and **154/168/171/177** as before.

Every cited line was re-read in the live file before the verdict; where a finding quotes a
number, that number was re-derived on disk from
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260902-023342/` (42 seat logs, 3,572 records, 3,483
prompt-bearing records - the amendment header's own totals, reproduced) with python, per #208
stating the FIELD and the UNIT. ASCII only;
`git diff -- projects/mtg/bin/Res/ai/baka/ | /usr/bin/grep -c $'\357\277\275'` = **0**; a
`[^ -~\t\n]` sweep over the edited guide returns nothing. No `src/` change, no build, no wagic
run.

## Verdicts

| # | site | verdict | reason |
|---|---|---|---|
| **B1** | deck125 W84 | **PASS (no edit), recorded** | Re-read at :459-465. The block now reads *"YOU NOW GET A WINDOW AT EVERY PHASE OF THE OPPONENT'S TURN - their Upkeep, Draw, both main phases, combat, End and Cleanup alike. Four hundred and seventy-nine of them..."*; neither retired sentence survives anywhere in the file (`/usr/bin/grep` for "only when they cast" and "end-of-their-turn window": no hit). Re-derived: **479** opponent-turn decision records at deck125's six seats, **End 35 / Cleanup 34** - the finding's numbers to the unit (field: `prompt`, marker `It is the opponent's turn.` in CURRENT SITUATION; unit: decision record). Corrected TO the render, as the finding says. |
| **B2** | deck126 D91b/D91d | **PASS (no edit), recorded** | Both occurrences re-read (:393-395 in RULE #7 entry 7, :628-630 in DECIDING SITUATIONS); both carry the full new literal ending *"- no legend rule, but the effect it gives your OTHER permanents is already on - this copy adds only its own abilities]"*, and RULE #7 entry 1 carries the contrast sentence for the bare Blood/Bond tag. Three-number audit re-derived at deck126's seats (field: `options_text` rows for renders, `chosen_text` for takes): **old bare Lantern tag 0 row renders**, **new partial-redundancy form 6 row renders**, **0 takes**. Reproduces exactly. |
| **B3** | deck123 A53-6 | **PASS (no edit), recorded; RETIRES on D21** | Re-read at :850-856. The bullet quotes the printed hint and teaches it against the two life totals; it is a teach about a printed string today. The hint renders in **5** deck123 prompts this corpus (field: `prompt`). No edit: D21 has not shipped, and #187 forbids keying to a string that does not print. The finding's D21 wording is carried to the obligations list verbatim. |
| **B4** | deck125 W78a/W78b | **PASS (no edit), recorded** | Re-read at :376-392 and :653-656. Both sites gate the floor on *"Forced Fruition is on the opponent battlefield line beside Underworld Dreams, Fate Unraveler or Ob Nixilis"* before the K test, so the floor cannot reach a cycling row today. Re-derived (field: `options_text` rows at deck125's seats): **27** `[DRAW PRICE:` rows carry the `- you would be at K` tail and **27 of 27 name Forced Fruition**; the cost-phrasing form carries no tail at this seat. #192 in its first shipped instance, satisfiable, and the D10 scope clause is owed only when the tail ships on the cost form. |
| **B5** | deck123 A53-2, deck130 D53-1/D53-2 | **PASS on the keys; ACCEPT - deck130 edited** (defect found in the re-read, not in the finding) | Keys re-read and correct: deck123 :286-291 reads the kill count off the row; deck130 :104-116 is step 0, promotes lethal above the whole procedure and states #189's clause verbatim (*"counts only CREATURES; the player is never in that count"*). No re-key: D4 has not shipped, so "no change" stands. **But the re-read found deck130's #2 RULE carrying TWO consecutive steps labelled `0.`** - the lethal step at :104 and the Starstorm `{X pricing:` step at :117 - in one top-down procedure that then runs 1, 2, 3, 4. #199 makes the winning step the unique step 0; a duplicate label lets a top-down reader execute the sweeper step AS step 0. Relabelled to `0b.` with an explicit ordering clause, and its three cross-references updated. Details below. |
| **B6** | deck146 146-A | **PASS (no edit), recorded** | Re-read at :141-159. Entry 5 no longer keys to `{right now:`; it states outright *"there is no `{right now:` clause on this card and there never will be, so do not wait for one"*, names the two mana-value-2 creatures, and makes the test a log name search, with the WHAT THIS COST paragraph attached. Re-derived (field: `options_text`, unit: row-instance): `{right now:` renders **1,727** times corpus-wide and **0 of 28** times on a `Cast Silverquill Command` row. The 0-of-28 is exact; the corpus-wide figure differs from the seat's **1,586** because the seat counted a different unit - both are true, #208, and only the 0-of-28 is load-bearing. |
| **B7** | deck152 152-A, 152-B | **PASS (no edit), recorded** | 152-A re-read at :590-598: it counts the distinct names in `their untapped blockers`, calls it B, states what B means for the unblocked surplus, and keeps the hold as a price comparison (#202/#178). 152-B re-read at :490-500: deck146's scoping sentence is present as a port, and it names the COLOUR cause explicitly (#203's second half). The corpus that motivated it reproduces: deck152 shipped **3** of 9 pregame looks, one of them `152v162` s1 at **2 lands, 5 spells, {G} 2** - a two-land seven under the coverage line, the third corpus of that shape; deck146 shipped 2 of 8, none at two or more lands. D17 obligation recorded. |
| **B8** | deck162 162-A, 162-B | **PASS (no edit), recorded** | 162-A re-read at :30-49: RULE 0c is at the CAST ask, headed *"THIS OUTRANKS RULE 1"*, keyed to L / `of which N are creatures` / `M of them without a restriction against attacking`, with the spend-the-whole-window instruction and the `{leaves N of your M ...}` clause - #205's "put the gate where the resource is spent", and the block-window arithmetic at :186-195 is left where it belongs. 162-B re-read at :105-122: the exempt branch now carries three imperatives and the printed discriminator (an untagged `Cast Howling Mine` row IS the first Mine), matching the firing branch's register (#206). Re-derived at deck162's seats (`options_text` rows / `chosen_text` takes): second-copy Mine rows at `converters on your battlefield: 0` **3 rows, 0 takes**; first-copy Mine rows **17 rows, 5 takes** (12 declines). The finding's 3/3 brake reproduces exactly and the first-Mine decline count reproduces in shape; its companion "4/4 takes at K>=1" does **not** reproduce on this unit (I count 2 rows, 2 takes on `Cast Howling Mine` rows alone) - a #208 denominator difference, direction unchanged. |
| **B9** | deck123 A53-4, deck130 D53-4 | **PASS (no edit), recorded; watch** | Re-read at deck123 :151-154 and :803-808, deck130 :717-721. Both are scoped to *"you have already answered 'Cast nothing right now' to this exact list"*; neither says "do not answer these windows", so no guide encodes an engine cadence. Re-derived corpus-wide (field: `options_text` / `chosen_text`, unit: row-instance): the HOLD row renders **1,173** times and is taken **29** - the finding's numbers exactly, and the unit is corpus-wide row-instances, not per-seat (deck123 alone: 249 renders, 1 take; deck130: 91 renders, 3 takes). D2 re-key obligation recorded. |
| **B10** | all seven, SIZE | **PASS as a MEASUREMENT finding; no prose cut** | Every guide measured (`wc -c`), table below. The deployed sizes reproduce to the byte. The two bounding facts were re-checked on disk and BOTH hold, one with a corrected denominator: the guide is not in the per-decision prompt - **0 hits for a guide-only phrase in 3,483 prompt fields, at all seven seats** (deck126: 0 of **254**, not "0 of 826"; no seat in this corpus has 826 prompts - #208, the substance is right and the denominator is not reproducible). Nothing was cut to chase the band; the band sentence is stale and the correction is proposed below for synthesis, not edited into the skill. |

## B5 - the deck130 edit

deck130's `#2 RULE - LETHAL FIRST; ...` opens *"Procedure, top down, every time:"* and then listed:

    0.  READ "Opponent life:" FIRST ...            <- the lethal step, added this wave under #199
    0.  FIRST read the "{X pricing:" annotation ... <- the Starstorm sweeper step, pre-existing
    1.  An opponent PLANESWALKER ...
    2.  An opponent CREATURE whose printed toughness this damage reaches ...
    3.  An opponent creature this will not kill but that is attacking you every turn ...
    4.  Only then "The opponent (player, life" ...

Two steps share the label `0.` at the same indent, in a procedure the guide tells the pilot to
walk top down. #199 is explicit that the step that wins the game is step 0 and overrides the
rest of the rule; a duplicate label makes "step 0" ambiguous in the one place the guide cannot
afford ambiguity, and it is the exact defect class that lost `130v162` s62 (the pilot walked the
list from the top). The sweep across all seven guides for a repeated step label at the same
indent inside one run returns **this site only**.

Edit 1 (:117): `  0. FIRST read the "{X pricing:" annotation on the Cast Starstorm line, and read
the WHOLE of it ...` -> `  0b. AFTER step 0 above, and before step 1 below: read the
"{X pricing:" annotation on the Cast Starstorm line, and read the WHOLE of it ...` (three lines
re-wrapped, no other word changed).

Edits 2-4 - the cross-references that name the Starstorm step, re-pointed so no reference is
ambiguous (#177's diff, run on the edit):

| line | before | after |
|---|---|---|
| 307 | `STARSTORM, when rule #2 step 0 says take it` | `... rule #2 step 0b says take it` |
| 373 | `not a card you cast (rule #2, step 0).` | `... (rule #2, step 0b).` |
| 682 | `hold the card (rule #2, step 0)` | `hold the card (rule #2, step 0b)` |

The two references that mean the LETHAL step were left alone and re-checked: :280 (*"rule #2's
step 0 is the whole turn"*, in the land-destruction plan's end condition) and :723 (*"Unless
step 0 of rule #2 fires"*, the face-latch exception). Both are correct as `step 0`, and both now
read unambiguously. No rule's action, key or ordering changed - only the label and the pointers
to it. **deck130: 61,746 -> 61,789 bytes (+43).**

## B10 - the measurement

Every strategy guide in `bin/Res/ai/baka/`, `wc -c`, after this pass (37 files, 668,337 bytes):

| pool guide | pre-wave-53 (`baf36b936~1`) | deployed (`baf36b936`) | after this pass | wave-53 delta |
|---|---|---|---|---|
| `deck123_strategy.txt` | 67,302 | 70,230 | 70,230 | +2,928 |
| `deck130_strategy.txt` | 59,011 | 61,746 | **61,789** | +2,735 (+43 here) |
| `deck146_strategy.txt` | 54,731 | 58,288 | 58,288 | +3,557 |
| `deck152_strategy.txt` | 53,727 | 57,075 | 57,075 | +3,348 |
| `deck125_strategy.txt` | 50,417 | 55,975 | 55,975 | +5,558 |
| `deck126_strategy.txt` | 49,293 | 53,926 | 53,926 | +4,633 |
| `deck162_strategy.txt` | 36,004 | 39,649 | 39,649 | +3,645 |
| **pool total** | **370,485** | **396,889** | **396,932** | **+26,404 (+7.1%)** |

The other thirty guides (never through a review wave) are unchanged and much smaller:

| KB | guides |
|---|---|
| 3.2-6.6 | deck8 3,277; deck59 4,399; deck93 5,124; deck22 5,385; deck148 5,709; deck17 5,909; deck18 5,940; deck134 5,945; deck49 6,319; deck122 6,575 |
| 7.1-10.4 | deck14 7,106; deck110 7,931; deck35 8,293; deck109 8,729; deck140 8,866; deck62 8,898; deck137 9,522; deck102 9,545; deck133 10,190; deck21 10,259; deck136 10,367 |
| 10.9-18.2 | deck27 10,948; deck105 10,962; deck44 12,010; deck116 12,239; deck36 12,447; deck131 13,264; deck135 13,357; deck158 13,641; deck139 18,249 |

**The distribution is bimodal and the skill's band sits in the empty gap between the two modes.**
Non-pool guides: 3.3-18.2 KB (median 9.5 KB). Pool guides: 39.6-70.2 KB (median 57.1 KB). Not one
of the 37 files is in 27-38 KB, and no file has been in that band since deck162 crossed it. The
"27-38 KB" figure appears **once** in the skill, at `strategy-writing-skill.md:11995`, inside
amendment **200(c)**; the synthesis note attributes it to #146, which is the matchup-imperative
audit and says nothing about size. So the finding is right that the band is stale, and its
citation is off by an amendment - both worth carrying forward.

**Proposed replacement sentence** (for synthesis to apply; this pass did NOT edit the skill):

> its supporting ANECDOTE may be compressed - a story about a hand that never comes is the
> cheapest text in a guide that now runs 40-70 KB (wave-53 measured range: 39.6-70.2 KB across
> the seven pool guides, median 57.1 KB; guides that have never been through a review wave run
> 3-18 KB), and every seat states its guide's byte size before and after.

Rationale for the shape of that sentence: the number is stated as a MEASURED range with its
wave and its population, so the next reviewer can tell whether it has gone stale rather than
inheriting a bare figure. #146's discipline cannot be applied to a number nobody prints - the
fix is the measurement obligation, not a cut.

**What was deliberately NOT done.** No guide prose was cut to move any file toward 27-38 KB.
Every wave-53 addition is justified by a cited seq in its seat file, the guide does not compete
with the game log for the prompt's tail (0 of 3,483, above), and cutting text to satisfy a
stale constant would be the #201 error (scoring a rule against a denominator that was never
its own) applied to a whole file.

## Re-derivations that did not reproduce (per #208)

| claim | as filed | re-derived here | reading |
|---|---|---|---|
| deck126 prompts carrying a guide-only phrase | 0 of **826** | 0 of **254** (deck126's six seats; corpus-wide 0 of 3,483) | substance CONFIRMED at every seat; the denominator is not reproducible on any field - no seat has 826 prompt records (deck125 1,063, deck146 811, deck123 529, deck130 492, deck126 254, deck152 199, deck162 135) |
| `{right now:` corpus-wide renders | 1,586 | **1,727** row-instances in `options_text` | different unit; the load-bearing half (0 of 28 on a Command row) is exact |
| deck162 second Mine taken at K>=1 | 4 of 4 | **2 of 2** `Cast Howling Mine` rows at K>=1 | direction identical; the seat's 4 counts a wider row family (the other draw engines the rule also governs) - both true, per #208 |

None of the three changes a verdict; each is recorded so it is not quoted forward unexamined.

## Obligations recorded (not edited for - the strings do not print yet)

1. **B3 / D21** - when the blockers hint's second half is gated so it is not printed while the
   pilot is behind, deck123 A53-6's derivation is CUT in the same wave and replaced by the
   finding's wording (*"...When their life is higher than yours the hint's second half is not
   printed at all: a token goes in front of the biggest attacker."*).
2. **B4 / D10** - when the `- you would be at K` tail ships on the COST form (58 rows, 21 of
   them cycling), deck125's floor is scoped to the row that names Forced Fruition, or a 1-life
   cycle is forbidden by a floor of 15.
3. **B5 / D4** - deck123 re-keys to `{kills 0 of the 2 CREATURE targets ...}`; deck130's step 0
   gains *"...and the row will say so: `- and N to the opponent at life K WINS THE GAME`."*
   **If D4 does not ship, neither sentence changes** - deck130's step 0 is the only surface on
   either guide that knows a face shot can be lethal.
4. **B6 / D5** - when the modal clause ships, deck146 entry 5 re-keys to
   `{modes live right now: ...}` and the graveyard log-search instruction is CUT, not left
   beside it (#187).
5. **B7 / D17** - when the attackers header prints `They have N untapped creatures able to
   block`, 152-A's distinct-name count is CUT and replaced by the header reading.
6. **B8 / D18** - 162-A's render companion; re-key when it ships. 162-B needs nothing.
7. **B9 / D2** - when the HOLD row leads with "Pass priority", BOTH deck123's and deck130's
   sentences quote the new row verbatim, once each, and the old quotation is cut in the same
   wave.
8. **B1 / wave-54 brief** - every reviewer runs the REVERSE audit as well as the forward one:
   not only "did the literal I quote change" but "did a lane make one of my SENTENCES false".
   deck125 W84 was false for a whole wave with every quoted string intact.
9. **B10 / wave-54 brief** - every seat file states its guide's byte size before and after and
   the net change with the trims that paid for it; synthesis re-states the band from the
   measured distribution (proposed sentence above).

## Not verified / notes forward

- The deck130 edit is derived from #199 and the live text, not corpus-tested. The wave-54 seat
  scores it the same way it scores step 0: at any window where a row is lethal, the answer is
  the face row, and the Starstorm step must not be reached first.
- The eight PASS verdicts rest on the live text plus the re-derived counts above; no finding
  was found wrong on disk, so no REJECT was issued this pass.
- The mood-word/TRUST-DOCTRINE sweep was not re-run here (synthesis ran it clean at one hit,
  inside a #174 FORBIDDEN THOUGHT at deck152 :254, re-read and confirmed to be a refutation).
- All seven guides carry a handful of over-long lines where a wave-53 replacement was not
  re-wrapped (lines over 105 columns: deck123 7, deck162 6, deck130 5, deck146 5, deck126 3,
  deck125 2, deck152 1 - e.g. deck123 :290 at 145 columns, deck162 :118 at 125). Cosmetic only,
  no rule affected; left for the owning reviewers rather than touched in a boundary pass.
