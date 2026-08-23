# deck125 "Revelation Fracture" — Step-0 (guideless) review, wave 39

Seat: deck125, UW draw-go control. Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-040744`,
binary `eabf463b1`, pilot `qwen36-35b-a3b` (FP8 35B, the standing opponent). **No guide was deployed**
— this is the guideless Step-0 corpus, so every behaviour below is the qwen x core interaction with
no guide confound. Deliverable: the INITIAL guide (`deck125_strategy.txt`, this directory).

Six games: vs139 (CRASHED at t15, see section 7), vs126, vs162, vs146, vs152, vs36. 169 model
decisions + 5 engine defers across the six. Per the owner ruling the win column is context only and
is not a verdict; the audit below is at the DECISION level.

Record (context only): 0 wins in the 5 completed games. Final life: vs126 0/44 (t31), vs162 -2/19
(t15), vs146 -24/16 (t42), vs152 -3/20 (t16), vs36 -7/20 (t20). The `139 125 adj1` row in
`results.tsv` is **bogus** — that game core-dumped at t15 with deck125 AHEAD 28-16; do not read it
as a loss.

Loss routing (stated so the #1-slot choice is auditable):

| game | routes to | one-line reason |
|---|---|---|
| vs36 | **PLAY (guide lane)** | the only Path drawn was spent t3 on a 1/1; a resolved Master of Etherium (8/8 -> 9/9) then killed it from 26 with no answer left |
| vs162 | PLAY + matchup | 20 life burned by non-combat drain (Underworld Dreams / Fate Unraveler x2 / Ob Nixilis) while the pilot Path'd a 0/6 wall on t2 and answered the drain engines only at 3 life |
| vs126 | CONSTRUCTION-ceilinged | no clock: 31 turns, opponent finished at **44** life on a Sanguine Bond + lifelink board; deck125's only damage source (Staff of Nin) came down t28 |
| vs152 | VARIANCE | 16-turn game, no sweeper drawn, 3 Essence Scatters spent correctly on real creature spells |
| vs139 | ENGINE (crash) | deck125 was ahead 28-16; see section 7 |

---

## 1. PRESERVE LEDGER (unconditional at Step-0; the guide must not legislate over these)

Quantified per seam over all six games. **These seams are FROZEN — the guide says at most one
confirmatory clause about each, and never a loud procedure.**

| seam | measure | result |
|---|---|---|
| land drop | "Play no land right now" offered / taken | **36 / 0** — a land every single turn it was legal |
| X announcement | X menus / picked the largest X | **5 / 5** (X=1, 3, 9, 2, 3; option 1 every time) |
| Staff of Nin activation | ping windows / activated | **11 / 11** (9 at the face, 2 at a planeswalker) |
| sweeper timing | Verdict/Final Judgment windows judged correct | **10 / 11** (6 casts, 5 declines — 3 of the declines at `of which 0 are creatures`; the one error is 3.4) |
| cast arrival | non-defer chosen casts that reached the narration | **61 / 61** (2 more unverifiable only because the log ends) |
| reply protocol | fallbacks other than the section-6 defers | **0** (0 unparsed, 0 empty, 0 retracted, `answer_replaced` false on all 169) |
| cast-nothing | cast-nothing / cast windows | 12 / 75 = 16%, and this seat holds **17 instants**, so the raw rate is uninformative; of the 12, 5 are the correct sweeper/Path declines above and none is an against-floor decline of a payable proactive permanent |
| target sub-menus | targeted casts that picked a sensible target | 8/8 Path targets legal and the intended one; 2 multi-target menus (`Fate Unraveler #1/#2`, `Nadaar/Triumphant Adventurer`) answered cleanly |

Consequence, per the Step-0-bis rule-0 exception: the guide contains **no attack floor, no blocker
rules, no land-drop procedure, and no X arithmetic procedure at the menu** — three of those seams are
perfect and the fourth does not exist (section 2).

## 2. ABSENCE TABLE (offered / taken / arrived) — what the plan required that never happened

| thing the plan needs | offered | taken | arrived | routing |
|---|---|---|---|---|
| Emrakul, the Aeons Torn on the battlefield | **0** option lines in **34 prompts holding it** | 0 | 0 | **CONSTRUCTION — unreachable cost.** Emrakul is `{15}`; the deck's only mana is 26 lands with zero ramp, rocks or cost reduction. Max `Mana available:` reached across six games: **12** (vs126 t30), then 11, 9, 8, 7, 7. |
| any creature of deck125's on the battlefield | 0 | 0 | 0 | construction — the deck contains exactly one creature and it is the line above |
| `attackers` decision window | **0 in 6 games** | — | — | structurally impossible; combat cannot occur at this seat |
| `blockers` decision window | **0 in 6 games** | — | — | same |
| mulligan taken | 6 keeps, **0 mulligans** | — | — | the mulligan seam is exercised in the KEEP direction only; the `"having already taken N mulligan"` string is **UNEXERCISED this corpus** (verified in the emitter instead — `AIPlayerGPT.cpp:11756`, emitted only when `mullsTaken > 0`) |
| Elixir of Immortality recursion loop (Elixir returns to library, is redrawn, recast) | — | — | **YES, vs152 s3 -> s10** (cast, activated, shuffled in, drawn again, recast) | works end-to-end |
| Staff of Nin on the battlefield | 7 | 5 | 5 (2 of them in one game, vs146) | fine |

A column of zeros outranks any individually-observed misplay: **Emrakul's zero is the seat's #1
finding**, and it is not a deploy-floor failure — it is a false plan (3.1).

## 3. FAILURE CLASSES, ranked, with seq repros

### 3.1 THE PLAN SLOT IS OCCUPIED BY AN UNCASTABLE CARD (rule-3 unreachable-cost face)

**Evidence.** "Emrakul" appears in **144 of 169 reasoning traces**. The pilot repeatedly derives its
unreachability from scratch and then keeps it as the plan anyway:

- vs146 s36 PLAN, verbatim: `Ramp to 15 mana to cast Emrakul, then attack for the win.` The deck has
  no ramp.
- vs146 s31: *"I can tap 10 lands for 10, but I need 15... I cannot cast Emrakul yet"* — the same
  derivation repeats at s26, s27, s30, s33, s35, s38, s40, i.e. every remaining decision of the game.
- vs146 s30 — **it changed a real decision**: *"Should I cast Path to Exile on Nadaar? Or save it for
  later? Actually, Emrakul is a 15/15 with flying and annihilator 6. It wins the game. I just need to
  survive until I can cast it."*
- vs126 s29/s33 at 10-12 mana: *"If I attack with Emrakul... Annihilator 6 will drain them. It's the
  win condition."* The game ended two decisions later with deck125 at 0.
- vs36 s14, a **ONE-OPTION** decision (`1. Life with Elixir of Immortality`) that cost **10,358
  characters** of reasoning carrying `develop mana for Sphinx's Revelation or Emrakul` and
  `Attack when board presence allows` — in a deck that never attacks.

- vs126 s31 (turn 30) — the engine itself flagged the plan as unactionable and the belief survived
  it. The carried PLAN read *"...continue ramping for Emrakul. Cast Emrakul when mana allows and
  attack to win"*, and the prompt appended the stale-plan note *"(note: this decision's list does not
  contain the actions your plan names. That is about this menu, not about what is legal for you -
  pick the best option below, and re-state your plan if it has gone out of date.)"*. This is the ONLY
  occurrence of the string `Cast Emrakul` in any deck125 prompt across the corpus, and it is inside
  the model's own plan echo — Emrakul was never an option line.

**Tax (the saliency signal, low-branching rows read first per wave-31 HL9).** Median reasoning 6,030
chars over 169 decisions. Top spikes: 12,733 (vs162 s21, a **land drop**), 11,268 (vs162 s20, a Staff
ping), 10,907 (vs162 s16), 10,637 (vs36 s8, a **land drop**), 10,358 (vs36 s14, **one option**).
Every one of the five rehearses the same two beliefs: what can I cast toward Emrakul, and can I
attack. The three biggest are all on decisions with 1-3 options, so the deliberation cannot be about
the choice.

**Routing: GUIDE.** The fact is not rendered anywhere (no "you cannot reach this cost" surface
exists), the belief is a deck fact, and the corrective is one flat unreachability statement plus a
named substitute win condition. -> guide rule #1.

### 3.2 INTERACTION SPENT ON THINGS THAT CANNOT KILL YOU

25 counters were spent across six games. Judged individually:

| verdict | n | instances (seat seq) |
|---|---|---|
| correct (real threat) | 16 | Illuna (139 s21), Beanstalk Giant (139 s27), Idyllic Tutor (126 s8), Exquisite Blood (126 s25), Ob Nixilis (162 s10), Kaya (146 s13), Silverquill Command (146 s18), Silverquill Silencer (146 s21), Acererak (146 s25), Triumphant Adventurer (146 s16), Tovolar's Huntmaster (152 s13), Augur of Autumn (152 s15), Brutal Cathar (152 s17), Master of Etherium (36 s16), Howling Mine (162 s26), Nuisance Engine (36 s11) |
| **spent on a mana producer / wall / 0-power body** | **9** | Dryad of the Ilysian Grove (139 s16), **Arboreal Grazer 0/3** (139 s28), **Overgrown Battlement 0/4** (126 s4), **Chromatic Lantern** (126 s6), **Pride Guardian x3** (126 s11, s15, s29 — a 5-mana Fall of the Gavel on the third), **Glaze Fiend 1/1** (36 s22), **Silver Myr** (36 s24) |

The decisive one is **vs36 s24**: with `Mana available: 8` and a **Master of Etherium (8/8)** sitting
on the opponent's battlefield hitting for 8-9 a turn, the pilot spent the 5-mana Fall of the Gavel
countering a **Silver Myr** (a mana dork). Life went 26 -> 17 -> 13 -> 3 -> -7 over the next four
turns. There was nothing else in hand — which is the point: **the answer that would have handled the
Master had already been spent** (vs36 s4/s5, turn 3: the game's only Path to Exile aimed at a
**Salvage Slasher 1/1**, with a Steel Wall 0/4 the other option).

Same shape at vs162: Path #1 spent t2 on **Shield Sphere** (a 0/6 defender); the drain engines
(Underworld Dreams, two Fate Unravelers, Ob Nixilis) were then answered only at **3 life** (s22).

**Routing: GUIDE**, as a NAMED-CARD table from the observed pool plus one catch-all keyed on a
visible number — because the deciding fact is NOT rendered (see 5.2: the stack line prints a bare
name).

### 3.3 SPHINX'S REVELATION CAST AT X = 0

vs139 s10 (turn 5, Main phase 2): with exactly `{U}{U}{W}` available — `Mana available: 3` — the
pilot committed to `Cast Sphinx's Revelation {u}{u}{w}{x}`, its own PLAN stating *"Cast Sphinx's
Revelation for X=1"*, when X=1 needs four mana. The only announceable X was 0: gain 0 life, draw 0
cards. The engine then vetoed the pick (section 6) — the misplay was prevented by an engine defect,
not by the pilot. Contrast the same card at s18 two turns later with four lands: X=1 announced, cast
resolved. **Verified arithmetic across the corpus: X = `Mana available: N total` minus 3** (n=4 where
the cast-window mana was readable: 4 -> X<=1, 6 -> X<=3, 6 -> X<=3, 12 -> X<=9). -> guide rule keyed
to that printed substring.

### 3.4 ONE SWEEPER MISFIRE

vs146 s9 (turn 7): `Cast Supreme Verdict` taken with the header reading
`Opponent battlefield (5 permanents listed, of which 0 are creatures)` — a 4-mana wipe into an empty
board. This is the ONLY sweeper error in 11 windows; the other 10 (including three correct declines
of Final Judgment at `0 are creatures`, vs146 s19/s20/s22) were right. Guide fix is one line keyed to
the header's own count, not a section.

### 3.5 LIGHTMINE FIELD FIRED AGAINST A SINGLE ATTACKER

`auto=@each blockers:foreach(creature[attacking]|Battlefield) damage:1 all(creature[attacking]|Battlefield)`
— the damage equals the NUMBER of attackers. vs36 s21 cast it into a board whose only real attacker
was the 8/8 Master of Etherium: 1 damage, no effect, and the deck died four turns later. Offered 9 /
taken 5 overall. -> guide: key it to the opponent's creature count (3 or more).

## 4. PER-CLAIM LEDGER for `deck125_strategy.txt` (HL2 — one row per checkable claim)

Every claim verified against BOTH the engine primitive (`bin/Res/sets/primitives/mtg.txt`) AND real
Oracle text (`api.scryfall.com/cards/named?exact=`), and cross-checked against how the corpus renders
it. **Engine text and Oracle text agree exactly for all 12 cards.**

| # | guide claim | primitive (mtg.txt) | Scryfall | corpus render | verdict |
|---|---|---|---|---|---|
| 1 | Emrakul costs `{15}` | `mana={15}`, `power=15 toughness=15` | `{15}`, 15/15 | `Emrakul, the Aeons Torn {15} (15/15) [creature]` in the hand line (vs36 s26) | TRUE |
| 2 | deck has no ramp; one land per turn is all the mana | decklist: 26 lands, no mana rock, no cost reducer (Elixir `{2},{T}` produces no mana; Staff produces no mana) | — | max `Mana available:` = 12 across 6 games | TRUE |
| 3 | Staff of Nin `{6}`; upkeep draw; `{T}` 1 damage to any target | `mana={6}`, `auto=@each my upkeep:draw:1`, `auto={T}:damage:1 target(anytarget)` | identical | option line prints the same text 51x | TRUE |
| 4 | Path to Exile `{W}`, exiles target creature, controller may fetch a basic | `mana={W}`, `target=creature`, `auto=moveto(exile) && ability$!name(search land)...!$ targetcontroller` | identical | narration: *"Opponent's Fate Unraveler was exiled... Opponent's Swamp entered the battlefield from the opponent's library"* | TRUE |
| 5 | Path is the deck's ONLY single-target creature removal | decklist sweep: the only other creature answers are Supreme Verdict / Final Judgment (mass) | — | — | TRUE |
| 6 | Essence Scatter `{1}{U}` counters **creature** spells only | `mana={1}{U}`, `target=creature|stack`, `auto=fizzle` | identical | the pilot itself verbalised it correctly, vs162 s21 | TRUE |
| 7 | Cancel `{1}{U}{U}` counters any spell | `mana={1}{U}{U}`, `target=*|stack` | identical | — | TRUE |
| 8 | Dream Fracture `{1}{U}{U}` counters and BOTH players draw | `auto=fizzle` + `auto=draw:1 controller` + `auto=draw:1 targetController` | identical ("Its controller draws a card. // Draw a card.") | narration vs139 s17: *"You drew Fall of the Gavel / Opponent drew a card"* | TRUE |
| 9 | Fall of the Gavel `{3}{W}{U}` counters and gains you 5 life | `mana={3}{W}{U}`, `auto=fizzle`, `auto=life:5 controller` | identical | *"You gained 5 life (now 31)"* vs139 s22 | TRUE |
| 10 | counter count = 13 (4 Essence Scatter, 3 Dream Fracture, 3 Cancel, 3 Fall of the Gavel) | deck125.txt | — | — | TRUE |
| 11 | Supreme Verdict `{1}{W}{W}{U}`, can't be countered, destroys all creatures, sorcery | `mana={1}{W}{W}{U}`, `abilities=nofizzle`, `auto=destroy all(creature)`, `type=Sorcery` | identical | option line text matches | TRUE |
| 12 | Final Judgment `{4}{W}{W}` **exiles** all creatures, sorcery | `auto=moveto(exile) all(creature)`, `mana={4}{W}{W}` | identical | narration vs146 s24: *"Opponent's Angel was exiled from the battlefield"* | TRUE |
| 13 | Sphinx's Revelation `{X}{W}{U}{U}` instant, gain X life draw X cards | `auto=life:X && draw:X`, `mana={X}{W}{U}{U}`, `type=Instant` | identical | *"You gained 9 life... You drew Path to Exile..."* vs126 s33 | TRUE |
| 14 | X = `Mana available: N total` minus 3 | derived | — | 4 -> 1, 6 -> 3, 6 -> 3, 12 -> 9 (4/4) | TRUE (corpus-verified) |
| 15 | X menu: option 1 is the largest X, reply with the option number | — | — | verbatim in the prompt: *"option 1 is the LARGEST X (X = 9). Reply with the OPTION number, not the X value"* | TRUE |
| 16 | Elixir `{1}`; `{2},{T}`: gain 5, shuffle Elixir **and your graveyard** into library | `auto={2}{T}:life:5 && moveTo(mylibrary) all(this) && moveTo(mylibrary) all(*|mygraveyard) && shuffle` | identical | *"Your Elixir of Immortality was put into your library / Your Path to Exile moved from your graveyard to your library"* vs162 s7 | TRUE |
| 17 | Lightmine Field `{2}{W}{W}`; damage to each attacker = number of attackers | `auto=@each blockers:foreach(creature[attacking]|Battlefield) damage:1 all(creature[attacking]|Battlefield)` | identical | — | TRUE |
| 18 | 18 lands make `{W}`, 20 make `{U}`; 12 make both | deck125.txt: Plains 6 + Tundra 4 + Glacial Fortress 4 + Seachrome Coast 4 = 18 W; Island 8 + those 12 = 20 U | Tundra `subtype=Plains Island`; Glacial Fortress / Seachrome Coast `{T}:Add{W}` + `{T}:Add{U}` | hand lines print `(land: taps for {W}{U})` | TRUE |
| 19 | cheapest cards cost one mana (Path `{W}`, Elixir `{1}`) | deck125.txt + primitives | — | pregame line: *"Cheapest spell in this hand: Path to Exile {w} (mana value 1)"* | TRUE |
| 20 | pregame prints `"Your hand (7 cards), counted by the engine: 3 lands, 4 spells"` | — | — | verbatim, all 6 mulligan prompts | TRUE (quoted string swept) |
| 21 | pregame prints `"Playing every land in this hand would cover the cost of:"` | — | — | verbatim, all 6 mulligan prompts | TRUE (quoted string swept) |
| 22 | mulligan prompt contains `"having already taken"` after a mulligan | `AIPlayerGPT.cpp:11756` | — | **0 occurrences this corpus (0 mulligans taken)** | TRUE at the emitter; UNEXERCISED in corpus — flagged |
| 23 | battlefield header prints `"Opponent battlefield (N permanents listed, of which M are creatures)"` | — | — | verbatim, 163 prompts | TRUE (quoted string swept) |
| 24 | stack render is `"1 (top): opponent's <name> [spell]"` | — | — | verbatim, 30 distinct lines | TRUE (quoted string swept) |
| 25 | opponent-pool names in the counter tables | — | — | every name taken from a `chosen_text` / battlefield line in this corpus | TRUE |

Quoted-render-string sweep: rows 15, 20, 21, 23, 24 grepped verbatim against the corpus `prompt`
fields — all non-zero except row 22, which is source-verified and flagged as unexercised. No guide
sentence asserts what the render does NOT do. No frequency or rarity word appears in the guide
(swept for "rarely", "usually", "often", "most games", "sometimes", "always happens").

## 5. ENGINE / REPRESENTATION LEDGER (wave-40 candidates)

### 5.1 HIGH — `validation_reject_reask_exhausted`: a legal, OFFERED cast is vetoed by the heuristic's own desirability score

This is the wave-39 status anomaly. **Verdict: ASK-SHAPE DEFECT, not strategic holds.** Full trace in
section 6.

### 5.2 HIGH — the counter seam renders no fact you can decide on

`ON THE STACK` prints only `1 (top): opponent's Silver Myr [spell]`, and the counter option prints
only `Cast <counter> - can target on the stack: <name> {card text: <the COUNTER's own text>}`. The
target spell's **type, mana cost and P/T are absent at both emitters** (30 distinct stack lines, 0
carrying any of them). A guide therefore cannot write "counter any creature spell with power 3+" —
the number is not on the screen — and the pilot must recall the card from the decklist block at the
top of a very long prompt. This is the offer-side twin of the absent-field audit (wave-32 deck36 P1).
**Ask:** annotate the stack entry and the `can target on the stack:` clause with the spell's type,
mana cost and (P/T) — e.g. `opponent's Master of Etherium {2}{u} (creature 0/0*) [spell]`. It would
convert 3.2's named-card table into a numeric rule that generalises past this pool.

### 5.3 HIGH — a COUNTERED spell is narrated as `"resolved"`

Every spell this seat countered narrates as `- Opponent's <name> resolved and went to the opponent's
graveyard`. The W35 register exists (`AIPlayerGPT.cpp:2036` emits `"<name> was countered"`, gated on
`mCounteredSpell` set from `WEventSpellCountered` raised by `ActionStack::Fizzle`,
`src/ActionStack.cpp:1592`) but it **never fires in this corpus**: `"was countered"` appears **0
times in any prompt across all 21 games** (the 22 hits corpus-wide are all inside the models' own
reasoning prose). Counts at this seat: 447 `resolved and went to the opponent's graveyard` + 634
`resolved and went to your graveyard`, covering both genuine instant/sorcery resolutions and every
counter. Harm is already observable at another seat — deck162 s29 reasoning: *"'...went to your
graveyard' immediately after casting. Usually, the game logs resolution. Maybe it was countered? Or
maybe the prompt just truncated the resolution details."* Under the trust doctrine `"resolved"` is
simply FALSE for a countered spell. Repro: deck125 vs126 s4 -> s5 (Essence Scatter -> *"Opponent's
Overgrown Battlement resolved and went to the opponent's graveyard"*, a creature spell that would
have entered the battlefield had it truly resolved).

### 5.4 MEDIUM — the cast window is asked twice and the second ask shows post-tap mana

vs126 s16 and s17 are the same `Cast Sphinx's Revelation` question back-to-back; s16 was answered
`Cast Sphinx's Revelation` and s17 re-asks with `Mana available: 1` while the following X menu
correctly offers up to X=3 (six mana's worth). Same shape at vs139 s3/s4 and s9/s10, and vs126
s13/s14. Any guide rule keyed to `Mana available:` is therefore unsafe at the SECOND ask of a pair.
The guide works around it (its threshold fires at the cast window and the X pick is delegated to the
menu's own "largest X" line), but the render should either not re-ask or should print the
pre-payment number.

### 5.5 LOW — `results.tsv` credits a win for a core-dumped game

`139 125 adj1 16 28 14` records deck139 as the winner of the crashed game while deck125 was ahead
28-16. A crashed game should record no winner.

## 6. THE 5-CONSECUTIVE-DEFER ANOMALY — classification

**Location:** deck125 vs deck139, turn 5, seq 11-15, `kind=defer`, `phase=Main phase 2`,
`fallback=validation_reject_reask_exhausted`, `chosen_text="Sphinx's Revelation"`, `choice=-1`,
`latency_ms=-1`, `coded_answers=0` on all five.

**VERDICT: ASK-SHAPE / ENGINE DEFECT. Not correct strategic holds, and not even five decisions —
the model was never asked.** `latency_ms=-1` and `coded_answers=0` on all five records mean no model
round trip occurred; these are five engine-internal re-ask iterations of ONE window, logged as five
records.

**Trace (resolution-verified from RESULT events, not inferred):**

1. s9 (t5, MP1, `Mana available: 3`): options `1. Cast Sphinx's Revelation / 2. Cast nothing` ->
   chose 2.
2. s10 (t5, MP2, same 3 mana: Island {u}, Glacial Fortress {w}|{u}, Plains {w}) -> chose
   `1. Cast Sphinx's Revelation`, PLAN: *"Cast Sphinx's Revelation for X=1"* (X=1 was unaffordable).
3. s11-s15: five `validation_reject_reask_exhausted` defers, no model call.
4. **Resolution:** at s16 (turn 6) the prompt still lists `Sphinx's Revelation {u}{u}{w}{x}` in
   `Your hand`, the battlefield still shows the same three untapped lands, and option 3 offers the
   cast again. **The cast never happened and no mana was spent.**

**Mechanism (pinned in the stderr and the source):** `game-139v125-1787476066.stderr` lines 465-481:

```
Should I play Sphinx's Revelation?
shouldPlayPercentage = -9
AIPlayerGPT: model chose Sphinx's Revelation but it fails validation; re-ask budget spent, deferring to heuristic
```

five times over. The "validation" the model's pick is put through is `AIPlayerBaka`'s own
desirability roll, not a legality check. `AIPlayerBaka.cpp:3367` applies the X-slack penalty
`shouldPlayPercentage -= (shouldPlayPercentage * 1.9f) / (1 + xDiff)`; with base 10 and
`xDiff = ManaEngine::maxAnnounceableX(...) = 0` this yields **-9**. `AIPlayerBaka.cpp:3422` then
reads:

```cpp
if ((forceBestAbilityUse || aiForcedCandidate) && shouldPlayPercentage > 0)
    chance = 0;
if (chance > shouldPlayPercentage)
    continue;
```

— the forced-candidate override is gated on `shouldPlayPercentage > 0`, so at a **negative** score the
model's explicit pick can never be honoured and the candidate is `continue`d. This is the exact
signature the N-137c comment block above line 3336 documents for convoke; that fix repriced `xDiff`
but left the negative-score half of the trap intact.

**Same-seam control, same game, same card (wave-31 HL4 rung 3):** vs139 s18-s19, turn 7, one more
land -> `shouldPlayPercentage = 1` (10 - 19/2), stderr logs
`shouldPlayPercentage was less than 10 this was a lottery roll on RNG`, and because the score is now
positive the forced-candidate branch zeroes the roll, so the cast is honoured:
`AIPlayerGPT: casting Sphinx's Revelation (model's pick)` -> `Announce the value of X ... X = 1` ->
`Action added to stack` -> `Resolving`. **Discriminator = X-affordability slack: 0 (veto) vs 1 (pass).**

**Trigger predicate and its corpus count:** *"the model picks an `{X}` spell in a window where
`maxAnnounceableX == 0`"*. Count this corpus: **1 window, 5 records** — the only `{X}` card in the
rotated pool is Sphinx's Revelation and only deck125 runs it. Pool count > 0, so this is a WATCH with
a live repro, not a probe request. Severity: MEDIUM (the vetoed play was itself bad — a 0-for-0
Revelation); **repro quality: EXCELLENT** (deterministic, one binary, control in the same game).

**Why it still matters at full severity:** the class is not "X spells". It is *any* model pick whose
Baka desirability score is negative — the pick is silently discarded, the prompt's own guarantee that
every listed option is legal and payable is violated, and the pilot is never told. Proposed fix: at
the GPT validation pass, either drop the `> 0` guard for `aiForcedCandidate`, or bypass the
desirability roll entirely for an explicit model pick (legality is already established by
`LegalActionsOracle`). Suggested regression fixture: an `{X}` spell with exactly its coloured pips
available and no slack.

## 7. THE 139v125 CRASH — what deck125 was doing, plus a line-level pointer

deck125's last decision is s28 (turn 14, opponent's Main phase 1): `Cast Essence Scatter` targeting
`Arboreal Grazer` on the stack; stderr confirms it resolved. deck125's own board at that moment was
**6 lands + Staff of Nin, 0 creatures** — it contributes no affinity card, no aura, no equipment and
no creature to the crashing state. deck125 was **ahead 28-16**.

The board state that IS unusual belongs to deck139: `Everquill Phoenix {g} (4/4) [flying, reach,
mutate] [mutated pile - ONE merged creature ...; 1 card underneath: Arboreal Grazer]` plus a
`Feather Eve [artifact]` token. The crash is `__dynamic_cast` inside `GameObserver::Affinity()`.
`src/GameObserver.cpp:1488` is an **unchecked** `dynamic_cast` dereference inside that function:

```cpp
vector<Targetable*> targetlist = mExtraPayment->costs[ec]->tc->getTargetsFrom();
for (...) { Targetable * cardMasked = *it;
            dynamic_cast<MTGCardInstance*>(cardMasked)->isExtraCostTarget = true; }
```

Crashing *inside* `__dynamic_cast` (rather than on a NULL return) indicates `cardMasked` is a
dangling pointer, i.e. `mExtraPayment` is holding a stale extra-cost target list pointing at a freed
card — consistent with a mutate merge having destroyed or merged card instances. Suggested first
moves for the step-1 lane: (a) null-check the `dynamic_cast` result before the write, (b) clear
`mExtraPayment` when a payment completes or is abandoned, (c) build a fixture that mutates onto a
host while an extra-cost payment is pending. NOT verified — offered as a pointer, not a diagnosis.

## 8. UNEXERCISED / honest nulls

- `attackers` and `blockers` seams: 0 windows, 6 games. Not testable at this seat, ever, while
  Emrakul is unreachable. **This seat must not be used as the witness for any combat-seam fix.**
- Mulligan: 0 mulligans taken; the `"having already taken"` rung is untested here.
- Supreme Verdict's `nofizzle` (can't be countered): never contested — no opponent countered it.
- Emrakul's `autostack` extra turn, annihilator 6, protection: 0 occurrences; unreachable by
  construction, not by variance. A probe deck (Emrakul + ramp) is the only way to exercise it, and
  that is a ROSTER/construction question, not a guide one.
- Final Judgment against a recursion deck: exercised once (vs146; the Angels show
  `moved from the opponent's exile to the opponent's zone` after being exiled — worth a separate
  look, but it is not this seat's item).

## 9. CONSTRUCTION FLAG (routes to the roster owner, not the guide)

deck125 has **one** win condition it can actually deploy: Staff of Nin, 2 copies, at `{6}`, dealing 1
per turn. Its nominal finisher costs `{15}` in a deck whose observed ceiling is 12 mana. Three of the
five completed losses ended with the opponent at 20, 20 and 19 life — the deck never had a plan
running (the wave-2 fast discriminator). The guide is written to play the deck it actually is (answer
everything, grind with Staff), but the seat is **CONSTRUCTION-CEILINGED**: the honest ceiling is
"answer everything and win on a 20-turn Staff clock". If the roster owner wants this deck to close,
the cheapest edits are replacing Emrakul with something castable off 26 lands, or adding a second
pair of Staff-class win conditions. Per the ceilinged-seat rule the guide is validated independent of
the win column.

## 10. WHAT THE GUIDE DOES (and the one thing it is built to fix)

The plan: answer every threat, resolve Staff of Nin, and win by pinging the opponent's face one
damage at a time while Elixir of Immortality shuffles the spent answers back into the deck.

The single biggest piloting problem: **the pilot believes its win condition is a card it can never
cast**, and it spends its answers and its reasoning budget on that belief — burning Path to Exile on
1/1s "while I survive to Emrakul" and rehearsing a 15-mana ramp plan in a deck with no ramp. Rule #1
replaces the plan; the answer-discipline and Staff rules give the interaction somewhere better to go.

## STOPGAP REGISTER (line, defect + citation, removal condition)

| guide line | defect it patches | removal condition |
|---|---|---|
| L54-55: "The stack prints only a name … The card's rules text is in the OPPONENT DECK list at the top of this prompt" | ledger **#4** — the stack line carries only a name, no type/cost/(P/T) | Delete when a stack line carries the spell's type/cost/(P/T), i.e. when `/usr/bin/grep -c "on the stack: .*{" <corpus>/*.jsonl` is non-empty. |
| L90-91: "If you choose Sphinx's Revelation and your very next prompt still shows Sphinx's Revelation in your hand with your mana unspent, cast something else this turn and take it again next turn" | ledger **#2/#8** — the chosen cast silently fails and the same ask repeats | Delete when the corpus shows zero `validation_reject_reask_exhausted` records and no duplicate back-to-back cast asks. |
