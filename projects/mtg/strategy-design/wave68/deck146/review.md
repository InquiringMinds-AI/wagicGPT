# deck146 (Orzhov Dungeons) — wave-68 review

Seat files: `matchups-20260906-134120/1788720{083,087,089,095,099,116}-ai_baka_deck146-*-vs-*.jsonl`
(435 records, 6 games, 3-3). Opponent seat files read for context. Fallbacks on this seat: 6
`plan_choice_conflict`, 1 `reply_truncated_reask`, 1 `plan_contradicts_noop_row_reask`; 0 wall
misses, 0 timeouts, 0 crashes, `wall_miss_events`/`wall_miss_no_retry` 0 in all six `gameend`s.

## 1. Game by game, and the decisions that decided them

| vs | result | turns | the decision |
|---|---|---|---|
|152|WIN 21 / -1|13|Mulligan of a 1-land 7 under the printed count reason (seq 2), then curve-out. No misplay found.|
|126|WIN 18 / -5|17|Verse on Exquisite Blood (seq 14-15) and Silencer naming Exquisite Blood (seq 10, 17) — the Sanguine/Exquisite rule fired exactly as written.|
|123|WIN 19 / 11|18|Soul Shatter on Bloodline Keeper (seq 24). One re-ask (seq 14) — see H3.|
|162|LOSS 0 / 18|11|**seq 14**: Kaya -3 exiled Shield Sphere (0/6 [defender]) with Master of the Feast (5/5 flier) on the same -3 list and the CRACK-BACK line reading "5 from 1 attacker nothing you control can legally block". The flier then killed the seat. **seq 22**: at 1 life, animated a Hive whose row read "[this land is TAPPED: animated, it still cannot attack this turn]" — 4 mana for a 3/3 that could neither attack nor block. **seq 10**: Nadaar sent into a tag reading "your attacker dies, their blocker lives" and died.|
|130|LOSS 0 / 17|22|**seq 21** at 8 life: Soul Shatter cast into "{right now: they sacrifice ONE of these 5, their choice - all tied at MV 0}" — it ate one 1/1 Goblin token. Two earlier Soul Shatters (seq 19, 20) correctly took Siege-Gang Commander; the deck simply out-produced the seat afterwards.|
|125|LOSS -1 / 80|74|Not decided by one call: two Supreme Verdicts, three counterspells and Elixir/Sphinx lifegain (opponent 20 -> 83). Two guide-relevant spots: seq 209 sent both 1/1 Adventurers into Lightmine Field (damage = number of attackers) and lost them for 2 extra damage; seq 152/166/174/188 kept a dead second Kaya through four cleanup discards (see H4).|

Hands were real: the only mulligan (152 seq 2) and the only bottom ask (152 seq 4) both matched
the guide's tests. Mulligan rule: 6 of 6 windows answered as written, 0 breaks.

## 2. Engine / interface / card items

**HIGH — the "becomes beholder" row's TAPPED tag is not the literal the guide (or the pilot)
matches.** File `...deck146-...-vs-deck162.jsonl` seq 22, row 4 renders:
`4. becomes beholder {rung ceiling: Hive of the Eye Tyrant's OWN mana is not counted toward this
activation - tapping it for mana leaves it tapped ...} with Hive of the Eye Tyrant #1 [cost: {3}{b}]
[this land is TAPPED: animated, it still cannot attack this turn]`
Offered twice on this seat with that bracket (deck162 seq 22, deck125 seq 243) and taken **2 of 2**,
both times spending the whole turn's mana on a creature that could not attack or block. The row
already warns, in two places, that the activation is worthless — an engine fix that simply does not
offer the row (or marks it `at 0 this does nothing`-style dead, the mark the pilot demonstrably
obeys 130+ times for Soul Shatter) would end the class. Guide-side re-key shipped below.

**MED — cleanup-discard rows carry `{you already control one: <name>}` and `{dead right now: ...}`,
and the pilot ignores both.** `...vs-deck125.jsonl` seq 174 prints
`6. Kaya the Inexorable {3}{b}{w} (planeswalker) {...} {you already control one: Kaya the Inexorable}`
next to `4./5. Vanishing Verse ... {dead right now: 0 legal targets on the board for it}`. Reply
sent a Verse. Same shape at seq 152, 166, 188: 4 of 4 cleanup discards kept the legend-dead Kaya.
Note the CAST menu's mark (`[legendary: you already control Kaya the Inexorable - legend rule ...]`,
seq 177/183) IS obeyed — the pilot even had a cast refused for it — so the discard mark's different
wording, not the fact, is what is being missed. Cheapest engine change: use the same
`[legendary: you already control ...]` string on the discard/bottom rows.

**MED — `- GANG BLOCK:` row tags and the collapsed multi-blocker tag never render.**
`/usr/bin/grep -c 'untapped creatures that could block this one' *.jsonl` = 0 and
`grep -- '- GANG BLOCK: '` = 0 across all 42 seat files; the only 12 hits are the prompt's own
boilerplate ("a `GANG BLOCK:` clause, when present, ..."). Either the tag is unreachable in the
current pool or it stopped being emitted. Five guide lines are keyed to it. Removal condition: if a
third corpus passes with 0 renders and no window where a two-blocker price mattered, cut the bullet.

**MED — `"(neither dies)"` is not a closed literal.** `...vs-deck126.jsonl` seq 22 renders
`Perimeter Captain (0/4) (neither dies (blocking trigger: they may gain 1))`. Any guide rule (or
pilot test) matching the closing paren fails on every priced tag. Re-keyed to the open form below;
worth the engine's attention because three guides quote the closed form.

**LOW — `[DRAW PRICE:` and `[repeat: activated this turn N times already]` printed 0 times on this
seat.** Both render elsewhere this corpus (`grep -l` finds them in the deck162/deck152/deck130
files), so both rules stay UNTESTED-AT-THIS-SEAT, not dead.

**LOW — bottom ask renders "You will be left with a 6-card hand"**, not the guide's quoted
"an N-card hand" (`...vs-deck152.jsonl` seq 4). Guide quotation trimmed to the invariant prefix.

## 3. Guide verdict: EDIT

The rewrite did **not** lose a rule the corpus needed: every break above happens under a rule the
short guide still carries (Kaya targeting, Hive CHECK 2, Soul Shatter's dead test, the
attacker stop, the cleanup scan). Checked each `(b)` drop in `wave68/rewrite/deck146/changes.md`
against the six games — the dropped material is narration, ledgers and re-statements of rules that
are still present; the one dropped item with decision content was the Kaya anecdote about exiling a
small permanent while a big flier sat on the same list, and its rule survived the cut. What failed
is the WORDING of surviving rules, and that is what this edit fixes.

`stat -c %s wave68/deck146/strategy.txt` = **19966** (was 19981; ceiling 20000).
A332 grep `corpus|wave[- ]?[0-9]|\bseq\b|previous game|last wave|this wave|census|paid (for )?by|[0-9]{3}v[0-9]{3}` = **0**;
`you (declined|took|cast|passed) [0-9]` = **0**. Net **-15 bytes**: every add was paid for by a cut.

### Edits, each with the decision that paid for it

**E1 — Kaya's -3 target.**
before: `THE TARGET IS THE BIGGEST BODY THAT WILL CONNECT: top of their power line, flier or trampler first, never a lord or anthem.`
after: `THE TARGET IS THE BODY HITTING YOU, NOT THE ONE IN YOUR WAY: take the name the CRACK-BACK line prices ("N from 1 attacker nothing you control can legally block" first), else the top of their power line, flier or trampler first. A permanent printing "[defender]" or a power of 0 never attacks you - exiling it buys one swing and leaves their clock whole, so never target it while an attacker of theirs is on the list; never a lord or anthem either.`
Paid by deck162 seq 14 (reply: "Remove Shield Sphere to clear the path for Triumphant Adventurer").
"WILL CONNECT" reads both ways; the pilot read it as *my* attacker connecting. The new condition is
two printed strings (`CRACK-BACK`, `[defender]`), both on that screen.
Prediction: window class = a Kaya -3 target menu with at least one opponent attacker AND one
opponent non-attacker on it. Refuting number: any take of a `[defender]`/0-power row while an
attacker row is offered. A hit is a break only if the CRACK-BACK line named an attacker on that
prompt. Removal condition (belief-correcting): both the render marking the -3 list with the
crack-back attacker AND zero breaks across two corpora with the triggering shape.

**E2 — Hive CHECK 2 literal.**
before: `or its entry reads "[tapped]" -> PASS.`
after: `or its ROW prints TAPPED in any form ("[this land is TAPPED: animated, it still cannot attack this turn]", "[tapped]") -> PASS. A tapped Hive becomes a 3/3 that neither attacks nor blocks, and the mana is gone.`
Paid by deck162 seq 22 and deck125 seq 243 (2 of 2 takes with the tag present).
Prediction: literal = `[this land is TAPPED`; refuting number: any `becomes beholder` take on a row
carrying it. Removal condition (information-supplying): one corpus in which the engine stops
offering the row, or marks it dead.

**E3 — Soul Shatter's dead test becomes a printed test.**
before: `Soul Shatter leaves this entry when its row names more than about three tied cards.`
after: `... when its row says "at 0 this does nothing" or "tied at MV 0" (those names are tokens: it eats a token and their real threats stay).` (spell note re-keyed to match)
Paid twice: deck130 seq 21 (cast into "ONE of these 5 ... all tied at MV 0" at 8 life) and deck123
seq 14, whose reply quotes the guide's own words — `The strategy guide says "Soul Shatter leaves
this entry when its row names more than about three tied cards."` — and then tries to cast into a
row marked `at 0 this does nothing`, drawing the `plan_contradicts_noop_row_reask`. "about three" is
not a number the prompt prints; MV 0 is.
Prediction: literal = `tied at MV 0`; refuting number: any Soul Shatter cast on a row carrying it.
Removal condition: none (it is a card fact).

**E4 — cleanup rungs (2)/(3) re-keyed to the printed clauses.**
before: `(2) A second copy of a card already on the list whose first copy you are keeping. (3) A second copy of a planeswalker already on the list or on your battlefield line.`
after: `(2) A row printing "{you already control one: <name>}" - a second Kaya or Lolth is dead in hand and goes before any live card, whatever its mana value. (3) A second copy of a card already on the list whose first copy you keep. Between two rows both printing "{dead right now: ...}", send the higher printed mana value.`
Paid by deck125 seq 152, 166, 174, 188 — 4 of 4. The old rung 3 asked the pilot to compare hand
against battlefield; the render already does it on the row. Worked situation rewritten to match.
Prediction: literal = `{you already control one:`; refuting number: any cleanup discard that sends a
different card while a row carries it. Removal condition: two corpora with the shape and 0 breaks.

**E5 — the attacker stop names the tag that was used to rationalise past it.**
after (added clause): `... a friendlier A-line and "[held back, it CANNOT block ...]" do not cancel it: that tag says only it cannot block THAT creature, and a body kept home still blocks the rest and swings later.`
Paid by deck162 seq 10 (Nadaar, the only body and the venture engine, sent into
`Master of the Feast (5/5) (your attacker dies, their blocker lives)` under a `[held back, it CANNOT
block ANY of their 1 creatures: Master of the Feast (flying)]` tag; it died and the seat never
rebuilt). One break in one window — an exclusion added to a rule that is otherwise correctly keyed,
not a re-key.

**E6 — literal repairs from the audit**: `"(neither dies)"` -> `"(neither dies"` in all three places
(the parenthesis gains a price clause, deck126 seq 22); collapsed-tag/GANG BLOCK bullet reworded to
describe the tag instead of quoting a string that renders 0 times corpus-wide; the dead sentence
quoting `most expensive to attack into: <name>` (0 renders in all 42 files) deleted; bottom-ask
quotation trimmed to the invariant prefix.

### Cuts that paid for the adds
Worked examples on rules that held with 0 breaks: the Hive-upkeep situation (CHECK 1 held at
deck162 seq 17 and deck125 seq 245/247/255-257), the "at 1 life answer tap" situation (the 3-life
rule held 2 of 2, deck130 seq 5 and 17), the 24-tied Soul Shatter clause (now inside the rule).
Prose compression, no rule lost, in: mulligan, bottom ask, 3-life menu, hold-priority row,
draw-price bracket, Silencer, Predation, Acererak, dungeon choice, counter/lifegain, CHECK 0,
CHECK B, converter, block ladder rung 3. Their rungs stand; only their bytes went.

## 4. What held (no edit)
Mulligan/bottom 6/6. Soul Shatter's `at 0 this does nothing` rows declined ~130/130. Verse fired on
its target list, never on "YOUR OWN". The Sanguine/Exquisite rule won game 126 outright. Land drop
taken every window it appeared. Lolth's -3-first and the 0's life floor never violated. The
`[legendary: you already control` cast mark obeyed. No engine-marked-dead row was ever cast.

## 5. Not checked
Opponent seat files were read only around the six deciding windows, not in full. Latency and
prompt-byte comparison against wave 67 is the engine seat's question, not answered here. The
`[DRAW PRICE:` and `[repeat: activated` branches never arose on this seat and are asserted reachable
only from other seats' files. I did not re-verify every card's primitive this wave — Vanishing
Verse, Soul Shatter, Kaya, Hive of the Eye Tyrant and Shield Sphere were checked against the
rendered card text in the prompts, not against `primitives/*.txt`.
