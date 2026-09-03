# Wave-55 synthesis (2026-09-03) — TWELFTH FAIR-HAND CORPUS, the first UNCONTAMINATED window economy since wave 53, and 21/21 for a second wave running

Inputs read, in order: `~/.claude/skills/wagicgpt/SKILL.md`; `wave54/synthesis-notes.md` and
`wave54/engine-ledger.md` (the wave-55 docket D1-D35 this corpus adjudicates); `wave55/review-carry.md`
and `wave55/reviewer-brief.md`; the four seat files (`seat-engine-narration.md`, `seat-123-130.md`,
`seat-125-126.md`, `seat-146-152-162.md`); all seven `deckN/edit-texts.md`, `deckN/skill.md` and
`deckN/general-suggestions.md`; `wave55/lane-{A,B,C,D,E}.md` in full and `wave54/lane-{P,Q,R}.md` as
needed; and the corpus itself at `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-074354/` —
**42 seat JSONL and 21 `game-*.stderr`, every load-bearing number re-derived on disk** with python
over the JSONL, **counting rows from the rendered `prompt`** (never from `options_text`), and
`/usr/bin/grep` over the stderr. Card facts were re-verified against
`projects/mtg/bin/Res/sets/primitives/{mtg,borderline}.txt`; the seven DEPLOYED guides were read for
the boundary pass. Nothing was built, run, deployed or committed; no file under
`projects/mtg/bin/Res/**` or `src/**` was modified; no git command was run.

Outputs: `wave55/strategy-writing-skill.md` (the wave-54 edition **byte-verbatim** — the
**1,186,215-byte prefix is `cmp`-verified** — plus **amendments 233-266**),
`wave55/general-strategy.md` (R1-R224 with a wave-55 verdict each, **R225-R258** new and
layer-routed, with the rejections reasoned), `wave55/engine-ledger.md` (**the wave-56 docket D1-D35**,
ranked, engine/UX only, each with a record citation and a falsifiable prediction; both discharge
tables; fallback classes; per-kind prompt chars; specimens), and the boundary findings below.

**Ownership (owner doctrine 2026-08-26).** Reviewers own every `deckN/strategy.txt`; this step owns
the skill and the general strategy; the core loop — engine correctness, LLM UX, bugs — is the purpose.
**No guide file was edited by this step**, and none could have been: the wave-55 guides are already
deployed (`bin/Res/ai/baka/deckN_strategy.txt` verified byte-identical to `wave55/deckN/strategy.txt`,
all seven, by `md5sum`). Step 4 was run as a REPORT (B1-B9).

---

## Reconciliation against the records

1. **Health, re-derived over all 42 JSONL.** 3,264 records = 42 `gamestart` + **42 `gameend`** +
   7 `recovery` + 2 `wall_miss` + **3,171 decisions** (ask 2,416 / priority 555 / attackers 90 /
   discard 68 / blockers 21 / reveal 12 / bottom 9). **21/21 natural — invariant 00 satisfied.**
   14 decision fallbacks (**0.44%**, back to the wave-53 floor): `plan_choice_conflict` 9,
   `named_row_reask` 3, `stale_echo` 1, `unparsed_reply` 1; **no `all_assignments_illegal`, no
   `stale_livelock`, no `empty_reply`, no `timeout`**. Baka executed **2 = 0.063%**. `recovery`
   pairing **7/7, 0 unpaired**. `pass_hold_ambiguous` **0** (was 16). Max prompt **30,031**;
   **1 prompt over 30,000**. Every harvest figure in the reviewer brief reproduced.
2. **LANE R IS DISCHARGED AND THE CLEARANCE IS THE WAVE'S ENABLING FACT.** Stall-floor releases
   **0** (was 468), `interrupt window held` **0** (was 936), `dropping stale async answer` **43**
   (was 488), `giving this decision to the heuristic` **0**. Opponent-turn share of decisions
   **19.9% -> 33.8%**; opponent-turn casting windows 403 -> **890**; decisions 2,212 -> 3,171 (+43%).
   Every wave-54 rate marked UNTESTED (denominator moved) is scored normally this wave. **The residual
   43 drops, classified by the line preceding each across all 21 stderr:** 16 follow a resolving
   `10DrawAction`, 10 a resolving `NextGamePhase`, **8 the engine's own
   `hold re-opened ... the board changed`**, 9 other; the next window is a `Casting decision` in 20 of
   43 and a `Land drop:` ask in 16 — the seam alternation lane E proved with `WAGIC_GPT_DRIFT`.
   Cost ≈0.31 h (was ≈3.2 h). **Ledger D18, MED, no lane of its own.**
3. **THE HOLD-ROW CONFLICT: it is not one rate, it is five, and the split is engine on one side and
   guide on the other.** Lane A gave the row COMPLETE coverage — 2,352 renders, last row 2,352/2,352,
   own-turn 1,323, and present in **753 of 753** of the declined-note N >= 3 windows. Corpus-wide
   adoption **67/753 = 8.9%** (I reproduce the harvest exactly). Split by deck (N >= 2 windows):
   **deck123 54/141 = 38.3%** (its wave-54 guide names the row AND commands it: "take that row at the
   FIRST dead window of the turn, not the fifth") · **deck130 8/48 = 16.7%** (quotes the declined-note
   literal, no imperative) · **deck125 25/381 = 6.6%** (names the row and calls it optional: "Take it
   or take 'Cast nothing right now' - **both are correct**") · **deck126 1/155 = 0.6%** and
   **deck146+152+162 0/190 = 0.0%** (say nothing about the row).
   **Ruling.** seat-123-130's 48 of 144 = 33.3% at its own pair is EXACT (46/123 + 2/21); so is
   seat-146-152-162's 190 windows / 0 takes; so is the corpus 8.9%. **All three numbers are right and
   they are the same distribution seen from three seats.** The residual after lane A is **GUIDE-SIDE**,
   which is seat-146's conclusion — but its stated MECHANISM ("the pilot takes the row its guide
   names") is too weak: deck123 and deck130 quoted the *retired* wave-54 string, which renders **0**
   times here, and still converted at 38.3% and 16.7%. **The pilot follows the description and its
   imperative, not the quotation** (skill #264), and deck125 is the decisive cell — most windows, names
   the row, converts at one sixth of deck123's rate because its sentence grants permission instead of
   issuing an order (skill **#233**, the wave's strongest single result).
   **What the ENGINE still owes on the same seam is a different number**: 113 of 282 takes (40.1%)
   were followed by at least one byte-identical same-turn re-ask, 301 re-asks in all — see 4.
4. **THE D2 LOOP PREDICTION FAILS, AND ITS CAUSE IS PRECISELY LOCATED.** `123v126` seq **75-87** is
   13 consecutive decisions in one Upkeep, life 20 -> 2, the two-row menu byte-identical, the declined
   note running to **27**, and **the HOLD row taken at s81-s85 and honoured none of the five times**;
   `123v162` seq 36-50 is 15 in one Draw step; seat-125-126 counted 16 at its own seat. Lane A's
   "< 5 decisions per loop" **FAILS**, and the engine seat's "UNTESTED (no window)" is wrong — the
   loop arose, at the other side of the game it read. **Lane A's two halves both work**: the option-SET
   key held (the note counts unbroken to 27 across annotations that move every iteration, exactly what
   defeated the wave-54 key) and the life drop held (**0 of 113 re-opens is a life-only change**).
   The re-opener is the STACK LINE, which is still in `holdBoardKeyOf`, and in the loop it alternates
   `Sanguine Bond's Life Loss` / `Exquisite Blood's Life` every iteration. **seat-123-130's E1
   mechanism ("a life total moving between windows re-opens the byte-identical option set") is REFUTED
   on disk** — its own cited repro `123v125` s104 -> s105 re-opens on a stack top and a hand-size
   change — **and its REMEDY is adopted in full**: make the re-open predicate the rendered option rows.
   Corpus-wide, **80 of the 111 identical-declined runs have a varying stack top while the rendered
   rows never change**, and those runs cost **2.91 h = 12.4% of the corpus's inference**. Ledger **D1**.
5. **A LANE SHIPPED A CLAUSE WHOSE EVERY NUMBER IS FALSE AND TWO SEATS SCORED IT PASS.** Lane C's D16
   put life arithmetic on ability rows that point damage at a player. The engine seat's #188 audit
   recorded 0 / 251 / 6 and scored **PASS on presence**. Re-derived against the records' own
   `opp_life`: **238 of the 248 `they would be at K` rows compute K from a CONSTANT BASE OF 166** —
   `2 damage -> at 164` on 176 rows, `1 damage -> at 165` on 61, at every real opponent life from 34
   down to 1, no variance; the 10 correct rows are target-menu player rows, a different emitter, and
   the spell cast-row tail is correct throughout. seat-123-130 found it by reading a number and its
   251/251 is the same finding on a slightly wider predicate. **Ledger D2 (HIGH), skill #259.**
6. **`reveal_stall` IS A TELEMETRY DEFECT, AND THE THREE-PART RULING IS THE ONE THE BRIEF ASKED FOR.**
   (a) the LIVELOCK half of D5(a) is a genuine **PASS**: `stale_livelock` 0 (was 1), wave-54 HIGH #4
   does not recur; (b) the FORCE-CLOSE half is **UNTESTED (N = 0)** and now for the right reason — no
   reveal parked; (c) the STAMP is a **DEFECT**: present on **12 of 12** reveal records,
   `reveal_stall_secs` equal to `latency_ms/1000` to the second on all twelve (4 … 474),
   `reveal_stall_phase` constant 0, against a poll-churn floor of 2,700 s. Three seats found this
   independently. Ledger **D12**, skill **#263**.
7. **THE LATENCY RULING: contention ACCEPTED, and confirmed by a control the seat did not run.** The
   engine seat argued server contention from cross-corpus prompt-size bins plus concurrency
   arithmetic. The decisive test is inside ONE corpus: 21 games launched together at `-j 21` finished
   over 1 h 58 min, so concurrency falls monotonically while each game's prompts GROW — the rival
   hypothesis predicts the opposite sign. `125v152` Q1 -> Q4: median latency **17.8 s -> 3.0 s** at a
   mean prompt of **12,622 -> 26,919**; `123v125` **24.5 -> 4.8 s** at 6,196 -> 16,431; `125v126`
   **20.7 -> 2.6 s** at 6,670 -> 17,847. Across 21 games,
   **corr(mean concurrency, median latency) = +0.82**. **Consequence, and it is a docket change:**
   per-decision inference at a fixed `-j` on a shared server is not an engine metric, lane R's
   `<= 22 s` is NOT SCORED rather than FAILED, and a prompt-byte trim may not be justified by a
   latency argument at this concurrency. Ledger **D19**, skill **#262**.
8. **A HIGH IS RE-SCOPED BY THE GUIDE THAT MANDATES ITS EVIDENCE.** The engine seat filed HIGH #3: the
   pilot answered `X = 3` on every Sphinx's Revelation menu where a larger X was affordable, and the
   menus carry no marker, no fit clause and no collapse — with a proposed wave-56 experiment to settle
   whether the anchor was the render or the guide. **The experiment is unnecessary**: deck125's guide
   carries a section headed `=== SPHINX'S REVELATION - X IS ALWAYS 3 ===` ending "reply with the
   OPTION number whose line reads 'X = 3'. Every time, and with no arithmetic in front of it.", and
   the deck seat scored the same records **11/11 CORRECT**. The take pattern is also one over: **6 of
   6** sub-maximal, not 7 of 7 (five further menus topped out at X <= 3 and it took the max on all
   five). The render item that survives is real and smaller — **the monotone family carries no
   `{leaves N sources}` fit clause, alone among priced rows** — and is MED. Ledger **D7**, skill #261.
9. **A DOCKET PREDICTION AND ITS LANE'S PREDICTION WERE TWO DIFFERENT TESTS, AND TWO SEATS SCORED
   DIFFERENT ONES.** D6 said "X menus rendering with no marker: 0"; lane C shipped three markers that
   rank a KILL dimension and wrote its own prediction as "X menus **(kill-priced)** ... 0". This
   corpus: 14 X menus, **3 kill-priced and all 3 marked** (lane C PASSES in scope, marked row taken
   2 of 3), **11 monotone with no marker family that fits**. One seat scored FAIL 11/14, another
   UNTESTED. Both defensible, neither useful. Skill **#260**: the ledger records both and the residual
   family becomes a new ranked item.
10. **THE `[RE-ASK]` DEFECT IS CONFIRMED AND IT COST A PLAY.** 12 notices corpus-wide, 8 of the
    CHOICE-line form, **2 mismatched — and both are exactly the records where `coded_answers = 2` and
    `latched_coded_line = 2`.** `130v123` s109 latched line 2 (`CHOICE: 5 (cycling with Starstorm)`)
    and executed row 5 correctly, then re-asked with `takes row 5 ("CHOICE: 0 (pass)")` and prose
    evidence lifted from the discarded first line; s110's entire reply is `CHOICE: 0 (pass)` and the
    record carries `plan_choice_conflict_recovered` — **a false success**. Ledger **D3 (HIGH)**.
11. **THE MDFC ITEM IS CONFIRMED AND SHARPENED.** 0 land-drop asks name `Emeria, Shattered Skyclave`
    or `Agadeem, the Undercrypt` in 3,171 decisions; the only rows naming them are 15
    `Flip Side ... -> DISPLAY TOGGLE only (this card's other face is a LAND)` rows, which say so
    honestly. `146v130` seq 22/23/24 are three land-drop-less turns at 2-3 Plains with an Emeria's
    Call in hand; the seat lost 19 -> 0. Primitive gate verified (`compare(isflipped)~equalto~1`).
    Ledger **D8**, and deck146's absence assertion is the pool's one legitimate one (B5 below).
12. **THREE DENOMINATOR DISAGREEMENTS, NONE OF THEM AN ERROR (#208 again).** (a) The HOLD row renders
    on **2,354 windows / 2,352 decisions** — the two extras are `wall_miss` records. (b) Total
    inference is **23.48 h over all positive latencies** and **22.98 h over decisions only**; the
    0.50 h difference is the two 900 s `wall_miss` records, which is exactly why the seat's 26.09 s
    and my 26.66 s reconcile. (c) Identical-declined runs are **21.5%** on the harvest predicate
    (HOLD take NOT a decline) and **25.2%, 111 runs, max 16** on mine (HOLD take counted as a
    decline); seat-146-152-162 could not reproduce either and correctly reported the declined-note
    share instead rather than guessing. All are true of different units and the ledger names each.

---

## The headline: lane R gave the loop its window economy back, every render lane shipped — and the wave's three worst findings are a latch that forgets what the model actually answered, a clause whose every number is a constant, and a re-ask that quotes the line it threw away

**(a) THE INSTRUMENT IS CLEAN AND THE CORPUS IS A TEST.** 21/21 natural for a second wave, 0 stall-floor
releases, 43 drops, fallbacks back to 0.44%, `pass_hold_ambiguous` 0, `all_assignments_illegal` 0,
`stale_livelock` 0, and 43% more decisions than wave 54. Wave-54's D1, D2(one half), D3, D4, D5, D7,
D9(partial), D15, D17, D18, D22 and D23 are all discharged, most of them PASS.

**(b) EVERY LANE SHIPPED ITS STRINGS, AND THREE OF THEM MOVED BEHAVIOUR.** The both-player edict verdict
renders on 151 windows and its commit clause on 217, answering on the row the exact question the
wave-54 model spent 900 words asking. Lane D's collapse took the `#N` mega-menu class to **0 records
with more than 3 rows of one shape** and put a 47-option reveal into a 9 KB prompt. The X marker
rendered **3 of 3** after rendering 0 in 42 logs last wave, on the bottom row of a collapsed run both
times, and the marked row was taken twice. `stale_livelock` went 1 -> 0, `pass_hold_ambiguous` 16 -> 0,
and the four-corpus colour-family mulligan break did not recur.

**(c) AND THE LATCH THE WAVE BUILT DOES NOT KEEP ITS PROMISE.** The HOLD row's own text says
`do not ask me again unless the board changes ... {taking this row skips the rest of this turn's
identical windows}`. **113 of the 282 takes were followed by a byte-identical same-turn re-ask**, 301
in all; **80 of the 111 identical-declined runs are re-opened by a stack top that changes while every
printed row stays the same**; a mandatory Sanguine Bond loop spent **13 windows in one Upkeep with the
row taken five times inside it**; and 8 of the 43 residual stale drops are the latch throwing away an
in-flight answer. The key lane A built is right and the predicate it hangs on is one level too broad:
the model answered a QUESTION, and the engine retires its answer on a change to the WORLD.

**(d) TWO OF THE WAVE'S THREE HIGHS ARE INSTRUMENT FAILURES DRESSED AS FEATURES.** 238 of 248 rows that
tell the pilot where the opponent's life will land compute it from a constant 166 — shipped by a lane
that pinned its helper, audited by a seat that counted the clause and never read a value, and scored
**PASS 0/251**. A `[RE-ASK]` notice built to resolve a two-line reply quotes the line the engine
DISCARDED beside the row number of the line it KEPT, and the one time that mattered it converted a
legitimate Starstorm cycle into a pass and logged the result as a recovery. And a stall stamp designed
to fire only on a parked reveal fires on 100% of reveals with a value the record already carries.

**(e) THE GUIDE HALF OF THE WAVE'S BIGGEST ECONOMY NUMBER IS A FIVE-CELL NATURAL EXPERIMENT.** Coverage
of the HOLD row is complete and adoption ranges from **38.3%** where a guide commanded the row to
**0 of 190** where three guides never mentioned it, monotone in the strength of the sentence, with the
deck that called the row "also correct" converting at 6.6% on the largest denominator in the pool.
That is the cleanest evidence this loop has produced that recognition text moves nothing and an
imperative moves a rate — and all seven wave-55 guides now carry a HOLD block, five of them new, so
wave 56 re-measures the same five cells.

---

## What merged into the skill (233-266)

**Convergence merges (>= 2 seats):**
- **234** every "any row" absolute carries its exclusion list inline (deck125 S4 + deck126 S9; it fired
  at deck126, in both directions, three windows apart).
- **238** a guide may cite a count as EVIDENCE, never as a FACT ABOUT THE RENDER — and an "the engine
  never offers X" claim ships with its re-verification grep, its primitive, and its retirement trigger
  (deck123 S1 + deck146 S-3; deck146's MDFC block is the pool's model and deck130's new "three of
  three" recital is the fresh violation).
- **239** closing a render item and re-keying the guide paragraph built on its absence are ONE pass
  (deck123 S2 + deck130 G7; three items closed at one seat pair, each with a paragraph shaped around
  its absence, each replacement shorter than what it replaced).
- **240** count row literals from the rendered `prompt`; `options_text` is a different object and it
  reported a shipped lane as missing (deck125 S5 + seat-125-126 LOW #6, after lane D found three
  wave-54 docket numbers derived from the array).
- **247** run the DECKLIST against the primitives once per deck for the structural properties that
  create a dead second copy or split the creature count from the attack list (deck123 S7 + deck130
  S13; `Rorix Bladewing *2` × `type=Legendary Creature` was on disk before the corpus ran).
- **263** a telemetry field present on 100% of a record kind is not an alarm (three seats, on
  `reveal_stall`), with the new second clause that a disable flag which CRASHES is worse than no flag.

**Decisive single-seat merges:** **235** key the rule to the number the ROW prints (deck123 S3) ·
**236** three paragraphs over three corpora means file the render item, with the sibling comparison as
the argument (deck123 S4) · **237** a gate on a count the pilot must convert is a proxy, and the breaks
cluster at the proxy's boundary (deck130 S9) · **241** a priority entry that can match two rows names
its winner in the entry (deck126 S8) · **242** the denominator for a list entry is windows where no
higher entry matched (deck126 S10) · **243** "can this deck ever act here?" gates adding guide text for
a window class (deck126 S11) · **244** a marker answers WHICH, never WHETHER (deck130 S8) · **245** two
thresholds on one action become one written line with one blank each (deck130 S10) · **246** the "only
playable card" failure mode gets one identically-worded paragraph in every guide with a hold rule, and
deck123's is the byte donor (deck130 S11) · **248** an ordered prose rung becomes a numbered SCAN with
an anti-anchor clause and a comparison step (deck146 S-1) · **249** an enumerating clause without a
quantifier is not a count (deck146 S-2) · **250** the trust doctrine's converse — a printed ROW is the
engine's assertion of legality (deck152 S-1) · **251** a matchup posture keys to a countable render
feature (deck152 S-2) · **252** a re-key is audited for the literal's COVERAGE, not its render count
(deck152 S-3) · **253** a brake ships with its RELEASE (deck162 S-1) · **254** a prohibition on a PLAN
SENTENCE is a prohibition on a paraphrase class — state the structural test (deck162 S-2) · **255** a
danger that lives in a relation is written as the comparison (deck125 S2) · **256** a gate whose every
violation was correct is mis-scoped (deck125 S3) · **257** send-list head as a TIER, tail as a sequence
(deck125 S6) · **258** a clause that lands INSIDE a string a rule keys on changes the composed string
(deck126 S7) · **265** where a rung HELD, change register from rebuke to standard (deck123 S5) ·
**266** a guide needs a small MENU-LEVEL section (deck123 S6).

**This step's own, forced by the reconciliations:**
- **233** an equivalence changes nothing; only an imperative moves a rate — the five-guide HOLD-row
  table (reconciliation 3). It generalises deck125 S1 and it governs every "the pilot ignores our
  clause" finding.
- **259** a literal audit's three numbers test PRESENCE, not TRUTH: a clause carrying a computed number
  is verified against the record's own field (reconciliation 5; 238 of 248 values false under a
  PASS 0/251).
- **260** a lane that narrows a docket item's scope creates two predictions; the ledger records both
  and the residual family is a new item, not a FAIL (reconciliation 9).
- **261** before filing a render item whose evidence is a repeated identical answer, grep the seat's
  own guide for the imperative that mandates it (reconciliation 8; cost one HIGH rank and one lane).
- **262** explain a corpus-level metric shift with the within-corpus control in which the rival
  explanation predicts the opposite sign (reconciliation 7; r = +0.82, and prompt bytes doubling while
  latency fell 6×).
- **264** a stale quoted literal is an AUDIT debt, not automatically a behaviour failure, because the
  pilot follows the description — #209 refined by the two decks that quoted a dead string and
  converted best (reconciliation 3).

## What was REJECTED, and why

- **"Skip the opponent-turn window whose stack is empty at a phase this seat has never cast in"**
  (deck125 G3, deck126 G9, FOURTH wave running, now at 291 windows / 0 casts on an UNCONTAMINATED
  denominator). **REJECTED again.** The denominator objection is gone; the doctrine objection is not —
  it removes a legal window. The levers the seats want are ledger D1 (which withholds nothing from a
  seat that has not pressed a row) and R243 (tell the pilot the menu is dead instead of hiding it).
- **"When the declined note reaches a threshold, stop asking for the rest of the turn and record an
  auto-hold"** (deck162 R-1's aggressive half; the engine seat's escalation candidate under its own
  HIGH #1 falsifier). **REJECTED.** The seat filed it as a proposal precisely because it removes a
  legal choice without the model asking.
- **"Hoist the HOLD row to row 1 when the declined note reads N >= 3"** (deck162 R-1's cheap half,
  seat-146-152-162 E-3). **NOT ADOPTED by this step**, and recorded so it is not re-filed as new. It
  removes nothing and the row-1 anchoring evidence is good (three independent measurements), but
  decline-ordering is a **standing owner ruling** — declines go LAST *because* the model favours option
  1 — and reversing it to exploit the same bias is the owner's call, not synthesis's. R249 takes the
  part that needs no ordering change: the declined COUNT inside the HOLD row's `{...}` annotation,
  which `optionSetKeyOf` already strips, resolving the key collision lane A named in writing.
- **seat-123-130's E1 MECHANISM** ("a life total moving between windows re-opens the byte-identical
  option set"). **REFUTED on disk** — 0 of 113 re-opens is life-only; its own repro re-opens on a stack
  top. **The finding and the remedy are adopted in full** as ledger D1; only the cause is rejected.
- **The engine seat's HIGH #3 as filed** and its proposed wave-56 experiment. **RE-SCOPED** — deck125's
  guide mandates `X = 3` verbatim (skill #261); and its "7 of 7" is 6 of 6.
- **The engine seat's PASS on the ability-row life tail** (0 of 251, presence only). **OVERTURNED**:
  238 of 248 values false (ledger D2).
- **The engine seat's "UNTESTED (no window)" on lane A's life-loss-loop prediction.** The loop arose at
  `123v126` s75-s87 and the prediction **FAILS** at 13, 15 and 16.
- **A guide compensation for any of ledger D1-D6.** Trust doctrine: a guide describes cards and printed
  strings, never the engine's window cadence, and never hedges against a rendered number — which is
  also why **no guide sentence may be written against D2's false life totals**; the surface is fixed.
- **Any win-rate attribution.** deck162 5-1, deck130 4-2, deck125 3-3, deck146 3-3, deck152 3-3,
  deck126 2-4, deck123 1-5; n = 6 per deck on real hands, in the wave whose window economy changed by
  a factor of four. Counts only (#87, #167, #217). All seven seats are correctly recorded NO EXIT.

---

## Layer routing (`general-strategy.md`)

**ENGINE, HIGH**: the HOLD latch's re-open predicate (R225) · the ability-row life falsehood (R226) ·
the `[RE-ASK]` wrong-line quote (R227). **RENDER, HIGH**: the bare discard rows (R228) · the missing
incoming-combat total (R229). **MED**: the empty `chosen_text` (R230) · the monotone X family (R231) ·
MDFC land backs (R232) · the edict tie clause's missing quantifier (R233) · the opponent's open mana
(R234) · plan staleness (R235) · `reveal_stall` (R236) · the residual seam alternation (R237) ·
`{feeds:`'s release (R238) · `{MV n}` on discard rows (R239) · the `[doesn't untap]` consequence
(R240) · the plan-vs-row-clause note (R241) · the nested blocking-trigger clause (R242) · the
menu-level dead verdict (R243) · the `plan_choice_conflict` predicate (R244) · what-changed on a
repeat (R245) · the unpriced equipment row (R246) · the duplicate cast row (R247) · the marker as a
ranking (R248) · the declined count inside the HOLD row (R249). **LOW**: the `(xN)` log collapse
(R250) · the generic named-narration label (R251) · the conditional `{kills N of the ...}` (R252) ·
resource 404s (R253) · the DRAW FORECAST product (R254).
**PROCESS/MEASUREMENT**: the `WAGIC_GPT_BOARDINDEX=0` segfault (R255, above any new instrumentation) ·
`cache_hits` on `gameend` (R256, third wave) · **per-decision inference is reported with its
concurrency and never carried as a target (R257)** · the log window, OWNER (R258).
**PASS/KEEP** (recorded so no lane trims them): the two-sided edict verdict and its commit clause · the
`{kills whichever you target: THEIRS/YOURS}` split and the `{X pricing: same kills as X=N}` bottom-row
marker · lane D's decode line, the copy-tag collapse, and the `(copy N of N in your hand)`
disambiguator · deck162's `DRAW FORECAST` + `DRAW PUNISHERS` pair · the `[named: <card>]` tag · the
LIFE-TO-DAMAGE CONVERTER block and its `their life LOOP is in play` attacker clause · lane E's
mulligan scope tail · the attackers blocker-count header at 90 of 90.
**Carried engine items with no corpus surface**: lane M's cache counters (D25) · lane B's un-executed
arms (D26) · `{feeds:` conditional converters (D27) · replay's four failures + the owner-approved RNG
split (D28) · `kMaxOptSources` 14 (D29) · the `&&`-wrapped producer (D30) · Dominating Vampire and the
Vita open reports (D31) · `GuiPlay::receiveEventPlus` (D32) · the Arena-grouping frontend item (D33) ·
the audit lanes' unobservability (D34) · the log window (D35).

---

## Guide boundary findings for the deck reviewers (B1-B9)

Step 4 was run as a REPORT over the seven DEPLOYED `deckN_strategy.txt` (verified byte-identical to
`wave55/deckN/strategy.txt`, all seven), so every finding is an obligation for wave 56, not a request
to change a pending edit. Sweeps: the B1 REVERSE audit against every literal lanes A-E changed; the
mood/hedge sweep for the trust doctrine; the absence-assertion sweep (skill #238's mechanical check);
the retired-derivation sweep against wave-54's B3/B4/B5 obligations; and the size / net-change
measurement.

**TRUST DOCTRINE: CLEAN, all seven.** The hedge sweep (`may be wrong`, `do not trust`, `the number may`,
`is sometimes wrong`, `engine sometimes`, …) returns **zero hits corpus-wide**. No guide teaches doubt
of a rendered value. **PASS.**

**WAVE-54 OBLIGATIONS: three of four HONOURED.** B3 (deck123's ahead-on-LIFE derivation) is **cut**.
B4 (deck152's "COUNT the distinct names in `their untapped blockers`") is **cut** — the remaining
mentions are tag reads. B5 (deck130's X-rung ZERO-times recital) is **retired**. **B2 (deck125's dead
HOLD quotation) is NOT honoured and is dead again** — B1 below.

| # | file : site | amendment | finding | suggested wording |
|---|---|---|---|---|
| **B1** | `deck125` lines 552-557 | **#209 / #264 / #233** | **A LIVE FALSE QUOTATION, THIRD WAVE RUNNING, IN THE POOL'S HIGHEST-VOLUME SEAT.** The guide quotes `"Pass priority, and do not ask me again this turn unless the board changes (any change re-opens this window; you give up no cast)"` — the **wave-54** string, which renders **0** times in this corpus — and quotes the live string **0** times, on a seat with **381 windows** carrying the declined note at N >= 2. Wave-54 B2 named this deck for the same failure against the wave-53 string; the reviewer updated the quotation to a string lane A retired in the same wave. **The quotation is not the whole problem** (#264: deck123 and deck130 quoted dead strings and converted best) — the paragraph's own sentence is: *"Take it or take 'Cast nothing right now' - **both are correct**."* That is recognition text, and this deck converted at **6.6%** against deck123's **38.3%** on an imperative. | Replace the quoted string with the live one verbatim — `"Hold priority for the rest of this turn: pass now, and do not ask me again unless the board changes (any change re-opens this window; you give up no cast) {taking this row skips the rest of this turn's identical windows}"` — and **replace "both are correct" with an order and its wrong answer**: *"When the prompt carries `[you declined this exact list N times already this turn]`, the HOLD row is the answer and `Cast nothing right now` is the wrong one."* |
| **B2** | `deck126` — the whole file | **#233 / #243** | **THE GUIDE THE CORPUS RAN SAID NOTHING ABOUT THE HOLD ROW, AND THE SEAT TOOK IT 1 TIME IN 155 N >= 2 WINDOWS.** deck126 was offered **169 opponent-turn windows across six games and made zero casts in them**; 91.4% of them sit inside runs of 3+ byte-identical declined menus. The wave-55 guide adds a HOLD block, so this is a WATCH, not a debt — and it is the cleanest single test of #233 in the pool because the deck's own seat file also argues (S11, adopted as #243) that no further opponent-turn instruction is worth bytes there. **Both can be true**: a HOLD imperative is not opponent-turn strategy, it is a way to stop being asked. | none now. Wave-56 seat file reports this deck's HOLD take rate at N >= 2 against **1 of 155**, and states whether the new block is an imperative or a recognition. |
| **B3** | `deck146` line 35, `deck152` lines 508 / 515 | **#210 / #209** | **THREE GUIDES QUOTE THE MULLIGAN COLOUR CLAUSE WITHOUT LANE E'S NEW SCOPE TAIL, so the quoted string renders 0 times as written.** deck146 quotes `"(you have 2 lands; no spell in it is castable off {B} alone)"` and deck152 quotes `"(you have 3 lands; no spell in it is castable off {W}{U} alone)"`; the emitter now always appends **`on turn one`** (1 land) or **`even with every land in this hand in play`** (2+). Skill #210 is exact on this: a literal that gains a cause clause is a NEW literal even though the old one is its prefix. Not false — a prefix — but un-greppable and, worse, **the tail is the half that carries the decision**, and the two families call for opposite answers. deck126 line 569 quotes the one-land branch WITH its tail and is the correct model. | Quote the full emitted string in each of the three sites, and split the paragraph on the tail: the `on turn one` branch is a turn-one statement about a hand that may still be kept; the `even with every land in this hand in play` branch is a hand-level verdict. |
| **B4** | `deck130` lines 185-195 (X-rung STEP 1) | **#238** | **THE SEAT THAT PROPOSED THE RULE WROTE THE VIOLATION, IN THE SAME WAVE.** deck130's own skill S1 (adopted as #238) says a guide *"may not state a count as a FACT ABOUT THE RENDER"*; the deployed rung opens *"Last corpus every X menu this deck reached carried a marker - **three of three**"*. It is TRUE today and it is the identical shape to the `ZERO times` recital it replaced — which is the sentence that licensed `130v146` s40's miss. The rung's imperative ("Answer the marked row's number and read nothing else") is correct and needs no count in front of it. Note the honest half: this rung retired wave 54's recital as its own amendment required, so the debt is one sentence, not the paragraph. | Cut the count. *"Every X menu this deck reaches carries exactly one `[<- ...]` marker, in one of two forms: ... Answer the marked row's number and read nothing else."* Add the absence branch as a conditional, per #238: *"If no marker is printed, ..."* |
| **B5** | `deck146` lines 119-125 | **#238** | **PASS, and it is the pool's model for an absence claim.** The guide asserts *"the game NEVER offers you their land face"* about Emeria's Call and Agadeem's Awakening — a sentence #238 would normally forbid — and it survives on all three of that amendment's conditions: **re-verified by this step (0 land-drop asks name either back face in 3,171 decisions; the only 15 rows naming them read `Flip Side ... -> DISPLAY TOGGLE only`)**, grounded in the primitive (`compare(isflipped)~equalto~1`, `borderline.txt:33968`), and carrying a standing prediction (P10) whose stated purpose is to detect the day the engine is fixed and delete the block in the same wave. | none. **Obligation on ledger D8:** when the land face is offered, this block is deleted in the same wave and P10 flips from "still 0" to "non-zero". |
| **B6** | `deck123` lines 128-142 and 805-810 | **#233 / #265** | **PASS, and it is the wave's strongest guide result.** The deployed guide names the live HOLD string, commands it (*"take that row at the FIRST dead window of the turn, not the fifth"*), states the engine-side residue as not the pilot's problem (*"THE ROW IS NOT KEPT WHEN A LIFE TOTAL IS TICKING, AND THAT IS NOT YOUR PROBLEM TO SOLVE"* — #265 written before the amendment existed), and re-labels the declined note as *"a counter, not new information"*. The wave-54 version of this paragraph produced **38.3%** adoption at N >= 2 against a pool floor of 0.0%. **It is also the only guide in the pool that SHRANK this wave (-113 bytes).** | none. Wave-56 measures whether the live-string version holds 38.3% — and whether the ledger D1 fix makes the residue paragraph retirable (#187). |
| **B7** | `deck162` lines 472-486 | **#233 / #253** | **PASS on shape, and it is the sharpest test of #233 in wave 56.** The new HOLD block is an imperative with its wrong answer named (*"answer the HOLD row, not 'Cast nothing right now'"*), keyed to a single literal, with the absence branch stated and a WHAT THIS COST receipt. That seat took the row **0 of 65** N >= 2 windows this corpus with no guide text at all. | none. Wave-56 reports this deck's rate against **0 of 65**; if an imperative this clean does not move it, #233 is wrong and the item returns to the render (ledger D1 / R249). |
| **B8** | all seven `edit-texts.md` — the STATED sizes | **#232** | **PASS, and the discipline is working.** Five of seven seat files state before/after bytes; measured on disk, no discrepancy changes a verdict. The band moved with the pool and is restated in B9. | none. |
| **B9** | all seven `deckN_strategy.txt` — SIZE and NET CHANGE | **#232 / #220 / #233** | **THE POOL GREW +22,980 BYTES (+5.5%) — THE IDENTICAL RATE TO WAVE 54, ON A BIGGER BASE.** Measured: deck162 **45.4**, deck126 60.4, deck152 63.9, deck146 64.1, deck125 66.9, deck130 67.3, deck123 **71.0 KB** — **band 45-71 KB, median 64.1 KB** (wave 54: 40-70, median 59). Net: **deck123 −113** · deck130 +2,463 · deck146 +3,352 · deck126 +3,734 · deck152 +3,893 · deck125 **+5,665 (+9.3%)** · deck162 **+3,986 (+9.6%)**. **deck125 has now grown 9.3% in each of two consecutive waves** (+5,215 then +5,665) — the exact failure wave-54 B10 named — and it is also the deck carrying B1's dead quotation and a 6.6% HOLD rate on the pool's largest denominator. **The counterexample is deck123**: the only guide that shrank, and the only one whose HOLD rung converted. | wave-56 brief: **every seat states its net change WITH the trims that paid for it**, and a guide whose predictions all passed states what it deleted (#220). deck125's wave-56 file states a net change **<= 0**. Synthesis publishes the band and the pool's net change each wave. |

**Not findings — checked so nobody re-files them.** deck123's `WHEN EVERY CAST ROW ON THE MENU IS
PRICED DEAD, ENTRY 8 IS THE PLAY, AND IT IS NOT INACTION` paragraph (correctly a permission, it caps
no choice, and its seat's dead casts went 5 -> 2 of 108 all-dead menus — the byte donor #246
recommends lifting into the guides that lack it); deck146's absence block (B5); deck162's `{feeds:}`
brake with its new release condition (#253's origin, and its control is written); deck152's
plan-staleness rule for BLOCK tags (the half that existed, with #250 filling the attackers side);
deck126's entry-1 tiebreak and Rule #2 exclusion edits (#241/#234 exactly). Card facts spot-checked
against the primitives: Sphinx's Revelation `mtg.txt:110851` (`auto=life:X && draw:X`,
`mana={X}{W}{U}{U}`), Pyrite Spellbomb `90168`, Staff of Nin `112425`, Siege-Gang Commander `105071`,
Sanguine Bond `99004`, Exquisite Blood `37966`, Rorix Bladewing `96836` (`type=Legendary Creature`),
Intruder Alarm `58850` (`lord(creature) doesnotuntap`), Starstorm `112835`, Lay Waste `66043`,
Fog Bank `42416`, Talisman of Impulse `117529`, Soul Shatter `borderline.txt:106069`, Emeria's Call
`borderline.txt:33968` and Agadeem's Awakening `:1177` (both `compare(isflipped)~equalto~1
... forcetype(land)`), Silverquill Silencer `borderline.txt:102764` — **no primitive/Oracle divergence
found by this step**, and no seat needed Scryfall except deck125's single Peer into the Abyss check,
which agreed.

---

## Specimen recommendation for the owner's per-wave prompt review (invariant 0)

**SHIP `wave55/lategame-specimen.txt` = `125v152` seq 69** (turn 42, Main phase 1, **21,279 chars,
6 rows**, 5.3 s latency, **22 life vs 33**, log share 75.8%) — the engine seat's primary, already
staged, nothing to regenerate. It carries a Sphinx's Revelation `{X pricing:` ladder, three distinct
`{leaves N sources ...}` fit forms plus `{taps you out}`, `{right now: ...}` verdicts,
**`[you declined this exact list 10 times already this turn]`**, and the HOLD row last on the menu with
its benefit clause — and the answer is **`Cast nothing right now`**. **It poses the owner's criterion
as one question: 21 KB and every render clause this wave shipped bought a decline at turn 42, on the
tenth identical look at the same list.** It is also the wave's headline defect (ledger D1) on one
screen, and its neighbour seq 71 is the same board one phase later.

**Beside it, relabelled as the log-cost exhibit and nothing else:**
`wave55/lategame-specimen-logcost.txt` = `125v152` seq **190** (turn 60, Main phase 2, **27,249 chars,
4 rows**, log share **82.4%**), answer `Cast nothing right now` — four real cast rows in front of
sixty turns of log. **If the owner wants one file, ship the primary.**

**seat-125-126's `125v152` seq 218 is DECLINED as a substitute and recorded as an alternate**: turn 63,
Upkeep, `priority`, 29,808 chars, 5 rows — 2.5 KB larger than the staged secondary and carrying no
clause the staged pair does not already show, so regenerating buys size and no new question.
**seat-146-152-162's `152v125` seqs 289-316 is DECLINED for invariant 0 and PROMOTED for the lanes**:
28 records over turns 64-65 carrying the corpus's largest prompt (seq 314), its only
`answer_replaced` (seq 313), and eleven consecutive `Cast nothing right now` on a menu carrying the
HOLD row — the best available exhibit for D1 and D11, and invariant 0 asks its question of ONE screen.
**The harvest default (`152v125` seq 309) was correctly replaced by the engine seat**: its 17 rows are
bare mana-source lines and it exhibits nothing this wave shipped.

## The owner-question ruling

**Nothing is owed to the owner this wave.** The standing trigger is *any repeated mulligan against an
explicit floor* (2026-08-26 doctrine). Re-derived: 51 opening asks over 42 seats, **33 seats kept 7,
9 mulliganed once and all 9 then kept 6** — **zero chains, zero double-mulligans, zero mull-to-zero,
no mulligan decided by the heuristic**, and 9 `bottom` records, one per mulligan, all the model's. The
four-corpus colour-family break did NOT recur (lane E's D12 scope tail rendered 3 times and all three
decisions were correct). The one recorded oddity, `162v126` s1 shipping a THREE-land seven under
`(3 lands are not enough for your cheapest spell at mana value 4)`, is a COUNT-family clause, is not a
floor break by that deck's own odds-form rule, is not repeated, and is a deck-content call its
reviewer recorded as the new-baseline rule asks. deck130 shipped its first mulligans in five corpora
(3 of them), lifting the standing #132-UNTESTABLE tag at that seat. **The only OWNER items in the
ledger are the standing ones**: the log window (D35) and the frontend/Arena grouping (D33) — plus the
one thing this step deliberately did NOT decide, recorded in the REJECT list: whether the HOLD row may
be moved off the LAST position when the declined note is high, which contradicts a standing ordering
ruling and is his call, not synthesis's.

---

## What this step did NOT check

- **I did not run wagic, build anything, run git, deploy anything, or edit any file under
  `projects/mtg/bin/Res/**` or `src/**`.** Every mechanism attribution below the corpus level is
  quoted from a seat's or a lane's source READ and is stated as a read, not a debugger trace. In
  particular the ledger's D1 remedy (re-key the hold's re-open predicate to the rendered option rows)
  is a proposal I could not A/B, and D2's "constant 166" is an inference from arithmetic invariance
  across 238 rows — **I did not read the emitter and cannot say whether 166 is an uninitialised value,
  a wrong player object, or a different life pool.**
- **I did not read all 21 `.stderr` files end to end.** I counted 14 strings across all of them and
  read the ~40-line neighbourhood of every `dropping stale async answer` line (43) for D18's
  classification.
- **I did not re-score any deck's guide rules.** Set A is summarised from the seat files' own verdicts;
  the only Set A numbers I re-derived are those a docket item or a boundary finding rests on (the HOLD
  economy per deck, the X-menu census, the discard row census, the ability-row life audit, the
  `[RE-ASK]` audit, the identical-declined runs, the MDFC land-drop search, the attackers header
  coverage, and the guide-size measurements).
- **I did not verify any lane's gate by running it.** Suite and PARSETEST numbers are quoted from the
  lane reports and the carry list. Lanes G/H/I/K/N are recorded UNTESTED — three seats said so
  independently, and I confirmed only that no crash, hang, segfault or new error signature appears in
  the stderr.
- **I did not exercise `WAGIC_GPT_BOARDINDEX=0`, `WAGIC_STALL_FLOOR`, `WAGIC_OPT_SOURCES_CAP` or
  `WAGIC_GPT_DRIFT`.** The segfault (D17) is quoted from lane E, which reproduced it on the archived
  base; no backtrace exists.
- **The contention ruling (D19) rests on an observational control, not an experiment.** I did not
  re-run the corpus at a lower `-j`; the within-game quartile comparison and the cross-game
  correlation are strong and both are inference from one corpus.
- **I did not check any Vita transcript, replay dump, or console artifact**, and ledger D28-D34 are
  carried from the lane reports and the carry list unchanged.
- **I did not diff prompts across polls** (only the final prompt is recorded), so D18's seam
  alternation is characterised from stderr ordering plus lane E's stub-game drift dump, not from two
  live prompt strings side by side.
- **I did not re-derive the corpus's win/loss table beyond a tally of `corpus-results.tsv`**, and no
  verdict in any output rests on a win rate (#87, #167, #217).
- **Every census names its field and unit per #208**: the HOLD/X/discard/kills/`they would be at`
  censuses were taken from rows parsed out of the rendered `prompt`; the declined-note, `[RE-ASK]` and
  clause censuses from the `prompt` text; the fallback, `plan_echo_count`, `chosen_text` and gameend
  counters from the record fields; and the drop/release/404 counts from the `.stderr`.
