# Wave-49 reviewer boundary pass (deck reviewer lane)

Scope: the six boundary findings B1-B6 in `wave49/synthesis-notes.md`, adjudicated by the
reviewer of each deck (reviewers own guides; synthesis only reports) and applied to the LIVE
guides in `projects/mtg/bin/Res/ai/baka/` (the deployed wave-49 reviewer revisions). Governing
amendments: **127** (a mitigation sentence is written about cards, never about the engine),
**103/128** (quoted render literals), **119/131** (odds-form mulligan sections; the twelve-hand
self-consistency check), and the owner's doctrine that guides impose no floors or hard caps.
Every cited line was re-read in the live file before the verdict. ASCII only;
`git diff -- projects/mtg/bin/Res/ai/baka/ | /usr/bin/grep -c $'\357\277\275'` = **0**; a
`[^\x00-\x7F]` sweep over all seven guides returns no file. No `src/` change, no commit, no
wagic run.

## Verdicts

| # | verdict | reason |
|---|---|---|
| B1 | **ACCEPT (both)** | deck123:569 confirmed verbatim ("the engine discards for you at end of turn") - a mechanism sentence that goes false when D4 routes the cleanup discard as an ask. Re-phrased to the card fact only. The synthesis's suggested tail "and you do not choose which" was NOT taken: it is the same mechanism claim in different words and goes false with D4 exactly as the original did. deck126:22-23 confirmed verbatim ("At your Upkeep the engine will offer you a single action ..."): describes D12's leak as the screen's design. Re-keyed on the row per the suggestion; the instruction (pass) and every other sentence of RULE #1 (why you never tap by hand, the ONE-source count, the card text) unchanged. |
| B2 | **ACCEPT (both), #131 run on both sections** | deck162:334-336 confirmed: "Never bottom SHIELD SPHERE ... never bottom your only punisher" beside "LANDS: bottom one ONLY when the keep would still hold FOUR OR MORE". Collision constructed: Sphere + one punisher + five lands at (keeping 5), bottom 2 - one land may go (keep holds 4), the second card has nowhere to come from. Re-shaped to a KEEP order (Sphere, only punisher, lands to four, spells by cost; a protected card goes under only when nothing below it is left) and the LANDS line's "ONLY" dropped since the order now carries it. deck152:29 confirmed: "a flat violation of this rule" - replaced with the odds sentence. **#131 on deck152 found two real disagreements**, both at (keeping 6): RULE -1's M-is-6 line (:12-16) ships an all-lands hand and a one-land hand under "would not cover any spell", while STEP 2 (:398-399, "or at (keeping 6)") sends those same hands to "TWO OR MORE LANDS: KEEP" and the ONE LAND "judgment call" branch. Fixed at STEP 2: RULE -1's M-is-6 line decides first at (keeping 6); the branches are for the untouched seven; the KEEP bullet now says "AND AT LEAST ONE SPELL" with "ALL LANDS: ship - it casts nothing" (the WHAT THIS COST at :28 opens with exactly that seven-land seven). Zero-land and one-land-with-coverage hands agreed at 7/6/4 in both files. **#131 on deck162: no disagreement on any of the twelve.** Noted, not edited: deck162 has no all-lands sentence at all ("TWO OR MORE LANDS: KEEP" is the only verdict a seven-land hand gets) - a single verdict, not a contradiction; the wave-50 reviewer may add the same one clause deck152 now has. Preventative both seats (no loss this corpus). |
| B3 | **ACCEPT** | deck123:84 confirmed: the quoted literal `x50` (line 585's DECIDING restatement already reads `x<N>`). Quoted the row's shape (`x<N>`) and added the one clause that says the number is the pilot's, from the PLAN line's subtraction - true today, true after D11 prints `x<N>` and the count. Line 98's `x50, x120 and x100` history left as history. Note: the row's other literal in the same quotation, "N is at most 200", is a second render string; left alone (no docket item changes it; #128's grep still matches). |
| B4 | **PASS (no edit), as instructed** | deck123:199-204 re-read: states the true recipient (the TARGET gains) without quoting or contradicting the row. Correct posture while D3 is open; stays true after. Nobody "fixes" it. |
| B5 | **ACCEPT (both), with one correction to the finding** | deck125:339 confirmed: "and it is a hard stop: do not cast Lightmine Field while ..." - the label replaced with the reason (costs the turn, deals nothing until they attack); the printed-count gate, the "Not at 0, not at 1, not at 2" line and the row quotation unchanged. deck123:168 confirmed to carry "THE HARD STOP, because this rule was broken at 14 against 6" - but it is the **Damnation** rule (RULE 3: do not cast Damnation when YOUR creature count is 4 or more), not the loop stop the finding describes; the loop stop (line 95, `PLAN: stop at M = ...`) carries no "hard stop" label. Same verdict on the sentence that is there: the label replaced with "THE STOP, and why"; the reason the rule already gives ("Four bodies is a board you win with ...") is the reason. The synthesis's suggested loop-stop wording was not applicable and not used. |
| B6 | **PASS (no edit), as instructed** | deck146 / deck152 / deck162 lanes 146-A/B, 152-B/D, 162-B: the UNTESTABLE-AT-THIS-SEAT flag is a seat-file obligation (#132) for the wave-50 seat, not guide wording. deck162's Rule 2 untouched (#102's exhaustion clause; D15). deck152's edits above are B2, not B6. |

## Edits applied, per guide

| guide | finding(s) | before -> after | byte delta |
|---|---|---|---|
| `deck123_strategy.txt` | B1, B3, B5 | :569 *"not this window unless you can cast what it draws - the engine discards for you at end of turn."* -> *"... - cards above seven leave your hand at end of turn."* · :84 *`e.g. "CHOICE: 2 (Create human with Thraben Doomsayer x50)"; the engine performs it N times ... N is at most 200]". Take THAT row, ONCE, with`* -> *`e.g. "CHOICE: 2 (Create human with Thraben Doomsayer x<N>)"; the engine performs it N times ... N is at most 200]" - the row's own example shows the format; the number is YOURS, from the PLAN line's subtraction. Take THAT row, ONCE, with`* · :168 *"THE HARD STOP, because this rule was broken at 14 against 6: if YOUR"* -> *"THE STOP, and why (this rule was broken at 14 against 6): if YOUR"* | 49,842 -> 49,946 (**+104**) |
| `deck126_strategy.txt` | B1 | :22-23 *"At your Upkeep the engine will offer you a single action: tapping Overgrown Battlement for green mana. Answer CHOICE: 0 (pass). Every time. Upkeep, Draw, the opponent's turn - pass all of them."* -> *"Whenever a window's only action row is "Add N green mana with Overgrown Battlement" - at your Upkeep, Draw, the opponent's turn, wherever it appears - answer CHOICE: 0 (pass). Every time."* (header line and the rest of RULE #1 unchanged) | 36,788 -> 36,782 (**-6**) |
| `deck162_strategy.txt` | B2 | :334-337 *"Never bottom SHIELD SPHERE - it costs ZERO mana, so it is free defence you can deploy on any turn no matter what your lands are doing - and never bottom your only punisher. LANDS: bottom one ONLY when ..."* -> *"KEEP in this order, so any N is buildable: Shield Sphere first (it costs ZERO mana, free defence on any turn no matter what your lands are doing), then your only punisher, then lands up to four, then spells by cost - a Sphere or your last punisher goes under only when nothing below it in the order is left. LANDS: bottom one when ..."* | 30,810 -> 30,940 (**+130**) |
| `deck152_strategy.txt` | B2 (+ #131) | :29 *"- a flat violation of this rule -"* -> *"- a fresh six is no likelier to hold two lands than that five, and is one card smaller -"* · STEP 2 (:398) gains *"At "(keeping 6)" RULE -1's M-is-6 line decides first (zero lands, all lands, or one land under "would not cover any spell" ships; the rest keeps); the branches below are for the untouched seven."* · *"TWO OR MORE LANDS: KEEP."* -> *"TWO OR MORE LANDS AND AT LEAST ONE SPELL: KEEP."* + *"ALL LANDS: ship - it casts nothing."* | 43,112 -> 43,423 (**+311**) |
| `deck125_strategy.txt` | B5 | :339 *"ITS GATE IS THE SWEEPER SECOND NUMBER, and it is a hard stop: do not cast Lightmine Field while"* -> *"ITS GATE IS THE SWEEPER SECOND NUMBER: it costs the turn and deals nothing until they attack, so do not cast Lightmine Field while"* | 40,013 -> 40,048 (**+35**) |
| `deck130_strategy.txt`, `deck146_strategy.txt` | - | untouched | 0 |

**Pool total: +574 bytes across five guides.** Every edit keeps the rule's action and its
printed-count key; only the mechanism claim, the render literal, the absolute label, or the
#131 contradiction changed.

## Not verified / notes forward

- B1: `deck123:569` no longer says who discards; when D4 ships the ask, the Skeins bullet may
  gain "the discard ask: bottom-order rules apply" - a wave-50 reviewer call, not owed now.
- B1: `deck126` RULE #1's header line still reads "AT YOUR UPKEEP, PASS" - true as a header
  (the pass is right at every such window) and left; when D12 holds the gate, the header names
  a window that no longer appears but instructs nothing false.
- B3: the second literal in the same quotation ("N is at most 200") is a render string with no
  docket item; #128's grep should include it on the next sweep.
- B5: the synthesis mis-identified deck123:168 as the loop stop; it is the Damnation rule.
  Recorded so the wave-50 seat does not look for a "hard stop" label on the loop rule.
- #131 on deck162: no all-lands verdict exists in the section (single verdict via "TWO OR
  MORE LANDS: KEEP"); not a contradiction, not edited - flagged for the reviewer.
- Card facts: none of B1-B6 turns on a card fact; nothing was re-verified against Scryfall.
