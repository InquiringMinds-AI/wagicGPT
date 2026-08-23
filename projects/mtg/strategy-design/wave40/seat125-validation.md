# deck125 "Revelation Fracture" — wave-40 VALIDATION seat

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-110407`, binary `465422fcf`, pilot
`qwen36-35b-a3b`. **First validation corpus for `deck125_strategy.txt`** (the wave-39 Step-0 guide,
deployed at `bin/Res/ai/baka/deck125_strategy.txt`, 155 lines). Six games, **all natural ends** —
vs139 (t26), vs146 (t20), vs152 (t30), vs126 (t48), vs162 (t15), vs123 (t35). **267 model decisions,
0 defers, 1 fallback.** Baselines quoted below are from `wave39/deck125/findings.md` (169 decisions,
guideless).

Per the ceilinged-seat rule the win column is context, never a verdict; every claim below is
resolution-verified against downstream game events, and unexercised rungs are recorded as nulls.

---

## 1. GUIDE PREDICTIONS

### 1.1 The Emrakul-plan belief — **VALIDATED, large effect**

| measure | baseline (guideless) | this corpus | |
|---|---|---|---|
| decisions whose reasoning/reply names Emrakul | **144 / 169 (85%)** | **45 / 267 (16.9%)** | ✅ |
| PLAN lines naming Staff of Nin as the win condition | 0 | **92 / 99 (93%)** | ✅ |
| PLAN lines verbatim-quoting the guide's line | 0 | **50 / 99** | ✅ |

The guide's word-for-word plan (`Answer their threats, resolve Staff of Nin, ping their face every
turn.`) is the single most common PLAN string in the seat, 50 exact repetitions.

Every remaining Emrakul mention was hand-read. **Not one is the old false belief.** They split into
(a) the model QUOTING the guide's own prohibition back at itself while reasoning — deck146 s3,
deck152 s1, deck126 s1/s13/s24/s57/s62, deck162 s12, deck139 s18 — and (b) deck123 s49–s71, where
Emrakul was **genuinely on the battlefield**. The phrase "ramp to 15 mana" appears only inside those
quotations. The wave-39 exemplar shape (`vs146 s36 PLAN: Ramp to 15 mana to cast Emrakul`) has **0
occurrences**.

**Escape-clause validation (guide L19-21, "if Cast Emrakul is ever actually printed, take it"):**
Emrakul was offered in **2** windows across the corpus and **taken 2/2**, and both **ARRIVED** —
resolution-verified from the narration, not the pick:
- deck123 **s49** (t28, 15+ mana): `- You cast Emrakul, the Aeons Torn` / `- Your Emrakul, the Aeons
  Torn resolved and entered the battlefield`, followed by the extra-turn trigger firing (narration
  shows `Turn 29 - YOUR turn` then `Turn 30 - YOUR turn`). Emrakul then died to a Devour Flesh
  sacrifice edict on t30 before any combat.
- deck126 **s66** (t48, 16 mana): same two lines, arrival confirmed in the final prompt's narration.

**Residual (small, one game):** deck126 s66/s67 reasoning shows the pilot unsure what to do with a
resolved Emrakul — *"we don't attack with Emrakul according to the plan"* … *"Emrakul can't
attack."* The guide's flat statements *"You never have a creature on the battlefield"* and *"you
have no creature to attack with"* become FALSE the moment the escape clause fires, and the pilot
noticed the contradiction. → guide item for the reviser, not an engine item.

### 1.2 The Staff-of-Nin plan adopted? — **VALIDATED, at the seam as well as in the plan text**

Options counted from the numbered list only (not prompt mentions):

| seam | offered | taken |
|---|---|---|
| `Deal 1 Damage with Staff of Nin … targeting a player` | 25 | **25** |
| `Cast Staff of Nin` | 6 | **6** (every one on the first window it appeared) |
| `Life with Elixir of Immortality` activation | 8 | **8** |
| land drop windows with a land in hand | 71 | 68 |

Baseline was 11/11 pings, 5/7 Staff casts. The clock is now taken every single window it exists, and
it is visibly *working*: vs123 the opponent went 20 → 7 purely on Staff pings.

The 3 declined land drops are all deck123 t12/t31 (`Play no land right now`) at 16+ lands in play —
no rule covers it, no mana was needed, harmless. Recorded as INFO, not a violation.

### 1.3 Counter discipline — **VALIDATED (large improvement), 2 residual misspends**

Baseline **9 of 25 counters (36%) spent on mana producers / walls / 0-power bodies.**
This corpus: **2 of 21 (9.5%).** All 21 enumerated with the stack line they answered.

Correct (19): Beanstalk Giant 7/7 ×2 (139 s16/s17), Brokkos 6/6 (139 s20), Nissa PW (139 s22),
Barrowin 3/3 (146 s10), Acererak 5/5 (146 s17), Tovolar's Huntmaster 6/6 (152 s13), Sigarda 4/4
(152 s17, 152 s52), Teferi PW (152 s21), Elite Spellbinder 3/1 (152 s31), Staff of Nin (126 s48 —
on the guide's own counter-on-sight list), Sorin PW ×2 (126 s49/s57), Liliana's Caress (162 s7 — a
per-discard drain engine vs the discard deck), Bloodline Keeper 3/3 (123 s10), Idyllic Tutor ×3
(123 s28/s29/s34 — named in the guide).

**VIOLATED (2):**
- **deck139 s25** (t26): `Cast Cancel` on `Arboreal Grazer {g} (creature 0/3)` — Arboreal Grazer is
  named on the guide's own LET-IT-RESOLVE list (L48-49), and the stack line printed `(creature 0/3)`.
  Adherence failure with the deciding fact on screen.
- **deck123 s7** (t5): `Cast Cancel` on `Vision Skeins {1}{u} (instant)` — a symmetric 2-mana cantrip
  that cannot kill you. Not on either list; the guide's unknown-name catch-all (L53-54) technically
  licenses it (a creature was on their board), so this is a **guide-rule gap**, not defiance.

**The new stack facts changed target quality — ledger #4's numeric rule is now writable.** 27
distinct stack lines this corpus; **all 24 spell lines carry name + mana cost + type + P/T**
(`1 (top): opponent's Master of Etherium…`-shape verified, e.g.
`1 (top): opponent's Overgrown Battlement {1}{g} (creature 0/4) [spell]`), and the counter option
line carries them too (`Cast Essence Scatter {1}{u} - can target on the stack: Acererak the Archlich
{2}{b} (creature 5/5)`). Baseline was 30 lines carrying **0** of the three fields.
**Ledger #4 → VALIDATED and CLOSED.** Metric it asked for: counters-on-non-threats **9/25 → 2/21**.

### 1.4 Path timing — **VIOLATED (the seat's #1 residual)**

13 Path casts. Two separate questions:

**(a) Target selection when a choice existed: 4 / 4 correct** — the highest-power creature every
time (139 s9 Dryad 2/4 over Grazer 0/3; 146 s8 Nadaar 3/3 over Triumphant Adventurer + a Goblin;
152 s9 Briarbridge Tracker 2/3 over Luminarch Aspirant; 126 s44 chose among three walls). ✅

**(b) The DECLINE floor (guide L64-66: decline when every offered target is `[defender]` or printed
power 0-1, unless life ≤ 12): 6 / 13 casts VIOLATE it, all at high life.**

| seq | life | sole/all offered target(s) | verdict |
|---|---|---|---|
| deck139 s5 (t3) | 20 | Arboreal Grazer 0/3 (a mana wall, named on the let-it-resolve list) | VIOLATED |
| deck152 s14 (t9) | 18 | Luminarch Aspirant (printed 1/1) | VIOLATED (defensible Magic — it is a real threat — but the guide's floor says decline) |
| deck126 s7 (t4) | 25 | Wall of Omens 0/4 | VIOLATED |
| deck126 s18 (t12) | 25 | Perimeter Captain {w} (0/4) [defender] | VIOLATED |
| deck126 s44 (t28) | 37 | Perimeter Captain, Overgrown Battlement, Wall of Omens — **all three 0/4 walls** | VIOLATED |
| deck126 s62 (t42) | 50 | Perimeter Captain 0/4 | VIOLATED |

Correct spends: 139 s9, 139 s24 (Dryad 2/4 at life 14), 146 s8, 152 s5 (Wolf), 152 s9, 162 s10 (Fate
Unraveler), 123 s18 (Bloodline Keeper).

**Contributing surface defect (wave-41 ledger candidate, §5.2):** the cast option line names the
target WITHOUT its P/T or `[defender]` tag — `Cast Path to Exile {w} - legal targets right now:
Perimeter Captain` — and when there is exactly one legal target no target sub-menu is asked, so the
window where the tags DO appear never opens. The facts are on the battlefield line (`Perimeter
Captain {w} (0/4) [defender]`), so this is an adherence failure with the fact available elsewhere,
not a false surface — but it is the same class as ledger #4 on a different emitter, and #4's fix
measurably moved behaviour once the fact rode the option.

### 1.5 Sweepers — **8 / 10 correct; the 0-creature class is GONE, a new 1-creature class appears**

Baseline's only error was a 4-mana wipe into `0 are creatures`. **This corpus has zero 0-creature
wipes.** All 10 windows, with the header count the guide keys on:

- ✅ ≥2 creatures: 139 s13 (3), 146 s12 (2), 152 s25 (3), 152 s37 (2), 126 s16 (2), 126 s51 (2),
  123 s27 (**17**).
- ⚠ exactly 1 creature, **no Path in hand**: 139 s14 (lone Brokkos 6/6 trample), 139 s18 (lone Illuna
  6/6). Guide says "exactly 1 creature → Path it and keep the sweeper"; the hand held no Path in
  either window and the lone creature was a 6/6. **Good Magic, guide-rule mismatch** → guide gap: the
  exactly-1 branch has no fallback for "you hold no Path" or "the lone creature is huge".
- ❌ **deck152 s47 (t27)**: `Cast Final Judgment {4}{w}{w}` at `of which 1 is a creature`
  (Intrepid Adversary 3/1) while holding **two Path to Exile** — and Path was offered as option 2 in
  the same list. Six mana and the exile-sweeper spent on a 3/1 the {W} instant answers. **VIOLATED.**

**Lightmine Field — 1 / 3 (VIOLATED).** Guide L106: cast only at 3+ opposing creatures.
- ❌ deck126 s33 (t20): header `12 permanents listed, of which **0 are creatures**` — 4 mana into an
  empty creature board.
- ❌ deck126 s38 (t22): `of which **1 is a creature**` (Perimeter Captain 0/4 defender) — the second
  copy, another 4 mana.
- ✅ deck123 s24 (t16): `of which **12 are creatures**`.
Both violations are in the same game against the same creature-light lifegain deck; 8 mana and 2
cards burned. This is the clearest single-rule defiance in the corpus and the header number was
correct on screen both times.

### 1.6 Sphinx's Revelation — cast threshold **VALIDATED**, X sizing **VALIDATED where asked**

- Every X menu was answered with **option 1 = the largest X**: 10/10 (X = 3, 3, 3, 4, 4, 5, 5, 9, 13
  … all `choice 1`). Baseline 5/5.
- Guide's "cast at 6+ mana" threshold held: the 12 windows where Revelation was offered at 3-4 mana
  (deck152 s11/s12/s15/s16/s38/s39/s40, deck126 s19, …) were all declined with `Cast nothing right
  now`. **This is what discharges the wave-39 defect situation** — see §2.
- **The stopgapped double-ask shape is gone** (§3).

### 1.7 Named-card counter table — **STOPGAP RETIRES; recommend REMOVING the table's stopgap line, KEEPING the table one more wave**

Register row 1 (`L54-55: "The stack prints only a name … read the rules text from the OPPONENT DECK
list"`), removal condition: *"delete when `/usr/bin/grep -c "on the stack: .*{" <corpus>/*.jsonl` is
non-empty."* Measured: **5, 2, 5, 7, 1, 5 — non-zero in all six seat files.**
**→ RETIRE L54-55. The sentence is now FALSE and under the trust doctrine a false statement about
the render must not ship.**

The named-card TABLE itself (L40-51) is a different question. Its ledger-#4 replacement — a numeric
rule keyed to the now-printed `(creature P/T)` — is finally writable, and the two residual misspends
argue for it (Arboreal Grazer's line literally read `(creature 0/3)`). **Recommendation to the guide
reviser: replace the two name lists with a numeric rule keyed to the printed type/P-T, and keep only
the non-derivable names (planeswalkers, Idyllic Tutor, and the drain/draw engines whose danger is
not visible in P/T) as a short exception list.** Do not delete the table without the numeric rule
landing in the same edit — the pool rotates, but the *facts* are now on screen.

---

## 2. LEDGER #2 — an explicit model pick vetoed by Baka's desirability roll

| metric the ledger asked for | result |
|---|---|
| `validation_reject_reask_exhausted` count | **0** — corpus-wide, all 64 seat files, **1,989 decisions** (was 5 in one window) |
| any `defer` record of any kind | **0** corpus-wide |
| `latency_ms = -1` records on the cast seam (or any seam) at deck125 | **0** across all 267 decisions |
| a committed X = 0 cast | **NULL — honest null, no attempt arose** |

**VALIDATED on the two falsifiable metrics.** The replay loop and the veto are both gone from the
evidence: not one deferred record, not one cached-replay record, in six games with 14 `{X}` casts.

**The X=0 null is a real null, and it is the guide's doing, not the engine's.** The trigger predicate
is *"the model picks an `{X}` spell in a window where `maxAnnounceableX == 0`"*. Zero-slack windows
**did** arise — Sphinx's Revelation was offered at exactly `Mana available: 3` twelve times (deck152
s11/s12/s15/s16/s38/s39/s40, deck126 s19, plus 4-mana near-misses) — and the pilot declined every
one, because the shipped guide tells it to cast Revelation only at 6+ mana. So the seat can no longer
reach the old repro by play, and **the regression fixture the ledger specified (an `{X}` spell with
exactly its coloured pips and no slack, GPT seat must commit X=0 in ONE record with a model call) is
still owed** — it is the only instrument that can close #2 positively. Recommend keeping #2 open as
FIXTURE-OWED rather than closing it on a corpus that structurally avoids the window.

---

## 3. LEDGER #8 — the cast window asked twice with post-tap mana

**VALIDATED. Zero occurrences of the defect shape.**

Mechanically re-derived rather than string-matched: for every adjacent record pair sharing turn,
phase and question line, I compared the option BLOCKS and the `Mana available:` figures.

- Pairs where the same cast question repeats back-to-back: **12**.
- Of those, pairs matching the ledger-#8 shape — *the spell answered in ask A is still offered in ask
  B, with B showing post-payment mana* — **0**. In all 11 substantive pairs the mana **dropped**
  (payment happened), the already-cast spell was **gone** from B's list, and B's option set was
  strictly different: 152 s14/s15 (4→3), 152 s25/s26 (6→2), 152 s37/s38 (7→3), 126 s18/s19 (5→4),
  126 s27/s28 (8→7), 126 s48/s49 (11→8), 126 s59/s60 (14→0), 126 s66/s67 (16→1), 123 s28/s29 (6→3),
  123 s66/s67 (18→0), 126 s33/s34 (9→5, identical options because the hand held a **second** Lightmine
  Field). These are correct fresh priority windows after a resolution, not re-asks.
- **One residual, harmless:** deck162 **s17/s18** (t13, MP1) — identical 2-option list, `Mana
  available: 1` on both, both answered `Cast nothing right now`. No mana lie, nothing lost, one extra
  round trip. Recorded as INFO-grade churn, n=1.

Baseline was 4 duplicate pairs in one game with the post-tap mana lie. **Ledger #8 → CLOSED.**

---

## 4. RECORD CHECK + LOSS AUDIT

### 4.1 The record is real — verified against `gameend`, not the table

`results.tsv` scores deck125 **0/6**. All six seat `gameend` records agree: `won: false` in every
game. The deck125 rows and their opponent-seat mirrors:

| game | results.tsv | deck125 `gameend` | opponent seat `gameend` | agree? |
|---|---|---|---|---|
| 139 v 125 | winner 139, 18-0, t26 | won false, my_life 0, opp 18, t26 | — | ✅ (and the wave-39 crash row is gone — this matchup completed naturally) |
| 146 v 125 | winner 146, 19-0, t20 | won false, 0 / 19, t20 | — | ✅ |
| 152 v 125 | winner 152, 34/-1, t30 | won false, −1 / 34, t30 | — | ✅ |
| 125 v 126 | winner 126, −10 / 80, t48 | won false, −10 / 80, t48 | — | ✅ |
| 125 v 162 | winner 162, 0 / 19, t15 | won false, 0 / 19, t15 | — | ✅ |
| **125 v 123** | winner 123, **life0 70, life1 7**, t35 | **won false, my_life 70, opp_life 7** | deck123: **won true, my_life 7** | ✅ — see §4.2 |

No ledger-#16-shape bogus row this corpus (no crash, no adjudication).

### 4.2 The vs123 loss is the seat's headline decision failure — **deck125 DECKED ITSELF while winning 70 to 7**

Resolution-verified chain:
1. deck123 was at **7 life**, deck125 at **67**, with two Staff of Nin on the board pinging 2/turn —
   a 4-turn kill, unopposed (deck123's board is 14 lands + three Intruder Alarms, **0 creatures**).
2. deck125's own prompt at **s71** (t35, MP1) printed, verbatim: **`Your library: 1 cards`**, and
   `Mana available: 18 total`.
3. The pilot chose option 4, `Cast Sphinx's Revelation`. It resolved for X = 3 (life 67 → 70,
   `gameend my_life 70`), drawing 3 from a one-card library → draw-from-empty → loss.
4. The two turns before it were the same mistake at lower cost: t32 Revelation drew 3 and t34
   Revelation drew 9 out of a library already under 15, with **10 cards discarded at cleanup** on t34
   because the hand blew past 7.

**Routing: GUIDE.** The guide's Revelation rule (L83, *"Cast Sphinx's Revelation when your Mana
available line reads SIX or more"*) is unconditional and there is no library clause anywhere in the
155 lines. The deciding fact — `Your library: N cards` — is rendered on every prompt and no rule
reads it. **Recommended guide edit for the reviser (highest value in this seat): a hard stop keyed to
the printed library count** — do not cast Sphinx's Revelation, and do not take an optional draw, when
`Your library:` is at or below the X you would announce (plus one for your draw step). Also worth a
clause: at 60+ life against a board that cannot damage you, the Staff clock alone wins; stop drawing.

### 4.3 The other five, routed at the decision level (ceilinged-seat rule)

| game | route | evidence |
|---|---|---|
| **vs139** (t26, 10 → 0) | **CONSTRUCTION + matchup** | Died to Everquill Phoenix 4/4 flier + Brokkos 6/6 recast **from the graveyard** (t25 narration). Three Supreme Verdicts were cast (s13/s14/s18) into a recursion deck; the guide's "use Final Judgment against anything that returns from a graveyard" was **unexercisable — Final Judgment never appeared in hand or in any option list all game** (honest null, not a violation). Two of the three Verdicts were the "lone 6/6, no Path in hand" windows of §1.5. |
| **vs146** (t20, 1 → 0) | **CONSTRUCTION-ceilinged** | Killed by combat: Triumphant Adventurer + a Goblin + **Hive of the Eye Tyrant** (a creature-land — a sweeper cannot answer it). Staff of Nin resolved at t18; the clock needed ~19 more turns. Decisions audited clean: 2/2 counters correct, Path on the right target, sweeper at 2 creatures. |
| **vs152** (t30, 8 → −1) | **PLAY (one identified error) + matchup** | Answer-discipline good (5 counters, all correct); the error is **s47**, Final Judgment on a lone 3/1 with two Paths in hand (§1.5). Also the seat's only two-Staff game that never assembled a clock. |
| **vs126** (t48, 50 → −10) | **matchup / combo** | At the last decision deck125 was at **50 life vs 20** with Emrakul resolved; it then died to the Sanguine Bond + Exquisite Blood loop in the untracked tail. Contributing decisions: 8 mana and both Lightmine Fields burned into a 0- and 1-creature board (§1.5), and four Paths spent on 0/4 walls (§1.4) instead of being held. Neither would have answered the enchantment combo, so this is a matchup loss with two identified wastes attached. |
| **vs162** (t15, 18 → 0) | **PLAY, but the guide's own rules collide** | t14 the opponent resolved **Underworld Dreams + Shield Sphere + two Howling Mines in one turn**, unanswered; Underworld Dreams then drained 5 in deck125's own draw step (Howling Mine's extra draws). Underworld Dreams and Howling Mine are **both on the guide's COUNTER-ON-SIGHT list**. The pilot DID get two windows (s17, s18) and answered `Cast nothing right now` **at `Mana available: 1`** — it had tapped out at t12/t13 to cast Staff of Nin (s15) per the guide's *"cast it the first turn it appears, ahead of anything except an answer you need this same turn"*. **The Staff rule and the "keep {1}{U} open" rule (L113) have no precedence between them**, and here the Staff rule won and cost the game. → guide item: order the two rules explicitly. |

---

## 5. NEW ITEMS — wave-41 engine/render ledger candidates

### 5.1 HIGH — an `{X}` cast sometimes commits with **no X-announcement window**; X is set below the affordable maximum by the payment path

**14 Sphinx's Revelation casts across the six games; only 10 raised `Announce the value of X`.** The
four that did not are all at high mana, and all resolved far below what was affordable:

| game | seq | `Mana available:` | X actually announced (from narration/life delta) | X affordable |
|---|---|---|---|---|
| deck123 | s56 (t31) | 16 | **3** | 13 |
| deck123 | s66 (t33) | 18 | **9** | 15 |
| deck123 | **s71 (t35)** | 18 | **3** | 15 |
| deck126 | s59 (t40) | 14 | (no menu; cast resolved, mana 14 → 0) | 11 |

Stderr proof, same file `game-125v123-1787501085.stderr`, same code path, menu present vs absent:

```
(WITH menu, line 2540)              (WITHOUT menu, line 2664)
AIPlayerGPT: casting Sphinx's ...   AIPlayerGPT: casting Sphinx's ...
AIPlayerBaka: ... using potential   AIPlayerBaka: ... using potential
  Mana now.                           Mana now.
in GetCost Seems ManaCost was not   ACTIONSTACK Add spell
  properly initialized              Action added to stack
AIPlayerGPT: Announce the value of
  X ... -> chose 1 of 14
ACTIONSTACK Add spell
```

The translog carries **no ANNOUNCE_X record** for the four, so the pilot was never asked — this is
the same *silent* class as ledger #2 (a decision taken away from the model without telling it), and
here it **decided a game**: s71 is the deck-out cast of §4.2, and had the pilot been asked it would
have had a chance to see `Your library: 1 cards` beside an X menu. Not root-caused; the correlation
is with high mana counts and with the `could be Paid with potential mana` payment branch. **Ask:**
the X window must arm on every `{X}` cast the GPT seat commits, or the engine must log which X it
chose and why. Repro quality: EXCELLENT (deterministic, one binary, with a with-menu control 100
lines earlier in the same stderr).

### 5.2 MEDIUM-HIGH, REPRESENTATION — the Path/target cast line names targets without P/T or `[defender]`, and the sub-menu that carries them never opens at one target

`Cast Path to Exile {w} - legal targets right now: Perimeter Captain` (deck126 s18) vs the
battlefield line for the same card, `Perimeter Captain {w} (0/4) [defender]`. When exactly one target
is legal the engine correctly makes no model call for the target — so the only surface that carries
the deciding facts is skipped. Cost this corpus: **6 of 13 Paths spent below the guide's floor**
(§1.4), 4 of them on printed 0/4 walls at 25-50 life. **Ask:** annotate the `legal targets right now:`
clause the way ledger #4 annotated the stack clause — `Perimeter Captain {w} (creature 0/4)
[defender]`. This is #4's fix applied to the adjacent emitter, and #4's payoff at this seat
(9/25 → 2/21) is the argument for it.

### 5.3 INFO — one duplicate cast-nothing ask

deck162 s17/s18, identical options, identical mana, both `Cast nothing right now`. n=1, no harm.
Logged so a future wave can tell whether it grows.

### 5.4 INFO — reasoning tax barely moved

Median `reasoning_chars` **6,030 → 5,558** (−8%) despite the Emrakul rehearsal dropping from 85% to
17% of traces; max 22,533 (was 12,733). The guide reclaimed the wasted deliberation and spent it
again. Not a defect; recorded so nobody claims a token win from the belief fix.

### 5.5 INFO — `unclosed <think> (budget/truncation); forcing the answer`

1 occurrence in each of the three games where deck125 was player 0 (125v123, 125v126, 125v162), 0 in
the other three. All three still produced a parsed answer (the corpus's only fallback at this seat is
1 `reasoning_only`, vs162). Worth a `n=3` watch line, nothing more.

---

## 6. CROSS-VALIDATION OF OTHER SHIPPED LEDGER ITEMS AT THIS SEAT

- **Ledger #3 (a COUNTERED spell narrated as `resolved`) — VALIDATED, coverage effectively 100% at
  this seat.** `was COUNTERED` lines in the final narration per game: 4, 2, 4, 3, 1, 5 = **19**,
  against 21 counters cast by deck125 — and the 2 shortfalls are both counters cast at the seat's
  LAST logged decision, with no subsequent prompt to narrate them. Baseline was **0 across all 21
  games.** The attribution the ledger asked for shipped too, verbatim from deck123's final prompt:
  `- Opponent's Bloodline Keeper was COUNTERED by Fall of the Gavel and went to the opponent's
  graveyard`. → **CLOSE #3.**
- **Ledger #4 — VALIDATED and CLOSED** (§1.3).
- **Ledger #8 — VALIDATED and CLOSED** (§3).
- **Ledger #2 — metrics VALIDATED, but keep open as FIXTURE-OWED** (§2).
- **Completeness invariant** — 6/6 natural ends at this seat, including the 139v125 matchup that
  core-dumped last wave. The Affinity/weak-registry crash fix held on the exact matchup that broke.

---

## 7. STOPGAP REGISTER — dispositions

| register row | removal condition | measured | disposition |
|---|---|---|---|
| **L54-55** *"The stack prints only a name … The card's rules text is in the OPPONENT DECK list at the top of this prompt"* (patches ledger #4) | `/usr/bin/grep -c "on the stack: .*{" <corpus>/*.jsonl` non-empty | **5, 2, 5, 7, 1, 5** — non-zero in all six seat files; 24/24 spell stack lines carry type + cost + P/T | **RETIRE.** The sentence is now false about the render; under the trust doctrine it must be deleted this wave. Replace with the numeric counter rule the facts now support (§1.7). |
| **L90-91 (guide L91-92)** *"If you choose Sphinx's Revelation and your very next prompt still shows Sphinx's Revelation in your hand with your mana unspent, cast something else this turn and take it again next turn"* (patches ledger #2/#8) | zero `validation_reject_reask_exhausted` records **and** no duplicate back-to-back cast asks | `validation_reject_reask_exhausted` = **0** corpus-wide (1,989 decisions); ledger-#8-shape duplicate pairs = **0** | **RETIRE.** Both halves of the condition fire. |

Both stopgaps come out; neither leaves a hole (the guide's own 6-mana threshold and the largest-X
menu line cover what they were bracing).

---

## 8. UNEXERCISED / HONEST NULLS

- **`attackers` / `blockers` windows: 0 in 6 games**, again — even in the two games where Emrakul
  resolved (it died to an edict before combat in one, and the game ended the turn it landed in the
  other). This seat still must not witness any combat-seam fix.
- **Mulligan: 6 keeps, 0 mulligans.** Guide L130-132 (*"If the mulligan question ever contains the
  words 'having already taken', this is your LAST look: answer 1, Keep this hand"*) is
  **UNEXERCISED for the second consecutive corpus** — 0 occurrences of the string in any deck125
  prompt. Per the skill's rung (a teach that goes a full corpus unexercised is carried at most one
  more wave), this is its last free wave: either a probe deck forces a mulligan, or the reviser cuts
  the line.
- **A committed X = 0 cast: never attempted** (§2) — the guide's 6-mana threshold structurally
  prevents the window. Fixture owed.
- **Supreme Verdict's `nofizzle`: never contested** — no opponent countered it, third corpus running.
- **Final Judgment vs a recursion deck (vs139):** the card never appeared in hand or options all
  game. The guide's exile-vs-recursion teach is untested here.
- **Emrakul's annihilator 6, protection, and the extra-turn attack:** the extra-turn trigger DID fire
  (deck123 t29→t30 narration), but annihilator and protection are still 0 occurrences — Emrakul never
  attacked. Still a construction/roster question, not a guide one.

---

## 9. VERDICT SUMMARY

| item | verdict |
|---|---|
| Guide #1 — Emrakul belief replaced | **VALIDATED** (144/169 → 45/267, and every survivor is a quotation or a real Emrakul) |
| Guide #1 escape clause — take a printed Emrakul | **VALIDATED 2/2, both arrived** (123 s49, 126 s66) |
| Staff-of-Nin plan adopted | **VALIDATED** (92/99 plans; pings 25/25, casts 6/6, Elixir 8/8) |
| Counter discipline | **VALIDATED** 9/25 → **2/21**; violations 139 s25, 123 s7 |
| Stack facts changed target quality (ledger #4) | **VALIDATED — CLOSE #4** |
| Path timing | **VIOLATED 6/13** (139 s5, 152 s14, 126 s7/s18/s44/s62); target choice 4/4 correct |
| Sweeper discipline | **8/10**; 0-creature class gone; **VIOLATED 152 s47**; 2 guide-gap windows (139 s14/s18) |
| Lightmine Field gate | **VIOLATED 2/3** (126 s33 at 0 creatures, 126 s38 at 1) |
| Sphinx's X sizing at the menu | **VALIDATED 10/10 largest X** |
| Ledger #2 metrics | **VALIDATED** (0 exhausted, 0 defers, 0 latency −1); **X=0 cast: NULL, fixture owed** |
| Ledger #8 | **VALIDATED — CLOSE** (0 defect-shape pairs; 1 harmless duplicate) |
| Ledger #3 | **VALIDATED — CLOSE** (19 `was COUNTERED` lines with attribution, was 0) |
| Record 0/6 | **VERIFIED** against 6 `gameend` records + the deck123 mirror |
| Loss audit | vs123 **PLAY (guide gap — self-deck at 70-7)**; vs162 **PLAY (guide rule collision)**; vs152 PLAY (1 error) + matchup; vs139/vs146 CONSTRUCTION; vs126 matchup/combo |
| New HIGH | **`{X}` cast commits with no X-announcement window, X below max — 4/14 casts, decided vs123** |
| New MEDIUM-HIGH | **Path cast line names targets without P/T / `[defender]`; sub-menu skipped at one target** |
