# Wave-13 review — deck109 (Hellrider, mono-red aggro / RDW)

**Corpus:** PRIMARY wave-13b `matchups-20260715-183228`, round-robin, deck109 seat = 6 games.
Binary `/tmp/wagic-a75e2625e` (Force r2+r3 + d5d7b1f2e auto-tap two-pass + c3e6d95b2 name-echo
protocol + 42e8bc180 BLOCKS:none + 240s timeout). Wave-11 guide (FROZEN). Core prompt unchanged
(8th PASS). First clean validation after the wave-12/13a Spark-crash contamination.

**Record: 5/6** — beat 110, 131, 44, 133, 135; LOST to 140. Per doctrine this is context only.
NOTE: the wave-11 result INVERTED — last wave deck109 lost to 133 and beat 140-class; this wave it
CRUSHED 133 (20-0, T8) and lost to 140. Pure draw-speed variance on the two structural-edge
opponents. Unit of analysis below is the DECISION with seq repros.

| seat file (epoch) | opp | result | end (my / opp, turn) | earned? |
|---|---|---|---|---|
| 1784158351 | 140 | **LOSS (adj0)** | 5 / 21, T32 (life-adjudicated, deck109 BEHIND) | matchup+flood, no decision error |
| 1784158356 | 110 | WIN (adj1) | 15 / 4, T8 (life-adjudicated, deck109 AHEAD) | earned race |
| 1784160756 | 131 | WIN | 14 / -6, T15 | earned wide grind |
| 1784162895 | 44  | WIN | 9 / -1, T9 | earned untouchable-flyers race |
| 1784168784 | 133 | WIN | 20 / 0, T8 | earned curve-out |
| 1784170894 | 135 | WIN | 20 / 0, T9 | earned curve-out |

---

## HEADLINE (NEW decision-level class) — the MIXED index+name ATTACK form SILENTLY DROPS the named attacker

This is the one genuine decision-level failure class surfaced beneath the fixed noise this corpus,
and it is a SHARPENING of the brief's flagged `ATTACK: <name>` ledger candidate. The corpus contains
THREE attacker-declaration forms; they fail at DIFFERENT layers with DIFFERENT severity:

**(A) FULL-INDEX `ATTACK: A1, A2, A3` — parses correctly.** 17/30 attacker windows. Clean.

**(B) FULL-NAME `ATTACK: Hellrider` / `ATTACK: Rakdos Cackler, Legion Loyalist, Legion Loyalist`
— UNPARSED -> heuristic -> OUTCOME-NEUTRAL.** 3 instances (vs140 s33, vs131 s19, vs135 s13). The
name-only form is rejected by the attackers parser (`fallback=unparsed_reply`, `choice=-1`,
`chosen_text='None'`), BUT the baka attacker heuristic then declares ALL eligible attackers, which
MATCHES the aggro intent. Verified in combat via the next record's game log:
- vs131 s19 -> next log `You declared attackers: Ash Zealot, Rakdos Cackler, Legion Loyalist` (all 3
  the model named).
- vs135 s13 -> next log `You declared attackers: Stromkirk Noble, Rakdos Cackler, Ash Zealot` (all 3).
- vs140 s33 (`ATTACK: Hellrider`) -> game log shows `Damage: 3 dealt by Hellrider` + `Damage: 1 dealt
  by Hellrider` — Hellrider (haste) DID attack.
This is exactly the brief's "note instances, don't re-diagnose" item; harmless HERE because the
fallback and the model's intent coincide for a pure-aggro deck.

**(C) MIXED `ATTACK: A1, Rakdos Cackler` — PARSES PARTIALLY, drops the named attacker, records a
NON-FALLBACK "success".** THIS IS THE DANGEROUS ONE and it is NOT caught by the fallback machinery.
The parser consumes the leading A-indices, ignores the trailing NAME token(s), and records a normal
`choice=N (count)` with fewer attackers than the model intended — no `fallback` field, no `-1`, so it
looks like a clean declaration in the ledger. 5 instances / 3 games, 6 attacker-declarations dropped:

| seq | T | line | intended | recorded | dropped |
|---|---|---|---|---|---|
| vs140 s7  | 3  | `A1, Rakdos Cackler` | 2 | 1 (Stromkirk Noble) | Rakdos Cackler |
| vs140 s40 | 31 | `A1, Rakdos Cackler` | 2 | 1 (Legion Loyalist) | Rakdos Cackler |
| vs110 s10 | 3  | `A1, Rakdos Cackler` | 2 | 1 (Legion Loyalist) | Rakdos Cackler |
| vs110 s14 | 5  | `A1, A2, Stromkirk Noble, Hellrider` | 4 | 2 | Stromkirk Noble + Hellrider |
| vs135 s8  | 3  | `A1, A2, Rakdos Cackler` | 3 | 2 | Rakdos Cackler |

**Confirmed in combat** at vs140 s7: the plan states verbatim *"I will attack with both to deal 3
damage total (1 from Noble, 2 from Cackler), bringing the opponent to 17"*, yet the next record's log
reads `You declared attackers: Stromkirk Noble` — ONE attacker, Rakdos Cackler held home. The model
obeyed the guide's "declare EVERY creature that can attack"; the HARNESS dropped it.

**Layer attribution: harness / attackers-parser (name->index reconcile), NOT the guide, NOT the
model, NOT the core.** The model reasons correctly and NAMES every attacker it wants; the loss is
purely at the parse seam. Per the binding layer-routing doctrine (parser bugs -> notes ledger, never
guide prose; "papering a parser bug in prose is the sanctioned-against escalation"), this routes to
notes.md Ledger Item A. The guide already says the right thing.

**Outcome coupling (reported separately per the wave-11 headline-4 method):** OUTCOME-NEUTRAL in the
4 wins (deck109 was so far ahead the dropped 1-2 power/turn didn't matter). In the LOSS (vs140) it
was a real but NON-DECISIVE clock tax — s7 dropped 2 Rakdos damage on T3, s40 dropped 2 more on T31,
in a race deck109 lost 5-vs-21 to a lifegain-control deck it could never have out-punched anyway (see
below). Do NOT inflate this into the cause of the loss; do NOT downgrade it because the corpus was
mostly won — it is a live, silent, recurring mechanical defect that a grindier or mirror corpus could
make fatal (the wave-11 harmless-recurrence-yields-clean-fixture logic applies: vs140 s7 is the clean
loss-decoupled-enough repro).

**Scoring note (why wave-11 missed this):** wave-11 scored "26/26 full declaration, 0 intent-collapse"
by COUNTING attacker windows and reading `chosen_text` (which shows creature names — just fewer than
intended). The mixed-form drop is invisible to a window count and to `chosen_text`; it only appears
when you parse the reply's `ATTACK:` line for named tokens and compare INTENDED vs RECORDED count.
See skill.md — this is a method refinement, not a guide edit.

## Hybrid {B/R} + {R/W} line — VERIFIED END-TO-END, all 7 casts EXECUTED (CLOSED, keep at full prominence)

The wave-10 hybrid-mana line held perfectly and the engine EXECUTED every hybrid cast:
- **Rakdos Cackler `{b/r}` cast+resolved 5/5** (vs140 s5, vs140 s38, vs110 s6, vs131 s9, vs135 s6) —
  each `hand -> stack -> battlefield` confirmed in events.
- **Boros Reckoner `{r/w}{r/w}{r/w}` (triple hybrid off three Mountains) cast+resolved 2/2**
  (vs131 s27, vs133 s14) — verifies the guide's "three Mountains cast it" line at the engine level.
- **0 "lacking black mana" / "no black" declines** across all 6 games (grep = 0).
- **0 hybrid spiral markers** ("trick"/"misleading"/"broken") — 0.
- **Unleash "1/1 Counter" taken 4/4** (vs140 s6/s23/s39, vs110 s7, vs135 s7).
The card note is the SOLE carrier (Ledger Item B: option-line `- hybrid:` annotation still UNSHIPPED —
0 `- hybrid:` strings in any deck109 prompt). Per pause-aware demotion doctrine: KEEP at full
prominence, frozen byte-identical.

## Burn discipline — CLEAN (0 self-target; the one flagged pick was a forced edict, not burn)

- **0 burn spells at own face/creatures.** The self-target scan's one hit (vs140 s17,
  `Goblin [your battlefield]`) was a FORCED SACRIFICE ("Choose the target for Sacrifice" — deck140 ran
  an edict effect); the model correctly sacrificed a tapped Goblin (least valuable). Not a burn
  decision, correct edict response.
- **Token-maker exception FIRED:** vs131 s7->s8, Searing Spear targeted **Young Pyromancer** (choice 2
  over the opponent's face) and it died (s9) — the guide's kill-on-sight exception, correctly applied
  (opp not near lethal; Young Pyromancer would have made a blocker per spell).
- **All other burn to FACE** (vs140 s14/s20/s35, vs110 s9/s16/s18, vs131 s16, vs44 s4, vs133 s9) — 9/9
  face targeting on the default windows.

## Ask-layer unparse fallbacks (3) — all the benign long-ramble discard class, all in WON games

`empty_reply = 0` at this seat (the wave-12/13a flood was Spark dying; CLOSED). Three `unparsed_reply`
ask fallbacks, all the brief's "long reasoning rambles correctly discarded by the guard" class — the
reply never reached a `CHOICE: N (name)` line before the budget/format guard fired, heuristic answered:
- vs131 s17 (T9, 5 cast options): model rambled comparing Ash Zealot vs Krenko's Command; heuristic
  answered, next window cast Legion Loyalist. WON.
- vs44 s16 (T9, Searing Spear TARGET list of 7): model rambled face-vs-Scion-of-Oona while Arcane
  Denial was on the stack; heuristic answered. WON (opp -1, T9) — target choice moot.
- vs135 s12 (T7, 4 cast options): truncated cast-sequencing ramble, no CHOICE line. WON 20-0.
**The wave-11 vs135 s12 unlisted-card fixation (`CHOICE: 5` for unlisted Hellrider) did NOT recur** —
this wave's vs135 s12 is a plain truncation ramble, not an out-of-range-illegal-index. That
sub-variant stays where it was (single/second-seat, unpromoted).

## Brief-item checks for this seat

- **#3 BLOCKS: none** — ONE blocker window all corpus (vs131 s25, T12): surfaced line
  `Unblocked ... up to 1 - you would be at 14 - NOT lethal`; model declared `BLOCKS: B1:none, B2:none,
  B3:none` (the PARSEABLE per-attacker form, not bare `BLOCKS: none`), `chosen_text='no blockers'`,
  reasoning obeys the NOT-lethal branch. CORRECT all-decline, NOT a misparse of intended blocks.
- **#4 name-echo remaps** — 0 echo-vs-index conflicts corpus-wide (every `CHOICE: N (name)` echo
  matches the option at index N). 100% echo compliance; no reconcile arm was invoked at this seat, so
  no wrong remap possible.
- **#5 auto-tap "Mana available" trust** — N/A: deck109 is 20 basic Mountains (single color), no
  color-stranding surface, no counterspell to be stranded. Brief items #1 (Force) and #2 (auto-tap
  color-stranding) are N/A at this seat for the same reason.

## The vs140 LOSS = lifegain-control matchup + mana flood, NO decision error of consequence

deck140 is a lifegain/control deck (Venser's Journal, Staff of Nin, edict/Sacrifice) — the model's own
final plan reads *"I am at 5 life against an opponent at 22 with multiple life-gain engines."* The
opponent's life went DOWN under burn/attacks then back UP (14 -> 17 -> 22) as the lifegain engines
outpaced deck109's clock. deck109 FLOODED — it drew ~10 Mountains and could not deploy its first
Hellrider until T25 (s32), spending T17/T19/T21/T23 doing nothing but `Play Mountain`. It played the
race line correctly (all burn to face, deployed every castable body, attacked each turn) but a pure
RDW deck has no reach to punch through a lifegain-control shell when it floods. Same structural class
as the wave-11 vs133 loss and the guide's EVASIVE/UNTOUCHABLE and Obliterator notes: a known deck-
construction ceiling (no card advantage, no reach vs lifegain), surfaced by draw variance and made a
loss by the latency adjudication (turn 32). Do NOT relitigate deck construction at 5/6; do NOT read
the adjudication as a play-quality regression (brief's binding note).

## Frozen guardrails / spine — held (freeze-checked verbatim against this corpus)
- **DEPLOY-AND-ATTACK + "declare EVERY creature that can attack"** — model INTENT was full declaration
  at every window; the only under-declarations are the MIXED-form HARNESS drops (headline), not
  guide-obedience misses. Every castable creature deployed each turn.
- **BURN face-default + Pillar `{R}` corrective** — 0 self-target, 0 false `{1}{R}` cost belief; Young
  Pyromancer killed on sight.
- **Hybrid `{B/R}` + `{R/W}` notes** — VERIFIED end-to-end, all 7 casts executed. Frozen at full
  prominence.
- **BLOCKING re-anchor (block ONLY on surfaced LETHAL)** — NOT-lethal branch OBSERVED OBEYED 1/1;
  LETHAL branch + exception-(a) + Obliterator absolute UNTESTED (vs133 raced, 0 blocker windows) — 4th
  consecutive zero-window wave, KEEP at full prominence (pause-aware demotion doctrine).
- **Unleash "1/1 Counter"** — always taken (4/4).
- **EVASIVE/UNTOUCHABLE BOARDS note** — load-bearing; vs44 WON this wave against the untouchable-
  flyers class by running the prescribed face race.
- **PHYREXIAN OBLITERATOR note** — vs133 cast Obliterator but deck109 raced it out 20-0 by T8 (never a
  block window); guardrail neither fired nor violated, KEEP.
- **adj rows:** vs140 (adj0, deck109 behind) + vs110 (adj1, deck109 ahead) cross-checked against seat
  life — consistent. No `[deals 0]` windows. No fetch/discard/artifact windows (all N/A for RDW).

## Guide verdict: FROZEN (byte-identical to the live guide; md5 9b82b500ac675be6936d2a31739ca615)

No decision-level failure class routes to the guide. The MIXED-form attacker drop is a harness/parser
item (notes Ledger Item A); the hybrid line is verified; brief items #3/#4 pass; #1/#2/#5 N/A.
`strategy.txt` is byte-identical to `deck109_strategy.txt` (verified `diff` = empty).
