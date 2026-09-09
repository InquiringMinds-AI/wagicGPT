# Wave 73 — engine-narration seat

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-225637-final` (binary `622b10827` =
`archives/wagic-4b90ed7d7-w73step1`, `--thinking on`, `WAGIC_GPT_TIMEOUT=900`, no operator token
ceiling). 21 clean games = 14 from the original run + 7 reruns after the pilot-server wedge and
restart, same binary; the reruns ran at `-j 6` or less, so **every latency comparison against wave
72 is an unequal-load comparison and is marked as such**. Baseline for ratios: wave 72's
`matchups-20260907-163626` + `wave72/engine-seat.md`. Read-only: no build, no run, no git, nothing
written outside `wave73/`.

**What I did NOT check.** No deck-guide reading and no strategy verdicts (deck seats own those). No
per-game narrative tracing beyond the decisions cited. No Scryfall calls — card facts were read off
the rendered prompt and, for the ghostform item, off `bin/Res/sets/primitives/planeswalkers.txt`
(Kaya the Inexorable, lines 1871-1881). PARSETEST/suite numbers are quoted from the lane files, not
re-run; no lane fixture was re-run. I did not read 2,338 reasoning bodies — every prediction whose
subject is a reasoning-body claim is marked UNTESTED. `serializeGameState()` is not the prompt, so
the §2 sibling board comparison is prompt-derived and is an approximation of the engine's own key
(the acting-row half of that verdict is exact and does not depend on it). I did not re-derive the
suite's coverage of any wave-73 surface.

---

## 1. Corpus census

| | wave 73 | wave 72 |
|---|---|---|
| seats / games | 42 seats, 21 games | 42 seats, 21 games |
| completed naturally | 42 gameend, 21 `results.tsv` rows, no `hang` row | 42 / 21, no hang |
| crashes / harness timeouts | 0 / 0 | 0 / 0 |
| **model decisions (records carrying a `prompt`)** | **2,338** | 1,910 |
| by kind | ask 1,757 · priority 416 · attackers 83 · discard 46 · blockers 23 · reveal 10 · bottom 3 | ask 1,393 · priority 351 · attackers 102 · discard 26 · blockers 18 · reveal 17 · bottom 3 |
| summed gameend turns / decisions per turn | 928 / **2.52** | 902 / 2.12 |
| same, excluding the two deck-125 stall games | 1,391 / 648 = **2.15** | — |
| fallbacks | **0**, of any kind | 3 |
| `transport` | `curl=0,http=200,empty=0` on **2,338 / 2,338** | 1,909/1,910 clean |
| `async_drops` | 43 over 42 records (`answer_replaced` 0) | 32 over 25 |
| `deadline_pct` | p50 7.1 · p95 19.1 · max 39.8 | p50 10.0 · p95 21.4 · max 51.8 |
| latency (UNEQUAL LOAD) | p50 **64.0 s** · p95 172.4 · max 358.6 | p50 90.1 · p95 193.3 · max 470.2 |
| HOLD rows rendered / taken | 1,592 / **275** (cast 1,176/218 = 18.5%, priority 416/57 = **13.7%**) | 1,181 / 260 (cast 19.3%, priority 28.5%) |
| `hold_windows_skipped` (cast / priority) | 2,168 (1,557 / 611) | 1,987 (1,421 / 566) |
| game length (turns) | mean 22.1, median 18, max **77** | mean 21.5, median 17, max 64 |
| stderr | 3.8 MB total, largest `game-125v123` 1.2 MB | 44 MB total, largest 304 KB |
| wins | deck152 5/6 · deck162 4/6 · deck146 4/6 · deck130 3/6 · deck123 2/6 · deck126 2/6 · deck125 1/6 | — |

**⚠ The brief's own census line is ask-seam only.** `1,757 model decisions … decisions/turn 1.89 …
reasoning_chars median 5,709 … latency p50 60.8 / p95 169.0` are, to the digit, the statistics of
the 1,757 `kind: ask` records alone (I reproduced all four exactly by filtering to that kind). The
corpus has **2,338** decisions and **2.52** decisions/turn. This matters for the wave's headline
reading: decisions/turn did not fall from 2.12 — it ROSE 19%, and the rise is one pair of games
(the 77-turn `125v123` and the 62-turn `125v126`); excluding them the figure is 2.15, i.e. flat.
Every "fewer decisions than last wave" conclusion drawn from 1,757 is wrong by 25%.

**Reply shape.** `protocol_replies` 2,338. `plan_line_missing` **72 (3.1%)**, `off_protocol_bytes>0`
**68 (2.9%)**, overlap 68 of 68 — as in wave 72, every off-protocol record is a mislabelled plan
line, and the class-G residue (missing PLAN, 0 off-protocol bytes) is 4 records (0.17%). By seam:
ask 51, attackers 12, discard 7, priority 2 — attackers is again the worst rate (12/83 = **14.5%**,
worse than wave 72's 11.8%; BW M15's dropped label is untouched and is the residual N17 named).
Off-protocol byte weight: n=68, sum 4,349 B, median 65, max 166. `action_before_plan_replies` 0.
`answer_replaced` 0. `post_answer_overrun>0` 0.

**Reasoning (invariant 000).** 2,338 / 2,338 records carry `thinking: on` and a non-empty native
`reasoning`. `reasoning_chars` min 737, p50 5,931, mean 6,343, max 24,849. **The corpus is valid
under 000.** Rescue markers: `reasoning_forced_close` 2 = `reasoning_budget_hit` 2 = `retry: 1` 2,
and `phase2_answer_recovered` 2 + `phase2_answer_missing` 0 = 2 — the wave-71 identity
(`recovered + missing == closes`) holds exactly, and unlike wave 72 no recovery lost its transport.

**Ceilings.** `max_tokens_answer`: ask 288 (1,755) / 400 (2 retries); priority 288 (416);
attackers 256 (82) / 288 (1); blockers/discard/reveal 256; bottom 512 (3). `reply_truncated`
present on 0 records, true on 0 — the 288/256 fit is again unrefuted. `dropped_assignments` 0 on
all 23 blockers records, so BS F5's doubled allowance is again **UNTESTED**.

**`askreplay/` sidecar.** 25 files, **1,262 records** = `ask_replays_reserved` 1,262, exactly.
`why`: `repeat_ask_reserved` 999, `cache_replay` 263. `ask_replays_refused` 0.

**`plan_steps_done`.** `(done,count)`: (0,1) 1,020, (1,2) 872, (1,3) 346, (1,4) 72, (1,5) 10,
(2,3) 8, (0,0) 7, (1,7)/(1,6)/(2,4) 1 each. The pointer reaches ≥2 on **9** records of 2,338
(wave 72: 8 of 1,910). Unchanged: the model re-states its plan far more often than it walks it.

---

## 2. REGRESSION WATCH — the wave-73 HOLD and collapse changes

### `hold_released_turn` = 101 — every release traced, **not one hold outlived its holder's untap**
The release trace names the turn taken, whose turn that was, and the turn now, so this is a
complete enumeration, not a sample: I parsed all 101 lines out of the 12 stderr files that carry
them. **Every one released at the earliest legal moment** — 44 holds taken on the holder's OWN turn
released at `heldTurn + 2`, 57 taken on the OPPONENT's turn released at `heldTurn + 1`. Zero
releases with a span greater than the predicate's minimum, so zero holds survived an untap, and
the opponent-turn class Astra's finding 2 was written about (a hold taken on their turn 14, an own
turn 15 that asks nothing, a call on their turn 16) is exactly the 57 that all released on time.
CB F2's eager `releaseHoldIfUntapPassed()` at the phase-change event is what makes this
enumerable: the release is counted where it happens rather than at the next menu.
BY N2(a) **PASS**; CB F2 **PASS**. Cost side: `hold_windows_skipped` rose 1,987 → 2,168 and its
priority half rose 566 → 611, so BY's "`hold_windows_skipped_priority` falls" half **FAILS** — but
against 22% more decisions, and with the per-hold suppression now bounded to one untap.

### The always-printed pass row — the largest behavioural change in the wave
**416 of 416** priority prompts print `0. Pass priority`, and **0** prompts advertise `0 = pass
priority` without a row (wave 72: 36 of deck146's 66). Row 0 was then actually taken **182 times**
(43.8% of priority windows). The knock-on is exactly what N2(b) predicted: the priority HOLD take
rate fell 28.5% → **13.7%**, and hold-as-pass takes (a priority HOLD whose own PLAN line says
"pass") fell **12 → 5** — and 3 of those 5 name a LATER window ("pass main phase 1"), so the
this-window hold-as-pass class is effectively 2. BY N2(b) **PASS**.

### `sibling_window_asks_skipped` = 0 — the key is repaired; **F1's subset bar is what binds**
Not a HIGH: no unseen acting row was suppressed, because nothing was suppressed. The code path is
now symmetric — `w73SiblingBoardKey` is applied at BOTH ends (`takeHold` at :28209 records the
normalised string, `holdHonoured` at :28093 compares one), so wave 72's cancelling-fixes defect is
genuinely fixed. The class is still there: **222** adjacent cast-ask → priority pairs at the same
(seat, turn, phase), **40** of them opened by a HOLD at the cast seam. Of those 40:
* **0** have a priority menu with an EMPTY acting set — every one offers at least one activation
  or ability row (`w72HeldMenuShowedEveryRow` returns false the moment it sees a row the cast menu
  could not print, and a cast menu prints no activations). Example: `126v130` deck130 seat seq
  32 → 33 (turn 10, Upkeep), whose priority menu offers three Pyrite Spellbomb targeting rows the
  cast menu never printed.
* 0 have identical boards even after normalising the answered tag — the surviving differences are
  real board movement (life totals, a permanent destroyed between the two windows, the plan
  heading's own "N windows ago" counter).

So both bars bind and F1 is the binding one, exactly as BY's weakest-evidence #2 warned. Verdict:
the collapse can only ever fire on a decline-only sibling menu, and this corpus produced none.
BY N1 **FAIL on the counter, PASS on safety**; BX F1 **PASS vacuously**. Owner decision, not mine:
either price a union rule (BY declined it, correctly on this evidence) or retire M4 and say so.

### `chain_windows_collapsed` = 0 — the collapse is unreachable, and that is the SAFE direction
`chainAutoPassApplies(chainOnStack||oppLoopProven, !anyLegalAction)` needs the seat to have NO
legal action at all. **0 of 416 priority windows in this corpus had an empty acting-row set**, and
95 of them carried a stack object — so `hasAnyLegalAction` was true at every window that reached
the ask, and the collapse never applied. CB F1 pushed it further in the same direction by two
widenings (the seat's own `GptManaPolicy` instead of `FreeProducerPolicy`, and phase-restricted
activations now visible in their own phase), both of which make the oracle find MORE legal actions
and the collapse fire LESS. Nothing was collapsed, so nothing was lost; and the wave-72 N6 repro
shape did not recur — the longest run of consecutive identical windows corpus-wide is **2** (14
runs of length 2, none of 3+), against wave 72's 12 and wave 71's 20-window chain.
BY N6 **FAIL on the counter, PASS on the shape**; CB F1 **UNTESTED** (no Petal/upkeep-only board
occurred).

### `stop_reached_windows_skipped` = 366 — every stop is the seat's OWN COUNT
Wave 72: 0, because the stop was never parsed. It parses now. **Method: complete enumeration, not
a sample.** The 366 collapses fall in 5 games and carry only **8 distinct stop values** in their
stderr traces (`stated stop (N)`), and every one of the 5 games is a deck123 seat; I matched each
of the 8 values back to the PLAN line in that seat's own translog that set it. Every one is
first-person, count-unit, own:

| stop | collapses | game | the PLAN line that bound it |
|---|---|---|---|
| 103 | 294 | `125v123` | seq 604 t68 `PLAN: stop=103, M=1; this window equip Greaves to Doomsayer, then tap Doomsayer repeatedly to create tokens until stop, then attack with all creatures` |
| 35 | 37 | `126v123` | seq 40 t10 `PLAN: stop=35, M=27; this window cast Thraben Doomsayer, then tap to make tokens until stop, then attack` |
| 22 | 9 | `126v123` | seq 77 t11 `PLAN: stop=22, M=36; this window pass, then attack with all creatures next turn.` |
| 27 | 8 | `152v123` / `126v123` | seq 103 t16 `PLAN: stop=27, M=1; this window 26, then attack with everything.` |
| 28 | 6 | `152v123` | seq 110 t17 `PLAN: stop=28, M=27; this window create 1 token to reach stop, then pass to let them attack.` |
| 26 | 6 | `162v123` | seq 18 t9 `PLAN: stop=26, M=26; pass this window, then attack all 26 creatures next turn.` |
| 24 | 5 | `162v123` | seq 11 t8 `PLAN: stop=24, M=2; this window tap Doomsayer 22 times to reach 24 creatures, then attack with everything next turn.` |
| 29 | 1 | `123v130` | seq 121 t19 `PLAN: stop=29, M=0; this window pass, then play Arcane Sanctum and hold mana for Bloodline Keeper next turn.` |

**No life stop, no mana stop, no their-stop bound anything.** Corpus-wide, 99 PLAN lines mention a
stop and **0** state it in a non-count unit (`stop at 3 life` and its family never occurred), and
the only two lines whose prose mixes an ownership word with "stop" are `125v152` seqs 16/17
("stop the 14 damage") — prose with no stop label, which the parser refuses on its own filler rule
("the" is not in the two-word filler list). So CB F3(a) and F3(b) are both **UNTESTED**: their
counterexample class did not occur, and F3's own deny-list is untested by any real reply. That is
a real verdict, not a pass — the guard is unexercised.

Also on the stop grammar: `repeatRowStopClause` rendered **49** times and `ALREADY AT OR PAST` 34
times (wave 72: **0** on 379 Doomsayer prompts) — BZ N3 **PASS**, and the closed bootstrap loop the
wave-72 seat named is open. 55 prompts print the taught bracket `stop=<N>; M=<N>`; 99 PLAN lines
carry the word stop, 68 carry a labelled `M=` count.
⚠ The 294-collapse case is one TURN (turn 67-68 of `125v123`, an Intruder Alarm + Thraben Doomsayer
untap loop). The collapse saved 294 model calls that would each have been a pass; it did not cause
the loop and it is not the defect. The loop is (see MED-1).

### `reserve_decline_windows_noted` 26 / `_skipped` 2
53 prompts render a `{reserve: ` clause; 9 prompts carry `reserve row declined earlier THIS TURN`.
CA N12(a)'s dead predicate is genuinely re-armed (both counters were 0 across all 42 gameends in
wave 72). The direction is the healthy one: N12(a)'s own risk case was "skipped rises but noted
stays 0", and the corpus is the opposite — 26 notes against 2 skips. **PASS.**

### The repeat row and its annotations
`repeat_annotated_takes` 4, all deck130 Siege-Gang Commander (n=1 ×3, n=2 ×1) — the annotation
round-trips. `repeat_count_missing` 4, all deck123, and all four are the same shape: the reply
copies the row's short name **with the placeholder letter still in it**
(`CHOICE: 2 (Create human with Thraben Doomsayer, repeated N times, then stop)`), which the row's
own bracket says is refused and re-asked. See MED-2.

---

## 3. Prediction adjudication

### lane BY
| # | prediction | verdict | evidence |
|---|---|---|---|
| N2a | `hold_released_turn` > 0; no suppressed run spans more than one of the seat's own turns | **PASS** | 101 releases, all at the predicate's minimum span (44 own-turn at +2, 57 their-turn at +1) |
| N2a | `hold_windows_skipped_priority` falls | **FAIL** | 566 → 611 (against 22% more decisions) |
| N2b | every priority prompt carries a `0. Pass priority` row; 0 sentence-without-row; the hold-as-pass share falls from 12/20 | **PASS, 3/3** | 416/416 rows, 0 orphan sentences, hold-as-pass 12 → 5 (2 of which are this-window), row 0 taken 182× |
| N2c/N9 | the cost leads every hold spelling | **PASS** | all 1,592 hold rows open `Hold priority - pass now, and do not ask me again - YOU CANNOT COME BACK AND TAKE ONE OF THE ROWS ABOVE LATER THIS TURN` |
| N1 | `sibling_window_asks_skipped` > 0; every skip's `rows now:` ⊆ `held rows:` | **FAIL (counter) / vacuous PASS (safety)** | 0 skips, 0 trace lines; 40 hold-opened pairs, 0 with an empty acting set — §2 |
| N5 | 0 prompts carry both `the HOLD row answers all of them at once` and a numbered acting row; LOOP COMPLETE and LOOP SCOPE never state opposite rules | **PASS, both** | 0 and 0; 170 LOOP COMPLETE prompts, **170** carry one of the two entry sentences, 12 also carry LOOP SCOPE and **0** disagree; the deleted false clause (`a number that reads as life to them is lethal to you`) renders **0** times |
| N6 | `chain_windows_collapsed` > 0; no turn shows 20 windows for one chain | **FAIL (counter) / PASS (shape)** | 0 collapses (unreachable, §2); longest identical-window run corpus-wide is 2 |
| N16 | `main_phase_windows_skipped` non-zero and equals own turns with no Main-1 record; the stderr reason names the instant-speed response arm | **PASS on the reason / FAIL on the equality** | 375, and **375 of 375** name the instant-speed arm (the unresolved-stack reason fired 0 times) — but 233 of the 375 phases DID put a Casting decision to the model, so the equality is false. HIGH-2 |

### lane BZ
| # | prediction | verdict | evidence |
|---|---|---|---|
| N3 | `stop=`/`M=` appear in deck123 PLAN lines; `repeatRowStopClause` renders > 0; `stop_reached_windows_skipped` can be non-zero | **PASS, 3/3** | 68 labelled `M=` PLAN lines, 49 clause renders (34 `ALREADY AT OR PAST`), 366 collapses |
| N4 | 0 `echo_index_conflict` notes on rung menus | **PASS** | 4 rung menus (≥3 rungs), **0** mis-ordered; the corpus's 2 `echo_index_conflict` notes are both on the descending X-value menu, both rescued by name match, 0 fallbacks |
| N7 | reveal/search rows print `[cost: …]` / `[no mana cost (mana value 0)]` | **PASS, 99/99** | 99 rendered reveal rows, **all 99** carry a cost tag; 3 windows use the `manacost>=3` header — `146v123` seq 16 prints `Idyllic Tutor (sorcery) [cost: {2}{w} (mana value 3)] … [eligible]` beside `Marsh Flats (land) [no mana cost (mana value 0)] … [does NOT qualify]`, and the seat took Intruder Alarm. (Counting from `options_text` gives 0/241 — the array holds bare names; the brief's count-from-the-prompt rule is load-bearing here.) |
| N10 | worked examples balanced | **UNTESTED** | my balance heuristic over 2,812 exemplified lines is unreliable (it slices at line end); I did not build a census keyed on `exampleCoreTrim`'s own call sites |
| N11 | the loyalty clause survives in the board frame | **PASS** | `to the battlefield under...` renders **0** times (wave 72: 92 prompts) |
| N15 | the noop re-ask leads with the name-match reason | **UNTESTED** | `names a row by its short name` renders 0 times — no noop re-ask with a `name_over_index` verdict fired (0 fallbacks corpus-wide) |

### lane CA
| # | prediction | verdict | evidence |
|---|---|---|---|
| N8 | every `replay_run` ≤ 64 and equals the window's own run; `replays_this_game` monotone; `ask_replays_refused` 0 unless a loop exists | **PASS, 3/3** | 1,262 sidecar records = `ask_replays_reserved` 1,262; max `replay_run` **50** (wave 72: 90 against a cap of 64); `replays_this_game` non-decreasing in **all 25** files across both `why` values; refused 0 |
| N12a | `reserve_decline_windows_skipped` non-zero; `noted` non-zero | **PASS, both** | 2 / 26, on 53 rendered reserve clauses |
| N12b | no change made | **n/a** | not re-measured this wave |
| N13 | refutation, no change | **n/a** | not re-measured |
| N14a | a creature-land animation into a listed killer is not taken, or the land loss is named | **UNTESTED** | 33 `{death price:` brackets render over 117 animation-cost brackets; the take-rate cross-tab is a reasoning-body measurement I did not build |
| N14b | a ghostform-countered blocker is not traded with as if killing it were removal | **FAIL — and the gloss is now FALSE.** | HIGH-1 |
| N14c | 0 replies call an MDFC row a menu typo; the front-face name always printed | **PASS** | 57 `PLAY THIS AS A LAND` tags; **all 29 back-face tags** name their front face (`…it is the OTHER FACE of "Brightclimb Pathway" in your hand…`), the other 28 are front-face rows which need no link; 28 `one card with two land faces` renders |
| N14d | 0 reasoning bodies call the two block tags contradictory | **UNTESTED (render PASS)** | 5 `held back` renders, **all 5** in the new `held back, THIS creature could not block <names>` subject form; no `<label>: <name>` short form survives; the reasoning-body half not measured |
| N17 | `YOUR PLAN:` at the head of a reply 10 → 0; `plan_line_missing` falls with it | **PASS on the render, PARTIAL on the counter** | `YOUR PLAN` renders **0** times in any prompt; **1** reply still opens `YOUR PLAN` (`146v130` deck130 seat seq 47). `plan_line_missing` 3.8% → **3.1%**; the residual is the attackers seam (14.5%), which N17 named and did not claim |

### lane CB
| # | prediction | verdict | evidence |
|---|---|---|---|
| F1 | no collapse on a turn whose next record offers a cast row; a sacrifice-mana seat is ASKED inside a chain; an upkeep-only activation reaches the menu | **UNTESTED** | 0 collapses at all; no Lotus-Petal-shaped board and no upkeep-only activation occurred. The widening's safe direction is confirmed only in that the collapse never fired |
| F2 | `hold_released_turn` ≥ 1 per hold spanning an untap, opponent-turn holds included; the trace names the turn taken | **PASS, 3/3** | 101 releases; 57 of them opponent-turn holds; every trace names `taken on turn N (own/their turn), now turn M` |
| F3 | no collapse against a non-count unit; none against a number attributed to the opponent | **UNTESTED** | true in the corpus (0 of 366), but no reply ever stated such a stop — the guard is unexercised, and F3's own weakest-evidence #3 (a deny-list, not an allow-list) is untouched |
| F4 | 0 prompts contain `NOT part of the chain`; a life-paying row gets the life sentence | **PASS on the first, UNTESTED on the second** | `NOT part of the chain` **0**; 110 `[LOOP RUNNING` brackets all render the "both halves of the opponent's life LOOP" head, and no Phyrexian-Reclamation-shaped life-cost row occurred to exercise the life branch |
| F5 | the marker wording prints when abilities are removed; no body treats such a creature as un-killable | **FAIL, inverted** | the marker wording printed on **56 of 56** ghostform renders with the ability LIVE — HIGH-1 |
| F6 | the meter equals own main phases that ended with no casting window; 0 in a game where every main phase casts normally | **PASS on the de-duplication, FAIL on the semantics** | every `(turn, phase)` appears **exactly once** across all traces — the per-tick over-count Astra found is fixed — but 233 of 375 counted phases DID offer a casting decision. HIGH-2 |
| F7 | `replays_this_game` monotone across both `why` values; final value = game's `ask_replays_reserved` | **PASS** | non-decreasing in all 25 sidecar files; sidecar total 1,262 = summed `ask_replays_reserved` 1,262 |

### Carried docket
- **The abandoned-{X} payment loss.** `dropped after its X was announced`: **0** in all 21 stderr
  files (wave 72: 0). Third wave unexercised — not discharged, and no deck ran a tapped-out X spell.
- **BS F1 (death-trigger lifegain).** Sanguine Bond renders in 628 prompts, Exquisite Blood in 560,
  and `125v126` ended with deck126 at **152 life** on a 62-turn drain/gain board — the enchantments
  move life. **PASS, indirect.**
- **BS F2 (Tribute's second creature).** 942 Tribute to Hunger prompts; 6 prompts render
  `is sacrificed, you gain N`. **UNTESTED** — I did not trace a two-edict turn.
- **BS F3 / the land latch.** `declined_face_latches` **0** on all 42 gameends. **UNTESTED**, third
  wave running.
- **BS F5 (blockers ceiling).** `dropped_assignments` 0 on all 23 blockers records; no
  `ceiling re-ask` line in any stderr. **UNTESTED.**
- **BS F6 (distinct-index re-ask).** no `distinct index` line in any stderr. **UNTESTED.**
- **N16's `main_phase_windows_skipped`.** adjudicated above and at HIGH-2.

---

## 4. Engine / render / narration defects

### HIGH-1 — the ghostform gloss states a FALSE fact, and contradicts another line of the same prompt
`legibleCounterNameForGrant` printed `(a marker only - the ability that returned it is gone)` on
**56 of 56** ghostform renders in the corpus, and the full gloss **0** times. In every one of those
56 prompts the granted trigger was live: Kaya the Inexorable's +1 had resolved, and neither game
contains any ability-removal effect (`/usr/bin/grep -ci "loses all abilities|Sudden Spoiling|Turn
to Frog|Merfolk Trickster" game-146v126-1788839833.stderr` = 0). So `w73GhostformTriggerLive`
answered "gone" unconditionally, and the direction is the dangerous one — the board tells the model
that killing the creature IS removal when it is not.

Repro: `1788839837-ai_baka_deck126-0x5587ed657c00-vs-ai_baka_deck146.jsonl` **seq 16** (16 renders,
seqs 16-31) and `1788839837-ai_baka_deck146-0x5587ec320be0-vs-ai_baka_deck126.jsonl` seq 13 (40
renders, seqs 13-53). The same prompt carries both readings, six lines apart:

```
- Opponent's Silverquill Silencer got a ghostform (when this creature dies or is exiled its owner
  returns it to hand and creates a 1/1 white flying Spirit - killing it is not removal) counter (now 3/2)
...
Opponent battlefield (...): Silverquill Silencer {b}{w} (3/2) [counters: 1x ghostform (a marker
  only - the ability that returned it is gone)] ...
```

Mechanism (code read, not reproduced): Kaya's script grants the return trigger with
`transforms((,newability[@movedto(this|graveyard) from(battlefield) …],…)) forever`
(`bin/Res/sets/primitives/planeswalkers.txt:1874`) — the triggers live on the CREATURE as granted
abilities, not as action-layer trigger objects, so an action-layer scan for them finds nothing and
reports the grant dead. F5 fixed a real hazard (the wave-72 unconditional gloss) by adding a
predicate that is wrong in the common case; the wave-72 wording was at least true here. Two
surfaces, one counter, opposite facts — the exact class N5 was written to close on the loop
sentence, reopened one wave later on another.

### HIGH-2 — `main_phase_windows_skipped` counts 375 phases and 233 of them DID offer a casting decision
The per-tick over-count Astra found is genuinely fixed: every `(turn, phase)` pair appears exactly
once across all traces, so the meter is a phase meter. But `noteMainPhaseCastingOffered()` is wired
only to the standard sorcery-speed branch (`AIPlayerBaka.cpp:4159`), and the instant-speed response
arm that fires the skip note (`:3949`) can itself issue a Casting decision for that same phase.
Cross-referencing every counted `(turn, phase)` against the `Casting decision (Main phase N, YOUR
turn)` headers in the same seat's translog: **233 of 375** counted phases put a casting decision to
the model. The honest "this main phase reached the model with no casting window at all" figure is
**142**, and N16's proposed validation ("equals the number of own turns with no Main-1 record") is
still not usable — the direction of the error simply moved from per-tick to per-arm.

All 375 name the instant-speed response arm; the unresolved-stack arm fired **0** times, which
confirms N16's code-reading hypothesis about WHICH arm swallows the phase and refutes the other.

What it did NOT cost, measured: **0** of the 202 counted Main-1 phases lost a land drop (a land sat
in hand and no land-drop window occurred that turn: 0 cases — the land seam runs independently of
this branch), and **0** of the 375 had a non-instant hand card tagged `[castable now]` with no cast
row for it. So the render stays self-consistent and no lost sorcery-speed cast is demonstrable in
this corpus. Repro of the shape: `123v130` deck123 seat seq 6, turn 3 Main 1 — the instant-only menu
offers Devour Flesh alone while the hand holds a Plains and two Intruder Alarms (both correctly
tagged `[cannot pay now]`), and the land drop was served separately at seq 5.

### MED-1 — one turn produced 294 collapsed windows, a 411-row menu and a 104 KB prompt
`125v123`, deck123 seat, turn 67-68: an Intruder Alarm + Thraben Doomsayer untap loop ran a 77/77
repeat plan, then 294 `stop_reached` collapses, then two priority windows of **411 and 409 options**
at **104,131 B** and **102,927 B** (seqs 658 and 664; wave 72's largest prompt was 37,917 B). The
menu does collapse ranges (`2-103. Equip with Lightning Greaves #1 targeting Human #1-#102`), and
the seat passed both — 46.2 s each. 18 windows corpus-wide carry ≥60 options, 17 of them this seat.
The stop collapse is the mitigation, not the cause, and it worked; but a menu that enumerates 102
equip targets one per row is the underlying interface defect, and it is the same class as the
mis-ordered rung menu BZ N4 fixed — a row generator with no cardinality budget.

### MED-2 — the repeat row's short name is unwritable, and the protocol asks the model to write it
4 records carry `parse_note: repeat_count_missing`, all deck123, all the same shape: the reply
writes `CHOICE: 2 (Create human with Thraben Doomsayer, repeated N times, then stop)` — the row's
own printed short name, copied verbatim. The row's bracket says exactly this is refused
(`copying this row's name with the letter N still in it names no count and is refused and
re-asked`), while the answer sentence at the foot of every prompt says to copy the row's SHORT NAME
in parentheses. The two instructions point in opposite directions on this one row family. Repro:
`125v123` deck123 seat seqs 620, 628, 639 and `126v123` seq 74. Cost is one re-ask each; the fix
direction is a row name that contains no placeholder (report only — this is protocol/row surface,
invariant 000(c)).

### MED-3 — the descending X menu is the corpus's only index/name conflict class, two waves running
Both `echo_index_conflict` notes are X-value menus rendered descending (`1. X = 7 … 7. X = 1`), where
the model writes `CHOICE: 5 (X = 5)` and index 5 is `X = 3` (`125v126` deck125 seat seq 100;
`125v123` deck123 seat seq 624 is the repeat-row twin). Both were rescued by the name match and the
corpus took **0** fallbacks, so this is not costing decisions — but the render convention that
produced wave 72's HIGH-3 (a numeric menu whose index does not track its number) is still present
on the one menu family BZ N4 deliberately left alone.

### LOW-1 — `async_drops` 32 → 43 over 42 records
`answer_replaced` 0, 0 fallbacks and 0 `phase2_answer_missing` followed any drop, so the
drop-and-re-ask path is self-healing as designed. The sample event is
`casting/question and board/re-asked`. Worth a glance next wave, not an action.

### LOW-2 — `identical_ask_answers_reserved` 999 of 1,262 replays
The repeat-latch path is now 79% of all re-serves (wave 72: 63%), and it is the path CA N8 brought
under the cap. It never hit the cap (max run 50 of 64) and refused 0 times, so the bound is
correct and untested at its edge — the same "untested at the cap" state BT M22 was left in.

---

## 5. Lategame specimen

`wave73/lategame-specimen.txt` —
`1788902003-ai_baka_deck125-0x5622aac60850-vs-ai_baka_deck126.jsonl` **seq 435, turn 63**, phase
Upkeep, 126 vs 3 life, 5 options, 51.0 s, 5,828 reasoning chars, prompt 32,487 B. The seat
took `Deal 1 damage with Staff of Nin targeting the opponent`. Chosen because it is a late-turn
decision from one of the two deck-125 stall games the brief flags AND it carries six wave-73 clause
families on one screen: the always-printed `0. Pass priority` row, the re-worded HOLD row with its
cost leading, the `[LOOP RUNNING` bracket, the `LOOP COMPLETE` header with the shared entry
sentence, the de-capsed `THE PLAN YOU LAST STATED` heading, and `[cost: ]` tags on activation rows.
Full `prompt` verbatim under a 3-line header. Alert-only per invariant 0: **the composed prompt
SHAPE changed this wave** — a row 0 now precedes every priority menu, the hold row leads with its
cost and names the untap release, the plan heading lost its caps phrase, and four clause families
are new on screen (`{death price: }`, `[cost: …(mana value N)]` on reveal rows, the ghostform
marker gloss, and `held back, THIS creature could not block`).
