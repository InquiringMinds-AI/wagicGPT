# Wave-79 deck 152 review — BANT HUMAN MIDRANGE

Corpus `matchups-20260912-000748-final`, SINGLE binary `wagic-5af6f7a95-w79step1`. My six seat files are
`<epoch>-ai_baka_deck152-<ptr>-vs-ai_baka_deck<N>.jsonl`; I read the six opponent seat files for context only.
Live guide = the wave-78 edition installed at a37b2d5ce, **19,993 B**. Short names below: `152v130` etc.

**207 model decisions** (123: 42 · 126: 36 · 125: 39 · 146: 31 · 162: 29 · 130: 30), `thinking` `on` on all
207, **100% with native reasoning** (median 5,043 / max 17,701 chars). Latency p50 90-124 s by game, max
346 s. **0 fallbacks on the ask seam**; the seat's only `fallback` is one `reveal`/`engine_answered`
(`152v126` seq 24) — the same zero-eligible Coven shape I filed as wave-78 MED-3, unchanged.
**0 `reply_truncated`, 0 `answer_replaced`, 0 `action_before_plan`, 0 `post_answer_overrun`,
0 `dropped_assignments`** on all six files. `protocol_deviation` = compliant 190 / `unlabelled_plan` 15 /
`plan_absent` 3 / `answer_label_absent` 0 → **17 `plan_line_missing` (8.2%; wave 78: 9.3%)**.

**Max prompt 20,328 B, 2 of 207 prompts over 20 KB** (wave 78: 28,621 B and 21 of 225). The T10 narration
trim is the largest single improvement visible from this seat and no prompt I read lost the seat's own last
PLAN, a stated stop or the loop state — `THE PLAN YOU LAST STATED` is present and correctly aged
("3 windows ago on turn 15") in every window I traced.

Seat **5-1** (`results.tsv`: wins over 126/162/125/123/146, loss to 130). Hands are real; I do not read the
swing from 4-2 as guide effect. The one loss turns on a single block decision, §3 S-1.

Gameend counters, by game (123/126/125/146/162/130): `ask_replays_reserved` 8/0/4/1/0/0,
`ask_key_continuation_differs` 3/2/4/3/1/2, `hold_windows_skipped` 15/0/0/0/6/0 (all `_cast`),
`hold_released_turn` 1/0/1/0/0/0, `hold_verdict_safer_ignored` 0/0/0/0/2/0,
`stack_drain_windows_asked` 0/0/0/0/4/0, `forced_close_events` 0/0/0/0/0/2 (`_unrecorded` 1),
`crossphase_identical_reputs` 2/0/0/0/2/0, `main_phase_windows_skipped` 2/0/0/0/0/19,
`own_turn_windows_skipped` 38/0/54/0/10/64, `reserve_decline_windows_noted` 0/0/0/0/4/0,
`blocker_forecast_rows` 0/6/0/0/6/6 (`_gang` 0/0/0/0/0/4, `_multi` 0/2/0/0/0/4, `_collapsed` 0),
`chain_acting_rows(_cast)` 25(5)/0/20(0)/26(0)/0(4)/0, `plan_names_stranded_card` 0/1/0/0/0/0.
**Zero on all six**: `identical_ask_answers_reserved`, `hold_windows_skipped_priority`,
`hold_reopened_new_threat`, `stop_reached_windows_skipped`, `stop_reached_reputs_collapsed`,
`own_loop_windows_asked`, `own_loop_verdict_lines_rendered`, `menu_pass_no_progress_suppressed`,
`phase2_answer_recovered/_missing`, `crossphase_board_unchanged`, `mana_only_windows_skipped`,
`plan_names_uncastable_zone_card`, `async_drops`, `identical_option_asks_resolved`, `chain_selfharm_rows`,
`repeat_annotated_takes`, `engine_reveal_floor_picks`, `wall_miss_*`, `ask_replays_refused`,
`answer_label_absent_heuristic_played`, `force_close_arms_*`, `put_gloss_stripped`,
`sibling_window_asks_skipped`, `defer` records.

## 1 — Game by game

| # | Opp | Result | Turn | Life | Deciding decisions |
|---|-----|--------|------|------|--------------------|
| 1 | 130 | **loss** | 21 | 0 / 11 | **seq 24 (t16, Blockers) is the game (§3 S-1).** At 18 life against two 1/1 Goblin tokens dealing 2, the seat put Katilda AND Luminarch Aspirant in front of them on `(all: both die)` rows and lost both engines to save 2 life. The header said everything: `NOT lethal: block only where the trade favors you`, `2 of those blockers die - taking it SPENDS Katilda, Dawnhart Prime, Luminarch Aspirant`, and `declining every block leaves you at 16 and costs you no permanent`. Board was empty from t17 on; the seat drew out. Second-order: seq 22 (t15) sent a lone 3/3 Brutal Cathar into four untapped blockers under a printed `GANG BLOCK: their 2 biggest together deal 3, enough to kill this attacker` and an `ATTACK TOTAL` reading `That is NOT lethal ... none of that damage is guaranteed` — the guide's GANG BLOCK rung says that creature stays home; Cathar died (it traded for Siege-Gang Commander + a token, so I do not price this as the loss). |
| 2 | 126 | win | 17 | 18 / -7 | The Sanguine Bond game (`LIFE-TO-DAMAGE CONVERTER` header in 17 prompts, ONE of the two names). **Wave-78 Edit 1 paid here**: seq 31 (t15) sent Sigarda AND Briarbridge Tracker for 8 with the stated reason `Sanguine Bond does not trigger from damage or blocking since they have zero creatures` — the exact card-direction error that lost wave 78's 126 game, answered right. seq 8 (t7) broke the Fateful Absence reserve rule (`plan_names_stranded_card: "Brutal Cathar"`, §3 S-3); seq 13 (t8) gave Katilda to an edict over a Wolf token (§3 S-2, EDIT 2's payer). |
| 3 | 125 | win | 19 | 20 / 0 | Lightmine Field game, played cleanly. seq 42 (t19) declared exactly the two attackers that survive 2 damage (`ATTACK: A3, A4`) for exact lethal, with the Ranger Class trigger placed on Briarbridge Tracker at seq 44. seq 32 took HOLD over two unused activated abilities — against the guide's absolute NEVER clause, and **right**, because both of those rows printed a tap bill that took its attackers (§3 S-4). |
| 4 | 146 | win | 16 | 23 / -3 | seq 15 (t11) Intrepid Adversary blocks Nadaar (rung 1, +4 lifelink); seq 22 (t14) three attackers take them 11 → 4; seq 25 animates Lair to a 5/5 in main 2 with the rung-ceiling and death-price clauses both read correctly; seq 31 closes. No Teferi window in the game. |
| 5 | 162 | win | 13 | 20 / -2 | Underworld Dreams / Fate Unraveler game. Two `their stack is draining` windows (seqs 8, 13), HOLD taken on both, both released correctly at the next main phase. seq 16 spent Fateful Absence on Fate Unraveler under a truthful `and it leaves you no mana for their turn` clause. seq 26 (t13 **Upkeep**) took Katilda's pump against the guide's MAIN-2-ONLY rule and against the tap bill — and won on the pumped swing at seq 30 (§3 S-5, and HIGH-1: that row's bill is SPLIT into two one-name clauses). |
| 6 | 123 | win | 19 | 26 / -3 | Mulligan machinery worked: seq 2 shipped a 0-land seven (guide floor), seq 3 kept 3 lands / 4 spells, seq 4 bottomed the duplicate Spellbinder. seq 15 (t7) took a cast row billing `paying this taps: Intrepid Adversary, Katilda, Dawnhart Prime - they cannot attack this turn` — two names in Main 1, a PASS by the guide — and the t7 attack was lost (no attackers record that turn); Damnation wiped the board at t8, so that damage never came back. seq 30 answered `Play no land right now` with Lair of the Hydra the only row — against an explicit guide rule, but the seat played Lair in main 2 of the same turn, so nothing was lost. |

Guide machinery that fired and was right: the `ATTACK TOTAL` last-clause rule (all six closing combats),
wave-78 Edit 1 (126 seq 31), the deploy floor (0 quiet-board declines with an untaken creature row), the
mulligan floor and bottom order, the PATHWAY face rule (23 windows, every face taken was the one the hand
needed), the Lair animation rule (146 seq 25, 125 seq 29 — both in a main phase, both at the rung the other
sources paid for), and the LIFE-TO-DAMAGE CONVERTER `ONE of the two names` bullet (126).

## 2 — Engine / interface / card items

### HIGH-1 (RENDER, a rule-grade under-count) — the tap bill is SPLIT into several one-name clauses on one row, and no clause states the row's real total
`152v162` **seq 26** row 1, t13 Upkeep:
`1. put 1/1 counters with Katilda, Dawnhart Prime [cost: {4}{g}{w}, Tap] {paying this taps: Luminarch Aspirant - it cannot attack this turn} {paying this taps: Katilda, Dawnhart Prime - it cannot attack this turn}`
Two clauses, each naming ONE body and each saying **"it"**, for a row that actually taps TWO creatures. The
narration one window later confirms the real bill: `Paid {4}{g}{w} for Katilda, Dawnhart Prime with Overgrown
Farmland; Hengegate Pathway; Luminarch Aspirant; Deserted Beach #1; #2; #3` plus the ability's own tap. The
split is the ability's own `{T}` cost rendering separately from the mana bill. **Three rows in my 207
prompts** do this, all Katilda's ability: `152v162` seq 26 (2 bodies), `152v125` seq 29 (2 bodies:
`{paying this taps: Brutal Cathar}` + `{paying this taps: Katilda}`), `152v125` seq 38 (**3** bodies:
`{paying this taps: Brutal Cathar, Briarbridge Tracker - they cannot attack this turn}` +
`{paying this taps: Katilda, Dawnhart Prime - it cannot attack this turn}`).
Why this is rule-grade and not cosmetic: the tap bill is the single input the guide's LETHAL COUNT PASS rule
counts names in, and **the largest number any one clause on 125 seq 38 states is 2, while the row costs 3
attackers**. A reader counting "the names in the bill" gets the wrong answer on every one of these rows.
Fix shape: one `{paying this taps: ...}` clause per row, covering the mana payment and the ability's own tap
together. Repro: those three files/seqs, and the `Paid ... with <sources>` narration line that follows each.
(I did NOT change the guide to hedge against this — the guide keeps counting the bill. The engine owes the
sum.)

### HIGH-2 (INTERFACE, a decision with one outcome is still put as a menu) — the Intrepid Adversary counter ask is asked when every row does the same thing
`152v130` **seq 6** (t3) and **seq 9** (t5), `152v123` **seq 8** (t3), `152v162` **seq 10** (t7): `options: 21`,
and the row-21 annotation states the window's own emptiness:
`{repeat cost: 20 x {1}{w} = 40 mana for all 20; you have 0 spendable now, which pays for 0 of them and stops}
{rows for 1 through 20 counters are all identical in effect right now: your mana pays for 0 payments and
stops, so every one of them adds the same 0 counters ...}`.
**T16's fold is LIVE and is a real improvement** — the menu is three physical lines (`1.`, `2-20.` collapsed
with both ends quoted and an `x19` tail, `21.`), the option vector is intact at 21, and the fold text is
true. What remains is that when the spendable mana pays for **zero** payments, all 21 rows have one outcome
and the window is not a decision at all: four of my 207 calls (≈2%) were spent on it, at p50 ~100 s each.
Contrast `152v146` seq 13, where 3 mana were spendable, the annotation read `pays for 1 of them`, and the seat
correctly took `add 1 counter` — that window IS a decision. Suggested gate: when the repeat-cost clause
computes 0 payable repeats, answer it in the engine (row 1) and do not ask. Repro: those four records,
`options: 21` on each, and the `pays for 0 of them and stops` clause in the row-21 annotation.

### MED-1 (RECORD, unverifiable counters) — `ask_key_continuation_differs`, `ask_replays_reserved` and `hold_verdict_safer_ignored` are gameend-only, with no per-record join key
My six files carry **15 `ask_key_continuation_differs`**, **13 `ask_replays_reserved`** and
**2 `hold_verdict_safer_ignored`**, and there is no per-record field or `*_seq` on any decision record that
identifies which window each belongs to. I scanned every field name that appears on any non-gameend record
across all six files; the list contains `hold_check_ref_seq`/`_window` (138 records, which DOES join) but
nothing for these three. Consequence for the owner's DC F1 rule — "a cached answer replays only when legal
continuations are identical" — is that **a seat cannot check it**: the replayed window writes no record at
all, so the 13 replays on `152v123`/`152v125`/`152v146` are invisible except as seq gaps. This is exactly the
shape the S11 `forced_close` per-event record fixed (see MED-2, which now reconciles); the same treatment
applied to replays and continuation-key changes would make the brief's "sample 20 and verify" instruction
executable. Repro: the gameend records of the six files vs. the record field inventory.

### MED-2 — wave-78 MED-2 is CLOSED: `forced_close` reconciles against stderr on this seat
`152v130` carries `forced_close_events: 2 / forced_close_unrecorded: 1`, two `forced_close` records
(**seq 20** `outcome: "armed"`, **seq 21** `outcome: "dropped_decision_moved"`, both `window_seq: 18`,
`arm: "cast"`, `defer_ticks: 0`, `park_armed: 0`, `unrecorded_so_far` 0 then 1), and
`game-152v130-1789189673.stderr` now prints **`forced closes: 2 events, 1 superseded
(dropped_decision_moved)`** — it named `deadline misses: 0 (0 unrecorded, 0 no-retry)` for the same game and
that is now a separate, correct figure. The file also has exactly **1** `unclosed <think>` line, matching the
1 `armed` event. Wave-77 LOW-2 / wave-78 MED-2 (the stderr summary disagreeing with the counters) does not
recur on this seat. **One residual**, LOW: that stderr line says `game end (turn 21, lost)` while the gameend
record says `turn: 22` for the same game (and `results.tsv` says 21). Repro: that file and that stderr.

### MED-3 (INTERFACE, decision shape) — the edict menu prints only the life price, never the material
`152v126` **seq 13** (t8), the rows in full:
`1. Wolf (2/2) [your battlefield] [tapped] [you SACRIFICE this; they gain 2 life (its toughness)]`
`2. Katilda, Dawnhart Prime (1/1) [your battlefield] - "Protection from Werewolves -- {4}{G}{W}, {T}: Put a +1/+1 counter on each creature you control. (...more)" [you SACRIFICE this; they gain 1 life (its toughness)]`
The only *number* on either row is the life they gain, and it points the wrong way: giving up the legendary
mana engine is the cheaper row by that number and the far more expensive row in every other sense. The seat
took row 2 with the plan `Sacrifice Katilda to Tribute to Hunger`. The engine already annotates material
elsewhere on the same class of row (`152v126` seq 26 prints the converter consequence
`- and with their Sanguine Bond that is 2 off YOUR life`), so a `[token - costs you no card]` /
`[your only copy]` style tag on the sacrifice rows is in keeping. This is the render half of the item; the
guide half is EDIT 2. Repro: `152v126` seqs 13, 16 and 26 (4 sacrifice menus on this seat, all in that game).

### LOW-1 — protocol shape is clean; the 17 `plan_line_missing` are all label drops
14 `unlabelled_plan` (a correct one-line plan with the `PLAN:` label dropped, or written `Plan:` — e.g.
`152v126` seqs 21/29/31, `152v162` seqs 22/24/25/27/29, `152v146` seq 24) and **3 `plan_absent`**, a bare
action line with no plan at all: `152v126` seq 26 `CHOICE: 3 (Luminarch Aspirant)` — the edict of S-2 —
`152v130` seq 23 `CHOICE: 1 (Siege-Gang Commander)` (the damage-order ask inside the combat that killed
Brutal Cathar) and `152v146` seq 23 `CHOICE: 0 (pass)`. No record carries prose outside the two lines;
`off_protocol_bytes` is again measuring the missing label. Down from 9.3% to 8.2%. Lane DD's parser fix is
post-corpus and is untested by any record here.

### LOW-2 — the `(...more)` gloss cuts are clean on this seat
399 `(...more)` marks across the six files. I flagged 11 candidate mid-clause cuts mechanically; on
inspection **all 11 end a whole unit** (`... {text: Flying (...more)}`, `...target of spells or abilities.)
-- Equip {0} (...more)`), so **0 genuine mid-clause cuts, 0 false statements**. T11/DC F7 passes from here.

### LOW-3 — the `[hold check:` brackets stay true
**138 brackets**, 12 distinct shapes. **18** `this is the first window I have asked you at this seam`, all
with `hold_check_ref_seq: -1`, none following an asked predecessor at that seam; every one of the other 120
carries a `hold_check_ref_seq` that resolves to an earlier record in the same file with
`hold_check_ref_window` beside it. I row-diffed each against the record its ref names: **86 verify exactly**
and 34 flag, and every flag I opened is one of two artifacts of my own normaliser rather than a false count —
(a) the `{reserve: ...}` clause contains nested braces, so a row that changed only inside that annotation
reads as gone-and-new (`152v126` seqs 7, 8, 11), and (b) the pass row's `(combat comes next this turn)`
suffix, which the engine's own rule explicitly calls the SAME row. Every flag shows the same
claimed-vs-measured offset shape (+1/+1 or +2/+2). Rank shifts were never counted as new.

## 3 — Strategy (guide-adherence) observations

**S-1 — THE DECIDING MISPLAY: two engines traded for two tokens to save 2 life at 18.**
`1789189675-ai_baka_deck152-0x561d0e9f6260-vs-ai_baka_deck130.jsonl` **seq 24** (t16, Blockers). Life 18,
attackers `A1. Goblin #1 (1/1) deals 1`, `A2. Goblin #2 (1/1) deals 1`. Blockers
`B1. Katilda, Dawnhart Prime (1/1) - may block A1, A2 (all: both die)` and
`B2. Luminarch Aspirant (1/1) - may block A1, A2 (all: both die)`. The header:
`INCOMING THIS COMBAT: 2 attackers, 2 unblocked damage - you would be at 16 ... best case ... you would be at
18 ... 2 of those blockers die - taking it SPENDS Katilda, Dawnhart Prime, Luminarch Aspirant. This assignment
is an OPTION, not an instruction: declining every block leaves you at 16 and costs you no permanent` and
`Your life: 18 ... NOT lethal: block only where the trade favors you`. Answer: `BLOCKS: B1:A1, B2:A2`, reason
`Block both goblins to save 2 life.` Both died; from t17 the seat's board was one Briarbridge Tracker and it
lost 0/11. **Every surface was TRUE and complete** — this is STRATEGY, not perception. What the guide said:
rung 2 `"(both die)" - TAKE IT unless the attacker it kills is worth less than your blocker` had the right
answer but gave the model no test for "worth less"; `KATILDA'S B-ROW: not on a "(both die)" ... line while N
is 10 or more` (N = 16) said DON'T for Katilda alone and was ignored; and the guide's absolute
`"BLOCKS: none" is allowed in exactly one case - every offered block reads "(your blocker dies, attacker
lives)" AND N is 10 or more` **forbade the correct answer**, because these were rung-2 rows. That internal
contradiction — one clause telling the seat a class of block is a decline, another telling it "none" is
illegal here — is what EDIT 1 removes.

**S-2 — an edict answered on the life number instead of the body.** `152v126` seq 13, quoted in MED-3. The
seat gave up Katilda (mana engine, one copy on board) to a Tribute to Hunger rather than a tapped vanilla
Wolf token, for 1 life instead of 2. The guide has no rule for a sacrifice menu at all; EDIT 2 buys one. Two
later edicts in the same game were answered correctly (seq 16 Wolf over Sigarda, seq 26 Aspirant over
Sigarda/Tracker) — in both, life-minimal and material-minimal happened to agree, which is why only seq 13
exposes the gap.

**S-3 — the Fateful Absence reserve VERDICT rule broken, at no cost.** `152v126` seq 8: the row printed
`VERDICT: taking this row STRANDS Brutal Cathar this turn - 1 source cannot pay 3, and it has no window after
your main phase`, the guide calls that row a PASS, and the seat took it (record carries
`plan_names_stranded_card: "Brutal Cathar"`; the plan named the Cathar for that same turn). The target choice
was right by the MATCHUPS lifegain rule — Perimeter Captain was the only gain-on-block body on the list. The
Cathar was not cast until t15. **No edit**: one violation in six games of a rule that is otherwise silent,
with no line lost.

**S-4 — the HOLD-PRIORITY NEVER clause pointed the wrong way and the seat was right to ignore it.**
`152v125` seq 32 (t17): rows 1 and 2 were unused activated abilities (`Draw 1 with Clue`, `Put a level counter
with Ranger Class`) and the seat took row 3, HOLD — the guide's `NEVER take it while any row above it is an
activated or loyalty ability you have not used this turn` says don't. It was correct: both of those rows
printed `{paying this taps: Brutal Cathar, Lair of the Hydra (animated this turn) - they cannot attack this
turn}`, i.e. both were already a PASS on the tap bill, so the hold priced nothing and the seat went to combat
with its attackers untapped and won the game. **No edit** (A334 — the hard case earns no concessive clause,
and the guide's own later sentence about rows "already ruled out" is the clause that governs). Recorded so
that the next seat does not write a qualifier for it.

**S-5 — Katilda's pump taken in UPKEEP, against two guide rules, and right.** `152v162` seq 26, t13 Upkeep,
opponent at 4 life: the guide says the ability is `MAIN 2 ONLY` and that a bill naming creatures in Upkeep is
a PASS. The seat took it, put a +1/+1 counter on all five of its creatures, and killed the opponent at seq 30
that same turn with the three untapped ones. It also cost the seat 6 life sitting through Underworld Dreams
draws. **No edit** — the rule exists to protect the attack, and here the pumped attack was lethal, which the
LETHAL COUNT section's own "if it clears cast nothing and go to combat" branch already outranks. Note that
HIGH-1's split bill means the seat could not have counted the names correctly here even if it had tried.

**S-6 — two smaller adherence misses, both costed.** `152v123` seq 15 took a cast row billing **two** named
creatures in Main 1 (a PASS by the guide) and lost that turn's attack with Intrepid Adversary; Damnation wiped
the board on the following turn, so the ~4 damage never returned. `152v123` seq 30 answered `Play no land
right now` with Lair of the Hydra the only land offered — an explicit guide rule — but the seat played Lair in
main 2 of the same turn, so the land drop was not lost. **No edit** on either: both rules are correctly worded
and were simply not followed once each.

## 4 — Guide verdict: **EDIT** (19,993 B → **19,963 B**, net −30)

One decision in six games was decided against the seat by the guide's own internal contradiction (S-1) and it
decided the only loss; a second class of window (S-2) has no rule at all. Both edits are paid for by a
decision the corpus shows going wrong. The guide does not grow: the two new rules are funded by six
reclaims, five of which delete text the runtime prompt itself states in every window of the relevant seam.

**EDIT 1 — BLOCKING. Remove the contradiction, and give rung 2 a test the model can apply.**
*Paid for by `152v130` seq 24.*

1a. before: `FIRST STEP OF EVERY BLOCK DECISION: find the header's "you would be at N". "BLOCKS: none" is
allowed in exactly one case - every offered block reads "(your blocker dies, attacker lives)" AND N is 10 or
more. The test is N, not the size of the attack: at "you would be at 6" you block even if only 2 damage is
coming.`
after: `FIRST STEP OF EVERY BLOCK DECISION: find the header's "you would be at N". At N of 9 or less the test
is N and not the size of the attack: block even if only 2 damage is coming. At N of 10 or more "BLOCKS: none"
is right whenever every block offered spends a body worth more than the attacker it stops, and the header's
own "taking it SPENDS <names>" line is that bill.` **+126 B.** Earns its bytes because the old sentence
declared the correct answer to seq 24 illegal; the new one names the exact render line (`taking it SPENDS
Katilda, Dawnhart Prime, Luminarch Aspirant`) the decision reads off.

1b. before: ` 2. "(both die)" - TAKE IT unless the attacker it kills is worth less than your blocker.`
after: ` 2. "(both die)" - TAKE IT unless the attacker it kills is worth less than your blocker. A TOKEN, or a
body whose row prints no ability, IS worth less than any ENGINE of yours - anything whose text makes mana,
counters or bodies, which is Katilda, Luminarch Aspirant, Sigarda and Ranger Class. At N of 10 or more that is
a decline, for every engine the header's "best case" line names at once.` **+332 B.** Earns its bytes because
"worth less" was unjudgeable before, and because the last clause is the half seq 24 needed: the header named
BOTH engines in one assignment and the old Katilda-only rule covered one of them.

1c. before: `KATILDA'S B-ROW: not on a "(both die)" or rung-3 line while N is 10 or more - she is every
Human's mana ability - but at N of 9 or less she blocks like everything else.`
after: `At N of 9 or less every engine blocks like everything else.` **−109 B** — the card-specific rule is
now the class rule in 1b, which is what let Aspirant through.

**EDIT 2 — NEW RULE, own block immediately above MULLIGAN.** *Paid for by `152v126` seq 13.*
before: (no such rule; the guide has nothing about a sacrifice/edict menu)
after:
```
A "you SACRIFICE this" MENU IS AN EDICT - ITS LIFE NUMBER IS A PRICE, NOT THE CHOICE
You pay one of these rows whatever you answer, so the smallest "they gain N life" is not the answer. Give up
the cheapest BODY: a TOKEN first, then a creature whose row prints no ability; never a lord or a mana engine
while another row exists.
```
**+342 B.** Earns its bytes because it is the only rule that answers the window S-2 lost, and because the
only figure the engine prints on that menu inverts the correct answer (MED-3). Heading is a scope declaration
(A333); no concessive clause for the case where the two orderings agree (A334).

**Reclaims** (all six are wording or duplication; no rule is dropped):
- R1, BLOCKING: removed `- ONE BLOCKER, AT MOST ONE ATTACKER: count the left-hand sides, each B-number
  appears at most once.` and `- The parenthesis is the computed result, first strike and deathtouch already
  applied; never override it.`, folding the gang-block bullet into one. **−269 B.** Both deleted sentences are
  restated in the blockers prompt itself, in every blockers window: `Assign each blocker to AT MOST ONE
  attacker (a creature cannot block two attackers), but several DIFFERENT blockers may gang-block the same
  attacker` and `Each parenthetical trade above is the ENGINE'S computed fight result ... Trust it over your
  own arithmetic; never re-derive it.` 4 blockers windows, `dropped_assignments: 0` on all of them.
- R2, ATTACKING blocking-trigger bullet: dropped the "is life they get for BLOCKING / not part of the
  outcome" half. **−51 B.** The attackers prompt prints it verbatim in every combat: `Inside a listed result,
  a parenthesis naming life THEY gain is the PRICE of that outcome, not part of it`. The half that is NOT in
  the prompt (it resolves at declaration, first strike does not cancel it) is kept.
- R3, MULLIGAN: `"No one-drop" is never a reason since you run none.` **−76 B** — the deck has no one-drops,
  so the card pool cannot raise the reason the sentence answers.
- R4, ATTACKING: `An A-line proves that creature is untapped, unsick and legal now - tapped creatures print
  "[tapped]" and get no A-line` → `the A-lines are the whole list of who can go, and the prompt names who
  cannot and why`. **−37 B.** The attackers prompt now prints its own block: `NOT offered above and NOT able
  to attack this turn (summoning sick) - <names>. A plan that names one of them as an attacker THIS turn
  cannot be executed.` (18 prompts this wave.)
- R5, MANA: tightened the "Mana available" bullet. **−15 B**, wording only.
- R6, MANA: removed `- A numbered "Cast ..." row IS payable - pick it, and never answer a number not on the
  list.` **−93 B.** 0 illegal or off-list answers in 432 decisions across waves 78-79, and the core prompt
  already declares the legal lists complete and authoritative.
- R7, ATTACKING: `Re-run that compare EVERY combat, since N climbs as their board grows - a swing that was
  fine two turns ago is how an opponent at 15 reaches 50.` → `Re-run that compare EVERY combat: N climbs as
  their board grows.` **−82 B**, illustrative flourish only; the rule and its trigger are unchanged.

Full revised guide at `wave79/deck152/strategy.txt`, **19,963 B**. No citation, count, history, corpus, wave
or seq reference appears in it (scanned); no `YOUR PLAN` quotation; the guide carries no PLAN template.

## 5 — Proposals

**None.** HIGH-1 and HIGH-2 are engine items with repros, not instruction-surface questions. MED-3 is a
render tag. S-4 and S-5 are A334 cases already settled by amendment. Nothing here generalises past this deck
in a way the general guide or the strategy-writing skill does not already cover.

## 6 — What I did NOT check

- Any seat but 152's. I opened the six opponent files only to confirm results and never traced their
  decisions; no other deck's windows were read at all, so nothing here adjudicates a corpus-wide counter.
- **T7 (`UNLESS <source> untaps them`) is UNTESTED from this seat.** I searched every one of my 207 prompts:
  14 `{paying this taps: ...}` clauses, **0 containing `UNLESS`**, and no opponent in my six games played an
  Intruder Alarm or any other both-sides untapper — the state the clause exists for never arose. Wave-78's
  HIGH-1 (the false "they cannot attack this turn" under an opposing Intruder Alarm, `152v123` seqs 17/18)
  therefore neither recurs nor is confirmed fixed by anything I read. HIGH-1 above is a DIFFERENT defect in
  the same clause.
- **DC F1 / the replay path.** I could not verify a single one of the 13 `ask_replays_reserved` or 15
  `ask_key_continuation_differs` windows, for the reason in MED-1: there is no record to read. I report the
  counters and the gap, not a verdict.
- The 2 `hold_verdict_safer_ignored` on `152v162` — same reason; I could not locate the windows.
- `stop_reached_*`, `own_loop_verdict`, `menu_pass_no_progress_suppressed`, `crossphase_board_unchanged`,
  `mana_only_windows_skipped`, `async_drops`, `phase2_*`, `identical_option_asks_resolved`,
  `blocker_forecast_collapsed`: **0 on all six of my games**, so every one is UNTESTED here, not confirmed.
- `HELD:` fired 3 times (`152v162` seqs 5/6, `152v123` seq 27), each correctly on Fateful Absence with
  `[no cast row now: it must have a target and there is no legal target on the board - HELD: this is an
  instant, so it is in your hand and castable the moment a legal target appears, including on their turn;
  this tag is about THIS window, never about your hand]` — T8's first half reads TRUE on all three. I did not
  audit the keep-X half at all: **0 counterspell rows and 0 `[X pricing]` blocks in my 207 prompts.**
- `GANG BLOCK` truth: 9 prompts carry a gang clause and I checked the arithmetic on one only
  (`152v130` seq 22: their two biggest are 2/2 + 1/1 = 3 power against a 3/3 — true). The other 8 I did not
  verify, and I ran no independent DP against the engine's `AT BEST` figures anywhere.
- T6 crack-back cover: verified on `152v130` seqs 17/18 only (life 20; the line's 5+2 = 7 → "at 13", the
  cover's 2-of-7 → "at 15"; both arithmetically true against the header's own total). 3 cover clauses total on
  this seat; I checked those two.
- No card was verified against Oracle text this wave. Katilda, Luminarch Aspirant, Brutal Cathar, Sigarda,
  Intrepid Adversary, Ranger Class, Briarbridge Tracker, Siege-Gang Commander, Sanguine Bond, Tribute to
  Hunger, Underworld Dreams, Fate Unraveler and Lightmine Field all behaved as their rendered text says and
  the rendered text matches my recollection, but **I opened no primitive**.
- stderr: only `game-152v130` and `game-152v125`, and only for the forced-close/deadline summary line and
  `unclosed <think>` counts. The harvest manifest, `askreplay/`, and `corpus-stats.py` output are untouched
  here.
- Teferi: **0 windows in six games.** The guide's Teferi bullet (~700 B, the largest single card entry) is
  therefore untested this wave; I did not cut it, because wave 78 saw 13 Teferi decisions in one game and a
  two-wave silence is not evidence it is dead.
- Wave-76 Edit 1 (`DECLARING A BLOCKER DOES NOT TAP IT`) remains effectively untested: 4 blockers windows,
  none turning on the tap question. So do the Katilda B-row rule as it stood (S-1 is the first board that put
  it in scope, and it was ignored), the MATCHUPS counterspell rule, and the `BOTH names` converter branch.
