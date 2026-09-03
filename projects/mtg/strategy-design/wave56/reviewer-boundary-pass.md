# Wave-56 reviewer boundary pass (deck reviewer lane)

Scope: the ten boundary findings B1-B10 filed in `wave56/synthesis-notes.md` ("Guide boundary
findings for the deck reviewers (B1-B10)"), adjudicated by the reviewer lane (reviewers own their
guides; synthesis only reports) and applied to the LIVE guides in `projects/mtg/bin/Res/ai/baka/` -
the deployed wave-56 revisions, master at `7ee3a51e8`. All seven `wave56/deckN/strategy.txt` were
re-verified byte-identical to their deployed `deckN_strategy.txt` by **`md5sum` before any edit
(7 of 7 IDENTICAL)**.

Governing amendments (wave-56 edition, `wave56/strategy-writing-skill.md`): **233** (only an
imperative with a trigger literal and a NAMED WRONG ANSWER moves a rate; its mechanical grep),
**238** (a count may be EVIDENCE for a rule, never a fact about the render; an absence claim ships
with its re-verification method and its retirement trigger), **259** (a clause carrying a COMPUTED
number is verified against the record's own fields; no guide may teach the `they would be at N`
value), **268** (the literal audit's fourth column: sentences that quote or assume the old string),
**270** (a trim ships with its falsifiable prediction), **285(c)** (`REMOVE THIS RULE WHEN ...`
inside the guide text), **289** (zero-take audit), **291** (a `WHAT THIS COST` entry carries the
corpus it was measured in), **294** (**normalise whitespace before counting a literal** - the
amendment B1 exists to serve), **299** (a withheld affordance runs at most three corpora); plus the
carried **209**, **220** (a passing wave is the wave to write the fewest lines), **232**, **208**
(re-derive every N-of-M from the field the claim is about, and state the field and the unit),
**240**/**263** (count row literals from the rendered `prompt`, prefix-match HOLD takes).

Every cited line was re-read in the LIVE file before its verdict; line numbers drift, so every site
was located by literal. Every number a finding rests on was re-derived on disk from
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-120952/` (42 seat JSONL, 2,854 records = 2,762
decisions), with python, **counting option rows from the rendered `prompt`** and **prefix-matching
HOLD takes on `chosen_text`**, stating the field and unit per #208; where a finding compares against
the previous wave the baseline was re-derived from `matchups-20260903-074354/` the same way. Guides
were read with `/usr/bin/grep` and with whitespace-normalised python counts (#294). No `src/` change,
no build, no wagic run, no corpus run, no `git add -A`.

## Verdicts

| # | site | verdict | reason (disk evidence) |
|---|---|---|---|
| **B1** | `deck130_strategy.txt` :784-789 (deciding-situations HOLD entry) | **ACCEPT - EDITED** | The wave-54 dead quotation is live and it is the pool's only one. Whitespace-normalised **count per file, all seven guides**: `Pass priority, and do not ask me again this turn` = **1 (deck130), 0 elsewhere**; the live `Hold priority for the rest of this turn: ...` = **1 in six guides, 0 in deck130** (deck126 is the zero-text control). Corpus: the dead string appears in **0 of 2,762** decision prompts; the live string in **2,018** prompts, **329 at deck130's own seats** (328 decisions + 1 `wall_miss` - synthesis's 328 is the decisions-only count). The `{taking this row skips ...}` tail is on **2,018 of 2,018**. The own-turn correction is real: of the 2,018 renders, **1,275 are on the seat's OWN turn** ("It is your turn." in the CURRENT SITUATION header) against 743 on the opponent's, and at deck130 it is 239 own / 90 theirs - so the entry's `It is the OPPONENT'S turn and ...` opener was itself false. Quotation replaced verbatim from the emitter, scope re-keyed, the imperative and the `at the FIRST such ask of the turn, not after four declines` clause kept word for word. |
| **B2** | `deck126_strategy.txt` - the whole file | **PASS - no edit; the SUNSET is recorded** | Re-verified: `Hold priority` **0**, `declined this exact list` **0**, `skips the rest of this turn` **0** in the deployed file. The behaviour reproduces: **402 HOLD-bearing windows, 11 takes (2.7%)** on the offered denominator, and **5 of 206 (2.4%)** on the N >= 2 denominator, against deck125's **33 of 235 (14.0%)** from the same reviewer and the same corpus. The finding asks for no edit this wave and this lane writes none: the control is the seat's declared experiment (#299 gives it one more corpus), and a boundary pass may not spend a seat's decision for it. Obligation 1 below. |
| **B3** | `deck123_strategy.txt` :138-139 | **ACCEPT - EDITED; the draft's two deck-level numbers are WRONG and were replaced with re-derived ones** | The flat absolute is live and reads *"THE ROW IS NOW KEPT."*, contradicted two sentences later by *"When a run does still come back ..."* - the #238 defect the finding names, correctly. **But the replacement string synthesis drafted re-ships two numbers that do not reproduce.** Re-derived (unit: a HOLD take, prefix-matched on `chosen_text`; "came back" = a later `ask`/`priority` record in the same game and the same turn with an identical option list): this deck took the row **60** times, not 119, and the identical list came back **twice**, not 8 times; corpus-wide it is **15 of 320** takes (synthesis's 14 of 320 - a one-record unit difference, its shape is right), against **116 of 282** in the wave-55 corpus (synthesis's 113; deck123's own prior-wave figure is **59 of 106**). The guide's own earlier sentence at :134 (*"took it 106 times last corpus"*) IS the wave-55 figure and is exact, which is what dates the 119 as unsourced. Applied with the re-derived numbers and the absolute demoted; the imperative at :140 is untouched. |
| **B4** | `deck146_strategy.txt` :615-618, `deck152_strategy.txt` :708-711 | **ACCEPT - EDITED (2 guides), and the wave-55 half of the receipt is exactly true** | Both files carry the undated *"190 windows across three of this pool's decks ... and not one of them took it"*. Re-derived on the **wave-55** corpus (unit: window whose prompt carries the live HOLD row and a declined note with N >= 2): deck146 **12** + deck152 **113** + deck162 **65** = **190 windows, 0 takes** - the receipt is exact and only needed its date. Re-derived on THIS corpus: deck146 **5 of 20 (25.0%)**, deck152 **2 of 35 (5.7%)**, deck162 **0 windows**. Synthesis's draft applied as written with the measurements filled in. deck152's neighbouring *"WHAT THIS COST, this corpus: 35 windows ... 33 answered 'Cast nothing right now'"* is consistent with 2 of 35 and was left alone. |
| **B5** | `deck152_strategy.txt` :467 | **ACCEPT - EDITED (trigger appended, imperative untouched)** | The count is exact. Re-derived (unit: numbered option row in the rendered `prompt`): **109 `Flip Side` rows in 90 windows** (deck146 80, deck152 29), **0 takes** in `chosen_text` corpus-wide. Counting every occurrence of the literal in the post-`CURRENT SITUATION` text instead gives 114 - the row unit is the one the claim is about (#208/#240). #238's missing halves (re-verification method, retirement trigger) appended inside the guide text per #285(c); `NEVER TAKE "Flip Side" ON A LAND MENU` stands as written. |
| **B6** | `deck146_strategy.txt` :126-139 | **ACCEPT IN PART - EDITED (one sentence); the #285(c) half is ALREADY SATISFIED and was not duplicated** | The block is the pool's model and is confirmed: `NOT LANDS FOR YOU` renders **0** times in the deployed file. The one defective sentence is live and was fixed. Re-derived (unit: numbered option row): **65 `PLAY THIS AS A LAND` rows in 62 records** (deck146 45, deck152 20), **6 takes** - and by phase, **32 own Main phase 1**, 17 opponent Upkeep, 15 own Blockers step, 1 opponent Draw. **So 33 of 65 rows render outside the seat's own main phase, not synthesis's 44** - the correction does not touch the verdict, and the finding's point (the sentence is contradicted by the render) holds. Replacement applied in the finding's own words. **The second half of the finding's replacement - a `REMOVE THE "ADD ONE" ARITHMETIC WHEN ...` line - was NOT added: the file already carries it at :140** (*"REMOVE THIS RULE when the mulligan header's land number already counts those three names"*), which is the same trigger; adding a second copy is the text #220 forbids. |
| **B7** | `deck125_strategy.txt` :577-584 | **PASS confirmed, no edit** | Re-read live. The quoted literal is byte-exact against the emitter including the `{taking this row skips ...}` tail (1 occurrence, whitespace-normalised), the ordered form is present verbatim (*"THAT ROW IS THE ANSWER AND 'Cast nothing right now' IS THE WRONG ONE - take it at the FIRST such window of the turn, not the fifth"*), and no #233 phrase survives anywhere in the file. The result reproduces: **685 renders, 127 takes**, and at N >= 2 **33 of 235 = 14.0%** against wave 55's **25 of 381 = 6.6%**. Wave-55 obligation 6 discharged as to the text (not as to the bytes - see B10). |
| **B8** | `deck146` :609-613 vs `deck152` :695-704 | **PASS confirmed, no edit; the pair is intact after this pass** | Both blocks re-read. deck146 carries the short form (*"answer the HOLD row, not 'Cast nothing right now'"*) at **25.0%** (5 of 20); deck152 carries the full anti-anchor paragraph (*"Do not let the HOLD row's position at the bottom of the list decide this"*) at **5.7%** (2 of 35). This pass added only a DATE and a measurement to each (B4) and no wording to either arm, so the #290 test wave 57 reads is unconfounded. |
| **B9** | `deck130_strategy.txt` :788-789 | **PASS - the sentence kept; the OPTIONAL demotion half DECLINED, with reason** | The recognition sentence is live and correct (#265 register), and B1's edit now supplies the live imperative directly above it. The finding's optional addition (*"and 'Cast nothing right now' is the wrong row once that line is on the screen"*) was **not** written: deck130 is the pool's only seat carrying an imperative-without-demotion at a large denominator (**16 of 63 = 25.4%** at N >= 2, second-best in the pool), B1 already changes this file's HOLD text this wave, and adding the demotion in the same wave makes deck130's wave-57 rate uninterpretable against B8's graded pair. #220: fewest lines. Recorded as obligation 3 so the seat may take it deliberately. |
| **B10** | all seven `deckN_strategy.txt` - SIZE and NET CHANGE | **PARTIAL REJECT - the sizes are exact; four of seven net changes are AGAIN measured from the wrong baseline (third wave running)** | The measured sizes reproduce to the byte (45.5 / 63.9 / 63.7 / 65.8 / 66.3 / 68.1 / 69.2 KB, band 45.5-69.2, median 65.8). **The net-change column does not.** It is measured from the wave-55 DEPLOYMENT commit `860528eb5`, not from the live file the wave-56 seats worked from (`d948447ab`, the wave-55 boundary pass) - the identical error wave-55 B9 caught and wave-55 obligation 5 forbade in writing. Corrected: deck125 **+2,663** (filed +2,906), deck130 **+598** (filed +537), deck146 **+2,934** (filed +3,214), deck152 **+1,149** (filed +1,396); deck123 -189, deck126 +5,077, deck162 +1,242 are exact (their two baselines coincide). Pool: **+13,474 B (+3.06%)**, not +14,183 (+3.2%). **Every load-bearing claim survives**: deck123 is still the only guide that has shrunk twice, deck126 is still the pool's fastest grower, deck125 still violated its <= 0 obligation (+2,663) and still halved its growth rate. |

## The edits

Four guides, six sites, net **+833 bytes** across the pool.

### B1 - `deck130_strategy.txt` :784-789 (6 lines -> 8)

> `- The last row of the menu - on your own turn as well as theirs - reads "Hold priority for the`
> `  rest of this turn: pass now, and do not ask me again unless the board changes (any change`
> `  re-opens this window; you give up no cast) {taking this row skips the rest of this turn's`
> `  identical windows}" -> take THAT row, at the FIRST such ask of the turn, not after four`
> `  declines. It is the same decision said once for the whole turn, a board change re-opens the`
> `  window, and you give up nothing.`

The imperative, the FIRST-not-fourth clause and the recognition sentence below it are untouched.
Two things changed: the **live literal** replaces the wave-54 string (0 renders) verbatim including
the tail, and the **scope opener** `It is the OPPONENT'S turn and` is gone, because 239 of this
deck's 329 renders are on its own turn. **deck130: 67,849 -> 67,966 bytes (+117).**

### B3 - `deck123_strategy.txt` :138-140 (2 lines -> 3)

> `THE ROW IS NOW KEPT ALMOST ALWAYS. Last corpus this deck took the hold row 60 times and the`
> `byte-identical list came back inside the same turn twice; corpus-wide it came back after 15 of`
> `320 takes, against 116 of 282 the corpus before: the hold nearly always holds.`

The absolute is demoted and the counts stay as EVIDENCE (which #238 permits), but they are **this
lane's re-derived counts, not the draft's**: 60 takes (not 119), twice (not 8), 116 of 282 (not
79 of 170). The next line's imperative (*"Take it at the first dead window of the turn and stop
reading the menu"*) already carries the rule. **deck123: 70,813 -> 70,887 bytes (+74).**

### B4 - `deck146_strategy.txt` :614-618 and `deck152_strategy.txt` :707-712

Both receipts dated and re-measured, in synthesis's own words:

> `WHAT THIS COST, wave 55: 190 windows across three of this pool's decks carried that declined line`
> `with the HOLD row on the menu, and not one of them took it - 85 in a single game, ...`
> `... WHAT IT IS NOW, wave 56: this`
> `deck took it 5 of 20 - the rule is working and the number is the measurement, not the story.`

deck152's copy carries `2 of 35` and its own 65-turn-game tail. **deck146 +275 total with B6;
deck152 +367 total with B5.**

### B5 - `deck152_strategy.txt` :468-470 (3 lines added)

> `  REMOVE THIS PARAGRAPH WHEN the "Flip Side" row stops rendering on a land menu - re-verify by`
> `  counting "Flip Side" option rows in the corpus; it was 109 rows / 0 takes in wave 56, and the`
> `  docket asks for the row to be suppressed.`

### B6 - `deck146_strategy.txt` :126-131 (4 lines -> 6)

> `  Agadeem's Awakening and Pelakka Predation each have a LAND on the back, and the game offers that`
> `  face as a row reading "<land name> with <spell name> -> PLAY THIS AS A LAND". TAKE IT ONLY ON`
> `  YOUR OWN MAIN PHASE WITH YOUR LAND DROP UNUSED - that is when playing a land is legal, and it is`
> `  the only window where taking it is a real land drop. Taking it puts the land onto your`
> `  battlefield for no mana and uses your land drop.`

The false `on your own main phase` scope becomes an ORDER about when to take the row (#233), which
is what the seat's own rules-correct teach needed; the block's `REMOVE THIS RULE when ...` trigger
at :142 already satisfies #285(c) and was not duplicated.

**deck146: 67,328 -> 67,603 bytes (+275)** (B4 + B6). **deck152: 65,261 -> 65,628 bytes (+367)**
(B4 + B5).

## B10 - the measurement

Seven pool guides, `wc -c`, measured from the LIVE file. The **live baseline** column is
`d948447ab` (the wave-55 boundary pass) - the file each wave-56 seat actually revised, NOT the
wave-55 deployment commit `860528eb5`.

| pool guide | live baseline (`d948447ab`) | wave-56 deployed (`7ee3a51e8`) | after this pass | wave-56 net | with this pass |
|---|---|---|---|---|---|
| `deck123_strategy.txt` | 71,002 | **70,813** | **70,887** | **-189** | -115 |
| `deck125_strategy.txt` | 67,098 | 69,761 | 69,761 | **+2,663 (+4.0%)** | +2,663 |
| `deck126_strategy.txt` | 60,384 | 65,461 | 65,461 | **+5,077 (+8.4%)** | +5,077 |
| `deck130_strategy.txt` | 67,251 | 67,849 | **67,966** | +598 | +715 |
| `deck146_strategy.txt` | 64,394 | 67,328 | **67,603** | +2,934 | +3,209 |
| `deck152_strategy.txt` | 64,112 | 65,261 | **65,628** | +1,149 | +1,516 |
| `deck162_strategy.txt` | 45,365 | 46,607 | 46,607 | +1,242 | +1,242 |
| **pool total** | **439,606** | **453,080** | **453,913** | **+13,474 (+3.06%)** | **+14,307 (+3.25%)** |

**The band after this pass is 45.5 - 69.2 KB, median 65.8 KB** (deck162 46,607; deck152 65,628;
deck126 65,461; deck146 67,603; deck130 67,966; deck125 69,761; deck123 70,887) - inside the pool
band 41-71 KB, with the floor still rising. Restate it from this table, not from memory (#232).

**What was deliberately NOT done.** No prose was cut to move a file toward a band; there is no
deletion in this pass. Every edit is either a dead literal replaced (B1), an absolute demoted (B3),
a receipt dated (B4), a retirement trigger supplied (B5) or a false scope re-keyed (B6). No new
recital was written, no guide gained a rule.

## Re-derivations that did not reproduce (#208)

| claim, as filed | re-derived here | reading |
|---|---|---|
| B3 replacement: deck123 *"took the hold row 119 times and the byte-identical list came back 8 times, against 79 of 170 the corpus before"* | **60 takes**, **2 returns**; corpus-wide **15 of 320** this corpus and **116 of 282** in wave 55; deck123's wave-55 figure is **59 of 106** | the draft re-ships numbers from no corpus on disk (all four runs under `selfplay-runs/` were checked). **The finding is right and its draft is wrong**; applied with re-derived numbers |
| B6: *"44 of 65 rows are offered elsewhere"* (than the seat's own main phase) | **33 of 65** (32 own Main 1, 17 opp Upkeep, 15 own Blockers, 1 opp Draw) | unit/phase-bucket correction only; the sentence is still contradicted by the render, verdict unchanged |
| B1: the live row renders *"328 times at this deck's own seats"* | **329** records (171 `ask` + 157 `priority` + 1 `wall_miss`); 328 is the decisions-only count | agreement, stated for the unit |
| B5: *"109 rows in 90 windows"* | **109 numbered option rows in 90 windows** on the row unit; **114** on a raw literal count of the post-situation text | the filed unit is the correct one; recorded so the next audit does not "find" 114 |
| B10 net change: pool +14,183 (+3.2%); deck125 +2,906, deck130 +537, deck146 +3,214, deck152 +1,396 | **+13,474 (+3.06%)**; deck125 **+2,663**, deck130 **+598**, deck146 **+2,934**, deck152 **+1,149** | four of seven measured from `860528eb5` instead of the live file at review time - **wave-55 obligation 5 restated a second time, third wave of the same defect**. No verdict changes |

## Sweeps run this pass

- **The wrapped-literal sweep (#294), across ALL SEVEN files, count per file, whitespace-normalised.**
  Every double-quoted span of 45+ characters in each guide (357 / 244 / 245 / 327 / 213 / 262 / 184
  spans; 82 / 54 / 59 / 58 / 50 / 72 / 32 of them 45+ chars) was checked for membership in the
  30.9 MB of rendered prompt text. **The only stale ENGINE literal in the pool is B1's**; every other
  non-matching span is either a TEMPLATE carrying `N`/`M`/`K`/`<name>` placeholders (the intended
  form) or a quotation of a PILOT plan sentence the guide is rebuking. Two concrete-looking colour
  clauses (`deck146` *"(you have 1 land; ... off {B} alone on turn one)"*, `deck152` *"(you have 3
  lands; ... off {W}{U} alone even with every land in this hand in play)"*) do not render THIS corpus:
  both are wave-55 boundary-pass examples of a live emitter family whose current tail they carry, and
  neither is a stale form. **Nothing else to file.**
- **ASCII**: `LC_ALL=C /usr/bin/grep -c -P '[^ -~\t\n]'` = **0** on each of the seven guides;
  `git diff -- projects/mtg/bin/Res/ai/baka/ | /usr/bin/grep -c $'\357\277\275'` = **0**.
- **Width**: no line added by this pass exceeds 100 columns (**0 over**; longest added line 98).
- **#233 forbidden phrases** (`both are correct`, `either is correct`, `equally correct`,
  `both are fine`, `is also correct`, `it is safe`, `either is fine`, `it is fine to`): **0 hits in
  all seven files** after the pass.
- **#259**: no line added by this pass teaches, quotes or leans on a `they would be at N` value
  (**0 hits** over the added lines).
- **#238 absence-assertion sweep** over the added lines (`zero times`, `never printed`,
  `does not exist`, `not there`, `never offers`): **0 hits**. The one absence-shaped number this
  pass writes (deck152's `109 rows / 0 takes in wave 56`) ships inside its own re-verification
  method and retirement trigger, which is the form #238 requires.

## Obligations recorded

1. **B2 / wave 57 - deck126's control is in its LAST corpus (#299).** It still has **no HOLD text**
   (`Hold priority` 0, `declined this exact list` 0) and converts at **5 of 206 = 2.4%** at N >= 2
   against deck125's **33 of 235 = 14.0%**. The wave-57 seat reports the ratio against those two
   figures and the HOLD block ships in wave 58 whatever the ratio reads. This lane wrote no text
   here, deliberately.
2. **B10 / deck125 - the net-change obligation is in its SECOND wave and was violated (+2,663).**
   The wave-57 file states a net change **<= 0** and names the recital it deleted to pay for it
   (#220), with the trim's own falsifiable prediction (#270). This pass added **0 bytes** to
   deck125.
3. **B9 / deck130 - the demotion half is the seat's call, not this lane's.** The optional
   *"and 'Cast nothing right now' is the wrong row once that line is on the screen"* was declined
   here to keep B8's graded pair readable while B1 changes this file's literal. If the seat wants
   the third arm, it takes it knowingly and says so.
4. **B4 / #291 - both receipts are now dated and neither may be deleted yet.** deck146 (5 of 20)
   and deck152 (2 of 35) had windows and did not pass twice; the rule stays, only the cautionary
   tale can go, and only after two consecutive passing corpora with windows.
5. **B5 / ledger D14 - the `Flip Side` paragraph now carries its own deletion trigger.** The day
   the row stops rendering on a land menu, the paragraph goes in that wave. Re-verification this
   pass: 109 rows / 0 takes.
6. **B10 / standing, third wave - the net-change baseline is the LIVE FILE AT REVIEW TIME**, which
   is the previous wave's BOUNDARY-PASS commit and not its deployment commit. Synthesis's table has
   now got this wrong in wave 54, wave 55 and wave 56. The cheapest fix is mechanical: `git show
   <prev boundary commit>:<path> | wc -c` as the baseline column, printed in the table.
7. **B7 / deck125 :572 - an undated wave-55 count in a present-tense sentence** (*"Four hundred and
   seventy-nine of them in six games last corpus, and you cast 47 spells across all of them"*).
   No B-finding cites the site as an edit, so this lane did not touch it; #291 says date it or drop
   it, and the rule needs no count in front of it. Wave-57 seat's call, and it is a byte donor for
   obligation 2.
8. **Carried, unchanged: B4/D6** (when the `[<- most kills ...]` marker becomes unconditional,
   deck130's STEP 1 collapses to the marker sentence and BOTH levels of the absence branch are cut
   in the SAME wave).

## Not verified / notes forward

- The six edits are derived from the live text plus the re-derivations above; **none is
  corpus-tested.** Falsifiable predictions for the wave-57 seats: (B1) deck130's N >= 2 take rate
  holds at or above 25.4% now that its quotation is live, and no reply at this seat quotes the
  wave-54 string; (B3) deck123's take rate does not fall from the demoted absolute (it is currently
  **19 of 70 = 27.1%** at N >= 2, 60 takes of 233 renders); (B6) no `PLAY THIS AS A LAND` row is
  taken at deck146's seats outside its own main phase (this corpus: 6 takes, all arrivals traced by
  synthesis, phase of each take not re-derived here - see below); (B5) the `Flip Side` count is
  re-run and the paragraph deleted if the row is gone.
- **What I did not verify.** I did not re-derive the PHASE of the 6 `PLAY THIS AS A LAND` takes
  (only the phase distribution of the 65 offered rows); I did not re-verify synthesis's arrival
  traces for those takes, its fallback census, its prompt-size figures, or any ledger item; I did
  not check card facts against the primitives or Scryfall (no edit in this pass asserts one); and I
  did not re-score any deck's rules. The `they would be at N` family was not re-derived - no edit
  touches it.
- **I did not touch** `wave56/deckN/strategy.txt` (they are now intentionally one edit behind their
  deployed twins at four decks, which is what a boundary pass produces), `wave56/deckN/edit-texts.md`
  (the seats' own records), the skill, or the general strategy - both are synthesis's, and the two
  corrections above (the B10 baseline; the B3 draft's unsourced counts) are recorded here for it.
- **I did not run wagic, build anything, or run a corpus.** No file under `src/` was touched, and no
  file outside `bin/Res/ai/baka/` and this document was written.
