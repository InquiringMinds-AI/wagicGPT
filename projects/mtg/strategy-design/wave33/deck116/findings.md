# deck116 "Time of Need" (GU cheat-into-play) — wave-33, FIRST GUIDED CORPUS (Step 0-ter)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260819-070739`, binary `bdb30d8e7`, 6 games,
**131 seat decisions**. **FALLBACKS: 0 / 131** — zero unparsed, zero defers, zero
`retracted_choice`, zero `truncated_abandoned`, zero `validation_reject_reask`, zero
`all_assignments_illegal`. Zero crash signatures in my six stderr files (the only `ERROR`
lines in any of them are 8 `ABILITYFACTORY Error parsing: {0}:doubleside(<...> pathway)` in
`game-152v116` — deck152's cards, deck152's lane).

Record **3/6**, up from 1/6.

| opp | result | end life | turn | creatures deployed | cheat arrivals |
|---|---|---|---|---|---|
| 139 | **WIN** (kill) | 16 / -1 | 13 | Emrakul, Piper, Pelakka Wurm | 3/3 |
| 152 | **WIN** (cap adj) | 21 / 15 | 15 | Thragtusk, Finks x2, Emrakul | 2/2 |
| 36  | **WIN** (cap adj) | 27 / 5 | 13 | Piper, Emrakul, Thragtusk, Pelakka Wurm | **2/3** |
| 146 | loss (cap adj) | 14 / 20 | 14 | Finks, Piper, Pelakka Wurm | 1/1 |
| 105 | loss (poison, at **23 life**) | 23 / 20 | 10 | Kitchen Finks | **0/2** |
| 158 | loss (kill) | -2 / 20 | 8 | **0** | n/a (no enabler landed) |

---

## HEADLINE

**The guide validates. Its target class — the absence table — collapsed: games ending with ZERO
creatures ever deployed went 3/6 -> 1/6, and the one survivor (vs158) drew no castable creature
all game (verified: every `Your hand:` line in that log across 10 records contains zero creatures
until Jin-Gitaxias {8}{U}{U} at s9). The deploy-floor leak the guide was written against is dead:
13 windows offered a BODY or an ENABLER, 12 were taken, and the one decline is CORRECT (vs139 s17
declined `Cast Elvish Piper {3}{g}` and put the same Piper into play with the Amulet at s18 for
{4} instead of {3}{G} plus the card).**

**Win attribution — the honest split.** Two of three wins are cap adjudications, so the record
alone is a coin-flip surface. Attribution by evidence rather than by count:

- **INTERIM CHANGES own the mulligan reversal outright.** i9's hand-only pregame plus the engine
  count header is the whole of it: **6/6 keeps at seven cards, ZERO mulligans in the corpus**
  (wave-32: 7 mulligans and a 0-card hand in vs158 alone, which lost that game). The guide's
  RULE #6 is a passenger; see the SUPERSEDED verdict below.
- **The guide owns the deploy floor and the hardcast delusion.** Neither is touched by any interim
  commit. Wave-32's *"accelerating my mana base significantly towards casting Ulamog"* class is at
  **zero occurrences in 131 replies**; the pilot now writes *"I cannot cast Emrakul normally (cost
  is too high), so this is the only way to deploy the win condition"* (vs105 s11) and *"I cannot
  cast Emrakul directly. My immediate goal is to establish an enabler"* (vs36 s11).
- **Neither owns the two adjudications.** vs152 and vs36 were ahead on board at the wall with
  Emrakul live (vs36 at 27-5). These are latency-lottery framings of positions the guide built.
  vs146 is the inverse — 14-20 at the wall with Pelakka Wurm just landed.
- **One win is confounded downward, not upward**: vs36 reached 27-5 *despite* a silent
  cheat-activation no-op (N-116g) costing it a turn.

**And the corpus produced a NEW, DECISIVE ENGINE DEFECT that outranks everything in the guide
lane: 3 of 11 cheat activations silently no-opped (N-116g). Both no-ops in vs105 were the deck's
entire win condition; that game is a loss to a broken engine seam, not to play.**

---

## (0) QUOTED-RENDER-STRING SWEEP (wave-32 HL3.1 — runs first). ZERO dead strings, 17/17.

Every substring the deployed guide presents as something the pilot will SEE, grepped verbatim
against the corpus `prompt` fields:

| quoted surface | corpus hits |
|---|---|
| `Put in Play with Elvish Piper targeting` / `... Quicksilver Amulet targeting` | 29 / 67 |
| `Glimmerpost (land)` | 47 |
| `(land: taps for {G})` / `(land: taps for {U}{G})` | 92 / 33 |
| `Mana available:` / `untapped sources` / `colours you can make` | 126 / 110 / 101 |
| `[does NOT qualify` | 399 |
| `Cast nothing right now` | 44 |
| `you kill it, your blocker lives` / `your blocker dies, attacker lives` | 9 / 2 |
| `Cast Kitchen Finks {1}{g/w}{g/w} (3/2)` | 4 |
| `[eligible]` / `Pick ONLY from the cards marked` | 9 / 9 |

Two surfaces GREW under the interim arc and the guide has not caught up — recorded here, fixed in
the revision:
- `Cast Kitchen Finks {1}{g/w}{g/w} (3/2)` now carries `[hybrid: each {g/w} pays with G or W -
  total 3 mana]` (4/4 renders). The guide's RULE #5 explains that same fact in prose. **SUPERSEDED.**
- The pregame ask is now hand-only with an engine count header (`Your hand (7 cards), counted by
  the engine: 3 lands, 4 spells.` / `Mana sources among those lands, counted by the engine: {U} 1,
  {G} 3.` / `Cheapest spell in this hand: ...` / `Playing every land in this hand would cover the
  cost of: ...`). RULE #6's five-line land enumeration is what that header now does. **SUPERSEDED.**

---

## (1) THE TAUGHT-CLASS FIRING TABLE (Step 0-ter rule 1 — reported before the win rate)

| # | teach | verdict | evidence |
|---|---|---|---|
| 1 | The five legends can never be cast; the FORBIDDEN THOUGHT | **FIRED** | 0/131 replies plan a hardcast or a ramp-toward-hardcast; 2 explicit correct statements (vs105 s11, vs36 s11). Wave-32's `vs36 s14` class is dead. |
| 2 | Enabler first, monster second | **FIRED** (1 protocol-side violation) | Fabricate cast only with no enabler in hand (vs139 s7, vs152 s7); Amulet cast the turn it became payable in every game it was held. Violation = vs158 s7, see (5). |
| 3 | How the cheat works; cast triggers do NOT fire | **FIRED** | 4 explicit correct statements (vs105 s11, vs146 s18, vs152 s11, vs36 s14). Wave-32's *"his 'When you cast' trigger resolves, drawing four cards"* is at zero. |
| 4 | Elvish Piper never attacks and never blocks | **MIXED** | FIRED at vs36 s13 (Piper the only blocker vs two 1/1s, both lines `(both die)`, answered `no blockers`). VIOLATED at vs146 s13 (`B2:A2`, Piper blocks a Goblin at 25 life) — but see (5): the reply's own reasoning says *"Piper is our win condition engine. We must save Piper"*. Zero Piper attacks in 7 attacker declarations. |
| 5 | Deploy a body every turn; a search spell is not a turn | **FIRED on its target class, MUTATED into a durdle** | Body/enabler offered 13x, taken 12x. But 7 of 19 `Cast nothing` windows are turn-3, 2-mana windows where a payable {1}{G} spell was the ONLY offer — see (4). |
| 6 | Count your lands before you mulligan | **SUPERSEDED** | 6/6 keeps, 0 mulligans, 0 "zero lands" claims (1 total hit, and it is a correct statement about the battlefield: vs152 s2 *"I have no lands in play yet"*). The engine header now carries the count. Delete the enumeration, quote the header. |
| 7 | What to search for | **FIRED, with one over-tight gate** | Fabricate->Amulet 3/3 correct; Time of Need->biggest legend 6/6 (Emrakul every time). **Farseek: offered in 36 windows, cast 0 times** — the demotion took completely. The gate *"cast Time of Need ONLY when an enabler is already on your battlefield or in your hand"* was violated 3x (vs105 s4, vs146 s5, vs36 s9), each time correctly: Time of Need was the best available search in a search-only window. |
| 8 | Trust the printed numbers | **FIRED, with a named successor** | 46 cost-payability claims across 131 replies; **3 name a card that WAS in the offer list** (vs36 s4, vs152 s24 x2) and **zero produced a false decline of a card the pilot wanted**. Successor belief in (6). |

---

## (2) N-116g (NEW, HIGH, DECISIVE) — 3 of 11 cheat activations SILENTLY NO-OP

Arrival-traced end to end, from my own event stream and from the opponent's independent log.
Mechanism isolation is in `notes.md`; the seat-level facts:

| seq | window | taken | narrated | ARRIVED |
|---|---|---|---|---|
| vs139 s15 / s18 / s24 | Main phase 1 | yes | yes | **yes x3** |
| vs146 s21 | Upkeep (my T14) | yes | yes | yes |
| vs152 s11 / s21 | Upkeep (my T10 / T14) | yes | yes | **yes x2** |
| vs36 s14 | Upkeep (my T10) | yes | yes | yes |
| vs36 s23 | Upkeep (my T14) | yes | yes | yes |
| **vs105 s11** | Upkeep (my T10) | yes | yes | **NO** |
| **vs105 s15** | Upkeep (opp T11) | yes | yes | **NO** |
| **vs36 s22** | Upkeep (opp T13) | yes | yes | **NO** |

In every failure the events narrate `- You: Put in Play with <enabler> targeting <creature>
[cost: ...]`, and then: the target is still in `Your hand` on the next prompt, `Your battlefield`
still reads `(creatures: 0)`, the enabler is **not** `[tapped]`, and `Mana available` does not
drop. Cross-check: the deck105-side log of the same game never contains
`Opponent's Emrakul, the Aeons Torn: hand -> battlefield` at all.

**Cost: the vs105 loss.** deck116 held Emrakul with the Amulet online at 8 poison, took the
activation, took it AGAIN the following upkeep, and died at 23 life with a 15/15 annihilator-6
body in hand and `creatures: 0` on its board. Both windows are on the ONE line that wins that
game. This is a REGRESSION against wave-32's 5/5 arrival-traced record on this class.

**Secondary damage (the plan-pollution face):** after the s11 no-op the pilot spent vs105 s13 and
s14 passing on the strength of its own PLAN field — *"The game state indicates that Emrakul has
already been put onto the battlefield via the Quicksilver Amulet activation during the Upkeep
phase of Turn 10 ('Your Plan' confirms this sequence occurred)"* — while the board line in the
same prompt read `Your battlefield (creatures: 0)`. The render was correct and the pilot preferred
its plan text. The guide gets a one-clause stopgap keyed to the board line.

---

## (3) THE REVEAL TAX (N-116a, KNOWN-OPEN — COUNTED, NOT RE-DIAGNOSED)

**9 whole-library reveals. Options offered 439; eligible 40. Ratio 11.0 : 1.**

| card | windows | options | `does NOT qualify` tags | eligible |
|---|---|---|---|---|
| Fabricate `target(<1>artifact|reveal)` | 3 | 50, 49, 50 | 47, 45, 46 | 3, 4, 4 |
| Time of Need `target(<1>creature[legendary]|reveal)` | 6 | 46, 51, 51, 47, 46, 49 | 41, 46, 46, 43, 41, 44 | 5, 5, 5, 4, 5, 5 |

Prompt cost unchanged and large: 12,535-24,984 chars per reveal prompt.

**The REPLY-side tax on reveals has collapsed and it is the guide's win.** Wave-32's reveal
replies ran to 13,796 chars / 220.6 s. Wave-33: **622-1,054 chars, 11-17 s, every one of the
nine.** RULE #7's closing line (*"Pick ONE card, from the lines marked [eligible]. Do not list
several."*) plus the answer-first protocol reduced a 3.7-minute decision to a 14-second one.
**Picks: 9/9 legal and 9/9 correct** (Amulet x3; Emrakul x6 — the largest annihilator every time).

**N-116d (subset framing / over-selection): 0 occurrences — UNEXERCISED, not fixed.** All nine
reveals came from `<1>`-tokened effects. The un-tokened effect in this deck is Farseek, and the
pilot cast Farseek zero times in 36 offered windows. **The guide suppressed the card that
exercises the defect.** Do not read a zero here as evidence the render changed; the corpus cannot
speak to it. (This is the fixture-rides-the-broken-branch lesson wearing a guide costume: the only
path to the defect was closed by a teach.)

---

## (4) THE MUTATED CLASS — the turn-3 DURDLE (the revision's #1 target)

deck116 contains **zero instants and zero sorcery-speed removal**; the only flash card is
Jin-Gitaxias {8}{U}{U}. Per HL10 the legitimate-hold branch is structurally empty, so the RAW
`Cast nothing` rate is the finding, not the split.

**19 of 44 cast windows answered `Cast nothing right now`.** Decomposed:

- **11 — settled board** (Emrakul already deployed and lethal, or believed so): vs139 s14/s19/
  s22/s23/s25, vs105 s13/s14, vs152 s29, vs36 s15/s16/s25. Harmless.
- **1 — correct decline** against the floor: vs139 s17 (Piper cast declined; the same Piper put
  into play with the Amulet at s18). *The guide's absolute did not license this correct play.*
- **7 — THE DURDLE. Every one is turn 3, `Mana available: 2 total`, and the ONLY offers are
  payable {1}{G} spells**: vs158 s4 + s5, vs146 s4, vs152 s4 + s5, vs36 s4 + s5.

The mechanism is verbatim, and it is the guide's own headline read as a ban:

> vs36 s4: *"My plan prioritizes getting an Enabler or a Creature onto the battlefield
> immediately. **None of the available spells (Farseek, Explore, Time of Need) are Enablers or
> Creatures.**"* -> `Cast nothing`.
> vs158 s5: *"Casting a search spell like Time of Need or Farseek now would leave me with no
> [mana] ..."* -> `Cast nothing`.
> vs36 s5: *"casting a non-threat spell is inferior to **saving mana for next turn's**
> creature/enabler drop"* -> `Cast nothing`.

RULE #5's own last line already says *"Cast a search spell only when the window offers NO creature
and NO enabler you can pay for"* — exactly this situation — and it loses to the headline. This is
HL8 rung 2's shape (the SAFETY argument — "nothing is urgent, save the mana" — is the one no floor
refutes) plus Step 0-ter rule 3 (a first revision tightens an over-broad rule). It is not free:
vs158 spent both of its turn-3 windows this way and died on turn 8 with four lands, having cast
its first spell on turn 5.

---

## (5) COMMIT-COUNTER FIELDS, and the ANSWER-FIRST SPECIMENS (owner directive 1 — EVIDENCE ONLY)

**Counted fields, deck116, 131 decisions:**

| field | value |
|---|---|
| replies with **no `PLAN:` line** (protocol-compliant per `f46dd58ee`) | **4** |
| replies whose text continues past their own `PLAN:` line (`post_plan_overrun` > 0) | **14** (10.7%) |
| of those, `commit_retracted` | **0** |
| of those, FATAL | **0** — every first-line answer stood and was executed |
| total post-PLAN overrun chars | **14,025** (max 3,353 at vs36 s17) |

**Instrument caveat, and it is large: `post_plan_overrun` measures the wrong boundary for this
seat.** Post-**ANSWER** text (everything after the committed `CHOICE:`/`BLOCKS:`/`ATTACK:` first
line) totals **263,247 chars — mean 2,009/reply, matching the brief's corpus-wide ~1,900** —
against the 14,025 the field counts. The field sees **5.3%** of the waste. The two largest spirals
at this seat register `post_plan_overrun = 0` (vs152 s3: 14,206 post-answer chars, field = 262
because the reply writes `Plan:` last and lowercase; vs139 s3: 13,326 post-answer chars, field = 0,
no PLAN line at all).

**Decision-divergence rate: 5 / 109 multi-option decisions (4.6%)** where the last option named in
the reply differs from the committed one — matching the corpus-wide 4.2%.

**(b) Does the post-answer self-correction carry decision VALUE?** At this seat: YES twice, NO
once, and it only materialises when the model happens to emit a second `CHOICE:` line.

1. **vs158 s7 — the strongest specimen in my seat, and it lost material.** First line:
   `CHOICE: 4 (Cast Fabricate {2}{u})`. The next ~5,700 characters are a complete, correct
   derivation that quotes the guide twice — *"The Strategy Guide explicitly states: 'DO NOT cast
   Fabricate to search for a Quicksilver Amulet when a Quicksilver Amulet is ALREADY IN YOUR HAND.'
   Therefore, casting Fabricate to find another Amulet is inefficient"* — and closes *"So **Time of
   Need** is the choice ... So I will cast **Time of Need**."* It never re-commits. Fabricate
   resolved; s9's hand holds **two** Quicksilver Amulets; the game was lost on turn 8.
   `post_plan_overrun = 0`. **A reviewer who did not read the discarded scratch would have scored
   RULE #2 as FAILED and rewritten a rule that worked perfectly.** That is owner-directive evidence
   (c) in its purest form.
2. **vs146 s13 (Blockers, 82 s).** First line: `BLOCKS: B1:A1, B2:A2` — the Elvish Piper blocks.
   The reasoning then works out the deathtouch and persist interactions correctly and concludes
   *"Losing Elvish Piper is catastrophic for your game plan (no more cheating in Emrakul) ...
   **Piper is our win condition engine. We must save Piper.**"* It never re-commits. Piper blocked
   and died. `post_plan_overrun = 0`. This is the seat's only RULE #4 violation, and **the rule was
   not the failure — the ordering was.**
3. **vs152 s3 (Land drop, 209 s, 14,240 chars, 4 options).** First line: `CHOICE: 3 (Play
   Hinterland Harbor)`. The reasoning immediately catches that Hinterland Harbor enters TAPPED on
   an empty board and ends *"I will choose **Island**."* Never re-commits. Minor tempo cost.
4. **vs152 s24 — the counter-example that shows the value is REAL but unreliable.** First line
   `CHOICE: 4 (Cast nothing right now)`; the reply then works out that Kitchen Finks IS payable and
   emits a SECOND `CHOICE: 3 (Cast Kitchen Finks ...)`. The parser honoured the LAST one
   (`choice: 3`) and the Finks was cast. The self-correction improved the decision — and it is the
   only one of the five that was mechanically capable of doing so.
5. **vs36 s14 — negative value.** Committed Piper->Emrakul (correct); the reasoning drifted to
   Piper->Thragtusk. The commit protected the better answer.

Summary for synthesis: **deck116 contributes ZERO fatal post-PLAN retractions to the 2-seat
promotion trigger**, and contributes instead the finding that at this seat the destructive pattern
is *post-ANSWER, pre-PLAN* and therefore invisible to the shipped counter. Three decisions were
committed against the reply's own concluded reasoning; two of those three conclusions were
correct; one of the two lost the game's key card.

---

## (6) TAX TABLE AND LATENCY (the cap-adjudication evidence)

Reply length p50 **1,012** / mean 2,041 / max 14,240; replies > 8k: **4**. Latency p50 **16 s** /
mean 30 s / max 209 s. Total seat model time across six games: **4,048 s**.

**The tax RELOCATED off the reveals and onto LAND DROPS.** Top six rows, low-branching first per
HL9:

| rank | seq | window | opts | chars | latency |
|---|---|---|---|---|---|
| 1 | vs152 s3 | **Land drop** | 4 | 14,240 | **209 s** |
| 2 | vs139 s3 | **Land drop** | 3 | 13,349 | **201 s** |
| 3 | vs146 s18 | Cast | 3 | 11,333 | 175 s |
| 4 | vs152 s17 | Cast | 3 | 10,832 | 155 s |
| 5 | vs36 s17 | Blockers | **1** | 7,936 | 115 s |
| 6 | vs152 s7 | Cast | 4 | 7,008 | 102 s |

Rows 1, 2 and 5 are the shape that matters: a 4-option land drop and a **1-option** blocker window.
Reading rows 1 and 5: neither is mana doubt (i12 is holding — see (1) #8); both are the model
re-deriving a decision it has already answered. **The successor belief at the land-drop seam is
Hinterland Harbor's enters-tapped condition** — vs152 s3 spends its first thousand characters on it
and gets it right, then spends thirteen thousand more.

**Cap evidence.** My three cap-adjudicated games are the three longest, and the wall is joint:

| game | my decisions / my latency | opponent decisions / latency | joint model time |
|---|---|---|---|
| vs146 (adj, LOSS 14-20 @T14) | 23 / **1,027 s** | 41 / **1,826 s** | **2,853 s** of a 3,000 s cap |
| vs152 (adj, WIN 21-15 @T15) | 31 / **1,073 s** | 44 / **1,822 s** | **2,895 s** |
| vs36 (adj, WIN 27-5 @T13) | 26 / **659 s** | 42 / **1,910 s** | **2,569 s** |

In vs152, **four decisions (s3, s7, s17, s24) consume 562 s — 52% of my entire game budget** out of
31 decisions. In vs146 two decisions consume 276 s of 1,027 s. The cap is not being reached by long
games; it is being reached by a handful of spirals inside ordinary-length games, and at this seat
every one of those spirals is post-answer text. Correctness was intact in all of them (zero
retractions, zero fallbacks).

---

## (7) i1 / i2 — THE POISON PAIR AT MY SEAT

**i1 VALIDATED, 100%.** Every prompt from the first poison event to the end of the vs105 game
carries both status lines: **10/10 at my seat, 12/12 at deck105's seat of the same game.** Form:

> `Poison counters (you): 8 of 10 - you LOSE the game at 10 poison counters, whatever the life
> total is; 2 more end it. Poison is not life: it does not reset between turns, and gaining life or
> preventing damage does not remove a poison counter.`

Narration is distinct from ordinary damage on every source, including the non-combat Ichor Rats
line: `- Infect damage: 2 from Ichor Rats to you - dealt as POISON COUNTERS, not life loss: no life
was lost (see the Poison line)`. **The wave-32 four-silent-events class is dead at this seat.**
Wave-32's diagnostic — a grep for `poison` returning only the pilot's own hand-tallies — now
returns **39 hits, all reading a rendered number**.

**i2 VALIDATED, 1/1, with its negative control clean.** The seat's single infect blocker window
(vs105 s8) prints the partitioned forecast and **zero** life arithmetic:

> `Unblocked, these attackers put up to 3 POISON COUNTERS on you - you would be at 5 of 10 poison -
> that is not yet 10, but poison counters never reset and nothing here removes them, so every
> counter you take is permanent progress toward losing.`

and each attacker line carries `[its combat damage to YOU is dealt as POISON COUNTERS, not life
loss - your life total will not move and blocking is the only thing that stops it]`. **The
prediction holds: zero `you would be at N - NOT lethal` emissions on the infect window.** All eight
non-infect blocker windows at this seat still carry the ordinary forecast, unchanged.

**The wave-32 chump-block class did NOT recur — and it was NOT TESTED. Report it as UNEXERCISED,
not as fixed.** Elvish Piper never reached the battlefield in the vs105 game (0 Piper-block offers
against infect). The one infect block available was `Kitchen Finks (3/2) [persist] - may block A1
(you kill it, your blocker lives (infect shrinks your blocker to 2/1))`, a value block, correctly
taken. The discriminator wave-32 identified (engine card + invisible poison) was not sampled. The
rendered count did visibly change the pilot's *reasoning* — vs105 s8 prices the two branches in
poison counters, and vs105 s11/s15 both open with *"I am at 8 poison counters and need to win
immediately"* — but that is comprehension evidence, not the repro.

---

## (8) PRESERVE LEDGER (HL9 rung 1 — unconditional, run before writing a line of revision)

Frozen. The revision legislates over none of it.

1. **Cheat activation take rate 11/13 offered windows**, and both `pass` answers are correct
   (vs152 s14 held the Amulet rather than cheat in a Kitchen Finks; vs36 s18 held rather than cheat
   in a Thragtusk while Emrakul was already attacking).
2. **Cheat target selection 11/11 correct** — the biggest available body every time, including
   Emrakul over Pelakka Wurm (vs105 s15) and Emrakul over Thragtusk and Pelakka Wurm (vs36 s14).
3. **Tutor picks 9/9 legal and 9/9 correct** off 46-51 option lists, at 14 s each.
4. **Attackers: 7 declarations, 0 durdles, 0 illegal names.** The one `ATTACK: none` (vs146 s12) is
   scored against the eligible set per Step 0-ter rule 2: the sole eligible attacker was
   `A1. Kitchen Finks (2/1) [persist]` into an untapped `Triumphant Adventurer (1/1) [deathtouch]`.
   Correct. Piper never declared as an attacker in any window.
5. **Blocks: 9 windows, 8 correct**, including the hard decline (vs36 s13). The ninth is vs146 s13,
   which the reply itself got right (see (5)).
6. **Zero fallbacks in 131 decisions**; zero illegal or out-of-range answers; every `BLOCKS:` line
   parsed (i8 / N-152j holding — both name-form and `B1:A1` index form appear and both parse).
7. **Mulligan 6/6 keeps, zero false mulligans, zero `Mana available: 0 total` echoes.**

---

## (9) CARD-FACT LEDGER (HL2 — terminal pass over the guide being SHIPPED)

Every checkable claim in the revised `strategy.txt`, read against
`Res/sets/primitives/mtg.txt` and `Res/ai/baka/deck116.txt` this session.

| claim as written | adjudicating line | verdict |
|---|---|---|
| Elvish Piper {3}{G} 1/1; `{G}, Tap` puts a creature from hand into play | `mana={3}{G}` `power=1 toughness=1` `auto={G}{T}:moveTo(myBattlefield) target(creature|myhand)` | TRUE |
| Quicksilver Amulet {4} artifact; `{4}, Tap` same | `mana={4}` `type=Artifact` `auto={4}{T}:moveTo(myBattlefield) target(creature|myhand)` | TRUE |
| Kozilek {10} 12/12 annihilator 4; draw 4 on CAST ONLY | `mana={10}` `power=12` `autostack=if casted(this) then draw:4` `auto=_ATTACKING_...sacrifice 4 permanents` | TRUE |
| Ulamog {11} 10/10 indestructible annihilator 4; destroy on CAST ONLY | `mana={11}` `power=10` `abilities=indestructible` `autostack=if casted(this) then destroy` | TRUE |
| Emrakul {15} 15/15 flying, protection from coloured spells, annihilator 6; extra turn on CAST ONLY | `mana={15}` `abilities=nofizzle,flying,protectionfromcoloredspells` `autostack=if casted(this) then turns:+1` `sacrifice 6 permanents` | TRUE |
| Jin-Gitaxias {8}{U}{U} 5/4 | `mana={8}{U}{U}` `power=5 toughness=4` | TRUE |
| Sheoldred {5}{B}{B} 6/6; reanimates on YOUR upkeep, opponent sacrifices on THEIRS; no black source in the deck | `mana={5}{B}{B}`, two `auto=@each ... upkeep` lines; decklist lands = Tropical Island / Hinterland Harbor / Glimmerpost / Forest / Island | TRUE |
| Time of Need {1}{G} finds a legendary creature | `mana={1}{G}` `target(<1>creature[legendary]|reveal)` | TRUE |
| Fabricate {2}{U} finds an artifact; the Amulet is the deck's only artifact | `mana={2}{U}` `target(<1>artifact|reveal)`; decklist artifacts = `Quicksilver Amulet * 4` | TRUE |
| Farseek {1}{G} cannot find a Forest; puts the land in TAPPED; does NOT draw | `target(plains,island,swamp,mountain|...)`, `and!(tap(noevent))!`, no `draw:` | TRUE (Tropical Island qualifies via `subtype=Forest Island`) |
| Explore {1}{G} = extra land + draw 1 | `auto=maxPlay(land)+1` `auto=draw:1` | TRUE |
| Urban Evolution {3}{G}{U} = draw 3 + extra land | `auto=draw:3` `auto=maxPlay(land)+1` | TRUE |
| Kitchen Finks {1}{GW}{GW} 3/2 persist, ETB gain 2 | `mana={1}{GW}{GW}` `power=3 toughness=2` `abilities=persist` `auto=life:2` | TRUE |
| Thragtusk {4}{G} 5/3, ETB gain 5, LTB 3/3 Beast token | `mana={4}{G}` `power=5 toughness=3` `auto=life:5 controller` `auto=@movedTo(this|nonbattlezone) from(battlefield):_BEASTTOKEN_` | TRUE |
| Pelakka Wurm {4}{G}{G}{G} 7/7 trample, ETB gain 7, dies->draw 1 | `mana={4}{G}{G}{G}` `power=7 toughness=7` `abilities=trample` `auto=life:7` `auto=_DIES_draw:1` | TRUE |
| Glimmerpost is a real land, taps for {1} colourless, gains 1 life per Locus | `type=Land subtype=Locus` `auto={T}:Add{1}` `auto=life:type:locus:battlefield controller` | TRUE |
| Hinterland Harbor enters untapped only if you control a Forest or Island | `auto=aslongas(forest,island|myBattlefield) tap(noevent) <1 oneshot` | TRUE |
| 24 lands, no rituals; 20 creatures | decklist `#24 lands` (4+4+4+9+3 = 24); creatures 4+4+4+3+5 = 20; zero rituals | TRUE |
| Persist returns Finks as a 2/1 with a -1/-1 counter, once | `abilities=persist`; observed live at vs146 (`Kitchen Finks (2/1) (printed 3/2) [persist] [counters: 1x -1/-1]`) | TRUE |

**Class sweep (HL2 rung 3):** the corrected class this wave is *"the option line already carries
the fact"* — grepped the whole shipped guide for every place it re-explains a rendered annotation.
Two hits, both deleted: the Kitchen Finks hybrid explanation (render:
`[hybrid: each {g/w} pays with G or W - total 3 mana]`) and the RULE #6 land enumeration (render:
the pregame count header). No other sentence in the guide re-states a printed annotation.

**PERSISTENCE check:** no card the revised guide names as a body self-bounces or self-sacrifices.
Kitchen Finks persists and is stated positively. Elvish Piper is named as the one body forbidden
from combat, and the reason given is preservation, not a card-text hazard.

---

## (10) VERDICT AND ROUTING

- **Guide: REVISED** (`strategy.txt` in this directory is complete, 132 lines; wave-32 shipped 121). Step 0-ter forbids
  treating a revised first-guided seat as a rotation candidate; re-validate next corpus, focused on
  vs158/vs146 (the durdle matchups) and on vs105 once N-116g is fixed.
- **#1 slot goes to the durdle** — the seat's largest measured leak (7 wasted development windows)
  that is neither engine nor construction. Per HL9 rung 3 the loss routing is stated explicitly and
  is auditable: **vs105 -> ENGINE (N-116g)**; **vs158 -> GUIDE (durdle) + draw variance (zero
  castable creatures drawn all game)**; **vs146 -> protocol (the s13 commit ordering) plus the cap
  wall**.
- **Engine/harness -> `notes.md`**: N-116g (HIGH, new, decisive), N-116h (plan field outranks the
  board snapshot), N-116i (`{1}` vs `{c}` in the colours field), N-116j (poison aggregate line
  precedes its per-source causes), N-116a/N-116d counted per the brief, `pruned_pairs` never
  emitted at this seat.
- **Skill -> `skill.md`**: three proposals.
- **Core -> `general-suggestions.md`**: **PASS, 28th consecutive.** Evidence-only contribution to
  owner directive 1.
- **Construction: no routing.** N-116e's wave-32 observations stand unchanged; the 1-of Eldrazi
  absent-engine-card risk again did NOT materialise — Time of Need hit 6/6 and Emrakul was in hand
  in five of six games. The deck's curve deficit (cheapest creature at 3) is real and now has a
  cleaner witness (vs158's turn-8 death), but it sits BEHIND the durdle: in that game the pilot had
  payable spells and cast none.
