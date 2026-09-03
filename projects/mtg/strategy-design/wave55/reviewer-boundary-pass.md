# Wave-55 reviewer boundary pass (deck reviewer lane)

Scope: the nine boundary findings B1-B9 filed in `wave55/synthesis-notes.md` ("Guide boundary
findings for the deck reviewers (B1-B9)"), adjudicated by the reviewer of each deck (reviewers own
their guides; synthesis only reports) and applied to the LIVE guides in
`projects/mtg/bin/Res/ai/baka/` - the deployed wave-55 revisions, commit `860528eb5`. All seven
`wave55/deckN/strategy.txt` were re-verified byte-identical to their deployed `deckN_strategy.txt`
with `cmp` before this pass began (7 of 7 IDENTICAL).

Governing amendments (wave-55 edition, `wave55/strategy-writing-skill.md`): **233** (a guide that
states an equivalence changes nothing; only an imperative with a trigger literal and a NAMED WRONG
ANSWER moves a rate - and its mechanical grep for `both`, `either`, `also correct`, `is safe`,
`you may`), **238** (a guide may cite a count as evidence for a rule; it may NOT state a count as a
fact about the render, and an absence claim ships with its re-verification grep, its primitive and
its retirement trigger), **259** (a clause carrying a number the engine COMPUTED is verified against
the record's own fields, not merely counted - the `they would be at N` family computes from a
constant base of 166 and **no guide may teach that number**), **261** (deck125's
`=== SPHINX'S REVELATION - X IS ALWAYS 3 ===` rung is a mandate the corpus obeys 11/11; nothing in
this pass touches it), **263** (a stamp reports the fraction of its record kind that carries it -
`reveal_stall` is on 12 of 12 and selects nothing), **264** (a stale quoted literal is an AUDIT
DEBT, not a behaviour failure, and may never be used to argue a rule should be deleted), **265**
(where a rung held, register moves from rebuke to standard and the engine-side residue is stated as
not the pilot's problem), **266**; plus the carried **209**, **210** (a literal that gains a cause
clause is a NEW literal), **220** (a passing wave is the wave to write the fewest lines), **232**
(the size band is restated from measurement each wave; the discipline is a NET-CHANGE budget),
**187**, **177**, **208** (re-derive every N-of-M from the field the claim is about, and state the
field and the unit), **240** (count row literals from the rendered `prompt`, not `options_text`).

Every cited line was re-read in the LIVE file before its verdict. Every number a finding rests on
was re-derived on disk from `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-074354/` (42 seat
JSONL, 3,264 records) with python, stating the field and unit per #208; the guides themselves were
read with `/usr/bin/grep`. ASCII only:
`git diff -- projects/mtg/bin/Res/ai/baka/ | /usr/bin/grep -c $'\357\277\275'` = **0**, and a
`[^ -~\t\n]` sweep over each of the four edited guides returns **0** matches. No line added by this
pass exceeds 100 columns (longest: 99). No `src/` change, no build, no wagic run, no `git add -A`.

## Verdicts

| # | site | verdict | reason (disk evidence) |
|---|---|---|---|
| **B1** | `deck125` :552-557 (opponent-turn windows block) | **ACCEPT - EDITED** | Both halves are live and both are worse than filed. Re-read at :552: the guide still quotes `"Pass priority, and do not ask me again this turn unless the board changes (any change re-opens this window; you give up no cast)"`. Re-derived (field `options_text`, unit row-instance, kinds `ask`+`priority`): that string renders **0** times in 3,264 records; the live row renders **2,352** times and is **one form only** - `Hold priority for the rest of this turn: pass now, and do not ask me again unless the board changes (any change re-opens this window; you give up no cast) {taking this row skips the rest of this turn's identical windows}`, with the `{...}` tail on **2,352 of 2,352**. The seat economy reproduces to the row: **381** windows at N >= 2 carrying the row, **25** takes, **6.6%**, against deck123's **141 / 54 / 38.3%** - and `Cast nothing right now` is quoted **17** times in this same file. Quotation replaced verbatim and the equivalence replaced with an order and its named wrong answer (#233); `it is safe` removed in the same edit, so the #233 forbidden-phrase grep over this paragraph is now clean. |
| **B2** | `deck126` - the whole file | **REJECT on its premise; the numbers stand; NO EDIT** | The finding says *"The wave-55 guide adds a HOLD block, so this is a WATCH, not a debt."* **That is false on disk.** `/usr/bin/grep -n "Hold priority\|do not ask me again\|skips the rest of this turn"` over the deployed `deck126_strategy.txt` returns **nothing**; `/usr/bin/grep -i hold` returns 11 hits, every one of them the ordinary verb ("hold the Tribute", "Hold it: it is entry 8"). The seat's own `wave55/deck126/edit-texts.md` records six edits (D-A..D-F) and none is about the row. The behaviour figures DO reproduce (field `options_text` / `chosen_text`, unit window): **1 take in 155** N >= 2 windows, **0.6%**. So deck126 is not a wave-56 WATCH on a new block - it is still the pool's **zero-text control** for #233, and by accident rather than by design. No guide text was written here: writing a HOLD section is new prose no finding cites, and this deck's own seat argument (#243) is on record against opponent-turn text. Restated as an obligation below. |
| **B3** | `deck146` :35, `deck152` :508 and :515 | **ACCEPT - EDITED (3 sites, 2 guides)** | The scope tail is real and the quoted strings are dead. Re-derived (field `prompt`, unit clause-instance): the colour family renders **3** times this corpus and **3 of 3 carry a tail** - `(you have 1 land; no spell in it is castable off {G} alone on turn one)`, `(you have 2 lands; ... off {W} alone even with every land in this hand in play)`, `(you have 4 lands; ... off {W} alone even with every land in this hand in play)`. The count family renders **6** times in four forms, none of which changed. Both quoted strings (`... off {B} alone)` and `... off {W}{U} alone)`) render **0** times as written. deck126 :569 quotes the one-land branch WITH its tail and is the correct model, as filed. Worse than filed at deck152: the prose around the dead quote said the clause *"is a statement about TURN ONE and nothing else"*, which the 2+ tail now contradicts in the engine's own words - so this was a false gloss, not only an un-greppable quote. Both sites re-keyed and split on the tail; the :515 receipt is elided rather than re-quoted (see the edits). |
| **B4** | `deck130` :185-186 (X-rung STEP 1) | **ACCEPT - EDITED (wording diverges from the suggestion, and is smaller)** | The sentence is live and reads *"Last corpus every X menu this deck reached carried a marker - three of three -"*. Re-derived (field `prompt`, unit X-menu ask): deck130 reached **3** X menus this corpus and **3 of 3** carried a `[<- ` marker, so the count is TRUE and it is exactly the shape #238 forbids. **The count was cut and no universal was written in its place.** The suggested replacement (*"Every X menu this deck reaches carries exactly one marker"*) is itself a claim about the render, and it would contradict the rung's own absence branch, which is ALREADY LIVE fifteen lines below at :201-203 (*"If no marker is printed: the LARGEST 'kills THEIRS' list whose 'YOURS:' reads 'none'"*, plus the second level at :203-206 for when no row reads none). #238's conditional is therefore already satisfied; the cheapest edit that satisfies the amendment is the deletion alone (#220), and that is what was made: **-61 bytes**. |
| **B5** | `deck146` :118-121 | **PASS - re-verified, no edit; unit corrected** | Re-read live: *"the game NEVER offers you their land face - it has not appeared on a single land-drop menu"*. Re-verified this pass (field `options_text`, unit ask): **377** asks in the corpus offer at least one `Play <card>` row and **0 of 377** offer `Emeria, Shattered Skyclave` or `Agadeem, the Undercrypt`; the only rows naming either back face are **11** `Flip Side with ... -> DISPLAY TOGGLE only (this card's other face is a LAND)` rows, **all 11 at deck146's own seats**. The finding's "15 rows" does not reproduce - it is **11** on the row-instance unit - and the correction changes no verdict. #238's three conditions hold (grep, primitive `compare(isflipped)~equalto~1` at `borderline.txt:33968`, standing prediction P10). Obligation stays on ledger D8. |
| **B6** | `deck123` :128-142 and :805-810 | **PASS confirmed, no edit** | Both sites re-read. :131-133 carries the live HOLD literal in full and verbatim (verified byte-exact against the emitted string after whitespace-unwrap), followed by the imperative *"take that row at the FIRST dead window of the turn, not the fifth"*; :141 carries the #265 residue clause *"THE ROW IS NOT KEPT WHEN A LIFE TOTAL IS TICKING, AND THAT IS NOT YOUR PROBLEM TO SOLVE"*; :805-810 re-states it in the deciding-situations list with the declined note as *"a counter, not new information"*. **141 windows / 54 takes / 38.3%** reproduces exactly. The :805 quotation is deliberately elided (`"Hold priority for the rest of this turn: pass now, ..."`) and remains greppable by prefix, with the full string at :131 - that is the form this pass adopted at deck152 :515. |
| **B7** | `deck162` :471-486 | **PASS confirmed, no edit** | Re-read. The block is an imperative keyed to one literal (*"answer the HOLD row, not 'Cast nothing right now'"*), quotes the live row in full, states the absence branch (*"When that line is absent ... answer 'Cast nothing right now' as usual"*) and carries its WHAT THIS COST receipt. Re-derived: **65** N >= 2 windows at this seat, **0** takes, **0.0%** - the finding's denominator is exact. Nothing owed; wave 56 scores it. |
| **B8** | all seven `wave55/deckN/edit-texts.md` - STATED sizes | **PASS confirmed, and it is stronger than filed** | Five of seven state before/after bytes (123 and 130 state none). Measured: deck125 61,248 -> 66,855; deck126 56,650 -> 60,384; deck146 60,349 -> 64,114; deck152 60,009 -> 63,865; deck162 41,379 -> 45,365. **All five reproduce exactly on disk, and all five take their baseline from the LIVE file at review time (`dd0cd2d74`, the wave-54 boundary pass)** - which is wave-54 boundary obligation #4 landing at the seats. Synthesis's own B9 table did not do this: see B9. |
| **B9** | all seven `deckN_strategy.txt` - SIZE and NET CHANGE | **PARTIAL REJECT - the sizes are exact, four of seven net changes are measured from the wrong baseline** | The measured sizes reproduce to the byte (45.4 / 60.4 / 63.9 / 64.1 / 66.9 / 67.3 / 71.0 KB, band 45-71 KB, median 64.1 KB). **The net-change column does not.** It is measured from the wave-54 DEPLOYMENT commit `5160eb1cb`, not from the live file the wave-55 reviewers actually worked from (`860528eb5~1` = `dd0cd2d74`) - the identical error wave-54 B9(b) caught and wave-54 obligation #4 forbade in writing. Corrected: deck123 **-126** (filed -113), deck125 **+5,607** (filed +5,665), deck146 **+3,765** (filed +3,352), deck152 **+3,856** (filed +3,893); deck126 +3,734, deck130 +2,463, deck162 +3,986 are exact (their two baselines coincide). Pool: **+23,285 B (+5.60%)**, not +22,980 (+5.5%). **Every load-bearing claim survives the correction**: deck123 is still the only guide that shrank, deck125 still grew ~9.2% (+5,607 on 61,248) for a second consecutive wave, and the pool still grew at wave 54's rate on a bigger base. |

## The edits

Four guides, five sites, net **+709 bytes** across the pool.

### B1 - `deck125_strategy.txt` :552-559 (6 lines -> 8)

> `THE LAST ROW OF EVERY MENU, on their turn and on your own, reads "Hold priority for the rest of`
> `this turn: pass now, and do not ask me again unless the board changes (any change re-opens this`
> `window; you give up no cast) {taking this row skips the rest of this turn's identical windows}".`
> `When the prompt carries "[you declined this exact list N times already this turn]", THAT ROW IS`
> `THE ANSWER AND "Cast nothing right now" IS THE WRONG ONE - take it at the FIRST such window of`
> `the turn, not the fifth. It gives up nothing: a new object on the stack is a board change, any`
> `change re-opens the window, so taking it never costs you a spell they cast afterwards. What is`
> `never correct is casting something off one of these windows because the window opened.`

Three changes, in the order #233 ranks them. (1) The **imperative with its named wrong answer**
replaces *"Take it or take 'Cast nothing right now' - both are correct"*; the trigger is the
declined-note literal, which this seat sees on 381 windows at N >= 2. (2) The **live literal**
replaces the wave-54 string, verbatim and on its own, with the `{taking this row ...}` tail that
prints on 2,352 of 2,352 rows (#209/#258 - the tail is inside the string an existing rule keys on,
so the rule is re-read as the composed string). (3) *"and it is safe"* - a #233 forbidden phrase -
is gone; the same fact now hangs off the order as its reason (*"It gives up nothing"*), which is
what the amendment asks for rather than a second permission. The row's scope was also corrected:
the block said "OPPONENT-TURN MENU", and the row is LAST on **own-turn windows too** (2,352 of
2,352 across `ask` and `priority` records).
**deck125: 66,855 -> 67,098 bytes (+243).** This is the pass's only growth of any size, on the
pool's largest file, and B9's wave-56 obligation (**deck125 states a net change <= 0**) is
inherited by that seat unchanged - #220's byte donor is a seat-level judgment about which of this
file's recitals is closed, and a boundary pass may not spend another seat's deletion budget.

### B3 - `deck146_strategy.txt` :33-40 (5 lines -> 8)

> `  THAT LINE NOW PRINTS ITS REASON IN BRACKETS, AND ONLY ONE OF THE TWO REASONS IS A TEST.`
> `  "(1 land is not enough for your cheapest spell at mana value 2)" is the engine's arithmetic on`
> `  the land COUNT. The COLOUR reason ends with a scope clause, and that clause is the half that`
> `  decides: "(you have 1 land; no spell in it is castable off {B} alone on turn one)" prices TURN`
> `  ONE only, on a hand the land count may still keep; "(you have 4 lands; no spell in it is`
> `  castable off {W} alone even with every land in this hand in play)" prices the WHOLE hand.`
> `  NEITHER IS A SHIP: each is the sentence COLOUR IS NEVER A REASON TO SHIP forbids, printed by`
> `  the engine, and colour ships nothing at any land count.`

The 2+ branch is quoted from the string that **actually rendered at this deck's own seat** this
corpus (`4 lands ... {W} alone even with every land in this hand in play`, `146v152` opening ask)
rather than from an invented two-land {B} example; the one-land branch keeps the guide's own
colour, since no one-land {B} opener occurred. The verdict the paragraph teaches is unchanged -
colour is never a reason to ship - and the split now names which half of the clause the pilot is
reading. **deck146: 64,114 -> 64,394 bytes (+280).**

### B3 - `deck152_strategy.txt` :508-515 (6 lines -> 8) and :516-518 (the receipt)

At the family split:

> `    COLOUR FAMILY - "(you have 3 lands; no spell in it is castable off {W}{U} alone even with`
> `    every land in this hand in play)", and at ONE land the same clause ends "alone on turn one"`
> `    instead. This is THE FORBIDDEN THOUGHT below, printed by the engine in its own voice, and the`
> `    tail says how far the engine looked: "on turn one" prices your first turn only, "even with`
> `    every land in this hand in play" prices the whole hand. NEITHER IS A SHIP. It says your lands`
> `    make one pair of colours and your spells want another, [...]`

The retired sentence was *"It is a statement about TURN ONE and nothing else"* - true of the
wave-53 string, **false of the string the engine now prints at three lands**, which says the
opposite in its own words. That is why this site was a correctness fix and not only an audit debt.

At the WHAT THIS COST receipt (:516-518) the quotation is **elided, not re-written**:
`"(you have 3 lands; no spell in it is castable off {W}{U} alone ...)"`. The receipt records a hand
shipped in an EARLIER corpus, under the string as it printed THEN; quoting today's tail into it
would put words in the engine's mouth about a render that never happened, and quoting the dead
string in full leaves an un-greppable literal (#209). The ellipsis is the form deck123 :805 already
uses for the same problem. **deck152: 63,865 -> 64,112 bytes (+247).**

### B4 - `deck130_strategy.txt` :185-186 (2 lines -> 2, count deleted)

> `       STEP 1. THE MENU MARKS ONE ROW, AND THAT ROW IS THE ANSWER. The marker prints in one`
> `               of two forms:`

The two marker forms, the imperative (*"Answer the marked row's number and read nothing else"*),
the WHICH-not-WHETHER paragraph, the `X = 3` counter-example and both levels of the absence branch
are untouched: they were already correct, and the absence branch is the reason no replacement
universal was written. **deck130: 67,312 -> 67,251 bytes (-61)** - the only deletion in the pass,
and the only guide this pass shrank.

## B9 - the measurement

Seven pool guides, `wc -c`. The **live baseline** column is `860528eb5~1` (= `dd0cd2d74`, the
wave-54 boundary pass), which is the file each wave-55 seat actually revised - not the wave-54
deployment commit.

| pool guide | live baseline (`860528eb5~1`) | wave-55 deployed (`860528eb5`) | after this pass | wave-55 net | with this pass |
|---|---|---|---|---|---|
| `deck123_strategy.txt` | 71,128 | **71,002** | 71,002 | **-126** | -126 |
| `deck125_strategy.txt` | 61,248 | 66,855 | **67,098** | **+5,607 (+9.2%)** | +5,850 |
| `deck126_strategy.txt` | 56,650 | 60,384 | 60,384 | +3,734 | +3,734 |
| `deck130_strategy.txt` | 64,849 | 67,312 | **67,251** | +2,463 | +2,402 |
| `deck146_strategy.txt` | 60,349 | 64,114 | **64,394** | +3,765 | +4,045 |
| `deck152_strategy.txt` | 60,009 | 63,865 | **64,112** | +3,856 | +4,103 |
| `deck162_strategy.txt` | 41,379 | 45,365 | 45,365 | +3,986 (+9.6%) | +3,986 |
| **pool total** | **415,612** | **438,897** | **439,606** | **+23,285 (+5.60%)** | **+23,994 (+5.77%)** |

**The band after this pass is 45.4 - 71.0 KB, median 64.4 KB** (deck162 45,365; deck126 60,384;
deck152 64,112; deck146 64,394; deck125 67,098; deck130 67,251; deck123 71,002). Restate it from
this table, not from memory (#232); the five seats that quoted "41-71 KB" this wave were quoting
the previous measurement, which no longer has a member below 45.4 KB.

The other thirty guides in `bin/Res/ai/baka/` have never been through a review wave and are
unchanged: **3,277 B (deck8) to 18,249 B (deck139), median 8,866 B**. The distribution is still
**bimodal with an empty gap between 18.2 KB and 45.4 KB**, and the gap widened again this wave
because the floor of the pool rose (41.4 -> 45.4 KB). Thirty-seven `*_strategy.txt` files total
**711,011 bytes**.

**What was deliberately NOT done.** No prose was cut to move any file toward a band. The only
deletion in this pass is deck130's 61-byte count, and it deletes a sentence #238 forbids - not a
size trim. deck125 grew again here (+243) because the imperative #233 demands is longer than the
equivalence it replaces; the offsetting deletion is that seat's call in wave 56, not this lane's.

## Re-derivations that did not reproduce (#208)

| claim, as filed | re-derived here | reading |
|---|---|---|
| B2: "the wave-55 guide adds a HOLD block" to deck126 | **no HOLD block exists in the deployed file**: `Hold priority` 0 hits, `do not ask me again` 0 hits, `skips the rest of this turn` 0 hits; the seat's `edit-texts.md` records six edits, none on the row | **the finding is wrong on disk**; REJECTED on its premise. Its data (1 of 155, 0.6%) is exact, and deck126 remains the zero-text control |
| B9: pool net "+22,980 B (+5.5%)"; deck123 -113, deck125 +5,665, deck146 +3,352, deck152 +3,893 | **+23,285 B (+5.60%)**; deck123 **-126**, deck125 **+5,607**, deck146 **+3,765**, deck152 **+3,856** | four of seven nets measured from `5160eb1cb` instead of the live file at review time - wave-54 obligation #4 restated. No verdict changes |
| B5: "the only 15 rows naming them read `Flip Side ... -> DISPLAY TOGGLE only`" | **11** such rows (field `options_text`, unit row-instance), all at deck146's seats; the load-bearing half (**0 of 377** `Play <card>` asks offer a back face) is exact | unit correction only; PASS stands |
| B1/#233: HOLD takes counted from `chosen_text` | an **exact** match under-reports badly: corpus-wide the row is taken **282** times, and **134 of those records store `chosen_text` WITHOUT the `{taking this row ...}` tail** while the offered `options_text` row carries it on 2,352 of 2,352. Prefix-matching `Hold priority for the rest` reproduces every filed per-deck figure (123: 54, 125: 25, 126: 1, 130: 8) | **a silent instrument**: an exact-match take census reads 76 at N >= 2 where the true figure is 88. Wave-56 seats match the row by prefix and say so. Not a second render form - the emitter has one |

## Obligations recorded

1. **B2 / wave 56 (was a WATCH, is now a debt with a decision in front of it).** deck126 has **no
   HOLD text at all** and takes the row **1 time in 155** N >= 2 windows. Its seat must either
   write the imperative in #233's form (trigger literal, named wrong answer, absence branch - the
   deck162 block at :471-486 is the template) or record, in its own words, that #243 wins and this
   deck accepts a 0.6% rate as the price of the bytes. It may not be left un-decided a third wave.
2. **`deck130` :779 - a SECOND live dead quotation, filed but NOT edited (#264, #209).** The
   deciding-situations list still reads *"the last row reads 'Pass priority, and do not ask me
   again this...'"* - the wave-54 string, **0 renders**. No B-finding cites this site and this lane
   edits only what a finding cites, so it stands. The replacement is written out here so wave 56
   costs one grep: `Hold priority for the rest of this turn: pass now, and do not ask me again
   unless the board changes (any change re-opens this window; you give up no cast) {taking this row
   skips the rest of this turn's identical windows}`. **Synthesis's B1 REVERSE audit missed it** -
   the sweep found deck125's copy of the same dead string and stopped; a literal audit is a grep
   over ALL seven files per literal, and its result is a count per file, not a first hit.
3. **B4 / D6 (unchanged).** When the `[<- most kills ...]` marker is made unconditional, deck130
   STEP 1 collapses to the marker sentence alone and BOTH levels of the absence branch at :201-206
   are cut in the SAME wave (#220/#239).
4. **B5 / D8 (unchanged).** The day a land-drop menu offers `Emeria, Shattered Skyclave` or
   `Agadeem, the Undercrypt`, the deck146 block at :118-121 is deleted in that wave and P10 flips
   from "still 0" to non-zero. Re-verification this pass: 0 of 377.
5. **B9 / standing, and this is its second wave running.** The net-change baseline is the **live
   file at review time**, which is the previous wave's boundary-pass commit and NOT its deployment
   commit. The five seats that stated bytes got this right; synthesis's own table did not. Synthesis
   publishes the band and the pool net from a fresh `wc -c` against that baseline.
6. **B1 / wave 56.** deck125's file states a net change **<= 0** including the +243 this pass added,
   and names the recital it deleted to pay for it (#220).

## Not verified / notes forward

- The four edits are derived from the live text plus the re-derivations above; **none is
  corpus-tested**. Falsifiable predictions for the wave-56 seats: (B1) deck125's take rate at
  N >= 2 rises off 6.6% and its replies stop naming `Cast nothing right now` on windows carrying
  the declined note - the #233 experiment now has deck125 moved from the "optional" cell to the
  "imperative" cell with its denominator (381) unchanged, which is the cleanest single test the
  pool can run; (B3) no mulligan at deck146 or deck152 is decided by a colour clause, and no reply
  quotes the clause without its tail; (B4) deck130 answers a marked X row on every marked menu, and
  a menu with NO marker (none occurred this corpus) is answered off the :201 branch.
- **#259 compliance check on this pass**: no edit teaches, quotes or leans on a `they would be at N`
  value. That clause computes from a constant base of 166 on 238 of 248 rows and **no guide may
  teach the number until D2 is fixed**; the four edited paragraphs are about the HOLD row, the
  mulligan colour clause and the X marker, none of which carries it.
- **#238 mechanical sweep over the four edited guides** (numeral adjacent to a render string,
  `zero times`, `never printed`, `not there`, `does not exist`): one hit remains, `deck146` :271
  (*"a wall they already control is a name that fires zero times"*). Read live: it is a statement
  about a NAMED CARD's effect under the Silencer rule, not a claim about what the engine prints.
  Cleared, not edited.
- **#233 forbidden-phrase sweep** (`both are correct`, `it is safe`, `either is fine`, `you may`)
  over the four edited guides after the pass: **0 hits**.
- **I did not re-score any deck's rules**, did not touch `wave55/deckN/strategy.txt` (they are now
  intentionally one edit behind their deployed twins at four decks, which is what a boundary pass
  produces), did not edit `wave55/edit-texts.md` (they are the seats' own records and B8 rests on
  them), and did not edit the skill or the general strategy - both are synthesis's, and the two
  corrections above (the B9 baseline, the `chosen_text` prefix rule) are recorded here for it.
- **I did not run wagic, build anything, or run a corpus.** No file under `src/` was touched, and
  no file outside `bin/Res/ai/baka/` and this document was written.
