# deck27 (Zombies UB tribal) — wave-21 findings

**Seat:** wave-20 surgical revision deployed (late-stall durdle off-case). Binary /tmp/wagic-3be7f5a67.
Corpus matchups-20260723-084938, 6 games round-robin (pool UNCHANGED from wave-20 — repeated-opponent
control). **Record 5/6 (up from 3/6).** Guide byte-identical to wave-20 (md5 de216c5a…, unchanged).

## Record — 5/6 (W vs102/14/49/62/135; L vs35)

| vs  | wave-20 | wave-21 | life (me/opp) | turn | shape |
|-----|---------|---------|---------------|------|-------|
| 102 | L (adj) | **WIN**  | 19 / -1  | 13 | Inkfathom unblockable from t4; **no Tourach drawn** (opponent variance) |
| 14  | L @cap  | **WIN**  | 16 / -11 | 15 | Inkfathom + ACTIVE islandwalk (opp on Islands) — evasion drew this time |
| 49  | WIN     | **WIN**  | 14 / -2  | 13 | Zombie Outlander (pro) + Lord of Atlantis + Inkfathom beatdown |
| 62  | L       | **WIN**  | 20 / 0   | 9  | Zombie Outlander pro-green + Inkfathom + Glen; my life untouched |
| 35  | WIN     | **LOSS** | -45 / 18 | 10 | Sliver god-draw: six 9-10/7-8 tramplers = ~56 trample; overkill |
| 135 | WIN     | **WIN**  | 19 / -4  | 11 | Vodalian pro-green islandwalk beatdown |

**Tax gauge: 135 decisions, 0 fallbacks (0.0%), every decision returned a model reply.** Wave-20 was
138 / 3 (2.2%). **The reasoning tax COLLAPSED at this seat this corpus** — the wave-20 three
fallbacks (vs49 s16 ahead-degeneration, vs62 s6 wither-induced spiral, vs62 s18 behind-spiral) all
had a candidate recurrence this corpus and NONE broke a decision. Answer-first (item b) is the
mechanism; the wither fix (item g) removed one confusion source.

---

## OWNED VALIDATION — Wither trade annotations (wave-21 item g): FIX CONFIRMED, ZERO false positives

Complete set of Oona's Gatewarden block pairings this corpus (Oona has DEFENDER -> only ever a
blocker; all 3 pairings verified against real lethality math — Oona is a 2/1, power 2, so its 2
wither damage = two -1/-1 counters and it KILLS an attacker only when attackerToughness <= 2 or by
deathtouch):

1. **vs102 s6 (t3, 20/18)** — verbatim:
   `B1. Oona's Gatewarden (2/1) [flying, defender, wither] - may block A1 (both die)` vs
   `A1. Elvish Doomsayer (1/1)`. **CORRECT**: blockerPower 2 >= attackerToughness 1 -> attacker dies;
   Oona (1 toughness) takes 1 -> dies. Genuine both-die. Chosen: Oona blocks (good — kills a 1/1
   token-engine for free-ish). No spiral.

2. **vs49 s4 (t2, 14/20)** — verbatim:
   `B1. Oona's Gatewarden (2/1) [flying, defender, wither] - may block A1 (both die)` vs
   `A1. Dragonmaster Outcast (1/1)`. **CORRECT**: 2 >= 1 -> both die. Chosen: Oona blocks (strong —
   kills a Dragonmaster Outcast before it makes 5/5 Dragons). No spiral.

3. **vs35 s20 (t10, 12/18) — THE positive validation of the fix** — verbatim (all six pairings):
   `B1. Oona's Gatewarden (2/1) [flying, defender, wither] - may block
   A1 (your blocker dies, attacker lives, 8 tramples to your face (wither shrinks it to 7/5)),
   A2 (your blocker dies, attacker lives, 8 tramples to your face (wither shrinks it to 7/5)),
   A3 (your blocker dies, attacker lives, 9 tramples to your face (wither shrinks it to 8/6)),
   A4 (your blocker dies, attacker lives, 8 tramples to your face (wither shrinks it to 7/5)),
   A5 (your blocker dies, attacker lives, 9 tramples to your face (wither shrinks it to 8/6)),
   A6 (your blocker dies, attacker lives, 9 tramples to your face (wither shrinks it to 8/6))`
   vs six Slivers (A1/A2/A4 = 9/7, A3/A5/A6 = 10/8, all trample). **CORRECT on every axis**:
   - blockerPower 2 < attackerToughness 7/8 -> "attacker lives" (NOT "both die"). This is EXACTLY
     the wave-20 false-positive class — a >2-toughness attacker Oona cannot kill. Wave-20's annotator
     would have printed "(both die)" for all six creatures Oona physically cannot kill.
   - shrink math: 9/7 -> 7/5, 10/8 -> 8/6 (-2/-2 from 2 wither). Correct on all six.
   - trample math: power - Oona's 1 toughness (9->8, 10->9). Correct on all six.
   - Chosen: **no blockers** — correct; chumping a lone 2/1 into one 9/7 trampler saves 1 life while
     surrendering the only flying wall against a lethal board. No spiral, no fallback.

**The wave-20 named false positives (vs62 Birds 3/4, vs135 Treefolk 8/8, vs102 Liege 1/3) did not
recur as pairings** (different opponent draws), so they cannot be re-cited — but the fix's correctness
is positively demonstrated by vs35 s20, a strictly harder case (six survivors, toughness 7-8, with
trample+shrink math), all annotated correctly. **Crucially the wave-20 vs62 s6 fallback was DIRECTLY
CAUSED by a "(both die)" false positive; this corpus has 0 fallbacks and 0 wither-induced confusion.**
Non-Oona blocker annotations also correct: vs135 s5 Vodalian 2/2 vs Icehide Golem 2/2 `(both die)` OK;
vs14 s13 Zombie Master 2/3 vs Merfolk 1/1 `(you kill it, your blocker lives)` OK (taken). **Item g:
VALIDATED — close it.**

---

## Attribution of the 3/6 -> 5/6 jump (honest decomposition)

**Dominant driver: opponent-field / draw variance.** Repeated-opponent control corpus, so flips
isolate to what changed in the DRAWS, and every flip has a clean variance explanation:

- **vs102 (L->W):** wave-20 lost to Tourach, Dread Cantor (7/6 pro-B/G/W — unanswerable by this deck).
  This corpus the opponent never presented that threat; the seat ran Inkfathom evasion (s10/s13
  unblockable) and closed 18->-1. Variance.
- **vs62 (L->W, my life 20 untouched):** wave-20 lost to a ground-swarm+trample with behind-spirals.
  This corpus the seat drew **Zombie Outlander (protection from green)** into a green deck — an
  unblockable, unkillable clock — plus Inkfathom (unblockable) and Glen (flying). Never behind (opp
  20->18->10->0), so the wave-20 behind-spiral class had no state to fire in. Draw variance + evasion.
- **vs14 (L@cap->W t15) — THE designated grind matchup:** wave-20 durdled to the turn cap under an
  EVASION DROUGHT (no Inkfathom/islandwalk; lone Glen vs walled ground). This corpus the seat **drew
  Inkfathom (unblockable) from t5 AND the opponent was on Islands so islandwalk was ACTIVE** — every
  attacker option carried [unblockable]/[islandwalk]/[swampwalk] (s7/s11/s17/s21/s26/s34). Swung the
  full evasive board every step, 20->1->dead. **Evasion-draw variance, not the late-stall revision.**
- **vs35 (W->L, -45):** deck35 drew a Sliver god-hand — six 9-10 power tramplers by t10 for ~56
  trample. The seat played on-curve and correctly declined the hopeless block (s20). Opponent
  god-draw variance; not pilot error.

**Second, real, non-variance contribution: ANSWER-FIRST (item b).** 0 fallbacks vs wave-20's 3. All
135 decisions emitted a clean coded first line (100% of ask/attackers/blockers/priority replies start
with `CHOICE:/ATTACK:/BLOCKS:`). Concrete saves:
  - **vs102 s19 (16.9k chars, longest reply corpus-wide):** rambled to 16.9k but line 1 `ATTACK: A1`
    -> parsed clean to Zombie Master. Wave-20's equivalent giants truncated to fallbacks.
  - **vs14 s2 (13.9k, truncated mid-sentence** "...cast a spell that costs"): line 1 `CHOICE: 1 (Play
    Swamp)` saved it. In wave-20 this is an unparsed_reply fallback.
  - **vs135 s18 (13k, degenerated tail):** watch item below — a genuine repetition loop that in
    wave-20 caused a fallback, now harmless because line 1 was already `CHOICE: 0 (pass)`.
None of the wave-20 fallbacks were themselves game-losing, so the fallback->win causal link is weak;
answer-first's contribution is robustness (no truncation ever cost a decision), not directly the +2.

**Third: the wither fix removed one confusion source** (vs62 s6-class spirals) — supporting, small.

**NOT a contributor: the late-stall durdle revision (unvalidated — see below).**

Honest verdict: **the jump is mostly variance (3 favorable flips, 1 unfavorable, in a 6-game
repeated-opponent sample), on top of a genuinely more robust decision surface (0 fallbacks).** The
evasion/protection win-path — BASE-guide, taught since wave-19 — executed correctly in all 5 wins. The
record is a coin-flip surface at this N; the robustness gain is the durable signal.

---

## Late-stall durdle revision (wave-20's one surgical change): NOT VALIDATED — off-case did not recur

The revision targets a specific state: a walled ground stall where the opponent has STOPPED attacking,
the seat's life is stable, and it holds buffed attackers back -> loses on the clock (wave-20 vs14 s30:
held Lord of Atlantis 3/3 + Lord of the Undead 4/4 while chipping 2/turn to the cap). **That state did
not occur this corpus.** vs14, the one grind, was won by evasion draw — the seat never entered a
walled stall (every attacker had evasion) and never held an eligible attacker (every attackers record
chose ALL offered attackers). Behavior was anti-durdle-consistent (attacked every turn), but the
operative mechanism was evasion, not the revision's "swing your whole board even into blockers / lone
flyer for 2" prose. **The revision remains unexercised. Re-flag for a future corpus with a genuinely
walled ground AND an evasion drought** — the exact draw wave-20 had and this corpus did not.

---

## Wave-20 watch items — recurrence check

- **Free favorable-block DECLINED (model-obedience, 2nd instance wave-20): DID NOT RECUR.** The only
  `(you kill it, your blocker lives)` offered this corpus (vs14 s13, Zombie Master vs Merfolk 1/1) was
  TAKEN. The wave-20 trigger (Bloodmark Mentor vs49) did not arise (different vs49 draw). No 3rd
  instance — stays a WATCH, unpromoted.

- **Ahead-position degeneration LOOP: RECURRED — 3rd instance — but DEFUSED by answer-first.**
  **vs135 s18 (priority, AHEAD 19/8, 13k chars):** line 1 `CHOICE: 0 (pass)` (correct — opp's only
  creature tapped, nothing to respond to), then the tail degenerated into a literal repetition loop:
  "...I have 4 lands. I can play one land. I have 4 lands. I can play one land." to truncation. Same
  decode-layer failure as wave-19 deck110 s20 and wave-20 deck27 vs49 s16 — the **3rd instance**,
  crossing the wave-20 skill.md threshold. **Caused NO fallback**: answer-first locked the decision on
  line 1 before the tail degenerated. Strongest single validation of item b — the exact wave-20
  fallback mechanism recurred and was neutralized. Routing: decode ledger (notes.md), NOT guide; its
  harm is now defused, so a decode-rung's priority DROPS. Item j: repetition_penalty OFF this corpus —
  enabling it would likely kill the loop entirely.
  - For contrast, the wave-20 vs49 s16 slot did NOT degenerate this corpus (vs49 s16 priority, ahead
    14/12, 5.9k, coherent planning tail). The loop is stochastic, not slot-bound.

## Answer-first: last-CHOICE-wins mind-changes (item b/c interaction) — benign

5 replies emitted >1 coded line (vs14 s16/s19, vs49 s6, vs35 s18, vs135 s9). In 4 the first==last (the
2nd coded line a restatement inside PLAN). **vs35 s18 is the one genuine first!=last:** line 1
`CHOICE: 0 (pass)`, then the model reasoned to a final `CHOICE: 1 (Put in Hand … Vodalian)` —
recurring a pro-green body while behind. Parser "last well-formed CHOICE wins" honored the revision
(chosen=Put-in-Hand); the revised play is sound. Answer-first thus functions as a truncation-safe
first draft the model may improve, with last-wins honoring completed reasoning. No degradation from
committing-before-reasoning observed. Flagged to synthesis as a healthy interaction, not a defect.
