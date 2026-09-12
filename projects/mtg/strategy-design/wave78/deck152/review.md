# Wave-78 deck 152 review — BANT HUMAN MIDRANGE

Corpus `matchups-20260911-125420-final`, SINGLE binary `wagic-47ef378c8-w78step1`. My seat files
`<epoch>-ai_baka_deck152-<ptr>-vs-<opp>.jsonl`; the six opponent seat files read for context. Live guide =
the wave-76 edition, **19,972 B** (deck152 was KEEP in wave 77, so nothing was installed for it at 031db297b).

**225 model decisions** (123: 20 · 126: 31 · 125: 62 · 146: 61 · 162: 22 · 130: 29), 100% with native
reasoning, `thinking` `on` on all 225, reasoning median 5,751 / max 16,820 chars. Latency p50 106.8 s,
max 345.1 s. Max prompt **28,621 B** (146 seq 60), **21 of 225 prompts over 20 KB** (wave 77: 25,953 / 19).

Seat **4-2**, down from 6-0. The two losses are not guide effect in the sense wave 77's brief warns about —
one is a turn-8 opposing combo the seat had no answer to, the other is one misplay (§3 S-1) — but the
hands were real and the swing is not evidence about the guide either way.

**0 `unparsed_reply`, 0 `answer_replaced`, 0 `reply_truncated`, 0 `action_before_plan`** on all six files.
`protocol_deviation`: compliant 204 / `unlabelled_plan` 17 / `plan_absent` 4 = **21 `plan_line_missing`
(9.3%, wave 77: 4.3%)** — see LOW-1. One `reveal`/`engine_answered` record (`152v123` seq 22, the
corpus-wide census anomaly) is on this seat and is MED-3.

Gameend counters by game (123/126/125/146/162/130): `hold_windows_skipped` 0/0/**45**(all cast)/0/0/0,
`ask_replays_reserved` 1/0/4/1/0/1, `identical_ask_answers_reserved` 0/0/1/1/0/1,
`identical_option_asks_resolved` 0/0/0/**6**/0/0, `main_phase_windows_skipped` 5/2/0/16/12/14,
`own_turn_windows_skipped` 30/47/0/55/34/40, `async_drops` 0/1/1/1/1/0,
`blocker_forecast_rows` 4/16/0/22/20/2 (`_gang` 4/0/0/6/0/2, `_collapsed` **4**/0/0/0/0/0),
`chain_acting_rows(_cast)` 0(0)/0(0)/0(15)/29(0)/2(0)/0(0), `chain_selfharm_rows` 0/0/0/0/2/0,
`crossphase_identical_reputs` 0/0/1/0/0/0, `forced_close_events` 0/0/4/0/2/0,
`forced_close_unrecorded` 0/0/2/0/1/0, `hold_released_turn` 0/0/1/0/0/0, `plan_line_missing` 0/4/6/5/2/4.
**Zero across all six**: `stack_drain_windows_asked`, `stop_reached_windows_skipped`,
`stop_reached_reputs_collapsed`, `own_loop_windows_asked`, `mana_only_windows_skipped`,
`crossphase_board_unchanged`, `declined_face_latches`, `plan_names_stranded_card`,
`plan_names_uncastable_zone_card`, `repeat_annotated_takes`, `reserve_decline_windows_noted/_skipped`,
`menu_pass_no_progress`, `engine_reveal_floor_picks`, `phase2_answer_recovered/_missing`,
`force_close_arms_refused/_deferred/_same_arm_deferred/_defer_bound_hits`, `wall_miss_*`, `ask_replays_refused`,
`put_gloss_stripped`, `sibling_window_asks_skipped`, `answer_label_absent_heuristic_played`, `defer` records.

## 1 — Game by game

| # | Opp | Result | Turn | Life | Deciding decisions |
|---|-----|--------|------|------|--------------------|
| 1 | 123 | **loss** | 10 | -46 / 1 | Opponent's t8 Intruder Alarm + Thraben Doomsayer made **25 Humans in one turn** — the seat had no priority window during that turn and nothing at instant speed. Seat's t9 was near-perfect: seq 17 took the largest counter row its 5 mana paid for, seq 18 Elite Spellbinder (which untapped Katilda + Briarbridge through **their** Intruder Alarm, so both attacked), seq 19 exiled Damnation, seq 21 sent all three for **15 into a 16 life total**. `ATTACK TOTAL` said so truthfully: "declaring all of them with none blocked puts them at 1. That is NOT lethal". Died at t10 to 27 attackers. **No line reached 16; the counter menu's own rows capped at 2 counters** (§2 HIGH-2 is the one false surface in this game, and the model beat it). |
| 2 | 126 | **loss** | 18 | 0 / 28 | **seq 23 (t15) is the game** (§3 S-1): 4 attackers / 23 power offered, `ATTACK TOTAL` "blocking can leave them as high as 15" against a **22** life total, both blockable rows tagged "(you kill it, your attacker lives)" — the seat sent **Sigarda alone** on a stated but false reading of their Exquisite Blood. seq 27 (t17) sent Sigarda alone again and **that one was right** (LOOP COMPLETE by then). Opponent won on t18 upkeep with one Staff of Nin ping into Sanguine Bond + Exquisite Blood. |
| 3 | 125 | win | 17 | 28 / -2 | Lightmine Field game. seq 45/46 planned an all-in, then **seq 60 correctly refused it** ("deals 7 damage to each of my 7 attackers"); seq 68 sent exactly the two Huntmasters that survive 2 damage. 45 `hold_windows_skipped` off three guide-legal holds (seqs 20/41/60, only a Fateful Absence row above each). |
| 4 | 146 | win | 16 | 19 / -1 | seq 26 Sigarda blocks Nadaar (rung 1); seq 45 five-attacker swing takes them 16 → 1; seq 55 forced sacrifice; seq 61 three attackers close it. |
| 5 | 162 | win | 13 | 17 / -6 | seq 6 Brutal Cathar exiles Shield Sphere; seq 21 two valor counters; seq 22 three attackers; **seq 24 declined a cast under Forced Fruition** (their "opponent casts a spell → that player draws seven") — an exception the guide does not name and the model got right unaided; seq 25 four-attacker lethal. |
| 6 | 130 | win | 15 | 24 / -3 | seq 10 Spellbinder exiles Rorix Bladewing; seq 17 exiles Molten Rain; seq 18/25 flier beats; seq 31 Sigarda for lethal at 1. |

The guide's load-bearing machinery fired and was right: the `ATTACK TOTAL` last-clause rule (6/6 closing
combats, and it is what the seat **failed to follow** at 126 seq 23), the Spellbinder named-card rule (Damnation
at 123 seq 19 off "kill or exile … on sight"; Rorix and Molten Rain at 130), the deploy floor (0 quiet-board
declines with an untaken creature row outside a cleared count or the Forced Fruition window), the HOLD-PRIORITY
NEVER clause (3 holds taken, an instant above each, **0 taken over an unused activated or loyalty ability** —
wave 77's S-2 does not recur), the block ladder (3 blockers windows, rungs 0/1 taken), and the LIFE-TO-DAMAGE
CONVERTER section's absolute LOOP clause (126 seq 27).

## 2 — Engine / interface / card items

### HIGH-1 (RENDER, false clause) — `{paying this taps: X, Y - they cannot attack this turn}` is stated as fact and was FALSE twice in one turn under an opposing Intruder Alarm
`1789149273-ai_baka_deck152-...-vs-ai_baka_deck123.jsonl` **seq 17** row 3 and **seq 18** row 3, both t9 Main 1:
`{paying this taps: Katilda, Dawnhart Prime, Briarbridge Tracker - they cannot attack this turn}`.
The opponent's Intruder Alarm is on their battlefield and glossed in the same prompt
(`Whenever a creature enters, untap all creatures.`). Elite Spellbinder entering at seq 19 untapped both,
and **seq 21 offered A1 Katilda (4/4) and A3 Briarbridge Tracker (5/6) as legal attackers** — they attacked
and dealt 9 of the turn's 15 damage. The clause is the single input the guide's tap-bill PASS rule keys on,
so a false one is a rule-grade error, not a cosmetic one: obeying it would have cost the seat two attackers.
Fix shape: the forecast is a prediction and should be worded as one, or should consult any "whenever a
creature enters, untap" effect in play before asserting "they cannot attack this turn". Repro: that file,
seqs 17/18 (the bill), seq 21 (the A-rows), and the Intruder Alarm gloss in either prompt's opponent line.
**Third wave in a row this rule is overridden and the override is right** (wave 77 §S-3, wave 76 MED-2) —
which is the evidence that the clause, not the rule, is what needs fixing.

### HIGH-2 (INTERFACE, ask cost) — a 21-row menu is still put for a decision whose real choice set is three
Same file **seq 17**: 21 options, rows 4-20 folded to one line reading `add 3 counters {same effect right
now: adds 2 counters} … add 19 counters … x17`. **Wave-77 MED-3's fold is LIVE and works** — the option
vector is intact at 21, the fold text is true, and the answer (`CHOICE: 3`) named a fully-quoted row. What
remains is that the window is still 21 options wide for a decision with three distinct outcomes (0, 1, 2
counters), and both Adversary windows this wave (123 seq 17, 126 seq 22) are that shape. The next step the
fold enables and has not taken: put the menu as the three payable rows plus one "largest the card offers"
row. Repro: `152v123` seq 17 and `152v126` seq 22, `options: 21` on both records.

### MED-1 (RENDER, self-contradicting sentence) — the hold-check bracket calls a price change both "the SAME row" and "a different cost"
`1789149277-...-vs-ai_baka_deck125.jsonl` **seq 66** (prev record seq 65, priority seam, t17). The two menus
are byte-identical except `Put a level counter with Ranger Class [cost: {1}{g}]` → `[cost: {3}{g}]`. The
bracket reads `1 row above is new … and 1 row that was on it is gone - a row that changes only in its
annotations (**a price**, a forecast, a clock, a count) is the SAME row and does not re-open a hold; only a
row appearing, disappearing, or naming a **different card, cost or target** does`. The same sentence names
"a price" as SAME and "a different cost" as NEW, and this row is both. Counting it as new is defensible under
MTG rules (it is a different level), so the defect is the wording, not the count: the "a price" example must
not cover a row's own activation cost. (Wave 77's seat read the same shape as TRUE on the rules side; I am
flagging the sentence, not re-opening that.) Repro: that file, records 65 and 66, option row 1 of each.

### MED-2 (RECORD, reconciliation) — `forced_close_unrecorded` and the stderr summary line still disagree, and the new records show why
`152v125` carries `forced_close_events: 4` / `forced_close_unrecorded: 2`, `152v162` carries 2 / 1.
**The S11 per-event record works**: the six `forced_close` records pair 1:1 on `window_seq`
(`152v125` seqs 11+12 on window 9, 48+49 on window 42; `152v162` seqs 12+13 on window 10), each pair being
one `outcome: "armed"` followed by one `outcome: "dropped_decision_moved"`, every one with `arm: "cast"`,
`defer_ticks: 0`, `park_armed: 0`. And the identity closes against stderr: `game-152v125-…stderr` has
**2** `unclosed <think>` lines and the file has 2 `armed` records; `game-152v162-…stderr` has **1** and the
file has 1. So the corpus's 16 `forced_close_unrecorded` are exactly the `dropped_decision_moved` half of the
pairs — they are no longer unattributable, and the counter's NAME is now the wrong thing about them. What
still disagrees is the stderr per-seat summary, which reports `deadline misses: 0 (0 unrecorded, 0 no-retry)`
for **both** seats of both those games (wave-77 LOW-2 recurs unchanged). Repro: those two files' `forced_close`
records and the two stderr files. No decision in either game was answered by a heuristic.

### MED-3 (RECORD, census) — the `reveal`/`engine_answered` record has no prompt, no reply and `latency_ms: -1`, and its own `recovery` record disagrees with it on the life totals
`152v123` **seq 22** (t9, Blockers): `kind: reveal`, `fallback: engine_answered`, `options: 5`, `choice: -1`,
`chosen_text: "none (no legal target)"`, `coded_answers: 0`, `reply: ""`, no `prompt` field,
`reveal_wait_ticks: 2`, `reveal_wait_unexplained_secs: 0`, and `my_life: 20 / opp_life: 16`. The **seq 23**
`recovery` record that names it (`recovers_seq: 22`, `recovers_kind: "reveal"`,
`recovers_fallback: "engine_answered"`) carries `opp_life: 1` — the post-combat figure — so the two records
of one event straddle the damage step and a per-seat census reading life off `kind` gets two different
answers. The engine answered correctly (Sigarda's Coven reveal turned up
`Barkchannel Pathway; Ranger Class; Barkchannel Pathway; Lair of the Hydra; Fateful Absence` — **no Human**,
so `get a human` had no legal target and there was nothing to ask). The item is that this is counted as a
FALLBACK in the corpus census while it is a window that should never have been opened: a reveal whose
eligibility filter selects zero rows is not a decision. Repro: that file, seqs 22 and 23.

### MED-4 (INTERFACE, decode cost) — the Teferi `+1` chain recurs for the third wave, now 11 calls in one game
`1789149284-...-vs-ai_baka_deck146.jsonl` **seqs 18-24** (t10, 7 calls) and **seqs 30-35** (t12, 6 calls);
`152v125` seqs 33-41 (t13, 8 calls). Each `+1` walks `+1` → `choose a land` → `choose your land` → the land →
`choose a creature` → `choose opponent creature` → the creature → `choose an artifact` / `Decline`. **13 of
the 146 game's 61 decisions (21%) and 11 of the 125 game's 62 are loyalty-ability plumbing**, at p50 107 s.
Unchanged from wave 77 MED-1 and wave 76 MED-4; one window carrying stage + side + target, or a pre-declined
empty stage, still buys back most of it.

### LOW-1 (protocol) — `plan_line_missing` DOUBLED on this seat, 4.3% → 9.3%
21 of 225 records: **17 `unlabelled_plan`** — a correct one-sentence plan with the `PLAN:` label dropped
(`126` seqs 20/24/28 · `125` seqs 36/43/61/63/67/68 · `146` seqs 17/29/33 · `162` seqs 16/24 ·
`130` seqs 23/28/31) — and **4 `plan_absent`**, a bare action line with no plan at all
(`126` seq 26 `CHOICE: 6 (Cast nothing right now)` · `146` seqs 36 `CHOICE: 1 (Sigarda, Champion of Light)`
and 37 `ATTACK: A1, A2` · `130` seq 24 `CHOICE: 2 (Elite Spellbinder)`). Two of the four `plan_absent`
records are consecutive (`146` 36 → 37, the counter placement and the attack it feeds), which is the shape
worth watching: the seam where the plan disappears is the seam where the action is conditioned on nothing.
Three records write `Plan:` in lower case (`162` seq 25, `130` seq 26, `125`'s tail) and parse fine. No
record carries prose outside the two lines; `off_protocol_bytes` is again measuring the missing label.

### LOW-2 (STRATEGY-adjacent, model text) — a plan named a removal spell that cannot hit its target
`152v125` **seq 63**: `I will resolve Teferi's +1 for 2 life, then cast Fateful Absence to destroy Lightmine
Field before attacking.` Fateful Absence destroys a creature or a planeswalker; Lightmine Field is an
enchantment, and the guide says so ("nothing here destroys an enchantment"). The seat never acted on it —
seq 65/66 levelled Ranger Class and passed — so nothing was lost. Recorded because it is the same failure
class as §3 S-1 (a card's reach asserted from memory instead of from the row) in a window where it cost
nothing.

### Verified NOT defects on this seat this wave
- **S9(b) / wave-77 MED-2 — the `N gone` clause is LIVE and my own wave-77 item is CLOSED.** 124 `[hold check:`
  brackets, and the disappearing half now prints (`1 row that was on the menu at the last window I asked you
  at this seam is gone and no row above is new`, `2 rows … are gone`, and the mixed form `1 row above is new
  … and 2 rows that were on it are gone`). Of the 106 brackets that name a count, **105 are TRUE** against
  the row sets of the record their `hold_check_ref_seq` names (8 of my checker's flags resolve as
  normaliser artifacts on inspection — nested braces in card text and the pass-row suffix); the one residual
  is MED-1's wording, not a false count. **18** `this is the first window I have asked you at this seam`
  sentences, and all 18 carry `hold_check_ref_seq: -1` — none claims a first window while naming a
  predecessor, and none follows an asked predecessor at that seam.
- **S7 — `hold_check_ref_seq` IS NOW A RECORD SEQ and joins.** 124 records carry it; every non-`-1` value
  resolves to an earlier record in the same file, `hold_check_ref_window` carries the ordinal beside it, and
  **0 of 106 resolve to a record of a different `kind`** (wave-77 MED-4, my own item, CLOSED). Cross-turn
  joins work (`126` record 31 → record 9, `125` record 55 → record 31).
- **S12 — `Opponent life trend:` prints gained and lost, and both figures are TRUE.** e.g. `126` seq 23
  `turn 11: 25, turn 13: 25, turn 15: 22, now 22 (-3 since turn 11; over those turns life-gain EVENTS put +1
  on them and life-loss EVENTS took -4 off …)` — 25 − 4 + 1 = 22, and the +1/−4 match the narrated events of
  that span; `126` seq 27 `+2 / -10` from 25 to 17 likewise. Checked on every trend line in the 126 game
  (the only one of my six with a two-sided life stream); **0 net-only lines with a live event source**.
- **S8 — the `{effect:}` gloss cuts at clause boundaries.** 36 distinct glosses, 135 `(...more)` marks, and
  every cut I read ends a whole clause or sentence: `"When Intrepid Adversary enters, you may pay {1}{W} any
  number of times. (...more) -- Creatures you control get +1/+1 for each valor counter …"`,
  `"Equipped creature has haste and shroud. (It can't be the target of spells or abilities.) (...more)"`
  (parenthesis kept atomic). **0 mid-clause cuts, 0 false statements.** Sigarda's gloss still truncates with
  a bare `…` at `look at the top five...` (no `(...more)`) — it is the trailing clause of a Coven reminder
  and states nothing false, but it is the one gloss not using the new marker.
- **S15 — no-op rows folded, option vector intact.** 21 options on both Adversary menus, the folded band
  quoted at both ends, and neither answer named a folded row.
- **S3 / S4 / S6 / S10 — none of the four fired on this seat.** `stop_reached_windows_skipped` 0,
  `stop_reached_reputs_collapsed` 0, `stack_drain_windows_asked` 0, `own_loop_windows_asked` 0, `defer`
  records 0. **Wave-77 S4 was MY item** (the 7-link drain, `152v162` seqs 55-63): the shape did **not** recur
  — no window in these six games carries `their stack is draining`, and the longest identical-window run on
  this seat is 2. So the S4 annotation is UNTESTED from my seat, not confirmed.
- **S5 / S14 — no counterspell rows, no `[X pricing]` block, no `[second copy:` clause in any of my 225
  prompts.** Deck 152 runs none; UNTESTED here.
- **`async_drops` 4 (126/125/146/162, one each) and 7 record-seq gaps.** The gaps are
  `123` 11 · `125` 19/39/58/71 · `146` 59 · `130` 29 (126 and 162 have none). Each is followed by an answered
  window at the same seam in the same phase, and no option row present before a gap is absent after it. I did
  not join the 4 `async_drops` to particular gaps — there are 7 gaps and 4 drops, so at least three gaps are
  something else, and I did not identify what.
- **Mulligans.** 2 mulligans (`126` seq 2, `130` seq 2), both to 6, both then kept, both bottom asks answered
  (`PUT: 3` each, the duplicate/priciest card) — wave-75 Edit 2 passes a third wave, no invented quotation.
- **Land drops.** ~30 land-drop windows, **0 `Play no land right now`** — wave-77 S-1 does NOT recur.
  23 prompts carry `TWO FACES, ONE CARD` and every Pathway row taken was the face the hand needed
  (`126` seq 6 Tidechannel for blue into Teferi; `162` seq 4 Hengegate for white).
- **The Sigarda COVEN reveal is the seat's busiest surface.** 62 prompts carry `get a human`, 8 carry
  `[does NOT qualify]`, and every pick was legal — including `123` seq 22's zero-eligible case (MED-3).
  Wave 76's byte reclamation of the COVEN paragraph was made on a two-wave silence that has now been
  falsified twice running; a later wave should not re-cut it on that premise.
- **Reply-label tolerance.** Three attackers answers used card NAMES instead of A-numbers (`126` seq 23
  `ATTACK: Sigarda, Champion of Light`; `130` seqs 25/31 `ATTACK: Sigarda, Champion of Light`) and all three
  parsed to the right creature. No fallback, no re-ask.

## 3 — Strategy (guide-adherence) observations

**S-1 — THE DECIDING MISPLAY OF THE CORPUS FOR THIS SEAT: three attackers held on an inverted reading of an
opponent-seat drain card.** `1789149275-...-vs-ai_baka_deck126.jsonl` **seq 23** (t15, Attackers).
Board: opponent at **22** with exactly two creatures, `Wall of Omens (0/4) [defender]` and
`Pride Guardian (0/3) [defender]`, one card in hand, and **Exquisite Blood alone** (Sanguine Bond did not
arrive until t16/17). Offered: A1 Sigarda 6/6 `[no creature they control can block this attacker]`,
A2 Tovolar's Huntmaster **9/9**, A3/A4 Wolves **4/4** each — every blockable row tagged
`(you kill it, your attacker lives)` with a `(blocking trigger: they gain 3)` price on Pride Guardian.
`ATTACK TOTAL: 4 attackers listed, 23 total combat damage … At least 10 damage lands whatever they block -
that damage alone puts them at 12 … blocking can leave them as high as 15.`
Answer: `ATTACK: Sigarda, Champion of Light`, with the reason stated in the plan:
`Exquisite Blood converts all combat damage into life gain for the opponent`.
**That is false, and the prompt says so.** Their Exquisite Blood is rendered with the opponent-seat prefix
and reads `Whenever an opponent loses life, you gain that much life` — it triggers on life **the seat**
loses, never on damage the seat deals them. Every surface was TRUE: the card text, the per-row tags, the
`ATTACK TOTAL` line. The guide's own rule pointed the other way and the seat had it: `blocking can leave them
as high as N` with **N = 15 BELOW their 22** is the branch that reads "send the blockable bodies too, priced
by the per-creature tags below", and all three tags read rung 1. 13 power was held for nothing. At the
floor of 10 the opponent finishes t15 at 12 rather than 22, and Sigarda's 6 on t17 puts them at 6 with the
seat's whole board untapped — the game is over before the t18 Staff of Nin ping starts the loop.
**PERCEPTION of a true surface, decided against the guide — but the defeater is a card-direction claim the
guide never answers**, which is what the edit below buys. (seq 27, the same shape at t17 with the LOOP
COMPLETE header printed, was answered correctly by the converter section's absolute clause — the section
works where it is in scope.)

**S-2 — the tap-bill PASS rule broken twice in one turn, right both times, THIRD wave running.**
`152v123` seqs 17 and 18: `{paying this taps: Katilda, Dawnhart Prime, Briarbridge Tracker - they cannot
attack this turn}` on both rows taken, which is `two or more names in Upkeep or Main 1 is a PASS`. Both were
taken and both were correct, because the opponent's Intruder Alarm untapped everything when Elite Spellbinder
entered and both billed creatures attacked (HIGH-1). The rule is a proxy for "a lost attack" and the board
had removed the loss. **No edit** (A334: the hard case earns no concessive clause, and the rule's premise —
the clause it reads — is the thing that is false, so the fix is HIGH-1, not a guide qualifier). Recorded so
that the third consecutive correct override is on the record.

**S-3 — an unnamed exception the model handled unaided.** `152v162` seq 24: under their Forced Fruition
(`whenever an opponent casts a spell, that player draws seven cards`) the seat declined a castable creature
(`CHOICE: 8 (Cast nothing right now)`) with the stated reason `hold all spells due to Forced Fruition`, and
won on the following combat. This is outside the deploy floor's three listed cases and the guide does not
name it. One instance, correct, costing nothing — **no edit**; a rule for it would be a card patch for a
board that appeared once.

## 4 — Guide verdict: **EDIT** (19,972 B → **19,993 B**, net +21)

One decision in six games was decided against the seat by an answer the guide does not supply (S-1), and it
decided a loss. The edit adds **one rule** and pays for it by removing one sentence the per-creature tag
ladder already supersedes plus two tightenings; no rule is dropped.

**Edit 1 — NEW RULE, ATTACKING section, immediately after the `ATTACK TOTAL` block.** *Paid for by
`152v126` seq 23.* Earns its bytes because it is the only rule that answers the window S-1 lost: a half-loop
drain card on their line with no converter header printed and no engine clause in scope.
- before: (no such rule; the section went straight from the `ATTACK TOTAL` compare to
  `Otherwise send each listed creature with any of: …`)
- after: `WHAT YOUR ATTACK COSTS YOU IS PRINTED ON THE A-ROWS, NOWHERE ELSE. Damage you deal them is life
  THEY lose, and no card of theirs turns that into life for them. The only life an attack hands them is a tag
  on an A-ROW - a "(blocking trigger: they gain N)" or a lifelink result - so price it off the ATTACK TOTAL
  line and those tags alone, never off a drain or lifegain card named on their battlefield line. No such tag
  on a row means that attacker costs you nothing: send it.`
  (Heading is a scope declaration, A333. No concessive clause for the LOOP case, A334 — the converter
  section already owns that board and is unchanged.)

**Edit 2 — reclaim, ATTACKING section.** *Paid for by the same seq: the sentence is superseded two lines
later by `Its "their untapped blockers" tag is a per-creature verdict:` and its ladder, which the engine
prompt itself calls authoritative (`Do not re-derive these outcomes; use them`).*
- before: `Otherwise send each listed creature with any of: no untapped blocker opposite it; flying they
  cannot block (Elite Spellbinder and Sigarda FLY); toughness beating the biggest thing that can block it; an
  even-or-better trade.`
- after: (removed) — −224 B.

**Edit 3 — reclaim, LETHAL COUNT.** *Redundancy only; the rule and its threshold are unchanged, and S-2
shows the rule itself needs no rewording.*
- before: `SEARCH EVERY CAST ROW FOR THE WORDS "paying this taps" BEFORE YOU TAKE IT - the source count
  beside it counts lands and creatures alike and never names a body. THEN COUNT THE NAMES IN THE BILL AND
  OBEY THE COUNT - quoting the bill and taking the row anyway is the same lost attack: two or more names …`
- after: `SEARCH EVERY CAST ROW FOR "paying this taps" BEFORE YOU TAKE IT - the source count beside it counts
  lands and creatures alike and never names a body. COUNT THE NAMES IN THE BILL AND OBEY THE COUNT: two or
  more names …` — −84 B.

**Edit 4 — reclaim, MATCHUPS / `ONE of the two names` bullet.** *Wording only; every clause survives.
Confirmed still needed by `152v126` seq 23, which is exactly this bullet's board.*
- before: `ONE of the two names -> race it with EVASION. An A-line reading "[no creature they control can
  block this attacker]" is free in both directions, since no block means no blocking trigger and no life
  gained: SEND every such creature … The tag prints both halves ("they gain 3, and their converter takes 3
  off you"), so an extra attacker past N is worth declaring when the power it adds beats TWICE that gain
  number. …`
- after: `ONE of the two names -> race it. An A-line reading "[no creature they control can block this
  attacker]" is free in both directions: SEND every such creature … the surplus above N lands its full power,
  so an extra attacker past N is worth declaring when the power it adds beats TWICE the gain number its own
  tag prints. …` — −145 B.

Full revised guide at `wave78/deck152/strategy.txt`, **19,993 B**, no citation, count, history, corpus, wave
or seq reference in it, no `YOUR PLAN` quotation, PLAN template untouched (the guide carries none).

## 5 — Proposals

**None.** HIGH-1 is an engine forecast that states a prediction as fact — an engine fix with a repro, not an
instruction-surface question. MED-1 is a sentence the engine prints. S-2 and S-3 are A334 cases already
settled by amendment. Nothing here generalises past this deck in a way the general guide or the
strategy-writing skill does not already cover.

## 6 — What I did NOT check

- Any seat but 152's, except the six opponent seat files for context — and of those, only deck126's
  (for the t18 kill) and deck123's (for the t8 Intruder Alarm turn) in any depth.
- stderr: only the six `game-152v*`/`game-146v152` files, and only by grep for `unclosed <think>` and the
  per-seat summary line. The harvest manifest, replay lines and every other engine-side reconciliation are
  untouched here.
- Corpus-wide lane adjudication. I verified S7/S8/S9/S12/S15 **only on my seat's own windows**, and S3, S4,
  S5, S6, S10, S14 not at all — **none of those surfaces fired in any of my six games** (0 drain
  annotations, 0 stop-reached rows, 0 own-loop windows, 0 counterspell rows, 0 `[X pricing]` blocks, 0
  `[second copy:` clauses, 0 `defer` records). S13's hoisted-shared-card-text half: my seat had no
  shared-source target menu, so it is UNTESTED here; I report only the byte figures.
- The 6 corpus `answer_label_absent` records and the deck162 `unlabelled_plan` fallback: all on other seats.
- I did not join the 4 `async_drops` to the 7 record-seq gaps, and did not identify what the other three
  gaps are; I confirmed each gap is followed by an answered window at the same seam and stopped there.
- No card was verified against Oracle text this wave. Intruder Alarm, Exquisite Blood, Sanguine Bond, Pride
  Guardian, Lightmine Field, Forced Fruition and Staff of Nin all behaved as their rendered text says, and
  their rendered text matches my recollection of their Oracle text, but **I opened no primitive**. Deck 152's
  own primitives were last verified in waves 72-73 and I did not re-check them.
- Wave-76 Edit 1 (`DECLARING A BLOCKER DOES NOT TAP IT`) is still effectively UNTESTED: 3 blockers windows,
  all rung 0/1 or forced-lethal, none turning on the tap question. So are the Lair of the Hydra animation
  rule (5 `becomes a` menus, all declined, none in Main 1 with spare mana), the reserve-VERDICT pass
  (1 `reserve:` print), the MATCHUPS counterspell rule, and Katilda's B-row rule.
