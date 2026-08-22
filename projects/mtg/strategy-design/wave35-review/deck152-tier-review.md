# deck152 (Bant Human Midrange) — Wave-35 A/B Strategy Review

**Seat**: deck152 | **Guide**: `projects/mtg/bin/Res/ai/baka/deck152_strategy.txt`
**Arm A (35B-A3B)**: `matchups-20260820-192210` | **Arm B (122B-A10B)**: `matchups-20260821-033000`
Rubric: win rate irrelevant; score guide-adherence per decision, and where the model diverges, whether diverging was correct AT THAT MOMENT.

## Guide core lines (extracted)
1. Trust the Mana-available line / cast list; never re-derive mana; a listed "Cast" IS payable.
2. Develop a creature every turn, attack with everything that profits; render lists only legal attackers.
3. Deploy floor: on own main, if a creature is offered, cast one — only exceptions (a) holding Fateful Absence as the sole offer, (b) lethal already on board.
4. Blocks: only from each blocker's "may block" list; "BLOCKS: none" unless a block trades up, saves your life, or stops poison.
5. Fateful Absence: save for their best creature/planeswalker — the ONE hold-with-mana-up card. Brutal Cathar: never bad-block (exile returns).
6. Intrepid counters: pay with a board + spare mana; no spare mana → take "don't add".
7. Ranger Class: level with SPARE mana only. Never reason about Coven.
8. Day/Night: read the battlefield line, never predict a face. Never spend a decision on "Flip Side".
9. Mulligan floor: if the coverage line names one card, KEEP.
10. Matchup posture: vs infect, block infect attackers even at creature cost, kill [infect] on sight, poison is the clock; vs lifegain kill the lifegainer on sight; vs fliers FA is the only answer.

---

## ARM A (35B-A3B) — per-game findings

### A vs deck105 (infect) — won 36/-1
Model-facing decisions all guide-adherent. seq1 keep (floor). seq6 Ranger Class t4; seq12-13 FA on **Blight Mamba [infect]** — exactly the "kill infect on sight, spend FA early" line. seq10-11/18-19 Intrepid +1 counter with spare mana (board out). seq16/22/27 escalating attacks (one body → two → team). seq23-24 leveled Ranger Class L2+L3 at t10 with spare mana post-development. No divergences.

### A vs deck146 — won 24/-6
seq7 (t4) BLOCKS: none vs deathtouch Triumphant Adventurer — adherent ("even-or-better trade" fails vs deathtouch; trace states the 2-for-1 correctly). seq14/18 "don't add any counter" — trace verified only {G} spare / no spare mana → adherent per lord rule. seq20-22 Brutal Cathar exiling deathtoucher, seq25 Sigarda on curve, seq24/27 blocks that trade up. **Divergence (trivial)**: seq28 (t15) "Hold Barkchannel Pathway" vs rule-2 "play a land" — opp at 1 life with lethal on board; land changes nothing. Justified audible. seq29 "Cast nothing" = guide exception (b) verbatim (trace: "we are already winning this turn").

### A vs deck116 (Kitchen Finks lifegain) — won 42/-12
seq10-11 FA on Kitchen Finks = vs-LIFEGAIN posture (kill the lifegainer on sight); seq17-18 second FA on the persisted 2/1 Finks — same posture line, and the model was attacking on the ground here (unlike B's copy of this game, its attackers Intrepid/Aspirant genuinely wanted the blocker gone). Deploy-floor perfect: creature every turn t4-t12. seq27 full team attack, coven never reasoned about. No divergences.

### A vs deck36 (artifacts) — won 2/-10
Long grind from 2 life; zero comprehension failures. seq15-16 FA on **Master of Etherium (8/8)** — their best creature, adherent. seq18/28/33/38 blocks all from listed trades. seq23 (t9) BLOCKS: none at 7 life — trace prices Katilda chump vs 2 damage correctly ("saving 1 life isn't worth losing Katilda") — adherent per rule 4. Day/Night handled exactly per rule 8: model attacked with its own **Moonrage Brute** (night-face Brutal Cathar) at t14-t18 reading the battlefield line, targeting Luminarch counters onto it (seq37/43/50/51). Won a 2-life squeaker by escalating flying attacks.

### A vs deck158 (LotR orcs) — won 3/-2
seq10-11/13-14 Elite Spellbinder exile picks (Mauhur, Feed the Swarm) — guide-silent, sensible. seq16-18 Brutal Cathar exiles Olog-hai Crusher (best attacker). seq21-22 FA on **Orc army (7/7)** — best creature, adherent. seq20 (t11) BLOCKS: none vs 7/7 with 2/2s — "die for nothing" clause, trace confirms ("2/2 vs 7/7 is suicide and doesn't remove the threat"). Kept attacking at 3 life per beatdown posture and won.

### A vs deck139 (ramp/mutate) — won 16/-4
**Divergence**: seq13-14 (t7) cast FA on own main at **Dryad of the Ilysian Grove** — vs rule 5 (hold FA; save for best creature). Verdict: **justified audible** — Dryad was their best permanent on board (ramp engine), model held a second copy, and the trace argues from those facts ("generates extra lands... I have 2 Fateful Absences in hand"). Second copy seq26-27 went to **Everquill Phoenix (4/4 flying)** — the vs-FLIERS line says FA is the only answer: adherent. seq15 leveled Ranger Class with spare mana after developing. Clean otherwise.

**Arm A totals**: ~168 scored decisions (excl. gamestart/gameend). Divergences: **2, both justified audibles** (one trivial). Comprehension failures: **0**. Notably disciplined: every "Cast nothing"/"no blockers"/"don't add counter" checked against the guide's own exception clauses in-trace.

---

## ARM B (122B-A10B) — per-game findings

### B vs deck36 — won 21/-2
Fully adherent: seq11 Intrepid blocks Salvage Slasher (listed trade), seq13-14 FA on **Master of Etherium (8/8)** (best creature), creature every turn, team attacks, Sigarda on curve. No divergences.

### B vs deck146 — won 14/-4
Adherent throughout: seq10-12 Brutal Cathar exile, seq15-16 FA on Nadaar (their best), deploy every turn, seq26 "Cast nothing" at t13 with opp at 2 = exception (b) (trace: Sigarda would be summoning-sick, attack wins). No divergences.

### B vs deck116 — **LOST -5/26** (the arm's key game)
- seq11-12 (t6) FA #1 on Kitchen Finks (3/2): defensible under vs-LIFEGAIN posture — counted adherent-leaning audible.
- **seq13-14 (t6) FA #2 on the persisted 2/1 Finks: COMPREHENSION FAILURE.** Trace: *"If I don't cast it, Finks stays. It's a 2/1 blocker... Removing the blocker is worth the Clue. I need to get through next turn."* — the model's ONLY attacker was **Elite Spellbinder, a flyer**; a ground 2/1 could never block it. It burned the deck's last removal on a creature that was not blocking anything, for a Clue's cost. Sheoldred (their actual best creature, named in its own later plans as the threat needing answers, seq21 trace) landed with zero FA left; model was green-screwed ({u}{w} only through t13) and died at t15.
- seq21 (t13) Sigarda blocks Kitchen Finks — listed block, saves life, adherent. Deploy-floor gaps at t8/t10 were engine-side (no castable creature offered on {u}{w}) — not chargeable.

### B vs deck105 (infect) — **LOST by poison** (gameend t15, won=False at 20 life, opp 8)
- seq7-8 FA on Plague Stinger [infect, flying], seq10-11 FA on Contagious Nim [infect] — "kill infect on sight" — adherent, both copies spent early per the guide.
- seq15 Tracker blocks Blight Mamba; seq18 double-block Necropede — adherent infect blocking.
- **seq21 (t13) BLOCKS: none vs Ichorclaw Myr at 2-of-10 poison: DIVERGENCE, graded FAILURE (poor audible).** The guide's named rule: *"block infect attackers even when the block loses you the creature and even when you are at a healthy life total."* The trace did read the poison lines and computed the blocked-Myr trade (4/4 vs 4/3 Tracker, both die), then chose to race the opponent's 8 life — the exact "life-race" framing the posture section exists to forbid. Worse, part of the stated logic is backwards plan-inertia: *"If I block and lose Tracker, the plan 'attack with Wolf and Tracker' becomes impossible. So I must NOT block to keep the plan valid."* A plan is not a reason to keep a plan valid. Poison went 4→10 across t14-t15 (Hand of the Praetors lord online) and the model lost at 20 life without ever attacking for the race it chose. Nuance: at 2-of-10 with opp at 8 the race was arguable, so this is the audible-quality boundary case — but it argued from plan preservation, not from a counter-budget, and the guide's bright line covered exactly this temptation.
- seq23-24 Elite Spellbinder exile: **prompt defect, not model failure** (see docket below).

### B vs deck158 — won 20/0
Adherent: Brutal Cathar t6, its Moonrage Brute night-face attacked t8/t12 (Day/Night read from the line), Sigarda t8, Intrepid +1 counter with spare mana t10, FA implicitly held. No divergences. (seq2 t2 sole land play — t0 handless land drop not offered; not chargeable.)

### B vs deck139 (ramp) — won 24/-3
- seq11 (t7) "no attackers": adherent — board was Intrepid 3/1 + Katilda into untapped 0/3-reach + 2/4 walls; every attack dies for nothing (rule-2 exception, trace works the math).
- seq16-17 FA #1 on **Nissa, Who Shakes the World** — planeswalker, best target: adherent.
- **seq21-22 (t11) FA #2 on Arboreal Grazer (0/3): DIVERGENCE, JUSTIFIED AUDIBLE.** Looks absurd (FA on a 0/3 wall vs two 2/4 Dryads) but the trace has the real reason: *"If I remove Grazer, Sigarda is unblockable — only Grazer has Reach."* Clearing the sole reach blocker for a 4/4 flying-trample lord in a beatdown deck is correct at that moment; the win followed through Sigarda. (seq22's target confirmation leaned on "the Plan said so" — the actual judgment lived in seq21.)
- seq31-32 Katilda mass-counter activations at 3+ creatures with spare mana: adherent to the lord section. seq23 Ranger level with spare mana: adherent.

**Arm B totals**: ~145 scored decisions. Divergences: **3** — 1 justified audible (FA-on-Grazer), **2 failures** (FA #2 on Finks = misread that a ground blocker mattered to a flying attacker; infect no-block vs the named posture rule, argued partly from plan-inertia). Both failures occurred in the arm's two losses.

---

## Divergence table

| Arm | Game | seq/turn | Choice | Guide line diverged from | Verdict |
|---|---|---|---|---|---|
| A | vs146 | seq28 t15 | Hold land | R2 "play a land" | Justified (lethal on board; zero impact) |
| A | vs139 | seq13 t7 | FA main-phase on Dryad | R5 hold FA / best creature | Justified audible (their best permanent; 2nd copy in hand) |
| B | vs116 | seq13 t6 | FA #2 on persisted 2/1 Finks | R5 save for best creature | **Failure** — "removes the blocker" but only attacker flew; last removal gone, Sheoldred unanswered, lost |
| B | vs105 | seq21 t13 | No block vs Ichorclaw Myr | Infect posture: block even at creature cost | **Failure (poor audible)** — chose the forbidden life-race framing + "keep the plan valid" logic; lost by poison at 20 life |
| B | vs139 | seq21 t11 | FA #2 on 0/3 Grazer | R5 best creature | Justified audible — sole REACH blocker; unlocked Sigarda for the win |

Mulligans: 12/12 keeps, coverage floor never violated in either arm; no forbidden mono-color mulligan reasoning observed.

## Prompt-caused defects (wave-36 docket)
1. **Elite Spellbinder exile-target list offers only LANDS** — B-vs-105 seq24 (t14): opponent's hand was 2 Forests; the target list was `1. Forest [land]` / `2. Forest [land]` with no decline option. Oracle: "You may exile a **nonland** card from it." When the revealed hand has no nonland card the render should offer "exile nothing" (or auto-resolve), never list lands as targets. The model was forced to pick a land. (Contrast A-vs-158 seq11/14 where nonland options rendered fine.) Model reactions were correct given the render — prompt-caused, not a model failure.
2. Minor, same decision: the model noted the prompt simultaneously implied the ability "is already on the stack" while the composed text read as if it might not have triggered — a small wording contradiction it worked around.
3. Observation (not a defect): B-vs-105 ended at t15 with no blockers decision surfaced on the final poison attack; if blockers existed and untapped, a decision record is missing — worth a one-off translog check.

## Per-arm verdicts
**Arm A (35B-A3B)**: Adherence ≈ 99% (166/168 scored decisions; 2 divergences, both justified). 0 comprehension failures. Executed every named rule that came up — infect-on-sight, day/night face reading (attacked with its own Moonrage Brute correctly), counter-payment mana gating, lethal exceptions, blocking clauses — and its audibles were argued from real board facts. 6-0 incidentally.

**Arm B (122B-A10B)**: Adherence ≈ 98% (142/145; 3 divergences: 1 excellent audible, 2 failures). Traces are ~3x longer (avg ~17k vs ~5.5k chars) and show a recurring **plan-inertia motif**: stale PLAN text used as a reason in itself ("must NOT block to keep the plan valid"; "the Plan specifically said Arboreal Grazer... deviating requires justification"). Its best divergence (Grazer/reach) was sharper than anything Arm A produced; its two failures were both consequential and both violated lines the guide states explicitly. 4-2 incidentally.

**A/B answer for this seat**: **YES — the 35B is sufficient to replace the 122B on deck152.** On the owner's rubric (adherence + divergence quality, not wins) the 35B is at least the 122B's equal here: equal-or-better adherence, zero comprehension failures vs two, and its divergences were sound. The 122B's one edge (the Grazer/reach insight) does not offset misreading a flyer-vs-ground-blocker interaction and overriding the guide's explicit infect-block rule with plan-inertia reasoning.
