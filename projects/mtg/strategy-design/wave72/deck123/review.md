# Wave 72 — deck 123 (Intruders of Thraben, WBU token combo) — per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-163626`, binary `ceb9b8de0` /
`archives/wagic-ba6af25f1-w72step1`, `--thinking on`. Six seat files
`...ai_baka_deck123-<ptr>-vs-<opp>.jsonl`; **1 win (vs162), 5 losses**.

Seat census: **271 decisions** (wave 71: 460 — the drop is the wave-72 window model, not shorter
games: `hold_windows_skipped` 239 across the six, `own_turn_windows_skipped` 52), 100% native
reasoning, **1 fallback** (`http_error`, 162 seq 23; 0 `noop_row_zero_reask` this wave),
0 `action_before_plan`, 0 `reply_truncated`, 0 `off_protocol_bytes>0`, 1 `plan_line_missing`
(162 seq 24, the recovery record after that http error), 0 `menu_pass_no_progress`,
`ask_replays_reserved` 223 / **refused 0**, `async_drops` 4.
`sibling_window_asks_skipped` **0** and `stop_reached_windows_skipped` **0** in all six games — §3
answers why for this seat, and it is not "nothing qualified".

**Headline: the guide's own PLAN template makes the wave-72 stop collapse unreachable, and the
hold latch ate the deck's win condition in a whole game.** Both are §3 HIGH items. Wave-71 HIGH-1
(eleven identical Create passes) did NOT recur — but it did not recur because the loop barely ran,
not because the collapse fired.

## 1. Game by game

**vs deck162 — WIN, turn 10 (16 / −4). The loop worked, in one shot, on the guide's line.**
Doomsayer t6, Intruder Alarm t8 (seq 9, maker already out — correct P3), then **seq 10** single
tap and **seq 11** the batch row: `CHOICE: 2 (Create human with Thraben Doomsayer x22)`, PLAN
"stop count 24, currently at 2, making 22 this window". Stop by the guide = L 20 + C 1 + 3 = 24, M
= 2, N = 22 — **exact, first try, no re-ask**. Attacked t10 with all 26 (**seq 53**) for the kill.
This is the deck doing what the file describes, and the arithmetic section earned its bytes here.
(That same reply is also the corpus's cleanest evidence for HIGH-1 below: it *states* a correct
stop and the engine cannot read it.)

**vs deck130 — LOSS, turn 22 (0 / 14). THE DECIDING DECISION IS seq 31, and it is a HOLD.**
Doomsayer resolved t5 and lived to t11+; Idyllic Tutor found Intruder Alarm t9 (**seq 36**). At
**seq 31** (t7, main 1) the menu was exactly two rows —
`1. Create human with Thraben Doomsayer [cost: Tap] {paying this taps: Thraben Doomsayer - it
cannot attack this turn} ... {if you pass here, this option is not offered again until the board
changes}` and `2. Hold priority` — and the model took **row 2**. Its `reasoning` is explicit and
otherwise sound: the token would be summoning sick, it wanted to attack with the Doomsayer instead,
*"I don't want to be asked again. So 2 is better than 0."* It read the hold as a better-worded pass
for one window. **It was not**: a Create row's text never moves, so the hold never lifted —
**that seat made ZERO tokens for the remaining fifteen turns of the game**, and only four windows in
the entire game ever carried a Create row (all on t7). Declining the token that turn was right;
declining it with the hold ended the deck. **Classification: STRATEGY (the P8 hold rule already
forbade it) compounded by an interface item (§2 HIGH-2)** — guide edit 2.

**vs deck126 — LOSS, turn 15 (0 / 39). The combo assembled and lost the race, not a misplay.**
Alarm t10 (**seq 12**, correct order: Tribute to Hunger t8 cleared their only blocker first),
Keeper t12, and on t14 the model ran the whole chain — **seq 17** tap, **seq 19**
`CHOICE: 5 (Create vampire with Bloodline Keeper, repeated 22 times, then stop)` (stop 23 vs L 20 +
C 0 + 3 = 23, exact), **seq 20** `Transform:backside` into Lord of Lineage. **Seq 40**'s attack
with Lord of Lineage alone is NOT a misplay: all 23 Vampires were made that turn and the prompt
listed every one under `NOT offered above and NOT able to attack this turn (summoning sick)`. The
Keeper only came off summoning sickness on t14, so there was no earlier opponent-turn window to
fire into (**seq 15/16**, t12 and t13, carried Marsh Flats rows and the hold and no Create row at
all). Deck126's Sanguine Bond + Exquisite Blood loop then took it from 24 to 4 on t15. The guide's
LOOP CLAUSES were read and obeyed throughout (`seq 39`, `seq 40`, `seq 47` all cite the loop).
**Variance/matchup, not guide effect.**

**vs deck152 — LOSS, turn 13 (−17 / 28). THE DECIDING DECISION IS seq 20, a maker block.**
Tutor t6 found the Alarm, Bloodline Keeper t8. At **seq 20** (t9 blockers) the header read
`INCOMING THIS COMBAT: 2 attackers, 8 unblocked damage - you would be at 8 ... you would be at 12
AT BEST`, and the only blocker row was
`B1. Bloodline Keeper (3/3) [flying] - may block A1 (both die) {their attacker's lifelink, this
block: they gain 4 from this block only, and this attacker deals nothing to your life}`. The model
blocked. By the guide that is wrong twice over: the LETHAL RULE needs unblocked damage ≥ life − 4
(8 < 12), and `OTHERWISE makers do NOT block: ... "(both die)" on a Doomsayer or Keeper is declined
while "you would be at N" is above 5` (N = 12). It traded its only maker and win condition to
prevent 4 damage while *giving them 4 lifelink life* — a net-zero race trade for the whole deck. It
never had a maker again and died on t13. **Classification: STRATEGY.** The rule is in the file, but
`STEP 1 - ... makers block LAST, after every token` reads as an ordering that reaches the maker
when there is no token; guide edit 3 bounds it.

**vs deck146 — LOSS, turn 23 (−9 / 23).** Land-light (4 lands at t14) against a Kaya /
Barrowin board. Both Damnations (**seq 68** t16, **seq 73** t20) were correct by the gate —
`destroys 3 of their creatures (3 without a restriction against attacking), 0 of yours`, M = 0,
K = 3. One P3 violation, not decisive: **seq 64** (t14) cast
`2. Cast Intruder Alarm {2}{u} {leaves 0 of your 3 untapped mana sources untapped - casting this
taps you out}` on `Your battlefield (4 permanents listed, of which 0 are creatures ...)`.
**Wave-71 edit 1 half-worked**: the self-contradiction it removed is gone — the 7,490-character
`reasoning` no longer quotes the TIMING line to overrule P3 — but it also never reaches P3 at all
("Intruder Alarm is definitely the right play"). Mitigation the file cannot see: no maker was on the
menu (Keeper needs 4 mana; Marsh Flats makes none, so 3 sources), so the alternative was casting
nothing. Not edited: the rule is already as plain as it can be written and one violation on a lost
board does not pay for bytes on a 20 KB budget.

**vs deck125 — LOSS, turn 33 (0 / 51). No decision to fix; the makers were killed on sight.**
Doomsayer t8, Keeper t10, Keeper again t16 and t26, Doomsayer t28 and t32 — six maker casts,
**and not one window in the entire 88-decision game ever offered a Create row**, because none
survived to an untapped priority window. The t22 cleanup discard of `Intruder Alarm (copy 2 of 2 in
your hand)` (**seq 111**) is the guide's own rule, correctly applied — I checked before flagging it.
This is the matchup, not the file.

## 2. Engine / interface / card items

**HIGH-1 — the wave-72 stop collapse (BT M10 / BX F2) is UNREACHABLE from this guide: 0 of 257
PLAN lines this seat wrote are parseable as a stop, so `stop_reached_windows_skipped` could not be
anything but 0, and `{right now: M=…, your stated stop=…}` was rendered ZERO times in 271 windows.**
`repeatPlanStopAndCurrent` (`src/AIPlayerGPT.cpp:8439`) requires BOTH a `stop` label and an `m`
label each followed *immediately* by a digit (`repeatPlanScanNumberAt`, :8395, skips only ` = : (`
and the word `is`). The live guide's mandated template is
`PLAN: this window <x<N> or pass>, then <the next action you intend>, then keep making humans until
M reaches <L+C+3>, then pass` — it contains no `stop` label at all, and `M reaches 24` fails the
digit test. Repro, best case in the corpus:
`...vs-ai_baka_deck162.jsonl` **seq 11**, `PLAN: Tap Thraben Doomsayer 22 times to reach 24
creatures (stop count 24, currently at 2, making 22 this window), then attack for lethal next turn.`
— a *correct, fully stated* stop that the parser rejects because `count` sits between `stop` and
`24`; and **seq 12**, `PLAN: I have reached the stop (24 creatures vs 20 life + 1 creature).`,
which parses `stop`→24 but finds no `m`, so `repeatPlanStopAndCurrent` still returns false.
Note the row's own bracket text asks for it in exactly the words that break it: *"a PLAN line
stating your stop count, the count you are at now"* → the model writes "stop count 24". Guide edit 1
fixes the guide side. **The engine side is the item: the parser should accept the wording its own
row prompts for** (`stop count <N>`, `stop of <N>`, `M is at <N>`, `at <N> now`), or the row should
print the literal template it will parse. Until one of those lands, M10 and F2 are inert for every
deck whose guide does not happen to write `stop=<N>` — and this seat's file is the one M10 was
built from. `w72StopReachedWindowCollapses` (:28085) itself is fine; it is starved, not broken.

**HIGH-2 — a HOLD taken over a repeatable Create row can never be revoked, because that row has no
text that changes; one such hold silenced a token engine for fifteen turns.**
Repro: `1788817003-ai_baka_deck123-0x5632bb0971b0-vs-ai_baka_deck130.jsonl` **seq 31** (t7, main 1),
rendered rows:
```
1. Create human with Thraben Doomsayer [cost: Tap] {paying this taps: Thraben Doomsayer - it cannot
   attack this turn} {card text: "{T}: Put a 1/1 white Human creature token onto the battlefield.
   -- Fateful hour - ..."} {if you pass here, this option is not offered again until the board changes}
2. Hold priority - pass now, and do not ask me again - this turn or later - until one of the rows
   above changes ...
[hold check: every row above was also on the menu at the last window at this seam (1 window in a
row now) - a hold taken here holds until one of them changes]
```
Answer `CHOICE: 2 (Hold priority)`; no Create row was ever printed again in that game (t7 → t22,
`hold_windows_skipped_priority` 9). Two render facts drove it and both are engine-side: (a) row 1
already carries `{if you pass here, this option is not offered again until the board changes}`, so
**pass and hold read as the same latch** and the hold reads as strictly cheaper — the reasoning says
so; (b) the Create row carries **no live counter** at this seam (no `[repeat: ...]` bracket, no
`{right now: M=…}` clause), so "until one of the rows above changes" is a condition that cannot
occur while the maker sits there. Suggested shape: exempt a repeat-eligible activation from the hold
latch, or print the maker's live creature count on the plain Create row so the row moves when the
board does. Related: wave-71 MED-2 (`{repeat: ...}` missing before the batch row exists) is the same
missing-counter defect, and it is unchanged — `...vs-ai_baka_deck162.jsonl` **seq 10** offers the
bare Create row, **seq 11** the batch row.

**MED-3 — the hold's own scope text does not warn that the latch outlives the turn in the one case
that matters.** The row says "this turn or later", which is accurate, but the `[hold check]`
annotation beside it is framed entirely around *re-opening* ("a hold taken here lasts only until one
moves again", 126 **seq 17**), which reads as reassurance. On a menu where the only acting row is
invariant, the honest annotation is the opposite. Repro: 130 **seq 31** above; contrast 126
**seq 17**'s `[hold check: 4 rows above are new since the last window at this seam ...]`, where the
same wording is true.

**MED-4 — `YOUR PLAN (as you last stated it, N windows ago on turn M)` is still a truncated
fragment.** Carried unchanged from wave-70 LOW-4 / wave-71 LOW-4. Repro:
`...vs-ai_baka_deck126.jsonl` **seq 40**, `YOUR PLAN (as you last stated it, 1 window ago on turn
14) reads "Pass priority to avoid triggering the life loop and wait for a better opportunity."` —
here it is intact, but it is the *hold's* plan being carried into the ATTACK window, where it reads
as advice against attacking. The model attacked anyway and correctly.

**LOW-5 — the new perspective/loop render clauses were all TRUE where they fired in this seat.**
`LOOP COMPLETE: BOTH halves of a life LOOP (Sanguine Bond + Exquisite Blood) are on THEIR
battlefield right now` (126 **seq 40**) — verified against the opponent seat's board. The
`CRACK-BACK NEXT TURN` line (49 renders), the `NOT offered above and NOT able to attack this turn
(summoning sick)` list (126 **seq 40**, 23 names), the sacrifice ask's TOKEN ENGINE annotation
(126 **seq 47**, `Lord of Lineage ... THIS IS NOT JUST A BODY: TOKEN ENGINE`) and the lifelink
blocker pricing (152 **seq 20**) all stated facts I could confirm. No false render found.

**Card facts verified** against `bin/Res/sets/primitives/` for every card that decided a window
here: Bloodline Keeper (`{T}` token, `{B}` transform gated on five Vampires — the row text matches
Oracle), Lord of Lineage (+2/+2 to other Vampires, 5/5 — matches the rendered `Vampire #1 (4/4)`
from printed 2/2), Intruder Alarm (symmetric untap-all on any creature entering), Thraben Doomsayer
(Fateful hour +2/+2 at 5 or less), Damnation, Tribute to Hunger, Devour Flesh, Tragic Slip,
Lightning Greaves. **No card-script defect found in this seat.**

## 3. Why `stop_reached_windows_skipped` and `sibling_window_asks_skipped` are both 0 here

- **`stop_reached_windows_skipped` = 0 — the counter is wired to a path that cannot run for this
  seat.** Not "no stop was reached": in vs162 the seat reached its stop and then answered three more
  Create windows by hand (**seq 12** pass, **seq 14** take, **seq 18** take). The collapse needs
  `carriedStop >= 0` from `mStatedStop`/`repeatPlanStopAndCurrent`, and §2 HIGH-1 shows that value
  was **never once populated** in 257 PLAN lines — hence `repeatRowStopClause` also printed nothing
  in 271 windows, so the model never saw the clause either. The M10 guard `everyBaseRowIsStopReached`
  is not what failed; the stop it compares against does not exist. UNTESTED, not passed.
- **`sibling_window_asks_skipped` = 0 — the BX F1 subset rule had almost nothing to collapse in this
  seat.** A casting hold is the precondition, and only vs146 and vs125 took holds on casting menus
  (`hold_windows_skipped_cast` 25 and 76; the other four games are 0). In those two games the seat
  had no creatures and therefore no activated rows for a sibling priority window to carry, so the
  subset test had no sibling menu with acting rows to compare. Corpus-wide adjudication is the
  engine seat's; from here it is "no qualifying window", not "the rule refused".

## 4. Guide verdict: **EDIT**

Revised guide at `wave72/deck123/strategy.txt` — **19,987 bytes** (live guide 19,968; ceiling
20,000). Three additions paid for by five compressions; the file grows 19 bytes net. No citations,
counts or history in the guide itself (Amendment 332).

| # | before → after | seq that paid for it |
|---|---|---|
| 1 | PLAN template `PLAN: this window <x<N> or pass>, then <the next action you intend>, then keep making humans until M reaches <L+C+3>, then pass` → `PLAN: stop=<L+C+3>, M=<the "of which" number now>; this window <x<N> or pass>, then <the next action you intend>` plus a line requiring a DIGIT touching the `=` and naming the three shapes that fail (`stop count 24`, `M reaches 24`, `the stop (24 creatures)`) | 162 **seq 11** and **seq 12**, and the 257-line zero. **This is the edit that earns its bytes** (+~290, the largest in the file): it is the only change that turns a wave-72 engine feature from inert into live, it costs the model nothing (it was already computing both numbers correctly), and the three failing shapes are quoted because two of them are what the *engine's own row text* asks for. |
| 2 | hold rule, appended: `A HOLD OVER A CREATE ROW IS PERMANENT, NOT A SKIP OF THIS WINDOW: that row's words never move, so the hold never lifts and the maker never taps again, this game. Declining the token for THIS turn only - to attack, to cast first - is "CHOICE: 0 (pass)"; the hold there ends the deck.` | 130 **seq 31**. Wave 71 added `a Create row BELOW the stop` to the hold's live-row list and the model still took the hold — because it was not treating the hold as a *hold*, it was treating it as a better-spelled pass, and its reasoning says exactly that. The missing fact is the hold's PERMANENCE on an invariant row, which no rule in the file stated. +~250. |
| 3 | `STEP 1 - ... makers block LAST, after every token.` → `... makers block LAST, after every token - so with no token to spend a maker's turn never comes: below the LETHAL RULE a lone maker answers "BLOCKS: none", and it does so on "(both die)" most of all.` | 152 **seq 20**. The BLOCKING section's last line already forbade this block, but STEP 1 is where the model enters the section and "LAST" is an ordering word: with one blocker, last is first. The bound goes on the line that produced the error. +~135. |
| — | **COMPRESSIONS paying for it (−~660):** the THE DECK paragraph (3 lines → 2, the card names are in the P-list); the `ANSWER FORMAT` "ANSWER:" bullet (protocol has been clean two waves: 0 `off_protocol_bytes`, 0 `action_before_plan`); the `"THAT WOULD KILL YOU"` section (rule kept, prose halved — 0 renders of that header in 12 games across two waves); the LIGHTNING GREAVES section (rule kept, 5 lines → 4); the BOTTOMING worked hand-sizes at N of 3/5/6 (0 mulligans and 0 bottoming asks in 12 games — all six hands this wave were untouched sevens kept correctly at **seq 2**). | — |
| — | **DELETED (−~250):** the `Rows: "Create human with Thraben Doomsayer [cost: Tap]", ...` enumeration (the model reads those names off the screen; it named all three correctly at 162 seq 11 and 126 seq 19 without it); `- Once it is right, answer a reopened run the same way every time.` (subsumed by the re-ask rule and by edit 2); the Perimeter Captain / Pride Guardian names inside DAMNATION, now a pointer to CHECK 1 A where the same two names already stand. | — |

### Did wave 71's edits pay off?
- **Edit 1 (ALARM ordering-only bound): HALF — the contradiction is gone, the violation is not.**
  146 **seq 64** casts the Alarm at 0 creatures again, but the 7,490-char `reasoning` no longer
  quotes the TIMING line to overrule P3; it simply never consults P3. The bound worked; reachability
  is the residual, and I did not spend bytes on it (see §1 vs146).
- **Edit 2 (hold admits a Create row past the stop / take the HOLD past the stop): NOT EXERCISED as
  written, and its INVERSE fired.** No window in the corpus reached a stated stop with a Create row
  as the only survivor — because no stop was ever machine-stated (§3). What did happen is 130
  **seq 31**: the hold taken on a Create row *below* the stop, which edit 2's other half already
  forbade and which needed the permanence fact instead. Hence edit 2 above.
- **Edit 3 (bait rule moved onto the P1/P2 line): UNTESTED.** vs125 is the game it was written for
  and the makers there were removed, not countered; no window in the corpus put a maker on a menu
  beside a dead spare with mana for two. No counter-evidence either.
- **The wave-71 deletion of "a Create row passed TWICE is retired for the turn": CONFIRMED right.**
  162 **seq 10/14/18** and 126 **seq 17/19** all take a Create row after passing one earlier in the
  same turn. Keeping that false line would have been actively harmful.

## 5. Optional proposals
**None.** All three edits are scoped to this deck's cards and to this file's own wording. The one
finding that looks general — "a guide's mandated PLAN wording is a machine interface, and an engine
feature keyed on it dies silently if the two are written apart" — belongs in §2 HIGH-1 as an ENGINE
item (the parser should read what its own row asks for), not as a strategy-writing amendment; and
proposing "keep the guide's PLAN format in sync with the parser" as skill guidance would be a rule
about a coupling the guide author cannot see from the guide. Flagging it to the core loop rather
than minting an amendment.

## 6. What I did NOT check
- Opponent seat files were read only for board context (deck126's loop halves, deck152's attackers);
  I did not audit any opponent's decisions.
- I did not adjudicate any lane prediction corpus-wide. §3 answers the two zeros **for this seat's
  six games only** — whether other decks' guides emit a parseable `stop=`, and therefore whether
  BT M10 is inert corpus-wide or only here, is the engine seat's call. My reading of
  `AIPlayerGPT.cpp` (:8395, :8439, :28085, :35015-35081) is static; I did not build or run anything.
- The 88-record vs125 game and the 53-record vs130 game were traced at their cast/attack/discard/
  hold decisions and at every window carrying a Create row; I did not read the `reasoning` of each
  of the 239 held/declined windows individually (I checked all of them programmatically for a live
  maker, Alarm, live-Tutor or `{kills:}` row — **130 seq 31 is the only hold over a live acting row
  in the seat**).
- I did not verify whether HIGH-2's hold latch predates wave 72 or is new with the F1/F3 hold
  changes; the wave-71 seat took no hold over a Create row, so I have no comparison point.
- The `askreplay/` sidecar (now harvested, 29 files run-wide) was not opened: this seat reserved 223
  replays and refused 0, so nothing here needed it.
- No build, no run, no git; nothing under `bin/Res`, `src/` or outside `wave72/deck123/` was touched.
