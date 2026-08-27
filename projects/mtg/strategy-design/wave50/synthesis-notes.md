# Wave-50 synthesis (2026-08-27) — SEVENTH FAIR-HAND CORPUS

Inputs read, in order: `wave49/synthesis-notes.md`; `wave49/strategy-writing-skill.md` (the
CURRENT edition, amendments 1-132); `wave49/general-strategy.md` (R1-R71); `wave49/engine-ledger.md`
(the wave-50 docket D1-D26: D1-D14 and D16-D19 shipped as lanes V/W/X/Y/Z; D15, D20-D26 carried);
`wave50/review-carry.md`; `wave50/lane-brief.md`; `wave50/probe-double-pay/REPORT.md`; the four
seat files (`seat-engine-narration.md`, `seat-123-130.md`, `seat-125-126.md`,
`seat-146-152-162.md`); all seven `deckN/skill.md` and `deckN/general-suggestions.md`; and the
corpus directory `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260827-115759/` plus
`src/WParsedInt.cpp` and `bin/Res/sets/primitives/borderline.txt` (read only) for the
reconciliations below.

Outputs: `wave50/strategy-writing-skill.md` (wave-49 edition **verbatim** — prefix `cmp`-verified
— plus **amendments 133-151**), `wave50/general-strategy.md` (R1-R71 with a wave-50 verdict each,
R72-R104 new, SHIPPED-OUT-OF-WAVE, GUIDE-only routing, nine measurement items, PASS/KEEP),
`wave50/engine-ledger.md` (the wave-51 docket D1-D32, ranked, engine/UX only, each with a record
citation; both discharge tables; fallback classes; per-kind prompt chars; lategame specimens), and
the boundary findings below.

**Ownership (owner doctrine 2026-08-26).** Reviewers own every `deckN/strategy.txt`; this step owns
the skill and the general strategy; the core loop — engine correctness, LLM UX, bugs — is the
purpose. **No guide file was edited by this step.** Step 4 was run as a report (B1-B7). One
proposal that would have removed a legal window (deferring Upkeep animations) is adopted in its
annotation form only; hard caps were proposed by nobody. **No deck-content question is open for
the owner.** No file under `projects/mtg/bin/Res/**` or `src/**` was modified.

---

## Reconciliation against the records

1. **The "deck123-vs-deck162 dead-loop, excluded" note is WRONG for this corpus for the second
   wave running and is dropped.** `seat-125-126.md`'s header carries it; the engine seat,
   seat-123-130 and seat-146-152-162 each caught it. Verified on disk by this step over all 42
   JSONL files: 42 `gamestart`, 42 `gameend`, 1,780 decisions across seven kinds, exactly 3
   `fallback` fields (all `stale_echo`). This run's 123v162 pair is `1787849886-...deck123-
   0x55de8e828b00-vs-...deck162.jsonl` (43 records, natural end at turn 13). Every rate in the
   three output files uses 21/21; seat-125-126's per-seat numbers are unaffected.
2. **Soul Shatter's mechanism — two seats, two hypotheses, one confirmed.** The engine seat
   offered lane W's `ownChooser()` gate OR the `convertedcost:highest` filter; seat-146 offered
   the filter with the planeswalker-outranks-creature reading. This step read
   `src/WParsedInt.cpp` 386-438: `convertedcost:highest:*:myBattlefield` builds a chooser over
   `*|myBattlefield` and takes the highest MV over EVERY permanent it can target, so Ob Nixilis
   (MV 5) sets the bar, `creature[manacost=5]` matches nothing, and the sacrifice resolves as a
   no-op. Seat-146's reading is the mechanism; lane W's gate is exonerated (its own prediction
   held 0/15). Recorded as a code-read, not a debugger step; lane A owns the fix. The second
   defect (Oracle says creature OR planeswalker; the script never takes a walker) is on the same
   docket line. Verified on disk: seq 25 -> 26 events carry `- Paid`, `You cast`, `resolved` and
   no victim.
3. **The receipts.** Engine seat: 396 `- Paid` receipts, 0 doubles, 69/386 generic-from-dual;
   seat-146: 33/146 on its seats by a land-only heuristic, and `- Paid` MISSING on 3/9 land
   animations. The engine seat counted doubles, not misses. This step re-read `146 vs126` seq
   32/36 (Hive: `You used` with no receipt) and `152 vs126` seq 25 (Lair: no receipt) against
   `152 vs125` seq 17/59 (Lair: receipt present) — seat-146 is right; D5 is on the docket as a
   hole in the instrument that D2 and the D1-regression check both depend on. The two dual-land
   counts are consistent (different denominators).
4. **Repeat-row takes: 9, not 8.** Seat-123-130's prose says "eight repeat-row replies" and then
   lists nine takes; the engine seat lists nine (x21, x0, x17, x1, x1, x3, x3, x28, x25). Nine is
   used; "PLAN with a stop number 8/8" becomes 9/9 in the ledger (the x0 reply also carried one).
5. **`parse_note` is 10, not 0** (engine seat's correction of the brief): 5 + 3 + 2, consistent
   with the per-seat counts (seat-123 5, seat-146 4, seat-125-126 1 `answer_replaced`-adjacent).
6. **Max prompt 25,793 (a priority record)**, not 25,018 (the max ASK) — the engine seat's
   correction; both seats that quote it agree.
7. **The "vanishing target rows" of wave-49 D6** were the designed two-decline cap (lane W's
   note in `review-carry.md`); the engine seat's 801 stderr auto-passes confirm the cap is live;
   no seat re-filed it. The cycling row's `{if you pass here ...}` tag is the same cap's
   wording (R101, LOW).

---

## The headline: the lanes closed the falsehoods, and what the guides lost was SEQUENCE

**(a) FIVE LANES SHIPPED AND EVERY WAVE-49 HIGH WAS DISCHARGED BY STRING.** The double-tap: 0/236
casts at 2x pips, 0/26 counter-then-payment in stderr, one `- Paid` per cast 396/396. The 34,333-
char row: max target clause 288 chars, max prompt 25,793. Devour Flesh's pronoun: 0/7 wrong
recipients, `they gain N - the sacrificing player gains, not you`. The engine's cleanup discard:
28 `discard` records, 26/26 narrations preceded by their record, the pilot choosing sanely 7/7 at
deck123. The stale reveal zone: 0/15 foreign-chooser messages, and the exact wave-49 loss window
(`126 vs146` Tribute) replayed correctly — skill #132's falsifier, paid. The self-target rows:
0/124 taken (was 2). The exemplar: 0/507. "The game is lost": 0 carries, and the 50-window correct
plan carried unbroken. The named card: 0/76. The FORECAST mirror: 50/50. **No render falsehood
this corpus** (wave 49 had one).

**(b) THE NEW DOCKET'S TOP IS A CARD THAT RESOLVES AND DOES NOTHING.** Soul Shatter against a
5/5 and a planeswalker: `You cast / resolved / went to your graveyard` and no victim; the
punisher stayed and the draw step killed deck146 from 7 life. The mechanism is in the ranking
grammar (reconciliation 2), the fix is card-script first per the owner's rule, and the same card
never takes a planeswalker at all. It is IN FLIGHT at lane A. The second item is the ordering the
receipt exposed the first corpus it existed — 69/386 payments took a dual for a generic pip with
a basic idle — IN FLIGHT at lane B. The third is the corpus's entire fallback set: three replies
that named a hand card with an unpayable pip, labelled `stale_echo`, sent to Baka, which cast
the wrong card twice.

**(c) THE GUIDE LANE'S LOSSES ARE ORDER-OF-WORK, NOT VOCABULARY.** The answer-first protocol
writes the count before the subtraction: four repeat-row replies put x<N> on line one and "this
window: pass" beneath it (#133), one of them `x0` (#134). A rule quoted back by the pilot and
broken in the same reply for a third corpus at TWO seats — the Tutor with an Alarm owned, the
Hammer return with the Commander in hand — is the executor failing to sequence, not to see, and
routes to the render with one guide line kept (#138). A new ask kind (cleanup discard) arrived
with no order at any seat and cost deck126 a Tribute and a game (#136). Three absolutes met
their least-harmful member — a `[defender]` victim, the seat's own Emrakul under `1 of yours`, a
1/1 lord — and the carve-outs are keyed to the printed tag (#135). A brake that counted punishers
missed the discard converter that dealt the win's damage (#149).

**(d) THREE PROPOSALS ARE ADOPTED IN A NARROWER FORM THAN OFFERED.** The Upkeep-animation DEFER
(removes a window) becomes an annotation (D7). "The guide teaches the primitive" where Oracle and
script disagree becomes "the guide states the card fact the engine delivers, the seat routes the
script fix" (#145) — the owner's Oracle-first rule governs. The X-menu exit row waits on the
engine seat's cancel-semantics ruling; the cast-row clause form ships (D25).

---

## What merged into the skill (133-151)

**Convergence merges (>= 2 seats):**
- **133** blanks filled before line one under answer-first — deck123 S1 + seat-123 M1 + deck146
  E-5/G-3 (the prose-reversal is the same commit-before-think shape).
- **135** absolutes audited by constructing the least-harmful member / the deck's own exception,
  carve-out keyed to the printed tag — deck123 S3 + deck125 S2 + deck126 S1.
- **136** a new decision kind gets a per-guide order the same wave, copied from the nearest
  existing one, and the engine-behaviour sentences retire — deck123 S4 + deck125 S4 + deck126 S2.
- **137** named-card checks port across the pool by tag string; in-guide class exceptions are
  cross-references — deck123 S5 + deck130 S3.
- **138** a third-corpus identical failure with the reply quoting the rule routes to the render,
  guide keeps one line — deck123 S6 + deck130 S5.

**Decisive single-seat merges:**
- **134** `x0` is not a count; the pass row taught beside the count grammar (deck123 S2).
- **139** the #128 grep per ROW FAMILY; the silent family named as a card fact (deck130 S1).
- **140** two-screen commits taught with the exit one screen earlier, exit row quoted (deck130 S2).
- **141** a damage rule names its quantity and the sibling kill case with its own condition
  (deck130 S4).
- **142** a fixed-number gate re-derived from its purpose when its breaks stop costing games
  (deck125 S1).
- **143** list-answer asks (`PUT:`) are a third emit surface; content rule in emit form + the
  excluded class at the current N (deck146 S-1).
- **144** cross-guide KEY diff in the boundary pass (deck146 S-2).
- **145** class words checked against the primitive's filter; divergence = card-script defect,
  guide states the delivered fact (deck146 S-3, adapted to the owner's Oracle-first rule).
- **146** matchup imperatives audited against what the row says the action does (deck146 S-4).
- **147** every Upkeep-offerable activated ability gets a section before its first corpus
  (deck152 S-1).
- **148** per-card block-ladder exceptions keyed to the B-row parenthesis + header N (deck152 S-2).
- **149** read the WIN's events before a violated-unpunished verdict; a missing converter is a
  fact (deck162 S-1).
- **150** the lethal check enumerates one-shot draw sources with their windows (deck162 S-2).
- **151** "this list" = the menu; one sentence per guide on the unpayable-pip reason (deck126 S3).

## What was REJECTED, and why
- **Deferring Upkeep mana activations to Main 1 (deck146 G-1's first form, seat E-4).** It removes
  a legal window; an Upkeep animation is occasionally right and the row is the model's to decline.
  Adopted as the annotation (D7). Same governing constraint as wave 49's raw-count expiry.
- **"Where Oracle and primitive disagree the guide teaches the primitive" (deck146 S-3 as
  written).** The owner's rule is Oracle-first: a divergence is a card-script defect to fix, not
  a fact to teach. Adapted (#145): the guide states the card fact the engine currently delivers,
  never the divergence; the seat routes the script fix; re-key when it ships.
- **A `0. Do not announce` row on the X menu (deck130 G4's first form) as a docket ask.** Cancel
  semantics after `Cast Card Normally` are the engine seat's to rule on; the cast-row clause form
  is docketed now (D25), the exit row conditionally.
- **Any raw-count plan expiry, re-filed (deck162 G-4, seat-146 P4).** Confirmed retired: the
  50-window run at `125 vs123` is a correct guide-sourced plan; content keys hold.
- **The "pass and hope" plan carried 5 as a defect (engine seat observation).** It named cards;
  the content test worked as designed; observation only (R102).
- **deck146's 1-5 and deck152/162's 5-1 as guide effects.** n=6, real hands: deck146 saw a 1-land
  seven three times in six games; three of its five losses are mana. Counts only (#87).
- **The `x1` repeat takes as a rule.** Harmless singles.
- **The cycling row's `{if you pass here ...}` rewording as a MED item.** One window, marked
  inference by the seat; carried LOW (R101).
- **Any guide compensation for D1 (Soul Shatter), D2 (auto-tap), D3 (the fallback label), D5
  (missing receipts), D6 (silent ability rows).** Trust doctrine: a guide describes cards and
  printed strings; #139 lets the guide say a row family prints no result (a fact about the row),
  never that the engine is wrong.
- **A fifth restatement of deck162's Rule 2.** Re-scoped by the Caress fact (162-E, #149) and
  carried to the engine (D11); the seat's own routing.

---

## Layer routing (`general-strategy.md`)

RENDER/CORE, ranked: Soul Shatter no-sacrifice, IN FLIGHT lane A (R72) · auto-tap generic from
duals, IN FLIGHT lane B (R73) · in-range index + off-menu name -> `[RE-ASK]` (R74) · `x0` = pass +
the CHOICE/PLAN conflict re-ask (R75) · `- Paid` missing on activations (R76) · ability-damage rows'
DIES/SURVIVES (R77) · Upkeep animation annotation (R78) · Tutor / second-copy tags (R79) · Hammer
return priced (R80) · Soul Shatter's `{right now:}` (R81) · feeds count + Caress (R82) · creature
converter tag (R83) · edict victim tags (R84) · opponent-draws pricing (R85) · discard-list grammar
(R86) · LOW: bare `{target text:}` back faces (R87), receipt separator (R88), `turn` off-by-one
(R89), `rejected_line_skipped` stamp (R90), `decision_reversed_in_prose` (R91), repeat row L + C
(R92), enters-tapped (R93), Lightmine header (R94), GANG punisher (R95), X-menu exit (R96), pain
sources (R97), stale state block (R98), damage-order prefix (R99), singular (R100), cycling tag
wording (R101), the "pass and hope" observation (R102), latency (R103), log-share decision (R104).
Of R48-R71: **R48, R49, R50 (own shape), R51, R52, R53, R55, R58, R60, R61, R62, R64 SHIPPED and
PASSED**; R54/R56 shipped with one observation each; R57/R65 shipped, untested; R59 half-tested;
R63 passed and re-opened on a third path; R32's ORDER failed by R64's receipt; R39 carried a fifth
corpus.
GUIDE-only: listed per deck in `general-strategy.md`.
MEASUREMENT: nine items, headed by the victim line per edict take and the receipt's three checks.
PASS/KEEP: every lane V/W/X/Y/Z string in the ledger's Set B; **no render falsehood this corpus**.

---

## Guide boundary findings for the deck reviewers

Step 4 was run as a REPORT over the seven wave-50 `deckN/edit-texts.md` proposals and the wave-49
`deckN/strategy.txt` files they amend (the reviewers' wave-50 guides are theirs to commit). Sweeps
by the seat files' own quotations and this step's reading of the edit texts: engine-behaviour
vocabulary (#99/#103/#136), the hard-floor family, quoted render literals (#128, now per row family
#139), the cross-guide KEY diff (#144), the Oracle/primitive class words (#145), the new `discard`
kind (#136). No finding below turns on a card fact this step did not see quoted from a primitive.

**TRUST DOCTRINE: CLEAN, all seven.** The wave-49 B1 sentences were cut on schedule and lane W made
one of them false exactly as predicted; no guide teaches doubt of a rendered value. #139's "these
rows print no result" (deck130 D50-5) is a fact about a row family, not a hedge — PASS.

| # | file : site | amendment | finding | suggested wording |
|---|---|---|---|---|
| **B1** | every guide's cleanup `discard` section — present only at deck123 (A50-9), deck125 (W25), deck126 (F18) | **136** | **Four guides (130, 146, 152, 162) have NO order for the new `discard` ask.** 28 asks this corpus; the three seats that lost something to it wrote sections; the four that did not will meet it next corpus with nothing (deck146 had six `discard` records on one land at `vs125` and chose reasonably by luck). The order is derived from each guide's bottoming order — the same list, a different N. | Copy the bottoming order's first line and invert it for discards: *"Cleanup discard (`PUT: n, m`): send the highest-cost spell you cannot cast in the next two turns, a duplicate of a card already on your line, then a land beyond the sixth; never the only <engine piece>."* Run #143's PUT-vs-PLAN metric. |
| **B2** | `deck146` entry 1 (Soul Shatter "creature or planeswalker you can hit") | **145** | **A class word the primitive does not deliver, and a lane IN FLIGHT that will change what it delivers.** Today the engine takes only a creature and, with a walker outranking it, nothing; after lane A it takes the highest-MV creature-or-walker. Either wording goes false at one end of the lane. | Until lane A ships: *"Soul Shatter takes their highest-mana-value creature"* (the delivered fact; do not plan it for a planeswalker). The wave lane A lands: re-key to *"their highest-mana-value creature or planeswalker"* — #136's checklist item catches the swap. Never: "the engine ignores planeswalkers". |
| **B3** | `deck130` D50-5 ("these rows print no result; read the (P/T) yourself") and `deck123` A50-5 / `deck130` D50-1 (the one-line Tutor / Hammer rules) | **139**, **138** | **PASS — recorded so nobody "fixes" them.** All three are the shapes the skill now prescribes: a row-family fact, and a one-line rule with the render ask in the seat file. When D6, D8, D9 ship, D50-5's sentence goes stale (a row that now prints a result) — cut it that wave; the Tutor/Hammer lines stay true. | none now; D50-5 is cut the wave D6 ships. |
| **B4** | `deck162` RULE 2 (162-E re-scope) and `deck162` G-1 | **149** | **The Caress converter is in the guide but not yet in the row (D11 unshipped, fifth corpus).** The guide's count now reads "punishers + Caress with their hand at 7+"; the executor must derive the hand size from the opponent-hand line. Correct and complete; the row will carry it when D11 ships. | none; re-check that the guide's count words match the `{feeds:}` string when it lands (#128). |
| **B5** | `deck152` 152-E (Lair section) and `deck146` 146-C (Hive) | **147**, **138** | **PASS with one obligation: both sections say "Upkeep -> pass" as an odds sentence, not a floor.** Confirmed by the seat quotations. When D7's annotation ships (`the same row is offered again in Main 1`), key the sentence to that literal. | none now; re-key to the annotation string when D7 lands. |
| **B6** | `deck125` W22 (the Staff gate re-derived to "the cost of the cheapest counter in hand that can hit what the opponent casts") | **142** | **PASS — but the new key has a class word ("can hit what the opponent casts": Scatter = creature spells) that #145 asks to check against the primitive.** Essence Scatter's primitive targets `creature` spells on the stack; a creature spell with a non-creature card type on the stack (an artifact creature is fine; a creature MDFC cast as its land face is not a spell) is the edge. Preventative. | Add the delivered fact once: *"Essence Scatter counters only a creature spell (the row says `[creature]` on the stack line)"* — keyed to the printed stack tag, not the class word. |
| **B7** | `deck123` A50-1 ("do the subtraction before you write the first line") and the `x0` line (A50-1/#134) | **133**, **134** | **PASS.** The first guide text written against the answer-first protocol's ordering. One check for the wave-51 seat: the metric is CHOICE-vs-PLAN disagreement 0/N; when D4 ships (x0 = pass; the conflict re-ask), the `x0` sentence becomes redundant but not false — leave it. | none. |

**Not findings — checked so nobody re-files them.** deck126 F17's `[defender]` carve-out and F16's
row key: #135/#118 exactly. deck125 W21's `1 of yours` key: #135. deck130 D50-7's ported walls
check: #137 — the tag string is identical at both seats, confirmed by the seat quotation. deck146
146-L (the BLOCK half cut, ATTACK half kept): #146 per imperative. deck152 152-F keyed to the
B-row parenthesis + N: #148. deck162 162-D/162-F: #150, #131. The wave-49 B2 NEVER-sets at
deck162/deck152: converted (162-F, deck152 B2) — PASS. The wave-49 B3 repeat-row literal: the row
now prints `x<N>` and deck123 quotes the shape — PASS (#128 grep hits). The wave-49 B5 "hard stop"
labels: gone — PASS. The 72c mood-word sweep: zero hits reported by any seat.

---

## For the next wave

The wave-51 docket is engine correctness at the top again, and for the first time the top item is
a CARD-SCRIPT defect with its mechanism read from source: **D1 is in flight at lane A (Soul
Shatter — the ranking grammar counts every permanent; the Oracle wants creature-or-walker), D2 at
lane B (the auto-tap order the receipt exposed), D3 is the whole fallback set (a re-ask instead of
a wrong label), D4-D6 are one-file parser/render fixes with PARSETEST cases and a suite pin each,
D5 closes the hole in the instrument D2 depends on.** D7-D14 are the render's remaining silences —
every one the lane-J principle (the row that asks carries the number), and three of them (D8, D9,
D11) are third-to-fifth-corpus items whose guide lane the skill now forbids restating. The guide
lane's work this wave is order-of-work (#133-#136) and the audits (#135, #139, #144-#146) — run
them BEFORE the corpus, not after. Four guides need a cleanup-discard order before wave 51's
corpus runs (B1). deck162 stays the pool's exit candidate: on the wave-51 agenda if D11 ships and
162-D fires in its first window; deck152 is exit-eligible if 152-E/F hold and a both-halves window
finally PASSES; deck146's converter lane is validated (4/4) and its UNTESTABLE flag is lifted. The
log-share decision (D30) is the owner's: prompts got 13-27% smaller and latency did not move, so
compaction is not asked for yet — measure once more.
