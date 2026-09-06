# Wave-67 per-deck review — deck 126 (Sanguine Blood, WGB walls/combo)

Record this wave **1/6** (3/6 last wave, KEPT guide). Corpus
`/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-102638/`, binary master 14977f3af.
Seat files `*deck126-*-vs-*` (6), opponent seats read for context.
Fallbacks in deck126's six seats: **0**. `unparsed_reply` 0. `engine_reveal_floor_picks` 0 in all
six gameend records; no `reveal_fallback_pick`; `reveal_wait_unexplained_secs` 0 on all 8 reveal
records; no `deadline_pct > 100`. Lane AX's reveal fix reached every Idyllic Tutor window in this
deck and none needed the floor.

## 1. Game by game

| opp | result | turn | end life | deciding decision |
|---|---|---|---|---|
| 130 | **WIN** | 22 | 34 / 0 | combo assembled t21 (seq 26-28), opponent's t22 attack triggered Pride Guardian's block-gain and the Bond/Blood loop ran to 0. The engine's loop works. |
| 152 | loss | 11 | -1 / 34 | **seq 14** (see below) — cast Sanguine Bond into a printed lethal crack-back with a Tribute row live. |
| 123 | loss | 10 | -3 / 17 | unwinnable by seq 12: Intruder Alarm + Thraben Doomsayer had already made 27 Humans; 28 attackers / 29 damage against two 0-power walls. Perimeter Captain instead of the Bond still reads "-7, still KILLS you" on its own crack-back-cover bracket. Matchup, not guide. |
| 162 | loss | 16 | 0 / 19 | Underworld Dreams + Fate Unraveler + Ob Nixilis + Howling Mine = 6 life per turn off compulsory draws (log at seq 24: 20 -> 3 over turns 13-15). Deck 126 holds no answer to a noncreature permanent. seq 19 attacking the lone lifelink Vampire into an untapped Fate Unraveler was a small further loss; the game was already on a 3-turn clock. Structural. |
| 125 | loss | 31 | 0 / 28 | hard-countered control: Exquisite Blood countered by Fall of the Gavel (t22, log at seq 49), Idyllic Tutor by Dream Fracture twice, Pride Guardian by Cancel, Overgrown Battlement by Essence Scatter (t28 log). Sanguine Bond sat alone from t12 to t31 with no lifegain event; opponent never attacked, so no block trigger existed. Both Tributes were dead (no opposing creature). Matchup. |
| 146 | loss | 29 | 0 / 15 | four combo halves exiled one at a time — Vanishing Verse (seq 13 log), Kaya -3 (seq 48 log), Kaya -3 again, Vanishing Verse again — 20 mana spent, 5 at a time, on halves that never met. Model was at 30 life on t20 and lost to a goblin swarm with a 1-damage-per-turn clock. |

Variance/matchup accounts for 123, 162, 125 and most of 146. One loss (152) is guide-caused and is
cited below. Hands were real; no mulligan in the six games looked wrong (all six keeps had 2+
lands and a castable spell; seq 15 in g162 discarded the redundant **second** copy of Exquisite
Blood, which is correct).

## 2. Engine / interface / card items

**HIGH — `post_answer_overrun` is blind to the answer-LAST replies, which are the expensive ones.**
Repro: `1788708415-ai_baka_deck126-...-vs-ai_baka_deck152.jsonl` seq 10 (blockers, t7) carries
`"post_answer_overrun": 0, "post_plan_overrun": 0` while **4572 bytes** of prose precede the coded
`BLOCKS:` line; `latency_ms` 825167, `deadline_pct` **91.6**. Same shape at
`...vs-ai_baka_deck123.jsonl` seq 13: `post_answer_overrun 0`, 3741 bytes before `BLOCKS:`,
462413 ms. These are the two slowest decisions in the deck and the metric scores both as clean.
The corpus's 71% figure therefore measures only the cheap half of the protocol; the answer-first
share cannot be read off it. A `pre_answer_bytes` counter beside the existing one would price this.

**HIGH — the blockers seam is where the answer-first protocol is dropped, and it is where the
deadline is nearly missed.** Every one of the deck's ask/reveal/priority/discard/attackers replies
(60 of them) put the coded line first with 0 pre-answer bytes. The only two answer-last replies in
the deck are both `blockers`, and g152 seq 10 spent 825 s — 75 s short of the 900 s curl wall —
on a **one-attacker, two-blocker** combat. The reply's own text shows the mechanism: it quotes
Rule #5's carve-out chain back to itself five times ("Rule #5 Step 1 says…", "Rule #5 also
states…", "Wait, check Rule #5 again") before deriving the answer from EV instead. The prompt's
format sentence is identical to the ask seams', so the cost is the seam's rule surface, not the
render.

**MED — the Tribute to Hunger row is the only removal row this deck has and it never prices itself
against the printed CRACK-BACK total.** Repro: `...vs-ai_baka_deck152.jsonl` seq 14, t10. The
header reads `CRACK-BACK NEXT TURN: 4 of their creatures will be able to attack ... for up to 18 -
you would be at 0; that would KILL you`. Row 2 reads
`Cast Tribute to Hunger {2}{b} {right now: they control 4 creatures - they choose which one - you
gain at least 3 ...}` — no crack-back clause. A **creature** row does get one: `...vs-deck123`
seq 12 row 3 prints `{crack-back cover: the CRACK-BACK NEXT TURN line above is 29 ... you would be
at -7, which still KILLS you}`. The row that actually subtracts an attacker from that total is the
one row that does not say so. A `{crack-back cover:}` clause on the Tribute row (they sacrifice
their smallest, so the floor is computable) would have made seq 14 answerable off the row.

**MED — first decision of a game costs 55-174 s.** seq 2 (the mulligan) latencies across the six
seats: 163.0, 174.2, 55.2, 168.5, 134.3, 77.1 s, against a median ask of ~30 s. Six games x ~2 min
is ~12 min of the 101-minute run spent on the one decision whose prompt is smallest.

**LOW — no menu row carries "they have shown removal for this".** g146: the model cast a lone
combo half into open mana four times (seq 10/12 Blood, seq 33 and 46 Bond) after the log had
already named Vanishing Verse and Kaya. The log does attribute every exile correctly on the
victim's side (verified at seq 13 and seq 48), so this is not a narration defect — it is an
absent affordance. Noting, not proposing.

## 3. Guide verdict: **EDIT** (one change, 5 lines)

The guide is good and most of it is doing its job — Rule #7's ordering produced the win in g130,
Rule #8's hold row kept g125 and g146 answerable at 75 and 65 decisions, the Rule #7 name-copying
and re-ask sections produced **zero** fallbacks across six games. One clause killed one game.

**Edit — Rule #7 entry 7b, inserted before "IN THE FIRST WINDOW THAT PRINTS THE ROW".**

Before: 7b's survival floor fires only on the string
`{right now: they control 1 creature - <name> is sacrificed, you gain N}`. Everything else is
governed by entry 1, which reads "NOTHING BELOW ENTRY 1 IS AN ANSWER WHILE AN ENTRY-1 ROW IS
PRINTED — not a wall, not a Lantern, not a Tutor, not 'leaves N', **not a life total**". The
string `CRACK-BACK` appears **nowhere** in the 70,992-byte guide.

After (added):

    THE CRACK-BACK LINE IS THE SECOND DOOR INTO THIS FLOOR AND IT DOES NOT COUNT THEIR CREATURES.
    When "CRACK-BACK NEXT TURN:" ends "that would KILL you", a row that REMOVES an attacker or ADDS
    a blocker outranks entry 1 this window at any N. At 18 life with six sources and that line
    printed you took Sanguine Bond over "Cast Tribute to Hunger ... they control 4 creatures ... you
    gain at least 3" and were dead at -1 the next turn. The half you skip is still in your hand.

Paid for by `1788708415-ai_baka_deck126-0x5621f03365b0-vs-ai_baka_deck152.jsonl` **seq 14**, t10,
18 life, 6 untapped sources, one 0/4 wall, `CRACK-BACK ... up to 18 - you would be at 0; that would
KILL you`. Rows: 1 Idyllic Tutor (dead — "every enchantment left in your library is a copy of one
you already control or hold"), 2 Tribute to Hunger (4 creatures, floor gain 3), 3 Sanguine Bond,
4 Exquisite Blood. The model answered `CHOICE: 3 (Cast Sanguine Bond)` — **exactly what the guide
says**: both names missing, both rows printed, and Rule #7 entry 1's "BOTH NAMES MISSING ... TAKE
SANGUINE BOND". 7b did not fire because they held 4 creatures, not 1. Sanguine Bond alone stopped
nothing; the game ended at -1 on t11 (gameend seq 15). The Tribute line survives it: they sacrifice
their smallest (Katilda 3/3), 18 -> 21 life and 18 -> 15 incoming, the Wall of Omens blocks the
largest blockable 5, and 10 damage leaves 11 life with 3 mana still up. It is also an instant, so
it could have been held for their turn.

This is the only edit. It does not touch the "1 creature" door, does not add a life-total
judgement (the trigger is a printed string, as 7b's existing doors are), and does not disturb
entry 1 in any window where the crack-back line is absent or non-lethal — which is every other
casting window in these six games (2 other lethal crack-back windows exist, g123 seq 12 and g162
seq 21/22, and in neither was a Tribute or blocker row printed that would have changed the
outcome, so neither is regressed).

**Rule #5 was NOT edited** despite being the direct cause of the 825 s and 462 s replies. In both
cases the model reached a defensible block after the deliberation (g152 seq 10 blocked with Pride
Guardian only, correctly declining the gang-block; g123 seq 13 took the two-blocker maximum on a
lethal header). No decision went wrong, so no trim is paid for. The cost is reported as an engine
item above instead.

Guide written to `wave67/deck126/strategy.txt`, **71,483 bytes** (`stat -c %s`), inside the
41,000-71,500 band with 17 bytes of headroom. The next wave has essentially no room left in this
guide; any further addition needs a trim in the same pass.

## 4. Proposals

None. No general-strategy change and no strategy-writing-skill amendment is supported by this
deck's six games: the one guide defect found is specific to deck 126's single removal card, and
the two cross-cutting findings (the overrun metric's blindness, the blockers-seam latency) are
engine items, not writing-skill items.

## Not checked

- I did not verify any card's primitive against Scryfall; no card in these six games behaved
  contrary to its printed text on the rendered line (the Bond/Blood loop firing correctly in g130
  is the positive control).
- I did not read the opponent seats for 152, 123, 130 or 162 beyond the outcome; only the deck146
  and deck125 opponent seats were read, to attribute the exiles and the counterspells.
- I did not adjudicate the lane predictions (engine seat's job); the AX reveal counts above are
  reported for this deck only, not the corpus.
- I did not measure whether the answer-last shape at the blockers seam appears in other decks.
