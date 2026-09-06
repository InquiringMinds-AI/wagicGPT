# Wave 66 - deck 162 ("Draw and Die!", UB forced-draw punish) per-deck review

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-064038/`, seat files
`*deck162-*-vs-*.jsonl` (6 games). **Record 3-3** (wave 65: 4-2). Model `qwen36-35b-a3b`.
Seat totals: 100 model decisions, **8 `unparsed_reply` + 1 `wall_miss_unrecorded_wall` = 9
heuristic fallbacks (9.0%, vs 0 last wave)**, `post_answer_overrun > 0` on 55/100, median
latency 26 s, p90 213 s, max **900 s** (the corpus max is this seat), 129 min of wall clock.
Card facts checked against `bin/Res/sets/primitives/`: Underworld Dreams {b}{b}{b}, Fate
Unraveler {3}{b} **(3/4 Enchantment Creature)** and Ob Nixilis {3}{b}{b} (Legendary
Planeswalker) all carry `auto=@drawfoeof(player):damage:1 opponent`; Master of the Feast
{1}{b}{b} 5/5 flier is `auto=@each my upkeep:draw:1 opponent` - an engine with a body, no
punisher; Dictate of Kruphix has `abilities=flash`.

---

## 1. Game by game

| game | result | deciding decision |
|---|---|---|
| vs 152 | **L** t11, -4/24 | mana screw (2 sources t4-t10, no land drop offered). seq 6 discard misread. |
| vs 126 | **W** t21, 17/0 | 4 punishers landed; 4 fallbacks did not cost it. |
| vs 146 | **L** t13, -2/15 | **seq 18** - Master of the Feast over Fate Unraveler on a body turn. |
| vs 130 | **L** t14, -2/3 | **seq 19** - Dictate #2 over Underworld Dreams #2; exactly lethal was on the table. |
| vs 125 | **W** t14, 17/0 | clean curve, Peer into the Abyss at seq 19-20 for the kill. |
| vs 123 | **W** t14, 18/0 | **seq 13** - the wave-65 mana-reservation edit fired and won the rematch of last wave's -1089 game. |

### vs 130 - the game the seat threw (STRATEGY, surfaces all correct)
- **seq 16** (t11 DRAW): the model ran the wave-65 arithmetic verbatim - *"Adding UWD is much
  better (+3 damage/turn vs +1 damage/turn) ... casting Dictate now PREVENTS me from casting UWD
  this turn"* - and answered `CHOICE: 3 (Cast nothing right now)`. **The guide edit fired.**
- **seq 17**: the identical Dictate row came back in the same DRAW step (one count inside its
  `{feeds:}` clause had moved), with `[you declined this exact list 1 time already this turn]`
  printed and the same `{reserve: ... your main phase this turn is its last window}` clause. The
  model **cast the Dictate**, spending exactly the mana it had just reserved.
- **seq 18 -> seq 19** repeats the pair at **1 life**, opponent at 8, forecast `4 x 1 = 4`.
  Underworld Dreams #2 was in hand at {b}{b}{b} against 4 untapped sources: casting it makes the
  forecast `4 x 2 = 8` against their 8 - **lethal on their own draw step**. The model's own reply
  computed that line ("*they take 10 damage from UD/UD2 ... they go from 8 to -2*") and cast the
  Dictate anyway with a PLAN the row's `{reserve:}` clause had already falsified. Log: they drew
  5, went 8 -> 3, and Hammer of Bogardan killed the seat at -2 in the main phase that followed.
  The whole loss is the 3 life the second punisher would have collected.

### vs 146 - the other decided loss (STRATEGY)
- **seq 18** (t12, life 3, 5 untapped): menu was `1. Cast Master of the Feast {1}{b}{b} (5/5)
  {leaves 2 of your 5 ...}` and `2. Cast Fate Unraveler {3}{b} (3/4) {leaves 1 of your 5 ...}`.
  Master's own row read `draw converters ... on your battlefield: 0` and `draw converters in your
  hand: 1 - Fate Unraveler` - the engine naming the punisher one row below it. Both rows carried
  the same crack-back verdict (`you would be at -1, which still KILLS you`), so neither body
  bought survival. The model cited **Rule 0c** ("take every creature row") and took Master of the
  Feast. It reached the block window with **0 punishers and 0 draw damage collected all game**.
  Rule 0c's body order in the live guide reads "Shield Sphere, then Fog Bank, then Master of the
  Feast" and its DECIDING SITUATIONS bullet said "**not the punisher**" - Fate Unraveler, the one
  card that is both, is not in either list. The guide caused this. Fixed (edit 1).
- seq 12/13 (t8, 17 life, 4 mana, converters 0) declining Master of the Feast was **correct** -
  the Rule 2 brake, working.

### vs 123 - the wave-65 edit's payoff
seq 13 (t9 DRAW): the row printed `{reserve: this row is INSTANT SPEED ... Taking it HERE, before
your main phase, leaves 1 source, and Underworld Dreams {b}{b}{b} in your hand needs 3 - it is
SORCERY SPEED, so your main phase this turn is its last window}` (the MED-1 render item from wave
65, now live). The model ran the subtraction and answered `CHOICE: 3 (Cast nothing right now)` -
**and the parser refused the reply** (see HIGH-1). Baka passed, which preserved the intent; seq
15/16 then played the land and cast Underworld Dreams. Won 18/0 where the same matchup was
-1089 last wave.

### vs 152 - do not read this as guide effect
Two mana sources from turn 4 through turn 10, no land drop ever offered again; 5 model decisions
in the whole game. Variance. The one real decision, **seq 6** (cleanup discard, hand of 8 with
no land), is discussed in LOW-1.

---

## 2. Engine / interface items

### HIGH-1 - the CHOICE parser refuses `ANSWER: CHOICE: n (...)`, which the reply protocol's own wording invites; the BLOCKS parser accepts it
`kReplyProtocol` (H2a) now reads *"Write your REASONING first, then your **ANSWER**, then your
PLAN"*. The model duly labels the middle section `ANSWER:` and writes the coded line **on that
same line**. Every parse refusal in this seat is one of exactly two shapes:
- **5 of 8**: `ANSWER: CHOICE: n (...)` - label present, correct number, correct short name, not
  line-initial. Refused. Repro: `...deck162-...-vs-...deck123.jsonl` seq 13, reply line
  `ANSWER: CHOICE: 3 (Cast nothing right now)`; also vs123 seq 7, vs126 seq 12 and seq 22,
  vs130 seq 21.
- **3 of 8**: no coded line at all, prose only ("*I will discard copy 1 (number 5)*") -
  vs152 seq 6, vs126 seq 6, vs126 seq 8. These are the corpus's "33 replies carry NO coded line"
  shape; the other 5 are the "50 carry a label the parser still refused" shape.
**The seams disagree.** `...vs-...deck146.jsonl` **seq 19** contains `ANSWER: BLOCKS: B1:A1` and
was **parsed correctly** (`choice=1`, `chosen_text="Shield Sphere blocks Silverquill Silencer"`,
no fallback). So the blockers parser tolerates a prefix on the answer line and the CHOICE/PUT
parser does not. One-line fix at the CHOICE seam (strip a leading `ANSWER:`/`ANSWER -` before
matching, or match the label anywhere on the line), or drop the word "ANSWER" from the protocol's
ordering sentence and name the section by its label instead.
**What the Baka answers did** (brief item 7): vs123 seq 13 - Baka passed, = the model's reserved
intent, **game won**; vs130 seq 21 - Baka cast Liliana's Caress, = the model's choice; vs126 seq
22 - Baka used Ob Nixilis's -2 on Overgrown Battlement **#2** where the model named **#1**,
identical effect, **game won**; vs152 seq 6 - Baka discarded Ob Nixilis, = the model's stated
pick. Two diverged: vs123 seq 7 (model wanted Howling Mine, Baka cast Dictate of Kruphix) and
vs126 seq 12 (model wanted Master of the Feast, Baka cast Howling Mine) - both games won, neither
divergence decided anything. **No fallback changed a result in this seat**, but seq 13 is the
near miss: the parser threw away the single decision the wave-65 guide edit exists to produce.

### HIGH-2 - a reservation decline is re-asked inside the same step and the model flips
`...vs-...deck130.jsonl` seq 16 -> 17 and seq 18 -> 19 (full trace in section 1). The engine
correctly re-opens the window when a row moves, but on a mana-reservation decline **nothing that
re-opens the window can change the answer** - the reserve arithmetic is a function of untapped
sources, and none of the triggering changes (a count inside `{feeds:}`, one more card in the
forecast) touches them. Two cheap options, both in the existing annotation style: (a) treat a
decline of a row carrying a `{reserve:}` clause as covering the rest of the step while the
untapped-source count is unchanged (the same shape as the HOLD row's own promise), or (b) print
the prior answer on the re-ask, e.g. `{you declined this row at this seam earlier this step, and
your untapped-source count has not moved since}`. Today's `[you declined this exact list 1 time
already this turn]` is a count, not a reason, and the model read past it twice in one game.

### MED-1 - a PLAN that the row's own `{reserve:}` clause has just falsified is not re-asked
`...vs-...deck130.jsonl` seq 19: the row printed `{reserve: ... leaves 1 source, and Underworld
Dreams {b}{b}{b} in your hand needs 3 ...}` and the accepted reply's plan was "*Cast Dictate of
Kruphix now. Then cast Underworld Dreams in Main Phase.*" The engine already computes this exact
contradiction to render the clause. Lane AR ships `plan_contradicts_noop_row` with a one-shot
re-ask; the same one-shot re-ask keyed on `{reserve:}` would have caught the decision that lost
the game. (Also visible at seq 17.)

### MED-2 - a 900 s transport timeout burns 12% of the seat's wall clock and delivers nothing
`...vs-...deck123.jsonl` seq 9: `kind=wall_miss`, `latency_ms=900022`, `deadline_pct=100.0`,
`transport=curl=28,http=0,empty=1,connect_ms=20000,phase=wall`, `fallback=
wall_miss_unrecorded_wall`, reply empty. curl 28 is a timeout and `connect_ms=20000` says the
connect alone took 20 s. 15 minutes of a 129-minute seat spent on one decision that produced no
bytes. A wall deadline that already knows the transport failed (empty=1) should not run to the
full 900 s; this is 1 of the corpus's 2 `wall_miss_unrecorded_wall` records.

### LOW-1 - "most expensive first" mis-cues the cleanup-discard list into a mana-cost sort
`...vs-...deck152.jsonl` seq 6 (hand of 8, no land): the guide's own list is a **priority order**
(Forced Fruition, Peer into the Abyss, Teferi's Puzzle Box, a second copy of a draw engine)
introduced by the phrase "most expensive first". The model resolved the two against each other in
favour of the label - *"The discard priority is 'most expensive spell first'. Ob Nixilis is 5
mana ... Teferi's Puzzle Box is 4 mana"* - and sent a punisher while Puzzle Box, the list's own
third name, stayed in hand. **I am not editing for this**: Ob Nixilis is a Legendary Planeswalker
and the hand held two, so the *choice* was defensible even though the *reasoning* was not, and
the reply was unparsed anyway (Baka made the same discard). Flagging the wording for a future
wave that gets a paid window. The prompt itself carried no disposability ordering and no
`DEFAULT ANSWER:` line (lane AT H5's discard work has no land row to hang on here) - the rows
were printed in raw hand order.

### LOW-2 - PLAN scratchpad spend continues
55 of 100 decisions carry `post_answer_overrun > 0`; vs146 seq 18 lost 1062 bytes past the PLAN
line mid-sentence. Unchanged from wave 65's LOW-1; an engine-seat measurement, not a defect.

### Not checked
The `system` prompt / decklist render; a prefix-matched HOLD render/take census (I noted 1 hold
take, vs123 seq 23, and `hold_windows_skipped=0` in every gameend); `async_drops`; lane
prediction adjudication (engine seat's); the opponents' seats beyond deck 130's and deck 126's
finishing sequences; mulligan/bottom seams beyond noting they fired (vs146 s2-s5, vs130 s2-s4).

---

## 3. Guide verdict: **EDIT**

Live guide = `wave65/deck162/strategy.txt` = `bin/Res/ai/baka/deck162_strategy.txt`, 65,630 B.
Revised guide at `wave66/deck162/strategy.txt`, **69,897 B (68.3 KB)** - inside the 41-71 KB band.

**Both wave-65 edits are judged on their windows first:**
- **Edit 1 (mana reservation) FIRED and is KEPT verbatim.** vs123 seq 13 and vs130 seq 16 / seq 18
  are three declines produced by the new subtraction, in the model's own words, and vs123's is the
  reason that matchup went from -1089 to 18/0. The rule is right; only its *durability across a
  re-ask* was missing, which is what edit 2 below adds.
- **Edit 2 (forced sacrifice) is UNTESTED.** Zero forced-sacrifice asks reached this seat in six
  games (`you SACRIFICE this` appears in 0 prompts; deck 126's Tribute to Hunger resolved at
  vs126 seq 22-23 *while the seat's reply was unparsed*, so the engine, not the model, answered).
  Kept unchanged - no evidence for or against.

**Edit 1 - RULE 0c body order, and its DECIDING SITUATIONS bullet.**
before: "Take every creature row ... cheapest per body first ... Shield Sphere {0} first because
it is free, then Fog Bank, then Master of the Feast", and the bullet "Every creature row you can
afford in this window ... and nothing else - **not the punisher**, not the walker."
after: Fate Unraveler is named in the body order between Fog Bank and Master of the Feast; a new
paragraph "FATE UNRAVELER IS A BODY. IT IS THE ONLY CARD THAT IS BOTH" states that when a body
turn offers more than one paid creature row the punisher body goes first, and that Master of the
Feast at `converters ... on your battlefield: 0` is a card handed to them every upkeep; the
bullet's exclusion is narrowed to "not Underworld Dreams, not the walker" and names Fate
Unraveler as a creature row.
paid for by: **vs146 seq 18** (and seq 17, where the same two rows stood).

**Edit 2 - RULE 2, new "THE RESERVATION MUST SURVIVE THE RE-ASK" block** after the subtraction.
before: the subtraction told the model to answer "Cast nothing right now"; nothing said what to do
when the same row returns.
after: nothing that re-opens the window gives you mana, so a repeat of the same engine row before
your main phase gets the same answer; the two places the prompt says it is a repeat (`[you
declined this exact list N times already this turn]` and the unchanged `{reserve:}` clause); and
"a PLAN that says 'cast Dictate now, then cast Underworld Dreams in Main Phase' IS THE ROW TELLING
YOU IT IS FALSE". A one-line trigger form was added to DECIDING SITUATIONS.
paid for by: **vs130 seq 17 and seq 19** (the second pair at 1 life, where exactly-lethal was on
the table and the seat lost by 3).

Everything else is KEPT verbatim: Rule 1, Rule 2's K brake and its two release conditions (the
brake produced the correct decline at vs146 seq 12/13), Rule 3/4, the Ob Nixilis -2 teach (used
at vs146 s16, vs126 s22 - both legitimate), the Sanguine Bond/Exquisite Blood bullet, the
mulligan/bottom section, and the FORCED SACRIFICE section (untested, see above).

No general-guide or skill proposals from this seat: HIGH-1, HIGH-2, MED-1 and MED-2 are engine /
parser / transport items for the core loop, not prompt-text items, and the LOW-1 wording problem
is a deck-guide phrase with no window that paid for changing it.
