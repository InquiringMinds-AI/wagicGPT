# Wave-55 general strategy — RENDER / CORE-PROMPT proposals, layer-routed (R1-R224 revised, R225-R258 new)

Corpus `matchups-20260903-074354` (TWELFTH fair-hand corpus), binary master **b228b8648**
(wave-55 step-1 lanes A 8fe5c6682 / B f7d13d99d / C 38ccaae55 / D 8f9ee5736 / E d56efcdaf on base
8a1768d1c = wave-54 step-1 + P purge + Q autotap search + **R stall-floor hotfix**; audit lanes
G-N carried. Gate on b228b8648: suite 1229 with the 2 known concurrency-only failures
(`lifeline`, `merrow_reejerey`), 51 AI/0, 0 timeouts, PARSETEST 2636/0). Pilot **qwen36-35b-a3b**
on Spark :8084.

42 seat logs, **3,264 records** = 42 `gamestart` + **42 `gameend`** + 7 `recovery` + 2 `wall_miss` +
**3,171 decisions** (ask 2,416 / priority 555 / attackers 90 / discard 68 / blockers 21 / reveal 12 /
bottom 9). **21/21 GAMES NATURAL — invariant 00 SATISFIED**: 0 timeouts, 0 crashes, 0 hangs, 0
adjudications, 0 `Segmentation`. **14 decision `fallback` fields = 0.44%** (`plan_choice_conflict` 9,
`named_row_reask` 3, `stale_echo` 1, `unparsed_reply` 1 — **no `all_assignments_illegal`, no
`stale_livelock`, no `empty_reply`, no `timeout`**) plus 2 on the non-decision `wall_miss` records.
**Baka executed 2 decisions = 0.063%**; `giving this decision to the heuristic` **0**. `choice: -1`
**5 decisions + 2 `wall_miss` = 7, and 7 `recovery` records, 0 unpaired.** Max seat log 318
(`152v125`, 64 turns); max prompt **30,031** (`152v125` seq 314); **1 prompt over 30,000** (was 2),
252 over 25,000 (was 37, on 43% more decisions). Total inference **23.48 h over all positive
latencies / 22.98 h over decisions only** = **26.1 s per decision** (two units, both stated, #208).

Sources: the four seat files, seven `deckN/general-suggestions.md`, seven `deckN/skill.md`, seven
`deckN/edit-texts.md`, `wave55/lane-{A,B,C,D,E}.md`, `wave54/lane-{P,Q,R}.md`, and the corpus —
**every load-bearing number below was re-derived on disk by this step** with python over the JSONL,
**counting rows from the rendered `prompt`** (#240), and `/usr/bin/grep` over the 21 `.stderr`.

**Routing rule (owner doctrine 2026-08-26).** The core loop — engine mechanics, LLM UX, bugs — is the
purpose. Everything below is a change to what the ENGINE RENDERS, what the PARSER executes, or the
CORE PROMPT's own text. Guide-side answers are named only where a rule must stay correct after the
render ships. Engine BUGS with call sites are in `engine-ledger.md` (the wave-56 docket); this file
is the ranked routing view with the evidence and the wave-56 falsifier for each item.

**Governing constraint.** Legality is enforced structurally; the model's CHOICE is never constrained.
Every proposal ADDS a true token, a true row, a receipt, a stamp or one re-ask, and DELETES nothing.
**Hard caps on legal choices are rejected on sight**; four are rejected below. Standing owner
rulings honoured throughout: X menus largest-first, history lines owner-prefixed, and **declines LAST
on the menu** — which is why R249 puts a number INSIDE the HOLD row rather than moving the row.

**THE INSTRUMENT NOTICE, and it governs how every per-window rate below is read.** Lane R discharged
wave-54 D1 completely: stall-floor releases **0** (was 468), `interrupt window held` **0** (was 936),
`dropping stale async answer` **43** (was 488), `giving this decision to the heuristic` **0** (was 1).
**The window economy is uncontaminated for the first time since wave 53**: opponent-turn share of
decisions **19.9% -> 33.8%**, opponent-turn casting windows 403 -> 890. Every wave-54 rate marked
UNTESTED (denominator moved) is therefore scored normally here. The one metric that is NOT an engine
number is per-decision inference: see **R257**.

---

## STATUS OF R1-R224, by the emitter's actual string

Verdicts carry from `wave54/general-strategy.md` except where a wave-55 lane touched them.

| item | wave-55 verdict | evidence (re-derived on disk) |
|---|---|---|
| **R193** the stall floor | **CLOSED — lane R, and it is the wave's largest single change** | 0 releases, 0 `interrupt window held`, 43 drops, 0 heuristic handoffs. Decisions 2,212 -> 3,171 (+43%); opponent-turn share 19.9% -> 33.8%. Residual 43 drops -> **R237** |
| **R194** the life-loss loop de-dup key | **HALF SHIPPED, HALF FAILS — and it is now wave 56's HIGH #1** | (a) the option-SET key WORKS: the declined note counts unbroken to **27** across 31 windows whose per-row life annotations moved every iteration — the exact thing that defeated the wave-54 key. (b) the HOLD key's life drop WORKS: **0 of 113** hold-then-re-ask pairs re-opened on a life change alone. But the loop is unchanged: `123v126` s75-s87 is **13 windows in one Upkeep** (life 20 -> 2), `123v162` s36-s50 is **15 in one Draw step**, and lane A's "< 5 per loop" **FAILS**. Cause: the hold's re-open predicate is still the BOARD key, and the loop's stack line alternates `Sanguine Bond's Life Loss` / `Exquisite Blood's Life` every iteration — **R225** |
| **R195** `all_assignments_illegal` with no re-ask | **UNTESTED (N = 0)** | the class never fired; all 21 blockers records carry `dropped_assignments: 0`. Lane B's re-ask arm and its `_exhausted` branch are both un-executed. The machinery DID fire on the sibling path (`[RE-ASK]` 12 renders, 3 `named_row_reask` all recovered, 0 wrong casts from a named-row reply) — and 2 of the 8 CHOICE-line notices quote the wrong coded line: **R227** |
| **R196** the HOLD row's own-turn exclusion | **CLOSED — PASS** | 2,352 renders on decisions (2,354 windows incl. `wall_miss`), **own-turn 1,323 / opponent-turn 1,029**, LAST row 2,352/2,352, both retired strings **0**. Longest own-turn identical run **12** (was 50); own-turn runs >= 20: **0**. Takes 282 = 12.0% (was 43 = 10.1% on an opponent-turn-only denominator) |
| **R197** the reveal livelock | **PASS 0** | `stale_livelock` **0** corpus-wide (was 1); no reveal parked; `force-close` 0 renders, still **UNTESTED and now for the right reason**. The stamp lane E shipped alongside it is a telemetry defect — **R236** |
| **R198** the X menu's absent marker | **PASS IN LANE C'S SCOPE, and the residual is a NEW family** | 14 X menus; **3 kill-priced, 3 marked, 2 of 3 marked rows taken**; both marker strings verbatim, both collapsed-run marks on the BOTTOM row with the kill lists restated. Takes of a `kills THEIRS: none` row while another affordable row named an opponent creature: **0**. The other **11 are monotone Sphinx's Revelation menus** with no marker family that fits — **R231**, and see skill #260 |
| **R199** the `target=player` half-priced verdict | **CLOSED — PASS 0/151** | every Devour Flesh row prints both halves; `targeting yourself` on 151 windows, the commit clause `{this row does not pick a target yet ...}` on **217 windows / 219 rows**, and the wave-54 model's 900-word prose question is answered on the row. 6 takes. **The sibling class is open**: `target=anytarget` damage rows (Spark Spray, Hammer of Bogardan) still enumerate both players and price one — **R231's cousin, filed under R226's family in the ledger as D2's neighbour** |
| **R200** the 2-D ability-menu collapse | **CLOSED — PASS** | decode line `... as the source instead of #1 ...` on **123 windows / 137 rows**; **0 records with more than 3 rendered rows of one `#N` shape**; largest rendered menu 21 rows. The wave-54 "403 redundant rows" was an `options_text` artifact (lane D's own correction, skill #240); the real class was the copy-tag grammar and it is collapsed on 123 windows |
| **R201** the discard ask's missing verdicts | **PARTIAL — the three clauses shipped and 84.8% of rows are still bare** | `{spare: you control N lands already}` **44 rows / 25 windows**, `{dead right now: ...}` **31 / 25**, `{you already control one}` **5 / 3**. Rendered discard rows **525**, bare **445 = 84.8%**. The land half WORKED: cleanup discards sending a counterspell past a listed spare land **0** (was 3 of 4) across 114 counterspell rows and 44 spare-land rows. **R228** |
| **R202** the `[named:` cast row's missing price | **UNTESTED (N = 0)** | `[named: <card>]` tags render on 83 windows; `[NAMED BY THEIR` renders **0** — in no window did a seat hold a `Cast <that card>` row. Carried unchanged; the tag itself is byte-identical and stays |
| **R203** `pass_hold_ambiguous` | **CLOSED — PASS 0** (was 16) | the exact-label tiebreak works; `hold_row_named` 5, all five `CHOICE: 0 (…)` naming the HOLD row and all five executing it, each co-stamped `echo_index_conflict`. Name-over-index wins where it should |
| **R204** the coverage clause's two families | **PASS 0 of 2, thin N** | 3 clause windows: 1 one-land -> `... alone on turn one` (KEPT); 2 at 4 and 2 lands -> `... alone even with every land in this hand in play` (both KEPT). The four-corpus colour-family mulligan break did NOT recur. Boundary debt: three guides quote the clause WITHOUT the new scope tail — B3 |
| **R205** the blockers menace header | **UNTESTED (N = 0)** | 21 blockers windows, **0 with a menace attacker**; `need TWO or more blockers each` and `[menace -` both render 0. The negative arm PASSES 21/21 (silent when it should be silent) |
| **R206** the pruned-pair record | **UNTESTED (N = 0)** | no `all_assignments_illegal` record exists |
| **R207** `{kills:}` ownership | **CLOSED — PASS** | `{kills whichever you target: THEIRS - ...; YOURS - ...}` **215 windows / 223 rows**, `{removes whichever ...}` 19. The wave-54 `130v146` s23 repro renders correctly. Residual: the conditional `{kills N of the ...}` form still has no split — **R252** |
| **R208** ability-row life totals | **SHIPPED, AND EVERY NUMBER IS FALSE** | the arithmetic renders on 100% of the rows (two seats scored it PASS on presence) and **238 of the 248 `they would be at K` rows compute K from a constant base of 166** — `2 damage -> at 164` (176 rows), `1 damage -> at 165` (61), at every real opponent life 34 down to 1. **R226**, and skill #259 |
| **R209** fetch-row colours | **STRUCK from the docket, and REGRESSION-PINNED** | lane C re-derived wave 54 on disk: 24/24 fetch rows already carried the colour clause; the "21 colourless renders" was wave 53's number restated. This corpus: 60 rows carry `and it adds {W} or {B} (you cannot make {W} right now)`, 7 takes |
| **R210** reveal-menu duplicate collapse | **CLOSED — PASS** | the copy-tag collapse reaches reveal AND cleanup-discard menus; `(copies k-m of n ...)` on 123 windows / 200 occurrences; the wave-54 50-row uncollapsed Tutor menu renders as ranged rows inside a 9 KB prompt. Residual identical rendered rows corpus-wide: **6 in 4 records** (target <= 12) |
| **R211** the own-turn declined-note | **CLOSED — PASS** | the note renders on **1,212 windows**, reaching own-turn Main 1 and Main 2 (the `castDeclineRow` phase clause is now out of the key). Max N reached **27** |
| **R212** the `{feeds:` conditional converter | **NOT SHIPPED — carried** | no lane took it. deck162's guide still hand-carries the qualifier. **R238** |
| **R213** the HOLD row's benefit clause | **SHIPPED 2,352/2,352, and the rate did not move where no guide named the row** | `{taking this row skips the rest of this turn's identical windows}` on 100% of HOLD rows. Takes in declined-note N >= 3 windows **67 of 753 = 8.9%** against lane A's >= 20% target: **FAILS corpus-wide, PASSES at deck123 (46 of 123 = 37.4%)**, and the spread is guide-shaped, not render-shaped — see the RULING below and skill #233. `hold_windows_skipped` sum **735** (target > 72), non-zero on **21 of 42** gameends (was 7) |
| **R214** the `chooseaname` header | **CLOSED — PASS** | old header `Choose one mode for` **0**; the new card-NAME header on **5 windows**, `{visible now: ...}` on 5 windows / 17 rows, public zones only, no hidden-zone leak. Narration `You named X with ...` present; 20 of 98 lines fall back to a generic source label — **R251** |
| **R215** the unrecorded wall miss | **CLOSED — PASS 3/3** | 3 `no reply after 900s` events: 2 wrote `kind: wall_miss` records (`162v146` s15, `130v125` s25) and 1 landed as `wall_miss: 1` on the consuming record (`126v162` s24). `gameend` carries `wall_miss_events` (sum 3) and `wall_miss_unrecorded` (sum 2) on 42/42 |
| **R216** the `(xN)` log collapse | **WORSE — 514 duplicate event lines inside single records** (was 471) | `- Phase: Draw` 230, `- Opponent drew a card` 101, `- Phase: Main phase N` 80. GAME LOG is **58.1%** of the mean prompt. **R250** |
| **R217** payee-less receipts | **PASS** | `- Paid` receipts render throughout; **`with no source tapped` 0 in all 21 stderr files**; the remaining no-payee shapes are the legitimate `from mana already floating` form |
| **R218** resource 404s | **IMPROVED — 1,725 lines over 37 distinct** (was 1,927 / 118) | every remaining miss is an audio file (`mana.wav` 105, `land.wav` 105, `creature.wav` 91, `basic.wav` 88) plus `thumbnail`. The image-cycling loop is gone. This is the silent `JSfx` stub, not lane J — **R253**, LOW |
| **R219** the generic effect label | **PASS on the stack line** | `ability: X's X` **0 of 1,103**; 566 lines carry `[triggered/activated ability]` AND name the effect. The generic fallback moved to the NARRATION — **R251** |
| **R220-R223** (wave-54 MED/LOW carried) | **carried unchanged** | no lane took them; re-measured only where a wave-55 item supersedes |
| **R224** the log window — OWNER | **carried, and the cost story is now inference, not bytes** | prompts > 30,000: **1**; mean prompt 10,740 -> **12,174 (+13.4%)** on 43% more decisions; total volume 23.8 M -> 38.6 M. See **R257** for why a byte trim would not buy latency at this concurrency |
| **R1-R192** | **carry from wave 54** except where a row above supersedes | no wave-55 lane touched them; the ones with live denominators are re-measured in the ledger's discharge tables |

---

## THE RULING THE BRIEF ASKED FOR: the HOLD row's take rate is FIVE rates, and the split is engine on one side and guide on the other

Lane A gave the row **complete coverage** — 753 of 753 of the declined-note N >= 3 windows carry it,
own-turn included, always last. Corpus-wide adoption is 8.9%. Split by what each deck's guide (the
wave-54 text the corpus ran) actually said, over N >= 2 windows:

**deck123 54/141 = 38.3%** (names the row and COMMANDS it: "take that row at the FIRST dead window of
the turn, not the fifth") · **deck130 8/48 = 16.7%** (quotes the declined-note literal, no imperative
on the row) · **deck125 25/381 = 6.6%** (names the row and calls it optional: "Take it or take 'Cast
nothing right now' - both are correct") · **deck126 1/155 = 0.6%** and **deck146 + deck152 + deck162
0/190 = 0.0%** (say nothing about the row).

**Ruling.** The 8.9% is a mixture, the ordering is monotone in the strength of the guide's sentence,
and **the residual after lane A is GUIDE-SIDE** — which is what seat-146-152-162 argued, and its
number is exact (190 windows, 0 takes, re-derived). Its stated MECHANISM ("the pilot takes the row
its guide names") is **too weak**: deck123 and deck130 quoted the *retired* wave-54 string, which
renders 0 times in this corpus, and still converted at 38.3% and 16.7% — the pilot follows the
DESCRIPTION and its imperative, not the quotation (skill #264). deck125 is the decisive cell: it
names the row, has the most windows, and converts at one sixth of deck123's rate because its sentence
grants permission instead of issuing an order (skill #233). All seven wave-55 guides now carry a HOLD
block; five of the seven are new, so wave 56 measures the same five cells against this table.

**What the ENGINE still owes on this seam, and it is a different number.** Of the 282 takes,
**113 (40.1%) were followed by at least one byte-identical same-turn re-ask — 301 re-asks in all**
(deck123 59/106, deck130 20/64, deck125 29/83, deck152 4/16, deck126 1/4, deck146 0/7, deck162 0/2 —
seat-123-130's 79 of 170 at its pair reproduces exactly). **R225.**

---

## NEW — ENGINE, HIGH

### R225 — the HOLD latch's re-open predicate is the BOARD key while the model's answer was about the OPTION SET, so a stack object that leaves every rendered row byte-identical re-opens a window the model has already closed. (= ledger **D1**; seat-123-130 **E1** phenomenon confirmed, mechanism corrected; lane A's D2/D4/D21 residual; seat-125-126 **MED #3**; seat-146-152-162 headline 1's engine half)
**Repro.** `123v126` seq **75-87**: 13 consecutive decisions, one turn, one Upkeep, life **20 -> 2**
and opponent 23 -> 41 three points at a time, the two-row menu byte-identical throughout, the
declined note running 15, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27. **The HOLD row was taken at
s81, s82, s83, s84 and s85 and stopped nothing**, because the stack top alternates
`ability: Sanguine Bond's Life Loss [from their Sanguine Bond]` /
`ability: Exquisite Blood's Life [from their Exquisite Blood]` on every iteration and the hold key
contains the stack line. Mirror at `123v162` seq **36-50**, 15 windows in one Draw step, life 10 -> 1,
the stack top cycling `Underworld Dreams` / `Fate Unraveler` / `Howling Mine`.
**Scale, re-derived.** 113 of 282 HOLD takes are followed by an identical same-turn re-ask, 301
re-asks total. Identical-declined runs of >= 3 (key `(turn, options_text)`, any decline including a
HOLD take): **798 of 3,171 = 25.2%, 111 runs, max run 16** — and **80 of the 111 runs have a VARYING
stack top while the rendered rows never change**; 74 contain a triggered-ability stack top. Those
runs consumed **2.91 h = 12.4% of the corpus's 23.48 h of inference.** 8 of the 43 residual stale
drops are preceded by the engine's own `hold re-opened ... the board changed` line, so the predicate
also throws away in-flight answers.
**What lane A got RIGHT and must not be reverted.** The option-SET key works (the note counts
unbroken to 27 across annotations that move every iteration) and the life drop works: **0 of the 113
re-opens is a life-only change** — every one carries a real stack, battlefield, hand or library
change. seat-123-130's stated mechanism ("a life total moving between windows re-opens the
byte-identical option set") is **REFUTED on disk**; its cited repro `123v125` s104 -> s105 re-opens on
a stack top and a hand-size change, not a life tick. Its REMEDY is the right one.
**Ask.** Make the hold's re-open predicate the same key the declined-note already uses: the RENDERED
option rows. If every printed row — name, cost, ordinals AND its `{right now: ...}` price — is
byte-identical to the set the model held, the hold stands; anything that changes a printed row
re-opens it immediately. This removes NO row and caps NOTHING: it honours a choice the model made by
pressing a row, and it is strictly narrower than "any board change" only where the board change could
not have altered a single thing the model was shown.
**Stated risk.** A change that alters the answer's VALUE without altering any printed row (an
incoming lethal attack while the menu is unchanged) would not re-open the window at that seam. Bounded
by construction — the model already declined that exact screen, and the turn boundary always retires
the hold — but it is the reason this is a re-open predicate change and not a suppression.
**Wave-56 prediction.** Decisions inside a single-phase mandatory life-loss loop **< 5** (this corpus
13, 15 and 16); HOLD takes followed by a byte-identical same-turn re-ask **< 20 of N** (was 113 of
282); identical-declined runs **< 15%** of decisions and max run **< 10** (was 25.2% / 16); inference
inside those runs **< 6%** of the corpus (was 12.4%).
**Falsifier.** The predicate ships and the runs do not shrink -> the re-opener is not the key and the
remaining windows are genuinely new questions; then the item is the loop's own priority grant.

### R226 — every ability row that points damage at the opponent prints a life total computed from a CONSTANT 166: 238 of 248 rows, at every real opponent life. (= ledger **D2**; seat-123-130 **E2 HIGH**; lane C **D16** landed with the wrong life source)
**Repro.** `130v152` seq 6, option 2:
`Deal 2 damage with Pyrite Spellbomb targeting the opponent {right now: takes 2 damage - they would
be at 164}` on a screen whose CURRENT SITUATION reads `Your life: 20 | Opponent life: 20`.
**Re-derived, and the pattern is exact and total.** 248 rendered rows carry `they would be at K`.
**238 are ability rows and every one implies a base of 166**: `2 damage -> at 164` on 176 rows,
`1 damage -> at 165` on 61, plus one more, across `Pyrite Spellbomb` (154), `Staff of Nin` (61),
`Siege-Gang Commander` (23), at opponent life values from 34 down to 1, with **no variance**. The
**10 correct rows are TARGET-MENU player rows** (`The opponent (player, life 27) {right now: takes 3
damage - ...}`), a different emitter, and the SPELL cast-row tail
(`- and 1 to the opponent at life 15 leaves them at 14`) is also correct. **The defect is confined to
the ability-row branch.** Primitives verified: Pyrite Spellbomb `mtg.txt:90168`
(`{R}{S}:Damage:2 target(anytarget)`), Staff of Nin `112425`, Siege-Gang Commander `105071`.
**Why HIGH.** By the trust doctrine the rendered statement is an instruction: a pilot reading the row
can never see a lethal face row from an ability menu, and deck130's entire face-damage rule is a
comparison between `Opponent life:` and a row's damage. 123 of one seat's 873 windows carried the
false number. **No guide sentence hedging against it exists or may be written** — route around or fix
the surface, never teach doubt.
**Wave-56 prediction.** Rows printing `they would be at K` where K != `opp_life` - damage: **0 of N**
(this corpus 238 of 248). A lethal face row reached from an ABILITY menu at least once.

### R227 — the `[RE-ASK]` conflict notice quotes the FIRST coded line while naming the LATCHED line's row number, and one such notice converted a legitimate cast into a pass and was recorded as a success. (= ledger **D3**; engine seat **HIGH #2**)
**Repro, verified on the records.** `130v123` seq 109 (turn 26, Upkeep, priority, 6 rows): the reply
carries two coded lines — `CHOICE: 0 (pass)` first, then 400 words of reasoning ending
`CHOICE: 5 (cycling with Starstorm)`. The engine latched the second (`coded_answers: 2`,
`latched_coded_line: 2`) and executed row 5 — correct — then stamped `plan_choice_conflict` and
re-asked with:
`[RE-ASK] Your CHOICE line takes row 5 ("CHOICE: 0 (pass)") but your reply says this window is a pass
("I will pass"). Answer again: ...`
The row number comes from the latched line and the quoted string and the prose evidence come from the
DISCARDED first line. Seq 110's whole reply is `CHOICE: 0 (pass)`, and the record carries
`plan_choice_conflict_recovered` — a false success. Second firing, same shape: `152v123` seq 78
(`takes row 2 ("CHOICE: 1 (…)")`, model then answered row 1).
**Denominator.** 12 `[RE-ASK]` notices corpus-wide: 8 of the CHOICE-line form (**2 mismatched, and
both are exactly the records where `coded_answers = 2` and `latched_coded_line = 2`**; the other 6
have `coded_answers = 1` and 6 of 6 recovered the model's real row), 3 of the
`"<name>" is not on this list` form (all 3 recovered), 1 repeat-count form.
**Ask.** Quote the LATCHED coded line, and take the prose evidence from the same region of the reply.
**Wave-56 prediction.** `[RE-ASK]` notices whose quoted `CHOICE: n` index differs from the row number
the same sentence names: **0 of N** (this corpus 2 of 8); `plan_choice_conflict_recovered` records
whose recovered row is a pass while the latched row was live: **0** (this corpus 1).

## NEW — RENDER, HIGH

### R228 — the `discard` ask is annotated for lands and for zero-target removal only, so 445 of 525 rendered rows are still bare — and the evaluator that would price them is already running on the cast rows of the same turn. (= ledger **D5**; deck125 **HIGH #2 / G2**, deck130 **G10 / E4**, seat-123-130 **E4**)
Lane D's three clauses shipped and the land half WORKED: `{spare: you control N lands already}` 44
rows, `{dead right now: 0 legal targets on the board for it}` 31 rows on exactly the targeted-removal
family (Path to Exile 13, Tragic Slip 9, Vanishing Verse 3 — **never a counterspell**, the withholding
rule genuinely exercised against 114 counterspell rows), `{you already control one}` 5 rows; and
**cleanup discards sending a counterspell past a listed spare land went 3-of-4 to 0**.
**What is still bare, with the verdict the engine already computes for the same card's CAST row:**
a Supreme Verdict or Final Judgment while the opponent header reads 0 creatures (`{right now:
destroys 0 of their creatures}` on the cast row) · a Lightmine Field at 0 attackers · an Essence
Scatter against a board of walls · an Emrakul below its land count · an edict at `they control 0
creatures` (`at 0 this does nothing` on the cast row) · a Damnation on an empty board · an Idyllic
Tutor whose only target is already on the battlefield. `123v125` seq 48 renders three of these on ONE
menu: rows 2-4 (`Tragic Slip x3`) correctly carry the dead verdict while a Tribute at 0 creatures, a
Tutor with its target owned, and a Damnation on an empty board carry nothing.
**Ask.** Run the SAME dead-verdict family the cast rows compute over the discard menu, so that the
ABSENCE of a verdict means "this card is live" — which is the rule a pilot most wants at that ask and
which no guide can currently teach.
**Wave-56 prediction.** Rendered `discard` rows carrying no annotation of any kind: **< 20%** (this
corpus 84.8%); cleanup discards sending a card the engine has priced dead ahead of a live one: **0**.

### R229 — no window during the opponent's combat prints the incoming damage total: 50 windows, 7 of them at or past lethal, all in the game that turned on it. (= ledger **D6**; seat-125-126 **HIGH #1**; skill #255)
**Repro.** `125v146` seq 32 (Main 1) and **35-41** (Attackers + six Blockers windows), turn 13,
`my_life` **10**. The opponent line prints `3 of them are attacking right now` and then each attacker
with its live P/T — `Barrowin of Clan Undurr (5/5) ...[tapped - attacking]; Goblin (2/2) ...[tapped -
attacking]; Nadaar, Selfless Paladin (4/4) [vigilance] [attacking]` — **eleven power against ten
life, and nothing in the prompt says eleven or says the seat is dead.** The seat controlled no
creature, so no `blockers` ask with its `you would be at N` header was ever generated for it; the
combat reached it only as `priority` windows where that header does not exist. It declined a Sphinx's
Revelation (`max affordable X=2` = 12 life) at every one of the seven and died at -1.
**Re-derived denominator.** 50 non-blockers windows corpus-wide have attackers declared against the
seat; **0 carry an incoming total**; **7 are at or past lethal** by the sum of the attacking
creatures' printed power.
**Ask.** On ANY window during the opponent's combat:
`INCOMING THIS COMBAT: 3 attackers, 11 unblocked damage - you would be at -1.`
with the blockable/unblockable split when the seat has bodies. It is one line, it reuses arithmetic
the blockers ask already does, and it is the `[DRAW PRICE: ... - you would be at K]` device applied to
the one number that ends games. Every other price in this engine is a finished subtraction.
**Wave-56 prediction.** Windows during the opponent's combat with attackers declared and no printed
incoming total: **0 of N** (this corpus 50 of 50).

## NEW — MED

### R230 — `chosen_text` is empty on 9 records that carry a live `choice`, and a record whose executed row is unnamed cannot be adjudicated by any later reviewer. (= ledger **D4**; seat-123-130 **E3**)
9 of 3,171 decisions, all on the re-ask path (`146v152` s17, `123v152` s56, `152v123` s77,
`123v126` s25 + one, `123v146` s16, `130v123` s109 + s122, `162v126` s33). The re-ask machinery is
right to refuse; the record is wrong to forget. **Ask:** `chosen_text` is always either the executed
row's text or an explicit reason string (`"<refused: named_row_not_offered>"`), never empty while
`choice >= 0`. Instrument quality: a harvest predicate keyed on `chosen_text` silently under-counts
every class that passes through a re-ask.

### R231 — the monotone / no-kill X family carries no marker and no mana-fit clause, so the one real trade-off on the menu is invisible. (= ledger **D7**; engine seat **HIGH #3**, RE-SCOPED — see skill #261)
14 X menus: 3 kill-priced (all marked, lane C PASSES in its own stated scope) and **11 Sphinx's
Revelation menus with no marker of any form**, because all three of lane C's marker families rank a
kill dimension this card does not have. Primitive: `mtg.txt:110851`, `auto=life:X && draw:X`,
`mana={X}{W}{U}{U}` — strictly increasing in X, cost is mana only. The rows read
`X = 12 {X pricing: X=12 - you gain 12 life and draw 12 cards}` down to `X = 0`, and — unlike every
cast row in the corpus — **carry no `{leaves N sources}` / `{taps you out}` fit clause**, so the one
real trade-off (holding up a counterspell) is not on the screen at all.
**The take pattern is NOT the evidence for this item.** The pilot answered `X = 3` on 6 of 6 menus
where a larger X was affordable and took the max on 5 of 5 menus topping out at X <= 3 — and
deck125's guide carries a section headed `=== SPHINX'S REVELATION - X IS ALWAYS 3 ===` ending
"reply with the OPTION number whose line reads 'X = 3'. Every time, and with no arithmetic in front
of it." That is a guide rung executing perfectly (the deck seat scored it 11/11 correct), and the
engine seat's proposed wave-56 experiment is unnecessary.
**Ask, reduced to what the render still does not say:** (a) carry the `{leaves N sources}` fit clause
onto X rows as it is carried onto every cast row; (b) a fourth marker family for a monotone menu,
e.g. `[<- largest affordable X - X=12 gains 12 and draws 12; no listed X does more]`, which states a
fact rather than a recommendation; (c) collapse the identical middle of a monotone run the way lane D
collapses `#N` runs (13 rows for a one-dimensional choice).
**Wave-56 prediction.** X rows carrying no mana-fit clause: **0** (this corpus 11 menus of 11).
No prediction is made about the taken X: that is deck125's rung.

### R232 — an MDFC's land back face is never offered as a land drop, and the row that names it says so. (= ledger **D8**; seat-146-152-162 **E-1**)
deck146 runs two Emeria's Call and one Agadeem's Awakening. Primitives (`borderline.txt:33968`,
`:1177`) gate the land face behind `restriction{can play land,compare(isflipped)~equalto~1}`, and
nothing appears to set `isflipped` from hand. **Re-derived over all 3,171 decisions: 0 land-drop asks
name either back face**; the only rows naming them are `Flip Side with Emeria's Call -> DISPLAY
TOGGLE only (this card's other face is a LAND) ...` (15 rows) — an honest render of an engine
limitation, and those rows also generate windows the seat can do nothing with (wave-54 D23's
auto-passed window was one). **Repro:** `146v130` seq 22/23/24 — turns 21, 23 and 25 with the
battlefield line at `2 are lands: Plains #1; Plains #2`, an Emeria's Call in hand, and **no
`Land drop:` ask issued on any of the three turns**; the seat spent nine turns at 2-3 mono-white
sources holding five black spells and lost 19 -> 0. **Ask:** pin whether `isflipped` is ever set for a
card in hand, or give the land face a separate `anyzone` hand entry. **Guide obligation on the fix:**
deck146's "NOT LANDS FOR YOU" block is deleted in the same wave (its own P10 exists to detect this).

### R233 — an enumerating render clause with no quantifier was read as an effect list, twice, in consecutive plans. (= ledger **D9**; seat-146-152-162 **E-2**, deck146 **R-2**; skill #249)
`146v123` seq 28 row 2: `Cast Soul Shatter {2}{b} {right now: 24 tied at MV 0 (their highest):
Human #1; Human #2; ...}`. Primitive `borderline.txt:106069`:
`sacrifice notaTarget(...manacost=convertedcost:highest...)`,
`text=Each opponent sacrifices a creature or planeswalker with the highest mana value ...` — **one
card**. The seat's plans at seq 24 AND seq 28 both read *"cast Soul Shatter to sacrifice all 24 Human
tokens, wiping their board"*. Lane C's D15/D16 added the commit clause to both-player edicts; the
missing piece is a COUNT on the tie list. **Ask:** `{right now: they sacrifice ONE of these 24, their
choice - all tied at MV 0 (their highest): ...}`.

### R234 — the opponent's open mana is not printed as a number. (= ledger **D10**; seat-146-152-162 **E-4**, deck152 **R-1**)
Every prompt lists the opponent's permanents with `[tapped]` markers and the pilot must count the
unmarked ones; our own side gets `Mana available: N total (untapped sources ...)` with the sources
enumerated. `152v125`: **eight creature spells countered across turns 25-51**, each cast with 5-8
unmarked opponent permanents on screen and their hand at 5-7; the one cast made at 2 unmarked (seq
101, Tovolar's Huntmaster 6/6) resolved. The seat led 34-11 on T19 and lost at -11 on T65. **Ask:**
`Their untapped sources: N (colours they could make: {u}{w})` under the opponent battlefield line —
a derived fact the engine already has, and the number the wave-55 deck152 posture (152-B) now makes
the pilot compute by hand.

### R235 — a served PLAN is carried alongside the note that retracts it, and plan staleness has quadrupled. (= ledger **D11**; seat-146-152-162 **E-6**, deck152 **R-3**, engine seat **D-5**)
`plan_echo_count` max **37 -> 143**, p90 **80**, and **328 records serve a plan more than 40 windows
old**. The mitigations fire (`"<card>" is no longer on your menu` renders 622×; `stale_echo_in_range`
4, `named_row_reask` 3, all recovered), so this is not yet a correctness defect — it is the mechanism
behind every `named_row_reask` in the corpus and it scales with the restored window economy.
`152v125` seq 101 serves `YOUR PLAN (as you last stated it, 6 windows ago on turn 39; "Katilda,
Dawnhart Prime" is no longer on your menu): Cast Katilda. Attack with nothing ...` followed by the
note that retracts it, at a seat whose prompts reach 30,031 chars. **Ask, two independent halves:**
(a) force a PLAN refresh at a hard age (~40 windows) rather than only on menu invalidation;
(b) when the retraction note fires AND the plan is more than ~4 windows old, serve the note alone.
**Wave-56 prediction.** `plan_echo_count` max **< 60** (this corpus 143); records serving a plan over
40 windows old **< 60** (this corpus 328).

### R236 — `reveal_stall` is stamped on 100% of reveal records with a value that duplicates `latency_ms`, and `reveal_stall_phase` never varies. (= ledger **D12**; three seats independently; skill #263)
Lane E built the triple to be "present only when the driver was parked". Re-derived: present on
**12 of 12** reveal records; `reveal_stall_secs` equals `latency_ms / 1000` to the second on all
twelve (4, 5, 14, 24, 24, 42, 47, 55, 60, 148, 211, 474); `reveal_stall_phase` is **0** on all
twelve; the poll-churn floor is max(1,800 s, 3 × 900 s) = **2,700 s** and the largest value is 474.
`force-close` renders 0 and **no reveal parked, so the guard's own half is UNTESTED, not failed**;
`stale_livelock` on a reveal is 0 and that is a genuine PASS (R197). The DEFECT is the instrument: a
signature on every record of a kind selects nothing. **Ask:** emit the pair only above the structural
threshold, or rename it `reveal_wait_ticks` / `reveal_wait_secs` and reserve `reveal_stall` for the
guarded case with a boolean; and make `reveal_stall_phase` carry the driver's real phase or drop it.

### R237 — the residual 43 stale drops are the turn-flip seam alternation plus the hold's own re-open. (= ledger **D18**; lane E's D5(b) finding; engine seat §1; seat-125-126 **LOW #5**)
Independently classified by the line preceding each drop across all 21 stderr: **16 follow a resolving
`10DrawAction`** (the turn's own draw), **10 follow a resolving `NextGamePhase`**, **8 follow the
engine's own `AIPlayerGPT: hold re-opened at the cast/priority seam - the board changed`** (so R225's
predicate is also throwing away in-flight answers), 9 other. The next window is a `Casting decision`
in 20 of 43 and a `Land drop:` ask in 16 — the land-drop and casting seams contending for the single
async slot across the turn flip, which lane E's `WAGIC_GPT_DRIFT` dump proved outright on a stub game
(24 of 30 drops alternating two questions on an unchanged board, first divergence byte printed).
Cost **43 × 26.1 s ≈ 0.31 h** (wave 54: ≈3.2 h). **Ask:** key the async slot on
`(seam, turn, phase, boardKey)` and discard-without-re-issue only when the SEAM changed. **Priority:
MED and no lane of its own** — it is now a rounding error, and R225 removes 8 of the 43 for free.

### R238 — the `{feeds:` clause names the state and not the exit. (= ledger **D20** carried; deck162 **R-2**)
The row prints `converters on your battlefield: 0 (nothing of yours punishes their draws or discards
yet ...)`. The engine knows the hand: appending `- you hold Underworld Dreams` / `- no converter in
your hand either` puts the wave-55 release condition on the row instead of asking the pilot to
cross-reference the hand line, and would delete a hand-written qualifier from the pool's smallest
guide. Three prompts whose `{feeds:}` clauses are byte-identical and whose correct answers differ:
`162v123` s18 and `162v126` s23 (punisher in hand, take correct) versus `162v125` s98 (none, the
break).

### R239 — the cleanup-discard menu is numbered in an order the pilot cannot predict, while every guide in the pool ranks discards by cost. (deck146 **R-1**; skill #248's render half)
Two of seven deck146 discards sent a cheaper card while a dearer one sat lower (`146v130` s23:
`PUT: 1` = Agadeem's Awakening with Emeria's Call at row 5; `146v162` s20: `PUT: 7, 1` = a spare
Plains + Acererak `{2}{b}` with two Lolths `{3}{b}{b}` and two Silverquill Commands `{2}{b}{w}` left),
and **4 of the 7 answers begin `PUT: 1`**. **Ask, cheapest first:** print `{MV n}` on each discard row
so the comparison is a scan rather than arithmetic over `{..}` symbols. Sorting the rows by descending
mana value is the stronger version and is NOT proposed here: it would make row 1 the answer the guides
want, which is a nudge on the same axis as the standing declines-LAST ruling and belongs to the owner.

### R240 — `[doesn't untap during its controller's untap step]` has no consequence clause on the A-line. (deck146 **R-3**)
`146v123` seq 24: both A-lines carried the bare tag, both were sent, and at seq 28 the seat's own line
read `2 are creatures, 0 of them able to attack right now`, both `[tapped - cannot attack or block
this turn]`; 24 unrestricted creatures attacked back and the seat died at -2. Primitive
`mtg.txt:58850` `auto=lord(creature) doesnotuntap`. **Ask:** a tail on the A-line only —
`(declaring this attacker taps it permanently - nothing untaps it)`. This is the attack-side twin of
`(you kill it, your attacker lives)`, which the pilot already trusts.

### R241 — the plan-vs-menu note covers a NAME mismatch and not a contradicted EFFECT. (deck146 **R-4**)
`146v123` seq 28's served plan asserted a 24-card board wipe while the row it named carried a clause
denying it. **Ask, narrowly:** when the plan text names a card that IS a row on this menu and that row
carries a `{right now:` clause, append `(your plan names this row; read its {right now:} clause again
before you act on the plan)`. Not proposed as a general checker.

### R242 — the `(blocking trigger: ...)` gain is nested inside the survival verdict, and the standing cost is printed nowhere. (deck126 **G7**; skill #258)
`126v146` seq 6: `B1. Pride Guardian (0/3) [defender] ... - may block A1 (your blocker dies, attacker
lives (blocking trigger: you gain 3 and may gain 2 more))`. Nesting a gain inside a survival verdict
makes a losing block read as a priced one. Both facts are true and both should print, as SEPARATE
annotations in the order the decision uses them, plus the third number no annotation carries:
`{your blocker DIES, attacker lives} {this combat: you gain 3, and Perimeter Captain may gain 2 more}
{after this combat: you control 1 fewer blocker}`. The seat had 2 blockers at seq 6 and 1 against five
attackers at seq 16.

### R243 — give the MENU a dead-verdict line when every cast row on it is priced dead. (deck123 **G1**)
108 windows at one seat where every `Cast ...` row carried a dead verdict; **2 casts off them** (was 5
of a comparable population, with the guide paragraph doing that work). The residue is a pilot that
reads six rows, agrees in prose that each is dead, and answers one anyway. **Ask:** one line under the
option list, emitted only when the predicate holds — `[every cast row on this menu is priced dead
right now; the rows that do something are <the non-cast rows>]`. It converts six per-row judgements
into one aggregation the engine has already computed, and it removes no row.

### R244 — narrow `plan_choice_conflict` to a prose that names a DIFFERENT ROW ON THIS MENU. (deck123 **G2**; carried from wave-54 unchanged in shape)
9 firings, the corpus's largest fallback class, and all 9 co-stamped `decision_reversed_in_prose`.
The shape is unchanged: a CHOICE line taking a live row beside PLAN prose containing the pool's own
mandated arithmetic vocabulary ("this window: pass", "stop reached"). **Ask:** fire only when the
prose names a card or row label that (a) appears on THIS menu and (b) is not the row the CHOICE line
took. Pairs with **R227**, which is a separate defect on the same seam and must be fixed even if this
narrowing lands.

### R245 — on a repeated window, print WHAT CHANGED since the last ask. (deck123 **G4**)
The declined note reaches **27** in one Upkeep and 15 in one Draw step; it tells the pilot how many
times it has declined and not why it is being asked again. The replies inside those runs read like a
model looking for a reason ("Board unchanged", "The loop is currently resolving; I will pass priority
and wait"). **Ask:** extend the existing note by one clause when the option SET is unchanged —
`[you declined this exact list N times already this turn; the only change since your last answer is:
the top of the stack]` — or, when nothing changed, `... nothing on this menu has changed`. It also
makes R225's defect legible from the transcript alone, which is how it was found. If R225 ships most
of these windows disappear and this line costs nothing on the ones that remain.

### R246 — the equipment cast row is the one recurring cast row in the pool with no `{right now:}` verdict. (deck123 **G5 / E6**; skill #236)
`123v130` seq 11: a three-row menu (`Cast Lightning Greaves {2}` / `Cast nothing right now` / the HOLD
row) on a battlefield line reading `of which 0 are creatures`; taken, tapping out. FIFTH corpus, four
separate guide paragraphs against it, and every sibling rung at that seat closed when its row got a
verdict (edicts at N=0: **0 of 171 rows**; Tragic Slip on a dead row: 0 for three corpora). **Ask:**
`{right now: you control 0 creatures - this equips nothing}` on an equipment cast row at zero own
creatures, and `{right now: <name> already wears Lightning Greaves #1}` on a re-offered equip line.

### R247 — two rows naming the same card at two prices with identical verdicts, and no mark on the cheaper one. (deck123 **E5**, deck130 **G11**; skill #266)
`123v152` seq 104: `1. Cast Damnation {2}{b}{b} {right now: destroys 1 of their creature (...), 5 of
yours}` and `3. Cast Damnation {4}{b}{b} [from exile] {castable from exile ...} {right now: <the same
verdict>}`. The pilot answered **3**, paid two extra mana, swept its own five-creature board and lost
the game. The X menu already solves exactly this with `{X pricing: same kills as X=N, for K less
mana}`. **Ask:** `{same effect as row N, for K more mana}` on the dearer row. One string, and the pool
will meet the shape again from flashback, adventure and any future exile-cast row.

### R248 — say that the X marker is a RANKING, not a reason to cast. (deck130 **G12**; skill #244)
`130v146` s34 -> s36: the cast row's `{X pricing: ...}`, then the X menu's
`[<- best trade: the most of THEIRS at the smallest cost to YOURS - at X=2 that is THEIRS: Silverquill
Silencer; YOURS: Goblin x2]`, taken, with the PLAN quoting the marker's own words. One of theirs for
two of its own is a trade that deck's cast order forbids — and the marker is correct: it is the best
row on THAT menu, which has no "cast nothing" row because that decision was one screen earlier.
**Ask (small, optional):** extend the marker by four words on a marked row whose YOURS list is longer
than its THEIRS list — `... (this is the best row on this menu, not a reason to cast)`. Cheaper and
arguably better: put the marker's `THEIRS n / YOURS m` counts on the CAST row one screen earlier,
where `Cast nothing right now` still exists.

### R249 — put the declined COUNT inside the HOLD row's own `{...}` annotation, where the option-set key already cannot see it. (engine seat **HIGH #1(a)**, deck125 **G4**; resolves the collision lane A named)
The row promises a saving the pilot cannot price: `{taking this row skips the rest of this turn's
identical windows}` does not say how many. Lane A deliberately kept the count OFF the row because the
row text sits inside the ask key and the priority seam's deadlock key, so a number rising with every
answer would mint a fresh question — its own writeup names this trap. **The collision has a mechanical
answer:** `optionSetKeyOf` already strips every `{...}` group that is not a mana symbol, so a count
carried INSIDE the existing annotation is invisible to both keys by construction —
`{taking this row skips the N identical windows left in this turn; you have declined this exact list
M times already}`. **Not proposed:** moving the row up the menu when N is high (deck162 **R-1**'s
cheap half). That is an ordering nudge on the axis of the standing owner ruling that declines go LAST
because the model favours option 1 — the very anchoring three seats measured this wave — and it is not
this step's to change.
**Wave-56 prediction.** With the count on the row AND the five new guide HOLD blocks live, takes in
declined-note N >= 3 windows **>= 20%** (this corpus 8.9% corpus-wide / 37.4% at the one deck whose
guide commanded it). The two levers ship together, so the falsifier is per-deck: any deck whose guide
now commands the row and still converts below 15% is a render failure, not a guide one.

## NEW — LOW

### R250 — the `(xN)` event-log collapse still misses phase lines and draws, and it got worse. (= ledger **D13**)
**514** duplicate event lines inside single records (wave 54: 471): `- Phase: Draw` 230,
`- Opponent drew a card` 101, `- Phase: Main phase N` 80, `- Phase: Cleanup` 5, loyalty/level/token
lines 3-6 each. Visible in a shipped artifact: the `162v146` seq 15 `wall_miss` record's `events`
block reads `- You drew Master of the Feast` twice. GAME LOG is **58.1%** of the mean prompt; this is
the cheapest byte reduction on the board and the wave-53 verdict declining the ADJACENT collapse is
not re-opened.

### R251 — `You named "<card>" with the spell`: 20 of 98 narration lines fall back to a generic source label. (= ledger **D14**)
The other 78 read `... with Silverquill Silencer`. Same generic-fallback family as the stack line's,
now in the narration.

### R252 — the conditional `{kills N of the ...}` form has no THEIRS/YOURS split. (= ledger **D15**)
31 rows (`{kills 1 of the 1 CREATURE target at 3 damage - and 3 to the opponent at life 12 leaves them
at 9}` 19, `{kills 1 of the 1 CREATURE target at -1/-1}` 12). Ground-truth-checked: none names a
creature the seat controls, so nothing is FALSE — but the split the 223 unconditional rows carry is
absent here and "1 of the 1" is opaque.

### R253 — 1,725 resource 404s over 37 distinct resources, all audio plus `thumbnail`. (= ledger **D16**)
Down from 1,927 / 118; the image-cycling loop wave 54 flagged is gone and `Destroying WCachedResource`
now names stable backdrops. The residual is the silent `JSfx` stub, not lane J's memo. No action asked.

### R254 — the DRAW FORECAST line could print the product. (deck162 **R-3**)
`162v123` seq 30 declared `ATTACK: none` and won on the opponent's draw step, reasoning entirely from
`DRAW PUNISHERS on the battlefield: yours - ... Every card the OPPONENT draws costs them 3 life` and
`DRAW FORECAST (theirs): their next draw step draws 5 cards (1 + Howling Mine #1 1 + Dictate of
Kruphix 1 + Howling Mine #2 1 + Howling Mine #3 1)`. The model did the multiplication (5 × 3 = 15
against their 10) correctly, which it does not always do. Suggested tail:
`- that is 15 damage on their next draw step; they are at 10`. **LOW precisely because it worked
without it** — recorded so the two lines are never simplified away.

## PROCESS / MEASUREMENT

### R255 — `WAGIC_GPT_BOARDINDEX=0` SEGFAULTS, on this binary and on its base. (= ledger **D17**)
Lane E hit it (exit 139, turn 12) and confirmed it reproduces on the archived base `8a1768d1c`, so it
is pre-existing. Lane L's board index therefore has **no working A/B for any reviewer**. The standing
silent-instrument rule from wave 54 (a lane whose change can alter any emitted string, window or
timing ships its flag in the same commit) gains a second clause: **a disable flag that crashes is
worse than no flag, because the lane reads as A/B-able.** This belongs above any new instrumentation
on the wave-56 docket.

### R256 — lane M's cache-hit counters still reach no log, a THIRD corpus running.
New `gameend` fields DO land (`hold_windows_skipped` 735, `mana_only_windows_skipped` 544,
`identical_option_asks_resolved` 9, `wall_miss_events` 3, `wall_miss_unrecorded` 2 — all present on
42/42). A `cache_hits` field on the same record is the cheap fix; the lane's own stated watch item has
now been unscoreable by every reviewer for three waves.

### R257 — per-decision inference at a fixed `-j` on a shared server is NOT an engine metric, and the ledger stops carrying it as a falsifiable engine prediction.
Per-decision inference rose 23.9 s -> 26.1 s and lane R's discharge target (`<= 22 s`) reads as a FAIL.
It is not one. The 21 games were launched together and finished over 1 h 58 min, so concurrency falls
monotonically while each game's prompts GROW — a control in which the prompt-size hypothesis predicts
the opposite sign. Measured, first quartile -> last quartile of the three longest games:
`125v152` median latency **17.8 s -> 3.0 s** while the mean prompt went **12,622 -> 26,919** chars;
`123v125` **24.5 -> 4.8 s** at 6,196 -> 16,431; `125v126` **20.7 -> 2.6 s** at 6,670 -> 17,847.
Across all 21 games, **corr(mean concurrency during the game, that game's median latency) = +0.82**;
inside the corpus `corr(prompt_chars, latency)` is negative. **The engine seat's contention diagnosis
is ACCEPTED and is now confirmed by a within-corpus control it did not run.** Consequence: the wave-56
docket carries per-decision inference as a REPORTED number with its concurrency, never as a target,
and a prompt-byte trim must not be justified by a latency argument at this concurrency (skill #262).

### R258 — the log window remains the OWNER's call. (= R224 / ledger **D28** carried)
Prompts over 30,000: **1**. Mean prompt 10,740 -> **12,174 (+13.4%)** and total volume 23.8 M ->
38.6 M, both attributable to the +43% decision count and the new lane clauses (the HOLD row on own
turns alone is ~0.52 M chars = 1.3% of the total). GAME LOG share **58.1%**, reaching **82.4%** on the
late-game specimens. Nothing here proposes a compaction; R250 is the free half.

---

## PASS / KEEP — recorded so no later lane trims them
- **Lane C's two-sided edict verdict and its commit clause** (151 windows / 217 windows). The clause
  `{this row does not pick a target yet - taking it asks you next which of the N legal targets above}`
  answers, on the row, the exact question the wave-54 model spent 900 words of a reply asking.
- **Lane C's ownership split** `{kills whichever you target: THEIRS - ...; YOURS - ...}` (215 windows /
  223 rows) and the `{X pricing: same kills as X=N, for K less mana}` collapse marker on the BOTTOM row.
- **Lane D's decode line and copy-tag collapse** (123 windows each). The `(copy N of N in your hand)`
  literal that renders 7,000+ times is the hand DISAMBIGUATOR, kept deliberately so a two-copy pick is
  nameable — it is not duplicate-row waste and must not be collapsed away.
- **deck162's `DRAW FORECAST (theirs)` + `DRAW PUNISHERS on the battlefield` pair** — `162v123` s30 won
  a game off them with no guide sentence about the multiplication (R254 is an addition, not a rewrite).
- **The `[named: <card>]` battlefield tag** (83 windows), byte-identical and correct; only the row's
  price is missing (R202, still UNTESTED).
- **The `LIFE-TO-DAMAGE CONVERTER` block and its `their life LOOP is in play` attacker clause** — it is
  what made `123v126` s53 answer `no attackers` correctly on 37 creatures, and what flipped `130v126`
  from a 0-44 loss to a 20-0 win via two `no attackers` on the same board.
- **Lane E's mulligan scope tail** (`... alone on turn one` / `... even with every land in this hand in
  play`), 3 windows, 3 correct decisions, ending a four-corpus break.
- **The attackers blocker-count header**, 90 of 90 including the singular form — the coverage that
  deck152's re-keyed attack rule depends on.

## REJECTED, and why
- **"Skip the opponent-turn window whose stack is empty at a phase this seat has never cast in"**
  (deck125 **G3**, deck126 **G9**, FOURTH wave running, now restated at 291 windows / 0 casts on an
  UNCONTAMINATED denominator). **REJECTED again.** A phase's cast history is not a legality fact and
  the denominator is no longer the objection — the objection is the doctrine: it removes a legal
  window. **The lever the seats want is R225**, which withholds nothing from a seat that has not
  pressed a row, and R243, which tells the pilot the menu is dead instead of hiding it.
- **"When the declined note reaches a threshold, stop asking for the rest of the turn and record an
  auto-hold"** (deck162 **R-1**'s aggressive half, and the engine seat's escalation candidate under
  HIGH #1's falsifier). **REJECTED.** The seat filed it as a proposal precisely because it removes a
  legal choice for the remainder of a turn without the model asking. R225 gets the same windows back
  by honouring a row the model DID press.
- **"Hoist the HOLD row to row 1 on any window whose declined note reads N >= 3"** (deck162 **R-1**'s
  cheap half; seat-146-152-162 **E-3**). **NOT ADOPTED by this step.** It removes nothing and the
  evidence for row-1 anchoring is good (three independent measurements this wave), but decline-ordering
  is a standing owner ruling — declines go LAST *because* the model favours option 1 — and reversing it
  to exploit the same bias is the owner's call, not synthesis's. Recorded here so it is not re-filed as
  new; R249 takes the part that needs no ordering change.
- **A blanket "cap re-opens within one phase"** (wave-54's rejected form, not re-filed this wave).
  R225 supersedes it: a predicate on what the model was SHOWN, not a counter.
- **seat-123-130's E1 MECHANISM** ("a life total moving between windows re-opens the byte-identical
  option set"). **REFUTED on disk**: 0 of 113 hold-then-re-ask pairs is a life-only change; its own
  cited repro re-opens on a stack top and a hand-size change. Lane A's D2b works. **The finding and its
  remedy are adopted in full as R225** — this rejection is of the cause, not of the item.
- **The engine seat's HIGH #3 as filed** (a render vacuum causing the X = 3 answers). **RE-SCOPED**:
  deck125's guide mandates `X = 3` verbatim and the deck seat scored it 11/11 correct. The render
  half that survives is R231, and the proposed wave-56 experiment is not needed (skill #261).
- **The engine seat's PASS on the ability-row life tail** (`0 of 251`, presence only). **OVERTURNED**:
  238 of 248 values are false (R226, skill #259).
- **Any guide compensation for R225-R229.** Trust doctrine: a guide describes cards and printed
  strings, never the engine's window cadence, and never hedges against a rendered number.
- **Any win-rate attribution.** deck162 5-1, deck125 3-3, deck126 2-4, deck130 4-2, deck123 1-5,
  deck146 3-3, deck152 3-3 on real hands, n = 6 per deck, in the wave whose window economy changed by
  a factor of four. Counts only (#87, #167, #217). All seven seats are correctly recorded NO EXIT.

## MEASUREMENT — what wave 56 must report
- **HOLD adoption per deck, against this wave's five-cell table** (#233). The corpus-wide rate is not
  a number; the per-deck rate against what that guide's sentence says is.
- **HOLD takes followed by a byte-identical same-turn re-ask** (this corpus 113 of 282) — the number
  that says whether R225 closed.
- **Every audited literal carrying a COMPUTED number is checked against the record's own field**, not
  only for presence (#259). R226 was scored PASS twice on presence.
- **Every new stamp reports the fraction of its record kind that carries it** (#263). `reveal_stall` is
  on 12 of 12.
- **Per-decision inference is reported WITH the concurrency profile and never as a target** (#262,
  R257).
- **A `cache_hits` field on `gameend`** (R256), third wave.
- **Marker RENDER counts for every optional annotation a rung keys to** (#221), and
  `N raw / M live / K consequential` per rung (#216) — both honoured by five of seven seats this wave.
