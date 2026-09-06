# Wave-63 engine-narration seat

Corpus read: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-191148` (21 games, binary
`98d13050f`) + the two owed reruns `matchups-20260905-214946` (152v162) and
`matchups-20260905-215345` (152v130), both on `0026a9d87`. Method: python over the JSONL, counts
taken from the rendered `prompt` (never from `options` / `options_text` alone where the brief's
prefix rule applies), `/usr/bin/grep` for source and primitives. No build, no git, no wagic run,
nothing written outside `wave63/`.

---

## 1. Corpus census

### Games

| | count | note |
|---|---|---|
| launched (191148) | 21 | 42 seat files |
| ended naturally | 19 | `results.tsv` |
| HUNG | 2 | 152v162, 152v130 — daybound marker loop, root cause **already recorded in `wave63/corpus-hang.md`** (DescriptorTargetChooser never accepted designation markers, so every marker count read 0); not re-derived here |
| crashed / SEGV / abort | **0** | no `Segmentation`, `SIGSEGV`, `core dumped`, `Assertion` or `abort` in any of the 23 `.stderr` files |
| reruns (0026a9d87) | 2 | 152v162 → 162 won (t14); 152v130 → 152 won (t24). Both natural. |
| **games in the adjudication set** | **21** | 19 natural + 2 reruns; the 2 hung partials are used as hang evidence only |

`gamestart` 46 / `system` 46 (23 games × 2 seats) vs `gameend` 42 (21 ended games) — the arithmetic
closes; the 4 missing `gameend`s are exactly the two hung games' four seats.

### Decisions

Decision records (`ask`/`priority`/`attackers`/`blockers`/`discard`/`reveal`/`bottom`):
**2,888 in the 21 ended games**, plus 125 written by the two hung games before the watchdog.
Total records 3,150.

| kind | ended games | hung partials |
|---|---|---|
| ask | 2,208 | 92 |
| priority | 486 | 15 |
| attackers | 94 | 9 |
| discard | 57 | 0 |
| blockers | 26 | 5 |
| reveal | 12 | 3 |
| bottom | 5 | 1 |
| recovery | 2 | 0 |
| wall_miss | 1 | 0 |

Decisions by deck seat (all 23 games): 125 = 1,083 · 146 = 473 · 130 = 442 · 152 = 360 · 123 = 317 ·
126 = 191 · 162 = 150.

### Fallbacks, transport, timeouts, stalls

| signal | count | citation |
|---|---|---|
| `fallback` records, all kinds | **4** | below |
| `fallback: wall_miss_unrecorded_wall` | 1 | `…deck123-…-vs-…deck130.jsonl` seq 11 (recovered at seq 12) |
| `fallback: repeat_count_reask` | 1 | `…deck123-…-vs-…deck126.jsonl` seq 28 (`parse_note: repeat_count_reask_recovered`) |
| `fallback: distinct_index_reask` | 1 | `…deck125-…-vs-…deck146.jsonl` seq 121 (`duplicate_index_short`, recovered) |
| `fallback: unparsed_reply` | 1 | `…deck146-…-vs-…deck125.jsonl` seq 154 (recovery record at seq 155) |
| `transport` field present | **1** | seq 11 above: `curl=28,http=0,empty=1,connect_ms=20000,phase=wall` |
| `transport_error` | **0** | field never written; no `transport_error` string in any `.stderr` |
| `empty_reply` / `bad_reply` / `reveal_stall_forced` | **0 / 0 / 0** | none in records or stderr |
| `deadline_pct > 100` | **0** | max 100.0, and that one is the wall_miss record itself; next highest 91.9 |
| `wall_miss_events` / `wall_miss_unrecorded` | 1 / 1 | one game only |
| `async_drops` | **29** across 14 seat files | worst `…deck146-…-vs-…deck125` = 7 |
| `dropped_assignments` | **0** | 31 records carry the field, all zero |
| `mana_only_windows_skipped` | 827 | summed over 42 `gameend` |
| `identical_ask_answers_reserved` / `identical_option_asks_resolved` | 110 / 3 | |
| `answer_replaced: true` | 14 | |
| `commit_retracted: true` | **0** | |
| records with `reply_trimmed_bytes > 0` | 41 (138,121 B total) | |
| `plan_echo_count` total / `post_answer_overrun` total | 41,631 / 836,183 B | |
| longest single wait | 900,027 ms (the wall_miss); longest *answered* window 827,678 ms | `…deck126-…-vs-…deck125` seq 14 |
| reveal waits | 15 records, 900 s total; **one of 561 s / 144,213 ticks** | `…deck152-…-vs-…deck146.jsonl` seq 31 — see HIGH-3 |

**Wave-58/59 signatures: none recur.** `empty_reply` 0, `transport_error` 0, `bad_reply` 0,
`deadline_pct > 100` 0, `reveal_stall_forced` 0, no SEGV/abort. The only translog gap past the
watchdog is the two hung games, whose mechanism is the one `wave63/corpus-hang.md` already fixed.

### HOLD (prefix-matched on `^\d+\. Hold priority:` in the rendered prompt; take = `chosen_text`
starting `Hold priority:`)

| | wave 62 | wave 63 |
|---|---|---|
| hold row rendered | 459 | **1,951** (ended games) + 57 in the hung partials |
| hold taken | 6 | **359** (+2 in the partials) |
| take rate | 1.3 % | **18.4 %** |
| `hold_windows_skipped` (Σ `gameend`) | 1,191 | **3,203** |
| distinct hold row texts | — | **1** (the new per-seam row with the two-main-phase span) |

### The Vita softlock (`wave63/softlock-blockers-1788650768.md`)

I looked for the reported shape — an AI seat stuck at Blockers with a calm stack. **Not found.**
26 `blockers` records in the ended games, none repeated on an unchanged board, and the only
same-seam repetition at the Blockers step (`…deck130-…-vs-…deck162.jsonl` seq 46 / 48 / 49, turn 14)
is a *cast* ask re-opening after a legitimate row change, and each was answered. No corpus evidence
for or against the human-defender softlock; it stays a Vita-only report.

---

## 2. Prediction adjudication

Counts are N of M with the file (abbreviated `deckAvdeckB@<epoch>`) and seq.

### Lane AB

| # | item | verdict | evidence |
|---|---|---|---|
| 1 | E1 header | **UNTESTED** | 137 records render `INCOMING THIS COMBAT`; **0 of them** sit on a board where the opponent controls both loop halves (13 records corpus-wide have Sanguine Bond + Exquisite Blood on the opponent battlefield, none of them a combat-header window). 25 `best case with every blocker assigned` lines, 0 co-occurrences with the loop clause — the negative half holds trivially; the withdrawal was never exercised. |
| 2 | E1 rows | **PARTIAL / mostly UNTESTED** | 0 B-row windows arose on a loop board, so the blockers half is UNTESTED. The symmetric **attackers**-side tail (the half lane AB flagged as having no corpus record) DID fire, correctly: `126v162@1788653525 seq28` A1, `chains without limit until you are at 0` on the opponent's lifelink and on their blocking trigger, and 0 clauses naming life the READING seat gains carry it. 1 of 1 correct. |
| 3 | E1 decline | **UNTESTED** | `Declining every block costs you no life`: 0 occurrences. No window. |
| 4 | E3a | **PASS** | 25 of 25 `one legal assignment that reaches it` clauses carry `chosen for your blockers' material as well as for the life`; 9 read `every blocker in it survives`, 16 read `N of those blockers die`, 0 neither. Second half: over the 24 headers whose A/B rows I could reconstruct, **0** could be improved by any permutation of the same blockers over the same matched attackers (the seq-25 chump-beside-a-kill shape does not recur). |
| 5 | E3b | **UNTESTED** | `no block saves you`: 0 occurrences — no lethal blockers screen arose in 26 blockers windows. |
| 6 | E4a | **PASS** | 4 of 4 `their untapped blockers` tags carrying `GANG BLOCK:` open with it (`[their untapped blockers: GANG BLOCK:`); 0 place it after a per-blocker result; 0 read `each result above is a LONE blocker only` — the pointer inverted with the hoist and now reads **below** (e.g. `162v152@1788662989 seq21`, `152v130@1788663228 seq45`). |
| 7 | E4b | **FAIL (by the letter), 1 of 1** | Exactly one A-line tag in the corpus lists ≥2 priced candidates: `126v162@1788653525 seq28` A1. It carries `(1 if all block), plus up to 16 more if they take every "may"` while the largest single price printed in the same tag is **4**. The prediction says the sum must be ≥ the worst single price; it is not. The render is nonetheless consistent with the lane's own stated design (certain and "may" halves deliberately not summed) — the prediction did not account for its own split. See MED-1. |

### Lane AC

| # | item | verdict | evidence |
|---|---|---|---|
| 1 | E2 | **PASS** (arithmetic half UNTESTED) | 4 `CLEANUP PRICE` clauses, all in `125v162@1788653517` (seq 53, 54, 56, 61). seq 61's own `ON THE STACK` lists Teferi's Puzzle Box + Howling Mine + Dictate of Kruphix draws and the clause reads `that count INCLUDES 7 cards from draw triggers already ON THE STACK`; seq 53/54 have only damage triggers on the stack and correctly carry no such sentence. 4 of 4 carry `(your L life plus the G this cast gains you, minus the C above)`. The falsifier could not fire: 0 of 4 rows were taken (all four seats chose `Cast nothing right now`), so no following cleanup ask exists to compare against. |
| 2 | E5 | **PASS, 48 of 48** | Every `noncreature permanents … not in that count` clause names **exactly one** permanent (histogram: 48 × 1 name), **0 repeats** — the five-times-one-Lair defect is gone. 48 of 48 carry `best rung their mana pays for: N power once animated` with a `for {…}` cost (`123v152@1788653513 seq10`: `Lair of the Hydra (best rung their mana pays for: 2 power once animated, for {2}{g}; its own mana is NOT counted…)`). 0 entries with neither a power figure nor the no-claim wording. |
| 3 | E5 / LOW-11 | **PASS, 24 of 24** | Every multi-rung self-animate menu carries `{rung ceiling:` on its rows (70 rows: Hive of the Eye Tyrant ×60, Lair of the Hydra 1/1–9/9 ×… ). 0 ceiling clauses on a permanent that makes no mana. (One apparent 25th is my regex matching the word "becomes" inside card text, not a rung menu.) |
| 4 | E9 | **PASS on firing / FAIL on the narration half** | `removes N from the CRACK-BACK total above` occurs in **8 records / 13 rows** (wave 62: 0) — e.g. `123v130@1788653511 seq47`, `125v146@1788653528 seq15`, `130v152@1788663228 seq43`. **13 of 13** opponent-battlefield `DIES` rows inside a record that also renders `CRACK-BACK NEXT TURN` carry it; **0** clauses on a seat-controlled body. But the prediction also required 0 narrated records containing `{removes `: **9 `chosen_text` values carry it**, 4 of them the crack-back clause itself (`123v130@1788653511 seq47`, `125v146@1788653528 seq15` and seq 129, `130v152@1788663228 seq43`). The GAME LOG the model reads is clean (0). See MED-2. |
| 5 | E13 | **PASS on the direction fixes / FAIL on one string / UNTESTED on LOOP CAUTION** | `life to you from your punishers above` **0**; `life to the punishers above` **0**; `life LOST BY YOU` 169, `life LOST BY THEM` 69, and **0 of 169 `DRAW FORECAST` lines price a draw without a `life LOST BY` label**. `LOOP CAUTION` **0 occurrences** — no board with the completed pair on the *opponent's* side reached the DRAW PUNISHERS paragraph, so that half is UNTESTED; where the pair was on the *reading* seat's side the paragraph correctly reads `LOOP SCOPE` (7 records, e.g. `126v162@1788653525 seq24`, both on the punisher sentence and on the forecast). `; converters on your battlefield:` (unqualified) is **30**, not 0 — all 30 on `oneShotDrawGrantTag`, the emitter lane AC's own "did NOT verify" section says it deliberately left alone, and each is immediately qualified in-line (`0 (nothing of yours punishes their draws…)`). Prediction/scope conflict, not an engine defect. |
| 6 | E15 | **PASS** | `Mana available: 0 total` lines: **269 of 269** carry `mana of ANY colour pays a generic cost like {2}` (wave 62: 0 of 177). Nonzero lines 2,485 of 2,485. **0** replies anywhere in the corpus assert coloured mana cannot pay a generic cost or conclude the deck uncastable. |
| 7 | E16 | **PASS** | 22 deck-125/146 cleanup asks hold Emrakul and list land rows; **19 carry `{spare:` on every land row**, and the 3 that do not (`125v123@1788653523 seq65` 13 lands, `125v146@1788653528 seq91` 13 lands, seq 99 14 lands) are windows where the reach figure IS 15 — Emrakul is reachable, so no spare is claimed. Correct in both directions; the falsifier (an 11-land Emrakul cleanup with no `{spare:`) did not occur. |

### Lane AD

| # | item | verdict | evidence |
|---|---|---|---|
| 1 | E6a | **PASS, 14 of 14** | Every `answer_replaced: true` record carries `latched_line`. The stated falsifier (a trimmed record with no `latched_line`) does not occur — 11 of the 14 have `reply_trimmed_bytes > 0` AND a `latched_line` that is *absent from the stored (trimmed) reply*, which is exactly the case the field was added for. The second conjunct ("the line appears in the untrimmed reply") is **not verifiable from the record**: the untrimmed reply is not stored. See HIGH-2. |
| 2 | E6b | **FAIL, 1 of 1** | One record carries `latched_line_in_plan: true`: `130v126@1788653538 seq29` (attackers, t18). It does **not** carry `plan_answer_line_only` — the stated falsifier — and it should not have needed the fail-safe: the reply's line 0 is a top-level `ATTACK: A2, A3, A4`, and the latched `ATTACK: A4` is on line 3, at column 0, after a blank line and a one-line `PLAN:`. Weaker prediction also FAILS: `plan_answer_line_ignored` fires **0** times (the field is never written). `unparsed_reply` count is 1, unchanged from wave 62, so the no-regression half holds. See HIGH-1. |
| 3 | E6c | **PASS, 46 of 46** | Every seat file opens `gamestart` then `system`; every one of the 46 `system` records carries `reply_protocol` containing `The LAST answer line you write is the one that runs`, `system_bytes` 98,398 and one single `reply_protocol_hash` (`af947d2bc622a08a`) across the whole corpus; **every** record with a `prompt` carries a `system_hash` equal to its own file's. 0 mismatches, 0 missing. D9(c) is now adjudicable. |
| 4 | E7 | **PASS, 34 of 34** | 34 ANNOUNCE_X asks. 31 carry `Decline - do not cast this after all …` and none of them prints the no-pass sentence. The 3 that lack the Decline row (`125v152@1788653553 seq58`, `130v152@1788663228 seq24` and seq 34) each state the reason in the wave-23 header: *"There is no decline row on this menu: this spell's costs are ALREADY PAID, so the announcement can no longer be cancelled…"*. Neither shape is crossed. (Wave 62: 17 of 18 with a bare exception.) |
| 5 | E10 | **FAIL by the stated metric (13, was 15) — but 0 promise breaks; second half UNTESTED** | 359 holds taken. 13 same-turn, same-seam windows re-ask a row set identical under `holdKeyRow` after a hold. **All 13** had an intervening window *at the same seam* whose rows differed, which `holdHonoured` retires and which the row's own `(any change re-opens this window)` explicitly covers — e.g. `130v162@1788653515` hold at seq 40 (3 rows), different 5-row menu at seq 43, identical 3-row menu re-asked at seq 46/48/49. Second half: the only two candidate main-1 → main-2 pairs (`123v125@1788653523` 40→44, `146v125@1788653528` 288→292) both sit downstream of such a change, so **no clean phase-clause-only pair arose** — the normaliser is not exercised. Outcome half: `hold_windows_skipped` 1,191 → **3,203** (rises materially, PASS), but "the hold TAKE rate is unchanged" is **FALSE** — 1.3 % → 18.4 %. The model, not only the latch, moved. |
| 6 | E11 | **PASS, 212 of 212** | Every `reveal`/`attackers`/`blockers`/`discard`/`bottom` record carries `choice_meaning: "selection size"`; `latched_row_mismatch`, `latched_coded_line` and `plan_contradicts_noop_row` appear on **0** of them. The 15 `reveal` records still carry a constant `choice: 1`, now labelled rather than read as a row index. |
| 7 | E14 | **(a) FAIL 1 of 2,801 · (b) PASS (positive direction UNTESTED)** | 2,801 served `YOUR PLAN` blocks; 341 carry the cut marker, **0 carry it twice**, and all 341 state the drop against the size the model wrote (`N further characters, of N you wrote`). One self-correction survives: `130v126@1788653538 seq35` — *"…Cast nothing right now. (Wait, the choice is to cast Starstorm. The plan needs to reflect the decision…"* — the marker word is preceded by `(`, which the sentence-start guard does not see. (b): **0** `YOUR PLAN was withdrawn: it says … on neither battlefield` blocks fired, and I found no served plan asserting a permanent's presence that neither battlefield line carries — the deck126 seq29→61 eight-turn shape does not recur. The withdrawal's positive direction is untested because it never fired; the false-positive direction is clean. |

### Lane AE

| item | verdict | evidence |
|---|---|---|
| E8 | **PASS** | 58 `enters (UN)TAPPED … (you control X, a T)` clauses across 88 records. 57 name a witness whose **printed** subtype is T, verified against `sets/primitives/*.txt`: Tundra `subtype=Plains Island`, Scrubland `Plains Swamp`, Bayou `Swamp Forest`, Savannah `Forest Plains`, Underground Sea `Island Swamp` (plus basics naming themselves). The 58th is the wave-62 defect shape itself, now correct: `126v152@1788653551 seq7` — *"Play Isolated Chapel [enters UNTAPPED … (you control **Urborg, Tomb of Yawgmoth, a Swamp - a type it has on the battlefield now, not one printed on the card**)"* — and `Urborg`'s primitive is `auto=lord(land) transforms((swamp))` with no `subtype=`, so the note is right. **0** granted-only witnesses on a prompt that also holds a printed-subtype land. Clause count 88 records / 58 witnessed clauses, not fallen (wave 62: 61). |
| E12 | **PASS** | 654 records mention Nadaar, Selfless Paladin; **191 rendered P/T readings of Nadaar itself, all `(3/3)`, 0 at `(4/4)`**, while its controller's other creatures read one higher with an explicit `(printed X/Y)` (e.g. `146v125@1788653528 seq221`: `Barrowin of Clan Undurr (4/4) (printed 3/3); Nadaar, Selfless Paladin (3/3)`). Primitive now reads `auto=this(variable{pdungeoncompleted}>0) lord(other creature|myBattlefield) 1/1`, matching Oracle *"Other creatures you control get +1/+1"*. |
| E17 | **UNTESTED** | A suite-determinism claim, not a corpus claim; running the suite is outside this seat's constraints. Not adjudicated. |
| E18 | **PASS — and the window finally arose** | **160** rows carry `{repeat cost:` (wave 62: 0 of 100 had the paired window). **19** carry both `{repeat cost:` and `{paying this taps:`, and **0** of the 19 name different numbers in the two halves: `152v162@1788653542 seq24` — `add 4 counters {repeat cost: … which pays for 3 of them and stops} {paying this taps: … (that is the 3 payments your mana covers, not all 4)}`. Rows whose taps half states no count all sit on a cost half whose `pays for N` equals the row's own counter number. |

### Lane AF

| # | item | verdict | evidence |
|---|---|---|---|
| 1 | R1 (X cap) | **UNTESTED** | Largest ANNOUNCE_X menu in the corpus is **22 rows**; no 51-row menu arose. Positive check anyway: across all 34 X menus, row count == max X + 1 (+1 where a Decline row is present) in **34 of 34** — no off-by-one and no clamp artefact. |
| 2 | R3 (`{spare:` cap) | **PASS** | `125v146@1788653528 seq52`: 10 lands down, 4 land cards among the discard rows, clause reads *"Emrakul, the Aeons Torn at 15 is NOT counted here - it is past what 10 lands plus the drops this hand can still make would pay for (**this comparison counts costs up to 14**)"* — 10 + 4, not lands + 2. |
| 3 | R4 (plan cut keeps "Wait") | **PASS** | **0** of 341 cuts truncate at a sentence opening `Wait until` / `Let's <verb>` / `Hold on to`; operative "Wait for main phase 2 to cast Elite Spellbinder…" survives intact (`152v123@1788653513 seq14`, 48 similar). |
| 4 | R5 (ownership-specific withdrawal) | **UNTESTED / negative half PASS** | 0 withdrawals fired, and **0 fired for a card the seat controls** (the half that matters for safety). The positive half ("at least one will fire for an 'I control X' claim whose only live copy is the opponent's") did not occur: I found no such claim in 2,801 served plans. Two near-misses are correctly *not* withdrawn because they are conditional, not assertions (`126v130@1788653538` seq 12/13/14: *"Once both Exquisite Blood and Sanguine Bond are on the battlefield…"*). |
| 5 | R6 (material lexicographic) | **PASS, 24 of 24** | See lane AB #4 — no header claims material over an assignment a legal reshuffle of the same matched attackers improves. |
| 6 | R7 (lord caveat) | **UNTESTED / negative half PASS** | 13 `removes N from the CRACK-BACK total above` rows; **0 are on a lord over another counted attacker** (Goblin ×5, Bloodline Keeper, Triumphant Adventurer ×2, Elite Spellbinder ×2, Luminarch Aspirant, …) and **0 plain bodies carry `BEFORE static effects`** — correct. The positive direction had no window. |
| 7 | R9 (`transport_error` ≠ HTTP 200) | **UNTESTED** | `transport_error` never occurs. The single `transport` record is `curl=28,http=0` (a connect timeout with no HTTP status at all), which is not the combination the prediction is about. |
| 8 | E19 (one counter event) | **UNTESTED** | **No doubler is in the corpus**: 0 records mention Kalonian Hydra, Mossborn Hydra, Ascendant Acolyte, Doubling Season, Corpsejack Menace, Branching Evolution, Hardened Scales or Winding Constrictor. The only counter traffic is loyalty, `+1/+1` from Luminarch Aspirant and Ranger Class level counters. |

**Tally: 12 PASS · 5 FAIL (2 by-the-letter only) · 11 UNTESTED · 2 mixed.**

---

## 3. Cross-deck engine / render / narration defects

### HIGH-1 — `latched_line_in_plan` fires on a line that is NOT in the plan block, and the fail-safe note is absent, so E6b/D9 is still unadjudicable

`130v126@1788653538` **seq 29** (attackers, t18). The reply is four lines:

```
ATTACK: A2, A3, A4
PLAN: Rorix flies over defenders for 6. … Opponent life goes from 21 to 13.
<blank>
ATTACK: A4
PLAN: Rorix flies for 6 damage. Opponent life 21 -> 15. …
```

The record: `latched_line: "ATTACK: A4"`, `latched_line_in_plan: true`, `answer_replaced: true`,
`parse_note: attack_last_line_taken`, `chosen_text: "Rorix Bladewing"`, and **no
`plan_answer_line_only`** — which lane AD names as the falsifier. Line 3 begins at column 0 after a
blank line, so `planParagraphBound` is classifying a second top-level answer as inside the plan
block. Two consequences: (a) `latched_line_in_plan` cannot be trusted as the field that settles D9
next wave, which was E6b's whole purpose; (b) if the bound is really this wide, a legitimate
outside-the-plan coded answer can be demoted with no record saying so — `plan_answer_line_ignored`
was written 0 times, so there is no counter for the demotion either. Repro: re-render
`findAnswerLabelLine` over that exact four-line reply and print the computed bound offset.

### HIGH-2 — the latched line is recorded but is still unverifiable against the reply it came from

11 of the 14 `answer_replaced` records carry a `latched_line` that does **not** appear anywhere in
the stored `reply`, because the reply was trimmed. Repro: `130v123@1788653511` **seq 30** —
`latched_line: "CHOICE: 1 (Cast Hammer of Bogardan)"`, `reply_trimmed_bytes: 5456`,
`coded_answers: 2`, and the string is absent from `reply`. Also seq 64, seq 67 (15,813 B trimmed),
`123v152@1788653513` seq 13, `130v146@1788653521` seq 46, `123v125@1788653523` seq 15 / 69,
`126v125@1788653534` seq 53, `130v126@1788653538` seq 34 / 49, `162v152@1788662989` seq 18.
E6a solved half the wave-62 problem (the line is no longer lost); the other half — *where in the
reply it stood*, which is what decides whether the latch was legitimate — is still unrecoverable.
A byte offset, or a short `reply_head`/`reply_tail` pair around the latch, would close it and cost
nothing near the trim budget.

### HIGH-3 — a 561-second reveal wait with no stall record

`152v146@1788653548` **seq 31**: `reveal_wait_secs: 561`, `reveal_wait_ticks: 144,213`, and no
`reveal_stall_forced` anywhere in the corpus. The other 14 reveal waits total 339 s (max 54 s), so
this one window is 62 % of all reveal wait time in 21 games — roughly 9.4 minutes of wall clock
inside a single seam. A per-deck reader sees one slow reveal; across the corpus it is the largest
non-hang time sink and the 20-second stall floor plainly did not release it. Repro: the record is
self-describing (`kind: reveal`, `choice: 1`, `chosen_text` naming the card); the question for the
engine is why the release path did not fire at ~20 s.

### MED-1 — the gang-block sum a reader sees first is 1/17th of the tag's real worst case

`126v162@1788653525` **seq 28**, row A1:

```
[their untapped blockers: these life prices SUM: each price below is for that ONE blocker, and
several of them may block this attacker together - (1 if all block), plus up to 16 more if they
take every "may" - they have 5 untapped creatures that could block this one, biggest Vampire (1/1)
(you kill it, your attacker lives (lifelink: they gain 1, …)); most expensive to attack into:
Perimeter Captain #1 (0/4) (neither dies (blocking trigger: they may gain 4, …))]
```

The only summed figure on the only multi-priced tag in the corpus is `(1 if all block)`, while the
largest single price printed in the same tag is `4`. The design reason is sound (certain vs "may"
kept apart), but the number that leads is the *smallest* one, and the wave-62 defect this item
exists for was a seat under-counting exactly here (146v126 seq 32: worst tag 5, opponent gained 9).
Either lead with the combined ceiling (`1 certain, up to 17 if they take every "may"`) or say in the
same breath that the leading figure excludes the "may" half.

### MED-2 — decision-time tags leak into `chosen_text`

9 records carry `{removes ` inside `chosen_text`; 4 of those are the new E9 crack-back clause:

```
123v130@1788653511 seq 47  chosen_text: "Goblin #1 (1/1) [doesn't untap …] [opponent's battlefield]
  {right now: -1/-1 - DIES} {removes 1 from the CRACK-BACK total above: 5 -> 4 - …"
```

also `125v146@1788653528` seq 15 and seq 129, `130v152@1788663228` seq 43 (crack-back clause), and
`146v162@1788653536` seq 31, `146v123@1788653544` seq 32, `130v123@1788653511` seq 31,
`123v146@1788653544` seq 14, `130v152@1788663228` seq 15 (`{removes whichever you target:`).
The **GAME LOG the model reads is clean** — 0 leaks — so no decision was harmed; this is a record
hygiene defect (`stripNarrationDecoration` is applied to the log but not to the stored
`chosen_text`), and it is what falsifies E9's own narration conjunct.

### MED-3 — the hold latch's honest promise still costs 13 same-turn re-asks a wave

The row promises *"do not ask me again - this turn or later - until one of the rows above changes"*
and, correctly, `(any change re-opens this window)`. The measured behaviour is that a **transient**
change discards the latch permanently: `130v162@1788653515` hold at seq 40 (3-row cast menu, t14),
a 5-row menu at seq 43, then the *identical* 3-row menu at seq 46, 48 and 49 — three re-asks of a
question the seat had already answered "hold" to, in the same turn, at the same seam. 13 such
sequences corpus-wide (wave 62: 15). Nothing is falsely claimed, so this is not a lying surface;
it is a cost: `mHoldRows.erase` throws away a key that becomes valid again seconds later. Keeping
retired keys per turn and re-honouring them on an exact re-match would recover these windows.

### MED-4 — one seat spent 14 minutes on a single answered window

`126v125@1788653534` **seq 14**: `latency_ms: 827,678`, `deadline_pct: 91.9` — inside the deadline,
so no timeout and no fallback, but nine minutes past the next-slowest answered window. Combined
with HIGH-3 and the one 900 s wall miss, three windows account for ~37 minutes of the corpus's wall
clock. No signature fires on any of them; there is no record field that flags "answered, but at
92 % of budget" for a later reader to sweep.

### LOW-1 — a self-correction survives the E14 cut when it opens with a bracket

`130v126@1788653538` **seq 35**, served plan: *"… Cast nothing right now. **(Wait,** the choice is to
cast Starstorm. The plan needs to reflect the decision. …"*. The closed word list is matched at a
sentence start; a leading `(` defeats it. 1 of 2,801, and lane AD's own note predicts exactly this
class.

### LOW-2 — `; converters on your battlefield:` is still unqualified in the string sense (30×)

All 30 sit on `oneShotDrawGrantTag` (`146v125@1788653528` seq 31, 51, 54 and 27 more), which lane AC
deliberately left alone; each is followed in-line by `(nothing of yours punishes their draws, so
those 1 draws take 0 off them)`, so the conflation deck162 named is not reproduced. Recorded only
because it is E13's stated falsifier and a future reader will re-find it.

### LOW-3 — `reveal` records still carry a constant `choice: 1`

15 of 15. Now honestly labelled (`choice_meaning: "selection size"`), and `chosen_text` names the
card, but a harvest that wants the row a reveal took still has to map back through `options_text`.
Lane AD chose this deliberately; noted so it is not re-found as a bug.

---

## 4. Owner's lategame specimen

Written verbatim to **`wave63/lategame-specimen.txt`** with the 3-line header:

* file `matchups-20260905-191148/1788653511-ai_baka_deck123-…-vs-ai_baka_deck130.jsonl`
* seq 95, turn 37, kind `attackers`, my_life 2, opp_life 13, 21,634 prompt bytes
* why: the lethal alpha strike that ENDED the game (`results.tsv` 123v130 → 130 at **-67**). The
  render carries the collapsed `A1-A20. Vampire #1-#20 (4/4) (printed 2/2)` range handle with its
  explanation, `They have 0 untapped creatures able to block`, and the `ATTACK TOTAL: 20 attackers
  listed, 80 total combat damage … that KILLS them whatever they block` verdict — a late window
  where the engine's collapse, its verdict and a 2-life seat all sit in one prompt.

---

## 5. What I did NOT check

* **I did not re-derive the hang.** `wave63/corpus-hang.md`'s mechanism (DescriptorTargetChooser vs
  designation markers) is cited, not re-verified; I did not run the probe, the fixtures or any
  binary.
* **No build, no suite, no PARSETEST, no git, no wagic run** — per the brief. So **E17 is
  unadjudicated by me**, and every lane's own gate figures are taken on trust.
* **`src/` was read, never edited.** I read `AIPlayerGPT.cpp` only around `holdKeyRow` /
  `holdHonoured` / `takeHold` (to learn that there are exactly two seams, `"priority"` and
  `"cast"`, which is what makes the E10 count meaningful) and around
  `planAssertedAbsentBlock` (to learn the exact withdrawal string to search for).
* **The untrimmed replies do not exist**, so E6a's "the latched line appears in the reply" conjunct
  is unverifiable in principle from this corpus — I verified only that the field is always present.
* **E1's whole header half, E3b, E7's D5 shape, R1, R5's positive half, R7's positive half, R9 and
  E19 had no window.** I report UNTESTED rather than inferring from adjacent evidence; several of
  these (E19 especially) need a corpus that contains the cards at all.
* **I did not verify card facts against Scryfall.** Every card claim above (Tundra/Scrubland/Bayou/
  Savannah/Underground Sea subtypes, Urborg's grant, Nadaar's lord) is checked against
  `bin/Res/sets/primitives/*.txt` only; Nadaar's Oracle line is quoted from the primitive's own
  `text=` field, which now matches the fixed `auto=`.
* **My E14(b) false-presence sweep is heuristic** (explicit `I control X` / `X is on the
  battlefield` phrases plus a name-membership test against the two battlefield lines). It misses
  abbreviated references ("my Lolth") and implicit claims ("both are out"), the same limitation
  lane AD names. A plan that lies without one of those phrases would not be caught.
* **The R6 / E3a reshuffle check covers permutations of the matched blockers over the matched
  attackers only** — it does not test replacing a matched blocker with an idle one, which is the
  other half of `improveAssignmentMaterial`.
* **HOLD take/render counts are prefix-matched on the rendered prompt**, per the brief; I did not
  cross-check them against `options` arrays, and a hold row rendered with different leading
  whitespace would be missed (none was: exactly one distinct hold row text exists corpus-wide).
* **The `.stderr` files were scanned by signature, not read.** A crash class that prints none of
  `Segmentation`/`SIGSEGV`/`core dumped`/`Assertion`/`abort` would not be seen.
* **The Vita softlock could not be confirmed or refuted** from a corpus of AI-vs-AI seats; nothing
  here should be read as evidence against the owner's report.
