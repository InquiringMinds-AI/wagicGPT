# Wave-52 reviewer boundary pass (deck reviewer lane)

Scope: the nine boundary findings B1-B9 in `wave52/synthesis-notes.md`, adjudicated by the
reviewer of each deck (reviewers own guides; synthesis only reports) and applied to the LIVE
guides in `projects/mtg/bin/Res/ai/baka/` (the deployed wave-52 revisions, commit `e68b7c7c6`).
Governing amendments: **99/136** (a guide states what the render prints, never an engine
behaviour), **118/133** (a refuted sentence is not reprinted inside its own refutation),
**153** (one shippable property per mulligan section - the engine's land count and coverage
line; (b) the carry step; (c) the thirteenth hand, 2 lands / no coverage), **171** (a sentence
quoting an engine literal says what the literal does NOT decide; a qualifier goes on the rung
it governs; #131's hands are constructed at every rung whose text changed), **131** (the
twelve-hand self-consistency check), **168** (the three-number literal audit), **177**
(prose-vs-DECIDING-SITUATIONS diff).

Every cited line was re-read in the live file before the verdict. Corpus literals were re-read
from `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260901-223430/` (42 game files) with python -
the pregame table below is that read, not the seat files'. ASCII only;
`git diff -- projects/mtg/bin/Res/ai/baka/ | /usr/bin/grep -c $'\357\277\275'` = **0**; a
`[^ -~\t\n]` sweep over all seven guides returns no file. No `src/` change, no commit, no wagic
run.

## Corpus re-read: every pregame mulligan decision this corpus (42 logs)

| seat | looks | KEEP | SHIP | empty_reply | bottom asks | `would cover` | `would not cover` |
|---|---|---|---|---|---|---|---|
| deck123 | 9 | 5 | 3 | 1 | 4 | 6 | 3 |
| deck125 | 7 | 6 | 1 | 0 | 10 | 6 | 1 |
| deck126 | 6 | 5 | 0 | 1 | 0 | 5 | 1 |
| **deck130** | **6** | **6** | **0** | 0 | **0** | **6** | **0** |
| deck146 | 10 | 6 | 4 | 0 | 5 | 6 | 4 |
| deck152 | 8 | 6 | 2 | 0 | 2 | 6 | 1 |
| deck162 | 6 | 6 | 0 | 0 | 0 | 5 | 1 |

(deck152's `0 lands, 7 spells` look prints neither coverage line - hence 6 + 1 of 8.) The ten
ships, by the engine's own land count: **2 lands / no coverage x5** (deck123, deck125, deck146
x2, deck152), **3 lands / no coverage x1** (deck146), **1 land / no coverage x3** (deck123 x2,
deck146), **0 lands x1** (deck152). Six of the ten were 2-or-3-land hands shipped under the
no-coverage literal - #171's evidence family reproducing in the corpus
that PRECEDED the wave-52 revisions - deck146's and deck152's scope sentences are the fix for
exactly these six, and this pass extends the same fence to deck130 and deck123.

**deck130 confirmed the holdout.** Six looks, six keeps, zero ships, zero bottom asks, and the
coverage literal printed `would cover the cost of:` 6 of 6 - the `would not cover any spell in
it` branch of its section has **0 renders at this seat this corpus**. #132-UNTESTED as the
synthesis said, so the section was audited against the amendments rather than against evidence.

## Verdicts

| # | verdict | reason |
|---|---|---|
| B1 | **ACCEPT - deck126 edited** | :474-475 re-read in the live file. *"This shape reached you once this corpus and the engine answered it for you."* is #99/#136 exactly: the pilot is told the ENGINE took a mulligan decision (it did - the 900 s empty_reply at seq 1 of `126 vs123`, this corpus's one deck126 `fallback: empty_reply`, confirmed in the re-read above), which is an engine behaviour in guide prose and a provenance note the pilot cannot act on. Cut. The rule above it (KEEP a 3+-land no-coverage seven, in odds form) is correct and untouched. |
| B2 | **ACCEPT - deck146 edited** | :105-107 re-read. The AFTER text printed the refuted sentence in full inside its refutation - #118/#133: a negated rule printed verbatim is still a printed rule, and this is the exact string the seat quoted back to itself as its reason to ship. Quotation deleted, imperative kept, the cross-reference to COLOUR IS NEVER A REASON TO SHIP preserved. The positive statement above it ("THIS LINE STATES NO COLOUR REQUIREMENT AND IT IS NOT A TEST") already carries the whole teach. |
| B3 | **PASS (no edit), recorded** | deck123 A52-1/2/3 and deck126 D65/D73 re-keys are in the live files. Obligation for wave 53 (three numbers per literal): recorded below. |
| B4 | **PASS (no edit), recorded** | deck126 D69's nine-line `{paying this taps: <wall>}` translation re-read; it is a derivation from a true string about the pilot's own deck, not a hedge - TRUST DOCTRINE clean. Re-key + CUT owed at D6. |
| B5 | **PASS (no edit), recorded** | deck123 A52-4/4b and deck126 D65 re-read; the `[second copy:` bullet is present in DECIDING SITUATIONS. Two re-keys owed at D11 (deck123's non-stacking sentence and its "a second Alarm changes nothing" derivation). deck126's D65 reason is Lantern-specific in the live text as the synthesis required. |
| B6 | **PASS (no edit), recorded** | deck130 D52-3's GANG BLOCK bullet and deck123 A52-4b re-read in place. The #177 mechanical diff across all seven guides is a wave-53 pre-corpus obligation, not a boundary edit. |
| B7 | **PASS (no edit)** | deck152 152-M re-read; the Perimeter Captain primitive `@combat(blocking) source(creature[defender]|mybattlefield):may life:2 controller` triggers on BLOCKING, so "first strike does not cancel it" is right. Nothing owed. |
| B8 | **ACCEPT - deck130 and deck123 edited; the everywhere-clause swept and found no fourth site** | Three parts, below. |
| B9 | **PASS (no edit), recorded** | deck123 A52-12 re-read: it teaches the window lane I opened and closes the dead-row loop; no sentence says "do not answer these windows". Watch + D2 re-key recorded. |

## B8 part 1 - deck130: #153(a), then #171, then the thirteen hands

**#153(a) - one shippable property: PASS, no second property found.** Section swept sentence by
sentence. The ship-side properties named are the engine's land count and its coverage line, and
nothing else; "colour is never a reason to ship" and "acts by turn three" are a negation and a
restatement of the same property (the wave-51 audit's reading, re-confirmed against the live
wave-52 text). No keep is gated on card identity, curve, or a cost.

**#171(a) - the scope sentence: DEFECT, edited.** `"would not cover any spell in it"` was quoted
with a ship-leaning consequence ("that is the shape where a smaller hand can be the better one")
and no land-count fence; the floor that would have fenced it ("two or more lands is a keep at
every N") is in the NEXT paragraph - #171(b)'s "a qualifier goes on the rung it governs" in
mirror image. This is the shape that produced four of the five 2-and-3-land no-coverage ships in
the corpus table above.

**#131 + #153(c) - the thirteen hands, before and after.** Two sentences disagreed:

| hand | (keeping 7) | (keeping 6) | (keeping 4) | verdict |
|---|---|---|---|---|
| 0 lands | ship | ship | **keep** (per "ships at any N above 4") vs ship (per "does THIS hand act by turn three") | **DEFECT** - a zero-land hand casts nothing at any size |
| 1 land, no coverage | ship | ship | keep | consistent |
| 1 land, coverage | keep | keep | keep | consistent |
| all lands | **keep** ("two or more lands is a keep at every N") vs **ship** ("all lands ships") | same | same | **DEFECT** - direct contradiction, both sentences unscoped |
| 13th: 2 lands, no coverage | **keep** (floor) vs **ship-lean** (the coverage paragraph) | same | - | **DEFECT** - #171 |

All three closed by the edits below; re-run after the edit, no two sentences disagree on any of
the thirteen.

**Edits (deck130, 58,875 -> 59,011, +136 bytes):**

1. BEFORE: *"If it reads "would not cover any spell in it", nothing in the hand happens until you
   draw, and that is the shape where a smaller hand can be the better one: a one-land seven of
   Starstorm, ..."*
   AFTER: *"If it reads "would not cover any spell in it", nothing in the hand happens until you
   draw, and at ONE land that is the shape where a smaller hand can be the better one - at two
   or more lands that line is not a ship, it is a turn-three hand: a one-land seven of
   Starstorm, ..."*
2. BEFORE: *"A hand with two or more lands is a keep at every N."*
   AFTER: *"A hand with two or more lands and at least one spell is a keep at every N."*
   (the deck152/deck162 form; #144 now has one wording across the pool)
3. BEFORE: *"Zero lands, or all lands, ships at any N above 4."*
   AFTER: *"Zero lands ships at any N; a hand of nothing but lands ships at any N above 4."*

## B8 part 2 - deck123: A52-10's twelve hands completed

A52-10 constructed only the 2-lands/no-coverage case (the seat says so). Completed here; the
two-lander case passes as written (*"would not cover any spell in it" is a KEEP at two lands*,
plus *"The coverage line decides ONE-land hands and nothing else"* - #171(a) already satisfied,
so no scope sentence was added). Two hands failed:

| hand | (keeping 7) | (keeping 6) | (keeping 4) | verdict |
|---|---|---|---|---|
| 0 lands | ship | ship | ship | consistent |
| 1 land, no coverage | ship | *silent* (the ship clause is scoped "on the untouched seven"; STEP 1's keep starts at (keeping 5)) | keep | **GAP, recorded not edited** |
| 1 land, coverage | keep | keep | keep | consistent |
| all lands | **keep** ("TWO OR MORE LANDS: keep. Full stop, at every (keeping N)") vs **ship** ("ZERO LANDS, or ALL lands: ship") vs STEP 1's *"Zero lands is the one hand a fresh look is worth the card for"* | same | **keep** ("a hand with a land in it is a keep, spell or no spell") vs ship | **DEFECT** - three sentences, two verdicts |
| 13th: 2 lands, no coverage | keep | keep | - | consistent |

**Edits (deck123, 67,217 -> 67,302, +85 bytes):**

1. BEFORE: *"Zero lands is the one hand a fresh look is worth the card for."*
   AFTER: *"Zero lands is the one hand a fresh look is worth the card for; a hand of nothing but
   lands is the other."*
2. BEFORE: *" - TWO OR MORE LANDS: keep. Full stop, at every "(keeping N)"."*
   AFTER: *" - TWO OR MORE LANDS AND AT LEAST ONE SPELL: keep. Full stop, at every "(keeping N)"."*
3. BEFORE: *" - AT FIVE CARDS OR FEWER: STEP 1's odds apply - a hand with a land in it is a keep,
   spell or no spell, because the ship buys the same land odds with one card fewer."*
   AFTER: *" - AT FIVE CARDS OR FEWER: STEP 1's odds apply - a hand with a land and a spell in it
   is a keep, whatever that spell costs, because the ship buys the same land odds with one card
   fewer."*
   (the "spell or no spell" clause was there to refuse a curve requirement; "whatever that spell
   costs" keeps that refusal and stops licensing an all-land keep)

The (keeping 6) one-land/no-coverage GAP was NOT filled: closing it means writing a keep-or-ship
verdict the guide has never stated, at odds this pass would have to derive, and a silence is not
a #131 disagreement. Recorded as a wave-53 obligation for deck123's reviewer, who owns it.

## B8 part 3 - the everywhere-clause sweep (#171(a) on every engine-literal sentence)

Swept every sentence in all seven guides that quotes `would cover the cost of:` /
`would not cover any spell in it` / `counted by the engine:` / the `(keeping N)` price line, and
added the "what it does NOT decide" clause ONLY where the guide was silent AND the silence
changes a decision. One site qualified (deck130, above). The rest:

| guide | the literal's fence | action |
|---|---|---|
| deck123 | *"The coverage line decides ONE-land hands and nothing else"* + *"NEITHER COLOUR NOR THE COVERAGE LINE IS A REASON TO SHIP A TWO-LANDER"* | already scoped - no edit |
| deck125 | the no-coverage branch is its OWN branch, unfenced by land count, and decides by draws-to-first-cast | not silent, decides deliberately - **no edit; flagged, see obligations** |
| deck126 | every branch is headed by its land count (THREE OR MORE / Two lands / Zero or one land) | already scoped - no edit |
| deck130 | none | **edited** |
| deck146 | *"THAT COVERAGE LINE IS A ONE-LAND TEST AND NOTHING ELSE ... At TWO OR MORE lands the sentence "would not cover any spell in it" is not a ship and never was"* | already scoped - the amendment's model form |
| deck152 | *"TWO OR MORE LANDS AND AT LEAST ONE SPELL: KEEP. That is the floor"* + RULE -1's M-is-6 line names the three ships explicitly | already scoped - no edit |
| deck162 | *"Tiebreakers between hands that already clear the land count, never a reason to ship one that does"* | already scoped - no edit |

Guides are 36-67 KB; nothing was added where the fence already stands.

## Edits applied, per guide

| guide | before -> after | byte delta |
|---|---|---|
| `deck123_strategy.txt` | three mulligan sentences (B8 part 2) | 67,217 -> 67,302 (**+85**) |
| `deck126_strategy.txt` | one sentence cut (B1) | 49,373 -> 49,293 (**-80**) |
| `deck130_strategy.txt` | three mulligan sentences (B8 part 1) | 58,875 -> 59,011 (**+136**) |
| `deck146_strategy.txt` | one quotation deleted (B2) | 54,878 -> 54,731 (**-147**) |
| `deck125_strategy.txt` | untouched | 50,417 (0) |
| `deck152_strategy.txt` | untouched | 53,727 (0) |
| `deck162_strategy.txt` | untouched | 36,004 (0) |

**Pool total: -6 bytes across four guides.** Every edit keeps its section's structure and its
rule's action.

## Cross-guide KEY diff (#144) after this pass

- The floor sentence is now one wording in five guides: *"two or more lands and at least one
  spell"* (deck123, deck130, deck152, deck162; deck146 states it as "KEEP a 7 or a 6 with 2-5
  lands" plus its explicit not-a-test clause; deck126 branches by land count instead).
- Every guide that quotes the coverage literal now fences it by land count EXCEPT deck125, whose
  no-coverage branch is a deliberate weigh. **deck125 is the pool's only unfenced no-coverage
  branch** - the single holdout by key, and it shipped a 2-land no-coverage seven this corpus.
- Engine-behaviour vocabulary in guide prose: zero guides after B1's cut.
- A refuted sentence printed verbatim inside its refutation: zero guides after B2's cut.

## Wave-53 obligations (recorded, not edited for)

1. **B3 / #168** - report THREE numbers per quoted literal, per seat: renders of the OLD string
   (expect 0), renders of the NEW string, takes under the new string. Owed on deck123 A52-1/2/3
   and deck126 D65/D73.
2. **B4 / D6** - when `- it cannot block on their turn` ships for a `defender` source, deck126's
   nine-line D69 translation collapses to the synthesis's one sentence and the translation
   instruction is CUT, not left standing beside the new literal (#154).
3. **B5 / D11** - deck123's `[second copy:` sentence re-keys to the non-stacking verdict and its
   "a second Alarm changes nothing" derivation is cut. deck126's D65 reason stays Lantern-
   specific.
4. **B6 / #177** - run the prose-vs-DECIDING-SITUATIONS mechanical diff on ALL SEVEN guides
   before the corpus, not only on rules that broke.
5. **B9 / D2** - when the HOLD row ships, deck123's A52-12 paragraph re-keys to the row's own
   literal and the dead-row loop sentence is cut; report opponent-turn declines and casts
   separately at every seat.
6. **deck130 (this pass)** - the section is still #132-UNTESTED: 0 ships in two corpora, 0 bottom
   asks in three, and its no-coverage branch has never rendered at the seat. The falsifiable
   prediction for the wave-53 seat: at any 2+-land look under `would not cover any spell in it`,
   the answer is KEEP; at a one-land look under it, SHIP at (keeping 7)/(keeping 6). If no such
   look occurs a third time, the section is UNTESTABLE-AT-THIS-SEAT (#132) and the check moves
   off the guide.
7. **deck123 (this pass)** - the (keeping 6) one-land / no-coverage GAP: its reviewer states a
   verdict there, in odds form, or scopes the ship clause so the silence is visible.
8. **deck125 (this pass, for its own reviewer)** - the no-coverage branch is the pool's only one
   unfenced by land count, and its weigh ("sharpest when the spells left are Final Judgment,
   Staff of Nin or Emrakul") reads as a second shippable property under #153(a). It shipped a
   2-land no-coverage seven this corpus. Not edited here - the branch is deliberate and deck125's
   reviewer owns it - but it is the #153(a) audit's remaining open question, and it is now the
   named holdout in the #144 diff.

## Not verified / notes forward

- The four edits are derived from the amendments and the corpus table, not corpus-tested. The
  wave-53 seats score the first looks under the new wording (metric: keep/ship agrees with the
  land count and coverage line, N/N; the all-land and zero-land hands are the ones this pass
  changed).
- No odds arithmetic was written this pass - every edit reuses numbers already in the guide.
- The corpus table's `would cover` / `would not cover` counts are over PREGAME prompts only
  (`Pre-game mulligan decision` in the `prompt` field); the literals also appear inside later
  narration and are not counted there.
- deck125, deck152 and deck162 were read for the sweep and not edited; their scope sentences were
  verified in the live files, not assumed from the seat files.
- B4, B5, B9's re-keys are keyed to strings that do NOT print today (D6, D11, D2 unshipped);
  nothing in this pass keys to an unshipped string.
