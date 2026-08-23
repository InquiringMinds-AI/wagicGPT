# Wave-36 validation — deck152 (Bant Humans) + deck105 (BG Infect)

**Corpus**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260822-155508` (FP8 pilot, binary
bb3cc9b12, revised wave-35 guides — deployed 14:29, binary 15:34, run 15:55; no `~/.Wagic`
prompt overrides). Rubric: owner ruling #1 — adherence + divergence quality; win table
context only (152: 3-3, 105: 2-4 with both wins by poison).

**Fallbacks: 0 / 0** across all 12 seat games (wave-35 had 2 at the 152 seat). No
parse_note records at either seat.

## deck152 — the two new teaches, one each way

### Trade-trust (wave-35 class: 3 blocks into printed-death lines) → **1, not 0**

12 blockers asks, `kBlockTradeTrustNote` present in **12/12** prompts. Blocks assigned
against a "(your blocker dies, attacker lives)" line: 3 —

- vs36 s23 (t9, life 1, LETHAL) — sanctioned (block-enough-to-survive; no survival line
  existed: 4 damage, one 1/1 blocker; "PLAN: Game lost." was accurate).
- vs105 s17 (t7) — sanctioned (rule 4's poison exception: Wolf chumped Ichorclaw to stop
  a poison counter; the B-line's new "includes its +2/+2 when-blocked trigger" attacker-side
  annotation rendered and was consumed without confusion).
- **vs158 s8 (t5, life 20) — FAILURE, the class surviving.** B1 Wolf (2/2): A1 Bowmasters
  "(you kill it, your blocker lives)", A2 Orc army 3/3 "(your blocker dies, attacker
  lives)". The trace RESTATED both parentheticals verbatim, then five sentences later
  asserted "I lose Wolf, kill Army … blocking A2 kills the bigger threat" and answered
  B1:A2 — Wolf died, the army lived, nothing was stopped. No "template error" language;
  this is the silent-drift variant (read correctly, re-derived anyway). Same trace also
  mis-read "Mana available: 1" as covering Fateful Absence {1}{W} (PLAN-only, no
  execution impact).

The note IS working where it is read at answer time: it is quoted-and-obeyed in traces at
vs105 s10 and (other seats) 36-vs-152 s9/s19. Verdict: class rate 3→1 per corpus; the
2-corpora retirement bar for the per-guide crutches is NOT met.

**The corpus's one "template error" string** is NOT this class: deck139 seat, 139-vs-105
s21 (t6, mutate over/under ask, both creatures 6/6) — "Maybe the prompt is just a template
error" about the mutate menu's stakes, a 139-lane confusion, no trade line involved.

### RETURN-SWING check → **0/1 armed, game-losing; prediction failed**

Armed (life ≤ biggest enemy creature power) at exactly one attackers ask: **vs36 s22 (t8,
life 1, Cathodion 3/3 opposite)**. The rule's answer is ATTACK: none (the sole attacker,
Wolf 3/3, is the biggest ground creature — it stays home). The pilot attacked; the trace
checked "no other options / life 1 vs 20" and never ran the swing-back. Next combat (s23)
was LETHAL 4 with only the 1/1 Aspirant left to block — dead at t9. Wolf home + Aspirant
blocks absorb all 4 and survive. This is the exact wave-35 vs158-s22 shape the teach was
written for, un-recalled at the moment it applied.

Calibration near-miss (not a violation): vs158 s36 (t14, life 6, Orc army 5/5 tapped) —
6 > 5, not armed by the letter; the army untapped, March pumped it to 6/6, +Crebain = 7,
dead at t15 -1. The teach has no growth term (deck105's step 2 does). Same game, s37: the
pilot activated Lair of the Hydra at the opponent's upkeep while Lair was **[tapped]** —
a tapped 1/1 hydra can't block; 2 mana for nothing (minor divergence). The subsequent
missing blockers ask at t15 is therefore CORRECT engine behavior (no legal blocker:
Wolf tapped from attacking, hydra tapped) — NOT a recurrence of the lane-B skip; the
lane-A combattrace instrument fired as designed (`[combattrace] t15 ph7 …
suppressed by unresolved-stack` in `game-152v158-1787432147.stderr`).

### Plan-is-a-memo → clean

Zero stale-PLAN-as-reason divergences. Plan usage is memo-and-verify throughout ("my plan
says X … board check … still right/adjust") across all six games; vs158 s11 quotes the new
"MEMO TO YOURSELF, NOT A RULE" line directly. The arm-B inertia motif did not appear.

### Intrepid Adversary valor menus → answers right, the new header NEVER rendered

4 valor menus (vs146 s9, s47; vs105 s35; vs158 s5). `payRepeatModeNote` text ("over-ask
never fails…") present in **0/4** — **NEW DEFECT (lane-A item 4 dead on the live path)**:
`isAddNCountersOption` matches `"Add "` (capitalized); the live menu prints lowercase
"add 1 counter"… (and no "Choose one mode" header appears on these asks — verify the menu
rides the seam the note was attached to). PARSETEST's fixture used capitalized options, so
the gate tested a casing the live surface never emits. Despite the missing note, all 4
answers were partial-pay-correct by self-derivation: 0 mana → "don't add any counter"
(s5, s47-with-1W), 1 spendable → don't add (s9), 4 mana → "add 2 counters" = exactly two
{1}{W} payments (s35). No trace reasoned "large N is unaffordable-and-therefore-illegal".

### Everything else at 152

Deploy floor held; every "Cast nothing" pick was a sanctioned FA-hold (the only cast
offered was Fateful Absence: vs116 s12 own-targets-only, vs139 s5, vs158 s21). Kitchen
Finks removal s19-vs116 (the lane-A hybrid fix's card, on the OTHER side) targeted
cleanly. Zero Flip Side decisions spent (never-flip held; the land-flip recipe was never
needed — no flip-loops). Mulligans: all keeps, guide-shaped. Katilda-vs-infect precedence
is UNSTATED in the guide: at vs105 s10 (poison 2/10, Katilda the only listed blocker on a
dies-line) the pilot weighed the infect block-always rule against "Katilda is a mana
source, not a trading piece" and declined — defensible, but the guide leaves the collision
to the pilot; one sentence would close it.

## deck105 — everything validated

### Numeric ≤13→BLOCK rule: **17/17 exact** (intact at all boundaries)

All 17 blockers asks executed the steps literally, walked in-trace: ≥14 → none at 18/17/
17/15/14/18/18 (vs158 s5; vs36 s8, s12, s18; vs139 s5, s9 — s9 explicitly ran step 2's
counters check before answering at exactly 14); boundary 13 → BLOCK NOW (vs146 s15, chose
the "both die" pairing per the printed-outcome preference); ≤13 → block with smallest
(vs139 s14; vs152 s16, s23); LETHAL → block enough (vs158 s12; vs139 s20 double-block,
survived at 2; vs139 s25 / vs146 s23 — no survival line existed, any answer lost). Zero
blocks the steps forbade, zero skips the steps demanded.

### Generic-hint scoping: **0 hint-cited overrides**

Every trace that read "taking damage while ahead …" (5+ instances: vs36 s8/s18, vs146
s15, vs152 s16/s23) immediately invoked the guide's replacement clause and ran the steps.
The wave-35 158-s11 self-worked-out refusal is now stated text and it is being used.

### Ichorclaw pump-misread class: **0 decision-affecting; 1 decision-neutral slip**

The new mechanism note fired at the live decision: vs139 s14 trace quotes it near-verbatim
("This is for when *I* attack with it. Here I am blocking … nothing has 'become
blocked'") and blocked correctly. One residual belief slip: vs146 s23 ("So it's a 3/3
blocking a 2/2 TA") — inside an unsurvivable LETHAL where every assignment lost;
decision-neutral. All non-mandatory Ichorclaw blocks (vs152 s5, vs139 s5) were clean —
wave-35's "expect recurrence at any non-mandatory block" did NOT materialize. Attacker-side:
the "includes its +2/+2 when-blocked trigger" annotation now rides the defender's B-lines
(seen consumed correctly by deck152).

### Poison arithmetic: steady

24/24 attackers asks declared every listed creature (guide step 3); poison read off the
printed line every time. vs116: won t10 by poison with the opponent at **32 life** —
never reconciled poison against life (the deck's core lesson, executed). vs36: won t15 by
poison at opp 20 life. The two new named exceptions both validated: 5-life holdback armed
3× (vs139 s23 life 2, vs152 s28 life 4 — in both, no block-set could survive the return,
so attack-all was the exception's own arithmetic; adherent) and the cast-skip exception's
shape (attack first when poison+power ≥ 10) held in the winning turns. Relic Putrescence
own-artifact declines: 4/4 on the annotation (vs116 s13/s15, vs146 s13/s14 — the positive
control still at 100%). Mulligans 6/6 guide-legal keeps.

## BOTH seats — damage-target verdict lines: **no coverage**

"SURVIVES (toughness" / "DIES" rendered **zero times corpus-wide** (all 42 files): no
damage-spell/ability target ask arose in this pool's games. The lane-C feature is
unexercised — no confusions, no counterexample, prediction untestable this corpus.

## New defects / docket items (with repros)

1. **payRepeatModeNote never fires live** — case mismatch (`"Add "` vs rendered
   "add N counters") and possibly wrong seam (no "Choose one mode" header on the live
   valor ask). Repro: any of the 4 menus above. Fix + re-case the PARSETEST fixtures to
   the live casing (the green gate tested the wrong surface).
2. **Lane-A item 1's caveat confirmed — ctx tap-state unreadable at the pay/tap seam.**
   139-vs-105 s6: the ask's own board line reads "Stomping Ground **[tapped]**" (Grazer
   put), yet the menu still model-called with the unconditional "pay 2 life - enters
   UNTAPPED" option; the pilot paid 2 life for nothing and the false "entered untapped
   (you paid 2 life)" narration recurred (also 139-vs-146 s12-class). Per the lane-A
   plan: move the fix to the menu-arming flow. (The wave-35 fallback cluster at this ask
   class did NOT recur — 0 fallbacks; pilots now pay through the lie instead of
   derailing, so the cost is 2 life + a false narration per occurrence.)
3. deck139 mutate menu: the corpus's one "template error" trace (139-vs-105 s21) is a
   mutate-stakes confusion (both 6/6, "over or under doesn't change P/T") — 139-lane
   review material, not a trade-line item.
4. deck152 guide gaps for the next revision: (a) trade-trust silent-drift variant —
   consider requiring the echo of the parenthetical in the block answer or a guide line
   "if your conclusion contradicts the parenthesis you just quoted, the parenthesis
   wins"; (b) RETURN-SWING recall failure — the check lives mid-rule-2 as an indented
   bullet; promote it or (better, perception-routing) render an attackers-ask fact when
   life ≤ biggest untapped enemy power; add a growth term (their +1/+1 counters) mirroring
   deck105 step 2; (c) one sentence resolving Katilda-vs-infect-block precedence.

## Verdicts

- **deck105: FREEZE.** This is the clean corpus the wave-35 review asked for: numeric core
  17/17, hint scoping 0 overrides, Ichorclaw note applied at the live decision (one
  decision-neutral slip), poison arithmetic and both new exceptions validated, 0
  fallbacks, losses all non-chargeable combat races. Remaining risk is render-lane
  (optional: annotate Ichorclaw's own B-line "as a BLOCKER it stays 1/1"), not guide-lane.
- **deck152: NOT freeze — one more revision + validation corpus.** The trade-trust
  teach+note cut the class 3→1 but the silent-drift variant survives (vs158 s8), and the
  RETURN-SWING teach went 0/1 armed on a game-losing miss (vs36 s22) with a calibration
  near-miss (vs158, no growth term). Both fixes are small wording/placement changes; the
  rest of the guide (mana trust, deploy floor, FA discipline, plan-memo, day/night,
  mulligan) executed at very high fidelity.
