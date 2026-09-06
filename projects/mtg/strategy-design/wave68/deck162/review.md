# Wave 68 — deck 162 (Draw and Die, UB forced-draw punish) — per-deck review

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/`
Seat files (all in that dir): `1788720095-…deck162-…-vs-…deck146`, `…107-…vs-…deck152`,
`…111-…vs-…deck126`, `…113-…vs-…deck125`, `…117-…vs-…deck130`, `…122-…vs-…deck123`.
Record **5/6** (W vs 146, 126, 125, 130, 123; L vs 152) — up from 4/6 last corpus with a guide cut
from 71,490 to 19,993 bytes. 113 decisions at this seat, 2 fallbacks
(`blocks_reversed_in_prose_reask` 1, `plan_choice_conflict` 1 — both re-asks upheld the pilot's own
first answer), 1 `reply_truncated` (deck130 seq 2, mulligan, +1795 B, answer kept and correct), 0
`wall_miss_events`, 0 `wall_miss_no_retry`, max `deadline_pct` 35.0, `pre_answer_overrun` 0 in all
113 records. **No decision at this seat was lost to a cap or a wall.**
Window counts moved (113 decisions in 6 games vs 144 last corpus); per-window rates are not
comparable across the two.

## 1. Game by game

**W vs 146 (turn 11, 18–0).** Mine T4 (seq 5), Master T6 (seq 7), Shield Sphere + Fate Unraveler
T8 (seq 11, 12 — Sphere free first, then the punisher: the ordered list run correctly), Dictate
flashed in upkeep T10 (seq 15, `{reserve:}` absent, mana sufficient — a correct take), Mine #2 at
K of 1 (seq 17). Won on draw-step damage. seq 8 tripped
`blocks_reversed_in_prose_reask` (a `BLOCKS: B1:A1` line with a PLAN that talked about casting
Howling Mine "on turn one"); the re-ask at seq 9 got the same block.

**W vs 126 (turn 12, 20 to −3).** Sphere T2, Master T6, Dictate T8, then Underworld Dreams over Ob
Nixilis and Mine (seq 12 — punisher first, correct), Mine after (seq 13), Sphere then Ob Nixilis
(seq 16, 17), and the −2 aimed at an opponent creature over three of its own (seq 18, correct: the
guide's own-permanent pass rule held). Their life 15 → 6 → 2 in two draw steps.

**W vs 125 (turn 29, 14–0).** Slow control matchup, 25 asks, no combat asks at all. Punishers landed
seq 8/10/12/25, Caress seq 16, Fog Bank seq 23. seq 19 `plan_choice_conflict`: the pilot answered
`CHOICE: 5 (Swamp)` on the opponent's Path-to-Exile "choose a card to put onto the battlefield" ask
and then wrote 2,276 characters of deliberation ending "I will just play the land and pass" — the
reversal predicate caught "I will pass". The re-ask (seq 20) returned the identical choice.

**W vs 130 (turn 13, 14 to −3).** Underworld Dreams T5 (seq 6) over Dictate and Master — punisher
first with all three affordable. Master T7, Caress T9, Fog Bank T11 (seq 11, the one Fog-Bank take
with creatures opposite), Sphere T13.

**W vs 123 (turn 15, 19–0).** Mine T3, Underworld Dreams T7 and #2 at T11/T13, Fate Unraveler T9,
Dictate T11, Master T13. Clean rung-1 discipline throughout.

**L vs 152 (turn 15, −8 to 12) — the whole loss is five cast windows.** deck152 is white aggro
(Luminarch Aspirant, Katilda, Sigarda, Tovolar's Huntmaster) and put a lethal crack-back on screen
from turn 10 onward.
- **seq 13 (T10, upkeep, 4 sources).** The flashed Dictate row printed
  `{reserve: this row is INSTANT SPEED … Taking it HERE, before your main phase, leaves 1 source, and
  Fate Unraveler {3}{b} in your hand needs 4 - it is SORCERY SPEED, so your main phase this turn is
  its last window}`. Answer: **`CHOICE: 1 (Cast Dictate of Kruphix)`**, with the plan
  *"In Main Phase 1, play a land, cast Fate Unraveler"* — the exact sentence the guide's
  `{reserve:}` section names as one the row printed impossible. Two windows later (seq 15) the hand
  line reads `Fate Unraveler {3}{b} … [cannot pay now: needs 4 mana, you have 2 untapped sources]`.
  Fate Unraveler #1 died in that turn's combat and the second copy never landed; K stayed at 0 for
  the rest of the game.
- **seq 15 (T10, MP1, life 12).** `CRACK-BACK NEXT TURN: 3 of their creatures … for up to 13 …
  you would be at -1 or lower; that would KILL you`. The menu's only spell row is `1. Cast Fog Bank
  {1}{u}`. Answer: **`CHOICE: 2 (Cast nothing right now)`**.
- **seq 17 (T10, MP2, same hand, `[you declined this exact list 1 time already this turn]` printed).**
  Answer: `Cast nothing right now`, with the plan sentence *"do not cast Fog Bank (unnecessary
  risk/cost)"* — a sentence the guide's `"Cast nothing right now" answers a menu of GIFTS, never one
  cheap permanent` clause already refutes.
- **seq 19 (T12, life 10, crack-back 15, "that would KILL you").** Menu: Fog Bank / Teferi's Puzzle
  Box / Liliana's Caress. The Puzzle Box row printed `draw converters … on your battlefield: 0` and
  `draw converters in your hand: 0`. Answer: **`CHOICE: 2 (Cast Teferi's Puzzle Box)`** — a K-of-0
  feeder, forbidden by the brake, with a wall row and a Caress row both on screen and rung 0 already
  firing (M=4 > N=1, life 10). Turn 13 the opponent bottomed 9 and drew 11, deploying Tovolar's
  Huntmaster + Ranger Class + a second Aspirant; turn 15 they attacked with 8 for 29.
- **seq 21 (T14, draw step, 6 sources).** The same clause again, this time naming the deck's best
  card: `… leaves 3 sources, and Ob Nixilis, the Hate-Twisted {3}{b}{b} in your hand needs 5 …`, on a
  row also printing `draw converters … on your battlefield: 0`. Answer: **`CHOICE: 1 (Cast Dictate of
  Kruphix)`**, plan *"Cast Ob Nixilis in Main Phase 1 to remove a key threat"*. At seq 23/24, the
  main phase it named, the menu is Caress / Mine #2 / Master / Shield Sphere — **no Ob Nixilis row**.
All five are **STRATEGY**, not perception: every number the rules read was printed on the row being
answered. seq 23/24 (T14) then ran rung 0 correctly (Shield Sphere, then Master, whole window spent)
— too late.

## 2. Engine / interface / card items

**MED — the `{crack-back cover:}` clause projects a life total that counts only the row's own new
body, and there is no counterpart that counts bodies already on the battlefield.**
Repro: `1788720107-ai_baka_deck162-0x560f5bc31670-vs-ai_baka_deck152.jsonl`, seq 15, row 1:
`… This adds 1 body … CHECKED: 1 of them is a body whose block legality against these attackers this
row computed. Counting only the checked bodies you cover 5 of 13, leaving 8 -> you would be at 4.`
At that window the battlefield line reads `of which 2 are creatures, 2 of them able to attack right
now` — Master of the Feast (5/5) and Fate Unraveler (3/4) were both untapped and could block, and
neither enters the arithmetic. The same clause at seq 19 prints `you cover 5 of 15, leaving 10 -> you
would be at 0` with Master untapped; **0 is a loss**, so the only survival arithmetic on screen at a
defensive-cast window reads as "this cast does not save you". The clause discloses its scope
("counting only the checked bodies", "THIS IS NOT A SURVIVAL VERDICT"), so this is understatement,
not a lie — but it is the number the pilot acted on, twice, in the corpus's only loss for this deck.
A second figure counting the untapped bodies already in play would make the row's real price visible.

**MED — the reply-reversal predicate scans the whole PLAN, including deliberation the pilot is
writing to itself.** Repro: `1788720113-…deck162-…-vs-…deck125.jsonl`, seq 19: `CHOICE: 5 (Swamp)`
followed by a 2,276-character plan that reasons through options and ends "I will just play the land
and pass"; the engine emitted `[RE-ASK] Your CHOICE line takes row 5 … but your reply says this
window is a pass ("I will pass")`. The re-ask cost a full round trip and returned the same row. The
same seat's plans are being truncated at carry time (`[…the rest of your plan was not carried: 1940
further characters, of 2276 you wrote]`), so the prompt already knows the plan is over-long: saying
the carry budget in the ask, or scoping the reversal test to forward-intent clauses, would remove
this class.

**MED — the `{reserve:}` clause states its two numbers but leaves the comparison to the reader.**
Repro: `1788720107-…deck162-…-vs-…deck152.jsonl`, seq 21, row 1: `{reserve: this row is INSTANT SPEED
- it still has a window at the end of THEIR turn. Taking it HERE, before your main phase, leaves 3
sources, and Ob Nixilis, the Hate-Twisted {3}{b}{b} in your hand needs 5 - it is SORCERY SPEED, so
your main phase this turn is its last window}`. Everything needed is printed, but 3 < 5 is arithmetic
the pilot has to run, and both times it rendered against a punisher (seq 13 and seq 21) the pilot
took the row and then wrote a plan naming the now-uncastable punisher. Every other priced seam in
this prompt family ends in a verdict word; this one ends in two operands. A closing clause that says
the row REMOVES that card's last window this turn would put the answer on the screen. (Filed as a
render item and not bought in the guide: the guide already states the rule, the subtraction and the
banned plan sentence, and a fourth restatement is not the remedy.)

**LOW — no owner line, guide or render, for the opponent-effect library-search ask.** Same seq 19/20:
`CHOOSE A CARD TO PUT ONTO THE BATTLEFIELD: Path to Exile's effect …` is a decision kind deck162's
guide has no rule for. The render carries enough to answer it ("with basic lands, the colour your
hand and your board are shortest of") and the answer was right, so this is noted, not bought — a
guide sentence here would be bytes spent on a once-in-six-games ask.

**Card facts: all verified against the primitives this wave, all correct.** `Forced Fruition`
(`auto=@movedTo(*[-land]|opponentstack):draw:7 opponent`, seven cards ✓), `Ob Nixilis, the
Hate-Twisted` (`{C(0/0,-2,Loyalty)}: … destroy target(creature) && draw:2 targetcontroller`, 5
starting loyalty ✓, and `target(creature)` is unrestricted — the guide's "offered only your own
creature, pass" rule is the right shape), `Peer into the Abyss` (`choice name(Target opponent)` /
`choice name(Target controller)` ✓), `Shield Sphere` 0/6 defender with `counter(0/-1)` on block ✓,
`Fog Bank` 0/2 flying defender, prevents damage both ways ✓, `Master of the Feast` 5/5 flier,
`@each my upkeep:draw:1 opponent` ✓, `Teferi's Puzzle Box`, `Liliana's Caress`, `Fate Unraveler`,
`Underworld Dreams`, `Dictate of Kruphix`, `Howling Mine` all ✓. No engine/Oracle disagreement found.

## 3. Guide verdict: EDIT (two changes, both repairs; the guide SHRANK)

Byte count **19,971** (`stat -c %s`), down from the live guide's 19,993; A332 grep (with the
`|you (declined|took|cast|passed) [0-9]` extension) prints **0**; the frequency-adjective grep prints
2, both pre-existing and both on exercised mechanics ("usually Master of the Feast" as the last body
at low life; "most often a two-lander" in the mulligan odds prose). No rule was added and no rule was
dropped — one threshold was re-keyed and one dead literal repaired, so nothing had to be bought.

**Edit 1 — rung 0's gate stops being a life constant and becomes the printed relation.**
before: `If M > N and L is 10 or less:` (with `L = your printed life;` defined ahead of it)
after: `If M > N and the CRACK-BACK NEXT TURN line reads "that would KILL you":`
Paid by **seq 15 (vs deck152, turn 10)**: life 12, so `L is 10 or less` was FALSE and rung 0 did not
fire, while the crack-back line on that same screen read `up to 13 … you would be at -1 or lower;
that would KILL you` and the menu's only spell was the wall. The danger there IS the relation between
two printed numbers, not a constant, and the render already prints that relation as a fixed literal.
This is one threshold replacing one threshold, not a second one added.
Coverage check: every window in this corpus where the old constant fired (seq 19, 23, 24, all vs
deck152) also prints `that would KILL you`, so the new gate is a strict superset — it adds seq 15
and 17 and loses nothing. Literal audit: `that would KILL you` renders in 9 windows, all in the vs-152
seat, 3 of them cast asks. The situations list was re-keyed the same edit (situation 1 now opens
`A crack-back line reading "that would KILL you", no creatures of yours, …`) so the branch it
illustrates still matches the rule.
Prediction: at any cast window where `that would KILL you` prints and their unrestricted-attacker
count exceeds your creature count, the answer is a creature row. A hit is a break only if a creature
row was on the menu and a noncreature row (or a decline) was taken.
Removal condition (information-supplying): one corpus in which the crack-back cover carries the
"bodies already in play" figure asked for in section 2, at which point the pilot can price the block
directly and this gate can go back to being one comparison.
Bytes: the rung's worked micro-example ("two bodies survives, one body plus a five-mana walker does
not") was cut to pay for the longer literal — a worked example on a rule that held (seq 23/24 spent
the whole window with two bodies and took no walker row).

**Edit 2 — the hold-priority row's quoted literal is repaired to the one the engine prints.**
before: `"Hold priority for the rest of this turn: pass now, and do not ask me again unless the board
changes ..."`
after: `"Hold priority: pass now, and do not ask me again - this turn or later - until one of the rows
above changes ..."`
Literal audit: the old string renders **0 times in 113 prompts**; the printed row renders in 45 cast
menus, and its companion trigger `[you declined this exact list` printed at **seq 17 (vs deck152)**,
the one window where the guide's hold rule was live. A quoted literal that does not print is audit
debt whether or not it changed an answer; this one's window arose, so it is a defect, not an UNTESTED.
Prediction: `grep -F "Hold priority for the rest of this turn"` over next corpus's prompts stays 0
and the guide no longer contains it. Removal condition: none — this is a repair.

**What was NOT edited, and why (KEEP).** The three rules the loss actually broke — rung 4 (`FOG BANK
{1}{u}, when they control any creature`), rung 6 (the K-of-0 brake, which names PUZZLE BOX by name and
keys on `draw converters … on your battlefield:`), and the `"Cast nothing right now" answers a menu of
GIFTS, never one cheap permanent` clause — are all present, correctly keyed to strings that printed on
the row being answered, and at the right rank. Each was broken with its fact on screen; a fourth
paraphrase of a rule that already says the right thing is exactly what the skill forbids. The failure
is the pilot not running the ordered list top-to-bottom, and the honest remedy for that is a render
that prices the wall (section 2, MED), not more guide prose.
Counts for the record: **Fog Bank offered 12 times, taken 2 (17%)**. Of the 10 declines, 4 were legal
under the list's own order (0 opponent creatures at deck146 seq 5; a higher rung taken at deck146
seq 11 Shield Sphere, deck146 seq 12 and deck152 seq 10 Fate Unraveler). The other **6 are genuine
breaks of rung 4** — deck146 seq 7, deck152 seq 5, 7, 15, 17, 19 — and 5 of the 6 are in the one loss.
The `{reserve:}` rule: the clause rendered **3 times**; once naming a non-punisher (deck146 seq 15,
Fog Bank — correctly taken) and **twice naming a punisher (deck152 seq 13, seq 21), both taken,
0 of 2**, and `reserve_decline_windows_skipped` is 0 in every `gameend` record. Both breaks carried
the plan sentence the guide's own clause names ("in Main Phase 1, cast <that punisher>"), and both
times the named punisher had no row in the main phase that followed.
Shield Sphere: **offered 5, taken 5 (100%)**. Land drop: **41 windows, `Play no land right now` taken
0 times**. Punisher-first: every window offering a punisher took one (deck126 seq 12, deck130 seq 6,
deck123 seq 8, deck125 seq 8) — 4 of 4.

**UNTESTED this corpus, rules stay** (branch reachable, window never arose): forced sacrifice (`you
SACRIFICE this` 0 renders), cleanup discard / `PUT:` (0), Peer into the Abyss and its target question
(0 casts), `[legendary:` (0 — Ob Nixilis was cast once and no second copy came up), `{reserve:}`
the last-ditch
SYMMETRIC release (`and so do YOU: 1 extra card per turn` printed 23 times, all with a release or a
brake already deciding), Sanguine Bond / Exquisite Blood (`LOOP SCOPE` 0), lethal-block arithmetic
(one LETHAL header, deck152 seq 25, where the render itself printed `no block saves you`), and the
mulligan ship branches (6 keeps, no ship-shaped hand).

## 4. The short-guide question the brief asked

**No dropped rule would have changed a decision at this seat.** Every rule the six games needed was
still in the 19,993-byte guide, and all five misplays above broke a rule that survived the rewrite —
rung 0, rung 4, rung 6 and the `{reserve:}` section each kept the sentence that answered its window.
What the rewrite dropped around the decisive rules was repetition, not content: the
Fog-Bank instruction went from roughly ten places to two (rung 4 and the WALLS section) and the
K-brake from about six statements to one. The pilot broke Fog Bank at 6 of 8 windows where rung 4 was
the first match; I cannot separate "repetition was load-bearing" from "this matchup" on one loss, and
I will not spend bytes restoring repetition on that basis — but it is the one place a second corpus
would be informative, and the falsifier is cheap: if Fog Bank's take rate under an opponent-creature
line stays at or under 25% next corpus with rung 4 unchanged, the miss is structural and belongs to
the render item in section 2, not to guide length.
Seat-level numbers for the engine seat: system prompt 49,560 B, median rendered prompt 8,139 B (max
20,067), 113 decisions, median latency 24.4 s, p90 62.7 s, max 315.2 s.

## 5. What I did not check

Deck 152's own seat file was read only for board context around the vs-152 windows, not decision by
decision. I did not compare per-decision prompt bytes or latency against the wave-67 corpus directly
(only against the totals quoted in the brief) — that comparison belongs to the engine seat. I did not
build the deck list from `Res/ai/baka/deck162.txt`, so the "second copy" structural pass is inherited
from the rewrite rather than re-derived. No general-guide or skill proposals: nothing here is
deck-independent enough to be one, and both would restate existing amendments.
