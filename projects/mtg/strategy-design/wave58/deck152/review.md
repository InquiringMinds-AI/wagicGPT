# Wave-58 deck152 review (Bant Midrange)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-015059/`; the 152-vs-126 pairing hung
(F2) and is read from the rerun `matchups-20260904-042235/`. Seat logs = the
`...deck152-<ptr>-vs-...` jsonl in each directory. Record: **3-3** (W vs 123, 130, 162; L vs 146,
125, 126-rerun). 449 consumed decisions across the six games; 28 empty_reply fallbacks, 1
wall_miss, 0 unparsed.

## 1. Game by game

| Opp | Result | Turn | Life | Deciding decisions |
|---|---|---|---|---|
| 123 | WIN | 11 | 30/-12 | Clean escalating swing: seq 8 (1 attacker) -> seq 21 (3) -> seq 27 (5 of 5 offered). Guide rule #2 executed as written. |
| 130 | WIN | 15 | 19/-9 | Same shape; the one empty_reply (seq 27, T15 attackers) was recovered by the heuristic and did not decide the game. |
| 162 | WIN | 13 | 20/0 | Won *through* a 10-deep empty_reply burst (seq 32-50, all T13, all ~2515 ms) - the heuristic answered every window from Upkeep to the final attack. |
| 146 | LOSS | 17 | -5/21 | Not a play failure. Every threat was answered on sight: Wolf to Soul Shatter (T5), Katilda to Soul Shatter (T7), Sigarda to Silverquill Command (T9), Elite Spellbinder in a block (T11), Brutal Cathar to Kaya -3 (T13). The seat had exactly ONE attackers window in 17 turns (seq 35, T16). Nothing in the guide reaches this. |
| 125 | LOSS | 45 | -6/21 | UW control + 2x Staff of Nin. By seq 185 (T39) the board is 14 lands / **0 creatures** on both sides; 4 attacker windows in 45 turns. Death is Emrakul: seq 199-204 (T43) and 207-212 (T45) are annihilator-6 sacrifices. Matchup, not misplay. |
| 126 (rerun) | LOSS | 18 | 0/25 | The one loss with real decisions in it - see below. |

**126 rerun, the deciding line.** The seat is far ahead on board all game (8 creatures vs 3 at
T17) and its restraint is CORRECT, not timid: with their Sanguine Bond out, every block trigger
and every lifelink point is priced against the seat, so at seq 56 (T15, 6 attackers offered) and
seq 81 (T17, 4 offered) sending only the unblockable fliers (11 damage, zero converter payment)
beats attacking wide, which would have handed them 4-5 life back through Pride Guardian and the
Vampires. I checked the arithmetic both ways. The game was lost when Exquisite Blood joined
Sanguine Bond on T18: seq 82's own render says it - "ANY nonzero payment on a tag above is fatal"
- and by then blocking (they gain 1 through lifelink) and not blocking (same) were both lethal.
The only lever the seat still had at T17 was raw power, which is where the engine defect below
took two valor counters away from it.

## 2. Engine / interface / card items

### HIGH-1 - Intrepid Adversary's repeated `{1}{W}` payment stops early, then the mana line reads 0 with sources still untapped
Repro: `matchups-20260904-042235/1788513757-ai_baka_deck152-0x55d8b0a0be50-vs-ai_baka_deck126.jsonl`.
- seq 70 casts Intrepid Adversary, render: `{leaves 12 of your 14 untapped mana sources untapped}`.
- seq 71 (the counter menu) renders `Mana available: 12 total` and lists all twelve, of which
  **seven can make {W}**: `Overgrown Farmland {g} or {w}; Tidechannel Pathway {u}; Lair of the
  Hydra {g}; Forest {g}; Briarbridge Tracker {g}; Katilda, Dawnhart Prime {g} or {w}; Luminarch
  Aspirant #1 {w}; Augur of Autumn {g}; Deserted Beach {w} or {u}; Elite Spellbinder {w};
  Luminarch Aspirant #2 {w}; Intrepid Adversary #1 {w}`. Twelve sources at {1}{W} per counter =
  **6 payments**.
- The engine made **4** (narration at seq 79): `Overgrown Farmland; Lair of the Hydra` /
  `Deserted Beach; Forest` / `Luminarch Aspirant #1; Tidechannel Pathway` / `Luminarch Aspirant #2;
  Katilda`. Untouched and still able to pay two more: `Elite Spellbinder {w}` + `Briarbridge
  Tracker {g}`, `Intrepid Adversary #1 {w}` + `Augur of Autumn {g}`.
- seq 72, immediately after, renders `Mana available: 0 total (no untapped sources)` - four
  sources vanished without paying for anything.
Second instance same game: seq 59-60 (T15), `leaves 7 of your 9`, 7 sources -> 3 payments
affordable, **2** counters delivered (confirmed on the seq 81 board line, `Intrepid Adversary #1
... [counters: 2x valor]`).
Cost: valor counters are a team anthem (`auto=thisforeach(counter{0/0.1.Valor}>0)
lord(creature|myBattlefield) 1/1`, Oracle-correct per Scryfall). Two lost counters = **+2/+2 on
eight creatures** on the turn the seat needed to close. This is the gate-vs-planner shape: the
rendered "Mana available" and the payment loop disagree, and the loop under-delivers. Note the
model behaved perfectly here - it took "add 20 counters" exactly as the over-ask note instructs.

### HIGH-2 - F2 evidence from this seat (the hung 152-vs-126 game)
`matchups-20260904-015059/1788504666-ai_baka_deck152-0x55a7f2ef7f70-vs-ai_baka_deck126.jsonl`
(31 records). The three records before the hang are **all heuristic fallbacks**: seq 26
(attackers, empty_reply, 2503 ms), seq 28 (reveal, empty_reply, 2502 ms), seq 30 (ask, blockers,
empty_reply, 2542 ms) - each followed by a `recovery` record (27, 29, and none after 30). The
narration inside seq 30 shows the Coven trigger already resolving: five cards revealed
(Plains, Intrepid Adversary, Ranger Class, Teferi, Katilda) and `You used: get a human with
Sigarda, Champion of Light`. So the "Choose an option for Sigarda ... -> chose -1 of 2" that
stderr records **arrived while the model had been out of the loop for three straight windows**;
the -1 is far more likely the Baka fallback answering `chooseMenuAction` than a model reply,
which also explains the missing translog record (the fallback path writes no receipt for that
seam). Suggest lane G test the reproduction with the seat forced onto the fallback path, and
that the fallback answer at every seam be stamped in the translog - a decision with no receipt
is invisible exactly when the game dies.

### HIGH-3 - Teferi's +1 is decided from a truncated text and verbless rows
`{effect: "+1: Choose up to one target artifact, up to one target creature, and..."}` -
the board render truncates at exactly the point where the only verbs live. Oracle/primitive
(`planeswalkers.txt:3350`): "**Untap** the chosen permanents you control. **Tap** the chosen
permanents you don't control. You gain 2 life." **48 occurrences** of that truncation across my
449 decisions. The follow-up menus then read (rerun seq 43/44/46/48, 2 options each):
`1. choose a land / 2. Decline - do nothing`, then `1. choose your land / 2. choose opponent
land` - raw lowercase script tokens, no verb, **42 such rows**. The model is being asked to pick
tap-vs-untap with the words "tap" and "untap" nowhere on screen. Observed waste at rerun seq 45:
it picked `Sunpetal Grove #1 [land] [opponent's battlefield] [tapped]` for the TAP branch - the
row's own `[tapped]` tag says the mode does nothing there. Fix shape: never truncate a card's
text mid-clause when a menu is being generated from it, and render the mode rows with the verb
they perform ("untap one of your permanents" / "tap one of theirs").

### MED-4 - One Teferi +1 costs seven model round trips
Rerun seq 42-48, 61-67, 72-78 and vs146 seq 24-31: `+1` -> `choose a land` -> `choose your/
opponent land` -> target -> `choose a creature` -> `choose your/opponent creature` -> target ->
`choose an artifact` -> ... Across my six games **40 of 449 decisions (8.9%) and 618 KB of
5.66 MB of prompt text (10.9%) were spent on six activations of one +1 ability**, each round
trip re-sending the full board. This is the bundling case the project already made for blockers
and attackers: one ask, "name up to one artifact, one creature and one land, and for each say
yours or theirs."

### MED-5 - Annihilator 6 is six full asks
vs125 seq 199-204 (T43) and 207-212 (T45): `FORCED sacrifice ... Choose card 1 of exactly 6`,
each a separate ~27 KB prompt. The "N of exactly 6" label is good; the six round trips are not,
and nothing in the sequence needs a fresh board read between picks.

### MED-6 - The attackers ask never totals the damage; the blockers ask does
Every blockers ask carries `INCOMING THIS COMBAT: N attackers, N unblocked damage - you would be
at X`. The attackers ask has no mirror: it prices each attacker's cost (`their converter takes 3
off you`) but never states what the swing DELIVERS, so the only summed number in front of the
model is the price. Compare rerun seq 56/81 with vs125 seq 199. A line of the form "if they make
their best block, they would be at N" would let the model net the two.

### MED-7 - empty_reply bursts have no receipt
28 of my 449 decisions (6.2%). Every one has latency 2357-2542 ms against 24-40 s for a real
reply, and they arrive in runs (vs162 seq 32-50, ten in a row at T13; vs125 seq 26-46, ten at
T13; vs126-hung seq 26-30). No correlation with prompt size (vs146 answered 18.5 KB prompts with
zero empties; vs125's empties were on 8-9 KB prompts). Nothing in the game stderr records a
status code or body - `/usr/bin/grep -i "http\|error\|timeout" game-152v162-*.stderr` finds only
the endpoint banner. Whatever the cause (server-side, on port 8084), the corpus cannot tell.
One `wall_miss_unrecorded` at **900037 ms** (vs125 seq 50) is in the same family.

### LOW-8 - A Class enchantment's live level is not derivable from its render
vs146 seq 35: `Ranger Class {1}{g} [enchantment] [counters: 1x level] {effect: "(Gain the next
level as a sorcery to add... -- When Ranger Class enters, create a 2/2... -- {1}{G}: Level 2 --
... -- {3}{G}: Level 3 -- ..."}`. All three tiers are printed with no marker of which are active,
and the model spent its entire PLAN at seq 35 arguing itself in circles about it ("It says '1x
level'. So it is level 1... Level 3 costs {3}{G}... Let's level to 3"). A `[level 1 of 3; live:
Wolf token]` style tag would end it.

### LOW-9 - raw engine tokens as keywords
`Augur of Autumn ... [canplaylandlibrarytop, canplaycreaturelibrarytop, showfromtoplibrary]`
(rerun seq 81 board line) is presented in the same brackets as real keywords like `[vigilance]`.

## 3. What the lanes DID buy (positive evidence from this seat)
- **Lane C / D9**: `INCOMING THIS COMBAT` now renders outside combat - vs125 seq 198 (Upkeep,
  `not declared yet - 1 of their creatures can attack, for up to 15 - you would be at 13`) and
  rerun seq 68 (Main phase 1). 20 renders, 0 of them contradicted by the combat that followed.
- **X over-ask note**: rerun seq 60, the model read the note and deliberately chose `add 20
  counters` with 7 mana, stating the real number in its plan. The instruction works; only the
  engine's payment (HIGH-1) fell short.
- Zero occurrences in 449 decisions of: `{right now: <verb> 0}` badges, `NO LIVE CAST ROW ON THIS
  MENU`, lowercased script tokens in narration, `[<- best trade ...]`. Absence of a window, not
  proof of a fix.

## 4. Guide verdict: KEEP as is
No `strategy.txt` written. The evidence does not support an edit:
- Rule #2 (develop, then attack with everything that profits) was **executed**: the three wins are
  1 -> 3 -> 5 attacker escalations, and the corpus contains no instance of a creature left home
  against a friendly tag.
- The two apparent under-attacks (rerun seq 56 and 81) are correct under the opponent's Sanguine
  Bond, which the render prices per attacker; widening those swings loses value. Writing a
  "swing wider anyway" teach on this evidence would have taught a misplay.
- The three losses trace to removal density (146), a control matchup with no creature windows
  (125), and a two-card life-loop the deck holds no answer to plus HIGH-1 (126). None is a guide
  gap.
The guide stays at 65,395 bytes, inside the 41-71 KB band.

## 5. Not checked
The opponents' seat logs beyond the context I needed for the traces; the vs123/vs130/vs162 games
decision-by-decision (I read their outcome shape and fallbacks only); whether HIGH-1 reproduces
outside Intrepid Adversary (any repeated-payment `thisforeach ... may pay(...)` card - I did not
test another); the mulligan/bottom seams (only the rerun had one, seq 1-3); the F2 `-1` claim is
inference from adjacency, not a traced call path; and I did not run wagic, a fixture, or any
probe - everything here is read off the translogs, the stderr files, the primitives and Scryfall.
