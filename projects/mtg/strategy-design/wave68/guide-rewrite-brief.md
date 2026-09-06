# Guide rewrite brief (owner ruling 2026-09-06 — read wave68/strategy-writing-skill.md Amendment 332 FIRST)

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use absolute paths in every command (cwd does not
persist). Do NOT edit anything under bin/Res, src/, or run git or wagic. Write only under
projects/mtg/strategy-design/wave68/rewrite/deck<N>/.

## Task (one agent per deck: 146, 152, 125, 126, 162, 123, 130)
Rewrite the live guide `projects/mtg/bin/Res/ai/baka/deck<N>_strategy.txt` (~71 KB, ~830 lines) into a guide
that holds ONLY decision-time instruction, per Amendment 332. This is a REWRITE from the rules outward, not a
trim: read the live guide once to harvest every decision RULE and deck-specific FACT it states, then write the
new guide from that list. Every rule the live guide states survives if it is a rule about what to do given
what the prompt prints; everything that is history, count, citation, or narration of past play is dropped
(the evidence already lives in wave6x/deck<N>/review.md — do not move it, just leave it there).

Inputs: the live guide; the deck list `projects/mtg/bin/Res/ai/baka/deck<N>.txt`; the card scripts
(`/usr/bin/grep -n "name=<Card>" -A8 projects/mtg/bin/Res/sets/primitives/*.txt`) for any fact you restate;
wave67/deck<N>/review.md and wave66/deck<N>/review.md for the rules most recently added (so you keep them).
For what the prompt PRINTS (the surface the rules key on), read ONE late-turn prompt verbatim:
wave67/lategame-specimen.txt (deck125) shows the composed shape every seat sees.

## Shape of the new guide
1. The deck in one paragraph: what it is trying to do, the engine, the win condition, the kill turn.
2. The engine's order of operations: what to cast/hold/activate first, and the printed conditions that change it.
3. Rules, each as: CONDITION (what the prompt shows) -> ACTION, one or two sentences, grouped by decision
   kind (land drop; what to cast; holds and passes; attackers; blockers; targets; X; discard; mulligan).
4. Card facts the prompt does not print (interactions, what a card is FOR in this deck, traps).
5. A short worked-situations section (5-10 items), each a situation and the right answer, no game references.
Plain prose and short lists; no headers that name waves, seats, or corpora; no "you did"; no counts of past
windows; no "newest only" ledgers; no meta-commentary about the guide itself.

## Size
Hard ceiling 20,000 bytes; expect 8,000-16,000. If a rule does not fit, it was not a rule.

## Verification (do both, print the numbers in changes.md)
- `stat -c %s` on the new guide (<= 20000).
- `/usr/bin/grep -ciE 'corpus|wave[- ]?[0-9]|\bseq\b|previous game|last wave|this wave|census|paid (for )?by|[0-9]{3}v[0-9]{3}|you (declined|took|cast|passed) [0-9]'` on the new guide must print 0.

## Output
- wave68/rewrite/deck<N>/strategy.txt — the full new guide.
- wave68/rewrite/deck<N>/changes.md — (a) the list of rules KEPT (one line each); (b) the list of things
  DROPPED by category with a byte estimate; (c) any rule you could NOT restate as condition->action and why;
  (d) the two verification numbers. <= 60 lines.
Final message: <= 6 lines — byte count, grep count, rules kept / dropped counts, anything you were unsure about.
