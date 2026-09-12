# Wave 80 — deck 125 (Revelation Fracture, UW draw-go control)

Seat files: the six `*ai_baka_deck125-*.jsonl` in
`/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final`, all with `gameend`, 0
timeout-tainted. All six ran on the 20-game binary `wagic-a7a6b8942-w80step1` (this deck is not in the
rerun game), so nothing here tests lanes DI or DJ.

**3 W / 3 L** (wave 79: 1/6; wave 78: 3/6). 584 decision records = ask 393 + priority 100 + hold_event 53 +
discard 34 + forced_close 2 + attackers 1 + bottom 1; 529 model prompts,
**reasoning on 529/529** (median 5,938 chars, max 17,629). Latency p50 74.8 s, p95 165.2 s, max 413.6 s.
`answer_replaced` 0, `post_answer_overrun` 0, `reply_truncated` 0, `action_before_plan` 0,
`off_protocol_bytes>0` 4 (0.76%), `plan_line_missing` 4, `protocol_deviation` compliant 525 /
`unlabelled_plan` 4 / `plan_absent` 0 — **no heuristic fallback on any seam**. Gameend sums over the six
seats: `ask_replays_reserved` 583 = `ask_replays_cache` 583 + `_repeat_latch` 0 (U9 identity holds here),
`ask_key_continuation_differs` 36, `hold_events` 53 = `hold_verdict_safer_ignored` 19 +
`hold_reopened_new_lethal` 4 + `hold_reopened_new_threat` 0 + **30 `reopen_rows_moved`, which no gameend
counter carries** (the brief's "+ ?"), `hold_released_turn` 28, `hold_windows_skipped` 370 (cast 250 /
priority 120; wave 79: 446), `crackback_verdict_lines_rendered` 76, `crackback_lethal_blocked_away` 0,
`stack_death_verdict_lines_rendered` 3, `plan_cast_steps_closed` 65, `crossphase_replayed` 0 /
`_board_unchanged` 0 (wave 79: 4) / `_identical_reputs` 20, `own_loop_*` 0, `menu_single_outcome` 0,
`forced_close_events` 2 / `_unrecorded` 1, `async_drops` 1, `stop_reached_*` 0,
`plan_names_uncastable_zone_card` 1, `stack_drain_windows_asked` 1. No hang, crash or timeout.

## 1. Game-by-game

| opp | result | turns | end life | the decision that decided it |
|---|---|---|---|---|
| 152 | **WIN** | 49 | 66–−12 | Two Staffs, 19 lands, six Revelations. Ended at `seq 354` by casting **Emrakul** for lethal *against the guide's own absolute*, which the model quoted verbatim and overrode — §2 S-1, §3 edit 5. The one blemish is `seq 270/271`, an announce-then-Decline that stranded **17 mana** (§3 edit 1) |
| 146 | **WIN** | 41 | 50–0 | Counter-heavy, clean. `seq 193` is the wave's best single read: the X-menu library ceiling said `X=9 is the largest value on this menu the library pays for` and enumerated the 3 owed draws (draw step + two Staff upkeeps); the guide told it to announce **K minus one per Staff = 7**; it announced **9** and was right. §3 edit 3 |
| 123 | **WIN** | 41 | 26–0 | Staff clock plus 6 counters. `seq 129/130` is an announce-then-Decline stranding **12 mana** at turn 27 — free only because the game was already won. §3 edit 1 |
| 126 | LOSS | 26 | 0–40 | **`seq 178` → `seq 179`, t23 Main 1, 19 life, 6 mana, Cancel + Dream Fracture in hand.** The cast row read `max affordable X=3`; the model took it, then at the X menu announced **X=1**, reasoning in full: *"the guide says 'THE SIZE FLOOR IS STOP (4), and at 10 life or less it is lifted: cast there at the largest X offered.' My life is 19. So stop (4) applies. I should not cast X=2 or 3. X=1 is acceptable."* Stop (4) does not apply at all (the row read 3, not "2 or less"), and stop (4) is a cast-or-not test, not a cap on X. The cast tapped all 6 sources; on the following turn a second **Sanguine Bond** resolved with the seat at 0 untapped and Cancel in hand, completing Sanguine Bond + Exquisite Blood, which took it 19 → 0 in one chain. §2 A-1, §3 edit 2 |
| 130 | LOSS | 28 | 0–20 | Variance. Never drew a Staff or an Emrakul in 28 turns; its whole board was Lightmine Field ×2 and 11 lands. At `seq 99` (7 life) the only cast row was Lightmine Field and it took it — correct under §LIGHTMINE FIELD's survival clause (their power 7 ≥ its life 7). No guide item is bought |
| 162 | LOSS | 17 | 0–21 | **`seq 4`, the bottom ask.** Mulliganed to six on a 3-land/4-spell one-colour seven (correct), kept a 2-land six (correct), then **bottomed one of the two lands** — Seachrome Coast — and was never offered another land drop between turn 1 and turn 9. Its own reasoning names the contradiction: *"I have 2 lands. I need more lands. I'll bottom the Seachrome Coast"*, on the invented premise *"Bottoming a land is usually the best play when you have few lands and need ramp."* §3 edit 4 |

## 2. Engine / interface / card items

**HIGH — A-1. The hold latch's verdict clamp suppresses every window of an opponent turn on which a new
opposing permanent reaches the stack, and `hold_reopened_new_threat` stays 0 while that happens.**
Repro `game-125v126-1789216932.stderr:1445-1447`: with a hold live at the cast seam, the log reads
`Action added to stack: Exquisite Blood` → `hold verdict CLAMPED at the cast seam - the live face is no
more dangerous than the held one: held [stack death verdict: you survive the stack] -> live [stack death
verdict: nothing lethal on the stack]` → `holding priority at the cast seam (the model's own hold row,
turn 13; 73 windows held this game)` → `Resolving Action on stack: Exquisite Blood`. The matching records
are `…vs-ai_baka_deck126.jsonl` `seq 92`/`93`, `what: verdict_clamped`. Exquisite Blood is on this guide's
own COUNTER-ON-SIGHT list, and the hold row's printed promise is `it stands until your next turn begins,
**or until one of the rows above changes (any change re-opens this window)**` — a spell arriving on the
stack is a row appearing. DH F3 made the LETHAL face an event keyed on threat identity; a new **non-lethal**
threat is still clamped away, and on this seat `hold_reopened_new_threat` reads 0 across 53 `hold_event`s
while 19 clamps fired. Price this wave: the t13 clamp cost nothing (the hand held only two Essence
Scatters, which cannot counter an enchantment — verified at `seq 89`), so the counter reading 0 is a FAIL
on rendering, not on outcome. The same shape recurs in `…deck126.jsonl` around `seq 70-76` (7 clamps in one
turn) and `seq 114-120`, `seq 136-140`.

**MED — B-1. The plan echo's label collides with the reply protocol's label, and all four protocol
deviations on this seat are that collision.** The echo prints `THE PLAN YOU LAST STATED (as you stated it,
1 window ago on turn 42) reads "…"`. The four `unlabelled_plan` replies are
`…vs-ai_baka_deck126.jsonl seq 164` (`THE PLAN: Hold priority through their turn…`), `seq 165`
(`THE PLAN: Cast nothing right now…`), `…vs-ai_baka_deck152.jsonl seq 63` (`THE PLAN: Play Tundra…`) and —
the clearest one — `…deck152 seq 111`, whose entire reply is
`THE PLAN YOU LAST STATED reads "End turn."` + `CHOICE: 6 (Cast nothing right now)`: the model echoed the
render's sentence in place of writing a plan. Two fixes, either sufficient: rename the echo so it does not
open with the protocol's own keyword (e.g. `EARLIER YOU WROTE:`), or read `THE PLAN:` as the plan line
under the owner's robust-parser rule — the answer is unambiguous and the plan precedes the action in all
four.

**MED — B-2. `hold_event.window_seq` is on a different numbering from the record `seq`, with no marker
saying so.** DH F12 made `replayed_from` a record seq; `hold_check_ref_seq` resolves against record seqs.
But in `…vs-ai_baka_deck126.jsonl` the hold taken at record `seq 89` produces `hold_event`s at `seq 90-96`
all carrying `window_seq: 42`, and record 42 in that same file is the turn-8 Cancel cast. Same shape on
every seat (`…deck162.jsonl`: hold taken at record `seq 17`, four `hold_event`s carry `window_seq: 14`).
A reviewer cross-referencing the two fields lands on an unrelated window. Either re-key `window_seq` to the
record seq or rename it.

**MED — B-3. The X-menu `Decline` row is reachable with the stops already failed and there is no
least-bad answer printed on it.** Both of the corpus's two `dropped after its X was announced` events are
this seat: `game-125v123-1789216946.stderr:2313` (`12 mana already paid stays floating and is lost`) and
`game-152v125-1789216918.stderr:4677` (`17 mana`). The decline row itself is now correct and complete —
`{declining strands the 17 mana already paid: the card comes back to your hand, your sources STAY TAPPED,
and the 17 mana stays floating in your pool. No row on this list can spend it …}` — so U1/DG + DH F5 PASS
on rendering. What the menu does not say is that some listed X is always at least as good as Decline when
no draw punisher is out. Cheap engine addition on the decline row: `every listed X spends the same mana and
returns it as life and cards; this row returns none of it`.

**LOW — B-4. Prompt bytes regressed hard on the long game.** 529 prompts, p50 15,236 B, max 27,189 B,
**157 of 529 = 29.7% over 20 KB** against 11.8% in wave 79. Per game: `125v152` p50 20,648 B with
**91 of 170 (54%) over 20 KB**, `125v146` 44/110 (40%), `125v123` 22/102 (22%), and 0% in the three short
games. The driver is board size, not a new clause — the 49-turn game ends with 23 permanents on the seat's
own battlefield line and 15 on the opponent's, each enumerated. The `#1-#7 x7` collapse is working; what is
not collapsed is the per-permanent `{effect: …}` text, reprinted in full for every land-less permanent.

**Verified TRUE / FIXED on this seat:**
**Wave-79 B-1 FIXED.** The clause `and the stack is not empty (see ON THE STACK above)` appears on **0** of
529 prompts with no `ON THE STACK, waiting to resolve` block, against 23 in wave 79; 145 prompts carry it
with the block present (true).
**U1/DG + DH F5 PASS.** `plan_cast_steps_closed` 65. At both declines the echo still carries the cast
clause at the X window — `…deck152 seq 271` echoes `"Cast Sphinx's Revelation for X=11. End turn. Next
turn, ping opponent with both Staff of Nins."` and `…deck123 seq 130` echoes `"Cast Sphinx's Revelation at
X=9 …"` — i.e. the step is held open to the cast event, as designed. I sampled 12 completing X menus (all
12 Revelation announces on this seat) and the cast clause was present in the echo at every one.
**U11 own-spell marker: UNTESTED here.** `[your stack]` appears on **0** of 529 prompts and in 0 target
asks corpus-wide; the only hit in the whole corpus is inside a model's *reasoning*
(`…vs-ai_baka_deck126.jsonl seq 42`), where it quotes wave-79 edit 2 back and rules it "Not applicable".
So the wave-79 self-target bug did not recur and the rule was read; the state never arose.
**Wave-77 edit 1 PASS, fourth wave running.** 12 Revelation announces, all 12 in a main phase, 0 at an
upkeep or draw window.
**T8(b) PASS.** Every keep-X clause on this seat names a counterspell; Path to Exile 0.
**Land drops.** No `Play no land right now` was taken at any land window in any of the six games.
**Pings.** 72 Staff activations, **0 at the seat's own face**; 61 at the opponent, 11 at a body, and the two
Staffs are distinct rows (`Staff of Nin #1` / `#2`) taken in the same upkeep — the wave-78 over-merge does
not recur.
**Sweepers.** Every `Cast Supreme Verdict` row seen at K of 0 was declined (`…deck126 seq 101/106/123/128/
147/152/164`, seven windows, `destroys 5-7 … all of them carry a restriction against attacking`).

## 3. Guide verdict: **EDIT** (`wave80/deck125/strategy.txt`, **19,957 bytes** against the live 19,997 — it
SHRINKS by 40.) Five edits; the two that add bytes are 1+2 (the X menu) and 5 (the Emrakul carve-out), paid
by nine prose trims that remove no decision rule.

| # | before → after | paid by |
|---|---|---|
| 1 | **The X-menu block is rewritten so the `Decline` row has one named shape and everything else announces.** `THE X MENU. The ask prints … at 0 or less take the "Decline" row.` / `When option 1 carries a life bill … and then take "Decline": no listed X is a gain. … Otherwise announce the largest X the ceiling allows.` → `THE X MENU OPENS AFTER THE MANA IS PAID: no stop and no gate reaches a row on it. THE "Decline" ROW IS YOUR ANSWER IN ONE SHAPE ONLY - a "DRAW PUNISHERS on the battlefield: theirs" paragraph is printed, and then no listed X is a gain. With no such paragraph on the screen Decline hands back nothing: the card returns to hand, your sources STAY TAPPED, and the mana stays floating until the step ends it. The largest listed X buys that same spent mana back as X life and X cards. ANNOUNCE THE LARGEST X THE CEILING ALLOWS, and where no ceiling sentence is printed the largest X on the list.` | **`123 seq 129/130` and `152 seq 270/271`** — the corpus's only two `dropped after its X was announced`, both this seat. At `123 seq 129` the cast-row reasoning never mentions the five stops at all; at `seq 130` the model discovers stop (3) at the X menu, quotes wave-79's `ANSWER THEM AT THE CAST ROW: taking that row PAYS THE MANA` **three times in its own reasoning**, and Declines anyway — 12 mana gone, 9 life and 9 cards refused. The wave-79 edit is read and is in the wrong place to help: by the X window the stop is already failed, and the menu needed its own least-bad answer. The new rule is unconditional, keyed to a printed paragraph, and cannot misfire |
| 2 | **Stop (4) stops reading as a cap on X.** `- THE SIZE FLOOR IS STOP (4), and at 10 life or less it is lifted: cast there at the largest X offered. The "[<- best X for this cast: ...]" badge ranks X values and does not know this floor; the floor wins.` → `- STOP (4) IS A CAST-OR-DO-NOT TEST ON THE CAST ROW AND NEVER A CAP ON X; at 10 life or less it is lifted.` (and `ahead of the tap-out gate and the size floor` → `ahead of the tap-out gate and stop (4)`) | **`126 seq 178/179`**, quoted in §1. The words `SIZE FLOOR` and `not lifted` composed into "keep X small"; the model announced X=1 off a max-X=3 row at 19 life, tapped out, and lost to the Bond/Blood loop the next turn. Stop (4)'s own text was read correctly in wave 79 — it is the *nickname* that inverted, so the nickname goes and the test stays verbatim |
| 3 | **The Staff subtraction on the library ceiling is DELETED — the render now does it.** `THAT K HOLDS BACK ONE DRAW ONLY - your draw step. EACH Staff of Nin … draws you one more card at your next upkeep and that K does not count them, so announce K MINUS ONE PER STAFF, never larger` → `K IS THE ANSWER: announce K, subtracting nothing. It has already held back every draw you cannot decline - your draw step and one per Staff of Nin on your battlefield line, which that sentence names one by one.` | **`146 seq 193`.** The ask reads `3 draws are already owed that you cannot decline (your next draw step, which you cannot decline, plus Staff of Nin's upkeep draw, plus Staff of Nin's upkeep draw) … X=9 is the largest value on this menu the library pays for.` The guide's rule says announce 7. The model announced 9 and its reasoning checks the arithmetic exactly (`Library 12. X=9 draws 9. Leaves 3. Next turn draw step (1) + Staff 1 (1) + Staff 2 (1) = 3. Exactly matches.`). The rule's WHY no longer holds against the render, so it is a deletion, not a re-phrase; the no-sentence fallback is kept unchanged |
| 4 | **A land floor is numbered into the bottom order, above it.** `BOTTOM IN THIS ORDER: Emrakul; then any "(copy 2 of 2 in your hand)" duplicate, a duplicate LAND included; then a land beyond the fifth; … Keep the LANDS and at least ONE answer.` → `BOTTOM. READ "counted by the engine: N lands" FIRST. AT N OF 5 OR FEWER NO LAND IS ON THIS LIST, whatever the hand needs to cast: a hand short of mana is a reason to keep every land in it, never to send one. THEN IN THIS ORDER: Emrakul; then any "(copy 2 of 2 in your hand)" duplicate; then a land beyond the fifth; … Keep ONE answer.` | **`162 seq 4`.** The model read the order list, found no rung matching, and went outside it. `Keep the LANDS` existed — as a trailing summary *after* the list, which it never reached. The floor is now a numbered check above the list, keyed to the engine's own `counted by the engine: 2 lands, 5 spells` header, and the one clause the model leaned on (`a duplicate LAND included`) is gone; at N of 6+ the `land beyond the fifth` rung still covers duplicate lands |
| 5 | **§EMRAKUL gains a lethal carve-out as step 1, above the absolute.** `- When the row IS printed and NO Staff … take it` / `- WITH ANY STAFF OF YOURS ON THE BOARD, EMRAKUL IS NOT YOUR ANSWER, at any mana and any life, and neither HAVING NOTHING ELSE TO DO nor THEIR LIFE BEING LOW is an exception.` → `1. THEIR PRINTED LIFE IS 15 OR LESS, their battlefield line reads 0 creatures, and no "LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs" paragraph is printed: TAKE THE ROW. Nothing on that line blocks, the 15 arrives next turn, and this step outranks every line below it and the tap-out gate.` / `2. …` / `3. WITH ANY STAFF OF YOURS ON THE BOARD AND STEP 1 UNANSWERED, EMRAKUL IS NOT YOUR ANSWER, at any mana and any life, and HAVING NOTHING ELSE TO DO is not an exception.` | **`152 seq 354`**, t48: opponent at 5 life with 0 creatures and 15 lands, the seat at 66 life with 19 untapped sources, two tapped Staffs and an Emrakul row. The model quoted the absolute back verbatim and refused it: *"This is explicitly telling me NOT to cast it. If I ignore it, I might fail a hidden test. But the prompt also says 'Your goal is to win before the opponent does.' … Casting Emrakul wins immediately. I will prioritize winning."* It was right — the game ended the next turn at −12 instead of three Staff turns later, and the rule as written had no falsifier on that board. Two Staffs stay ranked above Emrakul everywhere step 1 does not fire. The converter clause is preserved untouched and gates step 1 as well |
| — | **Byte payers, no decision rule lost:** `You never have a creature and nothing here attacks.` (false once step 1 of §EMRAKUL fires, and it contradicted it); `which is stop (5) of its own section` (cross-reference); `that tag is what every counterspell reads on your own turn, and it names THIS window, never your hand` (the render's own `HELD:` sentence now says it on every such tag — 267 prompts in wave 79, no recurrence of the failure it answered); `each drawing you a card at your upkeep and pinging 1 for a tap` and the Elixir/Path/Forced-Fruition cost and copy-count restatements (the rows print them); `that body grows their board every turn and one ping ends it` (rationale); `because the chain does not stop where that row's gloss stops` (rationale); `An "INCOMING THIS COMBAT ... you would be at N" line tells you what this turn costs.` (no imperative); `every card you draw costs 1 life per such permanent and they stack` → `the DRAW FORECAST prices them` (the forecast prints the multiplication); `subtract that total from your life` → `prints what it leaves you at` (the render does the arithmetic); `so it kills every counted creature whose printed TOUGHNESS is N or less` → `names which of them DIES and which SURVIVES` (the row prints `DIES at 2 (before prevention): … SURVIVES: …`) | bytes |

**Explicitly REJECTED.**
- No change for the §PUNISHER BOARDS carve-out or the §SWEEPERS K table: both were obeyed at every window
  they governed this wave and neither bought a loss.
- No new rule for the hold latch. The suppression in §2 A-1 is an ENGINE item; the guide cannot answer a
  window it is never shown, and telling the pilot to distrust the hold row would contradict §YOUR WINDOWS
  for the 28 holds that released correctly.
- No rule for the "the guide is a suggestion / the prompt says win faster" reasoning at `152 seq 354`.
  Naming a rationalization does not suppress it; edit 5 removes the board that produced it.

## 4. Not checked
No `blockers` and no `reveal` seam (0 records each) and exactly ONE `attackers` record — the Emrakul
attack in `125v152` — so §SWEEPERS' crack-back arithmetic and the whole blockers DP are untested here.
1 `bottom` record and 4 mulligan asks, so most of §MULLIGAN's ship branches ran once or not at all. No `DISCARD PUNISHERS` paragraph and
no `{... CLEANUP PRICE at X=N ...}` tail appeared on any of the 529 prompts, so §ORDER step 4 and Revelation
stop (2) are **UNTESTED for an eighth wave**; I trimmed nothing there. `own_loop_verdict_lines_rendered` 0
and `own_loop_windows_asked` 0 on all six seats, so U10/DH F6 is UNTESTED here; `stop_reached_*` 0
(T2 UNTESTED); `menu_single_outcome` 0 and `single_outcome_rows_spared` 0 (U13 UNTESTED here);
`crossphase_replayed` 0 (U8 UNTESTED here). I did **not** open `askreplay/`, did not sample the 36
`ask_key_continuation_differs` windows, did not reconcile the 1 `forced_close_unrecorded` or the 1
`async_drop` against stderr, did not audit the 76 rendered crack-back verdict lines against the board
(0 `crackback_lethal_blocked_away` on this seat), did not audit the U3/U4/U6/U7 cover/tap-bill/cycling/
`board sweep:` clauses (this deck casts no creature and cycles nothing), and did not compute the row-1 pick
rate for U5 — all engine-seat work. I verified no card against `primitives/*.txt` or an external Oracle
source this wave: Exquisite Blood, Sanguine Bond, Emrakul, Lightmine Field and Staff of Nin are quoted from
the engine's own `{effect:}` / `{target text:}` renders and from the game log. I opened opponent seat logs
for the results row and the shared stderr only; I reconstruct no opponent's line of play. I adjudicated
only the lane predictions my own windows touch (U1/DG + DH F5 PASS, U11 UNTESTED, DC F2 / DH F3 FAIL,
wave-79 B-1 FIXED) — the rest is the engine seat's. I made no general-guide and no strategy-writing-skill
proposal: all five edits are inside this deck's own guide, edits 1-3 are local to a seam (the X menu) no
other deck in the pool has, and edits 4-5 are instances of amendments the skill already carries
(A26 — a floor that must beat a list is numbered into it; A30/A308 — every absolute has a lethal carve-out
above it as a numbered check).
