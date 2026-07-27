# deck152 wave-31 -- engine / harness ledger (Step-0 seat, never rotates)

Corpus `matchups-20260727-025026`, binary cf7cb2e53. All seq/turn refs are the deck152 SEAT log
`*-ai_baka_deck152-0x*-vs-ai_baka_deckN.jsonl` unless noted.

## CLOSED THIS WAVE

**N-152b CLOSED (h5, display-toggle-only auto-pass).** Priority asks at this seat 148 -> 22;
display-toggle-ONLY windows 121 -> 0. Every surviving priority window carries a real action
(Ranger Class level, Katilda pump, Teferi loyalty, Clue sac, Packleader fight). Windows that
contain a Flip Side ALONGSIDE a real action still ask, correctly (deck148 seq36, deck139 seq28,
deck158 seq12). The wave-30 unparsed fallback born of a toggle-only window has no analogue;
seat fallbacks 1 -> 0.

**N-152a annotation half CLOSED (h4).** New truthful text present on 48 MDFC-land option lines
and in the play-land sub-menu; old misleading text absent from all land lines. Back-face land
PLACEMENT remains blocked as expected (engine-R ledger, carried). Note the ledger item's blast
radius is smaller than assumed: **Teferi is not blocked** -- he was cast and piloted via
Deserted Beach in the deck148 game (seq24 `Play Deserted Beach` -> seq25 `Cast Teferi, Who
Slows the Sunset {2}{u}{w}` -> +1 seq26, -2 seq36, -2 seq42, loyalty 4/5/3/1 coherent). Only
the Pathway back-face colors are unreachable.

---

## N-152c (NEW, FIRST-ORDER) -- strict potentialMana credits ONE mana ability per card, so a two-ability dual land contributes only its FIRST color and multi-pip second-color spells are silently NOT OFFERED

**Repro A -- deck152 vs deck146, seq26, turn 15** (`kind=ask`, Main phase 2, my_life 1):
```
Mana available: {g}{w} from 5 untapped sources (tapped automatically when you cast)
Your hand: Sigarda, Champion of Light {1}{g}{w}{w} (4/4); Fateful Absence {1}{w} [instant];
Teferi, Who Slows the Sunset {2}{u}{w} [planeswalker]; Katilda, Dawnhart Prime {g}{w} (1/1);
Brutal Cathar {2}{w} (2/2); Elite Spellbinder {2}{w} (3/1)
Your battlefield (creatures: 1): Overgrown Farmland #1; Overgrown Farmland #2; Barkchannel
Pathway; Moonrage Brute (3/3) (printed 2/2) [first strike, nightbound] [tapped - cannot attack
or block this turn]; Branchloft Pathway; Night; Hengegate Pathway
Casting decision (Main phase 2, YOUR turn): which card do you cast now, if any?
1. Cast Fateful Absence {1}{w} - legal targets right now: Goblin, Triumphant Adventurer,
   Goblin, Moonrage Brute
2. Cast Katilda, Dawnhart Prime {g}{w} (1/1)
3. Cast Brutal Cathar {2}{w} (2/2)
4. Cast Elite Spellbinder {2}{w} (3/1)
5. Cast nothing right now
```
Sigarda is in hand and payable. `mana={1}{G}{W}{W}` (Res/sets/primitives/borderline.txt). The
five untapped sources: Overgrown Farmland x2 (`auto=if type(other land|mybattlefield)~lessthan~2
then tap(noevent)` / `auto={T}:add{G}` / `auto={T}:add{W}`), Barkchannel Pathway
(`auto={T}:add{G}`), Branchloft Pathway (`auto={T}:add{G}`), Hengegate Pathway
(`auto={T}:add{W}`). Legal payment: {W} Hengegate + {W} Farmland#1 + {G} Barkchannel + {1}
Branchloft. NOT OFFERED.

**Repro B -- deck152 vs deck146, seq21, turn 13** (`kind=ask`, Main phase 1, my_life 3),
`Mana available: {g}{w} from 6 untapped sources`, battlefield Overgrown Farmland x2 +
Barkchannel + Branchloft + Katilda (Human, `lord(creature[human&green]) {T}:add{G}` then
`lord(creature[human&white]) {T}:add{W}`) + Brutal Cathar (white Human). Sigarda in hand,
options are Fateful Absence / Brutal Cathar / Elite Spellbinder / Cast nothing. NOT OFFERED.

**Negative control -- deck152 vs deck137, seq28, turn 13**: only 4 untapped sources
(`Mana available: {g}{u}{w} from 4 untapped sources`), white supplied by **Deserted Beach**
(`auto={T}:add{W}` FIRST, `auto={T}:add{U}` second) plus Hengegate. Sigarda IS offered
(`1. Cast Sigarda, Champion of Light {1}{g}{w}{w}`) and is cast. The difference between the
repro and the control is exactly which mana ability of the dual land comes FIRST.

**Mechanism (read in source, not inferred from behavior alone):**
`ManaEngine::potentialMana`, `projects/mtg/src/ManaEngine.cpp:163-199` --
```
if (!used[card] && producerUsable(p, amp, card, true) && amp->output->getConvertedCost() == 1)
{
    result->add(amp->output);
    used[card] = true;
}
```
The `used[card] = true` after the first credited ability means a card's SECOND `{T}:add{X}`
line is never counted. Overgrown Farmland therefore scores as `{G}` only; Deserted Beach as
`{W}` only.

The codebase already knows this class. `projects/mtg/src/LegalActions.cpp:296-301`:
```
//PERMISSIVE potential: strict potentialMana counts one ability per
//card, so a dual land only ever offered its FIRST color and payable
//responses were invisible - a missed window costs a game, a spurious
//one costs a single auto-answered ask (this predicate's documented
//bias).
ManaCost * pMana = ManaEngine::potentialManaPermissive(p, freePolicy);
```
That fix landed for `hasInstantResponse` ONLY. The cast-offer path is still strict:
- `LegalActions.cpp:348` `castableForDisplay` -> `potentialMana(p, freePolicy, NULL)`
- `DecisionContract.cpp:434` `buildCastSpell` -> `legalCasts(p, policy, pMana, ...)` with the
  caller's strict pMana; `DecisionContract.cpp:461` re-derives strict pMana for the staleness
  gate, so even a correct answer would be re-rejected there.
- `AIPlayerBaka.cpp:2754` `FindCardToPlay` -> `legalCasts(this, policy, pMana, instantWindow)`

**Impact.** Suppresses the deck's best card in its own colors. Both repros are in the game the
seat lost at 1 life while holding Sigarda, and the model's PLAN named her for three consecutive
decisions ("Play Hengegate Pathway, then immediately cast Sigarda"). Not deck-specific: any
deck whose colored pips outnumber its single-color sources hits this. The GPT seat is also
misled twice over, because the "Mana available" COLOR SET is computed on a different path and
DOES see both faces (deck148 renders `{g}{u}{w}` off Deserted Beach's second ability) -- so the
mana line and the cast list contradict each other, and the deployed guide's #1 rule instructs
the pilot to trust the cast list.

**Fix direction.** Build the OFFER set from `potentialManaPermissive` (same bias argument the
`hasInstantResponse` comment already makes) and let `ManaEngine::planPayment` be the authority
at commit time -- it already validates the whole plan and returns empty on failure, and
`DecisionContract.cpp` already refuses a partial payment. An over-offered cast costs one
re-ask; an under-offered one costs the game. The staleness gate at `DecisionContract.cpp:461`
must use the SAME potential as the build, or the two will disagree. Gate with a fixture:
Overgrown Farmland x2 + Barkchannel + Branchloft + Hengegate, Sigarda in hand -> must be
offered AND must resolve. Add the mirror negative (three Forests + Sigarda -> not offered).

---

## N-152d (NEW) -- the "(printed X/Y)" delta annotation reads the FRONT face of a transformed DFC

A flipped daybound/nightbound creature renders its CURRENT P/T correctly but appends a
`(printed X/Y)` that is the OTHER face's printed values, so an unbuffed transformed werewolf
is presented as a pumped small creature.

Repros (all deck152 seat):
- deck146 seq23 t15, seq26 t15: `Moonrage Brute (3/3) (printed 2/2) [first strike, nightbound]`
- deck137 seq27 t13: `Moonrage Brute (3/3) (printed 2/2) [first strike, nightbound]`
- deck139 seq16 t9, seq26 t11: same
- deck148 seq45 t19, seq49 t19: `Tovolar's Packleader (7/7) (printed 6/6) [nightbound]`

Ground truth from `Res/sets/primitives/borderline.txt`: `name=Moonrage Brute ... power=3
toughness=3`; `name=Tovolar's Packleader ... power=7 toughness=7`. At deck146 seq23 the Brute
had no counters and no anthem on board, so the correct render is a bare `(3/3)` with no
printed-delta.

The annotation is correct on genuine buffs in the same prompts -- `Briarbridge Tracker (5/4)
(printed 2/3) [vigilance] [counters: 1x +1/+1]`, `Elite Spellbinder (5/3) (printed 3/1)
[counters: 2x +1/+1]`, opponent `Serra Ascendant (6/6) (printed 1/1)` and `Kor ally (7/1)
(printed 1/1)` -- so this is face selection, not the annotation. Fix: source printed P/T from
the currently-displayed face's model (the same face the name, keywords and abilities are already
read from), not from `card->model` / the front-face record. Suppress the tag when the delta is
zero after that correction. PARSETEST should gain a case: transformed DFC with no counters ->
no `(printed ...)` tail.

No misplay traced to it this corpus, but the inference it invites (an anthem/counter is
inflating the body, so removing it shrinks the creature) is exactly the kind that costs a block.

---

## N-152e (NEW, cosmetic residual of h4) -- the OLD misleading Flip-Side text survives on TRANSFORM DFCs

The h4 rewrite covered MDFC lands. The two transform DFCs in this deck still carry the wave-30
wording, including the promise that the back face is castable:

deck148 seq36 t15, verbatim:
```
3. Flip Side with Tovolar's Huntmaster -> DISPLAY TOGGLE only: switches this hand card to show
its other face "backside". It does NOT cast anything and uses no stack. You usually do NOT need
it: the Cast menu is where you cast, and it lists every face you can afford (the other face
appears there as an alternative-cost cast). This only changes which face is displayed.
```
deck139 seq28 t11 is the same string for Brutal Cathar. Two defects: (a) the back face of a
transform DFC is NEVER castable, so "the other face appears there as an alternative-cost cast"
is false for this card class; (b) the back face renders as the literal placeholder `"backside"`
instead of `Tovolar's Packleader` / `Moonrage Brute`, which the MDFC path resolves correctly
(`its other face is "tidechannel pathway"`). Low priority -- both windows also offered a real
action, and the auto-pass means a toggle-only window never reaches the model -- but the same
one-line rewrite that fixed the land case should cover it, plus resolving the back-face name.

---

## CONFIRMED CLEAN (recorded so a later reviewer does not re-open)

- **Werewolf day/night flip EXERCISED at last** (wave-30's open item) -- night in 4/6 games,
  day-side steady in the others, both directions rendered. No flip-thrash, no oscillation, no
  isFlipped identity desync: name, keywords, abilities and current P/T switch together and stay
  switched. The `Day`/`Night` designation renders as a battlefield pseudo-permanent and is
  correctly excluded from the creature count. Packleader's `{2}{g}{g}` fight ability is offered
  on the night face only (deck148 seq45/49). The ONLY defect is N-152d above.
- **Class level-up** -- `[counters: Nx level]` up to 3x (deck158 seq40), L2 `[cost: {1}{g}]`
  and L3 `[cost: {3}{g}]` match the primitive, two copies level independently with no
  cross-copy counter bleed (deck122 seq48).
- **Katilda's `{4}{g}{w}, Tap` pump** renders with cost and is correctly declinable
  (deck122 seq22/29/38/45, all choice=0).
- **Planeswalker loyalty** renders and decrements coherently (Teferi 4 -> 5 -> 3 -> 1 across
  deck148 seq26/36/42, board shows `[counters: 1x loyalty]` at seq45).
- **Blockers**: summoning-sick creatures correctly offered as blockers with restriction-first
  wording; the naive-trade annotations read correctly. One first-line protocol violation
  (deck148 seq17 `BLOCKS: B2:A1, B2:A2`) was absorbed by the first-wins parser to the same
  block the model's own corrected reasoning chose -- working as designed, not routed.
- **No parser contradictions.** An apparent narration/prompt name mismatch at deck148 seq18
  (`Intrepid Adversary blocks Stone Haven Outfitter` vs a seq17 attacker list of `Stone Haven
  Pilgrim #1/#2`) is a red herring: the Outfitter block is an earlier combat in the append-only
  narration, and deck148's own log seq14 t10 confirms both seq17 attackers were Pilgrims.
- **Zero fallbacks, zero crash signatures** at this seat (243 decisions).

## CARRIED

- N-152a MDFC back-face LAND placement (engine-R). Unchanged, expected, correctly annotated
  now. Removal condition unchanged: both faces offered in the play-land menu.
- Reasoning tax at this seat: max reply 13,561 chars / max latency 217.8s (deck146). Coven
  pre-answering in the guide cut mentions 41% but not the tail. Watch; not yet actionable.
