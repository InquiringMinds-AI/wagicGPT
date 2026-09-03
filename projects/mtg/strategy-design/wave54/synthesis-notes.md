# Wave-54 synthesis (2026-09-03) — ELEVENTH FAIR-HAND CORPUS, and the first 21/21 since wave 52

Inputs read, in order: `~/.claude/skills/wagicgpt/SKILL.md`; `wave53/synthesis-notes.md` and
`wave53/engine-ledger.md` (the wave-54 docket D1-D38 this corpus adjudicates); `wave54/review-carry.md`
and `wave54/reviewer-brief.md`; the four seat files (`seat-engine-narration.md`, `seat-123-130.md`,
`seat-125-126.md`, `seat-146-152-162.md`); all seven `deckN/edit-texts.md`, `deckN/skill.md` and
`deckN/general-suggestions.md`; `wave54/lane-A.md` and `lane-F.md` in full and the other lane reports
as needed; and the corpus itself at
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-034355/` — **42 seat JSONL and 21 `game-*.stderr`,
every load-bearing number re-derived on disk** with python over the JSONL and `/usr/bin/grep` over
the stderr. Card facts were re-verified against `projects/mtg/bin/Res/sets/primitives/*.txt`; the
seven deployed guides were read for the boundary pass. Nothing was built, run, deployed or committed;
no file under `projects/mtg/bin/Res/**` or `src/**` was modified; no git command was run.

Outputs: `wave54/strategy-writing-skill.md` (the wave-53 edition **byte-verbatim** — the
**1,153,060-byte prefix is `cmp`-verified** — plus **amendments 209-232**),
`wave54/general-strategy.md` (R1-R192 with a wave-54 verdict each, **R193-R224** new and
layer-routed, with the rejections reasoned), `wave54/engine-ledger.md` (**the wave-55 docket D1-D35**,
ranked, engine/UX only, each with a record citation and a falsifiable prediction; both discharge
tables; fallback classes; per-kind prompt chars; specimens), and the boundary findings below.

**Ownership (owner doctrine 2026-08-26).** Reviewers own every `deckN/strategy.txt`; this step owns
the skill and the general strategy; the core loop — engine correctness, LLM UX, bugs — is the
purpose. **No guide file was edited by this step**, and none could have been: the wave-54 guides
were already deployed (`bin/Res/ai/baka/deckN_strategy.txt` verified byte-identical to
`wave54/deckN/strategy.txt`, all seven). Step 4 was run as a REPORT (B1-B10), and it found **two
live false quotations and two un-retired derivations** — the highest yield the boundary pass has had.

---

## Reconciliation against the records

1. **Health, re-derived over all 42 JSONL files.** 2,306 records = 42 `gamestart` + **42 `gameend`**
   + 10 `recovery` + **2,212 decisions** (ask 1,683 / priority 332 / attackers 99 / blockers 36 /
   discard 34 / reveal 20 / bottom 8). **21/21 natural — invariant 00 satisfied, wave-54 D1
   DISCHARGED.** 23 fallbacks (1.04%): `plan_choice_conflict` 12, `named_row_reask` 7,
   `named_row_not_offered` 2, `stale_livelock` 1, `all_assignments_illegal` 1; no `empty_reply`, no
   `timeout`, **0 `choice: -1` without a fallback**. Baka executed **2** (0.09%). `recovery` pairing
   **10/10, 0 unpaired**. Max prompt 32,215; **2 prompts over 30,000** (was 12) and **37 over 25,000**
   (was 588). Every harvest figure in the reviewer brief reproduced EXCEPT one (see 3).
2. **THE STALL FLOOR IS THE WAVE, AND THE CORRELATION SETTLES IT.** Three seats pinned the
   `dropping stale async answer` spike (63 -> **488**, +675%) to wave-53 lane AA's interrupt-window
   stall floor: it is denominated in `dt`, and `WAGIC_FASTCLOCK=0.1` turns its 1,200-*game*-second
   interactive-AI budget into 12,000 ticks. Confirmed on disk: **936 of 936
   `interrupt window held … for N ticks` lines read N = 12002 — one value, no variance** — with 468
   releases. seat-123-130 argued instead that lane M's ask-cache replay tracks the drops "more
   exactly" and that lane AA is "the RESCUER, not the cause", to be flipped LAST. **Re-derived over
   all 21 games: drops vs releases r = 0.9963** (equal exactly in 10 of 21; mean |difference| 1.14;
   434 of 488 drops within ±8 stderr lines of a release) **versus drops vs `chose to cast nothing`
   replays r = 0.5173**, with `125v123` (**809 replays / 45 drops**) against `162v130` (**5 replays /
   37 drops**) killing the two-point fit. The seat's sibling claim that "each drop is a wasted model
   call is REFUTED" is itself wrong: a dropped answer writes no record, so a 41-decision seat log is
   entirely consistent with 37 extra completed calls. **The A/B that settles it is
   `WAGIC_STALL_FLOOR=0`** on the hotfixed binary — one variable, no recipe change; `--realtime`
   (`FASTCLOCK=0`) is the no-rebuild fallback but changes pacing wholesale at ~20 wall-minutes a game.
   The hotfix lane is already rewriting the floor to wall-clock + never-while-pending and is not
   re-designed here. Recorded as skill **#231**.
3. **WHAT THE CONTAMINATION DOES AND DOES NOT LICENSE — the ruling the brief asked for.**
   Opponent-turn ask/priority windows fell **1,052 -> 441 (−58%)** while total decisions fell 36%:
   the opponent-turn SHARE of decisions fell **30.2% -> 19.9%**, a collapse the general shrinkage
   does not explain. That is the defensible corpus-level number. What the corpus does NOT establish
   is the causal chain end to end: the per-seat asymmetry is stark (`123v126`: all 64 releases on one
   seat, and that seat records **0** opponent-turn ask/priority windows against its counterpart's
   70), but at the aggregate the correlation runs the other way — a release presupposes an offered
   window, and the four zero-release games have zero opponent-turn windows too. **Ruling on lane A's
   HOLD-row predictions:** the two DENOMINATOR-DEPENDENT ones are **UNTESTED, not FAILED** —
   "opponent-turn casts >= 90% of 58" (26 casts on a 58%-smaller window pool; the RATE went 5.5% ->
   5.9%) and any comparison of HOLD renders 1,173 -> 426. The predictions measured INSIDE a class
   the floor does not gate are scored normally and **FAIL honestly**: HOLD takes in declined-note
   N >= 3 windows **22 of 212 = 10.4%** against a >= 20% target (a 25× improvement on 0.4%, and the
   denominator is conditioned on the row being offered), and identical-declined runs **16.1% /
   17.9%** against < 15% — whose largest instance is a 50-window OWN-turn run the HOLD row never
   covers, so it is not denominator-driven. **I could not reproduce the harvest's 12.2%** with either
   predicate and neither could the engine seat; my re-derivation gives 356/2,212 = 16.1% (HOLD take
   as a non-decline) and 396 = 17.9% (as a decline), and the harvest figure should not be carried
   forward. This is skill **#217** and **#208**.
4. **THE 40-RECORD DRAIN LOOP IS REAL AND ITS MECHANISM IS IN THE ROW.** `130v126` seq 67-106:
   **40 consecutive decisions**, one turn, one phase, life 20/25 -> 0/44 one point at a time — and
   **21 distinct `options_text` tuples across the 40**, because row 1's own annotation reads
   `{no creature target - and 1 to the opponent at life 26 leaves them at 25}` and that number ticks
   every iteration. `identical_option_asks_resolved: 0`, `hold_windows_skipped: 0`. Mirror at
   `123v126` seq 112-140 (29 records). This is the second-largest waste class in the corpus after the
   stall floor and the seat that found it is right about the mechanism. The remedy adopted is the
   KEY (option set, not rendered string) plus the HOLD row's own key ignoring a life-only change —
   the latter honours a choice the model made by pressing the row. The seat's *other* half, "cap
   re-opens within one phase", is rejected in that form: it suppresses windows the model never gave up.
5. **`all_assignments_illegal` IS A NEW CLASS, ITS FIRST FIRING, AND IT LOST A GAME.** `152v146` seq
   58, turn 19, Blockers, **6 life against 7**, header printing `you would be at 0 - LETHAL`. Reply
   `BLOCKS: B1:A3, B2:A4` on two menace attackers; both pruned
   (`pruned_pairs: "… (needs 2 blockers, only 1 assigned); … (needs 2 blockers, only 1 assigned)"`);
   **no blocks declared, no follower record, no `recovery`**; life 6 -> 2 at seq 59; the seat lost.
   Verified on disk. Legality is enforced structurally — but enforcing it by discarding the whole
   answer is a constraint on choice by omission, and the `named_row_reask` seam already shows the
   shape of the fix. The guide half (the model was told the menace rule three times) is deck152's
   152-D and skill **#225**; they are independent and both are owed.
6. **THE X-MENU MARKER RENDERED ZERO TIMES IN 42 LOGS, AND A RUNG WAS SCORED PASS TWICE ON IT.**
   Re-derived: `[<- most kills at any affordable X that costs you nothing]` appears in **0 prompts**
   corpus-wide. Wave 53 recorded it "taken 2/2". `130v152` s17 is the cost: at 9 life against 25 the
   pilot answered `X = 1 {kills THEIRS: none; YOURS: Goblin x3}` under a plan naming two of their
   creatures, swept its own board and lost. Starstorm is `damage:X all(creature)` (`mtg.txt:112835`),
   so the render was true throughout — this is a missing marker, not a false one. Skill **#221**, and
   it is the sharpest silent-instrument instance in eleven corpora: a missing metric and a passing
   test produced identical output.
7. **DEVOUR FLESH: 178 OF 180 ROWS PRICE HALF THEIR OWN TARGET LIST.** Re-derived: 180 rows carry a
   `legal targets right now:` list, **178 enumerate `you`**, **178** carry a `{right now: they …}`
   verdict speaking only of the opponent, 0 takes. The primitive is `target=player` with
   `sacrifice … targetedplayer`, so self-targeting is real and the verdict is FALSE for half the
   row's own list. Cost measured at `123v126` seq 104: ~900 words of reply spent asking whether
   taking the row lets it pick a target at all, then a genuine Sanguine-Bond win attempt abandoned.
8. **A SEAT VERDICT IS REVERSED ON DISK, AND IT WOULD HAVE INVALIDATED A FIELD.** seat-146-152-162's
   E-3 filed `146v152` seq 63 as a telemetry defect — the record says `choice: 5` /
   `chosen_text: "Cast nothing right now"` while `game-146v152-1788425074.stderr:2044-2070` shows
   Silverquill Command cast, moded (`chose 7 of 9`) and targeted — concluding that
   `latched_row_mismatch` fired on bad fields and that "everything downstream that reads
   `chosen_text`, this seat file's own take counts included, is affected". **The join is wrong and
   the record is right.** Seq 63's menu has FIVE rows; the cited stderr block's two casting decisions
   read `-> chose 3 of 4` and `-> chose 1 of 3`, and they sit between `Untap Phase --- Turn 14`
   (line 1993) and `Turn 15` (line 2162) — five engine turns earlier. Seq 63's own line is **3141**,
   `-> chose 5 of 5`, under `Turn 20`. The seat matched by card name in a game that casts that card
   more than once. The stamp is a TRUE positive exactly as the engine seat adjudicated it, and **both**
   `latched_row_mismatch` firings are true positives. Recorded as skill **#230**.
9. **THREE DENOMINATOR DISAGREEMENTS, NONE OF THEM AN ERROR (#208 again).** (a) `pass_hold_ambiguous`:
   the reviewer brief says all 16 are false positives; two seats independently found **13 false and
   3 true** (`146v126` s54/56/57, `CHOICE: 3 (Pass priority)` executing row 3, the HOLD row) — the
   brief's note is corrected. (b) HOLD takes in declined-note windows: 10.4% on the 212 windows that
   OFFERED the row (mine and the engine seat's) versus 8.0% on the 274 declined-note windows
   corpus-wide (seat-146's) — the conditional denominator is the right one, because a window with no
   HOLD row cannot have a HOLD take. (c) `- Paid`: **369 receipt lines across records' `events`**
   versus **15,170 occurrences across all prompt texts** — the log repeats every receipt into every
   later prompt; both numbers are true of different units and the ledger states which.
10. **THE OWNER-QUESTION RULING.** The standing trigger (2026-08-26 doctrine, restated in the brief)
    is a **repeated** mulligan against an explicit guide floor. The corpus contains **none**: 9
    mulligan decisions across 8 seats, one chain (`152v123`), and that chain was a 1-land seven at
    both looks — correct at both. The one WRONG ship is `152v146` s1, a **single** mulligan of a
    3-land seven under the NEW colour-family coverage clause, with the reply naming a colour as its
    reason. Under the doctrine that is a deck-content call owned by that deck's reviewer, who flagged
    it, diagnosed it (the trigger literal changed underneath the rule this wave) and shipped the fix
    (152-B). **No deck-content question is open for the owner.** Its render half is docket D12 and
    skill #210; the fourth-corpus recurrence is a lane, not a question.
11. **THE POSITIVE CONTROLS, because three of them are the wave's best results.** Lane C's D5 modal
    clause: **0 old / 17 new / 7 takes**, 11 of 11 Silverquill Command rows, and the card CAST TWICE
    against 0 of 28 last wave — a render change that moved a behaviour with no guide edit. Lane C's
    D10 tail: **43 of 43** DRAW PRICE rows tailed (was 58 of 85 untailed), `this KILLS you` 9 renders
    / **0 casts**, and four windows at `146v162` answered correctly off a clause **no guide mentions**,
    winning that game at 4 life (skill #223). Lane E's D21: `while ahead on LIFE` **18 renders, 0 at
    `my_life <= opp_life`** — **the corpus's only render FALSEHOOD is closed.** Lane L's trim v2:
    prompts over 25,000 **588 -> 37**, `ask` mean **−28%**.

---

## The headline: the corpus finished 21/21 and every render lane passed — and the instrument that was supposed to protect the console spent 488 of the model's answers and moved every window number in the file

**(a) INVARIANT 00 IS SATISFIED AND D1 IS DISCHARGED.** 42 `gameend`, no seat log without one, 0
timeouts, 0 crashes, 0 hangs, 0 adjudications, 1 h 45 min wall time against ~21 h. Lane X's reveal
fix held and the Sigarda shape did not recur; lane F's supervisor never had to fire. This is the
first corpus since wave 52 that is a test rather than a failed one.

**(b) EVERY RENDER LANE SHIPPED AND PASSED BY STRING, AND TWO OF THEM MOVED BEHAVIOUR.** Player
targets in kill summaries **0 of 586** wrong; the DRAW PRICE tail **43/43** with **0 casts** from a
lethal row; unconditional-removal victims **75 + 38** rows where there were none; the menu-fit clause
**1,203 renders / 218 takes**; the attackers blocker-count header **99/99**; the coverage cause
**11/11**; `ability: X's X` **0 of 401**; the ahead-on-life falsehood **0 of 18**; gain-framed
searches **0 loss-framed**; `your own effect` **0**; de-duplicated length-1 asks **0** (was 16);
`choice: -1` with no fallback **0** (was 1); `latched_row_mismatch` **2/2 true positives** with
**0 genuine mis-executions in 1,828 parentheticals** (was 2). The HOLD row's three mechanisms all
closed: index-0 degradation stamped 16/16, the plan-conflict collision **0 of 12** (was 5),
`hold_windows_skipped` non-zero on **7 of the 8** gameends with a take (was 2 of 6, and the eighth is
a Cleanup take with nothing left to skip). Takes went **29 -> 43** on a third of the renders.

**(c) AND THE WAVE'S OWN INSTRUMENT ATE THE EVIDENCE.** Wave-53 lane AA — a Vita softlock guard —
counts `dt`, and the corpus recipe pins `WAGIC_FASTCLOCK=0.1`, so its 1,200-second interactive-AI
allowance is 12,000 ticks of a headless pump: **936 of 936 release lines read exactly 12002 ticks**,
468 GPT-owned interrupt windows were force-released, **488 completed model answers were discarded
with no record of any kind**, and ≈**3.2 h of inference (+22% on the recorded 14.67 h) was bought and
thrown away**. The opponent-turn share of decisions fell 30.2% -> 19.9%. **Lane AA shipped with no
disable flag** — the one output-affecting change in the set that could not be A/B'd from the flag
table, and the one that moved the corpus. That is the silent-instrument rule violated, and it is now
a standing process line in the ledger: a lane whose change can alter any emitted string, window or
timing ships its flag in the same commit.

**(d) THE NEW HIGHS ARE FOUR PLACES THE ENGINE ASKS A QUESTION IT HAS ALREADY ANSWERED, AND ONE
WHERE IT ANSWERS FOR THE MODEL.** A mandatory Sanguine-Bond loop opened **40 full LLM windows in one
phase of one turn**, defeating the de-dup key with its own moving life number. The HOLD row is
excluded from the seat's own turn and the corpus's longest dead run is **50 byte-identical own-turn
windows** at `[you declined this exact list 49 times already this turn]`. An X menu's decisive marker
**rendered zero times in 42 logs** and the unmarked menu lost a game. A `target=player` row prices
half its own target list, **178 times**. And at a lethal blockers window the engine pruned every
assignment, **declared no blocks, and did not ask again** — the only place this wave where legality
enforcement removed the model's decision instead of shaping it.

**(e) THE BOUNDARY PASS FOUND MORE THAN THE CORPUS DID, IN A GUIDE.** deck146's cast-order entry 5
still tells the pilot **"there is no `{right now:` clause on this card and there never will be, so do
not wait for one"** and then instructs it to reconstruct its graveyard from the log — while lane C's
`{modes live right now: …}` renders on **11 of 11** of that card's rows in this corpus and the seat
scored the lane a PASS on it. A guide asserting the absence of a clause that renders every time is
the trust doctrine inverted at the author's end, and it survived the reviewer's own headline lane.
deck125 still quotes the RETIRED HOLD row string and quotes the new one zero times.

---

## What merged into the skill (209-232)

**Convergence merges (>= 2 seats):**
- **209** a guide quotes an engine literal AS a literal, never dissolved into the sentence's grammar,
  because the audit that keeps it true is a grep (deck123 S1 + deck130 S5's second half; two guides
  were describing a row that renders 0 times, and only the B1 REVERSE audit caught them).
- **210** a literal that gains a cause clause is a NEW literal even though the old one is its prefix;
  the audit's old-string column is scored on the FULL string and gains a fourth column, "the families
  the new string splits into" (deck146 S1 + deck152 152-B; one literal became two opposite decisions).
- **216** a rung break requires that a BETTER row existed on the same screen; "no alternative existed"
  is its own verdict; report `N raw / M live / K consequential` and put the exemptions in the GUIDE
  (deck125 S3 + deck130 S6; every count in the pool moved when applied).
- **220** a wave in which a guide's predictions all pass is the wave to write the fewest lines, and a
  closed lane's recital is the byte donor, retired in the same wave (deck126 S7 + deck152 S6).

**Decisive single-seat merges:**
- **211** a guide's prescribed reply template is part of the PARSER's input surface — a composition
  seam; 7 of one seat's 16 fallbacks are the guide's own PLAN vocabulary colliding with the reversal
  detector (deck123 S2).
- **212** a rung's exit test is TWO-sided: exhausted when the artefact is never produced OR when it is
  produced and the decision does not change (deck123 S3; the `N, K, M` line finally written, on the
  same line as the dead cast).
- **213** where a rule and an emitted clause can disagree, the rule says which wins; and a reviewer
  checks the primitive AND the trigger TIMING before writing "the render is false" (deck123 S4; two of
  that seat's four candidate HIGH render items dissolved on the check).
- **214** a constant floor is a comparison with one side missing; the fix is a SECOND printed literal
  that sorts the cases, not a different constant (deck125 S1; three "breaks", all correct plays).
- **215** each step of an ordered procedure names the string on screen that satisfies it, because the
  hardest-to-evaluate step falls to the bottom in practice wherever you wrote it (deck125 S2; skipped
  in 3 of 4 windows, two of them losses).
- **217** when a seat's window count moves by more than a factor, every per-window rate is suspect
  before it is evidence (deck125 S4; and it is what governed this whole wave's adjudication).
- **218** a clause the pilot keeps obeying against your instruction is a clause it has a THEORY about;
  answer the theory (deck126 S5; both breaks on the exact clause the rule names).
- **219** the OPPONENT's battlefield line is a dedicated line the guide must read whenever a tag there
  changes the price of your row; the finding question is "whose life does it move" (deck126 S6).
- **221** a rule keyed to an OPTIONAL annotation carries the absence branch, and the reviewer reports
  the marker's RENDER count (deck130 S5; 0 renders, scored PASS twice, one lost game).
- **222** bottoming is reported as offers/ships/order-implied with no verdict until the sample supports
  one (deck130 S7; first corpus with 8 bottom records).
- **223** a class the pilot got RIGHT only from a new render clause is a finding with a sentence owed —
  "WHAT THIS BOUGHT" — and the sentence states the scope the pilot left unstated (deck146 S2).
- **224** a superlative rung names the RENDERED FIELD it ranges over or it is not a rung (deck146 S3).
- **225** when a correctly-keyed rendering rule breaks anyway, the allowed edit is a recognise->do entry
  naming the WRONG ANSWER — never a third statement of the right one (deck152 S4).
- **226** when a lane adds a NUMBER to a clause a rule treats as a binary flag, the rule is rewritten as
  arithmetic in the same wave and the "FAIL" is reported as rule STALENESS (deck152 S5).
- **227** an exclusion rule states the RENDERED CLAUSE identifying its members; a category is a gloss,
  never the key (deck162 S7; 10 windows across two losses, `{feeds:` available the whole time).
- **228** a resource rule and an ordering rule are read against each other every wave: a prerequisite
  cannot be gated behind the thing it enables (deck162 S8; a read, not a corpus, and it caught a
  six-window loss).
- **229** a guide whose deck holds no instants says so once, keyed to the ask (deck162 S9; a per-deck
  fact that reads like a general one).

**This step's own, forced by the reconciliations:**
- **230** an engine claim joining stderr to a translog record is anchored to the WINDOW — option arity
  and turn banner — never to a card name (reconciliation 8).
- **231** competing mechanisms for one signal are adjudicated by a CORRELATION over all games with the
  coefficient printed, not by a matched pair (reconciliation 2; r = 0.9963 vs r = 0.5173).
- **232** the size band is restated from measurement each wave and the discipline is a NET-CHANGE
  budget, not an absolute ceiling — **measured: 40-70 KB, median 59 KB; the pool grew +19,028 bytes
  (+5.5%) in one wave** (discharges wave-53 B10).

## What was REJECTED, and why

- **"Skip the opponent-turn window whose stack is empty at a phase this seat has never cast in"**
  (deck125 G2, third wave running, restated at 41 windows / 0 casts). **REJECTED again.** A phase's
  cast history is not a legality fact; wave 53 supplied the counter-example. **And this wave the
  evidence is inadmissible on its own terms** — the seat's opponent-turn window count is precisely
  the number the stall floor moved (#217). The lever the seat wants is docket D1.
- **"Cap re-opens within one phase when the only board change is a life total"** (seat-123-130 E1's
  second half), in that form: a blanket cap suppresses windows the model never asked to give up.
  **Adopted narrowly** as D2(b) — the HOLD row's own key, which the model opts into by pressing the
  row, ignores a life-only change. Nothing is withheld from a seat that has not pressed it.
- **The suppression half of deck162 R-8.** The seat asked only for the annotation; the annotation is
  adopted (R211) and the suppression is not proposed. Same for any blind identical-menu cache.
- **seat-123-130's E2 mechanism** for the stale-drop spike (lane M's ask cache; lane AA flipped LAST
  as "the RESCUER"). **REFUTED on disk**, reconciliation 2, and its "each drop is a wasted model call
  is REFUTED" sub-claim is wrong in the other direction. Skill #231.
- **seat-146-152-162's E-3** (a mis-recorded `chosen_text` invalidating `latched_row_mismatch` and
  every downstream take count). **REFUTED on disk**, reconciliation 8. Skill #230.
- **Any win-rate attribution.** deck146 5-1, deck152 5-1, deck126 4-2, deck123 2-4, deck130 2-4,
  deck162 2-4, deck125 1-5; n = 6 per deck on real hands, and three of deck125's five losses were
  played in 23-31 decisions with one to five opponent-turn windows apiece. Counts only (#87, #167,
  #217). All seven seats are correctly recorded NO EXIT.
- **A guide compensation for any of D1-D7.** Trust doctrine: a guide describes cards and printed
  strings, never the engine's window cadence.

---

## Layer routing (`general-strategy.md`)

**ENGINE, HIGH**: the stall floor (R193) · the mandatory life-loss loop's de-dup key (R194) ·
`all_assignments_illegal` with no re-ask (R195) · the HOLD row's own-turn exclusion (R196) · the
reveal livelock (R197). **RENDER, HIGH**: the X menu's absent marker (R198) · the `target=player`
row's half-priced verdict (R199). **MED**: the 2-D ability-menu collapse (R200) · the discard ask's
missing verdicts (R201) · the `[named:` cast row's missing price (R202) · the `pass_hold_ambiguous`
tiebreak and the row-name collision (R203) · the coverage clause's two families (R204) · the blockers
menace header (R205) · the pruned-pair record (R206) · `{kills:}` ownership (R207) · ability-row life
totals (R208) · fetch-row colours (R209) · reveal-menu duplicate collapse (R210) · the own-turn
declined-note (R211) · the conditional converter in `{feeds:` (R212) · the HOLD row's unstated benefit
(R213) · the `chooseaname` header (R214). **LOW**: the unrecorded wall miss (R215) · the `(xN)` log
collapse (R216) · payee-less receipts (R217) · resource 404s (R218) · the generic effect label (R219).
**MEASUREMENT**: windows per seat-turn plus drops/releases per game (the two numbers that say whether
anything else is comparable) · a `cache_hits` field on `gameend` · marker RENDER counts for every
optional annotation a rung keys to · `N raw / M live / K consequential` per rung.
**OWNER**: the log window (R224) — and the trim WORKED, so the cost story moved to inference.
**PASS/KEEP** (recorded so no lane trims them): the `{feeds:` clause and its absence signal, the
`[named:` battlefield tag, the life-loop parenthesis's second half, the reveal chooser's qualify
tags, and the three-clause coexistence on `146v162` s46 row 5.
**Carried engine items with no corpus surface**: replay's four failures plus the equip auto-tap
blocker (D29 — still the item that compounds), `kMaxOptSources` 14 (D30), the `&&`-wrapped producer
blindness (D31), the Dominating Vampire hypothesis (D32), `GuiPlay::receiveEventPlus`'s
unconditional `Replace()` (D33), the Arena-grouping frontend item (D34), and the audit lanes'
unobservability (D35).

---

## Guide boundary findings for the deck reviewers (B1-B10)

Step 4 was run as a REPORT over the seven deployed `deckN_strategy.txt` (verified byte-identical to
`wave54/deckN/strategy.txt`), so every finding is an obligation for wave 55, not a request to change
a pending edit. Sweeps: the B1 REVERSE audit against every literal lanes A-F changed; the mood-word /
hedge sweep for the trust doctrine; the retired-derivation sweep against wave-53's B3/B6/B7
obligations; the size/net-change measurement; and the engine-vocabulary sweep (#99/#103/#136/#154).

**TRUST DOCTRINE: CLEAN, all seven.** No guide teaches doubt of a rendered value. The mood sweep
returns exactly **one** hit corpus-wide (`deck152` line 254) and it is inside a quoted FORBIDDEN
THOUGHT the guide is refuting — the #174 device, same site as wave 53. **PASS.** (B1 below is a
different failure: a guide asserting a clause does not exist, which is an author-side FALSEHOOD, not
taught doubt.)

| # | file : site | amendment | finding | suggested wording |
|---|---|---|---|---|
| **B1** | `deck146` cast-order entry 5, lines ~149-160 | **#209 / #187 / #223** | **A LIVE FALSE ASSERTION, and it is the wave's most important boundary result.** The entry reads *"The row prints its card text and its `{leaves N of your M` clause and NOTHING ELSE: there is no `{right now:` clause on this card and there never will be, so do not wait for one"*, and then instructs a name search through the log to reconstruct the graveyard. **Lane C's D5 shipped: `{modes live right now: …; dead (no legal object right now): …}` renders on 11 of 11 Silverquill Command rows in this corpus, and the seat scored the lane PASS on exactly those rows.** The guide quotes `modes live right now` **0 times**. Wave-53 B6 named this obligation verbatim; the reviewer honoured its first half (the graveyard-reconstruction instruction is gone from the DECIDING SITUATIONS block) and left the false sentence in the cast order. A guide that denies the existence of a clause the engine prints every time is the trust doctrine inverted at the author's end. | *"The Silverquill Command row prints `{modes live right now: …; dead (no legal object right now): …}`. Read it: that clause names which of the four modes has a legal object right now, including the return mode. Do not reconstruct your graveyard from the log."* Cut the "there never will be" sentence and the name-search paragraph in the same edit (#187). |
| **B2** | `deck125` line 499 (the opponent-turn windows block) | **#209 / #187** | **A LIVE FALSE QUOTATION.** The guide quotes `"Hold priority - do not ask me again this turn unless the board changes"` — a string that renders **0 times in the entire corpus** — and quotes the emitted string **0 times**, on a seat that saw **132 HOLD renders and 6 takes**. deck123 (A54-0) and deck130 (D54-0) both caught this by the B1 REVERSE audit and re-keyed; deck125 did not, and its seat file does not mention the row's text at all. The surrounding teach is still CORRECT in substance (the row is a safe decline; a new stack object re-opens it) — only the quotation is dead. | Replace the quoted string with `"Pass priority, and do not ask me again this turn unless the board changes (any change re-opens this window; you give up no cast)"`, verbatim and on its own, and keep the paragraph otherwise unchanged. |
| **B3** | `deck123` lines 850-856 (the blockers-hint bullet) | **#187** | **THE WAVE-53 B3 OBLIGATION WAS NOT HONOURED, and it is now due.** Wave 53 wrote: "PASS now, RETIRES on D21 — at which point the bullet is a derivation the literal answers, and #187 says it is cut in the SAME wave." **D21 shipped** (18 renders, 0 at `my_life <= opp_life`). The bullet still teaches the pilot to *"read it against the two life totals above it. While THEIR life is higher than yours you are not the player it describes"* — six lines describing what to do when a clause that is no longer printed in that case appears. Harmless today; a derivation over an absent literal tomorrow, which is the exact shape that cost deck126 its Lantern rule. | Wave 53's own suggested wording still applies: *"The blockers prompt prints `- NOT lethal` with your two life totals above it. When their life is higher than yours the hint's second half is not printed at all: a token goes in front of the biggest attacker."* Keep the concrete 7-against-12 citation; cut the derivation. |
| **B4** | `deck152` line 603 (`COUNT THE distinct names in "their untapped blockers"`) | **#187 / #215** | **THE WAVE-53 B7 OBLIGATION WAS NOT HONOURED.** Wave 53 wrote: "At D17: 152-A's 'count the distinct names in `their untapped blockers`' is a derivation the literal answers and is cut." **D17 shipped and renders 99 of 99 attackers prompts**, including all 27 at this seat, printing the count directly. The guide still asks the pilot to count names across A-lines — the highest-cost step in an ordered procedure, which #215 says is the step that gets skipped. | *"The attackers header prints `They have N untapped creature(s) able to block`. That N is the count — do not re-derive it from the A-lines. Declaring more than N attackers leaves the surplus unblocked; that is the trade, and the tag on any one blocker is not."* |
| **B5** | `deck130` STEP 1 of the X rung, lines 178-188 | **#221** | **PASS, and it is the model of the amendment done right — written before the amendment existed.** The rung keys to the marker, then states *"THE MARKER IS OFTEN NOT THERE. It is printed only when some X kills something of theirs and nothing of yours; across a whole corpus of twelve games it appeared ZERO times. Do not wait for it and do not read its absence as 'no row is good'"*, and carries a complete absence branch. Print condition, measured render count, and a full fallback — exactly #221's three requirements. **Obligation on D6:** when the marker is made unconditional, this paragraph's "ZERO times" recital is the byte donor (#220) and the absence branch collapses to one clause. | At D6: *"Exactly one row on every X menu carries a marker. Answer that row."* Retire the ZERO-times recital in the same wave. |
| **B6** | `deck123` A54-3 / A54-7 (the PLAN template and the cast-list numbering) | **#211** | **PASS on the edits, and a WATCH with the shortest fuse in the pool.** This seat produced **17 of the corpus's 23 fallbacks** from two guide-authoring shapes: a PLAN template whose arithmetic vocabulary ("stop reached") the reversal detector reads as a pass verdict (12 `plan_choice_conflict` corpus-wide, 7 here, four re-asks returning the byte-identical reply), and a cast list whose ENTRY numbers were answered as ROW numbers (all 7 `named_row_reask`, both Baka executions). Both edits are written. **The watch is that the fix is a composition seam**: the guide half (A54-7's CL0-CL8 relabelling) and the parser half (deck123 G2's narrowing) can each look sufficient alone and are not. Score them separately in wave 55. | none now. Wave-55 seat file reports the two counts separately (`named_row_reask` at this seat, and `plan_choice_conflict` firings whose re-ask returns the identical reply). |
| **B7** | `deck162` 162-A (`{feeds:` re-key) and `deck152` 152-B (the coverage family re-key) | **#210 / #227** | **PASS on both, with a DIRECTION note that must not be lost.** Both re-keys are the right shape — a row clause and a literal family replacing a category and a bare sentence — and both have built-in controls (the two correct Forced Fruition brakes at K of 0; the count family unchanged). **But 152-B's key depends on docket D12 landing**: if the coverage clause is NOT split or scoped, the colour family stays ambiguous and 152-B is the only thing standing between this deck and a fifth-corpus mulligan break. **If D12 does not ship, 152-B stays exactly as written.** Same direction as wave-53 B5. | none now. At D12: 152-B re-keys to the scoped literal and its family-disambiguation paragraph is cut (#187). |
| **B8** | `deck126` D100 / D101 and `deck162` 162-C | **#218 / #219 / #220** | **PASS, and the two cleanest applications of #220 in the wave.** deck126 passed every wave-53 prediction and wrote **three** edits, two of them new-shape and one a ceiling refresh; deck162 retired its closed eleven-window recital and the retirement paid for its two new findings almost exactly. Both seat files name freeze candidates explicitly. **This is what a passing guide's wave should look like** — and it is the contrast that makes B10 a finding. | none. |
| **B9** | all seven `edit-texts.md` — the STATED sizes | **#232** | **THREE SMALL DISCREPANCIES, and they matter only because the discipline is now a net-change budget.** `deck152` states the revised file at 59,215 B; on disk it is **59,972** (the 152-D edit landed after the header was written). `deck130` states its baseline at 61,789 B; the wave-53 file is **61,746**. Three files quote three DIFFERENT pool bands from memory ("39.6-70.2 KB" ×2, "39.6-74.7 KB" ×1) and none matches the distribution. Nothing here changes a verdict; all three would break a net-change budget check run by `stat`. | wave-55 brief: state the before/after byte counts by running `stat -c%s` on the two files, and take the band from this file rather than from memory. |
| **B10** | all seven `deckN_strategy.txt` — SIZE, discharging wave-53 B10 | **#232 / #220** | **THE BAND IS RESTATED AND THE POOL GREW 5.5% IN A WAVE WHERE FOUR SEATS SAID THEIR GUIDES WERE AT THEIR CEILING.** Measured on disk: **41.4 (deck162), 55.3, 58.6, 59.3, 59.8, 63.3, 69.4 KB (deck123) — band 40-70 KB, median 59 KB.** #146's "27-38 KB" is retired. Net change: +885 (123), +1,730 (162), +2,474 (146), +2,724 (126), +2,897 (152), +3,103 (130), **+5,215 (125)** = **+19,028 B, +5.5%**. Five of seven seat files honoured wave-53 B10 and stated their bytes; two did not. **deck125 grew 9.3% in the wave its own seat file argued its rates were incomparable and three of its five losses had no identifiable misplay** — which is #220's failure with the clearest available evidence, and the reason the discipline is now a net-change budget rather than a ceiling. | wave-55 brief: every seat states bytes before/after and its net change **with the trims that paid for it**; a guide whose predictions all passed states what it deleted. Synthesis publishes the band and the pool's net change each wave. |

**Not findings — checked so nobody re-files them.** deck130's X-rung absence branch (B5) and its
`{feeds:`-style key discipline; deck123's A54-1 all-dead-menu paragraph (the guide half of docket D9's
sibling — correctly written as a permission, not a prohibition, and it does not cap a choice);
deck146's 146-A DRAW-PRICE turn-level close with its explicit scope sentence (skill #223's origin, and
the scope half — attacks, land drops and loyalty abilities stay free — is stated); deck126's D101
`[named:` bullet sized to one sentence on the #198 distinct-turn evidence; deck152's 152-D
recognise->do entry naming the wrong answer rather than restating the affordance (#225 exactly);
deck125's W96/W97 second-literal branch (#214's origin, and both branches read printed strings).
Card facts spot-checked against the primitives: Sanguine Bond `mtg.txt:99004`, Exquisite Blood
`37966`, Perimeter Captain `85398`, Pride Guardian `88356`, Overgrown Battlement `83596`, Devour Flesh
(`target=player … targetedplayer`), Pelakka Predation (`optionone target(*[manacost>=3]|reveal)`),
Silverquill Silencer `borderline.txt:102764`, Silverquill Command, Starstorm `112835`
(`damage:X all(creature)`), Howling Mine `55935`, Teferi's Puzzle Box `118332`, Underworld Dreams
`125822`, Ob Nixilis `planeswalkers.txt:2684`, Marsh Flats `71436`, Tundra `124691`, Underground Sea
`125732`, Talisman of Impulse `117529` — **no primitive/Oracle divergence found by this step**, and no
seat needed Scryfall.

---

## Specimen recommendation for the owner's per-wave prompt review (invariant 0)

**SHIP `wave54/lategame-specimen.txt` = `146v162` seq 46** (turn 25, Main phase 2, **23,237 chars,
7 rows**, 6.9 s latency, **8 life vs 8 life**, 14-card hand, 4 untapped sources, log share **64.4%**).
It carries every render clause this wave shipped on one screen — `[DRAW PRICE: … - you would be at 1]`
with the life tail, `{modes live right now: …; dead (no legal object right now): …}` on a nine-mode
list, three distinct `{leaves N source(s) - …}` fit forms, the `{castable from exile - …}` cause
clause, the dungeons line and the counters/summoning-sick battlefield vocabulary — and the seat
answered `Cast nothing right now` at parity with a full grip. **That poses the owner's criterion as
one question: 23 KB bought a decline at 8-8.** It is also the file the harvest already staged as the
default, so nothing needs regenerating.

**Beside it, relabelled as the log-cost exhibit and nothing else:**
`wave54/lategame-specimen-logcost.txt` = `125v123` seq **188** (turn 63, Cleanup, `kind: discard`,
**25,767 chars, 9 rows**). Its nine rows are bare card names with no pricing, no verdict, no fit
clause, no HOLD row and no modes: **80.6% of a 25.8 KB prompt is game log, to choose two cards to
pitch.** It is the best exhibit of ONE thing — the log window (R224) and docket D9's missing discard
verdicts — and a poor exhibit of everything else. **If the owner wants one file, ship the primary.**

**seat-123-130's `123v125` seq 302-310 is DECLINED for invariant 0 and PROMOTED for the lanes.**
Nine consecutive records on a 44-permanent log at turn 63 (Upkeep ×5, Main 1 ×2, Attackers, Combat
ends), 22.0-23.8 KB each, carrying a same-phase repeat run, a HOLD take mid-run at s306 under
`[you declined this exact list 4 times already this turn]`, the declined-note counter resetting across
the phase boundary, and a correct opponent-turn edict at s310 after 300 decisions. It is the best
available EXHIBIT for the window-economy lanes (D1, D4, D21) — and invariant 0 asks its question of
ONE screen, not nine.

## The owner-question ruling

**Nothing is owed to the owner this wave.** The standing trigger is *any repeated mulligan against an
explicit floor* (2026-08-26 doctrine). Re-derived: 9 mulligan decisions across 8 seats, **one chain**
(`152v123`), and that chain was a 1-land seven at both looks — correct at both, and the keep at five
won 29-0. No mull-to-zero, no heuristic-decided opening, 8 `bottom` records all sending the most
expensive uncastable card. The single WRONG ship (`152v146` s1, a 3-land seven under the new
colour-family coverage clause, the reply naming a colour) is **not repeated**, and under the doctrine
it is a deck-content call owned by that deck's reviewer — who flagged it as the brief requires,
diagnosed it as a literal that changed underneath the rule, and shipped the fix (152-B, prediction
P5). Its render half is docket **D12**. **No deck-content question is open for the owner**, and the
only owner ITEMS in the ledger are the standing ones: the log window (D28) and the frontend/Arena
grouping (D34).

---

## What this step did NOT check

- **I did not run wagic, build anything, run git, deploy anything, or edit any file under
  `projects/mtg/bin/Res/**` or `src/**`.** Every mechanism attribution below the corpus level is
  quoted from a seat's or a lane's source READ, and is stated as a read, not as a debugger trace. In
  particular **I could not A/B lane AA**: the code-plus-log argument for D1 is strong (one tick value
  across 936 lines, r = 0.9963, the four zero-release games) but the flag flip is the proof and it has
  not happened.
- **I did not read all 21 `.stderr` files end to end.** I counted `dropping stale async answer`,
  `releasing so the game advances`, `interrupt window held … for N ticks`, `chose to cast nothing`,
  `-> chose N of M`, the pass-declined / land-drop / cast-ask / hold-re-opened / 404 families, and I
  read three windows in full (the `126v123` release cascade, the `146v123` livelock, and the
  `146v152` block that refuted seat-146's E-3).
- **I did not re-score any deck's guide rules.** Set A is summarised from the seat files' own
  verdicts; the only Set A numbers I re-derived are those a docket item or a boundary finding rests on.
- **I did not verify any lane's gate by running it.** Suite and PARSETEST numbers are quoted from the
  lane reports and the carry list. Lanes G/H/I/J/K/N are recorded UNTESTED — three seats said so
  independently, and I confirmed only that no crash, hang or new error signature appears in the stderr.
- **I did not audit `- Paid` receipts pip by pip** (369 receipt lines; counts, no-source and no-payee
  only), and I did not re-run the wave-53 opponent-turn phase-granularity analysis at per-seat level
  beyond the totals in D1.
- **I did not check any Vita transcript, replay dump, or console artifact**, and D29-D34 are carried
  from the lane reports unchanged.
- **I did not diff prompts across polls** (only the final prompt is recorded), so D5's per-tick reveal
  instability is identified and not localised — that is the flag order's job.
- **I did not re-derive the corpus's win/loss table**; `corpus-results.tsv` is quoted, and no verdict
  in any output rests on a win rate (#87, #167, #217).
- **The `#N`-family, Devour Flesh, HOLD, DRAW PRICE, modal, `{kills:}`/`{removes:}`, coverage-clause
  and identical-run censuses were taken from `options_text` (rows) and `prompt` (headers/annotations)
  as stated per census**, and the ledger names the field and unit for each (#208).
