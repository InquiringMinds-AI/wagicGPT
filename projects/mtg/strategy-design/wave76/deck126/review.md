# Wave-76 deck126 review — SANGUINE BLOOD (WGB wall prison, Exquisite Blood + Sanguine Bond)

Corpus `matchups-20260910-141526-final` (consolidated: 19 reruns + 2 first-run games; none of deck126's
six is a first-run game — all six are reruns, start epochs 1789074088-1789074153). SINGLE binary
`wagic-d372a1153-w76step1`. Record **3/6** (wave 75: 2/6; wave 74: 2/6). **266 decisions**, every one with a
prompt and a reply, across six games (125/123/130/162/146/152): 92/60/21/26/40/27.
**0 fallbacks, 0 `reply_truncated`, 0 `action_before_plan`, 0 `forced_close_prefill_echo`**;
`thinking: on` and a non-empty `reasoning` on every record (invariant 000 satisfied). One
`protocol_deviation` in 266 replies (`unlabelled_plan`, 162 seq 26, 103 B — the discard seam, answer read
correctly), `plan_line_missing` 1 (same record). `forced_close_unrecorded` 1 (123),
`phase2_answer_recovered` 1 (123) + 1 (146), `phase2_answer_missing` 0, `async_drops` 5 (125) + 1 (123).
Max prompt **24,039 B**; 22 of 266 over 20 KB. Max hold row **593 B** (CI's 650 budget held, all six games).

Counters per game (125/123/130/162/146/152): `hold_windows_skipped` 111/0/0/1/9/0 (cast seam in every
case); `hold_released_turn` 4/0/0/0/0/0; `ask_replays_reserved` 78/0/0/0/12/0;
`identical_ask_answers_reserved` 31/0/0/0/11/0; `crossphase_identical_reputs` 2/0/0/0/1/0;
`main_phase_windows_skipped` 8/8/0/0/0/0; `own_turn_windows_skipped` 41/41/16/24/3/22;
`mana_only_windows_skipped` 8/31/15/0/82/0; `plan_names_uncastable_zone_card` 17/0/9/0/16/0;
`plan_names_stranded_card` 0 in all six; `reserve_decline_windows_noted` 4/0/0/2/0/0;
`chain_acting_rows` 119/195/0/10/4/6; `blocker_forecast_rows` 0/0/0/0/0/6 (multi 6, collapsed 4);
`force_close_arms_refused/deferred/defer_bound_hits`, `repeat_annotated_takes`, `wall_miss_events`,
`menu_pass_no_progress`, `declined_face_latches`, `stop_reached_windows_skipped`,
`sibling_window_asks_skipped`, `chain_windows_collapsed` — **0 in all six**.

**Max asks in one (turn, phase) at this seat: 35** — `123` turn 17 Main phase 1, and that is HIGH-1
below. The next highest is 6. The `[you declined this exact list N times already this turn]` note
reached **N=31**, in the same place.

## 1. Game by game

| # | opp | result | deciding decisions |
|---|---|---|---|
| 1 | **125** | **WON** 46–0, t26 | Sorin t6 (seq10), a Vampire every turn, Exquisite Blood t8 (seq13), Bond t12 (seq24) — the pair went inert when the Bond was answered, and the seat ground the opponent from 20 to 4 on Vampires plus a Staff of Nin (seq36, t16) over sixteen turns. **seq147 (t26) re-cast Sanguine Bond from the second copy and the loop closed**, opponent 12 → 0 on the seq161 attack + seq171 ping. 20 of the seat's 23 hold-row violations (§2 MED-6) are in this game and cost nothing. |
| 2 | **123** | **WON** 40–0, t17 | The clean line: Sorin t9, Bond t11 (seq14), Tutor t13 for Exquisite Blood (seq19/20), Blood t15 (seq25), Staff t17 (seq28), and **seq29 pressed the Staff ping at the opponent with both halves out — the stated win button, taken in the first window it printed**. What followed is HIGH-1: 32 further model calls to pass priority while the loop resolved one point at a time. |
| 3 | **130** | **LOST** −1 to 14, t24 | Burn plus a sweeper. Bond t7 (seq11), then t13 (seq17) the seat's three walls were gone in one card and the battlefield line reads 0 creatures for the rest of the game. It drew **six lands in its last six turns** (seqs 19-22 are all bare land drops with a one-card hand) and never saw Exquisite Blood; Sanguine Bond alone converts nothing it can make. The one live Tribute, **seq18 (t15) on a Rorix Bladewing (6/5)**, gained 5 and the Bond took 5 off them (19 → 14) — correct, and the only damage this seat dealt. No decision at fault. |
| 4 | **162** | **WON** 33 to −4, t15 | Double Tutor (t5 seq8 → Bond, t7 seq11 → Blood), Blood t9 (seq14), Bond t13 (seq25) — pair complete t13 at **9 life** under a Fate Unraveler clock. **seq27 (t15) is the win**: Tribute on a Shield Sphere (0/6) reading `is sacrificed, you gain 6`, which the guide's HOLD-IT-on-a-[defender] rule releases the moment both halves are out. 6 life gained → 6 off them → the chain → −4. |
| 5 | **146** | **LOST** −10 to 14, t27 | Not a decision loss: **four halves answered one for one**. Blood t8 (seq12) exiled t9 by Vanishing Verse; Bond t12 (seq33) exiled t13 by the second Vanishing Verse; Bond copy 2 t14 (seq36) exiled t15 by Kaya `-3`; Blood copy 2 t16 (seq41) survived alone and starts nothing. Both Tutors were spent by t14 and the graveyard Tutor was exiled by Hive of the Eye Tyrant at t27. Two departures, neither decisive: **seq34 (t14) "Play no land right now"** with Savannah listed (the engine re-put the window and the seat played it at seq37 — the identical shape as wave-75's 152 seq24), and **seq53 (t27) `BLOCKS: none`** at 4 life with two untapped defenders, which the guide's "You never concede" forbids — see HIGH-2 for the render clause that licensed it, and §3 for why it is not an edit. |
| 6 | **152** | **LOST** −4 to 24, t15 | Curve-out: two Luminarch Aspirants at 5/5 and 6/5 by t12, Elite Spellbinder flying for 6, Teferi gaining 2 a turn. The seat's blocks were correct (seq11 double-blocked with two Perimeter Captains and took all 6 gain triggers at seq12, life 16 → 28), and **seq25 `ATTACK: none`** is the guide by the letter (every verdict `your attacker dies`, a `CRACK-BACK NEXT TURN ... would KILL you` printed, the Vampire needed as a body). **seq15 (t8) is the misplay and it is the guide's fault**: Sorin `-2` taken in the first window it printed, 3 loyalty → 1, no Vampire until t10. §3 edit A. |

**3/6, up from 2/6.** Both new wins are the loop actually closing (123 on the Staff ping, 162 on a
Tribute against a 0/6). The three losses are a sweeper plus flood (130), four halves removed one for
one (146) and a curve-out (152) — none is a decision fault. Play quality: **50 of 51 land drops
taken**, **49 `at 0 this does nothing` exposures and 0 takes**, 4/4 reveal picks, 0 fallbacks.

## 2. Engine / interface / card items

**HIGH-1 — A WON, DETERMINISTIC LOOP IS PUT TO THE MODEL ONCE PER ITERATION: 32 CONSECUTIVE
DECODES TO ANSWER `pass`.** Repro
`1789074103-ai_baka_deck126-0x56537b124da0-vs-ai_baka_deck123.jsonl` **seqs 30-61** (all turn 17,
Main phase 1). At seq 29 the seat took `Deal 1 damage with Staff of Nin targeting the opponent` with
`LOOP COMPLETE` printed. From seq 30 on, every iteration of the Sanguine Bond ↔ Exquisite Blood
chain opened a priority window carrying the SAME four rows
(`+1: create a 1/1 vampire` / `-2: emblem` / `-6: destroy up to three and reanimate` / hold), the
same `LOOP COMPLETE` paragraph, and one stack line —
`1 (top): ability: Exquisite Blood's Life [from your Exquisite Blood] [triggered/activated ability]`
alternating with Sanguine Bond's. Opponent life falls 16 → 1 one point per window; the seat answered
`pass` 32 times. Measured cost at this seat: **35 loop-on-stack no-op windows, 1,909 s + 129 s of
latency ≈ 34 minutes of wall clock** (`123` 32 windows, `125` 3), each a ~20 KB prompt and a full
reasoning decode, on a game whose outcome was already fixed. The `[you declined this exact list N
times already this turn]` note reads **31** by seq 61 — the counter is correct and nothing acts on
it. This is the regression watch's "any phase > 10 asks" tripwire firing: 35 asks in one
(turn, phase), against a next-highest of 6 anywhere else at this seat. The loop is not a decision:
both halves are the seat's own permanents, the chain has no choice in it, and no row on the menu can
change it. Falsification: any board with both halves out and a one-point trigger on the stack.

**HIGH-2 — THE BLOCKERS HEADER'S "AT BEST" FLOOR IGNORES THE BLOCKING LIFE GAIN THE SAME SCREEN
ADVERTISES, AND IT IS OFF BY THE WHOLE GAIN.** Repro
`1789074128-ai_baka_deck126-0x55f81a5822a0-vs-ai_baka_deck146.jsonl` **seq 53** (t27, life 4, six
attackers). Two lines apart the prompt prints:

`BLOCKING THIS COMBAT: each of your 2 blockers that blocks gains you 0 and may gain 2 more - up to 4 life for you`

`INCOMING THIS COMBAT: 6 attackers, 14 unblocked damage - you would be at -10; this KILLS you ... - at least 9 of that lands whatever you block (trample/menace counted as unblocked): you would be at -5 AT BEST (no assignment of your blockers does better); no block saves you`

`-5 AT BEST` and `no assignment of your blockers does better` are computed from damage alone. Both
blockers are Defenders and one of them is Perimeter Captain, so the best assignment is 4 − 9 + 4 =
**−1**, not −5. Here the sign does not flip (−1 is still dead, so `no block saves you` happens to be
true) and the seat's `BLOCKS: none` cost nothing — but the clause is a stated floor that is wrong by
the exact number the screen above it names, and on the same board at 9 life instead of 4 it would
say "no block saves you" about a combat that blocking survives. The model quoted it back:
its reasoning reads `the prompt says "no block saves you". So blocking none is optimal.` Fix shape:
the AT BEST floor is computed over assignments INCLUDING the blocking triggers it already meters,
or it says explicitly that it excludes them.

**MED-3 — Q1 IS NOT CLOSED: THE HOLD-CHECK BRACKET IS STILL FALSE ON 20 OF 106 CAST-SEAM WINDOWS
(19%).** The wording moved to `the last window I asked you at this seam` as designed, and the
nested-annotation residual is gone (0 cases where only a `{...}` group moved). The claim itself is
still measured against something other than the previous asked window at that seam. Two clean
repros, both verified by hand against the option sets:
- `...-vs-ai_baka_deck146.jsonl` **seq 33** (t12), one row, `Cast Sanguine Bond`, bracket reads
  `every row above was also on the menu at the last window I asked you at this seam (1 window in a
  row now)`. The previous cast-seam record is **seq 31** (t11 Upkeep), whose single row is
  `Cast Tribute to Hunger`. Sanguine Bond was on no earlier cast menu in the game.
- `...-vs-ai_baka_deck162.jsonl` **seq 19** (t11), six rows (Lantern, Bond, Battlement, Tutor, Staff,
  Tribute), bracket reads `2 rows above are new`. The previous cast-seam record is **seq 17**
  (t11 Draw), single row `Cast Tribute to Hunger`: **five** of the six are new.
Method: rows compared after stripping balanced `{...}` groups, seam taken as cast / priority / land
from the prompt's own menu header. 106 cast-seam brackets, 20 false; the same 20 are false whether
or not the seam key includes the phase, so they do not depend on my seam model. The priority-seam
brackets (61) I cannot adjudicate — the seam boundary between a Staff target menu and a loyalty menu
is not recoverable from the translog — so 20/106 is a floor, not the total.

**MED-4 — THE FORCED-SACRIFICE ROW'S GAIN CLAUSE CARRIES NO CONVERTER NOTE WHEN THE CONVERTER IS
YOURS.** Repro `1789074103-...-vs-ai_baka_deck123.jsonl` **seq 23** (t14 End, Devour Flesh). Rows:
`1. Perimeter Captain (0/4) ... [you SACRIFICE this; you gain 4 life (its toughness)]` /
`2. Vampire (1/1) [lifelink] ... [you SACRIFICE this; you gain 1 life (its toughness)]`, with
`LIFE-TO-DAMAGE CONVERTER on the battlefield: yours - Sanguine Bond` in the board block. Q6 added
`- and with their <converter> that is N off YOUR life` for THEIR converter; the mirror case is not
printed, and here it is the whole point of the seam — row 1 is not a 4-life price, it is **4 damage
to them**, and it is 3 more than row 2. The seat got it right off the converter paragraph, so this
costs nothing this corpus; it is the same clause family and one predicate away.

**MED-5 — `plan_names_uncastable_zone_card` NEEDS A THIRD EXEMPTION OR IT IS NOT MEASURING A DEFECT.
42 of 42 hits at this seat are a plan naming its own TUTOR OR DRAW TARGET.** All 42 flagged records
read on inspection as e.g. `130` seq 20 `PLAN: Play Scrubland, then cast Idyllic Tutor to find
Exquisite Blood. Next turn, cast Exquisite Blood to complete the loop and win.`, `125` seq 88
`PLAN: Cast Idyllic Tutor to find Sanguine Bond.`, `146` seq 46 `PLAN: Play Swamp ... then cast
Idyllic Tutor to find Sanguine Bond`. The named card is genuinely in the library — that is *why* the
plan names it — and the reply protocol states outright that the PLAN line `may name any card in your
deck or hand and any future turn, whether or not that card is among today's choices`. Under the
wave-75 classification these all count REAL. Suggested third exemption: a card named as the object
of a search/draw step in the same plan sentence. Per-record plumbing (Q9) works and is what let me
read them at all.

**LOW-6 — the hold row is still the answer the seat will not give: 23 of 75 trigger windows.** On
menus printing `NO LIVE CAST ROW ON THIS MENU` or `[you declined this exact list N times ...]` the
seat answered hold 16 times, took a live row 36 times, and answered `Cast nothing right now` **23**
times — 20 of them in the 125 game (seqs 30, 41, 42, 44, 49, 56, 69, 76, 82, 83, 95, 108, 116, 117,
123, 136, 148, 162, 165, 168), 2 in 146 (seqs 17, 24) and 1 in 162 (seq 16). Wave 75 measured 27 of
the same shape before edit B was installed; the rule is in the guide, flat and twice stated, and the
seat still declines about a third of the time. Reported, not re-edited (§3).

**LOW-7 — Q13's bracket prints, its board clause mostly does not.** `crossphase_identical_reputs` 3
at this seat (125 ×2, 146 ×1); the `[this exact list was put to you N windows ago at <phase> and you
declined]` bracket printed on all 3, and the `nothing on the board has changed` clause on **1 of 3**.
CQ F4's warning that the board comparison may be too strict to print is visible, at a small n.

**LOW-8 — the own-clock tag's horizon is wrong beside a lethal loop on the stack.**
`...-vs-ai_baka_deck123.jsonl` **seq 60**: `{the clock you already control: your Staff of Nin deals 1
damage a turn - at that rate alone the opponent reaches 0 in 1 more turn, with no card spent}` on a
window where the seat's own Exquisite Blood trigger is on the stack and the opponent is at 1 and
dies this window. Not false, but the slowest true statement available. (The wave-75 plural-template
defect is **FIXED**: every tag reads `deals 1 damage a turn` and `in 1 more turn`, 85 tags, 0 plural.)

**Wave-75 items, re-checked on this corpus.**
- **HIGH-1 (menace-blind ATTACK TOTAL aggregate cover, from `126v146` seq 45) — UNTESTED.** The
  board does not recur: this corpus has **8** ATTACK TOTAL headers at this seat (125 ×6, 152 ×2) and
  **not one has a menace or evasive attacker of mine**. Two print
  `Their N untapped blockers can cover every attacker you could send` (152 seq 25: 1 attacker vs 3
  untapped; 152 seq 28: 2 vs 8) and both are TRUE with the old arithmetic, so they do not
  discriminate. Q4(b)'s floor requalification is likewise untested here: **0 prompts in six games
  mention Lightmine Field or any other attack-punisher on their board.**
- **MED-2 (`<upto:3>` row printing one `targeting <name>`) — FIXED, verified.** 42 `-6` rows across
  125/123, every one reading
  `-6: destroy up to three and reanimate with Sorin, Lord of Innistrad [this ability chooses up to 3
  targets as it resolves - this row cannot name them yet, and they need not include any one body you
  have in mind]`. **0 rows name a target.** Q11 first half PASSES.
- **LOW-7 (Sorin `-2: emblem` printing `targeting Sorin`) — FIXED, verified.** 52 emblem rows in
  this corpus, **0** carry a `targeting` clause. Q11 second half PASSES.
- **MED-3 (unstable closing-row order) — FIXED, verified.** 112 menus carry both closing rows;
  **112 print `Hold priority` then `Cast nothing right now`, 0 reversed.** Q12 PASSES.
- **MED-5 (hold row P19 budget) — PASSES with room.** Max hold row **593 B** across all six games
  (wave 75: one 726 B outlier). `[HOW A HOLD ENDS:` appears exactly once per hold row.
- **MED-4 (`plan_names_uncastable_zone_card` real at this seat) — REVISED, see MED-5 above.**
- **Q3 (loop-complete false-verdict family) — PASSES.** 0 renders of
  `NOT lethal: block only where the trade favors you` beside a live `LOOP COMPLETE` (49 LOOP COMPLETE
  prompts); 0 of 85 own-clock tags beside a stated KILL of any kind.
- **Q5 `{crack-back cover:}` — present and arithmetically true** on all 25 exposures (130 ×3,
  162 ×22): e.g. 162 seq 19 prints both the removal form (`removes ONE of their creatures and THEY
  choose which one`) and the add-a-blocker form (`This adds 1 body - a creature that arrives this
  turn CAN block on their turn`) on the same menu. No regenerating or returning body appeared, so
  that half of Q5 is untested here.
- **Q15 `forced_close_prefill_echo`** 0; the one deviation (162 seq 26) is `unlabelled_plan` at the
  discard seam, `THE PLAN:` instead of `PLAN:`, answer consumed.
- **Q2 prompt size** — max **24,039 B**, 22 over 20 KB. **0** `- THEIRS:` rosters and 0 folded
  `(copy N of M in your hand)` rows in six games, so Q2's two fixes are untested at this seat.

## 3. Guide verdict: **EDIT** (`strategy.txt`, **19,990 B**, up 53 B from the live 19,937 — ceiling 20,000)

The guide **grows by 53 B**, and the rule that earned them is a rule that was **mechanically false as
written and fired**. One change, in two places, plus the two cuts that the same correction forces.

**A. Sorin's `-2` emblem is `+1/+0`. It raises power and NOT toughness, so "BEATS A 1-DAMAGE BOARD"
and "makes every Vampire a 2/1 [that survives]" are false: a 2/1 dies to exactly what a 1/1 dies to.
Paid by `1789074153-ai_baka_deck126-0x55885f1203a0-vs-ai_baka_deck152.jsonl` seq 15 (t8).** Sorin
resolved that turn at 3 loyalty against a board of `Luminarch Aspirant #1 (5/5)` and
`Luminarch Aspirant #2 (2/2)` — "anything on their line kills a 1/1 on sight" is TRUE, so the rule
as written fires — and the seat took `-2` in the first window it printed, leaving Sorin at 1 loyalty
with no Vampire on the board at all. Its stated reason is the guide's claim read back verbatim:
`PLAN: Activate Sorin's -2 ability to gain the +1/+0 emblem, ensuring future Vampire tokens survive
the opponent's lethal creatures.` A 2/1 does not survive a 5/5. The first Vampire arrived at t10
(seq 20) instead of t8, the second at t12, and the seat was dead at t15 with two bodies. `-2` was
offered on 52 menus in this corpus and taken exactly once — here — and the one time it fired it was
wrong. The emblem's real value is DAMAGE, which is only the deck's business when Sanguine Bond is
doubling it.
before: `lifelink. HIS -2 EMBLEM BEATS A 1-DAMAGE BOARD: it is PERMANENT and makes every Vampire a 2/1. Take it in the first window that prints it whenever anything on their line kills a 1/1 on sight, and the +1 every turn after.`
after: `lifelink. HIS -2 EMBLEM ADDS POWER AND NO TOUGHNESS: a 2/1 dies to every ping, sweeper and blocker a 1/1 dies to, so the emblem SAVES NOTHING and their board is never a reason to take it. TAKE THE +1 IN EVERY WINDOW BUT ONE: with SANGUINE BOND out and a Vampire able to attack, count its doubled damage - when one more point crosses their life, -2 is the kill.`

**A2 (the same error, second site — a cut).** The LIGHTMINE paragraph told the model to buy its way
past a declaration-time damage trigger with the same emblem: `ATTACK: none, or take Sorin's -2
emblem and send ONE 2/1 Vampire per turn` and `take the emblem first, or win off a Staff ping`. One
attacker under Lightmine Field takes 1 damage, which kills a 2/1 exactly as it kills a 1/1 — and the
paragraph's own next sentence already says a Vampire that dies on the declaration starts no loop. The
emblem clause contradicted it. Both mentions are cut; the paragraph keeps its stop and its two real
outs.
before: `... EACH attacker: ATTACK: none, or take Sorin's -2 emblem and send ONE 2/1 Vampire per turn. Same stop off the log ... so no loop starts - take the emblem first, or win off a Staff ping or a Tribute instead.`
after: `... EACH attacker: ATTACK: none. Same stop off the log ... so no loop starts - win off a Staff ping or a Tribute instead.`

**Net bytes.** A adds 240 B, A2 cuts 187 B; the guide goes 19,937 → **19,990**, under the 20,000
ceiling with 10 B to spare. The 53 B buy the removal of a false mechanical claim that the model
quoted back at the window it cost.

**What KEPT and earned it.** THE ZERO STOP: **49 exposures, 0 takes**, including every window of the
130 and 146 endgames. LAND DROP: **50 of 51**, the one decline re-put and taken. THE WIN BUTTON's
four starters: the 123 win is its third bullet (Staff ping) taken in the first window it printed, the
162 win its first (Tribute at one creature). TRIBUTE's `HOLD IT when the row names a victim tagged
[defender] ... unless both halves are out` is what released the Shield Sphere Tribute that won 162.
BLOCKING STEP 1's enchantment override: 152 seq 11 double-blocked with two Perimeter Captains under a
live enchantment and took all six gain triggers, 16 → 28. ATTACKING's `CRACK-BACK` exception:
152 seq 25, correct by the letter. Rung 1's `A COUNTERED or EXILED copy is not on your line, so cast
or tutor the next copy`: the 146 seat re-cast three times off it and the 125 seat won on the fourth
copy at t26. MULLIGAN + BOTTOMING: 7 windows, all by the letter (125 seq 2 shipped a seven, kept the
six, bottomed Staff of Nin).

**REJECTED edits.** (1) **The hold row (LOW-6), 23 violations.** Wave 75 already installed the flat
rule and the trigger sentence; the violations are in a game the seat WON by 46 and they cost decode
time, not decisions. A third statement of a rule stated twice is churn. (2) **`BLOCKS: none` at 146
seq 53**, against `You never concede`. The header's own numbers say −5 at best and the true best is
−1 (HIGH-2) — the seat was dead under every assignment, so the rule's purpose was not defeated; what
needs fixing is the floor, not the rule. (3) **The land-drop decline at 146 seq 34.** The guide
already says `TAKE ONE, every turn, at any life total, on any board` and `"Play no land right now" is
never your answer while a land is listed`; 50 of 51 is the rule working, and the engine re-put the
window. (4) **The `[second copy:` Chromatic Lantern taken at 125 seq 122.** Already covered twice
(the not-a-live-cast-row list and entry 7); no cost, won by 46. (5) **A rule for the forced-sacrifice
seam (123 seq 23).** The seat played it correctly off the converter paragraph in the prompt; a new
section for one correct decision is bytes this guide does not have. (6) **A rule to hold through the
won loop (HIGH-1).** The seat's `pass` was right and it won; the waste is the engine asking, and
weakening `NEVER HOLD ON A MENU THAT PRINTS [a live activated row]` to save decode time would trade a
live rail for a saving the engine should make itself.

No general-guide proposal and no skill proposal: A is a card fact specific to this deck, and every
item in §2 belongs to the engine seat.

## 4. Not checked
I did not read engine source; every mechanism sentence in §2 is a hypothesis stated as a falsifiable
repro, and I built no fixture. I read opponent seat files for none of these games and traced none of
their decisions, so I contribute nothing on the other decks. I opened **no `.stderr` file at all**,
so I contribute nothing on `deadline_pct`, `transport`, `askreplay/`, the stderr forced-close count,
the `{X}`-announcement payment loss, `Parser returned NULL` noise, or the harness's cross-pool log
harvest; the one `forced_close_unrecorded` (123) and the two `phase2_answer_recovered` are counted
but unreconciled. I could not cross-reference `main_phase_windows_skipped` (8 in 125, 8 in 123)
against rendered `Casting decision (Main phase N, YOUR turn)` headers from the translog alone — Q7 is
UNTESTED from this seat. I did not verify the 5 `async_drops` (125) or 1 (123) were re-asked.
`own_turn_windows_skipped` reaches 41 and I did not verify what those skips suppressed. The
priority-seam half of MED-3 (61 brackets) is unadjudicated because the seam boundary is not
recoverable from the translog. Card verification: **Sorin, Lord of Innistrad's `-2` (+1/+0, no
toughness) is the only card fact I checked against its rendered card text in the prompt itself**, and
edit A rests on it; Sanguine Bond, Exquisite Blood, Tribute to Hunger, Perimeter Captain, Pride
Guardian, Overgrown Battlement and Wall of Omens I re-used from earlier waves. **Devour Flesh,
Vanishing Verse, Kaya the Inexorable, Lolth, Hive of the Eye Tyrant, Acererak the Archlich,
Triumphant Adventurer, The Atropal, Lost Mine of Phandelver, Tomb of Annihilation, Soul Shatter,
Silverquill Command, Luminarch Aspirant, Elite Spellbinder, Teferi Who Slows the Sunset,
Briarbridge Tracker, Rorix Bladewing, Fate Unraveler, Shield Sphere, Staff of Nin and
Chromatic Lantern were NOT verified** beyond what the render stated.
