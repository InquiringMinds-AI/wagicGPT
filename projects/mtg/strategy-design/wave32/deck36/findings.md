# deck36 (Master of Ether — UB artifacts) — wave-32 FIRST READ (Step-0 guideless debut)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260727-115847`, binary `7fabd9bd0`, -T 3000.
Seat logs: 6 games, 144 decisions + 6 gameend. **Fallbacks 0/144.** Crash signatures 0/6 stderr.
Record **4/6**. Card facts below are read from `bin/Res/sets/primitives/mtg.txt` (`/usr/bin/grep`),
never from memory; render numbers are quoted from prompts.

---

## 0. HEADLINE

deck36 is the corpus's **cleanest-killing** deck: **4 wins, 4 CLEAN KILLS, zero adjudication-ahead
wins**, at turns 7 / 8 / 11 / 11, three of them at 20 or 18 life. Its two non-wins are a
**cap adjudication at 20-20 that it lost to an invisible clock (poison)** and one genuine death.
The seams the guide would normally legislate (attackers, blockers, mulligan) are already near
perfect and are entered in the PRESERVE LEDGER below. The whole leak surface is on the **casting**
seam, and it is one shape: **the pilot cannot see that a card costs {0}, and invents a rule to
explain the missing cost.**

---

## 1. PRESERVE LEDGER (HL8 — enumerated BEFORE any rule was written)

| Seam | Windows | Result |
|---|---|---|
| Attacker declaration | 21 | Every creature with power > 0 declared. Every non-declared body carried `[deals 0 - this attack deals no damage to the opponent]`. **20/21 windows perfect**; the sole exception (deck105 s19) declined a lone Silver Myr (1/1) into three untapped infect blockers — defensible. |
| Blocking | 14 | **14/14 defensible.** Steel Wall (0/4 defender) and Yotian Soldier (1/4 vigilance) used as walls correctly (Yotian walled Silverquill Silencer three turns running, deck146 s17/s21/s25); the four `no blockers` calls each had only a 0-power chump available. |
| Mulligan / London bottom | 2 mulligans in 6 games, 2 bottoms | Both bottoms were **Akroma's Memorial {7}** — the only card in the deck it cannot cast on curve. Correct both times (i7 shape: 1 mull -> exactly 1 bottom, constant labels). |
| Reply integrity | 144 | 0 fallbacks, 0 defers, 0 truncations, 0 `validation_reject_reask`. p50 reply 1,102 chars. |

**Consequence (HL8 inversion (a)):** the initial guide gets a SHORT confirmatory attack line, not a
loud #1-slot floor. **Consequence (HL8 inversion (b)):** the #1 slot goes to the leak that traces to
the losses, even though it is narrow — it is the {0}-cost class, not an archetype posture.

---

## 2. i10 VERDICT — the natural witness seat reports **UNEXERCISED**, corpus-wide

**(a) `{attached:` FORWARD renders and `[attached to: X #N]` REVERSE renders: 0 and 0 — and not
just at this seat.** A sweep of **all 42 translogs / 21 games** for `attached to:`, `{attached:`,
`[equipment]` and `Equip ` returns **zero occurrences at every seat**. deck36 runs no equipment and
no auras (verified against the decklist: no card with an Equipment or Enchantment type), and neither
does any other deck in the wave-32 pool. The wave-31 measurement (112 reverse / 0 forward) came from
**deck148 (mono-W Kor equipment), which rotated OUT in wave-31** — so the pool that was supposed to
validate the fix no longer contains a card that can trigger it.

**Verdict: UNEXERCISED, not clean.** Trigger condition, stated per HL4-2: *an Equipment or Aura
permanent attached to a creature, on either side of the board, at any render window.* Corpus count:
**0**. Deliverable is a PROBE SPEC (notes.md N-36f), not a pass.

**(b) The INSTANCE-HANDLE half of i10 IS exercised here, heavily, and it PASSES.** deck36 is an
identical-name farm (4x Ornithopter, 4x Silver Myr, 4x Nuisance Engine, 4x Seat of the Synod, 4x
Vault of Whispers, 2x Ancient Den, 2x Glaze Fiend, Pest tokens):

- Machine audit over every `Your battlefield:` / `Opponent battlefield:` line in all 144 prompts:
  **0 unhandled duplicates.** Every name appearing 2+ times on one battlefield line carries `#N` on
  every copy; single copies correctly carry no handle.
- Verbatim (deck152 s26 t9): `Ancient Den #1 [artifact]; Mox Jet [artifact]; Mox Sapphire
  [artifact]; Glaze Fiend #1 {1}{b} (2/3) (printed 0/1) [artifact] [flying]; Seat of the Synod
  [artifact]; Swamp; Tolarian Academy; Glaze Fiend #2 {1}{b} (2/3) (printed 0/1) [artifact]
  [flying]; Silver Myr {2} (1/1) [artifact]; Ancient Den #2 [artifact]`
- Handles bind to combat lines: deck158 s18 `A2. Salvage Slasher #1 (1/1)`; deck105 s16
  `A2. Silver Myr #1 (1/1)`.
- Handles reach the model's own output: it wrote `Attack with Glaze Fiend #2`, `Keep Glaze Fiend
  #1`, `Counter added to Silver Myr #2` (narration). Handle-bearing names across the seat:
  Glaze Fiend 33, Ancient Den 14, Seat of the Synod 10, Vault of Whispers 6, Silver Myr 3,
  Salvage Slasher 3, plus opponent Forest 53 / Overgrown Farmland 45 / Spider 18.
- **Cross-zone dup-name (open ledger L2) witnessed once**: with one Master of Etherium on the
  battlefield and a second in hand, the model wrote *"'Master of Etherium... (6/6)' in the
  battlefield section, but the hand section lists 'Master of Etherium'. This is a contradiction in
  the prompt's state description."* Handles are battlefield-scoped by design; the hand line carries
  none, so a 4-of that straddles zones still reads as a contradiction. 1 instance, recoverable.

---

## 3. THE ARTIFACT SURFACE (task (b))

### 3.1 The artifact-count lord renders correctly and tracks LIVE — with one systematic offset

`Master of Etherium` is `mana={2}{U}`, `power=*`, `toughness=*`,
`auto=lord(other creature[artifact]|mybattlefield) 1/1`,
`anyzone=type:artifact:mybattlefield/type:artifact:mybattlefield cdaactive`.

The CDA is evaluated at every render, **including in hand**, and it tracks every artifact that
enters. deck139, one game, consecutive decisions:

| seq | `Artifacts in play: you N` | hand-line render |
|---|---|---|
| s1 | (none yet) | `Master of Etherium {2}{u} (0/0) [artifact creature]` |
| s5 | 1 | `(1/1)` |
| s7 | 2 | `(2/2)` |
| s8 | 3 | `(3/3)` |
| s9 | 4 | `(4/4)` |
| s10 | 5 | `(5/5)` |
| s13 | 6 | `(6/6)` |

Same monotonic tracking at deck116 s2 -> s10 (0/0 -> 5/5). **The magnitude is visible on the line
and the pilot tracked it correctly** — it verbalized the artifact count as the reason to cast in
essentially every plan, and its arithmetic on the count was right in the two games it held the card.

**SYSTEMATIC +1 OFFSET at resolution (2/2 casts, both verified).** The cast option prices Master
against the artifacts *already* on the battlefield; Master is itself an artifact, so it lands one
bigger:

- deck139 s14: `Cast Master of Etherium {2}{u} (6/6) {card text: "..."}` at `you 6`
  -> s15 battlefield `Master of Etherium {2}{u} (7/7) (printed 0/0) [artifact]` at `you 7`.
- deck116 s10: `Cast Master of Etherium {2}{u} (5/5)` at `you 5`
  -> s11 battlefield `(6/6) (printed 0/0)` at `you 6`.

This is the Gray Merchant `{drains N}` shape (Step 5's *prefer the engine-surfaced number — AND
VERIFY ITS OFFSET*). The pilot's own estimates drifted in **both** directions off the un-annotated
number (deck116 s10: *"put a 4/4 (or larger due to artifact count) threat on the board"* while the
line said (5/5) and the body arrived (6/6); deck146 s24: *"Master will be a massive 9/9"* at
`you 10`, i.e. an 11/11). Guide carries the +1 as a STOPGAP with a removal condition; the durable
fix is notes.md **N-36b** (build the cast-line CDA against the post-resolution battlefield, exactly
as the GM `{drains N}` fix did).

**The `Artifacts in play: you N | opponent N` line is present in every prompt from the first
artifact onward and was correct at every window I hand-checked** (deck152 s26: 8 = 2 Ancient Den +
Seat + 2 Moxes + 2 Glaze Fiends + Silver Myr; Swamp and Tolarian Academy correctly excluded).
The deck110 wave-8 read-swap rung applies directly and the guide points at this line.

**The lord's `other artifact creature` scope is correctly narrow, and one deck card falls outside
it:** `Vedalken Archmage` is `type=Creature` (NOT Artifact Creature) — it does not raise the count,
does not get +1/+1, and is the only nonland card in the deck that is not an artifact.

### 3.2 {0}-cost casts — offered cleanly, and **this is the deck's #1 leak**

Mechanically the engine is faultless: `Cast Mox Jet`, `Cast Mox Sapphire`, `Cast Ornithopter` are
offered in every legal window and every take resolves (`hand -> stack` / `stack -> battlefield`
narrated, 0 no-ops). **The defect is the RENDER: a converted cost of 0 suppresses the cost token
entirely, at all three emitters.** In `src/AIPlayerGPT.cpp` the guard is
`if (cost && cost->getConvertedCost())` at the decklist emitter (~line 845), the cast-option emitter
(~line 1658) and the hand/board emitter (~line 4215). **347 renders of the three {0} cards across
this seat; not one carries a `{0}`.** The system-prompt decklist line is likewise bare — the model
quoted it: *"Deck list: '1x Mox Jet: {T}: Add {B}.' It does not show a mana cost."*

The pilot fills the hole by invention, four distinct false rules, all verbatim:

1. deck116 s3 t0 -> `Cast nothing right now`: *"Mox Jet and Mox Sapphire are artifacts that provide
   mana when tapped; **they are not spells you can cast from your hand.**"*
2. deck116 s4 t0 -> `Cast nothing right now`: *"you do not 'cast' artifacts... 'Cast Mox Jet' is
   likely a trick or invalid action... **putting them onto the battlefield requires a land drop
   slot**, and you have already played a land."*
3. deck139 s5 t1 -> `Cast nothing right now`: *"**Moxes are artifacts that I can cast, but they cost
   mana to cast. I have no extra mana to cast them.**"*
4. deck139 s3 t1 -> `Cast nothing right now`: *"Ornithopter/Moxes are low priority compared to
   fixing my mana base."*

**Cost, split by window composition (HL9):** 7 `Cast nothing right now` windows in the corpus.
**7/7 declined a payable permanent; 0/7 were legitimate instant-holds** (the deck contains zero
instants, so the legitimate-hold branch is structurally empty here). Of the 7: **4 declined a free
Mox or Ornithopter**, 1 was the generic-mana veto below, 2 declined a cheap body it then cast in
Main 2. Both Moxes were delayed a full turn in deck116 (declined s3 AND s4 on turn 0, cast s7/s8 on
turn 2). And the tax: the two largest cast-seam replies of the whole seat are **deck152 s3 (8,600
chars / 133 s) and s4 (7,040 / 107 s) — both spent on whether a Mox is free.** A land drop cannot be
hard and neither can a free spell; per HL9's low-branching rule these are the seat's best detector.
(Corroborating: 4 of the top 6 tax spikes overall are LAND DROPS — deck105 s21 14,316 chars /
216 s on `Play no land right now`, deck152 s25 12,738 / 215 s, deck105 s14 10,087, deck105 s18 8,695.)

### 3.3 The sibling belief: **coloured mana cannot pay a generic cost**

deck152 s4/s5, verbatim, twice in consecutive replies:

> *"If I have {W} and {B}, I cannot pay {1}."* ... *"Is there a mistake in my understanding of
> costs? 'Mana costs are written like {2}{R} (two generic plus one red).' '{1}' is one generic. If I
> have {W} and {B}, I cannot pay {1}."*

Cost: **deck152 s5 t1, `Mana available: 3 total (... colours you can make: {u}{b}{w})`, hand holding
Steel Wall {1}, Silver Myr {2} and Glaze Fiend {1}{B} — all three OFFERED, all three payable —
answered `Cast nothing right now`.** It recovered in Main 2 (s6/s7), so the game cost was one combat
step, not the game; but this is the deck's structural exposure: **35 of its 48 artifacts and 15 of
its 27 nonland card slots are colourless-generic costs** ({0} Ornithopter/Moxes, {1} Steel Wall, {2}
Silver Myr, {3} Cathodion / Nuisance Engine / Yotian Soldier / Coiled Tinviper / Scarecrone, {7}
Akroma's Memorial) paid off a five-colour artifact-land base (W Ancient Den, R Great Furnace, G Tree
of Tales, U Seat of the Synod, B Vault of Whispers). A pilot that thinks its {W} and {R} cannot pay
generic has vetoed its own curve. It is the same belief-that-VETOES face as deck17 / deck59 (Step
0-bis rule 3), and it oscillates — the SAME model got it right at deck152 s3 (*"{W} counts as
generic? Yes"*) and wrong at s4.

### 3.4 Artifact LANDS — coherent as artifacts, opaque as lands

In hand: `Vault of Whispers (land: taps for {B})`, `Seat of the Synod (land: taps for {U})` — full
land annotation, no `[artifact]` tag. On the battlefield: `Ancient Den #1 [artifact]`,
`Vault of Whispers [artifact]` — full artifact tag, **no land marker and no colour**. They are
counted in `Artifacts in play: you N` (correct) and excluded from the creature count (correct).

So the two type halves are each rendered somewhere, but **never on the same line**, and the
battlefield form is byte-identical in shape to a Mox (`Mox Jet [artifact]`). The pilot never
misplayed a land drop off this, and never miscounted the artifact total — the count line carries
that — so I am recording it as a coherence gap, not a demonstrated leak (notes.md N-36d). The one
thing it definitely costs is that the guide must supply the fact the render will not: *every land
you play except Island / Swamp / Academy Ruins / Tolarian Academy raises `Artifacts in play`.*

### 3.5 Three more artifact-surface behaviours, all CORRECT

- **Cathodion `auto=_DIES_Add{3}` fires and the mana is spendable.** deck152 s15, one decision after
  Fateful Absence killed it: `Mana available: 3 total (...) | Already in pool: {c}{c}{c}`, and the
  pilot spent it on the Clue. Clean.
- **Glaze Fiend's `@movedTo(other artifact|myBattlefield):2/2 ueot` fires per artifact and the
  battlefield line shows the running total against printed:** `Glaze Fiend {1}{b} (2/3) (printed
  0/1)` after one artifact, `(4/5) (printed 0/1)` after two (deck152 s15 -> s17). The pilot read
  these correctly every time and attacked into them (deck152 s10 t3 for 4 damage off a printed 0/1).
- **Salvage Slasher `auto=foreach(artifact|mygraveyard) 1/0`**: `(2/1) (printed 1/1)` at one
  artifact in graveyard, `(3/1)` at two (deck158 s19 -> s21). Correct.

---

## 4. STEP-1 STEADY STATE (task (d))

| Item | Verdict at deck36 | Evidence |
|---|---|---|
| **i13** card text on Cast lines | **PASS, 84/84** | Every real `N. Cast ...` line carries `{card text: "..."}`. The 44 non-carrying lines are all `Cast nothing right now`. No length degradation observed: p50 reply 1,102 chars, fallbacks 0/144 — this deck's card texts are one-liners, so its share of the +0.4-1.1k/ask cost is at the low end. |
| **i11** creature markers on hand lines | **PASS, 164/164** | Every hand entry with a `(P/T)` carries `[artifact creature]` — Steel Wall, Glaze Fiend, Silver Myr, Master of Etherium, Yotian Soldier, Ornithopter, Cathodion. **0 unmarked.** 0 "zero creatures" mulligans. |
| **i8** combat tags | **PASS** | `[tapped - attacking]` x25, `[summoning sick - cannot attack this turn, but CAN block]` x112. **Zero paradox verbalizations**: a regex sweep of all 144 replies for contradiction language returns 5 hits and NOT ONE is about a combat tag. |
| **i12** count-first mana line | Present in all 144 prompts. **One caveat** — it counts SOURCES, so Tolarian Academy (which makes {U} per artifact, 8-10 at the time) reads as `1` of `7 total` (deck152 s26). No "I cannot afford" decline of an offered cast was traced to the COUNT; the deck152 s5 decline is the generic-payability belief (3.3), a different mechanism from N-158g. |
| **i9** zero-power attacker annotation | **Load-bearing here.** `[deals 0 - this attack deals no damage to the opponent]` is the single reason the attacker seam scores 20/21 — every excluded body carries it and the pilot cited it. Same string on the block side: `[deals 0 - this block kills nothing, it only absorbs damage]`. |
| **i5 / i6** re-ask + truncation salvage | **0 markers.** Consistent with the corpus (0 validation rejects pool-wide). |
| **i16** AIStats FATAL | Clean: 0 crash signatures across the 6 stderr files. |

**Parser contradictions, verbatim (5 total, none combat-tag):** three are the same class —
*"the prompt says 'Phase: Main phase 1 | It is the opponent's turn.' **This is a direct
contradiction.** Let's assume the game is actually in **my** Main Phase 1"* (deck152 s15/s16) — the
pilot does not accept holding priority in the opponent's main phase. 2 opponent-turn priority
windows at this seat, 1 confused. Low volume here, cross-deck shape -> general-suggestions.md.
One is the cross-zone dup-name (section 2). One is a mana-line-vs-tapped-board read that
self-resolved.

---

## 5. 4/6 DECOMPOSITION (task (c))

**Pool adjudication rate this corpus: 1 / 21 (campaign low).** So the record is almost pure
kill-count and the HL7 decomposition is unusually clean:

**Wins — 4, all CLEAN KILLS, none adjudicated:**

- **vs139, T7, opp -6, own life 20 (untouched).** The engine line executed end to end: Mox, Mox,
  Ornithopter, Glaze Fiend, Salvage Slasher, then `Cast Master of Etherium {2}{u} (6/6)` at s14 ->
  arrives 7/7 -> s15 attack with 3 -> s17 attack with 4 for the kill. **19 decisions, whole game.**
- **vs116, T8, opp -8, own life 20 (untouched).** Same shape: Master cast s10 at (5/5) -> 6/6, then
  two identical full swings (s13, s15) `Silver Myr, Master of Etherium, Cathodion`. **17 decisions.**
- **vs152, T11, opp -8, own life 18.** No Master drawn. Won on Glaze Fiend beats alone — the
  battlefield line's live `(4/5) (printed 0/1)` did the work, plus a Nuisance Engine Pest for the
  count at s31 before the lethal swing.
- **vs146, T11, opp 0, own life 13.** No Master drawn. Won on Glaze Fiend + Salvage Slasher +
  Yotian Soldier while Yotian walled the only real attacker three turns running.

**Both games in which Master of Etherium was drawn were won by turn 8 with zero damage taken.**
That is the deck's ceiling signature and it is high.

**Loss 1 — vs105 (BG infect), the corpus's ONLY cap adjudication, lost at 20-20 on turn 12.
Mechanism: the clock was invisible.** `poison` does not appear anywhere in the engine's board
render (`/usr/bin/grep -i poison src/AIPlayerGPT.cpp` -> the only hit is an unrelated comment), so
neither player's poison total is ever shown. Worse, the blockers header computes life arithmetic
that is **false for infect damage**, and did so in **5 of 5 blocker windows** in that game:

> `Your life: 20. Unblocked, these attackers deal up to 5 - you would be at 15 - NOT lethal: block
> only where the trade favors you; taking damage while ahead is often correct.`

Life never moved from 20 all game. The pilot obeyed the surface it was given, verbatim at s25 t12
facing four infect attackers: *"Since the total damage (5) is not lethal (I would drop to 15), and
blocking with the Pest achieves nothing... I declare no blocks."* Per perception-vs-strategy this is
a **PERCEPTION** failure with a fabricated surface behind it — guide prose against a rendered number
loses, so **no guide line is written for it** and the whole item routes to notes.md (**N-36a**,
HIGH). Note the corpus-level consequence for synthesis: deck105's 6/6 guideless perfecta was played
against opponents that could not see its clock and were told its attacks were survivable.

**Loss 2 — vs158 (RB amass), death at -5 on turn 10.** Decomposed:
- **Draw/construction, dominant.** It mulliganed to 6, bottomed one **Akroma's Memorial {7}**, and
  drew the second — which sat in hand at s19 and s21 with `Mana available: 5` and was never
  castable. A 7-drop in a deck whose kills happen on turn 7-8 is a dead card; 2 copies.
- **Decision, secondary and real.** s21 t9 at **10 life vs 15**, with the opponent's Orc army (7/7)
  and Mauhur tapped but **Snarling Warg (4/4, menace) untapped**, it declared
  `ATTACK: Cathodion, Salvage Slasher` — its only two bodies with power — leaving `Ornithopter
  (0/2)` as its sole untapped blocker against a board that swung back for 13. This is the wave-31
  HL3 shape exactly: the tap cost of attacking is re-derived locally and lost.
- The block at s19 was **correct** (Steel Wall absorbing the 7/7, Salvage Slasher trading with
  Mauhur) — the seat is not sloppy at the blocker seam; it is sloppy about what attacking COSTS.

**Kill count is the headline number: 4 kills / 6 games, 0 adjudication-ahead wins, in a pool with a
1/21 adjudication rate.**

**CASTABLE-BUT-NOT-OFFERED audit, both losses (DoD line item).** Signature cards checked in every
`Casting decision` ask where they sat in `Your hand`: **vs105** — Master of Etherium was in hand from
s4 onward, and the seat's Casting-decision windows in that game (s5, s7, s11, s15, s22) each occurred
at `Mana available:` 1-3 with Master at {2}{U}; the only window at 3+ sources with {U} available
(s22, `you 5`) offered `Cast Nuisance Engine {3}` and Master was correctly absent because the seat's
only blue sources were tapped by the Nuisance Engine plan. **vs158** — Akroma's Memorial {7} in hand
at s17/s20 with `Mana available:` 4 then 5: genuinely unpayable, correctly not offered. **No silently
absent payable option found in either loss.**

---

## 6. SECOND LEAK — Tolarian Academy burned in the UPKEEP window (6 of 8 activations)

`Tolarian Academy` is `type=Legendary Land`,
`auto={T}:name(add blue mana for each artifact) foreach(artifact|myBattlefield) add{U}` — with 8-10
artifacts out, one tap is 8-10 blue mana. The engine offers it as a standalone priority action.

The pilot took it **in its own Upkeep on turns 3, 5, 7, 9 and 11 of the deck146 game** (s8, s13,
s18, s22, s26) plus deck152 s28 — 6 of 8 total activations. Mana in the upkeep step empties before
Main 1, and the Academy then reads `Tolarian Academy [tapped]` for the rest of the turn (visible at
deck146 s14/s19/s23/s27).

**Measured cost this corpus: zero.** At every affected main phase the hand held only lands
(deck146 s19, s23, s27: `Your hand: Island (land: taps for {U}); Vault of Whispers (land: taps for
{B})`), so no cast was actually blocked. I am **not** claiming this lost a game (HL6). It is a
per-prompt-window discipline gap with a live worst case — the same game reached `Artifacts in play:
you 10`, i.e. a ten-mana Academy tap thrown away five turns running — and it gets the Step-5
repeatable-activation treatment in the guide (name each window and its answer), not a prohibition.

Related but distinct, for notes.md (**N-36c**): the engine offers a bare mana ability as a priority
option with nothing to spend it on, which is the `Cast Card Normally with Mox Opal` defusal class
re-appearing on a different card.

---

## 7. PLAN-FIELD POLLUTION (contributes to the tax spiral)

**51 of 132 prompts (39%) carry a `YOUR PLAN` block longer than 400 characters**, several running
600-800. The plan splitter anchors on an in-reasoning quotation: at deck152 s3 the model wrote
*"But wait, the prompt says \"Your Plan: Play Ancient Den. Use Mox Jet and Mox Sapphire...\""* inside
its reasoning, and the s4 prompt's `YOUR PLAN (as you last stated it):` field is that quote **plus
the next twenty lines of deliberation** — including the whole "Moxes have no cost listed / I cannot
pay {1}" derivation. The s5 prompt then carries s4's confusion forward the same way. The pilot's own
unresolved reasoning is being fed back to it as authoritative plan text, which is precisely the fuel
for the belief-rehearsal spiral in 3.2/3.3. Routes to notes.md **N-36e** (parser).

---

## 8. CARD-FACT AUDIT (draft-time rail, DoD line item)

Every card fact written into `strategy.txt` was read from `bin/Res/sets/primitives/mtg.txt` this
session. Full table in `notes.md` appendix A. Persistence check (HL2): **no card in this deck
self-bounces, self-sacrifices or is conditionally returned on ETB** — every creature the guide names
as a body stays on the battlefield. Two facts the guide states because the render will not:
`Vedalken Archmage` is `type=Creature`, not an artifact; `Steel Wall` has `abilities=defender` and
therefore never appears on an attacker list. No frequency or rarity claim appears in the guide
(HL6): the deck's unexercised surfaces — Akroma's Memorial resolving, Scarecrone's reanimation,
Academy Ruins' recursion, Coiled Tinviper, Vedalken Archmage — are given their rule and both
outcomes, with no claim about how often they occur.

**Guide status: this is a HYPOTHESIS, not a freeze.** Step 0-bis contract: validate next corpus.
Falsifiable predictions for the wave-33 reviewer are listed at the end of notes.md.
