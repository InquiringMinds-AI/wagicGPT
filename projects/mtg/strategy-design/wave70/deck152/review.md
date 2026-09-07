# Wave-70 deck 152 review — BANT HUMAN MIDRANGE

Corpus: matchups-20260906-224849 (5 games) + matchups-20260907-065922 (the deck126 rerun).
Seat files: `...ai_baka_deck152-<ptr>-vs-...`. 277 records, 257 decisions, **0 fallbacks, 0 truncations,
0 action_before_plan, reasoning_chars 0 in none** (p50 5,293 / p95 10,302 / max 15,090).
Live guide read: the wave-70 rewrite installed 2026-09-06 (19,999 B).
Engine items already routed to wave-71 are cited by their known-bugs L-number, not re-derived.

## 1 — Game by game

| # | Opp | Result | Turn | Life | Deciding decisions |
|---|-----|--------|------|------|--------------------|
| 1 | 146 | **LOSS** | 20/21 | -10 / 20 | variance, see below |
| 2 | 123 | win | 15 | 24 / -20 | clean curve; seq 24 + 30 alpha strikes |
| 3 | 130 | win | 11 | 18 / -4 | seq 14/16/20 — three unopposed swings |
| 4 | 125 | win | 25 | 19 / -2 | Lair of the Hydra animated to 8/8→11/11, seq 47/53/56 |
| 5 | 162 | win | 15 | 7 / -37 | **seq 29** Fateful Absence on Ob Nixilis stopped a 13-point draw-burn turn |
| 6 | 126 | **HUNG (L1)**, rerun won T11 20/-5 | 15 | 20 / 1 | **seq 32 and seq 41 — see HIGH-1** |

Seat record 5-1 counting the rerun.

**Game 1 (loss vs 146) is variance, not guide.** Turn-2 and turn-4 land drops, then eight
consecutive non-land draws — no land drop was even offered between turn 6 and turn 18 (the
prompt's `Land drop:` line reads "you have no land you could play right now" throughout). Stuck
on two lands the seat could cast exactly one two-drop per turn into Vanishing Verse, Soul Shatter
x2 and Kaya -3, and was 1-for-1'd five times. Its plays were individually right: seq 8/13
`don't add any counter` on Intrepid Adversary both had rows reading "leaves 0 of your 2 untapped
mana sources untapped" (guide's "only with no spare mana" — correct); seq 18 `ATTACK: none` at
life 8 into a printed CRACK-BACK of 8 is correct and the reasoning derives it cleanly. The one
arguable call, seq 10 Fateful Absence on a 1/1 Triumphant Adventurer with a Brutal Cathar in
hand, was forced: the Cathar costs 3 and the seat had 2 mana, so it had no cast row. That
exposes a wording bug in the guide (below), not a decision error.

## 2 — Engine / interface / card items

### HIGH-1 — the "blocking can leave them as high as N" clause is not honoured, and it cost the game
`1788752973-ai_baka_deck152-...vs-ai_baka_deck126.jsonl` seq 32 (turn 11) and seq 41 (turn 13).

seq 41 rendered lines:
```
A1. Elite Spellbinder (8/6) [flying] [no creature they control can block this attacker]
A2. Sigarda, Champion of Light (5/5) [trample, flying] [no creature they control can block this attacker]
A3. Luminarch Aspirant (3/3) [their untapped blockers: ... up to 13 if they also take every "may" ...]
A4. Katilda, Dawnhart Prime (3/3) [ ... same ... ]
ATTACK TOTAL: ... At least 13 damage lands whatever they block - that damage alone puts them at
-12, but every blocker they declare also fires the blocking triggers tagged on the rows above -
up to 13 life back across their 5 blockers, so blocking can leave them as high as 1.
```
Opponent was at **1 life**. A1+A2 alone are 13 unblockable damage and fire no blocking trigger at
all — a guaranteed kill. The seat answered `ATTACK: A1, A2, A3, A4`; deck126's five lifegain
defenders gang-blocked the two ground bodies and the opponent finished combat still at 1.

The `reasoning` field shows this is a **rules re-derivation against a correct surface**, not a
misread. The model quotes the ATTACK TOTAL line verbatim, then overrides it:

> "Blocking triggers give them up to 13 life, but combat damage resolves simultaneously or
> before? ... Even if they gain life before damage (which they don't in MTG rules, damage is
> simultaneous), 13 damage to 1 life is lethal. So it's fine."

That is wrong — the triggers resolve in declare-blockers, before damage — and the render had
already computed the right answer for it. The guide's own "Do not re-derive these outcomes; use
them" covers the attack tags but never states this timing, and never tells the seat that the
unblockable subset is the safe attack.

The identical miss had already happened **two turns earlier**, seq 32 (opponent at 7):
A1(7/5 flying)+A2(5/5 flying) = 12 unblockable = a guaranteed kill; the seat added A3 and the
render's "blocking can leave them as high as 1" came true exactly (7 − 12 + 6 = 1).

Cost: two consecutive guaranteed wins (turn 11, turn 13) turned into a board that was still alive
on turn 15, where it hit the L1 land-drop livelock — 3,641 s, 1.23 GB of stderr, game abandoned.
The replacement game with the same two decks ended turn 11, 20 vs −5. **Classification: STRATEGY
(guide), on a true surface.** Fixed by the guide edit below; nothing in the engine needs to change
for it, though see MED-1.

### MED-1 — reveal/search rows carry no legendary or duplicate annotation, unlike cast rows
`...deck152-...vs-ai_baka_deck162.jsonl` seq 38 (turn 13). Rendered:
```
1. Katilda, Dawnhart Prime (copy 1 of 2 in this list) (1/1 creature) {...} [eligible for "get a human"]
3. Katilda, Dawnhart Prime (copy 2 of 2 in this list) (1/1 creature) {...} [eligible for "get a human"]
```
The battlefield line in the same prompt already contains `Katilda, Dawnhart Prime {g}{w} (2/2)`
and the hand line already contains a second `Katilda, Dawnhart Prime`. The seat answered
`PUT: 1` and took a **third** copy of a legendary it both controlled and held — a dead card.
Cast menus print `[legendary: you already control <name> ... casting this sends one copy to your
graveyard]`; the reveal menu prints nothing. The row should carry the same clause (and an
"already in hand: N" note), so that `PUT: none` becomes visibly correct when every eligible row
is dead. Repro: same file, seq 38; also seq 49 offers three Brutal Cathar copies with no
"copies you already hold" note. Low game impact here (both eligible rows were the same dead
card) but the asymmetry is cheap to remove and the guide has to compensate for it today.

### MED-2 — the reveal window never states the destination of the chosen card
Same windows (10 in this seat: v123 20/25/31, v125 23/36/46, v162 19/38/49, v126 21/33/43,
rerun 24/29). The prompt says only `Choose ONE card that goes to "get a human"; every other card
goes to "put on bottom"`. Nothing says the card goes to your **hand** — verified against
`bin/Res/sets/primitives/borderline.txt:102246` (`moveto(hand)`) and Scryfall (Sigarda's Coven
trigger; primitive and Oracle agree, no card bug). The model guessed right in 4 of 10 windows
("Take Katilda to hand", v126 seq 21) and simply did not engage with the window in the other 6,
writing an attack plan instead (below). Render fix: name the zone in the option label.

### MED-3 — the reveal window's carried PLAN makes the model answer the wrong question
6 of this seat's 10 reveal windows produced a PLAN line about combat, not about the choice:
`'Attack with all creatures to deal lethal.\nPUT: 3'` (v126 seq 43),
`'Attack with all untapped creatures to win.\nPUT: 1'` (v162 seq 49),
`'PLAN: Attack with Sigarda, Intrepid Adversary, Brutal Cathar, and Luminarch Aspirant for 15
damage to leave opponent at 1 life.\nPUT: 1'` (v162 seq 38 — the dead-Katilda pick above).
The window fires mid-combat and the carried `YOUR PLAN` is the attackers-window plan, so the
model restates it and treats the reveal as ceremony. Related to L11/L12 but distinct: here the
*plan carry itself* is what drags attention off the decision. Suggest the reveal seam not carry
the combat plan, or carry it labelled as stale.

### MED-4 — this seat's whole off-protocol charge is one shape: a plan line with no `PLAN:` label
14 of 277 records (5.1%, matching the corpus 4.9%). Every one is an unlabelled first line;
none is prose after the action line, none reverses the action, none is a heading transcription.
Sample: `'\n\nAttack with Elite Spellbinder to apply pressure.\nATTACK: A1'` (v125 seq 43),
`'\n\nReveal Luminarch Aspirant.\nPUT: 2'` (v123 seq 20). This is the L11 render question
("what in the prompt makes the label droppable?") with a clean single-seat sample: 9 of the 14
are at `reveal`/`attackers` seams whose instruction line ends `Write your PLAN: line first, then
on a line of its own PUT:/ATTACK: ...` — i.e. the seams where the *action* label is emphasised
in caps and the plan label is not.

### LOW-1 — a PLAN line that is arithmetically wrong but harmless
v123 seq 26 → 27: plan says "add 2 valor counters", the next window's rows priced the counters at
{1}{W} each with only 1 spare mana, and the seat correctly answered `don't add any counter`.
Plan/action divergence with the action right; no mechanism needs to fire, but it is a data point
against any predicate that treats a changed plan as a reversal.

### Already-known items this seat reproduces (no new derivation)
- **L1** — the hang: `game-152v126-1788752967.stderr`, seat log ends seq 55 turn 15 on
  `Decline - do nothing` at a Hengegate Pathway face menu. HIGH-1 is what put the game there.
- **L12** — seam label leak: v123 seq 25 answered `BLOCKS: Sigarda, Champion of Light blocks
  Vampire.` then `PUT: 1` at a reveal window.
- **L19 / W69-BJ F1** — fires in v162 and v126 in this seat.
- **L8** — the Puzzle Box hand-replacement not folded into counts: visible in the v162 turn-13
  log batch (13 × `Ob Nixilis dealt 1 damage to you` in one draw storm).

### Latency (measurement, not a defect)
This seat: p50 **97.0 s** / p95 189.2 s across the 21-way parallel corpus, but p50 **28.6 s** /
p95 47.9 s in the solo rerun of the same matchup — a **3.4×** ratio. The corpus p50 of 80.7 s is
dominated by 21 games contending on one server, not by the cost of reasoning. Any budget or
timeout re-fit should be read off the solo numbers.

## 3 — Guide verdict: **EDIT**

Revised guide at `wave70/deck152/strategy.txt`, **19,980 B** (live: 19,999 B). Amendment-332
grep clean (no wave/seq/corpus/game citation).

**Edit 1 — new rule at the head of ATTACKING (paid for by v126 seq 32 and seq 41).**
before: the section opened `Send each listed creature with any of:` and the only guidance about
lifegain blockers was the BLOCKING TRIGGERS bullet's *"Two or more trigger-printing blockers on
one A-line -> ... send only your fliers"* — which does not fire at seq 32, where only one
trigger-printing blocker is named on the collapsed row.
after:
```
THE "ATTACK TOTAL:" LINE PICKS WHO GOES - READ ITS LAST CLAUSE BEFORE ANY TAG.
- "...that KILLS them whatever they block" -> send EVERY attacker listed; no block saves them.
- "...blocking can leave them as high as N", N above 0 -> send ONLY the A-lines tagged "[no creature they
  control can block this attacker]", nobody else. Those cannot be blocked, so they fire no blocking trigger
  and hand back no life; every blockable body you add alongside them is pure life for them, and N is what
  they end at. If the unblockable A-lines alone reach their printed life that IS the win - bringing one more
  attacker turns a kill into their survival.
Otherwise send each listed creature with any of:
```
It keys on a string the engine already prints at every attackers window, it is decision-time
only, and it covers both misses with one test instead of counting trigger-printing blockers.

**Edit 2 — the BLOCKING TRIGGERS bullet now states the timing** (paid for by the seq 41
`reasoning`, which lost the game by deriving the opposite).
before: `... First strike does NOT cancel it - it fires on the BLOCK, and the tag already applied
first strike. Two or more trigger-printing blockers on one A-line -> the outnumber rule does not
apply; send only your fliers and grind with removal.`
after: `... It resolves when blockers are DECLARED, so that life is already theirs before any
damage lands; first strike does not cancel it and the tag already applied first strike.`
(The deleted half is subsumed and generalised by Edit 1.)

**Edit 3 — Sigarda's Coven reveal window** (paid for by v162 seq 38 and the 6 windows in MED-3;
the guide said nothing about a window this deck reaches ten times in six games).
before: `- SIGARDA (...): Humans get +1/+1. Best card here; cast on curve, and she needs TWO
separate white sources.`
after: adds `When she attacks alongside creatures of three different powers her COVEN look fires:
a "get a human" reveal of your top five, and the card you name goes TO YOUR HAND. Take the
cheapest Human you can cast NEXT turn. These rows carry NO legendary warning, so check your own
battlefield and hand first - a second Katilda or Sigarda is a dead card, and when every eligible
row is one answer "PUT: none".` This is the guide compensating for MED-1/MED-2; if those render
fixes land, the last sentence can be dropped.

**Bytes.** The three edits add ~1,200 B, so they were paid for by:
(a) deleting the trailing `SITUATIONS` block (506 B) — three worked examples that restate the
BLOCKING ladder and the A-line stop rule verbatim, with no rule of their own;
(b) the BLOCKING TRIGGERS compression in Edit 2;
(c) prose compression that changed no rule: the THE DECK / ORDER OF OPERATIONS restatement of
DEPLOY FLOOR, the mulligan "would not cover any spell" / "Mana sources" bullets, LAIR OF THE
HYDRA, ONE LAND PAYS ONE PIP, ELITE SPELLBINDER;
(d) one real deletion — Tovolar's Packleader's `{2}{G}{G}` fight ability (77 B). It was never
offered as a row in any of the six games and is the least decision-relevant line in the card
block; flag it if a later corpus surfaces the row.

**A wording bug found but NOT changed** (no seq pays for a change, and touching it costs bytes I
do not have): FATEFUL ABSENCE reads *"with a Brutal Cathar in hand, cast the CATHAR instead"* —
unconditioned on affordability. At v146 seq 10 the seat held a Cathar it could not pay for and
correctly ignored the rule. The rule should say "with a castable Brutal Cathar row on the menu".

## 4 — Proposals
None. No general-guide change and no strategy-writing-skill change is supported by this seat's
evidence; both findings are deck-local (Edit 1 keys on a render string, Edit 3 on one card), and
`wave68/strategy-writing-skill-v2.md` already carries the render-string-over-recomputation
discipline these edits apply.

## 5 — What I did NOT check
- The deck126, deck146, deck123, deck130, deck125 and deck162 seat files beyond the context reads
  needed for the decisions above; the other seats' reviews own those.
- `game-152v126-1788752967.stderr` (1.23 GB) — I did not open it; L1 already carries its forensics.
- The 5 `async_drops` in the v162 game (gameend census) — no decision in the trace looked affected,
  but I did not correlate them to windows; the engine seat owns `async_drops`.
- Anything about the deck LIST (contents, ratios); I read only what the prompts rendered.
- I did not re-verify every card primitive — only Sigarda, Champion of Light (primitive
  `borderline.txt:102243-102253` vs Scryfall: agree exactly).
