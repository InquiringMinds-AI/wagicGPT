# Wave-77 deck 152 review — BANT HUMAN MIDRANGE

Corpus `matchups-20260911-010157-final` (consolidated: 1 first-run game + 20 reruns after the pilot wedge),
SINGLE binary `wagic-42c41fc66-w77step1`. My seat files `...ai_baka_deck152-<ptr>-vs-...`; the six opponent
seat files read for context. Live guide = the wave-76 edition, **19,972 B**. None of my six games is the
first-run game (that is 126v162).

**210 model decisions** (146: 29 · 123: 33 · 125: 37 · 126: 18 · 130: 26 · 162: 67), 100% with native
reasoning, `thinking` `on` on all 210, reasoning median 5,341 / max 21,369 chars. Seat **6-0** — every game
won, opponent at -14/-15/-6/-4/-7/-4, turns 18/15/13/13/17/15.

**0 `unparsed_reply` on this seat** (`/usr/bin/grep -c unparsed_reply` = 0 on all six files), so none of the
corpus's 5 is quoted here. The corpus's ONE `defer`/`menu_not_askable` IS on this seat and is HIGH-1 below —
it is not my seat's decision. 0 `answer_replaced`, 0 `reply_truncated`, 0 `action_before_plan`, 0
`repeat_count_reask`, 0 `ask_replays_refused`, 0 `phase2_answer_missing`, 0 `blockers` records of any kind.
`plan_line_missing` 9/210 (4.3%), `off_protocol_bytes`>0 7 (one of them the engine's own defer text).
Latency p50 102.5 s, max 448.3 s. Max prompt **25,953 B**, **19** prompts over 20 KB (wave 76: 23,969 / 8) —
R5's `{effect:}` gloss is what grew them.

Gameend counters by game (146/123/125/126/130/162): `hold_windows_skipped` 0/8(priority)/11(cast)/0/0/1,
`ask_replays_reserved` 6/2/5/0/0/2, `identical_ask_answers_reserved` 6/0/5/0/0/0,
`crossphase_identical_reputs` 1/0/2/0/0/1, `crossphase_board_unchanged` **0 everywhere**,
`main_phase_windows_skipped` 4/0/10/12/16/4, `own_turn_windows_skipped` 71/0/28/36/48/18,
`chain_acting_rows(_cast)` 0(12)/2(0)/0(18)/0(0)/0(0)/18(2), `blocker_forecast_rows` 0/18/0/0/0/32,
`_multi` 0/0/0/0/0/22, `async_drops` 0/1/0/4/0/0, `forced_close_unrecorded` 0/0/0/**1**/0/0,
`hold_released_turn` 0/1/0/0/0/0, `own_loop_windows_asked` **0 everywhere**. **Zero across all six**:
`plan_names_uncastable_zone_card`, `plan_names_stranded_card`, `chain_selfharm_rows`,
`stop_reached_windows_skipped`, `mana_only_windows_skipped`, `declined_face_latches`,
`force_close_arms_refused/_deferred/_same_arm_deferred/_defer_bound_hits`, `menu_pass_no_progress`,
`engine_reveal_floor_picks`, `identical_option_asks_resolved`, `repeat_annotated_takes`.

## 1 — Game by game

| # | Opp | Result | Turn | Life | Deciding decisions |
|---|-----|--------|------|------|--------------------|
| 1 | 146 | win | 18 | 16 / -14 | seq 13 Aspirant counter onto Sigarda (the only body that could attack); seqs 19-20 Adversary + valor; seq 28 three-attacker lethal off `KILLS them whatever they block` |
| 2 | 123 | win | 15 | 21 / -15 | seq 11 Spellbinder exiles Devour Flesh; seq 19 exiles **Damnation**; **seqs 18/20/21 three tap-bill rows taken against the guide, all right — the opponent's Intruder Alarm untapped the bill (S-3)**; seq 35 nine-attacker 25-damage alpha |
| 3 | 125 | win | 13 | 26 / -6 | seq 15 Sigarda on curve; seqs 33-34 Adversary + 1 valor counter; seq 42 four-attacker lethal at `19 vs 14` |
| 4 | 126 | win | 13 | 20 / -4 | seq 7 Spellbinder exiles **Sanguine Bond** off the guide's named-card rule; seq 16 attacks into `LOOP HALF PENDING` with three unblockable bodies; seq 19 lethal with **both loop halves live** (wave-74 Edit 3 TESTED, below) |
| 5 | 130 | win | 17 | 16 / -7 | seq 20 Huntmaster tapping out at 6 sources; **seq 21 = the corpus's only `defer`, and it is the OPPONENT's Starstorm (HIGH-1)**; seq 26 two-attacker lethal |
| 6 | 162 | win | 15 | 17 / -4 | seqs 27-35 and 42-49 two Teferi +1 chains, 17 calls (MED-1); **seq 38 `Play no land right now` (S-1)**; **seqs 55-63 nine identical Draw-step windows (HIGH-2)**; seq 70 eight-attacker 23-damage lethal |

Nothing in these six games was decided against the seat. The guide's load-bearing decisions were: the
LETHAL COUNT / `ATTACK TOTAL` last-clause rule (fired at every one of the six closing combats and was right
every time), the Spellbinder named-card rule (Sanguine Bond at 126 seq 7, and Damnation at 123 seq 19 which
saved a nine-creature board), the deploy floor (no quiet-board hold anywhere: 0 own-main declines with an
untaken creature row outside a cleared lethal count), and the LIFE-TO-DAMAGE CONVERTER section. The guide did
NOT decide: any block (zero blockers windows all wave), any mulligan beyond a plain keep (6 untouched sevens,
6 keeps, 0 bottom asks), any Lair of the Hydra animation, any reserve-VERDICT pass (3 `reserve:` prints, all
declined), any Fateful Absence cast against a planeswalker.

## 2 — Engine / interface / card items

### HIGH-1 (ENGINE, seat attribution) — the corpus's only `defer` is the OPPONENT's decision, written into MY seat log
`1789114895-ai_baka_deck152-0x560bc3109c80-vs-ai_baka_deck130.jsonl` **seq 21** (t15, Main phase 1), rendered:
`menu on Starstorm could not be put to the model (no answerable shape - e.g. the announced X is no longer
affordable); the heuristic took option 0`, `kind: defer`, `fallback: menu_not_askable`, `options: 0`,
`chosen_text: Starstorm`, `latency_ms: -1`, `log_window_kind: unclassified`.
**Starstorm is not in deck152** (`/usr/bin/grep -il starstorm deck152.txt deck130.txt` matches deck130 only).
It is the opponent's instant, announced at their X-menu: the deck130 seat log
(`...-ai_baka_deck130-0x560bc44413a0-vs-ai_baka_deck152.jsonl`) shows seq 49 `CHOICE: 1 (Cast Starstorm)` and
seq 50 `CHOICE: 1 (Cast Card Normally)` at the same t15 — and then NO X row; the seat re-announces at t16
(seqs 51-53) and lands `X = 2`. So the deck130 seat's ANNOUNCE_X window died, and its defer record was written
to the **deck152** file. The record also carries deck152's own `events` (`You cast Tovolar's Huntmaster ...`),
so the record mixes one seat's window with the other seat's log and event stream. This is the residual CU F10
was meant to close: the defer path does not resolve the acting player before choosing a translog file.
Consequence for every later reader: the one `defer` in the corpus is attributed to the wrong deck, and any
per-seat census that counts `kind` is off by one in both directions. Repro: the two files above, seqs 21 and
49-53.

### HIGH-2 (ENGINE, window shape) — nine identical priority windows while the OPPONENT's stack drained, and the only escape row is priced at the turn's abilities
`...-vs-ai_baka_deck162.jsonl` **seqs 55-63** (t15, Draw step, my turn). The stack at seq 55 held **seven**
objects — `Fate Unraveler's deal 1 damage` ×2, `Underworld Dreams's deal 1 damage` ×4, `Howling Mine's Draw 1`.
Each resolution re-opened priority with a byte-identical three-row menu (`put 1/1 counters with Katilda`,
`Hold priority`, `Pass priority`). The hold-check bracket counted it out loud, truthfully, all the way:
`(1 window in a row now)` … `(9 windows in a row now)`, and the decline bracket likewise
(`declined this exact list 1 time already this turn` … `9 times`). The seat answered `CHOICE: 0 (Pass
priority)` eight times and took the hold at seq 63. **At p50 102 s that run is ~15 minutes of decode for zero
game state.**
The important half is WHY the model could not escape sooner, and it is not a guide failure: row 1 is Katilda's
`{4}{g}{w}, Tap` ability, unused this turn, and the hold row's own text says
`taking this row gives every one of them up for as long as these rows stand`. The guide's hold paragraph
forbids holding over an unused activated ability precisely so a real Main-2 line is not silenced — so the
guide-correct answer at each of seqs 55-62 WAS `pass`. The engine offers no cheap escape between "answer nine
times" and "forfeit every live ability row for the turn". What a better interface would do: while the only
change between windows is the opponent's own stack shrinking and the seat holds no instant it could respond
with, either auto-pass (as `mana_only_windows_skipped` already does for a mana-only menu) or offer a
**stack-scoped** hold row — "pass until this stack empties" — which costs the seat nothing it still wants.
Repro: that file, seqs 55-63, the `[hold check: ...]` and `[you declined this exact list N times ...]`
brackets, and the `ON THE STACK` block at seq 55.

### MED-1 (INTERFACE, decode cost) — wave-76 MED-4 RECURS, now twice in one game
`...-vs-ai_baka_deck162.jsonl` **seqs 27-35** (t11, 9 calls) and **seqs 42-49** (t13, 8 calls): one Teferi +1,
each time walked as `+1` → `choose a land` → `choose your land` → the land → `choose a creature` → `choose
opponent creature` → the creature → `choose an artifact` → the artifact / `Decline - do nothing`. **17 of this
game's 67 decisions (25%) are two activations of one loyalty ability**, at p50 102 s. Unchanged from wave 76's
write-up; one window carrying stage + side + target, or a pre-declined empty stage, still buys back most of it.

### MED-2 (RENDER, hold-check bracket) — a row that DISAPPEARED is never reported, only rows that appeared
`...-vs-ai_baka_deck125.jsonl` **seq 20** (prev asked window seq 19) and **seq 35** (prev seq 33). At seq 20
the Brutal Cathar row of seq 19 is gone (it was cast) and the bracket says
`1 row above is new since the last window I asked you at this seam`; at seq 35 the Intrepid Adversary row of
seq 33 is gone and the bracket says the same. In both the "new" row is real (Fateful Absence's
`{kills whichever you target: YOURS - 3 bodies: ...}` roster changed — R2c's REFUTED reading holds here), so
the COUNT is not false; what is missing is the other half. The `HOW A HOLD ENDS` block tells the model that
`a row disappearing` re-opens a hold, and then the bracket never says a row disappeared. A seat pricing a hold
is told only about arrivals. Fix shape: the bracket names both movements (`N rows above are new and M rows are
gone since ...`). Repro: that file, seqs 19→20 and 33→35, option lists.

### MED-3 (INTERFACE, repeat menus) — a 21-row menu whose rows 2-20 are provably identical no-ops
`...-vs-ai_baka_deck123.jsonl` **seq 7** (t5, Intrepid Adversary ETB, `Mana available: 1 total`). Rows: 1
`don't add any counter`, rows 2-20 `add N counters {same effect right now: adds 0 counters}`, row 21
`add 20 counters ... {repeat cost: 20 x {1}{w} = 40 mana for all 20; you have 1 spendable now, which pays for
0 of them and stops}`. Every row is truthful and every row does the same nothing; the seat answered
`CHOICE: 21 (add 20 counters)` and the events confirm the no-op. Two concrete asks: fold rows whose annotation
already reads `adds 0 counters` (the engine knows they are equivalent — that is what the annotation says), and
print the `{repeat cost: ...}` arithmetic on the FIRST paid row, not only on the last. Not a misplay: the
answer was effectively row 1, which is what the guide's Adversary rule wants at 1 spendable mana.

### MED-4 (RECORD) — `hold_check_ref_seq` cannot be joined to the record stream
It is a window counter, not a record `seq`: in `...-vs-ai_baka_deck162.jsonl` record 63 carries `ref 59`,
record 55 carries `ref 51`, record 4 carries `ref 1` (record 1 is the `system` record). A deck seat therefore
cannot do what R2's note promised — adjudicate the bracket "without re-implementing the key" — because the
referent it names does not exist in the file. Emitting the referenced record's `seq` (or both numbers) would
close it. I fell back to comparing option-row sets directly; see §"Verified NOT defects".

### LOW-1 (RENDER, typo) — `put on bopttom`
The 3-card reveal (Teferi's -2) renders `every other card goes to "put on bopttom"`, while the 5-card COVEN
reveal on the same seat renders `"put on bottom"` correctly. Repro: `...-vs-ai_baka_deck162.jsonl` seqs 14
and 21.

### LOW-2 (ENGINE) — a forced close that wrote no record
`game-152v126-1789114886.stderr` lines 815 and 1323:
`AIPlayerGPT: unclosed <think> (budget/truncation); forcing the answer`, the second immediately before
`AIPlayerGPT: Casting decision (Main phase 1, YOUR turn)` → `chose 1 of 3`. My seat's gameend for that game
carries `forced_close_unrecorded: 1`, and no record in the file shows a truncated or malformed reply (seq 14's
reply is well-formed at 7,636 reasoning chars). The same stderr's per-seat summary line reports
`deadline misses: 0 (0 unrecorded, 0 no-retry)` for BOTH seats — that counter and `forced_close_unrecorded`
disagree in the same file, which is worth one line of the engine seat's reconciliation.

### LOW-3 (protocol) — wave-76 LOW-1 RECURS unchanged, sixth wave
9 of 210 records `plan_line_missing` (4.3%, corpus 5.1%): **6 carry a correct one-sentence plan with the
`PLAN:` label dropped** (`Cast Luminarch Aspirant to boost Sigarda's power...` 146 seq 12 ·
`Attack with Sigarda and Luminarch Aspirant.` 146 seq 21 · `Cast Elite Spellbinder, attack with it, end turn.`
123 seq 18 · `Attack immediately.` 123 seq 35 · `Pass on Fateful Absence, no opponent creatures to target.`
125 seq 27 · `Tap Shield Sphere to unblock ground attackers.` 162 seq 32) and **3 are bare action lines**
(146 seq 14 `ATTACK: A1`, 146 seq 23 `PUT: 3`, 125 seq 25 `CHOICE: 3 (Cast nothing right now)`). No record
carries prose outside the two lines. `off_protocol_bytes` is again measuring the missing label.

### LOW-4 (reasoning shape) — wave-76 LOW-2 RECURS
Every seat file ends its decodes the same way: `Output matches. Proceeds. [Output Generation] -> *Proceeds*`,
`[Done] (Note: I'll ensure no extra spaces or newlines are added.) Proceeds.` (162 seqs 55/56/62, 130 seq 21
region, 123 seq 7 all quoted in my notes). It corrupted no answer this wave.

### Verified NOT defects on this seat this wave
- **R2 / CU F5 / CU F6 — the hold-check bracket is TRUE on 117 same-seam pairs.** I compared the option-row
  sets of every hold-check record against the previous record at the same seam (rows normalised only by
  collapsing the pass-row wording variants, which the bracket's own text declares identical). **42 of the 117
  are LAND-seam records and none of them compares against a cast menu** — R2a's 22 false cast/land brackets do
  not recur here. Five pairs mismatched my naive normaliser and all five resolve as TRUE on inspection: two are
  a Ranger Class level row whose COST moved `{1}{g}`→`{3}{g}` (123 seq 34, 130 seq 17 — "a different cost" is
  a new row by the bracket's own rule), three are Fateful Absence rows whose `{kills whichever you target:}`
  roster changed (125 seqs 20/35/37 — R2c's REFUTED reading). **15 `this is the first window I have asked you
  at this seam` sentences, every one at a genuinely first window at that seam** (0 following an asked
  predecessor). The one thing the bracket does not say is MED-2.
- **R5 — the creature `{effect:}` gloss is live and true.** 47 distinct glosses across the six games (Sigarda,
  Brutal Cathar, Luminarch Aspirant, Intrepid Adversary, Elite Spellbinder, Tovolar's Huntmaster/Packleader,
  Augur of Autumn, Moonrage Brute, and theirs: Shield Sphere, Fog Bank, Howling Mine, Intruder Alarm, Pyrite
  Spellbomb, Ranger Class). Every truncation ends in an explicit `...` and **none cuts into a false
  statement**; the nearest miss is Brutal Cathar's `exile target creature an opponent controls until this...`,
  whose missing half is carried on the rows themselves by
  `{if this leaves the battlefield: their <name> comes back from exile}`. The cost is prompt size: max 25,953 B
  and 19 prompts over 20 KB, both up on wave 76.
- **R7's aggregate half — the untapped-blocker count IS legality-filtered.** `...-vs-ai_baka_deck162.jsonl`
  seq 10: `Their 1 untapped blocker that can legally block at least one of these attackers can cover every
  attacker you could send`. The cover-clause half is UNTESTED here: **zero `{crack-back cover:}` clauses on
  this seat all wave** (45 prompts carry a `CRACK-BACK` line, none a cover clause), so wave-76 MED-1 could not
  recur and could not be confirmed fixed from my games.
- **Wave-76 Edit 2 (the "held back" tag) — TESTED, PASSES 4/4.** Four attackers menus print it (123 seq 35,
  162 seqs 37/53/70) and **every one of them attacked**; the wave-76 HIGH-1 shape (`ATTACK: none` under a
  `held back` tag) does not recur, and at 123 seq 35 six of the nine A-rows carried the tag and all nine went.
- **Wave-74 Edit 3 (the LIFE-TO-DAMAGE CONVERTER section) — TESTED for the first time, and PASSES.**
  `...-vs-ai_baka_deck126.jsonl`: at t11 the header read `LOOP HALF PENDING: Sanguine Bond is on THEIR
  battlefield and the other half ... is in their hand`, and seq 16 sent exactly the three A-rows tagged
  `[no creature they control can block this attacker]` into `0 untapped creatures able to block` — the ONE-name
  branch's answer. At t13 both halves were in play (`any life they gain or you lose in combat chains until you
  are at 0`) and the `ATTACK TOTAL` read `At least 9 damage lands whatever they block ... that KILLS them
  whatever they block`; seq 19 sent all four and won. The section's absolute LOOP clause ("no other attacker")
  and the lethal clause meet here, the model took the lethal clause, and it was right — recorded, no edit
  (A334: the hard case earns no concessive clause).
- **The Sigarda COVEN reveal is BACK.** Wave 76 reclaimed the COVEN paragraph's bytes on the ground that its
  strings had not appeared in a prompt for two waves. They appeared **5 times** this wave (146 seqs 15/23/30,
  125 seqs 26/43): `Choose ONE card that goes to "get a human"`, with a correct eligibility filter — Sigarda
  herself is marked `[does NOT qualify]` (Angel) at 146 seq 23, Tovolar's Huntmaster is marked eligible
  (Human Werewolf) at 146 seq 30, both right. All five picks were legal and sound (two forced by a single
  eligible row). No decision went wrong, so no edit — but the reclamation's premise is now false and a later
  wave should not re-cut that paragraph on a two-wave silence.
- **The lethal count and the deploy floor.** 6/6 closing combats opened with a `Cast nothing right now` under
  a cleared count (146 seq 24, 123 seqs 28/29/32, 125 seq 38, 126 seq 18, 162 seqs 51/66) and every one went
  straight to a winning attack. No creature was ever held to dodge removal.
- **Mulligans.** 6 untouched sevens, 6 keeps, 0 mulligans, 0 bottom asks, no invented quotation
  (wave-75 Edit 2 passes a second wave).
- **Land drops.** ~40 land-drop windows, one `Play no land right now` (S-1) and it was recovered the same turn.
  14 Pathway rows taken off `TWO FACES, ONE CARD`, tapped lands played without complaint (146 seq 4).
- **Reply-label tolerance.** Two attackers answers used card NAMES instead of A-numbers (126 seq 19
  `ATTACK: Elite Spellbinder, Brutal Cathar #1, Brutal Cathar #2, Augur of Autumn`; 130 seq 18
  `ATTACK: Brutal Cathar`) and both parsed to the right creatures. No fallback, no re-ask.

## 3 — Strategy (guide-adherence) observations

**S-1 — `Play no land right now` taken once (wave-75 HIGH-2 recurs, 1 in ~40, self-recovered).**
`...-vs-ai_baka_deck162.jsonl` **seq 38** (t13). Menu: 1 `Play Lair of the Hydra [enters TAPPED ...]`,
2-5 four Pathway faces (untapped), 6 `Play no land right now`. Answer `CHOICE: 6`. The reasoning names the
rule and then the excuse the guide already forbids: `The guide says "Play a land every turn you can". But
playing a land that enters tapped doesn't help this turn. It's fine. I'll stick with 6.` — it priced row 1 and
generalised to a menu where four rows were untapped Pathways. The live guide's LAND DROP rule says exactly
this: `never "Play no land right now" - ... not because the row says the land enters TAPPED`. The rule was
present, correctly worded, at the point of decision, and was paraphrased away rather than misread; the seat
then played Barkchannel Pathway at seq 41 in the same Main 1, so nothing was lost. **Surface TRUE, guide text
TRUE → adherence failure, not a guide defect.**

**S-2 — the hold was taken over an unused activated ability (162 seq 63).** The guide's `NEVER take it while
any row above it is an activated or loyalty ability you have not used this turn` was violated at the ninth
window of HIGH-2's run; Katilda's `{4}{g}{w}, Tap` sat above it unused. It cost nothing (the game ended in
that turn's combat) and the eight preceding passes were the guide-correct answer. This is the guide's NEVER
clause doing real work for eight windows and then losing to fatigue at the ninth — the engine item, not the
wording, is what is worth fixing (HIGH-2).

**S-3 — the tap-bill count rule broken three times in one turn, right all three times (wave-76 MED-2 recurs,
stronger).** `...-vs-ai_baka_deck123.jsonl` **t13 Main phase 1**: seq 17 Huntmaster (bill of ONE name, legal —
a 6/6 body), then seq 18 Elite Spellbinder `{paying this taps: Elite Spellbinder, Brutal Cathar, Katilda,
Dawnhart Prime - they cannot attack this turn}`, seq 20 Briarbridge Tracker (four names), seq 21 Ranger Class
(two names). The guide: `two or more names in Upkeep or Main 1 is a PASS, whatever the row is`. Three PASSes
owed, three rows taken — and the seq 20 plan says why: `Cast Briarbridge Tracker to trigger Intruder Alarm's
"whenever a creature enters" ability, untapping my entire board, then declare attackers with everything.`
The **opponent's** Intruder Alarm (`Creatures don't untap during their controllers' untap steps. -- Whenever a
creature enters, untap all creatures.`, glossed on their battlefield line by R5) refunded every bill. Seq 23
attacked with Katilda and Brutal Cathar — both billed — and seq 35 closed with nine attackers for 25. The rule
is a proxy for "a lost attack" and the board had removed the loss. **No edit** (A334: the hard case earns no
concessive clause, and the rule PASSED correctly at 123 seqs 28/29/32 and 162 seqs 51/66 in the same corpus).
Recorded so that the second wave in a row in which this rule is overridden correctly is on the record.

## 4 — Guide verdict: **KEEP as is**

No decision in six games was decided against the seat by the guide, and the two adherence failures (S-1, S-2)
are failures to follow text that is already present, correctly worded, in the right paragraph — S-1's excuse
("the row says the land enters TAPPED") is named verbatim in the rule the model paraphrased away, and S-2's
prohibition did its job for eight consecutive windows before the ninth. More words cannot make either rule
more explicit, and this wave's evidence pays for no new rule: the guide's decisive machinery (lethal count,
`ATTACK TOTAL` last clause, the Spellbinder named-card rule, the deploy floor, the converter section, the
"held back" tag from wave-76 Edit 2) fired in every game and was right every time. **19,972 B, unchanged; no
`wave77/deck152/strategy.txt` is written.** The one thing a later wave should carry forward is that wave-76's
byte reclamation of the Sigarda COVEN paragraph was made on a premise the corpus has now falsified (§2,
"The Sigarda COVEN reveal is BACK") — that is a note for whoever next needs bytes here, not an edit this
wave's decisions paid for.

## 5 — Proposals

**None.** HIGH-2 is an engine window-shape item with a repro, not an instruction-surface question — the guide
already answers it correctly, and the fix is a cheaper escape row. S-3 and the converter/lethal collision in
§2 are both A334 cases already settled by amendment; restating them would be noise under this wave's standard.

## 6 — What I did NOT check

- Any seat but 152's, except the six opponent seat files read for context — and of those only deck130's
  (for HIGH-1) and deck126's (for the converter) in any depth.
- stderr: only `game-152v126-...stderr` (for LOW-2), by grep. The other five, and every engine-side
  reconciliation (close counts, replay lines, the harvest manifest), are untouched here.
- Corpus-wide lane adjudication. I verified R2/R5/R7-aggregate ONLY on my seat's own windows, and R7's
  `{crack-back cover:}` half, R6's targeted-removal/X-sub-menu/stay-home cover rows, R3's sacrifice and edict
  clauses, R4's blockers AT BEST figure, R10's ANNOUNCE_X plan echo and CU F1's balanced-strip key not at all —
  **none of those surfaces fired in any of my six games** (0 cover clauses, 0 blockers windows, 0 sacrifice or
  edict rows on this seat, 0 ANNOUNCE_X windows of my own).
- Wave-76 Edit 1 (`DECLARING A BLOCKER DOES NOT TAP IT`) is **UNTESTED**: this seat had zero `blockers`
  records in six games. So is the whole BLOCKING section, Katilda's B-row rule, the Lair of the Hydra
  animation rule, the reserve-VERDICT pass (3 prints, all declined, but none was the STRANDS shape), and the
  MATCHUPS counterspell rule.
- The 4 `async_drops` in the deck126 game and the 1 in deck123: not traced to their re-asks. Record-`seq` gaps
  (146: 31-36, 123: 30/33, 125: 28-31/36, 162: 39/67) I noted but did not reconcile against the window counter.
- No card was verified against Oracle text this wave; nothing in my six games turned on a suspect card
  behaviour. Intruder Alarm's rendered text (S-3) matches its real Oracle text but I did not open its
  primitive. Deck 152's own primitives were last verified in waves 72-73 and I did not re-check them.
