# Wave-56 synthesis (2026-09-03) — THIRTEENTH FAIR-HAND CORPUS: the fallback floor halves again, the wave's two biggest lane wins are D2 and D8 — and D8's fix shipped a rules-legality break that only a seat re-derivation found, because the orchestrator's harvest of it was wrong on every number

Inputs read, in order: `~/.claude/skills/wagicgpt/SKILL.md`; `wave55/synthesis-notes.md` (the template
for this step) and `wave55/engine-ledger.md` (the wave-56 docket D1-D35 this corpus adjudicates);
`wave56/review-carry.md` and `wave56/reviewer-brief.md`; the four seat files
(`seat-engine-narration.md`, `seat-123-130.md`, `seat-125-126.md`, `seat-146-152-162.md`); all seven
`deckN/edit-texts.md`, `deckN/skill.md` and `deckN/general-suggestions.md`;
`wave56/lane-{A,B,C,D,E}.md` and `wave56/lane-{Y,Z,X}.md` (today's Vita hotfixes, merged on master
**after** the corpus binary — their fixes are NOT in this corpus); and the corpus itself at
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-120952/` — **42 seat JSONL and 21 `game-*.stderr`,
every load-bearing number re-derived on disk** with python over the JSONL, **counting rows from the
rendered `prompt`** (never from `options_text`), **PREFIX-matching HOLD takes**, **naming the tag
family in every verdict census**, **normalising whitespace before counting a quoted guide literal**,
and `/usr/bin/grep` over the stderr with the emitter's own quoted line. Card facts were checked
against `projects/mtg/bin/Res/sets/primitives/{mtg,borderline}.txt`; the seven DEPLOYED guides were
read for the boundary pass. Nothing was built, run, deployed or committed; **no file under
`projects/mtg/bin/Res/**` or `src/**` was modified; no git command was run; wagic was not run.**

Outputs: `wave56/strategy-writing-skill.md` (the wave-55 edition **byte-verbatim** — the
**1,225,519-byte prefix is `cmp -n 1225519`-verified, no difference** — plus **amendments 267-300**),
`wave56/general-strategy.md` (R1-R258 with a wave-56 verdict, **R259-R292** new and layer-routed,
with the rejections reasoned), `wave56/engine-ledger.md` (**the wave-57 docket D1-D44**, ranked,
engine/UX only, each with a record citation and a falsifiable prediction; both discharge tables;
fallback classes; per-kind prompt chars; specimens), and the boundary findings below.

**Ownership (owner doctrine 2026-08-26).** Reviewers own every `deckN/strategy.txt`; this step owns
the skill and the general strategy; the core loop — engine correctness, LLM UX, bugs — is the
purpose. **No guide file was edited by this step**, and none could have been: the wave-56 guides are
already deployed (`bin/Res/ai/baka/deckN_strategy.txt` verified **byte-identical** to
`wave56/deckN/strategy.txt`, **all seven, by `md5sum`**). Step 4 was run as a REPORT (B1-B10), so
every finding below is an obligation for wave 57, not a request to change a pending edit.

---

## Reconciliation against the records

1. **Health, re-derived over all 42 JSONL.** 2,854 records = 42 `gamestart` + **42 `gameend`** +
   6 `recovery` + 2 `wall_miss` + **2,762 decisions** (ask 2,127 / priority 456 / attackers 88 /
   discard 55 / blockers 25 / reveal 9 / bottom 2). **21/21 natural — invariant 00 satisfied**, and
   **0 `Segmentation` in 42 stderr** (D17's fix; the base binary crashed 2/5 stub games). **6 decision
   fallbacks = 0.22%**, a new floor (wave 55: 0.44%): `named_row_reask` 3, `wall_miss_unrecorded` 2,
   `stale_echo` 1 — **no `plan_choice_conflict` (was 9), no `all_assignments_illegal`, no
   `stale_livelock`, no `unparsed_reply`, no `empty_reply`, no `timeout`**. **Baka executed 0.**
   `recovery` pairing **6/6, 0 unpaired**. Max prompt **26,298** (`125v130` seq 113); **0 over
   30,000**, 6 over 25,000 (was 252). Inference **21.96 h all-latencies / 21.46 h decisions-only =
   27.97 s per decision**. Every harvest figure in the reviewer brief reproduced **except the three
   named in 2, 5 and 12 below.**
2. **THE HARVEST'S D8 NUMBERS ARE WRONG ON EVERY ARM, THE SEATS ARE RIGHT, AND THE DISK AGREES WITH
   THE SEATS.** The brief reported *"21 back-face land rows rendered, 0 takes, 0 arrivals — FAIL on
   arrival"*. Re-derived independently by this step: **65 `PLAY THIS AS A LAND` rows in 62 records,
   6 takes, and 6 arrivals** — `Agadeem, the Undercrypt` (`146v125` t15, `146v152` t9),
   `Emeria, Shattered Skyclave` (`146v162` t9), `Pelakka Caverns` (`146v126`), `Boulderloft Pathway`
   (`152v123`, `152v162`) — every one traced to a `- You played <back face>` line in the next
   prompt's log **and then to later `- Paid …` lines that tap it for mana**. `Flip Side` takes
   **0 of 109** (was 11). **0 turns with two land drops via this route.** Lane D's D8 is a **clean
   PASS on all four arms**. Two causes for the 21: the row census under-counted (the same literal
   also appears in the append-only GAME LOG, and there are two emitter forms — the `->` arrow form
   and the Pathway `[PLAY THIS AS A LAND: …]` bracket form), and the take census **exact-matched a
   `chosen_text` that is tail-less on 112 of 320 HOLD takes** — the wave-55 boundary-pass defect,
   still live, now proven to have corrupted a docket number for the second wave running (ledger D4).
3. **AND THE SAME FIX SHIPPED A RULES-LEGALITY BREAK — 44 OF THE 65 ROWS ARE OFFERED WHERE A LAND
   PLAY IS ILLEGAL, AND ONE RESOLVED ON THE OPPONENT'S TURN.** Classified on disk from each window's
   own `phase` and `Land drop:` line: **21 legal** (`ask` / Main phase 1 / `NOT yet used this turn`)
   against **17 `priority`/Upkeep, 15 `priority`/Blockers, 1 `priority`/Draw, and 11
   `priority`/Main phase 1 under a false `Land drop:` line**. `152v162` **seq 3** is the take:
   `phase: Upkeep`, `It is the opponent's turn.`, `Mana available: 0` — and the next prompt's log
   reads `- You played Boulderloft Pathway` under `=== Turn 2 - opponent's turn ===`, with
   `- You played Plains` on turn 3. **CR 305.1.** The deck146/152/162 seat found the mechanism in
   source and I quote it as its read, not as a trace: `restriction{can play land}` is a **no-op
   string** — `AbilityFactory::parseRestriction` (`src/MTGAbility.cpp:1069-1099`) recognises only
   `myturnonly` / `opponentturnonly` / `assorcery` / `<my|opponent><phase>only` — so the
   `compare(isflipped)~equalto~1` clause lane D removed had been the **only** thing keeping the row
   off illegal windows. My row count is 44 of 65 where the seat's is 41 of 62; **both are true of
   different units** (rows vs windows) and the ledger names each. **Ledger D1, HIGH, skill #296** —
   and lane W is already in flight, so the item is marked ABSORBABLE rather than re-filed.
4. **THE WAVE'S CLEANEST CORRECTNESS WIN IS D2, AND IT DECIDED THREE GAMES.** `they would be at K`
   is right on **145 of 145** ability rows (wave 55: **wrong on 238 of 248**, all reading the garbage
   constant 166 from an `AIAction` player-ability ctor that never wrote `player`). `at 164` / `at 165`
   render **0**. And the fix produced a surface that could not previously exist: **4 rows read
   `they would be at 0; THIS WINS THE GAME`, the model took the row in 3 of 3 games and won each**
   (`125v123` s166 t33, `126v130` s95 t27, `125v130` s115 t61). Under the wave-55 render those same
   windows offered `they would be at 165`. **Wave-55's ledger D2 and R226 are CLOSED.**
5. **THE D18 RESCUE FIRED, AND FOUR READERS FILED FOUR DIFFERENT COUNTS — 0, 3, 4 AND 8. THE DISK
   SAYS 8.** The rescue line is `AIPlayerGPT: consuming an in-flight answer whose prompt text drifted
   (same seam, turn, phase and board)`, and it appears **8 times, in 8 distinct games** (`123v130`,
   `125v126`, `125v162`, `146v123`, `146v125`, `146v126`, `146v152`, `162v130`). The **0** was filed
   as a HIGH observability defect (*"no `slotKey`/rescue/consume line appears in any of the 42
   stderr"*) on the strength of `/usr/bin/grep -h 'slot\|rescue' *.stderr` — **and the line contains
   neither word.** The seat measured its own predicate. The 3 and the 4 are correct counts of the two
   deck seats' own subsets. **Skill #295**, and it is why the task's own framing ("fires 3-4×") is
   also corrected here. **The rescue is not where the volume is**: 65 drops stand, **44 of them
   followed immediately by a `Land drop:` ask** and 21 by a `Casting decision`, with **32 of 65
   preceded by `AIPlayerBaka: Mana cost is NULL.`** — the Baka opponent auto-tapping a land, a real
   board mutation the slot key legitimately rejects while the in-flight answer was for a menu the land
   drop does not touch. **0 drops are phase-only on an unchanged board**, so D18's second prediction
   PASSES exactly while its headline fails at 65 against a target of 20. **Ledger D5: a second async
   slot for the land-drop arm.**
6. **THE HOLD ECONOMY IS THE WAVE'S BIGGEST BEHAVIOURAL MOVE, AND THE FIVE-CELL EXPERIMENT #233 WAS
   WRITTEN FOR NOW HAS ITS SECOND READING.** Corpus **320 takes / 2,016 renders = 15.9%**
   (wave 55: 282 / 2,352 = 12.0%), reproduced to the row. `hold_windows_skipped` **735 → 1,236**,
   non-zero on **30 of 42 seats** (was 21/42). Per deck, overall and at declined-note N ≥ 2:

   | deck | renders | takes | rate | N ≥ 2 windows | takes | rate | wave-55 N ≥ 2 |
   |---|---|---|---|---|---|---|---|
   | deck123 | 233 | 60 | 25.8% | 70 | 19 | **27.1%** | 38.3% |
   | deck125 | 685 | 127 | 18.5% | 235 | 33 | **14.0%** | 6.6% |
   | deck126 | 402 | 11 | **2.7%** | 206 | 5 | 2.4% | 0.6% |
   | deck130 | 328 | 59 | 18.0% | 63 | 16 | **25.4%** | 16.7% |
   | deck146 | 160 | 35 | 21.9% | 20 | 5 | **25.0%** | 0.0% |
   | deck152 | 143 | 24 | 16.8% | 35 | 2 | **5.7%** | 0.0% |
   | deck162 | 65 | 4 | 6.2% | **0** | — | — | 0.0% (of 65) |

   **#233 survives its second test and the effect is large**: the three decks that gained an
   imperative HOLD block this wave went from **0 of 190 combined** to 5/20, 2/35 and 0/0, and the
   pool rate rose from 8.9% to 15.9% while the render text did not change. The single cleanest cell is
   deck126, the **deliberate zero-hold-text control**, whose guide contains the string "Hold priority"
   **zero times** and which sits at 2.7% against deck125's 18.5% from the same reviewer, same corpus,
   same emitter string on 1,087 rows — **a ~7× ratio that held while deck126's decisions rose 51% and
   deck125's fell 14%** (skill #276). **Two cells complicate the monotone story and both are recorded
   rather than smoothed:** deck130 converts at **25.4%** with **no HOLD imperative at all** (its only
   mention, at line 788, is the recognition sentence *"[you declined this exact list N times already
   this turn]" is not new information and is not a reason to cast*, plus the **dead** wave-54
   quotation at :785 — B1); and deck162's N ≥ 2 denominator went **65 → 0**, so the sharpest test of
   #233 in the pool (wave-55 B7) is **UNTESTED, cause (a)**, not passed. Its lane closes on the window
   count instead (skill #292).
7. **THE RUN PREDICTION IS PASS, FAIL AND FAIL DEPENDING ON THE PREDICATE, AND EVERY PREDICATE IS
   DEFENSIBLE.** Lane A predicted identical-declined runs **< 15%, max run < 10**. Measured:
   **14.99%** (wave-53 predicate: key `(turn, options_text)`, runs ≥ 3, a HOLD take breaks the run;
   414 of 2,762, 68 runs, **1.18 h = 5.5%** of inference, target < 6% **PASS**) · **16.8%** (rendered
   rows, runs ≥ 3) · **22.3%** (rendered rows, runs ≥ 2, 151 runs). **The max run is 13 under every
   predicate I tried and FAILS every time.** The harvest's *"14.3%, max 12"* I **could not reproduce**
   under four predicates and it is recorded as irreproducible, not as wrong. The genuine improvement
   is on the side that matters: **opponent-turn 57.8% → 44.1%**, own-turn 2.4% / max 9. **Skill #300**
   and ledger D1(b), recorded **SPLIT** rather than PASS.
8. **A HIGH WAS DISCHARGED AND A HIGHER ONE OPENED UNDERNEATH IT: THE PREGAME LAND COUNT.** This
   corpus produced **exactly two mulligans and one bottom, and all three were made on a false land
   count.** `146v152` s1 reads `counted by the engine: 0 lands, 7 spells` and `Playing every land in
   this hand would not cover any spell in it` on a seven holding **both** `Agadeem's Awakening` and
   `Emeria's Call` — **a two-land seven in this binary** — and the seat mulliganed. `146v130` s1 reads
   `1 land, 6 spells` with `Pelakka Predation`; s2/s3 read `1 land` again with `Agadeem's Awakening`
   and the seat then answered `PUT: 2` — **it bottomed its own second land.** Both games lost. I
   verified the hands independently: of the 8 opening asks whose hand line names an MDFC, the two
   deck146 hands are the only ones the header under-counts (deck152's are Pathways, whose FRONT face
   is already a land and is already counted; deck162 runs none). **Ledger D2, HIGH, skill #297** — and
   note what makes it HIGH rather than MED: **a pregame prompt has no later window to correct it.**
9. **THE DISCARD ITEM SURVIVED MY OWN INSTRUMENT FAILURE.** My first census of the 426 discard rows
   used the predicate *"the row contains a `{`"* and returned **426 annotated, 0 bare** — which would
   have discharged a standing HIGH on the spot. The 125/126 seat reported **229 of 282 bare** at its
   own seats. Re-derived with the tag family enumerated (`{dead right now:`, `{spare:`, `{right now:`,
   `{visible now:`, `[second copy:`, `{MV`, `{removes:`, `[legendary:`): **357 of 426 rows (83.8%)
   carry no verdict**, because every row carries `{card text: …}` and a brace-delimited mana cost and
   neither is a claim. Per deck: deck125 220/272, deck123 100/115, deck146 28/29. **`{MV` renders 0
   times.** The seat was right, my predicate was not, and it is written up as **skill #298** because
   the failure was synthesis's.
10. **THREE SHIPPED RECOVERY ARMS ARE NOW TWO WAVES UN-EXECUTED, AND ONE OF THEM WAS REWRITTEN THIS
    WAVE AGAINST ZERO LIVE EVIDENCE.** D3's `[RE-ASK]` CHOICE-line form **never rendered** (all 3
    notices are the "not on this list" form, so 0/0 mismatches is vacuous); `all_assignments_illegal`
    and its `_exhausted` branch **never fired** (all 25 `blockers` records carry
    `dropped_assignments: 0`); `plan_choice_conflict_recovered` is **0**. Lane C nonetheless rewrote
    the first of the three. **Ledger D17**, and the ask is a forcing flag or a fixture per arm.
11. **THREE MECHANISMS EARNED A PASS THEY DID NOT DO THE WORK FOR, AND ALL THREE ARE RE-ATTRIBUTED.**
    (a) **D11**: plan echo max **20** against a target of 60 — but there were **0 records with a plan
    older than 40 windows** and **0 where the menu-diff note (473 renders) fired on a plan more than
    4 windows old**, and the withdrawal sentence renders **0**. The improvement is shorter games.
    (b) **D14**: equipment casts at 0 creatures fell 33% → 1.6% — but the render told the truth on
    **62 of 62 rows** and the residual take (`123v125` s45) is a guide break, so the credit is the
    render's and the residual is deck123's. (c) **D16**: `plan_choice_conflict` **0** (was 9) is a
    real PASS, but the broad census `decision_reversed_in_prose` that lane A promised would sit beside
    the narrowed stamp renders **0** (was 14) — so *"0 conflicts"* and *"0 counted"* are now
    indistinguishable. **Skill #283** (an UNTESTED names its cause) is what forces all three, and
    ledger D18/D16 carry them.
12. **THREE MORE DENOMINATOR DISAGREEMENTS, NONE OF THEM AN ERROR (#208 again).** (a) **D6**: the
    harvest read "38 of 40 opponent-combat windows", the engine seat 29/29 and 38/38 on two narrower
    predicates, and I read **25 of 25 `blockers`-kind windows** and **25 of 260 combat-phase windows
    of all kinds (9.6%)**. All four describe one emitter gated on the blockers ask; the number that
    matters for the docket is the last one, because **deck125 is 0 of 20** and is creature-less by
    design. (b) **D10**: `Their untapped sources:` is on **2,716 of 2,716** prompts carrying a
    `CURRENT SITUATION` block = **98.3% of all 2,762 decisions**; the 46 misses are 44 pregame asks
    and 2 `bottom`, which are HAND-ONLY by owner directive and must not carry a board line. Both
    figures are 100% of their own unit. (c) **Narration share**: **82.7% of the mean prompt sits above
    the option rows** and **54.0% is the GAME LOG above `--- CURRENT SITUATION ---`**; deck125's seat
    reports **63.2%** on its own 1,288 board-bearing prompts, and the specimen splits at **86%**. All
    three are true of different boundaries and the ledger prints the boundary with the number.

---

## The headline: the fallback floor halved again and two lanes landed cleanly — and the wave's three worst findings are a fix that broke a rule, a pregame count that decided every mulligan in the corpus, and four readers reporting four different counts of the same log line

**(a) THE INSTRUMENT IS CLEAN AND THE CORPUS IS A TEST.** 21/21 natural for a third wave, **0 SEGV**
where the base binary crashed 2/5 stub games, fallbacks **0.22%** (a new floor), `pass_hold_ambiguous`
0, `all_assignments_illegal` 0, `stale_livelock` 0, **Baka executed 0 decisions**, 6 `recovery` for 6
`choice: -1`, 0 prompts over 30 K. Of the wave-56 docket, **D2, D6, D8, D9, D10, D12, D16, D17 and
the D1 latch are all PASS**; D4, D5, D11, D14, D18 are SPLIT or re-attributed; D3, D7(c), D13(at some
seats), D15 and D26 are UNTESTED with a named cause.

**(b) THE TWO BIG LANES BOTH LANDED, AND ONE OF THEM CHANGED THREE GAMES BY ITSELF.** D2's root-cause
fix took a clause that was wrong on 238 of 248 rows to **145 of 145 right**, and the first lethal face
row this engine has ever printed was taken in **3 of 3 games and won each**. D8 put a modal-DFC land
face on the menu for the first time — **65 rows, 6 takes, 6 arrivals** — retired a three-wave guide
block **with its eight downstream citations in a single pass**, and produced the corpus's best colour
fix at `146v125` s13.

**(c) AND D8's FIX BROKE A RULE OF MAGIC.** Removing the `isflipped` gate removed the only thing
keeping the land row off illegal windows, because the clause underneath it —
`restriction{can play land}` — parses to nothing. **44 of the 65 rows are offered where a land play is
not legal**, and one **resolved on the opponent's Upkeep** with a second drop the following turn.
Nothing in lane D's gate caught it: its negative fixture asserts that a CLICK finds nothing in upkeep,
not that the ROW is absent, and the corpus contains a click that worked. **A lane that removes a gate
must ship the legality test the gate was accidentally providing (#296)** — and the sibling defects fell
out of the same change: the pregame land count (D2) and a `Land drop:` summary line that contradicts a
row three lines below it (D3), on **18 rows, 4 of the 6 takes**.

**(d) THE PREGAME SURFACE DECIDED EVERY MULLIGAN IN THE CORPUS AND IT WAS WRONG EVERY TIME.** Two
mulligans, one bottom, three false land counts, two lost games. A seven holding Agadeem's Awakening and
Emeria's Call was described to the model as **"0 lands, 7 spells … this hand holds no lands at all"**,
and the seat that shipped it played the same cards as lands four times later in the same corpus. This
is the first item in six waves that is ranked correctness purely because of **where** it renders.

**(e) AND THE WAVE'S METHOD FAILURE IS THAT FOUR READERS COUNTED THE SAME LINE FOUR WAYS.** The D18
rescue was filed as 0, 3, 4 and 8 firings; the 0 was a HIGH-ranked defect report built on a grep for
two words the line does not contain. This step's own discard census read 0 bare where the truth is 357.
Both are the same failure at different desks: **a predicate that was guessed rather than quoted**
(#295, #298). Three of this step's six own amendments are instrument rules, and one of them exists
because synthesis got it wrong first.

---

## What merged into the skill (267-300)

**22 seat proposals, 22 ACCEPTED, 0 REJECTED** — the first clean sweep this loop has had, and it is a
datum rather than a courtesy: every proposal this wave arrived with a **window count on its own
literal**, which is exactly what the amendment it produced (#292) now requires of all of them.
Three were amended on acceptance and two were merged.

**Convergence merge:** **288** a prediction ships with "a hit is a lane break only if <condition>", and
where every hit is a correct play the verdict is RULE OVER-BRAKES and the edit is a **carve-out**
(deck146 S4 + deck162 S9; three rules hit their own keys 8 times between them this corpus and **all 8
were correct plays**).

**Decisive single-seat merges:** **267** the board-presence floor measurement and "the ceiling is the
deck" as a terminal verdict (deck123 S1; 2 creatures in 194 windows across five losses) · **268** the
literal audit's fourth column (deck123 S2) · **269** the untagged branch of a conditional annotation
(deck123 S3) · **270** a trim is a claim and ships a prediction (deck123 S4) · **271** a combat
ordering rule needs its arithmetic gate (deck123 S5) · **272** a ceiling claim names its board class
(deck125 P1; 78 of 78 that lost a game) · **273** three corpora of breaks under growing prose means
file the RENDER item and cap the section (deck125 P2 — *a rule enforced by a number on the row holds;
a rule enforced by a paragraph elsewhere does not*) · **274** state the answer in RANGE terms too
(deck125 P3) · **275** the unruled-cluster scan (deck125 P4) · **277** an absolute's exclusion list is
enumerated, drawn from every other stop, and CLOSED (deck126 P7; six bad spends in one game) ·
**278** a per-row verdict describes the board, and the stack is not the board (deck126 P8) · **279**
a new lane literal that prices this deck's win or death condition is quoted THIS wave (deck126 P9) ·
**280** every hold-the-resource rule names its release in a printed number, and its breaks are
reported in two columns (deck130 S6; the split is 0 and 5) · **281** a one-way latch is a smell
(deck130 S7; one Elixir activation governed thirty turns) · **282** the PLAN line is written after the
CHOICE line and names only objects on this window's screen — **one general rule replacing three
per-deck copies** (deck130 S8) · **283** an UNTESTED verdict names which of three causes it is
(deck130 S9; it prevented a D8 mis-adjudication in this very wave) · **284** retire the derivation,
keep the pointer (deck130 S10) · **285** a rule that exists because of an engine defect ships its own
retirement machinery, including a `REMOVE THIS RULE WHEN …` line inside the guide (deck146 S1) ·
**286** a fact about a card class is stated once in a named block (deck146 S2; nine citation sites) ·
**289** the zero-take audit of every literal the guide names (deck152 S5; `Flip Side` 109 renders / 0
takes) · **290** an anti-anchor clause names the LOSING row (deck152 S6; 32 of 33 misses answered one
specific row) · **292** the admissible evidence is a window count, never a win rate (deck162 S8) ·
**293** a class-membership rule names one near-miss (deck162 S10; Forced Fruition).

**Accepted WITH an amendment this step adds:** **276** the taught-vs-control RATIO (deck126 P5;
~7×, held across a 51%/−14% swing in decisions) **plus a three-corpus SUNSET** — wave 57 is deck126's
last as the control and the block ships in wave 58 whatever the ratio reads (see #299) · **287**
"add one to the rendered number" is inside the trust doctrine (deck146 S3) **plus the debt clause** —
a route-around is written under #285's retirement machinery and the render fix (ledger D2) deletes it ·
**291** a `WHAT THIS COST` entry carries its corpus and is deleted after two passing corpora
(deck152 S7) **with the ruling the seat asked for**: a lane with **no windows** is UNTESTED, not
passed, so those two entries correctly stayed.

**This step's own, forced by the reconciliations:** **294** normalise whitespace before counting a
quoted literal (reconciliation B1 below — a dead string survived three waves of greps because the
guide wraps it across a newline) · **295** when readers disagree on a log-line count, the disk settles
it and the substring-guess predicate is the wrong one (reconciliation 5; 0 / 3 / 4 / 8) · **296** a
lane that removes a gate ships the legality test the gate was accidentally providing (reconciliation 3)
· **297** a pregame surface is load-bearing, so a count on it is a correctness item (reconciliation 8;
2 of 2 mulligans and the only bottom) · **298** a verdict-tag census names the tag family
(reconciliation 9; my own 0-bare against a true 357) · **299** a deliberately withheld affordance runs
at most three corpora · **300** a target met on one denominator and missed on another is recorded
SPLIT, with the denominator inside the verdict cell (reconciliation 7).

## What was REJECTED, and why

- **"Skip the opponent-turn window whose `ON THE STACK` is empty at a phase this seat has never cast
  in"** (deck125 G4, deck126 G8, seat-125-126 MED #4 — **FIFTH wave running**, on the strongest
  denominator it has ever had: deck125 **210 of 302**, deck126 **226 of 230**, and
  `mana_only_windows_skipped` at 1,482 as a working precedent in the same binary). **REJECTED again.**
  The denominator objection has been gone for two waves; the doctrine objection has not — **it removes
  a legal window from the model.** The sanctioned levers are R270 (tell the pilot the menu is dead
  instead of hiding it) and the HOLD row, which is now at **22.9% on the opponent-turn arm** and which
  the model takes *itself*. The distance from 22.9% to 100% is a guide problem (#233/#276), not a
  licence.
- **"Stop asking the model at all when the land-drop menu's only non-`Play` row is `Play no land right
  now`"** (seat-125-126 MED #5, second arm). **REJECTED on sight** — a hard cap on a legal choice, and
  the seat's own `76 of 76` is an argument that the model is doing it right. The **first** arm of the
  same item (a second async slot) is adopted in full as ledger D5.
- **"Hoist the HOLD row above `Cast nothing right now` when the declined note is high."** **NOT
  ADOPTED, and recorded so it is not re-filed as new.** deck152's evidence is the best yet — 32 of its
  33 N ≥ 2 misses answered that one row, two positions above HOLD — but decline-ordering is a
  **standing owner ruling** and reversing it to exploit the same position bias is the owner's call.
  The part needing no ordering change is adopted guide-side as skill **#290**.
- **A guide compensation for ledger D1, D2 or D3.** **REJECTED, and deck152's reviewer reached the
  same conclusion unprompted and wrote it down**: teaching the pilot to exploit an illegal-but-working
  land drop is a rule that dies with the fix. Trust doctrine unchanged — **no guide sentence may be
  written against a defective rendered surface**; deck146's `ADD ONE FOR EACH …` arithmetic is the one
  sanctioned exception, and only because both its inputs are true surfaces (#287) and it carries its
  own retirement trigger (#285).
- **The deck146/152/162 seat's "the D18 rescue NEVER FIRED" as a HIGH observability defect.**
  **OVERTURNED on disk** — 8 firings (#295). The seat's *other* ask inside the same item (emit the
  rescue's decision — fired / seam-changed / no matching slot — once per drop) is **adopted**, because
  it is exactly what would have prevented the disagreement.
- **The engine seat's "D8 PASS on all three arms" as a complete verdict.** **NOT overturned but
  RE-SCOPED**: all four arms do pass, and the same change opened ledger D1/D2/D3, which the engine seat
  did not have the seats to see. Neither reading is wrong; the deck seat's is the one with the windows.
- **Any win-rate attribution.** deck152 5-1, deck162 5-1, deck126 3-3, deck146 3-3, deck130 2-4,
  deck125 2-4, deck123 1-5; n = 6 per deck on real hands, in the corpus that is **also the O8 RNG-split
  shakedown**, so draw order is not comparable to wave 55 either. Counts only (#87, #167, #217, #292).
  All seven seats are correctly recorded **NO EXIT**.

---

## Layer routing (`general-strategy.md`)

**ENGINE, HIGH**: the MDFC land-row legality gate (R259) · the pregame MDFC land count (R260) · the
false `Land drop:` summary line (R261) · `chosen_text` fidelity (R262) · the shared async slot (R263).
**RENDER, HIGH**: `INCOMING THIS COMBAT` gated on the blockers ask (R264) · the opponent-converter
cast-row price (R265) · the discard verdict family (R266). **MED**: the X mana-fit clause (R267) ·
D13's nesting residual (R268) · the stack-aware `{right now:}` (R269) · the menu-level dead verdict
(R270) · the narration budget by ask kind (R271) · the `Flip Side` toggle (R272) · the opponent LIFE
TREND (R273) · the fetch row's positive colour form (R274) · the 3-life follow-up (R275) · the
assignable remainder (R276) · the marker promoted off X menus (R277) · cycling row pricing (R278) ·
the blocking-trigger total (R279) · `{feeds:}` in hand (R280) · the vanished broad census (R281) ·
D11's unfired rules (R282) · `[NAMED BY THEIR …]` per-cast vs one-off (R283). **LOW**: the lowercased
script token (R284) · adjacent duplicate log lines (R285) · the display-toggle census (R286) ·
`hold_windows_skipped`'s stderr companion (R287) · two cards, one verdict, two prices (R288).
**PROCESS/MEASUREMENT**: whitespace-normalised literal audits (R289) · quote the log line, never
paraphrase it (R290) · print the denominator inside the verdict (R291) · name the tag family (R292).
**PASS/KEEP** (recorded so no lane trims them): the corrected `they would be at K` tail and its
`THIS WINS THE GAME` flag · `Their untapped sources:` · `INCOMING THIS COMBAT` on the blockers ask ·
the menace header (3/3 and **0 false positives**) · the X marker family and its largest-first header ·
`reveal_wait_ticks/_secs` · the HOLD row and its tail · `{blocking trigger, …}` and `{after this
combat, …}` · **`(their life LOOP is in play: …)`** — 5 renders, six correct all-out-attack refusals,
and deck152 asks it be extended off combat rows to the board line · the `PLAY THIS AS A LAND` row
itself · `[NAMED BY THEIR …]` (now traced end to end) · the declined-note literal · `{leaves N sources
- no other row on this menu needs more than N}` · deck162's `DRAW FORECAST` + `DRAW PUNISHERS` pair ·
`mana_only_windows_skipped`.
**Carried engine items with no corpus surface**: the 30 spell back-face gates (D33) · the per-seat
target-chooser owner, lane Z + lane Y residual (D34) · the Runed Crown / Mantle of the Ancients
dormancy class, lane X residual (D35) · the third replay defect (D36) · lane M's cache counters (D37) ·
the phase-7 stub livelock (D38) · `kMaxOptSources` / the `&&`-wrapped producer / Baka's blindness to
the new row (D39) · the audit lanes' unobservability (D41) · Arena grouping, OWNER (D42) · the log
window, OWNER (D43) · Vita vpk15/vpk16 (D44).

---

## Guide boundary findings for the deck reviewers (B1-B10)

Step 4 was run as a **REPORT** over the seven DEPLOYED `deckN_strategy.txt`, **verified byte-identical
to `wave56/deckN/strategy.txt`, all seven, by `md5sum`** — so every finding is an obligation for wave
57, not a request to change a pending edit. Sweeps run: the REVERSE audit against every literal lanes
A-E changed; the mood/hedge sweep for the trust doctrine; the absence-assertion sweep (#238's
mechanical check); the retired-derivation sweep against wave-55's B1-B9 obligations; **and, new this
wave, a whitespace-normalised dead-string sweep (#294)**, which is the only sweep that found B1. Every
literal count below is a **COUNT PER FILE**, not a first hit (the wave-55 obligation).

**TRUST DOCTRINE: CLEAN, all seven.** The hedge sweep (`may be wrong`, `do not trust`, `the number
may`, `is sometimes wrong`, `engine sometimes`, `the render lies`, `cannot be trusted`, `may be
false`, `is not reliable`) returns **zero hits across all seven pool guides**. No guide teaches doubt
of a rendered value. **PASS, fourth wave.**

**#233 FORBIDDEN-PHRASE SWEEP: CLEAN, all seven.** `both are correct` / `either is correct` /
`equally correct` / `both are fine` / `is also correct`: **0 hits in the pool** (the single corpus-wide
hit, `deck59_strategy.txt:50`, is out of pool). Wave-55 B1's headline defect is **eradicated**.

**WAVE-55 OBLIGATIONS: five of six honoured.** B1 (deck125's dead quotation and its "both are
correct") **honoured in full** — B7 below. B3 (three guides quoting the mulligan colour clause without
lane E's scope tail) **honoured** — the full emitted string with `even with every land in this hand in
play` now appears in the deck146 and deck152 pregame prompts and renders correctly. B4 (deck130's
"three of three" X recital) **cut**. B5 (deck146's absence block) **retired exactly as P10 required**
— B6 below. B8/B9 (stated sizes, net change) — B10 below. **The one NOT honoured is wave-55's
standing obligation 2: deck130's second dead HOLD quotation — B1.**

| # | file : site | amendment | finding | replacement string |
|---|---|---|---|---|
| **B1** | `deck130_strategy.txt` **lines 785-786** | **#294 / #209 / #233** | **THE WAVE-54 DEAD HOLD QUOTATION IS STILL LIVE, THIRD WAVE RUNNING, AND IT SURVIVED BECAUSE THE GREP CANNOT SEE IT.** The guide quotes `"Pass priority, and do not ask me again this turn unless the board changes (any change re-opens this window; you give up no cast)"` — **the wave-54 string, which renders 0 times in this corpus** (verified: 0 occurrences across all 42 JSONL). The live string renders **328 times at this deck's own seats** and the guide quotes it **0 times**; the `{taking this row skips …}` tail likewise **0**. Wave-55's boundary pass named this site (`:779`); the wave-56 reviewer did not fix it, **and every single-line `/usr/bin/grep` since has reported the file clean, because the quotation wraps across a newline between `"this"` and `"turn"`.** This is #294's origin. **Two honest halves must be stated with it:** the paragraph's imperative is otherwise strong (*"take THAT row, at the FIRST such ask of the turn, not after four declines"*), and this deck converted at **25.4% at N ≥ 2** — the pool's second-best — on a dead quotation, which is #264 confirmed a second time (**the pilot follows the description, not the quotation**). It is an AUDIT debt, and it is a three-wave-old one. | Replace the quoted string, verbatim from the emitter: `"Hold priority for the rest of this turn: pass now, and do not ask me again unless the board changes (any change re-opens this window; you give up no cast) {taking this row skips the rest of this turn's identical windows}"`. Keep the imperative and the `at the FIRST such ask of the turn` clause exactly as they stand. Add the own-turn scope, which the wave-54 string wrongly excluded: the row is on **own-turn menus too** (own-turn renders 1,273 corpus-wide), so the sentence must not open `"It is the OPPONENT'S turn and …"`. |
| **B2** | `deck126_strategy.txt` — **the whole file** | **#233 / #276 / #299** | **THE ZERO-HOLD-TEXT CONTROL IS IN ITS SECOND WAVE AND IT IS WORKING AS AN EXPERIMENT.** `"Hold priority"` appears **0 times**; `"declined this exact list"` **0 times**. The deck answered **402 HOLD-bearing windows and took the row 11 times (2.7%)** against deck125's 18.5% from the same reviewer, same corpus — **ratio ~7×, held across a 51% rise in this deck's decisions and a 14% fall in deck125's**. It is the cleanest evidence this loop has produced that an imperative moves a rate, and its price is measurable and small (its wasted windows are absorbed by `mana_only_windows_skipped`, 1,482; no game lost to a declined window this corpus). **The reviewer declared it a control in writing, which is what makes it legitimate.** | **none now — and a SUNSET (#299).** Wave 57 is this control's **third and last** corpus. The wave-57 seat file reports the ratio against **18.5% / 2.7%**, and the HOLD block ships in wave 58 **whatever the ratio reads**. A control that runs indefinitely has stopped being an experiment. |
| **B3** | `deck123_strategy.txt` **lines 138-139** | **#238 / #250 / #268** | **AN ABSOLUTE ABOUT THE RENDER, WRITTEN IN THE WAVE THE RENDER STOPPED SUPPORTING IT.** The guide asserts **"THE ROW IS NOW KEPT."** followed by *"the byte-identical list came back inside the same turn 8 times … the hold holds."* Lane A's fix is real and large (14 of 320 corpus-wide, 4.4%, against 113 of 282), but **it is not zero**, and this deck's own rung is the one that tells the pilot to *"take it at the first dead window of the turn and stop reading the menu"*. The paragraph then contradicts its own absolute two sentences later (*"When a run does still come back …"*), which is the tell. The counts themselves are legitimate EVIDENCE citations under #238 — **the defect is the flat assertion in front of them**, and #268's fourth column is what should have caught it. | `THE ROW IS NOW KEPT ALMOST ALWAYS. Last corpus this deck took the hold row 119 times and the byte-identical list came back inside the same turn 8 times; corpus-wide the figure is 14 of 320 takes. Take it at the first dead window of the turn and stop reading the menu. When a run does still come back — a drain moves a life number and the price on a row moves with it — answer the SAME way every time.` Keep the `- and that is not your problem to solve` register (#265), which is the half that works. |
| **B4** | `deck146_strategy.txt` **line 616**, `deck152_strategy.txt` **line 709** | **#291 / #238** | **AN UNDATED `WHAT THIS COST` RECEIPT THAT THIS CORPUS FALSIFIED, IN TWO FILES.** Both guides carry *"190 windows across three of this pool's decks carried that declined line with the HOLD row on the menu, and not one of them took it"*. That was true of wave 55. **It is now false of the decks it describes**: deck146 took 5 of 20 (25.0%), deck152 2 of 35 (5.7%), deck162 had 0 such windows. The sentence is a cautionary tale for a lane that has since moved, and it is exactly the shape deck152's own S7 proposal (adopted as **#291**) says must carry its corpus. Note the correct model in the same file: deck152 line 703 reads *"WHAT THIS COST, **this corpus**: 35 windows at this deck's seats carried the N line with N of 2 or more, and 33 of them were answered 'Cast nothing right now'"* — dated, current, falsifiable. | Date it and re-measure, in both files: `WHAT THIS COST, wave 55: 190 windows across three of this pool's decks carried that declined line with the HOLD row on the menu and not one of them took it. WHAT IT IS NOW, wave 56: this deck took it <N of M> — the rule is working and the number is the measurement, not the story.` **#291's deletion clause does not apply yet**: the lane has had windows and has not passed twice. |
| **B5** | `deck152_strategy.txt` **line 467** | **#238 / #289** | **A COUNT ASSERTED AS A STANDING FACT, WITH NO RE-VERIFICATION GREP AND NO RETIREMENT TRIGGER — AND THE RENDER ITEM THAT WOULD RETIRE IT IS ALREADY IN THE DOCKET.** The guide says *"across the corpus the Flip Side row was offered 109 times and taken zero times, which is right."* The count is exactly right (109 rows in 90 windows, 0 takes — I reproduced it), and the rule around it is correct. But #238 requires an absence-shaped claim to ship with its re-verification method and its retirement condition, and **ledger D14 asks that the `Flip Side` row be suppressed from the AI seat's option set entirely** — at which point the sentence describes a row that no longer renders. This is also the file's own S5 (adopted as **#289**) applied to itself: a guide-named row at 0 takes needs an edit, and here the edit is a deletion trigger. | Append the trigger inside the guide text (#285(c)): `REMOVE THIS PARAGRAPH WHEN the "Flip Side" row stops rendering on a land menu — re-verify with a count of "Flip Side" option rows in the corpus; it was 109 rows / 0 takes in wave 56, and the docket asks for the row to be suppressed.` The imperative (`NEVER TAKE "Flip Side" ON A LAND MENU`) stays as written. |
| **B6** | `deck146_strategy.txt` **lines 126-139** | **#285 / #286 / #287** | **PASS, AND IT IS THE POOL'S MODEL — THE THREE-WAVE BLOCK RETIRED ITSELF ON SCHEDULE AND THE REPLACEMENT IS A LEGAL ROUTE-AROUND.** `NOT LANDS FOR YOU` renders **0 times** in the deployed file; the replacement (*"THREE OF YOUR SPELLS ARE ALSO LANDS, AND THE HAND HEADER DOES NOT COUNT THEM … READ THE HEADER'S LAND NUMBER, THEN ADD ONE FOR EACH of those three names on the hand line"*) uses only true surfaces as inputs and carries its own WHAT THIS COST with both losses cited by seq. **Nine citation sites were re-keyed in one pass** because wave-55's P10 was written as a detector (#285). **One sentence to fix, and it is small:** *"the game now offers that face **on your own main phase**"* is contradicted by the render — 44 of 65 rows are offered elsewhere (ledger D1). The seat's choice to teach the rules-correct line rather than the engine's current behaviour is **correct and is recorded as correct**; the sentence just needs to describe the guide's rule rather than the engine's menu. | `… and the game now offers that face as a row reading "<land name> with <spell name> -> PLAY THIS AS A LAND". Take it ONLY on your own main phase with your land drop unused — that is when playing a land is legal, and it is the only window where taking it is a real land drop.` Then, per **#285(c)**: `REMOVE THE "ADD ONE" ARITHMETIC WHEN the pregame header prints a land-on-the-back split of its own.` |
| **B7** | `deck125_strategy.txt` **lines 577-583** | **#209 / #233 / #264 / #274** | **PASS, AND IT IS THE WAVE'S BEST GUIDE RESULT: wave-55 B1 IS HONOURED IN FULL AND THE RATE MORE THAN DOUBLED.** The deck now quotes the **live** string verbatim including the `{taking this row skips …}` tail (1 occurrence, whitespace-normalised), and *"both are correct"* is gone, replaced by exactly the ordered form B1 asked for: **"THAT ROW IS THE ANSWER AND 'Cast nothing right now' IS THE WRONG ONE - take it at the FIRST such window of the turn, not the fifth."** Result on the pool's largest denominator: **6.6% → 14.0% at N ≥ 2**, 127 takes of 685 renders overall. The same file also shipped the collapsed-range decode (`count into the range - option 2 is X = 5 …`, lines 471-475) before **#274** existed to require it, and its X = 3 rung went 3 of 3 including the collapsed menu. | none. Wave 57 measures whether 14.0% holds and reports it against **6.6% → 14.0%**. Note for the seat: the guide's `"Four hundred and seventy-nine of them in six games last corpus"` (line 572) is a **wave-55** count in a present-tense sentence — date it per #291 or drop the number, since the rule needs no count in front of it. |
| **B8** | `deck146` **line 610** vs `deck152` **lines 696-700** | **#290** | **THE POOL NOW CONTAINS A CONTROLLED TEST OF THE AMENDMENT IT PRODUCED, AND IT IS WORTH NAMING BEFORE WAVE 57 READS IT.** deck152's wave-56 block adds a full anti-anchor paragraph — *"'Cast nothing right now' IS THE ANSWER FOR A LIST YOU HAVE NOT DECLINED YET. Once that N line is on the screen, that row is the wrong one … Do not let the HOLD row's position at the bottom of the list decide this"* — written from the finding that **32 of its 33 misses answered that row**. deck146's block, which produced the pool's **best** N ≥ 2 rate (25.0%), carries only the short form (*"answer the HOLD row, not 'Cast nothing right now'"*). So wave 57 reads a graded pair: short demotion at 25.0%, full demotion at 5.7% → ?. **If deck152 does not move, #290 is falsified and the residual is position, not wording — which returns the item to the owner's decline-ordering ruling.** | none now. Wave-57 seat file reports deck152's N ≥ 2 rate against **2 of 35 (5.7%)** and deck146's against **5 of 20 (25.0%)**, and states which block shape each deck carries. |
| **B9** | `deck130_strategy.txt` **line 788** | **#233 / #290** | **RECOGNITION TEXT WHERE THE FILE'S OWN NEIGHBOUR SENTENCE IS AN IMPERATIVE.** The line reads *"'[you declined this exact list N times already this turn]' is not new information and is not a reason to cast."* — true, useful, and **not an order**. It sits directly under the (dead-quoted) HOLD imperative at :785. This deck converted at **25.4% at N ≥ 2**, so nothing here is failing; the finding is that the file's HOLD instruction is **one dead quotation and one recognition sentence**, and B1's replacement is what carries it. Recorded so B1's fix is not read as cosmetic. | Keep the sentence — it is the correct #265 register — and let B1 supply the live literal above it. Optionally add the demotion half tested in B8: `and "Cast nothing right now" is the wrong row once that line is on the screen.` |
| **B10** | all seven `deckN_strategy.txt` — **SIZE and NET CHANGE** | **#232 / #220 / #270** | **THE POOL GREW +14,183 BYTES (+3.2%) — DOWN FROM +5.5% AND +5.5% IN THE TWO PRECEDING WAVES, ON A BIGGER BASE.** Measured: deck162 **45.5**, deck126 63.9, deck152 63.7, deck146 65.8, deck130 66.3, deck125 68.1, deck123 **69.2 KB** — **band 45.5-69.2 KB, median 65.8 KB** (wave 55: 45-71, median 64.1). Net: **deck123 −189** · deck130 +537 · deck152 +1,396 · deck162 +1,242 · **deck125 +2,906 (+4.3%)** · deck146 +3,214 (+5.0%) · **deck126 +5,077 (+8.4%)**. **deck125's wave-55 obligation was a net change ≤ 0 and it is VIOLATED** — but its growth rate has halved from +9.3% in each of two consecutive waves to +4.3%, and this is the wave its HOLD rung more than doubled its rate, so the trend is the right one and the obligation is **restated, not escalated**. **deck123 is the counterexample and now the only guide that has ever shrunk twice** (−113 then −189), in the wave its seat filed the terminal "the ceiling is the deck" verdict (#267) — and it shipped that trim **with a prediction attached**, which is #270 written from this very file. **deck126 is now the pool's fastest grower and the deck with zero HOLD text**, which is worth saying in one sentence beside B2's ratio. | wave-57 brief: **every seat states its net change WITH the trims that paid for it, and every trim states its own falsifiable prediction (#270).** deck125's wave-57 file states a net change **≤ 0** — second wave of the obligation. deck126's states whether its growth bought anything measurable. Synthesis publishes the band and the pool's net change each wave. |

**Not findings — checked so nobody re-files them.** deck146's numbered mulligan SCAN and its bottom
rung (P2 passed 0/4 + 0/4 even though its key is obsolete) · deck162's `{feeds:}` brake with its
release and its new Forced Fruition near-miss (#293's origin, and the break is cited by seq) ·
deck123's `WHEN EVERY CAST ROW ON THE MENU IS PRICED DEAD, ENTRY 8 IS THE PLAY` paragraph (a
permission, caps no choice; 85 all-dead menus and **one** cast off them) · deck130's numbered SCAN and
its X-marker rung (6 of 6) · deck152's `Their untapped sources:` posture (the line that inverted a
65-turn loss into a T15 win) · deck125's collapsed-range decode (shipped before #274 required it).
**Card facts spot-checked against the primitives by this step or by a seat and re-read here:**
Emeria's Call `borderline.txt:33968`, Agadeem's Awakening `:1177`, Pelakka Predation `:82646` (all
three `autohand={0}:restriction{can play land} … forcetype(land)`, `isflipped` gate removed),
Silverquill Silencer `borderline.txt:102764` (`auto=chooseanameopp … life:-3 opponent && draw:1
controller` — the NAMED-BY price traced end to end), Master of the Feast (5/5 `abilities=flying`),
Ob Nixilis the Hate-Twisted `planeswalkers.txt:2684-2692` (`counter(0/0,5,loyalty)`,
`@drawfoeof(player):damage:1 opponent`), Fate Unraveler `mtg.txt:39182-39190` (Enchantment Creature
3/4), Talisman of Impulse `117529` (three separate `{T}:Add` lines — the `[second copy:` takes are
correct), Hammer of Bogardan `52046`, Pyrite Spellbomb `90168`, Starstorm `112835`, Lay Waste `66043`,
Sphinx's Revelation, Staff of Nin, Intruder Alarm `58850`, Rorix Bladewing `96836`, Siege-Gang
Commander `105071`, Dwarven Blastminer `33251`. **No primitive/Oracle divergence was found by this
step or by any seat, and no Scryfall fetch was needed.**

---

## Specimen recommendation for the owner's per-wave prompt review (invariant 0)

**SHIP `wave56/lategame-specimen.txt` = `125v130` seq 112** (turn 59, `ask`, **24,299 chars, 3 rows**,
32 life against 2) — already staged, nothing to regenerate. Its companion
`wave56/lategame-specimen-logcost.txt` splits it: **narration 20,953 | current situation 2,910 |
legal choices + protocol 436.** The decision is `Land drop: which land do you play now, if any?` —
`Play Seachrome Coast / Play Island / Play no land right now` — with the opponent on **2 life, empty
hand, no creature**, the seat on 14 untapped sources making {W} 7 / {U} 12, holding three Path to
Exile, two Fall of the Gavel, a Dream Fracture and a Cancel, and **two Staffs of Nin that win on the
next upkeep whichever row is taken.** Rows 1 and 2 are interchangeable; row 3 is wrong for a reason
that fits in nine words. **The 125/126 seat's read is carried verbatim and it is the owner's criterion
put as one question: 86% of the prompt is spent on the part that cannot change the answer, at
27.97 s a decision, on the cheapest decision in the corpus.** The information that could change it is
~700 of the 2,910 situation chars and **none** of the 20,953 narration chars, which include twenty-two
`Opponent played Mountain` lines and every Staff ping since turn 35. It is also ledger **D13** on one
screen.

**Beside it, and the reason to keep two:** `126v146` **seq 41** (turn 27, Blockers, **25,023 chars**,
log share 59%) is the wave's whole NEW render surface on one page — the menace header, 9 A-lines with
per-attacker menace annotations, `INCOMING THIS COMBAT`, `{blocking trigger, this combat: …}`,
`{after this combat: you control 1 fewer blocker …}` and `Their untapped sources:` — **and the model
blocked the two Goblins with its two Walls, which is the right read of that header.** If the owner
wants one file, ship the primary; if he wants to see what the wave BUILT, this is the page.

**Recorded and DECLINED as substitutes, so they are not re-proposed:** `125v123` seq 166 (the D2
exhibit: two `they would be at 0; THIS WINS THE GAME` rows, taken, game over, 16,328 chars) is the
best *defect-closed* page but poses no question; `146v152` seqs 41-58 and `130v125` seqs 84-113 are
multi-record READING assignments — **promoted for the lanes, declined for invariant 0**, which asks its
question of ONE screen; `146v130` seqs 1-3 (three pregame prompts under 6 KB whose land tally is wrong
by one, ending in a `PUT` that bottoms a land) is **the shortest complete exhibit for ledger D2** and
is promoted for the wave-57 lane brief.

## The owner-question ruling

**Nothing is owed to the owner this wave.** The standing trigger is *any repeated mulligan against an
explicit floor* (2026-08-26 doctrine). Re-derived: **44 pregame prompts over 42 seats — 40 seats kept
7, 2 mulliganed once and both then kept 6**, with **2 `bottom` records, both the model's**. **Zero
chains, zero double-mulligans, zero mull-to-zero, no mulligan decided by the heuristic**, and no
`would not cover any spell in it` line at either deck125 or deck126 seat across twelve offers. The
wave-55 three-land-seven oddity did not recur. **Both mulligans and the one bottom are ledger D2** — a
RENDER defect with an engine fix, not a floor break — and the seat that owns them said so explicitly
and patched its guide's arithmetic rather than asking, which is the doctrine working. The only OWNER
items in the ledger remain the standing ones: the log window (**D43**) and the frontend/Arena grouping
(**D42**) — plus the one thing this step deliberately did NOT decide, recorded in the REJECT list:
whether the HOLD row may be moved off the LAST position when the declined note is high, which
contradicts a standing ordering ruling and is his call, not synthesis's. **Vita (D44) is his queue,
not a question**: vpk15 awaits his upload window, lanes Y/Z/X are all fixes to his own vpk15 reports,
and vpk16 must regenerate `cardauto.{idx,dat}` or the console builds keep the gated MDFC script.

---

## What this step did NOT check

- **I did not run wagic, build anything, run git, deploy anything, or edit any file under
  `projects/mtg/bin/Res/**` or `src/**`.** Every mechanism attribution below the corpus level is
  quoted from a seat's or a lane's source READ and is stated as a read, not a debugger trace. In
  particular **ledger D1's mechanism** (`restriction{can play land}` parsing to `NO_RESTRICTION`) is
  the deck146/152/162 seat's reading of `src/MTGAbility.cpp:1069-1099` and `:106-136`; **I did not open
  the file**, and I can say only that the rendered behaviour is exactly what that reading predicts.
- **I did not verify any lane's gate by running it.** Suite (1234, 2 known failures), AI (51/0) and
  PARSETEST (2736/0) numbers are quoted from the carry list. Lanes G/H/I/K/N are recorded UNTESTED;
  I confirmed only that no crash, hang, segfault or new error signature appears in the 42 stderr.
- **Lanes Y, Z and X are NOT in this corpus** and I made no attempt to adjudicate them. Their fixes
  merged on master after the corpus binary; I read the three lane files for their residuals only
  (ledger D34, D35) and I did not read the Vita transcripts they cite.
- **Lane W is in flight and I did not read it.** Ledger D1/D2/D3 are marked ABSORBABLE on the
  strength of the task's statement that lane W addresses the MDFC land-drop legality gate; if it lands
  differently, wave 57 re-measures.
- **I did not read all 21 `.stderr` end to end.** I counted 13 strings across all of them, classified
  the neighbourhood of every one of the 65 `dropping stale async answer` lines, and measured the
  consecutive-run length of `only display-toggle` in every file.
- **I did not re-score any deck's guide rules.** Set A is summarised from the seat files' own verdicts;
  the only Set A numbers I re-derived are those a docket item or a boundary finding rests on (the HOLD
  economy per deck and per N ≥ 2 band, the X-menu census, the discard verdict census, the ability-row
  life audit, the MDFC row/take/legality census, the mulligan and bottom census, the INCOMING coverage,
  the `Their untapped sources:` coverage, the guide literal sweeps, and the guide sizes).
- **I could not reproduce the harvest's identical-declined figures (14.3% share, max run 12)** under
  four predicates; mine are 14.99 / 16.8 / 22.3% with **max run 13 in all three**. Recorded as
  irreproducible, not as wrong.
- **I could not reproduce the harvest's `identical_option_asks_resolved` 3**: summed over 42 gameends
  it is **6**. Nothing rests on it.
- **My MDFC row count (65 rows / 44 illegal) and the seat's (62 windows / 41 illegal) differ by unit**,
  not by fact; I did not attempt to reconcile them to a single number because both are correct of what
  they count (#208).
- **I did not diff prompts across polls** (only the final prompt is logged), so the 65 drops are
  classified from stderr ordering and adjacency, not from two live prompt strings side by side.
- **I did not exercise `WAGIC_GPT_BOARDINDEX=0`, `WAGIC_SINGLE_RNG`, `WAGIC_STALL_FLOOR`,
  `WAGIC_OPT_SOURCES_CAP` or `WAGIC_GPT_DRIFT`.** The BOARDINDEX flag is usable for the first time and
  **no reviewer ran it**.
- **I did not check any Vita transcript, replay dump, or console artifact**, and ledger D33-D44 are
  carried from the lane reports and the carry list.
- **I did not re-derive the corpus's win/loss table beyond a tally of `corpus-results.tsv`**, and no
  verdict in any output rests on a win rate.
- **Every census names its field and unit per #208, and its tag family per #298**: row censuses were
  taken from rows parsed out of the rendered `prompt` (`^\d+\. `); clause censuses from the `prompt`
  text; fallback, `plan_echo_count`, `chosen_text` and gameend counters from the record fields; drop,
  release, rescue and 404 counts from the `.stderr` with the emitter's own quoted line; guide literal
  counts from the deployed files **whitespace-normalised** (#294).
