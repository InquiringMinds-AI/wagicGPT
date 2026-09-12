# Wave-78 deck126 review - SANGUINE BLOOD (WGB wall prison, Exquisite Blood + Sanguine Bond)

Corpus `matchups-20260911-125420-final`, SINGLE binary `wagic-47ef378c8-w78step1`, one run, all 21
games natural. **4/6** (wave 77: 1/6; 76: 3/6; 75: 2/6). The live guide under test is the wave-77
edition installed at 031db297b (19,901 B). **Hands are real - I do not read 1/6 -> 4/6 as guide
effect**; what actually changed is that the pair COMPLETED in four games this wave (it completed in
one last wave), and in all four the seat pressed the win button in the first window that offered it.

**Seat census.** 6 files, 323 records, **309 with a prompt**. Kinds: ask 250 / priority 42 /
attackers 11 / blockers 3 / reveal 4 / bottom 1 / forced_close 2 (+ gamestart/system/gameend).
`thinking: on` on all 315 records carrying the field; **309/309 replies with native reasoning**
(median 6,223 chars, max 15,228) - invariant 000 holds at this seat. **0 fallbacks**: 0
`unparsed_reply`, 0 `recovery`, 0 `defer`, 0 `reply_truncated`, 0 `action_before_plan`, 0
`answer_label_absent`. `protocol_deviation_replies` **3**: 1 `unlabelled_plan` (`126v125` seq 205,
reply opens `THE PLAN:` - 93 `off_protocol_bytes`, parser still found the action) and 2
`plan_absent` (`126v130` seq 48 `ATTACK: A1`, `126v162` seq 15 `BLOCKS: B1:A1, B2:A1, B3:A1` - both
the correct answer, both on a winning turn). Transport `curl=0,http=200,empty=0` on 309/309;
latency p50 **71.4 s**, p95 186.0 s, max 349.3 s; `deadline_pct` p50 7.9, **max 38.8** - no deadline
pressure anywhere. `async_drops` 6 (all `126v125`), every one carrying
`async_drop_events: ["casting/question and board/re-asked"]` - **all six re-asked**.

Counters, per game (123/125/152/146/130/162): `hold_windows_skipped` 17/1334/0/0/7/0 (cast
17/1324/0/0/5/0, priority 0/10/0/0/2/0); `hold_released_turn` 1/34/0/0/1/0;
`ask_replays_reserved` 11/80/0/0/22/11; `identical_ask_answers_reserved` 0/41/0/0/22/0;
`mana_only_windows_skipped` 14/64/0/4/**156**/**129**; `own_turn_windows_skipped` 37/8/0/0/44/28;
`main_phase_windows_skipped` 0/0/0/0/4/2; `crossphase_identical_reputs` 0/3/0/0/0/0 with
**`crossphase_board_unchanged` 0 in all six**; `plan_names_uncastable_zone_card` 0/**27**/0/0/0/0;
`stack_drain_windows_asked` 0/**16**/0/0/0/0; `own_loop_windows_asked` 0/0/0/0/**1**/**1**;
`forced_close_events` **2** (123) / `forced_close_unrecorded` 1 (123); `blocker_forecast_rows`
0/0/0/0/4/2, `_multi` 0/0/0/0/0/2. **0 in all six**: `stop_reached_windows_skipped`,
`stop_reached_reputs_collapsed`, `phase2_answer_recovered`, `phase2_answer_missing`,
`declined_face_latches`, `repeat_annotated_takes`, `identical_option_asks_resolved`,
`menu_pass_no_progress`, `wall_miss_events`, every `force_close_*` arm-bound counter.

**Play quality.** Land drop **57 of 57 (100%)**. THE ZERO STOP: **135 exposures, 0 takes**. Dead
menus (`NO LIVE CAST ROW` or `[you declined this exact list N times]`): 104, hold row taken **91**,
`Cast nothing right now` 9, a live activated row 4 (all four are Sorin `+1` or a priority `pass` on
a menu that printed a live row - correct by the guide, not violations). Max `[you declined this
exact list N times]` reached **N=4** (wave 77: 1; wave 76: 31). **Two rules WERE violated**, both
stated absolutely in the live guide and both quoted back by the model in the same trace: the seat
answered `Hold priority` on **7** menus that printed a live Sorin loyalty row (`NEVER HOLD ON A MENU
THAT PRINTS ONE`) - `126v125` seqs 96, 124, 149, 250, 260, `126v130` seq 31, `126v162` seq 35 - and
it wrote `ATTACK: none` once under a complete loop (`126v130` seq 32, §1 game 5). Sorin's `-2` was
offered on **57 menu rows** across three games and taken **0** times; the `+1` was taken in every
window where the seat did not hold. Max asks in one (turn, phase) is
**3-5 in five of six games**; the outlier is `126v125` **12 at t68 Main 1**, a genuine counter-war
(Idyllic Tutor -> Dream Fracture -> Sorin -> Cancel), board moving on every window.

## 1. Game by game

| # | opp | result | deciding decisions |
|---|-----|--------|--------------------|
| 1 | **123** | **WON** 32 to 0, t13 | Textbook. Tutor t5 (seq 8) -> Sanguine Bond; Bond t9 (seq 14); Exquisite Blood t11 (seq 16); `LOOP COMPLETE:` header first prints at seq 17; Staff of Nin t13 (seq 19); **seq 32 `Deal 1 damage with Staff of Nin targeting the opponent`** closed it 20 -> 0 in one window. Entries 1 and 8 of the cast list by the letter, and the win button's third bullet in the first window that offered it. |
| 2 | **125** | **LOST** 4 to 76, t86 | **Control deck, no decision at fault.** Exquisite Blood landed t10 (seq 15) and stayed to the end; Sanguine Bond was cast FOUR times and countered every time (seq 29 t16; seq 194 Tutor -> Dream Fracture; seq 198 Sorin -> countered; **seq 255 t82 Bond -> `opponent's Cancel {1}{u}{u} ... targeting Sanguine Bond`** at seq 256, with 22 untapped sources and no answer in the deck). The seat held 4 Tributes on a board that never had a creature (`at 0 this does nothing` on every one). The one real cost is §2 HIGH-2: five attacks into Lightmine Field. |
| 3 | **152** | **WON** 28 to 0, t18 | Mulligan a 1-land seven (seq 2, guide's `Zero or one land: SHIP on a seven`), keep the 3-land seven, bottom the duplicate Wall of Omens (seq 4) - the bottoming order exactly. Two Tributes on NAMED non-defender victims (seq 10 Briarbridge Tracker +3 life, seq 12 Brutal Cathar +2) - the "their choice is your choice" branch, cast in the window offered. Blood t12 (seq 15), Staff t14 (seq 17), **Bond t16 (seq 21)**, then **seq 23 the Staff ping on the opponent** from `LOOP COMPLETE` -> 18 to 0. |
| 4 | **146** | **LOST** 0 to 17, t15 | **The one decidable loss, and the guide paid for it - see §3.** Sanguine Bond cast t10 (seq 12, correctly preferring the untagged half over the Silencer-priced Blood) was EXILED by Vanishing Verse on t11. At **seq 19 (t14, life 5)** the header read `total to subtract from your life is 7 and you would be at -2; that would KILL you`, row 2 was `Cast Pride Guardian` whose own clause read `Counting those bodies AND the checked new ones you cover 4 of 4, leaving 0 -> you would be at 5`, and row 1 was `Cast Exquisite Blood` carrying `[NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life ... you would be at 2]` and `{paying this taps: Overgrown Battlement - it cannot block on their turn}`. The seat took row 1: it paid 3 life to 2, tapped its ONLY blocker, and put a Blood on the board whose partner was in exile. seq 20 then cast Pride Guardian at 2 life covering 3 of 4. Dead t15. |
| 5 | **130** | **WON** 31 to 0, t19 | Tutor t9 (seq 11/12) -> Sanguine Bond; Blood was already down from t7 (seq 9); Bond t11 (seq 14); `LOOP COMPLETE` from seq 15. Sorin t15 (seq 16), `+1` in the very next window (seq 17), **seq 48 attack with the lifelink Vampire** -> 31 to 0. **Two turns later than it had to be**: at **seq 32 (t17 Attackers)** the same Vampire was already able to attack, the A-line read `A1. Vampire (1/1) [lifelink] [held back, THIS creature could not block ANY of their 1 creatures: Rorix Bladewing (flying)] [their untapped blockers: Rorix Bladewing (6/5) (your attacker dies, their blocker lives (lifelink: you gain 1 from this block ...` and the header read `1 of that damage is LIFELINK, and your LIFE-TO-DAMAGE CONVERTER turns the life it gains you into that much life off them` - the guide's `A Vampire the line already calls unusable ("held back, THIS creature could not block ANY of ...") costs you nothing: send it` matched the printed string byte for byte. The seat answered `ATTACK: none`, and at seq 31 it had also held over a live Sorin `+1`. Cost: 2 turns in a game it won anyway. |
| 6 | **162** | **WON** 32 to 0, t13 | Blood t7 (seq 9), Tutor t9 (seq 11/12) -> Bond, **seq 15 triple-block `B1:A1, B2:A1, B3:A1`** with Perimeter Captain out and seq 16 taking all three `Gain 2 life` triggers (18 -> 24) - STEP 2's gang-block and the Captain rule together. Bond t11 (seq 18), Sorin (seq 19), `+1` (seq 20), **seq 36 attack with the Vampire** -> 32 to 0. This is the game where a `DRAW FORECAST` of `5 cards ... = 5 life LOST BY YOU` was correctly read as irrelevant: the header's own `LOOP SCOPE:` clause says the seat's own life loss does not enter the loop. |

**Where the guide earned the four wins.** All four closed on the WIN BUTTON's bullet list in the
FIRST window that offered it (123 Staff ping, 152 Staff ping, 130 Vampire attack, 162 Vampire
attack) - 0 windows wasted on a wall after `LOOP COMPLETE` printed. Entry 1 beat every wall,
Lantern and Tutor on every menu where a missing half was castable. Entry 8 (Staff/Sorin to the top
once both names are out) fired in 123 and 152.

## 2. Engine / interface / card items

**HIGH-1 - S6 / CY F4 DID NOT LAND: `[own loop verdict:]` STILL RENDERS NOWHERE, AND THE WAVE-77
PER-ROW TAG IS GONE TOO, SO THE COUNTER IS NOW WHOLLY UNOBSERVABLE.** Repro: `own_loop_windows_asked`
is **1 in `126v130` and 1 in `126v162`** - the corpus's only 2 - and a /usr/bin/grep over **all 42
seat logs** returns **0** for each of `own loop verdict`, `RESOLVING`, `THREATENED`, `UNPROVEN`,
`resolving on its own`. Wave 77 at least had the per-row tag
(`[your loop is resolving on its own - this row is not needed to win it ...]`, 1 occurrence); this
wave that string is **0 corpus-wide** as well. Worse, the two windows cannot even be LOCATED from
the seat log: `126v130` contains **no `ON THE STACK` block in any of its 25 prompts**, and
`126v162`'s only stack window is seq 16 (t10 Blockers, two Perimeter Captain triggers) which is
three turns BEFORE the pair completed at t11. So the counter fires on windows with no loop object
on any rendered surface, and there is no per-record field to join on. CY F4's stated risk (the
UNPROVEN face silencing a true RESOLVING verdict) **cannot be checked at all** - nothing is
printed to be wrong. Falsification: any prompt in the corpus containing the substring
`own loop verdict`. Fix shape (unchanged from wave 77's HIGH-1): emit the verdict as a prompt LINE
outside the ask key, and put a per-record `own_loop_verdict` field on the asked record so the
window is joinable.

**HIGH-2 - THE `ATTACK TOTAL` HEADER NAMES LIGHTMINE FIELD BUT NEVER RESOLVES IT AGAINST THE SET
BEING DECLARED, AND THE SEAT ATTACKED INTO IT FIVE TIMES FOR ZERO DAMAGE.** Repro
`1789149271-ai_baka_deck126-0x559b61e71100-vs-ai_baka_deck125.jsonl` **seqs 83, 95, 116, 120, 125**
(t42, t46, t50, t52, t54). Every one prints
`ATTACK TOTAL: 1 attacker listed, 1 total combat damage to a player - declaring all of them with
none blocked puts them at 40 ... Lightmine Field fires on your declaration, before any combat
damage, and can remove attackers from that total - so it is NOT a floor on what lands ... how much
it deals depends on how many you declare, so it is not folded into either number.` The engine
knows both halves of the arithmetic at render time: the declared set is 1 attacker, so Lightmine
deals 1 to each, and the only attacker is a **1/1** Vampire token. It dies before damage. The
opponent's life is **41 at seq 83 and still 41 at seq 151** - five declarations, **0 damage, 0
lifelink gain, 5 Vampires fed to the enchantment**. The clause that invites this is the explicit
refusal to fold the number in; the model's own words at seq 116 are
`there are no blockers and the life gain is free`, and at seq 116 the echoed plan from one window
earlier already read `attacking into Lightmine Field is unprofitable`. Fix shape: for the set
actually offered, state the outcome - `declaring these 1 attackers deals 1 to EACH of them:
Vampire #1 (1/1) DIES on the declaration, so 0 of the 1 lands`. The guide already says ATTACK: none
here (see §3 REJECTED), so the missing surface is the arithmetic, not the rule.

**MED-3 - S13 / CY F8 DID NOT FIRE ON A SHARED-SOURCE TARGET MENU: THE SAME `{card text:}` IS
REPEATED ON ELEVEN ROWS.** Repro `1789149275-ai_baka_deck126-0x559533726ca0-vs-ai_baka_deck152.jsonl`
**seq 23** (t18 Upkeep, 21,074 B). Rows 1-11 are all `Deal 1 damage with Staff of Nin targeting
<X>` from ONE source, and every one of the eleven carries
`{card text: "At the beginning of your upkeep, draw a card. -- {T}: Staff of Nin deals 1 damage to
any target."}` - ~1.2 KB of a 21 KB prompt. The same shape at `126v152` seq 18 (8 rows), seq 19 (9
rows) and `126v123` seq 32 (5 rows). **No prompt at this seat carries a hoisted header** (0 hits
for a shared-text line above the rows). This is the wave-74 CG shape reported for the FOURTH time.

**MED-4 - PROMPT BYTES: MAX 32,777 B AT THIS SEAT, 131 of 309 OVER 20 KB, AND 71% OF THE LARGEST
PROMPT IS THE GAME LOG.** Repro `126v125` **seq 279** (t84): total 32,777 B = GAME LOG **23,156 B**
+ 9,596 B of everything else. Wave 77 at this seat: max 22,273 B, 7 of 190 over 20 KB. The growth
is a long game (86 turns) against an unbounded log, not the S13 target. Two compressible terms are
visible in the same prompt: the per-source mana sub-list is **1,379 B** enumerating 28 sources that
all read `{w} or {b} or {g} or {r} or {u}` (four Chromatic Lanterns make them identical), and the
battlefield line is 1,273 B for 32 permanents. 102 of 309 prompts are over the 24,000 B trim
trigger, all in this one game.

**MED-5 - `stack_drain_windows_asked` COUNTS 16 IN `126v125` BUT ONLY 10 WINDOWS RENDER THE
ANNOTATION.** The 10 that render are seqs 182, 191, 207, 212, 218, 236, 242, 246, 252, 262 (all
Upkeep). Six increments have no rendered clause and no per-record field, so they cannot be named.
The 10 that DO render are all **TRUE and CY F2-clean**: each prints
`[their stack is draining 2 triggers - each link will put this same list to you; HOLD (Hold
priority) covers every link. The rows above are what is legal NOW - this says nothing about what
will still be legal after their stack resolves]` against a stack holding exactly two of THEIR
triggered abilities (`Staff of Nin's deal 1 damage` + `Staff of Nin's Draw 1`), the "stays
available after" promise is **gone**, and the seat held on all 10. **No seat held across a drain
and lost a legal option** - the triggers create no option for this seat.

**MED-6 - `{effect:}` GLOSS: ONE MID-CLAUSE CUT IN 29, AND IT USES A BARE ELLIPSIS INSTEAD OF
`(...more)`.** Repro `126v152` **seq 14**: Sigarda, Champion of Light renders
`"Flying, trample -- Humans you control get +1/+1. -- Coven - Whenever Sigarda attacks, if you
control three or more creatures with different powers, look at the top five..."` - cut inside the
Coven clause, with `...` and no marker. The other 28 distinct glosses at this seat are complete or
cut at a clause boundary with `(...more)` (23 markers, all after a whole clause/unit). **The two
wave-77 S8 flagships are FIXED and verified here**: Howling Mine renders in full
(`At the beginning of each player's draw step, if Howling Mine is untapped, that player draws an
additional card.`) and Brutal Cathar keeps its return clause
(`... exile target creature an opponent controls until this creature leaves the battlefield.`).

**LOW-7 - `chain_selfharm_rows` IS 0 ON A MENU THAT OFFERS THREE SELF-HARM ROWS.** `126v152` gameend
carries `chain_selfharm_rows: 0` / `chain_acting_rows: 0`, yet seq 23's menu prints rows 3, 7 and 11
as `targeting Pride Guardian [your battlefield] {this hits YOUR permanent}`,
`targeting Wall of Omens [your battlefield]` and `targeting you {right now: takes 1 damage - you
would be at 10}`. The rows are correctly TAGGED; the counter does not see them.

**LOW-8 - `THE PLAN:` DEFEATS THE PLAN-LABEL PARSER.** `126v125` seq 205, reply
`\n\nTHE PLAN: Cast Idyllic Tutor ...\nCHOICE: 2 (Cast Idyllic Tutor)` -> `plan_line_missing: true`,
`off_protocol_bytes: 93`, class `unlabelled_plan`. The action was still found and played; report
only - this is S16 territory and the standing ruling forbids widening tolerance.

**Wave-77 items, re-checked on this corpus.**
- **S7/S9 (`hold_check_ref_seq` was a window ordinal; the count over-counted; disappearances never
  reported) - FIXED, verified, 0 false.** Every asked record now carries BOTH
  `hold_check_ref_seq` (a real record `seq`) and `hold_check_ref_window` (the ordinal), and `-1/-1`
  on a first window. **278 brackets at this seat: 17 first-window, 261 adjudicated, 0 FALSE.**
  Method: rows extracted from the menu, balanced `{...}`/`[...]` groups stripped, hold/decline/pass
  rows excluded, seam taken from the menu header. Every one of the 261 refs resolved to a record,
  **every one at the SAME seam**, and every `N new` / `N gone` figure matched. All 17 first-window
  sentences are genuine (no earlier record at that seam in the file). The `N gone` clause is new
  this wave and is true on every print. The wave-77 HIGH-2 trap (two adjacent fields on different
  axes) is closed. **UNTESTED here**: the `Goblin` -> `Goblin #1` renumbering case - no row handle
  at this seat ever renumbered.
- **S11 (`forced_close_unrecorded` unattributable) - FIXED at this seat.** The 2 `forced_close`
  records in `126v123` are seq 4 `{"arm":"land","event":1,"outcome":"armed","window_seq":2}` and
  seq 5 `{"arm":"land","event":2,"outcome":"dropped_decision_moved","unrecorded_so_far":1,
  "window_seq":2}` - the record now NAMES what superseded it. `game-126v123-1789149260.stderr`
  carries exactly **2** `unclosed <think>` lines: 2 = 1 recorded + 1 unrecorded, identity holds.
  Every `force_close_*` arm-bound counter reads 0 and no window at this seat armed twice on one
  arm, so the bound is still **UNTESTED**, not refuted. `phase2_answer_recovered`/`_missing` are
  **0/0** in all six (wave 77: 3 and 1 at this seat) - the phase-2 path was not exercised here.
- **S12 / CY F7 (`Opponent life trend:` netted gain against loss) - FIXED, verified.** 301 trend
  lines at this seat: 259 `unchanged at N`, 39 net-only, **3 with the split**. The 3 are `126v146`
  seqs 13/14/15: `(-1 since turn 8; over those turns life-gain EVENTS put +1 on them and life-loss
  EVENTS took -2 off - the figure before this is the two netted)` - TRUE against the log (`Opponent
  lost 1`, `Opponent gained 1`, `Opponent lost 1`). I re-checked every one of the 39 net-only lines
  by replaying the log events inside its own turn span: **0 false net-only** (each span carries
  events in one direction only).
- **S14 `[second copy:` - the wave-77 MED-3 shape did NOT recur.** 11 exposures. The five
  **Exquisite Blood** second copies (`126v125` seqs 249, 255, 259, 266, 280) were **declined 5 of
  5**, against wave 77's take at the same shape. The reworded clause still carries an upside-shaped
  phrase (`this copy is a second instance of an effect you already have: each line it repeats
  happens a second time`) - `DOUBLING` is gone but wave 77's proposed `this copy changes no number
  on the board` was not adopted; on this evidence it did not need to be. The other 6 exposures are
  Chromatic Lantern, whose clause is different and ACCURATE (`the effect it gives your OTHER
  permanents is already on - this copy adds only its own abilities`); the seat cast Lanterns #2, #3
  and #4 (seqs 31, 40, 51) - each genuinely adds a mana source, so the engine clause is right and
  it is the GUIDE's blanket "not a live cast row" that over-reaches (see §3 REJECTED).
- **S3 (stop-reached re-puts), S5 (counterspell already targeted), S10 (`defer`), S15 (`[X pricing]`,
  loyalty cost groups, folded no-op bands), S2 (repeat rows) - UNTESTED at this seat.** No window in
  six games carries `ALREADY AT OR PAST your own stop`, a counterspell row, a `defer` record, an
  `[X pricing]` bracket, a repeat row marked `x<N>`, or a folded no-op band.
- **R8 (`crossphase_board_unchanged`) - 3 `crossphase_identical_reputs` at this seat, all in
  `126v125`, and `crossphase_board_unchanged` 0 in all six.** The clause never fired; all three
  boards had genuinely moved.
- **CU F3 (regenerating/returning bodies), the forced-sacrifice own-converter case, the ANNOUNCE_X
  plan echo, `wall_miss_events`, `menu_pass_no_progress` - UNTESTED**: none of those boards occurred.

## 3. Guide verdict: **EDIT** (`strategy.txt`, **19,985 B**, UP 84 B from the live 19,901)

One decision paid for this, and it is the whole of the only decidable loss: **`126v146` seq 19**.
The seat's own reasoning names the two guide sentences it followed and shows both failing:

> `- Wait, the guide says: "A half that COMPLETES the pair this turn: pay and cast. One that does
> NOT: skip at 12 life or less." I am at 5, so I must pay.`

That is the rule read **backwards** - at 5 life the rule says SKIP - and the model reached the same
inversion twice in one trace. And:

> `- I will cast Exquisite Blood. It's the highest priority play according to the guide ("1.
> Exquisite Blood or Sanguine Bond - whichever NAME is missing ... at any 'leaves N'"). Even though
> it costs 3 life, it's entry 1.`

Rung 0 is **above** entry 1 in the same list and was printed on the same menu
(`that would KILL you` in the header, `Cast Pride Guardian` adding a blocker, its own cover clause
reading `you cover 4 of 4, leaving 0 -> you would be at 5`). The reasoning never mentions rung 0
once. Entry 1's own emphatic sentence (`NOTHING BELOW ENTRY 1 IS AN ANSWER WHILE AN ENTRY-1 ROW IS
PRINTED`) is the text the model quoted, and it names no exception upward. The Pride Guardian line
survives on the board: Battlement (0/4) blocks Silverquill Silencer (3/2), Pride Guardian (0/3)
blocks Triumphant Adventurer and gains 3, Hive animates for 3 - the seat ends the turn at 5 with
both Bloods still in hand instead of dead at t15.

**Which rule earned the 84 bytes: rung 0's precedence over entry 1, restated inside entry 1.**

**A. Entry 1 now names rung 0.** Paid by `126v146` seq 19.
before: `"leaves N", or a life total. BOTH NAMES MISSING AND BOTH ROWS PRINTED: TAKE SANGUINE BOND.`
after: `"leaves N", or a life total. RUNG 0 IS ABOVE IT AND STILL BINDS: under a "that would KILL you" crack-back the row that adds a blocker or removes an attacker is this window's answer.` + the BOTH NAMES MISSING sentence moved to its own line, unchanged.

**B. Entry 1's Silencer carve-out was keyed on a string that did not print.** The live text fires
only on `"[NAMED BY THEIR Silverquill Silencer: ... this KILLS you]"`; the seq-19 row read
`... you would be at 2` - not lethal - so the carve-out was silent on the exact window it exists
for. Re-keyed to the condition that actually matters. Paid by `126v146` seq 19.
before: `The one price that outranks this entry is the row's own "[NAMED BY THEIR Silverquill Silencer: ... this KILLS you]": then take the other half if its row carries no such price, else go down.`
after: `The one price that outranks this entry is the row's own "[NAMED BY THEIR Silverquill Silencer" on a cast that does not close the pair in this window: take the other half if its row carries no such price, else go down.`

**C. THE SILENCER TAG's threshold sentence, the one the model inverted, is replaced by a
consequence with no threshold to invert** (and with no concessive clause for the hard case,
A334 - the Sanguine Bond case is stated as part of the rule, not as an exception to it). Paid by
`126v146` seq 19.
before: `A half that COMPLETES the pair this turn: pay and cast. One that does NOT: skip at 12 life or less.`
after: `PAY THE 3 ONLY WHEN THE CAST CLOSES THE PAIR IN THIS WINDOW, OR WHEN IT IS SANGUINE BOND. A named EXQUISITE BLOOD that leaves the other half off your line is never the answer, at EVERY life total: no trigger, their card drawn, your 3 gone.`

**D. THE WIN BUTTON's opening two sentences were FALSE and are cut** (this is a correction, not a
byte cut, though it pays for part of A-C). The live text reads `NO HEADER ANNOUNCES A COMPLETE
PAIR. The "LOOP HALF PENDING" block prints only while a half is MISSING, so its absence is not "no
loop"` - but the engine prints **`LOOP COMPLETE: BOTH halves of a life LOOP (Sanguine Bond +
Exquisite Blood) are on YOUR battlefield right now ...`** on every window after the pair closes, in
**all four** games where it closed (`126v123` from seq 17, `126v152` from seq 22, `126v130` from
seq 15, `126v162` from seq 19), and `LOOP HALF PENDING:` while one is missing. The guide was
telling the model to do by hand a check the prompt now hands it.
before: `Read your battlefield line at every decision - NO HEADER ANNOUNCES A COMPLETE PAIR. The "LOOP HALF PENDING" block prints only while a half is MISSING, so its absence is not "no loop". If BOTH "Exquisite Blood" and "Sanguine Bond" are on your line, take the first of these the menu offers:`
after: `A "LOOP COMPLETE:" header prints while BOTH "Exquisite Blood" and "Sanguine Bond" are on your line, and "LOOP HALF PENDING:" while one of them is missing. Under LOOP COMPLETE take the first of these the menu offers:`

**E. Three restatement cuts, to stay under the ceiling.** (1) THE HOLD ROW's `THIS BINDS ON EVERY
MENU WITH A ROW 0, TOO: an activated row there is live.` - the very next paragraph says the same
thing at much greater length, names the loyalty row explicitly, and is where the model's attention
demonstrably goes (it quoted no hold-row text at all on the 7 windows it held over a live row, so
the redundant one-liner bought nothing); the paragraph that survives is strictly stronger. (2) ASSEMBLING THE PAIR's ` 5. Four Tutors, two of each half.` - deck
trivia; no decision in twelve games keys on the count. (3) ATTACKING's duplicate
`and lifelink pays you for damage dealt to a BLOCKER as well` in the WIN BUTTON paragraph - the
identical sentence stands six lines above it in the same section.

**Net bytes.** A+B+C add 431 B, D cuts 57 B, E cuts 290 B: 19,901 -> **19,985** (ceiling 20,000).

**What KEPT and earned it.** THE WIN BUTTON's bullet list won four games, each in the first window
that offered a bullet. LAND DROP: **57 of 57**. THE ZERO STOP: **135 exposures, 0 takes** - and
`126v125` is a 86-turn game where the seat held four Tributes on an empty board for forty turns
rather than take one. THE HOLD ROW: 91 of 104 dead menus. SILENCER's `BOTH HALVES ON ONE MENU, ONE
ROW CARRYING THAT PRICE: cast the UNTAGGED half` fired correctly at `126v146` seq 12 (Bond untagged
over Blood tagged). MULLIGAN + BOTTOMING: `126v152` seqs 2-4 shipped a 1-land seven, kept the 3-land
seven and bottomed the duplicate wall - the bottoming order verbatim. BLOCKING STEP 2's gang-block
plus the Perimeter Captain rule: `126v162` seqs 15-16, three blockers on one attacker and all three
`Gain 2 life` triggers taken (18 -> 24). Sorin's `+1 IN EVERY WINDOW BUT ONE`: `-2` was offered on
**57 menu rows** across three games and taken **0** times (the wave-76 emblem misplay does not
recur); the `+1` was taken in every loyalty window the seat did not hold over. The wave-77 STEP 1 rewrite (the blocking trigger clause as the predicate, not an enchantment on
the line) had **no exposure** this wave - only 3 blockers windows, none with a benched wall - so it
is **UNTESTED**, neither paid nor refuted.

**REJECTED edits.** (1) **A Lightmine Field rule.** The guide already states it as an absolute
(`ONE STOP ... deals the NUMBER OF ATTACKERS to EACH attacker: ATTACK: none`), the header named the
enchantment on all five windows, and the model quoted the enchantment by name while attacking
anyway. Restating an absolute that was printed, named and read is not a guide fix; the missing
surface is the arithmetic, which is §2 HIGH-2. (2) **A rule benching a half whose partner is
nowhere.** It would have wrongly benched `126v152` seq 15, where the seat cast Exquisite Blood at
t12 with Sanguine Bond in neither hand nor library reach, drew the Bond four turns later and won.
The distinguishing fact at `126v146` seq 19 was rung 0 plus the 3-life price, and edits A-C carry
both. (3) **Narrowing the `[second copy:` not-a-live-cast-row rule to exclude artifacts.** The
blanket rule IS wrong for Chromatic Lantern (a second Lantern really does add a mana source, and
the engine's clause says so accurately), but the three Lantern casts in `126v125` cost nothing in a
game with 27 untapped sources and changed no decision, and narrowing it risks the second-Exquisite-
Blood regression the rule exists to stop. (4) **A rule about naming a graveyard or library card as
the next cast.** 27 `plan_names_uncastable_zone_card` flags in `126v125`; one is a genuine
misconception (seq 53, `cast Sanguine Bond from my graveyard` after it was countered) and the rest
name a card as the object of a search step. No better play existed on any of them. Re-rejected on
the same grounds as wave 77. (5) **A stronger hold-row or attack rule for the 7 live-loyalty holds and the `126v130` seq 32
`ATTACK: none`.** Both rules are already absolute, already specific to the exact printed strings
(`NEVER HOLD ON A MENU THAT PRINTS ONE`; `A Vampire the line already calls unusable ("held back,
THIS creature could not block ANY of ...") costs you nothing: send it`), and on the seq-32 window
the guide's exemption string and the rendered A-line match character for character. This is the
same failure mode as the Lightmine attacks - a printed absolute, read and not applied - and the
answer to it is not a fourth restatement. Worth the core loop's attention as a pattern: three
separate absolutes (Lightmine stop, live-row hold ban, held-back-Vampire send) each failed while
the exact trigger string was on screen, all in the same corpus. (6) **Anything about `126v125`.** Four Sanguine Bonds cast into four
counterspells, with a Lightmine Field shutting off the Vampire plan, is a matchup, not a rule.

No general-guide proposal and no skill proposal. Edits A-C are a deck-specific precedence fact
(which rung of this deck's own cast list binds under a lethal crack-back) and edit D is a
correction to a false statement about this deck's own prompt surface; every item in §2 belongs to
the engine seat, and none of them restates an existing amendment.

## 4. Not checked
I read **no engine source at all** this wave and built no fixture; every mechanism sentence in §2 is
a falsifiable repro against the logs, not a claim about the code. I opened **stderr only for
`126v123`**, and only to count `unclosed <think>` for the forced-close identity; I contribute
nothing on the other five games' stderr, on `Parser returned NULL` noise, or on the askreplay/
directory (which I did not open). I did not read the corpus-wide census tool output
(`~/.gatelogs/w78-stats.out`); all my figures are computed from my six seat logs. I did NOT quote
any of the corpus's 7 `unparsed_reply` replies or the `152v123` `engine_answered` record - none is
at this seat and the engine seat owns them. I traced **no opponent seat log** in any of the six
games: every opponent action in §1 is taken from my own seat's GAME LOG block, so opponent
DECISIONS (why deck125 held Cancel for t82, why deck146 exiled the Lantern rather than the Bond)
are unexamined. The 6 unrendered `stack_drain_windows_asked` increments (MED-5) and the 2
`own_loop_windows_asked` windows (HIGH-1) are **unlocated and unreconciled** - there is no
per-record field for either. I could not adjudicate `hold_windows_skipped` (1,358 at this seat),
`mana_only_windows_skipped` (367 - and `126v130` 156 / `126v162` 129 against wave 77's 5-23 at this
seat, a growth I can measure but not explain from the translog) or `own_turn_windows_skipped` (117):
what those skips suppressed is not visible. The `Goblin` -> `Goblin #1` renumbering case of S9(a) is
UNTESTED here. Card verification: I checked **Exquisite Blood, Sanguine Bond, Tribute to Hunger,
Pride Guardian, Perimeter Captain, Wall of Omens, Overgrown Battlement, Staff of Nin, Chromatic
Lantern, Sorin Lord of Innistrad and Lightmine Field** against their rendered `card text:` /
`{effect:}` in the prompts, and **Sigarda, Champion of Light** against the truncated gloss only
(MED-6). **Silverquill Silencer, Kaya the Inexorable, Triumphant Adventurer, Vanishing Verse,
Brutal Cathar, Briarbridge Tracker, Intrepid Adversary, Tovolar's Huntmaster, Teferi Who Slows the
Sunset, Hive of the Eye Tyrant, Fate Unraveler, Howling Mine, Bloodline Keeper, Essence Scatter,
Cancel, Dream Fracture, Path to Exile and Elixir of Immortality were NOT verified** beyond what the
render stated.
