# deck152 (Bant Midrange GWU) -- wave-32 findings

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260727-115847`, binary `7fabd9bd0`, -T 3000,
REPPENALTY=1.05. Comparison corpus: wave-31 `matchups-20260727-025026` (binary `cf7cb2e53`).
Second guided corpus; guide REVISED at the end of wave-31 (deploy-floor Rule #3, werewolf
section rewritten, T7b Pathway-flip teach deleted).

**Record 3/6**, read off `results.tsv` + the seat's own `gameend` records:

| opponent | result | my_life | opp_life | turn | shape |
|---|---|---|---|---|---|
| deck116 | **WIN** | 38 | -2 | 12 | kill |
| deck139 | **WIN** | 34 | -3 | 11 | kill |
| deck158 | **WIN** | 11 | -2 | 16 | kill |
| deck36 | LOSS | -8 | 18 | 11 | killed (flying artifact aggro) |
| deck105 | LOSS | **23** | 2 | 17 | **lost by POISON at 23 life** |
| deck146 | LOSS | 12 | 16 | 16 | cap adjudication (corpus's ONLY adj) |

**HL7 decomposition**: KILLS 3 -> 3 (wave-31 also had 3 decisive wins). Pool adjudication rate
collapsed 3/21 -> **1/21**, and that single adjudication is this seat's game. Record is
IDENTICAL (3/6) across a pool that swapped three decks; the pool-invariant statistic (kills) is
flat. No teach regressed.

**Fallbacks at this seat: 0/211 decisions** (2nd consecutive zero-fallback corpus). No crash
signature in any of my six `.stderr` files. Decision mix: 148 ask, 25 attackers, 29 priority,
6 blockers, 2 reveal, 1 bottom.

Latency median 17.7s / p90 66.3s / max 212.4s. Reply p50 1123 / p95 7562 / max 14458 (wave-31:
p50 ~1.1k, max 13561 -- flat).

---

## HL1 HARD GATE -- RETROACTIVE FULL-GUIDE CARD-FACT AUDIT: **PASS**

Every number, cost and zone claim in the deployed `deck152_strategy.txt` was re-read against
`Res/sets/primitives/borderline.txt` + `planeswalkers.txt`. **No false card fact found.**

| guide claim | primitive | verdict |
|---|---|---|
| Sigarda `{1}{G}{W}{W}` 4/4 flying trample, Humans +1/+1 | `mana={1}{G}{W}{W}` `power=4`; `text=Flying, trample -- Humans you control get +1/+1.` | OK |
| Intrepid Adversary `{1}{W}` 3/1 lifelink, ETB pay `{1}{W}` any number of times -> valor, +1/+1 each | `mana={1}{W}` `power=3` `toughness=1` `subtype=Human Scout`; `auto=thisforeach(counter{0/0.1.Valor}>0) lord(creature|myBattlefield) 1/1` | OK |
| Katilda `{G}{W}` 1/1, Humans tap for mana, pro-Werewolves, `{4}{G}{W},{T}` counter on each | `mana={G}{W}` `power=1`, text matches verbatim | OK |
| Luminarch Aspirant `{1}{W}` 1/1, +1/+1 counter each of your combats | `mana={1}{W}` `power=1`; `text=At the beginning of combat on your turn, put a +1/+1 counter on target creature you control.` | OK |
| Ranger Class `{1}{G}`, L2 `{1}{G}`, L3 `{3}{G}`, Wolf on ETB, L2 counter on an attacker, L3 cast creatures off top | `mana={1}{G}`; text `{1}{G}: Level 2 ... {3}{G}: Level 3` and both abilities as stated | OK |
| Fateful Absence `{1}{W}` instant, destroy creature or planeswalker, they investigate | `mana={1}{W}`, text verbatim | OK |
| Brutal Cathar `{2}{W}` 2/2, ETB/transform exiles an opponent creature until Cathar leaves | `mana={2}{W}` `power=2` `subtype=Human Soldier Werewolf`, text verbatim | OK |
| Moonrage Brute 3/3 first strike | `power=3 toughness=3`; `text=First strike -- Ward-Pay 3 life.` | OK (ward omitted -- see below) |
| Tovolar's Huntmaster `{4}{G}{G}` 6/6, ETB two 2/2 Wolves | `mana={4}{G}{G}`, `auto=_WOLFTOKEN_*2` | OK |
| Tovolar's Packleader 7/7, enters OR attacks -> two Wolves, `{2}{G}{G}` fight | `power=7 toughness=7`, text verbatim | OK |
| Elite Spellbinder `{2}{W}` 3/1 flying | `mana={2}{W}` `power=3 toughness=1` | OK |
| Teferi `{2}{W}{U}`, +1 untap yours/tap theirs + gain 2 life, -2 digs three | `mana={2}{W}{U}`, text verbatim | OK |
| Deserted Beach taps `{W}` or `{U}`; Overgrown Farmland `{G}` or `{W}` | `auto={T}:add{W}` then `add{U}`; `add{G}` then `add{W}` | OK |
| Augur of Autumn coven -> cast creatures off the top | `mana={1}{G}{G}` 2/3; coven clause correct | OK |

Three **OMISSIONS** (not falsehoods, no edit forced, recorded for completeness): Moonrage Brute
also has **Ward-Pay 3 life** (a real defensive stat the guide never names); Augur of Autumn's
look-at-top and **play lands from the top** are UNCONDITIONAL (only the creature-cast clause is
coven-gated); Briarbridge Tracker has **vigilance** and **+2/+0 while you control a token**, so
with its own Clue it is a 4/3 vigilance, not the 2/3 the guide's mulligan line implies. The
Brute's ward is added to the revised guide; the other two are below the guide's value bar.

---

## (i1) N-152c CASTABLE-BUT-NOT-OFFERED: **PARTIAL PASS**

The named mechanism is FIXED and demonstrated at its own discriminator. The CLASS is not dead:
two residual shapes survive, both with parser-visible model contradictions.

### The fix WORKS, proven at its own mechanism (the discriminating positive)

**deck158 seq19 t8, `kind=ask`.** Five untapped sources: `Hengegate Pathway` (`auto={T}:add{W}`
only), `Forest` (`{G}`), `Barkchannel Pathway` (`{T}:add{G}` only), `Branchloft Pathway`
(`{T}:add{G}` only), `Overgrown Farmland` (`auto={T}:add{G}` THEN `auto={T}:add{W}`).
Sigarda is `{1}{G}{W}{W}` -- **the second `{W}` exists ONLY as Overgrown Farmland's SECOND mana
ability.** Post-fix render:

```
Mana available: 5 total (five untapped sources, tapped automatically when you cast; colours you can make: {g}{w})
Casting decision (Main phase 1, YOUR turn): which card do you cast now, if any?
1. Cast Fateful Absence {1}{w} - legal targets right now: ...
2. Cast Sigarda, Champion of Light {1}{g}{w}{w} (4/4) {card text: "Flying, trample -- Humans you control get +1/+1. ..."}
3. Cast nothing right now
```

**She was offered, cast, and resolved** -- on the battlefield at seq34, attacking at seq37/seq47.
Under the pre-fix strict one-ability-per-source bag the Farmland contributes `{G}` only, one
`{W}` exists in total, and this is byte-for-byte the wave-31 seq21/seq26 silent drop.

**Negative control, same game, same board minus the Farmland**: seq14 t6 / seq16 t7 / seq17 t7,
FOUR untapped sources (Hengegate `{W}`, Forest `{G}`, Barkchannel `{G}`, Branchloft `{G}`) --
one white source in total, Sigarda genuinely unpayable, **correctly not offered**. The offer
appears on the exact turn the second white source arrives. That is the fix engaging, isolated.

**Second independent positive, the Teferi path**: deck146 **seq11 t7** (4 sources: Deserted
Beach, Overgrown Farmland, Plains, Branchloft Pathway) offers
`1. Cast Teferi, Who Slows the Sunset {2}{u}{w}`. The ONLY blue on that board is Deserted
Beach's SECOND ability (`auto={T}:add{W}` then `auto={T}:add{U}`). Repeats at deck146 seq14 t9,
seq20 t11 and deck139 seq23 t11. Teferi was cast and piloted to 4 loyalty in the deck146 game.

### RESIDUAL A (engine, MEDIUM-HIGH): all-flexible land base -> mutual deferral -> silent drop

**deck116, the whole game.** Land base at seq17 t8: `Deserted Beach #1..#4` + `Barkchannel
Pathway`, five untapped, mana line `5 total (five untapped sources ... colours you can make:
{g}{u}{w})`, hand `Fateful Absence; Fateful Absence; Teferi, Who Slows the Sunset {2}{u}{w}`.
Teferi is trivially payable (Beach#1 -> `{U}`, Beach#2 -> `{W}`, Beach#3/#4 -> generic).
Offer list:

```
1. Cast Fateful Absence {1}{w} - the only legal targets are YOUR OWN right now ...
2. Cast nothing right now
```

**Teferi is never offered in that game at any window** -- seq17, seq19, seq23, seq25, seq28,
seq30 all with 5 untapped sources; grep of the seat log finds zero `Cast Teferi` lines. The
model planned around it repeatedly (seq17 PLAN: *"If I play the land, I have 5 sources. Teferi
costs {2}{U}{W}. I can cast it."*) and finally verbalized the defect against the core prompt's
own completeness guarantee -- **deck116 seq18 t8, `kind=priority`, verbatim**:

> *"I have Teferi in hand. I have mana ({g}{u}{w}). I can cast Teferi. Why is it not listed?
> Maybe the 'Legal actions' list provided in the prompt is incomplete or I am supposed to infer
> the standard actions? No, the instructions say: 'The lists are also complete... if a spell is
> not in the cast list you CANNOT cast it right now'. **This is a contradiction.** I have Teferi
> in hand, I have the mana, it is my main phase. Why is it not listed?"*

**The discriminator against the working case is the presence of a MONO source.** deck146 seq11
(offered) has `Plains` -- a dedicated `{W}` -- so the flexible Beach is freed for `{U}`.
deck116 has **no mono source for either pip**: every `{W}` and every `{U}` lives on a Deserted
Beach.

Mechanism, code-grounded (`src/ManaEngine.cpp:121-160`, `deferFlexibleSource`): the SELF test
`sourceHasOtherNeeded` aggregates over **every ability whose `amp->source == card`** (lines
136-142), but the OTHER-source test `competesElsewhere` is evaluated **per-ABILITY** on whatever
`amp` the loop is holding (lines 143-156). A second Deserted Beach reached through its
`add{W}` ability has `amp->output->hasColor(U) == false`, so `competesElsewhere` stays false,
`otherUnusedCoversK` becomes true, and the first Beach defers -- and by symmetry so does every
other Beach. This is exactly the mutual deferral the function's own comment at lines 146-149
says it prevents ("two flexible sources could MUTUALLY defer and strand the payment"); the guard
fails because its two halves are evaluated at different scopes. The colour-first gate shipped in
`6ca1f8411` is upstream of this and never reaches it.

**HL5 signature, textbook.** The shipped fixture `Res/test/ai/dual_land_multipip_cast_n152c.txt`
is `Scrubland (W,B) + Vault of Whispers (B) + Darksteel Citadel` -- **one dual plus a MONO
source of the contested colour**, i.e. the deck146 shape. The all-flexible sibling path has no
fixture and is exactly where the class survives. Routed to notes.md as **N-152f** with a fixture
spec.

### RESIDUAL B (LOW, needs a probe -- do not act on a guess)

**deck105 seq36 t16**: 5 untapped sources `Hengegate Pathway {W}`, `Overgrown Farmland #1`,
`Overgrown Farmland #2`, `Lair of the Hydra {G}`, `Plains {W}`; hand holds
`Sigarda, Champion of Light {1}{g}{w}{w}`. Payable off MONO sources alone (Hengegate `{W}` +
Plains `{W}` + Lair `{G}` + a Farmland for the `{1}`), so Residual A's mechanism does not
explain it. Offer list is `1. Cast Luminarch Aspirant {1}{w} (1/1)` / `2. Cast nothing right
now`. The model flagged it at **deck105 seq28 t14**, verbatim:

> *"Let's recount mana carefully. Lands: 1. Hengegate Pathway (W) 2. Overgrown Farmland #1 (W/G)
> 3. Overgrown Farmland #2 (W/G) 4. Lair of the Hydra (G) 5. Plains (W) Total: 5 ... Total cost
> 4. I have 5. I can cast it. So why isn't it in the list? ... **Okay, there is a contradiction.**"*

**The free discriminator**: Sigarda WAS offered and cast in this same game at **seq14 t8** --
when no Sigarda was on the battlefield. Both absent windows (s28, s36) have
`Sigarda, Champion of Light {1}{g}{w}{w} (4/4)` already in play. Leading hypothesis is a
legendary-duplicate suppression on the cast path; I did **not** locate one in `LegalActions.cpp`,
and the `AIPlayerBaka.cpp:2902` gate is planeswalker/battle-subtype-scoped and cannot touch a
creature. Recorded with a trigger predicate, NOT as a diagnosis (notes N-152g).

### Cost of the class this corpus

The two contradiction windows above are among the seat's largest replies -- the model spends
kilobytes re-deriving payability against a list it is told is complete. Both resolved to a legal
in-list answer (no fallback), so the cost is tax and burned windows, not a wrong action.

---

## (i15) N-152d/e TRANSFORM-DFC HONESTY: **half PASS, half PATH-SCOPED PARTIAL**

### (e) Flip-Side text on BOTH emitter paths: **PASS**

Zero literal `backside` across all 42 corpus logs (`/usr/bin/grep -c backside` = **0**). Both
transform-DFC emitters carry truthful, name-resolved text.

Priority-window emitter, **deck105 seq29 t14** verbatim:

```
3. Flip Side with Tovolar's Packleader -> DISPLAY TOGGLE only (this is a TRANSFORMING
double-faced card): it currently shows "Tovolar's Packleader"; its other face is "Tovolar's
Huntmaster" ({4}{g}{g}). Flipping only changes which face is DISPLAYED - it casts nothing, uses
no stack, and gains you nothing playable: this card is NEVER cast as its other face (there is no
alternative-cost cast for it), it only TRANSFORMS through its own printed transform condition.
Ignore this option.
```

Sub-menu emitter, **deck146 seq15 t9** verbatim:

```
2. Flip Side [DISPLAY TOGGLE only (this is a TRANSFORMING double-faced card): it currently shows
"Brutal Cathar"; its other face is "Moonrage Brute". Flipping only changes which face is
DISPLAYED - it casts nothing, uses no stack, and gains you nothing playable: this card is NEVER
cast as its other face (there is no alternative-cost cast for it), it only TRANSFORMS through its
own printed transform condition. Ignore this option.]
```

Both paths resolve the real face name (`Moonrage Brute`, `Tovolar's Packleader`, `Tovolar's
Huntmaster`) and both explicitly negate the old alternative-cost promise. MDFC-land text (h4,
wave-31) is unchanged and still correct (deck152 seq10/19/26). **A THIRD path still carries the
wave-30 wording**: MDFC *spells*. deck146's seat, **seq36**: `Flip Side with Emeria's Call ->
... the Cast menu is where you cast, and it lists every face you can afford (the other face
appears there as an alternative-cost cast)` -- false there too (Emeria's back face is a land).
Not my seat's card; routed cross-seat (notes N-152h).

### (d) `(printed X/Y)` on a transformed werewolf: **STILL FALSE on the battlefield line**

The fix landed on the attacker/target enumerators and NOT on the board snapshot. Same card, same
prompt, two renders -- **deck158 seq38 t14**:

```
Your battlefield (creatures: 3): ... Moonrage Brute (3/3) (printed 2/2) [first strike, nightbound] [tapped - attacking]
...
2. Moonrage Brute (3/3) [first strike, nightbound] [your battlefield] [tapped] - "First strike -- Ward-Pay 3 life. -- Nightbound (..."
```

Moonrage Brute is `power=3 toughness=3` in `borderline.txt` and `subtype=Werewolf` -- NOT a
Human, so Sigarda's `lord(human)` does not touch it; there were no counters and no other anthem.
Delta is **0** and the correct render is a bare `(3/3)`. `(printed 2/2)` is Brutal Cathar's
front-face P/T.

Corpus-wide counts:

| string | occurrences |
|---|---|
| `Moonrage Brute (3/3) (printed 2/2)` | **22** |
| `Moonrage Brute (3/3) [` (clean) | 12 |
| `Tovolar's Packleader (7/7) (printed 6/6)` | **0** |
| `Tovolar's Packleader (7/7) [` (clean) | 3 |

Packleader appeared only on the FIXED paths this corpus, so its zero is coverage, not proof.
Witness seqs for the surviving battlefield-line emitter: deck158 s34, s38, s40, s43.

**Positive control, same seat, same tag**: `Brutal Cathar {2}{w} (3/3) (printed 2/2) [daybound]`
at **deck158 seq32** is CORRECT -- Cathar is `subtype=Human Soldier Werewolf`, Sigarda was on the
battlefield, so the +1/+1 lord really does make it 3/3 over a printed 2/2. Also correct on
`Sigarda ... (5/5) (printed 4/4) [counters: 1x +1/+1]` (deck158 s42) and `Luminarch Aspirant
(2/2) (printed 1/1)` (deck105 s38). The annotation logic is right; only the transformed-face
selection on the battlefield emitter is wrong. Routed as **N-152d layer 2**.

---

## REVISED-GUIDE VALIDATION (wave-31 revision, first corpus)

### R1 -- DEPLOY FLOOR (Rule #3): **FIRED. The failure mode it was written for did NOT recur.**

Wave-31's two fatal holds were at **3 life** and **1 life** with three and four casts offered.
Applying the HL9 cast-nothing SPLIT:

| corpus | casting decisions | `Cast nothing` | (a) removal-only window (legit hold) | (b) CREATURE offered (decline against the floor) | lowest life of a (b) decline |
|---|---|---|---|---|---|
| wave-30 | 39 | 3 | -- | -- | -- |
| wave-31 | 49 | 7 | 5 | **2** | **1 life** |
| wave-32 | 56 | 15 | **13** | **2** | **11 life** |

The raw rate rose 14% -> 27%, and the raw rate is again uninformative: **13 of 15 are
instant-hold windows whose only cast offer was Fateful Absence** (deck116 s12/s14/s15/s17/s19/s30,
deck158 s14/s16/s17/s35, deck139 s19/s21/s22 -- exactly the hold the guide sanctions). The
against-floor count is **flat at 2**, and neither is at a lethal life total; **zero declines
below 11 life**, against wave-31's 3 and 1. The rule's specific pathology is gone.

The two survivors, both in a game the seat WON:

- **deck158 seq27 t12, life 20, one creature on board**, offered `2. Cast Brutal Cathar {2}{w}
  (2/2)` -> chose `3. Cast nothing right now`. Reply verbatim: *"The opponent has no untapped
  creatures that can attack this turn ... Therefore, there is no immediate threat to remove with
  Fateful Absence or Brutal Cathar."* It then quotes the guide against itself -- *"However, the
  strategy guide says: 'DEPLOY A CREATURE EVERY TURN'. Holding a creature in hand does nothing."*
  -- and still answers 3. **A genuine violation.** The trigger shape is NEW and is not the
  wave-31 chump-block fear: it is *"the opponent cannot attack me this turn, so nothing is
  urgent"*. The guide's floor is written against DANGER ("low life is a reason to deploy"); it
  has no clause for SAFETY. The same reply also shows a mana wobble against the new count-first
  line (*"you have no white mana source untapped ... Wait ... You can definitely make {W}. Why
  not cast?"*), self-corrected inside the reply.
- **deck158 seq44 t16, life 11**, offered `1. Cast Elite Spellbinder {2}{w} (3/1)` -> chose
  nothing, because lethal was already on the board: *"Sigarda's trample ensures she deals at
  least 3 ... Moonrage Brute deals 3 ... Total damage is at least 6, which is lethal."* The
  attack was declared at seq47 and **won the game that turn**. This is a CORRECT decline the
  guide's absolute does not license -- the rule needs its second exception written in.

### R2 -- WEREWOLF REWRITE: teach TRUE, **frequency claim over-corrected** (HL6 violated with the sign flipped)

The mechanics half is right and fired. Day/Night designations render on the battlefield line and
are correctly excluded from the creature count (deck146 `Day` in 16 prompts / `Night` in 11;
deck158 `Night` in 15). A flip happened and was piloted well: `Moonrage Brute (3/3) [first
strike, nightbound]` attacked at deck158 seq37 and seq47 alongside Sigarda, dealt `Damage: 3
dealt by Moonrage Brute to Orc army` (s43), and its first strike was read correctly by the
OPPONENT's own reply (deck158 s35: *"Brute hits first. Crebain takes 3, dies. Brute deals 0 to
me."*). No flip-thrash, no identity desync.

**But the revised guide asserts a FREQUENCY, in the opposite direction from the one wave-31
retired.** Deployed text: *"WEREWOLVES / DAY-NIGHT - THIS HAPPENS OFTEN, PLAN FOR IT"*, *"which
happens regularly by mid-game - EXPECT your werewolves to flip"*. Observed:

| corpus | games with a werewolf on my battlefield | games where one FLIPPED |
|---|---|---|
| wave-31 | 6 | 4 |
| wave-32 | 3 (deck146, deck116, deck158) | **1** (deck158) |
| combined | 9 | 5 |

wave-31's HL6 says a frequency/rarity claim must never be authored at all -- write the TRIGGER
and BOTH outcomes and let the board line answer. The revision replaced *"this rarely happens"*
with *"THIS HAPPENS OFTEN"*, which is the **same rung violated with the sign flipped**, and the
new claim is 1/3 supported by this corpus. Removed in the revision below; the trigger sentence
and the read-the-designation instruction do all the work.

### R3 -- T7b PATHWAY-FLIP DELETION: **PASS, but the prohibition is UNDER-SCOPED**

Zero Pathway-flip decisions this corpus. But the seat spent **two priority windows** flipping a
TRANSFORM DFC in hand, against an annotation that literally says `Ignore this option`:
deck105 **seq28 t14** `choice 3` = `Flip Side with Tovolar's Huntmaster` (the hand card then
renders `Tovolar's Packleader (7/7)` at s29) and **seq31 t14** `choice 2` = the same toggle
again. The deployed guide forbids this only for *"A Pathway's blue back face"*. Generalized to
ALL Flip Side options in the revision.

Side observation, low severity: the displayed face of that hand card oscillates across snapshots
with only two model-issued flips -- s29 `Packleader`, s30 `Huntmaster`, s32 `Packleader`,
s33 `Huntmaster`. Recorded in notes without a diagnosis.

### R4 -- ATTACK FLOOR: **HOLDS, 24/25**

25 attacker asks, **zero mixed-form drops, zero parse failures**, and in 24 of 25 the seat
declared **every** creature on the eligibility list. Escalation intact: deck116 s9 (1 of 1) ->
s13 (2 of 2) -> s20 (3 of 3); deck139 s15 (2 of 2) -> s20 (4 of 4) -> s28 (5 of 5).

The one decline, **deck139 seq9 t5**: `A1. Luminarch Aspirant (1/1)` alone into `Arboreal Grazer
{g} (0/3) [reach]`. `ATTACK: none`. Not the wave-30 pathology -- the reasoning is a correct read
that a 1/1 into a 0/3 wall accomplishes nothing and costs a blocker. Strictly the guide's
"decline only when every attacker would die for nothing" does not cover it (nothing dies); this
is the one wording corner worth relaxing. One instance in 25; the game was won.

### R5 -- REMOVAL DISCIPLINE / Rule #1: **PASS**

13 Fateful-Absence-only windows held at instant speed. When Absence was spent it was on impact:
deck116 s23 on `Sheoldred, Whispering One (6/6)`. Zero off-list answers, zero invented Cast
numbers, zero fallbacks across 211 decisions.

### R6 -- LORDS / CLASS STEADY STATE: **CLEAN**

Ranger Class cast in 5 of 6 games, reached L2 in 4 (deck116 s18, deck36 s18, deck105 s37,
deck158 s36); L3 not reached this corpus. Two copies coexist and level independently -- deck158
s42 `Ranger Class #1 ... [counters: 2x level]` and `Ranger Class #2 ... [counters: 1x level]`, no
cross-copy bleed. Level counters render on the enchantment and are excluded from the creature
count. Intrepid Adversary's valor menu answered sanely; the lord fired and rendered
(`Intrepid Adversary (5/3) (printed 3/1) [lifelink] [counters: 2x +1/+1]`, deck116 s28).
Katilda's `{4}{G}{W},{T}` pump correctly declined when it did nothing.

---

## OTHER BATCH ITEMS WITNESSED AT THIS SEAT

- **(i8) combat tags: PASS.** `[tapped - attacking]` renders 22x; **zero** occurrences of the
  paradox string `[tapped - cannot attack or block this turn] [attacking]`, and zero paradox
  verbalizations. The two `contradiction` strings in this seat's replies are both the N-152c
  offer gap above, not tag misreads. Summoning-sick blockers render restriction-first and were
  read correctly (`Luminarch Aspirant (1/1) [summoning sick - cannot attack this turn, but CAN
  block]`, offered and used as a blocker).
- **(i12) count-first mana line: PASS with one wobble.** `Mana available: 5 total (five untapped
  sources ... colours you can make: {g}{w})` in every window. Zero "I cannot afford" declines of
  an offered payable cast. The one wobble (deck158 s27) self-corrected inside the reply.
- **(i13) card text on Cast lines: PASS, cost is tail-concentrated.** Every `Cast` option carries
  `{card text: "..."}`. Prompt lengths wave-31 -> wave-32 at this seat: p50 7564 -> 7610 (+0.6%),
  p90 14102 -> 15769 (+11.8%), max 17469 -> 23775 (+36%). No degradation: 0 fallbacks, reply p50
  flat, attack declarations flat. Flagged for the tail only.
- **(i5) loud re-ask: zero markers** (`validation_reject_reask` / `_exhausted` both 0 at this
  seat) -- consistent with the brief; no validation reject occurred here.
- **(i16) crash-grep hygiene: PASS**, 0 signatures in all six of my `.stderr` files.
- **(N-152a)** MDFC back-face land placement still unofferable, as expected. Teferi remains
  castable via Deserted Beach (deck146 s20, resolved, piloted to 4 loyalty).
- **Blocking: 6 asks, all defensible.** The two `no blockers` declines are correct: deck146 s36
  (`Intrepid Adversary (3/1) [lifelink]` vs `Triumphant Adventurer (1/1) [first strike,
  deathtouch]`, 1 damage non-lethal at 9 life) and s43 (two 1/1s vs the same first-strike
  deathtoucher at 11 life). Blocking either way loses a creature for nothing.

---

## THE UNPARSED REPLIES IN MY GAMES (both at the OPPONENT seat; ONE shape)

The corpus's 2 `unparsed_reply` fallbacks are both in deck152's games, both at the opponent's
seat, both `kind=blockers`, and both are the SAME defect: **a compliant `BLOCKS:` head line
written with CARD NAMES instead of the `B#:A#` labels.** My seat produced neither.

**deck139 seq21 t11** (the vs139 game), reply first line verbatim:

```
BLOCKS: Pollywog Symbiote blocks Elite Spellbinder
```

Offer was `B1. Pollywog Symbiote (1/3)` against `A1..A5`; the intended assignment is
unambiguously `B1:A5`. The rest of the reply is a correct, exhaustive damage analysis concluding
the block cannot save the player.

**deck158 seq35 t16**, reply first line verbatim:

```
BLOCKS: Orc army: Sigarda, Champion of Light; Dunland Crebain: Moonrage Brute
```

`name:name` form. Offer listed only `B1. Orc army (1/1)`; `Dunland Crebain` was tapped and not
offered, so a name-form parser would map `B1:A2` and drop the unknown name.

**Classification: a PARSER COVERAGE gap, not a model failure.** In both cases the answer-first
protocol was obeyed (correct `BLOCKS:` anchor on line 1), the assignment is unambiguous against
the offered lists, and the strategic content is sound. The heuristic answered instead. Both games
were won by deck152, so no outcome at my seat turned on it, but the deck158 instance decided a
turn-16 survival block at 6 life. Routed to notes.md (**N-152j**) with a name-resolution spec;
deliberately NOT routed to the core prompt (see general-suggestions.md).

---

## THE LOSSES

### deck105 (LOSS at **23 life**, opponent at 2) -- **THE FINDING OF THE CORPUS: the seat lost to an INVISIBLE alt-win clock.**

deck105 wins by poison. **No poison total is rendered anywhere in the 42-log corpus.**
`/usr/bin/grep -o "poison[^\"]*"` across every log returns only card text and model prose --
there is no `Your poison counters: N` line, no opponent equivalent, and no lethality warning.
Worse, the narration reports infect damage in the ordinary damage form **with no accompanying
life change**, so the board reads as if nothing is happening. Verbatim from the seat's final
narration (life stays 23 throughout):

```
- Damage: 1 dealt by Necropede to you
- Damage: 1 dealt by Blight Mamba to you
- Damage: 1 dealt by Plague Stinger to you
- Damage: 2 dealt by Contagious Nim to you
```

`gameend`: `{'won': False, 'my_life': 23, 'opp_life': 2, 'turn': 17}`. The seat was three points
from killing deck105 and made no visible mistake: it was ahead on board, attacking every turn
with Sigarda (s20/s23/s30/s39, all four attacker asks full), and removing infect bodies with
Fateful Absence when they were targetable. The -1/-1 counters infect puts on CREATURES do
narrate correctly (`Counter added to Wolf: -1/-1 (now 0/0) [from Necropede]`), which makes the
player-side omission more conspicuous. Pure perception defect at the render; no guide prose
substitutes for a missing number. Routed as **N-152i (HIGH)**. A stopgap matchup line is added
to the guide and explicitly marked for retirement when the render lands.

### deck36 (LOSS, t11, -8 vs 18) -- **STRUCTURAL.**

`Glaze Fiend {1}{b} (0/1) [artifact] [flying]` x2 behind eight artifacts on an
`Ancient Den / Seat of the Synod / Tolarian Academy / Mox` mana base. Deck152 maindecks no reach
and exactly two Fateful Absence, both already spent (s11, s13). Final board: two ground bodies
against two evasive threats. The seat deployed on curve every turn (Ranger Class t2, Intrepid t4,
Luminarch t8, Intrepid t10) with zero cast-nothing windows. Construction/matchup, no revision.
(The sideboard has four Skyclave Apparition -- roster-lane, not guide-lane.)

### deck146 (LOSS by cap adjudication, 12 vs 16 at t16) -- **GRIND, deploy floor HELD.**

The corpus's only adjudication and this seat's game, 44 decisions in 3000s. **Zero cast-nothing
windows in the entire game** -- the wave-31 pathology at this exact matchup did not repeat: 10
casts across 16 turns (Intrepid t5, Absence t7, Katilda t7, Cathar t9, Teferi t11, Spellbinder
t13, Intrepid t13, Katilda t15, Luminarch t15) and an EMPTY hand at the cap. What it could not
do was ATTACK: **one attacker ask in 44 decisions** (s42 t15). deck146's `Triumphant Adventurer
(1/1) [first strike, deathtouch]` plus exile removal kept the seat's board either summoning-sick
or gone every turn, so `hasLegalAttacker` was almost never true. This is the deploy-floor teach
working and the CLOCK still failing -- a ceiling signature for a midrange deck whose only reach
is combat damage, not a pilot error.

**Summary: 0 of 3 losses has a decisive guide-addressable pilot component** (wave-31 had 1). The
guide-addressable residue is the deck158 s27 safety-decline in a WON game and the under-scoped
Flip Side prohibition.

---

## ROTATION RECOMMENDATION

**HOLD one more corpus; do NOT freeze, do NOT project exit yet.** Reasoning:

- The HL1 card-fact audit **PASSED** clean, so the seat is now eligible for an exit projection on
  that gate -- the first of the two conditions.
- It fails the second: this wave's revision touches four sections (werewolf frequency, Flip Side
  scope, deploy-floor exception, infect posture) and **none of them has a corpus yet**. A guide
  revised this wave cannot be frozen this wave.
- The infect matchup line is an explicit STOPGAP against N-152i; the guide cannot be frozen while
  carrying a stopgap whose render fix is queued.
- Teach record is stable across two guided corpora (attack floor 27/27 -> 24/25; removal
  discipline clean twice; lords/Class clean twice; deploy-floor pathology cured) and KILLS are
  flat at 3 under a 3-deck pool change -- a good exit profile forming. **Projected exit: wave-34**
  if the four revised sections validate in wave-33 and N-152f/N-152i land.
