# Core-prompt / render proposals - deck126 seat (wave 47)

## G5 - Collapse identical TARGET rows the way blockers were collapsed

Lane M collapsed wide blocker lists (`B2-B22. Vampire #1-#21 ... x21`) and the battlefield line
already collapses runs (`Human #1-#40 (1/1) ... x40`). Target menus did not get the treatment.
Repro: `1787786558-ai_baka_deck126-0x55fa89ab8040-vs-ai_baka_deck123.jsonl` **seq 18 and 19**, turn
10 - **432 and 431 options, 116,128 and 116,148 characters**, of which ~425 are
`Deal 1 damage with Staff of Nin targeting Vampire #N [opponent's battlefield] [cost: Tap] {card
text: "At the beginning of your upkeep, draw a card. -- {T}: Staff of Nin deals 1 damage to any
target."}` repeated with only the index changing, including the full card text on every one. These
are the two largest prompts in the corpus by a factor of four; the seat's next largest is 18 KB.
The model answered both correctly (option 431, the player), so this is cost and not correctness -
but 116 KB per decision is 8x the seat's mean and it recurs on every wide board.
Two independent halves: (a) collapse an identical-except-for-index run of target rows into a range
the way blockers now are; (b) print a repeated `{card text:}` once per distinct SOURCE rather than
once per row - the source is the same Staff of Nin on all 431.

## G6 - Name the ability's own repeat count on the row, not only the source's

The engine's `[repeat: activated this turn N times already; you control M creatures]` tag is
present and readable (`...deck123-0x55fa8add5b20-vs-ai_baka_deck126.jsonl` seq 1567:
`[repeat: activated this turn 15 times already; you control 1540 creatures]`). It did not stop the
loop (see the seat file's HIGH engine item). Two observations for the prompt layer, both from that
record: the count says 15 while the creature count says 1540, so the tag is counting one ability
among several that feed the loop and the number the pilot sees badly understates what it has done;
and the tag states a fact without stating a consequence, which by the pool's own doctrine (a
consequence sentence beat a bare count on the land-drop ask, lane L) is the weaker of the two
shapes. If the tag is meant to be a stopping affordance it needs a consequence clause; if the
stopping is to be structural it belongs in the engine and the tag should not be relied on.

## G7 - `mana_only_windows_skipped` is doing its job and should be read per-game, not per-corpus

Corpus total 8,673, **all 8,673 at deck126's seat**, and **8,610 of them in a single game** - the
one where the opponent assembled an unbounded token loop and deck126 received priority thousands of
times with nothing but `Add N green mana with Overgrown Battlement` on the menu. The other five
games total **63** (wave 46: 180 across six games, on a comparable board). The counter is counting
the same event it counted last wave - it increments at the skip and nowhere else - and the gate is
unchanged (`AIPlayerGPT.cpp` ~11930-11946, rejecting on the first non-`isManaOnlyAction` option, so
a menu with any other action cannot reach it). **Zero mana-only windows leaked to the model this
corpus** (wave 46: 3). Suggestion: the gameend record would be more useful with the skip count
alongside the count of windows that DID reach the model, so a reader can see the ratio without
joining to the JSONL - and so a pathological game announces itself in one field.
