# Wave-57 synthesis (2026-09-03) — FOURTEENTH FAIR-HAND CORPUS: the instrument is the cleanest it has ever been (450 of 450 exact, 21/21 natural, 0 SEGV) and it is what let three separate menus be caught printing a number that was FALSE in the direction that kills — while the wave's biggest behavioural result closes an experiment instead of adding a rule

Inputs read, in order: `wave56/synthesis-notes.md` (the template for this step: inputs, outputs,
ownership doctrine, reconciliation method); `~/.claude/skills/wagicgpt/SKILL.md`;
`wave56/engine-ledger.md` (the wave-57 docket D1-D44 this corpus adjudicates); `wave57/review-carry.md`;
`wave57/reviewer-brief.md`; `wave57/owner-decisions.md` (three owner rulings, and the verbatim framing
**"on the vita, ive been playing only against the heuristic ai"**); the four seat files
(`seat-engine-narration.md`, `seat-123-130.md`, `seat-125-126.md`, `seat-146-152-162.md`); all seven
`deckN/edit-texts.md`, `deckN/skill.md` and `deckN/general-suggestions.md`;
`wave57/lane-{A,B,C,D,E,F,G,H,S,T,U,V}.md` — **lane V is the Baka blocker policy, merged AFTER the
corpus binary and NOT in this corpus; lane S is the vpk16 softlock, NOT reproduced** — and the corpus
itself at `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-174505/`: **42 seat JSONL and 21
`game-*.stderr`, every load-bearing number re-derived on disk** with python over the JSONL,
**counting rows from the rendered `prompt`** (never from `options_text`), **EXACT-matching HOLD takes**
(lane A's D4 made this possible for the first time — see 1 below), **naming the tag family in every
verdict census** (#298), **normalising whitespace before counting a quoted guide literal** (#294), and
`/usr/bin/grep` over the stderr with the emitter's own quoted line (#295). The seven DEPLOYED guides
were read for the boundary pass. Nothing was built, run, deployed or committed; **no file under
`projects/mtg/bin/Res/**` or `src/**` was modified; no git command was run; wagic was not run.**

Outputs: `wave57/strategy-writing-skill.md` (the wave-56 edition **byte-verbatim** — the
**1,265,202-byte prefix is `cmp -n 1265202`-verified, no difference** — plus **amendments 301-329**),
`wave57/general-strategy.md` (R1-R292 each with a wave-57 verdict, **R293-R318** new and layer-routed,
with the rejections reasoned), `wave57/engine-ledger.md` (**the wave-58 docket D1-D45**, ranked,
engine/UX only, each with a record citation and a falsifiable prediction; both discharge tables, with
lane V's predictions carried as **D22-D24** with its A/B numbers; fallback classes; per-kind prompt
chars; specimens), and the boundary findings **B1-B9** below.

**Ownership (owner doctrine 2026-08-26).** Reviewers own every `deckN/strategy.txt`; this step owns the
skill and the general strategy; the core loop — engine correctness, LLM UX, bugs — is the purpose.
**No guide file was edited by this step**, and none could have been: the wave-57 guides are already
deployed — `bin/Res/ai/baka/deckN_strategy.txt` verified **byte-identical** to
`wave57/deckN/strategy.txt`, **all seven, by `md5sum`** (deck123 `de16e62e…`, deck125 `8fb8889f…`,
deck126 `8a704641…`, deck130 `2a35c3c2…`, deck146 `3fd338cc…`, deck152 `21c3bc64…`, deck162
`5fa26c17…`). Step 4 is a REPORT, so every finding below is an obligation for wave 58, not a request
to change a pending edit.

---

## Reconciliation against the records

1. **Health, re-derived over all 42 JSONL, and the exact-match census reads 450 of 450.** 3,408
   records = 42 `gamestart` + **42 `gameend`** + 7 `recovery` + **3,317 decisions** (ask 2,642 /
   priority 481 / attackers 76 / discard 66 / blockers 35 / reveal 13 / bottom 4). **21/21 natural —
   invariant 00 satisfied**, **0 `Segmentation` in 21 stderr**. **8 fallbacks = 0.24%**
   (`named_row_reask` 6, `retracted_choice` 1, `unparsed_reply` 1) — **no `empty_reply`, no `timeout`,
   no `plan_choice_conflict`, no `all_assignments_illegal`, no `stale_livelock`, no
   `abandoned_in_flight`. Baka executed 0.** `recovery` pairing **7/7, 0 unpaired**.
   Inference **25.38 h = 27.55 s per decision**.

   **The task's question, answered directly: an exact HOLD census DOES read 450/450.** The row literal
   `Hold priority for the rest of this turn: pass now, and do not ask me again unless the board changes
   (any change re-opens this window; you give up no cast) {taking this row skips the rest of this
   turn's identical windows}` renders **2,490 times in 2,490 windows**; **450 takes**; and every one of
   the 450 `chosen_text` values matches its own rendered row **byte-for-byte, 450 of 450**. Lane A's D4
   is closed, the wave-55/56 tail-less-`chosen_text` defect is gone, and **every exact-match literal
   census in this wave's four files is trustworthy for the first time in three waves.** (One
   book-keeping note that matters for anyone re-running it: 2,503 windows contain the substring
   `Hold priority` — thirteen of them carry it only inside an echoed PLAN line, not as a row. Count on
   the row literal, not on the phrase.)

2. **THE MAXIMUM PROMPT IS NOT WHERE THE HARVEST SAID IT WAS, AND FOUR OTHER HARVEST NUMBERS ARE
   UNCALIBRATED PREDICATES.** The largest prompt is **30,228 chars at `125v162` seq 175** (turn 27,
   `ask`) — the brief recorded `125v130`; the engine seat caught the transposition and the disk
   confirms it. Also overturned, and none of them an arithmetic error: the harvest's **569 of 569**
   bare discard rows (a `{`-absent predicate on rows that all carry `{card text: …}` — the same shape
   as this step's own wave-56 failure, #298); the brief's **437** opponent-combat windows (a loose
   phase predicate; the emitter's own set is **256**); the brief's **80 of 82** unpriced converter rows
   (scope: the whole prompt, including boards where the converter is the seat's OWN — on the correct
   scope it is **0 of 2**). Every other harvest figure reproduced.

3. **D5's HEADLINE FAILS AND ITS MECHANISM PASSES, AND THE TWO MUST NOT BE MERGED — THE SECOND SLOT
   WORKS.** `dropping stale async answer` fires **108** times against a target of **< 20** (wave 56:
   65). Arms, from the line's own text: **casting 63, land-drop 45**. Cross-tabbed against the next
   `AIPlayerGPT:` ask in each stderr: land-arm drops are followed by a `Land drop:` ask **45 of 45**;
   casting-arm drops by a `Casting decision` 48, another seam 13, none-in-window 2. **CROSS-ARM
   DISPLACEMENT: 0 of 108.** Wave 56's 44 land-drop displacements of a casting answer are gone
   entirely. **The engine seat's read is exactly right and I reproduced every cell of it: 45 land-arm /
   63 casting-arm / 0 cross-arm / 11 rescues.** The residual is **intra-arm** drift, at **0.82 h** the
   corpus's largest inference sink after the identical-declined runs, and its casting-arm growth is
   concentrated: `game-126v130` carries **28 of its 29 drops inside one Sanguine Bond / Exquisite Blood
   trigger loop**. **`asyncSlotDriftKind` renders 0 times** in 21 stderr and 0 times in 3,317 prompts —
   the carry's expectation of that literal was simply wrong, the real field is the arm-labelled
   parenthetical, and the drop line names its ARM and never its OUTCOME. Ledger **D4**.

4. **THE WAVE'S THREE WORST FINDINGS ARE THE SAME DEFECT AT THREE MENUS: A NUMBER THAT IS FALSE IN THE
   DIRECTION THAT KILLS.**
   **(a)** `146v130` **s20** — `Your life: 1`, a two-row menu offering `pay 3 life - Emeria, Shattered
   Skyclave enters UNTAPPED`, taken, and **the next record in the file is `gameend`, `my_life: -2`,
   `won: false`** with the opponent doing nothing. `pay 3 life` renders **116 occurrences in 104
   windows / 9 takes**, and **10 of those windows put the row in front of a seat at ≤ 3 life**. The
   engine prints a lethality verdict on every other menu it has — `they would be at K` (196
   occurrences, **3 of them `THIS WINS THE GAME`, taken 3 of 3, won 3 of 3**), `this KILLS you` (27),
   `best case with every blocker assigned` (34) — **and none on the one row in the game that ends it.**
   Ledger **D1**, R293.
   **(b)** `123v130` **s45/s49/s50** — `NO LIVE CAST ROW ON THIS MENU: all 2 cast rows below carry a
   verdict computed from the board that reads zero` printed three lines above `Cast Devour Flesh {1}{b}
   {right now: they control 1 creature - Rorix Bladewing (6/5) [flying, haste …] is sacrificed}`. The
   pilot obeyed the header three times and the 6/5 flier killed it. **8 of 497 header windows carry a
   live-verdict cast row** on this step's predicate (deck123 ×7, deck126 ×1 — the Emrakul window,
   `126v125` s438), **1 cast taken from one**; the deck123/130 seat reads **6 of its own 92** on a
   wider predicate that also counts the Tragic Slip class. **Both are true of different units** and
   both are printed. Ledger **D2**, R243.
   **(c)** `126v152` **s14** — the seat at 5 life, printed `one legal assignment gets you to 1`, true
   floor **−5**. `assignableRemainderDamage` drops trample from its own baseline. **1 of 34** — and it
   is a BOUND, the one class of number where the direction of error decides whether the trust doctrine
   is safe. The model self-caught it and blocked correctly anyway, which is the only reason this is not
   a fourth lost game. Ledger **D7**, R303, and deck125's #G2 is adopted as the rule: **any printed
   bound must be conservative in the non-lethal direction, or it must not be printed.**

5. **AND A FOURTH, WHICH IS THE SAME DEFECT WITH A VERB WHITELIST UNDERNEATH IT.** `rowSaysNoOp` tests
   a fixed verb set (`does nothing`, `deals 0`, `destroys 0`, `kills 0`, `removes 0`, `drains 0`,
   `does not apply`, `gains 0`, `draws 0` — quoted as the deck125/126 seat's read of
   `AIPlayerGPT.cpp:19862`, not as a trace) and **`exiles 0` is not in it**. Measured: **9 rows in 8
   windows** read `{right now: exiles 0 of their creatures (0 without a restriction against attacking),
   0 of yours}` — a provably-zero six-mana sorcery — **and not one is badged `{dead right now:}`, and
   `NO LIVE CAST ROW` fires on none of them** (`hdr = False` on all 8), while the identical-shape
   `destroys 0` Supreme Verdict / Damnation rows fire it on **443**. Sites: `125v123` s187/s188/s189,
   `125v130` s14/s16/s98/s99/s100. The engine seat found the general form independently — **6 of 455
   discard rows carry a verdict the CAST evaluator computes and the DISCARD evaluator does not**
   (Lightning Greaves ×2, Silverquill Command, Barrowin). **Ledger D3.** Honest half: the pilot
   answered **7 of the 9 correctly anyway**.

6. **THE DISCARD CENSUS HAS FOUR LIVE PREDICATES AND NONE OF THEM IS WRONG (#298, third wave running).**
   Bare-row counts over the same 455 non-decline rows: this step, tag family enumerated — **292 (64.2%)**;
   the engine seat's "computable" predicate — **334 (73.4%)**; seat-125-126 at its own seats — **250 of
   374 (66.8%)**; the harvest's `{`-absent predicate — **569 of 569**. Wave 56 was 83.8%; at
   deck123/deck130 the class is now **closed at 36 of 36 annotated**. **The volume item is discharged;
   what remains is the 6-row evaluator gap and the whitelist, which is D3, not D8.**

7. **THE HOLD ECONOMY ROSE OVERALL AND REGRESSED WHERE IT COSTS MOST, AND THE CONTROL EXPERIMENT
   FINISHED AT 26×.** Pool **450 takes / 2,490 renders = 18.1%** (wave 56: 15.9%), reproduced to the
   row. Own-turn **247 of 1,521 = 16.2%**; opponent-turn **203 of 969 = 20.9%** (wave 56: 22.9%). But
   at the declined-note threshold the wave-56 rung was written for:

   | deck | renders | takes | rate | N ≥ 2 | rate | N ≥ 3 | rate | opp-turn rate |
   |---|---|---|---|---|---|---|---|---|
   | deck123 | 350 | 140 | **40.0%** | 14/51 | 27.5% | 4/38 | 10.5% | 82/112 **73.2%** |
   | deck125 | 749 | 119 | 15.9% | 42/275 | 15.3% | 32/220 | 14.5% | 73/369 19.8% |
   | deck126 | 499 | **3** | **0.6%** | 0/301 | 0.0% | 0/264 | **0.0%** | **0 of 336** |
   | deck130 | 469 | 149 | 31.8% | 35/88 | 39.8% | 20/64 | 31.2% | 40/80 50.0% |
   | deck146 | 101 | 15 | 14.9% | 0/8 | 0.0% | 0/2 | — | 2/4 |
   | deck152 | 226 | 20 | 8.8% | 7/73 | 9.6% | 6/52 | 11.5% | 6/61 9.8% |
   | deck162 | 96 | 4 | 4.2% | 0/5 | 0.0% | 0/4 | — | 0/7 |
   | **pool** | **2,490** | **450** | **18.1%** | **98/801** | 12.2% | **62/644** | **9.6%** | **203/969 20.9%** |

   **HOLD adoption at N ≥ 3 fell 12.0% → 9.6% while the class GREW from 501 to 644 windows** — the
   engine seat's HIGH #2, reproduced. That is the wave's honest negative, and it sits beside the
   positive without cancelling it.

8. **AND THE #233 CONTROL FINISHED, AT A RATIO THAT SETTLES THE QUESTION.** deck126's guide contains
   `Hold priority` **zero times**, the declined note zero times, and no instruction about the row of
   any kind — verified by a whitespace-normalised count on the deployed file. deck125, same reviewer,
   same corpus, same binary, same emitter string on 1,248 rows, carries a seven-line block naming it.
   **15.9% against 0.6% overall (26×), and 19.8% against 0.0% of 336 on the opponent-turn arm.** Wave
   55 read 6.7×, wave 56 6.9×. `hold_windows_skipped` sums **534 against 12**.

   **This step's ruling is skill #312, and it removes an obligation rather than adding one.** Three
   corpora at a large, stable, same-signed ratio is a settled result, and the general form is what goes
   into the skill: **a row the guide does not name is taken at roughly one-twentieth the rate of a row
   the guide instructs the pilot to take.** That is now a scoring calibration for every render proposal
   this loop will ever weigh — deck126's own #G9 states it in exactly those terms and it is adopted
   verbatim in substance. **#299's sunset is discharged as CLOSE, not as SHIP** (skill #329): the
   reviewer's counter-position — *"I am keeping deck126 text-free… the control's remaining job is to
   stay a control"* — is the reviewer's call under the ownership doctrine and is respected. What this
   step rules is only that **nobody owes the experiment anything any more.**

   **A note on the task's framing, because it matters for the record.** The 125/126 seat's item is
   headed as retiring #233 with a general finding; read in full, the seat is *keeping* its control and
   declaring the finding settled. The ruling above is written to be true of both readings: #233's
   QUESTION is closed and its general form is promoted; the deck-specific control is released rather
   than overridden. No guide was ordered to change.

9. **THE MARKER THE MODEL FOLLOWS MOST RELIABLY MARKED A 1-FOR-5, ONCE, AND THAT ONCE LOST A GAME.**
   `[<- best trade:` renders **exactly once in 3,317 prompts and was taken**: `130v152` **s33**, turn
   14 — `X = 4 {X pricing: kills THEIRS: Sigarda, Champion of Light; YOURS: Dwarven Blastminer,
   Siege-Gang Commander, Goblin x3}`. Rows 2-4 kill nothing on either side. deck130 lost **−20**. The
   marker is correct by its own definition; its definition is not the deck's goal. And the sibling
   **moved seam** — `[<- best X for this cast:` renders **408 windows / 11 takes** on the CAST row,
   where two guides located it "one screen later", which is skill #318's origin and #311's proof.
   Coverage itself is a clean PASS: 526 X rows, 506 marked, the 20 unmarked all `X = 0`. Ledger **D5**.

10. **ONE PHASE OF ONE GAME IS 2.2% OF THE WAVE'S DECISIONS.** `130v126` **turn 16, seq 62-134: 73
    decisions in one Main phase** (38 `priority` + 35 `ask`), **24.0 minutes of inference**, **45 of 73
    pure declines**, the declined note reaching **N = 43** — the corpus maximum against a wave-56
    maximum of 19 — on an unstoppable Sanguine Bond / Exquisite Blood trigger loop, and it is the same
    game that carries 28 of its 29 async drops. Corpus-wide the identical-declined economy went the
    wrong way: **636 of 3,317 = 19.2%, 96 runs, max run 13, 2.26 h = 8.9% of inference** (wave 56:
    14.99% / 68 / 13 / 5.5%). The engine seat reads 17.6% / 88 / 8.0% on its own predicate and the
    harvest 17.1% / max 12 — **three numbers, one sign, and all three fail the target.** Ledger **D8**.

11. **AND THE SAME CLAIM READS 0, 8, 29 AND 14 DEPENDING ON THE PREDICATE — ALL FOUR DEFENSIBLE, WHICH
    IS WHY SKILL #327 EXISTS.** Byte-identical same-turn re-asks after a HOLD take: **0 of 450** on
    strict adjacency (the NEXT record), **8 of 450** same-turn-and-same-phase, **29 of 450** on any
    later same-turn record with an identical `options_text`, and **14** in the carry, which this step
    could not reproduce under four predicates and records irreproducible rather than wrong. Three
    readers filed **PASS**, **FAIL** and **"the cleanest result in loop history"** about one behaviour.
    #300 put the denominator inside the verdict; that was not enough. **#327 puts the PREDICATE inside
    the prediction, as executable words**, and the wave-58 prediction is written that way (ledger D8).
    This is #295's second occurrence in two waves and the third instrument amendment this step has been
    forced to write.

12. **THE LOG-WINDOW A/B CANNOT BE READ YET, AND THAT BLOCKS AN OWNER DECISION.** The owner ruled
    **"measure."** Lane H built the mechanism default-OFF, proved the `full` arm byte-identical on 352
    assembled prompts, and this corpus verifies the control: `log_window` = `full` on **3,317 of
    3,317**. Two defects stop the harvest, both found by the engine seat and reproduced here: **the
    field carrying the elided-turn count does not exist in the record set at all** (46 distinct keys
    enumerated across 3,408 records), so the control-arm prediction "`elided` is 0 on 100%" is
    **UNTESTED — absent, not zero**; and **`log_window_kind` labels only 28.3% of decisions**
    (`land_drop` 377, `empty_stack_pass` 368, `combat` 111, `cleanup_discard` 66, `target_or_reveal`
    13, `pregame` 4; **2,378 = 71.7% `unclassified`**) while the `kind` arm's budget is defined ON those
    labels — arm B would window ~28% of the corpus and silently leave the rest at `full`. **Lane H's own
    residual predicted exactly this** and named the control-arm census as the harvest's first job; it is
    now run, and the classifier is wrong before the A/B starts. Ledger **D6**, and the §D design is
    otherwise correct as written (three arms, `-j 3` and never `-j 6`, 21 games/arm, metrics ranked
    composition → inference → fallback → misplays → win-rate BAND only at SE ≈ 10.9, rare-event rule
    in force). Related: **`WAGIC_SELFPLAY_SEED` does not exist**, so no arm can be paired game-for-game
    (ledger D21).

13. **THE NARRATION SHARE MOVED UP, AND IT IS THREE NUMBERS ON THREE BOUNDARIES (#208 again).** GAME
    LOG above `--- CURRENT SITUATION ---`: **64.0%** of the mean prompt (wave 56: 54.0%); everything
    above the first option row: **87.8%** (was 82.7%). Per seat: deck123/130 **56.9%**, deck125/126
    **67.6%**, deck162 **48.3%**. Total prompt text **40.46 MB**; `discard` is the most expensive class
    per record (15,872 mean) and `bottom` the cheapest (2,354, already ~0% narration — the existence
    proof that a per-kind budget is renderable). Meanwhile the mean prompt **ROSE 9.1%** while
    per-decision inference **FELL 1.5%** at the same `-j 21` — **R257's third confirmation that a byte
    trim is a context-headroom item and never a latency item.**

14. **LANE V IS CARRIED, NOT ADJUDICATED, AND THE OWNER'S FRAMING IS WHY IT IS A BAKA CHANGE.** His
    words: **"on the vita, ive been playing only against the heuristic ai."** Every Vita play report is
    a Baka-seat game, so it routes to `AIPlayerBaka` / the GUI / the card script and **never to a guide
    or to the GPT prompt surface** — lane V is the first item to arrive through that channel and is
    correctly a `chooseBlockers` policy change. Its A/B (10 fixed Baka-vs-Baka matchups, control
    binary vs lane binary) reads: blocks declared 83 → 82; value-pass blocks **80 → 57** with the
    survival sweep taking up the slack **3 → 25** (expected, not a regression); **value-pass blocks
    losing a creature with no attacker death and not facing lethal: 26 of 80 (32.5%) → 0 of 57**;
    **value-pass declarations spending more than the attacker is worth: 20 of 80 (25%) → 0 of 57**;
    **value-pass gangs 15 two-body and 6 three-body → 0 and 0.** Per `feedback-rare-event-ab-power`
    **10 games is a direction, not a p-value** — the two headline rows are shape claims over real
    declarations, not rate estimates, and the lane says so itself. Carried as ledger **D22** with its
    four predictions, plus its two named residuals: **D23**, `toggleDefenser`-committed blocks are
    never click-logged, so the survival sweep and the CR 509.1c repair sweep are **invisible to a
    transcript and unreplayable** — which silently biases every blocking diagnosis toward the click
    path, in the exact channel the owner's reports arrive through; and **D24**, the deck84 five-on-one
    pile has no fixture, because `AIStats::stats` cannot be populated inside a single-combat `[AI]`
    fixture.

---

## The headline: the loop's instrument finally reads true, and what it read is that three menus print a number that is false in the direction that kills

**(a) THE INSTRUMENT IS CLEAN.** 21/21 natural for a fourth wave, **0 SEGV**, fallbacks **0.24%**,
`abandoned_in_flight` **0**, Baka executed **0**, `recovery` 7/7, and — the one that matters most —
**`chosen_text` is the rendered row byte-for-byte on 2,960 of 2,960 single-row takes and the exact HOLD
census reads 450 of 450.** Two consecutive waves of corrupted docket numbers end here.

**(b) AND WHAT IT READ IS FOUR VARIANTS OF ONE DEFECT.** The pay-3-life row with no lethality verdict
(a seat dead at −2 from 1 life, unopposed). A menu header saying every row reads zero above a row
sacrificing a 6/5 flier (three declines, one dead deck). A verb whitelist that badges `destroys 0` dead
on 443 windows and `exiles 0` live on 9. A blocking bound that printed `gets you to 1` on a board whose
floor was −5. **Each is a TRUE-looking number in the direction that loses**, against a core prompt that
instructs the model to believe what it sees. The trust doctrine's obligation runs one way, and this
wave the surface owed four debts.

**(c) THE WAVE'S BIGGEST BEHAVIOURAL RESULT CLOSES AN EXPERIMENT INSTEAD OF ADDING A RULE.** Three
corpora, a 26× ratio, and one sentence that now prices every render proposal this loop will ever
weigh: **an untaught row is taken at about one-twentieth the rate of a taught one.** #233's question is
answered; #299's sunset is discharged as CLOSE; the control is released rather than overridden, because
the guide is the reviewer's.

**(d) TWO LANES LANDED CLEANLY AND ONE OF THEM LOOKS LIKE A FAILURE UNTIL YOU CROSS-TAB IT.** Lane W
closed the MDFC legality break, the pregame count and the contradicting summary line in one pass
(`PLAY THIS AS A LAND` 108 rows / 15 takes / **0 illegal**, was 44 of 65). Lane A's second async slot
took cross-arm displacement from 44 to **0 of 108** — and its headline drop count went UP, to 108. Both
statements are true, and a ledger that reports only the headline would have retired a working
mechanism.

**(e) AND THE WAVE'S METHOD FAILURE IS THE SAME ONE, A THIRD TIME.** One claim, four predicates, four
verdicts (0 / 8 / 29 / 14). A max-prompt filename transposed. A discard census with four honest
numbers. A literal (`asyncSlotDriftKind`) that a whole carry-list was written around and that **does
not exist**. Every one of them is a predicate that was guessed rather than quoted (#295) or a
denominator that was named without its unit (#300) — so **#327 puts the predicate inside the
prediction** and **#328 puts the baseline file inside the size obligation**, and both exist because
this step could not reproduce a number that nobody had gotten wrong.

---

## What merged into the skill (301-329)

**26 seat proposals, 26 ACCEPTED, 0 REJECTED as skill text** — the second clean sweep in a row, and it
is again a datum rather than a courtesy: every proposal this wave arrived with a window count on its
own literal (#292) and most arrived with the predicate too. **One (deck126 #S9, the two-`CHOICE:`-lines
reply) was accepted as a FINDING and ROUTED to the engine ledger** rather than written as doctrine,
because the reply protocol is code-appended after the user-editable template and no guide may edit it;
its guide-side half survives as **#314**.

**Convergence merges (three):** **306** — *a rule must be true at the window it FIRES in, not the
window it was learned in* (deck125 #S1 + deck130 S57-6; the `YOUR X IS 3` rung obeyed 3 of 3 at a seam
where its premise is false by construction, costing 4 life and 4 cards, and three deck130 rungs applied
at menus with no declining row) · **321** — *when a key measures something the pilot cannot act on,
RE-KEY it, do not sharpen it* (deck146 S2 + deck152 S5 + S6; 66 of 73 "breaks", **0 of 66 with an
unchanged situation block**) · **322** — *a prose brake does not bind at this tier; a brake is an
operation with a literal, a release and a verdict, run LAST* (deck146 S3 + deck162 S3, **cross-filed by
both reviewers themselves**; 5 breaks, every one restating the rule and then naming the wrong row in
the same reply).

**Decisive single-seat merges:** **301** a rule names the unit it reads (deck123 S57-1) · **302**
worked examples are the trim currency (deck123 S57-2; the one untrimmed paragraph is the one that
broke) · **303** a PASS on a dead denominator is UNTESTED, and the word is forced (deck123 S57-3) ·
**304** the ORDERING between two rules is itself a rung (deck123 S57-4; 26 power tapped for seven
turns) · **305** a rung that survives a render fix is re-dated (deck123 S57-5) · **307** one rule, one
place (deck125 #S2; the same floor three times, 70 lines apart, dead at −1) · **308** a ceiling never
tested against survival is not a ceiling (deck125 #S3) · **309** a three-corpus prose failure moves
into the PLAN line (deck125 #S4) · **310** write `perception status: CLOSED` when a render ships
(deck125 #S5) · **311** a ceiling describing engine output is re-verified as a LITERAL every wave
(deck125 #S6) · **313** an entry-ordered list ships a read instruction (deck126 #S8; 4 of 11) · **315**
name the substitute when a printed price turns the top rule off (deck126 #S10) · **316** count a
carve-out against the board that broke it (deck126 #S11; taken at 6 sources under a ≤ 4 clause) ·
**317** naming the rationalization does not suppress it — only a test on a printed string does
(deck130 S57-7; **the model wrote the forbidden sentence verbatim, twice, while committing the act**) ·
**318** a marker that moves seam makes every guide sentence about it stale (deck130 S57-8 +
seat-123-130 S57-8) · **319** a "best <thing>" marker gets its own guide arithmetic (deck130 S57-9) ·
**320** a rule's own removal condition is a commitment, checked FIRST (deck146 S1; **two came due this
wave and both were honoured**) · **323** `WHAT THIS COST` numbers age faster than the rule (deck146 S4)
· **324** a guide must not state a mechanism the render refutes even when the conclusion is right
(deck152 S5) · **325** a deck with no open lane is a rotation candidate, and the reviewer may say so
(deck152 S7) · **326** an exception is where a brake leaks — count the exclusion (deck162 S8; 3 of 5).

**And the wave's ruling:** **312** — the HOLD-row control is SETTLED at 26×; #233 is promoted from
experiment to finding; the general form (*untaught ≈ 1/20th of taught*) becomes a scoring calibration;
the control is RELEASED.

**This step's own, forced by the reconciliations:** **327** a prediction that counts a repeat, run,
drift or adjacency states its PREDICATE inside the prediction sentence (reconciliation 11; 0 / 8 / 29 /
14) · **328** a byte-size obligation names its BASELINE FILE (the boundary section below; five of seven
reviewers' net changes differ from this step's and every number is right) · **329** an experiment at a
stable large ratio over three corpora is CLOSED and its control released (#299 generalised, discharged
by #312).

## What was REJECTED, and why

- **"Skip the opponent-turn window whose `ON THE STACK` is empty at a phase this seat has never cast
  in"** — seat-125-126 MED #4, **SIXTH wave running**, on its biggest denominator yet (deck125 **237 of
  376**, deck126 **279 of 355**). **REJECTED again.** It removes a legal window from the model. The
  sanctioned levers are R305 (print what changed instead of hiding the window), R306 (measure the
  declined note), and the HOLD row, which is at **20.9% on the opponent-turn arm** and which the model
  takes *itself*. #312 now prices the gap precisely: the distance from 20.9% to 100% is a GUIDE problem
  worth about 20×, and it is not a licence.
- **"Do not ask when the menu has exactly one legal row"** (deck130 G57-5b) and **"extend
  `mana_only_windows_skipped` to more forced-ask classes"** (deck125 #G5). **NOT ADOPTED as engine
  behaviour; the HEADER halves ARE adopted** (R311). Both reviewers disclaim a cap explicitly and
  correctly — deck130's own words are *"UX, not legality: nothing is withheld and no choice is
  constrained"*, deck125's are *"I am not asking for a cap on legal choices — the standing ruling
  rejects those on sight"* — and both are right that the round trip is real. But "this menu has one
  row" is a decision the engine would be making for the model.
- **A guide compensation for D1, D2, D3 or D7.** **REJECTED as doctrine, ACCEPTED as debt.** deck146's
  `146-A` (default to `tap`, hard floor at 3 life) ships and is correct: both its inputs are TRUE
  surfaces (`Your life:` and the row's own text), which is what #287 permits, and D1's render deletes
  it. **No guide sentence may hedge against a printed bound** (#324) — and D7 is precisely a printed
  bound that lied.
- **Reordering the decline rows / hoisting HOLD above `Cast nothing right now`.** Not re-filed by any
  seat this wave; recorded so it is not read as newly open. Standing owner ruling.
- **The harvest's `569 of 569` bare discard rows, its `437` opponent-combat windows, its `80 of 82`
  unpriced converter rows, and its `125v130` max-prompt attribution.** All four **OVERTURNED on disk**;
  none is an arithmetic error, each is an uncalibrated predicate or a transposition.
- **The wave-56 carry's `asyncSlotDriftKind` expectation.** **OVERTURNED — the literal renders 0 times
  anywhere.** A whole prediction was written around a field name that does not exist, and the seats
  caught it. #295's second occurrence.
- **Any win-rate attribution.** deck152 4/6; deck123, deck126, deck162, deck125 and deck130 all
  **3/6**; deck146 2/6 — **the flattest band on record**, n = 6 per deck, real hands, no seed pinning.
  deck123's 1/6 → 3/6 is the one number a reader will want to attribute, and its own seat refuses to:
  it attributes the move to token-makers resolving and sticking in 6 of 6 games (board-presence windows
  with ≥ 1 creature **15.9% → 41.0%**, in losses **1.0% → 25.8%**) and **not** to a −189-byte guide
  edit. Counts only (#87, #167, #217, #292). All seven seats correctly recorded **NO EXIT**.

---

## Layer routing (`general-strategy.md`)

**RENDER, HIGH**: the pay-3-life lethality verdict (R293) · the discard/cast evaluator gap and the verb
whitelist (R294) · `INCOMING THIS COMBAT`'s phase gate (R295).
**MED**: the X=0 fit clause (R296) · the attackers-seam lifelink nesting (R297) · the `best trade`
marker's definition and its seam move (R298) · the log-window instrument (R299) · the converter loop
price (R300) · the pilot's own life trend (R301) · the fetch row's negative colour form (R302) · the
assignable-remainder bound (R303) · `{feeds:}`'s verdict shape (R304) · the declined note's second
number (R305) · the declined note as a measurement candidate (R306).
**LOW**: the lowercased narration tokens (R307) · the header's verb (R308) · the multi-select instance
handle (R309) · the pregame Pathway mana line (R310) · the one-row menu header (R311) · `repeat_n`
(R312).
**CORE-PROMPT**: two `CHOICE:` lines → `choice_revised` (R313) · the `[RE-ASK]` notice's reason (R314).
**PROCESS/MEASUREMENT**: predicate-inside-the-prediction (R315) · baseline-file-inside-the-size-obligation
(R316) · experiment closure (R317) · lane residuals as docket inputs (R318).
**PASS/KEEP** (recorded so no lane trims them): the corrected `they would be at K` and its
`THIS WINS THE GAME` flag · `Their untapped sources:` and `Opponent life trend:` (both 3,267/3,267) ·
`INCOMING THIS COMBAT` on its own window set · `best case with every blocker assigned` · the X marker
family, its header and its fit clause · **the `PLAY THIS AS A LAND` row and its legality gate** · the
pregame `counted TWICE` note · the HOLD row and its byte-exact tail · the `(xN)` log collapse ·
`Flip Side` suppression and its 1,309 auto-passes · `[NAMED BY THEIR …]` · `<refused: …>` ·
`reveal_wait_*` · `mana_only_windows_skipped` · the per-stderr `windows held` companion ·
`decision_reversed_in_prose` beside its narrowed stamp · `[second copy:` · the fetch row's positive
colour form · the `{dead right now:}` family and its legend.

---

## Guide boundary findings for the deck reviewers (B1-B9)

Step 4 run as a **REPORT** over the seven DEPLOYED `deckN_strategy.txt`, **verified byte-identical to
`wave57/deckN/strategy.txt`, all seven, by `md5sum`** — so every finding is an obligation for wave 58,
not a request to change a pending edit. Sweeps run: the trust-doctrine hedge sweep; the #233
forbidden-phrase sweep; the **whitespace-normalised live-literal audit** (#294) over 31 emitter
strings; the removal-condition sweep (#285(c) / #320); the `WHAT THIS COST` staleness sweep (#291 /
#323); and the size/net-change sweep on **both baselines** (#328). **Every literal count below is a
COUNT PER FILE with whitespace-normalised wrapped-literal matching**, which is the wave-56 B1 lesson
and skill #294.

**TRUST DOCTRINE: CLEAN, all seven, FIFTH wave.** The hedge sweep (`may be wrong`, `do not trust`,
`the number may`, `is sometimes wrong`, `engine sometimes`, `the render lies`, `cannot be trusted`,
`may be false`, `is not reliable`, `might be wrong`, `can be wrong`) returns **zero hits across all
seven pool guides**. No guide teaches doubt of a rendered value — including deck146's, in the wave
its own reviewer proved a rendered menu can kill the seat.

**WAVE-56 OBLIGATIONS: five of six honoured, and the sixth is B5.** B1 (deck130's three-wave dead HOLD
quotation) **HONOURED IN FULL** — the wave-54 dead string now renders **0 times in the file** and the
live literal renders **1**, with its `{taking this row skips …}` tail (deck130 converted at **31.8%
overall / 31.2% at N ≥ 3**, the pool's second- and first-best). B2 (deck126's control) **honoured** —
`Hold priority` still **0**, and #312 rules on it. B3 (deck123's `THE ROW IS NOW KEPT.` absolute)
**honoured**. B4 (the undated wave-55 receipt in two files) **honoured in deck146** (`WHAT IT IS NOW,
wave 56: this deck took it 5 of 20` at :637) and **NOT in deck152** — B5. B6 (deck146's absence block)
**honoured**. B7 (deck125's live quotation) **honoured on the literal, VIOLATED on the net change** —
B4 below. B8/B9/B10 → B4, B7, B8.

| # | file : site | amendment | finding | replacement string |
|---|---|---|---|---|
| **B1** | `deck146_strategy.txt` **lines 141-154**, and by inheritance every guide | **#285(c) / #287 / #320** | **THE RETIREMENT MACHINERY IS NOW ABSENT FROM ALL SEVEN GUIDES, IN THE WAVE IT PROVED ITSELF.** A normalised count of `REMOVE TH…` returns **0 in all seven files**. That is half a success: deck146's MDFC-count arithmetic and deck152's `NEVER TAKE "Flip Side"` paragraph both carried removal conditions, **both came due this corpus, and both reviewers honoured them and retired the paragraphs** — so those two lines correctly went with the text they governed (skill #320's origin, and the pool's best result this wave). **The defect is that the replacement carries none.** deck146's new `146-A` block (`pay 3 life` quoted 3×, :141-154) is a textbook #287 route-around: a guide floor written because the ROW does not price the payment. **Ledger D1 deletes it**, and nothing in the file says so. | Append inside the 146-A block, per **#285(c)**: `REMOVE THIS FLOOR WHEN the "pay 3 life" row prints its own life verdict - re-verify with a count of "pay 3 life" rows carrying "{this payment puts you at" in the corpus; it was 0 of 104 in wave 57, and the docket asks for the verdict (D1).` Same obligation on any wave-58 rung written against D2, D3 or D7. |
| **B2** | `deck152_strategy.txt` **lines 449-453** | **#289 / #320 / #311** | **A PARAGRAPH ABOUT A ROW THAT NO LONGER RENDERS, SECOND WAVE — AND THE REVIEWER RETIRED THE OTHER HALF OF IT.** `Flip Side` renders **0 times in 3,317 prompts** (wave 56: 109 rows / 90 windows / 0 takes) — lane D's D14 is a clean PASS and the display-toggle is gone from the AI seat entirely (1,309 auto-passes, `(suppressed 1 land-face toggle row(s))`). `152-A` correctly retired the `NEVER TAKE "Flip Side" ON A LAND MENU` imperative. **What survives at :449-453 is the surrounding explanation**, which still teaches the pilot to recognise and ignore a row it will never see, and which spends five lines doing it. It is not wrong — it is dead weight, and it is exactly the shape #289 (the zero-take audit of every literal the guide names) exists to catch. | Delete lines 449-452 (the `"FLIP SIDE"` heading and the two Brutal Cathar / Tovolar bullets). **Keep :453 onward** — the Pathway two-face rung is live and correct (`PLAY THIS AS A LAND` renders 1× in this file's own quotations and the row renders 108× corpus-wide with **0 illegal offers**). Net saving ~400 bytes, and it is the trim that pays for this deck's wave-58 additions (#302). |
| **B3** | `deck152_strategy.txt` — **16 `WHAT THIS COST` sites**, `deck146_strategy.txt` — **12**, `deck162_strategy.txt` — **9** | **#291 / #323** | **THE SCOREBOARD COUNT IS THE FINDING: SIXTEEN RECEIPTS IN ONE 64 KB FILE.** deck152 carries `WHAT THIS COST` at :17, 28, 83, 206, 215, 332, 337, 392, 421, 484, 521, 526, 596, 623, 635, 654 — and two of them are explicitly multi-wave (`a fourth corpus running` :521, `in two corpora running` :526). deck146 carries 12, deck162 nine. **#323 was written from this file** (deck146 S4: *"WHAT THIS COST numbers age faster than the rule they defend"*), and the file that produced the amendment is the file with the most instances of the defect. The receipts are individually good — dated, seq-cited, falsifiable, which is what #291 asked for — but sixteen of them in one guide is a corpus, not an argument, and the pilot cannot tell which one describes the game in front of it. | **One receipt per rule, newest corpus only, wave-named.** For each of the 16, either re-measure against wave 57 and date it, or delete it. Two are already retirable under #291's two-passing-corpora clause. deck146's :635-638 is the model to copy — dated `wave 55` then `WHAT IT IS NOW, wave 56` — **and it is itself now stale**: this deck took the HOLD row **0 of 8 at N ≥ 2 and 15 of 101 overall** in wave 57, so the `5 of 20` line needs its third date. |
| **B4** | `deck125_strategy.txt` — **SIZE**, and all seven — **BASELINE** | **#232 / #328** | **deck125's NET-CHANGE OBLIGATION IS VIOLATED FOR A SECOND WAVE — AND FIVE OF SEVEN NET CHANGES DISAGREE BETWEEN THE REVIEWERS AND THIS STEP, WITH EVERY NUMBER RIGHT.** Wave-56 B10 set deck125 a net change **≤ 0**. Measured: **+978 bytes** against the deployed baseline (its own reviewer's figure), **+1,005** against `wave56/deck125/strategy.txt`. The disagreement is systematic and is #328's origin: reviewers measured against the DEPLOYED guide (post-boundary-pass a93aad889), this step against the wave-56 reviewer file. Both columns, in bytes — **deck123 +34 / +108** · **deck125 +978 / +1,005** · **deck126 +4,650 / +4,650** · **deck130 +1,640 / +1,757** · **deck146 +2,023 / +2,298** · **deck152 +20 / +387** · **deck162 +1,314 / +1,314**. Pool **+10,686 (+2.35%)** on the deployed baseline, down from +3.2% and +5.5%. deck125's own growth rate has fallen **9.3% → 9.3% → 4.3% → 1.4%**, which is the right trend on the right side, so the obligation is **restated, not escalated** (#232). | Wave-58 brief: **every net change is stated as `N bytes against <named file>`, and the DEPLOYED guide is the baseline** (#328) — it is what the corpus served. deck125's wave-58 file states a net change **≤ 0**, third wave of the obligation, and states the trims that paid for it (#302) with each trim's own prediction (#270). Synthesis publishes both columns each wave. |
| **B5** | `deck152_strategy.txt` **line 635** (and the paragraph at :623-660) | **#291 / #323 / #321** | **THE ONE WAVE-56 OBLIGATION NOT HONOURED, AND IT IS NOW ATTACHED TO A RUNG THE REVIEWER HAS ITSELF DECLARED INVALID.** B4 asked both deck146 and deck152 to date the wave-55 `190 windows … not one of them took it` receipt and add a `WHAT IT IS NOW` line. deck146 did (:635-638). **deck152 did not — `WHAT IT IS NOW` returns 0 hits in the file.** And the rung it defends is the one this wave's own reviewer re-keyed rather than sharpened, on the finding that **0 of 66** of its "breaks" had an unchanged situation block (skill #321, ledger D10). So the file now carries an undated wave-55 scoreboard in front of a rule whose key its own author has retired. | Date it and re-measure, or delete it with the rung it defends: `WHAT THIS COST, wave 55: 190 windows across three of this pool's decks carried that declined line with the HOLD row on the menu and not one of them took it. WHAT IT IS NOW, wave 57: this deck took it 7 of 73 at N >= 2 (9.6%) and 20 of 226 overall - and the N line counts option LISTS, not board states, so this is not the number the rule should be keyed to (see the re-key).` |
| **B6** | `deck162_strategy.txt` **line 23** | **#233 / #294 / #298** | **A FORBIDDEN-PHRASE SWEEP HIT, AND IT IS A FALSE POSITIVE — REPORTED SO THE NEXT SWEEP DOES NOT "FIX" IT.** The #233 sweep (`both are correct` / `either is correct` / `equally correct` / `both are fine` / `is also correct`) returns **one hit in the pool**, deck162 at :23, and **a single-line `/usr/bin/grep -i` cannot see it** — the phrase wraps between `both are` and `correct` (the #294 shape again, this time producing a false ALARM rather than a false clean). Read in context it is not the #233 sense at all: *"'Mana available: N total (… colours you can make: {u}{b})' gives you two facts and both are correct: N is what you can spend this window, and the colour list names which COLOURED pips you can pay."* That is a correct statement about a rendered line, not a licence to answer two ways on one menu. **The pool remains CLEAN on #233, second wave.** | **None — do not edit.** The obligation is on the SWEEP, not the guide: a mechanical string sweep must have its sense checked before a hit is reported (#294's converse). Recorded here so wave 58 does not delete a true sentence to satisfy a grep. |
| **B7** | `deck130_strategy.txt` **lines 169-176, 211-213, 772** | **#311 / #318 / #319** | **PASS, AND IT IS THE WAVE'S BEST BOUNDARY RESULT: THE ONLY GUIDE THAT CAUGHT A MARKER MOVING SEAM.** deck130 quotes `best trade` **6 times** and `best X for this cast` **once** — it is the only file in the pool that names the new seam, and it did so in the same wave the marker moved (408 windows on the cast row). It also carries `THIS WINS THE GAME` (1), `Opponent life trend:` (1) and `since turn` (2) — the only guide quoting any of the three new lane-E/B literals. And B1's wave-54 dead HOLD quotation is **gone**, with the live literal and its tail both at 1. **The one thing to fix is small and it is #319**: the file defers to the marker without supplying the arithmetic, and this corpus is where that cost a game (`130v152` s33, `[<- best trade:` on a 1-for-5, taken, lost −20). | Add one sentence beside the marker rung: `The marker's own definition is "best trade", not "good trade". Count the names after "kills THEIRS:" and after "YOURS:" on the marked row - if the YOURS list is longer, the marker is NOT your answer and the next affordable X is.` Keep everything else; the seam-tracking is the model for the pool (#311). |
| **B8** | all seven `deckN_strategy.txt` — **SIZE and BAND** | **#232 / #220 / #270** | **THE POOL GREW +10,686 BYTES (+2.35%) — DOWN FROM +3.2% AND +5.5%, ON THE BIGGEST BASE YET, AND THE BAND TIGHTENED FROM BELOW.** Measured on the deployed files: deck162 **46.8**, deck152 64.1, deck130 68.0, deck146 68.0, deck126 68.5, deck125 69.1, deck123 **69.3 KB** — **band 46.8-69.3 KB, median 68.0 KB** (wave 56: 45.5-69.2, median 65.8). **Six of seven guides now sit inside a 5.2 KB span**, which is new: the pool is converging on a ceiling rather than spreading. deck126 is the fastest grower for a second wave (**+4,650, +7.1%**) and is still the deck with **zero HOLD text** — worth one sentence beside #312's ratio, because its growth is buying something other than the cheapest available rate improvement. deck152 is the smallest net change (**+20**) in the wave its reviewer filed a `NO OPEN LANE` verdict (#325), which is the honest pairing. | Wave-58 brief: **every seat states its net change against the DEPLOYED file** (#328), **with the trims that paid for it** (#302), **and every trim states its own falsifiable prediction** (#270). deck125's is **≤ 0**, third wave. deck126 states whether its 7.1% bought a measurable rung. Synthesis publishes the band and both net-change columns. |
| **B9** | all seven — **THE UNQUOTED LIVE LITERALS** | **#188 / #311 / #312** | **THE THREE HIGHEST-VALUE LINES THE RENDER PRODUCES ARE QUOTED BY NO GUIDE IN THE POOL, AND #312 IS EXACTLY WHY THAT MATTERS.** Normalised counts across all seven files: `they would be at` **0**, `Their untapped sources:` **0**, `NO LIVE CAST ROW` **0**. The first renders **196 occurrences in 168 windows** and produced this engine's first three lethal face rows — **`THIS WINS THE GAME`, taken 3 of 3, won 3 of 3** — and only deck130 names even that flag. The second renders on **3,267 of 3,267 board frames** and is the line that inverted deck152's 65-turn loss into a T15 win two waves ago. The third renders **497 times** and is currently FALSE on 8 of them (ledger D2), which is the one honest reason not to teach it yet. **#312's finding prices this: an untaught row is taken at ~1/20th the rate of a taught one.** Three lines, thousands of renders, zero guide sentences. | Wave-58 brief, one line per reviewer: **name the two clean ones.** `they would be at K` and its `THIS WINS THE GAME` flag belong in every guide that can attack or activate; `Their untapped sources:` belongs in every guide with an instant or a combat trick. **Do NOT teach `NO LIVE CAST ROW` until D2 ships** (#324: a guide may not state a mechanism the render refutes) — and note in the wave-58 file that it is deliberately untaught, so the omission is not read as an oversight. |

**Not findings — checked so nobody re-files them.** deck130's live HOLD literal and its tail (B1 of
wave 56, fully honoured) · deck146's `WHAT IT IS NOW, wave 56` receipt shape (:635-638, the model for
B3/B5, and separately stale) · deck146's `pay 3 life` block (3 quotations, correct, and it is the
sanctioned #287 route-around — B1 asks only for its removal trigger) · deck123's `this equips nothing`
rung (4 quotations, 45 rows / 0 takes, seventh corpus and the FIRST post-fix one — #305 re-dates it,
it is not a defect) · deck162's `{feeds:` block (8 quotations; the render is TRUE and the 4-of-20
breaks are a guide item, skill #322/#326, not a boundary defect) · every guide's `would not cover any
spell in it` quotation (1-4 per file; the emitter renders once and was acted on once, correctly) ·
deck146 and deck152's `even with every land in this hand in play` scope tail (1 and 2, honoured from
wave-55 B3) · the `PLAY THIS AS A LAND` quotations in deck146 (2) and deck152 (1), now describing a
row with a working legality gate.

**Card facts spot-checked.** No primitive/Oracle divergence was found by this step. Four reviewer
claims carry `mtg.txt:line` citations and are flagged for a wave-58 spot-check rather than accepted:
Intruder Alarm `:58850` and Thraben Doomsayer `:120583` (deck123 A57-2a / A57-4), Sanguine Bond
`:99004` and Exquisite Blood `:37966` (deck130 D57-4). The deck146/152/162 seat states plainly that it
did **not** verify Bloodline Keeper or Intruder Alarm against a primitive and that its 31-token chain
reading came from narration alone — **that is exactly the shape of the four hallucinated-Oracle
incidents this loop has already had**, and it is recorded as an open verification, not as a fact.
Lane D's Pelakka Caverns Oracle correction (verified against Scryfall, 2026-09-03) is accepted as
verified. **No seat fetched Scryfall this wave.**

---

## What this step did NOT do

Did not build, run, deploy, commit, or run any git command; did not run wagic; did not edit any file
under `bin/Res/**` or `src/**`; did not edit any `deckN/strategy.txt`. Did not read `src/` — the two
source attributions in this file (`rowSaysNoOp` at `AIPlayerGPT.cpp:19862`,
`assignableRemainderDamage` at `:14991`) are quoted as the deck125/126 seat's READS and are not traced.
Did not run the log-window A/B (it is a wave-58 step and D6 blocks it). Did not verify lane V's A/B
independently — its numbers are the lane's own, on a binary not in this corpus, and they are labelled
so. Did not adjudicate lane G (human seat, no corpus surface), lane T's softlock dump (no softlock
occurred), lane S's `WAGIC_STALLPROBE` (dev-gated, 0 lines), or lane F's disable flag
`WAGIC_NO_DEADREF_SWEEP=1` (not exercised) — all four are **UNTESTED by construction**, with the cause
named (#283). Did not reproduce the harvest's `14` same-turn re-asks under any of four predicates.
Did not open all 3,317 prompts individually: every count is scripted over the full record set, and
every narrative claim carries a file and a seq that was opened.
