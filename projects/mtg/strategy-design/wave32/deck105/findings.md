# deck105 "Infected Wounds" (BG Infect) — wave-32 seat review (STEP-0, GUIDELESS DEBUT)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260727-115847`, binary `7fabd9bd0`, -T 3000,
REPPENALTY=1.05. Seat logs: 6 games, **148 model decisions, 0 fallbacks, 0 crash signatures**.

**Record: 6/6 — the campaign's first Step-0 guideless perfecta.**

---

## 0. HEADLINE — the win condition, arrival-traced

**All six wins are POISON alt-wins. Not one opponent was reduced to 0 life.**

| game | loser | loser's life at gameend | deck105's own life | turn |
|---|---|---|---|---|
| 105 v 36  | deck36  | **20** | 20 | 12 |
| 146 v 105 | deck146 | **21** | 6  | 15 |
| 158 v 105 | deck158 | **15** | 3  | 13 |
| 105 v 116 | deck116 | **23** | 20 | 12 |
| 152 v 105 | deck152 | **23** | 2  | 17 |
| 139 v 105 | deck139 | **14** | 8  | 13 |

(`gameend` records, both seats; integrity clean — 12 gameend logs for 6 games, `won` fields
complementary in all 6.)

Arrival trace, worked in full for 105 v 36 (the cleanest — both players on 20 life at the end):
Ichor Rats ETB t7 (+1) -> Plague Stinger t7 (+1) -> Ichor Rats ETB t9 (+1) -> Contagious Nim +
Plague Stinger t11 (+3) = **6 poison visible in the last logged narration**; the game then ends
on the t13 swing that stderr records as `AIPlayerGPT: declared attack (4 of 4) in one reply` ->
`explicit all-decline (no blockers)` -> `Resolving Action on stack: 11DamageStack` ->
`WAGIC_SELFPLAY_RESULT winner=0 life0=20 life1=20 turn=12`. Those four attackers are Plague
Stinger 1 + Contagious Nim 2 + Necropede 1 + Ichorclaw Myr 1 = 5, so poison lands on **11 >= 10**
at 20-20 life. The same shape holds in the other five (last-logged poison 7-9, game ends on the
next unblocked infect swing).

**The deck does NOT win on -1/-1 wither bodies.** -1/-1 counters are real and do fire on
creatures — `- Counter added to Wolf: -1/-1 (now 0/0) [from Necropede]`, 84 occurrences — but
they are incidental combat cleanup. Every kill came from the player-facing half of infect.

Three losers were at **21, 23, 23** — they had GAINED life above 20 and it was worth nothing.
See section 5 (roster-balance).

---

## 1. POISON SURFACE AUDIT — the answers

### (a) How do poison counters render? THEY DO NOT. Anywhere.

Exhaustive check across all 12 logs of the six deck105 games:

- **Life area** — `AIPlayerGPT.cpp:2251` emits exactly
  `out << "Your life: " << this->life << " | Opponent life: " << (opp ? opp->life : 0)`.
  No poison field. Every board snapshot in all six games reads e.g.
  `Your life: 20 | Opponent life: 20` on the turn the opponent was on 6 poison.
- **Battlefield / player lines** — no poison token. `[counters: Nx -1/-1]` renders on creatures;
  players have no counter line at all.
- **Narration** — infect combat damage narrates as
  `- Damage: 1 dealt by Plague Stinger to the opponent`, **identical in shape to real damage**,
  the only difference being the ABSENCE of the adjacent `- Opponent's life -N (now X)` line that
  real damage emits (compare, same corpus, deck158's Orc army: `- Opponent's life -4 (now 8)` /
  `- Damage: 4 dealt by Orc army to the opponent`). The tell is a MISSING line, not a present
  one — the weakest possible signal.
- **Non-combat poison** — Ichor Rats' `auto=alterpoison:1 all(player)` narrates **nothing**.
  Verbatim from the vs36 log: `- Your Ichor Rats: hand -> stack` / `- Your Ichor Rats: stack ->
  battlefield` / `- Phase: Attackers`. Both players silently gained a counter between those
  lines. This happened 4x across the corpus and was never once visible.
- **Option lines** — the word "poison" appears in the prompt in exactly two places, both card
  REMINDER text shipped by i13: `{card text: "Infect (This creature deals damage to creatures in
  form of -1/-1 counters and to players in form of poison counters.)"}` and Ichor Rats'
  `"each player gets a poison counter"`. Nothing else.
- The engine HAS the number: `Player::poisonCount`, `Player::isPoisoned()`, and
  `GuiStatic.cpp:138-140` renders it **for the human frontend**. Textbook ishuman-lens gap — the
  human sees the count, the LLM opponent does not. -> **N-105a**.

### (b) Does the model UNDERSTAND poison? Yes — conceptually. It cannot COUNT it.

Understanding is not the problem. The pilot's plans are correct infect strategy from turn 0:
"aiming to reach 10 poison counters before the opponent stabilizes or deals lethal damage"
(vs36 s6 t4); "Infect deals damage in the form of -1/-1 counters to creatures and poison
counters to players. 5 damage = 5 poison counters" (vs158 s12 t9); and it priced Ichor Rats'
symmetric cost correctly — "giving the opponent a poison counter is negligible compared to the
immediate pressure ... and advancing your own counter count" (vs36 s8 t6). The wave-20 FOURTH
idiosyncrasy face (symmetric card read as one-sided) did **not** fire here.

What fails is arithmetic on an unrendered number, every time it matters:

- **vs36 s17 t12** — `"I am at 20 life and need 10 poison counters to win; I currently have 0."`
  The opponent was on **6**. It then attacked with all four and reached 11, winning, while
  writing in the same reply `"bringing the opponent to 5/10"` (s19 t12). It won the game while
  believing it was half way there.
- **vs146 s23 t15** (land drop, 5,262 chars) — `"The opponent is at 21 life and has 11 poison
  counters (based on the cumulative infect damage dealt by Corpse Cur and Plague Stinger in
  previous turns: 2+1+2+1+2+1+2 = 11)"` — actual was **8** (double-counted a turn) — and then, in
  the same reply, `"the opponent has 21 life. I need 20 poison. I have 11. I need 9 more."` It
  conflated the poison threshold with the LIFE total.
- **vs146 s22 t14** (blockers, **8,827 chars / 143.6 s**) — the diagnostic instance. Verbatim:
  `"So they have taken 11 poison damage ... and are at 21 life? ... If they have 11 poison
  counters, they have already lost. The game should be over. ... However, the game log continues
  to Turn 15. This implies the opponent is NOT dead yet. ... Maybe the log is simulating a game
  where the player forgot to check for poison loss? ... If the game is still going, then my count
  is wrong."` The pilot detected the inconsistency, could not resolve it against any rendered
  number, and burned its whole budget concluding its own count was untrustworthy.

Verdict: **the model does not need to be taught what poison is; it needs to be SHOWN the count.**
This is a representation defect, not a strategy or knowledge gap — per perception-vs-strategy
routing, guide prose cannot cure it, and the guide's counting procedure ships as an explicit
STOPGAP (removal condition recorded in notes.md N-105a).

### (c) Does infect damage narrate distinctly from regular damage? No — and the difference is a
DELETION, which is worse than a wrong label.

| | narration |
|---|---|
| regular damage | `- Opponent's life -4 (now 8)` **+** `- Damage: 4 dealt by Orc army to the opponent` |
| infect damage | `- Damage: 2 dealt by Contagious Nim to the opponent` (life line absent, life unchanged) |
| infect ETB poison | *(nothing at all)* |
| infect damage to a creature | `- Counter added to Wolf: -1/-1 (now 0/0) [from Necropede]` **+** `- Damage: ...` — correct and distinct |

The CREATURE half of infect is rendered honestly; the PLAYER half is not. -> **N-105a**.

### (d) The cross-seat consequence — a game-losing FALSE forecast at the DEFENDER's seam

`AIPlayerGPT.cpp:6768-6770` prints, at every blocker seam, a life forecast built from raw
attacker power with **no infect check**. Against infect attackers it is simply false. It fired in
**12 of 12** infect blocker windows across the six games, always in this shape:

> `Your life: 20. Unblocked, these attackers deal up to 5 - you would be at 15 - NOT lethal: block
> only where the trade favors you; taking damage while ahead is often correct.`

Three falsehoods in one line for an infect swing: the life will not change; "NOT lethal" is a
claim about a resource the attack does not touch; and "taking damage while ahead is often
correct" is the exact inverse of correct play against a 10-counter clock with no life cushion and
no removal.

**Game-losing repro, deck36 s25 t12 (kind=blockers)** — the killing swing, verbatim from the
reply: `"Since the total damage (5) is not lethal (I would drop to 15), and blocking with the Pest
achieves nothing ... I declare no blocks"` -> `BLOCKS: none` -> poison 6 -> 11, deck36 loses at
20 life. The engine told it the lethal swing was not lethal and it obeyed. -> **N-105b**.

4 of the 12 windows ended in `no blockers`. This surface is worth some fraction of the 6/6.

---

## 2. THE 6/6 DECOMPOSITION — the PRESERVE LEDGER (wave-31 HL8 / Step 0-bis rule 0)

Enumerated FIRST, before any rule was drafted. Every row is FROZEN territory the initial guide
must not legislate over.

| seam | metric | verdict |
|---|---|---|
| **Attackers** | **29 / 29 windows declared EVERY creature the engine offered.** Zero `ATTACK: none`. Zero partials (chosen count == `A1..An` eligibility count in all 29). | PERFECT — attack floor becomes a SHORT confirmatory line, never a loud #1 slot |
| **Casting** | **1 `Cast nothing` in 90 ask windows (1.1%)**, and it was CORRECT (deck116 s10 t8: 3 lands out, cheapest castable card in hand 4 MV) | no deploy floor needed |
| **Target selection** | 2 / 2 correct. Necropede's death trigger killed the opponent's Wolf, not its own Blight Mamba (vs152 s12); Corpse Cur returned **Contagious Nim** (2 power) over Ichorclaw Myr and a second Corpse Cur — the highest poison-per-swing option (vs139 s25) | correct, unaided |
| **Putrefax one-shot** | 1 / 1. vs158 s13 t9 cast it, s14 t9 attacked with it the SAME turn (haste), 5 poison landed, `- Your Putrefax: battlefield -> graveyard` at end step | understood the sacrifice clause unaided |
| **Corpse Cur ETB** | 2 / 2 taken, both recurred a real infect body | correct |
| **Ichor Rats symmetric ETB** | reasoned correctly at 3/3 casts; never read as one-sided | wave-20 FOURTH face did not fire |
| **Mechanical integrity** | 0 fallbacks / 148; 0 crash signatures / 6 stderr; 0 re-ask markers | clean |

Load-bearing behaviours the guide MUST preserve, with citations:

1. **Total-commitment attacking.** The whole clock is "every body swings every turn." 29/29.
   (vs36 s7/s10/s13/s16/s19; vs116 s7/s8/s11/s14/s16; vs152 s7/s11/s16/s28/s34/s40.)
2. **Deploy-every-turn casting.** 89/90 windows took an action. No hold behaviour to break.
3. **Racing while behind on life.** It finished at 6, 3, 2 and 8 life in four games and won all
   four. A guide that installs a defensive posture would convert wins into losses. The blocking
   rule must therefore be a POSITIVE per-item test with a life-total off-ramp, never
   "stabilise first."
4. **Putrefax / Corpse Cur / Necropede-trigger handling** — all correct; say each once, briefly.

### Near-misses the guide should shore up (ranked; there is no LOSS to key the #1 slot on)

**NM1 — BLOCKING TRADES THE CLOCK AWAY, THREE TIMES AT 19-20 LIFE. (the #1 slot)**
11 blocker windows, 7 blocks declared. Four of the seven spent an infect body for nothing:

- **vs36 s11 t7, at 20 life** — `Ichor Rats blocks Silver Myr`. Both died. Traded a 2-poison-per-
  turn attacker for a 1/1 mana dork, on 20 life, against a deck with no clock.
- **vs152 s8 t6, at 20 life** — `Ichor Rats blocks Intrepid Adversary`. Ichor Rats died. That game
  ended with deck105 on **2 life**, needing every point of clock it had.
- **vs139 s8 t6, at 20 life** — `Contagious Nim blocks Dryad of the Ilysian Grove` (2/4). Nim
  died; the Dryad only shrank.
- **vs146 s18 t12, at 15 life** — `Skithiryx, the Blight Dragon blocks Silverquill Silencer`.
  **The deck's best finisher — a 4/4 flying infect body, 40% of a lethal clock per swing — traded
  for a 4/3 ground creature**, one decision after the pilot's own PLAN line read *"Next turn,
  attack with Skithiryx, Corpse Cur, and Plague Stinger to deliver 7 poison counters and win the
  game."* The rendered forecast said `you would be at 7 - NOT lethal`. This is the wave-31 HL3
  shape exactly: the value teach the pilot itself wrote, evaporating at the combat seam under a
  local re-derivation ("blocking to remove a big threat is good").

The four declines were all defensible (vs146 s10/s14 at 20/19 life; vs139 s12 at 20; vs158 s5 at
19), and the ONE block at low life was right (vs146 s22 at 10 life vs a 9-power swing — took 4
instead of 9). The seam is not broken, it is UNGATED; a life-keyed gate fits the evidence in both
directions.

**NM2 — SKITHIRYX NEVER CONVERTED. 0 damage from 2 casts.** vs36 s18 t12 (cast, game ended before
it untapped) and vs146 s16 t11 (cast, then blocked with it and lost it). `{B}: gains haste` was
never activated once — the vs36 turn-12 cast could have swung for 4 the turn it landed. On a
10-counter clock a 4-power flier is 40% of the game.

**NM3 — MULLIGAN ON A CRITERION THE DECK CANNOT SATISFY.** vs146 s1 shipped
`Ichorclaw Myr {2} (1/1); Forest; Forest; Swamp; Ichor Rats {1}{b}{b} (2/1); Necropede {2} (1/1);
Ichor Rats {1}{b}{b} (2/1)` — 4 lands, 3 cheap infect creatures, both colours — reasoning *"lacks
any early-game mana fixing or low-cost threats ... Mulliganing for a hand with at least one
1-drop"*. **deck105 contains zero 1-mana cards** (verified against primitives: cheapest are Blight
Mamba {1}{G}, Plague Stinger {1}{B}, Necropede {2}, Ichorclaw Myr {2} — all MV 2). It mulliganed
again (s2), kept at 5 (s3), and at the bottoming step (s4) put **both** Blackcleave Goblins on the
bottom, keeping a five-card hand of **five lands**. It still won that game — on poison. i7's
bottoming machinery worked perfectly (2 named, 2 bottomed, labels constant); the CHOICE was the
failure. An unterminated mulligan pursuit (wave-31 HL10 / Step-5 rule 6) on a criterion that can
never be met.

**NM4 — MANA RE-DERIVATION TAX, concentrated on LAND DROPS (wave-31 HL9 signature).** 61
"cannot cast / cannot afford / not enough mana" statements across 148 replies. Sorted by tax,
**four of the top eight replies are LAND DROPS** — a decision that cannot be hard: deck158 s12 t9
`Play Forest` 8,683 chars / 188 s; deck116 s9 t8 `Play Forest` 8,358 / 151 s; deck152 s5 t5
`Play Swamp` 5,346 / 73 s; deck146 s23 t15 `Play Forest` 5,262 / 83 s. Reading the low-stakes rows
first surfaces two unsettled beliefs: (i) the poison count (1b), and (ii) generic-cost payability
off off-colour lands — deck116 s9 verbatim: *"I cannot pay {2} generic with a Swamp unless I have
a way to convert. Wait, 'any color can pay a generic cost'. Yes!"*, re-derived from scratch eleven
times in ONE reply.
**Decision cost: essentially zero.** i12's `Mana available: N total (... colours you can make:
{g}{b})` line held — not one false decline of an offered Cast in the corpus. The single exception
is NM3, where the same re-derivation ran at the MULLIGAN seam, where no option list exists to
correct it. i12 is VALIDATED for correctness and does not touch tax.

### Guide sizing

Leaks NM1-NM3 are the whole guide. Everything in the preserve ledger gets at most one confirmatory
clause. Per HL8, the first duty at a strong Step-0 seat is DO NO HARM.

---

## 3. STEP-1 BATCH — steady-state citations from this seat

- **(i8) combat tags — VALIDATED, zero paradox verbalizations.** 50 `[tapped - attacking]` and
  275 `[summoning sick - cannot attack this turn, but CAN block]` renders across the seat's logs.
  Worked instance, deck36 s25 t12: attackers render `Plague Stinger ... [tapped - attacking]` while
  the untapped Skithiryx renders `[summoning sick - cannot attack this turn, but CAN block]` in the
  same board line; the reply reasons about them correctly and separately (*"The opponent has four
  attackers, all tapped. I have only one blocker (Pest, 0/1)"*). Zero "no untapped creatures to
  block" misreads at either seat of these six games.
- **(i11) creature markers on mulligan hands — VALIDATED.** Every mulligan hand line carries
  `[creature]` / `[artifact creature]` / `[enchantment]` plus P/T, e.g. vs146 s1 `Ichorclaw Myr {2}
  (1/1) [artifact creature]; ... Ichor Rats {1}{b}{b} (2/1) [creature]`. **Zero "zero creatures"
  mulligans of hands containing one** across all 6 openers. NM3 is NOT this class — the pilot
  correctly enumerated the three creatures and shipped the hand on a curve criterion the deck
  cannot meet.
- **(i13) card text on Cast lines — VALIDATED, 67/67.** Every real Cast line carries
  `{card text: ...}` (the 40 without are `Cast nothing right now`). This is the load-bearing item
  at this seat: **the infect reminder text is the only place the word "poison" exists in the entire
  prompt**, and it is what let a guideless pilot play infect strategy at all from turn 0. No
  length-related degradation: p50 reply 1,047 chars, p95 6,266; 0 truncations, 0 unparsed across
  148 decisions. Latency median 17.1 s, p90 61.8 s, max 219 s (the max is a poison-count spiral,
  not a length effect).
- **(i7) London bottoming — VALIDATED at this seat.** vs146: 2 mulligans -> exactly 2 bottoms
  (`PUT: 1, 6`), option labels constant across the ask, no re-clamp needed.
- **(i12) count-first mana line — VALIDATED for correctness** (zero "I cannot afford" declines of
  an offered payable cast in 90 ask windows), **and does not reduce tax** (NM4).
- **(i5 / i6) — NOT EXERCISED at this seat.** Zero `validation_reject_reask`,
  `validation_reject_exhausted`, `truncated_abandoned_heuristic`, `unparsed_reply`, `empty_reply`,
  `retracted_choice` in 148 decisions. No X-spells, no convoke, no truncations. Consistent with the
  brief's corpus-level note.
- **(i16) — clean.** 0 crash/FATAL/assert signatures in all 6 stderr files for a brand-new deck.
- **(i1, i2, i3, i4, i9, i10, i14, i15) — not exercised at this seat** (no duals, no convoke, no
  Agadeem, no lifelink-ueot grants, no 0-power attackers, no equipment, no amass, no DFCs).

---

## 4. Parser / render contradictions, verbatim

1. **Poison invisible** (1a) -> N-105a.
2. **False life forecast vs infect attackers** (1d) -> N-105b, with the deck36 s25 repro.
3. **Core prompt asserts a false win condition.** `AIPlayerGPT.cpp:63`: *"You are playing a duel of
   Magic: The Gathering. You win by reducing the opponent's life from 20 to 0."* For this deck that
   sentence is flatly wrong, and it is the ONLY win-condition statement the pilot receives. It
   survives every prompt of every game. -> general-suggestions.md.
4. **Battlefield header read as a formatting bug.** deck152 s5 t5, verbatim: *"the board state
   lists 'Your battlefield (creatures: 1): Swamp #1; Swamp #2; Necropede...'. This is weird. It
   lists lands under 'creatures'? ... This looks like a formatting error in the prompt's
   representation of the board."* -> N-105d (legibility, low severity, clean repro).
5. **Bottom-line card text truncates mid-word.** vs146 s4: `{text: Haste -- Infect (This creature
   deals damage to creatures in form of -1/-1 counters and to players in form of poison...}` — the
   truncation lands one word before the payload. -> N-105e (cosmetic).

---

## 5. ROSTER-BALANCE FLAG (for synthesis — NOT a guide item)

**Is 6/6 pool-power?** Partly, and the mechanism is measurable.

1. **Poison ignores the pool's main defensive currency.** Three of six losers were at 21, 23, 23
   life — they had gained life ABOVE their starting total and it bought them nothing. deck105
   itself finished at <=8 life in four of six; in vs152 it was on **2 life** while the opponent sat
   on **23**, a 21-point life deficit, and won. Against this pool (deck152 Bant lifegain/midrange,
   deck116 GU ramp-to-fatties, deck146 Orzhov dungeons) the standard "stabilise, gain life,
   out-value" answer is a no-op. Every other deck on the roster competes on the life axis;
   deck105 does not.
2. **The engine actively helps it.** N-105b told all six opponents, in 12 of 12 infect blocker
   windows, that the swing was "NOT lethal" and that "taking damage while ahead is often correct" —
   including on the exact swing that killed deck36. **A record set while the opponents were being
   misinformed about lethality by the render is not a clean measurement of deck strength.**
   Recommend: fix N-105b, then re-run deck105 against the same pool before pricing it for the
   difficulty ladder. If it still goes 5-6/6, that is a construction/roster question; if it drops,
   the perfecta was substantially a render artifact.
3. **Construction note (not a guide item):** 35 lands in 65 cards (53.8% — 21 Forest, 14 Swamp).
   The deck flooded visibly (the vs146 five-land 5-card keep; four of the top eight tax spikes are
   land drops in games with 6-7 lands out and nothing to cast) and won anyway. Its curve tops at
   5 MV with only 30 spells. That is a lot of slack for a deck the pool cannot answer.

---

## 6. Guide drafted

`strategy.txt` in this directory, per wave31/strategy-writing-skill.md Step 0-bis (preserve ledger
first; attack floor demoted to a confirmatory clause; #1 slot given to the narrow leak that traces
to the near-losses, not to a manufactured archetype posture). Every card fact in it was read from
`Res/sets/primitives/mtg.txt` (`mana=` / `power=` / `toughness=` / `text=` / `auto=`) this session,
never from memory; the deck's two never-drawn cards (Hand of the Praetors, Relic Putrescence) are
written as RULE + BOTH OUTCOMES with no frequency claim (wave-31 HL6). It is a HYPOTHESIS to be
validated next corpus, not a freeze.
