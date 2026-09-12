# Wave-78 engine-narration seat

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-125420-final` (single binary
`archives/wagic-47ef378c8-w78step1`, source `7c0f15835`, `--thinking on`, one run, 21 concurrent).
Method: python over the 42 seat logs + `game-*.stderr`; `/usr/bin/grep` for source reads.
No git, no wagic, nothing written outside `wave78/engine-seat.md` and `wave78/lategame-specimen.txt`.
**Mechanisms below are this seat's readings, not the owner's positions.**

---

## 1. Corpus census

| | wave 78 | wave 77 |
|---|---|---|
| games / seat logs | 21 / 42, all with `gameend`, 0 HUNG, 0 timeout-tainted | 21 / 42 |
| record-kind sum | 2,670 = ask 1,906 + priority 532 + attackers 93 + discard 52 + forced_close 32 + blockers 22 + reveal 18 + recovery 8 + bottom 7 | — |
| `protocol_replies` | 2,629 | 2,740 |
| decisions / turn | **1.88** | 2.02 |
| native reasoning | 1,906/1,906 ask records, `thinking` = `on` on every one | same |
| `reasoning_chars` | median 5,966, p95 10,537, max 17,847, min 588 | median 5,871 |
| `reasoning_degenerate` | max 0.0145 (median 0.0030) — no incident | — |
| `reasoning_ngram_repeat` | median 0.126, p95 0.261, max 0.5145; 22 records > 0.35, 1 > 0.5 — no incident | — |
| `reply_truncated` | **0** | — |
| `action_before_plan` | **0** | — |
| latency | p50 78.3 s, p95 182.4 s, max 393.9 s | p50 78.3 |
| `deadline_pct` | p50 8.6, p95 20.2, **max 43.7** — never within 2x of the 900 s wall | — |
| `transport` | `curl=0,http=200,empty=0` on **all 2,629** — no retry, no empty body | — |
| `max_tokens` | (6288/288/6000) x2,438, (6256/256/6000) x184, (6512/512/6000) x7 | — |
| off_protocol_bytes>0 | 96 corpus-wide (3.7%) / 76 ask-seam | 4.4% |
| `plan_line_missing` | 103 (3.9%) | 5.1% |
| `protocol_deviation` | compliant 2,520 / `unlabelled_plan` 90 / `plan_absent` 13 / `answer_label_absent` 6 | — |
| `plan_steps_done` | sum 1,413 against `plan_step_count` sum 4,558 = **0.31**; distribution 1 x1,409, 0 x1,218, 2 x2 | — |
| `async_drops` | 46 | 53 |
| `askreplay/` | 30 files | — |
| fallbacks | **8** (7 ask `unparsed_reply` + 1 reveal `engine_answered`), each with a paired `recovery` record | — |

Reply shape: every parsed reply is a PLAN line + an answer line; `post_answer_overrun` 0 corpus-wide.

**Census-tool item (CY F12 FAIL).** `corpus-stats.py` prints
`CENSUS CHECK MISMATCH: the listed kinds sum to 40 but the difference is 41`. The 41st is the
`reveal`/`engine_answered` record (`152v123` seq 22, `latency_ms -1`): `reveal` is inside
`ROUNDTRIP_KINDS` but an engine-answered reveal makes no round trip. Fix = subtract records whose
`fallback == "engine_answered"` from the round-trip side, not move the whole `reveal` kind.

---

## 2. Regression watch on the wave-78 changes

### (a) FALLBACKS — all 8, verbatim

#### The six `answer_label_absent` (S1, all deck125 seats) — the owner's evidence

Every one: a labelled `PLAN:` line, then `<n> (<short name>)` with **no `CHOICE:` label**.
All six are **in range** and **byte-exact** against the row's short name. All six games were WON by
deck125 (125v123 62 turns, 125v126 85 turns, 125v130 68 turns). `answer_label_absent_heuristic_played`
= 6 and reconciles exactly with the six records (CY **F10 PASS**).

| # | seat / seq / turn / phase | raw reply bytes | opts | the row the answer names | heuristic played | cost |
|---|---|---|---|---|---|---|
| 1 | 125v123 s83 T23 Main 1 | `"\n\nPLAN: Cast Sphinx's Revelation for X=4.\n2 (Cast Sphinx's Revelation)"` | 4 | row 2 `Cast Sphinx's Revelation {u}{u}{w}{x} {X pricing: max affordable X=4 …}` | `cast nothing` (choice 0) | **1 wasted round trip, 90.5 s.** Window re-put at s85; the model cast Sphinx's Revelation X=4 anyway (s86-87). Board cost 0. |
| 2 | 125v123 s140 T33 Main 1 | `"\n\nPLAN: Play a land and hold for Emrakul next turn.\n3 (Cast nothing right now)"` | 3 | row 3 `Cast nothing right now {closes ONLY this window …}` | `cast nothing` | **0** — heuristic played the model's own answer. 55.7 s re-decode. |
| 3 | 125v123 s215 T39 Main 2 | `"\n\nPLAN: I will hold priority to skip further windows this turn, as the opponent has no threats and I have no plays to make right now.\n2 (Hold priority)"` | 3 | row 2 `Hold priority - pass now, and do not ask me again …` | `cast nothing` | **the hold was not taken.** Re-put at s217; the model then chose `Cast nothing right now` and the window-run was never closed. 53.6 s + the re-put. |
| 4 | 125v123 s305 T55 Main 2 | `"\n\nPLAN: I will hold priority to end my turn and proceed to the next turn to attack with the two Staffs of Nin, which will win the game in 4 turns.\n4 (Cast nothing right now)"` | 4 | row 4 `Cast nothing right now {the clock you already control: your Staff of Nin #1-#2 …}` | `cast nothing` | **0** board cost; re-put at s307 where the model took the hold. 64.6 s. |
| 5 | 125v126 s107 T21 Main 1 | `"\n\nPLAN: Pass main phase and hold priority.\n4 (Cast nothing right now)"` | 4 | row 4 `Cast nothing right now (combat comes next this turn) …` | `cast nothing` | **0** board cost; re-put at s109 where the model then CAST Supreme Verdict (sweeping 3 of their creatures). 96.6 s. |
| 6 | 125v130 s303 T55 **Upkeep** | `"\n\nPLAN: Pass upkeep, cast Staff of Nin in main phase 1.\n2 (Hold priority)"` | 3 | row 2 `Hold priority - pass now …` | **`cast Sphinx's Revelation`** (choice 1) — the heuristic cast the row the model had just declined, paying 12 mana with every land | **REAL COST.** At s305 the model was handed the announce window and answered `Decline - do not cast this after all {declining strands the 12 mana already paid …}` — the whole turn's mana stranded in the seat's own upkeep. Game still won. |

So: **4 of 6 cost only a re-ask (55-97 s each); 1 lost the hold it asked for; 1 (125v130 s303) made
a 12-mana play the model had explicitly declined and the model then had to un-make it, stranding the
mana.** No game turned on any of them. The parser is unchanged by ruling; no tolerance is proposed here.

#### The 7th `unparsed_reply` — `162v146` seq 16 (T10, Main 1, deck162, `unlabelled_plan`)

```
'\n\nCast nothing right now.\n3 (Cast nothing right now)'
```
`plan_line_missing: true`, `off_protocol_bytes: 49`, options 3
(`Cast Howling Mine {2} [second copy: …]` / `Hold priority …` / `Cast nothing right now (combat comes
next this turn) …`). **Which pin:** BOTH — no `PLAN:` label on line 1 and no `CHOICE:` label on line
2; the final class is `unlabelled_plan` because the plan pin is classified first. The heuristic played
`cast nothing`, which **is** the row the model named (option 3) — cost 0 board, one re-put at s18 where
the model took the hold. deck162 lost that game (life 5 vs 20 at T10) for reasons unrelated to this.

#### The `reveal`/`engine_answered` — `152v123` seq 22 (T9, Blockers, deck152)

`latency_ms -1`, `options 5`, `reply ""`, `chosen_text "none (no legal target)"`. The `recovery`
record (seq 23) carries the narration: the seat revealed 5 cards with Sigarda's coven trigger
(`Barkchannel Pathway x2, Ranger Class, Lair of the Hydra, Fateful Absence`) and
**`You revealed 5 cards but none was a legal target - took none`**. A reveal whose menu has zero legal
targets is not a decision, so no round trip was made. Genuine; not a model failure. It is the off-by-one
in the census tool.

### (b) S3 — the stop-reached re-put collapse fired **ZERO** times. **FAIL, and the shape recurred worse**

`stop_reached_windows_skipped` 44 (all via the wave-72 arm; the stderr line
`every live row is a repeat family already at the model's own stated stop` appears exactly 44 times).
`stop_reached_reputs_collapsed` **0** — the new arm never fired once.

Separately, **44 windows carrying `ALREADY AT OR PAST your own stop` were ASKED**, all on deck123
seats, in four re-put chains, for **5,448 s = 90.8 minutes of decode** that produced **3** non-pass
answers out of 44:

| chain | windows | seqs | decode | answers |
|---|---|---|---|---|
| 123v152 T9-10 | 13 | 16-26, 28, 40 | 1,836 s | 12 pass, 1 take (s25) |
| 123v130 T13-15 | 14 | 87-99, 103, 120 | 983 s | 12 pass, 1 transform (s90), 1 hold (s99) |
| 123v146 T9-10 | 10 | 17-22, 25, 38, 41, 42 | 1,501 s | 9 pass, 1 take (s22) |
| 123v162 T10 | 7 | 36-41, 58 | 1,128 s | 7 pass |

The wave-77 S3 shape (12 re-puts, seqs 41-52) **recurred** — 123v152 seqs 16-25 is a *ten*-window
version, 123v130 T13-15 a fourteen-window version.

**Which identity leg moved.** `declinedN` was >= 1 on every re-put (the prompt's
`[you declined this exact list N time(s) already this turn]` line reads 1,2,3,4,5,…,9), and the acting
rows were byte-stable, so the gate that refused is `w78RePutIdentityStands`. Two legs move
independently and either alone is fatal:

* **(b) the board key.** 123v162 seqs 36→41: the opponent's four Underworld Dreams triggers drained
  one link at a time and took the seat's life 20 → 19 → 18 → 17 across consecutive re-puts with
  identical rows. `declineBoardScope(serializeGameState())` changes on every link, so the identity
  never stands. This is exactly CY F1's own §Weakest item 1 ("a moving `M=31 → M=34` … unproven on
  live boards"), realised.
* **(c) the stop statement.** The identity carries `stopTurn` and `stopCountAtStatement`, and the
  model re-states its PLAN at essentially every window (the prompts show
  `THE PLAN YOU LAST STATED (as you stated it, 1 window ago …)` changing text between adjacent
  re-puts), so the stop is re-dated and re-counted under the collapse.

Not verifiable from the translog: which of the two moved first at any given window (no trace carries
the identity string). What IS verifiable is that neither ever held: 0/44.

### (c) S4 — the drain clause. **PASS on truth; CY F2 PASS**

`stack_drain_windows_asked` 60; the clause is rendered in **43** records (the 17-window gap is windows
counted at note-build time whose record was never written — an async drop or a cache-served re-put;
reported, not diagnosed). **One** wording exists corpus-wide, and it contains **no
"stays available after" promise**:

```
[their stack is draining N triggers - each link will put this same list to you; HOLD (Hold priority)
covers every link. The rows above are what is legal NOW - this says nothing about what will still be
legal after their stack resolves]
```

Five checked against the stack block at the same seq — all TRUE and all counting **opposing triggered/
activated abilities only**:

| seat / seq | stack block | clause N |
|---|---|---|
| 123v125 s360 T49 Upkeep | 2 x `Staff of Nin's Draw 1 [from their Staff of Nin] [triggered/activated ability]` | 2 ✓ |
| 123v125 s452 T57 Upkeep | same 2 | 2 ✓ |
| 123v146 s20 T9 Main 1 | 3 objects: 2 opposing Lost Mine triggers **+ 1 of the seat's OWN Intruder Alarm trigger** | **2 ✓** (own excluded) |
| 123v162 s38 T10 Draw | 4 x opposing Underworld Dreams triggers | 4 ✓ |
| 123v162 s39 T10 Draw | 3 (one resolved) | 3 ✓ |

**Did a hold across a drain lose a legal option a trigger created?** 19 drain windows were answered
with the hold row. In every one the seat was asked again at the next seam within the same or next
phase and went on casting (e.g. 126v125 s191 hold → s194 `Cast Idyllic Tutor`; s252 hold → s255
`Cast Sanguine Bond`). **No observed loss.**

### (d) S6 — the `[own loop verdict:]` line was **never rendered**. **UNTESTED**

`own_loop_windows_asked` = 2 (126v130 x1, 126v162 x1). `[own loop verdict:` occurs **0 times in all
42 logs**. `w78OwnLoopVerdictLine` is wired at two sites (`:42204` priority, `:46110` cast), and
`w77CountOwnLoopWindow` is called before the collapse gates, so the two counted windows produced no
asked record and the line reached no prompt. Consequence: this wave can say nothing about which face
was chosen, whether it was true, or whether the seat acted on it. **CY F4's own risk — the UNPROVEN
face silencing a true RESOLVING verdict — is likewise unobservable.** The two 126v130 loop games did
render the older `LOOP COMPLETE:` board note and the seat played into it correctly (s17 `+1: create a
1/1 vampire with Sorin`), but that is the wave-66 surface, not S6's.

### (e) S7 / S9 — the hold-check bracket. **S7 contract PASS; S9 one real residual**

* **2,182 brackets**; **2,063** carry `hold_check_ref_seq`; the **119** without one are exactly the
  119 `this is the first window I have asked you at this seam` sentences. **0 records carry
  `hold_check_ref_seq` without a bracket** (wave 77: 18) — S7 (b) PASS.
* **Every one of the 2,063 ref seqs resolves to a real record `seq` in the same seat file** — 100%
  (wave 77: ~20% off by 1-3). S7 (a) PASS.
* Ref == the previous asked window at the same seam: 1,831 of 2,063 by my three-way seam proxy
  (cast = `ask`/`unclassified`, land = `ask`/`land_drop`, priority = `priority`/*). The 232
  "mismatches" all point one or two windows further back **at the same seam class**, which my proxy
  cannot split (`unclassified` covers more than one real seam). **Not asserted as a defect** — I could
  not reproduce the engine's seam identity from the translog.
* **The `N gone` / `N new` figures.** Recounted independently over `options_text` with a brace/bracket
  depth stripper, decline+repeat rows excluded: **gone true on 1,761 of 1,779 (99.0%)**, **new true on
  2,042 of 2,063 (99.0%)**. Hand-checked residuals: most are annotation-only deltas the engine
  correctly ignores (a target's P/T moving 2/2 → 4/4 at `123v126` s165) or row text deeper than my
  view. **One is real** — see below.
* **Renumbering: NOT counted as new.** `125v123` seq 263 (ref 260): the two acting rows went from
  `deal 1 damage with Staff of Nin targeting …` to `deal 1 damage with Staff of Nin **#2** targeting …`
  — a second Staff had entered and the engine renumbered — and the bracket read
  `every row above was also on the menu at the last window I asked you at this seam (1 window in a row
  now), and no row that was on it is gone`. The bracket's own contract says a row "naming a different
  card" re-opens a hold; `w78StripHandleOrdinal` strips ` #N` before comparing, so an ordinal that
  **appears because a second instance arrived** is invisible. See defect MED-1.
* The `first window asked` sentence after an asked predecessor: **0 occurrences** — all 119 are
  genuinely first-at-seam.

### (f) S11 — `forced_close`: 32 records reconcile exactly, and the close never closed anything

The 32 records are **16 pairs**, every pair `outcome: armed` → `outcome: dropped_decision_moved` on
the same `arm` and `window_seq`, `park_armed: 0` and `defer_ticks: 0` on all 32.

```
32 forced_close events = 16 armed + 16 dropped_decision_moved
16 dropped_decision_moved == forced_close_unrecorded (16)
16 armed              == the 16 `unclosed <think> (budget/truncation); forcing the answer`
                          lines in the stderr files (123v130 2, 125v123 1, 125v130 5, 126v123 2,
                          146v130 1, 152v125 2, 152v130 1, 152v162 1, 162v123 1)
0 recorded closes, 0 deferred, 0 refused
```
So the identity closes to the byte: **32 = 16 + 16 + 0 deferred**, and what superseded each of the 16
is named on its own record — `dropped_decision_moved`, the decision moving out from under a pending
retry. 14 of the 16 drops are the seq immediately after their arm; the exception is `125v130`
event 5/6 (seq 80 → 86). **CV's S11 prediction that every wave-77 "ten" would read
`dropped_decision_moved` PASSES.**

**CY F11's continuation bound did NOT fire, and could not have**: `force_close_arms_refused`,
`_deferred`, `_same_arm_deferred`, `_defer_bound_hits` are all 0 because **no arm was ever deferred or
parked** this wave (`park_armed 0`, `defer_ticks 0` on every record). CY F11 = **UNTESTED**.

`phase2_answer_recovered` 0 / `_missing` 0 (wave 77: 15 / 1): with `reply_truncated` 0, `transport`
`http=200,empty=0` on all 2,629 calls and `deadline_pct` max 43.7, no reply arrived without an answer
segment, so the phase-2 second call was never entered. *This seat's reading, not an owner position:*
the path stopped being exercised because the 16 budget truncations were all taken by the force-close
arm instead, which then dropped them.

### (g) S5 — the conditional-counter clause. **PASS on its one instance; CY F3 vacuous**

Exactly **one** `{your <counter> already targets this …}` clause corpus-wide:

`125v126` seq 311 (T68, Main 1):
```
{your Dream Fracture already targets this on the stack - a second counter on the same spell is spent
for nothing unless yours is itself countered or removed first}
```
Stack at that seq: `2: your Dream Fracture {1}{u}{u} (instant) [spell] targeting Idyllic Tutor` /
`3: opponent's Idyllic Tutor {2}{w} (sorcery) [spell]`. **TRUE**, and Dream Fracture is an
unconditional counter, so "spent for nothing" is the right face. The seat answered
`Cast nothing right now` — **no double-counter anywhere in the corpus; the `125v146` seq 125 shape did
not recur.** CY F3's conditional (Mana Leak class) face: **UNTESTED** — `Mana Leak` appears in **0**
prompts, and 0 prompts pair any conditional counter with "spent for nothing".

### (h) S8 / S13 — the gloss and the prompt budget

**S8, `(...more)`: PASS.** 3,354 `{effect: "…"}` glosses, 49 distinct; 508 renders carry `(...more)`,
4 distinct. All four cut at a whole clause and keep quotes/parentheticals atomic (CY **F6 PASS**;
independent check: **0** `{card text:}`/`{effect:}` groups with an odd number of `"`). The four:
Lightning Greaves `… -- Equip {0} (...more)`; Intrepid Adversary `… any number of times. (...more) --`;
Briarbridge Tracker `… investigate. (...more) --`; Elite Spellbinder `… exile a nonland card from it. (...more)`.

**S8, the OTHER cutter: FAIL.** CW's S8 prediction was "zero glossed bodies end in a mid-clause `...`".
**Four distinct glosses take the wave-48 bare-`...` word cut and are mid-clause** (`textSnippetCore`
falls through to it when no clause boundary fits the per-clause budget). Worst is Ranger Class
(`152v125` s22), where the per-clause budget cuts *every* clause into an unreadable stub and leaves an
opening parenthesis unclosed:
```
(Gain the next level as a sorcery to add... -- When Ranger Class enters, create a 2/2... -- {1}{G}: Level 2
 -- Whenever you attack, put a +1/+1 counter... -- {3}{G}: Level 3 -- You may look at the top card of your...
 -- You may cast creature spells from the top of your library.
```
Also Sigarda (`… if you control three or more creatures with different...` — the coven condition is
the half that is cut; and `look at the top five...`) and Barrowin (`… from your graveyard...`). All
three cards' `text=` fields in `bin/Res/sets/primitives/borderline.txt` are complete — the truncation
is the engine's. See defect MED-2.

**S13 hoist: PASS.** 156 windows carry `Card text shared by options N-M (they all act with the one card
this text belongs to, so it is printed once here instead of on each of them): "…"`, and in **0** of
them does the hoisted body also appear inside a covered numbered row. (`options_text` keeps the full
row, which is correct — that is what `chosen_text` binds against.)

**S13 budget.** Max prompt **34,778 B** (`125v126` s318, T69 — the specimen), down from 36,211.
Over-20 KB share per deck: deck125 **44%** (348/783, wave 77: 42%), deck126 42%, deck130 24%,
deck123 22%, deck146 18%, deck152 9%, deck162 0%. deck130's max is 29,641 B at `130v125` s144 (T64),
of which **20,712 B (70%) is the game-log narration** — the 24,000 B trim fired (`(...earlier events
trimmed …)` is present) and the prompt is still narration-dominated. The CW deck130 narration item
stands. **CW's S13 prediction ("no prompt contains the same `{card text: "…"}` body twice") FAILS: 94
prompts do**, up to 5 copies (`126v123` s32, five rows each repeating Staff of Nin's text). The hoist
is keyed on *one card instance*; two instances of the same card are not hoisted. See defect MED-3.

### (i) S12 — `Opponent life trend:`. **PASS; CY F7 PASS**

2,573 lines. 1,393 `unchanged at N since turn N` (no span to split), **294 carry the GAINED/LOST split**
(`over those turns life-gain EVENTS put +G on them and life-loss EVENTS took -L off - the figure before
this is the two netted`), 886 net-only over a moving total (no WEventLife source for the span).
**gain + loss == the printed net on all 294** — 0 arithmetic errors. Sample (`123v126` s94, T13):
`turn 10: 20, turn 12: 18, turn 13: 13, now 14 (-6 since turn 10; … +1 … -7 …)` — consistent with the
printed trajectory. No false figure found.

### (j) S14 / S15

**S14 keep-X: PASS on colour (CY F9).** 50 `Holding up an instant:` clauses, 6 distinct cards, each
castable in the seat's own printed colours: Essence Scatter `{1}{u}` x25 (seat makes `{u}{w}`),
Cancel `{1}{u}{u}` x12, Path to Exile `{w}` x10, Soul Shatter `{2}{b}` x1 (seat makes `{b}{w}`),
Starstorm `{2}{r}{r}` x1 (deck130), Fall of the Gavel `{3}{u}{w}` x1. **No counter-example.**
*Note for the docket:* the implemented clause names the cheapest **instant**, not "the cheapest castable
COUNTERSPELL" as the brief states — 4 of the 6 named cards are removal/sweepers, not counters. The
render is honest (it says "instant"); the brief's wording is what is off.

**S14 second copy: PASS.** 79 `[second copy:` renders, 6 distinct; **0** contain "DOUBLING".

**S14 loyalty: near-PASS.** 146 loyalty rows, **143** carry `{counter cost: …}` naming the walker's
current total in its own group (`{counter cost: spends 2 loyalty counters - Sorin, Lord of Innistrad
has 5 now, leaving 3}`). The 3 without are all `+0:` rows on Lolth, Spider Queen (`146v152` s18/24/30),
where the cost is zero. Prediction fails in the letter; the cause is benign. No hold-check bracket was
found counting a walker row as new because its loyalty moved.

**S15 fold: PASS.** 5 windows. Option numbers intact (`1.` / `2.` / `3.` / `4-20. add 3 counters
{same effect right now: adds 2 counters} … add 19 counters … - one option per number from 3 to 19` /
`21.`); no prompt prints four or more consecutive un-folded `{same effect right now: adds …}` rows;
**no answer named a folded row** (choices taken were 1, 2, 3 — all outside the band).

### (k) S2 — the repeat-row carve-out. **PASS**

15 windows carry a repeat row. Every one closes with the carved-out golden sentence, e.g.
`123v130` s87:
```
… the number (0 = pass priority) and its SHORT NAME in parentheses (the action and card name only -
copy nothing from the {...} annotations; the ONE exception is a row whose name ends ", repeated then
stop": on that row you also write the count, as "<short name> x<N>" with N a digit you choose, exactly
as that row's own bracket shows), e.g. "CHOICE: 3 (Cast Example Card)" … Write nothing else.
```
`repeat_count_reask` is **not emitted** by this build's gameend and no stderr trace names a
count-less repeat refusal — 0 observed, but on only 15 windows and 4 `repeat_annotated_takes`, so the
prediction is weakly supported. `reasoning_ngram_repeat` / `reasoning_degenerate`: no incident (above).

### (l) S10 — **UNTESTED**

**0** `defer` records of any kind in the corpus; **0** records or prompts mentioning
`menu_not_askable`. **0** occurrences of `dropped after its X was announced`. CX's S10 prediction
cannot be adjudicated on this corpus.
One X-announcement payment loss did occur by a different route: 22 windows carried the
`{declining strands the N mana already paid …}` warning and **one** answer took the decline —
`125v130` s305, stranding 12 mana, and that window only existed because of the s303 fallback (a) #6.

### (m) Counters

* **`async_drops` 46 — all re-asked.** stderr: 39 `casting arm; the question and board moved`, 5
  `casting arm; the board moved`, 1 `land-drop arm; the board moved`, 1 `casting arm; the question
  (or turn/phase) moved` — **every one `[outcome: re-asked]`**. PASS.
* **`crossphase_identical_reputs` 93 / `_board_unchanged` 4.** 91 notes rendered in records (2 counted
  windows wrote none). The 4 unchanged-board ones, verbatim:
  * `125v126` s250 T43 End — `[this exact list was put to you 1 window ago at Main phase 2 and you declined; nothing on the board has changed]`
  * `125v130` s128 T19 End — `[this exact list was put to you 1 window ago at Main phase 2 and you declined; nothing on the board has changed]`
  * `125v130` s432 T66 Upkeep — `[this exact list was put to you 1 window ago at Untap and you declined; nothing on the board has changed]`
  * `125v130` s456 T68 Attackers — `[this exact list was put to you 2 windows ago at Combat begins and you declined; nothing on the board has changed]`
  The other 87 print the R8 negative branch (`the board has moved since then, but this list has not`).
* **`mana_only_windows_skipped` 367 (wave 77: 69).** The growth is **entirely deck126**:
  126v130 156, 126v162 129, 126v125 64, 126v123 14, 126v146 4 — 367, and 0 on every other seat.
  deck126's Chromatic Lantern / Overgrown Battlement boards put mana-only menus up constantly. I could
  not verify "truly mana-only" per window (a skipped window writes no record); the gate is
  `isManaOnlyAction` over every base row plus no pending cost, and no seat shows a missed mana ability
  in its events. Reported, not cleared.
* **`hold_windows_skipped` 2,816 (cast 2,310 / priority 506).** Concentrated in the long games:
  126v125 1,334 (1,324 cast) in the 85-turn game, 123v125 309, 125v123 240, 146v125 131. The cast-side
  growth is the 85-turn game, not a behaviour change.
* **`protocol_deviation_replies` 109 = 90 + 13 + 6.** 20 `unlabelled_plan` samples classified: in
  **every one** the plan line is present but unlabelled (or labelled `THE PLAN:`) while the ANSWER line
  is fully labelled — `CHOICE: 2 (Hold priority)`, `PUT: 1, 5`, `ATTACK: A1`, `CHOICE: 0 (pass)`. **The
  parser found the action in all 20** (`choice` >= 0, `chosen_text` set, no fallback). `unlabelled_plan`
  is a label-only deviation with zero decision cost — with the single exception of `162v146` s16, where
  it co-occurred with a missing CHOICE label and fell back.

### (n) CY F8 — the CG shape is **still live**. **F8 prediction FAIL**

Consecutive re-asks at the same seam in the same turn, over the 42 seat logs:

| | count |
|---|---|
| rows **byte-identical** to the previous put | **233** |
| rows identical **after stripping `{…}`/`[…]`** (differ only inside annotations — the CG shape) | **186** |
| rows genuinely different | 1,035 |

**419 of 1,454 (29%) same-seam same-turn re-asks put a question whose action content had not moved**,
costing **578.8 minutes of decode across the 42 seats**, and **236 of the 419 came back with the same
answer as the previous put**. CY F8 stripped the hoisted header from the ask/async keys; these 419 are
the residual — the CG shape caught a *fourth* time. (This is measured on windows that were actually
asked; `ask_replays_reserved` 1,344 and `identical_ask_answers_reserved` 953 are the ones the caches
did catch.) See defect HIGH-2.

### (o) The harness

No tripwire line in any of the 21 `game-*.stderr` (the only `storm` matches are the card *Starstorm*).
0 HUNG, 0 crashed, 0 timeout-tainted, `deadline_pct` max 43.7 of a 900 s wall. `results.tsv` records
the two long games at **85 turns** (`125v126`) and **68 turns** (`125v130`), with 62 (`125v123`) and
34 (`146v125`) behind them — the brief's "62 and 81" does not match `results.tsv`'s 85/68; worth
reconciling with whatever the late check read. Neither long game stalled: both ended naturally with a
winner.

---

## 3. Prediction adjudication

### lane CV

| pred | verdict | evidence |
|---|---|---|
| S3: no (turn,phase,seat) cell shows more than **two** asks over a stop-reached menu | **FAIL** | 123v152 T9 Main 1 alone shows **seven** (s18-24); 123v162 T10 Draw shows four (s38-41); 123v130 T14 Main 1 three |
| S3: `stop_reached_reputs_collapsed` non-zero on deck123's seats | **FAIL** | 0 on all 42 seats |
| S3 negative pin: a third ask with N>=1 means the key moved | **fires** | N reads 1..9 on the re-puts and the window was still asked → the blocker is `w78RePutIdentityStands`, i.e. the board key and/or the stop statement, not `optionSetKeyOf` (which did hold — `declinedN` kept climbing) |
| S3: collapsed never exceeds skipped | **PASS** (0 <= 44) | |
| S4: asks in a drain cell <= `theirTriggers` | **PASS** | 123v162 T10 Draw: 4 triggers, 4 asks (s38-41); 123v125 upkeeps: 2 triggers, 1-2 asks |
| S4: at least one drain window answered with the hold | **PASS** | 19 of 43 |
| S4 negative pin: counter > 0 with zero prompts carrying the clause | **not tripped** | 60 counted, 43 rendered — the gap is unwritten records, not an un-spliced note |
| S6: every gameend with `own_loop_windows_asked` > 0 has `[own loop verdict:` in its prompt | **FAIL** | 2 counted, **0** rendered lines in 42 logs |
| S6 negative pin: "the line is being built and dropped before the splice" | **not confirmed** — the counted windows produced no asked record at all, so the splice is untested | |
| S7: 0 records carry `hold_check_ref_seq` with no bracket | **PASS** (0, wave 77: 18) | |
| S7: `hold_check_ref_seq` joins to a real record `seq` at 100% | **PASS** (2,063/2,063) | |
| S7: `hold_check_ref_window` reproduces the wave-77 values | **UNTESTED** — different corpus, no comparable | |
| S9: no bracket's `N rows above are new` exceeds the ordinal-stripped absent count | **PASS at 99.0%** (2,042/2,063 on an independent recount); residuals are annotation-depth artifacts of my key, except MED-1 | |
| S9: at least one bracket prints `… is gone and no row above is new` | **PASS** — 1,779 brackets print a gone figure | |
| S11: `forced_close_events` == refused + same_arm_deferred + armed | **PASS** — 32 = 0 + 0 + 16 armed + 16 drops (the drops are the second half of each event pair) | |
| S11: records sum to the stderr `unclosed <think>` lines per seat | **PASS** — 16 arms == 16 stderr lines, matched per game | |
| S11: every wave-77 "ten" carries `dropped_decision_moved` | **PASS** — all 16 drops do | |
| S11 negative pin: another outcome while `same_arm_deferred` is 0 | **not tripped** | |

### lane CW

| pred | verdict | evidence |
|---|---|---|
| S2: zero `repeat_count_reask`; every repeat-row window's closing sentence contains `<short name> x<N>` | **PASS (weak)** — 15/15 windows carry it; the counter is not emitted and no refusal trace exists, on a small population | |
| S2 negative: ordinary sentence byte-unchanged where no repeat row | **PASS** | |
| S5: every counter row whose target is already targeted by the seat's own counter carries the clause | **PASS on n=1** (`125v126` s311) | |
| S5: no seat casts a second counter onto a spell its own counter already targets | **PASS** — 0 occurrences | |
| S8: zero glossed bodies end in a mid-clause `...` | **FAIL** — 4 distinct do (Ranger Class, Sigarda x2 forms, Barrowin) | |
| S8: every Brutal Cathar board gloss contains "leaves the battlefield" | **UNTESTED** — the Cathar gloss in this corpus is the un-truncated form; no cut render to test | |
| S12: every both-directions trend line carries both halves; no split with gained==0 or taken==0 | **PASS** — 294 splits, all non-zero on both halves, all arithmetically consistent | |
| S13: no prompt contains the same `{card text: "…"}` body twice | **FAIL** — 94 prompts do (up to x5) | |
| S13: every window that would have carries exactly one `Card text shared by option…` line | **PASS** — 156 windows, hoisted once, never inside a row | |
| S14: every `{X pricing:}` on a seat holding a castable instant carries `Holding up an instant:` | **UNTESTED** — I did not enumerate X-pricing blocks whose seat held an instant and no clause printed | |
| S14: no `[second copy:` contains "DOUBLING" | **PASS** (0/79) | |
| S14: every loyalty row carries `{counter cost:}` with the current total | **FAIL (3 of 146)**, all `+0:` rows on Lolth | |
| S14: no bracket counts a walker row as new because its loyalty moved | **PASS** — none found | |
| S15: no four-or-more consecutive `{same effect right now: adds …}` rows; every folded window's `chosen_text` names a single rung | **PASS** (5 windows) | |
| S15 negative: a live-verdict rung is never folded | **PASS** — rows 1-3 stayed unfolded in all 5 | |

### lane CX

| pred | verdict | evidence |
|---|---|---|
| S1: 0 records of the two-line shape read `prose_outside_two_lines`; all read `answer_label_absent` | **PASS** — the class does not appear at all; 6 records read `answer_label_absent` | |
| S1: `answer_label_absent_heuristic_played` == the count of `unparsed_reply` ask records of that shape | **PASS** — 6 == 6 | |
| S1 falsifier: an `answer_label_absent` record whose reply contains prose | **not tripped** — all 6 are exactly PLAN line + `<n> (<name>)` | |
| S10: every `defer` record carries non-empty `options_text` whose length equals `options`, and names a card in its own seat's deck | **UNTESTED** — 0 `defer` records | |

### lane CY (the twelve Astra findings)

| finding | verdict | evidence |
|---|---|---|
| F1 (S3, collapse identity) | **FAIL** | 0 collapses over 44 asked stop-reached windows; §Weakest 1 realised — the board key and/or the stop statement moves at every re-put |
| F2 (S4, drain clause wording + TRIGGERS only) | **PASS** | one wording, no "stays available"; N == opposing triggers on all 5 checked, own trigger correctly excluded |
| F3 (S5, conditional counters) | **UNTESTED (vacuous PASS)** | 0 Mana Leak / `pay…?fizzle` counters in the corpus; the one clause is for an unconditional counter and is correct |
| F4 (S6, UNPROVEN face) | **UNTESTED** | 0 rendered verdict lines; neither face observed |
| F5 (S9, note-reuse guard + shared latch key) | **UNTESTED from the translog** | stderr carries 33 `hold re-opened at the cast seam` traces in `125v123` alone but no seq, so the bracket-before-reopen join cannot be made |
| F6 (S8, quotes/parens atomic) | **PASS** | 0 odd-`"` groups; all 4 `(...more)` cuts keep units whole |
| F7 (S12, WEventLife split) | **PASS** | 294 splits, gain+loss == net on all |
| F8 (S13, hoisted header out of the keys) | **FAIL** | 419 same-seam same-turn re-asks (233 byte-identical + 186 identical-after-strip) still asked, 236 answered identically |
| F9 (S14, colour-aware keep-X) | **PASS** | all 50 clauses name a colour-affordable instant |
| F10 (S1, counter keyed on the final class) | **PASS** | `answer_label_absent_heuristic_played` 6 == 6 `answer_label_absent` records |
| F11 (S11, continuation checks arm + park) | **UNTESTED** | 0 deferrals, 0 parks — the bound had nothing to fire on |
| F12 (`corpus-stats.py` reconciliation) | **FAIL** | census still prints `CENSUS CHECK MISMATCH`; the `engine_answered` reveal is the 41st |

### Carried UNTESTED docket

| item | verdict |
|---|---|
| BS F2 Tribute second edict | **UNTESTED** — no Tribute-class second-edict window in the corpus |
| BS F3 / land latch — `declined_face_latches` 0 for the **6th** wave | **RETIRE** as scheduled: 0 again |
| BS F5 blockers ceiling | **UNTESTED** — 22 blocker records, `blocker_forecast_rows` 250, `_collapsed` 8; no window reached a ceiling |
| BS F6 distinct-index re-ask | **UNTESTED** — `identical_option_asks_resolved` 6, none of them a distinct-index shape |
| CF F1 pending-deadlock | **UNTESTED** — 0 deadlock traces, `menu_pass_no_progress` 0 |
| F2 / F5-nonlifelink / F8-haste / F9 (carried) | **UNTESTED** — no qualifying window found |
| reasoning-body halves of N14a / N14d / O20 | **UNTESTED** — the reasoning body is recorded but this seat did not read 2,629 reasoning traces; not attempted |
| `{X}`-announcement payment loss | **0** `dropped after its X was announced`; **1** real loss by another route (`125v130` s305, 12 mana stranded after the s303 fallback) |

---

## 4. Engine / render / narration defects

### HIGH-1 — the S3 collapse cannot fire on a live board; 90 minutes of decode for 3 decisions
`stop_reached_reputs_collapsed` 0 over 44 asked stop-reached windows in 4 chains (5,448 s). Repro:
`123v152` seqs 16-25 (T9) — ten windows, one acting row
`Create human with Thraben Doomsayer [cost: Tap] … {right now: M=26, your stated stop=26, so this
window would add to a count ALREADY AT OR PAST your own stop - past your stop = a wasted window}`
plus the hold row, `[you declined this exact list 1..9 times already this turn]`, nine `pass` answers.
`declinedN` climbed correctly, so the refusing leg is `w78RePutIdentityStands` — the board key
(`123v162` s36-41 proves it: life 20→19→18→17 across identical-row re-puts as their triggers drained)
and/or the stop statement's `stopTurn`/`stopCountAtStatement`, which the model re-dates by restating
its PLAN at nearly every window. *Reading, not an owner position:* an identity that includes the whole
serialised board can never be stable across a draining stack, which is exactly the situation the
collapse was built for.

### HIGH-2 — 419 same-seam re-asks whose action content had not moved (the CG shape, 4th wave)
233 byte-identical + 186 identical-after-annotation-strip, 29% of all same-seam same-turn re-asks,
578.8 min of decode, 236 answered identically. Repro: `123v126` s82→s85 (priority, T13) and
s83→s86 (cast, T13) — consecutive same-seam puts whose `options_text` differ only inside `{…}`/`[…]`.
CY F8's header strip did not reach these.

### MED-1 — a renumbered source is not counted as a new row (`Staff of Nin` → `Staff of Nin #2`)
`125v123` seq 263, bracket referent seq 260. Rows went from
`deal 1 damage with Staff of Nin targeting the opponent` / `… targeting you` to
`deal 1 damage with Staff of Nin **#2** targeting the opponent` / `… targeting you`, i.e. the rows now
address a **different permanent**, and the bracket printed
`[hold check: every row above was also on the menu at the last window I asked you at this seam (1 window
in a row now), and no row that was on it is gone …]`. The bracket's own contract says a row "naming a
different card" re-opens a hold. `w78StripHandleOrdinal` strips ` #N` unconditionally, so an ordinal
that appears *because a second instance entered* is invisible to the delta. S9's fix is correct for the
renumber-in-place case and wrong for the new-instance case.

### MED-2 — the fallback gloss cutter truncates mid-clause and can leave an unclosed parenthetical
`152v125` seq 22, Ranger Class (see §2(h) for the full rendered string): every clause is a stub
(`create a 2/2...`, `put a +1/+1 counter...`, `look at the top card of your...`) and the opening `(`
is never closed. Same cutter at Sigarda (`… creatures with different...` — the coven condition is what
is cut) and Barrowin (`… from your graveyard...`). The cards' `text=` fields in
`bin/Res/sets/primitives/borderline.txt` are complete; the cut is `textSnippetCore`'s wave-48 word-cut
fallback, which fires whenever no clause boundary fits the per-clause budget and marks the omission
with a bare `...` instead of `(...more)`.

### MED-3 — the S13 hoist is instance-scoped, so duplicate bodies survive
94 prompts print the same `{card text: "…"}` body 2-5 times in one option block. Repro: `126v123`
seq 32, five rows each repeating `At the beginning of your upkeep, draw a card. -- {T}: Staff of Nin
deals 1 damage to any target.` (two Staff instances, so the "one card this text belongs to" test
fails). Also `125v123` s203 / s222 / s235.

### MED-4 — deck130's prompt is still 70% narration after the trim
`130v125` seq 144 (T64): 29,641 B prompt, 20,712 B of it the game log, with the 24,000 B trim already
fired (`(...earlier events trimmed …)` present). deck125 remains at 44% of prompts over 20 KB.

### LOW-1 — `stack_drain_windows_asked` 60 vs 43 rendered
17 counted windows produced no record carrying the clause. The counter is incremented at note-build
time; a window whose answer is then cache-served or async-dropped is counted but never logged. Not a
truth defect; it makes the counter unusable as a render census.

### LOW-2 — `corpus-stats.py` reconciliation (CY F12 not closed)
See §1. `engine_answered` reveals make no round trip; subtract them rather than re-classing `reveal`.

### LOW-3 — the S14 keep-X clause names an *instant*, not a *counterspell*
4 of the 6 named cards (Path to Exile, Soul Shatter, Starstorm, and arguably Fall of the Gavel) are not
counters. The render is honest; the brief and the docket describe a narrower contract than what shipped.
Worth deciding which one is meant before a future lane "fixes" the render to match the docket.

---

## 5. Lategame specimen

`wave78/lategame-specimen.txt` — `125v126` (deck125 seat) record seq **318**, kind `priority`,
**turn 69**, phase Upkeep, life 66 vs 16, 10 option rows, **34,778 prompt bytes** (the largest prompt
in the corpus). 3-line header on the file, then the `prompt` field verbatim.

---

## What this seat did NOT check

* **The 2,629 reasoning bodies.** Not read. The reasoning-body halves of N14a / N14d / O20 are
  therefore UNTESTED, as is any claim about whether the model *read* the drain clause, the trend
  split, or the stop verdict — annotation-present-vs-outcome is a cross-tab, never causal.
* **Windows that were skipped or collapsed.** `mana_only_windows_skipped` 367,
  `hold_windows_skipped` 2,816, `own_turn_windows_skipped` 650, `main_phase_windows_skipped` 83 and
  the 44 wave-72 stop-reached skips write no record, so "was every skipped window truly mana-only /
  truly held" is unverifiable from this corpus. I checked only that no seat's event stream shows a
  mana ability it never got to use.
* **The engine's own seam identity and row-identity keys.** `holdActionKeyRow` / `optionSetKeyOf` /
  `declineBoardScope` were read in source but not reproduced byte-for-byte; my hold-check recount uses
  a brace-depth stripper and a three-way seam proxy, which is why I report 99.0% agreement and one
  hand-confirmed residual rather than a clean pass/fail per bracket.
* **Per-deck play quality.** Out of scope for this seat; the deck agents own it.
* **`askreplay/`** — 30 files present and counted, contents not opened.
* **The wave-77 corpus.** Comparisons use `~/.gatelogs/w77-stats.out` and `wave77/engine-seat.md`
  figures as quoted in the brief; I did not re-derive them from the wave-77 logs.
