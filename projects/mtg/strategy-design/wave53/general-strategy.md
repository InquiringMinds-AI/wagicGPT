# Wave-53 general strategy — RENDER / CORE-PROMPT proposals, layer-routed (R1-R160 revised, R161-R192 new)

Corpus `matchups-20260902-023342` (TENTH fair-hand corpus), binary master **c628dd531** (lanes
M 614312289 / N 3ab5c27c4 / O 0ba5ae2eb / P 1057c7c9d / Q ba33a9adb / R 1bb08b2b1 / S 6a4d5e119 /
T 00549b391; PARSETEST 2249/0, gate 1215 + 46 AI/0). Pilot **qwen36-35b-a3b** (FP8) on Spark :8084.
42 seat logs, **3,572 records**, **3,483 decisions** (ask 2,754 / priority 519 / attackers 120 /
discard 34 / blockers 30 / reveal 21 / bottom 5), plus 42 `gamestart`, **40 `gameend`** and **7
`recovery`** (the new kind, which is NOT a decision). **20/21 games natural — `152v125` HUNG for
~13 h and was cap-adjudicated**; under invariant 00 that matchup is a FAILED test, it is excluded
from every rate below, and lane X (merged to master after the review opened) is its fix.
**15 `fallback` fields = 0.43%** (`plan_choice_conflict` 8, `unparsed_reply` 3, `named_row_reask` 3,
`retracted_choice` 1); **Baka-executed 4 = 0.11%**. Longest seat **607** records (`146v125`, 84
turns); max prompt **32,068** (`146v123` seq 92, priority, turn 33) with **12 prompts over 30,000**.
Total inference **20.96 h**. Sources: the four seat files, seven `deckN/general-suggestions.md`,
seven `deckN/skill.md`, the eight lane reports plus lanes U/V/W/X, and the corpus itself — every
load-bearing number below was re-derived on disk by this step with python over the JSONL and
`/usr/bin/grep` over the `.stderr` files.

**Routing rule (owner doctrine 2026-08-26).** The core loop — engine mechanics, LLM UX, bugs — is
the purpose. Everything below is a change to what the ENGINE RENDERS, what the PARSER executes, or
the CORE PROMPT's own text. Guide-side answers are named only where a rule must stay correct AFTER
the render ships. Engine BUGS with call sites are in `engine-ledger.md` (the wave-54 docket); this
file is the ranked routing view with the evidence and the wave-54 falsifier for each item.

**Governing constraint on every item.** Legality is enforced structurally; the model's CHOICE is
never constrained. Every proposal ADDS a true token, a true row, a receipt, a stamp or one re-ask,
and DELETES nothing. Hard caps on legal choices are rejected on sight. **Three proposals this wave
would REMOVE a legal window and are rejected in that form** — withholding or ceasing to re-offer a
cast row the engine has marked dead (deck146 R-2, 613 renders / 2 takes), skipping an opponent-turn
window whose stack is empty at a phase with no cast history (deck125 G1), and a fourth restatement
of the identical-menu cache (implicit in several seats). The evidence for each rejection is under
R162; the adopted forms are the HOLD row's three repairs (R162) and one additive stamp (R174).
One proposal — bounding the PLAN capture — is adopted in a SHAPE form and explicitly recorded as
**not a cap on a choice**: `consumePlan` already caps the block at 1,600 bytes, and a prose bound is
a bound on the pilot's own quoted text, never on a menu.

---

## STATUS OF R1-R104, by the emitter's actual string

Verdicts unchanged from wave 52 except where a wave-53 lane touched them; the touched ones are
re-stated here, the rest CARRY as recorded in `wave52/general-strategy.md`.

| item | wave-53 verdict | evidence (re-derived on disk) |
|---|---|---|
| **R1** loop affordance | **HOLDS**: `stale_livelock` **0** in 3,483 decisions; `repeat plan iteration` 60 with `cast ask NOT issued` 257 | `[repeat:` rows **0** at deck146's Hive (second corpus); deck123's chains 12 offers / 4 takes, `x22`/`x23` executed |
| **R2** A-row collapse | **STILL FAILS on ability menus, and the class moved card** | `125v126` seq 220 = **28** rows of `Deal 1 damage with Staff of Nin #N targeting …` (seq 189/213 at 26); `130v125` s181-s186 = twelve forced-sacrifice menus of 18 -> 13 rows, fourteen byte-identical Mountains — **R168** |
| **R13/R71 -> R104 -> R132** GAME LOG share | the cap is working HARDER, and the tail grew anyway | **476** prompts carry `(…earlier events trimmed…)` (wave 52: 165); log segment max 24,107, p50 9,107; 12 prompts > 30,000 — **R192** |
| **R19/R38 -> R108** what paying taps | **PASS, and lane O split the clause correctly** | `{paying this taps:` on a **defender**: old string `cannot attack this turn` **0**, new string `cannot block on their turn` **27** (all Overgrown Battlement, `mtg.txt:83596` `abilities=defender`), 8 takes. Non-defender rows keep the attack clause **14/14 correctly** (Katilda 10, Luminarch Aspirant 4). Forecast source count == receipt source count **27/27** — wave-52's residual **CLOSED** |
| **R32/R64 -> R73** auto-tap order | **HOLDS**; `- Paid` receipts **496**, **0 with no source tapped** | 11 receipts lack a ` with ` clause and all 11 are the legitimate `from mana already floating` form |
| **R67 -> R79 -> R114 -> R146** second copy | **SHIPPED and PASSED — the clearest render-to-behaviour result of the wave** | 86 `[second copy:` rows / 12 takes, **all on genuinely stacking scripts**; the one Intruder Alarm row carries the dead tail and was NOT taken; Chromatic Lantern's partial form rendered **6** and was declined **6/6** — against **5 of 5 taken** last corpus under the bare string, **with no guide change** |
| **R70** counter-target size | CLOSED (wave 52); no new window | |

**R72-R104.** All those recorded SHIPPED-and-PASSED held by string: R72/R139 (Soul Shatter — the
edict re-resolution failure did NOT recur; `146v125` T50 resolved to `Opponent's Emrakul, the Aeons
Torn died`, T63's second copy was countered), R73, R74, R75, R76, R80-R101 with no failing window.
R103/R133 latency: max **868,729 ms = 96.5%** of the wall and it **answered** — the 900 s wall was
not reached once (**R169**). R84/R123 edict victim tags carried: no failing window.

## STATUS OF R105-R160 — the wave-52 items and the wave-53 lanes, by the emitter's actual string

| item | lane / docket | wave-53 verdict | evidence |
|---|---|---|---|
| **R105-R121** (wave-52 lanes G-L) | | **all HOLD** | `stale_livelock` 0; `at 0 this does nothing` **613 rows / 2 takes**; PUT parser 0 breaks; `{spends K of M}` and `{leaves K of M}` intact; `becomes` re-offer 0; animated-land tap 0; `{this hits YOUR permanent}` **33 rows / 0 takes** |
| **R136** silent zero-block after `multiblock_reask` | **M 614312289 / D1** | **SHIPPED — PASS 0/30** | `multiblock_reask` does not appear as a fallback class at all; all 30 `blockers` records carry a `chosen_text`; 30 asks <-> 30 stderr `declared blocks from N assignment(s)` lines, 1:1. `multiblock_first_wins` **first live firing**, `152v130` seq 16: `BLOCKS: B1:A1, B1:A2` -> `dropped_assignments: 1`, note stamped, `Sigarda blocks Siege-Gang Commander` executed. `blocker_handle_in_attacker_slot` **UNTESTED** (no `Bn:Bm` reply arrived). `blockers` prompts are **34% smaller** (mean 9,541 vs 14,350) |
| **R137** the opponent-turn window economy | **N 3ab5c27c4 / D2** | **SHIPPED — the mechanism works and the LEVER IS NOT BEING PULLED; every headline prediction FAILED** | windows **1,052** on the engine seat's key (`Casting decision (<phase>, opponent's turn)`, the
wave-52 comparable) or **1,126** on a looser substring match, or **1,243** counting every decision on
an opponent turn — three true denominators (skill #208); against wave 52's 884 that is **+19%** on
the like-for-like key. **58 casts** on all three (was 68, **-15%**; 5.5% / 5.2% / 4.7%); runs of 3+ byte-identical declined menus **912 / 3,483 = 26.2%**, 147.4 min (target < 10%, baseline 23.0%); HOLD row **1,173 renders, LAST row 1,173/1,173, opponent's turn only, 29 takes (2.5%)**; `hold_windows_skipped` **102, non-zero on 2 of 40 gameends**. Seven phases produced **0 casts in 445 windows / 3,939 s**. The falsifier did NOT fire in its stated form (the row IS taken) — **R162** |
| **R138** the `[DRAW PRICE:]` lethal tail | **O 0ba5ae2eb / D3** | **SHIPPED — PASS on the form it reaches, and it reaches ONE of two forms** | **85 DRAW PRICE row-instances / 71 records**; the tail `- you would be at K` renders on **25**, `; this KILLS you` on **15 row-instances / 8 records**, all at `125v162`; **0 casts from a KILLS row** (was the loss). The other **60** rows are the cost-phrasing form (`…so taking it costs you N life right now`, 39 cast + 21 cycling) and carry no total at all — **R170** |
| **R139** the edict forecast vs a pending stack object | **P 1057c7c9d / D4** | **SHIPPED — UNTESTED (0 renders)** | ` - a sacrifice is already on the stack aimed at this permanent` renders **0**; no second edict aimed at a permanent already under one arose. Old-string renders 0, new 0, takes 0. The wave-52 triple did NOT recur (R72 above) |
| **R140** the cast row's kill summary | **O / D5** | **SHIPPED — PASS on emission, FAIL on the denominator** | **129** `{kills N of the M legal targets …}` + **40** `{kills: <name>}` rows (was 0/85); 4 count-row takes, 3 named-row takes. But **26 rows list a PLAYER among their legal targets and exclude it from the count**, and one of them was lethal — **R164**. 244 of 413 creature-targeting removal rows (Path 224, Fateful Absence 15, Vanishing Verse 5) carry no verdict at all because "kills at N damage" is inapplicable to unconditional removal — **R171** |
| **R141** the defender tap consequence | **O / D6** | **SHIPPED — PASS** | see R19/R38 above: 0 defender rows on the old string, 27 on the new, 8 takes |
| **R142** ability-menu collapse | **P / D7** | **SHIPPED — FAIL** | 3 menus of > 12 rows collapse to <= 2 shapes masking `#N` (`123v126` seq 30 = 26 rows / 2; `130v125` seq 69 = **17 rows / 1**; `162v125` seq 26 = 14 / 2); 5 masking every digit; **16** asks whose de-duplicated list is length **1** (147 rows, 2.9 min) — three denominators, all true — **R168** |
| **R143** the from-exile cause | **P / D8** | **SHIPPED — PASS 2/2, sample too small to re-test** | only **2** `{castable from exile}` rows exist this corpus and both name their cause (`- your card, exiled by their Elite Spellbinder, which lets you cast it from there`); the causeless fallback renders **0**. Both taken |
| **R144** the `(Pass)` echo | **N / D9** | **SHIPPED — PASS on behaviour 284/284, and the NOTE never renders** | 284 replies wrote `CHOICE: 0 (Pass)`/`(pass)`/`(Pass priority)`; **284/284 executed row 0**; zero in-range non-zero index carried a pass parenthetical. `pass_row_named` renders **0** times — the behaviour is right, the tag is missing (**R191**) |
| **R145** timeout observability | **Q ba33a9adb / D10** | **SHIPPED — UNTESTED, N = 0** | **0 empty replies**, **0 records carry `retry`**, `fallback: "timeout"` renders 0, `no reply after` in stderr 0. Max latency **868,729 ms = 96.5%** of the wall — and it ANSWERED. Not a PASS: the arm never had a window. Wave-52's "empty_reply within 1 s of the wall" 3 -> **0**, and Baka-executed opening keeps 2 -> **0** |
| **R146** the `[second copy:` usefulness verdict | **P / D11** | **SHIPPED — PASS, and it moved behaviour with no guide change** | see R67 above. New residual: the BARE form now reads as "safe" by contrast — **R189** |
| **R147** plan age + the latched coded line | **N / D12** | **SHIPPED — PASS 3,329/3,329 on the stamp; `latched_coded_line` UNTESTED** | two forms only (`, 1 window ago on turn N` 2,375; `, N windows ago on turn N` 954); max `plan_echo_count` **43** (was 51). All **8** `plan_choice_conflict` records carry `coded_answers: 1`, so the predicted case never arose; the field itself works — **39** records carry it, 39 of the 43 with `coded_answers >= 2` |
| **R148** the converter folded into its rows | **O / D13** | **SHIPPED — UNTESTED (0 renders)** | ` - and their <name> takes N off YOU: life A -> B` renders **0**; no edict/gain row coincided with a converter on the board |
| **R149** a land count in the battlefield header | **P / D14** | **SHIPPED — PASS 3,429/3,429** | e.g. `… of which 0 are creatures, 0 of them able to attack right now, and 25 are lands` |
| **R150** the converter's branch | deck152 | **CARRIED** — the guide-side fix (#202/#204) ships first | |
| **R151** the draw-punisher tag | D16 | **CLOSED in effect by behaviour** | `130v162` s37-s39 took Starstorm X=4 naming Fate Unraveler the window it became affordable; 34 punisher windows, **0 draws/cycles taken** |
| **R152** discard-list duplicate markers | D17 | **CLOSED** | deck130's one discard ask printed `copy 1 of 2` and the duplicate was sent; deck123's four asks all printed markers |
| **R153** the `{feeds:}` K-0 clause | D18 | **CARRIED**, and the mirror lane opened: 11 first-Mine over-brake windows (was 1) — guide-side (#206) | |
| **R154** `Bn:Bm` dropped silently | M / D19 | **SHIPPED — UNTESTED** | no `Bn:Bm` assignment arrived in 30 blockers replies |
| **R155** the `(xN)` collapse | Q / D20 | **DECLINED ON MEASUREMENT, and the miss GREW** | duplicate event lines inside a single record **571** (wave 52: 280): `- Phase: Draw` 237, `- Opponent drew a card` 116, `- Phase: Main phase 1` 71 — **R186** |
| **R156** receipts without a payee | D21 | **CARRIED — narrower: 2** (7 -> 3 -> 2) | both the Intrepid Adversary pay-any-number-of-times ETB (`152v126` seq 25, `152v162` seq 29) — **R187** |
| **R157** the face row's life | D22 | **CARRIED, unchanged** | `130v125` s120/s158, `130v162` s35 — **R188** |
| **R158** the BLOCKS format line | M / D23 | **SHIPPED — PASS 30/30** | `…each B-number at most ONCE, and several B-numbers may share one A-number - or exactly "BLOCKS: none"…` |
| **R159** the `unparsed_reply` recovery record | Q / D24 | **SHIPPED — PASS 7/7** | exactly 7 `recovery` records and exactly 7 decision records with `choice: -1` AND a fallback; every one paired by `recovers_seq`; the 8 `plan_choice_conflict` records (choice != -1) correctly get none. **Gap:** the hung game's `reveal` at `152v125` seq 28 is a `choice: -1` with NO fallback field, so it gets no recovery record and is invisible to this lane — **R167** |
| **R160** the log window | D27 | **RE-MEASURED, still the owner's** | 476 trims (was 165), 12 prompts > 30,000, every one ~75-80% log — **R192** |

**Reconciliation note on R118 / `decision_reversed_in_prose` — the wave-52 rejection STANDS, and the
cost this wave is a different thing.** Wave 52 rejected narrowing the check because 3 of its 5
firings carried `coded_answers: 2` + `answer_replaced` and had changed the executed action. This
corpus has **8 firings and every one carries `coded_answers: 1`** — the wave-52 grounds for the
check's value did not recur, and neither did the grounds for narrowing it. **5 of the 8 are a NEW
collision the check did not exist to catch**: the coded line names the HOLD row and the PLAN
sentence then says *"I will pass priority"* (`146v125` s150, s371, s392, s398, s470, s491, s555),
which the detector reads as a reversal. Three of those re-asks flipped to a plain pass and three
re-confirmed the hold, at a full extra model call each. So the adopted fix is **not** the wave-52
narrowing — it is to **exempt the HOLD row from the pass-verdict test**, because a pass verdict
CONFIRMS a hold rather than contradicting it (**R162b**), which removes 5 of 8 firings while
narrowing nothing. The residual 3 are two real reversals and one false positive (`123v126` s22, a
single clean coded line with no second row named, re-asked and answered `pass`, losing the Transform
the pilot wanted). And the opposite failure is separately real and separately ADDITIVE: at
`123v125` s84 a reply reasoned in words to the opposite verdict, emitted no second coded line, and
cast a dead Damnation unflagged; at `126v125` seq 73/74 two replies wrote `CHOICE: 1 (Cast Tribute
to Hunger)` above a PLAN line arguing against exactly that, on a row the engine itself had labelled
`at 0 this does nothing`. That is a STAMP, not a suppression — **R174**.

---

## HIGH

### R161 — the interactive-reveal driver parks forever when option one has zero legal targets: one game, 13 hours, invariant 00 failed. (= ledger **D1**; engine seat **HIGH #1**, seat-125-126 **HIGH #1**, seat-146 **E-4**; lane X shipped the fix)
`1788334437-ai_baka_deck152-0x5583d9910230-vs-ai_baka_deck125.jsonl` seq **26-28** and
`game-152v125-1788334434.stderr:1097-1214`. Turn 15: deck152 declares four attackers; Ranger Class's
level-2 trigger resolves normally (seq 27, 4 rows, answered Sigarda); Sigarda's Coven `reveal:5`
opens on Branchloft Pathway x2, Barkchannel Pathway, Fateful Absence, Ranger Class — **no Human**.
Seat log seq **28**: `kind: "reveal"`, `options: 5`, `choice: -1`, `chosen_text: "none (no legal
target)"`, `latency_ms: -1`, **`prompt` 0** — the engine answered for the seat with no model call.
Then nothing for ~13 h; **neither seat carries a `gameend`**; the harness adjudicated 20/9 at turn 15.
**Mechanism (lane X, pinned and fixed).** `MTGRevealingCards::toResolve()` picks the branch UP FRONT:
option one's `<upto:1>human|reveal` has `countValidTargets() == 0`, so `abilityFirst` is never
constructed and **option TWO** is armed instead. `driveInteractiveReveal()` phase 0 did not know
that, asked the seat, then issued its finalize `CheckUserInput(JGE_BTN_NEXT)` against a chooser that
belonged to option two; `CheckUserInput`'s decline branch is gated on `!abilitySecond`, so the press
could not take it and option two's window was spent on zero targets. Phase 3 then reached
`if (!tc) return;` and waited forever for a chooser that had already come and gone. The reveal
display stays open and an open display holds every phase-advance path, including
`userRequestNextGamePhase`. `REVEAL_DBG` is `WAGIC_REVEAL_DEBUG`-gated, which is why 13 hours produced
**zero stderr**.
**Not card-specific and not rare.** Any reveal whose option one is predicate-gated and whose fail
branch is itself targeted is exposed. The corpus holds **5** Sigarda Coven reveals; four found a
Human and completed. **1 in 5 Coven triggers, 1 in 21 reveal windows.**
**Status.** Lane X shipped both halves (phase 0 hands straight to phase 3 when
`!abilityFirst && abilitySecond`; phase 3 rebuilds option two once, latched, with an ungated stderr
line), with a GPT-free fixture RED on base (`phase problem. Expected [Cleanup](12), got [End](11)`,
five cards stranded) and GREEN after. Gate 1216/2-known + 46 AI/0, PARSETEST 2249/0.
**Wave-54 falsifier.** A reveal whose option one has zero eligible cards resolves to option two and
the game continues: **N/N**; **no seat log ends without a `gameend`**; 21/21 natural; every `reveal`
record with `latency_ms: -1` and `prompt: 0` is followed by a further record from the same seat
inside the same turn: **N/N**.

### R162 — the HOLD row is the right mechanism and three separate defects hold it at 2.5%: an index-0 echo that silently degrades it to a plain pass, a plan-conflict collision that eats 5 of the corpus's 15 fallbacks, and a hold key that the opponent's own draw step retires on every opponent turn. (= ledger **D2**; engine seat **D-1 HIGH**, seat-125-126 **HIGH #2**, seat-146 **E-1 HIGH**, seat-123-130 **M1**, deck123 G5, deck146 R-3, deck162 R-10)
Lane N shipped exactly what wave 52 adopted and the render is correct: **1,173 renders, LAST row
1,173/1,173, opponent's turn only, 29 takes, `hold_windows_skipped` 102**. Every headline prediction
failed anyway — windows **up 19%** (884 -> 1,052), casts **down 15%** (68 -> 58), identical-declined
runs **26.2%** against a target of < 10%. The row is not the problem; three things around it are.

**(a) Index-0 degradation, 2 of 2 firings wrong.** `125v146` seq **74** (a 2-row menu) and
`146v125` seq **372** both reply `CHOICE: 0 (Hold priority)`. The engine takes index 0 = pass, the
name is ignored, **no note, no re-ask** — the hold never registers. This is wave-52's D9 inverted:
lane N's own D9 fix made `0` and `pass` a RESERVED echo that binds ahead of every index and name
verdict, so on a menu whose HOLD row is not row 0 the name can never win. **Ask:** reserve the HOLD
row's short name the way `pass` is now reserved, and where the reserved pass echo and a reserved
HOLD name conflict, take the NAMED row or re-ask — never silently execute the third thing.

**(b) The D14 collision, 5 of 8 `plan_choice_conflict` firings.** Taking the HOLD row means "I do
nothing this turn", so the model's PLAN prose says *"I will pass priority"*; the detector reads a
non-zero index plus a pass verdict and re-asks. `146v125` s150/s371/s392/s398/s470/s491/s555. Two
re-asks then degraded to a plain pass and three hit `plan_choice_conflict_exhausted`. Each costs a
full extra model call. **Ask:** exempt the HOLD row from the pass-verdict conflict test — a pass
verdict CONFIRMS it. This narrows nothing (see the R118 note above).

**(c) The hold key contains hidden-zone counters, so the opponent's draw step retires it — by
construction, on every opponent turn.** `holdBoardKeyOf` (`src/AIPlayerGPT.cpp:12904`) is
`serializeGameState()` minus its leading phase line only, and `serializeGameState` ends with
`yourLibraryLine(game->library->nb_cards, …)` (line 12314) and carries
`opponentZoneCountsLine(opp->game->hand->nb_cards, …)` (line 12239) —
`Opponent hand size: N | Opponent library: M cards` and `Your library: N cards` are **in the key**.
`holdStillStands` (12917) treats any key change as a re-opener. Repro: `125v126` seq **128** (turn
30, opponent's Upkeep, HOLD taken) -> seq **130** (turn 30, opponent's **Draw**, the byte-identical
two-row menu, asked again), with `game-125v126-1788334422.stderr` logging exactly one
`hold re-opened at the priority seam - the board changed`; twelve more windows followed in that turn.
That is why `hold_windows_skipped` is **0 on 38 of 40 gameends** while the row was taken at six
different seats. **Ask:** exclude hidden-zone counters (opponent hand size, both library counts)
from `holdBoardKeyOf` — a card moving into a hidden zone is not a board change the hold was about.
This is a KEY change, not a window removal: every re-opener the lane designed (life, poison, both
battlefields, both hands' CONTENTS where visible, the stack, a newly affordable row, the turn
ending) is untouched.

**(d) Adoption, once (a)-(c) are fixed.** In **703** windows the prompt already carried
`[you declined this exact list N times already this turn]` with N >= 3 (N reaches 18) and the pilot
took HOLD in **3** of them (0.4%). Two cheap wording asks, both additive: lead the row with the verb
the model already uses (*"Pass priority, and do not ask me again this turn unless the board
changes"*, deck146 R-3 — the pilot does not read "hold priority" and "pass priority" as different
acts), and put the guarantee ON the row (*"(any change re-opens this window; you give up no cast)"*,
deck123 G5), because the row today states what it does and never that it is free.

**Three window-removing proposals are REJECTED in that form.** (i) deck146 R-2, withholding or
ceasing to re-offer a cast row the engine has marked dead (`at 0 this does nothing` **613 renders,
2 takes**): it removes a legal action from the menu — the ceiling is the model's, not the engine's —
and wave 52 already showed such a clause can be stale against a pending stack object. (ii) deck125
G1, skipping an opponent-turn window whose stack is empty at a phase with no cast history: a phase's
cast history is not a legality fact, and this corpus supplies its own counter-example —
`125v152` seq **15**, a real `Cast Path to Exile` at the **Attackers** step, one of only two casts
outside Main 1. (iii) any blind identical-menu cache: wave 52 measured **30 real actions** following
a decline on a byte-identical set in the same turn and that finding is not re-opened.
**Wave-54 falsifier.** `CHOICE: 0 (Hold priority)` (or any HOLD-named echo) executes the HOLD row or
is re-asked: **N/N** (was 0/2). HOLD-row takes that also trip `plan_choice_conflict`: **0** (was 5).
`hold_windows_skipped` non-zero on a `gameend` where the row was taken at least once: **N/N** (was
2 of 6 such seats). HOLD takes in windows already carrying `[you declined this exact list N times…]`
with N >= 3: **>= 20%** (was 3/703 = 0.4%). Identical-declined runs of 3+: **< 15%** of decisions
(was 26.2%).

### R163 — a COMPENSATION search is framed to its beneficiary as a loss it must minimise: Path to Exile's basic-land fetch renders as "EXILE ONE OF YOUR OWN CARDS … each option is a card YOU will LOSE", 8/8. (= ledger **D3**; engine seat **D-2 HIGH**)
Repro `152v125` seq **12** (turn 9) and seq **19** (turn 12); also `146v125` 11/37/64/171,
`130v125` 69, `162v125` 26. Emitted header:
`EXILE ONE OF YOUR OWN CARDS: your own effect (Path to Exile) makes you exile a card from the list
below - each option is a card YOU will LOSE, NOT a target you attack or affect. Pick the card you
can best AFFORD TO LOSE (usually your LEAST valuable: a spare land or a redundant/dead card; KEEP
your best spells, answers, and threats).` Rows: `1. Forest [land] [your library] - "G" / 2. Plains
[land] [your library] - "W" / …`. The primitive (`mtg.txt:84676`) is
`auto=moveto(exile) && ability$!name(search land) notaTarget(land[basic]|mylibrary)
moveTo(mybattlefield) and!(tap(noevent))!!$ targetcontroller` — **the chosen card is put onto the
battlefield tapped. It is a gain.**
**Mechanism, pinned this step by reading the source (read-only).** `handRemovalVerb`
(`AIPlayerGPT.cpp:20084`) matches the lowercased *chooser-source-name plus ability-verb* string:
`"path to exile"` contains `exile`, so it returns the verb `"exile"` before anything looks at where
the card is going. `buildHandRemovalAsk` (20105) then picks the SELF branch, correctly — the search
ability is granted to `targetcontroller`, so `tc->source->controller() == this` — and prints the
loss wording for a gain.
**Consequence beyond the wording.** It inverts the value ordering: the header tells the seat to pick
the land it least wants, when the correct pick is the land it most NEEDS. It is also why two of the
three >12-row / <=2-shape mega-menus exist (17 library Mountains at `130v125` seq 69, 14 library
Swamps at `162v125` seq 26).
**Ask.** Gate the removal framing on the ability's DESTINATION ZONE, not on the source name: a
`moveTo(mybattlefield)` or `moveTo(hand)` destination is a GAIN ask ("CHOOSE A CARD TO PUT ONTO THE
BATTLEFIELD: … pick the land you most need"), and only `exile`/`graveyard`/`library` destinations
keep the loss framing. PARSETEST is the natural pin: the verb table already exists.
**Wave-54 falsifier.** Asks whose candidate zone is `mylibrary` and whose destination is the
battlefield or the hand, framed as a loss: **0** (was 8/8).

### R164 — the cast row's kill summary counts creatures only and hides a PLAYER target: 26 rows, one of them lethal, and it lost the game. (= ledger **D4**; seat-123-130 **H2/G6 HIGH #1**, engine seat **D-4**; skill #189)
Repro `130v162` seq **62** (turn 22, `Opponent life: 1`, ten untapped sources):
`Cast Hammer of Bogardan {1}{r}{r} {leaves 7 of your 10 untapped mana sources untapped}
{kills 0 of the 2 legal targets at 3 damage} - legal targets right now: Shield Sphere (artifact
creature 0/6) [defender] …, Fate Unraveler (3/4) …`. The row WAS taken; the target ask at seq **63**
then opened `1. The opponent (player, life 1)` / `2. Shield Sphere … SURVIVES` /
`3. Ob Nixilis, the Hate-Twisted [planeswalker]` / `4. Fate Unraveler … SURVIVES` / `5. Yourself`,
and the pilot answered **3**. Hammer of Bogardan is `target=anytarget auto=damage:3`
(`mtg.txt:52046`); 3 to a player at 1 life wins. The seat then declined a lethal Pyrite Spellbomb at
seq 65 and died 0 to 1.
**This is by design, and the design note is lane O's own:** "the D5 cast-row summary counts only
creature targets currently on their controller's battlefield; player targets and non-creature
permanents are excluded from the denominator." Re-derived corpus-wide, the class splits in two and
BOTH halves are live. (a) **26 `{kills …}` rows list `the opponent` among their enumerated legal
targets and exclude it from the count** — `130v125` seq 97-127 is nineteen of them, including seq
125 at opponent life **14** where the seat answered `Cast nothing right now` and seq 127 at life
**8** where it took the same row. (b) At `130v162` seq 62 the cast row's OWN enumeration omits the
player altogether (`- legal targets right now: Shield Sphere …, Fate Unraveler …`) even though
Hammer of Bogardan is `target=anytarget` — and the target ask one record later opens
`0. The opponent (player, life 1)`. **And that target ask prints `{right now: takes 3 damage -
SURVIVES (toughness 6)}` on the Shield Sphere and `- SURVIVES (toughness 4)` on the Fate Unraveler
and NOTHING on the player row that wins the game.** Two of the four `{kills 0 …}` takes in the
corpus are rows of class (a) — the model was right and the summary was misleading.
**Ask, three parts, all additive.** (i) Enumerate player targets on the cast row of an `anytarget`
spell, as the target ask already does. (ii) Count them separately and carry lane O's own lethal tail
across: `{kills 0 of the 2 creature targets at 3 damage - and 3 to the opponent at life 1 WINS THE
GAME}`; failing the lethal half, at minimum print the denominator honestly (`of the 2 CREATURE
targets`). (iii) Give the PLAYER row in the target ask the same `{right now: …}` verdict its
creature siblings get (`takes 3 damage - LETHAL (life 1)`), which is R188 from the other side. The
arithmetic is lane K's and lane O's, already written twice.
**Wave-54 falsifier.** `{kills …}` rows whose enumerated targets include a player and whose summary
does not say so: **0** (was 26). Cast rows for an `anytarget` spell whose enumeration omits the
players: **0** (was 1, and it was the game). Player rows in a target ask carrying no damage verdict
while a creature sibling carries one: **0**.

### R165 — a modal cast row prints no per-mode live/dead annotation, so a "Choose two" card is priced from eighty turns of narration. (= ledger **D5**; seat-146 **E-2 HIGH**, deck146 R-1)
Repro `146v125` seq **240** (T47) and seq **528** (T79, 20 untapped sources), with ~18 siblings
across turns 29-83. Every `Cast Silverquill Command {2}{b}{w}` row in the corpus (**28** of them)
prints exactly two annotations — `{leaves N of your M untapped mana sources untapped}` and
`{card text: "Choose two — … Return target creature card with mana value 2 or less from your
graveyard to the battlefield. …"}` — and **0** of the 28 carry any mode annotation
(`{modes live` renders 0 times corpus-wide). The pilot must reconstruct its own graveyard from the
log, and at s240 it asserted *"my graveyard has no creature cards with mana value 2 or less"* with
`- Your Triumphant Adventurer was COUNTERED …` and `- Your Silverquill Silencer was COUNTERED …` in
the log directly above. Verified: `Triumphant Adventurer mana={B}{W}`,
`Silverquill Silencer mana={B}{W}`, `Silverquill Command auto=choice name(Return creature and you
draw) target(creature[manacost<=2]|mygraveyard) moveto(mybattlefield) && draw:1 controller && …`.
**A second modal surface has the same hole.** Nine records render
`Choose one mode for Silverquill Silencer:` followed by ten to fourteen rows of **bare card names
only** (`1. Essence Scatter`, `2. Path to Exile`, …) — no mana, no board effect, no card text, no
"does nothing right now" (`146v125` s10/s81/s506; `126v146` s6/s10; `130v146` s6; `146v162` s19/s23;
`146v123` s66). **0 of 9 carry any annotation at all.**
**Ask.** Extend to modal spells exactly the treatment targeted and edict rows already get
(`{kills: <name>}` 40 renders, `{right now: they sacrifice <name>}` inside 1,586 `{right now:`
renders): one clause naming which printed modes have a legal object right now and which do not —
`{modes live right now: return (Triumphant Adventurer, Silverquill Silencer); draw. Dead: pump (you
control 0 creatures), sacrifice (they control 0 creatures)}`. This is information the engine must
already compute to build the mode sub-menu, and it generalises to every Choose-N spell in the pool.
**Wave-54 falsifier.** `Cast <modal spell>` rows carrying a per-mode live/dead clause: **N/N** (was
0/28). Replies asserting an empty mana-value-2 graveyard against a log that names one: **0** (was 2).

---

## MEDIUM

### R166 — an OPPONENT's annihilator makes the seat sacrifice, and the ask says `your own effect`. (= ledger **D6**; seat-125-126 **MED #3**)
Repro `126v125` seq **111** (turn 53): `SACRIFICE ONE OF YOUR OWN CARDS: your own effect (Emrakul,
the Aeons Torn) makes you sacrifice a card from the list below … Choose card 1 of exactly 6`.
Emrakul is on the **opponent's** battlefield. `buildHandRemovalAsk` already has the right branch —
`byOpponent` prints `FORCED SACRIFICE OF YOUR OWN CARD: the opponent's effect (<name>) forces YOU
to …` — and the discriminator that missed it is `selfInflicted = tc->source && tc->source->controller()
== this` (`AIPlayerGPT.cpp:20470`). An ability GRANTED to a player parses onto a **nameless dummy
card** controlled by the victim, so that test reads TRUE for the opponent's own annihilator.
**The fix is already in the tree, one lane over.** Lane P's D4 pinned exactly this indirection for
the stack line: the real granter is `MTGCardInstance::storedSourceCard` (set at
`include/AllAbilities.h:4470`, `myDummy->storedSourceCard = source;`, the same route
`MTGAbility.cpp` uses for token creation and mana inside that keyword). Resolve `selfInflicted`
through `storedSourceCard` when the chooser's source is nameless. Cost of the miss this time: the
reply picked row **23**, its lone 1/1 lifelink Vampire, out of a 23-row list holding 15 lands; at
seq 112, same framing, it reasoned correctly and began sacrificing Sunpetal Groves.
**Wave-54 falsifier.** Sacrifice/edict asks whose effect is sourced from a permanent the seat does
not control, framed as `your own effect`: **0** (was 1 of 1).

### R167 — the reveal driver has no bounded wait and no ungated trace, and the harness has no no-progress watchdog. (= ledger **D7**; engine seat **D-3**)
Generalisation of R161. The whole interactive-reveal driver can park in phase 0 or phase 3 with
`mAIDriveDone == false` and its only instrumentation is `REVEAL_DBG`, gated on
`WAGIC_REVEAL_DEBUG` — in a 21-game unattended corpus that turned a 13-hour hang into a **silent**
one. Two additional gaps the records show. (a) `152v125` seq 28 is a `choice: -1` with **no
`fallback` field**, so lane Q's new `recovery` contract (7/7 elsewhere) does not see it: a `-1` with
no fallback tag is currently invisible. (b) **The feasibility watchdog measures LATENCY, and a hang
produces no latency at all** — the process sat 13 h writing nothing and the watchdog never fired.
**Ask.** A tick budget per reveal phase that force-closes the display and writes a record on expiry
(lane X's phase-3 rebuild is the first half of this and carries its own stderr line); a fallback
class on any engine-answered `-1`; and a harness-side **no-progress** watchdog keyed on wall-clock
since the last translog write per seat, which is cheap and would have cut 13 h to minutes.
**Wave-54 falsifier.** Seat logs with no `gameend`: **0**. Records with `choice: -1` and no
`fallback` field: **0** (was 1). Any game whose two seats write nothing for > 30 min while the
harness still runs: **0**.

### R168 — the `#a-#b` collapse still does not reach ability, sacrifice or reveal menus, and a menu whose de-duplicated option list is length 1 is not a decision. (= ledger **D8**; engine seat **D-5**, seat-123-130 **M5/L2**, deck152 **R-6**, deck130 G8; R142's residual)
Three shapes, three denominators, all re-derived:
* **> 12 rows collapsing to <= 2 shapes**, three masks and three true answers (skill #208): **2**
  masking `#N` AND every digit (`130v125` seq **69** = **17 rows / 1**, seventeen library
  `Mountain [land] [your library] - "R"`; `162v125` seq 26 = 14 / 2); **3** on lane P's own mask
  (adding `123v126` seq 30, 26 rows / 2); **5** on a looser digit mask. 36 records carry > 12 rows.
  Two of the three are the R163 mis-framed Path search.
* **Ability menus:** `125v126` seq **220** = **28 rows** of `Deal 1 damage with Staff of Nin #N
  targeting …` (seq 189 and 213 at 26). The collapse works one seam away, on cast-row target clauses
  and on the battlefield block (`Glacial Fortress #1-#4 x4; Island #1-#8 x8`).
* **Forced-sacrifice menus:** `130v125` s181-s186 and s189-s194 — twelve consecutive annihilator
  asks of 18, 17, 16, 15, 14, 13 / 13, 12, 11, 10, 9, 8 rows, **fourteen of them byte-identical
  Mountains**, while the battlefield line directly above collapses the same permanents.
* **De-duplicated list of length 1:** **16 asks / 147 rows / 2.9 min** corpus-wide, 14 of them at
  `152v123` s23-s37 — a damage-assignment-order menu shrinking 12 -> 6 rows in which every row is
  the literal string `Vampire (2/2) [flying, doesn't untap during its controller's untap step]`.
  One of them (`152v123` s29, a six-row menu) answered `CHOICE: 7` — out of range, one of the
  corpus's 15 fallbacks, correctly recovered by lane Q.
**Ask, in that order.** (a) Extend lane P's `groupNumberedRows` permutation to the ability, sacrifice
and library-reveal menus, with the same index map-back it already proves index-safe. (b) ONLY THEN,
where the collapsed list has a single entry and the engine's own handle map says the members are
interchangeable, resolve internally and log it as `mana_only_windows_skipped` (406 corpus-wide)
already does. (b) is stated second on purpose: it is not a hard cap only if (a) has demonstrated
the choice is vacuous, and until then the model keeps the ask.
**Wave-54 falsifier.** Ability/sacrifice/reveal menus with > 12 rows over one verb+source and `#N`
handles: **0** (was 28 at one seat). Asks whose de-duplicated option list has length 1: **0** (was
16). Max non-log prompt segment stays **< 12 KB** (this wave 11,231, PASS).

### R169 — a reply that eats 96.5% of the deadline and ANSWERS is indistinguishable in the ledger from one that took five seconds. (= ledger **D9**; seat-125-126 **LOW #8**, seat-146 **E-7**)
Lane Q's timeout arm is correct and untested: it stamps `fallback: "timeout"` + `retry: 1` on an
**empty** body at >= 95% of the deadline, and **no reply in this corpus was empty**. Re-derived, the
corpus holds **six** replies at or above 600 s, all of which answered and none of which carries any
stamp: `126v125` seq **13** at **868,729 ms = 96.5%** of the 900 s wall, on a **3-row, 5,780-char**
menu (its reply is a visible self-argument and it is that seat's only `answer_replaced`, and it is
one of the two genuine mis-executions in R173); `126v146` seq 22 (**718,196**) and seq 23
(**608,145**), the two windows that lost that game; `123v126` seq 1 (**660,402**, the opening
mulligan on a **1,545-char** prompt); `152v123` seq 16 (**629,642**, a three-row land drop);
`162v126` seq 10 (**703,161**). Five different seats. The mulligan ask is systematically the most
expensive decision of a game (`146v125` s1 at 595 s; five seats above 140 s).
**Ask.** Stamp any reply above the same `>= 95%` threshold with a `long_reply` note and the elapsed
fraction — the classifier is already written and pure (`noAnswerClassFor`), and this is the branch
it does not cover. **Not** a deadline change: `WAGIC_GPT_TIMEOUT` is the owner's dial and this item
is observability, exactly as wave 52 ruled for D10.
**Wave-54 falsifier.** Replies whose round trip reaches >= 95% of `WAGIC_GPT_TIMEOUT` and carry no
stamp of any kind: **0** (was 1; 6 above 600 s).

### R170 — the `[DRAW PRICE:` tail reaches one of the tag's two forms; the other 60 rows print a cost with no total. (= ledger **D10**; deck125 G3, deck130 G7, seat-123-130 **M3**)
Re-derived, and this settles a three-way disagreement between the seats (skill #208). Corpus-wide
there are **85 `[DRAW PRICE:` row-instances in 71 records**, in exactly two emitter forms, and the
split is total:
* the **Forced Fruition form** — `[DRAW PRICE: casting this draws YOU N cards (their Forced
  Fruition), and their Underworld Dreams, Ob Nixilis, the Hate-Twisted deal you N - you would be at
  K]`, which states a DAMAGE TOTAL and therefore gets lane O's tail: **27 rows, 27 tailed (100%)**,
  **16 of them carrying `; this KILLS you`** (8 records), all at `125v162`, **0 casts from a KILLS
  row**. Every lethal row in the corpus carries the tail: **16/16**. This is the wave's single most
  valuable render change — last corpus the identical menu at 3 life took the counter and lost the
  game; here eight lethal rows were declined, including seq 88 where all three cast rows were
  counters aimed at an Underworld Dreams on the stack.
* the **cost form** — `[DRAW PRICE: this draws N card(s), and the opponent's <punishers> punish
  every draw, so taking it costs you N life right now]`, which states a COST rather than a total and
  gets **no tail on any of its 58 rows** (37 on `Cast …` rows, **21 on cycling ability rows**).
**Who was right.** seat-125-126's "8/8 records carry `this KILLS you` with the tail" is exactly
right. The engine seat's "85 rows, 16 lethal tails" is exactly right. seat-123-130 reported "0 of
58" over its own two seats: its DENOMINATOR is right to the row (deck123 + deck130 render exactly 58
DRAW PRICE rows) and its NUMERATOR is off by six (**6** of those 58 carry the tail — its seats do
meet the Fruition form, at `130v162`), and its diagnosis — "the tag states a damage total in all 58,
re-check the call site's `life` argument" — is **wrong**: the call site is fine and the two tags are
different functions. The finding underneath it is real and is what this item docket: **58 rows
corpus-wide state a life cost and print no resulting total.** None of them was lethal this corpus,
which is why this is MED and not HIGH.
**Ask.** Give the cost form the same finished subtraction (`… costs you N life right now - you would
be at K`, `; this KILLS you` at K <= 0). The guides now teach "read the number after `you would be
at`", and 60 of 85 rows do not have one.
**Wave-54 falsifier.** `[DRAW PRICE:` rows of ANY form stating a life cost and carrying no
`- you would be at K`: **0** (was 58/85). Casts from a row carrying `this KILLS you`: **0** (0/16).
Lethal rows carrying the tail: **N/N** (16/16 — must stay).

### R171 — unconditional removal carries a perfectly knowable victim list and no `{kills: <name>}`. (= ledger **D11**; engine seat **D-4** second half)
**244 of the 413 creature-targeting removal cast rows** carry no kill verdict — Path to Exile 224,
Fateful Absence 15, Vanishing Verse 5 — because lane O's magnitude test ("kills at N damage") is
inapplicable to a spell that simply exiles. `125v126` seq **13-23** is eleven consecutive Path rows
each printing a full target list and no verdict at all. The information is free: the target list is
already enumerated on the row.
**Ask.** For unconditional removal, emit `{kills: <name>}` (or `{removes: <name>}`) from the row's
own target list, single or several, using the same `castKillSummaryTag` writer.
**Wave-54 falsifier.** Cast rows for unconditional removal carrying a victim clause: **N/N** (was
0/244).

### R172 — the PLAN block is bounded by BYTES and not by SHAPE, and a stale plan is never diffed against the current menu. (= ledger **D12**; deck123 G1, deck162 **R-9**; skill #207)
**The filed repro is refuted and the class is real.** seat-123-130 H3 reports that `123v130` s57's
`PLAN:` swallowed ~2,000 characters of deliberation, including a false Morbid claim, and that s63/s64
were served all of it. On disk the served block at s63 and s64 is **151 characters**
(`"Fetch a dual (Tundra or UGS), then cast Intruder Alarm. …"`), with no Morbid claim and no phase
claim: `consumePlan` anchors on the **LAST** line-leading `PLAN:` marker (s57's reply has markers at
char 35 and char 3,755) and bounds the result at **1,600 bytes** with a sentence-boundary cut. Both
mechanisms worked. The false Morbid assertion the seat saw is in s64's and s65's **own** replies,
which is R174's class, not this one.
**What IS open.** (a) 1,600 bytes is far more than the protocol's "a few sentences", and the bound
is a byte count rather than a shape: a paragraph bound (capture to the first blank line, or to the
first sentence that starts a new line without a connective) is the cheaper and more honest limit.
**This is a bound on the pilot's own prose, not on any menu or choice, and it removes no legal
window.** (b) The genuine staleness instances are `162v152` s11 -> s12 (the plan the reply wrote
names `Cast Master of the Feast`, and the row it took in the same breath made Master unaffordable;
s12 was served that plan verbatim) and `125v126` seq 294/332 (recognition drift across repeats of an
identical board, at a cost of 30 life). Lane N's age stamp is working — **3,329 renders, 0
unstamped** — so staleness is visible as a NUMBER and not as a FACT.
**Ask.** The paragraph bound, plus a one-clause diff of the served plan against the CURRENT option
list: `YOUR PLAN (as you last stated it, 1 window ago on turn 10; "Master of the Feast" is no longer
on your menu)`. The engine already tracks the plan's provenance; naming the contradiction is the
smallest thing that makes a stale plan LOOK stale.
**Wave-54 falsifier.** Served PLAN blocks longer than one paragraph: **0**. Decisions taken on a
served plan naming a card absent from that window's option list, with no contradiction clause:
**0** (was 2).

### R173 — the latch path can execute a row that neither the index nor the name chose, with no note. (= ledger **D13**; engine seat **D-8**)
`126v125` seq **13**: reply `CHOICE: 2 (Cast Idyllic Tutor)` on a 3-row menu executed row **1**,
`Cast Perimeter Captain`; `coded_answers: 2`, `latched_coded_line: "2"`, `parse_note: None`. Seq 14
repeats it. Both times the index and the name AGREED with each other and disagreed with the executed
row, which points at the latch/replacement path rather than the echo parser. Corpus-wide the
reply-vs-executed-row audit is **25 disagreements over 3,253 parentheticals**: 6 cosmetic (the name
is a correct prefix of a longer label), 15 prose reversals the engine correctly resolved to the last
coded line, 2 the HOLD index-0 degradation (R162a), and these **2 genuine mis-executions** — a rate
of 2/3,253, **at** the carry's `<= 1/3,000` ceiling rather than under it.
**Ask.** At minimum a note when the latched coded line's index AND its parenthetical both differ
from the executed row; better, a re-ask, as the named-row path already does.
**Wave-54 falsifier.** Records where the reply's index and its parenthetical agree with each other
and neither matches the executed row: **0** (was 2), or **N/N** carry a note.

### R174 — a reply whose PLAN line contradicts its own CHOICE, on a row the engine has itself labelled a no-op, is executed silently. (= ledger **D14**; seat-125-126 **MED #4**, seat-123-130 **H1** false-negative half)
`126v125` seq **73** and **74** (turn 42, 6 life), two windows of one turn. Both menus are two rows:
`1. Cast Tribute to Hunger {2}{b} {right now: they control 0 creatures - at 0 this does nothing}` /
`2. Cast nothing right now`. Seq 73's reply: `CHOICE: 1 (Cast Tribute to Hunger)` + `PLAN: … Avoid
casting Tribute to Hunger as there are no creatures to target.` Seq 74's: the same choice +
`PLAN: The opponent has no creatures, so Tribute to Hunger does nothing. Pass the turn.` Both
executed; `answer_replaced` false, no parse note, no `decision_reversed_in_prose`. Same class at
`123v125` s84 (a dead Damnation cast under prose reaching the opposite verdict with no second coded
line) and `123v130` s64/s65 (a PLAN asserting Morbid against a row printing
`no creature has died this turn, so Morbid does NOT apply`).
Lane J's detector reads the prose BEFORE the `CHOICE:` line and structurally cannot see a PLAN line
after it.
**Ask, strictly ADDITIVE.** Stamp — do not suppress and do not re-ask — when the chosen row's own
annotation says the action does nothing (`at 0 this does nothing`, `deals 0`, `destroys 0 of their
creatures`, `kills 0 of the`, `does NOT apply`) AND the reply's PLAN line names that same action
negatively. At minimum the ledger can then count the class; whether it should re-ask is a decision
for the wave after the count exists.
**Wave-54 falsifier.** Records whose chosen row carries a no-op annotation and whose PLAN argues
against that row, carrying no stamp: **0** (was 4 at two seats).

### R175 — the echo matcher binds a card name that appears only inside another row's annotation. (= ledger **D15**; seat-125-126 **MED #5**)
`126v130` seq **21** (turn 21). Menu: `1. Cast Idyllic Tutor {2}{w} [finds only an enchantment card
- every enchantment left in your library is a copy of one you already control or hold: Exquisite
Blood, Sanguine Bond] …` / `2. Cast Overgrown Battlement` / `3. Cast nothing right now (combat comes
next this turn)`. Reply: `CHOICE: 3 (Cast Sanguine Bond)`. `Sanguine Bond` is on no row's LABEL and
appears exactly once on the whole menu — inside row 1's `[finds only …]` list. Result:
`parse_note: echo_index_conflict` (no `name_over_index`), **row 1 executed** — neither the index the
model wrote nor a row offering the card it named. The same shape at `126v125` seq 9 correctly
produced `named_row_not_offered` and a re-ask, so the seam resolves one shape two ways.
**Ask.** Match echoed names against **annotation-stripped row cores on every tier**, not only on the
exact-head path (`stripRenderAnnotationsLc()` already exists), so a name that appears only inside
`[finds only …]`, `{kills: …}`, `{right now: … <name> is sacrificed}` or
`[second copy: you already control <name>]` can never bind.
**Wave-54 falsifier.** Records whose reply names a card that occurs on the menu only inside an
annotation, and which execute a row rather than re-asking: **0** (was 1).

### R176 — the mulligan coverage line collapses two different causes into one string. (= ledger **D16**; deck146 **R-4**; skill #203)
`Playing every land in this hand would not cover any spell in it` is printed both for "you have too
few lands" and for "you have enough lands of the wrong colour". Six of the seven mulligan decisions
that met that line were correct; the one that was not (`152v162` s1 — 2 lands making {G}{G} against
five white spells) was the COLOUR case, and the reply's stated reason was the colour
(*"Mulligan for a hand with at least one white mana source"*). The engine has both facts on adjacent
lines already: the same prompt prints `Your hand (7 cards), counted by the engine: 2 lands, 5
spells.` and `Mana sources among those lands, counted by the engine: {G} 2.`
**Ask.** Append the cause: `… (you have 2 lands; no spell in it is castable off {G} alone)` versus
`… (1 land is not enough for your cheapest spell at mana value 2)`.
**Wave-54 falsifier.** `would not cover any spell` renders without a cause clause: **0**.

### R177 — the attackers prompt lists every potential blocker per attacker and never prints how many blockers exist. (= ledger **D17**; deck152 **R-5**)
Each A-line carries `their untapped blockers: <name> (P/T) (<result>); …`, repeated in full on every
attacker. What decides an alpha strike is the opposite number. At `152v126` s26/s32/s40 the same
**two** blockers were re-listed on four A-lines each time; the seat declared two attackers into two
blockers three combats running, held a 4/4 and a 7/5 whose worst printed price was 1 life, and lost
0-21 with the opponent never below 16.
**Ask.** One line in the attackers header, beside the existing LETHAL/converter lines:
`They have N untapped creatures able to block; declaring more than N attackers leaves at least
(your attackers - N) of them unblocked.` The engine has N already — it built the per-attacker lists
from it. This is the attack-side twin of the block header's `Unblocked, these attackers deal up to
X - you would be at Y`, which demonstrably works (9 block windows across those seats, 0 floor
breaks). Guide half: skill #202.
**Wave-54 falsifier.** Attackers prompts with no blocker-count line: **0**.

### R178 — a cast menu prices each row alone; nothing says which rows fit together in this window. (= ledger **D18**; deck162 **R-8 HIGH**; skill #205)
`162v152` s11 (turn 10, **4 life**, five untapped sources) offered on ONE menu:
`Ob Nixilis, the Hate-Twisted {3}{b}{b} {leaves 0 of your 5 … - casting this taps you out}` /
`Master of the Feast {1}{b}{b} (5/5) {leaves 2 of your 5 …}` / `Shield Sphere {0} (0/6) {leaves 5 of
your 5 …}`. Rows 3 and 4 are affordable **together** — a 5/5 flier and a 0/6, two blockers against
the four attackers the opponent's header had just printed. The seat took row 2, tapped out, the next
menu had two rows on it, and its own losing reply's PLAN said *"Cast Master of the Feast"* — a plan
the row it took had already made impossible.
**Ask.** Point the existing `{spends K of your M untapped mana sources this turn; <card> in your
hand needs N}` computation (666 renders corpus-wide) at the MENU instead of the hand:
`{taps you out - rows 3 and 4 on this menu become unaffordable}` on the rows that do, or
`{after this, rows 3 and 4 are still affordable}` on the rows that do not. The engine recomputes
affordability for the next ask anyway; printing it one ask earlier turns a sequence of single-row
prices into a window budget.
**Wave-54 falsifier.** `taps you out` rows carrying no what-else-dies clause: **0**.

### R179 — the effect label degenerates to the source name on 53 stack lines. (= ledger **D19**; engine seat **D-5** second half)
Lane P's D4 shipped clean — **706 stack lines emitted, 0 bare** (`ability: <source>'s <effect>
[from your/their <source>]`, 9 with the aim clause, e.g. `ability: Path to Exile's put a card onto
the battlefield (aimed at Plains) [from your Path to Exile]`). The residual is cosmetic and
systematic: **53** lines read `ability: Ob Nixilis, the Hate-Twisted's Ob Nixilis, the Hate-Twisted`
because the effect label falls back to the source name when the ability has none.
**Ask.** When the effect label equals the source name, print the ability's own menu text or the
generic (`triggered ability`) rather than the name twice.

### R180 — fetch rows name a land and never its colours. (= ledger **D20**; seat-123-130 **M2**, deck123 G3; skill #191)
`123v130` s36. The Marsh Flats menu printed seven rows — 3 Plains, 4 Scrubland, 5 Swamp, **6 Tundra,
7 Underground Sea** — each carrying `{this land makes no mana - crack it for a land: your untapped
mana sources go from 2 to 3}` and **no colour on any row**, while the mana line read
`colours you can make: {b}{w}` and the hand held two `{2}{u}` Intruder Alarms. The reply says
"I need a source that makes {U}" and answers **5, Swamp**. Tundra is `subtype=Plains Island`
(`mtg.txt:124691`) and Underground Sea `subtype=Island Swamp` (`mtg.txt:125732`); both are legal for
Marsh Flats' `target(*[swamp;plains]|myLibrary)` and both make {U}. **90 renders** of that clause at
that seat, colourless every time. The game was lost on turn 18.
**Ask.** Continue the existing clause with the colours in the words the mana line already uses:
`… sources go from 2 to 3, and it adds {W} or {U} (you cannot make {U} right now)`. The emitter
already prints `(land: taps for {W}{U})` on hand lines.
**Wave-54 falsifier.** Fetch target rows naming a land and no colours: **0** (was 90).

### R181 — the non-lethal blockers hint asserts "while ahead on LIFE" without checking the life totals. (= ledger **D21**; seat-123-130 **M4**, deck123 G4)
`123v130` s55, at **7 life against 12**: the blockers header printed `Unblocked, these attackers
deal up to 6 - you would be at 1 - NOT lethal: block only where the trade favors you; taking damage
while ahead on LIFE is often correct`. The pilot was BEHIND on life, answered `BLOCKS: none`, went
to 1 and died next turn.
**Ask.** Gate the second clause on the two totals the emitter already holds — print it only when
`my_life > opp_life`, otherwise print nothing (the guide owns the rest). A general hint that states
a false premise is worse than no hint, and this one is the only render falsehood in the corpus.
**Wave-54 falsifier.** `while ahead on LIFE` printed at a record where `my_life <= opp_life`: **0**
(was 1 of 1 such window).

### R182 — the `{paying this taps:` clause names the creatures but not the count, and the count is what decides. (= ledger **D22**; deck152 **R-7**; skill #204)
Lane O's rework landed cleanly on the defender case (0 old-string renders on a defender, 27 new).
What the clause still does not say is how many attackers it removes, which is the whole trade: at
`152v130` s11/s14 the clause held **one** name and taking the row was right (that game was won
11 to -4); at `152v162` s29/s32 it held **two** and passing was right.
**Ask (cheap).** Lead with the count: `{paying this taps 2 of your creatures: Luminarch Aspirant,
Intrepid Adversary - they cannot attack this turn}`.

### R183 — the LIFE-TO-DAMAGE CONVERTER block describes the effect symmetrically and never says who it is bad for. (= ledger **D23**; deck126 **G5**; skill #193)
Rendered **353 times** to deck125 (`theirs - Sanguine Bond`) and 147 to deck126 (`yours`), it is the
best single-purpose surface either seat has and it is what deck125 re-keyed two rules onto. The gap:
it ends *"This is not a trade: when you gain N, your total goes UP by N and theirs goes DOWN by N in
the same event"* and leaves the pilot to derive the consequence for the CARD IN FRONT OF IT. Both
decisions it should have decided were about a specific row — an Emrakul an edict turns into 15 life
for the converter's owner (`125v126` seq 294: 36/8 became **21/21** in one window), and a 1/1
lifelink token that is a 2-per-combat clock.
**Ask.** When the block says `theirs`, append one clause naming the class most affected — *"any
creature of theirs with lifelink is worth double its power against you, and any creature of YOURS
they can make you sacrifice pays them its toughness"*. One sentence on a block that already exists,
not a per-row annotation.

### R184 — the `{spends K of M …; <card> in your hand needs N}` clause names a card that may not be the binding one. (= ledger **D24**; deck125 **G2**)
The clause does real work (666 renders) and deck125 re-keyed its tap-out gate onto it. At
`125v162` seq **23** it read `{spends 2 of your 2 untapped mana sources this turn; Path to Exile {w}
in your hand needs 1}` — naming a Path, while the card the seat had to protect was a Fall of the
Gavel at 5. A guide keyed to the clause then has to warn the pilot that the NAME is not the price,
which is a sentence the render can make unnecessary.
**Ask.** When more than one unpayable card is in hand, name the **most expensive** one, or name the
count (`3 cards in your hand need more than that`). The subtraction half (`spends K of M`) is
correct in every render checked and is not touched.

### R185 — the forced-sacrifice list arrives in battlefield order, cheapest last. (= ledger **D25**; deck126 **G8**)
`126v125` seq **111**: a 23-row list in battlefield order — 15 lands, an enchantment, a
planeswalker, two artifacts, a wall, and the seat's only lifelink Vampire **last** — under a header
saying "usually your LEAST valuable: a spare land or a redundant/dead card". The pilot picked row
**23**. At seq 112, on the identical list, it reasoned correctly and picked lands. Six sequential
picks from a 23-row list is the widest ask either deck faces and it arrives while a game is being
decided.
**Ask.** Sort cheapest-first (lands, then duplicates, then the rest) or tag the obvious candidates
(`[spare land]`). Pairs naturally with R168's collapse of the fourteen identical Mountains.

---

## LOW

### R186 — the `(xN)` collapse still misses phase lines, draws and loyalty counters, and the miss GREW. (= ledger **D26**; engine seat **D-6**)
**571** duplicate event lines inside single records (wave 52: 280): `- Phase: Draw` 237,
`- Opponent drew a card` 116, `- Phase: Main phase 1` 71, `- Phase: Combat damage` 9,
`- Opponent's Ob Nixilis, the Hate-Twisted lost a loyalty counter` 5. Lane Q measured the ADJACENT
case correctly and declined it (0 adjacent runs >= 3 survive, and collapsing non-adjacent repeats
across turn boundaries would erase turn structure) — that verdict stands. What grew is the
within-record duplicate count at a log that is 75-80% of every 30 KB prompt, with 476 trims firing.
This is free space and is the only lever on the tail besides R192, which is the owner's.

### R187 — receipts without a payee: 2 (7 -> 3 -> 2). (= ledger **D27**; engine seat **D-7**)
`152v126` seq 25 (`- Paid {1}{w} with Plains #2; Deserted Beach`) and `152v162` seq 29 — both the
Intrepid Adversary pay-any-number-of-times ETB. **496 receipts, 0 with no source tapped**, 11 in the
legitimate `from mana already floating` form.

### R188 — ability rows that point damage at a player still print no life total. (= ledger **D28**; seat-123-130 **L1**, deck130 G9; R157 carried)
`130v125` s120, s158; `130v162` s35 — `Deal 2 damage with Pyrite Spellbomb targeting the opponent`
beside target menus that print `The opponent (player, life 8)`. With R164 this is the same ask from
the other side: every row that can point damage at a player prints that player's life.

### R189 — the bare `[second copy: … no legend rule]` now reads as the "safe" tag by contrast. (= ledger **D29**; deck126 **G7**, recorded as a wording risk created by a good change)
Lane P gave the Lantern a longer tag ending "this copy adds only its own abilities" and deck126
declined **6 of 6**. On the same seat `[second copy: you already control Sanguine Bond; both stay on
the battlefield - no legend rule]` — correct, because two Bonds really do both trigger — was taken
at `126v125` seq 22. With one form now carrying a "this does nothing extra" tail, the ABSENCE of
that tail reads as "this does something extra", which for a Sanguine Bond is technically true and
strategically false. deck126's own recommendation is (a) leave the tag alone and let the guide carry
it, which it did (D86); recorded here so a later wave does not read the take as a render failure.

### R190 — the priority seam's ask line does not say whose turn it is. (= ledger **D30**; deck125 **G4**)
Casting asks read `Casting decision (Draw, opponent's turn): which card do you cast now, if any?`,
which is good; the **priority** seam's ask line carries no turn attribution, and the priority seam is
where deck125's repeating two-row Elixir menu lives. Cheap and purely additive.

### R191 — `pass_row_named` never renders: the behaviour is right and the tag is missing. (= ledger **D31**; engine seat, R144's residual)
284 replies wrote a reserved pass echo on a menu carrying `0. Pass priority` and **284/284** executed
row 0 — lane N's D9 is a clean behavioural PASS — but **0 records carry the predicted
`pass_row_named` note**, so the wave-54 corpus cannot distinguish "the reserved-echo path fired"
from "the index happened to be 0". A one-field stamp.

### R192 — MEASUREMENT / DECISION: the log window, with the cap working harder and the tail still growing. (= ledger **D32**; R160 continued — the OWNER's call)
**476** prompts carry `(…earlier events trimmed…)` (wave 52: 165); log segment max **24,107**, p50
9,107; max non-log segment **11,231** (lane P's `< 12 KB` prediction PASSES); **12 prompts exceed
30,000** (wave 52: 7) and **588 exceed 25,000** (wave 52: 330); mean `ask` prompt up ~13% while the
max is flat. Every prompt above 29 KB is ~75-80% log. The corpus maximum is `146v123` seq **92**
(32,068 chars, turn 33, priority). Log compaction is the only remaining lever on that tail and the
decision is the owner's, not this step's.

---

## CARRIED ENGINE ITEMS FROM THE VITA LANES AND THE CARRY LIST — not corpus-testable, still open

- **Replay is unusable on human-vs-Baka dumps, for four separate reasons, and it blocks every
  human-report investigation.** (i) **RNG-stream drift** (lane T, NOT fixed):
  `GameObserver::randomGenerator` is ONE shared stream serving both `MTGGameZone::shuffle` and the
  AI seats' own `random_shuffle` in `AIPlayer.cpp`; AI seats are passive during a replay, so they do
  not consume their values, every later draw is offset, and a mid-game shuffle produces a different
  library. The fix is a separate `RandomGenerator` per AI seat (the class comment says that was the
  intent) or recording their draws separately. (ii) **A chooser-order divergence** (lane U):
  `WAGIC_REPLAY` on the vpk12 report-A dump diverges at action 83, turn 6, `chooser=read the bones`
  — a scry/surveil window resolving in a different order, and it fires BEFORE the game's only
  shuffle, so it is a distinct defect. (iii) Lane W's two dumps: one diverges at action 94
  (`p2.no 1` vs `p2.choice 0`), one **aborts in `dumpAssert` at load**. (iv) Lane R's dump diverges
  at action 49 with the hand one card short. Lane T's index-drift fix (name-based resolution under
  `WAGIC_REPLAY`) shipped and carried one dump from action 112 to 116; everything above is what
  remains. **Until this is fixed, every owner play report is investigated from a transcript and a
  fixture, never from the board that produced it** — four lanes said so independently this wave.
- **`kMaxOptSources` is 14 and the owner's option-preserving autotap is a no-op above it** (lane S).
  `refineForOptions` bails at `sources.size() > kMaxOptSources`; the owner's reported board offered
  **16** candidate producers, so the spec he wrote never applied in the game he reported. Raising the
  cap needs its own cost measurement — flagged, not changed.
- **`planPayment` / `potentialMana` / `selectAutoTapProducers` cannot see a `&&`-wrapped producer**
  (lanes R and S, the same blindness at three call sites). `ActionLayer::manaObjects` is built
  through `AbilityFactory::getCoreAbility`, but the readers use a plain
  `dynamic_cast<AManaProducer*>`, so `{T}:add{G} && counter(0/0,1,nest)` (Twitching Doll) is in the
  index and invisible to its readers. Lane R fixed the ONE site where it hung the phase ring (the
  unpayable-commitment release, now a permissive local walk) and deliberately left the planner walks
  alone because widening `potentialMana` itself regressed five fixtures (measured, then reverted).
  Residual: such a source must still be clicked by hand, and AI payment planning cannot see it.
- **The Dominating Vampire livelock candidate** (lane R, investigation only, NOT fixed, and stated
  as a hypothesis): `MayAbility::testDestroy` returns 0 while any `menuObject` is armed and
  `MayAbility::Update` fires once, so a `may` that trips its no-valid-targets early return on a tick
  when another card's menu is open stays registered forever with `triggered == 1`, still answering
  `isReactingToTargetClick` once targets become legal — while `AIPlayerBaka::computeActions` and
  `chooseTarget` both return 0 outright for a seat that is not `currentlyActing()`, so a chooser
  armed for the AI during the human's turn can never be answered and `ActionLayer::Update`
  early-returns on `menuObject`, pinning the phase ring. Four fixtures failed to arm it. The next
  step that settles it is a per-tick instrumented run printing `menuObject` /
  `currentWaitingAction` / `currentlyActing` and flagging any menu armed for a non-acting seat.
- **`GuiPlay::receiveEventPlus` calls `Replace()` unconditionally on every permanent entering play**
  (`src/GuiPlay.cpp:394`), i.e. a full re-layout of the play zone per ETB event, before the card is
  added to the CardSelector. On the wide boards lane W profiled (20-29 permanents, token swarms) that
  is an O(n) relayout per token created. Not measured on the console; recorded beside lane W's
  measured render ranking so the next frame-rate pass has it.
- **There is no `assertmtgid` suite command** (lane V), so a post-flip printing id cannot be pinned:
  PARSETEST runs before the card database loads and the suite deliberately SKIPS `setMTGId` on flip
  (`playMode != MODE_TEST_SUITE`). Lane V's `wagicPickFaceSiblingId` is pinned pure on the real MOM
  ids; the assembled `getOtherFaceCard` is not observed at runtime by any harness. Worth building.

---

## SHIPPED SINCE THE CORPUS — recorded, NOT docketed
**Lanes U, V, W and X are NOT in this corpus's binary (c628dd531).** They are adjudicated in the
ledger's Set B as SHIPPED-UNTESTED with their wave-54 predictions, and no rate in this file reflects
them: lane X (the reveal hang, master), lane U (Baka's deathtouch blocking + the per-observer
game-end one-shot), lane V (the DFC back-face printing/art fix + 207 repaired images, art NOT
uploaded, VPK not built), lane W (the `GuiGameZone::Render` skip + the alpha-gated Vita frame
telemetry).

## GUIDE-ONLY — stays in guides after the render ships
- **deck123**: A53-1 (the fetch carve-out fenced; #191), A53-2 (the Slip rule re-keyed to
  `{kills …}`; #187), A53-3 (the plan-vs-`{right now:}` sentence; #207), A53-4 (the HOLD row), A53-5
  (the same-turn Damnation), A53-6 (the blockers hint's premise), A53-7 (the twelve hands completed).
- **deck125**: W77a-c (the Emrakul stop re-keyed to the CONVERTER block; #193), W78a/b (the Fruition
  threshold as a constant against `- you would be at K`; #192), W79a/b (the Path carve-out), W80
  (the tap-out gate on `{spends K of M}`), W81 (the "nothing else was castable" refusal; #194),
  W82a, W83, W85/W86 (the trims and the #177 diffs), W87 (the one-land coverage branch).
- **deck126**: D84 (the format check; #196), D85 (the dead Tutor), D86 (the second enchantment),
  D87a (the walls that gain nothing; #195), D87c (cast something every turn), D88 (the block floor).
- **deck130**: D53-1/D53-2 (the win step promoted to step 0; #199), D53-3 (land destruction at a
  lethal opponent), D53-4 (the HOLD row), D53-6 (the mulligan anecdote compressed; #200).
- **deck146**: 146-A (entry 5 re-keyed off an unsatisfiable literal; #188), 146-C (the cleanup order
  given a DECIDING SITUATIONS surface; #190), 146-T deleted on its live denominator (#201).
- **deck152**: 152-A (the converter hold given the list's cardinality; #202), 152-B (#171's scoping
  sentence ported from deck146; #203), 152-C (the tap clause's name count; #204).
- **deck162**: 162-A (the survival gate moved to the cast window; #205), 162-B (the first-Mine
  exemption given imperatives; #206).

## MEASUREMENT items for the wave-54 reviewer briefs
1. HOLD row: renders, takes, `hold_windows_skipped` **per gameend**, and takes as a fraction of
   windows already carrying `[you declined this exact list N times…]` at N >= 3 (R162). Report
   `mana_only_windows_skipped` beside it (406 this wave, the suppression that works silently).
2. Opponent-turn windows per phase: offered / acted / seconds, plus consecutive identical-declined
   run lengths per turn, on a like-for-like denominator with this wave's 1,052 / 58 / 26.2% (R162).
3. Every `{kills …}` row: creature targets, player targets, printed damage, the seat's and the
   opponent's life, and whether the row was taken (R164, R171).
4. Every `[DRAW PRICE:` row **by FORM** (Forced-Fruition vs cost), with tail present/absent and
   takes; report row-instances AND records (R170, skill #208).
5. Modal cast rows: mode annotation present/absent, and replies asserting a mode's object is absent
   against a log or graveyard line that names one (R165).
6. Every `-1` decision: fallback field present, `recovery` record present, and whether any seat log
   lacks a `gameend` (R161, R167).
7. Reply-vs-executed-row audit by class, with `latched_coded_line`, `pass_row_named` and
   `answer_replaced` (R173, R191).
8. Menus by (printed rows, distinct shapes masking `#N`, distinct shapes masking all digits,
   de-duplicated list length), for every ask kind (R168) — report all four, per skill #208.
9. Latency: every record >= 90% of `WAGIC_GPT_TIMEOUT`, empty or not, with prompt chars and rows
   (R169).
10. The three-number literal audit (#168/#188), per (literal, CARD FAMILY), read from
    `options_text` for renders and `chosen_text` for takes, with the field named.
11. Served PLAN blocks: length, age stamp, and whether the plan names a card absent from that
    window's option list (R172).
12. `- Paid` receipts, duplicate event lines per record, prompts over 25,000 and 30,000, log share
    (R186, R192).

## PASS / KEEP — measured working, do not re-open
Lane M's blockers seam (`multiblock_reask` **0**, 30/30 asks paired with a `declared blocks from N
assignment(s)` line, `multiblock_first_wins` first live firing, blockers prompts **34% smaller**) ·
lane N's pass-row echo (**284/284**), the PLAN age stamp (**3,329/3,329**), and the HOLD row's
placement contract (LAST row **1,173/1,173**, opponent's turn only) · lane O's DRAW PRICE lethal
tail on the form it reaches (**15 rows, 0 casts**, and eight lethal declines that would have been
the loss), the kill summary's EMISSION (**169 rows**, 0 emitter disagreements between the cast row
and the target verdict), and the defender tap clause (**0 old-string on a defender, 27 new, 14
correct non-defender**) · lane P's stack lines (**706 emitted, 0 bare**), the `[second copy:`
usefulness verdict (**12 takes, all on stacking scripts; the Lantern 0/6 after 5/5**), the from-exile
cause (**2/2**), the land count in every battlefield header (**3,429/3,429**), and the `< 12 KB`
non-log segment (**11,231**) · lane Q's `recovery` contract (**7/7 paired, 0 unpaired**), the
`retracted_choice` class's first live firing (`146v125` seq 313, with the recovery record showing
Baka's substitute action), and D20's measured decline · receipts (**496, 0 with no source tapped**),
`turn` == narration turn **3,429/3,429**, `stale_livelock` **0**, land drops (deck123 46/47, deck130
55/60 with every hold followed by the drop, deck125 85/85, deck126 52/52, deck146 46/46, deck152
33/33, deck162 39/39), fallbacks **0.43%**, Baka-executed **0.11%** · **no render falsehood this
corpus except R181's one hint**, and the two mechanisms filed as falsehoods (the PLAN capture,
`decision_reversed_in_prose`'s value) were refuted on disk rather than fixed.
