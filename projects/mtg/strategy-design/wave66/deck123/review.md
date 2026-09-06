# Wave 66 - deck123 (Intruders of Thraben, WBU token combo) - per-deck review

Seat files (corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-064038/`), all
`1788694NNN-ai_baka_deck123-<ptr>-vs-ai_baka_deckX.jsonl`; opponent seats read for context.
Record: **2 of 6** (beat 146, beat 125; lost to 130, 152, 162, 126).

Seat census: 505 records / 6 games. Fallbacks on this seat: **20 `repeat_past_stop` - i.e. ALL 20
in the corpus**, 8 `unparsed_reply` (of 83), 3 `named_row_reask` (of 6). 0 wall misses, 0 reveal
stalls, 0 transport failures, 0 hangs. Max M reached: 102 (v126), 62 (v125), 36 (v162), 25 (v146),
0 (v130, v152). Wave 65's `x200` takes and 266 creatures are **reduced but not gone** (see HIGH-1).

## 1. Game by game

**v146 - WIN, turn 12, opponent to -80.** The clean line. Tutor -> Alarm (seq 11-12), Keeper +
Greaves turn 8 (seq 14-15), chain fired on turn 11 *inside the Blockers step* (seq 37-38) so the
team was untapped and unsick on turn 12; seq 50 `ATTACK: Lord of Lineage + 24 Vampires`. One
`repeat_past_stop` at seq 41 (M=24, stop=24) and the re-ask **recovered to pass at seq 42** -
H3 working exactly as designed. This game is the guide's own thesis executed.

**v125 - WIN, turn 59 (opponent decked at 44 life).** Deck sat at M=0 for 45 turns (Alarm out
turn 6, no maker until turn 46 - variance, not guide). Seq 91 took the repeat row correctly:
`x58` off `PLAN: L 58, C 0, stop 61; M 3 now` -> M=61, no refusal. **Deciding decision, seq 120**:
`ATTACK: A1-A60` took the opponent 61 -> **1**, one damage short. The stop had been set at seq 91
from L=58; by the attack window L was 61 (deck125 gains life), one maker was held home and one
token was still sick. The guide's `L + C + 3` buffer was consumed exactly by drift + held bodies.
Won anyway on decking; a lifegain opponent turns that +3 into 0.

**v162 - LOSS, turn 14, 0 to 18.** Built M=35 on turn 12 in its **own main phase 1** (seq 41-42),
then seq 45 `ATTACK: Thraben Doomsayer, Human, Human` - three attackers off 35 bodies, because
everything made that turn was summoning sick. This is verbatim the failure the guide already
names ("THE ALARM IS ALSO WHY A BIG M IS NOT A BIG ATTACK ... stood on 37 creatures ... and died").
STRATEGY, against an existing rule. It then passed 20 straight windows (seq 51-68) while a drain
took it 7 -> 0 with 36 creatures on the board.

**v126 - LOSS, turn 25, 0 to 21. The game of the wave; see HIGH-1 and HIGH-2.** Chain reached
M=102 on turn 13 against a stated stop of 26-27. Then **seq 114 (turn 14, Main 1) cast its own
Damnation and destroyed 96 Humans, 4 Vampires, the Doomsayer and Lord of Lineage** - board went
102 -> 0 in one window and the deck never recovered. The reply had *already retracted it* (see
HIGH-2). Everything after is a corpse: turn 18 rebuild to 23, turn 22 attack into three walls
under Exquisite Blood, dead turn 25.

**v130 - LOSS, turn 14, -1 to 20.** 17 asks, mulliganed a 1-land seven (seq 2), never saw a maker
or the Alarm; spent three Tragic Slips on a Goblin. Variance. One LOW guide miss: seq 5, turn 1,
two `Play` lines listed and it answered `Play no land right now` - it corrected in Main 2 the same
turn (seq 6), no cost.

**v152 - LOSS, turn 21, -4 to 24.** Never got M above 2; both makers were killed/blocked away and
it cast Intruder Alarm from exile for `{4}{u}` (seq 22) with no creature on board. One
`unparsed_reply` at seq 7 (Baka cast the same Tragic Slip the model had named - no change).

## 2. Engine / interface / card items

### HIGH-1 - `repeat_past_stop` costs a window and does NOT stop the overshoot: the re-ask is answered with the same count, and the second answer executes as given
File `1788694877-ai_baka_deck123-0x55ac3cfdf640-vs-ai_baka_deck126.jsonl`.
- seq 81 refused (`x37` at M=41, plan stop=26). seq 82, **same board**, named `x25` - executed.
  M 41 -> 66.
- seq 84 refused (`x34` at M=66). Rendered re-ask, verbatim:
  `[RE-ASK] Your CHOICE line names 34 repeats ("CHOICE: 3 (Create human with Thraben Doomsayer x34)") but your own PLAN puts you at 66 with your stop at 26, so every repeat in this window is past the stop you set. Answer again: 0 (pass) to stop here, or the repeat row with the stop you actually intend and a count that does not pass it.`
  seq 85 answered `CHOICE: 3 (Create human with Thraben Doomsayer x34)` - the **identical count**,
  with the identical contradicting PLAN. Executed. M 66 -> 100.
Recovery rate across the seat: 17 of 20 re-asks came back as `0 (pass)`; the 3 that did not
produced 59 extra bodies and drove M to 102. The guard is advisory by construction
(`AIPlayerGPT.cpp:31566`, "executes as given either way") and one re-ask per board
(`mPriorityReaskBoard != boardKey`, :31456), so a model that restates its count is unopposed.
Suggested shape (engine seat's call): on the *exhausted* re-ask, clamp N to `max(0, stop - M)`
rather than executing an unchanged past-stop count, or make the second refusal a hard pass.

### HIGH-2 - the H2b retraction vocabulary missed "Wait," / "Correct Plan:", and a first-wins CHOICE line cast a Damnation the reply had already retracted - this lost the game
Same file, **seq 114**, turn 14, Main phase 1, my_life 18 / opp 17, M=102. Rendered row:
`3. Cast Damnation {2}{b}{b} {right now: destroys 7 of their creatures (all of them carry a restriction against attacking), 102 of yours - ...}`
Reply, verbatim:
```
CHOICE: 3 (Cast Damnation)
PLAN: Damnation: N 7, K 0, M 102; hold (M > N, and K=0 means no attackers, so sweep is bad). Wait, Rule 3 says: "M at 4 or more - never." M is 102. Do not cast Damnation.
Correct Plan: Cast nothing right now.
CHOICE: 6 (Cast nothing right now)
PLAN: L=17, C=7, stop=27; M=102 now. M > stop. Attack with all 102 creatures.
```
`chosen_text` = Cast Damnation. The model reasoned correctly, quoted the right rule, named the
right row - and the engine ran the line it had retracted. H2b's headers are
"Correction:" / "Re-evaluating:" / "Actually," / "On reflection"; this reply used **"Wait,"** and
**"Correct Plan:"**, neither of which supersedes. Two cheap fixes, either alone would have saved
the game: (a) add "Wait," / "Correct Plan:" / "Corrected plan:" to the retraction headers; (b)
note that H2b's 3-non-blank-line window is measured *from the last coded line* - here the
retraction is on the same line as the PLAN and 2 lines below the CHOICE, so the window itself was
not the blocker, only the vocabulary was.

### HIGH-3 - `ANSWER: CHOICE: N (...)` is refused as `unparsed_reply`; the H2a protocol's own section name invites the prefix (6 of my 8 fallbacks)
The reordered `kReplyProtocol` (:215) says "Write your REASONING first, then your ANSWER, then your
PLAN". `REASONING:` and `PLAN:` are labels the model already writes; it generalised and wrote
`ANSWER:` too, in front of a perfectly well-formed CHOICE line, and the parser refused the whole
reply. Verbatim, all present in the corpus:
- `...vs-ai_baka_deck152.jsonl` seq 7: `ANSWER: CHOICE: 1 (Cast Tragic Slip)`
- `...vs-ai_baka_deck162.jsonl` seq 10: `ANSWER: CHOICE: 0 (pass)`
- `...vs-ai_baka_deck126.jsonl` seq 131: `ANSWER: CHOICE: 1 (Create human with Thraben Doomsayer)`
- `...vs-ai_baka_deck126.jsonl` seq 147 / 152: `ANSWER: CHOICE: 2 (Hold priority: pass now, and do not ask me again)`
- `...vs-ai_baka_deck126.jsonl` seq 149: `ANSWER: CHOICE: 1 (Sacrifice a creature of your choice)`
The other 2 (146 seq 8, 126 seq 142) carry **no** coded label at all - long prose ending in a
plan, the "33 replies with no coded line" shape from the brief. Fix: accept a leading
`ANSWER:` / `ANSWER -` before the real label (one strip), or rename the protocol's middle section
so it does not read as a label. **Did the Baka answer change the game?** 152 seq 7: no (Baka cast
the same Tragic Slip). 126 seq 149: no (Baka sacrificed a Human token, as intended). 126 seq
147/152: no (dead windows, Baka passed). 146 seq 8: no. **162 seq 10: yes, in kind** - the model
had reasoned explicitly "YOUR OWN UPKEEP IS THE WORST WINDOW ... CHOICE: 0 (pass)" and Baka tapped
Thraben Doomsayer in that upkeep anyway (game log turn 8: "You used: Create human with Thraben
Doomsayer"). One sick token, not the game. **No `unparsed_reply` on this seat decided a game**,
but 6 of 8 are one strip away from parsing.

### HIGH-4 - the H3 verdict clause `{right now: M=.., your stated stop=..}` is absent from 19 of the 20 windows it was built for
The clause is rendered from the **carried** `YOUR PLAN` block (`repeatRowStopClause`, :24810) while
the refusal reads the **reply's own** PLAN line (`repeatPastStop`, :31428). The two sources
disagree constantly: on this seat the repeat row rendered in 91 windows and carried the clause in
43, but of the 20 `repeat_past_stop` refusals **only seq 67 (v126) had it**. The carry itself is
the leak - after a refused reply the next prompt has no `YOUR PLAN` block at all (5 of 31 carried,
16%, vs 288 of 337 = 85% otherwise), and it also drops after many plain passes: seq 76 answered
`PLAN: L=17, C=6, stop=26; M=40 now; this window: pass (stop reached)` and seq 77 rendered no
`YOUR PLAN` line and no clause, so the model wrote `x20` into a window with no verdict on it.
Repro: same file, seq 76 (plan present) -> seq 77 (`grep -c "YOUR PLAN"` = 0, `grep -c "your
stated stop"` = 0, `fallback: repeat_past_stop`). Fix: render the clause from the same numbers
the guard uses (the reply's last stated stop, persisted), not from whether the plan block survived.

### MED-1 - `hold_windows_skipped` is enormous and the hold row is being taken correctly, but the seat still gets asked 20+ times in one drained turn
v162 seq 51-68: 18 windows in one turn (turn 14, Draw step), life ticking 7 -> 0, every one a pass,
none carrying a new fact. `hold_windows_skipped` 43 in that game / 891 in v125. The hold row was
taken at seq 49/50 and the run continued anyway because the life total changed each tick. This is
the "life total ticking re-opens the hold" class (AS H7 / AU R1); at zero legal actions and a
one-way life clock these windows have no decision in them.

### MED-2 - a 102-creature battlefield renders `Human #1-#35 (1/1) [...] x35` per bucket but the game log prints one line per token death
The turn-14 Damnation log in the seq 115 prompt is ~200 consecutive `Your Human died (that Human
was 1 of N copies ...)` / `Your Human (token) ceased to exist` pairs. The board line is already
bucketed; the log is not. Same seat, `...vs-ai_baka_deck126.jsonl` seq 115, `=== Turn 14` block.
This is most of that prompt and it crowds out the board the model has to read next.

### LOW-1 - `named_row_reask` x3, all in v125, all the same shape: a cast named off a menu that has no cast rows
`...vs-ai_baka_deck125.jsonl` seq 66 (`CHOICE: 7 (Cast Vision Skeins)`), seq 147 (`CHOICE: 5 (Cast
Idyllic Tutor)`), seq 154. All three at M=0 with the model hunting a maker; all three recovered.
`laterStepRouteClause` fires but the model re-named a card the menu did not list, twice at the
same seat within 90 windows.

### LOW-2 - `Play no land right now` chosen with `Play` lines on the menu, 4 times
v130 seq 5 (turn 1 - corrected in Main 2, seq 6), v146 seq 45, v125 seq 47/117/122/126/129.
None cost a game (all had 5+ sources), but Rule 0 is unconditional and the render already prints
`Land drop: NOT yet used this turn`.

## 3. Guide verdict: **EDIT**

Three of the six edits are paid for by the game this seat LOST outright (v126 seq 114), one by the
20 refusals, one by the near-miss in v125, one by the `ANSWER:` fallbacks. Revised guide written to
`wave66/deck123/strategy.txt` (start = the live `bin/Res/ai/baka/deck123_strategy.txt`; the file
stays inside the pool band). Edits, before -> after, each with the seq that paid:

**E1 - the guide states the engine's answer rule BACKWARDS.** (v126 seq 114 - lost the game.)
- before: `A reply that reverses itself is NOT thrown away: the engine executes the LAST answer line you wrote. Last corpus it wrote "CHOICE: 2 (Cast Damnation)" over "N 6, K 0, M 31; hold" and the Damnation destroyed its own 31 creatures.`
- after: the FIRST CHOICE line runs, a later one does not replace it, plus the seq-114 cost (102
  creatures) and the instruction to change your mind *above* the answer line.
- why: the engine is first-wins (`kReplyProtocol`, :215; `#W65-AO (G8)`). The live guide told the
  model the opposite, and at seq 114 the model relied on it - wrote the sweeper, retracted it in
  prose, wrote the correct row below, and the engine ran the first one.

**E2 - name the retraction headers that actually work.** (v126 seq 114.)
- before: (nothing)
- after: "Wait," and "Correct Plan:" are NOT retractions the engine honours; the only supersedes
  are "Correction:", "Re-evaluating:", "Actually,", "On reflection" - and the safe move is to not
  need one.

**E3 - the answer label is `CHOICE:`, bare.** (162 seq 10, 152 seq 7, 126 seq 131/147/149/152.)
- before: (nothing)
- after: `ANSWER: CHOICE: 0 (pass)` is not read as an answer at all - the reply is discarded and
  the built-in AI answers the window; six windows went that way, one of them tapping a maker in
  your own upkeep against a sweeper deck.

**E4 - the PLAN template taught an uncommitted answer.** (v126 seq 82, 85.)
- before: `this window: <x<N> / pass>`
- after: `this window: <x<N> OR pass - write ONE of them, never "x25 / pass">`
- why: the model copied the slash literally ("x25 / pass", "x34 / pass", "x0 / pass") in 24 of its
  repeat-row plans, including both past-stop takes that executed. A plan that names both is a plan
  that constrains neither.

**E5 - obey the RE-ASK.** (v126 seq 85.)
- before: (nothing, after "never x0")
- after: when the engine hands back `[RE-ASK] ... your own PLAN puts you at M with your stop at S`,
  the answer is `CHOICE: 0 (pass)` - never the same count again, with the seq-85 cost.

**E6 - the stop is against the life they will HAVE and the bodies you will SEND.** (v125 seq 91 ->
seq 120; v162 seq 41-45.)
- before: `M reaches L + C + 3: STOP.`
- after: same, plus: recompute in the ATTACK window from the life printed there, add back the maker
  you hold home and this turn's sick tokens, and against a lifegain deck add their last gain again
  - a stop of 61 set at L=58 met an opponent at 61 and 60 attackers left them at **1**.

Byte budget: the six edits added ~1.9 KB, paid for by cutting prose no seq paid for and that is
stated elsewhere verbatim - the "THE UNIT OF THIS TEST IS THE ROW" recap of Rules 3/4/5/6, the
named-row leak recap in WHAT TO CAST (RULE 0 already carries it), the Greaves haste/shroud
card-text restatement (the render prints it), the "SECOND-COPY BRACKET IS NOT PERMISSION" recap
(CL3 and the DECIDING SITUATIONS bullet already carry it), two DECIDING SITUATIONS Damnation
bullets that repeat RULE 3 verbatim, and the hold-row bullet's restatement of RULE 1's hold
paragraph. RULE 3 is byte-identical to the live guide. Final size 71,308 B (band 41,000-71,500).

Also corrected the now-false line in RULE -1 ("Last corpus: six looks, six keeps at the first hand,
no bottom ask") - this corpus took 4 mulligans and 4 bottom asks in 6 looks.

Not changed, deliberately: RULE 3 (Damnation). The model **quoted it correctly** at seq 114 and
reached the right answer; the rule did its job and the protocol threw it away. Editing it would be
paying for someone else's bug. RULE 1's stop headcount, RULE 2 (Marsh Flats - obeyed, v152 seq 15,
v125 seq 16, v125 seq 153), the COMBAT checks and the edict/Tragic Slip rules all held.

## 4. What I did NOT check
- I did not read the opponents' full seats except for outcome/log corroboration on the v126 and
  v162 games; opponent-side misplays are not assessed.
- I did not verify card primitives against Scryfall this wave: no card behaved unexpectedly on
  this seat (Damnation, Intruder Alarm, Thraben Doomsayer, Bloodline Keeper, Marsh Flats, Tragic
  Slip, Tribute to Hunger, Devour Flesh all matched their rendered text and their observed effect).
- I did not adjudicate the lane predictions (engine seat's job) beyond the H3/H2a/H2b evidence above.
- I did not measure latency or `async_drops` on this seat.
- No general-proposals.md or skill-proposals.md: every finding above is either a deck-guide edit or
  an engine item, and nothing here generalises past this deck's repeat row that the general guide
  does not already say.
