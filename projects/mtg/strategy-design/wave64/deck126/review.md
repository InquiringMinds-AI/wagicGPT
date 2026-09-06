# wave-64 per-deck review — deck 126 (Sanguine Blood, WGB walls + Sanguine Bond / Exquisite Blood)

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-001533/`, six games, seat files
`*-ai_baka_deck126-0x*`. 271 windows this seat (6 mulligans, 216 asks, 28 priority, 6 attackers,
1 blockers, 4 reveal, 6 gameend + system). **0 fallbacks, 0 dropped block assignments, 0 re-asks
recorded as such** (`identical_ask_answers_reserved` 10 in the 82-turn game, all reservations, no
model failure). Median latency 13.3 s; total model time 141 min.

## 1. Game by game

| opp | result | turns | end life | how it ended |
|---|---|---|---|---|
| 130 | **WIN** | 25 | 42 / 0 | Tutor→Bond (s10/s11 T11), Blood s29 T25, loop closed |
| 123 | **WIN** | 15 | 43 / 0 | Tutor→Blood (s8/s9 T7), Blood s12 T9, Bond s20 T13, loop closed |
| 146 | **WIN** | 14 | 30 / 0 | Tutor→Bond s7/s8 T6, Tutor→Blood s10/s11 T8, Bond s13 T10, Blood s14 T12 at 13 life paying 3 more |
| 162 | **WIN** | 13 | 41 / -3 | Bond s13 T9, Blood s16 T11, Tribute s19 closed it |
| 152 | LOSS | 11 | -8 / 20 | slow keep vs a curve-out; see below |
| 125 | LOSS | 83 | 0 / 44 | UW draw-go with 13 counterspells; see below |

**4-2.** In all four wins the seat executed the guide's stated line exactly: Idyllic Tutor for the
missing half, cast the half the turn it could, let the pair close. No misplay decided a win.

**126 v 152 (loss).** Deciding decision is the very first: `...deck126-0x55ee...-vs-...deck152`
**seq 2**, "Your hand (7 cards), counted by the engine: 6 lands, 1 spell. Cheapest spell in this
hand: Staff of Nin {6}" → **Keep this hand**. Deck 152 curved Katilda → Elite Spellbinder →
Tovolar's Huntmaster + 2 Wolves + Ranger Class; the seat's first spell landed turn 8 (seq 7,
Perimeter Captain) and both its real cards were exiled from hand by two Elite Spellbinders before
it could cast them (narration at seq 9). The keep is what the live guide **explicitly sanctions**
("six lands and one spell is a keep", MULLIGAN section) — see §3.

**126 v 125 (loss).** Not a play failure. Deck 125 is UW draw-go: 13 counterspells, 6 wraths,
2 Elixir of Immortality, 2 Staff of Nin. The seat's Sanguine Bond resolved T14 (seq 23) and every
subsequent half was countered — Staff of Nin T18 (seq 29) and T56 (seq 120) by Cancel, **Exquisite
Blood T60 (seq 127) by Fall of the Gavel** (the game-deciding one), Idyllic Tutor T80 and T82
(seq 148, 152) by Dream Fracture. It died to two Staff of Nin pinging 2/turn for 20 turns while
flooding to 22 lands. Three notes, all verified rather than assumed:
- the seat cast into open blue every time (seq 127 header: "Their untapped sources: 12 (colours
  they could make: {u}{b}{w})") — the surface was TRUE and PRESENT; this is a decision over a
  true surface, not a render defect;
- at seq 154, 1 life, it cast a **second** Sanguine Bond writing "Cast Sanguine Bond to complete
  the combo pair", against a row that says `[second copy: you already control Sanguine Bond]` and
  a LOOP HALF PENDING block that correctly named Exquisite Blood in the graveyard. The guide
  already forbids this ("a second Blood or a second Bond changes nothing about who is winning...
  this entry has NOT matched"). Teach present, teach ignored, seat already dead — no edit buys it;
- `Idyllic Tutor` row was truthful and named the target: `[finds only an enchantment card - still
  in your library...: Exquisite Blood]` (seq 148). Good surface.

## 2. Engine / interface / card items

### HIGH-1 — the best-case block projection re-uses a blocker that is already blocking
`1788671764-ai_baka_deck126-0x55ee4bdcab20-vs-ai_baka_deck152.jsonl` **seq 9**, turn 9, 19 life.
Blockers were already declared one window earlier (narration in this same prompt: "You declared
blockers: Perimeter Captain blocks Tovolar's Huntmaster"). The CURRENT SITUATION header then reads:

> `INCOMING THIS COMBAT: 5 attackers, 8 unblocked damage - you would be at 11 (of that, 3 from 1 attacker none of your creatures can block) - best case with every blocker assigned: you would be at 13 - one legal assignment that reaches it, chosen for your blockers' material as well as for the life: Perimeter Captain blocks Wolf #1; every blocker in it survives. This assignment is an OPTION, not an instruction: declining every block leaves you at 11 and costs you no permanent`

Both halves are false. The Captain is the seat's ONLY creature and it is already committed to the
6/6; it cannot also block Wolf #1. 19 − 8 = 11 is right, but the "13" is 19 − 6, i.e. the same
body's 2 damage subtracted a **second** time, and the named assignment is unreachable. The 8 in
"8 unblocked damage" already excludes the Huntmaster's 6 — so the header is subtracting the
Captain's work twice in one sentence.

Mechanism (read-only): `AIPlayerGPT.cpp:21480-21500` builds the candidate blocker set from
`bc->isCreature() && bc->canBlock()`, and `MTGCardInstance::canBlock()`
(`src/MTGCardInstance.cpp:1316`) tests tapped / CANTBLOCK / creature / battle / in-play — it has no
"already assigned to an attacker" test. So at any window in the Blockers step **after** the
declaration, every declared blocker is still counted free while its attacker is already removed
from `unblockedDamage`. The F8a/F8c machinery (material ranking, the OPTION-not-instruction label,
the decline figure) all fired correctly in FORM; the input set is what is wrong.

Cost here: the window at seq 9 is a "you may gain 2 life" trigger prompt, so the false figure did
not change the answer. But the same header renders at every post-declaration priority/instant
window, where it prices spending mana. Scope is corpus-wide, not deck-126-specific — one repro in
this seat because this seat had one blockers window all wave.

Suggested check for whoever takes it: exclude a creature whose `defenser` is set (or which appears
in some declared attacker's `blockers`) from `can`/`canNames`/`canCards`, and re-derive
`matchedAtk` from the same reduced set.

### MED-1 — the forced-sacrifice ask tells the seat to prefer the row that pays it LEAST, on the branch where the payment is a benefit
`1788671754-ai_baka_deck126-0x5648eb63fef0-vs-ai_baka_deck123.jsonl` **seq 10**, turn 8 (Devour
Flesh). The paragraph reads:

> `THE PRICE: YOU gain life equal to the sacrificed creature's TOUGHNESS - each row states what it would give you. Pick the creature you can best AFFORD TO LOSE (usually your least useful body, and - where the rows differ - the one that pays the least), and answer with the chosen creature's name.`

`buildForcedSacrificeAsk` (`AIPlayerGPT.cpp:25227-25248`) branches the PRICE sentence on
`gain` (1 = opponent gains, 2 = you gain) and then appends **one shared tail** for all three
values. On `gain == 2` the tail is exactly inverted: the seat is told to minimise a benefit, two
sentences after being told the benefit is its own. Two rows here differed by 1 life
(Pride Guardian 3 vs Overgrown Battlement 4) so the answer was dominated by which body to keep;
with two similar bodies the tail decides, and decides wrong.

Worse than generic for this deck: with Sanguine Bond on the battlefield, life the seat gains IS
damage dealt, and gaining the maximum can be the whole game. A guide teach cannot beat a
contradicting surface (perception-vs-strategy routing), so this belongs at the render.

Suggested check: on the `gain == 2` branch, invert the qualifier ("and - where the rows differ -
the one that pays you the MOST"), and keep the shared tail only for `gain` 0 and 1.

### MED-2 — the PLAN budget is never stated as a number, and the tail of that distribution is where the seat's inference time goes
The reply protocol (`AIPlayerGPT.cpp:233`) says the PLAN must be "CONCISE, a few sentences of
intent, not an analysis". The engine carries only `kPlanCarryMaxChars = 400`
(`AIPlayerGPT.cpp:395`). Neither the protocol nor any ask footer ever states 400.

Measured over this seat's 271 windows: 191 replies carried a PLAN line, median 312 chars, **max
5,618**; **62 prompts** carried the `[...the rest of your plan was not carried: N further
characters, of M you wrote]` notice — 3,315 of 3,542 discarded at
`...vs-...deck125` seq 149, 2,917 of 3,259 at seq 155. The four slowest decisions in this seat's
whole corpus are all long replies: 505 s / 5,794 chars (`...vs-...deck123` seq 21), 443 s /
2,507 (`...vs-...deck125` seq 41), 277 s / 1,573 (seq 32), 231 s / 2,184
(`...vs-...deck130` seq 15) — against a 13.3 s median. Roughly a fifth of this seat's total model
time was spent generating plan prose the engine then threw away.

This is an inference-economy item, not a play-quality one; the truncation notice already repairs
the downstream read. Filed as a proposal (see `general-proposals.md`) because the change is one
number in the reply protocol, not a deck teach.

### LOW-1 — first-decision latency
All six mulligan asks (seq 2, ~1.4 KB prompts — the smallest of the run) took 151-263 s, against a
13.3 s median for prompts 10x larger. Every game's first decision starts at the same instant under
`-j`, so this reads as server contention at run start rather than anything about the prompt.
Noted for the engine seat; no action proposed from one deck's view.

### Verified NOT defects (checked because they looked like ones)
- `[defender, doesn't untap during its controller's untap step]` on the seat's own walls
  (`...vs-...deck123` seq 10/18) is **true** — the opponent controlled Intruder Alarm, printed in
  the same prompt's opponent battlefield line. The live-keyword set was right.
- `Perimeter Captain`'s "may gain 2 life" (the extra menu at `...vs-...deck152` seq 9): the
  primitive (`mtg.txt:85398-85401`, `auto=@combat(blocking) source(creature[defender]|mybattlefield):may life:2 controller`)
  matches Scryfall's Oracle verbatim ("...you **may** gain 2 life"). Faithful; the extra window is
  the card, not the engine. `Pride Guardian` likewise matches (`mtg.txt:88356-88359`).
- The ~1,200-char collapsed-range explainer fired on 6 of 271 windows and **all 6** had a real
  collapsed row (e.g. `3-5. Add 6 green mana with Overgrown Battlement #1-#3 ... x3`). Correctly
  gated.

## 3. Guide verdict: **KEEP**

`bin/Res/ai/baka/deck126_strategy.txt`, 70,992 bytes, unchanged. This is a success verdict.

- The guide's win path executed in **4 of 4 wins** with no deviation: Rule #2 (Idyllic Tutor when a
  half is missing from hand and battlefield) fired at `130` seq 10, `123` seq 8, `146` seq 7 and
  seq 10; Rule #1's "cast the half even when the row says it taps you out" fired at `146` seq 14 —
  the seat cast Exquisite Blood at 13 life for 3 more life against a Silverquill Silencer naming
  it, and won two turns later. That is the exact line the guide argues for and it paid.
- Rule #4 (Tribute is an edict) was read correctly every time: three casts at `130` seq 15/19/23
  each quoting the "they choose which one" render, one at `162` seq 19 on the NAMED-victim form.
- The two losses do not indict a teach. The `152` keep is what the guide's own MULLIGAN section
  tells the seat to do, and the guide's reasoning for that line (36 of the 53 cards left are
  spells; a smaller hand does not fix a slow one) is not refuted by one loss to a hard curve-out
  plus two Elite Spellbinder exiles. One counter-example does not move an odds argument.
- The one edit I considered and **rejected**: a "bait the counterspell with a redundant permanent
  before casting a combo half when their untapped sources are high" teach for the `125` matchup.
  It would directly contradict the guide's existing, evidence-backed rule that "the thought 'I will
  cast it next turn with a land more behind it' is ALWAYS FALSE here — the half you did not cast is
  the half that gets countered, exiled or drawn past." Buying one draw-go matchup by hedging a rule
  that won four games this wave is a bad trade, and a guide that teaches doubt about a working line
  is the failure mode the trust doctrine names.
- The `125` seq 154 second-Sanguine-Bond cast violated a rule the guide already states verbatim.
  More prose against a teach that exists is noise.

## 4. What I did NOT check

- I read the deck-126 seat's six translogs in full plus `deck125.txt`'s decklist; I did **not**
  read the opponents' seat translogs for the other five games, so opponent-side decisions are known
  to me only through this seat's narration.
- I did not adjudicate any lane AG/AH/AI/AJ/AK prediction — that is the engine seat's job. HIGH-1
  touches F8's neighbourhood but is an input-set defect, not a verdict on F8a/F8b/F8c, all three of
  which rendered correctly in form at `152` seq 9.
- I did not measure HIGH-1's corpus-wide frequency (one blockers window in this deck all wave); the
  engine seat should count post-declaration windows carrying an `INCOMING THIS COMBAT ... best case`
  clause across all 42 files.
- I did not build, run wagic, or run the suite; no file under `bin/Res`, `src/`, or git was touched.
- I did not verify Oracle text for cards outside the four I named (Perimeter Captain, Pride
  Guardian, and the two combo enchantments as rendered).
- I proposed nothing to the strategy-writing skill: nothing this seat produced is a repeatable
  guide-writing lesson that amendments 1-331 do not already carry.
