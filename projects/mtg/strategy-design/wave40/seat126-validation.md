# deck126 "Sanguine Blood" — wave-40 SEAT VALIDATION

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-110407/`
Seat logs: `*-ai_baka_deck126-0x*.jsonl` — 6 games (opponents 123 / 125 / 139 / 146 / 152 / 162).
Pilot: `qwen36-35b-a3b`. **359 records = 347 decisions + 6 gamestart + 6 gameend.**
**Fallbacks 0/347.** `answer_replaced` 0, `commit_retracted` 0, `reasoning_degenerate` max 0.009.
Reasoning tax: median **7,062 chars/decision** (baseline 6,984 — flat), total 2.57 M chars.
Record **3-3** (context only, not the verdict).

## ⚠ TWO SIMULTANEOUS CHANGES — attribution frame

This corpus is the first on **BOTH**:
- **CONSTRUCTION**: owner fixed `Forest (SHM) * 16` → `* 1`. `deck126.txt` now totals **60 cards /
  24 lands (40 %)**, verified by re-count. Baseline was **75 cards / 39 lands (52 %)**.
- **GUIDE**: the wave-39 `deck126_strategy.txt` is deployed at `bin/Res/ai/baka/`.

Plus **three engine fixes landed between corpora** and are themselves under test here:
ledger **#9** (Battlement label now scales), **#11** (Tribute `text=` gains "of their choice"),
**#12** (Perimeter Captain now Oracle-faithful `may`), and **E1/countered narration**.

Independent construction discriminator used throughout: the **Idyllic Tutor reveal-menu option
count** (it enumerates the library). Baseline 61 options on t9 / 58 on t19. This corpus:
**50 (t5), 49 (t4), 45 (t11), 37 (t12), 37 (t27)** — the list is roughly two-thirds the size at
comparable turns. Construction change confirmed in-game, not just on disk.

---

## 1. GUIDE PREDICTION §8(a) — THE BATTLEMENT UPKEEP-TAP LEAK — **VALIDATED**

| metric | wave-39 baseline | wave-40 | |
|---|---|---|---|
| upkeep windows offering the tap | 22 | **25** | opportunity preserved |
| **activations taken in Upkeep** | **22 / 22** | **0 / 25** | **VALIDATED** |
| activations taken anywhere | 22 | **2** (both Main phase 1) | |
| blocker windows opening with a self-tapped Battlement | 5 / 8 | **1 / 25** | |

Every one of the 25 upkeep windows answered `CHOICE: 0 (pass)`. Prediction (a) said "at or near
0/N"; the result is exactly 0/N.

**Ledger #9 shipped and the phase-keyed rule still fires against the new label.** The label now
reads `Add N green mana with Overgrown Battlement [cost: Tap] {card text: "Defender -- {T}: Add
{G} for each creature with defender you control."}` — **57/57 emissions have N equal to the
defender count on the rendered battlefield line** (forms observed: Add 1, 2, 3, 4, 5, 6, 7).
The guide's #1 rule keys on the **phase** ("At your Upkeep …"), never on the label's number, so
the fix did not break its anchor. **Ledger #9 → VALIDATED, closeable.**

**Attribution — GUIDE, against the grain of the engine change.** The construction change does not
touch this seam (Battlement ×4 in both lists; upkeep windows went *up*, 22→25). The engine change
runs the *wrong* way: the option that used to promise 1 mana now promises up to 7, i.e. it became
strictly more attractive, and the pilot declined it anyway. The only variable that could produce
0/25 is the guide's #1 rule.

### Residual (new, uncovered window)
Both surviving activations are in **Main phase 1**, which rule #1 does not name:
- `126v162` s17 t8 — tapped for 6, **cast Chromatic Lantern {3} at s18 with the mana**. Productive;
  no leak. (Colours line read `{g}` only at that moment, so the Lantern was also rule-#7.7-legal.)
- `126v139` s14 t9 — tapped for **4**, then answered **"Cast nothing right now"** at s15 with
  `Already in pool: {g}{g}{g}{g}` on the line. Four mana burned, and the *next* blocker window
  (`126v139` s16 t10) shows `Overgrown Battlement … [tapped - cannot attack or block this turn]`.
  This is the entire residual of the old F1 defect: **1 wasted activation, 1 lost blocker, in
  347 decisions** (baseline: 21 wasted, 5 lost blockers, in 202).

Guide-lane consequence for wave 41: rule #1's window identifier is a phase, and the leak has moved
one phase later. The minimal edit is to extend the rule's "ALWAYS FALSE" clause to any window where
the Battlement tap is not being spent on a cast *in the same decision*, not to add a second
phase-keyed rule.

---

## 2. COMBO LINE — assembly still deliberate; frequency gain is **CONSTRUCTION**-attributed

### Absence table (OFFERED / TAKEN / ARRIVED)

| game | Exquisite Blood on BF | Sanguine Bond on BF | **BOTH at once** | outcome |
|---|---|---|---|---|
| vs123 | t12 | t10 | **YES t12** | WIN t12, 42-0 |
| vs162 | t6 | t12 | **YES t12** | WIN t13, 40-0 |
| vs125 | t7 | t15 | **YES t15** | WIN t48, 80 to -10 |
| vs152 | t12 | t14 | **YES t14** | **LOSS t16**, -8 to 20 |
| vs146 | t13 — **exiled t15** | never drawn | no | LOSS t22, -8 to 18 |
| vs139 | t9 (destroyed ~t23), t27 | never drawn | no | LOSS t30, -7 to 21 |

**BOTH-at-once 3/6 → 4/6.** The wave-39 finding "BOTH at once: 3/3 → win, 3/3 → loss" is now
**3 wins / 1 loss** — see the vs152 null below.

**Attribution.** Eight combo pieces diluted across 60 cards instead of 75 is a ~25 % relative
density increase; first-piece-on-battlefield turns moved t9/t9/t11/t17/t25/never → t6/t7/t9/t12/
t13/t9, and the reveal-menu counts confirm the smaller library in-game. **The frequency gain is
construction.** The guide's contribution is negative-space: it produced **zero** declined-castable-
piece windows, so none of the construction gain was given back.

### Deliberateness — still deliberate, no reverse errors
- **Tutor target accuracy 5/5** (baseline 4/4). The one that looks wrong is not:
  `126v139` s94 t27 picked **Exquisite Blood** while EB had been on the battlefield since t9 —
  but the narration shows `Your Exquisite Blood was put into your graveyard` from s77 onward, and
  the battlefield line at s93/s94 carries neither piece. With neither on the battlefield, guide
  rule #3.4 says "pick either". Correct.
  `126v162` s40 t12 is the clean positive: EB on the battlefield since t6, tutor picked **Sanguine
  Bond**, the missing half. Combo completed the same turn; won t13.
- **Tutors cast the turn they were drawable in 5/5 games**; no tutor was held.
- **No reverse errors**: never tutored for a piece already on the battlefield, never cast the piece
  it already had, never declined a castable combo piece.
- The pilot names the loop unprompted in PLAN prose in all six games (PRESERVE-ledger item holds).

### Draw-vs-play attribution for the three losses
- **vs146** — assembly was *not* undrawn: Idyllic Tutor t11 → EB to hand → **cast t13, arrived** →
  **exiled by the opponent t15**. Sanguine Bond never drawn (0 of 2 seen). Routing: **opponent
  interaction + draw**, not play. (This corrects the wave-39 shape where all three losses were pure
  "never drawn".)
- **vs139** — EB arrived t9, **destroyed ~t23**, second copy tutored and cast t27. SB never drawn
  despite **both copies still in the library at s94**. Routing: **draw**, not play.
- **vs152** — see §2.1.

### 2.1 The one BOTH-at-once loss — **HONEST NULL, no win-button violation**
`126v152` had both enchantments from t14 and died t16. Rule #2 requires a life-gain *source*:
- s25 t14: blocked with Wall of Omens — a 0-power wall with **no Perimeter Captain out**; the block
  gains nothing. No loop available.
- s28 t15: **cast Perimeter Captain** (correct rule-#7.3 play, and the correct win-button setup).
- Narration at s29: `Your Perimeter Captain was exiled from the battlefield` **at the opponent's
  t17 upkeep, before the block resolved**.
- s29 t16: assigned **both** offered blockers; with the Captain gone there was no gain trigger.

No window existed in which a listed win-button option was declined, in this or any other game.
**Rule #2 → 0 violations in 4 BOTH-at-once games.**

`126v125` is the counterpart null: both pieces from t15, then **33 consecutive decisions** with the
loop live and *no legal life-gain line* (opponent held zero creatures, Staff of Nin **COUNTERED**
t~46, Sorin **COUNTERED**). The pilot correctly restated the wait each turn and fired the instant a
creature appeared — s98 t48, Tribute to Hunger, 20/50 → **80 / -10**. Win.

---

## 3. LEDGER #12 ARRIVAL TRACE (owed by #22) — **VALIDATED, #22's debt DISCHARGED**

`primitives/mtg.txt:85440` now reads `auto=@combat(blocking) source(creature[defender]|
mybattlefield):may life:2 controller` — Oracle-faithful.

**The GPT seat sees the menu.** It renders as a `CHOOSE_MENU` ask, not a Yes/No:

```
Choose an option for Perimeter Captain:
1. Life
2. Decline - do nothing
```

| metric | value |
|---|---|
| Perimeter Captain may-asks | **22** |
| accepted (`Life`) | **22 / 22** (0 declines, 0 fallbacks) |
| games with at least one | 4 of 6 (123 and 125 never had a Captain block) |
| **max asks in one combat** | **8** (`126v139` t24 — two Captains × four blocking defenders) |
| mean asks per combat | **2.75** (8 combats) |
| share of all seat decisions | **22 / 347 = 6.3 %** |
| inference spent on them | **1,377 s** (median latency 52.1 s/ask) |
| reasoning spent on them | **132,364 chars** (median 5,702/ask) |

**Accepted gains LAND — traced downstream, per attempt.** `126v139` t24, the 8-ask combat, life 61
before the window; the next record's narration reads, in order:

```
- You chose Life for Perimeter Captain
- You gained 2 life (now 63)   … ×8, ending "now 77" …
- Phase: Combat damage
```

**8 asks → 8 `You chose Life` echoes → 8 × `+2 life` → 61 → 77, all before combat damage.**
Same shape verified at `126v152` s6/s11 (`Perimeter Captain blocks Wolf` → `You gained 2 life
(now 22)`), `126v162` t11 (4 asks) and t13, `126v146` t4, `126v139` t28 (3) and t30 (3).
No prompt was raised without a matching gain; no gain arrived without a prompt.

**Ledger #22's owed item — "a rendered may ask for the GPT seat + accepted gains landing" — is
DISCHARGED by this corpus.** The #12 behaviour change shipped without a suite fixture; live
arrival-tracing now covers it. The suite-driver wedge itself is untouched and stays a wave-41 item.

**Attribution: ENGINE (#12), not guide, not construction.** The guide's Perimeter Captain text
("gains you 2 life for EACH defender you control that blocks") describes the *outcome* and did not
need to change; the pilot answered the new menu correctly with no guide support for it at all.
Feeds design item **#20 (mass accept/deny)**: this is a decision class with 22/22 identical
answers costing 6.3 % of the seat's decisions and 23 minutes of inference across six games.

---

## 4. TRIBUTE TO HUNGER — **VALIDATED** (prediction §8(c))

**Ledger #11 shipped.** The option line now reads, verbatim from the corpus:

```
Cast Tribute to Hunger {2}{b} - legal targets right now: the opponent
{card text: "Target opponent sacrifices a creature of their choice. You gain life equal to that
creature's toughness."}
```

| metric | baseline | wave-40 |
|---|---|---|
| PLAN lines naming a **specific opponent creature** as the victim | **3** | **0 fabricated** |
| PLAN lines mentioning Tribute at all | — | 66 |
| offered windows / casts taken | 54 / — | **116 / 4** |
| ~12 k-char reasoning blowups on the card | 3 | 0 |

The **one** PLAN naming a creature is `126v125` s97/s98 — *"cast Tribute to Hunger targeting
Emrakul"*. Checked against the board: the opponent battlefield line at s98 reads **"19 permanents
listed, of which 1 is a creature"**. That is the guide's own sanctioned case ("The opponent
controls exactly ONE creature — then their choice is your choice"), and it **won the game**
(20/50 → 80/-10). Not the F2 fabrication class.

The pilot also reasons correctly about the *empty*-board case in ~15 separate PLAN lines
("Opponent has zero creatures, so Tribute to Hunger cannot be cast for the win right now"), which
is the guide's last line of rule #4 executing.

**§7 stopgap register — confirmed unchanged.** Stopgap 1 (*"The opponent picks which creature dies,
not you"*) has **removal condition NONE**, and this corpus re-earns it: the `text=` fix landed AND
the belief is absent, but the two cannot be separated on one corpus, and the wave-39 register
already ruled the belief half survives any `text=` change. **Keep the line. Re-check next corpus.**
Stopgap 2 (none written against the Battlement label) is still correct — §1 shows the guide never
had to quote or contradict the label, and #9's fix changed nothing in the guide.

**Attribution: mixed and deliberately not split.** Both halves (engine `text=` fix and guide
rule #4) landed in the same corpus. Honest statement: the fabrication is gone; which half did it is
not resolvable here, and the register's "removal condition: none" is what makes that acceptable.

---

## 5. MULLIGAN / LAND BEHAVIOUR ON 24-of-60 — construction-attributed observations

| metric | baseline (39/75, 52 %) | wave-40 (24/60, 40 %) |
|---|---|---|
| mulligans taken | 0 / 6 | **0 / 6** (prediction §8(d) holds) |
| opening hands | mixed | **6 / 6 exactly "3 lands, 4 spells"** |
| land-play windows offered / taken | 51 / 53 | **51 / 52** |
| Idyllic Tutor reveal-menu size | 61 (t9), 58 (t19) | 50, 49, 45, 37, 37 |
| flooded stretches | `Play Forest` / `Cast nothing` runs, vs125 t13-29 | none observed |

- **Six identical 3-land/4-spell openers.** At 40 % lands, 3-of-7 is the modal draw; at 52 % it is
  not. n=6 is too small to be a distributional claim, but it is the shape the fix predicts, and it
  is recorded as a construction observation for the roster record, not as a result.
- **Every opener carried a 1- or 2-mana play** (`Cheapest spell in this hand:` read Perimeter
  Captain {w}, Pride Guardian {w}, Wall of Omens {1}{w} or Overgrown Battlement {1}{g}), and
  `Playing every land in this hand would cover the cost of:` named 4 cards in 6/6 hands. The guide's
  mulligan floor ("KEEP whenever N is 2 or more") was never stressed — **every hand cleared it by
  a full land.** The floor is untested, not validated.
- **`having already taken` — still 0 occurrences in this corpus** (0 mulligans taken). The guide's
  "last look" line remains **source-verified only** (`AIPlayerGPT.cpp:11756`). Carry the flag
  forward again; two consecutive 0-mulligan corpora mean this line has never been observed live.
- The old flood symptom is gone: `126v125` ran to t48 and shows no `Play Forest` / `Cast nothing`
  run of the wave-39 kind — its long tail is the combo-assembled-but-no-creature stall (§2.1),
  a different cause.

### Two guide rules that did NOT fully land (both guide-lane, neither construction)

**Rule #5 blocking floor — VALIDATED as a rate, 2 minor violations.**
**78 / 81 blockers assigned across 25 windows (96.3 %)**, up from **14 / 20 (70 %)**. Prediction
§8(b) said "rising from 14/20" — validated. The three idles:
- `126v139` s92 — Vampire, every line reads *"your blocker dies, attacker lives"*, neither
  enchantment on the battlefield. **Guide-compliant decline** (the rule's one closed exception).
- `126v146` s13 — Vampire declined, but its line offered **A2 (both die)**, which is not the
  excepted wording and is a favourable trade plus 1 lifelink life. **Minor VIOLATION.**
- `126v139` s98 — Vampire declined with **Exquisite Blood on the battlefield**; the guide says
  "any line at all once an enchantment is down — blocks". **Letter VIOLATION, ~zero material
  value** (lifelink gain does nothing with EB alone; only Sanguine Bond converts it).
Best window of the corpus: `126v139` t24, 5/5 assigned, **+17 life** (8 Captain triggers + walls).

**Rule #7.7 Chromatic Lantern — VIOLATED (seq), 4 of 5 casts.**
Offered 118 windows, cast 5 (baseline 5 of 21 — **rate 24 % → 4.2 %**), but the failure *shape*
survives intact:
| cast | `colours you can make:` at that moment | rule-legal? |
|---|---|---|
| `126v162` s18 t8 | `{g}` | ✅ yes — {b} absent |
| `126v162` s28 t10 | `{g}{u}{r}{b}{w}` | ❌ |
| `126v162` s29 t10 | `{g}{u}{r}{b}{w}` | ❌ **second Lantern, same turn** |
| `126v146` s37 t21 | `{g}{b}{w}` | ❌ |
| `126v146` s38 t21 | `{g}{u}{r}{b}{w}` | ❌ **second Lantern, same turn** |
Both violations are **double-casts in a single turn** — the exact wave-39 vs146 shape — and
`126v146` s37/s38 spent six mana at **2 life on turn 21** of a game it lost the next turn.
Rule #7.7's condition (`{b}` on the colours line) is correctly stated; what it does not cover is
"you already control a Chromatic Lantern". Wave-41 guide edit: make #7.7 a zero-or-one rule.

**Staff of Nin — 1 minor deviation.** 3 activations. `126v146` s30 aimed at **Kaya the Inexorable**
rather than the opponent's face, against the guide's "Always aim it at the OPPONENT". No combo was
assembled at that moment (Exquisite Blood had been exiled three records earlier), so the ping was
not a forgone win button; recorded, not ranked. The other two aimed at a player.

**Rule #6 attacking — VALIDATED, F4 closed.** **3 / 3** attacker windows attacked with the lifelink
Vampire (`126v146` s17, `126v139` s97, `126v139` s108). Baseline was 1/3 with the F4 decline.

---

## 6. NEW / CARRIED ENGINE + RENDER ITEMS FOR THE WAVE-41 LEDGER

**CLOSED THIS WAVE (verified in corpus, no wave-41 lane):**
- **#9 W39-BATTLEMENT — CLOSED.** Label scales; 57/57 emissions equal the defender count; guide
  anchor unbroken (§1).
- **#11 W39-TRIBUTE — CLOSED.** `text=` carries "of their choice"; 0 fabricated victim namings (§4).
- **#12 W39-CAPTAIN — CLOSED behaviourally.** May-menu renders, 22/22 accepted, gains arrival-traced
  (§3). The `text=`/behaviour disagreement is resolved in Oracle's favour.
- **E1 countered-vs-resolved — CLOSED.** The corpus now renders
  `Your Staff of Nin was COUNTERED by Dream Fracture and went to your graveyard` and
  `Your Sorin, Lord of Innistrad was COUNTERED by Fall of the Gavel …`. **The pilot consumed it**:
  `126v125` s72 PLAN reads *"Staff of Nin was countered by Dream Fracture, removing the intended
  win trigger."* Zero countered-spells-narrated-as-resolved in this corpus.

**N1 — [MEDIUM, MAGNITUDE TRUTH, ledger-#10 class] `{right now: life 0}` on both combo pieces.**
27 emissions this corpus: `Cast Sanguine Bond {3}{b}{b} {right now: life 0}` ×19 and
`Cast Exquisite Blood {4}{b} {right now: life 0}` ×8. `dynamicMagnitudes` evaluates the
*triggered, future-event* life change at cast time and emits 0. This is the same defect as ledger
**#10** (`{right now: draws 0}` on Dictate of Kruphix), but on the **only two cards that win this
deck the game** — the annotation asserts the deck's win condition is worth zero life.
**Latent, not yet a measured loss**: the pilot cast both every time they were offered (guide rule
#3.1 is louder than the annotation). Docketed MEDIUM for the same reason #10 was: three waves of
magnitude work have trained the pilot to prefer annotations to card text, so a wrong magnitude is
worse than an absent one. Ask: suppress the clause for triggered-on-a-future-event amounts, or name
the branch (`{on each life gain: opponent loses that much}`).

**N2 — [MEDIUM, PROMPT VOLUME / feeds design #20] Perimeter Captain may-ask fan-out.**
The `may` raises **one independent model round trip per blocking defender, per Captain** — 8 asks
in a single combat at `126v139` t24, mean 2.75/combat, 22 asks = **6.3 % of the seat's decisions,
1,377 s of inference, 132 k reasoning chars, for 22 identical answers**. This is the strongest
concrete case yet for design item **#20 (mass accept/deny)**: an "accept all N triggers" or
"remember this answer for this combat" affordance would recover the whole cost. Note the
interaction: #12 made the card Oracle-faithful and simultaneously created this tax — the fix is
correct, the batching is what's missing.

**N3 — [LOW, LABEL TRUTH] the may-menu option reads `1. Life` — no magnitude, no source.**
`Choose an option for Perimeter Captain: / 1. Life / 2. Decline - do nothing`. The label carries
neither the amount (2) nor which trigger raised it, and the same bare "Life" label would render for
any life-granting `may`. The deciding fact is not on the option (P1/P2/P4 doctrine). Ask:
`1. Gain 2 life (Perimeter Captain — a defender you control blocked)`. No loss measured (22/22
correct), but the pilot spent a median 5.7 k reasoning chars re-deriving the magnitude from the
battlefield line each time — see N2.

**N4 — [INFO, CARRIED, = ledger #15] blocker echo still has no disambiguator.**
`126v139` s87 narrates `Wall of Omens blocks Dryad of the Ilysian Grove; Wall of Omens blocks
Pollywog Symbiote; Perimeter Captain blocks Pollywog Symbiote; …` — identical names, no `#1/#2`,
while the B-lines and battlefield lines both carry them. Unchanged from wave 39. Legibility only;
every reply was legal and executed correctly. **Now cheaper to fix than it was**, because the
Captain may-echo (`- You chose Life for Perimeter Captain`, ×8 in a row) makes the ambiguity worse:
eight identical lines, no way to tell which defender each belongs to.

**N5 — [OBSERVATION, core/reply-protocol lane] wave-39 ledger #17 (PLAN-protocol churn) did NOT
reproduce at this seat.** Baseline had `126v152` s10 at 11,603 chars and `126v36` s10 oscillating
over PLAN rule (a)/(b)/(c). This corpus's two largest replies are `126v162` s47 (20,056 chars,
Main-phase priority) and `126v125` s56 (23,872 chars, Main phase 2 during the 33-turn stall) —
neither is PLAN-protocol litigation. Median reasoning is flat at 7,062 chars. **Ledger #17 stays a
watch item; it did not gain a second corpus of evidence.**

---

## 7. VERDICT TABLE

| # | item | verdict | attribution |
|---|---|---|---|
| 1 | §8(a) Battlement upkeep leak → 0/N | **VALIDATED** (0/25 upkeep; 22/22 baseline) | **GUIDE** (engine change ran the other way) |
| 1b | ledger #9 label scales, guide anchor intact | **VALIDATED** (57/57 label == defender count) | ENGINE |
| 1c | main-phase manual tap (1 wasted, 1 blocker lost) | **VIOLATED (seq)** `126v139` s14→s16 | GUIDE gap (rule #1 is upkeep-only) |
| 2 | combo assembly deliberate; tutor targets | **VALIDATED** (5/5 targets, 0 reverse errors) | GUIDE preserves; **CONSTRUCTION** drives frequency 3/6→4/6 |
| 2b | "BOTH at once → win" | **REVISED**: 3 W / 1 L | ENGINE/opponent (Captain exiled pre-block) — **NULL**, no misplay |
| 3 | ledger #12 may-asks + arrival trace (#22's debt) | **VALIDATED / DISCHARGED** (22 asks, 22/22 accepted, 8/8 gains landed 61→77) | ENGINE |
| 4 | §8(c) Tribute victim-naming → 0 | **VALIDATED** (0 fabricated; 1 sanctioned single-creature naming, won the game) | GUIDE + ENGINE, not separable — stopgap removal condition stays **NONE** |
| 5 | §8(d) mulligans hold at 0 | **VALIDATED** (0/6; 6/6 openers 3 lands/4 spells; lands 51/52) | **CONSTRUCTION** |
| 5b | `having already taken` line | **NULL** (0 occurrences, 2nd corpus running) | source-verified only |
| 5c | §8(b) blockers assigned/offered | **VALIDATED** 78/81 (96 %) from 14/20 (70 %) | GUIDE |
| 5d | rule #5 idle blockers | **VIOLATED (seq)** ×2 minor: `126v146` s13, `126v139` s98 | GUIDE (exception wording too narrow) |
| 5e | rule #7.7 Chromatic Lantern | **VIOLATED (seq)** 4/5 casts; 2 same-turn double-casts | GUIDE (rule lacks a zero-or-one clause) |
| 5f | rule #6 lifelink attacks (old F4) | **VALIDATED** 3/3 | GUIDE |
| 5g | rule #2 win button | **VALIDATED** 0 violations in 4 BOTH-at-once games | GUIDE |
| 6 | E1 countered narration | **VALIDATED & CONSUMED** by the pilot | ENGINE |

**Protocol health:** 0 fallbacks / 347, 0 answer_replaced, 0 commit_retracted,
`reasoning_degenerate` max 0.009. The parse/protocol stack is clean at this seat for the second
consecutive wave.
