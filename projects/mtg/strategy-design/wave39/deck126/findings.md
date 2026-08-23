# deck126 "Sanguine Blood" — Step-0 review (GUIDELESS first pass), wave 39

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-040744/`
Seat logs: `*-ai_baka_deck126-0x*.jsonl` (6 games, one per opponent: 125 / 162 / 36 / 152 / 146 / 139)
Pilot: `qwen36-35b-a3b` (214/214 records), native thinking on, answer-first protocol.
Decisions: **202** (ask 146, priority 40, blockers 8, reveal 5, attackers 3) + 6 gamestart/gameend.
**Fallbacks: 0 / 202.** Zero `unparsed_reply`, zero `empty_reply`, zero `retracted_choice`, zero
defers, `reasoning_degenerate` 0.00 on every record. The parse/protocol stack is clean at this seat.
Record 3-3 (context only; per the owner ruling the record is not the verdict).

Reasoning tax: median **6,984 chars/decision**, total 1.43 M chars; latency up to 288 s
(deck36 seq17). Land drops and one-option upkeep windows are among the largest replies - the
low-branching diagnostic (skill rule 2 refinement) applies and is used below.

---

## 0. PRESERVE LEDGER (unconditional at every Step-0 seat)

Behaviours the guideless pilot already performs correctly. These are FROZEN; the guide below does
not legislate over them, and every one is a reason NOT to write a loud procedure.

| seam | measured | verdict |
|---|---|---|
| Tutor target selection | **4/4 correct** picks off 39-61-option reveal menus (vs125 s31 Sanguine Bond, vs125 s67 Exquisite Blood **completing the pair already on the battlefield**, vs139 s31 Sanguine Bond with Exquisite Blood already out, vs152 s17 either-is-fine) | FROZEN - no target table needed |
| Combo recognition in PLAN prose | the pilot names the loop unprompted with no guide: vs36 s17 *"starting the Sanguine Bond/Exquisite Blood infinite loop to drain the opponent to zero"*; vs139 s4, vs152 s10, vs125 s51 all name assembly | FROZEN - do not "teach the combo exists" |
| Land drops | 51/53 taken | FROZEN |
| Mulligan | **6/6 keeps, 0 mulligans, 0 spirals** | FROZEN - guide carries a FLOOR only, no keep procedure |
| Declining Tribute to Hunger with zero opponent creatures | vs146 s14/s15/s16 - three consecutive correct declines while the option was listed | FROZEN (see F2 for the belief that is nevertheless wrong) |
| Staff of Nin operation | 12/12 upkeep pings once deployed (vs125 s19-s72), always at the opponent's face - and the s72 ping is what **ignited the winning loop** | FROZEN |
| Reply protocol | 0 fallbacks in 202 decisions; every `CHOICE:` line named an option present in the list | FROZEN |

Consequence, per the Step-0 rule that a bad half-record does not license legislating over working
seams: the #1 slot below is NOT an archetype posture and NOT a combo-assembly procedure. It is the
one mechanically perfect leak the corpus contains.

---

## 1. ABSENCE TABLE (OFFERED / TAKEN / ARRIVED - the three-column form)

Per game, for the pieces the deck's own plan requires:

| game | Exquisite Blood on BF | Sanguine Bond on BF | **BOTH at once** | Sorin | Staff of Nin | outcome |
|---|---|---|---|---|---|---|
| vs36 | t11 (s17) | t9 (s13) | **YES, t11** | - | - | WIN t12, 21-0 |
| vs139 | t9 (s15) | t21 (s36) | **YES, t21** | t15 (s22) | - | WIN t21, 71-0 |
| vs125 | t30 (s68; first copy countered t26) | t17 (s33) | **YES, t30** | - | t11 (s18) | WIN t31, 44-0 |
| vs162 | **never - never drawn** | t10 (s17) | no | - | - | LOSS t13, 0-16 |
| vs152 | **never - never drawn** | t11 (s22, from exile) | no | - | - | LOSS t12, 0-35 |
| vs146 | **never - never drawn** | t25 (s28) | no | - | - | LOSS t26, -3-21 |

**The column that decides this deck is "BOTH at once": 3/3 -> win, 3/3 -> loss.** Every one of the
three wins was the loop firing, and no game was decided any other way. There is no second win path
in the observed corpus (0 damage-race wins, 0 attacks that mattered outside the loop).

Zeros with their routing (three causes, three layers):
- **Sorin, Lord of Innistrad: offered 1, taken 1, arrived 1** - one game only. Not a deploy-floor
  failure; a draw fact. Guide carries the card with the UNEXERCISED-CARD form (rule + what to do
  when present + explicit no-op on absence), no frequency claim.
- **Staff of Nin: offered 2, taken 1, arrived 1.** The one decline is vs125 s18's sibling window
  (it cast the Staff that turn); no leak.
- **Exquisite Blood absent in all three losses.** Routing: **draw/construction**, not play. In
  vs162 the pilot never saw an Exquisite Blood OR an Idyllic Tutor in 26 decisions; in vs152 it
  drew its only Idyllic Tutor on t9 and cast it the same turn; in vs146 the only Idyllic Tutor
  arrived on t25 at 2 life. There is no window in the corpus where a tutor or an enchantment was
  castable and declined while the pair was incomplete. **Assembly was not misplayed; it was
  undrawn.** This is why the #1 slot is taken from the leak measured across the WINS.
- **Attackers: 3 windows in 6 games, all in vs139.** All 16 of the deck's creatures are
  Defenders, so the engine's attacker list is empty almost always. Not a passivity finding.
  One real miss inside it: F4 below.

---

## 2. FAILURE CLASSES, ranked, with seq repros

### F1 - THE UPKEEP BATTLEMENT TAP. 22 activations, 22 in UPKEEP, 21 with the mana gone by the next decision. (#1)

Every single manual activation of Overgrown Battlement in the entire corpus happened at the
pilot's own Upkeep, in a window whose ONLY listed action was
`1. Add 1 green mana with Overgrown Battlement [cost: Tap]` (with `CHOICE: 0 (pass)` available and
documented in the same prompt). Distribution: deck139 x9, deck162 x5, deck36 x4, deck152 x2,
deck146 x2.

Cost 1 - the mana evaporates. For 21 of the 22, the very next decision record (Main phase 1)
carries no `Already in pool:` line at all. The one exception is a mid-chain record
(vs162 s20 -> s21, `Already in pool: {g}{g}{g}{g}`), and the following Main-phase record
(vs162 s22) again shows **no pool** - so even the four-mana float was lost at the phase change.
The deck's only ramp, spent 22 times, converted into a cast **zero** times.

Cost 2 - the blocker is gone for the opponent's whole next turn. **5 of the 8 blocker windows in
the corpus opened with an Overgrown Battlement on the battlefield reading
`[tapped - cannot attack or block this turn]`**, each traceable to that turn-cycle's upkeep
activation: vs162 s14, vs162 s19, vs36 s18, vs152 s13, vs139 s24, vs139 s34 (6 board states across
5 windows; deck162 s19 had a second, untapped copy). The sharpest instance is **vs36 s18** - the
game-winning block at 1 life - where the blocker list contained exactly ONE entry
(`B1. Perimeter Captain`) because the Battlement had been tapped in that turn's upkeep. At
vs139 s24 and s34 Perimeter Captain was on the battlefield, so each missing wall-block was a
forgone 2 life, and with Sanguine Bond deployed that is 2 damage as well.

Cost 3 - it is the corpus's cheapest reasoning tax. The upkeep window has ONE option and the
pilot spent 9,739 chars on it at vs139 s9 and 5-8 k routinely.

Zero upside was observed. The `Mana available:` line already lists the Battlement as a source
(`Overgrown Battlement {g} (VARIABLE output: ...)`, vs162 s20/s21), so the engine taps it for a
cast without any manual activation. **Guide lane, #1 slot, written per PROMPT WINDOW.**

### F2 - TRIBUTE TO HUNGER READ AS TARGETED REMOVAL (card-text fabrication). 3 named repros; drives 3 of the 7 largest replies.

Engine `auto=` gives the sacrifice ability to the OPPONENT
(`ability$!name(sacrifice) notaTarget(creature|mybattlefield) ... !$ targetedplayer`) and Scryfall
Oracle reads *"Target opponent sacrifices a creature **of their choice**."* The pilot believes it
picks the victim, in three separate games:

- **vs36 s10** (13,336 chars, the corpus's largest reply): *"Sacrificing Master of Etherium
  (toughness 8) is huge! ... I gain 8 life. I go from 5 to 13 ... This is definitely the right
  move."* Actual outcome at s11: life **5 -> 8** (+3), Master of Etherium still on the battlefield
  at s13. The pilot's own life plan was wrong by 5 and its removal plan did not happen.
- **vs146 s22**: `PLAN: Cast Tribute to Hunger on Triumphant Adventurer to remove it and gain 1
  life.` Opponent board at that moment: Spirit Cai, Triumphant Adventurer, Goblin.
- **vs162 s22**: `PLAN: ... cast Tribute to Hunger targeting Fate Unraveler #2 to remove the
  life-loss engine and gain 4 life.`

The render is not at fault: the option line prints
`Cast Tribute to Hunger {2}{b} - legal targets right now: the opponent {card text: "Target opponent
sacrifices a creature. You gain life equal to that creature's toughness."}`. The engine's `text=`
omits Oracle's *"of their choice"*, which is the one word that would settle it - see E3. This is a
DECK-FACT gap (the fact is on the line and misread as a targeting choice), so it is guide lane,
written as a positive rule plus one quote-and-forbid falsity fact.

Second half of the same card: with the combo assembled, Tribute to Hunger is not removal at all -
it is a WIN BUTTON (any life gain wins). The pilot never framed it that way. Two keyed triggers.

### F3 - UNDER-BLOCKING: 20 blockers offered across 8 windows, 14 assigned.

Idle blockers by window: vs162 s14 (1 of 2), vs162 s19 (1 of 3), vs152 s8 (1 of 3),
vs139 s34 (3 of 4). The generic hint on those prompts reads *"block only where the trade favors
you; taking damage while ahead on LIFE is often correct (your strategy guide's blocking rules
override this general hint)"* - correct in general, anti-teaching for a deck whose 0-power walls
never kill anything and whose entire clock is the life they gain. The clean repro is
**vs162 s19**: life 18, **Sanguine Bond already on the battlefield**, three blockers offered, one
assigned; the reply reasons *"accepting 5 from Master of the Feast"*. Pride Guardian blocking
there is 3 life gained = 3 life drained off a 20-life opponent that the pilot never got below 16.
The engine confirms the gains land at DECLARE-BLOCKERS, before damage (vs139 s24 log: seven
`You gained N life` lines, then `Phase: Combat damage`).

Counter-evidence that the seam is not simply broken: **vs139 s24 is the corpus's best decision** -
five blockers offered, five assigned, all onto one attacker, **+17 life in one window** (2+2+3+2+3+2+3,
i.e. three Pride Guardians at 3 each plus Perimeter Captain's 2 per blocking defender). So this is
a floor problem, not a capability problem. Guide lane: a positive per-item floor with ONE closed
exception, and the generic hint overridden by name.

### F4 - LIFELINK ATTACKERS DECLINED WHILE EXQUISITE BLOOD WAS OUT. 1/3 attacker windows.

**vs139 s28**: `ATTACK: none` with `A1. Vampire #1 (1/1) [lifelink]` offered, Exquisite Blood on
the battlefield, opponent at 18. A lifelink connection is life gain; with Sanguine Bond it is the
loop. The pilot corrected itself unprompted at s33 and s38 (`ATTACK: A1, A2` both times), and s38
is the attack that ignited the winning loop. One-line floor, not a section.

### F5 - CHROMATIC LANTERN CAST INTO A MANABASE THAT NEEDS NO FIXING.

Offered 21 windows, taken 5. In **vs146 the pilot cast TWO** (s10 t9, s13 t11) - six mana - while
its `colours you can make:` line already read `{g}{u}{r}{b}{w}` off the first one, and both were
then exiled by Vanishing Verse. The deck's lands are true duals (Bayou = Swamp Forest,
Scrubland = Plains Swamp, Savannah = Forest Plains, verified in `primitives/`) plus Urborg, which
makes every land a Swamp; the printed `colours you can make:` line answers the fixing question
exactly. Low-cost item; handled as a ranking entry in the deploy order, never a prohibition.

### F6 - PROTOCOL TAX ON THE PLAN LINE (core/protocol lane, NOT guide).

At **vs152 s10** - a three-option LAND DROP that took 11,603 chars and 236 s - roughly half the
reasoning is the model litigating rule (a)/(b)/(c) of the PLAN-line test against itself, reversing
four times (*"I'll write one to be safe" / "I'll stick to (c) and omit it" / "Actually, writing it
doesn't hurt"*), and the same oscillation closes vs36 s10. Routes to core/reply-protocol, not to
this guide. Recorded here because it is a large share of this seat's 1.43 M reasoning chars.

---

## 3. COMBO-ASSEMBLY ANALYSIS - did the pilot assemble deliberately, and does the ENGINE implement the loop?

**The engine implements it, faithfully, and terminates at opponent 0 = win.** Three independent
firings, each with the arithmetic checked against the life deltas:

| game | igniter | my life | opp life | consistency |
|---|---|---|---|---|
| vs36 s18->s19 | Perimeter Captain trigger on a declared block (+2) | 1 -> **21** | 20 -> **0** | gain 20 = loss 20; loop ran 10 cycles of 2 |
| vs139 s38->s39 | lifelink Vampire connects (+1) | 43 -> **71** (+28) | 27 -> **0** (-27) | seed gain 1 then 27 gain/drain cycles |
| vs125 s72->s73 | `Deal 1 Damage with Staff of Nin targeting a player` (opponent loses 1) | 18 -> **44** (+26) | 27 -> **0** (-27) | seed is a LOSS not a gain, so gain = loss - 1 |

Both directions are live: Sanguine Bond's `@lifeof(player) ... :life:-thatmuch opponent` fires on
your gain, Exquisite Blood's `@lifelostfoeof(player):life:thatmuch controller` fires on their
loss, and either one alone seeds the other into a loop that the engine runs to completion inside a
single trigger resolution. **No divergence from paper MTG was observed; the guide teaches the
engine's behaviour and the engine's behaviour is the paper behaviour.**

Deliberateness: **assembly was deliberate, not stumbled into.**
- vs36 is the cleanest chain. s13 (t9, at 8 life): `CHOICE: 4 (Cast Sanguine Bond)` /
  `PLAN: Cast Sanguine Bond now to establish the life-drain engine immediately. Use my defenders to
  block and trigger life gain, draining the opponent.` s17 (t11, at **1 life**, choosing between
  Chromatic Lantern, Idyllic Tutor and Exquisite Blood): `CHOICE: 3 (Cast Exquisite Blood)` /
  `PLAN: ... Block next turn's attack with defenders to trigger Perimeter Captain, starting the
  Sanguine Bond/Exquisite Blood infinite loop to drain the opponent to zero.` s18: blocks with its
  only legal blocker and wins. Three consecutive decisions, each naming the loop before making it.
- vs125 is deliberate under counter-pressure: three Idyllic Tutors cast (t7, t17, t29), the
  t7 one countered, the t17 one fetching Sanguine Bond, the t29 one fetching **Exquisite Blood
  specifically because Sanguine Bond was already deployed** (s67), then s68 casts it and the
  s72 upkeep ping wins.
- vs139 is deliberate but SLOW: Exquisite Blood down t9, the completing tutor not cast until t19.
  There is no earlier window to blame - the pilot held no Idyllic Tutor before drawing it at t19
  (hand reads `(none)` at s28), and after the tutor it had 4 mana against a 5-cost enchantment.
  The one thing it missed in the interval is F3/F4 value, not assembly.

**No instance of the reverse error was found**: the pilot never cast the piece it already had,
never tutored for a card already on the battlefield, and never declined a castable combo piece.

---

## 4. ENGINE / REPRESENTATION DEFECTS (wave-40 ledger candidates)

**E1 (HIGH, render truth) - a COUNTERED spell narrates as `resolved`.** In the vs125 game the
GAME LOG renders every countered spell as
`Your <card> resolved and went to your graveyard` - byte-identical to a sorcery resolving
normally. Repros in `1787476073-...-vs-deck125.jsonl`: Chromatic Lantern (t6, vs Dream Fracture),
Idyllic Tutor (t8, vs Dream Fracture), Exquisite Blood (t26, vs Dream Fracture), Pride Guardian
(t30, vs Fall of the Gavel) - the last two are a permanent narrated as having "resolved" while
never entering the battlefield. Grep of all `prompt` fields at this seat: **zero occurrences of
"countered" anywhere in the rendered narration** (the four hits in the file are the pilot's own
reasoning text, where it inferred the counters from context). Wave-35 lane B's countered-vs-resolved
distinction is not reaching this path. Decision-relevant for a combo deck: whether the piece was
countered or merely a sorcery finishing is the difference between "play around counters" and
"nothing happened".

**E2 (MEDIUM, magnitude truth - HL7 class) - the Overgrown Battlement activation label hard-codes
"Add 1".** Every one of the 22 option lines reads
`Add 1 green mana with Overgrown Battlement [cost: Tap]` regardless of defender count, while the
card adds {G} per defender. Proof of divergence inside one game: **vs162 s20** takes that
"Add 1" option and **vs162 s21** then prints
`Already in pool: {g}{g}{g}{g} (4 mana ALREADY produced and floating right now)`. Two surfaces in
the same prompt disagree - the `Mana available:` line correctly annotates the Battlement as
`(VARIABLE output: this ONE source adds more than one mana ...)` while the action label asserts 1.
An emitted magnitude asserts; this one is false at any defender count above 1.

**E3 (LOW, card-text fidelity) - `text=` for Tribute to Hunger drops Oracle's "of their choice".**
`primitives/`: *"Target opponent sacrifices a creature. You gain life equal to that creature's
toughness."* Scryfall: *"Target opponent sacrifices a creature **of their choice**. ..."* The
`auto=` is correct (opponent picks); only the rendered text is ambiguous, and F2 shows the pilot
resolving the ambiguity the wrong way in three games at ~12 k chars each. One-word `text=` fix.

**E4 (LOW, card-text fidelity) - `text=` for Perimeter Captain says "you MAY gain 2 life"; the
`auto=` is mandatory** (`@combat(blocking) source(creature[defender]|mybattlefield):life:2
controller`, no `may`), and the corpus confirms the gain always happens with no prompt (vs139 s24
log). Oracle does read "may", so the divergence is `auto=` vs Oracle; but the rendered text and the
rendered behaviour disagree with each other, which is the thing that costs decisions. Either add
the may-prompt or drop "may" from `text=`. **The guide below states only what the engine does and
does not instruct the pilot to disbelieve the line.**

**E5 (INFO, harness) - the blockers echo prints one blocker name per assignment with no
disambiguator.** vs139 s24 narrates `Pride Guardian blocks Forest; Pride Guardian blocks Forest;
Pride Guardian blocks Forest; Perimeter Captain blocks Forest; Vampire blocks Forest` - three
DIFFERENT Pride Guardians rendered identically. The reply itself was legal (`B1:A2, B2:A2, B3:A2,
B4:A2, B5:A2`, five distinct blockers on one attacker) and the engine executed it correctly
(seven separate gain triggers), so this is a legibility item, not a correctness one. The
board/blocker lists already carry `#1/#2/#3`; the echo does not.

**C1 (CONSTRUCTION, roster owner) - deck126 is a 75-card deck with 39 lands.** The file comments
`#24 lands` but lists 4+4+4+4+2+2+1+1+1+**16** = 39; `Forest (SHM) * 16` is almost certainly meant
to be `* 1`. Corroborated in-game: the Idyllic Tutor reveal menus enumerate the whole library at
**61 options on turn 9** (vs152 s17) and 58 on turn 19 (vs139 s31) - a 60-card list would be near
48 and 38. Consequences visible in the corpus: 52 % lands, long stretches of
`Play Forest` / `Cast nothing right now` (vs125 turns 13-29), and a combo whose eight pieces are
diluted across 75 cards, which is the whole content of the three losses' zero column. **Not a
guide item** - flagged upward as an expectation.

---

## 5. PER-CLAIM CARD-FACT LEDGER for `deck126_strategy.txt` (HL2)

Every checkable claim in the shipped guide, its adjudicating source, and its verdict. Oracle =
`https://api.scryfall.com/cards/named?exact=<NAME>` fetched 2026-08-23; engine =
`projects/mtg/bin/Res/sets/primitives/*.txt`; render = a quoted substring grepped against this
corpus's `prompt` fields.

| # | claim as written in the guide | adjudicating source | verdict |
|---|---|---|---|
| 1 | Exquisite Blood: "whenever the opponent loses life you gain that much" | engine `auto=@lifelostfoeof(player):life:thatmuch controller`; Oracle identical | TRUE |
| 2 | Sanguine Bond: "whenever you gain life the opponent loses that much" | engine `auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent`; Oracle identical | TRUE |
| 3 | "Both on the battlefield -> any life you gain or any life they lose runs to 0 and you win" | corpus: three firings, arithmetic reconciled in section 3 | TRUE (engine-observed) |
| 4 | Exquisite Blood costs {4}{B}; Sanguine Bond costs {3}{B}{B} | engine `mana=`; Oracle; render `Cast Exquisite Blood {4}{b}` / `Cast Sanguine Bond {3}{b}{b}` | TRUE |
| 5 | Idyllic Tutor {2}{W} puts the enchantment into your HAND (not play) | engine `transforms((...newability[moveto(hand)]))`; Oracle "put it into your hand"; corpus `Your revealed Sanguine Bond went to your hand` | TRUE |
| 6 | Perimeter Captain {W} 0/4 Defender: a defender you control blocking gains 2 life | engine `auto=@combat(blocking) source(creature[defender]|mybattlefield):life:2 controller`; corpus vs139 s24 gains | TRUE for the engine (Oracle says "may" - E4) |
| 7 | Pride Guardian {W} 0/3 Defender: gains 3 life when **it** blocks | engine `source(this)`; Oracle | TRUE |
| 8 | Wall of Omens {1}{W} 0/4 Defender, draws a card on entry | engine `auto=draw:1 controller`; Oracle | TRUE |
| 9 | Overgrown Battlement {1}{G} 0/4 Defender, taps for {G} per defender | engine `auto={T}:foreach(creature[defender]|myBattlefield) add{G}`; Oracle | TRUE (the option LABEL is wrong - E2; the guide does not quote the label's number) |
| 10 | Tribute to Hunger {2}{B} instant: the **opponent chooses** which creature to sacrifice | engine `ability$!... notaTarget(creature|mybattlefield) sacrifice!$ targetedplayer`; Oracle "of their choice"; corpus vs36 s10->s11 (+3, not the +8 the pilot expected) | TRUE - and the pilot's contrary belief is FALSE |
| 11 | Tribute to Hunger gains life equal to the sacrificed creature's toughness | engine `toughnesslifegain`; Oracle | TRUE |
| 12 | Sorin, Lord of Innistrad {2}{W}{B}, starts at 3 loyalty, +1 makes a 1/1 black Vampire with lifelink | engine `counter(0/0,3,loyalty)` + `token(Vampire,Creature Vampire,1/1,black,lifelink)`; Oracle | TRUE |
| 13 | Staff of Nin {6}: draws a card each of your upkeeps; {T} deals 1 damage to any target | engine `auto=@each my upkeep:draw:1` + `{T}:damage:1 target(anytarget)`; Oracle | TRUE |
| 14 | Chromatic Lantern {3}: your lands each tap for any colour | engine `lord(land|mybattlefield) transforms(...)`; Oracle | TRUE |
| 15 | "Your cheapest spell costs ONE mana" | engine `mana={W}` on Perimeter Captain and Pride Guardian; corpus pregame line `Cheapest spell in this hand: Pride Guardian {w} (mana value 1)` | TRUE |
| 16 | "all 16 of your creatures are Defenders and cannot attack" | engine `abilities=defender` on Perimeter Captain / Pride Guardian / Overgrown Battlement / Wall of Omens = 16 of 16 creatures | TRUE (a draft that said "15 of 16" was corrected before shipping) |
| 17 | Bayou/Scrubland/Savannah are untapped duals; Urborg makes every land a Swamp | engine `subtype=Swamp Forest` / `Plains Swamp` / `Forest Plains`, no `tap(noevent)`; Urborg `lord(land) transforms((swamp))` | TRUE |
| 18 | "You run four Idyllic Tutors and two copies of each enchantment" | `Res/ai/baka/deck126.txt`: `Idyllic Tutor * 4`, `Exquisite Blood * 2`, `Sanguine Bond * 2` | TRUE |
| 19 | "Sixteen walls" / "all sixteen of your creatures are Defenders" | `deck126.txt` `#16 walls`: Perimeter Captain 4 + Pride Guardian 4 + Overgrown Battlement 4 + Wall of Omens 4 = 16, and those are the deck's ONLY creatures | TRUE |

Quoted render strings, grepped verbatim against this corpus's `prompt` fields:
- `Add 1 green mana with Overgrown Battlement` - 22/22 upkeep option lines. PRESENT.
- `CHOICE: 0 (pass)` - printed in the instruction block of every priority ask. PRESENT.
- `Cast Tribute to Hunger {2}{b} - legal targets right now: the opponent` - 54/54. PRESENT.
- `Deal 1 Damage with Staff of Nin targeting a player` - 12/12. PRESENT.
- `+1: create a 1/1 vampire with Sorin, Lord of Innistrad` - 4/4. PRESENT.
- `deals 0 - this block kills nothing, it only absorbs damage` - every blocker line, 20/20. PRESENT.
- `your blocker dies, attacker lives` / `neither dies` - both forms present in the 8 blocker windows. PRESENT.
- `block only where the trade favors you` - present on the non-lethal blocker windows; the same
  line ends `(your strategy guide's blocking rules override this general hint)`, which is the
  sanctioned hook for overriding it by name. PRESENT.
- `colours you can make:` - every mana line. PRESENT.
- `Your hand (7 cards), counted by the engine:` and
  `Playing every land in this hand would cover the cost of:` - 6/6 pregame prompts. PRESENT.
- `having already taken` - **0 occurrences in this corpus (0 mulligans taken).** Verified at the
  SOURCE instead: `AIPlayerGPT.cpp:11756` emits `", and having already taken " << mullsTaken <<
  " mulligan"`. Flagged as source-verified-not-corpus-verified; re-grep next corpus.

---

## 6. LAYER ROUTING

- **PER-DECK (this guide)** - F1 upkeep-pass rule, F2 edict corrective, F3 blocking floor,
  F4 lifelink attack line, F5 deploy ranking, the win-button rule, mulligan floor.
- **ENGINE / REPRESENTATION** - E1 (countered-vs-resolved), E2 (Battlement "Add 1" label),
  E3/E4 (`text=` fidelity), E5 (blocker echo disambiguation).
- **CORE / REPLY-PROTOCOL** - F6, the PLAN-line three-case test consuming half of the largest
  replies. Single-seat evidence; recorded, not proposed as a core edit.
- **ROSTER OWNER** - C1, the 75-card / 39-land list.

## 7. STOPGAPS IN THE SHIPPED GUIDE (line, defect + citation, removal condition)

1. *"The opponent picks which creature dies, not you"* - patches E3 (`text=` omits "of their
   choice") on top of a genuine model belief (F2). **Removal condition: none.** The belief half
   survives any `text=` fix; keep the line even after E3 closes, and re-check it next corpus.
2. *(none written against the Battlement option label)* - the #1 rule tells the pilot to pass that
   window, so the guide never has to quote or contradict E2's "Add 1". If E2 is fixed, nothing in
   the guide changes.

## 8. VALIDATION PREDICTION (falsifiable, for the wave-40 seat review)

If the guide lands, the next deck126 corpus shows: (a) upkeep `Add 1 green mana with Overgrown
Battlement` activations at or near **0/N**, with `CHOICE: 0 (pass)` in those windows; (b) blockers
assigned / blockers offered rising from **14/20**; (c) zero PLAN lines naming a specific opponent
creature as a Tribute to Hunger target; (d) no change to tutor-target accuracy (holds at 4/4) or
mulligan count (holds at 0). If (a) does not move, the rule is not louder - it is in the wrong
window, and the next revision keys it to the phase label rather than the option text.
