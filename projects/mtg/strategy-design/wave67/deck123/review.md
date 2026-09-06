# Wave 67 - deck123 (Intruders of Thraben, WBU token combo) - per-deck review

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-102638/`, seat files
`1788708NNN-ai_baka_deck123-<ptr>-vs-ai_baka_deckX.jsonl`; opponent seats read for context.
Record **3 of 6** (beat 126, 152, 146; lost to 162, 125, 130) - up from 2/6 on a guide edited
last wave.

Seat census: 328 records / 6 games. Fallbacks: **3** (2 `wall_miss_unrecorded_wall`, 1
`named_row_reask`) - was 20 `repeat_past_stop` + 8 `unparsed_reply` + 3 `named_row_reask` last
wave. `unparsed_reply` 0. `repeat_past_stop` 0 and `repeat_clamped_to_own_stop` 0 (**corpus-wide
0 for both**). `post_answer_overrun > 0` on 212 of 307 (69%). p90 latency 92 s, two records at
the 900 s wall. `hold_windows_skipped` 439 across the six games.

**The three wave-66 guide edits that could fire, fired.** E3 (bare `CHOICE:`): 0 `unparsed_reply`
on the seat, against 8 last wave. E4/E5 (one committed count, obey the RE-ASK): the repeat row
was taken 4 times all game long and **every one landed exactly on the stated stop** - v162 seq 28
`x21` at M=4, stop 25; v126 seq 16 `x22` at M=3, stop 25; v152 seq 23 `x19` at M=4, stop 23;
v146 seq 26 `x26` at M=3, stop 29. No re-ask, no overshoot, no 102-creature board. Wave-66
HIGH-4 is also fixed at the render: the `{right now: M=.., your stated stop=..}` clause was
present on **41 of the 42** repeat rows this seat saw (was 43 of 91).

## 1. Game by game

**v126 - WIN, turn 10, opponent to -3.** Doomsayer t6, Alarm t8, chain fired in Main 2 of t8 and
again in the opponent's t9 upkeep/draw (seq 14-23), so the team was unsick on t10: seq 33
`ATTACK: A1-A28`. The thesis, executed, with the repeat row taken once and exactly.

**v146 - WIN, turn 16, opponent to 0.** Same shape, one turn slower. seq 26 `x26` -> M=29 in its
own Main 1; **seq 28 it correctly declined the attack** ("The tokens created this turn are
summoning sick"), rebuilt in the opponent's t15, and swung `ATTACK: A1-A29` at seq 49. The
sickness rule the guide has carried for four waves is now being applied unprompted.

**v152 - WIN, turn 18, opponent to -119.** Two Damnations bought the time (seq 12, 18), Greaves
on Keeper seq 20, chain to M=34 on t16-17, Keeper transformed to Lord of Lineage seq 49, seq 53
`ATTACK: A1-A34` (flying 4/4s). One real cost: **the own-stop discipline covers only the repeat
row.** From seq 30 to seq 48 the model took the *single* "Create vampire" row ~12 more times at
M=25..34 against its own stated stop of 23, burning most of turn 17 - see MED-1.

**v162 - LOSS, turn 12, 0 to 20. The game of the wave; see HIGH-1.** The build was RIGHT this
time: 21 tokens made in the OPPONENT'S turn 11 (seq 28), so at seq 32 the board line read
"25 creatures, **25 of them able to attack right now**", each a 3/3 under Fateful hour at 2 life
= 75 damage against 20. It died in its own draw step, one window before its main phase, holding
lethal and holding a Devour Flesh it could have gained 3 life off.

**v125 - LOSS, turn 46, 0 to 36.** Every maker it drew was answered (graveyard at seq 92:
Doomsayer, two Bloodline Keepers). It sat at M=0 for 40 turns opposite a counter/control deck and
died to a turn-45 Emrakul. Mostly matchup and variance - the mulligan (seq 2, kept at 6) was
right - but two windows were spent on nothing: seq 69 and seq 73 cast Damnation off rows reading
`{right now: destroys 0 of their creatures ..., 0 of yours}`, and seq 41/92 cast a second
Intruder Alarm off rows reading `[second copy: ... a second copy changes nothing]`. See MED-2.

**v130 - LOSS, turn 28, -1 to 16.** deck130 is red LD/burn: all three Doomsayers were killed
(seq 24 graveyard) and Molten Rain / Lay Waste held it at 2-3 lands, so `{2}{b}{b}` Bloodline
Keeper was uncastable from turn 15 to the end. Variance/matchup. Two windows still went wrong:
seq 41 named "Cast Bloodline Keeper" off a menu that did not list it (`named_row_reask`; the hand
line read "cannot pay now: needs 4 mana, you have 3 untapped sources"), and **seq 43** cast
Devour Flesh at 0 creatures - see HIGH-2.

## 2. Engine / interface / card items

### HIGH-1 - PERCEPTION, and the render was not at fault: `ON THE STACK: 3 damage to you - you would be at -1; that would KILL you` was printed, and the model wrote "damage to 2"
File `1788708401-ai_baka_deck123-0x55ab261b61b0-vs-ai_baka_deck162.jsonl`, **seq 32**, turn 12,
Draw step, my_life 2 / opp 20. Rendered, verbatim:
```
ON THE STACK, waiting to resolve (top resolves FIRST - you can respond now):
  1 (top): ability: Ob Nixilis, the Hate-Twisted's triggered ability (deals 1 damage to you) ...
  2: ability: Underworld Dreams's deal 1 damage ...
  3: ability: Underworld Dreams's deal 1 damage ...
ON THE STACK: 3 damage to you - you would be at -1; that would KILL you
Mana available: 5 total ...
1. Cast Devour Flesh {1}{b} {right now: they control 2 creatures - they choose which one; YOU
   control 25 creatures - targeting yourself sacrifices one of them, your choice, and you gain
   its toughness} {leaves 3 of your 5 untapped mana sources untapped}
```
Reply: `CHOICE: 2 (Hold priority ...)` / `PLAN: Stack resolves (damage to 2). Main phase: ...
Attack with all 25 Humans (3/3 each, total 75 damage) to win immediately.` It was dead before
that main phase. Row 1 self-targeted sacrifices one 3/3 Human (Fateful hour, verified against
`mtg.txt:120583-120586`) and gains 3 - life 5, then 2 after the triggers, alive, 24 3/3s swing
for 74 into 20 with 2 blockers. Verified against Scryfall: Devour Flesh
(`mtg.txt:29133-29136`) and Thraben Doomsayer both match Oracle.
The interface did everything right here. The two items it leaves for the engine seat are small:
(a) the death verdict sits **above** the "Mana available" block and ~40 lines above the choice
prompt, while the blockers-step equivalent is rendered immediately beside its rows; and (b) no
row-level annotation connects the death line to the row that answers it - Devour Flesh's clause
says "you gain its toughness" but not "+3 - that is above the 3 on the stack". Everything else on
this seat that carried an arithmetic verdict ON the row (`x26` -> stop 29, `destroys N`,
`- DIES`) was obeyed; the one verdict rendered as a standalone header was restated wrong.

### HIGH-2 - a reply that argues itself out of its own answer with no coded second line still executes: `answer_replaced` needs a prose-only path, or the guide alone carries it
File `...vs-ai_baka_deck130.jsonl`, **seq 43**, turn 27, Main 1. Row 2 read
`Cast Devour Flesh {1}{b} {right now: they control 0 creatures - at 0 this does nothing; YOU
control 0 creatures - targeting yourself does nothing}`. Reply, verbatim:
```
CHOICE: 2 (Cast Devour Flesh)

PLAN: Cast Devour Flesh to sacrifice nothing (waste) or hold if I can't find a target? No,
Devour Flesh at 0 creatures does nothing. ... I will hold priority. ... I will pass this turn.
```
`answer_replaced: false`, `coded_answers: 1` - the retraction was in prose only, so the cast ran.
**The contrast is the finding**: the AV correction path WORKS when the model uses it. Three
windows on this seat show `answer_replaced: true, coded_answers: 2` and every one was an
improvement - v152 seq 39 (`Correction: M=33, Stop=23. We should pass.` then
`CHOICE: 3 (Hold priority ...)`; `latched_line` = the hold, `latched_coded_line: 2`), v152 seq 42,
v162 seq 31. 3 of 3 beneficial, 0 harmful. Cheap engine option, if the owner wants one: when
`coded_answers == 1` and the post-answer text contains a first-person abandonment of the named
row ("I will hold priority", "I will pass", "does nothing"), re-ask once rather than execute.

### MED-1 - the own-stop guard is on the repeat row only; the single-tap row past a stated stop is unguarded and ate most of a turn
`...vs-ai_baka_deck152.jsonl`. seq 29 the model passed correctly, citing "M is 25, which is
greater than the stop of 23". Then seq 30, 31, 33-40, 46, 48 took the *single* `Create vampire
with Bloodline Keeper #1 [cost: Tap]` row, taking M 25 -> 34. The repeat row beside it carried
`{right now: M=33, your stated stop=23, so this window would add to a count ALREADY AT OR PAST
your own stop}`; **row 1 carried no such clause**, only `[repeat: ... activated this turn 10
times already. This turn will not advance while you keep taking this option; you have taken it 6
times in a row with no other action in between.]`. Repro: seq 39, that file. Fix shape: put the
same `{right now: M=.., your stated stop=..}` clause on the single-activation row.

### MED-2 - five windows spent on rows the render had already priced at zero
Every one of these carried its own dead verdict and was taken anyway:
- `...vs-ai_baka_deck125.jsonl` seq 69, seq 73: `Cast Damnation {2}{b}{b} {right now: destroys 0
  of their creatures (0 without a restriction against attacking), 0 of yours}` - both Damnations
  gone against a deck that then landed Emrakul.
- same file seq 41, seq 92: `Cast Intruder Alarm {2}{u} [second copy: ... a second copy changes
  nothing]` with 0 creatures on the board.
- `...vs-ai_baka_deck130.jsonl` seq 43: Devour Flesh at 0 creatures (HIGH-2).
This is NOT a missing-guide item - the live guide states it at six separate places (RULE 1's
dead-row hold, RULE 3's "N at 0 - never", RULE 5's "at 0 this does nothing", the CL8 paragraph
"WHEN EVERY CAST ROW ON THE MENU IS PRICED DEAD", the Greaves bullet, the second-copy bullet). A
seventh statement would be noise, so no edit is proposed for it. What it argues for is an
ENGINE-side answer: these five rows are exactly the class the "NO LIVE CAST ROW ON THIS MENU"
banner already describes (it was rendered at `...vs-ai_baka_deck125.jsonl` seq 16), and the
banner did not appear on the menus above because at least one *other* row was live. A per-row
`{this row changes no number on the board}` tag, or ordering priced-dead rows below the decline
row, would put the verdict where the model demonstrably does read it.

### LOW-1 - both 900 s wall misses on this seat landed on a menu whose only cast row was dead
`...vs-ai_baka_deck162.jsonl` seq 7 and `...vs-ai_baka_deck125.jsonl` seq 16, both
`transport: curl=28,http=0,empty=1,connect_ms=20000,phase=wall`, `latency_ms 900025`,
`deadline_pct 100.0`, empty reply. Engine-seat material (transport), but the coincidence is
worth one line: n=2, both on `Casting decision` menus offering only a dead Devour Flesh, one of
them carrying the "NO LIVE CAST ROW ON THIS MENU" banner. Neither cost a game (Baka passed).

### LOW-2 - `log_window_kind` disagrees with the rendered ask
`...vs-ai_baka_deck162.jsonl` seq 7 records `log_window_kind: 'land_drop'` and `phase: 'Main
phase 1'` on a prompt whose header reads `Casting decision (Draw, YOUR turn)`. The phase-field
mismatch is the known wave-59 note; the `log_window_kind` one is new and would mislead any
census that buckets by it.

## 3. Guide verdict: **EDIT**

Two of the four edits are paid for by the game this seat lost with lethal on the board (v162
seq 32); one by a false statement about the engine that the corpus contradicts three times and
that cost a card once; one by the model transcribing the guide's own template text. Revised
guide at `wave67/deck123/strategy.txt` (start = live `bin/Res/ai/baka/deck123_strategy.txt`),
**71,487 bytes** (band 41,000-71,500; `stat -c %s`).

**E1 - the "would KILL you" line at a PRIORITY window has no rule.** (v162 seq 32 - lost the
game.) The guide covers the *blockers* header exhaustively ("this KILLS you ... you would be at N
AT BEST", three places) and says nothing about `ON THE STACK: N damage to you - you would be at
X; that would KILL you`, which is a different header at a different seam.
- before: (nothing; RULE 5 ends "...is NOT on the battlefield and does not change N.")
- after: a paragraph in RULE 5 - that header is the only fact in the window, the triggers resolve
  in the step you are standing in and not at combat, do not restate it in your own words, take
  the row that moves the number, a hold is never the answer - with the seq-32 cost quoted
  verbatim ("Stack resolves (damage to 2). ... Attack with all 25 Humans ... to win immediately").

**E2 - the guide FORBIDS the play that would have won that game.** (v162 seq 32.)
- before: `"they choose which one" or "at 0 this does nothing" on the row: no edict. Naming "Yourself" is for giving up a token, nothing else.`
- after: keeps the edict rule, and adds that naming Yourself is also a life gain - the row's own
  words ("targeting yourself sacrifices one of them, your choice, and you gain its toughness"),
  that Fateful hour makes the Humans 3/3, and that three life for one token out of twenty-five is
  the cheapest turn the deck will ever buy.

**E3 - the guide's statement of the answer rule is FALSE and the corpus contradicts it three
times.** (v152 seq 39 + 42, v162 seq 31 - all `answer_replaced: true`; v130 seq 43 - the cost.)
- before: `THE FIRST CHOICE LINE IN YOUR REPLY IS THE ONE THAT RUNS. A second one below it does not replace it - it is not even read.`
- after: the first line runs and **prose** never takes it back (seq 43, quoted); a second full
  CHOICE line DOES replace it when a line starting "Correction:" / "Re-evaluating:" / "Actually,"
  / "On reflection" stands between them - both halves or nothing - with the v152 seq 39 pair
  quoted as the shape that worked, and "Wait," / "Correct Plan:" still named as the pair that
  does not.

**E4 - the PLAN template's inline instruction is copyable, and got copied.** (v130 seq 17.)
- before: `        this window: <x<N> OR pass - write ONE of them, never "x25 / pass">`
- after: `        this window: <x<N> or pass>` with the instruction moved to a prose line below
  the template block. seq 17's plan read, verbatim, `this window: pass - write ONE of them,
  never "x25 / pass"` - the wave-66 edit's own parenthetical, transcribed into the answer.

Byte budget: the four edits added ~1.6 KB, paid for by cutting six passages that no seq paid for
and that are stated in full elsewhere in the same file - the summoning-sickness recap in RULE 1
(the paragraph 30 lines above it carries the rule and the cost), and five DECIDING SITUATIONS
bullets that restate RULE -1's mulligan line, RULE 0's mana-count line, RULE 0's which-land line,
RULE 1's own-upkeep paragraph, and the blockers "AT BEST" paragraph verbatim. None of those five
was violated this wave (0 land-drop misses, 1 correct mulligan, 0 mana miscounts, the upkeep rule
obeyed at v126 seq 19-23 and v146 seq 31-33). RULE 1's stop arithmetic and RULE 3 are unchanged.
The RULE -1 corpus figure was refreshed (six looks, one mulligan, correct).

## 4. Proposals

None. No general-guide or skill proposal is supported by this seat's evidence: everything I would
have proposed (dead-row discipline, the stop, first-wins) is already an amendment or already in
the deck guide, and MED-2 argues for an engine change, not a written rule.

## What I did NOT check

The opponent seats were read only around the deciding windows, not decision-by-decision. I did
not adjudicate the lane predictions (engine seat's job) beyond the four counters the brief asked
this seat for: `repeat_past_stop` 0, `repeat_clamped_to_own_stop` 0, no take past a stated stop
on the repeat row (the single-tap row is a different mechanism, MED-1), `unparsed_reply` 0. I did
not open the `.stderr` files, did not verify the `{library:}` X-ceiling clause (this deck has no
drawing X row and none appeared), did not check `reveal_fallback_pick` (0 on this seat), and did
not rebuild or run anything.
