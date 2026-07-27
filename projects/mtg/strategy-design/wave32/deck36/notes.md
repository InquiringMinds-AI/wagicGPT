# deck36 — development notes (engine / harness / representation ledger)

Seat: deck36 (Master of Ether, UB artifacts), wave-32 Step-0 debut. Binary `7fabd9bd0`.
Corpus `matchups-20260727-115847`. All seq/turn citations are from the deck36 seat log of the
named matchup. Card facts read from `bin/Res/sets/primitives/mtg.txt`.

---

## N-36a — POISON COUNTERS ARE INVISIBLE, AND THE BLOCKERS HEADER COMPUTES FALSE LIFE MATH AGAINST INFECT  [HIGH — cost a game]

**Symptom.** deck36 lost its vs105 game as the corpus's only cap adjudication, at 20-20 on turn 12,
having taken zero life damage all game. The seat could not see the clock that was killing it.

**Two defects, one window.**

1. **No poison surface anywhere.** `/usr/bin/grep -i poison src/AIPlayerGPT.cpp` returns exactly one
   hit and it is an unrelated code comment. Neither player's poison-counter total appears in the
   board block, the life line, the narration, or any option annotation. The word `poison` reaches
   the pilot ONLY inside opponent card text (`{card text: "Infect -- When Ichor Rats enters, each
   player gets a poison counter"}`) and in the pilots' own prose.
2. **The blockers header states life arithmetic that is false for infect damage**, in 5 of 5 blocker
   windows of that game (s9, s13, s17, s20, s25). Verbatim, s25 t12:
   `Your life: 20. Unblocked, these attackers deal up to 5 - you would be at 15 - NOT lethal: block
   only where the trade favors you; taking damage while ahead is often correct.`
   Every attacker in that window (`Plague Stinger`, `Contagious Nim`, `Necropede`, `Ichorclaw Myr`)
   carries `[infect]`. Life did not move; poison did.

**Arrival-traced consequence (the decision).** s25 reply, verbatim: *"Since the total damage (5) is
not lethal (I would drop to 15), and blocking with the Pest achieves nothing but removing my only
potential future blocker, I declare no blocks."* The pilot reasoned correctly FROM the surface it
was given. **Same-seam control (HL4-1):** the identical header shape at deck158 s19 (non-infect
attackers, `you would be at 3 - NOT lethal`) produced a correct, favourable double block — so the
header machinery works; the branch that is wrong is the infect one.

**Fix, both halves.** (a) Add poison to the situation block for BOTH players wherever life is
printed (`Your life: 20 (poison 7 of 10) | Opponent life: 20 (poison 0 of 10)`) — 10 poison is a
loss, so this is an alternate-win threshold the pilot currently cannot pursue or defend.
(b) In the blockers header, when an attacker has infect, its damage must not be subtracted from
life; either partition the projection (`3 life damage -> you would be at 17; 5 INFECT damage ->
poison 7 of 10 - LETHAL at 10`) or suppress the life projection and print the poison one.
Per HL5, enumerate every emitter of the `you would be at N` string before declaring this fixed.

**Cross-seat consequence for synthesis.** deck105's 6/6 guideless perfecta was played against
opponents that could not see its clock and were told its attacks were survivable. Its record should
not be read as decision quality until this is fixed.

---

## N-36b — A {0} MANA COST RENDERS AS AN EMPTY STRING AT ALL THREE EMITTERS  [HIGH — the seat's #1 leak]

**Mechanism, located in code.** `src/AIPlayerGPT.cpp` guards the cost token with
`if (cost && cost->getConvertedCost())` at three sites: the DECKLIST emitter (~line 845, system
prompt), the CAST-OPTION emitter (~line 1658), and the HAND/BATTLEFIELD emitter (~line 4215). A
converted cost of 0 is falsy, so the token is dropped everywhere. Path enumeration per HL5: those
are all three; there is no fourth emitter of a card's cost.

**Live measurement.** 347 renders of `Mox Jet` / `Mox Sapphire` / `Ornithopter` across the 144
deck36 prompts. **Zero carry a `{0}`.** The system-prompt decklist is bare too — the pilot quoted
it: *"Deck list: '1x Mox Jet: {T}: Add {B}.' It does not show a mana cost."*

**Arrival-traced cost.** 4 of the seat's 7 `Cast nothing right now` decisions declined a free
artifact, each with an invented rule:
- deck116 s3 t0: *"Mox Jet and Mox Sapphire are artifacts that provide mana when tapped; they are
  not spells you can cast from your hand."*
- deck116 s4 t0: *"you do not 'cast' artifacts... putting them onto the battlefield requires a land
  drop slot, and you have already played a land."*
- deck139 s5 t1: *"Moxes are artifacts that I can cast, but they cost mana to cast. I have no extra
  mana to cast them."*
- deck139 s3 t1: *"Ornithopter/Moxes are low priority compared to fixing my mana base."*
Both Moxes were delayed a full turn in deck116. **Same-seam control:** in the same games the pilot
took the SAME option (`Cast Mox Jet`, `Cast Mox Sapphire`, `Cast Ornithopter`) 13 times and every
one resolved (`hand -> stack` / `stack -> battlefield` in the narration), so the engine's offer/
resolve path is sound — the defect is purely the render.

**Tax.** The two largest cast-seam replies at this seat are deck152 s3 (8,600 chars / 133 s) and s4
(7,040 / 107 s), both spent deriving whether a Mox is free — a low-branching decision, the HL9
saliency signal.

**Fix.** Print `{0}` when `cost` exists and its converted cost is 0. Suggested guard:
`if (cost && (cost->getConvertedCost() || card->hasZeroPrintedCost()))`, or simply emit
`cost->toString()` whenever `cost` is non-NULL and the card is not a land. Removal condition for the
guide's RULE #1 stopgap: two consecutive corpora in which no cast-nothing decision cites a missing
or unknown cost.

**Generalization worth checking (same guard shape elsewhere):** any emitter written
`if (value) print(value)` silently deletes a legitimate zero. Candidates in the same file: 0-power
creatures, 0 counters, empty subtype lines.

---

## N-36c — MASTER OF ETHERIUM'S CAST LINE IS ONE SHORT (self-exclusive CDA at option-build time)  [MED]

`Master of Etherium`: `power=*`, `toughness=*`,
`anyzone=type:artifact:mybattlefield/type:artifact:mybattlefield cdaactive`. The CDA is evaluated
correctly and tracks live in hand (deck139 s1 -> s13, 0/0 -> 6/6 in lockstep with
`Artifacts in play: you N`). But the cast option is built while the card is still in hand, so the
count excludes Master itself and the body arrives one bigger.

Verified 2/2 (every cast in the corpus):
- deck139 s14 `Cast Master of Etherium {2}{u} (6/6)` at `you 6` -> s15
  `Master of Etherium {2}{u} (7/7) (printed 0/0) [artifact]` at `you 7`.
- deck116 s10 `Cast Master of Etherium {2}{u} (5/5)` at `you 5` -> s11 `(6/6) (printed 0/0)` at
  `you 6`.

The pilot drifted in both directions off the unannotated number (deck116 s10 planned a "4/4"; deck146
s24 planned a "9/9" at `you 10`, i.e. an 11/11). Same family as Gray Merchant's `{drains N}`
under-count, whose fix was to build the magnitude against the post-resolution state. Guide carries
"+1" as a stopgap; **removal condition: the cast line prints the post-resolution value.**

---

## N-36d — A BARE MANA ABILITY IS OFFERED AS A STANDALONE PRIORITY ACTION  [LOW-MED]

`Tolarian Academy`'s `{T}: add {U} per artifact` is offered as the only legal action in windows where
there is nothing to spend it on, including the pilot's own UPKEEP. The pilot took it in Upkeep 6 of
8 times (deck146 s8/s13/s18/s22/s26, deck152 s28) — the mana empties at end of step and the land is
then `Tolarian Academy [tapped]` for the whole main phase (visible deck146 s14/s19/s23/s27).

**Measured game cost this corpus: zero** — at every affected main phase the hand held only lands
(deck146 s19/s23/s27: `Your hand: Island (land: taps for {U}); Vault of Whispers (land: taps for
{B})`). Recording it without a cost claim (HL6). Worst case is live and large: the same game reached
`Artifacts in play: you 10`, a ten-mana tap discarded five turns running.

This is the `Cast Card Normally with Mox Opal` defusal class on a new card. Options: suppress a
standalone mana-ability offer when the pool cannot be spent this window; or annotate it
(`[mana ability - this mana empties at the end of this step]`). Guide carries a per-window rule as
the interim.

---

## N-36e — THE PLAN FIELD ABSORBS THE MODEL'S OWN QUOTED PLAN PLUS EVERYTHING AFTER IT  [MED — parser]

**51 of 132 prompts (39%)** carry a `YOUR PLAN (as you last stated it):` block longer than 400
characters; several run 600-800. The splitter anchors on an in-reasoning quotation. deck152 s3's
reply contained, inside its deliberation, the string
`But wait, the prompt says "Your Plan: Play Ancient Den. Use Mox Jet and Mox Sapphire..."`, and the
s4 prompt's plan field is that quote plus the following ~20 lines of unresolved reasoning — the
entire "Moxes have no cost listed / I cannot pay {1}" derivation. s5's plan field then carries s4's
confusion the same way.

Effect: the pilot's own unsettled reasoning is fed back to it as authoritative plan text on the next
decision, which is exactly the fuel for the belief-rehearsal spiral in N-36b/N-36f. Fix: extract the
plan from the LAST line-anchored `PLAN:` occurrence only (line-leading, as the c4 answer-anchoring
already does for choices), and cap the field length.

---

## N-36f — "coloured mana cannot pay a generic cost", with the core line PRESENT  [MED — route to representation]

deck152 s4/s5 verbatim: *"'Mana costs are written like {2}{R} (two generic plus one red).' '{1}' is
one generic. If I have {W} and {B}, I cannot pay {1}."* The pilot QUOTES the core prompt's own
sentence and draws the opposite conclusion. Cost at deck152 s5 t1: `Mana available: 3 total (...
colours you can make: {u}{b}{w})` with Steel Wall {1}, Silver Myr {2} and Glaze Fiend {1}{B} all
offered — answered `Cast nothing right now` (recovered in Main 2, so one combat step lost, not the
game).

Per layer-routing this is a core line PRESENT and LOSING at distance, so it routes to representation,
not to a second core paragraph. Cheapest surface fix: make the mana line say what it buys rather than
what colours exist, e.g.
`Mana available: 3 total (three untapped sources; any of them pays a generic {1}; coloured pips
available: {u}{b}{w})`. Guide carries RULE #2 as the interim.

---

## N-36g — a Clue token's card text renders as literal `()`  [LOW — cosmetic]

deck152 s15/s16: `Draw 1 with Clue [cost: {2}, Sacrifice] {card text: "() source: Cathodion"}` and
`{card text: "() source: Steel Wall"}`. The Clues are legitimate (deck152's `Fateful Absence` killed
the creature and its controller investigates — narration confirms
`Your Cathodion: battlefield -> graveyard` / `Your Clue: created -> battlefield`), and the option
LABEL carries the real information. Only the `{card text: ...}` payload is empty and the `source:`
suffix reads as if the Clue were the dead creature. Cosmetic; the pilot took the right action both
times. Adjacent nit in the same log: `Opponent's Wolf: graveyard -> zone`.

---

## N-36h — i10 PROBE SPEC (the attachment renders have no witness left in the pool)

**Verdict at the natural witness seat: UNEXERCISED.** `attached to:`, `{attached:`, `[equipment]`
and `Equip ` occur **0 times across all 42 translogs of all 21 games** in this corpus. deck36 runs
no equipment or auras and neither does any other wave-32 pool deck; wave-31's 112-reverse/0-forward
measurement came from deck148, which rotated out in wave-31.

**Trigger condition:** an Equipment or Aura permanent ATTACHED to a creature, either side, at any
render window. **Corpus count: 0.**

**Probe spec.** Build `deck198.txt` / `deck199.txt` in the numbering gap (verify the contiguous
roster ends below 198 first). deck199 = the attachment probe: 4x a cheap Equipment with an Equip
cost, 4x a beneficial Aura, 8-10 cheap creatures with duplicate names (so the reverse render must
carry an instance handle: `[attached to: Memnite #2]`), 20 lands. deck198 = a plain creature deck as
the opponent. Copy the nearest equipment guide as `deck199_strategy.txt`. Run 2 one-shot GPT games
with `timeout 3000`, then grep the translogs for BOTH directions:
`[attached to: <name> #N]` on the creature's line and `{attached: ...}` on the equipment's line, and
confirm the handle in the reverse render matches the handle the battlefield line gives that creature.
Delete both probe decks afterwards and verify the highest `deckN.txt` is back to the real roster;
preserve decklists + translogs + report under `strategy-design/wave32/probe-attachments/`.

---

## Deck construction flags (roster owner, not fixable by guide)

- **2x Akroma's Memorial {7}** in a deck whose wins land on turns 7-8. It was drawn in 2 of 6 games,
  bottomed to a London mulligan once, and was never castable (deck158 s19/s21: in hand at
  `Mana available: 4` then `5`). It contributed directly to the vs158 loss as a dead card. The deck
  supports at most one, and arguably zero.
- **2x Vedalken Archmage {2}{U}{U}** is the only nonland non-artifact card in the deck; it is also
  the only double-blue cost in a five-colour artifact-land manabase. It was never cast in 6 games.
- Otherwise the shell is coherent and its ceiling is high: both games in which Master of Etherium was
  drawn ended by turn 8 with zero damage taken.

---

## Falsifiable predictions for the wave-33 reviewer (Step 0-ter validation of this guide)

1. **RULE #1 fires:** zero `Cast nothing right now` decisions in a window where a {0}-cost artifact
   was offered (baseline 4/7). If N-36b ships, also expect the cost token `{0}` on those lines.
2. **RULE #2 fires:** zero replies containing a "cannot pay {1} / no generic mana" derivation
   (baseline 2 replies, 1 costing a window).
3. **Master read:** every plan naming a Master cast states a power one HIGHER than the printed
   `(N/N)` (baseline 0/2 correct). If N-36c ships instead, the guide's "+1" line must be RETIRED that
   wave (stopgap-retirement is a positive obligation) and the prediction inverts to "the plan matches
   the printed number".
4. **Academy window:** zero Upkeep activations of Tolarian Academy (baseline 6/8).
5. **PRESERVE — must NOT regress:** attacker declarations stay at 20/21 or better against the
   non-`[deals 0]` eligible set; blocks stay defensible; mulligans stay <= 2 per game and Akroma's
   Memorial remains the first bottom. Any drop here is the guide doing harm and the relevant block
   comes back out.
6. **Sequencing:** artifacts cast in Main Phase 2 on a turn the seat also attacked should fall
   (baseline 4 instances: deck139 s4, deck152 s6/s7, deck158 s15).

---

## Appendix A — card-fact table, read from `bin/Res/sets/primitives/mtg.txt` this session

| Card | mana= | type= | P/T | script |
|---|---|---|---|---|
| Master of Etherium | {2}{U} | Artifact Creature | */* | `lord(other creature[artifact]\|mybattlefield) 1/1`; `anyzone=type:artifact:mybattlefield/... cdaactive` |
| Glaze Fiend | {1}{B} | Artifact Creature | 0/1 | `abilities=flying`; `@movedTo(other artifact\|myBattlefield):2/2 ueot` |
| Salvage Slasher | {1}{B} | Artifact Creature | 1/1 | `foreach(artifact\|mygraveyard) 1/0` |
| Cathodion | {3} | Artifact Creature | 3/3 | `_DIES_Add{3}` |
| Silver Myr | {2} | Artifact Creature | 1/1 | `{T}:Add{U}` |
| Steel Wall | {1} | Artifact Creature | 0/4 | `abilities=defender` |
| Yotian Soldier | {3} | Artifact Creature | 1/4 | `abilities=vigilance` |
| Coiled Tinviper | {3} | Artifact Creature | 2/1 | `abilities=first strike` |
| Ornithopter | {0} | Artifact Creature | 0/2 | `abilities=flying` |
| Scarecrone | {3} | Artifact Creature | 1/2 | `{1}{S(scarecrow\|myBattlefield)}:draw:1`; `{4}{T}:moveTo(myBattlefield) target(artifact[creature]\|mygraveyard)` |
| Nuisance Engine | {3} | Artifact | - | `{2}{T}:token(Pest,artifact creature pest, 0/1, artifact)` |
| Mox Jet | {0} | Artifact | - | `{T}:add{B}` |
| Mox Sapphire | {0} | Artifact | - | `{T}:add{U}` |
| Akroma's Memorial | {7} | Legendary Artifact | - | seven `lord(creature\|myBattlefield)` lines: flying, first strike, vigilance, trample, haste, protection from red, protection from black |
| Vedalken Archmage | {2}{U}{U} | **Creature** (not artifact) | 0/2 | `@movedTo(artifact\|mystack):draw:1 controller` |
| Ancient Den | - | Artifact Land | - | `{T}:Add{W}` |
| Great Furnace | - | Artifact Land | - | `{T}:Add{R}` |
| Seat of the Synod | - | Artifact Land | - | `{T}:Add{U}` |
| Tree of Tales | - | Artifact Land | - | `{T}:Add{G}` |
| Vault of Whispers | - | Artifact Land | - | `{T}:Add{B}` |
| Tolarian Academy | - | Legendary Land | - | `{T}: foreach(artifact\|myBattlefield) add{U}` |
| Academy Ruins | - | Legendary Land | - | `{T}:Add{1}`; `{1}{U}{T}:moveTo(mylibrary) target(artifact\|mygraveyard)` |

Composition: 60 cards = 35 nonland artifacts + 13 artifact lands (**48 artifacts**) + 2 Vedalken
Archmage + 6 Island + 2 Swamp + Academy Ruins + Tolarian Academy.
