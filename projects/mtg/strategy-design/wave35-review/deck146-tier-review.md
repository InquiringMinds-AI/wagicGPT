# deck146 "Orzhov Dungeons" — 35B-A3B vs 122B-A10B tier review (wave 35)

Rubric (owner, binding): win rate is irrelevant; score (a) how well each model follows the
provided strategy, and (b) where it diverges, whether diverging was a good choice at that
moment. Judged from the guide (`deck146_strategy.txt`) against every decision in the seat's
translogs; reasoning traces read for every divergence and pivotal turn.

Arms:
- **A = 35B-A3B**: matchups-20260820-192210 (6 games) + matchups-20260821-022400 (1 rerun) — 7 games, ~237 scoreable decisions.
- **B = 122B-A10B**: matchups-20260821-033000 — 6 games, ~200 scoreable decisions.

Guide core lines used as the scoring frame: engine-count-driven mulligans (hard floor 5);
removal on their best threat ON TIME with Verse's target list as the only truth; Kaya lands →
immediate -3 when behind, +1 only when nothing worth exiling; Acererak only in quiet turns
(he always bounces); ALWAYS pick Lost Mine by name; room branches = body/drain over scry;
hold blockers at low life; planeswalkers act every turn; never cast Agadeem's Awakening.

## Per-game findings — Arm A (35B)

**A1 vs deck152 (loss, t15).** Highly adherent throughout. Verse → Ranger Class over Wolf
(seq6-7, t4: killed the token engine — correct); second Verse fired next window (seq13).
Kaya cast + immediate -3 on Intrepid Adversary while behind (seq19-20, t8) — the guide's
exact line. seq24 t10 Acererak cast checked against the menu: options were only Acererak /
Pelakka / nothing (no removal in hand, opponent's board 2 creatures incl. a tapped Wolf) —
quiet-turn cast, adherent. seq33 t12 Kaya +1 (not -3) checked: Kaya at 2 loyalty, -3 not on
the menu; +1 on Silencer was the only meaningful use — adherent ("use them every turn").
seq41 t14 Kaya -3 exiled Sigarda on sight. Loss came from deck152's curve, not misplay.

**A2 vs deck36 (truncated at t10 — no gameend record; harness note).** Mulligan exemplary:
shipped a 7 the engine counted as "3 lands, {W} 3" with every spell needing {B} (seq1) —
the guide's named true-mulligan; kept the 6 with 4 lands {W}2/{B}3 (seq2). One flagged
divergence: **seq18 t8, alpha attack with all 4 creatures at 5 life** vs an artifact-aggro
board. Guide says hold blockers at low life. Mitigating board fact: the opponent's real
damage (Glaze Fiend, Ornithopter) FLIES — the ground deathtouchers could not block it
anyway; the trace argues pressure+venture but never states the can't-block-flyers fact.
Verdict: **defensible audible, weakly argued** (outcome-neutral; damage was coming through
regardless). Everything else adherent (Soul Shatter t8, Barrowin, Lost Mine, goblin/dark-pool picks).

**A3 vs deck158 (win, t14).** Near-perfect guide execution: Silencer naming Orcish
Bowmasters t2; Verse fired every turn a target existed (March from the Black Gate t4, Orc
army t6, Bowmasters t8); Nadaar → Lost Mine; Soul Shatter t10; Pelakka t12; closed by
attacking. One **fallback**: seq35 t14 priority reply unparsed (choice -1, heuristic
answered) — parse miss, not a decision failure.

**A4 vs deck105 (win, t20, vs infect).** Adherent: Silencer named Skithiryx t2 (their bomb);
Verse on-curve at every infect body (t6/t8/t9); Soul Shatter t10; Hive animated only on
flood turns and attacked; Acererak cast twice on genuinely quiet turns (t14, t18 — ahead
20/8, nothing else castable); Nadaar anthem online, closed with the venture team.

**A5 vs deck139 (win, t12).** Adherent: Verse ×3 on the ramp/mutate enablers (Pollywog
Symbiote, Arboreal Grazers) — on-time removal against that deck's engine; Lolth cast t10 +
immediate -3 for Spiders (the guide's exact first-use line); wide lethal attack t12.

**A6 vs deck116 (win, t12).** Adherent and fast: ventured early, animated Hive on spare
mana and attacked with it alongside Nadaar/Adventurer/Goblin; killed the Emrakul deck
before it assembled. Minor oddity seq15 t10 "Play no land" followed by playing Hive later
the same turn — net land drop still made; no cost.

**A7 vs deck36 rerun (win, t16).** Adherent: correct keep; **Silverquill Command return
creature + opponent-sacrifices** at t6 (seq9) — the guide's named two-for-one vs aggro,
executed from an empty board; Lolth t8 + -3 Spiders, then 0-draws on quiet turns (guide:
take the draw most turns); Verse t14 read a target list that included its OWN Lolth/
Spiders/Goblin and correctly hit the opponent's Salvage Slasher (seq40-41); Nadaar,
dungeon completion, wide attacks to the kill. Note in seq9's trace: the narration had
already said "cast Silverquill Command" before the mode ask arrived, and the model
paused over it ("maybe a template error") — see prompt-defect list.

## Per-game findings — Arm B (122B)

**B1 vs deck36 (loss, t9).** Short game, no misplay: same correct color-screw mulligan
shape as A2 (7 with {W}-only sources shipped, seq1; kept a guide-legal 2-land 6 with
W+B and two 2-drops, seq2; bottomed Kaya). Deployed Adventurer/Silencer, then correctly
declared **no attackers at 6 life** twice (seq12-13 — the guide's hold-back line).
Died to an affinity curve-out. Adherent.

**B2 vs deck152 (loss, t13).** Mostly adherent under pressure: Verse → Ranger Class t4;
double Verse t10 into the 4/4 Wolf + Brutal Cathar the turn it had mana. Two flagged:
- seq26-27 t12, **Silverquill Command "you draw" mode at 3 life** (self-draw costs 1 →
  went to 2). Guide says aim the draw at the opponent. Trace shows a real dig argument
  (stuck on {W}-only mana, Verse dead in hand, needed outs). **Defensible audible, risky.**
- seq29 t13 blockers, **comprehension failure**: the surface listed exactly "B1. Silverquill
  Silencer — may block A1" (Sigarda has flying), and the model answered `BLOCKS: B1:A2` —
  assigning its blocker to the flyer it was told it could not block. Fallback
  `all_assignments_illegal` → no block. Outcome-moot (2 life vs 8 on board: dead under any
  legal assignment — the surface's "block enough to survive" was unfollowable), but the
  model ignored an explicit legality list.

**B3 vs deck139 (loss, t31 — lost from 21/8 ahead).** The decisive game for the A/B question.
- Adherent early: Verse Pollywog t4; two more Verses t6 on the Grazers (3 copies held, ramp
  deck — fine); Kaya t8 with **+1 correctly** (checked: -3's only listed targets were its
  own permanents; opponent controlled zero creatures — the guide's "+1 when nothing worth
  exiling"); Kaya -3 exiled Brokkos t12 and Migratory Greathorn t16 on sight.
- seq35 t12, Silverquill Command target ask — **prompt-caused defect, not scored against
  the model** (see defect list): the ask fuses both chosen modes into one label
  ("creature gains 3/3 and sacrifice creature") and lists own creatures AND the opponent's
  Brokkos with no mode attribution. The model's plan was "force sacrifice of Brokkos +
  pump Adventurer"; it picked Brokkos while explicitly noting it could not tell which
  mode the target fed ("the spell requires two targets... I must answer... I will pick Brokkos").
- t12→t28 stall: **no attackers asks appear in the translog for nine of the model's turns**
  while it had 2-5 untapped, non-sick creatures and repeatedly wrote plans that said
  "attack with..." — the combat decisions were never offered. Not a model divergence;
  engine/harness item (see defect list). Its "Cast nothing" answers in that window were
  correct (hand was one dead Agadeem's; seq55 trace even re-derives "graveyard has no
  creatures → Agadeem's does nothing → don't cast" — guide-adherent).
- seq74-75 t28, **the headline comprehension failure**: with 10 mana it cast Agadeem's
  Awakening "X=5 to return Kaya the Inexorable from graveyard" — (i) a direct violation of
  a named guide do-not ("Do not cast Agadeem's Awakening... nothing happens — the card
  stays in your hand and the turn is gone"), and (ii) a rules misread: Agadeem's returns
  CREATURE cards; Kaya is a planeswalker. The trace re-checks the card text twice and still
  concludes "I only have Kaya (MV 5) in graveyard... So I target Kaya." Confirmed outcome:
  turn burned, all mana tapped, and at t31 the card is STILL in hand — exactly what the
  guide predicted. Illuna+Snapdax arrived the next turn and it died from 15/8.
- Even discounting the no-combat stall as engine-caused, seq74 is the one place in either
  arm where a model argued itself past an explicit named prohibition.

**B4 vs deck105 (win, t12, vs infect).** Near-perfect: correct mulligan (same {W}-only
screw shape), bottomed Acererak; Nadaar t4 → Lost Mine; Verse the flying infect threat
t6; Soul Shatter t8; storeroom counter onto Nadaar; two Silencers naming the pump
engines (Blackcleave Goblin, Hand of the Praetors); wide lethal t12. Fully adherent.

**B5 vs deck116 (loss, t15, Emrakul via Quicksilver Amulet).** Adherent in a near-unwinnable
spot: held 3 dead Verses (colorless board — the guide's exact "Verse is dead, win with
bodies" case), raced with Silencers/Nadaar, answered **Annihilator 6 by sacrificing six
LANDS and preserving both creatures** (seq26-32 — trace states the plan explicitly), then
all-in attacked at 3 life after verifying Emrakul was tapped and the attack was the only
non-losing line (seq33). **Justified audible** on the low-life-attack rule — correct
race math, correctly argued. Lost to the second Eldrazi.

**B6 vs deck158 (loss, t19).** Adherent: Silencer names (Feed the Swarm, Mordor Muster);
Verse March t4 / Orc army t6 / Orc army t12; Command pump+sacrifice t8; Soul Shatter t10
and t18; Acererak cast three times, each checked: only card in hand on spare-mana turns
(t12 8 mana / t16 / t18 main2) — the guide's "flooding with nothing to do" case; dark
pool picked over scry. Chump-blocked correctly with Silencer at 12 and 8 life. Lost the
race by 1-2 turns; one trace hallucinated that Fungi Cavern would "kill" Orc Army
(-4/-0 only reduces power) — plan-level fuzz, did not change any choice.

## Divergence table

| Arm | Game/seq (turn) | Choice | Verdict |
|---|---|---|---|
| A | A2 seq18 (t8) | Alpha attack, all 4 creatures, at 5 life | Defensible audible (opp damage was flying; blockers useless), weakly argued |
| A | A1 seq39/42 (t14) | Attacked/recast at 1 life facing lethal | Guide-silent, dead-anyway noise |
| B | B2 seq27 (t12) | Command self-draw at 3 life (guide: aim at opponent) | Defensible audible (mana-screw dig), risky |
| B | B2 seq29 (t13) | `BLOCKS: B1:A2` into a flyer the surface said B1 could not block | **Comprehension failure** (outcome-moot; fallback saved legality) |
| B | B3 seq74-75 (t28) | Cast Agadeem's Awakening, "return Kaya (planeswalker) with X=5" | **Comprehension failure + named guide violation**; turn burned, card confirmed still in hand at t31; lost from ahead |
| B | B3 seq35 (t12) | Command fused-mode target → picked opponent's Brokkos | **Prompt-caused** — not scored against the model |

Everything else in both arms scored guide-adherent or guide-silent-reasonable. Both arms:
Lost Mine of Phandelver picked by name at every three-dungeon list (11/11 A, 9/9 B);
every room branch took the body/drain room; every mulligan decided from the engine counts.

## Prompt/engine defect list (wave-36 docket)

1. **Silverquill Command fused-mode target ask** (B3 seq35): when two modes are chosen, the
   target ask labels itself with BOTH mode texts as one string and lists the union of legal
   targets (own creatures + opponent's) with no indication which mode the target feeds. A
   correct answer is not expressible; the 122B explicitly flagged the ambiguity in-trace.
   Related: mode-ask ordering — narration says "You cast Silverquill Command" before the
   mode/target asks arrive; A7 seq9's trace called it "maybe a template error".
2. **No attackers asks for nine consecutive turns** (B3 t12-t28) while the seat had multiple
   untapped, non-sick creatures; the model's plans said "attack" throughout. Engine-side
   combat-issuance check needed (pendingCombatDecision path in that game state).
3. **Agadeem's Awakening offered as a castable option that resolves to nothing** for this
   deck (B3 seq74; guide already routes around it). Trust-doctrine violation shape: the
   surface offers a no-op. Either filter the cast when no legal returns exist or make the
   option line carry the creature-cards-only constraint the way Verse's target list does.
4. **Blockers surface said "block enough to survive" in a lethal-either-way state** (B2
   seq29): the instruction was unfollowable; consider softening to the survival framing
   only when a surviving assignment exists.
5. Minor: A3 seq35 priority reply unparsed (35B, one occurrence); A2 game log ends at t10
   with no gameend record (harness truncation — the game never concluded).

## Verdicts

**Arm A (35B-A3B): adherence ~99% of scoreable decisions; 0 comprehension failures; 2
borderline divergences, both defensible; 1 parse fallback.** Executed every named guide
line that came up, including the subtle ones (engine-count mulligans, Kaya -3-first,
Acererak quiet-turn-only, Command two-for-one, own-permanent Verse lists).

**Arm B (122B-A10B): adherence ~97-98%; 2 comprehension failures (one severe: the
Agadeem's cast against a named prohibition, which converted a 21/8 winning position into
a loss), 2 defensible audibles (one excellent: the B5 Annihilator/all-in line), 1
prompt-caused non-score.** Its reasoning on hard board math is deeper than A's, but it is
also the only arm that reasoned itself past an explicit guide do-not.

**A/B answer for this seat: YES — the 35B is sufficient to replace the 122B on deck146.**
Under the owner's rubric the 35B follows the guide at least as faithfully as the 122B and
committed no comprehension failures across 7 games, while the 122B's one severe failure
was precisely the kind the guide exists to prevent. The 122B's edge (richer combat math,
B5's justified all-in) does not outweigh that on this seat. Caveats: the comparison rides
different opponents' draws (B faced harsher curve-outs); and 122B game B3 was contaminated
by the no-combat engine anomaly — recommend re-running that matchup after docket item 2
is investigated if the seat decision is close fleet-wide.
