# seat 162 "Draw and Die!" — wave-40 VALIDATION of the wave-39 Step-0 guide

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-110407/`, deck162 seat, 6 games, pilot
`qwen36-35b-a3b`. Baseline: wave-39 GUIDELESS corpus `matchups-20260823-040744` (findings.md).
This is the **first guided corpus for this seat** — corpus 1 of the crutch-lifecycle bar, not 2.

**Corpus integrity**: 6 `gameend` records, 6 natural life-total kills, no draws, no cap
adjudications. **0 fallbacks** across 134 decisions (0 `unparsed_reply`, 0 `empty_reply`).
Kinds: 113 `ask`, 5 `blockers`, 4 `attackers`, 6 `gamestart`, 6 `gameend`.

**Record (context only, not the verdict)**: deck162 went **2/6** (wins vs deck123 at 31 life and
vs deck125 at 19 life; losses to 152, 126, 139, 146). Baseline was 3/6. Per the wave-31 rung the
win column is a noisy signal at a seat whose decision quality moved; every verdict below is keyed
to decision evidence, and the two wins are the two games with the deck's full engine assembled.

---

## ITEM 1 — THE PUNISHER-BEFORE-ENGINE BRAKE — **VALIDATED** (with one countable violation)

Method: every `ask` carrying "which card do you cast now" was scored for (a) punishers on
`Your battlefield` (Underworld Dreams / Fate Unraveler / Ob Nixilis), (b) punisher options in the
cast list, (c) symmetric-engine options, (d) `chosen_text`. 62 cast windows scored.

### 1a. Prediction 1 — punisher offered, symmetric engine taken instead, **zero** punishers out

Baseline: **3** (all in `139v162`, wave-39 §3a). This corpus: **0**. **VALIDATED.**

Every one of the 6 windows in which a punisher appeared in the cast list with an empty punisher
line was answered with the punisher, same ask, in the same reply:

| game | seq | turn | punisher offered | engines also offered | chosen |
|---|---|---|---|---|---|
| vs152 | s11 | T7 | Underworld Dreams | Howling Mine | **Underworld Dreams** |
| vs123 | s11 | T6 | Fate Unraveler | Howling Mine, Master of the Feast | **Fate Unraveler** |
| vs123 | s23 | T14 | Ob Nixilis | Howling Mine | **Ob Nixilis** |
| vs126 | s9 | T7 | Fate Unraveler | Mine, Dictate, Master of the Feast | **Fate Unraveler** |
| vs139 | s13 | T9 | Underworld Dreams | Mine, Puzzle Box, Master of the Feast | **Underworld Dreams** |
| vs125 | s17 | T13 | Underworld Dreams | Howling Mine, Forced Fruition | **Underworld Dreams** |
| vs146 | s13 | T9 | Fate Unraveler | Howling Mine | **Fate Unraveler** |

The wave-39 signature reasoning ("Master of the Feast is the engine/threat", "it doesn't strictly
matter which order") does not appear anywhere in this corpus.

### 1b. The one exception window fired exactly as written — **VALIDATED**

`146v162` **s16, T11, life 7, opponent battlefield shows 4 creatures.** Ob Nixilis was offered;
the pilot cast **Fog Bank** and wrote *"Cast Fog Bank now to survive. Next turn, cast Ob Nixilis to
establish the punisher."* This is Rule 1's named exception (life ≤ 8 AND opponent creature) taken
verbatim, with the deferral stated. **Not a violation.**

### 1c. The countable brake ("one Howling Mine max while zero punishers out") — **1 VIOLATION**

Scored: an engine cast while `bfPunishers == 0` and ≥1 draw engine already on the battlefield line.

- **VIOLATED — `146v162` s9, T7, life 19/20.** Battlefield already held one Howling Mine, zero
  punishers. Options were Howling Mine + Shield Sphere-less list; chose **`CHOICE: 1 (Cast Howling
  Mine)`**, `PLAN: Cast Howling Mine now. Next turn, cast Ob Nixilis and play lands.` No punisher
  was payable this ask, so Rule 1 was not engaged — this is a clean Rule-2 brake failure, the only
  one in 62 windows. Cost is traceable: at s10/s11 the same turn it correctly declined a third
  Mine, and Fate Unraveler did not land until T10, by which time two Mines had been feeding a
  white-black aggro deck for three turns. Game lost 0–19.
- All other engine casts with zero punishers out were the **first** Mine (permitted by Rule 2) or
  were declined: `152v162` s7/s8, `123v162` s8/s17/s18, `125v162` s11/s12/s14/s15, `146v162`
  s10/s11 all answered **"Cast nothing right now"** with a Mine sitting in the list — 11 windows
  of the brake holding under temptation, which is the behaviour the guide was written to buy.
- Where a punisher WAS out, the brake correctly released: `123v162` s13/s14/s21/s24, `126v162`
  s11/s12/s16/s17/s24/s26, `125v162` s19/s20 all stacked engines as Rule 2 licenses.

### 1d. Rule 1 violated once with a punisher already out — **VIOLATED (new shape)**

`139v162` **s15, T11, life 16/15.** Battlefield: Underworld Dreams (1 punisher), 0 engines.
Mana available **5**. Options included `5. Cast Fate Unraveler {3}{b}` (4 mana). Chose
**`CHOICE: 1 (Cast Howling Mine)`**, `PLAN: Cast Teferi's Puzzle Box, Master of the Feast, Fate
Unraveler ... as mana allows this turn.` At **s16, same turn**, mana was down to 3 and Fate
Unraveler was **no longer in the list**. It never resolved; the game was lost on T12.

This is the wave-39 §3a s13 shape (a punisher displaced by a Mine inside one turn's mana) with the
sign changed: **Rule 2's "stack engines without limit" now licenses the displacement once one
punisher is out, and Rule 1's "ALWAYS" does not visibly win the collision.** The guide never
states which rule outranks the other when the brake is OFF; the ordered cast list at guide L52-62
does put punishers at 1 and engines at 5, but the pilot's reply cites neither. Routed to the
wave-41 ledger as a **guide-internal collision (W41-RULE12)**, not an engine item.

### 1e. Gap table (context, n=6 — reported, not argued from)

Turn of first symmetric-engine ARRIVAL vs first punisher ARRIVAL, from `events` narration
(narration turn headers alternate players, so 4 headers = 2 of the seat's own turns).

| matchup | first engine | first punisher | gap | result |
|---|---|---|---|---|
| 152 | Mine T4 | Underworld Dreams T8 | 4 | LOSS T10 |
| 123 | Mine T3 | Fate Unraveler T7 | 4 | **WIN**, 31 life |
| 126 | Mine T4 | Fate Unraveler T8 | 4 | LOSS T13 |
| 139 | Mine T6 | Underworld Dreams T10 | 4 | LOSS T12 |
| 125 | Mine T4 | Fate Unraveler T8 | 4 | **WIN**, 19 life |
| 146 | Mine T6 | Fate Unraveler T10 | 4 | LOSS T12 |

Baseline gaps: 2, 4, 4, 4, 6, 8. This corpus: **4, 4, 4, 4, 4, 4** — the 6 and 8 tails are gone and
the variance is zero. Honest reading: **the tail is eliminated, the median is unchanged**, and gap
4 is very close to this deck's mana floor (Mine {2} at 2 lands; the cheapest punisher is {3}{B} at
4 lands = 2 of the seat's own turns later), so the guide has moved the seat *to* the curve, not
past it. Two of the six losses (152, 126) are now decided by things other than the gap.

---

## ITEM 2 — WALL BLOCKS AND THE RULE-4 STOPGAP

### 2a. Ledger #5 (W39-WALLBLOCK) is SHIPPED and rendering — **VALIDATED**

`/usr/bin/grep -c 'STOPS all' *.jsonl` over the corpus: **non-empty on 11 of 64 logs, 33
emissions**; 12 on the deck162 seat's own five blocker prompts. Two rendered variants exist:

- single-attacker: `[deals 0 - this block kills nothing, but it STOPS all 3 damage from reaching you]`
- multi-attacker: `[deals 0 - this block kills nothing, but it STOPS all of the damage from
  whichever attacker it blocks - each A-line above says how much]`

The old buried-tail wording (`it only absorbs damage`) has **0 occurrences** corpus-wide.

### 2b. Prediction 3 — `no blockers` with a `(neither dies)` line offered and a non-lethal header

Baseline: **2** (both in `146v162`, 9 life). This corpus: **0**. **VALIDATED.**

All **5** block windows were answered with a block. All five headers read *NOT lethal*, i.e. the
exact shape that produced the baseline declines:

| game | seq | turn | attackers | wall line | choice |
|---|---|---|---|---|---|
| vs152 | s9 | T6 | Wolf 3/3 | Shield Sphere, `STOPS all 3`, `(neither dies)` | `B1:A1` |
| vs139 | s5 | T4 | Pollywog 1/3 | Fog Bank, `STOPS all 1`, `(neither dies)` | `B1:A1` |
| vs139 | s11 | T8 | 2x Pollywog | Fog Bank, `STOPS all 1`, `(neither dies)` both | `B1:A1` |
| vs139 | s14 | T10 | Greathorn 3/4 + 2x Pollywog | Fog Bank, multi-attacker variant | `B1:A2` |
| vs146 | s5 | T4 | Triumphant Adventurer 1/1 (first strike, deathtouch) | Fog Bank, `STOPS all 1`, `(your blocker dies, attacker lives)` | `B1:A1` |

`139v162` s5 is the clean resolution-verification specimen — the reply quotes the **render** and
the **guide** in the same breath: *"STOPS all 1 damage. - Rule 4 says: \"A line ending '(neither
dies)' - take that block. Your wall survives and the damage is gone.\" ... Is there any other
consideration? No."* It also quotes and overrides the core's blockers hint by name. **The two
surfaces are agreeing, which is exactly why they cannot be separated on this corpus.**

### 2c. Stopgap disposition — **DO NOT RETIRE. AMEND THE REMOVAL CONDITION.**

The register's removal condition as written (§10: *delete when `grep -c 'STOPS all' <corpus>/*.jsonl`
is non-empty*) **FIRES NOW**. Executing it would be wrong, for two independent reasons from the
skill's own crutch-lifecycle rungs:

1. **This crutch is BELIEF-CORRECTING, not information-supplying** (skill L2537, the deck109 rung).
   The baseline failure was not a missing fact — the fact was on the line (`it only absorbs damage`)
   and the pilot reasoned past it from a wrong prior ("blocking with a defender doesn't prevent
   damage to me"). Belief-correcting crutches require failure to stay 0 **across at least TWO
   corpora with the belief-triggering shape present**. This is corpus 1.
2. **Retirement pacing** (skill L2582-2584, deck135): never retire a crutch on the single corpus
   that first validates the engine fix it guarded — the crutch is the net if the fix regresses.

So the removal condition itself was under-specified for its crutch class. **Recommended edit, to
`wave39/deck162/findings.md` §10, replacing the removal-condition cell of row 1 verbatim:**

```
Delete only when BOTH hold: (a) `/usr/bin/grep -c 'STOPS all' <corpus>/*.jsonl` is non-empty,
and (b) `no blockers` chosen with a `(neither dies)` line offered under a NOT-lethal header
stays at 0 across TWO corpora that both contain a wall-block window (belief-correcting crutch,
2-corpora bar; corpus 1 = matchups-20260823-110407, PASSED 0/5 windows).
```

**One DEMOTION is due now** (the information-supplying half, which the render duplicates
verbatim and which the multi-attacker variant makes partly stale). In
`bin/Res/ai/baka/deck162_strategy.txt`, **delete L74-75**:

```
   Shield Sphere is tagged "[deals 0 - this block kills nothing, but it STOPS all N damage from
   reaching you]": that STOPS clause is the whole job, and N is the damage the block erases.
```

and replace with the one line that survives both rendered variants:

```
   The wall line's "STOPS all ..." clause is the whole job - it names the damage the block erases.
```

Keep L73, L81-82 (the falsity-fact `The thought "blocking with a defender doesn't prevent damage to
me" is ALWAYS FALSE`) and the Rule-4 override header unchanged until the 2-corpora bar is met.

### 2d. Rule 4's item-3 clause is now teaching against a FALSE render — **ledger, not guide**

`146v162` s5: the pilot blocked a first-strike deathtouch 1/1 with Fog Bank at **life 20** under a
NOT-lethal header, on a line reading `(your blocker dies, attacker lives)`. Guide Rule 4 item 3
says take that block *only* when lethal or life ≤ 8, so the pilot **deviated from the guide** — and
the deviation was **correct play**: Fog Bank has `auto=preventAllCombatDamage to(this)`, so no
damage is dealt and deathtouch (702.2b) never applies. The narration confirms the engine agrees:
Fog Bank survived the block and was lost two events later to **Vanishing Verse**, an unrelated exile.

Same falsity, with a measured cost, at `139v162` s14: Fog Bank's line read
`may block A1 (your blocker dies, attacker lives), A2 (neither dies), A3 (neither dies)` — A1 is
**Migratory Greathorn (3/4, reach, mutate, no deathtouch)**, which cannot damage Fog Bank at all.
The pilot blocked A2 (a 1-damage Pollywog) instead of the 3-damage Greathorn and took **4 instead
of 2**.

Per the trust doctrine the guide must not hedge against a render, so **no guide edit is proposed**;
this goes to the wave-41 ledger as **W41-PREVENT-BLOCKOUTCOME (HIGH)** — the block-outcome
predictor does not consult damage-prevention replacement effects. See item 5.

---

## ITEM 3 — LEDGER #10 AND #13

### #10 (W39-DRAWS0) `{right now: draws 0}` — **VALIDATED, confirmed 0**

`/usr/bin/grep -o 'draws 0' *.jsonl` corpus-wide: **2 hits, both inside model reasoning prose on
the deck125 seat** (about an X=0 Sphinx's Revelation), **zero in any rendered option line**.

On this seat's own cards the clause is now **suppressed entirely**: every `Cast Dictate of
Kruphix {1}{u}{u}`, `Cast Teferi's Puzzle Box {4}`, `Cast Howling Mine {2}` and
`Cast Forced Fruition {4}{u}{u}` option line in all 6 games renders with **no `{right now: ...}`
clause at all** — card text only. Metric as written in the ledger ("zero `draws 0` clauses on a
card whose `auto=` contains a `draw:` on a future step"): **MET.**

Note for the ledger: the fix took the *suppress* branch, not the *name-the-branch* branch
(`{each draw step: draws 1 each}` was not emitted anywhere). That is the safe outcome and no
misplay is attributable to the absence, but the ask's second half is unimplemented — mark #10
**CLOSED-BY-SUPPRESSION** rather than fully answered.

### #13 (W39-PEER) Peer branch labels — **VALIDATED**

Peer into the Abyss was rendered in **3 distinct payable windows** across `125v162`, every one
carrying labelled branches:

```
Cast Peer into the Abyss {4}{b}{b}{b} {right now: if you choose "target opponent": life -12,
draws 23; if you choose "target controller": life -10, draws 21}
```

The flattened `{right now: life -4, life -8, draws 25}` form has **0 occurrences**. Resolution
verified end-to-end: at s22 (T15, labelled `target opponent: life -9, draws 17`) the pilot cast it
with `PLAN: Cast Peer into the Abyss to win the game immediately`; at **s23 the follow-up menu
`['target opponent','target controller']` was answered `target opponent`**, and the game ended
that turn at 19–0. Guide L98-103 and ledger #13 both hold. n is still small (1 cast, 2/2 correct
across both corpora) — carry as a re-check, do not probe.

Bonus, unasked: **ledger #3 (countered-spell narration) has also shipped and is rendering on this
seat** — `125v162` narrates `- Your Liliana's Caress was COUNTERED by Cancel and went to your
graveyard`. The wave-39 E-162a falsity (`resolved and went to your graveyard`) has **0**
occurrences in this seat's logs.

---

## ITEM 4 — FOG BANK DEPLOY AND THE FATE UNRAVELER ORDERING RULE

### 4a. Fog Bank — **VALIDATED** (conditional compliance is perfect)

Baseline: **offered 21, cast 1.** This corpus: **offered 17, cast 4** — but the raw ratio is the
wrong metric, because guide Rule 3 gates Fog Bank on *"if the opponent's battlefield line shows ANY
creature."* Scored against the gate:

| game | offers | opponent creatures present in those windows | cast | verdict |
|---|---|---|---|---|
| vs123 | 4 (s20,s21,s23,s24) | **0 in all four** | 0 | correct declines — opponent is creatureless |
| vs125 | 7 (s4,s6,s8,s11,s12,s14,s15) | **0 in all seven** | 0 | correct declines — opponent is creatureless |
| vs126 | 3 (s4,s5,s7) | yes | 1 (s7) | **taken on the gate** |
| vs139 | 1 (s4, 3 creatures) | yes | 1 (s4) | **taken on the gate, first ask offered** |
| vs146 | 2 (s4, s16) | yes | 2 | **taken both, incl. the life-7 exception window** |
| vs152 | 0 | yes (opponent had creatures) | 0 | **NULL — never offered** (never drawn/payable) |

**Every window in which the gate was live was taken: 4/4.** The 11 declines are all
creature-free boards. Prediction 2's metric ("Fog Bank arrived per game where the opponent controls
a creature"): **3 of 4 eligible games**, and the fourth (vs152) never had the card offered — a draw
null, not a decision failure. **VALIDATED.**

Shield Sphere, same lane, unasked: **offered 8, taken 6.** Both declines
(`152v162` s14, `125v162` s17) took a **punisher** in that ask instead — Rule 1 outranking Rule 3
as written — and the Sphere was taken at the very next ask in both games. Baseline was 14/7.
Rule 3 is fully executed.

### 4b. Fate Unraveler ordering rule — **PARTIALLY VIOLATED (2 of 4 windows)**

4 attacker windows. The lethal exception is intact and the hold-at-home rule fired twice:

| game | seq | turn | offered | opponent life | choice | verdict |
|---|---|---|---|---|---|---|
| vs123 | s15 | T8 | Fate Unraveler only | 18 | **no attackers** | **COMPLIANT** — held home, not lethal |
| vs126 | s13 | T9 | Fate Unraveler only | 18 | **no attackers** | **COMPLIANT** — held home, not lethal |
| vs126 | s18 | T11 | Fate Unraveler + Master of the Feast (8 power) | 20 | `ATTACK: A1, A2` | **VIOLATED** — not lethal |
| vs126 | s27 | T13 | Fate Unraveler + 2x Master of the Feast (13 power) | 20 | `ATTACK: A1, A2, A3` | **VIOLATED** — not lethal |

Baseline was 4/4 attacks with the rule absent; the guide converted the two solo windows and lost
both mixed windows. The reasoning shows why: at s18/s27 the reply is the bare
`ATTACK: A1, A2` + a generic pressure plan — **the guide's lethal check is never run in the
reply**, and Fate Unraveler is simply swept up with the fliers. The cost repeated the exact
wave-39 N-162e outcome: both attacks were absorbed —
`- Fate Unraveler dealt 2 damage to Perimeter Captain` and
`- Fate Unraveler dealt 1 damage to Vampire`, **zero damage through**, punisher tapped out of
blocking. (vs126 was lost 0–40 to a Sanguine Bond / Exquisite Blood lifegain deck, so the
attacks were not the deciding factor — but they were free value declined twice.)

Diagnosis for the wave-41 guide lane: the rule is written at guide L129-131 as prose inside the
ATTACKING section and requires the pilot to run the multi-line LETHAL CHECK (L135-146) before every
declaration. The two windows where it failed are exactly the ones where a **second, unrestricted**
attacker was in the list — a list-shape trigger, not a comprehension gap. Candidate revision is a
countable one-liner at the declaration itself ("if your attacker list has any name other than Fate
Unraveler, declare those and leave Fate Unraveler out unless the printed opponent life is at or
below your total power") rather than more lethal-check prose. **Proposal only; not an edit made
here.**

### 4c. Lethal exception intact — **VALIDATED**

The exception did not need to fire this corpus (no window where Fate Unraveler's power completed
lethal), so it is **NULL-but-uncontradicted**: the two compliant windows show the hold, and the two
violations are over-attacking, never under-attacking. The wave-39 concern (a blanket "never attack"
would have cost the vs146 win) is not realized — the pilot's failure mode is the opposite sign.

---

## ITEM 5 — NEW ITEMS FOR THE WAVE-41 LEDGER

### W41-PREVENT-BLOCKOUTCOME — [HIGH, TRUST DOCTRINE / ANNOTATION TRUTH]

The block-outcome parenthetical does not consult **damage-prevention replacement effects**, so a
`preventAllCombatDamage to(this)` creature is predicted to die.

- **Mechanism**: Fog Bank (`mtg.txt`: `auto=preventAllCombatDamage to(this)` +
  `from(this)`) is rendered `(your blocker dies, attacker lives)` against attackers that cannot
  damage it. The predictor appears to compare attacker power to blocker toughness (and to apply
  deathtouch) without checking prevention.
- **Repro 1, engine self-refutes**: `146v162` s5 — `B1. Fog Bank (0/2) ... may block A1 (your
  blocker dies, attacker lives)`, A1 = Triumphant Adventurer 1/1 first strike + deathtouch. The
  pilot blocked anyway; the narration shows **Fog Bank survived** and was only lost later to
  Vanishing Verse. The engine and its own annotation disagree in the same game.
- **Repro 2, cost measured**: `139v162` s14 — A1 Migratory Greathorn (3/4, reach, mutate, **no
  deathtouch**) rendered `(your blocker dies, attacker lives)`; A2/A3 Pollywog Symbiote (1/3)
  rendered `(neither dies)`. The pilot blocked A2 and **took 4 instead of 2**.
- **Why it is HIGH**: this is the same class as W39-WALLBLOCK on the same line, one wave later —
  and this seat's entire defence is two prevention walls. It also inverts guide Rule 4's item-3
  teach, which is correct against a truthful render and harmful against this one.
- **Ask**: the outcome predictor must apply prevention/protection replacement effects before
  computing blocker death; Fog Bank vs any non-prevention-piercing attacker must read
  `(neither dies)`.
- **Validated next by**: seat 162, any Fog Bank / Guard Gomazoa / prevention-wall seat.
  Metric: zero `(your blocker dies` on a blocker whose `auto=` contains
  `preventAllCombatDamage to(this)`.

### W41-RULE12 — [GUIDE LANE, seat 162] Rule 1 and Rule 2 collide once the brake is OFF

The guide states Rule 1 as unconditional ("ALWAYS", "at every life total, on every board") and
Rule 2 as "cast every draw engine you are offered, every turn, as fast as you can" once a punisher
is out. `139v162` s15 is the collision: with 5 mana, one punisher out and Fate Unraveler payable,
the pilot took a {2} Mine and the {3}{B} punisher fell out of the list the same turn. Rule 1's
"ALWAYS" needs a mana-order clause that survives the brake being off. **Guide-lane, seat-scoped;
no engine change.**

### W41-ATTACKLIST — [GUIDE LANE, seat 162] the Fate Unraveler hold is list-shape sensitive

Held in both solo-attacker windows, lost in both mixed-attacker windows (item 4b), with the lethal
check never appearing in the reply. Candidate is a declaration-time one-liner, not more lethal-check
prose. Cross-check against other seats before generalizing — this may be a pool-wide pattern where
a per-creature restriction dissolves when the attacker list has more than one name.

### W41-MULL162 — [WATCH, 2 corpora] the mulligan seam remains UNEXERCISED at this seat

6/6 hands were `counted by the engine: 3 lands, 4 spells` and all were kept; **zero mulligans in
12 games across two corpora**. The guide's mulligan section (L148-160) has never been tested. Per
the skill's surface-unexercised rung, do **not** read this as "confirmed clean" — carry it forward
and do not spend guide budget growing it.

### Carried forward unchanged (no new evidence)

- **Forced Fruition** — offered in `125v162` (s11/s12/s14/s15/s22) and `126v162` (s24), **cast 0,
  resolved 0**. Now **UNEXERCISED across two corpora**. Every decline was guide-compliant (the
  brake was on, or a punisher / Peer outranked it). Guide L95-97 states the rule and no rate; leave
  it. A probe deck remains the only reliable way to exercise it.
- **Teferi's Puzzle Box** — offered several times, **cast 0** this corpus (1 cast in the baseline).
  No new evidence either way.
- **Ob Nixilis -2 targeting** — the seam did not arm this corpus (Ob Nixilis arrived once, vs123
  T15, one turn before the win). Guide L110-112 untested this wave; carry.

---

## SUMMARY TABLE

| # | item | verdict |
|---|---|---|
| 1a | punisher offered + engine taken, zero punishers out (pred. 1) | **VALIDATED** 3 → 0 (7/7 windows took the punisher) |
| 1b | Rule-1 named exception (life ≤ 8 + opponent creature) | **VALIDATED** — fired once, exactly as written |
| 1c | countable brake: one Mine max while zero punishers out | **VIOLATED 1×** — `146v162` s9 (11 correct holds) |
| 1d | Rule 1 with the brake OFF | **VIOLATED 1×** — `139v162` s15, punisher priced out of the turn |
| 1e | engine→punisher gap | tail eliminated (2-8 → flat 4); median unchanged |
| 2a | ledger #5 `STOPS all N` rendering | **VALIDATED** — 33 emissions, old wording 0 |
| 2b | `no blockers` with `(neither dies)` offered (pred. 3) | **VALIDATED** 2 → 0 (5/5 windows blocked) |
| 2c | Rule-4 stopgap disposition | **DO NOT RETIRE** (belief-correcting, corpus 1 of 2) — amend the removal condition; **demote L74-75** |
| 2d | Rule 4 item 3 vs the prevention render | guide correct-as-written, **render false** → W41-PREVENT-BLOCKOUTCOME |
| 3 | ledger #10 `draws 0` | **VALIDATED** — 0 rendered occurrences (suppress branch taken) |
| 3 | ledger #13 Peer branch labels | **VALIDATED** — labelled 3/3, menu answered `target opponent`, won the game |
| 3 | (bonus) ledger #3 countered narration | **VALIDATED** — `was COUNTERED by` rendering, old falsity 0 |
| 4a | Fog Bank deploy floor (pred. 2) | **VALIDATED** — 4/4 gate-live windows taken; 11 declines all creature-free; 1 draw NULL |
| 4a | Shield Sphere floor | **VALIDATED** — 8 offered / 6 taken, both declines took a punisher instead |
| 4b | Fate Unraveler ordering rule | **PARTIALLY VIOLATED** — 2/4 (both mixed-attacker windows) |
| 4c | lethal exception | **NULL, uncontradicted** — failure mode is over-attacking, not under |
| pred. 4 | regression watch: Howling Mine arrivals/game ≥ 2.0 | see note below |

**Prediction 4 (regression watch)** — "a punisher-first rule can become 'never cast Howling Mine'".
Mine arrivals per game this corpus: vs152 1, vs123 3, vs126 2, vs139 2, vs125 3, vs146 2 =
**2.17/game**, above the 2.0 floor. **NO REGRESSION.** The brake suppressed second Mines only while
punishers were absent and released cleanly afterward (`125v162` cast two Mines back-to-back at s19/s20
the turn after Underworld Dreams landed). The wave-31 pitfall did not fire.
