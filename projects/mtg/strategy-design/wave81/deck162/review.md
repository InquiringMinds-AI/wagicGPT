# Wave 81 - deck162 (Draw and Die!) per-deck review

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260917-191141-final`, binary `wagic-6e0c51238-w81step1`.
Seat: 6 games, **5 wins / 1 loss** (wave 80: 3/6). 137 model windows (ask 119, blockers 7, attackers 5,
discard 1, bottom 1, priority 4), **0 heuristic fallbacks**, 0 replayed-from-cache, native reasoning on
all 137 (median 7,152 chars, max 20,973), latency p50 120.7 s / max 461.8 s, 0 timeouts.
Guide under review: `projects/mtg/bin/Res/ai/baka/deck162_strategy.txt` (19,971 B).
File basenames below are abbreviated to the seat's opponent, e.g. `162v152` =
`1789690306-ai_baka_deck162-0x565093601250-vs-ai_baka_deck152.jsonl`.

---

## 1. Game by game

### 162 v 152 - WIN (opp 0, you 4, turn 10) - `162v152`
Curve was clean: Shield Sphere T2 (seq4), Howling Mine T4 (seq6), Underworld Dreams T6 (seq8),
Teferi's Puzzle Box T8 (seq10) - engines only after the punisher landed, exactly the guide's ordering.
The Puzzle Box + Mine forecast then read "their next draw step draws 8 cards ... they would be at 1".

**The deciding decision, and the seat's worst play of the corpus: seq11, T9, Blockers, life 15.**
Attack was Sigarda (5, flying, unblockable by this board) + Intrepid Adversary (6/4, lifelink).
The only blocker row was `B1. Shield Sphere (0/6) [defender] [deals 0 - ... it STOPS all 6 damage from
reaching you THIS COMBAT] - may block A2 (your blocker dies, attacker lives) {their attacker's lifelink,
this block: they gain 6 from this block only, and this attacker deals nothing to your life}`.
The prompt's own INCOMING line said `11 unblocked damage - you would be at 4 ... best case with every
blocker assigned: you would be at 10`. The seat answered `BLOCKS: none`. Its reasoning (verbatim):
"Wait, let's double check the lifelink math. - Intrepid Adversary deals 6 damage. - I gain 6 life. - So
net change is 0." and "This confirms blocking deals 6 damage to me and I gain 6 life. Net 0." Both
inversions of a clause that says the opposite. Guide BLOCKING rung 0 (15 - 11 = 4, which is 8 or less ->
block the biggest you may block, take the line even when it kills your blocker) commanded the block.
Cost: 6 life, taking the seat to 4 with a 13-damage crack-back forecast; it survived only because the
opponent's draw step killed them first (seq14, second Underworld Dreams, opp 0 on their T10 draw).
Classified PERCEPTION at the render (HIGH-1) *and* STRATEGY at the seat: see section 4.

### 162 v 146 - LOSS (you -7, opp 9, turn 16) - `162v146`
The one loss, and it was not decided by a misplay. deck146 answered every permanent the seat resolved:
Underworld Dreams exiled by Vanishing Verse the turn after it landed, Fate Unraveler killed by Soul
Shatter, Ob Nixilis and Liliana's Caress exiled by two more Vanishing Verses, the second Fate Unraveler
exiled mid-combat. Meanwhile Lost Mine of Phandelver ventures chipped 1-3 a turn under a Fog Bank that
could only ever stop one attacker.
- seq12 T8: cast Underworld Dreams through `[NAMED BY THEIR Silverquill Silencer: casting this costs you
  3 life and draws them a card - you would be at 15 ... PER CAST]`. Correct per rung 1b ("cast it THIS
  ask, ahead of every other card, **at every life total**"); Vanishing Verse took it the next upkeep.
- seq22/seq24 T12: rung 0 (BODY TURN) fired as designed - Ob Nixilis first while a creature of theirs was
  on his -2 list, then the -2 at seq24 killing Triumphant Adventurer #1 and drawing them 2 into the
  punishers (opp 18 -> 17). Repeated at seq28/seq31 T14. Both were the right rung.
- seq32 T15 Blockers, life 2: `Fog Bank blocks Goblin; Fate Unraveler blocks Silverquill Silencer` -
  Fate Unraveler took its `(you kill it, your blocker lives)` line, which is what BLOCKING clause (a)
  requires of the lone punisher. Correct; a third Vanishing Verse exiled it before damage.
- **seq36 T16, life 1: cast Dictate of Kruphix at K of 0 with `draw converters in your hand: 0`.**
  Dictate is one of the three rows the K-of-0 brake names, and the guide's LAST CHECK says in terms that
  this answer is wrong. The game was already lost; the violation is real. STRATEGY.

### 162 v 50 - WIN (you 21, opp 0, turn 14) - `162v50`
Erosion milled 20+ cards off the seat and wrathed twice; the seat won on punisher damage, not on bodies.
Ob Nixilis T10 (seq11) + Puzzle Box T12 (seq14) + Underworld Dreams/Shield Sphere/Howling Mine/Caress all
in one T14 window run (seq16-seq19) closed it: the opponent's T13 draw step alone paid 8.
- seq7 T6 and seq9 T8: cast Master of the Feast twice into `of which 0 are creatures` with Ob Nixilis x2
  in hand (`cannot pay now: needs 5 mana, you have 3/4`). Read literally, rung 4b forbids both - its
  0-creature branch is "cast it then only with no punisher in hand". Two violations; no cost here
  (both Masters were answered by Wrath of God and Evacuation anyway), but see section 4 for the
  ambiguity that probably produced them.
- seq12 T10 Cleanup discard: chose Forced Fruition, the most expensive spell in hand. Guide-correct.

### 162 v 125 - WIN (opp -3, you 10, turn 33) - `162v125`
The longest game and the best evidence that the K-of-0 brake is right. deck125 countered five spells
(Cancel, Dream Fracture, Essence Scatter, Fall of the Gavel, plus Path to Exile x2 and two Staff of Nin).
Underworld Dreams (T6), Ob Nixilis (T10), Master of the Feast (T12), Peer into the Abyss (T14) were all
answered; the board sat at K of 0 from T6 to T28 with Teferi's Puzzle Box the only castable row.
The seat declined it at every one of those windows (seq22, seq23, seq26, seq27, then the hold row at
seq29 and seq32) with plans reading "Pass to avoid feeding 5 cards with 0 converters" - correct, and
Puzzle Box would have handed a counterspell deck its whole hand back every turn for nothing.
**The deciding window is seq35, T28:** with Underworld Dreams re-drawn it cast the punisher first
(`draw converters ... on your battlefield: 0; ... in your hand: 1`), then Puzzle Box at seq36 with K now
1. Opp went 31 -> 24 -> 10 -> -3 on three draw steps. Textbook execution of the arithmetic paragraph.

### 162 v 130 - WIN (you 17, opp -1, turn 10) - `162v130`
Caress T3 (seq5), Master of the Feast T5 (seq7), Fate Unraveler T7 (seq9). All three attacker windows
(seq10, seq12, seq13) printed `[no creature they control can block this attacker]` on every A-line, so
the LETHAL CHECK's "EVERYTHING GOES" applied and the seat swung with everything including Fate Unraveler
(seq12, 3 damage, opp 13 -> 10). Deciding decision: seq12's double attack, which put them in range of the
Fate Unraveler draw trigger that finished them on their T10 draw step.

### 162 v 123 - WIN (you 16, opp 0, turn 13) - `162v123`
Master of the Feast T5 (seq6), Howling Mine T7 (seq11), Underworld Dreams T9 (seq19), two Dictates T11
(seq25, seq26), a second Underworld Dreams + Shield Sphere + Fate Unraveler T13 (seq28-seq30).
Every engine came after the first punisher; the two Dictates at K of 1 are exactly the release branch.
Deciding decision: seq19 (Underworld Dreams before Dictate at T9) - it is what made the T11 double
Dictate legal under the brake and turned their draw step into 3-4 a turn.
This game is also the seat's only lane-DI/DK exercise: `ask_replays_repeat_latch` 4 with
`identical_ask_answers_reserved` 4 (seq 8, 9, 14, 15 are absent from the translog), two
`cast_decision_reopened_new_stack`, two hold events both `hold_reopen_answer_invalidated`, and one
`crossphase_identical_reputs`. No `reask_reason` fired anywhere in the seat and no answer was replayed.

---

## 2. Engine / interface / card items

### HIGH-1 - The blockers seam prices an attacker's lifelink as a cost of BLOCKING and never as what they gain unblocked
Repro: `162v152` seq 11, T9, Blockers. Rendered lines:
- `A2. Intrepid Adversary (6/4) deals 6 [lifelink]`  <- bare tag, no owner, no figure
- `B1. Shield Sphere (0/6) ... - may block A2 (your blocker dies, attacker lives) {their attacker's
  lifelink, this block: they gain 6 from this block only, and this attacker deals nothing to your life}`

The gain figure appears only on the block line, so the menu reads as though declining avoids it. It does
not: a 6-power lifelinker gains its controller 6 whether it is absorbed by a blocker or dealt to the
seat's face. The one surface that stated ownership ("**they** gain 6") sat inside a brace next to the
phrase "from this block only", and the seat inverted both halves - its reasoning reads "I gain 6 life.
So net change is 0" and "blocking deals 6 damage to me". It answered BLOCKS: none and took 11 instead
of 5 in a game it then won at 4 life.
Direction: put the comparison on the A-line, both branches and the owner named, e.g.
`[lifelink - THEY gain 6 whether this attacker is blocked or not; you gain none of it]`, and drop
"from this block only" from the B-row brace. Note the one case where blocking *does* deny the gain -
a blocker that prevents the damage (Fog Bank) - which is exactly the comparison the seat cannot make
today because only one of the two figures is printed.

### HIGH-2 - P5: a collapsed damage batch prints the FIRST life total and never the batch's end total
Repro: `162v50` seq 15 (and seq 16-19), GAME LOG T13:
`Ob Nixilis, the Hate-Twisted -> 1 damage to the opponent (now 17) [x8 - 8 lines of this shape in this
batch; only the numbers in them differ]`
The opponent was at 18 before the batch and at **10** after it (the CURRENT SITUATION block of that same
prompt reads `Opponent life: 10`). A reader of the log alone comes away with 17, and the annotation's
"only the numbers in them differ" asserts the collapse is lossless when the one number that matters -
where the batch ended - is the one dropped. This is the exact line this deck's LETHAL CHECK is told to
read ("each Mine and each Dictate is 1 extra card per turn ... multiply extra cards by P").
6 collapsed batches in 4 of the 6 games: `162v152` seq11-14 (`(now 17) [x9]`); `162v50` seq13+
(`you milled Island [x5]`) and seq15+ (`(now 17) [x8]`); `162v125` seq37 (`(now 30) [x12]`) and seq38
(`(now 23) [x12]`); `162v123` seq27+ (`(now 12) [x4]`).
Direction: print the batch's closing total as well - `(now 17 ... 10)` - or drop the collapse for damage
lines specifically, where the running total is the payload.

### MED-3 - `async_drops` is a gameend-only counter with no per-window provenance
The seat carries 8 drops: `162v146` 6, `162v152` 1, `162v125` 1. No record in any translog names what
was dropped, at which window, or whether a re-ask followed; `dropped_assignments` is 0 on every blockers
record, and no `reask_reason` fired. The brief asks each seat to "say what dropped and whether the
re-ask was right" and the instrumentation cannot answer it. In this seat the count matches the blockers
window count in two games (`162v146` 6 blockers / 6 drops, `162v152` 1 / 1) but `162v125` has 1 drop and
0 blockers windows, and corpus-wide the correlation breaks (several 0-blocker games carry 2-5 drops), so
blockers alone is not the cause. Direction: an `async_drop` side record carrying seam, `window_seq` and
what was discarded. Relevant because the corpus total rose 9 -> 52 since wave 80.

### MED-4 - P5: the same draw step is reported twice, with different counts
Repro: `162v50` seq 15, GAME LOG: turn line `T13 (opp): drew.` followed by
`  Draw: opp put a card from their hand into their library (x6); drew 6; Ob Nixilis ... ; drew.`
P5's new register puts the draw on the turn line, but when the turn also gets a `Draw:` block the draw is
narrated in both places with different numbers. A seat whose whole clock is "cards they draw x punishers"
can read 1 where the answer is 6, or add the two. 7 turns across the seat: `162v152` T9 and T10,
`162v50` T13 and T14, `162v125` T30 and T32, `162v123` T10.
Direction: when a turn gets a `Draw:` block, the turn line should not also carry the draw.

### MED-5 - Seat's unlabelled-plan rate is more than double the corpus rate
9 of 137 windows (6.6%) are `plan_line_missing` / `protocol_deviation: unlabelled_plan` against the
corpus 2.9%: `162v152` seq11, 12, 14; `162v146` seq16, 19, 20, 30; `162v50` seq11; `162v125` seq36.
Every one of the nine wrote its plan sentence *before* the answer line and every one parsed to the
intended row - 0 re-asks, 0 `action_before_plan`, 0 fallbacks in the seat. So the robust parser held and
this is not an answer-quality item; it is a rate worth watching. Two of the nine also name an action that
is not on the menu: `162v146` seq16 plan "Play Island." at a Draw-step casting window, and seq20 plan
"Play Island to set up mana for Ob Nixilis next turn" answered `CHOICE: 2 (Hold priority)` - the hold
then re-opened at seq21 and the seat did cast Ob Nixilis at seq22, so nothing was lost.

### LOW-6 - P5: the discard punisher's life-loss line is printed before the discard that causes it
Repro: `162v125` seq 35, GAME LOG T21: `Cleanup: opp lost 2 life (now 28); opp discarded Plains.` Same
shape at T25. Liliana's Caress fires *on* the discard; the compacted log narrates the effect first.
Both facts are present and the total is right, so this is ordering only.

### LOW-7 - `deck162.txt` still carries `#HINT:dontattackwith(Fate Unraveler)` / `dontblockwith(Fate Unraveler)`
Those hints steer `AIPlayerBaka` and never reach the GPT seat, and the guide directs the opposite in both
cases (LETHAL CHECK: "swing with everything, Fate Unraveler included"; BLOCKING clause (a): it blocks on
a "you kill it, your blocker lives" line). Both guide behaviours fired correctly this corpus - `162v130`
seq12 (Fate Unraveler attacks for 3, opp 13 -> 10, into an empty board) and `162v146` seq32 (Fate
Unraveler blocks Silverquill Silencer on its kill line). Flagging the dead hint, not asking for a change
to either behaviour.

### LOW-8 - Reserved ask replays leave holes in the translog
`162v123` is missing seq 8, 9, 14 and 15 entirely; `identical_ask_answers_reserved` is 4 in that game's
gameend. The reservation consumes a seq but writes no record, so a reviewer cannot see which window was
re-served or confirm the latch key was right. A one-line side record would close it.

### Not found
No `WAGIC addToGame REFUSED a garbaged element` in any of the six games' stderr (`game-125v162`,
`game-146v162`, `game-152v162`, `game-162v123`, `game-162v130`, `game-50v162`). Those stderr files carry
only the known asset 404s (`*.wav`, `DBL/*.jpg`, `AFR/*.jpg`) and hold-re-open notices.

---

## 3. P9 and P5 counts

**P9 - the land drop folded into the casting menu.**
- Turns where a land row was on the menu: **45** (5 / 8 / 7 / 14 / 4 / 7 across v152 / v146 / v50 / v125
  / v130 / v123).
- Turns where a land drop was available and **NOT** taken: **0 of 45.**
- Turns where the land row was taken **instead of** a spell that should have gone first: **0.** The land
  row was taken in the turn's first casting window every time and the spell rows were re-offered in the
  same turn after it. Three turns ended with a land played and no spell cast - `162v125` T8 seq8
  (Master of the Feast the only spell row), T22 seq28 and T24 seq31 (Teferi's Puzzle Box the only spell
  row) - but in each the spell was declined at a later window on brake grounds (`draw converters ... on
  your battlefield: 0`), not displaced by the land.
- One shape worth recording, not a defect: when no spell is castable the land question still arrives as
  its own two-row ask (`1. Play Island / 2. Play no land right now`) rather than folded - `162v125`
  seq37 and seq38. The fold is conditional on a casting menu existing.

**P5 - the compacted GAME LOG register.**
15 defective instances in the seat, none of which is proven to have changed a decision:
- 6 collapsed damage batches printing the first life total and no end total (HIGH-2).
- 7 turns narrating the same draw step twice with different counts (MED-4).
- 2 cleanup steps printing the punisher's life loss before the discard that caused it (LOW-6).
No omission was found: in every case checked the fact was recoverable from the CURRENT SITUATION block
(life totals, hand size, library count), which is why none of the 15 is traceable to a bad answer. The
two decisions the seat did get wrong (`162v152` seq11, `162v146` seq36) both misread a surface that was
correct, so neither belongs to P5.

---

## 4. Guide verdict: **KEEP as is**

Across 137 windows the seat departed from the guide four times, and **every one of the four violated a
rule the guide already states in terms** - no gap was found that a new rule would fill:

| seq | what happened | the rule that already covers it |
|---|---|---|
| `162v152` seq11 | `BLOCKS: none` at 15 life into 11 unblocked | BLOCKING rung 0 - 15-11=4 is "8 OR LESS", so "block the BIGGEST 'deals N' you may block, and take that line when it kills your blocker" |
| `162v146` seq36 | cast Dictate of Kruphix at K of 0, hand converters 0 | THE BRAKE, K of 0 - Dictate is one of the three braked rows; LAST CHECK: "K of 0 on the row you picked AND your hand naming none of the three punishers -> your answer is WRONG" |
| `162v50` seq7, seq9 | Master of the Feast x2 into `of which 0 are creatures` with Ob Nixilis x2 in hand | rung 4b, 0-creature branch - "cast it then only with no punisher in hand and every other row braked, stranded or unwanted" |

Against that: 5/6 wins (3/6 in wave 80), 0 fallbacks, the K-of-0 brake obeyed at every one of the six
engine casts it governed except seq36, the punisher-before-engine ordering obeyed in all six games, the
first-Mine exemption used correctly twice (`162v152` seq6, `162v146` seq10), rung 0 (BODY TURN) firing
and paying off twice in the loss (`162v146` seq22/seq24, seq28/seq31), the `{reserve:}` STRANDS rule
obeyed at `162v152` seq12, clause (a) obeyed at `162v146` seq32, and the whole of `162v125` - 22 turns of
declining an engine at K of 0 against a five-counterspell deck, then punisher-first the moment K could
become 1 - is the guide working at its hardest case. That is a success verdict.

**The one rule in the guide that is wrong, and why it is not being edited this wave.** BLOCKING clause
(b) reads: `A "(lifelink: ... they gain N)" clause is damage you cannot undo - add that N to the line's
cost.` For an attacker's lifelink that is false: the controller collects N whether the attacker is
blocked by an absorbing body or hits the seat's face, so adding it to the block line's cost is a pure
distortion, and it points the same way the seat went at `162v152` seq11. The correct rule is a
comparison of two figures (what they gain from this block vs what they gain unblocked), and one of those
two figures **is not printed anywhere on the menu today** - that is HIGH-1. A correct clause costs about
150 bytes; the guide has 29 (19,971 of 20,000), so writing it means deleting a rule, and every rule my
six games exercised paid for itself. Writing a byte-neutral half-rule would either restate the wrong
thing or need the concessive clause A334 forbids. The ordering is therefore: land HIGH-1 so both figures
are on the page, and re-cut (b) against the new render in a later wave.

Two ambiguities noted for the record, neither edited: rung 4b's "no punisher in hand" does not say
whether a punisher that is `[cannot pay now: needs 5 mana, you have 3]` counts (the seat evidently read
it as not counting - `162v50` seq7, seq9); and BLOCKING rung 0's "block the BIGGEST" is what the seat
skipped past at seq11 after deciding, upstream of the rungs, that the trade was net zero.

No `wave81/deck162/strategy.txt` is written, and no general-guide or strategy-skill proposal is filed -
this seat's evidence points at the render (HIGH-1, HIGH-2), not at guide or skill text.

---

## 5. What I did NOT check

- The opponents' seat logs beyond the corpus-wide `async_drops` / blockers cross-tab in MED-3; deciding
  decisions were traced from the deck162 seat's own transcripts and the GAME LOG they carry.
- Card-script correctness against Oracle text for any card - no card behaved in a way that looked wrong
  in these six games, so no `primitives/*.txt` was opened and no Oracle text was retrieved.
- Whether the collapsed-batch and double-draw-narration defects (HIGH-2, MED-4) exist in other decks'
  prompts; I counted them only in the deck162 seat's 137 prompts.
- What a lifelink brace looks like on a *preventing* blocker (Fog Bank) - this corpus gave the seat
  exactly one lifelink combat and the only blocker offered was Shield Sphere, so the claim in HIGH-1
  about Fog Bank denying the gain is rules reasoning, not something I saw rendered.
- Any engine or source file: no `src/` or `bin/Res` file was read or edited, no build, no game run, no
  git. The deck list `bin/Res/ai/baka/deck162.txt` and the live guide were read only.
- P10 blockers ORDER: the seat had no `order` window this corpus, so nothing to report.
