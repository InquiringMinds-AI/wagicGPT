# Wave 72 — engine-narration seat

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-163626` (binary `ceb9b8de0` =
`archives/wagic-ba6af25f1-w72step1`, `--thinking on`, `WAGIC_GPT_TIMEOUT=900`, no operator token
ceiling). Baseline for every ratio: wave 71's `matchups-20260907-085638` + `wave71/engine-seat.md`.
Read-only: no build, no run, no git, nothing written outside `wave72/`.

**What I did NOT check.** No deck-guide reading and no strategy verdicts (deck seats own those). No
per-game narrative tracing beyond the decisions cited. No Scryfall calls; card facts were read off
the rendered prompt and, for Lair of the Hydra, off the row's own `{card text:}`. PARSETEST/suite
numbers are quoted from the lane files, not re-run. The three reasoning-body predictions (M8's
inverted-voice claim, M12's "no speculation about a prompt bug", M20's "zero replies justify a
sweeper by an un-animated land") are marked UNTESTED — I did not read 1,910 reasoning bodies.
The three BU suite fixtures were not re-run.

---

## 1. Corpus census

| | wave 72 | wave 71 |
|---|---|---|
| seats / games | 42 seats, 21 games | 42 seats, 21 games |
| completed naturally | 42 gameend, 21 `results.tsv` rows, **no `hang` row** | 42 gameend, 21 rows |
| crashes / harness timeouts | 0 / 0 | 0 / 0 |
| model decisions (records carrying a `prompt`) | **1,910** | 2,159 |
| by kind | ask 1,393 · priority 351 · attackers 102 · discard 26 · blockers 18 · reveal 17 · bottom 3 | ask 1,606 · priority 376 · attackers 81 · discard 50 · blockers 30 · reveal 12 · bottom 4 |
| summed turns / decisions per turn | 902 / **2.12** | 899(914 incl. gameend turn) / 2.36 |
| fallbacks | **3** (0.16%): 2 `noop_row_zero_reask`, 1 `http_error` | 4 (0.19%), all noop |
| `transport` | `curl=0,http=200,empty=0` on 1,909/1,910; one `http=400,empty=1` (the http_error) | 2,159/2,159 clean |
| `async_drops` | 32 | 17 |
| `deadline_pct` | p50 10.0 · p95 21.4 · max 51.8 | p50 8.8 · p95 20.9 · max 52.8 |
| latency | p50 **90.1 s** · p95 193.3 s · max 470.2 s | p50 79.3 · p95 188.6 · max 479.2 |
| HOLD rows rendered / taken | 1,181 / **260** (cast 828/160 = 19.3%, priority 351/100 = 28.5%) | 1,354 / 253 (cast 977/119 = 12.2%, priority 376/134 = 35.6%) |
| `hold_windows_skipped` (cast / priority) | 1,987 (**1,421** / 566) | 1,777 (774 / 1,003) |
| `mana_only_windows_skipped` | 24 | 270 |
| game length (turns) | mean 21.5, median 17, max 64 | mean 20.8, median 15, max 70 |
| wins | deck126 5/6 · deck146 5/6 · deck162 3/6 · deck152 3/6 · deck125 2/6 · deck130 2/6 · deck123 1/6 | — |

**Reply shape.** `protocol_replies` 1,910. `plan_line_missing` **73 (3.8%)**, `off_protocol_bytes>0`
**68 (3.6%)**, and the two classes overlap **68 of 68** — every off-protocol record this wave is a
mislabelled plan line, and the class-G residue (missing PLAN with 0 off-protocol bytes) is 5 records
(0.26%; wave 71: 7). `plan_line_missing` by seam: ask 44, attackers 12, priority 9, discard 5,
blockers 2, reveal 1 — the attackers seam is again the worst rate (12/102 = 11.8%), matching lane
BW's M15 reading that the label is dropped where the ACTION clause is longest. Off-protocol byte
weight: n=68, sum 4,736 B, median 72, max **152** (wave 71 max 294). `action_before_plan_replies`
**0**. `answer_replaced` 0. `post_answer_overrun > 0`: **0 records**.

**Reasoning (invariant 000).** 1,910/1,910 records carry `thinking: on` and non-empty native
`reasoning`. `reasoning_chars` min **1,166**, p50 5,749, mean 6,092, max 20,040 (wave 71 min 1,229,
p50 5,561, mean 5,997). `reasoning_degenerate` max 0.0116. **The corpus is valid under 000.**

**Rescue markers.** `reasoning_forced_close` **3** = `reasoning_budget_hit` **3** = records with
`retry: 1` **3**, all three re-issued at `max_tokens 400` at the `ask` seam
(`123v162` seq 23, `126v162` seq 12, `123v125` seq 5). But `phase2_answer_recovered` **2** +
`phase2_answer_missing` **1** = 3 — the wave-71 identity (`recovered + missing == closes`) still
holds, and the one MISS is `123v162` seq 23, which is also the corpus's only `http_error` fallback.
So the single non-noop fallback is a phase-2 recovery that lost its transport, not a parse failure.

**Ceilings.** `max_tokens_answer`: ask 288 (1,390) / 400 (3 retries); priority 288 (351);
attackers/discard/blockers/reveal 256; bottom 512 (3). **`reply_truncated` is present on 0 records
and true on 0** — the 288/256 fit is again unrefuted. `dropped_assignments` 0 on all 18 blockers
records; largest declarations offered were far under the cardinality floor, so the doubled-allowance
re-ask is again **UNTESTED**.

**`askreplay/` sidecar — now IN the corpus (BT M1 delivered).** 29 files, **521 records**, exactly
`ask_replays_reserved` 521. `why`: `repeat_ask_reserved` **327**, `cache_replay` **194** — the
composition INVERTED from wave 71 (68 / 657). `ask_replays_refused` 0.

**`plan_steps_done`.** `(done, count)`: (0,1) 873, (1,2) 698, (1,3) 230, (1,4) 70, (1,5) 16, (0,0)
12, (2,4) 4, (2,3) 2, (1,6) 2, (3,4) 2, (0,2) 1. The pointer reaches ≥2 on **8** records of 1,910
(wave 71: 9 of 2,159). Unchanged: the model re-states its plan far more often than it walks it.

### Why 249 fewer decisions than wave 71 — answered

**Not the M14 instrument, and not a lost window.** `own_turn_windows_skipped` is REPORT-ONLY by
construction (`AIPlayerBaka.cpp:4019`, a no-op virtual overridden only to increment;
`AIPlayerBaka.h:97`), so its 718 cannot have removed a single decision — those windows cost nothing
in wave 71 either. Both behaviour-changing collapses this wave fired **zero** times (§2). No other
window-removal shipped.

**It is one seat.** Wave 71's outlier `deck123-0x5629c69e3090 vs deck125` spent **279 decisions over
49 turns (5.69/turn)** on the ask-replay drain loop the wave-71 seat named as MED-2. The same
matchup this wave ran 33 turns / 86 decisions (2.61/turn). That one seat is **193 of the 249**.
Excluding it from both corpora: 1,880 decisions / 865 turns = **2.173** (w71) against 1,824 / 869 =
**2.099** (w72) — **−3.4%**, not −10%. The residual −3.4% is the cast-seam hold: the take rate rose
12.2% → 19.3% and `hold_windows_skipped_cast` 774 → 1,421, exactly the direction lane BW's M21
predicted and exactly the risk its own weakest-evidence #1 named.

---

## 2. REGRESSION WATCH — the wave-72 window model

### `sibling_window_asks_skipped` = 0 — the arm is UNREACHABLE from the seam that takes the holds (**HIGH-1**)
Zero skips, and zero `not asking the … seam over the same board` lines in any stderr (the arm's own
DebugTrace). Not one window was wrongly collapsed, so **no legal row was removed** — but the
mechanism is dead for a reason that is worth naming, and it is not the reason the CORPUS line
assumes.

* **The class did not vanish.** Adjacent cast/priority pairs at the same (seat, turn, phase): **243**
  in wave 72 against 222 in wave 71, and **72 of them had the casting ask answered with the HOLD
  row** (wave 71: 60). The double ask BU set out to remove is still being paid for at a slightly
  HIGHER rate than the wave it was measured on.
* **What died is the board-identity test.** Of those 72 hold-opened pairs, **0** have a
  byte-identical `--- CURRENT SITUATION ---` block (wave 71: 19 of 41 — BU's predicted ~19,
  reproduced exactly by re-running `lane-BU-double-ask-red.py`, which now reports **1** double ask
  corpus-wide, all of it board-difference, none of it collapse).
* **Mechanism.** `gptHoldCoversSiblingWindow` requires `heldBoard == nowBoard`, and both are
  `serializeGameState()` (`AIPlayerGPT.cpp:27598`, `:27689`). BT M13 / BX F3 put the CASTING-DECISION
  STATE INSIDE that string: the hand line's per-card verdict is `kHandCastableNow` while the cast
  window is open and `kHandCastableAnswered` once it closes (`:24947-24950`), and `takeHold` records
  the board from INSIDE the cast loop, where `mCastDecisionOpen` is still non-zero. So the sibling
  priority window's board differs from the recorded one on every hand card that was castable —
  measured: 232 prompts carry the answered tag, and 220 of the 242 differing pairs differ on a hand
  or castability line. **Two wave-72 fixes cancel each other**: M13 made the hand line truthful and
  in doing so made M4's board key non-comparable across the seam boundary.
* **BX F1 is a second, independent bar.** Even with an identical board, the subset rule requires
  every acting row of the priority menu to have been printed on the cast menu — and the two menus are
  disjoint by construction (`chooseOrderedAction` drops every hand cast; the cast menu prints no
  activation). The 19 wave-71 candidates were all cycling/activation menus, i.e. exactly the removal
  F1 was written to stop. **F1 is right and M4 is now a no-op**: the collapse can only ever fire on a
  sibling menu whose acting-row set is empty, which is a menu that would not be asked anyway.
* **Verdict.** Not "no qualifying window" and not "not wired" — it is wired to a key that a sibling
  fix moved, and narrowed by F1 to a set that the seam geometry makes empty. The 44-pair /
  ~2.2%-of-windows cost BU measured is still being paid. Fix direction: key the sibling arm on a
  board string that EXCLUDES the casting-decision annotations (the key variant already exists —
  `serializeGameStateImpl`'s `keyVariant` split), and price the F1 subset rule against a menu union
  rather than a subset, or retire M4 and say so.

### `stop_reached_windows_skipped` = 0 — the stop is never PARSED (**HIGH-2**)
Zero skips. The reason is upstream of the collapse: **`repeatRowStopClause` rendered 0 times in the
whole corpus** — 0 prompts contain `your stated stop=`, `M=` or `ALREADY AT OR PAST` (wave 71: 6).
`repeatRowStopClause` returns "" whenever `statedStop < 0`, so `mStatedStop` was never set in 21
games, and the collapse's `carriedStop < 0` guard refuses before F2's dating guard is even consulted.

**The target case DID occur, and the parser missed it.** `1788816992-ai_baka_deck123-…-vs-deck162`,
turn 8 Main 1, Thraben Doomsayer (visible in 379 prompts this corpus, against 134 in wave 71):

```
seq 11  priority  chose "Create human with Thraben Doomsayer, repeated N times, then …"
        PLAN: Tap Thraben Doomsayer 22 times to reach 24 creatures (stop count 24,
              currently at 2, making 22 this window), then attack for lethal next turn.
seq 12  priority  chose "pass"
        PLAN: I have reached the stop (24 creatures vs 20 life + 1 creature). I will
              pass this window, untap next turn, and attack for lethal.
```

`repeatPlanStopAndCurrent` (`:8439`) needs BOTH a `stop`-labelled number and an `m`-labelled number,
each immediately followed (after ` = : (` or ` is `) by a digit. `stop count 24` fails on the
intervening word; `currently at 2` and `making 22` are not an `m` label. So a reply that states its
stop twice, in plain English, in the seat's own voice, on the very card the mechanism was built for,
yields `stop = -1`. **Nothing in the prompt, the protocol or any deck guide teaches the `stop=N; M=N`
form** (`/usr/bin/grep -c "stop=" bin/Res/ai/baka/deck*_strategy.txt` = 0 everywhere) — the only
surface that would is `repeatRowStopClause` itself, which cannot render until a stop has been parsed.
That is a closed loop: the instrument can never bootstrap.
**Not a lost window** (the collapse only ever removes windows), and the wave-71 eleven-window /
1,287 s shape did NOT recur — but M10 is UNTESTED for a reason that will not fix itself. Fix
direction is either a looser own-voice stop scan (a number within N words of "stop", already 2 of 2
in this corpus) or a rendered, stated grammar the pilot can hit; both are prompt/protocol surfaces
and therefore owner-ruled under invariant 000(c) — reported, not proposed.

### `own_turn_windows_skipped` = 718 — correctly wired, report-only
`AIPlayerBaka.cpp:3924-4020`: `ownTurnArmOnly && !searchedForInstantCast`, i.e. an own-turn
instant-speed window whose hand/graveyard/exile/commandzone held no instant, flash or as-flash card,
so `FindCardToPlay` was never called and no model call was made. 718 over 902 turns = 0.80/turn.
Per seat it tracks deck composition exactly: deck152 269 and deck162 171 (the two least
instant-dense seats, and the two with 0 cast-seam hold skips) against deck125 16. The counter is
honest and the class it names is genuinely free. It explains nothing about the decision drop.

### Priority ping-pong, hangs
Consecutive windows identical in (kind, turn, phase, `options_text`) and actually asked: **12 runs,
every one of length exactly 2** (wave 71: 7). No run of 3+; no seat alternating with its opponent.
`menu_pass_no_progress` 0; no `menu pass floor fired` line in any stderr; `W69-BJ F1` 0 (wave 71 0).
21 games, 21 natural ends, largest stderr 304 KB, total 44 MB. **No hang.**

---

## 3. Prediction adjudication

### lane BT
| # | prediction | verdict | evidence |
|---|---|---|---|
| M1 | the outdir holds `askreplay/` reconcilable against `ask_replays_reserved` | **PASS** | 29 files, 521 records = 521 reserved, exactly |
| M2 | 0 noop fires on a subject-led-board-count + `until`/`equips nothing` row; the Supreme-Verdict shape still fires; survivors quote `whose own verdict … reads zero`; no `NO LIVE CAST ROW` over such a row | **PASS, 4/4** | 43 Greaves + 23 Lightmine prospective-zero rows rendered, **0 refused**; 0 of 160 `NO LIVE CAST ROW` headers sit over one; the surviving re-ask (`152v125` s62) carries `whose own verdict on this list reads zero: {right now: does nothing this turn}` |
| M10 | `stop_reached_windows_skipped` non-zero in any game running a repeat engine to a stated stop; the 11-window shape does not recur | **FAIL on the counter / PASS on the shape** | 0 skips (HIGH-2); the 1,287 s shape did not recur (max identical-window run is 2) |
| M13 | 0 prompts contain both `already answered this phase's Casting decision` and `[castable now]`; the new tag count is of the same order as 247 | **PASS, both halves** | 0 of 1,910 (wave 71: **123**); 232 `[no cast row now: you already answered` tags |
| M14 | `own_turn_windows_skipped` non-zero and prices the L4 arm's free windows | **PASS** | 718; §2 |
| M22 | no sidecar streak decreases mid-run; a two-window loop shows `ask_replays_refused` non-zero | **UNTESTED** | max real per-window run (`cache_replay`) is **11** against `kAskReplayRefuseMax` 64 — no run approached the cap, 0 refusals. See MED-2 for the `replay_run` field's second meaning |

### lane BU
| # | prediction | verdict | evidence |
|---|---|---|---|
| M4 | `sibling_window_asks_skipped` non-zero, ~45% of same-window pairs; `hold_windows_skipped` rises by exactly that number; decisions/turn −1% | **FAIL** | 0 skips; HIGH-1 |
| M5 | the three fixtures stay green; the opponent-turn ask share does not rise | **UNTESTED (fixtures) / PASS-as-worded, but see MED-1** | fixtures not re-run; the share FELL 15.0% → **6.8%** (298 → 119 of 1,744) |
| M5 | the {X}-announcement payment loss, docketed | **no recurrence** | `dropped after its X was announced`: **0** occurrences in all 21 stderr files (the carried item; see §3 Carried) |

### lane BV
| # | prediction | verdict | evidence |
|---|---|---|---|
| M6 | the activation spelling renders; priority holds over loyalty rows fall below 4/4 | **PASS on the render, PARTIAL on behaviour** | 351 activation-spelling renders, and `you give up no cast` is now **0** of 1,181 hold rows (wave 71: 376). Priority hold take rate fell 35.6% → 28.5%; no deck126-shaped 4-of-4 recurred (deck126 took 32 holds over 202 windows) |
| M7 | zero token names; the chosen row's remaining-copy count is > 0 in every name choice | **PASS, 6/6** | 6 name menus, all sorted strictly descending by remaining copies (4,4,4,3,3,3,3,2,2,2,2 at `146v126` s17), every pick a real card with count ≥ 2 |
| M8 | no reasoning attributes an opponent permanent's "you" to the reading seat | **UNTESTED** | 651 `written from THEIR seat` frames render (wave 71: 0); the claim is a reasoning-body measurement I did not take |
| M9 | `reserve_decline_windows_noted` > 0 in any game with a reserve row; no STRANDS verdict overridden-and-vindicated | **FAIL on (c) / PASS on (a)+(b)** | `reserve_decline_windows_noted` **0** across 42 gameends although 48 STRANDS rows rendered — see MED-3. (a): 12 STRANDS still print beside an unused land drop, and all 12 are TRUE (the clause reads `leaves 2 sources (your 1 untapped now plus …)`, i.e. the drop IS folded and the cost still does not fit). (b): all 48 open `TAKE THIS ROW AND YOU CANNOT CAST … AT ALL THIS TURN` |
| M12 | no reasoning at a legend-rule window speculates about a prompt bug | **UNTESTED** | 49 legend-rule renders; reasoning-body claim not measured |
| M16 | `to the battlefield under your control.` in full on every Sorin row | **PASS on the rows, FAIL on the board frame** | 0 numbered loyalty ROWS truncate (wave 71 truncated `1. +1:` and `2. -2:` directly); but **92 prompts still print `to the battlefield under...` inside the `Your battlefield` line** — the same clause, the same 140-byte budget, one surface over. MED-4 |
| M17 | zero reveal windows answered with `ATTACK:`/`BLOCKS:`; no duplicate legendary taken while a copy is on the seat's battlefield | **PASS on the first (0 of 17), UNTESTED on the second** | 4 destination-zone lines, 15 duplicate tags, 17 plan-scope facts render; no duplicate-legendary take was traced |
| M18 | the tap-you-out row taken less often than the leave-N row on lethal crack-backs | **UNTESTED** | 171 no-creature crack-back sentences and **65** tap-out clauses render (both 0 in wave 71); the take-rate cross-tab needs a per-row census I did not build |

### lane BW
| # | prediction | verdict | evidence |
|---|---|---|---|
| M3 | every window with a live hand-bottoming trigger renders the VOID line | **PASS, 3/3** | 3 prompts carry a Puzzle Box trigger on the stack; **all 3** carry `EVERY COUNT TAKEN FROM YOUR HAND ON THIS SCREEN IS ABOUT TO BE VOID` (`125v162` s21 the first). Wave 71: 3 stack renders, **0** lines — this is the wave-71 HIGH-3 closed |
| M11 | the life-race clause renders on a repeatable damage row against a rising total | **UNTESTED, guard correct** | 0 renders. 269 prompts carry a rising trend, 4 of them a per-turn damage row — and in all 4 the measured rise (+1.00, +1.25, +0.25, +0.25 per turn) is below the row's 3 damage, so the emitter's own "silent unless the rise is at least as fast as this row's damage" rule is why. No false render |
| M15 | report only | **delivered** | `plan_line_missing` 90 (4.2%) → 73 (3.8%); the `YOUR PLAN:` mislabel is **0** this corpus; attackers is still the worst seam |
| M19 | `LOOP COMPLETE` appears where a seat assembles both halves; `LOOP HALF PENDING` drops to closable cases | **PASS, both** | LOOP COMPLETE **67** (wave 71: 0); LOOP HALF PENDING 131 → **13** |
| M20 | the animation cost bracket renders on every creature-land | **PASS** | 394 `the animation costs` brackets, 10 `it is not a creature as the board stands` tails (both 0 in wave 71) |
| M21 | the plain-decline share falls with no fall in total declines; `hold_windows_skipped` rises | **PASS on the share, FAIL on "no fall"** | 822 scope clauses render. Cast-seam plain decline 459 → **273**, hold 119 → **160**: plain share of all declines 79.4% → **63.0%**, but total declines fell 578/977 (59.2%) → 433/828 (52.3%). `hold_windows_skipped` 1,777 → 1,987. The clause moved pilots off the plain decline as designed; §1 shows what the extra holds cost |
| M23a | the life-tax clause names the currency | **PASS, small N** | 8 renders |
| M23b | the de-coloned head | **PASS** | `Hold priority -` 1,173 renders, `Hold priority:` **0** (wave 71: 1,352); 0 replies used `HOLD PRIORITY:` as a label |
| M23c | the venture-step sentence, docketed | **unchanged** | 23 `DUNGEON ROOM CHOICE` headers, **0** carry `This is venture step` (wave 71: 20 / 0). The docket stands verbatim |

### lane BX
| # | prediction | verdict | evidence |
|---|---|---|---|
| F1 | no sibling skip on a window carrying an unheld row; the counter drops well below 44 | **PASS vacuously** | 0 skips — F1 is doing its job and M4 has nothing left to do (HIGH-1) |
| F2 | every stop-reached collapse has a same-turn own-voice `stop=` | **UNTESTED** | 0 collapses; no stop was ever parsed (HIGH-2) |
| F3 | no `Casting decision` prompt claims the decision is answered; no `[RE-ASK]` cast prompt carries the answered hand tag | **PASS** | 0 of 1,910 prompts whose own question line begins `Casting decision` contain `You have already answered this phase's Casting decision` |
| F4 | token copies keep their card name | **UNTESTED** | 6 name menus, no token-copy-of-a-card on any of them; the wave-71 `Goblin` shape did not recur |
| F5 | no naming prompt contains `SPENT` or `can never be cast again` | **PASS** | 0 and 0; the new `copies not visible in any public zone` wording renders 6/6 |
| F6 | the name-menu fallback commits its own answer | **UNTESTED** | 0 name-menu fallbacks occurred |
| F7 | a mana-less land no longer erases a reserve warning | **PASS, indirect** | all 48 STRANDS clauses that fold a land drop name a real source count; no false suppression found |
| F8 | the conditional crack-back sentence | **PASS** | 3 `unless you first make a blocker` renders inside 171 no-creature crack-backs |
| F9 | LOOP COMPLETE is silent under a lifegain prohibition | **UNTESTED** | 67 renders, **0** on a board carrying such a prohibition — the gate never had a case |

### Carried items
- **The abandoned-{X} payment loss (BU's wave-73 docket).** `dropped after its X was announced`:
  **0** in all 21 stderr files (wave 71's live witness was `130v126` s21-s31). deck130 played its six
  games and the Starstorm shape did not recur, so there are no tapped-out turns to count. Not
  discharged — unexercised.
- **The Puzzle Box line.** CLOSED, 3/3 (BW M3 above).
- **Idyllic Tutor one-window library lag.** **UNTESTED** — Idyllic Tutor appears in deck126's hand
  in the STRANDS renders above; I did not trace library/hand counts across any following window.
- **The venture-step sentence.** Still 0 of 23 (M23c above).

---

## 4. Engine / render / narration defects

### HIGH-1 — the sibling-window collapse is keyed on a string another wave-72 fix moved
Full mechanism in §2. Repro: `1788816988-ai_baka_deck152-0x560c24180dd0-vs-…deck123` seq 24 (cast
ask, HOLD taken) → seq 26 (priority ask, same turn, same phase), whose `--- CURRENT SITUATION ---`
differs from seq 24's only in the hand line's castability verdicts and the blocks that moved with
them. 72 such hold-opened pairs this corpus, 0 collapsed, 0 boards identical. Nothing false is
rendered and no option is removed — the cost is the wasted round trip BU set out to remove, still
being paid at 243 pairs / 1,744 windows (13.9%). Fixing it means keying the arm on the board string
WITHOUT the casting-decision annotations (`serializeGameStateImpl` already computes two variants),
not loosening F1.

### HIGH-2 — a plan that states its stop twice in English yields no stop
Full repro in §2 (`123v162` seq 11/12). `repeatPlanStopAndCurrent` requires two rigid
label-then-digit forms that no prompt, protocol or guide teaches, and the only surface that would
teach them cannot render until one has been parsed. Consequence: `repeatRowStopClause` rendered 0
times over 379 Thraben Doomsayer prompts, M10's collapse is unreachable, and the pilot gets no
read-back of its own stated ceiling at the window where it matters. Report only — the fix touches
protocol/prompt surface (invariant 000(c)).

### HIGH-3 — a creature-land rung menu is ordered LEXICOGRAPHICALLY, and it cost the corpus a fallback
`1788816990-ai_baka_deck152-0x561e5a77dde0-vs-…deck125` seq 62, turn 49 Main 1, 15 rows:
```
  1. becomes a 1/1 hydra …   2. becomes a 10/10 …   3. 11/11   4. 12/12   5. 13/13   6. 14/14
  7. becomes a 2/2 …         8. 3/3  …  14. 9/9     15. Hold priority - …
```
The model answered `CHOICE: 14 (becomes a 14/14 hydra)` — index 14 is the **9/9** row, the NAME is
row 6. `parse_note: echo_index_conflict;name_over_index;index_name_conflict`, and the record is one
of the corpus's two `noop_row_zero_reask` fallbacks. All **5** rung menus in this corpus are
mis-ordered; wave 71 had 16 and only 1 (every wave-71 menu had ≤9 rungs, where string order and
numeric order agree), so this is a **pre-existing latent sort, first exposed by a ≥10-rung board**,
not a wave-72 regression. It is the one place in the corpus where the render's own convention
("usually-correct option first"; declines last) is broken by a string comparator, and the largest
rung — the one the pilot wants — sits sixth. Note the X-announcement menus are NOT affected: they
render descending (`X = 3, 2, 1, 0`) by design.

### MED-1 — the opponent-turn ask share halved, 15.0% → 6.8%
298 of 1,982 → **119 of 1,744**. Per seat: deck123 46.5% → 20.7%, deck146 6.0% → 2.3%,
deck126 5.0% → 0.5%, deck152 0.6% → **0.0%**, deck130 1.1% → 0.7%. Excluding the wave-71 outlier
seat the fall is 147 → 98. Nothing this wave suppresses a window (both new collapses fired 0), and
`own_turn_windows_skipped` is own-turn only, so the most likely reading is BU's own M5 answer — an
opponent-turn window the seat cannot pay for is a mana fact — combined with the higher cast-seam
hold rate (a hold taken at the cast seam suppresses that seat's LATER casting windows, including
its opponent-turn response windows: `hold_windows_skipped_cast` 774 → 1,421). I could not separate
those two from the records; a per-seat `hold_windows_skipped_cast`-vs-opponent-turn-window census on
the next corpus would.

### MED-2 — `replay_run` in the sidecar means two different things
`logAskReplay("cache_replay", …, mAskReplayRuns[askKey])` passes THIS WINDOW's run;
`logAskReplay("repeat_ask_reserved", …, mRepeatAskAnswersReserved)` passes the GAME's running total
(`AIPlayerGPT.cpp:36296`, `:36317`). The maximum `replay_run` in the corpus is **90**
(`123v125` seq 184, `why: repeat_ask_reserved`) against `kAskReplayRefuseMax` 64 — which reads as a
breached cap and is not one; the true maximum per-window run is **11**. Worse, the two paths are not
equally bounded: `askReplayRefuseScoped` guards only the `mAskCache` path, while
`repeatAskAnswerStands` — now **327 of 521 replays (63%)**, up from 68 of 725 — is bounded only by
the turn boundary. The wave-71 drain loop's successor lives on the unbounded path and the
instrument that would show it is mislabelled.

### MED-3 — `reserve_decline_windows_noted` is 0 over 48 reserve rows
BV M9(c) shipped a TURN-scoped memory of a declined reserve row that re-serves the window with a
prompt-only note, counted on the gameend census. 48 STRANDS rows rendered across 5 seats and the
counter is 0 on all 42 gameends, and no prompt contains `reserve row declined earlier THIS TURN`.
Either no reserve row was declined and later re-offered in the same turn (possible: the cast-set key
plus turn is narrow), or the note is not wired to the path that renders. I did not read the
`reserveDeclineHonoured` call sites to separate those — that is the check the next lane owes.

### MED-4 — the loyalty clause is still truncated, one surface over
BV M16 gave every loyalty-lead clause the focus allowance at the ROW; **92 prompts still print
`to the battlefield under...`** inside the `Your battlefield (…)` permanent line
(`126v130` seq 12 is the first). The board frame budgets its snippets separately, so Sorin's `-6`
clause — the one that says WHOSE battlefield the reanimated creatures arrive on — is whole on the
menu and cut in the board description on the same screen. Two surfaces, one clause, different
verdicts; wave 71 had 119 of these plus the row truncations, so the fix removed the rows and left
the frame.

### LOW-1 — `mana_only_windows_skipped` fell 270 → 24, entirely inside deck126
deck126 alone was 270 of wave 71's 270 and is 24 of this wave's 24; every other seat is 0 in both.
Its decisions rose 157 → 219 over 101 → 117 turns. Not a defect on this evidence, but the counter is
a single-deck instrument and should be read as one.

### LOW-2 — `async_drops` 17 → 32
All 32 sit on 25 records (`async_drop_events` present on 25). No fallback and no `answer_replaced`
followed any of them, so the drop-and-re-ask path is self-healing as designed; the near-doubling
against a smaller corpus is worth a glance next wave, not an action now.

---

## 5. Lategame specimen

`wave72/lategame-specimen.txt` —
`1788816990-ai_baka_deck152-0x561e5a77dde0-vs-ai_baka_deck125.jsonl` **seq 68, turn 53**, phase
Upkeep, 11 vs 63 life, 17 options, 36.1 s, 6,046 reasoning chars, prompt 37,917 B. The seat took
option 8, `becomes a 16/16 hydra with Lair of the Hydra`. Chosen because it is the deepest late-turn
decision in the corpus AND it carries HIGH-3's mis-ordered rung menu, the M6 activation hold row, the
M20 animation-cost brackets and the M21 decline-scope clause on one screen. Full `prompt` verbatim
under a 3-line header. Alert-only per invariant 0: **the composed prompt SHAPE changed this wave** —
the hand line carries a new `[no cast row now: you already answered …]` verdict, the hold row is
de-coloned and has a third spelling, and four new clause families (`closes ONLY this window`,
`the animation costs`, `written from THEIR seat`, `LOOP COMPLETE`) are now present.
