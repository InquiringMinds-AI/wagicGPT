# deck152 -> development notes (wave-32)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260727-115847`, binary `7fabd9bd0`.
All items engine/render/harness lane. Seq citations are deck152-seat unless labelled otherwise.

---

## N-152i -- **HIGH, NEW.** Poison counters are rendered NOWHERE, and infect damage narrates as ordinary damage with no life change. Cost this seat a game at 23 life.

**Repro (no probe needed, it is in the corpus).** Game `152v105`, log
`1785178459-ai_baka_deck152-0x55f9261c6960-vs-ai_baka_deck105.jsonl`.
`gameend` (seq 40): `{'won': False, 'my_life': 23, 'opp_life': 2, 'turn': 17}`.
`results.tsv`: `152 105 1 23 2 17`.

Narration in the seat's own final prompt, with `Your life: 23` unchanged across all of it:

```
- Damage: 1 dealt by Necropede to you
- Damage: 1 dealt by Blight Mamba to you
- Damage: 1 dealt by Plague Stinger to you
- Damage: 2 dealt by Contagious Nim to you
```

`/usr/bin/grep -o "poison[^\"]*"` over **all 42 corpus logs** returns only card text and model
prose. There is no `Your poison counters:` line, no opponent equivalent, no threshold warning, on
EITHER seat -- deck105's own log cannot see its win condition's progress either.

Note the asymmetry that makes this a render bug and not a missing feature: infect's CREATURE
effect narrates correctly today --
`- Counter added to Wolf: -1/-1 (now 0/0) [from Necropede]` -- so the counter machinery is wired;
only the PLAYER-facing poison total is unrendered.

**Spec (three parts, all render/narration lane):**
1. `--- CURRENT SITUATION ---` gains `Your poison counters: N/10` and
   `Opponent poison counters: N/10`, emitted whenever either side is above 0 (mirroring how the
   `Dungeons completed (opponent): 1` line is conditionally emitted -- see deck146 s43).
2. Damage dealt to a PLAYER by a source with infect must narrate as the counter gain, not as
   damage: `- Poison counters +2 (now 7) [from Contagious Nim]`. The current
   `Damage: N dealt by X to you` with no accompanying `Your life -N` is actively misleading -- it
   reads as an event that did nothing.
3. Lethality warning parity with the combat header: at 8+ poison the situation block should carry
   the same shape as `Unblocked, these attackers deal up to N - you would be at -X - LETHAL`,
   e.g. `You are at 8 poison counters - TWO more lose you the game regardless of life total.`

**Gate / falsifiable prediction for wave-33**: any game against deck105 must show the poison line
in the victim seat's prompts; a loss with `won:false` and `my_life > 10` must not recur without
the number having been visible. Owning seat next wave: deck105 (attacker) + whoever draws it.

---

## N-152f -- **MEDIUM-HIGH, NEW: N-152c LAYER 2.** All-flexible land base -> mutual deferral in `deferFlexibleSource` -> payable multi-pip cast silently not offered.

**Live repro.** Game `152v116`, log `1785174946-ai_baka_deck152-0x56226868c850-vs-ai_baka_deck116.jsonl`.
Board at **seq17 t8** (identical at seq19, 23, 25, 28, 30):

```
Mana available: 5 total (five untapped sources, tapped automatically when you cast; colours you can make: {g}{u}{w})
Your hand: Fateful Absence {1}{w} [instant]; Fateful Absence {1}{w} [instant]; Teferi, Who Slows the Sunset {2}{u}{w} [planeswalker]
Your battlefield (creatures: 3): Deserted Beach #1; Barkchannel Pathway; Ranger Class ...; Deserted Beach #2; ...; Deserted Beach #3; ...; Deserted Beach #4
Casting decision (Main phase 1, YOUR turn): which card do you cast now, if any?
1. Cast Fateful Absence {1}{w} - the only legal targets are YOUR OWN right now ...
2. Cast nothing right now
```

Teferi is `mana={2}{W}{U}` (`planeswalkers.txt`); Deserted Beach is `auto={T}:add{W}` THEN
`auto={T}:add{U}` (`borderline.txt`). Beach#1 -> `{U}`, Beach#2 -> `{W}`, Beach#3/#4 -> generic
is a legal payment. **`Cast Teferi` appears zero times in the entire game log.** The model
verbalized it at seq18: *"I have Teferi in hand, I have the mana, it is my main phase. Why is it
not listed? ... This is a contradiction."*

**Same-seam positive controls (the fix DOES work where a mono source exists)**: deck146 s11 t7,
s14 t9, s20 t11 and deck139 s23 t11 all OFFER Teferi -- each of those boards contains a `Plains`
or `Hengegate Pathway` (dedicated `{W}`), freeing the single Beach for `{U}`. deck158 s19 t8
offers Sigarda `{1}{G}{W}{W}` off `Overgrown Farmland`'s SECOND ability, with s14/s16/s17
(one white source only) as the negative control. So the wave-32 colour-first gate in
`ManaEngine::planPayment` (`6ca1f8411`) is genuinely working; this is a DIFFERENT branch.

**Hypothesis, code-grounded (`src/ManaEngine.cpp:121-160`, `deferFlexibleSource`)** -- to be
CONFIRMED by the fixture below before any edit, not assumed:
- the SELF test `sourceHasOtherNeeded` aggregates over every ability with `amp->source == card`
  (lines 136-142) -> for a Deserted Beach considering `k = W`, its `add{U}` ability makes this
  true;
- the OTHER test `competesElsewhere` is evaluated **per-ABILITY** on whichever `amp` the loop
  holds (lines 143-156) -> another Beach reached via its `add{W}` ability has
  `amp->output->hasColor(U) == false`, so it is judged "dedicated to W" and
  `otherUnusedCoversK` becomes true;
- both are true -> `return true` -> Beach#1 defers `{W}`; by symmetry every Beach defers, and by
  the same argument every Beach defers `{U}`. Nothing claims either pip and the walk returns
  empty.
This is the exact deadlock the function's own comment at lines 146-149 claims to prevent ("two
flexible sources could MUTUALLY defer and strand the payment"); the guard fails because the SELF
half is source-scoped and the OTHER half is ability-scoped. Candidate fix: aggregate
`competesElsewhere` over ALL abilities of `amp->source`, symmetric with the self test.

**HL5 note -- why the shipped fixture missed it.** `Res/test/ai/dual_land_multipip_cast_n152c.txt`
is `Scrubland (W,B) + Vault of Whispers (B) + Darksteel Citadel`: ONE dual plus a MONO source of
the contested colour. That is the deck146 shape, the branch that already worked. The
all-flexible board has no fixture.

**Fixture spec (both required):**
- POSITIVE, currently failing: `[PLAYER1] inplay: Scrubland, Scrubland, Darksteel Citadel` /
  `hand: <a {1}{W}{B} spell>`. Both pips exist only on the two duals; CMC 3 over 3 sources is
  payable. PASS = the spell is cast and reaches the battlefield.
- NEGATIVE GUARD (keep the over-offer mirror honest): `Scrubland, Scrubland, Darksteel Citadel`
  with `{2}{B}{B}` must still NOT be offered -- the existing
  `dual_land_multipip_not_offered_n152c.txt` already covers this and must stay green, since the
  wrong fix (routing the cast path onto `potentialManaPermissive`) would double-count each dual.

---

## N-152d layer 2 -- **MEDIUM, PATH-SCOPED RESIDUAL.** `(printed X/Y)` on a transformed werewolf is FIXED on the attacker/target enumerators and STILL FALSE on the battlefield line.

Wave-31 raised N-152d; the wave-32 batch fixed it on some emitters only. Same card, same prompt,
**deck158 seq38 t14**:

```
Your battlefield (creatures: 3): ... Moonrage Brute (3/3) (printed 2/2) [first strike, nightbound] [tapped - attacking]
2. Moonrage Brute (3/3) [first strike, nightbound] [your battlefield] [tapped] - "First strike -- Ward-Pay 3 life. ..."
```

`borderline.txt` Moonrage Brute: `power=3 toughness=3`, `subtype=Werewolf` (NOT Human, so
Sigarda's `lord(human|mybattlefield) 1/1` does not apply); no counters, no other anthem. Delta is
0 and the tag must be absent, as it correctly is on the enumerator line.

Corpus counts (all 42 logs): `Moonrage Brute (3/3) (printed 2/2)` = **22**;
`Moonrage Brute (3/3) [` = 12. `Tovolar's Packleader (7/7) (printed 6/6)` = **0** and
`Tovolar's Packleader (7/7) [` = 3 -- Packleader only reached the fixed paths this corpus, so
its zero is COVERAGE, not proof; do not read it as the class being closed.

Witness seqs, battlefield-line emitter: deck158 s34, s38, s40, s43.
Positive controls in the same prompts (annotation logic is correct, only the transformed-face
selection is wrong): `Brutal Cathar {2}{w} (3/3) (printed 2/2) [daybound]` (deck158 s32 --
genuinely buffed, Cathar is `subtype=Human Soldier Werewolf`),
`Sigarda ... (5/5) (printed 4/4) [counters: 1x +1/+1]` (s42),
`Luminarch Aspirant (2/2) (printed 1/1)` (deck105 s38).

**Sweep obligation per HL5:** enumerate every emitter of the `(printed ` string and quote the
post-fix output from each; the battlefield snapshot is the highest-traffic one and was missed.

---

## N-152h -- **LOW, cross-seat.** The retired MDFC wording survives on a THIRD card class: MDFC SPELLS.

Wave-31 h4 fixed MDFC LANDS; wave-32 i15 fixed TRANSFORM DFCs. Both are clean at this seat
(deck152 s10/s15/s19/s21/s26; deck105 s27/s28/s29; deck146 s15). The old string
`the Cast menu is where you cast, and it lists every face you can afford (the other face appears
there as an alternative-cost cast)` survives at **deck146's seat, seq36**, on `Emeria's Call`
(`Emeria's Call // Emeria, Shattered Skyclave`) -- and it is false there too, since that back face
is a LAND, not an alternative-cost cast. Third path, same string, same sweep.

---

## N-152j -- **MEDIUM.** Blockers parser rejects a compliant `BLOCKS:` head line written with CARD NAMES; both corpus `unparsed_reply` fallbacks are this one shape.

Both are opponent seats inside deck152's games, both `kind=blockers`, both fell back to the
heuristic.

`1785180803-ai_baka_deck139-...-vs-ai_baka_deck152.jsonl` **seq21 t11**, reply line 1 verbatim:

```
BLOCKS: Pollywog Symbiote blocks Elite Spellbinder
```

Offered: `B1. Pollywog Symbiote (1/3)` vs `A1..A5` (`A5. Elite Spellbinder (3/1) deals 3
[flying]`). Unambiguous -> `B1:A5`.

`1785180695-ai_baka_deck158-...-vs-ai_baka_deck152.jsonl` **seq35 t16**, reply line 1 verbatim:

```
BLOCKS: Orc army: Sigarda, Champion of Light; Dunland Crebain: Moonrage Brute
```

Offered: `B1. Orc army (1/1)` vs `A1. Moonrage Brute (3/3)` / `A2. Sigarda, Champion of Light
(5/5)`. `Dunland Crebain` was tapped and never offered -> resolves to `B1:A2`, unknown name
dropped, which is the existing illegal-assignment discipline.

The protocol was otherwise obeyed in both (correct anchor, first line, sound reasoning), so this
is parser coverage, not compliance. **Spec:** after the existing `B#:A#` scan fails, retry the
head line against the offered blocker/attacker NAMES (case-insensitive, unique-prefix match,
accepting both `NAME blocks NAME` and `NAME: NAME` separated by `,` or `;`), drop names that
match nothing or match ambiguously, keep first-wins for duplicates. Ship with the usual three
PARSETEST cases: a positive of each form, a negative that must NOT match (a name-shaped phrase in
prose after the head line), and the ambiguous-name drop.

---

## N-152g -- **LOW, OPEN, needs a probe. Do not act on the hypothesis.**

`Sigarda, Champion of Light {1}{g}{w}{w}` payable and NOT offered at **deck105 seq36 t16**:
sources `Hengegate Pathway {W}`, `Overgrown Farmland #1`, `Overgrown Farmland #2`,
`Lair of the Hydra {G}`, `Plains {W}` -- payable from MONO sources alone, so N-152f's mechanism
does not explain it. Offer list was `1. Cast Luminarch Aspirant {1}{w} (1/1)` / `2. Cast nothing
right now`. Model flagged it at seq28: *"Total cost 4. I have 5. I can cast it. So why isn't it in
the list? ... Okay, there is a contradiction."*

**Free discriminator already in the corpus:** Sigarda WAS offered and cast in this same game at
**seq14 t8**, when no Sigarda was on the battlefield; both absent windows have
`Sigarda, Champion of Light {1}{g}{w}{w} (4/4)` already in play. **Trigger predicate for the
probe: a legendary creature in hand while a same-named legendary is on your own battlefield.**
I searched `LegalActions.cpp` and found no such gate; `AIPlayerBaka.cpp:2902` is
planeswalker/battle-SUBTYPE-scoped and cannot reach a creature. If the suppression is deliberate
it is undocumented and it silently contradicts the core prompt's completeness guarantee (which the
model called out) -- so either offer the cast or carve the guarantee. Probe deck: 4x Sigarda +
GW lands, one pinned game, check whether a second copy is ever offered.

---

## Low-severity observations (no action requested, recorded for pattern-matching)

- **Transform-DFC hand-card display oscillates.** deck105, one model-issued `Flip Side` at s28
  and one at s31, but the hand line reads `Tovolar's Packleader (7/7)` at s29, `Tovolar's
  Huntmaster {4}{g}{g} (6/6)` at s30, `Packleader` at s32, `Huntmaster` at s33. Either an
  additional flip is firing from somewhere, or the displayed face is being reset by an unrelated
  event. Not diagnosed; both faces render coherently in themselves and no misplay traced to it.
  Note that the model DID take the cosmetic toggle twice despite the annotation's closing
  `Ignore this option` -- the wave-32 guide now forbids it in general terms (P3), but if the
  toggle is genuinely never useful for a transform DFC in hand, auto-passing it the way h5
  auto-passes toggle-ONLY windows would remove the temptation entirely.
- **h5 auto-pass holds**: 29 priority asks at this seat, **0 toggle-ONLY windows** (wave-30 had
  121; wave-31 had 0). Second consecutive corpus clean.
- **i13 prompt cost is tail-concentrated at this seat**: p50 7564 -> 7610 (+0.6%), p90 14102 ->
  15769 (+11.8%), max 17469 -> 23775 (+36%), wave-31 -> wave-32. No quality degradation observed
  (0 fallbacks, reply p50 flat at ~1.1k, 24/25 full attacker declarations). Worth a watch line
  only if a seat with a larger board starts truncating.
- **`Dungeons completed (opponent): 1`** renders correctly in this seat's prompts against deck146
  (s43) -- the conditional-line pattern N-152i part 1 should copy.
