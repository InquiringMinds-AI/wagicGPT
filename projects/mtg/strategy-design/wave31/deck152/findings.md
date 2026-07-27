# deck152 (Bant Midrange GWU) -- wave-31 findings

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260727-025026`, binary cf7cb2e53, -T 3000,
REPPENALTY=1.05. Wave-30 comparison corpus: `matchups-20260726-181642` (binary 6e168aa05).

**Record 3/6** (up from 2/6 in wave-30), read off the seat's own `gameend` records:

| opponent | won | my_life | opp_life | turn |
|---|---|---|---|---|
| deck148 | **WIN** | 2 | -2 | 19 |
| deck137 | **WIN** | 3 | -6 | 13 |
| deck139 | **WIN** | 22 | -2 | 13 |
| deck146 | LOSS | 0 | 9 | 16 |
| deck122 | LOSS | -1 | 63 | 18 |
| deck158 | LOSS | -6 | 15 | 19 |

**Fallbacks at this seat: 0/243 decisions** (wave-30: 1/313 unparsed). No crash signature.
Decision mix: 162 ask, 27 attackers, 14 blockers, 22 priority, 5 bottom, 2 reveal.

---

## h4 -- MDFC LAND ANNOTATION: **PASS**

The misleading wave-30 text is GONE from every MDFC-land line and replaced with truthful
both-faces + colors text. Verbatim, from **deck158 seq12 t6, kind=priority**:

```
2. Flip Side with Barkchannel Pathway -> DISPLAY TOGGLE only (this is a modal double-faced
land): it currently shows "Barkchannel Pathway" (taps for {G}); its other face is
"tidechannel pathway" (taps for {U}). Flipping only changes which face is DISPLAYED - it
casts nothing, uses no stack, and gains you nothing playable: in this engine only the
currently-shown face can actually be played as a land. Just play the current face.
{card text: "{T}: Add {G}. // Tidechannel Pathway"}
```

The same text also rides the play-land sub-menu. Verbatim, **deck146 seq24 t15, kind=ask**:

```
Choose an option for Hengegate Pathway:
1. Play Land
2. Flip Side [DISPLAY TOGGLE only - this is a modal double-faced land. It currently shows
"Hengegate Pathway" (taps for {W}); its other face is "Mistgate Pathway" (taps for {U}).
Flipping only changes which face is DISPLAYED; it casts nothing and gains nothing playable -
in this engine only the currently-shown face can actually be played as a land. Choose Play
Land to play the current face.]
3. Decline - do nothing
```

Contrast, the wave-30 text this replaced (**wave-30 corpus, deck122 seq6 t0**):

```
1. Flip Side with Barkchannel Pathway -> DISPLAY TOGGLE only: switches this hand card to
show its other face "tidechannel pathway". It does NOT cast anything and uses no stack. You
usually do NOT need it: the Cast menu is where you cast, and it lists every face you can
afford (the other face appears there as an alternative-cost cast). This only changes which
face is displayed. {card text: "{T}: Add {G}. // Tidechannel Pathway"}
```

Counts across the six deck152 seat logs this corpus: new string
(`in this engine only the currently-shown face can actually be played as a land`) = **48
occurrences**; old string (`the Cast menu is where you cast, and it lists every face you can
afford`) = **4 occurrences, none of them a LAND** -- the 4 residuals are the two transform
DFCs, deck148 seq36 (Tovolar's Huntmaster) and deck139 seq28 (Brutal Cathar), which still
carry the old wording and still promise `the other face appears there as an alternative-cost
cast`. That promise is false for a transform DFC too (its back face is never castable), so
the old string survives on a second card class -> routed to notes.md as N-152e (cosmetic
residual, low priority; both windows also offered a real action so the model was never forced
to reach for it).

**Back-face land placement remains blocked** -- as EXPECTED per the brief. Play-land menus
still offer only the front face; e.g. **deck122 seq2 t1**: `3. Play Hengegate Pathway` /
`4. Play Barkchannel Pathway` -- no Mistgate/Tidechannel line. Engine-lane ledger item, NOT a
seat finding.

**Teferi is NOT blocked, however** -- correcting the brief's carried assumption at this seat.
Teferi resolved and was piloted in the deck148 game via Deserted Beach (the guide's own
stopgap teach): cast at **seq25 t13** (`Cast Teferi, Who Slows the Sunset {2}{u}{w}`, mana
line `{g}{u}{w} from 7 untapped sources`, immediately after `seq24 chose Play Deserted
Beach`), `+1` at **seq26**, `-2` at **seq36**, `-2` at **seq42**. Loyalty arithmetic renders
coherently: enters at 4, +1 -> 5, -2 -> 3, -2 -> 1, and **seq45 t19** shows
`Teferi, Who Slows the Sunset {2}{u}{w} [planeswalker] [counters: 1x loyalty]`. Blue is
reachable through Deserted Beach; only the Pathway back face is inaccessible.

---

## h5 -- DISPLAY-TOGGLE-ONLY AUTO-PASS: **PASS (decisive)**

| corpus | priority asks at this seat | display-toggle-ONLY windows |
|---|---|---|
| wave-30 (`matchups-20260726-181642`) | **148** | **121** |
| wave-31 (`matchups-20260727-025026`) | **22** | **0** |

An 85% drop in priority asks, and **zero** windows whose entire option list is cosmetic. All
22 surviving priority windows carry a real action: Ranger Class level-up (deck122 seq32/33/51,
deck148 seq33, deck158 seq33/37), Katilda's `{4}{g}{w},Tap` pump (deck122 seq22/29/38/45),
Teferi loyalty (deck148 seq26/36/42), Clue sacrifice (deck158 seq22/26), and Tovolar's
Packleader's `{2}{g}{g}` fight (deck148 seq45/49). Three of them ALSO list a Flip Side toggle
alongside real actions -- deck148 seq36, deck139 seq28, deck158 seq12 -- which is correct
behavior: the auto-pass is scoped to toggle-ONLY windows, not to toggle-containing ones.

Downstream: the wave-30 unparsed fallback (vs136 seq27, a bare Flip-Side-only window in which
the model fabricated `CHOICE: 2 (Cast Briarbridge Tracker)`) has **no analogue this corpus**;
seat fallbacks went 1 -> 0. N-152b is closed.

Cost side-effect: none observed. Median latency 16.0s -> 16.6s, p90 49s -> 61s; the ask
reduction did not buy speed because the removed asks were the cheap cached ones, but it
removed 121 chances to hallucinate.

---

## GUIDE TEACH VALIDATION -- first guided outing (wave-30 draft)

| # | Teach (guide line) | Verdict | Evidence |
|---|---|---|---|
| T1a | Trust the "Mana available" line; never answer an off-list number | **LANDED** | 0 fallbacks / 243 decisions (wave-30: 1 unparsed from exactly this failure). No phantom-option reply anywhere in the corpus. |
| T1b | "If your only listed action is Flip Side ... the correct answer is 0 (pass)" | **OBSOLETE** | h5 auto-pass means such a window never reaches the model again (0 occurrences). Dead prose; retire. |
| T2 | Develop a creature every turn, then attack with everything that profits; decline attacks only when everything would die for nothing | **LANDED HARD (attack half)** | **27/27 attacker asks declared attackers; ZERO "ATTACK: none"**. Escalation is textbook: deck139 seq9 t5 (A1) -> seq15 t7 (A1,A2) -> seq18 t9 (A1,A2,A3) -> seq32 t13 (A1..A4); deck137 seq19/22/26/29 the same ramp. Wave-30 had 20 attacker asks over 6 games with one game at zero; wave-31 has 27 with every game represented. |
| T2' | ... the DEPLOY half | **MISFIRED (this is the loss)** | "Cast nothing right now" chosen 7/49 casting decisions (wave-30: 3/39). Five are correct instant-holds (removal-only windows: deck139 seq20/27/29, deck158 seq11/13). **Two are fatal**: deck146 **seq21 t13** (life 3, `{g}{w} from 6 untapped sources`, offered Brutal Cathar / Elite Spellbinder / Fateful Absence -> chose nothing) and deck146 **seq26 t15** (life **1**, 5 sources, offered Katilda / Brutal Cathar / Elite Spellbinder / Fateful Absence -> chose nothing, reply: *"If I cast a creature now (like Brutal Cathar or Elite Spellbinder), I will likely need to block next turn. If I block with my new creature against the Adventurer, I die to deathtouch"*). Died at t16 with six cards in hand. The guide's "deploy a creature almost every turn" is a soft aspiration in prose; it lost to a locally-plausible chump-block fear. Needs a hard floor. |
| T3 | Lords/growth: Intrepid Adversary valor, Sigarda on curve, Katilda, Luminarch, level Ranger Class | **LANDED** | Ranger Class cast early and levelled in 5/6 games (L2 `[cost: {1}{g}]`, L3 `[cost: {3}{g}]` -- both match `borderline.txt` Ranger Class `{1}{G}: Level 2 ... {3}{G}: Level 3`); reached **L3** vs158 (seq37; seq40 board shows `[counters: 3x level]`). Katilda's `{4}{g}{w}, Tap` pump correctly DECLINED when it did nothing (deck122 seq22/29/38/45 all choice=0). Intrepid Adversary's valor menu answered sanely (deck148 t10 narration: `Choose an option for Intrepid Adversary: -> don't add any counter` with no spare mana). |
| T4 | Coven pre-answered to cut the reasoning tax | **PARTIAL** | Coven mentions in replies 115 -> 68 (-41%), 0.37 -> 0.28 per decision. But max reply still 13,561 chars (deck146) and max latency 217.8s (deck146), essentially unchanged from wave-30's 12,783 / 202.6s. The tax is reduced, not paid off. |
| T5 | Removal on real threats; protect Brutal Cathar | **LANDED** | Fateful Absence held at instant speed across 5 windows rather than spent on chump bodies (deck139 seq27/29 vs an Arboreal Grazer board; deck158 seq11/13). Cathar was not thrown into bad blocks. |
| T6 | "Werewolves ... flip to NIGHT only if a whole turn passes with NO spells cast ... **this rarely happens, so treat them as their day-side bodies**" | **FALSIFIED PREMISE** | Night flips happened in **4/6 games**. See the werewolf section. The teach's factual claim about frequency was derived from wave-30, a corpus in which the flip was never exercised -- absence of the event was mistaken for rarity. The pilot obeyed the teach and consequently under-planned around a 7/7 and a first-striker. |
| T7a | "lead on Deserted Beach when you want Teferi" | **LANDED** | deck148 seq24 t13 `Play Deserted Beach` -> seq25 `Cast Teferi`. The deck's only blue card resolved for the first time in two corpora, by executing this exact line. |
| T7b | "You can also flip a Pathway to its blue face before playing it" | **FALSE AND NOW CONTRADICTED BY THE SURFACE** | The new h4 annotation states verbatim `in this engine only the currently-shown face can actually be played as a land`. Guide prose contradicting an on-option annotation loses (perception-vs-strategy doctrine) and here the prose is simply wrong. Deleted. |
| T8 | Mulligan: keep 2+ lands and an early creature | **NOT EXERCISED** | 5 `bottom` decisions total; no mulligan-driven disaster keeps observed. Neutral, unchanged. |
| T9 | Matchup posture (grind vs aggro) | **NEUTRAL** | See loss decomposition -- the two matchup losses were structural, not posture errors. |

---

## WEREWOLF DAY/NIGHT -- **EXERCISED AT LAST**, one render defect

Wave-30's open question ("the flip itself never triggered; the flip-thrash fix's live steady
state is UNVALIDATED") is now answered. Day/night flipped in **4 of 6 games**:

- **deck146 seq23 t15**: `Moonrage Brute (3/3) (printed 2/2) [first strike, nightbound]`,
  battlefield carries the `Night` designation. Attacked seq25, `Damage: 3 dealt by Moonrage
  Brute to the opponent` (seq26).
- **deck137 seq27 t13** / **deck139 seq16 t9** / **deck139 seq26 t11**: same Moonrage Brute
  night face, attacking normally (deck139 seq18 `A3. Moonrage Brute (3/3) [first strike,
  nightbound]`; seq19 `Damage: 3 dealt by Moonrage Brute to Gemrazer`).
- **deck148 seq45 t19**: `Tovolar's Packleader (7/7) (printed 6/6) [nightbound]` with its
  `{2}{g}{g}` fight ability live and offered:
  `1. wolf of werewolf fights with Tovolar's Packleader targeting Wolf [cost: {2}{g}{g}]`.
  Attacked at seq48/49 as `A2. Tovolar's Packleader (7/7) [nightbound]`.
- Day-side steady state remained coherent throughout (deck122 seq26/29 `Brutal Cathar {2}{w}
  (2/2) [daybound]` with a `Day` designation on the battlefield line).
- **No flip-thrash, no oscillation, no isFlipped identity desync.** Name, keyword set
  (`first strike, nightbound` / `nightbound`), ability list and current P/T all switch
  together and stay switched. The day/night pseudo-permanent (`Day` / `Night`) renders on the
  battlefield line and is correctly excluded from the creature count. Both states observed
  (deck122 stayed Day all game; deck146/137/139 went Night mid-game and stayed).

**DEFECT FOUND -- the `(printed X/Y)` annotation reads the FRONT face** (N-152d, notes.md).
Verified against `Res/sets/primitives/borderline.txt`: Moonrage Brute is `power=3
toughness=3`; Tovolar's Packleader is `power=7 toughness=7`. Both render with a `(printed
...)` delta reporting the OTHER face's printed values:

- deck146 seq23/26, deck137 seq27, deck139 seq16/26: `Moonrage Brute (3/3) (printed 2/2)` --
  2/2 is Brutal Cathar's printed P/T, not Moonrage Brute's.
- deck148 seq45/49: `Tovolar's Packleader (7/7) (printed 6/6)` -- 6/6 is Tovolar's
  Huntmaster's.

At deck146 seq23 the Brute had no counters, no anthem and no Sigarda on board, so the correct
render is a bare `(3/3)` with NO printed-delta at all. The annotation's whole purpose is to
say "this body is buffed above its base", so it currently tells the pilot a transformed
werewolf is a pumped small creature -- inviting the inference that killing the pump shrinks
it, or that the buff is removable. Contrast: the annotation is CORRECT on genuine buffs in the
same prompts (`Briarbridge Tracker (5/4) (printed 2/3) ... [counters: 1x +1/+1]`,
`Elite Spellbinder (5/3) (printed 3/1) [counters: 2x +1/+1]`, opponent's
`Serra Ascendant (6/6) (printed 1/1)` and `Kor ally (7/1) (printed 1/1)`), so the bug is
face-selection, not the annotation. No misplay traced to it this corpus, but the inference it
invites is exactly the kind that costs a block.

---

## CLASS LEVEL-UP -- steady state CLEAN

- Level shown as counters on an enchantment, correctly excluded from the creature count:
  `Ranger Class {1}{g} [enchantment] [counters: 2x level]` (deck148 seq45),
  `[counters: 3x level]` (deck158 seq40).
- Costs match the primitive: `level Counter with Ranger Class [cost: {1}{g}]` (L2) and
  `[cost: {3}{g}]` (L3); `borderline.txt` Ranger Class `text=` reads `{1}{G}: Level 2 ...
  {3}{G}: Level 3`.
- The model levels with spare mana and declines without it (deck122 seq51: *"The only legal
  action listed is to level up a Ranger Class for {3}{G}. However ... you have no untapped
  Green mana sources"* -> pass; correct for that board).
- Two Ranger Classes coexist and level independently (deck122 seq48: `Ranger Class #1 ...
  [counters: 2x level]` and `Ranger Class #2 ... [counters: 2x level]`). No cross-copy bleed.

---

## FIRST-ORDER FINDING: CASTABLE-BUT-NOT-OFFERED (Sigarda), N-152c

**Sigarda, Champion of Light was payable and NOT offered in the game the seat lost at 1 life
holding her.** Two repros in the deck146 game:

**deck146 seq21 t13** (`kind=ask`, Main phase 1):
```
Mana available: {g}{w} from 6 untapped sources (tapped automatically when you cast)
Your hand: Sigarda, Champion of Light {1}{g}{w}{w} (4/4); Fateful Absence {1}{w} [instant];
Teferi, Who Slows the Sunset {2}{u}{w} [planeswalker]; Katilda, Dawnhart Prime {g}{w} (1/1);
Brutal Cathar {2}{w} (2/2); Elite Spellbinder {2}{w} (3/1)
Your battlefield (creatures: 2): Overgrown Farmland #1; Overgrown Farmland #2; Barkchannel
Pathway; Katilda, Dawnhart Prime {g}{w} (1/1); Brutal Cathar {2}{w} (2/2) [daybound];
Branchloft Pathway; Day
Casting decision (Main phase 1, YOUR turn): which card do you cast now, if any?
1. Cast Fateful Absence {1}{w} - legal targets right now: ...
2. Cast Brutal Cathar {2}{w} (2/2)
3. Cast Elite Spellbinder {2}{w} (3/1)
4. Cast nothing right now
```

**deck146 seq26 t15** (`kind=ask`, Main phase 2, life 1):
```
Mana available: {g}{w} from 5 untapped sources (tapped automatically when you cast)
Your hand: Sigarda, Champion of Light {1}{g}{w}{w} (4/4); ...
Your battlefield (creatures: 1): Overgrown Farmland #1; Overgrown Farmland #2; Barkchannel
Pathway; Moonrage Brute (3/3) (printed 2/2) [first strike, nightbound] [tapped - cannot
attack or block this turn]; Branchloft Pathway; Night; Hengegate Pathway
Casting decision (Main phase 2, YOUR turn): which card do you cast now, if any?
1. Cast Fateful Absence {1}{w} - legal targets right now: ...
2. Cast Katilda, Dawnhart Prime {g}{w} (1/1)
3. Cast Brutal Cathar {2}{w} (2/2)
4. Cast Elite Spellbinder {2}{w} (3/1)
5. Cast nothing right now
```
Sigarda costs `{1}{G}{W}{W}` (`borderline.txt` `mana={1}{G}{W}{W}`). At seq26 the five
untapped sources are Overgrown Farmland x2 (`auto={T}:add{G}` then `auto={T}:add{W}`),
Barkchannel Pathway (`{T}:add{G}`), Branchloft Pathway (`{T}:add{G}`) and Hengegate Pathway
(`{T}:add{W}`). A legal payment exists: `{W}` Hengegate + `{W}` Farmland#1 + `{G}` Barkchannel
+ `{1}` Branchloft. She was not offered.

Root cause identified in code (routed to notes.md): `ManaEngine::potentialMana`
(`src/ManaEngine.cpp:163`) sets `used[card] = true` after crediting a card's FIRST mana
ability, so a two-ability dual land contributes only its first color -- Overgrown Farmland
counts as `{G}` only, Deserted Beach as `{W}` only. The codebase already documents this exact
class at `src/LegalActions.cpp:296-301` ("strict potentialMana counts one ability per card, so
a dual land only ever offered its FIRST color") and fixed it for `hasInstantResponse` via
`potentialManaPermissive` -- but the CAST-OFFER path still uses strict `potentialMana`
(`LegalActions.cpp:348`, `DecisionContract.cpp:434/461`, `AIPlayerBaka.cpp:2754`).

The theory predicts the corpus exactly: **Sigarda WAS offered** at deck137 **seq28 t13** with
only 4 sources -- there the white came from Deserted Beach (first ability `{T}:add{W}`) plus
Hengegate, so two `{W}` survive the one-ability-per-card cull. She was cast and resolved. The
color-SET display line is unaffected (deck148 renders `{g}{u}{w}` off Deserted Beach's SECOND
ability), so the mana line and the cast list disagree silently -- and the guide's #1 rule
tells the pilot to trust the cast list.

Not a piloting error and no guide prose can fix it; the largest single play-quality item at
this seat.

---

## LOSS DECOMPOSITION (3/6)

**vs deck146 (LOSS, t16, 1 -> 0 life) -- PILOTING + the N-152c offer gap.** The only loss with
a fixable pilot component. Turn 13 (seq21, life 3, six sources, three casts offered) and turn
15 (seq26, life 1, five sources, four casts offered) both answered `Cast nothing right now`
while holding Sigarda, Katilda, Brutal Cathar, Elite Spellbinder and Fateful Absence. Final
board: one tapped Moonrage Brute. The seq26 reasoning is a locally-plausible deathtouch-chump
fear that argued itself into deploying nothing at 1 life. Compounding it, Sigarda -- the card
the model's own PLAN named for three consecutive decisions ("Play Hengegate Pathway, then
immediately cast Sigarda") -- was never in the option list (N-152c). Guide revision targets
the deploy half; the offer gap is engine-lane.

**vs deck122 (LOSS, t18, -1 vs 63) -- STRUCTURAL, not a pilot error.** Opponent's Serra
Ascendant rendered `(6/6) (printed 1/1) [flying, lifelink]`; opponent life ran 20 -> 65.
Deck152 has no reach and no maindeck flier answer but Fateful Absence, and the seat attacked
correctly to the end (seq48 t17 `ATTACK: A1, A2, A3` at 5 life against a 6/6 lifelinker,
reply: *"If I do not attack, I will likely die on the opponent's next turn"* -- the right
call). A lifegain deck outrunning a ground midrange clock is a construction/matchup outcome.
No revision.

**vs deck158 (LOSS, t19, 16 -> -6) -- BOARD PRESENCE against amass.** The Orc army grew
`(3/3) [counters: 3x +1/+1]` (seq40 t17) -> `(8/8) [counters: 8x +1/+1]` (seq43 t19) behind
Olog-hai Crusher (4/4 trample) and two Orcish Bowmasters. Deck152's battlefield was down to
ONE creature at both blocker windows (seq40 Luminarch Aspirant, summoning-sick; seq43 Intrepid
Adversary). The blocks were correct and the model correctly read Olog-hai Crusher's block
restriction (*"has Trample and cannot be blocked unless I control a Goblin or Orc. I do not
control any"*). Same "board never survives" shape as wave-30's vs136 loss -- one threat per
turn cannot race a growing army-token engine. Marginally helped by the new deploy floor;
mostly matchup.

Summary: **1 of 3 losses is guide-addressable** (vs146 deploy paralysis), with N-152c layered
on that same game; the other 2 are structural matchup outcomes.

---

## OTHER OBSERVATIONS (no action)

- **Blocking**: 14 blocker asks, all sane. One protocol violation at deck148 **seq17 t10**:
  first line `BLOCKS: B2:A1, B2:A2` -- one blocker on two attackers, which the prompt forbids
  three lines above (`Assign each blocker to AT MOST ONE attacker`). The model self-corrected
  in its own prose (*"Wait, I cannot assign B2 to both A1 and A2"*) but the first line is
  parsed; first-wins applied `B2:A1`, which is the block its corrected reasoning also chose.
  Handled correctly by design, single occurrence, no fallback, no wrong outcome. Not routed.
- **Summoning-sick creatures correctly offered as blockers** with restriction-first wording
  (`Luminarch Aspirant (1/1) [summoning sick - cannot attack this turn]` under `Your available
  blockers`). No misread.
- **Brutal Cathar's exile-until-leaves** and the Clue (`Draw 1 with Clue [cost: {2},
  Sacrifice]`) render and function as in wave-30.
- **No parser contradictions.** One apparent narration/prompt name mismatch (deck148 seq18
  narrating `Intrepid Adversary blocks Stone Haven Outfitter` against a seq17 attacker list of
  `Stone Haven Pilgrim #1/#2`) resolves cleanly: the Outfitter block is an EARLIER combat in
  the append-only narration, and the opponent's own log (deck148 seq14 t10
  `A1. Stone Haven Pilgrim #1 / A2. Stone Haven Pilgrim #2` -> `ATTACK: A1, A2`) confirms both
  seq17 attackers were Pilgrims. Not a defect.
- **Reasoning tax** persists: max reply 13,561 chars, max latency 217.8s (both deck146),
  6 replies over 8k chars. Coven pre-answering helped (-41% mentions) but the deck's
  long-deliberation shape remains.
