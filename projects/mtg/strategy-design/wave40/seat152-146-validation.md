# Wave-40 — deck152 / deck146 hold-seat validation

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-110407` (21 matchups, pool 146/139/152/
125/126/162/123). Deployed guides read from `projects/mtg/bin/Res/ai/baka/deck152_strategy.txt`
and `deck146_strategy.txt` — **both wave-39 §4 edits are present in the deployed 152 file**
(RETURN-SWING ordered arming test at L36-52; THE CHUMP TEST at L93-101), so the predictions below
are testable against the shipped text.

Baseline comparisons are read directly out of the wave-39 corpus
`matchups-20260823-040744`, not from the wave-39 write-up, wherever a before/after number is
claimed.

## 0. Seat volumes (both seats, this corpus)

| seat | games | W-L | ask | priority | attackers | blockers | fallbacks |
|---|---|---|---|---|---|---|---|
| **152** | 6 | **5-1** (loss: 139) | 231 | 83 | **34** | **3** | **0** |
| **146** | 6 | **4-2** (losses: 139, 152) | 177 | 62 | **40** | **9** | 1 (`unparsed_reply`, 0.36%) |

All 12 games carry paired `gameend` records; no crash, no core dump in either seat's `.stderr`.

**Structural note that scopes everything in §1-§2: seat 152 got THREE blocker windows in six
games.** It is an attack-side seat in this pool; the block-lane predictions are being adjudicated
on n=3. Say so out loud rather than reporting a percentage.

---

## 1. deck152 Edit 1 — RETURN-SWING arming test — prediction (a)

Prediction (a), verbatim: *sole-attacker violations at armed windows **1 → 0**, and zero replies
containing "free damage" at an armed window.*

**Arming census, all 34 attackers windows.** Arming read exactly as the guide states it: YOUR LIFE
at or below the POWER of their biggest creature (counters +1).

| games | attackers windows | ARMED | armed **and** sole-attacker |
|---|---|---|---|
| 6 | 34 | **1** | **0** |

The single armed window is `152v139 seq31` (T13, life **1**, opp 10, Illuna 6/6 arming the gate)
and it offered **FOUR** attackers (Wolf 6/6, Briarbridge Tracker 5/4, Luminarch Aspirant 1/1,
Katilda 1/1).

### VERDICT (a): **NULL on its precondition — not a confirmed 1 → 0.**

The class the edit was written against (armed **+** exactly one offered attacker) **did not occur
once in six games**. The corpus cannot distinguish "the edit fixed it" from "the situation never
came up". The honest report is a null, and the sub-clause that IS measurable is weak evidence:

- **"free damage" appears 0 times in all 357 deck152 records** (all kinds, armed or not). True,
  but with one armed window in the sample this is nearly uninformative on its own — the phrase was
  also absent from 22 of 23 wave-39 windows.

### What the one armed window actually shows (NEW, not predicted)

At `seq31` the guide's **≥2-attacker branch** applies: *"When the check arms and the list offers
TWO OR MORE attackers, keep your biggest ground creature HOME as a blocker and attack with the
rest."* The reply was `ATTACK: A1, A2, A3, A4` — everything, nothing home. **Branch VIOLATED.**

The lethal EXCEPTION does not rescue it: total attacking power 13 against opponent life 10, but
two untapped Arboreal Grazers (0/3) were available to block, so the swing was not unblockable
lethal.

**Counterfactual, stated because it changes the item's priority and not its verdict:** holding the
Wolf 6/6 home would not have saved the seat. The return swing was Illuna 6/6 trample **plus
Everquill Phoenix 4/4 flying** into life 1, and the seat had zero fliers and zero reach. The seat
died at `my_life -2` on T14 either way. So this is a **guide-conformance** violation with **no
measurable outcome cost** — record it, do not promote it.

### The paralysis-side cost of Edit 1 — MEASURED, on the attack lane

Prediction (c) was written only for the block lane, but the wave-31 pitfall it names showed up on
the lane Edit 1 actually touched:

| corpus | attackers windows | `ATTACK: none` answers | armed at the time? |
|---|---|---|---|
| wave-39 (pre-edit) | 23 | **0** | — |
| wave-40 (post-edit) | 34 | **2** | **both UNARMED** |

Both declines are `152v139`, both sole-attacker windows:

- **`seq9`** (T5, life 20, biggest opposing power 1): Wolf 2/2 sole attacker into untapped
  Arboreal Grazer 0/3 ×2 + Pollywog 1/3. `ATTACK: none`. **CORRECT** — rule #2 names this exact
  case (*"a 1/1 walking into an untapped 0/3 wall"*).
- **`seq19`** (T9, life **16**, biggest opposing power **3** — nowhere near armed): Wolf 4/4 sole
  attacker; opponent's only untapped real blocker is Migratory Greathorn 3/4, which the Wolf
  **kills while surviving**. Reply: `ATTACK: none / PLAN: Preserve the 4/4 Wolf for blocking next
  turn.` **A MISS** — rule #2's "even-or-better trade" clause says attack, and the return-swing
  gate is not armed at 16-vs-3.

`seq19` is the shape the edit invites: step (2) ("COUNT THE ATTACKERS… exactly ONE attacker →
ATTACK: none") now sits inside the arming block where the model does its counting, and step (2)
does not restate its own dependence on step (1). n=1, but the before/after (0/23 → 1/34 unforced
declines) is the right direction to watch. Ledger item W41-1.

---

## 2. deck152 Edit 2 — THE CHUMP TEST — predictions (b) and (c)

All three blocker windows, printed forecast and answer:

| window | forecast "you would be at" | options offered | answer | read |
|---|---|---|---|---|
| `152v139 seq15` (T8, life 20) | **16** | B1 Katilda 1/1 — *(your blocker dies, attacker lives)* on both attackers | `BLOCKS: none` | **chump test FOLLOWED** — this is the wave-39 violation's own shape (a 1/1 lord-body chumping a Pollywog-class attacker at high life), now answered correctly |
| `152v139 seq20` (T10, life 16) | **11** | B1 Wolf 4/4 / B2 Tracker 4/3 — ***(you kill it, your blocker lives)*** on A1; B3 Aspirant 1/1 — *(blocker dies)* | `BLOCKS: none` | **UNDER-BLOCK — see below** |
| `152v146 seq66` (T18, life 17) | **14** | B1 Elite Spellbinder 3/1 — *(both die)* on Hive of the Eye Tyrant 3/3 | `BLOCKS: B1:A1` | permitted — *(both die)* is not the forbidden parenthesis; trading a 3/1 for a creature-land at life 17 is defensible |

**Prediction (b): blocker-dies assignments at a printed forecast of 10 or more — 1 → 0.
CONFIRMED.** Two of three windows offered a *(your blocker dies, attacker lives)* line at N ≥ 10
(seq15 twice, seq20 once) and **none was taken**; wave-39's single violation (`152v139 s11`,
Luminarch Aspirant chumping a Pollywog at life 20) has no analogue in this corpus. Weight it as
n=2 opportunities declined, not as a rate.

**Prediction (c), first half: trade-drift stays 0/N. CONFIRMED, 0/3.** No reply quotes a
parenthesis and then overrides it; all three replies are terse and none contains "but actually",
"still stops it", or a re-derived combat result.

**Prediction (c), second half: declared blocks at forecast ≤ 9 do not fall. UNMEASURABLE.**
**Zero** deck152 blocker windows in this corpus had a forecast of 9 or less (16, 11, 14). The
brake's low-N side was never exercised. Report as a null with the reason, not as a pass.

### The paralysis signal that DID land — at N = 11, not N ≤ 9

`152v139 seq20` is the item. Forecast 11 (≥ 10, so the new clause is live). The offered block was
**B1. Wolf (4/4) — may block A1 (you kill it, your blocker lives)**: a free kill of Pollywog
Symbiote 1/3, blocker survives, and the seat's other attacker (Everquill Phoenix, flying) was
explicitly unblockable, so the Wolf had no competing job. The answer was `BLOCKS: none`, with no
PLAN line and therefore no recorded reasoning.

Edit 2's clause is scoped to the blocker-dies parenthesis in its own text, but its salient shape
in the guide is **"N is 10 or more → don't block"**, and the sentence immediately preceding it in
the deployed file is *"Answer 'BLOCKS: none' and take the damage unless…"*. A pilot compressing
that section to a threshold rule declines the free block too. This is the wave-31 pitfall arriving
one window above the range prediction (c) was watching. n=1. Ledger item W41-2, with the proposed
remedy: bind the N ≥ 10 clause explicitly to the parenthesis it is about **and** add the positive
form the section currently lacks — a block whose parenthesis reads *"(you kill it, your blocker
lives)"* costs nothing and is always taken.

---

## 3. deck146 hold-back gate — the wave-39 named arming requirement

Wave-39's condition, verbatim: *"the wave-40 review of seat 146 must report the arming count from
the 146v139 and 146v152 games specifically, and if the gate is still 0/2-armed after that, it is
retired rather than carried a third time."*

Gate as the guide states it: sum the power of **every** opposing creature (tapped included) = THEIR
TOTAL POWER; if that is your life total or more → `ATTACK: none`, Nadaar excepted (vigilance).

**Maximum opposing total power faced, per game:** 139 → **10**, 152 → **6**, 123 → 5, 126 → 1,
125 → **0**, 162 → **0**. The two named games are exactly the two that supplied beefy boards, as
wave-39 predicted they would.

### Arming census — 40 attackers windows, **3 ARMED, all three in the two named games**

| window | life | THEIR TOTAL POWER | offers | answer | verdict |
|---|---|---|---|---|---|
| `146v139 seq47` (T18) | 4 | **10** (Everquill 4 + Nethroi 5 + Pollywog 1 + Grazer 0) | 4 | `ATTACK: none` — *"Hold all bodies to block next turn's 10-power swing. Nadaar is summoning sick and cannot attack."* | **FOLLOWED**, and the reply names the gate's own quantity |
| `146v152 seq30` (T16) | 4 | **6** (Sigarda 6/6) | 1 (Silencer 3/2) | `ATTACK: none` — *"Hold back attackers to preserve Silencer as a blocker for next turn's Sigarda swing."* | **FOLLOWED** |
| `146v152 seq34` (T18) | 2 | **3** (Elite Spellbinder 3/1 flying) | 2 (Hive 3/3, Silencer 3/2) | `ATTACK: A1` — *"Hold Silverquill Silencer to block Elite Spellbinder next turn and survive."* | **PARTIAL** — attacked with 1 of 2; the held body is real, its stated purpose is not (see below) |

### VERDICT: the named requirement is **DISCHARGED**, and the standing retire recommendation is
### **REFUTED — KEEP the line.**

The gate armed **3 times in one corpus** after two corpora at one arming total, and it **bound the
decision in all three**: two clean `ATTACK: none` at life 4 with the reply reasoning from the
gate's own arithmetic, one partial hold. Both wave-39 conditions for retirement fail — it is not
0/2-armed, and it is not inert when armed.

Two honest qualifiers:

1. **Neither armed game was won.** 146 lost both 139 and 152. The gate is a survival brake, not a
   win condition; arming at life 4 and life 2 means it fired at the point where it can no longer
   change the result. That is an argument about WHERE it arms, not about retiring it.
2. **`seq34` is a partial with a false premise.** The seat kept Silverquill Silencer (3/2, ground)
   home explicitly *"to block Elite Spellbinder next turn"* — Spellbinder **flies**, and 146's
   render will list it as unblockable for a ground body when the blockers window comes. The hold
   was correct by the gate and worthless in fact. That is a perception item, not a gate item:
   the ATTACKERS window carries no evasion-vs-my-blockers statement, so the seat has to predict a
   block-legality the render will later hand it for free. Ledger item W41-5.

---

## 4. deck146 validated teaches — regression spot-check

### 4a. Chump anchor — **REGRESSION** (wave-39: 2/2 fired-and-followed → wave-40: 3/5)

Teach under watch (deployed L132, restated L231): *"When the ask's forecast says 'you would be at'
10 or less, block the biggest non-trample attacker with your cheapest creature."*

Five windows at forecast ≤ 10:

| window | forecast | blockable attackers | answer | verdict |
|---|---|---|---|---|
| `146v139 seq35` (life 12) | **7** | Everquill Phoenix **4/4 flying** — blockable, both Spiders have **reach** | `BLOCKS: none` — *"Let attackers through."* | **VIOLATED** — the biggest non-trampler was blockable by the cheapest creature and was not blocked; took 5 |
| `146v139 seq42` (life 7) | **2** | only Pollywog 1/3 (Phoenix unblockable) | `BLOCKS: B1:A2` | followed |
| `146v139 seq49` (life 4) | **−5, LETHAL** | Phoenix (reach-blockable), Pollywog, Gemrazer | `B4:A2, B1:A3, B2:A3` | followed (blocked to survive the non-fliers) |
| `146v152 seq32` (life 4) | **2** | Lair of the Hydra 2/2 — *(both die)* with Silencer | `BLOCKS: none` — *"Let Hydra through to preserve Silencer for future blocks."* | **VIOLATED** — an EVEN trade declined at life 4; the "future blocks" never came, the seat died two turns later |
| `146v152 seq35` (life 2) | **−4, LETHAL** | Lair 3/3 (Spellbinder flying, unblockable) | `BLOCKS: B1:A1` | followed |

**3 followed / 2 violated.** Both violations share one authored sentence in the reply —
*"preserve … for future blocks"* / *"let attackers through"* — i.e. the anchor is losing to a
creature-preservation instinct at exactly the life totals where preserving a creature is worthless.
The `"I take the damage either way"` string the wave-38 edit banned is still at **0 occurrences**,
so the banned WORDING held while the BEHAVIOUR it was banned for returned in different words. That
is the item, and it is the strongest single reason 146 cannot exit rotation. Ledger W41-3.

### 4b. MAIN-1 pumps — **BINDING, no regression**

Silverquill Command cast **4 times, 4 in MAIN PHASE 1, 0 in MAIN PHASE 2** (`146v125 seq17`,
`146v125 seq28`, `146v126 seq28`, `146v123 seq27`; phase read off each record's
`--- CURRENT SITUATION ---` header, not the narration). It was offered and passed over in 12 other
windows, all MAIN 1 — no held-into-MAIN-2 instance exists to regress.

Companion teach *"pick YOUR OWN creature"*: 1 observable target ask, `146v126 seq29` →
**Triumphant Adventurer [your battlefield]** — own creature, correct. `146v125 seq28` chose the
pump mode then attacked with the pumped Triumphant Adventurer at `seq30` (single legal target,
engine auto-resolved, no target ask emitted). The other two casts took non-pump mode pairs, which
the guide permits.

### 4c. Wave-39 ledger #6 (`all_assignments_illegal`, 146v36 s22)

**0 recurrences.** Nine blocker windows, every answer parsed and legal. Note the confound: deck36
rotated out, so the class's original context is absent from the pool — this is a clean corpus, not
a discharged defect.

### 4d. NEW, opposite sign: over-blocking at a HIGH forecast

`146v139 seq23` (T11, life 20, forecast **17**): both Spiders (2/1) gang-blocked Migratory
Greathorn 3/4, each line reading *(your blocker dies, attacker lives)*. Two creatures died, three
damage prevented, at life 20. The 146 guide **sanctions** this — L124-131 teaches *"'My blocker
dies anyway' is never a reason to take the damage - a 2/1 for 12 life is the best trade this deck
is ever offered"* with **no upper bound**, which is the precise calibration deck152 just had an
upper bound installed for. The two seats now teach opposite things about the same parenthesis.
Ledger W41-4: consider porting 152's chump test to 146 as the symmetric high-N bound, keeping
146's low-N anchor intact.

---

## 5. deck152 L201 — the pre-existing frequency claim

Deployed line 201-202, verbatim:

```
- vs REMOVAL/GRIND: keep deploying anyway - an empty board loses these far more often than
  walking into removal does. Keep attacking so the clock forces trades.
```

**Does the claim's subject occur? YES — twice, and they are the seat's two grindiest games.**
Permanent-loss events suffered by seat 152, per game (read off the final narration): 125 → **9**
(Path/exile-heavy UW control), 146 → **8** (Vanishing Verse, Soul Shatter, Kaya, Pelakka
Predation), 139 → 2, 126 → 1, 162 → 0, 123 → 0. So unlike the wave-31 werewolf case, this is **not
a manufactured-rarity claim about an unexercised mechanic** — the matchup class is real and was
sampled.

**And the taught behaviour is corroborated by outcome:** the seat kept deploying into both and
**won both** — `152v125` at 34 life on T30, `152v146` at 14 life on T19.

**The defect is the ADJECTIVE, exactly as wave-32 HL4 rewrote HL6:** *"A guide may state a
mechanic's TRIGGER, its OUTCOMES, and the STRING on the board that reports its current state. It
may never state how OFTEN … not 'rarely', not 'often' … regardless of how many times the corpus
sampled it."* HL6 reads as an evidence rule, so a reviewer holding two supporting games reaches for
the supporting adjective — that is the documented way to break it while fixing it. `"far more
often than"` is a licence to stop reading the board: it invites the pilot to compare two
frequencies it cannot observe instead of executing the deploy rule. The mandatory pre-ship grep
(`rarely|often|usually|regularly|typically|expect|…`) hits this line.

The same grep's only other hit in the 152 guide, L213 (*"taking damage while ahead is often
correct"*), is a **verbatim quotation of the engine's own render string** being overridden. Not a
defect — leave it byte-identical.

### RECOMMENDATION: **REWRITE.** Exact line (replaces L201-202, nothing else changes):

```
- vs REMOVAL/GRIND: keep deploying anyway. Their removal answers one creature at a time and your
  board is what wins the game; a creature you held in hand blocks nothing and deals nothing. Cast
  the best creature offered on every one of your main phases, and keep attacking so the clock
  forces trades.
```

This keeps the teach and its two corroborating games, drops the unobservable frequency comparison,
and states the rule as an action keyed to a surface the pilot already reads (the cast list on its
own main phase). It is also consistent with rule #3's existing *"Do not save creatures from removal
by holding them"* rather than competing with it.

---

## 6. New items for the wave-41 ledger

| # | item | seat | evidence | routing |
|---|---|---|---|---|
| **W41-1** | Edit 1 side-effect: unforced `ATTACK: none` at an **unarmed** sole-attacker window. Step (2) of the arming test does not restate its dependence on step (1), and it now sits where the model does its counting. Before/after: 0 declines / 23 windows → 2 / 34, both unarmed. | 152 | `152v139 seq19` (life 16, biggest opposing power 3, Wolf 4/4 declined against a Migratory Greathorn 3/4 it kills while surviving) | GUIDE — re-anchor step (2): *"only when (1) armed"*, and name the unarmed default in the same breath |
| **W41-2** | Edit 2 over-reach: a **trade-up** block declined at N = 11. The section has no positive form — nothing tells the pilot that *(you kill it, your blocker lives)* is always taken. | 152 | `152v139 seq20` (Wolf 4/4 free-kills Pollywog 1/3, other attacker unblockable, answered `BLOCKS: none`) | GUIDE — bind the N ≥ 10 clause to its parenthesis and add the positive clause |
| **W41-3** | **Chump-anchor regression, 2 violations / 5 windows** (wave-39: 0/2). Banned wording (`"I take the damage either way"`) still 0; the behaviour returned as *"preserve … for future blocks"* / *"let attackers through"*. | 146 | `146v139 seq35` (N=7, reach Spiders vs a blockable 4/4 flier, none), `146v152 seq32` (N=2, an even *(both die)* declined at life 4) | GUIDE — the anchor needs the preservation counter-argument named in the pilot's own words, the way the trade-drift rule names its |
| **W41-4** | 146 and 152 now teach **opposite** rules about the same *(your blocker dies, attacker lives)* parenthesis: 152 has a high-N chump bound, 146 has an unbounded "never take the damage". Produced a double-chump costing two Spiders at forecast 17. | 146 | `146v139 seq23` | GUIDE — consider porting 152's chump test as 146's high-N bound; keep 146's low-N anchor |
| **W41-5** | Attackers windows carry **no evasion-vs-your-blockers statement**, so a hold-back decision has to predict a block legality the blockers window later states outright. Produced a hold whose stated purpose was impossible. | 146 | `146v152 seq34` (Silencer 3/2 held *"to block Elite Spellbinder next turn"*; Spellbinder flies) | RENDER (perception) — a hold-back seat is deciding on next turn's blocks with this turn's attacker list |
| **W41-6** | 152's L201 frequency claim — rewrite in §5 above, ready to apply. | 152 | deployed L201-202 | GUIDE |
| **W41-7** | 146 guide carries two un-flagged HL6 frequency strings: L62 *"an artifact deck's board is **usually** untouchable"* (softened by the offer-as-evidence rail two lines below it) and L168 *"you will **rarely** finish it"* (Dungeon of the Mad Mage — a bare mechanic-frequency claim). | 146 | pre-ship grep on the deployed file | GUIDE — low priority; L168 is the real one |
| **W41-8** | Cross-kind protocol bleed: a **priority** (`CHOICE:`) window answered with `ATTACK: Barrowin, Spirit Cai, Goblin, Spider #1, Spider #2` — the model pre-answered the attackers question one decision early. Latency 180 s (the seat's slowest record). | 146 | `146v126 seq49`, `unparsed_reply` | PARSER/PROMPT — 1/177 for this seat; log it, do not act on n=1 |

---

## 7. Rotation adjudication input

Rule: *both* conditions must hold to exit — guide unmodified this wave **and** no seat signal for
additional work.

| seat | guide modified this wave? | seat signal for additional work? | eligibility |
|---|---|---|---|
| **152** | **YES** — this wave's two edits are the modification under test, and neither prediction resolved cleanly ((a) NULL on its precondition, (b) confirmed on n=2, (c) half-unmeasurable) | **YES** — W41-1, W41-2, W41-6 | **HOLD.** Its own edits are un-adjudicated: the armed-sole-attacker class did not occur once in six games, and the corpus produced no forecast ≤ 9 block window. A third corpus is required before either edit can be called validated. |
| **146** | No | **YES** — W41-3 (a regression of a previously VALIDATED teach), W41-4, W41-5, W41-7, W41-8 | **HOLD.** The hold-back gate's own blocker is cleared — the named requirement is discharged and the line is KEPT, not retired — but the chump anchor moved backwards, which is exactly the "signal for additional work" the exit rule is about. |

**Named condition to attach to 152's hold, so wave-41 does not produce another null:** the two
predictions need windows that this pool structurally starves. 152 saw **one** armed attackers
window and **three** blocker windows in six games, and zero blocker windows below forecast 10. The
wave-41 review of seat 152 must report (i) the armed-**sole**-attacker count and (ii) the count of
blocker windows at forecast ≤ 9; if either is still 0 after that corpus, the corresponding edit is
un-adjudicable by corpus and should be discharged by a **probe deck** (a stacked opponent with one
large body and a seat board of one creature) rather than carried a third wave on a null.
