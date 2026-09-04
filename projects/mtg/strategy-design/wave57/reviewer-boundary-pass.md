# Wave-57 reviewer boundary pass (deck reviewer lane)

Scope: the nine boundary findings B1-B9 filed in `wave57/synthesis-notes.md` ("Guide boundary
findings for the deck reviewers (B1-B9)"), adjudicated by the reviewer lane and applied to the LIVE
guides in `projects/mtg/bin/Res/ai/baka/` - the deployed wave-57 revisions, master at `3c7c0f3b3`.
All seven `wave57/deckN/strategy.txt` were re-verified byte-identical to their deployed
`deckN_strategy.txt` by **`md5sum` before any edit (7 of 7 IDENTICAL)**.

Every cited line was re-read in the LIVE file before its verdict; line numbers drift, so every site
was located by literal. Every number a finding rests on was re-derived on disk from
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-174505/` (42 seat JSONL, 3,408 records, **3,317
carrying a rendered `prompt`**), with python, **counting option rows from `options_text` and literal
occurrences from the rendered `prompt`**, prefix-matching HOLD takes on `chosen_text`, and stating
the field and the unit per #208. Guides were read with `/usr/bin/grep` and with
whitespace-normalised python counts (#294). No `src/` change, no build, no wagic run, no corpus run,
no `git add -A`.

**This wave's owner-confirmed doctrine correction is applied.** Edits are not added for their own
sake. Where a finding's replacement would add text carrying no falsifier the next corpus can
adjudicate, this lane took the smallest true edit or PASSED, and said so. Three findings were
REJECTED on disproof from the live file, one of them because **the wave-57 seat had already written
the fix the finding asks for, in the same wave** (B7) and one because **the text the finding wants
dated was deleted and re-measured by its own seat, and the drafted replacement would re-import a
wave-55 number into a file that no longer carries it** (B5). The wave-58 DRAFT clauses
(`wave58/reviewer-brief-clauses-DRAFT.md`) were read; its rotation clause is NOT applied (it is a
draft, unruled), but its spirit - the null outcome is a legitimate result - bounds this pass. Net
pool change: **+222 bytes**, of which one edit is a trim of **-253**.

## Verdicts

| # | site | verdict | reason (disk evidence) |
|---|---|---|---|
| **B1** | `deck146_strategy.txt` - the `146-A` pay-3-life floor (located at `NEVER PAY AT 3 LIFE OR LESS`) | **ACCEPT - EDITED; the draft's denominator is WRONG and was replaced with the re-derived one** | The headline reproduces: normalised `REMOVE TH` = **0 in all seven guides** before this pass, and deck146's new floor - the pool's newest hard rule - shipped without one. **The draft's falsifier is `0 of 104`, and 104 is the wrong unit.** Re-derived: `pay 3 life` occurs in **104 prompts**, but as a numbered OPTION ROW it renders **12 times** (deck146 10, deck130 2), taken **3**; `{this payment puts you at` renders **0 times anywhere in 3,317 prompts**. The count the trigger must re-run is the ROW count, so the line ships as **0 of 12 such rows (104 prompts named the payment)**. Applied inside the block, three lines, with method + trigger + docket item (#285(c), #238). |
| **B2** | `deck152_strategy.txt` - the `"FLIP SIDE" - ONE narrow use on LANDS` heading and its display-toggle bullet | **ACCEPT - EDITED (trim, -253 bytes)** | `Flip Side` renders **0 times in 3,317 prompts** (wave 56: 109 rows / 90 windows / 0 takes) - the row is gone from the AI seat. The surviving four lines taught the pilot to recognise and ignore a row it will never see: #289's exact shape. Deleted. **The finding's instruction "delete :449-452, keep :453 onward" would have orphaned the Pathway bullet under an unrelated paragraph**, so the heading was replaced rather than dropped: `PATHWAY LANDS - READ THE COLOUR ON EACH ROW BEFORE YOU SPEND THE LAND DROP:`. The kept rung is verified live: `PLAY THIS AS A LAND` renders **108 occurrences in 76 prompts**, as option rows **30 (deck146 15, deck152 15)**, taken **15 (deck146 8, deck152 7)** - the guide's own `15 such rows this corpus, 7 taken` is exact. `Flip Side` is now **0** in the file. |
| **B3** | `deck152` 16 / `deck146` 12 / `deck162` 9 `WHAT THIS COST` sites | **ACCEPT IN PART - ONE receipt re-dated; the 37-site sweep is FILED, not executed** | The census reproduces exactly: **16 / 12 / 9**, and `WHAT THIS COST` is **0** in the other four guides. The one receipt the finding proves stale was fixed: deck146's `WHAT IT IS NOW, wave 56: this deck took it 5 of 20`. Re-derived on this corpus (unit: window whose prompt carries the live HOLD row; take = `chosen_text` prefix `Hold priority`): deck146 is **15 of 101 overall** and **0 of 8** at `[you declined this exact list N times]` with N >= 2 - so the live line is stale *in the direction that flatters the rule*. Re-dated to wave 57 with both denominators and the honest reading. **The other 36 sites were NOT rewritten**: a receipt-by-receipt re-measure is a seat's job over its own rules, not a boundary edit, and rewriting 36 dated-but-old receipts is precisely the text the doctrine correction forbids. Obligation 1. |
| **B4** | `deck125_strategy.txt` SIZE; all seven BASELINE | **ACCEPT AS MEASUREMENT - no guide edit; one filed number does not reproduce** | Both baseline columns re-derived from disk. **Six of seven both-column pairs reproduce to the byte** (deck123 +34/+108, deck126 +4,650/+4,650, deck130 +1,640/+1,757, deck146 +2,023/+2,298, deck152 +20/+387, deck162 +1,314/+1,314). **deck125 does not: the finding reports +978 against the deployed baseline and +1,005 against the reviewer file, but the two baselines are the SAME 69,761 bytes** - deck125 took zero bytes in the wave-56 boundary pass - so both columns are **+1,005**. The filed pool total **+10,686 (+2.35%)** is right and is itself computed with +1,005, so the +978 is a stray in the prose, not in the arithmetic. deck125's <= 0 obligation is **VIOLATED for a second wave (+1,005)**; restated, not escalated (#232). Obligation 2. |
| **B5** | `deck152_strategy.txt` - the undated `190 windows ... not one of them took it` receipt | **REJECT AS DRAFTED - the text is already gone; a two-word dating fix applied instead** | The finding's premise is disproved on the live file. `190 windows` and `not one of them took it` return **0 hits in deck152** (they survive only in deck146 at its own site). The wave-57 seat did not fail to date the receipt - **it deleted the receipt with the rung it defended and replaced it with a wave-57 measurement**: *"At this deck's seats this corpus, 73 windows carried it with N of 2 or more ... the 66 of them answered 'Cast nothing right now'"*. Re-derived: deck152 is **7 of 73 at N >= 2 (9.6%)** and **20 of 226 overall** - so 73 windows and 66 non-takes are **exact**, and they are the same two numbers the finding's own replacement string carries. **Writing that replacement would re-import a wave-55 scoreboard into the one file that has already retired it.** `WHAT IT IS NOW` returns 0 in deck152 because the `WHAT THIS COST` it would qualify returns 0. What is true in the finding is the #291 dating: the live sentence says "this corpus", which stops being a date the moment a corpus ships. Changed to **"in wave 57"** - two words, no new claim. |
| **B6** | `deck162_strategy.txt` :23 (`both are / correct`, wrapped) | **PASS confirmed - no edit** | Reproduced exactly. Single-line `/usr/bin/grep -i` over the #233 phrase list returns **0 in all seven guides**; the whitespace-normalised sweep returns **one hit in the pool**, deck162, and it is the wrap. Read in context it is a statement about a rendered line (*"'Mana available: N total (... colours you can make: {u}{b})' gives you two facts and both are correct"*), not a licence to answer two ways on one menu. **The pool remains CLEAN on #233, second wave.** The obligation is on the sweep, and this pass ran both forms for exactly that reason. |
| **B7** | `deck130_strategy.txt` - the marker rung | **REJECT - the live file already carries the sentence, written by its own seat THIS wave, and the drafted rule is WRONG on its own cited seq** | Two independent disproofs. (1) **Already present.** The live file at the `THE MARKER NOW PRINTS ON THE CAST ROW TOO` block reads *"'best trade' is printed even when every trade is bad - count the two lists yourself and compare their lengths before you number that row"*, and the whole `TWO CAST-ROW ANNOTATIONS MEAN "DO NOT CAST STARSTORM AT ALL"` block above it is **new in wave 57** (`git show a93aad889:` returns 0 hits for `TWO CAST-ROW ANNOTATIONS`, the live file returns 1). The seat wrote the arithmetic in the same wave the finding says it is missing. (2) **The drafted verdict is rules-wrong.** *"if the YOURS list is longer, the marker is NOT your answer and the next affordable X is"* - applied to the finding's own `130v152` seq 33, the marked row is `X = 4 {kills THEIRS: Sigarda; YOURS: Dwarven Blastminer, Siege-Gang Commander, Goblin x3}` and **the next affordable X is `X = 3 {kills THEIRS: none; YOURS: Dwarven Blastminer, Siege-Gang Commander, Goblin x3}`** - the same five of yours and none of theirs, strictly worse. The correct answer was one screen earlier at seq 31, where the cast row named Siege-Gang Commander in `YOURS:` and the live guide already says *do not cast Starstorm at all*. Marker census re-derived: `best trade` renders **2 occurrences, both at deck130**; the family that moved is `best X for this cast` at **408**; `THIS WINS THE GAME` **3**, taken 3 of 3. |
| **B8** | all seven - SIZE and BAND | **ACCEPT AS MEASUREMENT - no guide edit** | Every deployed size reproduces to the byte: deck162 **46.8**, deck152 64.1, deck130 68.0, deck146 68.0, deck126 68.5, deck125 69.1, deck123 **69.3 KB**; band **46.8-69.3 KB**, median 68.0, pool **+10,686 (+2.35%)** on the deployed baseline. Six of seven inside a 5.2 KB span, confirmed. After this pass: band **46.8-69.3 KB**, median **68.5 KB**, pool **464,821 B (+222)**. Table below. |
| **B9** | all seven - the unquoted live literals | **ACCEPT AS OBLIGATION - no guide edit this pass, and the #259 status of `they would be at` is now RE-DERIVED and CLEAN** | The counts reproduce exactly: `they would be at` **196 occurrences in 168 prompts** and **0 quotations in all seven guides**; `Their untapped sources:` **3,267 of 3,267 board frames**, **0 quotations**; `NO LIVE CAST ROW` **497**, **0 quotations**. **The material new fact is that #259's falsehood is fixed**: of the 196 renders, **196 of 196** compute the printed K from the record's own `opp_life` minus the printed damage (wave-55: 238 of 248 computed from a constant base of 166). So the line B9 wants taught is now TRUE, which is the precondition its remedy needed and did not state. This lane writes none of it: the remedy is addressed to the wave-58 brief, one line per reviewer, and this lane is barred from writing `they would be at N` teaching. Obligation 4. |

## The edits

Two guides, four sites, net **+222 bytes** across the pool.

### B1 - `deck146_strategy.txt`, inside the 146-A pay-3-life floor (3 lines added)

> `  REMOVE THIS FLOOR WHEN the "pay 3 life" row prints its own life verdict - re-verify by counting`
> `  "pay 3 life" option rows carrying "{this payment puts you at" in the corpus; it was 0 of 12 such`
> `  rows (104 prompts named the payment) in wave 57, and the docket asks for that verdict (D1).`

Placed directly under `"Your life:" is printed two lines above the menu and reading it is your job.`
The floor, its two conditions and the `NEVER PAY AT 3 LIFE OR LESS` line are untouched. The
denominator is this lane's, not the draft's: **12 option rows**, not 104 prompts.

### B3 - `deck146_strategy.txt`, the HOLD receipt (2 lines -> 4)

> `Dictate menu re-asked eight to ten times a turn for eleven turns. WHAT IT IS NOW, wave 57: this`
> `deck took it 15 of 101 offered rows overall, and 0 of the 8 windows where the declined line read N`
> `of 2 or more - the rule's own key had eight windows and took none of them, so the wide denominator`
> `is the only place it is working. The number is the measurement, not the story.`

The wave-55 cautionary tale above it is KEPT: #291's two-consecutive-passing-corpora clause is not
met (wave 56 took 5 of 20 at the narrow key, wave 57 took 0 of 8), so only the date moved.
**deck146: 69,626 -> 70,101 bytes (+475)** for B1 + B3.

### B2 - `deck152_strategy.txt`, the dead Flip Side block (4 lines -> 1)

> `PATHWAY LANDS - READ THE COLOUR ON EACH ROW BEFORE YOU SPEND THE LAND DROP:`

replacing the `"FLIP SIDE" - ONE narrow use on LANDS; ignore it everywhere else:` heading and the
whole Brutal Cathar / Tovolar display-toggle bullet. The Pathway bullet below it is untouched and
keeps its own measurement. `Flip Side` is now **0** in the file and **0** in the corpus.

### B5 - `deck152_strategy.txt`, the HOLD paragraph (two words)

`At this deck's seats this corpus, 73 windows` -> `At this deck's seats in wave 57, 73 windows`.
No claim added, no line added. **deck152: 65,648 -> 65,395 bytes (-253)** for B2 + B5.

## B8 - the measurement

`wc -c`, measured from the LIVE file. **Deployed baseline** = `a93aad889` (the wave-56 boundary
pass), which is the file the wave-57 corpus was served from, per #328; the reviewer-file column is
`wave56/deckN/strategy.txt`.

| pool guide | wave-56 deployed | wave-56 reviewer file | wave-57 deployed | after this pass | wave-57 net (deployed) | with this pass |
|---|---|---|---|---|---|---|
| `deck123_strategy.txt` | 70,887 | 70,813 | 70,921 | 70,921 | +34 | +34 |
| `deck125_strategy.txt` | 69,761 | 69,761 | 70,766 | 70,766 | **+1,005** | +1,005 |
| `deck126_strategy.txt` | 65,461 | 65,461 | 70,111 | 70,111 | **+4,650 (+7.1%)** | +4,650 |
| `deck130_strategy.txt` | 67,966 | 67,849 | 69,606 | 69,606 | +1,640 | +1,640 |
| `deck146_strategy.txt` | 67,603 | 67,328 | 69,626 | **70,101** | +2,023 | +2,498 |
| `deck152_strategy.txt` | 65,628 | 65,261 | 65,648 | **65,395** | +20 | **-233** |
| `deck162_strategy.txt` | 46,607 | 46,607 | 47,921 | 47,921 | +1,314 | +1,314 |
| **pool total** | **453,913** | **453,080** | **464,599** | **464,821** | **+10,686 (+2.35%)** | **+10,908 (+2.40%)** |

**The band after this pass is 46.8 - 69.3 KB, median 68.5 KB** (deck162 47,921; deck152 65,395;
deck130 69,606; deck146 70,101; deck126 70,111; deck125 70,766; deck123 70,921) - inside the pool
band 41-71 KB, and deck123 is now 0.1 KB under the ceiling. Restate it from this table, not from
memory (#232). **deck152 is the only guide this pass shrinks, and it is the only one it trims.**

## Re-derivations that did not reproduce (#208)

| claim, as filed | re-derived here | reading |
|---|---|---|
| B1 trigger: *"it was 0 of 104 in wave 57"*, unit stated as `"pay 3 life" rows` | **0 of 12 option rows**; 104 is the count of PROMPTS containing the literal (the row's own warning tail carries it too) | unit error in the falsifier itself - a trigger re-run on 104 would never match the count that produced it. Fixed in the shipped line |
| B4: deck125 *"+978 bytes against the deployed baseline ... +1,005 against `wave56/deck125/strategy.txt`"* | **+1,005 against both** - `a93aad889:deck125` and `wave56/deck125/strategy.txt` are both **69,761 B** (deck125 took 0 bytes in the wave-56 boundary pass) | prose stray only; the filed pool total +10,686 is itself computed with +1,005 and is exact. Obligation unchanged |
| B5: *"deck152 did not [date the receipt] - `WHAT IT IS NOW` returns 0 hits in the file"* | true, and it is the wrong inference: `190 windows` and `not one of them took it` also return **0** - the receipt was **deleted with its rung** and replaced by a wave-57 measurement (**73 windows / 66 non-takes**, both exact) | the finding's own replacement would regress the file. REJECTED as drafted |
| B7: *"the file defers to the marker without supplying the arithmetic"* | the arithmetic sentence is **live** (*"count the two lists yourself and compare their lengths before you number that row"*) and its whole block is **new in wave 57** (0 hits at `a93aad889`, 1 hit live) | REJECTED; and the drafted rule picks a strictly worse row on its own cited seq |
| B7 marker census: `best trade` *"6 times"* in deck130 (guide) and the marker *"408 windows on the cast row"* | guide quotations **6**, correct; the RENDER splits: `best trade` **2 occurrences**, `best X for this cast` **408** - two different literals | recorded so wave 58 does not key a rule to a literal that renders twice |
| B9 (implicit): `they would be at K` as a line worth teaching | **196 of 196 values now compute from the record's own `opp_life`** (wave-55 #259: 238 of 248 from a constant base of 166) | the #259 falsehood is FIXED. B9's remedy is safe on the data, and the finding did not check it |

## Sweeps run this pass

- **The wrapped-literal audit (#294), all seven files, COUNT PER FILE, whitespace-normalised.**
  Double-quoted spans / spans of 45+ chars / non-matching / template-shaped / hard residue:
  deck123 **352 / 81 / 45 / 30 / 15** | deck125 **264 / 57 / 37 / 24 / 13** | deck126
  **261 / 66 / 53 / 28 / 25** | deck130 **336 / 62 / 43 / 25 / 18** | deck146 **230 / 51 / 33 /
  13 / 20** | deck152 **264 / 72 / 47 / 16 / 31** | deck162 **190 / 33 / 23 / 13 / 10**, against
  40.4 MB of rendered prompt text. **Every hard-residue span was classified and NONE is a stale
  engine literal**: they are (a) PILOT plan sentences the guide is rebuking and (b) concrete
  instances of a live emitter family carrying prior-corpus card names and numbers. Each such family
  was checked to render THIS corpus: `{kills whichever you target:` **300**, `{X pricing:` **580**,
  `{leaves 0 of your` **402**, `[cost: Tap]` **256**, `[DRAW PRICE:` **139**, `INCOMING THIS
  COMBAT:` **113**, `Mana sources among those lands, counted by the engine:` **50**, `GANG BLOCK:`
  **48**, `{paying this taps:` **45**, `{right now: returns NOTHING` **16**, `[legendary: you
  already control` **14**, `{blocking trigger` **14**, `castable from exile` **10**. **The one dead
  literal in the pool was `Flip Side` (0 renders) and B2 removed it.** Nothing else to file.
- **ASCII**: `LC_ALL=C /usr/bin/grep -c -P '[^ -~\t\n]'` = **0** on each of the seven guides after
  the pass; `git diff -- projects/mtg/bin/Res/ai/baka/ | /usr/bin/grep -c $'\357\277\275'` = **0**.
- **Width**: **0 lines added by this pass exceed 100 columns** (longest added line **98**). The pool
  carries **54 pre-existing lines over 100** (deck123 3, deck125 13, deck126 9, deck130 12, deck146
  6, deck152 3, deck162 8), none touched here - obligation 5.
- **#233 forbidden phrases**, both single-line and whitespace-normalised: **0** by the single-line
  sweep in all seven; **1** normalised, deck162's wrap, adjudicated a false positive under B6. Clean.
- **#259**: **0** occurrences of `they would be at` in all seven guides before and after; no line
  added by this pass teaches, quotes or leans on a computed `at N` value.
- **#238 absence-assertion sweep** over the added lines: the only absence-shaped number this pass
  writes is B1's `0 of 12 such rows`, and it ships inside its own re-verification method and
  retirement trigger, which is the form #238 requires.
- **#285(c) removal machinery**: `REMOVE TH` was **0 in all seven** before this pass and is **1**
  after (deck146). The other six guides carry none - obligation 3.
- **`WHAT THIS COST` census**: deck146 12, deck152 16, deck162 9, and **0** in deck123/125/126/130.
- **HOLD conversion, re-derived for every seat** (unit: window whose prompt carries the live
  `Hold priority for the rest of this turn` row; take = `chosen_text` prefix `Hold priority`; N from
  `[you declined this exact list N times already this turn]`): deck123 **140/350 (40.0%)**, N>=2
  **14/51**; deck125 **119/749 (15.9%)**, N>=2 **42/275**; deck126 **3/499 (0.6%)**, N>=2
  **0/301**; deck130 **149/469 (31.8%)**, N>=2 **35/88 (39.8%)**; deck146 **15/101**, N>=2
  **0/8**; deck152 **20/226 (8.8%)**, N>=2 **7/73 (9.6%)**; deck162 **4/96**, N>=2 **0/5**. Pool
  **450/2,490** and **98/801** at N>=2. deck126 remains the pool's only zero-HOLD-text guide
  (`Hold priority` = 0) and the pool's worst converter, which is #312's live result.

## Obligations recorded

1. **B3 / wave 58 - the 37 `WHAT THIS COST` receipts belong to their seats.** deck152 (16),
   deck146 (12) and deck162 (9) each state, per rule, ONE receipt from the newest corpus with its
   wave named, and delete the rest. This pass re-dated exactly the one the finding proved false
   (deck146's `5 of 20` -> wave 57's `15 of 101` / `0 of 8`) and wrote nothing else, deliberately:
   re-measuring 36 dated receipts is a seat's judgement over its own rules.
2. **B4 / deck125 - the <= 0 net-change obligation is in its THIRD wave and was violated again
   (+1,005).** The wave-58 file states a net change **<= 0**, names the trims that paid for it
   (#302), and gives each trim its own falsifiable prediction (#270). This pass added **0 bytes** to
   deck125. Its growth trend (9.3 -> 9.3 -> 4.3 -> 1.4%) is right; the obligation is restated, not
   escalated.
3. **B1 / #285(c) - six of seven guides carry NO removal machinery.** Only deck146's floor was
   covered by a B-finding and only deck146 was given a trigger. The wave-58 seats state, per guide,
   either the removal condition of their newest hard rule or that the rule is unconditional and why.
   A boundary pass may not invent six triggers for six rules it did not write.
4. **B9 / wave 58 - the two clean unquoted literals, one line per reviewer.**
   `Their untapped sources:` (3,267 of 3,267 frames) belongs in every guide with an instant or a
   combat trick; `they would be at K` and its `THIS WINS THE GAME` flag (3 renders, 3 takes, 3 wins)
   belong in every guide that can attack or activate - **and this lane's re-derivation clears the
   #259 blocker: 196 of 196 values are now computed from the record's own `opp_life`.**
   `NO LIVE CAST ROW` (497 renders) stays UNTAUGHT until D2 ships (#324), and the wave-58 files say
   so explicitly so the omission is not read as an oversight.
5. **Standing - 54 lines over 100 columns are already in the pool.** No pass has ever added one, and
   no pass has ever removed one. Either the constraint is on added lines only (say so in the brief)
   or the wave-58 seats rewrap their own files; a boundary pass rewrapping 54 lines across seven
   guides would produce the largest diff of any pass for no behavioural claim.
6. **Carried - B6's converse.** A mechanical string sweep reports a hit only after its sense is
   read. Both sweep forms (single-line and whitespace-normalised) are run every wave, and a
   normalised-only hit is adjudicated in prose before any edit.

## Not verified / notes forward

- The four edits are derived from the live text plus the re-derivations above; **none is
  corpus-tested.** Falsifiable predictions for the wave-58 seats: (B1) `{this payment puts you at`
  renders on some `pay 3 life` row and the floor is deleted in that wave, or it renders 0 again and
  the floor stands; (B3) deck146's HOLD take rate at N >= 2 rises off 0 of 8, or the rung is re-keyed
  to the wide denominator it actually works on; (B2) no reply at a deck152 seat mentions `Flip Side`
  and the Pathway take rate holds at or above 7 of 15; (B5) the wave-58 audit finds no undated
  "this corpus" in deck152's HOLD paragraph.
- **What I did not verify.** I did not re-derive the arrival traces, the fallback census, the
  prompt-size figures, the mulligan numbers or any ledger item; I did not open `src/` and no verdict
  above rests on the emitter's code (every literal claim is from the rendered `prompt`); I did not
  check card facts against the primitives or Scryfall, and no edit in this pass asserts one - the
  four flagged citations (Intruder Alarm `:58850`, Thraben Doomsayer `:120583`, Sanguine Bond
  `:99004`, Exquisite Blood `:37966`) and the unverified Bloodline Keeper / 31-token chain reading
  remain OPEN for a wave-58 spot-check. I did not verify B3's *"two are already retirable under
  #291's two-passing-corpora clause"* - I checked only the one receipt I edited, and it is NOT
  retirable. I did not score any deck's rules and did not re-run the corpus.
- **I did not touch** `wave57/deckN/strategy.txt` (they are now intentionally one edit behind their
  deployed twins at two decks, which is what a boundary pass produces), the seats' `edit-texts.md`,
  the skill, the general strategy, or `wave58/reviewer-brief-clauses-DRAFT.md` (read, not applied -
  it is unruled).
- **I did not run wagic, build anything, or run a corpus.** No file under `src/` was touched, and no
  file outside `bin/Res/ai/baka/` and this document was written.
