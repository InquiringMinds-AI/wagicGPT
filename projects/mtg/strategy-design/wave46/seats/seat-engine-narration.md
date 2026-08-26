# Wave-46 seat file — ENGINE / NARRATION

Seat scope: no deck of my own, no `deck<N>/` outputs. I adjudicate **prediction set B**
(the wave-46 engine lanes F/G/H/I + lane E) across **all 42 seat logs / 2113 decisions** of
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-155842`, by the emitter's actual strings at
`master 486c83189` (verified: `git diff 486c83189 HEAD -- src/AIPlayerGPT.cpp src/DecisionContract.cpp
include/GptPlanCaveat.h` is EMPTY, so the working tree I read IS the corpus binary).
Baseline corpus for every delta: wave 45 = `matchups-20260826-131833` (1909 records).

Prediction set A (guide edits) is **UNTESTED by this seat** — I own no deck.

---

## 0. Headline numbers

| metric | w45 (131833) | w46 (155842) | delta |
|---|---|---|---|
| decisions | 1909 | 2113 | +10.7% |
| fallbacks | 2 (0.105%) | 6 (0.284%) | **+4 (see §1)** |
| total prompt chars | 15.89 M | 20.74 M | +30.5% |
| chars / decision | 8,707 | 10,222 | **+17.4%** |
| latency median | 16,498 ms | 15,298 ms | **−7.3%** |
| latency p99 | 145,604 ms | 120,599 ms | **−17.2%** |
| latency max | 538,990 ms | **900,047 ms** | see §2 |
| post-answer overrun mean | 312 | 257 | **−17.6%** |
| post-answer overrun median | 236 | 192 | **−18.6%** |
| priority windows reaching the model | 427 | 504 | +18% |
| priority take-rate | 47.8% | 62.3% | +14.5 pt (denominator effect, §3) |

The wave-46 render additions cost **+17.4% prompt chars per decision** and bought a
**−17.6% reduction in post-answer reasoning overrun** and a **−17.2% p99 latency**. That is
the trade the lanes were shipped for, and it landed.

---

## 1. PRIORITY REGRESSION #1 — fallbacks 2 → 6, five of them `stale_echo`

### 1.1 The parent hypothesis is REFUTED

> *"a NEW render string from lanes F/G/H/I (copies-left-behind, `{effect:}`, GANG BLOCK,
> per-colour counts) is being echoed and mis-read as a stale prior answer by the INDEX-WINS
> echo pass."*

**FALSE, 0/5.** I grepped every `stale_echo` reply for `was 1 of`, `still there`, `{effect`,
`GANG BLOCK`, `sources that can make each`, `X pricing`, `LIFE-TO-DAMAGE`, `lifelink:`,
`copy 1 of`, `copies in your hand`. **Zero hits in any of the five replies.** Every echoed
label is a plain CARD NAME:

| file (prefix) | seq | kind | turn/phase | echoed label | option at that index |
|---|---|---|---|---|---|
| `1787777934-…deck146-…-vs-…deck152` | 13 | ask | 8 / Main 1 | `Silverquill Command` | `you draw and sacrifice creature` |
| `1787777935-…deck162-…-vs-…deck126` | 18 | ask | 11 / Blockers | `Cast Forced Fruition` | `Vampire (1/1) [lifelink]` |
| `1787777938-…deck126-…-vs-…deck123` | 41 | ask | 16 / Main 1 | `Cast Exquisite Blood` | `Cast Idyllic Tutor {2}{w} …` |
| `1787777956-…deck146-…-vs-…deck130` | 23 | ask | 12 / Main 1 | `Silverquill Command` | `you draw and sacrifice creature` |
| `1787777968-…deck152-…-vs-…deck126` | 10 | ask | 4 / Blockers | `Cast Elite Spellbinder` | `Perimeter Captain (0/4) [defender]` |

The INDEX-WINS pass behaved exactly as documented (`AIPlayerGPT.cpp:10376-10380`, "label
foreign to that option (INDEX-WINS) is stale -> defer to the heuristic"). The regression is
upstream of it: **the prompts these five answered had no subject the model could name.**
Two distinct root causes, below.

### 1.2 HIGH — #W46-6 is NOT closed: the residual "A choice is required" class is 100% Silverquill Command, and 50% of it falls back

`A choice is required - choose an option:` appears **4 times** in the corpus (down from 9,
so lane I's `storedSourceCard` unwrap did work on the `ability$!…!$` dummy class). But **all
4 remaining are the same card** — deck146's Silverquill Command modal `auto=choice` menu —
and **2 of the 4 produced `stale_echo`**:

| file | seq | header | model's answer | outcome |
|---|---|---|---|---|
| `1787777934-…deck146-…-vs-…deck152` | 13 | bare | `CHOICE: 5 (Silverquill Command)` | **stale_echo** |
| `1787777934-…deck146-…-vs-…deck152` | 23 | bare | `CHOICE: 8 (you draw and sacrifice creature)` | ok |
| `1787777934-…deck146-…-vs-…deck152` | 39 | bare | `CHOICE: 4 (creature gains 3/3 and sacrifice creature)` | ok |
| `1787777956-…deck146-…-vs-…deck130` | 23 | bare | `CHOICE: 5 (Silverquill Command)` | **stale_echo** |

The card (`Res/sets/primitives/borderline.txt:102762`) is nine `auto=choice name(…)` lines.
Each parses to a `MayAbility(must=true)` (`MTGAbility.cpp:2896-2913`), they arm through
`ActionLayer::setMenuObject` (distinct menu ids, which is why the six option texts differ),
and `DecisionManager::buildMenuChoice` renders the CHOOSE_MENU header. The header's name
ladder is only two rungs (`AIPlayerGPT.cpp:13073-13077`):

```
ctxName = ctx->getDisplayName();
if (ctxName.empty() && ctx->model && ctx->model->data) ctxName = ctx->model->data->getName();
…
decision = !ctxName.empty() ? ("Choose an option for " + ctxName + ":")
                            : string("A choice is required - choose an option:");
```

**Proof the source IS recoverable at this exact moment:** the very next record's `events`
for the same act reads
`- You used: you draw and sacrifice creature with Silverquill Command`
(`1787777934-…deck146-…-vs-…deck152` seq 24). That line is built by
`abilityActivationNarration` over `resolveOwningCardName` (`AIPlayerGPT.cpp:3805-3814`),
whose ladder is **three** rungs: `getDisplayName()` → `c->name` → `storedSourceCard`
(recursive). So the engine names the card one tick later from the same ability; only the
header's shorter ladder fails.

Two independent defects, both one-liners:

1. **The header ladder is short.** It never tries `ctx->name` and never hops
   `storedSourceCard`. Fix shape: replace the two-rung inline ladder with
   `resolveOwningCardName(ctx)` (it already exists, already handles the dummy hop, and
   already returns `""` for "no real card", which is the exact fallback condition).
2. **The wave-45 E-3 rescue is gated on the wrong predicate.**
   `DecisionContract.cpp:358` reads `if (!req.contextCard && firstOptionAbility)`. It fires
   only when contextCard is a NULL POINTER. A contextCard that is non-NULL but **nameless**
   skips the rescue entirely. Fix shape: gate on the resolved NAME being empty, not on the
   pointer being null — i.e. resolve the name first, and fall through to
   `firstOptionAbility->source` (through the same `nameableCardPointer` rail) whenever the
   name comes back empty.

I could not run the binary (brief forbids running wagic), so which of (1) or (2) is the
live branch is **UNVERIFIED**; both are real gaps in the code as written, and (1) alone
closes the observed surface. **Decisive wave-47 probe:** add a PARSETEST case that drives
`buildMenuChoice` with a contextCard whose `getDisplayName()` is empty, `model == NULL`, and
`name == "Silverquill Command"`, and assert the header reads `Choose an option for
Silverquill Command:`.

**Same root, third surface — the history line.** 84 `You chose …` history lines in the
corpus: 82 name their subject (`… for Brightclimb Pathway`, `… 's mode "…"`, room lines);
**2 are subject-less**, and both are the same card:
`- You chose you draw and sacrifice creature` and
`- You chose creature gains 3/3 and sacrifice creature`. Fixing the ladder fixes all three
surfaces at once.

**Consequence audit (honest):** at `1787777956-…deck146-…-vs-…deck130` seq 23 the heuristic
substituted mode 4 for the model's intended mode 5. Both modes carry the `sacrifice creature`
leg and the opponent chose the same Goblin either way, so **this instance was not
game-deciding**; deck146 lost that game for other reasons. I am not claiming a lost game.

### 1.3 HIGH — the damage-assignment-order screen is being answered as if it were a cast menu

Two of the five `stale_echo`s (`1787777935-…deck162` seq 18, `1787777968-…deck152` seq 10)
are the **`Combat damage order:`** screen. That screen is old (commit `d8664d268`,
2026-07-01), but its failure rate moved:

| corpus | damage-order windows | fallbacks |
|---|---|---|
| w45 | 11 | 0 |
| w46 | **4** | **2 (50%)** |

The screen renders:

```
Combat damage order: your attacker Wolf (2/2) is blocked by 2 creatures. Damage is assigned
in order, up to each blocker's toughness. Choose the blocker dealt damage in position 1.
1. Perimeter Captain (0/4) [defender]
2. Wall of Omens (0/4) [defender]
```

…and then the **fixed** reply-format exemplar:

```
e.g. "CHOICE: 3 (Cast Example Card)" (a placeholder - copy a real number and short name from the list)
```

On a window whose option labels are bare creature names (or bare mode phrases, §1.2), the
only "Cast <name>"-shaped string in the pilot's context is its own PLAN, and that is what it
copies. Corpus-wide: replies whose `CHOICE:` label starts with `Cast ` into a menu where **no
option** starts with `Cast ` — w45 3/1662, w46 **4/1868, 2 of which fell back**. Every one of
the three "Cast X" stale echoes is this shape.

**Fix shape (render, not guide):** make the exemplar come from THIS window's option list
instead of the hard-coded `Cast Example Card` — e.g. echo option 1's own short name into the
example. That removes the only affirmative "Cast …" substring on a non-cast screen, which is
precisely the trust-doctrine rule ("no affirmative substring a model can latch"). It is
cheap, it is provable in PARSETEST, and it covers §1.2's menus too.

**Consequence audit:** neither instance changed a game. Three 1/1 Vampires (identical) and
two 0/4 defenders (neither dies) — the heuristic's order was materially equivalent.

### 1.4 The sixth fallback (`unparsed_reply` class) — none. The remaining one is §2.

---

## 2. PRIORITY REGRESSION #2 — one decision hit the 900 s timeout (`empty_reply`)

**Record:** `1787777926-ai_baka_deck123-0x5639f884b1d0-vs-ai_baka_deck130.jsonl` **seq 13**,
`kind=priority`, turn 8, Main phase 1, `latency_ms=900047`, `reply=""`, `choice=-1`,
`coded_answers=0`, `post_answer_overrun=0`.

**Prompt size: 5,645 chars — small** (corpus mean 10,222; this is in the bottom decile). Size
is NOT the cause.

**What the model was doing:** a **Thraben Doomsayer + Intruder Alarm token loop**. The window
offered exactly ONE option:

```
1. Create human with Thraben Doomsayer [cost: Tap] {card text: "{T}: Put a 1/1 white Human
   creature token onto the battlefield. -- Fateful hour - …"}
```

Intruder Alarm untaps the Doomsayer on every creature ETB, so the same window re-arms
without bound. The neighbouring records:

| seq | latency | option-row tail |
|---|---|---|
| 12 | 19,803 | *(no repeat tag)* |
| **13** | **900,047 → empty_reply** | ***(no repeat tag)*** |
| 14 | 20,534 | `[repeat: activated this turn 2 times already; …]` |
| 15 | 10,518 | `[repeat: activated this turn 3 …]` |
| 16 | 7,856 | `[repeat: activated this turn 4 …]` |
| 17 | 7,465 | `[repeat: activated this turn 5 …]` |

**Mechanism (MED, and the one concrete fix here):** `kRepeatActivationFloor = 2`
(`AIPlayerGPT.cpp:8963`), and `repeatActivationNote` returns `""` while
`aa->counters < floor` (`:9022`). So the **first repeat** of a loop carries **no marker at
all** — seq 13's option row is byte-identical to seq 12's, in a phase whose state also has
not changed on the option row. The model was handed the same question twice with no
differentiator and produced nothing for 900 s. The moment the counter reached 2 and the
annotation appeared, latency collapsed monotonically (20.5 s → 10.5 → 7.9 → 7.5 s): **the
annotation demonstrably terminates the spiral**; it just starts one activation too late.

**Fix shape:** `kRepeatActivationFloor = 1`, so the FIRST repeat says
`[repeat: activated this turn 1 time already; you control N creatures]`. Note the singular
("1 time"/"1 times") needs the plural branch the current string lacks. Cost is one short
clause on exactly the windows that spiral. The existing key-stripping
(`stripRepeatAnnotation`) already protects the pass-decline memories from the moving digits,
so nothing else moves.

I did **NOT** verify server-side (no vLLM logs were harvested), so "decode spiral" is
inference from the empty body + 900 s wall time; the option-row identity is fact.

---

## 3. #W46-7 mana-only priority auto-pass — 180 windows skipped

- `mana_only_windows_skipped` **present in all 42 gameend records**. **PASS.**
- Total skipped **180**, concentrated in **3 seats, all deck126**: 98
  (`…deck126-…-vs-…deck123`), 54 (`…-vs-…deck130`), 28 (`…-vs-…deck125`); 39 seats report 0.

**"mana-only priority windows reaching the model 0/N" — PARTIAL PASS (3 leaked).**
Across the 3 skipping seats, 33 priority windows survived: 13 MIXED (a mana option beside a
real action), 17 with no mana option, and **3 all-mana** (`…deck123` seq 24; `…deck125` seq
28 and 30). All three leaked through the *payment half* of the gate
(`AIPlayerGPT.cpp:10945-10952` requires `!getManaPool()->getConvertedCost()`) — each had
mana already floating, which the comment says is deliberately kept for the model. So the
gate is behaving as written; the residual is 3/183 ≈ 1.6% and is a **LOW** ("a Main-phase-2
window whose only option is a mana ability and whose float has nothing to buy is still a
wasted trip").

**"no skipped window had a non-mana option" — PROVEN to the limit of the evidence.**
Skipped windows leave no record, so this is proved two ways:
1. **By the emitter.** `isManaOnlyAction` (`AIPlayerGPT.cpp`, just above the gate) is
   *type*-exact: `AManaProducer`, or a `GenericActivatedAbility`/`AForeach` wrapping one, and
   nothing else. A multi-effect ability cannot match. The gate breaks on the FIRST
   non-matching option (`allManaOnly = false; break;`).
2. **By reconstruction from the surviving windows.** In every game with skips I traced the
   opponent-visible history forward. Sorin, Lord of Innistrad's loyalty abilities and Staff
   of Nin's ping are the only non-mana permanents deck126 had; **13/13 windows in which
   either coexisted with the Battlement survived and were asked** (specimen:
   `…deck126-…-vs-…deck123` seq 14/20/27/42/45 all show the three Sorin rows *beside*
   `Add 3 green mana with Overgrown Battlement`). The one turn in which Sorin was NOT
   activated (log turn 17) is accounted for: both of that turn's windows (seq 42, seq 45)
   were asked and the model answered `pass` — the option was offered and declined, not
   hidden. **0 lost non-mana options.**

**"take-rate on surviving windows unchanged" — NOT SUPPORTED as stated.** Raw priority
take-rate rose 47.8% → 62.3%. That is the expected denominator effect (180 guaranteed-`pass`
windows removed), not a behaviour change, but the prediction as written is falsified by the
number it named. Restate for wave 47 with the mana-only class excluded from both corpora.

**#W46-1's behavioural half, measured here because the gate changed its base rate:**
- *"`Add N green` taken with no cast 0"* — **FAIL, 1 of 1.** The corpus contains exactly ONE
  priority window where the model took a mana ability: `…deck126-…-vs-…deck123` **seq 20**
  (turn 10, Main 2). It tapped Overgrown Battlement for 3 green, then at seq 21 answered
  `Cast nothing right now` and passed. Its own reply shows the confusion verbatim:
  *"cannot tutor for the combo pieces with only {W} and {B} available and no white source
  untapped (Savannah is tapped? No, Savannah is untapped…"*. Wave 45 offered 39 such windows
  to deck126; wave 46 offered ~3 and it took 1 — the gate removed nearly all the exposure but
  did not remove the misread.
- *"coloured-pip overcommits 0/N"* — **PASS at N=1** (the single take did not fail a cast;
  it simply wasted the tap). Low power; carry.

---

## 4. #W46-1 per-colour source counts — render correctness

**PASS, 1708/1708 exact.** Every window carrying `sources that can make each:` was recounted
from its own `Those sources, one per untapped card:` sub-list, paren-aware, with the
`| Already in pool:` tail excluded:

- colour-count mismatches: **0 / 1708**
- source-total mismatches (`N total` vs sub-list item count): **0 / 1708**
- the stated convention holds on the specimens that test it: `Savannah {w} or {g} or {r} or
  {u} or {b}` is counted under all five, and `Overgrown Battlement {g} (VARIABLE output…)`
  counts as ONE source, not as its mana.

(The brief asked for a 30-window sample; I ran the whole 1708 because it was cheap. The
first 30-sample pass reported one "mismatch" that was my splitter breaking inside the
VARIABLE-output parenthetical — the counts were right.)

**LOW nit:** `manaAvailableLine`'s number-word only covers 0-10 (`kNumWord[]`), so a big
board reads `Mana available: 25 total (untapped sources, tapped automatically…` — the
count-word slot silently vanishes above 10 (specimen: `…deck125-…-vs-…deck152` seq 170,
25 sources). Not a falsehood; a small grammatical seam on exactly the widest boards.

---

## 5. #W46-2 copies-left-behind clause — render correctness

- Clause present: **5,904 lines across 1,154 windows** (w45: 0).
- **Correctness: 107 hand-zone clauses cross-checked against the same record's `Your hand:`
  list. 96 matched exactly. All 11 apparent mismatches are snapshot lag inside the same
  `events` block** (an intervening draw/discard/cast of that same name between the clause and
  the snapshot) — automated filter for a later same-name event leaves **0 unexplained**. Two
  audited by hand (`…deck125-…-vs-…deck123` seq 41, `…deck125-…-vs-…deck152` seq 168 where
  Sphinx's Revelation drew a second copy in the same block). **0 falsehoods.**
- **"no 'contradiction' reasoning about the hand list" — PASS. 5 → 0.** Wave 45 had five
  replies containing an explicit contradiction arbitration, including the verbatim
  *"This is a contradiction. Usually, the current hand list is the ground truth."*
  Wave 46 has **zero** replies containing the word at all. This is the cleanest single result
  in the set.
- **"p99 latency on 2-option asks < 120 s" — PASS.** n=619, median 17.3 s, p90 52.7 s,
  **p99 117.8 s**, max 453.2 s (w45: n=488, p99 141.3 s, max 539.0 s).

---

## 6. #W46-3 `{effect:}` on opponent non-creature permanents + converter line

- **"textless 0/N" — PASS.** Over 1,958 windows carrying an `Opponent battlefield` line I
  extracted every non-creature, non-land permanent, looked its name up in
  `Res/sets/primitives/*.txt`, and kept those with both an `auto=` script and a `text=` line.
  Grouping per NAME (the clause deliberately rides the first copy only):
  **1,320 name-groups covered, 0 uncovered.**
- **Clause fidelity — PASS, 21/21.** Every distinct `(name, clause)` pair in the corpus is an
  exact prefix of that card's `text=` line in the primitives (truncated pairs compared
  ex-`...`). No invented text.
- **"converter line on a non-attackers window in every game with a converter on board" —
  PASS, 10/10.** Sanguine Bond reached a battlefield in 10 seat-logs; every one of them
  carried `LIFE-TO-DAMAGE CONVERTER` on at least one non-attackers window
  (**147 non-attackers windows corpus-wide**, vs 5 total windows in w45). The one seat that
  looked like a gap (`1787777959-…deck146-…-vs-…deck126`) has **Exquisite Blood** on board,
  not Sanguine Bond — Exquisite Blood is loss→gain, not gain→loss, so it is correctly outside
  `lifeToDamageConverterScript`'s class. **Not a defect.**

### `{effect:}` budget on wide boards — measured

| distinct effect-bearing names on the opponent line | windows | median longest clause | median opponent-line length | truncated clauses |
|---|---|---|---|---|
| 1 | 727 | 96 | 357 | 215 |
| 2 | 269 | 138 | 636 | 95 |
| 3 | 27 | 133 | 820 | 16 |
| 4 | 2 | 92 | 705 | 4 |

Max clause length observed **143 chars** (budget 140 + the `...`), max distinct names **4** —
so the 90-char tier was used and the **55-char tier (7+ names) was never reached**. Total
`{effect:}` cost ≈ 1,354 clauses × ~110 chars ≈ **149 k chars over 2113 decisions ≈ 70
chars/decision**, i.e. ~4.6% of the +17.4% per-decision growth. **The budget is not the cost
driver; board width is** (widest opponent line 2,918 chars, 30 permanents, ONE effect clause).

### MED (new) — the effect clause truncates planeswalkers mid-ability, hiding the ultimate

`boardEffectSnippetLen` is width-aware but not shape-aware, and a planeswalker's rules text
is a list of loyalty abilities. Observed truncations (count = windows rendered that way):

- `Sorin, Lord of Innistrad` ×81 → `… -2: You get an emblem with "Creatures you control get...` — the emblem's actual effect and the **entire −6 ultimate** are gone.
- `Lolth, Spider Queen` ×83 → cut inside `-3: Create...`; the −8 is gone.
- `Ob Nixilis, the Hate-Twisted` ×18 → cut inside `-2: Destroy target creature. Its...`.
- `Teferi, Who Slows the Sunset` ×32, `Kaya the Inexorable` ×8, `Ranger Class` ×34, `Lightning Greaves` ×72.

It truncates **visibly** (`...`), so it is not a falsehood and not a doctrine breach. But the
opponent's planeswalker ultimate is the class of fact that decides late games, and it is the
half being dropped. Proposal for wave 47: give a permanent whose text splits on ` -- ` into
loyalty-ability clauses a per-CLAUSE budget rather than one flat prefix (keep the last
clause, which is the ultimate), or exempt planeswalkers from the width tiers. Repro:
`1787777938-ai_baka_deck126-0x5618ce439fc0-vs-ai_baka_deck123.jsonl` seq 14 onward.

### MED (new) — `{effect:}` is opponent-only, and the model's OWN combo pieces stay textless

`describeZoneCards(…, effectText=true)` is passed at exactly one call site,
`AIPlayerGPT.cpp:8640` (opponent `inPlay`); the own-battlefield call at `:8635` passes the
default `false`. That is defensible for creatures and for anything with an option row — but
**deck126's own Sanguine Bond, Exquisite Blood and Staff of Nin render bare on `Your
battlefield:`** while the opponent's identical cards carry their text. Direct repro: the
`stale_echo` at `1787777938-…deck126-…-vs-…deck123` **seq 41** shows
`Sanguine Bond {3}{b}{b} [enchantment]` bare on the model's own board, in the same prompt
where the opponent's `Intruder Alarm` carries a full `{effect: …}`. The model was
re-deriving its own combo from memory in the PLAN field on every window of that game.
Note the whole #W46-3 evidence chain (deck125 dying to an ASSEMBLED Bond + 2× Blood) is a
combo the OWNER of the pieces also has to reason about. Proposal: extend `effectText` to the
own battlefield for non-creature, non-land permanents that have **no option row this
window** (a permanent whose ability is already offered as a choice needs no gloss).

---

## 7. #W46-5 blockers-screen GANG BLOCK mirror

- Blockers windows: **30** (w45: 33). `GANG BLOCK` reached the blockers screen in **1** of
  them; it remains overwhelmingly an attackers-screen string (40 attackers windows).
  The one blockers instance is `1787777973-…deck123-…-vs-…deck152` **seq 10**, and its
  arithmetic is correct: `A1. Briarbridge Tracker (3/4)` with blockers `Human (1/1)` and
  `Bloodline Keeper (3/3)` → `GANG BLOCK: any 2 of yours together deal 4, enough to kill this
  attacker` (1+3 = 4 ≥ toughness 4). **PASS.**
- **"hand-derived group arithmetic in blockers replies 0/N" — PASS, 0/30.** One reply
  (`…deck126-…-vs-…deck152` seq 39) contains `5+3=8`, but that is LIFE-total arithmetic about
  the Bond/Blood loop, not blocker-power arithmetic. No reply re-derives a group's power.
- **"all-`(your blocker dies)` windows with a lethal legal group and no price 0/N" — PASS,
  0/13.** Thirteen windows have every B-line reading `your blocker dies`. I summed the listed
  blockers' powers against each attacker's toughness in every one: none has a lethal legal
  group (8 of the 13 are 0-power defender walls; the closest call is
  `…deck162-…-vs-…deck126` seq 20, three 1/1 Vampires = 3 power against a 3/**4** Fate
  Unraveler — correctly NOT priced). **No unpriced lethal group anywhere.**
- **Token cost on that screen (no collapse threshold) — MEASURED, and it is the one place the
  wave cost time:**

| | w45 | w46 |
|---|---|---|
| blockers situation-tail chars, mean | 4,022 | **4,431 (+10.2%)** |
| blockers tail p90 | 5,381 | **6,567 (+22.0%)** |
| blockers latency mean | 35,797 ms | **45,964 ms (+28.4%)** |
| blockers latency p90 | 55,827 ms | **87,181 ms (+56.2%)** |
| blockers latency max | 157,111 ms | **257,701 ms** |

N=30 vs 33, so this is low-power and I am **not** calling it a regression. But it is the
only decision kind that got both bigger and slower, and it is the screen that shipped without
a collapse threshold. Attackers moved the same direction (mean 25.4 s → 32.0 s). Flagging as
the wave-47 watch item with a pre-registered test (§10).

---

## 8. #W46-9 lifelink tail bound to its branch — PASS

82 lifelink tails in the corpus, in four exact shapes:

| count | shape |
|---|---|
| 31 | `(lifelink: they gain N)` |
| 25 | `(lifelink: you gain N from this block only, and this attacker deals nothing to their life)` |
| 16 | `(lifelink: you gain N)` |
| 10 | `(lifelink: they gain N from this block only, and this attacker deals nothing to your life)` |

- **Every one is on an `A<n>.`/`B<n>.` trade line — 0 loose tails** anywhere else in any
  prompt. Kind distribution: 56 attackers, 26 blockers; no other kind.
- **"replies adding keyword benefit to face damage 0/N" — PASS, 0.** I scanned every reply in
  the corpus for a sentence containing `lifelink` together with any of
  `unblocked / to the face / face damage / goes through / gain N life from the attack`.
  **Zero hits.** The two replies that reason about lifelink at all
  (`…deck126-…-vs-…deck162` seq 20, `…deck126-…-vs-…deck152` seq 27) both correctly scope the
  gain to blocking.
- Magnitudes spot-checked: `Intrepid Adversary (4/2) [lifelink]` blocked by a 1/1 → `they
  gain 4 from this block only` (attacker's power, not the blocker's toughness — correct for
  Wagic's damage assignment). `Vampire (1/1) [lifelink]` blocking a 3/4 → `you gain 1`.

---

## 9. #W46-6 headers, #W46-8 X collapse, lane E

### #W46-6 — `A choice is required` headers: 9 → **4**. PARTIAL. See §1.2 (all 4 are one card, 2 fell back).

### #W46-8 X casts above a collapsed equal-kill row — PASS
- 21 ANNOUNCE_X windows; **21 carry `{X pricing: same kills as X=N, for M less mana}`**.
- **`same as X=` (the bare wave-45 shape) appears 0 times.**
- 4 windows actually offered a collapsed cheaper row. **The model chose the cheaper collapsed
  row in 4 of 4** and never the priced reference above it — the exact wave-45 failure
  (deck130 vs146 seq 17→18, plan said X=1, answer was X=2 for an identical kill list) did not
  recur. **0/4.**

### Lane E (#W45-20) prefix coverage — PASS, decisively
Full game-log replay of the final prompt in each of the 42 seats (9,302 history lines):

| | w45 | w46 |
|---|---|---|
| history lines | 8,294 | 9,302 |
| unprefixed lines | **586** (106 distinct shapes) | **4** (1 shape) |
| unprefixed **card-SUBJECT** lines | 586 | **0** |

The 4 survivors are all `- It became Day`, a game-wide CR 730 designation with no card
subject — correctly untagged. Every `… dealt N damage to …` line now opens on
`Your <card>` / `Opponent's <card>`.

**Damage OBJECT still bare — confirmed as declared, and quantified: 128 lines** (`Human` 24,
`Wall of Omens` 16, `Perimeter Captain` 14, `Pride Guardian` 12, `Goblin` 10, `Vampire` 6,
`Thraben Doomsayer` 4, `Sorin, Lord of Innistrad` 4, …). **LOW, not MED, in this pool:** I
checked every one of the 128 against the two battlefield lines in the same prompt and found
**0 cases where the bare object name was present on BOTH battlefields at that moment** — so
no instance was actually ambiguous here. The mirrored-pool hazard is structural, not
realised. Close it when a cheaper occasion arises.

---

## NEW -> wave-47 docket

(§10 — engine / render items, ranked, each with a repro file + seq)

| # | rank | item | repro (file prefix, seq) |
|---|---|---|---|
| E-1 | **HIGH** | #W46-6 residual: the CHOOSE_MENU header's name ladder is 2 rungs (`getDisplayName` → `model->data`) where `resolveOwningCardName` is 3 and already solves it; AND `DecisionContract.cpp:358`'s E-3 rescue is gated on a NULL POINTER instead of an empty NAME. 4 bare headers, 2 `stale_echo`, 2 subject-less history lines — all one card, all one root. Fix: use `resolveOwningCardName` in the header; re-gate the rescue on the resolved name. | `1787777934-…deck146-…-vs-…deck152` seq **13**, 23, 39; `1787777956-…deck146-…-vs-…deck130` seq **23** |
| E-2 | **HIGH** | The reply-format exemplar is the fixed string `"CHOICE: 3 (Cast Example Card)"` on **every** window, including menus with no cast option. It is the only affirmative `Cast …` substring on such a screen and the model latches it: 4 corpus replies, **2 fallbacks**. Fix: build the exemplar from THIS window's option 1. Closes E-1's menus too. | `1787777935-…deck162-…-vs-…deck126` seq **18**; `1787777968-…deck152-…-vs-…deck126` seq **10** |
| E-3 | **MED** | `kRepeatActivationFloor = 2` leaves the FIRST repeat of an unbounded activation loop with a byte-identical option row. One 900 s `empty_reply`; latency collapsed 20.5→7.5 s the moment the tag appeared. Fix: floor = 1 (add the singular plural branch). | `1787777926-…deck123-…-vs-…deck130` seq **13** (cf. 12, 14-17) |
| E-4 | **MED** | `{effect:}` truncates a planeswalker mid-loyalty-ability, dropping the ultimate (Sorin ×81, Lolth ×83, Ob Nixilis ×18, Teferi ×32, Kaya ×8). Visible truncation, so no falsehood — but the dropped half is the game-deciding half. Fix: split on ` -- ` and budget per clause, keeping the last. | `1787777938-…deck126-…-vs-…deck123` seq 14+ |
| E-5 | **MED** | `{effect:}` is opponent-only (`AIPlayerGPT.cpp:8640` vs `:8635`); the model's OWN Sanguine Bond / Exquisite Blood / Staff of Nin are bare while the opponent's identical cards are glossed. Fix: extend to own non-creature, non-land permanents that have no option row this window. | `1787777938-…deck126-…-vs-…deck123` seq **41** |
| E-6 | **LOW** | 3 all-mana priority windows leaked the #W46-7 gate via the floating-mana half. Correct by the comment; still 3 wasted trips. | `…deck126-…-vs-…deck123` seq 24; `…deck126-…-vs-…deck125` seq 28, 30 |
| E-7 | **LOW** | `manaAvailableLine`'s `kNumWord[]` stops at 10, so >10 sources renders `25 total (untapped sources, …` with the count-word slot gone. | `1787777970-…deck125-…-vs-…deck152` seq **170** |
| E-8 | **LOW** | Damage OBJECT still bare on 128 history lines (lane E's declared residual). 0 realised ambiguities in this pool. | any seat; `- Your Staff of Nin dealt 1 damage to Human` |

**New render/narration FALSEHOODS found: ZERO.** Every wave-46 string I could check against
ground truth checked out — 1708/1708 mana counts, 107/107 copies clauses, 21/21 effect
clauses vs primitives, 82/82 lifelink tails on their branch, 10/10 converter games, 21/21
X-pricing rows, 0 unprefixed card-subject history lines. The two HIGH items above are
**omissions and a mismatched exemplar**, not lies.

---

## 11. Lategame specimen candidate

`1787777970-ai_baka_deck125-0x557f248fbfe0-vs-ai_baka_deck152.jsonl` **seq 170**
(turn 61, Upkeep, life 85 vs 5, **29,161-char prompt — the largest in the corpus**,
latency 11.6 s). It exercises simultaneously: a 25-source mana line with the per-colour
clause and the >10 number-word seam (E-7), a 28-permanent own battlefield with run-collapsed
entries (`Island #4-#6 x3`), a 21-permanent opponent battlefield, a live stack entry, two
own Staff of Nin rendered **without** `{effect:}` while their ability text appears on the
option row (E-5), and the model still answered in 11.6 s. Good stress specimen for any
wave-47 width work.

---

## 12. Falsifiable predictions for wave 47

1. With E-1 shipped, `A choice is required - choose an option:` = **0/N** in the next corpus,
   and subject-less `- You chose <label>` history lines = **0/N**.
2. With E-2 shipped, replies whose `CHOICE:` label starts with `Cast ` into a menu with no
   cast option = **0/N** (baseline 4/1868), and `stale_echo` fallbacks = **0**.
3. With E-1 + E-2 shipped, total fallback rate returns to **≤ 0.11%** (w45's level) at
   comparable N.
4. With E-3 shipped (`floor = 1`), **no decision exceeds 300 s** on a single-option priority
   window, and the first repeat of any activation loop carries a `[repeat: …]` tag: **1/1**.
5. **Blockers-screen watch (pre-registered so it cannot be read post hoc):** blockers latency
   p90 in the next corpus is **≤ 60,000 ms** and the blockers situation-tail mean is
   **≤ 4,600 chars**. If either exceeds, the GANG BLOCK mirror needs the collapse threshold
   the attackers screen has. N will again be ~30, so treat a single-corpus result as a signal
   to size a longer run, not as proof.
6. With E-4 shipped, no `{effect:}` clause on a planeswalker ends inside a loyalty ability:
   **0/N** clauses whose `...` falls after a `+N:`/`-N:` marker with no closing sentence.
7. #W46-7 holds: `mana_only_windows_skipped` > 0 on every deck126 seat, all-mana windows
   reaching the model **≤ 2%** of the mana-only class, and **0** non-mana options lost
   (re-proved by the same mixed-window reconstruction).
8. Restated correctly for #W46-7: priority take-rate **excluding all-mana-only windows** is
   within ±5 pt of w45's same-basis figure.

---

## 13. What I did NOT check

- **I did not run the binary.** No PARSETEST, no suite, no wagic invocation (brief forbids).
  Every "the emitter says X" claim is from reading `src/AIPlayerGPT.cpp` /
  `src/DecisionContract.cpp` / `src/ActionLayer.cpp` / `src/MTGAbility.cpp` /
  `src/AllAbilities.cpp` at the corpus commit; every "the corpus shows X" claim is from the
  JSONL. **Which of E-1's two branches is the live one is therefore UNVERIFIED** — both are
  gaps as written; §1.2 gives the probe that decides it.
- **The 900 s timeout's server side.** No inference-server logs were harvested with this
  corpus, so "decode spiral" is inference. The identical option row is fact.
- **`Clue` tokens** (23 opponent-board entries) are not in `Res/sets/primitives/*.txt`, so
  they are outside my `{effect:}` coverage audit. Token bookkeeping text is deliberately
  excluded by `isEngineTokenText`; I did not verify that path.
- **Set A (guide edits) — not adjudicated.** I own no deck; those windows belong to the deck
  seats.
- **Mulligan / bottoming as deck evidence.** I checked the 8 `bottom` records for render and
  fallback health (all clean, 7 options each, 8-42 s, 0 fallbacks) but did not do the
  per-seat kept-land / mull-chain accounting the new-baseline rules ask of the deck seats.
- **Own-battlefield `{effect:}` cost.** E-5's proposal would add prompt chars; I did not
  estimate how many, because the "no option row this window" filter's hit rate is not
  measurable from the corpus.
- **Deck-level win/loss attribution.** deck126 went 5/6; I did not test whether the
  auto-pass or any render change contributed, and per the new-baseline rules I would not.
