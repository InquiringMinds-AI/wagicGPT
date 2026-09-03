# Wave-54 reviewer boundary pass (deck reviewer lane)

Scope: the ten boundary findings B1-B10 filed in `wave54/synthesis-notes.md` ("Guide boundary
findings for the deck reviewers"), adjudicated by the reviewer of each deck (reviewers own their
guides; synthesis only reports) and applied to the LIVE guides in
`projects/mtg/bin/Res/ai/baka/` - the deployed wave-54 revisions, commit `5160eb1cb`. All seven
`wave54/deckN/strategy.txt` were re-verified byte-identical to their deployed
`deckN_strategy.txt` with `cmp` before this pass began.

Governing amendments (wave-54 edition, `wave54/strategy-writing-skill.md`): **209** (a guide
quotes an engine literal AS a literal - the audit that keeps it true is a grep), **210** (a
literal that gains a cause clause is a NEW literal; the audit gains a families column),
**215** (each step of an ordered procedure names the string on screen that satisfies it, because
the hardest step falls to the bottom in practice), **220** (a passing wave is the wave to write
the fewest lines; a closed lane's recital is the byte donor), **221** (a rule keyed to an
OPTIONAL annotation carries the absence branch and the reviewer reports the marker's RENDER
count), **223** ("WHAT THIS BOUGHT"), **227** (an exclusion rule states the rendered clause, not
a category), **232** (the size band is restated from measurement each wave and the discipline is
a NET-CHANGE budget); plus the carried **187** (a derivation a shipped literal now answers is CUT
in the wave the literal first renders), **177**, **188**, **208** (re-derive every N-of-M from
the field the claim is about, and state the field and the unit).

Every cited line was re-read in the LIVE file before its verdict. Every number a finding rests on
was re-derived on disk from `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-034355/` (42 seat
JSONL) with python, stating the field and unit per #208; the guides themselves were read with
`/usr/bin/grep`. ASCII only:
`git diff -- projects/mtg/bin/Res/ai/baka/ | /usr/bin/grep -c $'\357\277\275'` = **0**, and a
`[^ -~\t\n]` sweep over each of the four edited guides returns **0** matches. No `src/` change,
no build, no wagic run, no `git add -A`.

## Verdicts

| # | site | verdict | reason (disk evidence) |
|---|---|---|---|
| **B1** | `deck146` cast-order entry 5, live :149-167 | **ACCEPT - EDITED** | The false assertion is live, and it is false TWICE OVER. Re-read at :152-154: *"The row prints its card text and its `{leaves N of your M` clause and NOTHING ELSE: there is no `{right now:` clause on this card and there never will be, so do not wait for one"*, followed by the log name-search paragraph. Re-derived (field `options_text`, unit row-instance): the corpus holds **11 `Cast Silverquill Command` rows** and **11 of 11 carry `{modes live right now: ...; dead (no legal object right now): ...}`** - and **4 of the 11 ALSO carry a standalone `{right now: returns NOTHING - your graveyard has no creature cards, so the return part cannot happen at any X}`**, which answers the return-mode question the guide sends the pilot to the log for. (The other 7 Silverquill Command strings in the corpus are bare names on mode/discard rows, not cast rows - the finding's "11 of 11" is exact on the cast-row unit.) The guide quotes `modes live right now` **0 times**. Wave-53 B6 named this obligation; only its first half was honoured. Edited per the finding, and the name-search paragraph cut in the same edit (#187). |
| **B2** | `deck125` :499 (opponent-turn windows block) | **ACCEPT - EDITED** | The dead quotation is live. `/usr/bin/grep -n "Hold priority"` over all seven pool guides returns **exactly one hit, `deck125_strategy.txt:499`**; `"Pass priority, and do not ask me again"` returns three hits, all in deck123 and deck130. Re-derived over all 42 logs: the old string renders in **0** prompts, the new one in **426**; the emitted row has **exactly one form**, `Pass priority, and do not ask me again this turn unless the board changes (any change re-opens this window; you give up no cast)`. Quotation replaced verbatim; the surrounding teach (safe decline, any change re-opens) is correct and untouched apart from re-wrapping. |
| **B3** | `deck123` :850-856 (blockers-hint bullet) | **ACCEPT - EDITED (wave-53 B3 obligation now due)** | D21 shipped and the derivation is now over an absent literal. Re-derived (field `prompt`, unit blockers record): the `NOT lethal` header renders in **32** blockers prompts in two forms - **18** with the `taking damage while ahead on LIFE is often correct` half, **ALL 18 at `my_life > opp_life`**, and **14** short (`- NOT lethal: block only where the trade favors you.`), **ALL 14 at `my_life <= opp_life`**. A clean partition. The bullet still told the pilot to *"read it against the two life totals above it"* - six lines of arithmetic over a clause the engine no longer prints in that case. Replaced with wave-53's own suggested wording; the 7-against-12 citation kept. |
| **B4** | `deck152` :603 (`COUNT THE DISTINCT NAMES`) | **ACCEPT - EDITED (wave-53 B7 obligation now due)** | D17 shipped. Re-derived (field `prompt`, unit attackers record): **99 of 99** attackers prompts carry the header, **27 of 27** at deck152's own six seats, in three forms - `They have N untapped creatures able to block; declaring more than N attackers leaves at least (your attackers - N) of them unblocked.` (19), its singular form at N of 1 (28), and at N of 0 `... able to block: every attacker you declare this turn is unblocked unless something changes first.` (49). **The header prints the surplus arithmetic itself**, so the finding's suggested second sentence is also a derivation and was not written. The guide still asked the pilot to count distinct names across the A-lines - #215's hardest step. Cut; N is now read off the header. The finding cited only the one site; the reviewer's own sweep found a SECOND (the DECIDING SITUATIONS entry at :656 counting the same names and closing "never counts against B") and re-pointed it in the same edit (#177). |
| **B5** | `deck130` X rung STEP 1, live :178-188 | **PASS (no edit), recorded; obligation on D6** | Re-read. The rung keys to the marker, then states *"THE MARKER IS OFTEN NOT THERE ... across a whole corpus of twelve games it appeared ZERO times. Do not wait for it and do not read its absence as 'no row is good'"*, and carries a complete two-level absence branch (largest `kills THEIRS` whose `YOURS:` reads `none`; and when no row reads none, never take a row whose `kills THEIRS` reads `none`). Print condition, measured render count, full fallback - #221's three requirements, written before the amendment existed. Re-derived: `most kills at any affordable X that costs you nothing` renders in **0 of 2,306 records** this corpus too, so the recital's "ZERO times" is still true and cannot be retired yet. At D6 it is the byte donor (#220). |
| **B6** | `deck123` A54-3 / A54-7 | **PASS on the edits, WATCH recorded - and the fallback split is corrected on disk (#208)** | Both edits re-read live: the PLAN template at :109-114 and the `CL0`-`CL8` relabelling at :368-387 with the explicit *"THESE LABELS ARE 'CL0' TO 'CL8' AND THEY ARE NOT CHOICE NUMBERS"* line. No edit is owed. **But the finding's counts do not reproduce.** Re-derived (field `fallback`, unit decision record): corpus **23** fallbacks - `plan_choice_conflict` 12, `named_row_reask` 7, `named_row_not_offered` 2, `stale_livelock` 1, `all_assignments_illegal` 1. At deck123's own six seats: **15**, not 17 - `plan_choice_conflict` **7** (the finding's 7 is exact), `named_row_reask` **6** of the corpus's 7 (not "all 7"), `named_row_not_offered` **2** (both at `123v125`, seq 58 and 107 - these ARE the two Baka executions and they are at this seat as filed). Direction and mechanism unchanged; the "17 of 23" and "all 7" figures should not be quoted forward. Wave-55 scores the guide half and the parser half separately, as the finding says. |
| **B7** | `deck162` 162-A, `deck152` 152-B | **PASS on both (no edit), DIRECTION note carried** | 162-A re-read at :85, :136-137, :193: the brake is keyed to the printed `{feeds:` clause, with the non-gift discriminator stated ("Its row prints no `{feeds:` clause because it hands them nothing") - #227 exactly. Re-derived: **66** `{feeds:` row-instances corpus-wide, so the key is live. 152-B re-read at :13-18 and :474-500. **The dependency the finding names is real and re-derived**: the coverage line renders **11** times in **two families** - **4** count-failure (`(N land(s) is/are not enough for your cheapest spell at mana value M)`) and **7** colour (`(you have N lands; no spell in it is castable off <colours> alone)`), which call for opposite decisions (#210). D12 has not shipped, so **152-B stays exactly as written** and no byte was touched. |
| **B8** | `deck126` D100/D101, `deck162` 162-C | **PASS (no edit), recorded** | deck126's `[named:` bullet re-read at :584-595, sized to the #198 evidence and pointing at the OPPONENT battlefield line (#219); the tag renders in **146** prompts, so the key is live. deck162's eleven-window recital is gone from the live file (`edit-texts.md` 162-C records the retirement) and paid for that guide's two new findings. Nothing owed. |
| **B9** | all seven `edit-texts.md` STATED sizes | **PARTIAL REJECT - one of the three sub-items is wrong on disk** | (a) `deck152` states its revised file at **59,215 B**; on disk before this pass it was **59,972**. **CONFIRMED** - the 152-D edit landed after the header was written. (b) `deck130` "states its baseline at 61,789 B; the wave-53 file is 61,746." **REJECTED.** `git show baf36b936:...deck130_strategy.txt \| wc -c` = 61,746, but that is not the file the reviewer worked from: the wave-53 BOUNDARY PASS (`a0067bae5`, the B5 step-0b relabelling) took it to **61,789**, and `git show 5160eb1cb~1:...deck130_strategy.txt \| wc -c` = **61,789**. The reviewer's baseline and its stated delta (+3,060) are both correct against the live file; synthesis's B10 row (+3,103) is measured from a stale anchor. Both numbers are true of different baselines and the live-file one is the right one. (c) the quoted bands: `deck125` and `deck126` quote "39.6-70.2 KB", `deck146` quotes "39.6-74.7 KB" - **two distinct bands across three files** (not three), and **neither matches the measured distribution**. CONFIRMED in substance. |
| **B10** | all seven `deckN_strategy.txt`, SIZE | **PASS as a MEASUREMENT finding; no prose cut to chase a band** | Measured below. The band and the +19,028 B pool growth reproduce to the byte against the wave-54 deployment. Nothing was deleted to move a file toward a number; the 413 bytes this pass removed from deck146 are the retirement of a false assertion and its dead procedure (#187 + #220), not a size trim. |

## The edits

Four guides, six sites, net **-305 bytes** across the pool.

### B1 - `deck146_strategy.txt` :149-167 (19 lines -> 15) and :167 cross-reference

The entry now reads the clause instead of denying it:

> ` 5. Silverquill Command - when at least ONE of its modes is live: the sacrifice mode, the pump`
> `    mode, THE RETURN MODE. You do not work any of them out yourself. The row prints a clause that`
> `    opens "{modes live right now:" and, after a semicolon, "dead (no legal object right now):".`
> `    Everything before that semicolon is live this window; everything after it is dead. Read the`
> `    live half and look for the words "return creature" in it - that, and nothing in the log, is`
> `    how you know the return mode is live. [...] When the return mode is dead the row often says`
> `    so outright in a second clause: "{right now: returns NOTHING - your graveyard has no creature`
> `    cards, so the return part cannot happen at any X}". Do not reconstruct your graveyard from`
> `    the log lines above the menu; the row has already done it, on every Command row it prints.`

Cut in the same edit (#187): the "there is no `{right now:` clause on this card and there never
will be" sentence, the whole name-search procedure ("if the log has printed ... going to YOUR
graveyard (died, was countered, was destroyed, was sacrificed) ..."), and its `WHAT THIS COST`
recital, which existed only to enforce that procedure (#220 - the lane closed, the recital is the
donor). The two mana-value-2 creature names are KEPT: they are a deck fact, not a derivation, and
they tell the pilot which live pair it is looking at. Entry 8b's back-reference was re-pointed
from *"with NONE of its three conditions true"* to *"with none of its three modes live"* so no
cross-reference names a test the entry no longer performs (#177).

**deck146: 60,762 -> 60,349 bytes (-413).**

### B2 - `deck125_strategy.txt` :499-504 (paragraph re-wrapped, quotation replaced)

> `THE LAST ROW OF AN OPPONENT-TURN MENU reads "Pass priority, and do not ask me again this turn`
> `unless the board changes (any change re-opens this window; you give up no cast)". It is a decline`
> `row like "Cast nothing right now", and it is safe: a new object on the stack is a board change,`
> `so taking it never gives up a window on a spell they cast afterwards. [...]`

The literal is verbatim and on its own, and nothing outside the quotes restates what is inside it
(#209). No other word of the block changed; the paragraph was re-wrapped because the replacement
string is longer than the retired one.

**deck125: 61,190 -> 61,248 bytes (+58).**

### B3 - `deck123_strategy.txt` :850-856 (7 lines -> 7)

> `- The blockers prompt says "- NOT lethal: block only where the trade favors you." and stops`
> `  there. That is the form it takes whenever their life is at or above yours: the engine does not`
> `  print the "taking damage while ahead on LIFE is often correct" half unless you are the player`
> `  it describes. A token goes in front of the biggest attacker, because a 2/2 you can remake is`
> `  not a cost and the "you would be at" number is the life your combo has to live on next turn.`
> `  At 7 against 12 this deck answered "BLOCKS: none" to a lone 6/5 with a Vampire untapped, went`
> `  to 1, and lost with the Intruder Alarm still in hand.`

The bullet is now a recognition of a printed FORM (which of the two the engine chose) rather than
a derivation over two life totals. The 18/14 partition above is what licenses "at or above yours".
The citation is kept, per the finding.

**deck123: 71,115 -> 71,128 bytes (+13).**

### B4 - `deck152_strategy.txt` :603-606 and :656-660

At the attack rule:

> `  BLOCK EVERYTHING. The attackers header prints the count for you: "They have N untapped`
> `  creatures able to block; declaring more than N attackers leaves at least (your attackers - N)`
> `  of them unblocked." Read N off that line - never re-derive it by counting names across the`
> `  A-lines. The surplus above N is unblocked, its full power lands on the opponent's life, and`
> `  the gain clauses on the other tags never fire at all. [...]`

and at DECIDING SITUATIONS, the same derivation re-pointed:

> `- Their line shows ONE converter name, you have more attackers than the N in the attackers`
> `  header's "They have N untapped creatures able to block", [...] never counts against N.`

The symbol `B` is retired from the guide; `/usr/bin/grep -n "distinct names\|call it B\|counts
against B"` now returns nothing. The rest of the rule - the gain-clause price, the hold condition,
both `WHAT THIS COST` recitals - is untouched: they are still doing work (#220 retires a recital
only when its lane closes, and the price rule's lane did not).

**deck152: 59,972 -> 60,009 bytes (+37).**

## B10 - the measurement

Seven pool guides, `wc -c`, wave-53 deployment through this pass:

| pool guide | wave-53 live (`5160eb1cb~1`) | wave-54 deployed (`5160eb1cb`) | after this pass | wave-54 net | with this pass |
|---|---|---|---|---|---|
| `deck123_strategy.txt` | 70,230 | 71,115 | **71,128** | +885 | +898 |
| `deck125_strategy.txt` | 55,975 | 61,190 | **61,248** | +5,215 | +5,273 |
| `deck126_strategy.txt` | 53,926 | 56,650 | 56,650 | +2,724 | +2,724 |
| `deck130_strategy.txt` | **61,789** | 64,849 | 64,849 | **+3,060** | +3,060 |
| `deck146_strategy.txt` | 58,288 | 60,762 | **60,349** | +2,474 | +2,061 |
| `deck152_strategy.txt` | 57,075 | 59,972 | **60,009** | +2,897 | +2,934 |
| `deck162_strategy.txt` | 39,649 | 41,379 | 41,379 | +1,730 | +1,730 |
| **pool total** | **396,932** | **415,917** | **415,612** | **+18,985 (+4.8%)** | **+18,680 (+4.7%)** |

Two corrections to the finding's table, both #208/#209 shape and neither changing a verdict:

1. **deck130's baseline is 61,789, not 61,746** (B9(b) above). The pool's wave-54 net change is
   therefore **+18,985 B (+4.8%)**, not +19,028 (+5.5%); the 43-byte difference is wave-53's own
   boundary-pass edit being counted twice, and the percentage differs because the correct
   denominator is 396,932 rather than 370,485. The finding's substance - the pool grew by roughly
   5% in a wave where four seats said their guides were at their ceiling - stands unchanged.
2. **The band measured after this pass is 41.4 - 71.1 KB, median 60.0 KB** (deck162 41,379;
   deck126 56,650; deck152 60,009; deck146 60,349; deck125 61,248; deck130 64,849; deck123
   71,128). "40-70 KB, median 59 KB" is the right shape and is now a byte or two off at both ends;
   **restate it from this table, not from the last one** (#232). The three bands quoted from
   memory in the edit-texts ("39.6-70.2" x2, "39.6-74.7" x1) match neither.

The other thirty guides in `bin/Res/ai/baka/` have never been through a review wave and are
unchanged: 3,277 B (deck8) to 18,249 B (deck139), median 9.5 KB. **The distribution is still
bimodal with an empty gap between 18.2 KB and 41.4 KB**, and #146's retired "27-38 KB" sits inside
that gap. Directory total after this pass: **687,017 bytes across 37 files**.

**What was deliberately NOT done.** No guide prose was cut to move any file toward any band. The
only deletion is deck146's, and it deletes a FALSE assertion plus the procedure that assertion
existed to justify - the cheapest 413 bytes in the pool by #220's own test, since the lane that
recital defended closed the moment lane C's clause shipped.

## Re-derivations that did not reproduce (#208)

| claim, as filed | re-derived here | reading |
|---|---|---|
| B6: "**17 of the corpus's 23 fallbacks**" at deck123's seat; "**all 7** `named_row_reask`" | **15 of 23** at that seat; `named_row_reask` **6 of 7** (the seventh is elsewhere); `plan_choice_conflict` **7** exact; `named_row_not_offered` **2**, both at this seat, and both ARE the corpus's two Baka executions | mechanism and both edits unaffected; the two aggregate figures should not be quoted forward |
| B2: deck125's seat saw "**132 HOLD renders and 6 takes**" | **109 renders / 5 takes** at deck125's own six seat logs (field `options_text` rows / `chosen_text`); corpus-wide **426 / 43**, which reproduces exactly | a unit difference at the seat level only; the load-bearing half (old string 0 renders, new string 426) is exact |
| B9(b): "deck130 states its baseline at 61,789 B; the wave-53 file is 61,746" | the file the reviewer worked from **was** 61,789 (`5160eb1cb~1`); 61,746 is `baf36b936`, before wave-53's own boundary pass | **the finding is wrong on disk**; REJECTED, and B10's +19,028/+5.5% inherits the error |
| B9(c): "three files quote three DIFFERENT pool bands" | three files quote **two** distinct bands | substance (none matches the measurement) confirmed |
| B10: pool grew "+19,028 B, +5.5%" | **+18,985 B, +4.8%** against the correct baselines | same finding, corrected arithmetic |
| B1: "11 of 11 Silverquill Command rows" | **11 of 11 CAST rows** (the corpus holds 18 strings containing that card name; 7 are bare names on mode/discard rows) | exact on the unit the finding means; the unit is now stated |

## Obligations recorded (strings that do not print yet)

1. **B5 / D6** - when `[<- most kills at any affordable X that costs you nothing]` is made
   unconditional, deck130 STEP 1 collapses to *"Exactly one row on every X menu carries a marker.
   Answer that row."* and the ZERO-times recital plus both absence branches are cut in the SAME
   wave (#220). Until then the recital is TRUE (0 renders again this corpus) and stays.
2. **B7 / D12** - if and only if the coverage clause is split or scoped, 152-B re-keys to the
   scoped literal and its family-disambiguation paragraph is cut (#187). **If D12 does not ship,
   152-B does not change** - it is the only thing standing between this deck and a fifth-corpus
   mulligan break.
3. **B6 / wave-55** - the deck123 seat file reports `named_row_reask` at that seat and
   `plan_choice_conflict` firings whose re-ask returns the byte-identical reply as TWO separate
   counts, so the guide half (A54-7) and the parser half can be scored independently. It is a
   composition seam; either half can look sufficient alone.
4. **B9 / wave-55 brief** - every seat states before/after bytes by running `stat -c%s` on the two
   files, and takes the band from the B10 table above rather than from memory. The baseline is the
   LIVE file at review time, which is `5160eb1cb~1` and not the previous wave's deployment commit
   - that is exactly what the deck130 sub-item got wrong at synthesis.
5. **B10 / standing** - synthesis publishes the band and the pool's net change each wave from a
   fresh `wc -c`, and a guide whose predictions all passed states what it deleted (#220).

## Not verified / notes forward

- The four edits are derived from the live text plus the re-derivations above; **none is
  corpus-tested**. Falsifiable predictions for the wave-55 seats: (B1) deck146 answers a live
  Silverquill Command row without any reply prose reconstructing its graveyard from the log, and
  the "no creature cards with mana value 2 or less" confabulation does not recur on a row whose
  live half names a return pair; (B3) deck123 blocks at least one attacker in a `my_life <=
  opp_life` window that prints the SHORT `NOT lethal` form; (B4) deck152 writes the header's N
  rather than a name count, and does not decline an attack whose surplus the header prices as
  unblocked.
- **I did not re-score any deck's rules**, did not touch `wave54/deckN/strategy.txt` (they are now
  intentionally one edit behind their deployed twins, which is what a boundary pass produces), and
  did not edit the skill or the general strategy - both are synthesis's, and the two corrections
  above (the #232 band, the baseline rule) are recorded here for it, not applied.
- **I did not run wagic, build anything, or run a corpus.** No file under `src/` was touched.
- The `wave54/deckN/edit-texts.md` files were READ for B9 and not corrected: they are the seats'
  own records of what they did, and rewriting a seat's header after the fact would destroy the
  evidence B9 rests on. The correction lives in this file and in the wave-55 brief obligation.
- Over-long lines: no line added by this pass exceeds 100 columns. The pre-existing over-length
  lines noted in the wave-53 pass were not re-wrapped - cosmetic, and they belong to their owning
  reviewers.
