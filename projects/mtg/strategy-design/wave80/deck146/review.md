# Wave 80 - deck 146 (Orzhov dungeon midrange) per-deck review

Seat files: `matchups-20260912-074153-final/*ai_baka_deck146-0x*-vs-*.jsonl` (6 games, all on the
20-game binary `wagic-a7a6b8942-w80step1` = cf72a002f; none of my games is the `162v123` rerun, so
lanes DI/DJ are UNTESTED here by construction). Live guide reviewed = the wave-79 edition installed
at 36ae77199 (19,998 B). Opponent seats read for board and log context only.

Seat census: **332 model decisions** (ask 274, attackers 27, priority 24, blockers 5, reveal 2) plus
36 `hold_event` records. **0 fallbacks of any kind** - no `unparsed_reply`, `empty_reply`,
`reply_truncated`, `action_before_plan`, `defer`, `recovery` or `forced_close` on this seat; the
corpus's one `priority`/`unparsed_reply` and one `reveal`/`engine_answered` are not mine, so there
is nothing to quote. `thinking` on and native reasoning on 332/332 (median 5,608 chars, max 18,502).
Latency p50 89.7 s, p95 204.4 s, max 418.0 s. `protocol_deviation` compliant 303 / `unlabelled_plan`
26 / `plan_absent` 3 / `answer_label_absent` **0**. Prompt bytes: mean **13,860** (wave 79: 15,000),
max 24,575, **55 of 332 over 20 KB (16.6 %; wave 79: 22.7 %)** - U16's fold paid, see MED 3.
Gameend sums for this seat: `ask_replays_reserved` 47 = `ask_replays_cache` 46 + `crossphase_replayed`
1 + `_repeat_latch` 0, `ask_key_continuation_differs` 16, `crossphase_identical_reputs` 18 /
`crossphase_board_unchanged` **0** (fifth wave at zero), `hold_events` 36 (`verdict_clamped` 20,
`reopen_rows_moved` 15, `reopen_new_lethal` 1) / `hold_verdict_safer_ignored` 20 /
`hold_reopened_new_threat` 0, `hold_windows_skipped` 124 (cast 113 / priority 11),
`hold_released_turn` 10, `crackback_verdict_lines_rendered` 38 / `crackback_lethal_blocked_away` 1 /
`stack_death_verdict_lines_rendered` 10, `plan_cast_steps_closed` 63, `stack_drain_windows_asked` 9,
`blocker_forecast_rows` 82 (multi 36, gang 2, collapsed 2), `identical_option_asks_resolved` 6,
`plan_names_uncastable_zone_card` 8, `plan_names_stranded_card` 1, `async_drops` 1,
`chain_windows_only_selfharm_cast` 102. **Zero renders/records on this seat of `own_loop_*`,
`menu_single_outcome` / `single_outcome_rows_spared`, `forced_close_*`, `stop_reached_*`,
`mana_only_windows_skipped`, `cached_replay_reasked`, CLEANUP DISCARD and the `AT BEST` header face**
- all UNTESTED here, not passed.

| # | Opponent | Result | Turn | Final life | What decided it |
|---|---|---|---|---|---|
| 1 | 162 | **LOSS** | 17 | 0 / 1 | **seq 52** - at 7 life, a NOT-lethal 2-attacker swing sent entirely at the face while `W1. Ob Nixilis, the Hate-Twisted [3 loyalty left]` - the board's named DRAW PUNISHER, killable by one 3-power attacker - sat unattacked; its pings then took exactly the 5 life the seat lost by, dead at 0 with the opponent on **1** |
| 2 | 130 | **WIN** | 17 | 21 / -9 | Nadaar + Goblins on curve; **seq 30** alpha strike on the "That IS lethal" line, all 4 bodies sent |
| 3 | 123 | **LOSS** | 18 | -2 / 11 | Thraben Doomsayer + Lightning Greaves + Intruder Alarm made **22 Human tokens on T16**; Soul Shatter killed the Doomsayer one turn too late (seq 27) and 21 attackers came in. No decision reachable |
| 4 | 152 | **WIN** | 17 | 12 / -10 | Lolth -3 for Spiders (seq 33), Emeria taken as a TAPPED land at seq 31, **seq 41** full alpha on the lethal line, 8 bodies |
| 5 | 125 | **LOSS** | 41 | 0 / 50 | A 41-turn grind under 2x Staff of Nin + Supreme Verdict + Elixir; **seq 141 paid 3 life** for an untapped Agadeem at 14 life with 15 untapped sources and a 2-card hand it could not cast - the seat died at exactly **0** |
| 6 | 126 | **WIN** | 19 | 9 / -9 | Soul Shatter on Sorin (seq 38), Command as an edict (seq 46), **seq 48** alpha on the lethal line |

**3-3** (wave 78: 4-2; wave 79: 4-2). Hands are real - the 123 loss is a combo hand and the 125 loss
is a bad matchup with a flooded draw - but the 162 loss and the 125 loss each carry one decision.

## 0. Wave-79 edits and the §U items that cite this deck: did each pay?

- **Edit 1 (BLOCKED DAMAGE IS ZERO + "SEND EVERY BODY LISTED, never a chosen few") - FIRED 3 times,
  PASSED 3/3.** Every `ATTACK TOTAL ... That IS lethal` window on this seat sent the whole board:
  `146v130` seq 30 (4 listed, 4 sent, 12 damage into a board at 3), `146v152` seq 41 (7 listed, all
  sent, 20 damage into 10), `146v126` seq 48 (8 listed, all sent, 21 into 12). All three games were
  won on that swing. Wave 79's failure shape (`146v130` seq 41, "Nadaar survives, deals 4 damage"
  about a BLOCKED attacker) has **no counterpart in 27 attacker windows this wave**, and the
  arithmetic is never argued with again.
- **Edit 2 (Acererak pinned below any row that leaves a body; "any clause counting him as a blocker
  is FALSE") - PAID, and the engine half is now redundant.** Acererak was never taken over a listed
  body row this wave. At `146v123` seq 13 and seq 19 he sat on the menu next to live rows and was
  declined both times. He was cast 7 times in `146v125` (seqs 104-126) purely as a repeatable
  venture with 7+ spare sources, which is the rule's own carve-out. The "any clause counting him as
  a blocker is FALSE" half is now dead weight - see U3 below - and I removed it for bytes.
- **U3 (crack-back cover crediting a body the row does not leave) - CLOSED, both halves.** All
  **3/3** Acererak cast rows under a printed crack-back now read
  `{crack-back cover: NONE from this body - Acererak the Archlich's own enters-the-battlefield text
  moves it straight back off the battlefield as it resolves (read the card text on this row), so it
  is not there to block during the CRACK-BACK NEXT TURN attack above (3). This row is priced as
  adding no blocker at all}` (`146v162` seq 38, `146v123` seqs 13 and 19). The mechanism sentence is
  now on every cover and stay-home clause (8 renders) and IS trample-aware - `146v123` seq 29:
  `HOW A BODY REMOVES POWER FROM THAT TOTAL: an attacker that is BLOCKED deals its combat damage to
  the blocker, not to you - all of it, whatever the blocker's size, and a 1/1 in front of a 5/5
  stops the whole 5. ... None of these attackers has trample.` Wave 79's second false shape (a
  Command row with a dead body-making mode still crediting 1 body) did not recur: no Command cast
  row on this seat carried a cover clause this wave, so that half is UNTESTED, not re-failed.
  **Wave 79's HIGH 2 (the model disbelieving TRUE cover figures for ~2,000 reasoning chars) is also
  closed** - no window this wave argues with a cover or best-case figure.
- **U14 (a "NOT lethal" blockers header against compulsory draw-punisher damage) - NOT RENDERED, and
  UNTESTED.** Corpus-wide there are 26 blockers lethality verdicts and **not one mentions the draw
  forecast**; only 5 sit in a prompt that prints a DRAW FORECAST, and only ONE of those forecasts
  damage to the seat being asked: `146v162` seq 44 - `Your life: 9. Unblocked, these attackers deal
  up to 3 - you would be at 6 - NOT lethal: block only where the trade favors you.` with
  `DRAW FORECAST: ... = 4 life LOST BY YOU ... you would be at 5` fourteen lines above. 9 - 3 - 4 =
  2, so the fold would not have flipped that verdict and the flipping state never arose this wave.
  I cannot tell from the bytes whether the fold shipped and stayed silent or did not ship; the
  engine seat has the source. Either way the seat blocked correctly (both Silencers on the Fate
  Unravelers, both Unravelers dealt with).
- **U16 (deck146's bytes) - PAID.** Mean prompt 15,000 -> **13,860 B**, over-20-KB share 22.7 % ->
  **16.6 %**, max 23,712 -> 24,575 (the max is up, driven by the game log - MED 3). The 1.4 KB
  hold-row + HOW-A-HOLD-ENDS block of wave 79 is now one folded bracket plus the block.
  `plan_line_missing` did NOT improve: **35 of the corpus's 82** are on this seat (10.4 % of 338
  records, the corpus's worst; deck123 and deck126 are at 0.0 %) - see LOW 1, none of them is a
  wrong answer.
- **DH F3 (a lethal face as an EVENT keyed on threat identity) - FIRED ONCE AND PAID.** `146v125`
  seq 214: `reopen_new_lethal ... a NEW crack-back threat over the same [crack-back verdict: LETHAL]
  face at the cast seam - the objects behind it are not the ones this hold was taken over`, re-opening
  a hold whose `window_seq` was **141** (turn 31) at turn 40, when Emrakul, the Aeons Torn landed.
  The seat was asked (seq 215) and cast Soul Shatter to make them sacrifice Emrakul. Without the
  identity keying that hold would have swallowed the window.
- **The 20 `hold_verdict_safer_ignored` on this seat are all one benign pair** - every one reads
  `held [stack death verdict: you survive the stack] over live [stack death verdict: nothing lethal
  on the stack]`. Both faces are non-lethal, so nothing was hidden by the clamp; `hold_reopened_new_threat`
  is 0 and no equal-rank new threat arose at a held priority seam on this seat.
- **T16, T3/CZ, DC F1, the `AT BEST` face, the gang-block trample spill - none recurred.** `AT BEST`
  renders 0 times in 332 prompts, `no assignment` 0, `LOSES THE GAME` 0; only 2 real `GANG BLOCK:`
  clauses rendered, neither on a trample attacker. UNTESTED, not passed.

## 1. Game by game - the deciding decisions

**vs 162 (LOSS, T17) - decided at `...vs-ai_baka_deck162.jsonl` seq 52 (T15 Attackers, 7 life vs
10).** The prompt lists two 3/2 Silencers, one Fog Bank as their only untapped blocker,
`ATTACK TOTAL: 2 attackers listed, 6 total combat damage ... That is NOT lethal: they survive at 4`,
and, under it, `Their planeswalkers/battles - you may send any attacker at ONE of these INSTEAD of
at them: W1. Ob Nixilis, the Hate-Twisted [planeswalker] [3 loyalty left: combat damage removes that
many counters, and it dies at 0]`. The same screen names Ob Nixilis as the board's DRAW PUNISHER
(`Every card YOU draw costs you 1 life`) and forecasts the seat's own draw step at 2. One Silencer's
3 power is EXACTLY the 3 loyalty. With one blocker they cannot stop both an A#>W# and a face
attacker: whichever they block, either the punisher dies or 3 damage lands. The seat sent both at
the face; **W1 appears nowhere in its 1,300 characters of reasoning**, which are spent instead on
whether a flier can block a ground creature. Ob Nixilis then took 2 life at T15's draw, 1 at T16 and
3 at T17, and the game ended with the seat at 0 and the opponent at **1**. That is the exact margin.
**STRATEGY -> guide, Edit 1.** (It also matters that this is a NOT-lethal swing: the guide's LETHAL
rule correctly owns the other case and must keep owning it.)

The same game's last window, seq 53 (T17 Upkeep, 3 life vs 1), is NOT a decision item - the seat had
eight mana, no instant in hand and only the Hive animation on the menu, and it took it. It was dead
in its own draw step. But the prompt SAID so - `DRAW FORECAST: ... 3 x 1 = 3 life LOST BY YOU ... if
it resolves as forecast you would be at 0; that KILLS you` - and the seat still wrote a plan for a
main phase and a combat it would never reach (`PLAN: Animate Hive now. Main 1: cast Kaya, -3 Master
of the Feast, attack all. Main 2: cast Barrowin.`). See HIGH 2.

**vs 125 (LOSS, T41) - decided at seq 141 (T31 Main 1, 14 life vs 22).** The window is the Agadeem
3-life menu. On the same screen: `Mana available: 15 total`, `Your hand (2 cards): Soul Shatter
[no cast row now: you already answered this phase's Casting decision] ; Vanishing Verse [no cast row
now: it must have a target and there is no legal target on the board]`, `Your battlefield (15
permanents ... of which 0 are creatures ... 15 are lands)`, opponent board 2x Staff of Nin pinging
for 2 every turn. Row 1 prints its own price: `{this payment puts you at 11}`. The seat read the
guide rule back verbatim, evaluated it correctly, and then overrode it:
> *"Since I don't have a card in hand that needs exactly one more mana, I should probably just tap
> it. ... the guide explicitly says to tap by default unless (a) and (b) are met. (b) is met (no
> crack-back), but (a) is not. So I should tap it. ... **I'll go with 1 (pay 3 life) because having
> an extra untapped land is usually better** ... But the guide says tap by default. I'll stick to
> the guide: tap. ... **Actually, it doesn't matter much. I'll choose 1.**"*

The rule was not misread; it was out-argued by "an extra source is generally good" and "it doesn't
matter much". The game ended with the seat at exactly **0** life against a 2-per-turn ping - 3 life
is a turn and a half of that clock. **STRATEGY -> guide, Edit 2.** The control is in the same
corpus: `146v152` seq 31, the same menu at 12 life with `[cannot pay now: needs 5 mana, you have 1
untapped source]` on the only card in hand, answered `2 (tap)` with the reason *"I have no spells
that can be cast with the single {W} mana this turn"*. When (a) is obviously false the rule holds;
when (a) is merely false it does not.

The rest of that game is a flood, not a decision: from T29 the seat's board was 15 lands and 0
creatures every turn, it held two uncastable instants, and it got exactly ONE attackers window in
41 turns (seq 122). Nothing in the deck answers two colorless Staffs once Kaya is in the graveyard,
which the guide already says.

**vs 123 (LOSS, T18) - not a decision.** T16: `Opponent used: Create human with Thraben Doomsayer
(x17)` then `(x5)` under Lightning Greaves and Intruder Alarm - 22 1/1s in one turn. The seat killed
the Doomsayer at seq 27 with Soul Shatter (`{right now: they sacrifice Thraben Doomsayer (MV 3,
their highest)}`), correctly, but the tokens already existed. At seq 29 it attacked with its lone
Silencer into 21 untapped blockers, and the render told it exactly why that was still right:
`{crack-back cover, STAY HOME: keeping all 1 of them back covers 1 of that 22, leaving 21 -> you
would be at -2, which still KILLS you}`. **NOTE for the engine seat so nobody chases it:** the seat
gets NO blockers window at T18 in this game, and that is CORRECT, not a suppression bug - Intruder
Alarm's `Creatures don't untap during their controllers' untap steps` left its one attacker tapped,
and the opponent's own attackers prompt confirms `They have 0 untapped creatures able to block`.

**vs 130 (WIN, T17), vs 152 (WIN, T17), vs 126 (WIN, T19) - clean.** 130: Nadaar + dungeon Goblins,
`dark pool` taken over a Treasure room at seq 18, full alpha at seq 30. 152: Verse on Intrepid
Adversary T5, Lolth cast and -3'd the same turn (seqs 32-33), Emeria taken TAPPED at seq 31, alpha
at seq 41. 126: two partial swings that are guide-compliant, not misplays -
seq 18 `ATTACK: none` into two `(neither dies) (blocking trigger: they may gain 2)` walls, and seq 27
sending only the unblockable Barrowin and the deathtouch first-striker under a printed
`LIFE-TO-DAMAGE CONVERTER ... theirs - Sanguine Bond`; then Command as an EDICT at seq 46
(`{this mode's SACRIFICE half is an EDICT: they control 1 creature - Pride Guardian is sacrificed}`)
and the alpha at seq 48.

## 2. Engine / interface / card items

### HIGH 1 - the attackers menu's `[held back ...]` tag renders INSIDE an offered attacker's row and reads as a restriction on attacking
Repro `146v162` seq 52, rows A1 and A2:
`A1. Silverquill Silencer #1 (3/2) [indestructible] [held back, THIS creature could not block ANY of
their 1 creatures: Fog Bank (flying)] [their untapped blockers: Fog Bank (0/2) (neither dies (no
combat damage is dealt either way - prevented))]`
Both bracketed facts are TRUE and they are about opposite directions of combat, so on one line they
read as a contradiction. The prompt's own glossary paragraph exists only because of this ("It runs
the opposite way from a 'their untapped blockers' tag"), and it did not work: the model spent
roughly 1,300 of its 1,500 reasoning characters on this window asking what "held back" means -
> *"'held back' might just be a tag indicating they are on the board. ... Could 'held back' mean
> they are tapped? No, 'Your creatures that can attack: A1... A2...' implies they are untapped"* -
and concluding (wrongly, but harmlessly here) that Fog Bank could not block a ground creature. That
is the whole budget of the window that lost the game, spent on a tag that decides nothing: the tag
prices a body you KEEP HOME, and it is printed on the row that offers to SEND it. Fix direction:
move the "could not block any of theirs" fact out of the A-row and into the STAY HOME clause where
it is priced, or drop it when the row is an attack offer. Pin `146v162` seq 52.

### HIGH 2 - a forecast that says "that KILLS you" before your next decision is buried in the board dump, while the crack-back verdict gets a bracket next to the rows
Repro `146v162` seq 53 (T17 Upkeep, 3 life). The prompt contains, in this order: the board, then
`DRAW FORECAST: your next draw step draws 3 cards (1 + Howling Mine 1 + Dictate of Kruphix 1) = 3 x
1 = 3 life LOST BY YOU ... if it resolves as forecast you would be at 0; that KILLS you`, then the
menu, then - immediately under the rows, where the model actually reads -
`[crack-back verdict: their UNBLOCKED total reaches your life total, but a legal block prevents it -
with your best block: 0 damage still gets through, leaving you at 3. So passing this window does NOT
hand them the game]`. That bracket is TRUE about their attack and, on this board, actively
misleading: the seat does not live to block. The model's reply quotes the crack-back verdict and
never mentions the forecast, and plans two phases it will not reach. The engine already computes
both numbers and already knows how to hoist a verdict into the row block; the compulsory,
unavoidable, self-inflicted one is the one that is not hoisted. Fix direction: when a COMPULSORY
forecast takes the seat to 0 or below before its next window, print a verdict line in the same
bracket block as the crack-back verdict, and say that the crack-back verdict is moot. Same item as
wave 79's MED 1, one rung up: this wave the buried figure said KILLS and was still not read.

### MED 1 - a legend-rule STATE-BASED ACTION was answered from the ask cache
Repro `146v162` seq 27, `askreplay/1789216925-ai_baka_deck146-...-vs-ai_baka_deck162.jsonl`:
`{'kind': 'ask_replay', 'why': 'cache_replay', 'seq': 27, 'turn': 11, 'replayed_from': 24,
'question': 'LEGEND RULE CHOICE for Nadaar, Selfless Paladin ...', 'choice': '2'}`. The two rows on
that ask differ only by `#1`/`#2` and a `[summoning sick - cannot attack this turn, but CAN block]`
tag, and between seq 24 and seq 27 the surviving copy took a `+1/+1` counter from the storeroom room
(it is a 4/4 by seq 29). The replay happened to be right both times - the newly cast copy is the
sick one both times, and the guide's LEGEND RULE says bin the sick copy - but the key for this ask
is a pair of interchangeable-looking index labels whose meaning is reassigned every cast, and the
answer that is replayed is which INDEX dies. This is the one seam on my seat where a replayed answer
can destroy the wrong permanent. Fix direction: exclude the legend-rule SBA from the cache, or key
it on the tag set of the row rather than the row number.

### MED 2 - `plan_names_uncastable_zone_card` fires 8 times on this seat and the render answers it late
Repro `146v162` seq 53: `THE PLAN YOU LAST STATED (as you stated it, 1 window ago on turn 15;
"Acererak the Archlich" is no longer on your menu) reads "In second main phase, cast Acererak the
Archlich."` followed by `(note: this decision's list does not contain the actions your plan names.)`
The parenthetical is good and it fired correctly. The item is the rate: 8 of 332 windows carry a
carried plan naming a card the current menu cannot reach, and every one is a window where the plan
echo costs bytes and buys nothing. Worth a count in the engine seat's plan-echo section, not a fix
on its own.

### MED 3 - the GAME LOG is now the dominant prompt cost on this seat, at 52 % of the mean prompt
Measured over all 332 prompts: mean total 13,860 B of which the pre-`--- CURRENT SITUATION ---` game
log is **7,240 B (52 %)**, and in the four largest prompts the log is 13.4-15.2 KB on its own
(`146v126` seq 45 = 24,575 B total / 13,598 log; `146v125` seq 213 = 22,758 / 15,164). Every
over-20-KB prompt on this seat is over because of the log, not the menu: the situation block is
7.6-11.8 KB in all of them. The log is already batch-collapsed (`[x4 - 4 lines of this shape in this
batch; only the numbers in them differ]` renders correctly at `146v162` seq 53), so the next byte
win is a window, not more collapsing - the seat has never once needed a fact from turn 3 at turn 40.

### LOW 1 - 35 of the corpus's 82 `plan_line_missing` are this seat, and all of them are the same benign shape
Repro `146v130` seq 20: `'\n\nAttack with both to apply pressure and test their blocking
decisions.\nATTACK: A1, A2'`; `146v162` seq 22: `'\n\nPlay Plains, cast Nadaar, venture, attack with
Silencer, Hive, and Nadaar.\nCHOICE: 1 (Play Plains)'`. In every one the plan is PRESENT, is a real
plan, and PRECEDES the action line - only the literal `PLAN:` label is dropped. Under the owner's
rule the parser read all 35 without a fallback and 0 were `action_before_plan`. The cost is that
`off_protocol_bytes` counts the unlabelled plan as off-protocol (26 records on this seat, also the
corpus's highest), which makes this seat look non-compliant in the census when it is not. Worth a
class split in the counter, not a protocol change.

### LOW 2 - reasoning termination drift is still here and is now most of some windows
Repro `146v123` seq 29: after reaching `ATTACK: A1` the model writes ~900 further characters of
`Done. / Proceeds. / Output matches. / [Final Check] / [Output Generation] -> *Proceeds*`, restating
the format instruction three times. Same shape at `146v162` seq 52 and `146v152` seq 31. It is pure
decode cost (seat p50 latency 89.7 s) and it is worst on the windows with the least to decide. Same
item as wave 79's LOW 2; unchanged.

### LOW 3 - `[castable now]` on a hand card the engine already knows is dead
Repro `146v125` seq 185: `Your hand (2 cards): Soul Shatter {2}{b} [instant] [castable now]` while
the opponent line reads `of which 0 are creatures` and Soul Shatter's own row elsewhere says it does
nothing. The sibling tag on the same line is right (`Vanishing Verse ... [no cast row now: it must
have a target and there is no legal target on the board]`), which makes the Soul Shatter tag read as
a statement about usefulness. Third wave for this item; still only a hand-header/cast-row split.

## 3. Guide verdict: EDIT

Live 19,998 B -> revised **19,995 B** (`wave80/deck146/strategy.txt`). Two rules added, no rule
removed and no condition weakened; the bytes come from the compression listed under Edit 3 plus the
two clauses the engine's wave-80 fixes made redundant.

**Edit 1 - COMBAT gains a rule for their planeswalker on a swing that cannot kill (A333: the bullet
declares its own scope in its first clause; A334: flat statement, the LETHAL precedence is stated by
POSITION - the bullet sits below the LETHAL bullet and the unblockable bullet - not by a concessive
tail).**
- before: the guide has NO rule about attacking a planeswalker at any life total; `A#>W#` appears
  nowhere in it, and the only planeswalker text is Kaya's -3 ladder in SPELLS, which is about
  removal she may not have.
- after, inserted in COMBAT between the `[no creature they control can block this attacker]` bullet
  and CHECK 0:
  `- THEIR WALKER WHEN THE SWING IS NOT LETHAL: ATTACK TOTAL reads "NOT lethal" and one attacker's
  power reaches a listed walker's "[N loyalty left]" -> send that one as A#>W#, first when a DRAW
  PUNISHERS or converter line names it. What it makes EVERY turn beats damage to a life total you
  cannot reach this turn.`
- paid for by **`146v162` seq 52** (7 life, two 3-power attackers, one 3-loyalty Ob Nixilis printed
  as W1 AND named on the DRAW PUNISHERS line, the swing printed NOT lethal, both attackers sent at
  the face, the walker's pings then taking exactly the 5 life the seat lost the game by). Every term
  is printed on that screen: `ATTACK TOTAL ... That is NOT lethal`, `[3 loyalty left: combat damage
  removes that many counters, and it dies at 0]`, `A1. Silverquill Silencer #1 (3/2)`, and
  `DRAW PUNISHERS on the battlefield: theirs - Ob Nixilis, the Hate-Twisted`. Placing it BELOW the
  LETHAL bullet is deliberate: every lethal window on this seat this wave was won by sending
  everything at the face, and nothing in this edit may reach those.

**Edit 2 - the 3-LIFE MENU rule gains its WHY and closes the "it hardly matters" escape.**
- before: `... or no CRACK-BACK line is printed. NEVER AT 3 LIFE OR LESS.` - a correct test with no
  reason attached, so failing (a) was a rule the model could weigh against a general heuristic.
- after, appended to that bullet: `NEVER AT 3 LIFE OR LESS. 3 life is a turn under any ping, drain
  or punisher of theirs, and a source you do not spend THIS turn buys nothing: (a) failing ends the
  row - "an extra source is good anyway" is not a condition.`
- paid for by **`146v125` seq 141**, where the model quoted the rule, computed (a) FALSE and (b)
  TRUE, wrote *"So I should tap it"*, and then took row 1 anyway on *"having an extra untapped land
  is usually better"* and *"it doesn't matter much"* - at 14 life against a board that pings 2 a
  turn, in a game that ended at exactly 0. The control that shows the rule is otherwise sound is
  **`146v152` seq 31**, the same menu answered `tap` with (a) visibly false.
- this edit GROWS the rule by ~180 B. It earns them from `146v125` alone: this is the only guide
  rule in six games that was read correctly, evaluated correctly and then discarded.

**Edit 3 - the bytes.** Two clauses were DELETED because the engine now prints the fact and the
deletion is paid by a verified render, not by a judgement call:
  * Acererak's `and any clause on his row counting him as a blocker is FALSE` (-59 B) - superseded
    by `{crack-back cover: NONE from this body ...}` on 3/3 of his rows (see U3 above). What the
    render cannot say - that he is in no damage math and reaches no combat - is kept verbatim.
  * COMBAT's `Never block with a creature you attack with.` (-45 B) - the same sentence's premise
    is the bullet's own heading (`ATTACKING TAPS YOUR BLOCKERS for their whole turn`), and the
    BLOCK LADDER decides blocks from the parenthesis on offered blocks only, so no decision reads
    the removed sentence.
  * the ENGINE line's `Life you pay buys a cast THIS window.` (-38 B) - superseded word for word by
    Edit 2's expanded condition, which now states the same thing where the decision is made.
Everything else is wording only, nothing decided was lost: the deck header, cast-order entries 1/2/6
and the entry-9 paragraph, Verse's target and hold sentences, Soul Shatter's wrap, Kaya's opening
and ladder wrap, Command's timing and draw-direction lines, Silencer's Pelakka reference, Pelakka's
reveal sentence, the lands/pathway/hand-header bullets, Agadeem's never-cast bullet, Hive's preamble
and CHECKs 0/0b/2, COMBAT's opening and CHECK 0 tail, the collapsed-blocker, worst-tag, lifegain and
converter bullets, the combat-math bullet, the BLOCK LADDER preamble and rungs, VENTURE's dungeon
bullet, Lolth's and the legend-reset paragraphs, the hold paragraph, the mulligan header and carried-
plan bullet, CLEANUP DISCARD rung (2), and SANGUINE BOND.

## 4. What I did NOT check
- **`146v125` window by window.** 174 records over 41 turns. I read every attacker, Kaya/Lolth, Hive,
  Agadeem and Acererak window, the Pelakka reveal at seq 25, the hold re-open at seq 214 and the
  board lines at roughly every tenth record. I did NOT open the ~70 consecutive `Cast nothing right
  now` / `Hold priority` windows of T19-T39 individually.
- **Whether sending A1 at W1 at `146v162` seq 52 actually wins that game.** I established that the
  loyalty was exactly reachable, that one blocker cannot cover both attackers, that the walker is
  the board's named punisher, and that its pings account for the full losing margin. I did not
  simulate the rest.
- **Whether any line beats deck125's Staff/Verdict/Elixir configuration, or deck123's T16 combo.** I
  confirmed the hands and the removal the seat had; I did not search earlier turns for a faster clock.
- **The 47 `ask_replays_reserved`, 124 `hold_windows_skipped` and 18 `crossphase_identical_reputs`.**
  A replayed or skipped window never reaches the model, so its correctness for the new board is not
  decidable from a seat log. I traced only the 2-7 entries that appear in `askreplay/` per game, and
  of those I verified correctness by hand for exactly one (the legend rule, MED 1).
- **`ask_key_continuation_differs` 16 and `crossphase_replayed` 1** - counted off gameend, NOT
  verified window by window.
- **`hold_check:` claim arithmetic** - I read the bracket on every window I opened and found no
  false statement, but I ran no normalised diff of every bracketed window against its
  `hold_check_ref_seq`, so I make no corpus-wide claim about those brackets.
- **Whether U14's fold shipped.** I proved it does not RENDER in any of the corpus's 26 blockers
  lethality verdicts and that the one eligible window would not have flipped. I did not read source.
- **`own_loop_*`, `menu_single_outcome`, `forced_close_*`, `stop_reached_*`, `cached_replay_reasked`,
  `mana_only_windows_skipped`, CLEANUP DISCARD, `AT BEST`, the gang-block trample spill** - 0 renders
  or 0 records each on this seat. UNTESTED, not passed; the state never arose here.
- **The rerun binary's lanes DI and DJ** - none of my six games is the rerun, so nothing here tests
  them.
- **Opponent seats** beyond board and log context; no audit of any opponent deck's play.
- I read no engine source. Every claim above is from the corpus bytes and the live guide.

No `general-proposals.md` and no `skill-proposals.md`: Edit 1 is A333 and A334 applied to a section
ordering, Edit 2 is the installed skill's own "hard rails carry their WHY". Nothing here needs a new
amendment.
