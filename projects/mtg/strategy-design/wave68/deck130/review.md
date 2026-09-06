# deck130 (mono-red land destruction) — wave-68 review

Seat files: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/*deck130-*-vs-*.jsonl`
(6 games, 351 records, 5 fallbacks — all `plan_choice_conflict`; 0 wall misses, 0 async drops,
0 transport errors, 0 hangs). Record 2/6.

## 1. Game by game

| Opp | Result | Turn | Final life | Deciding decisions |
|---|---|---|---|---|
| 146 | WIN | 22 | 17 / 0 | seq 23 Starstorm X=2 clears Silencer+Adventurer through a printed 3-life tax; seq 28/38 bodies land; seq 50/57/59 Goblins close. Guide followed throughout. |
| 123 | WIN | 24 | 20 / -5 | seq 11/15 land-kill on schedule; seq 21 face row legal (0 creatures, 2 lands, two resolved land-kills = the 6-life floor's release); seq 22/42/48 Hammer loop; seq 56/57 Rorix for the last point. Untouched at 20 life. |
| 152 | LOSS | 13 | -1 / 28 | Four lands from turn 8 to the end (real mana screw, not guide effect). seq 26 `plan_choice_conflict`: the CHOICE was Starstorm but the PLAN prose ended "So we pass." |
| 162 | LOSS | 13 | -3 / 14 | Master of the Feast (5/5 flier) with Underworld Dreams; deck has no answer at 5 mana. seq 25 broke the draw-punisher rule (N=1, life 4, not "above 5") for a Spellbomb draw — cosmetic, the 5/5 killed from 4 anyway. |
| 126 | LOSS | 20 | 0 / 36 | **The instructive loss.** seq 21/24/29/34/39 attacked into two 0/4 defenders every turn; seq 26/27/35/36/37/38 fed all four Goblins to the face at opponent life 25-19; seq 47 paid the Talisman's printed 1 life with both halves of the Sanguine Bond + Exquisite Blood loop on their battlefield — 19 life to 0 in three records (seq 48-50). |
| 125 | LOSS | 48 | -10 / 60 | Draw-go control (3x Cancel, 2x Essence Scatter, Dream Fracture, Fall of the Gavel, 3x Supreme Verdict). seq 61 Siege-Gang and seq 85 Rorix were each cast as the FIRST spell of the turn with 11-12 mana up and both were countered; from turn 24 the only line left was the Hammer loop at 3 a turn against +5 a turn of Sphinx's Revelation. |

## 2. Engine / interface / card items

**HIGH — none.** No false verdict, no missing fact, no parser refusal in 351 records. Every
decision below was made against a TRUE and complete surface; the failures are the guide's.
Notably the render did its job at the hardest moment: deck126 seq 39 printed
`LOOP HALF PENDING: ... every life payment you are pricing now becomes fatal rather than
expensive.` and seq 47's row printed `paying this costs you 1 life (Talisman of Impulse #2:
1 damage) - you would be at 18`. Both facts were on screen; the pilot took the row.

**MED — reply prose overruns the answer, and one seam eats decisions.**
243 of 351 records carry `post_answer_overrun` (104,050 B total; max 1,584 B at deck126 seq 38);
14 `reply_truncated`; 1 `pre_answer_overrun` (deck125 seq 12, 589 B). All 5 fallbacks are
`plan_choice_conflict`, and each is the same shape: a correct CHOICE line followed by paragraphs
of deliberation whose LAST sentence names a different action — deck152 seq 26 (`CHOICE: 1 (Cast
Starstorm)` … "We have 4 mana now. We cannot cast either. So we pass."), deck125 seq 110/113,
deck123 seq 27, deck152 seq 16. The engine caught all five. Repro: any of those seqs, field
`reply`. Suggestion for the engine seat, not this guide: the PLAN field is where the model does
its thinking out loud, and the reversal predicate is the only thing standing between that and a
wrong take.

**LOW — `[repeat: activated this turn N times already ...]` does not stop a repeat.**
deck126 seq 27/36/37/38 and deck162: 6 renders of the tag, 4 of them taken anyway (seq 38's tag
read "activated this turn 3 times already ... you have taken it 3 times in a row with no other
action in between" and the pilot took it a fourth time). No `repeat_past_stop` fired on this
seat. The tag is accurate; nothing acts on it.

## 3. Guide verdict: EDIT

**The rewrite lost nothing.** rewrite/deck130/changes.md (a) lists every rule as kept and (c)
records no rule that could not be restated. I checked the four rules that this seat's losses turn
on — the wall check, the counter play-around, the life-loop hold, and the Siege-Gang trade — and
all four are present in the 19,996-byte live guide. **No decision in these six games would have
gone differently under the 71 KB guide**: every break below is a break of a rule the short guide
still states. What the losses show is that four rules are keyed to something the pilot does not
read at the deciding window, so the edits re-key them; the guide neither grows nor gains a rule
it did not already have in substance.

Baseline 19,996 B -> **19,993 B** (`stat -c %s`). A332 grep = **0**
(`corpus|wave[- ]?[0-9]|\bseq\b|previous game|last wave|this wave|census|paid (for )?by|[0-9]{3}v[0-9]{3}|you (declined|took|cast|passed) [0-9]`);
frequency-adjective grep = **0**. Each addition below is funded by a named cut.

### E1 (HIGH) — WALL CHECK re-keyed, and the fact that a defender blocks

Before: `A tag reading "(blocking trigger: they may gain 2, and if they do their converter takes 2
off you)" means every attacker sent is 2 life off you and 2 on them, and they block every one …
Without the converter clause it is still "ATTACK: none" while Perimeter Captain or Pride Guardian
is on their line; kill those first.`

After: `A "[defender]" creature CANNOT ATTACK BUT BLOCKS NORMALLY, and one on their line does block
- it is named in the "their untapped blockers" tag of every attacker row it can take. While those
rows read "(blocking trigger: they may gain 2)", each creature you declare hands them up to 2 life
per blocker they use and your 1/1s and 2/2 kill nothing through a 0/4 … With a LIFE-TO-DAMAGE
CONVERTER line up as well, every one of those points comes off YOUR total too.`

Paid by: deck126 **seq 21, 24, 29, 34, 39** — 5 of 5 attack windows where
`(blocking trigger: they may gain 2)` printed, all attacked, 5 distinct (game, turn). Two reasons,
both fixed here. (1) A wrong BELIEF, printed in the reply at seq 21: *"Perimeter Captain and Wall
of Omens are defenders and cannot attack, so they cannot block."* The primitives say otherwise
(`mtg.txt:85398` Perimeter Captain `abilities=defender`, `auto=@combat(blocking) …:may life:2
controller`; `mtg.txt:131252` Wall of Omens `abilities=defender`), and so did the prompt: *"They
have 2 untapped creatures able to block"* and *"blocking can leave them as high as 19."*
A belief is corrected by one fact in the render's words, so the guide now states it once, as a
capability, where the attack is declared. (2) Literal audit: the composite string the old rule
quoted printed **0 times** in 351 records while its window arose 5 times; the printed form is
`(blocking trigger: they may gain 2)`, 5 renders, all deck126.
Cost of the breaks: opponent life 17 -> 19 -> 21 -> 25 across those turns, gained 2 per blocking
defender, and once Sanguine Bond landed each of those gains was also damage to deck130.
Prediction: on a board printing `(blocking trigger: they may gain 2)` with no
`LIFE-TO-DAMAGE CONVERTER`-free lethal of your own, `ATTACK: none` in at least 4 of the next 5
such windows; a hit is a break only if the attack row also printed a `- DIES` result for one of
the attackers. Removal condition (belief-correcting class): the render carries the corrective fact
AND zero breaks across two corpora with a `[defender]` blocker on their line.

### E2 (HIGH) — on a life-loop board, a printed life price is lethal, not expensive

Added to the existing loop paragraph: `ON THAT BOARD EVERY PRINTED LIFE PRICE IS LETHAL, NOT
EXPENSIVE: a row bracketed "paying this costs you N life" - the Talisman's own 1 among them -
closes the loop and kills you from ANY life total, so take a row priced at no life, or the hold
row. Read the same way the moment the frame prints "LOOP HALF PENDING".`

Paid by: deck126 **seq 47**. Board: `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs -
Sanguine Bond` plus Exquisite Blood, deck130 at 19 life. The chosen row read `Cast Molten Rain …
{paying this costs you 1 life (Talisman of Impulse #2: 1 damage) - you would be at 18}`. That
1 life was the ignition: seq 48 shows 18, seq 49 shows **2**, seq 50 shows 1, seq 51 is the
gameend at 0. The pilot's own PLAN at seq 47 says "we cannot attack or gain life" — it had the
loop right and did not price its own mana. The old text told it to hold everything and named
DEPLOYING as the wrong act; it said nothing about paying. The Talisman rule's threshold
("3 life or less") is false on that board and now carries the exception in its own sentence.
Prediction: with both halves printed, 0 takes of any row bracketed `paying this costs you`.
A hit is a break only if the row is simultaneously lethal for the opponent. Removal condition:
none — this is a card-interaction fact, not a route-around.

### E3 (MED) — the counter rule keyed to the fields the prompt actually prints

Before: `AGAINST A DECK THAT COUNTERS (… in their log) … never lead with Rorix or Siege-Gang while
they have two or more untapped lands and a card in hand.`
After: `THEY COUNTER when "Their graveyard" names Cancel, Essence Scatter, Dream Fracture or Fall
of the Gavel, "Their untapped sources:" reads 2 or more and "Opponent hand size:" is 1 or more …
RORIX AND SIEGE-GANG ARE NEVER THE FIRST SPELL OF A TURN there, whatever the cast order says, and
a turn with only one castable spell holds the body instead.` The stop is also restated inline on
cast-order entry 4, where the imperative to cast them lives.

Paid by: deck125 **seq 61** (turn 34, Siege-Gang first, countered by Essence Scatter) and **seq
85** (turn 40, Rorix first with 12 mana up, countered by Essence Scatter). At both windows the
three trigger fields were printed and unambiguous — seq 85: `Their graveyard (20 cards): Cancel
{1}{u}{u} x3; Essence Scatter {1}{u} x2; … Fall of the Gavel …; Dream Fracture …`,
`Their untapped sources: 6`, `Opponent hand size: 5` — and the log above the board listed six
previous counters by name. The old key was "in their log" and "untapped lands", neither of which
is a printed field name; the log is prose the pilot must scan, while the graveyard and the source
count are labelled lines. 2 of 2 body deployments in that game were led into open mana, and both
were the deck's only win conditions.
Prediction: with the three fields true, 0 windows where Rorix or Siege-Gang is the first spell of
a turn in which a cheaper castable spell was on the same menu. A hit is a break only if no other
row on the menu was castable. Removal condition: none while the pool contains a counter deck;
this is a matchup rule.

### E4 (MED) — the Siege-Gang sacrifice row is removal, not a clock

Before: `… is for killing a creature or finishing a player at 6 or less; a Goblin spent on 2 face
damage against a healthy opponent is the worst trade here.`
After: `… is a REMOVAL row, not a clock: take it only when the row reads "- DIES" against one of
their creatures, or when its "they would be at N" is 0 or less. At any other N the Goblin is worth
more as a body, and a row tagged "[repeat: activated this turn N times already]" means you have
already answered this question this turn - stop and move the turn on.`

Paid by: deck126 **seq 26, 27, 35, 36, 37, 38** — six activations at the face across two turns
with the row printing `they would be at 21 / 19 / 25 / 23 / 21 / 19`, four of them under the
repeat tag, spending all four Goblins for 8 damage against an opponent who gained 4 a turn and
finished at 36. Seat census: 9 sacrifice takes, 7 at the face (only deck162 seq 17 and deck123
seq 28 killed a creature), and the face takes bought a win in exactly 0 games. The rule existed
but sat in the card-facts paragraph as a judgement about "a healthy opponent" — an unrendered
input; it is now a comparison against the number the row itself prints.
Prediction: sacrifice takes at the face fall to 0 where `they would be at N` is above 0, over a
corpus with at least 5 such rows. A hit is a break only if the sacrifice was the last mana of a
turn with no creature on their battlefield. Removal condition: none.

### Cuts that funded the above (net -3 B)

- The never-printed converter composite literal in the wall check (0 renders / 5 windows).
- `Delete any plan sentence like "hold priority to avoid drawing" …` — a prohibition-only line
  whose banned sentence appeared in 0 of 20 draw-punisher windows.
- Two worked situations whose rules held at 100%: the Hammer-at-a-low-life-total situation (the face rule
  fired correctly at deck123 seq 47/51/55, opponent at 8, 5 and 4) and the empty-hand Hammer return (executed correctly in deck123 and
  deck125). Also the land-plan situation — every land-kill cast in the corpus was inside the two
  numbers.
- The GANG-BLOCK-that-kills paragraph compressed to its imperative (branch reachable but no
  blocking window arose; the rule stays, the anecdote goes).
- "usually turns 8-14" (unexercised frequency adjective) and the "turn 2 or 4 with one land"
  aside inside the 6-life floor.

## 4. Rules that held, and one that is drifting

- **Land plan**: 8 land-kill casts, all inside `their lands 4 or fewer AND life 9 or more`;
  nonbasic preferred where offered (deck162 seq 9, deck152 seq 24, deck126 seq 14, deck146 seq 44).
  KEEP unchanged.
- **Starstorm marked row**: 3 of 3 X menus carried `[<- most kills at any affordable X …]` and the
  pilot answered the marker each time (deck146 seq 25, deck152 seq 29, deck146 seq 15). The
  decide-at-the-cast-row rule held: 0 declines at an X menu after paying.
- **Mulligan**: 6 keeps, 6 correct by the guide's own bands; `would not cover any spell` printed 0
  times, so the ship branch is UNTESTED, not dead.
- **6-life floor — drifting, no edit.** 16 face-row takes; 10 were outside the floor (deck125 seq
  40/59/64/71/77/87/91/97/105 at opponent life 28-49, deck126 seq 44 at 21) and cost nothing,
  because on each of those boards the opponent had `0 are creatures` and the alternative was to
  bin the card. Violated-unpunished is its own verdict and never loosens a rule, so the floor
  stands this wave; if a third corpus repeats the pattern the release clause (`2 or fewer lands`)
  is the clause to re-derive — a control deck never has 2 lands late.
- **Draw punishers**: 20 renders, 8 `DRAW PRICE` rows, 1 break (deck162 seq 25, N=1 at 4 life).
  One break, correctly keyed, on a lost board: no restatement.

## 5. What I did NOT check

The opponents' seat files only for context around the deck126 loop and the deck125 counters — I
did not audit their decisions. I did not read `codex-review.md`, the lane files, or `known-bugs.md`
(the brief routes lane adjudication to the engine seat). I did not verify Scryfall against the
primitives for any card except Perimeter Captain and Wall of Omens; the rest of the guide's card
numbers were checked against `bin/Res/sets/primitives/mtg.txt` in earlier waves and were not
re-pulled this wave. Latency and prompt-byte comparisons against wave 67 belong to the engine
seat and I made none. No general-strategy or skill proposals: nothing here is a cross-deck rule
the skill does not already carry (A82 re-keying, A24 restating the stop where the imperative
lives, A174 stating a printed capability twice).
