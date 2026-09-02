# Wave-52 synthesis (2026-09-02) — NINTH FAIR-HAND CORPUS

Inputs read, in order: `wave51/synthesis-notes.md`; `wave51/strategy-writing-skill.md` (the CURRENT
edition, amendments 1-167 — amendment index and tail read, the rest grepped); `wave51/general-strategy.md`
(R1-R135); `wave51/engine-ledger.md` (the wave-52 docket D1-D27, of which lanes G-L shipped D1-D17);
`wave52/review-carry.md`; `wave52/reviewer-brief.md`; the four seat files (`seat-engine-narration.md`,
`seat-123-130.md`, `seat-125-126.md`, `seat-146-152-162.md`); all seven `deckN/skill.md`,
`deckN/general-suggestions.md` and `deckN/edit-texts.md`; and the corpus directory
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260901-223430/` (read with python, and the `.stderr` files
with `/usr/bin/grep`) for every reconciliation below. Card facts were re-verified against
`projects/mtg/bin/Res/sets/primitives/*.txt` with `/usr/bin/grep`; three emitter mechanisms were
pinned by READING `projects/mtg/src/AIPlayerGPT.cpp` (read-only, nothing built or run).

Outputs: `wave52/strategy-writing-skill.md` (the wave-51 edition **verbatim** — prefix `cmp`-verified —
plus **amendments 168-186**), `wave52/general-strategy.md` (R1-R135 with a wave-52 verdict each,
R136-R160 new, layer-routed, twelve measurement items, PASS/KEEP), `wave52/engine-ledger.md` (the
wave-53 docket D1-D28, ranked, engine/UX only, each with a record citation; both discharge tables;
fallback classes; per-kind prompt chars; lategame specimens), and the boundary findings below.

**Ownership (owner doctrine 2026-08-26).** Reviewers own every `deckN/strategy.txt`; this step owns
the skill and the general strategy; the core loop — engine correctness, LLM UX, bugs — is the
purpose. **No guide file was edited by this step.** Step 4 was run as a report (B1-B9). Four
proposals that would have removed a legal window are rejected in that form and one is adopted in a
constraint-safe form; hard caps were proposed by two seats and are rejected on sight; every mulligan
text quoted by the seats is odds-form. **No deck-content question is open for the owner.** The two
Baka-decided mulligans are an engine/observability item, not a guide question. No file under
`projects/mtg/bin/Res/**` or `src/**` was modified, no git command was run, wagic was not launched.

---

## Reconciliation against the records

1. **Health, re-derived on disk over all 42 JSONL files.** 3,384 records / 3,300 decisions (ask 2,540
   / priority 535 / attackers 107 / discard 54 / blockers 41 / reveal 15 / bottom 8), 42 `gamestart`,
   42 `gameend`, 21/21 natural. **15 `fallback` fields**: `plan_choice_conflict` 5, `empty_reply` 3,
   `unparsed_reply` 3, `multiblock_reask` 3, `named_row_reask` 1. Every seat file's per-seat count
   sums correctly (deck130 4 + deck146 3 + deck152 2 + deck125 2 + deck123 2 + deck126 2 + deck162 0
   = 15). `answer_replaced` 16, `coded_answers > 1` **50**, `commit_retracted` 0. The engine seat's
   whole health table is confirmed by string.
2. **`decision_reversed_in_prose` — the two seats reached opposite verdicts and the records settle it
   AGAINST the deck seat.** seat-123-130 **H1** and deck130 **G8** report the check "right 0 times",
   firing on deliberation, and ask that a prose verdict NAME a row before it counts. Read in full,
   all five records carry a prose pass verdict AND **three carry `answer_replaced: True` with
   `coded_answers: 2`** — the reply's FIRST coded line was `CHOICE: 0 (pass)`, the model then
   re-derived at length and emitted a SECOND coded line taking the row, and the engine had latched
   that second line (`choice: 1`). The re-ask reverted it to pass at `130v125` s48 -> s49 (a 5-mana
   Hammer of Bogardan return the model's own final answer refused), s66 -> s67 (the same), and
   `146v125` s59 -> s60 (a Lolth `+0` at 20 life); it confirmed the row at `130v125` s82 -> s83 and
   `146v125` s133 -> s134. **The lane changed the executed action in 3 of 5 firings.** The engine
   seat's "3 of 5 were genuine reversals" is right; seat-123-130 read the head of a 3,290-character
   reply and reported "no row is named anywhere in the prose" when row 1 is named in its tail. The
   proposal is REJECTED; the telemetry half (seat-146 E-4) is adopted as docket D12.
3. **The opponent-turn window count: three seats, three denominators, none reproducible as stated.**
   Engine seat: 913 windows / 97 acted (10.6%). seat-146: 375 of 1,351 records (27.8%) / 64.9 min at
   its three decks, grouping by (turn, option set, answer) with 3+ occurrences. seat-125-126: 381
   opponent-turn windows / 23 acted at its two decks. Re-derived corpus-wide: **884**
   `Casting decision (…, opponent's turn)` asks with **68 casts (7.7%)** and 10,979 s, **plus 82**
   opponent-turn `priority` windows with 17 acts — **966 windows / 85 acts (8.8%)**. The engine
   seat's per-phase WINDOW counts are within 3% of mine and its zero-act phases are **exact**; its
   cast total (97) counts something wider than the seat's own casts (its counterspell list — Cancel
   16, Essence Scatter 14, Fall of the Gavel 12, Dream Fracture 10 — and its removal list — Path 5,
   Soul Shatter 4, Tribute 3, Vanishing Verse 1, Tragic Slip 1 — reproduce to the card, 66, plus a
   Spark Spray and a Revelation = 68). Its prose total of 2,924 s for the dead phases is an
   arithmetic slip: its own table sums to **3,124 s**. seat-146's 375 is right on ITS definition
   (non-consecutive grouping); the stricter CONSECUTIVE form gives 308 of 1,351 (22.8%) / 41.6 min
   at those seats and **760 of 3,300 (23.0%) / 143.2 min corpus-wide**. All three seats are right on
   their own denominators; the ledger uses the two clean ones and names both.
4. **The suppression risk that decides the remedy.** The engine seat reported that a blind
   identical-menu cache "would have suppressed 5 real casts (3 under the stricter form)". Re-derived:
   grouping by (turn, byte-identical option set) with **no size floor**, **30 real actions** in the
   corpus followed a decline on an identical set within the same turn. The specimen the engine seat
   cited is real and is the clearest one — `130v152` seq **12 -> 13**, two byte-identical
   `Cast Spark Spray {r} … / Cast nothing right now` menus in turn 3, declined at Main phase 1 and
   **cast at Combat begins** — it simply lives in a group of size 2, which is why the >= 3 form
   misses it. Every cadence proposal is rejected on this number.
5. **The three Soul Shatters (`146v125` s327-329) — both of seat-146's candidate mechanisms are
   wrong, and the real one is in the records.** seat-146 offered (a) the sacrifice fizzling against
   `protectionfromcoloredspells` or (b) three copies coalescing, and did not separate them. On disk:
   s328's events read `- Your Soul Shatter resolved and went to your graveyard / - Opponent used:
   sacrifice a creature or planeswalker with Soul Shatter targeting Emrakul` and s328's **stack block
   reads `1 (top): ability: sacrifice a creature or planeswalker [triggered/activated ability]`**;
   s329's reads the same line **twice**; the death is narrated only at s330. The granted ability sits
   UNRESOLVED on the stack while the pilot is re-offered the cast row, and the `{right now: they
   sacrifice Emrakul (MV 15, their highest)}` forecast (src/AIPlayerGPT.cpp:1146) is computed from
   the battlefield without regard to it. Protection is not involved (Soul Shatter's
   `auto=ability$!… sacrifice notaTarget(…)!$ opponent` does not target; Emrakul is
   `abilities=nofizzle,flying,protectionfromcoloredspells`, and wave 51 already recorded it being
   sacrificed correctly). **Wave-51 lane A is NOT reopened** — three abilities were created and one
   found a permanent; nothing failed to sacrifice.
6. **`{paying this taps: <defender>}` — the class is right, the count is not.** seat-125-126 files 91
   rows "carrying `- it cannot attack this turn`". Re-derived: **101** `{paying this taps:` rows
   corpus-wide, **91 name a defender**, and **22 of those 91 carry the tail**. The emitter
   (`paymentTapsClause`, AIPlayerGPT.cpp ~3108-3120) appends it unconditionally when the caller sets
   the flag, so the caller already suppresses it in 69 of 91 — the ask narrows to a `defender` test.
   The seat's two repros (`126v130` s13, `126v125` s86) both carry the tail and both stand.
7. **`[second copy:` — the dead-cast class is Intruder Alarm ONLY.** 70 rows / 28 takes, by card:
   Staff of Nin 7, Chromatic Lantern 5, Talisman of Impulse 5, **Intruder Alarm 4 (of 14 rows)**,
   Howling Mine 3, Exquisite Blood 2, Ranger Class 2. Verified against the primitives: Intruder Alarm
   (`mtg.txt:58850`) is `lord(creature) doesnotuntap` + two `@movedTo(…):untap all(creature)` — a
   second copy adds nothing, and seat-123-130's H2 is confirmed. **Chromatic Lantern
   (`mtg.txt:19630`) carries the non-stacking `lord(land) transforms(…)` AND five own
   `{T}:Add{G/R/U/B/W}` abilities** — a second Lantern IS a mana source, so seat-125-126's reading of
   its five takes as dead casts is a deck judgment, not a render falsehood; deck126 G7's completion
   form ("this copy adds only its own {T}: Add") is the correct ask and is adopted.
8. **`{castable from exile}` 4/17 — the mechanism is now pinned.** `exileCastNote`
   (AIPlayerGPT.cpp ~12959-12987) derives the cause by scanning both battlefields for a permanent
   whose text contains `canplayfromexile`. Cross-checked against the board lines: `146v152` seq 32
   carries the cause **and Elite Spellbinder on the opponent's battlefield line**; seq 36/56/68/75/81/
   87/98/104 carry neither. The grant survives on the card
   (`newability[canplayfromexile forever]`); the cause does not. The fix is to stamp the cause at
   exile time, not to widen the scan.
9. **D8's collapse — seat-123-130 scored it PASS and the records say FAIL.** The seat wrote that
   "the 40-row menu at v162 s41 is 40 distinct creatures, one row each - a target list, not a
   duplication". Masking `#N`: `123v162` seq **41** = 40 rows / **2** distinct shapes, seq **42** =
   39 rows / **1**. Those are the only two windows in the corpus with > 12 rows collapsing to <= 2
   shapes (the 40-row attacker list at seq 43 and the 50-row library reveals ARE genuinely
   distinct). The engine seat is right; D8 shipped on cast-row target clauses and battlefield lines
   and did not reach ability menus.
10. **`- Paid` receipt totals differ by dedup method, not by fact.** The brief says 464, the engine
    seat 503, seat-123-130 124 at its seats, seat-146 177 at its seats. Deduping identical receipt
    LINES per seat gives **431 unique lines**; every method agrees on the load-bearing numbers —
    **0 `with no source tapped`**, **3 with no payee** (all the Intrepid Adversary ETB class), 2
    reading `from mana already floating`.
11. **The 900 s timeouts.** Three records, `latency_ms` 900,018-900,021, `reply: ""`,
    `fallback: empty_reply`, Baka executing all three. Two are MULLIGAN asks on tiny prompts
    (`126v146` seq 1, **1,418 chars**; `123v130` seq 3, **1,639 chars**). Exactly one record in the
    corpus carries a `retry` field, so the retry path exists and did not engage. `WAGIC_GPT_TIMEOUT=900`
    is the harness's dial and the proximate cause is server queueing at `-j 21`; the docket item is
    OBSERVABILITY (a `timeout` class, a stderr line, one retry), not the number and not the model.
12. **The zero-block multiblock is exactly as filed.** `152v162` seq 26 carries
    `fallback: multiblock_reask`, `dropped_assignments: 1`, `chosen_text` null and **no follower
    record**; `game-152v162-1788320098.stderr:1123`'s re-ask line is followed immediately by
    `Action added to stack: NextGamePhase. (Current phase is: Blockers)` — no
    `declared blocks from N assignment(s)`, no second model call. The other two `multiblock_reask`
    records have followers (`152v146` 76 -> 77, `126v152` 54 -> 55).

---

## The headline: the lanes shipped, and the render vocabulary the guides were keyed to changed under them in one wave

**(a) SIX LANES SHIPPED AND EVERY WAVE-52 HIGH WAS DISCHARGED BY STRING.** `stale_livelock` **0** in
3,300 decisions with lane G's deferral firing 124 times; the pass-row echo **0/543**; the PUT parser
**0/77**; the payment forecast **0/101** naming a wall twice; the index/name conflict **1/1** with the
named row executed; the life-cost clause **0** breaks; the self-hit tag on **179** rows with **0**
takes; `[legendary:` **19/19**; the tutor form warning against the missing half **0/44**; the
`becomes` re-offer **0**; the animated-land tap **0**; the fetch clause **69**; `{spends K of M}` on
**546** cast rows. `[already owned:` is **0 renders**. **No render falsehood this corpus — third wave
running.** And lane I is the largest capability change the pilot has had: **884 opponent-turn casting
asks where wave 51 had none**, 68 casts, 52 of them counterspells that deck125's whole thesis needs.

**(b) THE PRICE OF THAT CAPABILITY IS THE NEW DOCKET'S NUMBER TWO, AND EVERY OBVIOUS REMEDY IS
REJECTED.** 966 opponent-turn windows for 85 acts; **286 windows across six phases produced ZERO
casts and cost 3,124 s**; 23.0% of all decisions sit in a run of three or more consecutive
byte-identical declined menus inside one turn, worth 143 minutes of the corpus's 22 hours. But a
blind identical-set cache would have suppressed **30 real actions**, and the cleanest specimen is two
byte-identical menus one phase apart, declined then cast. The adopted form is a row the MODEL may
take — `Hold priority - do not ask me again this turn unless the board changes` — which removes no
window and turns nine dead stops into one answer.

**(c) THE OTHER THREE HIGHS ARE THE RENDER OWING A NUMBER IT ALREADY COMPUTES.** A `[DRAW PRICE:]`
naming seven damage at three life prints no lethal tail while lane K prints one for the same
arithmetic on a mana cost — 41 such rows, one take, and it was the game (D3). An edict's
`{right now: they sacrifice Emrakul}` is computed from the battlefield while two identical
sacrifices sit unresolved on the stack aimed at that same permanent — three Soul Shatters, one death,
six mana at nine life (D4). And a targeted spell's CAST row carries no kill summary while the FORCED
target ask that follows carries three `- SURVIVES` verdicts: **85 cast rows with no verdict against
9 target rows with one** (D5). In every case the engine has the number one screen too late to be
refusable.

**(d) THE GUIDE LANE'S LOSSES ARE A RETIRED LITERAL AND A TEST THAT LEAKED OUT OF ITS BRANCH.** Lane
K rewrote the ownership vocabulary and two guides' rules went from N/N to 0/N without a word of their
text changing: deck126's Lantern rule declined **9 of 9** `[already owned:` rows in wave 51 and took
**5 of 5** `[second copy:` rows here, and deck123's "the Slip's target rows print no verdict"
sentence became false the wave lane L started printing them — the sentence that cost a card at 6 life
(#168, #169). The second shape is a coverage line scoped to ONE-LAND hands being read as a ship at
two and three lands, in **two guides at once**, four times (#171); and a deploy floor with no
carve-out scoring **nine correct declines** of `[legendary:` second copies as breaks (#184).

**(e) ONE SEAT VERDICT IS REVERSED AND ONE MECHANISM IS PINNED THAT NO SEAT PINNED.**
`decision_reversed_in_prose` was filed as 3-for-3 false positives and is, on the records, the check
that reverted three 5-mana mistakes the model's own final answer refused; the proposal to narrow it
would have removed exactly those three. And the three-Soul-Shatter defect, filed with two candidate
mechanisms and neither separated, is a stale forecast against an unresolved stack object — which
makes it a RENDER item with a one-clause fix, not a rules bug.

---

## What merged into the skill (168-186)

**Convergence merges (>= 2 seats):**
- **168** the literal audit is the reviewer's FIRST step and reports three numbers per literal
  (old-string renders, new-string renders, takes under the new string) — deck123 S1 + deck125 S2 +
  deck152 S5.
- **169** a replaced clause answers its author's question, not the old clause's; re-key by QUESTION —
  deck123 S2 + deck126 S7 + deck125 S2, with the Chromatic Lantern counter-case attached.
- **171** a test named under one branch is applied to its neighbours; a rung that ends in a floor may
  not be followed by a qualifier — deck146 S1 + deck123 S4 (four windows, two guides).
- **179** a prohibition names what the card is held FOR as the alternative ACTION, and a defensive
  gate carries its offensive half — deck130 S8 + S10.

**Decisive single-seat merges:**
- **170** a rule lives on the window that can still refuse (deck123 S3; 85 vs 9 rows).
- **172** a plan template gives each INPUT its own slot (deck123 S5).
- **173** a rule whose trigger is a printed PRICE quotes the clause and never restates the formula
  (deck125 S1; the loss).
- **174** a card whose text is a reversible conditional gets an explicit FALSE SENTENCE (deck126 S4;
  nine turns of a backwards drain loop).
- **175** a win condition is a list of STARTERS with an explicit no-starter branch (deck126 S5).
- **176** the PLAN is an input: guides ask for forward INTENT, never a board claim (deck126 S6).
- **177** the DECIDING SITUATIONS list is a mechanical derivation of the prose (deck130 S6; the
  gang-block loss).
- **178** a count gate quotes the collapsed-handle form and states the arithmetic (deck130 S7; 8/19).
- **180** one paragraph per pool archetype that has BEATEN the deck, evidence-gated (deck130 S9).
- **181** a routing rule and a reporting ritual are different instruments; gate the ritual on a
  printed observable (deck146 S2; 20/84 with 0 misroutes).
- **182** the standing list of engine-literal absolutes that belong in EVERY guide (deck146 S3).
- **183** two-branch rules are symmetrically specified (deck152 S4; five held attackers).
- **184** a floor enumerates the engine-marked dead forms it does not reach (deck152 S6; 9 of 10
  "breaks" were correct play).
- **185** an alternatives list states its empty case with do-nothing promoted (deck162 S7; both
  breaks).
- **186** the review method counts NAMED CITATIONS as well as breaks (deck162 S8).

## What was REJECTED, and why

- **"Require the prose verdict to NAME a row before `decision_reversed_in_prose` counts" (seat-123-130
  H1, deck130 G8).** REFUTED on disk — reconciliation 2. Three of the five firings carry
  `answer_replaced: True` + `coded_answers: 2`, the engine had latched a SECOND coded line taking the
  row, and the re-ask reverted it to pass. The narrowing would suppress exactly the firings that
  changed the answer. Adopted instead: log the re-ask's reply and mark which coded line was latched
  (D12).
- **A blind "identical option set already declined this turn ⇒ reuse the decline" cache (seat-146
  E-2, deck152 G5a, seat-123-130 M1, deck123 G6).** REJECTED: **30 real actions** corpus-wide follow
  a decline on a byte-identical set in the same turn; `130v152` seq 12 -> 13 is two identical menus
  one phase apart, declined then cast. The model reads phase progression as information the menu does
  not carry.
- **"One ask per opponent turn, at their end step" (deck162 G9).** REJECTED twice over: it is a hard
  cap on legal windows (owner ruling), and it aims at the one stop that produced **0 casts in 78
  windows**.
- **"Do not open a window all of whose cast rows carry an engine-authored no-effect clause"
  (deck146 G2, deck125 G4, deck126 #6).** REJECTED: it removes a legal action from the menu (543 such
  rows, 0 taken — the ceiling is the model's, not the engine's), and D4 shows the clause itself can be
  stale against a pending stack object. Adopted instead: the model-owned HOLD row (D2) and, if that
  is not built, `[you declined this exact list N times already this turn]` on the row.
- **seat-146 E-1's two candidate mechanisms for the Soul Shatter window** (protection fizzle;
  coalescing copies). Both refuted — reconciliation 5. The item ships as a render/stack-labelling fix
  (D4), and **wave-51 lane A's PASS is NOT reopened**.
- **seat-125-126's reading of the five Chromatic Lantern `[second copy:` takes as dead casts.**
  Rejected against `mtg.txt:19630` — the Lantern has five own `{T}:Add` abilities. The dead-second-copy
  class is Intruder Alarm alone (4 of 14 rows). deck126 G7's completion form is adopted instead.
- **seat-123-130's PASS verdict on lane L's collapse.** Refuted — reconciliation 9; D8 FAILED on
  ability menus and the item is docketed (D7).
- **A guide compensation for any of D1-D5.** Trust doctrine: a guide describes cards and printed
  strings. deck123's A52-12 is accepted as a teach about the WINDOW lane I opened, not as a
  description of the cadence defect, and retires when D2 ships.
- **A fourth restatement of deck130's wall check.** Wave 51 said "the row form only if it fails a
  third time"; it did fail again (`130v146` s35), but the mechanism was the DECIDING SITUATIONS
  bullet dropping a branch (#177), not missing arithmetic — the guide fix (D52-3) ships first and
  R128/D25 stays LOW.
- **Any win-rate attribution.** deck162 5-1, deck125 5-1, deck126 1-5, deck152 1-5 on real hands,
  n=6 per deck. Counts only (#87, #167); the seats' own rotation calls are made on open lanes.

---

## Layer routing (`general-strategy.md`)

RENDER/CORE/ENGINE, ranked: silent zero-block after a `multiblock_reask` (R136) · the opponent-turn
window economy and the HOLD answer (R137) · the `[DRAW PRICE:]` lethal tail (R138) · the edict
forecast against a pending stack object + anonymous stack lines (R139) · the cast-row kill summary
(R140) · **MED**: the defender tap consequence (R141), ability-menu collapse (R142), the from-exile
cause (R143), the `(Pass)` echo (R144), timeout observability (R145), the `[second copy:` verdict
(R146), plan age + the re-ask's reply (R147), the converter folded into its rows (R148), a land
count in the battlefield header (R149), the converter's branch (R150), the draw-punisher tag (R151),
discard-list duplicate markers (R152), the `{feeds:}` K-0 clause (R153) · **LOW**: `Bn:Bm` dropped
silently (R154), the `(xN)` collapse for draws/counters/discards (R155), the payee (R156), the face
row's life (R157), the BLOCKS format line (R158), the `unparsed_reply` recovery record (R159), the
log-window decision (R160, the owner's).
Of R105-R135: **R105, R106, R107, R108, R109, R110, R111, R113, R114, R116, R117, R118, R120, R121
SHIPPED and PASSED**; R112, R115 and R119 shipped and each exposed a residual (R142, R143, R140);
R122-R135 carried, three of them narrowed by measurement (R124 7 -> 3, R127 2 -> 1, R126's shape did
not recur).
GUIDE-only: listed per deck in `general-strategy.md`.
MEASUREMENT: twelve items, headed by the opponent-turn phase table with consecutive-run lengths and
the three-number literal audit (#168).
PASS/KEEP: every lane G-L string in the ledger's Set B; **no render falsehood this corpus**.

---

## Guide boundary findings for the deck reviewers

Step 4 was run as a REPORT over the seven wave-52 `deckN/edit-texts.md` proposals and the live
`deckN_strategy.txt` files they amend (the reviewers' wave-52 guides are theirs to commit). Sweeps by
the seat files' own quotations and this step's reading of the edit texts: engine-behaviour vocabulary
(#99/#103/#136/#154), the hard-floor family, quoted render literals per row family with the
three-number audit (#168), the cross-guide KEY diff (#144), the Oracle/primitive class and effect
words (#145/#164), the mulligan single-property rule (#153/#171), the list-answer emit form
(#143/#152), and the new prose-vs-list derivation check (#177).

**TRUST DOCTRINE: CLEAN, all seven.** No guide teaches doubt of a rendered value. deck126's D69
(teaching that `- it cannot attack this turn` on a Defender identifies WHICH wall is tapped) is a
derivation from a true string about the pilot's OWN deck, not a hedge — PASS, with the obligation in
B4. deck123's A52-12 teaches the window lane I opened, not the cadence defect — PASS.

| # | file : site | amendment | finding | suggested wording |
|---|---|---|---|---|
| **B1** | `deck126` D72 (the 3+-land no-coverage mulligan sentence) | **#99/#136** | **ENGINE-BEHAVIOUR VOCABULARY IN A GUIDE.** The added sentence ends *"This shape reached you once this corpus and the engine answered it for you."* That tells the pilot the ENGINE decided a mulligan (it did — a 900 s timeout, docket D10) and is exactly the class #99/#103/#136 removed from every other guide. The rule itself (KEEP a 3+-land no-coverage seven) is correct and odds-form and stays. | Cut the last sentence. If a provenance note is wanted, put it in `edit-texts.md`, not in the guide. |
| **B2** | `deck146` 146-R (the KEEP line's colour clause cut) | **#118 / #133** | **THE REFUTED SENTENCE IS QUOTED VERBATIM INSIDE ITS OWN REFUTATION.** The AFTER text carries *"Keep 2-5 lands including a white source and a black source" is the sentence this seat quoted back to itself as its reason to ship a two-land seven*. The executor pattern-matches strings; a negated rule printed in full is still a printed rule, and this is the exact sentence the model already quoted once as its reason. The cut and the positive statement ("THIS LINE STATES NO COLOUR REQUIREMENT AND IT IS NOT A TEST") are right and sufficient. | Delete the quotation, keep the imperative: *"…is inside it. Colour is never a reason to ship — see COLOUR IS NEVER A REASON TO SHIP above."* |
| **B3** | `deck123` A52-1 / A52-2 / A52-3 and `deck126` D65 / D73 (the literal re-keys) | **#168** | **PASS — and this is the wave's proof of #168.** The wave-51 boundary pass B3 flagged the `[already owned:` / `[from exile]` re-key as an obligation and it was NOT done at deploy; deck126's Lantern rule then went **9/9 declines -> 0/5** with its text untouched. Both guides re-keyed this wave. Obligation for wave 53: report the THREE numbers per literal (old-string renders — expect 0; new-string renders; takes under the new string), not just "re-keyed". | none now; the reviewer brief carries the three-number form. |
| **B4** | `deck126` D69 (`{paying this taps: <wall>}` translated) | **#154** | **PASS with a standing re-key.** The paragraph is nine lines that exist only because the emitter states a vacuous consequence. When docket **D6** ships (`- it cannot block on their turn` for a `defender` source), the whole paragraph collapses to one sentence keyed to the new literal and the translation instruction must be CUT, not left standing beside it. | At D6: *"A cast row carrying `{paying this taps: Overgrown Battlement - it cannot block on their turn}` is telling you which wall you lose for their combat — weigh it against what the spell buys."* |
| **B5** | `deck123` A52-4 / A52-4b and `deck126` D65 (`[second copy:`) | **#169 / #177** | **PASS, and A52-4b is #177 found by the reviewer's own composition check** — the rule lived only in prose until a DECIDING SITUATIONS bullet was added. Obligation: when docket **D11** ships the non-stacking verdict, deck123's sentence re-keys to it and the guide's own "a second Alarm changes nothing" derivation is cut (#154). Note for deck126: its D65 reason must stay the LANTERN-specific one (the lord half is redundant, the `{T}: Add` half is not) — the primitive does not support a flat "a second Lantern is dead". | none now; add both re-keys to the wave-53 reviewer brief's #136 checklist. |
| **B6** | `deck130` D52-3 (the GANG BLOCK branch moved into DECIDING SITUATIONS) and `deck123` A52-4b | **#177** | **PASS — two independent instances of the same seam in one wave, which is what promoted #177.** Obligation: the wave-53 reviewers run #177 as a mechanical diff (every condition in a prose rule appears in its bullet) across ALL SEVEN guides before the corpus, not only on the rules that broke. | none now. |
| **B7** | `deck152` 152-M (first strike does not cancel a blocking trigger) | **#145 / #164** | **PASS — the card fact is right.** Perimeter Captain's primitive is `@combat(blocking) source(creature[defender]|mybattlefield):may life:2 controller`: the trigger is on BLOCKING, not on surviving damage, and the A-line already printed the priced outcome. This is a rules-reading correction of the model's own words, in the guide's voice, keyed to a printed string — the #174 shape one wave early. | none. |
| **B8** | `deck146` 146-Q / `deck152`'s MULLIGAN floor / `deck123` A52-10 / `deck126` D72 | **#153 / #171** | **PASS at all four, and the #153 audit's one remaining holdout is named.** seat-125-126 re-ran #131 on deck125's and deck126's sections at write time; deck146, deck152, deck123 and deck126 were all re-scoped this wave. **deck130's section is the holdout** — it has taken **0 mulligans in two corpora and 0 bottom asks in three**, so it is #132-UNTESTED, not passed, and it has never been checked for a second shippable property. deck123's A52-10 constructed only the two-land no-coverage case (the seat says so itself), so its twelve hands are not complete. | deck130's reviewer runs #153(a) then #171 on its section; deck123's completes A52-10's twelve hands. Everywhere else: for every sentence quoting an engine literal, add what the literal does NOT decide. |
| **B9** | `deck123` A52-12 (the opponent-turn window) and every guide's timing block | **#154 / D2** | **PASS — and a watch.** A52-12 teaches the window lane I opened and closes the dead-row loop; its own prediction (P10) is that correctness does NOT change (36/36 correct declines this corpus). The risk to watch is the sentence drifting into "do not answer these windows", which would be a guide encoding an engine cadence. When docket **D2**'s HOLD row ships, this paragraph re-keys to the row's own literal and the loop sentence is cut. | none now; wave-53 reviewers report opponent-turn declines and casts separately at every seat. |

**Not findings — checked so nobody re-files them.** deck125 W64 (the Fruition rule re-keyed to the
printed `[DRAW PRICE:]` clause): #173 exactly, and it stays correct after D3 ships — PASS.
deck130 D52-2 (the land gate keyed to the collapsed handle form): #178 — PASS. deck130 D52-9 (the
deck125 archetype paragraph): #180, evidence-gated by a 70-turn loss — PASS. deck130 D52-10 (the
punisher as a kill target): #179's offensive half — PASS. deck146 146-S (the empty-blocker-tag SEND
absolute ported from deck162): #182's first entry — PASS. deck146 146-T (the ceremony gated on
"the menu holds at least one other `Cast …` row"): #181 exactly — PASS. deck152 152-I (the rule's
trigger moved onto `{paying this taps:}`): #168/#184 — PASS. deck152 152-J (the one-name converter
branch given its own literal and imperative): #183 — PASS. deck152 152-K (the `[legendary:` carve-out
in the deploy floor): #184 — PASS. deck162 162-I (the two-row-ask empty-alternatives sentence, with
row 2 promoted): #185 — PASS. deck126 D67/D68/D74 (Exquisite Blood's direction as an always-false
sentence, the no-starter branch, and its DECIDING SITUATIONS entry): #174/#175/#177 — PASS, and the
primitive `@lifelostfoeof(player):life:thatmuch controller` was quoted correctly by two seats.
The 72c mood-word sweep: zero hits reported by any seat. Every guide's net size change is stated with
its trims in `edit-texts.md` (deck123 +6.0 KB net after five trims; deck130 +5.1 KB) — #146 honoured.

---

## For the next wave

The wave-53 docket is engine correctness at the top for the fourth wave running, and its shape has
moved again: **the top five are one silent execution failure, one cost curve, and three numbers the
render already computes but prints on the wrong screen.** D1 (a `multiblock_reask` that declares no
blocks and writes no record) is the only defect this corpus that lost a game by itself. D2 is the
bill for lane I's capability — 966 windows for 85 acts — and the ONLY remedy that survives the
records is a HOLD answer the model chooses; every cache and cadence proposal is refuted by 30 real
acts that followed an identical decline. D3, D4 and D5 are one-clause emitter changes with PARSETEST
cases and a suite pin each: the lethal tail on a draw price, the pending-sacrifice clause plus a
named stack line, and the kill summary on the refusable cast row. D6-D18 are the render's remaining
silences, and four of them (D6, D8, D11, D18) are re-keys guides are already waiting on (#154) — the
wave-53 reviewer brief must carry them in its #136 checklist BEFORE the corpus.

The guide lane's work this wave is the three audits the corpus proved, all run BEFORE the corpus:
the three-number literal audit on every quoted string (#168 — the wave-51 obligation that was skipped
at deploy and cost deck126 its Lantern rule), the prose-vs-DECIDING-SITUATIONS diff on all seven
guides (#177 — two independent instances in one wave), and #171's scope sentence on every guide
sentence that quotes an engine literal (four mulligans in two guides). deck162 is exit-eligible if
162-I holds and D18 lands; deck146 needs Q/R/S to hold; deck152 has three lanes open and is the deck
with the most guide left to do; deck125's two unpunished gates (Staff 4/11, Elixir 2/11) are on
borrowed time — lane I is what kept them free, and they will be punished the first corpus a window
closes; deck126's two levered losses are now four guide sentences and one core-prompt ask, and its
`vs125` game is the corpus's clearest argument that a combo guide needs a no-starter branch;
deck123's entry-0 Damnation lane is at its third corpus with the plan-text half still 0/2 — if the
plan text passes and the cast still misses, the guide lane is exhausted and the answer is a row
annotation; deck130's two floors (face damage, the four-land gate) are its whole difference between
a grind and a loss.

**Specimen for the owner's per-wave prompt review (invariant 0).** **Keep the harvest default** —
`wave52/lategame-specimen.txt` = `1788320112-…deck125-…-vs-…deck130.jsonl` seq **149**, turn **70**,
Main phase 1, 22,643 chars, 4 rows: the deepest turn in the corpus, and it carries three of this
wave's new forms on one screen (`[second copy:`, the X-pricing block, the sweeper's self-hit count).
**Recommended alongside it: the engine seat's render-dense companion,
`1788320091-…deck123-…-vs-…deck126.jsonl` seq 88** (turn 26, 15,761 chars, 6 rows, the seat at 5 life
vs 29 with 34 creatures) — it exercises eight annotation families to the default's four, at a size a
human can read end to end, which is what the owner's criterion (decision value per token) is measured
on. The size ceiling, if he wants the worst case instead, is `…deck146-…-vs-…deck152.jsonl` seq 107
(31,551 chars, 79% log) — the exhibit for the D27 log-compaction decision, which is his call and no
one else's. The engine seat's third candidate (`146v152` seq 107) and seat-146's (`146v125` seq
320-336) are the same two exhibits; seat-146's is the D4 defect trace and belongs to the lane, not to
the prompt review.
