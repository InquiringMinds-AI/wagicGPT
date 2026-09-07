# Wave 71 — deck 162 ("Draw and Die!", UB forced-draw punish) per-deck review

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-085638`, binary master b31f66cf3, `--thinking on`.
Seat files: `*-ai_baka_deck162-0x*-vs-*.jsonl` (6). 100 model decisions in this seat, **0 fallbacks**,
0 truncations, 0 `action_before_plan`, reasoning present on 100/100.
**3 records with `off_protocol_bytes`>0 / `plan_line_missing`** (3.0%; wave 70 was 8 of 134 at 6.0%).
Record: **4-2** — beat 123 (20–0 t11), 125 (20–0 t12), 126 (20–0 t12), 130 (19–(−4) t12);
lost to 146 (−4–16 t15) and 152 (−6–5 t13). Wave 70 was **2-4** with the same guide's parent.

**The wave-70 edits paid off, measurably and unevenly.** Edit A (Peer into the Abyss ungated) is
**UNTESTED** — zero payable Peer rows appeared in six games. Edit B (Dictate's flash is not a licence) and
edit C (`STRANDS` broadened to any card) **half-fired**: the model now cites the reserve rule in its
reasoning and declined a `STRANDS` row twice (152 seq16, 126 seq14) where wave 70 never declined one — but
it took the identical row at the **next window of the same turn** both times (152 seq17, 126 seq15), and
took a third with no decline at all (125 seq13). Edit D (Puzzle Box) and E (hold gate) had no test case.

## 1. Game by game

| Opp | Result | Deciding decisions |
|---|---|---|
| 123 | **WIN** t11, 20–0 | Fog Bank t3, Caress t5, Fate Unraveler t7, Puzzle Box t9; forced sac correctly took Fog Bank over Fate Unraveler (seq13). Clean. |
| 125 | **WIN** t12, 20–0 | 3 Underworld Dreams by t12 on a two-Mine board. Clean; seq13 took a `STRANDS` row (see below) and got away with it. |
| 126 | **WIN** t12, 20–0 | Sanguine Bond + Exquisite Blood on their side again — **won it this time**. Ob Nixilis t10 + −2, second Dreams t12, three −2 activations. |
| 130 | **WIN** t12, 19–(−4) | Ship a 1-lander (seq2), keep a 3-lander, bottom Puzzle Box (seq4): all three per guide. Lethal check cleared at seq17 and everything swung — correct, gang-block clause is below the lethal check. |
| 146 | LOSS t15, −4–16 | Double Vanishing Verse exiled Underworld Dreams (t6) and Fate Unraveler (t9). **seq10** cast a Dictate off a K-of-0 board into a 3-creature aggro board. Mostly matchup. |
| 152 | LOSS t13, −6–5 | **seq17 lost the game outright.** Detailed below. |

### 152 seq17 — the loss, and it is one decision

t12, own Draw step, life 14, **opponent at 9**, 5 untapped sources, board = 2 Underworld Dreams + 2 Howling
Mines, hand holds Ob Nixilis {3}{b}{b}. The Dictate row printed
`VERDICT: taking this row STRANDS Ob Nixilis, the Hate-Twisted this turn - 2 sources cannot pay 5`.
One window earlier (**seq16**, same turn, Upkeep, byte-identical row and verdict) the model **declined**,
plan: *"Cast Ob Nixilis in Main Phase 1, then Dictate of Kruphix if mana permits."* At seq17 it reversed.

The counterfactual is arithmetic, not judgement. Taken: 4 cards × 2 punishers = 8, opponent 9 → **1**, and
they killed the seat on t13 with 27 power. Declined: 5 sources + the land drop (seq18 played an Island) = 6,
Ob Nixilis castable for 5 leaving 1 — 3 punishers × 3 cards = **9 damage, opponent 9 → 0, dead on their own
draw step before combat.** The seat's own `DRAW FORECAST` at seq20 confirms the shape.

The `reasoning` shows two distinct failures, both quotable:
- **the verdict read as reassurance** — *"The verdict says it strands it \*this turn\* if I cast Dictate now,
  which is fine because I can cast it later in Main Phase 1. Actually, the prompt says 'so your main phase
  this turn is its last window.' That means I will cast it in Main Phase 1."* The clause that names the last
  window was read as a promise the window exists, not as the denial it is.
- **an off-by-one asserted as lethal** — *"9 - 8 = 1 life. They die."* Written twice in the trace.

STRATEGY on both counts; three separate guide rules already forbade it (the `STRANDS` rule, the
survive-the-re-ask rule, and THE ARITHMETIC's "a punisher in hand outranks every engine in it" — here the
punisher adds D+1 = 3/turn and the engine adds P = 2). More rules are not the fix; see §3.

### 146 seq10 — the same seam, with a **false** verdict

t10 Upkeep, life 17, K=0, their board 3 creatures vs the seat's 0. The Dictate row printed
`VERDICT: taking this row STRANDS Howling Mine this turn - 1 source cannot pay 2`. **That verdict is
wrong**: `Land drop: NOT yet used this turn` and the hand held two Islands, so the true main-phase count was
2, not 1. The model re-derived it correctly (*"But I can play a land first!"*) and was vindicated — seq11
played a Swamp, seq12 cast the Howling Mine. The reasoning also shows the K conflation:
*"draw converters in your hand: 2 ... So K is actually 2! ... The brake is released."* — the hand count read
as the battlefield K.

## 2. Engine / interface / card items

**HIGH — the `{reserve:}` STRANDS verdict ignores an unused land drop, and is provably wrong when one is
available.** `1788789423-...deck162-...-vs-...deck146.jsonl` **seq10**. The same prompt renders
`Land drop: NOT yet used this turn - you can still play a land`, a hand containing `Island (land: taps for
{U})`, and `VERDICT: taking this row STRANDS Howling Mine this turn - 1 source cannot pay 2`. Repro: read
seq10's verdict, then seq11 (`Play Swamp`) and seq12 (`Cast Howling Mine {2}`) — the strand never happened.
The guide's own no-`{reserve:}` arithmetic already says *"ADD 1 if 'Land drop:' reads NOT yet used and your
hand holds a land"*; the engine's verdict does not. Fix: fold the land drop into the reserve count, or say
in the clause that it is excluded. This matters because the guide (correctly) forbids re-deriving printed
verdicts — a false verdict here trains the model to override the true ones, which is what it then did at
152 seq17.

**HIGH — the STRANDS verdict buries its consequence behind a clause that reads as permission.** Same six
rows across five games (125 seq13, 152 seq16/17, 146 seq10, 126 seq14/15). The rendered order is
`... it is SORCERY SPEED, so your main phase this turn is its last window. VERDICT: taking this row STRANDS
X this turn - N sources cannot pay M, and it has no window after your main phase`. 152 seq17's reasoning
quotes the *"last window"* clause and concludes *"That means I will cast it in Main Phase 1."* Proposed
render: lead with the consequence in the second person — `VERDICT: take this row and you CANNOT cast X at
all this turn`. Six rows, two obeyed, four taken; the two obeyed were both reversed one window later.

**MED — a decline of a `{reserve:}` row is not carried to the next window at the same seam.** 152 seq16
(Upkeep, declined) → seq17 (Draw, identical row, taken); 126 seq14 (Upkeep, declined) → seq15 (Draw,
identical row, taken). `reserve_decline_windows_skipped` reads **0** in every gameend in this seat, so the
engine has a counter for this and it never fires. If the row is byte-identical and no mana entered between
the two windows, the engine holds enough state to either re-serve it with `[you declined this exact row at
the previous window this turn]` or suppress it, exactly as the hold machinery already does for repeated
lists. Either would have won the 152 game.

**LOW — `plan_line_missing` now fires, and it is the only protocol class left in this seat (3 of 100).**
All three are the wave-70 shape — the plan sentence written without the `PLAN:` label, then a well-formed
action line: `'\n\nCast Howling Mine.\nCHOICE: 1 (Cast Howling Mine)'` (152 seq12, 18 B);
`'\n\nCast Dictate of Kruphix to stack draws and guarantee lethal on their next turn.\nCHOICE: 1 ...'`
(152 seq17, 79 B); `'\n\nAttack with Master of the Feast for lethal damage; ...\nATTACK: A1, A2'`
(130 seq17, 199 B). Wave 70's MED item ("a reply with no PLAN line counts as protocol-conforming") is
**FIXED** — those records now carry `plan_line_missing: true` and non-zero `off_protocol_bytes`, and the
gameend counter agrees (152 gameend `plan_line_missing: 2`, 130 `: 1`). No answer was harmed.

**LOW — a search-to-battlefield ask omits that the card enters TAPPED, and offers no decline.**
`...deck162-...-vs-...deck125.jsonl` seq16: `Path to Exile's effect puts a card you choose from the list
below onto YOUR battlefield ... Pick the card you MOST NEED right now`. The primitive
(`mtg.txt:84676`) is `moveTo(mybattlefield) and!(tap(noevent))!!` and the printed text says
*"put that card onto the battlefield tapped"* — the render says neither, and the header ("this ask has no
pass row") gives no decline for what the card prints as *may search*. Harmless for a basic land in this
deck; a mana plan built on it would be false.

**LOW — the model under-reads a stack it is shown.** Same record (125 seq16): the prompt lists **six**
`Underworld Dreams's deal 1 damage` triggers plus a Mine draw, opponent at 5; the reply's plan reads
*"Opponent loses 3 life on this draw step, dropping to 2."* The stack renders one line per trigger with no
total. A one-line `STACK TOTAL: 6 damage to the opponent - they would be at -1` would close it.

## 3. Guide verdict: **EDIT**

New file `wave71/deck162/strategy.txt` — **19,911 bytes, from 19,963 (net −52)**, under the 20,000 ceiling.
Every addition is paid for by a cut; the cuts are rationale prose and a worked example that restates two
rungs, not decision rules.

| # | before → after | paid for by |
|---|---|---|
| A | `THE RESERVATION MUST SURVIVE THE RE-ASK: ... the SAME row returning gets the SAME answer.` → adds that upkeep, draw step and main phase are one turn and reach the SAME opponent draw step, so a flashed engine taken early collects **not one extra point of damage** and pays the strand in full; "declining is not delay"; and that *"I can still cast it in Main Phase 1"* is precisely what *"your main phase this turn is its last window"* **denies** | 152 seq16→seq17 and 126 seq14→seq15 (declined, then took the identical row one window later); 152 seq17 reasoning quotes the last-window clause as permission |
| B | `LAST CHECK BEFORE YOU SEND: K of 0 ...` → the check now leads with the strand: a picked row whose VERDICT begins `taking this row STRANDS` makes the answer WRONG, whatever it strands and however the count reads | 152 seq17, 125 seq13, 146 seq10 — the `{reserve:}` section is read early and lost by send time; the K brake already had a send-time check and holds |
| C | `K OF 1 OR MORE` / `K OF 0` → `K (A BATTLEFIELD COUNT, NEVER A HAND COUNT) OF 1 OR MORE` / `K (BATTLEFIELD) OF 0` | 146 seq10 reasoning: *"draw converters in your hand: 2 ... So K is actually 2! ... The brake is released."* |
| D | LETHAL CHECK gains: a count that leaves them at **1 or more is NOT lethal**, and the engine that produced it is the wrong card this turn — the next punisher adds D+1 a turn where the next engine adds only P | 152 seq17: *"9 - 8 = 1 life. They die."*; the same turn's correct line (Ob Nixilis for 3 punishers × 3 cards = 9) was the win |

Bytes returned: the `{reserve:}` clause quotation replaced by a reference to it (the row prints itself);
the duplicated "a plan that takes the row then says 'in Main Phase 1...'" tail (edit A says it harder);
the hold-row's "because the next window may differ" rationale; the MULLIGAN section's odds prose
(five mulligan/bottom decisions in this corpus, all correct — 130 seq2/3/4); the SANGUINE BOND paragraph
compressed (126 won under it this wave); SITUATIONS bullet 1 dropped (it restates rung 0 and the K brake).

**Not changed, deliberately:** BLOCKING, ATTACKING, FORCED SACRIFICE, WALLS, LAND DROP, CAST ORDER, the
`{feeds:}` brake and its release tests. No decision in these six games went wrong under them: 152 seq6
correctly skipped a Fog Bank at 0 enemy creatures and took the first Mine, quoting the right rung; 123 seq13
sacrificed Fog Bank over Fate Unraveler; 130 seq17 sent a Fate Unraveler into a printed GANG BLOCK because
the lethal check had cleared, which is the rule; 152 seq19 took the only affordable body on a KILL crack-back.
I considered tightening release test (ii) (the `life 15 or less` board gate, which let 146 seq10 through at
17) and rejected it: one game, and that Dictate was not what lost it.

## 4. Not checked / limits

- Only this seat's six translogs, plus the six opponent seats for outcomes. No lane adjudication (engine
  seat's job); no other deck's seats.
- **Peer into the Abyss saw zero payable rows** — wave-70 edit A remains unverified either way, and I did
  not touch it.
- The 146 loss is called *mostly matchup* on the strength of two Vanishing Verses exiling both punishers on
  curve; I did not model whether a different t10 line survives that board, so the cost of seq10 is bounded
  above, not measured.
- The 152 counterfactual assumes the seat's 6 post-land-drop sources include the {B}2 Ob Nixilis needs.
  Verified from seq19's untapped list (Underground Sea {u}/{b}, Swamp {b}, Island {u}) plus the three the
  Dictate tapped; not verified against a re-run.
- Path to Exile is verified against the primitive only (`mtg.txt:84674-84677`); no Scryfall call (network).
- No general-guide or skill proposals. Every defect here is either deck-162-local (the reserve seam, the K
  label, the lethal off-by-one) or an engine/render item; the reserve-seam fix belongs in the engine, and
  proposing "obey printed verdicts" as a general rule would be noise while one printed verdict is provably
  false (146 seq10).
