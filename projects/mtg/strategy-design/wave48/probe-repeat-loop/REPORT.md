# Wave-48 lane N live probe: the unbounded legal loop (docket D1 + D13)

Stacked probe decks in the roster's numbering gap (198/199; the real roster ends at
deck164). deck198 = 12 Thraben Doomsayer + 8 Intruder Alarm + 32 lands, i.e. the docket's
own loop; deck199 = 60 Forest, so the opponent can never end the game and the loop is the
only thing that can. Both probe decks were DELETED from `bin/Res/ai/baka/` after the runs
(verified: highest deckN back to 164); they are preserved here as the record.

Binary: this lane's worktree build (F1 + F2 + F3 + D13). Endpoint: `spark:8084`
(qwen36-35b-a3b) via `WAGIC_GPT_URL`; the owner's `~/.Wagic/ai/gpt/endpoints.txt` was not
touched. Selfplay one-shot, headless, fastclock, under the 4G memory scope.

## Run 1 - live model (`live-model-seat.jsonl`)

`WAGIC_SELFPLAY_DECK0=198 DECK1=199`, real model on both seats.

- Game **completed naturally**: winner deck198, turn 14, opponent at -9.
- **43 priority decisions in the whole game**; the busiest single turn took **23** windows
  (wave-47's repro: 1,871 in one turn, no gameend at all).
- The F3 decline row rendered in **31** prompts, the F1 repeat row in **36**.
- The model **passed priority three times** - `CHOICE: 0 (pass)` - and every one of them
  was a window whose decline row it could see. Seq 34 is the loop ending: turn 12, Main 1,
  `[repeat: activated this turn 17 times already; you control 21 creatures ...]`, two rows
  plus `0. Pass priority (take no action this window)`, answer `CHOICE: 0 (pass)`.
- D13 in the field, seq 24: the tag read `activated this turn 7 times already ... you have
  taken it 9 times in a row with no other action in between` - the per-turn number and the
  loop-scoped number visibly disagreeing, which is the whole point of the second one.
- The model did NOT choose the repeat row in this game. F1's ROW is proven live; its
  DISPATCH is proven by run 2.

## Run 2 - forced repeat (`forced-repeat-seat.jsonl`, `stub-llm.py`)

The live model never picked the repeat row, so the dispatch was exercised deterministically:
`stub-llm.py` is a local OpenAI-shaped stub that answers the F1 row (with `x25`) whenever the
prompt contains one, seeds the loop by taking option 1 at other priority windows, and returns
an empty choice everywhere else so the heuristic plays the rest of the game. No engine change,
no forced-choice flag.

- 10 repeat decisions, each translogged ONCE with the count (`chosen_text` ends `... x25`),
  no `repeat_count_*` parse notes.
- Narration receipts: `You used: Create human with Thraben Doomsayer (you named 25 repeats
  of it)` followed by `Your repeated activation ran 25 of the 25 times you named` - **25 of
  25 on every plan**, i.e. one decision for 25 activations.
- This run does NOT terminate, and that is the stub: it is written to always take the repeat
  row and never pass. F1 makes the loop cheap and gives it a reachable stop; it does not, and
  by owner doctrine must not, force one. Termination is run 1's evidence.

### The bug this probe caught

The first forced run cut every named 25 to 1 or 2 (`the cost could no longer be paid`). The
loop's own untap rides Intruder Alarm's ETB trigger on the stack, and this seam is reached
during that resolution too - the source is still tapped and the option is legitimately off
the menu for a window or two. "Absent this window" is not "unpayable". The plan now ends on
absence only when the stack is EMPTY (definitive), and otherwise waits, bounded, for the
trigger to resolve. Nothing but a live game shows that.
