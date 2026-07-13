# Deck-135 (Modern Snow) — Wave-5 game review

Corpus: one GPT-vs-GPT round-robin run, deck135 seat files in
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260713-145947/`. Record this wave: **1W-4L-1TO
(1/6), DOWN from wave-4's 5/12 (~2/6)**. Citations use opponent + `seq`/turn; game→file
map at the bottom. All six games read decision-by-decision.

Deck plan (verified against `Res/sets/primitives/borderline.txt` + `mtg.txt`): base-green
snow tempo-value. Cheap self-replacing snow creatures grind cards (Ice-Fang Coatl {G}{U}
1/1 flash flyer, ETB draw, deathtouch with 3+ OTHER snow permanents; Ohran Viper {1}{G}{G}
1/3, kills any blocker at end of combat + draws on player hit), Abominable Treefolk {2}{G}{U}
(power/toughness = snow permanents you control, trample, ETB freeze a creature) and Icehide
Golem {1} (2/2 snow body) apply the clock. Diamond Faerie is a {2}{G}{W}{U} 3/3 flyer with a
TEMPORARY (UEOT) team pump. The rest is a large non-creature value suite: 4 Arcum's Astrolabe
({S} cantrip fixer), 3 Search for Azcanta, 4 Glacial Revelation, 4 Into the North, 11
fetch/ramp lands.

## HEADLINE — the prime-suspect overshoot did NOT materialize; the leak moved upstream

The wave-4 attack floor is **obeyed and healthy**: across all six games there were **13
attacker decisions and the pilot declared at least one attacker in 13/13** — zero "no
attackers" freezes (the exact wave-4 failure) and **no board-emptying suicide loss**. The
central question "did wave-4 overcorrect passivity into aggression that loses?" —
**answer: no, not at the attack seam.** (Two minor genuine overshoots below.)

The 1/6 record is driven by a leak one step earlier than combat: **the pilot fills its main
phases with the deck's non-creature VALUE suite (fetch lands, Arcum's Astrolabe, Search for
Azcanta, Glacial Revelation, Into the North) and reaches combat with few or no real threats
deployed.** The attack floor faithfully swings whatever is on the board — usually a lone 1/1
or a Boreal Druid mana dork — so opponent life barely moves. All four losses ended with the
opponent at **18–23 life**; the timeout at 23. This is the wave-over-wave leak MOVING again as
the method predicts: blue-screw (w3) → attack-seam passivity (w4) → **deploy-a-threat-vs-
durdle-on-value (w5)**. Fixing the attack seam exposed the next bottleneck: getting a real
body onto the board in the first place.

Supporting metric — real creatures are RARELY castable, and deployment-when-offered is fine.
Across all 6 games only **17 windows** offered any real creature (Golem/Viper/Coatl/Treefolk/
Faerie) as a cast option, and the pilot **took the creature in 13 of 17** (the 4 passes are all
vs131). It does NOT refuse offered threats — it seldom gets to choose one, because it spends
the preceding turns ramping and digging. Diamond Faerie, the card Rule #2 spotlights, was
**offered 0 times** (needs G+W+U+5 mana; the deck has 2 Plains).

## OVERSHOOT AUDIT (mandatory) — each new wave-4 rule vs. the wave-5 logs

**RULE #1 "DECLARE AN ATTACKER EVERY TURN … (a) no untapped blocker (b) survives biggest
blocker (c) trades up / draws a card":**
- CORRECT — vs140 (WIN): 5 attacker decisions, all real bodies (Coatl, Treefolk); opp 24→8→0
  (seq33, 45, 58, 71). The floor closes games when a real threat exists.
- MILD OVERSHOOT (real, low-cost) — the "no untapped blocker → attack" free case sends the
  **Boreal Druid mana dork (1/1)** into combat: vs133 seq21 (T8), seq37 (T10); vs109 seq61
  (T10). Taps a mana source for 1 chip and removes a blocker while behind.
- No suicide/into-lethal attacks; the swing-back sub-rule was not violated.

**RULE #2 "A LISTED CAST IS YOURS — DIAMOND FAERIE ESPECIALLY" + the two falsity-facts:**
- NON-FIRING / MISPLACED — Diamond Faerie offered **0 times**; wave-4's 22/0 refusal did not
  recur. In vs110 the pilot correctly declined it for lacking WHITE mana ({2}{G}{U}{W}, only
  {U} available — seq53): a REAL affordability constraint, not the "I have no mana" delusion.
  The load-bearing part of Rule #2 is the general "a listed creature beats Cast nothing" —
  which drives the 13/17 deployment. Keep that; retire the Faerie hero-worship and the
  falsity-facts (they intercepted nothing this corpus).

**MANA section "ONE Island is ENOUGH, then fetch Forest":**
- PARTIALLY OBEYED — the pilot CITES the rule (vs133 seq4: "one Island is enough for blue, then
  Forests") and sometimes takes Forest. The wave-4 Island-hunt durdle is reduced. It is no
  longer the lever; the broader value-durdle and the fetch re-offer engine loop now dominate.

**DEFENDER role-check ("attacked twice by T3 / life ≤14 by T5 → deploy Golem+Druid as
blockers"):**
- TOO WEAK — vs110 (affinity), vs109 (RB), vs44 (UB evasion) all raced deck135 and it kept
  spending turns on fetch/Astrolabe/Search instead of stabilizing. Blocking itself works when
  asked (Golem blocks Memnite vs110 seq7; blocks vs109 seq10/52; vs131 seq42/54) — but it never
  BUILDS a defensive board because creatures trade off one at a time while it durdles. The
  role-check must bite at the CAST seam (deploy a body before the dig), not only at blocking.

## PER-GAME NARRATIVES

### vs140 — WIN, T14, 15 v 0 (the mirror)
Template win: Ice-Fang Coatl (seq15), Abominable Treefolk (seq31, a 6/6 off the snow count),
Icehide Golems (seq56, 69), attacked five times (seq33, 45, 58, 71), opp 24→15→8→0. Same deck,
same guide — the flipped variable is that real bodies hit the board and kept attacking. Still
took 14 turns/73 decisions; slow to kill.

### vs44 — LOSS, T18, **-11 v 20** (opponent never took a point)
Purest failure-to-close. Cast Coatl (seq4, T3) and a 6/6 Treefolk (seq39, T11) but had exactly
ONE attacker decision all game (seq8, T5, lone Coatl); opp finished at 20. Treefolk cast T11
never connected (answered — UB faeries has removal + evasion). 54/81 ask+priority decisions were
pass/nothing; the rest fetch/Astrolabe/Glacial/Into-the-North. Chipped out by evasive 1/1
faeries. BAD PLAY (durdle-over-deploy) + MATCHUP/VARIANCE.

### vs110 — LOSS, T15, -5 v 18 (raced by affinity, wrong role)
deck135 is the DEFENDER and played durdle-value. Opp put Cranial Plating on a **7/1 Memnite**
(seq45–48); the pilot at 19 had only a 1/1 Coatl, took 7 to 12. Diamond Faerie sat in hand from
T11, repeatedly PLANNED but never castable (no white — seq50–53). Attacked lone Coatl for 1
(seq53, 67) while being run over. BAD PLAY (never flipped to stabilize vs a T2 clock) + a
DECK-CONSTRUCTION residual (a GWU 5-drop is uncastable off a 2-Plains base).

### vs109 — LOSS, T11, -2 v 20 (ground out by RB aggro; opp at 20)
Defended better early — blocked Legion Loyalist (seq10) and Rakdos Cackler (seq52) with Icehide
Golem — but opp finished at 20; deck135 dealt ~no damage. Attacked Golem into the RB board twice
(seq16, 29) and Boreal Druid at T10 (seq61, ~0 value), then bled out. Force of Negation offered
4×, held 0/4 (no board wipe — correct). BAD PLAY (no clock built) + MATCHUP.

### vs131 — TIMEOUT, T16, **9 v 23 (BEHIND → effective LOSS)** — triage below.
### vs133 — LOSS, T11, -3 v 21 (opp GAINED life; durdle in pure form)
Clearest durdle. T0–T8 the entire stream is fetch Island (Prismatic Vista seq2–4 across three
phases; Flooded Strand seq10–15; Misty Rainforest seq20), cast **Astrolabe twice** (seq6, 7),
Search for Azcanta (seq17), Glacial Revelation (seq18). By T8 its only creature is the Boreal
Druid mana dork (seq21 — attacked with it). Ohran Viper doesn't land until seq35 (T10) at 7
life. Opp (RW) ended at 21. BAD PLAY — the headline leak in pure form.

## TIMEOUT TRIAGE — vs131 (1783977943)
**Genuine durdle-stall + heavy engine inflation; scores as a LOSS.** Final 9 v 23 — behind, so
not "winning-but-slow." 105 decisions to reach only T16; total model latency ~1418s (median
9.8s, 9 decisions >20s, max 79s) — latency alone is under the 2400s cap, so the cap was hit by
decision COUNT. Inflated by the fetch re-offer loop and durdle: the pilot cast Golem ×2, Viper,
Druid, but creatures kept trading off as blockers or were just-cast/summoning-sick, so it had no
untapped non-sick body at its own combat → **zero attacker asks in 105 decisions**. Opp deck131
runs Elixir of Immortality + Prism Ring (lifegain), climbing to 23. The 4 creature-passes noted
above are all here: seq21 (Astrolabe over Golem), seq33 (Cast-nothing over Golem), seq100
(Cast-nothing over Golem).

## WAVE-4 LEDGER STATUS
FIXED / IMPROVED:
- Attack seam fires 100% (13/13) — wave-4's #1 fix landed.
- Diamond Faerie 22/0 refusal did not recur (offered 0).
- Plan-echo splice (wave-4 E4) appears FIXED — 0 stray-fragment reply heads across vs131/vs140/
  vs110. Confirm before deleting the ledger item.
- Reply parser clean — 0 head-integer-vs-choice desyncs.
PERSISTING:
- Fetch re-offer loop (wave-4 E1 / wave-3 F2) — WORSE than "occasional." vs133: Prismatic Vista
  seq2→3→4 (3 calls/crack); Flooded Strand seq10→11→12→13→15 (**5 calls**, incl. two at the
  Attackers step). Dominant engine contributor to the vs131 timeout. Engine item, not guide.
- Durdle rationale — value/ramp/dig over deploying a body (the headline).
REGRESSED / NEW:
- Record 2/6 → 1/6: partly variance (the one win shows the deck functions), partly the
  deploy-vs-value leak the wave-4 revision did not target and arguably fed by stripping the
  wave-3 setup scaffolding without a "creatures before value" ordering.

## MODEL-QUALITY RESIDUALS
None new this corpus — no degenerate blocker replies, no illegal assignments, no numeric-offset
menu traps (Diamond Faerie's X-pump never reached; deck140 Lightmine not drawn into a solo-attack
decision here).

## GAME → FILE MAP  (run = matchups-20260713-145947)
| epoch | opp | outcome | turns | final (me v opp) |
|-------|-----|---------|-------|------------------|
| 1783972790 | vs44  | LOSS    | 18 | -11 v 20 |
| 1783973501 | vs140 | WIN     | 14 |  15 v 0  |
| 1783975194 | vs110 | LOSS    | 15 |  -5 v 18 |
| 1783975954 | vs109 | LOSS    | 11 |  -2 v 20 |
| 1783977943 | vs131 | TIMEOUT | 16 |   9 v 23 (behind = loss) |
| 1783979342 | vs133 | LOSS    | 11 |  -3 v 21 |
