# Wave-59 engine-narration seat

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-130044/` (42 seat JSONL, 21
stderr, `results.tsv`). Binary = master `5039427da`. Counted from the rendered `prompt` with
python; card facts checked against `bin/Res/sets/primitives/` and Scryfall.

**Method note that changed a verdict:** the translog's `phase` FIELD is not the rendered phase.
2,363 records carry a `Phase: ... | It is ... turn.` line; one disagrees with its own `phase`
field (130v162 seq 21: field `Main phase 1`, prompt `Phase: Draw`). Counting K8 from the field
produced one false FAIL. Every phase/turn-owner count below is read off the prompt.

---

## 1. Corpus census

| | wave 59 | wave 58 (for reference) |
|---|---|---|
| games natural / hung / crashed | **21 / 0 / 0** (21 `WAGIC_SELFPLAY_RESULT` lines) | 21 / ? / ? |
| decision records (with a rendered `prompt`) | **3,005** | 2,270 |
| records total | 3,098 (`ask` 2,195, `priority` 602, `attackers` 95, `discard` 66, `gamestart`/`gameend` 42+42, `blockers` 26, `reveal` 13, `recovery` 9, `bottom` 7, `wall_miss` 1) | — |
| **fallbacks (heuristic handoffs)** | **12 / 3,005 (0.4%)** | 108 / 2,270 (4.8%) |
| — by kind | `named_row_reask` 6, `unparsed_reply` 4, `wall_miss_unrecorded` 1, `no_pass_reask` 1 | incl. `empty_reply` 89 |
| — of which recovered by the next window | 9 of 12 (`recovery` records) | — |
| `empty_reply` | **0** | 89 |
| stale async drops (`async_drops`) | **83** across 19 seat files (stderr: 56 land-drop arm, 28 casting arm) | — |
| transport failures (new `transport` field) | **2**, both `curl=28,http=0,empty=1` | (invisible in wave 58) |
| `transport_error` classifications | **0** | — |
| transport retries (`retry`) | 1 (recovered the decision) | — |
| timeouts / wall misses | 0 `timeout` class; `wall_miss_events` **2**, `wall_miss_unrecorded` **1** | — |
| HOLD rows rendered / taken (prefix-match) | **2,140 / 560 (26%)**; `hold_windows_skipped` 2,999 | — |
| `identical_ask_answers_reserved` (new, lane J) | **161** (max 44 in one seat) | field did not exist |
| `identical_option_asks_resolved` | 3 | 0 |
| `mana_only_windows_skipped` | 635 | — |
| SEGV / `Aborted` / `Assertion` / `buildMenuChoice` in stderr | **0 / 0 / 0 / 0** | 1 abort class (F1) |
| translog gap past the watchdog / parked game | **0** | 1 (F2) |
| latency | median 15.4 s, mean 29.0 s, p95 87.5 s; 348 > 60 s, 79 > 120 s, **4 > 600 s** | — |

---

## 2. Prediction adjudication

### Lane I — K4 / K5 / K7 (`wave59/lane-I.md`)

**K4 — PASS.** 106 real `{dead right now: ...}` badges (excluding the legend sentence, which
carries the literal `{dead right now: ...}` in every prompt — 163 raw hits, 57 of them legend).
All 106 are one of six texts and **0 of 106 name a live magnitude in any scope**; every scope
clause reads `0 legal targets` / `destroys 0` / `exiles 0` / `at 0 this does nothing` /
`targeting yourself does nothing`. The badge is confined to `cleanup_discard` windows
(106 of 106) as designed. The lane-break negative also holds: the 36 `{right now: ...}` verdicts
in `cleanup_discard` windows are **0** all-zero — no badge vanished off a dead verdict. Tragic
Slip's Morbid qualifier now renders live 71 times as
`{right now: -1/-1 (no creature has died this turn, so Morbid does NOT apply)}`, and Devour
Flesh's two-scope verdict renders under `{right now: ...}` 108 times and under
`{dead right now: ...}` only on a genuinely empty board (4 renders).

**K5 — PASS (one arm UNTESTED).** 117 `INCOMING THIS COMBAT` lines.
`... or better`: **0 of 117**. `AT BEST`: 2 renders, both correct in form
(`162v152` seq 17 turn 13, `126v152` seq 24). The exact branch's
`; no block saves you`: 2 renders. **UNTESTED:** the arm "every rendered `AT BEST` with `N <= 0`
carries `; no block saves you`" had **no window** — both `AT BEST` ceilings are positive (1 and
12), so the proven-death branch of the non-exact form never fired. Trample cross-reference:
`each A-line above says how much` renders 8 times, **0 of them on a rung that also mentions a
trampler**; the one trample rung in the corpus (`162v152` seq 17, `B1. Fog Bank (0/2)`) carries
the new pointer `this row's own "may block" note`.

**K7 — PASS.** 28 MDFC land-face menu rows, **0 bare `Play Land`** (wave 58: 32 renders,
13 blind takes). Every row is
`<Face> (menu text: Play Land) [PLAY THIS AS A LAND: puts "<Face>" onto the battlefield as a land
(taps for {X}). ... USES YOUR LAND DROP for this turn.]`, byte-identical in shape to its
sibling back-face row; taken 24 of 28. Face facts verified: `152v123` seq 3 renders
`1. Mistgate Pathway ... {U}` / `2. Hengegate Pathway ... {W}`, which matches the primitive
(`borderline.txt:52388-52393`) **and** Scryfall (Hengegate `{T}: Add {W}`, Mistgate
`{T}: Add {U}`); Branchloft `{G}` likewise.

### Lane J — K8 / K9 / K10 (`wave59/lane-J.md`)

**K8 — PASS (both arms).** 406 `CRACK-BACK NEXT TURN` renders, **all** on `YOUR turn` and only in
Main phase 1 (329), Main phase 2 (47) and Attackers (30).
*Positive:* of 1,863 own-turn Main/Attackers windows, **0** lack the line while the opponent
controls a creature that will untap with power. Of the 235 absences with ≥1 opposing creature,
226 have total opposing power 0 and 9 are bodies tagged
`[doesn't untap during its controller's untap step]` (`123v130` seq 18-22, 36-40) — both
correctly silent under `crackBackBodyUntaps`.
*Negative:* **0** renders on the opponent's turn; **0** in untap/upkeep/draw/blockers/combat/
end/cleanup; **0** where the opponent controls 0 creatures; **0** prompts carry both this line
and `INCOMING THIS COMBAT`. 42 renders carry `; that would KILL you`
(e.g. `162v152` seq 15 turn 12: `5 ... for up to 18 - you would be at -10`).

**K9 — PASS.** 86 `PUT:` reply lines; **14 carry a trailing name gloss and 0 are
`unparsed_reply`**, including the multi-name form the lane was built for —
`125v126` seq 385: `PUT: 9, 10, 1 (Sphinx's Revelation, Sphinx's Revelation, Essence Scatter)`.
The degenerate-output half also holds: the 2 remaining unparsed `ask` replies
(`125v130` seq 113, `130v125` seq 156) are decode garbage. The other 2 unparsed replies are a
DIFFERENT shape the lane did not address — see HIGH-2.

**K10 — MIXED: one arm FAIL, two arms UNTESTED, mechanism partly working.**
The latch fires: `identical_ask_answers_reserved` totals **161** across 42 seats (wave 58: the
field did not exist; `identical_option_asks_resolved` still reads 3 corpus-wide, confirming the
lane's reading that it was the wrong counter). The longest single-turn ask run fell from wave
58's 32 to **22** (`123v162` turn 10).
*FAIL:* the stated arm "the maximum `[you declined this exact list N times already this turn]`
in that game falls below 5" is refuted — 23 (`125v126` turn 31, a turn with only **9** asks),
18 (`126v123` turn 27), 18 (`126v125` turn 30), 15, 13, 12, 11, 11, 10 x4. See MED-1: the
counter's meaning changed under the fix, so the number is no longer a run-length.
*UNTESTED:* "`identical_ask_answers_reserved` ≥ (run length − 1)" and "the ask-count drop is only
in windows where the rows did NOT move" cannot be adjudicated from a corpus — a re-serve writes
no translog record by design (the lane says so), so the absorbed windows are invisible.

### Lane H — K1 / F3 (`wave59/lane-H.md`)

**P1 — PASS.** `empty_reply` records: **0 of 3,005** (wave 58: 89). 93 records land in the
2,400-2,700 ms band and **none** is a fallback of any kind — the 2.5 s cluster is gone as a
failure fingerprint.

**P2 — PASS.** The one true non-answer record (`130v162` seq 21) carries
`transport: curl=28,http=0,empty=1`. No decision is filed as a transport fault with no cause.

**P3 — PASS on the headline, FAIL on the sub-clause.** Heuristic handoffs fell to **12 / 3,005
(0.4%)** from 108 / 2,270 — below the predicted ~19-record parse floor, and 9 of the 12 were
recovered by the next window. But "the surviving transport failures appear under
`transport_error`": **0 records carry `transport_error`**. Both transport failures burned the
whole deadline instead, and one was classed `wall_miss_unrecorded`. The new class never fired.

**P4 — PASS (1 of 1).** The wall miss reads `wall_miss` / `wall_miss_unrecorded`, never
`transport_error`. See HIGH-3 for the cost this fix moved rather than removed.

### Lane K — K6 (`wave59/lane-K.md`)

**P1 — UNTESTED.** No repeated-payment window arose: 3 `Choose an option for Intrepid Adversary`
asks corpus-wide (`152v162` seq 6 `don't add any counter`; `152v130` seq 17 `don't add any
counter`, seq 29 `add 1 counter`), and **0** Valor counters ever appeared on a rendered board.
The wave-58 repro was a rerun prediction and this corpus is a fresh run.

**P2 — PASS, with a positive confirmation.** 166 `Mana available: 0 total (no untapped sources)`
lines; **0** sit on a board holding an untapped, non-summoning-sick granted producer. The
converse is directly observed at `152v126` seq 28-31 (turn 13): Katilda is
`[tapped - cannot attack or block this turn]` and the line reads
`Mana available: 1 total ... Those sources, one per untapped card: Luminarch Aspirant {w}` —
exactly the shape that read `0 total` in wave 58. The six Katilda-tapped-looking zeros at
`152v126` seq 18/34 are correct on inspection (seq 18's Humans are summoning sick; seq 34 is the
opponent's turn and Tovolar's Huntmaster is still sick).

**P3 — PASS (3 of 3).** Every payment that spent a granted producer names the GRANTEE as the
tapped source: `- Paid {2}{w} for Brutal Cathar with Luminarch Aspirant; Lair of the Hydra;
Mistgate Pathway` (seq 18), `- Paid {1}{g}{w}{w} for Sigarda ... Brutal Cathar ...` (seq 23),
`- Paid {4}{g}{g} for Tovolar's Huntmaster with ... Brutal Cathar; Mistgate Pathway; Katilda,
Dawnhart Prime` (seq 28, names both, and Katilda is legitimately one of the sources). **0**
granter-taps-instead-of-grantee shapes.

**P4 — PASS.** 21/21 natural, 0 crash, 0 hang, 12/3,005 fallbacks; no fallback or defer spike
attributable to mana-source counting.

### Wave-58 lane F — F1 (`wave58/lane-F.md`)

**P1 — PASS.** 0 `Segmentation`, 0 `Aborted`, 0 `Assertion`, 0 `buildMenuChoice` across all 21
stderr logs; all 21 games reached `WAGIC_SELFPLAY_RESULT`. Dungeon/venture decks (146, 152) are
present in 12 of the 21 games, including the Lost Mine / Acererak lines
(`Dungeon of the Mad Mage` renders in stderr).
**P2 — UNTESTED.** 0 `ActionLayer: menu row N re-pointed` lines. A silently skipped row leaves no
receipt, so I cannot tell "the branch never fired" from "it fired quietly"; no window is
observable either way.
**P3 — PASS.** 0 games ended in a timeout or a hang; 2 wall misses in 3,005 decisions, 1 of them
recovered by the next window.

### Wave-58 lane G — F2 (`wave58/lane-G.md`)

**P1 — PASS.** 0 hung games, 0 translog gaps past the watchdog, 0 seats whose last activity is a
reveal.
**P2 — UNTESTED (not FAIL).** 0 `reveal_stall_forced` records and 0 `FORCE-CLOSING` lines. All
**13** reveals in the corpus completed by taking option ONE
(`AIPlayerGPT: reveal put N of N to option one in one reply`, 13 of 13) — the DECLINE that is
F2's precondition never happened, so the lane's exit was never exercised. Its literal
"count >= 1" is unmet because no window arose, which the lane itself distinguishes from a
refutation.
**P3 — UNTESTED.** 0 `(driver phase 6)` lines, for the same reason (the lane says absence over a
full corpus means the shape was never hit).
Observation not covered by a prediction: reveals are expensive. `reveal_wait_secs` runs 6-81 s
across the 13 reveals (`152v126` seq 13: 81 s / 29,202 driver ticks for a
`Katilda, Dawnhart Prime, Overgrown Farmland` reveal).

---

## 3. Engine / render / narration defects

### HIGH-1 — a `THIS WINS THE GAME` badge on a window where the seat is already dead on the stack; TARGET CHOICE renders no stack

**Repro:** `1788544889-ai_baka_deck130-0x55dcc18cad60-vs-ai_baka_deck146.jsonl` **seq 44**,
turn 26, Main phase 2, life 1 vs 2. The seat has just cast Hammer of Bogardan while the
opponent's Silverquill Silencer has named it. The rendered line:

```
1. The opponent (player, life 2) {right now: takes 3 damage - they would be at -1; THIS WINS THE GAME}
```

The Silencer's on-cast trigger (`they lose 3 life`) is **already on the stack above Hammer** and
resolves first: the seat at 1 life dies at -2 before Hammer ever resolves. The prompt renders
`Your life: 1`, has **no `ON THE STACK` section at all**, and the game log's last line is
`- Paid {1}{r}{r} for Hammer of Bogardan with Mountain #6; Forgotten Cave; Mountain #7` — the
trigger is not in the log either. The model's own plan on that window reads *"I win before they
can respond or act further"*; it took row 1 and the `gameend` record two seq later is
`life -2 / opp 2, lost`.

**Card data is innocent.** Silverquill Silencer's primitive
(`borderline.txt:102764`, `auto=chooseanameopp transforms((,newability[@movedto(*[chosenname]|
opponentstack):life:-3 opponent && draw:1 controller]))`) matches Scryfall clause for clause;
the cast row one window earlier (seq 43) states the price correctly
(`this costs you 3 life ... you would be at -2; this KILLS you`). The lie is that the TARGET
window then asserts a win with no reference to the death it just priced.

**Scope:** 4 `THIS WINS THE GAME` renders corpus-wide; **1 was false, and it was the one that
decided a game**. Structurally: **11 of 89** TARGET CHOICE windows render an `ON THE STACK`
section, while the window's own text says *"this spell/ability is already on the stack"*. This
is the lategame specimen (`wave59/lategame-specimen.txt`).

### HIGH-2 — a cleanup `PUT:` list with a repeated index is rejected whole, with no receipt and no re-ask

**Repro A:** `1788544892-ai_baka_deck125-0x556d17349150-vs-ai_baka_deck126.jsonl` **seq 246**,
turn 54, `discard`. Prompt: *"you must discard exactly 16 cards now. Name EXACTLY 16 card
numbers - this is the ONLY ask for them"*. Reply:
`PUT: 8, 11, 22, 19, 20, 16, 21, 23, 12, 10, 4, 5, 13, 17, 18, 19` — 16 numbers, but `19` twice.
Scored `unparsed_reply`, `parse_note` **null**, heuristic picked the 16 discards.
**Repro B:** same file **seq 354**, turn 78: `must discard exactly 2` -> `PUT: 4, 4` -> same.

Three separable faults: (a) the parser rejects the entire list rather than reporting the
duplicate or dropping it; (b) the ask never says the numbers must be DISTINCT — "Name EXACTLY 16
card numbers" admits the reading the model used; (c) unlike the `ask` channel, a cleanup discard
has **no recovery window** (the prompt says so itself), so both losses are permanent — 7 of the
9 `recovery` records cover `ask`, none covers `discard`. 2 of 66 discard windows, both in the
86-turn game, both while the seat was ahead. Lane J's K9 fixed the gloss shape; this is the
neighbouring shape it did not.

### HIGH-3 — removing the 2.5 s connect cap moved the cost rather than removing it: a connect failure now burns the whole decision deadline

`gptConnectTimeoutMs(timeoutMs)` makes connection setup as generous as the decision, so
`curl=28` now costs the full wall instead of 2.5 s. Both occurrences:

* `1788544895-ai_baka_deck130-0x5630b76e6980-vs-ai_baka_deck162.jsonl` seq 21, turn 10:
  `latency_ms 900020`, `transport curl=28,http=0,empty=1`, `fallback wall_miss_unrecorded`,
  `chosen_text "<refused: wall_miss_unrecorded>"`. **The decision was still lost** — 900 s spent
  to reach the same heuristic handoff the 2.5 s cap reached in 2.5 s.
* `1788544861-ai_baka_deck123-0x55d879872be0-vs-ai_baka_deck146.jsonl` seq 7:
  `latency_ms 977594`, same transport stamp, `retry 1`, **`deadline_pct 108.6`**. The retry
  recovered the answer but finished **8.6% past the deadline it was supposed to fit inside** —
  a direct falsification of lane H's stated guarantee that `remainingTransportRetryMs` bounds
  attempt + retry to one deadline. Worth checking whether the remainder is computed from the
  first attempt's *start* or its *completion*.

~30 minutes of corpus wall time went to these 2 decisions of 3,005. The `transport_error`
classification the lane added fired **0** times, because a connect stall now ends as a wall miss
before the classifier sees it. The right shape is probably a connect budget that is generous but
**bounded well below** the decision deadline (leaving room for the retry to land inside it).

### MED-1 — the decline counter now counts declines the model never saw

`[you declined this exact list N times already this turn]` is incremented by the K10 latch's
re-serves as well as by real asks. `125v126` turn 31 renders `... 23 times already this turn`
in a turn with **9** asks; `126v123` turn 27 renders 18 in a turn with 7. The sentence's subject
is now the engine's latch rather than the model, and the number is inflated by exactly the
mechanism that was meant to reduce the repetition it reports. Either the tag should count asks
the model actually saw, or it should say what it now means. (This is also what breaks lane J's
own prediction arm — the counter it predicted against changed units.)

### MED-2 — the `wall_miss` record loses its window, and its `phase` field is wrong

`130v162` seq 21 (the only `wall_miss` kind in the corpus): `options` is `0`-length,
`options_text` is `null`, `chosen_text` is `"<refused: wall_miss_unrecorded>"`, and `phase` reads
`Main phase 1` while the prompt reads `Phase: Draw | It is your turn.` — the **only** disagreement
in 2,363 records carrying both, sitting on the one record a reviewer most needs. A wave-58 MED
(deck146 MED-2) that recurs; the phase half is new. The prompt IS stored, so this is bookkeeping,
not data loss — but every automated census keyed on `phase` mis-files this record.

### MED-3 — TARGET CHOICE windows do not show the stack (generalization of HIGH-1)

11 of 89 TARGET CHOICE windows render `ON THE STACK` (543 of the other 2,916 windows do). The
window's own text tells the model the object is on the stack and then shows it nothing else that
is — including triggers that will resolve first and change whether the target is worth picking.

### LOW-1 — the `AT BEST` life ceiling can print above the number it follows

`126v152` seq 24: `... 10 unblocked damage - you would be at 8 ... at least 6 of that lands
...: you would be at 12 AT BEST`. Both numbers are true (8 = no blocks, 12 = best blocks) but
neither is labelled with its assumption, so the line reads as self-contradictory until the
parenthetical is parsed. 2 renders corpus-wide, so this is a wording note, not a defect that bit.

### LOW-2 — HOLD row economics

2,140 HOLD rows rendered (a row on ~71% of decision prompts), 560 taken (26%), 2,999
`hold_windows_skipped` across 42 seats. Working as intended; recorded so the token cost of the
row is on the record next to the windows it saves.

---

## 4. What I did NOT check

* **No build, no run, no git.** Read-only over the corpus, the primitives, `AIPlayerGPT.cpp`
  (`crackBackBodyUntaps` / `boardCreatureCanAttackNow` call sites, `deadline_pct`) and two
  Scryfall lookups. I did not compile, did not run PARSETEST or the suite, and did not verify
  any lane's gate numbers.
* **No wave-58 corpus re-read.** Every "wave 58: N" figure above is quoted from
  `wave59/known-bugs.md` and the lane files, not re-derived.
* **K10's absorbed windows are unobservable** and I did not instrument for them; the 161
  re-serves are taken from `gameend` on faith. Whether any re-serve was WRONG (the board moved
  in a way the row text does not show) cannot be answered from this corpus — that needs a
  translog record per re-serve, which the lane deliberately does not write.
* **F1's re-point branch and F2's decline path never arose.** I could not distinguish "fixed"
  from "not exercised" for either; both are recorded UNTESTED rather than PASS.
* **I did not classify the 83 async drops** beyond their stderr arm labels, did not check
  whether any drop cost a decision, and did not look for a pattern across the 19 seats.
* **HIGH-1's engine site is unlocated.** I established the shape and the rules basis, not which
  emitter owns the `THIS WINS THE GAME` badge or why TARGET CHOICE omits the stack section.
* **I did not read any deck guide** and made no strategy judgements; the model's choices appear
  above only as evidence about what a surface told it.
* **The 4 decisions over 600 s and the 79 over 120 s** were not investigated beyond the 2 with a
  `transport` stamp; whether the rest are model-side or transport-side is unknown.
* **Only two cards were verified against Scryfall** (the pathways and Silverquill Silencer).
  Every other card fact quoted here is from the primitive only.
