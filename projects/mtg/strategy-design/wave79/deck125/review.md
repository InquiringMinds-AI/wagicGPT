# Wave 79 — deck 125 (Revelation Fracture, UW draw-go control)

Seat files: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-000748-final/*ai_baka_deck125-*`
(six seats, all with `gameend`, 0 timeout-tainted). SINGLE binary `wagic-5af6f7a95-w79step1`.

**1 W / 5 L** (wave 78: 3/6; wave 77: 3/6). 417 decision records = ask 310 + priority 87 + discard 16 +
forced_close 2 + bottom 2; `protocol_replies` 415, **reasoning on 415/415** (median 6,514 chars, max 14,226 —
this seat's peak keeps falling: 35,886 in w77, 17,078 in w78). Latency p50 94.4 s, p95 196.3 s, max 242.4 s,
11.9 h of decode over six games. `protocol_deviation`: compliant 407 / `unlabelled_plan` 8 / `plan_absent` 0 /
**`answer_label_absent` 0**; `off_protocol_bytes>0` 8 (1.9%, w78 2.0%), `plan_line_missing` 8,
`action_before_plan` 0, `reply_truncated` 0, **`answer_replaced` 0 — no heuristic answer on any seam**
(w78: all six corpus `answer_label_absent` fallbacks were on this deck). Gameend sums for the six seats:
`ask_replays_reserved` 361, `identical_ask_answers_reserved` 0, `ask_key_continuation_differs` 34,
`hold_released_turn` 28, `hold_windows_skipped` 446 (cast 242 / priority 204; w78 767),
`hold_verdict_safer_ignored` 7 (all `125v162`), `hold_reopened_new_threat` 0,
`crossphase_identical_reputs` 18 / `_board_unchanged` 4 (all `125v126`), `stack_drain_windows_asked` 8,
`forced_close_events` 2 / `_unrecorded` 1 (`125v162`), `phase2_answer_recovered`/`_missing` 0,
`stop_reached_*` 0, `own_loop_windows_asked` 0 / `own_loop_verdict_lines_rendered` 0,
`plan_names_uncastable_zone_card` 3, `plan_names_stranded_card` 0, `async_drops` 0, `repeat_annotated_takes` 0,
`menu_pass_no_progress_suppressed` 0. No hang, crash or timeout.

## 1. Game-by-game

| opp | result | turns | end life | the decision that decided it |
|---|---|---|---|---|
| 123 | **WIN** | 33 | 33–0 | Clean two-Staff clock. Staff #1 t13, Staff #2 t17, then 2 damage every upkeep to exactly 0 on t33. 1 Revelation (X=3, main phase), 1 counter, 0 self-pings, 8 land drops taken. Both Staffs ping as **separate rows in the same upkeep** from t19 on (`seq 77/78`, `87/88`, …) — the T12 ordinal fix is live and paying |
| 126 | LOSS | 20 | 0–44 | **`seq 44` → `seq 45`, t9 Main 2.** The seat announced Sphinx's Revelation at `max affordable X=2` with 20 life — Revelation **stop (4)** — then at the X menu re-read stop (4) and took **Decline**. That tapped all 5 lands for nothing (`seq 46`: `Mana available: 0 total (no untapped sources) … Already in pool: {u}{u}{w}{w}{w}`). On t10 the opponent cast **Sanguine Bond** and the seat was put **no window at all**: it had 0 untapped sources. `Fall of the Gavel {3}{u}{w}` (5, exactly payable off the 5 sources) was on the hand line, and the same prompt printed `Cards you have seen in the opponent's hand: Exquisite Blood; Sanguine Bond` and a `LOOP HALF PENDING:` paragraph. Bond + lifelink (Sorin) killed the seat from 29 in one turn. §3 edit 1 |
| 146 | LOSS | 41 | −2–16 | **`seq 104`/`105`, t37 Main 1, 13 life.** With its own Dream Fracture already on the stack aimed at Vanishing Verse, the seat cast Cancel and, at the target ask, named **`Dream Fracture [instant] [your stack]` — its own counterspell**. Log: `Your Dream Fracture was COUNTERED by Cancel`, no draw. Two counters and 6 mana for nothing; Vanishing Verse resolved. §2 A-1, §3 edit 2. Earlier: Path/sweeper/counters all on-list; the game was close (15–15 at t34) |
| 162 | LOSS | 37 | 0–18 | Ground down by draw punishers. **`seq 242`, t32:** with Forced Fruition, Ob Nixilis and Underworld Dreams all out, the seat cast Cancel on Teferi's Puzzle Box off a row reading `[DRAW PRICE: casting this draws YOU 7 cards … deal you 14 - you would be at 6]`. §PUNISHER BOARDS' carve-out (`at any K above 1 when the spell countered has draw text of its own`) licensed it and the reasoning's arithmetic is sound (the Box would have cost the same 14 **every** draw step). 20 → 6, dead five turns later. **Upstream**: t28 `seq 199` the seat held at Upkeep and Underworld Dreams + Forced Fruition both resolved with no window — but its hand that window was Verdict ×2 / Emrakul / Lightmine Field / Revelation, **no counter**, so the hold cost no castable answer. §2 B-2 |
| 152 | LOSS | 19 | 0–20 | **`seq 9` → `seq 10`, t8 Main 1**, the same announce-then-Decline at `max affordable X=1`: `Sphinx's Revelation was NOT cast: you declined after the payment above … the 4 mana that paid for it is still floating, unspent`. Sigarda, Champion of Light resolved on t9 against a tapped-out seat. From t15 the board outran the deck (8 damage t17). §3 edit 1 |
| 130 | LOSS | 18 | −2–20 | Variance: **mulligan to four**. `seq 2`/`3` were one-land sevens (guide: ONE LAND → SHIP a seven and a six — correct); `seq 4` was 5 lands / 2 spells with `would not cover any spell in it` at a five-card keep, shipped to four. Kept 2 lands + 5 spells, bottomed Emrakul / Supreme Verdict / Path. Rorix Bladewing (6/5 flying haste) at t15 ended it. No guide item is bought |

## 2. Engine / interface / card items

**HIGH — A-1. The counterspell TARGET ask offers the seat's OWN spell on the stack with no marker that
taking it is self-destruction, and the cast row that opened the ask under-reported the target set.**
Repro `1789189706-ai_baka_deck125-…-vs-ai_baka_deck146.jsonl`:
- `seq 104` cast row: `Cast Cancel {1}{u}{u} {leaves 1 of your 4 untapped mana sources untapped} … - can target on the stack: Vanishing Verse {b}{w} (instant) …` — **names one target**.
- `seq 105` target ask rows: `0. Vanishing Verse [instant] [opponent's stack]` / `1. Dream Fracture [instant] [your stack] - "Counter target spell. Its controller draws a card. -- Draw a card."` — **two**.
The seat took row 1. The `[your stack]` tag is a location marker, not a warning, and the row reprints the
card's full text, which reads as if the draw still happens. The model's stated reason is exactly that:
*"If I target Dream Fracture with Cancel, it resolves first. Dream Fracture is countered. Its controller (me)
draws a card."* — false; a countered spell's text never resolves, and the log confirms no draw.
Fix, engine side: (i) a row whose spell is the SEAT'S OWN carries the same class of clause the wave-78 S5
cast-row guard carries, e.g. `[this is YOUR OWN spell - countering it spends both cards and none of its text
happens]`; and (ii) the cast row's `can target on the stack:` clause lists every target the ask will offer,
or says how many. 20 counters cast on this seat, 18 on-list; this is the only self-target.

**MED — B-1. A hand-line tag states a false fact and points at a block that is not printed.**
23 prompts on this seat carry `[no cast row now: sorcery speed - only in your own main phase with an empty
stack; … not a main phase **and the stack is not empty (see ON THE STACK above)**]` in a prompt whose
CURRENT SITUATION block has **no `ON THE STACK, waiting to resolve` section at all** — the stack is empty.
61 other prompts carry the same clause with a stack block present (true). Repro
`…vs-ai_baka_deck162.jsonl` **seq 199** (t28 Upkeep, no stack block, four hand cards each tagged that way);
also `…vs-ai_baka_deck152.jsonl` seq 58, `…vs-ai_baka_deck123.jsonl` seq 11/25/26/48,
`…vs-ai_baka_deck130.jsonl` seq 20/21, `…vs-ai_baka_deck162.jsonl` seq 41/42/63/64/65/67/83/104.
Fix: the "and the stack is not empty" half is conditional on the stack block existing; at an empty stack the
tag should read the phase reason alone. Cheap, and it is a render clause asserting a false fact — the class
the brief flags first.

**MED — B-2. The hold latch suppressed a whole opponent turn's casting windows six times; none cost a
castable answer this wave, but the row's own promise is not kept.** The hold row says
`it stands until your next turn begins, **or until one of the rows above changes (any change re-opens this
window)**`. Six holds taken at an EMPTY-stack window of the opponent's turn were followed, that same turn,
by opponent spells resolving with **no further window put to the seat**:
`162 seq 199` (t28 → Underworld Dreams + Forced Fruition, both on the guide's COUNTER-ON-SIGHT list),
`152 seq 58` (t15 → Katilda, Dawnhart Prime + Brutal Cathar), `126 seq 36` (t8 → Idyllic Tutor, also
COUNTER-ON-SIGHT), `126 seq 8` (t4 → Wall of Omens), and two (`162 seq 63`, `146 seq 66`) where the window
DID re-open in the same turn. I checked the hand and mana at each of the four suppressed ones and **every one
had no payable counter** (`199`: no counter in hand; `58`: no counter in hand; `36`: Gavel needs 5, four
sources; `8`: turn 4). So the price this wave is zero — but the suppression is not conditional on that, and a
spell arriving on the stack is a row appearing. `hold_reopened_new_threat` 0 on all six seats: Underworld
Dreams and Forced Fruition arriving under a live hold is exactly the DC-F2 state, and it did arise
(`162 seq 199`, t28), so that counter reading 0 is a FAIL on this seat's evidence, not UNTESTED.

**MED — B-3. `Decline` at the X menu is a real mana sink and the cast row does not price it.** Two
occurrences, both decisive-adjacent (`126 seq 44/45`, `152 seq 9/10`). The X-menu Decline row does say
`{declining strands the … mana already paid}` and the game log says so plainly, but the **cast row** — the
window where the stops are actually answerable — prints only `{no {leaves ...} count on this row: what it
spends depends on the X you announce at the next window}`. Engine suggestion: the Revelation cast row states
that taking it pays the mana whatever X is then chosen, and that the menu has no refund row. Guide side this
is edit 1.

**LOW — B-4. T10 prompt bytes: the trim paid, hard.** 417 prompts, p50 11,687 B, **max 25,471 B**
(`125v146`), **49 of 417 = 11.8% over 20 KB against 44.4% in wave 78**; per game 146 40% over, 162 15%,
123/126/130/152 **0%**. Peak down from 34,778 B. I sampled 30 of the 121 `(...more)` cuts and found **0
mid-clause cuts** and no false statement; I did not re-check the wave-78 clause-ranking complaint (no
`venture`/trigger clause was cut on this seat's samples).

**Verified TRUE / FIXED on this seat, with the evidence:**
**T8(a) PASS.** `HELD: this is an instant, so it is in your hand and castable the moment a legal target
appears, including on their turn; this tag is about THIS window, never about your hand` renders on **267**
prompts, appended to the `[no cast row now: it must have a target …]` tag. The wave-78 reasoning shape
("I have no counterspells in hand right now" with two Gavels on the hand line) does **not** recur anywhere in
415 replies.
**T8(b) PASS.** 62 keep-X clauses, every one reading `Holding up a counterspell: <name> …` and naming a
COUNTERSPELL — Dream Fracture 25, Essence Scatter 20, Fall of the Gavel 14, Cancel 3. **Path to Exile: 0**
(wave 78: 10 of 47). Path still appears, correctly labelled as a separate `Path to Exile {w} in your hand
needs 1: the largest X that still leaves it payable this turn is X=N` clause beside it.
**T12 PASS on the Staff case.** `Staff of Nin #1` and `Staff of Nin #2` are distinct rows and both are taken
in the same upkeep from `123 seq 77/78` onward — the wave-78 over-merge (`Staff of Nin` → `Staff of Nin #2`
counted as the SAME row, `125v123 seq 263`) does not recur. 369 `[hold check:` brackets, all with a resolving
`hold_check_ref_seq` or the first-window text.
**Wave-78 edit 4 PASS (and the guide was the cause, on this seat's evidence).** 397 of 397 non-PUT replies
carry `CHOICE:`; 18 PUT replies; **`answer_label_absent` 0** against 6 in wave 78, all six of which were this
deck. The bare `<n> (<name>)` form does not appear once. The owner's 2026-09-12 ruling stands separately —
the parser is to be robust — so this is now a belt-and-braces result, not a dependency.
**Wave-78 edit 1 PASS (the no-cast-row tag as gate price).** No window on this seat reads a counterspell
under that tag as absent; 20 counters cast, 18 on-list.
**Wave-78 edit 3 PASS (K-of-1 sweeper split).** 6 sweepers cast, **all at K of 2 or 3** (`destroys 2` ×5,
`exiles 3` ×1) — 0 at K of 1 and 0 at K of 0. The wave-78 failure (the model quoting the K-of-1 sentence five
times and casting anyway at power 3) does not recur; no K-of-1 sweeper was cast at all.
**Wave-77 edit 1 PASS (Revelation stop (5)).** 9 Revelation announces, **all 9 in a main phase**, 0 at an
upkeep or draw window — third wave running.
**Wave-77 edit 3 PASS (own-turn hold).** 249 own-turn windows offering a hold row, **30 holds** — 26 under the
printed `[you declined this exact list N times already this turn]` override and **4 uncovered**
(`123 seq 12`, `123 seq 56`, `162 seq 70`, `146 seq 59`), against 4 in wave 78 and 21 in wave 77.
**Guide compliance elsewhere:** 61 land windows / **60 lands played**, the one decline (`162 seq 266`, t35
Main 1) recovered in the same turn at `seq 273`; **31 pings, 0 self-pings**, 25 at the face and 6 at a body
(Ob Nixilis ×5, Fate Unraveler ×1 — both printed DRAW PUNISHERS, the guide's own exception); **0 Emrakul
casts** (0 rows offered); 5 Elixir activations; 16 cleanups, **no Staff sent**.
**§THEIR CONVERTER AND THEIR LOOP was TESTED for the first time in seven waves** and held: a new
`LOOP HALF PENDING: Sanguine Bond is on THEIR battlefield and the other half of the pair, Exquisite Blood, is
in their hand …` paragraph printed on 9 windows of `125v126` and `LIFE-TO-DAMAGE CONVERTER … Sanguine Bond`
on 25. Both statements are TRUE against that board, and the seat **paid no life of its own** in any of them
and countered Exquisite Blood on sight (`126 seq 64`, Fall of the Gavel).

## 3. Guide verdict: **EDIT** (`wave79/deck125/strategy.txt`, **19,998 bytes** against the live 19,997 — it
grows by 1. The two rules are paid out of six prose trims, none of which removes a decision rule.)

| # | before → after | paid by |
|---|---|---|
| 1 | **§SPHINX'S REVELATION's stop block gains the window the stops are answered at.** `… the answer stays "Cast nothing right now" when the Revelation is the only row printed.` **THE LOW-LIFE FLOOR …** → `… the answer stays "Cast nothing right now" when the Revelation is the only row printed. ANSWER THEM AT THE CAST ROW: taking that row PAYS THE MANA and the X menu it opens gives none back - announce and then Decline and your turn's mana is spent on nothing, with no untapped source left for their turn.` **THE LOW-LIFE FLOOR …** | **`126 seq 44`, the game, and `152 seq 9`.** At both cast rows the model quotes stop (4) verbatim and casts anyway — `"The guide says 'STOP (4) … reads 2 or less and your printed life is above 10'. This means I shouldn't cast it. But I have to… The guide is a suggestion"` (126) — then at the X menu it re-reads the same stop and Declines (`"The guide explicitly says stop (4) is a hard stop. I will trust the guide."`). It is not misreading the stop; it is not seeing, at the cast row, that the cast row is where the stop costs something. The sentence that said so lived 2,100 bytes downstream at the end of THE X MENU (`A DECLINE HERE IS NOT FREE: … answer the stops at the CAST row, before this menu opens`) — a window the model only reaches after it has already paid. The edit moves that fact into the stop block and states the price in the same breath. Per the wave-74 ruling I do **not** re-capitalise stop (4) itself: it was read correctly both times |
| 2 | **§COUNTERSPELLS gains the self-target rule.** `Your printed life is 12 or less: take Fall of the Gavel. An "INCOMING THIS COMBAT …` → `Your printed life is 12 or less: take Fall of the Gavel. AT THE TARGET ASK A ROW MARKED "[your stack]" IS YOUR OWN SPELL: never name it - countering your own counter spends both cards and its text never happens. An "INCOMING THIS COMBAT …` | **`146 seq 105`.** The seat named `Dream Fracture [instant] [your stack]` with Cancel on a false premise it stated in full (*"Its controller (me) draws a card"*). No rule anywhere in the guide says the target ask can offer your own spell, and none of the four counterspells in this deck has any use pointed at one. The rule is unconditional, cannot misfire, and answers the ask at the ask |
| 3 | Byte payers, no decision rule lost: `26 lands.` from the deck header (derivable from the battlefield line and never referenced); `Every cast row prints what it leaves untapped.` from §THE TAP-OUT GATE (the rows print it — the sentence only restates the screen); `, and you never send it at cleanup` from §STAFF (§CLEANUP step 6 is the rule and it holds — 16 cleanups, no Staff sent); `Every counterspell you can pay for has its own row, so COUNT THE ROWS:` → `COUNT THE COUNTER ROWS:`; `- "DISCARD PUNISHERS on the battlefield: theirs": Revelation stop (2) covers it.` from §PUNISHER BOARDS (a pure cross-reference — §ORDER step 4 carries the discard rule and stop (2) is printed in the stop list); the old X-menu tail sentence, now relocated by edit 1 | bytes |

**Nothing else is edited, and three candidates are explicitly REJECTED.**
- The §PUNISHER BOARDS Forced-Fruition carve-out (`at any K above 1 when the spell countered has draw text of
  its own`) is what licensed `162 seq 242` and the 14-life bill that lost the game — but the model's own
  arithmetic in that window is right: an uncountered Teferi's Puzzle Box charges the same 14 at **every**
  draw step. One window, and the alternative is worse; the evidence does not pay for a change.
- The Overgrown Battlement counter (`126 seq 92`) is licensed by the `{T}:` carve-out under the [defender]
  rule and it did cost a Scatter for a mana wall, but the same carve-out is the only thing that stops mana
  dorks and venture bodies resolving; one instance does not buy a re-write.
- There is no guide line for an opposing REMOVAL spell aimed at the seat's own permanents (Vanishing Verse,
  `146 seq 103/104`). One game, and the first counter on it was defensible; the second was the self-target
  bug edit 2 answers.

## 4. Not checked
No `blockers`, `attackers` or `reveal` seam — this deck fielded no creature in any of the six games and was
asked none of those; 2 `bottom` records and 4 mulligan asks only (all in `125v130` and `125v126`), so most of
§MULLIGAN's ship branches ran once or not at all. No `DISCARD PUNISHERS` paragraph and no
`{... CLEANUP PRICE at X=N ...}` tail appeared in any of the 417 prompts, so §ORDER step 4 and Revelation
stop (2) are **UNTESTED for a seventh wave**; I trimmed no rule there beyond the cross-reference line.
`Forced Fruition` printed on 33 prompts and the LOOP/CONVERTER paragraphs on 34, so those sections ARE tested
— but `own_loop_verdict` lines: **0 rendered on this seat, `own_loop_windows_asked` 0**, so T4/DC-F10 is
UNTESTED here. `stop_reached_*` 0 on all six seats (T2 UNTESTED here). I did not open `askreplay/`, did not
reconcile `ask_replays_reserved` 361 vs `identical_ask_answers_reserved` 0, did not sample the 34
`ask_key_continuation_differs` windows, did not reconcile the 1 `forced_close_unrecorded` or the 2
`forced_close_events` against stderr, and did not verify the 4 `crossphase_board_unchanged` windows — all
engine-seat work. I verified no card against `primitives/*.txt` or an external Oracle source this wave:
Dream Fracture, Vanishing Verse, Teferi's Puzzle Box, Forced Fruition, Sanguine Bond and Ob Nixilis are
quoted from the engine's own `{target text:}` / `{effect:}` renders and from the game log. I opened opponent
seat logs only for the results row and the shared game log; I reconstruct no opponent's line of play. I
adjudicated only the lane predictions my own windows touch (T8, T10, T11, T12, and DC F2 as a FAIL) — the
rest is the engine seat's job. I made no general-guide and no strategy-writing-skill proposal: both edits are
inside this deck's own guide, edit 1 relocates a sentence that already existed, and edit 2 is a new rule for
a seam (the counterspell target ask) no other deck in the pool has.
