# Wave-50 engine ledger — THE WAVE-51 DOCKET (engine / UX only)

Corpus `matchups-20260827-115759` (seventh fair-hand corpus), binary master **67aeffe45** (lanes V
d9144d62c, Z 5b6b44c44, X c37805358, Y 7858a403f, W b39b80375 merged in that order; PARSETEST
1930/0; suite 1142/0 + 39 AI/0 expected). 42 seat logs, **1,864 records**, **1,780 decisions**
(ask 1,214 / priority 376 / attackers 102 / blockers 37 / discard 28 / reveal 15 / bottom 8),
**21/21 games natural — 42/42 seats carry `gameend`** (verified on disk by this step over every
JSONL: 42 `gamestart`, 42 `gameend`, 3 `fallback` fields). The `seat-125-126.md` header's
"deck123-vs-deck162 dead-loop game is excluded from every corpus-wide number" is a stale-note
artifact for the second wave running (this run's pair is `1787849886-...deck123-0x55de8e828b00-vs-
...deck162.jsonl`, 43 records, natural `gameend` at turn 13, 0 to 21) and is **DROPPED**; its
numbers are per-seat and unaffected. Corpus root:
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260827-115759/`. Seat names `deckA-vs-deckB` = the deckA
seat's JSONL; `seq` is the record's own field; the JSONL `turn` field is the narration's turn minus
one (D18) — seat files quote the field, this ledger quotes the field.

**Health.** Baka-executed fallbacks **3 / 1,780 = 0.17%**, all `stale_echo`, all deck126, all one
shape (D3). `answer_replaced` **5** (all audited, all correct self-revisions). `commit_retracted`
0. `parse_note` **10**: `attack_last_line_taken` 5, `repeat_count_under_two` 3, `blocks_last_line_
taken` 2. `[RE-ASK]` rendered **0** times; none of lane Y's new notes fired. `mana_only_windows_
skipped` max **203** (was 265). Max priority windows in one turn **10** (was 16). Max prompt
**25,793** (a priority record, `deck126-vs-deck146` seq 52, 63% log); max ask 25,018. `- Paid`
receipts **396** by the events field (357 casts, 39 activations). Repeat row offered 18, taken 9.
Latency max 587 s (a 10 KB reply on an 8.7 KB prompt — deliberation, not size).

**Ownership (owner doctrine 2026-08-26).** Engine/UX only below. Guide content is the reviewers';
guide-side halves are named only where a docket item retires one. Hard caps on the model's legal
choices are rejected; every item adds a true token, a true row, a receipt or one re-ask, and
deletes nothing. Card-behaviour defects go CARD-SCRIPT FIRST (owner rule: express the Oracle
faithfully in script, engine only if the script cannot), verified rules-correct for the general
case.

---

# THE DOCKET, RANKED

## D1 — HIGH — Soul Shatter resolves with NO sacrifice when a non-creature outranks every creature by MV; the script also never takes a planeswalker — **IN FLIGHT (lane A, `~/Projects/wagicGPT-w51-a`)**
(= engine seat **D-1 HIGH** / seat-146-152-162 **E-1 HIGH** / `general-strategy.md` **R72**)
**Repro.** `1787849903-ai_baka_deck146-0x5568f6f12330-vs-ai_baka_deck162.jsonl` seq **25** (turn
12, 7 life): `Cast Soul Shatter {2}{b} {leaves 4 of your 7 ...}` against `Master of the Feast
{1}{b}{b} (5/5) [flying]; Ob Nixilis, the Hate-Twisted {3}{b}{b} [planeswalker] [counters: 1x
loyalty]` -> seq **26** events `- Paid {2}{b} for Soul Shatter with Shineshadow Snarl #2, Swamp #1,
Swamp #2 / - You cast Soul Shatter / - Your Soul Shatter resolved and went to your graveyard` —
nothing else (re-read on disk by this step). Opposite seat `1787849903-ai_baka_deck162-
0x5568f824bdc0-vs-ai_baka_deck146.jsonl` seq 16 narrates the same. stderr `game-146v162-
1787849900.stderr` line 1363 `Resolving Action on stack: Soul Shatter` is followed directly by the
next window; the working trace `game-146v152-1787849904.stderr` (`146 vs152` seq 24 -> 25, Intrepid
Adversary sacrificed) reads `Resolving ... / ActionLayer::doReactTo 140 / CHECKING Spell /
AIPlayerGPT: targeting with -> 1 target(s) ... / Action added to stack: StackAbility`. Third case
`146 vs123` seq 28 (Bloodline Keeper sacrificed). Both working boards had no planeswalker.
**Mechanism (reconciled against source by this step, not stepped in a debugger).** Primitive
`bin/Res/sets/primitives/borderline.txt:106093`: `sacrifice notaTarget(creature[manacost=
convertedcost:highest:*:myBattlefield]|myBattlefield)!$ opponent`. `src/WParsedInt.cpp` 386-438:
`convertedcost:highest:<type>:<zone>` builds a `TargetChooser` over `<type>|<zone>` (here
`*|myBattlefield`) and takes the highest `getConvertedCost()` over EVERY card that chooser can
target — so Ob Nixilis (MV 5) sets `highest = 5`, the outer filter `creature[manacost=5]` matches
nothing, the sacrifice has an empty candidate set, and the ability resolves as a no-op. This is
seat-146's hypothesis, confirmed by reading the ranking code; the engine seat's alternative (lane
W's `ownChooser()` / hold-while-unresolved gate) is not needed to explain the three cases and lane
W's own prediction (`none was a legal target` at a foreign chooser) held 0/15. Second defect, same
card: Oracle (and the primitive's own `text=`) reads "a creature OR PLANESWALKER with the highest
mana value among creatures and planeswalkers they control" — the script's sacrifice class is
creature-only (`146 vs162` seq 21 planned it for Ob Nixilis; `146 vs123` seq 20 cast it into a
board of lands).
**Ask.** Card-script first: rank over `creature,planeswalker:myBattlefield` and sacrifice from
`creature,planeswalker[manacost=...]`; if the filter grammar cannot express a two-type class with
the same-type ranking, the engine half is a `convertedcost:highest` that honours a multi-type
`<type>`. Suite pins: (a) walker MV 5 + creature MV 3 -> the walker is sacrificed; (b) creature MV
3 alone -> sacrificed; (c) creature MV 5 + walker MV 3 -> the creature; (d) enchantment MV 6 +
creature MV 3 -> the creature (the non-creature/non-walker must not set the bar). Audit siblings:
every primitive using `convertedcost:highest:*` or `:lowest:*` with a narrower outer class.
**Locus.** `borderline.txt:106093`; `src/WParsedInt.cpp:386-438`; the /wagic-cardscript loop.
**Wave-51 prediction.** Edict/sacrifice takes whose next events carry no victim line: **0/N**
(was 1/13); planeswalker sacrificed when it is their highest-MV creature-or-walker: N/N; pins
(a)-(d) green.

## D2 — HIGH — generic pips are paid from dual lands while a mono source sits untapped: 69/386 receipts — **IN FLIGHT (lane B, `~/Projects/wagicGPT-w51-b`)**
(= engine seat **D-2 HIGH by count** / seat-146-152-162 **E-3** / deck152 G-1 / **R73**; wave-49 D18's
receipt made it adjudicable and lane T's D4 ordering FAILS by it)
**Repro.** `1787849886-ai_baka_deck162-0x55de8d50c1f0-vs-ai_baka_deck123.jsonl` seq **5** `Those
sources: Drowned Catacomb #1 {u} or {b}; Drowned Catacomb #2 {u} or {b}; Swamp {b}` -> seq **6**
`- Paid {2} for Howling Mine with Drowned Catacomb #1, Drowned Catacomb #2` (Swamp untapped; re-read
on disk), seq **8** the same with Swamp #1 and Swamp #2 untapped; `1787849891-ai_baka_deck125-
0x...-vs-ai_baka_deck152.jsonl` seq 34 -> 35 `Paid {w} for Path to Exile with Seachrome Coast`
(two Plains untapped); `1787849907-ai_baka_deck152-0x55dbebc4b720-vs-ai_baka_deck146.jsonl` seq
6/9/16 `Paid {1}{w} ... with Deserted Beach #1, Deserted Beach #2` (Plains untapped); `1787849883-
ai_baka_deck152-...-vs-...deck123.jsonl` seq 15 -> 16 `Paid {1}{w} for Intrepid Adversary with
Plains #2, Overgrown Farmland #2` (Plains #1 untapped). Engine seat's exact-matching method over
all 386 receipts: 78 used more multi-colour sources than the minimum, 69 on costs with generic
pips; by deck 125 22, 152 17, 146 14, 123 12, 162 10, 126 3. Seat-146's land-only heuristic:
33/146 on its seats — consistent. No colour strand was observed this corpus (latent cost), but
the shape strands colours in exactly the response windows counters live in.
**Ask.** `selectAutoTapProducers` ordering: generic pips from mono sources first, coloured pips
from the scarcest colour among the untapped set; the receipt is the test. Suite pin: `{2}` with
`Catacomb, Catacomb, Swamp` untapped -> `Swamp` is in the receipt.
**Locus.** lane T's auto-tap ordering (a2fd9684a) as exercised by `AIPlayerBaka::payTheManaCost`.
**Wave-51 prediction.** Receipts using more multi-colour sources than the exact minimum over the
untapped set: **0/N** (was 78/386; 69/386 on generic pips).

## D3 — HIGH — an in-range index with an off-menu name is classified `stale_echo` and falls to Baka with NO re-ask (3/3 of the corpus's fallbacks; Baka cast the wrong card in 2)
(= seat-125-126 **HIGH #1** / deck126 G1 / engine seat **D-4** / **R74**; skill #151 guide half)
**Repro.** `1787849909-ai_baka_deck126-0x5601b9f034e0-vs-ai_baka_deck125.jsonl` seq **14**: menu
`1. Cast Overgrown Battlement {1}{g} (0/4) ... 2. Cast nothing right now`; reply `CHOICE: 1 (Cast
Sanguine Bond)` (Bond in hand, `{B} 1` on the mana line); record `choice: -1, chosen_text: "",
fallback: stale_echo, coded_answers: 1`; next events `- Paid {1}{g} for Overgrown Battlement`
(Baka cast the wall). The prior reply was `CHOICE: 1 (Play Savannah)` — nothing was echoed.
`1787849911-ai_baka_deck126-0x55fc821cb030-vs-ai_baka_deck162.jsonl` seq **16** (`CHOICE: 1 (Cast
Sanguine Bond)` on a Lantern/nothing menu; Baka cast the Lantern; prior reply `PUT: 31 (Sanguine
Bond)` gave the echo heuristic a hook); `1787849923-ai_baka_deck126-0x56221e2c00d0-vs-ai_baka_
deck130.jsonl` seq **13** (`CHOICE: 1 (Cast Exquisite Blood)` on a Tribute-at-0/nothing menu).
**Ask.** `parseChoice` (`AIPlayerGPT.cpp` ~13565: `echoNoMatch && choice > 0 &&
echoStaleForIndex(choice)` -> −1) routes an in-range index whose parenthetical names no row to the
`named_row_reask` path (lane S, wave 49: 1/1 recovered) with the reason the render already has:
`[RE-ASK] "Sanguine Bond" is not on this list. It is on your hand line but has no "Cast" row this
window: its {3}{b}{b} needs two {B} sources and your mana line counts {B} 1. Answer with a number
from 1 to 2.` The `stale_echo` label stays only for a genuine echo of the previous window's row.
PARSETEST: the seq 14 reply on its menu (expect re-ask, not −1); a true echo (expect `stale_echo`).
**Wave-51 prediction.** Baka-executed `stale_echo` on a reply whose index is in range: **0/N**
(was 3/3); every off-menu name -> one `[RE-ASK]`, ≥ 50% recovered.

## D4 — MED — a named repeat count of 0 is executed once; a CHOICE count that contradicts the PLAN line's `this window: pass` executes the count
(= seat-123-130 **H1** + **M1** / engine seat **D-3** / deck123 G2 / **R75**; skill #133, #134 guide halves)
**Repro (a).** `1787849886-ai_baka_deck123-0x55de8e828b00-vs-ai_baka_deck162.jsonl` seq **35**:
`CHOICE: 2 (Create human with Thraben Doomsayer x0) / PLAN: Stop at M = 25. M is 26 now. Pass.` ->
seq 36 `- You used: Create human with Thraben Doomsayer - ran 1 time (you named 0)`, `parse_note:
repeat_count_under_two`, M 26 -> 27.
**Repro (b).** `1787849892-ai_baka_deck123-0x5566744161e0-vs-ai_baka_deck130.jsonl` seq **31** (x1
under "this window: pass"), seq **46** (x3 under "pass (stop reached)"); `1787849921-ai_baka_
deck123-0x556aeb0dee00-vs-ai_baka_deck126.jsonl` seq **32** (x25 under "This window: pass"; M 33
-> 58, 25 past a stop of 30). The answer-first protocol writes the count before the subtraction.
**Ask.** (a) count 0 on the repeat row = the pass row (or one `[RE-ASK]` naming it), never floored
to 1; `x1` stays a single. (b) Proposal, not a defect: PLAN line contains `this window: pass` /
`stop reached` and the CHOICE line names N > 0 on a repeat row -> one `[RE-ASK]` quoting both
lines (`plan_choice_conflict`). PARSETEST: the seq 35 reply (expect pass), the seq 32 reply
(expect re-ask), an `x1` (expect single).
**Wave-51 prediction.** `ran 1 time (you named 0)`: **0/N** (was 1); conflicts executed without
a re-ask: 0/N (was 4).

## D5 — MED — the `- Paid` receipt is missing on 3 of 9 mana-cost land animations (the instrument for D2 and the D1-regression detector has a hole)
(= seat-146-152-162 **E-2** / deck152 G-4 / **R76**)
**Repro.** `1787849885-ai_baka_deck146-0x5620ff65a3b0-vs-ai_baka_deck126.jsonl` seq **32**, **36**
events `- You used: becomes beholder with Hive of the Eye Tyrant` with no `- Paid {3}{b}` line
(re-read on disk); `1787849889-ai_baka_deck152-0x55988eb1a020-vs-ai_baka_deck126.jsonl` seq **25**
`becomes a 1/1 hydra with Lair of the Hydra`, no receipt; contrast `1787849891-ai_baka_deck152-
0x5572f67d9140-vs-ai_baka_deck125.jsonl` seq 17/45/59/60/73/74 `- You used: becomes ... / - Paid
{1}{g} for Lair of the Hydra with Overgrown Farmland #1, Deserted Beach` (re-read). Both Hive
misses are Upkeep windows; the Lair miss is a `[this land is TAPPED]` row. Suspect (inference): the
receipt is emitted from the auto-tap path, and a `becomes` ability whose own land is part of the
payment (or is already tapped) takes a different payment path.
**Ask.** One receipt per committed mana payment regardless of path; suite pin: Hive animation in
Upkeep -> `- Paid {3}{b} for Hive of the Eye Tyrant with ...` present.
**Wave-51 prediction.** `- Paid` on every activation with a mana cost: N/N (was 6/9 on land
animations; 144/144 casts).

## D6 — MED — activated-ability damage rows print no `{right now: takes N damage - DIES / SURVIVES (toughness T)}`; spell target menus do (0/49; two wasted shots at 4 life)
(= seat-123-130 **H2 HIGH** / deck130 G1 / **R77**; skill #139 guide half)
**Repro.** `1787849901-ai_baka_deck130-0x55cf00bc5c80-vs-ai_baka_deck162.jsonl` seq **23** (turn
12 upkeep, 4 life, two punishers): `Deal 2 damage with Siege-Gang Commander targeting Fate
Unraveler [opponent's battlefield]` — a 3/4, no outcome clause; taken; a Goblin for nothing; dead on
the draw step. seq **17**: `... Pyrite Spellbomb targeting Ob Nixilis, the Hate-Twisted` at
`[counters: 3x loyalty]` — no clause; taken; survived. Spell menus on the same seat print
`Goblin (1/1) ... {right now: takes 3 damage - DIES}` (`130 vs146` seq 17), `Rorix Bladewing (6/5)
... SURVIVES (toughness 5)}` (`130 vs123` seq 38). 0/49 ability rows carry it.
**Ask.** The same clause on ability target rows; `- loyalty L` for planeswalkers (`{right now:
takes 2 - SURVIVES (loyalty 3)}`).
**Wave-51 prediction.** Ability-damage rows taken at a target whose toughness/loyalty exceeds the
damage: 0 (was 2); clause on 100% of ability damage rows.

## D7 — MED — mana-cost land animations are offered in UPKEEP; the Hive ONLY there (11/11); two Upkeep takes burned 8 mana before the draw. ADOPTED IN THE ANNOTATION FORM
(= seat-146-152-162 **E-4** / deck146 G-1 / deck152 G-2 / **R78**)
**Repro.** deck146 `becomes beholder`: 11 offers, 11 Upkeep, 0 main phase (six seats); `1787849885-
ai_baka_deck146-0x5620ff65a3b0-vs-ai_baka_deck126.jsonl` seq **31** (17/22) and **35** (16/22)
animated in Upkeep, `ATTACK: none` at seq 34/39. deck152's Lair: 19 Upkeep / 13 Main 1 / 1 Main 2
/ 1 Blockers; Katilda's pump 3 Upkeep / 1 Main 1.
**Ask.** On an Upkeep row for an own-permanent activation that will be offered again in Main 1,
append `{before your draw step - the same row is offered again in Main 1}`. **The DEFER form
(skip the Upkeep ask) is REJECTED**: it removes a legal window, and the row is the model's to
decline. Companion: why is the Hive never offered in main? — read the offer path; if a main-phase
offer is suppressed by the two-decline cap after the Upkeep decline, that is the cap doing its
job and the annotation should say `(declining here counts toward this turn's two declines)`.
**Wave-51 prediction.** Upkeep animation takes with the annotation rendered and no attack
following: 0/N (was 2/11 Hive); annotation on 100% of such rows; Hive main-phase offers > 0 or
the reason recorded.

## D8 — MED — the cast row does not say what is already owned: the Tutor with an Alarm out, the second copy of a non-stacking permanent (third corpus)
(= seat-123-130 **M2** / deck123 G3 / R67 promoted / **R79**; skill #138 keeps the guide at one line)
**Repro.** `1787849917-ai_baka_deck123-0x55f93524f640-vs-ai_baka_deck125.jsonl` seq **118**, **139**
`Cast Idyllic Tutor` with `Intruder Alarm` on the battlefield line (plan "already on battlefield,
but must cast it to dig for Bloodline Keeper"); seq **156** a second Intruder Alarm with one out;
`1787849885-ai_baka_deck126-0x5621009938d0-vs-ai_baka_deck146.jsonl` seq **37** a second Lantern
with `Chromatic Lantern` printed on its own line. Wave 49: 3 Tutors; wave 50: 2.
**Ask.** `{Intruder Alarm is already on your battlefield - this finds only an enchantment}` on the
Tutor row when any Alarm is owned; `{a copy is already on your battlefield}` on any non-stacking
permanent's cast row (a static/aura/legendary or an "if you control" duplicate).
**Wave-51 prediction.** Tutor casts with the tag rendered: 0 (was 2); second-copy casts with the
tag rendered: 0 (was 2 across two decks).

## D9 — MED — the Hammer of Bogardan upkeep return row does not price itself against the hand (third corpus)
(= seat-123-130 **M3** / deck130 G2 / **R80**; skill #138)
**Repro.** `1787849893-ai_baka_deck130-0x5566757367d0-vs-ai_baka_deck123.jsonl` seq **18** (turn 10
upkeep, 5 sources, Siege-Gang Commander in hand): `Put a card into hand with Hammer of Bogardan
[cost: {2}{r}{r}{r}]` taken under "wait, I have 5 mana, Siege-Gang is {3}{R}{R}, so I can cast it";
no Commander that turn; 19 Humans the next. Wave 49: 2.
**Ask.** `{spends 5 of your 5 untapped sources this turn; Siege-Gang Commander {3}{r}{r} in your
hand needs 5}` — the cast rows' `{leaves N of your M ...}` clause on an activated ability whose
cost is most of the turn, naming the most expensive castable card in hand it would strand.
**Wave-51 prediction.** Hammer returns with a castable body in hand and the clause rendered: 0
(was 1).

## D10 — MED — the Soul Shatter row prints only its card text; edict rows print `{right now: ...}` (ship with D1)
(= deck146 G-2 / **R81**)
**Repro.** `146 vs162` seq 25 (D1's window) and `1787849919-ai_baka_deck146-0x55d4b22a0310-vs-ai_
baka_deck123.jsonl` seq **20** (cast into `0 creatures`): no `{right now:}` clause. Lane X's edict
emitter keys on the `sacrifice!$ targetedplayer` shape; Soul Shatter's `!$ opponent` shape missed.
**Ask.** `{right now: they sacrifice <name> (MV N)}` / `{right now: they control 0 creatures or
planeswalkers - this does nothing}` on Soul Shatter and any `!$ opponent` sacrifice.
**Wave-51 prediction.** Soul Shatter casts at `0` with the clause: 0 (was 1); the clause's named
victim equals the narrated sacrifice N/N.

## D11 — MED — CARRIED wave-49 D15: the draw-engine cast row's feeds count, now with the discard converter (fifth corpus)
(= deck162 G-1 / **R82**; skill #149)
`1787849886-ai_baka_deck162-0x55de8d50c1f0-vs-ai_baka_deck123.jsonl` seq 7/10/13/14, `162 vs146`
seq 8: 0 `{feeds` strings; 5/9 breaks, 3 correct by Liliana's Caress (14 damage through discards
before any punisher). Ask: `{feeds: the opponent draws N extra per turn; converters on your
battlefield: <punishers> + Liliana's Caress}`.
**Wave-51 prediction.** Engine casts at converters 0 with opp `0 creatures`: 0.

## D12 — LOW-MED — the edict row's named victim carries its tags
(= deck123 G4 + deck126 G2 — two-seat convergence / **R84**; skill #135)
`1787849911-...deck126-...-vs-...deck162.jsonl` seq 6 `Shield Sphere is sacrificed, you gain 6`
(a `[defender]`; the guide's carve-out keys on the tag); `1787849883-...deck123-...-vs-...deck152.
jsonl` seq 6 `Katilda, Dawnhart Prime is sacrificed, they gain 1` (a 1/1 lord). Ask: the victim's
battlefield-line tags on the row (`Shield Sphere [defender] (0/6)`, `Katilda ... (1/1) [lord: ...]`).
**Wave-51 prediction.** Tag on 100% of named-victim rows.

## D13 — MED — a creature cast row carries a converter tag when the opponent controls Exquisite Blood / Sanguine Bond or has resolved an edict this game
(= deck125 G1 / **R83**; skill #135)
`1787849909-ai_baka_deck125-0x...-vs-ai_baka_deck126.jsonl` seq **42** (turn 28, 33 life): `Cast
Emrakul, the Aeons Torn {15}` beside `Exquisite Blood ... {effect: "Whenever an opponent loses
life, you gain that much life."}` on their line — taken; then seq 44 Verdict at `1 of yours`; loop
33 -> 0. Ask: `{their Exquisite Blood turns this creature's death into life for them}`.
**Wave-51 prediction.** Creature casts at deck125 with the tag rendered and a converter out: 0.

## D14 — MED — opponent-draws abilities priced like DRAW FORECAST
(= deck162 G-2 / **R85**; skill #150)
`1787849911-ai_baka_deck162-0x55fc834e92c0-vs-ai_baka_deck126.jsonl` seq **23/24**: Ob Nixilis -2
passed with P = 4 at their 3 life. Ask: `{their draw: 2 cards = 2 x P = <N> life}` on the -2 rows;
`= half their library x P` on Peer into the Abyss.
**Wave-51 prediction.** -2 passed with 2 x P ≥ their life and the clause rendered: 0/N (was 2/2).

## D15 — LOW — the `discard` list prints bare names; the bottoming ask prints cost and type
(= deck125 G3 + deck126 G3 — convergence / **R86**)
`1787849917-ai_baka_deck125-0x55f933f33530-vs-ai_baka_deck123.jsonl` seq 52; `1787849921-ai_baka_
deck126-0x556ae9dc1060-vs-ai_baka_deck123.jsonl` seq 7. Ask: the hand line's grammar on the list.

## D16 — LOW — bare back faces on the `{target text:}` of MDFC lands inside target clauses (15; a third emitter path)
(= engine seat **D-6** / **R87**)
`1787849905-ai_baka_deck130-0x560e786c0170-vs-ai_baka_deck152.jsonl` seq 15, 19 `Hengegate Pathway
(land) {target text: "{T}: Add {W}. // Mistgate Pathway"}`; Mistgate 11, Grimclimb 4 — all deck130
land-destruction rows. `markBareBackFaces` misses the target-clause text.
**Wave-51 prediction.** `// <name>"}` bare endings anywhere: 0/N (was 15).

## D17 — LOW — the receipt's source separator collides with card names
(= engine seat **D-5** / **R88**)
`1787849883-ai_baka_deck152-0x5596a8db3e60-vs-ai_baka_deck123.jsonl` seq 13 `- Paid {1}{w} for
Intrepid Adversary with Katilda, Dawnhart Prime, Elite Spellbinder` (10 receipts). Use `; `.

## D18 — LOW — CARRIED wave-49 D23: the JSONL `turn` field is the narration's turn minus one (fourth corpus)
Every `discard` record shows it (28/28; `deck123-vs-deck162` seq 23 `turn: 9` under `=== Turn 10`).

## D19 — LOW — `rejected_line_skipped` is not stamped when the rejected line is also out of range
(= engine seat **D-8** / **R90**)
`1787849909-ai_baka_deck126-0x5601b9f034e0-vs-ai_baka_deck125.jsonl` seq **19**: `CHOICE: 4 (Cast
Exquisite Blood) -- Wait, the list only goes to 3` not executed, `parse_note` empty. Stamp the
rejection before the range check so lane Y's guard is observable.

## D20 — LOW — a `decision_reversed_in_prose` counter (metric before any prompt change)
(= seat-146-152-162 **E-5** / deck146 G-3 / **R91**; skill #133)
`1787849919-ai_baka_deck146-0x55d4b22a0310-vs-ai_baka_deck123.jsonl` seq **20**: `CHOICE: 3 (Cast
Soul Shatter)` then 2,700 chars ending "I will cast Pelakka Predation", no second coded line. The
last "I will cast/attack X" sentence naming a row other than the last coded line's.

## D21 — LOW — the repeat row carries the opponent's life and creature count beside M
(= deck123 G1 / **R92**)
`123 vs162` seq 34 (C read as 1, line says 2); `123 vs130` seq 26 (stop 24 with C 4). The three
subtraction inputs on one line.

## D22 — LOW — enters-tapped on the `Play` rows
(= deck123 G5 / **R93**) `1787849886-...deck123-...-vs-...deck162.jsonl` seq 6.

## D23 — LOW — Lightmine Field priced on the attackers header
(= deck123 G6 / **R94**) `1787849917-...deck123-...-vs-...deck125.jsonl` seq 39, 63.

## D24 — LOW — the GANG BLOCK clause names the punisher
(= deck130 G3 / **R95**; skill #141) `1787849901-...deck130-...-vs-...deck162.jsonl` seq 22.

## D25 — LOW — an exit on the X menu, or the (a)/(b) verdict on the cast row (engine ruling needed on cancel semantics)
(= deck130 G4 / **R96**; skill #140) `1787849923-ai_baka_deck130-0x56221f5dde80-vs-ai_baka_
deck126.jsonl` seq 60. The cast-row clause form is safe now; the `0. Do not announce` row waits on
whether the card can return to hand after `Cast Card Normally`.

## D26 — LOW — the pain-source payment at low life on the cast row
(= deck130 G6 / **R97**) `1787849893-...deck130-...-vs-...deck123.jsonl` seq 56.

## D27 — LOW — the state block is stale across a sequential sacrifice ask
(= seat-146-152-162 **E-7** / deck146 G-4 / **R98**) `1787849899-ai_baka_deck146-0x55ad173f6180-vs-
ai_baka_deck125.jsonl` seq 60-65.

## D28 — LOW — `[damage ORDER, not a block]` on the combat-damage-order ask
(= E-8 / deck152 G-3 / **R99**) `1787849907-...deck152-...-vs-...deck146.jsonl` seq 13.

## D29 — LOW — `destroys 1 of their creature` (singular)
(= deck125 G2 / **R100**) `deck125-vs-deck152` seq 13.

## D30 — MEASUREMENT / DECISION — the game-log window is 54-62% of the mean prompt and 63-83% of the six largest
(= engine seat watch / **R104**; wave-49 R71)
`1787849917-ai_baka_deck125-0x55f933f33530-vs-ai_baka_deck123.jsonl` seq 96-98: 20.5-21.0 KB of
25.3-25.4 KB is log. No prompt > 26 KB; ask mean −13%, max −27% after D5's fix. The owner's
decision item: older-turn compaction is the next 10 KB if latency moves with size — the measured
p90s (ask 56 s, blockers 76 s) did not fall with the smaller prompts, so no change is asked;
re-measure next wave.

## D31 — MED (frontend / human seat) — CARRIED wave-49 D26 / wave-48 D20: Arena-style grouping + kill the O(n^2) RenderSpell scan (owner item)
Unchanged; this corpus produced 57- and 58-body boards (`Lord of Lineage (x27)` after `(x30)`,
`123 vs126`), so the human-seat pile problem remains reachable.

## D32 — LOW — CARRIED residuals, re-measured not re-derived
wave-49 D20 X free-kill marker (deck130 marker taken 2/2, no failing window) · D21 menace re-ask +
keyword canon (no window) · D22 overrun counter (multi-`CHOICE:` replies 1 on deck125/126) · D24
parity residuals (R15/R18/R22/R23/R24/R26/R44/R46/R47; `x1` takes lose the suffix; names-form
ATTACK tolerated) · D25 compound Doomsayer + Alarm loop (a 57-activation upkeep at `126 vs123`, the
model stopped; 21/21 natural — watch, ceiling unchanged) · R66 owner-side LOOP wording · R68
Battlement `{paying this taps:}` (receipts show the Battlement never paid; likely never reached) ·
R69 reading aids · R70 counter-target size (0/11 spreading this corpus — close on a second clean
pass) · the cycling row's `{if you pass here ...}` wording (R101, one window, inference) · the
"pass and hope" plan carried 5 past the content test (R102, observation) · lane W's `REVEAL_DBG`
runtime-gated trace -> compile-time gate per the owner's diagnostics rule (cleanup) · lane R's
`parseBlockAssignments` range re-scan count.

---

# SHIPPED OUT-OF-WAVE — recorded, NOT docketed (owner, Vita, 2026-08-27)

| change | commit | LLM-visible? | verify |
|---|---|---|---|
| Ability borders priced by strict potential + planner (Ironroot Warlord dual double-count) | **ca8a71e14** | no (human seat) | no window; regression only via the suite |
| Morph "Face Up" ability outliving the flip — AAMorph liveness on the `->next` chain + oracle skip of superseded instances | **pending gate** | narration only, if a morph is in the pool (none is) | suite pin + PARSETEST on the gate; by string next corpus if a morph deck rotates in |

---

# DISCHARGED — SET B: wave-50 engine lanes, by the emitter's actual string

| lane / commit | prediction | verdict | counts |
|---|---|---|---|
| **V d9144d62c** (D1) `nextCardToPlay` cleared | casts with tapped == 2x pips 0/N (was 12/234); counter-then-payment in stderr 0 | **PASS 0/236; PASS 0/26** | `Mana available` diffed across 236 non-X casts; 26 counters on the stack, none followed by `-  Target: <countered card>`; the Sigarda 5-for-4 (`152 vs126` seq 23) is Katilda's own tap |
| **Z 5b6b44c44** (D11) repeat row count + `x<N>` | exact-50 takes with no arithmetic 0; `x<N>` echo -> re-ask | **PASS 0/9; UNTESTED (0 echoes)** | offered 18, taken 9, max overshoot 28 (was 300), every N ≥ 2 take carried a stop line; **`x0` executed once -> D4** |
| **Z** (D12) mana-only gate | all-mana single-row windows with an empty stack 0 (was 3) | **PASS 0** | stderr auto-pass 379; skipped max 203 |
| **Z** (D18) `- Paid` receipt | one per cast/activation (two = D1) | **PASS 396/396 one-per-payment on casts; FAIL 6/9 on land animations** | **D5** |
| **Z (T)** auto-tap colour order | generic pips from a multi-colour source while a mono could pay: 0/N by string | **FAIL 69/386** | **D2**, IN FLIGHT |
| **X c37805358** (D3) gain recipient | disagreeing with the primitive 0/N | **PASS 0/7** | Devour `they gain N - the sacrificing player gains, not you`; Tribute `you gain N` + `You gained N life` 8/8 |
| **X** (D5) target-clause collapse | no row > 3,000; no prompt > 30,000 from the clause | **PASS** | max clause 288, max row 1,715, max prompt 25,793 |
| **X** (D13) sweeper K | `(0 able to attack)` 0 | **PASS 0; opponent-turn form UNTESTED** | 81 own-turn renders |
| **X** (D14) `[named: X]` | named-card casts by the punished seat 0 (was 3) | **PASS 0/76** | |
| **X** (D16) FORECAST mirror | on 100% of `DRAW PUNISHERS ... yours` (was 0/63) | **PASS 50/50** | |
| **X** (D17) MDFC menu marker | bare `// <name>"}` 0 (was 6) | **PASS on the wave-49 path 5/5; FAIL 15 on a THIRD path** | **D16** |
| **Y 7858a403f** (D7) rejection guard | executed lines with a verdict tail 0/N | **PASS 0/1; firing unobservable** | **D19** |
| **Y** (D8) exemplar parse | exemplar-equal replies failing 0/N | **PASS 0/507** | |
| **Y** (D9) `CHOICE: 0` no-pass | reaching Baka without sibling/re-ask 0/N | **UNTESTED** | 0 such replies |
| **Y** (D10) content-keyed expiry | "The game is lost" carries 0 (was 5); loop-lockout plan carried | **PASS 0; PASS** | longest correct run 50 (`125 vs123` seq 49-98); `plan_echo_count` max 26; target-absent line 6 |
| **Y** (D19) degenerate decode | garbage reaching Baka 0/N | **UNTESTED** | 0 non-ASCII replies |
| **W b39b80375** (D2) own-chooser reveal | `none was a legal target` at a foreign chooser 0; edict no-sacrifice 0/N | **PASS 0/15; FAIL 1/13 for a DIFFERENT mechanism** | the wave-49 Tribute shape replayed correctly (`126 vs146` seq 15); Soul Shatter -> **D1** |
| **W** (D4) cleanup discard ask | every `You discarded` preceded by a `discard` record | **PASS 26/26 (28 records)** | the 2 unnarrated precede `gameend` |
| **W** (D6) self-target tag | self-target rows taken 0 (was 2) | **PASS 0/124; all-self line UNTESTED (0 windows)** | the "vanish" was the designed two-decline cap (801 stderr auto-passes) |
| carry: max priority windows/turn ≤ 10 | | **PASS 10** (was 16) | |
| carry: fallbacks ≤ 0.31% | | **PASS 0.17%** | |
| carry: `mana_only_windows_skipped` ≤ 1,000 | | **PASS 203** | |
| carry: no prompt > 30,000 | | **PASS 25,793** | |
| Vita/human-seat fixes | not LLM-visible | no window | |

# DISCHARGED — SET A: wave-49 guide edits + boundary pass, by deck (reviewer verdicts, summarised)

| deck | verdict summary | routed |
|---|---|---|
| **123** (3-3) | A49-1 plan-with-numbers **PASS 9/9** (overshoot 300 -> 28); A49-2 own-upkeep **FAIL 2/3** (both won); A49-3 fetch 1/1; A49-4 sweeper re-key 1 right + **3 casts at N 0** (`vs125`); A49-5 edict **0/1** (Katilda declined); A49-6 2/2; A49-7 Tutor **FAIL 2**, "Tutor: dead" 0; A49-8 Skeins **FAIL 3/3** but the discard ask chose sanely 7/7; CHECK 2 Lightmine FAIL 2; cleanup discards 7/7 sane; `x0` executed once | D4, D8, D21, D22, D23 |
| **125** (2-4) | W46 Emrakul-into-converter **FAIL 1/1**; W47 Staff gate FAIL 2/5 as written, 0/5 re-derived (W22, skill #142); W48 X=3 **6/6**; W49 Verdict at `1 of yours` **FAIL 1** (own Emrakul); W50 Path **PASS 0 + 2/2** (first measurement after three untested waves); W51 land 72/72, Elixir 2; 9 discards, no order text | D13, D15, D29 |
| **126** (3-3) | D43 both-out Tribute **1/1 win**; D44 0; D45 0/9; D46 Tribute-at-1 **1 miss then 3/3** (a `[defender]` victim); D47 Lantern 1+1 (third corpus); D48 0/1; D50 mana-only leaks **0 (was 6)**; three `stale_echo` fallbacks (hand card named); a Tribute discarded over a Lantern -> -208 | D3, D12, D15 |
| **130** (2-6) | D49-1 mulligans 2/2; D49-2 Hammer **FAIL 1/1** (third corpus); D49-3 untested; D49-4 rule (a) broke once at the X menu; D49-5 self-target **0/49**; D49-6 face damage 0; D49-7 PASS; ability-damage rows taken at survivors 2; GANG kill declined at 7 life; walls handed 9 life; Starstorm on nothing | D6, D9, D24, D25, D26 |
| **146** (1-5) | 146-A converter ATTACK **PASS 4/4** (first windows after two no-window corpora); 146-B lone attacker FAIL 1 (no cost); Verse-own 14/14; the pair's BLOCK bullet was false (146-L); Hive Upkeep takes 2/11; a Pathway bottomed from a 2-land keep; a 1-land six kept under `would not cover`; **Soul Shatter no-sacrifice (D1)**; `- Paid` missing on Hive (D5); one loss to the engine | D1, D5, D7, D10, D20, D27 |
| **152** (5-1) | 152-A untested (no pair board); 152-C Katilda **0/4**; deploy 44/44; Fateful-own 9/9; Lair taken on TAPPED/repeat rows 4/34 (no section — 152-E); Katilda blocked tokens 2/2 (152-F); generic-from-dual 17 receipts | D2, D28 |
| **162** (5-1) | RULE 1 18/18; Rule 2 5/9 breaks, 3 correct by Caress (162-E); Ob Nixilis -2 lethal burst passed 2/2 (162-D); Peer 1/1; FORECAST-theirs 50/50; land 28/28; **closest to exit** — on the wave-51 agenda if D11 ships and 162-D fires in its first window | D11, D14 |
| **pool** | wave-49 boundary B1 (engine-behaviour sentences) CUT at deck123/deck126 — PASS, and lane W made the deck123 one false on schedule; B2 mulligan NEVERs at deck162/deck152 converted (162-F, deck152 B2) — PASS; B3 repeat-row literal re-quoted as `x<N>` — PASS (the row now prints it); B5 "hard stop" labels replaced — PASS; B6 UNTESTABLE flags: deck146's converter lanes finally had windows (flag lifted), deck152's pair lane still none (second corpus, flag stands) | |

---

# CARRIED — open, re-measured, not re-derived
See D11, D18, D31, D32. The wave-49 OPEN questions: "does the repeat row get taken" — CLOSED
again (9/18, every counted take executed exactly, overshoot bounded); "the ~800 s ceiling" — max
587 s, a complete 10 KB reply; the floating-mana class — gone with D1 (0 `Already in pool` after
an answered cast); the double-tap — PASS 0/236, its receipt now the standing regression detector.

---

# FALLBACK CLASSES — 3 Baka-executed / 1,780 = 0.17%

| n | class | records | disposition |
|---|---|---|---|
| 3 | **in-range index + off-menu (hand) name, labelled `stale_echo`** | `deck126-vs-deck125` seq 14; `deck126-vs-deck162` seq 16; `deck126-vs-deck130` seq 13 | **D3** (+ guide F19, skill #151); Baka cast the wrong card in 2 |
| 0 | garbage decode (was 2) | | lane Y's counter untested |
| 0 | unparseable exemplar / `CHOICE: 0` on a no-pass ask (was 1) | | lane Y paid |
| 0 | index-past-menu executed (was 0; wave 48: 4) | | lane S holds |
| 0 | designed re-asks fired (was 2) | | none needed |

---

# PROMPT CHARS PER DECISION KIND — wave 49 -> wave 50 (ALL games, 21/21)

| kind | n49 | mean49 | max49 | n50 | mean50 | max50 | p90 latency 49 -> 50 (s) |
|---|---|---|---|---|---|---|---|
| ask | 1,422 | 10,485 | 34,333 | 1,214 | 9,102 | 25,018 | 49.2 -> 56.3 |
| priority | 364 | 12,204 | 28,487 | 376 | 12,038 | 25,793 | 53.2 -> 55.9 |
| attackers | 91 | 10,690 | 25,701 | 102 | 11,227 | 25,087 | 49.5 -> 57.7 |
| blockers | 21 | 9,782 | 17,616 | 37 | 11,267 | 25,450 | 56.8 -> 76.2 |
| reveal | 8 | 11,883 | 16,303 | 15 | 14,048 | 20,689 | 102.5 -> 52.8 |
| bottom | 8 | 2,170 | 2,660 | 8 | 2,200 | 2,542 | 90.8 -> 98.8 |
| discard | — | — | — | 28 | 9,534 | 25,402 | — -> 57.1 |

Ask mean −13%, max −27% (the 34 KB row is gone). Log share: ask 54% / priority 58% / attackers
62% of the mean; 63-83% of the six largest (D30). Latency did not fall with size; p90s are
reasoning-bound.

---

# LATEGAME SPECIMEN — invariant 0 (for the owner's per-wave prompt review)

**Primary (one turn, both seats):** `1787849917-ai_baka_deck123-0x55f93524f640-vs-ai_baka_deck125.jsonl`
seq **163** (the 25,018-char Main-2 ask at turn-field 53 of the 54-turn game) paired with
`1787849917-ai_baka_deck125-0x55f933f33530-vs-ai_baka_deck123.jsonl` seq **96-98** (a `discard`
ask at 25,402 chars with 22 own permanents and a 9-card hand, then two priority windows at 81-83%
log, the carried 50-window plan). Every lane-W string, the new kind, and the log-share question.
**Engine-correctness specimen:** `1787849903-ai_baka_deck146-0x5568f6f12330-vs-ai_baka_deck162.jsonl`
seq **25-26** with the opposite seat's seq 16 (Soul Shatter into a 5/5 + a walker — **D1**).
**Repeat machinery:** `1787849921-ai_baka_deck123-0x556aeb0dee00-vs-ai_baka_deck126.jsonl` seq
**28-33** (`[you control M creatures right now` at M=3 and M=33, x28 then x25 -> 58, both receipts,
the CHOICE-vs-PLAN conflict — **D4**).
**Widest combat render, read correctly:** `1787849893-ai_baka_deck130-0x5566757367d0-vs-ai_baka_
deck123.jsonl` seq **49** (`A1-A21. Human #1-#21 ... x21`, `B4. Rorix ... may block A1-A21 (you
kill it, your blocker lives), A22 (both die)`, `Unblocked ... LETHAL`; the one surviving
assignment found).
**Natural late grind:** `1787849899-ai_baka_deck146-0x55ad173f6180-vs-ai_baka_deck125.jsonl` seq
**56-72** (two Kayas, a legend-rule ask, Emrakul's six annihilator asks with the stale state block
— **D27** — and the Command edict that took Emrakul).
